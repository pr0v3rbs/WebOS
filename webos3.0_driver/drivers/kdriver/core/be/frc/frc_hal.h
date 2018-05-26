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
 *  frc hal header file for be device
 *	
 *  author		justine.jeong
 *  version		1.0
 *  date		2012.03.16
 *  note		Additional information.
 *
 *  @addtogroup be
 *	@{
 */

#ifndef _FRC_HAL_H_
#define _FRC_HAL_H_

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
int BE_FRC_HAL_Init(void);
int BE_FRC_HAL_Close(void);
int BE_FRC_HAL_ControlMemc(LX_BE_FRC_MEMC_MODE_T *pstParams);
int BE_FRC_HAL_Set_Demo_Mode(LX_BE_FRC_DEMO_MODE_T *pstParams);
int BE_FRC_HAL_Set_Debug_Box(LX_BE_FRC_ENABLE_DEBUG_BOX_T *pstParams);
int BE_FRC_HAL_Set_Bypass_Memc_Reg(LX_BE_FRC_MEMC_BYPASS_REGION_T *pstParams);
int BE_FRC_HAL_Set_YUV_444_Mode(LX_BE_FRC_COLOR_YUV444_T *pstParams);
int BE_FRC_HAL_Set_Low_Delay(LX_BE_FRC_LOW_DELAY_T *pstParams);
int BE_FRC_HAL_Get_Firmware_Ver(LX_BE_FRC_VERSION_T *pstParams);
int BE_FRC_HAL_Force_Frame_Rate(LX_BE_FRC_FRAME_RATE_T *pstParams);
int BE_FRC_HAL_Set_Freeze(LX_BE_FRC_FREEZE_T *pstParams);
int BE_FRC_HAL_Update_FrameDelay(LX_BE_LOW_DELAY_T *pstParams);
/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

#endif /* _FRC_HAL_H_ */

/** @} */


