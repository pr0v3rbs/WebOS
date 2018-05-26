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
 *  author		dj911.kim@lge.com
 *  version		1.0
 *  date		2012.09.09
 *  note		Additional information.
 *
 *  @addtogroup lg115x_ovi
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
#include <linux/ioport.h>	/**< For request_region, check_region etc */
#include <asm/io.h>			/**< For ioremap_nocache */
#include <linux/dma-mapping.h>
#include <linux/platform_device.h>
#include "os_util.h"
#include "base_device.h"

#include "ovi_dbg.h"
#include "ovi_hal.h"
#include "ovi_drv.h"
#include "ovi_cfg.h"

#include "../be/video_status_keeper.h"
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
 *	main control block for ovi device.
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
OVI_DEVICE_T;

typedef struct
{
	bool			is_suspended;
}
OVI_DRVDATA_T;

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/
extern void OVI_PROC_Init(void);
extern void OVI_PROC_Cleanup(void);

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Functions
----------------------------------------------------------------------------------------*/
int OVI_Init(void);
void	OVI_Cleanup(void);

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/
int	g_ovi_debug_fd;
int 	g_ovi_major = OVI_MAJOR;
int 	g_ovi_minor = OVI_MINOR;

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/
static int OVI_Open(struct inode *, struct file *);
static int OVI_Close(struct inode *, struct file *);
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)
static int OVI_Ioctl (struct inode *, struct file *, unsigned int, unsigned long );
#else
static long OVI_Ioctl (struct file * filp, unsigned int cmd, unsigned long arg);
#endif
static int OVI_MMap(struct file *, struct vm_area_struct *);

static int OVI_IO_GetDateInfo(unsigned long arg);
static int OVI_IO_SetCombination(unsigned long arg);
static int OVI_IO_SetSpreadSpectrum(unsigned long arg);
static int OVI_IO_SetOdLut(unsigned long arg);
static int OVI_IO_SetFrameOdcEnable(unsigned long arg);
static int OVI_IO_SetColorTempEnable(unsigned long arg);
static int OVI_IO_SetGammaEnable(unsigned long arg);
static int OVI_IO_SetDitherEnable(unsigned long arg);
static int OVI_IO_SetLineOdcMode(unsigned long arg);
static int OVI_IO_SetDitherMode(unsigned long arg);
static int OVI_IO_SetTconReverse(unsigned long arg);
static int OVI_IO_SetLvdsReverse(unsigned long arg);
static int OVI_IO_SetColorTemp(unsigned long arg);
static int OVI_IO_SetGamma(unsigned long arg);
static int OVI_IO_SetPatternGenerator(unsigned long arg);
static int OVI_IO_GetOdLut(unsigned long arg);
static int OVI_IO_SetOdByteData(unsigned long arg);
static int OVI_IO_SetChannelPower(unsigned long arg);
static int OVI_IO_SetLvdsFormat(unsigned long arg);
static int OVI_IO_SetLvdsBitDepth(unsigned long arg);
static int OVI_IO_SetLvdsBlack(unsigned long arg);
static int OVI_IO_SetClock(unsigned long arg);
static int OVI_IO_DisplayResume(unsigned long arg);

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/

/**
 * main control block for ovi device
*/
static OVI_DEVICE_T*		g_ovi_device;

/**
 * file I/O description for ovi device
 *
*/
static struct file_operations g_ovi_fops =
{
	.open 	= OVI_Open,
	.release= OVI_Close,
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)
	.ioctl	= OVI_Ioctl,
#else
	.unlocked_ioctl	= OVI_Ioctl,
#endif
	.mmap	= OVI_MMap,
#ifdef SUPPORT_OVI_DEVICE_READ_WRITE_FOPS
	.read 	= OVI_Read,
	.write 	= OVI_Write,
#else
	.read	= NULL,
	.write	= NULL,
#endif
};

/*========================================================================================
	Implementation Group
========================================================================================*/
#ifdef KDRV_CONFIG_PM
/**
 *
 * suspending module.
 *
 * @param	struct platform_device *pdev pm_message_t state
 * @return	int 0 : OK , -1 : NOT OK
 *
 */
static int OVI_suspend(struct platform_device *pdev, pm_message_t state)
{
#ifdef KDRV_CONFIG_PM_OVI
	OVI_DRVDATA_T *drv_data;

	printk("OVI - suspend BEGIN\n");

	drv_data = platform_get_drvdata(pdev);

	drv_data->is_suspended = 1;

	printk("OVI - suspend OK\n");
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
static int OVI_resume(struct platform_device *pdev)
{
#ifdef KDRV_CONFIG_PM_OVI
	OVI_DRVDATA_T *drv_data;

	printk("OVI - resume BEGIN\n");

	drv_data = platform_get_drvdata(pdev);

	if(drv_data->is_suspended == 0)
	{
		printk("OVI - resume FAIL\n");
		return -1;
	}

	drv_data->is_suspended = 0;

	if(OVI_HAL_Resume())
	{
		printk("OVI - resume FAIL\n");
		return -1;
	}

	printk("OVI - resume OK\n");
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
 int  OVI_probe(struct platform_device *pdev)
{

	OVI_DRVDATA_T *drv_data=NULL;

	drv_data = (OVI_DRVDATA_T *)kmalloc(sizeof(OVI_DRVDATA_T) , GFP_KERNEL);
	if(drv_data == NULL) {
		OVI_PRINT("memory allocation failed\n");
		return -1;
	}

	OVI_PRINT("[%s] done probe\n", OVI_MODULE);

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
static int  OVI_remove(struct platform_device *pdev)
{
	OVI_DRVDATA_T *drv_data;

	drv_data = platform_get_drvdata(pdev);
	kfree(drv_data);

	OVI_PRINT("[%d] removed\n", OVI_MODULE);

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
static void  OVI_release(struct device *dev)
{
	OVI_PRINT("[%s] device released\n", OVI_MODULE);
}

/*
 *	module platform driver structure
 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,39)
static struct platform_driver ovi_driver =
#else
static struct platform_driver ovi_driver __refdata =
#endif
{
	.probe          = OVI_probe,
	.suspend        = OVI_suspend,
	.remove         = OVI_remove,
	.resume         = OVI_resume,
	.driver         =
	{
		.name   = OVI_MODULE,
	},
};

static struct platform_device ovi_device = {
	.name = OVI_MODULE,
	.id =  0,
	.id = -1,
	.dev = {
		.release = OVI_release,
	},
};
#endif

/** Initialize the device environment before the real H/W initialization
 *
 *  @note main usage of this function is to initialize the HAL layer and memory size adjustment
 *  @note it's natural to keep this function blank :)
 */
void OVI_PreInit(void)
{

}

int OVI_Init(void)
{
	int			i;
	int			err;
	dev_t		dev;

	OVI_PRINT("Attempting to open OVI device...\n");

	g_ovi_debug_fd = DBG_OPEN( OVI_MODULE );
	if (g_ovi_debug_fd < 0) {
		DBG_PRINT_ERROR("can't open OVI module\n" );
		return -EIO;
	}
	OS_DEBUG_EnableModule ( g_ovi_debug_fd );

	OS_DEBUG_EnableModuleByIndex ( g_ovi_debug_fd, OVI_DBG_PRT_IDX_PRINT, DBG_COLOR_NONE ); 		// OVI_PRINT
	OS_DEBUG_EnableModuleByIndex ( g_ovi_debug_fd, OVI_DBG_PRT_IDX_ERROR, DBG_COLOR_UL_RED );	// OVI_ERROR
	OS_DEBUG_DisableModuleByIndex ( g_ovi_debug_fd, OVI_DBG_PRT_IDX_DEBUG ); 						// OVI_DEBUG

#ifdef KDRV_CONFIG_PM
	if(platform_driver_register(&ovi_driver) < 0)
	{
		OVI_PRINT("[%s] platform driver register failed\n",OVI_MODULE);
	}
	else
	{
		if(platform_device_register(&ovi_device))
		{
			platform_driver_unregister(&ovi_driver);
			OVI_PRINT("[%s] platform device register failed\n",OVI_MODULE);
		}
		else
		{
			OVI_PRINT("[%s] platform register done\n", OVI_MODULE);
		}

	}
#endif

	g_ovi_device = (OVI_DEVICE_T*)OS_KMalloc( sizeof(OVI_DEVICE_T)*OVI_MAX_DEVICE );

	if ( NULL == g_ovi_device )
	{
		DBG_PRINT_ERROR("out of memory. can't allocate %d bytes\n", sizeof(OVI_DEVICE_T)* OVI_MAX_DEVICE );
		return -ENOMEM;
	}

	memset( g_ovi_device, 0x0, sizeof(OVI_DEVICE_T)* OVI_MAX_DEVICE );

	if (g_ovi_major)
	{
		dev = MKDEV( g_ovi_major, g_ovi_minor );
		err = register_chrdev_region(dev, OVI_MAX_DEVICE, OVI_MODULE );
	}
	else
	{
		err = alloc_chrdev_region(&dev, g_ovi_minor, OVI_MAX_DEVICE, OVI_MODULE );
		g_ovi_major = MAJOR(dev);
	}

	if ( err < 0 )
	{
		DBG_PRINT_ERROR("can't register ovi device\n" );
		return -EIO;
	}

	for ( i=0; i<OVI_MAX_DEVICE; i++ )
	{
		dev = MKDEV( g_ovi_major, g_ovi_minor+i );
		cdev_init( &(g_ovi_device[i].cdev), &g_ovi_fops );
		g_ovi_device[i].devno		= dev;
		g_ovi_device[i].cdev.owner = THIS_MODULE;
		g_ovi_device[i].cdev.ops   = &g_ovi_fops;

		err = cdev_add (&(g_ovi_device[i].cdev), dev, 1 );

		if (err)
		{
			DBG_PRINT_ERROR("error (%d) while adding ovi device (%d.%d)\n", err, MAJOR(dev), MINOR(dev) );
			return -EIO;
		}
        OS_CreateDeviceClass ( g_ovi_device[i].devno, "%s%d", OVI_MODULE, i );
	}

	OVI_InitCfg();
	OVI_PROC_Init();
	OVI_HAL_Init();
	VIDEO_SKeeperInit();

	OVI_PRINT("ovi device initialize success\n");

	return 0;
}

void OVI_Cleanup(void)
{
	int i;
	dev_t dev = MKDEV( g_ovi_major, g_ovi_minor );

#ifdef KDRV_CONFIG_PM
	platform_driver_unregister(&ovi_driver);
	platform_device_unregister(&ovi_device);
#endif

	OVI_PROC_Cleanup( );

	for ( i=0; i<OVI_MAX_DEVICE;i++)
	{
		cdev_del( &(g_ovi_device[i].cdev) );
	}

	unregister_chrdev_region(dev, OVI_MAX_DEVICE );

	OS_Free( g_ovi_device );
}


static int OVI_Open(struct inode *inode, struct file *filp)
{
	int					major,minor;
	struct cdev*    	cdev;
	OVI_DEVICE_T*	    my_dev;

    	cdev	= inode->i_cdev;
    	my_dev	= container_of ( cdev, OVI_DEVICE_T, cdev);

	OVI_PRINT("%s entered \n" , __func__);

    	my_dev->dev_open_count++;
    	filp->private_data = my_dev;

	/* some debug */
    	major = imajor(inode);
    	minor = iminor(inode);
    	OVI_PRINT("device opened (%d:%d)\n", major, minor );

    	return 0;
}

/**
 * release handler for ovi device
 *
 */
static int OVI_Close(struct inode *inode, struct file *file)
{
	int					major,minor;
	OVI_DEVICE_T*   	my_dev;
	struct cdev*		cdev;

	OVI_PRINT("%s entered \n" , __func__);
	cdev	= inode->i_cdev;
	my_dev	= container_of ( cdev, OVI_DEVICE_T, cdev);

	if ( my_dev->dev_open_count > 0 )
	{
		--my_dev->dev_open_count;
	}

	major = imajor(inode);
	minor = iminor(inode);

	OVI_PRINT("device closed (%d:%d)\n", major, minor );
	return 0;
}

/**
 * memory mapping to virtual region
 *
 */
static int OVI_MMap(struct file *file, struct vm_area_struct *vma)
{
#if 0
	unsigned long start;
	unsigned long len;
	unsigned long off = ((vma->vm_pgoff) << PAGE_SHIFT);

	OVI_PRINT("%s entered \n" , __func__);

	start = gstMemCfgOVI->buf_memory_base & PAGE_MASK;
	len = PAGE_ALIGN((start & ~PAGE_MASK) + gstMemCfgOVI->buf_memory_size);

	OVI_PRINT("MMAP : start - %08lx , len - %08lx , off - %08lx " , start , len , off);

	if ((vma->vm_end - vma->vm_start + off) > len)
	{
	 	OVI_PRINT("ERROR : GFX_SurfaceMmap leng is over\n");
	 	OVI_PRINT("start : %lx end : %lx offset : %lx len : %lx\n" ,
	 		vma->vm_start , vma->vm_end , off , len);

        	return -EINVAL;
	}

	off += start;

	OVI_PRINT("MMAP : start - %08lx , len - %08lx , off - %08lx " , start , len , vma->vm_pgoff);

	vma->vm_pgoff = off >> PAGE_SHIFT;
	vma->vm_flags |= VM_IO;
	vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);

	return io_remap_pfn_range(vma, vma->vm_start, off >> PAGE_SHIFT,
	                           vma->vm_end - vma->vm_start,
	                           vma->vm_page_prot);
#endif
	return 0;
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)
static int
OVI_Ioctl ( struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg )
#else
static long
OVI_Ioctl (struct file * filp, unsigned int cmd, unsigned long arg)
#endif
{
	int err = 0, ret = RET_OK;

	OVI_DEVICE_T*	    my_dev;
	struct cdev*		cdev;
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)
#else
	struct inode *inode = filp->f_path.dentry->d_inode;
#endif

	/* For Status Keeper */
	LX_VP_ST_KEEPER_MODULE_TYPE_T	mtype = LX_VP_ST_KEEPER_MODULE_OVI;
	LX_VP_ST_KEEPER_HISTORY_TYPE_T	htype = LX_VP_ST_KEEPER_HISTORY_IO;
	LX_VP_ST_KEEPER_HISTORY_INDEX_T pData;

	cdev	= inode->i_cdev;
	my_dev	= container_of ( cdev, OVI_DEVICE_T, cdev);

	/*
	* check if IOCTL command is valid or not.
	* - if magic value doesn't match, return error (-ENOTTY)
	* - if command is out of range, return error (-ENOTTY)
	*
	* note) -ENOTTY means "Inappropriate ioctl for device.
	*/
	if (_IOC_TYPE(cmd) != LX_OVI_IOC_MAGIC)
	{
		DBG_PRINT_WARNING("invalid magic. magic=0x%02X\n", _IOC_TYPE(cmd) );
		return -ENOTTY;
	}
	if (_IOC_NR(cmd) > LX_OVI_IOC_MAXNR)
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

	/* Load Status Keeper data */
	pData.ioctl = (unsigned int)cmd;
	pData.pData = (unsigned int)arg;
	pData.size  = _IOC_SIZE(cmd);

	VIDEO_SKeeper_PushHistoryLog(mtype, htype, &pData);

	switch(cmd)
	{
		case LX_OVI_IOW_SET_COMBINATION:
			ret = OVI_IO_SetCombination(arg);
			break;

		case LX_OVI_IOW_SET_SPREAD_SPECTRUM:
			ret = OVI_IO_SetSpreadSpectrum(arg);
			break;

		case LX_OVI_IOW_SET_OD_LUT:
			ret = OVI_IO_SetOdLut(arg);
			break;

		case LX_OVI_IOW_SET_FRAMEODC_ENABLE:
			ret = OVI_IO_SetFrameOdcEnable(arg);
			break;

		case LX_OVI_IOW_SET_COLORTEMP_ENABLE:
			ret = OVI_IO_SetColorTempEnable(arg);
			break;

		case LX_OVI_IOW_SET_GAMMA_ENABLE:
			ret = OVI_IO_SetGammaEnable(arg);
			break;

		case LX_OVI_IOW_SET_DITHER_ENABLE:
			ret = OVI_IO_SetDitherEnable(arg);
			break;

		case LX_OVI_IOW_SET_LINEODC_MODE:
			ret = OVI_IO_SetLineOdcMode(arg);
			break;

		case LX_OVI_IOW_SET_DITHER_MODE:
			ret = OVI_IO_SetDitherMode(arg);
			break;

		case LX_OVI_IOW_SET_COLOR_TEMP:
			ret = OVI_IO_SetColorTemp(arg);
			break;

		case LX_OVI_IOW_SET_GAMMA:
			ret = OVI_IO_SetGamma(arg);
			break;

		case LX_OVI_IORW_GET_TCON_OD_LUT:
			ret = OVI_IO_GetOdLut(arg);
			break;

		case LX_OVI_IOW_SET_TCON_BYTE_OD_WRITE:
			ret = OVI_IO_SetOdByteData(arg);
			break;

		case LX_OVI_IOW_SET_TCON_REVERSE:
			ret = OVI_IO_SetTconReverse(arg);
			break;

		case LX_OVI_IOW_SET_LVDS_REVERSE:
			ret = OVI_IO_SetLvdsReverse(arg);
			break;

		case LX_OVI_IOW_SET_TCON_TPG:
			ret = OVI_IO_SetPatternGenerator(arg);
			break;

		case LX_OVI_IOW_SET_CHANNEL_POWER:
			ret = OVI_IO_SetChannelPower(arg);
			break;

		case LX_OVI_IOW_SET_LVDS_FORMAT:
			ret = OVI_IO_SetLvdsFormat(arg);
			break;

		case LX_OVI_IOW_SET_LVDS_BIT:
			ret = OVI_IO_SetLvdsBitDepth(arg);
			break;

		case LX_OVI_IOW_SET_LVDS_BLACK:
			ret = OVI_IO_SetLvdsBlack(arg);
			break;

		case LX_OVI_IOR_GET_DATE_INFO:
			ret = OVI_IO_GetDateInfo(arg);
			break;

		case LX_OVI_IOW_SET_TCON_CLOCK:
			ret = OVI_IO_SetClock(arg);
			break;

		case LX_OVI_DISPLAY_RESUME:
			ret = OVI_IO_DisplayResume(arg);
			break;

	    	default:
	    	{
			/* redundant check but it seems more readable */
    	    		ret = -ENOTTY;
		}
    	}
    	return ret;
}

static int OVI_IO_GetDateInfo(unsigned long arg)
{
	int ret = RET_OK;

	do{
		ret = OVI_HAL_GetDateInfo();
		if(ret) OVI_BREAK_WRONG(ret);
	} while(0);
	return ret;
}

static int OVI_IO_SetCombination(unsigned long arg)
{
	int ret = RET_OK;
	UINT32 stParams;

	do{
		ret = copy_from_user(&stParams, (UINT32 *)arg, sizeof(UINT32));
		if(ret) OVI_BREAK_WRONG(ret);
		ret = OVI_HAL_SetCombination(stParams);
		if(ret) OVI_BREAK_WRONG(ret);
	} while(0);
	return ret;
}

static int OVI_IO_SetSpreadSpectrum(unsigned long arg)
{
	int ret = RET_OK;
	LX_OVI_SPREAD_SPECTRUM_T stParams;

	do{
		ret = copy_from_user(&stParams, (LX_OVI_SPREAD_SPECTRUM_T *)arg, sizeof(LX_OVI_SPREAD_SPECTRUM_T));
		if(ret) OVI_BREAK_WRONG(ret);
		ret = OVI_HAL_SetSpreadSpectrum(&stParams);
		if(ret) OVI_BREAK_WRONG(ret);
	} while(0);
	return ret;
}

static int OVI_IO_SetOdLut(unsigned long arg)
{
	int ret = RET_OK;
	LX_OVI_TCON_OD_DWLD_T stParams;

	do{
		ret = copy_from_user(&stParams, (LX_OVI_TCON_OD_DWLD_T *)arg, sizeof(LX_OVI_TCON_OD_DWLD_T));
		if(ret) OVI_BREAK_WRONG(ret);
		ret = OVI_HAL_SetOdLut(&stParams);
		if(ret) OVI_BREAK_WRONG(ret);
	} while(0);
	return ret;
}

static int OVI_IO_SetFrameOdcEnable(unsigned long arg)
{
	int ret = RET_OK;
	BOOLEAN bParams;

	do{
		ret = copy_from_user(&bParams, (BOOLEAN *)arg, sizeof(BOOLEAN));
		if(ret) OVI_BREAK_WRONG(ret);
		ret = OVI_HAL_SetFrameOdcEnable(bParams);
		if(ret) OVI_BREAK_WRONG(ret);
	} while(0);
	return ret;
}

static int OVI_IO_SetColorTempEnable(unsigned long arg)
{
	int ret = RET_OK;
	BOOLEAN bParams;

	do{
		ret = copy_from_user(&bParams, (BOOLEAN *)arg, sizeof(BOOLEAN));
		if(ret) OVI_BREAK_WRONG(ret);
		ret = OVI_HAL_SetColorTempEnable(bParams);
		if(ret) OVI_BREAK_WRONG(ret);
	} while(0);
	return ret;
}

static int OVI_IO_SetGammaEnable(unsigned long arg)
{
	int ret = RET_OK;
	BOOLEAN bParams;

	do{
		ret = copy_from_user(&bParams, (BOOLEAN *)arg, sizeof(BOOLEAN));
		if(ret) OVI_BREAK_WRONG(ret);
		ret = OVI_HAL_SetGammaEnable(bParams);
		if(ret) OVI_BREAK_WRONG(ret);
	} while(0);
	return ret;
}

static int OVI_IO_SetDitherEnable(unsigned long arg)
{
	int ret = RET_OK;
	BOOLEAN bParams;

	do{
		ret = copy_from_user(&bParams, (BOOLEAN *)arg, sizeof(BOOLEAN));
		if(ret) OVI_BREAK_WRONG(ret);
		ret = OVI_HAL_SetDitherEnable(bParams);
		if(ret) OVI_BREAK_WRONG(ret);
	} while(0);
	return ret;
}


static int OVI_IO_SetLineOdcMode(unsigned long arg)
{
	int ret = RET_OK;
	LX_OVI_TCON_LINEOD_MODE_T stParams;

	do{
		ret = copy_from_user(&stParams, (LX_OVI_TCON_LINEOD_MODE_T *)arg, sizeof(LX_OVI_TCON_LINEOD_MODE_T));
		if(ret) OVI_BREAK_WRONG(ret);
		ret = OVI_HAL_SetLineOdcMode(&stParams);
		if(ret) OVI_BREAK_WRONG(ret);
	} while(0);
	return ret;
}

static int OVI_IO_SetDitherMode(unsigned long arg)
{
	int ret = RET_OK;
	LX_OVI_TCON_DITHER_T enParams;

	do {
		ret = copy_from_user(&enParams, (LX_OVI_TCON_DITHER_T *)arg, sizeof(LX_OVI_TCON_DITHER_T));
		if(ret) OVI_BREAK_WRONG(ret);
		ret = OVI_HAL_SetDitherMode(enParams);
		if(ret) OVI_BREAK_WRONG(ret);
	}while(0);
	return ret;
}

static int OVI_IO_SetColorTemp(unsigned long arg)
{
	int ret = RET_OK;
	LX_OVI_TCON_COLOR_TEMP_T stParams;

	do {
		ret = copy_from_user(&stParams, (LX_OVI_TCON_COLOR_TEMP_T *)arg, sizeof(LX_OVI_TCON_COLOR_TEMP_T));
		if(ret) OVI_BREAK_WRONG(ret);
		ret = OVI_HAL_SetColorTemp(&stParams);
		if(ret) OVI_BREAK_WRONG(ret);
	}while(0);
	return ret;
}

static int OVI_IO_SetGamma(unsigned long arg)
{
	int ret = RET_OK;
	LX_OVI_TCON_GAMMA_T stParams;

	do {
		ret = copy_from_user(&stParams, (LX_OVI_TCON_GAMMA_T *)arg, sizeof(LX_OVI_TCON_GAMMA_T));
		if(ret) OVI_BREAK_WRONG(ret);
		ret = OVI_HAL_SetGamma(&stParams);
		if(ret) OVI_BREAK_WRONG(ret);
	}while(0);
	return ret;
}

static int OVI_IO_GetOdLut(unsigned long arg)
{
	int ret = RET_OK;
	LX_OVI_TCON_OD_DWLD_T stParams;

	do {
		ret = copy_from_user(&stParams, (LX_OVI_TCON_OD_DWLD_T *)arg, sizeof(LX_OVI_TCON_OD_DWLD_T));
		if(ret) OVI_BREAK_WRONG(ret);
		ret = OVI_HAL_GetOdLut(&stParams);
		if(ret) OVI_BREAK_WRONG(ret);
		ret = copy_to_user((void __user *)arg, &stParams, sizeof(LX_OVI_TCON_OD_DWLD_T));
		if(ret) OVI_BREAK_WRONG(ret);
	}while(0);
	return ret;
}

static int OVI_IO_SetOdByteData(unsigned long arg)
{
	int ret = RET_OK;
	LX_OVI_TCON_OD_LUT_DEBUG_T stParams;

	do {
		ret = copy_from_user(&stParams, (LX_OVI_TCON_OD_LUT_DEBUG_T *)arg, sizeof(LX_OVI_TCON_OD_LUT_DEBUG_T));
		if(ret) OVI_BREAK_WRONG(ret);
		ret = OVI_HAL_SetOdByteData(&stParams);
		if(ret) OVI_BREAK_WRONG(ret);
	}while(0);
	return ret;
}

static int OVI_IO_SetTconReverse(unsigned long arg)
{
	int ret = RET_OK;
	BOOLEAN bParams;

	do {
		ret = copy_from_user(&bParams, (BOOLEAN *)arg, sizeof(BOOLEAN));
		if(ret) OVI_BREAK_WRONG(ret);
		ret = OVI_HAL_SetTconReverse(bParams);
		if(ret) OVI_BREAK_WRONG(ret);
	}while(0);
	return ret;
}

static int OVI_IO_SetLvdsReverse(unsigned long arg)
{
	int ret = RET_OK;
	BOOLEAN bParams;

	do {
		ret = copy_from_user(&bParams, (BOOLEAN *)arg, sizeof(BOOLEAN));
		if(ret) OVI_BREAK_WRONG(ret);
		ret = OVI_HAL_SetLvdsReverse(bParams);
		if(ret) OVI_BREAK_WRONG(ret);
	}while(0);
	return ret;
}

static int OVI_IO_SetPatternGenerator(unsigned long arg)
{
	int ret = RET_OK;
	LX_OVI_TCON_TPG_MODE_T enParams;

	do {
		ret = copy_from_user(&enParams, (LX_OVI_TCON_TPG_MODE_T *)arg, sizeof(LX_OVI_TCON_TPG_MODE_T));
		if(ret) OVI_BREAK_WRONG(ret);
		ret = OVI_HAL_SetPatternGenerator(enParams);
		if(ret) OVI_BREAK_WRONG(ret);
	}while(0);
	return ret;
}

static int OVI_IO_SetChannelPower(unsigned long arg)
{
	int ret = RET_OK;
	BOOLEAN bParams;

	do {
		ret = copy_from_user(&bParams, (BOOLEAN *)arg, sizeof(BOOLEAN));
		if(ret) OVI_BREAK_WRONG(ret);
		ret = OVI_HAL_SetChannelPower(bParams);
		if(ret) OVI_BREAK_WRONG(ret);
	}while(0);
	return ret;
}

static int OVI_IO_SetLvdsFormat(unsigned long arg)
{
	int ret = RET_OK;
	LX_OVI_LVDS_OUT_STD_T enParams;

	do {
		ret = copy_from_user(&enParams, (LX_OVI_LVDS_OUT_STD_T *)arg, sizeof(LX_OVI_LVDS_OUT_STD_T));
		if(ret) OVI_BREAK_WRONG(ret);
		ret = OVI_HAL_SetLvdsFormat(enParams);
		if(ret) OVI_BREAK_WRONG(ret);
	}while(0);
	return ret;
}

static int OVI_IO_SetLvdsBitDepth(unsigned long arg)
{
	int ret = RET_OK;
	LX_OVI_BIT_DEPTH_T enParams;

	do {
		ret = copy_from_user(&enParams, (LX_OVI_BIT_DEPTH_T *)arg, sizeof(LX_OVI_BIT_DEPTH_T));
		if(ret) OVI_BREAK_WRONG(ret);
		ret = OVI_HAL_SetLvdsBitDepth(enParams);
		if(ret) OVI_BREAK_WRONG(ret);
	}while(0);
	return ret;
}

static int OVI_IO_SetLvdsBlack(unsigned long arg)
{
	int ret = RET_OK;
	BOOLEAN bParams;

	do {
		ret = copy_from_user(&bParams, (BOOLEAN *)arg, sizeof(BOOLEAN));
		if(ret) OVI_BREAK_WRONG(ret);
		ret = OVI_HAL_SetLvdsBlack(bParams);
		if(ret) OVI_BREAK_WRONG(ret);
	}while(0);
	return ret;
}

static int OVI_IO_SetClock(unsigned long arg)
{
	int ret = RET_OK;
	BOOLEAN bParams;

	do {
		ret = copy_from_user(&bParams, (BOOLEAN *)arg, sizeof(BOOLEAN));
		if(ret) OVI_BREAK_WRONG(ret);
		ret = OVI_HAL_SetClock(bParams);
		if(ret) OVI_BREAK_WRONG(ret);
	}while(0);
	return ret;
}

static int OVI_IO_DisplayResume(unsigned long arg)
{
	int ret = RET_OK;
	LX_OVI_DISPLAY_INFO_T stParams;

	do {
		printk("OVI - display resume BEGIN\n");
		
		ret = copy_from_user(&stParams, (LX_OVI_DISPLAY_INFO_T *)arg, sizeof(LX_OVI_DISPLAY_INFO_T));
		if(ret) OVI_BREAK_WRONG(ret);
		ret = OVI_HAL_DisplayResume(&stParams);
		if(ret) OVI_BREAK_WRONG(ret);

		printk("OVI - display resume OK\n");
	}while(0);
	return ret;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef KDRV_GLOBAL_LINK
#if defined(CONFIG_LG_BUILTIN_KDRIVER) && defined(CONFIG_LGSNAP)
user_initcall_grp("kdrv",OVI_Init);
#else
module_init(OVI_Init);
#endif
module_exit(OVI_Cleanup);

MODULE_AUTHOR("LGE");
MODULE_DESCRIPTION("base driver");
MODULE_LICENSE("GPL");
#endif

/** @} */

