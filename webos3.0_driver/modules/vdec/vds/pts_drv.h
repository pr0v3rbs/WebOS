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
 * date       2011.11.26
 * note       Additional information.
 *
 */

#ifndef _VDEC_PTS_H_
#define _VDEC_PTS_H_

#include "vdec_type_defs.h"
#include "disp_q.h"

#define VDEC_PTS_BIT_SIZE	32
#define VDEC_PTS_MASK		(0xFFFFFFFF>>(32-VDEC_PTS_BIT_SIZE))

typedef enum
{
	STC_PTS_NONE = 0,
	STC_PTS_DISCARD,
	STC_PTS_SMOOTH_DISCARD,
	STC_PTS_MATCHED,
	STC_PTS_SMOOTH_WAITING,
	STC_PTS_WAITING,
	STC_PTS_FREERUN,
	STC_PTS_MAX,
	STC_PTS_MAX_STR_LEN = 18,
	STC_PTS_UINT32 = 0x20110716
} PTS_MATCH_E;


typedef enum
{
	PTS_MATCH_ENABLE = 0,
	PTS_MATCH_FREERUN_BASED_SYNC,
	PTS_MATCH_FREERUN_IGNORE_SYNC,
	PTS_MATCH_ADAPTIVE_FREERUN,
	PTS_MATCH_MODE_MAX,
	PTS_MATCH_MODE_STR_LEN = 20,
	PTS_MATCH_MODE_UINT32 = 0x20120330
} PTS_MATCH_MODE_E;

struct _SYNC_T;
typedef struct _SYNC_T SYNC_T;

extern char azMatchType[STC_PTS_MAX][STC_PTS_MAX_STR_LEN];

#define		VDEC_UNKNOWN_PTS			0xFFFFFFFE

#ifdef __cplusplus
extern "C" {
#endif


const char *PTS_MatchedName (PTS_MATCH_E mat);
void PTS_Init(void);
SYNC_T* PTS_GetHandler(UINT8 ui8SyncCh);
SYNC_T* PTS_Open(UINT8 ui8SyncCh, BOOLEAN bUseGstc, UINT32 ui32DisplayOffset_ms,
		BOOLEAN bIsDualDecoding);
void PTS_Close(SYNC_T *pstSync);
void PTS_SetDisplayOffset(SYNC_T *pstSync, UINT32 ui32DisplayOffset_ms);
#if defined (CHIP_NAME_d14)
SINT32 PTS_GetDisplayOffset(SYNC_T *pstSync);
#endif
void PTS_Reset(SYNC_T *pstSync);
void PTS_Start(UINT8 ui8SyncCh, BOOLEAN bStep);
void PTS_Stop(UINT8 ui8SyncCh);

void PTS_WraparoundCheck(UINT8 ui8SyncCh, UINT32 ui32STC);
void PTS_SetMatchMode(SYNC_T *pstSync, PTS_MATCH_MODE_E eMatchMode);
PTS_MATCH_MODE_E PTS_GetMatchMode(SYNC_T *pstSync);
void PTS_SetStcDiscont(SYNC_T *pstSync);
BOOLEAN PTS_IsDiscont(SYNC_T *pstSync);

void PTS_Set_BaseTime(SYNC_T *pstSync, UINT32 ui32BaseTime, UINT32 ui32BasePTS);
void PTS_Get_BaseTime(SYNC_T *pstSync, UINT32 *pui32BaseTime, UINT32 *pui32BasePTS);

void PTS_SetMaxQlen(SYNC_T *pstSync, UINT8 max_qlen);
UINT32 PTS_GetMaxQlen(SYNC_T *pstSync);

void PTS_Flush(SYNC_T *pstSync);
void PTS_StepStart(SYNC_T *pstSync);

void PTS_SetDiscardLv(UINT8 u8Lv);
void PTS_NotiCurQlen(SYNC_T *pstSync, SINT32 i32Qlen);

PTS_MATCH_E PTS_GetNewFrame(UINT8 ui8SyncCh, UINT32 ui32STC,
		S_DISPQ_BUF_T *psPopDisBuf, UINT32 ui32GSTC, BOOLEAN *pbInterlaced);

SINT32 PTS_GetDiff(SYNC_T *pstSync);

UINT32 PTS_CheckFrame(SYNC_T *pstSync, S_DISPQ_BUF_T *pstDqFrame);
PTS_MATCH_E PTS_MatchCheck(SYNC_T *pstSync, UINT32 ui32PTS,	UINT32 ui32STC,
		UINT32 ui32PTS_Prev);

void PTS_SetPhaseShift(SYNC_T *pstSync, SINT32 shift);
BOOLEAN PTS_SetUseGstc(UINT8 ui8SyncCh, BOOLEAN bUseGstc);

#ifdef __cplusplus
}
#endif

#endif /* _VDEC_PTS_H_ */

