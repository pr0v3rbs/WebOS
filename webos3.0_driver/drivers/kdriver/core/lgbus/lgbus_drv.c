/*
 * SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 * Copyright(c) 2013 by LG Electronics Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */


/** @file
 *
 *  main driver implementation for lgbus device.
 *
 *  author		hwanwook.lee (hwanwook.lee@lge.com)
 *              bongrae.cho (bongrae.cho@lge.com)
 *  version		1.0
 *  date		2012.11.10
 *  note		Additional information.
 *
 *  @addtogroup lg115x_lgbus
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
#include <asm/uaccess.h>
#include <linux/version.h>
#include <linux/platform_device.h>


#include "os_util.h"
#include "base_device.h"
#include "sys_regs.h"

#include "lgbus_drv.h"
#include "lgbus_proc.h"
#include "lgbus_kapi.h"
#include "lgbus_hal.h"

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
 *	main control block for lgbus device.
 *	each minor device has unique control block
 *
 */
typedef struct
{
	// BEGIN of common device
	int						dev_open_count;		///< check if device is opened or not
	dev_t					devno;			///< device number
	struct cdev				cdev;			///< char device structure
	// END of command device

}
LGBUS_DEVICE_T;

typedef struct
{
	// add here extra parameter
	bool	is_suspended;
}LGBUS_DRVDATA_T;


/*----------------------------------------------------------------------------------------
  External Function Prototype Declarations
  ----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
  External Variables
  ----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
  global Functions
  ----------------------------------------------------------------------------------------*/
int		LGBUS_Init(void);
void	LGBUS_Cleanup(void);

/*----------------------------------------------------------------------------------------
  global Variables
  ----------------------------------------------------------------------------------------*/
int		g_lgbus_debug_fd;
int 	g_lgbus_major = LGBUS_MAJOR;
int 	g_lgbus_minor = LGBUS_MINOR;

/*----------------------------------------------------------------------------------------
  Static Function Prototypes Declarations
  ----------------------------------------------------------------------------------------*/
static int      LGBUS_Open(struct inode *, struct file *);
static int      LGBUS_Close(struct inode *, struct file *);
static long		LGBUS_Ioctl(struct file *file, unsigned int cmd, unsigned long arg);

int /*__init*/ LGBUS_probe(struct platform_device *pdev);
static int LGBUS_suspend(struct platform_device *pdev, pm_message_t state);
static int LGBUS_resume(struct platform_device *pdev);
static int  LGBUS_remove(struct platform_device *pdev);
static void  LGBUS_release(struct device *dev);
/*----------------------------------------------------------------------------------------
  Static Variables
  ----------------------------------------------------------------------------------------*/

/**
 * main control block for lgbus device
 */
static LGBUS_DEVICE_T*		g_lgbus_device;

/**
 * file I/O description for lgbus device
 *
 */
static struct file_operations g_lgbus_fops =
{
	.open			= LGBUS_Open,
	.release		= LGBUS_Close,
	.unlocked_ioctl	= LGBUS_Ioctl,
	.read			= NULL,
	.write			= NULL,
};

/*
 *	module platform driver structure
 */
static struct platform_driver lgbus_driver =
{
	.probe          = LGBUS_probe,
	.suspend        = LGBUS_suspend,
	.remove         = LGBUS_remove,
	.resume         = LGBUS_resume,
	.driver         =
	{
		.name   = LGBUS_MODULE,
	},
};

static struct platform_device lgbus_device = {
	.name = LGBUS_MODULE,
	.id = -1,
	.dev = {
		.release = LGBUS_release,
	},
};

/*========================================================================================
  Implementation Group
  ========================================================================================*/

/**
 *
 * probing module.
 *
 * @param	struct platform_device *pdev
 * @return	int 0 : OK , -1 : NOT OK
 *
 */
int /*__init*/ LGBUS_probe(struct platform_device *pdev)
{
	LGBUS_DRVDATA_T *drv_data;

	drv_data = (LGBUS_DRVDATA_T *)kmalloc(sizeof(LGBUS_DRVDATA_T) , GFP_KERNEL);

	// add here driver registering code & allocating resource code

	LGBUS_PRINT("lgbus dev probe\n");
	drv_data->is_suspended = 0;
	platform_set_drvdata(pdev, drv_data);

	return RET_OK;
}

/**
 *
 * suspending module.
 *
 * @param	struct platform_device *pdev pm_message_t state
 * @return	int 0 : OK , -1 : NOT OK
 *
 */
static int LGBUS_suspend(struct platform_device *pdev, pm_message_t state)
{
#ifdef KDRV_CONFIG_PM_LGBUS
	LGBUS_DRVDATA_T *drv_data;

	printk("LGBUS - suspend BEGIN\n");

	drv_data = platform_get_drvdata(pdev);

	// add here the suspend code
	LGBUS_HAL_Suspend();

	drv_data->is_suspended = 1;
	printk("LGBUS - suspend OK\n");
#endif
	return RET_OK;
}

/**
 *
 * resuming module.
 *
 * @param	struct platform_device *
 * @return	int 0 : OK , -1 : NOT OK
 *
 */
static int LGBUS_resume(struct platform_device *pdev)
{
#ifdef KDRV_CONFIG_PM_LGBUS
	LGBUS_DRVDATA_T *drv_data;

	printk("LGBUS - resume BEGIN\n");
	drv_data = platform_get_drvdata(pdev);

	if(drv_data->is_suspended == 0)
	{
		printk("LGBUS - resume FAIL\n");
	}

	// add here the resume code
	LGBUS_HAL_Resume();

	drv_data->is_suspended = 0;
	printk("LGBUS - resume OK\n");
#endif
	return RET_OK;
}

/**
 *
 * module remove function. this function will be called in rmmod fbdev module.
 *
 * @param	struct platform_device
 * @return	int 0 : OK , -1 : NOT OK
 *
 */
static int  LGBUS_remove(struct platform_device *pdev)
{
	LGBUS_DRVDATA_T *drv_data;

	// add here driver unregistering code & deallocating resource code

	drv_data = platform_get_drvdata(pdev);
	kfree(drv_data);

	LGBUS_PRINT("lgbus dev remove\n");

	return RET_OK;
}

/**
 *
 * module release function. this function will be called in rmmod module.
 *
 * @param	struct device *dev
 * @return	int 0 : OK , -1 : NOT OK
 *
 */
static void  LGBUS_release(struct device *dev)
{
	LGBUS_PRINT("lgbus dev release\n");
}


void LGBUS_PreInit(void)
{
	/* TODO: */
}


int LGBUS_Init(void)
{
	int			i;
	int			err;
	dev_t		dev;

	/* Get the handle of debug output for lgbus device.
	 *
	 * Most module should open debug handle before the real initialization of module.
	 * As you know, debug_util offers 4 independent debug outputs for your device driver.
	 * So if you want to use all the debug outputs, you should initialize each debug output
	 * using OS_DEBUG_EnableModuleByIndex() function.
	 */
	g_lgbus_debug_fd = DBG_OPEN( LGBUS_MODULE );

	if ( g_lgbus_debug_fd < 0 )
	{
		printk("[LGBUS] can't get debug handle\n");
	}
	else
	{
		OS_DEBUG_EnableModule ( g_lgbus_debug_fd );

		OS_DEBUG_EnableModuleByIndex ( g_lgbus_debug_fd, LGBUS_LOG_LEVEL_ERROR,	DBG_COLOR_RED );
		OS_DEBUG_EnableModuleByIndex ( g_lgbus_debug_fd, LGBUS_LOG_LEVEL_WARN,	DBG_COLOR_RED );
		OS_DEBUG_EnableModuleByIndex ( g_lgbus_debug_fd, LGBUS_LOG_LEVEL_INFO,	DBG_COLOR_RED );
//		OS_DEBUG_EnableModuleByIndex ( g_lgbus_debug_fd, LGBUS_LOG_LEVEL_PRINT,	DBG_COLOR_RED );
	}
	
	/* register linux platform driver */
	err = platform_driver_register(&lgbus_driver);
	LGBUS_CHECK_CODE(err < 0, return RET_ERROR, "lgbus platform drv reg fail\n");

	err = platform_device_register(&lgbus_device);
	LGBUS_CHECK_CODE(err != RET_OK, return RET_ERROR, "lgbus platform dev reg fail\n");

	g_lgbus_device = (LGBUS_DEVICE_T*)OS_KMalloc( sizeof(LGBUS_DEVICE_T)*LGBUS_MAX_DEVICE );

	if ( NULL == g_lgbus_device )
	{
		DBG_PRINT_ERROR("out of memory. can't allocate %d bytes\n", sizeof(LGBUS_DEVICE_T)* LGBUS_MAX_DEVICE );
		return -ENOMEM;
	}

	memset( g_lgbus_device, 0x0, sizeof(LGBUS_DEVICE_T)* LGBUS_MAX_DEVICE );

	/* register chr device */
	if (g_lgbus_major)
	{
		dev = MKDEV( g_lgbus_major, g_lgbus_minor );
		err = register_chrdev_region(dev, LGBUS_MAX_DEVICE, LGBUS_MODULE );
	}
	else
	{
		err = alloc_chrdev_region(&dev, g_lgbus_minor, LGBUS_MAX_DEVICE, LGBUS_MODULE );
		g_lgbus_major = MAJOR(dev);
	}

	if ( err < 0 )
	{
		DBG_PRINT_ERROR("can't register lgbus device\n" );
		return -EIO;
	}

	/* TODO : initialize your module not specific minor device */


	/* END */

	for ( i=0; i<LGBUS_MAX_DEVICE; i++ )
	{
		/* initialize cdev structure with predefined variable */
		dev = MKDEV( g_lgbus_major, g_lgbus_minor+i );
		cdev_init( &(g_lgbus_device[i].cdev), &g_lgbus_fops );
		g_lgbus_device[i].devno		= dev;
		g_lgbus_device[i].cdev.owner = THIS_MODULE;
		g_lgbus_device[i].cdev.ops   = &g_lgbus_fops;

		/* TODO: initialize minor device */


		/* END */

		err = cdev_add (&(g_lgbus_device[i].cdev), dev, 1 );

		if (err)
		{
			DBG_PRINT_ERROR("error (%d) while adding lgbus device (%d.%d)\n", err, MAJOR(dev), MINOR(dev) );
			return -EIO;
		}
		OS_CreateDeviceClass ( g_lgbus_device[i].devno, "%s%d", LGBUS_MODULE, i );
	}


	if(LGBUS_HAL_Init() < 0)
	{
		LGBUS_ERROR("HAL_Init fail!\n");
	}

	LGBUS_PROC_Init();

	LGBUS_PRINT("lgbus device initialized\n");

	return 0;
}

void LGBUS_Cleanup(void)
{
	int i;
	dev_t dev = MKDEV( g_lgbus_major, g_lgbus_minor );

	LGBUS_PROC_Cleanup();
	LGBUS_HAL_Cleanup();

	/* remove all minor devicies and unregister current device */
	for ( i=0; i<LGBUS_MAX_DEVICE;i++)
	{
		/* TODO: cleanup each minor device */


		/* END */
		cdev_del( &(g_lgbus_device[i].cdev) );
	}

	/* TODO : cleanup your module not specific minor device */

	unregister_chrdev_region(dev, LGBUS_MAX_DEVICE );

	OS_Free( g_lgbus_device );
}


///////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * open handler for lgbus device
 *
 */
static int
LGBUS_Open(struct inode *inode, struct file *filp)
{
	int					major,minor;
	struct cdev*    	cdev;
	LGBUS_DEVICE_T*		lgbus_dev;

	cdev		= inode->i_cdev;
	lgbus_dev	= container_of ( cdev, LGBUS_DEVICE_T, cdev);

	major = imajor(inode);
	minor = iminor(inode);

	if(lgbus_dev->dev_open_count == 0)
	{
		if(LGBUS_HAL_Open() < 0)
			return RET_ERROR;
	}

	lgbus_dev->dev_open_count++;
	filp->private_data = lgbus_dev;

	LGBUS_PRINT("device opened (%d:%d)\n", major, minor );

	return 0;
}

/**
 * release handler for lgbus device
 *
 */
static int
LGBUS_Close(struct inode *inode, struct file *file)
{
	int					major,minor;
	LGBUS_DEVICE_T*		lgbus_dev;
	struct cdev*		cdev;

	cdev		= inode->i_cdev;
	lgbus_dev	= container_of ( cdev, LGBUS_DEVICE_T, cdev);

	if ( lgbus_dev->dev_open_count > 0 )
	{
		--lgbus_dev->dev_open_count;
		if(lgbus_dev->dev_open_count == 0)
		{
			LGBUS_HAL_Close();
		}
	}

	/* TODO : add your device specific code */

	/* END */

	/* some debug */
	major = imajor(inode);
	minor = iminor(inode);
	LGBUS_PRINT("device closed (%d:%d)\n", major, minor );
	return 0;
}

/**
 * ioctl handler for lgbus device.
 *
 *
 * note: if you have some critial data, you should protect them using semaphore or spin lock.
 */
static long LGBUS_Ioctl  (struct file *file, unsigned int cmd, unsigned long arg)
{
	int ret = -EIO;
	int err;
	LGBUS_DEVICE_T*	lgbus_dev;

	/*
	 * get current lgbus device object
	 */
	lgbus_dev	= (LGBUS_DEVICE_T*)file->private_data;

	switch(cmd)
	{
		case LGBUS_IOR_MON_GETINFO:
		{
			LX_LGBUS_MON_INFO_T *info;

			info = (LX_LGBUS_MON_INFO_T *)kmalloc(sizeof(LX_LGBUS_MON_INFO_T), GFP_KERNEL);
			LGBUS_CHECK_CODE(info == NULL, goto exit, "error - lgbus kmalloc\n");

			err = LGBUS_HAL_MonGetInfo(info);
			LGBUS_CHECK_CODE(err < 0, do {kfree(info); goto exit;} while(0), "lgbus getinfo error\n");

			err = copy_to_user((void __user *)arg, (void *)info, sizeof(LX_LGBUS_MON_INFO_T));
			LGBUS_CHECK_CODE(err > 0, do {kfree(info); goto exit;} while(0), "copy to error\n");

			kfree(info);
			ret = 0;
		}
		break;

		case LGBUS_IOW_MON_START:
		{
			LX_LGBUS_MON_CFG_T cfg;

			err = copy_from_user((void *)&cfg, (void __user *)arg, sizeof(LX_LGBUS_MON_CFG_T));
			LGBUS_CHECK_CODE(err > 0, goto exit, "copy from error\n");

			err = LGBUS_HAL_MonStart(&cfg);
			LGBUS_CHECK_CODE(err < 0, goto exit, "lgbus mon start error\n");

			ret = 0;
		}
		break;

		case LGBUS_IOR_MON_READ:
		{
			LX_LGBUS_MON_DATA_T *data;

			data = (LX_LGBUS_MON_DATA_T *)kmalloc(sizeof(LX_LGBUS_MON_DATA_T), GFP_KERNEL);
			LGBUS_CHECK_CODE(data == NULL, goto exit, "error - lgbus kmalloc\n");

			err = LGBUS_HAL_MonRead(data);
			LGBUS_CHECK_CODE(err < 0, do {kfree(data); goto exit;} while(0), "lgbus read error\n");

			err = copy_to_user((void __user *)arg, (void *)data, sizeof(LX_LGBUS_MON_DATA_T));
			LGBUS_CHECK_CODE(err > 0, do {kfree(data); goto exit;} while(0), "copy to error\n");

			kfree(data);
			ret = 0;
		}
		break;

		case LGBUS_IO_MON_STOP:
		{
			err = LGBUS_HAL_MonStop();
			LGBUS_CHECK_CODE(err < 0, goto exit, "lgbus mon stop error\n");
			ret = 0;
		}
		break;

		case LGBUS_IOR_MON_READREG:
		{
			LX_LGBUS_MON_REG_T reg_data;

			err = copy_from_user((void *)&reg_data, (void __user *)arg, sizeof(LX_LGBUS_MON_REG_T));
			LGBUS_CHECK_CODE(err < 0, goto exit, "copy from error\n");

			err = LGBUS_HAL_MonReadReg(&reg_data);
			LGBUS_CHECK_CODE(err < 0, goto exit, "lgbus readreg error\n");

			err = copy_to_user((void __user *)arg, (void *)&reg_data, sizeof(LX_LGBUS_MON_REG_T));
			LGBUS_CHECK_CODE(err > 0, goto exit, "copy to error\n");

			ret = 0;
		}
		break;

		case LGBUS_IOW_MON_WRITEREG:
		{
			LX_LGBUS_MON_REG_T reg_data;

			err = copy_from_user((void *)&reg_data, (void __user *)arg, sizeof(LX_LGBUS_MON_REG_T));
			LGBUS_CHECK_CODE(err > 0, goto exit, "copy from error\n");

			err = LGBUS_HAL_MonWriteReg(&reg_data);
			LGBUS_CHECK_CODE(err < 0, goto exit, "lgbus mon writereg error\n");

			ret = 0;
		}
		break;

		default:
		{
			/* redundant check but it seems more readable */
			LGBUS_ERROR("ioctl:not defined");
			ret = -ENOTTY;
		}
	}

exit:
	return ret;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef KDRV_GLOBAL_LINK
#if defined(CONFIG_LG_BUILTIN_KDRIVER) && defined(CONFIG_LGSNAP)
user_initcall_grp("kdrv", LGBUS_Init);
#else
module_init(LGBUS_Init);
#endif
module_exit(LGBUS_Cleanup);

MODULE_AUTHOR("LGE");
MODULE_DESCRIPTION("base driver");
MODULE_LICENSE("GPL");
#endif

/** @} */

