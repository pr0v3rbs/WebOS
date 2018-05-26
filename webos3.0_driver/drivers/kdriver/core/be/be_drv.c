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
 *  drv file for BE device
 *
 *  author		justine.jeong
 *  version		1.0
 *  date		2012.02.07
 *  note		Additional information.
 *
 *  @addtogroup BE
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
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/version.h>
#include <asm/uaccess.h>
#include <linux/platform_device.h>

#include "os_util.h"
#include "base_device.h"
#include "be_dbg.h"

#include "be_kapi.h"
#include "be_cfg.h"
#include "be_def.h"
#include "be_drv.h"
#include "be_reg.h"

#include "be_top_drv.h"
#include "fwl_drv.h"
#include "int_drv.h"
#include "ipc_drv.h"
#include "h3d_drv.h"
#include "frc_drv.h"
#include "sre_drv.h"
#include "led_drv.h"
#include "pwm_pe_drv.h"

#include "int_hal.h"
#include "led_hal.h"

// Temp for OIF(Vx1) : 2012.05.09 dj911.kim
#include "../sys/sys_regs.h"

#include "video_status_keeper.h"
/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
//#define USE_BE_IOCTL_MACROS

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/
/**
 *  main control block for BE device.
 *  each minor device has unique control block
 *
 */
typedef struct
{
	int				dev_open_count;		///< check if device is opened or not
	dev_t			devno;				///< device number
	struct cdev		cdev;				///< char device structure
}
BE_DEVICE_T;

typedef struct
{
	bool			is_suspended;
	unsigned int fwInfo;
}
BE_DRVDATA_T;

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/
extern int BE_FRC_Ioctl(unsigned int cmd, unsigned long arg);
extern int BE_H3D_Ioctl(unsigned int cmd, unsigned long arg);
extern int BE_LED_Ioctl(unsigned int cmd, unsigned long arg);
extern int BE_SRE_Ioctl(unsigned int cmd, unsigned long arg);
extern int BE_PWM_PE_Ioctl(unsigned int cmd, unsigned long arg);

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/
int				g_be_major = BE_MAJOR;
int				g_be_minor = BE_MINOR;
int				g_be_debug_fd;

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/
static int		BE_Open	(struct inode *, struct file *);
static int		BE_Close	(struct inode *, struct file *);
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36))
static int		BE_Ioctl	(struct inode *, struct file *, UINT32, ULONG );
#else
static long		BE_Ioctl	(struct file *, unsigned int, unsigned long );
#endif
//static int		BE_mmap	(struct file *, struct vm_area_struct *);

static int		__BE_SKEEPER_Register_Meta_Data(void);
/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
static OS_SEM_T	gs_be_top_mtx;
static OS_SEM_T	gs_be_h3d_mtx;
static OS_SEM_T	gs_be_frc_mtx;
static OS_SEM_T	gs_be_sre_mtx;
static OS_SEM_T	gs_be_pwm_pe_mtx;
static OS_SEM_T	gs_be_led_mtx;

/**
 * main control block for gfx device
 */
static BE_DEVICE_T*		g_be_device;

/**
 * file I/O description for gfx device
 */
static struct file_operations g_be_fops =
{
	.open		=	BE_Open,
	.release	=	BE_Close,
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36))
	.ioctl		=	BE_Ioctl,
#else
	.unlocked_ioctl =	BE_Ioctl,
#endif
	.read		=	NULL,
	.write		=	NULL,
};

/*========================================================================================
	Implementation Group
========================================================================================*/
/**
 * suspending module/
 *
 * @param	struct platform_device *pdev pm_message_t state
 * @return	int 0 : OK , -1 : NOT OK
 *
 */
static int BE_suspend(struct platform_device *pdev, pm_message_t state)
{
#ifdef KDRV_CONFIG_PM_BE
	BE_DRVDATA_T	*drv_data;

	printk("BE - suspend BEGIN\n");
	drv_data = platform_get_drvdata(pdev);

	drv_data->is_suspended = 1;
	if (BE_FW_GetSuspendInfo(&drv_data->fwInfo)) return -1;

	printk("BE - suspend OK\n");
#endif
	return 0;
}

/**
 * resuming module.
 *
 * @param	struct platform_device *
 * @return	int 0 : OK , -1 : NOT OK
 *
 */
static int BE_resume(struct platform_device *pdev)
{
#ifdef KDRV_CONFIG_PM_BE
	BE_DRVDATA_T *drv_data;

	printk("BE - resume START\n");
	drv_data = platform_get_drvdata(pdev);

	if (drv_data->is_suspended == 0)
	{
		printk("BE - resume FAIL\n");
	}

	if (BE_INT_HAL_IntrEnable())
	{
		printk("BE - resume FAIL\n");
	}

	if (BE_LED_HAL_Resume())
	{
		printk("BE - resume FAIL\n");
	}

	if (BE_FW_Resume(drv_data->fwInfo))
	{
		printk("BE - resume FAIL\n");
	}
	drv_data->is_suspended = 0;
	drv_data->fwInfo = 0;

	printk("BE - resume OK\n");
#endif
	return 0;
}

/**
 * probing module.
 *
 * @param	struct platform_device *pdev
 * @return	int 0 : OK , -1 : NOT OK
 *
 */
int BE_probe(struct platform_device *pdev)
{
	BE_DRVDATA_T *drv_data;

	drv_data = (BE_DRVDATA_T *)kmalloc(sizeof(BE_DRVDATA_T) , GFP_KERNEL);

	BE_PRINT("[%s] done probe\n", BE_MODULE);

	drv_data->is_suspended = 0;
	drv_data->fwInfo = 0;

	platform_set_drvdata(pdev, drv_data);

	return 0;
}

/**
 * module remove function. this function will be called in rmmod BE module
 *
 * @param	struct platform_device
 * @return	int 0 : OK , -1 : NOT OK
 *
 */
static int BE_remove(struct platform_device *pdev)
{
	BE_DRVDATA_T *drv_data;

	drv_data = platform_get_drvdata(pdev);
	kfree(drv_data);

	BE_PRINT("[%s] removed\n", BE_MODULE);

	return 0;
}

/**
 * module release function. this function will be called in rmmod BE module
 *
 * @param	struct device *dev
 * @return	int 0 : OK , -1 : NOT OK
 *
 */
static void BE_release(struct device *dev)
{
	BE_PRINT("[%s] device released\n", BE_MODULE);
}

/*
 * module platform driver structure
 */
static struct platform_driver be_driver =
{
	.probe		= BE_probe,
	.suspend	= BE_suspend,
	.remove		= BE_remove,
	.resume		= BE_resume,
	.driver		=
	{
		.name	= BE_MODULE,
	},
};

static struct platform_device be_device =
{
	.name		= BE_MODULE,
	.id			= 0,
	.dev		=
	{
		.release = BE_MODULE,
	},
};

/**
 * open handler for BE device
 *
 * @param	*inode [in] inode
 * @param	*filp [in] file
 * @return	OK if success, ERROR otherwisw,
 * @see
 * @author
 */
static int BE_Open(struct inode *inode, struct file *filp)
{
	int				major, minor;
	struct cdev*	cdev;
	BE_DEVICE_T*	my_dev;

	BE_PRINT("Attempting to open BE device...\n");

	cdev	= inode->i_cdev;
	my_dev	= container_of (cdev, BE_DEVICE_T, cdev);

	my_dev->dev_open_count++;
	filp->private_data = my_dev;

	major = imajor(inode);
	minor = iminor(inode);

	BE_PRINT("device opened (%d:%d)\n", major, minor);

	return 0;
}

/**
 * BE close
 *
 * @param	*inode [in] inode
 * @param	*file [in] file
 * @return	OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int BE_Close(struct inode *inode, struct file *file)
{
	int				major, minor;
	BE_DEVICE_T*	my_dev;
	struct cdev*	cdev;

	cdev	= inode->i_cdev;
	my_dev	= container_of( cdev, BE_DEVICE_T, cdev);

	if( my_dev->dev_open_count > 0 )
	{
		--my_dev->dev_open_count;
	}

	major = imajor(inode);
	minor = iminor(inode);

	BE_PRINT("device closed (%d:%d)\n", major, minor);
	return 0;
}

/**
 * ioctl handler for BE device.
 *
 * @param   void
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 * @note: if you have some critial data, you should protect them using semaphore or spin lock.
 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)
static int
BE_Ioctl ( struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg )
#else
static long
BE_Ioctl ( struct file *filp, unsigned int cmd, unsigned long arg )
#endif
{
    int err = 0, ret = 0;

    BE_DEVICE_T*	my_dev;
    struct cdev*		cdev;

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)
#else
		struct inode *inode = filp->f_path.dentry->d_inode;
#endif

	/* For Status Keeper */
	LX_VP_ST_KEEPER_MODULE_TYPE_T	mtype = LX_VP_ST_KEEPER_MODULE_BE;
	LX_VP_ST_KEEPER_HISTORY_TYPE_T	htype = LX_VP_ST_KEEPER_HISTORY_IO;
	LX_VP_ST_KEEPER_HISTORY_INDEX_T pData;

	/*
	 * get current BE device object
	 */
    cdev	= inode->i_cdev;
    my_dev	= container_of ( cdev, BE_DEVICE_T, cdev);

    /*
     * check if IOCTL command is valid or not.
     * - if magic value doesn't match, return error (-ENOTTY)
     * - if command is out of range, return error (-ENOTTY)
     *
     * note) -ENOTTY means "Inappropriate ioctl for device.
     */
    if (_IOC_TYPE(cmd) != BE_IOC_MAGIC)
    {
    	DBG_PRINT_WARNING("invalid magic. magic=0x%02X\n", _IOC_TYPE(cmd) );
    	return -ENOTTY;
    }
    if ((_IOC_NR(cmd)) > BE_IOC_MAXNR)
    {
    	DBG_PRINT_WARNING("out of ioctl command. cmd_idx=%d\n", _IOC_NR(cmd) );
    	return -ENOTTY;
    }

	/* TODO : add some check routine for your device */

    /*
     * check if user memory is valid or not.
     * if memory can't be accessed from kernel, return error (-EFAULT)
     */
    if (_IOC_DIR(cmd) & _IOC_READ)
        err = !access_ok(VERIFY_WRITE, (void __user *)arg, _IOC_SIZE(cmd));
    else if (_IOC_DIR(cmd) & _IOC_WRITE)
        err =  !access_ok(VERIFY_READ, (void __user *)arg, _IOC_SIZE(cmd));
    if (err)
    {
    	DBG_PRINT_WARNING("memory access error. cmd_idx=%d, rw=%c%c, memptr=%p\n",
    													_IOC_NR(cmd),
    													(_IOC_DIR(cmd) & _IOC_READ)? 'r':'-',
    													(_IOC_DIR(cmd) & _IOC_WRITE)? 'w':'-',
    													(void*)arg );
        return -EFAULT;
	}

	BE_DEBUG("cmd = %08X (cmd_idx=%d)\n",cmd, _IOC_NR(cmd) );

	/* Load Status Keeper data */
	pData.ioctl = (unsigned int)cmd;
	pData.pData = (unsigned int)arg;
	pData.size  = _IOC_SIZE(cmd);

	VIDEO_SKeeper_PushHistoryLog(mtype, htype, &pData);

	if ((_IOC_NR(cmd)) < BE_IOC_TOP_MAX)		{ret = OS_LockMutex(&gs_be_top_mtx); ret = BE_TOP_Ioctl(cmd,arg); OS_UnlockMutex(&gs_be_top_mtx);}
	else if((_IOC_NR(cmd)) < BE_IOC_H3D_MAX)	{ret = OS_LockMutex(&gs_be_h3d_mtx); ret = BE_H3D_Ioctl(cmd,arg); OS_UnlockMutex(&gs_be_h3d_mtx);}
	else if((_IOC_NR(cmd)) < BE_IOC_FRC_MAX)	{ret = OS_LockMutex(&gs_be_frc_mtx); ret = BE_FRC_Ioctl(cmd,arg); OS_UnlockMutex(&gs_be_frc_mtx);}
	else if((_IOC_NR(cmd)) < BE_IOC_SRE_MAX)	{ret = OS_LockMutex(&gs_be_sre_mtx); ret = BE_SRE_Ioctl(cmd,arg); OS_UnlockMutex(&gs_be_sre_mtx);}
	else if((_IOC_NR(cmd)) < BE_IOC_LED_MAX)	{ret = OS_LockMutex(&gs_be_led_mtx); BE_LED_Ioctl(cmd,arg); OS_UnlockMutex(&gs_be_led_mtx);}
	else if((_IOC_NR(cmd)) < BE_IOC_PWM_MAX){ret = OS_LockMutex(&gs_be_pwm_pe_mtx); ret = BE_PWM_PE_Ioctl(cmd,arg); OS_UnlockMutex(&gs_be_pwm_pe_mtx);}
	else if((_IOC_NR(cmd)) < BE_IOC_STATUS_KEEPER_MAX){ret = VIDEO_SKeeper_DBG_Ioctl(cmd,arg);}
	else BE_ERROR("Warning! Wrong IOCTL call from kadpator!\n");

    return ret;
}

/** Initialize the device environment before the real H/W initialization
 *
 *  @note main usage of this function is to initialize the HAL layer and memory size adjustment
 *  @note it's natural to keep this function blank :)
 */
void BE_PreInit(void)
{
    /* TODO: do something */
}

/**
 * init BE drv
 *
 * @param   void
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int BE_Init(void)
{
	int			i;
	int			err;
	dev_t		dev;

	/* Get the handle of debug output for BE device.
	 *
	 * Most module should open debug handle before the real initialization of module.
	 * As you know, debug_util offers 4 independent debug outputs for your device driver.
	 * So if you want to use all the debug outputs, you should initialize each debug output
	 * using OS_DEBUG_EnableModuleByIndex() function.
	 */
	if (lx_chip_rev() < LX_CHIP_REV(H13, A0))
		return 0;

	g_be_debug_fd = DBG_OPEN( BE_MODULE );
	if (g_be_debug_fd < 0) {
		DBG_PRINT_ERROR("can't open BE module\n" );
		return -EIO;
	}
	OS_DEBUG_EnableModule ( g_be_debug_fd );

	OS_DEBUG_EnableModuleByIndex ( g_be_debug_fd, BE_DBG_PRT_IDX_PRINT, DBG_COLOR_NONE ); 	// BE_PRINT
	OS_DEBUG_EnableModuleByIndex ( g_be_debug_fd, BE_DBG_PRT_IDX_ERROR, DBG_COLOR_UL_RED );	// BE_ERROR
	OS_DEBUG_DisableModuleByIndex ( g_be_debug_fd, BE_DBG_PRT_IDX_DEBUG ); 					// BE_DEBUG
	OS_DEBUG_DisableModuleByIndex ( g_be_debug_fd, BE_DBG_PRT_IDX_TRACE );					// BE_TRACE

	/* allocate main device handler, register current device.
	 *
	 * If devie major is predefined then register device using that number.
	 * otherwise, major number of device is automatically assigned by Linux kernel.
	 *
	 */

	// added by SC Jung for quick booting
	if(platform_driver_register(&be_driver) < 0)
	{
		BE_ERROR("[%s] platform driver register failed\n",BE_MODULE);

	}
	else
	{
		if(platform_device_register(&be_device))
		{
			platform_driver_unregister(&be_driver);
			BE_ERROR("[%s] platform device register failed\n",BE_MODULE);
		}
		else
		{
			BE_PRINT("[%s] platform register done\n", BE_MODULE);
		}


	}

	g_be_device = (BE_DEVICE_T*)OS_KMalloc( sizeof(BE_DEVICE_T)*BE_MAX_DEVICE );

	if ( NULL == g_be_device )
	{
		DBG_PRINT_ERROR("out of memory. can't allocate %d bytes\n", sizeof(BE_DEVICE_T)* BE_MAX_DEVICE );
		return -ENOMEM;
	}

	memset( g_be_device, 0x0, sizeof(BE_DEVICE_T)* BE_MAX_DEVICE );

	if (g_be_major)
	{
		dev = MKDEV( g_be_major, g_be_minor );
		err = register_chrdev_region(dev, BE_MAX_DEVICE, BE_MODULE );
	}
	else
	{
		err = alloc_chrdev_region(&dev, g_be_minor, BE_MAX_DEVICE, BE_MODULE );
		g_be_major = MAJOR(dev);
	}

	if ( err < 0 )
	{
		DBG_PRINT_ERROR("can't register BE device\n" );
		return -EIO;
	}



	/* END */

	for ( i=0; i<BE_MAX_DEVICE; i++ )
	{
		/* initialize cdev structure with predefined variable */
		dev = MKDEV( g_be_major, g_be_minor+i );
		cdev_init( &(g_be_device[i].cdev), &g_be_fops );
		g_be_device[i].devno		= dev;
		g_be_device[i].cdev.owner = THIS_MODULE;
		g_be_device[i].cdev.ops   = &g_be_fops;

		/* TODO: initialize minor device */


		/* END */

		err = cdev_add (&(g_be_device[i].cdev), dev, 1 );

		if (err)
		{
			DBG_PRINT_ERROR("error (%d) while adding BE device (%d.%d)\n", err, MAJOR(dev), MINOR(dev) );
			return -EIO;
		}
        OS_CreateDeviceClass ( g_be_device[i].devno, "%s%d", BE_MODULE, i );
	}

	/* TODO : initialize your module not specific minor device */
	OS_InitMutex(&gs_be_top_mtx, OS_SEM_ATTR_DEFAULT);
	OS_InitMutex(&gs_be_h3d_mtx, OS_SEM_ATTR_DEFAULT);
	OS_InitMutex(&gs_be_frc_mtx, OS_SEM_ATTR_DEFAULT);
	OS_InitMutex(&gs_be_sre_mtx, OS_SEM_ATTR_DEFAULT);
	OS_InitMutex(&gs_be_pwm_pe_mtx, OS_SEM_ATTR_DEFAULT);
	OS_InitMutex(&gs_be_led_mtx, OS_SEM_ATTR_DEFAULT);

	BE_InitCfg();
	BE_REG_Init();
	BE_IPC_Init();
	BE_INT_Init();
	BE_FW_Init();

	BE_TOP_Init();
	BE_FRC_Init();
	BE_LED_Init();
	BE_SRE_Init();
	BE_H3D_Init();
	BE_PWM_PE_Init();

	VIDEO_SKeeperInit();
	__BE_SKEEPER_Register_Meta_Data();

	BE_PRINT("BE device initialized\n");

	return 0;
}

static int		__BE_SKEEPER_Register_Meta_Data(void)
{
	int ret = RET_ERROR;
	int i;
	LX_VP_ST_KEEPER_INDEX_T *pMetaData;

	do{
		/* Step 1. Allocate temperary memory */
		pMetaData = (LX_VP_ST_KEEPER_INDEX_T *)kmalloc(sizeof(LX_VP_ST_KEEPER_INDEX_T) * BE_SKEEPER_STATUS_MAXNUM, GFP_KERNEL);

		/* Step 2. Check result of allocation*/
		if(pMetaData == NULL){
			BE_PRINT("[%s:%d] SKeeper memory allocation failed for setting meta data\n", __F__, __L__);
			break;
		}

		/* Step 3. Set in meta data */
		for(i=0; i<BE_SKEEPER_STATUS_MAXNUM; i++){
			pMetaData[i].status = 0;
			pMetaData[i].last_modified_time = 0;
			pMetaData[i].modifier = 0;
		}

		SKEEPER_INSERT_DATA(pMetaData, BE_SKEEPER_FRC_MEMC_MODE);
		SKEEPER_INSERT_DATA(pMetaData, BE_SKEEPER_FRC_LOW_DELAY_MODE);
		SKEEPER_INSERT_DATA(pMetaData, BE_SKEEPER_FRC_RGB_YUV_MODE);
		SKEEPER_INSERT_DATA(pMetaData, BE_SKEEPER_FRC_FREEZE);
		SKEEPER_INSERT_DATA(pMetaData, BE_SKEEPER_H3D_3D_MODE);
		SKEEPER_INSERT_DATA(pMetaData, BE_SKEEPER_H3D_AUTO_CONVERGENCE);
		SKEEPER_INSERT_DATA(pMetaData, BE_SKEEPER_H3D_2D_TO_3D_DEPTH);
		SKEEPER_INSERT_DATA(pMetaData, BE_SKEEPER_H3D_3D_DEPTH);
		SKEEPER_INSERT_DATA(pMetaData, BE_SKEEPER_H3D_CONVERGENCE);
		SKEEPER_INSERT_DATA(pMetaData, BE_SKEEPER_H3D_LR_TOGGLE);

		/* Step 4. List up Index to Status Keeper */
		ret = VIDEO_SKeeperListUpIndex(LX_VP_ST_KEEPER_MODULE_BE, pMetaData, BE_SKEEPER_STATUS_MAXNUM);
		if(ret < 0){
			BE_PRINT("[%s:%d] SKeeper listup Index failed\n", __F__, __L__);
		}

		kfree((void *)pMetaData);
	}while(0);

	return ret;
}


/**
 * BE cleanup
 *
 * @param   void
 * @return  void
 * @see
 * @author
 */
void BE_Cleanup(void)
{
	int i;
	dev_t dev = MKDEV( g_be_major, g_be_minor );

	platform_driver_unregister(&be_driver);
	platform_device_unregister(&be_device);

	/* remove all minor devicies and unregister current device */
	for ( i=0; i<BE_MAX_DEVICE;i++)
	{
		/* TODO: cleanup each minor device */


		/* END */
		cdev_del( &(g_be_device[i].cdev) );
	}

	/* TODO : cleanup your module not specific minor device */
	BE_IPC_Free();

	unregister_chrdev_region(dev, BE_MAX_DEVICE );

	OS_Free( g_be_device );

	BE_PRINT("BE device cleanup\n");
}

#ifndef KDRV_GLOBAL_LINK
#if defined(CONFIG_LG_BUILTIN_KDRIVER) && defined(CONFIG_LGSNAP)
user_initcall_grp("kdrv",BE_Init);
#else
module_init(BE_Init);
#endif
module_exit(BE_Cleanup);

MODULE_AUTHOR("LGE");
MODULE_DESCRIPTION("be driver");
MODULE_LICENSE("GPL");
#endif

/** @} */


