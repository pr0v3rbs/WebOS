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

#ifndef _VDEC_DE_IF_H_
#define _VDEC_DE_IF_H_


#include "vdec_type_defs.h"
#include "disp_q.h"
#include "vdisp_cfg.h"
//#include "mcu_if.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DEIF_NUMOF_CH		VDISP_NUM_OF_CH

typedef enum
{
	DE_IF_DST_DE_IF0		= 0x0,
	DE_IF_DST_DE_IF1		= 0x1,
	DE_IF_DST_DE_IF2		= 0x2,
	DE_IF_DST_DE_IF3		= 0x3,
#if defined (CHIP_NAME_h15) || defined (CHIP_NAME_m16)
	DE_IF_DST_DE_IF4		= 0x4,
	DE_IF_DST_DE_IF5		= 0x5,
	DE_IF_DST_NONE			= 0x6,
#else
	DE_IF_DST_NONE      = 0x4,
#endif
	DE_IF_DST_32bits		= 0x20120321,
} DE_IF_DST_E;

#if defined (CHIP_NAME_h15) || defined (CHIP_NAME_m16)
#define	DEIF_NUMOF_HWPORT		6
#else
#define	DEIF_NUMOF_HWPORT		4
#endif

#define	DEIF_NUMOF_OUTPORT		((DEIF_NUMOF_HWPORT)+1)
#define	DE_IF_DST_MAX			((DEIF_NUMOF_HWPORT)+1)

typedef enum {
	DEIF_CB_DISP_DONE,
	DEIF_CB_REQ_NEXT,
	DEIF_CB_NONE
} DEIF_CB_TYPE_E;


void DE_IF_Init(void);
BOOLEAN DE_IF_Open(UINT8 u8DeIfCh, DE_IF_DST_E eOutPort);
void DE_IF_SelVsync(UINT8 u8DeIfCh, UINT32 ui32VSyncSrc);
DE_IF_DST_E DE_IF_GetTargetDe(UINT8 u8DeIfCh);
DE_IF_DST_E DE_IF_SetTargetDe(UINT8 u8DeIfCh, DE_IF_DST_E eTargetDe);
void DE_IF_Close(UINT8 u8DeIfCh);
void DE_IF_Freeze(UINT8 u8DeIfCh, BOOLEAN bOnOff);
void DE_IF_RepeatFrame(UINT8 u8DeIfCh, UINT8 u8nTimes);
UINT32 DE_IF_GetRunningFrameIndex(UINT8 u8DeIfCh);
void DE_IF_RunVdo(UINT8 u8FieldInfo);
BOOLEAN DE_IF_IsNeedNewFrame(UINT8 u8DeIfCh);
BOOLEAN DE_IF_SetNewFrame(UINT8 u8DeIfCh, DISPQ_FRM_T *psDisBuf, UINT32 ui32SyncField);
BOOLEAN DE_IF_UpdateDisplay(UINT8 u8DeIfCh, UINT32 ui32SyncField);
DE_IF_DST_E DE_IF_SetDualOutput(DE_IF_DST_E eMirrorSrc, DE_IF_DST_E eMirrorPort);
DE_IF_DST_E DE_IF_SetDualOutput2(DE_IF_DST_E eMirrorSrc, DE_IF_DST_E eMirrorPort);
void DE_IF_RepeatFrame(UINT8 u8DeIfCh, UINT8 u8nTimes);
void DE_IF_SetPrivateData(UINT8 u8DeIfCh, void *priv);
void DEIF_GetDispState(UINT8 u8DeIfCh, UINT32 *pu32DispPeriod,
		UINT32 *pu32FieldCnt, BOOLEAN *bValidCurFrm, BOOLEAN *bValidNextFrm);

void DEIF_Suspend(void);
void DEIF_Resume(void);
#if defined(CHIP_NAME_d14)
void DE_IF_SeamlessModeNoti(UINT8 ui8SeamlessMode);
#endif

#ifdef __cplusplus
}
#endif

#endif /* _VDEC_DE_IF_H_ */

