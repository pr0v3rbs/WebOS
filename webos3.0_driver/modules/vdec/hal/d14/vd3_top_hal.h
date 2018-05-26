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
#ifndef	__VD3_TOP_HAL__
#define __VD3_TOP_HAL__

#include "../top_hal_api.h"
#include "vdec_type_defs.h"

void VD3_HAL_Init(void);
UINT32 VD3_HAL_GetVersion(void);
void VD3_HAL_SetVsyncSrc(UINT8 ui8Ch, UINT8 ui8Src);
UINT8 VD3_HAL_GetVsyncSrc(UINT8 ui8Ch);
void VD3_HAL_EnableInterIntr(VDEC_INTR_T ui32IntrSrc);
int VD3_HAL_DisableInterIntr(VDEC_INTR_T ui32IntrSrc);
void VD3_HAL_ClearInterIntr(VDEC_INTR_T ui32IntrSrc);
void VD3_HAL_ClearInterIntrMsk(UINT32 ui32IntrMsk);
void VD3_HAL_DisableInterIntrAll(void);
UINT8 VD3_HAL_IsInterIntrEnable(VDEC_INTR_T ui32IntrSrc);
UINT32 VD3_HAL_GetInterIntrStatus(void);
void VD3_HAL_SetTimerIntrSelection(UINT8 ui8TimerIntrNum, UINT8 ui8TimerCh);
UINT8 VD3_HAL_GetTimerIntrSelection(UINT8 ui8TimerIntrNum);

#endif // __VD3_TOP_HAL__
