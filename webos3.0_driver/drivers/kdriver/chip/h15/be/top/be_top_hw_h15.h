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

#ifndef  BE_TOP_HW_H15_INC
#define  BE_TOP_HW_H15_INC
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
int BE_TOP_HW_H15_Initialize(void);
int BE_TOP_HW_H15_Close(void);
int BE_TOP_HW_H15_SetBypass(LX_BE_BLOCK_BYPASS_T *pstParams);
int BE_TOP_HW_H15_Toggle3DLR(LX_BE_3D_LR_T *pstParams);
int BE_TOP_HW_H15_Set3DOutFormat(LX_BE_3D_OUT_FORMAT_T *pstParams);
int BE_TOP_HW_H15_Set3DConvergence(LX_BE_3D_CONVERGENCE_T *pstParams);
int BE_TOP_HW_H15_SetBlackBar(LX_BE_BLACK_BAR_T *pstParams);
int BE_TOP_HW_H15_SetRealCinema(LX_BE_REAL_CINEMA_T *pstParams);
int BE_TOP_HW_H15_SetDebugDisplay(LX_BE_DEBUG_DISPLAY_T *pstParams);
int BE_TOP_HW_H15_Get_FirmwareInfo_Addr(BE_FIRMWARE_INFO_T *pstParams);

/*----------------------------------------------------------------------------------------
 *   External Variables
 *---------------------------------------------------------------------------------------*/

#endif   /* ----- #ifndef BE_TOP_HW_H15_INC  ----- */
/**  @} */

