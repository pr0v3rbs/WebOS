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
 *  main driver implementation for denc device.
 *	denc device will teach you how to make device driver with new platform.
 *
 *  author		wonsik.do (wonsik.do@lge.com)
 *  version		1.0
 *  date		2010.01.06
 *  note		Additional information.
 *
 *  @addtogroup lg1150_denc
 *	@{
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/
#undef	SUPPORT_DENC_DEVICE_READ_WRITE_FOPS

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <linux/version.h>
#ifdef	KDRV_CONFIG_PM	// added by SC Jung for quick booting
#include <linux/platform_device.h>
#endif
#include "os_util.h"
#include "base_device.h"
#include "denc_drv.h"
#include "denc_io.h"
#include "denc_hal.h"

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
 *	main control block for denc device.
 *	each minor device has unique control block
 *
 */
typedef struct
{
// BEGIN of common device
	int						dev_open_count;		///< check if device is opened or not
	dev_t					devno;			///< device number
	struct cdev				cdev;			///< char device structure
	int						is_initialized; ///< check if device is initialized or not
// END of command device

// BEGIN of device specific data


// END of device specific data
}
DENC_DEVICE_T;

#ifdef	KDRV_CONFIG_PM
typedef struct
{
	// add here extra parameter
	bool	is_suspended;
}DENC_DRVDATA_T;

#endif
/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/
extern	void	DENC_PROC_Init(void);
extern	void	DENC_PROC_Cleanup(void);

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Functions
----------------------------------------------------------------------------------------*/
int		DENC_Init(void);
void	DENC_Cleanup(void);

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/
int		g_denc_debug_fd;
int 	g_denc_major = DENC_MAJOR;
int 	g_denc_minor = DENC_MINOR;
int		g_denc_device_opened = 0;

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/
static int      DENC_Open(struct inode *, struct file *);
static int      DENC_Close(struct inode *, struct file *);
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)
static int 		DENC_Ioctl (struct inode *, struct file *, unsigned int, unsigned long );
#else
static long     DENC_Ioctl (struct file * filp, unsigned int cmd, unsigned long arg);
#endif
#ifdef SUPPORT_DENC_DEVICE_READ_WRITE_FOPS
static ssize_t  DENC_Read(struct file *, char *, size_t, loff_t *);
static ssize_t  DENC_Write(struct file *, const char *, size_t, loff_t *);
#endif

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/

/**
 * main control block for denc device
*/
static DENC_DEVICE_T*		g_denc_device;

/**
 * file I/O description for denc device
 *
*/
static struct file_operations g_denc_fops =
{
	.open 	= DENC_Open,
	.release= DENC_Close,
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)
	.ioctl	= DENC_Ioctl,
#else
	.unlocked_ioctl = DENC_Ioctl,
#endif
#ifdef SUPPORT_DENC_DEVICE_READ_WRITE_FOPS
	.read 	= DENC_Read,
	.write 	= DENC_Write,
#else
	.read	= NULL,
	.write	= NULL,
#endif
};

/*========================================================================================
	Implementation Group
========================================================================================*/
#ifdef	KDRV_CONFIG_PM	// added by SC Jung for quick booting
/**
 *
 * suspending module.
 *
 * @param	struct platform_device *pdev pm_message_t state
 * @return	int 0 : OK , -1 : NOT OK
 *
 */
static int DENC_suspend(struct platform_device *pdev, pm_message_t state)
{
#ifdef KDRV_CONFIG_PM_DENC
	DENC_DRVDATA_T *drv_data;

	printk("DENC - suspend BEGIN\n");

	drv_data = platform_get_drvdata(pdev);

	// add here the suspend code

	drv_data->is_suspended = 1;
	printk("DENC - suspend OK\n");
#endif
	return 0;
}


/**
 *
 * resuming module.
 *
 * @param	struct platform_device *
 * @return	int 0 : OK , -1 : NOT OK
 *
 */
static int DENC_resume(struct platform_device *pdev)
{
#ifdef KDRV_CONFIG_PM_DENC
	DENC_DRVDATA_T *drv_data;

	printk("DENC - resume BEGIN\n");

	drv_data = platform_get_drvdata(pdev);

	if(drv_data->is_suspended == 0)
	{
		printk("DENC - resume FAIL\n");
		return -1;
	}

	// add here the resume code
	if(g_denc_device_opened)
	{
		if(DENC_IO_Resume() != RET_OK)
		{
			printk("DENC - resume FAIL\n");
			return -1;
		}
	}

	drv_data->is_suspended = 0;
	printk("DENC - resume OK\n");
#endif
	return 0;
}
/**
 *
 * probing module.
 *
 * @param	struct platform_device *pdev
 * @return	int 0 : OK , -1 : NOT OK
 *
 */
int DENC_probe(struct platform_device *pdev)
{

	DENC_DRVDATA_T *drv_data;

	drv_data = (DENC_DRVDATA_T *)kmalloc(sizeof(DENC_DRVDATA_T) , GFP_KERNEL);


	// add here driver registering code & allocating resource code



	DENC_PRINT("[%s] done probe\n", DENC_MODULE);
	drv_data->is_suspended = 0;
	platform_set_drvdata(pdev, drv_data);

	return 0;
}


/**
 *
 * module remove function. this function will be called in rmmod denc module.
 *
 * @param	struct platform_device
 * @return	int 0 : OK , -1 : NOT OK
 *
 */
static int  DENC_remove(struct platform_device *pdev)
{
	DENC_DRVDATA_T *drv_data;

	// add here driver unregistering code & deallocating resource code



	drv_data = platform_get_drvdata(pdev);
	kfree(drv_data);

	DENC_PRINT("removed\n");

	return 0;
}

/**
 *
 * module release function. this function will be called in rmmod denc module.
 *
 * @param	struct device *dev
 * @return	int 0 : OK , -1 : NOT OK
 *
 */
static void DENC_release(struct device *dev)
{
	DENC_PRINT("device released\n");
}

/*
 *	module platform driver structure
 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,39)
static struct platform_driver denc_driver =
#else
static struct platform_driver denc_driver __refdata =
#endif
{
	.probe          = DENC_probe,
	.suspend        = DENC_suspend,
	.remove         = DENC_remove,
	.resume         = DENC_resume,
	.driver         =
	{
		.name   = DENC_MODULE,
	},
};

static struct platform_device denc_device = {
	.name = DENC_MODULE,
	.id = 0,
	.dev =
	{
		.release = DENC_release,
	},
};
#endif

/** Initialize the device environment before the real H/W initialization
 *
 *  @note main usage of this function is to initialize the HAL layer and memory size adjustment
 *  @note it's natural to keep this function blank :)
 */
void DENC_PreInit(void)
{
    /* TODO: do something */
}

int DENC_Init(void)
{
	int			i;
	int			err;
	dev_t		dev;

	/* Get the handle of debug output for denc device.
	 *
	 * Most module should open debug handle before the real initialization of module.
	 * As you know, debug_util offers 4 independent debug outputs for your device driver.
	 * So if you want to use all the debug outputs, you should initialize each debug output
	 * using OS_DEBUG_EnableModuleByIndex() function.
	 */
	g_denc_debug_fd = DBG_OPEN( DENC_MODULE );
	if (g_denc_debug_fd < 0) {
		DENC_ERROR("can't open denc module\n" );
		return -EIO;
	}
	OS_DEBUG_EnableModule ( g_denc_debug_fd );

	OS_DEBUG_EnableModuleByIndex ( g_denc_debug_fd, LX_LOGM_LEVEL_ERROR, DBG_COLOR_NONE );
	OS_DEBUG_EnableModuleByIndex ( g_denc_debug_fd, LX_LOGM_LEVEL_WARNING, DBG_COLOR_NONE );
	OS_DEBUG_EnableModuleByIndex ( g_denc_debug_fd, LX_LOGM_LEVEL_INFO, DBG_COLOR_NONE );
	OS_DEBUG_EnableModuleByIndex ( g_denc_debug_fd, LX_LOGM_LEVEL_NOTI, DBG_COLOR_NONE );
	OS_DEBUG_EnableModuleByIndex ( g_denc_debug_fd, LX_LOGM_LEVEL_TRACE, DBG_COLOR_NONE );
	OS_DEBUG_DisableModuleByIndex( g_denc_debug_fd, LX_LOGM_LEVEL_TRACE);

	/* allocate main device handler, register current device.
	 *
	 * If devie major is predefined then register device using that number.
	 * otherwise, major number of device is automatically assigned by Linux kernel.
	 *
	 */
#ifdef	KDRV_CONFIG_PM
	// added by SC Jung for quick booting
	if(platform_driver_register(&denc_driver) < 0)
	{
		DENC_ERROR("[%s] platform driver register failed\n",DENC_MODULE);

	}
	else
	{
		if(platform_device_register(&denc_device))
		{
			platform_driver_unregister(&denc_driver);
			DENC_ERROR("[%s] platform device register failed\n",DENC_MODULE);
		}
		else
		{
			DENC_NOTI("[%s] platform register done\n", DENC_MODULE);
		}


	}
#endif
	g_denc_device = (DENC_DEVICE_T*)OS_KMalloc( sizeof(DENC_DEVICE_T)*DENC_MAX_DEVICE );

	if ( NULL == g_denc_device )
	{
		DENC_ERROR("out of memory. can't allocate %d bytes\n", sizeof(DENC_DEVICE_T)* DENC_MAX_DEVICE );
		return -ENOMEM;
	}

	memset( g_denc_device, 0x0, sizeof(DENC_DEVICE_T)* DENC_MAX_DEVICE );

	if (g_denc_major)
	{
		dev = MKDEV( g_denc_major, g_denc_minor );
		err = register_chrdev_region(dev, DENC_MAX_DEVICE, DENC_MODULE );
	}
	else
	{
		err = alloc_chrdev_region(&dev, g_denc_minor, DENC_MAX_DEVICE, DENC_MODULE );
		g_denc_major = MAJOR(dev);
	}

	if ( err < 0 )
	{
		DENC_ERROR("can't register denc device\n" );
		return -EIO;
	}

	/* TODO : initialize your module not specific minor device */
	DENC_HAL_Create();
	DENC_IO_IPC_Init(0);

	/* END */

	for ( i=0; i<DENC_MAX_DEVICE; i++ )
	{
		/* initialize cdev structure with predefined variable */
		dev = MKDEV( g_denc_major, g_denc_minor+i );
		cdev_init( &(g_denc_device[i].cdev), &g_denc_fops );
		g_denc_device[i].devno		= dev;
		g_denc_device[i].cdev.owner = THIS_MODULE;
		g_denc_device[i].cdev.ops   = &g_denc_fops;

		/* TODO: initialize minor device */


		/* END */

		err = cdev_add (&(g_denc_device[i].cdev), dev, 1 );

		if (err)
		{
			DENC_ERROR("error (%d) while adding denc device (%d.%d)\n", err, MAJOR(dev), MINOR(dev) );
			return -EIO;
		}
		OS_CreateDeviceClass ( g_denc_device[i].devno, "%s%d", DENC_MODULE, i );
	}

	/* initialize proc system */
	DENC_PROC_Init ( );

	DENC_NOTI("denc device initialized\n");

	return 0;
}

void DENC_Cleanup(void)
{
	int i;
	dev_t dev = MKDEV( g_denc_major, g_denc_minor );

#ifdef	KDRV_CONFIG_PM
	// added by SC Jung for quick booting
	platform_driver_unregister(&denc_driver);
	platform_device_unregister(&denc_device);
#endif

	/* cleanup proc system */
	DENC_PROC_Cleanup( );

	/* remove all minor devicies and unregister current device */
	for ( i=0; i<DENC_MAX_DEVICE;i++)
	{
		/* TODO: cleanup each minor device */


		/* END */
		cdev_del( &(g_denc_device[i].cdev) );
	}

	/* TODO : cleanup your module not specific minor device */
	DENC_HAL_Destory();
	DENC_IO_IPC_Cleanup(0);

	unregister_chrdev_region(dev, DENC_MAX_DEVICE );

	OS_Free( g_denc_device );

	DENC_PRINT("denc device cleanup\n");
}


///////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * open handler for denc device
 *
 */
static int
DENC_Open(struct inode *inode, struct file *filp)
{
    int					major,minor;
    struct cdev*    	cdev;
    DENC_DEVICE_T*	my_dev;

    cdev	= inode->i_cdev;
    my_dev	= container_of ( cdev, DENC_DEVICE_T, cdev);

    /* TODO : add your device specific code */

	/* END */

    my_dev->dev_open_count++;
    filp->private_data = my_dev;

	/* some debug */
    major = imajor(inode);
    minor = iminor(inode);

	g_denc_device_opened++;

    DENC_PRINT("device opened (%d:%d)\n", major, minor );

    return 0;
}

/**
 * release handler for denc device
 *
 */
static int
DENC_Close(struct inode *inode, struct file *file)
{
    int					major,minor;
    DENC_DEVICE_T*	my_dev;
    struct cdev*		cdev;

    cdev	= inode->i_cdev;
    my_dev	= container_of ( cdev, DENC_DEVICE_T, cdev);

    if ( my_dev->dev_open_count > 0 )
    {
		--my_dev->dev_open_count;
    }

    /* TODO : add your device specific code */

	/* END */

	/* some debug */
    major = imajor(inode);
    minor = iminor(inode);
    DENC_PRINT("device closed (%d:%d)\n", major, minor );
    return 0;
}

/**
 * ioctl handler for denc device.
 *
 *
 * note: if you have some critial data, you should protect them using semaphore or spin lock.
 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)
static int
DENC_Ioctl ( struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg )
#else
static long
DENC_Ioctl (struct file * filp, unsigned int cmd, unsigned long arg)
#endif
{
    int err = 0, ret = 0;

    DENC_DEVICE_T*	my_dev;
    struct cdev*		cdev;
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)
#else
	struct inode *inode = filp->f_path.dentry->d_inode;
#endif

	/*
	 * get current denc device object
	 */
    cdev	= inode->i_cdev;
    my_dev	= container_of ( cdev, DENC_DEVICE_T, cdev);

    /*
     * check if IOCTL command is valid or not.
     * - if magic value doesn't match, return error (-ENOTTY)
     * - if command is out of range, return error (-ENOTTY)
     *
     * note) -ENOTTY means "Inappropriate ioctl for device.
     */
    if (_IOC_TYPE(cmd) != DENC_IOC_MAGIC)
    {
    	DENC_WARN("invalid magic. magic=0x%02X\n", _IOC_TYPE(cmd) );
    	return -ENOTTY;
    }
    if (_IOC_NR(cmd) > DENC_IOC_MAXNR)
    {
    	DENC_WARN("out of ioctl command. cmd_idx=%d\n", _IOC_NR(cmd) );
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
    	DENC_WARN("memory access error. cmd_idx=%d, rw=%c%c, memptr=%p\n",
    									_IOC_NR(cmd),
    									(_IOC_DIR(cmd) & _IOC_READ)? 'r':'-',
    									(_IOC_DIR(cmd) & _IOC_WRITE)? 'w':'-',
    									(void*)arg );
        return -EFAULT;
	}

//	DENC_TRACE("cmd = %08X (cmd_idx=%d)\n", cmd, _IOC_NR(cmd) );

	switch(cmd)
	{
#if 0
		case DENC_IO_RESET:
		{
			DENC_PRINT("reset ok\n");
			ret = 0;
		}
		break;

		case DENC_IOR_CHIP_REV_INFO:
		{
			CHIP_REV_INFO_T	rev_info;

			rev_info.version = 0x100;
			rev_info.date[0] =  9;	/* 2009/11/24 */
			rev_info.date[0] = 11;
			rev_info.date[0] = 24;

			DENC_PRINT("rev_info (%0X, %d:%d:%d)\n", rev_info.version,
														rev_info.date[0], rev_info.date[1], rev_info.date[2] );

	        if ( copy_to_user((void __user *)arg, &rev_info, sizeof(CHIP_REV_INFO_T)) )
    	        return -EFAULT;

			ret = 0;
		}
		break;

		case DENC_IOW_WRITE_UINT32:
		{
			UINT32	data;

			ret = __get_user( data, (UINT32 __user *)arg );

			DENC_PRINT("data = 0x%X\n", data );

			ret = 0;
		}
		break;
#endif

		case DENC_IO_INIT:
			DENC_PRINT("DENC init\n");
#ifndef USE_VIDEO_IOCTL_CALLING
			ret = 0;
#else
			if (my_dev->is_initialized == 0)
			{
				ret = DENC_IO_Init(arg);
				my_dev->is_initialized = 1;
			}
#endif
			break;

		case DENC_IOW_ONOFF:
#ifndef USE_VIDEO_IOCTL_CALLING
			ret = 0;
#else
			ret = DENC_IO_OnOff(arg);
#endif
			break;

		case DENC_IOW_SET_OUTPUT_VIDEO_STD:
	//		DENC_PRINT("Denc set output video standard\n");
#ifndef USE_VIDEO_IOCTL_CALLING
			ret = 0;
#else
			ret = DENC_IO_SetOutPutVideoStd(arg);
#endif
			break;

		case DENC_IOW_SET_SOURCE:
	//		DENC_PRINT("Denc Set source\n");
#ifndef USE_VIDEO_IOCTL_CALLING
			ret = 0;
#else
			ret = DENC_IO_SetSource(arg);
#endif
			break;

		case DENC_IOW_TTX_ENABLE:
#ifndef USE_VIDEO_IOCTL_CALLING
			ret = 0;
#else
			ret = DENC_IO_TtxEnable(arg);
#endif
			break;

		case DENC_IOW_WSS_ENABLE:
#ifndef USE_VIDEO_IOCTL_CALLING
			ret = 0;
#else
			ret = DENC_IO_WssEnable(arg);
#endif
			break;

		case DENC_IOW_VPS_ENABLE:
#ifndef USE_VIDEO_IOCTL_CALLING
			ret = 0;
#else
			ret = DENC_IO_VpsEnable(arg);
#endif
			break;

		case DENC_IOW_SET_TTX_DATA:
			//DENC_PRINT("DENC set TTX data\n");
#ifndef USE_VIDEO_IOCTL_CALLING
			ret = 0;
#else
			ret = DENC_IO_SetTtxData(arg);
#endif
			break;

		case DENC_IOW_SET_WSS_DATA:
			//DENC_PRINT("DENC set WSS data\n");
#ifndef USE_VIDEO_IOCTL_CALLING
			ret = 0;
#else
			ret = DENC_IO_SetWssData(arg);
#endif
			break;

		case DENC_IOW_SET_VPS_DATA:
			//DENC_PRINT("DENC set VPS data\n");
#ifndef USE_VIDEO_IOCTL_CALLING
			ret = 0;
#else
			ret = DENC_IO_SetVpsData(arg);
#endif
			break;

		case DENC_IOW_COLORBAR_ENABLE:
#ifndef USE_VIDEO_IOCTL_CALLING
			ret = 0;
#else
			ret = DENC_IO_ColorBarEnable(arg);
#endif
			break;

		case DENC_IOW_VDAC_POWER_CONTROL:
#ifndef USE_VIDEO_IOCTL_CALLING
			ret = 0;
#else
			ret = DENC_IO_VdacPowerControl(arg);
#endif
			break;

		case DENC_IOW_MUTE_CONTROL:
#ifndef USE_VIDEO_IOCTL_CALLING
			ret = 0;
#else
			ret = DENC_IO_MuteControl(arg);
#endif
			break;

		case DENC_IOW_STRIP_CAPTURE_CONTROL:
#ifndef USE_VIDEO_IOCTL_CALLING
			ret = 0;
#else
			ret = DENC_IO_StripCaptureControl(arg);
#endif
			break;

	    default:
	    {
			/* redundant check but it seems more readable */
    	    ret = -ENOTTY;
		}
    }

    return ret;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef KDRV_GLOBAL_LINK
#if defined(CONFIG_LG_BUILTIN_KDRIVER) && defined(CONFIG_LGSNAP)
user_initcall_grp("kdrv", DENC_Init);
#else
module_init(DENC_Init);
#endif
module_exit(DENC_Cleanup);

MODULE_AUTHOR("LGE");
MODULE_DESCRIPTION("DENC driver");
MODULE_LICENSE("GPL");
#endif

/** @} */

