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
 *  interrupt reg driver file for BE device
 *	
 *  author		dj911.kim
 *  version		1.0
 *  date		2013.04.03
 *  note		Additional information.
 *
 *  @addtogroup BE
 *	@{
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include <linux/kernel.h>
#include <asm/uaccess.h>
#include <asm/io.h>

#include "os_util.h"
#include "be_dbg.h"

#include "be_kapi.h"
#include "be_def.h"
#include "ipc_def.h"
#include "int_hal.h"
#include "be_int_hw_m14.h"
#include "../reg/be_reg_dppb_m14.h"

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
extern DPPB_TOP_REG_M14_T gDPPB_TOP_M14;

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/

/*========================================================================================
	Implementation Group
========================================================================================*/
int BE_INT_HW_M14_GetIrqNum(UINT32 *beIrq0, UINT32 *beIrq1)
{
	int ret = RET_OK;

	if (lx_chip_rev() >= LX_CHIP_REV( M14, B0))
	{
		*beIrq0 = BE_INT_HAL_INVALID_IRQ_NUM;
		*beIrq1 = BE_INT_HAL_INVALID_IRQ_NUM;
	}
	else
	{
		*beIrq0 = M14_IRQ_BVE0;
		*beIrq1 = M14_IRQ_BVE1;
	}

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief initialize interrupt register
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int BE_INT_HW_M14_IntrEnable(void)
{
	if (lx_chip_rev() >= LX_CHIP_REV( M14, B0))
	{
		/* Do Nothing */
	}
	else
	{
		DPPB_TOP_M14_RdFL(top_arm_intr_en);
		DPPB_TOP_M14_RdFL(top_rc_intr_en);
		DPPB_TOP_M14_RdFL(top_de_intr_en);

	//	DPPB_TOP_M14_Wr01(top_arm_intr_en, arm_dma_int_en, 1);
		DPPB_TOP_M14_Wr01(top_arm_intr_en, arm_swi_int_en, SWI_TYPE_IPC | SWI_TYPE_ACK);
		DPPB_TOP_M14_Wr(top_rc_intr_en,  0);
		DPPB_TOP_M14_Wr(top_de_intr_en,  0);

		DPPB_TOP_M14_WrFL(top_arm_intr_en);
		DPPB_TOP_M14_WrFL(top_rc_intr_en);
		DPPB_TOP_M14_WrFL(top_de_intr_en);
	}

	return RET_OK;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief get interrupt status
 *
 * @param intType [IN] interrupt type (hw interrupt/sw interrupt)
 * @param pIntrStatus [OUT] status  of hwi/swi
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int BE_INT_HW_M14_GetStatus(BE_INT_TYPE_T intrType, UINT32 *pIntrStatus)
{
	int ret = RET_OK;
	UINT32 intrStatus = 0;

	if (lx_chip_rev() >= LX_CHIP_REV( M14, B0))
	{
		/* Do Nothing */
	}
	else
	{
		DPPB_TOP_M14_RdFL(top_arm_intr_status);
		intrStatus = DPPB_TOP_M14_Rd(top_arm_intr_status);
		if(intrType == INT_HWI)
		{
			*pIntrStatus = 0x000003FF & intrStatus;
		}
		if(intrType == INT_SWI)
		{
			*pIntrStatus = 0x003FFFFFF & (intrStatus >> CPU_INT_SWI);
		}
	}

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief clear interrupt status
 *
 * @param intrType [IN] interrupt type (hw interrupt/sw interrupt)
 * @param pIntrStatus [IN] status  of hwi/swi
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int BE_INT_HW_M14_ClearStatus(BE_INT_TYPE_T intrType, UINT32 *pIntrStatus)
{
	int ret = RET_OK;
	UINT32 intrClear = 0;

	if (lx_chip_rev() >= LX_CHIP_REV( M14, B0))
	{
		/* Do Nothing */
	}
	else
	{
		if(intrType == INT_HWI)
		{
			intrClear = 0x000003FF & *pIntrStatus;
		}
		if(intrType == INT_SWI)
		{
			intrClear = 0xFFFFFC00 & (*pIntrStatus << CPU_INT_SWI);
		}

		DPPB_TOP_M14_RdFL(top_arm_intr_clr);
		DPPB_TOP_M14_Wr(top_arm_intr_clr, intrClear);
		DPPB_TOP_M14_WrFL(top_arm_intr_clr);

		BE_DEBUG("intrClear : 0x%08x\n", intrClear);
	}
	
	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief wakeup interrupt to cpu/mcu
 *
 * @param intrDir [IN] direction of interrupt
 * @param turnIn [IN] turn on/off
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int BE_INT_HW_M14_WakeUpReg(BE_INT_DIR_TYPE_T intrDir, BOOLEAN turnOn)
{
	int ret = RET_OK;

	if (lx_chip_rev() >= LX_CHIP_REV( M14, B0))
	{
		/* Do Nothing */
	}
	else
	{
		turnOn &= 0x1;
		switch (intrDir) {
			case INT_MCU_CPU_IPC :
				if (turnOn) {
					DPPB_TOP_M14_RdFL(top_arm_intr_status);
					DPPB_TOP_M14_Wr01(top_arm_intr_status, arm_swi_int_status, 1);
					DPPB_TOP_M14_WrFL(top_arm_intr_status);
				}
				else
				{
					DPPB_TOP_M14_RdFL(top_arm_intr_clr);
					DPPB_TOP_M14_Wr01(top_arm_intr_clr, arm_swi_int_clear, 1);
					DPPB_TOP_M14_WrFL(top_arm_intr_clr);
				}
				break;
			case INT_MCU_CPU_ACK :
				if (turnOn) {
					DPPB_TOP_M14_RdFL(top_arm_intr_status);
					DPPB_TOP_M14_Wr01(top_arm_intr_status, arm_swi_int_status, 2);
					DPPB_TOP_M14_WrFL(top_arm_intr_status);
				}
				else
				{
					DPPB_TOP_M14_RdFL(top_arm_intr_clr);
					DPPB_TOP_M14_Wr01(top_arm_intr_clr, arm_swi_int_clear, 2);
					DPPB_TOP_M14_WrFL(top_arm_intr_clr);
				}
				break;
			case INT_CPU_MCU_IPC :
				if (turnOn) {
					DPPB_TOP_M14_RdFL(top_rc_intr_status);
					DPPB_TOP_M14_Wr01(top_rc_intr_status, rc_swi_int_status, 1);
					DPPB_TOP_M14_WrFL(top_rc_intr_status);
				}
				else
				{
					DPPB_TOP_M14_RdFL(top_rc_intr_clr);
					DPPB_TOP_M14_Wr01(top_rc_intr_clr, rc_swi_int_clear, 1);
					DPPB_TOP_M14_WrFL(top_rc_intr_clr);
				}
				break;
			case INT_CPU_MCU_ACK :
				if (turnOn) {
					DPPB_TOP_M14_RdFL(top_rc_intr_status);
					DPPB_TOP_M14_Wr01(top_rc_intr_status, rc_swi_int_status, 2);
					DPPB_TOP_M14_WrFL(top_rc_intr_status);
				}
				else
				{
					DPPB_TOP_M14_RdFL(top_rc_intr_clr);
					DPPB_TOP_M14_Wr01(top_rc_intr_clr, rc_swi_int_clear, 2);
					DPPB_TOP_M14_WrFL(top_rc_intr_clr);
				}
				break;
			case INT_DE_MCU_IPC :
				if (turnOn) {
					DPPB_TOP_M14_RdFL(top_de_intr_status);
					DPPB_TOP_M14_Wr01(top_de_intr_status, de_swi_int_status, 1);
					DPPB_TOP_M14_WrFL(top_de_intr_status);
				}
				else
				{
					DPPB_TOP_M14_RdFL(top_de_intr_clr);
					DPPB_TOP_M14_Wr01(top_de_intr_clr, de_swi_int_clear, 1);
					DPPB_TOP_M14_WrFL(top_de_intr_clr);
				}
				break;
			case INT_DE_MCU_ACK :
				if (turnOn) {
					DPPB_TOP_M14_RdFL(top_de_intr_status);
					DPPB_TOP_M14_Wr01(top_de_intr_status, de_swi_int_status, 2);
					DPPB_TOP_M14_WrFL(top_de_intr_status);
				}
				else
				{
					DPPB_TOP_M14_RdFL(top_de_intr_clr);
					DPPB_TOP_M14_Wr01(top_de_intr_clr, de_swi_int_clear, 2);
					DPPB_TOP_M14_WrFL(top_de_intr_clr);
				}
				break;
			default :
				break;
		}
	}
	
	return ret;
}

