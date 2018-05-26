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
 *  SRE driver file for BE device
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
#include "sre_drv.h"
#include "sre_hal.h"

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

int BE_SRE_Init(void)
{
	int ret = RET_OK;

	ret = BE_SRE_HAL_Init();

	return ret;
}

int BE_SRE_Close(void)
{
	int ret = RET_OK;

	ret = BE_SRE_HAL_Close();

	return ret;
}

int BE_SRE_Ioctl(unsigned int cmd, unsigned long arg)
{
	int ret = RET_OK;

	switch (cmd)
	{
		case BE_IOW_SRE_SET_COMMAND: 
		{
			#ifndef USE_BE_IOCTL_MACROS
			ret = BE_SRE_Set_Command(arg);
			#else
			//Added macro code
			#endif
		}
		break;

		case BE_IOW_SRE_SET_PARAMS: 
		{
			#ifndef USE_BE_IOCTL_MACROS
			ret = BE_SRE_Set_Params(arg);
			#else
			//Added macro code
			#endif
		}
		break;

		case BE_IOR_SRE_GET_VERSION: 
		{
			#ifndef USE_BE_IOCTL_MACROS
			ret = BE_SRE_Get_Version(arg);
			#else
			//Added macro code
			#endif
		}
		break;

		case BE_IOW_SRE_SET_INPUT_FORMAT: 
		{
			#ifndef USE_BE_IOCTL_MACROS
			ret = BE_SRE_Set_Input_Format(arg);
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

int BE_SRE_Set_Command(unsigned long arg)
{
	int ret = RET_OK;
	LX_BE_SRE_CMN_T stParams;

	do {
		ret = copy_from_user(&stParams, (LX_BE_SRE_CMN_T *)arg, sizeof(LX_BE_SRE_CMN_T));
		if (ret) BREAK_WRONG(ret);

		ret = BE_SRE_HAL_Set_Command(&stParams);
		if (ret) BREAK_WRONG(ret);
	} while (0);

	return ret;
}

int BE_SRE_Set_Params(unsigned long arg)
{
	int ret = RET_OK;
	LX_BE_SRE_PRAM_T stParams;

	do {
		ret = copy_from_user(&stParams, (LX_BE_SRE_PRAM_T *)arg, sizeof(LX_BE_SRE_PRAM_T));
		if (ret) BREAK_WRONG(ret);

		ret = BE_SRE_HAL_Set_Params(&stParams);
		if (ret) BREAK_WRONG(ret);
	} while (0);

	return ret;
}

int BE_SRE_Get_Version(unsigned long arg)
{
	int ret = RET_OK;
	LX_BE_SRE_VERSION_T stParams;

	do {
		ret = copy_from_user(&stParams, (void __user *)arg, sizeof(LX_BE_SRE_VERSION_T));
		if (ret) BREAK_WRONG(ret);

		ret = BE_SRE_HAL_Get_Version(&stParams);
		if (ret) BREAK_WRONG(ret);

		ret = copy_to_user((void __user *)arg, &stParams, sizeof(LX_BE_SRE_VERSION_T));
		if (ret) BREAK_WRONG(ret);
	} while (0);

	return ret;
}

int BE_SRE_Set_Input_Format(unsigned long arg)
{
	int ret = RET_OK;
	LX_BE_SRE_INPUT_MODE_T stParams;

	do {
		ret = copy_from_user(&stParams, (LX_BE_SRE_INPUT_MODE_T *)arg, sizeof(LX_BE_SRE_INPUT_MODE_T));
		if (ret) BREAK_WRONG(ret);

		ret = BE_SRE_HAL_Set_Input_Format(&stParams);
		if (ret) BREAK_WRONG(ret);
	} while (0);

	return ret;
}

