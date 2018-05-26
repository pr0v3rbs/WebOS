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
 *  h3d drv header file for BE device
 *	
 *  author		justine.jeong
 *  version		1.0
 *  date		2012.03.16
 *  note		Additional information.
 *
 *  @addtogroup BE
 *	@{
 */

#ifndef BE_H3D_HW_H14_INC
#define BE_H3D_HW_H14_INC

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include "be_kapi.h"
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
int BE_H3D_HW_H14_Get_Version(LX_BE_H3D_VERSION_T *pstParams);
int BE_H3D_HW_H14_Reset_Block(LX_BE_H3D_RESET_BLOCK_T *pstParams);
int BE_H3D_HW_H14_Bypass_All(LX_BE_H3D_BYPASS_ALL_T *pstParams);
int BE_H3D_HW_H14_Set_Mode(LX_BE_H3D_MODE_T *pstParams);
int BE_H3D_HW_H14_Get_Mode(LX_BE_H3D_MODE_T *pstParams);
int BE_H3D_HW_H14_In_Format(LX_BE_H3D_IN_FORMAT_T *pstParams);
int BE_H3D_HW_H14_T3C_Depth_Level(LX_BE_H3D_T3C_DEPTH_LEVEL_T *pstParams);
int BE_H3D_HW_H14_A3C_Depth_Level(LX_BE_H3D_A3C_DEPTH_LEVEL_T *pstParams);
int BE_H3D_HW_H14_A3C_AutoConvergence(LX_BE_H3D_A3C_AUTOCONVERGENCE_T *pstParams);
int BE_H3D_HW_H14_Set_TridFmt( LX_BE_TRIDTV_FMT_CTRL_T *pstParams);

//int BE_FRC_Ioctl(unsigned int cmd, unsigned long arg);

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

#endif /* BE_H3D_HW_H14_INC */

/** @} */



