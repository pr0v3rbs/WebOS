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
 * @addtogroup lg1152_de
 * @{
 */

/*----------------------------------------------------------------------------------------
 *   Control Constants
 *---------------------------------------------------------------------------------------*/
#define CONTROL_FOR_MCU

/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/semaphore.h>
#include <asm/io.h>

#include "base_types.h"
#include "os_util.h"

#include "../de/de_ver_def.h"

#ifdef USE_KDRV_CODES_FOR_H15
#include "../de/de_def.h"
#include "../../chip/h15/de/de_reg_def_h15.h"

#include "../../chip/h15/de/de_atp_reg_h15.h"      	// 0x3c00
#include "../../chip/h15/de/de_cvd_reg_h15.h"       // 0x4100
#include "denc_kapi.h"
#include "denc_reg_h15.h"

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
#ifdef INCLUDE_KDRV_DE
extern volatile DE_CVD_REG_H15_T gDE_CVD_H15;
#endif

/*----------------------------------------------------------------------------------------
 *   global Functions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   global Variables
 *---------------------------------------------------------------------------------------*/
DE_ATP_REG_H15_T gDE_ATP_H15;

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
 * @return RET_OK(0)
 */
int DENC_REG_H15_Create(void)
{
    int ret = RET_OK;

	gDE_ATP_H15.shdw.addr = (UINT32 *)OS_KMalloc(sizeof(DE_ATP_REG_H15A0_T));
	gDE_ATP_H15.phys.addr = (volatile UINT32 *)ioremap(DE_ATP_REG_H15_BASE, sizeof(DE_ATP_REG_H15A0_T));

    return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief Destroy DENC module.
 *
 * @return RET_OK(0)
 */
int DENC_REG_H15_Destory(void)
{
    int ret = RET_OK;

	OS_Free((void *)gDE_ATP_H15.shdw.addr);
	if (gDE_ATP_H15.phys.addr) iounmap((void *)gDE_ATP_H15.phys.addr);

    return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief Initialize DENC module.
 *
 * @return RET_OK(0)
 */
int DENC_REG_H15_Init(void)
{
	int ret = RET_OK;

	do {
#ifdef INCLUDE_KDRV_DE
		DE_CVD_H15_FLWr(cvbsafe_001, 0x01311000);  // VDAC On
#endif
		ret = DENC_REG_H15_NTSC_Init();
	} while (0);

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief NTSC/PAL/SECAM encoder enable(TRUE : enable, FALSE : disable)
 *
 * @param OnOff
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DENC_REG_H15_OnOff ( BOOLEAN *pOnOff )
{
	DE_ATP_H15_FLCM(cve_config, enc_en, *pOnOff, 0x1);
	return RET_OK;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief Select the color standard of CVE.
 *
 * @param OnOff
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DENC_REG_H15_SetOutPutVideoStd(LX_DENC_VIDEO_SYSTEM_T *pstParams)
{
#ifndef CONTROL_FOR_MCU
		DE_ATP_H15_FLWf(cve_config, standard , *pstParams);
#endif
	return RET_OK;
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
int DENC_REG_H15_SetSource(LX_DENC_VIDEO_SOURCE_T *pstParams)
{
    int ret = RET_OK;

    return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief Teletext insertion enable (TRUE : enable, FALSE : disable)
 *
 * @param OnOff
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DENC_REG_H15_TtxEnable(BOOLEAN *pstParams)
{
	int ret = RET_OK;
	
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
int DENC_REG_H15_WssEnable(BOOLEAN *pstParams)
{
	DE_ATP_H15_FLCM(denc_vbi_ctrl, wss_en, *pstParams, 0x1);
	DE_ATP_H15_RdFL(vps_ctrl);
	DE_ATP_H15_WfCM(vps_ctrl, wss_odd_en,   *pstParams, 0x1);
	DE_ATP_H15_WfCM(vps_ctrl, wss_even_en,  *pstParams, 0x1);
	DE_ATP_H15_WrFL(vps_ctrl);

	return RET_OK;
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
int DENC_REG_H15_VpsEnable(BOOLEAN *pstParams)
{
#ifndef CONTROL_FOR_MCU
	DE_ATP_H15_RdFL(vps_ctrl);
	DE_ATP_H15_WfCM(vps_ctrl, vbi_odd,    *pstParams, 0x1);
	DE_ATP_H15_WfCM(vps_ctrl, vbi_odd,    *pstParams, 0x1);
	DE_ATP_H15_Wr01(vps_ctrl, vbi_even,               0x0);
	DE_ATP_H15_Wr01(vps_ctrl, lpf_bypass,             0x0);
	DE_ATP_H15_WrFL(vps_ctrl);
#endif

	return RET_OK;
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
int DENC_REG_H15_SetTtxData(LX_DENC_TTX_DATA_T *pstParams)
{
    int ret = RET_OK;
	UINT32 *pTtxData = NULL;
	UINT32 ttxSize;
	int i;

	do {
		CHECK_KNULL(pstParams->pPacket_Addr);
		ttxSize =  pstParams->Size/sizeof(UINT32);
		if (!ttxSize) break;
		pTtxData = (UINT32 *)pstParams->pPacket_Addr;
		CHECK_KNULL(pTtxData);

		DE_ATP_H15_FLWf(ttx_ver_ctrl1, ttx_line, pstParams->Line_Number );

		DE_ATP_H15_RdFL(ttx_load);
		DE_ATP_H15_Wr01(ttx_load, ttx_load, 1);
		for ( i=0; i<ttxSize;i++ ) {
			DE_ATP_H15_FLWr(ttx_data, pTtxData[i]);
			DE_ATP_H15_Wr01(ttx_load, ttx_write_addr, i);
			DE_ATP_H15_WrFL(ttx_load);
		}
		DE_ATP_H15_Wr01(ttx_load, ttx_load, 0);
		DE_ATP_H15_WrFL(ttx_load);
	} while (0);

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
int DENC_REG_H15_SetWssData(LX_DENC_WSS_DATA_T *pstParams)
{
	DE_ATP_H15_FLCM(denc_vbi_ctrl, wss_en, pstParams->Enable, 0x1);
	DE_ATP_H15_RdFL(vps_ctrl);
	DE_ATP_H15_WfCM(vps_ctrl, wss_odd_en,     pstParams->Enable, 0x1);
	DE_ATP_H15_WfCM(vps_ctrl, wss_even_en,    pstParams->Enable, 0x1);
	DE_ATP_H15_WrFL(vps_ctrl);
	DE_ATP_H15_FLWf(wss_data, wss_data, pstParams->WSS_DATA);

	return RET_OK;
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
int DENC_REG_H15_SetVpsData(LX_DENC_VPS_DATA_T *pstParams)
{
	int ret = RET_OK;
	UINT16 vpsData;
	int i;

	DE_ATP_H15_RdFL(vps_ctrl);
	DE_ATP_H15_WfCM(vps_ctrl, vps_enable, pstParams->Enable, 0x1);
	DE_ATP_H15_WfCM(vps_ctrl, vbi_odd,    pstParams->Enable, 0x1);
	DE_ATP_H15_Wr01(vps_ctrl, vbi_even,                      0x0);
	DE_ATP_H15_Wr01(vps_ctrl, lpf_bypass,                    0x0);
	DE_ATP_H15_Wr01(vps_ctrl, vps_wr_done,                   0x0);
	vpsData = 0;
	for (i=0;i<15;i++) {
		vpsData <<= 8;
		vpsData  |= pstParams->pVPS_DATA[i];
		if ( !(i%2) ) continue;
		DE_ATP_H15_Wr01(vps_ctrl, vps_data,    vpsData);
		DE_ATP_H15_WrFL(vps_ctrl);
		vpsData = 0;
	}
	DE_ATP_H15_Wr01(vps_ctrl, vps_wr_done, 0x1);
	DE_ATP_H15_Wr01(vps_ctrl, vps_data,    vpsData);
	DE_ATP_H15_WrFL(vps_ctrl);

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
int DENC_REG_H15_ColorBarEnable(BOOLEAN *pstParams)
{
	DE_ATP_H15_FLCM(cve_config, color_bar_en, *pstParams, 0x1);
    return RET_OK;
}

int DENC_REG_H15_NTSC_Init(void)
{
#ifndef CONTROL_FOR_MCU
	DE_ATP_H15_FLWr(ttx_ver_ctrl0, 0x17061606);
	DE_ATP_H15_FLWr(ttx_ver_ctrl1, 0x0003c4c0);
	DE_ATP_H15_FLWr(wss_data,      0x6a5a9919);
	DE_ATP_H15_FLWr(cgms_data,     0x0B800000);
	DE_ATP_H15_FLWr(cve_config,    0x0B381b89);
	DE_ATP_H15_FLWr(phase_incr,    0x2a098acb);
	DE_ATP_H15_FLWr(denc_vbi_ctrl, 0x4ee00800);
	DE_ATP_H15_FLWr(cc_data,       0x80808080);

	DE_ATP_H15_FLWr(mv_n0_3,		  0x21171500);
	DE_ATP_H15_FLWr(mv_n4_9,		  0x1BB61B15);
	DE_ATP_H15_FLWr(mv_n10_11,	  0x07F80024);
	DE_ATP_H15_FLWr(mv_n12_14,	  0x0F0F0000);
	DE_ATP_H15_FLWr(mv_n15_20,	  0xA05C0060);

	DE_ATP_H15_FLWr(vps_ctrl, 0x123400a0);
	DE_ATP_H15_FLWr(vps_ctrl, 0x567800a1);
	DE_ATP_H15_FLWr(vps_ctrl, 0x9abc00a2);
	DE_ATP_H15_FLWr(vps_ctrl, 0xdef100a3);
	DE_ATP_H15_FLWr(vps_ctrl, 0x234500a4);
	DE_ATP_H15_FLWr(vps_ctrl, 0x678900a5);
	DE_ATP_H15_FLWr(vps_ctrl, 0xabcd01a6);
#endif
	return RET_OK;
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
int DENC_REG_H15_VdacPowerControl(BOOLEAN *pstParams)
{
	int ret = RET_OK;

	if(*pstParams)
	{
		ret = DENC_REG_H15_NTSC_Init();
	}

	return ret;
}
#endif
/**  @} */
