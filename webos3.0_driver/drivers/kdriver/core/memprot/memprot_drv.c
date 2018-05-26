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
 *  main driver implementation for memprot device.
 *	memprot device will teach you how to make device driver with new platform.
 *
 *  author		sanghyeun.kim
 *  version		1.0
 *  date		2012.08.07
 *  note		Additional information.
 *
 *  @addtogroup lg1150_memprot
 *	@{
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/
#undef	SUPPORT_MEMPROT_DEVICE_READ_WRITE_FOPS

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/interrupt.h>
#include <asm/uaccess.h>
#include "os_util.h"
#include "base_device.h"
#include "memprot_drv.h"
#include "memprot_cfg.h"

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
 *	main control block for memprot device.
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
MEMPROT_DEVICE_T;

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/
extern	void	MEMPROT_PROC_Init(void);
extern	void	MEMPROT_PROC_Cleanup(void);
extern 	void 	MEMPROT_ReadViolation(unsigned monitor);
extern 	void	MEMPROT_RelProtectMonitor(unsigned monitor);
extern  unsigned int MEMPROT_GetMonitorInt(unsigned monitor);
extern	void	MEMPROT_SetProtectMonitor(unsigned monitor);
extern 	unsigned	MEMPROT_SetProtect(unsigned mointor ,unsigned reg1_s, unsigned reg1_e,  \
		unsigned reg2_s,unsigned reg2_e);
/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Functions
----------------------------------------------------------------------------------------*/
int		MEMPROT_Init(void);
void	MEMPROT_Cleanup(void);

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/
int		g_memprot_debug_fd;
int 	g_memprot_major = MEMPROT_MAJOR;
int 	g_memprot_minor = MEMPROT_MINOR;

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/
static int      MEMPROT_Open(struct inode *, struct file *);
static int      MEMPROT_Close(struct inode *, struct file *);
#ifdef HAVE_UNLOCKED_IOCTL
static long 		MEMPROT_Ioctl (struct file *, unsigned int, unsigned long );
#else
static int 		MEMPROT_Ioctl (struct inode *, struct file *, unsigned int, unsigned long );
#endif
#ifdef SUPPORT_MEMPROT_DEVICE_READ_WRITE_FOPS
static ssize_t  MEMPROT_Read(struct file *, char *, size_t, loff_t *);
static ssize_t  MEMPROT_Write(struct file *, const char *, size_t, loff_t *);
#endif

static irqreturn_t  MEMPROT_LBUS_irq_handler(int,void*);
static irqreturn_t  MEMPROT_GBUS_irq_handler(int,void*);
/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/

/**
 * main control block for memprot device
*/
static MEMPROT_DEVICE_T*		g_memprot_device;

static void*	gMEMPROT_LBUS_INT=NULL;
static void*	gMEMPROT_GBUS_INT=NULL;

/**
 * file I/O description for memprot device
 *
*/
static struct file_operations g_memprot_fops =
{
	.open 	= MEMPROT_Open,
	.release= MEMPROT_Close,

#ifdef HAVE_UNLOCKED_IOCTL
	.unlocked_ioctl = MEMPROT_Ioctl,
#else
	.ioctl	= MEMPROT_Ioctl,
#endif
#ifdef SUPPORT_MEMPROT_DEVICE_READ_WRITE_FOPS
	.read 	= MEMPROT_Read,
	.write 	= MEMPROT_Write,
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
void MEMPROT_PreInit(void)
{
    /* TODO: do something */
}

/**
 * Set IRQ and interrupt register for Memory protectcion.
 *
 */
void MEMPROT_Setup(void)
{

	/* assign  memprot monitors virtual addresses to get/set registers value  */
	/*  the initialization of memprot is done in the bootloader*/
	unsigned i;
	for(i = ENUM_MEMPROT_AUDIO ; i < ENUM_MEMPROT_MAX; i++)
		MEMPROT_SetProtectMonitor(i);

	/* Set Interrupt register*/
	gMEMPROT_LBUS_INT=ioremap(MEMPROT_LBUS_INT_STATUS, 0x4);
	gMEMPROT_GBUS_INT=ioremap(MEMPROT_GBUS_INT_STATUS, 0x4);

	/* Set IRQ of LBUS : IRQ is shared with B/W moinitoring module*/
	if(request_irq(H13_IRQ_LBUS, MEMPROT_LBUS_irq_handler, IRQF_SHARED, "memprot",g_memprot_device))
		MEMPROT_PRINT("[ ERROR ] Failed to request MEMPROT LBUS IRQ\n");

	/* Set IRQ of GBUS : IRQ is shared with B/W mointoring module*/
	if(request_irq(H13_IRQ_GBUS, MEMPROT_GBUS_irq_handler, IRQF_SHARED, "memprot",g_memprot_device))
		MEMPROT_PRINT("[ ERROR ] Failed to request MEMPROT GBUS IRQ\n");


}

/**
 * Free interrupt register and IRQ used by  Memory protectcion.
 *
 */
void MEMPROT_Release(void)
{

	//for testing
	//for(monitor= ENUM_MEMPROT_AUDIO; monitor <= ENUM_MEMPROT_GFX;++monitor)
	//	MEMPROT_RelProtectMonitor(monitor);

	iounmap(gMEMPROT_LBUS_INT);
	gMEMPROT_LBUS_INT=NULL;

	iounmap(gMEMPROT_GBUS_INT);
	gMEMPROT_GBUS_INT=NULL;

	free_irq(H13_IRQ_LBUS,g_memprot_device);
	free_irq(H13_IRQ_GBUS,g_memprot_device);

}

int MEMPROT_Init(void)
{
	int			i;
	int			err;
	dev_t		dev;

	/* Get the handle of debug output for memprot device.
	 *
	 * Most module should open debug handle before the real initialization of module.
	 * As you know, debug_util offers 4 independent debug outputs for your device driver.
	 * So if you want to use all the debug outputs, you should initialize each debug output
	 * using OS_DEBUG_EnableModuleByIndex() function.
	 */
	g_memprot_debug_fd = DBG_OPEN( MEMPROT_MODULE );
	OS_DEBUG_EnableModule ( g_memprot_debug_fd );

	OS_DEBUG_EnableModuleByIndex ( g_memprot_debug_fd, 0, DBG_COLOR_PURPLE );
	OS_DEBUG_EnableModuleByIndex ( g_memprot_debug_fd, 1, DBG_COLOR_NONE );
	OS_DEBUG_EnableModuleByIndex ( g_memprot_debug_fd, 2, DBG_COLOR_NONE );
	OS_DEBUG_EnableModuleByIndex ( g_memprot_debug_fd, 3, DBG_COLOR_NONE );

	/* allocate main device handler, register current device.
	 *
	 * If devie major is predefined then register device using that number.
	 * otherwise, major number of device is automatically assigned by Linux kernel.
	 *
	 */
	g_memprot_device = (MEMPROT_DEVICE_T*)OS_KMalloc( sizeof(MEMPROT_DEVICE_T)*MEMPROT_MAX_DEVICE );

	if ( NULL == g_memprot_device )
	{
		DBG_PRINT_ERROR("out of memory. can't allocate %d bytes\n", sizeof(MEMPROT_DEVICE_T)* MEMPROT_MAX_DEVICE );
		return -ENOMEM;
	}

	memset( g_memprot_device, 0x0, sizeof(MEMPROT_DEVICE_T)* MEMPROT_MAX_DEVICE );

	if (g_memprot_major)
	{
		dev = MKDEV( g_memprot_major, g_memprot_minor );
		err = register_chrdev_region(dev, MEMPROT_MAX_DEVICE, MEMPROT_MODULE );
	}
	else
	{
		err = alloc_chrdev_region(&dev, g_memprot_minor, MEMPROT_MAX_DEVICE, MEMPROT_MODULE );
		g_memprot_major = MAJOR(dev);
	}

	if ( err < 0 )
	{
		DBG_PRINT_ERROR("can't register memprot device\n" );
		return -EIO;
	}

	/* TODO : initialize your module not specific minor device */
	MEMPROT_Setup();

	// for testing
	//for(i = ENUM_MEMPROT_AUDIO ; i < ENUM_MEMPROT_MAX; i++)
	//	MEMPROT_SetProtect(i,0x00,0x1f,0x20,0x3f);

	/* END */

	for ( i=0; i<MEMPROT_MAX_DEVICE; i++ )
	{
		/* initialize cdev structure with predefined variable */
		dev = MKDEV( g_memprot_major, g_memprot_minor+i );
		cdev_init( &(g_memprot_device[i].cdev), &g_memprot_fops );
		g_memprot_device[i].devno		= dev;
		g_memprot_device[i].cdev.owner = THIS_MODULE;
		g_memprot_device[i].cdev.ops   = &g_memprot_fops;

		/* TODO: initialize minor device */

		/* END */

		err = cdev_add (&(g_memprot_device[i].cdev), dev, 1 );

		if (err)
		{
			DBG_PRINT_ERROR("error (%d) while adding memprot device (%d.%d)\n", err, MAJOR(dev), MINOR(dev) );
			return -EIO;
		}
        OS_CreateDeviceClass ( g_memprot_device[i].devno, "%s%d", MEMPROT_MODULE, i );
	}

	/* initialize proc system */
	MEMPROT_PROC_Init ( );

	MEMPROT_PRINT("memprot device initialized\n");

	return 0;
}

void MEMPROT_Cleanup(void)
{
	int i;
	dev_t dev = MKDEV( g_memprot_major, g_memprot_minor );

	/* cleanup proc system */
	MEMPROT_PROC_Cleanup( );

	/* remove all minor devicies and unregister current device */
	for ( i=0; i<MEMPROT_MAX_DEVICE;i++)
	{
		/* TODO: cleanup each minor device */


		/* END */
		cdev_del( &(g_memprot_device[i].cdev) );
	}

	/* TODO : cleanup your module not specific minor device */
	MEMPROT_Release();

	unregister_chrdev_region(dev, MEMPROT_MAX_DEVICE );

	OS_Free( g_memprot_device );
}


///////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * open handler for memprot device
 *
 */
static int
MEMPROT_Open(struct inode *inode, struct file *filp)
{
    int					major,minor;
    struct cdev*    	cdev;
    MEMPROT_DEVICE_T*	my_dev;

    cdev	= inode->i_cdev;
    my_dev	= container_of ( cdev, MEMPROT_DEVICE_T, cdev);

    /* TODO : add your device specific code */


	/* END */

    my_dev->dev_open_count++;
    filp->private_data = my_dev;

	/* some debug */
    major = imajor(inode);
    minor = iminor(inode);
    MEMPROT_PRINT("device opened (%d:%d)\n", major, minor );

    return 0;
}

/**
 * LBUS IRQ  handler for memprot device.
 *
 */

static irqreturn_t  MEMPROT_LBUS_irq_handler(int aa,void* bb)
{
	unsigned lbusInterrupt=0;
	unsigned i;

	lbusInterrupt=ioread32(gMEMPROT_LBUS_INT);

	for( i=ENUM_MEMPROT_AUDIO; i <= ENUM_MEMPROT_CPU_M ; ++i)
	{
		if(lbusInterrupt & MEMPROT_GetMonitorInt(i) )
			MEMPROT_ReadViolation(i);
	}

    return IRQ_HANDLED;
}

/**
 * GBUS IRQ  handler for memprot device.
 *
 */

static irqreturn_t  MEMPROT_GBUS_irq_handler(int aa,void* bb)
{
	unsigned gbusInterrupt=0;
	unsigned i;

	gbusInterrupt=ioread32(gMEMPROT_GBUS_INT);

	for( i= ENUM_MEMPROT_DE_A; i <= ENUM_MEMPROT_CPU_S ; ++i)
	{
		if(gbusInterrupt & MEMPROT_GetMonitorInt(i) )
			MEMPROT_ReadViolation(i);
	}

    return IRQ_HANDLED;
}



/**
 * release handler for memprot device
 *
 */
static int
MEMPROT_Close(struct inode *inode, struct file *file)
{
    int					major,minor;
    MEMPROT_DEVICE_T*	my_dev;
    struct cdev*		cdev;

    cdev	= inode->i_cdev;
    my_dev	= container_of ( cdev, MEMPROT_DEVICE_T, cdev);

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
    MEMPROT_PRINT("device closed (%d:%d)\n", major, minor );
    return 0;
}

/**
 * ioctl handler for memprot device.
 *
 *
 * note: if you have some critial data, you should protect them using semaphore or spin lock.
 */
#ifdef HAVE_UNLOCKED_IOCTL
static long
MEMPROT_Ioctl ( struct file *filp, unsigned int cmd, unsigned long arg )
#else
static int
MEMPROT_Ioctl ( struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg )
#endif
{
    int err = 0, ret = 0;

    MEMPROT_DEVICE_T*	my_dev;
    struct cdev*		cdev;

	/*
	 * get current memprot device object
	 */
#ifdef HAVE_UNLOCKED_IOCTL
	my_dev	= (MEMPROT_DEVICE_T *)filp->private_data;
	cdev	= &my_dev->cdev;
#else
    cdev	= inode->i_cdev;
    my_dev	= container_of ( cdev, MEMPROT_DEVICE_T, cdev);
#endif
    /*
     * check if IOCTL command is valid or not.
     * - if magic value doesn't match, return error (-ENOTTY)
     * - if command is out of range, return error (-ENOTTY)
     *
     * note) -ENOTTY means "Inappropriate ioctl for device.
     */
    if (_IOC_TYPE(cmd) != MEMPROT_IOC_MAGIC)
    {
    	DBG_PRINT_WARNING("invalid magic. magic=0x%02X\n", _IOC_TYPE(cmd) );
    	return -ENOTTY;
    }
    if (_IOC_NR(cmd) > MEMPROT_IOC_MAXNR)
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

	MEMPROT_TRACE("cmd = %08X (cmd_idx=%d)\n", cmd, _IOC_NR(cmd) );

	switch(cmd)
	{
		case MEMPROT_IO_RESET:
		{
			MEMPROT_PRINT("reset ok\n");
			ret = 0;
		}
		break;

		case MEMPROT_IOR_CHIP_REV_INFO:
		{
			CHIP_REV_INFO_T	rev_info;

			rev_info.version = 0x100;
			rev_info.date[0] =  9;	/* 2009/11/24 */
			rev_info.date[0] = 11;
			rev_info.date[0] = 24;

			MEMPROT_PRINT("rev_info (%0X, %d:%d:%d)\n", rev_info.version,
														rev_info.date[0], rev_info.date[1], rev_info.date[2] );

	        if ( copy_to_user((void __user *)arg, &rev_info, sizeof(CHIP_REV_INFO_T)) )
    	        return -EFAULT;

			ret = 0;
		}
		break;

		case MEMPROT_IOW_WRITE_UINT32:
		{
			UINT32	data;

			ret = __get_user( data, (UINT32 __user *)arg );

			MEMPROT_PRINT("data = 0x%X\n", data );

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
user_initcall_grp("kdrv",MEMPROT_Init);
#else
module_init(MEMPROT_Init);
#endif
module_exit(MEMPROT_Cleanup);

MODULE_AUTHOR("LGE");
MODULE_DESCRIPTION("base driver");
MODULE_LICENSE("GPL");
#endif

/** @} */

