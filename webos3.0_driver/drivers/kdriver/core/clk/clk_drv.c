/*
 * SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 * Copyright(c) 2013 by LG Electronics Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */


/** @file
 *
 *  main configuration file for clock device
 *
 *  author		gaius.lee
 *  version		1.0
 *  date		2010.03.07
 *  note		Additional information.
 *
 *  @addtogroup lg1150_clock
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
#include <linux/interrupt.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#ifdef KDRV_CONFIG_PM	// added by SC Jung for quick booting
#include <linux/platform_device.h>
#endif
#include "os_util.h"
#include "base_device.h"
#include "clk_drv.h"
#include "clk_core.h"

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
 *	main control block for clock device.
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

// BEGIN of device specific data


// END of device specific data
}
CLK_DEVICE_T;

#ifdef KDRV_CONFIG_PM
typedef struct
{
	// add here extra parameter
	bool	is_suspended;
}CLK_DRVDATA_T;
#endif

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/
extern	void	CLK_PROC_Init(void);
extern	void	CLK_PROC_Cleanup(void);

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Functions
----------------------------------------------------------------------------------------*/
int		CLK_Init(void);
void	CLK_Cleanup(void);

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/
int		g_clk_debug_fd = -1;
int 	g_clk_major = CLK_MAJOR;
int 	g_clk_minor = CLK_MINOR;

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/
static int      CLK_Open(struct inode *, struct file *);
static int      CLK_Close(struct inode *, struct file *);
#ifdef HAVE_UNLOCKED_IOCTL
static long 		CLK_Ioctl (struct file *, unsigned int, unsigned long );
#else
static int 		CLK_Ioctl (struct inode *, struct file *, unsigned int, unsigned long );
#endif
//static ssize_t  CLK_Read(struct file *, char *, size_t, loff_t *);
//static ssize_t  CLK_Write(struct file *, const char *, size_t, loff_t *);

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/

/**
 * main control block for clock device
*/
static CLK_DEVICE_T*		g_clk_device;

/**
 * file I/O description for clock device
 *
*/
static struct file_operations g_clk_fops =
{
	.open 	= CLK_Open,
	.release= CLK_Close,
#ifdef HAVE_UNLOCKED_IOCTL
	.unlocked_ioctl	= CLK_Ioctl,
#else
	.ioctl	= CLK_Ioctl,
#endif
//	.read 	= CLK_Read,
//	.write 	= CLK_Write,
};

/*========================================================================================
	Implementation Group
========================================================================================*/
#ifdef KDRV_CONFIG_PM	// added by SC Jung for quick booting
/**
 *
 * suspending module.
 *
 * @param	struct platform_device *pdev pm_message_t state
 * @return	int 0 : OK , -1 : NOT OK
 *
 */
static int CLK_suspend(struct platform_device *pdev, pm_message_t state)
{
#ifdef KDRV_CONFIG_PM_CLK
	CLK_DRVDATA_T *drv_data;

	printk("CLK - suspend START\n");

	drv_data = platform_get_drvdata(pdev);

	if ( drv_data->is_suspended == 1 )
	{
		printk("CLK - suspend FAIL\n");
		return -1;	//If already in suspend state, so ignore
	}

	// add here the suspend code

	/* Memorize the current register values in the local register variable */

	drv_data->is_suspended = 1;

	printk("CLK - suspend OK\n");
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
static int CLK_resume(struct platform_device *pdev)
{
#ifdef KDRV_CONFIG_PM_CLK
	CLK_DRVDATA_T *drv_data;

	printk("CIK - resume START\n");

	drv_data = platform_get_drvdata(pdev);

	if(drv_data->is_suspended == 0)
	{
		printk("CIK - resume FAIL\n");
		return -1;
	}

	// add here the resume code

	/* Re-enable the interrupts */

	drv_data->is_suspended = 0;

	printk("CIK - resume OK\n");
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
static int __init CLK_probe(struct platform_device *pdev)
{
	CLK_DRVDATA_T *drv_data;

	drv_data = (CLK_DRVDATA_T *)kmalloc(sizeof(CLK_DRVDATA_T) , GFP_KERNEL);


	// add here driver registering code & allocating resource code

	CLK_KDRV_LOG( CLK_TRACE, "[%s] done probe\n", CLK_MODULE);

	drv_data->is_suspended = 0;

	platform_set_drvdata(pdev, drv_data);

	return 0;
}


/**
 *
 * module remove function. this function will be called in rmmod fbdev module.
 *
 * @param	struct platform_device
 * @return	int 0 : OK , -1 : NOT OK
 *
 */
static int CLK_remove(struct platform_device *pdev)
{
	CLK_DRVDATA_T *drv_data;

	// add here driver unregistering code & deallocating resource code



	drv_data = platform_get_drvdata(pdev);
	kfree(drv_data);

	CLK_KDRV_LOG( CLK_TRACE, "released\n");

	return 0;
}

/**
 *
 * module release function. this function will be called in rmmod module.
 *
 * @param	struct device *dev
 * @return	int 0 : OK , -1 : NOT OK
 *
 */
static void  CLK_release(struct device *dev)
{


	CLK_KDRV_LOG( CLK_TRACE, "device released\n");
}

/*
 *	module platform driver structure
 */
static struct platform_driver clk_driver =
{
	.probe          = CLK_probe,
	.suspend        = CLK_suspend,
	.remove         = CLK_remove,
	.resume         = CLK_resume,
	.driver         =
	{
		.name   = CLK_MODULE,
	},
};

static struct platform_device clk_device = {
	.name = CLK_MODULE,
	.id = 0,
	.id = -1,
	.dev = {
		.release = CLK_release,
	},
};
#endif

/** Initialize the device environment before the real H/W initialization
 *
 *  @note main usage of this function is to initialize the HAL layer and memory size adjustment
 *  @note it's natural to keep this function blank :)
 */
void CLK_PreInit(void)
{
    /* TODO: do something */
}

int CLK_Init(void)
{
	int			i;
	int			err;
	dev_t		dev;

	/* Get the handle of debug output for clock device.
	 *
	 * Most module should open debug handle before the real initialization of module.
	 * As you know, debug_util offers 4 independent debug outputs for your device driver.
	 * So if you want to use all the debug outputs, you should initialize each debug output
	 * using OS_DEBUG_EnableModuleByIndex() function.
	 */
	g_clk_debug_fd = DBG_OPEN( CLK_MODULE );

	if ( g_clk_debug_fd >= 0 )
	{
		OS_DEBUG_EnableModule ( g_clk_debug_fd );
	//	OS_DEBUG_EnableModuleByIndex ( g_clk_debug_fd, 0, DBG_COLOR_NONE );
	//	OS_DEBUG_EnableModuleByIndex ( g_clk_debug_fd, 1, DBG_COLOR_NONE );
	//	OS_DEBUG_EnableModuleByIndex ( g_clk_debug_fd, 2, DBG_COLOR_NONE );
	//	OS_DEBUG_EnableModuleByIndex ( g_clk_debug_fd, 3, DBG_COLOR_NONE );
	}

	/* allocate main device handler, register current device.
	 *
	 * If devie major is predefined then register device using that number.
	 * otherwise, major number of device is automatically assigned by Linux kernel.
	 *
	 */
#ifdef KDRV_CONFIG_PM
	// added by SC Jung for quick booting
	if(platform_driver_register(&clk_driver) < 0)
	{
		CLK_KDRV_LOG( CLK_ERROR, "[%s] platform driver register failed\n",CLK_MODULE);

	}
	else
	{
		if(platform_device_register(&clk_device))
		{
			platform_driver_unregister(&clk_driver);
			CLK_KDRV_LOG( CLK_ERROR, "[%s] platform device register failed\n",CLK_MODULE);
		}
		else
		{
			CLK_KDRV_LOG( CLK_TRACE, "[%s] platform register done\n", CLK_MODULE);
		}


	}
#endif

	g_clk_device = (CLK_DEVICE_T*)OS_KMalloc( sizeof(CLK_DEVICE_T)*CLK_MAX_DEVICE );

	if ( NULL == g_clk_device )
	{
		DBG_PRINT_ERROR("out of memory. can't allocate %d bytes\n", sizeof(CLK_DEVICE_T)* CLK_MAX_DEVICE );
		return -ENOMEM;
	}

	memset( g_clk_device, 0x0, sizeof(CLK_DEVICE_T)* CLK_MAX_DEVICE );

	if (g_clk_major)
	{
		dev = MKDEV( g_clk_major, g_clk_minor );
		err = register_chrdev_region(dev, CLK_MAX_DEVICE, CLK_MODULE );
	}
	else
	{
		err = alloc_chrdev_region(&dev, g_clk_minor, CLK_MAX_DEVICE, CLK_MODULE );
		g_clk_major = MAJOR(dev);
	}

	if ( err < 0 )
	{
		DBG_PRINT_ERROR("can't register clk device\n" );
		return -EIO;
	}


	for ( i=0; i<CLK_MAX_DEVICE; i++ )
	{
		/* initialize cdev structure with predefined variable */
		dev = MKDEV( g_clk_major, g_clk_minor+i );
		cdev_init( &(g_clk_device[i].cdev), &g_clk_fops );
		g_clk_device[i].devno		= dev;
		g_clk_device[i].cdev.owner = THIS_MODULE;
		g_clk_device[i].cdev.ops   = &g_clk_fops;

		/* TODO: initialize minor device */


		/* END */

		err = cdev_add (&(g_clk_device[i].cdev), dev, 1 );

		if (err)
		{
			DBG_PRINT_ERROR("error (%d) while adding clk device (%d.%d)\n", err, MAJOR(dev), MINOR(dev) );
			return -EIO;
		}
        OS_CreateDeviceClass ( g_clk_device[i].devno, "%s%d", CLK_MODULE, i );
	}

	/* initialize proc system */
	CLK_PROC_Init ( );

	CLK_KDRV_LOG( CLK_TRACE, "clk device initialized\n");

	return 0;
}

void CLK_Cleanup(void)
{
	int i;
	dev_t dev = MKDEV( g_clk_major, g_clk_minor );

#ifdef KDRV_CONFIG_PM
	// added by SC Jung for quick booting
	platform_driver_unregister(&clk_driver);
	platform_device_unregister(&clk_device);
#endif

	/* cleanup proc system */
	CLK_PROC_Cleanup( );

	/* remove all minor devicies and unregister current device */
	for ( i=0; i<CLK_MAX_DEVICE;i++)
	{
		/* TODO: cleanup each minor device */


		/* END */
		cdev_del( &(g_clk_device[i].cdev) );
	}

	/* TODO : cleanup your module not specific minor device */

	unregister_chrdev_region(dev, CLK_MAX_DEVICE );

	OS_Free( g_clk_device );
}

///////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * open handler for clk device
 *
 */
static int
CLK_Open(struct inode *inode, struct file *filp)
{
    int					major,minor;
    struct cdev*    	cdev;
    CLK_DEVICE_T*	my_dev;

    cdev	= inode->i_cdev;
    my_dev	= container_of ( cdev, CLK_DEVICE_T, cdev);

    /* TODO : add your device specific code */

    if ( my_dev->dev_open_count == 0 )
    {
		/* Device initializations only for the first time */
    }
	/* END */

    my_dev->dev_open_count++;
    filp->private_data = my_dev;

	/* some debug */
    major = imajor(inode);
    minor = iminor(inode);
    CLK_KDRV_LOG( CLK_TRACE, "device opened (%d:%d)\n", major, minor );

    return 0;
}

/**
 * release handler for clk device
 *
 */
static int
CLK_Close(struct inode *inode, struct file *file)
{
    int					major,minor;
    CLK_DEVICE_T*	my_dev;
    struct cdev*		cdev;

    cdev	= inode->i_cdev;
    my_dev	= container_of ( cdev, CLK_DEVICE_T, cdev);

    if ( my_dev->dev_open_count > 0 )
    {
        --my_dev->dev_open_count;
    }

    /* TODO : add your device specific code */

	if (my_dev->dev_open_count == 0)
	{
		/* Last device handle, so close the device */
	}

	/* END */


	/* some debug */
    major = imajor(inode);
    minor = iminor(inode);
    CLK_KDRV_LOG( CLK_TRACE, "device closed (%d:%d)\n", major, minor );
    return 0;
}

/**
 * ioctl handler for clk device.
 *
 *
 * note: if you have some critial data, you should protect them using semaphore or spin lock.
 */
#ifdef HAVE_UNLOCKED_IOCTL
static long
CLK_Ioctl ( struct file *filp, unsigned int cmd, unsigned long arg )
#else
static int
CLK_Ioctl ( struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg )
#endif
{
    int err = 0, ret = 0;

    CLK_DEVICE_T*	my_dev;
    struct cdev*		cdev;

	/*
	 * get current clk device object
	 */
#ifdef HAVE_UNLOCKED_IOCTL
	my_dev	= (CLK_DEVICE_T *)filp->private_data;
	cdev	= &my_dev->cdev;
#else
    cdev	= inode->i_cdev;
    my_dev	= container_of ( cdev, CLK_DEVICE_T, cdev);
#endif
    /*
     * check if IOCTL command is valid or not.
     * - if magic value doesn't match, return error (-ENOTTY)
     * - if command is out of range, return error (-ENOTTY)
     *
     * note) -ENOTTY means "Inappropriate ioctl for device.
     */
    if (_IOC_TYPE(cmd) != CLK_IOC_MAGIC)
    {
    	DBG_PRINT_WARNING("invalid magic. magic=0x%02X\n", _IOC_TYPE(cmd) );
    	return -ENOTTY;
    }
    if (_IOC_NR(cmd) > CLK_IOC_MAXNR)
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

//	CLK_KDRV_LOG( CLK_TRACE, "cmd = %08X (cmd_idx=%d)\n", cmd, _IOC_NR(cmd) );

	switch(cmd)
	{
		case CLK_IOW_OPEN:
			{
				CLK_DD_Open(arg);
				CLK_KDRV_LOG( CLK_TRACE, "CLK_DD_Open ok\n");
				ret = 0;
			}
			break;

		case CLK_IOR_GET_CURRENTGSTC:
			{
				ret = CLK_DD_GetGstc(arg);
				CLK_KDRV_LOG( CLK_TRACE, "CLK_DD_GetGstc ok\n");
			}
			break;

		case CLK_IOR_GET_TIMESTAMP:
			{
				ret = CLK_DD_GetTimestamp(arg);
				CLK_KDRV_LOG( CLK_TRACE, "CLK_DD_GetTimestamp ok\n");
			}
			break;

		case CLK_IOR_GET_GSTC:
			{
				ret = CLK_DD_RevertTime(arg);
				CLK_KDRV_LOG( CLK_TRACE, "CLK_DD_RevertTime ok\n");
			}
			break;

		case CLK_IOW_SLAMTIME:
			{
				ret = CLK_DD_SlamTime(arg);
				CLK_KDRV_LOG( CLK_TRACE, "CLK_DD_SlamTime ok\n");
			}
			break;

		case CLK_IOW_SETRATE:
			{
				ret = CLK_DD_SetRate(arg);
				CLK_KDRV_LOG( CLK_TRACE, "CLK_DD_SetRate ok\n");
			}
			break;

		case CLK_IOR_GETRATE:
			{
				ret = CLK_DD_GetRate(arg);
				CLK_KDRV_LOG( CLK_TRACE, "CLK_DD_GetRate ok\n");
			}
			break;

		default :
			/* redundant check but it seems more readable */
    	    ret = -ENOTTY;
			break;
	}

    return ret;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef KDRV_GLOBAL_LINK
#if defined(CONFIG_LG_BUILTIN_KDRIVER) && defined(CONFIG_LGSNAP)
user_initcall_grp("kdrv",CLK_Init);
#else
module_init(CLK_Init);
#endif
module_exit(CLK_Cleanup);

MODULE_AUTHOR("LGE");
MODULE_DESCRIPTION("base driver");
MODULE_LICENSE("GPL");
#endif

/** @} */

