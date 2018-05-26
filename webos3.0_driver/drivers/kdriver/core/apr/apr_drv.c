
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
 *  drv file for APR device
 *
 *  author		justine.jeong
 *  version		1.0
 *  date		2012.07.06
 *  note		Additional information.
 *
 *  @addtogroup APR
 *	@{
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/
//#define _USE_PROC_

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
#include <linux/kthread.h>
#include <asm/uaccess.h>
#ifdef	KDRV_CONFIG_PM	// added by SC Jung for quick booting
#include <linux/platform_device.h>
#endif

#ifdef _USE_PROC_
#include "proc_util.h"
#endif
#include "os_util.h"
#include "base_device.h"
#include "debug_util.h"

#include "apr_kapi.h"
#include "apr_cfg.h"
#include "apr_def.h"
#include "apr_drv.h"
#include "apr_core.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/
/**
 *  main control block for APR device.
 *  each minor device has unique control block
 *
 */
typedef struct
{
	int				dev_open_count;		///< check if device is opened or not
	dev_t			devno;				///< device number
	struct cdev		cdev;				///< char device structure
}
APR_DEVICE_T;

typedef struct
{
	bool			is_suspended;
}
APR_DRVDATA_T;

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/
int				g_apr_major = APR_MAJOR;
int				g_apr_minor = APR_MINOR;
int				g_apr_debug_fd;
int				g_apr_device_opened = 0;

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/
static int  APR_Open      (struct inode *, struct file *);
static int  APR_Close     (struct inode *, struct file *);
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36))
static int  APR_Ioctl     (struct inode *, struct file *, UINT32, ULONG );
#else
static long	APR_Ioctl     (struct file *, unsigned int, unsigned long );
#endif
static int  APR_Thread    (void *data);
static int  APR_InitThread(void);

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
//static OS_SEM_T	apr_dev_mtx;

/**
 * main control block for gfx device
 */
static APR_DEVICE_T*		g_apr_device;

/**
 * file I/O description for gfx device
 */
static struct file_operations g_apr_fops =
{
	.open		=	APR_Open,
	.release	=	APR_Close,
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36))
	.ioctl		=	APR_Ioctl,
#else
	.unlocked_ioctl =	APR_Ioctl,
#endif
	.read		=	NULL,
	.write		=	NULL,
};

static struct task_struct *stAPR_Thread;
#ifdef _USE_PROC_
static struct proc_dir_entry *proc_root_entry;
#endif
/*========================================================================================
	Implementation Group
========================================================================================*/
#ifdef	KDRV_CONFIG_PM
/**
 * suspending module/
 *
 * @param	struct platform_device *pdev pm_message_t state
 * @return	int 0 : OK , -1 : NOT OK
 *
 */
static int APR_suspend(struct platform_device *pdev, pm_message_t state)
{
#ifdef KDRV_CONFIG_PM_APR
    APR_DRVDATA_T   *drv_data;

    printk("APR - suspend BEGIN\n");
    drv_data = platform_get_drvdata(pdev);

    drv_data->is_suspended = 1;

    printk("APR - suspend OK\n");
#endif
    return 0;
}

/**
 * resuming module.
 *
 * @param   struct platform_device *
 * @return  int 0 : OK , -1 : NOT OK
 *
 */
static int APR_resume(struct platform_device *pdev)
{
#ifdef KDRV_CONFIG_PM_APR
    APR_DRVDATA_T *drv_data;

    printk("APR - resume BEGIN\n");
    drv_data = platform_get_drvdata(pdev);

    if(drv_data->is_suspended == 0)
    {
        printk("APR - resume FAIL\n");
        return -1;
    }

    if(g_apr_device_opened)
    {
        if(APR_Core_Resume() != RET_OK)
        {
            printk("APR - resume FAIL\n");
            return -1;
        }
    }

    drv_data->is_suspended = 0;
    printk("APR - resume OK\n");
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
int APR_probe(struct platform_device *pdev)
{
	APR_DRVDATA_T *drv_data;

	drv_data = (APR_DRVDATA_T *)kmalloc(sizeof(APR_DRVDATA_T) , GFP_KERNEL);

	APR_PRINT("[%s] done probe\n", APR_MODULE);

	drv_data->is_suspended = 0;

	platform_set_drvdata(pdev, drv_data);

	return 0;
}

/**
 * module remove function. this function will be called in rmmod APR module
 *
 * @param	struct platform_device
 * @return	int 0 : OK , -1 : NOT OK
 *
 */
static int APR_remove(struct platform_device *pdev)
{
	APR_DRVDATA_T *drv_data;

	drv_data = platform_get_drvdata(pdev);
	kfree(drv_data);

	APR_PRINT("[%s] removed\n", APR_MODULE);

	return 0;
}

/**
 * module release function. this function will be called in rmmod APR module
 *
 * @param	struct device *dev
 * @return	int 0 : OK , -1 : NOT OK
 *
 */
static void APR_release(struct device *dev)
{
	APR_PRINT("[%s] device released\n", APR_MODULE);
}

/*
 * module platform driver structure
 */
static struct platform_driver apr_driver =
{
	.probe		= APR_probe,
	.suspend	= APR_suspend,
	.remove		= APR_remove,
	.resume		= APR_resume,
	.driver		=
	{
		.name	= APR_MODULE,
	},
};

static struct platform_device apr_device =
{
	.name		= APR_MODULE,
	.id			= 0,
	.dev		=
	{
		.release = APR_release,
	},
};
#endif

#ifdef _USE_PROC_
static OS_PROC_DESC_TABLE_T	_g_apr_device_proc_table[] =
{
	{ "logmask",		0, OS_PROC_FLAG_READ | OS_PROC_FLAG_WRITE },
	{ NULL, 		10		, 0 }
};

static int _APR_PROC_Reader(UINT32 procId, char* buffer )
{
	int ret=0;

	return ret;
}
static int _APR_PROC_Writer( UINT32 procId, char* command )
{
	switch( procId )
	{
		case 0:

			APR_PRINT("proc apr set \n");

			break;
	}

	return 0;
}

static void APR_proc_init(void)
{
	proc_root_entry = proc_mkdir ("apr", NULL);

	OS_PROC_CreateEntryEx( APR_MODULE, _g_apr_device_proc_table,
			_APR_PROC_Reader, _APR_PROC_Writer);

}

static void APR_proc_cleanup (void)
{
	OS_PROC_RemoveEntry( APR_MODULE );
	remove_proc_entry ("apr", NULL);
}
#endif

/**
 * open handler for APR device
 *
 * @param	*inode [in] inode
 * @param	*filp [in] file
 * @return	OK if success, ERROR otherwisw,
 * @see
 * @author
 */
static int APR_Open(struct inode *inode, struct file *filp)
{
	int				major, minor;
	struct cdev*	cdev;
	APR_DEVICE_T*	my_dev;
#ifdef _USE_PROC_
	struct proc_dir_entry *proc_entry;
#endif
	APR_PRINT("Attempting to open APR device...\n");

	cdev	= inode->i_cdev;
	my_dev	= container_of (cdev, APR_DEVICE_T, cdev);

	my_dev->dev_open_count++;
	filp->private_data = my_dev;

	major = imajor(inode);
	minor = iminor(inode);

#ifdef _USE_PROC_
	/* initialize proc directory */
	proc_entry = proc_mkdir ("apr", proc_root_entry);
	proc_create_data ("apr", 0440, proc_entry, NULL, NULL);
#endif

	g_apr_device_opened = TRUE;

	APR_PRINT("device opened (%d:%d)\n", major, minor);

	return 0;
}

/**
 * APR close
 *
 * @param	*inode [in] inode
 * @param	*file [in] file
 * @return	OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int APR_Close(struct inode *inode, struct file *file)
{
	int				major, minor;
	APR_DEVICE_T*	my_dev;
	struct cdev*	cdev;

	cdev	= inode->i_cdev;
	my_dev	= container_of( cdev, APR_DEVICE_T, cdev);

	if( my_dev->dev_open_count > 0 )
	{
		--my_dev->dev_open_count;
	}

	major = imajor(inode);
	minor = iminor(inode);

#ifdef _USE_PROC_
	APR_proc_cleanup();
#endif
	APR_PRINT("device closed (%d:%d)\n", major, minor);
	return 0;
}

/**
 * ioctl handler for APR device.
 *
 * @param   void
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 * @note: if you have some critial data, you should protect them using semaphore or spin lock.
 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)
static int
APR_Ioctl ( struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg )
#else
static long
APR_Ioctl ( struct file *filp, unsigned int cmd, unsigned long arg )
#endif
{
    int err = 0, ret = 0;

    APR_DEVICE_T*	my_dev;
    struct cdev*		cdev;

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)
#else
		struct inode *inode = filp->f_path.dentry->d_inode;
#endif

	/*
	 * get current APR device object
	 */
    cdev	= inode->i_cdev;
    my_dev	= container_of ( cdev, APR_DEVICE_T, cdev);

    /*
     * check if IOCTL command is valid or not.
     * - if magic value doesn't match, return error (-ENOTTY)
     * - if command is out of range, return error (-ENOTTY)
     *
     * note) -ENOTTY means "Inappropriate ioctl for device.
     */
    if (_IOC_TYPE(cmd) != APR_IOC_MAGIC)
    {
    	APR_WARN("invalid magic. magic=0x%02X\n", _IOC_TYPE(cmd) );
    	return -ENOTTY;
    }
    if (_IOC_NR(cmd) > APR_IOC_MAXNR)
    {
    	APR_WARN("out of ioctl command. cmd_idx=%d\n", _IOC_NR(cmd) );
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
    	APR_WARN("memory access error. cmd_idx=%d, rw=%c%c, memptr=%p\n",
    									_IOC_NR(cmd),
    									(_IOC_DIR(cmd) & _IOC_READ)? 'r':'-',
    									(_IOC_DIR(cmd) & _IOC_WRITE)? 'w':'-',
    									(void*)arg );
        return -EFAULT;
	}

	switch(cmd)
	{
		case APR_IOW_INITIALIZE :
			ret = APR_Initialize(arg);
			break;
		case APR_IOW_SETCONTROL :
			ret = APR_SetControl(arg);
			break;
		case APR_IOR_GETINFO :
			ret = APR_GetInfo(arg);
			break;
		case APR_IOW_SETCOMMAND :
			ret = APR_SetCommand(arg);
			break;
		default :
			ret = -ENOTTY;
			break;
	}

    return ret;
}

/** Initialize the device environment before the real H/W initialization
 *
 *  @note main usage of this function is to initialize the HAL layer and memory size adjustment
 *  @note it's natural to keep this function blank :)
 */
void APR_PreInit(void)
{
    /* TODO: do something */
}

/**
 * init APR drv
 *
 * @param   void
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int APR_Init(void)
{
	int			i;
	int			err;
	dev_t		dev;

	/* Get the handle of debug output for APR device.
	 *
	 * Most module should open debug handle before the real initialization of module.
	 * As you know, debug_util offers 4 independent debug outputs for your device driver.
	 * So if you want to use all the debug outputs, you should initialize each debug output
	 * using OS_DEBUG_EnableModuleByIndex() function.
	 */
	if (lx_chip_rev() < LX_CHIP_REV(H13, A0))
		return 0;

	g_apr_debug_fd = DBG_OPEN( APR_MODULE );
	if (g_apr_debug_fd < 0) {
		DBG_PRINT_ERROR("can't open APR module\n" );
		return -EIO;
	}
	OS_DEBUG_EnableModule ( g_apr_debug_fd );

	OS_DEBUG_EnableModuleByIndex ( g_apr_debug_fd, LX_LOGM_LEVEL_ERROR, DBG_COLOR_YELLOW );
	OS_DEBUG_EnableModuleByIndex ( g_apr_debug_fd, LX_LOGM_LEVEL_WARNING, DBG_COLOR_GRAY );
	OS_DEBUG_EnableModuleByIndex ( g_apr_debug_fd, LX_LOGM_LEVEL_INFO, DBG_COLOR_GRAY );
	OS_DEBUG_EnableModuleByIndex ( g_apr_debug_fd, LX_LOGM_LEVEL_NOTI, DBG_COLOR_GRAY );
//	OS_DEBUG_EnableModuleByIndex ( g_apr_debug_fd, LX_LOGM_LEVEL_TRACE, DBG_COLOR_GRAY );

	/* allocate main device handler, register current device.
	 *
	 * If devie major is predefined then register device using that number.
	 * otherwise, major number of device is automatically assigned by Linux kernel.
	 *
	 */
#ifdef	KDRV_CONFIG_PM
	// added by SC Jung for quick booting
	if(platform_driver_register(&apr_driver) < 0)
	{
		APR_ERROR("[%s] platform driver register failed\n",APR_MODULE);

	}
	else
	{
		if(platform_device_register(&apr_device))
		{
			platform_driver_unregister(&apr_driver);
			APR_ERROR("[%s] platform device register failed\n",APR_MODULE);
		}
		else
		{
			APR_NOTI("[%s] platform register done\n", APR_MODULE);
		}


	}
#endif

	g_apr_device = (APR_DEVICE_T*)OS_KMalloc( sizeof(APR_DEVICE_T)*APR_MAX_DEVICE );

	if ( NULL == g_apr_device )
	{
		APR_ERROR("out of memory. can't allocate %d bytes\n", sizeof(APR_DEVICE_T)* APR_MAX_DEVICE );
		return -ENOMEM;
	}

	memset( g_apr_device, 0x0, sizeof(APR_DEVICE_T)* APR_MAX_DEVICE );

	if (g_apr_major)
	{
		dev = MKDEV( g_apr_major, g_apr_minor );
		err = register_chrdev_region(dev, APR_MAX_DEVICE, APR_MODULE );
	}
	else
	{
		err = alloc_chrdev_region(&dev, g_apr_minor, APR_MAX_DEVICE, APR_MODULE );
		g_apr_major = MAJOR(dev);
	}

	if ( err < 0 )
	{
		APR_ERROR("can't register APR device\n" );
		return -EIO;
	}



	/* END */

	for ( i=0; i<APR_MAX_DEVICE; i++ )
	{
		/* initialize cdev structure with predefined variable */
		dev = MKDEV( g_apr_major, g_apr_minor+i );
		cdev_init( &(g_apr_device[i].cdev), &g_apr_fops );
		g_apr_device[i].devno		= dev;
		g_apr_device[i].cdev.owner = THIS_MODULE;
		g_apr_device[i].cdev.ops   = &g_apr_fops;

		/* TODO: initialize minor device */


		/* END */

		err = cdev_add (&(g_apr_device[i].cdev), dev, 1 );

		if (err)
		{
			APR_ERROR("error (%d) while adding APR device (%d.%d)\n", err, MAJOR(dev), MINOR(dev) );
			return -EIO;
		}
        OS_CreateDeviceClass ( g_apr_device[i].devno, "%s%d", APR_MODULE, i );
	}

#ifdef _USE_PROC_
	APR_proc_init();
#endif
	/* TODO : initialize your module not specific minor device */
	//APR_InitCfg();
	APR_InitThread();
	//APR_REG_Init();
	//APR_HAL_Init();
#ifndef	KDRV_CONFIG_PM
	//APR_SPD_probe();
#endif

	APR_NOTI("APR device initialized\n");

	return 0;
}


/**
 * APR cleanup
 *
 * @param   void
 * @return  void
 * @see
 * @author
 */
void APR_Cleanup(void)
{
	if(stAPR_Thread != NULL)
		kthread_stop(stAPR_Thread);

	APR_Core_Cleanup();

	APR_PRINT("APR device cleanup\n");
}

/**
 * APR Thread
 *
 * @param
 * @return
 * @see
 * @author
 */
static int APR_Thread(void *data)
{
	static int ret = RET_OK;

	while(1)
	{
		OS_MsecSleep(120);
		APR_Core_ThreadHandle();
	}

	return ret;
}

/**
 * APR Thread Init
 *
 * @param
 * @return
 * @see
 * @author
 */
static int APR_InitThread(void)
{
	stAPR_Thread = kthread_create(APR_Thread, (void*)NULL, "apr_thread");
	if(stAPR_Thread)
	{
		wake_up_process(stAPR_Thread);
	}
	else
	{
		APR_PRINT("APR Thread alreadu created\n");
	}

	return RET_OK;
}

/**
 * APR Initialize
 *
 * @param
 * @return
 * @see
 * @author
 */
int APR_Initialize(unsigned long arg)
{
	int ret = RET_OK;
	LX_APR_INIT_T stParams;

	do {
		ret = copy_from_user(&stParams, (void __user *)arg, sizeof(LX_APR_INIT_T));
		if(ret) BREAK_WRONG(ret);
		ret = APR_Core_Init(&stParams);
	} while(0);

	return ret;
}

/**
 * APR set control
 *
 * @param
 * @return
 * @see
 * @author
 */
int APR_SetControl(unsigned long arg)
{
	int ret = RET_OK;
	LX_APR_CONTROL_T stParams;

	do {
		ret = copy_from_user(&stParams, (void __user *)arg, sizeof(LX_APR_CONTROL_T));
		if(ret) BREAK_WRONG(ret);
		ret = APR_Core_SetControl(&stParams);
	} while(0);

	return ret;
}

/**
 * APR get information (3d pattern, dual screen)
 *
 * @param
 * @return
 * @see
 * @author
 */
int APR_GetInfo(unsigned long arg)
{
	int ret = RET_OK;
	LX_APR_INFO_T stParams;

	do {
		ret = APR_Core_GetInfo(&stParams);
		if(ret) BREAK_WRONG(ret);
		ret = copy_to_user((void __user *)arg, &stParams, sizeof(LX_APR_INFO_T));
	} while(0);

	return ret;
}

/**
 * APR set command for debug
 *
 * @param
 * @return
 * @see
 * @author
 */
int APR_SetCommand(unsigned long arg)
{
	int ret = RET_OK;
	LX_APR_COMMAND_T stParams;

	do {
		ret = copy_from_user(&stParams, (void __user *)arg, sizeof(LX_APR_COMMAND_T));
		if(ret) BREAK_WRONG(ret);
		ret = APR_Core_SetCommand(&stParams);
	} while(0);

	return ret;
}

#ifndef KDRV_GLOBAL_LINK
#if defined(CONFIG_LG_BUILTIN_KDRIVER) && defined(CONFIG_LGSNAP)
user_initcall_grp("kdrv",APR_Init);
#else
module_init(APR_Init);
#endif
module_exit(APR_Cleanup);

MODULE_AUTHOR("LGE");
MODULE_DESCRIPTION("apr driver");
MODULE_LICENSE("GPL");
#endif

/** @} */


