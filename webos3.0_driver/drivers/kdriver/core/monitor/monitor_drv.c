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
 *  main driver implementation for monitor device.
 *	monitor device will teach you how to make device driver with new platform.
 *
 *  author		raxis.lim@lge.com
 *  version		1.0
 *  date		2011.03.18
 *  note		Additional information.
 *
 *  @addtogroup lg1150_monitor
 *	@{
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/
#undef	SUPPORT_MONITOR_DEVICE_READ_WRITE_FOPS

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
#include "os_util.h"
#include "base_device.h"
#include "monitor_drv.h"

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
 *	main control block for monitor device.
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
MONITOR_DEVICE_T;

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/
extern	void	MONITOR_PROC_Init(void);
extern	void	MONITOR_PROC_Cleanup(void);

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Functions
----------------------------------------------------------------------------------------*/
int		MONITOR_Init(void);
void	MONITOR_Cleanup(void);

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/
int		g_monitor_debug_fd;
int 	g_monitor_major = MONITOR_MAJOR;
int 	g_monitor_minor = MONITOR_MINOR;

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/
static int      MONITOR_Open(struct inode *, struct file *);
static int      MONITOR_Close(struct inode *, struct file *);
#ifdef HAVE_UNLOCKED_IOCTL
static long             MONITOR_Ioctl ( struct file *, unsigned int, unsigned long );
#else
static int             MONITOR_Ioctl (struct inode *, struct file *, unsigned int, unsigned long );
#endif
#ifdef SUPPORT_MONITOR_DEVICE_READ_WRITE_FOPS
static ssize_t  MONITOR_Read(struct file *, char *, size_t, loff_t *);
static ssize_t  MONITOR_Write(struct file *, const char *, size_t, loff_t *);
#endif

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/

/**
 * main control block for monitor device
*/
static MONITOR_DEVICE_T*		g_monitor_device;

/**
 * file I/O description for monitor device
 *
*/
static struct file_operations g_monitor_fops =
{
	.open 	= MONITOR_Open,
	.release= MONITOR_Close,
#ifdef HAVE_UNLOCKED_IOCTL
	.unlocked_ioctl	= MONITOR_Ioctl,
#else
	.ioctl	= MONITOR_Ioctl,
#endif
#ifdef SUPPORT_MONITOR_DEVICE_READ_WRITE_FOPS
	.read 	= MONITOR_Read,
	.write 	= MONITOR_Write,
#else
	.read	= NULL,
	.write	= NULL,
#endif
};

/*========================================================================================
	Implementation Group
========================================================================================*/
/** Initialize the device environment before the real H/W initialization
 *
 *  @note main usage of this function is to initialize the HAL layer and memory size adjustment
 *  @note it's natural to keep this function blank :)
 */
void MONITOR_PreInit(void)
{
    /* TODO: do something */
}

int MONITOR_Init(void)
{
	int			i;
	int			err;
	dev_t		dev;

	/* Get the handle of debug output for monitor device.
	 *
	 * Most module should open debug handle before the real initialization of module.
	 * As you know, debug_util offers 4 independent debug outputs for your device driver.
	 * So if you want to use all the debug outputs, you should initialize each debug output
	 * using OS_DEBUG_EnableModuleByIndex() function.
	 */
	g_monitor_debug_fd = DBG_OPEN( MONITOR_MODULE );

    if ( g_monitor_debug_fd < 0 )
    {
        printk("[mon] can't get debug handle\n");
		g_monitor_debug_fd = g_global_debug_fd;
    }
    else
	{
		OS_DEBUG_EnableModule ( g_monitor_debug_fd );
		OS_DEBUG_EnableModuleByIndex ( g_monitor_debug_fd, LX_LOGM_LEVEL_ERROR, DBG_COLOR_NONE );
		OS_DEBUG_EnableModuleByIndex ( g_monitor_debug_fd, LX_LOGM_LEVEL_WARNING, DBG_COLOR_NONE );
	}

	/* allocate main device handler, register current device.
	 *
	 * If devie major is predefined then register device using that number.
	 * otherwise, major number of device is automatically assigned by Linux kernel.
	 *
	 */
	g_monitor_device = (MONITOR_DEVICE_T*)OS_KMalloc( sizeof(MONITOR_DEVICE_T)*MONITOR_MAX_DEVICE );

	if ( NULL == g_monitor_device )
	{
		DBG_PRINT_ERROR("out of memory. can't allocate %d bytes\n", sizeof(MONITOR_DEVICE_T)* MONITOR_MAX_DEVICE );
		return -ENOMEM;
	}

	memset( g_monitor_device, 0x0, sizeof(MONITOR_DEVICE_T)* MONITOR_MAX_DEVICE );

	if (g_monitor_major)
	{
		dev = MKDEV( g_monitor_major, g_monitor_minor );
		err = register_chrdev_region(dev, MONITOR_MAX_DEVICE, MONITOR_MODULE );
	}
	else
	{
		err = alloc_chrdev_region(&dev, g_monitor_minor, MONITOR_MAX_DEVICE, MONITOR_MODULE );
		g_monitor_major = MAJOR(dev);
	}

	if ( err < 0 )
	{
		DBG_PRINT_ERROR("can't register monitor device\n" );
		return -EIO;
	}

	/* TODO : initialize your module not specific minor device */


	/* END */

	for ( i=0; i<MONITOR_MAX_DEVICE; i++ )
	{
		/* initialize cdev structure with predefined variable */
		dev = MKDEV( g_monitor_major, g_monitor_minor+i );
		cdev_init( &(g_monitor_device[i].cdev), &g_monitor_fops );
		g_monitor_device[i].devno		= dev;
		g_monitor_device[i].cdev.owner = THIS_MODULE;
		g_monitor_device[i].cdev.ops   = &g_monitor_fops;

		/* TODO: initialize minor device */


		/* END */

		err = cdev_add (&(g_monitor_device[i].cdev), dev, 1 );

		if (err)
		{
			DBG_PRINT_ERROR("error (%d) while adding monitor device (%d.%d)\n", err, MAJOR(dev), MINOR(dev) );
			return -EIO;
		}
        OS_CreateDeviceClass ( g_monitor_device[i].devno, "%s%d", MONITOR_MODULE, i );
	}

	/* initialize proc system */
	MONITOR_PROC_Init ( );

	MONITOR_PRINT("monitor device initialized\n");

	return 0;
}

void MONITOR_Cleanup(void)
{
	int i;
	dev_t dev = MKDEV( g_monitor_major, g_monitor_minor );

	/* cleanup proc system */
	MONITOR_PROC_Cleanup( );

	/* remove all minor devicies and unregister current device */
	for ( i=0; i<MONITOR_MAX_DEVICE;i++)
	{
		/* TODO: cleanup each minor device */


		/* END */
		cdev_del( &(g_monitor_device[i].cdev) );
	}

	/* TODO : cleanup your module not specific minor device */

	unregister_chrdev_region(dev, MONITOR_MAX_DEVICE );

	OS_Free( g_monitor_device );
}


///////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * open handler for monitor device
 *
 */
static int
MONITOR_Open(struct inode *inode, struct file *filp)
{
    int					major,minor;
    struct cdev*    	cdev;
    MONITOR_DEVICE_T*	my_dev;

    cdev	= inode->i_cdev;
    my_dev	= container_of ( cdev, MONITOR_DEVICE_T, cdev);

    /* TODO : add your device specific code */


	/* END */

    my_dev->dev_open_count++;
    filp->private_data = my_dev;

	/* some debug */
    major = imajor(inode);
    minor = iminor(inode);
    MONITOR_PRINT("device opened (%d:%d)\n", major, minor );

    return 0;
}

/**
 * release handler for monitor device
 *
 */
static int
MONITOR_Close(struct inode *inode, struct file *file)
{
    int					major,minor;
    MONITOR_DEVICE_T*	my_dev;
    struct cdev*		cdev;

    cdev	= inode->i_cdev;
    my_dev	= container_of ( cdev, MONITOR_DEVICE_T, cdev);

    if ( my_dev->dev_open_count > 0 )
    {
        --my_dev->dev_open_count;
    }

    /* TODO : add your device specific code */
	if ( my_dev->dev_open_count <= 0 )
	{

	}

	/* END */

	/* some debug */
    major = imajor(inode);
    minor = iminor(inode);
    MONITOR_PRINT("device closed (%d:%d)\n", major, minor );
    return 0;
}

/**
 * ioctl handler for monitor device.
 *
 *
 * note: if you have some critial data, you should protect them using semaphore or spin lock.
 */
#ifdef HAVE_UNLOCKED_IOCTL
static long
MONITOR_Ioctl (struct file *filp, unsigned int cmd, unsigned long arg )
#else
static int
MONITOR_Ioctl ( struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg )
#endif
{
    int err = 0;
	int	ret = RET_ERROR;

    MONITOR_DEVICE_T*	my_dev;
    struct cdev*		cdev;

	/*
	 * get current monitor device object
	 */
#ifdef HAVE_UNLOCKED_IOCTL
	    my_dev  = (MONITOR_DEVICE_T *)filp->private_data;
	    cdev    = &my_dev->cdev;
#else
    cdev	= inode->i_cdev;
    my_dev	= container_of ( cdev, MONITOR_DEVICE_T, cdev);
#endif
    /*
     * check if IOCTL command is valid or not.
     * - if magic value doesn't match, return error (-ENOTTY)
     * - if command is out of range, return error (-ENOTTY)
     *
     * note) -ENOTTY means "Inappropriate ioctl for device.
     */
    if (_IOC_TYPE(cmd) != MONITOR_IOC_MAGIC)
    {
    	DBG_PRINT_WARNING("invalid magic. magic=0x%02X\n", _IOC_TYPE(cmd) );
    	return -ENOTTY;
    }
    if (_IOC_NR(cmd) > MONITOR_IOC_MAXNR)
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

	switch(cmd)
	{
		case MONITOR_IOW_WRITE_REG:
		{
			LX_REGISTER_T	reg;

			err = copy_from_user((void*)&reg, (void __user *)arg, sizeof(LX_REGISTER_T));
            MONITOR_CHECK_CODE( err > 0, break, "copy error\n");

			err = MONITOR_WriteReg( reg.addr, reg.value );
            MONITOR_CHECK_CODE( err < 0, break, "write_reg error\n");

			ret = RET_OK;
		}
		break;

		case MONITOR_IORW_READ_REG:
		{
			LX_REGISTER_T*	in_reg = (LX_REGISTER_T*)arg;
			LX_REGISTER_T	reg;

            MONITOR_CHECK_CODE( __get_user( reg.addr, &(in_reg->addr) ), break, "copy error\n");

			err = MONITOR_ReadReg( reg.addr, &reg.value );
            MONITOR_CHECK_CODE( err < 0, break, "read_reg error\n");

            MONITOR_CHECK_CODE( __put_user( reg.value, &(in_reg->value) ), break, "copy error\n");

			ret = RET_OK;
		}
		break;


		case DOUMI_I2C_INIT_CONFIG:
		{
			ret = Doumi_Init_I2C_Config();
		}
		break;

		case DOUMI_I2C_IOR_READ_REG:
		{
			LX_REGISTER_T read_param;
			if( copy_from_user((void *)&read_param , (void *)arg , sizeof(LX_REGISTER_T)) )
				return -EFAULT;

			ret = Doumi_I2C_Read(read_param.addr , &read_param.value);

			if( copy_to_user((void *)arg, (void *)&read_param, sizeof(LX_REGISTER_T)) )
				return -EFAULT;
		}
		break;
				
		case DOUMI_I2C_IOW_WRITE_REG:
		{
			LX_REGISTER_T write_param;

			if( copy_from_user((void *)&write_param , (void *)arg , sizeof(LX_REGISTER_T)) )
				return -EFAULT;

			ret = Doumi_I2C_Write(write_param.addr , write_param.value);

		}
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
user_initcall_grp("kdrv",MONITOR_Init);
#else
module_init(MONITOR_Init);
#endif
module_exit(MONITOR_Cleanup);

MODULE_AUTHOR("LGE");
MODULE_DESCRIPTION("MON driver");
MODULE_LICENSE("GPL");
#endif

/** @} */

