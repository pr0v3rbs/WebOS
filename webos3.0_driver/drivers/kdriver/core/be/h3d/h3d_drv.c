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
 *  h3d(3d depth) driver file for BE device
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
#include "h3d_hal.h"
#include "h3d_drv.h"

#include "be_def.h"

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
int BE_H3D_Init(void)
{
	int ret = RET_OK;

	ret = BE_H3D_HAL_Init();

	return ret;
}

int BE_H3D_Ioctl(unsigned int cmd, unsigned long arg)
{
	int ret = RET_OK;

	switch(cmd)
	{
		case BE_IOR_H3D_GET_VERSION: 
		{
			#ifndef USE_BE_IOCTL_MACROS
			ret = BE_H3D_Get_Version(arg);
			#else
			//Added macro code
			#endif
		}
		break;

		case BE_IOW_H3D_RESET_BLOCK: 
		{
			#ifndef USE_BE_IOCTL_MACROS
			ret = BE_H3D_Reset_Block(arg);
			#else
			//Added macro code
			#endif
		}
		break;
		
		case BE_IOW_H3D_BYPASS_ALL: 
		{
			#ifndef USE_BE_IOCTL_MACROS
			ret = BE_H3D_Bypass_All(arg);
			#else
			//Added macro code
			#endif
		}
		break;

		case BE_IOW_H3D_SET_MODE: 
		{
			#ifndef USE_BE_IOCTL_MACROS
			ret = BE_H3D_Set_Mode(arg);
			#else
			//Added macro code
			#endif
		}
		break;

		case BE_IOW_H3D_GET_MODE: 
		{
			#ifndef USE_BE_IOCTL_MACROS
			ret = BE_H3D_Get_Mode(arg);
			#else
			//Added macro code
			#endif
		}
		break;

		case BE_IOW_H3D_IN_FORMAT: 
		{
			#ifndef USE_BE_IOCTL_MACROS
			ret = BE_H3D_In_Format(arg);
			#else
			//Added macro code
			#endif
		}
		break;

		case BE_IOW_H3D_T3C_DEPTH_LEVEL: 
		{
			#ifndef USE_BE_IOCTL_MACROS
			ret = BE_H3D_T3C_Depth_Level(arg);
			#else
			//Added macro code
			#endif
		}
		break;
		
		case BE_IOW_H3D_A3C_DEPTH_LEVEL: 
		{
			#ifndef USE_BE_IOCTL_MACROS
			ret = BE_H3D_A3C_Depth_Level(arg);
			#else
			//Added macro code
			#endif
		}
		break;

		case BE_IOW_H3D_A3C_AUTOCONVERGENCE: 
		{
			#ifndef USE_BE_IOCTL_MACROS
			ret = BE_H3D_A3C_AutoConvergence(arg);
			#else
			//Added macro code
			#endif
		}
		break;

		case BE_IOW_H3D_SET_TRIDFMT:
		{
			#ifndef USE_BE_IOCTL_MACROS
			ret = BE_H3D_Set_Trid_Fmt(arg);
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

int BE_H3D_Get_Version(unsigned long arg)
{
	int ret = RET_OK;
	LX_BE_H3D_VERSION_T stParams;

	do{
		ret = BE_H3D_HAL_Get_Version(&stParams);
		ret = copy_to_user((LX_BE_H3D_VERSION_T *)arg, &stParams, sizeof(LX_BE_H3D_VERSION_T));//(to, from, n)
		if(ret) BREAK_WRONG(ret);		
	
	} while(0);
	return ret;
}

int BE_H3D_Reset_Block(unsigned long arg)
{
	int ret = RET_OK;
	LX_BE_H3D_RESET_BLOCK_T stParams;

	do{
		ret = copy_from_user(&stParams, (LX_BE_H3D_RESET_BLOCK_T *)arg, sizeof(LX_BE_H3D_RESET_BLOCK_T));
		if(ret) BREAK_WRONG(ret);		

		ret = BE_H3D_HAL_Reset_Block(&stParams);
		if(ret) BREAK_WRONG(ret);
		
	} while(0);
	return ret;
}

int BE_H3D_Bypass_All(unsigned long arg)
{
	int ret = RET_OK;
	LX_BE_H3D_BYPASS_ALL_T stParams;

	do{
		ret = copy_from_user(&stParams, (LX_BE_H3D_BYPASS_ALL_T *)arg, sizeof(LX_BE_H3D_BYPASS_ALL_T));
		if(ret) BREAK_WRONG(ret);		

		ret = BE_H3D_HAL_Bypass_All(&stParams);
		if(ret) BREAK_WRONG(ret);
	} while(0);
	return ret;
}

int BE_H3D_Set_Mode(unsigned long arg)
{
	int ret = RET_OK;
	LX_BE_H3D_MODE_T stParams;

	do{
		ret = copy_from_user(&stParams, (LX_BE_H3D_MODE_T *)arg, sizeof(LX_BE_H3D_MODE_T));
		if(ret) BREAK_WRONG(ret);		

		ret = BE_H3D_HAL_Set_Mode(&stParams);
		if(ret) BREAK_WRONG(ret);
	} while(0);
	return ret;
}

int BE_H3D_Get_Mode(unsigned long arg)
{
	int ret = RET_OK;
	LX_BE_H3D_MODE_T stParams;

	do{
		ret = BE_H3D_HAL_Get_Mode(&stParams);
		ret = copy_to_user((LX_BE_H3D_MODE_T *)arg, &stParams, sizeof(LX_BE_H3D_MODE_T));
		if(ret) BREAK_WRONG(ret);		

	} while(0);
	return ret;
}

int BE_H3D_In_Format(unsigned long arg)
{
	int ret = RET_OK;
	LX_BE_H3D_IN_FORMAT_T stParams;

	do{
		ret = copy_from_user(&stParams,(LX_BE_H3D_IN_FORMAT_T *)arg, sizeof(LX_BE_H3D_IN_FORMAT_T));
		if(ret) BREAK_WRONG(ret);		  

		ret = BE_H3D_HAL_In_Format(&stParams);
		if(ret) BREAK_WRONG(ret);
	} while(0);
	return ret;
}

int BE_H3D_T3C_Depth_Level(unsigned long arg)
{
	int ret = RET_OK;
	LX_BE_H3D_T3C_DEPTH_LEVEL_T stParams;

	do{
		ret = copy_from_user(&stParams,(LX_BE_H3D_T3C_DEPTH_LEVEL_T *)arg, sizeof(LX_BE_H3D_T3C_DEPTH_LEVEL_T));
		if(ret) BREAK_WRONG(ret);		

		ret = BE_H3D_HAL_T3C_Depth_Level(&stParams);
		if(ret) BREAK_WRONG(ret);
	} while(0);
	return ret;
}

int BE_H3D_A3C_Depth_Level(unsigned long arg)
{
	int ret = RET_OK;
	LX_BE_H3D_A3C_DEPTH_LEVEL_T stParams;

	do{
		ret = copy_from_user(&stParams,(LX_BE_H3D_A3C_DEPTH_LEVEL_T *)arg, sizeof(LX_BE_H3D_A3C_DEPTH_LEVEL_T));
		if(ret) BREAK_WRONG(ret);		

		ret = BE_H3D_HAL_A3C_Depth_Level(&stParams);
		if(ret) BREAK_WRONG(ret);
	} while(0);
	return ret;
}

int BE_H3D_A3C_AutoConvergence(unsigned long arg)
{
	int ret = RET_OK;
	LX_BE_H3D_A3C_AUTOCONVERGENCE_T stParams;

	do{
		ret = copy_from_user(&stParams,(LX_BE_H3D_A3C_AUTOCONVERGENCE_T *)arg, sizeof(LX_BE_H3D_A3C_AUTOCONVERGENCE_T));
		if(ret) BREAK_WRONG(ret);		

		ret =  BE_H3D_HAL_A3C_AutoConvergence(&stParams);
		if(ret) BREAK_WRONG(ret);
	} while(0);
	return ret;
}

int BE_H3D_Set_Trid_Fmt( unsigned long arg)
{

	int ret = RET_OK;
	LX_BE_TRIDTV_FMT_CTRL_T stParams;

	do{
		ret = copy_from_user(&stParams,(LX_BE_TRIDTV_FMT_CTRL_T *)arg, sizeof(LX_BE_TRIDTV_FMT_CTRL_T));
		if(ret) BREAK_WRONG(ret);		

		ret = BE_H3D_HAL_Set_Trid_Fmt(&stParams);
		if(ret) BREAK_WRONG(ret);
	} while(0);
	return ret;
}
