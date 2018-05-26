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
#include "../fwi/ipc_def_h15.h"
#include "ipc_drv.h"
#include "be_top_hw_h15.h"

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

} BE_TOP_HW_H15_INFO_T;

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
static BE_TOP_HW_H15_INFO_T beTopHwH15Info;

/*========================================================================================
	Implementation Group
========================================================================================*/
int BE_TOP_HW_H15_Initialize(void)
{
	int ret = RET_OK;

	do {

	} while(0);

	return ret;
}

int BE_TOP_HW_H15_Close(void)
{
	int ret = RET_OK;

	do {

	} while(0);

	return ret;
}

int BE_TOP_HW_H15_SetBypass(LX_BE_BLOCK_BYPASS_T *pstParams)
{
	int ret = RET_OK;

	do{
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
	} while(0);

	return ret;
}

int BE_TOP_HW_H15_Toggle3DLR(LX_BE_3D_LR_T *pstParams)
{
	int ret = RET_OK;
	unsigned int status;

	do{
		IPC_SET_DEFAULT_PARAM(BE_IPC_B0_3D_LR_T, stHwParams);

		/* change value LX_BE to BE_IPC */
		stHwParams.bOutLR = pstParams->bOutLR == 0xFFFF ? 0xFFFFFFFF : pstParams->bOutLR;

		/* Video Status */
		status = stHwParams.bOutLR;
		(void)VIDEO_SKeeperUpdateIndex(LX_VP_ST_KEEPER_MODULE_BE, BE_SKEEPER_H3D_LR_TOGGLE, status);

		ret = BE_KIPC_SetData(TOP_PACK_CMD(BE_TOP_B0_TOGGLE_3D_LR), (void*)&stHwParams, sizeof(BE_IPC_B0_3D_LR_T));
		if(ret) BREAK_WRONG(ret);
	} while(0);

	return ret;
}

int BE_TOP_HW_H15_Set3DOutFormat(LX_BE_3D_OUT_FORMAT_T *pstParams)
{
	int ret = RET_OK;
#if 0
	do{
		IPC_SET_DEFAULT_PARAM(BE_IPC_B0_3D_OUT_FORMAT_T, stHwParams);

		/* change value LX_BE to BE_IPC */
		stHwParams.uTDPGMode = pstParams->u8PGMode == 0xFF ? 0xFFFFFFFF : pstParams->u8PGMode;
		stHwParams.uTDSingleOn = pstParams->u8Single == 0xFF ? 0xFFFFFFFF : pstParams->u8Single;

		ret = BE_KIPC_SetData(TOP_PACK_CMD(BE_TOP_B0_SET_3D_OUT_FORMAT), (void *)&stHwParams, sizeof(BE_IPC_B0_3D_OUT_FORMAT_T));
		if(ret) BREAK_WRONG(ret);
	} while(0);
#endif
	return ret;
}

int BE_TOP_HW_H15_Set3DConvergence(LX_BE_3D_CONVERGENCE_T *pstParams)
{
	int ret = RET_OK;
	unsigned int status;

	do{
		IPC_SET_DEFAULT_PARAM(BE_IPC_B0_3D_CONVERGENCE_T, stHwParams);

		/* change value LX_BE to BE_IPC */
		stHwParams.bTDConvEnable = pstParams->u16ConvergenceEnable == 0xFFFF ? 0xFFFFFFFF : pstParams->u16ConvergenceEnable;
		stHwParams.uTDConvVal = pstParams->u16ConvergenceVal == 0xFFFF ? 0xFFFFFFFF : pstParams->u16ConvergenceVal;

		/* Video Status */
		status = (0xFFFF0000&(stHwParams.bTDConvEnable <<16))|(0x0000FFFF&(stHwParams.uTDConvVal));
		(void)VIDEO_SKeeperUpdateIndex(LX_VP_ST_KEEPER_MODULE_BE, BE_SKEEPER_H3D_CONVERGENCE, status);

		ret = BE_KIPC_SetData(TOP_PACK_CMD(BE_TOP_B0_SET_3D_CONVERGENCE), (void *)&stHwParams, sizeof(BE_IPC_B0_3D_CONVERGENCE_T));
		if(ret) BREAK_WRONG(ret);
	} while(0);

	return ret;
}

int BE_TOP_HW_H15_SetBlackBar(LX_BE_BLACK_BAR_T *pstParams)
{
	int ret = RET_OK;

	do{
		IPC_SET_DEFAULT_PARAM(BE_IPC_B0_BLACK_BAR_T, stHwParams);

		/* change value LX_BE to BE_IPC */
		stHwParams.uTDBlackBarEnable = pstParams->u8BarOn == 0xFF ? 0xFFFFFFFF : pstParams->u8BarOn;
		stHwParams.uTDBlackBarVal = pstParams->u8BarVal == 0xFF ? 0xFFFFFFFF : pstParams->u8BarVal;

		ret = BE_KIPC_SetData(TOP_PACK_CMD(BE_TOP_B0_SET_BLACK_BAR), (void *)&stHwParams, sizeof(BE_IPC_B0_BLACK_BAR_T));
		if(ret) BREAK_WRONG(ret);
	} while(0);

	return ret;
}

int BE_TOP_HW_H15_SetRealCinema(LX_BE_REAL_CINEMA_T *pstParams)
{
	int ret = RET_OK;

	do{
		IPC_SET_DEFAULT_PARAM(BE_IPC_B0_REAL_CINEMA_T, stHwParams);

		/* change value LX_BE to BE_IPC */
		stHwParams.uRealCinemaEnable = pstParams->u32RealCinemaEnable;

		ret = BE_KIPC_SetData(TOP_PACK_CMD(BE_TOP_B0_SET_REAL_CINEMA), (void *)&stHwParams, sizeof(BE_IPC_B0_REAL_CINEMA_T));
		if(ret) BREAK_WRONG(ret);
	} while(0);

	return ret;
}

int BE_TOP_HW_H15_SetDebugDisplay(LX_BE_DEBUG_DISPLAY_T *pstParams)
{
	int ret = RET_OK;

	do{
		IPC_SET_DEFAULT_PARAM(BE_IPC_B0_DEBUG_DISPLAY_T, stHwParams);

		/* change value LX_BE to BE_IPC */
		stHwParams.uH3DDebugdisplay = pstParams->u8H3DDebugDisplay == 0xFF ? 0xFFFFFFFF : pstParams->u8H3DDebugDisplay;
		stHwParams.uPEDebugDisplay = pstParams->u8PEDebugDisplay == 0xFF ? 0xFFFFFFFF : pstParams->u8PEDebugDisplay;
		stHwParams.uLEDDebugDisplay = pstParams->u8LEDDebugDisplay == 0xFF ? 0xFFFFFFFF : pstParams->u8LEDDebugDisplay;

		ret = BE_KIPC_SetData(TOP_PACK_CMD(BE_TOP_B0_SET_DEBUG_DISPLAY), (void *)&stHwParams, sizeof(BE_IPC_B0_DEBUG_DISPLAY_T));
		if(ret) BREAK_WRONG(ret);
	} while(0);

	return ret;
}

int BE_TOP_HW_H15_Get_FirmwareInfo_Addr(BE_FIRMWARE_INFO_T *pstParams)
{
	int ret = RET_OK;
	BE_CH_MEM_T *pFwMem = NULL;

	do{
		pFwMem = (BE_CH_MEM_T *)&gpBeMem->fw[5];

		if (pFwMem == NULL) {
			ret = RET_ERROR;
			BE_ERROR("[%s:%d] pFwMem is NULL\n", __F__, __L__);
			break;
		}

		pstParams->addr = pFwMem->fw_base;
		pstParams->size = pFwMem->fw_size;
	} while(0);

	return ret;
}

/** @} */
