/* ****************************************************************************************
 * SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 * Copyright(c) 1998-2010 by LG Electronics Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * *****************************************************************************************/

/**
 * @file
 *
 * HAL driver header for VDC device of VDEC.
 * VDEC device will teach you how to make device driver with new platform.
 *
 * author     youngwoo.jin@lge.com
 * version    0.1
 * date       2013.05.14
 * note       Additional information.
 *
 */

#ifndef _VDC_HAL_API_H_
#define _VDC_HAL_API_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include "../vdec_type_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
#define VDC_HAL_INT_SRC_BASE				(0x00)
#define VDC_HAL_INT_SRC_RESET_BASE			(0x10)

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/
typedef enum
{
    VDC_HAL_CMD_SET_DEC_MODE,
    VDC_HAL_CMD_GET_DEC_MODE,
    VDC_HAL_CMD_RETURN_OCCUPANCY,
} VDC_HAL_CMD_T;

typedef enum
{
    VDC_HAL_RET_OK,
    VDC_HAL_RET_G1_OCCUPIED,
    VDC_HAL_RET_HEVC_OCCUPIED,
} VDC_HAL_RET_T;

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Generic Usage Functions
----------------------------------------------------------------------------------------*/
void	VDC_HAL_Init(void);
void    VDC_HAL_Suspend(void);
void    VDC_HAL_Resume(void);
UINT32	VDC_HAL_GetVersion(UINT8 ui8CoreNum);
UINT32	VDC_HAL_SetApbMachOffset(UINT8 ui8CoreNum, UINT8 ui8Offset);
UINT32	VDC_HAL_SetApbVDO(UINT8 ui8CoreNum,UINT8 ui8GdiSelection, UINT8 ui8Offset);
void	VDC_HAL_ResetMach(UINT8 ui8CoreNum, UINT8 ui8SubIdx);
void	VDC_HAL_SetMachIntrMode(UINT8 ui8CoreNum, UINT8 IntrMode);
UINT8	VDC_HAL_GetMachIntrMode(UINT8 ui8CoreNum);
UINT8	VDC_HAL_GetMachIdleStatus(UINT8 ui8CoreNum);
UINT8	VDC_HAL_GetMachUnderRunStatus(UINT8 ui8CoreNum);
void	VDC_HAL_EnableInterrupt(UINT8 ui8CoreNum, UINT8 ui8IntrSrc);
BOOLEAN	VDC_HAL_DisableInterrupt(UINT8 ui8CoreNum, UINT8 ui8IntrSrc);
void	VDC_HAL_ClearInterrupt(UINT8 ui8CoreNum, UINT8 ui8IntrSrc);
void	VDC_HAL_ClearInterruptMask(UINT8 ui8CoreNum,UINT32 ui32IntrMsk);
void	VDC_HAL_DisableInterruptAll(UINT8 ui8CoreNum);
BOOLEAN	VDC_HAL_IsInterruptEnable(UINT8 ui8CoreNum, UINT8 ui8IntrSrc);
UINT32	VDC_HAL_GetInterruptStatus(UINT8 ui8CoreNum);
void	VDC_HAL_EnableFeedTimer(UINT8 ui8CoreNum);
void	VDC_HAL_DisableFeedTimer(UINT8 ui8CoreNum);
void	VDC_HAL_SetFeedTimerValue(UINT8 ui8CoreNum, UINT32 ui32Value);
UINT32	VDC_HAL_GetFeedTimerValue(UINT8 ui8CoreNum);
VDC_HAL_RET_T VDC_HAL_ExtraCmd(UINT8 ui8CoreNum,VDC_HAL_CMD_T eCmd);

#ifdef __cplusplus
}
#endif

#endif // endif of _VDC_HAL_API_H_

