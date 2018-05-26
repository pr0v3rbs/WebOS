/*
 * SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 * Copyright(c) 2013 by LG Electronics Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

/** @file pe_ccm_hw_m14.c
 *
 *  driver for picture enhance color correction module functions. ( used only within kdriver )
 *
 *	@author		Seung-Jun,Youm(sj.youm@lge.com)
 *	@version	0.1
 *	@note
 *	@date		2012.04.12
 *	@see
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/semaphore.h>
#include <linux/interrupt.h>
#include <asm/io.h>

#include "base_types.h"
#include "os_util.h"

#include "pe_hw_m14.h"
#include "pe_reg_m14.h"
#include "pe_ccm_hw_m14.h"
#include "pe_hst_hw_m14.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
#define PE_CCM_HW_M14_SAT_STATUS_TH		200
#define PE_CCM_HW_M14_LRCR_GAIN			0xa
#define PE_CCM_HW_M14_LRCR_MAX_HIS		224
#define PE_CCM_HW_M14_LRCR_VAL_BIN		72
#define PE_CCM_HW_M14_LRCR_TH1			128
#define PE_CCM_HW_M14_LRCR_TH0			129

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define PE_CCM_HW_M14_ERROR	printk

#define PE_CCM_HW_M14_DBG_PRINT(fmt,args...)	\
	if(_g_ccm_hw_m14_trace) printk("[%x,%x][%s,%d] "fmt,PE_CHIP_VER,g_pe_kdrv_ver_mask,__F__,__L__,##args)
#define PE_CCM_HW_M14_CHECK_CODE(_checker,_action,fmt,args...)	\
	{if(_checker){PE_CCM_HW_M14_ERROR(fmt,##args);_action;}}

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
	global Functions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/
static UINT32 PE_CCM_HW_M14_GetAcGain3dInfo(PE_TSK_M14_CFG_T *pCfg);

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
static UINT32 _g_ccm_hw_m14_trace=0x0;		//default should be off.
static PE_CCM_HW_M14_SETTINGS_T _g_pe_ccm_hw_m14_info;

/*========================================================================================
	Implementation Group
========================================================================================*/
/**
 * set debug print on off
 *
 * @param   on_off [in] UINT8
 * @return  void
 * @see
 * @author
 */
void PE_CCM_HW_M14_SetDbgPrint(UINT32 on_off)
{
	_g_ccm_hw_m14_trace = on_off? 1:0;
	return;
}
/**
 * init ccm
 *
 * @param   *pstParams [in] PE_CFG_CTRL_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_CCM_HW_M14_Init(PE_CFG_CTRL_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_M14BX)
		{
			PE_CCM_HW_M14_DBG_PRINT("init param.\n");
			memset(&_g_pe_ccm_hw_m14_info,0xff,sizeof(PE_CCM_HW_M14_SETTINGS_T));
			PE_PE1_M14B0_RdFL(pe1_fsw_ctrl_00);
			PE_PE1_M14B0_Wr01(pe1_fsw_ctrl_00,reg_color_filter_status,0x0);//0x0~0x2:off,r,g,b
			PE_PE1_M14B0_WrFL(pe1_fsw_ctrl_00);
			PE_CCM_HW_M14_DBG_PRINT("init lrcr config.\n");
			PE_PE1_M14B0_RdFL(pe1_lrcr_ctrl_00);
			PE_PE1_M14B0_RdFL(pe1_lrcr_ctrl_01);
			PE_PE1_M14B0_RdFL(pe1_lrcr_ctrl_02);
			PE_PE1_M14B0_RdFL(pe1_lrcr_ctrl_04);
			PE_PE1_M14B0_RdFL(pe1_lrcr_ctrl_06);
			PE_PE1_M14B0_Wr01(pe1_lrcr_ctrl_00, enable,				0x0);
			PE_PE1_M14B0_Wr01(pe1_lrcr_ctrl_00, adjust_en,			0x1);
			PE_PE1_M14B0_Wr01(pe1_lrcr_ctrl_00, protection_en,		0x1);
			PE_PE1_M14B0_Wr01(pe1_lrcr_ctrl_00, adjust_en2,			0x1);
			PE_PE1_M14B0_Wr01(pe1_lrcr_ctrl_00, ff_mode_en,			0x0);
			PE_PE1_M14B0_Wr01(pe1_lrcr_ctrl_00, ff_mode_parity,		0x0);
			PE_PE1_M14B0_Wr01(pe1_lrcr_ctrl_00, ll_mode_enable,		0x0);
			PE_PE1_M14B0_Wr01(pe1_lrcr_ctrl_00, line_parity_select,	0x0);
			PE_PE1_M14B0_Wr01(pe1_lrcr_ctrl_00, hist_rd_en,			0x0);
			PE_PE1_M14B0_Wr01(pe1_lrcr_ctrl_00, hist_mode,			0x0);
			PE_PE1_M14B0_Wr01(pe1_lrcr_ctrl_00, reg_lrcr_gain,		PE_CCM_HW_M14_LRCR_GAIN);
			PE_PE1_M14B0_Wr01(pe1_lrcr_ctrl_01, th_max_hist,		PE_CCM_HW_M14_LRCR_MAX_HIS);
			PE_PE1_M14B0_Wr01(pe1_lrcr_ctrl_01, th_valid_bins,		PE_CCM_HW_M14_LRCR_VAL_BIN);
			PE_PE1_M14B0_Wr01(pe1_lrcr_ctrl_01, th1,				PE_CCM_HW_M14_LRCR_TH1);
			PE_PE1_M14B0_Wr01(pe1_lrcr_ctrl_01, th0,				PE_CCM_HW_M14_LRCR_TH0);
			PE_PE1_M14B0_Wr01(pe1_lrcr_ctrl_02, hist_l_window_h_bbd,0x0);
			PE_PE1_M14B0_Wr01(pe1_lrcr_ctrl_02, hist_l_window_h_en,	0x1);
			PE_PE1_M14B0_Wr01(pe1_lrcr_ctrl_02, hist_l_window_v_bbd,0x0);
			PE_PE1_M14B0_Wr01(pe1_lrcr_ctrl_02, hist_l_window_v_en,	0x1);
			PE_PE1_M14B0_Wr01(pe1_lrcr_ctrl_04, hist_r_window_h_bbd,0x0);
			PE_PE1_M14B0_Wr01(pe1_lrcr_ctrl_04, hist_r_window_h_en,	0x1);
			PE_PE1_M14B0_Wr01(pe1_lrcr_ctrl_04, hist_r_window_v_bbd,0x0);
			PE_PE1_M14B0_Wr01(pe1_lrcr_ctrl_04, hist_r_window_v_en,	0x1);
			PE_PE1_M14B0_Wr01(pe1_lrcr_ctrl_06, active_window_h_bbd,0x0);
			PE_PE1_M14B0_Wr01(pe1_lrcr_ctrl_06, active_window_h_en,	0x1);
			PE_PE1_M14B0_Wr01(pe1_lrcr_ctrl_06, active_window_v_bbd,0x0);
			PE_PE1_M14B0_Wr01(pe1_lrcr_ctrl_06, active_window_v_en,	0x1);
			PE_PE1_M14B0_WrFL(pe1_lrcr_ctrl_00);
			PE_PE1_M14B0_WrFL(pe1_lrcr_ctrl_01);
			PE_PE1_M14B0_WrFL(pe1_lrcr_ctrl_02);
			PE_PE1_M14B0_WrFL(pe1_lrcr_ctrl_04);
			PE_PE1_M14B0_WrFL(pe1_lrcr_ctrl_06);
		}
		else if(PE_KDRV_VER_M14AX)
		{
			PE_CCM_HW_M14_DBG_PRINT("init param.\n");
			memset(&_g_pe_ccm_hw_m14_info,0xff,sizeof(PE_CCM_HW_M14_SETTINGS_T));
			PE_CCM_HW_M14_DBG_PRINT("init wb.\n");
			PE_P1L_M14A0_QWr04(pe1_wb_ctrl_00,	wb_en, 				0x1,\
												degamma_en, 		0x1,\
												gamma_en, 			0x1,\
												reg_color_filter_status,0x0);//0x0~0x2:off,r,g,b
			PE_P1L_M14A0_QWr03(pe1_wb_ctrl_01,	user_ctrl_g_gain,	0xc0,\
												user_ctrl_b_gain,	0xc0,\
												user_ctrl_r_gain,	0xc0);
			PE_P1L_M14A0_QWr03(pe1_wb_ctrl_02,	user_ctrl_g_offset,	0x80,\
												user_ctrl_b_offset,	0x80,\
												user_ctrl_r_offset,	0x80);
			PE_CCM_HW_M14_DBG_PRINT("init lrcr config.\n");
			PE_P1L_M14A0_RdFL(pe1_lrcr_ctrl_00);
			PE_P1L_M14A0_RdFL(pe1_lrcr_ctrl_01);
			PE_P1L_M14A0_RdFL(pe1_lrcr_ctrl_02);
			PE_P1L_M14A0_RdFL(pe1_lrcr_ctrl_04);
			PE_P1L_M14A0_RdFL(pe1_lrcr_ctrl_06);
			PE_P1L_M14A0_Wr01(pe1_lrcr_ctrl_00, enable,				0x0);
			PE_P1L_M14A0_Wr01(pe1_lrcr_ctrl_00, adjust_en,			0x1);
			PE_P1L_M14A0_Wr01(pe1_lrcr_ctrl_00, protection_en,		0x1);
			PE_P1L_M14A0_Wr01(pe1_lrcr_ctrl_00, adjust_en2,			0x1);
			PE_P1L_M14A0_Wr01(pe1_lrcr_ctrl_00, ff_mode_en,			0x0);
			PE_P1L_M14A0_Wr01(pe1_lrcr_ctrl_00, ff_mode_parity,		0x0);
			PE_P1L_M14A0_Wr01(pe1_lrcr_ctrl_00, ll_mode_enable,		0x0);
			PE_P1L_M14A0_Wr01(pe1_lrcr_ctrl_00, line_parity_select,	0x0);
			PE_P1L_M14A0_Wr01(pe1_lrcr_ctrl_00, hist_rd_en,			0x0);
			PE_P1L_M14A0_Wr01(pe1_lrcr_ctrl_00, hist_mode,			0x0);
			PE_P1L_M14A0_Wr01(pe1_lrcr_ctrl_00, reg_lrcr_gain,		PE_CCM_HW_M14_LRCR_GAIN);
			PE_P1L_M14A0_Wr01(pe1_lrcr_ctrl_01, th_max_hist,		PE_CCM_HW_M14_LRCR_MAX_HIS);
			PE_P1L_M14A0_Wr01(pe1_lrcr_ctrl_01, th_valid_bins,		PE_CCM_HW_M14_LRCR_VAL_BIN);
			PE_P1L_M14A0_Wr01(pe1_lrcr_ctrl_01, th1,				PE_CCM_HW_M14_LRCR_TH1);
			PE_P1L_M14A0_Wr01(pe1_lrcr_ctrl_01, th0,				PE_CCM_HW_M14_LRCR_TH0);
			PE_P1L_M14A0_Wr01(pe1_lrcr_ctrl_02, hist_l_window_h_bbd,0x0);
			PE_P1L_M14A0_Wr01(pe1_lrcr_ctrl_02, hist_l_window_h_en,	0x1);
			PE_P1L_M14A0_Wr01(pe1_lrcr_ctrl_02, hist_l_window_v_bbd,0x0);
			PE_P1L_M14A0_Wr01(pe1_lrcr_ctrl_02, hist_l_window_v_en,	0x1);
			PE_P1L_M14A0_Wr01(pe1_lrcr_ctrl_04, hist_r_window_h_bbd,0x0);
			PE_P1L_M14A0_Wr01(pe1_lrcr_ctrl_04, hist_r_window_h_en,	0x1);
			PE_P1L_M14A0_Wr01(pe1_lrcr_ctrl_04, hist_r_window_v_bbd,0x0);
			PE_P1L_M14A0_Wr01(pe1_lrcr_ctrl_04, hist_r_window_v_en,	0x1);
			PE_P1L_M14A0_Wr01(pe1_lrcr_ctrl_06, active_window_h_bbd,0x0);
			PE_P1L_M14A0_Wr01(pe1_lrcr_ctrl_06, active_window_h_en,	0x1);
			PE_P1L_M14A0_Wr01(pe1_lrcr_ctrl_06, active_window_v_bbd,0x0);
			PE_P1L_M14A0_Wr01(pe1_lrcr_ctrl_06, active_window_v_en,	0x1);
			PE_P1L_M14A0_WrFL(pe1_lrcr_ctrl_00);
			PE_P1L_M14A0_WrFL(pe1_lrcr_ctrl_01);
			PE_P1L_M14A0_WrFL(pe1_lrcr_ctrl_02);
			PE_P1L_M14A0_WrFL(pe1_lrcr_ctrl_04);
			PE_P1L_M14A0_WrFL(pe1_lrcr_ctrl_06);
			_g_pe_ccm_hw_m14_info.ccm_inf.lut_type = PE_CCM_HW_M14_LUT_A;
		}
		else
		{
			PE_CCM_HW_M14_DBG_PRINT("nothing to do\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * ccm Debug setting
 *
 * @param   *pstParams [in/out] LX_PE_DBG_SETTINGS_T int
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_CCM_HW_M14_SetDebugSettings(LX_PE_DBG_SETTINGS_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		PE_CCM_HW_M14_DBG_PRINT("wid:%d,type:0x%x,[0x%x]print:0x%x,bypass:0x%x\n",\
			pstParams->win_id,pstParams->type,(0x1<<PE_ITEM_PKTMODL(CCM)),\
			pstParams->print_lvl,pstParams->bypass);
		/* set print level */
		if(pstParams->type&LX_PE_DBG_LV)
		{
			_g_ccm_hw_m14_trace = (pstParams->print_lvl & (0x1<<PE_ITEM_PKTMODL(CCM)))? 0x1:0x0;
		}
		/* set bypass */
		if(pstParams->type&LX_PE_DBG_BY)
		{
			if(PE_KDRV_VER_M14BX)
			{
				PE_CCM_HW_M14_DBG_PRINT("not supported.\n");
			}
			else if(PE_KDRV_VER_M14AX)
			{
				if(pstParams->bypass & (0x1<<PE_ITEM_PKTMODL(CCM)))
				{
					PE_CCM_HW_M14_DBG_PRINT("wb,degamma,gamma,pxl_rep_en : disable.\n");
					if(PE_CHECK_WIN0(pstParams->win_id))
					{
						PE_P1L_M14A0_QWr03(pe1_wb_ctrl_00, 		wb_en, 			0x0,\
																degamma_en, 	0x0,\
																gamma_en, 		0x0);
						PE_P1L_M14A0_QWr01(pe1_gmc_ctrl_02, 	gmc_mode,		0x0);//read mode
					}
				}
				else
				{
					PE_CCM_HW_M14_DBG_PRINT("wb,degamma,gamma,pxl_rep_en,pxl_rep_dis : enable.\n");
					if(PE_CHECK_WIN0(pstParams->win_id))
					{

						PE_P1L_M14A0_QWr03(pe1_wb_ctrl_00, 		wb_en, 			0x1,\
																degamma_en, 	0x1,\
														 		gamma_en, 		0x1);
						PE_P1L_M14A0_QWr04(pe1_gmc_ctrl_00, 	pxl_rep_enable,	0x1,\
																pxl_rep_disable_r, 0x1,\
																pxl_rep_disable_g, 0x1,\
																pxl_rep_disable_b, 0x1);
						PE_P1L_M14A0_QWr01(pe1_gmc_ctrl_02, 	gmc_mode,		0x2);//normal mode
					}

				}
			}
			else
			{
				PE_CCM_HW_M14_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
			}
		}
	}while(0);
	return ret;
}
/**
 * set gamma correction lut
 *
 * @param   *pstParams [in] LX_PE_CCM_GAMMALUT_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_CCM_HW_M14_SetGammaLut(LX_PE_CCM_GAMMALUT_T *pstParams)
{
	int ret = RET_OK;
	UINT32 count = 0;
	UINT32 lutdata;
	PE_CCM_HW_M14_CTRL_T *ccm_info=&_g_pe_ccm_hw_m14_info.ccm_inf;
	PE_CCM_HW_M14_DBG_PRINT("set[%d] : lut_size:%d\n",pstParams->win_id, pstParams->size);
	do{
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_M14BX)
		{
			PE_CCM_HW_M14_DBG_PRINT("not supported.\n");
		}
		else if(PE_KDRV_VER_M14AX)
		{
			if(PE_CHECK_WIN0(pstParams->win_id))
			{
				PE_P1L_M14A0_RdFL(pe1_gmc_ia_data);
				PE_P1L_M14A0_QWr01(pe1_gmc_ctrl_02,	gmc_mode,		0x1);//write mode
				PE_P1L_M14A0_QWr02(pe1_gmc_ia_ctrl,	capture_ai,		0x1, //ai mode enble
													capture_addr,	0x0);
				PE_P1L_M14A0_QWr01(pe1_top_ctrl_00,	load_time,		0x1);
				for(count=0;count<pstParams->size;count++)
				{
					lutdata = pstParams->data[count];
					PE_P1L_M14A0_Wr01(pe1_gmc_ia_data,	lut_data_g, GET_BITS(lutdata,  0, 10));
					PE_P1L_M14A0_Wr01(pe1_gmc_ia_data,	lut_data_b, GET_BITS(lutdata, 10, 10));
					PE_P1L_M14A0_Wr01(pe1_gmc_ia_data,	lut_data_r, GET_BITS(lutdata, 20, 10));
					PE_P1L_M14A0_WrFL(pe1_gmc_ia_data);
				}
				PE_P1L_M14A0_QWr01(pe1_gmc_ctrl_01,	gmc_lut_wr_done,		0x1); //set done
				PE_P1L_M14A0_QWr01(pe1_gmc_ctrl_01,	gmc_lut_wr_done,		0x0); //reset done
				PE_P1L_M14A0_QWr01(pe1_top_ctrl_00,	load_time,		0x0);
				ccm_info->lut_type = (ccm_info->lut_type == PE_CCM_HW_M14_LUT_A)? \
					PE_CCM_HW_M14_LUT_B:PE_CCM_HW_M14_LUT_A;
			}
		}
		else
		{
			PE_CCM_HW_M14_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * get gamma correction lut
 *
 * @param   *pstParams [in/out] LX_PE_CCM_GAMMALUT_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_CCM_HW_M14_GetGammaLut(LX_PE_CCM_GAMMALUT_T *pstParams)
{
	int ret = RET_OK;
	UINT32 count = 0;
	LX_PE_WIN_ID win_id;
	PE_CCM_HW_M14_CTRL_T *ccm_info=&_g_pe_ccm_hw_m14_info.ccm_inf;
	do {
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		PE_CCM_HW_M14_CHECK_CODE(!(pstParams->size),return RET_ERROR,"[%s,%d] size is zero.\n",__F__,__L__);
		PE_CCM_HW_M14_CHECK_CODE(!(pstParams->data),return RET_ERROR,"[%s,%d] data is null.\n",__F__,__L__);
		win_id = PE_GET_CHECKED_WINID(pstParams->win_id);
		if(PE_KDRV_VER_M14BX)
		{
			PE_CCM_HW_M14_DBG_PRINT("not supported.\n");
		}
		else if(PE_KDRV_VER_M14AX)
		{
			if(PE_CHECK_WIN0(win_id))
			{
				PE_P1L_M14A0_QWr01(pe1_gmc_ctrl_02,	gmc_mode,		0x2);//read mode
				PE_P1L_M14A0_QWr01(pe1_gmc_ctrl_01,	gmc_lut_rd_sel,(ccm_info->lut_type)? 0x1:0x0);
				PE_P1L_M14A0_QWr02(pe1_gmc_ia_ctrl,	capture_ai,		0x1, //ai mode on
													capture_addr,	0x0);
				//OS_MsecSleep(20);	// 20110530, sj.youm, 20m : experimental value //  ??
				PE_P1L_M14A0_QWr01(pe1_top_ctrl_00,	load_time,		0x1);
				for(count=0;count<pstParams->size;count++)
				{
					PE_P1L_M14A0_RdFL(pe1_gmc_ia_data);
					pstParams->data[count] = PE_P1L_M14A0_Rd(pe1_gmc_ia_data);
				}
				PE_P1L_M14A0_QWr01(pe1_gmc_ctrl_02,	gmc_mode,		0x1);
				PE_P1L_M14A0_QWr01(pe1_top_ctrl_00,	load_time,		0x0);
			}
		}
		else
		{
			PE_CCM_HW_M14_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	} while (0);
	return ret;
}
/**
 * set pixel replacement
 *
 * @param   *pstParams [in/out] LX_PE_CCM_PIXEL_REPLACE_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_CCM_HW_M14_SetPixelReplacement(LX_PE_CCM_PIXEL_REPLACE_T *pstParams)
{
	int ret = RET_OK;
	UINT32 pxl_en,r_rep_off,g_rep_off,b_rep_off;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		PE_CCM_HW_M14_DBG_PRINT("set[%d] : r:%d,g:%d,b:%d\n",pstParams->win_id,\
			pstParams->pxl_rep_r,pstParams->pxl_rep_g,pstParams->pxl_rep_b);
		r_rep_off = (pstParams->pxl_rep_r)? 0x0:0x1;
		g_rep_off = (pstParams->pxl_rep_g)? 0x0:0x1;
		b_rep_off = (pstParams->pxl_rep_b)? 0x0:0x1;
		pxl_en = (pstParams->pxl_rep_r | pstParams->pxl_rep_g | pstParams->pxl_rep_b)? 0x1:0x0;
		if(PE_KDRV_VER_M14BX)
		{
			PE_PE1_M14B0_QWr01(pe1_fsw_ctrl_00,reg_color_filter_status,(!pxl_en)? 0x0:\
					(!pstParams->pxl_rep_r)? 0x1:(!pstParams->pxl_rep_g)? 0x2:0x3);//0x0~0x2:off,r,g,b
		}
		else if(PE_KDRV_VER_M14AX)
		{
			if(PE_CHECK_WIN0(pstParams->win_id))
			{
				PE_P1L_M14A0_QWr01(pe1_wb_ctrl_00,	reg_color_filter_status,(!pxl_en)? 0x0:\
					(!pstParams->pxl_rep_r)? 0x1:(!pstParams->pxl_rep_g)? 0x2:0x3);//0x0~0x2:off,r,g,b
			}
		}
		else
		{
			PE_CCM_HW_M14_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);

	return ret;
}
/**
 * get pixel replacement
 *
 * @param   *pstParams [in/out] LX_PE_CCM_PIXEL_REPLACE_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_CCM_HW_M14_GetPixelReplacement(LX_PE_CCM_PIXEL_REPLACE_T *pstParams)
{
	int ret = RET_OK;
	LX_PE_WIN_ID win_id;
	UINT32 cflter_state=0;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		win_id = PE_GET_CHECKED_WINID(pstParams->win_id);
		if(PE_KDRV_VER_M14BX)
		{
			if(PE_CHECK_WIN0(win_id))
			{
				PE_PE1_M14B0_QRd01(pe1_fsw_ctrl_00,	reg_color_filter_status,	cflter_state);
			}
		}
		else if(PE_KDRV_VER_M14AX)
		{
			if(PE_CHECK_WIN0(win_id))
			{
				PE_P1L_M14A0_QRd01(pe1_wb_ctrl_00,  reg_color_filter_status,	cflter_state);
			}
		}
		else
		{
			PE_CCM_HW_M14_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
		pstParams->pxl_rep_r = (cflter_state==0x1)? 0x1:0x0;
		pstParams->pxl_rep_g = (cflter_state==0x2)? 0x1:0x0;
		pstParams->pxl_rep_b = (cflter_state==0x3)? 0x1:0x0;
		PE_CCM_HW_M14_DBG_PRINT("get[%d] : r:%d, g:%d, b:%d\n", pstParams->win_id, \
			pstParams->pxl_rep_r, pstParams->pxl_rep_g, pstParams->pxl_rep_b);
	}while(0);
	return ret;
}
/**
 * set white Balance
 *
 * @param   *pstParams [in/out] LX_PE_CCM_WB_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_CCM_HW_M14_SetWhiteBalance(LX_PE_CCM_WB_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		PE_CCM_HW_M14_DBG_PRINT("set[%d] : gain:%d,%d,%d, offset:%d,%d,%d\n",\
			pstParams->win_id,pstParams->r_gain,pstParams->g_gain,pstParams->b_gain,\
			pstParams->r_offset,pstParams->g_offset,pstParams->b_offset);
		if(PE_KDRV_VER_M14BX)
		{
			PE_CCM_HW_M14_DBG_PRINT("not supported.\n");
		}
		else if(PE_KDRV_VER_M14AX)
		{
			if(PE_CHECK_WIN0(pstParams->win_id))
			{
				PE_P1L_M14A0_QWr03(pe1_wb_ctrl_01,  user_ctrl_g_gain,	GET_BITS(pstParams->g_gain,0,8),\
													user_ctrl_b_gain,	GET_BITS(pstParams->b_gain,0,8),\
													user_ctrl_r_gain,	GET_BITS(pstParams->r_gain,0,8));

				PE_P1L_M14A0_QWr03(pe1_wb_ctrl_02,  user_ctrl_g_offset,	GET_BITS(pstParams->g_offset,0,8),\
													user_ctrl_b_offset,	GET_BITS(pstParams->b_offset,0,8),\
													user_ctrl_r_offset,	GET_BITS(pstParams->r_offset,0,8));

			}
		}
		else
		{
			PE_CCM_HW_M14_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * get white Balance
 *
 * @param   *pstParams [in/out] LX_PE_CCM_WB_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_CCM_HW_M14_GetWhiteBalance(LX_PE_CCM_WB_T *pstParams)
{
	int ret = RET_OK;
	LX_PE_WIN_ID win_id;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		win_id = PE_GET_CHECKED_WINID(pstParams->win_id);
		if(PE_KDRV_VER_M14BX)
		{
			PE_CCM_HW_M14_DBG_PRINT("not supported.\n");
		}
		else if(PE_KDRV_VER_M14AX)
		{
			if(PE_CHECK_WIN0(win_id))
			{

				PE_P1L_M14A0_QRd03(pe1_wb_ctrl_01, 	user_ctrl_g_gain,	pstParams->g_gain,\
													user_ctrl_b_gain,	pstParams->b_gain,\
													user_ctrl_r_gain,	pstParams->r_gain);
				PE_P1L_M14A0_QRd03(pe1_wb_ctrl_02, 	user_ctrl_g_offset,	pstParams->g_offset,\
													user_ctrl_b_offset,	pstParams->b_offset,\
													user_ctrl_r_offset,	pstParams->r_offset);
			}
		}
		else
		{
			PE_CCM_HW_M14_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
		PE_CCM_HW_M14_DBG_PRINT("get[%d] : gain:%d,%d,%d, offset:%d,%d,%d\n",\
			pstParams->win_id,pstParams->r_gain,pstParams->g_gain,pstParams->b_gain,\
			pstParams->r_offset,pstParams->g_offset,pstParams->b_offset);
	}while(0);
	return ret;
}
/**
 * set auto correction
 *
 * @param   *pstParams [in/out] LX_PE_CCM_AUTO_CR_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_CCM_HW_M14_SetAutoCorrection(LX_PE_CCM_AUTO_CR_T *pstParams)
{
	int ret = RET_OK;
	LX_PE_CCM_AUTO_CR_T *acr_info = &_g_pe_ccm_hw_m14_info.auto_cr;
	do{
		CHECK_KNULL(pstParams);
		PE_CCM_HW_M14_DBG_PRINT("set pstParams : en:%d, max_hist:%d,val_bins:%d, th0,1:%d,%d\n",\
			pstParams->enable,pstParams->th_max_hist,pstParams->th_valid_bins,\
			pstParams->adj_th0,pstParams->adj_th1);
		if(PE_KDRV_VER_M14BX)
		{
			PE_PE1_M14B0_QWr04(pe1_lrcr_ctrl_01,	th_max_hist,	GET_BITS(pstParams->th_max_hist,0,8),\
													th_valid_bins,	GET_BITS(pstParams->th_valid_bins,0,8),\
													th1,			GET_BITS(pstParams->adj_th1,0,8),\
													th0,			GET_BITS(pstParams->adj_th0,0,8));
			PE_PE1_M14B0_QWr01(pe1_lrcr_ctrl_00,	enable,			pstParams->enable? 0x1:0x0);
			acr_info->enable = pstParams->enable? 0x1:0x0;
		}
		else if(PE_KDRV_VER_M14AX)
		{
			PE_P1L_M14A0_QWr04(pe1_lrcr_ctrl_01,	th_max_hist,	GET_BITS(pstParams->th_max_hist,0,8),\
													th_valid_bins,	GET_BITS(pstParams->th_valid_bins,0,8),\
													th1,			GET_BITS(pstParams->adj_th1,0,8),\
													th0,			GET_BITS(pstParams->adj_th0,0,8));
			PE_P1L_M14A0_QWr01(pe1_lrcr_ctrl_00,	enable,			pstParams->enable? 0x1:0x0);
			acr_info->enable = pstParams->enable? 0x1:0x0;
		}
		else
		{
			PE_CCM_HW_M14_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);

	return ret;
}
/**
 * get auto correction
 *
 * @param   *pstParams [in/out] LX_PE_CCM_AUTO_CR_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_CCM_HW_M14_GetAutoCorrection(LX_PE_CCM_AUTO_CR_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_M14BX)
		{
			PE_PE1_M14B0_QRd04(pe1_lrcr_ctrl_01,	th_max_hist,	pstParams->th_max_hist,\
													th_valid_bins,	pstParams->th_valid_bins,\
													th1,			pstParams->adj_th1,\
													th0,			pstParams->adj_th0);
			PE_PE1_M14B0_QRd01(pe1_lrcr_ctrl_00,	enable,			pstParams->enable);
		}
		else if(PE_KDRV_VER_M14AX)
		{
			PE_P1L_M14A0_QRd04(pe1_lrcr_ctrl_01,	th_max_hist,	pstParams->th_max_hist,\
													th_valid_bins,	pstParams->th_valid_bins,\
													th1,			pstParams->adj_th1,\
													th0,			pstParams->adj_th0);
			PE_P1L_M14A0_QRd01(pe1_lrcr_ctrl_00,	enable,			pstParams->enable);
		}
		else
		{
			PE_CCM_HW_M14_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
		PE_CCM_HW_M14_DBG_PRINT("get pstParams : en:%d, max_hist:%d,val_bins:%d, th0,1:%d,%d\n",\
			pstParams->enable,pstParams->th_max_hist,pstParams->th_valid_bins,\
			pstParams->adj_th0,pstParams->adj_th1);
	}while(0);

	return ret;
}
/**
 * set lrcr config
 *  - The lrcr of out type 2d_3d works like 2d,
 *    on [m14a0 with external frc] and [m14a0].
 *	  But it does like 3d(dual tb) on [m14a0 with internal frc]
 *
 * @param   *pCfg [in] PE_TSK_M14_CFG_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_CCM_HW_M14_SetLrcrConfig(PE_TSK_M14_CFG_T *pCfg)
{
	int ret = RET_OK;
	UINT32 ff_mode_on,histo_mode,ll_mode_en;
	LX_PE_INF_DISPLAY_T *disp0_info = &pCfg->inf.disp_info[LX_PE_WIN_0];
	PE_CCM_HW_M14_CTRL_T *ccm_info=&_g_pe_ccm_hw_m14_info.ccm_inf;
	do {
		if(PE_KDRV_VER_M14BX)
		{
			if(disp0_info->mode.is_ext_frc)	/* external frc */
			{
				ll_mode_en = 0x0;		//ll md off
				switch(disp0_info->out_type)
				{
					case LX_PE_OUT_2D:
					case LX_PE_OUT_2D_3D:
						{
							ff_mode_on = 0x0;		//ff md off
							histo_mode = 0x3;		//32 bin
						}
						break;
					case LX_PE_OUT_TB:
					case LX_PE_OUT_SS:
						{
							ff_mode_on = 0x0;		//ff md off
							histo_mode = 0x0;		//256 bin
						}
						break;
					case LX_PE_OUT_FS:			/* N/A on external frc */
					case LX_PE_OUT_DUAL_TB:		/* N/A on external frc */
					case LX_PE_OUT_DUAL_SS:		/* N/A on external frc */
					case LX_PE_OUT_DUAL_FULL:
						{
							ff_mode_on = 0x1;		//ff md on
							histo_mode = 0x0;		//256 bin
						}
						break;
					case LX_PE_OUT_UD:
					case LX_PE_OUT_PIP:
					case LX_PE_OUT_3D_2D:
					default:
						{
							ff_mode_on = 0x0;		//ff md off
							histo_mode = 0x0;		//256 bin
						}
						break;
				}
			}
			else	/* internal frc */
			{
				switch(disp0_info->out_type)
				{
					case LX_PE_OUT_2D:
					case LX_PE_OUT_2D_3D:
						{
							ff_mode_on = 0x0;		//ff md off
							ll_mode_en = 0x0;		//ll md off
							histo_mode = 0x3;		//32 bin
						}
						break;
					case LX_PE_OUT_DUAL_TB:
					case LX_PE_OUT_DUAL_SS:
					case LX_PE_OUT_DUAL_FULL:
						{
							ff_mode_on = 0x0;		//ff md off
							ll_mode_en = 0x1;		//ll md on
							histo_mode = 0x0;		//256 bin
						}
						break;
					case LX_PE_OUT_TB:/* N/A on internal frc */
					case LX_PE_OUT_SS:/* N/A on internal frc */
					case LX_PE_OUT_FS:/* N/A on internal frc */
					case LX_PE_OUT_UD:
					case LX_PE_OUT_PIP:
					case LX_PE_OUT_3D_2D:
					default:
						{
							ff_mode_on = 0x0;		//ff md off
							ll_mode_en = 0x0;		//ll md off
							histo_mode = 0x0;		//256 bin
						}
						break;
				}
			}
			/* ff mode */
			if(ccm_info->ff_mode_on!=ff_mode_on)
			{
				PE_CCM_HW_M14_DBG_PRINT("[inf]is_ext_frc:%d,out:%d\n",\
					disp0_info->mode.is_ext_frc,disp0_info->out_type);
				PE_CCM_HW_M14_DBG_PRINT("[set]ff_md:prev:%d,cur:%d\n",ccm_info->ff_mode_on,ff_mode_on);
				PE_PE1_M14B0_RdFL(pe1_lrcr_ctrl_00);
				PE_PE1_M14B0_Wr01(pe1_lrcr_ctrl_00, ff_mode_en,		ff_mode_on);
				PE_PE1_M14B0_WrFL(pe1_lrcr_ctrl_00);
				ccm_info->ff_mode_on = ff_mode_on;
			}
			/* ll mode */
			if(ccm_info->ll_mode_en!=ll_mode_en)
			{
				PE_CCM_HW_M14_DBG_PRINT("[inf]is_ext_frc:%d,out:%d\n",\
					disp0_info->mode.is_ext_frc,disp0_info->out_type);
				PE_CCM_HW_M14_DBG_PRINT("[set]ll_md:prev:%d,cur:%d\n",ccm_info->ll_mode_en,ll_mode_en);
				PE_PE1_M14B0_RdFL(pe1_lrcr_ctrl_00);
				PE_PE1_M14B0_Wr01(pe1_lrcr_ctrl_00, ll_mode_enable,	ll_mode_en);
				PE_PE1_M14B0_WrFL(pe1_lrcr_ctrl_00);
				ccm_info->ll_mode_en = ll_mode_en;
			}
			/* histo mode */
			if(ccm_info->histo_mode!=histo_mode)
			{
				PE_CCM_HW_M14_DBG_PRINT("[inf]is_ext_frc:%d,out:%d\n",\
					disp0_info->mode.is_ext_frc,disp0_info->out_type);
				PE_CCM_HW_M14_DBG_PRINT("[set]hist_md:prev:%d,cur:%d\n",ccm_info->histo_mode,histo_mode);
				PE_PE1_M14B0_RdFL(pe1_lrcr_ctrl_00);
				PE_PE1_M14B0_Wr01(pe1_lrcr_ctrl_00, hist_mode,		histo_mode);
				PE_PE1_M14B0_WrFL(pe1_lrcr_ctrl_00);
				ccm_info->histo_mode = histo_mode;
			}
		}
		else if(PE_KDRV_VER_M14AX)
		{
			if(disp0_info->mode.is_ext_frc)	/* external frc */
			{
				ll_mode_en = 0x0;		//ll md off
				switch(disp0_info->out_type)
				{
					case LX_PE_OUT_2D:
					case LX_PE_OUT_2D_3D:
						{
							ff_mode_on = 0x0;		//ff md off
							histo_mode = 0x3;		//32 bin
						}
						break;
					case LX_PE_OUT_TB:
					case LX_PE_OUT_SS:
						{
							ff_mode_on = 0x0;		//ff md off
							histo_mode = 0x0;		//256 bin
						}
						break;
					case LX_PE_OUT_FS:			/* N/A on external frc */
					case LX_PE_OUT_DUAL_TB:		/* N/A on external frc */
					case LX_PE_OUT_DUAL_SS:		/* N/A on external frc */
					case LX_PE_OUT_DUAL_FULL:
						{
							ff_mode_on = 0x1;		//ff md on
							histo_mode = 0x0;		//256 bin
						}
						break;
					case LX_PE_OUT_UD:
					case LX_PE_OUT_PIP:
					case LX_PE_OUT_3D_2D:
					default:
						{
							ff_mode_on = 0x0;		//ff md off
							histo_mode = 0x0;		//256 bin
						}
						break;
				}
			}
			else	/* internal frc */
			{
				switch(disp0_info->out_type)
				{
					case LX_PE_OUT_2D:
					case LX_PE_OUT_2D_3D:
						{
							ff_mode_on = 0x0;		//ff md off
							ll_mode_en = 0x0;		//ll md off
							histo_mode = 0x3;		//32 bin
						}
						break;
					case LX_PE_OUT_DUAL_TB:
					case LX_PE_OUT_DUAL_SS:
					case LX_PE_OUT_DUAL_FULL:
						{
							ff_mode_on = 0x0;		//ff md off
							ll_mode_en = 0x1;		//ll md on
							histo_mode = 0x0;		//256 bin
						}
						break;
					case LX_PE_OUT_TB:/* N/A on internal frc */
					case LX_PE_OUT_SS:/* N/A on internal frc */
					case LX_PE_OUT_FS:/* N/A on internal frc */
					case LX_PE_OUT_UD:
					case LX_PE_OUT_PIP:
					case LX_PE_OUT_3D_2D:
					default:
						{
							ff_mode_on = 0x0;		//ff md off
							ll_mode_en = 0x0;		//ll md off
							histo_mode = 0x0;		//256 bin
						}
						break;
				}
			}
			/* ff mode */
			if(ccm_info->ff_mode_on!=ff_mode_on)
			{
				PE_CCM_HW_M14_DBG_PRINT("[inf]is_ext_frc:%d,out:%d\n",\
					disp0_info->mode.is_ext_frc,disp0_info->out_type);
				PE_CCM_HW_M14_DBG_PRINT("[set]ff_md:prev:%d,cur:%d\n",ccm_info->ff_mode_on,ff_mode_on);
				PE_P1L_M14A0_RdFL(pe1_lrcr_ctrl_00);
				PE_P1L_M14A0_Wr01(pe1_lrcr_ctrl_00, ff_mode_en,		ff_mode_on);
				PE_P1L_M14A0_WrFL(pe1_lrcr_ctrl_00);
				ccm_info->ff_mode_on = ff_mode_on;
			}
			/* ll mode */
			if(ccm_info->ll_mode_en!=ll_mode_en)
			{
				PE_CCM_HW_M14_DBG_PRINT("[inf]is_ext_frc:%d,out:%d\n",\
					disp0_info->mode.is_ext_frc,disp0_info->out_type);
				PE_CCM_HW_M14_DBG_PRINT("[set]ll_md:prev:%d,cur:%d\n",ccm_info->ll_mode_en,ll_mode_en);
				PE_P1L_M14A0_RdFL(pe1_lrcr_ctrl_00);
				PE_P1L_M14A0_Wr01(pe1_lrcr_ctrl_00, ll_mode_enable,	ll_mode_en);
				PE_P1L_M14A0_WrFL(pe1_lrcr_ctrl_00);
				ccm_info->ll_mode_en = ll_mode_en;
			}
			/* histo mode */
			if(ccm_info->histo_mode!=histo_mode)
			{
				PE_CCM_HW_M14_DBG_PRINT("[inf]is_ext_frc:%d,out:%d\n",\
					disp0_info->mode.is_ext_frc,disp0_info->out_type);
				PE_CCM_HW_M14_DBG_PRINT("[set]hist_md:prev:%d,cur:%d\n",ccm_info->histo_mode,histo_mode);
				PE_P1L_M14A0_RdFL(pe1_lrcr_ctrl_00);
				PE_P1L_M14A0_Wr01(pe1_lrcr_ctrl_00, hist_mode,		histo_mode);
				PE_P1L_M14A0_WrFL(pe1_lrcr_ctrl_00);
				ccm_info->histo_mode = histo_mode;
			}
		}
		else
		{
			PE_CCM_HW_M14_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	} while (0);
	return ret;
}
/**
 * get d3d check flag to set auto correction gain
 *
 * @param   *pCfg [in] PE_TSK_M14_CFG_T
 * @return  d3d_chk(0:normal,1:not tb,2:not ss), see PE_CCM_HW_M14_D3D_CHK
 * @see
 * @author
 */
static UINT32 PE_CCM_HW_M14_GetAcGain3dInfo(PE_TSK_M14_CFG_T *pCfg)
{
	int ret = RET_OK;
	UINT32 type_chg=0;
	static UINT32 d3d_chk=PE_CCM_HW_M14_D3D_CHK_NORMAL;// 0:normal,1:not tb,2:not ss
	static UINT32 count = 0;
	static PE_HST_HW_M14_DET_3D_FMT cur_fmt = PE_HST_HW_M14_DET_3D_FMT_MAX;
	static LX_PE_OUT_TYPE pre_out = LX_PE_OUT_NUM;
	static LX_PE_3D_IN_TYPE pre_in = LX_PE_3D_IN_NUM;
	PE_INF_M14_CTRL_T *ctrl0_info = &pCfg->inf.ctrl_info[LX_PE_WIN_0];
	LX_PE_INF_DISPLAY_T *disp0_info = &pCfg->inf.disp_info[LX_PE_WIN_0];
	do{
		CHECK_KNULL(pCfg);
		if(PE_KDRV_VER_M14)
		{
			/* 0. check pattern detect result */
			if(!ctrl0_info->mute_on)
			{
				/* The auto fmt detection might work on both out type 2d and 2d_3d. */
				if(disp0_info->out_type==LX_PE_OUT_2D || disp0_info->out_type==LX_PE_OUT_2D_3D)
				{
					cur_fmt = ctrl0_info->det_3df;
				}
			}
			/* 1. check type change */
			if(pre_out!=disp0_info->out_type||pre_in!=disp0_info->in_type)
			{
				type_chg=1;
				PE_CCM_HW_M14_DBG_PRINT("[chg:0to1][mute:%d][o,i:pre:%d,%d,cur:%d,%d]\n"\
					"[def,cur_fmt:%d,%d][d3d_chk:%d(%s)]\n",\
					ctrl0_info->mute_on,pre_out,pre_in,disp0_info->out_type,disp0_info->in_type,\
					ctrl0_info->det_3df,cur_fmt,d3d_chk,\
					(d3d_chk==PE_CCM_HW_M14_D3D_CHK_NORMAL)? "normal":\
					(d3d_chk==PE_CCM_HW_M14_D3D_CHK_NOT_TB)? "not_tb":"not_ss");
				pre_out = disp0_info->out_type;
				pre_in = disp0_info->in_type;
			}
			/* 2. check d3d */
			if((disp0_info->out_type==LX_PE_OUT_TB || \
				disp0_info->out_type==LX_PE_OUT_SS || \
				disp0_info->out_type==LX_PE_OUT_FS || \
				disp0_info->out_type==LX_PE_OUT_DUAL_TB || \
				disp0_info->out_type==LX_PE_OUT_DUAL_SS || \
				disp0_info->out_type==LX_PE_OUT_DUAL_FULL) && \
				(disp0_info->in_type==LX_PE_3D_IN_TB || \
				disp0_info->in_type==LX_PE_3D_IN_SS))
			{
				if(type_chg)
				{
					if(disp0_info->in_type==LX_PE_3D_IN_SS && \
						cur_fmt!=PE_HST_HW_M14_DET_3D_FMT_3D_SS)
					{
						d3d_chk=PE_CCM_HW_M14_D3D_CHK_NOT_SS;	//not ss
					}
					else if(disp0_info->in_type==LX_PE_3D_IN_TB && \
						cur_fmt!=PE_HST_HW_M14_DET_3D_FMT_3D_TB)
					{
						d3d_chk=PE_CCM_HW_M14_D3D_CHK_NOT_TB;	//not tb
					}
					else
					{
						d3d_chk=PE_CCM_HW_M14_D3D_CHK_NORMAL;	//normal
					}
					PE_CCM_HW_M14_DBG_PRINT("[decide_chk][mute:%d][o,i:pre:%d,%d,cur:%d,%d]\n"\
						"[def,cur_fmt:%d,%d][d3d_chk:%d(%s)]\n",\
						ctrl0_info->mute_on,pre_out,pre_in,disp0_info->out_type,disp0_info->in_type,\
						ctrl0_info->det_3df,cur_fmt,d3d_chk,\
						(d3d_chk==PE_CCM_HW_M14_D3D_CHK_NORMAL)? "normal":\
						(d3d_chk==PE_CCM_HW_M14_D3D_CHK_NOT_TB)? "not_tb":"not_ss");
				}
				#if 0	//20130119, sj.youm, protection on
				if(PE_KDRV_VER_M14BX)
				{
					ret = RET_OK;
				}
				else
				#endif
				if(PE_KDRV_VER_M14)	//protection off until d3d ok for m14a0(temporal)
				{
					d3d_chk=PE_CCM_HW_M14_D3D_CHK_NORMAL;	//normal
				}
				/*else keep d3d_chk & don't care*/
			}
			else
			{
				d3d_chk=PE_CCM_HW_M14_D3D_CHK_NORMAL;	//normal
			}
			if(_g_ccm_hw_m14_trace)
			{
				if(count>PE_PRINT_COUNT_NUM)
				{
					PE_CCM_HW_M14_DBG_PRINT("[state][chg:%d][mute:%d][o,i:pre:%d,%d,cur:%d,%d]\n"\
						"[def,cur_fmt:%d,%d][d3d_chk:%d(%s)]\n",type_chg,\
						ctrl0_info->mute_on,pre_out,pre_in,disp0_info->out_type,disp0_info->in_type,\
						ctrl0_info->det_3df,cur_fmt,d3d_chk,\
						(d3d_chk==PE_CCM_HW_M14_D3D_CHK_NORMAL)? "normal":\
						(d3d_chk==PE_CCM_HW_M14_D3D_CHK_NOT_TB)? "not_tb":"not_ss");
					count=0;
				}
				count++;
			}
		}
		else
		{
			PE_CCM_HW_M14_DBG_PRINT("nothing to do.\n");
		}
	}while(0);
	return d3d_chk;
}
/**
 * set auto correction gain, work around for lrcr error
 *
 * @param   *pCfg [in] PE_TSK_M14_CFG_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_CCM_HW_M14_SetAutoCrGain(PE_TSK_M14_CFG_T *pCfg)
{
	int ret = RET_OK;
	static UINT32 count = 0;
	UINT32 d3d_chk = PE_CCM_HW_M14_D3D_CHK_NORMAL;// 0:normal,1:not tb,2:not ss
	UINT32 user_3d = 0;
	UINT32 lrcr_gain = 0xff;
	PE_INF_M14_CTRL_T *ctrl0_info = &pCfg->inf.ctrl_info[LX_PE_WIN_0];
	LX_PE_INF_DISPLAY_T *disp0_info = &pCfg->inf.disp_info[LX_PE_WIN_0];
	PE_CCM_HW_M14_CTRL_T *ccm_info = &_g_pe_ccm_hw_m14_info.ccm_inf;
	do{
		CHECK_KNULL(pCfg);
		if(PE_KDRV_VER_M14)
		{
			d3d_chk = PE_CCM_HW_M14_GetAcGain3dInfo(pCfg);
			user_3d = (disp0_info->out_type==LX_PE_OUT_UD && \
				(disp0_info->user_o_type==LX_PE_OUT_TB || disp0_info->user_o_type==LX_PE_OUT_SS) && \
				(disp0_info->user_i_type==LX_PE_3D_IN_TB || disp0_info->user_i_type==LX_PE_3D_IN_SS))? 1:0;
			/**
			* case of setting lrcr_gain to '0'
			*	if auto_cr_en is off,
			*	or if auto_cr_en is on, but d3d_chk is not tb(1) or not ss(2) (:=not normal),
			*	or if auto_cr_en is on, but sat_status is high(:= pattern),
			*	or if out_type is not one of TB,SS,FS and DUAL
			*	but no user_3d:1 & auto_cr_en:on
			*/
			if(!ctrl0_info->auto_cr_en || \
				(d3d_chk || ctrl0_info->sat_status>PE_CCM_HW_M14_SAT_STATUS_TH) || \
				(disp0_info->out_type!=LX_PE_OUT_TB && \
				disp0_info->out_type!=LX_PE_OUT_SS && \
				disp0_info->out_type!=LX_PE_OUT_FS && \
				disp0_info->out_type!=LX_PE_OUT_DUAL_TB && \
				disp0_info->out_type!=LX_PE_OUT_DUAL_SS && \
				disp0_info->out_type!=LX_PE_OUT_DUAL_FULL))
			{
				lrcr_gain = (user_3d && ctrl0_info->auto_cr_en)? PE_CCM_HW_M14_LRCR_GAIN:0x0;
			}
			else
			{
				lrcr_gain = PE_CCM_HW_M14_LRCR_GAIN;
			}
			if(ccm_info->lrcr_gain!=lrcr_gain)
			{
				if(PE_KDRV_VER_M14BX)
				{
					PE_PE1_M14B0_QWr01(pe1_lrcr_ctrl_00, reg_lrcr_gain,	lrcr_gain);
				}
				else if(PE_KDRV_VER_M14AX)
				{
					PE_P1L_M14A0_QWr01(pe1_lrcr_ctrl_00, reg_lrcr_gain,	lrcr_gain);
				}
				PE_CCM_HW_M14_DBG_PRINT("[set][gain:pre:0x%x,cur:0x%x][en:%d][user3d:%d]\n"\
					"[o,i:%d,%d][s_st:%d][chk:%d(%s)]\n\n",\
					ccm_info->lrcr_gain,lrcr_gain,ctrl0_info->auto_cr_en,user_3d,\
					disp0_info->out_type,disp0_info->in_type,ctrl0_info->sat_status,\
					d3d_chk,(d3d_chk==PE_CCM_HW_M14_D3D_CHK_NORMAL)? "normal":\
					(d3d_chk==PE_CCM_HW_M14_D3D_CHK_NOT_TB)? "not_tb":"not_ss");
				ccm_info->lrcr_gain=lrcr_gain;
			}
			if(_g_ccm_hw_m14_trace)
			{
				if(count>PE_PRINT_COUNT_NUM)
				{
					PE_CCM_HW_M14_DBG_PRINT("[state][gain:0x%x][en:%d][user3d:%d]\n"\
						"[o,i:%d,%d][s_st:%d][chk:%d(%s)]\n\n",\
						lrcr_gain,ctrl0_info->auto_cr_en,user_3d,\
						disp0_info->out_type,disp0_info->in_type,ctrl0_info->sat_status,\
						d3d_chk,(d3d_chk==PE_CCM_HW_M14_D3D_CHK_NORMAL)? "normal":\
						(d3d_chk==PE_CCM_HW_M14_D3D_CHK_NOT_TB)? "not_tb":"not_ss");
					count=0;
				}
				count++;
			}
		}
		else
		{
			PE_CCM_HW_M14_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * set wb gamma,degamma
 *
 * @param   *pCfg [in] PE_TSK_M14_CFG_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_CCM_HW_M14A_SetWbGammaDegamma(PE_TSK_M14_CFG_T *pCfg)
{
	int ret = RET_OK;
	static UINT32 count = 0;
	static UINT32 pre_is_wb_wog = 0;
	UINT32 cur_is_wb_wog = 0;
	LX_PE_INF_DISPLAY_T *disp0_info = &pCfg->inf.disp_info[LX_PE_WIN_0];
	do{
		CHECK_KNULL(pCfg);
		if(PE_KDRV_VER_M14AX)
		{
			cur_is_wb_wog = (disp0_info->mode.is_wb_wog)? 1:0;
			if(pre_is_wb_wog!=cur_is_wb_wog)
			{
				PE_P1L_M14A0_QWr02(pe1_wb_ctrl_00,	degamma_en, cur_is_wb_wog? 0:1,\
													gamma_en, 	cur_is_wb_wog? 0:1);
				PE_CCM_HW_M14_DBG_PRINT("[set][is_wb_wog:pre:%d,cur:%d]\n",\
					pre_is_wb_wog,cur_is_wb_wog);
				pre_is_wb_wog=cur_is_wb_wog;
			}
			if(_g_ccm_hw_m14_trace)
			{
				if(count>PE_PRINT_COUNT_NUM)
				{
					PE_CCM_HW_M14_DBG_PRINT("[state][is_wb_wog:%d]\n",cur_is_wb_wog);
					count=0;
				}
				count++;
			}
		}
		else
		{
			PE_CCM_HW_M14_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * get current ccm settings
 *
 * @param   *pstParams [in] PE_CCM_HW_M14_SETTINGS_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_CCM_HW_M14_GetCurCcmSettings(PE_CCM_HW_M14_SETTINGS_T *pstParams)
{
	int ret = RET_OK;
	static UINT32 count = 0;
	PE_CCM_HW_M14_SETTINGS_T *pInfo = &_g_pe_ccm_hw_m14_info;
	PE_CCM_HW_M14_CTRL_T *ccm_info = &_g_pe_ccm_hw_m14_info.ccm_inf;
	LX_PE_CCM_AUTO_CR_T *acr_info = &_g_pe_ccm_hw_m14_info.auto_cr;
	do{
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_M14)
		{
			memcpy(pstParams,pInfo,sizeof(PE_CCM_HW_M14_SETTINGS_T));
			if(_g_ccm_hw_m14_trace)
			{
				if(count>PE_PRINT_COUNT_NUM)
				{
					PE_CCM_HW_M14_DBG_PRINT("lrcr(a0) sel : r,l_h:%d,%d, r,l_p:%d,%d, r,l_o:%d,%d\n",\
						ccm_info->r_h_select,ccm_info->l_h_select,ccm_info->r_p_select,\
						ccm_info->l_p_select,ccm_info->r_o_select,ccm_info->l_o_select);
					PE_CCM_HW_M14_DBG_PRINT("lrcr(a0) r,l_bypass:%d,%d\n",\
						ccm_info->ppr_bypass,ccm_info->ppl_bypass);
					PE_CCM_HW_M14_DBG_PRINT("lrcr ff_md:%d, ll_md:%d, h_md:%d, gain:%d, lut:%d\n",\
						ccm_info->ff_mode_on,ccm_info->ll_mode_en,ccm_info->histo_mode,\
						ccm_info->lrcr_gain,ccm_info->lut_type);
					PE_CCM_HW_M14_DBG_PRINT("acr en:%d, max_hist:%d,val_bins:%d, th0,1:%d,%d\n",\
						acr_info->enable,acr_info->th_max_hist,acr_info->th_valid_bins,\
						acr_info->adj_th0,acr_info->adj_th1);
					count=0;
				}
				count++;
			}
		}
		else
		{
			PE_CCM_HW_M14_DBG_PRINT("nothing to do.\n");
		}
	} while (0);
	return ret;
}

