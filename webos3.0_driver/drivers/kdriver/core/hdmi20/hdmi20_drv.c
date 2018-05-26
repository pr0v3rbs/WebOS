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
 *  author		won.hur (won.hur@lge.com)
 *  version		1.0
 *  date		2012.04.16
 *  note		Additional information.
 *
 *  @addtogroup lg115x_hdmi20
 *	@{
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/
#undef	SUPPORT_HDMI20_DEVICE_READ_WRITE_FOPS

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
#include <linux/ioport.h>	/**< For request_region, check_region etc */
#include <asm/io.h>			/**< For ioremap_nocache */
#include <linux/dma-mapping.h>
#ifdef KDRV_CONFIG_PM	// added by SC Jung for quick booting
#include <linux/platform_device.h>
#endif
#include "os_util.h"
#include "base_device.h"

#include "hdmi20_cfg.h"
#include "hdmi20_drv.h"
#include "hdmi20_module.h"

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
 *	main control block for hdmi20 device.
 *	each minor device has unique control block
 *
 */
typedef struct
{
// BEGIN of common device
	int						dev_open_count;		///< check if device is opened or not
	dev_t					devno;		    	///< device number
	struct cdev				cdev;			    ///< char device structure
	int						dev_initialized;	///< check if device is initialized or not
}
HDMI20_DEVICE_T;

#ifdef KDRV_CONFIG_PM
typedef struct
{
	bool	is_suspended;
}HDMI20_DRVDATA_T;
#endif

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/
extern	void	HDMI20_PROC_Init(void);
extern	void	HDMI20_PROC_Cleanup(void);

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Functions
----------------------------------------------------------------------------------------*/
int		HDMI20_Init(void);
void	HDMI20_Cleanup(void);

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/
int		g_hdmi20_debug_fd;
int 	g_hdmi20_major = HDMI20_MAJOR;
int 	g_hdmi20_minor = HDMI20_MINOR;
bool 	g_hdmi20_open_done = false;

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/
static int      HDMI20_Open(struct inode *, struct file *);
static int      HDMI20_Close(struct inode *, struct file *);
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)
static int 		HDMI20_Ioctl (struct inode *, struct file *, unsigned int, unsigned long );
#else
static long     HDMI20_Ioctl (struct file * filp, unsigned int cmd, unsigned long arg);
#endif
#ifdef SUPPORT_HDMI20_DEVICE_READ_WRITE_FOPS
static ssize_t  HDMI20_Read(struct file *, char *, size_t, loff_t *);
static ssize_t  HDMI20_Write(struct file *, const char *, size_t, loff_t *);
#endif

static int __HDMI20_SetLogmPrint(unsigned long arg);

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/

/**
 * main control block for hdmi20 device
*/
static HDMI20_DEVICE_T*		g_hdmi20_device;

/**
 * file I/O description for hdmi20 device
 *
*/
static struct file_operations g_hdmi20_fops =
{
	.open 	= HDMI20_Open,
	.release= HDMI20_Close,
	#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)
	.ioctl	= HDMI20_Ioctl,
	#else
	.unlocked_ioctl	= HDMI20_Ioctl,
	#endif
	#ifdef SUPPORT_HDMI20_DEVICE_READ_WRITE_FOPS
	.read 	= HDMI20_Read,
	.write 	= HDMI20_Write,
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
static int HDMI20_suspend(struct platform_device *pdev, pm_message_t state)
{
	int ret = RET_OK;
#ifdef KDRV_CONFIG_PM_HDMI20
	HDMI20_DRVDATA_T *drv_data;

	printk("HDMI20 - suspend BEGIN\n");

	if(g_hdmi20_open_done == false)
	{
		printk("HDMI20 - suspend ignore\n");
		return 0;
	}

	drv_data = platform_get_drvdata(pdev);
	// add here the suspend code
	ret = HDMI20_Module_Suspend();

	drv_data->is_suspended = 1;
	printk("HDMI20 - suspend OK\n");
#endif
	return ret;
}


/**
 *
 * resuming module.
 *
 * @param	struct platform_device *
 * @return	int 0 : OK , -1 : NOT OK
 *
 */
static int HDMI20_resume(struct platform_device *pdev)
{
	int ret = RET_OK;
#ifdef KDRV_CONFIG_PM_HDMI20
	HDMI20_DRVDATA_T *drv_data;

	printk("HDMI20 - resume BEGIN\n");

	if(g_hdmi20_open_done == false)
	{
		printk("HDMI20 - resume ignore\n");
		return 0;
	}
	
	drv_data = platform_get_drvdata(pdev);
	if(drv_data->is_suspended == 0){
		printk("HDMI20 - resume FAIL\n");
		return -1;
	}

	ret = HDMI20_Module_Resume();

	drv_data->is_suspended = 0;

	printk("HDMI20 - resume OK\n");
#endif
	return ret;
}

/**
 *
 * probing module.
 *
 * @param	struct platform_device *pdev
 * @return	int 0 : OK , -1 : NOT OK
 *
 */
 int  HDMI20_probe(struct platform_device *pdev)
{
	HDMI20_DRVDATA_T *drv_data;

	drv_data = (HDMI20_DRVDATA_T *)kmalloc(sizeof(HDMI20_DRVDATA_T) , GFP_KERNEL);

	// add here driver registering code & allocating resource code


	HDMI20_PRINT("[%s] done probe\n", HDMI20_MODULE);
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
static int  HDMI20_remove(struct platform_device *pdev)
{
	HDMI20_DRVDATA_T *drv_data;

	// add here driver unregistering code & deallocating resource code

	drv_data = platform_get_drvdata(pdev);
	kfree(drv_data);

	HDMI20_PRINT("released\n");

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
static void  HDMI20_release(struct device *dev)
{
	HDMI20_PRINT("device released\n");
}

/*
 *	module platform driver structure
 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,39)
static struct platform_driver hdmi20_driver =
#else
static struct platform_driver hdmi20_driver __refdata =
#endif
{
	.probe          = HDMI20_probe,
	.suspend        = HDMI20_suspend,
	.remove         = HDMI20_remove,
	.resume         = HDMI20_resume,
	.driver         =
	{
		.name   = HDMI20_MODULE,
	},
};

static struct platform_device hdmi20_device = {
	.name = HDMI20_MODULE,
	.id = 0,
	.id = -1,
	.dev = {
		.release = HDMI20_release,
	},
};
#endif

/** Initialize the device environment before the real H/W initialization
 *
 *  @note main usage of this function is to initialize the HAL layer and memory size adjustment
 *  @note it's natural to keep this function blank :)
 */
void HDMI20_PreInit(void)
{
	HDMI20_InitCfg( );
    /* TODO: do something */
}

int HDMI20_Init(void)
{
	int 		ret;
	int			i;
	int			err;
	dev_t		dev;

	HDMI20_INFO("%s entered \n" , __func__);

	/* Get the handle of debug output for hdmi20 device.
	 *
	 * Most module should open debug handle before the real initialization of module.
	 * As you know, debug_util offers 4 independent debug outputs for your device driver.
	 * So if you want to use all the debug outputs, you should initialize each debug output
	 * using OS_DEBUG_EnableModuleByIndex() function.
	 */
	g_hdmi20_debug_fd = DBG_OPEN( HDMI20_MODULE );
	if( g_hdmi20_debug_fd < 0) return -1;

	OS_DEBUG_EnableModule ( g_hdmi20_debug_fd );

	OS_DEBUG_EnableModuleByIndex  ( g_hdmi20_debug_fd, LX_LOGM_LEVEL_NOTI, DBG_COLOR_NONE );
	OS_DEBUG_EnableModuleByIndex  ( g_hdmi20_debug_fd, LX_LOGM_LEVEL_ERROR, DBG_COLOR_NONE );
	OS_DEBUG_EnableModuleByIndex  ( g_hdmi20_debug_fd, LX_LOGM_LEVEL_INFO, DBG_COLOR_NONE );
	OS_DEBUG_EnableModuleByIndex  ( g_hdmi20_debug_fd, LX_LOGM_LEVEL_WARNING, DBG_COLOR_NONE );
	OS_DEBUG_DisableModuleByIndex ( g_hdmi20_debug_fd, LX_LOGM_LEVEL_TRACE+0 );	//HDMI20_RX_PORT0_PRINT
	OS_DEBUG_DisableModuleByIndex ( g_hdmi20_debug_fd, LX_LOGM_LEVEL_TRACE+1 );	//HDMI20_RX_PORT1_PRINT
	OS_DEBUG_DisableModuleByIndex ( g_hdmi20_debug_fd, LX_LOGM_LEVEL_TRACE+2 );	//HDMI20_RX_PORT_INTR
	OS_DEBUG_DisableModuleByIndex ( g_hdmi20_debug_fd, LX_LOGM_LEVEL_TRACE+3 );	//HDMI20_RX_PORT0&1 Clock Handler
	OS_DEBUG_DisableModuleByIndex ( g_hdmi20_debug_fd, LX_LOGM_LEVEL_TRACE+4 );	//HDMI20_RX_PORT0&1 VIDEO Handler
	OS_DEBUG_DisableModuleByIndex ( g_hdmi20_debug_fd, LX_LOGM_LEVEL_TRACE+5 );	//HDMI20_RX_PORT0&1 AUDIO Handler
	OS_DEBUG_DisableModuleByIndex ( g_hdmi20_debug_fd, LX_LOGM_LEVEL_TRACE+6 );	//HDMI20_RX_PORT0&1 PACKET Handler
	OS_DEBUG_DisableModuleByIndex ( g_hdmi20_debug_fd, LX_LOGM_LEVEL_TRACE+7 );	//HDMI20_RX_PORT0&1 AKSV Handler
	OS_DEBUG_DisableModuleByIndex ( g_hdmi20_debug_fd, LX_LOGM_LEVEL_TRACE+8 );	//HDMI20_RX Synopsys Memory Access

	ret = HDMI20_Module_InitChipRev();
	if(ret != RET_OK){
		HDMI20_WARN(" Warning! HDMI20 chip initiation failed[%d]\n", ret);
	}

	/* allocate main device handler, register current device.
	 *
	 * If devie major is predefined then register device using that number.
	 * otherwise, major number of device is automatically assigned by Linux kernel.
	 *
	 */
	#ifdef KDRV_CONFIG_PM
	// added by SC Jung for quick booting
	if(platform_driver_register(&hdmi20_driver) < 0)
	{
		HDMI20_ERROR("[%s] platform driver register failed\n",HDMI20_MODULE);
	}
	else
	{
		if(platform_device_register(&hdmi20_device))
		{
			platform_driver_unregister(&hdmi20_driver);
			HDMI20_ERROR("[%s] platform device register failed\n",HDMI20_MODULE);
		}
		else
		{
			HDMI20_PRINT("[%s] platform register done\n", HDMI20_MODULE);
		}
	}
	#endif

	g_hdmi20_device = (HDMI20_DEVICE_T*)OS_KMalloc( sizeof(HDMI20_DEVICE_T)*HDMI20_MAX_DEVICE );

	if ( NULL == g_hdmi20_device )
	{
		HDMI20_ERROR("out of memory. can't allocate %d bytes\n", sizeof(HDMI20_DEVICE_T)* HDMI20_MAX_DEVICE );
		return -ENOMEM;
	}

	memset( g_hdmi20_device, 0x0, sizeof(HDMI20_DEVICE_T)* HDMI20_MAX_DEVICE );

	if (g_hdmi20_major)
	{
		dev = MKDEV( g_hdmi20_major, g_hdmi20_minor );
		err = register_chrdev_region(dev, HDMI20_MAX_DEVICE, HDMI20_MODULE );
	}
	else
	{
		err = alloc_chrdev_region(&dev, g_hdmi20_minor, HDMI20_MAX_DEVICE, HDMI20_MODULE );
		g_hdmi20_major = MAJOR(dev);
	}

	if ( err < 0 )
	{
		HDMI20_ERROR("can't register hdmi20 device\n" );
		return -EIO;
	}

	/* TODO : initialize your module not specific minor device */


	/* END */

	for ( i=0; i<HDMI20_MAX_DEVICE; i++ )
	{
		/* initialize cdev structure with predefined variable */
		dev = MKDEV( g_hdmi20_major, g_hdmi20_minor+i );
		cdev_init( &(g_hdmi20_device[i].cdev), &g_hdmi20_fops );
		g_hdmi20_device[i].devno		= dev;
		g_hdmi20_device[i].cdev.owner = THIS_MODULE;
		g_hdmi20_device[i].cdev.ops   = &g_hdmi20_fops;

		/* TODO: initialize minor device */

		/* END */

		err = cdev_add (&(g_hdmi20_device[i].cdev), dev, 1 );

		if (err)
		{
			HDMI20_ERROR("error (%d) while adding hdmi20 device (%d.%d)\n", err, MAJOR(dev), MINOR(dev) );
			return -EIO;
		}
        OS_CreateDeviceClass ( g_hdmi20_device[i].devno, "%s%d", HDMI20_MODULE, i );
	}

	/* initialize proc system */
	HDMI20_PROC_Init();

	HDMI20_PRINT("hdmi20 device initialized\n");

	return 0;
}

void HDMI20_Cleanup(void)
{
	int i;
	dev_t dev = MKDEV( g_hdmi20_major, g_hdmi20_minor );

	#ifdef KDRV_CONFIG_PM

	platform_driver_unregister(&hdmi20_driver);
	platform_device_unregister(&hdmi20_device);
	#endif

	/* cleanup proc system */
	HDMI20_PROC_Cleanup( );

	/* remove all minor devicies and unregister current device */
	for ( i=0; i<HDMI20_MAX_DEVICE;i++)
	{
		/* TODO: cleanup each minor device */

		/* END */
		cdev_del( &(g_hdmi20_device[i].cdev) );
	}

	/* TODO : cleanup your module not specific minor device */

	unregister_chrdev_region(dev, HDMI20_MAX_DEVICE );
	OS_Free( g_hdmi20_device );
}


///////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * open handler for hdmi20 device
 *
 */
static int
HDMI20_Open(struct inode *inode, struct file *filp)
{
    int					major,minor;
    int					ret;
	struct cdev*    	cdev;
    HDMI20_DEVICE_T*	    my_dev;

    cdev	= inode->i_cdev;
    my_dev	= container_of ( cdev, HDMI20_DEVICE_T, cdev);

	HDMI20_PRINT("%s entered \n" , __func__);

    /* TODO : add your device specific code */

	

	/* END */
	g_hdmi20_open_done = true;

    my_dev->dev_open_count++;
    filp->private_data = my_dev;

	/* some debug */
    major = imajor(inode);
    minor = iminor(inode);
    HDMI20_PRINT("device opened (%d:%d)\n", major, minor );

    return 0;
}

/**
 * release handler for hdmi20 device
 *
 */
static int
HDMI20_Close(struct inode *inode, struct file *file)
{
    int					major,minor;
    HDMI20_DEVICE_T*   	my_dev;
    struct cdev*		cdev;

	HDMI20_PRINT("%s entered \n" , __func__);
    cdev	= inode->i_cdev;
    my_dev	= container_of ( cdev, HDMI20_DEVICE_T, cdev);

    if ( my_dev->dev_open_count > 0 )
    {
        --my_dev->dev_open_count;
    }

    /* TODO : add your device specific code */

	/* END */

	/* some debug */
    major = imajor(inode);
    minor = iminor(inode);
    HDMI20_PRINT("device closed (%d:%d)\n", major, minor );
    return 0;
}

/**
 * ioctl handler for hdmi20 device.
 *
 *
 * note: if you have some critial data, you should protect them using semaphore or spin lock.
 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)
static int HDMI20_Ioctl ( struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg )
#else
static long HDMI20_Ioctl (struct file * filp, unsigned int cmd, unsigned long arg)
#endif
{
    int err = 0, ret = RET_OK;

    HDMI20_DEVICE_T*	    my_dev;
    struct cdev*		cdev;
	#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)
	#else
	struct inode *inode = filp->f_path.dentry->d_inode;
	#endif

	LX_HDMI20_CMD_T hdmi20_msg;

	cdev	= inode->i_cdev;
    my_dev	= container_of ( cdev, HDMI20_DEVICE_T, cdev);

    /*
     * check if IOCTL command is valid or not.
     * - if magic value doesn't match, return error (-ENOTTY)
     * - if command is out of range, return error (-ENOTTY)
     *
     * note) -ENOTTY means "Inappropriate ioctl for device.
     */
    if (_IOC_TYPE(cmd) != HDMI20_IOC_MAGIC)
    {
    	HDMI20_WARN("invalid magic. magic=0x%02X\n", _IOC_TYPE(cmd) );
    	return -ENOTTY;
    }
    if (_IOC_NR(cmd) > (HDMI20_IOC_MAXNR))
    {
    	HDMI20_WARN("out of ioctl command. cmd_idx=%d\n", _IOC_NR(cmd) );
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
    	HDMI20_WARN("memory access error. cmd_idx=%d, rw=%c%c, memptr=%p\n",
    													_IOC_NR(cmd),
    													(_IOC_DIR(cmd) & _IOC_READ)? 'r':'-',
    													(_IOC_DIR(cmd) & _IOC_WRITE)? 'w':'-',
    													(void*)arg );
        return -EFAULT;
	}

	switch(cmd)
	{
 		case HDMI20_IOC_COMMAND:
		{
			/* Step 1. Copy data from user */
			ret = copy_from_user(&hdmi20_msg, (LX_HDMI20_CMD_T *)arg, sizeof(LX_HDMI20_CMD_T));
			if(ret) {
				HDMI20_ERROR("[%s:%d] Error during copying from user\n", __F__, __L__);
				ret = RET_ERROR;
				break;
			}

			/* Step 2. Process cmd */
			ret = HDMI20_Module_Process_Cmd(&hdmi20_msg);

			if(ret < 0){
				HDMI20_ERROR("[%s:%d] Error during CMD process. Ret = [%d]\n", __F__, __L__, ret);
				hdmi20_msg.status = HDMI20_CMD_STATUS_PROCESS_ERROR;
			}

			/* Step 3. Copy data to user */
			ret = copy_to_user((void __user *)arg, &hdmi20_msg, sizeof(LX_HDMI20_CMD_T));
			if(ret) {
				HDMI20_ERROR("[%s:%d] Error during copying to user\n", __F__, __L__);
				ret = RET_ERROR;
				break;
			}

			/* Step 4. End */
			ret = RET_OK;
		}
		break;

		case HDMI20_IOC_SET_LOGM:
			ret = __HDMI20_SetLogmPrint(arg);
		break;

		default:
	    {
			/* redundant check but it seems more readable */
    	    ret = -ENOTTY;
		}
    }
    return ret;
}

static int __HDMI20_SetLogmPrint(unsigned long arg)
{
	int ret = RET_ERROR;
	LX_HDMI20_LOGM_SETTING_T stParams;

	do{
		/* Step 1. Copy data from application */
		ret = copy_from_user(&stParams, (LX_HDMI20_LOGM_SETTING_T *)arg, sizeof(LX_HDMI20_LOGM_SETTING_T));
		if (ret) break;

		/* Step 2. Check validty of logLevel, whether it is in the LOGM Level*/
		if((stParams.logLevel < 0)||(stParams.logLevel >= LX_LOGM_LEVEL_MAX)){
			HDMI20_ERROR("Unknown LOGM Level[%d]\n", stParams.logLevel);
			ret = RET_ERROR;
			break;
		}

		/* Step 3. Operate Logm Setting */
		if(stParams.onOff){
			OS_DEBUG_EnableModuleByIndex  ( g_hdmi20_debug_fd, stParams.logLevel, DBG_COLOR_NONE );
			HDMI20_INFO("HDMI20 LOGM Enabled for LogLevel[%d]\n", stParams.logLevel);
		}
		else{
			OS_DEBUG_DisableModuleByIndex ( g_hdmi20_debug_fd, stParams.logLevel);
			HDMI20_INFO("HDMI20 LOGM disabled for LogLevel[%d]\n", stParams.logLevel);
		}

		ret = RET_OK;
	} while(0);

	return ret;

}

///////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef KDRV_GLOBAL_LINK
#if defined(CONFIG_LG_BUILTIN_KDRIVER) && defined(CONFIG_LGSNAP)
user_initcall_grp("kdrv",HDMI20_Init);
#else
module_init(HDMI20_Init);
#endif
module_exit(HDMI20_Cleanup);

MODULE_AUTHOR("LGE");
MODULE_DESCRIPTION("HDMI20 driver");
MODULE_LICENSE("GPL");
#endif

/* @} */

