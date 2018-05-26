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
 *  main driver implementation for pvr device.
 *	pvr device will teach you how to make device driver with new platform.
 *
 *  author		murugan.d (murugan.d@lge.com)
 *  version		1.0
 *  date		2010.02.05
 *  note		Additional information.
 *
 *  @addtogroup lg1150_pvr
 *	@{
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/
#undef	SUPPORT_PVR_DEVICE_READ_WRITE_FOPS

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#ifdef KDRV_CONFIG_PM	// added by SC Jung for quick booting
#include <linux/platform_device.h>
#endif
#include <linux/version.h>
#include <asm/cacheflush.h>
#include <linux/dma-mapping.h>
#include <linux/dmaengine.h>

#include "base_device.h"
#include "pvr_impl.h"
#include "pvr_drv.h"
#include "pvr_core.h"
#include "pvr_isr.h"
#include "pvr_pie.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define PVR_COPY_FROM_USER(to,from,n,fail_action) 					\
	do {															\
		if (copy_from_user((void*)to, (void __user *)from, n)) {	\
			PVR_PRINT_ERROR("ioctl: copy_from_user"); 				\
			fail_action;											\
		}															\
	} while(0)

#define PVR_COPY_TO_USER(to,from,n,fail_action)						\
	do {															\
		if (copy_to_user((void __user*)to, (void *)from, n)) {		\
			PVR_PRINT_ERROR("ioctl: copy_to_user");					\
			fail_action;											\
		}															\
	} while(0)


/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/

/**
 *	main control block for pvr device.
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
	PVR_CTRL_T				*ctrl;
// END of device specific data
}
PVR_DEVICE_T;

#ifdef KDRV_CONFIG_PM
typedef struct
{
	// add here extra parameter
	bool	is_suspended;
}PVR_DRVDATA_T;
#endif

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/
extern	void	PVR_PROC_Init(void);
extern	void	PVR_PROC_Cleanup(void);

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
	global Functions
----------------------------------------------------------------------------------------*/
int		PVR_Init(void);
void	PVR_Cleanup(void);

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/
int		g_pvr_debug_fd = -1;
int 	g_pvr_major = PVR_MAJOR;
int 	g_pvr_minor = PVR_MINOR;


/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/
static int      PVR_Open(struct inode *, struct file *);
static int      PVR_Close(struct inode *, struct file *);
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)
static int PVR_Ioctl (struct inode *, struct file *, unsigned int, unsigned long );
#else
static long PVR_Ioctl (struct file *filp, unsigned int cmd, unsigned long arg );
#endif
static int 	PVR_Mmap (struct file *filep, struct vm_area_struct *vma );
#ifdef SUPPORT_PVR_DEVICE_READ_WRITE_FOPS
static ssize_t  PVR_Read(struct file *, char *, size_t, loff_t *);
static ssize_t  PVR_Write(struct file *, const char *, size_t, loff_t *);
#endif

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/

/**
 * main control block for pvr device
*/
static PVR_DEVICE_T*		g_pvr_device;

/**
 * file I/O description for pvr device
 *
*/
static struct file_operations g_pvr_fops =
{
	.open 	= PVR_Open,
	.release= PVR_Close,
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)
	.ioctl	= PVR_Ioctl,
#else
	.unlocked_ioctl	= PVR_Ioctl,
#endif
#ifdef SUPPORT_PVR_DEVICE_READ_WRITE_FOPS
	.read 	= PVR_Read,
	.write 	= PVR_Write,
#else
	.read	= NULL,
	.write	= NULL,
#endif
	.mmap	= PVR_Mmap,
};

/*========================================================================================
	Implementation Group
========================================================================================*/
#ifdef KDRV_CONFIG_PM   // added by SC Jung for quick booting
/**
 *
 * suspending module.
 *
 * @param   struct platform_device *pdev pm_message_t state
 * @return  int 0 : OK , -1 : NOT OK
 *
 */
static int PVR_suspend(struct platform_device *pdev, pm_message_t state)
{
#ifdef KDRV_CONFIG_PM_PVR
    PVR_DRVDATA_T *drv_data;
    drv_data = platform_get_drvdata(pdev);

    printk("PVR - suspend BEGIN\n");

    if ( drv_data->is_suspended == 1 )
    {
        printk("PVR - suspend FAIL\n");
        return -1;  //If already in suspend state, so ignore
    }

    // add here the suspend code

    drv_data->is_suspended = 1;
    printk("PVR - suspend OK\n");
#endif
    return 0;
}


/**
 *
 * resuming module.
 *
 * @param   struct platform_device *
 * @return  int 0 : OK , -1 : NOT OK
 *
 */
static int PVR_resume(struct platform_device *pdev)
{
#ifdef KDRV_CONFIG_PM_PVR
    PVR_DRVDATA_T *drv_data;
    drv_data = platform_get_drvdata(pdev);

    printk("PVR - resume BEGIN\n");

    if(drv_data->is_suspended == 0) return -1;

    // add here the resume code

    drv_data->is_suspended = 0;
    printk("PVR - resume OK\n");
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
 int  PVR_probe(struct platform_device *pdev)
{

	PVR_DRVDATA_T *drv_data;

	drv_data = (PVR_DRVDATA_T *)kmalloc(sizeof(PVR_DRVDATA_T) , GFP_KERNEL);

	// add here driver registering code & allocating resource code

	PVR_KDRV_LOG( PVR_TRACE ,"[%s] done probe\n", PVR_MODULE);
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
static int  PVR_remove(struct platform_device *pdev)
{
	PVR_DRVDATA_T *drv_data;

	// add here driver unregistering code & deallocating resource code

	drv_data = platform_get_drvdata(pdev);
	kfree(drv_data);

	PVR_KDRV_LOG( PVR_TRACE ,"released\n");

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
static void  PVR_release(struct device *dev)
{


	PVR_KDRV_LOG( PVR_TRACE ,"device released\n");
}

/*
 *	module platform driver structure
 */
static struct platform_driver pvr_driver =
{
	.probe          = PVR_probe,
	.suspend        = PVR_suspend,
	.remove         = PVR_remove,
	.resume         = PVR_resume,
	.driver         =
	{
		.name   = PVR_MODULE,
	},
};

static struct platform_device pvr_device = {
	.name = PVR_MODULE,
	.id = 0,
	.id = -1,
	.dev = {
		.release = PVR_release,
	},
};
#endif

/** Initialize the device environment before the real H/W initialization
 *
 *  @note main usage of this function is to initialize the HAL layer and memory size adjustment
 *  @note it's natural to keep this function blank :)
 */
void PVR_PreInit(void)
{
    PVR_CFG_PreInit();
}

int PVR_Init(void)
{
	int			i;
	int			err;
	dev_t		dev;

	/* Get the handle of debug output for pvr device.
	 *
	 * Most module should open debug handle before the real initialization of module.
	 * As you know, debug_util offers 4 independent debug outputs for your device driver.
	 * So if you want to use all the debug outputs, you should initialize each debug output
	 * using OS_DEBUG_EnableModuleByIndex() function.
	 */
	g_pvr_debug_fd = DBG_OPEN( PVR_MODULE );

	if ( g_pvr_debug_fd >= 0 )
	{
		OS_DEBUG_EnableModule ( g_pvr_debug_fd );
		OS_DEBUG_EnableModuleByIndex ( g_pvr_debug_fd, LX_LOGM_LEVEL_ERROR, DBG_COLOR_RED );	/* ERROR */
		OS_DEBUG_EnableModuleByIndex ( g_pvr_debug_fd, LX_LOGM_LEVEL_NOTI, DBG_COLOR_GREEN );	/* NOTI */
	}
       else
	{
		PVR_KDRV_LOG( PVR_ERROR, "PVR_MODULE DBG_OPEN failed\n" );
		return -EIO;
	}
	/* allocate main device handler, register current device.
	 *
	 * If devie major is predefined then register device using that number.
	 * otherwise, major number of device is automatically assigned by Linux kernel.
	 *
	 */
#ifdef KDRV_CONFIG_PM
	// added by SC Jung for quick booting
	if(platform_driver_register(&pvr_driver) < 0)
	{
		PVR_KDRV_LOG( PVR_ERROR ,"[%s] platform driver register failed\n",PVR_MODULE);

	}
	else
	{
		if(platform_device_register(&pvr_device))
		{
			platform_driver_unregister(&pvr_driver);
			PVR_KDRV_LOG( PVR_ERROR ,"[%s] platform device register failed\n",PVR_MODULE);
		}
		else
		{
			PVR_KDRV_LOG( PVR_TRACE ,"[%s] platform register done\n", PVR_MODULE);
		}


	}
#endif

	g_pvr_device = (PVR_DEVICE_T*)OS_KMalloc( sizeof(PVR_DEVICE_T)*PVR_MAX_DEVICE );

	if ( NULL == g_pvr_device )
	{
		PVR_KDRV_LOG( PVR_ERROR, "out of memory. can't allocate %d bytes\n", sizeof(PVR_DEVICE_T)* PVR_MAX_DEVICE );
		return -ENOMEM;
	}

	memset( g_pvr_device, 0x0, sizeof(PVR_DEVICE_T)* PVR_MAX_DEVICE );

	if (g_pvr_major)
	{
		dev = MKDEV( g_pvr_major, g_pvr_minor );
		err = register_chrdev_region(dev, PVR_MAX_DEVICE, PVR_MODULE );
	}
	else
	{
		err = alloc_chrdev_region(&dev, g_pvr_minor, PVR_MAX_DEVICE, PVR_MODULE );
		g_pvr_major = MAJOR(dev);
	}

	if ( err < 0 )
	{
		PVR_KDRV_LOG( PVR_ERROR, "can't register pvr device\n" );
		return -EIO;
	}

	err = PVR_InitDevice();
	PVR_CHECK_ERROR(err != 0, return err, "can't init device");

	/* END */

	for ( i=0; i<PVR_MAX_DEVICE; i++ )
	{
		/* initialize cdev structure with predefined variable */
		dev = MKDEV( g_pvr_major, g_pvr_minor+i );
		cdev_init( &(g_pvr_device[i].cdev), &g_pvr_fops );
		g_pvr_device[i].devno		= dev;
		g_pvr_device[i].cdev.owner = THIS_MODULE;
		g_pvr_device[i].cdev.ops   = &g_pvr_fops;

		/* TODO: initialize minor device */
		g_pvr_device[i].ctrl = PVR_GetCtrl(LX_PVR_CH_A+i);

		/* END */

		err = cdev_add (&(g_pvr_device[i].cdev), dev, 1 );

		if (err)
		{
			PVR_KDRV_LOG( PVR_ERROR, "error (%d) while adding pvr device (%d.%d)\n", err, MAJOR(dev), MINOR(dev) );
			return -EIO;
		}
        OS_CreateDeviceClass ( g_pvr_device[i].devno, "%s%d", PVR_MODULE, i );
	}

	/* initialize proc system */
	PVR_PROC_Init ( );

	PVR_KDRV_LOG( PVR_TRACE ,"pvr device initialized\n");

	return 0;
}

void PVR_Cleanup(void)
{
	int i;
	dev_t dev = MKDEV( g_pvr_major, g_pvr_minor );

#ifdef KDRV_CONFIG_PM
	// added by SC Jung for quick booting
	platform_driver_unregister(&pvr_driver);
	platform_device_unregister(&pvr_device);
#endif

	/* cleanup proc system */
	PVR_PROC_Cleanup( );

	/* remove all minor devicies and unregister current device */
	for ( i=0; i<PVR_MAX_DEVICE;i++)
	{
		/* TODO: cleanup each minor device */


		/* END */
		cdev_del( &(g_pvr_device[i].cdev) );
	}

	PVR_CleanupDevice();

	unregister_chrdev_region(dev, PVR_MAX_DEVICE );

	OS_Free( g_pvr_device );
}


UINT32 DVR_DEV_Install ( void )
{
	PVR_KDRV_LOG( PVR_TRACE ,"device install success\n" );
	return 0;
}

UINT32 DVR_DEV_UnInstall ( void )
{
	PVR_KDRV_LOG( PVR_TRACE ,"device un-install success\n" );
	return 0;
}


///////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * open handler for pvr device
 *
 */
static int
PVR_Open(struct inode *inode, struct file *filp)
{
    int					major,minor;
    struct cdev*    	cdev;
    PVR_DEVICE_T*	pdev;

    cdev	= inode->i_cdev;
    pdev	= container_of ( cdev, PVR_DEVICE_T, cdev);

    /* TODO : add your device specific code */

    if ( pdev->dev_open_count == 0 )
    {
		/* Device initializations only for the first time */
		DVR_DEV_Install ( );
    }
	/* END */

    pdev->dev_open_count++;
    filp->private_data = pdev;

	/* some debug */
    major = imajor(inode);
    minor = iminor(inode);
    PVR_KDRV_LOG( PVR_TRACE ,"device opened (%d:%d)\n", major, minor );

    return 0;
}

/**
 * release handler for pvr device
 *
 */
static int
PVR_Close(struct inode *inode, struct file *file)
{
    int					major,minor;
    PVR_DEVICE_T*	pdev;
    struct cdev*		cdev;

    cdev	= inode->i_cdev;
    pdev	= container_of ( cdev, PVR_DEVICE_T, cdev);

    if ( pdev->dev_open_count > 0 )
    {
        --pdev->dev_open_count;
    }

    /* TODO : add your device specific code */

	if (pdev->dev_open_count == 0)
	{
		/* Last device handle, so close the device */
		DVR_DEV_UnInstall();
	}

	/* END */


	/* some debug */
    major = imajor(inode);
    minor = iminor(inode);
    PVR_KDRV_LOG( PVR_TRACE ,"device closed (%d:%d)\n", major, minor );
    return 0;
}

/**
 * ioctl handler for pvr device.
 *
 *
 * note: if you have some critial data, you should protect them using semaphore or spin lock.
 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)
static int PVR_Ioctl ( struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg )
#else
static long PVR_Ioctl (struct file *filp, unsigned int cmd, unsigned long arg )
#endif
{
	int err = 0, ret = RET_ERROR;

	PVR_DEVICE_T*	pdev;
	struct cdev*		cdev;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,36)
	struct inode *inode = filp->f_path.dentry->d_inode;
#endif

	/*
	 * get current pvr device object
	 */
    cdev	= inode->i_cdev;
    pdev	= container_of ( cdev, PVR_DEVICE_T, cdev);

    /*
     * check if IOCTL command is valid or not.
     * - if magic value doesn't match, return error (-ENOTTY)
     * - if command is out of range, return error (-ENOTTY)
     *
     * note) -ENOTTY means "Inappropriate ioctl for device.
     */
    if (_IOC_TYPE(cmd) != PVR_IOC_MAGIC)
    {
    	DBG_PRINT_WARNING("invalid magic. magic=0x%02X\n", _IOC_TYPE(cmd) );
    	return -ENOTTY;
    }
    if (_IOC_NR(cmd) > PVR_IOC_MAXNR)
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

//	PVR_TRACE("cmd = %08X (cmd_idx=%d)\n", cmd, _IOC_NR(cmd) );

	switch(cmd)
	{
		case PVR_IOW_PANIC:
			{
				LX_PVR_PANIC_T stPanic;

				PVR_COPY_FROM_USER(&stPanic, arg, sizeof(LX_PVR_PANIC_T), return -EFAULT);

				panic("[MARINE] PVR PANIC!! Addr(0x%x) BitPos(0x%x) DataSize(%d) bitSize(0x%x)", stPanic.dataaddress, stPanic.startBitPos, stPanic.dataSize, stPanic.bitSize);
				break;
			}

		case PVR_IOW_INIT:
			{
				PVR_InitModule(pdev->ctrl);
				PVR_KDRV_LOG( PVR_TRACE ,"PVR_IOW_INIT ok\n");
				ret = 0;
			}
			break;

		case PVR_IOR_GET_HW_INFO:
		{
			LX_PVR_HW_INFO_T param;

			PVR_GetHwInfo(&param);
			PVR_COPY_TO_USER(arg, &param, sizeof(LX_PVR_HW_INFO_T), return -EFAULT);
			ret = RET_OK;
		}
		break;

		case PVR_IOW_TERMINATE:
			{
				PVR_Terminate ( );
				PVR_KDRV_LOG( PVR_TRACE ,"PVR_IOW_TERMINATE ok\n");
				ret = 0;
			}
			break;

		case PVR_IOW_DN_START:
			{
				PVR_CTRL_T *ctrl = pdev->ctrl;
				LX_PVR_START_T	stDnStart;

				PVR_COPY_FROM_USER(&stDnStart, arg, sizeof(LX_PVR_START_T), return -EFAULT);

				if ( stDnStart.bStart )
				{
					ret = PVR_StartDownload (ctrl);
					PVR_PIE_Start(ctrl);
				}
				else
				{
					PVR_PIE_Stop(ctrl);
					ret = PVR_StopDownload (ctrl);
				}
			}
			break;

		case PVR_IOW_STOP_DN_SDT:
			{
				PVR_StopDownloadSDT(pdev->ctrl);

				PVR_KDRV_LOG( PVR_TRACE ,"PVR_IOW_STOP_DN_SDT ok\n");
				ret = 0;
			}
			break;

		case PVR_IOW_DN_SET_BUF:
			{
				LX_PVR_SET_BUFFER_T	stSetBuf;

				PVR_COPY_FROM_USER(&stSetBuf, arg, sizeof(LX_PVR_SET_BUFFER_T), return -EFAULT);

				PVR_SetDownloadBuffer(pdev->ctrl, stSetBuf.uiBufAddr, stSetBuf.uiBufSize);

				ret = 0;
			}
			break;

		case PVR_IOR_DN_GET_WRITE_ADD:
			{
				PVR_CTRL_T *ctrl = pdev->ctrl;
				LX_PVR_DN_GET_WRITE_ADD_T	param;
				UINT32 wptr;

				PVR_GetDownloadWritePtr(ctrl, &wptr);
				param.streamBuffer.uiWriteAddr = wptr;
				param.streamBuffer.eStatus = ctrl->buf_state.dn;

				// TODO: What is this ?
				/* Update the DVR overflow status */
				if (ctrl->buf_state.dn == LX_PVR_BUF_STAT_FULL)
				{
					PVR_PRINT_ERROR("Overflow detected !! Overflow[%d]", ctrl->stats.dn_overflow);
					/* Reset the buffer state to normal */
					ctrl->buf_state.dn = LX_PVR_BUF_STAT_READY;
				}

				PVR_COPY_TO_USER(arg, &param, sizeof(LX_PVR_DN_GET_WRITE_ADD_T), return -EFAULT);

				ret = 0;
			}
			break;

		case PVR_IOR_GET_STREAM_INFO:
			{
			#if 0
				LX_PVR_STREAMINFO_T		stGetStreamInfo;

				stGetStreamInfo.bFoundSeqSPS	= gStreamInfo.bFoundSeqSPS;
				stGetStreamInfo.bitRate			= gStreamInfo.bitRate;
				stGetStreamInfo.frRate			= gStreamInfo.frRate;

				if (copy_to_user( (LX_PVR_STREAMINFO_T *)arg, &stGetStreamInfo,	sizeof(LX_PVR_STREAMINFO_T)))
				{
					PVR_KDRV_LOG( PVR_ERROR ,"ioctl: copy_to_user error !!! \n");
					return -EFAULT;
				}
				ret = 0;
			#else
				return -EFAULT;
			#endif
			}
			break;

			/*
			 * To set the user read pointer for Download stream and index buffers
			 */
		case PVR_IOW_DN_SET_READ_ADDR:
		{
			PVR_CTRL_T *ctrl = pdev->ctrl;
			LX_PVR_DN_SET_READ_ADD_T param;

			PVR_KDRV_LOG(PVR_MSG_IOCTL, "PVR_IOW_DN_SET_READ_ADDR");

			PVR_COPY_FROM_USER(&param, arg, sizeof(LX_PVR_DN_SET_READ_ADD_T), return -EFAULT);

			PVR_CHECK_ERROR(param.readAddr == 0, return -EFAULT, "Invalid addr");

			PVR_SetDownloadReadAddr (ctrl, param.readAddr);
			ret = 0;
		}
		break;

		case PVR_IOW_PIE_START:
		{
			PVR_KDRV_LOG( PVR_TRACE ,"PVR_IOW_PIE_START ok\n");
			ret = 0;
		}
		break;

		case PVR_IOW_PIE_SET_BUF:
		{
			PVR_KDRV_LOG( PVR_TRACE ,"PVR_IOW_PIE_SET_BUF ok\n");
			ret = 0;
		}
		break;

		case PVR_IOW_PIE_SET_PID:
			{
				LX_PVR_PIE_SET_PID_T	stSetPid;
				PVR_CTRL_T *ctrl = pdev->ctrl;

				PVR_COPY_FROM_USER(&stSetPid, arg, sizeof(LX_PVR_PIE_SET_PID_T), return -EFAULT);

#ifndef USE_NEW_PIE_EXTRACTION
				ctrl->ui32PieIndex = 0;	//Write index
				ctrl->ui32PieRdIndex = 0;	//Read index
#endif

				if ((stSetPid.uiDownChunkPktCount < LX_PVR_DN_MIN_PKT_COUNT) ||
					(stSetPid.uiDownChunkPktCount > LX_PVR_DN_MAX_PKT_COUNT))
				{
					/* If invalid Packet count is passed, then assume the default packet count of 384KB  */
					// jinhwan.bae Rage is 128 <   < 6144
					// if valid range, always 0x800 = 2048 packet * 192 = 384K
					stSetPid.uiDownChunkPktCount = LX_PVR_DN_PKT_COUNT_DEFAULT;
				}

				PVR_KDRV_LOG( PVR_PIE ,"PIE (%c)> PID[0x%x] Down Chunk packet count[%d] Chunk Size[0x%08X]\n",
					ctrl->ch ? 'B' : 'A', stSetPid.uiPid,
					stSetPid.uiDownChunkPktCount,
					stSetPid.uiDownChunkPktCount * 192 );

				/*
				 * Murugan-18.01.2011 - Configure the chunk size for download and PIE numbering
				 * This value will affect the macro DVR_DN_MIN_BUF_CNT. This macro is used to configure
				 * the PKT_LIM register which control the UNIT_BUFF Interrupt.
				 */
				// TODO: 다른 ioctl로 분리가 되어야 한다.
				PVR_SetDownloadUnitBufPktCnt(ctrl, stSetPid.uiDownChunkPktCount);


				PVR_SetDownloadVPID(ctrl, stSetPid.uiPid);

				PVR_PIE_InitTable(ctrl);
				PVR_PIE_SetType(ctrl, stSetPid.ePidType);

				PVR_KDRV_LOG( PVR_PIE ,"ePidType %d\n", stSetPid.ePidType);

				if(stSetPid.ePidType == LX_PVR_PIE_TYPE_MPEG2TS)
					ctrl->state.ePieState = LX_PVR_PIE_STATE_MP2;
				else
					ctrl->state.ePieState = LX_PVR_PIE_STATE_GSCD;

				PVR_KDRV_LOG( PVR_PIE ,"ctrl->state.ePieState %d\n", ctrl->state.ePieState);

				PVR_KDRV_LOG( PVR_TRACE ,"PVR_IOW_PIE_SET_PID ok\n");
				ret = 0;
			}
			break;

		case PVR_IOW_PIE_SET_CONFIG:
			{
				PVR_KDRV_LOG( PVR_TRACE ,"PVR_IOW_PIE_SET_CONFIG ok\n");
				ret = 0;
			}
			break;

		case PVR_IOR_PIE_GET_TABLE:
		{
			LX_PVR_PIE_GET_TABLE_T	param;
			LX_PVR_INDEX_T *index = NULL;
			int size;
			PVR_CTRL_T *ctrl = pdev->ctrl;

			PVR_COPY_FROM_USER(&param, arg, sizeof(LX_PVR_PIE_GET_TABLE_T), return -EFAULT);

			if(param.size > 1024) param.size = 1024;
			index = (LX_PVR_INDEX_T*)OS_Malloc(sizeof(LX_PVR_INDEX_T) * param.size);
			PVR_CHECK_ERROR(index == NULL, return -EFAULT, "can't alloc");

			size = PVR_PIE_GetTable(ctrl, &param.ts, index, param.size);
			if(size < 0) goto pie_get_table_exit;
			else if(size > 0)
			{
				PVR_COPY_TO_USER(param.index, index, sizeof(LX_PVR_INDEX_T)*size, goto pie_get_table_exit);
			}
			param.size = size;
			PVR_COPY_TO_USER(arg, &param, sizeof(LX_PVR_PIE_GET_TABLE_T), goto pie_get_table_exit);

			ret = 0;
pie_get_table_exit:
			if(index != NULL) OS_Free(index);

			break;
		}


		case PVR_IOR_PIE_GET_DATA:
			{
			#if 0
				PVR_CTRL_T *ctrl = pdev->ctrl;
				UINT32	ui32Index, ui32Count;
				LX_PVR_PIE_GET_DATA_T	stGetPieData;

				ui32Index = ctrl->pie_data.ui32PieWrIndex;

				for ( ui32Count = 0; ui32Count < ui32Index; ui32Count++)
				{
					stGetPieData.ui32PieData[ui32Count].ui32Val	= ctrl->pie_data.astPieData[ui32Count].ui32Val;
				}

				stGetPieData.ui32PieDataCnt = ui32Index;
				ctrl->pie_data.ui32PieWrIndex = 0;

				PVR_COPY_TO_USER(arg, &stGetPieData, sizeof(LX_PVR_PIE_GET_DATA_T), return -EFAULT);
			#endif
				return -EFAULT;
			}
			break;

		case PVR_IOW_UP_INIT:
			{
				LX_PVR_SET_BUFFER_T	stSetBuf;

				PVR_COPY_FROM_USER(&stSetBuf, arg, sizeof(LX_PVR_SET_BUFFER_T), return -EFAULT);

				PVR_SetUploadbuffer (pdev->ctrl, stSetBuf.uiBufAddr, stSetBuf.uiBufSize);

				PVR_KDRV_LOG( PVR_TRACE ,"PVR_IOW_UP_INIT ok\n");
				ret = 0;
			}
			break;

			//23.05.2010 - Based on discussion for trick mode/jump case to restart upload hw
		case PVR_IOW_UP_RESTART:
			{
				LX_PVR_SET_BUFFER_T	stSetBuf;

				PVR_COPY_FROM_USER(&stSetBuf, arg, sizeof(LX_PVR_SET_BUFFER_T), return -EFAULT);

				PVR_RestartUpload(pdev->ctrl, stSetBuf.uiBufAddr, stSetBuf.uiBufSize);

				PVR_KDRV_LOG( PVR_TRACE ,"PVR_IOW_UP_RESTART ok\n");
				ret = 0;
			}
			break;

		case PVR_IOW_UP_RESET:	//Restart purpose???
			{
				PVR_ReSetUpload(pdev->ctrl);

				PVR_KDRV_LOG( PVR_TRACE ,"PVR_IOW_UP_RESET ok\n");
				ret = 0;
			}
		break;

		case PVR_IOW_UP_UPLOAD_BUFFER:
			{
				LX_PVR_SET_BUFFER_T	stUpSetBuf;

				PVR_COPY_FROM_USER(&stUpSetBuf, arg, sizeof(LX_PVR_SET_BUFFER_T), return -EFAULT);

				//Set the upload write pointer with the new value
				PVR_UpUploadbuffer (pdev->ctrl, stUpSetBuf.uiBufAddr );

				PVR_KDRV_LOG( PVR_UPLOAD ,"^Y^WrAddr [0x%x]\n", stUpSetBuf.uiBufAddr);

				//PVR_TRACE("PVR_IOW_UP_UPLOAD_BUFFER ok\n");
				ret = 0;
			}
			break;

		case PVR_IOW_UP_START:
			{
				LX_PVR_START_T	stUpStart;
				UINT8			ui8PacketLen = 192;

				PVR_COPY_FROM_USER(&stUpStart, arg, sizeof(LX_PVR_START_T), return -EFAULT);

				//Murugan-8/12/2010 - Enable the below line, to select between 192 byte or 188byte packet streams
				//ePacketLen argument has to be valid in the START command
				ui8PacketLen = (stUpStart.ePacketLen == LX_PVR_STREAM_TYPE_192)? 192 : 188;
				if ( stUpStart.bStart )
				{
					PVR_StartUpload (pdev->ctrl, ui8PacketLen );
				}
				else
				{
					PVR_StopUpload(pdev->ctrl);
				}
				ret = 0;
			}
			break;

		case PVR_IOW_UP_PAUSE:
		{
			PVR_CTRL_T *ctrl = pdev->ctrl;
			LX_PVR_UP_PAUSE_T	stGetUpPause;

			PVR_COPY_FROM_USER(&stGetUpPause, arg, sizeof(LX_PVR_UP_PAUSE_T), return -EFAULT);

			ret = -1;
			if (stGetUpPause.ePlay == LX_PVR_UP_PAUSE)
			{
				ret = PVR_PauseUpload (ctrl);
			}
			else if (stGetUpPause.ePlay == LX_PVR_UP_RESUME)
			{
				ret = PVR_ResumeUpload (ctrl);
			}
			PVR_KDRV_LOG( PVR_TRACE ,"PVR_IOW_UP_PAUSE ok\n");

		}
		break;

		case PVR_IOR_UP_GET_STATE:
		{
			PVR_CTRL_T *ctrl = pdev->ctrl;
			LX_PVR_UP_GET_STATE_T	stGetUpState;

			/* Get the upload buffer read pointer */
			PVR_GetUploadReadAddr (
				ctrl,
				&stGetUpState.upBuffer.uiWriteAddr,
				&stGetUpState.upBuffer.uiReadAddr );

			//Give the current buffer status
			stGetUpState.upBuffer.eStatus = ctrl->buf_state.up;

			/* Copy the current upload driver state */
			stGetUpState.eUpState = ctrl->state.eUpState;

			PVR_COPY_TO_USER( (LX_PVR_UP_GET_STATE_T *)arg,	&stGetUpState, sizeof(LX_PVR_UP_GET_STATE_T), return -EFAULT);
			//PVR_TRACE("PVR_IOR_UP_GET_STATE ok\n");
			ret = 0;
		}
		break;

		case PVR_IOW_PRINT_CONTROL:
		{
			LX_PVR_PRINT_CONTROL_T	stPrntCtrl;
			UINT32	msk, ui32Count;

			PVR_COPY_FROM_USER(&stPrntCtrl, arg, sizeof(LX_PVR_PRINT_CONTROL_T), return -EFAULT);

			msk = stPrntCtrl.ui32PrintMask & 0xF;

			for ( ui32Count = 0; ui32Count < 4; ui32Count++ )
			{
				if ( msk & (1 << ui32Count) )
				{
					OS_DEBUG_EnableModuleByIndex ( g_pvr_debug_fd, ui32Count, DBG_COLOR_NONE );
				}
				else
				{
					OS_DEBUG_DisableModuleByIndex ( g_pvr_debug_fd, ui32Count);
				}
			}
			ret = 0;
		}
		break;

		case PVR_IOW_UP_MODE:
		{
			LX_PVR_TRICK_MODE_T	stTrickMode;

			PVR_COPY_FROM_USER(&stTrickMode, arg, sizeof(LX_PVR_TRICK_MODE_T), return -EFAULT);

			ret = PVR_SetUploadMode(pdev->ctrl, stTrickMode.eMode);
		}
		break;

		case PVR_IOW_UP_SET_SPEED:
		{
			LX_PVR_UP_SPEED_T param;

			PVR_COPY_FROM_USER(&param, arg, sizeof(LX_PVR_UP_SPEED_T), return -EFAULT);

			ret = PVR_SetUploadSpeed(pdev->ctrl, param.speed);
		}
		break;


		case PVR_IOW_MM_GET_INFO:
		{
			LX_PVR_GPB_INFO_T	stLXPvrGPBInfo;

			ret = PVR_GetMemoryInfo(&stLXPvrGPBInfo);

			PVR_COPY_TO_USER(arg, &stLXPvrGPBInfo, sizeof(LX_PVR_GPB_INFO_T), return -EFAULT);
		}
		break;

		case PVR_IOW_MM_CACHE_CTRL:
		{
			LX_PVR_CACHE_CTRL_T cache_ctrl;

			PVR_COPY_FROM_USER(&cache_ctrl, arg, sizeof(LX_PVR_CACHE_CTRL_T), return -EFAULT);

			switch (cache_ctrl.op)
			{
				case LX_PVR_CACHE_OP_INVAL:
#ifdef __aarch64__
					dma_map_single(NULL,(void*)cache_ctrl.vaddr, cache_ctrl.size, DMA_FROM_DEVICE);
#else
					dmac_map_area((void*)cache_ctrl.vaddr, cache_ctrl.size, DMA_FROM_DEVICE);//v7_dma_inv_range
					outer_inv_range((phys_addr_t)cache_ctrl.paddr, (phys_addr_t)cache_ctrl.paddr + cache_ctrl.size);
#endif
					break;

				case LX_PVR_CACHE_OP_CLEAN:
#ifdef __aarch64__
					dma_map_single(NULL,(void*)cache_ctrl.vaddr, cache_ctrl.size, DMA_TO_DEVICE);
#else
					dmac_map_area((void*)cache_ctrl.vaddr, cache_ctrl.size, DMA_TO_DEVICE);//v7_dma_clean_range
					outer_clean_range((phys_addr_t)cache_ctrl.paddr, (phys_addr_t)cache_ctrl.paddr + cache_ctrl.size);
#endif
					break;

				default:
					PVR_PRINT_ERROR("invalid cache op 0x%x\n", cache_ctrl.op );
			}

			ret = RET_OK; /* all work done */
		}
		break;

		case PVR_IOR_GET_REG:
		{
			LX_PVR_GET_REG_T params;

			PVR_COPY_FROM_USER(&params, arg, sizeof(LX_PVR_GET_REG_T), return -EFAULT);
			ret = PVR_GetRegister(&params);
			PVR_COPY_TO_USER(arg, &params, sizeof(LX_PVR_GET_REG_T), return -EFAULT);
		}
		break;

		case PVR_IOW_SET_REG:
		{
			LX_PVR_SET_REG_T params;

			PVR_COPY_FROM_USER(&params, arg, sizeof(LX_PVR_SET_REG_T), return -EFAULT);
			ret = PVR_SetRegister(&params);
		}
		break;

		case PVR_IOR_GET_STATUS:
		{
			LX_PVR_STATUS_T params;

			ret = PVR_GetStatus(pdev->ctrl, &params);

			PVR_COPY_TO_USER(arg, &params, sizeof(LX_PVR_STATUS_T), return -EFAULT);
		}
		break;

		default:
		{
			PVR_PRINT_WARNING("Undefined ioctl cmd :0x%x ", cmd);
			/* redundant check but it seems more readable */
			ret = -ENOTTY;
		}
    }

    return ret;
}


static int 	PVR_Mmap (struct file *filep, struct vm_area_struct *vma )
{
	PVR_PRINT_TRACE("MMAP : start - %08lx , end - %08lx , off - %08lx , flags - %lx",
		vma->vm_start,
		vma->vm_end,
		(vma->vm_pgoff) << PAGE_SHIFT,
		vma->vm_flags);

	//to prevent swapout
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,10,0))
	vma->vm_flags |= VM_RESERVED;
#else
	vma->vm_flags |= VM_IO|VM_DONTEXPAND|VM_DONTDUMP;
#endif

	if(filep->f_flags & O_DSYNC)
	{
		// uncached
		vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);
	}

	return remap_pfn_range(vma, vma->vm_start, vma->vm_pgoff,
			vma->vm_end - vma->vm_start, vma->vm_page_prot);
}




///////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef KDRV_GLOBAL_LINK
#if defined(CONFIG_LG_BUILTIN_KDRIVER) && defined(CONFIG_LGSNAP)
user_initcall_grp("kdrv",PVR_Init);
#else
module_init(PVR_Init);
#endif
module_exit(PVR_Cleanup);

MODULE_AUTHOR("LGE");
MODULE_DESCRIPTION("base driver");
MODULE_LICENSE("GPL");
#endif

/** @} */

