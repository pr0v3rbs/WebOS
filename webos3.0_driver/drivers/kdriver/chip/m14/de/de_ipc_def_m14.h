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
 * main driver implementation for de device.
 * de device will teach you how to make device driver with new platform.
 *
 * author     jaemo.kim (jaemo.kim@lge.com)
 * version    1.0
 * date       2011.02.23
 * note       Additional information.
 *
 * @addtogroup lg1152_de
 * @{
 */

#ifndef  DE_IPC_DEF_M14_INC
#define  DE_IPC_DEF_M14_INC

/*----------------------------------------------------------------------------------------
 *   Control Constants
 *---------------------------------------------------------------------------------------*/
#define DTVSOC_IPC_FROM_CPU_M14_BASE	 DE_IPC_FRM_M14_BASE
#define DTVSOC_IPC_FROM_MCU_M14_BASE	 (DTVSOC_IPC_FROM_CPU_M14_BASE + \
                                          DTVSOC_IPC_FROM_CPU_SIZE)

#define DTVSOC_IPC_1_FROM_CPU_M14_BASE   (DPPB_TOP_REG_M14A0_BASE + \
                                          DPPB_TOP_IPC_M14A0_OFFSET)
#define DTVSOC_IPC_1_FROM_MCU_M14_BASE   (DTVSOC_IPC_1_FROM_CPU_M14_BASE + \
                                          IPC_1_FROM_CPU_SIZE)

#define DTVSOC_IPC_FROM_CPU_M14B_BASE	 DE_IPC_FRM_M14B_BASE
#define DTVSOC_IPC_FROM_MCU_M14B_BASE	 (DTVSOC_IPC_FROM_CPU_M14B_BASE + \
                                          DTVSOC_IPC_FROM_CPU_SIZE)


/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/
#include "mcu/de_ipc_def.h"

/*----------------------------------------------------------------------------------------
 *   Constant Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Macro Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Type Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   External Function Prototype Declarations
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   External Variables
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   global Functions
 *---------------------------------------------------------------------------------------*/

#endif   /* ----- #ifndef DE_IPC_DEF_M14_INC  ----- */
/**  @} */
