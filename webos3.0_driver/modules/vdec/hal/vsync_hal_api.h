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
 * date       2011.05.28
 * note       Additional information.
 *
 */


#ifndef _SYNC_HAL_API_h
#define _SYNC_HAL_API_h

#include <linux/spinlock.h>

#include "vdec_type_defs.h"
#include "vdec_base.h"
#include "de_ipc_hal_api.h"


#ifdef __cplusplus
extern "C" {
#endif

#define VSYNC_SW_NUM_OF_CH		(1)
#define	VSYNC_NUM_OF_CH			(VSYNC_HW_NUM_OF_CH+VSYNC_SW_NUM_OF_CH)

void VSync_HAL_Init(void);

void VSync_HAL_Suspend(void);
void VSync_HAL_Resume(void);

int VSync_HAL_IRQ_Init (void (*fpHandler)(UINT8 ch));

UINT32 VSync_HAL_GetVersion(void);
void VSync_HAL_Reset(void);
UINT32 VSync_HAL_UTIL_GCD(UINT32 a, UINT32 b);
BOOLEAN VSync_HAL_SetVsyncField(UINT8 ui8VSyncCh, UINT32 ui32FrameRateRes, UINT32 ui32FrameRateDiv, BOOLEAN bInterlaced);
void VSync_HAL_RestartAllVsync(void);
UINT32  VSync_HAL_IntField(UINT8 ui8VSyncCh);
UINT32  VSync_HAL_ExtField(UINT8 ui8VSyncCh);

void VSync_HAL_SetVsyncShift(UINT8 ui8SyncCh, SINT32 i32Delta);
void VSync_HAL_SetUpdateMode(UINT8 ui8TimerCh, UINT8 ui8Mode);
void VSync_HAL_ResetTimer(UINT8 ui8TimerCh);
void VSync_HAL_EnableTimer(UINT8 ui8TimerCh);
void VSync_HAL_DisableTimer(UINT8 ui8TimerCh);
void VSync_HAL_SetTimer(UINT8 ui8TimerCh, UINT32 ui32Value);

void VSync_HAL_SelVsync(UINT32 vsync_ch, UINT32 out_port);
void VSync_HAL_EnableVsync(UINT32 u32VsyncCh);
void VSync_HAL_DisableVsync(UINT32 u32VsyncCh);


extern spinlock_t VSync_HAL_IRQLock;

#ifdef __cplusplus
}
#endif


#endif // endif of _VDEC_HAL_API_h
