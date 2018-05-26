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
 *  be_top driver file for BE device
 *
 *  author		dj911.kim
 *  version		1.0
 *  date		2013.04.27
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
#include "be_reg.h"
#include "fwl_drv.h"
#include "be_top_hal.h"
#include "be_top_drv.h"

#include "video_status_keeper.h"
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
extern  LX_VP_ST_KEEPER_T *gSKeeperDataBase;
/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/
BOOLEAN gHalFuncSupportDbgLevel = BE_DBG_LEVEL_IGNORE;
/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/

/*========================================================================================
	Implementation Group
========================================================================================*/
int BE_TOP_Init(void)
{
	int ret = RET_OK;

	ret = BE_TOP_HAL_Init();

	return ret;
}

int BE_TOP_Close(void)
{
	int ret = RET_OK;

	ret = BE_TOP_HAL_Close();

	return ret;
}

int BE_TOP_Ioctl(unsigned int cmd, unsigned long arg)
{
	int ret = RET_OK;

	switch(cmd)
	{
		case BE_IOW_UPDATE_FIRMWARE:
		{
			BE_FW_DWLD_T stParams;

			ret = copy_from_user(&stParams, (BE_FW_DWLD_T *)arg, sizeof(BE_FW_DWLD_T));
			if(ret) BREAK_WRONG(ret);

			ret = BE_FW_Update(&stParams);
		}
		break;

		case BE_IOW_RESET_SW:
		{
			#ifndef USE_BE_IOCTL_MACROS
			ret = BE_TOP_Reset_SW();
			#else
			//Added macro code
			#endif
		}
		break;

		case BE_IOW_TOP_SET_BYPASS:
		{
			#ifndef USE_BE_IOCTL_MACROS
			ret = BE_TOP_Set_Bypass(arg);
			#else
			//Added macro code
			#endif
		}
		break;

		case BE_IOR_TOP_GET_BYPASS:
		{
			#ifndef USE_BE_IOCTL_MACROS
			ret = BE_TOP_Get_Bypass(arg);
			#else
			//Added macro code
			#endif
		}
		break;

		case BE_IOW_TOP_TOGGLE_3D_LR:
		{
			#ifndef USE_BE_IOCTL_MACROS
			ret = BE_TOP_Toggle_3D_LR(arg);
			#else
			//Added macro code
			#endif
		}
		break;

		case BE_IOW_TOP_SET_3D_IN_FORMAT:
		{
			#ifndef USE_BE_IOCTL_MACROS
			ret = BE_TOP_Set_3D_In_Format(arg);
			#else
			//Added macro code
			#endif
		}
		break;

		case BE_IOW_TOP_GET_3D_IN_FORMAT:
		{
			#ifndef USE_BE_IOCTL_MACROS
			ret = BE_TOP_Get_3D_In_Format(arg);
			#else
			//Added macro code
			#endif
		}
		break;

		case BE_IOW_TOP_SET_3D_OUT_FORMAT:
		{
			#ifndef USE_BE_IOCTL_MACROS
			ret = BE_TOP_Set_3D_Out_Format(arg);
			#else
			//Added macro code
			#endif
		}
		break;

		case BE_IOW_TOP_SET_3D_CONVERGENCE:
		{
			#ifndef USE_BE_IOCTL_MACROS
			ret = BE_TOP_Set_3D_Convergence(arg);
			#else
			//Added macro code
			#endif
		}
		break;

		case BE_IOW_TOP_SET_BLACK_BAR:
		{
			#ifndef USE_BE_IOCTL_MACROS
			ret = BE_TOP_Set_Black_Bar(arg);
			#else
			//Added macro code
			#endif
		}
		break;

		case BE_IOW_TOP_SET_VIDEO_MUTE:
		{
			#ifndef USE_BE_IOCTL_MACROS
			ret = BE_TOP_Set_Video_Mute(arg);
			#else
			//Added macro code
			#endif
		}
		break;

		case BE_IOW_TOP_SET_DISPLAY_TYPE:
		{
			#ifndef USE_BE_IOCTL_MACROS
			ret = BE_TOP_Set_Display_Type(arg);
			#else
			//Added macro code
			#endif
		}
		break;

		case BE_IOW_TOP_SET_DEBUG_DISPLAY:
		{
			#ifndef USE_BE_IOCTL_MACROS
			ret = BE_TOP_Set_Debug_Display(arg);
			#else
			//Added macro code
			#endif
		}
		break;

		case BE_IOW_SET_VID_OPERATION:
		{
			#ifndef USE_BE_IOCTL_MACROS
			ret = BE_TOP_Set_VID_Operation(arg);
			#else
			//Added macro code
			#endif
		}
		break;

		case BE_IOW_SET_INPUT_SRC_TYPE:
		{
			#ifndef USE_BE_IOCTL_MACROS
			ret = BE_TOP_Set_Input_Src_Type(arg);
			#else
			//Added macro code
			#endif
		}
		break;

		case BE_IOW_SET_ACTIVE_WINDOW_START:
		{
			#ifndef USE_BE_IOCTL_MACROS
			ret = BE_TOP_Set_Active_Window_Start(arg);
			#else
			//Added macro code
			#endif
		}
		break;

		case BE_IOW_SET_ACTIVE_WINDOW_SIZE:
		{
			#ifndef USE_BE_IOCTL_MACROS
			ret = BE_TOP_Set_Active_Window_Size(arg);
			#else
			//Added macro code
			#endif
		}
		break;

		case BE_IOW_SET_REAL_CINEMA:
		{
			#ifndef USE_BE_IOCTL_MACROS
			ret = BE_TOP_Set_Real_Cinema(arg);
			#else
			//Added macro code
			#endif
		}
		break;

		case BE_IOW_TOP_FIRMWARE_DEBUG:
		{
			#ifndef USE_BE_IOCTL_MACROS
			ret = BE_TOP_Firmware_Debug(arg);
			#else
			//Added macro code
			#endif
		}
		break;

		case BE_IOW_SET_UART_FOR_MCU:
		{
			#ifndef USE_BE_IOCTL_MACROS
			ret = BE_TOP_Set_UartForMcu(arg);
			#else
			//Added macro code
			#endif
		}
		break;

		case BE_IORW_REG_ACCESS:
		{
			#ifndef USE_BE_IOCTL_MACROS
			ret = BE_REG_RegAccess(arg);
			#else
			//Added macro code
			#endif
		}
		break;

		case BE_IOW_ENABLE_DEBUG_PRINT:
		{
			#ifndef USE_BE_IOCTL_MACROS
			ret = BE_TOP_Set_DebugPrint(arg);
			#else
			//Added macro code
			#endif
		}
		break;

		case BE_IOR_GET_FIRMWARE_INFO_ADDR:
		{
			#ifndef USE_BE_IOCTL_MACROS
			ret = BE_TOP_Get_FirmwareInfo_Addr(arg);
			#else
			//Added macro code
			#endif

		}
		break;

		case BE_IOR_GET_GIT_DATE_INFO:
		{
			printk("Does not support this DBG anymore...\n");
		}
		break;

		case BE_IOR_GET_BE_KERNEL_DRV_INFO:
		{
			ret = BE_TOP_Get_KernelDrvInfo(arg);
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

int BE_TOP_Reset_SW(void)
{
	int ret = RET_OK;

	do{
		ret = BE_TOP_HAL_ResetSW();
		if(ret) BREAK_WRONG(ret);
	} while(0);
	return ret;
}

int BE_TOP_Set_UartForMcu(unsigned long arg)
{
	int ret = RET_OK;
	BOOLEAN stParams;

	do{
		ret = copy_from_user(&stParams, (BOOLEAN *)arg, sizeof(BOOLEAN));
		if(ret) BREAK_WRONG(ret);
		ret = BE_TOP_HAL_SetUartForMcu(stParams);
		if(ret) BREAK_WRONG(ret);
	} while(0);
	return ret;
}

int BE_TOP_Set_DebugPrint(unsigned long arg)
{
	int ret = RET_OK;
	BE_DEBUG_PRINT_T stParams;

	do{
		ret = copy_from_user(&stParams, (BE_DEBUG_PRINT_T *)arg, sizeof(BE_DEBUG_PRINT_T));
		if(ret) BREAK_WRONG(ret);

		if(stParams.dbgMask & BE_DBG_PRT_MSK_PRINT) // BE_PRINT
		{
			OS_DEBUG_EnableModuleByIndex ( g_be_debug_fd, BE_DBG_PRT_IDX_PRINT, DBG_COLOR_NONE );
		}
		else
		{
			OS_DEBUG_DisableModuleByIndex ( g_be_debug_fd, BE_DBG_PRT_IDX_PRINT );
		}

		if(stParams.dbgMask & BE_DBG_PRT_MSK_ERROR) // BE_ERROR
		{
			OS_DEBUG_EnableModuleByIndex ( g_be_debug_fd, BE_DBG_PRT_IDX_ERROR, DBG_COLOR_UL_RED );
		}
		else
		{
			OS_DEBUG_DisableModuleByIndex ( g_be_debug_fd, BE_DBG_PRT_IDX_ERROR );
		}

		if(stParams.dbgMask & BE_DBG_PRT_MSK_DEBUG) // BE_DEBUG
		{
			OS_DEBUG_EnableModuleByIndex ( g_be_debug_fd, BE_DBG_PRT_IDX_DEBUG, DBG_COLOR_NONE );
		}
		else
		{
			OS_DEBUG_DisableModuleByIndex ( g_be_debug_fd, BE_DBG_PRT_IDX_DEBUG );
		}

		if(stParams.dbgMask & BE_DBG_PRT_MSK_TRACE) // BE_TRACE
		{
			OS_DEBUG_EnableModuleByIndex ( g_be_debug_fd, BE_DBG_PRT_IDX_TRACE, DBG_COLOR_NONE );
		}
		else
		{
			OS_DEBUG_DisableModuleByIndex ( g_be_debug_fd, BE_DBG_PRT_IDX_TRACE );
		}

		switch(stParams.funcSupportDbgLevel)
		{
			case 0 :
				gHalFuncSupportDbgLevel = BE_DBG_LEVEL_ERROR;
				break;
			case 1 :
				gHalFuncSupportDbgLevel = BE_DBG_LEVEL_WARN;
				break;
			case 2 :
				gHalFuncSupportDbgLevel = BE_DBG_LEVEL_IGNORE;
				break;
			default :
				gHalFuncSupportDbgLevel = BE_DBG_LEVEL_ERROR;
		}
	} while(0);
	return ret;
}

int BE_TOP_Set_Bypass(unsigned long arg)
{
	int ret = RET_OK;
	LX_BE_BLOCK_BYPASS_T stParams;

	do{
		ret = copy_from_user(&stParams, (LX_BE_BLOCK_BYPASS_T *)arg, sizeof(LX_BE_BLOCK_BYPASS_T));
		if(ret) BREAK_WRONG(ret);
		ret = BE_TOP_HAL_SetBypass(&stParams);
		if(ret) BREAK_WRONG(ret);
	} while(0);
	return ret;
}

int BE_TOP_Get_KernelDrvInfo(unsigned long arg)
{
	int ret = RET_OK;
	LX_BE_KDRV_INFO_T stParams;

	do{
		stParams.isFrcLowDelayMode = gSKeeperDataBase->table[LX_VP_ST_KEEPER_MODULE_BE].pRsrIndex[BE_SKEEPER_FRC_LOW_DELAY_MODE].status;

		ret = copy_to_user((void __user *)arg, &stParams, sizeof(LX_BE_KDRV_INFO_T));
		if(ret) BREAK_WRONG(ret);
	} while(0);
	return ret;
}


int BE_TOP_Get_Bypass(unsigned long arg)
{
	int ret = RET_OK;
	LX_BE_BLOCK_BYPASS_T stParams;

	do{
		ret = BE_TOP_HAL_GetBypass(&stParams);
		if(ret) BREAK_WRONG(ret);
		ret = copy_to_user((void __user *)arg, &stParams, sizeof(LX_BE_BLOCK_BYPASS_T));
		if(ret) BREAK_WRONG(ret);
	} while(0);
	return ret;
}


int BE_TOP_Toggle_3D_LR(unsigned long arg)
{
	int ret = RET_OK;
	LX_BE_3D_LR_T stParams;

	do{
		ret = copy_from_user(&stParams, (LX_BE_3D_LR_T *)arg, sizeof(LX_BE_3D_LR_T));
		if(ret) BREAK_WRONG(ret);
		ret = BE_TOP_HAL_Toggle3DLR(&stParams);
		if(ret) BREAK_WRONG(ret);
	} while(0);
	return ret;
}

int BE_TOP_Set_3D_In_Format(unsigned long arg)
{
	int ret = RET_OK;
	LX_BE_TOP_3D_IN_FORMAT_T stParams;

	do{
		ret = copy_from_user(&stParams, (LX_BE_TOP_3D_IN_FORMAT_T *)arg, sizeof(LX_BE_TOP_3D_IN_FORMAT_T));
		if(ret) BREAK_WRONG(ret);
		ret = BE_TOP_HAL_Set3DInFormat(&stParams);
		if(ret) BREAK_WRONG(ret);
	} while(0);
	return ret;
}

int BE_TOP_Get_3D_In_Format(unsigned long arg)
{
	int ret = RET_OK;
	LX_BE_TOP_3D_IN_FORMAT_T stParams;

	do{
		ret = BE_TOP_HAL_Get3DInFormat(&stParams);
		if(ret) BREAK_WRONG(ret);
		ret = copy_to_user((void __user *)arg, &stParams, sizeof(LX_BE_TOP_3D_IN_FORMAT_T));
		if(ret) BREAK_WRONG(ret);
	} while(0);
	return ret;
}

int BE_TOP_Set_3D_Out_Format(unsigned long arg)
{
	int ret = RET_OK;
	LX_BE_3D_OUT_FORMAT_T stParams;

	do{
		ret = copy_from_user(&stParams, (LX_BE_3D_OUT_FORMAT_T *)arg, sizeof(LX_BE_3D_OUT_FORMAT_T));
		if(ret) BREAK_WRONG(ret);
		ret = BE_TOP_HAL_Set3DOutFormat(&stParams);
		if(ret) BREAK_WRONG(ret);
	} while(0);
	return ret;
}

int BE_TOP_Set_3D_Convergence(unsigned long arg)
{
	int ret = RET_OK;
	LX_BE_3D_CONVERGENCE_T stParams;

	do{
		ret = copy_from_user(&stParams, (LX_BE_3D_CONVERGENCE_T *)arg, sizeof(LX_BE_3D_CONVERGENCE_T));
		if(ret) BREAK_WRONG(ret);
		ret = BE_TOP_HAL_Set3DConvergence(&stParams);
		if(ret) BREAK_WRONG(ret);
	} while(0);
	return ret;
}

int BE_TOP_Set_Black_Bar(unsigned long arg)
{
	int ret = RET_OK;
	LX_BE_BLACK_BAR_T stParams;

	do{
		ret = copy_from_user(&stParams, (LX_BE_BLACK_BAR_T *)arg, sizeof(LX_BE_BLACK_BAR_T));
		if(ret) BREAK_WRONG(ret);
		ret = BE_TOP_HAL_SetBlackBar(&stParams);
		if(ret) BREAK_WRONG(ret);
	} while(0);
	return ret;
}

int BE_TOP_Set_Video_Mute(unsigned long arg)
{
	int ret = RET_OK;
	LX_BE_VIDEO_MUTE_T stParams;

	do{
		ret = copy_from_user(&stParams, (LX_BE_VIDEO_MUTE_T *)arg, sizeof(LX_BE_VIDEO_MUTE_T));
		if(ret) BREAK_WRONG(ret);
		ret = BE_TOP_HAL_SetVideoMute(&stParams);
		if(ret) BREAK_WRONG(ret);
	} while(0);
	return ret;
}

int BE_TOP_Set_Display_Type(unsigned long arg)
{
	int ret = RET_OK;
	LX_BE_DISPLAY_TYPE_T stParams;

	do{
		ret = copy_from_user(&stParams, (LX_BE_DISPLAY_TYPE_T *)arg, sizeof(LX_BE_DISPLAY_TYPE_T));
		if(ret) BREAK_WRONG(ret);
		ret = BE_TOP_HAL_SetDisplayType(&stParams);
		if(ret) BREAK_WRONG(ret);
	} while(0);
	return ret;
}

int BE_TOP_Set_Debug_Display(unsigned long arg)
{
	int ret = RET_OK;
	LX_BE_DEBUG_DISPLAY_T stParams;

	do{
		ret = copy_from_user(&stParams, (LX_BE_DEBUG_DISPLAY_T *)arg, sizeof(LX_BE_DEBUG_DISPLAY_T));
		if(ret) BREAK_WRONG(ret);
		ret = BE_TOP_HAL_SetDebugDisplay(&stParams);
		if(ret) BREAK_WRONG(ret);
	} while(0);
	return ret;
}

int BE_TOP_Set_VID_Operation(unsigned long arg)
{
	int ret = RET_OK;
	LX_BE_VID_OPERATION_T stParams;

	do{
		ret = copy_from_user(&stParams, (LX_BE_VID_OPERATION_T *)arg, sizeof(LX_BE_VID_OPERATION_T));
		if(ret) BREAK_WRONG(ret);
		ret = BE_TOP_HAL_SetVideoOperation(&stParams);
		if(ret) BREAK_WRONG(ret);
	} while(0);
	return ret;
}

int BE_TOP_Set_Input_Src_Type(unsigned long arg)
{
	int ret = RET_OK;
	LX_BE_SRC_TYPE_T stParams;

	do{
		ret = copy_from_user(&stParams, (LX_BE_SRC_TYPE_T *)arg, sizeof(LX_BE_SRC_TYPE_T));
		if(ret) BREAK_WRONG(ret);
		ret = BE_TOP_HAL_SetInputSrcType(&stParams);
		if(ret) BREAK_WRONG(ret);
	} while(0);
	return ret;
}

int BE_TOP_Set_Active_Window_Start(unsigned long arg)
{
	int ret = RET_OK;
	LX_BE_ACTIVE_WINDOW_START_T stParams;

	do{
		ret = copy_from_user(&stParams, (LX_BE_ACTIVE_WINDOW_START_T *)arg, sizeof(LX_BE_ACTIVE_WINDOW_START_T));
		if(ret) BREAK_WRONG(ret);
		ret = BE_TOP_HAL_SetActiveWindowStart(&stParams);
		if(ret) BREAK_WRONG(ret);
	} while(0);
	return ret;
}

int BE_TOP_Set_Active_Window_Size(unsigned long arg)
{
	int ret = RET_OK;
	LX_BE_ACTIVE_WINDOW_SIZE_T stParams;

	do{
		ret = copy_from_user(&stParams, (LX_BE_ACTIVE_WINDOW_SIZE_T *)arg, sizeof(LX_BE_ACTIVE_WINDOW_SIZE_T));
		if(ret) BREAK_WRONG(ret);
		ret = BE_TOP_HAL_SetActiveWindowSize(&stParams);
		if(ret) BREAK_WRONG(ret);
	} while(0);
	return ret;
}

int BE_TOP_Set_Real_Cinema(unsigned long arg)
{
	int ret = RET_OK;
	LX_BE_REAL_CINEMA_T stParams;

	do{
		ret = copy_from_user(&stParams, (LX_BE_REAL_CINEMA_T *)arg, sizeof(LX_BE_REAL_CINEMA_T));
		if(ret) BREAK_WRONG(ret);
		ret = BE_TOP_HAL_SetRealCinema(&stParams);
		if(ret) BREAK_WRONG(ret);
	} while(0);
	return ret;
}

int BE_TOP_Firmware_Debug(unsigned long arg)
{
	int ret = RET_OK;
	BE_FIRMWARE_DEBUG_T stParams;

	do{
		ret = copy_from_user(&stParams, (void __user *)arg, sizeof(BE_FIRMWARE_DEBUG_T));
		if(ret) BREAK_WRONG(ret);
		ret = BE_TOP_HAL_FirmwareDebug(&stParams);
		if(ret) BREAK_WRONG(ret);

		if(stParams.u8Read)
		{
			ret = copy_to_user((void __user *)arg, &stParams, sizeof(BE_FIRMWARE_DEBUG_T));
			if(ret) BREAK_WRONG(ret);
		}
	} while(0);
	return ret;
}

int BE_TOP_Get_FirmwareInfo_Addr(unsigned long arg)
{
	int ret = RET_OK;
	BE_FIRMWARE_INFO_T stParams;

	do{
		ret = BE_TOP_HAL_Get_FirmwareInfo_Addr(&stParams);
		if(ret) BREAK_WRONG(ret);
		ret = copy_to_user((void __user *)arg, &stParams, sizeof(BE_FIRMWARE_INFO_T));
		if(ret) BREAK_WRONG(ret);
	} while(0);
	return ret;
}
