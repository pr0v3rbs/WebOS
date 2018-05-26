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
 *  main driver implementation for png device.
 *	png device will teach you how to make device driver with new platform.
 *
 *  author		bongrae.cho
 *  version		1.0
 *  date		2013.07.08
 *  note		Additional information.
 *
 *  @addtogroup lg_png
 *	@{
 */

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/platform_device.h>
#include <asm/uaccess.h>
#include <linux/slab.h>
#include <linux/mm.h>
#include <linux/interrupt.h>
#include <asm/irq.h>                    /**< For isr */
#include <asm/io.h>                    /**< For isr */

#include <linux/wait.h>
#include <linux/jiffies.h>


#include "os_util.h"
#include "base_device.h"

#include "png_drv.h"

#include "png_reg.h"
#include "png_cfg.h"

#include "png_kapi.h"
/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/
int		g_png_debug_fd;
int 	g_png_major = PNG_MAJOR;
int 	g_png_minor = PNG_MINOR;

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
#define PNG_MAX_DEVICE 1
#define PNG_DECODE_TIMEOUT 500

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define PNG_ERROR(format, args...) DBG_PRINT( g_png_debug_fd, LX_LOGM_LEVEL_ERROR, format ,##args)
#define PNG_WARN(format, args...) DBG_PRINT( g_png_debug_fd, LX_LOGM_LEVEL_WARNING, format ,##args)
#define PNG_NOTI(format, args...) DBG_PRINT( g_png_debug_fd, LX_LOGM_LEVEL_NOTI, format ,##args)
#define PNG_INFO(format, args...) DBG_PRINT( g_png_debug_fd, LX_LOGM_LEVEL_INFO, format ,##args)
#define PNG_DEBUG(format, args...) DBG_PRINT( g_png_debug_fd, LX_LOGM_LEVEL_DEBUG, format, ##args)
#define PNG_TRACE(format, args...) DBG_PRINT( g_png_debug_fd, LX_LOGM_LEVEL_TRACE, format, ##args)
#define PNG_PRINT(format, args...) DBG_PRINT( g_png_debug_fd, LX_LOGM_LEVEL_TRACE + 1, format, ##args) //user define


/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/
typedef enum
{
    PNG_STATUS_IDLE,
    PNG_STATUS_DECODE,
    PNG_STATUS_DECODE_DONE,
} PNG_STATUS;

typedef struct
{
	char*					module_name;
	UINT32					chip;
	UINT32					platform;
	int						devno;
	struct cdev				cdev;
	UINT32			 		max_devices;
	unsigned int 		    irq_num;
    int                     dev_open_count;

	LX_PNG_MEM_CFG_T	    pPhyBuf;
	LX_PNG_CFG_T            cfg;
    LX_PNG_DECODE_MODE      decode_mode;

    PNG_STATUS                  status;

    wait_queue_head_t       png_wq;

    unsigned int            workingCount;
    unsigned int            output_addr;
    unsigned int            output_length;

    unsigned int            totalWorkCount;
    LX_PNG_DECODE_PARAM_T   workList[10];
/*
    UINT32              src_buf_addr;
    UINT32              *src_buf;
    UINT32              src_buf_size;

    UINT32              output_buf_addr;
    UINT32              *output_buf;
    UINT32              output_buf_size;
*/
	UINT32			*IO_PNG_reg;
	UINT32			*IO_PNG_shadow_reg;
	UINT32			IO_PNG_reg_addr;
	UINT32			IO_PNG_reg_size;
	UINT32			*IO_ICOD_reg;
	UINT32			*IO_ICOD_shadow_reg;
	UINT32			IO_ICOD_reg_addr;
	UINT32			IO_ICOD_reg_size;


} PNG_DEVICE_T;

#ifdef KDRV_CONFIG_PM	// added by SC Jung for quick booting
typedef struct
{
	// add here extra parameter
	bool	is_suspended;
} PNG_DRVDATA_T;
#endif

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
	global Functions
----------------------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------------------
    Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/
static int PNG_Open(struct inode *inode, struct file *filp);
static int PNG_Close(struct inode *inode, struct file *file);
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)
static int PNG_Ioctl ( struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg );
#else
static long PNG_Unlocked_Ioctl(struct file *filp, unsigned int cmd, unsigned long arg);
#endif
static int PNG_Mmap(struct file *file, struct vm_area_struct *vma);
#ifdef SUPPORT_PNG_DEVICE_READ_WRITE_FOPS
PNG_Read
PNG_Write
#endif

irqreturn_t PNG_InterruptHandler(int i_iIrq, void *pDevId, struct pt_regs *pstRegs);

void PNG_PrintStatus(void);

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
/**
 * main control block for PNG device
*/
static PNG_DEVICE_T*		g_PNG_device;

/**
 * file I/O description for png device
 *
*/
static struct file_operations g_PNG_fops =
{
	.open 	= PNG_Open,
	.release= PNG_Close,
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)
	.ioctl	= PNG_Ioctl,
#else
	.unlocked_ioctl = PNG_Unlocked_Ioctl,
#endif
	.mmap	= PNG_Mmap,
#ifdef SUPPORT_PNG_DEVICE_READ_WRITE_FOPS
	.read 	= PNG_Read,
	.write 	= PNG_Write,
#else
	.read	= NULL,
	.write	= NULL,
#endif
};


static DECLARE_WAIT_QUEUE_HEAD(_wq_png);
static volatile int _pSemDecode = 0;


/*========================================================================================
    Implementation Group : PNG Module
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
static int
PNG_suspend(struct platform_device *pdev, pm_message_t state)
{
#ifdef KDRV_CONFIG_PM_PNG
	PNG_DRVDATA_T *drv_data;
	drv_data = platform_get_drvdata(pdev);

	printk("PNG - suspend BEGIN\n");

	// add here the suspend code

	if ( drv_data->is_suspended == 1 )
	{
		printk("PNG - suspend FAIL\n");
		return -1;	//If already in suspend state, so ignore
	}

	drv_data->is_suspended = 1;
	printk("PNG - suspend OK\n");
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
static int
PNG_resume(struct platform_device *pdev)
{
#ifdef KDRV_CONFIG_PM_PNG
	PNG_DRVDATA_T *drv_data;
	drv_data = platform_get_drvdata(pdev);

	printk("PNG - resume BEGIN\n");

	if(drv_data->is_suspended == 0)
	{
		printk("PNG - resume BEGIN\n");
		return -1;
	}

	// add here the resume code

	drv_data->is_suspended = 0;
	printk("PNG - resume OK\n");
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
 int
PNG_probe(struct platform_device *pdev)
{
	PNG_DRVDATA_T *drv_data;

	PNG_TRACE("PNG_probe\n");

	drv_data = (PNG_DRVDATA_T *)kmalloc(sizeof(PNG_DRVDATA_T) , GFP_KERNEL);

	// add here driver registering code & allocating resource code

	PNG_TRACE("[%s] done probe\n", PNG_MODULE);
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
static int
PNG_remove(struct platform_device *pdev)
{
	PNG_DRVDATA_T *drv_data;

	PNG_TRACE("PNG_remove\n");

	// add here driver unregistering code & deallocating resource code

	drv_data = platform_get_drvdata(pdev);
	kfree(drv_data);

	PNG_TRACE("released\n");

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
static void
PNG_release(struct device *dev)
{
	PNG_TRACE("PNG_released\n");
}

/*
 *	module platform driver structure
 */
static struct platform_driver png_driver =
{
	.probe			= PNG_probe,
	.suspend		= PNG_suspend,
	.remove 		= PNG_remove,
	.resume 		= PNG_resume,
	.driver 		=
	{
		.name	= PNG_MODULE,
	},
};

static struct platform_device png_device = {
	.name	= PNG_MODULE,
	.id 	= -1,
	.dev	= {
		.release = PNG_release,
	},
};
#endif




/** Initialize the device environment before the real H/W initialization
 *
 *  @note main usage of this function is to initialize the HAL layer and memory size adjustment
 *  @note it's natural to keep this function blank :)
 */
void PNG_PreInit(void)
{
	PNG_TRACE("PNG_PreInit\n");
    /* TODO: do something */
}

int PNG_Init(void)
{
	int			i;
	int			err;
	dev_t		dev;
    PNG_CFG_TBL_T pPNGCfg;

	PNG_TRACE("PNG_Init\n");

	/* Get the handle of debug output for png device.
	 *
	 * Most module should open debug handle before the real initialization of module.
	 * As you know, debug_util offers 4 independent debug outputs for your device driver.
	 * So if you want to use all the debug outputs, you should initialize each debug output
	 * using OS_DEBUG_EnableModuleByIndex() function.
	 */
	g_png_debug_fd = DBG_OPEN("png");

	if ( g_png_debug_fd < 0 )
	{
		PNG_ERROR("[PNG] can't register se device\n" );
		return -1;
	}

    OS_DEBUG_EnableModule ( g_png_debug_fd );
    OS_DEBUG_EnableModuleByIndex ( g_png_debug_fd, LX_LOGM_LEVEL_ERROR, DBG_COLOR_RED ); // error
    //OS_DEBUG_EnableModuleByIndex ( g_png_debug_fd, LX_LOGM_LEVEL_WARNING, DBG_COLOR_YELLOW ); // warn
    //OS_DEBUG_EnableModuleByIndex ( g_png_debug_fd, LX_LOGM_LEVEL_NOTI, DBG_COLOR_NONE ); // noti
    //OS_DEBUG_EnableModuleByIndex ( g_png_debug_fd, LX_LOGM_LEVEL_INFO, DBG_COLOR_NONE ); // info
    //OS_DEBUG_EnableModuleByIndex ( g_png_debug_fd, LX_LOGM_LEVEL_DEBUG, DBG_COLOR_NONE ); // debug
    //OS_DEBUG_EnableModuleByIndex ( g_png_debug_fd, LX_LOGM_LEVEL_TRACE, DBG_COLOR_GREEN ); // trace
    //OS_DEBUG_EnableModuleByIndex ( g_png_debug_fd, LX_LOGM_LEVEL_TRACE + 1, DBG_COLOR_NONE ); // user define

	if(PNG_Config(&pPNGCfg))
	{
		PNG_ERROR("[PNG] can't get config information\n" );
		return -1;
	}

    /* allocate main device handler, register current device.
     *
     * If devie major is predefined then register device using that number.
     * otherwise, major number of device is automatically assigned by Linux kernel.
     *
     */
#ifdef KDRV_CONFIG_PM
    // added by SC Jung for quick booting
    if(platform_driver_register(&png_driver) < 0)
    {
        PNG_ERROR("[%s] platform driver register failed\n", PNG_MODULE);
    }
    else
    {
        if(platform_device_register(&png_device))
        {
            platform_driver_unregister(&png_driver);
            PNG_ERROR("[%s] platform device register failed\n", PNG_MODULE);
        }
        else
        {
            PNG_DEBUG("[%s] platform register done\n", PNG_MODULE);
        }
    }
#endif

    g_PNG_device = (PNG_DEVICE_T*)OS_KMalloc( sizeof(PNG_DEVICE_T) * PNG_MAX_DEVICE );

    if ( NULL == g_PNG_device )
    {
        PNG_WARN("out of memory. can't allocate %d bytes\n", sizeof(PNG_DEVICE_T)* PNG_MAX_DEVICE );
        return -ENOMEM;
    }

    memset(g_PNG_device, 0x0, sizeof(PNG_DEVICE_T)* PNG_MAX_DEVICE );

    if (g_png_major)
    {
        dev = MKDEV( g_png_major, g_png_minor );
        err = register_chrdev_region(dev, PNG_MAX_DEVICE, PNG_MODULE );
    }
    else
    {
        err = alloc_chrdev_region(&dev, g_png_minor, PNG_MAX_DEVICE, PNG_MODULE );
        g_png_minor = MAJOR(dev);
    }

    if ( err < 0 )
    {
        PNG_WARN("can't register PNG device\n" );
        return -EIO;
    }

    /* TODO : initialize your module not specific minor device */
    g_PNG_device->chip = pPNGCfg.chip;
    g_PNG_device->IO_PNG_reg_addr = pPNGCfg.png_reg_base;
    g_PNG_device->IO_ICOD_reg_addr = pPNGCfg.icod_reg_base;
    g_PNG_device->irq_num = pPNGCfg.irq_num;
    g_PNG_device->max_devices = pPNGCfg.num_device;
    g_PNG_device->pPhyBuf = pPNGCfg.mem_cfg;

    PNG_Reg_Init(g_PNG_device->IO_PNG_reg_addr, g_PNG_device->IO_ICOD_reg_addr);

    err = request_irq(g_PNG_device->irq_num, (irq_handler_t)PNG_InterruptHandler, 0, "PNG", g_PNG_device);

    if (err)
    {
        PNG_WARN("request_irq IRQ_ICOD0 in %s is failed %d\n", __FUNCTION__, err);
        unregister_chrdev_region(dev, PNG_MAX_DEVICE );
        return -EIO;
    }

    PNG_PRINT("PNG IRQ registered :%d:\n", g_PNG_device->irq_num);

    /* END */

    for (i = 0; i < PNG_MAX_DEVICE; i++ )
    {
        /* initialize cdev structure with predefined variable */
        dev = MKDEV( g_png_major, g_png_minor + i );
        cdev_init( &(g_PNG_device[i].cdev), &g_PNG_fops );
        g_PNG_device[i].devno     = dev;
        g_PNG_device[i].cdev.owner = THIS_MODULE;
        g_PNG_device[i].cdev.ops   = &g_PNG_fops;

        /* TODO: initialize minor device */


        /* END */

        err = cdev_add (&(g_PNG_device[i].cdev), dev, 1 );

        if (err)
        {
            PNG_ERROR("error (%d) while adding PNG device (%d.%d)\n", err, MAJOR(dev), MINOR(dev) );
            return -EIO;
        }

        OS_CreateDeviceClass ( g_PNG_device[i].devno, "%s%d", PNG_MODULE, i );
    }

    /* initialize proc system */
    //PNG_PROC_Init ( );

    init_waitqueue_head(&g_PNG_device->png_wq);

	PNG_TRACE("png device initialized\n");

	return 0;
}

void PNG_Cleanup(void)
{
	int i;
	dev_t dev = MKDEV(g_png_major, g_png_minor);

	PNG_TRACE("PNG_Cleanup\n");

	/* cleanup proc system */
	//PNG_PROC_Cleanup( );

	/* remove all minor devicies and unregister current device */
	for (i = 0; i < PNG_MAX_DEVICE; i++)
	{
		/* TODO: cleanup each minor device */


		/* END */
		cdev_del( &(g_PNG_device[i].cdev) );
	}

	/* TODO : cleanup your module not specific minor device */

	free_irq( g_PNG_device->irq_num, NULL);

    PNG_Reg_Cleanup();

	OS_Free(g_PNG_device);

	unregister_chrdev_region(dev, PNG_MAX_DEVICE);

	PNG_TRACE("PNG device cleanup\n");
}


/**
 * open handler for PNG device
 *
 */
static int PNG_Open(struct inode *inode, struct file *filp)
{
    int					major,minor;
    struct cdev*    	cdev;
    PNG_DEVICE_T*	my_dev;

	PNG_TRACE("PNG_Open\n");

    cdev	= inode->i_cdev;
    my_dev	= container_of ( cdev, PNG_DEVICE_T, cdev);

    /* TODO : add your device specific code */
	if ( my_dev->dev_open_count == 0 )
	{
		if(lx_chip_rev() < LX_CHIP_REV(H13,A0))
            return -EIO;
	}

    my_dev->dev_open_count++;
    filp->private_data = my_dev;

	/* some debug */
    major = imajor(inode);
    minor = iminor(inode);
    PNG_NOTI("device opened (%d:%d)\n", major, minor);

    return 0;
}

/**
 * release handler for PNG device
 *
 */
static int PNG_Close(struct inode *inode, struct file *file)
{
    int				major,minor;
    PNG_DEVICE_T*	my_dev;
    struct cdev*		cdev;

	PNG_TRACE("PNG_Close\n");

    cdev	= inode->i_cdev;
    my_dev	= container_of ( cdev, PNG_DEVICE_T, cdev);

    if ( my_dev->dev_open_count > 0 )
    {
        --my_dev->dev_open_count;
    }

    /* TODO : add your device specific code */
	if ( my_dev->dev_open_count <= 0 )
	{
		if ( lx_chip_rev() < LX_CHIP_REV(H13,A0) )
            return -EIO;
	}

	/* END */

	/* some debug */
    major = imajor(inode);
    minor = iminor(inode);
    PNG_NOTI("device closed (%d:%d)\n", major, minor );
    return 0;
}

/**
 * ioctl handler for PNG device.
 *
 *
 * note: if you have some critial data, you should protect them using semaphore or spin lock.
 */

static long PNG_Unlocked_Ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	struct inode *inode = filp->f_path.dentry->d_inode;

    int err = 0, ret = 0;

    PNG_DEVICE_T*	my_dev;
    struct cdev*		cdev;

	PNG_TRACE("PNG_Unlocked_Ioctl\n");

	/*
	 * get current PNG device object
	 */
    cdev	= inode->i_cdev;
    my_dev	= container_of ( cdev, PNG_DEVICE_T, cdev);

    /*
     * check if IOCTL command is valid or not.
     * - if magic value doesn't match, return error (-ENOTTY)
     * - if command is out of range, return error (-ENOTTY)
     *
     * note) -ENOTTY means "Inappropriate ioctl for device.
     */
    if (_IOC_TYPE(cmd) != PNG_IOC_MAGIC)
    {
    	PNG_WARN("invalid magic. magic=0x%02X\n", _IOC_TYPE(cmd) );
    	return -ENOTTY;
    }
    if (_IOC_NR(cmd) > PNG_IOC_MAXNR)
    {
    	PNG_WARN("out of ioctl command. cmd_idx=%d\n", _IOC_NR(cmd) );
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
    	PNG_WARN("memory access error. cmd_idx=%d, rw=%c%c, memptr=%p\n",
    													_IOC_NR(cmd),
    													(_IOC_DIR(cmd) & _IOC_READ)? 'r':'-',
    													(_IOC_DIR(cmd) & _IOC_WRITE)? 'w':'-',
    													(void*)arg );
        return -EFAULT;
	}

	//PNG_TRACE("cmd = %08X (cmd_idx=%d)\n", cmd, _IOC_NR(cmd) );
	//ICOD_ClockOn();

	switch(cmd)
	{
		case PNG_IO_RESET:
		{
            PNG_TRACE("PNG_IO_RESET\n");
			//PNG_Core_Reset();
			//memset( &my_dev->stHeader, 0, sizeof(LX_PNG_HEADER_T) );

			break;
		}

		case PNG_IOR_GET_MEM_INFO:
		{
			LX_PNG_MEM_STAT_T mem_stat;
            PNG_TRACE("PNG_IOR_GET_MEM_INFO\n");

			mem_stat.mem_addr = g_PNG_device->pPhyBuf.memory_base;
			mem_stat.mem_size =  g_PNG_device->pPhyBuf.memory_size;

            PNG_INFO("[mem stat] addr : 0x%08X, size : %d\n", mem_stat.mem_addr, mem_stat.mem_size);


			ret = copy_to_user((void __user*)arg, (void *)&mem_stat, sizeof(LX_PNG_MEM_STAT_T));
			break;
		}

		case PNG_IOW_SET_MEM_INFO:
		{
			LX_PNG_MEM_STAT_T mem_stat;
            PNG_TRACE("PNG_IOW_SET_MEM_INFO\n");

			if ( copy_from_user((void *)&mem_stat, (void __user *)arg, sizeof(LX_PNG_MEM_STAT_T)))
			{
				ret = RET_ERROR;
				break;
			}

            PNG_INFO("[new mem stat] addr : 0x%08X, size : %d\n", mem_stat.mem_addr, mem_stat.mem_size);

            g_PNG_device->pPhyBuf.memory_base = mem_stat.mem_addr;
            g_PNG_device->pPhyBuf.memory_size = mem_stat.mem_size;
			break;
		}

		case PNG_IOR_GET_CONFIG:
		{
			LX_PNG_CFG_T png_cfg;

            //png_cfg.width = ;
            //png_cfg.height = ;
            //png_cfg.decode_mode = ;
            //png_cfg.color_type = ;
            //png_cfg.bit_depth = ;
            //png_cfg.output_format = ;

			ret = copy_to_user((void __user*)arg, (void *)&png_cfg, sizeof(LX_PNG_CFG_T));
			break;
		}

		case PNG_IOW_SET_CONFIG:
		{
			LX_PNG_CFG_T png_cfg;
            unsigned int num_component;
            PNG_TRACE("PNG_IOW_SET_CONFIG\n");

			if ( copy_from_user((void *)&png_cfg, (void __user *)arg, sizeof(LX_PNG_CFG_T)) )
			{
				ret = RET_ERROR;
				break;
			}

            PNG_INFO("[cfg param] width : %d, height : %d, bit depth : %d, color type : %d, format : %d\n", png_cfg.width, png_cfg.height, png_cfg.bit_depth, png_cfg.color_type, png_cfg.output_format);
            PNG_INFO("decode mode : %d\n", png_cfg.decode_mode);


            PNG_Reg_SetDecodeImageSize(png_cfg.width, png_cfg.height);
            PNG_Reg_SetDecodeImageInformation(png_cfg.bit_depth, png_cfg.color_type, png_cfg.output_format);

            switch(png_cfg.color_type)
            {
            case 0:
                num_component = 1;
                break;
            case 2:
                num_component = 3;
                break;
            case 3:
                num_component = 1;
                break;
            case 4:
                num_component = 2;
                break;
            case 6:
                num_component = 4;
                break;
             default:
                    num_component = 0;
                break;
            }

            g_PNG_device->output_length = png_cfg.width * png_cfg.height * png_cfg.bit_depth/8 * num_component;

            // set decoding mode
            g_PNG_device->decode_mode = png_cfg.decode_mode;

            PNG_Reg_ClearInterruptAll();
            PNG_Reg_EnableInterruptAll();

			break;
		}
		case PNG_IOWR_DECODE:
		{
            LX_PNG_DECODE_PARAM_T param;
            PNG_TRACE("PNG_IOWR_DECODE\n");

			if(copy_from_user((void *)&param, (void __user *)arg, sizeof(LX_PNG_DECODE_PARAM_T)))
			{
				ret = RET_ERROR;
				break;
			}
            g_PNG_device->output_addr = param.output_addr;

            PNG_INFO("[decode param] intput addr : 0x%08X, length : %d, output addr : 0x%08X\n", param.input_addr, param.input_length, param.output_addr);

            PNG_Reg_SetReadStartAddress(param.input_addr);
            PNG_Reg_SetReadLength(param.input_length);

            PNG_Reg_SetWriteStartAddress(param.output_addr);

            PNG_PrintStatus();
            g_PNG_device->status = PNG_STATUS_DECODE;

            //if(g_PNG_device->decode_mode == LX_PNG_DECODE_MODE_FULL)
            PNG_Reg_StartDecode();

            if(g_PNG_device->decode_mode == LX_PNG_DECODE_MODE_PARTIAL)
            {
                PNG_Reg_SetWriteAddressBoundary(g_PNG_device->pPhyBuf.memory_base + g_PNG_device->pPhyBuf.memory_size);
                PNG_Reg_StartDecodePartial();
            }

		    _pSemDecode = 0;

			break;
		}
		case PNG_IOR_GET_DECODE_RESULT:
		{
            LX_PNG_DECODE_PARAM_T param;
            PNG_TRACE("PNG_IOR_GET_DECODE_RESULT\n");

            PNG_PrintStatus();

		    ret = wait_event_interruptible_timeout(g_PNG_device->png_wq, _pSemDecode != 0, msecs_to_jiffies(PNG_DECODE_TIMEOUT));
            if(ret == 0)
            {
                ICOD_Reg_Reset();
                ret = -EFAULT;
            }
            else
            {
                param.output_addr = g_PNG_device->output_addr;
                param.output_length = g_PNG_device->output_length;

    			ret = copy_to_user((void __user*)arg, (void *)&param, sizeof(LX_PNG_DECODE_PARAM_T));

                PNG_INFO("[send decode param] output addr : 0x%08X, length : %d\n", param.output_addr, param.output_length);
            }


			break;
		}
	    default:
	    {
			/* redundant check but it seems more readable */
    	    ret = -ENOTTY;
		}
    }

    return ret;
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)
static int PNG_Ioctl ( struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg )
{
	PNG_TRACE("PNG_Ioctl\n");
	return (int)PNG_UnlockedIoctl( filp, cmd, arg );
}
#endif

/**
 * memory mapping to virtual region
 *
 */
static int PNG_Mmap(struct file *file, struct vm_area_struct *vma)
{
    int ret;
    UINT32 start;
    UINT32 len;
    UINT32 off = (vma->vm_pgoff) << PAGE_SHIFT;
    //desc = file->private_data;

	PNG_TRACE("PNG_Mmap\n");

	if (g_PNG_device == NULL)
	{
		PNG_WARN("ASSERT\n");
		return -EIO;
	}

	start = ((UINT32)g_PNG_device->pPhyBuf.memory_base) & PAGE_MASK;
	len = PAGE_ALIGN((start & ~PAGE_MASK) + g_PNG_device->pPhyBuf.memory_size);

	PNG_INFO("MMAP : start - %08x , len - %08x , off - %08x\n" , start , len , off);

    if ((vma->vm_end - vma->vm_start + off) > len)
    {
        PNG_ERROR("ERROR : PNG_mmap leng is over\n");
        PNG_ERROR("start : %x end : %x offset : %x len : %x\n" , (UINT32)vma->vm_start , (UINT32)vma->vm_end , off , len);
        return -ENOMEM;
    }

    off += start;
    PNG_DEBUG("%s:%d -- mmap : start - %08x , len - %08x , off - %08x\n", __F__, __L__, start , len , (UINT32)vma->vm_pgoff);

    vma->vm_pgoff = off >> PAGE_SHIFT;
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,10,0))
	vma->vm_flags |= VM_RESERVED;
#else
	vma->vm_flags |= VM_IO|VM_DONTEXPAND|VM_DONTDUMP;
#endif
    vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);

    ret = io_remap_pfn_range(vma, vma->vm_start, vma->vm_pgoff, vma->vm_end - vma->vm_start, vma->vm_page_prot);

    return ret;
}


/**
 * interrupt handler for PNG device.
 *
 */

#define PNG_INTR_FLAG_FILTER_DONE 0 //0x01 << 0
#define PNG_INTR_FLAG_INFLATE_DONE  1//0x01 << 1 0x02
#define PNG_INTR_FLAG_TCOPY_ERR 2//0x04
#define PNG_INTR_FLAG_TLEN_ERR  3//0x08
#define PNG_INTR_FLAG_TGEN_ERR  4//0x10
#define PNG_INTR_FLAG_WIN_ERR   5//0x20
#define PNG_INTR_FLAG_CODE_ERR  6//0x40
#define PNG_INTR_FLAG_BTYPE_ERR 7//0x80
#define PNG_INTR_FLAG_LEN_ERR   8//0x100
#define PNG_INTR_FLAG_ROW_ERR   9//0x200
#define PNG_INTR_FLAG_FTYPE_ERR 10//0x400
#define PNG_INTR_FLAG_READ_DONE 11//0x800
#define PNG_INTR_FLAG_TIME_OUT  12//0x1000
#define PNG_INTR_FLAG_PNG_DONE  13//0x1000

#define GET_INTR_FIELD_VALUE(x)     (0x01 << x)

irqreturn_t PNG_InterruptHandler(int irq, void *pDevId, struct pt_regs *pstRegs)
{
    unsigned int intr_status, intr_flag = 0;

    intr_status = PNG_Reg_GetInterruptStatusAll();

	PNG_DEBUG("\n\n<PNG_InterruptHandler>\n");
    PNG_DEBUG("INTR STATUS : 0x%08X\n", intr_status);

    PNG_PrintStatus();

    if(intr_status & GET_INTR_FIELD_VALUE(PNG_INTR_FLAG_FILTER_DONE))
    {
        intr_flag |= GET_INTR_FIELD_VALUE(PNG_INTR_FLAG_FILTER_DONE);


	    PNG_DEBUG("PNG_INTR_FLAG_FILTER_DONE\n");
    }
    if(intr_status & GET_INTR_FIELD_VALUE(PNG_INTR_FLAG_INFLATE_DONE))
    {
        intr_flag |= GET_INTR_FIELD_VALUE(PNG_INTR_FLAG_INFLATE_DONE);

	    PNG_DEBUG("PNG_INTR_FLAG_INFLATE_DONE\n");


    }
    if(intr_status & GET_INTR_FIELD_VALUE(PNG_INTR_FLAG_TCOPY_ERR))
    {
        intr_flag |= GET_INTR_FIELD_VALUE(PNG_INTR_FLAG_TCOPY_ERR);

	    PNG_DEBUG("PNG_INTR_FLAG_TCOPY_ERR\n");


    }
    if(intr_status & GET_INTR_FIELD_VALUE(PNG_INTR_FLAG_TLEN_ERR))
    {
        intr_flag |= GET_INTR_FIELD_VALUE(PNG_INTR_FLAG_TLEN_ERR);

	    PNG_DEBUG("PNG_INTR_FLAG_TLEN_ERR\n");


    }
    if(intr_status & GET_INTR_FIELD_VALUE(PNG_INTR_FLAG_TGEN_ERR))
    {
        intr_flag |= GET_INTR_FIELD_VALUE(PNG_INTR_FLAG_TGEN_ERR);


        PNG_DEBUG("PNG_INTR_FLAG_TGEN_ERR\n");

    }
    if(intr_status & GET_INTR_FIELD_VALUE(PNG_INTR_FLAG_WIN_ERR))
    {
        intr_flag |= GET_INTR_FIELD_VALUE(PNG_INTR_FLAG_WIN_ERR);

	    PNG_DEBUG("PNG_INTR_FLAG_WIN_ERR\n");


    }
    if(intr_status & GET_INTR_FIELD_VALUE(PNG_INTR_FLAG_CODE_ERR))
    {
        intr_flag |= GET_INTR_FIELD_VALUE(PNG_INTR_FLAG_CODE_ERR);

	    PNG_DEBUG("PNG_INTR_FLAG_CODE_ERR\n");


    }
    if(intr_status & GET_INTR_FIELD_VALUE(PNG_INTR_FLAG_BTYPE_ERR))
    {
        intr_flag |= GET_INTR_FIELD_VALUE(PNG_INTR_FLAG_BTYPE_ERR);

	    PNG_DEBUG("PNG_INTR_FLAG_BTYPE_ERR\n");


    }
    if(intr_status & GET_INTR_FIELD_VALUE(PNG_INTR_FLAG_LEN_ERR))
    {
        intr_flag |= GET_INTR_FIELD_VALUE(PNG_INTR_FLAG_LEN_ERR);


        //ICOD_Reg_Reset();
	    PNG_DEBUG("PNG_INTR_FLAG_LEN_ERR\n");

    }
    if(intr_status & GET_INTR_FIELD_VALUE(PNG_INTR_FLAG_ROW_ERR))
    {
        intr_flag |= GET_INTR_FIELD_VALUE(PNG_INTR_FLAG_ROW_ERR);


        PNG_DEBUG("PNG_INTR_FLAG_ROW_ERR\n");

    }
    if(intr_status & GET_INTR_FIELD_VALUE(PNG_INTR_FLAG_FTYPE_ERR))
    {
        intr_flag |= GET_INTR_FIELD_VALUE(PNG_INTR_FLAG_FTYPE_ERR);

	    PNG_DEBUG("PNG_INTR_FLAG_FTYPE_ERR\n");


    }
    if(intr_status & GET_INTR_FIELD_VALUE(PNG_INTR_FLAG_READ_DONE))
    {
        intr_flag |= GET_INTR_FIELD_VALUE(PNG_INTR_FLAG_READ_DONE);

	    PNG_DEBUG("PNG_INTR_FLAG_READ_DONE\n");


    }
    if(intr_status & GET_INTR_FIELD_VALUE(PNG_INTR_FLAG_TIME_OUT))
    {
        intr_flag |= GET_INTR_FIELD_VALUE(PNG_INTR_FLAG_TIME_OUT);

	    PNG_DEBUG("PNG_INTR_FLAG_TIME_OUT\n");


    }
    if(intr_status & GET_INTR_FIELD_VALUE(PNG_INTR_FLAG_PNG_DONE))
    {
        intr_flag |= GET_INTR_FIELD_VALUE(PNG_INTR_FLAG_PNG_DONE);

		_pSemDecode = 1;
		wake_up_interruptible(&g_PNG_device->png_wq);

        g_PNG_device->output_addr = PNG_Reg_GetWriteStartAddress();
        //g_PNG_device->output_length = PNG_Reg_GetCurrentWriteAddress() - PNG_Reg_GetWriteStartAddress();

        ICOD_Reg_Reset();
	    PNG_DEBUG("PNG_INTR_FLAG_PNG_DONE\n");
    }

    PNG_Reg_ClearInterrupt(intr_flag);
    PNG_PrintStatus();

	return IRQ_HANDLED;
}

void PNG_PrintStatus(void)
{
    unsigned int main_state, table_state, inflate_state;
    unsigned int in_fifo_valid_bits, fifo_space, filter_type;
    unsigned int cur_read_addr, cur_write_addr, write_offset, write_boundary;
    unsigned int filter_done, filter_last_line, inflate_done;
    unsigned int time_count;

    main_state = PNG_Reg_GetMainState();
    table_state = PNG_Reg_GetTableState();
    inflate_state = PNG_Reg_GetInflateState();

    in_fifo_valid_bits = PNG_Reg_GetInFIFOValidBits();
    fifo_space = PNG_Reg_GetFIFOSpace();

    cur_read_addr = PNG_Reg_GetCurrentReadAddress();
    cur_write_addr = PNG_Reg_GetCurrentWriteAddress();
    write_offset = PNG_Reg_GetWriteOffset();
    write_boundary = PNG_Reg_GetWriteAddressBoundary();

    filter_type = PNG_Reg_GetFilterType();

    filter_done = PNG_Reg_GetFilterDone();
    filter_last_line = PNG_Reg_GetFilterLastLine();
    inflate_done = PNG_Reg_GetInflateDone();

    time_count = PNG_Reg_GetCurrentTimeCount();

    PNG_INFO("\n");
    PNG_INFO("[PNG status]\n");
    PNG_INFO("cur time cnt : %d(0x%08X)\n", time_count, time_count);
    PNG_INFO("main : %d, table : %d, inflate : %d, filter type : %d\n", main_state, table_state, inflate_state, filter_type);
    PNG_INFO("in fifo valid bits : %d, r fifo space : %d\n", in_fifo_valid_bits, fifo_space);
    PNG_INFO("\n");
    PNG_INFO("cur read ptr : 0x%08X, cur write ptr : 0x%08X, write offset : 0x%08X(%d), boundary : 0x%08X\n", cur_read_addr, cur_write_addr, write_offset, write_offset, write_boundary);
    PNG_INFO("filter done : %d, filter last line : %d, inflate done : %d\n", filter_done, filter_last_line, inflate_done);
}


///////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef KDRV_GLOBAL_LINK
module_init(PNG_Init);
module_exit(PNG_Cleanup);

MODULE_AUTHOR("LGE");
MODULE_DESCRIPTION("PNG driver");
MODULE_LICENSE("GPL");
#endif

/** @} */

