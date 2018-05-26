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
 *  sdec driver
 *
 *  @author	Jihoon Lee ( gaius.lee@lge.com)
 *  @author	Jinhwan Bae ( jinhwan.bae@lge.com) - modifier
 *  @version	1.0
 *  @date		2010-03-30
 *  @note		Additional information.
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/
#undef	SUPPORT_SDEC_DEVICE_READ_WRITE_FOPS
#define	SUPPORT_SDEC_DEVICE_READ_WRITE_FOPS

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
#include <linux/irqreturn.h>
#include <asm/uaccess.h>
#include <asm/irq.h>                    /**< For isr */
#include <linux/mm.h>
#include <linux/rmap.h>
#include <asm/io.h>			/**< For ioremap_nocache */
#include <linux/platform_device.h>
#include <linux/version.h>
#include <asm/cacheflush.h>
#include <linux/dma-mapping.h>
#include <linux/dmaengine.h>

#include "base_device.h"

#include "sdec_impl.h"
#include "sdec_drv.h"
#include "sdec_io.h"
#include "sdec_isr.h"
#include "sdec_mm.h"
#include "sdec_pes.h"
#include "sdec_swparser.h"
#include "sdec_pcr.h"
#include "sdec_core.h"
#include "sdec_filter.h"
#include "sdec_cipher.h"
#include "ciplus/sdec_ciplus.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define SDEC_COPY_FROM_USER(to,from,n,fail_action) 					\
		do {															\
			if (copy_from_user((void*)to, (void __user *)from, n)) {	\
				SDEC_PRINT_ERROR("ioctl: copy_from_user");				\
				fail_action;											\
			}															\
		} while(0)

#define SDEC_COPY_TO_USER(to,from,n,fail_action)						\
		do {															\
			if (copy_to_user((void __user*)to, (void *)from, n)) {		\
				SDEC_PRINT_ERROR("ioctl: copy_to_user"); 				\
				fail_action;											\
			}															\
		} while(0)

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/

/**
 *	main control block for sdec device.
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

	S_SDEC_PARAM_T		stSdecParam;
// END of device specific data
}
SDEC_DEVICE_T;

typedef struct
{
	SDEC_DEVICE_T*		dev;
	SDEC_MM_CTX_T*		mm;
} SDEC_PRIV_T;

#ifdef KDRV_CONFIG_PM
typedef struct
{
	// add here extra parameter
	bool	is_suspended;
}SDEC_DRVDATA_T;
#endif

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/
extern	void	SDEC_PROC_Init(void);
extern	void	SDEC_PROC_Cleanup(void);

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Functions
----------------------------------------------------------------------------------------*/
int		SDEC_Init(void);
void	SDEC_Cleanup(void);

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/
int		g_sdec_debug_fd = -1;
int 	g_sdec_major = SDEC_MAJOR;
int 	g_sdec_minor = SDEC_MINOR;

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/
static void 	SDEC_Timer(ULONG param);
static int      SDEC_Open(struct inode *, struct file *);
static int      SDEC_Close(struct inode *, struct file *);
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)
static int SDEC_Ioctl (struct inode *, struct file *, unsigned int, unsigned long );
#else
static long SDEC_Ioctl (struct file *filp, unsigned int cmd, unsigned long arg );
#endif
static int 	SDEC_Mmap (struct file *filep, struct vm_area_struct *vma );
#ifdef SUPPORT_SDEC_DEVICE_READ_WRITE_FOPS
static ssize_t  SDEC_Read (
			struct file *i_pstrFilp, 				/* file pointer representing driver instance */
			SINT8        *o_pcBufferToLoad,     	/* buffer from user space */
			size_t      i_uiSizeToRead,    		/* size of buffer in bytes*/
			loff_t 		*i_FileOffset  );   		/* offset in the file */
//static ssize_t  SDEC_Write(struct file *, const char *, size_t, loff_t *);
#endif

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/

/**
 * main control block for sdec device
*/
static SDEC_DEVICE_T*		g_sdec_device;

/**
 * file I/O description for sdec device
 *
*/
static struct file_operations g_sdec_fops =
{
	.open 	= SDEC_Open,
	.release= SDEC_Close,
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)
	.ioctl	= SDEC_Ioctl,
#else
	.unlocked_ioctl	= SDEC_Ioctl,
#endif
#ifdef SUPPORT_SDEC_DEVICE_READ_WRITE_FOPS
	.read 	= (void *)SDEC_Read,
	.mmap	= SDEC_Mmap,
//	.write 	= SDEC_Write,
#else
//	.read	= NULL,
//	.write	= NULL,
#endif
};

static OS_TIMER_T		sdec_timer;


/*========================================================================================
	Implementation Group
========================================================================================*/
#ifdef KDRV_CONFIG_PM	// added by SC Jung for quick booting

/* jinhwan.bae M14_TBD, H14_TBD, not build component. check and delete it */

/**
 *
 * suspending module.
 *
 * @param	struct platform_device *pdev pm_message_t state
 * @return	int 0 : RET_OK , -1 : RET_ERROR
 *
 */


static int SDEC_suspend(struct platform_device *pdev, pm_message_t state)
{
#ifdef KDRV_CONFIG_PM_SDEC
	SDEC_DRVDATA_T *drv_data;
	drv_data = platform_get_drvdata(pdev);

	printk("SDEC - suspend BEGIN\n");

	if (SDEC_IO_Suspend(&g_sdec_device->stSdecParam))
	{
		printk("SDEC - Register Copy Failed\n");
	}

	drv_data->is_suspended = 1;

	printk("SDEC - suspend END\n");
#endif
	return 0;
}


/**
 *
 * resuming module.
 *
 * @param	struct platform_device *
 * @return	int 0 : RET_OK , -1 : RET_ERROR
 *
 */
static int SDEC_resume(struct platform_device *pdev)
{
#ifdef KDRV_CONFIG_PM_SDEC
	SDEC_DRVDATA_T *drv_data;
	drv_data = platform_get_drvdata(pdev);

	printk("SDEC - resume BEGIN\n");

	if(drv_data->is_suspended == 0)
	{
		printk("SDEC - resume FAIL\n");
		return -1;
	}

	//SDEC_InitHW(&g_sdec_device->stSdecParam);

	if(SDEC_IO_Resume(&g_sdec_device->stSdecParam))
	{
		printk("SDEC - Register Restore Failed\n");
	}

	drv_data->is_suspended = 0;
	printk("SDEC - resume OK\n");
#endif
	return 0;
}
/**
 *
 * probing module.
 *
 * @param	struct platform_device *pdev
 * @return	int 0 : RET_OK , -1 : RET_ERROR
 *
 */
 int  SDEC_probe(struct platform_device *pdev)
{

	SDEC_DRVDATA_T *drv_data;

	drv_data = (SDEC_DRVDATA_T *)kmalloc(sizeof(SDEC_DRVDATA_T) , GFP_KERNEL);


	// add here driver registering code & allocating resource code



	SDEC_PRINT_TRACE( "[%s] done probe\n", SDEC_MODULE);
	drv_data->is_suspended = 0;
	platform_set_drvdata(pdev, drv_data);

	return 0;
}


/**
 *
 * module remove function. this function will be called in rmmod fbdev module.
 *
 * @param	struct platform_device
 * @return	int 0 : RET_OK , -1 : RET_ERROR
 *
 */
static int  SDEC_remove(struct platform_device *pdev)
{
	SDEC_DRVDATA_T *drv_data;

	// add here driver unregistering code & deallocating resource code



	drv_data = platform_get_drvdata(pdev);
	kfree(drv_data);

	SDEC_PRINT_TRACE( "released\n");

	return 0;
}

/**
 *
 * module release function. this function will be called in rmmod module.
 *
 * @param	struct device *dev
 * @return	int 0 : RET_OK , -1 : RET_ERROR
 *
 */
static void  SDEC_release(struct device *dev)
{


	SDEC_PRINT_TRACE( "device released\n");
}

/*
 *	module platform driver structure
 */
static struct platform_driver sdec_driver =
{
	.probe          = SDEC_probe,
	.suspend        = SDEC_suspend,
	.remove         = SDEC_remove,
	.resume         = SDEC_resume,
	.driver         =
	{
		.name   = SDEC_MODULE,
	},
};

static struct platform_device sdec_device = {
	.name = SDEC_MODULE,
	.id = 0,
	.id = -1,
	.dev = {
		.release = SDEC_release,
	},
};
#endif

/** Initialize the device environment before the real H/W initialization
 *
 *  @note main usage of this function is to initialize the HAL layer and memory size adjustment
 *  @note it's natural to keep this function blank :)
 */
void SDEC_PreInit(void)
{
	SDEC_CFG_PreInit();
}


#if 1 // jinhwan.bae for JP mulit2 descrambler

#include "sdec_vdecmcufw.h"

/* If you want to use other platform except H13B0 JP FHD, MCU FW SROM/SRAM Address Should Be Reserved in Mem Map!
    jinhwan.bae */
#define	VDEC_MCU_SROM_BASE		0x6FE00000	// jinhwan.bae near guard, free space
#define	VDEC_MCU_SROM_SIZE		0x00080000

#define	VDEC_MCU_SRAM_BASE		(VDEC_MCU_SROM_BASE+VDEC_MCU_SROM_SIZE)		//0x6FE80000  jinhwan.bae
#define	VDEC_MCU_SRAM_SIZE		0x00080000

#define	VDEC_MCU_SRAM0_BASE		VDEC_MCU_SRAM_BASE
#define	VDEC_MCU_SRAM1_BASE		0x0
#define	VDEC_MCU_SRAM2_BASE		0x0

static void _SDEC_LoadVDECMcuFw(void)
{
	UINT8 *pu8RegPtr;
	UINT32 h13_mcu_rc, h13_mcu_fw, h13_mcu_ipc;
	UINT32 ui32Count, mcu_code_size, reg;
	UINT32 timeout = 0;

	// jinhwan.bae direct address access

	// ioremap register base
	h13_mcu_rc = (UINT32)ioremap(0xC0004600, 0x100);
	h13_mcu_ipc = (UINT32)ioremap(0xC0004800, 0x100);

	// set srom , sram 0, 1, 2 offset
	SDEC_WRITE32(h13_mcu_rc + 0x10 /* SROM_OFFSET */, VDEC_MCU_SROM_BASE /* hyun 확인 */ );
	SDEC_WRITE32(h13_mcu_rc + 0x14 /* SRAM0_OFFSET */, VDEC_MCU_SRAM0_BASE /* hyun 확인 */ );
	SDEC_WRITE32(h13_mcu_rc + 0x18 /* SRAM1_OFFSET */, VDEC_MCU_SRAM1_BASE /* 0 */ );
	SDEC_WRITE32(h13_mcu_rc + 0x1C /* SRAM2_OFFSET */, VDEC_MCU_SRAM2_BASE /* 0 */ );

	// FW down
	// 1. ioremap SROM addr
	h13_mcu_fw = (UINT32)ioremap(VDEC_MCU_SROM_BASE, VDEC_MCU_SROM_SIZE);
	// 2. stop mcu, set run_stall to 1
	SDEC_WRITE32(h13_mcu_rc + 0x00 , 1 );

	SDEC_WRITE32(h13_mcu_ipc + 0x10 , 0 );
	SDEC_WRITE32(h13_mcu_ipc + 0x14 , 0 );

	// 3. FW download
	pu8RegPtr = (UINT8 *)h13_mcu_fw;
	mcu_code_size = sizeof(aui8KeySetFW);
	for (ui32Count = 0; ui32Count < mcu_code_size; ui32Count ++)
	{
		*pu8RegPtr = aui8KeySetFW[ui32Count];
		pu8RegPtr++;
	}
	// 4. Reset
	reg = SDEC_READ32(h13_mcu_rc + 0x00);
	reg |= 0x10000000;
	SDEC_WRITE32(h13_mcu_rc + 0x00 , reg );

	timeout = msecs_to_jiffies(1000) + jiffies;
	while(1)
	{
		reg = SDEC_READ32(h13_mcu_rc + 0x00);
		if(!(reg & 0x10000000))
		{
			break;
		}

		if(jiffies > timeout)
		{
			printk("[SDEC] MCU Status in not ready ! break and go to following jobs\n");
			break;
		}
	}
	// 5. run mcu, set run_stall to 0
	SDEC_WRITE32(h13_mcu_rc + 0x00 , 0 );
	// 6. iounmap
	iounmap((void *)h13_mcu_fw);
	iounmap((void *)h13_mcu_ipc);

}
#endif


int SDEC_Init(void)
{
	int eRet = RET_OK;

	int			i;
	int			err;
	dev_t		dev;

	/* Get the handle of debug output for sdec device.
	 *
	 * Most module should open debug handle before the real initialization of module.
	 * As you know, debug_util offers 4 independent debug outputs for your device driver.
	 * So if you want to use all the debug outputs, you should initialize each debug output
	 * using OS_DEBUG_EnableModuleByIndex() function.
	 */
	g_sdec_debug_fd = DBG_OPEN( SDEC_MODULE );
	OS_DEBUG_EnableModule ( g_sdec_debug_fd );
	OS_DEBUG_EnableModuleByIndex ( g_sdec_debug_fd, LX_LOGM_LEVEL_ERROR, DBG_COLOR_RED );	/* ERROR */
	OS_DEBUG_EnableModuleByIndex ( g_sdec_debug_fd, LX_LOGM_LEVEL_NOTI, DBG_COLOR_GREEN );	/* NOTI */

	//LOGM_RegBitName(g_sdec_debug_fd, SDEC_MSG_DRV, "SDEC_MSG_DRV");
	LOGM_REG_CUS_BIT_NAME(g_sdec_debug_fd, SDEC_MSG_DRV);			/* 6 */
	LOGM_REG_CUS_BIT_NAME(g_sdec_debug_fd, SDEC_MSG_NORMAL);		/* 7 */
	LOGM_REG_CUS_BIT_NAME(g_sdec_debug_fd, SDEC_MSG_READ);			/* 8 */
	LOGM_REG_CUS_BIT_NAME(g_sdec_debug_fd, SDEC_MSG_ISR);			/* 9 */
	LOGM_REG_CUS_BIT_NAME(g_sdec_debug_fd, SDEC_MSG_IO);			/* 10 */
	LOGM_REG_CUS_BIT_NAME(g_sdec_debug_fd, SDEC_MSG_PIDSEC);		/* 11 */
	LOGM_REG_CUS_BIT_NAME(g_sdec_debug_fd, SDEC_MSG_RESET);			/* 12 */
	LOGM_REG_CUS_BIT_NAME(g_sdec_debug_fd, SDEC_MSG_PCR);			/* 13 */
	LOGM_REG_CUS_BIT_NAME(g_sdec_debug_fd, SDEC_MSG_CIA);			/* 14 */
	LOGM_REG_CUS_BIT_NAME(g_sdec_debug_fd, SDEC_MSG_PES);			/* 15 */
	LOGM_REG_CUS_BIT_NAME(g_sdec_debug_fd, SDEC_MSG_SWPARSER);		/* 16 */
	LOGM_REG_CUS_BIT_NAME(g_sdec_debug_fd, SDEC_MSG_DESC);			/* 17 */
	LOGM_REG_CUS_BIT_NAME(g_sdec_debug_fd, SDEC_MSG_RFOUT);			/* 18 */
	LOGM_REG_CUS_BIT_NAME(g_sdec_debug_fd, SDEC_MSG_ISR_SEC_ERR);	/* 19 */
	LOGM_REG_CUS_BIT_NAME(g_sdec_debug_fd, SDEC_MSG_MM);			/* 20 */
	LOGM_REG_CUS_BIT_NAME(g_sdec_debug_fd, SDEC_MSG_TRACE_EX);		/* 21 */


	SDEC_PRINT_NOTI("sdec device initialize");


	/* allocate main device handler, register current device.
	 *
	 * If devie major is predefined then register device using that number.
	 * otherwise, major number of device is automatically assigned by Linux kernel.
	 *
	 */
#ifdef KDRV_CONFIG_PM
	// added by SC Jung for quick booting
	if(platform_driver_register(&sdec_driver) < 0)
	{
		SDEC_PRINT_TRACE( "[%s] platform driver register failed\n",SDEC_MODULE);

	}
	else
	{
		if(platform_device_register(&sdec_device))
		{
			platform_driver_unregister(&sdec_driver);
			SDEC_PRINT_TRACE( "[%s] platform device register failed\n",SDEC_MODULE);
		}
		else
		{
			SDEC_PRINT_TRACE( "[%s] platform register done\n", SDEC_MODULE);
		}
	}
#endif

	g_sdec_device = (SDEC_DEVICE_T*)OS_KMalloc( sizeof(SDEC_DEVICE_T)*SDEC_MAX_DEVICE );
	if ( NULL == g_sdec_device )
	{
		SDEC_PRINT_ERROR( "out of memory. can't allocate %d bytes\n", sizeof(SDEC_DEVICE_T)* SDEC_MAX_DEVICE );
		return -ENOMEM;
	}

	memset( g_sdec_device, 0x0, sizeof(SDEC_DEVICE_T)* SDEC_MAX_DEVICE );

	if (g_sdec_major)//already know device major number
	{
		dev = MKDEV( g_sdec_major, g_sdec_minor );
		err = register_chrdev_region(dev, SDEC_MAX_DEVICE, SDEC_MODULE );
	}
	else//want to allocate device major number
	{
		err = alloc_chrdev_region(&dev, g_sdec_minor, SDEC_MAX_DEVICE, SDEC_MODULE );
		g_sdec_major = MAJOR(dev);
	}

	for ( i=0; i<SDEC_MAX_DEVICE; i++ )
	{
		/* initialize cdev structure with predefined variable */
		dev = MKDEV( g_sdec_major, g_sdec_minor+i );
		cdev_init( &(g_sdec_device[i].cdev), &g_sdec_fops );
		g_sdec_device[i].devno		= dev;
		g_sdec_device[i].cdev.owner = THIS_MODULE;
		g_sdec_device[i].cdev.ops   = &g_sdec_fops;

		err = cdev_add (&(g_sdec_device[i].cdev), dev, 1 );

		if (err)
		{
			SDEC_PRINT_ERROR( "error (%d) while adding sdec device (%d.%d)\n", err, MAJOR(dev), MINOR(dev) );
			return -EIO;
		}
        OS_CreateDeviceClass ( g_sdec_device[i].devno, "%s%d", SDEC_MODULE, i );
	}

	/* initialize proc system */
	SDEC_PROC_Init ( );

	SDEC_MM_Init();

	eRet = SDEC_Initialize(&g_sdec_device->stSdecParam);
	if (LX_IS_ERR(eRet))
	{
		SDEC_PRINT_ERROR( "SDEC_Initialize failed\n" );
		return -EIO;
	}


	eRet = SDEC_ISR_Init(&g_sdec_device->stSdecParam);
	SDEC_CHECK_ERROR(eRet < 0, return eRet, "ISR_Init fail[%d]", eRet);


	SDEC_PRINT_NOTI( "sdec device initialized");

	return 0;
}

void SDEC_Cleanup(void)
{
	int i;
	dev_t dev = MKDEV( g_sdec_major, g_sdec_minor );

	SDEC_ISR_Cleanup(&g_sdec_device->stSdecParam);
	SDEC_CleanupHW(&g_sdec_device->stSdecParam);
	SDEC_MM_Final();

	OS_StopTimer( &sdec_timer);

	/* cleanup proc system */
	SDEC_PROC_Cleanup( );

	/* remove all minor devicies and unregister current device */
	for ( i=0; i<SDEC_MAX_DEVICE;i++)
	{
		/* TODO: cleanup each minor device */


		/* END */
		cdev_del( &(g_sdec_device[i].cdev) );
	}

	unregister_chrdev_region(dev, SDEC_MAX_DEVICE );

#ifdef KDRV_CONFIG_PM
	// added by SC Jung for quick booting
	platform_driver_unregister(&sdec_driver);
	platform_device_unregister(&sdec_device);
#endif

	OS_Free( g_sdec_device );
}


///////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * open handler for sdec device
 *
 */
static int SDEC_Open(struct inode *inode, struct file *filp)
{
	int				major,minor;
    struct 	cdev	*cdev;
    SDEC_DEVICE_T	*my_dev;
	SDEC_PRIV_T		*priv;


	priv = kmalloc(sizeof(SDEC_PRIV_T), GFP_KERNEL);
	if(!priv)
	{
		return -ENOMEM;
	}
	priv->mm = SDEC_MM_Open();

    cdev	= inode->i_cdev;
    my_dev	= container_of ( cdev, SDEC_DEVICE_T, cdev);

    /* TODO : add your device specific code */
	if ( my_dev->dev_open_count == 0 )
	{
		my_dev->stSdecParam.ui32MsgMask = 0x11111111;

		OS_InitTimer( &sdec_timer);
		OS_StartTimer( &sdec_timer, SDEC_Timer, OS_TIMER_TIMETICK, 10 /* 10 Mhz */, (ULONG)&my_dev->stSdecParam );
	}

	/* END */

    my_dev->dev_open_count++;
	priv->dev = my_dev;
	filp->private_data = priv;

	/* some debug */
    major = imajor(inode);
    minor = iminor(inode);

    SDEC_PRINT_TRACE( "device opened (%d:%d)\n", major, minor );

	return 0;
}

/**
 * release handler for sdec device
 *
 */
static int SDEC_Close(struct inode *inode, struct file *file)
{
    int				major,minor;
    SDEC_DEVICE_T	*my_dev;
    struct cdev		*cdev;
	SDEC_PRIV_T		*priv = file->private_data;

    cdev	= inode->i_cdev;
    my_dev	= container_of ( cdev, SDEC_DEVICE_T, cdev);

    if ( my_dev->dev_open_count > 0 )
    {
        --my_dev->dev_open_count;
    }

    /* TODO : add your device specific code */
	if ( my_dev->dev_open_count == 0 )
	{
		OS_StopTimer( &sdec_timer);
	}
	/* END */

	SDEC_MM_Close(priv->mm);
	kfree(priv);

	/* some debug */
    major = imajor(inode);
    minor = iminor(inode);
    SDEC_PRINT_TRACE( "device closed (%d:%d)\n", major, minor );
    return 0;
}


/**
 * ioctl handler for sdec device.
 *
 *
 * note: if you have some critial data, you should protect them using semaphore or spin lock.
 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)
static int SDEC_Ioctl ( struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg )
#else
static long SDEC_Ioctl (struct file *filp, unsigned int cmd, unsigned long arg )
#endif
{
    int err = 0, ret = RET_ERROR;

    SDEC_DEVICE_T*	my_dev;
    struct cdev*		cdev;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,36)
	struct inode *inode = filp->f_path.dentry->d_inode;
#endif
	SDEC_PRIV_T *priv = filp->private_data;

	/*
	 * get current sdec device object
	 */
    cdev	= inode->i_cdev;
    my_dev	= container_of ( cdev, SDEC_DEVICE_T, cdev);

    /*
     * check if IOCTL command is valid or not.
     * - if magic value doesn't match, return error (-ENOTTY)
     * - if command is out of range, return error (-ENOTTY)
     *
     * note) -ENOTTY means "Inappropriate ioctl for device.
     */
    if (_IOC_TYPE(cmd) != SDEC_IOC_MAGIC)
    {
    	DBG_PRINT_WARNING("invalid magic. magic=0x%02X\n", _IOC_TYPE(cmd) );
    	return -ENOTTY;
    }
    if (_IOC_NR(cmd) > SDEC_IOC_MAXNR)
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

//	SDEC_PRINT_TRACE( "cmd = %08X (cmd_idx=%d)\n", cmd, _IOC_NR(cmd) );

	switch(cmd)
	{
		case SDEC_IOR_INIT:
		{
			LX_SDEC_CAP_T params;

			err = copy_from_user((void *)&params, (void __user *)arg, sizeof(LX_SDEC_CAP_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_from_user error \n");

			err = SDEC_IO_InitialaizeModule(&my_dev->stSdecParam, &params);
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "SDEC_IO_InitialaizeModule failed:[%d]", err);

			/* return the number of pid, sec filter and filter depth */
			err = copy_to_user((void __user *)arg, (void *)&params, sizeof(LX_SDEC_CAP_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_to_user error \n");

			ret = RET_OK; /* all work done */
		}
		break;

		case SDEC_IOW_SET_MCUMODE:
		{
			LX_SDEC_SET_MCUMODE_T params;

			if (lx_chip_rev() >= LX_CHIP_REV(M14, A0))
			{
				/* jinhwan.bae it's only for JP multi2 Descrambler Chip Bug in H13. H13 Only Routine.
				    H14/M14B0 RTL fix, H14A0/A1 ECO fix*/
			}
			else
			{
				_SDEC_LoadVDECMcuFw();

				err = copy_from_user((void *)&params, (void __user *)arg, sizeof(LX_SDEC_SET_MCUMODE_T));
				LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_from_user error \n");

				err = SDEC_IO_SetMCUDescramblerCtrlMode(&my_dev->stSdecParam, &params);
				LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "SDEC_IO_SetMCUDescramblerCtrlMode failed:[%d]", err);
			}

			ret = RET_OK; /* all work done */
		}
		break;

		case SDEC_IOW_SET_TVCTMODE:
		{
			LX_SDEC_SET_TVCTMODE_T params;

			err = copy_from_user((void *)&params, (void __user *)arg, sizeof(LX_SDEC_SET_TVCTMODE_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_from_user error \n");

			err = SDEC_IO_SetTVCTMode(&my_dev->stSdecParam, &params);
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "SDEC_IO_SetTVCTMode failed:[%d]", err);

			ret = RET_OK; /* all work done */
		}
		break;

		case SDEC_IOW_SET_SERIAL_INPUT_CLK_MODE:
		{
			LX_SDEC_SET_SERIALCLKMODE_T params;

			err = copy_from_user((void *)&params, (void __user *)arg, sizeof(LX_SDEC_SET_SERIALCLKMODE_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_from_user error \n");

			err = SDEC_IO_UseOrgExtDemodCLKinSerialInput(&my_dev->stSdecParam, &params);
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "SDEC_IO_UseOrgExtDemodCLKinSerialInput failed:[%d]", err);

			ret = RET_OK; /* all work done */
		}
		break;

		case SDEC_IOR_GET_STC_PCR:
		{
			LX_SDEC_GET_STC_PCR_T params;

			err = copy_from_user((void *)&params, (void __user *)arg, sizeof(LX_SDEC_GET_STC_PCR_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_from_user error \n");

			err = SDEC_IO_GetCurrentSTCPCR(&my_dev->stSdecParam, &params);
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "SDEC_IO_GetCurrentSTCPCR failed:[%d]", err);

			/* return current stc and pcr values */
			err = copy_to_user((void __user *)arg, (void *)&params, sizeof(LX_SDEC_GET_STC_PCR_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_to_user error \n");

			ret = RET_OK; /* all work done */
		}
		break;

		case SDEC_IOR_GET_STC:
		{
			LX_SDEC_GET_STC_T	params;

			SDEC_COPY_FROM_USER(&params, arg, sizeof(LX_SDEC_GET_STC_T), goto func_exit);

			err = SDEC_IO_GetLiveSTC(&my_dev->stSdecParam, &params);
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "SDEC_IO_GetLiveSTC failed:[%d]", err);

			SDEC_COPY_TO_USER(arg, &params, sizeof(LX_SDEC_GET_STC_T), goto func_exit);

			ret = RET_OK;
		}
		break;

		case SDEC_IOW_SEL_INPUT_PORT:
		{
			LX_SDEC_SEL_INPUT_T params;

			err = copy_from_user((void *)&params, (void __user *)arg, sizeof(LX_SDEC_SEL_INPUT_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_from_user error \n");

			err = SDEC_IO_SelInputPort(&my_dev->stSdecParam, &params);
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "SDEC_IO_SelInputPort failed:[%d]", err);

			ret = RET_OK; /* all work done */
		}
		break;

		case SDEC_IOW_CFG_INPUT_PORT:
		{
			LX_SDEC_CFG_INPUT_PARAM_T params;

			err = copy_from_user((void *)&params, (void __user *)arg, sizeof(LX_SDEC_CFG_INPUT_PARAM_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_from_user error \n");

			err = SDEC_IO_CfgInputPort(&my_dev->stSdecParam, &params);
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "SDEC_IO_CfgInputPort failed:[%d]", err);

			ret = RET_OK; /* all work done */
		}
		break;

		case SDEC_IOW_SEL_PAR_INPUT:
		{
			LX_SDEC_SEL_PAR_INPUT_T params;

			err = copy_from_user((void *)&params, (void __user *)arg, sizeof(LX_SDEC_SEL_PAR_INPUT_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_from_user error \n");

			err = SDEC_IO_SelParInput(&my_dev->stSdecParam, &params);
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "SDEC_IO_SelParInput failed:[%d]", err);

			ret = RET_OK; /* all work done */
		}
		break;

		case SDEC_IOW_SEL_INPUT_SRC:
		{
			LX_SDEC_SEL_INPUT_SRC_T params;

			err = copy_from_user((void *)&params, (void __user *)arg, sizeof(LX_SDEC_SEL_INPUT_SRC_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_from_user error \n");

			err = SDEC_IO_SelInputSource(&my_dev->stSdecParam, &params);
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "SDEC_IO_SelInputSource failed:[%d]", err);

			ret = RET_OK; /* all work done */
		}
		break;

		case SDEC_IOW_SEL_CI_INPUT:
		{
			LX_SDEC_SEL_CI_INPUT_T params;

			err = copy_from_user((void *)&params, (void __user *)arg, sizeof(LX_SDEC_SEL_CI_INPUT_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_from_user error \n");

			err = SDEC_IO_SelCiInput(&my_dev->stSdecParam, &params);
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "SDEC_IO_SelCiInput failed:[%d]", err);

			ret = RET_OK; /* all work done */
		}
		break;

		case SDEC_IOW_GET_PAR_INPUT:
		{
			LX_SDEC_GET_PAR_INPUT_T params;

			err = copy_from_user((void *)&params, (void __user *)arg, sizeof(LX_SDEC_GET_PAR_INPUT_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_from_user error \n");

			err = SDEC_IO_GetParInput(&my_dev->stSdecParam, &params);
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "SDEC_IO_GetParInput failed:[%d]", err);

			/* return current parallel input settings */
			err = copy_to_user((void __user *)arg, (void *)&params, sizeof(LX_SDEC_GET_PAR_INPUT_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_to_user error \n");

			ret = RET_OK; /* all work done */
		}
		break;

		case SDEC_IOW_GET_INPUT_SRC:
		{
			LX_SDEC_GET_INPUT_SRC_T params;

			err = copy_from_user((void *)&params, (void __user *)arg, sizeof(LX_SDEC_GET_INPUT_SRC_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_from_user error \n");

			err = SDEC_IO_GetInputSource(&my_dev->stSdecParam, &params);
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "SDEC_IO_GetInputSource failed:[%d]", err);

			/* return current parallel input settings */
			err = copy_to_user((void __user *)arg, (void *)&params, sizeof(LX_SDEC_GET_INPUT_SRC_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_to_user error \n");

			ret = RET_OK; /* all work done */
		}
		break;

		case SDEC_IOW_GET_CI_INPUT:
		{
			LX_SDEC_GET_CI_INPUT_T params;

			err = copy_from_user((void *)&params, (void __user *)arg, sizeof(LX_SDEC_GET_CI_INPUT_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_from_user error \n");

			err = SDEC_IO_GetCiInput(&my_dev->stSdecParam, &params);
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "SDEC_IO_GetCiInput failed:[%d]", err);

			/* return current ci input settings */
			err = copy_to_user((void __user *)arg, (void *)&params, sizeof(LX_SDEC_GET_CI_INPUT_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_to_user error \n");

			ret = RET_OK; /* all work done */
		}
		break;

		case SDEC_IOR_GET_INPUT_PORT:
		{
			LX_SDEC_GET_INPUT_T params;

			err = copy_from_user((void *)&params, (void __user *)arg, sizeof(LX_SDEC_GET_INPUT_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_from_user error \n");

			err = SDEC_IO_GetInputPort(&my_dev->stSdecParam, &params);
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "SDEC_IO_GetInputPort failed:[%d]", err);

			/* return current input settings */
			err = copy_to_user((void __user *)arg, (void *)&params, sizeof(LX_SDEC_GET_INPUT_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_to_user error \n");

			ret = RET_OK; /* all work done */
		}
		break;

		case SDEC_IOW_ENABLE_INPUT_PORT:
		{
			LX_SDEC_ENABLE_INPUT_T params;

			err = copy_from_user((void *)&params, (void __user *)arg, sizeof(LX_SDEC_ENABLE_INPUT_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_from_user error \n");

			err = SDEC_IO_InputPortEnable(&my_dev->stSdecParam, &params);
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "SDEC_IO_InputPortEnable failed:[%d]", err);

			ret = RET_OK; /* all work done */
		}
		break;

		case SDEC_IOW_SET_CIPHER_ENABLE:
		{
			LX_SDEC_CIPHER_ENABLE_T params;

			err = copy_from_user((void *)&params, (void __user *)arg, sizeof(LX_SDEC_CIPHER_ENABLE_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_from_user error \n");

			err = SDEC_IO_SetCipherEnable(&my_dev->stSdecParam, &params);
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "SDEC_IO_SetCipherEnable failed:[%d]", err);

			ret = RET_OK; /* all work done */
		}
		break;

		case SDEC_IOW_SET_CIPHER_MODE:
		{
			LX_SDEC_CIPHER_MODE_T params;

			err = copy_from_user((void *)&params, (void __user *)arg, sizeof(LX_SDEC_CIPHER_MODE_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_from_user error \n");

			err = SDEC_IO_SetCipherMode(&my_dev->stSdecParam, &params);
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "SDEC_IO_SetCipherMode failed:[%d]", err);

			ret = RET_OK; /* all work done */
		}
		break;

		case SDEC_IOW_SET_CIPHER_KEY:
		{
			LX_SDEC_CIPHERKEY_T params;

			err = copy_from_user((void *)&params, (void __user *)arg, sizeof(LX_SDEC_CIPHERKEY_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_from_user error \n");

			err = SDEC_IO_SetCipherKey(&my_dev->stSdecParam, &params);
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "SDEC_IO_SetCipherKey failed:[%d]", err);

			ret = RET_OK; /* all work done */
		}
		break;

		case SDEC_IOW_GET_CIPHER_KEY:
		{
			LX_SDEC_GET_CIPHERKEY_T params;

			err = copy_from_user((void *)&params, (void __user *)arg, sizeof(LX_SDEC_GET_CIPHERKEY_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_from_user error \n");

			err = SDEC_IO_GetCipherKey(&my_dev->stSdecParam, &params);
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "SDEC_IO_GetCipherKey failed:[%d]", err);

			/* return cipher keys */
			err = copy_to_user((void __user *)arg, (void *)&params, sizeof(LX_SDEC_GET_CIPHERKEY_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_to_user error \n");

			ret = RET_OK; /* all work done */
		}
		break;

		case SDEC_IOW_SET_PCR_PID:
		{
			LX_SDEC_PIDFLT_SET_PCRPID_T params;

			err = copy_from_user((void *)&params, (void __user *)arg, sizeof(LX_SDEC_PIDFLT_SET_PCRPID_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_from_user error \n");

			err = SDEC_IO_SetPCRPID(&my_dev->stSdecParam, &params);
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "SDEC_IO_SetPCRPID failed:[%d]", err);

			ret = RET_OK; /* all work done */
		}
		break;

		case SDEC_IOW_SET_PCR_RECOVERY:
		{
			LX_SDEC_SET_PCR_RECOVERY_T params;

			err = copy_from_user((void *)&params, (void __user *)arg, sizeof(LX_SDEC_SET_PCR_RECOVERY_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_from_user error \n");

			err = SDEC_IO_SetPcrRecovery(&my_dev->stSdecParam, &params);
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "SDEC_IO_SetPcrRecovery failed:[%d]", err);

			ret = RET_OK; /* all work done */
		}
		break;

		case SDEC_IOW_SET_VDEC_PORT:
		{
			LX_SDEC_SET_VDEC_PORT_T params;

			err = copy_from_user((void *)&params, (void __user *)arg, sizeof(LX_SDEC_SET_VDEC_PORT_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_from_user error \n");

			err = SDEC_IO_SetVidOutport(&my_dev->stSdecParam, &params);
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "SDEC_IO_SetVidOutport failed:[%d]", err);

			ret = RET_OK; /* all work done */
		}
		break;

		case SDEC_IOR_PIDFLT_ALLOC:
		{
			LX_SDEC_PIDFLT_ALLOC_T params;

			err = copy_from_user((void *)&params, (void __user *)arg, sizeof(LX_SDEC_PIDFLT_ALLOC_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_from_user error \n");

			err = SDEC_IO_PIDFilterAlloc(&my_dev->stSdecParam, &params);
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "SDEC_IO_PIDFilterAlloc failed:[%d]", err);

			/* return pidfilter index */
			err = copy_to_user((void __user *)arg, (void *)&params, sizeof(LX_SDEC_PIDFLT_ALLOC_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_to_user error \n");

			ret = RET_OK; /* all work done */
		}
		break;

		case SDEC_IOW_PIDFLT_FREE:
		{
			LX_SDEC_PIDFLT_FREE_T params;

			err = copy_from_user((void *)&params, (void __user *)arg, sizeof(LX_SDEC_PIDFLT_FREE_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_from_user error \n");

			err = SDEC_IO_PIDFilterFree(&my_dev->stSdecParam, &params);
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "SDEC_IO_PIDFilterFree failed:[%d]", err);

			ret = RET_OK; /* all work done */
		}
		break;

		case SDEC_IOW_PIDFLT_SET_PID:
		{
			LX_SDEC_PIDFLT_SET_PID_T params;

			err = copy_from_user((void *)&params, (void __user *)arg, sizeof(LX_SDEC_PIDFLT_SET_PID_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_from_user error \n");

			err = SDEC_IO_PIDFilterSetPID(&my_dev->stSdecParam, &params);
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "SDEC_IO_PIDFilterSetPID failed:[%d]", err);

			ret = RET_OK; /* all work done */
		}
		break;

		case SDEC_IOW_PIDFLT_MAP_SECFLT:
		{
			LX_SDEC_PIDFLT_SELSECFLT_T params;

			err = copy_from_user((void *)&params, (void __user *)arg, sizeof(LX_SDEC_PIDFLT_SELSECFLT_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_from_user error \n");

			err = SDEC_IO_PIDFilterMapSelect(&my_dev->stSdecParam, &params);
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "SDEC_IO_PIDFilterMapSelect failed:[%d]", err);

			ret = RET_OK; /* all work done */
		}
		break;

		case SDEC_IOW_PIDFLT_ENABLE:
		{
			LX_SDEC_PIDFLT_ENABLE_T params;

			err = copy_from_user((void *)&params, (void __user *)arg, sizeof(LX_SDEC_PIDFLT_ENABLE_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_from_user error \n");

			err = SDEC_IO_PIDFilterEnable(&my_dev->stSdecParam, &params);
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "SDEC_IO_PIDFilterEnable failed:[%d]", err);

			ret = RET_OK; /* all work done */
		}
		break;

		case SDEC_IOW_PIDFLT_CRC_ENABLE:
		{
			LX_SDEC_PIDFLT_ENABLE_T params;

			err = copy_from_user((void *)&params, (void __user *)arg, sizeof(LX_SDEC_PIDFLT_ENABLE_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_from_user error \n");

			err = SDEC_IO_PIDFilterCRCEnable(&my_dev->stSdecParam, &params);
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "SDEC_IO_PIDFilterCRCEnable failed:[%d]", err);

			ret = RET_OK; /* all work done */
		}
		break;

		case SDEC_IOR_PIDFLT_GET_STATE:
		{
			LX_SDEC_PIDFLT_STATE_T params;

			err = copy_from_user((void *)&params, (void __user *)arg, sizeof(LX_SDEC_PIDFLT_STATE_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_from_user error \n");

			err = SDEC_IO_PIDFilterGetState(&my_dev->stSdecParam, &params);
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "SDEC_IO_PIDFilterGetState failed:[%d]", err);

			/* return pidfilter state register values and section filter mapping information */
			err = copy_to_user((void __user *)arg, (void *)&params, sizeof(LX_SDEC_PIDFLT_STATE_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_to_user error \n");

			ret = RET_OK; /* all work done */
		}
		break;

		case SDEC_IOR_PIDFLT_ENABLE_SCMBCHK:
		{
			LX_SDEC_PIDFLT_ENABLE_T params;

			err = copy_from_user((void *)&params, (void __user *)arg, sizeof(LX_SDEC_PIDFLT_ENABLE_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_from_user error \n");

			err = SDEC_IO_PIDFilterEnableSCMBCHK(&my_dev->stSdecParam, &params);
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "SDEC_IO_PIDFilterEnableSCMBCHK failed:[%d]", err);

			ret = RET_OK; /* all work done */
		}
		break;

		case SDEC_IOW_PIDFLT_ENABLE_DOWNLOAD:
		{
			LX_SDEC_PIDFLT_ENABLE_T params;

			err = copy_from_user((void *)&params, (void __user *)arg, sizeof(LX_SDEC_PIDFLT_ENABLE_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_from_user error \n");

			err = SDEC_IO_PIDFilterEnableDownload(&my_dev->stSdecParam, &params);
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "SDEC_IO_PIDFilterEnableDownload failed:[%d]", err);

			ret = RET_OK; /* all work done */
		}
		break;

		case SDEC_IOR_SECFLT_ALLOC:
		{
			LX_SDEC_SECFLT_ALLOC_T params;

			err = copy_from_user((void *)&params, (void __user *)arg, sizeof(LX_SDEC_SECFLT_ALLOC_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_from_user error \n");

			err = SDEC_IO_SectionFilterAlloc(&my_dev->stSdecParam, &params);
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "SDEC_IO_SectionFilterAlloc failed:[%d]", err);

			/* return section filter index */
			err = copy_to_user((void __user *)arg, (void *)&params, sizeof(LX_SDEC_SECFLT_ALLOC_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_to_user error \n");

			ret = RET_OK; /* all work done */
		}
		break;

		case SDEC_IOW_SECFLT_FREE:
		{
			LX_SDEC_SECFLT_FREE_T params;

			err = copy_from_user((void *)&params, (void __user *)arg, sizeof(LX_SDEC_SECFLT_FREE_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_from_user error \n");

			err = SDEC_IO_SectionFilterFree(&my_dev->stSdecParam, &params);
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "SDEC_IO_SectionFilterFree failed:[%d]", err);

			ret = RET_OK; /* all work done */
		}
		break;

		case SDEC_IOW_PESFLT_FREE:
		{
			LX_SDEC_SECFLT_FREE_T params;

			err = copy_from_user((void *)&params, (void __user *)arg, sizeof(LX_SDEC_SECFLT_FREE_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_from_user error \n");

			err = SDEC_IO_PESFilterFree(&my_dev->stSdecParam, &params);
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "SDEC_IO_PESFilterFree failed:[%d]", err);

			ret = RET_OK; /* all work done */
		}
		break;

		case SDEC_IOW_SECFLT_PATTERN:
		{
			LX_SDEC_SECFLT_PATTERN_T params;

			err = copy_from_user((void *)&params, (void __user *)arg, sizeof(LX_SDEC_SECFLT_PATTERN_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_from_user error \n");

			err = SDEC_IO_SectionFilterPattern(&my_dev->stSdecParam, &params);
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "SDEC_IO_SectionFilterPattern failed:[%d]", err);

			ret = RET_OK; /* all work done */
		}
		break;

		case SDEC_IOW_SECFLT_BUFFER_RESET:
		{
			LX_SDEC_SECFLT_BUFFER_RESET params;

			err = copy_from_user((void *)&params, (void __user *)arg, sizeof(LX_SDEC_SECFLT_BUFFER_RESET));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_from_user error \n");

			err = SDEC_IO_SectionFilterBufferReset(&my_dev->stSdecParam, &params);
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "SDEC_IO_SectionFilterBufferReset failed:[%d]", err);

			ret = RET_OK; /* all work done */
		}
		break;

		case SDEC_IOW_SECFLT_BUFFER_SET:
		{
			LX_SDEC_SECFLT_BUFFER_SET_T params;

			err = copy_from_user((void *)&params, (void __user *)arg, sizeof(LX_SDEC_SECFLT_BUFFER_SET_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_from_user error \n");

			err = SDEC_IO_SectionFilterBufferSet(&my_dev->stSdecParam, &params);
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "SDEC_IO_SectionFilterBufferSet failed:[%d]", err);

			ret = RET_OK; /* all work done */
		}
		break;

		case SDEC_IOW_SECFLT_DUMMY_BUFFER_SET:
		{
			LX_SDEC_SECFLT_BUFFER_SET_T params;

			err = copy_from_user((void *)&params, (void __user *)arg, sizeof(LX_SDEC_SECFLT_BUFFER_SET_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_from_user error \n");

			err = SDEC_IO_SectionFilterDummyBufferSet(&my_dev->stSdecParam, &params);
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "SDEC_IO_SectionFilterDummyBufferSet failed:[%d]", err);

			ret = RET_OK; /* all work done */
		}
		break;

		case SDEC_IOW_SECFLT_BUFFER_GET_INFO:
		{
			LX_SDEC_SECFLT_BUFFER_GET_INFO_T params;

			err = copy_from_user((void *)&params, (void __user *)arg, sizeof(LX_SDEC_SECFLT_BUFFER_GET_INFO_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_from_user error \n");

			err = SDEC_IO_SectionFilterGetInfo(&my_dev->stSdecParam, &params);
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "SDEC_IO_SectionFilterGetInfo failed:[%d]", err);

			/* return section filter GPB buffer address */
			err = copy_to_user((void __user *)arg, (void *)&params, sizeof(LX_SDEC_SECFLT_BUFFER_GET_INFO_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_to_user error \n");

			ret = RET_OK; /* all work done */
		}
		break;

		case SDEC_IOW_SECFLT_SET_READPTR:
		{
			LX_SDEC_SECFLT_READPTR_SET_T params;

			err = copy_from_user((void *)&params, (void __user *)arg, sizeof(LX_SDEC_SECFLT_READPTR_SET_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_from_user error \n");

			err = SDEC_IO_SectionFilterSetReadPtr(&my_dev->stSdecParam, &params);
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "SDEC_IO_SectionFilterSetReadPtr failed:[%d]", err);

			ret = RET_OK; /* all work done */
		}
		break;

		case SDEC_IOW_SECFLT_GET_READPTR:
		{
			LX_SDEC_SECFLT_GET_READPTR_T params;

			err = copy_from_user((void *)&params, (void __user *)arg, sizeof(LX_SDEC_SECFLT_GET_READPTR_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_from_user error \n");

			err = SDEC_IO_SectionFilterGetReadPtr(&my_dev->stSdecParam, &params);
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "SDEC_IO_SectionFilterGetReadPtr failed:[%d]", err);

			/* return GPB read pointer */
			err = copy_to_user((void __user *)arg, (void *)&params, sizeof(LX_SDEC_SECFLT_GET_READPTR_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_to_user error \n");

			ret = RET_OK; /* all work done */
		}
		break;

		case SDEC_IOW_SECFLT_SET_WRITEPTR:
		{
			LX_SDEC_SECFLT_WRITEPTR_SET_T params;

			err = copy_from_user((void *)&params, (void __user *)arg, sizeof(LX_SDEC_SECFLT_WRITEPTR_SET_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_from_user error \n");

			err = SDEC_IO_SectionFilterSetWritePtr(&my_dev->stSdecParam, &params);
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "SDEC_IO_SectionFilterSetWritePtr failed:[%d]", err);

			ret = RET_OK; /* all work done */
		}
		break;

		case SDEC_IOW_SECFLT_GET_WRITEPTR:
		{

			LX_SDEC_SECFLT_GET_WRITEPTR_T params;

			err = copy_from_user((void *)&params, (void __user *)arg, sizeof(LX_SDEC_SECFLT_GET_WRITEPTR_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_from_user error \n");

			err = SDEC_IO_SectionFilterGetWritePtr(&my_dev->stSdecParam, &params);
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "SDEC_IO_SectionFilterGetWritePtr failed:[%d]", err);

			/* return GPB read pointer */
			err = copy_to_user((void __user *)arg, (void *)&params, sizeof(LX_SDEC_SECFLT_GET_WRITEPTR_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_to_user error \n");

			ret = RET_OK; /* all work done */
		}
		break;

		case SDEC_IOR_SECFLT_GET_STATE:
		{
			LX_SDEC_SECFLT_STATE_T params;

			err = copy_from_user((void *)&params, (void __user *)arg, sizeof(LX_SDEC_SECFLT_STATE_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_from_user error \n");

			err = SDEC_IO_SectionFilterGetState(&my_dev->stSdecParam, &params);
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "SDEC_IO_SectionFilterGetState failed:[%d]", err);

			/* return section filter GPB buffer address */
			err = copy_to_user((void __user *)arg, (void *)&params, sizeof(LX_SDEC_SECFLT_STATE_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_to_user error \n");

			ret = RET_OK; /* all work done */
		}
		break;

		case SDEC_IOW_SECFLT_ENABLE_SWPARSER:
		{
			LX_SDEC_SECFLT_ENABLE_SWPARSER_T params;

			err = copy_from_user((void *)&params, (void __user *)arg, sizeof(LX_SDEC_SECFLT_ENABLE_SWPARSER_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_from_user error \n");

			err = SDEC_SWP_EnableSWParser(&my_dev->stSdecParam, &params);
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "SDEC_SWP_EnableSWParser failed:[%d]", err);

			ret = RET_OK; /* all work done */
		}
		break;

		case SDEC_IOR_SECFLT_GET_AVAIL_NUMBER:
		{
			LX_SDEC_SECFLT_GET_AVAIL_NUMBER_T params;

			err = copy_from_user((void *)&params, (void __user *)arg, sizeof(LX_SDEC_SECFLT_GET_AVAIL_NUMBER_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_from_user error \n");

			err = SDEC_IO_SectionFilterGetAvailableNumber(&my_dev->stSdecParam, &params);
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "SDEC_IO_SectionFilterGetAvailableNumber failed:[%d]", err);

			/* return the number of available section filters */
			err = copy_to_user((void __user *)arg, (void *)&params, sizeof(LX_SDEC_SECFLT_GET_AVAIL_NUMBER_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_to_user error \n");

			ret = RET_OK; /* all work done */
		}
		break;

		case SDEC_IOR_GPB_GET_BASE_ADDR_OFFSET:
		{
			LX_SDEC_GPB_BASE_OFFSET_T params;

			err = copy_from_user((void *)&params, (void __user *)arg, sizeof(LX_SDEC_GPB_BASE_OFFSET_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_from_user error \n");

			err = SDEC_IO_GetGPBBaseOffset(&my_dev->stSdecParam, &params);
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "SDEC_IO_SectionFilterGetAvailableNumber failed:[%d]", err);

			/* return gpb base offset */
			err = copy_to_user((void __user *)arg, (void *)&params, sizeof(LX_SDEC_GPB_BASE_OFFSET_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_to_user error \n");

			ret = RET_OK; /* all work done */
		}
		break;

		case SDEC_IOW_ENABLE_LOG:
		{
			UINT32 params;

			err = copy_from_user((void *)&params, (void __user *)arg, sizeof(UINT32));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_from_user error \n");

			err = SDEC_IO_EnableLog(&my_dev->stSdecParam, &params);
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "SDEC_IO_EnableLog failed:[%d]", err);

			ret = RET_OK; /* all work done */
		}
		break;

		case SDEC_IOW_SET_FCW:
		{
			/* 	M14_TBD , jinhwan.bae 20130829
				this case is made to set STC related params for testing.
				2 cases are now on debug only but mixed in one ioctrl. FIXME */
			UINT32 params;

			err = copy_from_user((void *)&params, (void __user *)arg, sizeof(UINT32));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_from_user error \n");

			err = SDEC_IO_SettingFCW(&my_dev->stSdecParam, params);
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "SDEC_IO_SetFCW failed:[%d]", err);
			//err = SDEC_IO_SetSTCMultiply(&my_dev->stSdecParam, &params);
			//LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "SDEC_IO_SetSTCMultiply failed:[%d]", err);

			ret = RET_OK; /* all work done */
		}
		break;

		case SDEC_IOR_SET_STCCASG:
		{
			LX_SDEC_GET_STCCASG_T params;

			err = copy_from_user((void *)&params, (void __user *)arg, sizeof(LX_SDEC_GET_STCCASG_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_from_user error \n");

			err = SDEC_IO_SetSTCCASGStatus(&my_dev->stSdecParam, &params);
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "SDEC_IO_SetSTCCASGStatus failed:[%d]", err);

			ret = RET_OK; /* all work done */
		}
		break;

		case SDEC_IOR_GET_STCCASG:
		{
			LX_SDEC_GET_STCCASG_T params;

			err = copy_from_user((void *)&params, (void __user *)arg, sizeof(LX_SDEC_GET_STCCASG_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_from_user error \n");

			err = SDEC_IO_GetSTCCASGStatus(&my_dev->stSdecParam, &params);
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "SDEC_IO_GetSTCCASGStatus failed:[%d]", err);

			/* return main stc assign status */
			err = copy_to_user((void __user *)arg, (void *)&params, sizeof(LX_SDEC_GET_STCCASG_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_to_user error \n");

			ret = RET_OK; /* all work done */
		}
		break;

		case SDEC_IOR_GET_STCC_STATUS:
		{
			LX_SDEC_GET_STCC_STATUS_T params;

			err = copy_from_user((void *)&params, (void __user *)arg, sizeof(LX_SDEC_GET_STCC_STATUS_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_from_user error \n");

			err = SDEC_IO_GetSTCCStatus(&my_dev->stSdecParam, &params);
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "SDEC_IO_GetSTCCStatus failed:[%d]", err);

			/* return stcc register values */
			err = copy_to_user((void __user *)arg, (void *)&params, sizeof(LX_SDEC_GET_STCC_STATUS_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_to_user error \n");

			ret = RET_OK; /* all work done */
		}
		break;

		case SDEC_IOR_GET_GSTC:
		{
			LX_SDEC_GET_GSTC_T params;

			err = copy_from_user((void *)&params, (void __user *)arg, sizeof(LX_SDEC_GET_GSTC_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_from_user error \n");

			err = SDEC_IO_GetCurrentGSTC(&my_dev->stSdecParam, &params);
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "SDEC_IO_GetCurrentGSTC failed:[%d]", err);

			/* return gstc value */
			err = copy_to_user((void __user *)arg, (void *)&params, sizeof(LX_SDEC_GET_GSTC_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_to_user error \n");

			ret = RET_OK; /* all work done */
		}
		break;

		case SDEC_IOR_GET_GSTC32:
		{
			LX_SDEC_GET_GSTC32_T params;

			err = copy_from_user((void *)&params, (void __user *)arg, sizeof(LX_SDEC_GET_GSTC32_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_from_user error \n");

			err = SDEC_IO_GetCurrentGSTC32(&my_dev->stSdecParam, &params);
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "SDEC_IO_GetCurrentGSTC32 failed:[%d]", err);

			/* return gstc value */
			err = copy_to_user((void __user *)arg, (void *)&params, sizeof(LX_SDEC_GET_GSTC32_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_to_user error \n");

			ret = RET_OK; /* all work done */
		}
		break;

		case SDEC_IOR_SET_GSTC:
		{
			LX_SDEC_SET_GSTC_T params;

			err = copy_from_user((void *)&params, (void __user *)arg, sizeof(LX_SDEC_SET_GSTC_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_from_user error \n");

			err = SDEC_IO_SetCurrentGSTC(&my_dev->stSdecParam, &params);
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "SDEC_IO_SetCurrentGSTC failed:[%d]", err);

			ret = RET_OK; /* all work done */
		}
		break;

		case SDEC_IOR_GET_REG:
		{
			LX_SDEC_GET_REG_T params;

			err = copy_from_user((void *)&params, (void __user *)arg, sizeof(LX_SDEC_GET_REG_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_from_user error \n");

			err = SDEC_IO_GetRegister(&my_dev->stSdecParam, &params);
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "SDEC_IO_GetRegister failed:[%d]", err);

			/* return register value */
			err = copy_to_user((void __user *)arg, (void *)&params, sizeof(LX_SDEC_GET_REG_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_to_user error \n");

			ret = RET_OK; /* all work done */
		}
		break;

		case SDEC_IOR_SET_REG:
		{
			LX_SDEC_GET_REG_T params;

			err = copy_from_user((void *)&params, (void __user *)arg, sizeof(LX_SDEC_GET_REG_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_from_user error \n");

			err = SDEC_IO_SetRegister(&my_dev->stSdecParam, &params);
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "SDEC_IO_SetRegister failed:[%d]", err);

			ret = RET_OK; /* all work done */
		}
		break;

		case SDEC_IOW_MM_GETINFO:
		{
			LX_SDEC_MEM_INFO_T info;

			err = SDEC_MM_GetMemInfo(SDEC_MM_TYPE_NORMAL, &info);
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "SDEC_IOW_MM_GETINFO failed:[%d]", err);

			/* return mem base and size */
			err = copy_to_user((void __user *)arg, (void *)&info, sizeof(LX_SDEC_MEM_INFO_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_to_user error \n");

			ret = RET_OK; /* all work done */
		}
		break;

		case SDEC_IOW_MM_FREE:
		{
			UINT32 params;

			err = copy_from_user((void *)&params, (void __user *)arg, sizeof(UINT32));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_from_user error \n");

			err = SDEC_MM_Free(priv->mm, SDEC_MM_TYPE_NORMAL, (void*)params);
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "SDEC_MM_Free failed:[%d]", err);

			ret = RET_OK; /* all work done */
		}
		break;

		case SDEC_IOR_MM_ALLOC:
		{
			LX_SDEC_MEM_INFO_T params;
			void* ptr;

			err = copy_from_user((void *)&params, (void __user *)arg, sizeof(LX_SDEC_MEM_INFO_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_from_user error \n");

			ptr = SDEC_MM_Alloc(priv->mm, SDEC_MM_TYPE_NORMAL, params.size);
			LX_SDEC_CHECK_CODE( ptr == NULL, goto func_exit, "SDEC_MM_Alloc failed:[%d]", err);

			params.addr = (UINT32)ptr;

			/* return mem address and size */
			err = copy_to_user((void __user *)arg, (void *)&params, sizeof(LX_SDEC_MEM_INFO_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_to_user error \n");


			ret = RET_OK; /* all work done */
		}
		break;

		case SDEC_IOW_MM_CACHE_CTRL:
		{
			LX_SDEC_CACHE_CTRL_T cache_ctrl;

			err = copy_from_user( (void *)&cache_ctrl, (void __user*)arg, sizeof(LX_SDEC_CACHE_CTRL_T));
            LX_SDEC_CHECK_CODE( err > 0, goto func_exit, "copy error\n");

			switch (cache_ctrl.op)
			{
				case LX_SDEC_CACHE_OP_INVAL:
#ifdef __aarch64__
					dma_map_single(NULL,(void*)cache_ctrl.vaddr, cache_ctrl.size, DMA_FROM_DEVICE);
#else
					dmac_map_area((void*)cache_ctrl.vaddr, cache_ctrl.size, DMA_FROM_DEVICE);//v7_dma_inv_range
					outer_inv_range((phys_addr_t)cache_ctrl.paddr, (phys_addr_t)cache_ctrl.paddr + cache_ctrl.size);
#endif
					break;

				case LX_SDEC_CACHE_OP_CLEAN:
#ifdef __aarch64__
					dma_map_single(NULL,(void*)cache_ctrl.vaddr, cache_ctrl.size, DMA_TO_DEVICE);
#else
					dmac_map_area((void*)cache_ctrl.vaddr, cache_ctrl.size, DMA_TO_DEVICE);//v7_dma_clean_range
					outer_clean_range((phys_addr_t)cache_ctrl.paddr, (phys_addr_t)cache_ctrl.paddr + cache_ctrl.size);
#endif
					break;

				default:
					SDEC_PRINT_ERROR("invalid cache op 0x%x\n", cache_ctrl.op );
					goto func_exit;
			}

			ret = RET_OK; /* all work done */
		}
		break;

		case SDEC_IOR_MM_GET_STAT:
		{
			LX_SDEC_MM_STAT_T params;

			err = SDEC_MM_GetMemStat(SDEC_MM_TYPE_NORMAL, &params);
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "SDEC_MM_GetMemStat failed:[%d]", err);

			/* return mem address and size , allocation and free status */
			err = copy_to_user((void __user *)arg, (void *)&params, sizeof(LX_SDEC_MM_STAT_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_to_user error \n");

			ret = RET_OK; /* all work done */
		}
		break;

		case SDEC_IOW_DL_SEL:
		{
			LX_SDEC_DL_SEL_T params;

			err = copy_from_user((void *)&params, (void __user *)arg, sizeof(LX_SDEC_DL_SEL_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_from_user error \n");

			err = SDEC_IO_SelectPVRSource(&my_dev->stSdecParam, &params);
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "SDEC_IO_SelectPVRSource failed:[%d]", err);

			ret = RET_OK; /* all work done */
		}
		break;

		case SDEC_IOW_GET_CHIPCFG:
		{
			LX_SDEC_CHIP_CFG_T params;

			err = SDEC_IO_GetChipCfg(&my_dev->stSdecParam, &params);
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "SDEC_IO_GetChipCfg failed:[%d]", err);

			/* return chip cfg structure */
			err = copy_to_user((void __user *)arg, (void *)&params, sizeof(LX_SDEC_CHIP_CFG_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_to_user error \n");

			ret = RET_OK; /* all work done */
		}
		break;

		case SDEC_IOW_DBG_CMD:
		{
			LX_SDEC_DBG_CMD_T params;

			err = copy_from_user((void *)&params, (void __user *)arg, sizeof(LX_SDEC_DBG_CMD_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_from_user error \n");

			err = SDEC_IO_DebugCommand(&my_dev->stSdecParam, &params);
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "SDEC_IO_DebugCommand failed:[%d]", err);

			ret = RET_OK; /* all work done */
		}
		break;

		case SDEC_IOW_TSOUT_SET_SRC:
		{
			LX_SDEC_SERIALTSOUT_SET_SOURCE_T params;

			err = copy_from_user((void *)&params, (void __user *)arg, sizeof(LX_SDEC_SERIALTSOUT_SET_SOURCE_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_from_user error \n");

			err = SDEC_IO_SerialTSOUT_SetSrc(&my_dev->stSdecParam, &params);
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "SDEC_IO_SerialTSOUT_SetSrc failed:[%d]", err);

			ret = RET_OK; /* all work done */
		}
		break;

		case SDEC_IOW_BDRC_SET_DATA:
		{
			LX_SDEC_BDRC_T params;

			err = copy_from_user((void *)&params, (void __user *)arg, sizeof(LX_SDEC_BDRC_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_from_user error \n");

			err = SDEC_IO_BDRC_SetData(&my_dev->stSdecParam, &params);
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "SDEC_IO_BDRC_SetData failed:[%d]", err);

			ret = RET_OK; /* all work done */
		}
		break;

		case SDEC_IOW_BDRC_ENABLE:
		{
			LX_SDEC_BDRC_ENABLE_T params;

			err = copy_from_user((void *)&params, (void __user *)arg, sizeof(LX_SDEC_BDRC_ENABLE_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_from_user error \n");

			err = SDEC_IO_BDRC_Enable(&my_dev->stSdecParam, &params);
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "SDEC_IO_BDRC_Enable failed:[%d]", err);

			ret = RET_OK; /* all work done */
		}
		break;

		case SDEC_IOW_TSOUT_SET_BUFELEV:
		{
			LX_SDEC_SERIALTSOUT_SET_BUFELEV_T params;

			err = copy_from_user((void *)&params, (void __user *)arg, sizeof(LX_SDEC_SERIALTSOUT_SET_BUFELEV_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_from_user error \n");

			err = SDEC_IO_SerialTSOUT_SetBufELevel(&my_dev->stSdecParam, &params);
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "SDEC_IO_SerialTSOUT_SetBufELevel failed:[%d]", err);

			ret = RET_OK; /* all work done */
		}
		break;

		case SDEC_IOW_CFG_OUTPUT_PORT:
		{
			LX_SDEC_CFG_OUTPUT_T params;

			err = copy_from_user((void *)&params, (void __user *)arg, sizeof(LX_SDEC_CFG_OUTPUT_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_from_user error \n");

			err = SDEC_IO_CfgOutputPort(&my_dev->stSdecParam, &params);
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "SDEC_IO_CfgOutputPort failed:[%d]", err);

			ret = RET_OK; /* all work done */
		}
		break;

		case SDEC_IOW_BDRC_SET_WPTRUPD:
		{
			LX_SDEC_BDRC_WPTRUPD_T params;

			err = copy_from_user((void *)&params, (void __user *)arg, sizeof(LX_SDEC_BDRC_WPTRUPD_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_from_user error \n");

			err = SDEC_IO_BDRC_SetWritePtrUpdated(&my_dev->stSdecParam, &params);
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "SDEC_IO_BDRC_SetWritePtrUpdated failed:[%d]", err);

			ret = RET_OK; /* all work done */
		}
		break;

		case SDEC_IOW_CFG_IO_PORT:
		{
			LX_SDEC_CFG_IO_T params;

			err = copy_from_user((void *)&params, (void __user *)arg, sizeof(LX_SDEC_CFG_IO_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_from_user error \n");

			err = SDEC_IO_CfgIOPort(&my_dev->stSdecParam, &params);
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "SDEC_IO_CfgIOPort failed:[%d]", err);

			ret = RET_OK; /* all work done */
		}
		break;

		case SDEC_IOW_RFOUT_ENABLE: // LIVE_HEVC
		{
			LX_SDEC_RFOUT_ENABLE_T params;

			err = copy_from_user((void *)&params, (void __user *)arg, sizeof(LX_SDEC_RFOUT_ENABLE_T));
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_from_user error \n");

			err = SDEC_RFOUT_Enable(&my_dev->stSdecParam, &params);
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "SDEC_RFOUT_Enable failed:[%d]", err);

			ret = RET_OK; /* all work done */
		}
		break;


/************* CI PLUS IOCTL ************/
		case SDEC_IOW_CIPLUS_INIT:
		{
			err = SDEC_CIPLUS_Init();
			SDEC_CHECK_ERROR(err != RET_OK, goto func_exit, "SDEC_CIPLUS_Init failed[%d]", err);

			ret = RET_OK;
		}
		break;

		case SDEC_IOW_CIPLUS_CONTROL_CHANNEL:
		{
			LX_SDEC_CIPLUS_CHANNEL_CONTROL_PARAM_T params;

			SDEC_COPY_FROM_USER(&params, arg, sizeof(params), goto func_exit);

			switch(params.control)
			{
				case LX_SDEC_CIPLUS_CHANNEL_ALLOC:
					err = SDEC_CIPLUS_AllocChannel();
					SDEC_CHECK_ERROR(err < 0, goto func_exit, "SDEC_CIPLUS_AllocChannel failed[%d]", err);
					params.idx = (UINT8)err;
					SDEC_COPY_TO_USER(arg, &params, sizeof(params), goto func_exit);
					break;

				case LX_SDEC_CIPLUS_CHANNEL_OPEN:
					err = SDEC_CIPLUS_OpenChannel(params.idx);
					SDEC_CHECK_ERROR(err != RET_OK, goto func_exit, "SDEC_CIPLUS_OpenChannel failed[%d]", err);
					break;

				case LX_SDEC_CIPLUS_CHANNEL_CLOSE:
					err = SDEC_CIPLUS_CloseChannel(params.idx);
					SDEC_CHECK_ERROR(err != RET_OK, goto func_exit, "SDEC_CIPLUS_CloseChannel failed[%d]", err);
					break;

				case LX_SDEC_CIPLUS_CHANNEL_ENABLE:
					err = SDEC_CIPLUS_EnableChannel(params.idx);
					SDEC_CHECK_ERROR(err != RET_OK, goto func_exit, "SDEC_CIPLUS_EnableChannel failed[%d]", err);
					break;

				case LX_SDEC_CIPLUS_CHANNEL_DISABLE:
					err = SDEC_CIPLUS_DisableChannel(params.idx);
					SDEC_CHECK_ERROR(err != RET_OK, goto func_exit, "SDEC_CIPLUS_DisableChannel failed[%d]", err);
					break;

				default:
					SDEC_PRINT_ERROR("Invalid control[%d]\n", params.control);
					goto func_exit;
			}
			ret = RET_OK;
		}
		break;

		case SDEC_IOW_CIPLUS_CONFIG_CHANNEL:
		{
			LX_SDEC_CIPLUS_CHANNEL_CONF_PARAM_T params;

			SDEC_COPY_FROM_USER(&params, arg, sizeof(params), goto func_exit);

			err = SDEC_CIPLUS_ConfigureChannel(params.idx, &params.conf);
			SDEC_CHECK_ERROR(err != RET_OK, goto func_exit, "SDEC_CIPLUS_ConfigureChannel failed[%d]", err);

			ret = RET_OK;
		}
		break;

		case SDEC_IOW_CIPLUS_SET_BUFFER:
		{
			LX_SDEC_CIPLUS_SET_BUFFER_T params;

			SDEC_COPY_FROM_USER(&params, arg, sizeof(LX_SDEC_CIPLUS_SET_BUFFER_T), goto func_exit);

			if(params.type == LX_SDEC_CIPLUS_BUFFER_TYPE_INPUT)
				err = SDEC_CIPLUS_SetInputBuffer(params.idx, params.addr, params.size);
			else
				err = SDEC_CIPLUS_SetOutputBuffer(params.idx, params.addr, params.size);

			SDEC_CHECK_ERROR(err != RET_OK, goto func_exit, "SDEC_CIPLUS_Set___Buffer failed[%d]", err);

			ret = RET_OK;
		}
		break;

		case SDEC_IOW_CIPLUS_GET_BUFFER_PTR:
		{
			LX_SDEC_CIPLUS_BUFFER_PTR_PARAM_T params;

			SDEC_COPY_FROM_USER(&params, arg, sizeof(params), goto func_exit);

			if(params.type == LX_SDEC_CIPLUS_BUFFER_TYPE_INPUT)
				err = SDEC_CIPLUS_GetInputBufferPtr(params.idx, &params.wptr, &params.rptr);
			else
				err = SDEC_CIPLUS_GetOutputBufferPtr(params.idx, &params.wptr, &params.rptr);

			SDEC_CHECK_ERROR(err != RET_OK, goto func_exit, "SDEC_CIPLUS_GetBufferPtr failed[%d]", err);

			SDEC_COPY_TO_USER(arg, &params, sizeof(params), goto func_exit);

			ret = RET_OK;
		}
		break;

		case SDEC_IOW_CIPLUS_SET_BUFFER_PTR:
		{
			LX_SDEC_CIPLUS_BUFFER_PTR_PARAM_T params;

			SDEC_COPY_FROM_USER(&params, arg, sizeof(params), goto func_exit);

			if(params.type == LX_SDEC_CIPLUS_BUFFER_TYPE_INPUT)
				err = SDEC_CIPLUS_UpdateInputBufferWptr(params.idx, params.wptr);
			else
				err = SDEC_CIPLUS_UpdateOutputBufferRptr(params.idx, params.rptr);

			SDEC_CHECK_ERROR(err != RET_OK, goto func_exit, "SDEC_CIPLUS_SetBufferPtr failed[%d]", err);

			ret = RET_OK;
		}
		break;

		case SDEC_IOW_CIPLUS_ADD_PID_FILTER:
		{
			LX_SDEC_CIPLUS_PID_FILTER_T params;

			SDEC_COPY_FROM_USER(&params, arg, sizeof(params), goto func_exit);

			ret = SDEC_CIPLUS_AddPidFilter(params.idx, params.pid, params.npid);
			SDEC_CHECK_ERROR(ret < 0, goto func_exit, "SDEC_CIPLUS_AddPidFilter failed[%d]", ret);
		}
		break;

		case SDEC_IOW_CIPLUS_REMOVE_PID_FILTER:
		{
			LX_SDEC_CIPLUS_PID_FILTER_T params;

			SDEC_COPY_FROM_USER(&params, arg, sizeof(params), goto func_exit);

			ret = SDEC_CIPLUS_RemovePidFilter(params.idx, params.pid, params.npid);
			SDEC_CHECK_ERROR(ret < 0, goto func_exit, "SDEC_CIPLUS_RemovePidFilter failed[%d]", ret);
		}
		break;

		case SDEC_IOW_CIPLUS_GET_PID_FILTER:
		{
			LX_SDEC_CIPLUS_PID_FILTER_T params;
			int npid;

			SDEC_COPY_FROM_USER(&params, arg, sizeof(params), goto func_exit);

			npid = SDEC_CIPLUS_GetPidFilter(params.idx, params.pid,params.npid);
			SDEC_CHECK_ERROR(npid < 0, goto func_exit, "SDEC_CIPLUS_GetPidFilter failed[%d]", npid);

			params.npid = npid;
			SDEC_COPY_TO_USER(arg, &params, sizeof(params), goto func_exit);

			ret = RET_OK; /* all work done */
		}
		break;

		case SDEC_IOW_CIPLUS_SET_OUTPUT_CLOCK:
		{
			LX_SDEC_CIPLUS_OUT_CLOCK_T params;

			SDEC_COPY_FROM_USER(&params, arg, sizeof(params), goto func_exit);

			ret = SDEC_CIPLUS_SetOutClock(params);
			SDEC_CHECK_ERROR(ret < 0, goto func_exit, "SDEC_CIPLUS_SetOutClock failed[%d]", ret);
		}
		break;

		case SDEC_IOW_CIPLUS_MM_GETINFO:
		{
			LX_SDEC_MEM_INFO_T info;

			err = SDEC_MM_GetMemInfo(SDEC_MM_TYPE_CIPLUS, &info);
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "SDEC_IOW_MM_GETINFO failed[%d]", err);

			/* return mem base and size */
			SDEC_COPY_TO_USER(arg, &info, sizeof(LX_SDEC_MEM_INFO_T), goto func_exit);

			ret = RET_OK; /* all work done */
		}
		break;

		case SDEC_IOW_CIPLUS_MM_FREE:
		{
			UINT32 params;

			SDEC_COPY_FROM_USER(&params, arg, sizeof(UINT32), goto func_exit);

			err = SDEC_MM_Free(priv->mm, SDEC_MM_TYPE_CIPLUS, (void*)params);
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "SDEC_MM_Free failed[%d]", err);

			ret = RET_OK; /* all work done */
		}
		break;

		case SDEC_IOR_CIPLUS_MM_ALLOC:
		{
			LX_SDEC_MEM_INFO_T params;
			void* ptr;

			SDEC_COPY_FROM_USER(&params, arg, sizeof(LX_SDEC_MEM_INFO_T), goto func_exit);

			ptr = SDEC_MM_Alloc(priv->mm, SDEC_MM_TYPE_CIPLUS, params.size);
			LX_SDEC_CHECK_CODE( ptr == NULL, goto func_exit, "SDEC_MM_Alloc failed[%d]", err);

			params.addr = (UINT32)ptr;

			/* return mem address and size */
			SDEC_COPY_TO_USER(arg, &params, sizeof(LX_SDEC_MEM_INFO_T), goto func_exit);

			ret = RET_OK; /* all work done */
		}
		break;

		case SDEC_IOW_CIPLUS_MM_CACHE_CTRL:
		{
			LX_SDEC_CACHE_CTRL_T cache_ctrl;
			OS_CACHE_MAP_T cache_map;

			SDEC_COPY_FROM_USER(&cache_ctrl, arg, sizeof(LX_SDEC_CACHE_CTRL_T), goto func_exit);

			cache_map.phys_addr = (ULONG)cache_ctrl.paddr;
			cache_map.virt_addr = (ULONG)cache_ctrl.vaddr;
			cache_map.length = cache_ctrl.size;
			switch (cache_ctrl.op)
			{
				case LX_SDEC_CACHE_OP_INVAL:
					OS_InvalCacheMap(&cache_map, 0, cache_ctrl.size);
					break;

				case LX_SDEC_CACHE_OP_CLEAN:
					OS_CleanCacheMap(&cache_map, 0, cache_ctrl.size);
					break;

				default:
					SDEC_PRINT_ERROR("invalid cache op 0x%x\n", cache_ctrl.op );
					goto func_exit;
			}

			ret = RET_OK; /* all work done */
		}
		break;

		case SDEC_IOR_CIPLUS_MM_GET_STAT:
		{
			LX_SDEC_MM_STAT_T params;

			err = SDEC_MM_GetMemStat(SDEC_MM_TYPE_CIPLUS, &params);
			LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "SDEC_MM_GetMemStat failed:[%d]", err);

			/* return mem address and size , allocation and free status */
			SDEC_COPY_TO_USER(arg, &params, sizeof(LX_SDEC_MM_STAT_T), goto func_exit);

			ret = RET_OK; /* all work done */
		}
		break;

	    default:
	    {
			DBG_PRINT_WARNING("invalid command[0x%08x]. magic=0x%02X, num=%d", cmd, _IOC_TYPE(cmd), _IOC_NR(cmd));

			/* redundant check but it seems more readable */
    	    ret = -ENOTTY;
		}
		break;
    }

func_exit:
    return ret;
}


static int 	SDEC_Mmap (struct file *filep, struct vm_area_struct *vma )
{
	SDEC_PRINT_INFO("MMAP : start - %08lx , end - %08lx , off - %08lx , flags - %lx\n" ,
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


static ssize_t  SDEC_Read(
			struct file *i_pstrFilp, 				/* file pointer representing driver instance */
			SINT8        *o_pcBufferToLoad,     	/* buffer from user space */
			size_t      i_uiSizeToRead,    		/* size of buffer in bytes*/
			loff_t 		*i_FileOffset  )  		/* offset in the file */
{
	SDEC_DEVICE_T		*my_dev;
	S_SDEC_PARAM_T 	*stpSdecParam;
	SDEC_PRIV_T *priv = i_pstrFilp->private_data;

	UINT8 ui8GpbInfoWIdx = 0, ui8GpbInfoHeadCnt = 0, ui8GpbInfoRIdx = 0;
	UINT8 ui8DataCnt = 0;

	my_dev = priv->dev;
	stpSdecParam = &(my_dev->stSdecParam);
	//while(1)
	{
		//interruptible_sleep_on(&my_dev->stSdecParam.wait_command_queue[0]);
		/* if there is pending signal, return error code */
		//if(signal_pending(current))	return -EIO;

		wait_event_interruptible(stpSdecParam->wq, stpSdecParam->wq_condition);
		stpSdecParam->wq_condition = 0;

		ui8GpbInfoWIdx = stpSdecParam->ui8GpbInfoWIdx;
		ui8GpbInfoRIdx = stpSdecParam->ui8GpbInfoRIdx;

		/* there is no data. forbid false alarm */
		//if(ui8GpbInfoWIdx == ui8GpbInfoRIdx) continue;
		if(ui8GpbInfoWIdx == ui8GpbInfoRIdx) return -EAGAIN;

		/* wrap around happen */
		if(ui8GpbInfoWIdx < ui8GpbInfoRIdx)
		{
			SDEC_PRINT_READ( "wrap around happen");

			/*
			 *                                ui8GpbInfoRIdx
			 *                                       V
			 * |---------------------------------------------------------|
			 * |                                     | ui8GpbInfoHeadCnt |
			 * |---------------------------------------------------------|
			 * |   ui8GpbInfoWIdx |                                      |
			 * |---------------------------------------------------------|
			 *                    ^
			 *              ui8GpbInfoWIdx
			 */
			ui8GpbInfoHeadCnt 	= LX_SDEC_MAX_GPB_DATA - ui8GpbInfoRIdx;
			ui8DataCnt 			= ui8GpbInfoHeadCnt + ui8GpbInfoWIdx;

			SDEC_PRINT_READ( "Origianl #of data(#of head data)[%d] #of tail data [%d] ui8GpbInfoRIdx : [%d]",ui8GpbInfoWIdx, ui8GpbInfoHeadCnt,ui8GpbInfoRIdx);

			/* head copy */
			if(copy_to_user( o_pcBufferToLoad,
				&stpSdecParam->stGPBInfo[ui8GpbInfoRIdx],
				sizeof(LX_SDEC_NOTIFY_PARAM_T)*ui8GpbInfoHeadCnt))
	 			return -EFAULT;

			/* tail copy */
			if(copy_to_user( o_pcBufferToLoad + sizeof(LX_SDEC_NOTIFY_PARAM_T)*ui8GpbInfoHeadCnt,
				&stpSdecParam->stGPBInfo[0],
				sizeof(LX_SDEC_NOTIFY_PARAM_T)*ui8GpbInfoWIdx))
	 			return -EFAULT;

		}
		else
		{
			SDEC_PRINT_READ( "no wrap around happen");

			ui8DataCnt = ui8GpbInfoWIdx - ui8GpbInfoRIdx;
			SDEC_PRINT_READ( "ui8GpbInfoRIdx : [%d] ui8GpbInfoWIdx : [%d] #of data[%d]",ui8GpbInfoRIdx, ui8GpbInfoWIdx, ui8DataCnt);

			/* copy buffer to user */
			if(copy_to_user(o_pcBufferToLoad, &stpSdecParam->stGPBInfo[ui8GpbInfoRIdx],
				sizeof(LX_SDEC_NOTIFY_PARAM_T)*ui8DataCnt))
	 			return -EFAULT;
		}

		//break;
	}

	stpSdecParam->ui8GpbInfoRIdx = ui8GpbInfoWIdx;

	return sizeof(LX_SDEC_NOTIFY_PARAM_T)*ui8DataCnt;

}


static void SDEC_Timer(ULONG param)
{
	SDEC_PollThread((void*)param);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef KDRV_GLOBAL_LINK
#if defined(CONFIG_LG_BUILTIN_KDRIVER) && defined(CONFIG_LGSNAP)
user_initcall_grp("kdrv",SDEC_Init);
#else
module_init(SDEC_Init);
#endif
module_exit(SDEC_Cleanup);

//MODULE_AUTHOR("LGE");
//MODULE_AUTHOR("LGE");
MODULE_DESCRIPTION("base driver");
MODULE_LICENSE("GPL");
#endif

/** @} */

