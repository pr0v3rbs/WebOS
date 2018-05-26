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
 *  main driver implementation for ci device.
 *	ci device will teach you how to make device driver with new platform.
 *
 *  author		Srinivasan Shanmugam	(srinivasan.shanmugam@lge.com)
 *  author		Hwajeong Lee (hwajeong.lee@lge.com)
 *  author		Jinhwan Bae (jinhwan.bae@lge.com) - modifier
 *  author		Hyunho Kim (hyunho747.kim@lge.com) - modifier
 *  version		1.0
 *  date		2009.12.30
 *  note		Additional information.
 *
 *  @addtogroup lg1150_ci
 *	@{
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/
#undef	SUPPORT_CI_DEVICE_READ_WRITE_FOPS

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
#ifdef KDRV_CONFIG_PM	// added by SC Jung for quick booting
#include <linux/platform_device.h>
#endif
#include "os_util.h"
#include "base_device.h"
#include "ci_drv.h"
#include "ci_coredrv.h"
#include "ci_regdefs.h"
#include "ci_io.h"
#include "ci_io_dma.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/

#ifdef KDRV_CONFIG_PM
typedef struct
{
	// add here extra parameter
	bool	is_suspended;
}CI_DRVDATA_T;

#endif
/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/
extern	void	CI_PROC_Init(void);
extern	void	CI_PROC_Cleanup(void);

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Functions
----------------------------------------------------------------------------------------*/
int CI_Init(void);
void CI_Cleanup(void);

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/
int		g_ci_debug_fd = -1;
int		g_ci_burst_mode = 0;
int		g_ci_major = CI_MAJOR;
int		g_ci_minor = CI_MINOR;

#ifdef KDRV_CONFIG_PM

	S_CI_REG_T gCIReg_QB;

	extern volatile S_CI_REG_T* gpstCIReg;

#endif /* KDRV_CONFIG_PM */
/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/
static int      CI_Open(struct inode *, struct file *);
static int      CI_Close(struct inode *, struct file *);

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)
static int 		CI_Ioctl (struct inode *, struct file *, unsigned int, unsigned long );
#else
static long		CI_Ioctl (struct file *, unsigned int, unsigned long );
#endif

#ifdef SUPPORT_CI_DEVICE_READ_WRITE_FOPS
static ssize_t  CI_Read(struct file *, char *, size_t, loff_t *);
static ssize_t  CI_Write(struct file *, const char *, size_t, loff_t *);
#endif

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/

/**
 * main control block for ci device
*/
static CI_DEVICE_T*		g_ci_device;

/**
 * file I/O description for ci device
 *
*/
static struct file_operations g_ci_fops =
{
	.open 	= CI_Open,
	.release= CI_Close,

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)
	.ioctl	= CI_Ioctl,
#else
	.unlocked_ioctl = CI_Ioctl,
#endif

#ifdef SUPPORT_CI_DEVICE_READ_WRITE_FOPS
	.read 	= CI_Read,
	.write 	= CI_Write,
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
static int CI_suspend(struct platform_device *pdev, pm_message_t state)
{
#ifdef KDRV_CONFIG_PM_CI
	CI_DRVDATA_T *drv_data;

	printk("CI - suspend START\n");

	drv_data = platform_get_drvdata(pdev);

	if (drv_data->is_suspended == 1) {
		printk("CI - suspend FAIL\n");
		return -1;	//If already in suspend state, so ignore
	}

	/* unmap SMC only, resume do set again with ioremap and default init */
	CI_UnmapSMC();
	ci_dma_uninit();

	drv_data->is_suspended = 1;
	printk("CI - suspend OK\n");
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
static int CI_resume(struct platform_device *pdev)
{
#ifdef KDRV_CONFIG_PM_CI
	CI_DRVDATA_T *drv_data;

	printk("CI - resume START\n");

	drv_data = platform_get_drvdata(pdev);

	if (drv_data->is_suspended == 0) {
		printk("CI - resume FAIL\n");
		return -1;	//If already in resume state, so ignore
	}

	ci_dma_init();
	/* resume , SMC set with ioremap and do Default Init */
	CI_InitSMC();
	CI_DefaultInit();

	drv_data->is_suspended = 0;

	printk("CI - resume OK\n");
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
 int CI_probe(struct platform_device *pdev)
{

	CI_DRVDATA_T *drv_data;

	drv_data = (CI_DRVDATA_T *)kmalloc(sizeof(CI_DRVDATA_T) , GFP_KERNEL);


	// add here driver registering code & allocating resource code

	CI_SOC_MSG_DBG("[%s] done probe\n", CI_MODULE);
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
static int  CI_remove(struct platform_device *pdev)
{
	CI_DRVDATA_T *drv_data;

	// add here driver unregistering code & deallocating resource code



	drv_data = platform_get_drvdata(pdev);
	kfree(drv_data);

	CI_SOC_MSG_DBG("released\n");

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
static void  CI_release(struct device *dev)
{
	CI_SOC_MSG_DBG("device released\n");
}

/*
 *	module platform driver structure
 */
static struct platform_driver ci_driver =
{
	.probe          = CI_probe,
	.suspend        = CI_suspend,
	.remove         = CI_remove,
	.resume         = CI_resume,
	.driver         =
	{
		.name   = CI_MODULE,
	},
};

static struct platform_device ci_device = {
	.name = CI_MODULE,
	.id = 0,
	.id = -1,
	.dev = {
		.release = CI_release,
	},
};
#endif

/** Initialize the device environment before the real H/W initialization
 *
 *  @note main usage of this function is to initialize the HAL layer and memory size adjustment
 *  @note it's natural to keep this function blank :)
 */
void CI_PreInit(void)
{
    /* TODO: do something */
}

static void CI_Device_Free(void)
{
	if (g_ci_device != NULL) {
		OS_Free(g_ci_device);
		g_ci_device = NULL;
	}
}

static int CI_Device_Alloc(void)
{
	g_ci_device = (CI_DEVICE_T*)OS_KMalloc( sizeof(CI_DEVICE_T)*CI_MAX_DEVICE );
	if (g_ci_device == NULL) {
		CI_SOC_MSG_ERR("out of memory. can't allocate %d bytes\n", sizeof(CI_DEVICE_T)* CI_MAX_DEVICE );
		return -ENOMEM;
	}
	CI_SOC_MSG_DBG("g_ci_device[%d] allocated\n", CI_MAX_DEVICE);
	memset( g_ci_device, 0x0, sizeof(CI_DEVICE_T)* CI_MAX_DEVICE );
	return 0;
}

static void CI_Chrdev_Free(void)
{
	dev_t devt = MKDEV(g_ci_major, 0);
	unregister_chrdev_region(devt, CI_MAX_DEVICE);
}

static int CI_Chrdev_Region(void)
{
	dev_t devt;
	int err;

	if (g_ci_major) {
		devt = MKDEV(g_ci_major, g_ci_minor);
		err = register_chrdev_region(devt, CI_MAX_DEVICE, CI_MODULE);
	} else {
		err = alloc_chrdev_region(&devt, g_ci_minor, CI_MAX_DEVICE, CI_MODULE);
		g_ci_major = MAJOR(devt);
	}
	if (err < 0) {
		CI_SOC_MSG_ERR("can't register ci device\n");
		return -EIO;
	}
	CI_SOC_MSG_DBG("g_ci_major[%d] allocated\n", g_ci_major);
	return 0;
}

static void CI_Cdev_Del(void)
{
	int i;

	for (i=0; i<CI_MAX_DEVICE;i++) {
		/* TODO: cleanup each minor device */

		/* END */
		OS_DestroyDeviceClass(g_ci_device[i].devno);
		cdev_del(&(g_ci_device[i].cdev));
	}
}

static int CI_Cdev_Add(void)
{
	dev_t devt;
	int i, j;
	int err;

	for (i=0; i<CI_MAX_DEVICE; i++) {
		/* initialize cdev structure with predefined variable */
		devt = MKDEV( g_ci_major, g_ci_minor+i );
		cdev_init( &(g_ci_device[i].cdev), &g_ci_fops);
		g_ci_device[i].devno		= devt;
		g_ci_device[i].cdev.owner = THIS_MODULE;
		g_ci_device[i].cdev.ops   = &g_ci_fops;

		/* TODO: initialize minor device */


		/* END */

		err = cdev_add(&(g_ci_device[i].cdev), devt, 1);
		if (err) {
			CI_SOC_MSG_ERR("error (%d) while adding ci device (%d.%d)\n",
							err, MAJOR(devt), MINOR(devt) );
		#if (CI_MAX_DEVICE > 1)
			for (j=0 ; j<i ; j++) {
				OS_DestroyDeviceClass(g_ci_device[i].devno);
				cdev_del(&(g_ci_device[i].cdev));
			}
		#endif
			return -EIO;
		}
		CI_SOC_MSG_DBG("g_ci_device[0x08%x] added\n", devt);
        OS_CreateDeviceClass(g_ci_device[i].devno, "%s%d", CI_MODULE, i);
	}
	return 0;
}

/**
 *	Initialize CI device
*/
int CI_Init(void)
{
	int			err;

	/* Get the handle of debug output for ci device.
	 *
	 * Most module should open debug handle before the real initialization of module.
	 * As you know, debug_util offers 4 independent debug outputs for your device driver.
	 * So if you want to use all the debug outputs, you should initialize each debug output
	 * using OS_DEBUG_EnableModuleByIndex() function.
	 */
	g_ci_debug_fd = DBG_OPEN(CI_MODULE);
	if ( g_ci_debug_fd >= 0 ) {
		OS_DEBUG_EnableModule ( g_ci_debug_fd );
		OS_DEBUG_EnableModuleByIndex ( g_ci_debug_fd, CI_ERR, DBG_COLOR_NONE );
		OS_DEBUG_EnableModuleByIndex ( g_ci_debug_fd, CI_NOTI, DBG_COLOR_NONE );	/* NOTI */

	#if defined(CI_DEBUG_PRINT_ON)
		OS_DEBUG_EnableModuleByIndex ( g_ci_debug_fd, CI_INFO, DBG_COLOR_NONE );
		OS_DEBUG_EnableModuleByIndex ( g_ci_debug_fd, CI_TRACE, DBG_COLOR_NONE );
		OS_DEBUG_EnableModuleByIndex ( g_ci_debug_fd, CI_DBG_INFO, DBG_COLOR_NONE );
		//
		OS_DEBUG_EnableModuleByIndex ( g_ci_debug_fd, CIS_INFO, DBG_COLOR_NONE );
		OS_DEBUG_EnableModuleByIndex ( g_ci_debug_fd, CIS_TUPLE_DUMP, DBG_COLOR_NONE );
		OS_DEBUG_EnableModuleByIndex ( g_ci_debug_fd, CIS_PARSE_DUMP, DBG_COLOR_NONE );
		OS_DEBUG_EnableModuleByIndex ( g_ci_debug_fd, CIS_CIPLUS, DBG_COLOR_NONE );
		//
		OS_DEBUG_EnableModuleByIndex ( g_ci_debug_fd, CI_IO_INFO, DBG_COLOR_NONE );
		OS_DEBUG_EnableModuleByIndex ( g_ci_debug_fd, CIS_ERR, DBG_COLOR_NONE );
	#endif

	#if 0
		LOGM_REG_CUS_BIT_NAME(g_ci_debug_fd, CI_DRV);			/* 6 */
		LOGM_REG_CUS_BIT_NAME(g_ci_debug_fd, CI_NORMAL);		/* 7 */
		LOGM_REG_CUS_BIT_NAME(g_ci_debug_fd, CI_ISR);			/* 8 */
		LOGM_REG_CUS_BIT_NAME(g_ci_debug_fd, CI_DBG_INFO);		/* 9 */
		LOGM_REG_CUS_BIT_NAME(g_ci_debug_fd, CIS_INFO);			/* 10 */
		LOGM_REG_CUS_BIT_NAME(g_ci_debug_fd, CIS_TUPLE_DUMP);	/* 11 */
		LOGM_REG_CUS_BIT_NAME(g_ci_debug_fd, CIS_PARSE_DUMP);	/* 12 */
		LOGM_REG_CUS_BIT_NAME(g_ci_debug_fd, CI_IO_INFO);		/* 13 */
		LOGM_REG_CUS_BIT_NAME(g_ci_debug_fd, CIS_CIPLUS);		/* 14 */
		LOGM_REG_CUS_BIT_NAME(g_ci_debug_fd, CIS_ERR);			/* 15 */
		LOGM_REG_CUS_BIT_NAME(g_ci_debug_fd, CI_DBG_INFO_2);	/* 16 */
		LOGM_REG_CUS_BIT_NAME(g_ci_debug_fd, CI_DBG_INFO_3);	/* 17 */
		LOGM_REG_CUS_BIT_NAME(g_ci_debug_fd, CIS_TUPLE_DUMP_2);	/* 18 */
		LOGM_REG_CUS_BIT_NAME(g_ci_debug_fd, CIS_PARSE_DUMP_2);	/* 19 */
		LOGM_REG_CUS_BIT_NAME(g_ci_debug_fd, CI_ERR_2);			/* 20 */
	#endif

		CI_SOC_MSG_DBG("CI DBG_OPEN done[%d]\n", g_ci_debug_fd );
	} else {
		CI_SOC_MSG_ERR("CI DBG_OPEN failed\n" );
		return -EIO;
	}

#ifdef CI_BUILD_TIME_PRINT
	CI_SOC_MSG_NOTI("%s, %s\n", __DATE__, __TIME__);
#endif

#ifdef _CI_ACCESS_BURST_MODE_
	if ( lx_chip_rev() >= LX_CHIP_REV(H15, A0) ) {
	//	g_ci_burst_mode = (1<<1);
	}
	else if(lx_chip() == LX_CHIP_M14) {
		if( ( lx_chip_rev() >= LX_CHIP_REV(M14, B0) ) && ( lx_chip_rev() < LX_CHIP_REV(H14, A0) ) )
		{
			g_ci_burst_mode = (1<<0);
		}
	}
	CI_SOC_MSG_DBG("CI Burst Mode : %s\n",
				(g_ci_burst_mode)? "ON" : "OFF");
#endif

	/* allocate main device handler, register current device.
	 *
	 * If devie major is predefined then register device using that number.
	 * otherwise, major number of device is automatically assigned by Linux kernel.
	 *
	 */
#ifdef KDRV_CONFIG_PM
	// added by SC Jung for quick booting
	err = platform_driver_register(&ci_driver);
	if (err < 0) {
		CI_SOC_MSG_ERR("CI platform driver register failed\n");
		goto out;
	} else {
		CI_SOC_MSG_DBG("CI platform driver registered[%d]\n", err );

		err = platform_device_register(&ci_device);
		if (err) {
			platform_driver_unregister(&ci_driver);
			CI_SOC_MSG_ERR("CI platform device register failed\n");
			goto out;
		} else {
			CI_SOC_MSG_DBG("[%s] platform register done\n", CI_MODULE);
		}
	}
#endif

	err = CI_Device_Alloc();
	if (err) {
		CI_SOC_MSG_ERR("CI_Device_Alloc failed\n");
		goto out_plt;
	}

	err = CI_Chrdev_Region();
	if (err) {
		CI_SOC_MSG_ERR("CI_Chrdev_Region failed\n");
		goto out_device;
	}

	/* TODO : initialize your module not specific minor device */


	/* END */

	err = CI_Cdev_Add();
	if (err) {
		CI_SOC_MSG_ERR("CI_Cdev_Add failed\n");
		goto out_chrdev;
	}

	/* initialize proc system */
	CI_PROC_Init();

	err = HW_IO_Init();
	if (err) {
		CI_SOC_MSG_ERR("HW_IO_Init failed\n");
		goto out_proc;
	}

	/* initialze CI H/W */
	err = CI_Initialize();
	if (err) {
		CI_SOC_MSG_ERR("CI_Initialize failed\n");
		goto out_hw_init;
	}

	CI_SOC_MSG_NOTI("CI device initialized\n");

	return 0;

out_proc:
	CI_PROC_Cleanup();
	CI_Cdev_Del();

out_hw_init:
	HW_IO_Uninit();

out_chrdev:
	CI_Chrdev_Free();

out_device:
	CI_Device_Free();

out_plt:
#ifdef KDRV_CONFIG_PM
	// added by SC Jung for quick booting
	platform_driver_unregister(&ci_driver);
	platform_device_unregister(&ci_device);
#endif

out:
//	OS_DEBUG_DisableModule(g_ci_debug_fd);
	DBG_CLOSE(g_ci_debug_fd);
	return err;
}


/**
 *	Cleanup CI device
*/
void CI_Cleanup(void)
{
	/* TODO : cleanup your module not specific minor device */

	/* */

	// shutdown H/W
	CI_UnInitialize();

	HW_IO_Uninit();

	/* cleanup proc system */
	CI_PROC_Cleanup();

	/* remove all minor devicies and unregister current device */
	CI_Cdev_Del();

	CI_Chrdev_Free();

	CI_Device_Free();

#ifdef KDRV_CONFIG_PM
	// added by SC Jung for quick booting
	platform_driver_unregister(&ci_driver);
	platform_device_unregister(&ci_device);
#endif

//	OS_DEBUG_DisableModule(g_ci_debug_fd);
	DBG_CLOSE(g_ci_debug_fd);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * open handler for ci device
 *
 */
static int CI_Open(struct inode *inode, struct file *filp)
{
    int					major,minor;
    struct cdev*    	cdev;
    CI_DEVICE_T*	my_dev;

    cdev = inode->i_cdev;
    my_dev = container_of ( cdev, CI_DEVICE_T, cdev);

#if 0
	if (my_dev->dev_open_count == 0)
	{	// check first open
		CI_Initialize();	// init H/W
	}
#endif


    my_dev->dev_open_count++;
    filp->private_data = my_dev;

	/* some debug */
    major = imajor(inode);
    minor = iminor(inode);

    CI_SOC_MSG_DBG("[ CI_Close: CI device opened  <<S>> ]  \n");

    return 0;
}

/**
 * release handler for ci device
 *
 */
static int CI_Close(struct inode *inode, struct file *filp)
{
    int					major,minor;
    CI_DEVICE_T*		my_dev;
#if 0
    struct cdev*		cdev;

    cdev	= inode->i_cdev;
    my_dev	= container_of ( cdev, CI_DEVICE_T, cdev);
#else
	my_dev = filp->private_data;
#endif

    if (my_dev->dev_open_count > 0) {
        --my_dev->dev_open_count;
	#if 0
		if (my_dev->dev_open_count <= 0) {		// check last close
			CI_UnInitialize();					// shutdown H/W
		}
	#endif
	}

	/* some debug */
    major = imajor(inode);
    minor = iminor(inode);

	CI_SOC_MSG_DBG("[ CI_Close: CI device closed  <<S>> ]  \n");

    return 0;
}

/**
 * ioctl handler for ci device.
 *
 *
 * note: if you have some critial data, you should protect them using semaphore or spin lock.
 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)
static int CI_Ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg)
#else
static long CI_Ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
#endif
{
    int err = 0, ret = 0;

#if 0
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)
    CI_DEVICE_T*	my_dev;
    struct cdev*		cdev;

	// get current ci device object
    cdev	= inode->i_cdev;
    my_dev	= container_of ( cdev, CI_DEVICE_T, cdev);

	// if need iode
	// struct inode *inode = filp->f_path.dentry->d_inode;
#endif
#else
	CI_DEVICE_T*	my_dev = filp->private_data;
#endif

    /*
     * check if IOCTL command is valid or not.
     * - if magic value doesn't match, return error (-ENOTTY)
     * - if command is out of range, return error (-ENOTTY)
     *
     * note) -ENOTTY means "Inappropriate ioctl for device.
     */
    if (_IOC_TYPE(cmd) != CI_IOC_MAGIC)
    {
    	DBG_PRINT_WARNING("invalid magic. magic=0x%02X\n", _IOC_TYPE(cmd) );
    	return -ENOTTY;
    }
    if (_IOC_NR(cmd) > CI_IOC_MAXNR)
    {
    	DBG_PRINT_WARNING("out of ioctl command. cmd_idx=%d\n", _IOC_NR(cmd) );
    	return -ENOTTY;
    }
#if 0
    if (_IOC_DIR(cmd) & (_IOC_WRITE|_IOC_READ)) {
		if (!arg) {
			vipm_err("NULL arg\n");
			return -ENOTTY;
		}
	}
#endif

	/* TODO : add some check routine for your device */

    /*
     * check if user memory is valid or not.
     * if memory can't be accessed from kernel, return error (-EFAULT)
     */
    if (_IOC_DIR(cmd) & _IOC_READ)
        err = !access_ok(VERIFY_WRITE, (void __user *)arg, _IOC_SIZE(cmd));
    else if (_IOC_DIR(cmd) & _IOC_WRITE)
        err =  !access_ok(VERIFY_READ, (void __user *)arg, _IOC_SIZE(cmd));
    if (err) {
    	DBG_PRINT_WARNING("memory access error. cmd_idx=%d, rw=%c%c, memptr=%p\n",
    													_IOC_NR(cmd),
    													(_IOC_DIR(cmd) & _IOC_READ)? 'r':'-',
    													(_IOC_DIR(cmd) & _IOC_WRITE)? 'w':'-',
    													(void*)arg );
        return -EFAULT;
	}

	//CI_SOC_MSG_TRACE("cmd = %08X (cmd_idx=%d)\n", cmd, _IOC_NR(cmd) );
//	CI_SOC_MSG_DBG("CMD = 0x%02X", _IOC_NR(cmd));

	switch(cmd)
	{
		case CI_IO_RESET:
		{
			ret = CI_ResetCI();
		}
		break;

		case CI_IO_CHECK_CIS:
		{
			ret = CI_CheckCIS();
		}
		break;

		case CI_IO_WRITE_COR:
		{
			ret = CI_WriteCOR();
		}
		break;

		case CI_IOR_DETECT_CARD:
		{
			UINT32 uwDetect = 0;

			if (!arg) {
				CI_SOC_MSG_ERR("Null arg\n");
				return -ENOTTY;
			}

			ret = CI_CAMDetectStatus( &uwDetect );
			if( ret == 0 )
			{
				CI_SOC_MSG_DBG("[ CI Card %s <<S>> ]  \n", ((uwDetect==1)? ("INSERTED") : ("REMOVED")));

	 	        if( copy_to_user( ( void __user * )arg, &uwDetect, sizeof( UINT32 ) ) )
	 	        {
					CI_SOC_MSG_ERR("[ CI_Ioctl(CI_IOR_DETECT_CARD): copy_to_user <<F>> ]\n");
    	        	return -1;
	 	        }
			}
		}
		break;

		case CI_IOR_READ_DATA:
		{
			LX_CI_IOCTL_PARAM_T stReadData;

			if (!arg) {
				CI_SOC_MSG_ERR("Null arg\n");
				return -ENOTTY;
			}

			if( copy_from_user( &stReadData, (void __user *)arg, sizeof( LX_CI_IOCTL_PARAM_T ) ) )
			{
				CI_SOC_MSG_ERR("[ CI_Ioctl(CI_IOR_READ_DATA): copy_to_user <<F>> ]\n");
	        	return -1;
 	        }

			ret = CI_ReadData( stReadData.pBuf, &stReadData.sLength, 1 );

			if( copy_to_user( ( void __user * )arg, &stReadData, sizeof( LX_CI_IOCTL_PARAM_T ) ) )
 	        {
				CI_SOC_MSG_ERR(" copy_to_user <<F>>\n" );
	        	return -1;
 	        }
		}
		break;

		case CI_IOR_NEGOTIATE_BUF_SIZE:
		{
			UINT32 uwBufSize = 0;

			if (!arg) {
				CI_SOC_MSG_ERR("Null arg\n");
				return -ENOTTY;
			}

			//Negotiate Buffer size
			ret = CI_NegoBuff( &uwBufSize );
			if( ret == 0 )
			{
				// return the resulted size
				CI_SOC_MSG_DBG("Negotiated BUF size %d <<S>> ]\n", uwBufSize);

				if( copy_to_user( ( void __user * )arg, &uwBufSize, sizeof( UINT32 ) ) )
	 	        {
					CI_SOC_MSG_ERR("[ CI_Ioctl(CI_IOR_NEGOTIATE_BUF_SIZE): copy_to_user <<F>> ] \n");
    	        	return -1;
	 	        }
			}
			else
			{
				CI_SOC_MSG_ERR("[ CI_Ioctl(CI_IOR_NEGOTIATE_BUF_SIZE): "
								"Calc Negotiation Buffer <<F>> ]\n");
			}
		}
		break;

		case CI_IOR_READ_DA_STATUS:
		{
			UINT32 uwDataAvailable = 0;

			if (!arg) {
				CI_SOC_MSG_ERR("Null arg\n");
				return -ENOTTY;
			}

			//read the status of being Data-Available
			ret = CI_ReadDAStatus( &uwDataAvailable, 1 );
			if( ret == 0 )
			{
				CI_SOC_MSG_DBG("[ CI Read DA status is %s <<S>> ]\n",
						((uwDataAvailable)? ("Available") : ("Not Available")) );

				if( copy_to_user( ( void __user * )arg, &uwDataAvailable, sizeof( UINT32 ) ) )
	 	        {
					CI_SOC_MSG_ERR("[ CI_Ioctl(CI_IOR_READ_DA_STATUS): copy_to_user <<F>> ]\n");
    	        	return -1;
	 	        }
			}
		}
		break;

		case CI_IOW_WRITE_DATA:
		{
			LX_CI_IOCTL_PARAM_T stReadData;

			if (!arg) {
				CI_SOC_MSG_ERR("Null arg\n");
				return -ENOTTY;
			}

			if ( copy_from_user( &stReadData, (void __user *)arg, sizeof( LX_CI_IOCTL_PARAM_T ) ) )
			{
				CI_SOC_MSG_ERR("[ CI_Ioctl(CI_IOW_WRITE_DATA): copy_from_user <<F>> ]\n");
	        	return -1;
 	        }

			ret = CI_WriteData( stReadData.pBuf, stReadData.sLength, 1 );
		}
		break;

		case CI_IO_SET_PHY_RESET:
		{
			ret = CI_ResetPhysicalIntrf();
		}
		break;

		case CI_IO_SET_RS:
		{
			ret = CI_SetRS();
		}
		break;

		case CI_IOR_READ_IIR_STATUS:
		{
			// Read IIR status from status register and return it.
			UINT32 uwIIRStatus = 0;

			if (!arg) {
				CI_SOC_MSG_ERR("Null arg\n");
				return -ENOTTY;
			}

			//read the status of being Data-Available
			ret = CI_ReadIIRStatus( &uwIIRStatus, 1 );
			if( ret == 0 )
			{
				CI_SOC_MSG_DBG("[ CI_Ioctl(CI_IOR_READ_IIR_STATUS): Read IIR status is %s <<S>> ]\n", ((uwIIRStatus)? ("RESET") : ("NO")));

				if ( copy_to_user( ( void __user * )arg, &uwIIRStatus, sizeof( UINT32 ) ) )
	 	        {
					CI_SOC_MSG_ERR("[ CI_Ioctl(CI_IOR_READ_IIR_STATUS): copy_to_user <<F>> ]\n");
    	        	return -1;
	 	        }
			}
		}
		break;

		case CI_IOR_CHECK_CAPABILITY:
		{
			LX_CI_IOCTL_PARAM_CAMTYPE stData;

			if (!arg) {
				CI_SOC_MSG_ERR("Null arg\n");
				return -ENOTTY;
			}

			ret = CI_CheckCAMType( &stData.uwRtnValue, &stData.uwCheckCAMType );
			if (ret == 0) {
				if ( copy_to_user( ( void __user * )arg, &stData, sizeof( LX_CI_IOCTL_PARAM_CAMTYPE ) ) )
	 	        {
					CI_SOC_MSG_ERR("[ CI_Ioctl(CI_IOR_CHECK_CAPABILITY): copy_to_user <<F>> ]\n");
    	        	return -1;
	 	        }
			} else {
				CI_SOC_MSG_ERR("[ CI_Ioctl(CI_IOR_CHECK_CAPABILITY): Check CAM Type <<F>> ]\n");
			}
		}
		break;

		case CI_IOR_GET_CIPLUS_VERSION:
		{
			LX_CI_IOCTL_PARAM_VERSION stData;

			if (!arg) {
				CI_SOC_MSG_ERR("Null arg\n");
				return -ENOTTY;
			}

			ret = CI_GetCIPlusSupportVersion( &stData.uwRtnValue, &stData.uwVersion );
			if( ret == 0 )
			{
				if ( copy_to_user( ( void __user * )arg, &stData, sizeof( LX_CI_IOCTL_PARAM_VERSION ) ) )
	 	        {
					CI_SOC_MSG_ERR("[ CI_Ioctl(CI_IOR_GET_CIPLUS_VERSION): copy_to_user <<F>> ]\n");
    	        	return -1;
	 	        }
			} else {
				CI_SOC_MSG_ERR("[ CI_Ioctl(CI_IOR_GET_CIPLUS_VERSION): Check CAM Support Version <<F>> ]\n");
			}
		}
		break;

		case CI_IOR_GET_CIPLUS_OPROFILE:
		{
			LX_CI_IOCTL_PARAM_OPROFILE stData;

			if (!arg) {
				CI_SOC_MSG_ERR("Null arg\n");
				return -ENOTTY;
			}

			ret = CI_GetCIPlusOperatorProfile( &stData.uwRtnValue, &stData.uwProfile );
			if (ret == 0) {
				if ( copy_to_user( ( void __user * )arg, &stData, sizeof( LX_CI_IOCTL_PARAM_OPROFILE ) ) )
	 	        {
					CI_SOC_MSG_ERR("[ CI_Ioctl(CI_IOR_GET_CIPLUS_OPROFILE): copy_to_user <<F>> ]\n");
    	        	return -1;
	 	        }
			} else {
				CI_SOC_MSG_ERR("[ CI_Ioctl(CI_IOR_GET_CIPLUS_OPROFILE): Check CAM Operator Profile <<F>> ]\n");
			}
		}
		break;

		case CI_IOR_CAM_INIT:
		{
			ret = CI_CAMInit();
		}
		break;

		case CI_IOR_PRINT_REG:
		{
			ret = CI_RegPrint();
		}
		break;

		case CI_IOR_CAM_POWEROFF:
		{
			ret = CI_CAMPowerOff();
		}
		break;

		case CI_IOR_CAM_POWERONCOMPLETED:
		{
			ret = CI_CAMPowerOnCompleted();
		}
		break;

		case CI_IOW_CAM_SET_DELAY:
		{
			LX_CI_IOCTL_PARAM_SETDELAY stSetDelay;

			if (!arg) {
				CI_SOC_MSG_ERR("Null arg\n");
				return -ENOTTY;
			}

			if( copy_from_user( &stSetDelay, (void __user *)arg, sizeof( LX_CI_IOCTL_PARAM_SETDELAY ) ) )
			{
				CI_SOC_MSG_ERR("[ CI_Ioctl(CI_IOW_CAM_SET_DELAY): copy_from_user <<F>> ]\n");
	        	return -1;
 	        }
			ret = CI_CAMSetDelay( stSetDelay.eDelayType , stSetDelay.uiDelayValue);
		}
		break;

		case CI_IO_CAM_PRINT_DELAY_VALUES:
		{
			ret = CI_CAMPrintDelayValues();
		}
		break;

		case CI_IO_READ_REGISTERS:
		{
			ret = CI_RegPrint();
		}
		break;

		case CI_IOW_WRITE_REGISTER:
		{
			LX_CI_IOCTL_PARAM_REGISTER stRegWrite;

			if (!arg) {
				CI_SOC_MSG_ERR("Null arg\n");
				return -ENOTTY;
			}

			if( copy_from_user( &stRegWrite, (void __user *)arg, sizeof( LX_CI_IOCTL_PARAM_REGISTER ) ) )
			{
				CI_SOC_MSG_ERR("[ CI_Ioctl(LX_CI_IOCTL_PARAM_REGISTER): copy_from_user <<F>> ]\n");
	        	return -1;
 	        }

			ret = CI_RegWrite( stRegWrite.uiOffset, stRegWrite.uiValue );
		}
		break;

		case CI_IOW_SET_PCMCIA_SPEED:
		{
			LX_CI_IOCTL_PARAM_SPEED stPCMCIASpeed;
			CI_BUS_SPEED_T speed;

			if (!arg) {
				CI_SOC_MSG_ERR("Null arg\n");
				return -ENOTTY;
			}

			CI_SOC_MSG_NOTI("\n\n CI_IOW_SET_PCMCIA_SPEED 1 \n\n");

			if( copy_from_user( &stPCMCIASpeed, (void __user *)arg, sizeof( LX_CI_IOCTL_PARAM_SPEED ) ) )
			{
				CI_SOC_MSG_ERR("[ CI_Ioctl(LX_CI_IOCTL_PARAM_SPEED): copy_from_user <<F>> ]\n");
	        	return -1;
 	        }

			if(stPCMCIASpeed.ui8Speed == 0) speed = PCMCIA_BUS_SPEED_LOW;
			else						 speed = PCMCIA_BUS_SPEED_HIGH;
			ret = CI_SetPCCardBusTiming( speed );
		}
		break;

		case CI_IOW_ENABLE_LOG:
		{
			LX_CI_IOCTL_PARAM_LOG_ENABLE stLogEnable;

			if (!arg) {
				CI_SOC_MSG_ERR("Null arg\n");
				return -ENOTTY;
			}

			if( copy_from_user( &stLogEnable, (void __user *)arg, sizeof( LX_CI_IOCTL_PARAM_LOG_ENABLE ) ) )
			{
				CI_SOC_MSG_ERR("[ CI_Ioctl(LX_CI_IOCTL_PARAM_LOG_ENABLE): copy_from_user <<F>> ]\n");
	        	return -1;
			}

			ret = CI_IO_EnableLog(stLogEnable.mask);

			CI_SOC_MSG_DBG("CI_IOW_ENABLE_LOG ok\n");
		}
		break;

		case CI_IOW_CHANGE_ACCESSMODE:
		{
			LX_CI_IOCTL_PARAM_ACCESSMODE_CHANGE stAcessMode;
			CI_ACCESS_MODE_T mode;

			if (!arg) {
				CI_SOC_MSG_ERR("Null arg\n");
				return -ENOTTY;
			}

			if( copy_from_user( &stAcessMode, (void __user *)arg, sizeof( LX_CI_IOCTL_PARAM_ACCESSMODE_CHANGE ) ) )
			{
				CI_SOC_MSG_DBG("[ CI_Ioctl(LX_CI_IOCTL_PARAM_ACCESSMODE_CHANGE): copy_from_user <<F>> ]\n");
        		return -1;
	        }

			if	(stAcessMode.ui32Mode== 0) 			mode = ACCESS_1BYTE_MODE;
			else	if (stAcessMode.ui32Mode== 1 )		mode = ACCESS_2BYTE_MODE;
			else									mode = ACCESS_4BYTE_MODE;

			ret = CI_ChangeAccessMode( mode );
			if (LX_IS_ERR(ret)) {
				CI_SOC_MSG_ERR("CI_ChangeAccessMode failed:[%d]\n", ret);
				ret = -ENOTTY;
			} else {
				ret = 0;
			}
		}
		break;

		case CI_IO_DATA_TEST:
		{
		#if defined(CI_DATA_TEST_ON)
			ret = CI_DataTest(MOD_A);
		#else
			CI_SOC_MSG_WARN("IOCTL(CI_IO_DATA_TEST) disabled\n");
			ret = 0;
		#endif
		}
		break;

		case CI_IOWR_OPTION:
		{
			LX_CI_IOCTL_PARAM_OPTION stOption;
			UINT32 set_option = 0;
			UINT32 set_value;

			if (!arg) {
				CI_SOC_MSG_ERR("Null arg\n");
				return -ENOTTY;
			}

			if( copy_from_user( &stOption, (void __user *)arg, sizeof( LX_CI_IOCTL_PARAM_OPTION ) ) )
			{
				CI_SOC_MSG_DBG("[ CI_Ioctl(LX_CI_IOCTL_PARAM_OPTION): copy_from_user <<F>> ]\n");
        		return -1;
	        }

			set_option = !!(stOption.ui32Option & 0x80000000);
			stOption.ui32Option &= 0xFFFF;

	        if (set_option) {	//Set new option
				switch (stOption.ui32Option) {
					case 0x0000 : HW_IO_SetChkMode(stOption.ui32Value); break;
					default : break;
				}
	        } else {			//Get current option
				switch (stOption.ui32Option) {
					case 0x0000 : stOption.ui32Value = HW_IO_GetChkMode(); break;
					default : break;
				}
	        }
			if (!set_option) {
				if( copy_to_user( ( void __user * )arg, &stOption, sizeof( LX_CI_IOCTL_PARAM_OPTION ) ) )
	 	        {
					CI_SOC_MSG_ERR("[ CI_Ioctl(LX_CI_IOCTL_PARAM_OPTION): copy_to_user <<F>> ]\n");
    	        	return -1;
	 	        }
			}
		}
		break;

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
user_initcall_grp("kdrv",CI_Init);
#else
module_init(CI_Init);
#endif
module_exit(CI_Cleanup);

MODULE_AUTHOR("LGE");
MODULE_DESCRIPTION("base driver");
MODULE_LICENSE("GPL");
#endif

/** @} */

