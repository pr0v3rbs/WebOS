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
 *  author		dj911.kim
 *  version		1.0
 *  date		2013.04.03
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
#include "ipc_def_m14.h"
#include "be_ipc_hw_m14.h"
#include "../reg/be_reg_def_m14.h"
#include "../reg/be_reg_dppb_m14.h"

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
extern DPPB_TOP_REG_M14_T gDPPB_TOP_M14;

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
int BE_IPC_HW_M14_Init(UINT32 *pIPCBuffToMCU, UINT32 *pIPCBuffFrMCU)
{
	if (lx_chip_rev() >= LX_CHIP_REV( M14, B0))
	{
		/* Do Nothing */
	}
	else
	{
#ifdef USE_QEMU_SYSTEM
		*pIPCBuffToMCU     = (UINT32)OS_KMalloc(IPC_FROM_CPU_SIZE);
		*pIPCBuffFrMCU     = (UINT32)OS_KMalloc(IPC_FROM_MCU_SIZE);
#else
		*pIPCBuffToMCU = (UINT32)ioremap(BE_IPC_FROM_CPU_M14A0_BASE, IPC_FROM_CPU_SIZE);
		*pIPCBuffFrMCU = (UINT32)ioremap(BE_IPC_FROM_MCU_M14A0_BASE, IPC_FROM_MCU_SIZE);
#endif
	}
	
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
int BE_IPC_HW_M14_Close(UINT32 *pIPCBuffToMCU, UINT32 *pIPCBuffFrMCU)
{
	if (lx_chip_rev() >= LX_CHIP_REV( M14, B0))
	{
		/* Do Nothing */
	}
	else
	{
#ifdef USE_QEMU_SYSTEM
   		OS_Free(*pIPCBuffToMCU);
   		OS_Free(*pIPCBuffFrMCU);
#else
		if (*pIPCBuffToMCU) iounmap((void *)(*pIPCBuffToMCU));
		if (*pIPCBuffFrMCU) iounmap((void *)(*pIPCBuffFrMCU));
#endif
	}
	
	return RET_OK;
}

