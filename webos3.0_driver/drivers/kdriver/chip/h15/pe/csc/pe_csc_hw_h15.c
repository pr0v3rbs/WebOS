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

/** @file pe_csc_hw_h15.c
 *
 *  driver for picture enhance csc functions. ( used only within kdriver )
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

#include "pe_hw_h15.h"
#include "pe_reg_h15.h"
#include "pe_fwi_h15.h"
#include "pe_csc_hw_h15.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
#define PE_CSC_HW_H15_BT709_H_SIZE	1024

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define PE_CSC_HW_H15_ERROR	printk
#define PE_CSC_HW_H15_DBG_PRINT(fmt,args...)	\
	if(_g_csc_hw_h15_trace) printk("[%x,%x][%s,%d] "fmt,PE_CHIP_VER,g_pe_kdrv_ver_mask,__F__,__L__,##args)
#define PE_CSC_HW_H15_CHECK_CODE(_checker,_action,fmt,args...)	\
	if(_checker){PE_CSC_HW_H15_ERROR(fmt,##args);_action;}

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
static int PE_CSC_HW_H15_SetPostCscMode(PE_TSK_H15_CFG_T *pCfg);
static int PE_CSC_HW_H15_SetPostCscReg(UINT32 type);

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
static UINT32 _g_csc_hw_h15_trace=0x0;		//default should be off.
static PE_CSC_HW_H15_SETTINGS_T _g_pe_csc_hw_h15_info;

/* input csc, bypass */
const UINT16 _g_pe_csc_hw_h15_in_mtrx_byps[PE_NUM_OF_CSC_COEF] = {
	0x1000, 0x0000, 0x0000,		//  1.0000, 0.0000, 0.0000
	0x0000, 0x1000, 0x0000,		//  0.0000, 1.0000, 0.0000
	0x0000, 0x0000, 0x1000};	//  0.0000, 0.0000, 1.0000
const UINT16 _g_pe_csc_hw_h15_in_ofst_byps[PE_NUM_OF_CSC_OFST] = {
	0x0000, 0x0000, 0x0000,		//   0,   0,   0
	0x0000, 0x0000, 0x0000};	//   0,   0,   0

/* post csc, normal, ncl2020 ycbcr to gbr*/
const UINT16 _g_pe_csc_hw_h15_post_1x3_mtrx_bypass[PE_NUM_OF_CSC_COEF] = {
	0x1000, 0x0000, 0x0000,		//  1.000, 0.000, 0.000
	0x0000, 0x1000, 0x0000,		//  0.000, 1.000, 0.000
	0x0000, 0x0000, 0x1000};	//  0.000, 0.000, 1.000
const UINT16 _g_pe_csc_hw_h15_post_1x3_y709_expand[PE_NUM_OF_CSC_COEF] = {
	0x129F, 0xFC98, 0xF775,		//  1.164,-0.213,-0.534
	0x129F, 0x21D7, 0x0000,   	//  1.164, 2.115, 0.000
	0x129F, 0x0000, 0x1CB0};  	//  1.164, 0.000, 1.793
const UINT16 _g_pe_csc_hw_h15_post_1x3_y709_linear[PE_NUM_OF_CSC_COEF] = {
	0x1000, 0xFD13, 0xF8A8,		//  1.000,-0.183,-0.459
	0x1000, 0x1D0E, 0x0000,   	//  1.000, 1.816, 0.000
	0x1000, 0x0000, 0x18A3};  	//  1.000, 0.000, 1.540
const UINT16 _g_pe_csc_hw_h15_post_1x3_y601_expand[PE_NUM_OF_CSC_COEF] = {
	0x129F, 0xF9BF, 0xF2FE,		//  1.164,-0.391,-0.813
	0x129F, 0x2049, 0x0000,  	//  1.164, 2.018, 0.000
	0x129F, 0x0000, 0x1989};  	//  1.164, 0.000, 1.596
const UINT16 _g_pe_csc_hw_h15_post_1x3_y601_linear[PE_NUM_OF_CSC_COEF] = {
	0x1000, 0xFAA0, 0xF4D5,		//  1.000,-0.336,-0.698
	0x1000, 0x1BB6, 0x0000,   	//  1.000, 1.732, 0.000
	0x1000, 0x0000, 0x15EF};  	//  1.000, 0.000, 1.371
const UINT16 _g_pe_csc_hw_h15_post_1x3_ncl2020_exp[PE_NUM_OF_CSC_COEF] = {
	0x12AF, 0xFCFE, 0xF590,		//  1.168,-0.188,-0.652
	0x12AF, 0x225F, 0x0000,		//  1.168, 2.148, 0.000
	0x12AF, 0x0000, 0x1AF0};	//  1.168, 0.000, 1.684
const UINT16 _g_pe_csc_hw_h15_post_1x3_ncl2020_lin[PE_NUM_OF_CSC_COEF] = {
	0x1000, 0xFD6D, 0xF710,		//  1.000,-0.161,-0.559
	0x1000, 0x1D6E, 0x0000,		//  1.000, 1.839, 0.000
	0x1000, 0x0000, 0x1711};	//  1.000, 0.000, 1.442
const UINT16 _g_pe_csc_hw_h15_post_1x3_ofst_bypass[PE_NUM_OF_CSC_OFST] = {
	0x0000, 0x0000, 0x0000,		//   0,   0,   0
	0x0000, 0x0000, 0x0000};	//   0,   0,   0
const UINT16 _g_pe_csc_hw_h15_post_1x3_ofst_expand[PE_NUM_OF_CSC_OFST] = {
	0xFFC0, 0xFE00, 0xFE00,		// -64,-512,-512
	0x0000, 0x0000, 0x0000};	//   0,   0,   0
const UINT16 _g_pe_csc_hw_h15_post_1x3_ofst_linear[PE_NUM_OF_CSC_OFST] = {
	0x0000, 0xFE00, 0xFE00,		//   0,-512,-512
	0x0000, 0x0000, 0x0000};	//   0,   0,   0
const UINT16 _g_pe_csc_hw_h15_post_1x3_ofst_cl_lin[PE_NUM_OF_CSC_OFST] = {
	0x0000, 0x0000, 0x0000,		//   0,   0,   0
	0x0040, 0x0040, 0x0040};	//  64,  64,  64

/* post csc, cl2020 ybr to gbr(1x3) */
const UINT16 _g_pe_csc_hw_h15_post_1x3_cl_2020_exp[PE_NUM_OF_CSC_COEF] = {
	0x1799, 0xFE9A, 0xF9CD,		//  1.475,-0.088,-0.388
	0x0000, 0x1000, 0x0000,		//  0.000, 1.000, 0.000
	0x0000, 0x0000, 0x1000};	//  0.000, 0.000, 1.000
const UINT16 _g_pe_csc_hw_h15_post_1x3_cl_2020_lin[PE_NUM_OF_CSC_COEF] = {
	0x1435, 0xFECD, 0xFAB1,		//  1.263,-0.075,-0.332
	0x0000, 0x0DB3, 0x0000,		//  0.000, 1.857, 0.000
	0x0000, 0x0000, 0x0DB3};	//  0.000, 0.000, 1.857

/* post csc, cl2020 ycbcr to ybr(2x3a) */
const UINT16 _g_pe_csc_hw_h15_post_2x3a_cl2020_exp[PE_NUM_OF_CSC_COEF] = {
	0x12AF,	0x0000,	0x0000,		// 1.1678	0.000	0.000
	0x12AF,	0x2372,	0x0000,		// 1.1678	2.215	0.000
	0x12AF,	0x0000,	0x1F64};	// 1.1678	0.000	1.962
const UINT16 _g_pe_csc_hw_h15_post_2x3a_cl2020_lin[PE_NUM_OF_CSC_COEF] = {
	0x12AF,	0x0000,	0x0000,		// 1.1678	0.000	0.000
	0x12AF,	0x2372,	0x0000,		// 1.1678	2.215	0.000
	0x12AF,	0x0000,	0x1F64};	// 1.1678	0.000	1.962
/* post csc, cl2020 ycbcr to ybr(2x3b) */
const UINT16 _g_pe_csc_hw_h15_post_2x3b_cl2020_exp[PE_NUM_OF_CSC_COEF] = {
	0x12AF,	0x0000,	0x0000,		// 1.1678	0.000	0.000
	0x12AF,	0x1CE5,	0x0000,		// 1.1678	1.806	0.000
	0x12AF,	0x0000,	0x1227};	// 1.1678	0.000	1.134
const UINT16 _g_pe_csc_hw_h15_post_2x3b_cl2020_lin[PE_NUM_OF_CSC_COEF] = {
	0x12AF,	0x0000,	0x0000,		// 1.1678	0.000	0.000
	0x12AF,	0x1CE5,	0x0000,		// 1.1678	1.806	0.000
	0x12AF,	0x0000,	0x1227};	// 1.1678	0.000	1.134

/* post csc, cl2020 offset */
const UINT16 _g_pe_csc_hw_h15_post_2x3_ofs_2020_ex[PE_NUM_OF_CSC_OFST] = {
	0xFFC0, 0xFE00, 0xFE00,		// -64,-512,-512
	0x0000, 0x0000, 0x0000};	//   0,   0,   0
const UINT16 _g_pe_csc_hw_h15_post_2x3_ofs_2020_li[PE_NUM_OF_CSC_OFST] = {
	0xFFC0, 0xFE00, 0xFE00,		// -64,-512,-512
	0x0040, 0x0040, 0x0040};	//  64,  64,  64

/*========================================================================================
	Implementation Group
========================================================================================*/
/**
 * init csc
 *
 * @param   *pstParams [in] PE_CFG_CTRL_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_CSC_HW_H15_Init(PE_CFG_CTRL_T *pstParams)
{
	int ret = RET_OK;
	LX_PE_CSC_INPUT_T *p_in_csc = &(_g_pe_csc_hw_h15_info.pre_in_csc);
	do{
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H15)
		{
			PE_CSC_HW_H15_DBG_PRINT("init csc.\n");
			PE_CEO_PE1_H15A0_QWr01(pe1_ocsc_ctrl_00,outputcsc_sel,     0x1);	// 0x0:xvycc, 0x1:bt2020
			PE_CEO_PE1_H15A0_QWr05(pe1_xvy_ctrl_00, outputcsc_en,      0x0, \
                                                    degamma_en,        0x0, \
                                                    pcc_en,            0x0, \
                                                    gamma_en,          0x0, \
                                                    hsv_hsl_selection, 0x0);
			PE_CEO_PE1_H15A0_QWr01(pe1_bt20_ctrl_07,r_1x3_en,          0x1);
			PE_CEO_PE1_H15A0_QWr02(pe1_bt20_ctrl_08,decontour_en,      0x0, \
													bit_mode,          0x0);
			PE_CEO_PE1_H15A0_QWr01(pe1_bt20_ctrl_09,reg_csc_cb_th,     512);
			PE_CEO_PE1_H15A0_QWr01(pe1_bt20_ctrl_0c,reg_csc_cr_th,     512);
			_g_pe_csc_hw_h15_info.pre_post_type = LX_PE_POST_CSC_NUM;	//reset pre value
			if (!pstParams->resume_mode)	//normal init
			{
				/* init input csc value(bypass) */
				p_in_csc->win_id = LX_PE_WIN_0;
				p_in_csc->enable = 1;
				memcpy(p_in_csc->matrix, _g_pe_csc_hw_h15_in_mtrx_byps, sizeof(UINT16)*PE_NUM_OF_CSC_COEF);
				memcpy(p_in_csc->offset, _g_pe_csc_hw_h15_in_ofst_byps, sizeof(UINT16)*PE_NUM_OF_CSC_OFST);
				/* init cur_post_type */
				_g_pe_csc_hw_h15_info.cur_post_type = LX_PE_POST_CSC_Y601_LINEAR;
			}
			else	//resume init
			{
				/* set input csc from prev one */
				p_in_csc->win_id = LX_PE_WIN_0;
				ret = PE_FWI_H15_SetCviCsc(p_in_csc);
				PE_CSC_HW_H15_CHECK_CODE(ret, break, \
					"[%s,%d] PE_FWI_H15_SetCviCsc() error.\n",__F__,__L__);
				/* if resume_mode, keep cur_post_type */
			}
		}
		else
		{
			PE_CSC_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * debug setting
 *
 * @param   *pstParams [in] LX_PE_DBG_SETTINGS_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_CSC_HW_H15_SetDebugSettings(LX_PE_DBG_SETTINGS_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		PE_CSC_HW_H15_DBG_PRINT("wid:%d,type:0x%x,[0x%x]print:0x%x,bypass:0x%x\n",\
			pstParams->win_id,pstParams->type,(0x1<<PE_ITEM_PKTMODL(CSC)),\
			pstParams->print_lvl,pstParams->bypass);
		/* set print level */
		if(pstParams->type&LX_PE_DBG_LV)
		{
			_g_csc_hw_h15_trace = (pstParams->print_lvl & (0x1<<PE_ITEM_PKTMODL(CSC)))? 0x1:0x0;
		}
		/* set bypass */
		if(pstParams->type&LX_PE_DBG_BY)
		{
			if(PE_KDRV_VER_H15)
			{
				PE_CSC_HW_H15_DBG_PRINT("not supported.\n");
			}
			else
			{
				PE_CSC_HW_H15_DBG_PRINT("do nothing.\n");	ret = RET_OK;
			}
		}
	}while(0);
	return ret;
}
/**
 * set color gamut
 *
 * @param   *pstParams [in] LX_PE_CSC_GAMUT_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_CSC_HW_H15_SetColorGamut(LX_PE_CSC_GAMUT_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		PE_CSC_HW_H15_DBG_PRINT("set pstParams[%d] : 0x%x,0x%x,0x%x, 0x%x,0x%x,0x%x, 0x%x,0x%x,0x%x\n",\
			pstParams->win_id,pstParams->matrix[0],pstParams->matrix[1],pstParams->matrix[2],\
			pstParams->matrix[3],pstParams->matrix[4],pstParams->matrix[5],pstParams->matrix[6],\
			pstParams->matrix[7],pstParams->matrix[8]);
		if(PE_KDRV_VER_H15)
		{
			PE_CSC_HW_H15_DBG_PRINT("not supported.\n");
		}
		else
		{
			PE_CSC_HW_H15_DBG_PRINT("nothing to do\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * get color gamut
 *
 * @param   *pstParams [in/out] LX_PE_CSC_GAMUT_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_CSC_HW_H15_GetColorGamut(LX_PE_CSC_GAMUT_T *pstParams)
{
	int ret = RET_OK;
	LX_PE_WIN_ID win_id;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		win_id = PE_GET_CHECKED_WINID(pstParams->win_id);
		if(PE_KDRV_VER_H15)
		{
			PE_CSC_HW_H15_DBG_PRINT("not supported.\n");
		}
		else
		{
			PE_CSC_HW_H15_DBG_PRINT("nothing to do\n");	ret = RET_OK;
		}
		PE_CSC_HW_H15_DBG_PRINT("get pstParams[%d] : 0x%x,0x%x,0x%x, 0x%x,0x%x,0x%x, 0x%x,0x%x,0x%x\n",\
			pstParams->win_id,pstParams->matrix[0],pstParams->matrix[1],pstParams->matrix[2],pstParams->matrix[3],\
			pstParams->matrix[4],pstParams->matrix[5],pstParams->matrix[6],pstParams->matrix[7],pstParams->matrix[8]);
	}while(0);
	return ret;
}
/**
 * set post csc
 *
 * @param   *pstParams [in] LX_PE_CSC_POST_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_CSC_HW_H15_SetPostCsc(LX_PE_CSC_POST_T *pstParams)
{
	int ret = RET_OK;
	PE_CSC_HW_H15_SETTINGS_T *pInfo=&_g_pe_csc_hw_h15_info;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		PE_CSC_HW_H15_DBG_PRINT("set pstParams[%d] : type:%d\n"\
			"coef:%x,%x,%x, %x,%x,%x, %x,%x,%x, ofs:%x,%x,%x, %x,%x,%x\n",\
			pstParams->win_id, pstParams->type,\
			pstParams->matrix[0],pstParams->matrix[1],pstParams->matrix[2],\
			pstParams->matrix[3],pstParams->matrix[4],pstParams->matrix[5],pstParams->matrix[6],\
			pstParams->matrix[7],pstParams->matrix[8],pstParams->offset[0],pstParams->offset[1],\
			pstParams->offset[2],pstParams->offset[3],pstParams->offset[4],pstParams->offset[5]);
		if(PE_KDRV_VER_H15)
		{
			if(PE_CHECK_WIN0(pstParams->win_id))
			{
				PE_CSC_HW_H15_DBG_PRINT("set type:%d -> %d\n", pInfo->cur_post_type, pstParams->type);
				pInfo->cur_post_type = pstParams->type;
			}
		}
		else
		{
			PE_CSC_HW_H15_DBG_PRINT("nothing to do\n");	ret = RET_OK;
		}
	}while(0);

	return ret;
}
/**
 * get post csc
 *
 * @param   *pstParams [in/out] LX_PE_CSC_POST_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_CSC_HW_H15_GetPostCsc(LX_PE_CSC_POST_T *pstParams)
{
	int ret = RET_OK;
	LX_PE_WIN_ID win_id;
	PE_CSC_HW_H15_SETTINGS_T *pInfo = &_g_pe_csc_hw_h15_info;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		win_id = PE_GET_CHECKED_WINID(pstParams->win_id);
		if(PE_KDRV_VER_H15)
		{
			if(PE_CHECK_WIN0(win_id))
			{
				pstParams->type = pInfo->pre_post_type;
				PE_CE_PE1_H15A0_QRd02(pe1_bt20_ctrl_0d, r_1x3_coef0, pstParams->matrix[0],\
	                                                    r_1x3_coef1, pstParams->matrix[1]);
				PE_CE_PE1_H15A0_QRd02(pe1_bt20_ctrl_0e, r_1x3_coef2, pstParams->matrix[2],\
	                                                    r_1x3_coef3, pstParams->matrix[3]);
				PE_CE_PE1_H15A0_QRd02(pe1_bt20_ctrl_0f, r_1x3_coef4, pstParams->matrix[4],\
	                                                    r_1x3_coef5, pstParams->matrix[5]);
				PE_CE_PE1_H15A0_QRd02(pe1_bt20_ctrl_10, r_1x3_coef6, pstParams->matrix[6],\
	                                                    r_1x3_coef7, pstParams->matrix[7]);
				PE_CE_PE1_H15A0_QRd01(pe1_bt20_ctrl_11, r_1x3_coef8, pstParams->matrix[8]);
				PE_CE_PE1_H15A0_QRd02(pe1_bt20_ctrl_12, r_1x3_ofst0, pstParams->offset[0],\
	                                                    r_1x3_ofst1, pstParams->offset[1]);
				PE_CE_PE1_H15A0_QRd02(pe1_bt20_ctrl_13, r_1x3_ofst2, pstParams->offset[2],\
	                                                    r_1x3_ofst3, pstParams->offset[3]);
				PE_CE_PE1_H15A0_QRd02(pe1_bt20_ctrl_14, r_1x3_ofst4, pstParams->offset[4],\
	                                                    r_1x3_ofst5, pstParams->offset[5]);
				PE_CE_PE1_H15A0_QRd02(pe1_bt20_ctrl_00, r_2x3_coef0, pstParams->coeff0[0],\
	                                                    r_2x3_coef1, pstParams->coeff0[1]);
				PE_CE_PE1_H15A0_QRd02(pe1_bt20_ctrl_01, r_2x3_coef2, pstParams->coeff0[2],\
	                                                    r_2x3_coef3a,pstParams->coeff0[3]);
				PE_CE_PE1_H15A0_QRd02(pe1_bt20_ctrl_02, r_2x3_coef4a,pstParams->coeff0[4],\
	                                                    r_2x3_coef5a,pstParams->coeff0[5]);
				PE_CE_PE1_H15A0_QRd02(pe1_bt20_ctrl_03, r_2x3_coef6a,pstParams->coeff0[6],\
	                                                    r_2x3_coef7a,pstParams->coeff0[7]);
				PE_CE_PE1_H15A0_QRd02(pe1_bt20_ctrl_04, r_2x3_coef8a,pstParams->coeff0[8],\
	                                                    r_2x3_ofst0, pstParams->offst0[0]);
				PE_CE_PE1_H15A0_QRd02(pe1_bt20_ctrl_05, r_2x3_ofst1, pstParams->offst0[1],\
	                                                    r_2x3_ofst2, pstParams->offst0[2]);
				PE_CE_PE1_H15A0_QRd02(pe1_bt20_ctrl_06, r_2x3_ofst3, pstParams->offst0[3],\
	                                                    r_2x3_ofst4, pstParams->offst0[4]);
				PE_CE_PE1_H15A0_QRd01(pe1_bt20_ctrl_07, r_2x3_ofst5, pstParams->offst0[5]);
				PE_CE_PE1_H15A0_QRd01(pe1_bt20_ctrl_09, r_2x3_coef3b,pstParams->coeff1[3]);
				PE_CE_PE1_H15A0_QRd02(pe1_bt20_ctrl_0a, r_2x3_coef4b,pstParams->coeff1[4],\
	                                                    r_2x3_coef5b,pstParams->coeff1[5]);
				PE_CE_PE1_H15A0_QRd02(pe1_bt20_ctrl_0b, r_2x3_coef6b,pstParams->coeff1[6],\
	                                                    r_2x3_coef7b,pstParams->coeff1[7]);
				PE_CE_PE1_H15A0_QRd01(pe1_bt20_ctrl_0c, r_2x3_coef8b,pstParams->coeff1[8]);
				pstParams->coeff1[0] = pstParams->coeff0[0];
				pstParams->coeff1[1] = pstParams->coeff0[1];
				pstParams->coeff1[2] = pstParams->coeff0[2];
			}
			if(PE_CHECK_WIN1(win_id))
			{
				pstParams->type = pInfo->pre_post_type;
				PE_CO_PE1_H15A0_QRd02(pe1_bt20_ctrl_0d, r_1x3_coef0, pstParams->matrix[0],\
	                                                    r_1x3_coef1, pstParams->matrix[1]);
				PE_CO_PE1_H15A0_QRd02(pe1_bt20_ctrl_0e, r_1x3_coef2, pstParams->matrix[2],\
	                                                    r_1x3_coef3, pstParams->matrix[3]);
				PE_CO_PE1_H15A0_QRd02(pe1_bt20_ctrl_0f, r_1x3_coef4, pstParams->matrix[4],\
	                                                    r_1x3_coef5, pstParams->matrix[5]);
				PE_CO_PE1_H15A0_QRd02(pe1_bt20_ctrl_10, r_1x3_coef6, pstParams->matrix[6],\
	                                                    r_1x3_coef7, pstParams->matrix[7]);
				PE_CO_PE1_H15A0_QRd01(pe1_bt20_ctrl_11, r_1x3_coef8, pstParams->matrix[8]);
				PE_CO_PE1_H15A0_QRd02(pe1_bt20_ctrl_12, r_1x3_ofst0, pstParams->offset[0],\
	                                                    r_1x3_ofst1, pstParams->offset[1]);
				PE_CO_PE1_H15A0_QRd02(pe1_bt20_ctrl_13, r_1x3_ofst2, pstParams->offset[2],\
	                                                    r_1x3_ofst3, pstParams->offset[3]);
				PE_CO_PE1_H15A0_QRd02(pe1_bt20_ctrl_14, r_1x3_ofst4, pstParams->offset[4],\
	                                                    r_1x3_ofst5, pstParams->offset[5]);
				PE_CO_PE1_H15A0_QRd02(pe1_bt20_ctrl_00, r_2x3_coef0, pstParams->coeff0[0],\
	                                                    r_2x3_coef1, pstParams->coeff0[1]);
				PE_CO_PE1_H15A0_QRd02(pe1_bt20_ctrl_01, r_2x3_coef2, pstParams->coeff0[2],\
	                                                    r_2x3_coef3a,pstParams->coeff0[3]);
				PE_CO_PE1_H15A0_QRd02(pe1_bt20_ctrl_02, r_2x3_coef4a,pstParams->coeff0[4],\
	                                                    r_2x3_coef5a,pstParams->coeff0[5]);
				PE_CO_PE1_H15A0_QRd02(pe1_bt20_ctrl_03, r_2x3_coef6a,pstParams->coeff0[6],\
	                                                    r_2x3_coef7a,pstParams->coeff0[7]);
				PE_CO_PE1_H15A0_QRd02(pe1_bt20_ctrl_04, r_2x3_coef8a,pstParams->coeff0[8],\
	                                                    r_2x3_ofst0, pstParams->offst0[0]);
				PE_CO_PE1_H15A0_QRd02(pe1_bt20_ctrl_05, r_2x3_ofst1, pstParams->offst0[1],\
	                                                    r_2x3_ofst2, pstParams->offst0[2]);
				PE_CO_PE1_H15A0_QRd02(pe1_bt20_ctrl_06, r_2x3_ofst3, pstParams->offst0[3],\
	                                                    r_2x3_ofst4, pstParams->offst0[4]);
				PE_CO_PE1_H15A0_QRd01(pe1_bt20_ctrl_07, r_2x3_ofst5, pstParams->offst0[5]);
				PE_CO_PE1_H15A0_QRd01(pe1_bt20_ctrl_09, r_2x3_coef3b,pstParams->coeff1[3]);
				PE_CO_PE1_H15A0_QRd02(pe1_bt20_ctrl_0a, r_2x3_coef4b,pstParams->coeff1[4],\
	                                                    r_2x3_coef5b,pstParams->coeff1[5]);
				PE_CO_PE1_H15A0_QRd02(pe1_bt20_ctrl_0b, r_2x3_coef6b,pstParams->coeff1[6],\
	                                                    r_2x3_coef7b,pstParams->coeff1[7]);
				PE_CO_PE1_H15A0_QRd01(pe1_bt20_ctrl_0c, r_2x3_coef8b,pstParams->coeff1[8]);
				pstParams->coeff1[0] = pstParams->coeff0[0];
				pstParams->coeff1[1] = pstParams->coeff0[1];
				pstParams->coeff1[2] = pstParams->coeff0[2];
			}
		}
		else
		{
			PE_CSC_HW_H15_DBG_PRINT("nothing to do\n");	ret = RET_OK;
		}
		PE_CSC_HW_H15_DBG_PRINT("get pstParams[%d] : type:%d\n"\
			"matrix:%x,%x,%x, %x,%x,%x, %x,%x,%x\n"\
			"offset:%x,%x,%x, %x,%x,%x\n"\
			"coeff0:%x,%x,%x, %x,%x,%x, %x,%x,%x\n"\
			"coeff1:%x,%x,%x, %x,%x,%x, %x,%x,%x\n"\
			"offst0:%x,%x,%x, %x,%x,%x\n",\
			pstParams->win_id,pstParams->type,\
			pstParams->matrix[0],pstParams->matrix[1],pstParams->matrix[2],\
			pstParams->matrix[3],pstParams->matrix[4],pstParams->matrix[5],\
			pstParams->matrix[6],pstParams->matrix[7],pstParams->matrix[8],\
			pstParams->offset[0],pstParams->offset[1],pstParams->offset[2],\
			pstParams->offset[3],pstParams->offset[4],pstParams->offset[5],\
			pstParams->coeff0[0],pstParams->coeff0[1],pstParams->coeff0[2],\
			pstParams->coeff0[3],pstParams->coeff0[4],pstParams->coeff0[5],\
			pstParams->coeff0[6],pstParams->coeff0[7],pstParams->coeff0[8],\
			pstParams->coeff1[0],pstParams->coeff1[1],pstParams->coeff1[2],\
			pstParams->coeff1[3],pstParams->coeff1[4],pstParams->coeff1[5],\
			pstParams->coeff1[6],pstParams->coeff1[7],pstParams->coeff1[8],\
			pstParams->offst0[0],pstParams->offst0[1],pstParams->offst0[2],\
			pstParams->offst0[3],pstParams->offst0[4],pstParams->offst0[5]);
	}while(0);
	return ret;
}
/**
 * set input csc
 *
 * @param   *pstParams [in] LX_PE_CSC_INPUT_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_CSC_HW_H15_SetInputCsc(LX_PE_CSC_INPUT_T *pstParams)
{
	int ret = RET_OK;
	LX_PE_WIN_ID win_id;
	PE_CSC_HW_H15_SETTINGS_T *pInfo=&_g_pe_csc_hw_h15_info;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		PE_CSC_HW_H15_DBG_PRINT("set pstParams[%d] : en:%d\n"\
			"coef:%x,%x,%x, %x,%x,%x, %x,%x,%x, ofs:%x,%x,%x, %x,%x,%x\n",\
			pstParams->win_id,pstParams->enable,\
			pstParams->matrix[0],pstParams->matrix[1],pstParams->matrix[2],\
			pstParams->matrix[3],pstParams->matrix[4],pstParams->matrix[5],\
			pstParams->matrix[6],pstParams->matrix[7],pstParams->matrix[8],\
			pstParams->offset[0],pstParams->offset[1],pstParams->offset[2],\
			pstParams->offset[3],pstParams->offset[4],pstParams->offset[5]);
		if(PE_KDRV_VER_H15)
		{
			win_id = pstParams->win_id;
			if(PE_CHECK_WIN0(win_id))
			{
				pstParams->win_id = LX_PE_WIN_0;
				ret = PE_FWI_H15_SetCviCsc(pstParams);
				PE_CSC_HW_H15_CHECK_CODE(ret,break,"[%s,%d] PE_FWI_H15_SetCviCsc() error.\n",__F__,__L__);
				memcpy(&(pInfo->pre_in_csc), pstParams, sizeof(LX_PE_CSC_INPUT_T));
			}
			if(PE_CHECK_WIN1(win_id))
			{
				pstParams->win_id = LX_PE_WIN_1;
				ret = PE_FWI_H15_SetCviCsc(pstParams);
				PE_CSC_HW_H15_CHECK_CODE(ret,break,"[%s,%d] PE_FWI_H15_SetCviCsc() error.\n",__F__,__L__);
			}
		}
		else
		{
			PE_CSC_HW_H15_DBG_PRINT("nothing to do\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * get input csc
 *
 * @param   *pstParams [in/out] LX_PE_CSC_INPUT_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_CSC_HW_H15_GetInputCsc(LX_PE_CSC_INPUT_T *pstParams)
{
	int ret = RET_OK;
	LX_PE_WIN_ID win_id;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		win_id = PE_GET_CHECKED_WINID(pstParams->win_id);
		if(PE_KDRV_VER_H15)
		{
			if(PE_CHECK_WIN0(win_id))
			{
				PE_CVI_A_H15A0_QRd02(csc_ctrl0, csc_coefficient0,	pstParams->matrix[0],\
                                                csc_coefficient1,	pstParams->matrix[1]);
				PE_CVI_A_H15A0_QRd02(csc_ctrl1, csc_coefficient2,	pstParams->matrix[2],\
                                                csc_coefficient3,	pstParams->matrix[3]);
				PE_CVI_A_H15A0_QRd02(csc_ctrl2, csc_coefficient4,	pstParams->matrix[4],\
                                                csc_coefficient5,	pstParams->matrix[5]);
				PE_CVI_A_H15A0_QRd02(csc_ctrl3, csc_coefficient6,	pstParams->matrix[6],\
                                                csc_coefficient7,	pstParams->matrix[7]);
				PE_CVI_A_H15A0_QRd02(csc_ctrl4, csc_coefficient8,	pstParams->matrix[8],\
                                                csc_offset0,		pstParams->offset[0]);
				PE_CVI_A_H15A0_QRd02(csc_ctrl5, csc_offset1,		pstParams->offset[1],\
                                                csc_offset2,		pstParams->offset[2]);
				PE_CVI_A_H15A0_QRd02(csc_ctrl6, csc_offset3,		pstParams->offset[3],\
                                                csc_offset4,		pstParams->offset[4]);
				PE_CVI_A_H15A0_QRd02(csc_ctrl7, csc_offset5,		pstParams->offset[5],\
                                                csc_en,				pstParams->enable);
			}
			if(PE_CHECK_WIN1(win_id))
			{
				PE_CVI_B_H15A0_QRd02(csc_ctrl0, csc_coefficient0,	pstParams->matrix[0],\
                                                csc_coefficient1,	pstParams->matrix[1]);
				PE_CVI_B_H15A0_QRd02(csc_ctrl1, csc_coefficient2,	pstParams->matrix[2],\
                                                csc_coefficient3,	pstParams->matrix[3]);
				PE_CVI_B_H15A0_QRd02(csc_ctrl2, csc_coefficient4,	pstParams->matrix[4],\
                                                csc_coefficient5,	pstParams->matrix[5]);
				PE_CVI_B_H15A0_QRd02(csc_ctrl3, csc_coefficient6,	pstParams->matrix[6],\
                                                csc_coefficient7,	pstParams->matrix[7]);
				PE_CVI_B_H15A0_QRd02(csc_ctrl4, csc_coefficient8,	pstParams->matrix[8],\
                                                csc_offset0,		pstParams->offset[0]);
				PE_CVI_B_H15A0_QRd02(csc_ctrl5, csc_offset1,		pstParams->offset[1],\
                                                csc_offset2,		pstParams->offset[2]);
				PE_CVI_B_H15A0_QRd02(csc_ctrl6, csc_offset3,		pstParams->offset[3],\
                                                csc_offset4,		pstParams->offset[4]);
				PE_CVI_B_H15A0_QRd02(csc_ctrl7, csc_offset5,		pstParams->offset[5],\
                                                csc_en,				pstParams->enable);
			}
		}
		else
		{
			PE_CSC_HW_H15_DBG_PRINT("nothing to do\n");	ret = RET_OK;
		}
		PE_CSC_HW_H15_DBG_PRINT("get pstParams[%d] : en:%d\n"\
			"coef:%x,%x,%x, %x,%x,%x, %x,%x,%x, ofs:%x,%x,%x, %x,%x,%x\n",\
			pstParams->win_id,pstParams->enable,\
			pstParams->matrix[0],pstParams->matrix[1],pstParams->matrix[2],\
			pstParams->matrix[3],pstParams->matrix[4],pstParams->matrix[5],\
			pstParams->matrix[6],pstParams->matrix[7],pstParams->matrix[8],\
			pstParams->offset[0],pstParams->offset[1],pstParams->offset[2],\
			pstParams->offset[3],pstParams->offset[4],pstParams->offset[5]);
	}while(0);
	return ret;
}
/**
 * set xvYcc
 *
 * @param   *pstParams [in] LX_PE_CSC_XVYCC_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_CSC_HW_H15_SetxvYCC(LX_PE_CSC_XVYCC_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		PE_CSC_HW_H15_DBG_PRINT("set pstParams[%d] : en:%d, sclr:0x%x\n",\
			pstParams->win_id,pstParams->enable,pstParams->scaler);
		if(PE_KDRV_VER_H15)
		{
			PE_CSC_HW_H15_DBG_PRINT("not supported.\n");
		}
		else
		{
			PE_CSC_HW_H15_DBG_PRINT("nothing to do\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * get xvYcc
 *
 * @param   *pstParams [in/out] LX_PE_CSC_XVYCC_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_CSC_HW_H15_GetxvYCC(LX_PE_CSC_XVYCC_T *pstParams)
{
	int ret = RET_OK;
	LX_PE_WIN_ID win_id;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		win_id = PE_GET_CHECKED_WINID(pstParams->win_id);
		if(PE_KDRV_VER_H15)
		{
			PE_CSC_HW_H15_DBG_PRINT("not supported.\n");
		}
		else
		{
			PE_CSC_HW_H15_DBG_PRINT("nothing to do\n");	ret = RET_OK;
		}
		PE_CSC_HW_H15_DBG_PRINT("get pstParams[%d] : en:%d, sclr:0x%x\n",\
			pstParams->win_id,pstParams->enable,pstParams->scaler);
	}while(0);
	return ret;
}
/**
 * set csc mode
 *
 * @param   *pCfg [in] PE_TSK_H15_CFG_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_CSC_HW_H15_SetCscMode(PE_TSK_H15_CFG_T *pCfg)
{
    int ret = RET_OK;
	do{
		CHECK_KNULL(pCfg);
		if(PE_KDRV_VER_H15)
		{
			ret = PE_CSC_HW_H15_SetPostCscMode(pCfg);
			PE_CSC_HW_H15_CHECK_CODE(ret, break, \
				"[%s,%d] PE_CSC_HW_H15_SetPostCscMode() error.\n",__F__,__L__);
		}
		else
		{
			PE_CSC_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * set tnr lb ctrl
 *
 * @param   *pCfg [in] PE_TSK_H15_CFG_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_CSC_HW_H15_SetPostCscMode(PE_TSK_H15_CFG_T *pCfg)
{
	int ret = RET_OK;
	static UINT32 count = 0;
	UINT32 cur_post_type;
	PE_TSK_H15_OPR_T *p_opr = &(pCfg->opr);
	LX_PE_INF_DISPLAY_T *disp0_info = &pCfg->inf.disp_info[LX_PE_WIN_0];
	PE_CSC_HW_H15_SETTINGS_T *pInfo=&_g_pe_csc_hw_h15_info;
	do{
		CHECK_KNULL(pCfg);
		if(PE_KDRV_VER_H15)
		{
			/* checking post type */
			if (p_opr->pause_task == PE_TSK_H15_RUN_STOP_ALGO)
			{
				cur_post_type = pInfo->cur_post_type;
			}
			else if (disp0_info->src_type==LX_PE_SRC_DTV && \
				(disp0_info->dtv_type==LX_PE_DTV_NORMAL || \
				(disp0_info->dtv_type==LX_PE_DTV_HDDPLAY && \
				disp0_info->hdd_type==LX_PE_HDD_SRC_DTV)))
			{
				if (disp0_info->in_h_size>=PE_CSC_HW_H15_BT709_H_SIZE)
				{
					cur_post_type = LX_PE_POST_CSC_Y709_EXPEND;
				}
				else
				{
					cur_post_type = LX_PE_POST_CSC_Y601_EXPEND;
				}
			}
			else
			{
				cur_post_type = pInfo->cur_post_type;
			}
			if (pInfo->pre_post_type != cur_post_type)
			{
				PE_CSC_HW_H15_DBG_PRINT("[inf] src,dtv,hdd:%d,%d,%d, h:%d, ext:%d\n",\
					disp0_info->src_type, disp0_info->dtv_type, disp0_info->hdd_type, \
					disp0_info->in_h_size, disp0_info->mode.is_ext_frc);
				PE_CSC_HW_H15_DBG_PRINT("[set] user:%d, post_type:%d -> %d\n",\
					pInfo->cur_post_type, pInfo->pre_post_type, cur_post_type);
				ret = PE_CSC_HW_H15_SetPostCscReg(cur_post_type);
				PE_CSC_HW_H15_CHECK_CODE(ret, break, \
					"[%s,%d] PE_CSC_HW_H15_SetPostCscReg() error.\n",__F__,__L__);
				pInfo->pre_post_type = cur_post_type;
			}
			/* **********************************************************/
			if(_g_csc_hw_h15_trace)
			{
				if(count>PE_PRINT_COUNT_NUM)
				{
					PE_CSC_HW_H15_DBG_PRINT("[state] src,dtv,hdd:%d,%d,%d, h:%d, ext:%d\n",\
						disp0_info->src_type, disp0_info->dtv_type, disp0_info->hdd_type, \
						disp0_info->in_h_size, disp0_info->mode.is_ext_frc);
					PE_CSC_HW_H15_DBG_PRINT("[state] user:%d, post_type:%d\n",\
						pInfo->cur_post_type, cur_post_type);
					count=0;
				}
				count++;
			}
		}
		else
		{
			PE_CSC_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * set post csc register
 *
 * @param   *pstParams [in] LX_PE_CSC_POST_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_CSC_HW_H15_SetPostCscReg(UINT32 type)
{
	int ret = RET_OK;
	const UINT16 *p_1x3_co, *p_1x3_of, *p_2x3a_co, *p_2x3b_co, *p_2x3_ofs;
	UINT32 r_2x3_en, r_dgm_en, r_dth_en;
	do{
		if(PE_KDRV_VER_H15)
		{
			/* cur_ctrl_type */
			switch (type)
			{
				case LX_PE_POST_CSC_2020_CL_EXP:
				case LX_PE_POST_CSC_2020_CL_LIN:
					r_2x3_en = r_dgm_en = r_dth_en = 1;
					break;
				case LX_PE_POST_CSC_BYPASS:
				case LX_PE_POST_CSC_Y709_EXPEND:
				case LX_PE_POST_CSC_Y709_LINEAR:
				case LX_PE_POST_CSC_Y601_EXPEND:
				case LX_PE_POST_CSC_Y601_LINEAR:
				case LX_PE_POST_CSC_2020_NCL_EX:
				case LX_PE_POST_CSC_2020_NCL_LI:
				default:
					r_2x3_en = r_dgm_en = r_dth_en = 0;
					break;
			}
			/* cur_1x3_type */
			switch (type)
			{
				case LX_PE_POST_CSC_BYPASS:
					p_1x3_co = _g_pe_csc_hw_h15_post_1x3_mtrx_bypass;
					p_1x3_of = _g_pe_csc_hw_h15_post_1x3_ofst_bypass;
					break;
				case LX_PE_POST_CSC_Y709_EXPEND:
					p_1x3_co = _g_pe_csc_hw_h15_post_1x3_y709_expand;
					p_1x3_of = _g_pe_csc_hw_h15_post_1x3_ofst_expand;
					break;
				case LX_PE_POST_CSC_Y709_LINEAR:
					p_1x3_co = _g_pe_csc_hw_h15_post_1x3_y709_linear;
					p_1x3_of = _g_pe_csc_hw_h15_post_1x3_ofst_linear;
					break;
				case LX_PE_POST_CSC_Y601_EXPEND:
					p_1x3_co = _g_pe_csc_hw_h15_post_1x3_y601_expand;
					p_1x3_of = _g_pe_csc_hw_h15_post_1x3_ofst_expand;
					break;
				case LX_PE_POST_CSC_Y601_LINEAR:
					p_1x3_co = _g_pe_csc_hw_h15_post_1x3_y601_linear;
					p_1x3_of = _g_pe_csc_hw_h15_post_1x3_ofst_linear;
					break;
				case LX_PE_POST_CSC_2020_NCL_EX:
					p_1x3_co = _g_pe_csc_hw_h15_post_1x3_ncl2020_exp;
					p_1x3_of = _g_pe_csc_hw_h15_post_1x3_ofst_expand;
					break;
				case LX_PE_POST_CSC_2020_NCL_LI:
					p_1x3_co = _g_pe_csc_hw_h15_post_1x3_ncl2020_lin;
					p_1x3_of = _g_pe_csc_hw_h15_post_1x3_ofst_linear;
					break;
				case LX_PE_POST_CSC_2020_CL_EXP:
					p_1x3_co = _g_pe_csc_hw_h15_post_1x3_cl_2020_exp;
					p_1x3_of = _g_pe_csc_hw_h15_post_1x3_ofst_bypass;
					break;
				case LX_PE_POST_CSC_2020_CL_LIN:
					p_1x3_co = _g_pe_csc_hw_h15_post_1x3_cl_2020_lin;
					p_1x3_of = _g_pe_csc_hw_h15_post_1x3_ofst_cl_lin;
					break;
				default:
					p_1x3_co = _g_pe_csc_hw_h15_post_1x3_y601_linear;
					p_1x3_of = _g_pe_csc_hw_h15_post_1x3_ofst_linear;
					break;
			}
			/* cur_2x3_type */
			switch (type)
			{
				case LX_PE_POST_CSC_2020_CL_EXP:
					p_2x3a_co = _g_pe_csc_hw_h15_post_2x3a_cl2020_exp;
					p_2x3b_co = _g_pe_csc_hw_h15_post_2x3b_cl2020_exp;
					p_2x3_ofs = _g_pe_csc_hw_h15_post_2x3_ofs_2020_ex;
					break;
				case LX_PE_POST_CSC_2020_CL_LIN:
				default:
					p_2x3a_co = _g_pe_csc_hw_h15_post_2x3a_cl2020_lin;
					p_2x3b_co = _g_pe_csc_hw_h15_post_2x3b_cl2020_lin;
					p_2x3_ofs = _g_pe_csc_hw_h15_post_2x3_ofs_2020_li;
					break;
			}
			PE_CEO_PE1_H15A0_QWr02(pe1_bt20_ctrl_07,r_2x3_en,r_2x3_en,r_degamma_en,r_dgm_en);
			PE_CEO_PE1_H15A0_QWr01(pe1_bt20_ctrl_08,dither_en,r_dth_en);
			PE_CEO_PE1_H15A0_QWr02(pe1_bt20_ctrl_0d,r_1x3_coef0, GET_BITS(p_1x3_co[0],0,15),\
                                                    r_1x3_coef1, GET_BITS(p_1x3_co[1],0,15));
			PE_CEO_PE1_H15A0_QWr02(pe1_bt20_ctrl_0e,r_1x3_coef2, GET_BITS(p_1x3_co[2],0,15),\
                                                    r_1x3_coef3, GET_BITS(p_1x3_co[3],0,15));
			PE_CEO_PE1_H15A0_QWr02(pe1_bt20_ctrl_0f,r_1x3_coef4, GET_BITS(p_1x3_co[4],0,15),\
                                                    r_1x3_coef5, GET_BITS(p_1x3_co[5],0,15));
			PE_CEO_PE1_H15A0_QWr02(pe1_bt20_ctrl_10,r_1x3_coef6, GET_BITS(p_1x3_co[6],0,15),\
                                                    r_1x3_coef7, GET_BITS(p_1x3_co[7],0,15));
			PE_CEO_PE1_H15A0_QWr01(pe1_bt20_ctrl_11,r_1x3_coef8, GET_BITS(p_1x3_co[8],0,15));
			PE_CEO_PE1_H15A0_QWr02(pe1_bt20_ctrl_12,r_1x3_ofst0, GET_BITS(p_1x3_of[0],0,11),\
                                                    r_1x3_ofst1, GET_BITS(p_1x3_of[1],0,11));
			PE_CEO_PE1_H15A0_QWr02(pe1_bt20_ctrl_13,r_1x3_ofst2, GET_BITS(p_1x3_of[2],0,11),\
                                                    r_1x3_ofst3, GET_BITS(p_1x3_of[3],0,11));
			PE_CEO_PE1_H15A0_QWr02(pe1_bt20_ctrl_14,r_1x3_ofst4, GET_BITS(p_1x3_of[4],0,11),\
                                                    r_1x3_ofst5, GET_BITS(p_1x3_of[5],0,11));
			PE_CEO_PE1_H15A0_QWr02(pe1_bt20_ctrl_00,r_2x3_coef0, GET_BITS(p_2x3a_co[0],0,15),\
                                                    r_2x3_coef1, GET_BITS(p_2x3a_co[1],0,15));
			PE_CEO_PE1_H15A0_QWr02(pe1_bt20_ctrl_01,r_2x3_coef2, GET_BITS(p_2x3a_co[2],0,15),\
                                                    r_2x3_coef3a,GET_BITS(p_2x3a_co[3],0,15));
			PE_CEO_PE1_H15A0_QWr02(pe1_bt20_ctrl_02,r_2x3_coef4a,GET_BITS(p_2x3a_co[4],0,15),\
                                                    r_2x3_coef5a,GET_BITS(p_2x3a_co[5],0,15));
			PE_CEO_PE1_H15A0_QWr02(pe1_bt20_ctrl_03,r_2x3_coef6a,GET_BITS(p_2x3a_co[6],0,15),\
                                                    r_2x3_coef7a,GET_BITS(p_2x3a_co[7],0,15));
			PE_CEO_PE1_H15A0_QWr02(pe1_bt20_ctrl_04,r_2x3_coef8a,GET_BITS(p_2x3a_co[8],0,15),\
                                                    r_2x3_ofst0, GET_BITS(p_2x3_ofs[0],0,11));
			PE_CEO_PE1_H15A0_QWr02(pe1_bt20_ctrl_05,r_2x3_ofst1, GET_BITS(p_2x3_ofs[1],0,11),\
                                                    r_2x3_ofst2, GET_BITS(p_2x3_ofs[2],0,11));
			PE_CEO_PE1_H15A0_QWr02(pe1_bt20_ctrl_06,r_2x3_ofst3, GET_BITS(p_2x3_ofs[3],0,11),\
                                                    r_2x3_ofst4, GET_BITS(p_2x3_ofs[4],0,11));
			PE_CEO_PE1_H15A0_QWr01(pe1_bt20_ctrl_07,r_2x3_ofst5, GET_BITS(p_2x3_ofs[5],0,11));
			PE_CEO_PE1_H15A0_QWr01(pe1_bt20_ctrl_09,r_2x3_coef3b,GET_BITS(p_2x3b_co[3],0,15));
			PE_CEO_PE1_H15A0_QWr02(pe1_bt20_ctrl_0a,r_2x3_coef4b,GET_BITS(p_2x3b_co[4],0,15),\
                                                    r_2x3_coef5b,GET_BITS(p_2x3b_co[5],0,15));
			PE_CEO_PE1_H15A0_QWr02(pe1_bt20_ctrl_0b,r_2x3_coef6b,GET_BITS(p_2x3b_co[6],0,15),\
                                                    r_2x3_coef7b,GET_BITS(p_2x3b_co[7],0,15));
			PE_CEO_PE1_H15A0_QWr01(pe1_bt20_ctrl_0c,r_2x3_coef8b,GET_BITS(p_2x3b_co[8],0,15));
			PE_CSC_HW_H15_DBG_PRINT("[set] post type:%d, 2x3,dgm,dth_en:%d,%d,%d\n", \
				type, r_2x3_en, r_dgm_en, r_dth_en);
			PE_CSC_HW_H15_DBG_PRINT("[set] 1x3  co:%x,%x,%x,%x,%x,%x,%x,%x,%x\n", \
				p_1x3_co[0], p_1x3_co[1], p_1x3_co[2], \
				p_1x3_co[3], p_1x3_co[4], p_1x3_co[5], \
				p_1x3_co[6], p_1x3_co[7], p_1x3_co[8]);
			PE_CSC_HW_H15_DBG_PRINT("[set] 1x3  of:%x,%x,%x,%x,%x,%x\n", \
				p_1x3_of[0], p_1x3_of[1], p_1x3_of[2], \
				p_1x3_of[3], p_1x3_of[4], p_1x3_of[5]);
			PE_CSC_HW_H15_DBG_PRINT("[set] 2x3a co:%x,%x,%x,%x,%x,%x,%x,%x,%x\n", \
				p_2x3a_co[0], p_2x3a_co[1], p_2x3a_co[2], \
				p_2x3a_co[3], p_2x3a_co[4], p_2x3a_co[5], \
				p_2x3a_co[6], p_2x3a_co[7], p_2x3a_co[8]);
			PE_CSC_HW_H15_DBG_PRINT("[set] 2x3b co:%x,%x,%x,%x,%x,%x,%x,%x,%x\n", \
				p_2x3b_co[0], p_2x3b_co[1], p_2x3b_co[2], \
				p_2x3b_co[3], p_2x3b_co[4], p_2x3b_co[5], \
				p_2x3b_co[6], p_2x3b_co[7], p_2x3b_co[8]);
			PE_CSC_HW_H15_DBG_PRINT("[set] 2x3  of:%x,%x,%x,%x,%x,%x\n", \
				p_2x3_ofs[0], p_2x3_ofs[1], p_2x3_ofs[2], \
				p_2x3_ofs[3], p_2x3_ofs[4], p_2x3_ofs[5]);
		}
		else
		{
			PE_CSC_HW_H15_DBG_PRINT("nothing to do\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}

