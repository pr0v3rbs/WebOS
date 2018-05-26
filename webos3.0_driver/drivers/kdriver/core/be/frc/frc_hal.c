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
 *  frc hal file for BE device
 *
 *  author		justine.jeong
 *  version		1.0
 *  date			2012.03.16
 *  note			Additional information.
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
#include <linux/delay.h>

#include "os_util.h"
#include "be_dbg.h"

#include "be_kapi.h"
#include "frc_hal.h"
#include "h13/be/frc/be_frc_hw_h13.h"
#include "h14/be/frc/be_frc_hw_h14.h"
#include "m14/be/frc/be_frc_hw_m14.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/
typedef struct BE_FRC_DRV_CHIP
{
	int	(*pfnInitializeFRC) (void);
	int	(*pfnCloseFRC) (void);
	int	(*pfnControlMemcFRC) (LX_BE_FRC_MEMC_MODE_T *pstParams);
	int	(*pfnSetDemoModeFRC) (LX_BE_FRC_DEMO_MODE_T *pstParams);
	int	(*pfnSetDebugBoxFRC) (LX_BE_FRC_ENABLE_DEBUG_BOX_T *pstParams);
	int	(*pfnSetBypassMemcRegFRC) (LX_BE_FRC_MEMC_BYPASS_REGION_T *pstParams);
	int	(*pfnSetYUV444ModeFRC) (LX_BE_FRC_COLOR_YUV444_T *pstParams);
	int	(*pfnSetLowDelayFRC) (LX_BE_FRC_LOW_DELAY_T *pstParams);
	int	(*pfnGetFirmwareVerFRC) (LX_BE_FRC_VERSION_T *pstParams);
	int	(*pfnForceFrameRateFRC) (LX_BE_FRC_FRAME_RATE_T *pstParams);
	int	(*pfnSetFreezeFRC) (LX_BE_FRC_FREEZE_T *pstParams);
	int (*pfnUpdateFrameDelay) (LX_BE_LOW_DELAY_T *pstParams);
} BE_FRC_HW_FUNCTION_T;

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/
BE_FRC_HW_FUNCTION_T gstBeFrcHwFunction;

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/

/*========================================================================================
	Implementation Group
========================================================================================*/

int BE_FRC_HAL_Init(void)
{
	int ret = RET_OK;

	if (0)
	{

	}
#ifdef INCLUDE_H15_CHIP_KDRV
	else if ( lx_chip_rev( ) >= LX_CHIP_REV( H15, A0) )
	{
		BE_PRINT("BE FRC chip revision is set to H15 A0\n");
		gstBeFrcHwFunction.pfnInitializeFRC				=		NULL;
		gstBeFrcHwFunction.pfnCloseFRC				=		NULL;
		gstBeFrcHwFunction.pfnControlMemcFRC			=		NULL;
		gstBeFrcHwFunction.pfnSetDemoModeFRC		=		NULL;
		gstBeFrcHwFunction.pfnSetDebugBoxFRC			=		NULL;
		gstBeFrcHwFunction.pfnSetBypassMemcRegFRC 	=		NULL;
		gstBeFrcHwFunction.pfnSetYUV444ModeFRC	 	=		NULL;
		gstBeFrcHwFunction.pfnSetLowDelayFRC			=		NULL;
		gstBeFrcHwFunction.pfnGetFirmwareVerFRC	 	=		NULL;
		gstBeFrcHwFunction.pfnForceFrameRateFRC	 	=		NULL;
		gstBeFrcHwFunction.pfnSetFreezeFRC		 	=		NULL;
		gstBeFrcHwFunction.pfnUpdateFrameDelay		= NULL;
	}
#endif
#ifdef INCLUDE_H14_CHIP_KDRV
	else if ( lx_chip_rev( ) >= LX_CHIP_REV( H14, A0) )
	{
		BE_PRINT("BE FRC chip revision is set to H14 A0\n");
		gstBeFrcHwFunction.pfnInitializeFRC				=		BE_FRC_HW_H14_Initialize;
		gstBeFrcHwFunction.pfnCloseFRC				=		BE_FRC_HW_H14_Close;
		gstBeFrcHwFunction.pfnControlMemcFRC			=		BE_FRC_HW_H14_ControlMemc;
		gstBeFrcHwFunction.pfnSetDemoModeFRC		=		BE_FRC_HW_H14_Set_Demo_Mode;
		gstBeFrcHwFunction.pfnSetDebugBoxFRC			=		BE_FRC_HW_H14_Set_Debug_Box;
		gstBeFrcHwFunction.pfnSetBypassMemcRegFRC 	=		BE_FRC_HW_H14_Set_Bypass_Memc_Reg;
		gstBeFrcHwFunction.pfnSetYUV444ModeFRC	 	=		BE_FRC_HW_H14_Set_YUV_444_Mode;
		gstBeFrcHwFunction.pfnSetLowDelayFRC			=		BE_FRC_HW_H14_Set_Low_Delay;
		gstBeFrcHwFunction.pfnGetFirmwareVerFRC	 	=		BE_FRC_HW_H14_Get_Firmware_Ver;
		gstBeFrcHwFunction.pfnForceFrameRateFRC	 	=		BE_FRC_HW_H14_Force_Frame_Rate;
		gstBeFrcHwFunction.pfnSetFreezeFRC		 	=		NULL;
		gstBeFrcHwFunction.pfnUpdateFrameDelay		= NULL;
	}
#endif
#ifdef INCLUDE_M14_CHIP_KDRV
	else if ( lx_chip_rev( ) >= LX_CHIP_REV( M14, A0) )
	{
		BE_PRINT("BE FRC chip revision is set to M14 A0\n");
		gstBeFrcHwFunction.pfnInitializeFRC		 		=		BE_FRC_HW_M14_Initialize;
		gstBeFrcHwFunction.pfnCloseFRC				=		BE_FRC_HW_M14_Close;
		gstBeFrcHwFunction.pfnControlMemcFRC			=		BE_FRC_HW_M14_ControlMemc;
		gstBeFrcHwFunction.pfnSetDemoModeFRC	 	=		BE_FRC_HW_M14_Set_Demo_Mode;
		gstBeFrcHwFunction.pfnSetDebugBoxFRC			=		BE_FRC_HW_M14_Set_Debug_Box;
		gstBeFrcHwFunction.pfnSetBypassMemcRegFRC 	=		BE_FRC_HW_M14_Set_Bypass_Memc_Reg;
		gstBeFrcHwFunction.pfnSetYUV444ModeFRC	 	=		BE_FRC_HW_M14_Set_YUV_444_Mode;
		gstBeFrcHwFunction.pfnSetLowDelayFRC			=		BE_FRC_HW_M14_Set_Low_Delay;
		gstBeFrcHwFunction.pfnGetFirmwareVerFRC	 	=		BE_FRC_HW_M14_Get_Firmware_Ver;
		gstBeFrcHwFunction.pfnForceFrameRateFRC	 	=		BE_FRC_HW_M14_Force_Frame_Rate;
		gstBeFrcHwFunction.pfnSetFreezeFRC		 	=		BE_FRC_HW_M14_Set_Freeze;
		gstBeFrcHwFunction.pfnUpdateFrameDelay		= BE_FRC_HW_M14_Update_FrameDelay;
	}
#endif
#ifdef INCLUDE_H13_CHIP_KDRV
	else if ( lx_chip_rev( ) >= LX_CHIP_REV( H13, A0) )
	{
		BE_PRINT("BE FRC chip revision is set to H13 A0\n");
		gstBeFrcHwFunction.pfnInitializeFRC				=		BE_FRC_HW_H13_Initialize;
		gstBeFrcHwFunction.pfnCloseFRC				=		BE_FRC_HW_H13_Close;
		gstBeFrcHwFunction.pfnControlMemcFRC			=		BE_FRC_HW_H13_ControlMemc;
		gstBeFrcHwFunction.pfnSetDemoModeFRC	 	=		BE_FRC_HW_H13_Set_Demo_Mode;
		gstBeFrcHwFunction.pfnSetDebugBoxFRC			=		BE_FRC_HW_H13_Set_Debug_Box;
		gstBeFrcHwFunction.pfnSetBypassMemcRegFRC	=		BE_FRC_HW_H13_Set_Bypass_Memc_Reg;
		gstBeFrcHwFunction.pfnSetYUV444ModeFRC	 	=		BE_FRC_HW_H13_Set_YUV_444_Mode;
		gstBeFrcHwFunction.pfnSetLowDelayFRC			=		BE_FRC_HW_H13_Set_Low_Delay;
		gstBeFrcHwFunction.pfnGetFirmwareVerFRC	 	=		BE_FRC_HW_H13_Get_Firmware_Ver;
		gstBeFrcHwFunction.pfnForceFrameRateFRC	 	=		BE_FRC_HW_H13_Force_Frame_Rate;
		gstBeFrcHwFunction.pfnSetFreezeFRC		 	=		NULL;
		gstBeFrcHwFunction.pfnUpdateFrameDelay		= NULL;
	}
#endif
	else
	{
		BE_ERROR("BE FRC ERROR! Unknown chip revision at BE FRC module\n");
		gstBeFrcHwFunction.pfnInitializeFRC				=		NULL;
		gstBeFrcHwFunction.pfnCloseFRC				=		NULL;
		gstBeFrcHwFunction.pfnControlMemcFRC			=		NULL;
		gstBeFrcHwFunction.pfnSetDemoModeFRC		=		NULL;
		gstBeFrcHwFunction.pfnSetDebugBoxFRC			=		NULL;
		gstBeFrcHwFunction.pfnSetBypassMemcRegFRC 	=		NULL;
		gstBeFrcHwFunction.pfnSetYUV444ModeFRC	 	=		NULL;
		gstBeFrcHwFunction.pfnSetLowDelayFRC			=		NULL;
		gstBeFrcHwFunction.pfnGetFirmwareVerFRC	 	=		NULL;
		gstBeFrcHwFunction.pfnForceFrameRateFRC	 	=		NULL;
		gstBeFrcHwFunction.pfnSetFreezeFRC		 	=		NULL;
		gstBeFrcHwFunction.pfnUpdateFrameDelay		= NULL;
		return RET_ERROR;
	}

	if (gstBeFrcHwFunction.pfnInitializeFRC)
	{
		ret = gstBeFrcHwFunction.pfnInitializeFRC();
	}

	return ret;
}

int BE_FRC_HAL_Close(void)
{
	int ret = RET_OK;

	if (gstBeFrcHwFunction.pfnCloseFRC)
	{
		ret = gstBeFrcHwFunction.pfnCloseFRC();
	}

	gstBeFrcHwFunction.pfnInitializeFRC		 		=		NULL;
	gstBeFrcHwFunction.pfnCloseFRC				=		NULL;
	gstBeFrcHwFunction.pfnControlMemcFRC			=		NULL;
	gstBeFrcHwFunction.pfnSetDemoModeFRC		=		NULL;
	gstBeFrcHwFunction.pfnSetDebugBoxFRC			=		NULL;
	gstBeFrcHwFunction.pfnSetBypassMemcRegFRC 	=		NULL;
	gstBeFrcHwFunction.pfnSetYUV444ModeFRC	 	=		NULL;
	gstBeFrcHwFunction.pfnSetLowDelayFRC			=		NULL;
	gstBeFrcHwFunction.pfnGetFirmwareVerFRC	 	=		NULL;
	gstBeFrcHwFunction.pfnForceFrameRateFRC	 	=		NULL;

	return ret;
}

int BE_FRC_HAL_ControlMemc(LX_BE_FRC_MEMC_MODE_T *pstParams)
{
	int ret = RET_OK;

	if (gstBeFrcHwFunction.pfnControlMemcFRC == NULL)
	{
		if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_ERROR)
		{
			BE_ERROR("BE_FRC_HAL  ERROR! This function is not supported!\n");
			ret = RET_ERROR;
		}
		else if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_WARN)
		{
			BE_ERROR("BE_FRC_HAL  WARNING! This function is not supported!\n");
		}
	}
	else
	{
		ret  = gstBeFrcHwFunction.pfnControlMemcFRC(pstParams);
	}

	return ret;
}

int BE_FRC_HAL_Set_Demo_Mode(LX_BE_FRC_DEMO_MODE_T *pstParams)
{
	int ret = RET_OK;

	if (gstBeFrcHwFunction.pfnSetDemoModeFRC == NULL)
	{
		if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_ERROR)
		{
			BE_ERROR("BE_FRC_HAL  ERROR! This function is not supported!\n");
			ret = RET_ERROR;
		}
		else if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_WARN)
		{
			BE_ERROR("BE_FRC_HAL  WARNING! This function is not supported!\n");
		}
	}
	else
	{
		ret  = gstBeFrcHwFunction.pfnSetDemoModeFRC(pstParams);
	}

	return ret;
}

int BE_FRC_HAL_Set_Debug_Box(LX_BE_FRC_ENABLE_DEBUG_BOX_T *pstParams)
{
	int ret = RET_OK;

	if (gstBeFrcHwFunction.pfnSetDebugBoxFRC == NULL)
	{
		if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_ERROR)
		{
			BE_ERROR("BE_FRC_HAL  ERROR! This function is not supported!\n");
			ret = RET_ERROR;
		}
		else if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_WARN)
		{
			BE_ERROR("BE_FRC_HAL  WARNING! This function is not supported!\n");
		}
	}
	else
	{
		ret  = gstBeFrcHwFunction.pfnSetDebugBoxFRC(pstParams);
	}

	return ret;
}

int BE_FRC_HAL_Set_Bypass_Memc_Reg(LX_BE_FRC_MEMC_BYPASS_REGION_T *pstParams)
{
	int ret = RET_OK;

	if (gstBeFrcHwFunction.pfnSetBypassMemcRegFRC == NULL)
	{
		if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_ERROR)
		{
			BE_ERROR("BE_FRC_HAL  ERROR! This function is not supported!\n");
			ret = RET_ERROR;
		}
		else if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_WARN)
		{
			BE_ERROR("BE_FRC_HAL  WARNING! This function is not supported!\n");
		}
	}
	else
	{
		ret  = gstBeFrcHwFunction.pfnSetBypassMemcRegFRC(pstParams);
	}

	return ret;
}

int BE_FRC_HAL_Set_YUV_444_Mode(LX_BE_FRC_COLOR_YUV444_T *pstParams)
{
	int ret = RET_OK;

	if (gstBeFrcHwFunction.pfnSetYUV444ModeFRC == NULL)
	{
		if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_ERROR)
		{
			BE_ERROR("BE_FRC_HAL  ERROR! This function is not supported!\n");
			ret = RET_ERROR;
		}
		else if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_WARN)
		{
			BE_ERROR("BE_FRC_HAL  WARNING! This function is not supported!\n");
		}
	}
	else
	{
		ret  = gstBeFrcHwFunction.pfnSetYUV444ModeFRC(pstParams);
	}

	return ret;
}

int BE_FRC_HAL_Set_Low_Delay(LX_BE_FRC_LOW_DELAY_T *pstParams)
{
	int ret = RET_OK;

	if (gstBeFrcHwFunction.pfnSetLowDelayFRC == NULL)
	{
		if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_ERROR)
		{
			BE_ERROR("BE_FRC_HAL  ERROR! This function is not supported!\n");
			ret = RET_ERROR;
		}
		else if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_WARN)
		{
			BE_ERROR("BE_FRC_HAL  WARNING! This function is not supported!\n");
		}
	}
	else
	{
		ret  = gstBeFrcHwFunction.pfnSetLowDelayFRC(pstParams);
	}

	return ret;
}

int BE_FRC_HAL_Get_Firmware_Ver(LX_BE_FRC_VERSION_T *pstParams)
{
	int ret = RET_OK;

	if (gstBeFrcHwFunction.pfnGetFirmwareVerFRC == NULL)
	{
		if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_ERROR)
		{
			BE_ERROR("BE_FRC_HAL  ERROR! This function is not supported!\n");
			ret = RET_ERROR;
		}
		else if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_WARN)
		{
			BE_ERROR("BE_FRC_HAL  WARNING! This function is not supported!\n");
		}
	}
	else
	{
		ret  = gstBeFrcHwFunction.pfnGetFirmwareVerFRC(pstParams);
	}

	return ret;
}

int BE_FRC_HAL_Force_Frame_Rate(LX_BE_FRC_FRAME_RATE_T *pstParams)
{
	int ret = RET_OK;

	if (gstBeFrcHwFunction.pfnForceFrameRateFRC == NULL)
	{
		if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_ERROR)
		{
			BE_ERROR("BE_FRC_HAL  ERROR! This function is not supported!\n");
			ret = RET_ERROR;
		}
		else if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_WARN)
		{
			BE_ERROR("BE_FRC_HAL  WARNING! This function is not supported!\n");
		}
	}
	else
	{
		ret  = gstBeFrcHwFunction.pfnForceFrameRateFRC(pstParams);
	}

	return ret;
}

int BE_FRC_HAL_Set_Freeze(LX_BE_FRC_FREEZE_T *pstParams)
{
	int ret = RET_OK;

	if (gstBeFrcHwFunction.pfnSetFreezeFRC == NULL)
	{
		if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_ERROR)
		{
			BE_ERROR("BE_FRC_HAL  ERROR! This function is not supported!\n");
			ret = RET_ERROR;
		}
		else if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_WARN)
		{
			BE_ERROR("BE_FRC_HAL  WARNING! This function is not supported!\n");
		}
	}
	else
	{
		ret  = gstBeFrcHwFunction.pfnSetFreezeFRC(pstParams);
	}

	return ret;
}

int BE_FRC_HAL_Update_FrameDelay(LX_BE_LOW_DELAY_T *pstParams)
{
	int ret = RET_OK;

	if (gstBeFrcHwFunction.pfnUpdateFrameDelay == NULL)
	{
		if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_ERROR)
		{
			BE_ERROR("BE_FRC_HAL  ERROR! This function is not supported!\n");
			ret = RET_ERROR;
		}
		else if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_WARN)
		{
			BE_ERROR("BE_FRC_HAL  WARNING! This function is not supported!\n");
		}
	}
	else
	{
		ret  = gstBeFrcHwFunction.pfnUpdateFrameDelay(pstParams);
	}

	return ret;
}
