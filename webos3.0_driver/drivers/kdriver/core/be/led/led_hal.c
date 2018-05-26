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
#include "led_hal.h"
#include "h13/be/led/be_led_hw_h13.h"
#include "h14/be/led/be_led_hw_h14.h"
#include "m14/be/led/be_led_hw_m14.h"
#include "h15/be/led/be_led_hw_h15.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/
typedef struct BE_LED_DRV_CHIP
{
	int 	(*pfnInitialize) (void);
	int 	(*pfnClose) (void);
	int 	(*pfnResume) (void);

	int	(*pfnGetVersionLED) (LX_BE_LED_VERSION_T *pstParams);
	int	(*pfnSetModeLED) (LX_BE_LED_MODE_T *pstParams);
	int	(*pfnControlLED) (LX_BE_LED_CONTROL_T *pstParams);
	int	(*pfnGetAplLED) (LX_BE_LED_APL_T *pstParams);
	int	(*pfnSetClippingValueLED) (UINT32 u32Param);
	int	(*pfnGetRegInfoLED) (LX_BE_LED_REG_INFO_T *pstParams);

	int	(*pfnGetVersionHDR) (LX_BE_HDR_VERSION_T *pstParams);
	int	(*pfnSetModeHDR) (LX_BE_HDR_MODE_T *pstParams);
	int	(*pfnControlHDR) (LX_BE_HDR_CONTROL_T *pstParams);

	int	(*pfnGetVersionWCG) (LX_BE_WCG_VERSION_T *pstParams);
	int	(*pfnSetModeWCG) (LX_BE_WCG_MODE_T *pstParams);
	int	(*pfnControlWCG) (LX_BE_WCG_CONTROL_T *pstParams);
} BE_LED_HW_FUNCTION_T;

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/
BE_LED_HW_FUNCTION_T gstBeLedHwFunction;

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/

/*========================================================================================
	Implementation Group
========================================================================================*/

int BE_LED_HAL_Init(void)
{
	int ret = RET_OK;

	if (0)
	{

	}
#ifdef INCLUDE_H15_CHIP_KDRV
	else if ( lx_chip_rev( ) >= LX_CHIP_REV( H15, A0) )
	{
		BE_PRINT("BE LED chip revision is set to H15 A0\n");
		gstBeLedHwFunction.pfnInitialize		 	=		NULL;
		gstBeLedHwFunction.pfnClose				=		NULL;
		gstBeLedHwFunction.pfnResume			=		NULL;

		gstBeLedHwFunction.pfnGetVersionLED		=		NULL;
		gstBeLedHwFunction.pfnSetModeLED		=		NULL;
		gstBeLedHwFunction.pfnControlLED 			=		NULL;
		gstBeLedHwFunction.pfnGetAplLED 			=		NULL;
		gstBeLedHwFunction.pfnSetClippingValueLED 	=		NULL;
		gstBeLedHwFunction.pfnGetRegInfoLED 		=		NULL;

		gstBeLedHwFunction.pfnGetVersionHDR	 	=		NULL;
		gstBeLedHwFunction.pfnSetModeHDR	 	=		NULL;
		gstBeLedHwFunction.pfnControlHDR		 	=		NULL;

		gstBeLedHwFunction.pfnGetVersionWCG		=		NULL;
		gstBeLedHwFunction.pfnSetModeWCG	 	=		BE_WCG_HW_H15_Set_Mode;
		gstBeLedHwFunction.pfnControlWCG	 	=		BE_WCG_HW_H15_Control;
	}
#endif
#ifdef INCLUDE_H14_CHIP_KDRV
	else if ( lx_chip_rev( ) >= LX_CHIP_REV( H14, A0) )
	{
		BE_PRINT("BE LED chip revision is set to H14 A0\n");
		gstBeLedHwFunction.pfnInitialize		 	=		BE_LED_HW_H14_Initialize;
		gstBeLedHwFunction.pfnClose				=		BE_LED_HW_H14_Close;
		gstBeLedHwFunction.pfnResume			=		BE_LED_HW_H14_Resume;

		gstBeLedHwFunction.pfnGetVersionLED		=		BE_LED_HW_H14_Get_Version;
		gstBeLedHwFunction.pfnSetModeLED		=		BE_LED_HW_H14_Set_Mode;
		gstBeLedHwFunction.pfnControlLED 			=		BE_LED_HW_H14_Control;
		gstBeLedHwFunction.pfnGetAplLED 			=		NULL;
		gstBeLedHwFunction.pfnSetClippingValueLED 	=		NULL;
		gstBeLedHwFunction.pfnGetRegInfoLED 		=		BE_LED_HW_H14_Get_RegInfo;

		gstBeLedHwFunction.pfnGetVersionHDR	 	=		BE_HDR_HW_H14_Get_Version;
		gstBeLedHwFunction.pfnSetModeHDR	 	=		BE_HDR_HW_H14_Set_Mode;
		gstBeLedHwFunction.pfnControlHDR		 	=		BE_HDR_HW_H14_Control;

		gstBeLedHwFunction.pfnGetVersionWCG		=		BE_WCG_HW_H14_Get_Version;
		gstBeLedHwFunction.pfnSetModeWCG	 	=		BE_WCG_HW_H14_Set_Mode;
		gstBeLedHwFunction.pfnControlWCG	 	=		BE_WCG_HW_H14_Control;
	}
#endif
#ifdef INCLUDE_M14_CHIP_KDRV
	else if ( lx_chip_rev( ) >= LX_CHIP_REV( M14, A0) )
	{
		BE_PRINT("BE LED chip revision is set to M14 A0\n");
		gstBeLedHwFunction.pfnInitialize		 	=		BE_LED_HW_M14_Initialize;
		gstBeLedHwFunction.pfnClose				=		BE_LED_HW_M14_Close;
		gstBeLedHwFunction.pfnResume			=		BE_LED_HW_M14_Resume;

		gstBeLedHwFunction.pfnGetVersionLED		=		BE_LED_HW_M14_Get_Version;
		gstBeLedHwFunction.pfnSetModeLED		=		BE_LED_HW_M14_Set_Mode;
		gstBeLedHwFunction.pfnControlLED 			=		BE_LED_HW_M14_Control;
		gstBeLedHwFunction.pfnGetAplLED 			=		BE_LED_HW_M14_Get_Apl;
		gstBeLedHwFunction.pfnSetClippingValueLED 	=		BE_LED_HW_M14_Set_ClippingValue;
		gstBeLedHwFunction.pfnGetRegInfoLED 		=		BE_LED_HW_M14_Get_RegInfo;

		gstBeLedHwFunction.pfnGetVersionHDR	 	=		BE_HDR_HW_M14_Get_Version;
		gstBeLedHwFunction.pfnSetModeHDR	 	=		BE_HDR_HW_M14_Set_Mode;
		gstBeLedHwFunction.pfnControlHDR		 	=		BE_HDR_HW_M14_Control;

		gstBeLedHwFunction.pfnGetVersionWCG		=		BE_WCG_HW_M14_Get_Version;
		gstBeLedHwFunction.pfnSetModeWCG	 	=		BE_WCG_HW_M14_Set_Mode;
		gstBeLedHwFunction.pfnControlWCG	 	=		BE_WCG_HW_M14_Control;
	}
#endif
#ifdef INCLUDE_H13_CHIP_KDRV
	else if ( lx_chip_rev( ) >= LX_CHIP_REV( H13, A0) )
	{
		BE_PRINT("BE LED chip revision is set to H13 A0\n");
		gstBeLedHwFunction.pfnInitialize		 	=		BE_LED_HW_H13_Initialize;
		gstBeLedHwFunction.pfnClose				=		BE_LED_HW_H13_Close;
		gstBeLedHwFunction.pfnResume			=		BE_LED_HW_H13_Resume;

		gstBeLedHwFunction.pfnGetVersionLED		=		BE_LED_HW_H13_Get_Version;
		gstBeLedHwFunction.pfnSetModeLED		=		BE_LED_HW_H13_Set_Mode;
		gstBeLedHwFunction.pfnControlLED 			=		BE_LED_HW_H13_Control;
		gstBeLedHwFunction.pfnGetAplLED 			=		NULL;
		gstBeLedHwFunction.pfnSetClippingValueLED 	=		NULL;
		gstBeLedHwFunction.pfnGetRegInfoLED 		=		BE_LED_HW_H13_Get_RegInfo;

		gstBeLedHwFunction.pfnGetVersionHDR	 	=		BE_HDR_HW_H13_Get_Version;
		gstBeLedHwFunction.pfnSetModeHDR	 	=		BE_HDR_HW_H13_Set_Mode;
		gstBeLedHwFunction.pfnControlHDR		 	=		BE_HDR_HW_H13_Control;

		gstBeLedHwFunction.pfnGetVersionWCG		=		BE_WCG_HW_H13_Get_Version;
		gstBeLedHwFunction.pfnSetModeWCG	 	=		BE_WCG_HW_H13_Set_Mode;
		gstBeLedHwFunction.pfnControlWCG	 	=		BE_WCG_HW_H13_Control;
	}
#endif
	else
	{
		BE_ERROR("BE LED ERROR! Unknown chip revision at BE LED module\n");
		gstBeLedHwFunction.pfnInitialize		 	=		NULL;
		gstBeLedHwFunction.pfnClose				=		NULL;
		gstBeLedHwFunction.pfnResume			=		NULL;

		gstBeLedHwFunction.pfnGetVersionLED		=		NULL;
		gstBeLedHwFunction.pfnSetModeLED		=		NULL;
		gstBeLedHwFunction.pfnControlLED 			=		NULL;
		gstBeLedHwFunction.pfnGetAplLED 			=		NULL;
		gstBeLedHwFunction.pfnSetClippingValueLED 	=		NULL;
		gstBeLedHwFunction.pfnGetRegInfoLED 		=		NULL;

		gstBeLedHwFunction.pfnGetVersionHDR	 	=		NULL;
		gstBeLedHwFunction.pfnSetModeHDR	 	=		NULL;
		gstBeLedHwFunction.pfnControlHDR		 	=		NULL;

		gstBeLedHwFunction.pfnGetVersionWCG		=		NULL;
		gstBeLedHwFunction.pfnSetModeWCG	 	=		NULL;
		gstBeLedHwFunction.pfnControlWCG	 	=		NULL;

		return RET_ERROR;
	}

	if (gstBeLedHwFunction.pfnInitialize)
	{
		ret = gstBeLedHwFunction.pfnInitialize();
	}

	return ret;
}

int BE_LED_HAL_Close(void)
{
	int ret = RET_OK;

	if (gstBeLedHwFunction.pfnClose)
	{
		ret = gstBeLedHwFunction.pfnClose();
	}

	gstBeLedHwFunction.pfnInitialize		 	=		NULL;
	gstBeLedHwFunction.pfnClose				=		NULL;
	gstBeLedHwFunction.pfnResume			=		NULL;

	gstBeLedHwFunction.pfnGetVersionLED		=		NULL;
	gstBeLedHwFunction.pfnSetModeLED		=		NULL;
	gstBeLedHwFunction.pfnControlLED 			=		NULL;
	gstBeLedHwFunction.pfnGetAplLED 			=		NULL;
	gstBeLedHwFunction.pfnSetClippingValueLED 	=		NULL;
	gstBeLedHwFunction.pfnGetRegInfoLED 		=		NULL;

	gstBeLedHwFunction.pfnGetVersionHDR	 	=		NULL;
	gstBeLedHwFunction.pfnSetModeHDR	 	=		NULL;
	gstBeLedHwFunction.pfnControlHDR		 	=		NULL;

	gstBeLedHwFunction.pfnGetVersionWCG		=		NULL;
	gstBeLedHwFunction.pfnSetModeWCG	 	=		NULL;
	gstBeLedHwFunction.pfnControlWCG	 	=		NULL;

	return ret;
}

int BE_LED_HAL_Resume(void)
{
	int ret = RET_OK;

	if (gstBeLedHwFunction.pfnResume == NULL)
	{
		if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_ERROR)
		{
			BE_ERROR("BE_LED_HAL  ERROR! This function is not supported!\n");
			ret = RET_ERROR;
		}
		else if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_WARN)
		{
			BE_ERROR("BE_LED_HAL  WARNING! This function is not supported!\n");
		}
	}
	else
	{
		ret  = gstBeLedHwFunction.pfnResume();
	}

	return ret;
}

int BE_LED_HAL_Get_Version(LX_BE_LED_VERSION_T *pstParams)
{
	int ret = RET_OK;

	if (gstBeLedHwFunction.pfnGetVersionLED == NULL)
	{
		if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_ERROR)
		{
			BE_ERROR("BE_LED_HAL  ERROR! This function is not supported!\n");
			ret = RET_ERROR;
		}
		else if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_WARN)
		{
			BE_ERROR("BE_LED_HAL  WARNING! This function is not supported!\n");
		}
	}
	else
	{
		ret  = gstBeLedHwFunction.pfnGetVersionLED(pstParams);
	}

	return ret;
}

int BE_LED_HAL_Set_Mode(LX_BE_LED_MODE_T *pstParams)
{
	int ret = RET_OK;

	if (gstBeLedHwFunction.pfnSetModeLED == NULL)
	{
		if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_ERROR)
		{
			BE_ERROR("BE_LED_HAL  ERROR! This function is not supported!\n");
			ret = RET_ERROR;
		}
		else if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_WARN)
		{
			BE_ERROR("BE_LED_HAL  WARNING! This function is not supported!\n");
		}
	}
	else
	{
		ret  = gstBeLedHwFunction.pfnSetModeLED(pstParams);
	}

	return ret;
}

int BE_LED_HAL_Control(LX_BE_LED_CONTROL_T *pstParams)
{
	int ret = RET_OK;

	if (gstBeLedHwFunction.pfnControlLED == NULL)
	{
		if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_ERROR)
		{
			BE_ERROR("BE_LED_HAL  ERROR! This function is not supported!\n");
			ret = RET_ERROR;
		}
		else if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_WARN)
		{
			BE_ERROR("BE_LED_HAL  WARNING! This function is not supported!\n");
		}
	}
	else
	{
		ret  = gstBeLedHwFunction.pfnControlLED(pstParams);
	}

	return ret;
}

int BE_LED_HAL_Get_Apl(LX_BE_LED_APL_T *pstParams)
{
	int ret = RET_OK;

	if (gstBeLedHwFunction.pfnGetAplLED == NULL)
	{
		if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_ERROR)
		{
			BE_ERROR("BE_LED_HAL  ERROR! This function is not supported!\n");
			ret = RET_ERROR;
		}
		else if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_WARN)
		{
			BE_ERROR("BE_LED_HAL  WARNING! This function is not supported!\n");
		}
	}
	else
	{
		ret  = gstBeLedHwFunction.pfnGetAplLED(pstParams);
	}

	return ret;
}

int BE_LED_HAL_Set_ClippingValue(UINT32 u32Param)
{
	int ret = RET_OK;

	if (gstBeLedHwFunction.pfnSetClippingValueLED == NULL)
	{
		if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_ERROR)
		{
			BE_ERROR("BE_LED_HAL  ERROR! This function is not supported!\n");
			ret = RET_ERROR;
		}
		else if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_WARN)
		{
			BE_ERROR("BE_LED_HAL  WARNING! This function is not supported!\n");
		}
	}
	else
	{
		ret  = gstBeLedHwFunction.pfnSetClippingValueLED(u32Param);
	}

	return ret;
}

int BE_LED_HAL_Get_RegInfo(LX_BE_LED_REG_INFO_T *pstParams)
{
	int ret = RET_OK;

	if (gstBeLedHwFunction.pfnGetRegInfoLED == NULL)
	{
		if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_ERROR)
		{
			BE_ERROR("BE_LED_HAL  ERROR! This function is not supported!\n");
			ret = RET_ERROR;
		}
		else if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_WARN)
		{
			BE_ERROR("BE_LED_HAL  WARNING! This function is not supported!\n");
		}
	}
	else
	{
		ret  = gstBeLedHwFunction.pfnGetRegInfoLED(pstParams);
	}

	return ret;
}

int BE_HDR_HAL_Get_Version(LX_BE_HDR_VERSION_T *pstParams)
{
	int ret = RET_OK;

	if (gstBeLedHwFunction.pfnGetVersionHDR == NULL)
	{
		if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_ERROR)
		{
			BE_ERROR("BE_LED_HAL  ERROR! This function is not supported!\n");
			ret = RET_ERROR;
		}
		else if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_WARN)
		{
			BE_ERROR("BE_LED_HAL  WARNING! This function is not supported!\n");
		}
	}
	else
	{
		ret  = gstBeLedHwFunction.pfnGetVersionHDR(pstParams);
	}

	return ret;
}

int BE_HDR_HAL_Set_Mode(LX_BE_HDR_MODE_T *pstParams)
{
	int ret = RET_OK;

	if (gstBeLedHwFunction.pfnSetModeHDR == NULL)
	{
		if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_ERROR)
		{
			BE_ERROR("BE_LED_HAL  ERROR! This function is not supported!\n");
			ret = RET_ERROR;
		}
		else if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_WARN)
		{
			BE_ERROR("BE_LED_HAL  WARNING! This function is not supported!\n");
		}
	}
	else
	{
		ret  = gstBeLedHwFunction.pfnSetModeHDR(pstParams);
	}

	return ret;
}

int BE_HDR_HAL_Control(LX_BE_HDR_CONTROL_T *pstParams)
{
	int ret = RET_OK;

	if (gstBeLedHwFunction.pfnControlHDR == NULL)
	{
		if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_ERROR)
		{
			BE_ERROR("BE_LED_HAL  ERROR! This function is not supported!\n");
			ret = RET_ERROR;
		}
		else if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_WARN)
		{
			BE_ERROR("BE_LED_HAL  WARNING! This function is not supported!\n");
		}
	}
	else
	{
		ret  = gstBeLedHwFunction.pfnControlHDR(pstParams);
	}

	return ret;
}

int BE_WCG_HAL_Get_Version(LX_BE_WCG_VERSION_T *pstParams)
{
	int ret = RET_OK;

	if (gstBeLedHwFunction.pfnGetVersionWCG == NULL)
	{
		if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_ERROR)
		{
			BE_ERROR("BE_LED_HAL  ERROR! This function is not supported!\n");
			ret = RET_ERROR;
		}
		else if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_WARN)
		{
			BE_ERROR("BE_LED_HAL  WARNING! This function is not supported!\n");
		}
	}
	else
	{
		ret  = gstBeLedHwFunction.pfnGetVersionWCG(pstParams);
	}

	return ret;
}

int BE_WCG_HAL_Set_Mode(LX_BE_WCG_MODE_T *pstParams)
{
	int ret = RET_OK;

	if (gstBeLedHwFunction.pfnSetModeWCG == NULL)
	{
		if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_ERROR)
		{
			BE_ERROR("BE_LED_HAL  ERROR! This function is not supported!\n");
			ret = RET_ERROR;
		}
		else if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_WARN)
		{
			BE_ERROR("BE_LED_HAL  WARNING! This function is not supported!\n");
		}
	}
	else
	{
		ret  = gstBeLedHwFunction.pfnSetModeWCG(pstParams);
	}

	return ret;
}

int BE_WCG_HAL_Control(LX_BE_WCG_CONTROL_T *pstParams)
{
	int ret = RET_OK;

	if (gstBeLedHwFunction.pfnControlWCG == NULL)
	{
		if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_ERROR)
		{
			BE_ERROR("BE_LED_HAL  ERROR! This function is not supported!\n");
			ret = RET_ERROR;
		}
		else if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_WARN)
		{
			BE_ERROR("BE_LED_HAL  WARNING! This function is not supported!\n");
		}
	}
	else
	{
		ret  = gstBeLedHwFunction.pfnControlWCG(pstParams);
	}

	return ret;
}

