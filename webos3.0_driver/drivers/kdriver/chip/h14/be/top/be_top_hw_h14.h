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
 *	BE TOP module control Drivers linux module
 *
 *  author      dj911.kim@lge.com
 *  version		1.0
 *  date
 *  note		Additional information.
 *
 *  @addtogroup lg115x_be
 *	@{
 */
 
#ifndef  BE_TOP_HW_H14_INC
#define  BE_TOP_HW_H14_INC
/*----------------------------------------------------------------------------------------
 *   Control Constants
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/
#include "be_kapi.h"

/*----------------------------------------------------------------------------------------
 *   Constant Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Macro Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Type Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   External Function Prototype Declarations
 *---------------------------------------------------------------------------------------*/
int BE_TOP_HW_H14_Initialize(void);
int BE_TOP_HW_H14_Close(void);

int BE_TOP_HW_H14_ResetSW(void);
int BE_TOP_HW_H14_SetUartForMcu(BOOLEAN isForMcu);
int BE_TOP_HW_H14_SetBypass(LX_BE_BLOCK_BYPASS_T *pstParams);
int BE_TOP_HW_H14_GetBypass(LX_BE_BLOCK_BYPASS_T *pstParams);
int BE_TOP_HW_H14_Toggle3DLR(LX_BE_3D_LR_T *pstParams);
int BE_TOP_HW_H14_Set3DInFormat(LX_BE_TOP_3D_IN_FORMAT_T *pstParams);
int BE_TOP_HW_H14_Get3DInFormat(LX_BE_TOP_3D_IN_FORMAT_T *pstParams);
int BE_TOP_HW_H14_Set3DOutFormat(LX_BE_3D_OUT_FORMAT_T *pstParams);
int BE_TOP_HW_H14_Set3DConvergence(LX_BE_3D_CONVERGENCE_T *pstParams);
int BE_TOP_HW_H14_SetBlackBar(LX_BE_BLACK_BAR_T *pstParams);
int BE_TOP_HW_H14_SetVideoMute(LX_BE_VIDEO_MUTE_T *pstParams);
int BE_TOP_HW_H14_SetDisplayType(LX_BE_DISPLAY_TYPE_T *pstParams);
int BE_TOP_HW_H14_SetDebugDisplay(LX_BE_DEBUG_DISPLAY_T *pstParams);
int BE_TOP_HW_H14_SetVideoOperation(LX_BE_VID_OPERATION_T *pstParams);
int BE_TOP_HW_H14_SetInputSrcType(LX_BE_SRC_TYPE_T *pstParams);
int BE_TOP_HW_H14_SetActiveWindowStart(LX_BE_ACTIVE_WINDOW_START_T *pstParams);
int BE_TOP_HW_H14_SetActiveWindowSize(LX_BE_ACTIVE_WINDOW_SIZE_T *pstParams);
int BE_TOP_HW_H14_FirmwareDebug(BE_FIRMWARE_DEBUG_T *pstParams);

/*----------------------------------------------------------------------------------------
 *   External Variables
 *---------------------------------------------------------------------------------------*/

#endif   /* ----- #ifndef BE_TOP_HW_H14_INC  ----- */
/**  @} */

