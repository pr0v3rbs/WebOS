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
 * main driver implementation for de device.
 * de device will teach you how to make device driver with new platform.
 *
 * author     jaemo.kim (jaemo.kim@lge.com), justine.jeong@lge.com
 * version    1.0
 * date       2012.03.14
 * note       Additional information.
 *
 * @addtogroup lg1150_denc
 * @{
 */

/*----------------------------------------------------------------------------------------
 *   Control Constants
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/
#include <linux/sched.h>
#include <linux/wait.h>
#include <linux/ctype.h>
#include <linux/semaphore.h>
#include <asm/uaccess.h>
#include <asm/io.h>


#include "base_types.h"
#include "os_util.h"

#include "../de/de_model.h"
#include "../de/de_ver_def.h"
#include "denc_kapi.h"

#ifdef USE_KDRV_CODES_FOR_L9
#include "../../chip/l9/denc/denc_reg_l9.h"
#endif
#ifdef USE_KDRV_CODES_FOR_H13
#include "../../chip/h13/denc/denc_reg_h13.h"
#endif
#ifdef USE_KDRV_CODES_FOR_M14
#include "../../chip/m14/denc/denc_reg_m14.h"
#endif
#ifdef USE_KDRV_CODES_FOR_H14
#include "../../chip/h14/denc/denc_reg_h14.h"
#endif
#ifdef USE_KDRV_CODES_FOR_H15
#include "../../chip/h15/denc/denc_reg_h15.h"
#endif

/*----------------------------------------------------------------------------------------
 *   Constant Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Macro Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Type Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   External Function Prototype Declarations
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   External Variables
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   global Functions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   global Variables
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Static Function Prototypes Declarations
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Static Variables
 *---------------------------------------------------------------------------------------*/

/*========================================================================================
 *   Implementation Group
 *=======================================================================================*/
/**
 * @callgraph
 * @callergraph
 *
 * @brief Initialize DENC module.
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DENC_HAL_Init(void)
{
	int ret = RET_OK;

#ifdef USE_KDRV_CODES_FOR_H15
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H15) ) {
		ret = DENC_REG_H15_Init();
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_H14
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H14) ) {
		ret = DENC_REG_H14_Init();
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_M14
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_M14) ) {
		ret = DENC_REG_M14_Init();
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_H13
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H13) ) {
		ret = DENC_REG_H13_Init();
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_L9
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_L9) ) {
		ret = DENC_REG_L9_Init();
	} else
#endif
	{
	}
	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief NTSC/PAL/SECAM encoder enable(TRUE : enable, FALSE : disable)
 *
 * @param pstParams [IN] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DENC_HAL_OnOff(BOOLEAN *pstParams)
{
	int ret = RET_OK;

#ifdef USE_KDRV_CODES_FOR_H15
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H15) ) {
		ret = DENC_REG_H15_OnOff(pstParams);
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_H14
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H14) ) {
		ret = DENC_REG_H14_OnOff(pstParams);
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_M14
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_M14) ) {
		ret = DENC_REG_M14_OnOff(pstParams);
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_H13
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H13) ) {
		ret = DENC_REG_H13_OnOff(pstParams);
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_L9
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_L9) ) {
		ret = DENC_REG_L9_OnOff(pstParams);
	} else
#endif
	{
	}
	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief Select the color standard of CVE.
 *
 * @param pstParams [IN] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DENC_HAL_SetOutPutVideoStd(LX_DENC_VIDEO_SYSTEM_T *pstParams)
{
	int ret = RET_OK;

#ifdef USE_KDRV_CODES_FOR_H15
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H15) ) {
		ret = DENC_REG_H15_SetOutPutVideoStd(pstParams);
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_H14
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H14) ) {
		ret = DENC_REG_H14_SetOutPutVideoStd(pstParams);
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_M14
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_M14) ) {
		ret = DENC_REG_M14_SetOutPutVideoStd(pstParams);
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_H13
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H13) ) {
		ret = DENC_REG_H13_SetOutPutVideoStd(pstParams);
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_L9
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_L9) ) {
		ret = DENC_REG_L9_SetOutPutVideoStd(pstParams);
	} else
#endif
	{
	}
	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief Select the input source of CVE.
 *
 * @param pstParams [IN] address of buffer to be carried
 *
 * @return RET_OK(0)
 */
int DENC_HAL_SetSource(LX_DENC_VIDEO_SOURCE_T *pstParams)
{
	int ret = RET_OK;

#ifdef USE_KDRV_CODES_FOR_H15
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H15) ) {
		ret = DENC_REG_H15_SetSource(pstParams);
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_H14
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H14) ) {
		ret = DENC_REG_H14_SetSource(pstParams);
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_M14
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_M14) ) {
		ret = DENC_REG_M14_SetSource(pstParams);
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_H13
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H13) ) {
		ret = DENC_REG_H13_SetSource(pstParams);
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_L9
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_L9) ) {
		ret = DENC_REG_L9_SetSource(pstParams);
	} else
#endif
	{
	}
	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief Teletext insertion enable (TRUE : enable, FALSE : disable)
 *
 * @param pstParams [IN] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DENC_HAL_TtxEnable(BOOLEAN *pstParams)
{
	int ret = RET_OK;

#ifdef USE_KDRV_CODES_FOR_H15
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H15) ) {
		ret = DENC_REG_H15_TtxEnable(pstParams);
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_H14
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H14) ) {
		ret = DENC_REG_H14_TtxEnable(pstParams);
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_M14
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_M14) ) {
		ret = DENC_REG_M14_TtxEnable(pstParams);
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_H13
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H13) ) {
		ret = DENC_REG_H13_TtxEnable(pstParams);
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_L9
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_L9) ) {
		ret = DENC_REG_L9_TtxEnable(pstParams);
	} else
#endif
	{
	}
	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief WSS insertion enable (TRUE : enable, FALSE : disable)
 *
 * @param pstParams [IN] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DENC_HAL_WssEnable(BOOLEAN *pstParams)
{
	int ret = RET_OK;

#ifdef USE_KDRV_CODES_FOR_H15
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H15) ) {
		ret = DENC_REG_H15_WssEnable(pstParams);
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_H14
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H14) ) {
		ret = DENC_REG_H14_WssEnable(pstParams);
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_M14
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_M14) ) {
		ret = DENC_REG_M14_WssEnable(pstParams);
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_H13
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H13) ) {
		ret = DENC_REG_H13_WssEnable(pstParams);
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_L9
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_L9) ) {
		ret = DENC_REG_L9_WssEnable(pstParams);
	} else
#endif
	{
	}
	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief VPS insertion enable (TRUE : enable, FALSE : disable)
 *
 * @param pstParams [IN] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DENC_HAL_VpsEnable(BOOLEAN *pstParams)
{
	int ret = RET_OK;

#ifdef USE_KDRV_CODES_FOR_H15
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H15) ) {
		ret = DENC_REG_H15_VpsEnable(pstParams);
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_H14
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H14) ) {
		ret = DENC_REG_H14_VpsEnable(pstParams);
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_M14
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_M14) ) {
		ret = DENC_REG_M14_VpsEnable(pstParams);
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_H13
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H13) ) {
		ret = DENC_REG_H13_VpsEnable(pstParams);
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_L9
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_L9) ) {
		ret = DENC_REG_L9_VpsEnable(pstParams);
	} else
#endif
	{
	}
	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief Inserts Teletext data to VBI lines.
 *
 * @param pstParams [IN] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DENC_HAL_SetTtxData(LX_DENC_TTX_DATA_T *pstParams)
{
	int ret = RET_OK;

#ifdef USE_KDRV_CODES_FOR_H15
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H15) ) {
		ret = DENC_REG_H15_SetTtxData(pstParams);
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_H14
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H14) ) {
		ret = DENC_REG_H14_SetTtxData(pstParams);
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_M14
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_M14) ) {
		ret = DENC_REG_M14_SetTtxData(pstParams);
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_H13
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H13) ) {
		ret = DENC_REG_H13_SetTtxData(pstParams);
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_L9
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_L9) ) {
		ret = DENC_REG_L9_SetTtxData(pstParams);
	} else
#endif
	{
	}
	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief Inserts WSS data to VBI line.
 *
 * @param pstParams [IN] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DENC_HAL_SetWssData(LX_DENC_WSS_DATA_T *pstParams)
{
	int ret = RET_OK;

#ifdef USE_KDRV_CODES_FOR_H15
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H15) ) {
		ret = DENC_REG_H15_SetWssData(pstParams);
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_H14
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H14) ) {
		ret = DENC_REG_H14_SetWssData(pstParams);
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_M14
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_M14) ) {
		ret = DENC_REG_M14_SetWssData(pstParams);
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_H13
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H13) ) {
		ret = DENC_REG_H13_SetWssData(pstParams);
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_L9
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_L9) ) {
		ret = DENC_REG_L9_SetWssData(pstParams);
	} else
#endif
	{
	}
	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief Inserts VPS data to VBI lines.
 *
 * @param pstParams [IN] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DENC_HAL_SetVpsData(LX_DENC_VPS_DATA_T *pstParams)
{
	int ret = RET_OK;

#ifdef USE_KDRV_CODES_FOR_H15
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H15) ) {
		ret = DENC_REG_H15_SetVpsData(pstParams);
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_H14
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H14) ) {
		ret = DENC_REG_H14_SetVpsData(pstParams);
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_M14
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_M14) ) {
		ret = DENC_REG_M14_SetVpsData(pstParams);
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_H13
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H13) ) {
		ret = DENC_REG_H13_SetVpsData(pstParams);
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_L9
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_L9) ) {
		ret = DENC_REG_L9_SetVpsData(pstParams);
	} else
#endif
	{
	}
	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief Enable/disable internal colorbar pattern (TRUE : enable, FALSE : disable)
 *
 * @param pstParams [IN] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DENC_HAL_ColorBarEnable(BOOLEAN *pstParams)
{
	int ret = RET_OK;

#ifdef USE_KDRV_CODES_FOR_H15
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H15) ) {
		ret = DENC_REG_H15_ColorBarEnable(pstParams);
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_H14
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H14) ) {
		ret = DENC_REG_H14_ColorBarEnable(pstParams);
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_M14
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_M14) ) {
		ret = DENC_REG_M14_ColorBarEnable(pstParams);
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_H13
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H13) ) {
		ret = DENC_REG_H13_ColorBarEnable(pstParams);
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_L9
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_L9) ) {
		ret = DENC_REG_L9_ColorBarEnable(pstParams);
	} else
#endif
	{
	}
	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief Power On/Off VDAC and Buffer Power on DENC Block (0:off, 1:on)
 *
 * @param pstParams [IN] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DENC_HAL_VdacPowerControl(BOOLEAN *pstParams)
{
	int ret = RET_OK;

#ifdef USE_KDRV_CODES_FOR_H15
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H15) ) {
		ret = DENC_REG_H15_VdacPowerControl(pstParams);
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_H14
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H14) ) {
		ret = DENC_REG_H14_VdacPowerControl(pstParams);
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_M14
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_M14) ) {
		ret = DENC_REG_M14_VdacPowerControl(pstParams);
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_H13
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H13) ) {
		ret = DENC_REG_H13_VdacPowerControl(pstParams);
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_L9
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_L9) ) {
		ret = DENC_REG_L9_VdacPowerControl(pstParams);
	} else
#endif
	{
	}
	return ret;
}

int DENC_HAL_Create(void)
{
    int ret = RET_OK;

#ifdef USE_KDRV_CODES_FOR_H15
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H15) ) {
		ret = DENC_REG_H15_Create();
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_H14
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H14) ) {
		ret = DENC_REG_H14_Create();
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_M14
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_M14) ) {
		ret = DENC_REG_M14_Create();
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_H13
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H13) ) {
		ret = DENC_REG_H13_Create();
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_L9
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_L9) ) {
		ret = DENC_REG_L9_Create();
	} else
#endif
	{
	}
    return ret;
}

int DENC_HAL_Destory(void)
{
    int ret = RET_OK;

#ifdef USE_KDRV_CODES_FOR_H15
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H15) ) {
		ret = DENC_REG_H15_Destory();
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_H14
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H14) ) {
		ret = DENC_REG_H14_Destory();
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_M14
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_M14) ) {
		ret = DENC_REG_M14_Destory();
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_H13
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H13) ) {
		ret = DENC_REG_H13_Destory();
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_L9
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_L9) ) {
		ret = DENC_REG_L9_Destory();
	} else
#endif
	{
	}
    return ret;
}
/**  @} */
