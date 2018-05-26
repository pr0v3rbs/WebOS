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

#ifndef _H3D_HAL_H_
#define _H3D_HAL_H_

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
int BE_H3D_HAL_Init(void);
int BE_H3D_HAL_Get_Version(LX_BE_H3D_VERSION_T *pstParams);
int BE_H3D_HAL_Reset_Block(LX_BE_H3D_RESET_BLOCK_T *pstParams);
int BE_H3D_HAL_Bypass_All(LX_BE_H3D_BYPASS_ALL_T *pstParams);
int BE_H3D_HAL_Set_Mode(LX_BE_H3D_MODE_T *pstParams);
int BE_H3D_HAL_Get_Mode(LX_BE_H3D_MODE_T *pstParams);
int BE_H3D_HAL_In_Format(LX_BE_H3D_IN_FORMAT_T *pstParams);
int BE_H3D_HAL_T3C_Depth_Level(LX_BE_H3D_T3C_DEPTH_LEVEL_T *pstParams);
int BE_H3D_HAL_A3C_Depth_Level(LX_BE_H3D_A3C_DEPTH_LEVEL_T *pstParams);
int BE_H3D_HAL_A3C_AutoConvergence(LX_BE_H3D_A3C_AUTOCONVERGENCE_T *pstParams);
int BE_H3D_HAL_Set_Trid_Fmt(LX_BE_TRIDTV_FMT_CTRL_T *pstParams);

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

#endif /* _H3D_HAL_H_ */

/** @} */



