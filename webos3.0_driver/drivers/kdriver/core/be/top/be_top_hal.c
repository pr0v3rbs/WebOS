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
 *	BE TOP module control Drivers linux module
 *
 *  author     dj911.kim@lge.com
 *  version		1.0
 *  date
 *  note		Additional information.
 *
 *  @addtogroup lg115x_be
 *	@{
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>	/**< printk() */
#include <linux/slab.h> 	/**< kmalloc() */
#include <linux/fs.h> 		/**< everything\ldots{} */
#include <linux/types.h> 	/**< size_t */
#include <linux/fcntl.h>	/**< O_ACCMODE */
#include <asm/uaccess.h>
#include <linux/timer.h>
#include <linux/ioport.h>	/**< For request_region, check_region etc */
#include <asm/io.h>			/**< For ioremap_nocache */
#include <linux/interrupt.h>
#include "os_util.h"

#include "be_dbg.h"
#include "be_top_hal.h"
#include "h13/be/top/be_top_hw_h13.h"
#include "h14/be/top/be_top_hw_h14.h"
#include "m14/be/top/be_top_hw_m14.h"
#include "h15/be/top/be_top_hw_h15.h"

// Add here for register header file
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
	int (*BE_TOP_HWInitialize)(void);
	int (*BE_TOP_HWClose)(void);
	int (*BE_TOP_HWResetSW)(void);
	int (*BE_TOP_HWSetUartForMcu)(BOOLEAN isForMcu);
	int (*BE_TOP_HWSetBypass)(LX_BE_BLOCK_BYPASS_T *pstParams);
	int (*BE_TOP_HWGetBypass)(LX_BE_BLOCK_BYPASS_T *pstParams);
	int (*BE_TOP_HWToggle3DLR)(LX_BE_3D_LR_T *pstParams);
	int (*BE_TOP_HWSet3DInFormat)(LX_BE_TOP_3D_IN_FORMAT_T *pstParams);
	int (*BE_TOP_HWGet3DInFormat)(LX_BE_TOP_3D_IN_FORMAT_T *pstParams);
	int (*BE_TOP_HWSet3DOutFormat)(LX_BE_3D_OUT_FORMAT_T *pstParams);
	int (*BE_TOP_HWSet3DConvergence)(LX_BE_3D_CONVERGENCE_T *pstParams);
	int (*BE_TOP_HWSetBlackBar)(LX_BE_BLACK_BAR_T *pstParams);
	int (*BE_TOP_HWSetVideoMute)(LX_BE_VIDEO_MUTE_T *pstParams);
	int (*BE_TOP_HWSetDisplayType)(LX_BE_DISPLAY_TYPE_T *pstParams);
	int (*BE_TOP_HWSetDebugDisplay)(LX_BE_DEBUG_DISPLAY_T *pstParams);
	int (*BE_TOP_HWSetVideoOperation)(LX_BE_VID_OPERATION_T *pstParams);
	int (*BE_TOP_HWSetInputSrcType)(LX_BE_SRC_TYPE_T *pstParams);
	int (*BE_TOP_HWSetActiveWindowStart)(LX_BE_ACTIVE_WINDOW_START_T *pstParams);
	int (*BE_TOP_HWSetActiveWindowSize)(LX_BE_ACTIVE_WINDOW_SIZE_T *pstParams);
	int (*BE_TOP_HWSetRealCinema)(LX_BE_REAL_CINEMA_T *pstParams);
	int (*BE_TOP_HWFirmwareDebug)(BE_FIRMWARE_DEBUG_T *pstParams);
	int (*BE_TOP_HWGet_FirmwareInfo_Addr)(BE_FIRMWARE_INFO_T *pstParams);
} BE_TOP_HW_FUNCTION_T;

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
static BE_TOP_HW_FUNCTION_T stBeTopHwFunction;

/*========================================================================================
	Implementation Group
========================================================================================*/

/**
* BE_TOP Module Initialize
*
* @param void
* @return int
*/
int BE_TOP_HAL_Init(void)
{
	int ret = RET_OK;

	if(0)
	{

	}
#ifdef INCLUDE_H15_CHIP_KDRV
	else if( lx_chip_rev( ) >= LX_CHIP_REV( H15, A0) )
	{
		BE_PRINT("BE TOP chip revision is set to H15 A0\n");
		stBeTopHwFunction.BE_TOP_HWInitialize				= BE_TOP_HW_H15_Initialize;
		stBeTopHwFunction.BE_TOP_HWResetSW				= NULL;
		stBeTopHwFunction.BE_TOP_HWSetUartForMcu		= NULL;
		stBeTopHwFunction.BE_TOP_HWSetBypass			= NULL;
		stBeTopHwFunction.BE_TOP_HWGetBypass			= NULL;
		stBeTopHwFunction.BE_TOP_HWToggle3DLR			= BE_TOP_HW_H15_Toggle3DLR;
		stBeTopHwFunction.BE_TOP_HWSet3DInFormat		= NULL;
		stBeTopHwFunction.BE_TOP_HWGet3DInFormat		= NULL;
		stBeTopHwFunction.BE_TOP_HWSet3DOutFormat		= BE_TOP_HW_H15_Set3DOutFormat;
		stBeTopHwFunction.BE_TOP_HWSet3DConvergence	= BE_TOP_HW_H15_Set3DConvergence;
		stBeTopHwFunction.BE_TOP_HWSetBlackBar			= BE_TOP_HW_H15_SetBlackBar;
		stBeTopHwFunction.BE_TOP_HWSetVideoMute			= NULL;
		stBeTopHwFunction.BE_TOP_HWSetDisplayType		= NULL;
		stBeTopHwFunction.BE_TOP_HWSetDebugDisplay		= BE_TOP_HW_H15_SetDebugDisplay;
		stBeTopHwFunction.BE_TOP_HWSetVideoOperation		= NULL;
		stBeTopHwFunction.BE_TOP_HWSetInputSrcType		= NULL;
		stBeTopHwFunction.BE_TOP_HWSetActiveWindowStart	= NULL;
		stBeTopHwFunction.BE_TOP_HWSetActiveWindowSize	= NULL;
		stBeTopHwFunction.BE_TOP_HWSetRealCinema		= BE_TOP_HW_H15_SetRealCinema;
		stBeTopHwFunction.BE_TOP_HWFirmwareDebug		= NULL;
		stBeTopHwFunction.BE_TOP_HWClose				= NULL;
		stBeTopHwFunction.BE_TOP_HWGet_FirmwareInfo_Addr= NULL;
	}
#endif
#ifdef INCLUDE_H14_CHIP_KDRV
	else if( lx_chip_rev( ) >= LX_CHIP_REV( H14, A0) )
	{
		BE_PRINT("BE TOP chip revision is set to H14 A0\n");
		stBeTopHwFunction.BE_TOP_HWInitialize				= BE_TOP_HW_H14_Initialize;
		stBeTopHwFunction.BE_TOP_HWResetSW				= BE_TOP_HW_H14_ResetSW;
		stBeTopHwFunction.BE_TOP_HWSetUartForMcu		= BE_TOP_HW_H14_SetUartForMcu;
		stBeTopHwFunction.BE_TOP_HWSetBypass			= BE_TOP_HW_H14_SetBypass;
		stBeTopHwFunction.BE_TOP_HWGetBypass			= BE_TOP_HW_H14_GetBypass;
		stBeTopHwFunction.BE_TOP_HWToggle3DLR			= BE_TOP_HW_H14_Toggle3DLR;
		stBeTopHwFunction.BE_TOP_HWSet3DInFormat		= BE_TOP_HW_H14_Set3DInFormat;
		stBeTopHwFunction.BE_TOP_HWGet3DInFormat		= BE_TOP_HW_H14_Get3DInFormat;
		stBeTopHwFunction.BE_TOP_HWSet3DOutFormat		= BE_TOP_HW_H14_Set3DOutFormat;
		stBeTopHwFunction.BE_TOP_HWSet3DConvergence	= BE_TOP_HW_H14_Set3DConvergence;
		stBeTopHwFunction.BE_TOP_HWSetBlackBar			= BE_TOP_HW_H14_SetBlackBar;
		stBeTopHwFunction.BE_TOP_HWSetVideoMute			= BE_TOP_HW_H14_SetVideoMute;
		stBeTopHwFunction.BE_TOP_HWSetDisplayType		= BE_TOP_HW_H14_SetDisplayType;
		stBeTopHwFunction.BE_TOP_HWSetDebugDisplay		= BE_TOP_HW_H14_SetDebugDisplay;
		stBeTopHwFunction.BE_TOP_HWSetVideoOperation		= BE_TOP_HW_H14_SetVideoOperation;
		stBeTopHwFunction.BE_TOP_HWSetInputSrcType		= BE_TOP_HW_H14_SetInputSrcType;
		stBeTopHwFunction.BE_TOP_HWSetActiveWindowStart	= BE_TOP_HW_H14_SetActiveWindowStart;
		stBeTopHwFunction.BE_TOP_HWSetActiveWindowSize	= BE_TOP_HW_H14_SetActiveWindowSize;
		stBeTopHwFunction.BE_TOP_HWSetRealCinema		= NULL;
		stBeTopHwFunction.BE_TOP_HWFirmwareDebug		= BE_TOP_HW_H14_FirmwareDebug;
		stBeTopHwFunction.BE_TOP_HWClose				= BE_TOP_HW_H14_Close;
		stBeTopHwFunction.BE_TOP_HWGet_FirmwareInfo_Addr= NULL;
	}
#endif
#ifdef INCLUDE_M14_CHIP_KDRV
	else if( lx_chip_rev( ) >= LX_CHIP_REV( M14, A0) )
	{
		BE_PRINT("BE TOP chip revision is set to M14 A0\n");
		stBeTopHwFunction.BE_TOP_HWInitialize				= BE_TOP_HW_M14_Initialize;
		stBeTopHwFunction.BE_TOP_HWResetSW				= BE_TOP_HW_M14_ResetSW;
		stBeTopHwFunction.BE_TOP_HWSetUartForMcu		= BE_TOP_HW_M14_SetUartForMcu;
		stBeTopHwFunction.BE_TOP_HWSetBypass			= BE_TOP_HW_M14_SetBypass;
		stBeTopHwFunction.BE_TOP_HWGetBypass			= BE_TOP_HW_M14_GetBypass;
		stBeTopHwFunction.BE_TOP_HWToggle3DLR			= BE_TOP_HW_M14_Toggle3DLR;
		stBeTopHwFunction.BE_TOP_HWSet3DInFormat		= BE_TOP_HW_M14_Set3DInFormat;
		stBeTopHwFunction.BE_TOP_HWGet3DInFormat		= BE_TOP_HW_M14_Get3DInFormat;
		stBeTopHwFunction.BE_TOP_HWSet3DOutFormat		= BE_TOP_HW_M14_Set3DOutFormat;
		stBeTopHwFunction.BE_TOP_HWSet3DConvergence	= BE_TOP_HW_M14_Set3DConvergence;
		stBeTopHwFunction.BE_TOP_HWSetBlackBar			= BE_TOP_HW_M14_SetBlackBar;
		stBeTopHwFunction.BE_TOP_HWSetVideoMute			= BE_TOP_HW_M14_SetVideoMute;
		stBeTopHwFunction.BE_TOP_HWSetDisplayType		= BE_TOP_HW_M14_SetDisplayType;
		stBeTopHwFunction.BE_TOP_HWSetDebugDisplay		= BE_TOP_HW_M14_SetDebugDisplay;
		stBeTopHwFunction.BE_TOP_HWSetVideoOperation		= BE_TOP_HW_M14_SetVideoOperation;
		stBeTopHwFunction.BE_TOP_HWSetInputSrcType		= BE_TOP_HW_M14_SetInputSrcType;
		stBeTopHwFunction.BE_TOP_HWSetActiveWindowStart	= BE_TOP_HW_M14_SetActiveWindowStart;
		stBeTopHwFunction.BE_TOP_HWSetActiveWindowSize	= BE_TOP_HW_M14_SetActiveWindowSize;
		stBeTopHwFunction.BE_TOP_HWSetRealCinema		= NULL;
		stBeTopHwFunction.BE_TOP_HWFirmwareDebug		= BE_TOP_HW_M14_FirmwareDebug;
		stBeTopHwFunction.BE_TOP_HWClose				= BE_TOP_HW_M14_Close;
		stBeTopHwFunction.BE_TOP_HWGet_FirmwareInfo_Addr= BE_TOP_HW_M14_Get_FirmwareInfo_Addr;
	}
#endif
#ifdef INCLUDE_H13_CHIP_KDRV
	else if ( lx_chip_rev( ) >= LX_CHIP_REV( H13, A0) )
	{
		BE_PRINT("BE TOP chip revision is set to H13 A0\n");
		stBeTopHwFunction.BE_TOP_HWInitialize				= BE_TOP_HW_H13_Initialize;
		stBeTopHwFunction.BE_TOP_HWResetSW				= BE_TOP_HW_H13_ResetSW;
		stBeTopHwFunction.BE_TOP_HWSetUartForMcu		= BE_TOP_HW_H13_SetUartForMcu;
		stBeTopHwFunction.BE_TOP_HWSetBypass			= BE_TOP_HW_H13_SetBypass;
		stBeTopHwFunction.BE_TOP_HWGetBypass			= BE_TOP_HW_H13_GetBypass;
		stBeTopHwFunction.BE_TOP_HWToggle3DLR			= BE_TOP_HW_H13_Toggle3DLR;
		stBeTopHwFunction.BE_TOP_HWSet3DInFormat		= BE_TOP_HW_H13_Set3DInFormat;
		stBeTopHwFunction.BE_TOP_HWGet3DInFormat		= BE_TOP_HW_H13_Get3DInFormat;
		stBeTopHwFunction.BE_TOP_HWSet3DOutFormat		= BE_TOP_HW_H13_Set3DOutFormat;
		stBeTopHwFunction.BE_TOP_HWSet3DConvergence	= BE_TOP_HW_H13_Set3DConvergence;
		stBeTopHwFunction.BE_TOP_HWSetBlackBar			= BE_TOP_HW_H13_SetBlackBar;
		stBeTopHwFunction.BE_TOP_HWSetVideoMute			= BE_TOP_HW_H13_SetVideoMute;
		stBeTopHwFunction.BE_TOP_HWSetDisplayType		= BE_TOP_HW_H13_SetDisplayType;
		stBeTopHwFunction.BE_TOP_HWSetDebugDisplay		= BE_TOP_HW_H13_SetDebugDisplay;
		stBeTopHwFunction.BE_TOP_HWSetVideoOperation		= BE_TOP_HW_H13_SetVideoOperation;
		stBeTopHwFunction.BE_TOP_HWSetInputSrcType		= BE_TOP_HW_H13_SetInputSrcType;
		stBeTopHwFunction.BE_TOP_HWSetActiveWindowStart	= BE_TOP_HW_H13_SetActiveWindowStart;
		stBeTopHwFunction.BE_TOP_HWSetActiveWindowSize	= BE_TOP_HW_H13_SetActiveWindowSize;
		stBeTopHwFunction.BE_TOP_HWSetRealCinema		= NULL;
		stBeTopHwFunction.BE_TOP_HWFirmwareDebug		= BE_TOP_HW_H13_FirmwareDebug;
		stBeTopHwFunction.BE_TOP_HWClose				= BE_TOP_HW_H13_Close;
		stBeTopHwFunction.BE_TOP_HWGet_FirmwareInfo_Addr= NULL;
	}
#endif
	else
	{
		BE_ERROR("BE TOP ERROR! Unknown chip revision at BE TOP module\n");
		stBeTopHwFunction.BE_TOP_HWInitialize				= NULL;
		stBeTopHwFunction.BE_TOP_HWResetSW				= NULL;
		stBeTopHwFunction.BE_TOP_HWSetUartForMcu		= NULL;
		stBeTopHwFunction.BE_TOP_HWSetBypass			= NULL;
		stBeTopHwFunction.BE_TOP_HWGetBypass			= NULL;
		stBeTopHwFunction.BE_TOP_HWToggle3DLR			= NULL;
		stBeTopHwFunction.BE_TOP_HWSet3DInFormat		= NULL;
		stBeTopHwFunction.BE_TOP_HWGet3DInFormat		= NULL;
		stBeTopHwFunction.BE_TOP_HWSet3DOutFormat		= NULL;
		stBeTopHwFunction.BE_TOP_HWSet3DConvergence	= NULL;
		stBeTopHwFunction.BE_TOP_HWSetBlackBar			= NULL;
		stBeTopHwFunction.BE_TOP_HWSetVideoMute			= NULL;
		stBeTopHwFunction.BE_TOP_HWSetDisplayType		= NULL;
		stBeTopHwFunction.BE_TOP_HWSetDebugDisplay		= NULL;
		stBeTopHwFunction.BE_TOP_HWSetVideoOperation		= NULL;
		stBeTopHwFunction.BE_TOP_HWSetInputSrcType		= NULL;
		stBeTopHwFunction.BE_TOP_HWSetActiveWindowStart	= NULL;
		stBeTopHwFunction.BE_TOP_HWSetActiveWindowSize	= NULL;
		stBeTopHwFunction.BE_TOP_HWSetRealCinema		= NULL;
		stBeTopHwFunction.BE_TOP_HWFirmwareDebug		= NULL;
		stBeTopHwFunction.BE_TOP_HWClose				= NULL;
		stBeTopHwFunction.BE_TOP_HWGet_FirmwareInfo_Addr= NULL;
		ret = RET_ERROR;
	}

	if(stBeTopHwFunction.BE_TOP_HWInitialize)
	{
		ret = stBeTopHwFunction.BE_TOP_HWInitialize();
	}

	return ret;
}

int BE_TOP_HAL_Close(void)
{
	int ret = RET_OK;

	if(stBeTopHwFunction.BE_TOP_HWClose)
	{
		ret = stBeTopHwFunction.BE_TOP_HWClose();
	}

	stBeTopHwFunction.BE_TOP_HWInitialize				= NULL;
	stBeTopHwFunction.BE_TOP_HWResetSW				= NULL;
	stBeTopHwFunction.BE_TOP_HWSetUartForMcu		= NULL;
	stBeTopHwFunction.BE_TOP_HWSetBypass			= NULL;
	stBeTopHwFunction.BE_TOP_HWGetBypass			= NULL;
	stBeTopHwFunction.BE_TOP_HWToggle3DLR			= NULL;
	stBeTopHwFunction.BE_TOP_HWSet3DInFormat		= NULL;
	stBeTopHwFunction.BE_TOP_HWGet3DInFormat		= NULL;
	stBeTopHwFunction.BE_TOP_HWSet3DOutFormat		= NULL;
	stBeTopHwFunction.BE_TOP_HWSet3DConvergence	= NULL;
	stBeTopHwFunction.BE_TOP_HWSetBlackBar			= NULL;
	stBeTopHwFunction.BE_TOP_HWSetVideoMute			= NULL;
	stBeTopHwFunction.BE_TOP_HWSetDisplayType		= NULL;
	stBeTopHwFunction.BE_TOP_HWSetDebugDisplay		= NULL;
	stBeTopHwFunction.BE_TOP_HWSetVideoOperation		= NULL;
	stBeTopHwFunction.BE_TOP_HWSetInputSrcType		= NULL;
	stBeTopHwFunction.BE_TOP_HWSetActiveWindowStart	= NULL;
	stBeTopHwFunction.BE_TOP_HWSetActiveWindowSize	= NULL;
	stBeTopHwFunction.BE_TOP_HWSetRealCinema		= NULL;
	stBeTopHwFunction.BE_TOP_HWFirmwareDebug		= NULL;
	stBeTopHwFunction.BE_TOP_HWClose				= NULL;
	stBeTopHwFunction.BE_TOP_HWGet_FirmwareInfo_Addr= NULL;

	return ret;
}

int BE_TOP_HAL_ResetSW(void)
{
	int ret = RET_OK;

	if(stBeTopHwFunction.BE_TOP_HWResetSW)
	{
		ret = stBeTopHwFunction.BE_TOP_HWResetSW();
	}
	else
	{
		if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_ERROR)
		{
			BE_ERROR("BE_TOP_HAL  ERROR! This function is not supported!\n");
			ret = RET_ERROR;
		}
		else if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_WARN)
		{
			BE_ERROR("BE_TOP_HAL  WARNING! This function is not supported!\n");
		}
	}

	return ret;
}

int BE_TOP_HAL_SetUartForMcu(BOOLEAN isForMcu)
{
	int ret = RET_OK;

	if(stBeTopHwFunction.BE_TOP_HWSetUartForMcu)
	{
		ret = stBeTopHwFunction.BE_TOP_HWSetUartForMcu(isForMcu);
	}
	else
	{
		if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_ERROR)
		{
			BE_ERROR("BE_TOP_HAL  ERROR! This function is not supported!\n");
			ret = RET_ERROR;
		}
		else if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_WARN)
		{
			BE_ERROR("BE_TOP_HAL  WARNING! This function is not supported!\n");
		}
	}

	return ret;
}

int BE_TOP_HAL_SetBypass(LX_BE_BLOCK_BYPASS_T *pstParams)
{
	int ret = RET_OK;

	if(stBeTopHwFunction.BE_TOP_HWSetBypass)
	{
		ret = stBeTopHwFunction.BE_TOP_HWSetBypass(pstParams);
	}
	else
	{
		if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_ERROR)
		{
			BE_ERROR("BE_TOP_HAL  ERROR! This function is not supported!\n");
			ret = RET_ERROR;
		}
		else if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_WARN)
		{
			BE_ERROR("BE_TOP_HAL  WARNING! This function is not supported!\n");
		}
	}

	return ret;
}

int BE_TOP_HAL_GetBypass(LX_BE_BLOCK_BYPASS_T *pstParams)
{
	int ret = RET_OK;

	if(stBeTopHwFunction.BE_TOP_HWGetBypass)
	{
		ret = stBeTopHwFunction.BE_TOP_HWGetBypass(pstParams);
	}
	else
	{
		if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_ERROR)
		{
			BE_ERROR("BE_TOP_HAL  ERROR! This function is not supported!\n");
			ret = RET_ERROR;
		}
		else if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_WARN)
		{
			BE_ERROR("BE_TOP_HAL  WARNING! This function is not supported!\n");
		}
	}

	return ret;
}

int BE_TOP_HAL_Toggle3DLR(LX_BE_3D_LR_T *pstParams)
{
	int ret = RET_OK;

	if(stBeTopHwFunction.BE_TOP_HWToggle3DLR)
	{
		ret = stBeTopHwFunction.BE_TOP_HWToggle3DLR(pstParams);
	}
	else
	{
		if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_ERROR)
		{
			BE_ERROR("BE_TOP_HAL  ERROR! This function is not supported!\n");
			ret = RET_ERROR;
		}
		else if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_WARN)
		{
			BE_ERROR("BE_TOP_HAL  WARNING! This function is not supported!\n");
		}
	}

	return ret;
}

int BE_TOP_HAL_Set3DInFormat(LX_BE_TOP_3D_IN_FORMAT_T *pstParams)
{
	int ret = RET_OK;

	if(stBeTopHwFunction.BE_TOP_HWSet3DInFormat)
	{
		ret = stBeTopHwFunction.BE_TOP_HWSet3DInFormat(pstParams);
	}
	else
	{
		if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_ERROR)
		{
			BE_ERROR("BE_TOP_HAL  ERROR! This function is not supported!\n");
			ret = RET_ERROR;
		}
		else if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_WARN)
		{
			BE_ERROR("BE_TOP_HAL  WARNING! This function is not supported!\n");
		}
	}

	return ret;
}

int BE_TOP_HAL_Get3DInFormat(LX_BE_TOP_3D_IN_FORMAT_T *pstParams)
{
	int ret = RET_OK;

	if(stBeTopHwFunction.BE_TOP_HWGet3DInFormat)
	{
		ret = stBeTopHwFunction.BE_TOP_HWGet3DInFormat(pstParams);
	}
	else
	{
		if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_ERROR)
		{
			BE_ERROR("BE_TOP_HAL  ERROR! This function is not supported!\n");
			ret = RET_ERROR;
		}
		else if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_WARN)
		{
			BE_ERROR("BE_TOP_HAL  WARNING! This function is not supported!\n");
		}
	}

	return ret;
}

int BE_TOP_HAL_Set3DOutFormat(LX_BE_3D_OUT_FORMAT_T *pstParams)
{
	int ret = RET_OK;

	if(stBeTopHwFunction.BE_TOP_HWSet3DOutFormat)
	{
		ret = stBeTopHwFunction.BE_TOP_HWSet3DOutFormat(pstParams);
	}
	else
	{
		if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_ERROR)
		{
			BE_ERROR("BE_TOP_HAL  ERROR! This function is not supported!\n");
			ret = RET_ERROR;
		}
		else if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_WARN)
		{
			BE_ERROR("BE_TOP_HAL  WARNING! This function is not supported!\n");
		}
	}

	return ret;
}

int BE_TOP_HAL_Set3DConvergence(LX_BE_3D_CONVERGENCE_T *pstParams)
{
	int ret = RET_OK;

	if(stBeTopHwFunction.BE_TOP_HWSet3DConvergence)
	{
		ret = stBeTopHwFunction.BE_TOP_HWSet3DConvergence(pstParams);
	}
	else
	{
		if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_ERROR)
		{
			BE_ERROR("BE_TOP_HAL  ERROR! This function is not supported!\n");
			ret = RET_ERROR;
		}
		else if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_WARN)
		{
			BE_ERROR("BE_TOP_HAL  WARNING! This function is not supported!\n");
		}
	}

	return ret;
}

int BE_TOP_HAL_SetBlackBar(LX_BE_BLACK_BAR_T *pstParams)
{
	int ret = RET_OK;

	if(stBeTopHwFunction.BE_TOP_HWSetBlackBar)
	{
		ret = stBeTopHwFunction.BE_TOP_HWSetBlackBar(pstParams);
	}
	else
	{
		if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_ERROR)
		{
			BE_ERROR("BE_TOP_HAL  ERROR! This function is not supported!\n");
			ret = RET_ERROR;
		}
		else if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_WARN)
		{
			BE_ERROR("BE_TOP_HAL  WARNING! This function is not supported!\n");
		}
	}

	return ret;
}

int BE_TOP_HAL_SetVideoMute(LX_BE_VIDEO_MUTE_T *pstParams)
{
	int ret = RET_OK;

	if(stBeTopHwFunction.BE_TOP_HWSetVideoMute)
	{
		ret = stBeTopHwFunction.BE_TOP_HWSetVideoMute(pstParams);
	}
	else
	{
		if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_ERROR)
		{
			BE_ERROR("BE_TOP_HAL  ERROR! This function is not supported!\n");
			ret = RET_ERROR;
		}
		else if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_WARN)
		{
			BE_ERROR("BE_TOP_HAL  WARNING! This function is not supported!\n");
		}
	}

	return ret;
}

int BE_TOP_HAL_SetDisplayType(LX_BE_DISPLAY_TYPE_T *pstParams)
{
	int ret = RET_OK;

	if(stBeTopHwFunction.BE_TOP_HWSetDisplayType)
	{
		ret = stBeTopHwFunction.BE_TOP_HWSetDisplayType(pstParams);
	}
	else
	{
		if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_ERROR)
		{
			BE_ERROR("BE_TOP_HAL  ERROR! This function is not supported!\n");
			ret = RET_ERROR;
		}
		else if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_WARN)
		{
			BE_ERROR("BE_TOP_HAL  WARNING! This function is not supported!\n");
		}
	}

	return ret;
}

int BE_TOP_HAL_SetDebugDisplay(LX_BE_DEBUG_DISPLAY_T *pstParams)
{
	int ret = RET_OK;

	if(stBeTopHwFunction.BE_TOP_HWSetDebugDisplay)
	{
		ret = stBeTopHwFunction.BE_TOP_HWSetDebugDisplay(pstParams);
	}
	else
	{
		if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_ERROR)
		{
			BE_ERROR("BE_TOP_HAL  ERROR! This function is not supported!\n");
			ret = RET_ERROR;
		}
		else if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_WARN)
		{
			BE_ERROR("BE_TOP_HAL  WARNING! This function is not supported!\n");
		}
	}

	return ret;
}

int BE_TOP_HAL_SetVideoOperation(LX_BE_VID_OPERATION_T *pstParams)
{
	int ret = RET_OK;

	if(stBeTopHwFunction.BE_TOP_HWSetVideoOperation)
	{
		ret = stBeTopHwFunction.BE_TOP_HWSetVideoOperation(pstParams);
	}
	else
	{
		if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_ERROR)
		{
			BE_ERROR("BE_TOP_HAL  ERROR! This function is not supported!\n");
			ret = RET_ERROR;
		}
		else if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_WARN)
		{
			BE_ERROR("BE_TOP_HAL  WARNING! This function is not supported!\n");
		}
	}

	return ret;
}

int BE_TOP_HAL_SetInputSrcType(LX_BE_SRC_TYPE_T *pstParams)
{
	int ret = RET_OK;

	if(stBeTopHwFunction.BE_TOP_HWSetInputSrcType)
	{
		ret = stBeTopHwFunction.BE_TOP_HWSetInputSrcType(pstParams);
	}
	else
	{
		if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_ERROR)
		{
			BE_ERROR("BE_TOP_HAL  ERROR! This function is not supported!\n");
			ret = RET_ERROR;
		}
		else if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_WARN)
		{
			BE_ERROR("BE_TOP_HAL  WARNING! This function is not supported!\n");
		}
	}

	return ret;
}

int BE_TOP_HAL_SetActiveWindowStart(LX_BE_ACTIVE_WINDOW_START_T *pstParams)
{
	int ret = RET_OK;

	if(stBeTopHwFunction.BE_TOP_HWSetActiveWindowStart)
	{
		ret = stBeTopHwFunction.BE_TOP_HWSetActiveWindowStart(pstParams);
	}
	else
	{
		if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_ERROR)
		{
			BE_ERROR("BE_TOP_HAL  ERROR! This function is not supported!\n");
			ret = RET_ERROR;
		}
		else if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_WARN)
		{
			BE_ERROR("BE_TOP_HAL  WARNING! This function is not supported!\n");
		}
	}

	return ret;
}

int BE_TOP_HAL_SetActiveWindowSize(LX_BE_ACTIVE_WINDOW_SIZE_T *pstParams)
{
	int ret = RET_OK;

	if(stBeTopHwFunction.BE_TOP_HWSetActiveWindowSize)
	{
		ret = stBeTopHwFunction.BE_TOP_HWSetActiveWindowSize(pstParams);
	}
	else
	{
		if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_ERROR)
		{
			BE_ERROR("BE_TOP_HAL  ERROR! This function is not supported!\n");
			ret = RET_ERROR;
		}
		else if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_WARN)
		{
			BE_ERROR("BE_TOP_HAL  WARNING! This function is not supported!\n");
		}
	}

	return ret;
}

int BE_TOP_HAL_SetRealCinema(LX_BE_REAL_CINEMA_T *pstParams)
{
	int ret = RET_OK;

	if(stBeTopHwFunction.BE_TOP_HWSetRealCinema)
	{
		ret = stBeTopHwFunction.BE_TOP_HWSetRealCinema(pstParams);
	}
	else
	{
		if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_ERROR)
		{
			BE_ERROR("BE_TOP_HAL  ERROR! This function is not supported!\n");
			ret = RET_ERROR;
		}
		else if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_WARN)
		{
			BE_ERROR("BE_TOP_HAL  WARNING! This function is not supported!\n");
		}
	}

	return ret;
}

int BE_TOP_HAL_FirmwareDebug(BE_FIRMWARE_DEBUG_T *pstParams)
{
	int ret = RET_OK;

	if(stBeTopHwFunction.BE_TOP_HWFirmwareDebug)
	{
		ret = stBeTopHwFunction.BE_TOP_HWFirmwareDebug(pstParams);
	}
	else
	{
		if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_ERROR)
		{
			BE_ERROR("BE_TOP_HAL  ERROR! This function is not supported!\n");
			ret = RET_ERROR;
		}
		else if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_WARN)
		{
			BE_ERROR("BE_TOP_HAL  WARNING! This function is not supported!\n");
		}
	}

	return ret;
}

int BE_TOP_HAL_Get_FirmwareInfo_Addr(BE_FIRMWARE_INFO_T *pstParams)
{
	int ret = RET_OK;

	if(stBeTopHwFunction.BE_TOP_HWGet_FirmwareInfo_Addr)
	{
		ret = stBeTopHwFunction.BE_TOP_HWGet_FirmwareInfo_Addr(pstParams);
	}
	else
	{
		pstParams->addr = NULL;
		pstParams->size = 0;
	}

	return ret;

}
/** @} */
