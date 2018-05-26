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
 *	BE LED module control Drivers linux module
 *
 *  author      dj911.kim@lge.com
 *  version		1.0
 *  date
 *  note		Additional information.
 *
 *  @addtogroup lg115x_be
 *	@{
 */
 
#ifndef  BE_LED_HW_H13_INC
#define  BE_LED_HW_H13_INC
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
int BE_LED_HW_H13_Initialize(void);
int BE_LED_HW_H13_Close(void);
int BE_LED_HW_H13_Resume(void);

int BE_LED_HW_H13_Get_Version(LX_BE_LED_VERSION_T *pstParams);
int BE_LED_HW_H13_Set_Mode(LX_BE_LED_MODE_T *pstParams);
int BE_LED_HW_H13_Control(LX_BE_LED_CONTROL_T *pstParams);
int BE_LED_HW_H13_Get_RegInfo(LX_BE_LED_REG_INFO_T *pstParams);

int BE_HDR_HW_H13_Get_Version(LX_BE_HDR_VERSION_T *pstParams);
int BE_HDR_HW_H13_Set_Mode(LX_BE_HDR_MODE_T *pstParams);
int BE_HDR_HW_H13_Control(LX_BE_HDR_CONTROL_T *pstParams);

int BE_WCG_HW_H13_Get_Version(LX_BE_WCG_VERSION_T *pstParams);
int BE_WCG_HW_H13_Set_Mode(LX_BE_WCG_MODE_T *pstParams);
int BE_WCG_HW_H13_Control(LX_BE_WCG_CONTROL_T *pstParams);

/*----------------------------------------------------------------------------------------
 *   External Variables
 *---------------------------------------------------------------------------------------*/

#endif   /* ----- #ifndef BE_LED_HW_H13_INC  ----- */
/**  @} */
