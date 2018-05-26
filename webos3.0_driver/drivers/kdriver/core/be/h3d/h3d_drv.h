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
 *  author		won.hur	
 *  version		1.0
 *  date		2012.04.27
 *  note		Additional information.
 *
 *  @addtogroup BE
 *	@{
 */

#ifndef _H3D_DRV_H_
#define _H3D_DRV_H_

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
int BE_H3D_Init(void);
int BE_H3D_Get_Version(unsigned long arg);
int BE_H3D_Reset_Block(unsigned long arg);
int BE_H3D_Bypass_All(unsigned long arg);
int BE_H3D_Set_Mode(unsigned long arg);
int BE_H3D_Get_Mode(unsigned long arg);
int BE_H3D_In_Format(unsigned long arg);
int BE_H3D_T3C_Depth_Level(unsigned long arg);
int BE_H3D_A3C_Depth_Level(unsigned long arg);
int BE_H3D_A3C_AutoConvergence(unsigned long arg);
int BE_H3D_Set_Trid_Fmt( unsigned long arg);
int BE_H3D_Ioctl(unsigned int cmd, unsigned long arg);


/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

#endif /* _H3D_DRV_H_ */

/** @} */
