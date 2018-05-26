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
 *  main driver implementation for pm device.
 *	pm device will teach you how to make device driver with new platform.
 *
 *  author		hankyung.yu (hankyung.yu@lge.com)
 *  version		1.0
 *  date			2014.07.18
 *  note			Additional information.
 *
 *  @addtogroup lg1210_pm
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/
#undef	SUPPORT_PM_DEVICE_READ_WRITE_FOPS
#undef	PM_DRV_PRINT_ENABLE

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#ifdef KDRV_CONFIG_PM	// added by SC Jung for quick booting
#include <linux/platform_device.h>
#endif
#include <asm/uaccess.h>
#include <linux/poll.h>
#include <linux/irq.h>
#include <linux/interrupt.h>
#include <linux/sched.h>
#include <linux/cpu.h>
#include <linux/io.h>

#include <linux/device.h>
#include <linux/init.h>
#include <linux/kernel_stat.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/slab.h>
#include <linux/suspend.h>

#include <linux/tick.h>
#include <trace/events/power.h>

#include <linux/syscalls.h>
#include <linux/file.h>
#include <linux/fcntl.h>
#include <asm/uaccess.h>

#include <linux/kthread.h>
#include <linux/completion.h>
#include <linux/regulator/consumer.h>
#include <linux/delay.h>

#include "os_util.h"
#include "base_device.h"
#include "pm_drv.h"
#include "pm_core.h"

#include "../../chip/h15/pm/pm_h15common.h"
/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

//#define PM_DEBUG_PRINT

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define PM_COPY_FROM_USER(d,s,l) 							\
		do {												\
			if (copy_from_user((void*)d, (void *)s, l)) {	\
				PM_ERROR("ioctl: copy_from_user\n");		\
				return -EFAULT; 							\
			}												\
		} while(0)

#define PM_COPY_TO_USER(d,s,l) 							\
		do {												\
			if (copy_to_user((void*)d, (void *)s, l)) { 	\
				PM_ERROR("ioctl: copy_to_user\n");		\
				return -EFAULT; 							\
			}												\
		} while(0)


#define CPU1_NO_HOTPLUG	0
#define CPU1_UP	1
#define CPU1_DOWN	2

#define PMS_VER_MAGIC	0x1411190A

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/
/**
 *	main control block for pm device.
 *	each minor device has unique control block
 *
 */
typedef struct PM_DEVICE_t
{
// BEGIN of common device
	int						dev_open_count;		///< check if device is opened or not
	dev_t					devno;			///< device number
	struct cdev				cdev;			///< char device structure
// END of command device

// BEGIN of device specific data
	OS_SEM_T				mutex;
// END of device specific data
}
PM_DEVICE_T;


#ifdef KDRV_CONFIG_PM
typedef struct
{
	// add here extra parameter
	bool			is_suspended;
}PM_DRVDATA_T;
#endif

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/
extern	void	PM_PROC_Init(void);
extern	void	PM_PROC_Cleanup(void);
extern	void 	loglevel_change(unsigned int level);

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Functions
----------------------------------------------------------------------------------------*/
int		PM_Init(void);
void	PM_Cleanup(void);

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/
int		g_pm_debug_fd;
int 	g_pm_major = PM_MAJOR;
int 	g_pm_minor = PM_MINOR;

static pms_driver_t* pms_driver = NULL;
/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/
static int      PM_Open(struct inode *, struct file *);
static int      PM_Close(struct inode *, struct file *);
static long		PM_Ioctl (struct file *file, unsigned int cmd, unsigned long arg);

#ifdef SUPPORT_PM_DEVICE_READ_WRITE_FOPS
static ssize_t  PM_Read(struct file *, char *, size_t, loff_t *);
static ssize_t  PM_Write(struct file *, const char *, size_t, loff_t *);
#endif

//static unsigned int PM_Poll(struct file *filp, poll_table *wait);


/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/

/**
 * main control block for pm device
*/
static PM_DEVICE_T*		g_pm_device;

/**
 * file I/O description for pm device
 *
*/
static struct file_operations g_pm_fops =
{
	.open 	= PM_Open,
	.release= PM_Close,
	.unlocked_ioctl	= PM_Ioctl,
#ifdef SUPPORT_PM_DEVICE_READ_WRITE_FOPS
	.read 	= PM_Read,
	.write 	= PM_Write,
#else
	.read	= NULL,
	.write	= NULL,
#endif
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
 * @return	int 0 : OK, -1 : NOT OK
 *
 */
static int PM_suspend(struct platform_device *pdev, pm_message_t state)
{
#ifdef KDRV_CONFIG_PM_PM
        PM_DRVDATA_T    *drv_data;
        drv_data = platform_get_drvdata(pdev);

        printk("PM - suspend BEGIN\n");

        PM_DevSuspend();

        drv_data->is_suspended = 1;

		if (pms_driver)
			pms_driver->suspend();

        printk("PM - suspend OK\n");
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
static int PM_resume(struct platform_device *pdev)
{
#ifdef KDRV_CONFIG_PM_PM
        PM_DRVDATA_T    *drv_data;

        printk("PM - resume BEGIN\n");

        drv_data = platform_get_drvdata(pdev);
        if(drv_data->is_suspended == 0)
        {
                printk("PM - resume FAIL\n");
                return -1;
        }

        PM_DevResume();

        drv_data->is_suspended = 0;

		if (pms_driver)
			pms_driver->resume();

        printk("PM - resume OK\n");
#endif
        return 0;
}
/**
 *
 * probing module.
 *
 * @param	struct platform_device *pdev
 * @return	int 0 : OK, -1 : NOT OK
 *
 */
 int  PM_probe(struct platform_device *pdev)
{

	PM_DRVDATA_T *drv_data;

	drv_data = (PM_DRVDATA_T *)kmalloc(sizeof(PM_DRVDATA_T) , GFP_KERNEL);

	// add here driver registering code & allocating resource code

	PM_PRINT("[%s] done probe\n", PM_MODULE);
	drv_data->is_suspended = 0;
	platform_set_drvdata(pdev, drv_data);

	return 0;
}


/**
 *
 * module remove function, this function will be called in rmmod pm module
 *
 * @param	struct platform_device
 * @return	int 0 : OK , -1 : NOT OK
 *
 */
static int  PM_remove(struct platform_device *pdev)
{
	PM_DRVDATA_T *drv_data;

	// add here driver unregistering code & deallocating resource code

	drv_data = platform_get_drvdata(pdev);
	kfree(drv_data);

	PM_PRINT("released\n");

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
static void  PM_release(struct device *dev)
{
	PM_PRINT("device released\n");
}

/*
 *	module platform driver structure
 */
static struct platform_driver pm_driver =
{
	.probe		= PM_probe,
	.suspend	= PM_suspend,
	.remove		= PM_remove,
	.resume		= PM_resume,
	.driver		=
	{
		.name	= PM_MODULE,
	},
};

static struct platform_device pm_device = {
	.name = PM_MODULE,
	.id = 0,
	.id = -1,
	.dev = {
		.release = PM_release,
	},
};
#endif

/** Initialize the device environment before the real H/W initialization
 *
 *  @note main usage of this function is to initialize the HAL layer and memory size adjustment
 *  @note it's natural to keep this function blank :)
 */
void PM_PreInit(void)
{
    /* TODO: do something */
}

int PM_Init(void)
{
	int			i;
	int			err;
	dev_t		dev;
//	unsigned int  nothing = 0;

	/* Get the handle of debug output for pm device.
	 *
	 * Most module should open debug handle before the real initialization of module.
	 * As you know, debug_util offers 4 independent debug outputs for your device driver.
	 * So if you want to use all the debug outputs, you should initialize each debug output
	 * using OS_DEBUG_EnableModuleByIndex() function.
	 */
	g_pm_debug_fd = DBG_OPEN( PM_MODULE );
	if(g_pm_debug_fd < 0) return -1;

	OS_DEBUG_EnableModule ( g_pm_debug_fd );

#if 0
	OS_DEBUG_EnableModuleByIndex ( g_pm_debug_fd, PM_MSG_TRACE, DBG_COLOR_NONE );
	OS_DEBUG_EnableModuleByIndex ( g_pm_debug_fd, PM_MSG_TRACE, DBG_COLOR_NONE );
#endif
	OS_DEBUG_EnableModuleByIndex ( g_pm_debug_fd, PM_MSG_DEBUG, DBG_COLOR_NONE );
	OS_DEBUG_EnableModuleByIndex ( g_pm_debug_fd, PM_MSG_ERROR, DBG_COLOR_RED );
	OS_DEBUG_EnableModuleByIndex ( g_pm_debug_fd, PM_MSG_NOTI, DBG_COLOR_BLACK );
	/* allocate main device handler, register current device.
	 *
	 * If devie major is predefined then register device using that number.
	 * otherwise, major number of device is automatically assigned by Linux kernel.
	 *
	 */
#ifdef KDRV_CONFIG_PM
	// added by SC Jung for quick booting
	if(platform_driver_register(&pm_driver) < 0)
	{
		PM_PRINT("[%s] platform driver register failed\n",PM_MODULE);
	}
	else
	{
		if(platform_device_register(&pm_device))
		{
			platform_driver_unregister(&pm_driver);
			PM_PRINT("[%s] platform device register failed\n",PM_MODULE);
		}
		else
		{
			PM_PRINT("[%s] platform register done\n", PM_MODULE);
		}
	}
#endif

	PM_DevInit();

	g_pm_device = (PM_DEVICE_T*)OS_KMalloc( sizeof(PM_DEVICE_T)*PM_MAX_DEVICE );

	if ( NULL == g_pm_device )
	{
		DBG_PRINT_ERROR("out of memory. can't allocate %d bytes\n", sizeof(PM_DEVICE_T)* PM_MAX_DEVICE );
		return -ENOMEM;
	}

	memset( g_pm_device, 0x0, sizeof(PM_DEVICE_T)* PM_MAX_DEVICE );

	if (g_pm_major)
	{
		dev = MKDEV( g_pm_major, g_pm_minor );
		err = register_chrdev_region(dev, PM_MAX_DEVICE, PM_MODULE );
	}
	else
	{
		err = alloc_chrdev_region(&dev, g_pm_minor, PM_MAX_DEVICE, PM_MODULE );
		g_pm_major = MAJOR(dev);
	}

	if ( err < 0 )
	{
		DBG_PRINT_ERROR("can't register pm device\n" );
		return -EIO;
	}

	/* TODO : initialize your module not specific minor device */


	/* END */
	for ( i=0; i<PM_MAX_DEVICE; i++ )
	{
		/* initialize cdev structure with predefined variable */
		dev = MKDEV( g_pm_major, g_pm_minor+i );
		cdev_init( &(g_pm_device[i].cdev), &g_pm_fops );
		g_pm_device[i].devno		= dev;
		g_pm_device[i].cdev.owner = THIS_MODULE;
		g_pm_device[i].cdev.ops   = &g_pm_fops;

		/* TODO: initialize minor device */


		/* END */

		err = cdev_add (&(g_pm_device[i].cdev), dev, 1 );

		if (err)
		{
			DBG_PRINT_ERROR("error (%d) while adding pm device (%d.%d)\n", err, MAJOR(dev), MINOR(dev) );
			return -EIO;
		}
        OS_CreateDeviceClass ( g_pm_device[i].devno, "%s%d", PM_MODULE, i );
	}

	/* initialize system */
	PM_NOTI("--------------------------------------------------\n");
	if (( lx_chip_rev() & 0xFFFFFF00) == (LX_CHIP_REV(H15,A0) & 0xFFFFFF00))
	{
		PM_NOTI("H15 CPU Power Management Init\n");
		PM_NOTI("PMS VER[%8x]\n",PMS_VER_MAGIC);
		pms_driver = get_h15pm_driver();
		pms_driver->init();

		PM_PROC_Init();
	}
	PM_PRINT("pm device initialized\n");
	PM_NOTI("--------------------------------------------------\n");
	return 0;
}

void PM_Cleanup(void)
{
	int i;
	dev_t dev = MKDEV( g_pm_major, g_pm_minor );

#ifdef KDRV_CONFIG_PM
	// added by SC Jung for quick booting
	platform_driver_unregister(&pm_driver);
	platform_device_unregister(&pm_device);
#endif

	/* cleanup proc system */
	PM_PROC_Cleanup( );

	/* remove all minor devicies and unregister current device */
	for ( i=0; i<PM_MAX_DEVICE;i++)
	{
		/* TODO: cleanup each minor device */


		/* END */
		cdev_del( &(g_pm_device[i].cdev) );
	}

	/* TODO : cleanup your module not specific minor device */

	unregister_chrdev_region(dev, PM_MAX_DEVICE );

	OS_Free( g_pm_device );
}


///////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * open handler for pm device
 *
 */
static int PM_Open(struct inode *inode, struct file *filp)
{
    int					major,minor;
    struct cdev*    	cdev;
    PM_DEVICE_T*	my_dev;

    cdev	= inode->i_cdev;
    my_dev	= container_of ( cdev, PM_DEVICE_T, cdev);

    /* TODO : add your device specific code */

	/* END */

	if(my_dev->dev_open_count == 0)
	{

	}

    my_dev->dev_open_count++;
    filp->private_data = my_dev;

	/* some debug */
    major = imajor(inode);
    minor = iminor(inode);
    PM_PRINT("device opened (%d:%d)\n", major, minor );

    return 0;
}

/**
 * release handler for pm device
 *
 */
static int PM_Close(struct inode *inode, struct file *file)
{
    int					major,minor;
    PM_DEVICE_T*	my_dev;
    struct cdev*		cdev;

    cdev	= inode->i_cdev;
    my_dev	= container_of ( cdev, PM_DEVICE_T, cdev);

    if ( my_dev->dev_open_count > 0 )
    {
        --my_dev->dev_open_count;
    }

    /* TODO : add your device specific code */

	/* END */

	/* some debug */
    major = imajor(inode);
    minor = iminor(inode);
    PM_PRINT("device closed (%d:%d)\n", major, minor );
    return 0;
}



/**
 * ioctl handler for pm device.
 *
 *
 * note: if you have some critial data, you should protect them using semaphore or spin lock.
 */
static long PM_Ioctl (struct file *file, unsigned int cmd, unsigned long arg)
{
	PM_DEVICE_T*	pm_dev;
//	int err = 0;
	int ret = 0;

	if ( lx_chip_rev() < LX_CHIP_REV(H15,A0) )
		return 0;

	/*
	  * get current pm device object
	  */
	pm_dev = (PM_DEVICE_T*)file->private_data;

    /*
	  * check if IOCTL command is valid or not.
	  * - if magic value doesn't match, return error (-ENOTTY)
	  * - if command is out of range, return error (-ENOTTY)
	  *
	  * note) -ENOTTY means "Inappropriate ioctl for device.
	  */

	if ((_IOC_TYPE(cmd) != PM_DEV_MAJOR) || (_IOC_NR(cmd) != 'L'))
		return -EINVAL;

	if (pms_driver)
		pms_driver->ioctl(cmd, arg);

    return ret;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef KDRV_GLOBAL_LINK
#if defined(CONFIG_LG_BUILTIN_KDRIVER) && defined(CONFIG_LGSNAP)
user_initcall_grp("kdrv",PM_Init);
#else
module_init(PM_Init);
#endif
module_exit(PM_Cleanup);

MODULE_AUTHOR("LGE");
MODULE_DESCRIPTION("pm driver");
MODULE_LICENSE("GPL");

#endif

/** @} */

