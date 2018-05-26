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
 *  led hal header file for be device
 *	
 *  author		justine.jeong
 *  version		1.0
 *  date		2012.03.16
 *  note		Additional information.
 *
 *  @addtogroup be
 *	@{
 */

#ifndef _LED_HAL_H_
#define _LED_HAL_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include "base_types.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/
int BE_LED_HAL_Init(void);
int BE_LED_HAL_Close(void);
int BE_LED_HAL_Resume(void);

int BE_LED_HAL_Get_Version(LX_BE_LED_VERSION_T *pstParams);
int BE_LED_HAL_Set_Mode(LX_BE_LED_MODE_T *pstParams);
int BE_LED_HAL_Control(LX_BE_LED_CONTROL_T *pstParams);
int BE_LED_HAL_Get_Apl(LX_BE_LED_APL_T *pstParams);
int BE_LED_HAL_Set_ClippingValue(UINT32 u32Param);
int BE_LED_HAL_Get_RegInfo(LX_BE_LED_REG_INFO_T *pstParams);

int BE_HDR_HAL_Get_Version(LX_BE_HDR_VERSION_T *pstParams);
int BE_HDR_HAL_Set_Mode(LX_BE_HDR_MODE_T *pstParams);
int BE_HDR_HAL_Control(LX_BE_HDR_CONTROL_T *pstParams);

int BE_WCG_HAL_Get_Version(LX_BE_WCG_VERSION_T *pstParams);
int BE_WCG_HAL_Set_Mode(LX_BE_WCG_MODE_T *pstParams);
int BE_WCG_HAL_Control(LX_BE_WCG_CONTROL_T *pstParams);

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

#endif /* _LED_HAL_H_ */

/** @} */
