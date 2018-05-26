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

/** @file pe_csc_hw_m14.c
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

#include "pe_hw_m14.h"
#include "pe_reg_m14.h"
#include "pe_fwi_m14.h"
#include "pe_csc_hw_m14.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
#define PE_CSC_HW_M14_BT709_H_SIZE	1024

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define PE_CSC_HW_M14_ERROR	printk
#define PE_CSC_HW_M14_DBG_PRINT(fmt,args...)	\
	if(_g_csc_hw_m14_trace) printk("[%x,%x][%s,%d] "fmt,PE_CHIP_VER,g_pe_kdrv_ver_mask,__F__,__L__,##args)
#define PE_CSC_HW_M14_CHECK_CODE(_checker,_action,fmt,args...)	\
	if(_checker){PE_CSC_HW_M14_ERROR(fmt,##args);_action;}

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
static int PE_CSC_HW_M14_SetPostCscMode(PE_TSK_M14_CFG_T *pCfg);
static int PE_CSC_HW_M14_SetPostCscReg(UINT32 type);

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
static UINT32 _g_csc_hw_m14_trace=0x0;		//default should be off.
static PE_CSC_HW_M14_SETTINGS_T _g_pe_csc_hw_m14_info;

/* normal, ncl2020 ycbcr to gbr*/
const UINT16 _g_pe_csc_hw_m14_post_mtrx_bypass[PE_NUM_OF_CSC_COEF] = {
	0x1000, 0x0000, 0x0000,		//  1.000, 0.000, 0.000
	0x0000, 0x1000, 0x0000,		//  0.000, 1.000, 0.000
	0x0000, 0x0000, 0x1000};	//  0.000, 0.000, 1.000
const UINT16 _g_pe_csc_hw_m14_post_y709_expand[PE_NUM_OF_CSC_COEF] = {
	0x129F, 0xFC98, 0xF775,		//  1.164,-0.213,-0.534
	0x129F, 0x21D7, 0x0000,   	//  1.164, 2.115, 0.000
	0x129F, 0x0000, 0x1CB0};  	//  1.164, 0.000, 1.793
const UINT16 _g_pe_csc_hw_m14_post_y709_linear[PE_NUM_OF_CSC_COEF] = {
	0x1000, 0xFD13, 0xF8A8,		//  1.000,-0.183,-0.459
	0x1000, 0x1D0E, 0x0000,   	//  1.000, 1.816, 0.000
	0x1000, 0x0000, 0x18A3};  	//  1.000, 0.000, 1.540
const UINT16 _g_pe_csc_hw_m14_post_y601_expand[PE_NUM_OF_CSC_COEF] = {
	0x129F, 0xF9BF, 0xF2FE,		//  1.164,-0.391,-0.813
	0x129F, 0x2049, 0x0000,  	//  1.164, 2.018, 0.000
	0x129F, 0x0000, 0x1989};  	//  1.164, 0.000, 1.596
const UINT16 _g_pe_csc_hw_m14_post_y601_linear[PE_NUM_OF_CSC_COEF] = {
	0x1000, 0xFAA0, 0xF4D5,		//  1.000,-0.336,-0.698
	0x1000, 0x1BB6, 0x0000,   	//  1.000, 1.732, 0.000
	0x1000, 0x0000, 0x15EF};  	//  1.000, 0.000, 1.371
const UINT16 _g_pe_csc_hw_m14_post_ncl2020_exp[PE_NUM_OF_CSC_COEF] = {
	0x12AF, 0xFCFE, 0xF590,		//  1.168,-0.188,-0.652
	0x12AF, 0x225F, 0x0000,		//  1.168, 2.148, 0.000
	0x12AF, 0x0000, 0x1AF0};	//  1.168, 0.000, 1.684
const UINT16 _g_pe_csc_hw_m14_post_ncl2020_lin[PE_NUM_OF_CSC_COEF] = {
	0x1000, 0xFD6D, 0xF710,		//  1.000,-0.161,-0.559
	0x1000, 0x1D6E, 0x0000,		//  1.000, 1.839, 0.000
	0x1000, 0x0000, 0x1711};	//  1.000, 0.000, 1.442
const UINT16 _g_pe_csc_hw_m14_post_ofst_bypass[PE_NUM_OF_CSC_OFST] = {
	0x0000, 0x0000, 0x0000,		//   0,   0,   0
	0x0000, 0x0000, 0x0000};	//   0,   0,   0
const UINT16 _g_pe_csc_hw_m14_post_ofst_expand[PE_NUM_OF_CSC_OFST] = {
	0xFFC0, 0xFE00, 0xFE00,		// -64,-512,-512
	0x0000, 0x0000, 0x0000};	//   0,   0,   0
const UINT16 _g_pe_csc_hw_m14_post_ofst_linear[PE_NUM_OF_CSC_OFST] = {
	0x0000, 0xFE00, 0xFE00,		//   0,-512,-512
	0x0000, 0x0000, 0x0000};	//   0,   0,   0
const UINT16 _g_pe_csc_hw_m14_post_ofst_cl_lin[PE_NUM_OF_CSC_OFST] = {
	0x0000, 0x0000, 0x0000,		//   0,   0,   0
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
int PE_CSC_HW_M14_Init(PE_CFG_CTRL_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_M14BX)
		{
			PE_CSC_HW_M14_DBG_PRINT("init csc.\n");
			PE_PE1_M14B0_QWr01(pe1_ocsc_ctrl_07,	outputcsc_en,	0x1);
			_g_pe_csc_hw_m14_info.pre_post_type = LX_PE_POST_CSC_NUM;	//reset pre value
			_g_pe_csc_hw_m14_info.cur_post_type = LX_PE_POST_CSC_Y601_EXPEND;
		}
		else if(PE_KDRV_VER_M14AX)
		{
			PE_CSC_HW_M14_DBG_PRINT("init csc.\n");
			PE_P1L_M14A0_QWr05(pe1_xvy_ctrl_00,	outputcsc_en,		0x1, \
												degamma_en,			0x1, \
												pcc_en,				0x1, \
												gamma_en,			0x1, \
												hsv_hsl_selection,	0x0);
		}
		else
		{
			PE_CSC_HW_M14_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
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
int PE_CSC_HW_M14_SetDebugSettings(LX_PE_DBG_SETTINGS_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		PE_CSC_HW_M14_DBG_PRINT("wid:%d,type:0x%x,[0x%x]print:0x%x,bypass:0x%x\n",\
			pstParams->win_id,pstParams->type,(0x1<<PE_ITEM_PKTMODL(CSC)),\
			pstParams->print_lvl,pstParams->bypass);
		/* set print level */
		if(pstParams->type&LX_PE_DBG_LV)
		{
			_g_csc_hw_m14_trace = (pstParams->print_lvl & (0x1<<PE_ITEM_PKTMODL(CSC)))? 0x1:0x0;
		}
		/* set bypass */
		if(pstParams->type&LX_PE_DBG_BY)
		{
			if(PE_KDRV_VER_M14BX)
			{
				PE_CSC_HW_M14_DBG_PRINT("not supported.\n");
			}
			else if(PE_KDRV_VER_M14AX)
			{
				if(pstParams->bypass & (0x1<<PE_ITEM_PKTMODL(CSC)))
				{
					PE_CSC_HW_M14_DBG_PRINT("degamma,pcc,gamma,s scaler : disable.\n");
					if(PE_CHECK_WIN0(pstParams->win_id))
					{
						PE_P1L_M14A0_QWr04(pe1_xvy_ctrl_00, degamma_en,					0x0, \
															pcc_en,						0x0, \
															gamma_en,					0x0, \
															s_adpative_scaler_enable,	0x0);
					}
				}
				else
				{
					PE_CSC_HW_M14_DBG_PRINT("outcsc,degamma,pcc,gamma : enable.\n");
					if(PE_CHECK_WIN0(pstParams->win_id))
					{
						PE_P1L_M14A0_QWr03(pe1_xvy_ctrl_00, degamma_en,			0x1, \
															pcc_en,				0x1, \
															gamma_en,			0x1);
					}
				}
			}
			else
			{
				PE_CSC_HW_M14_DBG_PRINT("do nothing.\n");	ret = RET_OK;
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
int PE_CSC_HW_M14_SetColorGamut(LX_PE_CSC_GAMUT_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		PE_CSC_HW_M14_DBG_PRINT("set pstParams[%d] : 0x%x,0x%x,0x%x, 0x%x,0x%x,0x%x, 0x%x,0x%x,0x%x\n",\
			pstParams->win_id,pstParams->matrix[0],pstParams->matrix[1],pstParams->matrix[2],\
			pstParams->matrix[3],pstParams->matrix[4],pstParams->matrix[5],pstParams->matrix[6],\
			pstParams->matrix[7],pstParams->matrix[8]);
		if(PE_KDRV_VER_M14BX)
		{
			PE_CSC_HW_M14_DBG_PRINT("not supported.\n");
		}
		else if(PE_KDRV_VER_M14AX)
		{
			if(PE_CHECK_WIN0(pstParams->win_id))
			{
				PE_P1L_M14A0_QWr01(pe1_xvy_ctrl_08,	r_pcc_coef0, GET_BITS(pstParams->matrix[0],0,15));
				PE_P1L_M14A0_QWr02(pe1_xvy_ctrl_09,	r_pcc_coef1, GET_BITS(pstParams->matrix[1],0,15),\
													r_pcc_coef2, GET_BITS(pstParams->matrix[2],0,15));
				PE_P1L_M14A0_QWr02(pe1_xvy_ctrl_0a,	r_pcc_coef3, GET_BITS(pstParams->matrix[3],0,15),\
													r_pcc_coef4, GET_BITS(pstParams->matrix[4],0,15));
				PE_P1L_M14A0_QWr02(pe1_xvy_ctrl_0b,	r_pcc_coef5, GET_BITS(pstParams->matrix[5],0,15),\
													r_pcc_coef6, GET_BITS(pstParams->matrix[6],0,15));
				PE_P1L_M14A0_QWr02(pe1_xvy_ctrl_0c,	r_pcc_coef7, GET_BITS(pstParams->matrix[7],0,15),\
													r_pcc_coef8, GET_BITS(pstParams->matrix[8],0,15));
			}
		}
		else
		{
			PE_CSC_HW_M14_DBG_PRINT("nothing to do\n");	ret = RET_OK;
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
int PE_CSC_HW_M14_GetColorGamut(LX_PE_CSC_GAMUT_T *pstParams)
{
	int ret = RET_OK;
	LX_PE_WIN_ID win_id;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		win_id = PE_GET_CHECKED_WINID(pstParams->win_id);
		if(PE_KDRV_VER_M14BX)
		{
			PE_CSC_HW_M14_DBG_PRINT("not supported.\n");
		}
		else if(PE_KDRV_VER_M14AX)
		{
			if(PE_CHECK_WIN0(win_id))
			{
				PE_P1L_M14A0_QRd01(pe1_xvy_ctrl_08,	r_pcc_coef0, pstParams->matrix[0]);
				PE_P1L_M14A0_QRd02(pe1_xvy_ctrl_09,	r_pcc_coef1, pstParams->matrix[1],\
													r_pcc_coef2, pstParams->matrix[2]);
				PE_P1L_M14A0_QRd02(pe1_xvy_ctrl_0a,	r_pcc_coef3, pstParams->matrix[3],\
													r_pcc_coef4, pstParams->matrix[4]);
				PE_P1L_M14A0_QRd02(pe1_xvy_ctrl_0b,	r_pcc_coef5, pstParams->matrix[5],\
													r_pcc_coef6, pstParams->matrix[6]);
				PE_P1L_M14A0_QRd02(pe1_xvy_ctrl_0c,	r_pcc_coef7, pstParams->matrix[7],\
													r_pcc_coef8, pstParams->matrix[8]);
			}
		}
		else
		{
			PE_CSC_HW_M14_DBG_PRINT("nothing to do\n");	ret = RET_OK;
		}
		PE_CSC_HW_M14_DBG_PRINT("get pstParams[%d] : 0x%x,0x%x,0x%x, 0x%x,0x%x,0x%x, 0x%x,0x%x,0x%x\n",\
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
int PE_CSC_HW_M14_SetPostCsc(LX_PE_CSC_POST_T *pstParams)
{
	int ret = RET_OK;
	PE_CSC_HW_M14_SETTINGS_T *pInfo=&_g_pe_csc_hw_m14_info;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		PE_CSC_HW_M14_DBG_PRINT("set pstParams[%d] : type:%d\n"\
			"coef:%x,%x,%x, %x,%x,%x, %x,%x,%x, ofs:%x,%x,%x, %x,%x,%x\n",\
			pstParams->win_id,pstParams->type,\
			pstParams->matrix[0],pstParams->matrix[1],pstParams->matrix[2],\
			pstParams->matrix[3],pstParams->matrix[4],pstParams->matrix[5],pstParams->matrix[6],\
			pstParams->matrix[7],pstParams->matrix[8],pstParams->offset[0],pstParams->offset[1],\
			pstParams->offset[2],pstParams->offset[3],pstParams->offset[4],pstParams->offset[5]);
		if(PE_KDRV_VER_M14BX)
		{
			if(PE_CHECK_WIN0(pstParams->win_id))
			{
				PE_CSC_HW_M14_DBG_PRINT("set type:%d -> %d\n", pInfo->cur_post_type, pstParams->type);
				pInfo->cur_post_type = pstParams->type;
			}
		}
		else if(PE_KDRV_VER_M14AX)
		{
			if(PE_CHECK_WIN0(pstParams->win_id))
			{
				PE_P1L_M14A0_QWr02(pe1_xvy_ctrl_01,	r_csc_coef0, GET_BITS(pstParams->matrix[0],0,15),\
													r_csc_coef1, GET_BITS(pstParams->matrix[1],0,15));
				PE_P1L_M14A0_QWr02(pe1_xvy_ctrl_02,	r_csc_coef2, GET_BITS(pstParams->matrix[2],0,15),\
													r_csc_coef3, GET_BITS(pstParams->matrix[3],0,15));
				PE_P1L_M14A0_QWr02(pe1_xvy_ctrl_03,	r_csc_coef4, GET_BITS(pstParams->matrix[4],0,15),\
													r_csc_coef5, GET_BITS(pstParams->matrix[5],0,15));
				PE_P1L_M14A0_QWr02(pe1_xvy_ctrl_04,	r_csc_coef6, GET_BITS(pstParams->matrix[6],0,15),\
													r_csc_coef7, GET_BITS(pstParams->matrix[7],0,15));
				PE_P1L_M14A0_QWr02(pe1_xvy_ctrl_05,	r_csc_coef8, GET_BITS(pstParams->matrix[8],0,15),\
													r_csc_ofst0, GET_BITS(pstParams->offset[0],0,11));
				PE_P1L_M14A0_QWr02(pe1_xvy_ctrl_06,	r_csc_ofst1, GET_BITS(pstParams->offset[1],0,11),\
													r_csc_ofst2, GET_BITS(pstParams->offset[2],0,11));
				PE_P1L_M14A0_QWr02(pe1_xvy_ctrl_07,	r_csc_ofst3, GET_BITS(pstParams->offset[3],0,14),\
													r_csc_ofst4, GET_BITS(pstParams->offset[4],0,14));
				PE_P1L_M14A0_QWr01(pe1_xvy_ctrl_08,	r_csc_ofst5, GET_BITS(pstParams->offset[5],0,14));
			}
		}
		else
		{
			PE_CSC_HW_M14_DBG_PRINT("nothing to do\n");	ret = RET_OK;
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
int PE_CSC_HW_M14_GetPostCsc(LX_PE_CSC_POST_T *pstParams)
{
	int ret = RET_OK;
	LX_PE_WIN_ID win_id;
	PE_CSC_HW_M14_SETTINGS_T *pInfo=&_g_pe_csc_hw_m14_info;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		win_id = PE_GET_CHECKED_WINID(pstParams->win_id);
		if(PE_KDRV_VER_M14BX)
		{
			if(PE_CHECK_WIN0(win_id))
			{
				pstParams->type = pInfo->pre_post_type;
				PE_PE1_M14B0_QRd02(pe1_ocsc_ctrl_00,r_csc_coef0, pstParams->matrix[0],\
													r_csc_coef1, pstParams->matrix[1]);
				PE_PE1_M14B0_QRd02(pe1_ocsc_ctrl_01,r_csc_coef2, pstParams->matrix[2],\
													r_csc_coef3, pstParams->matrix[3]);
				PE_PE1_M14B0_QRd02(pe1_ocsc_ctrl_02,r_csc_coef4, pstParams->matrix[4],\
													r_csc_coef5, pstParams->matrix[5]);
				PE_PE1_M14B0_QRd02(pe1_ocsc_ctrl_03,r_csc_coef6, pstParams->matrix[6],\
													r_csc_coef7, pstParams->matrix[7]);
				PE_PE1_M14B0_QRd02(pe1_ocsc_ctrl_04,r_csc_coef8, pstParams->matrix[8],\
													r_csc_ofst0, pstParams->offset[0]);
				PE_PE1_M14B0_QRd02(pe1_ocsc_ctrl_05,r_csc_ofst1, pstParams->offset[1],\
													r_csc_ofst2, pstParams->offset[2]);
				PE_PE1_M14B0_QRd02(pe1_ocsc_ctrl_06,r_csc_ofst3, pstParams->offset[3],\
													r_csc_ofst4, pstParams->offset[4]);
				PE_PE1_M14B0_QRd01(pe1_ocsc_ctrl_07,r_csc_ofst5, pstParams->offset[5]);
				memset(pstParams->coeff0, 0x0, sizeof(UINT16)*PE_NUM_OF_CSC_COEF);
				memset(pstParams->coeff1, 0x0, sizeof(UINT16)*PE_NUM_OF_CSC_COEF);
				memset(pstParams->offst0, 0x0, sizeof(UINT16)*PE_NUM_OF_CSC_OFST);
			}
		}
		else if(PE_KDRV_VER_M14AX)
		{
			if(PE_CHECK_WIN0(win_id))
			{
				pstParams->type = pInfo->pre_post_type;
				PE_P1L_M14A0_QRd02(pe1_xvy_ctrl_01,	r_csc_coef0, pstParams->matrix[0],\
													r_csc_coef1, pstParams->matrix[1]);
				PE_P1L_M14A0_QRd02(pe1_xvy_ctrl_02,	r_csc_coef2, pstParams->matrix[2],\
													r_csc_coef3, pstParams->matrix[3]);
				PE_P1L_M14A0_QRd02(pe1_xvy_ctrl_03,	r_csc_coef4, pstParams->matrix[4],\
													r_csc_coef5, pstParams->matrix[5]);
				PE_P1L_M14A0_QRd02(pe1_xvy_ctrl_04,	r_csc_coef6, pstParams->matrix[6],\
													r_csc_coef7, pstParams->matrix[7]);
				PE_P1L_M14A0_QRd02(pe1_xvy_ctrl_05,	r_csc_coef8, pstParams->matrix[8],\
													r_csc_ofst0, pstParams->offset[0]);
				PE_P1L_M14A0_QRd02(pe1_xvy_ctrl_06,	r_csc_ofst1, pstParams->offset[1],\
													r_csc_ofst2, pstParams->offset[2]);
				PE_P1L_M14A0_QRd02(pe1_xvy_ctrl_07,	r_csc_ofst3, pstParams->offset[3],\
													r_csc_ofst4, pstParams->offset[4]);
				PE_P1L_M14A0_QRd01(pe1_xvy_ctrl_08,	r_csc_ofst5, pstParams->offset[5]);
				memset(pstParams->coeff0, 0x0, sizeof(UINT16)*PE_NUM_OF_CSC_COEF);
				memset(pstParams->coeff1, 0x0, sizeof(UINT16)*PE_NUM_OF_CSC_COEF);
				memset(pstParams->offst0, 0x0, sizeof(UINT16)*PE_NUM_OF_CSC_OFST);
			}
		}
		else
		{
			PE_CSC_HW_M14_DBG_PRINT("nothing to do\n");	ret = RET_OK;
		}
		PE_CSC_HW_M14_DBG_PRINT("get pstParams[%d] : type:%d\n"\
			"coef:%x,%x,%x, %x,%x,%x, %x,%x,%x, ofs:%x,%x,%x, %x,%x,%x\n",\
			pstParams->win_id,pstParams->type,\
			pstParams->matrix[0],pstParams->matrix[1],pstParams->matrix[2],\
			pstParams->matrix[3],pstParams->matrix[4],pstParams->matrix[5],pstParams->matrix[6],\
			pstParams->matrix[7],pstParams->matrix[8],pstParams->offset[0],pstParams->offset[1],\
			pstParams->offset[2],pstParams->offset[3],pstParams->offset[4],pstParams->offset[5]);
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
int PE_CSC_HW_M14_SetInputCsc(LX_PE_CSC_INPUT_T *pstParams)
{
	int ret = RET_OK;
	LX_PE_WIN_ID win_id;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		PE_CSC_HW_M14_DBG_PRINT("set pstParams[%d] : en:%d\n"\
			"coef:%x,%x,%x, %x,%x,%x, %x,%x,%x, ofs:%x,%x,%x, %x,%x,%x\n",\
			pstParams->win_id,pstParams->enable,\
			pstParams->matrix[0],pstParams->matrix[1],pstParams->matrix[2],\
			pstParams->matrix[3],pstParams->matrix[4],pstParams->matrix[5],\
			pstParams->matrix[6],pstParams->matrix[7],pstParams->matrix[8],\
			pstParams->offset[0],pstParams->offset[1],pstParams->offset[2],\
			pstParams->offset[3],pstParams->offset[4],pstParams->offset[5]);
		if(PE_KDRV_VER_M14BX)
		{
			win_id = pstParams->win_id;
			if(PE_CHECK_WIN0(win_id))
			{
				pstParams->win_id = LX_PE_WIN_0;
				ret = PE_FWI_M14_SetCviCsc(pstParams);
				PE_CSC_HW_M14_CHECK_CODE(ret,break,"[%s,%d] PE_FWI_M14_SetCviCsc() error.\n",__F__,__L__);
			}
			if(PE_CHECK_WIN1(win_id))
			{
				pstParams->win_id = LX_PE_WIN_1;
				ret = PE_FWI_M14_SetCviCsc(pstParams);
				PE_CSC_HW_M14_CHECK_CODE(ret,break,"[%s,%d] PE_FWI_M14_SetCviCsc() error.\n",__F__,__L__);
			}
		}
		else if(PE_KDRV_VER_M14AX)
		{
			/* If the reg. setting has a transition artifact, use the firmware setting. */
			#if 0	// reg. setting
			if(PE_CHECK_WIN0(pstParams->win_id))
			{
				PE_CVA_M14A0_RdFL(cva_csc_ctrl0);
				PE_CVA_M14A0_RdFL(cva_csc_ctrl1);
				PE_CVA_M14A0_RdFL(cva_csc_ctrl2);
				PE_CVA_M14A0_RdFL(cva_csc_ctrl3);
				PE_CVA_M14A0_RdFL(cva_csc_ctrl4);
				PE_CVA_M14A0_RdFL(cva_csc_ctrl5);
				PE_CVA_M14A0_RdFL(cva_csc_ctrl6);
				PE_CVA_M14A0_RdFL(cva_csc_ctrl7);
				PE_CVA_M14A0_Wr01(cva_csc_ctrl0, csc_coefficient0,	GET_BITS(pstParams->matrix[0],0,15));
				PE_CVA_M14A0_Wr01(cva_csc_ctrl0, csc_coefficient1,	GET_BITS(pstParams->matrix[1],0,15));
				PE_CVA_M14A0_Wr01(cva_csc_ctrl1, csc_coefficient2,	GET_BITS(pstParams->matrix[2],0,15));
				PE_CVA_M14A0_Wr01(cva_csc_ctrl1, csc_coefficient3,	GET_BITS(pstParams->matrix[3],0,15));
				PE_CVA_M14A0_Wr01(cva_csc_ctrl2, csc_coefficient4,	GET_BITS(pstParams->matrix[4],0,15));
				PE_CVA_M14A0_Wr01(cva_csc_ctrl2, csc_coefficient5,	GET_BITS(pstParams->matrix[5],0,15));
				PE_CVA_M14A0_Wr01(cva_csc_ctrl3, csc_coefficient6,	GET_BITS(pstParams->matrix[6],0,15));
				PE_CVA_M14A0_Wr01(cva_csc_ctrl3, csc_coefficient7,	GET_BITS(pstParams->matrix[7],0,15));
				PE_CVA_M14A0_Wr01(cva_csc_ctrl4, csc_coefficient8,	GET_BITS(pstParams->matrix[8],0,15));
				PE_CVA_M14A0_Wr01(cva_csc_ctrl4, csc_offset0,		GET_BITS(pstParams->offset[0],0,11));
				PE_CVA_M14A0_Wr01(cva_csc_ctrl5, csc_offset1,		GET_BITS(pstParams->offset[1],0,11));
				PE_CVA_M14A0_Wr01(cva_csc_ctrl5, csc_offset2,		GET_BITS(pstParams->offset[2],0,11));
				PE_CVA_M14A0_Wr01(cva_csc_ctrl6, csc_offset3,		GET_BITS(pstParams->offset[3],0,11));
				PE_CVA_M14A0_Wr01(cva_csc_ctrl6, csc_offset4,		GET_BITS(pstParams->offset[4],0,11));
				PE_CVA_M14A0_Wr01(cva_csc_ctrl7, csc_offset5,		GET_BITS(pstParams->offset[5],0,11));
				PE_CVA_M14A0_Wr01(cva_csc_ctrl7, csc_en,			GET_BITS(pstParams->enable,0,1));
				PE_CVA_M14A0_WrFL(cva_csc_ctrl0);
				PE_CVA_M14A0_WrFL(cva_csc_ctrl1);
				PE_CVA_M14A0_WrFL(cva_csc_ctrl2);
				PE_CVA_M14A0_WrFL(cva_csc_ctrl3);
				PE_CVA_M14A0_WrFL(cva_csc_ctrl4);
				PE_CVA_M14A0_WrFL(cva_csc_ctrl5);
				PE_CVA_M14A0_WrFL(cva_csc_ctrl6);
				PE_CVA_M14A0_WrFL(cva_csc_ctrl7);
				if(pstParams->enable)
				{
					PE_CVA_M14A0_Wr01(cva_csc_ctrl7, csc_coeff_load,	0x1);	//this bit is auto cleared
					PE_CVA_M14A0_WrFL(cva_csc_ctrl7);
				}
			}
			if(PE_CHECK_WIN1(pstParams->win_id))
			{
				PE_CVB_M14A0_RdFL(cvb_csc_ctrl0);
				PE_CVB_M14A0_RdFL(cvb_csc_ctrl1);
				PE_CVB_M14A0_RdFL(cvb_csc_ctrl2);
				PE_CVB_M14A0_RdFL(cvb_csc_ctrl3);
				PE_CVB_M14A0_RdFL(cvb_csc_ctrl4);
				PE_CVB_M14A0_RdFL(cvb_csc_ctrl5);
				PE_CVB_M14A0_RdFL(cvb_csc_ctrl6);
				PE_CVB_M14A0_RdFL(cvb_csc_ctrl7);
				PE_CVB_M14A0_Wr01(cvb_csc_ctrl0, csc_coefficient0,	GET_BITS(pstParams->matrix[0],0,15));
				PE_CVB_M14A0_Wr01(cvb_csc_ctrl0, csc_coefficient1,	GET_BITS(pstParams->matrix[1],0,15));
				PE_CVB_M14A0_Wr01(cvb_csc_ctrl1, csc_coefficient2,	GET_BITS(pstParams->matrix[2],0,15));
				PE_CVB_M14A0_Wr01(cvb_csc_ctrl1, csc_coefficient3,	GET_BITS(pstParams->matrix[3],0,15));
				PE_CVB_M14A0_Wr01(cvb_csc_ctrl2, csc_coefficient4,	GET_BITS(pstParams->matrix[4],0,15));
				PE_CVB_M14A0_Wr01(cvb_csc_ctrl2, csc_coefficient5,	GET_BITS(pstParams->matrix[5],0,15));
				PE_CVB_M14A0_Wr01(cvb_csc_ctrl3, csc_coefficient6,	GET_BITS(pstParams->matrix[6],0,15));
				PE_CVB_M14A0_Wr01(cvb_csc_ctrl3, csc_coefficient7,	GET_BITS(pstParams->matrix[7],0,15));
				PE_CVB_M14A0_Wr01(cvb_csc_ctrl4, csc_coefficient8,	GET_BITS(pstParams->matrix[8],0,15));
				PE_CVB_M14A0_Wr01(cvb_csc_ctrl4, csc_offset0,		GET_BITS(pstParams->offset[0],0,11));
				PE_CVB_M14A0_Wr01(cvb_csc_ctrl5, csc_offset1,		GET_BITS(pstParams->offset[1],0,11));
				PE_CVB_M14A0_Wr01(cvb_csc_ctrl5, csc_offset2,		GET_BITS(pstParams->offset[2],0,11));
				PE_CVB_M14A0_Wr01(cvb_csc_ctrl6, csc_offset3,		GET_BITS(pstParams->offset[3],0,11));
				PE_CVB_M14A0_Wr01(cvb_csc_ctrl6, csc_offset4,		GET_BITS(pstParams->offset[4],0,11));
				PE_CVB_M14A0_Wr01(cvb_csc_ctrl7, csc_offset5,		GET_BITS(pstParams->offset[5],0,11));
				PE_CVB_M14A0_Wr01(cvb_csc_ctrl7, csc_en,			GET_BITS(pstParams->enable,0,1));
				PE_CVB_M14A0_WrFL(cvb_csc_ctrl0);
				PE_CVB_M14A0_WrFL(cvb_csc_ctrl1);
				PE_CVB_M14A0_WrFL(cvb_csc_ctrl2);
				PE_CVB_M14A0_WrFL(cvb_csc_ctrl3);
				PE_CVB_M14A0_WrFL(cvb_csc_ctrl4);
				PE_CVB_M14A0_WrFL(cvb_csc_ctrl5);
				PE_CVB_M14A0_WrFL(cvb_csc_ctrl6);
				PE_CVB_M14A0_WrFL(cvb_csc_ctrl7);
				if(pstParams->enable)
				{
					PE_CVB_M14A0_Wr01(cvb_csc_ctrl7, csc_coeff_load,	0x1);	//this bit is auto cleared
					PE_CVB_M14A0_WrFL(cvb_csc_ctrl7);
				}
			}
			#else	// firmware setting
			win_id = pstParams->win_id;
			if(PE_CHECK_WIN0(win_id))
			{
				pstParams->win_id = LX_PE_WIN_0;
				ret = PE_FWI_M14_SetCviCsc(pstParams);
				PE_CSC_HW_M14_CHECK_CODE(ret,break,"[%s,%d] PE_FWI_M14_SetCviCsc() error.\n",__F__,__L__);
			}
			if(PE_CHECK_WIN1(win_id))
			{
				pstParams->win_id = LX_PE_WIN_1;
				ret = PE_FWI_M14_SetCviCsc(pstParams);
				PE_CSC_HW_M14_CHECK_CODE(ret,break,"[%s,%d] PE_FWI_M14_SetCviCsc() error.\n",__F__,__L__);
			}
			#endif
		}
		else
		{
			PE_CSC_HW_M14_DBG_PRINT("nothing to do\n");	ret = RET_OK;
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
int PE_CSC_HW_M14_GetInputCsc(LX_PE_CSC_INPUT_T *pstParams)
{
	int ret = RET_OK;
	LX_PE_WIN_ID win_id;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		win_id = PE_GET_CHECKED_WINID(pstParams->win_id);
		if(PE_KDRV_VER_M14BX)
		{
			if(PE_CHECK_WIN0(win_id))
			{
				PE_CVA_M14B0_QRd02(cva_csc_ctrl0,	csc_coefficient0,	pstParams->matrix[0],\
													csc_coefficient1,	pstParams->matrix[1]);
				PE_CVA_M14B0_QRd02(cva_csc_ctrl1,	csc_coefficient2,	pstParams->matrix[2],\
													csc_coefficient3,	pstParams->matrix[3]);
				PE_CVA_M14B0_QRd02(cva_csc_ctrl2,	csc_coefficient4,	pstParams->matrix[4],\
													csc_coefficient5,	pstParams->matrix[5]);
				PE_CVA_M14B0_QRd02(cva_csc_ctrl3,	csc_coefficient6,	pstParams->matrix[6],\
													csc_coefficient7,	pstParams->matrix[7]);
				PE_CVA_M14B0_QRd02(cva_csc_ctrl4,	csc_coefficient8,	pstParams->matrix[8],\
													csc_offset0,		pstParams->offset[0]);
				PE_CVA_M14B0_QRd02(cva_csc_ctrl5,	csc_offset1,		pstParams->offset[1],\
													csc_offset2,		pstParams->offset[2]);
				PE_CVA_M14B0_QRd02(cva_csc_ctrl6,	csc_offset3,		pstParams->offset[3],\
													csc_offset4,		pstParams->offset[4]);
				PE_CVA_M14B0_QRd02(cva_csc_ctrl7,	csc_offset5,		pstParams->offset[5],\
													csc_en,				pstParams->enable);
			}
			if(PE_CHECK_WIN1(win_id))
			{
				PE_CVB_M14B0_QRd02(cvb_csc_ctrl0,	csc_coefficient0,	pstParams->matrix[0],\
													csc_coefficient1,	pstParams->matrix[1]);
				PE_CVB_M14B0_QRd02(cvb_csc_ctrl1,	csc_coefficient2,	pstParams->matrix[2],\
													csc_coefficient3,	pstParams->matrix[3]);
				PE_CVB_M14B0_QRd02(cvb_csc_ctrl2,	csc_coefficient4,	pstParams->matrix[4],\
													csc_coefficient5,	pstParams->matrix[5]);
				PE_CVB_M14B0_QRd02(cvb_csc_ctrl3,	csc_coefficient6,	pstParams->matrix[6],\
													csc_coefficient7,	pstParams->matrix[7]);
				PE_CVB_M14B0_QRd02(cvb_csc_ctrl4,	csc_coefficient8,	pstParams->matrix[8],\
													csc_offset0,		pstParams->offset[0]);
				PE_CVB_M14B0_QRd02(cvb_csc_ctrl5,	csc_offset1,		pstParams->offset[1],\
													csc_offset2,		pstParams->offset[2]);
				PE_CVB_M14B0_QRd02(cvb_csc_ctrl6,	csc_offset3,		pstParams->offset[3],\
													csc_offset4,		pstParams->offset[4]);
				PE_CVB_M14B0_QRd02(cvb_csc_ctrl7,	csc_offset5,		pstParams->offset[5],\
													csc_en,				pstParams->enable);
			}
		}
		else if(PE_KDRV_VER_M14AX)
		{
			if(PE_CHECK_WIN0(win_id))
			{
				PE_CVA_M14A0_QRd02(cva_csc_ctrl0,	csc_coefficient0,	pstParams->matrix[0],\
													csc_coefficient1,	pstParams->matrix[1]);
				PE_CVA_M14A0_QRd02(cva_csc_ctrl1,	csc_coefficient2,	pstParams->matrix[2],\
													csc_coefficient3,	pstParams->matrix[3]);
				PE_CVA_M14A0_QRd02(cva_csc_ctrl2,	csc_coefficient4,	pstParams->matrix[4],\
													csc_coefficient5,	pstParams->matrix[5]);
				PE_CVA_M14A0_QRd02(cva_csc_ctrl3,	csc_coefficient6,	pstParams->matrix[6],\
													csc_coefficient7,	pstParams->matrix[7]);
				PE_CVA_M14A0_QRd02(cva_csc_ctrl4,	csc_coefficient8,	pstParams->matrix[8],\
													csc_offset0,		pstParams->offset[0]);
				PE_CVA_M14A0_QRd02(cva_csc_ctrl5,	csc_offset1,		pstParams->offset[1],\
													csc_offset2,		pstParams->offset[2]);
				PE_CVA_M14A0_QRd02(cva_csc_ctrl6,	csc_offset3,		pstParams->offset[3],\
													csc_offset4,		pstParams->offset[4]);
				PE_CVA_M14A0_QRd02(cva_csc_ctrl7,	csc_offset5,		pstParams->offset[5],\
													csc_en,				pstParams->enable);
			}
			if(PE_CHECK_WIN1(win_id))
			{
				PE_CVB_M14A0_QRd02(cvb_csc_ctrl0,	csc_coefficient0,	pstParams->matrix[0],\
													csc_coefficient1,	pstParams->matrix[1]);
				PE_CVB_M14A0_QRd02(cvb_csc_ctrl1,	csc_coefficient2,	pstParams->matrix[2],\
													csc_coefficient3,	pstParams->matrix[3]);
				PE_CVB_M14A0_QRd02(cvb_csc_ctrl2,	csc_coefficient4,	pstParams->matrix[4],\
													csc_coefficient5,	pstParams->matrix[5]);
				PE_CVB_M14A0_QRd02(cvb_csc_ctrl3,	csc_coefficient6,	pstParams->matrix[6],\
													csc_coefficient7,	pstParams->matrix[7]);
				PE_CVB_M14A0_QRd02(cvb_csc_ctrl4,	csc_coefficient8,	pstParams->matrix[8],\
													csc_offset0,		pstParams->offset[0]);
				PE_CVB_M14A0_QRd02(cvb_csc_ctrl5,	csc_offset1,		pstParams->offset[1],\
													csc_offset2,		pstParams->offset[2]);
				PE_CVB_M14A0_QRd02(cvb_csc_ctrl6,	csc_offset3,		pstParams->offset[3],\
													csc_offset4,		pstParams->offset[4]);
				PE_CVB_M14A0_QRd02(cvb_csc_ctrl7,	csc_offset5,		pstParams->offset[5],\
													csc_en,				pstParams->enable);
			}
		}
		else
		{
			PE_CSC_HW_M14_DBG_PRINT("nothing to do\n");	ret = RET_OK;
		}
		PE_CSC_HW_M14_DBG_PRINT("get pstParams[%d] : en:%d\n"\
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
int PE_CSC_HW_M14_SetxvYCC(LX_PE_CSC_XVYCC_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		PE_CSC_HW_M14_DBG_PRINT("set pstParams[%d] : en:%d, sclr:0x%x\n",\
			pstParams->win_id,pstParams->enable,pstParams->scaler);
		if(PE_KDRV_VER_M14BX)
		{
			PE_CSC_HW_M14_DBG_PRINT("not supported.\n");
		}
		else if(PE_KDRV_VER_M14AX)
		{
			if(PE_CHECK_WIN0(pstParams->win_id))
			{
				PE_P1L_M14A0_QWr01(pe1_xvy_ctrl_00, scaler_en,	(pstParams->enable)? 0x1:0x0);
				PE_P1L_M14A0_QWr01(pe1_xvy_ctrl_0d, scaler,		GET_BITS(pstParams->scaler,0,8));
			}
		}
		else
		{
			PE_CSC_HW_M14_DBG_PRINT("nothing to do\n");	ret = RET_OK;
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
int PE_CSC_HW_M14_GetxvYCC(LX_PE_CSC_XVYCC_T *pstParams)
{
	int ret = RET_OK;
	LX_PE_WIN_ID win_id;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		win_id = PE_GET_CHECKED_WINID(pstParams->win_id);
		if(PE_KDRV_VER_M14BX)
		{
			PE_CSC_HW_M14_DBG_PRINT("not supported.\n");
		}
		else if(PE_KDRV_VER_M14AX)
		{
			if(PE_CHECK_WIN0(win_id))
			{
				PE_P1L_M14A0_QRd01(pe1_xvy_ctrl_00, scaler_en,	pstParams->enable);
				PE_P1L_M14A0_QRd01(pe1_xvy_ctrl_0d, scaler,		pstParams->scaler);
			}
		}
		else
		{
			PE_CSC_HW_M14_DBG_PRINT("nothing to do\n");	ret = RET_OK;
		}
		PE_CSC_HW_M14_DBG_PRINT("get pstParams[%d] : en:%d, sclr:0x%x\n",\
			pstParams->win_id,pstParams->enable,pstParams->scaler);
	}while(0);
	return ret;
}
/**
 * set csc mode
 *
 * @param   *pCfg [in] PE_TSK_M14_CFG_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_CSC_HW_M14_SetCscMode(PE_TSK_M14_CFG_T *pCfg)
{
    int ret = RET_OK;
	do{
		CHECK_KNULL(pCfg);
		if(PE_KDRV_VER_M14)
		{
			ret = PE_CSC_HW_M14_SetPostCscMode(pCfg);
			PE_CSC_HW_M14_CHECK_CODE(ret, break, \
				"[%s,%d] PE_CSC_HW_M14_SetPostCscMode() error.\n",__F__,__L__);
		}
		else
		{
			PE_CSC_HW_M14_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * set tnr lb ctrl
 *
 * @param   *pCfg [in] PE_TSK_M14_CFG_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_CSC_HW_M14_SetPostCscMode(PE_TSK_M14_CFG_T *pCfg)
{
	int ret = RET_OK;
	static UINT32 count = 0;
	UINT32 cur_post_type;
	PE_TSK_M14_OPR_T *p_opr = &(pCfg->opr);
	LX_PE_INF_DISPLAY_T *disp0_info = &pCfg->inf.disp_info[LX_PE_WIN_0];
	PE_CSC_HW_M14_SETTINGS_T *pInfo=&_g_pe_csc_hw_m14_info;
	do{
		CHECK_KNULL(pCfg);
		if(PE_KDRV_VER_M14)
		{
			/* checking post type */
			if (p_opr->pause_task == PE_TSK_M14_RUN_STOP_ALGO)
			{
				cur_post_type = pInfo->cur_post_type;
			}
			else if (disp0_info->src_type==LX_PE_SRC_DTV && \
				(disp0_info->dtv_type==LX_PE_DTV_NORMAL || \
				(disp0_info->dtv_type==LX_PE_DTV_HDDPLAY && \
				disp0_info->hdd_type==LX_PE_HDD_SRC_DTV)))
			{
				if (disp0_info->in_h_size>=PE_CSC_HW_M14_BT709_H_SIZE)
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
				PE_CSC_HW_M14_DBG_PRINT("[inf] src,dtv,hdd:%d,%d,%d, h:%d, ext:%d\n",\
					disp0_info->src_type, disp0_info->dtv_type, disp0_info->hdd_type, \
					disp0_info->in_h_size, disp0_info->mode.is_ext_frc);
				PE_CSC_HW_M14_DBG_PRINT("[set] user:%d, post_type:%d -> %d\n",\
					pInfo->cur_post_type, pInfo->pre_post_type, cur_post_type);
				ret = PE_CSC_HW_M14_SetPostCscReg(cur_post_type);
				PE_CSC_HW_M14_CHECK_CODE(ret, break, \
					"[%s,%d] PE_CSC_HW_M14_SetPostCscReg() error.\n",__F__,__L__);
				pInfo->pre_post_type = cur_post_type;
			}
			/* **********************************************************/
			if(_g_csc_hw_m14_trace)
			{
				if(count>PE_PRINT_COUNT_NUM)
				{
					PE_CSC_HW_M14_DBG_PRINT("[state] src,dtv,hdd:%d,%d,%d, h:%d, ext:%d\n",\
						disp0_info->src_type, disp0_info->dtv_type, disp0_info->hdd_type, \
						disp0_info->in_h_size, disp0_info->mode.is_ext_frc);
					PE_CSC_HW_M14_DBG_PRINT("[state] user:%d, post_type:%d\n",\
						pInfo->cur_post_type, cur_post_type);
					count=0;
				}
				count++;
			}
		}
		else
		{
			PE_CSC_HW_M14_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
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
static int PE_CSC_HW_M14_SetPostCscReg(UINT32 type)
{
	int ret = RET_OK;
	const UINT16 *p_co, *p_of;
	do{
		if(PE_KDRV_VER_M14)
		{
			/* cur_type */
			switch (type)
			{
				case LX_PE_POST_CSC_BYPASS:
					p_co = _g_pe_csc_hw_m14_post_mtrx_bypass;
					p_of = _g_pe_csc_hw_m14_post_ofst_bypass;
					break;
				case LX_PE_POST_CSC_Y709_EXPEND:
					p_co = _g_pe_csc_hw_m14_post_y709_expand;
					p_of = _g_pe_csc_hw_m14_post_ofst_expand;
					break;
				case LX_PE_POST_CSC_Y709_LINEAR:
					p_co = _g_pe_csc_hw_m14_post_y709_linear;
					p_of = _g_pe_csc_hw_m14_post_ofst_linear;
					break;
				case LX_PE_POST_CSC_Y601_LINEAR:
					p_co = _g_pe_csc_hw_m14_post_y601_linear;
					p_of = _g_pe_csc_hw_m14_post_ofst_linear;
					break;
				case LX_PE_POST_CSC_Y601_EXPEND:
				default:
					p_co = _g_pe_csc_hw_m14_post_y601_expand;
					p_of = _g_pe_csc_hw_m14_post_ofst_expand;
					break;
			}
			PE_PE1_M14B0_QWr02(pe1_ocsc_ctrl_00,r_csc_coef0, GET_BITS(p_co[0],0,15),\
												r_csc_coef1, GET_BITS(p_co[1],0,15));
			PE_PE1_M14B0_QWr02(pe1_ocsc_ctrl_01,r_csc_coef2, GET_BITS(p_co[2],0,15),\
												r_csc_coef3, GET_BITS(p_co[3],0,15));
			PE_PE1_M14B0_QWr02(pe1_ocsc_ctrl_02,r_csc_coef4, GET_BITS(p_co[4],0,15),\
												r_csc_coef5, GET_BITS(p_co[5],0,15));
			PE_PE1_M14B0_QWr02(pe1_ocsc_ctrl_03,r_csc_coef6, GET_BITS(p_co[6],0,15),\
												r_csc_coef7, GET_BITS(p_co[7],0,15));
			PE_PE1_M14B0_QWr02(pe1_ocsc_ctrl_04,r_csc_coef8, GET_BITS(p_co[8],0,15),\
												r_csc_ofst0, GET_BITS(p_of[0],0,11));
			PE_PE1_M14B0_QWr02(pe1_ocsc_ctrl_05,r_csc_ofst1, GET_BITS(p_of[1],0,11),\
												r_csc_ofst2, GET_BITS(p_of[2],0,11));
			PE_PE1_M14B0_QWr02(pe1_ocsc_ctrl_06,r_csc_ofst3, GET_BITS(p_of[3],0,11),\
												r_csc_ofst4, GET_BITS(p_of[4],0,11));
			PE_PE1_M14B0_QWr01(pe1_ocsc_ctrl_07,r_csc_ofst5, GET_BITS(p_of[5],0,11));
			PE_CSC_HW_M14_DBG_PRINT("[set] post type:%d\n", type);
			PE_CSC_HW_M14_DBG_PRINT("[set] co:%x,%x,%x,%x,%x,%x,%x,%x,%x\n", \
				p_co[0], p_co[1], p_co[2], p_co[3], p_co[4], p_co[5], p_co[6], p_co[7], p_co[8]);
			PE_CSC_HW_M14_DBG_PRINT("[set] of:%x,%x,%x,%x,%x,%x\n", \
				p_of[0], p_of[1], p_of[2], p_of[3], p_of[4], p_of[5]);
		}
		else
		{
			PE_CSC_HW_M14_DBG_PRINT("nothing to do\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}

