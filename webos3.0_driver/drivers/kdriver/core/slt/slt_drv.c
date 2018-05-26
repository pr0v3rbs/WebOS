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
 *  main driver implementation for slt device.
 *
 *  @author		wunseo.yeo@lge.com
 *  @version	1.0
 *  @date		2014.08
 *
 *  @addtogroup lg115x_slt
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


#include "os_util.h"
#include "base_device.h"
#include "sys_regs.h"

#include "slt_drv.h"
#include "slt_proc.h"
#include "slt_kapi.h"
#include "slt_hal.h"

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
 *	main control block for slt device.
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
SLT_DEVICE_T;

/*----------------------------------------------------------------------------------------
  External Function Prototype Declarations
  ----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
  External Variables
  ----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
  global Functions
  ----------------------------------------------------------------------------------------*/
int		SLT_Init(void);
void	SLT_Cleanup(void);

/*----------------------------------------------------------------------------------------
  global Variables
  ----------------------------------------------------------------------------------------*/
int		g_slt_debug_fd;
int 	g_slt_major = SLT_MAJOR;
int 	g_slt_minor = SLT_MINOR;

/*----------------------------------------------------------------------------------------
  Static Function Prototypes Declarations
  ----------------------------------------------------------------------------------------*/
static int      SLT_Open(struct inode *, struct file *);
static int      SLT_Close(struct inode *, struct file *);
static long		SLT_Ioctl(struct file *file, unsigned int cmd, unsigned long arg);


/*----------------------------------------------------------------------------------------
  Static Variables
  ----------------------------------------------------------------------------------------*/

/**
 * main control block for slt device
 */
static SLT_DEVICE_T*		g_slt_device;

/**
 * file I/O description for slt device
 *
 */
static struct file_operations g_slt_fops =
{
	.open			= SLT_Open,
	.release		= SLT_Close,
	.unlocked_ioctl	= SLT_Ioctl,
	.read			= NULL,
	.write			= NULL,
};

/*========================================================================================
  Implementation Group
  ========================================================================================*/
/** Initialize the device environment before the real H/W initialization
 *
 *  @note main usage of this function is to initialize the HAL layer and memory size adjustment
 *  @note it's natural to keep this function blank :)
 */
void SLT_PreInit(void)
{
	/* TODO: do something */
}


int SLT_Init(void)
{
	int			i;
	int			err;
	dev_t		dev;

	/* Get the handle of debug output for slt device.
	 *
	 * Most module should open debug handle before the real initialization of module.
	 * As you know, debug_util offers 4 independent debug outputs for your device driver.
	 * So if you want to use all the debug outputs, you should initialize each debug output
	 * using OS_DEBUG_EnableModuleByIndex() function.
	 */
	g_slt_debug_fd = DBG_OPEN( SLT_MODULE );

	if ( g_slt_debug_fd < 0 )
	{
		SLT_ERROR("[SLT] can't get debug handle\n");
	}
	else
	{
		OS_DEBUG_EnableModule ( g_slt_debug_fd );

		OS_DEBUG_EnableModuleByIndex ( g_slt_debug_fd, SLT_LOG_LEVEL_ERROR,	DBG_COLOR_RED );
		OS_DEBUG_EnableModuleByIndex ( g_slt_debug_fd, SLT_LOG_LEVEL_WARN,	DBG_COLOR_RED );
		OS_DEBUG_EnableModuleByIndex ( g_slt_debug_fd, SLT_LOG_LEVEL_INFO,	DBG_COLOR_BLUE );
		OS_DEBUG_EnableModuleByIndex ( g_slt_debug_fd, SLT_LOG_LEVEL_PRINT,	DBG_COLOR_PURPLE );
	}

	/* allocate main device handler, register current device.
	 *
	 * If devie major is predefined then register device using that number.
	 * otherwise, major number of device is automatically assigned by Linux kernel.
	 *
	 */
	g_slt_device = (SLT_DEVICE_T*)OS_KMalloc( sizeof(SLT_DEVICE_T)*SLT_MAX_DEVICE );

	if ( NULL == g_slt_device )
	{
		SLT_ERROR("out of memory. can't allocate %d bytes\n", sizeof(SLT_DEVICE_T)* SLT_MAX_DEVICE );
		return -ENOMEM;
	}

	memset( g_slt_device, 0x0, sizeof(SLT_DEVICE_T)* SLT_MAX_DEVICE );

	if (g_slt_major)
	{
		dev = MKDEV( g_slt_major, g_slt_minor );
		err = register_chrdev_region(dev, SLT_MAX_DEVICE, SLT_MODULE );
	}
	else
	{
		err = alloc_chrdev_region(&dev, g_slt_minor, SLT_MAX_DEVICE, SLT_MODULE );
		g_slt_major = MAJOR(dev);
	}

	if ( err < 0 )
	{
		SLT_ERROR("can't register slt device\n" );
		return -EIO;
	}

	/* TODO : initialize your module not specific minor device */


	/* END */

	for ( i=0; i<SLT_MAX_DEVICE; i++ )
	{
		/* initialize cdev structure with predefined variable */
		dev = MKDEV( g_slt_major, g_slt_minor+i );
		cdev_init( &(g_slt_device[i].cdev), &g_slt_fops );
		g_slt_device[i].devno		= dev;
		g_slt_device[i].cdev.owner = THIS_MODULE;
		g_slt_device[i].cdev.ops   = &g_slt_fops;

		/* TODO: initialize minor device */


		/* END */

		err = cdev_add (&(g_slt_device[i].cdev), dev, 1 );

		if (err)
		{
			SLT_ERROR("error (%d) while adding slt device (%d.%d)\n", err, MAJOR(dev), MINOR(dev) );
			return -EIO;
		}
		OS_CreateDeviceClass ( g_slt_device[i].devno, "%s%d", SLT_MODULE, i );
	}


	if(SLT_HAL_Init() < 0)
	{
		SLT_ERROR("HAL_Init fail!\n");
	}

	SLT_PROC_Init();

	SLT_PRINT("slt device initialized\n");

	return 0;
}

void SLT_Cleanup(void)
{
	int i;
	dev_t dev = MKDEV( g_slt_major, g_slt_minor );

	/* cleanup proc system */
	SLT_PROC_Cleanup();
	SLT_HAL_Cleanup();

	/* remove all minor devicies and unregister current device */
	for ( i=0; i<SLT_MAX_DEVICE;i++)
	{
		/* TODO: cleanup each minor device */


		/* END */
		cdev_del( &(g_slt_device[i].cdev) );
	}

	/* TODO : cleanup your module not specific minor device */
	unregister_chrdev_region(dev, SLT_MAX_DEVICE );

	OS_Free( g_slt_device );
}


///////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * open handler for slt device
 *
 */
static int
SLT_Open(struct inode *inode, struct file *filp)
{
	int					major,minor;
	struct cdev*    	cdev;
	SLT_DEVICE_T*		slt_dev;

	cdev		= inode->i_cdev;
	slt_dev		= container_of ( cdev, SLT_DEVICE_T, cdev);

	major = imajor(inode);
	minor = iminor(inode);

	/* TODO : add your device specific code */
	if(slt_dev->dev_open_count == 0)
	{
		if(SLT_HAL_Open() < 0)
			return RET_ERROR;
	}

	slt_dev->dev_open_count++;
	filp->private_data = slt_dev;

	SLT_PRINT("device opened (%d:%d)\n", major, minor );

	return 0;
}

/**
 * release handler for slt device
 *
 */
static int
SLT_Close(struct inode *inode, struct file *file)
{
	int					major,minor;
	SLT_DEVICE_T*		slt_dev;
	struct cdev*		cdev;

	cdev		= inode->i_cdev;
	slt_dev		= container_of ( cdev, SLT_DEVICE_T, cdev);

	if ( slt_dev->dev_open_count > 0 )
	{
		--slt_dev->dev_open_count;
		if(slt_dev->dev_open_count == 0)
		{
			SLT_HAL_Close();
		}
	}

	/* TODO : add your device specific code */

	/* END */

	/* some debug */
	major = imajor(inode);
	minor = iminor(inode);

	SLT_PRINT("device closed (%d:%d)\n", major, minor );

	return 0;
}

/**
 * ioctl handler for slt device.
 *
 *
 * note: if you have some critial data, you should protect them using semaphore or spin lock.
 */
static long SLT_Ioctl  (struct file *file, unsigned int cmd, unsigned long arg)
{
	int ret = RET_ERROR;
	int err = RET_OK;
	SLT_DEVICE_T*	slt_dev;

	/*
	 * get current slt device object
	 */
	slt_dev	= (SLT_DEVICE_T*)file->private_data;

	/*
	 * check if IOCTL command is valid or not.
	 * - if magic value doesn't match, return error (-ENOTTY)
	 * - if command is out of range, return error (-ENOTTY)
	 *
	 * note) -ENOTTY means "Inappropriate ioctl for device.
	 */
	if (_IOC_TYPE(cmd) != SLT_IOC_MAGIC)
	{
		SLT_ERROR("invalid magic. magic=0x%02X\n", _IOC_TYPE(cmd) );
		return -ENOTTY;
	}
	if (_IOC_NR(cmd) > SLT_IOC_MAXNR)
	{
		SLT_ERROR("out of ioctl command. cmd_idx=%d\n", _IOC_NR(cmd) );
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
		SLT_ERROR("memory access error. cmd_idx=%d, rw=%c%c, memptr=%p\n",
				_IOC_NR(cmd),
				(_IOC_DIR(cmd) & _IOC_READ)? 'r':'-',
				(_IOC_DIR(cmd) & _IOC_WRITE)? 'w':'-',
				(void*)arg );
		return -EFAULT;
	}

	switch(cmd)
	{
	case SLT_IOW_SET_TEST_MODE:
		{
			LX_SLT_TEST_MODE_E mode;

			err = copy_from_user( &mode, (void*)arg, sizeof(LX_SLT_TEST_MODE_E) );
			SLT_CHECK_CODE(err > 0, goto exit, "copy from error\n");
			SLT_PRINT( "SLT_TEST_MODE = %d\n", mode );

			SLT_HAL_SetTestMode(&mode);
			SLT_CHECK_CODE( err < 0, goto exit, "slt test mode error" ); 

			ret = 0;
		}
		break;

	case SLT_IO_CRC_INTR_ENABLE:
		{
			SLT_HAL_IntrEnable();
			ret = 0;
		}
		break;

	case SLT_IO_CRC_INTR_DISABLE:
		{
			SLT_HAL_IntrDisable();
			ret = 0;
		}
		break;

	case SLT_IOR_GET_CRC_DATA:
		{
			LX_SLT_CRC_DATA_T *crc;
			crc = (LX_SLT_CRC_DATA_T*)kmalloc(sizeof(LX_SLT_CRC_DATA_T), GFP_KERNEL);
			SLT_CHECK_CODE( crc==NULL, goto exit, "error kmalloc\n" );

			err = SLT_HAL_GetRefCrc( crc );
			SLT_CHECK_CODE(err < 0, do {kfree(crc); goto exit;} while(0), "get_ref_crc error\n");

			err = copy_to_user((void __user *)arg, (void *)crc, sizeof(LX_SLT_CRC_DATA_T));
			SLT_CHECK_CODE(err > 0, do {kfree(crc); goto exit;} while(0), "copy to error\n");
			ret = 0;
		}
		break;

		default:
		{
			/* redundant check but it seems more readable */
			SLT_ERROR("ioctl:not defined");
			ret = -ENOTTY;
		}
	}

exit:
	return ret;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef KDRV_GLOBAL_LINK
#if defined(CONFIG_LG_BUILTIN_KDRIVER) && defined(CONFIG_LGSNAP)
user_initcall_grp("kdrv", SLT_Init);
#else
module_init(SLT_Init);
#endif
module_exit(SLT_Cleanup);

MODULE_AUTHOR("LGE");
MODULE_DESCRIPTION("base driver");
MODULE_LICENSE("GPL");
#endif

/** @} */

