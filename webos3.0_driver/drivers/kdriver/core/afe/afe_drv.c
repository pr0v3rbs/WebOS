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
 *  main driver implementation for afe device.
 *	afe device will teach you how to make device driver with new platform.
 *
 *  author		wonsik.do (wonsik.do@lge.com)
 *  version		1.0
 *  date		2009.12.30
 *  note		Additional information.
 *
 *  @addtogroup lg1150_afe
 *	@{
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/
#undef	SUPPORT_AFE_DEVICE_READ_WRITE_FOPS

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

#ifdef KDRV_CONFIG_PM	// added by SC Jung for quick booting
#include <linux/platform_device.h>
//LGE_UPDATE_S shinhoe.kim@lge.com
bool g_AFE_Open_done = false;
//LGE_UPDATE_E shinhoe.kim@lge.com
#endif

#include "os_util.h"
#include "base_device.h"
#include "afe_drv.h"
#include <linux/interrupt.h>
#include "afe_kapi.h"
#include "adc_module.h"
//#include "cvd_control.h"
#include "cvd_module.h"

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
 *	main control block for afe device.
 *	each minor device has unique control block
 *
 */
typedef struct
{
// BEGIN of common device
	int						dev_open_count;		///< check if device is opened or not
	dev_t					devno;			///< device number
	struct cdev				cdev;			///< char device structure
	int						adc_is_initialized;		///< check if device is initialized or not
	int						cvd_is_initialized;		///< check if device is initialized or not
// END of command device

// BEGIN of device specific data


// END of device specific data
}
AFE_DEVICE_T;

#define NUM_OF_AFE_PRINT		12
#define NUM_OF_AFE_PRINT_COLOR	9

/* AFE Debug Print Type */
UINT8 *ui8AfePrintType[NUM_OF_AFE_PRINT]  =
							  { "AFE_PRINT", 	  "CVD_DEBUG", 	"ADC_DEBUG",	\
								"AFE_TRACE", "AFE_ERROR", 	"CVD_CSD_DEBUG",	\
								"CVD_WA_DEBUG", "ADC_CALIB_DEBUG", "ADC_APA_DEBUG",	\
								"ADC_IFM_DEBUG", "CVD_THREAD_DEBUG", "ADC_THREAD_DEBUG",};

/* AFE Debug Print Color */
UINT8 *ui8AfePrintColor[NUM_OF_AFE_PRINT_COLOR] = {
			"COLOR_NONE",
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
}AFE_DRVDATA_T;

#endif

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/
extern	void	AFE_PROC_Init(void);
extern	void	AFE_PROC_Cleanup(void);

//extern void	CVD_UnInit(void);

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Functions
----------------------------------------------------------------------------------------*/
int		AFE_Init(void);
void	AFE_Cleanup(void);

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/
int		g_afe_debug_fd;
int 	g_afe_major = AFE_MAJOR;
int 	g_afe_minor = AFE_MINOR;

OS_SEM_T	g_CVD_Sema;
/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/
static int      AFE_Open(struct inode *, struct file *);
static int      AFE_Close(struct inode *, struct file *);
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)
static int 		AFE_Ioctl (struct inode *, struct file *, unsigned int, unsigned long );
#else
static long     AFE_Ioctl (struct file * filp, unsigned int cmd, unsigned long arg);
#endif
#ifdef SUPPORT_AFE_DEVICE_READ_WRITE_FOPS
static ssize_t  AFE_Read(struct file *, char *, size_t, loff_t *);
static ssize_t  AFE_Write(struct file *, const char *, size_t, loff_t *);
#endif

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/

/**
 * main control block for afe device
*/
static AFE_DEVICE_T*		g_afe_device;

/**
 * file I/O description for afe device
 *
*/
static struct file_operations g_afe_fops =
{
	.open 	= AFE_Open,
	.release= AFE_Close,
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)
	.ioctl	= AFE_Ioctl,
#else
	.unlocked_ioctl = AFE_Ioctl,
#endif
#ifdef SUPPORT_AFE_DEVICE_READ_WRITE_FOPS
	.read 	= AFE_Read,
	.write 	= AFE_Write,
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
static int AFE_suspend(struct platform_device *pdev, pm_message_t state)
{
#ifdef KDRV_CONFIG_PM_AFE
	AFE_DRVDATA_T *drv_data;

  //LGE_UPDATE_S shinhoe.kim@lge.com
  if(g_AFE_Open_done == false)
  {
    printk("[%s] AFE_Open is not done skip suspend\n", AFE_MODULE);
    return 0;
  }
  //LGE_UPDATE_E shinhoe.kim@lge.com
	printk("AFE - suspend BEGIN\n");

	drv_data = platform_get_drvdata(pdev);

	// add here the suspend code
#if 1
	CVD_RunSuspend();

	if(ADC_RunSuspend() >= 0)
	{
		drv_data->is_suspended = 1;
		printk("AFE - suspend OK\n");
	}
	else
	{
		drv_data->is_suspended = 0;
		printk("AFE - suspend FAIL\n");
	}
#endif

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
static int AFE_resume(struct platform_device *pdev)
{
#ifdef KDRV_CONFIG_PM_AFE
	AFE_DRVDATA_T *drv_data;
  //LGE_UPDATE_S shinhoe.kim@lge.com
  if(g_AFE_Open_done == false)
  {
    printk("[%s] AFE_Open is not done skip resume\n", AFE_MODULE);
    return 0;
  }
  //LGE_UPDATE_E shinhoe.kim@lge.com

	printk("AFE - resume BEGIN\n");

	drv_data = platform_get_drvdata(pdev);

	if(drv_data->is_suspended == 0) return -1;


	// add here the resume code
	CVD_RunResume();
	printk("AFE - AFTER CVD RESUME \n");
	ADC_RunResume();

	drv_data->is_suspended = 0;

	printk("AFE - resume OK\n");
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
 int AFE_probe(struct platform_device *pdev)
{

	AFE_DRVDATA_T *drv_data;

	drv_data = (AFE_DRVDATA_T *)kmalloc(sizeof(AFE_DRVDATA_T) , GFP_KERNEL);


	// add here driver registering code & allocating resource code



	AFE_PRINT("[%s] done probe\n", AFE_MODULE);
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
static int  AFE_remove(struct platform_device *pdev)
{
	AFE_DRVDATA_T *drv_data;

	// add here driver unregistering code & deallocating resource code



	drv_data = platform_get_drvdata(pdev);
	kfree(drv_data);

	AFE_PRINT("released\n");

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
static void  AFE_release(struct device *dev)
{


	AFE_PRINT("device released\n");
}

/*
 *	module platform driver structure
 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,39)
static struct platform_driver afe_driver =
#else
static struct platform_driver afe_driver __refdata =
#endif
{
	.probe          = AFE_probe,
	.suspend        = AFE_suspend,
	.remove         = AFE_remove,
	.resume         = AFE_resume,
	.driver         =
	{
		.name   = AFE_MODULE,
	},
};

static struct platform_device afe_device = {
	.name = AFE_MODULE,
	.id = 0,
	.id = -1,
	.dev = {
		.release = AFE_release,
	},
};
#endif

/** Initialize the device environment before the real H/W initialization
 *
 *  @note main usage of this function is to initialize the HAL layer and memory size adjustment
 *  @note it's natural to keep this function blank :)
 */
void AFE_PreInit(void)
{
	AFE_InitCfg( );

    /* TODO: do something */
}

int AFE_Init(void)
{
	int			i;
	int			err;
	dev_t		dev;

	/* Get the handle of debug output for afe device.
	 *
	 * Most module should open debug handle before the real initialization of module.
	 * As you know, debug_util offers 4 independent debug outputs for your device driver.
	 * So if you want to use all the debug outputs, you should initialize each debug output
	 * using OS_DEBUG_EnableModuleByIndex() function.
	 */
	g_afe_debug_fd = DBG_OPEN( AFE_MODULE );
	if(g_afe_debug_fd < 0) return -1;
	OS_DEBUG_EnableModule  ( g_afe_debug_fd );
//	OS_DEBUG_DisableModule ( g_afe_debug_fd );

	OS_DEBUG_EnableModuleByIndex ( g_afe_debug_fd, LX_LOGM_LEVEL_NOTI, DBG_COLOR_NONE );	//AFE_PRINT
	OS_DEBUG_DisableModuleByIndex ( g_afe_debug_fd, LX_LOGM_LEVEL_TRACE );				//CVD_DEBUG
	OS_DEBUG_DisableModuleByIndex ( g_afe_debug_fd, LX_LOGM_LEVEL_TRACE+1 );				//ADC_DEBUG
//	OS_DEBUG_EnableModuleByIndex ( g_afe_debug_fd, LX_LOGM_LEVEL_TRACE+1 , DBG_COLOR_NONE );				//ADC_DEBUG
	OS_DEBUG_DisableModuleByIndex ( g_afe_debug_fd, LX_LOGM_LEVEL_TRACE+2 );				//AFE_TRACE
	OS_DEBUG_EnableModuleByIndex (g_afe_debug_fd, LX_LOGM_LEVEL_ERROR, DBG_COLOR_NONE );	//AFE_ERROR
	OS_DEBUG_DisableModuleByIndex(g_afe_debug_fd, LX_LOGM_LEVEL_TRACE+3);					//CVD_CSD_DEBUG
	OS_DEBUG_DisableModuleByIndex(g_afe_debug_fd, LX_LOGM_LEVEL_TRACE+4);					//CVD_WA_DEBUG
	OS_DEBUG_DisableModuleByIndex(g_afe_debug_fd, LX_LOGM_LEVEL_TRACE+5);					//ADC_CALIB_DEBUG
	OS_DEBUG_DisableModuleByIndex(g_afe_debug_fd, LX_LOGM_LEVEL_TRACE+6);					//ADC_APA_DEBUG
	OS_DEBUG_DisableModuleByIndex(g_afe_debug_fd, LX_LOGM_LEVEL_TRACE+7);					//ADC_IFM_DEBUG
	OS_DEBUG_DisableModuleByIndex(g_afe_debug_fd, LX_LOGM_LEVEL_TRACE+8);					//CVD_THREAD_DEBUG
	OS_DEBUG_DisableModuleByIndex(g_afe_debug_fd, LX_LOGM_LEVEL_TRACE+9);					//ADC_THREAD_DEBUG

/* AFE Debug Print Color */
//	DBG_COLOR_BLACK,			///< foreground : black
//	DBG_COLOR_RED,				///< foreground : red
//	DBG_COLOR_GREEN,			///< foreground : green
//	DBG_COLOR_YELLOW,			///< foreground : yellow
//	DBG_COLOR_BLUE,				///< foreground : blue
//	DBG_COLOR_PURPLE,			///< foreground : purple
//	DBG_COLOR_CYAN,				///< foreground : cyan
//	DBG_COLOR_GRAY,				///< foreground : gray

	/* allocate main device handler, register current device.
	 *
	 * If devie major is predefined then register device using that number.
	 * otherwise, major number of device is automatically assigned by Linux kernel.
	 *
	 */
#ifdef KDRV_CONFIG_PM
	// added by SC Jung for quick booting
	if(platform_driver_register(&afe_driver) < 0)
	{
		AFE_PRINT("[%s] platform driver register failed\n",AFE_MODULE);

	}
	else
	{
		if(platform_device_register(&afe_device))
		{
			platform_driver_unregister(&afe_driver);
			AFE_PRINT("[%s] platform device register failed\n",AFE_MODULE);
		}
		else
		{
			device_enable_async_suspend(&((&afe_device)->dev));
			AFE_PRINT("[%s] platform register done\n", AFE_MODULE);
		}


	}
#endif

	g_afe_device = (AFE_DEVICE_T*)OS_KMalloc( sizeof(AFE_DEVICE_T)*AFE_MAX_DEVICE );

	if ( NULL == g_afe_device )
	{
		DBG_PRINT_ERROR("out of memory. can't allocate %d bytes\n", sizeof(AFE_DEVICE_T)* AFE_MAX_DEVICE );
		return -ENOMEM;
	}

	memset( g_afe_device, 0x0, sizeof(AFE_DEVICE_T)* AFE_MAX_DEVICE );

	if (g_afe_major)
	{
		dev = MKDEV( g_afe_major, g_afe_minor );
		err = register_chrdev_region(dev, AFE_MAX_DEVICE, AFE_MODULE );
	}
	else
	{
		err = alloc_chrdev_region(&dev, g_afe_minor, AFE_MAX_DEVICE, AFE_MODULE );
		g_afe_major = MAJOR(dev);
	}

	if ( err < 0 )
	{
		DBG_PRINT_ERROR("can't register afe device\n" );
		return -EIO;
	}

	/* TODO : initialize your module not specific minor device */


	/* END */

	for ( i=0; i<AFE_MAX_DEVICE; i++ )
	{
		/* initialize cdev structure with predefined variable */
		dev = MKDEV( g_afe_major, g_afe_minor+i );
		cdev_init( &(g_afe_device[i].cdev), &g_afe_fops );
		g_afe_device[i].devno		= dev;
		g_afe_device[i].cdev.owner = THIS_MODULE;
		g_afe_device[i].cdev.ops   = &g_afe_fops;

		/* TODO: initialize minor device */


		/* END */

		err = cdev_add (&(g_afe_device[i].cdev), dev, 1 );

		if (err)
		{
			DBG_PRINT_ERROR("error (%d) while adding afe device (%d.%d)\n", err, MAJOR(dev), MINOR(dev) );
			return -EIO;
		}
        OS_CreateDeviceClass ( g_afe_device[i].devno, "%s%d", AFE_MODULE, i );
	}

	/* initialize proc system */
	AFE_PROC_Init ( );

	ADC_DEBUG("afe device initialized\n");

	return 0;
}

void AFE_Cleanup(void)
{
	int i;
	dev_t dev = MKDEV( g_afe_major, g_afe_minor );

#ifdef KDRV_CONFIG_PM
	// added by SC Jung for quick booting
	platform_driver_unregister(&afe_driver);
	platform_device_unregister(&afe_device);
#endif

	/* cleanup proc system */
	AFE_PROC_Cleanup( );

	/* remove all minor devicies and unregister current device */
	for ( i=0; i<AFE_MAX_DEVICE;i++)
	{
		/* TODO: cleanup each minor device */


		/* END */
		cdev_del( &(g_afe_device[i].cdev) );
	}

	/* TODO : cleanup your module not specific minor device */

	unregister_chrdev_region(dev, AFE_MAX_DEVICE );

	OS_Free( g_afe_device );
}


///////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * open handler for afe device
 *
 */
static int
AFE_Open(struct inode *inode, struct file *filp)
{
    int					major,minor;
    struct cdev*    	cdev;
    AFE_DEVICE_T*	my_dev;

    cdev	= inode->i_cdev;
    my_dev	= container_of ( cdev, AFE_DEVICE_T, cdev);

    /* TODO : add your device specific code */


	/* END */

    my_dev->dev_open_count++;
    filp->private_data = my_dev;

	/* some debug */
    major = imajor(inode);
    minor = iminor(inode);

/* ADC_Initialize is executed in video_ddi.c
    AFE_PRINT("device opened (%d:%d)\n", major, minor );
if(ADC_Initialize() < 0) AFE_ERROR("ADC Initialze error\n");
*/

	//Initialize CVD semaphore
	OS_InitMutex(&g_CVD_Sema, OS_SEM_ATTR_DEFAULT);

  //LGE_UPDATE_S shinhoe.kim@lge.com
  #ifdef KDRV_CONFIG_PM
  g_AFE_Open_done = true;
  #endif
  //LGE_UPDATE_E shinhoe.kim@lge.com

    return 0;
}

/**
 * release handler for afe device
 *
 */
static int
AFE_Close(struct inode *inode, struct file *file)
{
    int					major,minor;
    AFE_DEVICE_T*	my_dev;
    struct cdev*		cdev;

    cdev	= inode->i_cdev;
    my_dev	= container_of ( cdev, AFE_DEVICE_T, cdev);

    if ( my_dev->dev_open_count > 0 )
    {
        --my_dev->dev_open_count;
    }

    /* TODO : add your device specific code */

	/* disalbe the irq & timer/thread */
	CVD_UnInit();

	/* END */

	/* some debug */
    major = imajor(inode);
    minor = iminor(inode);
    AFE_PRINT("device closed (%d:%d)\n", major, minor );
    return 0;
}

/**
 * ioctl handler for afe device.
 *
 *
 * note: if you have some critial data, you should protect them using semaphore or spin lock.
 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)
static int
AFE_Ioctl ( struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg )
#else
static long
AFE_Ioctl (struct file * filp, unsigned int cmd, unsigned long arg)
#endif
{
    int err = 0, ret = 0;

    AFE_DEVICE_T*	my_dev;
    struct cdev*		cdev;

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)
#else
	struct inode *inode = filp->f_path.dentry->d_inode;
#endif
/*
    LX_AFE_ADC_SET_INPUT_T *stInput;
	LX_AFE_ADJ_PC_T	*stAdj;
	LX_AFE_ADJ_PCMODE_INFO_T *stAdjMode;
	LX_AFE_ADC_TIMING_INFO_T *stTiming;
	LX_AFE_ADC_GAIN_VALUE_T *stGain;
	LX_AFE_ADC_PIXEL_VALUE_T	*stPixelValue;
	LX_AFE_ADC_OFFSET_VALUE_T *stOffset;
	LX_AFE_PCMODE_INFO_T *stPCinfo;
	LX_AFE_SET_PCMODE_TABLE_T *stTableInfo;
	LX_AFE_ADC_CALIB_INFO_T *stCalInfo;
	LX_AFE_ADC_TEST_PARAM_T	*stADCTest;
	LX_AFE_CVD_TEST_PARAM_T	*stCVDTest;
	BOOLEAN	*stADC_Sync;
	UINT32	enable;
	LX_AFE_CVD_PQ_MODE_T	cvd_pq_mode;
	LX_AFE_CVD_BYPASS_CONTROL_T	*stCVDBypassControl;
	*/

	// for test
	//void *unused;
	//LX_AFE_SCART_MODE_T *mode;
	//S_INTERFACE_REG *reg;

	/*
	 * get current afe device object
	 */
    cdev	= inode->i_cdev;
    my_dev	= container_of ( cdev, AFE_DEVICE_T, cdev);

    /*
     * check if IOCTL command is valid or not.
     * - if magic value doesn't match, return error (-ENOTTY)
     * - if command is out of range, return error (-ENOTTY)
     *
     * note) -ENOTTY means "Inappropriate ioctl for device.
     */
    if (_IOC_TYPE(cmd) != AFE_IOC_MAGIC)
    {
    	DBG_PRINT_WARNING("invalid magic. magic=0x%02X\n", _IOC_TYPE(cmd) );
    	return -ENOTTY;
    }
    if (_IOC_NR(cmd) > AFE_IOC_MAXNR)
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

//	AFE_TRACE("cmd = %08X (cmd_idx=%d)\n", cmd, _IOC_NR(cmd) );


	switch(cmd)
	{
		case AFE_IO_ADC_INIT:


			if(my_dev->adc_is_initialized == 0)
			{
				if(ADC_Initialize() < 0)
				{
					AFE_ERROR("ADC Initialze error\n");
					ret = -1;
				}
				else
				{
					my_dev->adc_is_initialized = 1;
					ret = 0;
				}
			}
			else
			{
				AFE_ERROR("ADC Already Initialzed\n");
				ret = 0;
			}

			break;

		case AFE_IOW_CVD_INIT:
			if ((UINT32)arg == LX_CVD_MAIN)
			{
				AFE_PRINT("Main ");
				if( !(my_dev->cvd_is_initialized & (1<<LX_CVD_MAIN)) )
				{
					CVD_Init(arg);
					my_dev->cvd_is_initialized |= (1<<LX_CVD_MAIN);
					ret = 0;
				}
				else
				{
					AFE_ERROR("CVD Already Initialzed\n");
					ret = 0;
				}
			}
			else if ((UINT32)arg == LX_CVD_SUB)
			{
				ADC_DEBUG("Sub ");
				if( !(my_dev->cvd_is_initialized & (1<<LX_CVD_SUB)) )
				{
					CVD_Init(arg);
					my_dev->cvd_is_initialized |= (1<<LX_CVD_SUB);
					ret = 0;
				}
				else
				{
					ret = -1;
				}
			}
			else
				return -EFAULT;
			AFE_PRINT("CVD init\n");


			break;

		case AFE_IOR_CVD_GET_STATES:
//			AFE_PRINT("CVD GET STATE\n");
			ret = CVD_Get_States(arg);
			break;

		case AFE_IOW_SET_HSYNC_DETECTION_FOR_TUNING:
		{
			BOOLEAN	bEnable;

			if ( copy_from_user((void *)&bEnable, (void *)arg, sizeof(BOOLEAN)) )
				return -EFAULT;

			CVD_DEBUG("CVD SET Hsync detection for tuning[%d]\n", bEnable);

			ret = CVD_Set_for_Tunning(bEnable);
		}
			break;

		case AFE_IOW_SET_SCART_OVERLAY:
		{
			BOOLEAN	bEnable;

			if ( copy_from_user((void *)&bEnable, (void *)arg, sizeof(BOOLEAN)) )
				return -EFAULT;
		//	AFE_PRINT("CVD Set Scart Overlay\n");
			ret = CVD_Set_Scart_Overlay(bEnable);
		}
			break;

		case AFE_IOW_CVD_SET_SOURCE_TYPE:
		{
			LX_AFE_CVD_SET_INPUT_T	cvd_input_info;

			if (copy_from_user(&cvd_input_info, (void __user *)arg, sizeof(LX_AFE_CVD_SET_INPUT_T)))
			{
				AFE_PRINT("Error return at CVD_Set_Source_Type_Control\n");
				return -EFAULT;
			}
			ADC_DEBUG("CVD Set Source Type\n");
			ret = CVD_Set_Source_Type_Control(cvd_input_info);
		}
			break;

		case AFE_IOW_ADC_SET_SOURCE_TYPE:
		{
			LX_AFE_ADC_SET_INPUT_T stInput;

			ADC_DEBUG("ADC Set Source Type\n");

			if ( copy_from_user((void *)&stInput, (void *)arg, sizeof(LX_AFE_ADC_SET_INPUT_T)) )
				return -EFAULT;

			ret = ADC_SetInputFormat(&stInput);
#if 0
			ADC_KernelRun();	// test
#endif
		}
			break;

		case AFE_IOW_SET_ANALOG_COLOR_SYSTEM:
			ADC_DEBUG("CVD Set Analog Color System\n");
		//	ret = CVD_Set_NTSC_Only(arg);
			ret = CVD_Set_Color_System(arg);
			break;

		case AFE_IOR_GET_FB_STATUS:
			{
				LX_AFE_SCART_MODE_T		scart_fb_mode;

				CVD_Get_FB_Status(&scart_fb_mode);

				if (copy_to_user((UINT32*)arg, (UINT32*)&scart_fb_mode, sizeof(LX_AFE_SCART_MODE_T)))
				{
					return -EFAULT;
				}
				ret=0;
			}
			break;

		case AFE_IOR_GET_SCART_AR:
		{
			LX_AFE_SCART_AR_INFO_T	scart_ar_param;

			if (copy_from_user(&scart_ar_param, (void __user *)arg, sizeof(LX_AFE_SCART_AR_INFO_T)))
			{
				return -EFAULT;
			}
			//AFE_PRINT("CVD Get Scart Aspect Ratio\n");
			CVD_Get_Scart_AR(&scart_ar_param);

			if (copy_to_user((UINT32*)arg, (UINT32*)&scart_ar_param, sizeof(LX_AFE_SCART_AR_INFO_T)))
			{
				return -EFAULT;
			}
			ret=0;
		}
			break;

		case AFE_IOR_CVD_GET_TIMING_INFO:
			//AFE_TRACE("CVD Get Timing Info\n");
			CVD_Get_Timing_Info(arg);
			ret=0;
			break;

		case AFE_IOR_CVD_GET_SVIDEO_TIMING_INFO:
			//AFE_TRACE("CVD Get SVIDEO Timing Info\n");
			ret=0;
			break;

		case AFE_IOR_ADC_GET_TIMING_INFO:
			{
				LX_AFE_ADC_TIMING_INFO_T stTiming;
				//AFE_TRACE("ADC Get Timing Info\n");

				ADC_GetTimingInfo(&stTiming);

				if( copy_to_user((void *)arg, (void *)&stTiming, sizeof(LX_AFE_ADC_TIMING_INFO_T)))
					return -EFAULT;

			}

			break;

		case AFE_IOW_SET_TTX_VIDEO_STANDARD:
			ADC_DEBUG("CVD Set TTX Video Standard\n");
			ret=0;
			break;

		case AFE_IOR_PC_AUTO_ADJUST:
			{
				LX_AFE_ADJ_PC_T	stAdj;
				ADC_DEBUG("ADC PC Auto ADJUST\n");

				if( copy_from_user((void *)&stAdj, (void *)arg, sizeof(LX_AFE_ADJ_PC_T)))
					return -EFAULT;

				ADC_AutoAdjust(&stAdj);

				if( copy_to_user((void *)arg, (void *)&stAdj, sizeof(LX_AFE_ADJ_PC_T)))
					return -EFAULT;

			}
			break;

		case AFE_IOW_PC_ADJUST:
			{
				LX_AFE_ADJ_PCMODE_INFO_T stAdjMode;
				//			AFE_PRINT("ADC PC Adjust\n");

				if( copy_from_user((void *)&stAdjMode, (void *)arg, sizeof(LX_AFE_ADJ_PCMODE_INFO_T)))
					return -EFAULT;

				ret = ADC_PCAdjust(&stAdjMode);

			}
			break;

		case AFE_IOW_ADC_AUTO_CALIBRATION:
			{
				LX_AFE_ADC_CALIB_INFO_T stCalInfo;
				ADC_DEBUG("ADC Auto Calibration\n");

				if( copy_from_user((void *)&stCalInfo, (void *)arg, sizeof(LX_AFE_ADC_CALIB_INFO_T)) )
					return -EFAULT;

				if (ADC_GOC(&stCalInfo)>=0)
					stCalInfo.bResult = TRUE;
				else						//Calibration Error
					stCalInfo.bResult = FALSE;

				if( copy_to_user((void *)arg, (void *)&stCalInfo, sizeof(LX_AFE_ADC_CALIB_INFO_T)) )
					return -EFAULT;

			}
			break;

		case AFE_IOW_ADC_SET_GAIN:
			{
				LX_AFE_ADC_GAIN_VALUE_T stGain;
				ADC_DEBUG("ADC Set Gain\n");

				if( copy_from_user((void *)&stGain, (void *)arg, sizeof(LX_AFE_ADC_GAIN_VALUE_T)) )
					return -EFAULT;

				ret = ADC_SetGainValue(&stGain);

			}

			break;

		case AFE_IOW_ADC_SET_OFFSET:
			{
				LX_AFE_ADC_OFFSET_VALUE_T stOffset;
				ADC_DEBUG("ADC Set Offset\n");

				if( copy_from_user((void *)&stOffset, (void *)arg, sizeof(LX_AFE_ADC_OFFSET_VALUE_T)) )
					return -EFAULT;

				ret = ADC_SetOffsetValue(&stOffset);

			}
			break;

		case AFE_IOR_ADC_GET_GAIN:
			{
				LX_AFE_ADC_GAIN_VALUE_T stGain;
				ADC_DEBUG("ADC Get Gain\n");

				ADC_GetGainValue(&stGain);

				if( copy_to_user((void *)arg, (void *)&stGain, sizeof(LX_AFE_ADC_GAIN_VALUE_T)))
					return -EFAULT;


			}

			break;

		case AFE_IOR_ADC_GET_OFFSET:
		{
			LX_AFE_ADC_OFFSET_VALUE_T stOffset;
			ADC_DEBUG("ADC Get Offset\n");

			ret = ADC_GetOffsetValue(&stOffset);

			if( copy_to_user((void *)arg, (void *)&stOffset, sizeof(LX_AFE_ADC_OFFSET_VALUE_T)) )
				return -EFAULT;

		}

			break;

		case AFE_IOW_SET_COMP_SYNC_LEVEL:
			{
				UINT32	sog_lvl;

				if( copy_from_user((void *)&sog_lvl, (void *)arg, sizeof(UINT32)) )
					return -EFAULT;

				ADC_DEBUG("ADC Set component Sync Level to [%d]\n", sog_lvl);

				ret = ADC_Set_SOG_Level(sog_lvl);

			}
			break;

		case AFE_IOW_SET_3DCOMB_VALUE:
			{
				LX_AFE_CVD_PQ_MODE_T	cvd_pq_mode;
				ADC_DEBUG("CVD Set 3DComb Value\n");

				if( copy_from_user((void *)&cvd_pq_mode, (void *)arg, sizeof(LX_AFE_CVD_PQ_MODE_T)) )
					return -EFAULT;

				if(cvd_pq_mode >= LX_CVD_PQ_NUM)
				{
					ret = -1;
					break;
				}

				ret = CVD_Set_PQ_Mode(cvd_pq_mode);

			}
			break;

		case AFE_IOR_GET_PCMODE_INFO:
			{
				LX_AFE_PCMODE_INFO_T stPCinfo;
				//AFE_PRINT("ADC Get PCMODE Info\n");

				ret = ADC_GetPCModeInfo(&stPCinfo); //dws tmp

				if( copy_to_user((void *)arg, (void *)&stPCinfo, sizeof(LX_AFE_PCMODE_INFO_T)))
					return -EFAULT;
			}

			break;

		case AFE_IOW_SET_PCMODE_TABLE:
			{

				LX_AFE_SET_PCMODE_TABLE_T stTableInfo;
				ADC_DEBUG("ADC Set PC mode table\n");

				if( copy_from_user((void *)&stTableInfo, (void *)arg, sizeof(LX_AFE_SET_PCMODE_TABLE_T)) )
					return -EFAULT;

				ret = ADC_SetPCmodeTable(&stTableInfo);

			}
			break;

		case AFE_IOW_INTERFACE:
			ADC_DEBUG("Can NOT be used\n");
#if 0
			reg = (S_INTERFACE_REG *)kmalloc(sizeof(S_INTERFACE_REG),GFP_KERNEL);

			ret = copy_from_user((void *)reg, (void *)arg, sizeof(S_INTERFACE_REG));

			ADC_RunRegister(reg);

			kfree(reg);
#endif
			ret = 0;


		break;

		case AFE_IOW_ADC_SET_SCART_MODE:
			ADC_DEBUG("ADC Set Scart mode :");
			if(arg == TRUE)
				ADC_DEBUG("Enable \n");
			else
				ADC_DEBUG("Disable \n");

			ret = ADC_Enable_Scart_Mode((BOOLEAN)arg);

		break;

		 case AFE_IOW_VPORT_OUTPUT:
			ADC_DEBUG("AFE Select CVI Source\n");
		 break;

		case AFE_IOR_VPORT_REG_READ:
		 {
			 LX_AFE_REG_RW_T reg_addr_data_t;
			 if (copy_from_user(&reg_addr_data_t, (void __user *)arg, sizeof(LX_AFE_REG_RW_T)))
			 {
				 return -EFAULT;
			 }
			 //AFE_PRINT("AFE REG READ\n");

			 ret = CVD_Vport_Reg_Read(&reg_addr_data_t);

			 if (copy_to_user((UINT32*)arg, (UINT32*)&reg_addr_data_t, sizeof(LX_AFE_REG_RW_T)))
			 {
				 return -EFAULT;
			 }
		 }
			break;

		case AFE_IOW_VPORT_REG_WRITE:
			{
				LX_AFE_REG_RW_T reg_addr_data_t;
				if (copy_from_user(&reg_addr_data_t, (void __user *)arg, sizeof(LX_AFE_REG_RW_T)))
				{
					return -EFAULT;
				}
				ret = CVD_Vport_Reg_Write(&reg_addr_data_t);
			}
			break;

		case AFE_IOW_CVD_STOP_TIMER:
			ADC_DEBUG("AFE Stop Timer\n");
			ret = CVD_Stop_Timer(arg);
			break;

		case AFE_IOW_CVD_START_TIMER:
			ADC_DEBUG("AFE Start Timer\n");
			ret = CVD_Start_Timer(arg);
			break;

		case AFE_IOW_CVD_PROGRAM_COLOR_SYSTEM:
			{
				LX_AFE_CVD_PROGRAM_COLOR_SYSTEM_T program_color_system_t;

				if (copy_from_user(&program_color_system_t, (void __user *)arg, sizeof(LX_AFE_CVD_PROGRAM_COLOR_SYSTEM_T)))
				{
					return -EFAULT;
				}

				ADC_DEBUG("AFE Program Color System\n");
				ret = CVD_Program_Color_System(program_color_system_t);
			}
			break;

		case AFE_IOW_SET_USER_PCMODE_RESOLUTION:
			ADC_DEBUG("AFE Set User PCMODE Resolution\n");

			ret = ADC_Set_PCMODE_Resoultion(arg);
			break;

		case AFE_IOW_ADC_SET_LLPLL:
			ADC_DEBUG("AFE Set ADC LLPLL\n");

			ret = ADC_Set_LLPLL(arg);
			break;

		case AFE_IOW_CVD_SET_VBI_TYPE:
			ADC_DEBUG("Warning! No longer support this IOCTL call from H13A0\n");

			ret = RET_ERROR;
			break;

		case AFE_IOR_ADC_SYNC_EXIST:
			{
				BOOLEAN	stADC_Sync;

				if(ADC_Check_Sync_Exist() == 0) // Sync Loss
					stADC_Sync = FALSE;
				else
					stADC_Sync = TRUE;

				if( copy_to_user((void *)arg, (void *)&stADC_Sync, sizeof(BOOLEAN)) )
					return -EFAULT;

			}

			break;

		case AFE_IOW_ADC_TEST:
			{
				LX_AFE_ADC_TEST_PARAM_T	stADCTest;
				ADC_DEBUG("ADC TEST Program\n");

				if( copy_from_user((void *)&stADCTest, (void *)arg, sizeof(LX_AFE_ADC_TEST_PARAM_T)) )
					return -EFAULT;

				ret = ADC_Test_Program(&stADCTest);

				if( copy_to_user((void *)arg, (void *)&stADCTest, sizeof(LX_AFE_ADC_TEST_PARAM_T)) )
					return -EFAULT;

			}
			break;

		case AFE_IOW_CVD_TEST:
			{
				LX_AFE_CVD_TEST_PARAM_T	stCVDTest;

				if( copy_from_user((void *)&stCVDTest, (void *)arg, sizeof(LX_AFE_CVD_TEST_PARAM_T)) )
					return -EFAULT;

				ret = CVD_Test_Program(&stCVDTest);

			}
			break;

		case	AFE_IOW_ADC_RESET_DIGITAL:
				ADC_Reset_Digital();
				break;

		case	AFE_IOW_ADC_RESET_DIGITAL_24MHZ:
				ADC_Reset_Digital_24MHZ();
				break;

		case	AFE_IOW_ADC_RESET_LLPLL:
				ADC_Reset_LLPLL();
				break;

		case AFE_IOW_CVD_SET_SW_3DCOMB_CONTROL:
				//CVD_Set_SW_3DCOMB_Control(arg);
				break;

		case AFE_IOW_ADC_POWER_CONTROL:
				{
					UINT32	enable;

					if( copy_from_user(&enable, (void *)arg, sizeof(UINT32)) )
						return -EFAULT;

//					ADC_DEBUG("ADC Power Control [%d]\n", enable);

					/*
					if ( lx_chip_rev( ) >= LX_CHIP_REV( L9, A0 ) )
						ADC_LVDS_Control(enable); // added by won.hur(won.hur@lge.com) @2011.08.17
						*/

					if(enable >0)
					{
						ADC_Channel_Power_Control(1, 1, 1, 1);

						// 20110602 by wonsik.do
						// Workaround for horizontal shift problem on component input.
						// 121218 : WA code move into ADC_Channel_Power_Control function
						//ADC_Reset_Digital();
					}
					else
						ADC_Channel_Power_Control(0, 0, 0, 0);
				}
				break;

		case AFE_IOW_CVD_POWER_CONTROL:
				{
					UINT32	enable;
					if( copy_from_user(&enable, (void *)arg, sizeof(UINT32)) )
						return -EFAULT;

					CVD_Channel_Power_Control(enable);
				}
				break;

		case AFE_IOR_ADC_READ_PIXEL_VALUE:
				{
					LX_AFE_ADC_PIXEL_VALUE_T	stPixelValue;

					if( copy_from_user((void *)&stPixelValue, (void *)arg, sizeof(LX_AFE_ADC_PIXEL_VALUE_T)) )
						return -EFAULT;

					ret =	ADC_Read_Pixel_Value(&stPixelValue);

					if( copy_to_user((void *)arg, (void *)&stPixelValue, sizeof(LX_AFE_ADC_PIXEL_VALUE_T)) )
						return -EFAULT;


				}
				break;

		case AFE_IOW_ADC_ENABLE_PERIODIC_SIGNAL_INFO_READ:
				{
					UINT32	enable;

					if( copy_from_user(&enable, (void *)arg, sizeof(UINT32)) )
						return -EFAULT;

					ret = ADC_Enable_Periodic_Signal_Info_Read(enable);

				}
				break;

		case AFE_IOW_ADC_EXECUTE_FORMAT_DETECTION:
			ret =  ADC_Execute_Format_Detection();

			break;

		case AFE_IOW_ADC_ENABLE_COMPONENT_AUTO_PHASE:
			{
				UINT32	enable;
				if( copy_from_user(&enable, (void *)arg, sizeof(UINT32)) )
					return -EFAULT;

				ret = ADC_Enable_Component_Auto_Phase(enable);
			}

			break;

		case AFE_IOW_CVD_ENABLE_DETECTION_INTERRUPT:
			{
				UINT32	enable;
				if( copy_from_user(&enable, (void *)arg, sizeof(UINT32)) )
					return -EFAULT;

		//		ret = CVD_Enable_Detection_Interrupt(enable);
				ret = 0;
			}
			break;

		case AFE_IOR_CVD_GET_STATES_DETAIL:
			ret = CVD_Get_States_Detail(arg);
			break;

		case AFE_IOW_CVD_BYPASS_CONTROL:
			{
				LX_AFE_CVD_BYPASS_CONTROL_T	stCVDBypassControl;

				if( copy_from_user((void *)&stCVDBypassControl, (void *)arg, sizeof(LX_AFE_CVD_BYPASS_CONTROL_T)) )
					return -EFAULT;

				if ( lx_chip_rev( ) >= LX_CHIP_REV( L9, A0 ) )
					ret =	CVD_Bypass_Control(&stCVDBypassControl);

			}
			break;

		case AFE_IOW_CVD_VDAC_POWER_CONTROL:
			{
				BOOLEAN	bVDAC_Power_Control;

				if( copy_from_user((void *)&bVDAC_Power_Control, (void *)arg, sizeof(BOOLEAN)) )
					return -EFAULT;

				ret =	CVD_OnOff_VDAC(bVDAC_Power_Control);
			}
			break;

		case AFE_IOW_LVDS_SRC_CONTROL:
			{
				LX_AFE_LVDS_SRC_CONTROL_T LVDS_Control_t;

				if( copy_from_user((void *)&LVDS_Control_t, (void *)arg, sizeof(LX_AFE_LVDS_SRC_CONTROL_T)) )
					return -EFAULT;

					ret =	ADC_LVDS_Src_Control(&LVDS_Control_t);
			}
			break;

		case AFE_IOW_CVD_SET_HSTATE_MAX:
			{
				UINT32	hstate_max_value;

				if( copy_from_user((void *)&hstate_max_value, (void *)arg, sizeof(UINT32)) )
					return -EFAULT;

				ADC_DEBUG("ADC Set component Sync Level to [%d]\n", hstate_max_value);

				ret = CVD_Set_Hstate_Max(hstate_max_value);

			}
			break;

		case AFE_IOR_GET_MEM_CFG:
			{
				LX_AFE_CVD_MEM_CFG_T	cvd_mem_cfg_t;

				AFE_GetMemCfg(&cvd_mem_cfg_t.mem_base_comb_buffer, &cvd_mem_cfg_t.mem_size_comb_buffer, &cvd_mem_cfg_t.mem_base_cvd_reg, &cvd_mem_cfg_t.mem_size_cvd_reg );

				if( copy_to_user((void *)arg, (void *)&cvd_mem_cfg_t, sizeof(LX_AFE_CVD_MEM_CFG_T)) )
					return -EFAULT;
			}
			break;

		case AFE_IOR_READ_ACE_REG:
			{
				LX_AFE_REG_RW_T read_param;

				if( copy_from_user((void *)&read_param , (void *)arg , sizeof(LX_AFE_REG_RW_T)) )
					return -EFAULT;

				ret = ADC_Read_ACE_Reg(read_param.addr , &read_param.data);

				if( copy_to_user((void *)arg, (void *)&read_param, sizeof(LX_AFE_REG_RW_T)) )
					return -EFAULT;
			}
			break;

		case AFE_IOW_WRITE_ACE_REG:
			{
				LX_AFE_REG_RW_T write_param;

				if( copy_from_user((void *)&write_param , (void *)arg , sizeof(LX_AFE_REG_RW_T)) )
					return -EFAULT;

				ret = ADC_Write_ACE_Reg(write_param.addr , write_param.data);

			}
			break;

		case AFE_IOR_GET_CK_DETCTION:
			{
				LX_AFE_CVD_CK_T ck_detection_t;

				ret = CVD_Get_Crunky_Status(&ck_detection_t);

				if( copy_to_user((void *)arg, (void *)&ck_detection_t, sizeof(LX_AFE_CVD_CK_T)) )
					return -EFAULT;
			}
			break;

 		case AFE_IOW_DEBUG_PRINT_CTRL:
			{
				LX_AFE_DEBUG_CTRL_T	i_stDebugCtrl;

				memset(&i_stDebugCtrl, 0x0, sizeof(LX_AFE_DEBUG_CTRL_T));

				if(copy_from_user( &i_stDebugCtrl, (void __user *)arg, sizeof(LX_AFE_DEBUG_CTRL_T)))
					return RET_ERROR;

				AFE_TRACE("AFE_IOW_DEBUG_PRINT_CTRL:type[%d],color[%d],print[%d]\n", i_stDebugCtrl.printType, i_stDebugCtrl.printColor, i_stDebugCtrl.printEnable);

//				if( (i_stDebugCtrl.printType > ui8AfePrintType[NUM_OF_AFE_PRINT]) || (i_stDebugCtrl.printColor >= NUM_OF_AFE_PRINT_COLOR) )
//				{
//					AFE_ERROR("Check Argument\n" );
//					return RET_ERROR;
//				}

				if ( i_stDebugCtrl.printEnable == 0)
				{
					OS_DEBUG_DisableModuleByIndex(g_afe_debug_fd, i_stDebugCtrl.printType);
					AFE_PRINT("AFE_PRINT_TYPE[%d] is disabled.\n", i_stDebugCtrl.printType);
				}
				else
				{
					OS_DEBUG_EnableModuleByIndex(g_afe_debug_fd, i_stDebugCtrl.printType, i_stDebugCtrl.printColor);
					AFE_PRINT("AFE_PRINT_TYPE[%d] is enabled Color is [%d].\n", i_stDebugCtrl.printType, i_stDebugCtrl.printColor);
				}

				return RET_OK;
			}
		break;

		case AFE_IOR_DETECT_COMPONENT_PSEUDO_PULSE:
			{
				LX_AFE_ADC_COMP_PSEUDO_PULSE_T comp_psp_t;

				ret = ADC_Get_Component_Pseudo_Pulse(&comp_psp_t);

				if( copy_to_user((void *)arg, (void *)&comp_psp_t, sizeof(LX_AFE_ADC_COMP_PSEUDO_PULSE_T)) )
					return -EFAULT;
			}
			break;

		case AFE_IOW_SET_CVD_BLACK_LEVEL:
			{
				LX_AFE_CVD_BLACK_LEVEL_T cvd_black_level;

				if( copy_from_user((void *)&cvd_black_level , (void *)arg , sizeof(LX_AFE_CVD_BLACK_LEVEL_T)) )
					return -EFAULT;

				ret = CVD_Set_Black_Level(cvd_black_level);

			}
			break;

		case AFE_IOR_GET_ACE_FB_STATUS:
			{
				LX_AFE_SCART_MODE_T scart_mode;

				ret = ADC_Get_ACE_FB_Status(&scart_mode);

				if( copy_to_user((void *)arg, (void *)&scart_mode, sizeof(LX_AFE_SCART_MODE_T)) )
					return -EFAULT;
			}
			break;

		case AFE_IOW_SET_SCART_BYPASS_MODE:
			{
				LX_AFE_SCART_BYPASS_MODE_T scart_rgb_mode;

				if( copy_from_user((void *)&scart_rgb_mode , (void *)arg , sizeof(LX_AFE_SCART_BYPASS_MODE_T)) )
					return -EFAULT;

				ret = ADC_Set_SCART_RGB_Operation_Mode(scart_rgb_mode);

			}
			break;

		case AFE_IOW_CVD_CHANNEL_CHANGE:
			{

				BOOLEAN bATV_Channel_Change;

				if( copy_from_user((void *)&bATV_Channel_Change , (void *)arg , sizeof(BOOLEAN)) )
					return -EFAULT;

				ret = CVD_Channel_Change_Control(bATV_Channel_Change);
			}
			break;

		case AFE_IOR_CVD_GET_NOISE_STATUS:
			{
				UINT32 cvd_noise_status;

				cvd_noise_status = CVD_Get_Noise_Status();

				if( copy_to_user((void *)arg, (void *)&cvd_noise_status, sizeof(UINT32)) )
					return -EFAULT;

				ret = 0;
			}
			break;

		case AFE_IOW_CVD_VDAC_MUTE_CONTROL:
			{
				int mute_enable;

				if( copy_from_user((void *)&mute_enable, (void *)arg, sizeof(int)) )
					return -EFAULT;

				ret =	CVD_VDAC_mute_control(mute_enable);
			}
			break;

		case AFE_IOR_GET_COMPONENT_VBI_CP:
			{
				LX_AFE_ADC_COMP_VBI_CP_T	stComp_CP_Data;

				ADC_Component_Get_CP_Data(&stComp_CP_Data);

				if( copy_to_user((void *)arg, (void *)&stComp_CP_Data, sizeof(LX_AFE_ADC_COMP_VBI_CP_T)) )
					return -EFAULT;

				ret = 0;
			}
			break;

		case AFE_IO_FLUSH_COMPONENT_VBI_CP:
			{
				ret = ADC_Flush_Component_CP_Data();

			}
			break;

		case AFE_IOW_SET_INTERNAL_DEMOD_MODE:
			{
				BOOLEAN internal_demod_mode;

				if( copy_from_user((void *)&internal_demod_mode, (void *)arg, sizeof(BOOLEAN)) )
					return -EFAULT;

				ret = CVD_Set_Use_Internal_Demod(internal_demod_mode);
			}
			break;

#if 0
		case AFE_IO_RESET:
		{
			AFE_PRINT("reset ok\n");
			ret = 0;
		}
		break;
		case AFE_IOR_CHIP_REV_INFO:
		{
			CHIP_REV_INFO_T	rev_info;

			rev_info.version = 0x100;
			rev_info.date[0] =  9;	/* 2009/11/24 */
			rev_info.date[0] = 11;
			rev_info.date[0] = 24;

			AFE_PRINT("rev_info (%0X, %d:%d:%d)\n", rev_info.version,
														rev_info.date[0], rev_info.date[1], rev_info.date[2] );

	        if ( copy_to_user((void __user *)arg, &rev_info, sizeof(CHIP_REV_INFO_T)) )
    	        return -EFAULT;

			ret = 0;
		}
		break;

		case AFE_IOW_WRITE_UINT32:
		{
			UINT32	data;

			ret = __get_user( data, (UINT32 __user *)arg );

			AFE_PRINT("data = 0x%X\n", data );

			ret = 0;
		}
		break;
#endif


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
user_initcall_grp("kdrv",AFE_Init);
#else
module_init(AFE_Init);
#endif
module_exit(AFE_Cleanup);

MODULE_AUTHOR("LGE");
MODULE_DESCRIPTION("base driver");
MODULE_LICENSE("GPL");
#endif

/** @} */

