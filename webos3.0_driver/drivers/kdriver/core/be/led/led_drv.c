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
 *  led(dppb) driver file for BE device
 *	
 *  author		justine.jeong
 *  version		1.0
 *  date		2012.03.06
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
#include "led_drv.h"
#include "led_hal.h"

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

int BE_LED_Init(void)
{
	int ret = RET_OK;

	ret = BE_LED_HAL_Init();

	return ret;
}

int BE_LED_Close(void)
{
	int ret = RET_OK;

	ret = BE_LED_HAL_Close();

	return ret;
}

int BE_LED_Ioctl(unsigned int cmd, unsigned long arg)
{
	int ret = RET_OK;

	switch (cmd)
	{
		case BE_IOR_LED_GET_VERSION: 
		{
			#ifndef USE_BE_IOCTL_MACROS
			ret = BE_LED_Get_Version(arg);
			#else
			//Added macro code
			#endif
		}
		break;

		case BE_IOW_LED_SET_MODE: 
		{
			#ifndef USE_BE_IOCTL_MACROS
			ret = BE_LED_Set_Mode(arg);
			#else
			//Added macro code
			#endif
		}
		break;

		case BE_IOW_LED_CONTROL: 
		{
			#ifndef USE_BE_IOCTL_MACROS
			ret = BE_LED_Control(arg);
			#else
			//Added macro code
			#endif
		}
		break;

		case BE_IOW_LED_GET_APL: 
		{
			#ifndef USE_BE_IOCTL_MACROS
			ret = BE_LED_Get_Apl(arg);
			#else
			//Added macro code
			#endif
		}
		break;

		case BE_IOW_LED_SET_CLIPPING_VALUE: 
		{
			#ifndef USE_BE_IOCTL_MACROS
			ret = BE_LED_Set_ClippingValue(arg);
			#else
			//Added macro code
			#endif
		}
		break;

		case BE_IOW_LED_GET_REG_INFO: 
		{
			#ifndef USE_BE_IOCTL_MACROS
			ret = BE_LED_Get_RegInfo(arg);
			#else
			//Added macro code
			#endif
		}
		break;

		case BE_IOR_HDR_GET_VERSION: 
		{
			#ifndef USE_BE_IOCTL_MACROS
			ret = BE_HDR_Get_Version(arg);
			#else
			//Added macro code
			#endif
		}
		break;

		case BE_IOW_HDR_SET_MODE: 
		{
			#ifndef USE_BE_IOCTL_MACROS
			ret = BE_HDR_Set_Mode(arg);
			#else
			//Added macro code
			#endif
		}
		break;

		case BE_IOW_HDR_CONTROL: 
		{
			#ifndef USE_BE_IOCTL_MACROS
			ret = BE_HDR_Control(arg);
			#else
			//Added macro code
			#endif
		}
		break;

		case BE_IOR_WCG_GET_VERSION: 
		{
			#ifndef USE_BE_IOCTL_MACROS
			ret = BE_WCG_Get_Version(arg);
			#else
			//Added macro code
			#endif
		}
		break;

		case BE_IOW_WCG_SET_MODE: 
		{
			#ifndef USE_BE_IOCTL_MACROS
			ret = BE_WCG_Set_Mode(arg);
			#else
			//Added macro code
			#endif
		}
		break;

		case BE_IOW_WCG_CONTROL: 
		{
			#ifndef USE_BE_IOCTL_MACROS
			ret = BE_WCG_Control(arg);
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

int BE_LED_Get_Version(unsigned long arg)
{
	int ret = RET_OK;
	LX_BE_LED_VERSION_T stParams;

	do {
		ret = copy_from_user(&stParams, (void __user *)arg, sizeof(LX_BE_LED_VERSION_T));
		if (ret) BREAK_WRONG(ret);
		
		ret = BE_LED_HAL_Get_Version(&stParams);
		if (ret) BREAK_WRONG(ret);

		ret = copy_to_user((void __user *)arg, &stParams, sizeof(LX_BE_LED_VERSION_T));
		if (ret) BREAK_WRONG(ret);
	} while (0);

	return ret;
}

int BE_LED_Set_Mode(unsigned long arg)
{
	int ret = RET_OK;
	LX_BE_LED_MODE_T stParams;

	do {
		ret = copy_from_user(&stParams, (LX_BE_LED_MODE_T *)arg, sizeof(LX_BE_LED_MODE_T));
		if (ret) BREAK_WRONG(ret);

		ret = BE_LED_HAL_Set_Mode(&stParams);
		if (ret) BREAK_WRONG(ret);
	} while (0);

	return ret;
}

int BE_LED_Control(unsigned long arg)
{
	int ret = RET_OK;
	LX_BE_LED_CONTROL_T stParams;

	do {
		ret = copy_from_user(&stParams, (LX_BE_LED_CONTROL_T *)arg, sizeof(LX_BE_LED_CONTROL_T));
		if (ret) BREAK_WRONG(ret);

		ret = BE_LED_HAL_Control(&stParams);
		if (ret) BREAK_WRONG(ret);
	} while (0);

	return ret;
}

int BE_LED_Get_Apl(unsigned long arg)
{
	int ret = RET_OK;
	LX_BE_LED_APL_T stParams;

	do {
		ret = BE_LED_HAL_Get_Apl(&stParams);
		if (ret) BREAK_WRONG(ret);

		ret = copy_to_user((void __user *)arg, &stParams, sizeof(LX_BE_LED_APL_T));
		if (ret) BREAK_WRONG(ret);
	} while (0);

	return ret;
}

int BE_LED_Set_ClippingValue(unsigned long arg)
{
	int ret = RET_OK;
	UINT32 param;

	do {
		ret = copy_from_user(&param, (UINT32 *)arg, sizeof(UINT32));
		if (ret) BREAK_WRONG(ret);

		ret = BE_LED_HAL_Set_ClippingValue(param);
		if (ret) BREAK_WRONG(ret);
	} while (0);

	return ret;
}

int BE_LED_Get_RegInfo(unsigned long arg)
{
	int ret = RET_OK;
	LX_BE_LED_REG_INFO_T stParams;

	do {
		ret = BE_LED_HAL_Get_RegInfo(&stParams);
		if (ret) BREAK_WRONG(ret);

		ret = copy_to_user((void __user *)arg, &stParams, sizeof(LX_BE_LED_REG_INFO_T));
		if (ret) BREAK_WRONG(ret);
	} while (0);

	return ret;
}

int BE_HDR_Get_Version(unsigned long arg)
{
	int ret = RET_OK;
	LX_BE_HDR_VERSION_T stParams;

	do {
		ret = copy_from_user(&stParams, (void __user *)arg, sizeof(LX_BE_HDR_VERSION_T));
		if (ret) BREAK_WRONG(ret);

		ret = BE_HDR_HAL_Get_Version(&stParams);
		if (ret) BREAK_WRONG(ret);

		ret = copy_to_user((void __user *)arg, &stParams, sizeof(LX_BE_HDR_VERSION_T));
		if (ret) BREAK_WRONG(ret);
	} while (0);

	return ret;
}

int BE_HDR_Set_Mode(unsigned long arg)
{
	int ret = RET_OK;
	LX_BE_HDR_MODE_T stParams;

	do {
		ret = copy_from_user(&stParams, (LX_BE_HDR_MODE_T *)arg, sizeof(LX_BE_HDR_MODE_T));
		if (ret) BREAK_WRONG(ret);

		ret = BE_HDR_HAL_Set_Mode(&stParams);
		if (ret) BREAK_WRONG(ret);
	} while (0);

	return ret;
}

int BE_HDR_Control(unsigned long arg)
{
	int ret = RET_OK;
	LX_BE_HDR_CONTROL_T stParams;

	do {
		ret = copy_from_user(&stParams, (LX_BE_HDR_CONTROL_T *)arg, sizeof(LX_BE_HDR_CONTROL_T));
		if (ret) BREAK_WRONG(ret);

		ret = BE_HDR_HAL_Control(&stParams);
		if (ret) BREAK_WRONG(ret);
	} while (0);

	return ret;
}

int BE_WCG_Get_Version(unsigned long arg)
{
	int ret = RET_OK;
	LX_BE_WCG_VERSION_T stParams;

	do {
		ret = copy_from_user(&stParams, (void __user *)arg, sizeof(LX_BE_WCG_VERSION_T));
		if (ret) BREAK_WRONG(ret);

		ret = BE_WCG_HAL_Get_Version(&stParams);
		if (ret) BREAK_WRONG(ret);

		ret = copy_to_user((void __user *)arg, &stParams, sizeof(LX_BE_WCG_VERSION_T));
		if (ret) BREAK_WRONG(ret);
	} while (0);

	return ret;
}

int BE_WCG_Set_Mode(unsigned long arg)
{
	int ret = RET_OK;
	LX_BE_WCG_MODE_T stParams;

	do {
		ret = copy_from_user(&stParams, (LX_BE_WCG_MODE_T *)arg, sizeof(LX_BE_WCG_MODE_T));
		if (ret) BREAK_WRONG(ret);

		ret = BE_WCG_HAL_Set_Mode(&stParams);
		if (ret) BREAK_WRONG(ret);
	} while (0);

	return ret;
}

int BE_WCG_Control(unsigned long arg)
{
	int ret = RET_OK;
	LX_BE_WCG_CONTROL_T stParams;

	do {
		ret = copy_from_user(&stParams, (LX_BE_WCG_CONTROL_T *)arg, sizeof(LX_BE_WCG_CONTROL_T));
		if (ret) BREAK_WRONG(ret);

		ret = BE_WCG_HAL_Control(&stParams);
		if (ret) BREAK_WRONG(ret);
	} while (0);

	return ret;
}

