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
 *  register interface implementation for png device of h13. ( used only within kdriver )
 *	png device will teach you how to make device driver with new platform.
 *
 *  @author		Taewoong Kim (taewoong.kim@lge.com)
 *  @version	1.0
 *  @date		2013.11.05
 *
 *  @addtogroup d14
 *	@{
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/
#define LOG_NAME	vcp_reg
#define LOG_LEVEL	log_level_error

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include <linux/io.h>
#include "vdec_base.h"

#include "vcp_reg.h"

#include "log.h"
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
	global Functions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/
volatile UINT32 stpVcpReg = 0;
volatile UINT32 stpHdmiReg = 0;

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
void VCP_Init(void)
{
	log_noti("VCP Init\n");
	stpVcpReg		= (UINT32)ioremap(VDEC_VCP_BASE, 0x1000); // D14 Fix
	stpHdmiReg		= (UINT32)ioremap(VDEC_HDMI_BASE, 0x40000); // D14 Fix
}

void VCP_ClearInterrupt (UINT32 irqstatus)
{
	VCP_CH_VCP_INTR_REG(0) = irqstatus; //== clear intr
}

UINT32 VCP_GetInterruptstatus (void)
{
	unsigned int irqstatus = 0;
	irqstatus = VCP_CH_VCP_INTR_REG(0);
	return irqstatus;
}

void VCP_EnableInterrupt (BOOLEAN bEnable)
{
	if(bEnable)
	{
		VCP_CH_VCP_INTR_MASK(0) = 0xffffffbf; //== intr enable
	}
	else
	{
		VCP_CH_VCP_INTR_MASK(0) = 0xffffffff; //== intr enable
	}
}
