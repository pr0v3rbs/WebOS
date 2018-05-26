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
 *
 * author     taewoong.kim@lge.com
 * version    0.1
 * date       2013.11.14
 * note       Additional information.
 *
 */

#ifndef _VDEC_DIF_HAL_H_
#define _VDEC_DIF_HAL_H_

#include "de_ipc_hal_api.h"
#include "vdec_type_defs.h"
#include "../vds/disp_q.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct 
{
	BOOLEAN			valid;
	DISPQ_FRM_T		frm_info;
	UINT16			scan_type;
	UINT16			freeze;
	UINT32			update_info;
} DIF_FRM_T;

//void DE_IF_Init(void);
//BOOLEAN DE_IF_Open(UINT8 u8DeIfCh, DE_IF_DST_E eOutPort);
//void DE_IF_Close(UINT8 u8DeIfCh);
//BOOLEAN DE_IF_IsNeedNewFrame(UINT8 u8DeIfCh);
//BOOLEAN DE_IF_SetNewFrame(UINT8 u8DeIfCh, DISPQ_FRM_T *psDisBuf, UINT32 ui32SyncField);
//BOOLEAN DE_IF_UpdateDisplay(UINT8 u8DeIfCh, UINT32 ui32SyncField);

void DIF_HAL_Init(void);
void DIF_HAL_Reset(UINT8 u8DeIfCh);
void DIF_HAL_Close(UINT8 u8DeIfCh);
void DIF_HAL_Open(UINT8 u8DeIfCh);

//BOOLEAN DIF_HAL_NeedNewFrame(UINT8 ch);
void DIF_HAL_SetNewFrame(UINT8 ch, DISPQ_FRM_T *frm);
//void DIF_HAL_SetNewFrame(UINT8 ch, DISPQ_FRM_T *psDisBuf);
BOOLEAN DIF_HAL_UpdateDisplay(UINT8 ch, DIF_FRM_T *frame);
//BOOLEAN DIF_HAL_UpdateDisplay(UINT8 ch);

void DIF_HAL_SetMirrorOut(UINT8 src, UINT8 mirror_out);
void DIF_HAL_SelVsync(UINT8 ch, UINT32 ui32VSyncSrc);
void DIF_HAL_Suspend(void);
void DIF_HAL_Resume(void);
#if defined (CHIP_NAME_d14)
void DIF_HAL_SetSeamless(UINT8 ui8Seamless);
void DIF_SetFrameInfoDxIpc(UINT32 ui32Resolution, UINT32 ui32DualOutMode, UINT32 ui32FrameRate, UINT32 ui32V_Offset);
UINT32 DIF_GetClockSrc(void);
#endif
#ifdef __cplusplus
}
#endif

#endif /* _VDEC_DIF_HAL_H_ */

