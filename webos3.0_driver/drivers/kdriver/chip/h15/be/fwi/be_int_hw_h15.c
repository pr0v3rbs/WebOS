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
 *  interrupt reg driver file for BE device
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
#include <asm/uaccess.h>
#include <asm/io.h>

#include "os_util.h"
#include "be_dbg.h"

#include "be_kapi.h"
#include "be_def.h"
#include "ipc_def.h"
#include "int_hal.h"
#include "be_int_hw_h15.h"

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
int BE_INT_HW_H15_GetIrqNum(UINT32 *beIrq0, UINT32 *beIrq1)
{
	int ret = RET_OK;

	*beIrq0 = BE_INT_HAL_INVALID_IRQ_NUM;
	*beIrq1 = BE_INT_HAL_INVALID_IRQ_NUM;

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief initialize interrupt register
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int BE_INT_HW_H15_IntrEnable(void)
{
	/* Do Nothing */

	return RET_OK;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief get interrupt status
 *
 * @param intType [IN] interrupt type (hw interrupt/sw interrupt)
 * @param pIntrStatus [OUT] status  of hwi/swi
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int BE_INT_HW_H15_GetStatus(BE_INT_TYPE_T intrType, UINT32 *pIntrStatus)
{
	int ret = RET_OK;
	UINT32 intrStatus = 0;

	/* Do Nothing */

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief clear interrupt status
 *
 * @param intrType [IN] interrupt type (hw interrupt/sw interrupt)
 * @param pIntrStatus [IN] status  of hwi/swi
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int BE_INT_HW_H15_ClearStatus(BE_INT_TYPE_T intrType, UINT32 *pIntrStatus)
{
	int ret = RET_OK;
	UINT32 intrClear = 0;

	/* Do Nothing */

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief wakeup interrupt to cpu/mcu
 *
 * @param intrDir [IN] direction of interrupt
 * @param turnIn [IN] turn on/off
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int BE_INT_HW_H15_WakeUpReg(BE_INT_DIR_TYPE_T intrDir, BOOLEAN turnOn)
{
	int ret = RET_OK;

	/* Do Nothing */

	return ret;
}

