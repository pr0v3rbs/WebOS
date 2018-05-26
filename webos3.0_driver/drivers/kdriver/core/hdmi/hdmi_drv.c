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
 *  main driver implementation for hdmi device.
 *  hdmi device will teach you how to make device driver with new platform.
 *
 *  author	sh.myoung (sh.myoung@lge.com)
 *  version	1.0
 *  date		2010.02.21
 *  note		Additional information.
 *
 *  @addtogroup lg115x_hdmi
 *  @{
 */

/*----------------------------------------------------------------------------------------
  Control Constants
  ----------------------------------------------------------------------------------------*/
#undef	SUPPORT_HDMI_DEVICE_READ_WRITE_FOPS

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

#ifdef KDRV_CONFIG_PM	// added by SC Jung for quick booting
#include <linux/platform_device.h>
//LGE_UPDATE_S shinhoe.kim@lge.com
bool g_HDMI_Open_done = false;
//LGE_UPDATE_E shinhoe.kim@lge.com
#endif

#include "os_util.h"
#include "base_device.h"
#include "hdmi_drv.h"
#include "hdmi_module.h"

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
 *	main control block for hdmi device.
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
	int						dev_initialized;	///< check if device is initialized or not
	// END of device specific data
}
HDMI_DEVICE_T;

typedef struct
{
	unsigned int read_write;
	unsigned int sub;
	unsigned int addr;
	unsigned int size;
	unsigned int value;

}S_INTERFACE_REG;

#ifdef KDRV_CONFIG_PM
typedef struct
{
	// add here extra parameter
	bool	is_suspended;
}HDMI_DRVDATA_T;
#endif

/* HDMI Debug Print Type */
UINT8 *ui8HDMIPrintType[LX_HDMI_MAX]  = { "HDMI_ERROR", "HDMI_WARN", "HDMI_NOTI", "HDMI_PRINT", "HDMI_DEBUG", "HDMI_TRACE",\
	"HDMI_AUDIO", "HDMI_ATASK", "HDMI_VIDEO", "HDMI_TASK", "HDMI_INTR"};

/* HDMI Debug Print Color */
UINT8 *ui8HDMIPrintColor[8] = {
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
extern	void	HDMI_PROC_Init(void);
extern	void	HDMI_PROC_Cleanup(void);

/*----------------------------------------------------------------------------------------
  External Variables
  ----------------------------------------------------------------------------------------*/
OS_SEM_T	g_HDMI_Sema;

/*----------------------------------------------------------------------------------------
  global Functions
  ----------------------------------------------------------------------------------------*/
int	HDMI_Init(void);
void	HDMI_Cleanup(void);

/*----------------------------------------------------------------------------------------
  global Variables
  ----------------------------------------------------------------------------------------*/
int	g_hdmi_debug_fd;
int 	g_hdmi_major = HDMI_MAJOR;
int 	g_hdmi_minor = HDMI_MINOR;

/*----------------------------------------------------------------------------------------
  Static Function Prototypes Declarations
  ----------------------------------------------------------------------------------------*/
static int      HDMI_Open(struct inode *, struct file *);
static int      HDMI_Close(struct inode *, struct file *);
static long		HDMI_Ioctl (struct file *, unsigned int, unsigned long );
#ifdef SUPPORT_HDMI_DEVICE_READ_WRITE_FOPS
static ssize_t  HDMI_Read(struct file *, char *, size_t, loff_t *);
static ssize_t  HDMI_Write(struct file *, const char *, size_t, loff_t *);
#endif

/*----------------------------------------------------------------------------------------
  Static Variables
  ----------------------------------------------------------------------------------------*/
static UINT32 _g_hdmi_print_mask = 0;

/**
 * main control block for hdmi device
 */
static HDMI_DEVICE_T*		g_hdmi_device;

/**
 * file I/O description for hdmi device
 *
 */
static struct file_operations g_hdmi_fops =
{
	.open 	= HDMI_Open,
	.release= HDMI_Close,
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)
	.ioctl	= HDMI_Ioctl,
#else
	.unlocked_ioctl= HDMI_Ioctl,
#endif

#ifdef SUPPORT_HDMI_DEVICE_READ_WRITE_FOPS
	.read 	= HDMI_Read,
	.write 	= HDMI_Write,
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
static int HDMI_suspend(struct platform_device *pdev, pm_message_t state)
{
#ifdef KDRV_CONFIG_PM_HDMI
	HDMI_DRVDATA_T *drv_data;
  //LGE_UPDATE_S shinhoe.kim@lge.com
	printk("HDMI - suspend BEGIN\n");

  if(g_HDMI_Open_done == false)
  {
	printk("HDMI - suspend ignore\n");
    return 0;
  }
  //LGE_UPDATE_E shinhoe.kim@lge.com

	drv_data = platform_get_drvdata(pdev);

	// add here the suspend code
	if(HDMI_RunSuspend() >=0 )
	{
		drv_data->is_suspended = 1;
		printk("HDMI - suspend OK\n");
	}
	else
	{
		drv_data->is_suspended = 0;
		printk("HDMI - suspend FAIL\n");
	}
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
static int HDMI_resume(struct platform_device *pdev)
{
#ifdef KDRV_CONFIG_PM_HDMI
	HDMI_DRVDATA_T *drv_data;
    //LGE_UPDATE_S shinhoe.kim@lge.com

	printk("HDMI - resume BEGIN\n");

  if(g_HDMI_Open_done == false)
  {
		printk("HDMI - resume ignore\n");
		return 0;
  }
  //LGE_UPDATE_E shinhoe.kim@lge.com

	drv_data = platform_get_drvdata(pdev);

	if(drv_data->is_suspended == 0)
	{
		printk("HDMI - resume FAIL\n");
 		return -1;
	}

	// add here the resume code
	HDMI_RunResume();

	drv_data->is_suspended = 0;
	printk("HDMI - resume OK\n");
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
int  HDMI_probe(struct platform_device *pdev)
{
	HDMI_DRVDATA_T *drv_data;

	drv_data = (HDMI_DRVDATA_T *)kmalloc(sizeof(HDMI_DRVDATA_T) , GFP_KERNEL);

	// add here driver registering code & allocating resource code
	HDMI_PRINT("[%s] done probe\n", HDMI_MODULE);
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
static int  HDMI_remove(struct platform_device *pdev)
{
	HDMI_DRVDATA_T *drv_data;

	// add here driver unregistering code & deallocating resource code
	drv_data = platform_get_drvdata(pdev);
	kfree(drv_data);

	HDMI_PRINT("released\n");

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
static void  HDMI_release(struct device *dev)
{
	HDMI_PRINT("device released\n");
}

/*
 *	module platform driver structure
 */
static struct platform_driver hdmi_driver =
{
	.probe          = HDMI_probe,
	.suspend        = HDMI_suspend,
	.remove         = HDMI_remove,
	.resume         = HDMI_resume,
	.driver         =
	{
		.name   = HDMI_MODULE,
	},
};

static struct platform_device hdmi_device = {
	.name = HDMI_MODULE,
	.id = 0,
	.id = -1,
	.dev = {
			.release = HDMI_release,
		},
};
#endif

/** Initialize the device environment before the real H/W initialization
 *
 *  @note main usage of this function is to initialize the HAL layer and memory size adjustment
 *  @note it's natural to keep this function blank :)
 */
void HDMI_PreInit(void)
{
	/* TODO: do something */
}

int HDMI_Init(void)
{
	int			i;
	int			err;
	dev_t		dev;

	/* Get the handle of debug output for hdmi device.
	 *
	 * Most module should open debug handle before the real initialization of module.
	 * As you know, debug_util offers 4 independent debug outputs for your device driver.
	 * So if you want to use all the debug outputs, you should initialize each debug output
	 * using OS_DEBUG_EnableModuleByIndex() function.
	 */
	g_hdmi_debug_fd = DBG_OPEN( HDMI_MODULE );
	if(g_hdmi_debug_fd < 0) return -1;

	OS_DEBUG_EnableModule ( g_hdmi_debug_fd );
	OS_DEBUG_EnableModuleByIndex ( g_hdmi_debug_fd, LX_LOGM_LEVEL_ERROR, DBG_COLOR_RED);	//HDMI_ERROR
	OS_DEBUG_EnableModuleByIndex ( g_hdmi_debug_fd, LX_LOGM_LEVEL_WARNING, DBG_COLOR_RED);	//HDMI_WARN
	OS_DEBUG_EnableModuleByIndex ( g_hdmi_debug_fd, LX_LOGM_LEVEL_NOTI, DBG_COLOR_BLACK);		//HDMI_NOTI
	OS_DEBUG_DisableModuleByIndex ( g_hdmi_debug_fd, LX_LOGM_LEVEL_INFO);		//HDMI_PRINT
//	OS_DEBUG_EnableModuleByIndex ( g_hdmi_debug_fd, LX_LOGM_LEVEL_DEBUG, DBG_COLOR_BLACK);	//HDMI_DEBUG
	OS_DEBUG_DisableModuleByIndex ( g_hdmi_debug_fd, LX_LOGM_LEVEL_DEBUG);	//HDMI_DEBUG
	OS_DEBUG_DisableModuleByIndex ( g_hdmi_debug_fd, LX_LOGM_LEVEL_TRACE);	//HDMI_TRACE
	OS_DEBUG_DisableModuleByIndex ( g_hdmi_debug_fd, LX_HDMI_AUDIO);		//HDMI_AUDIO
	OS_DEBUG_DisableModuleByIndex ( g_hdmi_debug_fd, LX_HDMI_ATASK);		//HDMI_ATASK
	OS_DEBUG_DisableModuleByIndex ( g_hdmi_debug_fd, LX_HDMI_VIDEO);		//HDMI_VIDEO
//	OS_DEBUG_EnableModuleByIndex ( g_hdmi_debug_fd, LX_HDMI_VIDEO, DBG_COLOR_BLACK);		//HDMI_VIDEO
	OS_DEBUG_DisableModuleByIndex ( g_hdmi_debug_fd, LX_HDMI_TASK);		//HDMI_TASK
//	OS_DEBUG_EnableModuleByIndex ( g_hdmi_debug_fd, LX_HDMI_TASK, DBG_COLOR_BLACK);		//HDMI_TASK
	OS_DEBUG_DisableModuleByIndex ( g_hdmi_debug_fd, LX_HDMI_INTR);		//HDMI_INTR

	/* allocate main device handler, register current device.
	 *
	 * If devie major is predefined then register device using that number.
	 * otherwise, major number of device is automatically assigned by Linux kernel.
	 *
	 */
#ifdef KDRV_CONFIG_PM
	// added by SC Jung for quick booting
	if(platform_driver_register(&hdmi_driver) < 0)
	{
		HDMI_ERROR("[%s] platform driver register failed\n",HDMI_MODULE);
	}
	else
	{
		if(platform_device_register(&hdmi_device))
		{
			platform_driver_unregister(&hdmi_driver);
			HDMI_ERROR("[%s] platform device register failed\n",HDMI_MODULE);
		}
		else
		{
			device_enable_async_suspend(&((&hdmi_device)->dev));
			HDMI_NOTI("[%s] platform register done\n", HDMI_MODULE);
		}
	}
#endif

	g_hdmi_device = (HDMI_DEVICE_T*)OS_KMalloc( sizeof(HDMI_DEVICE_T)*HDMI_MAX_DEVICE );

	if ( NULL == g_hdmi_device )
	{
		HDMI_ERROR("out of memory. can't allocate %d bytes\n", sizeof(HDMI_DEVICE_T)* HDMI_MAX_DEVICE );
		return -ENOMEM;
	}

	memset( g_hdmi_device, 0x0, sizeof(HDMI_DEVICE_T)* HDMI_MAX_DEVICE );

	if (g_hdmi_major)
	{
		dev = MKDEV( g_hdmi_major, g_hdmi_minor );
		err = register_chrdev_region(dev, HDMI_MAX_DEVICE, HDMI_MODULE );
	}
	else
	{
		err = alloc_chrdev_region(&dev, g_hdmi_minor, HDMI_MAX_DEVICE, HDMI_MODULE );
		g_hdmi_major = MAJOR(dev);
	}

	if ( err < 0 )
	{
		HDMI_ERROR("can't register hdmi device\n" );
		return -EIO;
	}

	/* TODO : initialize your module not specific minor device */
	/* END */

	for ( i=0; i<HDMI_MAX_DEVICE; i++ )
	{
		/* initialize cdev structure with predefined variable */
		dev = MKDEV( g_hdmi_major, g_hdmi_minor+i );
		cdev_init( &(g_hdmi_device[i].cdev), &g_hdmi_fops );
		g_hdmi_device[i].devno		= dev;
		g_hdmi_device[i].cdev.owner = THIS_MODULE;
		g_hdmi_device[i].cdev.ops   = &g_hdmi_fops;

		/* TODO: initialize minor device */
		/* END */

		err = cdev_add (&(g_hdmi_device[i].cdev), dev, 1 );

		if (err)
		{
			HDMI_ERROR("error (%d) while adding hdmi device (%d.%d)\n", err, MAJOR(dev), MINOR(dev) );
			return -EIO;
		}
		OS_CreateDeviceClass ( g_hdmi_device[i].devno, "%s%d", HDMI_MODULE, i );
	}

	/* initialize proc system */
	HDMI_PROC_Init ( );

	HDMI_NOTI("hdmi device initialized\n");

	return 0;
}

void HDMI_Cleanup(void)
{
	int i;
	dev_t dev = MKDEV( g_hdmi_major, g_hdmi_minor );

#ifdef KDRV_CONFIG_PM
	// added by SC Jung for quick booting
	platform_driver_unregister(&hdmi_driver);
	platform_device_unregister(&hdmi_device);
#endif

	/* cleanup proc system */
	HDMI_PROC_Cleanup( );

	/* remove all minor devicies and unregister current device */
	for ( i=0; i<HDMI_MAX_DEVICE;i++)
	{
		/* TODO: cleanup each minor device */
		/* END */
		cdev_del( &(g_hdmi_device[i].cdev) );
	}

	/* TODO : cleanup your module not specific minor device */
	unregister_chrdev_region(dev, HDMI_MAX_DEVICE );

	OS_Free( g_hdmi_device );
}

/**
 * open handler for hdmi device
 *
 */
	static int
HDMI_Open(struct inode *inode, struct file *filp)
{
	int					major,minor;
	struct cdev*    	cdev;
	HDMI_DEVICE_T*	my_dev;

	cdev	= inode->i_cdev;
	my_dev	= container_of ( cdev, HDMI_DEVICE_T, cdev);

	/* TODO : add your device specific code */
	if( my_dev->dev_open_count == 0 )
	{
		//reset initialized value
		my_dev->dev_initialized = 0;

		//HDMI_Initialize();
	}
	/* END */

	my_dev->dev_open_count++;
	filp->private_data = my_dev;

	/* some debug */
	major = imajor(inode);
	minor = iminor(inode);

	//HDMI_Initialize(NULL);
	OS_InitMutex(&g_HDMI_Sema, OS_SEM_ATTR_DEFAULT);


	HDMI_NOTI("hdmi device opened (%d:%d)\n", major, minor);
#ifdef KDRV_CONFIG_PM	// added by SC Jung for quick booting
  //LGE_UPDATE_S shinhoe.kim@lge.com
  g_HDMI_Open_done = true;
  //LGE_UPDATE_E shinhoe.kim@lge.com
#endif

	return 0;
}

/**
 * release handler for hdmi device
 *
 */
static int HDMI_Close(struct inode *inode, struct file *file)
{
	int					major,minor;
	HDMI_DEVICE_T*	my_dev;
	struct cdev*		cdev;

	cdev	= inode->i_cdev;
	my_dev	= container_of ( cdev, HDMI_DEVICE_T, cdev);

	if ( my_dev->dev_open_count > 0 )
	{
		--my_dev->dev_open_count;
	}

	/* TODO : add your device specific code */
	if ( my_dev->dev_open_count <= 0 )
	{
		HDMI_exit();
	}
	/* END */

	/* some debug */
	major = imajor(inode);
	minor = iminor(inode);
	HDMI_NOTI("hdmi device closed (%d:%d)\n", major, minor);
	return 0;
}

/**
 * ioctl handler for hdmi device.
 *
 *
 * note: if you have some critial data, you should protect them using semaphore or spin lock.
 */
static long HDMI_Ioctl ( struct file *filp, unsigned int cmd, unsigned long arg )
{
	int err = 0, ret = 0;

	HDMI_DEVICE_T*	my_dev;
	struct cdev*		cdev;

	LX_HDMI_PHY_REG_WRITE_T *reg;

	UINT32 temp;
	UINT32 temp_arg;

#ifdef CONFIG_LX_CHIP_EMUL
	/* HDMI device can't support M14A0 emulation */
	#if CONFIG_LX_CHIP_EMUL==0x14A0
	HDMI_ERROR("HDMI device can't support 0x%x emulation\n", lx_chip_rev() ); return RET_ERROR;
	#endif
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)
	/*
	 * get current hdmi device object
	 */
	cdev	= inode->i_cdev;
	my_dev	= container_of ( cdev, HDMI_DEVICE_T, cdev);
#else
	my_dev	= (HDMI_DEVICE_T *)filp->private_data;
	cdev	= &my_dev->cdev;
#endif

	/*
	 * check if IOCTL command is valid or not.
	 * - if magic value doesn't match, return error (-ENOTTY)
	 * - if command is out of range, return error (-ENOTTY)
	 *
	 * note) -ENOTTY means "Inappropriate ioctl for device.
	 */
	if (_IOC_TYPE(cmd) != HDMI_IOC_MAGIC)
	{
		HDMI_WARN("invalid magic. magic=0x%02X\n", _IOC_TYPE(cmd) );
		return -ENOTTY;
	}
	if (_IOC_NR(cmd) > HDMI_IOC_MAXNR)
	{
		HDMI_WARN("out of ioctl command. cmd_idx=%d\n", _IOC_NR(cmd) );
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
		HDMI_WARN("memory access error. cmd_idx=%d, rw=%c%c, memptr=%p\n",
				_IOC_NR(cmd),
				(_IOC_DIR(cmd) & _IOC_READ)? 'r':'-',
				(_IOC_DIR(cmd) & _IOC_WRITE)? 'w':'-',
				(void*)arg );
		return -EFAULT;
	}

	HDMI_PRINT("cmd = %08X (cmd_idx=%d)\n", cmd, _IOC_NR(cmd) );

	switch(cmd)
	{
		case HDMI_IOW_INIT:
			{
				if( my_dev->dev_initialized == 0 )
				{
					LX_HDMI_INIT_T *init;
					init = (LX_HDMI_INIT_T *)kmalloc(sizeof(LX_HDMI_INIT_T),GFP_KERNEL);
					ret = copy_from_user((void *)init, (void *)arg, sizeof(LX_HDMI_INIT_T));

					if(ret != 0)	break;

					ret = HDMI_Initialize(init);

					kfree(init);

					//set my_dev->dev_initialized value
					if(ret == 0)
						my_dev->dev_initialized = 1;
				}
				else
				{
					HDMI_DEBUG("[hdmi_drv] HDMI_IO_INIT IOCTL : ignore multiple initialization \n");
					ret = 0;
				}
				HDMI_TRACE("[hdmi_drv] HDMI_IO_INIT IOCTL\n");
			}
			break;

		case HDMI_IOW_SET_PORT:
			{
				UINT32 *port;
				port = (UINT32 *)kmalloc(sizeof(UINT32) , GFP_KERNEL);
				ret = copy_from_user((void *)port, (void *)arg, sizeof(UINT32));
				ret = HDMI_SetPort(port);	// set hot plug detect
				kfree(port);
				HDMI_TRACE("[hdmi_drv]HDMI_IOW_SET_PORT IOCTL\n");
			}
			break;
		case HDMI_IOR_GET_MODE:
			{
				LX_HDMI_MODE_T *mode;
				mode = (LX_HDMI_MODE_T *)kmalloc(sizeof(LX_HDMI_MODE_T) , GFP_KERNEL);
				ret = copy_from_user((void *)mode, (void *)arg, sizeof(LX_HDMI_MODE_T));

				ret = HDMI_GetMode(mode);
				ret = copy_to_user((void *)arg, (void *)mode, sizeof(LX_HDMI_MODE_T));

				kfree(mode);
				HDMI_TRACE("[hdmi_drv] HDMI_IOR_GET_MODE IOCTL\n");
			}
			break;

		case HDMI_IOR_GET_ASPECTRATIO:
			{
				LX_HDMI_ASPECTRATIO_T *ratio;
				ratio = (LX_HDMI_ASPECTRATIO_T *)kmalloc(sizeof(LX_HDMI_ASPECTRATIO_T) , GFP_KERNEL);

				ret = HDMI_GetAspectRatio(ratio);
				ret = copy_to_user((void *)arg, (void *)ratio, sizeof(LX_HDMI_ASPECTRATIO_T));

				kfree(ratio);
				HDMI_TRACE("[hdmi_drv] HDMI_IOR_GET_ASPECTRATIO IOCTL\n");
			}
			break;

		case HDMI_IOR_GET_COLOR_DOMAIN:
			{
				LX_HDMI_COLOR_DOMAIN_T *color;
				color = (LX_HDMI_COLOR_DOMAIN_T *)kmalloc(sizeof(LX_HDMI_COLOR_DOMAIN_T) , GFP_KERNEL);
				ret = HDMI_GetColorDomain(color);
				ret = copy_to_user((void *)arg, (void *)color, sizeof(LX_HDMI_COLOR_DOMAIN_T));
				kfree(color);
				HDMI_TRACE("[hdmi_drv] HDMI_IOW_SET_COLOR_DOMAIN IOCTL\n");
			}
			break;

		case HDMI_IOR_GET_TIMING_INFO:
			{
				LX_HDMI_TIMING_INFO_T *info;

				info = (LX_HDMI_TIMING_INFO_T *)kmalloc(sizeof(LX_HDMI_TIMING_INFO_T) , GFP_KERNEL);

				ret |= copy_from_user((void *)info, (void *)arg, sizeof(LX_HDMI_TIMING_INFO_T));
				ret |= HDMI_GetTimingInfo(info);
				ret = copy_to_user((void *)arg, (void *)info, sizeof(LX_HDMI_TIMING_INFO_T));

				kfree(info);
				HDMI_TRACE("[hdmi_drv] HDMI_IOR_GET_TIMING_INFO IOCTL\n");
			}
			break;

		case HDMI_IOR_GET_AVI_INFO:
			{
				LX_HDMI_INFO_PACKET_T *avi_packet;
				avi_packet = (LX_HDMI_INFO_PACKET_T *)kmalloc(sizeof(LX_HDMI_INFO_PACKET_T) , GFP_KERNEL);

				ret |= copy_from_user((void *)avi_packet, (void *)arg, sizeof(LX_HDMI_INFO_PACKET_T));
				ret |= HDMI_GetAviPacket(avi_packet);
				ret = copy_to_user((void *)arg, (void *)avi_packet, sizeof(LX_HDMI_INFO_PACKET_T));

				kfree(avi_packet);
				HDMI_TRACE("[hdmi_drv] HDMI_IOR_GET_AVI_INFO IOCTL\n");
			}
			break;

		case HDMI_IOR_GET_VSI_INFO:
			{
				LX_HDMI_INFO_PACKET_T *vsi_packet;

				vsi_packet = (LX_HDMI_INFO_PACKET_T *)kmalloc(sizeof(LX_HDMI_INFO_PACKET_T) , GFP_KERNEL);

				ret |= copy_from_user((void *)vsi_packet, (void *)arg, sizeof(LX_HDMI_INFO_PACKET_T));
				ret |= HDMI_GetVsiPacket(vsi_packet);
				ret = copy_to_user((void *)arg, (void *)vsi_packet, sizeof(LX_HDMI_INFO_PACKET_T));

				kfree(vsi_packet);
				HDMI_TRACE("[hdmi_drv]HDMI_IOR_GET_VSI_INFO IOCTL\n");
			}
			break;

		case HDMI_IOR_GET_SPD_INFO:
			{

				LX_HDMI_INFO_PACKET_T *spd_packet;

				spd_packet = (LX_HDMI_INFO_PACKET_T *)kmalloc(sizeof(LX_HDMI_INFO_PACKET_T) , GFP_KERNEL);

				ret |= copy_to_user((void *)spd_packet, (void *)arg, sizeof(LX_HDMI_INFO_PACKET_T));
				ret |= HDMI_GetSpdPacket(spd_packet);
				ret = copy_to_user((void *)arg, (void *)spd_packet, sizeof(LX_HDMI_INFO_PACKET_T));

				kfree(spd_packet);
				HDMI_TRACE("[hdmi_drv]HDMI_IOR_GET_SPD_INFO\n");
			}
			break;

		case HDMI_IOR_GET_INFOFRAME:
			{
				LX_HDMI_INFO_PACKET_T *info_packet;

				info_packet = (LX_HDMI_INFO_PACKET_T *)kmalloc(sizeof(LX_HDMI_INFO_PACKET_T) , GFP_KERNEL);

				ret = HDMI_GetInfoPacket(info_packet);
				ret = copy_to_user((void *)arg, (void *)info_packet, sizeof(LX_HDMI_INFO_PACKET_T));

				kfree(info_packet);
				HDMI_TRACE("[hdmi_drv]HDMI_IOR_GET_SPD_INFO\n");

			}
			break;

		case HDMI_IOW_SET_HPD:
			{
				LX_HDMI_HPD_T *hpd;

				HDMI_TRACE("[hdmi_drv]HDMI_IOW_SET_HPD IOCTL\n");

				hpd = (LX_HDMI_HPD_T *)kmalloc(sizeof(LX_HDMI_HPD_T) , GFP_KERNEL);

				ret = copy_from_user((void *)hpd, (void *)arg, sizeof(LX_HDMI_HPD_T));
				ret = HDMI_SetHPD(hpd);	// set hot plug detect

				kfree(hpd);
			}
			break;

		case HDMI_IOW_SET_POWER:
			{
				UINT32 power;

				ret = copy_from_user((void *)&power, (void *)arg, sizeof(UINT32));

				ret = HDMI_PowerConsumption(power);

				HDMI_TRACE("[hdmi_drv]HDMI_IOW_SET_POWER\n");
			}
			break;

		case HDMI_IOR_GET_HDMI_STATUS:
			{
				LX_HDMI_STATUS_T *status;
				status = (LX_HDMI_STATUS_T *)kmalloc(sizeof(LX_HDMI_STATUS_T) , GFP_KERNEL);
				ret = HDMI_GetStatus(status);
				ret |= copy_to_user((void *)arg, (void *)status, sizeof(LX_HDMI_STATUS_T));
				kfree(status);
				HDMI_TRACE("[hdmi_drv] HDMI_IOR_GET_HDMI_STATUS IOCTL\n");
			}
			break;

		case HDMI_IOR_REG_RD:
			{
				HDMI_TRACE("HDMI_IOR_PHY_REG_RD IOCTL\n");
				ret = copy_from_user((void *)&temp_arg , (void *)arg , sizeof(UINT32));

				ret = HDMI_GetRegister(temp_arg, &temp);

				ret |= copy_to_user((void *)arg , (void *)&temp , sizeof(UINT32));

			}
			break;

		case HDMI_IOW_REG_WR:
			{
				HDMI_TRACE("HDMI_IOW_PHY_REG_WR IOCTL\n");
				reg = (LX_HDMI_PHY_REG_WRITE_T *)kmalloc(sizeof(LX_HDMI_PHY_REG_WRITE_T) , GFP_KERNEL);

				ret = copy_from_user((void *)reg , (void *)arg , sizeof(LX_HDMI_PHY_REG_WRITE_T));

				ret |= HDMI_SetRegister(reg->wr_addr , reg->wr_data);

				kfree(reg);

			}
			break;

		case HDMI_IOW_HDCP_ENABLE:
			{
				HDMI_TRACE("[hdmi_drv]HDMI_IOW_HDCP_ENABLE IOCTL\n");
				ret = 0;
			}
			break;

		case HDMI_IOW_SET_HDCP:
			{
				LX_HDMI_HDCP_T s_hdmi_hdcp_data;

				ret = copy_from_user((void *)&s_hdmi_hdcp_data, (void *)arg, sizeof(LX_HDMI_HDCP_T));

				ret |= HDMI_Write_HDCP_Key(s_hdmi_hdcp_data.u8HdcpKey );

				HDMI_TRACE("[hdmi_drv]HDMI_IOW_SET_HDCP IOCTL\n");
				ret = 0;
			}
			break;

		case HDMI_IOR_GET_HDCP:
			{
				LX_HDMI_HDCP_T s_hdmi_hdcp_data;

				ret = HDMI_Read_HDCP_Key(s_hdmi_hdcp_data.u8HdcpKey);
				ret |= copy_to_user((void *)arg, (void *)&s_hdmi_hdcp_data, sizeof(LX_HDMI_HDCP_T));

				HDMI_TRACE("[hdmi_drv]HDMI_IOW_GET_HDCP IOCTL\n");
				ret = 0;
			}
			break;

		case HDMI_IOR_GET_AUDIO_INFO:
			{
				LX_HDMI_AUDIO_INFO_T *pAudioInfo;

				pAudioInfo = (LX_HDMI_AUDIO_INFO_T *)kmalloc(sizeof(LX_HDMI_AUDIO_INFO_T), GFP_KERNEL);
				ret = copy_from_user((void *)pAudioInfo, (void *)arg, sizeof(LX_HDMI_AUDIO_INFO_T));

				ret = HDMI_GetAudioInfo(pAudioInfo);
				ret |= copy_to_user((void *)arg, (void *)pAudioInfo, sizeof(LX_HDMI_AUDIO_INFO_T));

				kfree(pAudioInfo);
				HDMI_TRACE("[hdmi_drv] HDMI_IOR_GET_AUDIO_INFO\n");
			}
			break;

		case HDMI_IOR_GET_COPY_INFO:
			{
				LX_HDMI_AUDIO_COPY_INFO_T *pCopyInfo;

				pCopyInfo = (LX_HDMI_AUDIO_COPY_INFO_T *)kmalloc(sizeof(LX_HDMI_AUDIO_COPY_INFO_T), GFP_KERNEL);
				ret = copy_from_user((void *)pCopyInfo, (void *)arg, sizeof(LX_HDMI_AUDIO_COPY_INFO_T));

				ret = HDMI_GetAudioCopyInfo(pCopyInfo);
				ret |= copy_to_user((void *)arg, (void *)pCopyInfo, sizeof(LX_HDMI_AUDIO_COPY_INFO_T));

				kfree(pCopyInfo);
				HDMI_TRACE("[hdmi_drv] HDMI_IOR_GET_COPY_INFO\n");
			}
			break;

		case HDMI_IOW_SET_ARC:
			{
				LX_HDMI_ARC_CTRL_T *arc;

				arc = (LX_HDMI_ARC_CTRL_T *)kmalloc(sizeof(LX_HDMI_ARC_CTRL_T) , GFP_KERNEL);
				ret = copy_from_user((void *)arc, (void *)arg, sizeof(LX_HDMI_ARC_CTRL_T));

				ret |= HDMI_SetArc(arc);
				kfree(arc);
				HDMI_TRACE("[hdmi_drv]HDMI_IOW_SET_ARC\n");
			}
			break;

		case HDMI_IOW_SET_MUTE:
			{
				LX_HDMI_MUTE_CTRL_T *mute;
				mute = (LX_HDMI_MUTE_CTRL_T *)kmalloc(sizeof(LX_HDMI_MUTE_CTRL_T) , GFP_KERNEL);
				ret = copy_from_user((void *)mute, (void *)arg, sizeof(LX_HDMI_MUTE_CTRL_T));

				ret |= HDMI_SetMute(mute);
				kfree(mute);
				HDMI_TRACE("[hdmi_drv]HDMI_IOW_SET_MUTE\n");
			}
			break;

		case HDMI_IOR_DEBUG_GET_AUDIO_INFO:
			{
				LX_HDMI_DEBUG_AUDIO_INFO_T *pAudioDebugInfo;

				pAudioDebugInfo = (LX_HDMI_DEBUG_AUDIO_INFO_T *)kmalloc(sizeof(LX_HDMI_DEBUG_AUDIO_INFO_T), GFP_KERNEL);
				ret = copy_from_user((void *)pAudioDebugInfo, (void *)arg, sizeof(LX_HDMI_DEBUG_AUDIO_INFO_T));

				ret = HDMI_GetAudioDebugInfo(pAudioDebugInfo);
				ret |= copy_to_user((void *)arg, (void *)pAudioDebugInfo, sizeof(LX_HDMI_DEBUG_AUDIO_INFO_T));

				kfree(pAudioDebugInfo);
				HDMI_TRACE("[hdmi_drv] HDMI_IOR_DEBUG_GET_AUDIO_INFO\n");
			}
			break;

		case HDMI_IOW_DEBUG_PRINT_CTRL:
			{
				LX_HDMI_DEBUG_CTRL_T	i_stDebugCtrl;

				memset(&i_stDebugCtrl, 0x0, sizeof(LX_HDMI_DEBUG_CTRL_T));

				HDMI_TRACE("HDMI_IOW_DEBUG_PRINT_CTRL\n");

				if(copy_from_user( &i_stDebugCtrl, (void __user *)arg, sizeof(LX_HDMI_DEBUG_CTRL_T)))
					return RET_ERROR;

				if( (i_stDebugCtrl.printType > LX_HDMI_MAX) || (i_stDebugCtrl.printColor >= 8) )
				{
					HDMI_ERROR("Check Argument\n" );
					return RET_ERROR;
				}
				if ( i_stDebugCtrl.printType == LX_HDMI_MAX) {
					if( (_g_hdmi_print_mask != 0) && ( i_stDebugCtrl.printColor != DBG_COLOR_NONE) ) {
						HDMI_DEBUG("HDMI SetModuleMask fd[%d], mask [0x%x]\n",  g_hdmi_debug_fd, _g_hdmi_print_mask );
						OS_DEBUG_SetModuleMask( g_hdmi_debug_fd, _g_hdmi_print_mask );
					}
					else if( i_stDebugCtrl.printColor == DBG_COLOR_NONE) {
						LOGM_ObjMaskGet(g_hdmi_debug_fd, &_g_hdmi_print_mask);
						HDMI_DEBUG("HDMI GetModuleMask fd[%d], mask [0x%x]\n",  g_hdmi_debug_fd, _g_hdmi_print_mask );
//						_g_hdmi_print_mask = OS_DEBUG_GetModuleMask(g_hdmi_debug_fd);
//						_g_hdmi_print_mask = _g_debug_manage_table[module_fd].print_mask;
						OS_DEBUG_SetModuleMask( g_hdmi_debug_fd, 0 );
					}
					ret = 0;

					break;
				}

				if ( i_stDebugCtrl.printColor == DBG_COLOR_NONE)
				{
					OS_DEBUG_DisableModuleByIndex(g_hdmi_debug_fd, i_stDebugCtrl.printType);
					HDMI_ERROR("HDMI_PRINT_TYPE[%s] is disabled.\n", ui8HDMIPrintType[i_stDebugCtrl.printType]);
				}
				else
				{
					OS_DEBUG_EnableModuleByIndex(g_hdmi_debug_fd, i_stDebugCtrl.printType, i_stDebugCtrl.printColor);
					HDMI_ERROR("HDMI_PRINT_TYPE[%s] is enabled Color is [%s].\n", ui8HDMIPrintType[i_stDebugCtrl.printType], ui8HDMIPrintColor[i_stDebugCtrl.printColor - 1]);
				}
				ret = 0;
			}
			break;

		case HDMI_IOW_WRITE_EDID_DATA:
			{
				LX_HDMI_EDID_DATA_T s_hdmi_edid_data;

				ret = copy_from_user((void *)&s_hdmi_edid_data, (void *)arg, sizeof(LX_HDMI_EDID_DATA_T));
				ret |= HDMI_Write_EDID_Data(s_hdmi_edid_data.prt_sel, s_hdmi_edid_data.edid_data_array );

				HDMI_TRACE("[hdmi_drv] HDMI_IOW_WRITE_EDID_DATA\n");
			}
			break;
		case HDMI_IOR_READ_EDID_DATA:
			{
				LX_HDMI_EDID_DATA_T s_hdmi_edid_data;

				ret = copy_from_user((void *)&s_hdmi_edid_data, (void *)arg, sizeof(LX_HDMI_EDID_DATA_T));
				ret |= HDMI_Read_EDID_Data(s_hdmi_edid_data.prt_sel, s_hdmi_edid_data.edid_data_array);
				ret |= copy_to_user((void *)arg, (void *)&s_hdmi_edid_data, sizeof(LX_HDMI_EDID_DATA_T));

				HDMI_TRACE("[hdmi_drv] HDMI_IOR_READ_EDID_DATA\n");
			}
			break;
		case HDMI_IOR_GET_PHY_STATUS:
			{
				LX_HDMI_PHY_INFORM_T s_hdmi_phy_status;

				ret = HDMI_Get_Phy_Status(&s_hdmi_phy_status);
				ret |= copy_to_user((void *)arg, (void *)&s_hdmi_phy_status, sizeof(LX_HDMI_PHY_INFORM_T));

				HDMI_TRACE("[hdmi_drv] HDMI_IOR_GET_PHY_STATUS\n");
			}
			break;
		case HDMI_IOW_THREAD_CONTROL:
			{
				UINT32 sleep_on;
				ret = copy_from_user((void *)&sleep_on, (void *)arg, sizeof(UINT32));
				ret |= HDMI_Thread_Control(sleep_on);	// set hot plug detect
				HDMI_TRACE("[hdmi_drv]HDMI_IOW_THREAD_CONTROL IOCTL\n");
			}
			break;
		case HDMI_IOR_GET_AKSV_DATA:
			{
				LX_HDMI_AKSV_DATA_T s_hdmi_aksv_data;

				ret = copy_from_user((void *)&s_hdmi_aksv_data, (void *)arg, sizeof(LX_HDMI_AKSV_DATA_T));
				ret |= HDMI_Get_Aksv_Data(s_hdmi_aksv_data.prt_sel, s_hdmi_aksv_data.Aksv_array);
				ret |= copy_to_user((void *)arg, (void *)&s_hdmi_aksv_data, sizeof(LX_HDMI_AKSV_DATA_T));

				HDMI_TRACE("[hdmi_drv] HDMI_IOR_GET_AKSV_DATA\n");
			}
			break;
		case HDMI_IOW_SEND_RCP:
			{
				UINT8	rcp_data;
				ret = copy_from_user((void *)&rcp_data, (void *)arg, sizeof(UINT8));
				ret |= HDMI_MHL_Send_RCP(rcp_data);
				HDMI_TRACE("[hdmi_drv]HDMI_IOW_SEND_RCP IOCTL\n");
			}
			break;
		case HDMI_IOW_SEND_WRITEBURST:
			{
				LX_HDMI_MHL_WRITEBURST_DATA_T sWriteburst_data;

				ret = copy_from_user((void *)&sWriteburst_data, (void *)arg, sizeof(LX_HDMI_MHL_WRITEBURST_DATA_T));
				ret |= HDMI_MHL_Send_WriteBurst( &sWriteburst_data );

				HDMI_TRACE("[hdmi_drv] HDMI_IOW_SEND_WRITEBURST\n");
			}
			break;
		case HDMI_IOR_READ_WRITEBURST:
			{
				LX_HDMI_MHL_WRITEBURST_DATA_T sWriteburst_data;

				ret = HDMI_MHL_Read_WriteBurst( &sWriteburst_data );
				ret |= copy_to_user((void *)arg, (void *)&sWriteburst_data, sizeof(LX_HDMI_MHL_WRITEBURST_DATA_T));

				HDMI_TRACE("[hdmi_drv] HDMI_IOR_READ_WRITEBURST\n");
			}
			break;
		case HDMI_IOW_CALL_TYPE:
			{
				LX_HDMI_CALL_TYPE_T	hdmi_call_type;
				ret = copy_from_user((void *)&hdmi_call_type, (void *)arg, sizeof(LX_HDMI_CALL_TYPE_T));
				ret |= HDMI_Module_Call_Type(hdmi_call_type);
				HDMI_TRACE("[hdmi_drv]HDMI_IOW_CALL_TYPE IOCTL\n");
			}
			break;
		case HDMI_IOW_RESET_INTERNAL_EDID:
			{
				LX_HDMI_RESET_INTERNAL_EDID_T s_hdmi_rst_int_edid;
				ret = copy_from_user((void *)&s_hdmi_rst_int_edid, (void *)arg, sizeof(LX_HDMI_RESET_INTERNAL_EDID_T));
				ret |= HDMI_Reset_Internal_Edid(s_hdmi_rst_int_edid.prt_num, s_hdmi_rst_int_edid.edid_resetn);
				HDMI_TRACE("[hdmi_drv]HDMI_IOW_RESET_INTERNAL_EDID IOCTL\n");
			}
			break;
		case HDMI_IOW_ENABLE_EXTERNAL_DDC_ACCESS:
			{
				LX_HDMI_EXT_DDC_ACCESS_T s_hdmi_ext_ddc_access;
				ret = copy_from_user((void *)&s_hdmi_ext_ddc_access, (void *)arg, sizeof(LX_HDMI_EXT_DDC_ACCESS_T));
				ret |= HDMI_Enable_External_DDC_Access(s_hdmi_ext_ddc_access.prt_num, s_hdmi_ext_ddc_access.enable);
				HDMI_TRACE("[hdmi_drv]HDMI_IOW_ENABLE_EXTERNAL_DDC_ACCESS IOCTL\n");
			}
			break;
		case HDMI_IOR_RECEIVE_RCP:
			{
				LX_HDMI_RCP_RECEIVE_T s_MHL_RCP_rcv_msg;

				ret = copy_from_user((void *)&s_MHL_RCP_rcv_msg, (void *)arg, sizeof(LX_HDMI_RCP_RECEIVE_T));
				ret |= HDMI_MHL_Receive_RCP(&s_MHL_RCP_rcv_msg);
				ret |= copy_to_user((void *)arg, (void *)&s_MHL_RCP_rcv_msg, sizeof(LX_HDMI_RCP_RECEIVE_T));

				HDMI_TRACE("[hdmi_drv] HDMI_IOR_RECEIVE_RCP\n");
			}
			break;
		case HDMI_IOR_GET_DRIVER_STATUS:
			{
				LX_HDMI_DRIVER_STATUS_T s_hdmi_driver_status;

				ret = HDMI_Get_Driver_Status(&s_hdmi_driver_status);
				ret |= copy_to_user((void *)arg, (void *)&s_hdmi_driver_status, sizeof(LX_HDMI_DRIVER_STATUS_T));

				HDMI_TRACE("[hdmi_drv] HDMI_IOR_GET_DRIVER_STATUS\n");
			}
			break;
		case HDMI_IOW_KEY_CONTROL:
			{
				LX_HDMI_CONTROL_KEY_TYPE_T key_control_cmd;

				ret = copy_from_user((void *)&key_control_cmd, (void *)arg, sizeof(LX_HDMI_CONTROL_KEY_TYPE_T));
				ret = HDMI_Key_Control(key_control_cmd);

				HDMI_TRACE("[hdmi_drv] HDMI_IOW_KEY_CONTROL\n");
			}
			break;
		case HDMI_IOW_DEBUG_HDMI_DRV:
			{
				LX_HDMI_DRV_DEBUG_T s_hdmi_drv_debug;

				ret = copy_from_user((void *)&s_hdmi_drv_debug, (void *)arg, sizeof(LX_HDMI_DRV_DEBUG_T));
				ret |= HDMI_Driver_Debug(&s_hdmi_drv_debug);
				ret |= copy_to_user((void *)arg, (void *)&s_hdmi_drv_debug, sizeof(LX_HDMI_DRV_DEBUG_T));

				HDMI_TRACE("[hdmi_drv] HDMI_IOW_DEBUG_HDMI_DRV\n");
			}
			break;
		case HDMI_IOW_ENABLE_MHL:
			{
				BOOLEAN bMHLEnable;

				ret = copy_from_user((void *)&bMHLEnable, (void *)arg, sizeof(BOOLEAN));

				ret = HDMI_EnableMHL(bMHLEnable);

				HDMI_TRACE("[hdmi_drv]HDMI_IOW_ENABLE_MHL\n");
			}
			break;

		case HDMI_IOW_HPD_HDCP22_CONTROL:
			{
				LX_HDMI_HPD_HDCP22_CONTROL_T hpd_hdcp22_control;

				ret = copy_from_user((void *)&hpd_hdcp22_control, (void *)arg, sizeof(LX_HDMI_HPD_HDCP22_CONTROL_T));

				ret = HDMI_HPD_HDCP22_Control(hpd_hdcp22_control);

				HDMI_TRACE("[hdmi_drv]HDMI_IOW_HPD_HDCP22_CONTROL\n");
			}
			break;

		case HDMI_IOW_SET_MHL_ODT:
			{
				LX_HDMI_MHL_ODT_IMPEDANCE_T odt_value;

				ret = copy_from_user((void *)&odt_value, (void *)arg, sizeof(LX_HDMI_MHL_ODT_IMPEDANCE_T));

				ret = HDMI_Set_MHL_ODT(odt_value);

				HDMI_TRACE("[hdmi_drv]HDMI_IOW_SET_MHL_ODT\n");
			}
			break;
			/*
			   case HDMI_IOW_INTERFACE:
			   {
			   HDMI_PRINT("ADC interface with register\n");
			   reg = (S_HDMI_INTERFACE_REG *)kmalloc(sizeof(S_HDMI_INTERFACE_REG),GFP_KERNEL);

			   ret = copy_from_user((void *)reg, (void *)arg, sizeof(S_HDMI_INTERFACE_REG));

			   HDMI_RunRegister(reg);

			   kfree(reg);
			   ret = 0;
			   }
			   break;
			 */
		default:
			{
				/* redundant check but it seems more readable */
				ret = -ENOTTY;
			}
	}

	return ret;
}


#ifndef KDRV_GLOBAL_LINK
#if defined(CONFIG_LG_BUILTIN_KDRIVER) && defined(CONFIG_LGSNAP)
user_initcall_grp("kdrv",HDMI_Init);
#else
module_init(HDMI_Init);
#endif
module_exit(HDMI_Cleanup);

MODULE_AUTHOR("LGE");
MODULE_DESCRIPTION("base driver");
MODULE_LICENSE("GPL");
#endif

/** @} */

