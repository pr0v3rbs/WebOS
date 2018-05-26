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
 *  main driver implementation for gpu device.
 *	gpu device will teach you how to make device driver with new platform.
 *
 *  author		raxis.lim (raxis.lim@lge.com)
 *  version		1.0
 *  date		2009.12.30
 *  note		Additional information.
 *
 *  @addtogroup lg1150_gpu
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
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#ifdef KDRV_CONFIG_PM	// added by SC Jung for quick booting
#include <linux/platform_device.h>
#endif
#include "gpu_cfg.h"
#include "gpu_drv.h"
#include "base_types.h"
#include "os_util.h"
#include "base_device.h"

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
 *	main control block for gpu device.
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

	//OS_SEM_T				mutex;

// END of device specific data
}
GPU_DEVICE_T;

#ifdef KDRV_CONFIG_PM
typedef struct
{
	// add here extra parameter
	bool			is_suspended;
    bool            is_busy;
}GPU_DRVDATA_T;
#endif

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/
extern	void	GPU_PROC_Init(void);
extern	void	GPU_PROC_Cleanup(void);

extern  void    GPU_H13_InitCtx ( GPU_HW_CTX_T*  ctx );    /* implemented at h13/gpu_drv_h13.c */
extern  void    GPU_M14_InitCtx ( GPU_HW_CTX_T*  ctx );    /* implemented at m14/gpu_drv_m14.c */
extern  void    GPU_H14_InitCtx ( GPU_HW_CTX_T*  ctx );    /* implemented at h14/gpu_drv_h14.c */
extern  void    GPU_H15_InitCtx ( GPU_HW_CTX_T*  ctx );    /* implemented at h15/gpu_drv_h15.c */

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Functions
----------------------------------------------------------------------------------------*/
int		GPU_Init(void);
void	GPU_Cleanup(void);

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/
int		g_gpu_debug_fd;
int 	g_gpu_major = GPU_MAJOR;
int 	g_gpu_minor = GPU_MINOR;

GPU_HW_CTX_T    g_gpu_hw_ctx;

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/
static int      GPU_Open(struct inode *, struct file *);
static int      GPU_Close(struct inode *, struct file *);
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36))
static int 		GPU_Ioctl (struct inode *, struct file *, unsigned int, unsigned long );
#else
static long		GPU_Ioctl(struct file *file, unsigned int cmd, unsigned long arg);
#endif
#ifdef SUPPORT_GPU_DEVICE_READ_WRITE_FOPS
static ssize_t  GPU_Read(struct file *, char *, size_t, loff_t *);
static ssize_t  GPU_Write(struct file *, const char *, size_t, loff_t *);
#endif

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
/**
 * main control block for gpu device
*/
static GPU_DEVICE_T*        g_gpu_device;


/**
 * file I/O description for gpu device
 *
*/
static struct file_operations g_gpu_fops =
{
    .open   = GPU_Open,
    .release= GPU_Close,
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36))
    .ioctl  = GPU_Ioctl,
#else
    .unlocked_ioctl = GPU_Ioctl,
#endif
#ifdef SUPPORT_GPU_DEVICE_READ_WRITE_FOPS
    .read   = GPU_Read,
    .write  = GPU_Write,
#else
    .read   = NULL,
    .write  = NULL,
#endif
};

/*========================================================================================
    Implementation Group
========================================================================================*/
#ifdef KDRV_CONFIG_PM   // added by SC Jung for quick booting
/**
 *
 * suspending module.
 *
 * @param   struct platform_device *pdev pm_message_t state
 * @return  int 0 : OK , -1 : NOT OK
 *
 */
static int GPU_suspend(struct platform_device *pdev, pm_message_t state)
{
#ifdef KDRV_CONFIG_PM_GPU
    GPU_DRVDATA_T *drv_data;

	printk("GPU - suspend BEGIN\n");
    drv_data = platform_get_drvdata(pdev);

    // add here the suspend code


    drv_data->is_suspended = 1;
	printk("GPU - suspend OK\n");
#endif
    return 0;
}

/**
 *
 * resuming module.
 *
 * @param   struct platform_device *
 * @return  int 0 : OK , -1 : NOT OK
 *
 */
static int GPU_resume(struct platform_device *pdev)
{
#ifdef KDRV_CONFIG_PM_GPU
    GPU_DRVDATA_T *drv_data;

	printk("GPU - resume BEGIN\n");

    drv_data = platform_get_drvdata(pdev);

    if(drv_data->is_suspended == 0)
	{
		printk("GPU - resume FAIL\n");
		return -1;
	}

    // add here the resume code

    drv_data->is_suspended = 0;
	printk("GPU - resume OK\n");
#endif
    return 0;
}
/**
 *
 * probing module.
 *
 * @param   struct platform_device *pdev
 * @return  int 0 : OK , -1 : NOT OK
 *
 */
static int __init GPU_probe(struct platform_device *pdev)
{

    GPU_DRVDATA_T *drv_data;

    drv_data = (GPU_DRVDATA_T *)kmalloc(sizeof(GPU_DRVDATA_T) , GFP_KERNEL);

    // add here driver registering code & allocating resource code

    GPU_PRINT("[%s] done probe\n", GPU_MODULE);
    drv_data->is_suspended = 0;
    platform_set_drvdata(pdev, drv_data);

    return 0;
}


/**
 *
 * module remove function. this function will be called in rmmod fbdev module.
 *
 * @param   struct platform_device
 * @return  int 0 : OK , -1 : NOT OK
 *
 */
static int  GPU_remove(struct platform_device *pdev)
{
    GPU_DRVDATA_T *drv_data;

    // add here driver unregistering code & deallocating resource code

    drv_data = platform_get_drvdata(pdev);
    kfree(drv_data);

    GPU_PRINT("released\n");

    return 0;
}

/**
 *
 * module release function. this function will be called in rmmod module.
 *
 * @param   struct device *dev
 * @return  int 0 : OK , -1 : NOT OK
 *
 */
static void  GPU_release(struct device *dev)
{

    GPU_PRINT("device released\n");
}

/*
 *  module platform driver structure
 */
static struct platform_driver gpu_driver =
{
    .probe          = GPU_probe,
    .suspend        = GPU_suspend,
    .remove         = GPU_remove,
    .resume         = GPU_resume,
    .driver         =
    {
        .name   = GPU_MODULE,
    },
};

static struct platform_device gpu_device = {
    .name = GPU_MODULE,
    .id = 0,
    .id = -1,
    .dev = {
        .release = GPU_release,
    },
};
#endif

/** Initialize the device environment before the real H/W initialization
 *
 *  @note main usage of this function is to initialize the HAL layer and memory size adjustment
 *  @note it's natural to keep this function blank :)
 */
void GPU_PreInit(void)
{
    memset( &g_gpu_hw_ctx, 0x0, sizeof(GPU_HW_CTX_T));

    /* initialize HAL (Hardware Abstraction Layer) */
	switch(lx_chip())
	{
#ifdef INCLUDE_H15_CHIP_KDRV
		case LX_CHIP_H15: GPU_H15_InitCtx( &g_gpu_hw_ctx ); break;
#endif
#ifdef INCLUDE_H14_CHIP_KDRV
		case LX_CHIP_H14: GPU_H14_InitCtx( &g_gpu_hw_ctx ); break;
#endif
#ifdef INCLUDE_M14_CHIP_KDRV
		case LX_CHIP_M14: GPU_M14_InitCtx( &g_gpu_hw_ctx ); break;
#endif
#ifdef INCLUDE_H13_CHIP_KDRV
		case LX_CHIP_H13: GPU_H13_InitCtx( &g_gpu_hw_ctx ); break;
#endif
		default:		  DBG_PRINT_ERROR("unknown chip 0x%x\n", lx_chip_rev()); break;
	}
}

int GPU_Init(void)
{
	int			i;
	int			err;
	dev_t		dev;

	/* Get the handle of debug output for gpu device.
	 *
	 * Most module should open debug handle before the real initialization of module.
	 * As you know, debug_util offers 4 independent debug outputs for your device driver.
	 * So if you want to use all the debug outputs, you should initialize each debug output
	 * using OS_DEBUG_EnableModuleByIndex() function.
	 */
	g_gpu_debug_fd = DBG_OPEN( GPU_MODULE );
	if (g_gpu_debug_fd < 0) return -1;

	OS_DEBUG_EnableModule ( g_gpu_debug_fd );

	//OS_DEBUG_EnableModuleByIndex ( g_gpu_debug_fd, 0, DBG_COLOR_NONE );
	//OS_DEBUG_EnableModuleByIndex ( g_gpu_debug_fd, 1, DBG_COLOR_RED );
	//OS_DEBUG_EnableModuleByIndex ( g_gpu_debug_fd, 2, DBG_COLOR_YELLOW);
	//OS_DEBUG_EnableModuleByIndex ( g_gpu_debug_fd, 3, DBG_COLOR_NONE );


	/* allocate main device handler, register current device.
	 *
	 * If devie major is predefined then register device using that number.
	 * otherwise, major number of device is automatically assigned by Linux kernel.
	 *
	 */
#ifdef KDRV_CONFIG_PM
	// added by SC Jung for quick booting
	if(platform_driver_register(&gpu_driver) < 0)
	{
		GPU_PRINT("[%s] platform driver register failed\n",GPU_MODULE);

	}
	else
	{
		if(platform_device_register(&gpu_device))
		{
			platform_driver_unregister(&gpu_driver);
			GPU_PRINT("[%s] platform device register failed\n",GPU_MODULE);
		}
		else
		{
			GPU_PRINT("[%s] platform register done\n", GPU_MODULE);
		}


	}
#endif

	/* TODO : initialize your module not specific minor device */
	g_gpu_device = (GPU_DEVICE_T*)OS_KMalloc( sizeof(GPU_DEVICE_T)*GPU_MAX_DEVICE );

	if ( g_gpu_device == NULL )
	{
		DBG_PRINT_ERROR("out of memory. can't allocate %d bytes\n", sizeof(GPU_DEVICE_T)* GPU_MAX_DEVICE );
		return -ENOMEM;
	}

	memset( g_gpu_device, 0x0, sizeof(GPU_DEVICE_T)* GPU_MAX_DEVICE );

	if (g_gpu_major)
	{
		dev = MKDEV( g_gpu_major, g_gpu_minor );
		err = register_chrdev_region(dev, GPU_MAX_DEVICE, GPU_MODULE );
	}
	else
	{
		err = alloc_chrdev_region(&dev, g_gpu_minor, GPU_MAX_DEVICE, GPU_MODULE );
		g_gpu_major = MAJOR(dev);
	}

	if ( err < 0 )
	{
		DBG_PRINT_ERROR("can't register gpu device\n" );
		return -EIO;
	}

	GPU_InitHW( );

	/* END */

	for ( i=0; i<GPU_MAX_DEVICE; i++ )
	{
		/* initialize cdev structure with predefined variable */
		dev = MKDEV( g_gpu_major, g_gpu_minor+i );
		cdev_init( &(g_gpu_device[i].cdev), &g_gpu_fops );
		g_gpu_device[i].devno		= dev;
		g_gpu_device[i].cdev.owner = THIS_MODULE;
		g_gpu_device[i].cdev.ops   = &g_gpu_fops;

		/* TODO: initialize minor device */

		/* END */

		err = cdev_add (&(g_gpu_device[i].cdev), dev, 1 );

		if (err)
		{
			DBG_PRINT_ERROR("error (%d) while adding gpu device (%d.%d)\n", err, MAJOR(dev), MINOR(dev) );
			return -EIO;
		}
	}

	/* initialize proc system */
	GPU_PROC_Init ( );

	GPU_DEBUG("gpu device initialized\n");

	return 0;
}

void GPU_Cleanup(void)
{
	int i;
	dev_t dev = MKDEV( g_gpu_major, g_gpu_minor );

#ifdef KDRV_CONFIG_PM
	// added by SC Jung for quick booting
	platform_driver_unregister(&gpu_driver);
	platform_device_unregister(&gpu_device);
#endif

	/* cleanup proc system */
	GPU_PROC_Cleanup( );

	/* remove all minor devicies and unregister current device */
	for ( i=0; i<GPU_MAX_DEVICE;i++)
	{
		/* TODO: cleanup each minor device */


		/* END */
		cdev_del( &(g_gpu_device[i].cdev) );
	}

	/* TODO : cleanup your module not specific minor device */

	unregister_chrdev_region(dev, GPU_MAX_DEVICE );

	GPU_DEBUG("gpu device cleanup\n");

	OS_KFree( g_gpu_device );
}


///////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * open handler for gpu device
 *
 */
static int GPU_Open(struct inode *inode, struct file *filp)
{
    int					major,minor;
    GPU_DEVICE_T*		gpu_dev;
    //struct cdev*    	cdev;

    //cdev	= inode->i_cdev;
    gpu_dev	= container_of ( inode->i_cdev, GPU_DEVICE_T, cdev);

    major = imajor(inode);
    minor = iminor(inode);

	if ( minor >= GPU_MAX_DEVICE )
	{
		GPU_PRINT_ERROR( "Invalid device num[%d] for Open!!\n", minor );
		return -1;
	}

	if(gpu_dev->dev_open_count == 0)
	{
		/* do nothing */
	}

    gpu_dev->dev_open_count++;
    filp->private_data = gpu_dev;

    GPU_DEBUG("device opened (%d:%d)\n", major, minor );

    return 0;
}

/**
 * release handler for gpu device
 *
 */
static int GPU_Close(struct inode *inode, struct file *filp)
{
    int					major,minor;
    GPU_DEVICE_T*		gpu_dev;
    //struct cdev*		cdev;

    //cdev	= inode->i_cdev;
    gpu_dev	= container_of ( inode->i_cdev, GPU_DEVICE_T, cdev);

    if ( gpu_dev->dev_open_count > 0 )
    {
        --gpu_dev->dev_open_count;
    }
    major = imajor(inode);
    minor = iminor(inode);

	/* END */

	/* some debug */
    GPU_DEBUG("device closed (%d:%d)\n", major, minor );

    return 0;
}

/**
 * ioctl handler for gpu device.
 *
 *
 * note: if you have some critial data, you should protect them using semaphore or gpun lock.
 */
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36))
static int GPU_Ioctl (struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg)
#else
static long GPU_Ioctl (struct file *filp, unsigned int cmd, unsigned long arg)
#endif
{
    int ret = RET_ERROR;
    int err = 0;

    GPU_DEVICE_T*   my_dev;
    struct cdev*    cdev;

    /*
     * get current gfx device object
     */
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36))
    cdev    = inode->i_cdev;
    my_dev  = container_of ( cdev, GPU_DEVICE_T, cdev);
#else
    my_dev  = (GPU_DEVICE_T*)filp->private_data;
    cdev    = &my_dev->cdev;
#endif

    /*
     * check if IOCTL command is valid or not.
     * - if magic value doesn't match, return error (-ENOTTY)
     * - if command is out of range, return error (-ENOTTY)
     *
     * note) -ENOTTY means "Inappropriate ioctl for device.
     */
    if (_IOC_TYPE(cmd) != GPU_IOC_MAGIC)
    {
        DBG_PRINT_WARNING("invalid magic. magic=0x%02X\n", _IOC_TYPE(cmd) );
        return -ENOTTY;
    }
    if (_IOC_NR(cmd) > GPU_IOC_MAXNR)
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

    GPU_PRINT("%s : IOC 0x%08x (%d)\n", __F__, cmd, _IOC_NR(cmd));

	switch(cmd)
	{
		case GPU_IO_DO_NOTHING:
		{
			/* do nothing */
		}
        break;

        default:
            GPU_PRINT("ioctl: default\n");
            /* redundant check but it seems more readable */
            ret = -ENOTTY;
	}

	return ret;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef KDRV_GLOBAL_LINK
module_init(GPU_Init);
module_exit(GPU_Cleanup);

MODULE_AUTHOR("LGE")
MODULE_DESCRIPTION("GPU Driver");
MODULE_LICENSE("GPL");
#endif

/** @} */

