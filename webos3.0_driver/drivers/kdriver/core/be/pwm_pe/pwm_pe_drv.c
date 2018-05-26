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
 *  pwm pe driver file for BE device
 *	
 *  author		won.hur	
 *  version		1.0
 *  date		2012.04.27
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
#include "be_def.h"
#include "pwm_pe_drv.h"
#include "pwm_hal.h"

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
int BE_PWM_PE_Init(void)
{
	int ret = RET_OK;

	ret = BE_PWM_HAL_Init();

	return ret;
}

int BE_PWM_PE_Ioctl(unsigned int cmd, unsigned long arg)
{
	int ret = RET_OK;

	switch(cmd)
	{
		case BE_IOW_PWM_SET_CONTROL: 
		{
			#ifndef USE_BE_IOCTL_MACROS
			ret = BE_PWM_Set_Control(arg);
			#else
			//Added macro code
			#endif
		}
		break;

		case BE_IOW_PWM_SET_ADAPT_FREQ_CONTROL: 
		{
			#ifndef USE_BE_IOCTL_MACROS
			ret = BE_PWM_Set_AdaptFreqControl(arg);
			#else
			//Added macro code
			#endif
		}
		break;

		case BE_IOW_PWM_SET_FREQUENCY: 
		{
			#ifndef USE_BE_IOCTL_MACROS
			ret = BE_PWM_Set_Frequency(arg);
			#else
			//Added macro code
			#endif
		}
		break;

		case BE_IOW_PWM_SET_DUTY_CYCLE: 
		{
			#ifndef USE_BE_IOCTL_MACROS
			ret = BE_PWM_Set_DutyCycle(arg);
			#else
			//Added macro code
			#endif
		}
		break;

		case BE_IOR_PWM_GET_EXTERNAL_PWM: 
		{
			#ifndef USE_BE_IOCTL_MACROS
			ret = BE_PWM_Get_ExternalPwm(arg);
			#else
			//Added macro code
			#endif
		}
		break;

		default :
		{
			BE_ERROR("WARNING! WRONG IOCTL CALL!!\n");
			ret = RET_ERROR;
		}
		break;
	}

	return ret;
}

int BE_PWM_Set_Control(unsigned long arg)
{
	int ret = RET_OK;
	BE_PWM_CTRL_T stParams;

	do{
		ret = copy_from_user(&stParams, (BE_PWM_CTRL_T *)arg, sizeof(BE_PWM_CTRL_T));
		if(ret) BREAK_WRONG(ret);

		ret = BE_PWM_HAL_SetPwmControl(&stParams);
		if(ret) BREAK_WRONG(ret);
	} while(0);
	
	return ret;
}

int BE_PWM_Set_AdaptFreqControl(unsigned long arg)
{
	int ret = RET_OK;
	BE_PWM_ADAPT_FREQ_CTRL_T stParams;

	do{
		ret = copy_from_user(&stParams, (BE_PWM_ADAPT_FREQ_CTRL_T *)arg, sizeof(BE_PWM_ADAPT_FREQ_CTRL_T));
		if(ret) BREAK_WRONG(ret);

		ret = BE_PWM_HAL_SetPwmAdaptFreqControl(&stParams);
		if(ret) BREAK_WRONG(ret);
	} while(0);
	
	return ret;
}

int BE_PWM_Set_Frequency(unsigned long arg)
{
	int ret = RET_OK;
	BE_PWM_FREQ_T stParams;

	do{
		ret = copy_from_user(&stParams, (BE_PWM_FREQ_T *)arg, sizeof(BE_PWM_FREQ_T));
		if(ret) BREAK_WRONG(ret);

		ret = BE_PWM_HAL_SetPwmFrequency(&stParams);
		if(ret) BREAK_WRONG(ret);
	} while(0);
	
	return ret;
}

int BE_PWM_Set_DutyCycle(unsigned long arg)
{
	int ret = RET_OK;
	BE_PWM_DUTY_T stParams;

	do{
		ret = copy_from_user(&stParams, (BE_PWM_DUTY_T *)arg, sizeof(BE_PWM_DUTY_T));
		if(ret) BREAK_WRONG(ret);

		ret = BE_PWM_HAL_SetPwmDutyCycle(&stParams);
		if(ret) BREAK_WRONG(ret);
	} while(0);
	
	return ret;
}

int BE_PWM_Get_ExternalPwm(unsigned long arg)
{
	int ret = RET_OK;
	BE_EXTERNAL_PWM_T stParams;

	do{
		ret = copy_from_user(&stParams, (void __user *)arg, sizeof(BE_EXTERNAL_PWM_T));
		if(ret) BREAK_WRONG(ret);

		ret = BE_PWM_HAL_GetPwmInfoExternal(&stParams);
		if(ret) BREAK_WRONG(ret);
		
		ret = copy_to_user((void __user *)arg, &stParams, sizeof(BE_EXTERNAL_PWM_T));
		if(ret) BREAK_WRONG(ret);
	} while(0);
	
	return ret;
}

