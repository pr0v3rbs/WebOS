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

/** @file pe_inf_h13.c
 *
 *  information management for picture enhance. ( used only within kdriver )
 *	
 *	@author		Seung-Jun,Youm(sj.youm@lge.com)
 *	@version	0.1
 *	@note		
 *	@date		2012.04.15
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
#include <linux/kthread.h>
#include <asm/io.h>

#include "base_types.h"
#include "os_util.h"

#include "pe_hw_h13.h"
#include "pe_reg_h13.h"
#include "pe_inf_h13.h"
#include "pe_fwi_h13.h"
#include "pe_win_hw_h13.h"
#include "pe_shp_hw_h13.h"
#include "pe_ccm_hw_h13.h"
#include "pe_hst_hw_h13.h"
#include "pe_cmn_hw_h13.h"
#include "pe_dnt_hw_h13.h"
#include "pe_nrd_hw_h13.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
/* drv ver, yyyy/mm/dd/hh:mm */
#define PE_INF_H13_DRV_VER		"2014/09/22/23:00"

#define PE_INF_H13_SCL_RESIZE_H_MAX		960		/* 1920 * 0.5 */
#define PE_INF_H13_SCL_RESIZE_V_MAX		540		/* 1080 * 0.5 */
#define PE_INF_H13_SCL_IS_HD_MIN		1024

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define PE_INF_H13_ERROR	printk

#define PE_INF_H13_DBG_PRINT(fmt,args...)	\
	if(_g_pe_inf_h13_trace) printk("[%x,%x][%s,%d] "fmt,PE_CHIP_VER,g_pe_kdrv_ver_mask,__F__,__L__,##args)
#define PE_INF_H13_CHECK_CODE(_checker,_action,fmt,args...)	\
	{if(_checker){PE_INF_H13_ERROR(fmt,##args);_action;}}

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
static int PE_INF_H13_GetGeneralPicInfo(LX_PE_ETC_TBL_T *pstParams);
static int PE_INF_H13_SetPathMonitor(LX_PE_ETC_TBL_T *pstParams);
static int PE_INF_H13_GetPathMonitor(LX_PE_ETC_TBL_T *pstParams);

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
static UINT32 _g_pe_inf_h13_trace=0x0;		//default should be off.
static PE_INF_H13_SETTINGS_T _g_pe_inf_hw_h13_info;

static PE_ETC_MISC_TBL_T _g_pe_inf_h13_dbg_set_fn[] = {\
	{/* 00 */ PE_TSK_H13_SetTaskOperation,		"PE_TSK_H13_SetTaskOperation"}, \
	{/* 01 */ PE_REG_H13_WrReg,					"PE_REG_H13_WrReg"}, \
	{/* 02 */ PE_INF_H13_SetPathMonitor,		"PE_INF_H13_SetPathMonitor"}};
static PE_ETC_MISC_TBL_T _g_pe_inf_h13_dbg_get_fn[] = {\
	{/* 00 */ PE_TSK_H13_GetTaskOperation,		"PE_TSK_H13_GetTaskOperation"}, \
	{/* 01 */ PE_REG_H13_RdReg,					"PE_REG_H13_RdReg"}, \
	{/* 02 */ PE_INF_H13_GetGeneralPicInfo,		"PE_INF_H13_GetGeneralPicInfo"}, \
	{/* 03 */ PE_HST_HW_H13_GetGlobalHistInfo,	"PE_HST_HW_H13_GetGlobalHistInfo"}, \
	{/* 04 */ PE_INF_H13_GetPathMonitor,		"PE_INF_H13_GetPathMonitor"}, \
	{/* 05 */ PE_SHP_HW_H13_RdSclFltCoeff,		"PE_SHP_HW_H13_RdSclFltCoeff"}, \
	{/* 06 */ PE_DNT_HW_H13_RdDefault,			"PE_DNT_HW_H13_RdDefault"}, \
	{/* 07 */ PE_NRD_HW_H13_RdDefault,			"PE_NRD_HW_H13_RdDefault"}, \
	{/* 08 */ PE_SHP_HW_H13_RdDefault,			"PE_SHP_HW_H13_RdDefault"}
};

/*========================================================================================
	Implementation Group
========================================================================================*/
/**
 * init inf
 *
 * @param   *pstParams [in] PE_CFG_CTRL_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_INF_H13_Init(PE_CFG_CTRL_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H13)
		{
			PE_INF_H13_DBG_PRINT("init global values.\n");
			memset0(_g_pe_inf_hw_h13_info);
		}
		else
		{
			PE_INF_H13_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
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
int PE_INF_H13_SetDebugSettings(LX_PE_DBG_SETTINGS_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		PE_INF_H13_DBG_PRINT("wid:%d,type:0x%x,[0x%x]print:0x%x,bypass:0x%x\n",\
			pstParams->win_id,pstParams->type,(0x1<<PE_ITEM_PKTMODL(ETC)),\
			pstParams->print_lvl,pstParams->bypass);
		/* set print level */
		if(pstParams->type&LX_PE_DBG_LV)
		{
			_g_pe_inf_h13_trace = (pstParams->print_lvl & (0x1<<PE_ITEM_PKTMODL(ETC)))? 0x1:0x0;
		}
	}while(0);
	return ret;
}
/**
 * set display info
 *
 * @param   *pstParams [in] LX_PE_INF_DISPLAY_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_INF_H13_SetDisplayInfo(LX_PE_INF_DISPLAY_T *pstParams)
{
	int ret = RET_OK;
	PE_FWI_H13_SRC_INFO fwi_src;
	LX_PE_INF_DISPLAY_T *pp = NULL;
	LX_PE_INF_DISPLAY_T *disp0_info = &_g_pe_inf_hw_h13_info.disp_info[LX_PE_WIN_0];
	LX_PE_INF_DISPLAY_T *disp1_info = &_g_pe_inf_hw_h13_info.disp_info[LX_PE_WIN_1];
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		pp = pstParams;
		PE_INF_H13_DBG_PRINT("set[%d] : src:%d,res:%d (h:%d,v:%d)\n"\
			"fr:%d(%d),scan:%d,cstd:%d,hdmi:%d,scart:%d,dtv:%d,hdd:%d\n"\
			"out:%d,in:%d,user_o:%d,user_i:%d\n"\
			"in_win:%d,%d,%d,%d,out_win:%d,%d,%d,%d,af:%d\n"\
			"mode: tp:%d,venc:%d,adpt_st:%d,ext_frc:%d,wb_wog:%d,rev:%d\n",\
			pp->win_id, pp->src_type, pp->fmt_type, pp->in_h_size, pp->in_v_size, \
			pp->fr_type, pp->in_f_rate, pp->scan_type, pp->cstd_type, \
			pp->hdmi_type, pp->scart_type, pp->dtv_type, pp->hdd_type, \
			pp->out_type, pp->in_type, pp->user_o_type, pp->user_i_type, \
			pp->in_win.x_ofst, pp->in_win.y_ofst, \
			pp->in_win.h_size, pp->in_win.v_size, \
			pp->out_win.x_ofst, pp->out_win.y_ofst, \
			pp->out_win.h_size, pp->out_win.v_size, pp->act_fmt, \
			pp->mode.is_tp, pp->mode.is_venc, \
			pp->mode.is_adpt_st, pp->mode.is_ext_frc, \
			pp->mode.is_wb_wog, pp->mode.is_reverse);
		if(PE_KDRV_VER_H13)
		{
			if(PE_CHECK_WIN0(pstParams->win_id))
			{
				memcpy(disp0_info,pstParams,sizeof(LX_PE_INF_DISPLAY_T));
			}
			if(PE_CHECK_WIN1(pstParams->win_id))
			{
				memcpy(disp1_info,pstParams,sizeof(LX_PE_INF_DISPLAY_T));
			}
			/* sync parameters with no relationship to window */
			disp0_info->out_type	= disp1_info->out_type	= pstParams->out_type;
			disp0_info->hdd_type	= disp1_info->hdd_type	= pstParams->hdd_type;
			disp0_info->in_type		= disp1_info->in_type	= pstParams->in_type;
			disp0_info->user_o_type	= disp1_info->user_o_type	= pstParams->user_o_type;
			disp0_info->user_i_type	= disp1_info->user_i_type	= pstParams->user_i_type;
			disp0_info->act_fmt		= disp1_info->act_fmt		= pstParams->act_fmt;
			disp0_info->mode		= disp1_info->mode		= pstParams->mode;
			/* sync parameters when all win case(ud,3d) */
			if(pstParams->win_id==LX_PE_WIN_ALL)
			{
				disp0_info->src_type	= disp1_info->src_type	= pstParams->src_type;
				disp0_info->fmt_type	= disp1_info->fmt_type	= pstParams->fmt_type;
				disp0_info->fr_type		= disp1_info->fr_type	= pstParams->fr_type;
				disp0_info->scan_type	= disp1_info->scan_type	= pstParams->scan_type;
				disp0_info->dtv_type	= disp1_info->dtv_type	= pstParams->dtv_type;
				disp0_info->cstd_type	= disp1_info->cstd_type	= pstParams->cstd_type;
				disp0_info->hdmi_type	= disp1_info->hdmi_type	= pstParams->hdmi_type;
				disp0_info->scart_type	= disp1_info->scart_type= pstParams->scart_type;
			}
			if(PE_CHECK_WIN0(pstParams->win_id))
			{
				fwi_src.dtv_mode = pstParams->dtv_type;
				fwi_src.hdd_mode = pstParams->hdd_type;
				ret = PE_FWI_H13_SetSrcInfo(&fwi_src);
				PE_INF_H13_CHECK_CODE(ret,break,"[%s,%d] PE_FWI_H13_SetSrcInfo() error.\n",__F__,__L__);
			}
		}
		else
		{
			PE_INF_H13_DBG_PRINT("nothing to do\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * get display info
 *
 * @param   *pstParams [in/out] LX_PE_INF_DISPLAY_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_INF_H13_GetDisplayInfo(LX_PE_INF_DISPLAY_T *pstParams)
{
	int ret = RET_OK;
	LX_PE_WIN_ID win_id,temp;
	PE_INF_H13_SETTINGS_T *pInfo=&_g_pe_inf_hw_h13_info;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		win_id = PE_GET_CHECKED_WINID(pstParams->win_id);
		if(PE_KDRV_VER_H13)
		{
			if(PE_CHECK_WIN0(win_id))
			{
				temp = pstParams->win_id;
				memcpy(pstParams,&pInfo->disp_info[LX_PE_WIN_0],sizeof(LX_PE_INF_DISPLAY_T));
				pstParams->win_id = temp;
			}
			if(PE_CHECK_WIN1(win_id))
			{
				temp = pstParams->win_id;
				memcpy(pstParams,&pInfo->disp_info[LX_PE_WIN_1],sizeof(LX_PE_INF_DISPLAY_T));
				pstParams->win_id = temp;
			}
		}
		else
		{
			PE_INF_H13_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * check(get) scaler info.
 *
 * @param   void
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_INF_H13_CheckSclInfo(void)
{
	int ret = RET_OK;
	UINT32 r_data;
	PE_INF_H13_CTRL_T *ctrl0_info = &_g_pe_inf_hw_h13_info.ctrl_info[LX_PE_WIN_0];
	PE_INF_H13_CTRL_T *ctrl1_info = &_g_pe_inf_hw_h13_info.ctrl_info[LX_PE_WIN_1];
	PE_CCM_HW_H13_SETTINGS_T ccm_info;
	do{
		if(PE_KDRV_VER_H13BX)
		{
			ret = PE_CCM_HW_H13_GetCurCcmSettings(&ccm_info);
			if(ret)	{PE_INF_H13_ERROR("[%s,%d] PE_CCM_HW_H13_GetCurCcmSettings() error.\n",__F__,__L__);}
			/* L */
	        PE_MSL_H13B0_RdFL(msc_scr_size_l);
	        PE_MSL_H13B0_RdFL(msc_win_in_offset_l);
	        PE_MSL_H13B0_RdFL(msc_win_in_size_l);
	        PE_MSL_H13B0_RdFL(msc_win_out_size_l);
	        PE_MSL_H13B0_RdFL(msc_c_format_l);
	        PE_MSL_H13B0_Rd01(msc_scr_size_l, 		input_h_size,		ctrl0_info->scl_src_h);
	        PE_MSL_H13B0_Rd01(msc_scr_size_l, 		input_v_size,		ctrl0_info->scl_src_v);
	        PE_MSL_H13B0_Rd01(msc_win_in_offset_l, 	in_win_h_offset,	ctrl0_info->i_win_h_ofst);
	        PE_MSL_H13B0_Rd01(msc_win_in_offset_l, 	in_win_v_offset,	ctrl0_info->i_win_v_ofst);
	        PE_MSL_H13B0_Rd01(msc_win_in_size_l, 	in_win_h_size,		ctrl0_info->i_win_h_size);
	        PE_MSL_H13B0_Rd01(msc_win_in_size_l, 	in_win_v_size,		ctrl0_info->i_win_v_size);
	        PE_MSL_H13B0_Rd01(msc_win_out_size_l, 	out_win_h_size,		ctrl0_info->o_win_h_size);
	        PE_MSL_H13B0_Rd01(msc_win_out_size_l, 	out_win_v_size,		ctrl0_info->o_win_v_size);
	        PE_MSL_H13B0_Rd01(msc_c_format_l, 		in_c_format,		r_data);
			ctrl0_info->is_yuv444 = (r_data==0x6)? 0x1:0x0;	//"100":yuv420,"101":yuv422,"110":yuv444
			PE_OVL_H13B0_RdFL(ovl_ov_moffset);
			PE_OVL_H13B0_RdFL(ovl_ov_msize);
			PE_OVL_H13B0_Rd01(ovl_ov_moffset, 		m_hoffset,			ctrl0_info->m_win_h_ofst);
			PE_OVL_H13B0_Rd01(ovl_ov_moffset, 		m_voffset,			ctrl0_info->m_win_v_ofst);
			PE_OVL_H13B0_Rd01(ovl_ov_msize, 		m_hsize,			ctrl0_info->m_win_h_size);
			PE_OVL_H13B0_Rd01(ovl_ov_msize, 		m_vsize,			ctrl0_info->m_win_v_size);
			PE_P0L_H13B0_RdFL(p0l_pe0_src_size);
			PE_P0L_H13B0_Rd01(p0l_pe0_src_size, 	hsize,				ctrl0_info->pe0_src_h);
			PE_P0L_H13B0_Rd01(p0l_pe0_src_size, 	vsize,				ctrl0_info->pe0_src_v);
			ctrl0_info->scl_resize = (ctrl0_info->o_win_h_size<PE_INF_H13_SCL_RESIZE_H_MAX && \
								ctrl0_info->o_win_v_size<PE_INF_H13_SCL_RESIZE_V_MAX)? 1:0;
			ctrl0_info->scl_hd = (ctrl0_info->scl_src_h>=PE_INF_H13_SCL_IS_HD_MIN)? 1:0;
			/* R */
	        PE_MSR_H13B0_RdFL(msc_scr_size_r);
	        PE_MSR_H13B0_RdFL(msc_win_in_offset_r);
	        PE_MSR_H13B0_RdFL(msc_win_in_size_r);
	        PE_MSR_H13B0_RdFL(msc_win_out_size_r);
	        PE_MSR_H13B0_RdFL(msc_c_format_r);
	        PE_MSR_H13B0_Rd01(msc_scr_size_r, 		input_h_size,		ctrl1_info->scl_src_h);
	        PE_MSR_H13B0_Rd01(msc_scr_size_r,		input_v_size,		ctrl1_info->scl_src_v);
	        PE_MSR_H13B0_Rd01(msc_win_in_offset_r, 	in_win_h_offset,	ctrl1_info->i_win_h_ofst);
	        PE_MSR_H13B0_Rd01(msc_win_in_offset_r, 	in_win_v_offset,	ctrl1_info->i_win_v_ofst);
	        PE_MSR_H13B0_Rd01(msc_win_in_size_r, 	in_win_h_size,		ctrl1_info->i_win_h_size);
	        PE_MSR_H13B0_Rd01(msc_win_in_size_r, 	in_win_v_size,		ctrl1_info->i_win_v_size);
	        PE_MSR_H13B0_Rd01(msc_win_out_size_r, 	out_win_h_size,		ctrl1_info->o_win_h_size);
	        PE_MSR_H13B0_Rd01(msc_win_out_size_r, 	out_win_v_size,		ctrl1_info->o_win_v_size);
	        PE_MSR_H13B0_Rd01(msc_c_format_r,		in_c_format,		r_data);
			ctrl1_info->is_yuv444 = (r_data==0x6)? 0x1:0x0;	//"100":yuv420,"101":yuv422,"110":yuv444
			PE_OVR_H13B0_RdFL(ovr_ov_moffset);
			PE_OVR_H13B0_RdFL(ovr_ov_msize);
			PE_OVR_H13B0_Rd01(ovr_ov_moffset,		m_hoffset,			ctrl1_info->m_win_h_ofst);
			PE_OVR_H13B0_Rd01(ovr_ov_moffset,		m_voffset,			ctrl1_info->m_win_v_ofst);
			PE_OVR_H13B0_Rd01(ovr_ov_msize, 		m_hsize,			ctrl1_info->m_win_h_size);
			PE_OVR_H13B0_Rd01(ovr_ov_msize, 		m_vsize,			ctrl1_info->m_win_v_size);
			PE_P0R_H13B0_RdFL(p0r_pe0_src_size);
			PE_P0R_H13B0_Rd01(p0r_pe0_src_size,		hsize,				ctrl1_info->pe0_src_h);
			PE_P0R_H13B0_Rd01(p0r_pe0_src_size,		vsize,				ctrl1_info->pe0_src_v);
			ctrl1_info->scl_resize = (ctrl1_info->o_win_h_size<PE_INF_H13_SCL_RESIZE_H_MAX && \
								ctrl1_info->o_win_v_size<PE_INF_H13_SCL_RESIZE_V_MAX)? 1:0;
			ctrl1_info->scl_hd = (ctrl1_info->scl_src_h>=PE_INF_H13_SCL_IS_HD_MIN)? 1:0;
			/* sub scaler */
			PE_SSC_H13_RdFL(ssc_scr_size);
			PE_SSC_H13_RdFL(ssc_win_in_offset);
			PE_SSC_H13_RdFL(ssc_win_in_size);
			PE_SSC_H13_RdFL(ssc_win_out_size);
			PE_SSC_H13_Rd01(ssc_scr_size,			input_h_size,		ctrl0_info->ssc_src_h);
			PE_SSC_H13_Rd01(ssc_scr_size,			input_v_size,		ctrl0_info->ssc_src_v);
			PE_SSC_H13_Rd01(ssc_win_in_offset,		in_win_h_offset,	ctrl0_info->ssc_i_win_x);
			PE_SSC_H13_Rd01(ssc_win_in_offset,		in_win_v_offset,	ctrl0_info->ssc_i_win_y);
			PE_SSC_H13_Rd01(ssc_win_in_size,		in_win_h_size,		ctrl0_info->ssc_i_win_h);
			PE_SSC_H13_Rd01(ssc_win_in_size,		in_win_v_size,		ctrl0_info->ssc_i_win_v);
			PE_SSC_H13_Rd01(ssc_win_out_size,		out_win_h_size,		ctrl0_info->ssc_o_win_h);
			PE_SSC_H13_Rd01(ssc_win_out_size,		out_win_v_size,		ctrl0_info->ssc_o_win_v);
			ctrl1_info->ssc_src_h	= ctrl0_info->ssc_src_h;
			ctrl1_info->ssc_src_v	= ctrl0_info->ssc_src_v;
			ctrl1_info->ssc_i_win_x	= ctrl0_info->ssc_i_win_x;
			ctrl1_info->ssc_i_win_y	= ctrl0_info->ssc_i_win_y;
			ctrl1_info->ssc_i_win_h	= ctrl0_info->ssc_i_win_h;
			ctrl1_info->ssc_i_win_v	= ctrl0_info->ssc_i_win_v;
			ctrl1_info->ssc_o_win_h	= ctrl0_info->ssc_o_win_h;
			ctrl1_info->ssc_o_win_v	= ctrl0_info->ssc_o_win_v;
			/* etc */
			ctrl0_info->hv_same = (ctrl0_info->m_win_h_size==ctrl1_info->m_win_h_size && \
				ctrl0_info->m_win_v_size==ctrl1_info->m_win_v_size)? 1:0;
			ctrl1_info->hv_same = (ctrl0_info->m_win_h_size<=PE_INF_H13_SCL_RESIZE_H_MAX && \
				ctrl0_info->m_win_v_size<=PE_INF_H13_SCL_RESIZE_V_MAX)? 1:0;
			ret = PE_HST_HW_H13_GetDetected3dFmt(&r_data);
			if(ret)	{PE_INF_H13_ERROR("[%s,%d] PE_HST_HW_H13_GetDetected3dFmt() error.\n",__F__,__L__);}
			ctrl0_info->det_3df = r_data;
			ctrl0_info->auto_cr_en = ccm_info.auto_cr.enable? 1:0;
			ret = PE_HST_HW_H13_GetSaturationState(LX_PE_WIN_0,&r_data);
			if(ret)	{PE_INF_H13_ERROR("[%s,%d] PE_HST_HW_H13_GetSaturationState() error.\n",__F__,__L__);}
			ctrl0_info->sat_status = r_data;
			ret = PE_HST_HW_H13_GetSaturationState(LX_PE_WIN_1,&r_data);
			if(ret)	{PE_INF_H13_ERROR("[%s,%d] PE_HST_HW_H13_GetSaturationState() error.\n",__F__,__L__);}
			ctrl1_info->sat_status = r_data;
			PE_P1L_H13B0_QRd01(pe1_conv_ctrl_03, pxl_rep_in,	ctrl0_info->mute_on);
			PE_P1L_H13B0_QRd01(pe1_conv_ctrl_08, pxl_rep_in,	ctrl1_info->mute_on);
			PE_P1L_H13B0_QRd01(pe1_wb_ctrl_00,	reg_color_filter_status,	ctrl0_info->pxl_rp_st);
			PE_P1L_H13B0_QRd01(pe1_dse_ctrl_01,	reg_src_freeze,	ctrl0_info->is_src_frz);
		}
		else if(PE_KDRV_VER_H13AX)
		{
			ret = PE_CCM_HW_H13_GetCurCcmSettings(&ccm_info);
			if(ret)	{PE_INF_H13_ERROR("[%s,%d] PE_CCM_HW_H13_GetCurCcmSettings() error.\n",__F__,__L__);}
			/* L */
	        PE_MSL_H13_RdFL(msc_scr_size_l);
	        PE_MSL_H13_RdFL(msc_win_in_offset_l);
	        PE_MSL_H13_RdFL(msc_win_in_size_l);
	        PE_MSL_H13_RdFL(msc_win_out_size_l);
	        PE_MSL_H13_RdFL(msc_c_format_l);
	        PE_MSL_H13_Rd01(msc_scr_size_l, 	input_h_size,		ctrl0_info->scl_src_h);
	        PE_MSL_H13_Rd01(msc_scr_size_l, 	input_v_size,		ctrl0_info->scl_src_v);
	        PE_MSL_H13_Rd01(msc_win_in_offset_l,in_win_h_offset,	ctrl0_info->i_win_h_ofst);
	        PE_MSL_H13_Rd01(msc_win_in_offset_l,in_win_v_offset,	ctrl0_info->i_win_v_ofst);
	        PE_MSL_H13_Rd01(msc_win_in_size_l, 	in_win_h_size,		ctrl0_info->i_win_h_size);
	        PE_MSL_H13_Rd01(msc_win_in_size_l, 	in_win_v_size,		ctrl0_info->i_win_v_size);
	        PE_MSL_H13_Rd01(msc_win_out_size_l, out_win_h_size,		ctrl0_info->o_win_h_size);
	        PE_MSL_H13_Rd01(msc_win_out_size_l, out_win_v_size,		ctrl0_info->o_win_v_size);
	        PE_MSL_H13_Rd01(msc_c_format_l, 	in_c_format,		r_data);
			ctrl0_info->is_yuv444 = (r_data==0x6)? 0x1:0x0;	//"100":yuv420,"101":yuv422,"110":yuv444
			PE_OVL_H13_RdFL(ovl_ov_moffset);
			PE_OVL_H13_RdFL(ovl_ov_msize);
			PE_OVL_H13_Rd01(ovl_ov_moffset, m_hoffset,	ctrl0_info->m_win_h_ofst);
			PE_OVL_H13_Rd01(ovl_ov_moffset, m_voffset,	ctrl0_info->m_win_v_ofst);
			PE_OVL_H13_Rd01(ovl_ov_msize, 	m_hsize,	ctrl0_info->m_win_h_size);
			PE_OVL_H13_Rd01(ovl_ov_msize, 	m_vsize,	ctrl0_info->m_win_v_size);
			PE_P0L_H13_RdFL(p0l_pe0_src_size);
			PE_P0L_H13_Rd01(p0l_pe0_src_size, hsize,	ctrl0_info->pe0_src_h);
			PE_P0L_H13_Rd01(p0l_pe0_src_size, vsize,	ctrl0_info->pe0_src_v);
			ctrl0_info->scl_resize = (ctrl0_info->o_win_h_size<PE_INF_H13_SCL_RESIZE_H_MAX && \
								ctrl0_info->o_win_v_size<PE_INF_H13_SCL_RESIZE_V_MAX)? 1:0;
			ctrl0_info->scl_hd = (ctrl0_info->scl_src_h>=PE_INF_H13_SCL_IS_HD_MIN)? 1:0;
			/* R */
	        PE_MSR_H13_RdFL(msc_scr_size_r);
	        PE_MSR_H13_RdFL(msc_win_in_offset_r);
	        PE_MSR_H13_RdFL(msc_win_in_size_r);
	        PE_MSR_H13_RdFL(msc_win_out_size_r);
	        PE_MSR_H13_RdFL(msc_c_format_r);
	        PE_MSR_H13_Rd01(msc_scr_size_r, 	input_h_size,		ctrl1_info->scl_src_h);
	        PE_MSR_H13_Rd01(msc_scr_size_r, 	input_v_size,		ctrl1_info->scl_src_v);
	        PE_MSR_H13_Rd01(msc_win_in_offset_r,in_win_h_offset,	ctrl1_info->i_win_h_ofst);
	        PE_MSR_H13_Rd01(msc_win_in_offset_r,in_win_v_offset,	ctrl1_info->i_win_v_ofst);
	        PE_MSR_H13_Rd01(msc_win_in_size_r, 	in_win_h_size,		ctrl1_info->i_win_h_size);
	        PE_MSR_H13_Rd01(msc_win_in_size_r, 	in_win_v_size,		ctrl1_info->i_win_v_size);
	        PE_MSR_H13_Rd01(msc_win_out_size_r, out_win_h_size,		ctrl1_info->o_win_h_size);
	        PE_MSR_H13_Rd01(msc_win_out_size_r, out_win_v_size,		ctrl1_info->o_win_v_size);
	        PE_MSR_H13_Rd01(msc_c_format_r, 	in_c_format,		r_data);
			ctrl1_info->is_yuv444 = (r_data==0x6)? 0x1:0x0;	//"100":yuv420,"101":yuv422,"110":yuv444
			PE_OVR_H13_RdFL(ovr_ov_moffset);
			PE_OVR_H13_RdFL(ovr_ov_msize);
			PE_OVR_H13_Rd01(ovr_ov_moffset, m_hoffset,	ctrl1_info->m_win_h_ofst);
			PE_OVR_H13_Rd01(ovr_ov_moffset, m_voffset,	ctrl1_info->m_win_v_ofst);
			PE_OVR_H13_Rd01(ovr_ov_msize, 	m_hsize,	ctrl1_info->m_win_h_size);
			PE_OVR_H13_Rd01(ovr_ov_msize, 	m_vsize,	ctrl1_info->m_win_v_size);
			PE_P0R_H13_RdFL(p0r_pe0_src_size);
			PE_P0R_H13_Rd01(p0r_pe0_src_size, hsize,	ctrl1_info->pe0_src_h);
			PE_P0R_H13_Rd01(p0r_pe0_src_size, vsize,	ctrl1_info->pe0_src_v);
			ctrl1_info->scl_resize = (ctrl1_info->o_win_h_size<PE_INF_H13_SCL_RESIZE_H_MAX && \
								ctrl1_info->o_win_v_size<PE_INF_H13_SCL_RESIZE_V_MAX)? 1:0;
			ctrl1_info->scl_hd = (ctrl1_info->scl_src_h>=PE_INF_H13_SCL_IS_HD_MIN)? 1:0;
			/* sub scaler */
			PE_SSC_H13_RdFL(ssc_scr_size);
			PE_SSC_H13_RdFL(ssc_win_in_offset);
			PE_SSC_H13_RdFL(ssc_win_in_size);
			PE_SSC_H13_RdFL(ssc_win_out_size);
			PE_SSC_H13_Rd01(ssc_scr_size,			input_h_size,		ctrl0_info->ssc_src_h);
			PE_SSC_H13_Rd01(ssc_scr_size,			input_v_size,		ctrl0_info->ssc_src_v);
			PE_SSC_H13_Rd01(ssc_win_in_offset,		in_win_h_offset,	ctrl0_info->ssc_i_win_x);
			PE_SSC_H13_Rd01(ssc_win_in_offset,		in_win_v_offset,	ctrl0_info->ssc_i_win_y);
			PE_SSC_H13_Rd01(ssc_win_in_size,		in_win_h_size,		ctrl0_info->ssc_i_win_h);
			PE_SSC_H13_Rd01(ssc_win_in_size,		in_win_v_size,		ctrl0_info->ssc_i_win_v);
			PE_SSC_H13_Rd01(ssc_win_out_size,		out_win_h_size,		ctrl0_info->ssc_o_win_h);
			PE_SSC_H13_Rd01(ssc_win_out_size,		out_win_v_size,		ctrl0_info->ssc_o_win_v);
			ctrl1_info->ssc_src_h	= ctrl0_info->ssc_src_h;
			ctrl1_info->ssc_src_v	= ctrl0_info->ssc_src_v;
			ctrl1_info->ssc_i_win_x	= ctrl0_info->ssc_i_win_x;
			ctrl1_info->ssc_i_win_y	= ctrl0_info->ssc_i_win_y;
			ctrl1_info->ssc_i_win_h	= ctrl0_info->ssc_i_win_h;
			ctrl1_info->ssc_i_win_v	= ctrl0_info->ssc_i_win_v;
			ctrl1_info->ssc_o_win_h	= ctrl0_info->ssc_o_win_h;
			ctrl1_info->ssc_o_win_v	= ctrl0_info->ssc_o_win_v;
			/* etc */
			ret = PE_HST_HW_H13_GetDetected3dFmt(&r_data);
			if(ret)	{PE_INF_H13_ERROR("[%s,%d] PE_HST_HW_H13_GetDetected3dFmt() error.\n",__F__,__L__);}
			ctrl0_info->det_3df = r_data;
			ctrl0_info->auto_cr_en = ccm_info.auto_cr.enable? 1:0;
			PE_P0L_H13_QRd01(p0l_tpd_stat_00, 	reg_s_status,	ctrl0_info->sat_status);
			PE_P1L_H13_QRd01(pe1_conv_ctrl_03, 	pxl_rep_in,		ctrl0_info->mute_on);
			PE_P1L_H13_QRd01(pe1_conv_ctrl_08, 	pxl_rep_in,		ctrl1_info->mute_on);
		}
		else
		{
			PE_INF_H13_DBG_PRINT("nothing to do\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * get current info.
 *
 * @param   *pstParams [in/out] PE_INF_H13_SETTINGS_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_INF_H13_GetCurInfSettings(PE_INF_H13_SETTINGS_T *pstParams)
{
	int ret = RET_OK;
	static UINT32 count = 0;
	PE_INF_H13_SETTINGS_T *pInfo = &_g_pe_inf_hw_h13_info;
	LX_PE_INF_DISPLAY_T *disp0_info = &_g_pe_inf_hw_h13_info.disp_info[LX_PE_WIN_0];
	LX_PE_INF_DISPLAY_T *disp1_info = &_g_pe_inf_hw_h13_info.disp_info[LX_PE_WIN_1];
	PE_INF_H13_CTRL_T *ctrl0_info = &_g_pe_inf_hw_h13_info.ctrl_info[LX_PE_WIN_0];
	PE_INF_H13_CTRL_T *ctrl1_info = &_g_pe_inf_hw_h13_info.ctrl_info[LX_PE_WIN_1];
	do{
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H13)
		{
			memcpy(pstParams,pInfo,sizeof(PE_INF_H13_SETTINGS_T));
			if(_g_pe_inf_h13_trace)
			{
				if(count>PE_PRINT_COUNT_NUM)
				{
					PE_INF_H13_DBG_PRINT("disp[0] : src:%d,fmt:%d (h:%d,v:%d)\n", \
						disp0_info->src_type, disp0_info->fmt_type, \
						disp0_info->in_h_size, disp0_info->in_v_size);
					PE_INF_H13_DBG_PRINT("disp[0] : fr:%d(%d),scan:%d,cstd,hdmi,scart,dtv,hdd:%d,%d,%d,%d,%d\n", \
						disp0_info->fr_type, disp0_info->in_f_rate, disp0_info->scan_type, \
						disp0_info->cstd_type, disp0_info->hdmi_type, disp0_info->scart_type, \
						disp0_info->dtv_type, disp0_info->hdd_type);
					PE_INF_H13_DBG_PRINT("disp[0] : out:%d,in:%d,user_o:%d,user_i:%d\n", \
						disp0_info->out_type, disp0_info->in_type, disp0_info->user_o_type, \
						disp0_info->user_i_type);
					PE_INF_H13_DBG_PRINT("disp[0] : in_win:%d,%d,%d,%d,out_win:%d,%d,%d,%d,af:%d\n", \
						disp0_info->in_win.x_ofst, disp0_info->in_win.y_ofst, \
						disp0_info->in_win.h_size, disp0_info->in_win.v_size, \
						disp0_info->out_win.x_ofst, disp0_info->out_win.y_ofst, \
						disp0_info->out_win.h_size, disp0_info->out_win.v_size, \
						disp0_info->act_fmt);
					PE_INF_H13_DBG_PRINT("disp[0] : tp:%d,venc:%d,adpt_st:%d,ext_frc:%d,wb_wog:%d,rev:%d\n", \
						disp0_info->mode.is_tp, disp0_info->mode.is_venc, \
						disp0_info->mode.is_adpt_st, disp0_info->mode.is_ext_frc, \
						disp0_info->mode.is_wb_wog, disp0_info->mode.is_reverse);
					PE_INF_H13_DBG_PRINT("ctrl[0] : src:%d,%d,scl:%d,%d,i_w:%d,%d,%d,%d\n",\
						ctrl0_info->pe0_src_h,ctrl0_info->pe0_src_v,\
						ctrl0_info->scl_src_h,ctrl0_info->scl_src_v,\
						ctrl0_info->i_win_h_ofst,ctrl0_info->i_win_v_ofst,\
						ctrl0_info->i_win_h_size,ctrl0_info->i_win_v_size);
					PE_INF_H13_DBG_PRINT("ctrl[0] : scl_o_w:%d,%d,ov_m_w:%d,%d,%d,%d\n",\
						ctrl0_info->o_win_h_size,ctrl0_info->o_win_v_size,\
						ctrl0_info->m_win_h_ofst,ctrl0_info->m_win_v_ofst,\
						ctrl0_info->m_win_h_size,ctrl0_info->m_win_v_size);
					PE_INF_H13_DBG_PRINT("ctrl[0] : y444:%d,det_3df:%d,acr_en:%d,s_stat:%d,mute_on:%d,hv_same:%d\n",\
						ctrl0_info->is_yuv444,ctrl0_info->det_3df,\
						ctrl0_info->auto_cr_en,ctrl0_info->sat_status,\
						ctrl0_info->mute_on,ctrl0_info->hv_same);
					PE_INF_H13_DBG_PRINT("ctrl[0] : sub_scl src:%d,%d, i_w:%d,%d,%d,%d, o_w:%d,%d, pxl_rp:%d, src_frz:%d\n",\
						ctrl0_info->ssc_src_h,ctrl0_info->ssc_src_v,\
						ctrl0_info->ssc_i_win_x,ctrl0_info->ssc_i_win_y,\
						ctrl0_info->ssc_i_win_h,ctrl0_info->ssc_i_win_v,\
						ctrl0_info->ssc_o_win_h,ctrl0_info->ssc_o_win_v,\
						ctrl0_info->pxl_rp_st,ctrl0_info->is_src_frz);
					PE_INF_H13_DBG_PRINT("disp[1] : src:%d,fmt:%d (h:%d,v:%d)\n", \
						disp1_info->src_type, disp1_info->fmt_type, \
						disp1_info->in_h_size, disp1_info->in_v_size);
					PE_INF_H13_DBG_PRINT("disp[1] : fr:%d(%d),scan:%d,cstd,hdmi,scart,dtv,hdd:%d,%d,%d,%d,%d\n", \
						disp1_info->fr_type, disp1_info->in_f_rate, disp1_info->scan_type, \
						disp1_info->cstd_type, disp1_info->hdmi_type, disp1_info->scart_type, \
						disp1_info->dtv_type, disp1_info->hdd_type);
					PE_INF_H13_DBG_PRINT("disp[1] : out:%d,in:%d,user_o:%d,user_i:%d\n", \
						disp1_info->out_type, disp1_info->in_type, disp1_info->user_o_type, \
						disp1_info->user_i_type);
					PE_INF_H13_DBG_PRINT("disp[1] : in_win:%d,%d,%d,%d,out_win:%d,%d,%d,%d,af:%d\n", \
						disp1_info->in_win.x_ofst, disp1_info->in_win.y_ofst, \
						disp1_info->in_win.h_size, disp1_info->in_win.v_size, \
						disp1_info->out_win.x_ofst, disp1_info->out_win.y_ofst, \
						disp1_info->out_win.h_size, disp1_info->out_win.v_size, \
						disp0_info->act_fmt);
					PE_INF_H13_DBG_PRINT("disp[1] : tp:%d,venc:%d,adpt_st:%d,ext_frc:%d,wb_wog:%d,rev:%d\n", \
						disp1_info->mode.is_tp, disp1_info->mode.is_venc, \
						disp1_info->mode.is_adpt_st, disp1_info->mode.is_ext_frc, \
						disp1_info->mode.is_wb_wog, disp1_info->mode.is_reverse);
					PE_INF_H13_DBG_PRINT("ctrl[1] : src:%d,%d,scl:%d,%d,i_w:%d,%d,%d,%d\n",\
						ctrl1_info->pe0_src_h,ctrl1_info->pe0_src_v,\
						ctrl1_info->scl_src_h,ctrl1_info->scl_src_v,\
						ctrl1_info->i_win_h_ofst,ctrl1_info->i_win_v_ofst,\
						ctrl1_info->i_win_h_size,ctrl1_info->i_win_v_size);
					PE_INF_H13_DBG_PRINT("ctrl[1] : scl_o_w:%d,%d,ov_m_w:%d,%d,%d,%d\n",\
						ctrl1_info->o_win_h_size,ctrl1_info->o_win_v_size,\
						ctrl1_info->m_win_h_ofst,ctrl1_info->m_win_v_ofst,\
						ctrl1_info->m_win_h_size,ctrl1_info->m_win_v_size);
					PE_INF_H13_DBG_PRINT("ctrl[1] : y444:%d,det_3df:%d,acr_en:%d,s_stat:%d,mute_on:%d,hv_same:%d\n",\
						ctrl1_info->is_yuv444,ctrl1_info->det_3df,\
						ctrl1_info->auto_cr_en,ctrl1_info->sat_status,\
						ctrl1_info->mute_on,ctrl1_info->hv_same);
					PE_INF_H13_DBG_PRINT("ctrl[1] : sub_scl src:%d,%d, i_w:%d,%d,%d,%d, o_w:%d,%d, pxl_rp:%d, src_frz:%d\n",\
						ctrl1_info->ssc_src_h,ctrl1_info->ssc_src_v,\
						ctrl1_info->ssc_i_win_x,ctrl1_info->ssc_i_win_y,\
						ctrl1_info->ssc_i_win_h,ctrl1_info->ssc_i_win_v,\
						ctrl1_info->ssc_o_win_h,ctrl1_info->ssc_o_win_v,\
						ctrl1_info->pxl_rp_st,ctrl1_info->is_src_frz);
					PE_INF_H13_DBG_PRINT("\n");
					count=0;
				}
				count++;
			}
		}
		else
		{
			PE_INF_H13_DBG_PRINT("nothing to do.\n");
		}
	}while(0);
	return ret;
}
/**
 * get general(all) picture info
 *
 * @param   *pstParams [in] LX_PE_ETC_TBL_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_INF_H13_GetGeneralPicInfo(LX_PE_ETC_TBL_T *pstParams)
{
    int ret = RET_OK;
	char buffer[PE_TRACE_STR_SIZE];
	static UINT32 rd_cnt=0;
	PE_WIN_HW_H13_SETTINGS_T win_set;
	PE_SHP_HW_H13_SETTINGS_T shp_set;
	PE_CCM_HW_H13_SETTINGS_T ccm_set;
	LX_PE_INF_DISPLAY_T *disp0_inf = &(_g_pe_inf_hw_h13_info.disp_info[LX_PE_WIN_0]);
	LX_PE_INF_DISPLAY_T *disp1_inf = &(_g_pe_inf_hw_h13_info.disp_info[LX_PE_WIN_1]);
	PE_INF_H13_CTRL_T *ctrl0_inf = &(_g_pe_inf_hw_h13_info.ctrl_info[LX_PE_WIN_0]);
	PE_INF_H13_CTRL_T *ctrl1_inf = &(_g_pe_inf_hw_h13_info.ctrl_info[LX_PE_WIN_1]);

#define WIN0	LX_PE_WIN_0
#define WIN1	LX_PE_WIN_1
#define PE_INF_H13_PRINT_START		"START_OF_PRINT"
#define PE_INF_H13_PRINT_RESERVED	"RESERVED"
#define PE_INF_H13_PRINT_END		"END_OF_PRINT"
#define PE_INF_H13_CASE_SPRINT(_cnt,_action,_buf,fmt,args...)	\
	case (_cnt):sprintf(_buf,fmt,##args);_action;break
#define PE_INF_H13_DFLT_SPRINT(_action,_buf,fmt,args...)		\
	default:sprintf(_buf,fmt,##args);_action;break

	do{
		CHECK_KNULL(pstParams);
		CHECK_KNULL(pstParams->data);
		if(PE_KDRV_VER_H13)
		{
			/* get win settings */
			ret = PE_WIN_HW_H13_GetCurWinSettings(&win_set);
			PE_INF_H13_CHECK_CODE(ret,break,"[%s,%d] PE_WIN_HW_H13_GetCurWinSettings() error.\n",__F__,__L__);
			/* get shp settings */
			ret = PE_SHP_HW_H13_GetCurShpSettings(&shp_set);
			PE_INF_H13_CHECK_CODE(ret,break,"[%s,%d] PE_SHP_HW_H13_GetCurShpSettings() error.\n",__F__,__L__);
			/* get ccm settings */
			ret = PE_CCM_HW_H13_GetCurCcmSettings(&ccm_set);
			PE_INF_H13_CHECK_CODE(ret,break,"[%s,%d] PE_CCM_HW_H13_GetCurCcmSettings() error.\n",__F__,__L__);
			switch(rd_cnt)
			{
				PE_INF_H13_CASE_SPRINT( 0,rd_cnt++,buffer,"%s",PE_INF_H13_PRINT_START);
				PE_INF_H13_CASE_SPRINT( 1,rd_cnt++,buffer,\
					"- [ kdrv ver. %s ] --------------------------------------------\n", \
					PE_INF_H13_DRV_VER);
				PE_INF_H13_CASE_SPRINT( 2,rd_cnt++,buffer,\
					"src       : 0~6:dtv,atv,cvbs,scart,comp,rgbpc,hdmi\n");
				PE_INF_H13_CASE_SPRINT( 3,rd_cnt++,buffer,\
					"fmt/fr    : 0~2:sd,hd,uhd / 0~1:60hz,50hz\n");
				PE_INF_H13_CASE_SPRINT( 4,rd_cnt++,buffer,\
					"scan/cstd : 0~1:i,p / 0~2:ntsc,pal,secam\n");
				PE_INF_H13_CASE_SPRINT( 5,rd_cnt++,buffer,\
					"hdmi/scart: 0~1:tv,pc / 0~1:av,rgb\n");
				PE_INF_H13_CASE_SPRINT( 6,rd_cnt++,buffer,\
					"dtv       : 0:normal,1:file,2:hdd,3:photo,4:tp,5:camera,6:invalid\n");
				PE_INF_H13_CASE_SPRINT( 7,rd_cnt++,buffer,\
					"hdd       : 0:dtv,1:atv60,2:atv50,3:av60,4:av50,5:scartrgb,6:invalid\n");
				PE_INF_H13_CASE_SPRINT( 8,rd_cnt++,buffer,\
					"out       : 0~3:2d,3_2d,2_3d,ud, 4~6:tb,ss,fs, 7~9:dual_t,s,f, 10:pip\n");
				PE_INF_H13_CASE_SPRINT( 9,rd_cnt++,buffer,\
					"in        : 0tb,ss,q,cb,fp,5fi,fa,fs,la,9ssf,dua,ca,lah\n");
				PE_INF_H13_CASE_SPRINT(10,rd_cnt++,buffer,\
					"---------------------------------------------------------------------------\n");
				PE_INF_H13_CASE_SPRINT(11,rd_cnt++,buffer,\
					"[%02d]disp0: wid:%d,src:%d,fmt:%d (h:%d,v:%d)\n",rd_cnt,\
					disp0_inf->win_id,disp0_inf->src_type,disp0_inf->fmt_type,\
					disp0_inf->in_h_size,disp0_inf->in_v_size);
				PE_INF_H13_CASE_SPRINT(12,rd_cnt++,buffer,\
					"[%02d]disp0: fr:%d(%d),scan:%d,cstd:%d,hdmi:%d,scart:%d,dtv:%d,hdd:%d\n",rd_cnt,\
					disp0_inf->fr_type,disp0_inf->in_f_rate,disp0_inf->scan_type,disp0_inf->cstd_type,\
					disp0_inf->hdmi_type,disp0_inf->scart_type,disp0_inf->dtv_type,disp0_inf->hdd_type);
				PE_INF_H13_CASE_SPRINT(13,rd_cnt++,buffer,\
					"[%02d]disp0: out:%d,in:%d,user_o:%d,user_i:%d\n",rd_cnt,\
					disp0_inf->out_type,disp0_inf->in_type,disp0_inf->user_o_type,disp0_inf->user_i_type);
				PE_INF_H13_CASE_SPRINT(14,rd_cnt++,buffer,\
					"[%02d]disp0: in_win:%d,%d,%d,%d,out_win:%d,%d,%d,%d,af:%d\n",rd_cnt,\
					disp0_inf->in_win.x_ofst,disp0_inf->in_win.y_ofst,disp0_inf->in_win.h_size,\
					disp0_inf->in_win.v_size,disp0_inf->out_win.x_ofst,disp0_inf->out_win.y_ofst,\
					disp0_inf->out_win.h_size,disp0_inf->out_win.v_size,disp0_inf->act_fmt);
				PE_INF_H13_CASE_SPRINT(15,rd_cnt++,buffer,\
					"[%02d]disp0: is tp:%d,venc:%d,adpt_st:%d,ext_frc:%d,wb_wog:%d,rev:%d\n",rd_cnt,\
					disp0_inf->mode.is_tp,disp0_inf->mode.is_venc,disp0_inf->mode.is_adpt_st,\
					disp0_inf->mode.is_ext_frc,disp0_inf->mode.is_wb_wog,disp0_inf->mode.is_reverse);
				PE_INF_H13_CASE_SPRINT(16,rd_cnt++,buffer,\
					"[%02d]disp1: wid:%d,src:%d,fmt:%d (h:%d,v:%d)\n",rd_cnt,\
					disp1_inf->win_id,disp1_inf->src_type,disp1_inf->fmt_type,\
					disp1_inf->in_h_size,disp1_inf->in_v_size);
				PE_INF_H13_CASE_SPRINT(17,rd_cnt++,buffer,\
					"[%02d]disp1: fr:%d(%d),scan:%d,cstd:%d,hdmi:%d,scart:%d,dtv:%d,hdd:%d\n",rd_cnt,\
					disp1_inf->fr_type,disp1_inf->in_f_rate,disp1_inf->scan_type,disp1_inf->cstd_type,\
					disp1_inf->hdmi_type,disp1_inf->scart_type,disp1_inf->dtv_type,disp1_inf->hdd_type);
				PE_INF_H13_CASE_SPRINT(18,rd_cnt++,buffer,\
					"[%02d]disp1: out:%d,in:%d,user_o:%d,user_i:%d\n",rd_cnt,\
					disp1_inf->out_type,disp1_inf->in_type,disp1_inf->user_o_type,disp1_inf->user_i_type);
				PE_INF_H13_CASE_SPRINT(19,rd_cnt++,buffer,\
					"[%02d]disp1: in_win:%d,%d,%d,%d,out_win:%d,%d,%d,%d,af:%d\n",rd_cnt,\
					disp1_inf->in_win.x_ofst,disp1_inf->in_win.y_ofst,disp1_inf->in_win.h_size,\
					disp1_inf->in_win.v_size,disp1_inf->out_win.x_ofst,disp1_inf->out_win.y_ofst,\
					disp1_inf->out_win.h_size,disp1_inf->out_win.v_size,disp1_inf->act_fmt);
				PE_INF_H13_CASE_SPRINT(20,rd_cnt++,buffer,\
					"[%02d]disp1: is tp:%d,venc:%d,adpt_st:%d,ext_frc:%d,wb_wog:%d,rev:%d\n",rd_cnt,\
					disp1_inf->mode.is_tp,disp1_inf->mode.is_venc,disp1_inf->mode.is_adpt_st,\
					disp1_inf->mode.is_ext_frc,disp1_inf->mode.is_wb_wog,disp1_inf->mode.is_reverse);
				PE_INF_H13_CASE_SPRINT(21,rd_cnt++,buffer,\
					"---------------------------------------------------------------------------\n");
				PE_INF_H13_CASE_SPRINT(22,rd_cnt++,buffer,\
					"det_3df: 0:2d,1:cb,2,cc,3:ll,4:ss,5:tb,6:unknown,7:max\n");
				PE_INF_H13_CASE_SPRINT(23,rd_cnt++,buffer,\
					"---------------------------------------------------------------------------\n");
				PE_INF_H13_CASE_SPRINT(24,rd_cnt++,buffer,"[%02d]ctrl0:\n",rd_cnt);
				PE_INF_H13_CASE_SPRINT(25,rd_cnt++,buffer,\
					"[%02d]pe0_src_h,v:%d,%d,scl_src_h,v:%d,%d,resize:%d,scl_hd:%d\n",rd_cnt,\
					ctrl0_inf->pe0_src_h,ctrl0_inf->pe0_src_v,ctrl0_inf->scl_src_h,\
					ctrl0_inf->scl_src_v,ctrl0_inf->scl_resize,ctrl0_inf->scl_hd);
				PE_INF_H13_CASE_SPRINT(26,rd_cnt++,buffer,\
					"[%02d]i_win:%d,%d,%d,%d,o_win:%d,%d,m_win:%d,%d,%d,%d\n",rd_cnt,\
					ctrl0_inf->i_win_h_ofst,ctrl0_inf->i_win_v_ofst,ctrl0_inf->i_win_h_size,\
					ctrl0_inf->i_win_v_size,ctrl0_inf->o_win_h_size,ctrl0_inf->o_win_v_size,\
					ctrl0_inf->m_win_h_ofst,ctrl0_inf->m_win_v_ofst,ctrl0_inf->m_win_h_size,\
					ctrl0_inf->m_win_v_size);
				PE_INF_H13_CASE_SPRINT(27,rd_cnt++,buffer,\
					"[%02d]is_yuv444:%d,det_3df:%d,auto_en:%d,s_state:%d,mute_on:%d,hv_same:%d\n",rd_cnt,\
					ctrl0_inf->is_yuv444,ctrl0_inf->det_3df,ctrl0_inf->auto_cr_en,\
					ctrl0_inf->sat_status,ctrl0_inf->mute_on,ctrl0_inf->hv_same);
				PE_INF_H13_CASE_SPRINT(28,rd_cnt++,buffer,\
					"[%02d]ssc_src,i,o_win:%d,%d, %d,%d,%d,%d, %d,%d, pxl_rp:%d,src_frz:%d\n",rd_cnt,\
					ctrl0_inf->ssc_src_h,ctrl0_inf->ssc_src_v,ctrl0_inf->ssc_i_win_x,ctrl0_inf->ssc_i_win_y,\
					ctrl0_inf->ssc_i_win_h,ctrl0_inf->ssc_i_win_v,ctrl0_inf->ssc_o_win_h,\
					ctrl0_inf->ssc_o_win_v,ctrl0_inf->pxl_rp_st,ctrl0_inf->is_src_frz);
				PE_INF_H13_CASE_SPRINT(29,rd_cnt++,buffer,"%s",PE_INF_H13_PRINT_RESERVED);
				PE_INF_H13_CASE_SPRINT(30,rd_cnt++,buffer,"%s",PE_INF_H13_PRINT_RESERVED);
				PE_INF_H13_CASE_SPRINT(31,rd_cnt++,buffer,"[%02d]ctrl1:\n",rd_cnt);
				PE_INF_H13_CASE_SPRINT(32,rd_cnt++,buffer,\
					"[%02d]pe0_src_h,v:%d,%d,scl_src_h,v:%d,%d,resize:%d,scl_hd:%d\n",rd_cnt,\
					ctrl1_inf->pe0_src_h,ctrl1_inf->pe0_src_v,ctrl1_inf->scl_src_h,\
					ctrl1_inf->scl_src_v,ctrl1_inf->scl_resize,ctrl1_inf->scl_hd);
				PE_INF_H13_CASE_SPRINT(33,rd_cnt++,buffer,\
					"[%02d]i_win:%d,%d,%d,%d,o_win:%d,%d,m_win:%d,%d,%d,%d\n",rd_cnt,\
					ctrl1_inf->i_win_h_ofst,ctrl1_inf->i_win_v_ofst,ctrl1_inf->i_win_h_size,\
					ctrl1_inf->i_win_v_size,ctrl1_inf->o_win_h_size,ctrl1_inf->o_win_v_size,\
					ctrl1_inf->m_win_h_ofst,ctrl1_inf->m_win_v_ofst,ctrl1_inf->m_win_h_size,\
					ctrl1_inf->m_win_v_size);
				PE_INF_H13_CASE_SPRINT(34,rd_cnt++,buffer,\
					"[%02d]is_yuv444:%d,det_3df:%d,auto_en:%d,s_state:%d,mute_on:%d,hv_same:%d\n",rd_cnt,\
					ctrl1_inf->is_yuv444,ctrl1_inf->det_3df,ctrl1_inf->auto_cr_en,\
					ctrl1_inf->sat_status,ctrl1_inf->mute_on,ctrl1_inf->hv_same);
				PE_INF_H13_CASE_SPRINT(35,rd_cnt++,buffer,\
					"[%02d]ssc_src,i,o_win:%d,%d, %d,%d,%d,%d, %d,%d, pxl_rp:%d,src_frz:%d\n",rd_cnt,\
					ctrl1_inf->ssc_src_h,ctrl1_inf->ssc_src_v,ctrl1_inf->ssc_i_win_x,ctrl1_inf->ssc_i_win_y,\
					ctrl1_inf->ssc_i_win_h,ctrl1_inf->ssc_i_win_v,ctrl1_inf->ssc_o_win_h,\
					ctrl1_inf->ssc_o_win_v,ctrl1_inf->pxl_rp_st,ctrl1_inf->is_src_frz);
				PE_INF_H13_CASE_SPRINT(36,rd_cnt++,buffer,"%s",PE_INF_H13_PRINT_RESERVED);
				PE_INF_H13_CASE_SPRINT(37,rd_cnt++,buffer,"%s",PE_INF_H13_PRINT_RESERVED);
				PE_INF_H13_CASE_SPRINT(38,rd_cnt++,buffer,"%s",PE_INF_H13_PRINT_RESERVED);
				PE_INF_H13_CASE_SPRINT(39,rd_cnt++,buffer,"%s",PE_INF_H13_PRINT_RESERVED);
				PE_INF_H13_CASE_SPRINT(40,rd_cnt++,buffer,"%s",PE_INF_H13_PRINT_RESERVED);
				PE_INF_H13_CASE_SPRINT(41,rd_cnt++,buffer,"[%02d]win:\n",rd_cnt);
				PE_INF_H13_CASE_SPRINT(42,rd_cnt++,buffer,\
					"[%02d]act0:%d,%d,%d,%d, act1:%d,%d,%d,%d\n",rd_cnt,\
					win_set.win_inf.act0_x0,win_set.win_inf.act0_y0,win_set.win_inf.act0_x1,\
					win_set.win_inf.act0_y1,win_set.win_inf.act1_x0,win_set.win_inf.act1_y0,\
					win_set.win_inf.act1_x1,win_set.win_inf.act1_y1);
				PE_INF_H13_CASE_SPRINT(43,rd_cnt++,buffer,\
					"[%02d]win0:%d,%d,%d,%d, win1:%d,%d,%d,%d\n",rd_cnt,\
					win_set.win_inf.win0_x0,win_set.win_inf.win0_y0,win_set.win_inf.win0_x1,\
					win_set.win_inf.win0_y1,win_set.win_inf.win1_x0,win_set.win_inf.win1_y0,\
					win_set.win_inf.win1_x1,win_set.win_inf.win1_y1);
				PE_INF_H13_CASE_SPRINT(44,rd_cnt++,buffer,\
					"[%02d]h,v_max:%d,%d, w0,w1,w01_en:[%d,%d],[%d,%d],[%d,%d]\n",rd_cnt,\
					win_set.win_inf.h_max,win_set.win_inf.v_max,win_set.win_inf.win0_en[0],\
					win_set.win_inf.win0_en[1],win_set.win_inf.win0_en[0],win_set.win_inf.win0_en[1],\
					win_set.win_inf.win01_en[0],win_set.win_inf.win01_en[1]);
				PE_INF_H13_CASE_SPRINT(45,rd_cnt++,buffer,\
					"[%02d]lrcr histo_l,r, apply_r : %d,%d,%d,%d, %d,%d,%d,%d, %d,%d,%d,%d\n",rd_cnt,\
					win_set.lrcr_inf.histo_l_x0,win_set.lrcr_inf.histo_l_y0,win_set.lrcr_inf.histo_l_x1,\
					win_set.lrcr_inf.histo_l_y1,win_set.lrcr_inf.histo_r_x0,win_set.lrcr_inf.histo_r_y0,\
					win_set.lrcr_inf.histo_r_x1,win_set.lrcr_inf.histo_r_y1,win_set.lrcr_inf.apply_r_x0,\
					win_set.lrcr_inf.apply_r_y0,win_set.lrcr_inf.apply_r_x1,win_set.lrcr_inf.apply_r_y1);
				PE_INF_H13_CASE_SPRINT(46,rd_cnt++,buffer,\
					"[%02d]win: tnr win h,v size0:%d,%d, size1:%d,%d\n",rd_cnt,\
					win_set.tnr_inf.h_size0,win_set.tnr_inf.v_size0,\
					win_set.tnr_inf.h_size1,win_set.tnr_inf.v_size1);
				PE_INF_H13_CASE_SPRINT(47,rd_cnt++,buffer,"%s",PE_INF_H13_PRINT_RESERVED);
				PE_INF_H13_CASE_SPRINT(48,rd_cnt++,buffer,"%s",PE_INF_H13_PRINT_RESERVED);
				PE_INF_H13_CASE_SPRINT(49,rd_cnt++,buffer,"%s",PE_INF_H13_PRINT_RESERVED);
				PE_INF_H13_CASE_SPRINT(50,rd_cnt++,buffer,"%s",PE_INF_H13_PRINT_RESERVED);
				PE_INF_H13_CASE_SPRINT(51,rd_cnt++,buffer,"[%02d]shp:\n",rd_cnt);
				PE_INF_H13_CASE_SPRINT(52,rd_cnt++,buffer,\
					"[%02d]cur_scl_idx[0] : wid:%d, luma h,v:%d,%d, chrm h,v:%d,%d\n",rd_cnt,\
					shp_set.cur_scl_idx[WIN0].win_id,shp_set.cur_scl_idx[WIN0].h_luma_id,\
					shp_set.cur_scl_idx[WIN0].v_luma_id,shp_set.cur_scl_idx[WIN0].h_chrm_id,\
					shp_set.cur_scl_idx[WIN0].v_chrm_id);
				PE_INF_H13_CASE_SPRINT(53,rd_cnt++,buffer,\
					"[%02d]cur_scl_idx[1] : wid:%d, luma h,v:%d,%d, chrm h,v:%d,%d\n",rd_cnt,\
					shp_set.cur_scl_idx[WIN1].win_id,shp_set.cur_scl_idx[WIN1].h_luma_id,\
					shp_set.cur_scl_idx[WIN1].v_luma_id,shp_set.cur_scl_idx[WIN1].h_chrm_id,\
					shp_set.cur_scl_idx[WIN1].v_chrm_id);
				PE_INF_H13_CASE_SPRINT(54,rd_cnt++,buffer,\
					"[%02d]pre_scl_idx[0] : wid:%d, luma h,v:%d,%d, chrm h,v:%d,%d\n",rd_cnt,\
					shp_set.pre_scl_idx[WIN0].win_id,shp_set.pre_scl_idx[WIN0].h_luma_id,\
					shp_set.pre_scl_idx[WIN0].v_luma_id,shp_set.pre_scl_idx[WIN0].h_chrm_id,\
					shp_set.pre_scl_idx[WIN0].v_chrm_id);
				PE_INF_H13_CASE_SPRINT(55,rd_cnt++,buffer,\
					"[%02d]pre_scl_idx[1] : wid:%d, luma h,v:%d,%d, chrm h,v:%d,%d\n",rd_cnt,\
					shp_set.pre_scl_idx[WIN1].win_id,shp_set.pre_scl_idx[WIN1].h_luma_id,\
					shp_set.pre_scl_idx[WIN1].v_luma_id,shp_set.pre_scl_idx[WIN1].h_chrm_id,\
					shp_set.pre_scl_idx[WIN1].v_chrm_id);
				PE_INF_H13_CASE_SPRINT(56,rd_cnt++,buffer,\
					"[%02d]h_y_tap:%d,%d, h_c_tap:%d,%d, v_y_tap:%d,%d, v_c_tap:%d,%d\n",rd_cnt,\
					shp_set.h_y_tap[WIN0],shp_set.h_y_tap[WIN1],shp_set.h_c_tap[WIN0],\
					shp_set.h_c_tap[WIN1],shp_set.v_y_tap[WIN0],shp_set.v_y_tap[WIN1],\
					shp_set.v_c_tap[WIN0],shp_set.v_c_tap[WIN1]);
				PE_INF_H13_CASE_SPRINT(57,rd_cnt++,buffer,\
					"[%02d]y_fir:%d,%d, c_fir:%d,%d, pre_res_on:%d,%d\n",rd_cnt,\
					shp_set.y_fir[WIN0],shp_set.y_fir[WIN1],shp_set.c_fir[WIN0],\
					shp_set.c_fir[WIN1],shp_set.pre_res_en[WIN0],shp_set.pre_res_en[WIN1]);
				PE_INF_H13_CASE_SPRINT(58,rd_cnt++,buffer,"%s",PE_INF_H13_PRINT_RESERVED);
				PE_INF_H13_CASE_SPRINT(59,rd_cnt++,buffer,"%s",PE_INF_H13_PRINT_RESERVED);
				PE_INF_H13_CASE_SPRINT(60,rd_cnt++,buffer,"%s",PE_INF_H13_PRINT_RESERVED);
				PE_INF_H13_CASE_SPRINT(61,rd_cnt++,buffer,"[%02d]ccm:\n",rd_cnt);
				PE_INF_H13_CASE_SPRINT(62,rd_cnt++,buffer,\
					"[%02d]lrcr(a0) sel : r,l_h:%d,%d, r,l_p:%d,%d, r,l_o:%d,%d\n",rd_cnt,\
					ccm_set.ccm_inf.r_h_select,ccm_set.ccm_inf.l_h_select,ccm_set.ccm_inf.r_p_select,\
					ccm_set.ccm_inf.l_p_select,ccm_set.ccm_inf.r_o_select,ccm_set.ccm_inf.l_o_select);
				PE_INF_H13_CASE_SPRINT(63,rd_cnt++,buffer,\
					"[%02d]lrcr(a0) r,l_bypass:%d,%d\n",rd_cnt,\
					ccm_set.ccm_inf.ppr_bypass,ccm_set.ccm_inf.ppl_bypass);
				PE_INF_H13_CASE_SPRINT(64,rd_cnt++,buffer,\
					"[%02d]lrcr ff_md:%d, ll_md:%d, h_md:%d, gain:%d, lut:%d\n",rd_cnt,\
					ccm_set.ccm_inf.ff_mode_on,ccm_set.ccm_inf.ll_mode_en,ccm_set.ccm_inf.histo_mode,\
					ccm_set.ccm_inf.lrcr_gain,ccm_set.ccm_inf.lut_type);
				PE_INF_H13_CASE_SPRINT(65,rd_cnt++,buffer,\
					"[%02d]acr en:%d, max_hist:%d,val_bins:%d, th0,1:%d,%d\n",rd_cnt,\
					ccm_set.auto_cr.enable,ccm_set.auto_cr.th_max_hist,ccm_set.auto_cr.th_valid_bins,\
					ccm_set.auto_cr.adj_th0,ccm_set.auto_cr.adj_th1);
				PE_INF_H13_CASE_SPRINT(66,rd_cnt++,buffer,"%s",PE_INF_H13_PRINT_RESERVED);
				PE_INF_H13_CASE_SPRINT(67,rd_cnt++,buffer,"%s",PE_INF_H13_PRINT_RESERVED);
				PE_INF_H13_CASE_SPRINT(68,rd_cnt++,buffer,"%s",PE_INF_H13_PRINT_RESERVED);
				PE_INF_H13_CASE_SPRINT(69,rd_cnt++,buffer,"%s",PE_INF_H13_PRINT_RESERVED);
				PE_INF_H13_CASE_SPRINT(70,rd_cnt++,buffer,"%s",PE_INF_H13_PRINT_RESERVED);
				PE_INF_H13_DFLT_SPRINT(rd_cnt=0,buffer,"%s",PE_INF_H13_PRINT_END);
			}
			memcpy(pstParams->data,buffer,sizeof(char)*PE_TRACE_STR_SIZE);
		}
		else
		{
			PE_INF_H13_DBG_PRINT("nothing to do.\n");
		}
	}while(0);
	return ret;
}
/**
 * set PathMonitor operation
 *
 * @param   *pstParams [in] LX_PE_ETC_TBL_T
 * @return  OK if success, ERROR otherwise.
 * @see     
 * @author
 */
static int PE_INF_H13_SetPathMonitor(LX_PE_ETC_TBL_T *pstParams)
{
    int ret = RET_OK;
	UINT32 w_data[3], sel = 0;
	UINT32 yuv08_data[6][3] = {\
		{0x00, 0x08, 0x08}, {0x80, 0x08, 0x08}, {0xff, 0x08, 0x08}, \
		{0x00, 0x08, 0x0f}, {0x80, 0x00, 0x00}, {0x00, 0x0f, 0x08}\
		};//0:black, 1:gray, 2:white, 3:R, 4:G, 5:B
	UINT32 rgb10_data[6][3] = {\
		{0x000, 0x000, 0x000}, {0x200, 0x200, 0x200}, {0x3ff, 0x3ff, 0x3ff}, \
		{0x3ff, 0x000, 0x000}, {0x000, 0x3ff, 0x000}, {0x000, 0x000, 0x3ff}\
		};//0:black, 1:gray, 2:white, 3:R, 4:G, 5:B
	do{
		CHECK_KNULL(pstParams);
		CHECK_KNULL(pstParams->data);
		if(PE_KDRV_VER_H13BX)
		{
			w_data[0] = pstParams->data[1];
			switch (pstParams->data[0])
			{
				case PE_INF_HW_H13_M_CVA_PATTERN_TYPE:
					PE_CVA_H13B0_QWr01(cva_misc_ctrl,	pattern_type,		w_data[0]);
					break;
				case PE_INF_HW_H13_M_CVA_PATTERN_CSC:
					PE_CVA_H13B0_QWr01(cva_misc_ctrl,	pattern_csc,		w_data[0]);
					break;
				case PE_INF_HW_H13_M_CVA_PATTERN_DETAIL:
					PE_CVA_H13B0_QWr01(cva_misc_ctrl,	pattern_detail,		w_data[0]);
					break;
				case PE_INF_HW_H13_M_CVA_FIR_EN:
					PE_CVA_H13B0_QWr01(cva_dig_filt_ctrl3,	fir_en,			w_data[0]);
					break;
				case PE_INF_HW_H13_M_CVA_FIR_Y_EN:
					PE_CVA_H13B0_QWr01(cva_dig_filt_ctrl3,	fir_y_en,		w_data[0]);
					break;
				case PE_INF_HW_H13_M_CVA_FIR_C_EN:
					PE_CVA_H13B0_QWr01(cva_dig_filt_ctrl3,	fir_c_en,		w_data[0]);
					break;
				case PE_INF_HW_H13_M_CVB_PATTERN_TYPE:
					PE_CVB_H13B0_QWr01(cvb_misc_ctrl,	pattern_type,		w_data[0]);
					break;
				case PE_INF_HW_H13_M_CVB_PATTERN_CSC:
					PE_CVB_H13B0_QWr01(cvb_misc_ctrl,	pattern_csc,		w_data[0]);
					break;
				case PE_INF_HW_H13_M_CVB_PATTERN_DETAIL:
					PE_CVB_H13B0_QWr01(cvb_misc_ctrl,	pattern_detail,		w_data[0]);
					break;
				case PE_INF_HW_H13_M_CVB_FIR_EN:
					PE_CVB_H13B0_QWr01(cvb_dig_filt_ctrl3,	fir_en,			w_data[0]);
					break;
				case PE_INF_HW_H13_M_CVB_FIR_Y_EN:
					PE_CVB_H13B0_QWr01(cvb_dig_filt_ctrl3,	fir_y_en,		w_data[0]);
					break;
				case PE_INF_HW_H13_M_CVB_FIR_C_EN:
					PE_CVB_H13B0_QWr01(cvb_dig_filt_ctrl3,	fir_c_en,		w_data[0]);
					break;
				case PE_INF_HW_H13_M_P0L_DETOUR:
					PE_P0L_H13B0_QWr01(p0l_pe0_opmode,		detour_enable,	w_data[0]);
					break;
				case PE_INF_HW_H13_M_P0L_IPC_Y:
					PE_P0L_H13B0_QWr01(p0l_ipc_ctrl_00,		cr_luma_mode,	w_data[0]);
					break;
				case PE_INF_HW_H13_M_P0L_IPC_C:
					PE_P0L_H13B0_QWr01(p0l_ipc_ctrl_00,		cr_chroma_mode,	w_data[0]);
					break;
				case PE_INF_HW_H13_M_P0R_DETOUR:
					PE_P0R_H13B0_QWr01(p0r_pe0_opmode,		detour_enable,	w_data[0]);
					break;
				case PE_INF_HW_H13_M_P0R_IPC_Y:
					PE_P0R_H13B0_QWr01(p0r_ipc_ctrl_00,		cr_luma_mode,	w_data[0]);
					break;
				case PE_INF_HW_H13_M_P0R_IPC_C:
					PE_P0R_H13B0_QWr01(p0r_ipc_ctrl_00,		cr_chroma_mode,	w_data[0]);
					break;
				case PE_INF_HW_H13_M_MSL_FLT_SEL_Y:
					PE_MSL_H13B0_QWr01(msc_ctrl_l,	filter_type_sel_luma,	w_data[0]);
					break;
				case PE_INF_HW_H13_M_MSR_FLT_SEL_Y:
					PE_MSR_H13B0_QWr01(msc_ctrl_r,	filter_type_sel_luma,	w_data[0]);
					break;
				case PE_INF_HW_H13_M_OVL_DARK_W0_EN:
					PE_OVL_H13B0_QWr01(ovl_ov_ctrl4,		w0_dark_en,		w_data[0]);
					break;
				case PE_INF_HW_H13_M_OVL_DARK_W1_EN:
					PE_OVL_H13B0_QWr01(ovl_ov_ctrl4,		w1_dark_en,		w_data[0]);
					break;
				case PE_INF_HW_H13_M_OVL_DARK_COLOR:
					sel = (w_data[0]<6)? w_data[0]:5;
					w_data[0] = yuv08_data[sel][0];	// y
					w_data[1] = yuv08_data[sel][1];	// cb
					w_data[2] = yuv08_data[sel][2];	// cr
					PE_OVL_H13B0_QWr01(ovl_ov_ctrl2,		dark_color_yy,	w_data[0]);
					PE_OVL_H13B0_QWr01(ovl_ov_ctrl2,		dark_color_cb,	w_data[1]);
					PE_OVL_H13B0_QWr01(ovl_ov_ctrl2,		dark_color_cr,	w_data[2]);
					break;
				case PE_INF_HW_H13_M_OVL_BACK_COLOR:
					sel = (w_data[0]<6)? w_data[0]:5;
					w_data[0] = yuv08_data[sel][0];	// y
					w_data[1] = yuv08_data[sel][1];	// cb
					w_data[2] = yuv08_data[sel][2];	// cr
					PE_OVL_H13B0_QWr01(ovl_ov_ctrl2,		back_color_yy,	w_data[0]);
					PE_OVL_H13B0_QWr01(ovl_ov_ctrl2,		back_color_cb,	w_data[1]);
					PE_OVL_H13B0_QWr01(ovl_ov_ctrl2,		back_color_cr,	w_data[2]);
					break;
				case PE_INF_HW_H13_M_OVR_DARK_W0_EN:
					PE_OVR_H13B0_QWr01(ovr_ov_ctrl4,		w0_dark_en,		w_data[0]);
					break;
				case PE_INF_HW_H13_M_OVR_DARK_W1_EN:
					PE_OVR_H13B0_QWr01(ovr_ov_ctrl4,		w1_dark_en,		w_data[0]);
					break;
				case PE_INF_HW_H13_M_OVR_DARK_COLOR:
					sel = (w_data[0]<6)? w_data[0]:5;
					w_data[0] = yuv08_data[sel][0];	// y
					w_data[1] = yuv08_data[sel][1];	// cb
					w_data[2] = yuv08_data[sel][2];	// cr
					PE_OVR_H13B0_QWr01(ovr_ov_ctrl2,		dark_color_yy,	w_data[0]);
					PE_OVR_H13B0_QWr01(ovr_ov_ctrl2,		dark_color_cb,	w_data[1]);
					PE_OVR_H13B0_QWr01(ovr_ov_ctrl2,		dark_color_cr,	w_data[2]);
					break;
				case PE_INF_HW_H13_M_OVR_BACK_COLOR:
					sel = (w_data[0]<6)? w_data[0]:5;
					w_data[0] = yuv08_data[sel][0];	// y
					w_data[1] = yuv08_data[sel][1];	// cb
					w_data[2] = yuv08_data[sel][2];	// cr
					PE_OVR_H13B0_QWr01(ovr_ov_ctrl2,		back_color_yy,	w_data[0]);
					PE_OVR_H13B0_QWr01(ovr_ov_ctrl2,		back_color_cb,	w_data[1]);
					PE_OVR_H13B0_QWr01(ovr_ov_ctrl2,		back_color_cr,	w_data[2]);
					break;
				case PE_INF_HW_H13_M_PE1_DETOUR:
					PE_P1L_H13B0_QWr01(pe1_top_ctrl_02,		pe1_detour,		w_data[0]);
					break;
				case PE_INF_HW_H13_M_PE1_REP0_IN_EN:
					PE_P1L_H13B0_QWr01(pe1_conv_ctrl_03,	pxl_rep_in,		w_data[0]);
					break;
				case PE_INF_HW_H13_M_PE1_REP0_COLOR:
					sel = (w_data[0]<6)? w_data[0]:5;
					w_data[0] = rgb10_data[sel][0];	// r
					w_data[1] = rgb10_data[sel][1];	// g
					w_data[2] = rgb10_data[sel][2];	// b
					PE_P1L_H13B0_QWr01(pe1_conv_ctrl_05,	pxl_rep_value_r,	w_data[0]);
					PE_P1L_H13B0_QWr01(pe1_conv_ctrl_05,	pxl_rep_value_g,	w_data[1]);
					PE_P1L_H13B0_QWr01(pe1_conv_ctrl_05,	pxl_rep_value_b,	w_data[2]);
					break;
				case PE_INF_HW_H13_M_PE1_REP1_IN_EN:
					PE_P1L_H13B0_QWr01(pe1_conv_ctrl_08,		pxl_rep_in,		w_data[0]);
					break;
				case PE_INF_HW_H13_M_PE1_REP1_COLOR:
					sel = (w_data[0]<6)? w_data[0]:5;
					w_data[0] = rgb10_data[sel][0];	// r
					w_data[1] = rgb10_data[sel][1];	// g
					w_data[2] = rgb10_data[sel][2];	// b
					PE_P1L_H13B0_QWr01(pe1_conv_ctrl_0a,	pxl_rep_value_r,	w_data[0]);
					PE_P1L_H13B0_QWr01(pe1_conv_ctrl_0a,	pxl_rep_value_g,	w_data[1]);
					PE_P1L_H13B0_QWr01(pe1_conv_ctrl_0a,	pxl_rep_value_b,	w_data[2]);
					break;
				case PE_INF_HW_H13_M_DISPLAY:
				default:
					break;
			}
		}
		else
		{
			PE_INF_H13_DBG_PRINT("nothing to do.\n");
		}
	}while(0);
	return ret;
}
/**
 * get PathMonitor operation
 *
 * @param   *pstParams [in/out] LX_PE_ETC_TBL_T
 * @return  OK if success, ERROR otherwise.
 * @see     
 * @author
 */
static int PE_INF_H13_GetPathMonitor(LX_PE_ETC_TBL_T *pstParams)
{
	int ret = RET_OK;
	UINT32 r_data[3];
	char buffer[PE_TRACE_STR_SIZE];
	static UINT32 rd_cnt=0;

#define PE_INF_H13_PRINT_START		"START_OF_PRINT"
#define PE_INF_H13_PRINT_RESERVED	"RESERVED"
#define PE_INF_H13_PRINT_END		"END_OF_PRINT"

	do{
		CHECK_KNULL(pstParams);
		CHECK_KNULL(pstParams->data);
		if(PE_KDRV_VER_H13BX)
		{
			switch (rd_cnt)
			{
				case PE_INF_HW_H13_M_DISPLAY:
					sprintf(buffer, "%s", PE_INF_H13_PRINT_START);
					break;
				case PE_INF_HW_H13_M_CVA_PATTERN_TYPE:
					PE_CVA_H13B0_QRd01(cva_misc_ctrl,	pattern_type,		r_data[0]);
					sprintf(buffer, "   [0x%02x][CVA_pattern_en](0:off,1:on)  : %d\n", \
						PE_INF_HW_H13_M_CVA_PATTERN_TYPE, r_data[0]);
					break;
				case PE_INF_HW_H13_M_CVA_PATTERN_CSC:
					PE_CVA_H13B0_QRd01(cva_misc_ctrl,	pattern_csc,		r_data[0]);
					sprintf(buffer, "   [0x%02x][CVA_pattern_csc](0:yc,1:rgb) : %d\n", \
						PE_INF_HW_H13_M_CVA_PATTERN_CSC, r_data[0]);
					break;
				case PE_INF_HW_H13_M_CVA_PATTERN_DETAIL:
					PE_CVA_H13B0_QRd01(cva_misc_ctrl,	pattern_detail,		r_data[0]);
					sprintf(buffer, "   [0x%02x][CVA_pattern_num](0~12)       : %d\n", \
						PE_INF_HW_H13_M_CVA_PATTERN_DETAIL, r_data[0]);
					break;
				case PE_INF_HW_H13_M_CVA_FIR_EN:
					PE_CVA_H13B0_QRd01(cva_dig_filt_ctrl3,	fir_en,			r_data[0]);
					sprintf(buffer, "   [0x%02x][CVA_fir_en](0:off,1:on)      : %d\n", \
						PE_INF_HW_H13_M_CVA_FIR_EN, r_data[0]);
					break;
				case PE_INF_HW_H13_M_CVA_FIR_Y_EN:
					PE_CVA_H13B0_QRd01(cva_dig_filt_ctrl3,	fir_y_en,		r_data[0]);
					sprintf(buffer, "   [0x%02x][CVA_fir_y_en](0:off,1:on)    : %d\n", \
						PE_INF_HW_H13_M_CVA_FIR_Y_EN, r_data[0]);
					break;
				case PE_INF_HW_H13_M_CVA_FIR_C_EN:
					PE_CVA_H13B0_QRd01(cva_dig_filt_ctrl3,	fir_c_en,		r_data[0]);
					sprintf(buffer, "   [0x%02x][CVA_fir_c_en](0:off,1:on)    : %d\n", \
						PE_INF_HW_H13_M_CVA_FIR_C_EN, r_data[0]);
					break;
				case PE_INF_HW_H13_M_CVB_PATTERN_TYPE:
					PE_CVB_H13B0_QRd01(cvb_misc_ctrl,	pattern_type,		r_data[0]);
					sprintf(buffer, "   [0x%02x][CVB_pattern_en](0:off,1:on)  : %d\n", \
						PE_INF_HW_H13_M_CVB_PATTERN_TYPE, r_data[0]);
					break;
				case PE_INF_HW_H13_M_CVB_PATTERN_CSC:
					PE_CVB_H13B0_QRd01(cvb_misc_ctrl,	pattern_csc,		r_data[0]);
					sprintf(buffer, "   [0x%02x][CVB_pattern_csc](0:yc,1:rgb) : %d\n", \
						PE_INF_HW_H13_M_CVB_PATTERN_CSC, r_data[0]);
					break;
				case PE_INF_HW_H13_M_CVB_PATTERN_DETAIL:
					PE_CVB_H13B0_QRd01(cvb_misc_ctrl,	pattern_detail,		r_data[0]);
					sprintf(buffer, "   [0x%02x][CVB_pattern_num](0~12)       : %d\n", \
						PE_INF_HW_H13_M_CVB_PATTERN_DETAIL, r_data[0]);
					break;
				case PE_INF_HW_H13_M_CVB_FIR_EN:
					PE_CVB_H13B0_QRd01(cvb_dig_filt_ctrl3,	fir_en,			r_data[0]);
					sprintf(buffer, "   [0x%02x][CVB_fir_en](0:off,1:on)      : %d\n", \
						PE_INF_HW_H13_M_CVB_FIR_EN, r_data[0]);
					break;
				case PE_INF_HW_H13_M_CVB_FIR_Y_EN:
					PE_CVB_H13B0_QRd01(cvb_dig_filt_ctrl3,	fir_y_en,		r_data[0]);
					sprintf(buffer, "   [0x%02x][CVB_fir_y_en](0:off,1:on)    : %d\n", \
						PE_INF_HW_H13_M_CVB_FIR_Y_EN, r_data[0]);
					break;
				case PE_INF_HW_H13_M_CVB_FIR_C_EN:
					PE_CVB_H13B0_QRd01(cvb_dig_filt_ctrl3,	fir_c_en,		r_data[0]);
					sprintf(buffer, "   [0x%02x][CVB_fir_c_en](0:off,1:on)    : %d\n", \
						PE_INF_HW_H13_M_CVB_FIR_C_EN, r_data[0]);
					break;
				case PE_INF_HW_H13_M_P0L_DETOUR:
					PE_P0L_H13B0_QRd01(p0l_pe0_opmode,		detour_enable,	r_data[0]);
					sprintf(buffer, "   [0x%02x][P0L_detour](0:off,1:on)      : %d\n", \
						PE_INF_HW_H13_M_P0L_DETOUR, r_data[0]);
					break;
				case PE_INF_HW_H13_M_P0L_IPC_Y:
					PE_P0L_H13B0_QRd01(p0l_ipc_ctrl_00,		cr_luma_mode,	r_data[0]);
					sprintf(buffer, "   [0x%02x][P0L_ipc_y_md](0:2d,1:3d,2:ma): %d\n", \
						PE_INF_HW_H13_M_P0L_IPC_Y, r_data[0]);
					break;
				case PE_INF_HW_H13_M_P0L_IPC_C:
					PE_P0L_H13B0_QRd01(p0l_ipc_ctrl_00,		cr_chroma_mode,	r_data[0]);
					sprintf(buffer, "   [0x%02x][P0L_ipc_c_md](0:2d,1:3d,2:ma): %d\n", \
						PE_INF_HW_H13_M_P0L_IPC_C, r_data[0]);
					break;
				case PE_INF_HW_H13_M_P0R_DETOUR:
					PE_P0R_H13B0_QRd01(p0r_pe0_opmode,		detour_enable,	r_data[0]);
					sprintf(buffer, "   [0x%02x][P0R_detour](0:off,1:on)      : %d\n", \
						PE_INF_HW_H13_M_P0R_DETOUR, r_data[0]);
					break;
				case PE_INF_HW_H13_M_P0R_IPC_Y:
					PE_P0R_H13B0_QRd01(p0r_ipc_ctrl_00,		cr_luma_mode,	r_data[0]);
					sprintf(buffer, "   [0x%02x][P0R_ipc_y_md](0:2d,1:3d,2:ma): %d\n", \
						PE_INF_HW_H13_M_P0R_IPC_Y, r_data[0]);
					break;
				case PE_INF_HW_H13_M_P0R_IPC_C:
					PE_P0R_H13B0_QRd01(p0r_ipc_ctrl_00,		cr_chroma_mode,	r_data[0]);
					sprintf(buffer, "   [0x%02x][P0R_ipc_c_md](0:2d,1:3d,2:ma): %d\n", \
						PE_INF_HW_H13_M_P0R_IPC_C, r_data[0]);
					break;
				case PE_INF_HW_H13_M_MSL_FLT_SEL_Y:
					PE_MSL_H13B0_QRd01(msc_ctrl_l,	filter_type_sel_luma,	r_data[0]);
					sprintf(buffer, "   [0x%02x][MSL_bilinear](0:off,1:on)    : %d\n", \
						PE_INF_HW_H13_M_MSL_FLT_SEL_Y, r_data[0]);
					break;
				case PE_INF_HW_H13_M_MSR_FLT_SEL_Y:
					PE_MSR_H13B0_QRd01(msc_ctrl_r,	filter_type_sel_luma,	r_data[0]);
					sprintf(buffer, "   [0x%02x][MSR_bilinear](0:off,1:on)    : %d\n", \
						PE_INF_HW_H13_M_MSR_FLT_SEL_Y, r_data[0]);
					break;
				case PE_INF_HW_H13_M_OVL_DARK_W0_EN:
					PE_OVL_H13B0_QRd01(ovl_ov_ctrl4,		w0_dark_en,		r_data[0]);
					sprintf(buffer, "   [0x%02x][OVL_w0_dark_en](0:off,1:on)  : %d\n", \
						PE_INF_HW_H13_M_OVL_DARK_W0_EN, r_data[0]);
					break;
				case PE_INF_HW_H13_M_OVL_DARK_W1_EN:
					PE_OVL_H13B0_QRd01(ovl_ov_ctrl4,		w1_dark_en,		r_data[0]);
					sprintf(buffer, "   [0x%02x][OVL_w1_dark_en](0:off,1:on)  : %d\n", \
						PE_INF_HW_H13_M_OVL_DARK_W1_EN, r_data[0]);
					break;
				case PE_INF_HW_H13_M_OVL_DARK_COLOR:
					PE_OVL_H13B0_QRd01(ovl_ov_ctrl2,		dark_color_yy,	r_data[0]);
					PE_OVL_H13B0_QRd01(ovl_ov_ctrl2,		dark_color_cb,	r_data[1]);
					PE_OVL_H13B0_QRd01(ovl_ov_ctrl2,		dark_color_cr,	r_data[2]);
					sprintf(buffer, "   [0x%02x][OVL_dark_color](ycbcr)(0~5)  : 0x%02x 0x%02x 0x%02x\n", \
						PE_INF_HW_H13_M_OVL_DARK_COLOR, r_data[0], r_data[1], r_data[2]);
					break;
				case PE_INF_HW_H13_M_OVL_BACK_COLOR:
					PE_OVL_H13B0_QRd01(ovl_ov_ctrl2,		back_color_yy,	r_data[0]);
					PE_OVL_H13B0_QRd01(ovl_ov_ctrl2,		back_color_cb,	r_data[1]);
					PE_OVL_H13B0_QRd01(ovl_ov_ctrl2,		back_color_cr,	r_data[2]);
					sprintf(buffer, "   [0x%02x][OVL_back_color](ycbcr)(0~5)  : 0x%02x 0x%02x 0x%02x\n", \
						PE_INF_HW_H13_M_OVL_BACK_COLOR, r_data[0], r_data[1], r_data[2]);
					break;
				case PE_INF_HW_H13_M_OVR_DARK_W0_EN:
					PE_OVR_H13B0_QRd01(ovr_ov_ctrl4,		w0_dark_en,		r_data[0]);
					sprintf(buffer, "   [0x%02x][OVR_w0_dark_en](0:off,1:on)  : %d\n", \
						PE_INF_HW_H13_M_OVR_DARK_W0_EN, r_data[0]);
					break;
				case PE_INF_HW_H13_M_OVR_DARK_W1_EN:
					PE_OVR_H13B0_QRd01(ovr_ov_ctrl4,		w1_dark_en,		r_data[0]);
					sprintf(buffer, "   [0x%02x][OVR_w1_dark_en](0:off,1:on)  : %d\n", \
						PE_INF_HW_H13_M_OVR_DARK_W1_EN, r_data[0]);
					break;
				case PE_INF_HW_H13_M_OVR_DARK_COLOR:
					PE_OVR_H13B0_QRd01(ovr_ov_ctrl2,		dark_color_yy,	r_data[0]);
					PE_OVR_H13B0_QRd01(ovr_ov_ctrl2,		dark_color_cb,	r_data[1]);
					PE_OVR_H13B0_QRd01(ovr_ov_ctrl2,		dark_color_cr,	r_data[2]);
					sprintf(buffer, "   [0x%02x][OVR_dark_color](ycbcr)(0~5)  : 0x%02x 0x%02x 0x%02x\n", \
						PE_INF_HW_H13_M_OVR_DARK_COLOR, r_data[0], r_data[1], r_data[2]);
					break;
				case PE_INF_HW_H13_M_OVR_BACK_COLOR:
					PE_OVR_H13B0_QRd01(ovr_ov_ctrl2,		back_color_yy,	r_data[0]);
					PE_OVR_H13B0_QRd01(ovr_ov_ctrl2,		back_color_cb,	r_data[1]);
					PE_OVR_H13B0_QRd01(ovr_ov_ctrl2,		back_color_cr,	r_data[2]);
					sprintf(buffer, "   [0x%02x][OVR_back_color](ycbcr)(0~5)  : 0x%02x 0x%02x 0x%02x\n", \
						PE_INF_HW_H13_M_OVR_BACK_COLOR, r_data[0], r_data[1], r_data[2]);
					break;
				case PE_INF_HW_H13_M_PE1_DETOUR:
					PE_P1L_H13B0_QRd01(pe1_top_ctrl_02,		pe1_detour,		r_data[0]);
					sprintf(buffer, "   [0x%02x][P1L_detour](0:off,1:on)      : %d\n", \
						PE_INF_HW_H13_M_PE1_DETOUR, r_data[0]);
					break;
				case PE_INF_HW_H13_M_PE1_REP0_IN_EN:
					PE_P1L_H13B0_QRd01(pe1_conv_ctrl_03,	pxl_rep_in,		r_data[0]);
					sprintf(buffer, "   [0x%02x][P1L_L_mute](0:off,1:on)      : %d\n", \
						PE_INF_HW_H13_M_PE1_REP0_IN_EN, r_data[0]);
					break;
				case PE_INF_HW_H13_M_PE1_REP0_COLOR:
					PE_P1L_H13B0_QRd01(pe1_conv_ctrl_05,	pxl_rep_value_r,	r_data[0]);
					PE_P1L_H13B0_QRd01(pe1_conv_ctrl_05,	pxl_rep_value_g,	r_data[1]);
					PE_P1L_H13B0_QRd01(pe1_conv_ctrl_05,	pxl_rep_value_b,	r_data[2]);
					sprintf(buffer, "   [0x%02x][P1L_L_color](rgb)(0~5)       : 0x%03x 0x%03x 0x%03x\n", \
						PE_INF_HW_H13_M_PE1_REP0_COLOR, r_data[0], r_data[1], r_data[2]);
					break;
				case PE_INF_HW_H13_M_PE1_REP1_IN_EN:
					PE_P1L_H13B0_QRd01(pe1_conv_ctrl_08,		pxl_rep_in,		r_data[0]);
					sprintf(buffer, "   [0x%02x][P1L_R_mute](0:off,1:on)      : %d\n", \
						PE_INF_HW_H13_M_PE1_REP1_IN_EN, r_data[0]);
					break;
				case PE_INF_HW_H13_M_PE1_REP1_COLOR:
					PE_P1L_H13B0_QRd01(pe1_conv_ctrl_0a,	pxl_rep_value_r,	r_data[0]);
					PE_P1L_H13B0_QRd01(pe1_conv_ctrl_0a,	pxl_rep_value_g,	r_data[1]);
					PE_P1L_H13B0_QRd01(pe1_conv_ctrl_0a,	pxl_rep_value_b,	r_data[2]);
					sprintf(buffer, "   [0x%02x][P1L_R_color](rgb)(0~5)       : 0x%03x 0x%03x 0x%03x\n", \
						PE_INF_HW_H13_M_PE1_REP1_COLOR, r_data[0], r_data[1], r_data[2]);
					break;
				default:
					sprintf(buffer, "%s", PE_INF_H13_PRINT_END);
					break;
			}
			if (rd_cnt<PE_INF_HW_H13_M_MAX)
			{
				rd_cnt++;
			}
			else
			{
				rd_cnt = 0;
			}
			memcpy(pstParams->data,buffer,sizeof(char)*PE_TRACE_STR_SIZE);
		}
		else
		{
			PE_INF_H13_DBG_PRINT("nothing to do.\n");
		}
	}while(0);
	return ret;
}
/**
 * get level info
 *
 * @param   *pstParams [in/out] LX_PE_INF_LEVEL_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_INF_H13_GetLevelInfo(LX_PE_INF_LEVEL_T *pstParams)
{
	int ret = RET_OK;
	UINT32 r_data = 0;
	LX_PE_WIN_ID win_id;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		win_id = PE_GET_CHECKED_WINID(pstParams->win_id);
		if(PE_KDRV_VER_H13BX)
		{
			if(PE_CHECK_WIN0(win_id))
			{
				if(pstParams->sel&LX_PE_LEVEL_MOTION)
				{	PE_P0L_H13B0_QRd01(p0l_tnr_status_00, x_avg_t,	 	pstParams->motion_level);}
				if(pstParams->sel&LX_PE_LEVEL_NOISE)
				{	PE_P0L_H13B0_QRd01(p0l_tnr_status_00, nt_lvl, 		pstParams->noise_level);}
				if(pstParams->sel&LX_PE_LEVEL_PEAKPOS)
				{
					PE_P0L_H13B0_QRd01(p0l_tnr_status_00, x_peak_t2, 	r_data);
					pstParams->peakpos_level = GET_BITS(r_data, 0, 6)<<4;
				}
			}
			if(PE_CHECK_WIN1(win_id))
			{
				if(pstParams->sel&LX_PE_LEVEL_MOTION)
				{	PE_P0R_H13B0_QRd01(p0r_tnr_status_00, x_avg_t, 		pstParams->motion_level);}
				if(pstParams->sel&LX_PE_LEVEL_NOISE)
				{	PE_P0R_H13B0_QRd01(p0r_tnr_status_00, nt_lvl, 		pstParams->noise_level);}
				if(pstParams->sel&LX_PE_LEVEL_PEAKPOS)
				{
					PE_P0R_H13B0_QRd01(p0r_tnr_status_00, x_peak_t2, 	r_data);
					pstParams->peakpos_level = GET_BITS(r_data, 0, 6)<<4;
				}
			}
		}
		else if(PE_KDRV_VER_H13AX)
		{
			if(PE_CHECK_WIN0(win_id))
			{
				if(pstParams->sel&LX_PE_LEVEL_MOTION)
				{	PE_P0L_H13_QRd01(p0l_tnr_status_00, x_avg_t, 	pstParams->motion_level);}
				if(pstParams->sel&LX_PE_LEVEL_NOISE)
				{	PE_P0L_H13_QRd01(p0l_tnr_status_00, nt_lvl, 	pstParams->noise_level);}
				if(pstParams->sel&LX_PE_LEVEL_PEAKPOS)
				{
					PE_P0L_H13_QRd01(p0l_tnr_status_00, x_peak_t2, 	r_data);
					pstParams->peakpos_level = GET_BITS(r_data, 0, 6)<<4;
				}
			}
			if(PE_CHECK_WIN1(win_id))
			{
				if(pstParams->sel&LX_PE_LEVEL_MOTION)
				{	PE_P0R_H13_QRd01(p0r_tnr_status_00, x_avg_t, 	pstParams->motion_level);}
				if(pstParams->sel&LX_PE_LEVEL_NOISE)
				{	PE_P0R_H13_QRd01(p0r_tnr_status_00, nt_lvl, 	pstParams->noise_level);}
				if(pstParams->sel&LX_PE_LEVEL_PEAKPOS)
				{
					PE_P0R_H13_QRd01(p0r_tnr_status_00, x_peak_t2, 	r_data);
					pstParams->peakpos_level = GET_BITS(r_data, 0, 6)<<4;
				}
			}
		}
		else
		{
			PE_INF_H13_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}

/**
 * set misc table
 *
 * @param   *pstParams [in] LX_PE_ETC_TBL_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_INF_H13_SetMiscTbl(LX_PE_ETC_TBL_T *pstParams)
{
    int ret = RET_OK;
	UINT32 max_num = 0;
	do{
		CHECK_KNULL(pstParams);
		if (PE_KDRV_VER_H13)
		{
			max_num = sizeof(_g_pe_inf_h13_dbg_set_fn)/sizeof(PE_ETC_MISC_TBL_T);
			PE_INF_H13_CHECK_CODE(pstParams->func_num>=max_num, break, \
				"[%s,%d] invalid func_num[%d].\n", __F__, __L__, pstParams->func_num);
			CHECK_KNULL(_g_pe_inf_h13_dbg_set_fn[pstParams->func_num].func);
			ret = _g_pe_inf_h13_dbg_set_fn[pstParams->func_num].func(pstParams);
			PE_INF_H13_CHECK_CODE(ret, break, "[%s,%d] %s() error.\n", \
				__F__, __L__, _g_pe_inf_h13_dbg_set_fn[pstParams->func_num].fn_name);
		}
		else
		{
			PE_INF_H13_DBG_PRINT("nothing to do.\n");
			ret = RET_OK;
		}
	}while (0);
	return ret;
}
/**
 * get misc debug(for debug)
 *
 * @param   *pstParams [in/out] LX_PE_ETC_TBL_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_INF_H13_GetMiscTbl(LX_PE_ETC_TBL_T *pstParams)
{
    int ret = RET_OK;
	UINT32 max_num = 0;
	do{
		CHECK_KNULL(pstParams);
		if (PE_KDRV_VER_H13)
		{
			max_num = sizeof(_g_pe_inf_h13_dbg_get_fn)/sizeof(PE_ETC_MISC_TBL_T);
			PE_INF_H13_CHECK_CODE(pstParams->func_num>=max_num, break, \
				"[%s,%d] invalid func_num[%d].\n", __F__, __L__, pstParams->func_num);
			CHECK_KNULL(_g_pe_inf_h13_dbg_get_fn[pstParams->func_num].func);
			ret = _g_pe_inf_h13_dbg_get_fn[pstParams->func_num].func(pstParams);
			PE_INF_H13_CHECK_CODE(ret, break, "[%s,%d] %s() error.\n", \
				__F__, __L__, _g_pe_inf_h13_dbg_get_fn[pstParams->func_num].fn_name);
		}
		else
		{
			PE_INF_H13_DBG_PRINT("nothing to do.\n");
			ret = RET_OK;
		}
	}while (0);
	return ret;
}

