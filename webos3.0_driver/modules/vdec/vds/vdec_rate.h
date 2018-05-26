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
 * version    1.0
 * date       2011.03.11
 * note       Additional information.
 *
 */

#ifndef _VDEC_RATE_H_
#define _VDEC_RATE_H_

#include "vdec_type_defs.h"

#define	VDEC_SPEED_MAX						16
#define	VDEC_FRAMERATE_MAX					60
#define	VDEC_FRAMERATE_MIN					7

// 24Hz
#define	VDEC_DEFAULT_FRAME_RATE_RES			24
#define	VDEC_DEFAULT_FRAME_RATE_DIV			1


#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
	RATE_SRC_NONE,
	RATE_SRC_PTS,
	RATE_SRC_CONF,
	RATE_SRC_CODEC,
	RATE_SRC_CODEC_DBL,
	RATE_SRC_DEBUG,
	RATE_SRC_MAX
} RATE_SRC_E;

void VRATE_Init(void);
void VRATE_SetUseGSTC(UINT8 ui8SyncCh, BOOLEAN bUseGstc);
void VRATE_Reset(UINT8 ui8LipSyncCh);
void VRATE_Flush(UINT8 ui8SyncCh);

BOOLEAN VRATE_SetSpeed(UINT8 ui8SyncCh, SINT32 i32Speed);
SINT32 VRATE_GetSpeed(UINT8 ui8SyncCh );

void VRATE_SetRateLimit(UINT8 ui8SyncCh, UINT32 ui32RateLimit);
void VRATE_UpdateConfigRate(UINT8 ui8LipSyncCh, UINT32 ui32FrameRateRes,
		UINT32 ui32FrameRateDiv);
void VRATE_UpdateCodecRate(UINT8 ui8LipSyncCh,
		UINT32 ui32FrameRateRes, UINT32 ui32FrameRateDiv, BOOLEAN bInterlaced);
UINT32 VDEC_RATE_UpdateFrameRate_byDTS(UINT8 ui8LipSyncCh, UINT32 ui32DTS,
		UINT32 *pui32FrameRateRes, UINT32 *pui32FrameRateDiv);
UINT32 VRATE_UpdateByDqPop(UINT8 ui8LipSyncCh, UINT32 ui32GSTC,
		UINT32 *pui32FrameRateRes, UINT32 *pui32FrameRateDiv);
UINT32 VRATE_UpdateByPTS(UINT8 ui8LipSyncCh, UINT32 ui32PTS,
		UINT32 *pui32FrameRateRes, UINT32 *pui32FrameRateDiv);
UINT32 VRATE_UpdateByDqPTS(UINT8 ui8SyncCh, UINT32 ui32PTS,
		UINT32 *pui32FrameRateRes, UINT32 *pui32FrameRateDiv);
void VRATE_UpdateDebugRate(UINT8 ui8SyncCh, UINT32 ui32FrameRateRes,
		UINT32 ui32FrameRateDiv);

UINT32 VDEC_RATE_GetDTSRate(UINT8 ui8LipSyncCh, UINT32 *pui32FrameRateRes,
		UINT32 *pui32FrameRateDiv);
UINT32 VRATE_GetPTSRate(UINT8 ui8LipSyncCh, UINT32 *pui32FrameRateRes,
		UINT32 *pui32FrameRateDiv);

UINT32 VRATE_GetDuration(UINT8 ui8LipSyncCh);
UINT32 VRATE_GetResDiv(UINT8 ui8LipSyncCh, UINT32 *pui32FrameRateRes,
		UINT32 *pui32FrameRateDiv);
BOOLEAN VRATE_IsModifiedRate(UINT8 ui8SyncCh);
RATE_SRC_E VRATE_GetRateSource(UINT8 ui8SyncCh);
BOOLEAN VRATE_UseEnqRate(UINT8 u8SyncCh, BOOLEAN on, UINT32 res, UINT32 div);


#ifdef __cplusplus
}
#endif

#endif /* _VDEC_RATE_H_ */

