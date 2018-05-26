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
 *  main driver implementation for sci device.
 *	sci device will teach you how to make device driver with new platform.
 *
 *  author		bongrae.cho (bongrae.cho@lge.com)
 *  version		1.0
 *  date		2012.02.23
 *  note		Additional information.
 *
 *  @addtogroup lg1154_sci
 *	@{
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/
#undef	SUPPORT_SCI_DEVICE_READ_WRITE_FOPS
#undef	SCI_DRV_PRINT_ENABLE
//#define SCI_DRV_PRINT_ENABLE
//static void Debug_SCI_Print(void);

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/slab.h> // for hibernation, if KDRV_CONFIG_PM included, kmalloc, kfree build error

#ifdef KDRV_CONFIG_PM
#include <linux/platform_device.h>
#endif

#include "os_util.h"
#include "base_types.h"
#include "base_device.h"

#include "sci_drv.h"
#include "sci_core.h"
#include "sci_reg.h"
#include "sci_isr.h"
#include "sci_err.h"

#include "sci_ctop_cfg.h"

#include "sci_kapi.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define SCI_MUTEX_INIT(h)				OS_InitMutex(&h->mutex, OS_SEM_ATTR_DEFAULT)
#define SCI_MUTEX_LOCK(h)				OS_LockMutex(&h->mutex)
#define SCI_MUTEX_UNLOCK(h)			    OS_UnlockMutex(&h->mutex)


/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/
/**
 *	main control block for sci device.
 *	each minor device has unique control block
 *
 */

typedef struct SCI_DEVICE_t
{
// BEGIN of common device
	int						dev_open_count;		///< check if device is opened or not
	UINT32				    chip;
	UINT32				    platform;
	dev_t					devno;			///< device number
	struct cdev				cdev;			///< char device structure
// END of command device

// BEGIN of device specific data
	OS_SEM_T				mutex;

    SCI_INFO_T              info;
}
SCI_DEVICE_T;

#ifdef KDRV_CONFIG_PM
typedef struct
{
	// add here extra parameter
	bool			is_suspended;
}SCI_DRVDATA_T;
#endif

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Global Functions Declarations
----------------------------------------------------------------------------------------*/
int		SCI_Init(void);
void	SCI_Cleanup(void);

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/
int		g_sci_debug_fd;
int 	g_sci_major = SCI_MAJOR;
int 	g_sci_minor = SCI_MINOR;


/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/
static int      SCI_Open(struct inode *, struct file *);
static int      SCI_Close(struct inode *, struct file *);
#ifdef HAVE_UNLOCKED_IOCTL
static long 		SCI_Ioctl (struct file *, unsigned int, unsigned long );
#else
static int 		SCI_Ioctl (struct inode *, struct file *, unsigned int, unsigned long );
#endif
#ifdef SUPPORT_SCI_DEVICE_READ_WRITE_FOPS
static ssize_t  SCI_Read(struct file *, char *, size_t, loff_t *);
static ssize_t  SCI_Write(struct file *, const char *, size_t, loff_t *);
#endif

static irqreturn_t SCI_IrqHandler(int irq, void *dev, struct pt_regs *regs);

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
/**
 * main control block for sci device
*/
static SCI_DEVICE_T*		g_sci_device;

/**
 * file I/O description for sci device
 *
*/
static struct file_operations g_sci_fops =
{
	.open 	= SCI_Open,
	.release= SCI_Close,
#ifdef HAVE_UNLOCKED_IOCTL
	.unlocked_ioctl	= SCI_Ioctl,
#else
	.ioctl	= SCI_Ioctl,
#endif
#ifdef SUPPORT_SCI_DEVICE_READ_WRITE_FOPS
	.read 	= SCI_Read,
	.write 	= SCI_Write,
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

static int SCI_suspend(struct platform_device *pdev, pm_message_t state)
{
#ifdef KDRV_CONFIG_PM_SCI
	SCI_DRVDATA_T	*drv_data;
	drv_data = platform_get_drvdata(pdev);

	printk("SCI - suspend BEGIN\n");

	//Debug_SCI_Print();

	// add here the suspend code

	drv_data->is_suspended = 1;
	printk("SCI - suspend END\n");
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
static int SCI_resume(struct platform_device *pdev)
{
#ifdef KDRV_CONFIG_PM_SCI
	SCI_DRVDATA_T	*drv_data;
	drv_data = platform_get_drvdata(pdev);

	printk("SCI - resume BEGIN\n");

	if(drv_data->is_suspended == 0)
	{
		printk("SCI - resume FAIL@%d\n",__LINE__);
        return -1;
	}

	/* default H/W Init */
	g_sci_device->info.clock_supplied_khz = SCI_CTOP_Clock_Config();
    if(g_sci_device->info.clock_supplied_khz == 0)
    {
		printk("SCI - resume FAIL@%d\n",__LINE__);
        DBG_PRINT_ERROR("SCI clock Init error(Unknown Chip Rev)\n");
        return -1;
    }
	SCI_RegSetInit();

	drv_data->is_suspended = 0;
	//Debug_SCI_Print();
	printk("SCI - resume OK\n");
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
int  SCI_probe(struct platform_device *pdev)
{

	SCI_DRVDATA_T *drv_data;

	drv_data = (SCI_DRVDATA_T *)kmalloc(sizeof(SCI_DRVDATA_T) , GFP_KERNEL);


	// add here driver registering code & allocating resource code


	SCI_PRINT("[%s] done probe\n", SCI_MODULE);
	drv_data->is_suspended = 0;
	platform_set_drvdata(pdev, drv_data);

	return 0;
}


/**
 *
 * module remove function, this function will be called in rmmod sci module
 *
 * @param	struct platform_device
 * @return	int 0 : OK , -1 : NOT OK
 *
 */
static int  SCI_remove(struct platform_device *pdev)
{
	SCI_DRVDATA_T *drv_data;

	// add here driver unregistering code & deallocating resource code


	drv_data = platform_get_drvdata(pdev);
	kfree(drv_data);

	SCI_PRINT("released\n");

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
static void  SCI_release(struct device *dev)
{

	SCI_PRINT("device released\n");
}

/*
 *	module platform driver structure
 */
static struct platform_driver sci_driver =
{
	.probe		= SCI_probe,
	.suspend	= SCI_suspend,
	.remove		= SCI_remove,
	.resume		= SCI_resume,
	.driver		=
	{
		.name	= SCI_MODULE,
	},
};

static struct platform_device sci_device = {
	.name = SCI_MODULE,
	.id = 0,
	.id = -1,
	.dev = {
		.release = SCI_release,
	},
};
#endif

/** Initialize the device environment before the real H/W initialization
 *
 *  @note main usage of this function is to initialize the HAL layer and memory size adjustment
 *  @note it's natural to keep this function blank :)
 */
void SCI_PreInit(void)
{
    /* TODO: do something */
}

int SCI_Init(void)
{
	int			err;
	dev_t		dev;

	/* Get the handle of debug output for sci device.
	 *
	 * Most module should open debug handle before the real initialization of module.
	 * As you know, debug_util offers 4 independent debug outputs for your device driver.
	 * So if you want to use all the debug outputs, you should initialize each debug output
	 * using OS_DEBUG_EnableModuleByIndex() function.
	 */
	g_sci_debug_fd = DBG_OPEN( SCI_MODULE );
	if(g_sci_debug_fd < 0)
        return -1;
	OS_DEBUG_EnableModule ( g_sci_debug_fd );

	OS_DEBUG_EnableModuleByIndex ( g_sci_debug_fd, LX_LOGM_LEVEL_ERROR, DBG_COLOR_NONE );
	OS_DEBUG_EnableModuleByIndex ( g_sci_debug_fd, LX_LOGM_LEVEL_WARNING, DBG_COLOR_NONE );

	/* allocate main device handler, register current device.
	 *
	 * If devie major is predefined then register device using that number.
	 * otherwise, major number of device is automatically assigned by Linux kernel.
	 *
	 */
#ifdef KDRV_CONFIG_PM
	// added by SC Jung for quick booting
	if(platform_driver_register(&sci_driver) < 0)
		SCI_PRINT("[%s] platform driver register failed\n", SCI_MODULE);
	else
	{
		if(platform_device_register(&sci_device))
		{
			platform_driver_unregister(&sci_driver);
			SCI_PRINT("[%s] platform device register failed\n", SCI_MODULE);
		}
		else
			SCI_PRINT("[%s] platform register done\n", SCI_MODULE);


	}
#endif

	g_sci_device = (SCI_DEVICE_T*)OS_KMalloc( sizeof(SCI_DEVICE_T));

	if ( NULL == g_sci_device )
	{
		DBG_PRINT_ERROR("out of memory. can't allocate %d bytes\n", sizeof(SCI_DEVICE_T));
		return -ENOMEM;
	}

	memset( g_sci_device, 0x0, sizeof(SCI_DEVICE_T));

	if (g_sci_major)
	{
		dev = MKDEV( g_sci_major, g_sci_minor );
		err = register_chrdev_region(dev, SCI_MAX_DEVICE, SCI_MODULE );
	}
	else
	{
		err = alloc_chrdev_region(&dev, g_sci_minor, SCI_MAX_DEVICE, SCI_MODULE );
		g_sci_major = MAJOR(dev);
	}

	if ( err < 0 )
	{
		DBG_PRINT_ERROR("can't register sci device\n" );
		return -EIO;
	}

	/* TODO : initialize your module not specific minor device */


	/* END */

	/* initialize cdev structure with predefined variable */
	dev = MKDEV( g_sci_major, g_sci_minor);
	cdev_init( &(g_sci_device->cdev), &g_sci_fops );
	g_sci_device->devno		= dev;
	g_sci_device->cdev.owner = THIS_MODULE;
	g_sci_device->cdev.ops   = &g_sci_fops;

	/* TODO: initialize minor device */


	/* END */

	err = cdev_add (&(g_sci_device->cdev), dev, 1);

	if (err)
	{
		DBG_PRINT_ERROR("error (%d) while adding sci device (%d.%d)\n", err, MAJOR(dev), MINOR(dev) );
		return -EIO;
	}
    OS_CreateDeviceClass ( g_sci_device->devno, "%s", SCI_MODULE);

    g_sci_device->info.clock_supplied_khz = SCI_CTOP_Clock_Config();
    if(g_sci_device->info.clock_supplied_khz == 0)
    {
        DBG_PRINT_ERROR("SCI clock Init error(Unknown Chip Rev)\n");
        return -1;
    }

    err = SCI_RegInit();

    if(err != 0)
    {
        DBG_PRINT_ERROR("reg Init error\n");
        return -1;
    }

    if(request_irq(H13_IRQ_SCI, (irq_handler_t)SCI_IrqHandler, IRQF_DISABLED, "SCI", &g_sci_device->info))
    {
        DBG_PRINT_ERROR("IRQ[%d] request error\n", H13_IRQ_SCI);
        return -1;
    }

	SCI_PRINT("sci device initialized\n");

	return 0;
}

void SCI_Cleanup(void)
{
	dev_t dev = MKDEV( g_sci_major, g_sci_minor);

	free_irq (H13_IRQ_SCI, &g_sci_device->info);

#ifdef KDRV_CONFIG_PM
	// added by SC Jung for quick booting
	platform_driver_unregister(&sci_driver);
	platform_device_unregister(&sci_device);
#endif

	/* remove all minor devicies and unregister current device */
	/* TODO: cleanup each minor device */

	/* END */
	cdev_del( &(g_sci_device->cdev) );

	/* TODO : cleanup your module not specific minor device */

	unregister_chrdev_region(dev, SCI_MAX_DEVICE);

    SCI_RegCleanup();

	OS_KFree( g_sci_device );
}


///////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * open handler for sci device
 *
 */
static int SCI_Open(struct inode *inode, struct file *filp)
{
    int					major,minor;
    SCI_DEVICE_T*	my_dev;

    my_dev	= container_of ( inode->i_cdev, SCI_DEVICE_T, cdev);

    /* TODO : add your device specific code */


	/* END */

	if(my_dev->dev_open_count == 0)
	{
		SCI_MUTEX_INIT(my_dev);
	}

    my_dev->dev_open_count++;
    filp->private_data = my_dev;

	/* some debug */
    major = imajor(inode);
    minor = iminor(inode);
    SCI_PRINT("device opened (%d:%d)\n", major, minor );

    return 0;
}

/**
 * release handler for sci device
 *
 */
static int SCI_Close(struct inode *inode, struct file *file)
{
    int					major,minor;
    SCI_DEVICE_T*	my_dev;

    my_dev	= container_of ( inode->i_cdev, SCI_DEVICE_T, cdev);

    if ( my_dev->dev_open_count > 0 )
    {
        --my_dev->dev_open_count;
    }

    /* TODO : add your device specific code */

	/* END */

	/* some debug */
    major = imajor(inode);
    minor = iminor(inode);
    SCI_PRINT("device closed (%d:%d)\n", major, minor );
    return 0;
}

/**
 * ioctl handler for sci device.
 *
 *
 * note: if you have some critial data, you should protect them using semaphore or spin lock.
 */
#ifdef HAVE_UNLOCKED_IOCTL
static long SCI_Ioctl ( struct file *filp, unsigned int cmd, unsigned long arg )
#else
static int SCI_Ioctl ( struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg )
#endif
{
    int err = 0, ret = 0;
    SCI_DEVICE_T*	my_dev;

	/*
	 * get current sci device object
	 */
#ifdef HAVE_UNLOCKED_IOCTL
	my_dev	= (SCI_DEVICE_T *)filp->private_data;
#else
    my_dev	= container_of ( inode->i_cdev, SCI_DEVICE_T, cdev);
#endif
    /*
     * check if IOCTL command is valid or not.
     * - if magic value doesn't match, return error (-ENOTTY)
     * - if command is out of range, return error (-ENOTTY)
     *
     * note) -ENOTTY means "Inappropriate ioctl for device.
     */
    if (_IOC_TYPE(cmd) != SCI_IOC_MAGIC)
    {
    	DBG_PRINT_WARNING("invalid magic. magic=0x%02X\n", _IOC_TYPE(cmd) );
    	return -ENOTTY;
    }
    if (_IOC_NR(cmd) > SCI_IOC_MAXNR)
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

	SCI_TRACE("cmd = %08X (cmd_idx=%d)\n", cmd, _IOC_NR(cmd) );

	switch(cmd)
	{
        case SCI_IO_RESET:
        {
			SCI_PRINT("ioctl: SCI_IO_RESET\n");

        	if(my_dev == NULL)
        	{
        		SCI_ERROR("ASSERT\n");
        		return SCI_FAIL;
        	}

        	SCI_Reset(&my_dev->info);

			my_dev->info.clock_supplied_khz = SCI_CTOP_Clock_Config();
			if(my_dev->info.clock_supplied_khz == 0)
			{
				DBG_PRINT_ERROR("SCI clock Init error(Unknown Chip Rev)\n");
				return SCI_FAIL;
			}
        }
        break;

		case SCI_IO_ACTIVATE:
		{
			SCI_PRINT("ioctl: SCI_IOR_ACTIVATE\n");

        	if(my_dev == NULL)
        	{
        		SCI_ERROR("ASSERT\n");
        		return SCI_FAIL;
        	}

        	ret = SCI_Activate(&my_dev->info);

        	if (ret != SCI_SUCCESS)
        	{
        		SCI_Deactivate(&my_dev->info);
                SCI_ERROR("ret : %d\n", ret);
        	}
		}
		break;

		case SCI_IO_DEACTIVATE:
		{
			SCI_PRINT("ioctl: SCI_IO_DEACTIVATE\n");

        	if(my_dev == NULL)
        	{
        		SCI_ERROR("ASSERT\n");
        		return SCI_FAIL;
        	}

        	SCI_Deactivate(&my_dev->info);

#if 0 // jinhwan.bae , static analysis, dead code , 20130917
        	if (ret != SCI_SUCCESS)
        	{
        		return ret;
        	}
#endif

		}
		break;

		case SCI_IOWR_TRANSMIT:
		{
        	LX_SCI_TRANSMIT transmitArg;

			SCI_PRINT("ioctl: SCI_IOWR_TRANSMIT\n");

        	if(my_dev == NULL)
        	{
        		SCI_ERROR("ASSERT\n");
        		return SCI_FAIL;
        	}

        	if(copy_from_user(&transmitArg, (void *)arg, sizeof(LX_SCI_TRANSMIT)))
        	{
        		SCI_ERROR("ERR COPY FROM USR\n");
        		return -EFAULT;
        	}

        	ret = SCI_Transmit(&my_dev->info, &transmitArg);

            if(ret == 0)    // transmit success, send rx data to user
            {
            	if(copy_to_user((void *)arg, &transmitArg, sizeof(LX_SCI_TRANSMIT)))
            	{
            		SCI_ERROR("ERR COPY TO USR\n");
            		return -EFAULT;
            	}
            }
            else
                SCI_ERROR("ret : %d\n", ret);
		}
		break;

		case SCI_IO_GET_ATR:
        {
			SCI_PRINT("ioctl: SCI_IOR_GET_ATR\n");

        	if(my_dev == NULL)
        	{
        		SCI_ERROR("ASSERT\n");
        		return SCI_FAIL;
        	}

        	ret = SCI_GetATR(&my_dev->info.atr);

        	if (ret != SCI_SUCCESS)
                SCI_ERROR("ret : %d\n", ret);
		}
		break;

		case SCI_IOR_GET_STATUS:
		{
        	LX_SCI_STATUS statusArg;

			SCI_PRINT("ioctl: SCI_IOR_GET_STATUS\n");

        	if(my_dev == NULL)
        	{
        		SCI_ERROR("ASSERT\n");
        		return SCI_FAIL;
        	}

        	statusArg = SCI_GetStatus();

        	if(copy_to_user((void *)arg, &statusArg, sizeof(LX_SCI_STATUS)))
        	{
        		SCI_ERROR("ERR COPY TO USR\n");
        		return -EFAULT;
        	}
		}
		break;

		case SCI_IOR_GET_VOLTAGE:
		{
        	LX_SCI_VOLTAGE voltageArg;

			SCI_PRINT("ioctl: SCI_IOR_GET_VOLTAGE\n");

        	if(my_dev == NULL)
        	{
        		SCI_ERROR("ASSERT\n");
        		return SCI_FAIL;
        	}

        	voltageArg = SCI_GetVoltage();

        	if(copy_to_user((void *)arg, &voltageArg, sizeof(LX_SCI_VOLTAGE)))
        	{
        		SCI_ERROR("ERR COPY TO USR\n");
        		return -EFAULT;
        	}
		}
		break;

		case SCI_IOW_SET_VOLTAGE:
		{
        	LX_SCI_VOLTAGE voltageArg;

			SCI_PRINT("ioctl: SCI_IOW_SET_VOLTAGE\n");

        	if(my_dev == NULL)
        	{
        		SCI_ERROR("ASSERT\n");
        		return SCI_FAIL;
        	}

        	if(copy_from_user(&voltageArg, (void *)arg, sizeof(LX_SCI_VOLTAGE)))
        	{
        		SCI_ERROR("ERR COPY FROM USR\n");
        		return -EFAULT;
        	}

        	ret = SCI_SetVoltage(voltageArg);
		}
		break;

		case SCI_IO_DUMP_REG:
        {
			SCI_PRINT("ioctl: SCI_IO_DUMP_REG\n");

            SCI_RegDump();

            ret = 0; // success
        }
        break;

        case SCI_IOW_WRITE_REG:
        {
        	LX_SCI_REG regArg;

			SCI_PRINT("ioctl: SCI_IOW_WRITE_REG\n");

        	if(my_dev == NULL)
        	{
        		SCI_ERROR("ASSERT\n");
        		return SCI_FAIL;
        	}

        	if(copy_from_user((void*)(&regArg), (void *)arg, sizeof(LX_SCI_REG)))
        	{
        		SCI_ERROR("ERR COPY FROM USR\n");
        		return -EFAULT;
        	}

        	SCI_RegWrite(regArg.regIndex, regArg.data);

            ret = 0; // success
        }
        break;

        case SCI_IOR_WAIT_CD:
        {
        	LX_SCI_STATUS statusArg;

			SCI_PRINT("ioctl: SCI_IOR_WAIT_CD\n");

        	if(my_dev == NULL)
        	{
        		SCI_ERROR("ASSERT\n");
        		return SCI_FAIL;
        	}

            SCI_ISRWaitCDIntr();

        	statusArg = SCI_GetStatus();

        	if(copy_to_user((void *)arg, &statusArg, sizeof(LX_SCI_STATUS)))
        	{
        		SCI_ERROR("ERR COPY TO USR\n");
        		return -EFAULT;
        	}

            ret = 0;
        }
        break;

	    default:
	    {
	    	SCI_PRINT("ioctl: default\n");
			/* redundant check but it seems more readable */
    	    ret = -ENOTTY;
		}
    }

    return ret;
}

static irqreturn_t SCI_IrqHandler(int irq, void *dev, struct pt_regs *regs)
{
    SCI_ISRHandler(irq, dev, regs);

	return IRQ_HANDLED;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef KDRV_GLOBAL_LINK
#if defined(CONFIG_LG_BUILTIN_KDRIVER) && defined(CONFIG_LGSNAP)
user_initcall_grp("kdrv", SCI_Init);
#else
module_init(SCI_Init);
#endif
module_exit(SCI_Cleanup);

MODULE_AUTHOR("LGE");
MODULE_DESCRIPTION("SCI driver");
MODULE_LICENSE("GPL");
#endif

/** @} */

