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
#include "../lg1154_vdec_base.h"
#include "lq_reg.h"
#include "../lq_hal_api.h"

#ifndef __XTENSA__
#include <linux/kernel.h>
#endif

#include "log.h"

/*----------------------------------------------------------------------------------------
 *   Constant Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Macro Definitions
 *---------------------------------------------------------------------------------------*/
#define VDEC_LQC0_BASE					(H13_VDEC_BASE + 0x0C00)
#define VDEC_LQC1_BASE					(H13_VDEC_BASE + 0x0C40)
#define VDEC_LQC2_BASE					(H13_VDEC_BASE + 0x0C80)
#define VDEC_LQC3_BASE					(H13_VDEC_BASE + 0x0CC0)

#define VDEC_LQC_BASE(ch)				(H13_VDEC_BASE + 0x0C00 + (ch*0x40))
#define VDEC_LQ_BASE					(H13_VDEC_BASE + 0x0D00)


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
static volatile UINT32				stpLQC_RegBase = 0;
static volatile LQC_REG_T			*stpLQC_Reg = NULL;
static volatile LQC_REG_T			stpLQC_RegShadow[1];

static volatile LQ_REG_T			*stpLQ_Reg = NULL;
static volatile LQ_REG_T			stpLQ_RegShadow[1];

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
static int LQC_SelectCh(UINT8 ch)
{
	if(ch>=NUM_OF_LQC)
		return -1;

	stpLQC_Reg		= (volatile LQC_REG_T *)(stpLQC_RegBase+ch*0x40);
	return 0;
}

void LQ_HAL_Init(void)
{
	stpLQ_Reg = (volatile LQ_REG_T *) VDEC_TranselateVirualAddr(VDEC_LQ_BASE, 0x100);
	stpLQC_RegBase = (volatile UINT32) VDEC_TranselateVirualAddr(VDEC_LQC0_BASE, 0x100);
}

UINT32 LQ_HAL_GetVersion(void)
{
	LQ_RdFL(lq_ver);
	return LQ_Rd(lq_ver);
}

void LQ_HAL_Reset(UINT8 ui8ch)
{
	LQ_RdFL(lq_reset);

	if(ui8ch&0x80)
	{
		if(ui8ch&0x1)
			LQ_Wr01(lq_reset, reg_lqc0_srst, 0x1);
		if(ui8ch&0x2)
			LQ_Wr01(lq_reset, reg_lqc1_srst, 0x1);
		if(ui8ch&0x4)
			LQ_Wr01(lq_reset, reg_lqc2_srst, 0x1);
		if(ui8ch&0x8)
			LQ_Wr01(lq_reset, reg_lqc3_srst, 0x1);
	}
	else
	{
		switch(ui8ch)
		{
		case 0:
			LQ_Wr01(lq_reset, reg_lqc0_srst, 0x1);
			break;
		case 1:
			LQ_Wr01(lq_reset, reg_lqc1_srst, 0x1);
			break;
		case 2:
			LQ_Wr01(lq_reset, reg_lqc2_srst, 0x1);
			break;
		case 3:
			LQ_Wr01(lq_reset, reg_lqc3_srst, 0x1);
			break;
		case 0xF:
			LQ_Wr04(lq_reset, reg_lqc0_srst, 0x1, reg_lqc1_srst, 0x1, reg_lqc2_srst, 0x1, reg_lqc3_srst, 0x1);
			break;
		default:
			log_error ("[LQ_HAL %d][Err] Unknown Channel , %s", ui8ch, __FUNCTION__ );
			break;
		}
	}
	LQ_WrFL(lq_reset);
}

/*
	LQ_APB_PRT                      	lq_apb_prt                      ;	// 0x0108 : ''
*/

static void LQ_HAL_SetCmdType(UINT8 ui8ch, LQ_CMD lqcmd)
{
	LQC_CMD_TYPE	cmd_type;
	if(ui8ch>=NUM_OF_LQC)
		return;

	cmd_type.reg_lqc_cmd_type = lqcmd;

	LQ_RdFL(lqc_cmd[ui8ch]);
	LQ_Wr01(lqc_cmd[ui8ch], cmd_type, cmd_type);
	LQ_WrFL(lqc_cmd[ui8ch]);
}

static void LQ_HAL_SetCmdData(UINT8 ui8ch, UINT32 ui32Data)
{
	if(ui8ch>=NUM_OF_LQC)
		return;

	LQ_RdFL(lqc_cmd[ui8ch]);
	LQ_Wr01(lqc_cmd[ui8ch], cmd_data, ui32Data);
	LQ_WrFL(lqc_cmd[ui8ch]);
}

static UINT32 LQ_HAL_GetCmdData(UINT8 ui8ch)
{
	UINT32 ui32Data;

	if(ui8ch>=NUM_OF_LQC)
		return -1;

	LQ_RdFL(lqc_cmd[ui8ch]);
	LQ_Rd01(lqc_cmd[ui8ch], cmd_data, ui32Data);

	return ui32Data;
}

UINT32 LQ_HAL_GetQLen(UINT8 ui8ch)
{
	UINT32 n;

	LQ_HAL_SetCmdType(ui8ch, LQCMD_USED_NUM);
	n = LQ_HAL_GetCmdData(ui8ch);

	return n;
}

void LQ_HAL_PushData(UINT32 ch, LIPSYNC_QUEUE_T *pLQue)
{
	UINT32 ui32Cmd = 0x0, ui32Pts = 0x0, ui32Dts=0x0;

	if(ch >= NUM_OF_LQC)
	{
		log_error ("[LQ_HAL %d][Err] Channel Number, %s", ch, __FUNCTION__ );
		return;
	}

	if(pLQue->ui32AuStartAddr & 0x1FF) // check 512 bytes align
		log_error ("[LQ_HAL %d][Err] AU Start Address: 0x%X is not aligned", ch, pLQue->ui32AuStartAddr );
	pLQue->ui32AuStartAddr &= 0xFFFFFE00;
#if 0
	if(pLQue->bFlushBeforeRun & ~0x1)
		log_error ("[LQ_HAL %d][Err] Flush Before Run: 0x%X", pLQue->bFlushBeforeRun );
	pLQue->bFlushBeforeRun &= 0x1;
	if(pLQue->bStccDiscontinuity & ~0x3)
		log_error ("[LQ_HAL %d][Err] stc discontinuity : 0x%X", pLQue->bStccDiscontinuity );
	pLQue->bStccDiscontinuity &= 0x3;
#endif
	if(pLQue->ui32EndAddrCheckMode & ~0x3)
		log_error ("[LQ_HAL %d][Err] End Address Check Mode: 0x%X", ch, pLQue->ui32EndAddrCheckMode );
	pLQue->ui32EndAddrCheckMode &= 0x3;
	if(pLQue->ui32AuType & ~0xF)
		log_error ("[LQ_HAL %d][Err] Au Type: 0x%X", ch, pLQue->ui32AuType );
	pLQue->ui32AuType &= 0xF;

	ui32Cmd = (pLQue->ui32AuStartAddr | (pLQue->ui32EndAddrCheckMode << 4) | pLQue->ui32AuType);

	pLQue->ui32RunDTS &= 0x7FFFFFFF;
	if(pLQue->ui32IgnoreDTSMatching & ~0x1)
		log_error ("[LQ_HAL %d][Err] ui32IgnoreDTSMatching : 0x%X", ch, pLQue->ui32IgnoreDTSMatching );
	pLQue->ui32IgnoreDTSMatching &= 0x1;
	ui32Dts = (pLQue->ui32RunDTS | (pLQue->ui32IgnoreDTSMatching << 29));// | (pLQue->ui32WaitSync0 << 30) | (pLQue->ui32WaitSync1<<31));

	pLQue->ui32PTS &= 0x7FFFFFFF;
	if(pLQue->ui32PTSerr & ~0x1)
		log_error ("[LQ_HAL %d][Err] PTS Error : 0x%X", ch, pLQue->ui32PTSerr );
	pLQue->ui32PTSerr &= 0x1;
	ui32Pts = (pLQue->ui32PTS | (pLQue->ui32PTSerr << 28));

	LQ_HAL_SetCmdData(ch, pLQue->ui32wParam);
	LQ_HAL_SetCmdType(ch, LQCMD_WDATA3);
	LQ_HAL_SetCmdData(ch, ui32Pts);
	LQ_HAL_SetCmdType(ch, LQCMD_WDATA2);
	LQ_HAL_SetCmdData(ch, ui32Dts);
	LQ_HAL_SetCmdType(ch, LQCMD_WDATA1);
	LQ_HAL_SetCmdData(ch, ui32Cmd);
	LQ_HAL_SetCmdType(ch, LQCMD_WDATA0);

	LQ_HAL_SetCmdType(ch, LQCMD_UPDATE_WPTR);
}

SINT32 LQ_HAL_GetData(UINT32 ch, LIPSYNC_QUEUE_T *pLQue)
{
	UINT32 ui32Cmd = 0x0, ui32Pts = 0x0, ui32Dts=0x0;

	if(ch >= NUM_OF_LQC)
	{
		log_error ("[LQ_HAL %d][Err] Channel Number, %s", ch, __FUNCTION__ );
		return -1;
	}

	LQ_HAL_SetCmdType(ch, LQCMD_RDATA3);
	pLQue->ui32wParam = LQ_HAL_GetCmdData(ch);

	LQ_HAL_SetCmdType(ch, LQCMD_RDATA2);
	ui32Pts = LQ_HAL_GetCmdData(ch);
	pLQue->ui32PTS = ui32Pts & 0x7FFFFFFF;
	pLQue->ui32PTSerr = (ui32Pts>>28)&0x1;

	LQ_HAL_SetCmdType(ch, LQCMD_RDATA1);
	ui32Dts = LQ_HAL_GetCmdData(ch);
	pLQue->ui32RunDTS = ui32Dts & 0x7FFFFFFF;
	pLQue->ui32IgnoreDTSMatching = (ui32Dts>>29) & 0x1;

	LQ_HAL_SetCmdType(ch, LQCMD_RDATA0);
	ui32Cmd = LQ_HAL_GetCmdData(ch);
	pLQue->ui32AuStartAddr = ui32Cmd&(~0x1FF);
//	pLQue->bFlushBeforeRun = (ui32Cmd>>8)&0x1;
//	pLQue->bStccDiscontinuity = (ui32Cmd>>6)&0x3;
	pLQue->ui32EndAddrCheckMode = (ui32Cmd>>4)&0x3;
	pLQue->ui32AuType = ui32Cmd&0xF;

	return 0;
}

void LQ_HAL_UpdateRdPtr(UINT32 ui8ch)
{
	UINT32 u32LQ_Wptr, u32LQ_Rptr;
	UINT32 u32Len=0;

	LQ_HAL_SetCmdType(ui8ch, LQCMD_CUR_WPTR);
	u32LQ_Wptr = LQ_HAL_GetCmdData(ui8ch);
	LQ_HAL_SetCmdType(ui8ch, LQCMD_CUR_RPTR);
	u32LQ_Rptr = LQ_HAL_GetCmdData(ui8ch);

	if( u32LQ_Wptr == u32LQ_Rptr )
		return;

	u32Len = LQ_HAL_GetQLen(ui8ch);
	if( u32Len == 0 )
		return;

	LQ_HAL_SetCmdType(ui8ch, LQCMD_UPDATE_RPTR);	// Update LQ Rptr
}

void LQ_HAL_Flush(UINT8 ui8ch)
{
	UINT32 u32LQ_Wptr, u32LQ_Rptr;
	UINT32 u32Len=0;

	while(1)
	{
		LQ_HAL_SetCmdType(ui8ch, LQCMD_CUR_WPTR);
		u32LQ_Wptr = LQ_HAL_GetCmdData(ui8ch);
		LQ_HAL_SetCmdType(ui8ch, LQCMD_CUR_RPTR);
		u32LQ_Rptr = LQ_HAL_GetCmdData(ui8ch);

		if( u32LQ_Wptr == u32LQ_Rptr )
			break;

		u32Len = LQ_HAL_GetQLen(ui8ch);
		if( u32Len == 0 )
			break;

		LQ_HAL_SetCmdType(ui8ch, LQCMD_UPDATE_RPTR);	// Update LQ Rptr
	}
}


/////////////////////////////////////////////////////////////////////
//	Control Register for LQC0~3
/////////////////////////////////////////////////////////////////////
void LQ_HAL_SetQCConf(UINT8 ui8ch, UINT32 ui32data)
{
	if(LQC_SelectCh(ui8ch)!=0)
		return;

	LQC_RdFL(lqc_conf);
	LQC_Wr(lqc_conf, ui32data);
}

void LQC_HAL_Init(UINT8 ui8ch)
{
	if(LQC_SelectCh(ui8ch)!=0)
		return;
	LQC_RdFL(lqc_conf);
	LQC_Wr01(lqc_conf, reg_lqc_channel, ui8ch);
	LQC_Wr01(lqc_conf, reg_lqc_lq_keep_count, 0);	// if wptr-rptr > keepcnt , occures interrupt.
	LQC_Wr01(lqc_conf, reg_lqc_auto_end_address, 0);		// using next aui offset for end address
	LQC_Wr01(lqc_conf, reg_lqc_stcc_discontinuity, 0);		// make disable stc matched condition all of AUI in the LQ Buffer
	LQC_Wr01(lqc_conf, reg_lqc_lq_vsync_en, 0);			// for debugging, please don't set this field
	LQC_Wr01(lqc_conf, reg_lqc_de_vsync_en, 1);			// exclusive with lq_vsync_en, always set this field.
	LQC_Wr01(lqc_conf, reg_lqc_use_global_stc, 0);		// using global STC for file play
	LQC_Wr01(lqc_conf, reg_lqc_wait_sync1, 0);
	LQC_Wr01(lqc_conf, reg_lqc_wait_sync0, 0);
	LQC_Wr01(lqc_conf, reg_lqc_lq_discard, 0);			// don't set this field
	LQC_Wr01(lqc_conf, reg_lqc_lq_skip, 0);				// don't set this field
	LQC_Wr01(lqc_conf, reg_lqc_wait_wptr_dis, 1);
	LQC_Wr01(lqc_conf, reg_lqc_ignore_stc_dts_matching, 0);
	LQC_Wr01(lqc_conf, reg_lqc_lq_delay_dis, 1);		// disable keep count
	LQC_Wr01(lqc_conf, reg_lqc_auto_update_rptr_dis, 1);
	LQC_Wr01(lqc_conf, reg_lqc_no_wait_intr_ack, 0);
	LQC_Wr01(lqc_conf, reg_lqc_pause, 0);
	LQC_Wr01(lqc_conf, reg_lqc_stop, 0);
	LQC_Wr01(lqc_conf, reg_lqc_enable, 0);
	LQC_WrFL(lqc_conf);
}

void LQ_HAL_EnableKeepcount(UINT8 ui8ch)
{
	if(LQC_SelectCh(ui8ch)!=0)
		return;

	LQC_RdFL(lqc_conf);
	LQC_Wr01(lqc_conf, reg_lqc_lq_delay_dis, 0);		// disable keep count
	LQC_WrFL(lqc_conf);
}

void LQ_HAL_DisableKeepcount(UINT8 ui8ch)
{
	if(LQC_SelectCh(ui8ch)!=0)
		return;

	LQC_RdFL(lqc_conf);
	LQC_Wr01(lqc_conf, reg_lqc_lq_delay_dis, 1);		// disable keep count
	LQC_WrFL(lqc_conf);
}

void LQ_HAL_SetSTCDiscontinuity(UINT8 ui8ch)
{
	if(LQC_SelectCh(ui8ch)!=0)
		return;

	LQC_RdFL(lqc_conf);
	LQC_Wr01(lqc_conf, reg_lqc_stcc_discontinuity, 1);		// Pusshed LQ Data will be run Free
	LQC_WrFL(lqc_conf);
}

void LQ_HAL_EnableUsingGSTC(UINT8 ui8ch)
{
	if(LQC_SelectCh(ui8ch)!=0)
		return;

	LQC_RdFL(lqc_conf);
	LQC_Wr01(lqc_conf, reg_lqc_use_global_stc, 1);
	LQC_WrFL(lqc_conf);
}

void LQ_HAL_DisableUsingGSTC(UINT8 ui8ch)
{
	if(LQC_SelectCh(ui8ch)!=0)
		return;

	LQC_RdFL(lqc_conf);
	LQC_Wr01(lqc_conf, reg_lqc_use_global_stc, 0);
	LQC_WrFL(lqc_conf);
}

UINT8 LQ_HAL_GetUsingGSTCStatus(UINT8 ui8ch)
{
	UINT8 use_gstc_status;
	if(LQC_SelectCh(ui8ch)!=0)
		return -1;

	LQC_RdFL(lqc_conf);
	LQC_Rd01(lqc_conf, reg_lqc_use_global_stc, use_gstc_status);

	return (use_gstc_status&0x1);
}

void LQ_HAL_SetSTCMatchedCondition(UINT8 ui8ch, UINT8 stcMatchedCondition)
{
	if(LQC_SelectCh(ui8ch)!=0)
		return;

	LQC_RdFL(lqc_conf);
	LQC_Wr01(lqc_conf, reg_lqc_ignore_stc_dts_matching, stcMatchedCondition&1);
	LQC_WrFL(lqc_conf);
}

UINT8 LQ_HAL_GetSTCMatchedCondition(UINT8 ui8ch)
{
	UINT8 stcMatchedCondition = 0;
	if(LQC_SelectCh(ui8ch)!=0)
		return -1;

	LQC_RdFL(lqc_conf);
	LQC_Rd01(lqc_conf, reg_lqc_ignore_stc_dts_matching, stcMatchedCondition);

	return stcMatchedCondition;
}

UINT8 LQ_HAL_GetRunState(UINT8 ui8ch)
{
	UINT8 ui8Stop=0, ui8Enable=0, ui8Res = 0;
	if(LQC_SelectCh(ui8ch)!=0)
		return ui8Res;

	LQC_RdFL(lqc_conf);
	LQC_Rd01(lqc_conf, reg_lqc_enable, ui8Enable);
	LQC_Rd01(lqc_conf, reg_lqc_stop, ui8Stop);

	if((ui8Enable==1)&&(ui8Stop==0))
		ui8Res = 1;
	else
		ui8Res = 0;

	return ui8Res;
}

void LQ_HAL_Pause(UINT8 ui8ch)
{
	if(LQC_SelectCh(ui8ch)!=0)
		return;

	LQC_RdFL(lqc_conf);
	LQC_Wr01(lqc_conf, reg_lqc_stop, 0x1);
	LQC_WrFL(lqc_conf);
}

void LQ_HAL_Resume(UINT8 ui8ch)
{
	if(LQC_SelectCh(ui8ch)!=0)
		return;

	LQC_RdFL(lqc_conf);
	LQC_Wr01(lqc_conf, reg_lqc_stop, 0x0);
	LQC_WrFL(lqc_conf);
}

void LQ_HAL_Enable(UINT8 ui8ch)
{
	if(LQC_SelectCh(ui8ch)!=0)
		return;

	LQC_RdFL(lqc_conf);
	LQC_Wr01(lqc_conf, reg_lqc_enable, 0x1);
	LQC_WrFL(lqc_conf);
}

void LQ_HAL_Disable(UINT8 ui8ch)
{
	if(LQC_SelectCh(ui8ch)!=0)
		return;

	LQC_RdFL(lqc_conf);
	LQC_Wr01(lqc_conf, reg_lqc_enable, 0x0);
	LQC_WrFL(lqc_conf);
}

void LQ_HAL_SetBaseAddr(UINT8 ui8ch, UINT16 ui16Addr)
{
	if(LQC_SelectCh(ui8ch)!=0)
		return;

	LQC_RdFL(lqc_lq_conf);
	LQC_Wr01(lqc_lq_conf, reg_lqc_lq_base, ui16Addr&0xFFF);
	LQC_WrFL(lqc_lq_conf);
}

void LQ_HAL_SetSize(UINT8 ui8ch, UINT16 ui8Size)
{
	if(LQC_SelectCh(ui8ch)!=0)
		return;

	LQC_RdFL(lqc_lq_conf);
	LQC_Wr01(lqc_lq_conf, reg_lqc_lq_size, ui8Size);
	LQC_WrFL(lqc_lq_conf);
}

void LQ_HAL_SetEmptyTimer(UINT8 ui8ch, UINT32 ui32Time_ms)
{
	UINT32	ui32Time_Cnt;

	if(LQC_SelectCh(ui8ch)!=0)
		return;

	ui32Time_Cnt = ui32Time_ms * 200000;
	if( ui32Time_Cnt > 0xFFFFFF )
	{
		log_error ("[LQ_HAL %d][Err] Big Timer Value: %d , %s", ui8ch, ui32Time_ms, __FUNCTION__ );
	}

	LQC_RdFL(lqc_lq_aempty_time);
	LQC_Wr01(lqc_lq_aempty_time, reg_lqc_lq_aempty_timer, ui32Time_Cnt&0xFFFFFF);
	LQC_WrFL(lqc_lq_aempty_time);
}

UINT32 LQ_HAL_GetEmptyTimer(UINT8 ui8ch)
{
	UINT32	ui32Time_Cnt, ui32Time_ms;

	if(LQC_SelectCh(ui8ch)!=0)
		return 0;

	LQC_RdFL(lqc_lq_aempty_time);
	LQC_Rd01(lqc_lq_aempty_time, reg_lqc_lq_aempty_timer, ui32Time_Cnt);

	ui32Time_ms = (UINT32) ui32Time_Cnt/200000;

	return ui32Time_ms;
}

void LQ_HAL_EnableEmptyTimer(UINT8 ui8ch)
{
	if(LQC_SelectCh(ui8ch)!=0)
		return;

	LQC_RdFL(lqc_lq_aempty_time);
	LQC_Wr01(lqc_lq_aempty_time, reg_lqc_lq_aempty_timer_en, 1);
	LQC_WrFL(lqc_lq_aempty_time);
}

void LQ_HAL_DisableEmptyTimer(UINT8 ui8ch)
{
	if(LQC_SelectCh(ui8ch)!=0)
		return;

	LQC_RdFL(lqc_lq_aempty_time);
	LQC_Wr01(lqc_lq_aempty_time, reg_lqc_lq_aempty_timer_en, 0);
	LQC_WrFL(lqc_lq_aempty_time);
}

UINT8 LQ_HAL_GetEmptyTimerStatus(UINT8 ui8ch)
{
	UINT8 bEnable;
	if(LQC_SelectCh(ui8ch)!=0)
		return 0xFF;

	LQC_RdFL(lqc_lq_aempty_time);
	LQC_Rd01(lqc_lq_aempty_time, reg_lqc_lq_aempty_timer_en, bEnable);

	return bEnable;
}

void LQ_HAL_SetWritePtr(UINT8 ui8ch, UINT8 ui8ptr)
{
	if(LQC_SelectCh(ui8ch)!=0)
		return;

	LQC_RdFL(lqc_lq_wptr);
	LQC_Wr01(lqc_lq_wptr, reg_lqc_lq_wptr, ui8ptr);
	LQC_WrFL(lqc_lq_wptr);
}

UINT8 LQ_HAL_GetWritePtr(UINT8 ui8ch)
{
	UINT8 ui8ptr;
	if(LQC_SelectCh(ui8ch)!=0)
		return -1;

	LQC_RdFL(lqc_lq_wptr);
	LQC_Rd01(lqc_lq_wptr, reg_lqc_lq_wptr, ui8ptr);

	return ui8ptr;
}

void LQ_HAL_SetReadPtr(UINT8 ui8ch, UINT8 ui8ptr)
{
	if(LQC_SelectCh(ui8ch)!=0)
		return;

	LQC_RdFL(lqc_lq_rptr);
	LQC_Wr01(lqc_lq_rptr, reg_lqc_lq_rptr, ui8ptr);
	LQC_WrFL(lqc_lq_rptr);
}

UINT8 LQ_HAL_GetReadPtr(UINT8 ui8ch)
{
	UINT8 ui8ptr;
	if(LQC_SelectCh(ui8ch)!=0)
		return -1;

	LQC_RdFL(lqc_lq_rptr);
	LQC_Rd01(lqc_lq_rptr, reg_lqc_lq_rptr, ui8ptr);

	return ui8ptr;
}

void LQ_HAL_SetAFullLevel(UINT8 ui8ch, UINT8 ui8ptr)
{
	if(LQC_SelectCh(ui8ch)!=0)
		return;

	LQC_RdFL(lqc_lq_afull_lev);
	LQC_Wr01(lqc_lq_afull_lev, reg_lqc_lq_afull_lev, ui8ptr);
	LQC_WrFL(lqc_lq_afull_lev);
}

void LQ_HAL_SetAEmptyLevel(UINT8 ui8ch, UINT8 ui8ptr)
{
	if(LQC_SelectCh(ui8ch)!=0)
		return;

	LQC_RdFL(lqc_lq_aempty_lev);
	LQC_Wr01(lqc_lq_aempty_lev, reg_lqc_lq_aempty_lev, ui8ptr);
	LQC_WrFL(lqc_lq_aempty_lev);
}

void LQ_HAL_EnableSTCUpperLimit(UINT8 ui8ch)
{
	if(LQC_SelectCh(ui8ch)!=0)
		return;

	LQC_RdFL(lqc_stc_upper);
	LQC_Wr01(lqc_stc_upper, reg_lqc_stc_upper_limit_en, 1);
	LQC_WrFL(lqc_stc_upper);
}

void LQ_HAL_DisableSTCUpperLimit(UINT8 ui8ch)
{
	if(LQC_SelectCh(ui8ch)!=0)
		return;

	LQC_RdFL(lqc_stc_upper);
	LQC_Wr01(lqc_stc_upper, reg_lqc_stc_upper_limit_en, 0);
	LQC_WrFL(lqc_stc_upper);
}

void LQ_HAL_SetSTCUpperLevel(UINT8 ui8ch, UINT32 ui8STCLvl)
{
	if(LQC_SelectCh(ui8ch)!=0)
		return;

	LQC_RdFL(lqc_stc_upper);
	LQC_Wr01(lqc_stc_upper, reg_lqc_stc_ignore_upper, (ui8STCLvl&0x7FFFFFFF));
	LQC_WrFL(lqc_stc_upper);
}

UINT32 LQ_HAL_GetSTCUpperLevel(UINT8 ui8ch)
{
	UINT32 ui8STCLvl;
	if(LQC_SelectCh(ui8ch)!=0)
		return 0;

	LQC_RdFL(lqc_stc_upper);
	LQC_Rd01(lqc_stc_upper, reg_lqc_stc_ignore_upper, ui8STCLvl);

	return ui8STCLvl;
}

UINT32 LQ_HAL_GetSTC(UINT8 ui8ch)
{
	UINT32 ui32stc;
	if(LQC_SelectCh(ui8ch)!=0)
		return -1;

	LQC_RdFL(lqc_stc);
	LQC_Rd01(lqc_stc, reg_lqc_stc, ui32stc);

	return ui32stc;
}

UINT32 LQ_HAL_GetGSTCDiffer(UINT8 ui8ch)
{
	UINT32 ui32stc;
	if(LQC_SelectCh(ui8ch)!=0)
		return -1;

	LQC_RdFL(lqc_stc_gstc);
	LQC_Rd01(lqc_stc_gstc, reg_lqc_stc_gstcc_diff, ui32stc);

	return ui32stc;
}

void LQ_HAL_SetTimer(UINT8 ui8ch, UINT32 ui32Time)
{
	if(LQC_SelectCh(ui8ch)!=0)
		return;

	LQC_RdFL(lqc_intr_timer);
	LQC_Wr01(lqc_intr_timer, reg_lqc_intr_timer, ui32Time&0xFFFFFF);
	LQC_WrFL(lqc_intr_timer);
}

void LQ_HAL_EnableTimer(UINT8 ui8ch)
{
	if(LQC_SelectCh(ui8ch)!=0)
		return;

	LQC_RdFL(lqc_intr_timer);
	LQC_Wr01(lqc_intr_timer, reg_lqc_intr_timer_en, 1);
	LQC_WrFL(lqc_intr_timer);
}

void LQ_HAL_DisableTimer(UINT8 ui8ch)
{
	if(LQC_SelectCh(ui8ch)!=0)
		return;

	LQC_RdFL(lqc_intr_timer);
	LQC_Wr01(lqc_intr_timer, reg_lqc_intr_timer_en, 0);
	LQC_WrFL(lqc_intr_timer);
}

UINT8 LQ_HAL_GetStatus(UINT8 ui8ch)
{
	UINT8 ui8Status;
	if(LQC_SelectCh(ui8ch)!=0)
		return -1;

	LQC_RdFL(lqc_status);
	LQC_Rd01(lqc_status, reg_lqc_lq_st, ui8Status);

	return ui8Status;
}

/*
	LQC_SYNC_CNT                   	lqc_sync_cnt                   ;	// 0x0034 : ''
*/

UINT32 LQ_HAL_GetDTS_STC(UINT8 ui8ch)
{
	UINT32 ui32diff;
	if(LQC_SelectCh(ui8ch)!=0)
		return -1;

	LQC_RdFL(lqc_dts_stc);
	LQC_Rd01(lqc_dts_stc, reg_lqc_dts_stc_diff, ui32diff);

	return ui32diff;
}

UINT32 LQ_HAL_GetDTS_GSTC(UINT8 ui8ch)
{
	UINT32 ui32diff;
	if(LQC_SelectCh(ui8ch)!=0)
		return -1;

	LQC_RdFL(lqc_dts_gstc);
	LQC_Rd01(lqc_dts_gstc, reg_lqc_dts_gstc_diff, ui32diff);

	return ui32diff;
}

/** @} */

