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
 *  interrupt hal header file for BE device
 *	
 *  author		justine.jeong
 *  version		1.0
 *  date		2012.03.08
 *  note		Additional information.
 *
 *  @addtogroup BE
 *	@{
 */

#ifndef _BE_INT_HAL_H_
#define _BE_INT_HAL_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include "base_types.h"

#include "ipc_def.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define BE_INT_HAL_INVALID_IRQ_NUM 0xFFFFFFFF
/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/
int BE_INT_HAL_Init(void);
int BE_INT_HAL_Close(void);
int BE_INT_HAL_GetIrqNum(UINT32 *beIrq0, UINT32 *beIrq1);
int BE_INT_HAL_IntrEnable(void);
int BE_INT_HAL_GetStatus(BE_INT_TYPE_T intrType, UINT32 *pIntrStatus);
int BE_INT_HAL_ClearStatus(BE_INT_TYPE_T intrType, UINT32 *pIntrStatus);
int BE_INT_HAL_WakeUpReg(BE_INT_DIR_TYPE_T intrDir, BOOLEAN turnon);

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

#endif /* _BE_INT_HAL_H_ */

/** @} */










