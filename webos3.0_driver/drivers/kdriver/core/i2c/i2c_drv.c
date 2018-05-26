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
 *  main driver implementation for i2c device.
 *	i2c device will teach you how to make device driver with new platform.
 *
 *  author		ingyu.yang (ingyu.yang@lge.com)
 *  				jun.kong (jun.kong@lge.com)
 *  version		1.0
 *  date		2009.12.30
 *  note		Additional information.
 *
 *  @addtogroup lg1150_i2c
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
#ifdef KDRV_CONFIG_PM	// added by SC Jung for quick bootling
#include <linux/platform_device.h>
bool g_iic_open_done = false;
#endif
#include "os_util.h"
#include "base_device.h"
#include "i2c_drv.h"
#include "base_types.h"
#include "i2c_core.h"
#include "i2c_gpio.h"
#include "sys_regs.h"
/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define I2C_COPY_FROM_USER(d,s,l,_action) 						\
			do {												\
				if (copy_from_user((void*)d, (void *)s, l)) {	\
					I2C_PRINT_ERROR("ioctl: copy_from_user\n");	\
					_action; 									\
				}												\
			} while(0)

#define I2C_COPY_TO_USER(d,s,l,_action)							\
			do {												\
				if (copy_to_user((void*)d, (void *)s, l)) { 	\
					I2C_PRINT_ERROR("ioctl: copy_to_user\n");	\
					_action; 									\
				}												\
			} while(0)


/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/
typedef struct I2C_DEVICE_FUNC
{
	LX_I2C_DEV_HANDLE 	(*open)(UINT8 ch);
	int 	(*close)(LX_I2C_DEV_HANDLE dev);
	int 	(*setclock)(LX_I2C_DEV_HANDLE dev, UINT32 clock);
	int 	(*transfer)(LX_I2C_DEV_HANDLE dev, LX_I2C_RW_DATA_T *data, UINT8 mode);

	int 	(*setport)(LX_I2C_DEV_HANDLE dev, LX_I2C_GPIO_MAP_T *map);
} I2C_DEVICE_FUNC_T;

/**
 *	main control block for i2c device.
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
	LX_I2C_DEV_HANDLE		handle;
	I2C_DEVICE_FUNC_T*		func;

// END of device specific data
}
I2C_DEVICE_T;

#ifdef KDRV_CONFIG_PM
typedef struct
{
	// add here extra parameter
	bool	is_suspended;
}I2C_DRVDATA_T;
#endif



/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/
extern	void	I2C_PROC_Init(void);
extern	void	I2C_PROC_Cleanup(void);

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Functions
----------------------------------------------------------------------------------------*/
int		I2C_Init(void);
void	I2C_Cleanup(void);

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/
int		g_i2c_debug_fd;
int 	g_i2c_major = I2C_MAJOR;
int 	g_i2c_minor = I2C_MINOR;

int		g_i2c_num_devices;

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/
static int      I2C_Open(struct inode *, struct file *);
static int      I2C_Close(struct inode *, struct file *);
static long		I2C_Ioctl(struct file *file, unsigned int cmd, unsigned long arg);
#ifdef SUPPORT_I2C_DEVICE_READ_WRITE_FOPS
static ssize_t  I2C_Read(struct file *, char *, size_t, loff_t *);
static ssize_t  I2C_Write(struct file *, const char *, size_t, loff_t *);
#endif

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
static int	i2c_drv_inited = 0;

static int	i2c_hw_num_devices;
static int	i2c_gpio_num_devices;

static I2C_DEVICE_FUNC_T i2c_funcs[] =
{
	[I2C_TYPE_HW] =
	{
		.open		= I2C_DevOpen,
		.close		= I2C_DevClose,
		.setclock	= I2C_DevSetClock,
		.transfer	= I2C_DevTransfer,
		.setport	= NULL,
	},
	[I2C_TYPE_GPIO] =
	{
		.open		= I2C_GPIO_Open,
		.close		= I2C_GPIO_Close,
		.setclock	= I2C_GPIO_SetClock,
		.transfer	= I2C_GPIO_Transfer,
		.setport	= I2C_GPIO_SetPort,
	}
};


/**
 * main control block for i2c device
*/
static I2C_DEVICE_T*		g_i2c_device;

/**
 * file I/O description for i2c device
 *
*/
static struct file_operations g_i2c_fops =
{
	.open 	= I2C_Open,
	.release= I2C_Close,
	.unlocked_ioctl = I2C_Ioctl,
#ifdef SUPPORT_I2C_DEVICE_READ_WRITE_FOPS
	.read 	= I2C_Read,
	.write 	= I2C_Write,
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
 * @return	int 0 : OK , -1 : NOT OK
 *
 */

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,16,0))
static int I2C_suspend(struct device *dev)
{
#ifdef KDRV_CONFIG_PM_I2C
	I2C_DRVDATA_T *drv_data;
	struct platform_device *pdev = to_platform_device(dev);

	printk("I2C - suspend BEGIN\n");

	if(g_iic_open_done == false)
	{
		printk("I2C - suspend ignore\n");
		return 0;
	}

	drv_data = platform_get_drvdata(pdev);

	// add here the suspend code
	I2C_DevSuspendAll();



	drv_data->is_suspended = 1;
	printk("I2C - suspend OK\n");
#endif

	return 0;
}
#else
static int I2C_suspend(struct platform_device *pdev, pm_message_t state)
{
#ifdef KDRV_CONFIG_PM_I2C
	I2C_DRVDATA_T *drv_data;

	printk("I2C - suspend BEGIN\n");

	if(g_iic_open_done == false)
	{
		printk("I2C - suspend ignore\n");
		return 0;
	}

	drv_data = platform_get_drvdata(pdev);

	// add here the suspend code
	I2C_DevSuspendAll();

	drv_data->is_suspended = 1;
	printk("I2C - suspend OK\n");
#endif

	return 0;
}
#endif





/**
 *
 * resuming module.
 *
 * @param	struct platform_device *
 * @return	int 0 : OK , -1 : NOT OK
 *
 */
 #if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,16,0))
 static int I2C_resume(struct device *dev)
{
#ifdef KDRV_CONFIG_PM_I2C
	I2C_DRVDATA_T *drv_data;

	struct platform_device *pdev = to_platform_device(dev);

	printk("I2C - resume BEGIN\n");


	/* I2C resume is ignore sometimes, so move here  */

	/* ACE(analog chip) Init code
	 * same code  exist in reg_ctrl.c for Normal Boot
	 * below code is for instant boot
	 * I2C use early resume , it is excuted prior to nomal dirver using I2C
	 */

	if(0)
	{}
	else if(lx_chip_rev() >= LX_CHIP_REV(H15, A0))
	{
		#ifdef INCLUDE_H15_CHIP_KDRV
		ACE_REG_H15A0_RdFL(h13a_version_0);
		printk("h15a VER : 0x%02x\n", ACE_REG_H15A0_Rd(h13a_version_0));

		//ACE 24MHz Clock Reset Release. (ACE HW Engineer recommend)
		ACE_REG_H15A0_RdFL(soft_reset_0);

		if(ACE_REG_H15A0_RdFd(soft_reset_0,swrst_f24m) == 1)
		{
			ACE_REG_H15A0_Wr01(soft_reset_0,swrst_f24m,0);
			ACE_REG_H15A0_WrFL(soft_reset_0);
		}

		/* Set Interrupt Sense to get the value */
		ACE_REG_H15A0_Wr(gpio_4, 0xFF);
		ACE_REG_H15A0_WrFL(gpio_4);
		ACE_REG_H15A0_Wr(gpio_5, 0xFF);
		ACE_REG_H15A0_WrFL(gpio_5);

		/*
		(3) GPIO_IEV 가 1일면 input level인 ‘1’ 인 경우 High, ‘0’이면 Low
		(4) GPIO_IEV 가 0일면 input level인 ‘0’ 인 경우 High, ‘1’이면 Low
		*/
		ACE_REG_H15A0_Wr(gpio_8, 0xFF);
		ACE_REG_H15A0_WrFL(gpio_8);
		ACE_REG_H15A0_Wr(gpio_9, 0xFF);
		ACE_REG_H15A0_WrFL(gpio_9);

		/* Workaround code for AAD clock latchup, to sync with normal boot ACE condition  */
		ACE_REG_H15A0_RdFL(main_pll_4);
		ACE_REG_H15A0_Wr01(main_pll_4, dr3p_pdb, 0);
		ACE_REG_H15A0_WrFL(main_pll_4);

		ACE_REG_H15A0_RdFL(main_pll_4);
		ACE_REG_H15A0_Wr01(main_pll_4, dr3p_pdb, 1);
		ACE_REG_H15A0_WrFL(main_pll_4);

		#endif
	}


	if(g_iic_open_done == false)
	{
		printk("I2C - resume ignore\n");
		return 0;
	}

	drv_data = platform_get_drvdata(pdev);

	if(drv_data->is_suspended == 0)
	{
		printk("I2C - resume FAIL\n");
		return -1;
	}

	// add here the resume code
	I2C_DevResumeAll();

	drv_data->is_suspended = 0;
	printk("I2C - resume OK\n");
#endif
	return 0;
}
 #else
static int I2C_resume(struct platform_device *pdev)
{
#ifdef KDRV_CONFIG_PM_I2C
	I2C_DRVDATA_T *drv_data;

	printk("I2C - resume BEGIN\n");

	if(g_iic_open_done == false)
	{
		printk("I2C - resume ignore\n");
		return 0;
	}

	drv_data = platform_get_drvdata(pdev);

	if(drv_data->is_suspended == 0)
	{
		printk("I2C - resume FAIL\n");
		return -1;
	}

	// add here the resume code
	I2C_DevResumeAll();

	drv_data->is_suspended = 0;
	printk("I2C - resume OK\n");
#endif
	return 0;
}
 #endif


 #if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,16,0))
#ifdef KDRV_CONFIG_PM_I2C
static struct dev_pm_ops pm_ops = {
       SET_LATE_SYSTEM_SLEEP_PM_OPS(I2C_suspend, I2C_resume)
};
#endif
#endif
/**
 *
 * probing module.
 *
 * @param	struct platform_device *pdev
 * @return	int 0 : OK , -1 : NOT OK
 *
 */
 int  I2C_probe(struct platform_device *pdev)
{

	I2C_DRVDATA_T *drv_data;

	drv_data = (I2C_DRVDATA_T *)kmalloc(sizeof(I2C_DRVDATA_T) , GFP_KERNEL);


	// add here driver registering code & allocating resource code



	I2C_PRINT("[%s] done probe\n", I2C_MODULE);
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
static int  I2C_remove(struct platform_device *pdev)
{
	I2C_DRVDATA_T *drv_data;

	// add here driver unregistering code & deallocating resource code



	drv_data = platform_get_drvdata(pdev);
	kfree(drv_data);

	I2C_PRINT("released\n");

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
static void  I2C_release(struct device *dev)
{
	I2C_PRINT("device released\n");
}

/*
 *	module platform driver structure
 */
/* I2C Driver should be resume before other Driver using I2C
 * it cause I2C error in i2c transfer while init I2C HW in I2C_resume */

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,16,0))
 static struct platform_driver i2c_driver =
{
	.probe          = I2C_probe,
	.remove         = I2C_remove,
								/* suspend move late suspend */
								/* resume  move ealry resume */
	.driver         =
	{
		.name   = I2C_MODULE,
		.pm = &pm_ops,
	},
};
#else
  static struct platform_driver i2c_driver =
{

	.probe          = I2C_probe,
	.remove         = I2C_remove,
	.suspend        = I2C_suspend,
	.resume         = I2C_resume,
	.driver         =
	{
		.name   = I2C_MODULE,

	},
};
#endif

static struct platform_device i2c_device = {
	.name = I2C_MODULE,
	.id = 0,
	.id = -1,
	.dev = {
		.release = I2C_release,
	},
};
#endif

/** Initialize the device environment before the real H/W initialization
 *
 *  @note main usage of this function is to initialize the HAL layer and memory size adjustment
 *  @note it's natural to keep this function blank :)
 */
void I2C_PreInit(void)
{
    /* TODO: do something */
}

int I2C_Init(void)
{
	int			i;
	int			err;
	dev_t		dev;

	if(i2c_drv_inited) return 0;

	/* Get the handle of debug output for i2c device.
	 *
	 * Most module should open debug handle before the real initialization of module.
	 * As you know, debug_util offers 4 independent debug outputs for your device driver.
	 * So if you want to use all the debug outputs, you should initialize each debug output
	 * using OS_DEBUG_EnableModuleByIndex() function.
	 */
	g_i2c_debug_fd = DBG_OPEN( I2C_MODULE );
	if (g_i2c_debug_fd < 0) return -1;

	OS_DEBUG_EnableModule ( g_i2c_debug_fd );

	//OS_DEBUG_EnableModuleByIndex ( g_i2c_debug_fd, 0, DBG_COLOR_NONE );
	OS_DEBUG_EnableModuleByIndex ( g_i2c_debug_fd, I2C_MSG_ERROR, DBG_COLOR_RED );
	OS_DEBUG_EnableModuleByIndex ( g_i2c_debug_fd, I2C_MSG_NORMAL, DBG_COLOR_YELLOW);


	/* allocate main device handler, register current device.
	 *
	 * If devie major is predefined then register device using that number.
	 * otherwise, major number of device is automatically assigned by Linux kernel.
	 *
	 */
#ifdef KDRV_CONFIG_PM
	// added by SC Jung for quick booting
	if(platform_driver_register(&i2c_driver) < 0)
	{
		I2C_PRINT("[%s] platform driver register failed\n",I2C_MODULE);

	}
	else
	{
		if(platform_device_register(&i2c_device))
		{
			platform_driver_unregister(&i2c_driver);
			I2C_PRINT("[%s] platform device register failed\n",I2C_MODULE);
		}
		else
		{
			I2C_PRINT("[%s] platform register done\n", I2C_MODULE);
		}


	}
#endif

	/* TODO : initialize your module not specific minor device */
	I2C_DevInit();
	i2c_hw_num_devices = I2C_DevGetDeviceNums();

	I2C_GPIO_Init();
	i2c_gpio_num_devices = I2C_GPIO_GetDeviceNums();

	g_i2c_num_devices = i2c_hw_num_devices + i2c_gpio_num_devices;

	g_i2c_device = (I2C_DEVICE_T*)OS_KMalloc( sizeof(I2C_DEVICE_T)*g_i2c_num_devices );
	if ( NULL == g_i2c_device )
	{
		DBG_PRINT_ERROR("out of memory. can't allocate %d bytes\n", sizeof(I2C_DEVICE_T)* g_i2c_num_devices );
		return -ENOMEM;
	}

	memset( g_i2c_device, 0x0, sizeof(I2C_DEVICE_T)* g_i2c_num_devices );

	if (g_i2c_major)
	{
		dev = MKDEV( g_i2c_major, g_i2c_minor );
		err = register_chrdev_region(dev, g_i2c_num_devices, I2C_MODULE );
	}
	else
	{
		err = alloc_chrdev_region(&dev, g_i2c_minor, g_i2c_num_devices, I2C_MODULE );
		g_i2c_major = MAJOR(dev);
	}

	if ( err < 0 )
	{
		DBG_PRINT_ERROR("can't register i2c device\n" );
		return -EIO;
	}

	/* END */

	for ( i=0; i<g_i2c_num_devices; i++ )
	{
		/* initialize cdev structure with predefined variable */

		// TODO: I2C-GPIO
		dev = MKDEV( g_i2c_major, g_i2c_minor+i );
		cdev_init( &(g_i2c_device[i].cdev), &g_i2c_fops );
		g_i2c_device[i].devno		= dev;
		g_i2c_device[i].cdev.owner = THIS_MODULE;
		g_i2c_device[i].cdev.ops   = &g_i2c_fops;

		/* TODO: initialize minor device */


		/* END */

		err = cdev_add (&(g_i2c_device[i].cdev), dev, 1 );

		if (err)
		{
			DBG_PRINT_ERROR("error (%d) while adding i2c device (%d.%d)\n", err, MAJOR(dev), MINOR(dev) );
			return -EIO;
		}
        OS_CreateDeviceClass ( g_i2c_device[i].devno, "%s%d", I2C_MODULE, i );
	}

	/* initialize proc system */
	I2C_PROC_Init ( );

	I2C_PRINT("i2c device initialized\n");

	i2c_drv_inited = 1;

	return 0;
}

void I2C_Cleanup(void)
{
	int i;
	dev_t dev = MKDEV( g_i2c_major, g_i2c_minor );

#ifdef KDRV_CONFIG_PM
	// added by SC Jung for quick booting
	platform_driver_unregister(&i2c_driver);
	platform_device_unregister(&i2c_device);
#endif

	/* cleanup proc system */
	I2C_PROC_Cleanup( );

	/* remove all minor devicies and unregister current device */
	for ( i=0; i<g_i2c_num_devices;i++)
	{
		/* TODO: cleanup each minor device */


		/* END */
		cdev_del( &(g_i2c_device[i].cdev) );
	}

	/* TODO : cleanup your module not specific minor device */

	unregister_chrdev_region(dev, g_i2c_num_devices );

	I2C_DEBUG("i2c device cleanup\n");

	OS_Free( g_i2c_device );
	g_i2c_num_devices = 0;

	i2c_drv_inited = 0;
}


///////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * open handler for i2c device
 *
 */
static int
I2C_Open(struct inode *inode, struct file *filp)
{
    int					major,minor;
    struct cdev*    	cdev;
    I2C_DEVICE_T*		i2c_dev;

    cdev	= inode->i_cdev;
    i2c_dev	= container_of ( cdev, I2C_DEVICE_T, cdev);

    major = imajor(inode);
    minor = iminor(inode);

	if ( minor >= g_i2c_num_devices )
	{
		I2C_PRINT_ERROR( "Invalid device num[%d] for Open!!\n", minor );
		return -1;
	}

	if(i2c_dev->dev_open_count == 0)
	{
		UINT8 ch;
		if(minor < i2c_hw_num_devices)
		{
			i2c_dev->func = &i2c_funcs[I2C_TYPE_HW];
			ch = minor;
		}
		else
		{
			i2c_dev->func = &i2c_funcs[I2C_TYPE_GPIO];
			ch = (UINT8)(minor - i2c_hw_num_devices);
		}
		i2c_dev->handle = i2c_dev->func->open(ch);

		if(i2c_dev->handle == NULL)
		{
			I2C_PRINT_ERROR( "I2C device error...\n");
			return -EIO;
		}
	}

    i2c_dev->dev_open_count++;
    filp->private_data = i2c_dev;

#ifdef KDRV_CONFIG_PM
 g_iic_open_done = true;
#endif
    I2C_NOTI("device opened (%d:%d)\n", major, minor );

    return 0;
}

/**
 * release handler for i2c device
 *
 */
static int
I2C_Close(struct inode *inode, struct file *file)
{
    int					major,minor;
    I2C_DEVICE_T*		i2c_dev;
    struct cdev*		cdev;

    cdev	= inode->i_cdev;
    i2c_dev	= container_of ( cdev, I2C_DEVICE_T, cdev);

    if ( i2c_dev->dev_open_count > 0 )
    {
        --i2c_dev->dev_open_count;
		if(i2c_dev->dev_open_count == 0)
		{
			/* UnInstall the device */
			//i2c_dev->func->close(i2c_dev->handle);
		}
    }
    major = imajor(inode);
    minor = iminor(inode);

	/* END */

	/* some debug */
    I2C_NOTI("device closed (%d:%d)\n", major, minor );

    return 0;
}

/**
 * ioctl handler for i2c device.
 *
 *
 * note: if you have some critial data, you should protect them using semaphore or spin lock.
 */
static long I2C_Ioctl (struct file *file, unsigned int cmd, unsigned long arg)
{
	I2C_DEVICE_T*	i2c_dev = (I2C_DEVICE_T*)file->private_data;

	switch(cmd)
	{
		case I2C_IOW_SET_CLOCK:
		{
			LX_I2C_CLOCK_T clock;
			I2C_COPY_FROM_USER(&clock, arg, sizeof(LX_I2C_CLOCK_T), return -EFAULT);
			return i2c_dev->func->setclock(i2c_dev->handle, clock);
		}

		case I2C_IOW_WRITE:
		case I2C_IORW_READ:
		{
			int res = -EFAULT;
			LX_I2C_RW_DATA_T	rw_data;
			UINT8	sbuf[32];
			UINT8*	mbuf = NULL;
			UINT8*	kbuf;
			UINT8*	ubuf;

			I2C_COPY_FROM_USER(&rw_data, arg, sizeof(LX_I2C_RW_DATA_T), return -EFAULT);

			/* To prevent the defect(TAINTED_SCALAR) */
			if(rw_data.bufSize > (32*1024))
				return -EFAULT;

			ubuf = rw_data.buf;

			if(rw_data.bufSize > 32)
			{
				mbuf = (UINT8*)OS_Malloc(rw_data.bufSize);
				if(mbuf == NULL) return -ENOMEM;
				kbuf = mbuf;
			}
			else
			{
				kbuf = sbuf;
			}

			if(cmd == I2C_IOW_WRITE)
			{
				I2C_COPY_FROM_USER(kbuf, (void __user *)ubuf, rw_data.bufSize, goto rw_err);
			}

			rw_data.buf = kbuf;
			res = i2c_dev->func->transfer(i2c_dev->handle, &rw_data,
								(cmd == I2C_IOW_WRITE) ? I2C_WRITE_MODE : I2C_READ_MODE);

			if(cmd == I2C_IORW_READ && res >= 0)
			{
				I2C_COPY_TO_USER((void __user *)ubuf, (void*)kbuf, rw_data.bufSize, goto rw_err);
			}
rw_err:
			if(mbuf != NULL) OS_Free(mbuf);

			return res;
		}

		case I2C_IOW_SET_GPIO_PORT:
		{
			LX_I2C_GPIO_MAP_T map;
			I2C_COPY_FROM_USER(&map, arg, sizeof(LX_I2C_GPIO_MAP_T), return -EFAULT);
			if(i2c_dev->func->setport)
				return i2c_dev->func->setport(i2c_dev->handle, &map);
			return 0;
		}

		default:
			I2C_DEBUG("ioctl: default\n");
			/* redundant check but it seems more readable */
			return -ENOTTY;
	}

	return 0;
}

/* R/W functions are used with ioctl */
#if 0
static ssize_t I2C_Read(struct file *filp, char *buf, size_t count, loff_t *f_pos)
{
	I2C_DEVICE_T*	i2c_dev = (I2C_DEVICE_T*)filp->private_data;
	LX_I2C_RW_DATA_T  rw_data;

	if( copy_from_user((void*)&rw_data, (void __user *)buf, sizeof(LX_I2C_RW_DATA_T)) )
	{
		return -1;
	}

	return I2C_DevTransfer(i2c_dev->handle, &rw_data, I2C_READ_MODE, 0);
}

static ssize_t I2C_Write(struct file *filp, const char *buf, size_t count, loff_t *f_pos)
{
	I2C_DEVICE_T*	i2c_dev = (I2C_DEVICE_T*)filp->private_data;
	LX_I2C_RW_DATA_T  rw_data;

	if( copy_from_user((void*)&rw_data, (void __user *)buf, sizeof(LX_I2C_RW_DATA_T)) )
	{
		return -1;
	}

	/* Write to the I2C device */
	return I2C_DevTransfer(i2c_dev->handle, &rw_data, I2C_WRITE_MODE, 0);
}
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef KDRV_GLOBAL_LINK
#if defined(CONFIG_LG_BUILTIN_KDRIVER) && defined(CONFIG_LGSNAP)
user_initcall_grp("kdrv",I2C_Init);
#else
module_init(I2C_Init);
#endif
module_exit(I2C_Cleanup);

MODULE_AUTHOR("LGE");
MODULE_DESCRIPTION("i2c driver");
MODULE_LICENSE("GPL");


#endif

/** @} */

