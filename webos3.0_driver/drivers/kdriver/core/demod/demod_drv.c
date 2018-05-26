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
 *  main driver implementation for demod device.
 *	demod device will teach you how to make device driver with new platform.
 *
 *  author		jeongpil.yun (jeongpil.yun@lge.com)
 *  version		1.0
 *  date		2009.12.30
 *  note		Additional information.
 *
 *  @addtogroup lg1150_demod
 *	@{
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/
#undef	SUPPORT_DEMOD_DEVICE_READ_WRITE_FOPS

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/version.h>
#include <asm/uaccess.h>

#include <linux/kthread.h>

#ifdef KDRV_CONFIG_PM	// added by SC Jung for quick booting
#include <linux/platform_device.h>
#endif
#include "os_util.h"
//#include "base_dev_cfg.h"
#include "base_device.h"

#include "demod_module.h"

#include "demod_impl.h"
#include "demod_drv.h"

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
 *	main control block for demod device.
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
DEMOD_DEVICE_T;

UINT8 *ui8DemodPrintColor[8] = {
			"COLOR_BLACK",
			"COLOR_RED",
			"COLOR_GREEN",
			"COLOR_YELLOW",
			"COLOR_BLUE",
			"COLOR_PURPLE",
			"COLOR_CYAN",
			"COLOR_GRAY",
};




#ifdef KDRV_CONFIG_PM
typedef struct
{
	// add here extra parameter
	bool	is_suspended;
}DEMOD_DRVDATA_T;


#endif
/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/
extern	void	DEMOD_PROC_Init(void);
extern	void	DEMOD_PROC_Cleanup(void);

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Functions
----------------------------------------------------------------------------------------*/
int		DEMOD_Init(void);
void	DEMOD_Cleanup(void);

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/
int		g_demod_debug_fd;
int 	g_demod_major = DEMOD_MAJOR;
int 	g_demod_minor = DEMOD_MINOR;

int    	g_demod_trace_depth;

LX_DEMOD_CFG_T	g_demod_cfg;
DEMOD_CTX_T	g_demod_ctx_info[DEMOD_MAX_DEVICE];

BOOLEAN _g_print_enb = TRUE;

BOOLEAN _gExecuteHwRest_gbb = FALSE;
BOOLEAN _gExecuteHwRest_abb = FALSE;

BOOLEAN _gIsOpen = FALSE;

struct task_struct 	*g_pstDemodTuningTask = NULL;

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/
static int      DEMOD_Open(struct inode *, struct file *);
static int      DEMOD_Close(struct inode *, struct file *);

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)
static int 		DEMOD_Ioctl (struct inode *, struct file *, unsigned int, unsigned long );
#else
static long		DEMOD_Ioctl (struct file *, unsigned int, unsigned long );
#endif


#ifdef SUPPORT_DEMOD_DEVICE_READ_WRITE_FOPS
static ssize_t  DEMOD_Read(struct file *, char *, size_t, loff_t *);
static ssize_t  DEMOD_Write(struct file *, const char *, size_t, loff_t *);
#endif

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
static OS_SEM_T demod_dev_mtx;


/**
 * main control block for demod device
*/
static DEMOD_DEVICE_T*		g_demod_device;

/**
 * file I/O description for demod device
 *
*/
static struct file_operations g_demod_fops =
{
	.open 	= DEMOD_Open,
	.release= DEMOD_Close,
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)
	.ioctl	= DEMOD_Ioctl,
#else
	.unlocked_ioctl= DEMOD_Ioctl,
#endif
#ifdef SUPPORT_DEMOD_DEVICE_READ_WRITE_FOPS
	.read 	= DEMOD_Read,
	.write 	= DEMOD_Write,
#else
	.read	= NULL,
	.write	= NULL,
#endif
};

/** lock demod device */
void            DEMOD_LockDevice              (void)
{
	OS_LockMutex( &demod_dev_mtx );
}

/** unlock demod device */
void            DEMOD_UnlockDevice            (void)
{
	OS_UnlockMutex( &demod_dev_mtx );
}

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
static int DEMOD_suspend(struct platform_device *pdev, pm_message_t state)
{
#ifdef KDRV_CONFIG_PM_DEMOD
	DEMOD_DRVDATA_T *drv_data;

	drv_data = platform_get_drvdata(pdev);

	drv_data->is_suspended = 1;
	DEMOD_PRINT("[%s] done suspend\n", DEMOD_MODULE);
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
static int DEMOD_resume(struct platform_device *pdev)
{
#ifdef KDRV_CONFIG_PM_DEMOD
	DEMOD_DRVDATA_T *drv_data;

	Handle_t phDevice;
	UINT32 chipDevID;
	UINT32 handleI2CAbb;
	LX_DEMOD_RF_MODE_T rfMode;

	DEMOD_CTX_T *pDemodInfo;

	drv_data = platform_get_drvdata(pdev);

	if(drv_data->is_suspended == 0)
	{
		DEMOD_PRINT("DEMOD - resume FAIL\n");
		return -1;
	}
	// add here the resume code

	//DEMOD_Init();
	DEMOD_InitHW();

	if( !LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_M14) )
	{
		g_demod_ctx_info[0].bIsAbbMode = TRUE;
		if(_gIsOpen == TRUE)
		{
			DEMOD_ANALOG_ResetHW();

			OS_MsecSleep(10);

			pDemodInfo = &g_demod_ctx_info[0];

			chipDevID = LX_ABB_M14_DEVICE + 0x800;

			pDemodInfo->handleATV.portI2C = 0x7;

			handleI2CAbb = (UINT32)(pDemodInfo->handleATV.portI2C) << 24;
			handleI2CAbb = handleI2CAbb + 0x10;

			DEMOD_ANALOG_Demod_Open(chipDevID, handleI2CAbb, &phDevice);

			pDemodInfo->setCfgParam.analog.handleABB = phDevice;
			pDemodInfo->bIsAbbMode = TRUE;

			rfMode = pDemodInfo->setCfgParam.analog.rfMode;

			DEMOD_ANALOG_Init(phDevice, rfMode);

		}

	}

	drv_data->is_suspended = 0;
	DEMOD_PRINT("DEMOD - resume OK\n");
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
static int __init DEMOD_probe(struct platform_device *pdev)
{

	DEMOD_DRVDATA_T *drv_data;

	drv_data = (DEMOD_DRVDATA_T *)kmalloc(sizeof(DEMOD_DRVDATA_T) , GFP_KERNEL);


	// add here driver registering code & allocating resource code



	DEMOD_PRINT("[%s] done probe\n", DEMOD_MODULE);
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
static int  DEMOD_remove(struct platform_device *pdev)
{
	DEMOD_DRVDATA_T *drv_data;

	// add here driver unregistering code & deallocating resource code



	drv_data = platform_get_drvdata(pdev);
	kfree(drv_data);

	DEMOD_PRINT("released\n");

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
static void  DEMOD_release(struct device *dev)
{


	DEMOD_PRINT("device released\n");
}

/*
 *	module platform driver structure
 */
static struct platform_driver demod_driver =
{
	.probe          = DEMOD_probe,
	.suspend        = DEMOD_suspend,
	.remove         = DEMOD_remove,
	.resume         = DEMOD_resume,
	.driver         =
	{
		.name   = DEMOD_MODULE,
	},
};

static struct platform_device demod_device = {
	.name = DEMOD_MODULE,
	.id = 0,
	.id = -1,
	.dev = {
		.release = DEMOD_release,
	},
};
#endif
/** Initialize the device environment before the real H/W initialization
 *
 *  @note main usage of this function is to initialize the HAL layer and memory size adjustment
 *  @note it's natural to keep this function blank :)
 */
void DEMOD_PreInit(void)
{
    /* TODO: do something */
}


int DEMOD_Init(void)
{
	int			i;
	int			err;
	dev_t		dev;

	/* Get the handle of debug output for demod device.
	 *
	 * Most module should open debug handle before the real initialization of module.
	 * As you know, debug_util offers 4 independent debug outputs for your device driver.
	 * So if you want to use all the debug outputs, you should initialize each debug output
	 * using OS_DEBUG_EnableModuleByIndex() function.
	 */
	g_demod_debug_fd = DBG_OPEN( DEMOD_MODULE );
	if(g_demod_debug_fd < 0) return -1;
	//OS_DEBUG_EnableModule ( g_demod_debug_fd );//if enable LOGM, this is not invalid.

// 	OS_DEBUG_EnableModuleByIndex ( g_demod_debug_fd, LX_LOGM_LEVEL_INFO, DBG_COLOR_NONE );
// 	OS_DEBUG_EnableModuleByIndex ( g_demod_debug_fd, LX_LOGM_LEVEL_TRACE, DBG_COLOR_NONE );
	OS_DEBUG_EnableModuleByIndex ( g_demod_debug_fd, LX_LOGM_LEVEL_WARNING, DBG_COLOR_NONE );
	OS_DEBUG_EnableModuleByIndex ( g_demod_debug_fd, LX_LOGM_LEVEL_ERROR, DBG_COLOR_NONE );
	OS_DEBUG_EnableModuleByIndex ( g_demod_debug_fd, LX_LOGM_LEVEL_ALWAYS, DBG_COLOR_NONE );

	//OS_DEBUG_EnableModuleByIndex ( g_demod_debug_fd, LX_LOGM_LEVEL_NOTI, DBG_COLOR_NONE );

	_g_print_enb = TRUE;


	/* allocate main device handler, register current device.
	 *
	 * If devie major is predefined then register device using that number.
	 * otherwise, major number of device is automatically assigned by Linux kernel.
	 *
	 */
#ifdef KDRV_CONFIG_PM
	// added by SC Jung for quick booting
	if(platform_driver_register(&demod_driver) < 0)
	{
		DEMOD_PRINT("[%s] platform driver register failed\n",DEMOD_MODULE);

	}
	else
	{
		if(platform_device_register(&demod_device))
		{
			platform_driver_unregister(&demod_driver);
			DEMOD_PRINT("[%s] platform device register failed\n",DEMOD_MODULE);
		}
		else
		{
			DEMOD_PRINT("[%s] platform register done\n", DEMOD_MODULE);
		}


	}
#endif

	g_demod_device = (DEMOD_DEVICE_T*)OS_KMalloc( sizeof(DEMOD_DEVICE_T)*DEMOD_MAX_DEVICE );

	if ( NULL == g_demod_device )
	{
		DBG_PRINT_ERROR("out of memory. can't allocate %d bytes\n", sizeof(DEMOD_DEVICE_T)* DEMOD_MAX_DEVICE );
		return -ENOMEM;
	}

	memset( g_demod_device, 0x0, sizeof(DEMOD_DEVICE_T)* DEMOD_MAX_DEVICE );

	if (g_demod_major)
	{
		dev = MKDEV( g_demod_major, g_demod_minor );
		err = register_chrdev_region(dev, DEMOD_MAX_DEVICE, DEMOD_MODULE );
	}
	else
	{
		err = alloc_chrdev_region(&dev, g_demod_minor, DEMOD_MAX_DEVICE, DEMOD_MODULE );
		g_demod_major = MAJOR(dev);
	}

	if ( err < 0 )
	{
		DBG_PRINT_ERROR("can't register demod device\n" );
		return -EIO;
	}

	memset( &g_demod_ctx_info, 0x0, sizeof(DEMOD_CTX_T)* DEMOD_MAX_DEVICE );


	/* TODO : initialize your module not specific minor device */
	DEMOD_InitHW();

	if( !LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_M14) )
	{
		g_demod_ctx_info[0].bIsAbbMode = TRUE;
	}


	/* END */

	for ( i=0; i<DEMOD_MAX_DEVICE; i++ )
	{
		/* initialize cdev structure with predefined variable */
		dev = MKDEV( g_demod_major, g_demod_minor+i );
		cdev_init( &(g_demod_device[i].cdev), &g_demod_fops );
		g_demod_device[i].devno		= dev;
		g_demod_device[i].cdev.owner = THIS_MODULE;
		g_demod_device[i].cdev.ops   = &g_demod_fops;

		/* TODO: initialize minor device */


		/* END */

		err = cdev_add (&(g_demod_device[i].cdev), dev, 1 );

		if (err)
		{
			DBG_PRINT_ERROR("error (%d) while adding demod device (%d.%d)\n", err, MAJOR(dev), MINOR(dev) );
			return -EIO;
		}
        OS_CreateDeviceClass ( g_demod_device[i].devno, "%s%d", DEMOD_MODULE, i );
	}

	/* initialize proc system */
	DEMOD_PROC_Init ( );

	DEMOD_PRINT("demod device initialized\n");

	return 0;
}

void DEMOD_Cleanup(void)
{
	int i;
	dev_t dev = MKDEV( g_demod_major, g_demod_minor );

#ifdef KDRV_CONFIG_PM
	// added by SC Jung for quick booting
	platform_driver_unregister(&demod_driver);
	platform_device_unregister(&demod_device);
#endif

	/* cleanup proc system */
	DEMOD_PROC_Cleanup( );

	/* remove all minor devicies and unregister current device */
	for ( i=0; i<DEMOD_MAX_DEVICE;i++)
	{
		/* TODO: cleanup each minor device */


		/* END */
		cdev_del( &(g_demod_device[i].cdev) );
	}

	/* TODO : cleanup your module not specific minor device */

	unregister_chrdev_region(dev, DEMOD_MAX_DEVICE );

	OS_Free( g_demod_device );
}


///////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * open handler for demod device
 *
 */
static int
DEMOD_Open(struct inode *inode, struct file *filp)
{
    int					major,minor;
    struct cdev*    	cdev;
    DEMOD_DEVICE_T*	my_dev;

    cdev	= inode->i_cdev;
    my_dev	= container_of ( cdev, DEMOD_DEVICE_T, cdev);

    DEMOD_PRINT("demod device opened start\n" );
    /* TODO : add your device specific code */
	_gIsOpen = FALSE;

	if( my_dev->dev_open_count == 0 )
	{

		 /* init H/W */
		 OS_InitMutex(&demod_dev_mtx, OS_SEM_ATTR_DEFAULT);

		 //Create thread for DEMOD module
		 g_pstDemodTuningTask = kthread_run( DEMOD_Tuning_Task, NULL, "DEMODTASK" );

		 if ( g_pstDemodTuningTask )
		 {
			 DEMOD_PRINT("DEMOD> Init - Monitor task created successfully\n");
		 }
		 else
		 {
			 DEMOD_ERROR("ERROR> ADEC Monitor task creation failed!!!\n");
		 }

	}


	/* END */

    my_dev->dev_open_count++;
    filp->private_data = my_dev;



	/* some debug */
    major = imajor(inode);
    minor = iminor(inode);
    DEMOD_PRINT("device opened (%d:%d)\n", major, minor );

	_gIsOpen = TRUE;

    return 0;
}

/**
 * release handler for demod device
 *
 */
static int
DEMOD_Close(struct inode *inode, struct file *file)
{
    int					major,minor;
    DEMOD_DEVICE_T*	my_dev;
    struct cdev*		cdev;

    cdev	= inode->i_cdev;
    my_dev	= container_of ( cdev, DEMOD_DEVICE_T, cdev);

    if ( my_dev->dev_open_count > 0 )
    {
        --my_dev->dev_open_count;
    }

    /* TODO : add your device specific code */

	if( my_dev->dev_open_count <= 0 )

	{
		  /* shutdown H/W */

		  //Release monitor thread
		  if ( g_pstDemodTuningTask )
		  {
			  kthread_stop( g_pstDemodTuningTask );
			  DEMOD_PRINT("kthread_stop ADEC Monitor task\n");
		  }

	}

	/* END */

	/* some debug */
    major = imajor(inode);
    minor = iminor(inode);
    DEMOD_PRINT("device closed (%d:%d)\n", major, minor );
    return 0;
}

/**
 * ioctl handler for demod device.
 *
 *
 * note: if you have some critial data, you should protect them using semaphore or spin lock.
 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)
static int DEMOD_Ioctl ( struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg )
#else
static long DEMOD_Ioctl ( struct file *filp, unsigned int cmd, unsigned long arg )
#endif
{
    int err = 0;

    DEMOD_DEVICE_T*	my_dev;
    struct cdev*		cdev;

	DEMOD_CTX_T *pDemodInfo;
	int device_no;


#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)
#else
	struct inode *inode = filp->f_path.dentry->d_inode;
#endif

	/*
	 * get current demod device object
	 */
	cdev	= inode->i_cdev;
	my_dev	= container_of ( cdev, DEMOD_DEVICE_T, cdev);

    device_no = iminor(inode);

    /*
     * check if IOCTL command is valid or not.
     * - if magic value doesn't match, return error (-ENOTTY)
     * - if command is out of range, return error (-ENOTTY)
     *
     * note) -ENOTTY means "Inappropriate ioctl for device.
     */
    if (_IOC_TYPE(cmd) != DEMOD_IOC_MAGIC)
    {
    	DBG_PRINT_WARNING("invalid magic. magic=0x%02X\n", _IOC_TYPE(cmd) );
    	return -ENOTTY;
    }
    if (_IOC_NR(cmd) > DEMOD_IOC_MAXNR)
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

	DEMOD_TRACE("cmd = %08X (cmd_idx=%d)\n", cmd, _IOC_NR(cmd) );
	pDemodInfo = &g_demod_ctx_info[device_no];

	switch(cmd)
	{
		case DEMOD_IO_HW_RESET:
		{
			DEMOD_PRINT("DEMOD_IO_HW_RESET ok\n");
			if(FALSE == _gExecuteHwRest_gbb)
			{
				if(RET_OK != DEMOD_ResetHW())
				{
					DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_ResetHW() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}

				pDemodInfo->bSetParams = FALSE;
				_gExecuteHwRest_gbb = TRUE;
			}
			else
			{
				DEMOD_PRINT("[DEMOD] (%s:%d) DEMOD_ResetHW() has been already called, Nothing to do !!!\n", __F__, __L__);

			}
		}
		break;

		case DEMOD_IOW_I2C_OPEN:
		{
			LX_DEMOD_I2C_DESC_T i2cOpenParam;


			DEMOD_PRINT("DEMOD_IOW_I2C_OPEN \n");

			if ( copy_from_user( &i2cOpenParam, (void *)arg, sizeof(LX_DEMOD_I2C_DESC_T ) ) )
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) copy_from_user() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

			if(RET_OK != DEMOD_I2C_open(i2cOpenParam.portI2C,i2cOpenParam.slvAddr))
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_I2C_open() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

			pDemodInfo->handleDTV.portI2C = i2cOpenParam.portI2C;
			pDemodInfo->handleDTV.slvAddr = i2cOpenParam.slvAddr;

		}
		break;

		case DEMOD_IO_I2C_CLOSE:
		{
			DEMOD_PRINT("DEMOD_IO_I2C_CLOSE ok\n");

			if(RET_OK != DEMOD_I2C_Close())
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_I2C_Close() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

		}
		break;

		case DEMOD_IOW_MEDIA_CHANGE:
		{
			LX_DEMOD_INIT_PARAM_T initParam;


			DEMOD_PRINT("DEMOD_IOW_MEDIA_CHANGE \n");

			if ( copy_from_user( &initParam, (void *)arg, sizeof(LX_DEMOD_INIT_PARAM_T ) ) )
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) copy_from_user() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

			if(RET_OK != DEMOD_MediaChange(pDemodInfo, &initParam))
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_MediaChange() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

		}

		break;

		case DEMOD_IOW_CHANNEL_CHANGE:
		{
			LX_DEMOD_CONFIG_T configParams;


			DEMOD_PRINT("DEMOD_IOW_CHANNEL_CHANGE \n");

			if ( copy_from_user( &configParams, (void *)arg, sizeof(LX_DEMOD_CONFIG_T ) ) )
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) copy_from_user() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

			if(RET_OK != DEMOD_ChannelChange(pDemodInfo,&configParams))
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_ChannelChange() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}


		}
		break;

		case DEMOD_IOW_SERIAL_CONTROL:
		{
			BOOLEAN bEnable;


			DEMOD_PRINT("DEMOD_IOW_SERIAL_CONTROL \n");

			if ( copy_from_user( &bEnable, (void *)arg, sizeof(BOOLEAN ) ) )
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) copy_from_user() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

			if(RET_OK != DEMOD_Serial_Control(bEnable))
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_Serial_Control() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

		}
		break;

		case DEMOD_IOW_SET_IFAGC_POL:
		{
			LX_DEMOD_AGC_POLAR_T agcPola;


			DEMOD_PRINT("DEMOD_IOW_SET_IFAGC_POL \n");

			if ( copy_from_user( &agcPola, (void *)arg, sizeof(LX_DEMOD_AGC_POLAR_T ) ) )
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) copy_from_user() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

			if(RET_OK != DEMOD_Set_AGCPolarity(agcPola))
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_Set_AGCPolarity() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

		}
		break;


		case DEMOD_IOR_GET_CHANNEL_INFO:
		{
			LX_DEMOD_CONFIG_T demodTunedCfg;


			DEMOD_PRINT("DEMOD_IOR_GET_CHANNEL_INFO \n");

			memcpy(&demodTunedCfg,&(pDemodInfo->getCfgParam),sizeof(LX_DEMOD_CONFIG_T));

			if ( copy_to_user((void *)arg, (void *)&demodTunedCfg,  sizeof(LX_DEMOD_CONFIG_T ) ) )
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) copy_to_user() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}


		}
		break;


		case DEMOD_IOR_GET_SIGNAL_STATUS:
		{
			LX_DEMOD_SIGNAL_CTX_T sigCtx;


			DEMOD_PRINT("DEMOD_IOR_GET_SIGNAL_STATUS \n");

			if(pDemodInfo->lockStatus != LX_DEMOD_LOCKED )
			{

				sigCtx.bSignalValid	= FALSE;
				sigCtx.packetError	= 0xffffffff;
				sigCtx.ber			= 0xffffffff;
				sigCtx.agc			= 0;
				sigCtx.m.mse		= 0xffff;
				sigCtx.m.bMse 		= 0xffff;
				sigCtx.m.constPwr 	= 0xffff;
			}
			else
			{
				memcpy(&sigCtx,&(pDemodInfo->signalStatus),sizeof(LX_DEMOD_SIGNAL_CTX_T));
			}


			if ( copy_to_user((void *)arg, (void *)&sigCtx,  sizeof(LX_DEMOD_SIGNAL_CTX_T ) ) )
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) copy_to_user() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

		}
		break;

		case DEMOD_IOR_I2C_READ:
		{
			LX_DEMOD_I2C_COMM_T i2cReadParams;
			LX_DEMOD_I2C_COMM_T* in;



			DEMOD_PRINT("DEMOD_IOR_I2C_READ \n");

			in = (LX_DEMOD_I2C_COMM_T*)arg;

			if ( copy_from_user( &i2cReadParams, (void *)in , sizeof(LX_DEMOD_I2C_COMM_T ) ) )
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) copy_from_user() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}


			if(RET_OK != DEMOD_I2C_Read(i2cReadParams.address, i2cReadParams.nBytes, i2cReadParams.buf))
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_I2C_Read() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

			if(i2cReadParams.nBytes <= sizeof(in->buf))
			{
				if ( copy_to_user((void *)in->buf, (void *)&i2cReadParams.buf,	i2cReadParams.nBytes ) )
				{
					DEMOD_PRINT("[DEMOD] FAIL (%s:%d) copy_to_user() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}
			}

		}
		break;

		case DEMOD_IOW_I2C_WRITE:
		{
			LX_DEMOD_I2C_COMM_T i2cWriteParams;

			DEMOD_PRINT("DEMOD_IOW_I2C_WRITE \n");

			if ( copy_from_user( &i2cWriteParams, (void *)arg, sizeof(LX_DEMOD_I2C_COMM_T ) ) )
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) copy_from_user() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}


			if(RET_OK != DEMOD_I2C_Write(i2cWriteParams.address, i2cWriteParams.nBytes, i2cWriteParams.buf))
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_I2C_Write() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

		}
		break;



//Common
		case DEMOD_IOW_REPEATER_ENB:
		{
			BOOLEAN bEnable;


			DEMOD_PRINT("DEMOD_IOW_REPEATER_ENB \n");

			if ( copy_from_user( &bEnable, (void *)arg, sizeof(BOOLEAN ) ) )
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) copy_from_user() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

			if(RET_OK != DEMOD_RepeaterEnable(bEnable))
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_RepeaterEnable() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

		}
		break;


		case DEMOD_IOW_POWER_SAVE:
		{
			BOOLEAN bEnable;


			DEMOD_PRINT("DEMOD_IOW_POWER_SAVE \n");

			if ( copy_from_user( &bEnable, (void *)arg, sizeof(BOOLEAN ) ) )
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) copy_from_user() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

			if(RET_OK != DEMOD_Power_Save(bEnable))
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_Power_Save() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

		}
		break;


		case DEMOD_IOW_TPOUT_ENB:
		{
			BOOLEAN bEnable;


			DEMOD_PRINT("DEMOD_IOW_TPOUT_ENB \n");

			if ( copy_from_user( &bEnable, (void *)arg, sizeof(BOOLEAN ) ) )
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) copy_from_user() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

			if(RET_OK != DEMOD_TPOutCLKEnable(bEnable))
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_TPOutCLKEnable() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}
			pDemodInfo->bIsTPoutEn = bEnable;



		}
		break;


		case DEMOD_IOW_SET_IF_FREQ:
		{
			UINT32 ifFrq;


			DEMOD_PRINT("DEMOD_IOW_SET_IF_FREQ \n");

			if ( copy_from_user( &ifFrq, (void *)arg, sizeof(UINT32 ) ) )
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) copy_from_user() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

			if(RET_OK != DEMOD_Set_IF_Frq(ifFrq))
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_Set_IF_Frq() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

		}
		break;


		case DEMOD_IOW_SET_SPECTRUM_CTRL:
		{
			LX_DEMOD_SPECTRUM_CTRL_T spectrumParam;


			DEMOD_PRINT("DEMOD_IOW_SET_SPECTRUM_CTRL \n");

			if ( copy_from_user( &spectrumParam, (void *)arg, sizeof(LX_DEMOD_SPECTRUM_CTRL_T ) ) )
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) copy_from_user() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

			if(RET_OK != DEMOD_Set_SpectrumCtrl(spectrumParam.bAutoEnable, spectrumParam.bSpecInvEnable))
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_Set_SpectrumCtrl() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

		}
		break;


		case DEMOD_IOR_GET_ID:
		{
			UINT16 id;


			DEMOD_PRINT("DEMOD_IOR_GET_ID \n");


			if(RET_OK != DEMOD_Get_Id(&id))
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_Get_Id() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

			if ( copy_to_user((void *)arg, (void *)&id,  sizeof(UINT16 ) ) )
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) copy_to_user() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

		}
		break;

		case DEMOD_IOR_GET_SPECTRUM_CTRL:
		{
			LX_DEMOD_SPECTRUM_CTRL_T getSpectrumParams;
			LX_DEMOD_LOCK_STATE_T spectrumAuto;
			LX_DEMOD_LOCK_STATE_T spectrumInv;


			DEMOD_PRINT("DEMOD_Get_SpectrumStatus \n");


			if(RET_OK != DEMOD_Get_SpectrumStatus(&spectrumAuto, &spectrumInv))
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_Get_Id() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

			if(LX_DEMOD_AUTO == spectrumAuto) getSpectrumParams.bAutoEnable = TRUE;
			else getSpectrumParams.bAutoEnable = FALSE;

			if(LX_DEMOD_INVERSION == spectrumInv) getSpectrumParams.bSpecInvEnable = TRUE;
			else getSpectrumParams.bSpecInvEnable = FALSE;

			if ( copy_to_user((void *)arg, (void *)&getSpectrumParams,  sizeof(LX_DEMOD_SPECTRUM_CTRL_T ) ) )
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) copy_to_user() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

		}
		break;

		case DEMOD_IOR_GET_IFAGC:
		{
			UINT16 ifAgc;


			DEMOD_PRINT("DEMOD_IOR_GET_IFAGC \n");

			if(pDemodInfo->lockStatus != LX_DEMOD_LOCKED )
			{
				ifAgc = 0;
			}
			else
			{
				ifAgc = pDemodInfo->signalStatus.agc;
			}

			if ( copy_to_user((void *)arg, (void *)&ifAgc,  sizeof(UINT16 ) ) )
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) copy_to_user() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

		}
		break;

		case DEMOD_IOR_GET_OPERATION_MODE:
		{
			LX_DEMOD_OPER_MODE_T operMode;


			DEMOD_PRINT("DEMOD_IOR_GET_OPERATION_MODE \n");


			if(RET_OK != DEMOD_Get_OperMode(&operMode))
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_Get_OperMode() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

			if ( copy_to_user((void *)arg, (void *)&operMode,  sizeof(LX_DEMOD_OPER_MODE_T ) ) )
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) copy_to_user() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

		}
		break;

		case DEMOD_IOR_GET_LOCK_STATUS_NOTIFY:
		{
			BOOLEAN notifiedstatus;

			DEMOD_PRINT("DEMOD_IOR_GET_LOCK_STATUS_NOTIFY \n");

			if((TRUE == pDemodInfo->bNotifiedLock) || (TRUE == pDemodInfo->bNotifiedUnlock))
			{
				notifiedstatus = TRUE;
				DEMOD_PRINT("notifiedstatus = TRUE \n");
			}
			else
			{
				notifiedstatus = FALSE;
				DEMOD_PRINT("notifiedstatus = FALSE \n");

			}


			if(pDemodInfo->operMode == LX_DEMOD_ANALOG)
			{
				if(pDemodInfo->getCfgParam.analog.frequency != pDemodInfo->setCfgParam.analog.frequency)
				{
					notifiedstatus = FALSE;

					pDemodInfo->controlState 				= LX_DEMOD_TUNE_START;
					pDemodInfo->acqState					= LX_DEMOD_TUNNING_PROCESS;
					pDemodInfo->scanJobFlag					= LX_DEMOD_FLAG_START;
		   			pDemodInfo->lockStatus 					= LX_DEMOD_UNLOCKED;

					pDemodInfo->bNotifiedLock				= FALSE;
					pDemodInfo->bNotifiedUnlock 			= FALSE;

					DEMOD_PRINT("notifiedstatus = FALSE \n");
				}

			}

			if ( copy_to_user((void *)arg, (void *)&notifiedstatus,  sizeof(BOOLEAN ) ) )
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) copy_to_user() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

		}
		break;

		case DEMOD_IOR_GET_OFFSETFRQ:
		{
			SINT32 freqOff;


			DEMOD_PRINT("DEMOD_IOR_GET_OFFSETFRQ \n");

			freqOff = pDemodInfo->freqOffset;

			if ( copy_to_user((void *)arg, (void *)&freqOff,  sizeof(SINT32 ) ) )
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) copy_to_user() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

		}
		break;

		case DEMOD_IOR_GET_TPIF_ENB:
		{
			BOOLEAN bEnable;


			DEMOD_PRINT("DEMOD_IOR_GET_TPIF_ENB \n");


			if(RET_OK != DEMOD_Get_TPIFStatus(&bEnable))
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_Get_TPIFStatus() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

			if ( copy_to_user((void *)arg, (void *)&bEnable,  sizeof(BOOLEAN ) ) )
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) copy_to_user() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

		}
		break;

		case DEMOD_IOR_GET_VABER:
		{
			UINT32 vBer;


			DEMOD_PRINT("DEMOD_IOR_GET_VABER \n");

			if(pDemodInfo->lockStatus != LX_DEMOD_LOCKED )
			{
				vBer = 0xffffffff;
			}
			else
			{
				vBer = pDemodInfo->signalStatus.ber;
			}

			if ( copy_to_user((void *)arg, (void *)&vBer,  sizeof(UINT32 ) ) )
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) copy_to_user() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

		}
		break;

		case DEMOD_IOR_GET_MSE_INFO:
		{
			LX_DEMOD_MSE_T sigMse;


			DEMOD_PRINT("DEMOD_IOR_GET_MSE_INFO \n");

			if(pDemodInfo->lockStatus != LX_DEMOD_LOCKED )
			{
				sigMse.mse			= 0xffff;
				sigMse.bMse 		= 0xffff;
				sigMse.constPwr 	= 0xffff;
			}
			else
			{
				memcpy(&sigMse,&(pDemodInfo->signalStatus.m),sizeof(LX_DEMOD_MSE_T));
			}

			if ( copy_to_user((void *)arg, (void *)&sigMse,  sizeof(LX_DEMOD_MSE_T ) ) )
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) copy_to_user() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

		}
		break;

		case DEMOD_IOR_GET_PKERROR:
		{
			UINT32 pkError;


			DEMOD_PRINT("DEMOD_IOR_GET_PKERROR \n");

			if(pDemodInfo->lockStatus != LX_DEMOD_LOCKED )
			{
				pkError = 0xffffffff;
			}
			else
			{
				pkError = pDemodInfo->signalStatus.packetError;
			}

			if ( copy_to_user((void *)arg, (void *)&pkError,  sizeof(UINT32 ) ) )
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) copy_to_user() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

		}
		break;


		case DEMOD_IOR_GET_BANDWIDTH:
		{
			LX_DEMOD_BWMODE_T channelBW;


			DEMOD_PRINT("DEMOD_IOR_GET_BANDWIDTH \n");


			if(RET_OK != DEMOD_Get_BandWidthMode(&channelBW))
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_Get_BandWidthMode() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

			if ( copy_to_user((void *)arg, (void *)&channelBW,  sizeof(LX_DEMOD_BWMODE_T ) ) )
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) copy_to_user() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

		}
		break;

		case DEMOD_IOR_GET_QAM_MODE:
		{
			LX_DEMOD_RF_MODE_T qamMode ;


			DEMOD_PRINT("DEMOD_IOR_GET_QAM_MODE \n");


			if(RET_OK != DEMOD_Get_QAMMode(&qamMode))
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_Get_QAMMode() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

			if ( copy_to_user((void *)arg, (void *)&qamMode,  sizeof(LX_DEMOD_RF_MODE_T ) ) )
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) copy_to_user() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

		}
		break;

		case DEMOD_IOR_GET_LOCK:
		{

			LX_DEMOD_LOCK_STATE_T 	lockState;

//			DEMOD_PRINT("DEMOD_IOWR_GET_LOCK \n");

			if(TRUE == pDemodInfo->bNotifiedLock)
			{
				lockState = LX_DEMOD_LOCKED;
				DEMOD_PRINT("[DEMOD] (%s:%d) lockState = LX_DEMOD_LOCKED !!!\n", __F__, __L__);
			}
			else
			{
				lockState = LX_DEMOD_UNLOCKED;
				DEMOD_PRINT("[DEMOD] (%s:%d) lockState = LX_DEMOD_UNLOCKED !!!\n", __F__, __L__);
			}
			if ( copy_to_user((void *)arg, (void *)&lockState,  sizeof(LX_DEMOD_LOCK_STATE_T ) ) )
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) copy_to_user() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

		}
		break;

		case DEMOD_IOR_GET_SYSLOCKTIME:
		{
			UINT16	syslocktime;

			DEMOD_PRINT("DEMOD_IOR_GET_SYSLOCKTIME \n");

			if(RET_OK != DEMOD_Get_SysLockTime(&syslocktime))
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_Get_SysLockTime() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

			if ( copy_to_user( (void *)arg, (void *)&syslocktime,  sizeof(UINT16) ) )
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) copy_to_user() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

		}
		break;

		case DEMOD_IOR_GET_FECLOCKTIME:
		{
			UINT16	feclocktime;

			DEMOD_PRINT("DEMOD_IOR_GET_FECLOCKTIME \n");

			if(RET_OK != DEMOD_Get_FecLockTime(&feclocktime))
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_Get_FecLockTime() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

			if ( copy_to_user( (void *)arg, (void *)&feclocktime,  sizeof(UINT16) ) )
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) copy_to_user() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

		}
		break;


		case DEMOD_IO_SOFT_RESET:
		{
			DEMOD_PRINT("DEMOD_IO_SOFT_RESET ok\n");

			if(RET_OK != DEMOD_SoftwareReset())
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_SoftwareReset() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

		}
		break;

		case DEMOD_IOW_MONITOR_ENB:
		{
			BOOLEAN bEnable;


			DEMOD_PRINT("DEMOD_IOW_MONITOR_ENB \n");

			if ( copy_from_user( &bEnable, (void *)arg, sizeof(BOOLEAN ) ) )
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) copy_from_user() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

			pDemodInfo->bMonitorEnable = bEnable;

		}
		break;

		case DEMOD_IOW_ADC_CTRL_FOR_TEST:
		{
			LX_DEMOD_ADC_VOLTAGE_SEL_T voltageSel;


			DEMOD_PRINT("DEMOD_IOW_ADC_CTRL_FOR_TEST \n");

			if ( copy_from_user( &voltageSel, (void *)arg, sizeof(LX_DEMOD_ADC_VOLTAGE_SEL_T ) ) )
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) copy_from_user() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

			if(RET_OK != DEMOD_ADC_Control(voltageSel))
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_ADC_Control() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

		}
		break;

		case DEMOD_IOW_PRINT_ON_OFF:
		{
			UINT32  printColor = 0;


			DEMOD_PRINT("DEMOD_IOW_PRINT_ON_OFF \n");

			if(copy_from_user( &printColor, (void __user *)arg, sizeof(UINT32)))
					return RET_ERROR;


			if( (printColor >= 9) )
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) Print color is invalid !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

			if ( printColor == DBG_COLOR_NONE)
			{
				OS_DEBUG_DisableModuleByIndex( g_demod_debug_fd, 0);
				_g_print_enb = FALSE;

				DEMOD_ERROR("DEMOD_PRINT is disabled.\n");
			}
			else
			{
				OS_DEBUG_EnableModuleByIndex ( g_demod_debug_fd, 0, DBG_COLOR_NONE );
				_g_print_enb = TRUE;

				DEMOD_PRINT("DEMOD_PRINT is enabled Color is [%s].\n", ui8DemodPrintColor[printColor-1]);
			}

			return RET_OK;

		}
		break;



//DVBC

		case DEMOD_IOW_DVBC_SET_SYM_RATE:
		{
			LX_DEMOD_SYMBOL_CTX_T symbolRateInfo;


			DEMOD_PRINT("DEMOD_IOW_DVBC_SET_SYM_RATE \n");

			if ( copy_from_user( &symbolRateInfo, (void *)arg, sizeof(LX_DEMOD_SYMBOL_CTX_T ) ) )
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) copy_from_user() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

			if(RET_OK != DEMOD_DVBC_AutoSymbolRateDet(symbolRateInfo.symbolMode, symbolRateInfo.symbolRate))
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_DVBC_AutoSymbolRateDet() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

		}
		break;


		case DEMOD_IOW_DVBC_SET_CONFIG_AUTO:
		{
			BOOLEAN bEnable;


			DEMOD_PRINT("DEMOD_IOW_DVBC_SET_CONFIG_AUTO \n");

			if ( copy_from_user( &bEnable, (void *)arg, sizeof(BOOLEAN ) ) )
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) copy_from_user() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

			if(RET_OK != DEMOD_DVBC_Set_Config_auto(bEnable))
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_DVBC_Set_Config_auto() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

		}
		break;


		case DEMOD_IOW_DVBC_SET_QAM_MODE:
		{
			LX_DEMOD_RF_MODE_T constel;


			DEMOD_PRINT("DEMOD_IOW_DVBC_SET_QAM_MODE \n");

			if ( copy_from_user( &constel, (void *)arg, sizeof(LX_DEMOD_RF_MODE_T ) ) )
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) copy_from_user() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

			if(RET_OK != DEMOD_DVBC_Set_QamMode(constel))
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_DVBC_Set_QamMode() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

		}
		break;


		case DEMOD_IOW_DVBC_SET_NEVERLOCK_TIME:
		{
			UINT32 waitMs;


			DEMOD_PRINT("DEMOD_IOW_DVBC_SET_NEVERLOCK_TIME \n");

			if ( copy_from_user( &waitMs, (void *)arg, sizeof(UINT32 ) ) )
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) copy_from_user() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

			if(RET_OK != DEMOD_DVBC_Set_NeverLockWaitTime(waitMs))
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_DVBC_Set_NeverLockWaitTime() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

		}
		break;

		case DEMOD_IOR_DVBC_GET_SYM_STATUS:
		{
			LX_DEMOD_SYMBOLRATE_STATE_T	symbolParam ;

			DEMOD_PRINT("DEMOD_IOR_DVBC_GET_SYM_STATUS \n");


			if(RET_OK != DEMOD_DVBC_Get_SymbolRateStatus(&symbolParam ))
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_DVBC_Get_SymbolRateStatus() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}


			if ( copy_to_user((void *)arg, (void *)&symbolParam,  sizeof(LX_DEMOD_SYMBOLRATE_STATE_T ) ) )
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) copy_to_user() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

		}
		break;

		case DEMOD_IOR_DVBC_GET_QAM_STATUS:
		{
			LX_DEMOD_QAMMODE_STATE_T	qamModeDet ;

			DEMOD_PRINT("DEMOD_IOR_DVBC_GET_SYM_STATUS \n");


			if(RET_OK != DEMOD_DVBC_Get_QamModeDetectStatus(&qamModeDet ))
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_DVBC_Get_QamModeDetectStatus() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}


			if ( copy_to_user((void *)arg, (void *)&qamModeDet,  sizeof(LX_DEMOD_QAMMODE_STATE_T ) ) )
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) copy_to_user() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

		}
		break;

		case DEMOD_IO_DVBC_PRINT_INFO:
		{
			DEMOD_PRINT("DEMOD_IO_DVBC_PRINT_INFO ok\n");
			if(FALSE == _g_print_enb)
				OS_DEBUG_EnableModuleByIndex ( g_demod_debug_fd, 0, DBG_COLOR_NONE );

			if(RET_OK != DEMOD_DVBC_Get_DvbInfo())
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_DVBC_Get_DvbInfo() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

			if(FALSE == _g_print_enb)
				OS_DEBUG_DisableModuleByIndex( g_demod_debug_fd, 0);

		}
		break;

		case DEMOD_IO_DVBC_SET_DEFAULT_VALUE:
		{
			DEMOD_PRINT("DEMOD_IO_DVBC_SET_DEFAULT_VALUE \n");

			if(RET_OK != DEMOD_DVBC_Set_DefaultRegisterValue())
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_DVBC_Set_DefaultRegisterValue() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

		}
		break;

//DVBC2
		case DEMOD_IOW_DVBC2_SET_CONFIG_PARTIAL:
		{
			LX_DEMOD_DVBC2_CONFIG_T dvbc2ConfigPartial;


			DEMOD_PRINT("DEMOD_IOW_DVBC2_SET_CONFIG_PARTIAL \n");

			if ( copy_from_user( &dvbc2ConfigPartial, (void *)arg, sizeof(LX_DEMOD_DVBC2_CONFIG_T ) ) )
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) copy_from_user() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

			if(RET_OK != DEMOD_DVBC2_Set_PartialConfig(&dvbc2ConfigPartial))
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_DVBC2_Set_PartialConfig() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

		}
		break;


		case DEMOD_IOR_DVBC2_GET_MULTIPLP_INFO:
		{
			LX_DEMOD_DVBC2_MULTI_PLP_INFO_T paramMultiPLPInfo;


			DEMOD_PRINT("DEMOD_IOW_DVBC2_GET_MULTIPLP_INFO \n");

			memset( &paramMultiPLPInfo, 0x0, sizeof(LX_DEMOD_DVBC2_MULTI_PLP_INFO_T) );

			if(RET_OK != DEMOD_DVBC2_Get_multiPLP_ID(&paramMultiPLPInfo))
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_DVBC2_GetMultiPLPInfo() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

			if ( copy_to_user((void *)arg, (void *)&paramMultiPLPInfo,  sizeof(LX_DEMOD_DVBC2_MULTI_PLP_INFO_T ) ) )
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) copy_to_user() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}


		}
		break;

		case DEMOD_IO_DVBC2_PRINT_INFO:
		{
			DEMOD_PRINT("DEMOD_IO_DVBC2_PRINT_INFO ok\n");

			if(FALSE == _g_print_enb)
				OS_DEBUG_EnableModuleByIndex ( g_demod_debug_fd, 0, DBG_COLOR_NONE );

			if(RET_OK != DEMOD_DVBC2_Get_DvbInfo())
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_DVBC2_Get_TotalInfo() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}
			if(FALSE == _g_print_enb)
				OS_DEBUG_DisableModuleByIndex( g_demod_debug_fd, 0);

		}
		break;



//DVBT

		case DEMOD_IO_DVBT_SET_DEFAULT_VALUE:
		{
			LX_DEMOD_BWMODE_T band;

			band = pDemodInfo->setCfgParam.dvbt.channelBW;

			DEMOD_PRINT("DEMOD_IO_DVBT_SET_DEFAULT_VALUE \n");

			if(RET_OK != DEMOD_DVBT_Set_DefaultRegisterValue(band))
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_DVBT_Set_DefaultRegisterValue() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

		}
		break;

		case DEMOD_IOW_DVBT_SET_CONFIG_AUTO:
		{
			BOOLEAN bEnable;


			DEMOD_PRINT("DEMOD_IOW_DVBT_SET_CONFIG_AUTO \n");

			if ( copy_from_user( &bEnable, (void *)arg, sizeof(BOOLEAN ) ) )
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) copy_from_user() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

			if(RET_OK != DEMOD_DVBT_Set_Config_auto(bEnable))
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_DVBT_Set_Config_auto() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

		}
		break;


		case DEMOD_IOW_DVBT_SET_CONFIG_PARTIAL:
		{
			LX_DEMOD_DVBT_CONFIG_T dvbtConfigPartial;


			DEMOD_PRINT("DEMOD_IOW_DVBT_SET_CONFIG_PARTIAL \n");

			if ( copy_from_user( &dvbtConfigPartial, (void *)arg, sizeof(LX_DEMOD_DVBT_CONFIG_T ) ) )
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) copy_from_user() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

			if(RET_OK != DEMOD_DVBT_Set_PartialConfig(&dvbtConfigPartial))
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_DVBT_Set_PartialConfig() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

		}
		break;

		case DEMOD_IOW_DVBT_SET_CONFIG_FULL:
		{
			LX_DEMOD_DVBT_CONFIG_T dvbtConfigFull;


			DEMOD_PRINT("DEMOD_IOW_DVBT_SET_CONFIG_FULL \n");

			if ( copy_from_user( &dvbtConfigFull, (void *)arg, sizeof(LX_DEMOD_DVBT_CONFIG_T ) ) )
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) copy_from_user() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

			if(RET_OK != DEMOD_DVBT_Set_FullConfig(&dvbtConfigFull))
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_DVBT_Set_FullConfig() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

		}
		break;

		case DEMOD_IOW_DVBT_SET_NEVERLOCK_TIME:
		{
			UINT32 waitMs;


			DEMOD_PRINT("DEMOD_IOW_DVBT_SET_NEVERLOCK_TIME \n");

			if ( copy_from_user( &waitMs, (void *)arg, sizeof(UINT32 ) ) )
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) copy_from_user() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

			if(RET_OK != DEMOD_DVBT_Set_NeverLockWaitTime(waitMs))
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_DVBT_Set_NeverLockWaitTime() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

		}
		break;

		case DEMOD_IO_DVBT_PRINT_INFO:
		{
			DEMOD_PRINT("DEMOD_IO_DVBT_PRINT_INFO ok\n");

			if(FALSE == _g_print_enb)
				OS_DEBUG_EnableModuleByIndex ( g_demod_debug_fd, 0, DBG_COLOR_NONE );

			if(RET_OK != DEMOD_DVBT_Get_TotalInfo())
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_DVBT_Get_TotalInfo() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}
			if(FALSE == _g_print_enb)
				OS_DEBUG_DisableModuleByIndex( g_demod_debug_fd, 0);

		}
		break;

		case DEMOD_IOR_DVBT_GET_CELL_ID:
		{
			UINT16 cell_ID;
			UINT8 cellIDLength;


			DEMOD_PRINT("DEMOD_IOR_DVBT_GET_CELL_ID \n");

			if(RET_OK != DEMOD_DVBT_Get_CellId(&cellIDLength, &cell_ID))
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_DVBT_Get_CellId() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

			if ( copy_to_user((void *)arg, (void *)&cell_ID,  sizeof(UINT16 ) ) )
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) copy_to_user() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

		}
		break;

//DVBT2

		case DEMOD_IOW_DVBT2_SET_CONFIG_PARTIAL:
		{
			LX_DEMOD_DVBT2_CONFIG_T dvbt2ConfigPartial;


			DEMOD_PRINT("DEMOD_IOW_DVBT2_SET_CONFIG_PARTIAL \n");

			if ( copy_from_user( &dvbt2ConfigPartial, (void *)arg, sizeof(LX_DEMOD_DVBT2_CONFIG_T ) ) )
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) copy_from_user() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

			if(RET_OK != DEMOD_DVBT2_Set_PartialConfig(&dvbt2ConfigPartial))
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_DVBT2_Set_PartialConfig() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

		}
		break;

		case DEMOD_IOR_DVBT2_GET_MULTIPLP_INFO:
		{
			LX_DEMOD_DVBT2_MULTI_PLP_INFO_T paramMultiPLPInfo;


			DEMOD_PRINT("DEMOD_IOW_DVBT2_GET_MULTIPLP_INFO \n");

			memset( &paramMultiPLPInfo, 0x0, sizeof(LX_DEMOD_DVBT2_MULTI_PLP_INFO_T) );

			if(RET_OK != DEMOD_DVBT2_Get_multiPLP_ID(&paramMultiPLPInfo))
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_DVBT2_GetMultiPLPInfo() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

			if ( copy_to_user((void *)arg, (void *)&paramMultiPLPInfo,  sizeof(LX_DEMOD_DVBT2_MULTI_PLP_INFO_T ) ) )
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) copy_to_user() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}


		}
		break;

		case DEMOD_IO_DVBT2_PRINT_INFO:
		{
			DEMOD_PRINT("DEMOD_IO_DVBT2_PRINT_INFO ok\n");

			if(FALSE == _g_print_enb)
				OS_DEBUG_EnableModuleByIndex ( g_demod_debug_fd, 0, DBG_COLOR_NONE );

			if(RET_OK != DEMOD_DVBT2_Get_TotalInfo())
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_DVBT2_Get_TotalInfo() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}
			if(FALSE == _g_print_enb)
				OS_DEBUG_DisableModuleByIndex( g_demod_debug_fd, 0);

		}
		break;


//VSB

		case DEMOD_IO_VSB_SET_DEFAULT_VALUE:
		{
			DEMOD_PRINT("DEMOD_IO_VSB_SET_DEFAULT_VALUE \n");

			if(RET_OK != DEMOD_VSB_Set_DefaultRegisterValue())
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_VSB_Set_DefaultRegisterValue() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

		}
		break;

		case DEMOD_IO_VSB_PRINT_INFO:
		{
			DEMOD_PRINT("DEMOD_IO_VSB_PRINT_INFO ok\n");

			if(FALSE == _g_print_enb)
				OS_DEBUG_EnableModuleByIndex ( g_demod_debug_fd, 0, DBG_COLOR_NONE );

			if(RET_OK != DEMOD_VSB_Get_TotalInfo())
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_VSB_Get_TotalInfo() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}
			if(FALSE == _g_print_enb)
				OS_DEBUG_DisableModuleByIndex( g_demod_debug_fd, 0);

		}
		break;

		case DEMOD_IOR_GET_MSE_DYN:
		{
			BOOLEAN bMsedyn;


			DEMOD_PRINT("DEMOD_IOR_GET_MSE_DYN \n");


			if(RET_OK != DEMOD_VSB_Get_MSEdynStatus(&bMsedyn))
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_VSB_Get_MSEdynStatus() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

			if ( copy_to_user((void *)arg, (void *)&bMsedyn,  sizeof(BOOLEAN ) ) )
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) copy_to_user() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

		}
		break;

		case DEMOD_IOR_IS_COCHANNEL:
		{
			BOOLEAN bCochannel;


			DEMOD_PRINT("DEMOD_IOR_IS_COCHANNEL \n");


			if(RET_OK != DEMOD_VSB_CochannelExist(&bCochannel))
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_VSB_CochannelExist() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

			if ( copy_to_user((void *)arg, (void *)&bCochannel,  sizeof(BOOLEAN ) ) )
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) copy_to_user() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

		}
		break;



//QAM

		case DEMOD_IO_QAM_SET_DEFAULT_VALUE:
		{
			DEMOD_PRINT("DEMOD_IO_QAM_SET_DEFAULT_VALUE \n");

			if(RET_OK != DEMOD_QAM_Set_DefaultRegisterValue())
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_QAM_Set_DefaultRegisterValue() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

		}
		break;

		case DEMOD_IO_QAM_PRINT_INFO:
		{
			DEMOD_PRINT("DEMOD_IO_QAM_PRINT_INFO ok\n");

			if(FALSE == _g_print_enb)
				OS_DEBUG_EnableModuleByIndex ( g_demod_debug_fd, 0, DBG_COLOR_NONE );


			if(RET_OK != DEMOD_QAM_Get_TotalInfo())
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_QAM_Get_TotalInfo() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}
			if(FALSE == _g_print_enb)
				OS_DEBUG_DisableModuleByIndex( g_demod_debug_fd, 0);

		}
		break;

//ISDBT

		case DEMOD_IO_ISDBT_SET_DEFAULT_VALUE:
		{
			DEMOD_PRINT("DEMOD_IO_ISDBT_SET_DEFAULT_VALUE \n");

			if(RET_OK != DEMOD_ISDBT_Set_DefaultRegisterValue())
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_ISDBT_Set_DefaultRegisterValue() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

		}
		break;

		case DEMOD_IOW_ISDBT_SET_CONFIG_AUTO:
		{
			BOOLEAN bEnable;


			DEMOD_PRINT("DEMOD_IOW_ISDBT_SET_CONFIG_AUTO \n");

			if ( copy_from_user( &bEnable, (void *)arg, sizeof(BOOLEAN ) ) )
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) copy_from_user() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

			if(RET_OK != DEMOD_ISDBT_Set_Config_auto(bEnable))
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_ISDBT_Set_Config_auto() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

		}
		break;


		case DEMOD_IOW_ISDBT_SET_CONFIG_PARTIAL:
		{
			LX_DEMOD_ISDBT_CONFIG_T isdbtConfigPartial;


			DEMOD_PRINT("DEMOD_IOW_ISDBT_SET_CONFIG_PARTIAL \n");

			if ( copy_from_user( &isdbtConfigPartial, (void *)arg, sizeof(LX_DEMOD_ISDBT_CONFIG_T ) ) )
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) copy_from_user() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

			if(RET_OK != DEMOD_ISDBT_Set_PartialConfig(&isdbtConfigPartial))
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_ISDBT_Set_PartialConfig() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

		}
		break;

		case DEMOD_IOW_ISDBT_SET_CONFIG_FULL:
		{
			LX_DEMOD_ISDBT_CONFIG_T isdbtConfigFull;


			DEMOD_PRINT("DEMOD_IOW_ISDBT_SET_CONFIG_FULL \n");

			if ( copy_from_user( &isdbtConfigFull, (void *)arg, sizeof(LX_DEMOD_ISDBT_CONFIG_T ) ) )
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) copy_from_user() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

			if(RET_OK != DEMOD_ISDBT_Set_FullConfig(&isdbtConfigFull))
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_ISDBT_Set_FullConfig() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

		}
		break;
		case DEMOD_IOW_ISDBT_SET_NEVERLOCK_TIME:
		{
			UINT32 waitMs;


			DEMOD_PRINT("DEMOD_IOW_ISDBT_SET_NEVERLOCK_TIME \n");

			if ( copy_from_user( &waitMs, (void *)arg, sizeof(UINT32 ) ) )
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) copy_from_user() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

			if(RET_OK != DEMOD_ISDBT_Set_NeverLockWaitTime(waitMs))
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_DVBT_Set_NeverLockWaitTime() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

		}
		break;

		case DEMOD_IO_ISDBT_PRINT_INFO:
		{
			DEMOD_PRINT("DEMOD_IO_ISDBT_PRINT_INFO ok\n");

			if(FALSE == _g_print_enb)
				OS_DEBUG_EnableModuleByIndex ( g_demod_debug_fd, 0, DBG_COLOR_NONE );

			if(RET_OK != DEMOD_ISDBT_Get_TotalInfo())
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_ISDBT_Get_TotalInfo() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}
			if(FALSE == _g_print_enb)
				OS_DEBUG_DisableModuleByIndex( g_demod_debug_fd, 0);

		}
		break;

//ABB
		case DEMOD_IOW_ANALOG_I2C_OPEN:
		{
			LX_DEMOD_I2C_DESC_T i2cOpenParam;


			DEMOD_PRINT("DEMOD_IOW_ANALOG_I2C_OPEN \n");

			if ( copy_from_user( &i2cOpenParam, (void *)arg, sizeof(LX_DEMOD_I2C_DESC_T ) ) )
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) copy_from_user() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

			if(RET_OK != DEMOD_ANALOG_I2C_open(i2cOpenParam.portI2C,i2cOpenParam.slvAddr))
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_I2C_open() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}
			pDemodInfo->handleATV.portI2C = i2cOpenParam.portI2C;
			pDemodInfo->handleATV.slvAddr = i2cOpenParam.slvAddr;


		}
		break;

		case DEMOD_IO_ANALOG_I2C_CLOSE:
		{
			DEMOD_PRINT("DEMOD_IO_I2C_CLOSE ok\n");

			if(RET_OK != DEMOD_ANALOG_I2C_Close())
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_I2C_Close() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

		}
		break;



		case DEMOD_IOW_ANALOG_DEMOD_OPEN:
		{
			LX_DEMOD_ANALOG_DEMOD_OPEN_PARAM_T openParam;

			UINT32 chipDevID;
			UINT32 handleI2C;
			Handle_t phDevice;



			DEMOD_PRINT("DEMOD_IOW_ANALOG_DEMOD_OPEN \n");

			if ( copy_from_user( &openParam, (void *)arg, sizeof(LX_DEMOD_ANALOG_DEMOD_OPEN_PARAM_T ) ) )
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) copy_from_user() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

			chipDevID = openParam.chipDevID;
			handleI2C = openParam.handleI2C;

			if (RET_OK != DEMOD_ANALOG_Demod_Open(chipDevID, handleI2C, &phDevice))
			{
				DEMOD_PRINT( "[ADEMOD] FAIL (%s:%d) DEMOD_ANALOG_Demod_Open !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

			pDemodInfo->setCfgParam.analog.handleABB = phDevice;
			pDemodInfo->bIsAbbMode = TRUE;
			DEMOD_PRINT( "[ADEMOD](%s:%d)chipDevID = %d !!!\n", __F__, __L__,chipDevID);
		}
		break;


		case DEMOD_IOW_ANALOG_INIT:
		{
			Handle_t phDevice;
			LX_DEMOD_RF_MODE_T rfMode;

			DEMOD_PRINT("DEMOD_IOW_ANALOG_INIT \n");

			phDevice = pDemodInfo->setCfgParam.analog.handleABB;

			if ( copy_from_user( &rfMode, (void *)arg, sizeof(LX_DEMOD_RF_MODE_T ) ) )
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) copy_from_user() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

			if (RET_OK != DEMOD_ANALOG_Init(phDevice, rfMode ))
			{
				DEMOD_PRINT( "[ADEMOD] FAIL (%s:%d) DEMOD_ANALOG_Init !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}
			pDemodInfo->setCfgParam.analog.rfMode = rfMode;

		}
		break;

		case DEMOD_IO_ANALOG_SOFT_RESET:
		{
			Handle_t phDevice;

			DEMOD_PRINT("DEMOD_IO_SOFT_RESET ok\n");

			phDevice = pDemodInfo->setCfgParam.analog.handleABB;

			if(RET_OK != DEMOD_ANALOG_SoftwareReset(phDevice))
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_ANALOG_SoftwareReset() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

		}
		break;

		case DEMOD_IOW_ANALOG_SET_CVDLOCK:
		{
			BOOLEAN bCvdLock;

			DEMOD_PRINT("DEMOD_IOW_ANALOG_SET_CVDLOCK \n");

			if ( copy_from_user( &bCvdLock, (void *)arg, sizeof(BOOLEAN ) ) )
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) copy_from_user() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

			pDemodInfo->setCfgParam.analog.cvdLock = bCvdLock;
		}
		break;

		case DEMOD_IOW_ANALOG_SET_AFT_RANGE:
		{
			Handle_t phDevice;
			LX_DEMOD_ANALOG_ATFRANGE_PARAM_T aftRange;


			phDevice = pDemodInfo->setCfgParam.analog.handleABB;

			DEMOD_PRINT("DEMOD_IOW_ANALOG_SET_AFT_RANGE \n");

			if ( copy_from_user( &aftRange, (void *)arg, sizeof(LX_DEMOD_ANALOG_ATFRANGE_PARAM_T ) ) )
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) copy_from_user() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

			if(RET_OK != DEMOD_ANALOG_Set_AftRange(phDevice,aftRange ))
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_M14_ANALOG_Set_AftRange() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}
		}
		break;

		case DEMOD_IOW_ANALOG_SET_CVBS_DEC_SEL:
		{
			Handle_t phDevice;
			LX_DEMOD_ANALOG_CVBSDEC_SEL_T sel;


			phDevice = pDemodInfo->setCfgParam.analog.handleABB;

			DEMOD_PRINT("DEMOD_IOW_ANALOG_SET_CVBS_DEC_SEL \n");

			if ( copy_from_user( &sel, (void *)arg, sizeof(LX_DEMOD_ANALOG_CVBSDEC_SEL_T ) ) )
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) copy_from_user() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

			if(RET_OK != DEMOD_ANALOG_Set_CvbsDecCtrl(phDevice,sel ))
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_M14A0_ANALOG_Set_CvbsDecCtrl() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}
		}
		break;
		case DEMOD_IO_HW_RESET_ABB:
		{
			DEMOD_PRINT("DEMOD_IO_HW_RESET ok\n");
			if(FALSE == _gExecuteHwRest_abb)
			{
				if(RET_OK != DEMOD_ANALOG_ResetHW())
				{
					DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_ResetHW() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}

				pDemodInfo->bSetParams = FALSE;
				pDemodInfo->bIsAbbMode = TRUE;
				_gExecuteHwRest_abb = TRUE;
			}
			else
			{
				DEMOD_PRINT("[DEMOD] (%s:%d) DEMOD_ResetHW() has been already called, Nothing to do !!!\n", __F__, __L__);

			}
		}
		break;

		case DEMOD_IO_ANALOG_Set_WORK_AROUND:
		{
			DEMOD_PRINT("DEMOD_ANALOG_Set_WorkAround ok\n");
			if(RET_OK != DEMOD_ANALOG_Set_WorkAround())
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_ANALOG_Set_WorkAround() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

		}
		break;

		case DEMOD_IOW_ANALOG_SET_RATE_CONV_CTRL:
		{
			Handle_t phDevice;
			UINT32 pllClkSource;


			phDevice = pDemodInfo->setCfgParam.analog.handleABB;

			DEMOD_PRINT("DEMOD_IOW_ANALOG_SET_RATE_CONV_CTRL \n");

			if ( copy_from_user( &pllClkSource, (void *)arg, sizeof(UINT32 ) ) )
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) copy_from_user() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

			if(RET_OK != DEMOD_ANALOG_Set_CvbsRateConversion(phDevice,pllClkSource))
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_ANALOG_Set_CvbsRateConversion() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}
		}
		break;

		case DEMOD_IOW_ANALOG_SET_HIGH_RATE_CONV_OFFSET:
		{
			Handle_t phDevice;
			BOOLEAN bHiRateConvOffset;


			phDevice = pDemodInfo->setCfgParam.analog.handleABB;

			DEMOD_PRINT("DEMOD_IOW_ANALOG_SET_HIGH_RATE_CONV_OFFSET \n");

			if ( copy_from_user( &bHiRateConvOffset, (void *)arg, sizeof(BOOLEAN ) ) )
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) copy_from_user() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

			if(RET_OK != DEMOD_ANALOG_Set_HighCvbsRateOffset(phDevice,bHiRateConvOffset))
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_ANALOG_Set_HighCvbsRateOffset() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}
		}
		break;



		case DEMOD_IOW_ANALOG_SET_SIF_CTRL:
		{
			Handle_t phDevice;
			UINT32 pllClkSource;


			phDevice = pDemodInfo->setCfgParam.analog.handleABB;

			DEMOD_PRINT("DEMOD_IOW_ANALOG_SET_SIF_CTRL \n");

			if ( copy_from_user( &pllClkSource, (void *)arg, sizeof(UINT32 ) ) )
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) copy_from_user() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

			if(RET_OK != DEMOD_ANALOG_Set_SifCtrl(phDevice,pllClkSource))
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_ANALOG_Set_SifCtrl() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}
		}
		break;

		case DEMOD_IOW_ANALOG_SET_SIF_PATH_CTRL:
		{
			Handle_t phDevice;
			UINT32 pllClkSource;
			LX_DEMOD_ANALOG_SIFPATH_SEL_T sel;


			phDevice = pDemodInfo->setCfgParam.analog.handleABB;
			pllClkSource = pDemodInfo->setCfgParam.analog.pllClkSource;

			DEMOD_PRINT("DEMOD_IOW_ANALOG_SET_SIF_PATH_CTRL \n");

			if ( copy_from_user( &sel, (void *)arg, sizeof(LX_DEMOD_ANALOG_SIFPATH_SEL_T ) ) )
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) copy_from_user() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

			if(RET_OK != DEMOD_ANALOG_Set_SifPathCtrl(phDevice,pllClkSource,sel ))
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_ANALOG_Set_SifPathCtrl() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}
		}
		break;

		case DEMOD_IOW_ANALOG_SET_SPECIAL_AUD_MODE:
		{
			Handle_t phDevice;
			LX_DEMOD_ANALOG_SIF_SOUNDSYSTEM_T audioSystem;


			phDevice = pDemodInfo->setCfgParam.analog.handleABB;

			DEMOD_PRINT("DEMOD_IOW_ANALOG_SET_SIF_PATH_CTRL \n");

			if ( copy_from_user( &audioSystem, (void *)arg, sizeof(LX_DEMOD_ANALOG_SIF_SOUNDSYSTEM_T ) ) )
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) copy_from_user() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}


			if(RET_OK != DEMOD_ANALOG_Set_SpecialSifData(phDevice,audioSystem ))
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_ANALOG_Set_SpecialSifData() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}
		}
		break;


		case DEMOD_IOW_ANALOG_DEBUG_SET_SPECTRUMINV:
		{
			Handle_t phDevice;
			BOOLEAN bInv;
			UINT32 pllClkSource;
			LX_DEMOD_SMARTTUNE_PARAM_T  smartTuneParam;

			phDevice = pDemodInfo->setCfgParam.analog.handleABB;

			DEMOD_PRINT("DEMOD_IOW_ANALOG_DEBUG_SET_SPECTRUMINV \n");

			if ( copy_from_user( &bInv, (void *)arg, sizeof(BOOLEAN ) ) )
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) copy_from_user() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

			pllClkSource = pDemodInfo->setCfgParam.analog.pllClkSource;

			if(TRUE == bInv)
			{
				switch(pllClkSource)
				{
					case LX_DEMOD_ANALOG_SYMBOL_RATE_24000_KHZ :
						{
							if(LX_DEMOD_NTSC == pDemodInfo->setCfgParam.analog.rfMode)
							{
								pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_42_250_KHZ;
							}
							else
							{
								pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_41_250_KHZ;

							}
						}
						break;

					case LX_DEMOD_ANALOG_SYMBOL_RATE_25000_KHZ:
						{
							if(LX_DEMOD_NTSC == pDemodInfo->setCfgParam.analog.rfMode)
							{
								pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_42_250_KHZ;
							}
							else
							{
								pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_41_250_KHZ;

							}
						}
						break;


					case LX_DEMOD_ANALOG_SYMBOL_RATE_27000_KHZ:
						{
							if(LX_DEMOD_NTSC == pDemodInfo->setCfgParam.analog.rfMode)
							{
								pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_34_250_KHZ;
							}
							else
							{
								pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_33_250_KHZ;

							}
						}
						break;

					case LX_DEMOD_ANALOG_SYMBOL_RATE_28000_KHZ:
						{
							if(LX_DEMOD_NTSC == pDemodInfo->setCfgParam.analog.rfMode)
							{
								pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_34_250_KHZ;
							}
							else
							{
								pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_33_250_KHZ;

							}
						}
						break;

					case LX_DEMOD_ANALOG_SYMBOL_RATE_29000_KHZ:
						{
							if(LX_DEMOD_NTSC == pDemodInfo->setCfgParam.analog.rfMode)
							{
								pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_34_250_KHZ;
							}
							else
							{
								pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_33_250_KHZ;

							}
						}
						break;


					case LX_DEMOD_ANALOG_SYMBOL_RATE_31000_KHZ:
						{
							if(LX_DEMOD_NTSC == pDemodInfo->setCfgParam.analog.rfMode)
							{
								pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_34_250_KHZ;
							}
							else
							{
								pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_33_250_KHZ;

							}
						}
						break;

					case LX_DEMOD_ANALOG_SYMBOL_RATE_33000_KHZ:
						{
							if(LX_DEMOD_NTSC == pDemodInfo->setCfgParam.analog.rfMode)
							{
								pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_34_250_KHZ;
							}
							else
							{
								pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_33_250_KHZ;

							}
						}
						break;

					default :
						DEMOD_PRINT( "^r^[M14  ATV] FAIL (%s:%d) Invaild symbol rate !!!\n",  __F__, __L__);
						break;
				}
			}
			else
			{
				switch(pllClkSource)
				{
					case LX_DEMOD_ANALOG_SYMBOL_RATE_24000_KHZ :
						{
							if(LX_DEMOD_NTSC == pDemodInfo->setCfgParam.analog.rfMode)
							{
								pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_45_750_KHZ;
							}
							else
							{
								pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_46_750_KHZ;

							}
						}
						break;

					case LX_DEMOD_ANALOG_SYMBOL_RATE_25000_KHZ:
						{
							if(LX_DEMOD_NTSC == pDemodInfo->setCfgParam.analog.rfMode)
							{
								pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_45_750_KHZ;
							}
							else
							{
								pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_46_750_KHZ;

							}
						}
						break;


					case LX_DEMOD_ANALOG_SYMBOL_RATE_27000_KHZ:
						{
							if(LX_DEMOD_NTSC == pDemodInfo->setCfgParam.analog.rfMode)
							{
								pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_37_750_KHZ;
							}
							else
							{
								pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_38_750_KHZ;

							}
						}
						break;

					case LX_DEMOD_ANALOG_SYMBOL_RATE_28000_KHZ:
						{
							if(LX_DEMOD_NTSC == pDemodInfo->setCfgParam.analog.rfMode)
							{
								pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_37_750_KHZ;
							}
							else
							{
								pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_38_750_KHZ;

							}
						}
						break;

					case LX_DEMOD_ANALOG_SYMBOL_RATE_29000_KHZ:
						{
							if(LX_DEMOD_NTSC == pDemodInfo->setCfgParam.analog.rfMode)
							{
								pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_37_750_KHZ;
							}
							else
							{
								pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_38_750_KHZ;

							}
						}
						break;


					case LX_DEMOD_ANALOG_SYMBOL_RATE_31000_KHZ:
						{
							if(LX_DEMOD_NTSC == pDemodInfo->setCfgParam.analog.rfMode)
							{
								pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_37_750_KHZ;
							}
							else
							{
								pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_38_750_KHZ;

							}
						}
						break;

					case LX_DEMOD_ANALOG_SYMBOL_RATE_33000_KHZ:
						{
							if(LX_DEMOD_NTSC == pDemodInfo->setCfgParam.analog.rfMode)
							{
								pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_37_750_KHZ;
							}
							else
							{
								pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_38_750_KHZ;

							}
						}
						break;

					default :
						DEMOD_PRINT( "^r^[M14  ATV] FAIL (%s:%d) Invaild symbol rate !!!\n",  __F__, __L__);
						break;
				}

			}
			smartTuneParam.phDevice 	= pDemodInfo->setCfgParam.analog.handleABB;
			smartTuneParam.analogIF 	= pDemodInfo->setCfgParam.analog.analogIF;
			smartTuneParam.audioSystem	= pDemodInfo->setCfgParam.analog.audioSystem;
			smartTuneParam.frequency	= pDemodInfo->setCfgParam.analog.frequency;
			smartTuneParam.rfMode		= pDemodInfo->setCfgParam.analog.rfMode;
			smartTuneParam.tuneMode 	= pDemodInfo->setCfgParam.analog.tuneMode;
			smartTuneParam.sampleRate	= pDemodInfo->setCfgParam.analog.sampleRate;

			if(RET_OK != DEMOD_ANALOG_Set_SpectrumInv(phDevice,bInv,FALSE))
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_ANALOG_Set_SpectrumInv() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

			if(RET_OK !=DEMOD_ANALOG_Dbg_SmartTune(&smartTuneParam))
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_ANALOG_Dbg_SmartTune() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

		}
		break;

		case DEMOD_IOR_ANALOG_I2C_READ:
		{
			Handle_t phDevice;
			LX_DEMOD_I2C_COMM_T i2cReadParams;
			LX_DEMOD_I2C_COMM_T* in;



			DEMOD_PRINT("DEMOD_IOR_ANALOG_I2C_READ \n");

			phDevice = pDemodInfo->setCfgParam.analog.handleABB;

			in = (LX_DEMOD_I2C_COMM_T*)arg;

			if ( copy_from_user( &i2cReadParams, (void *)in , sizeof(LX_DEMOD_I2C_COMM_T ) ) )
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) copy_from_user() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}


			if(RET_OK != DEMOD_ANALOG_Dbg_Get_RegisterValue(phDevice,(UINT32)(i2cReadParams.address), (UINT32)(i2cReadParams.nBytes), (UINT32 *)&(i2cReadParams.buf)))
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_IOR_ANALOG_I2C_READ() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

			if(i2cReadParams.nBytes <= sizeof(in->buf))
			{
				if ( copy_to_user((void *)in->buf, (void *)&i2cReadParams.buf, i2cReadParams.nBytes) )
				{
					DEMOD_PRINT("[DEMOD] FAIL (%s:%d) copy_to_user() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}
			}

		}
		break;

		case DEMOD_IOW_ANALOG_I2C_WRITE:
		{
			LX_DEMOD_I2C_COMM_T i2cWriteParams;
			Handle_t phDevice;

			UINT32 wrData = 0;
			UINT8 count;

			DEMOD_PRINT("DEMOD_IOW_ANALOG_I2C_WRITE \n");

			phDevice = pDemodInfo->setCfgParam.analog.handleABB;

			if ( copy_from_user( &i2cWriteParams, (void *)arg, sizeof(LX_DEMOD_I2C_COMM_T ) ) )
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) copy_from_user() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

			if( i2cWriteParams.nBytes>4 )
			{
				DEMOD_RETURN_FAIL(__F__);
			}

			for(count = 0; count < i2cWriteParams.nBytes; count++ )
			{
				wrData += i2cWriteParams.buf[count] << (8 *count) ;
			}

			if(RET_OK != DEMOD_ANALOG_Dbg_Set_RegisterValue(phDevice, (UINT32)(i2cWriteParams.address), (UINT32)(i2cWriteParams.nBytes), wrData))
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_ANALOG_Dbg_Set_RegisterValue() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

		}
		break;

		case DEMOD_IO_ANALOG_I2C_REGDUMP:
		{
			Handle_t phDevice;

			DEMOD_PRINT("DEMOD_IOW_ANALOG_I2C_REGDUMP \n");

			phDevice = pDemodInfo->setCfgParam.analog.handleABB;

			if(RET_OK != DEMOD_ANALOG_Dbg_Get_RegDump(phDevice))
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_ANALOG_I2C_Write() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

		}
		break;

		case DEMOD_IOW_ANALOG_SET_ABB_MODE:
		{
			BOOLEAN bAbb;

			DEMOD_PRINT("DEMOD_IOW_ANALOG_SET_ABB_MODE \n");

			if ( copy_from_user( &bAbb, (void *)arg, sizeof(BOOLEAN ) ) )
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) copy_from_user() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

			if(RET_OK != DEMOD_ANALOG_Set_AbbMode(bAbb ))
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) bAbb() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}
		}
		break;

		case DEMOD_IOW_ANALOG_DEBUG_SET_IF_FREQ:
		{
			LX_DEMOD_ANALOG_PLL_PARAM_T ifPllParam;
			LX_DEMOD_SMARTTUNE_PARAM_T  smartTuneParam;
			Handle_t phDevice;
			BOOLEAN bSpectrumInv;

			DEMOD_PRINT("DEMOD_IOW_ANALOG_SET_IF_FREQ \n");

			phDevice = pDemodInfo->setCfgParam.analog.handleABB;

			if ( copy_from_user( &ifPllParam, (void *)arg, sizeof(LX_DEMOD_ANALOG_PLL_PARAM_T ) ) )
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) copy_from_user() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

			if(RET_OK != DEMOD_ANALOG_Set_IF_Frq(phDevice, ifPllParam.pllClkSource, ifPllParam.ifFrq))
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) bDEMOD_ANALOG_Set_IF_Frq() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}
			bSpectrumInv = pDemodInfo->setCfgParam.analog.bSpectrumInv;

			if(TRUE == bSpectrumInv)
			{
				switch(ifPllParam.pllClkSource)
				{
					case LX_DEMOD_ANALOG_SYMBOL_RATE_24000_KHZ :
						{
							if(LX_DEMOD_NTSC == pDemodInfo->setCfgParam.analog.rfMode)
							{
								pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_42_250_KHZ;
							}
							else
							{
								pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_41_250_KHZ;

							}
						}
						break;

					case LX_DEMOD_ANALOG_SYMBOL_RATE_25000_KHZ:
						{
							if(LX_DEMOD_NTSC == pDemodInfo->setCfgParam.analog.rfMode)
							{
								pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_42_250_KHZ;
							}
							else
							{
								pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_41_250_KHZ;

							}
						}
						break;


					case LX_DEMOD_ANALOG_SYMBOL_RATE_27000_KHZ:
						{
							if(LX_DEMOD_NTSC == pDemodInfo->setCfgParam.analog.rfMode)
							{
								pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_34_250_KHZ;
							}
							else
							{
								pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_33_250_KHZ;

							}
						}
						break;

					case LX_DEMOD_ANALOG_SYMBOL_RATE_28000_KHZ:
						{
							if(LX_DEMOD_NTSC == pDemodInfo->setCfgParam.analog.rfMode)
							{
								pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_34_250_KHZ;
							}
							else
							{
								pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_33_250_KHZ;

							}
						}
						break;

					case LX_DEMOD_ANALOG_SYMBOL_RATE_29000_KHZ:
						{
							if(LX_DEMOD_NTSC == pDemodInfo->setCfgParam.analog.rfMode)
							{
								pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_34_250_KHZ;
							}
							else
							{
								pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_33_250_KHZ;

							}
						}
						break;


					case LX_DEMOD_ANALOG_SYMBOL_RATE_31000_KHZ:
						{
							if(LX_DEMOD_NTSC == pDemodInfo->setCfgParam.analog.rfMode)
							{
								pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_34_250_KHZ;
							}
							else
							{
								pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_33_250_KHZ;

							}
						}
						break;

					case LX_DEMOD_ANALOG_SYMBOL_RATE_33000_KHZ:
						{
							if(LX_DEMOD_NTSC == pDemodInfo->setCfgParam.analog.rfMode)
							{
								pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_34_250_KHZ;
							}
							else
							{
								pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_33_250_KHZ;

							}
						}
						break;

					default :
						DEMOD_PRINT( "^r^[M14  ATV] FAIL (%s:%d) Invaild symbol rate !!!\n",  __F__, __L__);
						break;
				}
			}
			else
			{
				switch(ifPllParam.pllClkSource)
				{
					case LX_DEMOD_ANALOG_SYMBOL_RATE_24000_KHZ :
						{
							if(LX_DEMOD_NTSC == pDemodInfo->setCfgParam.analog.rfMode)
							{
								pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_45_750_KHZ;
							}
							else
							{
								pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_46_750_KHZ;

							}
						}
						break;

					case LX_DEMOD_ANALOG_SYMBOL_RATE_25000_KHZ:
						{
							if(LX_DEMOD_NTSC == pDemodInfo->setCfgParam.analog.rfMode)
							{
								pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_45_750_KHZ;
							}
							else
							{
								pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_46_750_KHZ;

							}
						}
						break;


					case LX_DEMOD_ANALOG_SYMBOL_RATE_27000_KHZ:
						{
							if(LX_DEMOD_NTSC == pDemodInfo->setCfgParam.analog.rfMode)
							{
								pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_37_750_KHZ;
							}
							else
							{
								pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_38_750_KHZ;

							}
						}
						break;

					case LX_DEMOD_ANALOG_SYMBOL_RATE_28000_KHZ:
						{
							if(LX_DEMOD_NTSC == pDemodInfo->setCfgParam.analog.rfMode)
							{
								pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_37_750_KHZ;
							}
							else
							{
								pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_38_750_KHZ;

							}
						}
						break;

					case LX_DEMOD_ANALOG_SYMBOL_RATE_29000_KHZ:
						{
							if(LX_DEMOD_NTSC == pDemodInfo->setCfgParam.analog.rfMode)
							{
								pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_37_750_KHZ;
							}
							else
							{
								pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_38_750_KHZ;

							}
						}
						break;


					case LX_DEMOD_ANALOG_SYMBOL_RATE_31000_KHZ:
						{
							if(LX_DEMOD_NTSC == pDemodInfo->setCfgParam.analog.rfMode)
							{
								pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_37_750_KHZ;
							}
							else
							{
								pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_38_750_KHZ;

							}
						}
						break;

					case LX_DEMOD_ANALOG_SYMBOL_RATE_33000_KHZ:
						{
							if(LX_DEMOD_NTSC == pDemodInfo->setCfgParam.analog.rfMode)
							{
								pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_37_750_KHZ;
							}
							else
							{
								pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_38_750_KHZ;

							}
						}
						break;

					default :
						DEMOD_PRINT( "^r^[M14  ATV] FAIL (%s:%d) Invaild symbol rate !!!\n",  __F__, __L__);
						break;
				}

			}
			pDemodInfo->setCfgParam.analog.pllClkSource = ifPllParam.pllClkSource;
			pDemodInfo->setCfgParam.analog.sampleRate = ifPllParam.sampleRate;


			smartTuneParam.phDevice 	= pDemodInfo->setCfgParam.analog.handleABB;
			smartTuneParam.analogIF 	= pDemodInfo->setCfgParam.analog.analogIF;
			smartTuneParam.audioSystem	= pDemodInfo->setCfgParam.analog.audioSystem;
			smartTuneParam.frequency	= pDemodInfo->setCfgParam.analog.frequency;
			smartTuneParam.rfMode		= pDemodInfo->setCfgParam.analog.rfMode;
			smartTuneParam.tuneMode 	= pDemodInfo->setCfgParam.analog.tuneMode;
			smartTuneParam.sampleRate	= pDemodInfo->setCfgParam.analog.sampleRate;


			if(RET_OK != DEMOD_ANALOG_Set_SpectrumInv(phDevice,bSpectrumInv, FALSE))
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_ANALOG_Set_SpectrumInv() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

			if(RET_OK !=DEMOD_ANALOG_Dbg_SmartTune(&smartTuneParam))
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_ANALOG_Dbg_SmartTune() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

		}
		break;


		case DEMOD_IOW_ANALOG_DEBUG_ABB:
		{

			LX_ADEMOD_DEBUG_COMMAND_T command;
			Handle_t phDevice;

			DEMOD_PRINT("DEMOD_IOW_ANALOG_DEBUG_ABB \n");

			phDevice = pDemodInfo->setCfgParam.analog.handleABB;

			if ( copy_from_user( &command, (void *)arg, sizeof(LX_ADEMOD_DEBUG_COMMAND_T ) ) )
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) copy_from_user() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

			if(RET_OK != DEMOD_ANALOG_Dbg_TestFunctions(phDevice, command.id, command.value))
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_ANALOG_Dbg_TestFunctions() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}
		}
		break;


	    default:
	    {
			DEMOD_PRINT("[DEMOD] FAIL (%s:%d) Invalid IOCTL() !!!\n", __F__, __L__);
			DEMOD_RETURN_FAIL(__F__);
		}
    }

    return RET_OK;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef KDRV_GLOBAL_LINK
#if defined(CONFIG_LG_BUILTIN_KDRIVER) && defined(CONFIG_LGSNAP)
user_initcall_grp("kdrv",DEMOD_Init);
#else
module_init(DEMOD_Init);
#endif
module_exit(DEMOD_Cleanup);

MODULE_AUTHOR("LGE");
MODULE_DESCRIPTION("base driver");
MODULE_LICENSE("GPL");
#endif

/** @} */

