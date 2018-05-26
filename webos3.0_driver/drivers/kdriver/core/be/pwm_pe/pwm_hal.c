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
 *  PWM HAL file for BE device
 *
 *  author		dj911.kim
 *  version		1.0
 *  date		2012.06.30
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
#include <linux/delay.h>


#include "os_util.h"
#include "be_dbg.h"

#include "pwm_hal.h"
#include "h13/be/pwm_pe/be_pwm_hw_h13.h"
#include "h14/be/pwm_pe/be_pwm_hw_h14.h"
#include "m14/be/pwm_pe/be_pwm_hw_m14.h"
#include "h15/be/pwm_pe/be_pwm_hw_h15.h"

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
	int (*pfnInitializePWM)(void);
	int (*pfnClosePWM)(void);
	int (*pfnSetControlPWM)(BE_PWM_CTRL_T *pstParams);
	int (*pfnSetAdaptFreqControlPWM)(BE_PWM_ADAPT_FREQ_CTRL_T *pstParams);
	int (*pfnSetFrequencyPWM)(BE_PWM_FREQ_T *pstParams);
	int (*pfnSetDutyCyclePWM)(BE_PWM_DUTY_T *pstParams);
	int (*pfnGetInfoExternalPWM)(BE_EXTERNAL_PWM_T *pstParams);
	int (*pfnSetSyncPWM)(BE_PWM_FREQ_T *pstParams);
} BE_PWM_HW_FUNCTION_T;

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
static BE_PWM_HW_FUNCTION_T gstBePwmHwFunction;

/*========================================================================================
	Implementation Group
========================================================================================*/
int BE_PWM_HAL_Init(void)
{
	int ret = RET_OK;

	if(0)
	{

	}
#ifdef INCLUDE_H15_CHIP_KDRV
	else if( lx_chip_rev( ) >= LX_CHIP_REV( H15, A0) )
	{
		BE_PRINT("BE PWM chip revision is set to H15 A0\n");
		gstBePwmHwFunction.pfnInitializePWM				= BE_PWM_HW_H15_Init;
		gstBePwmHwFunction.pfnClosePWM					= NULL;
		gstBePwmHwFunction.pfnSetControlPWM				= BE_PWM_HW_H15_SetPwmControl;
		gstBePwmHwFunction.pfnSetAdaptFreqControlPWM		= BE_PWM_HW_H15_SetPwmAdaptFreqControl;
		gstBePwmHwFunction.pfnSetFrequencyPWM			= BE_PWM_HW_H15_SetPwmFrequency;
		gstBePwmHwFunction.pfnSetDutyCyclePWM			= BE_PWM_HW_H15_SetPwmDutyCycle;
		gstBePwmHwFunction.pfnGetInfoExternalPWM			= BE_PWM_HW_H15_GetPwmInfo;
		gstBePwmHwFunction.pfnSetSyncPWM				= BE_PWM_HW_H15_SetPwmSync;
	}
#endif
#ifdef INCLUDE_H14_CHIP_KDRV
	else if( lx_chip_rev( ) >= LX_CHIP_REV( H14, A0) )
	{
		BE_PRINT("BE PWM chip revision is set to H14 A0\n");
		gstBePwmHwFunction.pfnInitializePWM				= BE_PWM_HW_H14_Init;
		gstBePwmHwFunction.pfnClosePWM					= NULL;
		gstBePwmHwFunction.pfnSetControlPWM				= BE_PWM_HW_H14_SetPwmControl;
		gstBePwmHwFunction.pfnSetAdaptFreqControlPWM		= BE_PWM_HW_H14_SetPwmAdaptFreqControl;
		gstBePwmHwFunction.pfnSetFrequencyPWM			= BE_PWM_HW_H14_SetPwmFrequency;
		gstBePwmHwFunction.pfnSetDutyCyclePWM			= BE_PWM_HW_H14_SetPwmDutyCycle;
		gstBePwmHwFunction.pfnGetInfoExternalPWM			= BE_PWM_HW_H14_GetPwmInfo;
		gstBePwmHwFunction.pfnSetSyncPWM				= BE_PWM_HW_H14_SetPwmSync;
	}
#endif
#ifdef INCLUDE_M14_CHIP_KDRV
	else if( lx_chip_rev( ) >= LX_CHIP_REV( M14, A0) )
	{
		BE_PRINT("BE PWM chip revision is set to M14 A0\n");
		gstBePwmHwFunction.pfnInitializePWM				= BE_PWM_HW_M14_Init;
		gstBePwmHwFunction.pfnClosePWM					= NULL;
		gstBePwmHwFunction.pfnSetControlPWM				= BE_PWM_HW_M14_SetPwmControl;
		gstBePwmHwFunction.pfnSetAdaptFreqControlPWM		= BE_PWM_HW_M14_SetPwmAdaptFreqControl;
		gstBePwmHwFunction.pfnSetFrequencyPWM			= BE_PWM_HW_M14_SetPwmFrequency;
		gstBePwmHwFunction.pfnSetDutyCyclePWM			= BE_PWM_HW_M14_SetPwmDutyCycle;
		gstBePwmHwFunction.pfnGetInfoExternalPWM			= BE_PWM_HW_M14_GetPwmInfo;
		gstBePwmHwFunction.pfnSetSyncPWM				= BE_PWM_HW_M14_SetPwmSync;
	}
#endif
#ifdef INCLUDE_H13_CHIP_KDRV
	else if ( lx_chip_rev( ) >= LX_CHIP_REV( H13, A0) )
	{
		BE_PRINT("BE PWM chip revision is set to H13 A0\n");
		gstBePwmHwFunction.pfnInitializePWM				= BE_PWM_HW_H13_Init;
		gstBePwmHwFunction.pfnClosePWM					= NULL;
		gstBePwmHwFunction.pfnSetControlPWM				= BE_PWM_HW_H13_SetPwmControl;
		gstBePwmHwFunction.pfnSetAdaptFreqControlPWM		= BE_PWM_HW_H13_SetPwmAdaptFreqControl;
		gstBePwmHwFunction.pfnSetFrequencyPWM			= BE_PWM_HW_H13_SetPwmFrequency;
		gstBePwmHwFunction.pfnSetDutyCyclePWM			= BE_PWM_HW_H13_SetPwmDutyCycle;
		gstBePwmHwFunction.pfnGetInfoExternalPWM			= BE_PWM_HW_H13_GetPwmInfo;
		gstBePwmHwFunction.pfnSetSyncPWM				= BE_PWM_HW_H13_SetPwmSync;
	}
#endif
	else
	{
		BE_ERROR("BE PWM ERROR! Unknown chip revision at BE PWM module\n");
		gstBePwmHwFunction.pfnInitializePWM				= NULL;
		gstBePwmHwFunction.pfnClosePWM					= NULL;
		gstBePwmHwFunction.pfnSetControlPWM				= NULL;
		gstBePwmHwFunction.pfnSetAdaptFreqControlPWM		= NULL;
		gstBePwmHwFunction.pfnSetFrequencyPWM			= NULL;
		gstBePwmHwFunction.pfnSetDutyCyclePWM			= NULL;
		gstBePwmHwFunction.pfnGetInfoExternalPWM			= NULL;
		gstBePwmHwFunction.pfnSetSyncPWM				= NULL;
		ret = RET_ERROR;
	}

	if(gstBePwmHwFunction.pfnInitializePWM)
	{
		ret = gstBePwmHwFunction.pfnInitializePWM();
	}

	return ret;
}

int BE_PWM_HAL_Close(void)
{
	int ret = RET_OK;

	if(gstBePwmHwFunction.pfnClosePWM)
	{
		ret = gstBePwmHwFunction.pfnClosePWM();
	}

	gstBePwmHwFunction.pfnInitializePWM				= NULL;
	gstBePwmHwFunction.pfnClosePWM					= NULL;
	gstBePwmHwFunction.pfnSetControlPWM				= NULL;
	gstBePwmHwFunction.pfnSetAdaptFreqControlPWM		= NULL;
	gstBePwmHwFunction.pfnSetFrequencyPWM			= NULL;
	gstBePwmHwFunction.pfnSetDutyCyclePWM			= NULL;
	gstBePwmHwFunction.pfnGetInfoExternalPWM			= NULL;
	gstBePwmHwFunction.pfnSetSyncPWM				= NULL;

	return ret;
}

int BE_PWM_HAL_SetPwmControl(BE_PWM_CTRL_T *pstParams)
{
	int ret = RET_OK;

	if(gstBePwmHwFunction.pfnSetControlPWM)
	{
		ret = gstBePwmHwFunction.pfnSetControlPWM(pstParams);
	}
	else
	{
		if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_ERROR)
		{
			BE_ERROR("BE_PWM_HAL  ERROR! This function is not supported!\n");
			ret = RET_ERROR;
		}
		else if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_WARN)
		{
			BE_ERROR("BE_PWM_HAL  WARNING! This function is not supported!\n");
		}
	}

	return ret;
}

int BE_PWM_HAL_SetPwmAdaptFreqControl(BE_PWM_ADAPT_FREQ_CTRL_T *pstParams)
{
	int ret = RET_OK;

	if(gstBePwmHwFunction.pfnSetAdaptFreqControlPWM)
	{
		ret = gstBePwmHwFunction.pfnSetAdaptFreqControlPWM(pstParams);
	}
	else
	{
		if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_ERROR)
		{
			BE_ERROR("BE_PWM_HAL  ERROR! This function is not supported!\n");
			ret = RET_ERROR;
		}
		else if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_WARN)
		{
			BE_ERROR("BE_PWM_HAL  WARNING! This function is not supported!\n");
		}
	}

	return ret;
}

int BE_PWM_HAL_SetPwmFrequency(BE_PWM_FREQ_T *pstParams)
{
	int ret = RET_OK;

	if(gstBePwmHwFunction.pfnSetFrequencyPWM)
	{
		ret = gstBePwmHwFunction.pfnSetFrequencyPWM(pstParams);
	}
	else
	{
		if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_ERROR)
		{
			BE_ERROR("BE_PWM_HAL  ERROR! This function is not supported!\n");
			ret = RET_ERROR;
		}
		else if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_WARN)
		{
			BE_ERROR("BE_PWM_HAL  WARNING! This function is not supported!\n");
		}
	}

	return ret;
}

int BE_PWM_HAL_SetPwmDutyCycle(BE_PWM_DUTY_T *pstParams)
{
	int ret = RET_OK;

	if(gstBePwmHwFunction.pfnSetDutyCyclePWM)
	{
		ret = gstBePwmHwFunction.pfnSetDutyCyclePWM(pstParams);
	}
	else
	{
		if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_ERROR)
		{
			BE_ERROR("BE_PWM_HAL  ERROR! This function is not supported!\n");
			ret = RET_ERROR;
		}
		else if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_WARN)
		{
			BE_ERROR("BE_PWM_HAL  WARNING! This function is not supported!\n");
		}
	}

	return ret;
}

int BE_PWM_HAL_GetPwmInfoExternal(BE_EXTERNAL_PWM_T *pstParams)
{
	int ret = RET_OK;

	if(gstBePwmHwFunction.pfnGetInfoExternalPWM)
	{
		ret = gstBePwmHwFunction.pfnGetInfoExternalPWM(pstParams);
	}
	else
	{
		if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_ERROR)
		{
			BE_ERROR("BE_PWM_HAL  ERROR! This function is not supported!\n");
			ret = RET_ERROR;
		}
		else if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_WARN)
		{
			BE_ERROR("BE_PWM_HAL  WARNING! This function is not supported!\n");
		}
	}

	return ret;
}

int BE_PWM_HAL_SetPwmSync(UINT32 u32Frequency)
{
	int ret = RET_OK;
	BE_PWM_FREQ_T stParams;

	if(gstBePwmHwFunction.pfnSetSyncPWM)
	{
		stParams.port = BE_PWM_MAX; // Call from ISR
		stParams.frequency = u32Frequency;
		ret = gstBePwmHwFunction.pfnSetSyncPWM(&stParams);
	}
	else
	{
		if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_ERROR)
		{
			BE_ERROR("BE_PWM_HAL  ERROR! This function is not supported!\n");
			ret = RET_ERROR;
		}
		else if (gHalFuncSupportDbgLevel == BE_DBG_LEVEL_WARN)
		{
			BE_ERROR("BE_PWM_HAL  WARNING! This function is not supported!\n");
		}
	}

	return ret;
}

