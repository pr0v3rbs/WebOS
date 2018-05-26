/*
	SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
	Copyright(c) 2013 by LG Electronics Inc.

	This program is free software; you can redistribute it and/or 
	modify it under the terms of the GNU General Public License
	version 2 as published by the Free Software Foundation.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of 
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
	GNU General Public License for more details.
*/ 


/** @file
 *
 *  BE TOP module
 *
 *  author      dj911.kim@lge.com
 *  version     1.0
 *  date
 *  note        Additional information.
 *
 *  @addtogroup lg115x_be
 *  @{
 */

#ifndef _BE_TOP_HAL_H_
#define _BE_TOP_HAL_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include "be_kapi.h"

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */


/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/
int BE_TOP_HAL_Init(void);
int BE_TOP_HAL_Close(void);

int BE_TOP_HAL_ResetSW(void);
int BE_TOP_HAL_SetUartForMcu(BOOLEAN isForMcu);
int BE_TOP_HAL_SetBypass(LX_BE_BLOCK_BYPASS_T *pstParams);
int BE_TOP_HAL_GetBypass(LX_BE_BLOCK_BYPASS_T *pstParams);
int BE_TOP_HAL_Toggle3DLR(LX_BE_3D_LR_T *pstParams);
int BE_TOP_HAL_Set3DInFormat(LX_BE_TOP_3D_IN_FORMAT_T *pstParams);
int BE_TOP_HAL_Get3DInFormat(LX_BE_TOP_3D_IN_FORMAT_T *pstParams);
int BE_TOP_HAL_Set3DOutFormat(LX_BE_3D_OUT_FORMAT_T *pstParams);
int BE_TOP_HAL_Set3DConvergence(LX_BE_3D_CONVERGENCE_T *pstParams);
int BE_TOP_HAL_SetBlackBar(LX_BE_BLACK_BAR_T *pstParams);
int BE_TOP_HAL_SetVideoMute(LX_BE_VIDEO_MUTE_T *pstParams);
int BE_TOP_HAL_SetDisplayType(LX_BE_DISPLAY_TYPE_T *pstParams);
int BE_TOP_HAL_SetDebugDisplay(LX_BE_DEBUG_DISPLAY_T *pstParams);
int BE_TOP_HAL_SetVideoOperation(LX_BE_VID_OPERATION_T *pstParams);
int BE_TOP_HAL_SetInputSrcType(LX_BE_SRC_TYPE_T *pstParams);
int BE_TOP_HAL_SetActiveWindowStart(LX_BE_ACTIVE_WINDOW_START_T *pstParams);
int BE_TOP_HAL_SetActiveWindowSize(LX_BE_ACTIVE_WINDOW_SIZE_T *pstParams);
int BE_TOP_HAL_SetRealCinema(LX_BE_REAL_CINEMA_T *pstParams);
int BE_TOP_HAL_FirmwareDebug(BE_FIRMWARE_DEBUG_T *pstParams);
int BE_TOP_HAL_Get_FirmwareInfo_Addr(BE_FIRMWARE_INFO_T *pstParams);

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/


#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _BE_TOP_HAL_H_ */

/** @} */

