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



/** @file	audio_kdrv.c
 *
 *  main driver implementation for  audio device.
 *  audio device will teach you how to make device driver with new platform.
 *
 *  author	wonchang.shin (wonchang.shin@lge.com)
 *  version	0.1
 *  date		2012.04.24
 *  note		Additional information.
 *
 *  @addtogroup lg1150_audio
 *	@{
 */


/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/interrupt.h>    /**< For isr */
#include <linux/irq.h>			/**< For isr */
#include <linux/ioport.h>		/**< For request_region, check_region etc */
#include <linux/rmap.h>
#include <linux/kthread.h>
#include <asm/io.h>				/**< For ioremap_nocache */
#include <asm/memory.h>
#include <asm/uaccess.h>
#include <linux/poll.h>
#include <linux/version.h>

#ifdef KDRV_CONFIG_PM	// added by SC Jung for quick booting
#include <linux/platform_device.h>
#endif

#include "os_util.h"
#include "base_device.h"

#include "debug_util.h"
#include "audio_cfg.h"
#include "audio_drv.h"
#include "audio_imc_func.h"

#include "audio_drv_decoder.h"
#include "audio_drv_encoder.h"
#include "audio_drv_capturer.h"
#include "audio_drv_renderer.h"
#include "audio_drv_master.h"

#include "alsa/alsa_machine.h"


/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/
const char* gcAudDevMod[AUDIO_DEVICE_MAX] =
{
	"adec0",		// audio decoder 	00
	"aenc0",		// audio encoder 	01
	"acap0",		// audio capturer 	02
	"aren0",		// audio renderer 	03
	"amst0"			// audio master 	04
};

#ifdef KDRV_CONFIG_PM
typedef struct
{
	// add here extra parameter
	bool	is_suspended;
} AUD_DRVDATA_T;
#endif

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/
extern void	AUDIO_PROC_Init (char* procName);
extern void	AUDIO_PROC_Cleanup (char* procName);
extern irqreturn_t AUDIO_interrupt0 (int irq, void *dev_id, struct pt_regs *regs);
extern irqreturn_t AUDIO_interrupt1 (int irq, void *dev_id, struct pt_regs *regs);

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/
extern LX_AUD_REG_CFG_T	*pstAudRegCfg;
extern LX_AUD_KDRV_T	g_AudKdrv;
extern UINT8			g_AdcPortNum;

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/
int	g_AudDebugFd = 0;
int g_AudMajor 	 = AUDIO_MAJOR;
int g_AudMinor 	 = AUDIO_MINOR;
int	g_AudIrq0Dev = 0;
int	g_AudIrq1Dev = 0;

AUD_DEVICE_T 	*g_AudDev;

#ifdef KDRV_CONFIG_PM
bool 	g_audio_open_done = FALSE;

#ifdef DISABLE_SCART_HW_BYPASS
extern BOOLEAN	_gRenderScartReset;
#endif	//#ifdef DISABLE_SCART_HW_BYPASS

#endif

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/
static unsigned int KDRV_AUDIO_PollMaster(struct file *filp, poll_table *wait);

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
static struct file_operations g_audio_fops[LX_MAX_DEVICE_NAME] =
{
	// audio decoder
	{
	.open 		= KDRV_AUDIO_OpenDecoder,
	.release	= KDRV_AUDIO_CloseDecoder,
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)
	.ioctl		= KDRV_AUDIO_IoctlDecoder,
#else
	.unlocked_ioctl	= KDRV_AUDIO_IoctlDecoder,
#endif
	},
	// audio encoder
	{
	.open 		= KDRV_AUDIO_OpenEncoder,
	.release	= KDRV_AUDIO_CloseEncoder,
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)
	.ioctl		= KDRV_AUDIO_IoctlEncoder,
#else
	.unlocked_ioctl	= KDRV_AUDIO_IoctlEncoder,
#endif
	},
	// audio capturer
	{
	.open 	= KDRV_AUDIO_OpenCapture,
	.release	= KDRV_AUDIO_CloseCapture,
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)
	.ioctl		= KDRV_AUDIO_IoctlCapture,
#else
	.unlocked_ioctl	= KDRV_AUDIO_IoctlCapture,
#endif
	},
	// audio renderer
	{
	.open 	= KDRV_AUDIO_OpenRenderer,
	.release	= KDRV_AUDIO_CloseRenderer,
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)
	.ioctl		= KDRV_AUDIO_IoctlRenderer,
#else
	.unlocked_ioctl	= KDRV_AUDIO_IoctlRenderer,
#endif
	},
	// audio master
	{
	.open 		= KDRV_AUDIO_OpenMaster,
	.release	= KDRV_AUDIO_CloseMaster,
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)
	.ioctl		= KDRV_AUDIO_IoctlMaster,
#else
	.unlocked_ioctl	= KDRV_AUDIO_IoctlMaster,
#endif
	.poll 		= KDRV_AUDIO_PollMaster,
	},
};

/*========================================================================================
	Implementation Group
========================================================================================*/
#ifdef KDRV_CONFIG_PM	// added by SC Jung for quick booting
/**
 * Run suspend the adec module.
 * ADEC module shall be suspend itself with pre-defined configuration value.
 * @see ADEC_suspend
*/
int AUD_RunSuspend(void)
{
#ifdef KDRV_CONFIG_PM_AUDIO
	printk("AUD - suspend START\n");

	// SE Copy From
	AUDIO_SE_CopyFrom();

	printk("AUD - suspend OK\n");
#endif
	return 0;
}

/**
 * Run resume the adec module.
 * ADEC module shall be resumed itself with pre-defined configuration value.
 * @see ADEC_suspend
*/
int AUD_RunResume(void)
{
#ifdef KDRV_CONFIG_PM_AUDIO
	printk("AUD - resume START\n");

	//IMC Finalize
	(void)AUDIO_IMC_Finalize();
	(void)AUDIO_IPC_DBG_Finalize();

	//Initializes the audio module.
	(void)AUDIO_InitModule();

	// SE Copy To
	AUDIO_SE_CopyTo();

	//Restart
	(void)KDRV_AUDIO_ResetCapturer();
	(void)KDRV_AUDIO_ResetEncoder();
	(void)KDRV_AUDIO_ResetDisconnectMaster();
	(void)KDRV_AUDIO_ResetRenderer();
	(void)KDRV_AUDIO_ResetDecoder();
	(void)KDRV_AUDIO_ResetMaster();

	(void)KDRV_AUDIO_ResetMasterParam();
	(void)KDRV_AUDIO_ResetDecoderParam();
	(void)KDRV_AUDIO_ResetRendererParam();
	(void)KDRV_AUDIO_ResetConnectMaster();
	(void)KDRV_AUDIO_ResetEncoderParam();
	(void)KDRV_AUDIO_ResetCapturerParam();

	(void)KDRV_AUDIO_RestartMaster();
	(void)KDRV_AUDIO_RestartDecoder();
	(void)KDRV_AUDIO_RestartRenderer();
	(void)KDRV_AUDIO_RestartEncoder();
	(void)KDRV_AUDIO_RestartCapturer();

	printk("AUD - resume OK\n");
#endif
	return 0;
}

/**
 *
 * suspending module.
 *
 * @param	struct platform_device *pdev pm_message_t state
 * @return	int 0 : OK , -1 : NOT OK
 *
 */
static int AUD_Suspend(struct platform_device *pdev, pm_message_t state)
{
	AUD_DRVDATA_T *drv_data;

	if(g_audio_open_done == FALSE)
	{
		AUD_KDRV_DEBUG("[%s] AUDIO_Open is not done. Skip Suspend!!!\n", AUDIO_MODULE);
		return 0;
	}

	drv_data = platform_get_drvdata(pdev);

	// add here the suspend code
	if(drv_data->is_suspended == 1) return -1;

	// run to suspend mode
	AUD_RunSuspend();

	drv_data->is_suspended = 1;

	/* Update Internal Status Value */
	g_AudKdrv.bSuspended = TRUE;
	g_AudKdrv.bInitDone  = FALSE;

	/* Set a default ADC Number. */
	g_AdcPortNum = 0xFF;	//Not exist ADC Port Number.

	#ifdef DISABLE_SCART_HW_BYPASS
	_gRenderScartReset	 = FALSE;
	#endif	//#ifdef DISABLE_SCART_HW_BYPASS

	AUD_KDRV_PRINT("[%s] done suspend\n", AUDIO_MODULE);
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
static int AUD_Resume(struct platform_device *pdev)
{
	AUD_DRVDATA_T *drv_data;

	if(g_audio_open_done == FALSE)
	{
		AUD_KDRV_DEBUG("[%s] AUDIO_Open is not done. Skip Resume!!!\n", AUDIO_MODULE);
		return 0;
	}

	drv_data = platform_get_drvdata(pdev);

	if(drv_data->is_suspended == 0) return -1;

	// run to resume mode
	AUD_RunResume();

	drv_data->is_suspended = 0;

	/* Update Internal Status Value */
	g_AudKdrv.bSuspended = FALSE;
	g_AudKdrv.bInitDone  = TRUE;

	AUD_KDRV_PRINT("[%s] done resume!!!\n", AUDIO_MODULE);
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
static int AUD_Probe(struct platform_device *pdev)
{

	AUD_DRVDATA_T *drv_data;

	drv_data = (AUD_DRVDATA_T *)kmalloc(sizeof(AUD_DRVDATA_T), GFP_KERNEL);

	// add here driver registering code & allocating resource code

	drv_data->is_suspended = 0;
	platform_set_drvdata(pdev, drv_data);

	AUD_KDRV_PRINT("[%s] done probe\n", AUDIO_MODULE);

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
static int  AUD_Remove(struct platform_device *pdev)
{
	AUD_DRVDATA_T *drv_data;

	// add here driver unregistering code & deallocating resource code

	drv_data = platform_get_drvdata(pdev);
	kfree(drv_data);

	AUD_KDRV_PRINT("[%s] done removed\n", AUDIO_MODULE);

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
static void  AUD_Release(struct device *dev)
{
	AUD_KDRV_PRINT("[%s] device released\n", AUDIO_MODULE);
}

/*
 *	module platform driver structure
 */
static struct platform_driver aud_platform_driver =
{
	.probe          = AUD_Probe,
	.suspend        = AUD_Suspend,
	.remove         = AUD_Remove,
	.resume         = AUD_Resume,
	.driver         =
	{
		.name   = AUDIO_MODULE,
	},
};

static struct platform_device aud_platform_device = {
	.name = AUDIO_MODULE,
	.id = -1,
	.dev = {
		.release = AUD_Release,
	},
};
#endif

/** Initialize the device environment before the real H/W initialization
 *
 *  @note main usage of this function is to initialize the HAL layer and memory size adjustment
 *  @note it's natural to keep this function blank :)
 */
void AUDIO_PreInit(void)
{
    /* TODO: do something */
}

int AUDIO_Init(void)
{
	SINT32		retVal = 0;
	SINT32		i;
	dev_t		dev;
	char 		dev_name[LX_MAX_DEVICE_NAME];

	g_AudDebugFd = DBG_OPEN(AUDIO_MODULE);

	if(g_AudDebugFd < 0)
	{
		DBG_PRINT_ERROR("debug system shutdowned.\n" );
		return -EIO;
	}

	OS_DEBUG_EnableModule (g_AudDebugFd);
	OS_DEBUG_EnableModuleByIndex(g_AudDebugFd, LX_LOGM_LEVEL_ERROR, DBG_COLOR_RED);			//AUD_KDRV_ERROR
	OS_DEBUG_DisableModuleByIndex(g_AudDebugFd, LX_LOGM_LEVEL_WARNING);						//AUD_KDRV_WARN
	OS_DEBUG_DisableModuleByIndex(g_AudDebugFd, LX_LOGM_LEVEL_NOTI);						//AUD_KDRV_NOTI
	OS_DEBUG_EnableModuleByIndex(g_AudDebugFd, LX_LOGM_LEVEL_INFO, DBG_COLOR_GREEN);		//AUD_KDRV_INFO
	OS_DEBUG_DisableModuleByIndex(g_AudDebugFd, LX_LOGM_LEVEL_DEBUG);						//AUD_KDRV_DEBUG
	OS_DEBUG_DisableModuleByIndex(g_AudDebugFd, LX_LOGM_LEVEL_TRACE);						//AUD_KDRV_TRACE
	OS_DEBUG_DisableModuleByIndex(g_AudDebugFd, LX_LOGM_LEVEL_TRACE_PRINT);					//AUD_KDRV_PRINT
	OS_DEBUG_DisableModuleByIndex(g_AudDebugFd, LX_LOGM_LEVEL_TRACE_ISR_DEBUG);				//AUD_KDRV_ISR_DEBUG
	OS_DEBUG_DisableModuleByIndex(g_AudDebugFd, LX_LOGM_LEVEL_TRACE_MIP_DEBUG);				//AUD_KDRV_MIP_DEBUG
	OS_DEBUG_DisableModuleByIndex(g_AudDebugFd, LX_LOGM_LEVEL_TRACE_DEBUG_TMP);				//AUD_KDRV_DEBUG_TMP
	OS_DEBUG_DisableModuleByIndex(g_AudDebugFd, LX_LOGM_LEVEL_TRACE_ALSA_PRINT);			//AUD_KDRV_ALSA_PRINT
	OS_DEBUG_DisableModuleByIndex(g_AudDebugFd, LX_LOGM_LEVEL_TRACE_ALSA_DEBUG);			//AUD_KDRV_ALSA_DEBUG
	OS_DEBUG_DisableModuleByIndex(g_AudDebugFd, LX_LOGM_LEVEL_TRACE_IPC_DEBUG);				//AUD_KDRV_IPC_DEBUG
	OS_DEBUG_DisableModuleByIndex(g_AudDebugFd, LX_LOGM_LEVEL_TRACE_IPC_MSG0);				//AUD_KDRV_IPC_MSG0
	OS_DEBUG_DisableModuleByIndex(g_AudDebugFd, LX_LOGM_LEVEL_TRACE_IPC_MSG1);				//AUD_KDRV_IPC_MSG1
	OS_DEBUG_DisableModuleByIndex(g_AudDebugFd, LX_LOGM_LEVEL_TRACE_IPC_DBG_ERR);			//AUD_KDRV_IPC_DBG_ERR
	OS_DEBUG_DisableModuleByIndex(g_AudDebugFd, LX_LOGM_LEVEL_TRACE_RM);					//AUD_KDRV_RM
	OS_DEBUG_DisableModuleByIndex(g_AudDebugFd, LX_LOGM_LEVEL_TRACE_RMD);					//AUD_KDRV_RMD
	OS_DEBUG_DisableModuleByIndex(g_AudDebugFd, LX_LOGM_LEVEL_TRACE_MIP_I2C);				//AUD_KDRV_MIP_I2C
	OS_DEBUG_EnableModuleByIndex(g_AudDebugFd, LX_LOGM_LEVEL_TRACE_PRINTF, DBG_COLOR_RED);	//AUD_KDRV_PRINTF

#ifdef KDRV_CONFIG_PM
	// added by SC Jung for quick booting
	if(platform_driver_register(&aud_platform_driver) < 0)
	{
		AUD_KDRV_DEBUG("[%s] platform driver register failed\n", AUDIO_MODULE);

	}
	else
	{
		if(platform_device_register(&aud_platform_device))
		{
			platform_driver_unregister(&aud_platform_driver);
			AUD_KDRV_DEBUG("[%s] platform device register failed\n", AUDIO_MODULE);
		}
		else
		{
			//Enable async suspend and resume.
			device_enable_async_suspend(&((&aud_platform_device)->dev));
			AUD_KDRV_INFO("[%s][ASYNC] platform register done\n", AUDIO_MODULE);
		}
	}
#endif

	g_AudDev = (AUD_DEVICE_T*)OS_KMalloc( sizeof(AUD_DEVICE_T)*AUDIO_DEVICE_MAX );
	if ( g_AudDev == NULL )
	{
		AUD_KDRV_ERROR("out of memory. can't allocate %d bytes\n", sizeof(AUD_DEVICE_T)* AUDIO_DEVICE_MAX );
		return -ENOMEM;
	}

	memset( g_AudDev, 0x0, sizeof(AUD_DEVICE_T) * AUDIO_DEVICE_MAX );

	if (g_AudMajor)
	{
		dev = MKDEV( g_AudMajor, g_AudMinor );
		retVal = register_chrdev_region(dev, AUDIO_DEVICE_MAX, AUDIO_MODULE );
	}
	else
	{
		retVal = alloc_chrdev_region(&dev, g_AudMinor, AUDIO_DEVICE_MAX, AUDIO_MODULE );
		g_AudMajor = MAJOR(dev);
	}

	if ( retVal < 0 )
	{
		AUD_KDRV_ERROR("can't register audio device\n" );
		return -EIO;
	}

	for ( i=0; i<AUDIO_DEVICE_MAX; i++ )
	{
		/* initialize cdev structure with predefined variable */
		dev = MKDEV( g_AudMajor, g_AudMinor+i );
		cdev_init(&(g_AudDev[i].cdev), &g_audio_fops[i]);

		g_AudDev[i].devNo			= dev;
		g_AudDev[i].cdev.owner		= THIS_MODULE;
		g_AudDev[i].cdev.ops		= &g_audio_fops[i];
		g_AudDev[i].allocDev		= LX_AUD_DEV_NONE;

 		retVal = cdev_add (&g_AudDev[i].cdev, dev, 1 );
		if (retVal)
		{
			AUD_KDRV_ERROR("error (%d) while adding audio capture device (%d.%d)\n", retVal, MAJOR(dev), MINOR(dev) );
			return -EIO;
		}

		OS_CreateDeviceClass(dev, "%s", gcAudDevMod[i]);
		sprintf(dev_name, "%s",  gcAudDevMod[i]);

		/* initialize proc system */
		AUDIO_PROC_Init(dev_name);
	}

	//Initialize Audio Decoder(DSP) register base
	/* Check LX Chip Revision Number */
	if(lx_chip_rev() >= LX_CHIP_REV(H15, A0))
	{
		AUD_KDRV_DEBUG_TMP ("LX_CHIP_REV(H15, A0)(0x%X)\n", lx_chip_rev());
		pstAudRegCfg = &g_stAudRegCfg[4];
	}
	else if(lx_chip_rev() >= LX_CHIP_REV(H14, A0))
	{
		AUD_KDRV_DEBUG_TMP ("LX_CHIP_REV(H14, A0)(0x%X)\n", lx_chip_rev());
		pstAudRegCfg = &g_stAudRegCfg[3];
	}
	else if(lx_chip_rev() >= LX_CHIP_REV(M14, B0))
	{
		AUD_KDRV_DEBUG_TMP ("LX_CHIP_REV(M14, B0)(0x%X)\n", lx_chip_rev());
		pstAudRegCfg = &g_stAudRegCfg[2];
	}
	else if(lx_chip_rev() >= LX_CHIP_REV(M14, A0))
	{
		AUD_KDRV_DEBUG_TMP ("LX_CHIP_REV(M14, XX)(0x%X)\n", lx_chip_rev());
		pstAudRegCfg = &g_stAudRegCfg[1];
	}
	else if(lx_chip_rev() >= LX_CHIP_REV(H13, B0))
	{
		AUD_KDRV_DEBUG_TMP ("LX_CHIP_REV(H13, XX)(0x%X)\n", lx_chip_rev());
		pstAudRegCfg = &g_stAudRegCfg[0];
	}
	else
	{
		DBG_PRINT_ERROR("LX_CHIP_REV => Unknown(0x%X) : ERROR\n", lx_chip_rev());
		return -EIO;
	}

	pstMemCfgAud = &g_stMemCfgAud[0];
	AUD_KDRV_DEBUG("reg_base_addr = 0x%X, size = 0x%X\n", pstAudRegCfg->reg_base_addr, pstAudRegCfg->reg_size);

	//Initialize IRQ0 of ADEC DSP0
	retVal = request_irq(pstAudRegCfg->irq0_num, (irq_handler_t)AUDIO_interrupt0, 0, "AUDIO0", &g_AudIrq0Dev);
	if (retVal)
	{
		AUD_KDRV_ERROR("request_irq IRQ_AUD0 in %s is failed %d\n", "ADEC0 ", retVal);
		return -1;
	}
	AUD_KDRV_PRINT("adec irq0 registered :%d:\n", pstAudRegCfg->irq0_num );

	//Initialize IRQ1 of ADEC DSP1
	retVal = request_irq(pstAudRegCfg->irq1_num, (irq_handler_t)AUDIO_interrupt1, 0, "AUDIO1", &g_AudIrq1Dev);
	if (retVal)
	{
		AUD_KDRV_ERROR("request_irq IRQ_AUD1 in %s is failed %d\n", "ADEC1 ", retVal);
		free_irq(pstAudRegCfg->irq0_num, NULL);
		return -1;
	}
	AUD_KDRV_PRINT("adec irq1 registered :%d:\n", pstAudRegCfg->irq1_num);

	//Initializes ALSA Machine driver.
	retVal = ALSA_MACHINE_Init();
	if(retVal != RET_OK)
	{
		return retVal;
	}

	AUD_KDRV_ALSA_PRINT("[%d] %s : ALSA_Init is completed.", __L__, __F__);
	return retVal;
}

void AUDIO_Cleanup(void)
{
	SINT32 	i;

	dev_t 	dev = MKDEV( g_AudMajor, g_AudMinor );
	char 	dev_name[LX_MAX_DEVICE_NAME];

#ifdef KDRV_CONFIG_PM
	// added by SC Jung for quick booting
	platform_driver_unregister(&aud_platform_driver);
	platform_device_unregister(&aud_platform_device);
#endif

	//Uninitializes ALSA Machine driver.
	(void)ALSA_MACHINE_Cleanup();

	/* remove all minor devicies and unregister current device */
	for(i = 0; i < AUDIO_DEVICE_MAX; i++)
	{
		sprintf(dev_name, "%s",  gcAudDevMod[i]);
		AUDIO_PROC_Cleanup(dev_name);
		cdev_del( &(g_AudDev[i].cdev) );
	}

	/* TODO : add your device specific code */

	//Release IRQ0 of AUDIO
	free_irq(pstAudRegCfg->irq0_num, &g_AudIrq0Dev);
	AUD_KDRV_PRINT("free_irq IRQ_AUD0 released :%d:\n", pstAudRegCfg->irq0_num);

	//Release IRQ1 of ADEC
	free_irq(pstAudRegCfg->irq1_num, &g_AudIrq1Dev);
	AUD_KDRV_PRINT("free_irq IRQ_AUD1 released :%d:\n", pstAudRegCfg->irq1_num);

	/* END */

	unregister_chrdev_region(dev, AUDIO_DEVICE_MAX );
	OS_Free( g_AudDev );

	return;
}

static unsigned int KDRV_AUDIO_PollMaster(struct file *filp, poll_table *wait)
{
	UINT8	i;

	AUD_KDRV_PRINT("Audio Poll wait!!!\n");

	poll_wait(filp, &gAudPollWaitQueue, wait);

	//Set a audio GET event type for next event.
	for(i = 0; i < LX_AUD_DEV_MAX; i++)
	{
		if( (gAudGetEvent[i].allocDev != LX_AUD_DEV_NONE)		\
		  &&(gAudGetEvent[i].eventMsg & LX_AUD_EVENT_MASK_ALL) )
		{
		 	return POLLIN;
		}
	}

   	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef KDRV_GLOBAL_LINK
#if defined(CONFIG_LG_BUILTIN_KDRIVER) && defined(CONFIG_LGSNAP)
user_initcall_grp("kdrv",AUDIO_Init);
#else
module_init(AUDIO_Init);
#endif
module_exit(AUDIO_Cleanup);

MODULE_AUTHOR("LGE");
MODULE_DESCRIPTION("base driver");
MODULE_LICENSE("GPL");
#endif

/** @} */


