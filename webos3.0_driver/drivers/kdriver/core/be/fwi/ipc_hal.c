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
 *  ipc hal file for BE device
 *
 *  author		dj911.kim
 *  version		1.0
 *  date		2014.06.02
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

#include "os_util.h"
#include "be_dbg.h"

#include "be_def.h"
#include "h13/be/fwi/be_ipc_hw_h13.h"
#include "h14/be/fwi/be_ipc_hw_h14.h"
#include "m14/be/fwi/be_ipc_hw_m14.h"
#include "h15/be/fwi/be_ipc_hw_h15.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/
typedef struct
{
	int (*pfnInitializeIPC)(UINT32 *pIPCBuffToMCU, UINT32 *pIPCBuffFrMCU);
	int (*pfnCloseIPC)(UINT32 *pIPCBuffToMCU, UINT32 *pIPCBuffFrMCU);
} BE_IPC_HW_FUNCTION_T;

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
static BE_IPC_HW_FUNCTION_T gstBeIpcHwFunction;

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
int BE_IPC_HAL_Init(UINT32 *pIPCBuffToMCU, UINT32 *pIPCBuffFrMCU)
{
	int ret = RET_OK;

	if(0)
	{

	}
#ifdef INCLUDE_H15_CHIP_KDRV
	if( lx_chip_rev( ) >= LX_CHIP_REV( H15, A0) )
	{
		BE_PRINT("BE IPC chip revision is set to H15 A0\n");
		gstBeIpcHwFunction.pfnInitializeIPC			= BE_IPC_HW_H15_Init;
		gstBeIpcHwFunction.pfnCloseIPC			= BE_IPC_HW_H15_Close;
	}
#endif
#ifdef INCLUDE_H14_CHIP_KDRV
	else if( lx_chip_rev( ) >= LX_CHIP_REV( H14, A0) )
	{
		BE_PRINT("BE IPC chip revision is set to H14 A0\n");
		gstBeIpcHwFunction.pfnInitializeIPC			= BE_IPC_HW_H14_Init;
		gstBeIpcHwFunction.pfnCloseIPC			= BE_IPC_HW_H14_Close;
	}
#endif
#ifdef INCLUDE_M14_CHIP_KDRV
	else if( lx_chip_rev( ) >= LX_CHIP_REV( M14, A0) )
	{
		BE_PRINT("BE IPC chip revision is set to M14 A0\n");
		gstBeIpcHwFunction.pfnInitializeIPC			= BE_IPC_HW_M14_Init;
		gstBeIpcHwFunction.pfnCloseIPC			= BE_IPC_HW_M14_Close;
	}
#endif
#ifdef INCLUDE_H13_CHIP_KDRV
	else if ( lx_chip_rev( ) >= LX_CHIP_REV( H13, A0) )
	{
		BE_PRINT("BE IPC chip revision is set to H13 A0\n");
		gstBeIpcHwFunction.pfnInitializeIPC			= BE_IPC_HW_H13_Init;
		gstBeIpcHwFunction.pfnCloseIPC			= BE_IPC_HW_H13_Close;
	}
#endif
	else
	{
		BE_ERROR("BE IPC ERROR! Unknown chip revision at BE IPC module\n");
		gstBeIpcHwFunction.pfnInitializeIPC			= NULL;
		gstBeIpcHwFunction.pfnCloseIPC			= NULL;
		ret = RET_ERROR;
	}

	if(gstBeIpcHwFunction.pfnInitializeIPC)
	{
		ret = gstBeIpcHwFunction.pfnInitializeIPC(pIPCBuffToMCU, pIPCBuffFrMCU);
	}

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief free ipc register
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int BE_IPC_HAL_Close(UINT32 *pIPCBuffToMCU, UINT32 *pIPCBuffFrMCU)
{
	int ret = RET_OK;

	if(gstBeIpcHwFunction.pfnCloseIPC)
	{
		ret = gstBeIpcHwFunction.pfnCloseIPC(pIPCBuffToMCU, pIPCBuffFrMCU);
	}

	gstBeIpcHwFunction.pfnInitializeIPC			= NULL;
	gstBeIpcHwFunction.pfnCloseIPC			= NULL;

	return ret;
}


