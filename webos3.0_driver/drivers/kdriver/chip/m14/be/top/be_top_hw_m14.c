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
#include "be_cfg.h"
#include "be_def.h"
#include "../fwi/ipc_def_m14.h"
#include "ipc_drv.h"
#include "be_top_hw_m14.h"

// For CTOP register access
#include "../../../core/sys/sys_regs.h"

#include "ipc_cmd_header.h"

// For Status Management
#include "../../../core/be/video_status_keeper.h"


/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
#define OSD_MIRROR_ADDR_Ax	0xC0035200
#define OSD_MIRROR_ADDR_Bx	0xC0024200

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/* For register direct access
 * 2013-10-14 chobits : Need to change this ...*/

static unsigned int *gMapped_address;
#define BE_REG_WR(address,data)   \
	gMapped_address=(volatile UINT32 *)ioremap(address, sizeof(UINT32));\
	do{\
		if(gMapped_address == NULL) break;\
		*gMapped_address = data;\
		iounmap((void *)gMapped_address);\
	}while(0);\

#define BE_REG_RD(address,data)   \
	gMapped_address = (volatile UINT32 *)ioremap(address, sizeof(UINT32));\
	do{\
		if(gMapped_address == NULL) {data = 0; break;}\
		data = (unsigned int)*gMapped_address;\
		iounmap((void *)gMapped_address);\
	}while(0);\

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/
typedef struct {
	
} BE_TOP_HW_M14_INFO_T;

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/
extern BE_MEM_CFG_T *gpBeMem;
/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
static BE_TOP_HW_M14_INFO_T beTopHwM14Info;

/*========================================================================================
	Implementation Group
========================================================================================*/
int BE_TOP_HW_M14_Initialize(void)
{
	int ret = RET_OK;

	do {
		if(lx_chip_rev() >= LX_CHIP_REV( M14, B0))
		{
			
		}
		else
		{

		}
	} while(0);

	return ret;
}

int BE_TOP_HW_M14_Close(void)
{
	int ret = RET_OK;

	if(lx_chip_rev() >= LX_CHIP_REV( M14, B0))
	{
		
	}
	else
	{

	}
		
	return ret;
}

int BE_TOP_HW_M14_ResetSW(void)
{
	int ret = RET_OK;

	if (lx_chip_rev() >= LX_CHIP_REV( M14, B0))
	{
		/* Do Nothing */
	}
	else
	{
		CTOP_CTRL_M14A0_RdFL(ctr29);
		CTOP_CTRL_M14A0_Wr01(ctr29, swrst_dppb_cpu, 1); // MCU reset 1
		CTOP_CTRL_M14A0_WrFL(ctr29);
		
		CTOP_CTRL_M14A0_RdFL(ctr29);
		CTOP_CTRL_M14A0_Wr01(ctr29, swrst_dppb_cpu, 0); // MCU reset 0
		CTOP_CTRL_M14A0_WrFL(ctr29);
	}

	return ret;
}

int BE_TOP_HW_M14_SetUartForMcu(BOOLEAN isForMcu)
{
	int ret = RET_OK;

	if (lx_chip_rev() >= LX_CHIP_REV( M14, B0))
	{
		/* Do Nothing */
	}
	else
	{
		CTOP_CTRL_M14A0_RdFL(ctr58);
		if(isForMcu)
		{
			CTOP_CTRL_M14A0_Wr01(ctr58, uart0_sel, 6); // UART0 =hdp 
			CTOP_CTRL_M14A0_Wr01(ctr58, uart1_sel, 2); // UART1 = cpu0
			CTOP_CTRL_M14A0_Wr01(ctr58, rx_sel_hdpmcu1, 0); // 0:HDP from UART0
		}
		else
		{
			CTOP_CTRL_M14A0_Wr01(ctr58, uart0_sel, 2); // UART0 = cpu0
			CTOP_CTRL_M14A0_Wr01(ctr58, uart1_sel, 1); // UART1 = de
			CTOP_CTRL_M14A0_Wr01(ctr58, rx_sel_hdpmcu0, 1); // 2:DE from UART1
		}
		CTOP_CTRL_M14A0_WrFL(ctr58);
	}

	return ret;
}

int BE_TOP_HW_M14_SetBypass(LX_BE_BLOCK_BYPASS_T *pstParams)
{
	int ret = RET_OK;

	do{
		if (lx_chip_rev() >= LX_CHIP_REV( M14, B0))
		{
			IPC_SET_DEFAULT_PARAM(BE_IPC_B0_BLOCK_BYPASS_T, stHwParams);
			
			if ( pstParams->bH3DBypass >1 || pstParams->bFRCBypass > 1 || pstParams->bSREBypass > 1 ||
				pstParams->bPEBypass > 1 || pstParams->bLEDBypass > 1 || pstParams->bHDRBypass > 1 || pstParams->bWCGBypass > 1 )
			{
				BE_ERROR("Invalied input params, only use [bypass 1: on, 0: off]\n");
				return RET_INVALID_PARAMS;
			}
			
			/* change value LX_BE to BE_IPC */
			stHwParams.uH3DBypass = pstParams->bH3DBypass;
			stHwParams.uFRCBypass = pstParams->bFRCBypass;
			stHwParams.uWCGBypass = pstParams->bWCGBypass;
			
			ret = BE_KIPC_SetData(TOP_PACK_CMD(BE_TOP_B0_SET_BLOCK_BYPASS), (void*)&stHwParams, sizeof(BE_IPC_B0_BLOCK_BYPASS_T));
			if(ret) BREAK_WRONG(ret);
		}
		else
		{
			IPC_SET_DEFAULT_PARAM(BE_IPC_BLOCK_BYPASS_T, stHwParams);
			
			if ( pstParams->bH3DBypass >1 || pstParams->bFRCBypass > 1 || pstParams->bSREBypass > 1 ||
				pstParams->bPEBypass > 1 || pstParams->bLEDBypass > 1 || pstParams->bHDRBypass > 1 || pstParams->bWCGBypass > 1 )
			{
				BE_ERROR("Invalied input params, only use [bypass 1: on, 0: off]\n");
				return RET_INVALID_PARAMS;
			}
			
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
		}
	} while(0);
	
	return ret;
}

int BE_TOP_HW_M14_GetBypass(LX_BE_BLOCK_BYPASS_T *pstParams)
{
	int ret = RET_OK;

	do{
		if (lx_chip_rev() >= LX_CHIP_REV( M14, B0))
		{
			/* Do Nothing */
		}
		else
		{
			IPC_SET_GET_PARAM(BE_IPC_BLOCK_BYPASS_T, stHwParams);
			
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
		}
	} while(0);
	
	return ret;
}

int BE_TOP_HW_M14_Toggle3DLR(LX_BE_3D_LR_T *pstParams)
{
	int ret = RET_OK;
	unsigned int status;

	do{
		if (lx_chip_rev() >= LX_CHIP_REV( M14, B0))
		{
			IPC_SET_DEFAULT_PARAM(BE_IPC_B0_3D_LR_T, stHwParams);
			
			/* change value LX_BE to BE_IPC */
			stHwParams.bOutLR = pstParams->bOutLR == 0xFFFF ? 0xFFFFFFFF : pstParams->bOutLR;
		
			/* Video Status */
			status = stHwParams.bOutLR;
			(void)VIDEO_SKeeperUpdateIndex(LX_VP_ST_KEEPER_MODULE_BE, BE_SKEEPER_H3D_LR_TOGGLE, status);

			ret = BE_KIPC_SetData(TOP_PACK_CMD(BE_TOP_B0_TOGGLE_3D_LR), (void*)&stHwParams, sizeof(BE_IPC_B0_3D_LR_T));
			if(ret) BREAK_WRONG(ret);
		}
		else
		{
			IPC_SET_DEFAULT_PARAM(BE_IPC_3D_LR_T, stHwParams);
			
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
		}
	} while(0);
	
	return ret;
}

int BE_TOP_HW_M14_Set3DInFormat(LX_BE_TOP_3D_IN_FORMAT_T *pstParams)
{
	int ret = RET_OK;
		
	do{
		if (lx_chip_rev() >= LX_CHIP_REV( M14, B0))
		{
			/* Do Nothing */
		}
		else
		{
			IPC_SET_DEFAULT_PARAM(BE_IPC_TOP_3D_IN_FORMAT_T, stHwParams);
			
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
		}
	} while(0);
	
	return ret;
}

int BE_TOP_HW_M14_Get3DInFormat(LX_BE_TOP_3D_IN_FORMAT_T *pstParams)
{
	int ret = RET_OK;

	do{
		if (lx_chip_rev() >= LX_CHIP_REV( M14, B0))
		{
			/* Do Nothing */
		}
		else
		{
			IPC_SET_GET_PARAM(BE_IPC_TOP_3D_IN_FORMAT_T, stHwParams);
			
			/* Send command  to MCU via IPC & Check result */
			ret = BE_KIPC_SetData(BE_TOP_SET_3D_IN_FORMAT, (void *)&stHwParams, sizeof(BE_IPC_TOP_3D_IN_FORMAT_T));
			if(ret) BREAK_WRONG(ret);
			ret = BE_KIPC_GetData((void *)&stHwParams, sizeof(BE_IPC_TOP_3D_IN_FORMAT_T));
			if(ret) BREAK_WRONG(ret);

			/* change value BE_IPC to BE_IPCLX_BE */
			pstParams->u8BE3DInFormat =  stHwParams.u4BE3DInFormat;
			pstParams->u8BE3DInSize =  stHwParams.u4BE3DInSize;
			pstParams->u8BE3DInType =  stHwParams.u4BE3DInType;
		}
	} while(0);
	
	return ret;
}

int BE_TOP_HW_M14_Set3DOutFormat(LX_BE_3D_OUT_FORMAT_T *pstParams)
{
	int ret = RET_OK;
		
	do{
		if (lx_chip_rev() >= LX_CHIP_REV( M14, B0))
		{
			IPC_SET_DEFAULT_PARAM(BE_IPC_B0_3D_OUT_FORMAT_T, stHwParams);
			
			/* change value LX_BE to BE_IPC */
			stHwParams.uTDPGMode = pstParams->u8PGMode == 0xFF ? 0xFFFFFFFF : pstParams->u8PGMode;
			stHwParams.uTDSingleOn = pstParams->u8Single == 0xFF ? 0xFFFFFFFF : pstParams->u8Single;
			
			ret = BE_KIPC_SetData(TOP_PACK_CMD(BE_TOP_B0_SET_3D_OUT_FORMAT), (void *)&stHwParams, sizeof(BE_IPC_B0_3D_OUT_FORMAT_T));
			if(ret) BREAK_WRONG(ret);
		}
		else
		{
			IPC_SET_DEFAULT_PARAM(BE_IPC_3D_OUT_FORMAT_T, stHwParams);
			
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
		}
	} while(0);
	
	return ret;
}

int BE_TOP_HW_M14_Set3DConvergence(LX_BE_3D_CONVERGENCE_T *pstParams)
{
	int ret = RET_OK;
	unsigned int status;	

	do{
		if (lx_chip_rev() >= LX_CHIP_REV( M14, B0))
		{
			IPC_SET_DEFAULT_PARAM(BE_IPC_B0_3D_CONVERGENCE_T, stHwParams);

			/* change value LX_BE to BE_IPC */
			stHwParams.bTDConvEnable = pstParams->u16ConvergenceEnable == 0xFFFF ? 0xFFFFFFFF : pstParams->u16ConvergenceEnable;
			stHwParams.uTDConvVal = pstParams->u16ConvergenceVal == 0xFFFF ? 0xFFFFFFFF : pstParams->u16ConvergenceVal;

			/* Video Status */
			status = (0xFFFF0000&(stHwParams.bTDConvEnable <<16))|(0x0000FFFF&(stHwParams.uTDConvVal));
			(void)VIDEO_SKeeperUpdateIndex(LX_VP_ST_KEEPER_MODULE_BE, BE_SKEEPER_H3D_CONVERGENCE, status);

			ret = BE_KIPC_SetData(TOP_PACK_CMD(BE_TOP_B0_SET_3D_CONVERGENCE), (void *)&stHwParams, sizeof(BE_IPC_B0_3D_CONVERGENCE_T));
			if(ret) BREAK_WRONG(ret);
		}
		else
		{
			IPC_SET_DEFAULT_PARAM(BE_IPC_3D_CONVERGENCE_T, stHwParams);

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
		}
	} while(0);
	
	return ret;
}

int BE_TOP_HW_M14_SetBlackBar(LX_BE_BLACK_BAR_T *pstParams)
{
	int ret = RET_OK;
	
	do{
		if (lx_chip_rev() >= LX_CHIP_REV( M14, B0))
		{
			IPC_SET_DEFAULT_PARAM(BE_IPC_B0_BLACK_BAR_T, stHwParams);
			
			/* change value LX_BE to BE_IPC */
			stHwParams.uTDBlackBarEnable = pstParams->u8BarOn == 0xFF ? 0xFFFFFFFF : pstParams->u8BarOn;
			stHwParams.uTDBlackBarVal = pstParams->u8BarVal == 0xFF ? 0xFFFFFFFF : pstParams->u8BarVal;
			
			ret = BE_KIPC_SetData(TOP_PACK_CMD(BE_TOP_B0_SET_BLACK_BAR), (void *)&stHwParams, sizeof(BE_IPC_B0_BLACK_BAR_T));
			if(ret) BREAK_WRONG(ret);
		}
		else
		{
			IPC_SET_DEFAULT_PARAM(BE_IPC_BE_BLACK_BAR_T, stHwParams);
			
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
		}
	} while(0);
	
	return ret;
}

int BE_TOP_HW_M14_SetVideoMute(LX_BE_VIDEO_MUTE_T *pstParams)
{
	int ret = RET_OK;
	
	do{
		if (lx_chip_rev() >= LX_CHIP_REV( M14, B0))
		{
			/* Do Nothing */
		}
		else
		{
			IPC_SET_DEFAULT_PARAM(BE_IPC_VIDEO_MUTE_T, stHwParams);
			
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
		}
	} while(0);
	
	return ret;
}

int BE_TOP_HW_M14_SetDisplayType(LX_BE_DISPLAY_TYPE_T *pstParams)
{
	int ret = RET_OK;
	unsigned int regValueForOsd;

	do{
		if (lx_chip_rev() >= LX_CHIP_REV( M14, B0))
		{
			IPC_SET_DEFAULT_PARAM(BE_IPC_B0_VIDEO_TYPE_T, stHwParams1);
			IPC_SET_DEFAULT_PARAM(BE_IPC_B0_PANEL_INFO_T, stHwParams2);
			
			/* change value LX_BE to BE_IPC */
			stHwParams1.uMainVideoType = pstParams->u32BEPanelType;
			stHwParams2.uPanelRate = pstParams->u32BEPanelRate;
			stHwParams2.uPanelRvrsEnable = pstParams->u32BEPanelReverse;
			
			ret = BE_KIPC_SetData(TOP_PACK_CMD(BE_TOP_B0_SET_VIDEO_TYPE), (void *)&stHwParams1, sizeof(BE_IPC_B0_VIDEO_TYPE_T));
			if(ret) BREAK_WRONG(ret);
			ret = BE_KIPC_SetData(TOP_PACK_CMD(BE_TOP_B0_SET_PANEL_INFO), (void *)&stHwParams2, sizeof(BE_IPC_B0_PANEL_INFO_T));
			if(ret) BREAK_WRONG(ret);

			/* Reverse for OSD */
			if(pstParams->u32BEPanelReverse != 0xFFFFFFFF) {
				/* Read OSD reg */
				BE_REG_RD(OSD_MIRROR_ADDR_Bx ,regValueForOsd);
				
				/* Set value for OSD reg */
				if(pstParams->u32BEPanelReverse == 1){
					regValueForOsd |= 0x00000100;	
				}
				else if(pstParams->u32BEPanelReverse == 0){
					regValueForOsd &= (~0x00000100);	
				}

				BE_REG_WR(OSD_MIRROR_ADDR_Bx ,regValueForOsd);
			}

		}
		else
		{
			IPC_SET_DEFAULT_PARAM(BE_IPC_DISPLAY_TYPE_T, stHwParams);
			
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
		
			/* Reverse for OSD */
			if(pstParams->u32BEPanelReverse != 0xFFFFFFFF) {
				/* Read OSD reg */
				BE_REG_RD(OSD_MIRROR_ADDR_Ax ,regValueForOsd);
				
				/* Set value for OSD reg */
				if(pstParams->u32BEPanelReverse == 1){
					regValueForOsd |= 0x00000100;	
				}
				else if(pstParams->u32BEPanelReverse == 0){
					regValueForOsd &= (~0x00000100);	
				}
				
				BE_REG_WR(OSD_MIRROR_ADDR_Ax ,regValueForOsd);
			}
		}
	} while(0);
	
	return ret;
}

int BE_TOP_HW_M14_SetDebugDisplay(LX_BE_DEBUG_DISPLAY_T *pstParams)
{
	int ret = RET_OK;
	
	do{
		if (lx_chip_rev() >= LX_CHIP_REV( M14, B0))
		{
			IPC_SET_DEFAULT_PARAM(BE_IPC_B0_DEBUG_DISPLAY_T, stHwParams);
			
			/* change value LX_BE to BE_IPC */
			stHwParams.uH3DDebugdisplay = pstParams->u8H3DDebugDisplay == 0xFF ? 0xFFFFFFFF : pstParams->u8H3DDebugDisplay;
			stHwParams.uFRCDebugDisplay = pstParams->u8FRCDebugDisplay == 0xFF ? 0xFFFFFFFF : pstParams->u8FRCDebugDisplay;
			stHwParams.uPEDebugDisplay = pstParams->u8PEDebugDisplay == 0xFF ? 0xFFFFFFFF : pstParams->u8PEDebugDisplay;
			stHwParams.uLEDDebugDisplay = pstParams->u8LEDDebugDisplay == 0xFF ? 0xFFFFFFFF : pstParams->u8LEDDebugDisplay;
			
			ret = BE_KIPC_SetData(TOP_PACK_CMD(BE_TOP_B0_SET_DEBUG_DISPLAY), (void *)&stHwParams, sizeof(BE_IPC_B0_DEBUG_DISPLAY_T));
			if(ret) BREAK_WRONG(ret);
		}
		else
		{
			IPC_SET_DEFAULT_PARAM(BE_IPC_DEBUG_DISPLAY_T, stHwParams);
			
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
		}
	} while(0);
	
	return ret;
}

int BE_TOP_HW_M14_SetVideoOperation(LX_BE_VID_OPERATION_T *pstParams)
{
	int ret = RET_OK;
	
	do{
		if (lx_chip_rev() >= LX_CHIP_REV( M14, B0))
		{
			IPC_SET_DEFAULT_PARAM(BE_IPC_B0_VID_OPERATION_T, stHwParams);
			
			/* change value LX_BE to BE_IPC */
			stHwParams.uVideoOperation = pstParams->u32Mode;
			
			ret = BE_KIPC_SetData(TOP_PACK_CMD(BE_TOP_B0_SET_VID_OPERATION), (void *)&stHwParams, sizeof(BE_IPC_B0_VID_OPERATION_T));
			if(ret) BREAK_WRONG(ret);
		}
		else
		{
			IPC_SET_DEFAULT_PARAM(BE_IPC_VID_OPERATION_T, stHwParams);
			
			/* change value LX_BE to BE_IPC */
			stHwParams.u4Mode = pstParams->u32Mode;
			
			/* return unexpected value */
			if(*(ULONG *)&stHwParams == (ULONG)0xFFFFFFFF)
			{
				return ret;
			}
			
			ret = BE_KIPC_SetData(BE_TOP_SET_VID_OPERATION, (void *)&stHwParams, sizeof(BE_IPC_VID_OPERATION_T));
			if(ret) BREAK_WRONG(ret);
		}
	} while(0);
	
	return ret;
}

int BE_TOP_HW_M14_SetInputSrcType(LX_BE_SRC_TYPE_T *pstParams)
{
	int ret = RET_OK;
	
	do{
		if (lx_chip_rev() >= LX_CHIP_REV( M14, B0))
		{
			/* Do Nothing */
		}
		else
		{
			IPC_SET_DEFAULT_PARAM(BE_IPC_SRC_TYPE_T, stHwParams);
			
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
		}
	} while(0);
	
	return ret;
}

int BE_TOP_HW_M14_SetActiveWindowStart(LX_BE_ACTIVE_WINDOW_START_T *pstParams)
{
	int ret = RET_OK;
		
	do{
		if (lx_chip_rev() >= LX_CHIP_REV( M14, B0))
		{
			/* Do Nothing */
		}
		else
		{
			IPC_SET_DEFAULT_PARAM(BE_IPC_ACTIVE_WINDOW_START_T, stHwParams);
			
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
		}
	} while(0);
	
	return ret;
}

int BE_TOP_HW_M14_SetActiveWindowSize(LX_BE_ACTIVE_WINDOW_SIZE_T *pstParams)
{
	int ret = RET_OK;
	
	do{
		if (lx_chip_rev() >= LX_CHIP_REV( M14, B0))
		{
			/* Do Nothing */
		}
		else
		{
			IPC_SET_DEFAULT_PARAM(BE_IPC_ACTIVE_WINDOW_SIZE_T, stHwParams);
			
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
		}
	} while(0);
	
	return ret;
}

int BE_TOP_HW_M14_FirmwareDebug(BE_FIRMWARE_DEBUG_T *pstParams)
{
	int ret = RET_OK;
	
	do{
		if (lx_chip_rev() >= LX_CHIP_REV( M14, B0))
		{
			/* Do Nothing */
		}
		else
		{
			ret = BE_KIPC_SetData(pstParams->u32Cmd, &(pstParams->u32Param), sizeof(pstParams->u32Param));
			if(ret) BREAK_WRONG(ret);

			if(pstParams->u8Read)
			{
				ret = BE_KIPC_GetData(&(pstParams->u32Param), sizeof(pstParams->u32Param));
				if(ret) BREAK_WRONG(ret);
			}
		}
	} while(0);
	
	return ret;
}

int BE_TOP_HW_M14_Get_FirmwareInfo_Addr(BE_FIRMWARE_INFO_T *pstParams)
{
	int ret = RET_ERROR;
	BE_CH_MEM_T *pFwMem = NULL;

	do{
		pFwMem = (BE_CH_MEM_T *)&gpBeMem->fw[5];
		
		if (pFwMem == NULL) {
			BE_ERROR("[%s:%d] pFwMem is NULL\n", __F__, __L__);	
			break;
		}

		if (lx_chip_rev() >= LX_CHIP_REV( M14, B0))
		{
			pstParams->addr = pFwMem->fw_base;
			pstParams->size = pFwMem->fw_size;
			ret = RET_OK;	
		}
		else
		{
			BE_ERROR("Firmware Info read operation not supported at M14A0\n");
		}
	} while(0);
	
	return ret;
}

/** @} */
