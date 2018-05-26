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

/** @file pe_csc_hw_h14.c
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

#include "pe_hw_h14.h"
#include "pe_reg_h14.h"
#include "pe_fwi_h14.h"
#include "pe_csc_hw_h14.h"



/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define PE_CSC_HW_H14_ERROR	printk
#define PE_CSC_HW_H14_DBG_PRINT(fmt,args...)	\
	if(_g_csc_hw_h14_trace) printk("[%x,%x][%s,%d] "fmt,PE_CHIP_VER,g_pe_kdrv_ver_mask,__F__,__L__,##args)
#define PE_CSC_HW_H14_CHECK_CODE(_checker,_action,fmt,args...)	\
	if(_checker){PE_CSC_HW_H14_ERROR(fmt,##args);_action;}

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

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
static UINT32 _g_csc_hw_h14_trace=0x0;		//default should be off.

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
int PE_CSC_HW_H14_Init(PE_CFG_CTRL_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H14)
		{
			PE_CSC_HW_H14_DBG_PRINT("init csc.\n");
			PE_P1L_H14_QWr05(pe1_xvy_ctrl_00,	outputcsc_en,		0x1, \
												degamma_en,			0x1, \
												pcc_en,				0x1, \
												gamma_en,			0x1, \
												hsv_hsl_selection,	0x0);
		}
		else
		{
			PE_CSC_HW_H14_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
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
int PE_CSC_HW_H14_SetDebugSettings(LX_PE_DBG_SETTINGS_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		PE_CSC_HW_H14_DBG_PRINT("wid:%d,type:0x%x,[0x%x]print:0x%x,bypass:0x%x\n",\
			pstParams->win_id,pstParams->type,(0x1<<PE_ITEM_PKTMODL(CSC)),\
			pstParams->print_lvl,pstParams->bypass);
		/* set print level */
		if(pstParams->type&LX_PE_DBG_LV)
		{
			_g_csc_hw_h14_trace = (pstParams->print_lvl & (0x1<<PE_ITEM_PKTMODL(CSC)))? 0x1:0x0;
		}
		/* set bypass */
		if(pstParams->type&LX_PE_DBG_BY)
		{
			if(PE_KDRV_VER_H14)
			{
				if(pstParams->bypass & (0x1<<PE_ITEM_PKTMODL(CSC)))
				{
					PE_CSC_HW_H14_DBG_PRINT("degamma,pcc,gamma,s scaler : disable.\n");
					if(PE_CHECK_WIN0(pstParams->win_id))
					{
						PE_P1L_H14_QWr04(pe1_xvy_ctrl_00, degamma_en,					0x0, \
															pcc_en,						0x0, \
															gamma_en,					0x0, \
															s_adpative_scaler_enable,	0x0);
					}
				}
				else
				{
					PE_CSC_HW_H14_DBG_PRINT("outcsc,degamma,pcc,gamma : enable.\n");
					if(PE_CHECK_WIN0(pstParams->win_id))
					{
						PE_P1L_H14_QWr03(pe1_xvy_ctrl_00, degamma_en,			0x1, \
															pcc_en,				0x1, \
															gamma_en,			0x1);
					}
				}
			}
			else
			{
				PE_CSC_HW_H14_DBG_PRINT("do nothing.\n");	ret = RET_OK;
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
int PE_CSC_HW_H14_SetColorGamut(LX_PE_CSC_GAMUT_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		PE_CSC_HW_H14_DBG_PRINT("set pstParams[%d] : 0x%x,0x%x,0x%x, 0x%x,0x%x,0x%x, 0x%x,0x%x,0x%x\n",\
			pstParams->win_id,pstParams->matrix[0],pstParams->matrix[1],pstParams->matrix[2],\
			pstParams->matrix[3],pstParams->matrix[4],pstParams->matrix[5],pstParams->matrix[6],\
			pstParams->matrix[7],pstParams->matrix[8]);
		if(PE_KDRV_VER_H14)
		{
			if(PE_CHECK_WIN0(pstParams->win_id))
			{
				PE_P1L_H14_QWr01(pe1_xvy_ctrl_08,	r_pcc_coef0, GET_BITS(pstParams->matrix[0],0,15));
				PE_P1L_H14_QWr02(pe1_xvy_ctrl_09,	r_pcc_coef1, GET_BITS(pstParams->matrix[1],0,15),\
													r_pcc_coef2, GET_BITS(pstParams->matrix[2],0,15));
				PE_P1L_H14_QWr02(pe1_xvy_ctrl_0a,	r_pcc_coef3, GET_BITS(pstParams->matrix[3],0,15),\
													r_pcc_coef4, GET_BITS(pstParams->matrix[4],0,15));
				PE_P1L_H14_QWr02(pe1_xvy_ctrl_0b,	r_pcc_coef5, GET_BITS(pstParams->matrix[5],0,15),\
													r_pcc_coef6, GET_BITS(pstParams->matrix[6],0,15));
				PE_P1L_H14_QWr02(pe1_xvy_ctrl_0c,	r_pcc_coef7, GET_BITS(pstParams->matrix[7],0,15),\
													r_pcc_coef8, GET_BITS(pstParams->matrix[8],0,15));
			}
		}
		else
		{
			PE_CSC_HW_H14_DBG_PRINT("nothing to do\n");	ret = RET_OK;
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
int PE_CSC_HW_H14_GetColorGamut(LX_PE_CSC_GAMUT_T *pstParams)
{
	int ret = RET_OK;
	LX_PE_WIN_ID win_id;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		win_id = PE_GET_CHECKED_WINID(pstParams->win_id);
		if(PE_KDRV_VER_H14)
		{
			if(PE_CHECK_WIN0(win_id))
			{
				PE_P1L_H14_QRd01(pe1_xvy_ctrl_08,	r_pcc_coef0, pstParams->matrix[0]);
				PE_P1L_H14_QRd02(pe1_xvy_ctrl_09,	r_pcc_coef1, pstParams->matrix[1],\
													r_pcc_coef2, pstParams->matrix[2]);
				PE_P1L_H14_QRd02(pe1_xvy_ctrl_0a,	r_pcc_coef3, pstParams->matrix[3],\
													r_pcc_coef4, pstParams->matrix[4]);
				PE_P1L_H14_QRd02(pe1_xvy_ctrl_0b,	r_pcc_coef5, pstParams->matrix[5],\
													r_pcc_coef6, pstParams->matrix[6]);
				PE_P1L_H14_QRd02(pe1_xvy_ctrl_0c,	r_pcc_coef7, pstParams->matrix[7],\
													r_pcc_coef8, pstParams->matrix[8]);
			}
		}
		else
		{
			PE_CSC_HW_H14_DBG_PRINT("nothing to do\n");	ret = RET_OK;
		}
		PE_CSC_HW_H14_DBG_PRINT("get pstParams[%d] : 0x%x,0x%x,0x%x, 0x%x,0x%x,0x%x, 0x%x,0x%x,0x%x\n",\
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
int PE_CSC_HW_H14_SetPostCsc(LX_PE_CSC_POST_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		PE_CSC_HW_H14_DBG_PRINT("set pstParams[%d] : \n"\
			"coef:%x,%x,%x, %x,%x,%x, %x,%x,%x, ofs:%x,%x,%x, %x,%x,%x\n",\
			pstParams->win_id,pstParams->matrix[0],pstParams->matrix[1],pstParams->matrix[2],\
			pstParams->matrix[3],pstParams->matrix[4],pstParams->matrix[5],pstParams->matrix[6],\
			pstParams->matrix[7],pstParams->matrix[8],pstParams->offset[0],pstParams->offset[1],\
			pstParams->offset[2],pstParams->offset[3],pstParams->offset[4],pstParams->offset[5]);
		if(PE_KDRV_VER_H14)
		{
			if(PE_CHECK_WIN0(pstParams->win_id))
			{
				PE_P1L_H14_QWr02(pe1_xvy_ctrl_01,	r_csc_coef0, GET_BITS(pstParams->matrix[0],0,15),\
													r_csc_coef1, GET_BITS(pstParams->matrix[1],0,15));
				PE_P1L_H14_QWr02(pe1_xvy_ctrl_02,	r_csc_coef2, GET_BITS(pstParams->matrix[2],0,15),\
													r_csc_coef3, GET_BITS(pstParams->matrix[3],0,15));
				PE_P1L_H14_QWr02(pe1_xvy_ctrl_03,	r_csc_coef4, GET_BITS(pstParams->matrix[4],0,15),\
													r_csc_coef5, GET_BITS(pstParams->matrix[5],0,15));
				PE_P1L_H14_QWr02(pe1_xvy_ctrl_04,	r_csc_coef6, GET_BITS(pstParams->matrix[6],0,15),\
													r_csc_coef7, GET_BITS(pstParams->matrix[7],0,15));
				PE_P1L_H14_QWr02(pe1_xvy_ctrl_05,	r_csc_coef8, GET_BITS(pstParams->matrix[8],0,15),\
													r_csc_ofst0, GET_BITS(pstParams->offset[0],0,11));
				PE_P1L_H14_QWr02(pe1_xvy_ctrl_06,	r_csc_ofst1, GET_BITS(pstParams->offset[1],0,11),\
													r_csc_ofst2, GET_BITS(pstParams->offset[2],0,11));
				PE_P1L_H14_QWr02(pe1_xvy_ctrl_07,	r_csc_ofst3, GET_BITS(pstParams->offset[3],0,14),\
													r_csc_ofst4, GET_BITS(pstParams->offset[4],0,14));
				PE_P1L_H14_QWr01(pe1_xvy_ctrl_08,	r_csc_ofst5, GET_BITS(pstParams->offset[5],0,14));
			}
		}
		else
		{
			PE_CSC_HW_H14_DBG_PRINT("nothing to do\n");	ret = RET_OK;
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
int PE_CSC_HW_H14_GetPostCsc(LX_PE_CSC_POST_T *pstParams)
{
	int ret = RET_OK;
	LX_PE_WIN_ID win_id;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		win_id = PE_GET_CHECKED_WINID(pstParams->win_id);
		if(PE_KDRV_VER_H14)
		{
			if(PE_CHECK_WIN0(win_id))
			{
				PE_P1L_H14_QRd02(pe1_xvy_ctrl_01,	r_csc_coef0, pstParams->matrix[0],\
													r_csc_coef1, pstParams->matrix[1]);
				PE_P1L_H14_QRd02(pe1_xvy_ctrl_02,	r_csc_coef2, pstParams->matrix[2],\
													r_csc_coef3, pstParams->matrix[3]);
				PE_P1L_H14_QRd02(pe1_xvy_ctrl_03,	r_csc_coef4, pstParams->matrix[4],\
													r_csc_coef5, pstParams->matrix[5]);
				PE_P1L_H14_QRd02(pe1_xvy_ctrl_04,	r_csc_coef6, pstParams->matrix[6],\
													r_csc_coef7, pstParams->matrix[7]);
				PE_P1L_H14_QRd02(pe1_xvy_ctrl_05,	r_csc_coef8, pstParams->matrix[8],\
													r_csc_ofst0, pstParams->offset[0]);
				PE_P1L_H14_QRd02(pe1_xvy_ctrl_06,	r_csc_ofst1, pstParams->offset[1],\
													r_csc_ofst2, pstParams->offset[2]);
				PE_P1L_H14_QRd02(pe1_xvy_ctrl_07,	r_csc_ofst3, pstParams->offset[3],\
													r_csc_ofst4, pstParams->offset[4]);
				PE_P1L_H14_QRd01(pe1_xvy_ctrl_08,	r_csc_ofst5, pstParams->offset[5]);
			}
		}
		else
		{
			PE_CSC_HW_H14_DBG_PRINT("nothing to do\n");	ret = RET_OK;
		}
		PE_CSC_HW_H14_DBG_PRINT("get pstParams[%d] : \n"\
			"coef:%x,%x,%x, %x,%x,%x, %x,%x,%x, ofs:%x,%x,%x, %x,%x,%x\n",\
			pstParams->win_id,pstParams->matrix[0],pstParams->matrix[1],pstParams->matrix[2],\
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
int PE_CSC_HW_H14_SetInputCsc(LX_PE_CSC_INPUT_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		PE_CSC_HW_H14_DBG_PRINT("set pstParams[%d] : en:%d\n"\
			"coef:%x,%x,%x, %x,%x,%x, %x,%x,%x, ofs:%x,%x,%x, %x,%x,%x\n",\
			pstParams->win_id,pstParams->enable,\
			pstParams->matrix[0],pstParams->matrix[1],pstParams->matrix[2],\
			pstParams->matrix[3],pstParams->matrix[4],pstParams->matrix[5],\
			pstParams->matrix[6],pstParams->matrix[7],pstParams->matrix[8],\
			pstParams->offset[0],pstParams->offset[1],pstParams->offset[2],\
			pstParams->offset[3],pstParams->offset[4],pstParams->offset[5]);
		if(PE_KDRV_VER_H14)
		{
			/* If the reg. setting has a transition artifact, use the firmware setting. */
			#if 0	// reg. setting
			if(PE_CHECK_WIN0(pstParams->win_id))
			{
				PE_CVA_H14_RdFL(cva_csc_ctrl0);
				PE_CVA_H14_RdFL(cva_csc_ctrl1);
				PE_CVA_H14_RdFL(cva_csc_ctrl2);
				PE_CVA_H14_RdFL(cva_csc_ctrl3);
				PE_CVA_H14_RdFL(cva_csc_ctrl4);
				PE_CVA_H14_RdFL(cva_csc_ctrl5);
				PE_CVA_H14_RdFL(cva_csc_ctrl6);
				PE_CVA_H14_RdFL(cva_csc_ctrl7);
				PE_CVA_H14_Wr01(cva_csc_ctrl0, csc_coefficient0,	GET_BITS(pstParams->matrix[0],0,15));
				PE_CVA_H14_Wr01(cva_csc_ctrl0, csc_coefficient1,	GET_BITS(pstParams->matrix[1],0,15));
				PE_CVA_H14_Wr01(cva_csc_ctrl1, csc_coefficient2,	GET_BITS(pstParams->matrix[2],0,15));
				PE_CVA_H14_Wr01(cva_csc_ctrl1, csc_coefficient3,	GET_BITS(pstParams->matrix[3],0,15));
				PE_CVA_H14_Wr01(cva_csc_ctrl2, csc_coefficient4,	GET_BITS(pstParams->matrix[4],0,15));
				PE_CVA_H14_Wr01(cva_csc_ctrl2, csc_coefficient5,	GET_BITS(pstParams->matrix[5],0,15));
				PE_CVA_H14_Wr01(cva_csc_ctrl3, csc_coefficient6,	GET_BITS(pstParams->matrix[6],0,15));
				PE_CVA_H14_Wr01(cva_csc_ctrl3, csc_coefficient7,	GET_BITS(pstParams->matrix[7],0,15));
				PE_CVA_H14_Wr01(cva_csc_ctrl4, csc_coefficient8,	GET_BITS(pstParams->matrix[8],0,15));
				PE_CVA_H14_Wr01(cva_csc_ctrl4, csc_offset0,		GET_BITS(pstParams->offset[0],0,11));
				PE_CVA_H14_Wr01(cva_csc_ctrl5, csc_offset1,		GET_BITS(pstParams->offset[1],0,11));
				PE_CVA_H14_Wr01(cva_csc_ctrl5, csc_offset2,		GET_BITS(pstParams->offset[2],0,11));
				PE_CVA_H14_Wr01(cva_csc_ctrl6, csc_offset3,		GET_BITS(pstParams->offset[3],0,11));
				PE_CVA_H14_Wr01(cva_csc_ctrl6, csc_offset4,		GET_BITS(pstParams->offset[4],0,11));
				PE_CVA_H14_Wr01(cva_csc_ctrl7, csc_offset5,		GET_BITS(pstParams->offset[5],0,11));
				PE_CVA_H14_Wr01(cva_csc_ctrl7, csc_en,			GET_BITS(pstParams->enable,0,1));
				PE_CVA_H14_WrFL(cva_csc_ctrl0);
				PE_CVA_H14_WrFL(cva_csc_ctrl1);
				PE_CVA_H14_WrFL(cva_csc_ctrl2);
				PE_CVA_H14_WrFL(cva_csc_ctrl3);
				PE_CVA_H14_WrFL(cva_csc_ctrl4);
				PE_CVA_H14_WrFL(cva_csc_ctrl5);
				PE_CVA_H14_WrFL(cva_csc_ctrl6);
				PE_CVA_H14_WrFL(cva_csc_ctrl7);
				if(pstParams->enable)
				{
					PE_CVA_H14_Wr01(cva_csc_ctrl7, csc_coeff_load,	0x1);	//this bit is auto cleared
					PE_CVA_H14_WrFL(cva_csc_ctrl7);
				}
			}
			if(PE_CHECK_WIN1(pstParams->win_id))
			{
				PE_CVB_H14_RdFL(cvb_csc_ctrl0);
				PE_CVB_H14_RdFL(cvb_csc_ctrl1);
				PE_CVB_H14_RdFL(cvb_csc_ctrl2);
				PE_CVB_H14_RdFL(cvb_csc_ctrl3);
				PE_CVB_H14_RdFL(cvb_csc_ctrl4);
				PE_CVB_H14_RdFL(cvb_csc_ctrl5);
				PE_CVB_H14_RdFL(cvb_csc_ctrl6);
				PE_CVB_H14_RdFL(cvb_csc_ctrl7);
				PE_CVB_H14_Wr01(cvb_csc_ctrl0, csc_coefficient0,	GET_BITS(pstParams->matrix[0],0,15));
				PE_CVB_H14_Wr01(cvb_csc_ctrl0, csc_coefficient1,	GET_BITS(pstParams->matrix[1],0,15));
				PE_CVB_H14_Wr01(cvb_csc_ctrl1, csc_coefficient2,	GET_BITS(pstParams->matrix[2],0,15));
				PE_CVB_H14_Wr01(cvb_csc_ctrl1, csc_coefficient3,	GET_BITS(pstParams->matrix[3],0,15));
				PE_CVB_H14_Wr01(cvb_csc_ctrl2, csc_coefficient4,	GET_BITS(pstParams->matrix[4],0,15));
				PE_CVB_H14_Wr01(cvb_csc_ctrl2, csc_coefficient5,	GET_BITS(pstParams->matrix[5],0,15));
				PE_CVB_H14_Wr01(cvb_csc_ctrl3, csc_coefficient6,	GET_BITS(pstParams->matrix[6],0,15));
				PE_CVB_H14_Wr01(cvb_csc_ctrl3, csc_coefficient7,	GET_BITS(pstParams->matrix[7],0,15));
				PE_CVB_H14_Wr01(cvb_csc_ctrl4, csc_coefficient8,	GET_BITS(pstParams->matrix[8],0,15));
				PE_CVB_H14_Wr01(cvb_csc_ctrl4, csc_offset0,		GET_BITS(pstParams->offset[0],0,11));
				PE_CVB_H14_Wr01(cvb_csc_ctrl5, csc_offset1,		GET_BITS(pstParams->offset[1],0,11));
				PE_CVB_H14_Wr01(cvb_csc_ctrl5, csc_offset2,		GET_BITS(pstParams->offset[2],0,11));
				PE_CVB_H14_Wr01(cvb_csc_ctrl6, csc_offset3,		GET_BITS(pstParams->offset[3],0,11));
				PE_CVB_H14_Wr01(cvb_csc_ctrl6, csc_offset4,		GET_BITS(pstParams->offset[4],0,11));
				PE_CVB_H14_Wr01(cvb_csc_ctrl7, csc_offset5,		GET_BITS(pstParams->offset[5],0,11));
				PE_CVB_H14_Wr01(cvb_csc_ctrl7, csc_en,			GET_BITS(pstParams->enable,0,1));
				PE_CVB_H14_WrFL(cvb_csc_ctrl0);
				PE_CVB_H14_WrFL(cvb_csc_ctrl1);
				PE_CVB_H14_WrFL(cvb_csc_ctrl2);
				PE_CVB_H14_WrFL(cvb_csc_ctrl3);
				PE_CVB_H14_WrFL(cvb_csc_ctrl4);
				PE_CVB_H14_WrFL(cvb_csc_ctrl5);
				PE_CVB_H14_WrFL(cvb_csc_ctrl6);
				PE_CVB_H14_WrFL(cvb_csc_ctrl7);
				if(pstParams->enable)
				{
					PE_CVB_H14_Wr01(cvb_csc_ctrl7, csc_coeff_load,	0x1);	//this bit is auto cleared
					PE_CVB_H14_WrFL(cvb_csc_ctrl7);
				}
			}
			#else	// firmware setting
			LX_PE_WIN_ID win_id = pstParams->win_id;
			if(PE_CHECK_WIN0(win_id))
			{
				pstParams->win_id = LX_PE_WIN_0;
				ret = PE_FWI_H14_SetCviCsc(pstParams);
				PE_CSC_HW_H14_CHECK_CODE(ret,break,"[%s,%d] PE_FWI_H14_SetCviCsc() error.\n",__F__,__L__);
			}
			if(PE_CHECK_WIN1(win_id))
			{
				pstParams->win_id = LX_PE_WIN_1;
				ret = PE_FWI_H14_SetCviCsc(pstParams);
				PE_CSC_HW_H14_CHECK_CODE(ret,break,"[%s,%d] PE_FWI_H14_SetCviCsc() error.\n",__F__,__L__);
			}
			#endif
		}
		else
		{
			PE_CSC_HW_H14_DBG_PRINT("nothing to do\n");	ret = RET_OK;
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
int PE_CSC_HW_H14_GetInputCsc(LX_PE_CSC_INPUT_T *pstParams)
{
	int ret = RET_OK;
	LX_PE_WIN_ID win_id;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		win_id = PE_GET_CHECKED_WINID(pstParams->win_id);
		if(PE_KDRV_VER_H14)
		{
			if(PE_CHECK_WIN0(win_id))
			{
				PE_CVA_H14_QRd02(cva_csc_ctrl0,	csc_coefficient0,	pstParams->matrix[0],\
												csc_coefficient1,	pstParams->matrix[1]);
				PE_CVA_H14_QRd02(cva_csc_ctrl1,	csc_coefficient2,	pstParams->matrix[2],\
												csc_coefficient3,	pstParams->matrix[3]);
				PE_CVA_H14_QRd02(cva_csc_ctrl2,	csc_coefficient4,	pstParams->matrix[4],\
												csc_coefficient5,	pstParams->matrix[5]);
				PE_CVA_H14_QRd02(cva_csc_ctrl3,	csc_coefficient6,	pstParams->matrix[6],\
												csc_coefficient7,	pstParams->matrix[7]);
				PE_CVA_H14_QRd02(cva_csc_ctrl4,	csc_coefficient8,	pstParams->matrix[8],\
												csc_offset0,		pstParams->offset[0]);
				PE_CVA_H14_QRd02(cva_csc_ctrl5,	csc_offset1,		pstParams->offset[1],\
												csc_offset2,		pstParams->offset[2]);
				PE_CVA_H14_QRd02(cva_csc_ctrl6,	csc_offset3,		pstParams->offset[3],\
												csc_offset4,		pstParams->offset[4]);
				PE_CVA_H14_QRd02(cva_csc_ctrl7,	csc_offset5,		pstParams->offset[5],\
												csc_en,				pstParams->enable);
			}
			if(PE_CHECK_WIN1(win_id))
			{
				PE_CVB_H14_QRd02(cvb_csc_ctrl0,	csc_coefficient0,	pstParams->matrix[0],\
												csc_coefficient1,	pstParams->matrix[1]);
				PE_CVB_H14_QRd02(cvb_csc_ctrl1,	csc_coefficient2,	pstParams->matrix[2],\
												csc_coefficient3,	pstParams->matrix[3]);
				PE_CVB_H14_QRd02(cvb_csc_ctrl2,	csc_coefficient4,	pstParams->matrix[4],\
												csc_coefficient5,	pstParams->matrix[5]);
				PE_CVB_H14_QRd02(cvb_csc_ctrl3,	csc_coefficient6,	pstParams->matrix[6],\
												csc_coefficient7,	pstParams->matrix[7]);
				PE_CVB_H14_QRd02(cvb_csc_ctrl4,	csc_coefficient8,	pstParams->matrix[8],\
												csc_offset0,		pstParams->offset[0]);
				PE_CVB_H14_QRd02(cvb_csc_ctrl5,	csc_offset1,		pstParams->offset[1],\
												csc_offset2,		pstParams->offset[2]);
				PE_CVB_H14_QRd02(cvb_csc_ctrl6,	csc_offset3,		pstParams->offset[3],\
												csc_offset4,		pstParams->offset[4]);
				PE_CVB_H14_QRd02(cvb_csc_ctrl7,	csc_offset5,		pstParams->offset[5],\
												csc_en,				pstParams->enable);
			}
		}
		else
		{
			PE_CSC_HW_H14_DBG_PRINT("nothing to do\n");	ret = RET_OK;
		}
		PE_CSC_HW_H14_DBG_PRINT("get pstParams[%d] : en:%d\n"\
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
int PE_CSC_HW_H14_SetxvYCC(LX_PE_CSC_XVYCC_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		PE_CSC_HW_H14_DBG_PRINT("set pstParams[%d] : en:%d, sclr:0x%x\n",\
			pstParams->win_id,pstParams->enable,pstParams->scaler);
		if(PE_KDRV_VER_H14)
		{
			if(PE_CHECK_WIN0(pstParams->win_id))
			{
				PE_P1L_H14_QWr01(pe1_xvy_ctrl_00, scaler_en,	(pstParams->enable)? 0x1:0x0);
				PE_P1L_H14_QWr01(pe1_xvy_ctrl_0d, scaler,		GET_BITS(pstParams->scaler,0,8));
			}
		}
		else
		{
			PE_CSC_HW_H14_DBG_PRINT("nothing to do\n");	ret = RET_OK;
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
int PE_CSC_HW_H14_GetxvYCC(LX_PE_CSC_XVYCC_T *pstParams)
{
	int ret = RET_OK;
	LX_PE_WIN_ID win_id;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		win_id = PE_GET_CHECKED_WINID(pstParams->win_id);
		if(PE_KDRV_VER_H14)
		{
			if(PE_CHECK_WIN0(win_id))
			{
				PE_P1L_H14_QRd01(pe1_xvy_ctrl_00, scaler_en,	pstParams->enable);
				PE_P1L_H14_QRd01(pe1_xvy_ctrl_0d, scaler,		pstParams->scaler);
			}
		}
		else
		{
			PE_CSC_HW_H14_DBG_PRINT("nothing to do\n");	ret = RET_OK;
		}
		PE_CSC_HW_H14_DBG_PRINT("get pstParams[%d] : en:%d, sclr:0x%x\n",\
			pstParams->win_id,pstParams->enable,pstParams->scaler);
	}while(0);
	return ret;
}

