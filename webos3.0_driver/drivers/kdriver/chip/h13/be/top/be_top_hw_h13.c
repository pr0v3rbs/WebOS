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
 *	BE TOP module control Drivers linux module
 *
 *  author      dj911.kim@lge.com
 *  version		1.0
 *  date
 *  note		Additional information.
 *
 *  @addtogroup lg115x_be
 *	@{
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>	/**< printk() */
#include <linux/slab.h> 	/**< kmalloc() */
#include <linux/fs.h> 		/**< everything\ldots{} */
#include <linux/types.h> 	/**< size_t */
#include <linux/fcntl.h>	/**< O_ACCMODE */
#include <asm/uaccess.h>
#include <linux/ioport.h>	/**< For request_region, check_region etc */
#include <asm/io.h>			/**< For ioremap_nocache */
#include "os_util.h"

#include "be_dbg.h"
#include "be_def.h"
#include "../fwi/ipc_def_h13.h"
#include "ipc_drv.h"
#include "be_top_hw_h13.h"

// For CTOP register access
#include "../../../core/sys/sys_regs.h"


// For Status Management
#include "../../../core/be/video_status_keeper.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/
typedef struct {
	
} BE_TOP_HW_H13_INFO_T;

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
static BE_TOP_HW_H13_INFO_T beTopHwH13Info;

/*========================================================================================
	Implementation Group
========================================================================================*/
int BE_TOP_HW_H13_Initialize(void)
{
	int ret = RET_OK;

	do {
		if(lx_chip_rev() >= LX_CHIP_REV( H13, B0))
		{
			
		}
		else
		{

		}
	} while(0);

	return ret;
}

int BE_TOP_HW_H13_Close(void)
{
	int ret = RET_OK;

	if(lx_chip_rev() >= LX_CHIP_REV( H13, B0))
	{
		
	}
	else
	{

	}
		
	return ret;
}

int BE_TOP_HW_H13_ResetSW(void)
{
	int ret = RET_OK;

	CTOP_CTRL_H13A0_RdFL(ctr29);
	CTOP_CTRL_H13A0_Wr01(ctr29, swrst_dppb_cpu, 1); // MCU reset 1
	CTOP_CTRL_H13A0_WrFL(ctr29);
	
	CTOP_CTRL_H13A0_RdFL(ctr29);
	CTOP_CTRL_H13A0_Wr01(ctr29, swrst_dppb_cpu, 0); // MCU reset 0
	CTOP_CTRL_H13A0_WrFL(ctr29);
	
	return ret;
}

int BE_TOP_HW_H13_SetUartForMcu(BOOLEAN isForMcu)
{
	int ret = RET_OK;

	CTOP_CTRL_H13A0_RdFL(ctr58);
	if(isForMcu)
	{
		CTOP_CTRL_H13A0_Wr01(ctr58, uart0_sel, 6); // UART0 =hdp 
		CTOP_CTRL_H13A0_Wr01(ctr58, uart1_sel, 2); // UART1 = cpu0
		CTOP_CTRL_H13A0_Wr01(ctr58, rx_sel_hdpmcu1, 0); // 0:HDP from UART0
	}
	else
	{
		CTOP_CTRL_H13A0_Wr01(ctr58, uart0_sel, 2); // UART0 = cpu0
		CTOP_CTRL_H13A0_Wr01(ctr58, uart1_sel, 1); // UART1 = de
		CTOP_CTRL_H13A0_Wr01(ctr58, rx_sel_hdpmcu0, 1); // 2:DE from UART1
	}
	CTOP_CTRL_H13A0_WrFL(ctr58);

	return ret;
}

int BE_TOP_HW_H13_SetBypass(LX_BE_BLOCK_BYPASS_T *pstParams)
{
	int ret = RET_OK;
	IPC_SET_DEFAULT_PARAM(BE_IPC_BLOCK_BYPASS_T, stHwParams);

	do{
		/* change value LX_BE to BE_IPC */
		stHwParams.bH3DBypass = pstParams->bH3DBypass;
		stHwParams.bFRCBypass = pstParams->bFRCBypass;
		stHwParams.bSREBypass = pstParams->bSREBypass;
		stHwParams.bPEBypass = pstParams->bPEBypass;
		stHwParams.bLEDBypass = pstParams->bLEDBypass;
		stHwParams.bHDRBypass = pstParams->bHDRBypass;
		stHwParams.bWCGBypass = pstParams->bWCGBypass;

		/* return unexpected value */
		if(*(ULONG *)&stHwParams == (ULONG)0xFFFFFFFF)
		{
			return ret;
		}
		
		ret = BE_KIPC_SetData(BE_TOP_SET_BLOCK_BYPASS, (void*)&stHwParams, sizeof(BE_IPC_BLOCK_BYPASS_T));
		if(ret) BREAK_WRONG(ret);
	} while(0);
	
	return ret;
}

int BE_TOP_HW_H13_GetBypass(LX_BE_BLOCK_BYPASS_T *pstParams)
{
	int ret = RET_OK;
	IPC_SET_GET_PARAM(BE_IPC_BLOCK_BYPASS_T, stHwParams);

	do{
		ret = BE_KIPC_SetData(BE_TOP_SET_BLOCK_BYPASS, (void*)&stHwParams, sizeof(BE_IPC_BLOCK_BYPASS_T));
		if(ret) BREAK_WRONG(ret);
		ret = BE_KIPC_GetData((void *)&stHwParams, sizeof(BE_IPC_BLOCK_BYPASS_T));
		if(ret) BREAK_WRONG(ret);

		/* change value BE_IPC to BE_IPCLX_BE */
		pstParams->bH3DBypass = stHwParams.bH3DBypass;
		pstParams->bFRCBypass = stHwParams.bFRCBypass;
		pstParams->bSREBypass = stHwParams.bSREBypass;
		pstParams->bPEBypass = stHwParams.bPEBypass;
		pstParams->bLEDBypass = stHwParams.bLEDBypass;
		pstParams->bHDRBypass = stHwParams.bHDRBypass;
		pstParams->bWCGBypass = stHwParams.bWCGBypass;
	} while(0);
	
	return ret;
}

int BE_TOP_HW_H13_Toggle3DLR(LX_BE_3D_LR_T *pstParams)
{
	int ret = RET_OK;
	unsigned int status;

	IPC_SET_GET_PARAM(BE_IPC_3D_LR_T, stHwParams);
	
	do{
		/* change value LX_BE to BE_IPC */
		stHwParams.bOutLR = pstParams->bOutLR;

		/* return unexpected value */
		if(*(ULONG *)&stHwParams == (ULONG)0xFFFFFFFF)
		{
			return ret;
		}	

		/* Video Status */
		status = stHwParams.bOutLR;
		(void)VIDEO_SKeeperUpdateIndex(LX_VP_ST_KEEPER_MODULE_BE, BE_SKEEPER_H3D_LR_TOGGLE, status);


		ret = BE_KIPC_SetData(BE_TOP_TOGGLE_3D_LR, (void*)&stHwParams, sizeof(BE_IPC_3D_LR_T));
		if(ret) BREAK_WRONG(ret);
	} while(0);

	return ret;
}

int BE_TOP_HW_H13_Set3DInFormat(LX_BE_TOP_3D_IN_FORMAT_T *pstParams)
{
	int ret = RET_OK;
	IPC_SET_GET_PARAM(BE_IPC_TOP_3D_IN_FORMAT_T, stHwParams);
	
	
	do{
		/* change value LX_BE to BE_IPC */
		stHwParams.u4BE3DInFormat = pstParams->u8BE3DInFormat;
		stHwParams.u4BE3DInSize = pstParams->u8BE3DInSize;
		stHwParams.u4BE3DInType = pstParams->u8BE3DInType;

		/* return unexpected value */
		if(*(ULONG *)&stHwParams == (ULONG)0xFFFFFFFF)
		{
			return ret;
		}	
		
		ret = BE_KIPC_SetData(BE_TOP_SET_3D_IN_FORMAT, (void*)&stHwParams, sizeof(BE_IPC_TOP_3D_IN_FORMAT_T));
		if(ret) BREAK_WRONG(ret);
	} while(0);
	
	return ret;
}

int BE_TOP_HW_H13_Get3DInFormat(LX_BE_TOP_3D_IN_FORMAT_T *pstParams)
{
	int ret = RET_OK;
	IPC_SET_GET_PARAM(BE_IPC_TOP_3D_IN_FORMAT_T, stHwParams);
		
	do{
		/* Send command  to MCU via IPC & Check result */
		ret = BE_KIPC_SetData(BE_TOP_SET_3D_IN_FORMAT, (void*)&stHwParams, sizeof(BE_IPC_TOP_3D_IN_FORMAT_T));
		if(ret) BREAK_WRONG(ret);
		ret = BE_KIPC_GetData((void *)&stHwParams, sizeof(BE_IPC_TOP_3D_IN_FORMAT_T));
		if(ret) BREAK_WRONG(ret);

		/* change value BE_IPC to BE_IPCLX_BE */
		pstParams->u8BE3DInFormat = stHwParams.u4BE3DInFormat;
		pstParams->u8BE3DInSize = stHwParams.u4BE3DInSize;
		pstParams->u8BE3DInType = stHwParams.u4BE3DInType;
	} while(0);
	
	return ret;
}

int BE_TOP_HW_H13_Set3DOutFormat(LX_BE_3D_OUT_FORMAT_T *pstParams)
{
	int ret = RET_OK;
	IPC_SET_GET_PARAM(BE_IPC_3D_OUT_FORMAT_T, stHwParams);
		
	do{
		/* change value LX_BE to BE_IPC */
		stHwParams.u4GSyncEnable = pstParams->u8GSyncEnable;
		stHwParams.u4OutFormat = pstParams->u8OutFormat;
		stHwParams.u4PGMode = pstParams->u8PGMode;
		stHwParams.u4Single = pstParams->u8Single;

		/* return unexpected value */
		if(*(ULONG *)&stHwParams == (ULONG)0xFFFFFFFF)
		{
			return ret;
		}	
		
		ret = BE_KIPC_SetData(BE_TOP_SET_3D_OUT_FORMAT, (void *)&stHwParams, sizeof(BE_IPC_3D_OUT_FORMAT_T));
		if(ret) BREAK_WRONG(ret);
	} while(0);
	
	return ret;
}

int BE_TOP_HW_H13_Set3DConvergence(LX_BE_3D_CONVERGENCE_T *pstParams)
{
	int ret = RET_OK;
	unsigned int status;

	IPC_SET_GET_PARAM(BE_IPC_3D_CONVERGENCE_T, stHwParams);
		
	do{
		/* change value LX_BE to BE_IPC */
		stHwParams.u16ConvergenceEnable = pstParams->u16ConvergenceEnable;
		stHwParams.u16ConvergenceVal = pstParams->u16ConvergenceVal;

		/* return unexpected value */
		if(*(ULONG *)&stHwParams == (ULONG)0xFFFFFFFF)
		{
			return ret;
		}
		
		/* Video Status */
		status = (0xFFFF0000&(stHwParams.u16ConvergenceEnable<<16))|(0x0000FFFF&(stHwParams.u16ConvergenceVal));
		(void)VIDEO_SKeeperUpdateIndex(LX_VP_ST_KEEPER_MODULE_BE, BE_SKEEPER_H3D_CONVERGENCE, status);

		ret = BE_KIPC_SetData(BE_TOP_SET_3D_CONVERGENCE, (void *)&stHwParams, sizeof(BE_IPC_3D_CONVERGENCE_T));
		if(ret) BREAK_WRONG(ret);
	} while(0);

	return ret;
}

int BE_TOP_HW_H13_SetBlackBar(LX_BE_BLACK_BAR_T *pstParams)
{
	int ret = RET_OK;
	IPC_SET_GET_PARAM(BE_IPC_BE_BLACK_BAR_T, stHwParams);
		
	do{
		/* change value LX_BE to BE_IPC */
		stHwParams.u8BarOn = pstParams->u8BarOn;
		stHwParams.u8BarStep = pstParams->u8BarStep;
		stHwParams.u8BarVal = pstParams->u8BarVal;

		/* return unexpected value */
		if(*(ULONG *)&stHwParams == (ULONG)0xFFFFFFFF)
		{
			return ret;
		}
		
		ret = BE_KIPC_SetData(BE_TOP_SET_BLACK_BAR, (void *)&stHwParams, sizeof(BE_IPC_BE_BLACK_BAR_T));
		if(ret) BREAK_WRONG(ret);
	} while(0);
	
	return ret;
}

int BE_TOP_HW_H13_SetVideoMute(LX_BE_VIDEO_MUTE_T *pstParams)
{
	int ret = RET_OK;
	IPC_SET_DEFAULT_PARAM(BE_IPC_VIDEO_MUTE_T, stHwParams);
	
	do{
		/* change value LX_BE to BE_IPC */
		stHwParams.bVideoMuteOn = pstParams->bVideoMuteOn;
		stHwParams.u4DEInRate = pstParams->u32DEInRate;
		stHwParams.u4DEOutRate = pstParams->u32DEOutRate;

		/* return unexpected value */
		if(*(ULONG *)&stHwParams == (ULONG)0xFFFFFFFF)
		{
			return ret;
		}
		
		ret = BE_KIPC_SetData(BE_TOP_SET_VIDEO_MUTE, (void *)&stHwParams, sizeof(BE_IPC_VIDEO_MUTE_T));
		if(ret) BREAK_WRONG(ret);
	} while(0);
	
	return ret;
}

int BE_TOP_HW_H13_SetDisplayType(LX_BE_DISPLAY_TYPE_T *pstParams)
{
	int ret = RET_OK;
	IPC_SET_DEFAULT_PARAM(BE_IPC_DISPLAY_TYPE_T, stHwParams);
	
	do{
		/* change value LX_BE to BE_IPC */
		stHwParams.u4BEPanelType = pstParams->u32BEPanelType;
		stHwParams.u4BEPanelRate = pstParams->u32BEPanelRate;
		stHwParams.u4BEPanelReverse = pstParams->u32BEPanelReverse;

		/* return unexpected value */
		if(*(ULONG *)&stHwParams == (ULONG)0xFFFFFFFF)
		{
			return ret;
		}
		
		ret = BE_KIPC_SetData(BE_TOP_SET_DISPLAY_TYPE, (void *)&stHwParams, sizeof(BE_IPC_DISPLAY_TYPE_T));
		if(ret) BREAK_WRONG(ret);
	} while(0);
	
	return ret;
}

int BE_TOP_HW_H13_SetDebugDisplay(LX_BE_DEBUG_DISPLAY_T *pstParams)
{
	int ret = RET_OK;
	IPC_SET_DEFAULT_PARAM(BE_IPC_DEBUG_DISPLAY_T, stHwParams);
	
	do{
		/* change value LX_BE to BE_IPC */
		stHwParams.u8H3DDebugDisplay = pstParams->u8H3DDebugDisplay;
		stHwParams.u8FRCDebugDisplay = pstParams->u8FRCDebugDisplay;
		stHwParams.u8PEDebugDisplay = pstParams->u8PEDebugDisplay;
		stHwParams.u8LEDDebugDisplay = pstParams->u8LEDDebugDisplay;
		
		/* return unexpected value */
		if(*(ULONG *)&stHwParams == (ULONG)0xFFFFFFFF)
		{
			return ret;
		}
		
		ret = BE_KIPC_SetData(BE_TOP_SET_DEBUG_DISPLAY, (void *)&stHwParams, sizeof(BE_IPC_DEBUG_DISPLAY_T));
		if(ret) BREAK_WRONG(ret);
	} while(0);
	
	return ret;
}

int BE_TOP_HW_H13_SetVideoOperation(LX_BE_VID_OPERATION_T *pstParams)
{
	int ret = RET_OK;
	IPC_SET_DEFAULT_PARAM(BE_IPC_VID_OPERATION_T, stHwParams);
	
	do{
		/* change value LX_BE to BE_IPC */
		stHwParams.u4Mode = pstParams->u32Mode;
		
		/* return unexpected value */
		if(*(ULONG *)&stHwParams == (ULONG)0xFFFFFFFF)
		{
			return ret;
		}
		
		ret = BE_KIPC_SetData(BE_TOP_SET_VID_OPERATION, (void *)&stHwParams, sizeof(BE_IPC_VID_OPERATION_T));
		if(ret) BREAK_WRONG(ret);
	} while(0);
	
	return ret;
}

int BE_TOP_HW_H13_SetInputSrcType(LX_BE_SRC_TYPE_T *pstParams)
{
	int ret = RET_OK;
	IPC_SET_DEFAULT_PARAM(BE_IPC_SRC_TYPE_T, stHwParams);
	
	do{
		/* change value LX_BE to BE_IPC */
		stHwParams.u16SrcInputType = pstParams->u32SrcInputType;
		stHwParams.u16SrcInputSize = pstParams->u32SrcInputSize;
		
		/* return unexpected value */
		if(*(ULONG *)&stHwParams == (ULONG)0xFFFFFFFF)
		{
			return ret;
		}
		
		ret = BE_KIPC_SetData(BE_TOP_SET_SRC_TYPE, (void *)&stHwParams, sizeof(BE_IPC_SRC_TYPE_T));
		if(ret) BREAK_WRONG(ret);
	} while(0);
	
	return ret;
}

int BE_TOP_HW_H13_SetActiveWindowStart(LX_BE_ACTIVE_WINDOW_START_T *pstParams)
{
	int ret = RET_OK;
	IPC_SET_GET_PARAM(BE_IPC_ACTIVE_WINDOW_START_T, stHwParams);
		
	do{
		/* change value LX_BE to BE_IPC */
		stHwParams.u16ActiveWindowStartX = pstParams->u16ActiveWindowStartX;
		stHwParams.u16ActiveWindowStartY = pstParams->u16ActiveWindowStartY;

		/* return unexpected value */
		if(*(ULONG *)&stHwParams == (ULONG)0xFFFFFFFF)
		{
			return ret;
		}
		
		ret = BE_KIPC_SetData(BE_TOP_SET_ACTIVE_WINDOW_START, (void *)&stHwParams, sizeof(BE_IPC_ACTIVE_WINDOW_START_T));
		if(ret) BREAK_WRONG(ret);
	} while(0);
	
	return ret;
}

int BE_TOP_HW_H13_SetActiveWindowSize(LX_BE_ACTIVE_WINDOW_SIZE_T *pstParams)
{
	int ret = RET_OK;
	IPC_SET_GET_PARAM(BE_IPC_ACTIVE_WINDOW_SIZE_T, stHwParams);
		
	do{
		/* change value LX_BE to BE_IPC */
		stHwParams.u16ActiveWindowHeight = pstParams->u16ActiveWindowHeight;
		stHwParams.u16ActiveWindowWidth = pstParams->u16ActiveWindowWidth;

		/* return unexpected value */
		if(*(ULONG *)&stHwParams == (ULONG)0xFFFFFFFF)
		{
			return ret;
		}
		
		ret = BE_KIPC_SetData(BE_TOP_SET_ACTIVE_WINDOW_SIZE, (void *)&stHwParams, sizeof(BE_IPC_ACTIVE_WINDOW_SIZE_T));
		if(ret) BREAK_WRONG(ret);
	} while(0);
	
	return ret;
}

int BE_TOP_HW_H13_FirmwareDebug(BE_FIRMWARE_DEBUG_T *pstParams)
{
	int ret = RET_OK;
	
	do{
		ret = BE_KIPC_SetData(pstParams->u32Cmd, &(pstParams->u32Param), sizeof(pstParams->u32Param));
		if(ret) BREAK_WRONG(ret);

		if(pstParams->u8Read)
		{
			ret = BE_KIPC_GetData(&(pstParams->u32Param), sizeof(pstParams->u32Param));
			if(ret) BREAK_WRONG(ret);
		}
	} while(0);
	
	return ret;
}

/** @} */
