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
 *  date		2012.03.16
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
#include <linux/delay.h>

#include "os_util.h"
#include "be_dbg.h"

#include "be_kapi.h"
#include "h3d_hal.h"

#include "h13/be/h3d/be_h3d_hw_h13.h"
#include "h14/be/h3d/be_h3d_hw_h14.h"
#include "m14/be/h3d/be_h3d_hw_m14.h"
#include "h15/be/h3d/be_h3d_hw_h15.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/
typedef struct BE_H3D_DRV_CHIP
{

	int (*BE_H3D_HWGetVersion)(LX_BE_H3D_VERSION_T *pstParams);
	int	(*BE_H3D_HWResetBlock)(LX_BE_H3D_RESET_BLOCK_T *pstParams);
	int	(*BE_H3D_HWBypassAll)(LX_BE_H3D_BYPASS_ALL_T *pstParams);
	int	(*BE_H3D_HWSetMode)(LX_BE_H3D_MODE_T *pstParams);
	int	(*BE_H3D_HWGetMode)(LX_BE_H3D_MODE_T *pstParams);
	int	(*BE_H3D_HWInFormat)(LX_BE_H3D_IN_FORMAT_T *pstParams);
	int	(*BE_H3D_HWT3CDepthLevel)(LX_BE_H3D_T3C_DEPTH_LEVEL_T *pstParams);
	int	(*BE_H3D_HWA3CDepthLevel)(LX_BE_H3D_A3C_DEPTH_LEVEL_T *pstParams);
	int	(*BE_H3D_HWA3CAutoConvergence)(LX_BE_H3D_A3C_AUTOCONVERGENCE_T *pstParams);
	int	(*BE_H3D_HWSetTridFmT)(LX_BE_TRIDTV_FMT_CTRL_T *pstParams);

} BE_H3D_HW_FUNCTION_T;

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

BE_H3D_HW_FUNCTION_T stBeH3DHwFunction;

int BE_H3D_HAL_Init(void)
{
	int ret = RET_OK;

	if (0)
	{

	}
#ifdef INCLUDE_H15_CHIP_KDRV
	else if( lx_chip_rev( ) >= LX_CHIP_REV( H15, A0) )
	{
		BE_PRINT("BE H3D chip revision is set to H15 A0\n");
		stBeH3DHwFunction.BE_H3D_HWGetVersion	 			=		NULL;
		stBeH3DHwFunction.BE_H3D_HWResetBlock	 			=		NULL;
		stBeH3DHwFunction.BE_H3D_HWBypassAll				=		NULL;
		stBeH3DHwFunction.BE_H3D_HWSetMode		 		=		BE_H3D_HW_H15_Set_Mode;
		stBeH3DHwFunction.BE_H3D_HWGetMode 				=		NULL;
		stBeH3DHwFunction.BE_H3D_HWInFormat	 			=		NULL;
		stBeH3DHwFunction.BE_H3D_HWT3CDepthLevel	 		=		BE_H3D_HW_H15_T3C_Depth_Level;
		stBeH3DHwFunction.BE_H3D_HWA3CDepthLevel	 		=		NULL;
		stBeH3DHwFunction.BE_H3D_HWA3CAutoConvergence		=		NULL;
		stBeH3DHwFunction.BE_H3D_HWSetTridFmT 				= 		BE_H3D_HW_H15_Set_TridFmt;
	}
#endif
#ifdef INCLUDE_H14_CHIP_KDRV
	else if( lx_chip_rev( ) >= LX_CHIP_REV( H14, A0) )
	{
		BE_PRINT("BE H3D chip revision is set to H14 A0\n");
		stBeH3DHwFunction.BE_H3D_HWGetVersion	 			=		BE_H3D_HW_H14_Get_Version;
		stBeH3DHwFunction.BE_H3D_HWResetBlock	 			=		BE_H3D_HW_H14_Reset_Block;
		stBeH3DHwFunction.BE_H3D_HWBypassAll				=		BE_H3D_HW_H14_Bypass_All;
		stBeH3DHwFunction.BE_H3D_HWSetMode		 		=		BE_H3D_HW_H14_Set_Mode;
		stBeH3DHwFunction.BE_H3D_HWGetMode 				=		BE_H3D_HW_H14_Get_Mode;
		stBeH3DHwFunction.BE_H3D_HWInFormat	 			=		BE_H3D_HW_H14_In_Format;
		stBeH3DHwFunction.BE_H3D_HWT3CDepthLevel	 		=		BE_H3D_HW_H14_T3C_Depth_Level;
		stBeH3DHwFunction.BE_H3D_HWA3CDepthLevel	 		=		BE_H3D_HW_H14_A3C_Depth_Level;
		stBeH3DHwFunction.BE_H3D_HWA3CAutoConvergence	 	=		BE_H3D_HW_H14_A3C_AutoConvergence;
		stBeH3DHwFunction.BE_H3D_HWSetTridFmT 				= 		BE_H3D_HW_H14_Set_TridFmt;
	}
#endif
#ifdef INCLUDE_M14_CHIP_KDRV
	else if( lx_chip_rev( ) >= LX_CHIP_REV( M14, A0) )
	{
		BE_PRINT("BE H3D chip revision is set to M14 A0\n");
		stBeH3DHwFunction.BE_H3D_HWGetVersion	 			=		BE_H3D_HW_M14_Get_Version;
		stBeH3DHwFunction.BE_H3D_HWResetBlock	 			=		BE_H3D_HW_M14_Reset_Block;
		stBeH3DHwFunction.BE_H3D_HWBypassAll				=		BE_H3D_HW_M14_Bypass_All;
		stBeH3DHwFunction.BE_H3D_HWSetMode		 		=		BE_H3D_HW_M14_Set_Mode;
		stBeH3DHwFunction.BE_H3D_HWGetMode 				=		BE_H3D_HW_M14_Get_Mode;
		stBeH3DHwFunction.BE_H3D_HWInFormat	 			=		BE_H3D_HW_M14_In_Format;
		stBeH3DHwFunction.BE_H3D_HWT3CDepthLevel	 		=		BE_H3D_HW_M14_T3C_Depth_Level;
		stBeH3DHwFunction.BE_H3D_HWA3CDepthLevel	 		=		BE_H3D_HW_M14_A3C_Depth_Level;
		stBeH3DHwFunction.BE_H3D_HWA3CAutoConvergence	 	=		BE_H3D_HW_M14_A3C_AutoConvergence;
		stBeH3DHwFunction.BE_H3D_HWSetTridFmT 				= 		BE_H3D_HW_M14_Set_TridFmt;
	}
#endif
#ifdef INCLUDE_H13_CHIP_KDRV
	else if ( lx_chip_rev( ) >= LX_CHIP_REV( H13, A0) )
	{
		BE_PRINT("BE H3D chip revision is set to H13 A0\n");
		stBeH3DHwFunction.BE_H3D_HWGetVersion	 			=		BE_H3D_HW_H13_Get_Version;
		stBeH3DHwFunction.BE_H3D_HWResetBlock	 			=		BE_H3D_HW_H13_Reset_Block;
		stBeH3DHwFunction.BE_H3D_HWBypassAll				=		BE_H3D_HW_H13_Bypass_All;
		stBeH3DHwFunction.BE_H3D_HWSetMode		 		=		BE_H3D_HW_H13_Set_Mode;
		stBeH3DHwFunction.BE_H3D_HWGetMode 				=		BE_H3D_HW_H13_Get_Mode;
		stBeH3DHwFunction.BE_H3D_HWInFormat	 			=		BE_H3D_HW_H13_In_Format;
		stBeH3DHwFunction.BE_H3D_HWT3CDepthLevel	 		=		BE_H3D_HW_H13_T3C_Depth_Level;
		stBeH3DHwFunction.BE_H3D_HWA3CDepthLevel	 		=		BE_H3D_HW_H13_A3C_Depth_Level;
		stBeH3DHwFunction.BE_H3D_HWA3CAutoConvergence	 	=		BE_H3D_HW_H13_A3C_AutoConvergence;
		stBeH3DHwFunction.BE_H3D_HWSetTridFmT 				= 		BE_H3D_HW_H13_Set_TridFmt;
	}
#endif
	else
	{
		BE_ERROR("BE H3D ERROR! Unknown chip revision at BE FRC module\n");
		stBeH3DHwFunction.BE_H3D_HWGetVersion	 			=		NULL;
		stBeH3DHwFunction.BE_H3D_HWResetBlock	 			=		NULL;
		stBeH3DHwFunction.BE_H3D_HWBypassAll				=		NULL;
		stBeH3DHwFunction.BE_H3D_HWSetMode		 		=		NULL;
		stBeH3DHwFunction.BE_H3D_HWGetMode 				=		NULL;
		stBeH3DHwFunction.BE_H3D_HWInFormat	 			=		NULL;
		stBeH3DHwFunction.BE_H3D_HWT3CDepthLevel	 		=		NULL;
		stBeH3DHwFunction.BE_H3D_HWA3CDepthLevel	 		=		NULL;
		stBeH3DHwFunction.BE_H3D_HWA3CAutoConvergence		=		NULL;
		stBeH3DHwFunction.BE_H3D_HWSetTridFmT 				= 		NULL;

		ret = RET_ERROR;
	}

	return ret;
}

int BE_H3D_HAL_Get_Version(LX_BE_H3D_VERSION_T *pstParams)
{
	int ret = RET_OK;

	if(stBeH3DHwFunction.BE_H3D_HWGetVersion == NULL)
	{
		if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_ERROR)
		{
			BE_ERROR("BE_H3D_HAL  ERROR! This function is not supported!\n");
			ret = RET_ERROR;
		}
		else if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_WARN)
		{
			BE_ERROR("BE_H3D_HAL  WARNING! This function is not supported!\n");
		}
	}
	else
	{
		ret  = stBeH3DHwFunction.BE_H3D_HWGetVersion(pstParams);
	}

	return ret;
}

int BE_H3D_HAL_Reset_Block(LX_BE_H3D_RESET_BLOCK_T *pstParams)
{
	int ret = RET_OK;

	if(stBeH3DHwFunction.BE_H3D_HWResetBlock == NULL)
	{
		if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_ERROR)
		{
			BE_ERROR("BE_H3D_HAL  ERROR! This function is not supported!\n");
			ret = RET_ERROR;
		}
		else if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_WARN)
		{
			BE_ERROR("BE_H3D_HAL  WARNING! This function is not supported!\n");
		}
	}
	else
	{
		ret  = stBeH3DHwFunction.BE_H3D_HWResetBlock(pstParams);
	}

	return ret;
}

int BE_H3D_HAL_Bypass_All(LX_BE_H3D_BYPASS_ALL_T *pstParams)
{
	int ret = RET_OK;

	if(stBeH3DHwFunction.BE_H3D_HWBypassAll == NULL)
	{
		if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_ERROR)
		{
			BE_ERROR("BE_H3D_HAL  ERROR! This function is not supported!\n");
			ret = RET_ERROR;
		}
		else if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_WARN)
		{
			BE_ERROR("BE_H3D_HAL  WARNING! This function is not supported!\n");
		}
	}
	else
	{
		ret  = stBeH3DHwFunction.BE_H3D_HWBypassAll(pstParams);
	}

	return ret;
}

int BE_H3D_HAL_Set_Mode(LX_BE_H3D_MODE_T *pstParams)
{
	int ret = RET_OK;

	if(stBeH3DHwFunction.BE_H3D_HWSetMode == NULL)
	{
		if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_ERROR)
		{
			BE_ERROR("BE_H3D_HAL  ERROR! This function is not supported!\n");
			ret = RET_ERROR;
		}
		else if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_WARN)
		{
			BE_ERROR("BE_H3D_HAL  WARNING! This function is not supported!\n");
		}
	}
	else
	{
		ret  = stBeH3DHwFunction.BE_H3D_HWSetMode(pstParams);
	}

	return ret;
}

int BE_H3D_HAL_Get_Mode(LX_BE_H3D_MODE_T *pstParams)
{
	int ret = RET_OK;

	if(stBeH3DHwFunction.BE_H3D_HWGetMode == NULL)
	{
		if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_ERROR)
		{
			BE_ERROR("BE_H3D_HAL  ERROR! This function is not supported!\n");
			ret = RET_ERROR;
		}
		else if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_WARN)
		{
			BE_ERROR("BE_H3D_HAL  WARNING! This function is not supported!\n");
		}
	}
	else
	{
		ret  = stBeH3DHwFunction.BE_H3D_HWGetMode(pstParams);
	}

	return ret;
}

int BE_H3D_HAL_In_Format(LX_BE_H3D_IN_FORMAT_T *pstParams)
{
	int ret = RET_OK;

	if(stBeH3DHwFunction.BE_H3D_HWInFormat == NULL)
	{
		if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_ERROR)
		{
			BE_ERROR("BE_H3D_HAL  ERROR! This function is not supported!\n");
			ret = RET_ERROR;
		}
		else if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_WARN)
		{
			BE_ERROR("BE_H3D_HAL  WARNING! This function is not supported!\n");
		}
	}
	else
	{
		ret  = stBeH3DHwFunction.BE_H3D_HWInFormat(pstParams);
	}

	return ret;
}

int BE_H3D_HAL_T3C_Depth_Level(LX_BE_H3D_T3C_DEPTH_LEVEL_T *pstParams)
{
	int ret = RET_OK;

	if(stBeH3DHwFunction.BE_H3D_HWT3CDepthLevel == NULL)
	{
		if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_ERROR)
		{
			BE_ERROR("BE_H3D_HAL  ERROR! This function is not supported!\n");
			ret = RET_ERROR;
		}
		else if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_WARN)
		{
			BE_ERROR("BE_H3D_HAL  WARNING! This function is not supported!\n");
		}
	}
	else
	{
		ret  = stBeH3DHwFunction.BE_H3D_HWT3CDepthLevel(pstParams);
	}

	return ret;
}

int BE_H3D_HAL_A3C_Depth_Level(LX_BE_H3D_A3C_DEPTH_LEVEL_T *pstParams)
{
	int ret = RET_OK;

	if(stBeH3DHwFunction.BE_H3D_HWA3CDepthLevel == NULL)
	{
		if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_ERROR)
		{
			BE_ERROR("BE_H3D_HAL  ERROR! This function is not supported!\n");
			ret = RET_ERROR;
		}
		else if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_WARN)
		{
			BE_ERROR("BE_H3D_HAL  WARNING! This function is not supported!\n");
		}
	}
	else
	{
		ret  = stBeH3DHwFunction.BE_H3D_HWA3CDepthLevel(pstParams);
	}

	return ret;
}

int BE_H3D_HAL_A3C_AutoConvergence(LX_BE_H3D_A3C_AUTOCONVERGENCE_T *pstParams)
{
	int ret = RET_OK;

	if(stBeH3DHwFunction.BE_H3D_HWA3CAutoConvergence == NULL)
	{
		if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_ERROR)
		{
			BE_ERROR("BE_H3D_HAL  ERROR! This function is not supported!\n");
			ret = RET_ERROR;
		}
		else if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_WARN)
		{
			BE_ERROR("BE_H3D_HAL  WARNING! This function is not supported!\n");
		}
	}
	else
	{
		ret  = stBeH3DHwFunction.BE_H3D_HWA3CAutoConvergence(pstParams);
	}

	return ret;
}

int BE_H3D_HAL_Set_Trid_Fmt(LX_BE_TRIDTV_FMT_CTRL_T *pstParams)
{
	int ret = RET_OK;

	if(stBeH3DHwFunction.BE_H3D_HWSetTridFmT == NULL)
	{
		if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_ERROR)
		{
			BE_ERROR("BE_H3D_HAL  ERROR! This function is not supported!\n");
			ret = RET_ERROR;
		}
		else if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_WARN)
		{
			BE_ERROR("BE_H3D_HAL  WARNING! This function is not supported!\n");
		}
	}
	else
	{
		ret  = stBeH3DHwFunction.BE_H3D_HWSetTridFmT(pstParams);
	}

	return ret;
}

