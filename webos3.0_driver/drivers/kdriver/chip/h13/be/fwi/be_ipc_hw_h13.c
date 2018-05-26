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
 *  ipc reg driver file for BE device
 *	
 *  author		justine.jeong
 *  version		1.0
 *  date		2012.03.08
 *  note		Additional information.
 *
 *  @addtogroup BE
 *	@{
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include <linux/kernel.h>
#include <asm/uaccess.h>
#include <asm/io.h>

#include "os_util.h"
#include "be_dbg.h"

#include "be_kapi.h"
#include "be_def.h"
#include "be_cfg.h"
#include "ipc_def.h"
#include "ipc_def_h13.h"
#include "be_ipc_hw_h13.h"
#include "../reg/be_reg_def_h13.h"
#include "../reg/be_reg_dppb_h13.h"

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

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/

/*========================================================================================
	Implementation Group
========================================================================================*/

/**
 * @callgraph
 * @callergraph
 *
 * @brief initialize ipc register
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int BE_IPC_HW_H13_Init(UINT32 *pIPCBuffToMCU, UINT32 *pIPCBuffFrMCU)
{
#ifdef USE_QEMU_SYSTEM
	*pIPCBuffToMCU     = (UINT32)OS_KMalloc(IPC_FROM_CPU_SIZE);
	*pIPCBuffFrMCU     = (UINT32)OS_KMalloc(IPC_FROM_MCU_SIZE);
#else
	if(lx_chip_rev() >= LX_CHIP_REV(H13, B0))
	{
		*pIPCBuffToMCU = (UINT32)ioremap(BE_IPC_FROM_CPU_H13B0_BASE, IPC_FROM_CPU_SIZE);
		*pIPCBuffFrMCU = (UINT32)ioremap(BE_IPC_FROM_MCU_H13B0_BASE, IPC_FROM_MCU_SIZE);
	}
	else
	{
		*pIPCBuffToMCU = (UINT32)ioremap(BE_IPC_FROM_CPU_H13A0_BASE, IPC_FROM_CPU_SIZE);
		*pIPCBuffFrMCU = (UINT32)ioremap(BE_IPC_FROM_MCU_H13A0_BASE, IPC_FROM_MCU_SIZE);
	}
#endif
	return RET_OK;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief free ipc register
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int BE_IPC_HW_H13_Close(UINT32 *pIPCBuffToMCU, UINT32 *pIPCBuffFrMCU)
{
#ifdef USE_QEMU_SYSTEM
   OS_Free(*pIPCBuffToMCU);
   OS_Free(*pIPCBuffFrMCU);
#else
	if (*pIPCBuffToMCU) iounmap((void *)(*pIPCBuffToMCU));
	if (*pIPCBuffFrMCU) iounmap((void *)(*pIPCBuffFrMCU));
#endif
	return RET_OK;
}

