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
 *  mcu ipc test file for BE device
 *	
 *  author		justine.jeong
 *  version		1.0
 *  date		2012.03.23
 *  note		Additional information.
 *
 *  @addtogroup BE
 *	@{
 */

#ifndef _MCU_IPC_TEST_H_
#define _MCU_IPC_TEST_H_

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
int MCU_IPC_Init(UINT32 *pIPCBuffToMCU, UINT32 *pIPCBuffFrMCU);
int MCU_IPC_InitInterrupt(void);
int MCU_ISR_FROM_CPU(void);
int MCU_ISR_TO_CPU(void);

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

#endif /* _MCU_IPC_TEST_H_ */

/** @} */
