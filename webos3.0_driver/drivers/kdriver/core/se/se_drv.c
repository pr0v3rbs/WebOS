/*
SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
Copyright(c) 2013 by LG Electronics Inc.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
version 2 as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.
*/

/** @file
 *
 *  main driver implementation for se device.
 *	se device will teach you how to make device driver with new platform.
 *
 *  author		stan.kim@lge.com
 *  version		1.0
 *  date		2012.05
 *  note		Additional information.
 *
 *  @addtogroup lg_se
 *	@{
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/rmap.h>
#include <linux/cdev.h>
#include <linux/platform_device.h>
#include <linux/sched.h>
#include <linux/kthread.h>
#include <linux/dma-mapping.h>
#include <linux/dmaengine.h>
#include <linux/completion.h>

#include <asm/io.h>
#include <asm/memory.h>
#include <asm/uaccess.h>
#include <asm/cacheflush.h>

#include "os_util.h"
#include "base_device.h"
#include "debug_util.h"

#include "se_cfg.h"
#include "se_drv.h"
#include "se_kapi.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
#define SE_MODULE	"se"

#define SE_TZ_THREAD_PRIORITY	92

#define SE_TZ_SCHED_WAIT		100		// 100usec
#define SE_USE_YIELD_AS_WAIT	1

#define SE_WAIT					100		// 100usec
#define SE_WAIT_RETRY			50000	// Max : SE_DMA_WAIT_RETRY * SE_DMA_WAIT = 5sec
#define SE_WAIT_LOOP			0x1000000	// > DMA 10MBytes

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define SE_ERROR(fmt, args...)   DBG_PRINT(_gSeInfo.debugFd, LX_LOGM_LEVEL_ERROR, fmt, ##args)
#define SE_WARN(fmt, args...)    DBG_PRINT(_gSeInfo.debugFd, LX_LOGM_LEVEL_WARNING, fmt, ##args)
#define SE_NOTI(fmt, args...)	DBG_PRINT(_gSeInfo.debugFd, LX_LOGM_LEVEL_NOTI, fmt, ##args)
#define SE_INFO(fmt, args...)	DBG_PRINT(_gSeInfo.debugFd, LX_LOGM_LEVEL_INFO, fmt, ##args)
#define SE_DEBUG(fmt, args...)	DBG_PRINT(_gSeInfo.debugFd, LX_LOGM_LEVEL_DEBUG, fmt, ##args)
#define SE_TRACE(fmt, args...)   DBG_PRINT(_gSeInfo.debugFd, LX_LOGM_LEVEL_TRACE, fmt, ##args)

#define SE_CHECK_RESULT(arg, result)	if (arg) {\
											SE_ERROR("%s(%d)\n", __FILE__, __LINE__);\
											return (result);\
										}

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/
typedef enum {
	TZ_SUCCESS					= 8000,
	TZ_TIMER_SCHED				= TZ_SUCCESS + 4,
} TZ_RETURN_T;

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/
static int _SE_Open(struct inode *inode, struct file *filp);
static int _SE_Close(struct inode *inode, struct file *filp);
static int _SE_MMAP(struct file *file, struct vm_area_struct *vma);
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36))
static int _SE_IOCTL(struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg);
#endif
static long _SE_UNLOCKED_IOCTL(struct file *file, unsigned int cmd, unsigned long arg);

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/

// SE Device Info
static struct _SeInfo_tag {
	UINT32 chip;
	UINT32 platform;

	int debugFd;

	// For TZ Operation
	struct task_struct *pTzOperationThread;

	struct completion compTzOperationStart;
	struct completion compTzOperationEnd;

	UINT32 tzArgs[8];
	int tzResult;

	UINT8 *pWaitMap;
	UINT32 *pWait;
#if (SE_USE_YIELD_AS_WAIT==0)
	wait_queue_head_t waitQueue;
#endif

	BOOLEAN bCachedMap;
	BOOLEAN bResume;

	OS_SEM_T lock;	// Operation Lock

	// For Device
	dev_t devNum;
	struct cdev cDev;
	struct file_operations fileOperations;
} _gSeInfo = {
	.debugFd			= -1,
	.pTzOperationThread = NULL,

	.pWaitMap			= NULL,
	.pWait				= NULL,

	.bCachedMap			= FALSE,
	.bResume			= FALSE,

	.fileOperations 	= {
		.open			= _SE_Open,
		.release		= _SE_Close,
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36))
		.ioctl			= _SE_IOCTL,
#else
		.unlocked_ioctl = _SE_UNLOCKED_IOCTL,
#endif
		.mmap			= _SE_MMAP,
		.read			= NULL,
		.write			= NULL,
	},
};

/*========================================================================================
    Implementation Group : SE CDEV
========================================================================================*/

static void _TzCmd(void)
{
#ifdef __arm__
	isb();
	dsb();
	dmb();

	asm volatile (
        "LDM    %1, {r0-r7}\n"
        ".word  0xe1600070 @SMC 0\n"
        "MOV    %0, r0\n"
        : "=r" (_gSeInfo.tzArgs[0])
        : "r"  (_gSeInfo.tzArgs)
        : "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r12"
    );
#endif
}

static int _RunTzCmd(void)
{
	do {
		_TzCmd();
		if ((TZ_RETURN_T)_gSeInfo.tzArgs[0]==TZ_TIMER_SCHED) {
#if (SE_USE_YIELD_AS_WAIT==0)
			/*
			; Set current process state to interruptible sleep.
			*/
			set_current_state(TASK_INTERRUPTIBLE);

			/*
			; Take a nap and wake up after specified time.
			*/
			if (schedule_timeout(usecs_to_jiffies(SE_TZ_SCHED_WAIT))<0)	// Just Check
				SE_ERROR("Negative return in schedule_timeout()\n");
#else	// for performance
			yield();
#endif
		}
	} while ((TZ_RETURN_T)_gSeInfo.tzArgs[0]==TZ_TIMER_SCHED);

	if ((TZ_RETURN_T)_gSeInfo.tzArgs[0]==TZ_SUCCESS)
		return 0;

	SE_NOTI("TZ Return (0x%x)\n", _gSeInfo.tzArgs[0]);
	return -1;
}

static int _tzOperationThread(void *pArg)
{
	while (!kthread_should_stop()) {
		wait_for_completion(&_gSeInfo.compTzOperationStart);
		init_completion(&_gSeInfo.compTzOperationStart);

		_gSeInfo.tzResult = _RunTzCmd();

		complete(&_gSeInfo.compTzOperationEnd);
	}
	SE_NOTI("Stop thread\n");

	return 0;
}

static int _runTzOperation(unsigned long arg)
{
	SE_CHECK_RESULT(copy_from_user((void *)_gSeInfo.tzArgs, (void __user *)arg, sizeof(_gSeInfo.tzArgs)), -EIO);

	complete(&_gSeInfo.compTzOperationStart);
	wait_for_completion(&_gSeInfo.compTzOperationEnd);
	init_completion(&_gSeInfo.compTzOperationEnd);

	SE_CHECK_RESULT(copy_to_user((void __user *)arg, (void *)_gSeInfo.tzArgs, sizeof(_gSeInfo.tzArgs)), -EIO);
	if (_gSeInfo.tzResult)
		return -EIO;

	return 0;
}

static int _wait(unsigned long arg)
{
	UINT32 pageAddr;
	int i;

	if (_gSeInfo.pWaitMap==NULL) {
		SE_CHECK_RESULT(arg==0, -EIO);

		pageAddr = arg & (~(PAGE_SIZE-1));
		_gSeInfo.pWaitMap = ioremap(pageAddr, PAGE_SIZE);
		SE_CHECK_RESULT(_gSeInfo.pWaitMap==NULL, -EIO);

		_gSeInfo.pWait = (UINT32 *)(_gSeInfo.pWaitMap + (arg - pageAddr));
#if (SE_USE_YIELD_AS_WAIT==0)
		init_waitqueue_head(&_gSeInfo.waitQueue);
#endif
	}

#if (SE_USE_YIELD_AS_WAIT==0)
	for (i=0; i<SE_WAIT_RETRY; i++) {
		if (*_gSeInfo.pWait==0) {
			return 0;
		}
		wait_event_timeout_interruptible(_gSeInfo.waitQueue, (*_gSeInfo.pWait==0), usecs_to_jiffies(SE_WAIT));
	}
#else	// for performance
	for (i=0; i<SE_WAIT_LOOP; i++) {
		if (*_gSeInfo.pWait==0) {
			return 0;
		}
		yield();
	}
#endif

	SE_ERROR("Time out!\n");
	return -EIO;
}

static int _getMemInfo(unsigned long arg)
{
	SE_CHECK_RESULT(copy_to_user((void __user *)arg,
		(void *)gpMemCfgSE, sizeof(LX_MEMCFG_T)), -EIO);
	SE_CHECK_RESULT(copy_to_user((void __user *)arg + sizeof(LX_MEMCFG_T),
		(void *)gpMemCfgSE_TZFW, sizeof(LX_MEMCFG_T)), -EIO);

	return 0;
}

static int _cacheCtrl(unsigned long arg)
{
	SE_CACHE_CTRL_T cacheCtrl;

	SE_CHECK_RESULT(copy_from_user((void *)&cacheCtrl, (void __user *)arg, sizeof(SE_CACHE_CTRL_T)), -EIO);

	switch (cacheCtrl.operation) {
		case SE_CACHE_MAP :
			_gSeInfo.bCachedMap = TRUE;
			break;

		case SE_CACHE_INVALIDATE :
#ifdef __aarch64__
			dma_map_single(NULL,cacheCtrl.vaddr, cacheCtrl.size, DMA_FROM_DEVICE);
#else
			dmac_map_area(cacheCtrl.vaddr, cacheCtrl.size, DMA_FROM_DEVICE);//v7_dma_inv_range
			outer_inv_range((phys_addr_t)cacheCtrl.paddr, (phys_addr_t)cacheCtrl.paddr + cacheCtrl.size);
#endif
			break;

		case SE_CACHE_CLEAN :
#ifdef __aarch64__
			dma_map_single(NULL,cacheCtrl.vaddr, cacheCtrl.size, DMA_TO_DEVICE);
#else
			dmac_map_area(cacheCtrl.vaddr, cacheCtrl.size, DMA_TO_DEVICE);//v7_dma_clean_range
			outer_clean_range((phys_addr_t)cacheCtrl.paddr, (phys_addr_t)cacheCtrl.paddr + cacheCtrl.size);
#endif
			break;

		default :
			SE_ERROR("Not defined(%d)\n", cacheCtrl.operation);
			return -EINVAL;
	}

	return 0;
}

static long _SE_UNLOCKED_IOCTL(struct file *file, unsigned int cmd, unsigned long arg)
{
	long result = -ENOTTY;

	OS_LockMutex(&_gSeInfo.lock);

	switch (cmd) {
		case SE_IORW_RUN_TZ_OP :
			result = _runTzOperation(arg);
			break;

		case SE_IOR_WAIT :
			result = _wait(arg);
			break;

		case SE_IOW_CACHE_CTRL :
			result = _cacheCtrl(arg);
			break;

		case SE_IOW_GET_MEM_INFO :
			result = _getMemInfo(arg);
			break;

		case SE_IO_CHECK_RESUME :
			if (_gSeInfo.bResume==TRUE)
				result = 0;	// Return SUCCESS
			_gSeInfo.bResume = FALSE;	// Clean Resume Flag
			break;

		default :
			SE_ERROR("Not defined(%d)\n", cmd);
			break;
	}

	OS_UnlockMutex(&_gSeInfo.lock);

	return result;
}

#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36))
static int _SE_IOCTL(struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg)
{
	return (int)_SE_UNLOCKED_IOCTL(file, cmd, arg);
}
#endif

static int _SE_MMAP(struct file *file, struct vm_area_struct *vma)
{
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,10,0))
		vma->vm_flags |= VM_RESERVED;//to prevent swapout
#else
		vma->vm_flags |= VM_IO|VM_DONTEXPAND|VM_DONTDUMP;//to prevent swapout
#endif

	if (_gSeInfo.bCachedMap==FALSE)
		vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);
	_gSeInfo.bCachedMap = FALSE;

	return remap_pfn_range(vma, vma->vm_start, vma->vm_pgoff,
		vma->vm_end - vma->vm_start, vma->vm_page_prot);
}

static int _SE_Open(struct inode *inode, struct file *filp)
{
	return 0;
}

static int _SE_Close(struct inode *inode, struct file *filp)
{
	return 0;
}

static int _SE_Suspend(struct platform_device *pdev, pm_message_t state)
{
#ifdef KDRV_CONFIG_PM_SE
	SE_NOTI("Suspend\n");
#endif
	return 0;
}

static int _SE_Resume(struct platform_device *pdev)
{
#ifdef KDRV_CONFIG_PM_SE
	SE_NOTI("Resume\n");
#endif
	_gSeInfo.bResume = TRUE;

	return 0;
}

static int _SE_Probe(struct platform_device *pdev)
{
	SE_NOTI("Probe\n");

	return 0;
}

static int  _SE_Remove(struct platform_device *pdev)
{
	SE_NOTI("Remove\n");

	return 0;
}

static void _SE_Release(struct device *dev)
{
	SE_NOTI("Release\n");
}

/*
 *	module platform driver structure
 */
static struct platform_driver _se_driver =
{
	.probe		= _SE_Probe,
	.suspend	= _SE_Suspend,
	.remove		= _SE_Remove,
	.resume		= _SE_Resume,
	.driver		= {
		.name = SE_MODULE,
	},
};

static struct platform_device _se_device = {
	.name = SE_MODULE,
	.id = -1,
	.dev = {
		.release = _SE_Release,
	},
};

/*========================================================================================
    Implementation Group : SE Module
========================================================================================*/
/** Initialize the device environment before the real H/W initialization
 *
 *  @note main usage of this function is to initialize the HAL layer and memory size adjustment
 *  @note it's natural to keep this function blank :)
 */
void SE_PreInit(void)
{
    /* TODO: do something */
}

int SE_Init(void)
{
	/* Get the handle of debug output for se device.
	*
	* Most module should open debug handle before the real initialization of module.
	* As you know, debug_util offers 4 independent debug outputs for your device driver.
	* So if you want to use all the debug outputs, you should initialize each debug output
	* using OS_DEBUG_EnableModuleByIndex() function.
	*/
	_gSeInfo.debugFd = DBG_OPEN(SE_MODULE);
	if (_gSeInfo.debugFd < 0) {
		printk("[SE] can't get debug handle\n");
	}
	else {
		OS_DEBUG_EnableModule(_gSeInfo.debugFd);
		OS_DEBUG_EnableModuleByIndex(_gSeInfo.debugFd, LX_LOGM_LEVEL_ERROR, DBG_COLOR_RED );		// error
		OS_DEBUG_EnableModuleByIndex(_gSeInfo.debugFd, LX_LOGM_LEVEL_WARNING, DBG_COLOR_YELLOW );	// warn
		OS_DEBUG_EnableModuleByIndex(_gSeInfo.debugFd, LX_LOGM_LEVEL_NOTI, DBG_COLOR_NONE );		// noti
		OS_DEBUG_EnableModuleByIndex(_gSeInfo.debugFd, LX_LOGM_LEVEL_INFO, DBG_COLOR_NONE );		// info
		OS_DEBUG_EnableModuleByIndex(_gSeInfo.debugFd, LX_LOGM_LEVEL_DEBUG, DBG_COLOR_NONE );		// debug
		OS_DEBUG_EnableModuleByIndex(_gSeInfo.debugFd, LX_LOGM_LEVEL_TRACE, DBG_COLOR_GREEN );		// trace
		//OS_DEBUG_EnableModuleByIndex ( g_gfx_debug_fd, LX_LOGM_LEVEL_TRACE + 1, DBG_COLOR_NONE );		// user macro
	}

	_gSeInfo.chip = lx_chip_rev();
	_gSeInfo.platform = lx_chip_plt();

	SE_InitCfg();

#if 0	// Currently Not Used
	switch (_gSeInfo.platform) {
		case LX_CHIP_PLATFORM_GP :
			break;

		case LX_CHIP_PLATFORM_COSMO :
			break;

		default :
			SE_WARN("Unknown Platform Setting : Use default\n");
			break;
	}

	switch (_gSeInfo.chip) {
		case LX_CHIP_REV(L9,A0):
		case LX_CHIP_REV(L9,A1):
		case LX_CHIP_REV(L9,B0):
		case LX_CHIP_REV(L9,B1):
		case LX_CHIP_REV(H13,A0):
		case LX_CHIP_REV(H13,A1):
		case LX_CHIP_REV(H13,B0):
		case LX_CHIP_REV(M14,A0):
		case LX_CHIP_REV(M14,A1):
		case LX_CHIP_REV(H14,A0):
			break;

		default :
			SE_WARN("Unknown Chip Setting : Use default\n");
			break;
	}
#endif

	// for Sync of tz operation thread
	init_completion(&_gSeInfo.compTzOperationStart);
	init_completion(&_gSeInfo.compTzOperationEnd);

	// for Operation Lock
	OS_InitMutex(&_gSeInfo.lock, OS_SEM_ATTR_DEFAULT);

	// Thread create
	_gSeInfo.pTzOperationThread = kthread_create(_tzOperationThread, NULL, "setzkthread");
	if (IS_ERR(_gSeInfo.pTzOperationThread)) {
		SE_ERROR("while kthread_create()\n");
		_gSeInfo.pTzOperationThread = NULL;
		return -EIO;
	}

	// Bind se_tz_kthread to CPU0
	kthread_bind(_gSeInfo.pTzOperationThread, 0);	// Bind setzkthread to CPU0

	// Set Real-Time Priority
	{
		struct sched_param param = { 0 };

		param.sched_priority = SE_TZ_THREAD_PRIORITY;
		if (sched_setscheduler(_gSeInfo.pTzOperationThread, SCHED_FIFO, &param)!=0) {
			SE_ERROR("Can't change priority\n");
		}
	}

	// Start thread
	wake_up_process(_gSeInfo.pTzOperationThread);

	// Register Driver
	if (platform_driver_register(&_se_driver)) {
		SE_ERROR("platform_driver_register error\n");
	}
	else {
		if (platform_device_register(&_se_device)) {
			SE_ERROR("platform_device_register error\n");
			platform_driver_unregister(&_se_driver);
		}
 	}

	// Register Device
	_gSeInfo.devNum = MKDEV(SE_MAJOR, SE_MINOR);
	SE_CHECK_RESULT(register_chrdev_region(_gSeInfo.devNum, 1, SE_MODULE), -EIO);
	cdev_init(&_gSeInfo.cDev, &_gSeInfo.fileOperations);
	SE_CHECK_RESULT(cdev_add(&_gSeInfo.cDev, _gSeInfo.devNum, 1), -EIO);
	OS_CreateDeviceClass(_gSeInfo.devNum, "%s%d", SE_MODULE, 0);

	return 0;
}

void SE_Cleanup(void)
{
	// Stop Thread
	if (_gSeInfo.pTzOperationThread!=NULL) {
		kthread_stop(_gSeInfo.pTzOperationThread);
		complete(&_gSeInfo.compTzOperationStart);
		_gSeInfo.pTzOperationThread = NULL;
	}

	// iounmap
	if (_gSeInfo.pWaitMap!=NULL)
		iounmap(_gSeInfo.pWaitMap);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
#if !defined(KDRV_GLOBAL_LINK) || defined(MAKE_SE_MODULE)
#if defined(CONFIG_LG_BUILTIN_KDRIVER) && defined(CONFIG_LGSNAP) && !defined(MAKE_SE_MODULE)
user_initcall_grp("kdrv",SE_Init);
#else
module_init(SE_Init);
#endif
module_exit(SE_Cleanup);

MODULE_AUTHOR("LGE");
MODULE_DESCRIPTION("SE driver");
MODULE_LICENSE("GPL");
#endif
/** @} */

