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
 *  driver interrupt service routine header for sci device. ( used only within kdriver )
 *	this file lists exported function, types for the external modules.
 *
 *  @author		bongrae.cho (bongrae.cho@lge.com)
 *  @version	1.0
 *  @date		2012.02.23
 *
 *  @addtogroup lg1154_sci
 *	@{
 */


#ifndef	_SCI_ISR_H_
#define	_SCI_ISR_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/
#include <linux/interrupt.h>

#include "sci_err.h"

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/


#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

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
	Smartcard Interface IRQ Definitions
*/
typedef enum
{
	SCI_IRQ_FRF		= 0x01,
	SCI_IRQ_FRNE	= 0x02,
	SCI_IRQ_FTF		= 0x04,
	SCI_IRQ_FTE		= 0x08,
	SCI_IRQ_EOR		= 0x10,
	SCI_IRQ_EOT		= 0x20,
	SCI_IRQ_ERR		= 0x40,
	SCI_IRQ_CDPWR 	= 0x80,
	SCI_IRQ_ALL     = 0xFF
} SCI_IRQ_T;


/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
	Global Function Prototype Declaration
----------------------------------------------------------------------------------------*/
irqreturn_t SCI_ISRHandler (int irq, void *dev_id, struct pt_regs *regs);
SCI_RETURN_T SCI_ISRWaitIRQ (SCI_IRQ_T irq, UINT32 timeout);

void SCI_ISRWaitCDIntr(void);


#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _SCI_ISR_H_ */

/** @} */

