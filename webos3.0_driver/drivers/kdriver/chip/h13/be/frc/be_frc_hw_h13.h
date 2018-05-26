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
 *	BE FRC module control Drivers linux module
 *
 *  author      dj911.kim@lge.com
 *  version		1.0
 *  date
 *  note		Additional information.
 *
 *  @addtogroup lg115x_be
 *	@{
 */
 
#ifndef  BE_FRC_HW_H13_INC
#define  BE_FRC_HW_H13_INC
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
int BE_FRC_HW_H13_Initialize(void);
int BE_FRC_HW_H13_Close(void);
int BE_FRC_HW_H13_ControlMemc(LX_BE_FRC_MEMC_MODE_T *pstParams);
int BE_FRC_HW_H13_Set_Demo_Mode(LX_BE_FRC_DEMO_MODE_T *pstParams);
int BE_FRC_HW_H13_Set_Debug_Box(LX_BE_FRC_ENABLE_DEBUG_BOX_T *pstParams);
int BE_FRC_HW_H13_Set_Bypass_Memc_Reg(LX_BE_FRC_MEMC_BYPASS_REGION_T *pstParams);
int BE_FRC_HW_H13_Set_YUV_444_Mode(LX_BE_FRC_COLOR_YUV444_T *pstParams);
int BE_FRC_HW_H13_Set_Low_Delay(LX_BE_FRC_LOW_DELAY_T *pstParams);
int BE_FRC_HW_H13_Get_Firmware_Ver(LX_BE_FRC_VERSION_T *pstParams);
int BE_FRC_HW_H13_Force_Frame_Rate(LX_BE_FRC_FRAME_RATE_T *pstParams);

/*----------------------------------------------------------------------------------------
 *   External Variables
 *---------------------------------------------------------------------------------------*/

#endif   /* ----- #ifndef BE_FRC_HW_H13_INC  ----- */
/**  @} */

