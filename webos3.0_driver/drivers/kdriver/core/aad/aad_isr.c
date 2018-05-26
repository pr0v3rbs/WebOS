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
 *  AAD ISR releated functions.
 *	aad device will teach you how to make device driver with new platform.
 *
 *  author		Sudhaher (kanthiraj.s@lge.com)
 *  version		1.0
 *  date		2010.03.10
 *  note		Additional information.
 *
 *  @addtogroup lg1150_aad
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
#include <linux/fs.h>
#include <linux/interrupt.h>    /* For isr */
#include <asm/irq.h>			/* For isr */
#include "proc_util.h"
#include "debug_util.h"
#include "aad_coredrv.h"
#include "aad_drv.h"

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
DECLARE_WAIT_QUEUE_HEAD(AAD_ASD_Task_wait);
int AAD_ASD_Task_waitq_flag;


/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/
irqreturn_t aad_interrupt(int irq, void *dev_id, struct pt_regs *regs);


/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/

/*========================================================================================
	Implementation Group
========================================================================================*/

/**
 * AAD Interrupt handler.
*/
irqreturn_t aad_interrupt(int irq, void *dev_id, struct pt_regs *regs)
{
	UINT8	ui8AsdComplete;

	//Disable AAD interrupt(status change output disable)
	AAD_RdFL(status_pin_ctrl);
	AAD_Wr01(status_pin_ctrl, staten, 0);
	AAD_WrFL(status_pin_ctrl);

	//Read status
	AAD_RdFL(status);
	AAD_Rd01(status, asdcomplete, ui8AsdComplete);
	if(ui8AsdComplete == 1)
	{
		AAD_ISR_DEBUG ("ASD completed\n");
		AAD_ASD_WakeUp_Interruptible();
	}

	AAD_ISR_DEBUG("#AAD_ISR(status = 0x%X)#\n", AAD_Rd(status));
	return IRQ_HANDLED;
}
/** @} */


