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
 *  hal file for apr device
 *	
 *  author		justine.jeong
 *  version		1.0
 *  date		2012.07.10
 *  note		Additional information.
 *
 *  @addtogroup apr
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

#include "base_types.h"
#include "os_util.h"
#include "debug_util.h"

#include "apr_def.h"
#include "apr_core.h"

#ifdef USE_KDRV_CODES_FOR_H15
#include "../../chip/h15/apr/apr_reg_h15.h"
#endif
#ifdef USE_KDRV_CODES_FOR_H14
#include "../../chip/h14/apr/apr_reg_h14.h"
#endif
#ifdef USE_KDRV_CODES_FOR_M14
#include "../../chip/m14/apr/apr_reg_m14.h"
#endif
#ifdef USE_KDRV_CODES_FOR_H13
#include "../../chip/h13/apr/apr_reg_h13.h"
#endif
#ifdef USE_KDRV_CODES_FOR_L9
#include "../../chip/l9/apr/apr_reg_l9.h"
#endif

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
int APR_HAL_Init(void)
{
	int ret = RET_OK;

#ifdef USE_KDRV_CODES_FOR_H15
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H15) ) {
		ret = APR_H15_Init();
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_H14
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H14) ) {
		ret = APR_H14_Init();
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_M14
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_M14) ) {
		ret = APR_M14_Init();
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_H13
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H13) ) {
		ret = APR_H13_Init();
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_L9
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_L9) ) {
		ret = APR_L9_Init();
	} else
#endif
	{
	}

	return ret;
}

int APR_HAL_Uninit(void)
{
	int ret = RET_OK;

#ifdef USE_KDRV_CODES_FOR_H15
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H15) ) {
		ret = APR_H15_Uninit();
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_H14
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H14) ) {
		ret = APR_H14_Uninit();
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_M14
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_M14) ) {
		ret = APR_M14_Uninit();
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_H13
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H13) ) {
		ret = APR_H13_Uninit();
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_L9
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_L9) ) {
		ret = APR_L9_Uninit();
	} else
#endif
	{
	}

	return ret;
}

int APR_HAL_InitHardware(void)
{
	int ret = RET_OK;

#ifdef USE_KDRV_CODES_FOR_H15
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H15) ) {
		ret = APR_H15_InitHardware();
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_H14
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H14) ) {
		ret = APR_H14_InitHardware();
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_M14
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_M14) ) {
		ret = APR_M14_InitHardware();
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_H13
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H13) ) {
		ret = APR_H13_InitHardware();
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_L9
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_L9) ) {
		ret = APR_L9_InitHardware();
	} else
#endif
	{
	}

	return ret;
}

int APR_HAL_CollectBasicData(APR_CFG_T *apr_cfg, APR_DTA_T *apr_data)
{
	int ret = RET_OK;

#ifdef USE_KDRV_CODES_FOR_H15
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H15) ) {
		ret = APR_H15_CollectBasicData(apr_cfg, apr_data);
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_H14
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H14) ) {
		ret = APR_H14_CollectBasicData(apr_cfg, apr_data);
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_M14
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_M14) ) {
		ret = APR_M14_CollectBasicData(apr_cfg, apr_data);
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_H13
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H13) ) {
		ret = APR_H13_CollectBasicData(apr_cfg, apr_data);
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_L9
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_L9) ) {
		ret = APR_L9_CollectBasicData(apr_cfg, apr_data);
	} else
#endif
	{
	}

	return ret;
}

int APR_HAL_GetOVInfo(APR_CFG_T *apr_cfg, APR_CAP_SRC_T ov_src, APR_OV_INFO_T *ov_info)
{
	int ret = RET_OK;

#ifdef USE_KDRV_CODES_FOR_H15
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H15) ) {
		ret = APR_H15_GetOVInfo(apr_cfg, ov_src, ov_info);
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_H14
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H14) ) {
		ret = APR_H14_GetOVInfo(apr_cfg, ov_src, ov_info);
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_M14
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_M14) ) {
		ret = APR_M14_GetOVInfo(apr_cfg, ov_src, ov_info);
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_H13
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H13) ) {
		ret = APR_H13_GetOVInfo(apr_cfg, ov_src, ov_info);
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_L9
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_L9) ) {
		ret = APR_L9_GetOVInfo(apr_cfg, ov_src, ov_info);
	} else
#endif
	{
	}

	return ret;
}

int APR_HAL_GetHistInfo(APR_HST_INFO_T *hist_info)
{
	int ret = RET_OK;

#ifdef USE_KDRV_CODES_FOR_H15
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H15) ) {
		ret = APR_H15_GetHistInfo(hist_info);
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_H14
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H14) ) {
		ret = APR_H14_GetHistInfo(hist_info);
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_M14
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_M14) ) {
		ret = APR_M14_GetHistInfo(hist_info);
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_H13
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H13) ) {
		ret = APR_H13_GetHistInfo(hist_info);
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_L9
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_L9) ) {
	} else
#endif
	{
	}

	return ret;
}

int APR_HAL_GetUSecTimerVal(UINT32 *tv)
{
	int ret = RET_OK;

#ifdef USE_KDRV_CODES_FOR_H15
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H15) ) {
		ret = APR_H15_GetUSecTimerVal(tv);
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_H14
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H14) ) {
		ret = APR_H14_GetUSecTimerVal(tv);
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_M14
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_M14) ) {
		ret = APR_M14_GetUSecTimerVal(tv);
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_H13
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H13) ) {
		ret = APR_H13_GetUSecTimerVal(tv);
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_L9
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_L9) ) {
		ret = APR_L9_GetUSecTimerVal(tv);
	} else
#endif
	{
	}

	return ret;
}

int APR_HAL_SetFd3Config(APR_CFG_T *apr_cfg)
{
	int ret = RET_OK;

#ifdef USE_KDRV_CODES_FOR_H15
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H15) ) {
		ret = APR_H15_SetFd3Config(apr_cfg);
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_H14
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H14) ) {
		ret = APR_H14_SetFd3Config(apr_cfg);
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_M14
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_M14) ) {
		ret = APR_M14_SetFd3Config(apr_cfg);
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_H13
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H13) ) {
		if (lx_chip_rev( ) >= LX_CHIP_REV(H13,B0) )
			ret = APR_H13_SetFd3Config(apr_cfg);
	} else
#endif
	{
	}

	return ret;
}

int APR_HAL_CollectFd3Data(APR_DTA_T *apr_data)
{
	int ret = RET_OK;

#ifdef USE_KDRV_CODES_FOR_H15
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H15) ) {
		ret = APR_H15_CollectFd3Data(apr_data);
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_H14
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H14) ) {
		ret = APR_H14_CollectFd3Data(apr_data);
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_M14
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_M14) ) {
		ret = APR_M14_CollectFd3Data(apr_data);
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_H13
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H13) ) {
		if (lx_chip_rev( ) >= LX_CHIP_REV(H13,B0) )
			ret = APR_H13_CollectFd3Data(apr_data);
	} else
#endif
	{
	}

	return ret;
}

int APR_HAL_DetectBlackBar(APR_CFG_T *apr_cfg, APR_DTA_T *apr_data)
{
	int ret = RET_OK;

#ifdef USE_KDRV_CODES_FOR_H15
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H15) ) {
		ret = APR_H15_DetectBlackBar(apr_cfg, apr_data);
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_H14
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H14) ) {
		ret = APR_H14_DetectBlackBar(apr_cfg, apr_data);
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_M14
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_M14) ) {
		ret = APR_M14_DetectBlackBar(apr_cfg, apr_data);
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_H13
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H13) ) {
		if (lx_chip_rev( ) >= LX_CHIP_REV(H13,B0) )
			ret = APR_H13_DetectBlackBar(apr_cfg, apr_data);
	} else
#endif
	{
	}

	return ret;
}

int APR_HAL_SetFd3SubtConfig(APR_CFG_T *apr_cfg, int alt_seg)
{
	int ret = RET_OK;

#ifdef USE_KDRV_CODES_FOR_H15
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H15) ) {
		ret = APR_H15_SetFd3SubtConfig(apr_cfg, alt_seg);
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_H14
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H14) ) {
		ret = APR_H14_SetFd3SubtConfig(apr_cfg, alt_seg);
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_M14
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_M14) ) {
		ret = APR_M14_SetFd3SubtConfig(apr_cfg, alt_seg);
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_H13
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H13) ) {
		if (lx_chip_rev( ) >= LX_CHIP_REV(H13,B0) )
			ret = APR_H13_SetFd3SubtConfig(apr_cfg, alt_seg);
	} else
#endif
	{
	}

	return ret;
}

int APR_HAL_SetCurrentInfo(APR_CFG_T *apr_cfg, APR_DTA_T *apr_data)
{
	int ret = RET_OK;

#ifdef USE_KDRV_CODES_FOR_H15
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H15) ) {
		ret = APR_H15_SetCurrentInfo(apr_cfg, apr_data);
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_H14
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H14) ) {
		ret = APR_H14_SetCurrentInfo(apr_cfg, apr_data);
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_M14
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_M14) ) {
		ret = APR_M14_SetCurrentInfo(apr_cfg, apr_data);
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_H13
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H13) ) {
		if (lx_chip_rev( ) >= LX_CHIP_REV(H13,B0) )
			ret = APR_H13_SetCurrentInfo(apr_cfg, apr_data);
	} else
#endif
	{
	}

	return ret;
}

int APR_HAL_GetCapacity(APR_CAPA_INFO_T *capa)
{
	int ret = RET_OK;

#ifdef USE_KDRV_CODES_FOR_H15
		if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H15) ) {
			capa->max_src_width = 3840;
		} else
#endif
#ifdef USE_KDRV_CODES_FOR_H14
		if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H14) ) {
			capa->max_src_width = 1920;
		} else
#endif
#ifdef USE_KDRV_CODES_FOR_M14
		if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_M14) ) {
			capa->max_src_width = 1920;
		} else
#endif
#ifdef USE_KDRV_CODES_FOR_H13
		if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H13) ) {
			capa->max_src_width = 1920;
		} else
#endif
		{
			capa->max_src_width = 1920;
		}

	return ret;
}

