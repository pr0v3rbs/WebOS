/* ****************************************************************************************
 * SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 * Copyright(c) 1998-2010 by LG Electronics Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * *****************************************************************************************/

/**
 * @file
 *
 * HAL driver implementation for VDC device of VDEC.
 * VDEC device will teach you how to make device driver with new platform.
 *
 * author     youngwoo.jin@lge.com
 * version    0.1
 * date       2013.05.14
 * note       Additional information.
 *
 * @addtogroup d13_vdec
 * @{
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include "vdec_type_defs.h"
#include "hal/vdec_base.h"
#include "hal/vdc_hal_api.h"
#include "hal/top_hal_api.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define _GET_INT_SRC(_src)																\
	( (_src < VDC_HAL_INT_SRC_RESET_BASE)?												\
		(_src - VDC_HAL_INT_SRC_BASE + MACH0) :											\
		(_src - VDC_HAL_INT_SRC_RESET_BASE + MACH0_SRST_DONE) )

#define _GET_INT_SRC_MASK(_src)															\
	( (_src & ((1 << VDC_HAL_INT_SRC_RESET_BASE) - 1)) |								\
	  ((_src >> VDC_HAL_INT_SRC_RESET_BASE) << (MACH0_SRST_DONE)) )

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
	global Functions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/

/*========================================================================================
    Implementation Group
========================================================================================*/
/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
void	VDC_HAL_Init(void)
{
}

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
void	VDC_HAL_Suspend(void)
{
}
EXPORT_SYMBOL(VDC_HAL_Suspend);

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
void	VDC_HAL_Resume(void)
{
}
EXPORT_SYMBOL(VDC_HAL_Resume);

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
UINT32	VDC_HAL_GetVersion(UINT8 ui8CoreNum)
{
	return TOP_HAL_GetVersion();
}
EXPORT_SYMBOL(VDC_HAL_GetVersion);

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
UINT32	VDC_HAL_SetApbMachOffset(UINT8 ui8CoreNum, UINT8 ui8Offset)
{
	return TOP_HAL_SetApbMachOffset(ui8CoreNum, ui8Offset);
}
EXPORT_SYMBOL(VDC_HAL_SetApbMachOffset);

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
UINT32	VDC_HAL_SetApbVDO(UINT8 ui8CoreNum, UINT8 ui8GdiSelection, UINT8 ui8Offset)
{
	return TOP_HAL_SetApbVDO(ui8GdiSelection, ui8Offset);
}
EXPORT_SYMBOL(VDC_HAL_SetApbVDO);

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
void	VDC_HAL_ResetMach(UINT8 ui8CoreNum, UINT8 ui8SubIdx)
{
	TOP_HAL_ResetMach(ui8SubIdx);
}
EXPORT_SYMBOL(VDC_HAL_ResetMach);

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
void	VDC_HAL_SetMachIntrMode(UINT8 ui8CoreNum, UINT8 IntrMode)
{
	TOP_HAL_SetMachIntrMode(ui8CoreNum, IntrMode);
}
EXPORT_SYMBOL(VDC_HAL_SetMachIntrMode);

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
UINT8	VDC_HAL_GetMachIntrMode(UINT8 ui8CoreNum)
{
	return TOP_HAL_GetMachIntrMode(ui8CoreNum);
}
EXPORT_SYMBOL(VDC_HAL_GetMachIntrMode);

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
UINT8	VDC_HAL_GetMachIdleStatus(UINT8 ui8CoreNum)
{
	return TOP_HAL_GetMachIdleStatus(ui8CoreNum);
}
EXPORT_SYMBOL(VDC_HAL_GetMachIdleStatus);

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
UINT8	VDC_HAL_GetMachUnderRunStatus(UINT8 ui8CoreNum)
{
	return TOP_HAL_GetMachUnderRunStatus(ui8CoreNum);
}
EXPORT_SYMBOL(VDC_HAL_GetMachUnderRunStatus);

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
void	VDC_HAL_EnableInterrupt(UINT8 ui8CoreNum, UINT8 ui8IntrSrc)
{
	ui8IntrSrc = _GET_INT_SRC(ui8IntrSrc);
	TOP_HAL_EnableExtIntr(ui8IntrSrc);
}
EXPORT_SYMBOL(VDC_HAL_EnableInterrupt);

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
BOOLEAN	VDC_HAL_DisableInterrupt(UINT8 ui8CoreNum, UINT8 ui8IntrSrc)
{
	ui8IntrSrc = _GET_INT_SRC(ui8IntrSrc);
	return TOP_HAL_DisableExtIntr(ui8IntrSrc);
}
EXPORT_SYMBOL(VDC_HAL_DisableInterrupt);

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
void	VDC_HAL_ClearInterrupt(UINT8 ui8CoreNum, UINT8 ui8IntrSrc)
{
	ui8IntrSrc = _GET_INT_SRC(ui8IntrSrc);
	TOP_HAL_ClearExtIntr(ui8IntrSrc);
}
EXPORT_SYMBOL(VDC_HAL_ClearInterrupt);

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
void	VDC_HAL_ClearInterruptMask(UINT8 ui8CoreNum, UINT32 ui32IntrMsk)
{
	ui32IntrMsk = _GET_INT_SRC_MASK(ui32IntrMsk);
	TOP_HAL_ClearExtIntrMsk(ui32IntrMsk);
}
EXPORT_SYMBOL(VDC_HAL_ClearInterruptMask);

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
void	VDC_HAL_DisableInterruptAll(UINT8 ui8CoreNum)
{
	TOP_HAL_DisableExtIntrAll();
}
EXPORT_SYMBOL(VDC_HAL_DisableInterruptAll);

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
BOOLEAN	VDC_HAL_IsInterruptEnable(UINT8 ui8CoreNum, UINT8 ui8IntrSrc)
{
	ui8IntrSrc = _GET_INT_SRC(ui8IntrSrc);
	return TOP_HAL_IsExtIntrEnable(ui8IntrSrc);
}
EXPORT_SYMBOL(VDC_HAL_IsInterruptEnable);

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
UINT32	VDC_HAL_GetInterruptStatus(UINT8 ui8CoreNum)
{
	UINT32	ui32RetVal = 0x00;
	UINT32	ui32Status = TOP_HAL_GetExtIntrStatus();

	if ( ui32Status & (1 << MACH0) ) {
		ui32RetVal |= (1 << (VDC_HAL_INT_SRC_BASE + 0));
	}

	if ( ui32Status & (1 << MACH1) ) {
		ui32RetVal |= (1 << (VDC_HAL_INT_SRC_BASE + 1));
	}

	if ( ui32Status & (1 << MACH0_SRST_DONE) ) {
		ui32RetVal |= (1 << (VDC_HAL_INT_SRC_RESET_BASE + 0));
	}

	if ( ui32Status & (1 << MACH1_SRST_DONE) ) {
		ui32RetVal |= (1 << (VDC_HAL_INT_SRC_RESET_BASE + 1));
	}

	return ui32RetVal;
}
EXPORT_SYMBOL(VDC_HAL_GetInterruptStatus);

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
void	VDC_HAL_EnableFeedTimer(UINT8 ui8CoreNum)
{
	TOP_HAL_EnableFeedTimer();
}
EXPORT_SYMBOL(VDC_HAL_EnableFeedTimer);

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
void	VDC_HAL_DisableFeedTimer(UINT8 ui8CoreNum)
{
	TOP_HAL_DisableFeedTimer();
}
EXPORT_SYMBOL(VDC_HAL_DisableFeedTimer);

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
void	VDC_HAL_SetFeedTimerValue(UINT8 ui8CoreNum, UINT32 ui32Value)
{
	TOP_HAL_SetFeedTimerValue(ui32Value);
}
EXPORT_SYMBOL(VDC_HAL_SetFeedTimerValue);

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
UINT32	VDC_HAL_GetFeedTimerValue(UINT8 ui8CoreNum)
{
	return TOP_HAL_GetFeedTimerValue();
}
EXPORT_SYMBOL(VDC_HAL_GetFeedTimerValue);

/**
 ********************************************************************************
 * @brief
 *
 * @remarks
 *  DETAIL INFORMATION
 * @par requirements:
 *
 * @param
 *
 * @return
 *
 ********************************************************************************
 */
VDC_HAL_RET_T VDC_HAL_ExtraCmd(UINT8 ui8CoreNum, VDC_HAL_CMD_T eCmd)
{
    return VDC_HAL_RET_OK;
}
EXPORT_SYMBOL(VDC_HAL_ExtraCmd);

/*========================================================================================
    Static Implementation Group
========================================================================================*/

/** @} */

