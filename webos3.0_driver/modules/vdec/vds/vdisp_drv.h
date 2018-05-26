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
 * date       2011.05.03
 * note       Additional information.
 *
 */

#ifndef _VDEC_VDISP_DRV_H_
#define _VDEC_VDISP_DRV_H_


#include "vdec_type_defs.h"
#include "de_if_drv.h"
#include "pts_drv.h"
#include "disp_q.h"

typedef enum
{
	VDISP_3D_DUAL_LEFT = 0,
	VDISP_3D_DUAL_RIGHT = 1,
	VDISP_3D_DUAL_NONE = 2
}VDISP_3D_DUAL_TYPE_E;

typedef enum
{
	VDISP_CLK_NONE,
	VDISP_CLK_PDEC0,
	VDISP_CLK_PDEC1,
	VDISP_CLK_PDEC2,
	VDISP_CLK_PDEC3,
	VDISP_CLK_GSTC,
	VDISP_CLK_MAX
} VDISP_CLK_SRC;

typedef enum
{
	VDISP_ST_EMPTY = 1,
	VDISP_ST_SYNC_DIFF			//data unit: ms, +:delay, -:early displayed  
} VDISP_STATUS_TYPE;

typedef	DE_IF_DST_E			E_VDISP_DST_T;

typedef void (*VDISP_STATUS_CB)(UINT8 u8VdispCh, VDISP_STATUS_TYPE eType, UINT32 u32Data);

void VDISP_Init(void);
UINT8 VDISP_Open(E_VDISP_DST_T eVdispDstCh, VDISP_CLK_SRC eClkSrc, UINT32 ui32DisplayOffset_ms,
		UINT32 ui32NumOfDq, BOOLEAN bIsDualDecoding, BOOLEAN bFixedVSync, VDISP_STATUS_CB fpStatusCb,
		VDISP_3D_DUAL_TYPE_E eDual3dtype);
void VDISP_Close(UINT8 u8VdispCh);
void VDISP_Suspend(void);
void VDISP_Resume(void);
void VDISP_SetDisplayOffset(UINT8 u8VdispCh, UINT32 ui32DisplayOffset_ms);
void VDISP_SetFrameRate(UINT8 u8VdispCh, UINT32 u32Res, UINT32 u32Div);
UINT32 VDISP_GetRateInfo(UINT8 u8VdispCh, UINT32 *pui32FrameRateRes,
		UINT32 *pui32FrameRateDiv);
void VDISP_Reset(UINT8 u8VdispCh);
void VDISP_Start(UINT8 u8VdispCh, BOOLEAN bStep);
void VDISP_Stop(UINT8 u8VdispCh);
void VDISP_Flush(UINT8 u8VdispCh);
void VDISP_Freeze( UINT8 u8VdispCh, BOOLEAN bOnOff );
void VDISP_RepeatFrame(UINT8 u8VdispCh, UINT8 u8nRepeat);

E_VDISP_DST_T VDISP_SetOutPort(UINT8 u8VdispCh, E_VDISP_DST_T eOutPort);
UINT32 VDISP_GetSTC(UINT8 u8VdispCh);
BOOLEAN VDISP_SetClkSrc(UINT8 u8VdispCh, VDISP_CLK_SRC eClockSrc);
void VDISP_SetMatchMode(UINT8 u8VdispCh, PTS_MATCH_MODE_E eMatchMode);

void VDISP_SetMaxQlen(UINT8 u8VdispCh, UINT8 u8MaxQlen);
UINT32 VDISP_GetMaxQlen(UINT8 u8VdispCh);

void VDISP_SetSpeed( UINT8 u8VdispCh, SINT32 i32Speed );
SINT32 VDISP_GetSpeed( UINT8 u8VdispCh );

void VDISP_SetBaseTime( UINT8 u8VdispCh, UINT32 ui32BaseTime, UINT32 ui32BasePTS );
void VDISP_GetBaseTime( UINT8 u8VdispCh, UINT32 *pui32BaseTime, UINT32 *pui32BasePTS );

void VDISP_SetStcDiscont(UINT8 u8VdispCh, UINT32 u32AuiId);
BOOLEAN VDISP_SetDualOutput(E_VDISP_DST_T src, BOOLEAN bOn);
void VDISP_StepComplete(UINT8 u8VdispCh);
void VDISP_RunCmd(UINT8 u8VdispCh);

void VDISP_DE_IF_UpdateDisplay(UINT8 u8VdispCh, DE_IF_DST_E eDeIfDstCh, UINT32 ui32SyncField);
BOOLEAN VDISP_DE_IF_SetNextFrame(UINT8 u8VdispCh, S_DISPQ_BUF_T *psDisBuf, UINT32 ui32SyncField);

void VDISP_DebugCmd(UINT32 u32nParam, CHAR *apParams[]);
UINT32 VDISP_GetDispQLen(UINT8 u8VdispCh);
wait_queue_head_t *VDISP_GetDispQWait (int ch);
void VDISP_SendNewFrame(UINT8 u8VdispCh, S_DISPQ_BUF_T *data,
		void (*displayed_callback) (void *arg, S_DISPQ_BUF_T *buffer),
		void *displayed_callback_arg);

typedef struct
{
	int width;
	int height;

	int fr_num;
	int fr_den;
} S_VDISP_DISP_INFO_T;

void VDISP_GetCurrentInfo(UINT8 k8VdispCh, S_VDISP_DISP_INFO_T *psInfo);
#if defined (CHIP_NAME_d14)
void VDISP_SeamlessModeNoti(UINT8 ui8SeamlessMode);
void _VDISP_SetFirstFrmInfo(S_DISPQ_BUF_T *pstDqFrm);
#endif
#endif /* _VDEC_VDISP_DRV_H_ */

