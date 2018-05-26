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
 *  interrupt hal file for BE device
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

#include "be_kapi.h"
#include "be_def.h"
#include "be_cfg.h"
#include "int_hal.h"
#include "h13/be/fwi/be_int_hw_h13.h"
#include "h14/be/fwi/be_int_hw_h14.h"
#include "m14/be/fwi/be_int_hw_m14.h"
#include "h15/be/fwi/be_int_hw_h15.h"

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
	int (*pfnInitializeINT)(void);
	int (*pfnCloseINT)(void);
	int (*pfnGetIrqNumINT)(UINT32 *beIrq0, UINT32 *beIrq1);
	int (*pfnIntrEnableINT)(void);
	int (*pfnGetStatusINT)(BE_INT_TYPE_T intrType, UINT32 *pIntrStatus);
	int (*pfnClearStatusINT)(BE_INT_TYPE_T intrType, UINT32 *pIntrStatus);
	int (*pfnWakeUpRegINT)(BE_INT_DIR_TYPE_T intrDir, BOOLEAN turnon);
} BE_INT_HW_FUNCTION_T;

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
static BE_INT_HW_FUNCTION_T gstBeIntHwFunction;

/*========================================================================================
	Implementation Group
========================================================================================*/

/**
 * @callgraph
 * @callergraph
 *
 * @brief initialize interrupt register
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int BE_INT_HAL_Init(void)
{
	int ret = RET_OK;

	if(0)
	{

	}
#ifdef INCLUDE_H15_CHIP_KDRV
	else if( lx_chip_rev( ) >= LX_CHIP_REV( H15, A0) )
	{
		BE_PRINT("BE INT chip revision is set to H15 A0\n");
		gstBeIntHwFunction.pfnInitializeINT			= NULL;
		gstBeIntHwFunction.pfnCloseINT			= NULL;
		gstBeIntHwFunction.pfnGetIrqNumINT		= BE_INT_HW_H15_GetIrqNum;
		gstBeIntHwFunction.pfnIntrEnableINT		= BE_INT_HW_H15_IntrEnable;
		gstBeIntHwFunction.pfnGetStatusINT		= BE_INT_HW_H15_GetStatus;
		gstBeIntHwFunction.pfnClearStatusINT		= BE_INT_HW_H15_ClearStatus;
		gstBeIntHwFunction.pfnWakeUpRegINT		= BE_INT_HW_H15_WakeUpReg;
	}
#endif
#ifdef INCLUDE_H14_CHIP_KDRV
	else if( lx_chip_rev( ) >= LX_CHIP_REV( H14, A0) )
	{
		BE_PRINT("BE INT chip revision is set to H14 A0\n");
		gstBeIntHwFunction.pfnInitializeINT			= NULL;
		gstBeIntHwFunction.pfnCloseINT			= NULL;
		gstBeIntHwFunction.pfnGetIrqNumINT		= BE_INT_HW_H14_GetIrqNum;
		gstBeIntHwFunction.pfnIntrEnableINT		= BE_INT_HW_H14_IntrEnable;
		gstBeIntHwFunction.pfnGetStatusINT		= BE_INT_HW_H14_GetStatus;
		gstBeIntHwFunction.pfnClearStatusINT		= BE_INT_HW_H14_ClearStatus;
		gstBeIntHwFunction.pfnWakeUpRegINT		= BE_INT_HW_H14_WakeUpReg;
	}
#endif
#ifdef INCLUDE_M14_CHIP_KDRV
	else if( lx_chip_rev( ) >= LX_CHIP_REV( M14, A0) )
	{
		BE_PRINT("BE INT chip revision is set to M14 A0\n");
		gstBeIntHwFunction.pfnInitializeINT			= NULL;
		gstBeIntHwFunction.pfnCloseINT			= NULL;
		gstBeIntHwFunction.pfnGetIrqNumINT		= BE_INT_HW_M14_GetIrqNum;
		gstBeIntHwFunction.pfnIntrEnableINT		= BE_INT_HW_M14_IntrEnable;
		gstBeIntHwFunction.pfnGetStatusINT		= BE_INT_HW_M14_GetStatus;
		gstBeIntHwFunction.pfnClearStatusINT		= BE_INT_HW_M14_ClearStatus;
		gstBeIntHwFunction.pfnWakeUpRegINT		= BE_INT_HW_M14_WakeUpReg;
	}
#endif
#ifdef INCLUDE_H13_CHIP_KDRV
	else if ( lx_chip_rev( ) >= LX_CHIP_REV( H13, A0) )
	{
		BE_PRINT("BE INT chip revision is set to H13 A0\n");
		gstBeIntHwFunction.pfnInitializeINT			= NULL;
		gstBeIntHwFunction.pfnCloseINT			= NULL;
		gstBeIntHwFunction.pfnGetIrqNumINT		= BE_INT_HW_H13_GetIrqNum;
		gstBeIntHwFunction.pfnIntrEnableINT		= BE_INT_HW_H13_IntrEnable;
		gstBeIntHwFunction.pfnGetStatusINT		= BE_INT_HW_H13_GetStatus;
		gstBeIntHwFunction.pfnClearStatusINT		= BE_INT_HW_H13_ClearStatus;
		gstBeIntHwFunction.pfnWakeUpRegINT		= BE_INT_HW_H13_WakeUpReg;
	}
#endif
	else
	{
		BE_ERROR("BE INT ERROR! Unknown chip revision at BE INT module\n");
		gstBeIntHwFunction.pfnInitializeINT			= NULL;
		gstBeIntHwFunction.pfnCloseINT			= NULL;
		gstBeIntHwFunction.pfnGetIrqNumINT		= NULL;
		gstBeIntHwFunction.pfnIntrEnableINT		= NULL;
		gstBeIntHwFunction.pfnGetStatusINT		= NULL;
		gstBeIntHwFunction.pfnClearStatusINT		= NULL;
		gstBeIntHwFunction.pfnWakeUpRegINT		= NULL;
		ret = RET_ERROR;
	}

	if(gstBeIntHwFunction.pfnInitializeINT)
	{
		ret = gstBeIntHwFunction.pfnInitializeINT();
	}

	return ret;
}

int BE_INT_HAL_Close(void)
{
	int ret = RET_OK;

	if(gstBeIntHwFunction.pfnCloseINT)
	{
		ret = gstBeIntHwFunction.pfnCloseINT();
	}

	gstBeIntHwFunction.pfnInitializeINT			= NULL;
	gstBeIntHwFunction.pfnCloseINT			= NULL;
	gstBeIntHwFunction.pfnGetIrqNumINT		= NULL;
	gstBeIntHwFunction.pfnIntrEnableINT		= NULL;
	gstBeIntHwFunction.pfnGetStatusINT		= NULL;
	gstBeIntHwFunction.pfnClearStatusINT		= NULL;
	gstBeIntHwFunction.pfnWakeUpRegINT		= NULL;

	return ret;
}

int BE_INT_HAL_GetIrqNum(UINT32 *beIrq0, UINT32 *beIrq1)
{
	int ret = RET_OK;

	if(gstBeIntHwFunction.pfnGetIrqNumINT)
	{
		ret = gstBeIntHwFunction.pfnGetIrqNumINT(beIrq0, beIrq1);
	}
	else
	{
		BE_ERROR("BE_INT_HAL  ERROR! This function is not supported!\n");
		ret = RET_ERROR;
	}

	return ret;
}

int BE_INT_HAL_IntrEnable(void)
{
	int ret = RET_OK;

	if(gstBeIntHwFunction.pfnIntrEnableINT)
	{
		ret = gstBeIntHwFunction.pfnIntrEnableINT();
	}
	else
	{
		BE_ERROR("BE_INT_HAL  ERROR! This function is not supported!\n");
		ret = RET_ERROR;
	}

	return ret;
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
int BE_INT_HAL_GetStatus(BE_INT_TYPE_T intrType, UINT32 *pIntrStatus)
{
	int ret = RET_OK;

	if(gstBeIntHwFunction.pfnGetStatusINT)
	{
		ret = gstBeIntHwFunction.pfnGetStatusINT(intrType, pIntrStatus);
	}
	else
	{
		BE_ERROR("BE_INT_HAL  ERROR! This function is not supported!\n");
		ret = RET_ERROR;
	}

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
int BE_INT_HAL_ClearStatus(BE_INT_TYPE_T intrType, UINT32 *pIntrStatus)
{
	int ret = RET_OK;

	if(gstBeIntHwFunction.pfnClearStatusINT)
	{
		ret = gstBeIntHwFunction.pfnClearStatusINT(intrType, pIntrStatus);
	}
	else
	{
		BE_ERROR("BE_INT_HAL  ERROR! This function is not supported!\n");
		ret = RET_ERROR;
	}

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
int BE_INT_HAL_WakeUpReg(BE_INT_DIR_TYPE_T intrDir, BOOLEAN turnon)
{
	int ret = RET_OK;

	if(gstBeIntHwFunction.pfnWakeUpRegINT)
	{
		ret = gstBeIntHwFunction.pfnWakeUpRegINT(intrDir, turnon);
	}
	else
	{
		BE_ERROR("BE_INT_HAL  ERROR! This function is not supported!\n");
		ret = RET_ERROR;
	}

	return ret;
}
