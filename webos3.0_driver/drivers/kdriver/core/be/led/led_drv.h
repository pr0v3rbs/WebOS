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
 *  led drv header file for BE device
 *	
 *  author		justine.jeong
 *  version		1.0
 *  date		2012.03.06
 *  note		Additional information.
 *
 *  @addtogroup BE
 *	@{
 */

#ifndef _LED_DRV_H_
#define _LED_DRV_H_

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
int BE_LED_Init(void);
int BE_LED_Close(void);

int BE_LED_Get_Version(unsigned long arg);
int BE_LED_Set_Mode(unsigned long arg);
int BE_LED_Control(unsigned long arg);
int BE_LED_Get_Apl(unsigned long arg);
int BE_LED_Set_ClippingValue(unsigned long arg);
int BE_LED_Get_RegInfo(unsigned long arg);

int BE_HDR_Get_Version(unsigned long arg);
int BE_HDR_Set_Mode(unsigned long arg);
int BE_HDR_Control(unsigned long arg);

int BE_WCG_Get_Version(unsigned long arg);
int BE_WCG_Set_Mode(unsigned long arg);
int BE_WCG_Control(unsigned long arg);

int BE_LED_Ioctl(unsigned int cmd, unsigned long arg);

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

#endif /* _LED_DRV_H_ */

/** @} */
