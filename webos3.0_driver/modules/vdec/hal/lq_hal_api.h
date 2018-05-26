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
 * author     youngki.lyu@lge.com
 * version    0.1
 * date       2011.02.23
 * note       Additional information.
 *
 */

#ifndef _LQ_HAL_API_h
#define _LQ_HAL_API_h

#include "vdec_type_defs.h"
#include "vdec_base.h"

#ifdef __cplusplus
extern "C" {
#endif

#define NUM_OF_LQC					LQC_NUM_OF_CHANNEL
#define NUM_OF_LQC_SIZE				40

enum
{
	LQ_S_IDLE                  = 0x0,
	LQ_S_READ_CUR_ADDRESS      = 0x1,
	LQ_S_READ_CUR_ADDRESS_END  = 0x2,
	LQ_S_READ_DTS              = 0x3,
	LQ_S_READ_DTS_END          = 0x4,
	LQ_S_READ_RESERVED1        = 0x5,
	LQ_S_READ_RESERVED1_END    = 0x6,
	LQ_S_READ_RESERVED2        = 0x7,
	LQ_S_READ_RESERVED2_END    = 0x8,
	LQ_S_READ_NEXT_ADDRESS     = 0x9,
	LQ_S_READ_NEXT_ADDRESS_END = 0xA,
	LQ_S_WRITE_RESERVED2       = 0xB,
	LQ_S_WAIT_STC              = 0xC,
	LQ_S_WAIT_SYNC0            = 0xD,
	LQ_S_WAIT_SYNC1            = 0xE,
	LQ_S_WAIT_WPTR             = 0xF,
	LQ_S_MATCHED               = 0x10,
	LQ_S_GEN_INTR              = 0x11,
	LQ_S_WAIT_INTR_ACK         = 0x12,
	LQ_S_UPDATE_RD_PTR         = 0x13,
	LQ_S_SET_TIMER             = 0x14,
	LQ_S_WAIT_TIMER            = 0x15,
};

typedef enum
{
	LQCMD_WDATA0			= 0x00,
	LQCMD_WDATA1			= 0x01,
	LQCMD_WDATA2			= 0x02,
	LQCMD_WDATA3			= 0x03,
	LQCMD_UPDATE_WPTR	= 0x04,
	LQCMD_UPDATE_RPTR	= 0x05,
	LQCMD_EMPTY_NUM		= 0x06,
	LQCMD_USED_NUM		= 0x07,
	LQCMD_RESET			= 0x08,
	LQCMD_CUR_WPTR		= 0x09,
	LQCMD_CUR_RPTR	 	= 0x0A,
	LQCMD_ONE_SKIP		= 0x0B,
	LQCMD_RDATA0			= 0x0C,
	LQCMD_RDATA1			= 0x0D,
	LQCMD_RDATA2			= 0x0E,
	LQCMD_RDATA3			= 0x0F,
	LQCMD_DISCARD			= 0x10,
	LQCMD_BACK_WPTR		= 0x11,
	LQCMD_BACK_RPTR		= 0x12
} LQ_CMD;

typedef struct{
	/*	COMMAND 0
		[3:0]   au_type
		[5:4]   end_address_check_mode
		[8]      flush_before_run
		[7]      stc_discontinuity
		[31:9] au_start_address
	*/
	UINT32		ui32AuStartAddr;			// 512bytes aligned size
//	UINT32		bFlushBeforeRun;
//	UINT32		bStccDiscontinuity;

	// 00 end=next_start+1024, 01 end=next_start+512
	// 02 end=next_start , 11 ignore write pointer
	UINT32		ui32EndAddrCheckMode;	// AUI_INFO1 - 5:4

	// SEQ_INIT(1), SEQ_END(2), I_PIC(3), P_PIC(4), NOSKIP_B_PIC(5), SKIP_B_PIC(6)
	// SET_FRAME_BUFFER(8)  /  DEC_FLUSH(9)  /  VPU_WAKE_UP(11)
	UINT32		ui32AuType;

	/*	COMMAND 1
		[27:0] dts
		[29]    ignore STC DTS Matching
		[30]    wait sync0 :: default 0
		[31]    wait sync1 :: default 0
	*/
	UINT32		ui32IgnoreDTSMatching;
	UINT32		ui32RunDTS;			// Running Time

	/*	COMMAND 2
		[27:0] pts
		[28]    pts error
	*/
	UINT32		ui32PTSerr;
	UINT32		ui32PTS;

	/*	COMMAND 3
		[31:0] AUI Next Address
	*/
	UINT32		ui32wParam;
}LIPSYNC_QUEUE_T;

typedef struct{
	UINT32		ui32AuRealStartAddr;		// not aligned size
	UINT32		ui32AuRealEndAddr;		// not aligned size
	BOOLEAN		bRingBufferMode;
	UINT64		ui64TimeStamp;
} LIPSYNC_AUI_T;

void LQ_HAL_Init(void);
UINT32 LQ_HAL_GetVersion(void);
void LQ_HAL_Reset(UINT8 ui8ch);
UINT32 LQ_HAL_GetQLen(UINT8 ui8ch);
void LQ_HAL_PushData(UINT32 ch, LIPSYNC_QUEUE_T *pLQue);
SINT32 LQ_HAL_GetData(UINT32 ch, LIPSYNC_QUEUE_T *pLQue);
void LQ_HAL_UpdateRdPtr(UINT32 ch);
void LQ_HAL_Flush(UINT8 ui8ch);

/////////////////////////////////////////////////////////////////////
//	Control Register for LQC0~3
/////////////////////////////////////////////////////////////////////
void LQ_HAL_SetQCConf(UINT8 ui8ch, UINT32 ui32data);
void LQC_HAL_Init(UINT8 ui8ch);
void LQ_HAL_EnableKeepcount(UINT8 ui8ch);
void LQ_HAL_DisableKeepcount(UINT8 ui8ch);
void LQ_HAL_SetSTCDiscontinuity(UINT8 ui8ch);
void LQ_HAL_EnableUsingGSTC(UINT8 ui8ch);
void LQ_HAL_DisableUsingGSTC(UINT8 ui8ch);
UINT8 LQ_HAL_GetUsingGSTCStatus(UINT8 ui8ch);
void LQ_HAL_SetSTCMatchedCondition(UINT8 ui8ch, UINT8 stcMatchedCondition);
UINT8 LQ_HAL_GetSTCMatchedCondition(UINT8 ui8ch);
void LQ_HAL_Pause(UINT8 ui8ch);
UINT8 LQ_HAL_GetRunState(UINT8 ui8ch);
void LQ_HAL_Resume(UINT8 ui8ch);
void LQ_HAL_Enable(UINT8 ui8ch);
void LQ_HAL_Disable(UINT8 ui8ch);
void LQ_HAL_SetBaseAddr(UINT8 ui8ch, UINT16 ui16Addr);
void LQ_HAL_SetSize(UINT8 ui8ch, UINT16 ui8Size);
void LQ_HAL_SetEmptyTimer(UINT8 ui8ch, UINT32 ui32Time_ms);
UINT32 LQ_HAL_GetEmptyTimer(UINT8 ui8ch);
void LQ_HAL_EnableEmptyTimer(UINT8 ui8ch);
void LQ_HAL_DisableEmptyTimer(UINT8 ui8ch);
UINT8 LQ_HAL_GetEmptyTimerStatus(UINT8 ui8ch);
void LQ_HAL_SetWritePtr(UINT8 ui8ch, UINT8 ui8ptr);
UINT8 LQ_HAL_GetWritePtr(UINT8 ui8ch);
void LQ_HAL_SetReadPtr(UINT8 ui8ch, UINT8 ui8ptr);
UINT8 LQ_HAL_GetReadPtr(UINT8 ui8ch);
void LQ_HAL_SetAFullLevel(UINT8 ui8ch, UINT8 ui8ptr);
void LQ_HAL_SetAEmptyLevel(UINT8 ui8ch, UINT8 ui8ptr);
void LQ_HAL_EnableSTCUpperLimit(UINT8 ui8ch);
void LQ_HAL_DisableSTCUpperLimit(UINT8 ui8ch);
void LQ_HAL_SetSTCUpperLevel(UINT8 ui8ch, UINT32 ui8STCLvl);
UINT32 LQ_HAL_GetSTCUpperLevel(UINT8 ui8ch);
UINT32 LQ_HAL_GetSTC(UINT8 ui8ch);
UINT32 LQ_HAL_GetGSTCDiffer(UINT8 ui8ch);
void LQ_HAL_SetTimer(UINT8 ui8ch, UINT32 ui32Time);
void LQ_HAL_EnableTimer(UINT8 ui8ch);
void LQ_HAL_DisableTimer(UINT8 ui8ch);
UINT8 LQ_HAL_GetStatus(UINT8 ui8ch);
UINT32 LQ_HAL_GetDTS_STC(UINT8 ui8ch);
UINT32 LQ_HAL_GetDTS_GSTC(UINT8 ui8ch);

#ifdef __cplusplus
}
#endif

#endif // endif of _LQ_HAL_API_h

