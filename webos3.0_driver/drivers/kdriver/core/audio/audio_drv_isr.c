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



/** @file	audio_drv_isr.c
 *
 *  main driver implementation for  audio device.
 *  audio device will teach you how to make device driver with new platform.
 *
 *  author	wonchang.shin (wonchang.shin@lge.com)
 *  version	0.1
 *  date	2012.04.24
 *  note	Additional information.
 *
 *  @addtogroup lg1150_audio
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
#include "audio_drv_hal.h"
#include "audio_imc_func.h"


/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
	Function Prototypes Declarations
----------------------------------------------------------------------------------------*/
irqreturn_t AUDIO_interrupt0(int irq, void *dev_id, struct pt_regs *regs);
irqreturn_t AUDIO_interrupt1(int irq, void *dev_id, struct pt_regs *regs);


/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/


/*========================================================================================
	Implementation Group
========================================================================================*/
/**
 * Audio Interrupt 0 definition for Audio DSP0.
 *
 * @see AUDIO_ISR0
*/
irqreturn_t AUDIO_interrupt0(int irq, void *dev_id, struct pt_regs *regs)
{
	//Clear CPU interrupt status for audio dsp0 interrupt.
	(void)AUDIO_WriteReg(LX_AUD_REG_INT0_CLR, 0x1);
	(void)AUDIO_WriteReg(LX_AUD_REG_INT0_CLR, 0x0);

	//Receive IMC message
	AUDIO_IMC_Process();

	//Receive IPC Debug message
	AUDIO_IPC_ProcessDebug();

	return IRQ_HANDLED;
}

/**
 * Audio Interrupt 1 definition for Audio DSP1.
 *
 * @see AUDIO_ISR1
*/
irqreturn_t AUDIO_interrupt1(int irq, void *dev_id, struct pt_regs *regs)
{
	//Clear CPU interrupt status for audio dsp1 interrupt.
	(void)AUDIO_WriteReg(LX_AUD_REG_INT1_CLR, 0x1);
	(void)AUDIO_WriteReg(LX_AUD_REG_INT1_CLR, 0x0);

	//Receive IMC message
	AUDIO_IMC_Process();

	//Receive IPC Debug message
	AUDIO_IPC_ProcessDebug();

	return IRQ_HANDLED;
}

/** @} */



