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
#define USE_CONFIG_FOR_M14B0

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

#ifdef USE_KDRV_CODES_FOR_M14
#include "../de/de_def.h"
#include "../../chip/m14/de/de_reg_def_m14.h"

#include "../../chip/m14/de/de_atp_reg_m14.h"      	// 0x3c00
#include "../../chip/m14/de/de_cvd_reg_m14.h"       // 0x4100
#include "denc_kapi.h"
#include "denc_reg_m14.h"

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
extern volatile DE_CVD_REG_M14_T gDE_CVD_M14;
extern volatile DE_CVD_REG_M14B_T gDE_CVD_M14B;
#endif

/*----------------------------------------------------------------------------------------
 *   global Functions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   global Variables
 *---------------------------------------------------------------------------------------*/
DE_ATP_REG_M14_T gDE_ATP_M14;
DE_ATP_REG_M14B_T gDE_ATP_M14B;

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
int DENC_REG_M14_Create(void)
{
    int ret = RET_OK;

	if(lx_chip_rev() >= LX_CHIP_REV(M14,B0))
	{
		gDE_ATP_M14B.shdw.addr = (UINT32 *)OS_KMalloc(sizeof(DE_ATP_REG_M14B0_T));
		gDE_ATP_M14B.phys.addr = (volatile UINT32 *)ioremap(DE_ATP_REG_M14B_BASE, sizeof(DE_ATP_REG_M14B0_T));
	}
	else
	{
		gDE_ATP_M14.shdw.addr = (UINT32 *)OS_KMalloc(sizeof(DE_ATP_REG_M14A0_T));
		gDE_ATP_M14.phys.addr = (volatile UINT32 *)ioremap(DE_ATP_REG_M14_BASE, sizeof(DE_ATP_REG_M14A0_T));
	}

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
int DENC_REG_M14_Destory(void)
{
    int ret = RET_OK;

	if(lx_chip_rev() >= LX_CHIP_REV(M14,B0))
	{
		OS_Free((void *)gDE_ATP_M14B.shdw.addr);
		if (gDE_ATP_M14B.phys.addr) iounmap((void *)gDE_ATP_M14B.phys.addr);
	}
	else
	{
		OS_Free((void *)gDE_ATP_M14.shdw.addr);
		if (gDE_ATP_M14.phys.addr) iounmap((void *)gDE_ATP_M14.phys.addr);
	}

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
int DENC_REG_M14_Init(void)
{
	int ret = RET_OK;

	do {
		if(lx_chip_rev() >= LX_CHIP_REV(M14,B0))
		{
#ifdef INCLUDE_KDRV_DE
			DE_CVD_M14B_FLWr(cvbsafe_001, 0x01311000);  // VDAC On
#endif
		}
		else
		{
#ifdef USE_CVD_CHECK_LATER
			REG_WD(&gVportRegBx->cvbsafe_010, 0x00633502);                        // CVBS PLL Setting Value
			REG_WD(&gVportRegBx->top_003, REG_RD(&gVportRegBx->top_003) | 0x10);  // CVBS PLL Power Down 0:Power Down, 1:Power On
			REG_WD(&gVportRegBx->top_007, REG_RD(&gVportRegBx->top_007) & ~0x1);  // CVE Soft Reset 0:Normal Operation, 1:Reset
#endif
#ifdef INCLUDE_KDRV_DE
			DE_CVD_M14_FLWr(cvbsafe_001, 0x01311000);  // VDAC On
#endif
		}
		ret = DENC_REG_M14_NTSC_Init();
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
int DENC_REG_M14_OnOff ( BOOLEAN *pOnOff )
{
	if(lx_chip_rev() >= LX_CHIP_REV(M14,B0))
	{
		DE_ATP_M14B_FLCM(cve_config, enc_en, *pOnOff, 0x1);
	}
	else
	{
		DE_ATP_M14_FLCM(cve_config, enc_en, *pOnOff, 0x1);
	}
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
int DENC_REG_M14_SetOutPutVideoStd(LX_DENC_VIDEO_SYSTEM_T *pstParams)
{
	if(lx_chip_rev() >= LX_CHIP_REV(M14,B0))
	{
#ifndef USE_CONFIG_FOR_M14B0
		DE_ATP_M14B_FLWf(cve_config, standard , *pstParams);
#endif
	}
	else
	{
		DE_ATP_M14_FLWf(cve_config, standard , *pstParams);
	}
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
int DENC_REG_M14_SetSource(LX_DENC_VIDEO_SOURCE_T *pstParams)
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
int DENC_REG_M14_TtxEnable(BOOLEAN *pstParams)
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
int DENC_REG_M14_WssEnable(BOOLEAN *pstParams)
{
	if(lx_chip_rev() >= LX_CHIP_REV(M14,B0))
	{
		DE_ATP_M14B_FLCM(denc_vbi_ctrl, wss_en, *pstParams, 0x1);
		DE_ATP_M14B_RdFL(vps_ctrl);
		DE_ATP_M14B_WfCM(vps_ctrl, wss_odd_en,   *pstParams, 0x1);
		DE_ATP_M14B_WfCM(vps_ctrl, wss_even_en,  *pstParams, 0x1);
		DE_ATP_M14B_WrFL(vps_ctrl);
	}
	else
	{
		DE_ATP_M14_FLCM(denc_vbi_ctrl, wss_en, *pstParams, 0x1);
		DE_ATP_M14_RdFL(vps_ctrl);
		DE_ATP_M14_WfCM(vps_ctrl, wss_odd_en,   *pstParams, 0x1);
		DE_ATP_M14_WfCM(vps_ctrl, wss_even_en,  *pstParams, 0x1);
		DE_ATP_M14_WrFL(vps_ctrl);
	}

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
int DENC_REG_M14_VpsEnable(BOOLEAN *pstParams)
{
	if(lx_chip_rev() >= LX_CHIP_REV(M14,B0))
	{
#ifndef USE_CONFIG_FOR_M14B0
		DE_ATP_M14B_RdFL(vps_ctrl);
		DE_ATP_M14B_WfCM(vps_ctrl, vbi_odd,    *pstParams, 0x1);
		DE_ATP_M14B_WfCM(vps_ctrl, vbi_odd,    *pstParams, 0x1);
		DE_ATP_M14B_Wr01(vps_ctrl, vbi_even,               0x0);
		DE_ATP_M14B_Wr01(vps_ctrl, lpf_bypass,             0x0);
		DE_ATP_M14B_WrFL(vps_ctrl);
#endif
	}
	else
	{
		DE_ATP_M14_RdFL(vps_ctrl);
		DE_ATP_M14_WfCM(vps_ctrl, vbi_odd,    *pstParams, 0x1);
		DE_ATP_M14_WfCM(vps_ctrl, vbi_odd,    *pstParams, 0x1);
		DE_ATP_M14_Wr01(vps_ctrl, vbi_even,               0x0);
		DE_ATP_M14_Wr01(vps_ctrl, lpf_bypass,             0x0);
		DE_ATP_M14_WrFL(vps_ctrl);
	}

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
int DENC_REG_M14_SetTtxData(LX_DENC_TTX_DATA_T *pstParams)
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

		if(lx_chip_rev() >= LX_CHIP_REV(M14,B0))
		{
			DE_ATP_M14B_FLWf(ttx_ver_ctrl1, ttx_line, pstParams->Line_Number );

			DE_ATP_M14B_RdFL(ttx_load);
			DE_ATP_M14B_Wr01(ttx_load, ttx_load, 1);
			for ( i=0; i<ttxSize;i++ ) {
				DE_ATP_M14B_FLWr(ttx_data, pTtxData[i]);
				DE_ATP_M14B_Wr01(ttx_load, ttx_write_addr, i);
				DE_ATP_M14B_WrFL(ttx_load);
			}
			DE_ATP_M14B_Wr01(ttx_load, ttx_load, 0);
			DE_ATP_M14B_WrFL(ttx_load);
		}
		else
		{
			DE_ATP_M14_FLWf(ttx_ver_ctrl1, ttx_line, pstParams->Line_Number );

			DE_ATP_M14_RdFL(ttx_load);
			DE_ATP_M14_Wr01(ttx_load, ttx_load, 1);
			for ( i=0; i<ttxSize;i++ ) {
				DE_ATP_M14_FLWr(ttx_data, pTtxData[i]);
				DE_ATP_M14_Wr01(ttx_load, ttx_write_addr, i);
				DE_ATP_M14_WrFL(ttx_load);
			}
			DE_ATP_M14_Wr01(ttx_load, ttx_load, 0);
			DE_ATP_M14_WrFL(ttx_load);
		}
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
int DENC_REG_M14_SetWssData(LX_DENC_WSS_DATA_T *pstParams)
{
	if(lx_chip_rev() >= LX_CHIP_REV(M14,B0))
	{
		DE_ATP_M14B_FLCM(denc_vbi_ctrl, wss_en, pstParams->Enable, 0x1);
		DE_ATP_M14B_RdFL(vps_ctrl);
		DE_ATP_M14B_WfCM(vps_ctrl, wss_odd_en,     pstParams->Enable, 0x1);
		DE_ATP_M14B_WfCM(vps_ctrl, wss_even_en,    pstParams->Enable, 0x1);
		DE_ATP_M14B_WrFL(vps_ctrl);
		DE_ATP_M14B_FLWf(wss_data, wss_data, pstParams->WSS_DATA);
	}
	else
	{
		DE_ATP_M14_FLCM(denc_vbi_ctrl, wss_en, pstParams->Enable, 0x1);
		DE_ATP_M14_RdFL(vps_ctrl);
		DE_ATP_M14_WfCM(vps_ctrl, wss_odd_en,     pstParams->Enable, 0x1);
		DE_ATP_M14_WfCM(vps_ctrl, wss_even_en,    pstParams->Enable, 0x1);
		DE_ATP_M14_WrFL(vps_ctrl);
		DE_ATP_M14_FLWf(wss_data, wss_data, pstParams->WSS_DATA);
	}

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
int DENC_REG_M14_SetVpsData(LX_DENC_VPS_DATA_T *pstParams)
{
	int ret = RET_OK;
	UINT16 vpsData;
	int i;

	if(lx_chip_rev() >= LX_CHIP_REV(M14,B0))
	{
		DE_ATP_M14B_RdFL(vps_ctrl);
		DE_ATP_M14B_WfCM(vps_ctrl, vps_enable, pstParams->Enable, 0x1);
		DE_ATP_M14B_WfCM(vps_ctrl, vbi_odd,    pstParams->Enable, 0x1);
		DE_ATP_M14B_Wr01(vps_ctrl, vbi_even,                      0x0);
		DE_ATP_M14B_Wr01(vps_ctrl, lpf_bypass,                    0x0);
		DE_ATP_M14B_Wr01(vps_ctrl, vps_wr_done,                   0x0);
		vpsData = 0;
		for (i=0;i<15;i++) {
			vpsData <<= 8;
			vpsData  |= pstParams->pVPS_DATA[i];
			if ( !(i%2) ) continue;
			DE_ATP_M14B_Wr01(vps_ctrl, vps_data,    vpsData);
			DE_ATP_M14B_WrFL(vps_ctrl);
			vpsData = 0;
		}
		DE_ATP_M14B_Wr01(vps_ctrl, vps_wr_done, 0x1);
		DE_ATP_M14B_Wr01(vps_ctrl, vps_data,    vpsData);
		DE_ATP_M14B_WrFL(vps_ctrl);
	}
	else
	{
		DE_ATP_M14_RdFL(vps_ctrl);
		DE_ATP_M14_WfCM(vps_ctrl, vps_enable, pstParams->Enable, 0x1);
		DE_ATP_M14_WfCM(vps_ctrl, vbi_odd,    pstParams->Enable, 0x1);
		DE_ATP_M14_Wr01(vps_ctrl, vbi_even,                      0x0);
		DE_ATP_M14_Wr01(vps_ctrl, lpf_bypass,                    0x0);
		DE_ATP_M14_Wr01(vps_ctrl, vps_wr_done,                   0x0);
		vpsData = 0;
		for (i=0;i<15;i++) {
			vpsData <<= 8;
			vpsData  |= pstParams->pVPS_DATA[i];
			if ( !(i%2) ) continue;
			DE_ATP_M14_Wr01(vps_ctrl, vps_data,    vpsData);
			DE_ATP_M14_WrFL(vps_ctrl);
			vpsData = 0;
		}
		DE_ATP_M14_Wr01(vps_ctrl, vps_wr_done, 0x1);
		DE_ATP_M14_Wr01(vps_ctrl, vps_data,    vpsData);
		DE_ATP_M14_WrFL(vps_ctrl);
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
int DENC_REG_M14_ColorBarEnable(BOOLEAN *pstParams)
{
	if(lx_chip_rev() >= LX_CHIP_REV(M14,B0))
	{
		DE_ATP_M14B_FLCM(cve_config, color_bar_en, *pstParams, 0x1);
	}
	else
	{
		DE_ATP_M14_FLCM(cve_config, color_bar_en, *pstParams, 0x1);
	}
    return RET_OK;
}

int DENC_REG_M14_NTSC_Init(void)
{
	if(lx_chip_rev() >= LX_CHIP_REV(M14,B0))
	{
#ifndef USE_CONFIG_FOR_M14B0
		DE_ATP_M14B_FLWr(ttx_ver_ctrl0, 0x17061606);
		DE_ATP_M14B_FLWr(ttx_ver_ctrl1, 0x0003c4c0);
		DE_ATP_M14B_FLWr(wss_data,      0x6a5a9919);
		DE_ATP_M14B_FLWr(cgms_data,     0x0B800000);
		DE_ATP_M14B_FLWr(cve_config,    0x0B381b89);
		DE_ATP_M14B_FLWr(phase_incr,    0x2a098acb);
		DE_ATP_M14B_FLWr(denc_vbi_ctrl, 0x4ee00800);
		DE_ATP_M14B_FLWr(cc_data,       0x80808080);

		DE_ATP_M14B_FLWr(mv_n0_3,		  0x21171500);
		DE_ATP_M14B_FLWr(mv_n4_9,		  0x1BB61B15);
		DE_ATP_M14B_FLWr(mv_n10_11,	  0x07F80024);
		DE_ATP_M14B_FLWr(mv_n12_14,	  0x0F0F0000);
		DE_ATP_M14B_FLWr(mv_n15_20,	  0xA05C0060);

		DE_ATP_M14B_FLWr(vps_ctrl, 0x123400a0);
		DE_ATP_M14B_FLWr(vps_ctrl, 0x567800a1);
		DE_ATP_M14B_FLWr(vps_ctrl, 0x9abc00a2);
		DE_ATP_M14B_FLWr(vps_ctrl, 0xdef100a3);
		DE_ATP_M14B_FLWr(vps_ctrl, 0x234500a4);
		DE_ATP_M14B_FLWr(vps_ctrl, 0x678900a5);
		DE_ATP_M14B_FLWr(vps_ctrl, 0xabcd01a6);
#endif
	}
	else
	{
		DE_ATP_M14_FLWr(ttx_ver_ctrl0, 0x17061606);
		DE_ATP_M14_FLWr(ttx_ver_ctrl1, 0x0003c4c0);
		DE_ATP_M14_FLWr(wss_data,      0x6a5a9919);
		DE_ATP_M14_FLWr(cgms_data,     0x0B800000);
		DE_ATP_M14_FLWr(cve_config,    0x0B381b89);
		DE_ATP_M14_FLWr(phase_incr,    0x2a098acb);
		DE_ATP_M14_FLWr(denc_vbi_ctrl, 0x4ee00800);
		DE_ATP_M14_FLWr(cc_data,       0x80808080);

		DE_ATP_M14_FLWr(mv_n0_3,		  0x21171500);
		DE_ATP_M14_FLWr(mv_n4_9,		  0x1BB61B15);
		DE_ATP_M14_FLWr(mv_n10_11,	  0x07F80024);
		DE_ATP_M14_FLWr(mv_n12_14,	  0x0F0F0000);
		DE_ATP_M14_FLWr(mv_n15_20,	  0xA05C0060);

		DE_ATP_M14_FLWr(vps_ctrl, 0x123400a0);
		DE_ATP_M14_FLWr(vps_ctrl, 0x567800a1);
		DE_ATP_M14_FLWr(vps_ctrl, 0x9abc00a2);
		DE_ATP_M14_FLWr(vps_ctrl, 0xdef100a3);
		DE_ATP_M14_FLWr(vps_ctrl, 0x234500a4);
		DE_ATP_M14_FLWr(vps_ctrl, 0x678900a5);
		DE_ATP_M14_FLWr(vps_ctrl, 0xabcd01a6);
	}
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
int DENC_REG_M14_VdacPowerControl(BOOLEAN *pstParams)
{
	int ret = RET_OK;

	if(*pstParams)
	{
		ret = DENC_REG_M14_NTSC_Init();
	}

	return ret;
}
#endif
/**  @} */
