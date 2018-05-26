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
 *  main driver implementation for sys device.
 *	sys device will teach you how to make device driver with new platform.
 *
 *  author		ks.hyun (ks.hyun@lge.com)
 *  version		1.0
 *  date		2010.12.13
 *  note		Additional information.
 *
 *  @addtogroup lg1150_sys
 *	@{
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/
#undef	SUPPORT_SYS_DEVICE_READ_WRITE_FOPS

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
#include "sys_drv.h"
#include "reg_ctrl.h"
#include "sys_io.h"
#include "sys_diag.h"


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
 *	main control block for sys device.
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
SYS_DEVICE_T;

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/
extern	void	SYS_PROC_Init(void);
extern	void	SYS_PROC_Cleanup(void);

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Functions
----------------------------------------------------------------------------------------*/
int		SYS_Init(void);
void	SYS_Cleanup(void);

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/
int		g_sys_debug_fd;
int 	g_sys_major = SYS_MAJOR;
int 	g_sys_minor = SYS_MINOR;

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/
static int      SYS_Open(struct inode *, struct file *);
static int      SYS_Close(struct inode *, struct file *);
#ifdef HAVE_UNLOCKED_IOCTL
static long             SYS_Ioctl (struct file *, unsigned int, unsigned long );
#else
static int             SYS_Ioctl (struct inode *, struct file *, unsigned int, unsigned long );
#endif
#ifdef SUPPORT_SYS_DEVICE_READ_WRITE_FOPS
static ssize_t  SYS_Read(struct file *, char *, size_t, loff_t *);
static ssize_t  SYS_Write(struct file *, const char *, size_t, loff_t *);
#endif

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/

/**
 * main control block for sys device
*/
static SYS_DEVICE_T*		g_sys_device;

/**
 * file I/O description for sys device
 *
*/
static struct file_operations g_sys_fops =
{
	.open 	= SYS_Open,
	.release= SYS_Close,
#ifdef HAVE_UNLOCKED_IOCTL
	.unlocked_ioctl	= SYS_Ioctl,
#else
	.ioctl	= SYS_Ioctl,
#endif
#ifdef SUPPORT_SYS_DEVICE_READ_WRITE_FOPS
	.read 	= SYS_Read,
	.write 	= SYS_Write,
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
void SYS_PreInit(void)
{
    diag_init();
}

int SYS_Init(void)
{
	int			i;
	int			err;
	dev_t		dev;

	/* Get the handle of debug output for sys device.
	 *
	 * Most module should open debug handle before the real initialization of module.
	 * As you know, debug_util offers 4 independent debug outputs for your device driver.
	 * So if you want to use all the debug outputs, you should initialize each debug output
	 * using OS_DEBUG_EnableModuleByIndex() function.
	 */
	g_sys_debug_fd = DBG_OPEN( SYS_MODULE );
	if(g_sys_debug_fd < 0)
	{
		DBG_PRINT_ERROR("can't open debug module\n");
		return -EIO;
	}

	OS_DEBUG_EnableModule ( g_sys_debug_fd );

	OS_DEBUG_EnableModuleByIndex ( g_sys_debug_fd, SYS_LOG_LEVEL_ERROR, DBG_COLOR_NONE );
	OS_DEBUG_EnableModuleByIndex ( g_sys_debug_fd, SYS_LOG_LEVEL_WARN, DBG_COLOR_NONE );
	OS_DEBUG_EnableModuleByIndex ( g_sys_debug_fd, SYS_LOG_LEVEL_DEBUG, DBG_COLOR_NONE );


	/* allocate main device handler, register current device.
	 *
	 * If devie major is predefined then register device using that number.
	 * otherwise, major number of device is automatically assigned by Linux kernel.
	 *
	 */
	g_sys_device = (SYS_DEVICE_T*)OS_KMalloc( sizeof(SYS_DEVICE_T)*SYS_MAX_DEVICE );

	if ( NULL == g_sys_device )
	{
		DBG_PRINT_ERROR("out of memory. can't allocate %d bytes\n", sizeof(SYS_DEVICE_T)* SYS_MAX_DEVICE );
		return -ENOMEM;
	}

	memset( g_sys_device, 0x0, sizeof(SYS_DEVICE_T)* SYS_MAX_DEVICE );

	if (g_sys_major)
	{
		dev = MKDEV( g_sys_major, g_sys_minor );
		err = register_chrdev_region(dev, SYS_MAX_DEVICE, SYS_MODULE );
	}
	else
	{
		err = alloc_chrdev_region(&dev, g_sys_minor, SYS_MAX_DEVICE, SYS_MODULE );
		g_sys_major = MAJOR(dev);
	}

	if ( err < 0 )
	{
		DBG_PRINT_ERROR("can't register sys device\n" );
		return -EIO;
	}

	/* TODO : initialize your module not specific minor device */
	REG_CTRL_Init();

	/* END */

	for ( i=0; i<SYS_MAX_DEVICE; i++ )
	{
		/* initialize cdev structure with predefined variable */
		dev = MKDEV( g_sys_major, g_sys_minor+i );
		cdev_init( &(g_sys_device[i].cdev), &g_sys_fops );
		g_sys_device[i].devno		= dev;
		g_sys_device[i].cdev.owner = THIS_MODULE;
		g_sys_device[i].cdev.ops   = &g_sys_fops;

		/* TODO: initialize minor device */


		/* END */

		err = cdev_add (&(g_sys_device[i].cdev), dev, 1 );

		if (err)
		{
			DBG_PRINT_ERROR("error (%d) while adding sys device (%d.%d)\n", err, MAJOR(dev), MINOR(dev) );
			return -EIO;
		}
        OS_CreateDeviceClass ( g_sys_device[i].devno, "%s%d", SYS_MODULE, i );
	}

	/* initialize proc system */
	SYS_PROC_Init ( );

	SYS_PRINT("sys device initialized\n");

	return 0;
}

void SYS_Cleanup(void)
{
	int i;
	dev_t dev = MKDEV( g_sys_major, g_sys_minor );

	/* cleanup proc system */
	SYS_PROC_Cleanup( );

	REG_CTRL_Free();

	/* remove all minor devicies and unregister current device */
	for ( i=0; i<SYS_MAX_DEVICE;i++)
	{
		/* TODO: cleanup each minor device */


		/* END */
		cdev_del( &(g_sys_device[i].cdev) );
	}

	/* TODO : cleanup your module not specific minor device */

	unregister_chrdev_region(dev, SYS_MAX_DEVICE );

	OS_Free( g_sys_device );
}


///////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * open handler for sys device
 *
 */
static int
SYS_Open(struct inode *inode, struct file *filp)
{
    int					major,minor;
    struct cdev*    	cdev;
    SYS_DEVICE_T*	my_dev;

    cdev	= inode->i_cdev;
    my_dev	= container_of ( cdev, SYS_DEVICE_T, cdev);

    /* TODO : add your device specific code */


	/* END */

    my_dev->dev_open_count++;
    filp->private_data = my_dev;

	/* some debug */
    major = imajor(inode);
    minor = iminor(inode);
    SYS_PRINT("device opened (%d:%d)\n", major, minor );

    return 0;
}

/**
 * release handler for sys device
 *
 */
static int
SYS_Close(struct inode *inode, struct file *file)
{
    int					major,minor;
    SYS_DEVICE_T*	my_dev;
    struct cdev*		cdev;

    cdev	= inode->i_cdev;
    my_dev	= container_of ( cdev, SYS_DEVICE_T, cdev);

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
    SYS_PRINT("device closed (%d:%d)\n", major, minor );
    return 0;
}

/**
 * ioctl handler for sys device.
 *
 *
 * note: if you have some critial data, you should protect them using semaphore or spin lock.
 */
#ifdef HAVE_UNLOCKED_IOCTL
static long
SYS_Ioctl ( struct file *filp, unsigned int cmd, unsigned long arg )
#else
static int
SYS_Ioctl ( struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg )
#endif
{
    int err = 0, ret = 0;

    SYS_DEVICE_T*	my_dev;
    struct cdev*		cdev;

    /*
     * get current sys device object
     */
#ifdef HAVE_UNLOCKED_IOCTL
	my_dev	= (SYS_DEVICE_T *)filp->private_data;
	cdev	= &my_dev->cdev;
#else
    cdev	= inode->i_cdev;
    my_dev	= container_of ( cdev, SYS_DEVICE_T, cdev);
#endif
    /*
     * check if IOCTL command is valid or not.
     * - if magic value doesn't match, return error (-ENOTTY)
     * - if command is out of range, return error (-ENOTTY)
     *
     * note) -ENOTTY means "Inappropriate ioctl for device.
     */
    if (_IOC_TYPE(cmd) != SYS_IOC_MAGIC)
    {
    	DBG_PRINT_WARNING("invalid magic. magic=0x%02X\n", _IOC_TYPE(cmd) );
    	return -ENOTTY;
    }
    if (_IOC_NR(cmd) > SYS_IOC_MAXNR)
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

//	SYS_TRACE("cmd = %08X (cmd_idx=%d)\n", cmd, _IOC_NR(cmd) );

	switch(cmd)
	{
		case SYS_IO_RESET:
		{
			SYS_PRINT("reset ok\n");
			ret = 0;
		}
		break;

		case SYS_IOR_CHIP_REV_INFO:
		{
			CHIP_REV_INFO_T	rev_info;

			rev_info.version = 0x100;
			rev_info.date[0] =  9;	/* 2009/11/24 */
			rev_info.date[0] = 11;
			rev_info.date[0] = 24;

			SYS_PRINT("rev_info (%0X, %d:%d:%d)\n", rev_info.version,
														rev_info.date[0], rev_info.date[1], rev_info.date[2] );

	        if ( copy_to_user((void __user *)arg, &rev_info, sizeof(CHIP_REV_INFO_T)) )
    	        return -EFAULT;

			ret = 0;
		}
		break;

		case SYS_IOW_WRITE_MEM:
		{
			LX_SYS_MEM_T mem;

			if(__copy_from_user((void*)&mem, (void __user *)arg, sizeof(LX_SYS_MEM_T)) != 0)
				return -EFAULT;

			ret = SYS_IO_WriteRegArray(mem.addr, 4, &mem.value);
		}
		break;

		case SYS_IORW_READ_MEM:
		{
			UINT32 addr, value;
			LX_SYS_MEM_T *in_mem = (LX_SYS_MEM_T*)arg;

			if(__get_user(addr, &in_mem->addr) != 0)
				return -EFAULT;

			ret = SYS_IO_ReadRegArray(addr, 4, &value);

			if(__put_user(value, &in_mem->value) != 0)
				return -EFAULT;
		}
		break;

		case SYS_IOW_WRITE_MEM_ARRAY:
		{
			LX_SYS_MEM_ARRAY_T mem;
			UINT8 *data;

			if(__copy_from_user((void*)&mem, (void __user *)arg, sizeof(LX_SYS_MEM_ARRAY_T)) != 0)
				return -EFAULT;

			if(mem.size == 0 || mem.data == NULL)
				return -EFAULT;

			if((data = (UINT8*)OS_Malloc(mem.size)) == NULL)
				return -ENOMEM;

			if(copy_from_user((void __user *)data, (void*)mem.data, mem.size) != 0)
			{
				OS_Free(data);
				return -EFAULT;
			}

			ret = SYS_IO_WriteRegArray(mem.addr, mem.size, data);

			OS_Free(data);
		}
		break;

		case SYS_IORW_READ_MEM_ARRAY:
		{
			LX_SYS_MEM_ARRAY_T mem;
			UINT8 *data;

			if(__copy_from_user((void*)&mem, (void __user *)arg, sizeof(LX_SYS_MEM_ARRAY_T)) != 0)
				return -EFAULT;

			if(mem.size == 0 || mem.data == NULL)
				return -EFAULT;

			if((data = (UINT8*)OS_Malloc(mem.size)) == NULL)
				return -ENOMEM;

			ret = SYS_IO_ReadRegArray(mem.addr, mem.size, data);

			if(copy_to_user((void __user *)mem.data, (void*)data, mem.size) != 0)
			{
				OS_Free(data);
				return -EFAULT;
			}

			OS_Free(data);
		}
		break;

		case SYS_IOW_SPREAD_SPRECTRUM:
		{
			LX_SPREAD_SPECTRUM_T ss;

			if(__copy_from_user((void*)&ss, (void __user *)arg, sizeof(LX_SPREAD_SPECTRUM_T)) != 0)
				return -EFAULT;

			ret = SYS_IO_SetSpreadSpectrum(&ss);
		}
		break;

		case SYS_IORW_DIAGNOSIS:
			diag_invoke_reset();
			diag_invoke_all();
			diag_report(LX_DIAG_REPORT_TYPE_PRINT);
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
user_initcall_grp("kdrv",SYS_Init);
#else
module_init(SYS_Init);
#endif
module_exit(SYS_Cleanup);

MODULE_AUTHOR("LGE");
MODULE_DESCRIPTION("base driver");
MODULE_LICENSE("GPL");
#endif

/** @} */

