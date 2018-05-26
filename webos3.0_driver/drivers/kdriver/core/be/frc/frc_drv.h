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
 *  frc drv header file for BE device
 *	
 *  author		justine.jeong
 *  version		1.0
 *  date		2012.03.16
 *  note		Additional information.
 *
 *  @addtogroup BE
 *	@{
 */

#ifndef _FRC_DRV_H_
#define _FRC_DRV_H_

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
int BE_FRC_Init(void);
int BE_FRC_Close(void);
int BE_FRC_Control_Memc(unsigned long arg);
int BE_FRC_Set_Demo_Mode(unsigned long arg);
int BE_FRC_Set_Debug_Box(unsigned long arg);
int BE_FRC_Set_Bypass_Memc_Reg(unsigned long arg);
int BE_FRC_Set_YUV_444_Mode(unsigned long arg);
int BE_FRC_Set_Low_Delay(unsigned long arg);
int BE_FRC_Get_Firmware_Ver(unsigned long arg);
int BE_FRC_Force_Frame_Rate(unsigned long arg);
int BE_FRC_Set_Freeze(unsigned long arg);
int BE_FRC_Update_FrameDelay(unsigned long arg);

int BE_FRC_Ioctl(unsigned int cmd, unsigned long arg);

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

#endif /* _FRC_DRV_H_ */

/** @} */


