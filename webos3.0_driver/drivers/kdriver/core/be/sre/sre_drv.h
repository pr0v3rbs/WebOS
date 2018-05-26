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
 *  sre drv header file for BE device
 *	
 *  author		won.hur	
 *  version		1.0
 *  date		2012.04.27
 *  note		Additional information.
 *
 *  @addtogroup BE
 *	@{
 */

#ifndef _SRE_DRV_H_
#define _SRE_DRV_H_

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

int BE_SRE_Init(void);
int BE_SRE_Close(void);
int BE_SRE_Set_Command(unsigned long arg);
int BE_SRE_Set_Params(unsigned long arg);
int BE_SRE_Get_Version(unsigned long arg);
int BE_SRE_Set_Input_Format(unsigned long arg);
int BE_SRE_Ioctl(unsigned int cmd, unsigned long arg);


int BE_SRE_SR_Strength_Level(unsigned long arg);
int BE_SRE_Write_Data(unsigned long arg);
int BE_SRE_Get_Data(unsigned long arg);
int BE_SRE_Set_Noise_Control(unsigned long arg);
int BE_SRE_Set_Local_Bypass(unsigned long arg);
int BE_SRE_Set_Clip_Control(unsigned long arg);
int BE_SRE_Set_Weight_Control(unsigned long arg);
int BE_SRE_Set_Etc_Control(unsigned long arg);


/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

#endif /* _SRE_DRV_H_ */

/** @} */







