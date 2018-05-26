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
#include "sre_hal.h"

#include "h13/be/sre/be_sre_hw_h13.h"
#include "h14/be/sre/be_sre_hw_h14.h"
#include "m14/be/sre/be_sre_hw_m14.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/
typedef struct BE_SRE_DRV_CHIP
{
	int 	(*pfnInitializeSRE) (void);
	int 	(*pfnCloseSRE) (void);
	int	(*pfnCommandSRE) (LX_BE_SRE_CMN_T *pstParams);
	int	(*pfnSetParamsSRE) (LX_BE_SRE_PRAM_T *pstParams);
	int	(*pfnGetVersionSRE) (LX_BE_SRE_VERSION_T *pstParams);
	int	(*pfnSetInputFormatSRE) (LX_BE_SRE_INPUT_MODE_T *pstParams);
} BE_SRE_HW_FUNCTION_T;

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/
BE_SRE_HW_FUNCTION_T gstBeSreHwFunction;

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/

/*========================================================================================
	Implementation Group
========================================================================================*/

int BE_SRE_HAL_Init(void)
{
	int ret = RET_OK;

	if (0)
	{

	}
#ifdef INCLUDE_H15_CHIP_KDRV
	else if ( lx_chip_rev( ) >= LX_CHIP_REV( H15, A0) )
	{
		BE_PRINT("BE SRE chip revision is set to H15 A0\n");
		gstBeSreHwFunction.pfnInitializeSRE		 =		NULL;
		gstBeSreHwFunction.pfnCloseSRE			=		NULL;
		gstBeSreHwFunction.pfnCommandSRE		=		NULL;
		gstBeSreHwFunction.pfnSetParamsSRE		=		NULL;
		gstBeSreHwFunction.pfnGetVersionSRE 		=		NULL;
		gstBeSreHwFunction.pfnSetInputFormatSRE	=		NULL;
	}
#endif
#ifdef INCLUDE_H14_CHIP_KDRV
	else if ( lx_chip_rev( ) >= LX_CHIP_REV( H14, A0) )
	{
		BE_PRINT("BE SRE chip revision is set to H14 A0\n");
		gstBeSreHwFunction.pfnInitializeSRE		 =		BE_SRE_HW_H14_Initialize;
		gstBeSreHwFunction.pfnCloseSRE			=		BE_SRE_HW_H14_Close;
		gstBeSreHwFunction.pfnCommandSRE		=		BE_SRE_HW_H14_Set_Command;
		gstBeSreHwFunction.pfnSetParamsSRE		=		BE_SRE_HW_H14_Set_Params;
		gstBeSreHwFunction.pfnGetVersionSRE 		=		BE_SRE_HW_H14_Get_Version;
		gstBeSreHwFunction.pfnSetInputFormatSRE	=		BE_SRE_HW_H14_Set_Input_Format;
	}
#endif
#ifdef INCLUDE_M14_CHIP_KDRV
	else if ( lx_chip_rev( ) >= LX_CHIP_REV( M14, A0) )
	{
		BE_PRINT("BE SRE chip revision is set to M14 A0\n");
		gstBeSreHwFunction.pfnInitializeSRE		 =		BE_SRE_HW_M14_Initialize;
		gstBeSreHwFunction.pfnCloseSRE			=		BE_SRE_HW_M14_Close;
		gstBeSreHwFunction.pfnCommandSRE		=		BE_SRE_HW_M14_Set_Command;
		gstBeSreHwFunction.pfnSetParamsSRE		=		BE_SRE_HW_M14_Set_Params;
		gstBeSreHwFunction.pfnGetVersionSRE 		=		BE_SRE_HW_M14_Get_Version;
		gstBeSreHwFunction.pfnSetInputFormatSRE	=		BE_SRE_HW_M14_Set_Input_Format;
	}
#endif
#ifdef INCLUDE_H13_CHIP_KDRV
	else if ( lx_chip_rev( ) >= LX_CHIP_REV( H13, A0) )
	{
		BE_PRINT("BE SRE chip revision is set to H13 A0\n");
		gstBeSreHwFunction.pfnInitializeSRE		 =		BE_SRE_HW_H13_Initialize;
		gstBeSreHwFunction.pfnCloseSRE			=		BE_SRE_HW_H13_Close;
		gstBeSreHwFunction.pfnCommandSRE		=		BE_SRE_HW_H13_Set_Command;
		gstBeSreHwFunction.pfnSetParamsSRE		=		BE_SRE_HW_H13_Set_Params;
		gstBeSreHwFunction.pfnGetVersionSRE 		=		BE_SRE_HW_H13_Get_Version;
		gstBeSreHwFunction.pfnSetInputFormatSRE	=		BE_SRE_HW_H13_Set_Input_Format;
	}
#endif
	else
	{
		BE_ERROR("BE SRE ERROR! Unknown chip revision at BE SRE module\n");
		gstBeSreHwFunction.pfnInitializeSRE		 =		NULL;
		gstBeSreHwFunction.pfnCloseSRE			=		NULL;
		gstBeSreHwFunction.pfnCommandSRE		=		NULL;
		gstBeSreHwFunction.pfnSetParamsSRE		=		NULL;
		gstBeSreHwFunction.pfnGetVersionSRE 		=		NULL;
		gstBeSreHwFunction.pfnSetInputFormatSRE	=		NULL;

		return RET_ERROR;
	}

	if (gstBeSreHwFunction.pfnInitializeSRE)
	{
		ret = gstBeSreHwFunction.pfnInitializeSRE();
	}

	return ret;
}

int BE_SRE_HAL_Close(void)
{
	int ret = RET_OK;

	if (gstBeSreHwFunction.pfnCloseSRE)
	{
		ret = gstBeSreHwFunction.pfnCloseSRE();
	}

	gstBeSreHwFunction.pfnInitializeSRE		=		NULL;
	gstBeSreHwFunction.pfnCloseSRE			=		NULL;
	gstBeSreHwFunction.pfnCommandSRE		=		NULL;
	gstBeSreHwFunction.pfnSetParamsSRE		=		NULL;
	gstBeSreHwFunction.pfnGetVersionSRE 		=		NULL;
	gstBeSreHwFunction.pfnSetInputFormatSRE	=		NULL;

	return ret;
}

int BE_SRE_HAL_Set_Command(LX_BE_SRE_CMN_T *pstParams)
{
	int ret = RET_OK;

	if (gstBeSreHwFunction.pfnCommandSRE == NULL)
	{
		if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_ERROR)
		{
			BE_ERROR("BE_SRE_HAL  ERROR! This function is not supported!\n");
			ret = RET_ERROR;
		}
		else if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_WARN)
		{
			BE_ERROR("BE_SRE_HAL  WARNING! This function is not supported!\n");
		}
	}
	else
	{
		ret  = gstBeSreHwFunction.pfnCommandSRE(pstParams);
	}

	return ret;
}

int BE_SRE_HAL_Set_Params(LX_BE_SRE_PRAM_T *pstParams)
{
	int ret = RET_OK;

	if (gstBeSreHwFunction.pfnSetParamsSRE == NULL)
	{
		if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_ERROR)
		{
			BE_ERROR("BE_SRE_HAL  ERROR! This function is not supported!\n");
			ret = RET_ERROR;
		}
		else if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_WARN)
		{
			BE_ERROR("BE_SRE_HAL  WARNING! This function is not supported!\n");
		}
	}
	else
	{
		ret  = gstBeSreHwFunction.pfnSetParamsSRE(pstParams);
	}

	return ret;
}

int  BE_SRE_HAL_Get_Version(LX_BE_SRE_VERSION_T *pstParams)
{
	int ret = RET_OK;

	if (gstBeSreHwFunction.pfnGetVersionSRE == NULL)
	{
		if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_ERROR)
		{
			BE_ERROR("BE_SRE_HAL  ERROR! This function is not supported!\n");
			ret = RET_ERROR;
		}
		else if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_WARN)
		{
			BE_ERROR("BE_SRE_HAL  WARNING! This function is not supported!\n");
		}
	}
	else
	{
		ret  = gstBeSreHwFunction.pfnGetVersionSRE(pstParams);
	}

	return ret;
}

int  BE_SRE_HAL_Set_Input_Format(LX_BE_SRE_INPUT_MODE_T *pstParams)
{
	int ret = RET_OK;

	if (gstBeSreHwFunction.pfnSetInputFormatSRE == NULL)
	{
		if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_ERROR)
		{
			BE_ERROR("BE_SRE_HAL  ERROR! This function is not supported!\n");
			ret = RET_ERROR;
		}
		else if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_WARN)
		{
			BE_ERROR("BE_SRE_HAL  WARNING! This function is not supported!\n");
		}
	}
	else
	{
		ret  = gstBeSreHwFunction.pfnSetInputFormatSRE(pstParams);
	}

	return ret;
}

