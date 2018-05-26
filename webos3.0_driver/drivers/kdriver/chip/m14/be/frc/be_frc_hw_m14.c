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
 *	BE FRC module control Drivers linux module
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
#include <linux/fcntl.h>		/**< O_ACCMODE */
#include <asm/uaccess.h>
#include <linux/ioport.h>	/**< For request_region, check_region etc */
#include <asm/io.h>		/**< For ioremap_nocache */
#include "os_util.h"

#include "be_dbg.h"
#include "be_def.h"
#include "../fwi/ipc_def_m14.h"
#include "ipc_drv.h"
#include "be_frc_hw_m14.h"

// For CTOP register access
#include "../../../core/sys/sys_regs.h"

// For temporary
#include "../../../core/de/de_io.h"
#include "de_kapi.h"

#include "ipc_cmd_header.h"

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
	UINT8		u8Judder;
	UINT8		u8Blur;
	UINT8		u8MemcType;
	UINT8		u8LowDelay;
	UINT8		u8RGBYUV;
} BE_FRC_HW_M14_INFO_T;

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
static BE_FRC_HW_M14_INFO_T _beFrcHwM14Info= {0,};

/*========================================================================================
	Implementation Group
========================================================================================*/
int BE_FRC_HW_M14_Initialize(void)
{
	int ret = RET_OK;

	do {
		if (lx_chip_rev() >= LX_CHIP_REV( M14, B0))
		{

		}
		else
		{

		}
	} while (0);

	return ret;
}

int BE_FRC_HW_M14_Close(void)
{
	int ret = RET_OK;

	if (lx_chip_rev() >= LX_CHIP_REV( M14, B0))
	{

	}
	else
	{

	}

	return ret;
}

int BE_FRC_HW_M14_ControlMemc(LX_BE_FRC_MEMC_MODE_T *pstParams)
{
	int ret = RET_OK;
	unsigned int status = 0;

	do {
		if (lx_chip_rev() >= LX_CHIP_REV( M14, B0))
		{
			IPC_SET_DEFAULT_PARAM(BE_IPC_B0_FRC_MEMC_MODE_T, stHwParams);

			/* change value LX_BE to BE_IPC */
			stHwParams.uFRCBlur		= pstParams->u8Blur == 0xFF ? 0xFFFFFFFF : pstParams->u8Blur;
			stHwParams.uFRCJudder	= pstParams->u8Judder == 0xFF ? 0xFFFFFFFF : pstParams->u8Judder;
			stHwParams.uFRCUserLevel	= pstParams->u8MemcType == 0xFF ? 0xFFFFFFFF : pstParams->u8MemcType;

			/* Set value for argument */
			if (stHwParams.uFRCUserLevel == LX_BE_FRC_MEMC_ENABLE)
			{
				stHwParams.uFRCBlur		=	_beFrcHwM14Info.u8Blur;
				stHwParams.uFRCJudder	=	_beFrcHwM14Info.u8Judder;
				stHwParams.uFRCUserLevel	=	_beFrcHwM14Info.u8MemcType;
			}
			else
			{
				if (stHwParams.uFRCBlur != 0xFFFFFFFF)
				_beFrcHwM14Info.u8Blur 	= stHwParams.uFRCBlur;

				if (stHwParams.uFRCJudder != 0xFFFFFFFF)
				_beFrcHwM14Info.u8Judder 	= stHwParams.uFRCJudder;

				if (stHwParams.uFRCUserLevel != 0xFFFFFFFF)
				_beFrcHwM14Info.u8MemcType  = stHwParams.uFRCUserLevel;
			}

			/* For Status Management */
			status = 0x00FFFFFF&((0x00FF0000&(stHwParams.uFRCBlur<<16))|(0x0000FF00&(stHwParams.uFRCJudder<<8))|(0x000000FF&(stHwParams.uFRCUserLevel)));
			(void)VIDEO_SKeeperUpdateIndex(LX_VP_ST_KEEPER_MODULE_BE, BE_SKEEPER_FRC_MEMC_MODE, status);

			/* Send command  to MCU via IPC & Check result */
			ret = BE_KIPC_SetData(FRC_PACK_CMD(BE_FRC_B0_SET_MEMC_MODE), &stHwParams, sizeof(BE_IPC_B0_FRC_MEMC_MODE_T));
			if (ret) BREAK_WRONG(ret);
		}
		else
		{
			IPC_SET_DEFAULT_PARAM(BE_IPC_FRC_MEMC_MODE_T, stHwParams);

			/* change value LX_BE to BE_IPC */
			stHwParams.u8Blur			= pstParams->u8Blur;
			stHwParams.u8Judder		= pstParams->u8Judder;
			stHwParams.u8MemcType	= pstParams->u8MemcType;

			/* Set value for argument */
			if (stHwParams.u8MemcType == LX_BE_FRC_MEMC_ENABLE)
			{
				stHwParams.u8Blur			=	_beFrcHwM14Info.u8Blur;
				stHwParams.u8Judder		=	_beFrcHwM14Info.u8Judder;
				stHwParams.u8MemcType	=	_beFrcHwM14Info.u8MemcType;
			}
			else
			{
				if (stHwParams.u8Blur != 0xFF)
				_beFrcHwM14Info.u8Blur 	= stHwParams.u8Blur;

				if (stHwParams.u8Judder != 0xFF)
				_beFrcHwM14Info.u8Judder 	= stHwParams.u8Judder;

				if (stHwParams.u8MemcType != 0xFF)
				_beFrcHwM14Info.u8MemcType  = stHwParams.u8MemcType;
			}

			/* return unexpected value */
			if (*(ULONG *)&stHwParams == (ULONG)0xFFFFFFFF)
			{
				return ret;
			}

			/* For Status Management */
			status =  0x00FFFFFF&((0x00FF0000&(stHwParams.u8Blur<<16))|(0x0000FF00&(stHwParams.u8Judder<<8))|(0x000000FF&(stHwParams.u8MemcType)));
			(void)VIDEO_SKeeperUpdateIndex(LX_VP_ST_KEEPER_MODULE_BE, BE_SKEEPER_FRC_MEMC_MODE, status);

			/* Send command  to MCU via IPC & Check result */
			ret = BE_KIPC_SetData(BE_FRC_SET_MEMC_MODE, &stHwParams, sizeof(BE_IPC_FRC_MEMC_MODE_T));
			if (ret) BREAK_WRONG(ret);
		}
	} while (0);

	return ret;
}

int BE_FRC_HW_M14_Set_Demo_Mode(LX_BE_FRC_DEMO_MODE_T *pstParams)
{
	int ret = RET_OK;

	do {
		if (lx_chip_rev() >= LX_CHIP_REV( M14, B0))
		{
			/* Do Nothing */
		}
		else
		{
			IPC_SET_DEFAULT_PARAM(BE_IPC_FRC_DEMO_MODE_T, stHwParams);

			/* change value LX_BE to BE_IPC */
			stHwParams.bDemoMode = pstParams->u16DemoMode;
			stHwParams.bDemoSide = pstParams->u16DemoSide;

			/* return unexpected value */
			if (*(ULONG *)&stHwParams == (ULONG)0xFFFFFFFF)
			{
				return ret;
			}

			/* Send command  to MCU via IPC & Check result */
			ret = BE_KIPC_SetData(BE_FRC_SET_DEMO_MODE, &stHwParams, sizeof(BE_IPC_FRC_DEMO_MODE_T));
			if (ret) BREAK_WRONG(ret);
		}
	} while (0);

	return ret;
}

int BE_FRC_HW_M14_Set_Debug_Box(LX_BE_FRC_ENABLE_DEBUG_BOX_T *pstParams)
{
	int ret = RET_OK;

	do {
		if (lx_chip_rev() >= LX_CHIP_REV( M14, B0))
		{
			IPC_SET_DEFAULT_PARAM(BE_IPC_B0_FRC_ENABLE_DEBUG_BOX_T, stHwParams);

			/* change value LX_BE to BE_IPC */
			if (pstParams->u16FRCDebugBoxOn)
			{
				stHwParams.uDeboxMode = pstParams->u16FRCDebugBoxMode;
			}
			else
			{
				stHwParams.uDeboxMode = 0;
			}

			/* Send command  to MCU via IPC & Check result */
			ret = BE_KIPC_SetData(FRC_PACK_CMD(BE_FRC_B0_ENABLE_DEBUG_BOX), &stHwParams, sizeof(BE_IPC_B0_FRC_ENABLE_DEBUG_BOX_T));
			if (ret) BREAK_WRONG(ret);
		}
		else
		{
			IPC_SET_DEFAULT_PARAM(BE_IPC_FRC_ENABLE_DEBUG_BOX_T, stHwParams);

			/* change value LX_BE to BE_IPC */
			stHwParams.u4FRCDebugBoxMode	= pstParams->u16FRCDebugBoxMode;
			stHwParams.u4FRCDebugBoxOn		= pstParams->u16FRCDebugBoxOn;

			/* return unexpected value */
			if (*(ULONG *)&stHwParams == (ULONG)0xFFFFFFFF)
			{		
				return ret;
			}

			/* Send command  to MCU via IPC & Check result */
			ret = BE_KIPC_SetData(BE_FRC_ENABLE_DEBUG_BOX, &stHwParams, sizeof(BE_IPC_FRC_ENABLE_DEBUG_BOX_T));
			if (ret) BREAK_WRONG(ret);
		}
	} while (0);

	return ret;
}

int BE_FRC_HW_M14_Set_Bypass_Memc_Reg(LX_BE_FRC_MEMC_BYPASS_REGION_T *pstParams)
{
	int ret = RET_OK;

	do {
		if (lx_chip_rev() >= LX_CHIP_REV( M14, B0))
		{
			/* Do Nothing */
		}
		else
		{
			IPC_SET_DEFAULT_PARAM(BE_IPC_FRC_MEMC_BYPASS_REGION_T, stHwParams);

			/* change value LX_BE to BE_IPC */
			stHwParams.u32BypassEn.bBypassRegion0	= pstParams->u32BypassEn.u8BypassRegion0;
			stHwParams.u32BypassEn.bBypassRegion1	= pstParams->u32BypassEn.u8BypassRegion1;
			stHwParams.u32BypassEn.bBypassRegion2	= pstParams->u32BypassEn.u8BypassRegion2;
			stHwParams.u32BypassEn.bBypassRegion3	= pstParams->u32BypassEn.u8BypassRegion3;

			stHwParams.u32BypassXY.u16MemcBypassX	= pstParams->u32BypassXY.u16MemcBypassX;
			stHwParams.u32BypassXY.u16MemcBypassY	= pstParams->u32BypassXY.u16MemcBypassY;

			stHwParams.u32BypassWH.u16MemcBypassW	= pstParams->u32BypassWH.u16MemcBypassW;
			stHwParams.u32BypassWH.u16MemcBypassH	= pstParams->u32BypassWH.u16MemcBypassH;

			/* return unexpected value */
			if ( *(ULONG *)&stHwParams.u32BypassEn == (ULONG)0xFFFFFFFF
			&& *(ULONG *)&stHwParams.u32BypassXY == (ULONG)0xFFFFFFFF
			&& *(ULONG *)&stHwParams.u32BypassWH == (ULONG)0xFFFFFFFF )
			{
				return ret;
			}

			/* Send command  to MCU via IPC & Check result */
			ret = BE_KIPC_SetData(BE_FRC_BYPASS_REGION, &(stHwParams.u32BypassEn), sizeof(BE_IPC_FRC_MEMC_BYPASS_ENABLE_T));
			if (ret) BREAK_WRONG(ret);
			ret = BE_KIPC_SetData(BE_FRC_BYPASS_REGION0_XY, &(stHwParams.u32BypassXY), sizeof(BE_IPC_FRC_MEMC_BYPASS_XY_T));
			if (ret) BREAK_WRONG(ret);
			ret = BE_KIPC_SetData(BE_FRC_BYPASS_REGION0_WH, &(stHwParams.u32BypassWH), sizeof(BE_IPC_FRC_MEMC_BYPASS_WH_T));
			if (ret) BREAK_WRONG(ret);

			ret = BE_KIPC_SetData(BE_FRC_BYPASS_REGION1_XY, &(stHwParams.u32BypassXY), sizeof(BE_IPC_FRC_MEMC_BYPASS_XY_T));
			if (ret) BREAK_WRONG(ret);
			ret = BE_KIPC_SetData(BE_FRC_BYPASS_REGION1_WH, &(stHwParams.u32BypassWH), sizeof(BE_IPC_FRC_MEMC_BYPASS_WH_T));
			if (ret) BREAK_WRONG(ret);

			ret = BE_KIPC_SetData(BE_FRC_BYPASS_REGION2_XY, &(stHwParams.u32BypassXY), sizeof(BE_IPC_FRC_MEMC_BYPASS_XY_T));
			if (ret) BREAK_WRONG(ret);
			ret = BE_KIPC_SetData(BE_FRC_BYPASS_REGION2_WH, &(stHwParams.u32BypassWH), sizeof(BE_IPC_FRC_MEMC_BYPASS_WH_T));
			if (ret) BREAK_WRONG(ret);

			ret = BE_KIPC_SetData(BE_FRC_BYPASS_REGION3_XY, &(stHwParams.u32BypassXY), sizeof(BE_IPC_FRC_MEMC_BYPASS_XY_T));
			if (ret) BREAK_WRONG(ret);
			ret = BE_KIPC_SetData(BE_FRC_BYPASS_REGION3_WH, &(stHwParams.u32BypassWH), sizeof(BE_IPC_FRC_MEMC_BYPASS_WH_T));
			if (ret) BREAK_WRONG(ret);
		}
	} while (0);

	return ret;
}

int BE_FRC_HW_M14_Set_YUV_444_Mode(LX_BE_FRC_COLOR_YUV444_T *pstParams)
{
	int ret = RET_OK;
	unsigned int status = 0;

	do {
		if (lx_chip_rev() >= LX_CHIP_REV( M14, B0))
		{
			IPC_SET_DEFAULT_PARAM(BE_IPC_B0_FRC_COLOR_YUV444_T, stHwParams);

			/* change value LX_BE to BE_IPC */
			stHwParams.bFRCProcYUV444 = pstParams->u8ColorYuv == 0xFF ? 0xFFFFFFFF : pstParams->u8ColorYuv;

			/* For Status Management */
			status = (unsigned int)stHwParams.bFRCProcYUV444;
			(void)VIDEO_SKeeperUpdateIndex(LX_VP_ST_KEEPER_MODULE_BE, BE_SKEEPER_FRC_RGB_YUV_MODE, status);


			/* Send command  to MCU via IPC & Check result */
			ret = BE_KIPC_SetData(FRC_PACK_CMD(BE_FRC_B0_SET_YUV444), &stHwParams, sizeof(BE_IPC_B0_FRC_COLOR_YUV444_T));
			if (ret) BREAK_WRONG(ret);
		}
		else
		{
			IPC_SET_DEFAULT_PARAM(BE_IPC_FRC_COLOR_YUV444_T, stHwParams);

			/* change value LX_BE to BE_IPC */
			stHwParams.u4ColorYuv		= pstParams->u8ColorYuv;
			stHwParams.u4ColorDepth	= pstParams->u8ColorDepth;
			stHwParams.u4CompMode	= pstParams->u8CompMode;

			/* return unexpected value */
			if (*(ULONG *)&stHwParams == (ULONG)0xFFFFFFFF)
			{
				return ret;
			}

			/* For Status Management */
			status = (unsigned int)stHwParams.u4ColorYuv;
			(void)VIDEO_SKeeperUpdateIndex(LX_VP_ST_KEEPER_MODULE_BE, BE_SKEEPER_FRC_RGB_YUV_MODE, status);


			/* Send command  to MCU via IPC & Check result */
			ret = BE_KIPC_SetData(BE_FRC_SET_YUV444, &stHwParams, sizeof(BE_IPC_FRC_COLOR_YUV444_T));
			if (ret) BREAK_WRONG(ret);
		}
	} while (0);

	return ret;
}

int BE_FRC_HW_M14_Set_Low_Delay(LX_BE_FRC_LOW_DELAY_T *pstParams)
{
	int ret = RET_OK;
	unsigned int status = 0;

	do {
		if (lx_chip_rev() >= LX_CHIP_REV( M14, B0))
		{
			IPC_SET_DEFAULT_PARAM(BE_IPC_B0_FRC_LOW_DELAY_T, stHwParams);

			/* change value LX_BE to BE_IPC */
			stHwParams.bFRCLowDelay	= pstParams->u32Lowdelay;

			/* For Status Management */
			status = (unsigned int)stHwParams.bFRCLowDelay;
			(void)VIDEO_SKeeperUpdateIndex(LX_VP_ST_KEEPER_MODULE_BE, BE_SKEEPER_FRC_LOW_DELAY_MODE, status);

			/* Send command  to MCU via IPC & Check result */
			ret = BE_KIPC_SetData(FRC_PACK_CMD(BE_FRC_B0_SET_LOW_DELAY), &stHwParams, sizeof(BE_IPC_B0_FRC_LOW_DELAY_T));
			if (ret) BREAK_WRONG(ret);
		}
		else
		{
			IPC_SET_DEFAULT_PARAM(BE_IPC_FRC_LOW_DELAY_T, stHwParams);

			/* change value LX_BE to BE_IPC */
			stHwParams.bLowdelay	= pstParams->u32Lowdelay;

			/* return unexpected value */
			if (*(ULONG *)&stHwParams == (ULONG)0xFFFFFFFF)
			{
				return ret;
			}

			/* For Status Management */
			status = (unsigned int)stHwParams.bLowdelay;
			(void)VIDEO_SKeeperUpdateIndex(LX_VP_ST_KEEPER_MODULE_BE, BE_SKEEPER_FRC_LOW_DELAY_MODE, status);

			/* Send command  to MCU via IPC & Check result */
			ret = BE_KIPC_SetData(BE_FRC_SET_LOW_DELAY, &stHwParams, sizeof(BE_IPC_FRC_LOW_DELAY_T));
			if (ret) BREAK_WRONG(ret);
		}
	} while (0);

	return ret;
}

int BE_FRC_HW_M14_Get_Firmware_Ver(LX_BE_FRC_VERSION_T *pstParams)
{
	int ret = RET_OK;

	do {
		if (lx_chip_rev() >= LX_CHIP_REV( M14, B0))
		{
			/* Do Nothing */
		}
		else
		{
			IPC_SET_GET_PARAM(BE_IPC_FRC_VERSION_T, stHwParams);

			/* Send command  to MCU via IPC & Check result */
			ret = BE_KIPC_SetData(BE_FRC_GET_FIRMWARE_VERSION, &stHwParams, sizeof(BE_IPC_FRC_VERSION_T));
			if (ret) BREAK_WRONG(ret);
			ret = BE_KIPC_GetData(&stHwParams, sizeof(BE_IPC_FRC_VERSION_T));
			if (ret) BREAK_WRONG(ret);

			/* change value BE_IPC to BE_IPCLX_BE */
			pstParams->u16Version	= stHwParams.u16Version;
			pstParams->u16Subversion	= stHwParams.u16Subversion;
		}
	} while (0);

	return ret;
}

int BE_FRC_HW_M14_Force_Frame_Rate(LX_BE_FRC_FRAME_RATE_T *pstParams)
{
	int ret = RET_OK;

	do {
		if (lx_chip_rev() >= LX_CHIP_REV( M14, B0))
		{
			/* Do Nothing */
		}
		else
		{
			IPC_SET_DEFAULT_PARAM(BE_IPC_FRC_FRAME_RATE_T, stHwParams);

			/* change value LX_BE to BE_IPC */
			stHwParams.u8FrameRate	= pstParams->u32FrameRate;

			/* return unexpected value */
			if (*(ULONG *)&stHwParams == (ULONG)0xFFFFFFFF)
			{
				return ret;
			}

			/* Send command  to MCU via IPC & Check result */
			ret = BE_KIPC_SetData(BE_FRC_FORCE_FRAME_RATE, &stHwParams, sizeof(BE_IPC_FRC_FRAME_RATE_T));
			if (ret) BREAK_WRONG(ret);
		}
	} while (0);

	return ret;
}

int BE_FRC_HW_M14_Set_Freeze(LX_BE_FRC_FREEZE_T *pstParams)
{
	int ret = RET_OK;
	unsigned int status;

	do {
		if (lx_chip_rev() >= LX_CHIP_REV( M14, B0))
		{
			IPC_SET_DEFAULT_PARAM(BE_IPC_B0_FRC_FREEZE_T, stHwParams);

			/* change value LX_BE to BE_IPC */
			stHwParams.bFRCFreeze	= pstParams->u32Freeze;

			/* For Status Management */
			status = (unsigned int)stHwParams.bFRCFreeze;
			(void)VIDEO_SKeeperUpdateIndex(LX_VP_ST_KEEPER_MODULE_BE, BE_SKEEPER_FRC_FREEZE, status);

			/* Send command  to MCU via IPC & Check result */
			ret = BE_KIPC_SetData(FRC_PACK_CMD(BE_FRC_B0_SET_FREEZE), &stHwParams, sizeof(BE_IPC_B0_FRC_FREEZE_T));
			if (ret) BREAK_WRONG(ret);
		}
		else
		{
			/* Do Nothing */
		}
	} while (0);

	return ret;
}

int BE_FRC_HW_M14_Update_FrameDelay(LX_BE_LOW_DELAY_T *pstParams)
{
	int ret = RET_OK;

	do {
		if (lx_chip_rev() >= LX_CHIP_REV( M14, B0))
		{
			//ret = DE_IO_GetLowDelay((unsigned long)pstParams);
			/* Never use DE_IO on BE kdriver */
		}
		else
		{
			/* Do Nothing */
		}
	} while (0);

	return ret;
}
/** @} */
