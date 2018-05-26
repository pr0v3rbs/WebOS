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

/** @file pe_dcm_hw_h14.c
 *
 *  driver for picture enhance dynamic contrast module functions. ( used only within kdriver )
 *	- dynamic contrast enhancement, histogram information and configuration.
 *	
 *	@author		Seung-Jun,Youm(sj.youm@lge.com)
 *	@version	0.1
 *	@note		
 *	@date		2012.04.16
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
#include "pe_dcm_hw_h14.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define PE_DCM_HW_H14_ERROR	printk

#define PE_DCM_HW_H14_DBG_PRINT(fmt,args...)	\
	if(_g_dcm_hw_h14_trace) printk("[%x,%x][%s,%d] "fmt,PE_CHIP_VER,g_pe_kdrv_ver_mask,__F__,__L__,##args)
		
#define PE_DCM_HW_H14_CHECK_CODE(_checker,_action,fmt,args...) \
	{if(_checker){PE_DCM_HW_H14_ERROR(fmt,##args);_action;}}

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
static UINT32 _g_dcm_hw_h14_trace=0x0;	//default should be off.
static PE_DCM_HW_H14_SETTINGS_T _g_pe_dcm_hw_h14_info;

/*========================================================================================
	Implementation Group
========================================================================================*/
/**
 * init dynamic contrast module
 *
 * @param   *pstParams [in] PE_CFG_CTRL_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_DCM_HW_H14_Init(PE_CFG_CTRL_T *pstParams)
{
	int ret = RET_OK;
	UINT32 count = 0;
	LX_PE_DCM_DCE_CONF_T stParams;
	UINT32 dce_lut_data[32] = {\
		   0,  64, 122, 181, 239, 298, 356, 415, 473, \
		 532, 590, 649, 707, 766, 824, 882, 940,1023, \
		1023,1023,1023,1023,1023,1023,1023,1023,1023, \
		1023,1023,1023,1023,1023};
	UINT32 dse_lut_data[32] = {\
		   0,  64, 128, 192, 256, 320, 384, 448, 512, \
		 576, 640, 704, 768, 832, 896, 960,1023,1023, \
		1023,1023,1023,1023,1023,1023,1023,1023,1023, \
		1023,1023,1023,1023,1023};
	do{
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H14)
		{
			PE_DCM_HW_H14_DBG_PRINT("init histo info.\n");
			memset0(_g_pe_dcm_hw_h14_info);
			_g_pe_dcm_hw_h14_info.cur_blur_gain = 0xf;
			_g_pe_dcm_hw_h14_info.pre_blur_gain = 0xf;
			PE_P1L_H14_RdFL(pe1_dce_hist_ia_ctrl);
			PE_P1L_H14_Wr(pe1_dce_hist_ia_ctrl,	0x8000);	//0:read mode, 1:normal mode 
			PE_P1L_H14_WrFL(pe1_dce_hist_ia_ctrl);
			/* set blur_v_gain 0xf temporally */
			PE_P1L_H14_QWr01(pe1_dce_ctrl_01,		blur_v_gain,	0xf);	//0x0:blur V ~ 0xf:original V
			PE_DCM_HW_H14_DBG_PRINT("init dce config\n");
			/* synchronization with pe1_win2_ctrl_00 win_en(0,1,01) aettings in win.c */
			PE_P1L_H14_QWr03(pe1_dce_ctrl_00,		window0_enable,		0x1,\
													window1_enable,		0x1,\
													window01_enable,	0x1);
			PE_P1L_H14_QWr03(pe1_dse_ctrl_00,		window0_enable,		0x1,\
													window1_enable,		0x1,\
													window01_enable,	0x1);
			PE_P1L_H14_QWr02(pe1_dce_ctrl_09,		dce_ktd_csc_en,		0x1,\
													dce_ktd_csc_sel,	0x0);
			stParams.domain = LX_PE_YC_DOMAIN;
			stParams.min_pos = 26;
			stParams.max_pos = 220;
			stParams.win_id = LX_PE_WIN_ALL;
			ret = PE_DCM_HW_H14_SetDceConfig(&stParams);
			PE_DCM_HW_H14_CHECK_CODE(ret, break, \
				"[%s,%d][win all] PE_DCM_HW_H14_SetDceConfig() error.\n",__F__,__L__);
			/* dce lut init */
			PE_P1L_H14_RdFL(pe1_dce_ia_ctrl);
			PE_P1L_H14_RdFL(pe1_dce_ctrl_08);
			PE_P1L_H14_Wr01(pe1_dce_ia_ctrl,	hif_dce_enable,		0x0);	//0:host access, 1:normal mode
			PE_P1L_H14_Wr01(pe1_dce_ia_ctrl,	hif_dce_ai,			0x1);	//ai 0:disable, 1:enable
			PE_P1L_H14_Wr01(pe1_dce_ia_ctrl,	hif_dyc_address,	0x0);	//address
			PE_P1L_H14_WrFL(pe1_dce_ia_ctrl);
			for(count=0;count<32;count++)
			{
				PE_P1L_H14_Wr01(pe1_dce_ia_data,	hif_dyc_wdata_y,dce_lut_data[count]);	//y data
				PE_P1L_H14_Wr01(pe1_dce_ia_data,	hif_dyc_wdata_x,dce_lut_data[count]);	//x data
				PE_P1L_H14_WrFL(pe1_dce_ia_data);
			}
			PE_P1L_H14_Wr01(pe1_dce_ctrl_08,	hif_dyc_wdata_y_32nd,	1023);	//y data
			PE_P1L_H14_Wr01(pe1_dce_ctrl_08,	hif_dyc_wdata_x_32nd,	1023);	//x data
			PE_P1L_H14_WrFL(pe1_dce_ctrl_08);
			PE_P1L_H14_Wr01(pe1_dce_ia_ctrl,	hif_dce_load,	 	0x1);	//0:clear, 1:enable
			PE_P1L_H14_Wr01(pe1_dce_ia_ctrl,	hif_dce_enable,		0x1);		//0:host access, 1:normal mode
			PE_P1L_H14_WrFL(pe1_dce_ia_ctrl);
			/* dse lut init */
			PE_P1L_H14_RdFL(pe1_dse_ia_ctrl);
			PE_P1L_H14_RdFL(pe1_dse_ctrl_02);
			PE_P1L_H14_Wr01(pe1_dse_ia_ctrl,	hif_dse_enable,		0x0);	//0:host access, 1:normal mode
			PE_P1L_H14_Wr01(pe1_dse_ia_ctrl,	hif_dse_ai,			0x1);	//ai 0:disable, 1:enable
			PE_P1L_H14_Wr01(pe1_dse_ia_ctrl,	hif_dse_address,	0x0);	//address
			PE_P1L_H14_Wr01(pe1_dse_ia_ctrl,	hif_dse_load,	 	0x0);		//0:clear, 1:enable
			PE_P1L_H14_WrFL(pe1_dse_ia_ctrl);
			for(count=0;count<32;count++)
			{
				PE_P1L_H14_Wr01(pe1_dse_ia_data,	hif_dse_wdata_y,dse_lut_data[count]);	//y data
				PE_P1L_H14_Wr01(pe1_dse_ia_data,	hif_dse_wdata_x,dse_lut_data[count]);	//x data
				PE_P1L_H14_WrFL(pe1_dse_ia_data);
			}
			PE_P1L_H14_Wr01(pe1_dse_ctrl_02,	hif_dse_wdata_y_32nd,	1023);	//y data
			PE_P1L_H14_Wr01(pe1_dse_ctrl_02,	hif_dse_wdata_x_32nd,	1023);	//x data
			PE_P1L_H14_WrFL(pe1_dse_ctrl_02);
			PE_P1L_H14_Wr01(pe1_dse_ia_ctrl,	hif_dse_load,	 	0x1);		//0:clear, 1:enable
			PE_P1L_H14_Wr01(pe1_dse_ia_ctrl,	hif_dse_enable,		0x1);		//0:host access, 1:normal mode
			PE_P1L_H14_WrFL(pe1_dse_ia_ctrl);
		}
		else
		{
			PE_DCM_HW_H14_DBG_PRINT("nothing to do\n");	ret = RET_OK;
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
int PE_DCM_HW_H14_SetDebugSettings(LX_PE_DBG_SETTINGS_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		PE_DCM_HW_H14_DBG_PRINT("wid:%d,type:0x%x,[0x%x]print:0x%x,bypass:0x%x\n",\
			pstParams->win_id,pstParams->type,(0x1<<PE_ITEM_PKTMODL(DCM)),\
			pstParams->print_lvl,pstParams->bypass);
		/* set print level */
		if(pstParams->type&LX_PE_DBG_LV)
		{
			_g_dcm_hw_h14_trace = (pstParams->print_lvl & (0x1<<PE_ITEM_PKTMODL(DCM)))? 0x1:0x0;
		}
		/* set bypass */
		if(pstParams->type&LX_PE_DBG_BY)
		{
			if(PE_KDRV_VER_H14)
			{
				if(pstParams->bypass & (0x1<<PE_ITEM_PKTMODL(DCM)))
				{
					PE_DCM_HW_H14_DBG_PRINT("dynamic_contrast_en : disable.\n");
					if(PE_CHECK_WIN0(pstParams->win_id))
					{
						PE_P1L_H14_QWr01(pe1_dce_ctrl_00,	dynamic_contrast_en,	0x0);
					}
				}
				else
				{
					PE_DCM_HW_H14_DBG_PRINT("dynamic_contrast_en : enable.\n");
					if(PE_CHECK_WIN0(pstParams->win_id))
					{
						PE_P1L_H14_QWr01(pe1_dce_ctrl_00,	dynamic_contrast_en,	0x1);
					}
				}
			}
			else
			{
				PE_DCM_HW_H14_DBG_PRINT("nothing to do\n");	ret = RET_OK;
			}
		}
	}while(0);
	return ret;
}
/**
 * set dce config
 *
 * @param   *pstParams [in] LX_PE_DCM_DCE_CONF_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_DCM_HW_H14_SetDceConfig(LX_PE_DCM_DCE_CONF_T *pstParams)
{
	int ret = RET_OK;
	UINT32 domain,min_pos,max_pos;
	do {
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		if(pstParams->domain==LX_PE_YC_DOMAIN)			{domain=1;}	//yc
		else											{domain=0;}	//scaled hsv,LX_PE_HSV_DOMAIN
		min_pos = pstParams->min_pos;
		max_pos = pstParams->max_pos;
		PE_DCM_HW_H14_DBG_PRINT("[win id:%d] domain:%d, min:%d, max:%d\n",\
			pstParams->win_id,pstParams->domain,pstParams->min_pos,pstParams->max_pos);
		if(PE_KDRV_VER_H14)
		{
			if(PE_CHECK_WIN0(pstParams->win_id))
			{
				/* tpg position(related with apl position for yc) */
				PE_P1L_H14_RdFL(pe1_top_ctrl_02);
				PE_P1L_H14_Wr01(pe1_top_ctrl_02,	tpg_positon,	(domain==1)? 0x0:0x1);	//pe1 in(0),out(1)
				PE_P1L_H14_WrFL(pe1_top_ctrl_02);
				/* dce config */
				PE_P1L_H14_RdFL(pe1_dce_ctrl_00);
				PE_P1L_H14_RdFL(pe1_dce_ctrl_01);
				PE_P1L_H14_RdFL(pe1_dce_ia_data);
				PE_P1L_H14_RdFL(pe1_apl_ctrl_02);
				PE_P1L_H14_Wr01(pe1_dce_ctrl_00,	dynamic_contrast_en,	0x1);					//enable
				PE_P1L_H14_Wr01(pe1_dce_ctrl_00,	hist_bin_mode,			0x0);					//32bin mode
				PE_P1L_H14_Wr01(pe1_dce_ctrl_00,	window_acquire,			0x1);					//window acquire on
				PE_P1L_H14_Wr01(pe1_dce_ctrl_00,	dce_domain_sel,			(domain==1)? 0x1:0x0);	//HSV or YC domain
				PE_P1L_H14_Wr01(pe1_dce_ctrl_00,	win_selection,			0x0);					//from pe1 win2
				PE_P1L_H14_Wr01(pe1_dce_ctrl_00,	window_mode_enable,		0x1);					//window sync
				PE_P1L_H14_Wr01(pe1_dce_ctrl_00,	window_inout_sel,		0x0);					//window inout selection by win0,win1
				PE_P1L_H14_Wr01(pe1_dce_ctrl_00,	histogram_mode,			0x0);					//histogram mode, use own side's histo
				PE_P1L_H14_Wr01(pe1_dce_ctrl_01,	hist_bin_min_position,	min_pos);				//min position
				PE_P1L_H14_Wr01(pe1_dce_ctrl_01,	hist_bin_max_position,	max_pos);				//max position
				PE_P1L_H14_Wr01(pe1_apl_ctrl_02,	apl_position,			(domain==1)? 0x0:0x3);	//0x0:PE1 input(Y/C),0x3:xvycc out(RGB)
				PE_P1L_H14_WrFL(pe1_dce_ctrl_00);
				PE_P1L_H14_WrFL(pe1_dce_ctrl_01);
				PE_P1L_H14_WrFL(pe1_apl_ctrl_02);
				/* CEN block ON */
				PE_P1L_H14_RdFL(pe1_cen_ctrl_00);
				PE_P1L_H14_Wr01(pe1_cen_ctrl_00,	demo_mode,		0x0);
				PE_P1L_H14_Wr01(pe1_cen_ctrl_00,	vsp_sel,		(domain==3)? 0x0:0x1);	//0:hsl,1:hsv
				PE_P1L_H14_WrFL(pe1_cen_ctrl_00);
				/****************/
				/* dse config */
				PE_P1L_H14_RdFL(pe1_dse_ctrl_00);
				PE_P1L_H14_Wr01(pe1_dse_ctrl_00,	dynamic_saturation_en,	0x1);
				PE_P1L_H14_Wr01(pe1_dse_ctrl_00,	window_mode_enable,		0x1);	//window sync
				PE_P1L_H14_Wr01(pe1_dse_ctrl_00,	window_inout_sel,		0x0);	//window inout selection by win0,win1
				PE_P1L_H14_Wr01(pe1_dce_ctrl_00,	win_selection,			0x0);	//from pe1 win2
				PE_P1L_H14_Wr01(pe1_dse_ctrl_00,	window_acquire,			0x1);	//window acquire on
				PE_P1L_H14_WrFL(pe1_dse_ctrl_00);
			}		
		}
		else
		{
			PE_DCM_HW_H14_DBG_PRINT("nothing to do\n");	ret = RET_OK;
		}
	}while (0);
	return ret;
}
/**
 * get dce config
 *
 * @param   *pstParams [in/out] LX_PE_DCM_DCE_CONF_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_DCM_HW_H14_GetDceConfig(LX_PE_DCM_DCE_CONF_T *pstParams)
{
	int ret = RET_OK;
	LX_PE_WIN_ID win_id;
	UINT32 domain=0;
	UINT32 min_pos=0;
	UINT32 max_pos=0;
	do {
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		win_id = PE_GET_CHECKED_WINID(pstParams->win_id);
		if(PE_KDRV_VER_H14)
		{
			if(PE_CHECK_WIN0(win_id))
			{
				PE_P1L_H14_RdFL(pe1_dce_ctrl_00);
				PE_P1L_H14_RdFL(pe1_dce_ctrl_01);
				PE_P1L_H14_Rd01(pe1_dce_ctrl_00,	dce_domain_sel,			domain);	//HSV or YC domain
				PE_P1L_H14_Rd01(pe1_dce_ctrl_01,	hist_bin_min_position,	min_pos);	//min position
				PE_P1L_H14_Rd01(pe1_dce_ctrl_01,	hist_bin_max_position,	max_pos);	//max position
				pstParams->domain = domain? LX_PE_YC_DOMAIN:LX_PE_HSV_DOMAIN;
				pstParams->min_pos = min_pos;
				pstParams->max_pos = max_pos;
			}
		}
		else
		{
			PE_DCM_HW_H14_DBG_PRINT("nothing to do\n");	ret = RET_OK;
		}
	}while (0);
	return ret;
}
/**
 * set dce lut
 *
 * @param   *pstParams [in] LX_PE_DCM_DCE_LUT_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_DCM_HW_H14_SetDceLut(LX_PE_DCM_DCE_LUT_T *pstParams)
{
	int ret = RET_OK;
	UINT32 count=0;
	UINT32 lut_data=0;
	do {
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		PE_DCM_HW_H14_CHECK_CODE((pstParams->size>PE_DCM_HW_H14_LUT_MAX), break, \
			"[%s,%d] pstParams->size(%d) is over.\n", __F__,__L__, pstParams->size);
		if(PE_KDRV_VER_H14)
		{
			if(PE_CHECK_WIN0(pstParams->win_id))
			{
				PE_P1L_H14_RdFL(pe1_dce_ia_ctrl);
				PE_P1L_H14_RdFL(pe1_dce_ia_data);
				PE_P1L_H14_Wr01(pe1_dce_ia_ctrl,	hif_dce_enable,		0x0);	//0:host access, 1:normal mode
				PE_P1L_H14_Wr01(pe1_dce_ia_ctrl,	hif_dce_ai,			0x1);	//ai 0:disable, 1:enable
				PE_P1L_H14_Wr01(pe1_dce_ia_ctrl,	hif_dyc_address,	0x0);	//address
				PE_P1L_H14_WrFL(pe1_dce_ia_ctrl);
				for(count=0;count<(pstParams->size);count++)
				{
					lut_data = pstParams->data[count];
					if (count<(PE_DCM_HW_H14_LUT_MAX-1))
					{
						PE_P1L_H14_Wr01(pe1_dce_ia_data,hif_dyc_wdata_y,GET_BITS(lut_data,0,10));//y data
						PE_P1L_H14_Wr01(pe1_dce_ia_data,hif_dyc_wdata_x,GET_BITS(lut_data,16,10));//x data
						PE_P1L_H14_WrFL(pe1_dce_ia_data);
					}
					else	//count == (PE_DCM_HW_H14_LUT_MAX-1)
					{
						PE_P1L_H14_Wr01(pe1_dce_ctrl_08,hif_dyc_wdata_y_32nd,GET_BITS(lut_data,0,10));//y data
						PE_P1L_H14_Wr01(pe1_dce_ctrl_08,hif_dyc_wdata_x_32nd,GET_BITS(lut_data,16,10));//x data
						PE_P1L_H14_WrFL(pe1_dce_ctrl_08);
					}
				}
				PE_P1L_H14_Wr01(pe1_dce_ia_ctrl,	hif_dce_enable, 	0x1);	//0:host access, 1:normal mode
				PE_P1L_H14_Wr01(pe1_dce_ia_ctrl,	hif_dce_load,	 	0x1);	//0:clear, 1:enable
				PE_P1L_H14_WrFL(pe1_dce_ia_ctrl);
			}
		}
		else
		{
			PE_DCM_HW_H14_DBG_PRINT("nothing to do\n");	ret = RET_OK;
		}
	}while (0);
	return ret;
}
/**
 * get dce lut
 *
 * @param   *pstParams [in/out] LX_PE_DCM_DCE_LUT_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_DCM_HW_H14_GetDceLut(LX_PE_DCM_DCE_LUT_T *pstParams)
{
	int ret = RET_OK;
	LX_PE_WIN_ID win_id;
	UINT32 count=0;
	do {
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		PE_DCM_HW_H14_CHECK_CODE((pstParams->size>PE_DCM_HW_H14_LUT_MAX), break, \
			"[%s,%d] pstParams->size(%d) is over.\n", __F__,__L__, pstParams->size);
		win_id = PE_GET_CHECKED_WINID(pstParams->win_id);
		if(PE_KDRV_VER_H14)
		{
			if(PE_CHECK_WIN0(win_id))
			{
				PE_P1L_H14_RdFL(pe1_dce_ia_ctrl);
				PE_P1L_H14_Wr01(pe1_dce_ia_ctrl,	hif_dce_enable, 	0x0);	//0:host access, 1:normal mode
				PE_P1L_H14_Wr01(pe1_dce_ia_ctrl,	hif_dce_ai, 		0x1);	//ai 0:disable, 1:enable
				PE_P1L_H14_Wr01(pe1_dce_ia_ctrl,	hif_dyc_address,	0x0);	//address
				PE_P1L_H14_WrFL(pe1_dce_ia_ctrl);
				for(count=0;count<(pstParams->size);count++)
				{
					if (count<(PE_DCM_HW_H14_LUT_MAX-1))
					{
						PE_P1L_H14_RdFL(pe1_dce_ia_data);
						pstParams->data[count] = PE_P1L_H14_Rd(pe1_dce_ia_data);
					}
					else	//count == (PE_DCM_HW_H14_LUT_MAX-1)
					{
						PE_P1L_H14_RdFL(pe1_dce_ctrl_08);
						pstParams->data[count] = PE_P1L_H14_Rd(pe1_dce_ctrl_08);
					}
				}
				PE_P1L_H14_Wr01(pe1_dce_ia_ctrl,	hif_dce_enable, 	0x1);	//0:host access, 1:normal mode
				PE_P1L_H14_WrFL(pe1_dce_ia_ctrl);
			}
		}
		else
		{
			PE_DCM_HW_H14_DBG_PRINT("nothing to do\n");	ret = RET_OK;
		}
	}while (0);
	return ret;
}
/**
 * set dse lut
 *
 * @param   *pstParams [in] LX_PE_DCM_DSE_LUT_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_DCM_HW_H14_SetDseLut(LX_PE_DCM_DSE_LUT_T *pstParams)
{
	int ret = RET_OK;
	UINT32 count=0;
	UINT32 lut_data=0;
	do {
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		PE_DCM_HW_H14_CHECK_CODE((pstParams->size>PE_DCM_HW_H14_LUT_MAX), break, \
			"[%s,%d] pstParams->size(%d) is over.\n", __F__,__L__, pstParams->size);
		if(PE_KDRV_VER_H14)
		{
			if(PE_CHECK_WIN0(pstParams->win_id))
			{
				PE_P1L_H14_RdFL(pe1_dse_ia_ctrl);
				PE_P1L_H14_RdFL(pe1_dse_ia_data);
				PE_P1L_H14_Wr01(pe1_dse_ia_ctrl,	hif_dse_enable,		0x0);	//0:host access, 1:normal mode
				PE_P1L_H14_Wr01(pe1_dse_ia_ctrl,	hif_dse_ai,			0x1);	//ai 0:disable, 1:enable
				PE_P1L_H14_Wr01(pe1_dse_ia_ctrl,	hif_dse_address,	0x0);	//address
				PE_P1L_H14_Wr01(pe1_dse_ia_ctrl,	hif_dse_load,	 	0x0);	//0:clear, 1:enable
				PE_P1L_H14_WrFL(pe1_dse_ia_ctrl);
				for(count=0;count<(pstParams->size);count++)
				{
					lut_data = pstParams->data[count];
					if (count<(PE_DCM_HW_H14_LUT_MAX-1))
					{
						PE_P1L_H14_Wr01(pe1_dse_ia_data,hif_dse_wdata_y,GET_BITS(lut_data,0,10));//y data
						PE_P1L_H14_Wr01(pe1_dse_ia_data,hif_dse_wdata_x,GET_BITS(lut_data,16,10));//x data
						PE_P1L_H14_WrFL(pe1_dse_ia_data);
					}
					else	//count == (PE_DCM_HW_H14_LUT_MAX-1)
					{
						PE_P1L_H14_Wr01(pe1_dse_ctrl_02,hif_dse_wdata_y_32nd,GET_BITS(lut_data,0,10));//y data
						PE_P1L_H14_Wr01(pe1_dse_ctrl_02,hif_dse_wdata_x_32nd,GET_BITS(lut_data,16,10));//x data
						PE_P1L_H14_WrFL(pe1_dse_ctrl_02);
					}
				}
				PE_P1L_H14_Wr01(pe1_dse_ia_ctrl,	hif_dse_enable, 	0x1);	//0:host access, 1:normal mode
				PE_P1L_H14_Wr01(pe1_dse_ia_ctrl,	hif_dse_load,	 	0x1);	//0:clear, 1:enable
				PE_P1L_H14_WrFL(pe1_dse_ia_ctrl);
			}
		}
		else
		{
			PE_DCM_HW_H14_DBG_PRINT("nothing to do\n");	ret = RET_OK;
		}
	}while (0);
	return ret;
}
/**
 * get dse lut
 *
 * @param   *pstParams [in/out] LX_PE_DCM_DSE_LUT_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_DCM_HW_H14_GetDseLut(LX_PE_DCM_DSE_LUT_T *pstParams)
{
	int ret = RET_OK;
	LX_PE_WIN_ID win_id;
	UINT32 count=0;
	do {
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		PE_DCM_HW_H14_CHECK_CODE((pstParams->size>PE_DCM_HW_H14_LUT_MAX), break, \
			"[%s,%d] pstParams->size(%d) is over.\n", __F__,__L__, pstParams->size);
		win_id = PE_GET_CHECKED_WINID(pstParams->win_id);
		if(PE_KDRV_VER_H14)
		{
			if(PE_CHECK_WIN0(win_id))
			{
				PE_P1L_H14_RdFL(pe1_dse_ia_ctrl);
				PE_P1L_H14_Wr01(pe1_dse_ia_ctrl,	hif_dse_enable,		0x0);	//0:host access, 1:normal mode
				PE_P1L_H14_Wr01(pe1_dse_ia_ctrl,	hif_dse_ai,			0x1);	//ai 0:disable, 1:enable
				PE_P1L_H14_Wr01(pe1_dse_ia_ctrl,	hif_dse_address,	0x0);	//address
				PE_P1L_H14_Wr01(pe1_dse_ia_ctrl,	hif_dse_load,	 	0x0);	//0:clear, 1:enable
				PE_P1L_H14_WrFL(pe1_dse_ia_ctrl);
				for(count=0;count<(pstParams->size);count++)
				{
					if (count<(PE_DCM_HW_H14_LUT_MAX-1))
					{
						PE_P1L_H14_RdFL(pe1_dse_ia_data);
						pstParams->data[count] = PE_P1L_H14_Rd(pe1_dse_ia_data);
					}
					else	//count == (PE_DCM_HW_H14_LUT_MAX-1)
					{
						PE_P1L_H14_RdFL(pe1_dse_ctrl_02);
						pstParams->data[count] = PE_P1L_H14_Rd(pe1_dse_ctrl_02);
					}
				}
				PE_P1L_H14_Wr01(pe1_dse_ia_ctrl,	hif_dse_enable, 	0x1);	//0:host access, 1:normal mode
				PE_P1L_H14_Wr01(pe1_dse_ia_ctrl,	hif_dse_load,	 	0x1);	//0:clear, 1:enable
				PE_P1L_H14_WrFL(pe1_dse_ia_ctrl);
			}
		}
		else
		{
			PE_DCM_HW_H14_DBG_PRINT("nothing to do\n");	ret = RET_OK;
		}
	}while (0);
	return ret;
}
/**
 * set smooth contrast
 *
 * @param   *pstParams [in] void
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_DCM_HW_H14_SetDceSmoothCtrl(void *pstParams)
{
	int ret = RET_OK;
	#ifdef PE_DCM_HW_H14_SET_BLUR_GAIN_ON_TASK
	PE_DCM_HW_H14_SETTINGS_T *pInfo=&_g_pe_dcm_hw_h14_info;
	#endif
	do {
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H14)
		{
			LX_PE_DCM_DCE_SMOOTH1_T *pp=(LX_PE_DCM_DCE_SMOOTH1_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			PE_DCM_HW_H14_DBG_PRINT("set[%d] : \n"\
				"map_sel:%d, a_tap:%d, a,t_gain:%d,%d, th:%d,%d,%d,%d, v_gain:%d\n", \
				pp->win_id,pp->sc_map_sel, pp->sc_amap_filter_tap, pp->amap_gain, \
				pp->tmap_gain, pp->e_to_w_th_x0, pp->e_to_w_th_x1, \
				pp->e_to_w_th_y0, pp->e_to_w_th_y1, pp->blur_v_gain);
			if(PE_CHECK_WIN0(pp->win_id))
			{
				PE_P1L_H14_QWr04(pe1_dce_blur_ctrl_00,	sc_map_sel,			GET_BITS(pp->sc_map_sel,0,2),\
														sc_amap_filter_tap,	GET_BITS(pp->sc_amap_filter_tap,0,2),\
														amap_gain,			GET_BITS(pp->amap_gain,0,5),\
														tmap_gain,			GET_BITS(pp->tmap_gain,0,5));
				PE_P1L_H14_QWr04(pe1_dce_blur_ctrl_01,	e_to_w_th_x0,		GET_BITS(pp->e_to_w_th_x0,0,8),\
														e_to_w_th_x1,		GET_BITS(pp->e_to_w_th_x1,0,8),\
														e_to_w_th_y0,		GET_BITS(pp->e_to_w_th_y0,0,8),\
														e_to_w_th_y1,		GET_BITS(pp->e_to_w_th_y1,0,8));
				#ifdef PE_DCM_HW_H14_SET_BLUR_GAIN_ON_TASK
				pInfo->cur_blur_gain = GET_BITS(pp->blur_v_gain,0,4);
				#else
				PE_P1L_H14_QWr01(pe1_dce_ctrl_01,		blur_v_gain, 	 	GET_BITS(pp->blur_v_gain,0,4));
				#endif
			}
		}
		else
		{
			PE_DCM_HW_H14_DBG_PRINT("nothing to do\n");	ret = RET_OK;
		}
	}while (0);
	return ret;
}
/**
 * get smooth contrast
 *
 * @param   *pstParams [in] void
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_DCM_HW_H14_GetDceSmoothCtrl(void *pstParams)
{
	int ret = RET_OK;
	LX_PE_WIN_ID win_id;
	do {
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H14)
		{
			LX_PE_DCM_DCE_SMOOTH1_T *pp=(LX_PE_DCM_DCE_SMOOTH1_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			win_id = PE_GET_CHECKED_WINID(pp->win_id);
			if(PE_CHECK_WIN0(win_id))
			{
				PE_P1L_H14_QRd04(pe1_dce_blur_ctrl_00,	sc_map_sel,			pp->sc_map_sel,\
														sc_amap_filter_tap,	pp->sc_amap_filter_tap,\
														amap_gain,			pp->amap_gain,\
														tmap_gain,			pp->tmap_gain);
				PE_P1L_H14_QRd04(pe1_dce_blur_ctrl_01,	e_to_w_th_x0,		pp->e_to_w_th_x0,\
														e_to_w_th_x1,		pp->e_to_w_th_x1,\
														e_to_w_th_y0,		pp->e_to_w_th_y0,\
														e_to_w_th_y1,		pp->e_to_w_th_y1);
				PE_P1L_H14_QRd01(pe1_dce_ctrl_01,		blur_v_gain, 	 	pp->blur_v_gain);
			}
			PE_DCM_HW_H14_DBG_PRINT("get[%d] : \n"\
				"map_sel:%d, a_tap:%d, a,t_gain:%d,%d, th:%d,%d,%d,%d, v_gain:%d\n", \
				pp->win_id,pp->sc_map_sel, pp->sc_amap_filter_tap, pp->amap_gain, \
				pp->tmap_gain, pp->e_to_w_th_x0, pp->e_to_w_th_x1, \
				pp->e_to_w_th_y0, pp->e_to_w_th_y1, pp->blur_v_gain);
		}
		else
		{
			PE_DCM_HW_H14_DBG_PRINT("nothing to do\n");	ret = RET_OK;
		}
	}while (0);
	return ret;
}
/**
 * set blur_v_gain 0xf(protection) for pattern
 *
 * @param   *pCfg [in] PE_TSK_H14_CFG_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_DCM_HW_H14_SetBlurGain(PE_TSK_H14_CFG_T *pCfg)
{
	int ret = RET_OK;
	static UINT32 count = 0;
	static UINT32 sat_th = PE_DCM_HW_H14_SAT_STATUS_DTV_TH;		//pre threshold
	static LX_PE_SRC_TYPE pre_src_type = LX_PE_SRC_DTV;
	static UINT32 pre_sat_st = 0;	//pre status
	UINT32 blur_gain,is_atv_case,high_val,mid_val,low_val;
	PE_INF_H14_CTRL_T *ctrl0_info = &pCfg->inf.ctrl_info[LX_PE_WIN_0];
	LX_PE_INF_DISPLAY_T *disp0_info = &pCfg->inf.disp_info[LX_PE_WIN_0];
	PE_DCM_HW_H14_SETTINGS_T *pInfo=&_g_pe_dcm_hw_h14_info;
	do{
		CHECK_KNULL(pCfg);
		#ifdef PE_KAPI_USE_NEW_DISP_TYPE
		is_atv_case = (disp0_info->src_type==LX_PE_SRC_ATV || \
			disp0_info->src_type==LX_PE_SRC_CVBS || \
			disp0_info->src_type==LX_PE_SRC_SCART)? 1:0;
		#else
		is_atv_case = (disp0_info->src_type==LX_PE_SRC_ATV || disp0_info->src_type==LX_PE_SRC_CVBS)? 1:0;
		#endif
		if(pre_src_type!=disp0_info->src_type)	// init saturation status threshold on input change
		{
			sat_th = (is_atv_case)? PE_DCM_HW_H14_SAT_STATUS_ATV_HIGH_TH:PE_DCM_HW_H14_SAT_STATUS_DTV_HIGH_TH;
			pre_src_type=disp0_info->src_type;
			blur_gain = (ctrl0_info->sat_status>sat_th)? PE_DCM_HW_H14_BLUR_ORG_VAL:pInfo->cur_blur_gain;
			pre_sat_st = ctrl0_info->sat_status;
		}
		else	// apply hysterisis : increase:use high th, decrease:use low th
		{
			if(pre_sat_st!=ctrl0_info->sat_status)
			{
				high_val = PE_DCM_HW_H14_BLUR_ORG_VAL;
				mid_val = pInfo->pre_blur_gain;
				low_val = pInfo->cur_blur_gain;
				mid_val = pInfo->pre_blur_gain;
				low_val = pInfo->cur_blur_gain;
				if(is_atv_case)
				{
					if(pre_sat_st<ctrl0_info->sat_status)	//increase
					{
						/* if higher than hi_th, use high val, else mid_val */
						blur_gain = (ctrl0_info->sat_status>PE_DCM_HW_H14_SAT_STATUS_ATV_HIGH_TH)? high_val:mid_val;
					}
					else	//decrease
					{
						/* if lower than lo_th, use low val, else mid_val */
						blur_gain = (ctrl0_info->sat_status>PE_DCM_HW_H14_SAT_STATUS_ATV_LOW_TH)? mid_val:low_val;
					}
				}
				else
				{
					if(pre_sat_st<ctrl0_info->sat_status)	//increase
					{
						/* if higher than hi_th, use high val, else mid_val */
						blur_gain = (ctrl0_info->sat_status>PE_DCM_HW_H14_SAT_STATUS_DTV_HIGH_TH)? high_val:mid_val;
					}
					else	//decrease
					{
						/* if lower than lo_th, use low val, else mid_val */
						blur_gain = (ctrl0_info->sat_status>PE_DCM_HW_H14_SAT_STATUS_DTV_LOW_TH)? mid_val:low_val;
					}
				}
				/*PE_DCM_HW_H14_DBG_PRINT("[inf ] sat:%d -> %d, blur_gain:%d -> %d\n",\
					pre_sat_st,ctrl0_info->sat_status,pInfo->pre_blur_gain,blur_gain);*/
				pre_sat_st = ctrl0_info->sat_status;
			}
			else
			{
				blur_gain = pInfo->pre_blur_gain;
			}
		}
		/* if it's a pattern, turn off blur gain(set 0xf). */
		if(pInfo->pre_blur_gain!=blur_gain)
		{
			PE_DCM_HW_H14_DBG_PRINT("[set ] src:%d(atv_case:%d), sat:%d(th:%d), cur_gain:%d, blur_gain:%d -> %d\n",\
				disp0_info->src_type,is_atv_case,ctrl0_info->sat_status,sat_th,\
				pInfo->cur_blur_gain,pInfo->pre_blur_gain,blur_gain);
			PE_P1L_H14_QWr01(pe1_dce_ctrl_01,		 blur_v_gain, 	 	GET_BITS(blur_gain,0,4));
			pInfo->pre_blur_gain=blur_gain;
		}
		if(_g_dcm_hw_h14_trace)
		{
			if(count>PE_PRINT_COUNT_NUM)
			{
				PE_DCM_HW_H14_DBG_PRINT("[info] src:%d(atv_case:%d), sat:%d(th:%d), cur_gain:%d, blur_gain:%d\n",\
					disp0_info->src_type,is_atv_case,ctrl0_info->sat_status,sat_th,pInfo->pre_blur_gain,blur_gain);
				count=0;
			}
			count++;
		}
	}while(0);
	return ret;
}
/**
 * set dce blending
 *
 * @param   *pstParams [in] LX_PE_DCM_BLENDING_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_DCM_HW_H14_SetDceBlendingCtrl(LX_PE_DCM_BLENDING_T *pstParams)
{
	int ret = RET_OK;
	UINT32 color_out_gain;
	UINT32 region_sel;
	do {
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		if(pstParams->color_out_gain==0)
		{
			color_out_gain = 1;
		}
		else if(pstParams->color_out_gain>255)
		{
			color_out_gain = 255;
		}
		else
		{
			color_out_gain = pstParams->color_out_gain;
		}
		region_sel = pstParams->region_sel;
		PE_DCM_HW_H14_DBG_PRINT("[wid:%d] set: color_out_gain:%d, color_sel:0x%08x\n"\
			"color_region_en: %d\n"\
			"grad_gain(y,cb,cr):%d,%d,%d\n"\
			"y_range(min,max):%d,%d, cb_range:%d,%d, cr_range:%d,%d\n",\
			pstParams->win_id,color_out_gain,region_sel,\
			pstParams->color_region_en, pstParams->y_grad_gain, \
			pstParams->cb_grad_gain, pstParams->cr_grad_gain,\
			pstParams->y_range_min, pstParams->y_range_max,\
			pstParams->cb_range_min, pstParams->cb_range_max,\
			pstParams->cr_range_min, pstParams->cr_range_max);
		if(PE_KDRV_VER_H14)
		{
			if(PE_CHECK_WIN0(pstParams->win_id))
			{
				PE_P1L_H14_RdFL(pe1_dce_ctrl_00);
				PE_P1L_H14_RdFL(pe1_dce_ctrl_01);
				PE_P1L_H14_Wr01(pe1_dce_ctrl_00,	color_region0_sel,	GET_BITS(region_sel,0,1));
				PE_P1L_H14_Wr01(pe1_dce_ctrl_00,	color_region1_sel,	GET_BITS(region_sel,1,1));
				PE_P1L_H14_Wr01(pe1_dce_ctrl_00,	color_region2_sel,	GET_BITS(region_sel,2,1));
				PE_P1L_H14_Wr01(pe1_dce_ctrl_00,	color_region3_sel,	GET_BITS(region_sel,3,1));
				PE_P1L_H14_Wr01(pe1_dce_ctrl_00,	color_region4_sel,	GET_BITS(region_sel,4,1));
				PE_P1L_H14_Wr01(pe1_dce_ctrl_00,	color_region5_sel,	GET_BITS(region_sel,5,1));
				PE_P1L_H14_Wr01(pe1_dce_ctrl_00,	color_region6_sel,	GET_BITS(region_sel,6,1));
				PE_P1L_H14_Wr01(pe1_dce_ctrl_00,	color_region7_sel,	GET_BITS(region_sel,7,1));
				PE_P1L_H14_Wr01(pe1_dce_ctrl_00,	color_region8_sel,	GET_BITS(region_sel,8,1));
				PE_P1L_H14_Wr01(pe1_dce_ctrl_00,	color_region9_sel,	GET_BITS(region_sel,9,1));
				PE_P1L_H14_Wr01(pe1_dce_ctrl_00,	color_region10_sel,	GET_BITS(region_sel,10,1));
				PE_P1L_H14_Wr01(pe1_dce_ctrl_00,	color_region11_sel,	GET_BITS(region_sel,11,1));
				PE_P1L_H14_Wr01(pe1_dce_ctrl_00,	color_region12_sel,	GET_BITS(region_sel,12,1));
				PE_P1L_H14_Wr01(pe1_dce_ctrl_00,	color_region13_sel,	GET_BITS(region_sel,13,1));
				PE_P1L_H14_Wr01(pe1_dce_ctrl_00,	color_region14_sel,	GET_BITS(region_sel,14,1));
				PE_P1L_H14_Wr01(pe1_dce_ctrl_00,	color_region15_sel,	GET_BITS(region_sel,15,1));
				PE_P1L_H14_Wr01(pe1_dce_ctrl_01,	color_region_gain,	GET_BITS(color_out_gain,0,8));
				PE_P1L_H14_WrFL(pe1_dce_ctrl_00);
				PE_P1L_H14_WrFL(pe1_dce_ctrl_01);
				PE_P1L_H14_QWr04(pe1_dce_ctrl_02,	color_region_en,	GET_BITS(pstParams->color_region_en,0,1),\
													y_grad_gain,		GET_BITS(pstParams->y_grad_gain,0,2),\
													cb_grad_gain,		GET_BITS(pstParams->cb_grad_gain,0,2),\
													cr_grad_gain,		GET_BITS(pstParams->cr_grad_gain,0,2));
				PE_P1L_H14_QWr02(pe1_dce_ctrl_03,	y_range_min,		GET_BITS(pstParams->y_range_min,0,10),\
													y_range_max,		GET_BITS(pstParams->y_range_max,0,10));
				PE_P1L_H14_QWr02(pe1_dce_ctrl_04,	cb_range_min,		GET_BITS(pstParams->cb_range_min,0,10),\
													cb_range_max,		GET_BITS(pstParams->cb_range_max,0,10));
				PE_P1L_H14_QWr02(pe1_dce_ctrl_05,	cr_range_min,		GET_BITS(pstParams->cr_range_min,0,10),\
													cr_range_max,		GET_BITS(pstParams->cr_range_max,0,10));
			}
		}
		else
		{
			PE_DCM_HW_H14_DBG_PRINT("nothing to do.\n");
		}
	}while (0);
	return ret;
}
/**
 * get dce blending
 *
 * @param   *pstParams [in/out] LX_PE_DCM_BLENDING_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_DCM_HW_H14_GetDceBlendingCtrl(LX_PE_DCM_BLENDING_T *pstParams)
{
	int ret = RET_OK;
	LX_PE_WIN_ID win_id;
	UINT32 r_data;
	do {
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		win_id = PE_GET_CHECKED_WINID(pstParams->win_id);
		if(PE_KDRV_VER_H14)
		{
			if(PE_CHECK_WIN0(win_id))
			{
				PE_P1L_H14_RdFL(pe1_dce_ctrl_00);
				PE_P1L_H14_RdFL(pe1_dce_ctrl_01);
				PE_P1L_H14_Rd01(pe1_dce_ctrl_01,	color_region_gain,	pstParams->color_out_gain);
				r_data = PE_P1L_H14_Rd(pe1_dce_ctrl_00);
				pstParams->region_sel = GET_BITS(r_data,8,16);	//[23:8]
				PE_P1L_H14_QRd04(pe1_dce_ctrl_02,	color_region_en,	pstParams->color_region_en,\
													y_grad_gain,		pstParams->y_grad_gain,\
													cb_grad_gain,		pstParams->cb_grad_gain,\
													cr_grad_gain,		pstParams->cr_grad_gain);
				PE_P1L_H14_QRd02(pe1_dce_ctrl_03,	y_range_min,		pstParams->y_range_min,\
													y_range_max,		pstParams->y_range_max);
				PE_P1L_H14_QRd02(pe1_dce_ctrl_04,	cb_range_min,		pstParams->cb_range_min,\
													cb_range_max,		pstParams->cb_range_max);
				PE_P1L_H14_QRd02(pe1_dce_ctrl_05,	cr_range_min,		pstParams->cr_range_min,\
													cr_range_max,		pstParams->cr_range_max);
			}
		}
		else
		{
			PE_DCM_HW_H14_DBG_PRINT("nothing to do.\n");
		}
		PE_DCM_HW_H14_DBG_PRINT("[wid:%d] get: color_out_gain:%d, color_sel:0x%08x\n"\
			"color_region_en: %d\n"\
			"grad_gain(y,cb,cr):%d,%d,%d\n"\
			"y_range(min,max):%d,%d, cb_range:%d,%d, cr_range:%d,%d\n",\
			pstParams->win_id,pstParams->color_out_gain,pstParams->region_sel,\
			pstParams->color_region_en, pstParams->y_grad_gain, \
			pstParams->cb_grad_gain, pstParams->cr_grad_gain,\
			pstParams->y_range_min, pstParams->y_range_max,\
			pstParams->cb_range_min, pstParams->cb_range_max,\
			pstParams->cr_range_min, pstParams->cr_range_max);
	}while (0);
	return ret;
}

