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
 * main driver implementation for de device.
 * de device will teach you how to make device driver with new platform.
 *
 * author     sooya.joo (sooya.joo@lge.com)
 * version    0.1
 * date       2011-04-05
 * note       Additional information.
 *
 * @addtogroup h13_vdec
 * @{
 */

/*----------------------------------------------------------------------------------------
 *   Control Constants
 *---------------------------------------------------------------------------------------*/
#define LOG_NAME vdec_lg_hal

/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/
#include "vdec_type_defs.h"
#include "hal/vdec_base.h"
#include "hal/lq_hal_api.h"
#include "hal/pdec_hal_api.h"

#include <linux/kernel.h>

#include "log.h"

/*----------------------------------------------------------------------------------------
 *   Constant Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Macro Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Type Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   External Function Prototype Declarations
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   External Variables
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   global Functions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   global Variables
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Static Function Prototypes Declarations
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Static Variables
 *---------------------------------------------------------------------------------------*/

/**
********************************************************************************
* @brief
*   register setting
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*   stpSdecParam :SDEC parameter
* @return
*  DTV_STATUS_T
********************************************************************************
*/
/*
static int LQC_SelectCh(UINT8 ch)
{
	return 0;
}
*/

void LQ_HAL_Init(void)
{
}

UINT32 LQ_HAL_GetVersion(void)
{
	return 0;
}

void LQ_HAL_Reset(UINT8 ui8ch)
{
	return;
}

/*
	LQ_APB_PRT                      	lq_apb_prt                      ;	// 0x0108 : ''
*/
/*
static void LQ_HAL_SetCmdType(UINT8 ui8ch, LQ_CMD lqcmd)
{
	return;
}

static void LQ_HAL_SetCmdData(UINT8 ui8ch, UINT32 ui32Data)
{
	return;
}

static UINT32 LQ_HAL_GetCmdData(UINT8 ui8ch)
{
	return 0;
}
*/
UINT32 LQ_HAL_GetQLen(UINT8 ui8ch)
{
	return 0;
}

void LQ_HAL_PushData(UINT32 ch, LIPSYNC_QUEUE_T *pLQue)
{
	return;
}

SINT32 LQ_HAL_GetData(UINT32 ch, LIPSYNC_QUEUE_T *pLQue)
{
	return 0;
}

void LQ_HAL_UpdateRdPtr(UINT32 ui8ch)
{
	return;
}

void LQ_HAL_Flush(UINT8 ui8ch)
{
	return;
}


/////////////////////////////////////////////////////////////////////
//	Control Register for LQC0~3
/////////////////////////////////////////////////////////////////////
void LQ_HAL_SetQCConf(UINT8 ui8ch, UINT32 ui32data)
{
	return;
}

void LQC_HAL_Init(UINT8 ui8ch)
{
	return;
}

void LQ_HAL_EnableKeepcount(UINT8 ui8ch)
{
	return;
}

void LQ_HAL_DisableKeepcount(UINT8 ui8ch)
{
	return;
}

void LQ_HAL_SetSTCDiscontinuity(UINT8 ui8ch)
{
	return;
}

void LQ_HAL_EnableUsingGSTC(UINT8 ui8ch)
{
	return;
}

void LQ_HAL_DisableUsingGSTC(UINT8 ui8ch)
{
	return;
}

UINT8 LQ_HAL_GetUsingGSTCStatus(UINT8 ui8ch)
{
	return 0;
}

void LQ_HAL_SetSTCMatchedCondition(UINT8 ui8ch, UINT8 stcMatchedCondition)
{
	return;
}

UINT8 LQ_HAL_GetSTCMatchedCondition(UINT8 ui8ch)
{
	return 0;
}

UINT8 LQ_HAL_GetRunState(UINT8 ui8ch)
{
	return 0;
}

void LQ_HAL_Pause(UINT8 ui8ch)
{
	return;
}

void LQ_HAL_Resume(UINT8 ui8ch)
{
	return;
}

void LQ_HAL_Enable(UINT8 ui8ch)
{
	return;
}

void LQ_HAL_Disable(UINT8 ui8ch)
{
	return;
}

void LQ_HAL_SetBaseAddr(UINT8 ui8ch, UINT16 ui16Addr)
{
	return;
}

void LQ_HAL_SetSize(UINT8 ui8ch, UINT16 ui8Size)
{
	return;
}

void LQ_HAL_SetEmptyTimer(UINT8 ui8ch, UINT32 ui32Time_ms)
{
	return;
}

UINT32 LQ_HAL_GetEmptyTimer(UINT8 ui8ch)
{
	return 0;
}

void LQ_HAL_EnableEmptyTimer(UINT8 ui8ch)
{
	return;
}

void LQ_HAL_DisableEmptyTimer(UINT8 ui8ch)
{
	return;
}

UINT8 LQ_HAL_GetEmptyTimerStatus(UINT8 ui8ch)
{
	return 0;
}

void LQ_HAL_SetWritePtr(UINT8 ui8ch, UINT8 ui8ptr)
{
	return;
}

UINT8 LQ_HAL_GetWritePtr(UINT8 ui8ch)
{
	return 0;
}

void LQ_HAL_SetReadPtr(UINT8 ui8ch, UINT8 ui8ptr)
{
	return;
}

UINT8 LQ_HAL_GetReadPtr(UINT8 ui8ch)
{
	return 0;
}

void LQ_HAL_SetAFullLevel(UINT8 ui8ch, UINT8 ui8ptr)
{
	return;
}

void LQ_HAL_SetAEmptyLevel(UINT8 ui8ch, UINT8 ui8ptr)
{
	return;
}

void LQ_HAL_EnableSTCUpperLimit(UINT8 ui8ch)
{
	return;
}

void LQ_HAL_DisableSTCUpperLimit(UINT8 ui8ch)
{
	return;
}

void LQ_HAL_SetSTCUpperLevel(UINT8 ui8ch, UINT32 ui8STCLvl)
{
	return;
}

UINT32 LQ_HAL_GetSTCUpperLevel(UINT8 ui8ch)
{
	return 0;
}

UINT32 LQ_HAL_GetSTC(UINT8 ui8ch)
{
	return PDEC_HAL_GetStc(ui8ch);
}

UINT32 LQ_HAL_GetGSTCDiffer(UINT8 ui8ch)
{
	return 0;
}

void LQ_HAL_SetTimer(UINT8 ui8ch, UINT32 ui32Time)
{
	return;
}

void LQ_HAL_EnableTimer(UINT8 ui8ch)
{
	return;
}

void LQ_HAL_DisableTimer(UINT8 ui8ch)
{
	return;
}

UINT8 LQ_HAL_GetStatus(UINT8 ui8ch)
{
	return 0;
}

/*
	LQC_SYNC_CNT                   	lqc_sync_cnt                   ;	// 0x0034 : ''
*/

UINT32 LQ_HAL_GetDTS_STC(UINT8 ui8ch)
{
	return 0;
}

UINT32 LQ_HAL_GetDTS_GSTC(UINT8 ui8ch)
{
	return 0;
}

/** @} */

