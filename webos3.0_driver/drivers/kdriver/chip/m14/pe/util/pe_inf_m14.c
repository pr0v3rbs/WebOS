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

/** @file pe_inf_m14.c
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

#include "pe_hw_m14.h"
#include "pe_reg_m14.h"
#include "pe_inf_m14.h"
#include "pe_fwi_m14.h"
#include "pe_win_hw_m14.h"
#include "pe_shp_hw_m14.h"
#include "pe_ccm_hw_m14.h"
#include "pe_hst_hw_m14.h"
#include "pe_cmn_hw_m14.h"
#include "pe_dnt_hw_m14.h"
#include "pe_nrd_hw_m14.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
/* drv ver, yyyy/mm/dd/hh:mm */
#define PE_INF_M14_DRV_VER		"2014/11/05/10:00"

#define PE_INF_M14_BUF_MAX				100

#define PE_INF_M14_SCL_RESIZE_H_MAX		960		/* 1920 * 0.5 */
#define PE_INF_M14_SCL_RESIZE_V_MAX		540		/* 1080 * 0.5 */
#define PE_INF_M14_SCL_IS_HD_MIN		1024

/* define for showing more detail trace log */
#undef PE_INF_M14_DETAIL_TRACE_LOG

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define PE_INF_M14_ERROR	printk

#define PE_INF_M14_DBG_PRINT(fmt,args...)	\
	if(_g_pe_inf_m14_trace) printk("[%x,%x][%s,%d] "fmt,PE_CHIP_VER,g_pe_kdrv_ver_mask,__F__,__L__,##args)
#define PE_INF_M14_CHECK_CODE(_checker,_action,fmt,args...)	\
	{if(_checker){PE_INF_M14_ERROR(fmt,##args);_action;}}

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/
/**
 *	pe trace time type
 */
typedef struct {
	UINT32 sec;			///< sec
	UINT32 msec;		///< msec
	UINT32 usec;		///< usec
}
PE_INF_M14_TIME_T;

/**
 *	pe trace data buffer type
 */
typedef struct {
	PE_INF_M14_TIME_T time;			///< time
	char str[PE_TRACE_STR_SIZE];	///< str
}
PE_INF_M14_DATA_BUF_T;

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
static int PE_INF_M14_GetGeneralPicInfo(LX_PE_ETC_TBL_T *pstParams);
static int PE_INF_M14_SetPathMonitor(LX_PE_ETC_TBL_T *pstParams);
static int PE_INF_M14_GetPathMonitor(LX_PE_ETC_TBL_T *pstParams);
static void PE_INF_M14_SortTraceDataBuf(PE_INF_M14_DATA_BUF_T *dst);
static int PE_INF_M14_GetTraceDataBuf(LX_PE_ETC_TBL_T *pstParams);
static int PE_INF_M14_SetLedPixelValue(LX_PE_ETC_TBL_T *pstParams);
static int PE_INF_M14_GetLedPixelValue(LX_PE_ETC_TBL_T *pstParams);

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
static PE_INF_M14_DATA_BUF_T _g_pe_inf_m14_buf[PE_INF_M14_BUF_MAX];
static PE_INF_M14_DATA_BUF_T _g_pe_inf_m14_buf_temp[PE_INF_M14_BUF_MAX];

static UINT32 _g_pe_inf_m14_trace=0x0;		//default should be off.
static PE_INF_M14_SETTINGS_T _g_pe_inf_hw_m14_info;

static PE_ETC_MISC_TBL_T _g_pe_inf_m14_dbg_set_fn[] = {\
	{/* 00 */ PE_TSK_M14_SetTaskOperation,		"PE_TSK_M14_SetTaskOperation"}, \
	{/* 01 */ PE_REG_M14_WrReg,					"PE_REG_M14_WrReg"}, \
	{/* 02 */ PE_INF_M14_SetPathMonitor,		"PE_INF_M14_SetPathMonitor"}, \
	{/* 03 */ PE_INF_M14_SetLedPixelValue,		"PE_INF_M14_SetLedPixelValue"}
};
static PE_ETC_MISC_TBL_T _g_pe_inf_m14_dbg_get_fn[] = {\
	{/* 00 */ PE_TSK_M14_GetTaskOperation,		"PE_TSK_M14_GetTaskOperation"}, \
	{/* 01 */ PE_REG_M14_RdReg,					"PE_REG_M14_RdReg"}, \
	{/* 02 */ PE_INF_M14_GetGeneralPicInfo,		"PE_INF_M14_GetGeneralPicInfo"}, \
	{/* 03 */ PE_HST_HW_M14_GetGlobalHistInfo,	"PE_HST_HW_M14_GetGlobalHistInfo"}, \
	{/* 04 */ PE_INF_M14_GetPathMonitor,		"PE_INF_M14_GetPathMonitor"}, \
	{/* 05 */ PE_SHP_HW_M14_RdSclFltCoeff,		"PE_SHP_HW_M14_RdSclFltCoeff"}, \
	{/* 06 */ PE_DNT_HW_M14_RdDefault,			"PE_DNT_HW_M14_RdDefault"}, \
	{/* 07 */ PE_NRD_HW_M14_RdDefault,			"PE_NRD_HW_M14_RdDefault"}, \
	{/* 08 */ PE_SHP_HW_M14_RdDefault,			"PE_SHP_HW_M14_RdDefault"}, \
	{/* 09 */ PE_INF_M14_GetTraceDataBuf,		"PE_INF_M14_GetTraceDataBuf"}, \
	{/* 10 */ PE_INF_M14_GetLedPixelValue,		"PE_INF_M14_GetLedPixelValue"}
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
int PE_INF_M14_Init(PE_CFG_CTRL_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_M14)
		{
			PE_INF_M14_DBG_PRINT("init global values.\n");
			memset0(_g_pe_inf_hw_m14_info);
		}
		else
		{
			PE_INF_M14_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
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
int PE_INF_M14_SetDebugSettings(LX_PE_DBG_SETTINGS_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		PE_INF_M14_DBG_PRINT("wid:%d,type:0x%x,[0x%x]print:0x%x,bypass:0x%x\n",\
			pstParams->win_id,pstParams->type,(0x1<<PE_ITEM_PKTMODL(ETC)),\
			pstParams->print_lvl,pstParams->bypass);
		/* set print level */
		if(pstParams->type&LX_PE_DBG_LV)
		{
			_g_pe_inf_m14_trace = (pstParams->print_lvl & (0x1<<PE_ITEM_PKTMODL(ETC)))? 0x1:0x0;
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
int PE_INF_M14_SetDisplayInfo(LX_PE_INF_DISPLAY_T *pstParams)
{
	int ret = RET_OK;
	PE_FWI_M14_SRC_INFO fwi_src;
	LX_PE_INF_DISPLAY_T *pp = NULL;
	LX_PE_INF_DISPLAY_T *disp0_info = &_g_pe_inf_hw_m14_info.disp_info[LX_PE_WIN_0];
	LX_PE_INF_DISPLAY_T *disp1_info = &_g_pe_inf_hw_m14_info.disp_info[LX_PE_WIN_1];
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		pp = pstParams;
		PE_INF_M14_DBG_PRINT("set[%d] : src:%d,res:%d (h:%d,v:%d)\n"\
			"fr:%d(%d),scan:%d,cstd:%d,hdmi:%d,scart:%d,dtv:%d,hdd:%d\n"\
			"out:%d,in:%d,user_o:%d,user_i:%d\n"\
			"in_win:%d,%d,%d,%d,out_win:%d,%d,%d,%d,af:%d\n"\
			"mode: tp,venc,adp,ext,wog,rev,oled:%d,%d,%d,%d,%d,%d,%d\n",\
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
			pp->mode.is_wb_wog, pp->mode.is_reverse, \
			pp->mode.is_oled);
		if(PE_KDRV_VER_M14)
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
			disp0_info->out_type	= disp1_info->out_type		= pstParams->out_type;
			disp0_info->hdd_type	= disp1_info->hdd_type		= pstParams->hdd_type;
			disp0_info->in_type		= disp1_info->in_type		= pstParams->in_type;
			disp0_info->user_o_type	= disp1_info->user_o_type	= pstParams->user_o_type;
			disp0_info->user_i_type	= disp1_info->user_i_type	= pstParams->user_i_type;
			disp0_info->act_fmt		= disp1_info->act_fmt		= pstParams->act_fmt;
			disp0_info->mode		= disp1_info->mode			= pstParams->mode;
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
				ret = PE_FWI_M14_SetSrcInfo(&fwi_src);
				PE_INF_M14_CHECK_CODE(ret,break,"[%s,%d] PE_FWI_M14_SetSrcInfo() error.\n",__F__,__L__);
			}
		}
		else
		{
			PE_INF_M14_DBG_PRINT("nothing to do\n");	ret = RET_OK;
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
int PE_INF_M14_GetDisplayInfo(LX_PE_INF_DISPLAY_T *pstParams)
{
	int ret = RET_OK;
	LX_PE_WIN_ID win_id,temp;
	PE_INF_M14_SETTINGS_T *pInfo=&_g_pe_inf_hw_m14_info;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		win_id = PE_GET_CHECKED_WINID(pstParams->win_id);
		if(PE_KDRV_VER_M14)
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
			PE_INF_M14_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
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
int PE_INF_M14_CheckSclInfo(void)
{
	int ret = RET_OK;
	UINT32 r_data;
	PE_INF_M14_CTRL_T *ctrl0_info = &_g_pe_inf_hw_m14_info.ctrl_info[LX_PE_WIN_0];
	PE_INF_M14_CTRL_T *ctrl1_info = &_g_pe_inf_hw_m14_info.ctrl_info[LX_PE_WIN_1];
	LX_PE_INF_DISPLAY_T *disp0_info=&_g_pe_inf_hw_m14_info.disp_info[LX_PE_WIN_0];
	LX_PE_INF_DISPLAY_T *disp1_info=&_g_pe_inf_hw_m14_info.disp_info[LX_PE_WIN_1];
	PE_CCM_HW_M14_SETTINGS_T ccm_info;
	do{
		if(PE_KDRV_VER_M14BX)
		{
			ret = PE_CCM_HW_M14_GetCurCcmSettings(&ccm_info);
			if(ret)	{PE_INF_M14_ERROR("[%s,%d] PE_CCM_HW_M14_GetCurCcmSettings() error.\n",__F__,__L__);}
			/* L */
	        PE_MSC_M14B0_RdFL(msc_scr_size);
	        PE_MSC_M14B0_RdFL(msc_win_in_offset);
	        PE_MSC_M14B0_RdFL(msc_win_in_size);
	        PE_MSC_M14B0_RdFL(msc_win_out_size);
	        PE_MSC_M14B0_RdFL(msc_c_format);
	        PE_MSC_M14B0_Rd01(msc_scr_size, 		input_h_size,		ctrl0_info->scl_src_h);
	        PE_MSC_M14B0_Rd01(msc_scr_size, 		input_v_size,		ctrl0_info->scl_src_v);
	        PE_MSC_M14B0_Rd01(msc_win_in_offset, 	in_win_h_offset,	ctrl0_info->i_win_h_ofst);
	        PE_MSC_M14B0_Rd01(msc_win_in_offset, 	in_win_v_offset,	ctrl0_info->i_win_v_ofst);
	        PE_MSC_M14B0_Rd01(msc_win_in_size, 		in_win_h_size,		ctrl0_info->i_win_h_size);
	        PE_MSC_M14B0_Rd01(msc_win_in_size, 		in_win_v_size,		ctrl0_info->i_win_v_size);
	        PE_MSC_M14B0_Rd01(msc_win_out_size, 	out_win_h_size,		ctrl0_info->o_win_h_size);
	        PE_MSC_M14B0_Rd01(msc_win_out_size, 	out_win_v_size,		ctrl0_info->o_win_v_size);
	        PE_MSC_M14B0_Rd01(msc_c_format, 		out_c_format,		r_data);
			ctrl0_info->is_yuv444 = (r_data==0x6)? 0x1:0x0;	//"100":yuv420,"101":yuv422,"110":yuv444
			#ifdef PE_HW_M14_USE_M14B_WCP_REG
			PE_OVL_M14B0_RdFL(ovl_ov_moffset);
			PE_OVL_M14B0_RdFL(ovl_ov_msize);
			PE_OVL_M14B0_Rd01(ovl_ov_moffset, 		m_hoffset,			ctrl0_info->m_win_h_ofst);
			PE_OVL_M14B0_Rd01(ovl_ov_moffset, 		m_voffset,			ctrl0_info->m_win_v_ofst);
			PE_OVL_M14B0_Rd01(ovl_ov_msize, 		m_hsize,			ctrl0_info->m_win_h_size);
			PE_OVL_M14B0_Rd01(ovl_ov_msize, 		m_vsize,			ctrl0_info->m_win_v_size);
			#else
			ctrl0_info->m_win_h_ofst = disp0_info->out_win.x_ofst;
			ctrl0_info->m_win_v_ofst = disp0_info->out_win.y_ofst;
			ctrl0_info->m_win_h_size = disp0_info->out_win.h_size;
			ctrl0_info->m_win_v_size = disp0_info->out_win.v_size;
			#endif
			PE_P0M_M14B0_RdFL(p0m_pe0_src_size);
			PE_P0M_M14B0_Rd01(p0m_pe0_src_size, 	hsize,				ctrl0_info->pe0_src_h);
			PE_P0M_M14B0_Rd01(p0m_pe0_src_size, 	vsize,				ctrl0_info->pe0_src_v);
			ctrl0_info->scl_resize = (ctrl0_info->o_win_h_size<PE_INF_M14_SCL_RESIZE_H_MAX && \
								ctrl0_info->o_win_v_size<PE_INF_M14_SCL_RESIZE_V_MAX)? 1:0;
			ctrl0_info->scl_hd = (ctrl0_info->scl_src_h>=PE_INF_M14_SCL_IS_HD_MIN)? 1:0;
			/* R */
			#ifdef PE_HW_M14_USE_M14B_WCP_REG
			PE_OVR_M14B0_RdFL(ovr_ov_moffset);
			PE_OVR_M14B0_RdFL(ovr_ov_msize);
			PE_OVR_M14B0_Rd01(ovr_ov_moffset,		m_hoffset,			ctrl1_info->m_win_h_ofst);
			PE_OVR_M14B0_Rd01(ovr_ov_moffset,		m_voffset,			ctrl1_info->m_win_v_ofst);
			PE_OVR_M14B0_Rd01(ovr_ov_msize, 		m_hsize,			ctrl1_info->m_win_h_size);
			PE_OVR_M14B0_Rd01(ovr_ov_msize, 		m_vsize,			ctrl1_info->m_win_v_size);
			#else
			ctrl1_info->m_win_h_ofst = disp1_info->out_win.x_ofst;
			ctrl1_info->m_win_v_ofst = disp1_info->out_win.y_ofst;
			ctrl1_info->m_win_h_size = disp1_info->out_win.h_size;
			ctrl1_info->m_win_v_size = disp1_info->out_win.v_size;
			#endif
			/* sub scaler */
			PE_SSC_M14B0_RdFL(ssc_scr_size);
			PE_SSC_M14B0_RdFL(ssc_win_in_offset);
			PE_SSC_M14B0_RdFL(ssc_win_in_size);
			PE_SSC_M14B0_RdFL(ssc_win_out_size);
			PE_SSC_M14B0_Rd01(ssc_scr_size,			input_h_size,		ctrl0_info->ssc_src_h);
			PE_SSC_M14B0_Rd01(ssc_scr_size,			input_v_size,		ctrl0_info->ssc_src_v);
			PE_SSC_M14B0_Rd01(ssc_win_in_offset,	in_win_h_offset,	ctrl0_info->ssc_i_win_x);
			PE_SSC_M14B0_Rd01(ssc_win_in_offset,	in_win_v_offset,	ctrl0_info->ssc_i_win_y);
			PE_SSC_M14B0_Rd01(ssc_win_in_size,		in_win_h_size,		ctrl0_info->ssc_i_win_h);
			PE_SSC_M14B0_Rd01(ssc_win_in_size,		in_win_v_size,		ctrl0_info->ssc_i_win_v);
			PE_SSC_M14B0_Rd01(ssc_win_out_size,		out_win_h_size,		ctrl0_info->ssc_o_win_h);
			PE_SSC_M14B0_Rd01(ssc_win_out_size,		out_win_v_size,		ctrl0_info->ssc_o_win_v);
			ctrl1_info->ssc_src_h	= ctrl0_info->ssc_src_h;
			ctrl1_info->ssc_src_v	= ctrl0_info->ssc_src_v;
			ctrl1_info->ssc_i_win_x	= ctrl0_info->ssc_i_win_x;
			ctrl1_info->ssc_i_win_y	= ctrl0_info->ssc_i_win_y;
			ctrl1_info->ssc_i_win_h	= ctrl0_info->ssc_i_win_h;
			ctrl1_info->ssc_i_win_v	= ctrl0_info->ssc_i_win_v;
			ctrl1_info->ssc_o_win_h	= ctrl0_info->ssc_o_win_h;
			ctrl1_info->ssc_o_win_v	= ctrl0_info->ssc_o_win_v;
			/* etc */
			ctrl0_info->hv_same = (ctrl0_info->o_win_h_size==ctrl0_info->ssc_o_win_h && \
				ctrl0_info->o_win_v_size==ctrl0_info->ssc_o_win_v)? 1:0;
			ctrl1_info->hv_same = (ctrl0_info->o_win_h_size<=PE_INF_M14_SCL_RESIZE_H_MAX && \
				ctrl0_info->o_win_v_size<=PE_INF_M14_SCL_RESIZE_V_MAX)? 1:0;
			ret = PE_HST_HW_M14_GetDetected3dFmt(&r_data);
			if(ret)	{PE_INF_M14_ERROR("[%s,%d] PE_HST_HW_M14_GetDetected3dFmt() error.\n",__F__,__L__);}
			ctrl0_info->det_3df = r_data;
			ctrl0_info->auto_cr_en = ccm_info.auto_cr.enable? 1:0;
			ret = PE_HST_HW_M14_GetSaturationState(LX_PE_WIN_0,&r_data);
			if(ret)	{PE_INF_M14_ERROR("[%s,%d] PE_HST_HW_M14_GetSaturationState() error.\n",__F__,__L__);}
			ctrl0_info->sat_status = r_data;
			ret = PE_HST_HW_M14_GetSaturationState(LX_PE_WIN_1,&r_data);
			if(ret)	{PE_INF_M14_ERROR("[%s,%d] PE_HST_HW_M14_GetSaturationState() error.\n",__F__,__L__);}
			ctrl1_info->sat_status = r_data;
			PE_PE1_M14B0_QRd01(pe1_mute_ctrl_00, pxl_rep_in,	ctrl0_info->mute_on);
			PE_PE1_M14B0_QRd01(pe1_mute_ctrl_05, pxl_rep_in,	ctrl1_info->mute_on);
			PE_PE1_M14B0_QRd01(pe1_fsw_ctrl_00,	reg_color_filter_status,ctrl0_info->pxl_rp_st);
			PE_PE1_M14B0_QRd01(pe1_top_ctrl_02,	pmux_position,	ctrl0_info->pmux_pos);	//0:after,1:before shp
			PE_PE1_M14B0_RdFL(pe1_fsw_ctrl_02);
			PE_PE1_M14B0_Rd01(pe1_fsw_ctrl_02,	reg_src_freeze,	ctrl0_info->is_src_frz);
			PE_PE1_M14B0_Rd01(pe1_fsw_ctrl_02,	reg_src_info,	ctrl0_info->fw_src);
		}
		else if(PE_KDRV_VER_M14AX)
		{
			ret = PE_CCM_HW_M14_GetCurCcmSettings(&ccm_info);
			if(ret)	{PE_INF_M14_ERROR("[%s,%d] PE_CCM_HW_M14_GetCurCcmSettings() error.\n",__F__,__L__);}
			/* L */
	        PE_MSL_M14A0_RdFL(msc_scr_size_l);
	        PE_MSL_M14A0_RdFL(msc_win_in_offset_l);
	        PE_MSL_M14A0_RdFL(msc_win_in_size_l);
	        PE_MSL_M14A0_RdFL(msc_win_out_size_l);
	        PE_MSL_M14A0_RdFL(msc_c_format_l);
	        PE_MSL_M14A0_Rd01(msc_scr_size_l, 		input_h_size,		ctrl0_info->scl_src_h);
	        PE_MSL_M14A0_Rd01(msc_scr_size_l, 		input_v_size,		ctrl0_info->scl_src_v);
	        PE_MSL_M14A0_Rd01(msc_win_in_offset_l, 	in_win_h_offset,	ctrl0_info->i_win_h_ofst);
	        PE_MSL_M14A0_Rd01(msc_win_in_offset_l, 	in_win_v_offset,	ctrl0_info->i_win_v_ofst);
	        PE_MSL_M14A0_Rd01(msc_win_in_size_l, 	in_win_h_size,		ctrl0_info->i_win_h_size);
	        PE_MSL_M14A0_Rd01(msc_win_in_size_l, 	in_win_v_size,		ctrl0_info->i_win_v_size);
	        PE_MSL_M14A0_Rd01(msc_win_out_size_l, 	out_win_h_size,		ctrl0_info->o_win_h_size);
	        PE_MSL_M14A0_Rd01(msc_win_out_size_l, 	out_win_v_size,		ctrl0_info->o_win_v_size);
	        PE_MSL_M14A0_Rd01(msc_c_format_l, 		in_c_format,		r_data);
			ctrl0_info->is_yuv444 = (r_data==0x6)? 0x1:0x0;	//"100":yuv420,"101":yuv422,"110":yuv444
			PE_OVL_M14A0_RdFL(ovl_ov_moffset);
			PE_OVL_M14A0_RdFL(ovl_ov_msize);
			PE_OVL_M14A0_Rd01(ovl_ov_moffset, 		m_hoffset,			ctrl0_info->m_win_h_ofst);
			PE_OVL_M14A0_Rd01(ovl_ov_moffset, 		m_voffset,			ctrl0_info->m_win_v_ofst);
			PE_OVL_M14A0_Rd01(ovl_ov_msize, 		m_hsize,			ctrl0_info->m_win_h_size);
			PE_OVL_M14A0_Rd01(ovl_ov_msize, 		m_vsize,			ctrl0_info->m_win_v_size);
			PE_P0L_M14A0_RdFL(p0l_pe0_src_size);
			PE_P0L_M14A0_Rd01(p0l_pe0_src_size, 	hsize,				ctrl0_info->pe0_src_h);
			PE_P0L_M14A0_Rd01(p0l_pe0_src_size, 	vsize,				ctrl0_info->pe0_src_v);
			ctrl0_info->scl_resize = (ctrl0_info->o_win_h_size<PE_INF_M14_SCL_RESIZE_H_MAX && \
								ctrl0_info->o_win_v_size<PE_INF_M14_SCL_RESIZE_V_MAX)? 1:0;
			ctrl0_info->scl_hd = (ctrl0_info->scl_src_h>=PE_INF_M14_SCL_IS_HD_MIN)? 1:0;
			/* R */
	        PE_MSR_M14A0_RdFL(msc_scr_size_r);
	        PE_MSR_M14A0_RdFL(msc_win_in_offset_r);
	        PE_MSR_M14A0_RdFL(msc_win_in_size_r);
	        PE_MSR_M14A0_RdFL(msc_win_out_size_r);
	        PE_MSR_M14A0_RdFL(msc_c_format_r);
	        PE_MSR_M14A0_Rd01(msc_scr_size_r, 		input_h_size,		ctrl1_info->scl_src_h);
	        PE_MSR_M14A0_Rd01(msc_scr_size_r,		input_v_size,		ctrl1_info->scl_src_v);
	        PE_MSR_M14A0_Rd01(msc_win_in_offset_r, 	in_win_h_offset,	ctrl1_info->i_win_h_ofst);
	        PE_MSR_M14A0_Rd01(msc_win_in_offset_r, 	in_win_v_offset,	ctrl1_info->i_win_v_ofst);
	        PE_MSR_M14A0_Rd01(msc_win_in_size_r, 	in_win_h_size,		ctrl1_info->i_win_h_size);
	        PE_MSR_M14A0_Rd01(msc_win_in_size_r, 	in_win_v_size,		ctrl1_info->i_win_v_size);
	        PE_MSR_M14A0_Rd01(msc_win_out_size_r, 	out_win_h_size,		ctrl1_info->o_win_h_size);
	        PE_MSR_M14A0_Rd01(msc_win_out_size_r, 	out_win_v_size,		ctrl1_info->o_win_v_size);
	        PE_MSR_M14A0_Rd01(msc_c_format_r,		in_c_format,		r_data);
			ctrl1_info->is_yuv444 = (r_data==0x6)? 0x1:0x0;	//"100":yuv420,"101":yuv422,"110":yuv444
			PE_OVR_M14A0_RdFL(ovr_ov_moffset);
			PE_OVR_M14A0_RdFL(ovr_ov_msize);
			PE_OVR_M14A0_Rd01(ovr_ov_moffset,		m_hoffset,			ctrl1_info->m_win_h_ofst);
			PE_OVR_M14A0_Rd01(ovr_ov_moffset,		m_voffset,			ctrl1_info->m_win_v_ofst);
			PE_OVR_M14A0_Rd01(ovr_ov_msize, 		m_hsize,			ctrl1_info->m_win_h_size);
			PE_OVR_M14A0_Rd01(ovr_ov_msize, 		m_vsize,			ctrl1_info->m_win_v_size);
			PE_P0R_M14A0_RdFL(p0r_pe0_src_size);
			PE_P0R_M14A0_Rd01(p0r_pe0_src_size,		hsize,				ctrl1_info->pe0_src_h);
			PE_P0R_M14A0_Rd01(p0r_pe0_src_size,		vsize,				ctrl1_info->pe0_src_v);
			ctrl1_info->scl_resize = (ctrl1_info->o_win_h_size<PE_INF_M14_SCL_RESIZE_H_MAX && \
								ctrl1_info->o_win_v_size<PE_INF_M14_SCL_RESIZE_V_MAX)? 1:0;
			ctrl1_info->scl_hd = (ctrl1_info->scl_src_h>=PE_INF_M14_SCL_IS_HD_MIN)? 1:0;
			/* sub scaler */
			PE_SSC_M14A0_RdFL(ssc_scr_size);
			PE_SSC_M14A0_RdFL(ssc_win_in_offset);
			PE_SSC_M14A0_RdFL(ssc_win_in_size);
			PE_SSC_M14A0_RdFL(ssc_win_out_size);
			PE_SSC_M14A0_Rd01(ssc_scr_size,			input_h_size,		ctrl0_info->ssc_src_h);
			PE_SSC_M14A0_Rd01(ssc_scr_size,			input_v_size,		ctrl0_info->ssc_src_v);
			PE_SSC_M14A0_Rd01(ssc_win_in_offset,	in_win_h_offset,	ctrl0_info->ssc_i_win_x);
			PE_SSC_M14A0_Rd01(ssc_win_in_offset,	in_win_v_offset,	ctrl0_info->ssc_i_win_y);
			PE_SSC_M14A0_Rd01(ssc_win_in_size,		in_win_h_size,		ctrl0_info->ssc_i_win_h);
			PE_SSC_M14A0_Rd01(ssc_win_in_size,		in_win_v_size,		ctrl0_info->ssc_i_win_v);
			PE_SSC_M14A0_Rd01(ssc_win_out_size,		out_win_h_size,		ctrl0_info->ssc_o_win_h);
			PE_SSC_M14A0_Rd01(ssc_win_out_size,		out_win_v_size,		ctrl0_info->ssc_o_win_v);
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
			ctrl1_info->hv_same = (ctrl0_info->m_win_h_size<=PE_INF_M14_SCL_RESIZE_H_MAX && \
				ctrl0_info->m_win_v_size<=PE_INF_M14_SCL_RESIZE_V_MAX)? 1:0;
			ret = PE_HST_HW_M14_GetDetected3dFmt(&r_data);
			if(ret)	{PE_INF_M14_ERROR("[%s,%d] PE_HST_HW_M14_GetDetected3dFmt() error.\n",__F__,__L__);}
			ctrl0_info->det_3df = r_data;
			ctrl0_info->auto_cr_en = ccm_info.auto_cr.enable? 1:0;
			ret = PE_HST_HW_M14_GetSaturationState(LX_PE_WIN_0,&r_data);
			if(ret)	{PE_INF_M14_ERROR("[%s,%d] PE_HST_HW_M14_GetSaturationState() error.\n",__F__,__L__);}
			ctrl0_info->sat_status = r_data;
			ret = PE_HST_HW_M14_GetSaturationState(LX_PE_WIN_1,&r_data);
			if(ret)	{PE_INF_M14_ERROR("[%s,%d] PE_HST_HW_M14_GetSaturationState() error.\n",__F__,__L__);}
			ctrl1_info->sat_status = r_data;
			PE_P1L_M14A0_QRd01(pe1_conv_ctrl_03, pxl_rep_in,	ctrl0_info->mute_on);
			PE_P1L_M14A0_QRd01(pe1_conv_ctrl_08, pxl_rep_in,	ctrl1_info->mute_on);
			PE_P1L_M14A0_QRd01(pe1_wb_ctrl_00,	reg_color_filter_status,	ctrl0_info->pxl_rp_st);
			PE_P1L_M14A0_QRd01(pe1_dse_ctrl_01,	reg_src_freeze,	ctrl0_info->is_src_frz);
			ctrl0_info->pmux_pos = 1;	//before shp
			ctrl0_info->fw_src = 1;		//default
		}
		else
		{
			PE_INF_M14_DBG_PRINT("nothing to do\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * get current info.
 *
 * @param   *pstParams [in/out] PE_INF_M14_SETTINGS_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_INF_M14_GetCurInfSettings(PE_INF_M14_SETTINGS_T *pstParams)
{
	int ret = RET_OK;
	static UINT32 count = 0;
	PE_INF_M14_SETTINGS_T *pInfo = &_g_pe_inf_hw_m14_info;
	LX_PE_INF_DISPLAY_T *disp0_info = &_g_pe_inf_hw_m14_info.disp_info[LX_PE_WIN_0];
	LX_PE_INF_DISPLAY_T *disp1_info = &_g_pe_inf_hw_m14_info.disp_info[LX_PE_WIN_1];
	PE_INF_M14_CTRL_T *ctrl0_info = &_g_pe_inf_hw_m14_info.ctrl_info[LX_PE_WIN_0];
	PE_INF_M14_CTRL_T *ctrl1_info = &_g_pe_inf_hw_m14_info.ctrl_info[LX_PE_WIN_1];
	do{
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_M14)
		{
			memcpy(pstParams,pInfo,sizeof(PE_INF_M14_SETTINGS_T));
			if(_g_pe_inf_m14_trace)
			{
				if(count>PE_PRINT_COUNT_NUM)
				{
					PE_INF_M14_DBG_PRINT("disp[0] : src:%d,fmt:%d (h:%d,v:%d)\n", \
						disp0_info->src_type, disp0_info->fmt_type, \
						disp0_info->in_h_size, disp0_info->in_v_size);
					PE_INF_M14_DBG_PRINT("disp[0] : fr:%d(%d),scan:%d,cstd,hdmi,scart,dtv,hdd:%d,%d,%d,%d,%d\n", \
						disp0_info->fr_type, disp0_info->in_f_rate, disp0_info->scan_type, \
						disp0_info->cstd_type, disp0_info->hdmi_type, disp0_info->scart_type, \
						disp0_info->dtv_type, disp0_info->hdd_type);
					PE_INF_M14_DBG_PRINT("disp[0] : out:%d,in:%d,user_o:%d,user_i:%d\n", \
						disp0_info->out_type, disp0_info->in_type, disp0_info->user_o_type, \
						disp0_info->user_i_type);
					PE_INF_M14_DBG_PRINT("disp[0] : in_win:%d,%d,%d,%d,out_win:%d,%d,%d,%d,af:%d\n", \
						disp0_info->in_win.x_ofst, disp0_info->in_win.y_ofst, \
						disp0_info->in_win.h_size, disp0_info->in_win.v_size, \
						disp0_info->out_win.x_ofst, disp0_info->out_win.y_ofst, \
						disp0_info->out_win.h_size, disp0_info->out_win.v_size, disp0_info->act_fmt);
					PE_INF_M14_DBG_PRINT("disp[0] : tp,venc,adp,ext,wog,rev,oled:%d,%d,%d,%d,%d,%d,%d\n", \
						disp0_info->mode.is_tp, disp0_info->mode.is_venc, \
						disp0_info->mode.is_adpt_st, disp0_info->mode.is_ext_frc, \
						disp0_info->mode.is_wb_wog, disp0_info->mode.is_reverse, \
						disp0_info->mode.is_oled);
					PE_INF_M14_DBG_PRINT("ctrl[0] : src:%d,%d,scl:%d,%d,i_w:%d,%d,%d,%d\n",\
						ctrl0_info->pe0_src_h,ctrl0_info->pe0_src_v,\
						ctrl0_info->scl_src_h,ctrl0_info->scl_src_v,\
						ctrl0_info->i_win_h_ofst,ctrl0_info->i_win_v_ofst,\
						ctrl0_info->i_win_h_size,ctrl0_info->i_win_v_size);
					PE_INF_M14_DBG_PRINT("ctrl[0] : scl_o_w:%d,%d,ov_m_w:%d,%d,%d,%d\n",\
						ctrl0_info->o_win_h_size,ctrl0_info->o_win_v_size,\
						ctrl0_info->m_win_h_ofst,ctrl0_info->m_win_v_ofst,\
						ctrl0_info->m_win_h_size,ctrl0_info->m_win_v_size);
					PE_INF_M14_DBG_PRINT("ctrl[0] : y444:%d,det_3df:%d,acr_en:%d,s_stat:%d,mute_on:%d,hv_same:%d\n",\
						ctrl0_info->is_yuv444,ctrl0_info->det_3df,\
						ctrl0_info->auto_cr_en,ctrl0_info->sat_status,\
						ctrl0_info->mute_on,ctrl0_info->hv_same);
					PE_INF_M14_DBG_PRINT("ctrl[0] : ssc src,i,o:%d,%d, %d,%d,%d,%d, %d,%d, pxl_rp,s_frz,pmux,f_s:%d,%d,%d,%d\n",\
						ctrl0_info->ssc_src_h,ctrl0_info->ssc_src_v,\
						ctrl0_info->ssc_i_win_x,ctrl0_info->ssc_i_win_y,\
						ctrl0_info->ssc_i_win_h,ctrl0_info->ssc_i_win_v,\
						ctrl0_info->ssc_o_win_h,ctrl0_info->ssc_o_win_v,\
						ctrl0_info->pxl_rp_st,ctrl0_info->is_src_frz,ctrl0_info->pmux_pos,ctrl0_info->fw_src);
					PE_INF_M14_DBG_PRINT("disp[1] : src:%d,fmt:%d (h:%d,v:%d)\n", \
						disp1_info->src_type, disp1_info->fmt_type, \
						disp1_info->in_h_size, disp1_info->in_v_size);
					PE_INF_M14_DBG_PRINT("disp[1] : fr:%d(%d),scan:%d,cstd,hdmi,scart,dtv,hdd:%d,%d,%d,%d,%d\n", \
						disp1_info->fr_type, disp1_info->in_f_rate, disp1_info->scan_type, \
						disp1_info->cstd_type, disp1_info->hdmi_type, disp1_info->scart_type, \
						disp1_info->dtv_type, disp1_info->hdd_type);
					PE_INF_M14_DBG_PRINT("disp[1] : out:%d,in:%d,user_o:%d,user_i:%d\n", \
						disp1_info->out_type, disp1_info->in_type, disp1_info->user_o_type, \
						disp1_info->user_i_type);
					PE_INF_M14_DBG_PRINT("disp[1] : in_win:%d,%d,%d,%d,out_win:%d,%d,%d,%d,af:%d\n", \
						disp1_info->in_win.x_ofst, disp1_info->in_win.y_ofst, \
						disp1_info->in_win.h_size, disp1_info->in_win.v_size, \
						disp1_info->out_win.x_ofst, disp1_info->out_win.y_ofst, \
						disp1_info->out_win.h_size, disp1_info->out_win.v_size, disp1_info->act_fmt);
					PE_INF_M14_DBG_PRINT("disp[1] : tp,venc,adp,ext,wog,rev,oled:%d,%d,%d,%d,%d,%d,%d\n", \
						disp1_info->mode.is_tp, disp1_info->mode.is_venc, \
						disp1_info->mode.is_adpt_st, disp1_info->mode.is_ext_frc, \
						disp1_info->mode.is_wb_wog, disp1_info->mode.is_reverse, \
						disp0_info->mode.is_oled);
					PE_INF_M14_DBG_PRINT("ctrl[1] : src:%d,%d,scl:%d,%d,i_w:%d,%d,%d,%d\n",\
						ctrl1_info->pe0_src_h,ctrl1_info->pe0_src_v,\
						ctrl1_info->scl_src_h,ctrl1_info->scl_src_v,\
						ctrl1_info->i_win_h_ofst,ctrl1_info->i_win_v_ofst,\
						ctrl1_info->i_win_h_size,ctrl1_info->i_win_v_size);
					PE_INF_M14_DBG_PRINT("ctrl[1] : scl_o_w:%d,%d,ov_m_w:%d,%d,%d,%d\n",\
						ctrl1_info->o_win_h_size,ctrl1_info->o_win_v_size,\
						ctrl1_info->m_win_h_ofst,ctrl1_info->m_win_v_ofst,\
						ctrl1_info->m_win_h_size,ctrl1_info->m_win_v_size);
					PE_INF_M14_DBG_PRINT("ctrl[1] : y444:%d,det_3df:%d,acr_en:%d,s_stat:%d,mute_on:%d,hv_same:%d\n",\
						ctrl1_info->is_yuv444,ctrl1_info->det_3df,\
						ctrl1_info->auto_cr_en,ctrl1_info->sat_status,\
						ctrl1_info->mute_on,ctrl1_info->hv_same);
					PE_INF_M14_DBG_PRINT("ctrl[1] : ssc src,i,o:%d,%d, %d,%d,%d,%d, %d,%d, pxl_rp,s_frz,pmux,f_s:%d,%d,%d,%d\n",\
						ctrl1_info->ssc_src_h,ctrl1_info->ssc_src_v,\
						ctrl1_info->ssc_i_win_x,ctrl1_info->ssc_i_win_y,\
						ctrl1_info->ssc_i_win_h,ctrl1_info->ssc_i_win_v,\
						ctrl1_info->ssc_o_win_h,ctrl1_info->ssc_o_win_v,\
						ctrl1_info->pxl_rp_st,ctrl1_info->is_src_frz,ctrl1_info->pmux_pos,ctrl1_info->fw_src);
					PE_INF_M14_DBG_PRINT("\n");
					count=0;
				}
				count++;
			}
		}
		else
		{
			PE_INF_M14_DBG_PRINT("nothing to do.\n");
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
static int PE_INF_M14_GetGeneralPicInfo(LX_PE_ETC_TBL_T *pstParams)
{
    int ret = RET_OK;
	char buffer[PE_TRACE_STR_SIZE];
	static UINT32 rd_cnt=0;
	PE_WIN_HW_M14_SETTINGS_T win_set;
	PE_SHP_HW_M14_SETTINGS_T shp_set;
	PE_CCM_HW_M14_SETTINGS_T ccm_set;
	LX_PE_INF_DISPLAY_T *disp0_inf = &(_g_pe_inf_hw_m14_info.disp_info[LX_PE_WIN_0]);
	LX_PE_INF_DISPLAY_T *disp1_inf = &(_g_pe_inf_hw_m14_info.disp_info[LX_PE_WIN_1]);
	PE_INF_M14_CTRL_T *ctrl0_inf = &(_g_pe_inf_hw_m14_info.ctrl_info[LX_PE_WIN_0]);
	PE_INF_M14_CTRL_T *ctrl1_inf = &(_g_pe_inf_hw_m14_info.ctrl_info[LX_PE_WIN_1]);

#define WIN0	LX_PE_WIN_0
#define WIN1	LX_PE_WIN_1
#define PE_INF_M14_PRINT_START		"START_OF_PRINT"
#define PE_INF_M14_PRINT_RESERVED	"RESERVED"
#define PE_INF_M14_PRINT_END		"END_OF_PRINT"
#define PE_INF_M14_CASE_SPRINT(_cnt,_action,_buf,fmt,args...)	\
	case (_cnt):sprintf(_buf,fmt,##args);_action;break
#define PE_INF_M14_DFLT_SPRINT(_action,_buf,fmt,args...)		\
	default:sprintf(_buf,fmt,##args);_action;break

	do{
		CHECK_KNULL(pstParams);
		CHECK_KNULL(pstParams->data);
		if(PE_KDRV_VER_M14)
		{
			/* get win settings */
			ret = PE_WIN_HW_M14_GetCurWinSettings(&win_set);
			PE_INF_M14_CHECK_CODE(ret,break,"[%s,%d] PE_WIN_HW_M14_GetCurWinSettings() error.\n",__F__,__L__);
			/* get shp settings */
			ret = PE_SHP_HW_M14_GetCurShpSettings(&shp_set);
			PE_INF_M14_CHECK_CODE(ret,break,"[%s,%d] PE_SHP_HW_M14_GetCurShpSettings() error.\n",__F__,__L__);
			/* get ccm settings */
			ret = PE_CCM_HW_M14_GetCurCcmSettings(&ccm_set);
			PE_INF_M14_CHECK_CODE(ret,break,"[%s,%d] PE_CCM_HW_M14_GetCurCcmSettings() error.\n",__F__,__L__);
			switch(rd_cnt)
			{
				PE_INF_M14_CASE_SPRINT( 0,rd_cnt++,buffer,"%s",PE_INF_M14_PRINT_START);
				PE_INF_M14_CASE_SPRINT( 1,rd_cnt++,buffer,\
					"---------------------------------------------------------------------------\n");
				PE_INF_M14_CASE_SPRINT( 2,rd_cnt++,buffer,\
					"src       : 0~6:dtv,atv,cvbs,scart,comp,rgbpc,hdmi\n");
				PE_INF_M14_CASE_SPRINT( 3,rd_cnt++,buffer,\
					"fmt/fr    : 0~2:sd,hd,uhd / 0~1:60hz,50hz\n");
				PE_INF_M14_CASE_SPRINT( 4,rd_cnt++,buffer,\
					"scan/cstd : 0~1:i,p / 0~2:ntsc,pal,secam\n");
				PE_INF_M14_CASE_SPRINT( 5,rd_cnt++,buffer,\
					"hdmi/scart: 0~1:tv,pc / 0~1:av,rgb\n");
				PE_INF_M14_CASE_SPRINT( 6,rd_cnt++,buffer,\
					"dtv       : 0:normal,1:file,2:hdd,3:photo,4:tp,5:camera,6:invalid\n");
				PE_INF_M14_CASE_SPRINT( 7,rd_cnt++,buffer,\
					"hdd       : 0:dtv,1:atv60,2:atv50,3:av60,4:av50,5:scartrgb,6:invalid\n");
				PE_INF_M14_CASE_SPRINT( 8,rd_cnt++,buffer,\
					"out       : 0~3:2d,3_2d,2_3d,ud, 4~6:tb,ss,fs, 7~9:dual_t,s,f, 10:pip\n");
				PE_INF_M14_CASE_SPRINT( 9,rd_cnt++,buffer,\
					"in        : 0tb,ss,q,cb,fp,5fi,fa,fs,la,9ssf,dua,ca,lah\n");
				PE_INF_M14_CASE_SPRINT(10,rd_cnt++,buffer,\
					"---------------------------------------------------------------------------\n");
				PE_INF_M14_CASE_SPRINT(11,rd_cnt++,buffer,\
					"[%02d]disp0: wid:%d,src:%d,fmt:%d (h:%d,v:%d)\n",rd_cnt,\
					disp0_inf->win_id,disp0_inf->src_type,disp0_inf->fmt_type,\
					disp0_inf->in_h_size,disp0_inf->in_v_size);
				PE_INF_M14_CASE_SPRINT(12,rd_cnt++,buffer,\
					"[%02d]disp0: fr:%d(%d),scan:%d,cstd:%d,hdmi:%d,scart:%d,dtv:%d,hdd:%d\n",rd_cnt,\
					disp0_inf->fr_type,disp0_inf->in_f_rate,disp0_inf->scan_type,disp0_inf->cstd_type,\
					disp0_inf->hdmi_type,disp0_inf->scart_type,disp0_inf->dtv_type,disp0_inf->hdd_type);
				PE_INF_M14_CASE_SPRINT(13,rd_cnt++,buffer,\
					"[%02d]disp0: out:%d,in:%d,user_o:%d,user_i:%d\n",rd_cnt,\
					disp0_inf->out_type,disp0_inf->in_type,disp0_inf->user_o_type,disp0_inf->user_i_type);
				PE_INF_M14_CASE_SPRINT(14,rd_cnt++,buffer,\
					"[%02d]disp0: in_win:%d,%d,%d,%d,out_win:%d,%d,%d,%d,af:%d\n",rd_cnt,\
					disp0_inf->in_win.x_ofst,disp0_inf->in_win.y_ofst,disp0_inf->in_win.h_size,\
					disp0_inf->in_win.v_size,disp0_inf->out_win.x_ofst,disp0_inf->out_win.y_ofst,\
					disp0_inf->out_win.h_size,disp0_inf->out_win.v_size,disp0_inf->act_fmt);
				PE_INF_M14_CASE_SPRINT(15,rd_cnt++,buffer,\
					"[%02d]disp0: tp,venc,adp,ext,wog,rev,oled:%d,%d,%d,%d,%d,%d,%d\n",rd_cnt,\
					disp0_inf->mode.is_tp,disp0_inf->mode.is_venc,disp0_inf->mode.is_adpt_st,\
					disp0_inf->mode.is_ext_frc,disp0_inf->mode.is_wb_wog,\
					disp0_inf->mode.is_reverse,disp0_inf->mode.is_oled);
				PE_INF_M14_CASE_SPRINT(16,rd_cnt++,buffer,\
					"[%02d]disp1: wid:%d,src:%d,fmt:%d (h:%d,v:%d)\n",rd_cnt,\
					disp1_inf->win_id,disp1_inf->src_type,disp1_inf->fmt_type,\
					disp1_inf->in_h_size,disp1_inf->in_v_size);
				PE_INF_M14_CASE_SPRINT(17,rd_cnt++,buffer,\
					"[%02d]disp1: fr:%d(%d),scan:%d,cstd:%d,hdmi:%d,scart:%d,dtv:%d,hdd:%d\n",rd_cnt,\
					disp1_inf->fr_type,disp1_inf->in_f_rate,disp1_inf->scan_type,disp1_inf->cstd_type,\
					disp1_inf->hdmi_type,disp1_inf->scart_type,disp1_inf->dtv_type,disp1_inf->hdd_type);
				PE_INF_M14_CASE_SPRINT(18,rd_cnt++,buffer,\
					"[%02d]disp1: out:%d,in:%d,user_o:%d,user_i:%d\n",rd_cnt,\
					disp1_inf->out_type,disp1_inf->in_type,disp1_inf->user_o_type,disp1_inf->user_i_type);
				PE_INF_M14_CASE_SPRINT(19,rd_cnt++,buffer,\
					"[%02d]disp1: in_win:%d,%d,%d,%d,out_win:%d,%d,%d,%d,af:%d\n",rd_cnt,\
					disp1_inf->in_win.x_ofst,disp1_inf->in_win.y_ofst,disp1_inf->in_win.h_size,\
					disp1_inf->in_win.v_size,disp1_inf->out_win.x_ofst,disp1_inf->out_win.y_ofst,\
					disp1_inf->out_win.h_size,disp1_inf->out_win.v_size,disp1_inf->act_fmt);
				PE_INF_M14_CASE_SPRINT(20,rd_cnt++,buffer,\
					"[%02d]disp1: tp,venc,adp,ext,wog,rev,oled:%d,%d,%d,%d,%d,%d,%d\n",rd_cnt,\
					disp1_inf->mode.is_tp,disp1_inf->mode.is_venc,disp1_inf->mode.is_adpt_st,\
					disp1_inf->mode.is_ext_frc,disp1_inf->mode.is_wb_wog,\
					disp1_inf->mode.is_reverse,disp1_inf->mode.is_oled);
				PE_INF_M14_CASE_SPRINT(21,rd_cnt++,buffer,\
					"---------------------------------------------------------------------------\n");
				PE_INF_M14_CASE_SPRINT(22,rd_cnt++,buffer,\
					"det_3df: 0:2d,1:cb,2,cc,3:ll,4:ss,5:tb,6:unknown,7:max\n");
				PE_INF_M14_CASE_SPRINT(23,rd_cnt++,buffer,\
					"---------------------------------------------------------------------------\n");
				PE_INF_M14_CASE_SPRINT(24,rd_cnt++,buffer,"[%02d]ctrl0:\n",rd_cnt);
				PE_INF_M14_CASE_SPRINT(25,rd_cnt++,buffer,\
					"[%02d]pe0_src_h,v:%d,%d,scl_src_h,v:%d,%d,resize:%d,scl_hd:%d\n",rd_cnt,\
					ctrl0_inf->pe0_src_h,ctrl0_inf->pe0_src_v,ctrl0_inf->scl_src_h,\
					ctrl0_inf->scl_src_v,ctrl0_inf->scl_resize,ctrl0_inf->scl_hd);
				PE_INF_M14_CASE_SPRINT(26,rd_cnt++,buffer,\
					"[%02d]i_win:%d,%d,%d,%d,o_win:%d,%d,m_win:%d,%d,%d,%d\n",rd_cnt,\
					ctrl0_inf->i_win_h_ofst,ctrl0_inf->i_win_v_ofst,ctrl0_inf->i_win_h_size,\
					ctrl0_inf->i_win_v_size,ctrl0_inf->o_win_h_size,ctrl0_inf->o_win_v_size,\
					ctrl0_inf->m_win_h_ofst,ctrl0_inf->m_win_v_ofst,ctrl0_inf->m_win_h_size,\
					ctrl0_inf->m_win_v_size);
				PE_INF_M14_CASE_SPRINT(27,rd_cnt++,buffer,\
					"[%02d]is_yuv444:%d,det_3df:%d,auto_en:%d,s_state:%d,mute_on:%d,hv_same:%d\n",rd_cnt,\
					ctrl0_inf->is_yuv444,ctrl0_inf->det_3df,ctrl0_inf->auto_cr_en,\
					ctrl0_inf->sat_status,ctrl0_inf->mute_on,ctrl0_inf->hv_same);
				PE_INF_M14_CASE_SPRINT(28,rd_cnt++,buffer,\
					"[%02d]ssc_src,i,o_win:%d,%d, %d,%d,%d,%d, %d,%d, pxl_rp,s_frz,pmux,f_s:%d,%d,%d,%d\n",rd_cnt,\
					ctrl0_inf->ssc_src_h,ctrl0_inf->ssc_src_v,ctrl0_inf->ssc_i_win_x,ctrl0_inf->ssc_i_win_y,\
					ctrl0_inf->ssc_i_win_h,ctrl0_inf->ssc_i_win_v,ctrl0_inf->ssc_o_win_h,\
					ctrl0_inf->ssc_o_win_v,ctrl0_inf->pxl_rp_st,ctrl0_inf->is_src_frz,\
					ctrl0_inf->pmux_pos,ctrl0_inf->fw_src);
				PE_INF_M14_CASE_SPRINT(29,rd_cnt++,buffer,"%s",PE_INF_M14_PRINT_RESERVED);
				PE_INF_M14_CASE_SPRINT(30,rd_cnt++,buffer,"%s",PE_INF_M14_PRINT_RESERVED);
				PE_INF_M14_CASE_SPRINT(31,rd_cnt++,buffer,"[%02d]ctrl1:\n",rd_cnt);
				PE_INF_M14_CASE_SPRINT(32,rd_cnt++,buffer,\
					"[%02d]pe0_src_h,v:%d,%d,scl_src_h,v:%d,%d,resize:%d,scl_hd:%d\n",rd_cnt,\
					ctrl1_inf->pe0_src_h,ctrl1_inf->pe0_src_v,ctrl1_inf->scl_src_h,\
					ctrl1_inf->scl_src_v,ctrl1_inf->scl_resize,ctrl1_inf->scl_hd);
				PE_INF_M14_CASE_SPRINT(33,rd_cnt++,buffer,\
					"[%02d]i_win:%d,%d,%d,%d,o_win:%d,%d,m_win:%d,%d,%d,%d\n",rd_cnt,\
					ctrl1_inf->i_win_h_ofst,ctrl1_inf->i_win_v_ofst,ctrl1_inf->i_win_h_size,\
					ctrl1_inf->i_win_v_size,ctrl1_inf->o_win_h_size,ctrl1_inf->o_win_v_size,\
					ctrl1_inf->m_win_h_ofst,ctrl1_inf->m_win_v_ofst,ctrl1_inf->m_win_h_size,\
					ctrl1_inf->m_win_v_size);
				PE_INF_M14_CASE_SPRINT(34,rd_cnt++,buffer,\
					"[%02d]is_yuv444:%d,det_3df:%d,auto_en:%d,s_state:%d,mute_on:%d,hv_same:%d\n",rd_cnt,\
					ctrl1_inf->is_yuv444,ctrl1_inf->det_3df,ctrl1_inf->auto_cr_en,\
					ctrl1_inf->sat_status,ctrl1_inf->mute_on,ctrl1_inf->hv_same);
				PE_INF_M14_CASE_SPRINT(35,rd_cnt++,buffer,\
					"[%02d]ssc_src,i,o_win:%d,%d, %d,%d,%d,%d, %d,%d, pxl_rp,s_frz,pmux,f_s:%d,%d,%d,%d\n",rd_cnt,\
					ctrl1_inf->ssc_src_h,ctrl1_inf->ssc_src_v,ctrl1_inf->ssc_i_win_x,ctrl1_inf->ssc_i_win_y,\
					ctrl1_inf->ssc_i_win_h,ctrl1_inf->ssc_i_win_v,ctrl1_inf->ssc_o_win_h,\
					ctrl1_inf->ssc_o_win_v,ctrl1_inf->pxl_rp_st,ctrl1_inf->is_src_frz,\
					ctrl1_inf->pmux_pos,ctrl1_inf->fw_src);
				PE_INF_M14_CASE_SPRINT(36,rd_cnt++,buffer,"%s",PE_INF_M14_PRINT_RESERVED);
				PE_INF_M14_CASE_SPRINT(37,rd_cnt++,buffer,"%s",PE_INF_M14_PRINT_RESERVED);
				PE_INF_M14_CASE_SPRINT(38,rd_cnt++,buffer,"%s",PE_INF_M14_PRINT_RESERVED);
				PE_INF_M14_CASE_SPRINT(39,rd_cnt++,buffer,"%s",PE_INF_M14_PRINT_RESERVED);
				PE_INF_M14_CASE_SPRINT(40,rd_cnt++,buffer,"%s",PE_INF_M14_PRINT_RESERVED);
				PE_INF_M14_CASE_SPRINT(41,rd_cnt++,buffer,"[%02d]win:\n",rd_cnt);
				PE_INF_M14_CASE_SPRINT(42,rd_cnt++,buffer,\
					"[%02d]act0:%d,%d,%d,%d, act1:%d,%d,%d,%d\n",rd_cnt,\
					win_set.win_inf.act0_x0,win_set.win_inf.act0_y0,win_set.win_inf.act0_x1,\
					win_set.win_inf.act0_y1,win_set.win_inf.act1_x0,win_set.win_inf.act1_y0,\
					win_set.win_inf.act1_x1,win_set.win_inf.act1_y1);
				PE_INF_M14_CASE_SPRINT(43,rd_cnt++,buffer,\
					"[%02d]win0:%d,%d,%d,%d, win1:%d,%d,%d,%d\n",rd_cnt,\
					win_set.win_inf.win0_x0,win_set.win_inf.win0_y0,win_set.win_inf.win0_x1,\
					win_set.win_inf.win0_y1,win_set.win_inf.win1_x0,win_set.win_inf.win1_y0,\
					win_set.win_inf.win1_x1,win_set.win_inf.win1_y1);
				PE_INF_M14_CASE_SPRINT(44,rd_cnt++,buffer,\
					"[%02d]h,v_max:%d,%d, w0,w1,w01_en:[%d,%d],[%d,%d],[%d,%d]\n",rd_cnt,\
					win_set.win_inf.h_max,win_set.win_inf.v_max,win_set.win_inf.win0_en[0],\
					win_set.win_inf.win0_en[1],win_set.win_inf.win0_en[0],win_set.win_inf.win0_en[1],\
					win_set.win_inf.win01_en[0],win_set.win_inf.win01_en[1]);
				PE_INF_M14_CASE_SPRINT(45,rd_cnt++,buffer,\
					"[%02d]lrcr histo_l,r, apply_r : %d,%d,%d,%d, %d,%d,%d,%d, %d,%d,%d,%d\n",rd_cnt,\
					win_set.lrcr_inf.histo_l_x0,win_set.lrcr_inf.histo_l_y0,win_set.lrcr_inf.histo_l_x1,\
					win_set.lrcr_inf.histo_l_y1,win_set.lrcr_inf.histo_r_x0,win_set.lrcr_inf.histo_r_y0,\
					win_set.lrcr_inf.histo_r_x1,win_set.lrcr_inf.histo_r_y1,win_set.lrcr_inf.apply_r_x0,\
					win_set.lrcr_inf.apply_r_y0,win_set.lrcr_inf.apply_r_x1,win_set.lrcr_inf.apply_r_y1);
				PE_INF_M14_CASE_SPRINT(46,rd_cnt++,buffer,\
					"[%02d]win: tnr win h,v size0:%d,%d, size1:%d,%d\n",rd_cnt,\
					win_set.tnr_inf.h_size0,win_set.tnr_inf.v_size0,\
					win_set.tnr_inf.h_size1,win_set.tnr_inf.v_size1);
				PE_INF_M14_CASE_SPRINT(47,rd_cnt++,buffer,"%s",PE_INF_M14_PRINT_RESERVED);
				PE_INF_M14_CASE_SPRINT(48,rd_cnt++,buffer,"%s",PE_INF_M14_PRINT_RESERVED);
				PE_INF_M14_CASE_SPRINT(49,rd_cnt++,buffer,"%s",PE_INF_M14_PRINT_RESERVED);
				PE_INF_M14_CASE_SPRINT(50,rd_cnt++,buffer,"%s",PE_INF_M14_PRINT_RESERVED);
				PE_INF_M14_CASE_SPRINT(51,rd_cnt++,buffer,"[%02d]shp:\n",rd_cnt);
				PE_INF_M14_CASE_SPRINT(52,rd_cnt++,buffer,\
					"[%02d]cur_scl_idx[0] : wid:%d, luma h,v:%d,%d, chrm h,v:%d,%d\n",rd_cnt,\
					shp_set.cur_scl_idx[WIN0].win_id,shp_set.cur_scl_idx[WIN0].h_luma_id,\
					shp_set.cur_scl_idx[WIN0].v_luma_id,shp_set.cur_scl_idx[WIN0].h_chrm_id,\
					shp_set.cur_scl_idx[WIN0].v_chrm_id);
				PE_INF_M14_CASE_SPRINT(53,rd_cnt++,buffer,\
					"[%02d]cur_scl_idx[1] : wid:%d, luma h,v:%d,%d, chrm h,v:%d,%d\n",rd_cnt,\
					shp_set.cur_scl_idx[WIN1].win_id,shp_set.cur_scl_idx[WIN1].h_luma_id,\
					shp_set.cur_scl_idx[WIN1].v_luma_id,shp_set.cur_scl_idx[WIN1].h_chrm_id,\
					shp_set.cur_scl_idx[WIN1].v_chrm_id);
				PE_INF_M14_CASE_SPRINT(54,rd_cnt++,buffer,\
					"[%02d]pre_scl_idx[0] : wid:%d, luma h,v:%d,%d, chrm h,v:%d,%d\n",rd_cnt,\
					shp_set.pre_scl_idx[WIN0].win_id,shp_set.pre_scl_idx[WIN0].h_luma_id,\
					shp_set.pre_scl_idx[WIN0].v_luma_id,shp_set.pre_scl_idx[WIN0].h_chrm_id,\
					shp_set.pre_scl_idx[WIN0].v_chrm_id);
				PE_INF_M14_CASE_SPRINT(55,rd_cnt++,buffer,\
					"[%02d]pre_scl_idx[1] : wid:%d, luma h,v:%d,%d, chrm h,v:%d,%d\n",rd_cnt,\
					shp_set.pre_scl_idx[WIN1].win_id,shp_set.pre_scl_idx[WIN1].h_luma_id,\
					shp_set.pre_scl_idx[WIN1].v_luma_id,shp_set.pre_scl_idx[WIN1].h_chrm_id,\
					shp_set.pre_scl_idx[WIN1].v_chrm_id);
				PE_INF_M14_CASE_SPRINT(56,rd_cnt++,buffer,\
					"[%02d]h_y_tap:%d,%d, h_c_tap:%d,%d, v_y_tap:%d,%d, v_c_tap:%d,%d\n",rd_cnt,\
					shp_set.h_y_tap[WIN0],shp_set.h_y_tap[WIN1],shp_set.h_c_tap[WIN0],\
					shp_set.h_c_tap[WIN1],shp_set.v_y_tap[WIN0],shp_set.v_y_tap[WIN1],\
					shp_set.v_c_tap[WIN0],shp_set.v_c_tap[WIN1]);
				PE_INF_M14_CASE_SPRINT(57,rd_cnt++,buffer,\
					"[%02d]y_fir:%d,%d, c_fir:%d,%d, pre_res_on:%d,%d\n",rd_cnt,\
					shp_set.y_fir[WIN0],shp_set.y_fir[WIN1],shp_set.c_fir[WIN0],\
					shp_set.c_fir[WIN1],shp_set.pre_res_en[WIN0],shp_set.pre_res_en[WIN1]);
				PE_INF_M14_CASE_SPRINT(58,rd_cnt++,buffer,"%s",PE_INF_M14_PRINT_RESERVED);
				PE_INF_M14_CASE_SPRINT(59,rd_cnt++,buffer,"%s",PE_INF_M14_PRINT_RESERVED);
				PE_INF_M14_CASE_SPRINT(60,rd_cnt++,buffer,"%s",PE_INF_M14_PRINT_RESERVED);
				PE_INF_M14_CASE_SPRINT(61,rd_cnt++,buffer,"[%02d]ccm:\n",rd_cnt);
				PE_INF_M14_CASE_SPRINT(62,rd_cnt++,buffer,\
					"[%02d]lrcr(a0) sel : r,l_h:%d,%d, r,l_p:%d,%d, r,l_o:%d,%d\n",rd_cnt,\
					ccm_set.ccm_inf.r_h_select,ccm_set.ccm_inf.l_h_select,ccm_set.ccm_inf.r_p_select,\
					ccm_set.ccm_inf.l_p_select,ccm_set.ccm_inf.r_o_select,ccm_set.ccm_inf.l_o_select);
				PE_INF_M14_CASE_SPRINT(63,rd_cnt++,buffer,\
					"[%02d]lrcr(a0) r,l_bypass:%d,%d\n",rd_cnt,\
					ccm_set.ccm_inf.ppr_bypass,ccm_set.ccm_inf.ppl_bypass);
				PE_INF_M14_CASE_SPRINT(64,rd_cnt++,buffer,\
					"[%02d]lrcr ff_md:%d, ll_md:%d, h_md:%d, gain:%d, lut:%d\n",rd_cnt,\
					ccm_set.ccm_inf.ff_mode_on,ccm_set.ccm_inf.ll_mode_en,ccm_set.ccm_inf.histo_mode,\
					ccm_set.ccm_inf.lrcr_gain,ccm_set.ccm_inf.lut_type);
				PE_INF_M14_CASE_SPRINT(65,rd_cnt++,buffer,\
					"[%02d]acr en:%d, max_hist:%d,val_bins:%d, th0,1:%d,%d\n",rd_cnt,\
					ccm_set.auto_cr.enable,ccm_set.auto_cr.th_max_hist,ccm_set.auto_cr.th_valid_bins,\
					ccm_set.auto_cr.adj_th0,ccm_set.auto_cr.adj_th1);
				PE_INF_M14_CASE_SPRINT(66,rd_cnt++,buffer,"%s",PE_INF_M14_PRINT_RESERVED);
				PE_INF_M14_CASE_SPRINT(67,rd_cnt++,buffer,"%s",PE_INF_M14_PRINT_RESERVED);
				PE_INF_M14_CASE_SPRINT(68,rd_cnt++,buffer,"%s",PE_INF_M14_PRINT_RESERVED);
				PE_INF_M14_CASE_SPRINT(69,rd_cnt++,buffer,"%s",PE_INF_M14_PRINT_RESERVED);
				PE_INF_M14_CASE_SPRINT(70,rd_cnt++,buffer,"%s",PE_INF_M14_PRINT_RESERVED);
				PE_INF_M14_CASE_SPRINT(71,rd_cnt++,buffer,\
					"---------------------------------------------------------------------------\n");
				PE_INF_M14_CASE_SPRINT(72,rd_cnt++,buffer,\
					"- kdrv ver. : %s\n", PE_INF_M14_DRV_VER);
				PE_INF_M14_CASE_SPRINT(73,rd_cnt++,buffer,\
					"---------------------------------------------------------------------------\n");
				PE_INF_M14_DFLT_SPRINT(rd_cnt=0,buffer,"%s",PE_INF_M14_PRINT_END);
			}
			memcpy(pstParams->data,buffer,sizeof(char)*PE_TRACE_STR_SIZE);
		}
		else
		{
			PE_INF_M14_DBG_PRINT("nothing to do.\n");
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
static int PE_INF_M14_SetPathMonitor(LX_PE_ETC_TBL_T *pstParams)
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
		if(PE_KDRV_VER_M14BX)
		{
			w_data[0] = pstParams->data[1];
			switch (pstParams->data[0])
			{
				case PE_INF_HW_M14B_M_CVA_TPG___TYPE:
					PE_CVA_M14B0_QWr01(cva_misc_ctrl,pattern_type,w_data[0]);
					break;
				case PE_INF_HW_M14B_M_CVA_TPG__C_S_C:
					PE_CVA_M14B0_QWr01(cva_misc_ctrl,pattern_csc,w_data[0]);
					break;
				case PE_INF_HW_M14B_M_CVA_TPG_DETAIL:
					PE_CVA_M14B0_QWr01(cva_misc_ctrl,pattern_detail,w_data[0]);
					break;
				case PE_INF_HW_M14B_M_CVB_TPG___TYPE:
					PE_CVB_M14B0_QWr01(cvb_misc_ctrl,pattern_type,w_data[0]);
					break;
				case PE_INF_HW_M14B_M_CVB_TPG__C_S_C:
					PE_CVB_M14B0_QWr01(cvb_misc_ctrl,pattern_csc,w_data[0]);
					break;
				case PE_INF_HW_M14B_M_CVB_TPG_DETAIL:
					PE_CVB_M14B0_QWr01(cvb_misc_ctrl,pattern_detail,w_data[0]);
					break;
				case PE_INF_HW_M14B_M_P0M_DETOUR_ENB:
					PE_P0M_M14B0_QWr01(p0m_pe0_opmode,detour_enable,w_data[0]);
					break;
				case PE_INF_HW_M14B_M_P0M_TNR_ENABLE:
					PE_P0M_M14B0_QWr01(p0m_tnr_ctrl_00,tnr_enable,w_data[0]);
					break;
				case PE_INF_HW_M14B_M_P0M_IPC_Y_MODE:
					PE_P0M_M14B0_QWr01(p0m_ipc_ctrl_00,cr_luma_mode,w_data[0]);
					break;
				case PE_INF_HW_M14B_M_P0M_IPC_C_MODE:
					PE_P0M_M14B0_QWr01(p0m_ipc_ctrl_00,cr_chroma_mode,w_data[0]);
					break;
				case PE_INF_HW_M14B_M_P0M_FMD_ENABLE:
					PE_P0M_M14B0_QWr01(p0m_ipc_ctrl_00,cr_film_mode_enable,w_data[0]);
					break;
				case PE_INF_HW_M14B_M_P0M_CLC_FLT_EN:
					PE_P0M_M14B0_QWr01(p0m_clc_ctrl_00,cl_filter_enable,w_data[0]);
					break;
				case PE_INF_HW_M14B_M_P0D_DETOUR_ENB:
					PE_P0D_M14B0_QWr01(p0d_pe0_opmode,detour_enable,w_data[0]);
					break;
				case PE_INF_HW_M14B_M_P0D_VFILTER_EN:
					PE_P0D_M14B0_QWr01(p0d_pe0_opmode,detour_enable,w_data[0]);
					break;
				case PE_INF_HW_M14B_M_P0D_CTI_ENABLE:
					PE_P0D_M14B0_QWr01(p0d_cti_ctrl_0,cti_en,w_data[0]);
					break;
				case PE_INF_HW_M14B_M_P0D_DNR_MAX_EN:
					PE_P0D_M14B0_QWr01(p0d_dnr_max_ctrl,reg_dnr_max_enable,w_data[0]);
					break;
				case PE_INF_HW_M14B_M_P0D_IFC_MAX_EN:
					PE_P0D_M14B0_QWr01(p0d_dnr_max_ctrl,reg_ifc_max_enable,w_data[0]);
					break;
				case PE_INF_HW_M14B_M_P0D_DNR_IFC_SL:
					PE_P0D_M14B0_QWr01(p0d_dnr_max_ctrl,reg_dnr_ifc_sel,w_data[0]);
					break;
				case PE_INF_HW_M14B_M_P0S_DETOUR_ENB:
					PE_P0S_M14B0_QWr01(p0s_pe0_opmode,detour_enable,w_data[0]);
					break;
				case PE_INF_HW_M14B_M_P0S_IPC_Y_MODE:
					PE_P0S_M14B0_QWr01(p0s_ipc_ctrl_00,cr_luma_mode,w_data[0]);
					break;
				case PE_INF_HW_M14B_M_P0S_IPC_C_MODE:
					PE_P0S_M14B0_QWr01(p0s_ipc_ctrl_00,cr_chroma_mode,w_data[0]);
					break;
				case PE_INF_HW_M14B_M_P0S_FMD_ENABLE:
					PE_P0S_M14B0_QWr01(p0s_ipc_ctrl_00,cr_film_mode_enable,w_data[0]);
					break;
				case PE_INF_HW_M14B_M_MSC_FILT_SEL_Y:
					PE_MSC_M14B0_QWr01(msc_ctrl,filter_type_sel_luma,w_data[0]);
					break;
				case PE_INF_HW_M14B_M_MSC_FILT_SEL_C:
					PE_MSC_M14B0_QWr01(msc_ctrl,filter_type_sel_chroma,w_data[0]);
					break;
				case PE_INF_HW_M14B_M_MSC_SAMPL_MODE:
					PE_MSC_M14B0_QWr01(msc_ctrl,manual_sampling_mode,w_data[0]);
					break;
				case PE_INF_HW_M14B_M_SSC_FILT_SEL_Y:
					PE_SSC_M14B0_QWr01(ssc_ctrl,filter_type_sel,w_data[0]);
					break;
				#ifdef PE_HW_M14_USE_M14B_WCP_REG
				case PE_INF_HW_M14B_M_OVL_DARK_W0_EN:
					PE_OVL_M14B0_QWr01(ovl_ov_ctrl4,w0_dark_en,w_data[0]);
					break;
				case PE_INF_HW_M14B_M_OVL_DARK_W1_EN:
					PE_OVL_M14B0_QWr01(ovl_ov_ctrl4,w1_dark_en,w_data[0]);
					break;
				case PE_INF_HW_M14B_M_OVL_DARK_COLOR:
					sel = (w_data[0]<6)? w_data[0]:5;
					w_data[0] = yuv08_data[sel][0];	// y
					w_data[1] = yuv08_data[sel][1];	// cb
					w_data[2] = yuv08_data[sel][2];	// cr
					PE_OVL_M14B0_QWr01(ovl_ov_ctrl2,dark_color_yy,w_data[0]);
					PE_OVL_M14B0_QWr01(ovl_ov_ctrl2,dark_color_cb,w_data[1]);
					PE_OVL_M14B0_QWr01(ovl_ov_ctrl2,dark_color_cr,w_data[2]);
					break;
				case PE_INF_HW_M14B_M_OVL_BACK_COLOR:
					sel = (w_data[0]<6)? w_data[0]:5;
					w_data[0] = yuv08_data[sel][0];	// y
					w_data[1] = yuv08_data[sel][1];	// cb
					w_data[2] = yuv08_data[sel][2];	// cr
					PE_OVL_M14B0_QWr01(ovl_ov_ctrl2,back_color_yy,w_data[0]);
					PE_OVL_M14B0_QWr01(ovl_ov_ctrl2,back_color_cb,w_data[1]);
					PE_OVL_M14B0_QWr01(ovl_ov_ctrl2,back_color_cr,w_data[2]);
					break;
				case PE_INF_HW_M14B_M_OVR_DARK_W0_EN:
					PE_OVR_M14B0_QWr01(ovr_ov_ctrl4,w0_dark_en,w_data[0]);
					break;
				case PE_INF_HW_M14B_M_OVR_DARK_W1_EN:
					PE_OVR_M14B0_QWr01(ovr_ov_ctrl4,w1_dark_en,w_data[0]);
					break;
				case PE_INF_HW_M14B_M_OVR_DARK_COLOR:
					sel = (w_data[0]<6)? w_data[0]:5;
					w_data[0] = yuv08_data[sel][0];	// y
					w_data[1] = yuv08_data[sel][1];	// cb
					w_data[2] = yuv08_data[sel][2];	// cr
					PE_OVR_M14B0_QWr01(ovr_ov_ctrl2,dark_color_yy,w_data[0]);
					PE_OVR_M14B0_QWr01(ovr_ov_ctrl2,dark_color_cb,w_data[1]);
					PE_OVR_M14B0_QWr01(ovr_ov_ctrl2,dark_color_cr,w_data[2]);
					break;
				case PE_INF_HW_M14B_M_OVR_BACK_COLOR:
					sel = (w_data[0]<6)? w_data[0]:5;
					w_data[0] = yuv08_data[sel][0];	// y
					w_data[1] = yuv08_data[sel][1];	// cb
					w_data[2] = yuv08_data[sel][2];	// cr
					PE_OVR_M14B0_QWr01(ovr_ov_ctrl2,back_color_yy,w_data[0]);
					PE_OVR_M14B0_QWr01(ovr_ov_ctrl2,back_color_cb,w_data[1]);
					PE_OVR_M14B0_QWr01(ovr_ov_ctrl2,back_color_cr,w_data[2]);
					break;
				#endif
				case PE_INF_HW_M14B_M_PE1_DETOUR_ENB:
					PE_PE1_M14B0_QWr01(pe1_top_ctrl_02,pe1_detour,w_data[0]);
					break;
				case PE_INF_HW_M14B_M_PE1_WIN1_W0_EN:
					PE_PE1_M14B0_QWr01(pe1_win1_ctrl_00,win0_en,w_data[0]);
					break;
				case PE_INF_HW_M14B_M_PE1_WIN1_W1_EN:
					PE_PE1_M14B0_QWr01(pe1_win1_ctrl_00,win1_en,w_data[0]);
					break;
				case PE_INF_HW_M14B_M_PE1_WIN1_01_EN:
					PE_PE1_M14B0_QWr01(pe1_win1_ctrl_00,win01_en,w_data[0]);
					break;
				case PE_INF_HW_M14B_M_PE1_WIN2_W0_EN:
					PE_PE1_M14B0_QWr01(pe1_win2_ctrl_00,win0_en,w_data[0]);
					break;
				case PE_INF_HW_M14B_M_PE1_WIN2_W1_EN:
					PE_PE1_M14B0_QWr01(pe1_win2_ctrl_00,win1_en,w_data[0]);
					break;
				case PE_INF_HW_M14B_M_PE1_WIN2_01_EN:
					PE_PE1_M14B0_QWr01(pe1_win2_ctrl_00,win01_en,w_data[0]);
					break;
				case PE_INF_HW_M14B_M_PE1_WIN3_W0_EN:
					PE_PE1_M14B0_QWr01(pe1_win3_ctrl_00,win0_en,w_data[0]);
					break;
				case PE_INF_HW_M14B_M_PE1_WIN3_W1_EN:
					PE_PE1_M14B0_QWr01(pe1_win3_ctrl_00,win1_en,w_data[0]);
					break;
				case PE_INF_HW_M14B_M_PE1_WIN3_01_EN:
					PE_PE1_M14B0_QWr01(pe1_win3_ctrl_00,win01_en,w_data[0]);
					break;
				case PE_INF_HW_M14B_M_PE1_PMUX3D_ENB:
					PE_PE1_M14B0_QWr01(pe1_pmux_ctrl_00,mux3d_en,w_data[0]);
					break;
				case PE_INF_HW_M14B_M_PE1_PMUXOUT_SL:
					PE_PE1_M14B0_QWr01(pe1_pmux_ctrl_00,reg_out_mux,w_data[0]);
					break;
				case PE_INF_HW_M14B_M_PE1_DR_H_Y_ENB:
					PE_PE1_M14B0_QWr01(pe1_derh_ctrl_06,edge_y_filter_en,w_data[0]);
					break;
				case PE_INF_HW_M14B_M_PE1_DR_H_C_ENB:
					PE_PE1_M14B0_QWr01(pe1_derh_ctrl_06,edge_c_filter_en,w_data[0]);
					break;
				case PE_INF_HW_M14B_M_PE1_DR_V__ENAB:
					PE_PE1_M14B0_QWr01(pe1_derv_ctrl_0,der_v_en,w_data[0]);
					break;
				case PE_INF_HW_M14B_M_PE1_MP__ENABLE:
					PE_PE1_M14B0_QWr01(pe1_mp_ctrl_00,edge_enhance_enable,w_data[0]);
					break;
				case PE_INF_HW_M14B_M_PE1_SP__ENABLE:
					PE_PE1_M14B0_QWr01(pe1_sp_ctrl_00,edge_enhance_enable,w_data[0]);
					break;
				case PE_INF_HW_M14B_M_PE1_SR_OUT_MUX:
					PE_PE1_M14B0_QWr01(pe1_sre_ctrl_00,reg_out_mux_sel,w_data[0]);
					break;
				case PE_INF_HW_M14B_M_PE1_TI__ENABLE:
					PE_PE1_M14B0_QWr01(pe1_ti_ctrl_0,enable,w_data[0]);
					break;
				case PE_INF_HW_M14B_M_PE1_DJ__ENABLE:
					PE_PE1_M14B0_QWr01(pe1_dj_ctrl_00,edf_en,w_data[0]);
					break;
				case PE_INF_HW_M14B_M_PE1_CTI_ENABLE:
					PE_PE1_M14B0_QWr01(pe1_cti_ctrl_00,cti_en,w_data[0]);
					break;
				case PE_INF_HW_M14B_M_PE1_YCM_ENABLE:
					PE_PE1_M14B0_QWr01(pe1_cti_ctrl_02,ycm_en1,w_data[0]);
					break;
				case PE_INF_HW_M14B_M_PE1_DCE_ENABLE:
					PE_PE1_M14B0_QWr01(pe1_dce_ctrl_00,dynamic_contrast_en,w_data[0]);
					break;
				case PE_INF_HW_M14B_M_PE1_LR_CR_ENAB:
					PE_PE1_M14B0_QWr01(pe1_lrcr_ctrl_00,enable,w_data[0]);
					break;
				case PE_INF_HW_M14B_M_PE1_CEN_DETOUR:
					PE_PE1_M14B0_QWr01(pe1_cen_ctrl_00,cen_detour,w_data[0]);
					break;
				case PE_INF_HW_M14B_M_PE1_DSE_ENABLE:
					PE_PE1_M14B0_QWr01(pe1_dse_ctrl_00,dynamic_saturation_en,w_data[0]);
					break;
				case PE_INF_HW_M14B_M_PE1_DRC_ENABLE:
					PE_PE1_M14B0_QWr01(pe1_drc_ctrl_00,reg_drc_en,w_data[0]);
					break;
				case PE_INF_HW_M14B_M_PE1_CW__ENABLE:
					PE_PE1_M14B0_QWr01(pe1_cw_ctrl_00,cw_en,w_data[0]);
					break;
				case PE_INF_HW_M14B_M_PE1_CONV_L_ENB:
					PE_PE1_M14B0_QWr01(pe1_conv_ctrl_03,pxl_rep_in,w_data[0]);
					break;
				case PE_INF_HW_M14B_M_PE1_CONV_L_COL:
					sel = (w_data[0]<6)? w_data[0]:5;
					w_data[0] = rgb10_data[sel][0];	// r
					w_data[1] = rgb10_data[sel][1];	// g
					w_data[2] = rgb10_data[sel][2];	// b
					PE_PE1_M14B0_QWr01(pe1_conv_ctrl_05,pxl_rep_value_r,w_data[0]);
					PE_PE1_M14B0_QWr01(pe1_conv_ctrl_05,pxl_rep_value_g,w_data[1]);
					PE_PE1_M14B0_QWr01(pe1_conv_ctrl_05,pxl_rep_value_b,w_data[2]);
					break;
				case PE_INF_HW_M14B_M_PE1_CONV_R_ENB:
					PE_PE1_M14B0_QWr01(pe1_conv_ctrl_08,pxl_rep_in,w_data[0]);
					break;
				case PE_INF_HW_M14B_M_PE1_CONV_R_COL:
					sel = (w_data[0]<6)? w_data[0]:5;
					w_data[0] = rgb10_data[sel][0];	// r
					w_data[1] = rgb10_data[sel][1];	// g
					w_data[2] = rgb10_data[sel][2];	// b
					PE_PE1_M14B0_QWr01(pe1_conv_ctrl_0a,pxl_rep_value_r,w_data[0]);
					PE_PE1_M14B0_QWr01(pe1_conv_ctrl_0a,pxl_rep_value_g,w_data[1]);
					PE_PE1_M14B0_QWr01(pe1_conv_ctrl_0a,pxl_rep_value_b,w_data[2]);
					break;

				case PE_INF_HW_M14B_M_PE1_CMUX3D_ENB:
					PE_PE1_M14B0_QWr01(pe1_conv_ctrl_0d,mux3d_en,w_data[0]);
					break;
				case PE_INF_HW_M14B_M_PE1_CMUX3D_COL:
					sel = (w_data[0]<6)? w_data[0]:5;
					w_data[0] = rgb10_data[sel][0]>>2;	// r
					w_data[1] = rgb10_data[sel][1]>>2;	// g
					w_data[2] = rgb10_data[sel][2]>>2;	// b
					PE_PE1_M14B0_QWr01(pe1_conv_ctrl_12,bg_color_r,w_data[0]);
					PE_PE1_M14B0_QWr01(pe1_conv_ctrl_12,bg_color_g,w_data[1]);
					PE_PE1_M14B0_QWr01(pe1_conv_ctrl_12,bg_color_b,w_data[2]);
					break;
				case PE_INF_HW_M14B_M_PE1_MUTE_L_ENB:
					PE_PE1_M14B0_QWr01(pe1_mute_ctrl_00,pxl_rep_in,w_data[0]);
					break;
				case PE_INF_HW_M14B_M_PE1_MUTE_L_COL:
					sel = (w_data[0]<6)? w_data[0]:5;
					w_data[0] = rgb10_data[sel][0];	// r
					w_data[1] = rgb10_data[sel][1];	// g
					w_data[2] = rgb10_data[sel][2];	// b
					PE_PE1_M14B0_QWr01(pe1_mute_ctrl_02,pxl_rep_value_r,w_data[0]);
					PE_PE1_M14B0_QWr01(pe1_mute_ctrl_02,pxl_rep_value_g,w_data[1]);
					PE_PE1_M14B0_QWr01(pe1_mute_ctrl_02,pxl_rep_value_b,w_data[2]);
					break;
				case PE_INF_HW_M14B_M_PE1_MUTE_R_ENB:
					PE_PE1_M14B0_QWr01(pe1_mute_ctrl_05,pxl_rep_in,w_data[0]);
					break;
				case PE_INF_HW_M14B_M_PE1_MUTE_R_COL:
					sel = (w_data[0]<6)? w_data[0]:5;
					w_data[0] = rgb10_data[sel][0];	// r
					w_data[1] = rgb10_data[sel][1];	// g
					w_data[2] = rgb10_data[sel][2];	// b
					PE_PE1_M14B0_QWr01(pe1_mute_ctrl_07,pxl_rep_value_r,w_data[0]);
					PE_PE1_M14B0_QWr01(pe1_mute_ctrl_07,pxl_rep_value_g,w_data[1]);
					PE_PE1_M14B0_QWr01(pe1_mute_ctrl_07,pxl_rep_value_b,w_data[2]);
					break;
				case PE_INF_HW_M14B_M_PE1_MMUX3D_ENB:
					PE_PE1_M14B0_QWr01(pe1_mute_ctrl_0a,mux3d_en,w_data[0]);
					break;
				case PE_INF_HW_M14B_M_PE1_MMUX3D_COL:
					sel = (w_data[0]<6)? w_data[0]:5;
					w_data[0] = rgb10_data[sel][0]>>2;	// r
					w_data[1] = rgb10_data[sel][1]>>2;	// g
					w_data[2] = rgb10_data[sel][2]>>2;	// b
					PE_PE1_M14B0_QWr01(pe1_mute_ctrl_0f,bg_color_r,w_data[0]);
					PE_PE1_M14B0_QWr01(pe1_mute_ctrl_0f,bg_color_g,w_data[1]);
					PE_PE1_M14B0_QWr01(pe1_mute_ctrl_0f,bg_color_b,w_data[2]);
					break;

				case PE_INF_HW_M14B_M_DISPLAY:
				default:
					break;
			}
		}
		else if(PE_KDRV_VER_M14AX)
		{
			w_data[0] = pstParams->data[1];
			switch (pstParams->data[0])
			{
				case PE_INF_HW_M14A_M_CVA_PATTERN_TYPE:
					PE_CVA_M14A0_QWr01(cva_misc_ctrl,	pattern_type,		w_data[0]);
					break;
				case PE_INF_HW_M14A_M_CVA_PATTERN_CSC:
					PE_CVA_M14A0_QWr01(cva_misc_ctrl,	pattern_csc,		w_data[0]);
					break;
				case PE_INF_HW_M14A_M_CVA_PATTERN_DETAIL:
					PE_CVA_M14A0_QWr01(cva_misc_ctrl,	pattern_detail,		w_data[0]);
					break;
				case PE_INF_HW_M14A_M_CVA_FIR_EN:
					PE_CVA_M14A0_QWr01(cva_dig_filt_ctrl3,	fir_en,			w_data[0]);
					break;
				case PE_INF_HW_M14A_M_CVA_FIR_Y_EN:
					PE_CVA_M14A0_QWr01(cva_dig_filt_ctrl3,	fir_y_en,		w_data[0]);
					break;
				case PE_INF_HW_M14A_M_CVA_FIR_C_EN:
					PE_CVA_M14A0_QWr01(cva_dig_filt_ctrl3,	fir_c_en,		w_data[0]);
					break;
				case PE_INF_HW_M14A_M_CVB_PATTERN_TYPE:
					PE_CVB_M14A0_QWr01(cvb_misc_ctrl,	pattern_type,		w_data[0]);
					break;
				case PE_INF_HW_M14A_M_CVB_PATTERN_CSC:
					PE_CVB_M14A0_QWr01(cvb_misc_ctrl,	pattern_csc,		w_data[0]);
					break;
				case PE_INF_HW_M14A_M_CVB_PATTERN_DETAIL:
					PE_CVB_M14A0_QWr01(cvb_misc_ctrl,	pattern_detail,		w_data[0]);
					break;
				case PE_INF_HW_M14A_M_CVB_FIR_EN:
					PE_CVB_M14A0_QWr01(cvb_dig_filt_ctrl3,	fir_en,			w_data[0]);
					break;
				case PE_INF_HW_M14A_M_CVB_FIR_Y_EN:
					PE_CVB_M14A0_QWr01(cvb_dig_filt_ctrl3,	fir_y_en,		w_data[0]);
					break;
				case PE_INF_HW_M14A_M_CVB_FIR_C_EN:
					PE_CVB_M14A0_QWr01(cvb_dig_filt_ctrl3,	fir_c_en,		w_data[0]);
					break;
				case PE_INF_HW_M14A_M_P0L_DETOUR:
					PE_P0L_M14A0_QWr01(p0l_pe0_opmode,		detour_enable,	w_data[0]);
					break;
				case PE_INF_HW_M14A_M_P0L_IPC_Y:
					PE_P0L_M14A0_QWr01(p0l_ipc_ctrl_00,		cr_luma_mode,	w_data[0]);
					break;
				case PE_INF_HW_M14A_M_P0L_IPC_C:
					PE_P0L_M14A0_QWr01(p0l_ipc_ctrl_00,		cr_chroma_mode,	w_data[0]);
					break;
				case PE_INF_HW_M14A_M_P0R_DETOUR:
					PE_P0R_M14A0_QWr01(p0r_pe0_opmode,		detour_enable,	w_data[0]);
					break;
				case PE_INF_HW_M14A_M_P0R_IPC_Y:
					PE_P0R_M14A0_QWr01(p0r_ipc_ctrl_00,		cr_luma_mode,	w_data[0]);
					break;
				case PE_INF_HW_M14A_M_P0R_IPC_C:
					PE_P0R_M14A0_QWr01(p0r_ipc_ctrl_00,		cr_chroma_mode,	w_data[0]);
					break;
				case PE_INF_HW_M14A_M_MSL_FLT_SEL_Y:
					PE_MSL_M14A0_QWr01(msc_ctrl_l,	filter_type_sel_luma,	w_data[0]);
					break;
				case PE_INF_HW_M14A_M_MSR_FLT_SEL_Y:
					PE_MSR_M14A0_QWr01(msc_ctrl_r,	filter_type_sel_luma,	w_data[0]);
					break;
				case PE_INF_HW_M14A_M_OVL_DARK_W0_EN:
					PE_OVL_M14A0_QWr01(ovl_ov_ctrl4,		w0_dark_en,		w_data[0]);
					break;
				case PE_INF_HW_M14A_M_OVL_DARK_W1_EN:
					PE_OVL_M14A0_QWr01(ovl_ov_ctrl4,		w1_dark_en,		w_data[0]);
					break;
				case PE_INF_HW_M14A_M_OVL_DARK_COLOR:
					sel = (w_data[0]<6)? w_data[0]:5;
					w_data[0] = yuv08_data[sel][0];	// y
					w_data[1] = yuv08_data[sel][1];	// cb
					w_data[2] = yuv08_data[sel][2];	// cr
					PE_OVL_M14A0_QWr01(ovl_ov_ctrl2,		dark_color_yy,	w_data[0]);
					PE_OVL_M14A0_QWr01(ovl_ov_ctrl2,		dark_color_cb,	w_data[1]);
					PE_OVL_M14A0_QWr01(ovl_ov_ctrl2,		dark_color_cr,	w_data[2]);
					break;
				case PE_INF_HW_M14A_M_OVL_BACK_COLOR:
					sel = (w_data[0]<6)? w_data[0]:5;
					w_data[0] = yuv08_data[sel][0];	// y
					w_data[1] = yuv08_data[sel][1];	// cb
					w_data[2] = yuv08_data[sel][2];	// cr
					PE_OVL_M14A0_QWr01(ovl_ov_ctrl2,		back_color_yy,	w_data[0]);
					PE_OVL_M14A0_QWr01(ovl_ov_ctrl2,		back_color_cb,	w_data[1]);
					PE_OVL_M14A0_QWr01(ovl_ov_ctrl2,		back_color_cr,	w_data[2]);
					break;
				case PE_INF_HW_M14A_M_OVR_DARK_W0_EN:
					PE_OVR_M14A0_QWr01(ovr_ov_ctrl4,		w0_dark_en,		w_data[0]);
					break;
				case PE_INF_HW_M14A_M_OVR_DARK_W1_EN:
					PE_OVR_M14A0_QWr01(ovr_ov_ctrl4,		w1_dark_en,		w_data[0]);
					break;
				case PE_INF_HW_M14A_M_OVR_DARK_COLOR:
					sel = (w_data[0]<6)? w_data[0]:5;
					w_data[0] = yuv08_data[sel][0];	// y
					w_data[1] = yuv08_data[sel][1];	// cb
					w_data[2] = yuv08_data[sel][2];	// cr
					PE_OVR_M14A0_QWr01(ovr_ov_ctrl2,		dark_color_yy,	w_data[0]);
					PE_OVR_M14A0_QWr01(ovr_ov_ctrl2,		dark_color_cb,	w_data[1]);
					PE_OVR_M14A0_QWr01(ovr_ov_ctrl2,		dark_color_cr,	w_data[2]);
					break;
				case PE_INF_HW_M14A_M_OVR_BACK_COLOR:
					sel = (w_data[0]<6)? w_data[0]:5;
					w_data[0] = yuv08_data[sel][0];	// y
					w_data[1] = yuv08_data[sel][1];	// cb
					w_data[2] = yuv08_data[sel][2];	// cr
					PE_OVR_M14A0_QWr01(ovr_ov_ctrl2,		back_color_yy,	w_data[0]);
					PE_OVR_M14A0_QWr01(ovr_ov_ctrl2,		back_color_cb,	w_data[1]);
					PE_OVR_M14A0_QWr01(ovr_ov_ctrl2,		back_color_cr,	w_data[2]);
					break;
				case PE_INF_HW_M14A_M_PE1_DETOUR:
					PE_P1L_M14A0_QWr01(pe1_top_ctrl_02,		pe1_detour,		w_data[0]);
					break;
				case PE_INF_HW_M14A_M_PE1_REP0_IN_EN:
					PE_P1L_M14A0_QWr01(pe1_conv_ctrl_03,	pxl_rep_in,		w_data[0]);
					break;
				case PE_INF_HW_M14A_M_PE1_REP0_COLOR:
					sel = (w_data[0]<6)? w_data[0]:5;
					w_data[0] = rgb10_data[sel][0];	// r
					w_data[1] = rgb10_data[sel][1];	// g
					w_data[2] = rgb10_data[sel][2];	// b
					PE_P1L_M14A0_QWr01(pe1_conv_ctrl_05,	pxl_rep_value_r,	w_data[0]);
					PE_P1L_M14A0_QWr01(pe1_conv_ctrl_05,	pxl_rep_value_g,	w_data[1]);
					PE_P1L_M14A0_QWr01(pe1_conv_ctrl_05,	pxl_rep_value_b,	w_data[2]);
					break;
				case PE_INF_HW_M14A_M_PE1_REP1_IN_EN:
					PE_P1L_M14A0_QWr01(pe1_conv_ctrl_08,		pxl_rep_in,		w_data[0]);
					break;
				case PE_INF_HW_M14A_M_PE1_REP1_COLOR:
					sel = (w_data[0]<6)? w_data[0]:5;
					w_data[0] = rgb10_data[sel][0];	// r
					w_data[1] = rgb10_data[sel][1];	// g
					w_data[2] = rgb10_data[sel][2];	// b
					PE_P1L_M14A0_QWr01(pe1_conv_ctrl_0a,	pxl_rep_value_r,	w_data[0]);
					PE_P1L_M14A0_QWr01(pe1_conv_ctrl_0a,	pxl_rep_value_g,	w_data[1]);
					PE_P1L_M14A0_QWr01(pe1_conv_ctrl_0a,	pxl_rep_value_b,	w_data[2]);
					break;
				case PE_INF_HW_M14A_M_DISPLAY:
				default:
					break;
			}
		}
		else
		{
			PE_INF_M14_DBG_PRINT("nothing to do.\n");
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
static int PE_INF_M14_GetPathMonitor(LX_PE_ETC_TBL_T *pstParams)
{
	int ret = RET_OK;
	UINT32 r_data[3];
	char buffer[PE_TRACE_STR_SIZE];
	static UINT32 rd_cnt=0;

#define PE_INF_M14_PRINT_START		"START_OF_PRINT"
#define PE_INF_M14_PRINT_RESERVED	"RESERVED"
#define PE_INF_M14_PRINT_END		"END_OF_PRINT"
#define PE_INF_M14_GETPM1_SPRINT(_buf, _type, _r1)	\
	sprintf(_buf, "   [0x%02x][%s] : %d\n", \
	(_type), #_type, (_r1))
#define PE_INF_M14_GETPM3_SPRINT(_buf, _type, _r1, _r2, _r3)	\
	sprintf(_buf, "   [0x%02x][%s] : 0x%03x 0x%03x 0x%03x\n", \
	(_type), #_type, (_r1), (_r2), (_r3))

	do{
		CHECK_KNULL(pstParams);
		CHECK_KNULL(pstParams->data);
		if(PE_KDRV_VER_M14BX)
		{
			switch (rd_cnt)
			{
				case PE_INF_HW_M14B_M_DISPLAY:
					sprintf(buffer, "%s", PE_INF_M14_PRINT_START);
					break;
				case PE_INF_HW_M14B_M_CVA_TPG___TYPE:
					PE_CVA_M14B0_QRd01(cva_misc_ctrl,pattern_type,r_data[0]);
					PE_INF_M14_GETPM1_SPRINT(buffer, PE_INF_HW_M14B_M_CVA_TPG___TYPE, r_data[0]);
					break;
				case PE_INF_HW_M14B_M_CVA_TPG__C_S_C:
					PE_CVA_M14B0_QRd01(cva_misc_ctrl,pattern_csc,r_data[0]);
					PE_INF_M14_GETPM1_SPRINT(buffer, PE_INF_HW_M14B_M_CVA_TPG__C_S_C, r_data[0]);
					break;
				case PE_INF_HW_M14B_M_CVA_TPG_DETAIL:
					PE_CVA_M14B0_QRd01(cva_misc_ctrl,pattern_detail,r_data[0]);
					PE_INF_M14_GETPM1_SPRINT(buffer, PE_INF_HW_M14B_M_CVA_TPG_DETAIL, r_data[0]);
					break;
				case PE_INF_HW_M14B_M_CVB_TPG___TYPE:
					PE_CVB_M14B0_QRd01(cvb_misc_ctrl,pattern_type,r_data[0]);
					PE_INF_M14_GETPM1_SPRINT(buffer, PE_INF_HW_M14B_M_CVB_TPG___TYPE, r_data[0]);
					break;
				case PE_INF_HW_M14B_M_CVB_TPG__C_S_C:
					PE_CVB_M14B0_QRd01(cvb_misc_ctrl,pattern_csc,r_data[0]);
					PE_INF_M14_GETPM1_SPRINT(buffer, PE_INF_HW_M14B_M_CVB_TPG__C_S_C, r_data[0]);
					break;
				case PE_INF_HW_M14B_M_CVB_TPG_DETAIL:
					PE_CVB_M14B0_QRd01(cvb_misc_ctrl,pattern_detail,r_data[0]);
					PE_INF_M14_GETPM1_SPRINT(buffer, PE_INF_HW_M14B_M_CVB_TPG_DETAIL, r_data[0]);
					break;
				case PE_INF_HW_M14B_M_P0M_DETOUR_ENB:
					PE_P0M_M14B0_QRd01(p0m_pe0_opmode,detour_enable,r_data[0]);
					PE_INF_M14_GETPM1_SPRINT(buffer, PE_INF_HW_M14B_M_P0M_DETOUR_ENB, r_data[0]);
					break;
				case PE_INF_HW_M14B_M_P0M_TNR_ENABLE:
					PE_P0M_M14B0_QRd01(p0m_tnr_ctrl_00,tnr_enable,r_data[0]);
					PE_INF_M14_GETPM1_SPRINT(buffer, PE_INF_HW_M14B_M_P0M_TNR_ENABLE, r_data[0]);
					break;
				case PE_INF_HW_M14B_M_P0M_IPC_Y_MODE:
					PE_P0M_M14B0_QRd01(p0m_ipc_ctrl_00,cr_luma_mode,r_data[0]);
					PE_INF_M14_GETPM1_SPRINT(buffer, PE_INF_HW_M14B_M_P0M_IPC_Y_MODE, r_data[0]);
					break;
				case PE_INF_HW_M14B_M_P0M_IPC_C_MODE:
					PE_P0M_M14B0_QRd01(p0m_ipc_ctrl_00,cr_chroma_mode,r_data[0]);
					PE_INF_M14_GETPM1_SPRINT(buffer, PE_INF_HW_M14B_M_P0M_IPC_C_MODE, r_data[0]);
					break;
				case PE_INF_HW_M14B_M_P0M_FMD_ENABLE:
					PE_P0M_M14B0_QRd01(p0m_ipc_ctrl_00,cr_film_mode_enable,r_data[0]);
					PE_INF_M14_GETPM1_SPRINT(buffer, PE_INF_HW_M14B_M_P0M_FMD_ENABLE, r_data[0]);
					break;
				case PE_INF_HW_M14B_M_P0M_CLC_FLT_EN:
					PE_P0M_M14B0_QRd01(p0m_clc_ctrl_00,cl_filter_enable,r_data[0]);
					PE_INF_M14_GETPM1_SPRINT(buffer, PE_INF_HW_M14B_M_P0M_CLC_FLT_EN, r_data[0]);
					break;
				case PE_INF_HW_M14B_M_P0D_DETOUR_ENB:
					PE_P0D_M14B0_QRd01(p0d_pe0_opmode,detour_enable,r_data[0]);
					PE_INF_M14_GETPM1_SPRINT(buffer, PE_INF_HW_M14B_M_P0D_DETOUR_ENB, r_data[0]);
					break;
				case PE_INF_HW_M14B_M_P0D_VFILTER_EN:
					PE_P0D_M14B0_QRd01(p0d_pe0_opmode,detour_enable,r_data[0]);
					PE_INF_M14_GETPM1_SPRINT(buffer, PE_INF_HW_M14B_M_P0D_VFILTER_EN, r_data[0]);
					break;
				case PE_INF_HW_M14B_M_P0D_CTI_ENABLE:
					PE_P0D_M14B0_QRd01(p0d_cti_ctrl_0,cti_en,r_data[0]);
					PE_INF_M14_GETPM1_SPRINT(buffer, PE_INF_HW_M14B_M_P0D_CTI_ENABLE, r_data[0]);
					break;
				case PE_INF_HW_M14B_M_P0D_DNR_MAX_EN:
					PE_P0D_M14B0_QRd01(p0d_dnr_max_ctrl,reg_dnr_max_enable,r_data[0]);
					PE_INF_M14_GETPM1_SPRINT(buffer, PE_INF_HW_M14B_M_P0D_DNR_MAX_EN, r_data[0]);
					break;
				case PE_INF_HW_M14B_M_P0D_IFC_MAX_EN:
					PE_P0D_M14B0_QRd01(p0d_dnr_max_ctrl,reg_ifc_max_enable,r_data[0]);
					PE_INF_M14_GETPM1_SPRINT(buffer, PE_INF_HW_M14B_M_P0D_IFC_MAX_EN, r_data[0]);
					break;
				case PE_INF_HW_M14B_M_P0D_DNR_IFC_SL:
					PE_P0D_M14B0_QRd01(p0d_dnr_max_ctrl,reg_dnr_ifc_sel,r_data[0]);
					PE_INF_M14_GETPM1_SPRINT(buffer, PE_INF_HW_M14B_M_P0D_DNR_IFC_SL, r_data[0]);
					break;
				case PE_INF_HW_M14B_M_P0S_DETOUR_ENB:
					PE_P0S_M14B0_QRd01(p0s_pe0_opmode,detour_enable,r_data[0]);
					PE_INF_M14_GETPM1_SPRINT(buffer, PE_INF_HW_M14B_M_P0S_DETOUR_ENB, r_data[0]);
					break;
				case PE_INF_HW_M14B_M_P0S_IPC_Y_MODE:
					PE_P0S_M14B0_QRd01(p0s_ipc_ctrl_00,cr_luma_mode,r_data[0]);
					PE_INF_M14_GETPM1_SPRINT(buffer, PE_INF_HW_M14B_M_P0S_IPC_Y_MODE, r_data[0]);
					break;
				case PE_INF_HW_M14B_M_P0S_IPC_C_MODE:
					PE_P0S_M14B0_QRd01(p0s_ipc_ctrl_00,cr_chroma_mode,r_data[0]);
					PE_INF_M14_GETPM1_SPRINT(buffer, PE_INF_HW_M14B_M_P0S_IPC_C_MODE, r_data[0]);
					break;
				case PE_INF_HW_M14B_M_P0S_FMD_ENABLE:
					PE_P0S_M14B0_QRd01(p0s_ipc_ctrl_00,cr_film_mode_enable,r_data[0]);
					PE_INF_M14_GETPM1_SPRINT(buffer, PE_INF_HW_M14B_M_P0S_FMD_ENABLE, r_data[0]);
					break;
				case PE_INF_HW_M14B_M_MSC_FILT_SEL_Y:
					PE_MSC_M14B0_QRd01(msc_ctrl,filter_type_sel_luma,r_data[0]);
					PE_INF_M14_GETPM1_SPRINT(buffer, PE_INF_HW_M14B_M_MSC_FILT_SEL_Y, r_data[0]);
					break;
				case PE_INF_HW_M14B_M_MSC_FILT_SEL_C:
					PE_MSC_M14B0_QRd01(msc_ctrl,filter_type_sel_chroma,r_data[0]);
					PE_INF_M14_GETPM1_SPRINT(buffer, PE_INF_HW_M14B_M_MSC_FILT_SEL_C, r_data[0]);
					break;
				case PE_INF_HW_M14B_M_MSC_SAMPL_MODE:
					PE_MSC_M14B0_QRd01(msc_ctrl,manual_sampling_mode,r_data[0]);
					PE_INF_M14_GETPM1_SPRINT(buffer, PE_INF_HW_M14B_M_MSC_SAMPL_MODE, r_data[0]);
					break;
				case PE_INF_HW_M14B_M_SSC_FILT_SEL_Y:
					PE_SSC_M14B0_QRd01(ssc_ctrl,filter_type_sel,r_data[0]);
					PE_INF_M14_GETPM1_SPRINT(buffer, PE_INF_HW_M14B_M_SSC_FILT_SEL_Y, r_data[0]);
					break;
				#ifdef PE_HW_M14_USE_M14B_WCP_REG
				case PE_INF_HW_M14B_M_OVL_DARK_W0_EN:
					PE_OVL_M14B0_QRd01(ovl_ov_ctrl4,w0_dark_en,r_data[0]);
					PE_INF_M14_GETPM1_SPRINT(buffer, PE_INF_HW_M14B_M_OVL_DARK_W0_EN, r_data[0]);
					break;
				case PE_INF_HW_M14B_M_OVL_DARK_W1_EN:
					PE_OVL_M14B0_QRd01(ovl_ov_ctrl4,w1_dark_en,r_data[0]);
					PE_INF_M14_GETPM1_SPRINT(buffer, PE_INF_HW_M14B_M_OVL_DARK_W1_EN, r_data[0]);
					break;
				case PE_INF_HW_M14B_M_OVL_DARK_COLOR:
					PE_OVL_M14B0_QRd01(ovl_ov_ctrl2,dark_color_yy,r_data[0]);
					PE_OVL_M14B0_QRd01(ovl_ov_ctrl2,dark_color_cb,r_data[1]);
					PE_OVL_M14B0_QRd01(ovl_ov_ctrl2,dark_color_cr,r_data[2]);
					PE_INF_M14_GETPM3_SPRINT(buffer, PE_INF_HW_M14B_M_OVL_DARK_COLOR, r_data[0], r_data[1], r_data[2]);
					break;
				case PE_INF_HW_M14B_M_OVL_BACK_COLOR:
					PE_OVL_M14B0_QRd01(ovl_ov_ctrl2,back_color_yy,r_data[0]);
					PE_OVL_M14B0_QRd01(ovl_ov_ctrl2,back_color_cb,r_data[1]);
					PE_OVL_M14B0_QRd01(ovl_ov_ctrl2,back_color_cr,r_data[2]);
					PE_INF_M14_GETPM3_SPRINT(buffer, PE_INF_HW_M14B_M_OVL_BACK_COLOR, r_data[0], r_data[1], r_data[2]);
					break;
				case PE_INF_HW_M14B_M_OVR_DARK_W0_EN:
					PE_OVR_M14B0_QRd01(ovr_ov_ctrl4,w0_dark_en,r_data[0]);
					PE_INF_M14_GETPM1_SPRINT(buffer, PE_INF_HW_M14B_M_OVR_DARK_W0_EN, r_data[0]);
					break;
				case PE_INF_HW_M14B_M_OVR_DARK_W1_EN:
					PE_OVR_M14B0_QRd01(ovr_ov_ctrl4,w1_dark_en,r_data[0]);
					PE_INF_M14_GETPM1_SPRINT(buffer, PE_INF_HW_M14B_M_OVR_DARK_W1_EN, r_data[0]);
					break;
				case PE_INF_HW_M14B_M_OVR_DARK_COLOR:
					PE_OVR_M14B0_QRd01(ovr_ov_ctrl2,dark_color_yy,r_data[0]);
					PE_OVR_M14B0_QRd01(ovr_ov_ctrl2,dark_color_cb,r_data[1]);
					PE_OVR_M14B0_QRd01(ovr_ov_ctrl2,dark_color_cr,r_data[2]);
					PE_INF_M14_GETPM3_SPRINT(buffer, PE_INF_HW_M14B_M_OVR_DARK_COLOR, r_data[0], r_data[1], r_data[2]);
					break;
				case PE_INF_HW_M14B_M_OVR_BACK_COLOR:
					PE_OVR_M14B0_QRd01(ovr_ov_ctrl2,back_color_yy,r_data[0]);
					PE_OVR_M14B0_QRd01(ovr_ov_ctrl2,back_color_cb,r_data[1]);
					PE_OVR_M14B0_QRd01(ovr_ov_ctrl2,back_color_cr,r_data[2]);
					PE_INF_M14_GETPM3_SPRINT(buffer, PE_INF_HW_M14B_M_OVR_BACK_COLOR, r_data[0], r_data[1], r_data[2]);
					break;
				#endif
				case PE_INF_HW_M14B_M_PE1_DETOUR_ENB:
					PE_PE1_M14B0_QRd01(pe1_top_ctrl_02,pe1_detour,r_data[0]);
					PE_INF_M14_GETPM1_SPRINT(buffer, PE_INF_HW_M14B_M_PE1_DETOUR_ENB, r_data[0]);
					break;
				case PE_INF_HW_M14B_M_PE1_WIN1_W0_EN:
					PE_PE1_M14B0_QRd01(pe1_win1_ctrl_00,win0_en,r_data[0]);
					PE_INF_M14_GETPM1_SPRINT(buffer, PE_INF_HW_M14B_M_PE1_WIN1_W0_EN, r_data[0]);
					break;
				case PE_INF_HW_M14B_M_PE1_WIN1_W1_EN:
					PE_PE1_M14B0_QRd01(pe1_win1_ctrl_00,win1_en,r_data[0]);
					PE_INF_M14_GETPM1_SPRINT(buffer, PE_INF_HW_M14B_M_PE1_WIN1_W1_EN, r_data[0]);
					break;
				case PE_INF_HW_M14B_M_PE1_WIN1_01_EN:
					PE_PE1_M14B0_QRd01(pe1_win1_ctrl_00,win01_en,r_data[0]);
					PE_INF_M14_GETPM1_SPRINT(buffer, PE_INF_HW_M14B_M_PE1_WIN1_01_EN, r_data[0]);
					break;
				case PE_INF_HW_M14B_M_PE1_WIN2_W0_EN:
					PE_PE1_M14B0_QRd01(pe1_win2_ctrl_00,win0_en,r_data[0]);
					PE_INF_M14_GETPM1_SPRINT(buffer, PE_INF_HW_M14B_M_PE1_WIN2_W0_EN, r_data[0]);
					break;
				case PE_INF_HW_M14B_M_PE1_WIN2_W1_EN:
					PE_PE1_M14B0_QRd01(pe1_win2_ctrl_00,win1_en,r_data[0]);
					PE_INF_M14_GETPM1_SPRINT(buffer, PE_INF_HW_M14B_M_PE1_WIN2_W1_EN, r_data[0]);
					break;
				case PE_INF_HW_M14B_M_PE1_WIN2_01_EN:
					PE_PE1_M14B0_QRd01(pe1_win2_ctrl_00,win01_en,r_data[0]);
					PE_INF_M14_GETPM1_SPRINT(buffer, PE_INF_HW_M14B_M_PE1_WIN2_01_EN, r_data[0]);
					break;
				case PE_INF_HW_M14B_M_PE1_WIN3_W0_EN:
					PE_PE1_M14B0_QRd01(pe1_win3_ctrl_00,win0_en,r_data[0]);
					PE_INF_M14_GETPM1_SPRINT(buffer, PE_INF_HW_M14B_M_PE1_WIN3_W0_EN, r_data[0]);
					break;
				case PE_INF_HW_M14B_M_PE1_WIN3_W1_EN:
					PE_PE1_M14B0_QRd01(pe1_win3_ctrl_00,win1_en,r_data[0]);
					PE_INF_M14_GETPM1_SPRINT(buffer, PE_INF_HW_M14B_M_PE1_WIN3_W1_EN, r_data[0]);
					break;
				case PE_INF_HW_M14B_M_PE1_WIN3_01_EN:
					PE_PE1_M14B0_QRd01(pe1_win3_ctrl_00,win01_en,r_data[0]);
					PE_INF_M14_GETPM1_SPRINT(buffer, PE_INF_HW_M14B_M_PE1_WIN3_01_EN, r_data[0]);
					break;
				case PE_INF_HW_M14B_M_PE1_PMUX3D_ENB:
					PE_PE1_M14B0_QRd01(pe1_pmux_ctrl_00,mux3d_en,r_data[0]);
					PE_INF_M14_GETPM1_SPRINT(buffer, PE_INF_HW_M14B_M_PE1_PMUX3D_ENB, r_data[0]);
					break;
				case PE_INF_HW_M14B_M_PE1_PMUXOUT_SL:
					PE_PE1_M14B0_QRd01(pe1_pmux_ctrl_00,reg_out_mux,r_data[0]);
					PE_INF_M14_GETPM1_SPRINT(buffer, PE_INF_HW_M14B_M_PE1_PMUXOUT_SL, r_data[0]);
					break;
				case PE_INF_HW_M14B_M_PE1_DR_H_Y_ENB:
					PE_PE1_M14B0_QRd01(pe1_derh_ctrl_06,edge_y_filter_en,r_data[0]);
					PE_INF_M14_GETPM1_SPRINT(buffer, PE_INF_HW_M14B_M_PE1_DR_H_Y_ENB, r_data[0]);
					break;
				case PE_INF_HW_M14B_M_PE1_DR_H_C_ENB:
					PE_PE1_M14B0_QRd01(pe1_derh_ctrl_06,edge_c_filter_en,r_data[0]);
					PE_INF_M14_GETPM1_SPRINT(buffer, PE_INF_HW_M14B_M_PE1_DR_H_C_ENB, r_data[0]);
					break;
				case PE_INF_HW_M14B_M_PE1_DR_V__ENAB:
					PE_PE1_M14B0_QRd01(pe1_derv_ctrl_0,der_v_en,r_data[0]);
					PE_INF_M14_GETPM1_SPRINT(buffer, PE_INF_HW_M14B_M_PE1_DR_V__ENAB, r_data[0]);
					break;
				case PE_INF_HW_M14B_M_PE1_MP__ENABLE:
					PE_PE1_M14B0_QRd01(pe1_mp_ctrl_00,edge_enhance_enable,r_data[0]);
					PE_INF_M14_GETPM1_SPRINT(buffer, PE_INF_HW_M14B_M_PE1_MP__ENABLE, r_data[0]);
					break;
				case PE_INF_HW_M14B_M_PE1_SP__ENABLE:
					PE_PE1_M14B0_QRd01(pe1_sp_ctrl_00,edge_enhance_enable,r_data[0]);
					PE_INF_M14_GETPM1_SPRINT(buffer, PE_INF_HW_M14B_M_PE1_SP__ENABLE, r_data[0]);
					break;
				case PE_INF_HW_M14B_M_PE1_SR_OUT_MUX:
					PE_PE1_M14B0_QRd01(pe1_sre_ctrl_00,reg_out_mux_sel,r_data[0]);
					PE_INF_M14_GETPM1_SPRINT(buffer, PE_INF_HW_M14B_M_PE1_SR_OUT_MUX, r_data[0]);
					break;
				case PE_INF_HW_M14B_M_PE1_TI__ENABLE:
					PE_PE1_M14B0_QRd01(pe1_ti_ctrl_0,enable,r_data[0]);
					PE_INF_M14_GETPM1_SPRINT(buffer, PE_INF_HW_M14B_M_PE1_TI__ENABLE, r_data[0]);
					break;
				case PE_INF_HW_M14B_M_PE1_DJ__ENABLE:
					PE_PE1_M14B0_QRd01(pe1_dj_ctrl_00,edf_en,r_data[0]);
					PE_INF_M14_GETPM1_SPRINT(buffer, PE_INF_HW_M14B_M_PE1_DJ__ENABLE, r_data[0]);
					break;
				case PE_INF_HW_M14B_M_PE1_CTI_ENABLE:
					PE_PE1_M14B0_QRd01(pe1_cti_ctrl_00,cti_en,r_data[0]);
					PE_INF_M14_GETPM1_SPRINT(buffer, PE_INF_HW_M14B_M_PE1_CTI_ENABLE, r_data[0]);
					break;
				case PE_INF_HW_M14B_M_PE1_YCM_ENABLE:
					PE_PE1_M14B0_QRd01(pe1_cti_ctrl_02,ycm_en1,r_data[0]);
					PE_INF_M14_GETPM1_SPRINT(buffer, PE_INF_HW_M14B_M_PE1_YCM_ENABLE, r_data[0]);
					break;
				case PE_INF_HW_M14B_M_PE1_DCE_ENABLE:
					PE_PE1_M14B0_QRd01(pe1_dce_ctrl_00,dynamic_contrast_en,r_data[0]);
					PE_INF_M14_GETPM1_SPRINT(buffer, PE_INF_HW_M14B_M_PE1_DCE_ENABLE, r_data[0]);
					break;
				case PE_INF_HW_M14B_M_PE1_LR_CR_ENAB:
					PE_PE1_M14B0_QRd01(pe1_lrcr_ctrl_00,enable,r_data[0]);
					PE_INF_M14_GETPM1_SPRINT(buffer, PE_INF_HW_M14B_M_PE1_LR_CR_ENAB, r_data[0]);
					break;
				case PE_INF_HW_M14B_M_PE1_CEN_DETOUR:
					PE_PE1_M14B0_QRd01(pe1_cen_ctrl_00,cen_detour,r_data[0]);
					PE_INF_M14_GETPM1_SPRINT(buffer, PE_INF_HW_M14B_M_PE1_CEN_DETOUR, r_data[0]);
					break;
				case PE_INF_HW_M14B_M_PE1_DSE_ENABLE:
					PE_PE1_M14B0_QRd01(pe1_dse_ctrl_00,dynamic_saturation_en,r_data[0]);
					PE_INF_M14_GETPM1_SPRINT(buffer, PE_INF_HW_M14B_M_PE1_DSE_ENABLE, r_data[0]);
					break;
				case PE_INF_HW_M14B_M_PE1_DRC_ENABLE:
					PE_PE1_M14B0_QRd01(pe1_drc_ctrl_00,reg_drc_en,r_data[0]);
					PE_INF_M14_GETPM1_SPRINT(buffer, PE_INF_HW_M14B_M_PE1_DRC_ENABLE, r_data[0]);
					break;
				case PE_INF_HW_M14B_M_PE1_CW__ENABLE:
					PE_PE1_M14B0_QRd01(pe1_cw_ctrl_00,cw_en,r_data[0]);
					PE_INF_M14_GETPM1_SPRINT(buffer, PE_INF_HW_M14B_M_PE1_CW__ENABLE, r_data[0]);
					break;
				case PE_INF_HW_M14B_M_PE1_CONV_L_ENB:
					PE_PE1_M14B0_QRd01(pe1_conv_ctrl_03,pxl_rep_in,r_data[0]);
					PE_INF_M14_GETPM1_SPRINT(buffer, PE_INF_HW_M14B_M_PE1_CONV_L_ENB, r_data[0]);
					break;
				case PE_INF_HW_M14B_M_PE1_CONV_L_COL:
					PE_PE1_M14B0_QRd01(pe1_conv_ctrl_05,pxl_rep_value_r,r_data[0]);
					PE_PE1_M14B0_QRd01(pe1_conv_ctrl_05,pxl_rep_value_g,r_data[1]);
					PE_PE1_M14B0_QRd01(pe1_conv_ctrl_05,pxl_rep_value_b,r_data[2]);
					PE_INF_M14_GETPM3_SPRINT(buffer, PE_INF_HW_M14B_M_PE1_CONV_L_COL, r_data[0], r_data[1], r_data[2]);
					break;
				case PE_INF_HW_M14B_M_PE1_CONV_R_ENB:
					PE_PE1_M14B0_QRd01(pe1_conv_ctrl_08,pxl_rep_in,r_data[0]);
					PE_INF_M14_GETPM1_SPRINT(buffer, PE_INF_HW_M14B_M_PE1_CONV_R_ENB, r_data[0]);
					break;
				case PE_INF_HW_M14B_M_PE1_CONV_R_COL:
					PE_PE1_M14B0_QRd01(pe1_conv_ctrl_0a,pxl_rep_value_r,r_data[0]);
					PE_PE1_M14B0_QRd01(pe1_conv_ctrl_0a,pxl_rep_value_g,r_data[1]);
					PE_PE1_M14B0_QRd01(pe1_conv_ctrl_0a,pxl_rep_value_b,r_data[2]);
					PE_INF_M14_GETPM3_SPRINT(buffer, PE_INF_HW_M14B_M_PE1_CONV_R_COL, r_data[0], r_data[1], r_data[2]);
					break;

				case PE_INF_HW_M14B_M_PE1_CMUX3D_ENB:
					PE_PE1_M14B0_QRd01(pe1_conv_ctrl_0d,mux3d_en,r_data[0]);
					PE_INF_M14_GETPM1_SPRINT(buffer, PE_INF_HW_M14B_M_PE1_CMUX3D_ENB, r_data[0]);
					break;
				case PE_INF_HW_M14B_M_PE1_CMUX3D_COL:
					PE_PE1_M14B0_QRd01(pe1_conv_ctrl_12,bg_color_r,r_data[0]);
					PE_PE1_M14B0_QRd01(pe1_conv_ctrl_12,bg_color_g,r_data[1]);
					PE_PE1_M14B0_QRd01(pe1_conv_ctrl_12,bg_color_b,r_data[2]);
					PE_INF_M14_GETPM3_SPRINT(buffer, PE_INF_HW_M14B_M_PE1_CMUX3D_COL, r_data[0], r_data[1], r_data[2]);
					break;
				case PE_INF_HW_M14B_M_PE1_MUTE_L_ENB:
					PE_PE1_M14B0_QRd01(pe1_mute_ctrl_00,pxl_rep_in,r_data[0]);
					PE_INF_M14_GETPM1_SPRINT(buffer, PE_INF_HW_M14B_M_PE1_MUTE_L_ENB, r_data[0]);
					break;
				case PE_INF_HW_M14B_M_PE1_MUTE_L_COL:
					PE_PE1_M14B0_QRd01(pe1_mute_ctrl_02,pxl_rep_value_r,r_data[0]);
					PE_PE1_M14B0_QRd01(pe1_mute_ctrl_02,pxl_rep_value_g,r_data[1]);
					PE_PE1_M14B0_QRd01(pe1_mute_ctrl_02,pxl_rep_value_b,r_data[2]);
					PE_INF_M14_GETPM3_SPRINT(buffer, PE_INF_HW_M14B_M_PE1_MUTE_L_COL, r_data[0], r_data[1], r_data[2]);
					break;
				case PE_INF_HW_M14B_M_PE1_MUTE_R_ENB:
					PE_PE1_M14B0_QRd01(pe1_mute_ctrl_05,pxl_rep_in,r_data[0]);
					PE_INF_M14_GETPM1_SPRINT(buffer, PE_INF_HW_M14B_M_PE1_MUTE_R_ENB, r_data[0]);
					break;
				case PE_INF_HW_M14B_M_PE1_MUTE_R_COL:
					PE_PE1_M14B0_QRd01(pe1_mute_ctrl_07,pxl_rep_value_r,r_data[0]);
					PE_PE1_M14B0_QRd01(pe1_mute_ctrl_07,pxl_rep_value_g,r_data[1]);
					PE_PE1_M14B0_QRd01(pe1_mute_ctrl_07,pxl_rep_value_b,r_data[2]);
					PE_INF_M14_GETPM3_SPRINT(buffer, PE_INF_HW_M14B_M_PE1_MUTE_R_COL, r_data[0], r_data[1], r_data[2]);
					break;
				case PE_INF_HW_M14B_M_PE1_MMUX3D_ENB:
					PE_PE1_M14B0_QRd01(pe1_mute_ctrl_0a,mux3d_en,r_data[0]);
					PE_INF_M14_GETPM1_SPRINT(buffer, PE_INF_HW_M14B_M_PE1_MMUX3D_ENB, r_data[0]);
					break;
				case PE_INF_HW_M14B_M_PE1_MMUX3D_COL:
					PE_PE1_M14B0_QRd01(pe1_mute_ctrl_0f,bg_color_r,r_data[0]);
					PE_PE1_M14B0_QRd01(pe1_mute_ctrl_0f,bg_color_g,r_data[1]);
					PE_PE1_M14B0_QRd01(pe1_mute_ctrl_0f,bg_color_b,r_data[2]);
					PE_INF_M14_GETPM3_SPRINT(buffer, PE_INF_HW_M14B_M_PE1_MMUX3D_COL, r_data[0], r_data[1], r_data[2]);
					break;
				default:
					sprintf(buffer, "%s", PE_INF_M14_PRINT_END);
					break;
			}
			if (rd_cnt<PE_INF_HW_M14B_M_MAX)
			{
				rd_cnt++;
			}
			else
			{
				rd_cnt = 0;
			}
			memcpy(pstParams->data,buffer,sizeof(char)*PE_TRACE_STR_SIZE);
		}
		else if(PE_KDRV_VER_M14AX)
		{
			switch (rd_cnt)
			{
				case PE_INF_HW_M14A_M_DISPLAY:
					sprintf(buffer, "%s", PE_INF_M14_PRINT_START);
					break;
				case PE_INF_HW_M14A_M_CVA_PATTERN_TYPE:
					PE_CVA_M14A0_QRd01(cva_misc_ctrl,	pattern_type,		r_data[0]);
					sprintf(buffer, "   [0x%02x][CVA_pattern_en](0:off,1:on)  : %d\n", \
						PE_INF_HW_M14A_M_CVA_PATTERN_TYPE, r_data[0]);
					break;
				case PE_INF_HW_M14A_M_CVA_PATTERN_CSC:
					PE_CVA_M14A0_QRd01(cva_misc_ctrl,	pattern_csc,		r_data[0]);
					sprintf(buffer, "   [0x%02x][CVA_pattern_csc](0:yc,1:rgb) : %d\n", \
						PE_INF_HW_M14A_M_CVA_PATTERN_CSC, r_data[0]);
					break;
				case PE_INF_HW_M14A_M_CVA_PATTERN_DETAIL:
					PE_CVA_M14A0_QRd01(cva_misc_ctrl,	pattern_detail,		r_data[0]);
					sprintf(buffer, "   [0x%02x][CVA_pattern_num](0~12)       : %d\n", \
						PE_INF_HW_M14A_M_CVA_PATTERN_DETAIL, r_data[0]);
					break;
				case PE_INF_HW_M14A_M_CVA_FIR_EN:
					PE_CVA_M14A0_QRd01(cva_dig_filt_ctrl3,	fir_en,			r_data[0]);
					sprintf(buffer, "   [0x%02x][CVA_fir_en](0:off,1:on)      : %d\n", \
						PE_INF_HW_M14A_M_CVA_FIR_EN, r_data[0]);
					break;
				case PE_INF_HW_M14A_M_CVA_FIR_Y_EN:
					PE_CVA_M14A0_QRd01(cva_dig_filt_ctrl3,	fir_y_en,		r_data[0]);
					sprintf(buffer, "   [0x%02x][CVA_fir_y_en](0:off,1:on)    : %d\n", \
						PE_INF_HW_M14A_M_CVA_FIR_Y_EN, r_data[0]);
					break;
				case PE_INF_HW_M14A_M_CVA_FIR_C_EN:
					PE_CVA_M14A0_QRd01(cva_dig_filt_ctrl3,	fir_c_en,		r_data[0]);
					sprintf(buffer, "   [0x%02x][CVA_fir_c_en](0:off,1:on)    : %d\n", \
						PE_INF_HW_M14A_M_CVA_FIR_C_EN, r_data[0]);
					break;
				case PE_INF_HW_M14A_M_CVB_PATTERN_TYPE:
					PE_CVB_M14A0_QRd01(cvb_misc_ctrl,	pattern_type,		r_data[0]);
					sprintf(buffer, "   [0x%02x][CVB_pattern_en](0:off,1:on)  : %d\n", \
						PE_INF_HW_M14A_M_CVB_PATTERN_TYPE, r_data[0]);
					break;
				case PE_INF_HW_M14A_M_CVB_PATTERN_CSC:
					PE_CVB_M14A0_QRd01(cvb_misc_ctrl,	pattern_csc,		r_data[0]);
					sprintf(buffer, "   [0x%02x][CVB_pattern_csc](0:yc,1:rgb) : %d\n", \
						PE_INF_HW_M14A_M_CVB_PATTERN_CSC, r_data[0]);
					break;
				case PE_INF_HW_M14A_M_CVB_PATTERN_DETAIL:
					PE_CVB_M14A0_QRd01(cvb_misc_ctrl,	pattern_detail,		r_data[0]);
					sprintf(buffer, "   [0x%02x][CVB_pattern_num](0~12)       : %d\n", \
						PE_INF_HW_M14A_M_CVB_PATTERN_DETAIL, r_data[0]);
					break;
				case PE_INF_HW_M14A_M_CVB_FIR_EN:
					PE_CVB_M14A0_QRd01(cvb_dig_filt_ctrl3,	fir_en,			r_data[0]);
					sprintf(buffer, "   [0x%02x][CVB_fir_en](0:off,1:on)      : %d\n", \
						PE_INF_HW_M14A_M_CVB_FIR_EN, r_data[0]);
					break;
				case PE_INF_HW_M14A_M_CVB_FIR_Y_EN:
					PE_CVB_M14A0_QRd01(cvb_dig_filt_ctrl3,	fir_y_en,		r_data[0]);
					sprintf(buffer, "   [0x%02x][CVB_fir_y_en](0:off,1:on)    : %d\n", \
						PE_INF_HW_M14A_M_CVB_FIR_Y_EN, r_data[0]);
					break;
				case PE_INF_HW_M14A_M_CVB_FIR_C_EN:
					PE_CVB_M14A0_QRd01(cvb_dig_filt_ctrl3,	fir_c_en,		r_data[0]);
					sprintf(buffer, "   [0x%02x][CVB_fir_c_en](0:off,1:on)    : %d\n", \
						PE_INF_HW_M14A_M_CVB_FIR_C_EN, r_data[0]);
					break;
				case PE_INF_HW_M14A_M_P0L_DETOUR:
					PE_P0L_M14A0_QRd01(p0l_pe0_opmode,		detour_enable,	r_data[0]);
					sprintf(buffer, "   [0x%02x][P0L_detour](0:off,1:on)      : %d\n", \
						PE_INF_HW_M14A_M_P0L_DETOUR, r_data[0]);
					break;
				case PE_INF_HW_M14A_M_P0L_IPC_Y:
					PE_P0L_M14A0_QRd01(p0l_ipc_ctrl_00,		cr_luma_mode,	r_data[0]);
					sprintf(buffer, "   [0x%02x][P0L_ipc_y_md](0:2d,1:3d,2:ma): %d\n", \
						PE_INF_HW_M14A_M_P0L_IPC_Y, r_data[0]);
					break;
				case PE_INF_HW_M14A_M_P0L_IPC_C:
					PE_P0L_M14A0_QRd01(p0l_ipc_ctrl_00,		cr_chroma_mode,	r_data[0]);
					sprintf(buffer, "   [0x%02x][P0L_ipc_c_md](0:2d,1:3d,2:ma): %d\n", \
						PE_INF_HW_M14A_M_P0L_IPC_C, r_data[0]);
					break;
				case PE_INF_HW_M14A_M_P0R_DETOUR:
					PE_P0R_M14A0_QRd01(p0r_pe0_opmode,		detour_enable,	r_data[0]);
					sprintf(buffer, "   [0x%02x][P0R_detour](0:off,1:on)      : %d\n", \
						PE_INF_HW_M14A_M_P0R_DETOUR, r_data[0]);
					break;
				case PE_INF_HW_M14A_M_P0R_IPC_Y:
					PE_P0R_M14A0_QRd01(p0r_ipc_ctrl_00,		cr_luma_mode,	r_data[0]);
					sprintf(buffer, "   [0x%02x][P0R_ipc_y_md](0:2d,1:3d,2:ma): %d\n", \
						PE_INF_HW_M14A_M_P0R_IPC_Y, r_data[0]);
					break;
				case PE_INF_HW_M14A_M_P0R_IPC_C:
					PE_P0R_M14A0_QRd01(p0r_ipc_ctrl_00,		cr_chroma_mode,	r_data[0]);
					sprintf(buffer, "   [0x%02x][P0R_ipc_c_md](0:2d,1:3d,2:ma): %d\n", \
						PE_INF_HW_M14A_M_P0R_IPC_C, r_data[0]);
					break;
				case PE_INF_HW_M14A_M_MSL_FLT_SEL_Y:
					PE_MSL_M14A0_QRd01(msc_ctrl_l,	filter_type_sel_luma,	r_data[0]);
					sprintf(buffer, "   [0x%02x][MSL_bilinear](0:off,1:on)    : %d\n", \
						PE_INF_HW_M14A_M_MSL_FLT_SEL_Y, r_data[0]);
					break;
				case PE_INF_HW_M14A_M_MSR_FLT_SEL_Y:
					PE_MSR_M14A0_QRd01(msc_ctrl_r,	filter_type_sel_luma,	r_data[0]);
					sprintf(buffer, "   [0x%02x][MSR_bilinear](0:off,1:on)    : %d\n", \
						PE_INF_HW_M14A_M_MSR_FLT_SEL_Y, r_data[0]);
					break;
				case PE_INF_HW_M14A_M_OVL_DARK_W0_EN:
					PE_OVL_M14A0_QRd01(ovl_ov_ctrl4,		w0_dark_en,		r_data[0]);
					sprintf(buffer, "   [0x%02x][OVL_w0_dark_en](0:off,1:on)  : %d\n", \
						PE_INF_HW_M14A_M_OVL_DARK_W0_EN, r_data[0]);
					break;
				case PE_INF_HW_M14A_M_OVL_DARK_W1_EN:
					PE_OVL_M14A0_QRd01(ovl_ov_ctrl4,		w1_dark_en,		r_data[0]);
					sprintf(buffer, "   [0x%02x][OVL_w1_dark_en](0:off,1:on)  : %d\n", \
						PE_INF_HW_M14A_M_OVL_DARK_W1_EN, r_data[0]);
					break;
				case PE_INF_HW_M14A_M_OVL_DARK_COLOR:
					PE_OVL_M14A0_QRd01(ovl_ov_ctrl2,		dark_color_yy,	r_data[0]);
					PE_OVL_M14A0_QRd01(ovl_ov_ctrl2,		dark_color_cb,	r_data[1]);
					PE_OVL_M14A0_QRd01(ovl_ov_ctrl2,		dark_color_cr,	r_data[2]);
					sprintf(buffer, "   [0x%02x][OVL_dark_color](ycbcr)(0~5)  : 0x%02x 0x%02x 0x%02x\n", \
						PE_INF_HW_M14A_M_OVL_DARK_COLOR, r_data[0], r_data[1], r_data[2]);
					break;
				case PE_INF_HW_M14A_M_OVL_BACK_COLOR:
					PE_OVL_M14A0_QRd01(ovl_ov_ctrl2,		back_color_yy,	r_data[0]);
					PE_OVL_M14A0_QRd01(ovl_ov_ctrl2,		back_color_cb,	r_data[1]);
					PE_OVL_M14A0_QRd01(ovl_ov_ctrl2,		back_color_cr,	r_data[2]);
					sprintf(buffer, "   [0x%02x][OVL_back_color](ycbcr)(0~5)  : 0x%02x 0x%02x 0x%02x\n", \
						PE_INF_HW_M14A_M_OVL_BACK_COLOR, r_data[0], r_data[1], r_data[2]);
					break;
				case PE_INF_HW_M14A_M_OVR_DARK_W0_EN:
					PE_OVR_M14A0_QRd01(ovr_ov_ctrl4,		w0_dark_en,		r_data[0]);
					sprintf(buffer, "   [0x%02x][OVR_w0_dark_en](0:off,1:on)  : %d\n", \
						PE_INF_HW_M14A_M_OVR_DARK_W0_EN, r_data[0]);
					break;
				case PE_INF_HW_M14A_M_OVR_DARK_W1_EN:
					PE_OVR_M14A0_QRd01(ovr_ov_ctrl4,		w1_dark_en,		r_data[0]);
					sprintf(buffer, "   [0x%02x][OVR_w1_dark_en](0:off,1:on)  : %d\n", \
						PE_INF_HW_M14A_M_OVR_DARK_W1_EN, r_data[0]);
					break;
				case PE_INF_HW_M14A_M_OVR_DARK_COLOR:
					PE_OVR_M14A0_QRd01(ovr_ov_ctrl2,		dark_color_yy,	r_data[0]);
					PE_OVR_M14A0_QRd01(ovr_ov_ctrl2,		dark_color_cb,	r_data[1]);
					PE_OVR_M14A0_QRd01(ovr_ov_ctrl2,		dark_color_cr,	r_data[2]);
					sprintf(buffer, "   [0x%02x][OVR_dark_color](ycbcr)(0~5)  : 0x%02x 0x%02x 0x%02x\n", \
						PE_INF_HW_M14A_M_OVR_DARK_COLOR, r_data[0], r_data[1], r_data[2]);
					break;
				case PE_INF_HW_M14A_M_OVR_BACK_COLOR:
					PE_OVR_M14A0_QRd01(ovr_ov_ctrl2,		back_color_yy,	r_data[0]);
					PE_OVR_M14A0_QRd01(ovr_ov_ctrl2,		back_color_cb,	r_data[1]);
					PE_OVR_M14A0_QRd01(ovr_ov_ctrl2,		back_color_cr,	r_data[2]);
					sprintf(buffer, "   [0x%02x][OVR_back_color](ycbcr)(0~5)  : 0x%02x 0x%02x 0x%02x\n", \
						PE_INF_HW_M14A_M_OVR_BACK_COLOR, r_data[0], r_data[1], r_data[2]);
					break;
				case PE_INF_HW_M14A_M_PE1_DETOUR:
					PE_P1L_M14A0_QRd01(pe1_top_ctrl_02,		pe1_detour,		r_data[0]);
					sprintf(buffer, "   [0x%02x][P1L_detour](0:off,1:on)      : %d\n", \
						PE_INF_HW_M14A_M_PE1_DETOUR, r_data[0]);
					break;
				case PE_INF_HW_M14A_M_PE1_REP0_IN_EN:
					PE_P1L_M14A0_QRd01(pe1_conv_ctrl_03,	pxl_rep_in,		r_data[0]);
					sprintf(buffer, "   [0x%02x][P1L_L_mute](0:off,1:on)      : %d\n", \
						PE_INF_HW_M14A_M_PE1_REP0_IN_EN, r_data[0]);
					break;
				case PE_INF_HW_M14A_M_PE1_REP0_COLOR:
					PE_P1L_M14A0_QRd01(pe1_conv_ctrl_05,	pxl_rep_value_r,	r_data[0]);
					PE_P1L_M14A0_QRd01(pe1_conv_ctrl_05,	pxl_rep_value_g,	r_data[1]);
					PE_P1L_M14A0_QRd01(pe1_conv_ctrl_05,	pxl_rep_value_b,	r_data[2]);
					sprintf(buffer, "   [0x%02x][P1L_L_color](rgb)(0~5)       : 0x%03x 0x%03x 0x%03x\n", \
						PE_INF_HW_M14A_M_PE1_REP0_COLOR, r_data[0], r_data[1], r_data[2]);
					break;
				case PE_INF_HW_M14A_M_PE1_REP1_IN_EN:
					PE_P1L_M14A0_QRd01(pe1_conv_ctrl_08,		pxl_rep_in,		r_data[0]);
					sprintf(buffer, "   [0x%02x][P1L_R_mute](0:off,1:on)      : %d\n", \
						PE_INF_HW_M14A_M_PE1_REP1_IN_EN, r_data[0]);
					break;
				case PE_INF_HW_M14A_M_PE1_REP1_COLOR:
					PE_P1L_M14A0_QRd01(pe1_conv_ctrl_0a,	pxl_rep_value_r,	r_data[0]);
					PE_P1L_M14A0_QRd01(pe1_conv_ctrl_0a,	pxl_rep_value_g,	r_data[1]);
					PE_P1L_M14A0_QRd01(pe1_conv_ctrl_0a,	pxl_rep_value_b,	r_data[2]);
					sprintf(buffer, "   [0x%02x][P1L_R_color](rgb)(0~5)       : 0x%03x 0x%03x 0x%03x\n", \
						PE_INF_HW_M14A_M_PE1_REP1_COLOR, r_data[0], r_data[1], r_data[2]);
					break;
				default:
					sprintf(buffer, "%s", PE_INF_M14_PRINT_END);
					break;
			}
			if (rd_cnt<PE_INF_HW_M14A_M_MAX)
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
			PE_INF_M14_DBG_PRINT("nothing to do.\n");
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
int PE_INF_M14_GetLevelInfo(LX_PE_INF_LEVEL_T *pstParams)
{
	int ret = RET_OK;
	UINT32 r_data = 0;
	LX_PE_WIN_ID win_id;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		win_id = PE_GET_CHECKED_WINID(pstParams->win_id);
		if(PE_KDRV_VER_M14BX)
		{
			if(PE_CHECK_WIN0(win_id))
			{
				if(pstParams->sel&LX_PE_LEVEL_MOTION)
				{
					PE_P0M_M14B0_QRd01(p0m_tnr_status_02, ma_motion,pstParams->motion_level);
				}
				if(pstParams->sel&LX_PE_LEVEL_NOISE)
				{
					/* TODO_M14BX, read pe1_fsw_ctrl_03, [7:0]reg_nt_lvl */
					PE_P0M_M14B0_QRd01(p0m_tnr_status_04, nlvl, 	r_data);
					pstParams->noise_level = GET_BITS(r_data, 0, 6)<<2;
				}
				if(pstParams->sel&LX_PE_LEVEL_PEAKPOS)
				{
					PE_P0M_M14B0_QRd01(p0m_tnr_status_04, nlvl, 	r_data);
					pstParams->peakpos_level = GET_BITS(r_data, 0, 6)<<4;
				}
			}
		}
		else if(PE_KDRV_VER_M14AX)
		{
			if(PE_CHECK_WIN0(win_id))
			{
				if(pstParams->sel&LX_PE_LEVEL_MOTION)
				{
					PE_P0L_M14A0_QRd01(p0l_tnr_status_00, x_avg_t,	 	pstParams->motion_level);
				}
				if(pstParams->sel&LX_PE_LEVEL_NOISE)
				{
					PE_P0L_M14A0_QRd01(p0l_tnr_status_00, nt_lvl, 		pstParams->noise_level);
				}
				if(pstParams->sel&LX_PE_LEVEL_PEAKPOS)
				{
					PE_P0L_M14A0_QRd01(p0l_tnr_status_00, x_peak_t2, 	r_data);
					pstParams->peakpos_level = GET_BITS(r_data, 0, 6)<<4;
				}
			}
			if(PE_CHECK_WIN1(win_id))
			{
				if(pstParams->sel&LX_PE_LEVEL_MOTION)
				{
					PE_P0R_M14A0_QRd01(p0r_tnr_status_00, x_avg_t, 		pstParams->motion_level);
				}
				if(pstParams->sel&LX_PE_LEVEL_NOISE)
				{
					PE_P0R_M14A0_QRd01(p0r_tnr_status_00, nt_lvl, 		pstParams->noise_level);
				}
				if(pstParams->sel&LX_PE_LEVEL_PEAKPOS)
				{
					PE_P0R_M14A0_QRd01(p0r_tnr_status_00, x_peak_t2, 	r_data);
					pstParams->peakpos_level = GET_BITS(r_data, 0, 6)<<4;
				}
			}
		}
		else
		{
			PE_INF_M14_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
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
int PE_INF_M14_SetMiscTbl(LX_PE_ETC_TBL_T *pstParams)
{
    int ret = RET_OK;
	UINT32 max_num = 0;
	do{
		CHECK_KNULL(pstParams);
		if (PE_KDRV_VER_M14)
		{
			max_num = sizeof(_g_pe_inf_m14_dbg_set_fn)/sizeof(PE_ETC_MISC_TBL_T);
			PE_INF_M14_CHECK_CODE(pstParams->func_num>=max_num, break, \
				"[%s,%d] invalid func_num[%d].\n", __F__, __L__, pstParams->func_num);
			CHECK_KNULL(_g_pe_inf_m14_dbg_set_fn[pstParams->func_num].func);
			ret = _g_pe_inf_m14_dbg_set_fn[pstParams->func_num].func(pstParams);
			PE_INF_M14_CHECK_CODE(ret, break, "[%s,%d] %s() error.\n", \
				__F__, __L__, _g_pe_inf_m14_dbg_set_fn[pstParams->func_num].fn_name);
		}
		else
		{
			PE_INF_M14_DBG_PRINT("nothing to do.\n");
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
int PE_INF_M14_GetMiscTbl(LX_PE_ETC_TBL_T *pstParams)
{
    int ret = RET_OK;
	UINT32 max_num = 0;
	do{
		CHECK_KNULL(pstParams);
		if (PE_KDRV_VER_M14)
		{
			max_num = sizeof(_g_pe_inf_m14_dbg_get_fn)/sizeof(PE_ETC_MISC_TBL_T);
			PE_INF_M14_CHECK_CODE(pstParams->func_num>=max_num, break, \
				"[%s,%d] invalid func_num[%d].\n", __F__, __L__, pstParams->func_num);
			CHECK_KNULL(_g_pe_inf_m14_dbg_get_fn[pstParams->func_num].func);
			ret = _g_pe_inf_m14_dbg_get_fn[pstParams->func_num].func(pstParams);
			PE_INF_M14_CHECK_CODE(ret, break, "[%s,%d] %s() error.\n", \
				__F__, __L__, _g_pe_inf_m14_dbg_get_fn[pstParams->func_num].fn_name);
		}
		else
		{
			PE_INF_M14_DBG_PRINT("nothing to do.\n");
			ret = RET_OK;
		}
	}while (0);
	return ret;
}
/**
 * sort trace data buf to dst buf
 * - ascending, bubble sorting
 *
 * @param   *dst [in/out] PE_INF_M14_DATA_BUF_T
 * @return  void
 * @see
 * @author	sj.youm@lge.com
 */
static void PE_INF_M14_SortTraceDataBuf(PE_INF_M14_DATA_BUF_T *dst)
{
	int ret = RET_OK;
	UINT32 size=0, i=0, max=0;
	UINT32 j=0, s0=0, s1=0, us0=0, us1=0;
	PE_INF_M14_DATA_BUF_T temp;
	PE_INF_M14_DATA_BUF_T *src = _g_pe_inf_m14_buf;

	do{
		CHECK_KNULL(dst);
		max = PE_INF_M14_BUF_MAX;
		size = sizeof(PE_INF_M14_DATA_BUF_T);
		memcpy(dst, src, (size*max));
		for (i=0; i<max; i++)
		{
			for (j=0; j<(max-1-i); j++)
			{
				s0 = dst[j].time.sec;
				s1 = dst[j+1].time.sec;
				us0 = dst[j].time.usec;
				us1 = dst[j+1].time.usec;
				if ((s0>s1) || (s0==s1 && us0>us1))
				{
					memcpy(&temp, &dst[j], size);
					memcpy(&dst[j], &dst[j+1], size);
					memcpy(&dst[j+1], &temp, size);
				}
			}
		}
	}while (0);
	return;
}
/**
 * get trace data buf
 *
 * @param   *data [in] LX_PE_ETC_TBL_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author	sj.youm@lge.com
 */
static int PE_INF_M14_GetTraceDataBuf(LX_PE_ETC_TBL_T *pstParams)
{
	int ret = RET_OK;
	char buffer[PE_TRACE_STR_SIZE];
	static UINT32 rd_cnt=0;
	PE_INF_M14_DATA_BUF_T *p_buf = _g_pe_inf_m14_buf_temp;
	UINT32 *p_data = NULL;
	PE_INF_M14_TIME_T *p_time = NULL;
	char *p_str = NULL;

#define PE_INF_M14_PRINT_RESERVED	"RESERVED"
#define PE_INF_M14_PRINT_END		"END_OF_PRINT"

	do{
		CHECK_KNULL(pstParams);
		CHECK_KNULL(pstParams->data);
		if (rd_cnt<PE_INF_M14_BUF_MAX)
		{
			if(rd_cnt==0)
			{
				PE_INF_M14_SortTraceDataBuf(p_buf);
			}
			p_time = &(p_buf[rd_cnt].time);
			p_str = p_buf[rd_cnt].str;
			if(p_time->sec || p_time->usec)
			{
				sprintf(buffer, "[%06d.%06d] %s\n", \
					p_time->sec, p_time->usec, p_str);
			}
			else
			{
				sprintf(buffer, "%s", PE_INF_M14_PRINT_RESERVED);
			}
			rd_cnt++;
		}
		else
		{
			sprintf(buffer, "%s", PE_INF_M14_PRINT_END);
			rd_cnt=0;
		}
		p_data = pstParams->data;
		memcpy(p_data, buffer, PE_TRACE_STR_SIZE);
	}while (0);
	return ret;
}

/**
 * set trace data buf
 *
 * @param   *data [in] char
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author	sj.youm@lge.com
 */
int PE_INF_M14_SetTraceDataBuf(char *data)
{
    int ret = RET_OK;
	UINT32 i = 0;
	char *p_str = NULL;
	PE_INF_M14_TIME_T *p_time = NULL;
	static UINT32 cnt = 0;
	PE_INF_M14_DATA_BUF_T *p_buf = _g_pe_inf_m14_buf;
	do{
		CHECK_KNULL(data);
		if(cnt>=PE_INF_M14_BUF_MAX)	cnt = 0;
		p_str = p_buf[cnt].str;
		p_time = &(p_buf[cnt].time);
		OS_GetCurrentTicks(&(p_time->sec), \
			&(p_time->msec), &(p_time->usec));
		memset(p_str, 0, PE_TRACE_STR_SIZE);
		for(i=0;i<PE_TRACE_STR_SIZE;i++)
		{
			p_str[i] = data[i];
			if(data[i] == 0) break;
		}
		cnt++;
	}while (0);
	return ret;
}

/**
 * set led pixel value
 *
 * @param   *pstParams [in/out] LX_PE_ETC_TBL_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_INF_M14_SetLedPixelValue(LX_PE_ETC_TBL_T *pstParams)
{
	int ret = RET_OK;

	do{
		CHECK_KNULL(pstParams);
		CHECK_KNULL(pstParams->data);
		if (PE_KDRV_VER_M14BX)
		{
			switch (pstParams->data[0])
			{
				case PE_INF_HW_M14B_LPV_HOR_POS:
					PE_LED_M14B0_QWr01(led_rp_hv_pos,rp_h_pos,pstParams->data[1]);
					break;
				case PE_INF_HW_M14B_LPV_VER_POS:
					PE_LED_M14B0_QWr01(led_rp_hv_pos,rp_v_pos,pstParams->data[1]);
					break;
				case PE_INF_HW_M14B_LPV_MARK_EN:
					PE_LED_M14B0_QWr01(led_rp_make_en,mark_en,pstParams->data[1]);
					break;
				default:
					break;
			}
		}
	}while (0);
	return ret;
}
/**
 * get led pixel value
 *
 * @param   *pstParams [in/out] LX_PE_ETC_TBL_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_INF_M14_GetLedPixelValue(LX_PE_ETC_TBL_T *pstParams)
{
	int ret = RET_OK;
	char buffer[PE_TRACE_STR_SIZE];
	static UINT32 rd_cnt=0;
	UINT32 rd_data[6];

#define PE_INF_M14_LPV_NO_PRINT			0
#define PE_INF_M14_LPV_PRINT_START		"START_OF_PRINT"
#define PE_INF_M14_LPV_PRINT_END		"END_OF_PRINT"
#define PE_INF_M14_LPV_PRINT_EXIT		"EXIT_OF_PRINT"
#define PE_INF_M14_LPV_PRINT_RESERV		"RESERVED"
#define PE_INF_M14_LPV_CASE_PRMENU(_cnt, _action, _buf, _item, _val)	\
	case (_cnt):sprintf(_buf, "   [0x%02x] %s : %d\n", (_item), #_item, (_val));_action;break
#define PE_INF_M14_LPV_CASE_SPRINT(_cnt, _action, _buf, fmt, args...)	\
	case (_cnt):sprintf(_buf, fmt, ##args);_action;break
#define PE_INF_M14_LPV_DFLT_SPRINT(_action,_buf,fmt,args...)		\
	default:sprintf(_buf,fmt,##args);_action;break

	do{
		CHECK_KNULL(pstParams);
		CHECK_KNULL(pstParams->data);
		if (PE_KDRV_VER_M14BX)
		{
			if (pstParams->data[0]==PE_INF_HW_M14B_LPV_DISPLAY)
			{
				PE_LED_M14B0_QRd02(led_rp_hv_pos,rp_h_pos,rd_data[0],rp_v_pos,rd_data[1]);
				PE_LED_M14B0_QRd01(led_rp_make_en,mark_en,rd_data[2]);
				PE_LED_M14B0_QRd01(led_rp_r_value,rd_pixel_r,rd_data[3]);
				PE_LED_M14B0_QRd01(led_rp_g_value,rd_pixel_g,rd_data[4]);
				PE_LED_M14B0_QRd01(led_rp_b_value,rd_pixel_b,rd_data[5]);
				switch (rd_cnt)
				{
					PE_INF_M14_LPV_CASE_SPRINT( 0, rd_cnt++, buffer, "%s", PE_INF_M14_LPV_PRINT_START);
					PE_INF_M14_LPV_CASE_SPRINT( 1, rd_cnt++, buffer, "   * get led pixel *\n");
					PE_INF_M14_LPV_CASE_PRMENU( 2, rd_cnt++, buffer, PE_INF_HW_M14B_LPV_HOR_POS, rd_data[0]);
					PE_INF_M14_LPV_CASE_PRMENU( 3, rd_cnt++, buffer, PE_INF_HW_M14B_LPV_VER_POS, rd_data[1]);
					PE_INF_M14_LPV_CASE_PRMENU( 4, rd_cnt++, buffer, PE_INF_HW_M14B_LPV_MARK_EN, rd_data[2]);
					PE_INF_M14_LPV_CASE_PRMENU( 5, rd_cnt++, buffer, PE_INF_HW_M14B_LPV_PIXEL_R, (rd_data[3]>>2));
					PE_INF_M14_LPV_CASE_PRMENU( 6, rd_cnt++, buffer, PE_INF_HW_M14B_LPV_PIXEL_G, (rd_data[4]>>2));
					PE_INF_M14_LPV_CASE_PRMENU( 7, rd_cnt++, buffer, PE_INF_HW_M14B_LPV_PIXEL_B, (rd_data[5]>>2));
					PE_INF_M14_LPV_CASE_SPRINT( 8, rd_cnt++, buffer, "%s", PE_INF_M14_LPV_PRINT_RESERV);
					PE_INF_M14_LPV_CASE_SPRINT( 9, rd_cnt++, buffer, "%s", PE_INF_M14_LPV_PRINT_RESERV);
					PE_INF_M14_LPV_DFLT_SPRINT(rd_cnt=0, buffer, "%s", PE_INF_M14_LPV_PRINT_END);
				}
			}
			else
			{
				sprintf(buffer, "%s", PE_INF_M14_LPV_PRINT_EXIT);
				rd_cnt=0;
			}
			memcpy(pstParams->data,buffer,sizeof(char)*PE_TRACE_STR_SIZE);
		}
		else
		{
			sprintf(buffer, "%s", PE_INF_M14_LPV_PRINT_EXIT);
			memcpy(pstParams->data,buffer,sizeof(char)*PE_TRACE_STR_SIZE);
		}
	}while (0);
	return ret;
}

/**
 * set trace packet data
 *
 * @param   *pstParams [in] LX_PE_PKT_T int
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author	sj.youm@lge.com
 */
int PE_INF_M14_SetTracePacketData(LX_PE_PKT_T *pstParams)
{
    int ret = RET_OK;
	char str[PE_TRACE_STR_SIZE];
	UINT32 rw_t=0, mdl_t=0, fn_t=0, valid=0, d_size=0;
	UINT32 *d_arg = NULL;

#define PE_INF_M14_CPY_PARAM_TO_STR(_struct, _fmt, _args...)	\
	{\
		_struct p;\
		ret = copy_from_user(&p, (void __user *)d_arg, d_size);\
		PE_INF_M14_CHECK_CODE(ret, break, "copy_from_user error.\n");\
		sprintf(str, "[%s]"_fmt, #_struct, ##_args);\
		valid = 1;\
	}

	do{
		CHECK_KNULL(pstParams);
		CHECK_KNULL(pstParams->data);
		rw_t = PE_GET_PKTINFO_RWTYPE(pstParams->info);
		mdl_t = PE_GET_PKTINFO_MODLTYPE(pstParams->info);
		fn_t = PE_GET_PKTINFO_FUNCTYPE(pstParams->info);
		d_arg = pstParams->data;
		d_size = pstParams->size;
		switch (rw_t)
		{
			case PE_ITEM_PKTRW(INIT):
				PE_INF_M14_CPY_PARAM_TO_STR(LX_PE_INIT_SETTINS_T, \
					"%d, %d", p.win_id, p.suspend_mode);
				break;
			case PE_ITEM_PKTRW(SET):
				switch (mdl_t)
				{
					case PE_ITEM_PKTMODL(DEFAULT):
						PE_INF_M14_CPY_PARAM_TO_STR(LX_PE_DEFAULT_SETTINGS_T, \
							"%d", p.win_id);
						break;
					case PE_ITEM_PKTMODL(CMN):
						switch (fn_t)
						{
							#ifdef PE_INF_M14_DETAIL_TRACE_LOG
							case PE_ITEM_PKTFUNC(LX_PE_CMN_CONTRAST_T):
								PE_INF_M14_CPY_PARAM_TO_STR(LX_PE_CMN_CONTRAST_T, \
									"%d, %d", p.win_id, p.contrast);
								break;
							case PE_ITEM_PKTFUNC(LX_PE_CMN_BRIGHTNESS_T):
								PE_INF_M14_CPY_PARAM_TO_STR(LX_PE_CMN_BRIGHTNESS_T, \
									"%d, %d", p.win_id, p.brightness);
								break;
							case PE_ITEM_PKTFUNC(LX_PE_CMN_SATURATION_T):
								PE_INF_M14_CPY_PARAM_TO_STR(LX_PE_CMN_SATURATION_T, \
									"%d, %d", p.win_id, p.saturation);
								break;
							case PE_ITEM_PKTFUNC(LX_PE_CMN_HUE_T):
								PE_INF_M14_CPY_PARAM_TO_STR(LX_PE_CMN_HUE_T, \
									"%d, %d", p.win_id, p.hue);
								break;
							case PE_ITEM_PKTFUNC(LX_PE_CMN_LEVEL_CTRL_T):
								PE_INF_M14_CPY_PARAM_TO_STR(LX_PE_CMN_LEVEL_CTRL_T, \
									"%d, %d,%d,%d,%d", p.win_id, \
									p.offset, p.center, p.gain, p.tp_on);
								break;
							#else
							case PE_ITEM_PKTFUNC(LX_PE_CMN_CONTRAST_T):
							case PE_ITEM_PKTFUNC(LX_PE_CMN_BRIGHTNESS_T):
							case PE_ITEM_PKTFUNC(LX_PE_CMN_SATURATION_T):
							case PE_ITEM_PKTFUNC(LX_PE_CMN_HUE_T):
							case PE_ITEM_PKTFUNC(LX_PE_CMN_LEVEL_CTRL_T):
							#endif
							default:
								break;
						}
						break;
					case PE_ITEM_PKTMODL(CSC):
						switch (fn_t)
						{
							case PE_ITEM_PKTFUNC(LX_PE_CSC_POST_T):
								PE_INF_M14_CPY_PARAM_TO_STR(LX_PE_CSC_POST_T, \
									"%d, %x,%x,%x,%x,%x,%x,%x,%x,%x, "\
									"%d,%d,%d,%d,%d,%d", p.win_id, \
									p.matrix[0], p.matrix[1], p.matrix[2], \
									p.matrix[3], p.matrix[4], p.matrix[5], \
									p.matrix[6], p.matrix[7], p.matrix[8], \
									(SINT16)p.offset[0], (SINT16)p.offset[1], \
									(SINT16)p.offset[2], (SINT16)p.offset[3], \
									(SINT16)p.offset[4], (SINT16)p.offset[5]);
								break;
							case PE_ITEM_PKTFUNC(LX_PE_CSC_INPUT_T):
								PE_INF_M14_CPY_PARAM_TO_STR(LX_PE_CSC_INPUT_T, \
									"%d, %d, %x,%x,%x,%x,%x,%x,%x,%x,%x, "\
									"%d,%d,%d,%d,%d,%d", p.win_id, p.enable, \
									p.matrix[0], p.matrix[1], p.matrix[2], \
									p.matrix[3], p.matrix[4], p.matrix[5], \
									p.matrix[6], p.matrix[7], p.matrix[8], \
									(SINT16)p.offset[0], (SINT16)p.offset[1], \
									(SINT16)p.offset[2], (SINT16)p.offset[3], \
									(SINT16)p.offset[4], (SINT16)p.offset[5]);
								break;
							#ifdef PE_INF_M14_DETAIL_TRACE_LOG
							case PE_ITEM_PKTFUNC(LX_PE_CSC_XVYCC_T):
								PE_INF_M14_CPY_PARAM_TO_STR(LX_PE_CSC_XVYCC_T, \
									"%d, %d,%d", p.win_id, p.enable, p.scaler);
								break;
							case PE_ITEM_PKTFUNC(LX_PE_CSC_GAMUT_T):
								PE_INF_M14_CPY_PARAM_TO_STR(LX_PE_CSC_GAMUT_T, \
									"%d, %x,%x,%x,%x,%x,%x,%x,%x,%x", p.win_id, \
									p.matrix[0], p.matrix[1], p.matrix[2], \
									p.matrix[3], p.matrix[4], p.matrix[5], \
									p.matrix[6], p.matrix[7], p.matrix[8]);
								break;
							#else
							case PE_ITEM_PKTFUNC(LX_PE_CSC_XVYCC_T):
							case PE_ITEM_PKTFUNC(LX_PE_CSC_GAMUT_T):
							#endif
							default:
								break;
						}
						break;
					case PE_ITEM_PKTMODL(CMG):
						switch (fn_t)
						{
							#ifdef PE_INF_M14_DETAIL_TRACE_LOG
							case PE_ITEM_PKTFUNC(LX_PE_CMG_ENABLE_T):
								PE_INF_M14_CPY_PARAM_TO_STR(LX_PE_CMG_ENABLE_T, \
									"%d, %d", p.win_id, p.enable);
								break;
							case PE_ITEM_PKTFUNC(LX_PE_CMG_GLOBAL_CTRL_T):
								PE_INF_M14_CPY_PARAM_TO_STR(LX_PE_CMG_GLOBAL_CTRL_T, \
									"%d, %d,%d,%d,%d,%d,%d", p.win_id, \
									p.global_delta[0], p.global_delta[1], \
									p.global_delta[2], p.global_delta[3], \
									p.global_delta[4], p.global_delta[5]);
								break;
							#else
							case PE_ITEM_PKTFUNC(LX_PE_CMG_ENABLE_T):
							case PE_ITEM_PKTFUNC(LX_PE_CMG_GLOBAL_CTRL_T):
							#endif
							case PE_ITEM_PKTFUNC(LX_PE_CMG_REGION_ENABLE_T):
							case PE_ITEM_PKTFUNC(LX_PE_CMG_REGION_T):
							case PE_ITEM_PKTFUNC(LX_PE_CMG_REGION_CTRL_T):
							case PE_ITEM_PKTFUNC(LX_PE_CMG_COLOR_CTRL_T):
							case PE_ITEM_PKTFUNC(LX_PE_CMG_CW_CTRL_T):
							case PE_ITEM_PKTFUNC(LX_PE_CMG_CW_GAIN_CTRL_T):
							default:
								break;
						}
						break;
					case PE_ITEM_PKTMODL(NRD):
						switch (fn_t)
						{
							#ifdef PE_INF_M14_DETAIL_TRACE_LOG
							case PE_ITEM_PKTFUNC(LX_PE_NRD_DNR2_CMN_T):
								PE_INF_M14_CPY_PARAM_TO_STR(LX_PE_NRD_DNR2_CMN_T, \
									"%d, %d,%d,%d", p.win_id, \
									p.enable_ac_bnr, p.enable_mnr, p.enable_dc_bnr);
								break;
							case PE_ITEM_PKTFUNC(LX_PE_NRD_DNR2_DETAIL_T):
								PE_INF_M14_CPY_PARAM_TO_STR(LX_PE_NRD_DNR2_DETAIL_T, \
									"%d", p.win_id);
								break;
							case PE_ITEM_PKTFUNC(LX_PE_NRD_TNR2_CMN_T):
								PE_INF_M14_CPY_PARAM_TO_STR(LX_PE_NRD_TNR2_CMN_T, \
									"%d, %d,%d,%d,%d,%d", p.win_id, \
									p.auto_mode_en, p.adjust_nt_lvl, \
									p.adjust_nt_lvl_val, p.y_gain, p.c_gain);
								break;
							case PE_ITEM_PKTFUNC(LX_PE_NRD_TNR2_DETAIL_T):
								PE_INF_M14_CPY_PARAM_TO_STR(LX_PE_NRD_TNR2_DETAIL_T, \
									"%d", p.win_id);
								break;
							case PE_ITEM_PKTFUNC(LX_PE_NRD_DNR4_CMN_T):
								PE_INF_M14_CPY_PARAM_TO_STR(LX_PE_NRD_DNR4_CMN_T, \
									"%d, %d,%d,%d", p.win_id, \
									p.enable_ac_bnr, p.enable_mnr, p.enable_dc_bnr);
								break;
							case PE_ITEM_PKTFUNC(LX_PE_NRD_DNR4_DETAIL_T):
								PE_INF_M14_CPY_PARAM_TO_STR(LX_PE_NRD_DNR4_DETAIL_T, \
									"%d", p.win_id);
								break;
							case PE_ITEM_PKTFUNC(LX_PE_NRD_TNR4_CMN_T):
								PE_INF_M14_CPY_PARAM_TO_STR(LX_PE_NRD_TNR4_CMN_T, \
									"%d, %d,%d,%d,%d,%d", p.win_id, \
									p.auto_mode_en, p.adjust_nt_lvl, \
									p.adjust_nt_lvl_val, p.y_gain, p.c_gain);
								break;
							case PE_ITEM_PKTFUNC(LX_PE_NRD_TNR4_DETAIL_T):
								PE_INF_M14_CPY_PARAM_TO_STR(LX_PE_NRD_TNR4_DETAIL_T, \
									"%d", p.win_id);
								break;
							#else
							case PE_ITEM_PKTFUNC(LX_PE_NRD_DNR2_CMN_T):
							case PE_ITEM_PKTFUNC(LX_PE_NRD_DNR2_DETAIL_T):
							case PE_ITEM_PKTFUNC(LX_PE_NRD_TNR2_CMN_T):
							case PE_ITEM_PKTFUNC(LX_PE_NRD_TNR2_DETAIL_T):
							case PE_ITEM_PKTFUNC(LX_PE_NRD_DNR4_CMN_T):
							case PE_ITEM_PKTFUNC(LX_PE_NRD_DNR4_DETAIL_T):
							case PE_ITEM_PKTFUNC(LX_PE_NRD_TNR4_CMN_T):
							case PE_ITEM_PKTFUNC(LX_PE_NRD_TNR4_DETAIL_T):
							#endif
							case PE_ITEM_PKTFUNC(LX_PE_NRD_DNR0_CMN_T):
							case PE_ITEM_PKTFUNC(LX_PE_NRD_TNR1_CMN_T):
							case PE_ITEM_PKTFUNC(LX_PE_NRD_DNR3_CMN_T):
							case PE_ITEM_PKTFUNC(LX_PE_NRD_TNR3_CMN_T):
							default:
								break;
						}
						break;
					case PE_ITEM_PKTMODL(DNT):
						switch (fn_t)
						{
							case PE_ITEM_PKTFUNC(LX_PE_DNT_FILMMODE_T):
								PE_INF_M14_CPY_PARAM_TO_STR(LX_PE_DNT_FILMMODE_T, \
									"%d, %d", p.win_id, p.enable);
								break;
							case PE_ITEM_PKTFUNC(LX_PE_DNT_LD_MODE_T):
								PE_INF_M14_CPY_PARAM_TO_STR(LX_PE_DNT_LD_MODE_T, \
									"%d, %d", p.win_id, p.enable);
								break;
							default:
								break;
						}
						break;
					case PE_ITEM_PKTMODL(SHP):
						switch (fn_t)
						{
							case PE_ITEM_PKTFUNC(LX_PE_SHP_SCLFILTER_T):
								PE_INF_M14_CPY_PARAM_TO_STR(LX_PE_SHP_SCLFILTER_T, \
									"%d, %d,%d,%d,%d", p.win_id, \
									p.h_luma_id, p.v_luma_id, p.h_chrm_id, p.v_chrm_id);
								break;
							case PE_ITEM_PKTFUNC(LX_PE_SHP_SR0_T):
								PE_INF_M14_CPY_PARAM_TO_STR(LX_PE_SHP_SR0_T, \
									"%d, %d,%d,%d,0x%x,0x%x,%d", p.win_id, \
									p.reg_out_mux_sel, p.reg_sc_gs_sra, p.reg_sc_gs_srb, \
									p.reg_sc_ls_lv_sra, p.reg_sc_ls_lv_srb, p.reg_mode_wei);
								break;
							#ifdef PE_INF_M14_DETAIL_TRACE_LOG
							case PE_ITEM_PKTFUNC(LX_PE_SHP_EE1_T):
								PE_INF_M14_CPY_PARAM_TO_STR(LX_PE_SHP_EE1_T, \
									"%d, %d,%d,%d,%d", p.win_id, \
									p.mp_edge_gain_b, p.mp_edge_gain_w, \
									p.sp_edge_gain_b, p.sp_edge_gain_w);
								break;
							case PE_ITEM_PKTFUNC(LX_PE_SHP_DE1_T):
								PE_INF_M14_CPY_PARAM_TO_STR(LX_PE_SHP_DE1_T, \
									"%d, %d,%d,%d,%d", p.win_id, \
									p.mp_texture_gain_b, p.mp_texture_gain_w, \
									p.sp_texture_gain_b, p.sp_texture_gain_w);
								break;
							case PE_ITEM_PKTFUNC(LX_PE_SHP_RE2_CMN_T):
								PE_INF_M14_CPY_PARAM_TO_STR(LX_PE_SHP_RE2_CMN_T, \
									"%d", p.win_id);
								break;
							case PE_ITEM_PKTFUNC(LX_PE_SHP_RE2_HOR_T):
								PE_INF_M14_CPY_PARAM_TO_STR(LX_PE_SHP_RE2_HOR_T, \
									"%d", p.win_id);
								break;
							case PE_ITEM_PKTFUNC(LX_PE_SHP_RE2_VER_T):
								PE_INF_M14_CPY_PARAM_TO_STR(LX_PE_SHP_RE2_VER_T, \
									"%d", p.win_id);
								break;
							case PE_ITEM_PKTFUNC(LX_PE_SHP_RE2_MISC_T):
								PE_INF_M14_CPY_PARAM_TO_STR(LX_PE_SHP_RE2_MISC_T, \
									"%d", p.win_id);
								break;
							case PE_ITEM_PKTFUNC(LX_PE_SHP_RE3_MISC_T):
								PE_INF_M14_CPY_PARAM_TO_STR(LX_PE_SHP_RE3_MISC_T, \
									"%d", p.win_id);
								break;
							#else
							case PE_ITEM_PKTFUNC(LX_PE_SHP_EE1_T):
							case PE_ITEM_PKTFUNC(LX_PE_SHP_DE1_T):
							case PE_ITEM_PKTFUNC(LX_PE_SHP_RE2_CMN_T):
							case PE_ITEM_PKTFUNC(LX_PE_SHP_RE2_HOR_T):
							case PE_ITEM_PKTFUNC(LX_PE_SHP_RE2_VER_T):
							case PE_ITEM_PKTFUNC(LX_PE_SHP_RE2_MISC_T):
							case PE_ITEM_PKTFUNC(LX_PE_SHP_RE3_MISC_T):
							#endif
							case PE_ITEM_PKTFUNC(LX_PE_SHP_RE1_CMN_T):
							case PE_ITEM_PKTFUNC(LX_PE_SHP_RE1_HOR_T):
							case PE_ITEM_PKTFUNC(LX_PE_SHP_RE1_VER_T):
							case PE_ITEM_PKTFUNC(LX_PE_SHP_RE1_MISC_T):
							default:
								break;
						}
						break;
					case PE_ITEM_PKTMODL(CCM):
						switch (fn_t)
						{
							case PE_ITEM_PKTFUNC(LX_PE_CCM_AUTO_CR_T):
								PE_INF_M14_CPY_PARAM_TO_STR(LX_PE_CCM_AUTO_CR_T, \
									"%d,%d,%d,%d,%d", p.enable, p.th_max_hist, \
									p.th_valid_bins, p.adj_th0, p.adj_th1);
								break;
							#ifdef PE_INF_M14_DETAIL_TRACE_LOG
							case PE_ITEM_PKTFUNC(LX_PE_CCM_GAMMALUT_T):
								PE_INF_M14_CPY_PARAM_TO_STR(LX_PE_CCM_GAMMALUT_T, \
									"%d, %d", p.win_id, p.size);
								break;
							case PE_ITEM_PKTFUNC(LX_PE_CCM_PIXEL_REPLACE_T):
								PE_INF_M14_CPY_PARAM_TO_STR(LX_PE_CCM_PIXEL_REPLACE_T, \
									"%d, %d,%d,%d", p.win_id, \
									p.pxl_rep_r, p.pxl_rep_g, p.pxl_rep_b);
								break;
							case PE_ITEM_PKTFUNC(LX_PE_CCM_WB_T):
								PE_INF_M14_CPY_PARAM_TO_STR(LX_PE_CCM_WB_T, \
									"%d, %d,%d,%d, %d,%d,%d", p.win_id, \
									p.r_gain, p.g_gain, p.b_gain, \
									p.r_offset, p.g_offset, p.b_offset);
								break;
							#else
							case PE_ITEM_PKTFUNC(LX_PE_CCM_GAMMALUT_T):
							case PE_ITEM_PKTFUNC(LX_PE_CCM_PIXEL_REPLACE_T):
							case PE_ITEM_PKTFUNC(LX_PE_CCM_WB_T):
							#endif
							default:
								break;
						}
						break;
					case PE_ITEM_PKTMODL(DCM):
						switch (fn_t)
						{
							#ifdef PE_INF_M14_DETAIL_TRACE_LOG
							case PE_ITEM_PKTFUNC(LX_PE_DCM_DCE_CONF_T):
								PE_INF_M14_CPY_PARAM_TO_STR(LX_PE_DCM_DCE_CONF_T, \
									"%d, %d,%d,%d", p.win_id, \
									p.domain, p.min_pos, p.max_pos);
								break;
							case PE_ITEM_PKTFUNC(LX_PE_DCM_BLENDING_T):
								PE_INF_M14_CPY_PARAM_TO_STR(LX_PE_DCM_BLENDING_T, \
									"%d, %d,0x%x", p.win_id, \
									p.color_out_gain, p.region_sel);
								break;
							case PE_ITEM_PKTFUNC(LX_PE_DCM_DCE_SMOOTH0_T):
								PE_INF_M14_CPY_PARAM_TO_STR(LX_PE_DCM_DCE_SMOOTH0_T, \
									"%d", p.win_id);
								break;
							case PE_ITEM_PKTFUNC(LX_PE_DCM_DCE_SMOOTH1_T):
								PE_INF_M14_CPY_PARAM_TO_STR(LX_PE_DCM_DCE_SMOOTH1_T, \
									"%d", p.win_id);
								break;
							#else
							case PE_ITEM_PKTFUNC(LX_PE_DCM_DCE_CONF_T):
							case PE_ITEM_PKTFUNC(LX_PE_DCM_BLENDING_T):
							case PE_ITEM_PKTFUNC(LX_PE_DCM_DCE_SMOOTH0_T):
							case PE_ITEM_PKTFUNC(LX_PE_DCM_DCE_SMOOTH1_T):
							#endif
							case PE_ITEM_PKTFUNC(LX_PE_DCM_DCE_LUT_T):
							case PE_ITEM_PKTFUNC(LX_PE_DCM_DSE_LUT_T):
							case PE_ITEM_PKTFUNC(LX_PE_SHP_RE1_VER_T):
							case PE_ITEM_PKTFUNC(LX_PE_SHP_RE1_MISC_T):
							default:
								break;
						}
						break;
					case PE_ITEM_PKTMODL(HST):
						switch (fn_t)
						{
							#ifdef PE_INF_M14_DETAIL_TRACE_LOG
							case PE_ITEM_PKTFUNC(LX_PE_HST_HISTO_CFG_T):
								PE_INF_M14_CPY_PARAM_TO_STR(LX_PE_HST_HISTO_CFG_T, \
									"%d, %d,%d,%d", p.win_id, p.opr.src_apl_op, \
									p.opr.src_hist_op, p.opr.lrc_hist_op);
								break;
							#else
							case PE_ITEM_PKTFUNC(LX_PE_HST_HISTO_CFG_T):
							#endif
							default:
								break;
						}
						break;
					case PE_ITEM_PKTMODL(ETC):
						switch (fn_t)
						{
							case PE_ITEM_PKTFUNC(LX_PE_INF_DISPLAY_T):
								PE_INF_M14_CPY_PARAM_TO_STR(LX_PE_INF_DISPLAY_T, \
									"%d,sr,f,sc:%d,%d,%d,chsdh:%d,%d,%d,%d,%d,"\
									"oi:%d,%d,fr:%d(%d),hv:%d,%d,ae:%d,%d,ol:%d", \
									p.win_id, p.src_type, p.fmt_type, p.scan_type, p.cstd_type, \
									p.hdmi_type, p.scart_type, p.dtv_type,  p.hdd_type, \
									p.out_type, p.in_type, p.fr_type, p.in_f_rate, p.in_h_size, \
									p.in_v_size, p.act_fmt, p.mode.is_ext_frc, p.mode.is_oled);
								break;
							default:
								break;
						}
						break;
					case PE_ITEM_PKTMODL(INIT):
					case PE_ITEM_PKTMODL(DBG):
					case PE_ITEM_PKTMODL(WIN):
					default:
						break;
				}
				break;
			case PE_ITEM_PKTRW(GET):
			default:
				break;
		}
		if (valid)
		{
			ret = PE_INF_M14_SetTraceDataBuf(str);
			PE_INF_M14_CHECK_CODE(ret, break, \
				"PE_INF_M14_SetTraceDataBuf() error.\n");
		}
	}while (0);
	return ret;
}

