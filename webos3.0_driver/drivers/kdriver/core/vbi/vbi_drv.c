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
 *  @addtogroup lg115x_vbi
 *	@{
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/
#undef	SUPPORT_VBI_DEVICE_READ_WRITE_FOPS

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

#include "vbi_module.h"
#include "vbi_hw.h"
#include "vbi_drv.h"
#include "vbi_data.h"

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
 *	main control block for vbi device.
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
VBI_DEVICE_T;

#ifdef KDRV_CONFIG_PM
typedef struct
{
	bool	is_suspended;
}VBI_DRVDATA_T;
#endif

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/
extern	void	VBI_PROC_Init(void);
extern	void	VBI_PROC_Cleanup(void);

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Functions
----------------------------------------------------------------------------------------*/
int		VBI_Init(void);
void	VBI_Cleanup(void);

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/
int		g_vbi_debug_fd;
int 	g_vbi_major = VBI_MAJOR;
int 	g_vbi_minor = VBI_MINOR;

sCGMS	gZeroCGMSData = {0,};

LX_VBI_INIT_MODE_T gVBIInitiateMode = LX_VBI_INIT_NONE;
int	gVBIInterruptControl = 0;
/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/
static int      VBI_Open(struct inode *, struct file *);
static int      VBI_Close(struct inode *, struct file *);
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)
static int 		VBI_Ioctl (struct inode *, struct file *, unsigned int, unsigned long );
#else
static long     VBI_Ioctl (struct file * filp, unsigned int cmd, unsigned long arg);
#endif
static int VBI_MMap(struct file *, struct vm_area_struct *);
#ifdef SUPPORT_VBI_DEVICE_READ_WRITE_FOPS
static ssize_t  VBI_Read(struct file *, char *, size_t, loff_t *);
static ssize_t  VBI_Write(struct file *, const char *, size_t, loff_t *);
#endif

static int		__VBI_SKEEPER_Register_Meta_Data(void);
/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/

/**
 * main control block for vbi device
*/
static VBI_DEVICE_T*		g_vbi_device;

/**
 * file I/O description for vbi device
 *
*/
static struct file_operations g_vbi_fops =
{
	.open 	= VBI_Open,
	.release= VBI_Close,
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)
	.ioctl	= VBI_Ioctl,
#else
	.unlocked_ioctl	= VBI_Ioctl,
#endif
	.mmap	= VBI_MMap,
#ifdef SUPPORT_VBI_DEVICE_READ_WRITE_FOPS
	.read 	= VBI_Read,
	.write 	= VBI_Write,
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
static int VBI_suspend(struct platform_device *pdev, pm_message_t state)
{
	int ret = RET_OK;
#ifdef KDRV_CONFIG_PM_VBI
	VBI_DRVDATA_T *drv_data;
	drv_data = platform_get_drvdata(pdev);

	printk("VBI - suspend BEGIN\n");

	// add here the suspend code
	ret = VBI_Data_Suspend(); // 2014.03.26 : For VBI DDR header data

	drv_data->is_suspended = 1;
	printk("VBI - suspend OK. %d,%d\n", gVBIInitiateMode, gVBIInterruptControl);
	//VBI_PRINT("[%s] VBI done suspend. Mode type[%d], ISR[%d]\n", VBI_MODULE, gVBIInitiateMode, gVBIInterruptControl);
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
static int VBI_resume(struct platform_device *pdev)
{
	int ret = RET_OK;
#ifdef KDRV_CONFIG_PM_VBI
	VBI_DRVDATA_T *drv_data;
	drv_data = platform_get_drvdata(pdev);

	printk("VBI - resume BEGIN\n");

	if(drv_data->is_suspended == 0){
		printk("VBI - resume FAIL\n");
		VBI_PRINT("[%s:%d] Warning! Did not RESUMED because it was not suspended![%d].\n", __F__, __L__, drv_data->is_suspended);
		VBI_PRINT("[%s:%d] VBI Info. Last Initiated Mode type [%d], ISR[%d]\n", __F__, __L__, gVBIInitiateMode, gVBIInterruptControl);
		return -1;
	}

	ret = VBI_Module_Resume(gVBIInitiateMode, gVBIInterruptControl);
	ret |= VBI_Data_Resume();

	drv_data->is_suspended = 0;

	if (ret == RET_OK ) printk("VBI - resume OK\n");
	else				printk("VBI - resume FAIL\n");
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
 int  VBI_probe(struct platform_device *pdev)
{

	VBI_DRVDATA_T *drv_data;

	drv_data = (VBI_DRVDATA_T *)kmalloc(sizeof(VBI_DRVDATA_T) , GFP_KERNEL);


	// add here driver registering code & allocating resource code


	VBI_PRINT("[%s] done probe\n", VBI_MODULE);
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
static int  VBI_remove(struct platform_device *pdev)
{
	VBI_DRVDATA_T *drv_data;

	// add here driver unregistering code & deallocating resource code
	VBI_TOP_UnInitialize();

	drv_data = platform_get_drvdata(pdev);
	kfree(drv_data);

	VBI_PRINT("released\n");

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
static void  VBI_release(struct device *dev)
{
	VBI_PRINT("device released\n");
}

/*
 *	module platform driver structure
 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,39)
static struct platform_driver vbi_driver =
#else
static struct platform_driver vbi_driver __refdata =
#endif
{
	.probe          = VBI_probe,
	.suspend        = VBI_suspend,
	.remove         = VBI_remove,
	.resume         = VBI_resume,
	.driver         =
	{
		.name   = VBI_MODULE,
	},
};

static struct platform_device vbi_device = {
	.name = VBI_MODULE,
	.id = 0,
	.id = -1,
	.dev = {
		.release = VBI_release,
	},
};
#endif

/** Initialize the device environment before the real H/W initialization
 *
 *  @note main usage of this function is to initialize the HAL layer and memory size adjustment
 *  @note it's natural to keep this function blank :)
 */
void VBI_PreInit(void)
{
	VBI_InitCfg( );
    /* TODO: do something */
}

int VBI_Init(void)
{
	int			i;
	int			err;
	dev_t		dev;

	VBI_INFO("%s entered \n" , __func__);

	/* Get the handle of debug output for vbi device.
	 *
	 * Most module should open debug handle before the real initialization of module.
	 * As you know, debug_util offers 4 independent debug outputs for your device driver.
	 * So if you want to use all the debug outputs, you should initialize each debug output
	 * using OS_DEBUG_EnableModuleByIndex() function.
	 */
	g_vbi_debug_fd = DBG_OPEN( VBI_MODULE );
	if( g_vbi_debug_fd < 0) return -1;
	OS_DEBUG_EnableModule ( g_vbi_debug_fd );

	OS_DEBUG_EnableModuleByIndex  ( g_vbi_debug_fd, LX_LOGM_LEVEL_NOTI, DBG_COLOR_NONE );
	OS_DEBUG_EnableModuleByIndex  ( g_vbi_debug_fd, LX_LOGM_LEVEL_ERROR, DBG_COLOR_NONE );
	OS_DEBUG_EnableModuleByIndex  ( g_vbi_debug_fd, LX_LOGM_LEVEL_INFO, DBG_COLOR_NONE );
	OS_DEBUG_EnableModuleByIndex  ( g_vbi_debug_fd, LX_LOGM_LEVEL_WARNING, DBG_COLOR_NONE );
	OS_DEBUG_DisableModuleByIndex ( g_vbi_debug_fd, LX_LOGM_LEVEL_TRACE+0 );	//VBI_INTR_PRINT
	OS_DEBUG_EnableModuleByIndex ( g_vbi_debug_fd, LX_LOGM_LEVEL_TRACE+1, DBG_COLOR_NONE);	//TTX_PRINT
	OS_DEBUG_EnableModuleByIndex ( g_vbi_debug_fd, LX_LOGM_LEVEL_TRACE+2, DBG_COLOR_NONE);	//CC_PRINT
	OS_DEBUG_DisableModuleByIndex ( g_vbi_debug_fd, LX_LOGM_LEVEL_TRACE+3 );	//VBI_INTR_PRINT

	/* allocate main device handler, register current device.
	 *
	 * If devie major is predefined then register device using that number.
	 * otherwise, major number of device is automatically assigned by Linux kernel.
	 *
	 */
#ifdef KDRV_CONFIG_PM
	// added by SC Jung for quick booting
	if(platform_driver_register(&vbi_driver) < 0)
	{
		VBI_ERROR("[%s] platform driver register failed\n",VBI_MODULE);

	}
	else
	{
		if(platform_device_register(&vbi_device))
		{
			platform_driver_unregister(&vbi_driver);
			VBI_ERROR("[%s] platform device register failed\n",VBI_MODULE);
		}
		else
		{
			VBI_PRINT("[%s] platform register done\n", VBI_MODULE);
		}


	}
#endif

	g_vbi_device = (VBI_DEVICE_T*)OS_KMalloc( sizeof(VBI_DEVICE_T)*VBI_MAX_DEVICE );

	if ( NULL == g_vbi_device )
	{
		VBI_ERROR("out of memory. can't allocate %d bytes\n", sizeof(VBI_DEVICE_T)* VBI_MAX_DEVICE );
		return -ENOMEM;
	}

	memset( g_vbi_device, 0x0, sizeof(VBI_DEVICE_T)* VBI_MAX_DEVICE );

	if (g_vbi_major)
	{
		dev = MKDEV( g_vbi_major, g_vbi_minor );
		err = register_chrdev_region(dev, VBI_MAX_DEVICE, VBI_MODULE );
	}
	else
	{
		err = alloc_chrdev_region(&dev, g_vbi_minor, VBI_MAX_DEVICE, VBI_MODULE );
		g_vbi_major = MAJOR(dev);
	}

	if ( err < 0 )
	{
		VBI_ERROR("can't register vbi device\n" );
		return -EIO;
	}

	/* TODO : initialize your module not specific minor device */


	/* END */

	for ( i=0; i<VBI_MAX_DEVICE; i++ )
	{
		/* initialize cdev structure with predefined variable */
		dev = MKDEV( g_vbi_major, g_vbi_minor+i );
		cdev_init( &(g_vbi_device[i].cdev), &g_vbi_fops );
		g_vbi_device[i].devno		= dev;
		g_vbi_device[i].cdev.owner = THIS_MODULE;
		g_vbi_device[i].cdev.ops   = &g_vbi_fops;

		/* TODO: initialize minor device */


		/* END */

		err = cdev_add (&(g_vbi_device[i].cdev), dev, 1 );

		if (err)
		{
			VBI_ERROR("error (%d) while adding vbi device (%d.%d)\n", err, MAJOR(dev), MINOR(dev) );
			return -EIO;
		}
        OS_CreateDeviceClass ( g_vbi_device[i].devno, "%s%d", VBI_MODULE, i );
	}

	/* initialize proc system */
	VBI_PROC_Init ( );
	VIDEO_SKeeperInit();
	__VBI_SKEEPER_Register_Meta_Data();

	VBI_PRINT("vbi device initialized\n");
	//dws
	//VBI_Initialize();

	return 0;
}

void VBI_Cleanup(void)
{
	int i;
	dev_t dev = MKDEV( g_vbi_major, g_vbi_minor );

#ifdef KDRV_CONFIG_PM

	platform_driver_unregister(&vbi_driver);
	platform_device_unregister(&vbi_device);
#endif

	/* cleanup proc system */
	VBI_PROC_Cleanup( );

	/* remove all minor devicies and unregister current device */
	for ( i=0; i<VBI_MAX_DEVICE;i++)
	{
		/* TODO: cleanup each minor device */


		/* END */
		cdev_del( &(g_vbi_device[i].cdev) );
	}

	/* TODO : cleanup your module not specific minor device */

	unregister_chrdev_region(dev, VBI_MAX_DEVICE );

	OS_Free( g_vbi_device );
}


///////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * open handler for vbi device
 *
 */
static int
VBI_Open(struct inode *inode, struct file *filp)
{
    int					major,minor;
    struct cdev*    	cdev;
    VBI_DEVICE_T*	    my_dev;

    cdev	= inode->i_cdev;
    my_dev	= container_of ( cdev, VBI_DEVICE_T, cdev);

	VBI_PRINT("%s entered \n" , __func__);

    /* TODO : add your device specific code */


	/* END */


    my_dev->dev_open_count++;
    filp->private_data = my_dev;

	/* some debug */
    major = imajor(inode);
    minor = iminor(inode);
    VBI_PRINT("device opened (%d:%d)\n", major, minor );

    return 0;
}

/**
 * release handler for vbi device
 *
 */
static int
VBI_Close(struct inode *inode, struct file *file)
{
    int					major,minor;
    VBI_DEVICE_T*   	my_dev;
    struct cdev*		cdev;

	VBI_PRINT("%s entered \n" , __func__);
    cdev	= inode->i_cdev;
    my_dev	= container_of ( cdev, VBI_DEVICE_T, cdev);

    if ( my_dev->dev_open_count > 0 )
    {
        --my_dev->dev_open_count;
    }

    /* TODO : add your device specific code */

	/* END */

	/* some debug */
    major = imajor(inode);
    minor = iminor(inode);
    VBI_PRINT("device closed (%d:%d)\n", major, minor );
    return 0;
}

/**
 * memory mapping to virtual region
 *
 */
static int VBI_MMap(struct file *file, struct vm_area_struct *vma)
{
	unsigned long start;
	unsigned long len;
	unsigned long off = ((vma->vm_pgoff) << PAGE_SHIFT);

	VBI_PRINT("%s entered \n" , __func__);

	start = gstMemCfgVBI->buf_memory_base & PAGE_MASK; // or 0
	len = PAGE_ALIGN((start & ~PAGE_MASK) + gstMemCfgVBI->buf_memory_size);

	VBI_PRINT("MMAP : start - %08lx , len - %08lx , off - %08lx " , start , len , off);

	if ((vma->vm_end - vma->vm_start + off) > len)
	{
	 	VBI_ERROR("ERROR : GFX_SurfaceMmap leng is over\n");
	 	VBI_ERROR("start : %lx end : %lx offset : %lx len : %lx\n" ,
	 		vma->vm_start , vma->vm_end , off , len);

        return -EINVAL;
	}

	off += start;

	VBI_PRINT("MMAP : start - %08lx , len - %08lx , off - %08lx " , start , len , vma->vm_pgoff);

	vma->vm_pgoff = off >> PAGE_SHIFT;
	vma->vm_flags |= VM_IO;
	vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);

	return io_remap_pfn_range(vma, vma->vm_start, off >> PAGE_SHIFT,
	                           vma->vm_end - vma->vm_start,
	                           vma->vm_page_prot);
	return 0;
}

/**
 * ioctl handler for vbi device.
 *
 *
 * note: if you have some critial data, you should protect them using semaphore or spin lock.
 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)
static int
VBI_Ioctl ( struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg )
#else
static long
VBI_Ioctl (struct file * filp, unsigned int cmd, unsigned long arg)
#endif
{
    int err = 0, ret = RET_OK;

    VBI_DEVICE_T*	    my_dev;
    struct cdev*		cdev;
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)
#else
	struct inode *inode = filp->f_path.dentry->d_inode;
#endif
    unsigned int temp;
    unsigned int slicer_mode;
	unsigned int status;

	/* For Status Keeper */
	LX_VP_ST_KEEPER_MODULE_TYPE_T	mtype = LX_VP_ST_KEEPER_MODULE_VBI;
	LX_VP_ST_KEEPER_HISTORY_TYPE_T	htype = LX_VP_ST_KEEPER_HISTORY_IO;
	LX_VP_ST_KEEPER_HISTORY_INDEX_T pData;

    cdev	= inode->i_cdev;
    my_dev	= container_of ( cdev, VBI_DEVICE_T, cdev);

    /*
     * check if IOCTL command is valid or not.
     * - if magic value doesn't match, return error (-ENOTTY)
     * - if command is out of range, return error (-ENOTTY)
     *
     * note) -ENOTTY means "Inappropriate ioctl for device.
     */
    if (_IOC_TYPE(cmd) != VBI_IOC_MAGIC)
    {
    	VBI_WARN("invalid magic. magic=0x%02X\n", _IOC_TYPE(cmd) );
    	return -ENOTTY;
    }
    if (_IOC_NR(cmd) > VBI_IOC_MAXNR)
    {
    	VBI_WARN("out of ioctl command. cmd_idx=%d\n", _IOC_NR(cmd) );
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
    	VBI_WARN("memory access error. cmd_idx=%d, rw=%c%c, memptr=%p\n",
    													_IOC_NR(cmd),
    													(_IOC_DIR(cmd) & _IOC_READ)? 'r':'-',
    													(_IOC_DIR(cmd) & _IOC_WRITE)? 'w':'-',
    													(void*)arg );
        return -EFAULT;
	}

	/* Load Status Keeper data */
	pData.ioctl = (unsigned int)cmd;
	pData.pData = (unsigned int *)((unsigned int)arg);
	pData.size  = _IOC_SIZE(cmd);

	if((cmd != VBI_CC_IOR_GET_CC_PACKET)&&(cmd != VBI_IO_CHECK_VLINE625)\
			&&(cmd != VBI_IO_GET_ANALOG_CP))
		VIDEO_SKeeper_PushHistoryLog(mtype, htype, &pData);

	switch(cmd)
	{
        case VBI_TTX_IO_INIT:
			VBI_PRINT("VBI TTX init\n");
			if(my_dev->dev_initialized == 0)
			{
				if(VBI_TTX_Initialize() < 0)
				{
					ret = RET_ERROR;
				}
				else
				{
					ret = RET_OK;
					my_dev->dev_initialized = 1;
				}

				status = ret;
				(void)VIDEO_SKeeperUpdateIndex(LX_VP_ST_KEEPER_MODULE_VBI, VBI_SKEEPER_TTX_INITIALIZE, status);
			}
			break;

		case VBI_TTX_IOW_ONOFF_INTERRUPT:
			if((BOOLEAN)arg)
				ret = VBI_TTX_SetInterrupt(ON);
			else
				ret = VBI_TTX_SetInterrupt(OFF);

			gVBIInterruptControl = (int)arg;

			status = (UINT32)arg;
			(void)VIDEO_SKeeperUpdateIndex(LX_VP_ST_KEEPER_MODULE_VBI, VBI_SKEEPER_TTX_ONOFF_INTERRUPT, status);
			break;

		case VBI_TTX_IOW_ONOFF_SLICER:
			if((BOOLEAN)arg)
				VBI_PRINT("VBI TTX Slicer ON\n");
			else
				VBI_PRINT("VBI TTX Interrupt OFF\n");
			ret = RET_OK;

			status = (UINT32)arg;
			(void)VIDEO_SKeeperUpdateIndex(LX_VP_ST_KEEPER_MODULE_VBI, VBI_SKEEPER_TTX_ONOFF_INTERRUPT, status);
			break;

		case VBI_TTX_IO_RESET_TTX_BUFFER:
			ret = VBI_TTX_ClearTTXMemory();
			break;

		case VBI_TTX_IO_RESET_VPS_BUFFER:
			ret = VBI_TTX_ClearVPSMemory();
			break;

		case VBI_TTX_IO_RESET_WSS_BUFFER:
			ret = VBI_TTX_ClearWSSMemory();
			break;

		case VBI_TTX_IOR_GET_PARAMETER_OFFSET:
			//VBI_PRINT("VBI TTX Get Parameter offset(Warning : empty ioctl call)\n");
			ret = RET_OK;
			break;

		case VBI_TTX_IOR_GET_MMAP_SIZE:
			VBI_PRINT("VBI TTX Get Memory Map Size\n");
			temp = (unsigned int)VBI_TTX_GetMemorySize();

			status = (UINT32)temp;
			(void)VIDEO_SKeeperUpdateIndex(LX_VP_ST_KEEPER_MODULE_VBI, VBI_SKEEPER_TTX_GET_MMAP_SIZE, status);

			ret = copy_to_user((void *)arg, (void *)&temp, sizeof(unsigned int));
			break;

		case VBI_TTX_IO_RESET_TTX:
			//VBI_PRINT("VBI TTX Reset Parameters(Warning : empty ioctl call\n");
			ret = RET_OK;
			break;

		case VBI_TTX_IO_RELEASE:
			//VBI_PRINT("VBI TTX Release(Warning : empty ioctl call\n");
			ret = RET_OK;
			break;

		case VBI_TTX_IOW_ENABLE_VPS_SLICER:
			{
				BOOLEAN bEnable;

				if( copy_from_user((void *)&bEnable, (void *)arg, sizeof(BOOLEAN)) )
					return -EFAULT;

				if(bEnable) VBI_PRINT("[%s:%d] ENABLE VPS SLICER\n", __F__, __L__);
				else VBI_PRINT("[%s:%d] DISABLE VPS SLICER\n", __F__, __L__);

				status = (UINT32)bEnable;
				(void)VIDEO_SKeeperUpdateIndex(LX_VP_ST_KEEPER_MODULE_VBI, VBI_SKEEPER_TTX_ENABLE_VPS_SLICER, status);

				ret = VBI_TTX_Enable_VPS_Slicer(bEnable);
			}
			break;

		case VBI_CC_IO_RESET:
		{
			//VBI_PRINT("Reset ok(Warning! Empty ioctl call\n");
			ret = RET_OK;
		}
		break;

		case VBI_CC_IO_INIT:
			VBI_PRINT("VBI CC init\n");
			if(my_dev->dev_initialized == 0)
			{
				if(VBI_CC_Initialize() <0){
					ret = RET_ERROR;
				}
				else{
					my_dev->dev_initialized = 1;
					ret = RET_OK;
				}


				status = (UINT32)ret;
				(void)VIDEO_SKeeperUpdateIndex(LX_VP_ST_KEEPER_MODULE_VBI, VBI_SKEEPER_CC_INITIALIZE, status);

			}
			break;

		case VBI_CC_IOW_ONOFF_INTERRUPT:
			if((BOOLEAN)arg)
				VBI_PRINT("VBI CC interrupt ON\n");
			else
				VBI_PRINT("VBI CC Interrupt OFF\n");
			ret = VBI_CC_InterruptControl(arg);

			gVBIInterruptControl = (int)arg;

			status = (UINT32)arg;
			(void)VIDEO_SKeeperUpdateIndex(LX_VP_ST_KEEPER_MODULE_VBI, VBI_SKEEPER_CC_ONOFF_INTERRUPT, status);
			break;

		case VBI_CC_IOW_ONOFF_SLICER:
            //if((BOOLEAN)arg)
			//	VBI_PRINT("VBI CC Slicer ON(Warning! Empty ioctl call)\n");
			//else
			//	VBI_PRINT("VBI CC Slicer OFF(Warning! Empty ioctl call)\n");
			ret = RET_OK;
			break;

		case VBI_CC_IOR_GET_PACKET_COUNT:
			temp = VBI_CC_GetVBIBufferInform();
			ret = copy_to_user((void *)arg , (void *)&temp , sizeof(int));
			break;

		case VBI_CC_IO_RESET_CC_BUFFER:
			VBI_PRINT("VBI CC Reset CC Buffer \n");
			ret = VBI_CC_SetVBIBufferClear();
			break;

		case VBI_CC_IOR_GET_CC_PACKET:
			ret = VBI_CC_GetVBIData((void *)&temp , 1);
			ret |= copy_to_user((void *)arg , (void *)&temp , sizeof(int));
			break;

		case VBI_TTX_IOW_VBI_SET_OPTION_LPF:
			ret = VBI_TTX_SetVBILPF(arg);
			break;

		case VBI_TTX_IOW_SET_TYPE_LINE318:
			ret = VBI_TTX_SetTypeLine318(arg);
			break;

		case VBI_TTX_IOW_CHECK_LINE318:
			ret = VBI_TTX_CheckOnLine318(arg);
			break;

        case VBI_TTX_IOW_SET_TT_SLICER_MODE:

			if( copy_from_user((void *)&slicer_mode, (void *)arg, sizeof(unsigned int)) )
				return -EFAULT;
			ret = VBI_TTX_Set_TT_Slicer_Mode(slicer_mode);
			break;

		case VBI_CC_IOW_SET_DEBUGMODE_PRINT:
			if(arg==2) ret = VBI_CC_SetPrintResultOfLineDetect(1);
			else if(arg == 1) ret = VBI_CC_DebugPrintMode(1);
			else
			{
				ret = VBI_CC_SetPrintResultOfLineDetect(0);
				ret |= VBI_CC_DebugPrintMode(0);
			}
			break;

		case VBI_IO_SET_INIT_CGMS_DATA:
			VBI_PutCGMSData(&gZeroCGMSData);
			break;

		case VBI_TTX_IOW_SET_DEBUGMODE_PRINT:
			if(arg == 1) VBI_TTX_DebugPrintMode(1);
			else VBI_TTX_DebugPrintMode(0);
			break;

		case VBI_IO_CHECK_VLINE625:
			VBI_CheckVline625();
			break;

		case VBI_IO_GET_ANALOG_CP:
			ret = VBI_CheckAnalogCP();
			break;

		case VBI_IOW_SET_LOGM_PRINT:
			ret = VBI_SetLogmPrint(arg);
			break;
	    default:
	    {
			/* redundant check but it seems more readable */
    	    ret = -ENOTTY;
		}
    }
    return ret;
}

static int		__VBI_SKEEPER_Register_Meta_Data(void)
{
	int ret = RET_ERROR;
	int i;
	LX_VP_ST_KEEPER_INDEX_T *pMetaData;

	do{
		/* Step 1. Allocate temperary memory */
		pMetaData = (LX_VP_ST_KEEPER_INDEX_T *)kmalloc(sizeof(LX_VP_ST_KEEPER_INDEX_T) * VBI_SKEEPER_STATUS_MAXNUM, GFP_KERNEL);

		/* Step 2. Check result of allocation*/
		if(pMetaData == NULL){
			VBI_PRINT("[%s:%d] SKeeper memory allocation failed for setting meta data\n", __F__, __L__);
			break;
		}

		/* Step 3. Set in meta data */
		for(i=0; i<VBI_SKEEPER_STATUS_MAXNUM; i++){
			pMetaData[i].status = 0;
			pMetaData[i].last_modified_time = 0;
			pMetaData[i].modifier = 0;
		}

		SKEEPER_INSERT_DATA(pMetaData, VBI_SKEEPER_TTX_INITIALIZE);
		SKEEPER_INSERT_DATA(pMetaData, VBI_SKEEPER_TTX_ONOFF_INTERRUPT);
		SKEEPER_INSERT_DATA(pMetaData, VBI_SKEEPER_TTX_GET_MMAP_SIZE);
		SKEEPER_INSERT_DATA(pMetaData, VBI_SKEEPER_TTX_ENABLE_VPS_SLICER);
		SKEEPER_INSERT_DATA(pMetaData, VBI_SKEEPER_CC_INITIALIZE);
		SKEEPER_INSERT_DATA(pMetaData, VBI_SKEEPER_CC_ONOFF_INTERRUPT);
		SKEEPER_INSERT_DATA(pMetaData, VBI_SKEEPER_VBI_DATA_ERROR_STATUS);
		SKEEPER_INSERT_DATA(pMetaData, VBI_SKEEPER_VBI_MODULE_ERROR_STATUS);

		/* Step 4. List up Index to Status Keeper */
		ret = VIDEO_SKeeperListUpIndex(LX_VP_ST_KEEPER_MODULE_VBI, pMetaData, VBI_SKEEPER_STATUS_MAXNUM);
		if(ret < 0){
			VBI_ERROR("[%s:%d] SKeeper listup Index failed\n", __F__, __L__);
		}

		kfree((void *)pMetaData);
	}while(0);

	return ret;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef KDRV_GLOBAL_LINK
#if defined(CONFIG_LG_BUILTIN_KDRIVER) && defined(CONFIG_LGSNAP)
user_initcall_grp("kdrv",VBI_Init);
#else
module_init(VBI_Init);
#endif
module_exit(VBI_Cleanup);

MODULE_AUTHOR("LGE");
MODULE_DESCRIPTION("VBI driver");
MODULE_LICENSE("GPL");
#endif

/** @} */

