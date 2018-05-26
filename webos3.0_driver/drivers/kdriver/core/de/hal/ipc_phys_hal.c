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
 *	hal for physical ipc.
 *
 *	author		
 *	version		1.0
 *	date		
 *	note		
 *
 *	@addtogroup 
 *	@{
 */

/*------------------------------------------------------------------------------
	Control Constants
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
	File Inclusions
------------------------------------------------------------------------------*/
#include "os_util.h"
#include "base_device.h"

#include "de_model.h"
#include "de_kapi.h"
#include "de_ver_def.h"
#include "de_def.h"
#include "de_hal_def.h"

#include "mcu/de_int_def.h"
#include "mcu/de_ipc_def.h"
#include "ipc_phys_hal.h"
#ifdef USE_KDRV_CODES_FOR_L9
#include "l9/de/de_reg_l9.h"
#include "l9/de/de_ipc_l9.h"
#endif
#ifdef USE_KDRV_CODES_FOR_H13
#include "h13/de/de_reg_h13.h"
#include "h13/de/de_ipc_h13.h"
#endif
#ifdef USE_KDRV_CODES_FOR_H14
#include "h14/de/de_reg_h14.h"
#include "h14/de/de_ipc_h14.h"
#endif
#ifdef USE_KDRV_CODES_FOR_M14
#include "m14/de/de_reg_m14.h"
#include "m14/de/de_ipc_m14.h"
#endif
#ifdef USE_KDRV_CODES_FOR_H15
#include "h15/de/de_reg_h15.h"
#include "h15/de/de_ipc_h15.h"
#endif

/*------------------------------------------------------------------------------
	Constant Definitions
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
	Macro Definitions
------------------------------------------------------------------------------*/
#define CHECK_REG_NULL(fp) {if(fp == NULL || g_chip_id < 0)\
							{DE_ERROR("%s is null\n",#fp); \
							return RET_ERROR;}}
#define GET_ID() g_chip_id

/*------------------------------------------------------------------------------
	Type Definitions
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
	External Function Prototype Declarations
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
	External Variables
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
	global Functions  
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
	global Variables
------------------------------------------------------------------------------*/
static int g_chip_id = 0;
ipc_reg_cb_t gp_ipc_reg;

/*------------------------------------------------------------------------------
	Static Function Prototypes Declarations
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
	Static Variables
------------------------------------------------------------------------------*/

/*==============================================================================
	Implementation Group
==============================================================================*/

int ipc_phys_hal_init(void)
{
	int ret = RET_OK;

	memset(&gp_ipc_reg, 0, sizeof(ipc_reg_cb_t));

#ifdef USE_KDRV_CODES_FOR_H15
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H15) ) {
		g_chip_id = 4;
		gp_ipc_reg.get_irq_num           = DE_REG_H15_GetIrqNum;
		gp_ipc_reg.get_ipc_intr_status   = DE_IPC_H15_GetStatusReg;
		gp_ipc_reg.clear_ipc_intr_status = DE_IPC_H15_ClearStatus;
		gp_ipc_reg.clear_wdg_status      = DE_WDG_H15_WakeUpReg;
		gp_ipc_reg.wakeup_ipc            = DE_IPC_H15_WakeUpReg;
		gp_ipc_reg.init_ipc              = DE_IPC_H15_Init;
		gp_ipc_reg.free_ipc              = DE_IPC_H15_Free;
		gp_ipc_reg.check_irq4vsync       = DE_REG_H15_CheckIrq4Vsync;
		gp_ipc_reg.init_interrupt        = DE_REG_H15_InitInterrupt;
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_H14
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H14) ) {
		g_chip_id = 3;
		gp_ipc_reg.get_irq_num           = DE_REG_H14_GetIrqNum;
		gp_ipc_reg.get_ipc_intr_status   = DE_IPC_H14_GetStatusReg;
		gp_ipc_reg.clear_ipc_intr_status = DE_IPC_H14_ClearStatus;
		gp_ipc_reg.clear_wdg_status      = DE_WDG_H14_WakeUpReg;
		gp_ipc_reg.wakeup_ipc            = DE_IPC_H14_WakeUpReg;
		gp_ipc_reg.init_ipc              = DE_IPC_H14_Init;
		gp_ipc_reg.free_ipc              = DE_IPC_H14_Free;
		gp_ipc_reg.check_irq4vsync       = DE_REG_H14_CheckIrq4Vsync;
		gp_ipc_reg.init_interrupt        = DE_REG_H14_InitInterrupt;
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_M14
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_M14) ) {
		g_chip_id = 2;
		gp_ipc_reg.get_irq_num           = DE_REG_M14_GetIrqNum;
		gp_ipc_reg.get_ipc_intr_status   = DE_IPC_M14_GetStatusReg;
		gp_ipc_reg.clear_ipc_intr_status = DE_IPC_M14_ClearStatus;
		gp_ipc_reg.clear_wdg_status      = DE_WDG_M14_WakeUpReg;
		gp_ipc_reg.wakeup_ipc            = DE_IPC_M14_WakeUpReg;
		gp_ipc_reg.init_ipc              = DE_IPC_M14_Init;
		gp_ipc_reg.free_ipc              = DE_IPC_M14_Free;
		gp_ipc_reg.check_irq4vsync       = DE_REG_M14_CheckIrq4Vsync;
		gp_ipc_reg.init_interrupt        = DE_REG_M14_InitInterrupt;
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_H13
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H13) ) {
		g_chip_id = 1;
		gp_ipc_reg.get_irq_num           = DE_REG_H13_GetIrqNum;
		gp_ipc_reg.get_ipc_intr_status   = DE_IPC_H13_GetStatusReg;
		gp_ipc_reg.clear_ipc_intr_status = DE_IPC_H13_ClearStatus;
		gp_ipc_reg.clear_wdg_status      = DE_WDG_H13_WakeUpReg;
		gp_ipc_reg.wakeup_ipc            = DE_IPC_H13_WakeUpReg;
		gp_ipc_reg.init_ipc              = DE_IPC_H13_Init;
		gp_ipc_reg.free_ipc              = DE_IPC_H13_Free;
		gp_ipc_reg.check_irq4vsync       = DE_REG_H13_CheckIrq4Vsync;
		gp_ipc_reg.init_interrupt        = DE_REG_H13_InitInterrupt;
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_L9
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_L9) ) {
		g_chip_id = 0;
		gp_ipc_reg.get_irq_num           = DE_REG_L9_GetIrqNum;
		gp_ipc_reg.get_ipc_intr_status   = DE_IPC_L9_GetStatusReg;
		gp_ipc_reg.clear_ipc_intr_status = DE_IPC_L9_ClearStatus;
		gp_ipc_reg.clear_wdg_status      = DE_WDG_L9_WakeUpReg;
		gp_ipc_reg.wakeup_ipc            = DE_IPC_L9_WakeUpReg;
		gp_ipc_reg.init_ipc              = DE_IPC_L9_Init;
		gp_ipc_reg.free_ipc              = DE_IPC_L9_Free;
		gp_ipc_reg.check_irq4vsync       = DE_REG_L9_CheckIrq4Vsync;
		gp_ipc_reg.init_interrupt        = DE_REG_L9_InitInterrupt;
	} else
#endif
	{
		g_chip_id = -1;
		ret = RET_ERROR;
	}

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief Get interrupt status for IPC
 *
 * @param ipcType [IN] interrupt type for IPC which is happend 
 *                      from either MCU or CPU
 * @param pStatus [OUT] interrupt status for IPC
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int ipc_phys_hal_GetIntrStatus(	UINT32 mcu_id,           \
								VIDEO_IPC_TYPE_T ipcType,\
								UINT32 *pStatus)
{
	int ret = RET_OK;

	CHECK_REG_NULL(gp_ipc_reg.get_ipc_intr_status);
	ret = gp_ipc_reg.get_ipc_intr_status(mcu_id, ipcType, pStatus);

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief Clear Inerrupt status of CPU for IPC
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int ipc_phys_hal_ClearIPCStatus(UINT32 mcu_id, VIDEO_IPC_TYPE_T ipcType, UINT32 *pStatus)
{
	int ret = RET_OK;

	CHECK_REG_NULL(gp_ipc_reg.clear_ipc_intr_status);
	ret = gp_ipc_reg.clear_ipc_intr_status(mcu_id, ipcType, pStatus);

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief Clear Inerrupt status of CPU for watch dog
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int ipc_phys_hal_ClearWDGStatus(UINT32 mcu_id)
{
	int ret = RET_OK;

	CHECK_REG_NULL(gp_ipc_reg.clear_wdg_status);
	ret = gp_ipc_reg.clear_wdg_status(mcu_id, FALSE);

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief make IPC interrupt of MCU be happen
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int ipc_phys_hal_WakeUpIPC(UINT32 mcu_id)
{
	int ret = RET_OK;

	CHECK_REG_NULL(gp_ipc_reg.wakeup_ipc);
	ret = gp_ipc_reg.wakeup_ipc(mcu_id, VIDEO_IPC_MCU, TRUE);

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief initialze IPC
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int ipc_phys_hal_IPCInit(UINT32 mcu_id, UINT32 *pBuffToMCU, UINT32 *pBuffFrMCU)
{
	int ret = RET_OK;

	CHECK_REG_NULL(gp_ipc_reg.init_ipc);
	ret = gp_ipc_reg.init_ipc(mcu_id, pBuffToMCU, pBuffFrMCU);

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief uninitialze IPC
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
BOOLEAN ipc_phys_hal_IPCFree(UINT32 *pBuffToMCU, UINT32 *pBuffFrMCU)
{
	int ret = RET_OK;

	CHECK_REG_NULL(gp_ipc_reg.free_ipc);
	ret = gp_ipc_reg.free_ipc(pBuffToMCU, pBuffFrMCU);

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief get irq number
 *
 * @param fr_rate [IN] value of Frame rate of Display
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int ipc_phys_hal_GetIrqNum(UINT32 mcu_id, UINT32 *ipc_irq_num, UINT32 *sync_irq_num)
{
	int ret = RET_OK;

	CHECK_REG_NULL(gp_ipc_reg.get_irq_num);
	ret = gp_ipc_reg.get_irq_num(mcu_id, ipc_irq_num, sync_irq_num);

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief init interrupt
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int ipc_phys_hal_InitInterrupt(UINT32 mcu_id, BOOLEAN intr_en)
{
	int ret = RET_OK;

	CHECK_REG_NULL(gp_ipc_reg.init_interrupt);
	ret = gp_ipc_reg.init_interrupt(mcu_id, intr_en);

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief Clear Inerrupt status of CPU for DMA
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
BOOLEAN ipc_phys_hal_CheckIrq4Vsync(UINT32 mcu_id)
{
	BOOLEAN vsyncIrq = 0;

	CHECK_REG_NULL(gp_ipc_reg.check_irq4vsync);
	vsyncIrq = gp_ipc_reg.check_irq4vsync(mcu_id);

	return vsyncIrq;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief make AMZ(seamless mode) interrupt of MCU be happen
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int ipc_phys_hal_WakeUpSeamless(UINT32 mcu_id)
{
	int ret = RET_OK;

	CHECK_REG_NULL(gp_ipc_reg.wakeup_ipc);
	ret = gp_ipc_reg.wakeup_ipc(mcu_id, VIDEO_AMZ_MCU, TRUE);

	return ret;
}


