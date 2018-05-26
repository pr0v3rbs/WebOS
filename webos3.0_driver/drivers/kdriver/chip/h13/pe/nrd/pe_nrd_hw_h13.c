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

/** @file pe_nrd_hw_h13.c
 *
 *  driver for picture enhance noise reduction functions. ( used only within kdriver )
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

#include "pe_hw_h13.h"
#include "pe_reg_h13.h"
#include "pe_fwi_h13.h"
#include "pe_nrd_hw_h13.h"
#include "pe_nrd_hw_h13a0.h"
#include "pe_nrd_hw_h13b0.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
/* 352,288 = 704/2,576/2 */
#define PE_NRD_HW_H13_H_MIN			352
#define PE_NRD_HW_H13_V_MIN			288

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define PE_NRD_HW_H13_ERROR	printk

#define PE_NRD_HW_H13_DBG_PRINT(fmt,args...)	\
	if(_g_nrd_hw_h13_trace) printk("[%x,%x][%s,%d] "fmt,PE_CHIP_VER,g_pe_kdrv_ver_mask,__F__,__L__,##args)
#define PE_NRD_HW_H13_CHECK_CODE(_checker,_action,fmt,args...)	\
	{if(_checker){PE_NRD_HW_H13_ERROR(fmt,##args);_action;}}
#define PE_NRD_HW_H13_DBG_PRINT_FMT_STATE(_pre,_cur)	\
	PE_NRD_HW_H13_DBG_PRINT("%s(%d),%s(%d) : %s.\n",\
	#_pre,(_pre),#_cur,(_cur),((_pre)!=(_cur))? "different":"same")

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
static int PE_NRD_HW_H13B_SetDefault(LX_PE_DEFAULT_SETTINGS_T *pstParams);
static int PE_NRD_HW_H13A_SetDefault(LX_PE_DEFAULT_SETTINGS_T *pstParams);
static PE_NRD_HW_H13B_TNR_FMT PE_NRD_HW_H13B_ConvDispInfoToTnrFmt(LX_PE_INF_DISPLAY_T *disp_inf);
static PE_NRD_HW_H13B_DNR_FMT PE_NRD_HW_H13B_ConvDispInfoToDnrFmt(LX_PE_INF_DISPLAY_T *disp_inf);
static PE_NRD_HW_H13A_TNR_FMT PE_NRD_HW_H13A_ConvDispInfoToTnrFmt(LX_PE_INF_DISPLAY_T *disp_inf);
static PE_NRD_HW_H13A_DNR_FMT PE_NRD_HW_H13A_ConvDispInfoToDnrFmt(LX_PE_INF_DISPLAY_T *disp_inf);

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
static UINT8 _g_nrd_hw_h13_trace=0x0;	//default should be off.
static PE_NRD_HW_H13_SETTINGS_T _g_pe_nrd_hw_h13_info;

/*----------------------------------------------------------------------------------------
	Implementation Group
----------------------------------------------------------------------------------------*/
/**
 * init nrd
 *
 * @param   *pstParams [in] PE_CFG_CTRL_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_NRD_HW_H13_Init(PE_CFG_CTRL_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H13BX)
		{
			#ifdef PE_HW_H13_USE_DNR_MODE_ON_TASK
			_g_pe_nrd_hw_h13_info.reg_bnr_ac_h_en0 = 0;
			_g_pe_nrd_hw_h13_info.reg_bnr_ac_h_en1 = 0;
			#endif
			PE_P0L_H13B0_QWr01(p0l_pe0_load,	load_time,	0x1);
			PE_P0R_H13B0_QWr01(p0r_pe0_load,	load_time,	0x1);
			ret = PE_FWI_H13_SetFwTnrEnable(FALSE);
			PE_NRD_HW_H13_CHECK_CODE(ret,break,"[%s,%d] PE_FWI_H13_SetFwTnrEnable() error.\n",__F__,__L__);
			PE_SET_REG_TABLE(NRD_HW_H13,tnr_l_crg_init_ia_h13b0,		PE_REG_H13_WRITE_BASE);
			PE_SET_REG_TABLE(NRD_HW_H13,tnr_l_alphalut_init_ia_h13b0,	PE_REG_H13_WRITE_BASE);
			PE_SET_REG_TABLE(NRD_HW_H13,tnr_l_crg_init_ia_h13b0,		PE_PE0_REG_H13_LRDIFF);
			PE_SET_REG_TABLE(NRD_HW_H13,tnr_l_alphalut_init_ia_h13b0,	PE_PE0_REG_H13_LRDIFF);
			PE_SET_REG_TABLE(NRD_HW_H13,nr_l_init_h13b0,PE_REG_H13_WRITE_BASE);
			PE_SET_REG_TABLE(NRD_HW_H13,nr_l_init_h13b0,PE_PE0_REG_H13_LRDIFF);
			ret = PE_FWI_H13_SetFwTnrEnable(TRUE);
			PE_NRD_HW_H13_CHECK_CODE(ret,break,"[%s,%d] PE_FWI_H13_SetFwTnrEnable() error.\n",__F__,__L__);
			PE_P0L_H13B0_QWr01(p0l_pe0_load,	load_time,	0x0);
			PE_P0R_H13B0_QWr01(p0r_pe0_load,	load_time,	0x0);
			/* enable tnr noise measure window */
			PE_P0L_H13B0_QWr01(p0l_tnr_ctrl_14,	noise_measure_win_en,	0x1);
			PE_P0R_H13B0_QWr01(p0r_tnr_ctrl_14,	noise_measure_win_en,	0x1);
		}
		else if(PE_KDRV_VER_H13AX)
		{
			PE_P0L_H13_QWr01(p0l_pe0_load,	load_time,	0x1);
			PE_P0R_H13_QWr01(p0r_pe0_load,	load_time,	0x1);
			ret = PE_FWI_H13_SetFwTnrEnable(FALSE);
			PE_NRD_HW_H13_CHECK_CODE(ret,break,"[%s,%d] PE_FWI_H13_SetFwTnrEnable() error.\n",__F__,__L__);
			PE_SET_REG_TABLE(NRD_HW_H13,tnr_l_crg_lut_init_ia_h13a0,0);
			PE_SET_REG_TABLE(NRD_HW_H13,tnr_l_alpha_lut_init_ia_h13a0,0);
			PE_SET_REG_TABLE(NRD_HW_H13,tnr_r_crg_lut_init_ia_h13a0,0);
			PE_SET_REG_TABLE(NRD_HW_H13,tnr_r_alpha_lut_init_ia_h13a0,0);
			PE_SET_REG_TABLE(NRD_HW_H13,nr_l_init_h13a0,0);
			PE_SET_REG_TABLE(NRD_HW_H13,nr_r_init_h13a0,0);
			ret = PE_FWI_H13_SetFwTnrEnable(TRUE);
			PE_NRD_HW_H13_CHECK_CODE(ret,break,"[%s,%d] PE_FWI_H13_SetFwTnrEnable() error.\n",__F__,__L__);
			PE_P0L_H13_QWr01(p0l_pe0_load,	load_time,	0x0);
			PE_P0R_H13_QWr01(p0r_pe0_load,	load_time,	0x0);
		}
		else
		{
			PE_NRD_HW_H13_DBG_PRINT("nothing to do\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * set debug settings
 *
 * @param   *pstParams [in] LX_PE_DBG_SETTINGS_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_NRD_HW_H13_SetDebugSettings(LX_PE_DBG_SETTINGS_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		PE_NRD_HW_H13_DBG_PRINT("wid:%d,type:0x%x,[0x%x]print:0x%x,bypass:0x%x\n",\
			pstParams->win_id,pstParams->type,(0x1<<PE_ITEM_PKTMODL(NRD)),\
			pstParams->print_lvl,pstParams->bypass);
		/* set print level */
		if(pstParams->type&LX_PE_DBG_LV)
		{
			_g_nrd_hw_h13_trace = (pstParams->print_lvl & (0x1<<PE_ITEM_PKTMODL(NRD)))? 0x1:0x0;
		}
		/* set bypass */
		if(pstParams->type&LX_PE_DBG_BY)
		{
			if(PE_KDRV_VER_H13BX)
			{
				if(pstParams->bypass & (0x1<<PE_ITEM_PKTMODL(NRD)))
				{
					PE_NRD_HW_H13_DBG_PRINT("tnr,dnr : disable.\n");
					if(PE_CHECK_WIN0(pstParams->win_id))
					{
						PE_P0L_H13B0_QWr01(p0l_tnr_ctrl_00,	tnr_enable, 	0x0);
						PE_P0L_H13B0_QWr03(p0l_dnr_max_ctrl,enable_mnr, 	0x0,\
															enable_dc_bnr, 	0x0,\
															enable_ac_bnr, 	0x0);
					}
					if(PE_CHECK_WIN1(pstParams->win_id))
					{
						PE_P0R_H13B0_QWr01(p0r_tnr_ctrl_00,	tnr_enable, 	0x0);
						PE_P0R_H13B0_QWr03(p0r_dnr_max_ctrl,enable_mnr, 	0x0,\
															enable_dc_bnr, 	0x0,\
															enable_ac_bnr, 	0x0);
					}
				}
				else
				{
					PE_NRD_HW_H13_DBG_PRINT("tnr,dnr : enable.\n");
					if(PE_CHECK_WIN0(pstParams->win_id))
					{
						PE_P0L_H13B0_QWr01(p0l_tnr_ctrl_00,	tnr_enable, 	0x1);
						PE_P0L_H13B0_QWr03(p0l_dnr_max_ctrl,enable_mnr, 	0x1,\
															enable_dc_bnr, 	0x1,\
															enable_ac_bnr, 	0x1);
					}
					if(PE_CHECK_WIN1(pstParams->win_id))
					{
						PE_P0R_H13B0_QWr01(p0r_tnr_ctrl_00,	tnr_enable, 	0x1);
						PE_P0R_H13B0_QWr03(p0r_dnr_max_ctrl,enable_mnr, 	0x1,\
															enable_dc_bnr, 	0x1,\
															enable_ac_bnr, 	0x1);
					}
				}
			}
			else if(PE_KDRV_VER_H13AX)
			{
				if(pstParams->bypass & (0x1<<PE_ITEM_PKTMODL(NRD)))
				{
					PE_NRD_HW_H13_DBG_PRINT("tnr,dnr : disable.\n");
					if(PE_CHECK_WIN0(pstParams->win_id))
					{
						PE_P0L_H13_QWr01(p0l_tnr_ctrl_00,	tnr_enable, 	0x0);
						PE_P0L_H13_QWr04(p0l_dnr_max_ctrl,	enable_snr, 	0x0,\
															enable_mnr, 	0x0,\
															enable_dc_bnr, 	0x0,\
															enable_ac_bnr, 	0x0);
					}
					if(PE_CHECK_WIN1(pstParams->win_id))
					{
						PE_P0R_H13_QWr01(p0r_tnr_ctrl_00,	tnr_enable, 	0x0);
						PE_P0R_H13_QWr04(p0r_dnr_max_ctrl,	enable_snr, 	0x0,\
															enable_mnr, 	0x0,\
															enable_dc_bnr, 	0x0,\
															enable_ac_bnr, 	0x0);
					}
				}
				else
				{
					PE_NRD_HW_H13_DBG_PRINT("tnr,dnr : enable.\n");
					if(PE_CHECK_WIN0(pstParams->win_id))
					{
						PE_P0L_H13_QWr01(p0l_tnr_ctrl_00,	tnr_enable, 	0x1);
						PE_P0L_H13_QWr04(p0l_dnr_max_ctrl,	enable_snr, 	0x1,\
															enable_mnr, 	0x1,\
															enable_dc_bnr, 	0x1,\
															enable_ac_bnr, 	0x1);
					}
					if(PE_CHECK_WIN1(pstParams->win_id))
					{
						PE_P0R_H13_QWr01(p0r_tnr_ctrl_00,	tnr_enable, 	0x1);
						PE_P0R_H13_QWr04(p0r_dnr_max_ctrl,	enable_snr, 	0x1,\
															enable_mnr, 	0x1,\
															enable_dc_bnr, 	0x1,\
															enable_ac_bnr, 	0x1);
					}
				}
			}
			else
			{
				PE_NRD_HW_H13_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
			}
		}
	}while(0);
	return ret;
}
/**
 * set nr default setting
 *
 * @param   *pstParams [in] LX_PE_DEFAULT_SETTINGS_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_NRD_HW_H13_SetDefault(LX_PE_DEFAULT_SETTINGS_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H13BX)
		{
			ret = PE_NRD_HW_H13B_SetDefault(pstParams);
			PE_NRD_HW_H13_CHECK_CODE(ret,break,"[%s,%d] PE_NRD_HW_H13B_SetDefault() error.\n",__F__,__L__);
		}
		else if(PE_KDRV_VER_H13AX)
		{
			ret = PE_NRD_HW_H13A_SetDefault(pstParams);
			PE_NRD_HW_H13_CHECK_CODE(ret,break,"[%s,%d] PE_NRD_HW_H13A_SetDefault() error.\n",__F__,__L__);
		}
		else
		{
			PE_NRD_HW_H13_DBG_PRINT("nothing to do\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * set nr default setting(for H13BX)
 *
 * @param   *pstParams [in] LX_PE_DEFAULT_SETTINGS_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_NRD_HW_H13B_SetDefault(LX_PE_DEFAULT_SETTINGS_T *pstParams)
{
	int ret = RET_OK;
	PE_FWI_H13_TNR_CTRL fwi_tnr;
	PE_FWI_H13_SETTINGS_T fwi_inf;
	PE_INF_H13_SETTINGS_T inf_set;
	LX_PE_INF_DISPLAY_T *cur0_inf = NULL;
	LX_PE_INF_DISPLAY_T *cur1_inf = NULL;
	PE_NRD_HW_H13B_TNR_FMT cur0_tnr_fmt,cur1_tnr_fmt;
	PE_NRD_HW_H13B_DNR_FMT cur0_dnr_fmt,cur1_dnr_fmt;
	static PE_NRD_HW_H13B_TNR_FMT pre0_tnr_fmt = PE_NRD_H13B_TNR_NUM;
	static PE_NRD_HW_H13B_TNR_FMT pre1_tnr_fmt = PE_NRD_H13B_TNR_NUM;
	static PE_NRD_HW_H13B_DNR_FMT pre0_dnr_fmt = PE_NRD_H13B_DNR_NUM;
	static PE_NRD_HW_H13B_DNR_FMT pre1_dnr_fmt = PE_NRD_H13B_DNR_NUM;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		ret = PE_FWI_H13_GetCurFwiSettings(&fwi_inf);
		PE_NRD_HW_H13_CHECK_CODE(ret,break,"[%s,%d] PE_FWI_H13_GetCurFwiSettings() error.\n",__F__,__L__);
		ret = PE_INF_H13_GetCurInfSettings(&inf_set);
		PE_NRD_HW_H13_CHECK_CODE(ret,break,"[%s,%d] PE_INF_H13_GetCurInfSettings() error.\n",__F__,__L__);
		cur0_inf=&inf_set.disp_info[LX_PE_WIN_0];
		cur1_inf=&inf_set.disp_info[LX_PE_WIN_1];
		CHECK_KNULL(cur0_inf);
		CHECK_KNULL(cur1_inf);
		/* L : tnr */
		if(PE_CHECK_WIN0(pstParams->win_id))
		{
			cur0_tnr_fmt = PE_NRD_HW_H13B_ConvDispInfoToTnrFmt(cur0_inf);
			PE_NRD_HW_H13_DBG_PRINT_FMT_STATE(pre0_tnr_fmt,cur0_tnr_fmt);
			if(pre0_tnr_fmt!=cur0_tnr_fmt)
			{
				switch(cur0_tnr_fmt)
				{
					case PE_NRD_H13B_TNR_SD:
						PE_SET_REG_TABLE(NRD_HW_H13,tnr_l_sd_default_h13b0,			PE_REG_H13_WRITE_BASE);
						break;
					case PE_NRD_H13B_TNR_SD_MC_OFF:
						PE_SET_REG_TABLE(NRD_HW_H13,tnr_l_sd_mc_off_default_h13b0,	PE_REG_H13_WRITE_BASE);
						break;
					case PE_NRD_H13B_TNR_ATV:
						PE_SET_REG_TABLE(NRD_HW_H13,tnr_l_atv_default_h13b0,		PE_REG_H13_WRITE_BASE);
						break;
					case PE_NRD_H13B_TNR_DVR_ATV:
						PE_SET_REG_TABLE(NRD_HW_H13,tnr_l_dvr_atv_default_h13b0,	PE_REG_H13_WRITE_BASE);
						break;
					case PE_NRD_H13B_TNR_ATV_SECAM:
						PE_SET_REG_TABLE(NRD_HW_H13,tnr_l_atv_secam_default_h13b0,	PE_REG_H13_WRITE_BASE);
						break;
					case PE_NRD_H13B_TNR_AV:
						PE_SET_REG_TABLE(NRD_HW_H13,tnr_l_av_default_h13b0,			PE_REG_H13_WRITE_BASE);
						break;
					case PE_NRD_H13B_TNR_AV_SECAM:
						PE_SET_REG_TABLE(NRD_HW_H13,tnr_l_av_secam_default_h13b0,	PE_REG_H13_WRITE_BASE);
						break;
					case PE_NRD_H13B_TNR_HD:
						PE_SET_REG_TABLE(NRD_HW_H13,tnr_l_hd_default_h13b0,			PE_REG_H13_WRITE_BASE);
						break;
					case PE_NRD_H13B_TNR_HD_I:
						PE_SET_REG_TABLE(NRD_HW_H13,tnr_l_hd_i_default_h13b0,		PE_REG_H13_WRITE_BASE);
						break;
					case PE_NRD_H13B_TNR_UD:
						PE_SET_REG_TABLE(NRD_HW_H13,tnr_l_ud_default_h13b0,			PE_REG_H13_WRITE_BASE);
						break;
					default:
						break;
				}
				pre0_tnr_fmt=cur0_tnr_fmt;
			}
			/* fwi tnr ctrl */
			fwi_tnr.tnr_en = 1;
			fwi_tnr.m_fir_en = fwi_inf.tnr_ctrl[LX_PE_WIN_0].m_fir_en;
			/* sync with default, if sd or hd&interlace, turn on m_fir_en. otherwise,off */
			/* 20130511, sj.youm, add ext_frc,3dfmt case(such as ud model), always turn on m_fir_en. */
			/* 20130605, sj.youm, off m_fir_en for ud input for ext_frc,3dfmt case(such as ud model) */
			if(cur0_inf->mode.is_ext_frc)	/* external frc */
			{
				if(cur0_inf->out_type==LX_PE_OUT_UD)
					fwi_tnr.m_fir_en = 0;
				else
					fwi_tnr.m_fir_en = 1;
			}
			else	/* internal frc */
			{
				switch(cur0_inf->src_type)
				{
					case LX_PE_SRC_ATV:
					case LX_PE_SRC_CVBS:
					case LX_PE_SRC_SCART:
						fwi_tnr.m_fir_en = 1;
						break;
					case LX_PE_SRC_COMP:
					case LX_PE_SRC_DTV:
					case LX_PE_SRC_HDMI:
					case LX_PE_SRC_RGBPC:
					default:
						if(cur0_inf->fmt_type==LX_PE_FMT_SD)
						{
							fwi_tnr.m_fir_en = 1;
						}
						else
						{
							if(cur0_inf->scan_type==LX_PE_SCAN_INTERLACE)
								fwi_tnr.m_fir_en = 1;
							else
								fwi_tnr.m_fir_en = 0;
						}
						break;
				}
			}
			ret = PE_FWI_H13_SetTnrCtrl(LX_PE_WIN_0,&fwi_tnr);
			PE_NRD_HW_H13_CHECK_CODE(ret,break,"[%s,%d] PE_FWI_H13_SetTnrCtrl() error.\n",__F__,__L__);
			PE_NRD_HW_H13_DBG_PRINT("set[0]tnr_en:%d,m_fir_en:%d\n",fwi_tnr.tnr_en,fwi_tnr.m_fir_en);
			/* 20130613, sj.youm, reset force_nt_lvl_val for ext frc & ud, because of trailing on photo slide */
			if(cur0_inf->mode.is_ext_frc && cur0_inf->out_type==LX_PE_OUT_UD)
			{
				PE_P0L_H13B0_QWr01(p0l_tnr_ctrl_13,	force_nt_lvl_val,	0x0);
			}
		}
		/* R : tnr */
		if(PE_CHECK_WIN1(pstParams->win_id))
		{
			cur1_tnr_fmt = PE_NRD_HW_H13B_ConvDispInfoToTnrFmt(cur1_inf);
			PE_NRD_HW_H13_DBG_PRINT_FMT_STATE(pre1_tnr_fmt,cur1_tnr_fmt);
			if(pre1_tnr_fmt!=cur1_tnr_fmt)
			{
				switch(cur1_tnr_fmt)
				{
					case PE_NRD_H13B_TNR_SD:
						PE_SET_REG_TABLE(NRD_HW_H13,tnr_l_sd_default_h13b0,			PE_PE0_REG_H13_LRDIFF);
						break;
					case PE_NRD_H13B_TNR_SD_MC_OFF:
						PE_SET_REG_TABLE(NRD_HW_H13,tnr_l_sd_mc_off_default_h13b0,	PE_PE0_REG_H13_LRDIFF);
						break;
					case PE_NRD_H13B_TNR_ATV:
						PE_SET_REG_TABLE(NRD_HW_H13,tnr_l_atv_default_h13b0,		PE_PE0_REG_H13_LRDIFF);
						break;
					case PE_NRD_H13B_TNR_DVR_ATV:
						PE_SET_REG_TABLE(NRD_HW_H13,tnr_l_dvr_atv_default_h13b0,	PE_PE0_REG_H13_LRDIFF);
						break;
					case PE_NRD_H13B_TNR_ATV_SECAM:
						PE_SET_REG_TABLE(NRD_HW_H13,tnr_l_atv_secam_default_h13b0,	PE_PE0_REG_H13_LRDIFF);
						break;
					case PE_NRD_H13B_TNR_AV:
						PE_SET_REG_TABLE(NRD_HW_H13,tnr_l_av_default_h13b0,			PE_PE0_REG_H13_LRDIFF);
						break;
					case PE_NRD_H13B_TNR_AV_SECAM:
						PE_SET_REG_TABLE(NRD_HW_H13,tnr_l_av_secam_default_h13b0,	PE_PE0_REG_H13_LRDIFF);
						break;
					case PE_NRD_H13B_TNR_HD:
						PE_SET_REG_TABLE(NRD_HW_H13,tnr_l_hd_default_h13b0,			PE_PE0_REG_H13_LRDIFF);
						break;
					case PE_NRD_H13B_TNR_HD_I:
						PE_SET_REG_TABLE(NRD_HW_H13,tnr_l_hd_i_default_h13b0,		PE_PE0_REG_H13_LRDIFF);
						break;
					case PE_NRD_H13B_TNR_UD:
						PE_SET_REG_TABLE(NRD_HW_H13,tnr_l_ud_default_h13b0,			PE_PE0_REG_H13_LRDIFF);
						break;
					default:
						break;
				}
				pre1_tnr_fmt=cur1_tnr_fmt;
			}
			/* fwi tnr ctrl */
			fwi_tnr.tnr_en = 1;
			fwi_tnr.m_fir_en = fwi_inf.tnr_ctrl[LX_PE_WIN_1].m_fir_en;
			/* sync with default, if sd or hd&interlace, turn on m_fir_en. otherwise,off */
			/* 20130511, sj.youm, add ext_frc,3dfmt case(such as ud model), always turn on m_fir_en. */
			/* 20130605, sj.youm, off m_fir_en for ud input for ext_frc,3dfmt case(such as ud model) */
			if(cur1_inf->mode.is_ext_frc)	/* external frc */
			{
				if(cur1_inf->out_type==LX_PE_OUT_UD)
					fwi_tnr.m_fir_en = 0;
				else
					fwi_tnr.m_fir_en = 1;
			}
			else	/* internal frc */
			{
				switch(cur1_inf->src_type)
				{
					case LX_PE_SRC_ATV:
					case LX_PE_SRC_CVBS:
					case LX_PE_SRC_SCART:
						fwi_tnr.m_fir_en = 1;
						break;
					case LX_PE_SRC_COMP:
					case LX_PE_SRC_DTV:
					case LX_PE_SRC_HDMI:
					case LX_PE_SRC_RGBPC:
					default:
						if(cur1_inf->fmt_type==LX_PE_FMT_SD)
						{
							fwi_tnr.m_fir_en = 1;
						}
						else
						{
							if(cur1_inf->scan_type==LX_PE_SCAN_INTERLACE)
								fwi_tnr.m_fir_en = 1;
							else
								fwi_tnr.m_fir_en = 0;
						}
						break;
				}
			}
			ret = PE_FWI_H13_SetTnrCtrl(LX_PE_WIN_1,&fwi_tnr);
			PE_NRD_HW_H13_CHECK_CODE(ret,break,"[%s,%d] PE_FWI_H13_SetTnrCtrl() error.\n",__F__,__L__);
			PE_NRD_HW_H13_DBG_PRINT("set[1]tnr_en:%d,m_fir_en:%d\n",fwi_tnr.tnr_en,fwi_tnr.m_fir_en);
			/* 20130613, sj.youm, reset force_nt_lvl_val for ext frc & ud, because of trailing on photo slide */
			if(cur1_inf->mode.is_ext_frc && cur1_inf->out_type==LX_PE_OUT_UD)
			{
				PE_P0R_H13B0_QWr01(p0r_tnr_ctrl_13,	force_nt_lvl_val,	0x0);
			}
		}
		/* L : dnr */
		if(PE_CHECK_WIN0(pstParams->win_id))
		{
			cur0_dnr_fmt = PE_NRD_HW_H13B_ConvDispInfoToDnrFmt(cur0_inf);
			PE_NRD_HW_H13_DBG_PRINT_FMT_STATE(pre0_dnr_fmt,cur0_dnr_fmt);
			if(pre0_dnr_fmt!=cur0_dnr_fmt)
			{
				switch(cur0_dnr_fmt)
				{
					case PE_NRD_H13B_DNR_SD:
						PE_SET_REG_TABLE(NRD_HW_H13,dnr_sd_l_default_h13b0,		PE_REG_H13_WRITE_BASE);
						break;
					case PE_NRD_H13B_DNR_HD:
						PE_SET_REG_TABLE(NRD_HW_H13,dnr_hd_l_default_h13b0,		PE_REG_H13_WRITE_BASE);
						break;
					case PE_NRD_H13B_DNR_TP:
						PE_SET_REG_TABLE(NRD_HW_H13,dnr_tp_l_default_h13b0,		PE_REG_H13_WRITE_BASE);
						break;
					default:
						break;
				}
				pre0_dnr_fmt=cur0_dnr_fmt;
			}
		}
		/* R : dnr */
		if(PE_CHECK_WIN1(pstParams->win_id))
		{
			cur1_dnr_fmt = PE_NRD_HW_H13B_ConvDispInfoToDnrFmt(cur1_inf);
			PE_NRD_HW_H13_DBG_PRINT_FMT_STATE(pre1_dnr_fmt,cur1_dnr_fmt);
			if(pre1_dnr_fmt!=cur1_dnr_fmt)
			{
				switch(cur1_dnr_fmt)
				{
					case PE_NRD_H13B_DNR_SD:
						PE_SET_REG_TABLE(NRD_HW_H13,dnr_sd_l_default_h13b0,		PE_PE0_REG_H13_LRDIFF);
						break;
					case PE_NRD_H13B_DNR_HD:
						PE_SET_REG_TABLE(NRD_HW_H13,dnr_hd_l_default_h13b0,		PE_PE0_REG_H13_LRDIFF);
						break;
					case PE_NRD_H13B_DNR_TP:
						PE_SET_REG_TABLE(NRD_HW_H13,dnr_tp_l_default_h13b0,		PE_PE0_REG_H13_LRDIFF);
						break;
					default:
						break;
				}
				pre1_dnr_fmt=cur1_dnr_fmt;
			}
		}
	}while(0);
	return ret;
}
/**
 * set nr default setting(for H13AX)
 *
 * @param   *pstParams [in] LX_PE_DEFAULT_SETTINGS_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_NRD_HW_H13A_SetDefault(LX_PE_DEFAULT_SETTINGS_T *pstParams)
{
	int ret = RET_OK;
	UINT32 yc_motion;
	PE_FWI_H13_TNR_CTRL fwi_tnr;
	PE_FWI_H13_SETTINGS_T fwi_inf;
	PE_INF_H13_SETTINGS_T inf_set;
	LX_PE_INF_DISPLAY_T *cur0_inf = NULL;
	LX_PE_INF_DISPLAY_T *cur1_inf = NULL;
	PE_NRD_HW_H13A_TNR_FMT cur0_tnr_fmt,cur1_tnr_fmt;
	PE_NRD_HW_H13A_DNR_FMT cur0_dnr_fmt,cur1_dnr_fmt;
	static PE_NRD_HW_H13A_TNR_FMT pre0_tnr_fmt = PE_NRD_H13A_TNR_NUM;
	static PE_NRD_HW_H13A_TNR_FMT pre1_tnr_fmt = PE_NRD_H13A_TNR_NUM;
	static PE_NRD_HW_H13A_DNR_FMT pre0_dnr_fmt = PE_NRD_H13A_DNR_NUM;
	static PE_NRD_HW_H13A_DNR_FMT pre1_dnr_fmt = PE_NRD_H13A_DNR_NUM;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		ret = PE_FWI_H13_GetCurFwiSettings(&fwi_inf);
		PE_NRD_HW_H13_CHECK_CODE(ret,break,"[%s,%d] PE_FWI_H13_GetCurFwiSettings() error.\n",__F__,__L__);
		ret = PE_INF_H13_GetCurInfSettings(&inf_set);
		PE_NRD_HW_H13_CHECK_CODE(ret,break,"[%s,%d] PE_INF_H13_GetCurInfSettings() error.\n",__F__,__L__);
		cur0_inf=&inf_set.disp_info[LX_PE_WIN_0];
		cur1_inf=&inf_set.disp_info[LX_PE_WIN_1];
		CHECK_KNULL(cur0_inf);
		CHECK_KNULL(cur1_inf);
		/* L : tnr */
		if(PE_CHECK_WIN0(pstParams->win_id))
		{
			cur0_tnr_fmt = PE_NRD_HW_H13A_ConvDispInfoToTnrFmt(cur0_inf);
			PE_NRD_HW_H13_DBG_PRINT_FMT_STATE(pre0_tnr_fmt,cur0_tnr_fmt);
			if(pre0_tnr_fmt!=cur0_tnr_fmt)
			{
				switch(cur0_tnr_fmt)
				{
					case PE_NRD_H13A_TNR_SD_I:
						PE_SET_REG_TABLE(NRD_HW_H13,tnr_sd_i_l_default_h13a0,0);
						break;
					case PE_NRD_H13A_TNR_SD_P:
						PE_SET_REG_TABLE(NRD_HW_H13,tnr_sd_p_l_default_h13a0,0);
						break;
					case PE_NRD_H13A_TNR_HD_I:
						PE_SET_REG_TABLE(NRD_HW_H13,tnr_hd_i_l_default_h13a0,0);
						break;
					case PE_NRD_H13A_TNR_HD_P:
						PE_SET_REG_TABLE(NRD_HW_H13,tnr_hd_p_l_default_h13a0,0);
						break;
					default:
						break;
				}
				pre0_tnr_fmt=cur0_tnr_fmt;
			}
			/* yc motion,motion_4f_en */
			yc_motion = ((cur0_inf->src_type==LX_PE_SRC_ATV || \
						cur0_inf->src_type==LX_PE_SRC_CVBS || \
						cur0_inf->src_type==LX_PE_SRC_SCART) && \
						(cur0_inf->cstd_type==LX_PE_CSTD_SECAM))? 0x0:0x1;
			/*m_4f_en = (cur0_inf->scan_type==LX_PE_SCAN_INTERLACE)? 0x1:0x0; --> move to default setting*/
			PE_P0L_H13_RdFL(p0l_tnr_ctrl_00);
			PE_P0L_H13_Wr01(p0l_tnr_ctrl_00, calc_motion_max_yc,	yc_motion);
			PE_P0L_H13_Wr01(p0l_tnr_ctrl_00, hist_motion_max_yc,	yc_motion);
			/*PE_P0L_H13_Wr01(p0l_tnr_ctrl_00, motion_4f_en,		m_4f_en);*/
			PE_P0L_H13_WrFL(p0l_tnr_ctrl_00);
			PE_NRD_HW_H13_DBG_PRINT("inf[0]src:%d,fmt:%d,scan:%d\n",\
				cur0_inf->src_type,cur0_inf->fmt_type,cur0_inf->scan_type);
			PE_NRD_HW_H13_DBG_PRINT("set[0]yc_motion:%d\n",yc_motion);
			/* fwi tnr ctrl */
			fwi_tnr.tnr_en = 1;
			fwi_tnr.m_fir_en = fwi_inf.tnr_ctrl[LX_PE_WIN_0].m_fir_en;
			/* sync with default, if sd or hd&interlace, turn on m_fir_en. otherwise,off */
			switch(cur0_inf->src_type)
			{
				case LX_PE_SRC_ATV:
				case LX_PE_SRC_CVBS:
				case LX_PE_SRC_SCART:
					fwi_tnr.m_fir_en = 1;
					break;
				case LX_PE_SRC_COMP:
				case LX_PE_SRC_DTV:
				case LX_PE_SRC_HDMI:
				case LX_PE_SRC_RGBPC:
				default:
					if(cur0_inf->fmt_type==LX_PE_FMT_SD)
					{
						fwi_tnr.m_fir_en = 1;
					}
					else
					{
						if(cur0_inf->scan_type==LX_PE_SCAN_INTERLACE)
							fwi_tnr.m_fir_en = 1;
						else
							fwi_tnr.m_fir_en = 0;
					}
					break;
			}
			ret = PE_FWI_H13_SetTnrCtrl(LX_PE_WIN_0,&fwi_tnr);
			PE_NRD_HW_H13_CHECK_CODE(ret,break,"[%s,%d] PE_FWI_H13_SetTnrCtrl() error.\n",__F__,__L__);
			PE_NRD_HW_H13_DBG_PRINT("set[0]tnr_en:%d,m_fir_en:%d\n",fwi_tnr.tnr_en,fwi_tnr.m_fir_en);
		}
		/* R : tnr */
		if(PE_CHECK_WIN1(pstParams->win_id))
		{
			cur1_tnr_fmt = PE_NRD_HW_H13A_ConvDispInfoToTnrFmt(cur1_inf);
			PE_NRD_HW_H13_DBG_PRINT_FMT_STATE(pre1_tnr_fmt,cur1_tnr_fmt);
			if(pre1_tnr_fmt!=cur1_tnr_fmt)
			{
				switch(cur1_tnr_fmt)
				{
					case PE_NRD_H13A_TNR_SD_I:
						PE_SET_REG_TABLE(NRD_HW_H13,tnr_sd_i_r_default_h13a0,0);
						break;
					case PE_NRD_H13A_TNR_SD_P:
						PE_SET_REG_TABLE(NRD_HW_H13,tnr_sd_p_r_default_h13a0,0);
						break;
					case PE_NRD_H13A_TNR_HD_I:
						PE_SET_REG_TABLE(NRD_HW_H13,tnr_hd_i_r_default_h13a0,0);
						break;
					case PE_NRD_H13A_TNR_HD_P:
						PE_SET_REG_TABLE(NRD_HW_H13,tnr_hd_p_r_default_h13a0,0);
						break;
					default:
						break;
				}
				pre1_tnr_fmt=cur1_tnr_fmt;
			}
			/* yc motion */
			yc_motion = ((cur1_inf->src_type==LX_PE_SRC_ATV || \
						cur1_inf->src_type==LX_PE_SRC_CVBS || \
						cur1_inf->src_type==LX_PE_SRC_SCART) && \
						(cur1_inf->cstd_type==LX_PE_CSTD_SECAM))? 0x0:0x1;
			/*m_4f_en = (cur1_inf->scan_type==LX_PE_SCAN_INTERLACE)? 0x1:0x0; --> move to default setting*/
			PE_P0R_H13_RdFL(p0r_tnr_ctrl_00);
			PE_P0R_H13_Wr01(p0r_tnr_ctrl_00, calc_motion_max_yc,	yc_motion);
			PE_P0R_H13_Wr01(p0r_tnr_ctrl_00, hist_motion_max_yc,	yc_motion);
			/*PE_P0R_H13_Wr01(p0r_tnr_ctrl_00, motion_4f_en,		m_4f_en);*/
			PE_P0R_H13_WrFL(p0r_tnr_ctrl_00);
			PE_NRD_HW_H13_DBG_PRINT("inf[1]src:%d,fmt:%d,scan:%d\n",\
				cur1_inf->src_type,cur1_inf->fmt_type,cur1_inf->scan_type);
			PE_NRD_HW_H13_DBG_PRINT("set[1]yc_motion:%d\n",yc_motion);
			/* fwi tnr ctrl */
			fwi_tnr.tnr_en = 1;
			fwi_tnr.m_fir_en = fwi_inf.tnr_ctrl[LX_PE_WIN_1].m_fir_en;
			/* sync with default, if sd or hd&interlace, turn on m_fir_en. otherwise,off */
			switch(cur1_inf->src_type)
			{
				case LX_PE_SRC_ATV:
				case LX_PE_SRC_CVBS:
				case LX_PE_SRC_SCART:
					fwi_tnr.m_fir_en = 1;
					break;
				case LX_PE_SRC_COMP:
				case LX_PE_SRC_DTV:
				case LX_PE_SRC_HDMI:
				case LX_PE_SRC_RGBPC:
				default:
					if(cur1_inf->fmt_type==LX_PE_FMT_SD)
					{
						fwi_tnr.m_fir_en = 1;
					}
					else
					{
						if(cur1_inf->scan_type==LX_PE_SCAN_INTERLACE)
							fwi_tnr.m_fir_en = 1;
						else
							fwi_tnr.m_fir_en = 0;
					}
					break;
			}
			ret = PE_FWI_H13_SetTnrCtrl(LX_PE_WIN_1,&fwi_tnr);
			PE_NRD_HW_H13_CHECK_CODE(ret,break,"[%s,%d] PE_FWI_H13_SetTnrCtrl() error.\n",__F__,__L__);
			PE_NRD_HW_H13_DBG_PRINT("set[1]tnr_en:%d,m_fir_en:%d\n",fwi_tnr.tnr_en,fwi_tnr.m_fir_en);
		}
		/* L : dnr */
		if(PE_CHECK_WIN0(pstParams->win_id))
		{
			cur0_dnr_fmt = PE_NRD_HW_H13A_ConvDispInfoToDnrFmt(cur0_inf);
			PE_NRD_HW_H13_DBG_PRINT_FMT_STATE(pre0_dnr_fmt,cur0_dnr_fmt);
			if(pre0_dnr_fmt!=cur0_dnr_fmt)
			{
				switch(cur0_dnr_fmt)
				{
					case PE_NRD_H13A_DNR_SD_I:
						PE_SET_REG_TABLE(NRD_HW_H13,dnr_sd_i_l_default_h13a0,0);
						break;
					case PE_NRD_H13A_DNR_SD_P:
						PE_SET_REG_TABLE(NRD_HW_H13,dnr_sd_p_l_default_h13a0,0);
						break;
					case PE_NRD_H13A_DNR_HD_I:
						PE_SET_REG_TABLE(NRD_HW_H13,dnr_hd_i_l_default_h13a0,0);
						break;
					case PE_NRD_H13A_DNR_HD_P:
						PE_SET_REG_TABLE(NRD_HW_H13,dnr_hd_p_l_default_h13a0,0);
						break;
					case PE_NRD_H13A_DNR_PC:
						PE_SET_REG_TABLE(NRD_HW_H13,dnr_pc_l_default_h13a0,0);
						break;
					case PE_NRD_H13A_DNR_AV:
						PE_SET_REG_TABLE(NRD_HW_H13,dnr_av_l_default_h13a0,0);
						break;
					case PE_NRD_H13A_DNR_DTV_SD:
						PE_SET_REG_TABLE(NRD_HW_H13,dnr_dtv_sd_l_default_h13a0,0);
						break;
					default:
						break;
				}
				pre0_dnr_fmt=cur0_dnr_fmt;
			}
		}
		/* R : dnr */
		if(PE_CHECK_WIN1(pstParams->win_id))
		{
			cur1_dnr_fmt = PE_NRD_HW_H13A_ConvDispInfoToDnrFmt(cur1_inf);
			PE_NRD_HW_H13_DBG_PRINT_FMT_STATE(pre1_dnr_fmt,cur1_dnr_fmt);
			if(pre1_dnr_fmt!=cur1_dnr_fmt)
			{
				switch(cur1_dnr_fmt)
				{
					case PE_NRD_H13A_DNR_SD_I:
						PE_SET_REG_TABLE(NRD_HW_H13,dnr_sd_i_r_default_h13a0,0);
						break;
					case PE_NRD_H13A_DNR_SD_P:
						PE_SET_REG_TABLE(NRD_HW_H13,dnr_sd_p_r_default_h13a0,0);
						break;
					case PE_NRD_H13A_DNR_HD_I:
						PE_SET_REG_TABLE(NRD_HW_H13,dnr_hd_i_r_default_h13a0,0);
						break;
					case PE_NRD_H13A_DNR_HD_P:
						PE_SET_REG_TABLE(NRD_HW_H13,dnr_hd_p_r_default_h13a0,0);
						break;
					case PE_NRD_H13A_DNR_PC:
						PE_SET_REG_TABLE(NRD_HW_H13,dnr_pc_r_default_h13a0,0);
						break;
					case PE_NRD_H13A_DNR_AV:
						PE_SET_REG_TABLE(NRD_HW_H13,dnr_av_r_default_h13a0,0);
						break;
					case PE_NRD_H13A_DNR_DTV_SD:
						PE_SET_REG_TABLE(NRD_HW_H13,dnr_dtv_sd_r_default_h13a0,0);
						break;
					default:
						break;
				}
				pre1_dnr_fmt=cur1_dnr_fmt;
			}
		}
	}while(0);
	return ret;
}
/**
 * get tnr fmt from display info.(for H13BX)
 *
 * @param   *disp_inf [in] LX_PE_INF_DISPLAY_T
 * @return  PE_NRD_HW_H13B_TNR_FMT
 * @see
 * @author
 */
static PE_NRD_HW_H13B_TNR_FMT PE_NRD_HW_H13B_ConvDispInfoToTnrFmt(LX_PE_INF_DISPLAY_T *disp_inf)
{
	PE_NRD_HW_H13B_TNR_FMT tnr_fmt = PE_NRD_H13B_TNR_NUM;
	if(disp_inf->out_type==LX_PE_OUT_UD)
	{
		tnr_fmt = PE_NRD_H13B_TNR_UD;
	}
	else
	{
		switch(disp_inf->src_type)
		{
			case LX_PE_SRC_ATV:
				if(disp_inf->cstd_type==LX_PE_CSTD_SECAM)
				{
					tnr_fmt = PE_NRD_H13B_TNR_ATV_SECAM;
				}
				else
				{
					tnr_fmt = PE_NRD_H13B_TNR_ATV;
				}
				break;
			case LX_PE_SRC_CVBS:
			case LX_PE_SRC_SCART:
				if(disp_inf->cstd_type==LX_PE_CSTD_SECAM)
				{
					tnr_fmt = PE_NRD_H13B_TNR_AV_SECAM;
				}
				else
				{
					tnr_fmt = PE_NRD_H13B_TNR_AV;
				}
				break;
			case LX_PE_SRC_DTV:
				if(disp_inf->fmt_type==LX_PE_FMT_SD)
				{
					if(disp_inf->dtv_type==LX_PE_DTV_HDDPLAY && (disp_inf->hdd_type==LX_PE_HDD_SRC_ATV60 || \
						disp_inf->hdd_type==LX_PE_HDD_SRC_ATV50 || disp_inf->hdd_type==LX_PE_HDD_SRC_AV60 || \
						disp_inf->hdd_type==LX_PE_HDD_SRC_AV50 || disp_inf->hdd_type==LX_PE_HDD_SRC_SCARTRGB))
					{
						if(disp_inf->hdd_type==LX_PE_HDD_SRC_ATV60 || disp_inf->hdd_type==LX_PE_HDD_SRC_ATV50)
						{
							tnr_fmt = PE_NRD_H13B_TNR_DVR_ATV;
						}
						else
						{
							tnr_fmt = PE_NRD_H13B_TNR_AV;
						}
					}
					else if(disp_inf->dtv_type==LX_PE_DTV_FILEPLAY && \
						(disp_inf->in_h_size<=PE_NRD_HW_H13_H_MIN && disp_inf->in_v_size<=PE_NRD_HW_H13_V_MIN))
					{
						tnr_fmt = PE_NRD_H13B_TNR_SD_MC_OFF;	//because of boundary noise on divx(<=352x288)
					}
					else
					{
						tnr_fmt = PE_NRD_H13B_TNR_SD;
					}
				}
				else
				{
					tnr_fmt = PE_NRD_H13B_TNR_HD;
				}
				break;
			case LX_PE_SRC_RGBPC:
			case LX_PE_SRC_COMP:
			case LX_PE_SRC_HDMI:
			default:
				if(disp_inf->fmt_type==LX_PE_FMT_SD)
				{
					tnr_fmt = PE_NRD_H13B_TNR_SD;
				}
				else
				{
					if(disp_inf->scan_type==LX_PE_SCAN_INTERLACE)
						tnr_fmt = PE_NRD_H13B_TNR_HD_I;
					else
						tnr_fmt = PE_NRD_H13B_TNR_HD;
				}
				break;
		}
	}
	return tnr_fmt;
}
/**
 * get dnr fmt from display info.(for H13BX)
 *
 * @param   *disp_inf [in] LX_PE_INF_DISPLAY_T
 * @return  PE_NRD_HW_H13B_DNR_FMT
 * @see
 * @author
 */
static PE_NRD_HW_H13B_DNR_FMT PE_NRD_HW_H13B_ConvDispInfoToDnrFmt(LX_PE_INF_DISPLAY_T *disp_inf)
{
	PE_NRD_HW_H13B_DNR_FMT dnr_fmt = PE_NRD_H13B_DNR_NUM;
	switch(disp_inf->src_type)
	{
		case LX_PE_SRC_ATV:
		case LX_PE_SRC_CVBS:
		case LX_PE_SRC_SCART:
			{
				dnr_fmt = PE_NRD_H13B_DNR_SD;
			}
			break;
		case LX_PE_SRC_DTV:
			if (disp_inf->dtv_type==LX_PE_DTV_TESTPIC)
			{
				dnr_fmt = PE_NRD_H13B_DNR_TP;
			}
			else if (disp_inf->fmt_type==LX_PE_FMT_SD)
			{
				dnr_fmt = PE_NRD_H13B_DNR_SD;
			}
			else
			{
				dnr_fmt = PE_NRD_H13B_DNR_HD;
			}
			break;
		case LX_PE_SRC_RGBPC:
		case LX_PE_SRC_COMP:
		case LX_PE_SRC_HDMI:
		default:
			if (disp_inf->fmt_type==LX_PE_FMT_SD)
			{
				dnr_fmt = PE_NRD_H13B_DNR_SD;
			}
			else
			{
				dnr_fmt = PE_NRD_H13B_DNR_HD;
			}
			break;
	}
	return dnr_fmt;
}
/**
 * get tnr fmt from display info.(for H13AX)
 *
 * @param   *disp_inf [in] LX_PE_INF_DISPLAY_T
 * @return  PE_NRD_HW_H13A_TNR_FMT
 * @see
 * @author
 */
static PE_NRD_HW_H13A_TNR_FMT PE_NRD_HW_H13A_ConvDispInfoToTnrFmt(LX_PE_INF_DISPLAY_T *disp_inf)
{
	PE_NRD_HW_H13A_TNR_FMT tnr_fmt = PE_NRD_H13A_TNR_NUM;
	switch(disp_inf->src_type)
	{
		case LX_PE_SRC_ATV:
		case LX_PE_SRC_CVBS:
		case LX_PE_SRC_SCART:
			{
				tnr_fmt = PE_NRD_H13A_TNR_SD_I;
			}
			break;
		case LX_PE_SRC_RGBPC:
		case LX_PE_SRC_DTV:
		case LX_PE_SRC_COMP:
		case LX_PE_SRC_HDMI:
		default:
			if(disp_inf->fmt_type==LX_PE_FMT_SD)
			{
				if(disp_inf->scan_type==LX_PE_SCAN_INTERLACE)
				{
					tnr_fmt = PE_NRD_H13A_TNR_SD_I;
				}
				else
				{
					tnr_fmt = PE_NRD_H13A_TNR_SD_P;
				}
			}
			else
			{
				if(disp_inf->scan_type==LX_PE_SCAN_INTERLACE)
				{
					tnr_fmt = PE_NRD_H13A_TNR_HD_I;
				}
				else
				{
					tnr_fmt = PE_NRD_H13A_TNR_HD_P;
				}
			}
			break;
	}
	return tnr_fmt;
}
/**
 * get dnr fmt from display info.(for H13AX)
 *
 * @param   *disp_inf [in] LX_PE_INF_DISPLAY_T
 * @return  PE_NRD_HW_H13A_DNR_FMT
 * @see
 * @author
 */
static PE_NRD_HW_H13A_DNR_FMT PE_NRD_HW_H13A_ConvDispInfoToDnrFmt(LX_PE_INF_DISPLAY_T *disp_inf)
{
	PE_NRD_HW_H13A_DNR_FMT dnr_fmt = PE_NRD_H13A_DNR_NUM;
	switch(disp_inf->src_type)
	{
		case LX_PE_SRC_ATV:
			{
				dnr_fmt = PE_NRD_H13A_DNR_SD_I;
			}
			break;
		case LX_PE_SRC_CVBS:
		case LX_PE_SRC_SCART:
			{
				dnr_fmt = PE_NRD_H13A_DNR_AV;
			}
			break;
		case LX_PE_SRC_RGBPC:
			{
				dnr_fmt = PE_NRD_H13A_DNR_PC;
			}
			break;
		case LX_PE_SRC_DTV:
			if(disp_inf->fmt_type==LX_PE_FMT_SD)
			{
				dnr_fmt = PE_NRD_H13A_DNR_DTV_SD;
			}
			else
			{
				if(disp_inf->scan_type==LX_PE_SCAN_INTERLACE)
				{
					dnr_fmt = PE_NRD_H13A_DNR_HD_I;
				}
				else
				{
					dnr_fmt = PE_NRD_H13A_DNR_HD_P;
				}
			}
			break;
		case LX_PE_SRC_COMP:
		case LX_PE_SRC_HDMI:
		default:
			if(disp_inf->src_type==LX_PE_SRC_HDMI && \
				disp_inf->hdmi_type==LX_PE_HDMI_PC)
			{
				dnr_fmt = PE_NRD_H13A_DNR_PC;
			}
			else if(disp_inf->fmt_type==LX_PE_FMT_SD)
			{
				if(disp_inf->scan_type==LX_PE_SCAN_INTERLACE)
				{
					dnr_fmt = PE_NRD_H13A_DNR_SD_I;
				}
				else
				{
					dnr_fmt = PE_NRD_H13A_DNR_SD_P;
				}
			}
			else
			{
				if(disp_inf->scan_type==LX_PE_SCAN_INTERLACE)
				{
					dnr_fmt = PE_NRD_H13A_DNR_HD_I;
				}
				else
				{
					dnr_fmt = PE_NRD_H13A_DNR_HD_P;
				}
			}
			break;
	}
	return dnr_fmt;
}
/**
 * read default setting
 *
 * @param   *pstParams [in/out] LX_PE_ETC_TBL_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_NRD_HW_H13_RdDefault(LX_PE_ETC_TBL_T *pstParams)
{
	int ret = RET_OK;
	char buffer[PE_TRACE_STR_SIZE];
	UINT32 param_size=0, table_size=0, is_valid=0;
	static UINT32 rd_cnt=0;
	PE_REG_PARAM_T param;

#define PE_NRD_H13_PRINT_START		"START_OF_PRINT"
#define PE_NRD_H13_PRINT_END		"END_OF_PRINT"
#define PE_NRD_H13_PRINT_EXIT		"EXIT_OF_PRINT"
#define PE_NRD_H13_CASE_PRMENU(_cnt, _action, _buf, _item)	\
	case (_cnt):sprintf(_buf, "   [0x%02x] %s\n", (_item), #_item);_action;break
#define PE_NRD_H13_CASE_SPRINT(_cnt, _action, _buf, fmt, args...)	\
	case (_cnt):sprintf(_buf, fmt, ##args);_action;break
#define PE_NRD_H13_DFLT_SPRINT(_action, _buf, fmt, args...)		\
	default:sprintf(_buf, fmt, ##args);_action;break

	do{
		CHECK_KNULL(pstParams);
		CHECK_KNULL(pstParams->data);
		if (PE_KDRV_VER_H13BX)
		{
			if (pstParams->data[0]>=PE_NRD_HW_H13_RD_DFLT_MAX)
			{
				sprintf(buffer, "%s", PE_NRD_H13_PRINT_EXIT);
				rd_cnt=0;
			}
			else if (pstParams->data[0]==PE_NRD_HW_H13_RD_DFLT_DISPLAY)
			{
				switch (rd_cnt)
				{
					PE_NRD_H13_CASE_SPRINT( 0, rd_cnt++, buffer, "%s", PE_NRD_H13_PRINT_START);
					PE_NRD_H13_CASE_PRMENU( 1, rd_cnt++, buffer, PE_NRD_HW_H13_RD_DFLT_TNR_L);
					PE_NRD_H13_CASE_PRMENU( 2, rd_cnt++, buffer, PE_NRD_HW_H13_RD_DFLT_TNR_R);
					PE_NRD_H13_CASE_PRMENU( 3, rd_cnt++, buffer, PE_NRD_HW_H13_RD_DFLT_DNR_L);
					PE_NRD_H13_CASE_PRMENU( 4, rd_cnt++, buffer, PE_NRD_HW_H13_RD_DFLT_DNR_R);
					PE_NRD_H13_DFLT_SPRINT(rd_cnt=0, buffer, "%s", PE_NRD_H13_PRINT_END);
				}
			}
			else
			{
				is_valid = 0;
				switch (pstParams->data[0])
				{
					case PE_NRD_HW_H13_RD_DFLT_TNR_L:
					case PE_NRD_HW_H13_RD_DFLT_TNR_R:
						table_size = sizeof(tnr_l_sd_default_h13b0);
						param_size = table_size/sizeof(PE_REG_PARAM_T);
						if (rd_cnt<param_size)
						{
							param.addr = tnr_l_sd_default_h13b0[rd_cnt].addr;
							if(pstParams->data[0]==PE_NRD_HW_H13_RD_DFLT_TNR_R)
								param.addr += PE_PE0_REG_H13_LRDIFF;
							is_valid = 1;
						}
						break;
					case PE_NRD_HW_H13_RD_DFLT_DNR_L:
					case PE_NRD_HW_H13_RD_DFLT_DNR_R:
					default:
						table_size = sizeof(dnr_sd_l_default_h13b0);
						param_size = table_size/sizeof(PE_REG_PARAM_T);
						if (rd_cnt<param_size)
						{
							param.addr = dnr_sd_l_default_h13b0[rd_cnt].addr;
							if(pstParams->data[0]==PE_NRD_HW_H13_RD_DFLT_DNR_R)
								param.addr += PE_PE0_REG_H13_LRDIFF;
							is_valid = 1;
						}
						break;
				}
				if (is_valid)
				{
					param.data = PE_REG_H13_RD(param.addr);
					sprintf(buffer, "   {0x%04X, 0x%08X},\n", param.addr, param.data);
					rd_cnt++;
				}
				else
				{
					sprintf(buffer, "%s", PE_NRD_H13_PRINT_END);
					rd_cnt = 0;
				}
			}
			memcpy(pstParams->data, buffer, sizeof(char)*PE_TRACE_STR_SIZE);
		}
	}while (0);
	return ret;
}
/**
 * set dnr common ctrl
 * - use input struct LX_PE_NRD_DNR2_CMN_T for H13BX
 * - use input struct LX_PE_NRD_DNR0_CMN_T for H13AX
 *
 * @param   *pstParams [in] void
 * @return  OK if success, ERROR otherwise.
 * @see		set discription
 * @author
 */
int PE_NRD_HW_H13_SetDnrCmnCtrl(void *pstParams)
{
	int ret = RET_OK;
	PE_NRD_HW_H13_SETTINGS_T *pInfo = &_g_pe_nrd_hw_h13_info;
	do{
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H13BX)
		{
			LX_PE_NRD_DNR2_CMN_T *pp=(LX_PE_NRD_DNR2_CMN_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			PE_NRD_HW_H13_DBG_PRINT("set[%d] : dnr main : en: ac:%d, mnr:%d, dc:%d\n"\
				"mnr : en:%d, master_gain%d, chroma_gain:%d,h_gain:%d, v_gain:%d\n"\
				"ac_bnr: h,v_en:%d,%d,  mo_max,min:%d,%d,  h,v_bmax:%d,%d,  str_h,v_max:%d,%d\n"\
				"dc_bnr: en:%d,master_gain%d, chroma_gain:%d\n",pp->win_id,\
				/* dnr main */
				pp->enable_mnr,pp->enable_ac_bnr,pp->enable_dc_bnr,\
				/* mnr */
				pp->reg_mnr_enable,pp->reg_mnr_master_gain,pp->reg_chroma_master_gain,pp->reg_mnr_h_gain,pp->reg_mnr_v_gain,\
				/* ac */
				pp->bnr_h_en,pp->bnr_v_en,pp->ac_bnr_protect_motion_max,pp->ac_bnr_protect_motion_min,\
				pp->hbmax_gain,pp->vbmax_gain,pp->strength_h_max,pp->strength_v_max,\
				/* dc */
				pp->reg_dc_bnr_enable,pp->reg_dc_bnr_mastergain,pp->reg_dc_bnr_chromagain);
			if(PE_CHECK_WIN0(pp->win_id))
			{
				PE_P0L_H13B0_QWr03(p0l_dnr_max_ctrl,	enable_ac_bnr,			GET_BITS(pp->enable_ac_bnr,0,1),\
														enable_mnr,				GET_BITS(pp->enable_mnr,0,1),\
														enable_dc_bnr,			GET_BITS(pp->enable_dc_bnr,0,1));
				PE_P0L_H13B0_QWr03(p0l_mnr_ctrl_00,		reg_mnr_enable,			GET_BITS(pp->reg_mnr_enable,0,1),\
														reg_mnr_master_gain,	GET_BITS(pp->reg_mnr_master_gain,0,8),\
														reg_chroma_master_gain,	GET_BITS(pp->reg_chroma_master_gain,0,8));
				PE_P0L_H13B0_QWr02(p0l_mnr_ctrl_02,		reg_mnr_h_gain,			GET_BITS(pp->reg_mnr_h_gain,0,8),\
														reg_mnr_v_gain,			GET_BITS(pp->reg_mnr_v_gain,0,8));
				#ifdef PE_HW_H13_USE_DNR_MODE_ON_TASK
				PE_P0L_H13B0_QWr03(p0l_bnr_ac_ctrl_00,	reg_bnr_ac_v_en,		GET_BITS(pp->bnr_v_en,0,1),\
														reg_bnr_ac_gain_h_max,	GET_BITS(pp->hbmax_gain,0,4),\
														reg_bnr_ac_gain_v_max,	GET_BITS(pp->vbmax_gain,0,4));
				pInfo->reg_bnr_ac_h_en0 = GET_BITS(pp->bnr_h_en,0,1);
				#else
				PE_P0L_H13B0_QWr04(p0l_bnr_ac_ctrl_00,	reg_bnr_ac_h_en,		GET_BITS(pp->bnr_h_en,0,1),\
														reg_bnr_ac_v_en,		GET_BITS(pp->bnr_v_en,0,1),\
														reg_bnr_ac_gain_h_max,	GET_BITS(pp->hbmax_gain,0,4),\
														reg_bnr_ac_gain_v_max,	GET_BITS(pp->vbmax_gain,0,4));
				#endif
				PE_P0L_H13B0_QWr02(p0l_bnr_ac_ctrl_06,	ac_bnr_protect_motion_max,	GET_BITS(pp->ac_bnr_protect_motion_max,0,8),\
														ac_bnr_protect_motion_min,	GET_BITS(pp->ac_bnr_protect_motion_min,0,8));
				PE_P0L_H13B0_QWr01(p0l_bnr_ac_ctrl_01,	strength_h_max,			GET_BITS(pp->strength_h_max,0,8));
				PE_P0L_H13B0_QWr01(p0l_bnr_ac_ctrl_02,	strength_v_max,			GET_BITS(pp->strength_v_max,0,8));
				PE_P0L_H13B0_QWr03(p0l_bnr_dc_ctrl_00,	reg_dc_bnr_enable,		GET_BITS(pp->reg_dc_bnr_enable,0,1),\
														reg_dc_bnr_mastergain,	GET_BITS(pp->reg_dc_bnr_mastergain,0,8),\
														reg_dc_bnr_chromagain,	GET_BITS(pp->reg_dc_bnr_chromagain,0,8));
			}
			if(PE_CHECK_WIN1(pp->win_id))
			{
				PE_P0R_H13B0_QWr03(p0r_dnr_max_ctrl,	enable_ac_bnr,			GET_BITS(pp->enable_ac_bnr,0,1),\
														enable_mnr,				GET_BITS(pp->enable_mnr,0,1),\
														enable_dc_bnr,			GET_BITS(pp->enable_dc_bnr,0,1));
				PE_P0R_H13B0_QWr03(p0r_mnr_ctrl_00,		reg_mnr_enable,			GET_BITS(pp->reg_mnr_enable,0,1),\
														reg_mnr_master_gain,	GET_BITS(pp->reg_mnr_master_gain,0,8),\
														reg_chroma_master_gain,	GET_BITS(pp->reg_chroma_master_gain,0,8));
				PE_P0R_H13B0_QWr02(p0r_mnr_ctrl_02,		reg_mnr_h_gain,			GET_BITS(pp->reg_mnr_h_gain,0,8),\
														reg_mnr_v_gain,			GET_BITS(pp->reg_mnr_v_gain,0,8));
				#ifdef PE_HW_H13_USE_DNR_MODE_ON_TASK
				PE_P0R_H13B0_QWr03(p0r_bnr_ac_ctrl_00,	reg_bnr_ac_v_en,		GET_BITS(pp->bnr_v_en,0,1),\
														reg_bnr_ac_gain_h_max,	GET_BITS(pp->hbmax_gain,0,4),\
														reg_bnr_ac_gain_v_max,	GET_BITS(pp->vbmax_gain,0,4));
				pInfo->reg_bnr_ac_h_en1 = GET_BITS(pp->bnr_h_en,0,1);
				#else
				PE_P0R_H13B0_QWr04(p0r_bnr_ac_ctrl_00,	reg_bnr_ac_h_en,		GET_BITS(pp->bnr_h_en,0,1),\
														reg_bnr_ac_v_en,		GET_BITS(pp->bnr_v_en,0,1),\
														reg_bnr_ac_gain_h_max,	GET_BITS(pp->hbmax_gain,0,4),\
														reg_bnr_ac_gain_v_max,	GET_BITS(pp->vbmax_gain,0,4));
				#endif
				PE_P0R_H13B0_QWr02(p0r_bnr_ac_ctrl_06,	ac_bnr_protect_motion_max,	GET_BITS(pp->ac_bnr_protect_motion_max,0,8),\
														ac_bnr_protect_motion_min,	GET_BITS(pp->ac_bnr_protect_motion_min,0,8));
				PE_P0R_H13B0_QWr01(p0r_bnr_ac_ctrl_01,	strength_h_max,			GET_BITS(pp->strength_h_max,0,8));
				PE_P0R_H13B0_QWr01(p0r_bnr_ac_ctrl_02,	strength_v_max,			GET_BITS(pp->strength_v_max,0,8));
				PE_P0R_H13B0_QWr03(p0r_bnr_dc_ctrl_00,	reg_dc_bnr_enable,		GET_BITS(pp->reg_dc_bnr_enable,0,1),\
														reg_dc_bnr_mastergain,	GET_BITS(pp->reg_dc_bnr_mastergain,0,8),\
														reg_dc_bnr_chromagain,	GET_BITS(pp->reg_dc_bnr_chromagain,0,8));
			}
		}
		else if(PE_KDRV_VER_H13AX)
		{
			UINT32 ac_bnr_h_gain,ac_bnr_v_gain;
			LX_PE_NRD_DNR0_CMN_T *pp=(LX_PE_NRD_DNR0_CMN_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			PE_NRD_HW_H13_DBG_PRINT("set[%d] : en:m:%d,a:%d,%d,d:%d, gain:m:%d,a:%d,%d,d:%d\n",pp->win_id,\
				pp->mnr_enable,pp->ac_bnr_h_en,pp->ac_bnr_v_en,pp->dc_bnr_en,\
				pp->mnr_gain,pp->ac_bnr_h_gain,pp->ac_bnr_h_gain,pp->dc_bnr_gain);
			ac_bnr_h_gain = (GET_BITS(pp->ac_bnr_h_gain,0,8)>0x80)? 0x80:GET_BITS(pp->ac_bnr_h_gain,0,8);
			ac_bnr_v_gain = (GET_BITS(pp->ac_bnr_v_gain,0,8)>0x80)? 0x80:GET_BITS(pp->ac_bnr_v_gain,0,8);
			if(PE_CHECK_WIN0(pp->win_id))
			{
				PE_P0L_H13_QWr02(p0l_mnr_ctrl_00,		reg_mnr_enable, 		(pp->mnr_enable)? 0x1:0x0,\
														reg_mnr_master_gain,	GET_BITS(pp->mnr_gain,0,8));
				PE_P0L_H13_QWr02(p0l_bnr_ac_ctrl_00,	bnr_h_en,	 			(pp->ac_bnr_h_en)? 0x1:0x0,\
														bnr_v_en,				(pp->ac_bnr_v_en)? 0x1:0x0);
				PE_P0L_H13_QWr01(p0l_bnr_ac_ctrl_01,	strength_h_max,			ac_bnr_h_gain);
				PE_P0L_H13_QWr01(p0l_bnr_ac_ctrl_02,	strength_v_max,			ac_bnr_v_gain);
				PE_P0L_H13_QWr02(p0l_bnr_dc_ctrl_00,	reg_dc_bnr_enable,		(pp->dc_bnr_en)? 0x1:0x0,\
														reg_dc_bnr_mastergain,	GET_BITS(pp->dc_bnr_gain,0,8));
			}
			if(PE_CHECK_WIN1(pp->win_id))
			{
				PE_P0R_H13_QWr02(p0r_mnr_ctrl_00,		reg_mnr_enable, 		(pp->mnr_enable)? 0x1:0x0,\
														reg_mnr_master_gain,	GET_BITS(pp->mnr_gain,0,8));
				PE_P0R_H13_QWr02(p0r_bnr_ac_ctrl_00,	bnr_h_en,	 			(pp->ac_bnr_h_en)? 0x1:0x0,\
														bnr_v_en,				(pp->ac_bnr_v_en)? 0x1:0x0);
				PE_P0R_H13_QWr01(p0r_bnr_ac_ctrl_01,	strength_h_max,			ac_bnr_h_gain);
				PE_P0R_H13_QWr01(p0r_bnr_ac_ctrl_02,	strength_v_max,			ac_bnr_v_gain);
				PE_P0R_H13_QWr02(p0r_bnr_dc_ctrl_00,	reg_dc_bnr_enable,		(pp->dc_bnr_en)? 0x1:0x0,\
														reg_dc_bnr_mastergain,	GET_BITS(pp->dc_bnr_gain,0,8));
			}
		}
		else
		{
			PE_NRD_HW_H13_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * get dnr common ctrl
 * - use input struct LX_PE_NRD_DNR2_CMN_T for H13BX
 * - use input struct LX_PE_NRD_DNR0_CMN_T for H13AX
 *
 * @param   *pstParams [in/out] void
 * @return  OK if success, ERROR otherwise.
 * @see		set discription
 * @author
 */
int PE_NRD_HW_H13_GetDnrCmnCtrl(void *pstParams)
{
	int ret = RET_OK;
	LX_PE_WIN_ID win_id;
	do{
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H13BX)
		{
			LX_PE_NRD_DNR2_CMN_T *pp = (LX_PE_NRD_DNR2_CMN_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			win_id = PE_GET_CHECKED_WINID(pp->win_id);
			if(PE_CHECK_WIN0(win_id))
			{
				PE_P0L_H13B0_QRd03(p0l_dnr_max_ctrl,	enable_ac_bnr,			pp->enable_ac_bnr,\
														enable_mnr,				pp->enable_mnr,\
														enable_dc_bnr,			pp->enable_dc_bnr);
				PE_P0L_H13B0_QRd03(p0l_mnr_ctrl_00,		reg_mnr_enable,			pp->reg_mnr_enable,\
														reg_mnr_master_gain,	pp->reg_mnr_master_gain,\
														reg_chroma_master_gain,	pp->reg_chroma_master_gain);
				PE_P0L_H13B0_QRd02(p0l_mnr_ctrl_02,		reg_mnr_h_gain,			pp->reg_mnr_h_gain,\
														reg_mnr_v_gain,			pp->reg_mnr_v_gain);
				PE_P0L_H13B0_QRd04(p0l_bnr_ac_ctrl_00,	reg_bnr_ac_h_en,		pp->bnr_h_en,\
														reg_bnr_ac_v_en,		pp->bnr_v_en,\
														reg_bnr_ac_gain_h_max,	pp->hbmax_gain,\
														reg_bnr_ac_gain_v_max,	pp->vbmax_gain);
				PE_P0L_H13B0_QRd02(p0l_bnr_ac_ctrl_06,	ac_bnr_protect_motion_max,	pp->ac_bnr_protect_motion_max,\
														ac_bnr_protect_motion_min,	pp->ac_bnr_protect_motion_min);
				PE_P0L_H13B0_QRd01(p0l_bnr_ac_ctrl_01,	strength_h_max,			pp->strength_h_max);
				PE_P0L_H13B0_QRd01(p0l_bnr_ac_ctrl_02,	strength_v_max,			pp->strength_v_max);
				PE_P0L_H13B0_QRd03(p0l_bnr_dc_ctrl_00,	reg_dc_bnr_enable,		pp->reg_dc_bnr_enable,\
														reg_dc_bnr_mastergain,	pp->reg_dc_bnr_mastergain,\
														reg_dc_bnr_chromagain,	pp->reg_dc_bnr_chromagain);
			}
			if(PE_CHECK_WIN1(win_id))
			{
				PE_P0R_H13B0_QRd03(p0r_dnr_max_ctrl,	enable_ac_bnr,			pp->enable_ac_bnr,\
														enable_mnr,				pp->enable_mnr,\
														enable_dc_bnr,			pp->enable_dc_bnr);
				PE_P0R_H13B0_QRd03(p0r_mnr_ctrl_00,		reg_mnr_enable,			pp->reg_mnr_enable,\
														reg_mnr_master_gain,	pp->reg_mnr_master_gain,\
														reg_chroma_master_gain,	pp->reg_chroma_master_gain);
				PE_P0R_H13B0_QRd02(p0r_mnr_ctrl_02,		reg_mnr_h_gain,			pp->reg_mnr_h_gain,\
														reg_mnr_v_gain,			pp->reg_mnr_v_gain);
				PE_P0R_H13B0_QRd04(p0r_bnr_ac_ctrl_00,	reg_bnr_ac_h_en,		pp->bnr_h_en,\
														reg_bnr_ac_v_en,		pp->bnr_v_en,\
														reg_bnr_ac_gain_h_max,	pp->hbmax_gain,\
														reg_bnr_ac_gain_v_max,	pp->vbmax_gain);
				PE_P0R_H13B0_QRd02(p0r_bnr_ac_ctrl_06,	ac_bnr_protect_motion_max,	pp->ac_bnr_protect_motion_max,\
														ac_bnr_protect_motion_min,	pp->ac_bnr_protect_motion_min);
				PE_P0R_H13B0_QRd01(p0r_bnr_ac_ctrl_01,	strength_h_max,			pp->strength_h_max);
				PE_P0R_H13B0_QRd01(p0r_bnr_ac_ctrl_02,	strength_v_max,			pp->strength_v_max);
				PE_P0R_H13B0_QRd03(p0r_bnr_dc_ctrl_00,	reg_dc_bnr_enable,		pp->reg_dc_bnr_enable,\
														reg_dc_bnr_mastergain,	pp->reg_dc_bnr_mastergain,\
														reg_dc_bnr_chromagain,	pp->reg_dc_bnr_chromagain);
			}
			PE_NRD_HW_H13_DBG_PRINT("get[%d] : dnr main : en: ac:%d, mnr:%d, dc:%d\n"\
						  	 "          mnr      : en:%d, master_gain%d, chroma_gain:%d,h_gain:%d, v_gain:%d\n"\
						 	 "          ac_bnr   : h,v_en:%d,%d,  mo_max,min:%d,%d,  h,v_bmax:%d,%d,  str_h,v_max:%d,%d\n"\
						  	 "          dc_bnr   : en:%d,master_gain%d, chroma_gain:%d\n",pp->win_id,\
				/* dnr main */
				pp->enable_mnr,pp->enable_ac_bnr,pp->enable_dc_bnr,\
				/* mnr */
				pp->reg_mnr_enable,pp->reg_mnr_master_gain,pp->reg_chroma_master_gain,pp->reg_mnr_h_gain,pp->reg_mnr_v_gain,\
				/* ac */
				pp->bnr_h_en,pp->bnr_v_en,pp->ac_bnr_protect_motion_max,pp->ac_bnr_protect_motion_min,\
				pp->hbmax_gain,pp->vbmax_gain,pp->strength_h_max,pp->strength_v_max,\
				/* dc */
				pp->reg_dc_bnr_enable,pp->reg_dc_bnr_mastergain,pp->reg_dc_bnr_chromagain);
		}
		else if(PE_KDRV_VER_H13AX)
		{
			LX_PE_NRD_DNR0_CMN_T *pp = (LX_PE_NRD_DNR0_CMN_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			win_id = PE_GET_CHECKED_WINID(pp->win_id);
			if(PE_CHECK_WIN0(win_id))
			{
				PE_P0L_H13_QRd02(p0l_mnr_ctrl_00,		reg_mnr_enable, 		pp->mnr_enable,\
														reg_mnr_master_gain,	pp->mnr_gain);
				PE_P0L_H13_QRd02(p0l_bnr_ac_ctrl_00,	bnr_h_en,	 			pp->ac_bnr_h_en,\
														bnr_v_en,				pp->ac_bnr_v_en);
				PE_P0L_H13_QRd01(p0l_bnr_ac_ctrl_01,	strength_h_max,			pp->ac_bnr_h_gain);
				PE_P0L_H13_QRd01(p0l_bnr_ac_ctrl_02,	strength_v_max,			pp->ac_bnr_v_gain);
				PE_P0L_H13_QRd02(p0l_bnr_dc_ctrl_00,	reg_dc_bnr_enable,		pp->dc_bnr_en,\
														reg_dc_bnr_mastergain,	pp->dc_bnr_gain);
			}
			if(PE_CHECK_WIN1(win_id))
			{
				PE_P0R_H13_QRd02(p0r_mnr_ctrl_00,		reg_mnr_enable, 		pp->mnr_enable,\
														reg_mnr_master_gain,	pp->mnr_gain);
				PE_P0R_H13_QRd02(p0r_bnr_ac_ctrl_00,	bnr_h_en,	 			pp->ac_bnr_h_en,\
														bnr_v_en,				pp->ac_bnr_v_en);
				PE_P0R_H13_QRd01(p0r_bnr_ac_ctrl_01,	strength_h_max,			pp->ac_bnr_h_gain);
				PE_P0R_H13_QRd01(p0r_bnr_ac_ctrl_02,	strength_v_max,			pp->ac_bnr_v_gain);
				PE_P0R_H13_QRd02(p0r_bnr_dc_ctrl_00,	reg_dc_bnr_enable,		pp->dc_bnr_en,\
														reg_dc_bnr_mastergain,	pp->dc_bnr_gain);
			}
			PE_NRD_HW_H13_DBG_PRINT("get[%d] : en:m:%d,a:%d,%d,d:%d, gain:m:%d,a:%d,%d,d:%d\n",pp->win_id,\
				pp->mnr_enable,pp->ac_bnr_h_en,pp->ac_bnr_v_en,pp->dc_bnr_en,\
				pp->mnr_gain,pp->ac_bnr_h_gain,pp->ac_bnr_h_gain,pp->dc_bnr_gain);
		}
		else
		{
			PE_NRD_HW_H13_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * set dnr detail ctrl
 * - use input struct LX_PE_NRD_DNR2_DETAIL_T for H13BX
 *
 * @param   *pstParams [in] void
 * @return  OK if success, ERROR otherwise.
 * @see		set discription
 * @author
 */
int PE_NRD_HW_H13_SetDnrDetailCtrl(void *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H13BX)
		{
			LX_PE_NRD_DNR2_DETAIL_T *pp=(LX_PE_NRD_DNR2_DETAIL_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			PE_NRD_HW_H13_DBG_PRINT("set[%d] \n"\
				"mnr   : h_expend:%d,  gain_flt_size:%d,  mmd_min s1,s2:%d,%d ratio_min,max: %d,%d \n"\
				"mnr   : filter x0,x1,y0,y1:%d,%d,%d,%d\n"\
				"mnr   : motion_en:%d, filter:%d, min:%d, max:%d\n"\
				"mnr   : motion x0,x1,y0,y1:%d,%d,%d,%d\n"\
				"ac_bnr: protect_en:%d, sdhd_sel(source type):%d, flt_thpe:%d,\n"\
				"ac_bnr: strength h_x0,h_x1,v_x0,v_x1:%d,%d,%d,%d, detect_min_th:%d\n"\
				"dc_bnr: var_en:%d,  motion_en:%d,  acadaptive:%d, sdhd_sel:%d\n"\
				"dc_bnr: blur_sel:%d, protection_en:%d \n"\
				"dc_bnr: var_th0,th1,th2,th3%d,%d,%d,%d, motion_th0,th1,th2,th3%d,%d,%d,%d\n"\
				"dc_bnr: chroma_var:%d, var_h_gain:%d, protection_th:%d\n",pp->win_id,\
				/* mnr */
				pp->reg_h_expend,pp->reg_gain_flt_size,\
				pp->reg_mnr_s1_mmd_min,pp->reg_mnr_s2_ratio_min,pp->reg_mnr_s2_ratio_max,pp->reg_mnr_s2_mmd_min,\
				pp->reg_filter_x0,pp->reg_filter_x1,pp->reg_filter_y0,pp->reg_filter_y1,\
				pp->reg_motion_mnr_en,pp->reg_motion_mnr_filter,pp->reg_mnr_motion_min,pp->reg_mnr_motion_max,\
				pp->reg_motion_mnr_x0,pp->reg_motion_mnr_x1,pp->reg_motion_mnr_y0,pp->reg_motion_mnr_y1,\
				/* ac_bnr */
				pp->motion_protect_enable,pp->source_type,pp->fiter_type,\
				pp->strength_h_x0,pp->strength_h_x1,pp->strength_v_x0,pp->strength_v_x1,\
				pp->detect_min_th,\
				/* dc_bnr */
				pp->reg_dc_bnr_var_en,pp->reg_dc_bnr_motion_en,pp->reg_dc_bnr_acadaptive,pp->reg_dc_bnr_sdhd_sel,\
				pp->reg_dc_blur_sel,pp->reg_dc_protection_en,\
				pp->reg_dc_bnr_var_th0,pp->reg_dc_bnr_var_th1,\
				pp->reg_dc_bnr_var_th2,pp->reg_dc_bnr_var_th3,\
				pp->reg_dc_bnr_motion_th0,pp->reg_dc_bnr_motion_th1,\
				pp->reg_dc_bnr_motion_th2,pp->reg_dc_bnr_motion_th3,\
				pp->reg_dc_chroma_variance,pp->reg_dc_var_h_gain,pp->reg_dc_protection_th);
			if(PE_CHECK_WIN0(pp->win_id))
			{
				/* mnr */
				PE_P0L_H13B0_QWr02(p0l_mnr_ctrl_00,	reg_h_expend,		GET_BITS(pp->reg_h_expend,0,1),\
													reg_gain_flt_size,	GET_BITS(pp->reg_gain_flt_size,0,1));

				PE_P0L_H13B0_QWr04(p0l_mnr_ctrl_01,	reg_mnr_s1_mmd_min,		GET_BITS(pp->reg_mnr_s1_mmd_min,0,8),\
													reg_mnr_s2_ratio_min,	GET_BITS(pp->reg_mnr_s2_ratio_min,0,8),\
													reg_mnr_s2_ratio_max,	GET_BITS(pp->reg_mnr_s2_ratio_max,0,8),\
													reg_mnr_s2_mmd_min,		GET_BITS(pp->reg_mnr_s2_mmd_min,0,8));
				PE_P0L_H13B0_QWr04(p0l_mnr_ctrl_03,	reg_filter_x0,	GET_BITS(pp->reg_filter_x0,0,8),\
													reg_filter_x1,	GET_BITS(pp->reg_filter_x1,0,8),\
													reg_filter_y0,	GET_BITS(pp->reg_filter_y0,0,8),\
													reg_filter_y1,	GET_BITS(pp->reg_filter_y1,0,8));
				PE_P0L_H13B0_QWr04(p0l_mnr_ctrl_04,	reg_motion_mnr_en,		GET_BITS(pp->reg_motion_mnr_en,0,1),\
													reg_motion_mnr_filter,	GET_BITS(pp->reg_motion_mnr_filter,0,1),\
													reg_mnr_motion_min,		GET_BITS(pp->reg_mnr_motion_min,0,8),\
													reg_mnr_motion_max,		GET_BITS(pp->reg_mnr_motion_max,0,8));
				PE_P0L_H13B0_QWr04(p0l_mnr_ctrl_05,	reg_motion_mnr_x0,		GET_BITS(pp->reg_motion_mnr_x0,0,8),\
													reg_motion_mnr_x1,		GET_BITS(pp->reg_motion_mnr_x1,0,8),\
													reg_motion_mnr_y0,		GET_BITS(pp->reg_motion_mnr_y0,0,8),\
													reg_motion_mnr_y1,		GET_BITS(pp->reg_motion_mnr_y1,0,8));
				/* ac_bnr */
				PE_P0L_H13B0_QWr03(p0l_bnr_ac_ctrl_00,	reg_bnr_ac_protect_en,	GET_BITS(pp->motion_protect_enable,0,1),\
														reg_bnr_ac_sdhd_sel,	GET_BITS(pp->source_type,0,1),\
														reg_bnr_ac_flt_type,	GET_BITS(pp->fiter_type,0,1));
				PE_P0L_H13B0_QWr03(p0l_bnr_ac_ctrl_01,	strength_h_x0,	GET_BITS(pp->strength_h_x0,0,8),\
														strength_h_x1,	GET_BITS(pp->strength_h_x1,0,8),\
														detect_min_th,	GET_BITS(pp->detect_min_th,0,8));
				PE_P0L_H13B0_QWr02(p0l_bnr_ac_ctrl_02,	strength_v_x0,	GET_BITS(pp->strength_v_x0,0,8),\
														strength_v_x1,	GET_BITS(pp->strength_v_x1,0,8));
				/* dc_bnr */
				PE_P0L_H13B0_QWr04(p0l_bnr_dc_ctrl_00,	reg_dc_bnr_var_en,		GET_BITS(pp->reg_dc_bnr_var_en,0,1),\
														reg_dc_bnr_motion_en,	GET_BITS(pp->reg_dc_bnr_motion_en,0,1),\
														reg_dc_bnr_acadaptive,	GET_BITS(pp->reg_dc_bnr_acadaptive,0,1),
														reg_dc_bnr_sdhd_sel,	GET_BITS(pp->reg_dc_bnr_sdhd_sel,0,1));
				PE_P0L_H13B0_QWr02(p0l_bnr_dc_ctrl_00,	reg_dc_blur_sel,		GET_BITS(pp->reg_dc_blur_sel,0,2),\
														reg_dc_protection_en,	GET_BITS(pp->reg_dc_protection_en,0,1));
				PE_P0L_H13B0_QWr04(p0l_bnr_dc_ctrl_01,	reg_dc_bnr_var_th0,		GET_BITS(pp->reg_dc_bnr_var_th0,0,8),\
														reg_dc_bnr_var_th1,		GET_BITS(pp->reg_dc_bnr_var_th1,0,8),\
														reg_dc_bnr_var_th2,		GET_BITS(pp->reg_dc_bnr_var_th2,0,8),\
														reg_dc_bnr_var_th3,		GET_BITS(pp->reg_dc_bnr_var_th3,0,8));
				PE_P0L_H13B0_QWr04(p0l_bnr_dc_ctrl_02,	reg_dc_bnr_motion_th0,	GET_BITS(pp->reg_dc_bnr_motion_th0,0,8),\
														reg_dc_bnr_motion_th1,	GET_BITS(pp->reg_dc_bnr_motion_th1,0,8),\
														reg_dc_bnr_motion_th2,	GET_BITS(pp->reg_dc_bnr_motion_th2,0,8),\
														reg_dc_bnr_motion_th3,	GET_BITS(pp->reg_dc_bnr_motion_th3,0,8));
				PE_P0L_H13B0_QWr03(p0l_bnr_dc_ctrl_03,	reg_dc_chroma_variance,	GET_BITS(pp->reg_dc_chroma_variance,0,8),\
														reg_dc_var_h_gain,		GET_BITS(pp->reg_dc_var_h_gain,0,8),\
														reg_dc_protection_th,	GET_BITS(pp->reg_dc_protection_th,0,8));
			}
			if(PE_CHECK_WIN1(pp->win_id))
			{
				/* mnr */
				PE_P0R_H13B0_QWr02(p0r_mnr_ctrl_00,	reg_h_expend,		GET_BITS(pp->reg_h_expend,0,1),\
													reg_gain_flt_size,	GET_BITS(pp->reg_gain_flt_size,0,1));

				PE_P0R_H13B0_QWr04(p0r_mnr_ctrl_01,	reg_mnr_s1_mmd_min,		GET_BITS(pp->reg_mnr_s1_mmd_min,0,8),\
													reg_mnr_s2_ratio_min,	GET_BITS(pp->reg_mnr_s2_ratio_min,0,8),\
													reg_mnr_s2_ratio_max,	GET_BITS(pp->reg_mnr_s2_ratio_max,0,8),\
													reg_mnr_s2_mmd_min,		GET_BITS(pp->reg_mnr_s2_mmd_min,0,8));
				PE_P0R_H13B0_QWr04(p0r_mnr_ctrl_03,	reg_filter_x0,	GET_BITS(pp->reg_filter_x0,0,8),\
													reg_filter_x1,	GET_BITS(pp->reg_filter_x1,0,8),\
													reg_filter_y0,	GET_BITS(pp->reg_filter_y0,0,8),\
													reg_filter_y1,	GET_BITS(pp->reg_filter_y1,0,8));
				PE_P0R_H13B0_QWr04(p0r_mnr_ctrl_04,	reg_motion_mnr_en,		GET_BITS(pp->reg_motion_mnr_en,0,1),\
													reg_motion_mnr_filter,	GET_BITS(pp->reg_motion_mnr_filter,0,1),\
													reg_mnr_motion_min,		GET_BITS(pp->reg_mnr_motion_min,0,8),\
													reg_mnr_motion_max,		GET_BITS(pp->reg_mnr_motion_max,0,8));
				PE_P0R_H13B0_QWr04(p0r_mnr_ctrl_05,	reg_motion_mnr_x0,		GET_BITS(pp->reg_motion_mnr_x0,0,8),\
													reg_motion_mnr_x1,		GET_BITS(pp->reg_motion_mnr_x1,0,8),\
													reg_motion_mnr_y0,		GET_BITS(pp->reg_motion_mnr_y0,0,8),\
													reg_motion_mnr_y1,		GET_BITS(pp->reg_motion_mnr_y1,0,8));
				/* ac_bnr */
				PE_P0R_H13B0_QWr03(p0r_bnr_ac_ctrl_00,	reg_bnr_ac_protect_en,	GET_BITS(pp->motion_protect_enable,0,1),\
														reg_bnr_ac_sdhd_sel,	GET_BITS(pp->source_type,0,1),\
														reg_bnr_ac_flt_type,	GET_BITS(pp->fiter_type,0,1));
				PE_P0R_H13B0_QWr03(p0r_bnr_ac_ctrl_01,	strength_h_x0,	GET_BITS(pp->strength_h_x0,0,8),\
														strength_h_x1,	GET_BITS(pp->strength_h_x1,0,8),\
														detect_min_th,	GET_BITS(pp->detect_min_th,0,8));
				PE_P0R_H13B0_QWr02(p0r_bnr_ac_ctrl_02,	strength_v_x0,	GET_BITS(pp->strength_v_x0,0,8),\
														strength_v_x1,	GET_BITS(pp->strength_v_x1,0,8));
				/* dc_bnr */
				PE_P0R_H13B0_QWr04(p0r_bnr_dc_ctrl_00,	reg_dc_bnr_var_en,		GET_BITS(pp->reg_dc_bnr_var_en,0,1),\
														reg_dc_bnr_motion_en,	GET_BITS(pp->reg_dc_bnr_motion_en,0,1),\
														reg_dc_bnr_acadaptive,	GET_BITS(pp->reg_dc_bnr_acadaptive,0,1),
														reg_dc_bnr_sdhd_sel,	GET_BITS(pp->reg_dc_bnr_sdhd_sel,0,1));
				PE_P0R_H13B0_QWr02(p0r_bnr_dc_ctrl_00,	reg_dc_blur_sel,		GET_BITS(pp->reg_dc_blur_sel,0,2),\
														reg_dc_protection_en,	GET_BITS(pp->reg_dc_protection_en,0,1));
				PE_P0R_H13B0_QWr04(p0r_bnr_dc_ctrl_01,	reg_dc_bnr_var_th0,		GET_BITS(pp->reg_dc_bnr_var_th0,0,8),\
														reg_dc_bnr_var_th1,		GET_BITS(pp->reg_dc_bnr_var_th1,0,8),\
														reg_dc_bnr_var_th2,		GET_BITS(pp->reg_dc_bnr_var_th2,0,8),\
														reg_dc_bnr_var_th3,		GET_BITS(pp->reg_dc_bnr_var_th3,0,8));
				PE_P0R_H13B0_QWr04(p0r_bnr_dc_ctrl_02,	reg_dc_bnr_motion_th0,	GET_BITS(pp->reg_dc_bnr_motion_th0,0,8),\
														reg_dc_bnr_motion_th1,	GET_BITS(pp->reg_dc_bnr_motion_th1,0,8),\
														reg_dc_bnr_motion_th2,	GET_BITS(pp->reg_dc_bnr_motion_th2,0,8),\
														reg_dc_bnr_motion_th3,	GET_BITS(pp->reg_dc_bnr_motion_th3,0,8));
				PE_P0R_H13B0_QWr03(p0r_bnr_dc_ctrl_03,	reg_dc_chroma_variance,	GET_BITS(pp->reg_dc_chroma_variance,0,8),\
														reg_dc_var_h_gain,		GET_BITS(pp->reg_dc_var_h_gain,0,8),\
														reg_dc_protection_th,	GET_BITS(pp->reg_dc_protection_th,0,8));
			}
		}
		else
		{
			PE_NRD_HW_H13_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * get dnr detail ctrl
 * - use input struct LX_PE_NRD_DNR2_DETAIL_T for H13BX
 *
 * @param   *pstParams [in/out] void
 * @return  OK if success, ERROR otherwise.
 * @see		set discription
 * @author
 */
int PE_NRD_HW_H13_GetDnrDetailCtrl(void *pstParams)
{
	int ret = RET_OK;
	LX_PE_WIN_ID win_id;
	do{
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H13BX)
		{
			LX_PE_NRD_DNR2_DETAIL_T *pp=(LX_PE_NRD_DNR2_DETAIL_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			win_id = PE_GET_CHECKED_WINID(pp->win_id);
			if(PE_CHECK_WIN0(win_id))
			{
				/* mnr */
				PE_P0L_H13B0_QRd02(p0l_mnr_ctrl_00,	reg_h_expend,			pp->reg_h_expend,\
													reg_gain_flt_size,		pp->reg_gain_flt_size);
				PE_P0L_H13B0_QRd04(p0l_mnr_ctrl_01,	reg_mnr_s1_mmd_min,		pp->reg_mnr_s1_mmd_min,\
													reg_mnr_s2_ratio_min,	pp->reg_mnr_s2_ratio_min,\
													reg_mnr_s2_ratio_max,	pp->reg_mnr_s2_ratio_max,\
													reg_mnr_s2_mmd_min,		pp->reg_mnr_s2_mmd_min);
				PE_P0L_H13B0_QRd04(p0l_mnr_ctrl_03,	reg_filter_x0,			pp->reg_filter_x0,\
													reg_filter_x1,			pp->reg_filter_x1,\
													reg_filter_y0,			pp->reg_filter_y0,\
													reg_filter_y1,			pp->reg_filter_y1);
				PE_P0L_H13B0_QRd04(p0l_mnr_ctrl_04,	reg_motion_mnr_en,		pp->reg_motion_mnr_en,\
													reg_motion_mnr_filter,	pp->reg_motion_mnr_filter,\
													reg_mnr_motion_min,		pp->reg_mnr_motion_min,\
													reg_mnr_motion_max,		pp->reg_mnr_motion_max);
				PE_P0L_H13B0_QRd04(p0l_mnr_ctrl_05,	reg_motion_mnr_x0,		pp->reg_motion_mnr_x0,\
													reg_motion_mnr_x1,		pp->reg_motion_mnr_x1,\
													reg_motion_mnr_y0,		pp->reg_motion_mnr_y0,\
													reg_motion_mnr_y1,		pp->reg_motion_mnr_y1);
				/* ac_bnr */
				PE_P0L_H13B0_QRd03(p0l_bnr_ac_ctrl_00,	reg_bnr_ac_protect_en,	pp->motion_protect_enable,\
														reg_bnr_ac_sdhd_sel,	pp->source_type,\
														reg_bnr_ac_flt_type,	pp->fiter_type);
				PE_P0L_H13B0_QRd03(p0l_bnr_ac_ctrl_01,	strength_h_x0,		pp->strength_h_x0,\
														strength_h_x1,		pp->strength_h_x1,\
														detect_min_th,		pp->detect_min_th);
				PE_P0L_H13B0_QRd02(p0l_bnr_ac_ctrl_02,	strength_v_x0,		pp->strength_v_x0,\
														strength_v_x1,		pp->strength_v_x1);
				/* dc_bnr */
				PE_P0L_H13B0_QRd04(p0l_bnr_dc_ctrl_00,	reg_dc_bnr_var_en,		pp->reg_dc_bnr_var_en,\
														reg_dc_bnr_motion_en,	pp->reg_dc_bnr_motion_en,\
														reg_dc_bnr_acadaptive,	pp->reg_dc_bnr_acadaptive,\
														reg_dc_bnr_sdhd_sel,	pp->reg_dc_bnr_sdhd_sel);
				PE_P0L_H13B0_QRd02(p0l_bnr_dc_ctrl_00,	reg_dc_blur_sel,		pp->reg_dc_blur_sel,\
														reg_dc_protection_en,	pp->reg_dc_protection_en);
				PE_P0L_H13B0_QRd04(p0l_bnr_dc_ctrl_01,	reg_dc_bnr_var_th0,		pp->reg_dc_bnr_var_th0,\
														reg_dc_bnr_var_th1,		pp->reg_dc_bnr_var_th1,\
														reg_dc_bnr_var_th2,		pp->reg_dc_bnr_var_th2,\
														reg_dc_bnr_var_th3,		pp->reg_dc_bnr_var_th3);
				PE_P0L_H13B0_QRd04(p0l_bnr_dc_ctrl_02,	reg_dc_bnr_motion_th0,	pp->reg_dc_bnr_motion_th0,\
														reg_dc_bnr_motion_th1,	pp->reg_dc_bnr_motion_th1,\
														reg_dc_bnr_motion_th2,	pp->reg_dc_bnr_motion_th2,\
														reg_dc_bnr_motion_th3,	pp->reg_dc_bnr_motion_th3);
				PE_P0L_H13B0_QRd03(p0l_bnr_dc_ctrl_03,	reg_dc_chroma_variance,	pp->reg_dc_chroma_variance,\
														reg_dc_var_h_gain,		pp->reg_dc_var_h_gain,\
														reg_dc_protection_th,	pp->reg_dc_protection_th);
			}
			if(PE_CHECK_WIN1(win_id))
			{
				/* mnr */
				PE_P0R_H13B0_QRd02(p0r_mnr_ctrl_00,	reg_h_expend,			pp->reg_h_expend,\
													reg_gain_flt_size,		pp->reg_gain_flt_size);

				PE_P0R_H13B0_QRd04(p0r_mnr_ctrl_01,	reg_mnr_s1_mmd_min,		pp->reg_mnr_s1_mmd_min,\
													reg_mnr_s2_ratio_min,	pp->reg_mnr_s2_ratio_min,\
													reg_mnr_s2_ratio_max,	pp->reg_mnr_s2_ratio_max,\
													reg_mnr_s2_mmd_min,		pp->reg_mnr_s2_mmd_min);
				PE_P0R_H13B0_QRd04(p0r_mnr_ctrl_03,	reg_filter_x0,			pp->reg_filter_x0,\
													reg_filter_x1,			pp->reg_filter_x1,\
													reg_filter_y0,			pp->reg_filter_y0,\
													reg_filter_y1,			pp->reg_filter_y1);
				PE_P0R_H13B0_QRd04(p0r_mnr_ctrl_04,	reg_motion_mnr_en,		pp->reg_motion_mnr_en,\
													reg_motion_mnr_filter,	pp->reg_motion_mnr_filter,\
													reg_mnr_motion_min,		pp->reg_mnr_motion_min,\
													reg_mnr_motion_max,		pp->reg_mnr_motion_max);
				PE_P0R_H13B0_QRd04(p0r_mnr_ctrl_05,	reg_motion_mnr_x0,		pp->reg_motion_mnr_x0,\
													reg_motion_mnr_x1,		pp->reg_motion_mnr_x1,\
													reg_motion_mnr_y0,		pp->reg_motion_mnr_y0,\
													reg_motion_mnr_y1,		pp->reg_motion_mnr_y1);
				/* ac_bnr */
				PE_P0R_H13B0_QRd03(p0r_bnr_ac_ctrl_00,	reg_bnr_ac_protect_en,	pp->motion_protect_enable,\
														reg_bnr_ac_sdhd_sel,	pp->source_type,\
														reg_bnr_ac_flt_type,	pp->fiter_type);
				PE_P0R_H13B0_QRd03(p0r_bnr_ac_ctrl_01,	strength_h_x0,		pp->strength_h_x0,\
														strength_h_x1,		pp->strength_h_x1,\
														detect_min_th,		pp->detect_min_th);
				PE_P0R_H13B0_QRd02(p0r_bnr_ac_ctrl_02,	strength_v_x0,		pp->strength_v_x0,\
														strength_v_x1,		pp->strength_v_x1);
				/* dc_bnr */
				PE_P0R_H13B0_QRd04(p0r_bnr_dc_ctrl_00,	reg_dc_bnr_var_en,		pp->reg_dc_bnr_var_en,\
														reg_dc_bnr_motion_en,	pp->reg_dc_bnr_motion_en,\
														reg_dc_bnr_acadaptive,	pp->reg_dc_bnr_acadaptive,\
														reg_dc_bnr_sdhd_sel,	pp->reg_dc_bnr_sdhd_sel);
				PE_P0R_H13B0_QRd02(p0r_bnr_dc_ctrl_00,	reg_dc_blur_sel,		pp->reg_dc_blur_sel,\
														reg_dc_protection_en,	pp->reg_dc_protection_en);
				PE_P0R_H13B0_QRd04(p0r_bnr_dc_ctrl_01,	reg_dc_bnr_var_th0,		pp->reg_dc_bnr_var_th0,\
														reg_dc_bnr_var_th1,		pp->reg_dc_bnr_var_th1,\
														reg_dc_bnr_var_th2,		pp->reg_dc_bnr_var_th2,\
														reg_dc_bnr_var_th3,		pp->reg_dc_bnr_var_th3);
				PE_P0R_H13B0_QRd04(p0r_bnr_dc_ctrl_02,	reg_dc_bnr_motion_th0,	pp->reg_dc_bnr_motion_th0,\
														reg_dc_bnr_motion_th1,	pp->reg_dc_bnr_motion_th1,\
														reg_dc_bnr_motion_th2,	pp->reg_dc_bnr_motion_th2,\
														reg_dc_bnr_motion_th3,	pp->reg_dc_bnr_motion_th3);
				PE_P0R_H13B0_QRd03(p0r_bnr_dc_ctrl_03,	reg_dc_chroma_variance,	pp->reg_dc_chroma_variance,\
														reg_dc_var_h_gain,		pp->reg_dc_var_h_gain,\
														reg_dc_protection_th,	pp->reg_dc_protection_th);
			}
			PE_NRD_HW_H13_DBG_PRINT("get[%d] \n"\
				"mnr   : h_expend:%d,  gain_flt_size:%d,  mmd_min s1,s2:%d,%d ratio_min,max: %d,%d \n"\
				"mnr   : filter x0,x1,y0,y1:%d,%d,%d,%d\n"\
				"mnr   : motion_en:%d, filter:%d, min:%d, max:%d\n"\
				"mnr   : motion x0,x1,y0,y1:%d,%d,%d,%d\n"\
				"ac_bnr: protect_en:%d, sdhd_sel(source type):%d, flt_thpe:%d,\n"\
				"ac_bnr: strength h_x0,h_x1,v_x0,v_x1:%d,%d,%d,%d, detect_min_th:%d\n"\
				"dc_bnr: var_en:%d,  motion_en:%d,  acadaptive:%d, sdhd_sel:%d\n"\
				"dc_bnr: blur_sel:%d, protection_en:%d \n"\
				"dc_bnr: var_th0,th1,th2,th3%d,%d,%d,%d, motion_th0,th1,th2,th3%d,%d,%d,%d\n"\
				"dc_bnr: chroma_var:%d, var_h_gain:%d, protection_th:%d\n",pp->win_id,\
				/* mnr */
				pp->reg_h_expend,pp->reg_gain_flt_size,\
				pp->reg_mnr_s1_mmd_min,pp->reg_mnr_s2_ratio_min,pp->reg_mnr_s2_ratio_max,pp->reg_mnr_s2_mmd_min,\
				pp->reg_filter_x0,pp->reg_filter_x1,pp->reg_filter_y0,pp->reg_filter_y1,\
				pp->reg_motion_mnr_en,pp->reg_motion_mnr_filter,pp->reg_mnr_motion_min,pp->reg_mnr_motion_max,\
				pp->reg_motion_mnr_x0,pp->reg_motion_mnr_x1,pp->reg_motion_mnr_y0,pp->reg_motion_mnr_y1,\
				/* ac_bnr */
				pp->motion_protect_enable,pp->source_type,pp->fiter_type,\
				pp->strength_h_x0,pp->strength_h_x1,pp->strength_v_x0,pp->strength_v_x1,\
				pp->detect_min_th,\
				/* dc_bnr */
				pp->reg_dc_bnr_var_en,pp->reg_dc_bnr_motion_en,pp->reg_dc_bnr_acadaptive,pp->reg_dc_bnr_sdhd_sel,\
				pp->reg_dc_blur_sel,pp->reg_dc_protection_en,\
				pp->reg_dc_bnr_var_th0,pp->reg_dc_bnr_var_th1,\
				pp->reg_dc_bnr_var_th2,pp->reg_dc_bnr_var_th3,\
				pp->reg_dc_bnr_motion_th0,pp->reg_dc_bnr_motion_th1,\
				pp->reg_dc_bnr_motion_th2,pp->reg_dc_bnr_motion_th3,\
				pp->reg_dc_chroma_variance,pp->reg_dc_var_h_gain,pp->reg_dc_protection_th);
		}
		else
		{
			PE_NRD_HW_H13_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * set tnr common ctrl
 * - use input struct LX_PE_NRD_TNR2_CMN_T for H13BX
 * - use input struct LX_PE_NRD_TNR1_CMN_T for H13AX
 *
 * @param   *pstParams [in] void
 * @return  OK if success, ERROR otherwise.
 * @see		set discription
 * @author
 */
int PE_NRD_HW_H13_SetTnrCmnCtrl(void *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H13BX)
		{
			LX_PE_NRD_TNR2_CMN_T *pp=(LX_PE_NRD_TNR2_CMN_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			PE_NRD_HW_H13_DBG_PRINT("set[%d] : auto_en:%d, y,c_g:%d,%d, adj_nt_en,val:%d,%d\n",pp->win_id,\
				pp->auto_mode_en,pp->y_gain,pp->c_gain,pp->adjust_nt_lvl,pp->adjust_nt_lvl_val);
			if(PE_CHECK_WIN0(pp->win_id))
			{
				PE_P0L_H13B0_QWr02(p0l_tnr_ctrl_13,	adjust_nt_lvl,		GET_BITS(pp->adjust_nt_lvl,0,1),\
													adjust_nt_lvl_val,	GET_BITS(pp->adjust_nt_lvl_val,0,8));
				PE_P0L_H13B0_QWr02(p0l_tnr_ctrl_18,	y_gain,				GET_BITS(pp->y_gain,0,8),\
													c_gain,				GET_BITS(pp->c_gain,0,8));
				PE_P1L_H13B0_QWr01(pe1_dse_ctrl_01,	reg_tnr_auto_en,	(pp->auto_mode_en)? 0x1:0x0);
			}
			if(PE_CHECK_WIN1(pp->win_id))
			{
				PE_P0R_H13B0_QWr02(p0r_tnr_ctrl_13,	adjust_nt_lvl,		GET_BITS(pp->adjust_nt_lvl,0,1),\
													adjust_nt_lvl_val,	GET_BITS(pp->adjust_nt_lvl_val,0,8));
				PE_P0R_H13B0_QWr02(p0r_tnr_ctrl_18,	y_gain,				GET_BITS(pp->y_gain,0,8),\
													c_gain,				GET_BITS(pp->c_gain,0,8));
			}
		}
		else if(PE_KDRV_VER_H13AX)
		{
			LX_PE_NRD_TNR1_CMN_T *pp=(LX_PE_NRD_TNR1_CMN_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			PE_NRD_HW_H13_DBG_PRINT("set[%d] : y,c_g:%d,%d, adj_nt_en,val:%d,%d\n",pp->win_id,\
				pp->y_gain,pp->c_gain,pp->adjust_nt_lvl,pp->adjust_nt_lvl_val);
			if(PE_CHECK_WIN0(pp->win_id))
			{
				PE_P0L_H13_QWr02(p0l_tnr_ctrl_13,	adjust_nt_lvl,		GET_BITS(pp->adjust_nt_lvl,0,1),\
													adjust_nt_lvl_val,	GET_BITS(pp->adjust_nt_lvl_val,0,8));
				PE_P0L_H13_QWr02(p0l_tnr_ctrl_18,	y_gain,				GET_BITS(pp->y_gain,0,8),\
													c_gain,				GET_BITS(pp->c_gain,0,8));
			}
			if(PE_CHECK_WIN1(pp->win_id))
			{
				PE_P0R_H13_QWr02(p0r_tnr_ctrl_13,	adjust_nt_lvl,		GET_BITS(pp->adjust_nt_lvl,0,1),\
													adjust_nt_lvl_val,	GET_BITS(pp->adjust_nt_lvl_val,0,8));
				PE_P0R_H13_QWr02(p0r_tnr_ctrl_18,	y_gain,				GET_BITS(pp->y_gain,0,8),\
													c_gain,				GET_BITS(pp->c_gain,0,8));
			}
		}
		else
		{
			PE_NRD_HW_H13_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * get tnr common ctrl
 * - use input struct LX_PE_NRD_TNR2_CMN_T for H13BX
 * - use input struct LX_PE_NRD_TNR1_CMN_T for H13AX
 *
 * @param   *pstParams [in/out] void
 * @return  OK if success, ERROR otherwise.
 * @see		set discription
 * @author
 */
int PE_NRD_HW_H13_GetTnrCmnCtrl(void *pstParams)
{
	int ret = RET_OK;
	LX_PE_WIN_ID win_id;
	do{
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H13BX)
		{
			LX_PE_NRD_TNR2_CMN_T *pp=(LX_PE_NRD_TNR2_CMN_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			win_id = PE_GET_CHECKED_WINID(pp->win_id);
			if(PE_CHECK_WIN0(win_id))
			{
				PE_P0L_H13B0_QRd02(p0l_tnr_ctrl_13,	adjust_nt_lvl,		pp->adjust_nt_lvl,\
													adjust_nt_lvl_val,	pp->adjust_nt_lvl_val);
				PE_P0L_H13B0_QRd02(p0l_tnr_ctrl_18,	y_gain,				pp->y_gain,\
													c_gain,				pp->c_gain);
				PE_P1L_H13B0_QRd01(pe1_dse_ctrl_01,	reg_tnr_auto_en,	pp->auto_mode_en);
			}
			if(PE_CHECK_WIN1(win_id))
			{
				PE_P0R_H13B0_QRd02(p0r_tnr_ctrl_13,	adjust_nt_lvl,		pp->adjust_nt_lvl,\
													adjust_nt_lvl_val,	pp->adjust_nt_lvl_val);
				PE_P0R_H13B0_QRd02(p0r_tnr_ctrl_18,	y_gain,				pp->y_gain,\
													c_gain,				pp->c_gain);
				pp->auto_mode_en = 0xff;
			}
			PE_NRD_HW_H13_DBG_PRINT("get[%d] : auto_en:%d, y,c_g:%d,%d, adj_nt_en,val:%d,%d\n",pp->win_id,\
				pp->auto_mode_en,pp->y_gain,pp->c_gain,pp->adjust_nt_lvl,pp->adjust_nt_lvl_val);
		}
		else if(PE_KDRV_VER_H13AX)
		{
			LX_PE_NRD_TNR1_CMN_T *pp=(LX_PE_NRD_TNR1_CMN_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			win_id = PE_GET_CHECKED_WINID(pp->win_id);
			if(PE_CHECK_WIN0(win_id))
			{
				PE_P0L_H13_QRd02(p0l_tnr_ctrl_13,	adjust_nt_lvl,		pp->adjust_nt_lvl,\
													adjust_nt_lvl_val,	pp->adjust_nt_lvl_val);
				PE_P0L_H13_QRd02(p0l_tnr_ctrl_18,	y_gain,				pp->y_gain,\
													c_gain,				pp->c_gain);
			}
			if(PE_CHECK_WIN1(win_id))
			{
				PE_P0R_H13_QRd02(p0r_tnr_ctrl_13,	adjust_nt_lvl,		pp->adjust_nt_lvl,\
													adjust_nt_lvl_val,	pp->adjust_nt_lvl_val);
				PE_P0R_H13_QRd02(p0r_tnr_ctrl_18,	y_gain,				pp->y_gain,\
													c_gain,				pp->c_gain);
			}
			PE_NRD_HW_H13_DBG_PRINT("get[%d] : y,c_g:%d,%d, adj_nt_en,val:%d,%d\n",pp->win_id,\
				pp->y_gain,pp->c_gain,pp->adjust_nt_lvl,pp->adjust_nt_lvl_val);
		}
		else
		{
			PE_NRD_HW_H13_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * set tnr detail ctrl
 * - use input struct LX_PE_NRD_TNR2_DETAIL_T for H13BX
 *
 * @param   *pstParams [in] void
 * @return  OK if success, ERROR otherwise.
 * @see		set discription
 * @author
 */
int PE_NRD_HW_H13_SetTnrDetailCtrl(void *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H13BX)
		{
			LX_PE_NRD_TNR2_DETAIL_T *pp=(LX_PE_NRD_TNR2_DETAIL_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			PE_NRD_HW_H13_DBG_PRINT("set[%d] \n"\
				"ma:\nmotion: 	 coring_en (y,c): %d,%d, gain (y,c): %d,%d,\n"\
					 "         	 coring_th: %d, offset_en: %d, gain_en_var (m,a): %d,%d\n"\
					 "         	 wa_y(0,1,2,3):%d,%d,%d,%d, wa_x(0,1,2,3):%d,%d,%d,%d\n"\
					 "alph_gain: gain_x(p0,p1,p2,p3):%d,%d,%d,%d, gain_y(p0,p1,p2,p3):%d,%d,%d,%d\n"\
					 "alph_remap:ave_en:%d, mapping_y_en:%d, mapping_c_en:%d\n"\
					 "           map_y(x0,x1,x2,x3):%d,%d,%d,%d, map_y(y0,y1,y2,y3):%d,%d,%d,%d\n"\
					 "           map_c(x0,x1,x2,x3):%d,%d,%d,%d, map_c(y0,y1,y2,y3):%d,%d,%d,%d\n"\
				     "adj_gain:  gm_en:%d, lpct:%d, avg_ts_en:%d, lpct_sel:%d\n"\
				     "           gm(x0,x1,x2,x3):%d,%d,%d,%d, gm(y0,y1,y2,y3):%d,%d,%d,%d\n"\
				     "           st(x0,x1,x2,x3):%d,%d,%d,%d, st(y0,y1,y2,y3):%d,%d,%d,%d\n"\
				     "           lpct(x0,x1,x2,x3):%d,%d,%d,%d, lpct(y0,y1,y2,y3):%d,%d,%d,%d\n"\
				     "           m_gain_en_var2:%d,\n"\
				     "           ctrl(x0,x1,y0,y1):%d,%d,%d,%d\n"\
				     "           sf_map_flt_en:%d, map_tap:%d, map_gain:%d\n"\
				     "           sf_th(0,1,2,3,4):%d,%d,%d,%d,%d\n"\
				     "           sf_mb(x0,x1,x2,x3):%d,%d,%d,%d, sf_mb(y0,y1,y2,y3):%d,%d,%d,%d\n"\
				     "           sf_gain_md:%d, sf_gain:%d, sf_en(y,c):%d,%d, sf_mo_exp_md:%d\n"\
				"mc:\n           sel_pels:%d, mamc_blend:%d, blend_sel:%d\n"\
				     "           blend(x0,x1,y0,y1):%d,%d,%d,%d\n"\
				"me:\n           hlf_pel_en:%d, smth(gain,en):%d,%d, gmv_smth(gain,en):%d,%d\n"\
				     "           mv0_th:%d, mv0_en:%d, sad_en:%d, sad_gm_en:%d\n"\
				     "           ctrl(x0,x1,y0,y1):%d,%d,%d,%d\n",\
				pp->win_id,\
				// ma: motion
				pp->m_coring_en_y,pp->m_coring_en_c,pp->m_gain_y,pp->m_gain_c,\
				pp->m_coring_th,pp->m_offset_mode_en,pp->m_gain_en_var,pp->a_gain_en_var,\
				pp->wa_y0,pp->wa_y1,pp->wa_y2,pp->wa_y3,pp->wa_x0,pp->wa_x1,pp->wa_x2,pp->wa_x3,\
				// ma: alpha gain
				pp->luma_gain_p0_x,pp->luma_gain_p1_x,pp->luma_gain_p2_x,pp->luma_gain_p3_x,\
				pp->luma_gain_p0_y,pp->luma_gain_p1_y,pp->luma_gain_p2_y,pp->luma_gain_p3_y,\
				// alpha remapping
				pp->alpha_avg_en,pp->alpha_mapping_y_en,pp->alpha_mapping_c_en,\
				pp->alpha_mapping_y_x0,pp->alpha_mapping_y_x1,pp->alpha_mapping_y_x2,pp->alpha_mapping_y_x3,\
				pp->alpha_mapping_y_y0,pp->alpha_mapping_y_y1,pp->alpha_mapping_y_y2,pp->alpha_mapping_y_y3,\
				pp->alpha_mapping_c_x0,pp->alpha_mapping_c_x1,pp->alpha_mapping_c_x2,pp->alpha_mapping_c_x3,\
				pp->alpha_mapping_c_y0,pp->alpha_mapping_c_y1,pp->alpha_mapping_c_y2,pp->alpha_mapping_c_y3,\
				// adjust gain
				pp->nt_lvl_adjust_gm_enable,pp->nt_lvl_adjust_lpct_enable,\
				pp->nt_lvl_adjust_avg_ts_enable,pp->nt_lvl_adjust_lpct_sel,\
				pp->nt_lvl_gain_gm_x0,pp->nt_lvl_gain_gm_x1,pp->nt_lvl_gain_gm_x2,pp->nt_lvl_gain_gm_x3,\
				pp->nt_lvl_gain_gm_y0,pp->nt_lvl_gain_gm_y1,pp->nt_lvl_gain_gm_y2,pp->nt_lvl_gain_gm_y3,\
				pp->nt_lvl_gain_st_x0,pp->nt_lvl_gain_st_x1,pp->nt_lvl_gain_st_x2,pp->nt_lvl_gain_st_x3,\
				pp->nt_lvl_gain_st_y0,pp->nt_lvl_gain_st_y1,pp->nt_lvl_gain_st_y2,pp->nt_lvl_gain_st_y3,\
				pp->nt_lvl_gain_lpct_x0,pp->nt_lvl_gain_lpct_x1,pp->nt_lvl_gain_lpct_x2,pp->nt_lvl_gain_lpct_x3,\
				pp->nt_lvl_gain_lpct_y0,pp->nt_lvl_gain_lpct_y1,pp->nt_lvl_gain_lpct_y2,pp->nt_lvl_gain_lpct_y3,\
				pp->m_gain_en_var2,\
				pp->m_gain_ctrl2_x0,pp->m_gain_ctrl2_x1,pp->m_gain_ctrl2_y0,pp->m_gain_ctrl2_y1,\
				pp->sf_map_flt_en,pp->sf_map_tap,pp->sf_map_gain,\
				pp->sf_th0,pp->sf_th1,pp->sf_th2,pp->sf_th3,pp->sf_th4,\
				pp->sf_mb_x0,pp->sf_mb_x1,pp->sf_mb_x2,pp->sf_mb_x3,\
				pp->sf_mb_y0,pp->sf_mb_y1,pp->sf_mb_y2,pp->sf_mb_y3,\
				pp->sf_gain_mode,pp->sf_gain,pp->sf_blend_en_y,pp->sf_blend_en_c,pp->sf_blend_motion_exp_mode,\
				// mc:
				pp->sad_sel_pels,pp->sad_mamc_blend,pp->mamc_blend_sel,\
				pp->mamc_blend_x0,pp->mamc_blend_x1,pp->mamc_blend_y0,pp->mamc_blend_y1,\
				// me:
				pp->hme_half_pel_en,pp->mv_cost_smooth_gain,pp->mv_cost_smooth_en,\
				pp->mv_cost_gmv_smooth_gain,pp->mv_cost_gmv_smooth_en,\
				pp->mv0_protect_th,pp->mv0_protect_en,pp->sad_protect_en,pp->sad_protect_gm_en,\
				pp->mv_protect_control_x0,pp->mv_protect_control_x1,\
				pp->mv_protect_control_y0,pp->mv_protect_control_y1);
			if(PE_CHECK_WIN0(pp->win_id))
			{
				/* ma: motion */
				PE_P0L_H13B0_QWr02(p0l_tnr_ctrl_38,	m_coring_en_c,		GET_BITS(pp->m_coring_en_c,0,1),\
													m_coring_en_y,		GET_BITS(pp->m_coring_en_y,0,1));
				PE_P0L_H13B0_QWr03(p0l_tnr_ctrl_38,	m_gain_c,			GET_BITS(pp->m_gain_c,0,4),\
													m_gain_y,			GET_BITS(pp->m_gain_y,0,4),\
													m_coring_th,		GET_BITS(pp->m_coring_th,0,6));
				PE_P0L_H13B0_QWr03(p0l_tnr_ctrl_02,	m_offset_mode_en,	GET_BITS(pp->m_offset_mode_en,0,1),\
													m_gain_en_var,		GET_BITS(pp->m_gain_en_var,0,1),\
													a_gain_en_var,		GET_BITS(pp->a_gain_en_var,0,1));
				PE_P0L_H13B0_QWr04(p0l_tnr_ctrl_03,	wa_y0,				GET_BITS(pp->wa_y0,0,8),\
													wa_y1,				GET_BITS(pp->wa_y1,0,8),\
													wa_x0,				GET_BITS(pp->wa_x0,0,8),\
													wa_x1,				GET_BITS(pp->wa_x1,0,8));
				PE_P0L_H13B0_QWr04(p0l_tnr_ctrl_29,	wa_y2,				GET_BITS(pp->wa_y2,0,8),\
													wa_y3,				GET_BITS(pp->wa_y3,0,8),\
													wa_x2,				GET_BITS(pp->wa_x2,0,8),\
													wa_x3,				GET_BITS(pp->wa_x3,0,8));
				/* ma: alpha gain */
				PE_P0L_H13B0_QWr04(p0l_tnr_ctrl_19,	luma_gain_p0_x,			GET_BITS(pp->luma_gain_p0_x,0,8),\
													luma_gain_p1_x,			GET_BITS(pp->luma_gain_p1_x,0,8),\
													luma_gain_p2_x,			GET_BITS(pp->luma_gain_p2_x,0,8),\
													luma_gain_p3_x,			GET_BITS(pp->luma_gain_p3_x,0,8));
				PE_P0L_H13B0_QWr04(p0l_tnr_ctrl_20,	luma_gain_p0_y,			GET_BITS(pp->luma_gain_p0_y,0,8),\
													luma_gain_p1_y,			GET_BITS(pp->luma_gain_p1_y,0,8),\
													luma_gain_p2_y,			GET_BITS(pp->luma_gain_p2_y,0,8),\
													luma_gain_p3_y,			GET_BITS(pp->luma_gain_p3_y,0,8));
				/* ma: alhpa remapping*/
				PE_P0L_H13B0_QWr03(p0l_tnr_ctrl_00,	alpha_avg_en,		GET_BITS(pp->alpha_avg_en,0,1),\
													alpha_mapping_y_en,	GET_BITS(pp->alpha_mapping_y_en,0,8),\
													alpha_mapping_c_en,	GET_BITS(pp->alpha_mapping_c_en,0,8));
				PE_P0L_H13B0_QWr04(p0l_tnr_ctrl_23,	alpha_mapping_y_y0,	GET_BITS(pp->alpha_mapping_y_y0,0,8),\
													alpha_mapping_y_x0,	GET_BITS(pp->alpha_mapping_y_x0,0,8),\
													alpha_mapping_y_y1,	GET_BITS(pp->alpha_mapping_y_y1,0,8),\
													alpha_mapping_y_x1,	GET_BITS(pp->alpha_mapping_y_x1,0,8));
				PE_P0L_H13B0_QWr04(p0l_tnr_ctrl_24,	alpha_mapping_y_y2,	GET_BITS(pp->alpha_mapping_y_y2,0,8),\
													alpha_mapping_y_x2,	GET_BITS(pp->alpha_mapping_y_x2,0,8),\
													alpha_mapping_y_y3,	GET_BITS(pp->alpha_mapping_y_y3,0,8),\
													alpha_mapping_y_x3,	GET_BITS(pp->alpha_mapping_y_x3,0,8));
				PE_P0L_H13B0_QWr04(p0l_tnr_ctrl_25,	alpha_mapping_c_y0,	GET_BITS(pp->alpha_mapping_c_y0,0,8),\
													alpha_mapping_c_x0,	GET_BITS(pp->alpha_mapping_c_x0,0,8),\
													alpha_mapping_c_y1,	GET_BITS(pp->alpha_mapping_c_y1,0,8),\
													alpha_mapping_c_x1,	GET_BITS(pp->alpha_mapping_c_x1,0,8));
				PE_P0L_H13B0_QWr04(p0l_tnr_ctrl_26,	alpha_mapping_c_y2,	GET_BITS(pp->alpha_mapping_c_y2,0,8),\
													alpha_mapping_c_x2,	GET_BITS(pp->alpha_mapping_c_x2,0,8),\
													alpha_mapping_c_y3,	GET_BITS(pp->alpha_mapping_c_y3,0,8),\
													alpha_mapping_c_x3,	GET_BITS(pp->alpha_mapping_c_x3,0,8));
				/*ma: noise level adjust gain*/
				PE_P0L_H13B0_QWr04(p0l_tnr_ctrl_00,	nt_lvl_adjust_gm_enable,		GET_BITS(pp->nt_lvl_adjust_gm_enable,0,1),\
													nt_lvl_adjust_lpct_enable,		GET_BITS(pp->nt_lvl_adjust_lpct_enable,0,1),\
													nt_lvl_adjust_avg_ts_enable,	GET_BITS(pp->nt_lvl_adjust_avg_ts_enable,0,1),\
													nt_lvl_adjust_lpct_sel,			GET_BITS(pp->nt_lvl_adjust_lpct_sel,0,1));
				PE_P0L_H13B0_QWr04(p0l_tnr_ctrl_04,	nt_lvl_gain_gm_y0,		GET_BITS(pp->nt_lvl_gain_gm_y0,0,8),\
													nt_lvl_gain_gm_x0,		GET_BITS(pp->nt_lvl_gain_gm_x0,0,8),\
													nt_lvl_gain_gm_y1,		GET_BITS(pp->nt_lvl_gain_gm_y1,0,8),\
													nt_lvl_gain_gm_x1,		GET_BITS(pp->nt_lvl_gain_gm_x1,0,8));
				PE_P0L_H13B0_QWr04(p0l_tnr_ctrl_12,	nt_lvl_gain_gm_y2,		GET_BITS(pp->nt_lvl_gain_gm_y2,0,8),\
													nt_lvl_gain_gm_x2,		GET_BITS(pp->nt_lvl_gain_gm_x2,0,8),\
													nt_lvl_gain_gm_y3,		GET_BITS(pp->nt_lvl_gain_gm_y3,0,8),\
													nt_lvl_gain_gm_x3,		GET_BITS(pp->nt_lvl_gain_gm_x3,0,8));
				PE_P0L_H13B0_QWr04(p0l_tnr_ctrl_28,	nt_lvl_gain_st_y0,		GET_BITS(pp->nt_lvl_gain_st_y0,0,8),\
													nt_lvl_gain_st_x0,		GET_BITS(pp->nt_lvl_gain_st_x0,0,8),\
													nt_lvl_gain_st_y1,		GET_BITS(pp->nt_lvl_gain_st_y1,0,8),\
													nt_lvl_gain_st_x1,		GET_BITS(pp->nt_lvl_gain_st_x1,0,8));
				PE_P0L_H13B0_QWr04(p0l_tnr_ctrl_30,	nt_lvl_gain_st_y2,		GET_BITS(pp->nt_lvl_gain_st_y2,0,8),\
													nt_lvl_gain_st_x2,		GET_BITS(pp->nt_lvl_gain_st_x2,0,8),\
													nt_lvl_gain_st_y3,		GET_BITS(pp->nt_lvl_gain_st_y3,0,8),\
													nt_lvl_gain_st_x3,		GET_BITS(pp->nt_lvl_gain_st_x3,0,8));
				PE_P0L_H13B0_QWr04(p0l_tnr_ctrl_32,	nt_lvl_gain_lpct_y0,	GET_BITS(pp->nt_lvl_gain_lpct_y0,0,8),\
													nt_lvl_gain_lpct_x0,	GET_BITS(pp->nt_lvl_gain_lpct_x0,0,8),\
													nt_lvl_gain_lpct_y1,	GET_BITS(pp->nt_lvl_gain_lpct_y1,0,8),\
													nt_lvl_gain_lpct_x1,	GET_BITS(pp->nt_lvl_gain_lpct_x1,0,8));
				PE_P0L_H13B0_QWr04(p0l_tnr_ctrl_33,	nt_lvl_gain_lpct_y2,	GET_BITS(pp->nt_lvl_gain_lpct_y2,0,8),\
													nt_lvl_gain_lpct_x2,	GET_BITS(pp->nt_lvl_gain_lpct_x2,0,8),\
													nt_lvl_gain_lpct_y3,	GET_BITS(pp->nt_lvl_gain_lpct_y3,0,8),\
													nt_lvl_gain_lpct_x3,	GET_BITS(pp->nt_lvl_gain_lpct_x3,0,8));
				PE_P0L_H13B0_QWr01(p0l_tnr_ctrl_02,	m_gain_en_var2,			GET_BITS(pp->m_gain_en_var2,0,1));
				PE_P0L_H13B0_QWr04(p0l_tnr_ctrl_48,	m_gain_ctrl2_y0,		GET_BITS(pp->m_gain_ctrl2_y0,0,8),\
													m_gain_ctrl2_x0,		GET_BITS(pp->m_gain_ctrl2_x0,0,8),\
													m_gain_ctrl2_y1,		GET_BITS(pp->m_gain_ctrl2_y1,0,8),\
													m_gain_ctrl2_x1,		GET_BITS(pp->m_gain_ctrl2_x1,0,8));
				PE_P0L_H13B0_QWr03(p0l_tnr_ctrl_40,	sf_map_flt_en,			GET_BITS(pp->sf_map_flt_en,0,1),\
													sf_map_tap,				GET_BITS(pp->sf_map_tap,0,2),\
													sf_map_gain,			GET_BITS(pp->sf_map_gain,0,6));
				PE_P0L_H13B0_QWr02(p0l_tnr_ctrl_40,	sf_th0,					GET_BITS(pp->sf_th0,0,8),\
													sf_th1,					GET_BITS(pp->sf_th1,0,8));
				PE_P0L_H13B0_QWr03(p0l_tnr_ctrl_41,	sf_th2,					GET_BITS(pp->sf_th2,0,8),\
													sf_th3,					GET_BITS(pp->sf_th3,0,8),\
													sf_th4,					GET_BITS(pp->sf_th4,0,8));
				PE_P0L_H13B0_QWr04(p0l_tnr_ctrl_42,	sf_mb_x0,				GET_BITS(pp->sf_mb_x0,0,8),\
													sf_mb_x1,				GET_BITS(pp->sf_mb_x1,0,8),\
													sf_mb_y0,				GET_BITS(pp->sf_mb_y0,0,8),\
													sf_mb_y1,				GET_BITS(pp->sf_mb_y1,0,8));
				PE_P0L_H13B0_QWr04(p0l_tnr_ctrl_44,	sf_mb_x2,				GET_BITS(pp->sf_mb_x2,0,8),\
													sf_mb_x3,				GET_BITS(pp->sf_mb_x3,0,8),\
													sf_mb_y2,				GET_BITS(pp->sf_mb_y2,0,8),\
													sf_mb_y3,				GET_BITS(pp->sf_mb_y3,0,8));
				PE_P0L_H13B0_QWr02(p0l_tnr_ctrl_45,	sf_gain_mode,			GET_BITS(pp->sf_gain_mode,0,1),\
													sf_gain,				GET_BITS(pp->sf_gain,0,4));
				PE_P0L_H13B0_QWr04(p0l_tnr_ctrl_36,	sf_blend_en_y,			GET_BITS(pp->sf_blend_en_y,0,1),\
													sf_blend_en_c,			GET_BITS(pp->sf_blend_en_c,0,1),\
													sf_blend_motion_exp_mode,	GET_BITS(pp->sf_blend_motion_exp_mode,0,2),\
													sad_sel_pels,			GET_BITS(pp->sad_sel_pels,0,1));
				/* mc */
				PE_P0L_H13B0_QWr01(p0l_tnr_ctrl_21,	sad_mamc_blend,		GET_BITS(pp->sad_mamc_blend,0,8));
				PE_P0L_H13B0_QWr01(p0l_tnr_ctrl_00,	mamc_blend_sel,		GET_BITS(pp->mamc_blend_sel,0,1));
				PE_P0L_H13B0_QWr04(p0l_tnr_ctrl_46,	mamc_blend_x0,		GET_BITS(pp->mamc_blend_x0,0,8),\
													mamc_blend_x1,		GET_BITS(pp->mamc_blend_x1,0,8),\
													mamc_blend_y0,		GET_BITS(pp->mamc_blend_y0,0,8),\
													mamc_blend_y1,		GET_BITS(pp->mamc_blend_y1,0,8));
				/* me */
				PE_P0L_H13B0_QWr03(p0l_tnr_ctrl_36,	hme_half_pel_en,		GET_BITS(pp->hme_half_pel_en,0,1),\
													mv_cost_smooth_gain,	GET_BITS(pp->mv_cost_smooth_gain,0,5),\
													mv_cost_smooth_en,		GET_BITS(pp->mv_cost_smooth_en,0,1));
				PE_P0L_H13B0_QWr02(p0l_tnr_ctrl_36,	mv0_protect_th,			GET_BITS(pp->mv0_protect_th,0,7),\
													mv0_protect_en,			GET_BITS(pp->mv0_protect_en,0,1));
				PE_P0L_H13B0_QWr04(p0l_tnr_ctrl_38,	mv_cost_gmv_smooth_gain,	GET_BITS(pp->mv_cost_gmv_smooth_gain,0,5),\
													mv_cost_gmv_smooth_en,		GET_BITS(pp->mv_cost_gmv_smooth_en,0,1),\
													sad_protect_en,				GET_BITS(pp->sad_protect_en,0,1),\
													sad_protect_gm_en,			GET_BITS(pp->sad_protect_gm_en,0,1));
				PE_P0L_H13B0_QWr04(p0l_tnr_ctrl_22,	mv_protect_control_x0,		GET_BITS(pp->mv_protect_control_x0,0,8),\
													mv_protect_control_x1,		GET_BITS(pp->mv_protect_control_x1,0,8),\
													mv_protect_control_y0,		GET_BITS(pp->mv_protect_control_y0,0,8),\
													mv_protect_control_y1,		GET_BITS(pp->mv_protect_control_y1,0,8));
			}
			if(PE_CHECK_WIN1(pp->win_id))
			{
				/* ma: motion */
				PE_P0R_H13B0_QWr02(p0r_tnr_ctrl_38,	m_coring_en_c,		GET_BITS(pp->m_coring_en_c,0,1),\
													m_coring_en_y,		GET_BITS(pp->m_coring_en_y,0,1));
				PE_P0R_H13B0_QWr03(p0r_tnr_ctrl_38,	m_gain_c,			GET_BITS(pp->m_gain_c,0,4),\
													m_gain_y,			GET_BITS(pp->m_gain_y,0,4),\
													m_coring_th,		GET_BITS(pp->m_coring_th,0,6));
				PE_P0R_H13B0_QWr03(p0r_tnr_ctrl_02,	m_offset_mode_en,	GET_BITS(pp->m_offset_mode_en,0,1),\
													m_gain_en_var,		GET_BITS(pp->m_gain_en_var,0,1),\
													a_gain_en_var,		GET_BITS(pp->a_gain_en_var,0,1));
				PE_P0R_H13B0_QWr04(p0r_tnr_ctrl_03,	wa_y0,				GET_BITS(pp->wa_y0,0,8),\
													wa_y1,				GET_BITS(pp->wa_y1,0,8),\
													wa_x0,				GET_BITS(pp->wa_x0,0,8),\
													wa_x1,				GET_BITS(pp->wa_x1,0,8));
				PE_P0R_H13B0_QWr04(p0r_tnr_ctrl_29,	wa_y2,				GET_BITS(pp->wa_y2,0,8),\
													wa_y3,				GET_BITS(pp->wa_y3,0,8),\
													wa_x2,				GET_BITS(pp->wa_x2,0,8),\
													wa_x3,				GET_BITS(pp->wa_x3,0,8));
				/* ma: alpha gain */
				PE_P0R_H13B0_QWr04(p0r_tnr_ctrl_19,	luma_gain_p0_x,			GET_BITS(pp->luma_gain_p0_x,0,8),\
													luma_gain_p1_x,			GET_BITS(pp->luma_gain_p1_x,0,8),\
													luma_gain_p2_x,			GET_BITS(pp->luma_gain_p2_x,0,8),\
													luma_gain_p3_x,			GET_BITS(pp->luma_gain_p3_x,0,8));
				PE_P0R_H13B0_QWr04(p0r_tnr_ctrl_20,	luma_gain_p0_y,			GET_BITS(pp->luma_gain_p0_y,0,8),\
													luma_gain_p1_y,			GET_BITS(pp->luma_gain_p1_y,0,8),\
													luma_gain_p2_y,			GET_BITS(pp->luma_gain_p2_y,0,8),\
													luma_gain_p3_y,			GET_BITS(pp->luma_gain_p3_y,0,8));
				/* ma: alhpa remapping*/
				PE_P0R_H13B0_QWr03(p0r_tnr_ctrl_00,	alpha_avg_en,		GET_BITS(pp->alpha_avg_en,0,1),\
													alpha_mapping_y_en,	GET_BITS(pp->alpha_mapping_y_en,0,8),\
													alpha_mapping_c_en,	GET_BITS(pp->alpha_mapping_c_en,0,8));
				PE_P0R_H13B0_QWr04(p0r_tnr_ctrl_23,	alpha_mapping_y_y0,	GET_BITS(pp->alpha_mapping_y_y0,0,8),\
													alpha_mapping_y_x0,	GET_BITS(pp->alpha_mapping_y_x0,0,8),\
													alpha_mapping_y_y1,	GET_BITS(pp->alpha_mapping_y_y1,0,8),\
													alpha_mapping_y_x1,	GET_BITS(pp->alpha_mapping_y_x1,0,8));
				PE_P0R_H13B0_QWr04(p0r_tnr_ctrl_24,	alpha_mapping_y_y2,	GET_BITS(pp->alpha_mapping_y_y2,0,8),\
													alpha_mapping_y_x2,	GET_BITS(pp->alpha_mapping_y_x2,0,8),\
													alpha_mapping_y_y3,	GET_BITS(pp->alpha_mapping_y_y3,0,8),\
													alpha_mapping_y_x3,	GET_BITS(pp->alpha_mapping_y_x3,0,8));
				PE_P0R_H13B0_QWr04(p0r_tnr_ctrl_25,	alpha_mapping_c_y0,	GET_BITS(pp->alpha_mapping_c_y0,0,8),\
													alpha_mapping_c_x0,	GET_BITS(pp->alpha_mapping_c_x0,0,8),\
													alpha_mapping_c_y1,	GET_BITS(pp->alpha_mapping_c_y1,0,8),\
													alpha_mapping_c_x1,	GET_BITS(pp->alpha_mapping_c_x1,0,8));
				PE_P0R_H13B0_QWr04(p0r_tnr_ctrl_26,	alpha_mapping_c_y2,	GET_BITS(pp->alpha_mapping_c_y2,0,8),\
													alpha_mapping_c_x2,	GET_BITS(pp->alpha_mapping_c_x2,0,8),\
													alpha_mapping_c_y3,	GET_BITS(pp->alpha_mapping_c_y3,0,8),\
													alpha_mapping_c_x3,	GET_BITS(pp->alpha_mapping_c_x3,0,8));
				/*ma: noise level adjust gain*/
				PE_P0R_H13B0_QWr04(p0r_tnr_ctrl_00,	nt_lvl_adjust_gm_enable,		GET_BITS(pp->nt_lvl_adjust_gm_enable,0,1),\
													nt_lvl_adjust_lpct_enable,		GET_BITS(pp->nt_lvl_adjust_lpct_enable,0,1),\
													nt_lvl_adjust_avg_ts_enable,	GET_BITS(pp->nt_lvl_adjust_avg_ts_enable,0,1),\
													nt_lvl_adjust_lpct_sel,			GET_BITS(pp->nt_lvl_adjust_lpct_sel,0,1));
				PE_P0R_H13B0_QWr04(p0r_tnr_ctrl_04,	nt_lvl_gain_gm_y0,		GET_BITS(pp->nt_lvl_gain_gm_y0,0,8),\
													nt_lvl_gain_gm_x0,		GET_BITS(pp->nt_lvl_gain_gm_x0,0,8),\
													nt_lvl_gain_gm_y1,		GET_BITS(pp->nt_lvl_gain_gm_y1,0,8),\
													nt_lvl_gain_gm_x1,		GET_BITS(pp->nt_lvl_gain_gm_x1,0,8));
				PE_P0R_H13B0_QWr04(p0r_tnr_ctrl_12,	nt_lvl_gain_gm_y2,		GET_BITS(pp->nt_lvl_gain_gm_y2,0,8),\
													nt_lvl_gain_gm_x2,		GET_BITS(pp->nt_lvl_gain_gm_x2,0,8),\
													nt_lvl_gain_gm_y3,		GET_BITS(pp->nt_lvl_gain_gm_y3,0,8),\
													nt_lvl_gain_gm_x3,		GET_BITS(pp->nt_lvl_gain_gm_x3,0,8));
				PE_P0R_H13B0_QWr04(p0r_tnr_ctrl_28,	nt_lvl_gain_st_y0,		GET_BITS(pp->nt_lvl_gain_st_y0,0,8),\
													nt_lvl_gain_st_x0,		GET_BITS(pp->nt_lvl_gain_st_x0,0,8),\
													nt_lvl_gain_st_y1,		GET_BITS(pp->nt_lvl_gain_st_y1,0,8),\
													nt_lvl_gain_st_x1,		GET_BITS(pp->nt_lvl_gain_st_x1,0,8));
				PE_P0R_H13B0_QWr04(p0r_tnr_ctrl_30,	nt_lvl_gain_st_y2,		GET_BITS(pp->nt_lvl_gain_st_y2,0,8),\
													nt_lvl_gain_st_x2,		GET_BITS(pp->nt_lvl_gain_st_x2,0,8),\
													nt_lvl_gain_st_y3,		GET_BITS(pp->nt_lvl_gain_st_y3,0,8),\
													nt_lvl_gain_st_x3,		GET_BITS(pp->nt_lvl_gain_st_x3,0,8));
				PE_P0R_H13B0_QWr04(p0r_tnr_ctrl_32,	nt_lvl_gain_lpct_y0,	GET_BITS(pp->nt_lvl_gain_lpct_y0,0,8),\
													nt_lvl_gain_lpct_x0,	GET_BITS(pp->nt_lvl_gain_lpct_x0,0,8),\
													nt_lvl_gain_lpct_y1,	GET_BITS(pp->nt_lvl_gain_lpct_y1,0,8),\
													nt_lvl_gain_lpct_x1,	GET_BITS(pp->nt_lvl_gain_lpct_x1,0,8));
				PE_P0R_H13B0_QWr04(p0r_tnr_ctrl_33,	nt_lvl_gain_lpct_y2,	GET_BITS(pp->nt_lvl_gain_lpct_y2,0,8),\
													nt_lvl_gain_lpct_x2,	GET_BITS(pp->nt_lvl_gain_lpct_x2,0,8),\
													nt_lvl_gain_lpct_y3,	GET_BITS(pp->nt_lvl_gain_lpct_y3,0,8),\
													nt_lvl_gain_lpct_x3,	GET_BITS(pp->nt_lvl_gain_lpct_x3,0,8));
				PE_P0R_H13B0_QWr01(p0r_tnr_ctrl_02,	m_gain_en_var2,			GET_BITS(pp->m_gain_en_var2,0,1));
				PE_P0R_H13B0_QWr04(p0r_tnr_ctrl_48,	m_gain_ctrl2_y0,		GET_BITS(pp->m_gain_ctrl2_y0,0,8),\
													m_gain_ctrl2_x0,		GET_BITS(pp->m_gain_ctrl2_x0,0,8),\
													m_gain_ctrl2_y1,		GET_BITS(pp->m_gain_ctrl2_y1,0,8),\
													m_gain_ctrl2_x1,		GET_BITS(pp->m_gain_ctrl2_x1,0,8));
				PE_P0R_H13B0_QWr03(p0r_tnr_ctrl_40,	sf_map_flt_en,			GET_BITS(pp->sf_map_flt_en,0,1),\
													sf_map_tap,				GET_BITS(pp->sf_map_tap,0,2),\
													sf_map_gain,			GET_BITS(pp->sf_map_gain,0,6));
				PE_P0R_H13B0_QWr02(p0r_tnr_ctrl_40,	sf_th0,					GET_BITS(pp->sf_th0,0,8),\
													sf_th1,					GET_BITS(pp->sf_th1,0,8));
				PE_P0R_H13B0_QWr03(p0r_tnr_ctrl_41,	sf_th2,					GET_BITS(pp->sf_th2,0,8),\
													sf_th3,					GET_BITS(pp->sf_th3,0,8),\
													sf_th4,					GET_BITS(pp->sf_th4,0,8));
				PE_P0R_H13B0_QWr04(p0r_tnr_ctrl_42,	sf_mb_x0,				GET_BITS(pp->sf_mb_x0,0,8),\
													sf_mb_x1,				GET_BITS(pp->sf_mb_x1,0,8),\
													sf_mb_y0,				GET_BITS(pp->sf_mb_y0,0,8),\
													sf_mb_y1,				GET_BITS(pp->sf_mb_y1,0,8));
				PE_P0R_H13B0_QWr04(p0r_tnr_ctrl_44,	sf_mb_x2,				GET_BITS(pp->sf_mb_x2,0,8),\
													sf_mb_x3,				GET_BITS(pp->sf_mb_x3,0,8),\
													sf_mb_y2,				GET_BITS(pp->sf_mb_y2,0,8),\
													sf_mb_y3,				GET_BITS(pp->sf_mb_y3,0,8));
				PE_P0R_H13B0_QWr02(p0r_tnr_ctrl_45,	sf_gain_mode,			GET_BITS(pp->sf_gain_mode,0,1),\
													sf_gain,				GET_BITS(pp->sf_gain,0,4));
				PE_P0R_H13B0_QWr04(p0r_tnr_ctrl_36,	sf_blend_en_y,			GET_BITS(pp->sf_blend_en_y,0,1),\
													sf_blend_en_c,			GET_BITS(pp->sf_blend_en_c,0,1),\
													sf_blend_motion_exp_mode,	GET_BITS(pp->sf_blend_motion_exp_mode,0,2),\
													sad_sel_pels,			GET_BITS(pp->sad_sel_pels,0,1));
				/* mc */
				PE_P0R_H13B0_QWr01(p0r_tnr_ctrl_21,	sad_mamc_blend,		GET_BITS(pp->sad_mamc_blend,0,8));
				PE_P0R_H13B0_QWr01(p0r_tnr_ctrl_00,	mamc_blend_sel,		GET_BITS(pp->mamc_blend_sel,0,1));
				PE_P0R_H13B0_QWr04(p0r_tnr_ctrl_46,	mamc_blend_x0,		GET_BITS(pp->mamc_blend_x0,0,8),\
													mamc_blend_x1,		GET_BITS(pp->mamc_blend_x1,0,8),\
													mamc_blend_y0,		GET_BITS(pp->mamc_blend_y0,0,8),\
													mamc_blend_y1,		GET_BITS(pp->mamc_blend_y1,0,8));
				/* me */
				PE_P0R_H13B0_QWr03(p0r_tnr_ctrl_36,	hme_half_pel_en,		GET_BITS(pp->hme_half_pel_en,0,1),\
													mv_cost_smooth_gain,	GET_BITS(pp->mv_cost_smooth_gain,0,5),\
													mv_cost_smooth_en,		GET_BITS(pp->mv_cost_smooth_en,0,1));
				PE_P0R_H13B0_QWr02(p0r_tnr_ctrl_36,	mv0_protect_th,			GET_BITS(pp->mv0_protect_th,0,7),\
													mv0_protect_en,			GET_BITS(pp->mv0_protect_en,0,1));
				PE_P0R_H13B0_QWr04(p0r_tnr_ctrl_38,	mv_cost_gmv_smooth_gain,	GET_BITS(pp->mv_cost_gmv_smooth_gain,0,5),\
													mv_cost_gmv_smooth_en,		GET_BITS(pp->mv_cost_gmv_smooth_en,0,1),\
													sad_protect_en,				GET_BITS(pp->sad_protect_en,0,1),\
													sad_protect_gm_en,			GET_BITS(pp->sad_protect_gm_en,0,1));
				PE_P0R_H13B0_QWr04(p0r_tnr_ctrl_22,	mv_protect_control_x0,		GET_BITS(pp->mv_protect_control_x0,0,8),\
													mv_protect_control_x1,		GET_BITS(pp->mv_protect_control_x1,0,8),\
													mv_protect_control_y0,		GET_BITS(pp->mv_protect_control_y0,0,8),\
													mv_protect_control_y1,		GET_BITS(pp->mv_protect_control_y1,0,8));
				}
		}
		else
		{
			PE_NRD_HW_H13_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * get tnr detail ctrl
 * - use input struct LX_PE_NRD_TNR2_DETAIL_T for H13BX
 *
 * @param   *pstParams [in/out] void
 * @return  OK if success, ERROR otherwise.
 * @see		set discription
 * @author
 */
int PE_NRD_HW_H13_GetTnrDetailCtrl(void *pstParams)
{
	int ret = RET_OK;
	LX_PE_WIN_ID win_id;
	do{
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H13BX)
		{
			LX_PE_NRD_TNR2_DETAIL_T *pp=(LX_PE_NRD_TNR2_DETAIL_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			win_id = PE_GET_CHECKED_WINID(pp->win_id);
			if(PE_CHECK_WIN0(win_id))
			{
				// ma: motion
				PE_P0L_H13B0_QRd02(p0l_tnr_ctrl_38,	m_coring_en_c,		pp->m_coring_en_c,\
													m_coring_en_y,		pp->m_coring_en_y);
				PE_P0L_H13B0_QRd03(p0l_tnr_ctrl_38,	m_gain_c,			pp->m_gain_c,\
													m_gain_y,			pp->m_gain_y,\
													m_coring_th,		pp->m_coring_th);
				PE_P0L_H13B0_QRd03(p0l_tnr_ctrl_02,	m_offset_mode_en,	pp->m_offset_mode_en,\
													m_gain_en_var,		pp->m_gain_en_var,\
													a_gain_en_var,		pp->a_gain_en_var);
				PE_P0L_H13B0_QRd04(p0l_tnr_ctrl_03,	wa_y0,				pp->wa_y0,\
													wa_y1,				pp->wa_y1,\
													wa_x0,				pp->wa_x0,\
													wa_x1,				pp->wa_x1);
				PE_P0L_H13B0_QRd04(p0l_tnr_ctrl_29,	wa_y2,				pp->wa_y2,\
													wa_y3,				pp->wa_y3,\
													wa_x2,				pp->wa_x2,\
													wa_x3,				pp->wa_x3);
				// ma: alpha gain
				PE_P0L_H13B0_QRd04(p0l_tnr_ctrl_19,	luma_gain_p0_x,		pp->luma_gain_p0_x,\
													luma_gain_p1_x,		pp->luma_gain_p1_x,\
													luma_gain_p2_x,		pp->luma_gain_p2_x,\
													luma_gain_p3_x,		pp->luma_gain_p3_x);
				PE_P0L_H13B0_QRd04(p0l_tnr_ctrl_20,	luma_gain_p0_y,		pp->luma_gain_p0_y,\
													luma_gain_p1_y,		pp->luma_gain_p1_y,\
													luma_gain_p2_y,		pp->luma_gain_p2_y,\
													luma_gain_p3_y,		pp->luma_gain_p3_y);
				// ma: alhpa remapping
				PE_P0L_H13B0_QRd03(p0l_tnr_ctrl_00,	alpha_avg_en,		pp->alpha_avg_en,\
													alpha_mapping_y_en,	pp->alpha_mapping_y_en,\
													alpha_mapping_c_en,	pp->alpha_mapping_c_en);
				PE_P0L_H13B0_QRd04(p0l_tnr_ctrl_23,	alpha_mapping_y_y0,	pp->alpha_mapping_y_y0,\
													alpha_mapping_y_x0,	pp->alpha_mapping_y_x0,\
													alpha_mapping_y_y1,	pp->alpha_mapping_y_y1,\
													alpha_mapping_y_x1,	pp->alpha_mapping_y_x1);
				PE_P0L_H13B0_QRd04(p0l_tnr_ctrl_24,	alpha_mapping_y_y2,	pp->alpha_mapping_y_y2,\
													alpha_mapping_y_x2,	pp->alpha_mapping_y_x2,\
													alpha_mapping_y_y3,	pp->alpha_mapping_y_y3,\
													alpha_mapping_y_x3,	pp->alpha_mapping_y_x3);
				PE_P0L_H13B0_QRd04(p0l_tnr_ctrl_25,	alpha_mapping_c_y0,	pp->alpha_mapping_c_y0,\
													alpha_mapping_c_x0,	pp->alpha_mapping_c_x0,\
													alpha_mapping_c_y1,	pp->alpha_mapping_c_y1,\
													alpha_mapping_c_x1,	pp->alpha_mapping_c_x1);
				PE_P0L_H13B0_QRd04(p0l_tnr_ctrl_26,	alpha_mapping_c_y2,	pp->alpha_mapping_c_y2,\
													alpha_mapping_c_x2,	pp->alpha_mapping_c_x2,\
													alpha_mapping_c_y3,	pp->alpha_mapping_c_y3,\
													alpha_mapping_c_x3,	pp->alpha_mapping_c_x3);
				//ma: noise level adjust gain
				PE_P0L_H13B0_QRd04(p0l_tnr_ctrl_00,	nt_lvl_adjust_gm_enable,		pp->nt_lvl_adjust_gm_enable,\
													nt_lvl_adjust_lpct_enable,		pp->nt_lvl_adjust_lpct_enable,\
													nt_lvl_adjust_avg_ts_enable,	pp->nt_lvl_adjust_avg_ts_enable,\
													nt_lvl_adjust_lpct_sel,			pp->nt_lvl_adjust_lpct_sel);
				PE_P0L_H13B0_QRd04(p0l_tnr_ctrl_04,	nt_lvl_gain_gm_y0,		pp->nt_lvl_gain_gm_y0,\
													nt_lvl_gain_gm_x0,		pp->nt_lvl_gain_gm_x0,\
													nt_lvl_gain_gm_y1,		pp->nt_lvl_gain_gm_y1,\
													nt_lvl_gain_gm_x1,		pp->nt_lvl_gain_gm_x1);
				PE_P0L_H13B0_QRd04(p0l_tnr_ctrl_12,	nt_lvl_gain_gm_y2,		pp->nt_lvl_gain_gm_y2,\
													nt_lvl_gain_gm_x2,		pp->nt_lvl_gain_gm_x2,\
													nt_lvl_gain_gm_y3,		pp->nt_lvl_gain_gm_y3,\
													nt_lvl_gain_gm_x3,		pp->nt_lvl_gain_gm_x3);
				PE_P0L_H13B0_QRd04(p0l_tnr_ctrl_28,	nt_lvl_gain_st_y0,		pp->nt_lvl_gain_st_y0,\
													nt_lvl_gain_st_x0,		pp->nt_lvl_gain_st_x0,\
													nt_lvl_gain_st_y1,		pp->nt_lvl_gain_st_y1,\
													nt_lvl_gain_st_x1,		pp->nt_lvl_gain_st_x1);
				PE_P0L_H13B0_QRd04(p0l_tnr_ctrl_30,	nt_lvl_gain_st_y2,		pp->nt_lvl_gain_st_y2,\
													nt_lvl_gain_st_x2,		pp->nt_lvl_gain_st_x2,\
													nt_lvl_gain_st_y3,		pp->nt_lvl_gain_st_y3,\
													nt_lvl_gain_st_x3,		pp->nt_lvl_gain_st_x3);
				PE_P0L_H13B0_QRd04(p0l_tnr_ctrl_32,	nt_lvl_gain_lpct_y0,	pp->nt_lvl_gain_lpct_y0,\
													nt_lvl_gain_lpct_x0,	pp->nt_lvl_gain_lpct_x0,\
													nt_lvl_gain_lpct_y1,	pp->nt_lvl_gain_lpct_y1,\
													nt_lvl_gain_lpct_x1,	pp->nt_lvl_gain_lpct_x1);
				PE_P0L_H13B0_QRd04(p0l_tnr_ctrl_33,	nt_lvl_gain_lpct_y2,	pp->nt_lvl_gain_lpct_y2,\
													nt_lvl_gain_lpct_x2,	pp->nt_lvl_gain_lpct_x2,\
													nt_lvl_gain_lpct_y3,	pp->nt_lvl_gain_lpct_y3,\
													nt_lvl_gain_lpct_x3,	pp->nt_lvl_gain_lpct_x3);
				PE_P0L_H13B0_QRd01(p0l_tnr_ctrl_02,	m_gain_en_var2,		pp->m_gain_en_var2);
				PE_P0L_H13B0_QRd04(p0l_tnr_ctrl_48,	m_gain_ctrl2_y0,	pp->m_gain_ctrl2_y0,\
													m_gain_ctrl2_x0,	pp->m_gain_ctrl2_x0,\
													m_gain_ctrl2_y1,	pp->m_gain_ctrl2_y1,\
													m_gain_ctrl2_x1,	pp->m_gain_ctrl2_x1);
				PE_P0L_H13B0_QRd03(p0l_tnr_ctrl_40,	sf_map_flt_en,		pp->sf_map_flt_en,\
													sf_map_tap,			pp->sf_map_tap,\
													sf_map_gain,		pp->sf_map_gain);
				PE_P0L_H13B0_QRd02(p0l_tnr_ctrl_40,	sf_th0,				pp->sf_th0,\
													sf_th1,				pp->sf_th1);
				PE_P0L_H13B0_QRd03(p0l_tnr_ctrl_41,	sf_th2,				pp->sf_th2,\
													sf_th3,				pp->sf_th3,\
													sf_th4,				pp->sf_th4);
				PE_P0L_H13B0_QRd04(p0l_tnr_ctrl_42,	sf_mb_x0,			pp->sf_mb_x0,\
													sf_mb_x1,			pp->sf_mb_x1,\
													sf_mb_y0,			pp->sf_mb_y0,\
													sf_mb_y1,			pp->sf_mb_y1);
				PE_P0L_H13B0_QRd04(p0l_tnr_ctrl_44,	sf_mb_x2,			pp->sf_mb_x2,\
													sf_mb_x3,			pp->sf_mb_x3,\
													sf_mb_y2,			pp->sf_mb_y2,\
													sf_mb_y3,			pp->sf_mb_y3);
				PE_P0L_H13B0_QRd02(p0l_tnr_ctrl_45,	sf_gain_mode,		pp->sf_gain_mode,\
													sf_gain,			pp->sf_gain);
				PE_P0L_H13B0_QRd04(p0l_tnr_ctrl_36,	sf_blend_en_y,		pp->sf_blend_en_y,\
													sf_blend_en_c,		pp->sf_blend_en_c,\
													sf_blend_motion_exp_mode,	pp->sf_blend_motion_exp_mode,\
													sad_sel_pels,				pp->sad_sel_pels);
				//mc
				PE_P0L_H13B0_QRd01(p0l_tnr_ctrl_21,	sad_mamc_blend,		pp->sad_mamc_blend);
				PE_P0L_H13B0_QRd01(p0l_tnr_ctrl_00,	mamc_blend_sel,		pp->mamc_blend_sel);
				PE_P0L_H13B0_QRd04(p0l_tnr_ctrl_46,	mamc_blend_x0,		pp->mamc_blend_x0,\
													mamc_blend_x1,		pp->mamc_blend_x1,\
													mamc_blend_y0,		pp->mamc_blend_y0,\
													mamc_blend_y1,		pp->mamc_blend_y1);
				// me
				PE_P0L_H13B0_QRd03(p0l_tnr_ctrl_36,	hme_half_pel_en,		pp->hme_half_pel_en,\
													mv_cost_smooth_gain,	pp->mv_cost_smooth_gain,\
													mv_cost_smooth_en,		pp->mv_cost_smooth_en);
				PE_P0L_H13B0_QRd02(p0l_tnr_ctrl_36,	mv0_protect_th,			pp->mv0_protect_th,\
													mv0_protect_en,			pp->mv0_protect_en);
				PE_P0L_H13B0_QRd04(p0l_tnr_ctrl_38,	mv_cost_gmv_smooth_gain,pp->mv_cost_gmv_smooth_gain,\
													mv_cost_gmv_smooth_en,	pp->mv_cost_gmv_smooth_en,\
													sad_protect_en,			pp->sad_protect_en,\
													sad_protect_gm_en,		pp->sad_protect_gm_en);
				PE_P0L_H13B0_QRd04(p0l_tnr_ctrl_22,	mv_protect_control_x0,	pp->mv_protect_control_x0,\
													mv_protect_control_x1,	pp->mv_protect_control_x1,\
													mv_protect_control_y0,	pp->mv_protect_control_y0,\
													mv_protect_control_y1,	pp->mv_protect_control_y1);

			}
			if(PE_CHECK_WIN1(win_id))
			{
				/* ma: motion */
				PE_P0R_H13B0_QRd02(p0r_tnr_ctrl_38,	m_coring_en_c,		pp->m_coring_en_c,\
													m_coring_en_y,		pp->m_coring_en_y);
				PE_P0R_H13B0_QRd03(p0r_tnr_ctrl_38,	m_gain_c,			pp->m_gain_c,\
													m_gain_y,			pp->m_gain_y,\
													m_coring_th,		pp->m_coring_th);
				PE_P0R_H13B0_QRd03(p0r_tnr_ctrl_02,	m_offset_mode_en,	pp->m_offset_mode_en,\
													m_gain_en_var,		pp->m_gain_en_var,\
													a_gain_en_var,		pp->a_gain_en_var);
				PE_P0R_H13B0_QRd04(p0r_tnr_ctrl_03,	wa_y0,				pp->wa_y0,\
													wa_y1,				pp->wa_y1,\
													wa_x0,				pp->wa_x0,\
													wa_x1,				pp->wa_x1);
				PE_P0R_H13B0_QRd04(p0r_tnr_ctrl_29,	wa_y2,				pp->wa_y2,\
													wa_y3,				pp->wa_y3,\
													wa_x2,				pp->wa_x2,\
													wa_x3,				pp->wa_x3);
				/* ma: alpha gain */
				PE_P0R_H13B0_QRd04(p0r_tnr_ctrl_19,	luma_gain_p0_x,		pp->luma_gain_p0_x,\
													luma_gain_p1_x,		pp->luma_gain_p1_x,\
													luma_gain_p2_x,		pp->luma_gain_p2_x,\
													luma_gain_p3_x,		pp->luma_gain_p3_x);
				PE_P0R_H13B0_QRd04(p0r_tnr_ctrl_20,	luma_gain_p0_y,		pp->luma_gain_p0_y,\
													luma_gain_p1_y,		pp->luma_gain_p1_y,\
													luma_gain_p2_y,		pp->luma_gain_p2_y,\
													luma_gain_p3_y,		pp->luma_gain_p3_y);
				/* ma: alhpa remapping*/
				PE_P0R_H13B0_QRd03(p0r_tnr_ctrl_00,	alpha_avg_en,		pp->alpha_avg_en,\
													alpha_mapping_y_en,	pp->alpha_mapping_y_en,\
													alpha_mapping_c_en,	pp->alpha_mapping_c_en);
				PE_P0R_H13B0_QRd04(p0r_tnr_ctrl_23,	alpha_mapping_y_y0,	pp->alpha_mapping_y_y0,\
													alpha_mapping_y_x0,	pp->alpha_mapping_y_x0,\
													alpha_mapping_y_y1,	pp->alpha_mapping_y_y1,\
													alpha_mapping_y_x1,	pp->alpha_mapping_y_x1);
				PE_P0R_H13B0_QRd04(p0r_tnr_ctrl_24,	alpha_mapping_y_y2,	pp->alpha_mapping_y_y2,\
													alpha_mapping_y_x2,	pp->alpha_mapping_y_x2,\
													alpha_mapping_y_y3,	pp->alpha_mapping_y_y3,\
													alpha_mapping_y_x3,	pp->alpha_mapping_y_x3);
				PE_P0R_H13B0_QRd04(p0r_tnr_ctrl_25,	alpha_mapping_c_y0,	pp->alpha_mapping_c_y0,\
													alpha_mapping_c_x0,	pp->alpha_mapping_c_x0,\
													alpha_mapping_c_y1,	pp->alpha_mapping_c_y1,\
													alpha_mapping_c_x1,	pp->alpha_mapping_c_x1);
				PE_P0R_H13B0_QRd04(p0r_tnr_ctrl_26,	alpha_mapping_c_y2,	pp->alpha_mapping_c_y2,\
													alpha_mapping_c_x2,	pp->alpha_mapping_c_x2,\
													alpha_mapping_c_y3,	pp->alpha_mapping_c_y3,\
													alpha_mapping_c_x3,	pp->alpha_mapping_c_x3);
				/*ma: noise level adjust gain*/
				PE_P0R_H13B0_QRd04(p0r_tnr_ctrl_00,	nt_lvl_adjust_gm_enable,		pp->nt_lvl_adjust_gm_enable,\
													nt_lvl_adjust_lpct_enable,		pp->nt_lvl_adjust_lpct_enable,\
													nt_lvl_adjust_avg_ts_enable,	pp->nt_lvl_adjust_avg_ts_enable,\
													nt_lvl_adjust_lpct_sel,			pp->nt_lvl_adjust_lpct_sel);
				PE_P0R_H13B0_QRd04(p0r_tnr_ctrl_04,	nt_lvl_gain_gm_y0,		pp->nt_lvl_gain_gm_y0,\
													nt_lvl_gain_gm_x0,		pp->nt_lvl_gain_gm_x0,\
													nt_lvl_gain_gm_y1,		pp->nt_lvl_gain_gm_y1,\
													nt_lvl_gain_gm_x1,		pp->nt_lvl_gain_gm_x1);
				PE_P0R_H13B0_QRd04(p0r_tnr_ctrl_12,	nt_lvl_gain_gm_y2,		pp->nt_lvl_gain_gm_y2,\
													nt_lvl_gain_gm_x2,		pp->nt_lvl_gain_gm_x2,\
													nt_lvl_gain_gm_y3,		pp->nt_lvl_gain_gm_y3,\
													nt_lvl_gain_gm_x3,		pp->nt_lvl_gain_gm_x3);
				PE_P0R_H13B0_QRd04(p0r_tnr_ctrl_28,	nt_lvl_gain_st_y0,		pp->nt_lvl_gain_st_y0,\
													nt_lvl_gain_st_x0,		pp->nt_lvl_gain_st_x0,\
													nt_lvl_gain_st_y1,		pp->nt_lvl_gain_st_y1,\
													nt_lvl_gain_st_x1,		pp->nt_lvl_gain_st_x1);
				PE_P0R_H13B0_QRd04(p0r_tnr_ctrl_30,	nt_lvl_gain_st_y2,		pp->nt_lvl_gain_st_y2,\
													nt_lvl_gain_st_x2,		pp->nt_lvl_gain_st_x2,\
													nt_lvl_gain_st_y3,		pp->nt_lvl_gain_st_y3,\
													nt_lvl_gain_st_x3,		pp->nt_lvl_gain_st_x3);
				PE_P0R_H13B0_QRd04(p0r_tnr_ctrl_32,	nt_lvl_gain_lpct_y0,	pp->nt_lvl_gain_lpct_y0,\
													nt_lvl_gain_lpct_x0,	pp->nt_lvl_gain_lpct_x0,\
													nt_lvl_gain_lpct_y1,	pp->nt_lvl_gain_lpct_y1,\
													nt_lvl_gain_lpct_x1,	pp->nt_lvl_gain_lpct_x1);
				PE_P0R_H13B0_QRd04(p0r_tnr_ctrl_33,	nt_lvl_gain_lpct_y2,	pp->nt_lvl_gain_lpct_y2,\
													nt_lvl_gain_lpct_x2,	pp->nt_lvl_gain_lpct_x2,\
													nt_lvl_gain_lpct_y3,	pp->nt_lvl_gain_lpct_y3,\
													nt_lvl_gain_lpct_x3,	pp->nt_lvl_gain_lpct_x3);
				PE_P0R_H13B0_QRd01(p0r_tnr_ctrl_02,	m_gain_en_var2,			pp->m_gain_en_var2);
				PE_P0R_H13B0_QRd04(p0r_tnr_ctrl_48,	m_gain_ctrl2_y0,		pp->m_gain_ctrl2_y0,\
													m_gain_ctrl2_x0,		pp->m_gain_ctrl2_x0,\
													m_gain_ctrl2_y1,		pp->m_gain_ctrl2_y1,\
													m_gain_ctrl2_x1,		pp->m_gain_ctrl2_x1);
				PE_P0R_H13B0_QRd03(p0r_tnr_ctrl_40,	sf_map_flt_en,			pp->sf_map_flt_en,\
													sf_map_tap,				pp->sf_map_tap,\
													sf_map_gain,			pp->sf_map_gain);
				PE_P0R_H13B0_QRd02(p0r_tnr_ctrl_40,	sf_th0,					pp->sf_th0,\
													sf_th1,					pp->sf_th1);
				PE_P0R_H13B0_QRd03(p0r_tnr_ctrl_41,	sf_th2,					pp->sf_th2,\
													sf_th3,					pp->sf_th3,\
													sf_th4,					pp->sf_th4);
				PE_P0R_H13B0_QRd04(p0r_tnr_ctrl_42,	sf_mb_x0,				pp->sf_mb_x0,\
													sf_mb_x1,				pp->sf_mb_x1,\
													sf_mb_y0,				pp->sf_mb_y0,\
													sf_mb_y1,				pp->sf_mb_y1);
				PE_P0R_H13B0_QRd04(p0r_tnr_ctrl_44,	sf_mb_x2,				pp->sf_mb_x2,\
													sf_mb_x3,				pp->sf_mb_x3,\
													sf_mb_y2,				pp->sf_mb_y2,\
													sf_mb_y3,				pp->sf_mb_y3);
				PE_P0R_H13B0_QRd02(p0r_tnr_ctrl_45,	sf_gain_mode,			pp->sf_gain_mode,\
													sf_gain,				pp->sf_gain);
				PE_P0R_H13B0_QRd04(p0r_tnr_ctrl_36,	sf_blend_en_y,			pp->sf_blend_en_y,\
													sf_blend_en_c,			pp->sf_blend_en_c,\
													sf_blend_motion_exp_mode,	pp->sf_blend_motion_exp_mode,\
													sad_sel_pels,			pp->sad_sel_pels);
				/* mc */
				PE_P0R_H13B0_QRd01(p0r_tnr_ctrl_21,	sad_mamc_blend,		pp->sad_mamc_blend);
				PE_P0R_H13B0_QRd01(p0r_tnr_ctrl_00,	mamc_blend_sel,		pp->mamc_blend_sel);
				PE_P0R_H13B0_QRd04(p0r_tnr_ctrl_46,	mamc_blend_x0,		pp->mamc_blend_x0,\
													mamc_blend_x1,		pp->mamc_blend_x1,\
													mamc_blend_y0,		pp->mamc_blend_y0,\
													mamc_blend_y1,		pp->mamc_blend_y1);
				/* me */
				PE_P0R_H13B0_QRd03(p0r_tnr_ctrl_36,	hme_half_pel_en,		pp->hme_half_pel_en,\
													mv_cost_smooth_gain,	pp->mv_cost_smooth_gain,\
													mv_cost_smooth_en,		pp->mv_cost_smooth_en);
				PE_P0R_H13B0_QRd02(p0r_tnr_ctrl_36,	mv0_protect_th,			pp->mv0_protect_th,\
													mv0_protect_en,			pp->mv0_protect_en);
				PE_P0R_H13B0_QRd04(p0r_tnr_ctrl_38,	mv_cost_gmv_smooth_gain,pp->mv_cost_gmv_smooth_gain,\
													mv_cost_gmv_smooth_en,	pp->mv_cost_gmv_smooth_en,\
													sad_protect_en,			pp->sad_protect_en,\
													sad_protect_gm_en,		pp->sad_protect_gm_en);
				PE_P0R_H13B0_QRd04(p0r_tnr_ctrl_22,	mv_protect_control_x0,	pp->mv_protect_control_x0,\
													mv_protect_control_x1,	pp->mv_protect_control_x1,\
													mv_protect_control_y0,	pp->mv_protect_control_y0,\
													mv_protect_control_y1,	pp->mv_protect_control_y1);
			}
			PE_NRD_HW_H13_DBG_PRINT("get[%d] \n"\
				"ma:\nmotion: 	 coring_en (y,c): %d,%d, gain (y,c): %d,%d,\n"\
					 "         	 coring_th: %d, offset_en: %d, gain_en_var (m,a): %d,%d\n"\
					 "         	 wa_y(0,1,2,3):%d,%d,%d,%d, wa_x(0,1,2,3):%d,%d,%d,%d\n"\
					 "alph_gain: gain_x(p0,p1,p2,p3):%d,%d,%d,%d, gain_y(p0,p1,p2,p3):%d,%d,%d,%d\n"\
					 "alph_remap:ave_en:%d, mapping_y_en:%d, mapping_c_en:%d\n"\
					 "           map_y(x0,x1,x2,x3):%d,%d,%d,%d, map_y(y0,y1,y2,y3):%d,%d,%d,%d\n"\
					 "           map_c(x0,x1,x2,x3):%d,%d,%d,%d, map_c(y0,y1,y2,y3):%d,%d,%d,%d\n"\
				     "adj_gain:  gm_en:%d, lpct:%d, avg_ts_en:%d, lpct_sel:%d\n"\
				     "           gm(x0,x1,x2,x3):%d,%d,%d,%d, gm(y0,y1,y2,y3):%d,%d,%d,%d\n"\
				     "           st(x0,x1,x2,x3):%d,%d,%d,%d, st(y0,y1,y2,y3):%d,%d,%d,%d\n"\
				     "           lpct(x0,x1,x2,x3):%d,%d,%d,%d, lpct(y0,y1,y2,y3):%d,%d,%d,%d\n"\
				     "           m_gain_en_var2:%d,\n"\
				     "           ctrl(x0,x1,y0,y1):%d,%d,%d,%d\n"\
				     "           sf_map_flt_en:%d, map_tap:%d, map_gain:%d\n"\
				     "           sf_th(0,1,2,3,4):%d,%d,%d,%d,%d\n"\
				     "           sf_mb(x0,x1,x2,x3):%d,%d,%d,%d, sf_mb(y0,y1,y2,y3):%d,%d,%d,%d\n"\
				     "           sf_gain_md:%d, sf_gain:%d, sf_en(y,c):%d,%d, sf_mo_exp_md:%d\n"\
				"mc:\n           sel_pels:%d, mamc_blend:%d, blend_sel:%d\n"\
				     "           blend(x0,x1,y0,y1):%d,%d,%d,%d\n"\
				"me:\n           hlf_pel_en:%d, smth(gain,en):%d,%d, gmv_smth(gain,en):%d,%d\n"\
				     "           mv0_th:%d, mv0_en:%d, sad_en:%d, sad_gm_en:%d\n"\
				     "           ctrl(x0,x1,y0,y1):%d,%d,%d,%d\n",\
				pp->win_id,\
				// ma: motion
				pp->m_coring_en_y,pp->m_coring_en_c,pp->m_gain_y,pp->m_gain_c,\
				pp->m_coring_th,pp->m_offset_mode_en,pp->m_gain_en_var,pp->a_gain_en_var,\
				pp->wa_y0,pp->wa_y1,pp->wa_y2,pp->wa_y3,pp->wa_x0,pp->wa_x1,pp->wa_x2,pp->wa_x3,\
				// ma: alpha gain
				pp->luma_gain_p0_x,pp->luma_gain_p1_x,pp->luma_gain_p2_x,pp->luma_gain_p3_x,\
				pp->luma_gain_p0_y,pp->luma_gain_p1_y,pp->luma_gain_p2_y,pp->luma_gain_p3_y,\
				// alpha remapping
				pp->alpha_avg_en,pp->alpha_mapping_y_en,pp->alpha_mapping_c_en,\
				pp->alpha_mapping_y_x0,pp->alpha_mapping_y_x1,pp->alpha_mapping_y_x2,pp->alpha_mapping_y_x3,\
				pp->alpha_mapping_y_y0,pp->alpha_mapping_y_y1,pp->alpha_mapping_y_y2,pp->alpha_mapping_y_y3,\
				pp->alpha_mapping_c_x0,pp->alpha_mapping_c_x1,pp->alpha_mapping_c_x2,pp->alpha_mapping_c_x3,\
				pp->alpha_mapping_c_y0,pp->alpha_mapping_c_y1,pp->alpha_mapping_c_y2,pp->alpha_mapping_c_y3,\
				// adjust gain
				pp->nt_lvl_adjust_gm_enable,pp->nt_lvl_adjust_lpct_enable,\
				pp->nt_lvl_adjust_avg_ts_enable,pp->nt_lvl_adjust_lpct_sel,\
				pp->nt_lvl_gain_gm_x0,pp->nt_lvl_gain_gm_x1,pp->nt_lvl_gain_gm_x2,pp->nt_lvl_gain_gm_x3,\
				pp->nt_lvl_gain_gm_y0,pp->nt_lvl_gain_gm_y1,pp->nt_lvl_gain_gm_y2,pp->nt_lvl_gain_gm_y3,\
				pp->nt_lvl_gain_st_x0,pp->nt_lvl_gain_st_x1,pp->nt_lvl_gain_st_x2,pp->nt_lvl_gain_st_x3,\
				pp->nt_lvl_gain_st_y0,pp->nt_lvl_gain_st_y1,pp->nt_lvl_gain_st_y2,pp->nt_lvl_gain_st_y3,\
				pp->nt_lvl_gain_lpct_x0,pp->nt_lvl_gain_lpct_x1,pp->nt_lvl_gain_lpct_x2,pp->nt_lvl_gain_lpct_x3,\
				pp->nt_lvl_gain_lpct_y0,pp->nt_lvl_gain_lpct_y1,pp->nt_lvl_gain_lpct_y2,pp->nt_lvl_gain_lpct_y3,\
				pp->m_gain_en_var2,\
				pp->m_gain_ctrl2_x0,pp->m_gain_ctrl2_x1,pp->m_gain_ctrl2_y0,pp->m_gain_ctrl2_y1,\
				pp->sf_map_flt_en,pp->sf_map_tap,pp->sf_map_gain,\
				pp->sf_th0,pp->sf_th1,pp->sf_th2,pp->sf_th3,pp->sf_th4,\
				pp->sf_mb_x0,pp->sf_mb_x1,pp->sf_mb_x2,pp->sf_mb_x3,\
				pp->sf_mb_y0,pp->sf_mb_y1,pp->sf_mb_y2,pp->sf_mb_y3,\
				pp->sf_gain_mode,pp->sf_gain,pp->sf_blend_en_y,pp->sf_blend_en_c,pp->sf_blend_motion_exp_mode,\
				// mc:
				pp->sad_sel_pels,pp->sad_mamc_blend,pp->mamc_blend_sel,\
				pp->mamc_blend_x0,pp->mamc_blend_x1,pp->mamc_blend_y0,pp->mamc_blend_y1,\
				// me:
				pp->hme_half_pel_en,pp->mv_cost_smooth_gain,pp->mv_cost_smooth_en,\
				pp->mv_cost_gmv_smooth_gain,pp->mv_cost_gmv_smooth_en,\
				pp->mv0_protect_th,pp->mv0_protect_en,pp->sad_protect_en,pp->sad_protect_gm_en,\
				pp->mv_protect_control_x0,pp->mv_protect_control_x1,\
				pp->mv_protect_control_y0,pp->mv_protect_control_y1);

		}
		else
		{
			PE_NRD_HW_H13_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}

/**
 * set dnr mode
 *
 * @param   *pCfg [in] PE_TSK_H13_CFG_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_NRD_HW_H13_SetDnrMode(PE_TSK_H13_CFG_T *pCfg)
{
	int ret = RET_OK;
	static UINT32 count = 0;
	static UINT32 pre_bnr_ac_h_en0 = 0xff;		//pre bnr_ac_h_en0
	static UINT32 pre_bnr_ac_h_en1 = 0xff;		//pre bnr_ac_h_en1
	UINT32 cur_bnr_ac_h_en0 = 0;
	UINT32 cur_bnr_ac_h_en1 = 0;
	PE_INF_H13_CTRL_T *ctrl0_info = &pCfg->inf.ctrl_info[LX_PE_WIN_0];
	PE_INF_H13_CTRL_T *ctrl1_info = &pCfg->inf.ctrl_info[LX_PE_WIN_1];
	LX_PE_INF_DISPLAY_T *disp0_info = &pCfg->inf.disp_info[LX_PE_WIN_0];
	LX_PE_INF_DISPLAY_T *disp1_info = &pCfg->inf.disp_info[LX_PE_WIN_1];
	PE_NRD_HW_H13_SETTINGS_T *pInfo = &_g_pe_nrd_hw_h13_info;
	do{
		CHECK_KNULL(pCfg);
		if(PE_KDRV_VER_H13BX)
		{
			/* win 0 */
			cur_bnr_ac_h_en0 = (disp0_info->src_type == LX_PE_SRC_DTV && \
				disp0_info->dtv_type == LX_PE_DTV_FILEPLAY && \
				ctrl0_info->pe0_src_h <= PE_NRD_HW_H13_H_MIN && \
				ctrl0_info->pe0_src_v <= PE_NRD_HW_H13_V_MIN)? \
				0x0:pInfo->reg_bnr_ac_h_en0;
			/* compare pre vs. cur bnr_ac_h_en */
			if (pre_bnr_ac_h_en0 != cur_bnr_ac_h_en0)
			{
				PE_NRD_HW_H13_DBG_PRINT("[inf0] src:%d,dtv:%d, h,v:%d,%d, en:%d\n",\
					disp0_info->src_type, disp0_info->dtv_type, \
					ctrl0_info->pe0_src_h, ctrl0_info->pe0_src_v, \
					pInfo->reg_bnr_ac_h_en0);
				PE_NRD_HW_H13_DBG_PRINT("[set0] bnr_ac_h_en : %d -> %d\n",\
					pre_bnr_ac_h_en0, cur_bnr_ac_h_en0);
				PE_P0L_H13B0_QWr01(p0l_bnr_ac_ctrl_00,reg_bnr_ac_h_en,cur_bnr_ac_h_en0);
				pre_bnr_ac_h_en0 = cur_bnr_ac_h_en0;
			}
			/* win 1 */
			cur_bnr_ac_h_en1 = (disp1_info->src_type == LX_PE_SRC_DTV && \
				disp1_info->dtv_type == LX_PE_DTV_FILEPLAY && \
				ctrl1_info->pe0_src_h <= PE_NRD_HW_H13_H_MIN && \
				ctrl1_info->pe0_src_v <= PE_NRD_HW_H13_V_MIN)? \
				0x0:pInfo->reg_bnr_ac_h_en1;
			/* compare pre vs. cur bnr_ac_h_en */
			if (pre_bnr_ac_h_en1 != cur_bnr_ac_h_en1)
			{
				PE_NRD_HW_H13_DBG_PRINT("[inf1] src:%d,dtv:%d, h,v:%d,%d, en:%d\n",\
					disp1_info->src_type, disp1_info->dtv_type, \
					ctrl1_info->pe0_src_h, ctrl1_info->pe0_src_v, \
					pInfo->reg_bnr_ac_h_en1);
				PE_NRD_HW_H13_DBG_PRINT("[set1] bnr_ac_h_en : %d -> %d\n",\
					pre_bnr_ac_h_en1, cur_bnr_ac_h_en1);
				PE_P0R_H13B0_QWr01(p0r_bnr_ac_ctrl_00,reg_bnr_ac_h_en,cur_bnr_ac_h_en1);
				pre_bnr_ac_h_en1 = cur_bnr_ac_h_en1;
			}
			if(_g_nrd_hw_h13_trace)
			{
				if(count>PE_PRINT_COUNT_NUM)
				{
					PE_NRD_HW_H13_DBG_PRINT("[state0] src:%d,dtv:%d, "\
						"pe0 h,v:%d,%d, en:%d, bnr_ac_h_en:%d\n", \
						disp0_info->src_type, disp0_info->dtv_type, \
						ctrl0_info->pe0_src_h, ctrl0_info->pe0_src_v, \
						pInfo->reg_bnr_ac_h_en0, cur_bnr_ac_h_en0);
					PE_NRD_HW_H13_DBG_PRINT("[state1] src:%d,dtv:%d, "\
						"pe0 h,v:%d,%d, en:%d, bnr_ac_h_en:%d\n", \
						disp1_info->src_type, disp1_info->dtv_type, \
						ctrl1_info->pe0_src_h, ctrl1_info->pe0_src_v, \
						pInfo->reg_bnr_ac_h_en1, cur_bnr_ac_h_en1);
					count=0;
				}
				count++;
			}
		}
		else
		{
			PE_NRD_HW_H13_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}

