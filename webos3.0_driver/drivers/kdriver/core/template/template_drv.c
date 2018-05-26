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
 *  main driver implementation for template device.
 *	template device will teach you how to make device driver with new platform.
 *
 *  author		your name ( your e-mail)
 *  author		your name ( your e-mail) - modifier
 *  version		1.0
 *  date		2009.11.04
 *  note		Additional information.
 *
 *  @addtogroup lg1150_template
 *	@{
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/
#undef	SUPPORT_TEMPLATE_DEVICE_READ_WRITE_FOPS

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
#include "template_drv.h"

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
 *	main control block for template device.
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
TEMPLATE_DEVICE_T;

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/
extern	void	TEMPLATE_PROC_Init(void);
extern	void	TEMPLATE_PROC_Cleanup(void);

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Functions
----------------------------------------------------------------------------------------*/
int		TEMPLATE_Init(void);
void	TEMPLATE_Cleanup(void);

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/
int		g_template_debug_fd;
int 	g_template_major = TEMPLATE_MAJOR;
int 	g_template_minor = TEMPLATE_MINOR;

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/
static int      TEMPLATE_Open(struct inode *, struct file *);
static int      TEMPLATE_Close(struct inode *, struct file *);
static int 		TEMPLATE_Ioctl (struct inode *, struct file *, unsigned int, unsigned long );
#ifdef SUPPORT_TEMPLATE_DEVICE_READ_WRITE_FOPS
static ssize_t  TEMPLATE_Read(struct file *, char *, size_t, loff_t *);
static ssize_t  TEMPLATE_Write(struct file *, const char *, size_t, loff_t *);
#endif

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/

/**
 * main control block for template device
*/
static TEMPLATE_DEVICE_T*		g_template_device;

/**
 * file I/O description for template device
 *
*/
static struct file_operations g_template_fops =
{
	.open 	= TEMPLATE_Open,
	.release= TEMPLATE_Close,
	.ioctl	= TEMPLATE_Ioctl,
#ifdef SUPPORT_TEMPLATE_DEVICE_READ_WRITE_FOPS
	.read 	= TEMPLATE_Read,
	.write 	= TEMPLATE_Write,
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
void TEMPLATE_PreInit(void)
{
    /* TODO: do something */
}

int TEMPLATE_Init(void)
{
	int			i;
	int			err;
	dev_t		dev;

	/* Get the handle of debug output for template device.
	 *
	 * Most module should open debug handle before the real initialization of module.
	 * As you know, debug_util offers 4 independent debug outputs for your device driver.
	 * So if you want to use all the debug outputs, you should initialize each debug output
	 * using OS_DEBUG_EnableModuleByIndex() function.
	 */
	g_template_debug_fd = DBG_OPEN( TEMPLATE_MODULE );
	OS_DEBUG_EnableModule ( g_template_debug_fd );

	OS_DEBUG_EnableModuleByIndex ( g_template_debug_fd, 0, DBG_COLOR_NONE );
	OS_DEBUG_EnableModuleByIndex ( g_template_debug_fd, 1, DBG_COLOR_NONE );
	OS_DEBUG_EnableModuleByIndex ( g_template_debug_fd, 2, DBG_COLOR_NONE );
	OS_DEBUG_EnableModuleByIndex ( g_template_debug_fd, 3, DBG_COLOR_NONE );

	/* allocate main device handler, register current device.
	 *
	 * If devie major is predefined then register device using that number.
	 * otherwise, major number of device is automatically assigned by Linux kernel.
	 *
	 */
	g_template_device = (TEMPLATE_DEVICE_T*)OS_KMalloc( sizeof(TEMPLATE_DEVICE_T)*TEMPLATE_MAX_DEVICE );

	if ( NULL == g_template_device )
	{
		DBG_PRINT_ERROR("out of memory. can't allocate %d bytes\n", sizeof(TEMPLATE_DEVICE_T)* TEMPLATE_MAX_DEVICE );
		return -ENOMEM;
	}

	memset( g_template_device, 0x0, sizeof(TEMPLATE_DEVICE_T)* TEMPLATE_MAX_DEVICE );

	if (g_template_major)
	{
		dev = MKDEV( g_template_major, g_template_minor );
		err = register_chrdev_region(dev, TEMPLATE_MAX_DEVICE, TEMPLATE_MODULE );
	}
	else
	{
		err = alloc_chrdev_region(&dev, g_template_minor, TEMPLATE_MAX_DEVICE, TEMPLATE_MODULE );
		g_template_major = MAJOR(dev);
	}

	if ( err < 0 )
	{
		DBG_PRINT_ERROR("can't register template device\n" );
		return -EIO;
	}

	/* TODO : initialize your module not specific minor device */


	/* END */

	for ( i=0; i<TEMPLATE_MAX_DEVICE; i++ )
	{
		/* initialize cdev structure with predefined variable */
		dev = MKDEV( g_template_major, g_template_minor+i );
		cdev_init( &(g_template_device[i].cdev), &g_template_fops );
		g_template_device[i].devno		= dev;
		g_template_device[i].cdev.owner = THIS_MODULE;
		g_template_device[i].cdev.ops   = &g_template_fops;

		/* TODO: initialize minor device */


		/* END */

		err = cdev_add (&(g_template_device[i].cdev), dev, 1 );

		if (err)
		{
			DBG_PRINT_ERROR("error (%d) while adding template device (%d.%d)\n", err, MAJOR(dev), MINOR(dev) );
			return -EIO;
		}
        OS_CreateDeviceClass ( g_template_device[i].devno, "%s%d", TEMPLATE_MODULE, i );
	}

	/* initialize proc system */
	TEMPLATE_PROC_Init ( );

	TEMPLATE_PRINT("template device initialized\n");

	return 0;
}

void TEMPLATE_Cleanup(void)
{
	int i;
	dev_t dev = MKDEV( g_template_major, g_template_minor );

	/* cleanup proc system */
	TEMPLATE_PROC_Cleanup( );

	/* remove all minor devicies and unregister current device */
	for ( i=0; i<TEMPLATE_MAX_DEVICE;i++)
	{
		/* TODO: cleanup each minor device */


		/* END */
		cdev_del( &(g_template_device[i].cdev) );
	}

	/* TODO : cleanup your module not specific minor device */

	unregister_chrdev_region(dev, TEMPLATE_MAX_DEVICE );

	OS_Free( g_template_device );
}


///////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * open handler for template device
 *
 */
static int
TEMPLATE_Open(struct inode *inode, struct file *filp)
{
    int					major,minor;
    struct cdev*    	cdev;
    TEMPLATE_DEVICE_T*	my_dev;

    cdev	= inode->i_cdev;
    my_dev	= container_of ( cdev, TEMPLATE_DEVICE_T, cdev);

    /* TODO : add your device specific code */


	/* END */

    my_dev->dev_open_count++;
    filp->private_data = my_dev;

	/* some debug */
    major = imajor(inode);
    minor = iminor(inode);
    TEMPLATE_PRINT("device opened (%d:%d)\n", major, minor );

    return 0;
}

/**
 * release handler for template device
 *
 */
static int
TEMPLATE_Close(struct inode *inode, struct file *file)
{
    int					major,minor;
    TEMPLATE_DEVICE_T*	my_dev;
    struct cdev*		cdev;

    cdev	= inode->i_cdev;
    my_dev	= container_of ( cdev, TEMPLATE_DEVICE_T, cdev);

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
    TEMPLATE_PRINT("device closed (%d:%d)\n", major, minor );
    return 0;
}

/**
 * ioctl handler for template device.
 *
 *
 * note: if you have some critial data, you should protect them using semaphore or spin lock.
 */
static int
TEMPLATE_Ioctl ( struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg )
{
    int err = 0, ret = 0;

    TEMPLATE_DEVICE_T*	my_dev;
    struct cdev*		cdev;

	/*
	 * get current template device object
	 */
    cdev	= inode->i_cdev;
    my_dev	= container_of ( cdev, TEMPLATE_DEVICE_T, cdev);

    /*
     * check if IOCTL command is valid or not.
     * - if magic value doesn't match, return error (-ENOTTY)
     * - if command is out of range, return error (-ENOTTY)
     *
     * note) -ENOTTY means "Inappropriate ioctl for device.
     */
    if (_IOC_TYPE(cmd) != TEMPLATE_IOC_MAGIC)
    {
    	DBG_PRINT_WARNING("invalid magic. magic=0x%02X\n", _IOC_TYPE(cmd) );
    	return -ENOTTY;
    }
    if (_IOC_NR(cmd) > TEMPLATE_IOC_MAXNR)
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

	TEMPLATE_TRACE("cmd = %08X (cmd_idx=%d)\n", cmd, _IOC_NR(cmd) );

	switch(cmd)
	{
		case TEMPLATE_IO_RESET:
		{
			TEMPLATE_PRINT("reset ok\n");
			ret = 0;
		}
		break;

		case TEMPLATE_IOR_CHIP_REV_INFO:
		{
			CHIP_REV_INFO_T	rev_info;

			rev_info.version = 0x100;
			rev_info.date[0] =  9;	/* 2009/11/24 */
			rev_info.date[0] = 11;
			rev_info.date[0] = 24;

			TEMPLATE_PRINT("rev_info (%0X, %d:%d:%d)\n", rev_info.version,
														rev_info.date[0], rev_info.date[1], rev_info.date[2] );

	        if ( copy_to_user((void __user *)arg, &rev_info, sizeof(CHIP_REV_INFO_T)) )
    	        return -EFAULT;

			ret = 0;
		}
		break;

		case TEMPLATE_IOW_WRITE_UINT32:
		{
			UINT32	data;

			ret = __get_user( data, (UINT32 __user *)arg );

			TEMPLATE_PRINT("data = 0x%X\n", data );

			ret = 0;
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
user_initcall_grp("kdrv",TEMPLATE_Init);
#else
module_init(TEMPLATE_Init);
#endif
module_exit(TEMPLATE_Cleanup);

MODULE_AUTHOR("LGE");
MODULE_DESCRIPTION("base driver");
MODULE_LICENSE("GPL");
#endif

/** @} */

