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
 *  main driver implementation for aad device.
 *	aad device will teach you how to make device driver with new platform.
 *
 *  author		jongsang.oh (jongsang.oh@lge.com)
 *  version		1.0
 *  date		2010.01.26
 *  note		Additional information.
 *
 *  @addtogroup lg1150_aad
 *	@{
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/
#undef	SUPPORT_AAD_DEVICE_READ_WRITE_FOPS
//#define SUPPORT_AAD_UNLOCKED_IOCTL


/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/interrupt.h>    /**< For isr */
#include <linux/irq.h>			/**< For isr */
#include <asm/uaccess.h>
#include <asm/io.h>				/**< For ioremap_nocache */
#include <linux/kthread.h>

#ifdef KDRV_CONFIG_PM	// added by SC Jung for quick booting
#include <linux/platform_device.h>
#endif

#include "os_util.h"
#include "base_device.h"
#include "aad_drv.h"
#include "aad_coredrv.h"

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
 *	main control block for aad device.
 *	each minor device has unique control block
 *
 */
typedef struct
{
// BEGIN of common device
	int						dev_open_count;	///< check if device is opened or not
	dev_t					devno;			///< device number
	struct cdev				cdev;			///< char device structure
// END of command device

// BEGIN of device specific data
	bool					is_initialized;	//check if device is initialized

// END of device specific data
}AAD_DEVICE_T;

#ifdef KDRV_CONFIG_PM
typedef struct
{
	// add here extra parameter
	bool	is_suspended;
}AAD_DRVDATA_T;
#endif

/* AAD Debug Print Number. */
#define NUM_OF_AAD_PRINT_TYPE		9
#define NUM_OF_AAD_PRINT_COLOR		8

/* AAD Debug Print Type */
UINT8 *ui8AadPrintType[NUM_OF_AAD_PRINT_TYPE]  = {	\
			"AAD_ERROR", "AAD_WARN",  "AAD_NOTI",      "AAD_INFO",      "AAD_DEBUG", \
			"AAD_TRACE", "AAD_PRINT", "AAD_ISR_DEBUG", "AAD_DEBUG_TMP" };

/* AAD Debug Print Color */
UINT8 *ui8AadPrintColor[NUM_OF_AAD_PRINT_COLOR] = {	\
			"COLOR_BLACK",
			"COLOR_RED",
			"COLOR_GREEN",
			"COLOR_YELLOW",
			"COLOR_BLUE",
			"COLOR_PURPLE",
			"COLOR_CYAN",
			"COLOR_GRAY",
};


/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/
extern	void	AAD_PROC_Init(void);
extern	void	AAD_PROC_Cleanup(void);
extern irqreturn_t aad_interrupt(int irq, void *dev_id, struct pt_regs *regs);

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/
#ifdef KDRV_CONFIG_PM
extern BOOLEAN					g_bAadSuspended;
#endif

//This value is set by IOCTL function.
extern BOOLEAN g_bHighDevOnOff;		///< The Status of High Deviation to setted


/*----------------------------------------------------------------------------------------
	global Functions
----------------------------------------------------------------------------------------*/
int		AAD_Init(void);
void	AAD_Cleanup(void);

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/
int		g_aad_debug_fd;
int 	g_aad_major = AAD_MAJOR;
int 	g_aad_minor = AAD_MINOR;
int		g_aad_irq_dev = 0;

struct task_struct 	*g_pstAADMonitorTask = NULL;

#ifdef KDRV_CONFIG_PM
BOOLEAN	g_aad_open_done = FALSE;
#endif


/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/
static int      AAD_Open(struct inode *, struct file *);
static int      AAD_Close(struct inode *, struct file *);
#ifdef HAVE_UNLOCKED_IOCTL
static long		AAD_Ioctl(struct file *, unsigned int, unsigned long);
#else
static int		AAD_Ioctl(struct inode *, struct file *, unsigned int, unsigned long);
#endif
#ifdef SUPPORT_AAD_DEVICE_READ_WRITE_FOPS
static ssize_t  AAD_Read(struct file *, char *, size_t, loff_t *);
static ssize_t  AAD_Write(struct file *, const char *, size_t, loff_t *);
#endif

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/

/**
 * main control block for aad device
*/
static AAD_DEVICE_T*		g_aad_device;

/**
 * file I/O description for aad device
 *
*/
static struct file_operations g_aad_fops =
{
	.open 	= AAD_Open,
	.release= AAD_Close,
#ifdef HAVE_UNLOCKED_IOCTL
	.unlocked_ioctl	= AAD_Ioctl,
#else
	.ioctl	= AAD_Ioctl,
#endif
#ifdef SUPPORT_AAD_DEVICE_READ_WRITE_FOPS
	.read 	= AAD_Read,
	.write 	= AAD_Write,
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
static int AAD_suspend(struct platform_device *pdev, pm_message_t state)
{
#ifdef KDRV_CONFIG_PM_AAD
	AAD_DRVDATA_T *drv_data;

	if(g_aad_open_done == FALSE)
	{
		AAD_DEBUG("[%s] AAD_Open is not done. Skip Suspend!!!\n", AAD_MODULE);
		return 0;
	}

	printk("AAD - suspend BEGIN\n");

	drv_data = platform_get_drvdata(pdev);

	// add here the suspend code

	/* Set a suspend state. */
	drv_data->is_suspended = 1;

	/* Update Internal Status Value. */
	g_bAadSuspended = TRUE;

	printk("AAD - suspend DONE\n");
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
static int AAD_resume(struct platform_device *pdev)
{
#ifdef KDRV_CONFIG_PM_AAD
	AAD_DRVDATA_T *drv_data;

	if(g_aad_open_done == FALSE)
	{
		AAD_DEBUG("[%s] AAD_Open is not done. Skip Resume!!!\n", AAD_MODULE);
		return 0;
	}

	printk("AAD - resume BEGIN\n");

	drv_data = platform_get_drvdata(pdev);

	if(drv_data->is_suspended == 0) return -1;

	// add here the resume code

	/* Set a resume state. */
	drv_data->is_suspended = 0;

	/* Update Internal Status Value. */
	g_bAadSuspended = FALSE;

	printk("AAD - resume DONE\n");
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
static int __init AAD_probe(struct platform_device *pdev)
{
	AAD_DRVDATA_T *drv_data;

	// add here driver registering code & allocating resource code
	drv_data = (AAD_DRVDATA_T *)kmalloc(sizeof(AAD_DRVDATA_T), GFP_KERNEL);

	/* Set a resume state. */
	drv_data->is_suspended = 0;

	platform_set_drvdata(pdev, drv_data);

	AAD_PRINT("[%s] done probe\n", AAD_MODULE);
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
static int  AAD_remove(struct platform_device *pdev)
{
	AAD_DRVDATA_T *drv_data;

	// add here driver unregistering code & deallocating resource code

	drv_data = platform_get_drvdata(pdev);

	if(drv_data != NULL)
		kfree(drv_data);

	AAD_PRINT("[%s] released\n", AAD_MODULE);
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
static void  AAD_release(struct device *dev)
{
	AAD_PRINT("device released\n");
}

/*
 *	module platform driver structure
 */
static struct platform_driver aad_driver =
{
	.probe          = AAD_probe,
	.suspend        = AAD_suspend,
	.remove         = AAD_remove,
	.resume         = AAD_resume,
	.driver         =
	{
		.name   = AAD_MODULE,
	},
};

static struct platform_device aad_device = {
	.name = AAD_MODULE,
	.id = 0,
	.id = -1,
	.dev = {
		.release = AAD_release,
	},
};
#endif

/** Initialize the device environment before the real H/W initialization
 *
 *	@note main usage of this function is to initialize the HAL layer and memory size adjustment
 *	@note it's natural to keep this function blank :)
 */
void AAD_PreInit(void)
{
	/* TODO: do something */
}

int AAD_Init(void)
{
	int			i;
	int			err;
	dev_t		dev;

	/* Get the handle of debug output for aad device.
	 *
	 * Most module should open debug handle before the real initialization of module.
	 * As you know, debug_util offers 4 independent debug outputs for your device driver.
	 * So if you want to use all the debug outputs, you should initialize each debug output
	 * using OS_DEBUG_EnableModuleByIndex() function.
	 */
	g_aad_debug_fd = DBG_OPEN( AAD_MODULE );

	if ( g_aad_debug_fd < 0 )
	{
		DBG_PRINT_ERROR("debug system shutdowned.\n" );
		return -EIO;
	}

	OS_DEBUG_EnableModule ( g_aad_debug_fd );

	OS_DEBUG_EnableModuleByIndex (g_aad_debug_fd, LX_LOGM_LEVEL_ERROR, DBG_COLOR_RED);		//AAD_ERROR
	OS_DEBUG_EnableModuleByIndex (g_aad_debug_fd, LX_LOGM_LEVEL_WARNING, DBG_COLOR_GREEN);	//AAD_WARN
	OS_DEBUG_DisableModuleByIndex(g_aad_debug_fd, LX_LOGM_LEVEL_NOTI);						//AAD_NOTI
	OS_DEBUG_DisableModuleByIndex(g_aad_debug_fd, LX_LOGM_LEVEL_INFO);						//AAD_INFO
	OS_DEBUG_DisableModuleByIndex(g_aad_debug_fd, LX_LOGM_LEVEL_DEBUG);						//AAD_DEBUG
	OS_DEBUG_DisableModuleByIndex(g_aad_debug_fd, LX_LOGM_LEVEL_TRACE);						//AAD_TRACE
	OS_DEBUG_DisableModuleByIndex(g_aad_debug_fd, LX_LOGM_LEVEL_DEBUG + 1);					//AAD_PRINT
	OS_DEBUG_DisableModuleByIndex(g_aad_debug_fd, LX_LOGM_LEVEL_DEBUG + 2);					//AAD_ISR_DEBUG
	OS_DEBUG_DisableModuleByIndex(g_aad_debug_fd, LX_LOGM_LEVEL_DEBUG + 3);					//AAD_DEBUG_TMP

#ifdef KDRV_CONFIG_PM
	// added by SC Jung for quick booting
	if(platform_driver_register(&aad_driver) < 0)
	{
		AAD_PRINT("[%s] platform driver register failed\n", AAD_MODULE);
	}
	else
	{
		if(platform_device_register(&aad_device))
		{
			platform_driver_unregister(&aad_driver);
			AAD_ERROR("[%s] platform device register failed\n", AAD_MODULE);
		}
		else
		{
			AAD_PRINT("[%s] platform register done\n", AAD_MODULE);
		}
	}
#endif

	/* allocate main device handler, register current device.
	 *
	 * If devie major is predefined then register device using that number.
	 * otherwise, major number of device is automatically assigned by Linux kernel.
	 *
	 */
	g_aad_device = (AAD_DEVICE_T*)OS_KMalloc( sizeof(AAD_DEVICE_T)*AAD_MAX_DEVICE );

	if ( NULL == g_aad_device )
	{
		DBG_PRINT_ERROR("out of memory. can't allocate %d bytes\n", sizeof(AAD_DEVICE_T)* AAD_MAX_DEVICE );
		return -ENOMEM;
	}

	memset( g_aad_device, 0x0, sizeof(AAD_DEVICE_T)* AAD_MAX_DEVICE );

	if (g_aad_major)
	{
		dev = MKDEV( g_aad_major, g_aad_minor );
		err = register_chrdev_region(dev, AAD_MAX_DEVICE, AAD_MODULE );
	}
	else
	{
		err = alloc_chrdev_region(&dev, g_aad_minor, AAD_MAX_DEVICE, AAD_MODULE );
		g_aad_major = MAJOR(dev);
	}

	if ( err < 0 )
	{
		DBG_PRINT_ERROR("can't register aad device\n" );
		return -EIO;
	}

	/* TODO : initialize your module not specific minor device */

	/* END */

	for ( i=0; i<AAD_MAX_DEVICE; i++ )
	{
		/* initialize cdev structure with predefined variable */
		dev = MKDEV( g_aad_major, g_aad_minor+i );
		cdev_init( &(g_aad_device[i].cdev), &g_aad_fops );
		g_aad_device[i].devno		= dev;
		g_aad_device[i].cdev.owner = THIS_MODULE;
		g_aad_device[i].cdev.ops   = &g_aad_fops;

		/* TODO: initialize minor device */

		/* END */

		err = cdev_add (&(g_aad_device[i].cdev), dev, 1 );

		if (err)
		{
			DBG_PRINT_ERROR("error (%d) while adding aad device (%d.%d)\n", err, MAJOR(dev), MINOR(dev) );
			return -EIO;
		}

		/* Support udev for GCD platform */
        OS_CreateDeviceClass ( g_aad_device[i].devno, "%s%d", AAD_MODULE, i );

	}

	/* Check LX Chip Revision Number */
	if (lx_chip_rev() >= LX_CHIP_REV(H15, A0))
	{
		AAD_DEBUG_TMP ("LX_CHIP_REV(H15, XX)(0x%X)\n", lx_chip_rev());
		pstAadRegCfg = &gstAadRegCfg[4];
	}
	else if (lx_chip_rev() >= LX_CHIP_REV(H14, A0))
	{
		AAD_DEBUG_TMP ("LX_CHIP_REV(H14, XX)(0x%X)\n", lx_chip_rev());
		pstAadRegCfg = &gstAadRegCfg[3];
	}
	else if (lx_chip_rev() >= LX_CHIP_REV(M14, B0))
	{
		AAD_DEBUG_TMP ("LX_CHIP_REV(M14, XX)(0x%X)\n", lx_chip_rev());
		pstAadRegCfg = &gstAadRegCfg[2];
	}
	else if (lx_chip_rev() >= LX_CHIP_REV(M14, A0))
	{
		AAD_DEBUG_TMP ("LX_CHIP_REV(M14, XX)(0x%X)\n", lx_chip_rev());
		pstAadRegCfg = &gstAadRegCfg[1];
	}
	else if (lx_chip_rev() >= LX_CHIP_REV(H13, A0))
	{
		AAD_DEBUG_TMP ("LX_CHIP_REV(H13, XX)(0x%X)\n", lx_chip_rev());
		pstAadRegCfg = &gstAadRegCfg[0];
	}
	else
	{
		DBG_PRINT_ERROR("AAD : LX_CHIP_REV => Unknown(0x%X) : ERROR\n", lx_chip_rev());
		return -EIO;
	}

	//Initialize Analog audio decoder register base
	AAD_DEBUG_TMP("reg_base_addr = 0x%X, size = 0x%X\n", pstAadRegCfg->reg_base_addr, pstAadRegCfg->reg_size);
	gpRealRegAAD = (volatile AAD_REG_T *)ioremap(pstAadRegCfg->reg_base_addr, pstAadRegCfg->reg_size);
	if(gpRealRegAAD == NULL)
	{
		DBG_PRINT_ERROR("ERROR : can't allocate for register\n");
		return -EIO;
	}

	gpRegAAD = (AAD_REG_T *)kmalloc(sizeof(AAD_REG_T), GFP_KERNEL);
	if(gpRegAAD == NULL)
	{
		DBG_PRINT_ERROR("ERROR : can't allocate for register\n");
		return -EIO;
	}

	AAD_DEBUG_TMP("gpRealRegAAD = 0x%p, gpRegAAD = 0x%p\n", gpRealRegAAD, gpRegAAD);

	/* initialize proc system */
	AAD_PROC_Init ( );

	AAD_PRINT("aad device initialized\n");

	return 0;
}

void AAD_Cleanup(void)
{
	int i;
	dev_t dev = MKDEV( g_aad_major, g_aad_minor );

	/* cleanup proc system */
	AAD_PROC_Cleanup( );

	/* remove all minor devicies and unregister current device */
	for ( i=0; i<AAD_MAX_DEVICE;i++)
	{
		/* TODO: cleanup each minor device */


		/* END */
		cdev_del( &(g_aad_device[i].cdev) );
	}

#ifdef KDRV_CONFIG_PM
	// added by SC Jung for quick booting
	platform_driver_unregister(&aad_driver);
	platform_device_unregister(&aad_device);
#endif

	/* TODO : cleanup your module not specific minor device */

	unregister_chrdev_region(dev, AAD_MAX_DEVICE );
	OS_Free( g_aad_device );

	AAD_DEBUG("aad device cleanup\n");
}


///////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * open handler for aad device
 *
 */
static int
AAD_Open(struct inode *inode, struct file *filp)
{
    int				major,minor;
    struct cdev*    cdev;
    AAD_DEVICE_T*	my_dev;

	int				iErrReturn = 0;

    cdev	= inode->i_cdev;
    my_dev	= container_of ( cdev, AAD_DEVICE_T, cdev);

    /* TODO : add your device specific code */
	if ( my_dev->dev_open_count == 0 )
	{
		//reset initialized value
		my_dev->is_initialized = 0;

		//Intialize interrupt request
		if (lx_chip_rev() >= LX_CHIP_REV(H13, A0))
		{
			iErrReturn = request_irq(pstAadRegCfg->irq_num, (irq_handler_t)aad_interrupt, 0, "AAD", &g_aad_irq_dev);

			if (iErrReturn) {
				AAD_ERROR ("request_irq IRQ_AAD in %s is failed %d\n", "AAD ", iErrReturn);
				return -1;
			}
			AAD_PRINT("aad irq registered :%d:\n", pstAadRegCfg->irq_num);
		}
	}
	/* END */

    my_dev->dev_open_count++;
    filp->private_data = my_dev;

	/* some debug */
    major = imajor(inode);
    minor = iminor(inode);
    AAD_PRINT("device opened (%d:%d)\n", major, minor );

#ifdef KDRV_CONFIG_PM
	g_aad_open_done = TRUE;
#endif

    return 0;
}

/**
 * release handler for aad device
 *
 */
static int
AAD_Close(struct inode *inode, struct file *file)
{
    int					major,minor;
    AAD_DEVICE_T*		my_dev;
    struct cdev*		cdev;

    cdev	= inode->i_cdev;
    my_dev	= container_of ( cdev, AAD_DEVICE_T, cdev);

    if ( my_dev->dev_open_count > 0 )
    {
        --my_dev->dev_open_count;
    }

    /* TODO : add your device specific code */
	if ( my_dev->dev_open_count <= 0 )
	{
		if (lx_chip_rev() >= LX_CHIP_REV(H13, A0))
		{
			free_irq(pstAadRegCfg->irq_num, &g_aad_irq_dev);
			AAD_PRINT("free_irq IRQ_AAD released :%d:\n", pstAadRegCfg->irq_num);
		}

		//Release monitor thread
		if ( g_pstAADMonitorTask != NULL )
		{
			kthread_stop( g_pstAADMonitorTask );
			AAD_PRINT("kthread_stop for AAD Monitor task\n");
		}
	}
	/* END */

	/* some debug */
    major = imajor(inode);
    minor = iminor(inode);

    AAD_PRINT("device closed (%d:%d)\n", major, minor );
    return 0;
}

/**
 * ioctl handler for aad device.
 *
 *
 * note: if you have some critial data, you should protect them using semaphore or spin lock.
 */
#ifdef HAVE_UNLOCKED_IOCTL
static long AAD_Ioctl ( struct file* filp, unsigned int cmd, unsigned long arg )
#else
static int  AAD_Ioctl ( struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg )
#endif
{
    int err = 0, ret = 0;

    AAD_DEVICE_T*	my_dev;
    struct cdev*	cdev;

	/*
	 * get current aad device object
	 */
#ifdef HAVE_UNLOCKED_IOCTL
	my_dev  = (AAD_DEVICE_T *)filp->private_data;
	cdev    = &my_dev->cdev;
#else
    cdev = inode->i_cdev;
    my_dev = container_of ( cdev, AAD_DEVICE_T, cdev);
#endif

    /*
     * check if IOCTL command is valid or not.
     * - if magic value doesn't match, return error (-ENOTTY)
     * - if command is out of range, return error (-ENOTTY)
     *
     * note) -ENOTTY means "Inappropriate ioctl for device.
     */
    if (_IOC_TYPE(cmd) != AAD_IOC_MAGIC)
    {
    	DBG_PRINT_WARNING("invalid magic. magic=0x%02X\n", _IOC_TYPE(cmd) );
    	return -ENOTTY;
    }
    if (_IOC_NR(cmd) > AAD_IOC_MAXNR)
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

	AAD_TRACE("cmd = %08X (cmd_idx=%d)\n", cmd, _IOC_NR(cmd) );

	switch(cmd)
	{
		case AAD_IOW_INIT_MODULE:
		{
			LX_AAD_INIT_TYPE_T 	i_eInitType;

			AAD_TRACE("AAD_IOW_INIT_MODULE...\n");
			if ( my_dev->is_initialized == 0 )
			{
				if ( copy_from_user(&i_eInitType, (void __user *)arg, sizeof(LX_AAD_INIT_TYPE_T)) )
					return RET_ERROR;

				ret = AAD_SIF_InitModule(i_eInitType, my_dev->is_initialized);

				//Create thread for AAD module for A2 monitor task.
				g_pstAADMonitorTask = kthread_run( AAD_Monitor_Task, NULL, "AADMTASK" );

				if ( g_pstAADMonitorTask )
				{
					AAD_PRINT("AAD> Init - Monitor task created successfully\n");
				}
				else
				{
					AAD_ERROR("ERROR> AAD Monitor task creation failed!!!\n");
				}

				//set my_dev->is_initialized value
				if(ret == RET_OK)
				{
					my_dev->is_initialized = 1;
				}
			}
			else
			{
				if ( copy_from_user(&i_eInitType, (void __user *)arg, sizeof(LX_AAD_INIT_TYPE_T)) )
					return RET_ERROR;

				ret = AAD_SIF_InitModule(i_eInitType, my_dev->is_initialized);
				AAD_DEBUG("AAD_IOW_INIT_MODULE is recalled.(%d)\n", my_dev->dev_open_count);
			}
		}
		break;

		case AAD_IOW_SIF_SET_HIGH_DEV_MODE:
		{
			LX_AAD_HIGH_DEV_SET_T 	i_eHighDevLevel;

			memset(&i_eHighDevLevel, 0x0, sizeof(LX_AAD_HIGH_DEV_SET_T));

			AAD_TRACE("AAD_IOW_SIF_SET_HIGH_DEV_MODE...\n");

			if ( copy_from_user( &i_eHighDevLevel, (void __user *)arg, sizeof(LX_AAD_HIGH_DEV_SET_T ) ) )
				return RET_ERROR;

			/* Set a g_bHighDevOnOff variable from app. call.*/
			g_bHighDevOnOff = i_eHighDevLevel.bHighDevOnOff;
			AAD_WARN("SET_HIGH_DEV_MODE : g_bHighDevOnOff = %d\n", g_bHighDevOnOff);

			ret = AAD_SIF_SetHighDeviationMode( i_eHighDevLevel );
		}
		break;

		case AAD_IOW_SIF_SET_BAND_SETUP:
		{
			LX_AAD_SIF_SOUNDSYSTEM_T	i_eSoundSystem;

			memset(&i_eSoundSystem, 0x0, sizeof(LX_AAD_SIF_SOUNDSYSTEM_T));

			AAD_TRACE("AAD_IOW_SIF_SET_BAND_SETUP...\n");

			if ( copy_from_user( &i_eSoundSystem, (void __user *)arg, sizeof(LX_AAD_SIF_SOUNDSYSTEM_T ) ) )
				return RET_ERROR;

			ret = AAD_SIF_SetBandSetup( i_eSoundSystem );
		}
		break;

		case AAD_IOW_SIF_SET_MODE_SETUP:
		{
			LX_AAD_SIF_STANDARD_T		i_eStandard;

			AAD_TRACE("AAD_IOW_SIF_SET_MODE_SETUP...\n");

			if ( copy_from_user( &i_eStandard, (void __user *)arg, sizeof(LX_AAD_SIF_STANDARD_T ) ) )
				return RET_ERROR;

			ret = AAD_SIF_SetModeSetup( i_eStandard );
		}
		break;

		case AAD_IOW_SIF_SET_USER_ANALOG_MODE:
		{
			LX_ATV_AUDIO_MODE_SET_T 	i_eAudioModeSet;

			AAD_TRACE("AAD_IOW_SIF_SET_USER_ANALOG_MODE...\n");

			if ( copy_from_user( &i_eAudioModeSet, (void __user *)arg, sizeof(LX_ATV_AUDIO_MODE_SET_T ) ) )
				return RET_ERROR;

			ret = AAD_SIF_SetUserAnalogMode( i_eAudioModeSet );
		}
		break;

		case AAD_IOW_SIF_SET_THRESHOLD_LEVEL:
		{
			LX_AAD_SIF_THRESHOLD_T	i_stTresholdLevel;

			AAD_TRACE("AAD_IOW_SIF_SET_THRESHOLD_LEVEL...\n");

			if ( copy_from_user( &i_stTresholdLevel, (void __user *)arg, sizeof(LX_AAD_SIF_THRESHOLD_T ) ) )
				return RET_ERROR;

			ret = AAD_SIF_SetThresholdLevel( i_stTresholdLevel );
		}
		break;

		case AAD_IORW_SIF_GET_BAND_DETECT:
		{
			LX_AAD_SIF_SYSTEM_INFO_T	io_stSystemInfo;

			AAD_TRACE("AAD_IORW_SIF_GET_BAND_DETECT...\n");

			if ( copy_from_user( &io_stSystemInfo, (void __user *)arg, sizeof(LX_AAD_SIF_SYSTEM_INFO_T) ) )
				return RET_ERROR;

			ret = AAD_SIF_GetBandDetect( &io_stSystemInfo );

			if (copy_to_user((void *)arg, (void *)&io_stSystemInfo, sizeof(LX_AAD_SIF_SYSTEM_INFO_T)) )
				return RET_ERROR;
		}
		break;

		case AAD_IORW_SIF_CHECK_SOUND_MODE:
		{
			LX_AAD_SIF_STANDARD_INFO_T	io_stStandardInfo;

			AAD_TRACE("AAD_IORW_SIF_CHECK_SOUND_MODE...\n");

			if ( copy_from_user( &io_stStandardInfo, (void __user *)arg, sizeof(LX_AAD_SIF_STANDARD_INFO_T) ) )
				return RET_ERROR;

			ret = AAD_SIF_CheckSoundMode( &io_stStandardInfo );

			if (copy_to_user((void *)arg, (void *)&io_stStandardInfo, sizeof(LX_AAD_SIF_STANDARD_INFO_T)))
				return RET_ERROR;
		}
		break;

		case AAD_IOR_SIF_GET_THRESHOLD_LEVEL:
		{
			LX_AAD_SIF_THRESHOLD_T	io_stTresholdLevel;

			AAD_TRACE("AAD_IOR_SIF_GET_THRESHOLD_LEVEL...\n");

			if ( copy_from_user( &io_stTresholdLevel, (void __user *)arg, sizeof(LX_AAD_SIF_THRESHOLD_T ) ) )
				return RET_ERROR;

			ret = AAD_SIF_GetThresholdLevel( &io_stTresholdLevel );

			if (copy_to_user((void *)arg, (void *)&io_stTresholdLevel, sizeof(LX_AAD_SIF_THRESHOLD_T)))
				return RET_ERROR;
		}
		break;

		case AAD_IOR_SIF_GET_CUR_ANALOG_MODE:
		{
			LX_ATV_AUDIO_MODE_GET_T eAudioMode;

			AAD_TRACE("AAD_IOR_SIF_GET_CUR_ANALOG_MODE...\n");

			ret = AAD_SIF_GetCurAnalogMode( &eAudioMode );

			if (copy_to_user((void *)arg, (void *)&eAudioMode, sizeof(LX_ATV_AUDIO_MODE_GET_T)))
				return RET_ERROR;
		}
		break;

		case AAD_IOR_SIF_GET_SIF_STATUS:
		{
			BOOLEAN 	bSIFStatus;

			AAD_TRACE("AAD_IOR_SIF_GET_SIF_STATUS...\n");

			ret = AAD_SIF_GetSIFSatus( &bSIFStatus );

			if ( copy_to_user((void *)arg, (void *)&bSIFStatus, sizeof(BOOLEAN)) )
				return RET_ERROR;
		}
		break;

		case AAD_IOW_SIF_SET_MONITOR_TASK:
		{
			BOOLEAN 	bTaskStatus;

			AAD_TRACE("AAD_IOW_SIF_SET_MONITOR_TASK...\n");

			if ( copy_from_user( &bTaskStatus, (void __user *)arg, sizeof(BOOLEAN ) ) )
				return RET_ERROR;

			ret = AAD_SIF_SetTaskSatus( bTaskStatus );
		}
		break;

		case AAD_IOW_SIF_SET_AAD_SIF_INPUT:
		{
			LX_AAD_SIF_INPUT_T sifSource;

			AAD_TRACE("AAD_IOW_SIF_SET_AAD_SIF_INPUT...\n");

			if(copy_from_user(&sifSource, (void __user *)arg, sizeof(LX_AAD_SIF_INPUT_T)))
				return RET_ERROR;

			ret = AAD_SIF_SetAADSIFInputPort(sifSource);
		}
		break;


		case AAD_IORW_SIF_READ_AND_WRITE_REG:
		{
			LX_AAD_REG_INFO_T	io_stRegInfo = { 0 };

			memset(&io_stRegInfo, 0x0, sizeof(LX_AAD_REG_INFO_T));

			AAD_TRACE("AAD_IORW_SIF_READ_AND_WRITE_REG...\n");

			if ( copy_from_user( &io_stRegInfo, (void __user *)arg, sizeof(LX_AAD_REG_INFO_T) ) )
				return RET_ERROR;

			ret = AAD_SIF_ReadAndWriteReg( &io_stRegInfo );

			if (copy_to_user((void *)arg, (void *)&io_stRegInfo, sizeof(LX_AAD_REG_INFO_T)))
				return RET_ERROR;
		}
		break;

 		case AAD_IOW_DEBUG_PRINT_CTRL:
		{
			LX_AAD_DEBUG_CTRL_T	i_stDebugCtrl;

			memset(&i_stDebugCtrl, 0x0, sizeof(LX_AAD_DEBUG_CTRL_T));

			AAD_TRACE("AAD_IOW_DEBUG_PRINT_CTRL\n");

			if(copy_from_user( &i_stDebugCtrl, (void __user *)arg, sizeof(LX_AAD_DEBUG_CTRL_T)))
					return RET_ERROR;

			if( (i_stDebugCtrl.printType >= NUM_OF_AAD_PRINT_TYPE) || (i_stDebugCtrl.printColor >= NUM_OF_AAD_PRINT_COLOR) )
			{
				AAD_ERROR("Check Argument\n" );
				return RET_ERROR;
			}

			if ( i_stDebugCtrl.printColor == DBG_COLOR_NONE)
			{
				OS_DEBUG_DisableModuleByIndex(g_aad_debug_fd, i_stDebugCtrl.printType);
				AAD_WARN("AAD_PRINT_TYPE[%s] is disabled.\n", ui8AadPrintType[i_stDebugCtrl.printType]);
			}
			else
			{
				OS_DEBUG_EnableModuleByIndex(g_aad_debug_fd, i_stDebugCtrl.printType, i_stDebugCtrl.printColor);
				AAD_WARN("AAD_PRINT_TYPE[%s] is enabled Color is [%s].\n", ui8AadPrintType[i_stDebugCtrl.printType], ui8AadPrintColor[i_stDebugCtrl.printColor - 1]);
			}

			return RET_OK;
 		}
		break;


	    default:
	    {
			/* redundant check but it seems more readable */
			AAD_ERROR("Invalid IOCTL Call!!!\n");
    	    ret = RET_INVALID_IOCTL;
		}
    }

    return ret;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef KDRV_GLOBAL_LINK
#if defined(CONFIG_LG_BUILTIN_KDRIVER) && defined(CONFIG_LGSNAP)
user_initcall_grp("kdrv",AAD_Init);
#else
module_init(AAD_Init);
#endif
module_exit(AAD_Cleanup);

MODULE_AUTHOR("LGE");
MODULE_DESCRIPTION("base driver");
MODULE_LICENSE("GPL");
#endif

/** @} */

