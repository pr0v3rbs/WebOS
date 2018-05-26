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
 * DE IPC HAL.
 * (Register Access Hardware Abstraction Layer)
 *
 * author     youngki.lyu (youngki.lyu@lge.com)
 * version    0.1
 * date       2011.05.03
 * note       Additional information.
 *
 */

#ifndef _VDEC_DE_IF_HAL_API_H_
#define _VDEC_DE_IF_HAL_API_H_

#include "vdec_type_defs.h"
#include "vdec_base.h"


#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	UINT32 ui32Tiled_FrameBaseAddr;
	UINT32 ui32Y_FrameBaseAddr;
	UINT32 ui32C_FrameBaseAddr;
	UINT32 ui32Y_FrameBaseAddrBot;
	UINT32 ui32C_FrameBaseAddrBot;
	UINT32 ui32Stride;
	UINT32 ui32ColorFormat;

	UINT32 ui32FrameIdx;
	UINT32 ui32ScalerFreeze;
	enum
	{
		DE_IPC_TOP_FIELD	= 1,
		DE_IPC_BOTTOM_FIELD	= 2,
		DE_IPC_FRAME		= 3,
	} ui32DisplayMode;
	UINT32 ui32FieldRepeat;
	UINT32 ui32VdecPause;
	UINT32 ui32AspectRatio;
	UINT32 ui16PicWidth;
	UINT32 ui16PicHeight;
	UINT32 ui32H_Offset;
	UINT32 ui32V_Offset;
	UINT32 ui32UpdateInfo;
	UINT32 ui32FramePackArrange;
	UINT32 ui32LR_Order;

	UINT32 ui16FrameRateRes;
	UINT32 ui16FrameRateDiv;
	UINT32 ui16ParW;
	UINT32 ui16ParH;

	UINT32 ui32PTS;
	UINT32 ui32DpbMapType;
#if defined (CHIP_NAME_h15) || defined (CHIP_NAME_m16)
	//For H15
	UINT32 ui32TopBottomGap;
	BOOLEAN bSmartBalancing;
	UINT8 ui8SmartBalancingMode;
	UINT16 ui16SmBalanOffsetM0;
	UINT16 ui16SmBalanOffsetM1;
#endif
}S_DE_IPC_T;



void DE_IPC_HAL_Init(void);
void DE_IPC_HAL_Resume(void);
BOOLEAN DE_IPC_HAL_Update(UINT8 ui8DeIpcCh, S_DE_IPC_T *psDeIpc);
BOOLEAN DE_IPC_HAL_SetNewFrame(UINT8 ui8DeIpcCh, S_DE_IPC_T *psDeIpc);

#ifdef __cplusplus
}
#endif

#endif /* _VDEC_DE_IF_HAL_API_H_ */

