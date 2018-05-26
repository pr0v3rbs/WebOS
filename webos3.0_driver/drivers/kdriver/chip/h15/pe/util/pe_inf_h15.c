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

/** @file pe_inf_h15.c
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

#include "pe_hw_h15.h"
#include "pe_reg_h15.h"
#include "pe_inf_h15.h"
#include "pe_fwi_h15.h"
#include "pe_win_hw_h15.h"
#include "pe_shp_hw_h15.h"
#include "pe_ccm_hw_h15.h"
#include "pe_hst_hw_h15.h"
#include "pe_cmn_hw_h15.h"
#include "pe_dnt_hw_h15.h"
#include "pe_nrd_hw_h15.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
/* drv ver, yyyy/mm/dd/hh:mm */
#define PE_INF_H15_DRV_VER		"2014/12/22/10:30"

#define PE_INF_H15_BUF_MAX				100

#define PE_INF_H15_SCL_RESIZE_H_MAX		1920	/* 3840 * 0.5 */
#define PE_INF_H15_SCL_RESIZE_V_MAX		1080	/* 2160 * 0.5 */
#ifdef PE_INF_H15_USE_LEGACY_INFO
#define PE_INF_H15_SCL_IS_HD_MIN		1024
#endif

/* define for showing more detail trace log */
#undef PE_INF_H15_DETAIL_TRACE_LOG

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define PE_INF_H15_ERROR	printk

#define PE_INF_H15_DBG_PRINT(fmt,args...)	\
	if(_g_pe_inf_h15_trace) printk("[%x,%x][%s,%d] "fmt,PE_CHIP_VER,g_pe_kdrv_ver_mask,__F__,__L__,##args)
#define PE_INF_H15_CHECK_CODE(_checker,_action,fmt,args...)	\
	{if(_checker){PE_INF_H15_ERROR(fmt,##args);_action;}}

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
PE_INF_H15_TIME_T;

/**
 *	pe trace data buffer type
 */
typedef struct {
	PE_INF_H15_TIME_T time;			///< time
	char str[PE_TRACE_STR_SIZE];	///< str
}
PE_INF_H15_DATA_BUF_T;

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
static int PE_INF_H15_GetGeneralPicInfo(LX_PE_ETC_TBL_T *pstParams);
static int PE_INF_H15_SetPathMonitor(LX_PE_ETC_TBL_T *pstParams);
static int PE_INF_H15_GetPathMonitor(LX_PE_ETC_TBL_T *pstParams);
static void PE_INF_H15_SortTraceDataBuf(PE_INF_H15_DATA_BUF_T *dst);
static int PE_INF_H15_GetTraceDataBuf(LX_PE_ETC_TBL_T *pstParams);
static int PE_INF_H15_SetPixelRead(LX_PE_ETC_TBL_T *pstParams);
static int PE_INF_H15_GetPixelRead(LX_PE_ETC_TBL_T *pstParams);

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
static PE_INF_H15_DATA_BUF_T _g_pe_inf_h15_buf[PE_INF_H15_BUF_MAX];
static PE_INF_H15_DATA_BUF_T _g_pe_inf_h15_buf_temp[PE_INF_H15_BUF_MAX];

static UINT32 _g_pe_inf_h15_trace=0x0;		//default should be off.
static PE_INF_H15_SETTINGS_T _g_pe_inf_hw_h15_info;

static PE_ETC_MISC_TBL_T _g_pe_inf_h15_dbg_set_fn[] = {\
	{/* 00 */ PE_TSK_H15_SetTaskOperation,		"PE_TSK_H15_SetTaskOperation"}, \
	{/* 01 */ PE_REG_H15_WrReg,					"PE_REG_H15_WrReg"}, \
	{/* 02 */ PE_INF_H15_SetPathMonitor,		"PE_INF_H15_SetPathMonitor"}, \
	{/* 03 */ PE_INF_H15_SetPixelRead,			"PE_INF_H15_SetPixelRead"}
};
static PE_ETC_MISC_TBL_T _g_pe_inf_h15_dbg_get_fn[] = {\
	{/* 00 */ PE_TSK_H15_GetTaskOperation,		"PE_TSK_H15_GetTaskOperation"}, \
	{/* 01 */ PE_REG_H15_RdReg,					"PE_REG_H15_RdReg"}, \
	{/* 02 */ PE_INF_H15_GetGeneralPicInfo,		"PE_INF_H15_GetGeneralPicInfo"}, \
	{/* 03 */ PE_HST_HW_H15_GetGlobalHistInfo,	"PE_HST_HW_H15_GetGlobalHistInfo"}, \
	{/* 04 */ PE_INF_H15_GetPathMonitor,		"PE_INF_H15_GetPathMonitor"}, \
	{/* 05 */ PE_SHP_HW_H15_RdSclFltCoeff,		"PE_SHP_HW_H15_RdSclFltCoeff"}, \
	{/* 06 */ PE_DNT_HW_H15_RdDefault,			"PE_DNT_HW_H15_RdDefault"}, \
	{/* 07 */ PE_NRD_HW_H15_RdDefault,			"PE_NRD_HW_H15_RdDefault"}, \
	{/* 08 */ PE_SHP_HW_H15_RdDefault,			"PE_SHP_HW_H15_RdDefault"}, \
	{/* 09 */ PE_INF_H15_GetTraceDataBuf,		"PE_INF_H15_GetTraceDataBuf"}, \
	{/* 10 */ PE_INF_H15_GetPixelRead,			"PE_INF_H15_GetPixelRead"}
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
int PE_INF_H15_Init(PE_CFG_CTRL_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H15)
		{
			if (!pstParams->resume_mode)	//normal init
			{
				PE_INF_H15_DBG_PRINT("init global values.\n");
				memset0(_g_pe_inf_hw_h15_info);
			}
		}
		else
		{
			PE_INF_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
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
int PE_INF_H15_SetDebugSettings(LX_PE_DBG_SETTINGS_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		PE_INF_H15_DBG_PRINT("wid:%d,type:0x%x,[0x%x]print:0x%x,bypass:0x%x\n",\
			pstParams->win_id,pstParams->type,(0x1<<PE_ITEM_PKTMODL(ETC)),\
			pstParams->print_lvl,pstParams->bypass);
		/* set print level */
		if(pstParams->type&LX_PE_DBG_LV)
		{
			_g_pe_inf_h15_trace = (pstParams->print_lvl & (0x1<<PE_ITEM_PKTMODL(ETC)))? 0x1:0x0;
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
int PE_INF_H15_SetDisplayInfo(LX_PE_INF_DISPLAY_T *pstParams)
{
	int ret = RET_OK;
	PE_FWI_H15_SRC_INFO fwi_src;
	LX_PE_INF_DISPLAY_T *pp = NULL;
	LX_PE_INF_DISPLAY_T *disp0_info = &_g_pe_inf_hw_h15_info.disp_info[LX_PE_WIN_0];
	LX_PE_INF_DISPLAY_T *disp1_info = &_g_pe_inf_hw_h15_info.disp_info[LX_PE_WIN_1];
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		pp = pstParams;
		PE_INF_H15_DBG_PRINT("set[%d] : src:%d,res:%d (h:%d,v:%d)\n"\
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
		if(PE_KDRV_VER_H15)
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
				ret = PE_FWI_H15_SetSrcInfo(&fwi_src);
				PE_INF_H15_CHECK_CODE(ret,break,"[%s,%d] PE_FWI_H15_SetSrcInfo() error.\n",__F__,__L__);
			}
		}
		else
		{
			PE_INF_H15_DBG_PRINT("nothing to do\n");	ret = RET_OK;
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
int PE_INF_H15_GetDisplayInfo(LX_PE_INF_DISPLAY_T *pstParams)
{
	int ret = RET_OK;
	LX_PE_WIN_ID win_id,temp;
	PE_INF_H15_SETTINGS_T *pInfo=&_g_pe_inf_hw_h15_info;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		win_id = PE_GET_CHECKED_WINID(pstParams->win_id);
		if(PE_KDRV_VER_H15)
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
			PE_INF_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
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
int PE_INF_H15_CheckSclInfo(void)
{
	int ret = RET_OK;
	UINT32 r_data;
	PE_INF_H15_CTRL_T *ctrl0_info = &_g_pe_inf_hw_h15_info.ctrl_info[LX_PE_WIN_0];
	PE_INF_H15_CTRL_T *ctrl1_info = &_g_pe_inf_hw_h15_info.ctrl_info[LX_PE_WIN_1];
	LX_PE_INF_DISPLAY_T *disp0_info=&_g_pe_inf_hw_h15_info.disp_info[LX_PE_WIN_0];
	LX_PE_INF_DISPLAY_T *disp1_info=&_g_pe_inf_hw_h15_info.disp_info[LX_PE_WIN_1];
	PE_CCM_HW_H15_SETTINGS_T ccm_info;
	do{
		if(PE_KDRV_VER_H15)
		{
			ret = PE_CCM_HW_H15_GetCurCcmSettings(&ccm_info);
			if(ret)	{PE_INF_H15_ERROR("[%s,%d] PE_CCM_HW_H15_GetCurCcmSettings() error.\n",__F__,__L__);}
			#ifdef PE_INF_H15_USE_LEGACY_INFO
			/* L */
			PE_MSC_Y_H15A0_RdFL(msc_m1_y_crop_param3);
			PE_MSC_Y_H15A0_Rd01(msc_m1_y_crop_param3, reg_hisize_m1, ctrl0_info->scl_src_h);
			PE_MSC_Y_H15A0_Rd01(msc_m1_y_crop_param3, reg_visize_m1, ctrl0_info->scl_src_v);
			PE_MSC_Y_H15A0_RdFL(msc_m1_y_crop_param1);
			PE_MSC_Y_H15A0_Rd01(msc_m1_y_crop_param1, reg_hoffset_m1, ctrl0_info->i_win_h_ofst);
			PE_MSC_Y_H15A0_Rd01(msc_m1_y_crop_param1, reg_voffset_m1, ctrl0_info->i_win_v_ofst);
			PE_MSC_Y_H15A0_RdFL(msc_m1_y_crop_param2);
			PE_MSC_Y_H15A0_Rd01(msc_m1_y_crop_param2, reg_hactive_m1, ctrl0_info->i_win_h_size);
			PE_MSC_Y_H15A0_Rd01(msc_m1_y_crop_param2, reg_vactive_m1, ctrl0_info->i_win_v_size);
			PE_MSC_Y_H15A0_RdFL(msc_m1_y_outsize);
			PE_MSC_Y_H15A0_Rd01(msc_m1_y_outsize, reg_hosize_m1, ctrl0_info->o_win_h_size);
			PE_MSC_Y_H15A0_Rd01(msc_m1_y_outsize, reg_vosize_m1, ctrl0_info->o_win_v_size);
			ctrl0_info->scl_resize = (ctrl0_info->o_win_h_size<PE_INF_H15_SCL_RESIZE_H_MAX && \
								ctrl0_info->o_win_v_size<PE_INF_H15_SCL_RESIZE_V_MAX)? 1:0;
			ctrl0_info->scl_hd = (ctrl0_info->scl_src_h>=PE_INF_H15_SCL_IS_HD_MIN)? 1:0;
			/* R */
			PE_MSC_Y_H15A0_RdFL(msc_m2_y_crop_param3);
			PE_MSC_Y_H15A0_Rd01(msc_m2_y_crop_param3, reg_hisize_r, ctrl1_info->scl_src_h);
			PE_MSC_Y_H15A0_Rd01(msc_m2_y_crop_param3, reg_visize_r, ctrl1_info->scl_src_v);
			PE_MSC_Y_H15A0_RdFL(msc_m2_y_crop_param1);
			PE_MSC_Y_H15A0_Rd01(msc_m2_y_crop_param1, reg_hoffset_r, ctrl1_info->i_win_h_ofst);
			PE_MSC_Y_H15A0_Rd01(msc_m2_y_crop_param1, reg_voffset_r, ctrl1_info->i_win_v_ofst);
			PE_MSC_Y_H15A0_RdFL(msc_m2_y_crop_param2);
			PE_MSC_Y_H15A0_Rd01(msc_m2_y_crop_param2, reg_hactive_r, ctrl1_info->i_win_h_size);
			PE_MSC_Y_H15A0_Rd01(msc_m2_y_crop_param2, reg_vactive_r, ctrl1_info->i_win_v_size);
			PE_MSC_Y_H15A0_RdFL(msc_m2_y_outsize);
			PE_MSC_Y_H15A0_Rd01(msc_m2_y_outsize, reg_hosize_r, ctrl1_info->o_win_h_size);
			PE_MSC_Y_H15A0_Rd01(msc_m2_y_outsize, reg_vosize_r, ctrl1_info->o_win_v_size);
			ctrl1_info->scl_resize = (ctrl1_info->o_win_h_size<PE_INF_H15_SCL_RESIZE_H_MAX && \
								ctrl1_info->o_win_v_size<PE_INF_H15_SCL_RESIZE_V_MAX)? 1:0;
			ctrl1_info->scl_hd = (ctrl1_info->scl_src_h>=PE_INF_H15_SCL_IS_HD_MIN)? 1:0;
			/* sub */
			PE_SSC_H15A0_RdFL(ssc_scr_size);
			PE_SSC_H15A0_Rd01(ssc_scr_size,			input_h_size,		ctrl0_info->ssc_src_h);
			PE_SSC_H15A0_Rd01(ssc_scr_size,			input_v_size,		ctrl0_info->ssc_src_v);
			PE_SSC_H15A0_RdFL(ssc_win_in_offset);
			PE_SSC_H15A0_Rd01(ssc_win_in_offset,	in_win_h_offset,	ctrl0_info->ssc_i_win_x);
			PE_SSC_H15A0_Rd01(ssc_win_in_offset,	in_win_v_offset,	ctrl0_info->ssc_i_win_y);
			PE_SSC_H15A0_RdFL(ssc_win_in_size);
			PE_SSC_H15A0_Rd01(ssc_win_in_size,		in_win_h_size,		ctrl0_info->ssc_i_win_h);
			PE_SSC_H15A0_Rd01(ssc_win_in_size,		in_win_v_size,		ctrl0_info->ssc_i_win_v);
			PE_SSC_H15A0_RdFL(ssc_win_out_size);
			PE_SSC_H15A0_Rd01(ssc_win_out_size,		out_win_h_size,		ctrl0_info->ssc_o_win_h);
			PE_SSC_H15A0_Rd01(ssc_win_out_size,		out_win_v_size,		ctrl0_info->ssc_o_win_v);
			ctrl1_info->ssc_src_h	= ctrl0_info->ssc_src_h;
			ctrl1_info->ssc_src_v	= ctrl0_info->ssc_src_v;
			ctrl1_info->ssc_i_win_x	= ctrl0_info->ssc_i_win_x;
			ctrl1_info->ssc_i_win_y	= ctrl0_info->ssc_i_win_y;
			ctrl1_info->ssc_i_win_h	= ctrl0_info->ssc_i_win_h;
			ctrl1_info->ssc_i_win_v	= ctrl0_info->ssc_i_win_v;
			ctrl1_info->ssc_o_win_h	= ctrl0_info->ssc_o_win_h;
			ctrl1_info->ssc_o_win_v	= ctrl0_info->ssc_o_win_v;
			#endif
			/* L */
			ctrl0_info->u_i_win_h_ofst = disp0_info->in_win.x_ofst;
			ctrl0_info->u_i_win_v_ofst = disp0_info->in_win.y_ofst;
			ctrl0_info->u_i_win_h_size = disp0_info->in_win.h_size;
			ctrl0_info->u_i_win_v_size = disp0_info->in_win.v_size;
			ctrl0_info->u_o_win_h_ofst = disp0_info->out_win.x_ofst;
			ctrl0_info->u_o_win_v_ofst = disp0_info->out_win.y_ofst;
			ctrl0_info->u_o_win_h_size = disp0_info->out_win.h_size;
			ctrl0_info->u_o_win_v_size = disp0_info->out_win.v_size;
			PE_ND_ND1_H15A0_RdFL(pe0_src_size);
			PE_ND_ND1_H15A0_Rd01(pe0_src_size, cs_type, ctrl0_info->i_c_format);//0x6:444,0x5:422, else 420
			PE_ND_ND1_H15A0_Rd01(pe0_src_size, hsize, ctrl0_info->pe0_src_h);
			PE_ND_ND1_H15A0_Rd01(pe0_src_size, vsize, ctrl0_info->pe0_src_v);
			PE_ND_ND1_H15A0_Rd01(pe0_src_size, scan_type, ctrl0_info->i_sc_type);//1:p, 0:i
			/* R */
			ctrl1_info->u_i_win_h_ofst = disp1_info->in_win.x_ofst;
			ctrl1_info->u_i_win_v_ofst = disp1_info->in_win.y_ofst;
			ctrl1_info->u_i_win_h_size = disp1_info->in_win.h_size;
			ctrl1_info->u_i_win_v_size = disp1_info->in_win.v_size;
			ctrl1_info->u_o_win_h_ofst = disp1_info->out_win.x_ofst;
			ctrl1_info->u_o_win_v_ofst = disp1_info->out_win.y_ofst;
			ctrl1_info->u_o_win_h_size = disp1_info->out_win.h_size;
			ctrl1_info->u_o_win_v_size = disp1_info->out_win.v_size;
			PE_ND_ND2_H15A0_RdFL(pe0_src_size);
			PE_ND_ND2_H15A0_Rd01(pe0_src_size, cs_type, ctrl1_info->i_c_format);//0x6:444,0x5:422, else 420
			PE_ND_ND2_H15A0_Rd01(pe0_src_size, hsize, ctrl1_info->pe0_src_h);
			PE_ND_ND2_H15A0_Rd01(pe0_src_size, vsize, ctrl1_info->pe0_src_v);
			PE_ND_ND2_H15A0_Rd01(pe0_src_size, scan_type, ctrl1_info->i_sc_type);//1:p, 0:i
			/* etc */
			ctrl0_info->hv_same = (ctrl0_info->u_o_win_h_size==ctrl1_info->u_o_win_h_size && \
				ctrl0_info->u_o_win_v_size==ctrl1_info->u_o_win_v_size)? 1:0;
			ctrl1_info->hv_same = (ctrl0_info->u_o_win_h_size<PE_INF_H15_SCL_RESIZE_H_MAX && \
				ctrl0_info->u_o_win_v_size<PE_INF_H15_SCL_RESIZE_V_MAX)? 1:0;
			ret = PE_HST_HW_H15_GetDetected3dFmt(&r_data);
			if(ret)	{PE_INF_H15_ERROR("[%s,%d] PE_HST_HW_H15_GetDetected3dFmt() error.\n",__F__,__L__);}
			ctrl0_info->det_3df = r_data;
			ctrl0_info->auto_cr_en = ccm_info.auto_cr.enable? 1:0;
			ret = PE_HST_HW_H15_GetSaturationState(LX_PE_WIN_0,&r_data);
			if(ret)	{PE_INF_H15_ERROR("[%s,%d] PE_HST_HW_H15_GetSaturationState() error.\n",__F__,__L__);}
			ctrl0_info->sat_status = r_data;
			ret = PE_HST_HW_H15_GetSaturationState(LX_PE_WIN_1,&r_data);
			if(ret)	{PE_INF_H15_ERROR("[%s,%d] PE_HST_HW_H15_GetSaturationState() error.\n",__F__,__L__);}
			ctrl1_info->sat_status = r_data;
			PE_CE_PE1_H15A0_QRd01(pe1_mute_ctrl_00, pxl_rep_in,	ctrl0_info->mute_on);
			PE_CO_PE1_H15A0_QRd01(pe1_mute_ctrl_00, pxl_rep_in,	ctrl1_info->mute_on);
			PE_CE_PE1_H15A0_QRd01(pe1_fsw_ctrl_00,	reg_color_filter_status,ctrl0_info->pxl_rp_st);
			ctrl0_info->pmux_pos = 0;	//0:after,1:before shp, temp
			PE_CE_PE1_H15A0_RdFL(pe1_fsw_ctrl_02);
			PE_CE_PE1_H15A0_Rd01(pe1_fsw_ctrl_02,	reg_src_freeze,	ctrl0_info->is_src_frz);
			PE_CE_PE1_H15A0_Rd01(pe1_fsw_ctrl_02,	reg_src_info,	ctrl0_info->fw_src);
		}
		else
		{
			PE_INF_H15_DBG_PRINT("nothing to do\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * get current info.
 *
 * @param   *pstParams [in/out] PE_INF_H15_SETTINGS_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_INF_H15_GetCurInfSettings(PE_INF_H15_SETTINGS_T *pstParams)
{
	int ret = RET_OK;
	static UINT32 count = 0;
	PE_INF_H15_SETTINGS_T *pInfo = &_g_pe_inf_hw_h15_info;
	LX_PE_INF_DISPLAY_T *disp0_info = &_g_pe_inf_hw_h15_info.disp_info[LX_PE_WIN_0];
	LX_PE_INF_DISPLAY_T *disp1_info = &_g_pe_inf_hw_h15_info.disp_info[LX_PE_WIN_1];
	PE_INF_H15_CTRL_T *ctrl0_info = &_g_pe_inf_hw_h15_info.ctrl_info[LX_PE_WIN_0];
	PE_INF_H15_CTRL_T *ctrl1_info = &_g_pe_inf_hw_h15_info.ctrl_info[LX_PE_WIN_1];
	do{
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H15)
		{
			memcpy(pstParams,pInfo,sizeof(PE_INF_H15_SETTINGS_T));
			if(_g_pe_inf_h15_trace)
			{
				if(count>PE_PRINT_COUNT_NUM)
				{
					#ifdef PE_INF_H15_USE_LEGACY_INFO
					PE_INF_H15_DBG_PRINT("ctrl[0] : scl:%d,%d,i_w:%d,%d,%d,%d, scl_o_w:%d,%d\n",\
						ctrl0_info->scl_src_h,ctrl0_info->scl_src_v,\
						ctrl0_info->i_win_h_ofst,ctrl0_info->i_win_v_ofst,\
						ctrl0_info->i_win_h_size,ctrl0_info->i_win_v_size,\
						ctrl0_info->o_win_h_size,ctrl0_info->o_win_v_size);
					PE_INF_H15_DBG_PRINT("ctrl[0] : ssc src,i,o:%d,%d, %d,%d,%d,%d, %d,%d\n",\
						ctrl0_info->ssc_src_h,ctrl0_info->ssc_src_v,\
						ctrl0_info->ssc_i_win_x,ctrl0_info->ssc_i_win_y,\
						ctrl0_info->ssc_i_win_h,ctrl0_info->ssc_i_win_v,\
						ctrl0_info->ssc_o_win_h,ctrl0_info->ssc_o_win_v);
					PE_INF_H15_DBG_PRINT("ctrl[1] : scl:%d,%d,i_w:%d,%d,%d,%d, scl_o_w:%d,%d\n",\
						ctrl1_info->scl_src_h,ctrl1_info->scl_src_v,\
						ctrl1_info->i_win_h_ofst,ctrl1_info->i_win_v_ofst,\
						ctrl1_info->i_win_h_size,ctrl1_info->i_win_v_size,\
						ctrl1_info->o_win_h_size,ctrl1_info->o_win_v_size);
					PE_INF_H15_DBG_PRINT("ctrl[1] : ssc src,i,o:%d,%d, %d,%d,%d,%d, %d,%d\n",\
						ctrl1_info->ssc_src_h,ctrl1_info->ssc_src_v,\
						ctrl1_info->ssc_i_win_x,ctrl1_info->ssc_i_win_y,\
						ctrl1_info->ssc_i_win_h,ctrl1_info->ssc_i_win_v,\
						ctrl1_info->ssc_o_win_h,ctrl1_info->ssc_o_win_v);
					#endif
					PE_INF_H15_DBG_PRINT("disp[0] : src:%d,fmt:%d (h:%d,v:%d)\n", \
						disp0_info->src_type, disp0_info->fmt_type, \
						disp0_info->in_h_size, disp0_info->in_v_size);
					PE_INF_H15_DBG_PRINT("disp[0] : fr:%d(%d),scan:%d,cstd,hdmi,scart,dtv,hdd:%d,%d,%d,%d,%d\n", \
						disp0_info->fr_type, disp0_info->in_f_rate, disp0_info->scan_type, \
						disp0_info->cstd_type, disp0_info->hdmi_type, disp0_info->scart_type, \
						disp0_info->dtv_type, disp0_info->hdd_type);
					PE_INF_H15_DBG_PRINT("disp[0] : out:%d,in:%d,user_o:%d,user_i:%d\n", \
						disp0_info->out_type, disp0_info->in_type, disp0_info->user_o_type, \
						disp0_info->user_i_type);
					PE_INF_H15_DBG_PRINT("disp[0] : in_win:%d,%d,%d,%d,out_win:%d,%d,%d,%d,af:%d\n", \
						disp0_info->in_win.x_ofst, disp0_info->in_win.y_ofst, \
						disp0_info->in_win.h_size, disp0_info->in_win.v_size, \
						disp0_info->out_win.x_ofst, disp0_info->out_win.y_ofst, \
						disp0_info->out_win.h_size, disp0_info->out_win.v_size, disp0_info->act_fmt);
					PE_INF_H15_DBG_PRINT("disp[0] : tp,venc,adp,ext,wog,rev,oled:%d,%d,%d,%d,%d,%d,%d\n", \
						disp0_info->mode.is_tp, disp0_info->mode.is_venc, \
						disp0_info->mode.is_adpt_st, disp0_info->mode.is_ext_frc, \
						disp0_info->mode.is_wb_wog, disp0_info->mode.is_reverse, \
						disp0_info->mode.is_oled);
					PE_INF_H15_DBG_PRINT("ctrl[0] : src:%d,%d,sc:%d, u_w i:%d,%d,%d,%d, o:%d,%d,%d,%d\n",\
						ctrl0_info->pe0_src_h,ctrl0_info->pe0_src_v,ctrl0_info->i_sc_type,\
						ctrl0_info->u_i_win_h_ofst,ctrl0_info->u_i_win_v_ofst,\
						ctrl0_info->u_i_win_h_size,ctrl0_info->u_i_win_v_size,\
						ctrl0_info->u_o_win_h_ofst,ctrl0_info->u_o_win_v_ofst,\
						ctrl0_info->u_o_win_h_size,ctrl0_info->u_o_win_v_size);
					PE_INF_H15_DBG_PRINT("ctrl[0] : c_fmt:%d,det_3df:%d,acr_en:%d,s_stat:%d,mute_on:%d,hv_same:%d\n",\
						ctrl0_info->i_c_format,ctrl0_info->det_3df,\
						ctrl0_info->auto_cr_en,ctrl0_info->sat_status,\
						ctrl0_info->mute_on,ctrl0_info->hv_same);
					PE_INF_H15_DBG_PRINT("ctrl[0] : pxl_rp,s_frz,pmux,f_s:%d,%d,%d,%d\n",\
						ctrl0_info->pxl_rp_st,ctrl0_info->is_src_frz,ctrl0_info->pmux_pos,ctrl0_info->fw_src);
					PE_INF_H15_DBG_PRINT("disp[1] : src:%d,fmt:%d (h:%d,v:%d)\n", \
						disp1_info->src_type, disp1_info->fmt_type, \
						disp1_info->in_h_size, disp1_info->in_v_size);
					PE_INF_H15_DBG_PRINT("disp[1] : fr:%d(%d),scan:%d,cstd,hdmi,scart,dtv,hdd:%d,%d,%d,%d,%d\n", \
						disp1_info->fr_type, disp1_info->in_f_rate, disp1_info->scan_type, \
						disp1_info->cstd_type, disp1_info->hdmi_type, disp1_info->scart_type, \
						disp1_info->dtv_type, disp1_info->hdd_type);
					PE_INF_H15_DBG_PRINT("disp[1] : out:%d,in:%d,user_o:%d,user_i:%d\n", \
						disp1_info->out_type, disp1_info->in_type, disp1_info->user_o_type, \
						disp1_info->user_i_type);
					PE_INF_H15_DBG_PRINT("disp[1] : in_win:%d,%d,%d,%d,out_win:%d,%d,%d,%d,af:%d\n", \
						disp1_info->in_win.x_ofst, disp1_info->in_win.y_ofst, \
						disp1_info->in_win.h_size, disp1_info->in_win.v_size, \
						disp1_info->out_win.x_ofst, disp1_info->out_win.y_ofst, \
						disp1_info->out_win.h_size, disp1_info->out_win.v_size, disp1_info->act_fmt);
					PE_INF_H15_DBG_PRINT("disp[1] : tp,venc,adp,ext,wog,rev,oled:%d,%d,%d,%d,%d,%d,%d\n", \
						disp1_info->mode.is_tp, disp1_info->mode.is_venc, \
						disp1_info->mode.is_adpt_st, disp1_info->mode.is_ext_frc, \
						disp1_info->mode.is_wb_wog, disp1_info->mode.is_reverse, \
						disp0_info->mode.is_oled);
					PE_INF_H15_DBG_PRINT("ctrl[1] : src:%d,%d,sc:%d, u_w i:%d,%d,%d,%d, o:%d,%d,%d,%d\n",\
						ctrl1_info->pe0_src_h,ctrl1_info->pe0_src_v,ctrl1_info->i_sc_type,\
						ctrl1_info->u_i_win_h_ofst,ctrl1_info->u_i_win_v_ofst,\
						ctrl1_info->u_i_win_h_size,ctrl1_info->u_i_win_v_size,\
						ctrl1_info->u_o_win_h_ofst,ctrl1_info->u_o_win_v_ofst,\
						ctrl1_info->u_o_win_h_size,ctrl1_info->u_o_win_v_size);
					PE_INF_H15_DBG_PRINT("ctrl[1] : c_fmt:%d,det_3df:%d,acr_en:%d,s_stat:%d,mute_on:%d,hv_same:%d\n",\
						ctrl1_info->i_c_format,ctrl1_info->det_3df,\
						ctrl1_info->auto_cr_en,ctrl1_info->sat_status,\
						ctrl1_info->mute_on,ctrl1_info->hv_same);
					PE_INF_H15_DBG_PRINT("ctrl[1] : pxl_rp,s_frz,pmux,f_s:%d,%d,%d,%d\n",\
						ctrl1_info->pxl_rp_st,ctrl1_info->is_src_frz,ctrl1_info->pmux_pos,ctrl1_info->fw_src);
					PE_INF_H15_DBG_PRINT("\n");
					count=0;
				}
				count++;
			}
		}
		else
		{
			PE_INF_H15_DBG_PRINT("nothing to do.\n");
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
static int PE_INF_H15_GetGeneralPicInfo(LX_PE_ETC_TBL_T *pstParams)
{
    int ret = RET_OK;
	char buffer[PE_TRACE_STR_SIZE];
	static UINT32 rd_cnt=0;
	PE_WIN_HW_H15_SETTINGS_T *win_set = NULL;
	PE_SHP_HW_H15_SETTINGS_T *shp_set = NULL;
	PE_CCM_HW_H15_SETTINGS_T *ccm_set = NULL;
	static PE_DDR_H15_TOP_HEADER_T *p_h_top = NULL;
	static PE_DDR_H15_DATA_T *p_sec_data = NULL;
	LX_PE_INF_DISPLAY_T *disp0_inf = &(_g_pe_inf_hw_h15_info.disp_info[LX_PE_WIN_0]);
	LX_PE_INF_DISPLAY_T *disp1_inf = &(_g_pe_inf_hw_h15_info.disp_info[LX_PE_WIN_1]);
	PE_INF_H15_CTRL_T *ctrl0_inf = &(_g_pe_inf_hw_h15_info.ctrl_info[LX_PE_WIN_0]);
	PE_INF_H15_CTRL_T *ctrl1_inf = &(_g_pe_inf_hw_h15_info.ctrl_info[LX_PE_WIN_1]);

#define WIN0	LX_PE_WIN_0
#define WIN1	LX_PE_WIN_1
#define PE_INF_H15_PRINT_START		"START_OF_PRINT"
#define PE_INF_H15_PRINT_RESERVED	"RESERVED"
#define PE_INF_H15_PRINT_END		"END_OF_PRINT"
#define PE_INF_H15_PRINT_EXIT		"EXIT_OF_PRINT"
#define PE_INF_H15_CASE_PRMENU(_cnt, _action, _buf, _item)	\
	case (_cnt):sprintf(_buf, "   [0x%02x] %s\n", (_item), #_item);_action;break
#define PE_INF_H15_CASE_SPRINT(_cnt,_action,_buf,fmt,args...)	\
	case (_cnt):sprintf(_buf,fmt,##args);_action;break
#define PE_INF_H15_DFLT_SPRINT(_action,_buf,fmt,args...)		\
	default:sprintf(_buf,fmt,##args);_action;break

	do{
		CHECK_KNULL(pstParams);
		CHECK_KNULL(pstParams->data);
		if(PE_KDRV_VER_H15)
		{
			if (pstParams->data[0]==PE_INF_HW_H15A_GINFO_DISPLAY)
			{
				switch (rd_cnt)
				{
					PE_INF_H15_CASE_SPRINT( 0, rd_cnt++, buffer, PE_INF_H15_PRINT_START);
					PE_INF_H15_CASE_PRMENU( 1, rd_cnt++, buffer, PE_INF_HW_H15A_GINFO__KDRV_INFO);
					PE_INF_H15_CASE_PRMENU( 2, rd_cnt++, buffer, PE_INF_HW_H15A_GINFO_DDR_H_PHYS);
					PE_INF_H15_CASE_PRMENU( 3, rd_cnt++, buffer, PE_INF_HW_H15A_GINFO_DDR_H_SHDW);
					PE_INF_H15_CASE_SPRINT( 4, rd_cnt++, buffer, PE_INF_H15_PRINT_RESERVED);
					PE_INF_H15_CASE_SPRINT( 5, rd_cnt++, buffer, PE_INF_H15_PRINT_RESERVED);
					PE_INF_H15_DFLT_SPRINT(rd_cnt=0, buffer, PE_INF_H15_PRINT_END);
				}
			}
			else if (pstParams->data[0]==PE_INF_HW_H15A_GINFO__KDRV_INFO)
			{
				win_set = (PE_WIN_HW_H15_SETTINGS_T *)OS_Malloc(sizeof(PE_WIN_HW_H15_SETTINGS_T));
				PE_INF_H15_CHECK_CODE(!win_set, break, "[%s,%d] win_set is null.\n", __F__, __L__);
				shp_set = (PE_SHP_HW_H15_SETTINGS_T *)OS_Malloc(sizeof(PE_SHP_HW_H15_SETTINGS_T));
				PE_INF_H15_CHECK_CODE(!shp_set, break, "[%s,%d] shp_set is null.\n", __F__, __L__);
				ccm_set = (PE_CCM_HW_H15_SETTINGS_T *)OS_Malloc(sizeof(PE_CCM_HW_H15_SETTINGS_T));
				PE_INF_H15_CHECK_CODE(!ccm_set, break, "[%s,%d] ccm_set is null.\n", __F__, __L__);
				/* get win settings */
				ret = PE_WIN_HW_H15_GetCurWinSettings(win_set);
				PE_INF_H15_CHECK_CODE(ret,break,\
					"[%s,%d] PE_WIN_HW_H15_GetCurWinSettings() error.\n",__F__,__L__);
				/* get shp settings */
				ret = PE_SHP_HW_H15_GetCurShpSettings(shp_set);
				PE_INF_H15_CHECK_CODE(ret,break,\
					"[%s,%d] PE_SHP_HW_H15_GetCurShpSettings() error.\n",__F__,__L__);
				/* get ccm settings */
				ret = PE_CCM_HW_H15_GetCurCcmSettings(ccm_set);
				PE_INF_H15_CHECK_CODE(ret,break,\
					"[%s,%d] PE_CCM_HW_H15_GetCurCcmSettings() error.\n",__F__,__L__);
				switch(rd_cnt)
				{
					PE_INF_H15_CASE_SPRINT( 0,rd_cnt++,buffer,PE_INF_H15_PRINT_START);
					PE_INF_H15_CASE_SPRINT( 1,rd_cnt++,buffer,\
						"---------------------------------------------------------------------------\n");
					PE_INF_H15_CASE_SPRINT( 2,rd_cnt++,buffer,\
						"src       : 0~6:dtv,atv,cvbs,scart,comp,rgbpc,hdmi\n");
					PE_INF_H15_CASE_SPRINT( 3,rd_cnt++,buffer,\
						"fmt/fr    : 0~2:sd,hd,uhd / 0~1:60hz,50hz\n");
					PE_INF_H15_CASE_SPRINT( 4,rd_cnt++,buffer,\
						"scan/cstd : 0~1:i,p / 0~2:ntsc,pal,secam\n");
					PE_INF_H15_CASE_SPRINT( 5,rd_cnt++,buffer,\
						"hdmi/scart: 0~1:tv,pc / 0~1:av,rgb\n");
					PE_INF_H15_CASE_SPRINT( 6,rd_cnt++,buffer,\
						"dtv       : 0:normal,1:file,2:hdd,3:photo,4:tp,5:camera,6:invalid\n");
					PE_INF_H15_CASE_SPRINT( 7,rd_cnt++,buffer,\
						"hdd       : 0:dtv,1:atv60,2:atv50,3:av60,4:av50,5:scartrgb,6:invalid\n");
					PE_INF_H15_CASE_SPRINT( 8,rd_cnt++,buffer,\
						"out       : 0~3:2d,3_2d,2_3d,ud, 4~6:tb,ss,fs, 7~9:dual_t,s,f, 10:pip\n");
					PE_INF_H15_CASE_SPRINT( 9,rd_cnt++,buffer,\
						"in        : 0tb,ss,q,cb,fp,5fi,fa,fs,la,9ssf,dua,ca,lah\n");
					PE_INF_H15_CASE_SPRINT(10,rd_cnt++,buffer,\
						"---------------------------------------------------------------------------\n");
					PE_INF_H15_CASE_SPRINT(11,rd_cnt++,buffer,\
						"[%02d]disp0: wid:%d,src:%d,fmt:%d (h:%d,v:%d)\n",rd_cnt,\
						disp0_inf->win_id,disp0_inf->src_type,disp0_inf->fmt_type,\
						disp0_inf->in_h_size,disp0_inf->in_v_size);
					PE_INF_H15_CASE_SPRINT(12,rd_cnt++,buffer,\
						"[%02d]disp0: fr:%d(%d),scan:%d,cstd:%d,hdmi:%d,scart:%d,dtv:%d,hdd:%d\n",rd_cnt,\
						disp0_inf->fr_type,disp0_inf->in_f_rate,disp0_inf->scan_type,disp0_inf->cstd_type,\
						disp0_inf->hdmi_type,disp0_inf->scart_type,disp0_inf->dtv_type,disp0_inf->hdd_type);
					PE_INF_H15_CASE_SPRINT(13,rd_cnt++,buffer,\
						"[%02d]disp0: out:%d,sub:%d,in:%d,user_o:%d,user_i:%d\n",rd_cnt,\
						disp0_inf->out_type,disp0_inf->out_sub,disp0_inf->in_type,\
						disp0_inf->user_o_type,disp0_inf->user_i_type);
					PE_INF_H15_CASE_SPRINT(14,rd_cnt++,buffer,\
						"[%02d]disp0: in_win:%d,%d,%d,%d,out_win:%d,%d,%d,%d,af:%d\n",rd_cnt,\
						disp0_inf->in_win.x_ofst,disp0_inf->in_win.y_ofst,disp0_inf->in_win.h_size,\
						disp0_inf->in_win.v_size,disp0_inf->out_win.x_ofst,disp0_inf->out_win.y_ofst,\
						disp0_inf->out_win.h_size,disp0_inf->out_win.v_size,disp0_inf->act_fmt);
					PE_INF_H15_CASE_SPRINT(15,rd_cnt++,buffer,\
						"[%02d]disp0: tp,venc,adp,ext,wog,rev,oled:%d,%d,%d,%d,%d,%d,%d\n",rd_cnt,\
						disp0_inf->mode.is_tp,disp0_inf->mode.is_venc,disp0_inf->mode.is_adpt_st,\
						disp0_inf->mode.is_ext_frc,disp0_inf->mode.is_wb_wog,\
						disp0_inf->mode.is_reverse,disp0_inf->mode.is_oled);
					PE_INF_H15_CASE_SPRINT(16,rd_cnt++,buffer,\
						"[%02d]disp1: wid:%d,src:%d,fmt:%d (h:%d,v:%d)\n",rd_cnt,\
						disp1_inf->win_id,disp1_inf->src_type,disp1_inf->fmt_type,\
						disp1_inf->in_h_size,disp1_inf->in_v_size);
					PE_INF_H15_CASE_SPRINT(17,rd_cnt++,buffer,\
						"[%02d]disp1: fr:%d(%d),scan:%d,cstd:%d,hdmi:%d,scart:%d,dtv:%d,hdd:%d\n",rd_cnt,\
						disp1_inf->fr_type,disp1_inf->in_f_rate,disp1_inf->scan_type,disp1_inf->cstd_type,\
						disp1_inf->hdmi_type,disp1_inf->scart_type,disp1_inf->dtv_type,disp1_inf->hdd_type);
					PE_INF_H15_CASE_SPRINT(18,rd_cnt++,buffer,\
						"[%02d]disp1: out:%d,sub:%d,in:%d,user_o:%d,user_i:%d\n",rd_cnt,\
						disp1_inf->out_type,disp1_inf->out_sub,disp1_inf->in_type,\
						disp1_inf->user_o_type,disp1_inf->user_i_type);
					PE_INF_H15_CASE_SPRINT(19,rd_cnt++,buffer,\
						"[%02d]disp1: in_win:%d,%d,%d,%d,out_win:%d,%d,%d,%d,af:%d\n",rd_cnt,\
						disp1_inf->in_win.x_ofst,disp1_inf->in_win.y_ofst,disp1_inf->in_win.h_size,\
						disp1_inf->in_win.v_size,disp1_inf->out_win.x_ofst,disp1_inf->out_win.y_ofst,\
						disp1_inf->out_win.h_size,disp1_inf->out_win.v_size,disp1_inf->act_fmt);
					PE_INF_H15_CASE_SPRINT(20,rd_cnt++,buffer,\
						"[%02d]disp1: tp,venc,adp,ext,wog,rev,oled:%d,%d,%d,%d,%d,%d,%d\n",rd_cnt,\
						disp1_inf->mode.is_tp,disp1_inf->mode.is_venc,disp1_inf->mode.is_adpt_st,\
						disp1_inf->mode.is_ext_frc,disp1_inf->mode.is_wb_wog,\
						disp1_inf->mode.is_reverse,disp1_inf->mode.is_oled);
					PE_INF_H15_CASE_SPRINT(21,rd_cnt++,buffer,\
						"---------------------------------------------------------------------------\n");
					PE_INF_H15_CASE_SPRINT(22,rd_cnt++,buffer,\
						"det_3df: 0:2d,1:cb,2,cc,3:ll,4:ss,5:tb,6:unknown,7:max\n");
					PE_INF_H15_CASE_SPRINT(23,rd_cnt++,buffer,\
						"---------------------------------------------------------------------------\n");
					PE_INF_H15_CASE_SPRINT(24,rd_cnt++,buffer,"[%02d]ctrl0:\n",rd_cnt);
					PE_INF_H15_CASE_SPRINT(25,rd_cnt++,buffer,\
						"[%02d]pe0_src_h,v:%d,%d, i_sc:%d\n",rd_cnt,\
						ctrl0_inf->pe0_src_h,ctrl0_inf->pe0_src_v,ctrl0_inf->i_sc_type);
					PE_INF_H15_CASE_SPRINT(26,rd_cnt++,buffer,\
						"[%02d]u_win i:%d,%d,%d,%d, o:%d,%d,%d,%d\n",rd_cnt,\
						ctrl0_inf->u_i_win_h_ofst,ctrl0_inf->u_i_win_v_ofst,\
						ctrl0_inf->u_i_win_h_size,ctrl0_inf->u_i_win_v_size,\
						ctrl0_inf->u_o_win_h_ofst,ctrl0_inf->u_o_win_v_ofst,\
						ctrl0_inf->u_o_win_h_size,ctrl0_inf->u_o_win_v_size);
					PE_INF_H15_CASE_SPRINT(27,rd_cnt++,buffer,\
						"[%02d]c_fmt:%d,det_3df:%d,auto_en:%d,s_state:%d,mute_on:%d,hv_same:%d\n",rd_cnt,\
						ctrl0_inf->i_c_format,ctrl0_inf->det_3df,ctrl0_inf->auto_cr_en,\
						ctrl0_inf->sat_status,ctrl0_inf->mute_on,ctrl0_inf->hv_same);
					PE_INF_H15_CASE_SPRINT(28,rd_cnt++,buffer,\
						"[%02d]pxl_rp,s_frz,pmux,f_s:%d,%d,%d,%d\n",rd_cnt,\
						ctrl0_inf->pxl_rp_st,ctrl0_inf->is_src_frz,\
						ctrl0_inf->pmux_pos,ctrl0_inf->fw_src);
					PE_INF_H15_CASE_SPRINT(29,rd_cnt++,buffer,PE_INF_H15_PRINT_RESERVED);
					PE_INF_H15_CASE_SPRINT(30,rd_cnt++,buffer,PE_INF_H15_PRINT_RESERVED);
					PE_INF_H15_CASE_SPRINT(31,rd_cnt++,buffer,"[%02d]ctrl1:\n",rd_cnt);
					PE_INF_H15_CASE_SPRINT(32,rd_cnt++,buffer,\
						"[%02d]pe0_src_h,v:%d,%d, i_sc:%d\n",rd_cnt,\
						ctrl1_inf->pe0_src_h,ctrl1_inf->pe0_src_v,ctrl1_inf->i_sc_type);
					PE_INF_H15_CASE_SPRINT(33,rd_cnt++,buffer,\
						"[%02d]u_win i:%d,%d,%d,%d, o:%d,%d,%d,%d\n",rd_cnt,\
						ctrl1_inf->u_i_win_h_ofst,ctrl1_inf->u_i_win_v_ofst,\
						ctrl1_inf->u_i_win_h_size,ctrl1_inf->u_i_win_v_size,\
						ctrl1_inf->u_o_win_h_ofst,ctrl1_inf->u_o_win_v_ofst,\
						ctrl1_inf->u_o_win_h_size,ctrl1_inf->u_o_win_v_size);
					PE_INF_H15_CASE_SPRINT(34,rd_cnt++,buffer,\
						"[%02d]c_fmt:%d,det_3df:%d,auto_en:%d,s_state:%d,mute_on:%d,hv_same:%d\n",rd_cnt,\
						ctrl1_inf->i_c_format,ctrl1_inf->det_3df,ctrl1_inf->auto_cr_en,\
						ctrl1_inf->sat_status,ctrl1_inf->mute_on,ctrl1_inf->hv_same);
					PE_INF_H15_CASE_SPRINT(35,rd_cnt++,buffer,\
						"[%02d]pxl_rp,s_frz,pmux,f_s:%d,%d,%d,%d\n",rd_cnt,\
						ctrl1_inf->pxl_rp_st,ctrl1_inf->is_src_frz,\
						ctrl1_inf->pmux_pos,ctrl1_inf->fw_src);
					#ifdef PE_INF_H15_USE_LEGACY_INFO
					PE_INF_H15_CASE_SPRINT(36,rd_cnt++,buffer,\
						"[%02d]ctrl0:scl_src_h,v:%d,%d,resize:%d,scl_hd:%d,i_win:%d,%d,%d,%d,o_win:%d,%d\n",rd_cnt,\
						ctrl0_inf->scl_src_h,ctrl0_inf->scl_src_v,ctrl0_inf->scl_resize,ctrl0_inf->scl_hd,\
						ctrl0_inf->i_win_h_ofst,ctrl0_inf->i_win_v_ofst,ctrl0_inf->i_win_h_size,\
						ctrl0_inf->i_win_v_size,ctrl0_inf->o_win_h_size,ctrl0_inf->o_win_v_size);
					PE_INF_H15_CASE_SPRINT(37,rd_cnt++,buffer,\
						"[%02d]ctrl0:ssc_src,i,o_win:%d,%d, %d,%d,%d,%d, %d,%d\n",rd_cnt,\
						ctrl0_inf->ssc_src_h,ctrl0_inf->ssc_src_v,ctrl0_inf->ssc_i_win_x,ctrl0_inf->ssc_i_win_y,\
						ctrl0_inf->ssc_i_win_h,ctrl0_inf->ssc_i_win_v,ctrl0_inf->ssc_o_win_h,\
						ctrl0_inf->ssc_o_win_v);
					PE_INF_H15_CASE_SPRINT(38,rd_cnt++,buffer,\
						"[%02d]ctrl1:scl_src_h,v:%d,%d,resize:%d,scl_hd:%d,i_win:%d,%d,%d,%d,o_win:%d,%d\n",rd_cnt,\
						ctrl1_inf->scl_src_h,ctrl1_inf->scl_src_v,ctrl1_inf->scl_resize,ctrl1_inf->scl_hd,\
						ctrl1_inf->i_win_h_ofst,ctrl1_inf->i_win_v_ofst,ctrl1_inf->i_win_h_size,\
						ctrl1_inf->i_win_v_size,ctrl1_inf->o_win_h_size,ctrl1_inf->o_win_v_size);
					PE_INF_H15_CASE_SPRINT(39,rd_cnt++,buffer,\
						"[%02d]ctrl1:ssc_src,i,o_win:%d,%d, %d,%d,%d,%d, %d,%d\n",rd_cnt,\
						ctrl1_inf->ssc_src_h,ctrl1_inf->ssc_src_v,ctrl1_inf->ssc_i_win_x,ctrl1_inf->ssc_i_win_y,\
						ctrl1_inf->ssc_i_win_h,ctrl1_inf->ssc_i_win_v,ctrl1_inf->ssc_o_win_h,\
						ctrl1_inf->ssc_o_win_v);
					#else
					PE_INF_H15_CASE_SPRINT(36,rd_cnt++,buffer,PE_INF_H15_PRINT_RESERVED);
					PE_INF_H15_CASE_SPRINT(37,rd_cnt++,buffer,PE_INF_H15_PRINT_RESERVED);
					PE_INF_H15_CASE_SPRINT(38,rd_cnt++,buffer,PE_INF_H15_PRINT_RESERVED);
					PE_INF_H15_CASE_SPRINT(39,rd_cnt++,buffer,PE_INF_H15_PRINT_RESERVED);
					#endif
					PE_INF_H15_CASE_SPRINT(40,rd_cnt++,buffer,PE_INF_H15_PRINT_RESERVED);
					PE_INF_H15_CASE_SPRINT(41,rd_cnt++,buffer,"[%02d]win:\n",rd_cnt);
					PE_INF_H15_CASE_SPRINT(42,rd_cnt++,buffer,\
						"[%02d]act0:%d,%d,%d,%d, act1:%d,%d,%d,%d\n",rd_cnt,\
						win_set->win_inf.act0_x0,win_set->win_inf.act0_y0,win_set->win_inf.act0_x1,\
						win_set->win_inf.act0_y1,win_set->win_inf.act1_x0,win_set->win_inf.act1_y0,\
						win_set->win_inf.act1_x1,win_set->win_inf.act1_y1);
					PE_INF_H15_CASE_SPRINT(43,rd_cnt++,buffer,\
						"[%02d]win0:%d,%d,%d,%d, win1:%d,%d,%d,%d\n",rd_cnt,\
						win_set->win_inf.win0_x0,win_set->win_inf.win0_y0,win_set->win_inf.win0_x1,\
						win_set->win_inf.win0_y1,win_set->win_inf.win1_x0,win_set->win_inf.win1_y0,\
						win_set->win_inf.win1_x1,win_set->win_inf.win1_y1);
					PE_INF_H15_CASE_SPRINT(44,rd_cnt++,buffer,\
						"[%02d]h,v_max:%d,%d, w0,w1,w01_en:[%d,%d],[%d,%d],[%d,%d]\n",rd_cnt,\
						win_set->win_inf.h_max,win_set->win_inf.v_max,win_set->win_inf.win0_en[0],\
						win_set->win_inf.win0_en[1],win_set->win_inf.win1_en[0],win_set->win_inf.win1_en[1],\
						win_set->win_inf.win01_en[0],win_set->win_inf.win01_en[1]);
					PE_INF_H15_CASE_SPRINT(45,rd_cnt++,buffer,\
						"[%02d]lrcr histo_l,r, apply_r : %d,%d,%d,%d, %d,%d,%d,%d, %d,%d,%d,%d\n",rd_cnt,\
						win_set->lrcr_inf.histo_l_x0,win_set->lrcr_inf.histo_l_y0,win_set->lrcr_inf.histo_l_x1,\
						win_set->lrcr_inf.histo_l_y1,win_set->lrcr_inf.histo_r_x0,win_set->lrcr_inf.histo_r_y0,\
						win_set->lrcr_inf.histo_r_x1,win_set->lrcr_inf.histo_r_y1,win_set->lrcr_inf.apply_r_x0,\
						win_set->lrcr_inf.apply_r_y0,win_set->lrcr_inf.apply_r_x1,win_set->lrcr_inf.apply_r_y1);
					PE_INF_H15_CASE_SPRINT(46,rd_cnt++,buffer,\
						"[%02d]win: tnr win h,v size0:%d,%d, size1:%d,%d\n",rd_cnt,\
						win_set->tnr_inf.h_size0,win_set->tnr_inf.v_size0,\
						win_set->tnr_inf.h_size1,win_set->tnr_inf.v_size1);
					PE_INF_H15_CASE_SPRINT(47,rd_cnt++,buffer,PE_INF_H15_PRINT_RESERVED);
					PE_INF_H15_CASE_SPRINT(48,rd_cnt++,buffer,PE_INF_H15_PRINT_RESERVED);
					PE_INF_H15_CASE_SPRINT(49,rd_cnt++,buffer,PE_INF_H15_PRINT_RESERVED);
					PE_INF_H15_CASE_SPRINT(50,rd_cnt++,buffer,PE_INF_H15_PRINT_RESERVED);
					PE_INF_H15_CASE_SPRINT(51,rd_cnt++,buffer,"[%02d]shp:\n",rd_cnt);
					PE_INF_H15_CASE_SPRINT(52,rd_cnt++,buffer,\
						"[%02d]user_idx gsc:%d,%d,%d,%d, psc:%d,%d\n",rd_cnt,\
						shp_set->user_gsc_idx.h_y_indx,shp_set->user_gsc_idx.v_y_indx,\
						shp_set->user_gsc_idx.h_c_indx,shp_set->user_gsc_idx.v_c_indx,\
						shp_set->user_psc_idx.h_y_indx,shp_set->user_psc_idx.v_y_indx);
					PE_INF_H15_CASE_SPRINT(53,rd_cnt++,buffer,\
						"[%02d]cur m1_y id:%d,%d,%d,%d, i:%d,%d, a:%d,%d, o:%d,%d\n",rd_cnt,\
						shp_set->cur_info.m1_y.h_y_indx,shp_set->cur_info.m1_y.v_y_indx,\
						shp_set->cur_info.m1_y.h_c_indx,shp_set->cur_info.m1_y.v_c_indx,\
						shp_set->cur_info.m1_y.h_i_size,shp_set->cur_info.m1_y.v_i_size,\
						shp_set->cur_info.m1_y.h_active,shp_set->cur_info.m1_y.v_active,\
						shp_set->cur_info.m1_y.h_o_size,shp_set->cur_info.m1_y.v_o_size);
					PE_INF_H15_CASE_SPRINT(54,rd_cnt++,buffer,\
						"[%02d]cur m1_y c,p,af:%d,%d,%d, v,a,b:%d,%d,%d, co:%d,%d,%d,%d\n",rd_cnt,\
						shp_set->cur_info.m1_y.in_c_fmt,shp_set->cur_info.m1_y.pixel_rp,\
						shp_set->cur_info.m1_y.acti_fmt,\
						shp_set->cur_info.m1_y.vscl_mode,shp_set->cur_info.m1_y.adaptv_on,\
						shp_set->cur_info.m1_y.blnear_on,shp_set->cur_info.m1_y.h1_co_num,\
						shp_set->cur_info.m1_y.h2_co_num,shp_set->cur_info.m1_y.v1_co_num,\
						shp_set->cur_info.m1_y.v2_co_num);
					PE_INF_H15_CASE_SPRINT(55,rd_cnt++,buffer,\
						"[%02d]cur m2_y id:%d,%d,%d,%d, i:%d,%d, a:%d,%d, o:%d,%d\n",rd_cnt,\
						shp_set->cur_info.m2_y.h_y_indx,shp_set->cur_info.m2_y.v_y_indx,\
						shp_set->cur_info.m2_y.h_c_indx,shp_set->cur_info.m2_y.v_c_indx,\
						shp_set->cur_info.m2_y.h_i_size,shp_set->cur_info.m2_y.v_i_size,\
						shp_set->cur_info.m2_y.h_active,shp_set->cur_info.m2_y.v_active,\
						shp_set->cur_info.m2_y.h_o_size,shp_set->cur_info.m2_y.v_o_size);
					PE_INF_H15_CASE_SPRINT(56,rd_cnt++,buffer,\
						"[%02d]cur m2_y c,p,af:%d,%d,%d, v,a,b:%d,%d,%d, co:%d,%d,%d,%d\n",rd_cnt,\
						shp_set->cur_info.m2_y.in_c_fmt,shp_set->cur_info.m2_y.pixel_rp,\
						shp_set->cur_info.m2_y.acti_fmt,\
						shp_set->cur_info.m2_y.vscl_mode,shp_set->cur_info.m2_y.adaptv_on,\
						shp_set->cur_info.m2_y.blnear_on,shp_set->cur_info.m2_y.h1_co_num,\
						shp_set->cur_info.m2_y.h2_co_num,shp_set->cur_info.m2_y.v1_co_num,\
						shp_set->cur_info.m2_y.v2_co_num);
					PE_INF_H15_CASE_SPRINT(57,rd_cnt++,buffer,\
						"[%02d]cur m1_c id:%d,%d,%d,%d, i:%d,%d, a:%d,%d, o:%d,%d\n",rd_cnt,\
						shp_set->cur_info.m1_c.h_y_indx,shp_set->cur_info.m1_c.v_y_indx,\
						shp_set->cur_info.m1_c.h_c_indx,shp_set->cur_info.m1_c.v_c_indx,\
						shp_set->cur_info.m1_c.h_i_size,shp_set->cur_info.m1_c.v_i_size,\
						shp_set->cur_info.m1_c.h_active,shp_set->cur_info.m1_c.v_active,\
						shp_set->cur_info.m1_c.h_o_size,shp_set->cur_info.m1_c.v_o_size);
					PE_INF_H15_CASE_SPRINT(58,rd_cnt++,buffer,\
						"[%02d]cur m1_c c,p,af:%d,%d,%d, v,a,b:%d,%d,%d, co:%d,%d,%d,%d\n",rd_cnt,\
						shp_set->cur_info.m1_c.in_c_fmt,shp_set->cur_info.m1_c.pixel_rp,\
						shp_set->cur_info.m1_c.acti_fmt,\
						shp_set->cur_info.m1_c.vscl_mode,shp_set->cur_info.m1_c.adaptv_on,\
						shp_set->cur_info.m1_c.blnear_on,shp_set->cur_info.m1_c.h1_co_num,\
						shp_set->cur_info.m1_c.h2_co_num,shp_set->cur_info.m1_c.v1_co_num,\
						shp_set->cur_info.m1_c.v2_co_num);
					PE_INF_H15_CASE_SPRINT(59,rd_cnt++,buffer,\
						"[%02d]cur m2_c id:%d,%d,%d,%d, i:%d,%d, a:%d,%d, o:%d,%d\n",rd_cnt,\
						shp_set->cur_info.m2_c.h_y_indx,shp_set->cur_info.m2_c.v_y_indx,\
						shp_set->cur_info.m2_c.h_c_indx,shp_set->cur_info.m2_c.v_c_indx,\
						shp_set->cur_info.m2_c.h_i_size,shp_set->cur_info.m2_c.v_i_size,\
						shp_set->cur_info.m2_c.h_active,shp_set->cur_info.m2_c.v_active,\
						shp_set->cur_info.m2_c.h_o_size,shp_set->cur_info.m2_c.v_o_size);
					PE_INF_H15_CASE_SPRINT(60,rd_cnt++,buffer,\
						"[%02d]cur m2_c c,p,af:%d,%d,%d, v,a,b:%d,%d,%d, co:%d,%d,%d,%d\n",rd_cnt,\
						shp_set->cur_info.m2_c.in_c_fmt,shp_set->cur_info.m2_c.pixel_rp,\
						shp_set->cur_info.m2_c.acti_fmt,\
						shp_set->cur_info.m2_c.vscl_mode,shp_set->cur_info.m2_c.adaptv_on,\
						shp_set->cur_info.m2_c.blnear_on,shp_set->cur_info.m2_c.h1_co_num,\
						shp_set->cur_info.m2_c.h2_co_num,shp_set->cur_info.m2_c.v1_co_num,\
						shp_set->cur_info.m2_c.v2_co_num);
					PE_INF_H15_CASE_SPRINT(61,rd_cnt++,buffer,"[%02d]ccm:\n",rd_cnt);
					PE_INF_H15_CASE_SPRINT(62,rd_cnt++,buffer,\
						"[%02d]lrcr(a0) sel : r,l_h:%d,%d, r,l_p:%d,%d, r,l_o:%d,%d\n",rd_cnt,\
						ccm_set->ccm_inf.r_h_select,ccm_set->ccm_inf.l_h_select,ccm_set->ccm_inf.r_p_select,\
						ccm_set->ccm_inf.l_p_select,ccm_set->ccm_inf.r_o_select,ccm_set->ccm_inf.l_o_select);
					PE_INF_H15_CASE_SPRINT(63,rd_cnt++,buffer,\
						"[%02d]lrcr(a0) r,l_bypass:%d,%d\n",rd_cnt,\
						ccm_set->ccm_inf.ppr_bypass,ccm_set->ccm_inf.ppl_bypass);
					PE_INF_H15_CASE_SPRINT(64,rd_cnt++,buffer,\
						"[%02d]lrcr ff_md:%d, ll_md:%d, h_md:%d, gain:%d, lut:%d\n",rd_cnt,\
						ccm_set->ccm_inf.ff_mode_on,ccm_set->ccm_inf.ll_mode_en,ccm_set->ccm_inf.histo_mode,\
						ccm_set->ccm_inf.lrcr_gain,ccm_set->ccm_inf.lut_type);
					PE_INF_H15_CASE_SPRINT(65,rd_cnt++,buffer,\
						"[%02d]acr en:%d, max_hist:%d,val_bins:%d, th0,1:%d,%d\n",rd_cnt,\
						ccm_set->auto_cr.enable,ccm_set->auto_cr.th_max_hist,ccm_set->auto_cr.th_valid_bins,\
						ccm_set->auto_cr.adj_th0,ccm_set->auto_cr.adj_th1);
					PE_INF_H15_CASE_SPRINT(66,rd_cnt++,buffer,\
						"---------------------------------------------------------------------------\n");
					PE_INF_H15_CASE_SPRINT(67,rd_cnt++,buffer,\
						"[%02d]db mode : shp:%d,hst:%d,scl:%d,tnr:%d,dnr:%d\n",rd_cnt,\
						PE_CMN_HW_H15_GetDbInfo(PE_CMN_HW_H15_DB_INF_MD0_SHP_FW_EN, 0), \
						PE_CMN_HW_H15_GetDbInfo(PE_CMN_HW_H15_DB_INF_MD0_HST_FW_EN, 0), \
						PE_CMN_HW_H15_GetDbInfo(PE_CMN_HW_H15_DB_INF_MD0_SCL_FW_EN, 0), \
						PE_CMN_HW_H15_GetDbInfo(PE_CMN_HW_H15_DB_INF_MD0_TNR_FW_EN, 0), \
						PE_CMN_HW_H15_GetDbInfo(PE_CMN_HW_H15_DB_INF_MD0_DNR_FW_EN, 0));
					PE_INF_H15_CASE_SPRINT(68,rd_cnt++,buffer,PE_INF_H15_PRINT_RESERVED);
					PE_INF_H15_CASE_SPRINT(69,rd_cnt++,buffer,PE_INF_H15_PRINT_RESERVED);
					PE_INF_H15_CASE_SPRINT(70,rd_cnt++,buffer,PE_INF_H15_PRINT_RESERVED);
					PE_INF_H15_CASE_SPRINT(71,rd_cnt++,buffer,\
						"---------------------------------------------------------------------------\n");
					PE_INF_H15_CASE_SPRINT(72,rd_cnt++,buffer,\
						"- kdrv ver. : %s\n", PE_INF_H15_DRV_VER);
					PE_INF_H15_CASE_SPRINT(73,rd_cnt++,buffer,\
						"---------------------------------------------------------------------------\n");
					PE_INF_H15_DFLT_SPRINT(rd_cnt=0,buffer,PE_INF_H15_PRINT_END);
				}
			}
			else if (pstParams->data[0]==PE_INF_HW_H15A_GINFO_DDR_H_PHYS || \
				pstParams->data[0]==PE_INF_HW_H15A_GINFO_DDR_H_SHDW)
			{
				PE_DDR_DB_SHDW_REG_H15_T *p_shdw_db = gPE_DDR_DB_H15.shdw.data;
				UINT32 *p_s_base = NULL;
				UINT32 *p_s_size = NULL;
				UINT32 sec_typ;
				PE_DDR_H15_HEADER_T *p_sec_h = NULL;
				if (rd_cnt==0)
				{
					p_sec_data = (PE_DDR_H15_DATA_T *)OS_Malloc(sizeof(PE_DDR_H15_DATA_T)*PE_DDR_H15_SEC_MAX_SIZE);
					CHECK_KNULL(p_sec_data);
					p_h_top = (PE_DDR_H15_TOP_HEADER_T *)OS_Malloc(sizeof(PE_DDR_H15_TOP_HEADER_T));
					CHECK_KNULL(p_h_top);
					if (pstParams->data[0]==PE_INF_HW_H15A_GINFO_DDR_H_PHYS)
					{
						PE_INF_H15_GetCurDdrTopHeader((void *)p_h_top);
						for (sec_typ=0; sec_typ<PE_DDR_H15_SEC_NUM; sec_typ++)
						{
							p_sec_h = &(p_sec_data[sec_typ].header);
							PE_INF_H15_GetCurDdrSecHeader((void *)p_h_top, (void *)p_sec_h, sec_typ);
						}
					}
					else
					{
						memcpy(p_h_top, &(p_shdw_db->top_h), sizeof(PE_DDR_H15_TOP_HEADER_T));
						for (sec_typ=0; sec_typ<PE_DDR_H15_SEC_NUM; sec_typ++)
						{
							p_sec_data[sec_typ].header = p_shdw_db->sec_d[sec_typ].header;
						}
					}
					for (sec_typ=0; sec_typ<PE_DDR_H15_SEC_NUM; sec_typ++)
					{
						p_sec_data[sec_typ].data_base = p_shdw_db->sec_d[sec_typ].data_base;
					}
				}
				p_s_base = p_h_top->sec_base;
				p_s_size = p_h_top->sec_size;
				/* causion : p_s_base and p_s_size should sync with PE_DDR_H15_SEC_MAX_SIZE */
				switch(rd_cnt)
				{
					PE_INF_H15_CASE_SPRINT( 0,rd_cnt++,buffer,PE_INF_H15_PRINT_START);
					PE_INF_H15_CASE_SPRINT( 1,rd_cnt++,buffer,\
						"--------------------------------------------------------------------------------\n");
					PE_INF_H15_CASE_SPRINT( 2,rd_cnt++,buffer,\
						"[%02d]top_h     : ver:%08x b_a:%08x s_n:%08x\n", rd_cnt, \
						p_h_top->version, p_h_top->db_base, p_h_top->sec_num);
					PE_INF_H15_CASE_SPRINT( 3,rd_cnt++,buffer, \
						"[%02d]top_h base: [00]%08x [01]%08x [02]%08x [03]%08x [04]%08x\n", rd_cnt, \
						p_s_base[0], p_s_base[1], p_s_base[2], p_s_base[3], p_s_base[4]);
					PE_INF_H15_CASE_SPRINT( 4,rd_cnt++,buffer, \
						"[%02d]top_h size: [00]%08x [01]%08x [02]%08x [03]%08x [04]%08x\n", rd_cnt, \
						p_s_size[0], p_s_size[1], p_s_size[2], p_s_size[3], p_s_size[4]);
					PE_INF_H15_CASE_SPRINT( 5,rd_cnt++,buffer, \
						"[%02d]top_h base: [05]%08x [06]%08x [07]%08x [08]%08x [09]%08x\n", rd_cnt, \
						p_s_base[5], p_s_base[6], p_s_base[7], p_s_base[8], p_s_base[9]);
					PE_INF_H15_CASE_SPRINT( 6,rd_cnt++,buffer, \
						"[%02d]top_h size: [05]%08x [06]%08x [07]%08x [08]%08x [09]%08x\n", rd_cnt, \
						p_s_size[5], p_s_size[6], p_s_size[7], p_s_size[8], p_s_size[9]);
					PE_INF_H15_CASE_SPRINT( 7,rd_cnt++,buffer, \
						"[%02d]top_h base: [10]%08x [11]%08x [12]%08x [13]%08x [14]%08x\n", rd_cnt, \
						p_s_base[10], p_s_base[11], p_s_base[12], p_s_base[13], p_s_base[14]);
					PE_INF_H15_CASE_SPRINT( 8,rd_cnt++,buffer, \
						"[%02d]top_h size: [10]%08x [11]%08x [12]%08x [13]%08x [14]%08x\n", rd_cnt, \
						p_s_size[10], p_s_size[11], p_s_size[12], p_s_size[13], p_s_size[14]);
					PE_INF_H15_CASE_SPRINT( 9,rd_cnt++,buffer, \
						"[%02d]top_h base: [15]%08x [16]%08x [17]%08x [18]%08x [19]%08x\n", rd_cnt, \
						p_s_base[15], p_s_base[16], p_s_base[17], p_s_base[18], p_s_base[19]);
					PE_INF_H15_CASE_SPRINT(10,rd_cnt++,buffer, \
						"[%02d]top_h size: [15]%08x [16]%08x [17]%08x [18]%08x [19]%08x\n", rd_cnt, \
						p_s_size[15], p_s_size[16], p_s_size[17], p_s_size[18], p_s_size[19]);
					PE_INF_H15_CASE_SPRINT(11,rd_cnt++,buffer, \
						"[%02d]top_h base: [20]%08x [21]%08x [22]%08x [23]%08x [24]%08x\n", rd_cnt, \
						p_s_base[20], p_s_base[21], p_s_base[22], p_s_base[23], p_s_base[24]);
					PE_INF_H15_CASE_SPRINT(12,rd_cnt++,buffer, \
						"[%02d]top_h size: [20]%08x [21]%08x [22]%08x [23]%08x [24]%08x\n", rd_cnt, \
						p_s_size[20], p_s_size[21], p_s_size[22], p_s_size[23], p_s_size[24]);
					PE_INF_H15_CASE_SPRINT(13,rd_cnt++,buffer, \
						"[%02d]top_h base: [25]%08x [26]%08x [27]%08x [28]%08x [29]%08x\n", rd_cnt, \
						p_s_base[25], p_s_base[26], p_s_base[27], p_s_base[28], p_s_base[29]);
					PE_INF_H15_CASE_SPRINT(14,rd_cnt++,buffer, \
						"[%02d]top_h size: [25]%08x [26]%08x [27]%08x [28]%08x [29]%08x\n", rd_cnt, \
						p_s_size[25], p_s_size[26], p_s_size[27], p_s_size[28], p_s_size[29]);
					PE_INF_H15_CASE_SPRINT(15,rd_cnt++,buffer, \
						"[%02d]top_h base: [30]%08x [31]%08x [32]%08x [33]%08x [34]%08x\n", rd_cnt, \
						p_s_base[30], p_s_base[31], p_s_base[32], p_s_base[33], p_s_base[34]);
					PE_INF_H15_CASE_SPRINT(16,rd_cnt++,buffer, \
						"[%02d]top_h size: [30]%08x [31]%08x [32]%08x [33]%08x [34]%08x\n", rd_cnt, \
						p_s_size[30], p_s_size[31], p_s_size[32], p_s_size[33], p_s_size[34]);
					PE_INF_H15_CASE_SPRINT(17,rd_cnt++,buffer, \
						"[%02d]top_h base: [35]%08x [36]%08x [37]%08x [38]%08x [39]%08x\n", rd_cnt, \
						p_s_base[35], p_s_base[36], p_s_base[37], p_s_base[38], p_s_base[39]);
					PE_INF_H15_CASE_SPRINT(18,rd_cnt++,buffer, \
						"[%02d]top_h size: [35]%08x [36]%08x [37]%08x [38]%08x [39]%08x\n", rd_cnt, \
						p_s_size[35], p_s_size[36], p_s_size[37], p_s_size[38], p_s_size[39]);
					PE_INF_H15_CASE_SPRINT(19,rd_cnt++,buffer, \
						"[%02d]top_h base: [40]%08x [41]%08x [42]%08x [43]%08x [44]%08x\n", rd_cnt, \
						p_s_base[40], p_s_base[41], p_s_base[42], p_s_base[43], p_s_base[44]);
					PE_INF_H15_CASE_SPRINT(20,rd_cnt++,buffer, \
						"[%02d]top_h size: [40]%08x [41]%08x [42]%08x [43]%08x [44]%08x\n", rd_cnt, \
						p_s_size[40], p_s_size[41], p_s_size[42], p_s_size[43], p_s_size[44]);
					PE_INF_H15_CASE_SPRINT(21,rd_cnt++,buffer, \
						"[%02d]top_h base: [45]%08x [46]%08x [47]%08x [48]%08x [49]%08x\n", rd_cnt, \
						p_s_base[45], p_s_base[46], p_s_base[47], p_s_base[48], p_s_base[49]);
					PE_INF_H15_CASE_SPRINT(22,rd_cnt++,buffer, \
						"[%02d]top_h size: [45]%08x [46]%08x [47]%08x [48]%08x [49]%08x\n", rd_cnt, \
						p_s_size[45], p_s_size[46], p_s_size[47], p_s_size[48], p_s_size[49]);
					PE_INF_H15_CASE_SPRINT(23,rd_cnt++,buffer,PE_INF_H15_PRINT_RESERVED);
					PE_INF_H15_CASE_SPRINT(24,rd_cnt++,buffer,PE_INF_H15_PRINT_RESERVED);
					PE_INF_H15_CASE_SPRINT(25,rd_cnt++,buffer,PE_INF_H15_PRINT_RESERVED);
					PE_INF_H15_CASE_SPRINT(26,rd_cnt++,buffer,PE_INF_H15_PRINT_RESERVED);
					PE_INF_H15_CASE_SPRINT(27,rd_cnt++,buffer,PE_INF_H15_PRINT_RESERVED);
					PE_INF_H15_CASE_SPRINT(28,rd_cnt++,buffer,PE_INF_H15_PRINT_RESERVED);
					PE_INF_H15_CASE_SPRINT(29,rd_cnt++,buffer,PE_INF_H15_PRINT_RESERVED);
					PE_INF_H15_CASE_SPRINT(30,rd_cnt++,buffer,PE_INF_H15_PRINT_RESERVED);
					PE_INF_H15_CASE_SPRINT(31,rd_cnt++,buffer,PE_INF_H15_PRINT_RESERVED);
					PE_INF_H15_CASE_SPRINT(32,rd_cnt++,buffer,PE_INF_H15_PRINT_RESERVED);
					PE_INF_H15_CASE_SPRINT(33,rd_cnt++,buffer,PE_INF_H15_PRINT_RESERVED);
					PE_INF_H15_CASE_SPRINT(34,rd_cnt++,buffer,PE_INF_H15_PRINT_RESERVED);
					PE_INF_H15_CASE_SPRINT(35,rd_cnt++,buffer,PE_INF_H15_PRINT_RESERVED);
					PE_INF_H15_CASE_SPRINT(36,rd_cnt++,buffer,PE_INF_H15_PRINT_RESERVED);
					PE_INF_H15_CASE_SPRINT(37,rd_cnt++,buffer,PE_INF_H15_PRINT_RESERVED);
					PE_INF_H15_CASE_SPRINT(38,rd_cnt++,buffer,PE_INF_H15_PRINT_RESERVED);
					PE_INF_H15_CASE_SPRINT(39,rd_cnt++,buffer,\
						"--------------------------------------------------------------------------------\n");
					PE_INF_H15_CASE_SPRINT(40,rd_cnt++,buffer, \
						"[%02d]%s: ver:%08x s_t:%08x i_n:%08x i_s:%08x d_b:%08x\n", rd_cnt, \
						PE_DDR_REG_H15_SEC_TYPE_TO_STR(rd_cnt-40), \
						p_sec_data[rd_cnt-40].header.version, p_sec_data[rd_cnt-40].header.sec_type, \
						p_sec_data[rd_cnt-40].header.item_num, p_sec_data[rd_cnt-40].header.item_size, \
						p_sec_data[rd_cnt-40].data_base);
					PE_INF_H15_CASE_SPRINT(41,rd_cnt++,buffer, \
						"[%02d]%s: ver:%08x s_t:%08x i_n:%08x i_s:%08x d_b:%08x\n", rd_cnt, \
						PE_DDR_REG_H15_SEC_TYPE_TO_STR(rd_cnt-40), \
						p_sec_data[rd_cnt-40].header.version, p_sec_data[rd_cnt-40].header.sec_type, \
						p_sec_data[rd_cnt-40].header.item_num, p_sec_data[rd_cnt-40].header.item_size, \
						p_sec_data[rd_cnt-40].data_base);
					PE_INF_H15_CASE_SPRINT(42,rd_cnt++,buffer, \
						"[%02d]%s: ver:%08x s_t:%08x i_n:%08x i_s:%08x d_b:%08x\n", rd_cnt, \
						PE_DDR_REG_H15_SEC_TYPE_TO_STR(rd_cnt-40), \
						p_sec_data[rd_cnt-40].header.version, p_sec_data[rd_cnt-40].header.sec_type, \
						p_sec_data[rd_cnt-40].header.item_num, p_sec_data[rd_cnt-40].header.item_size, \
						p_sec_data[rd_cnt-40].data_base);
					PE_INF_H15_CASE_SPRINT(43,rd_cnt++,buffer, \
						"[%02d]%s: ver:%08x s_t:%08x i_n:%08x i_s:%08x d_b:%08x\n", rd_cnt, \
						PE_DDR_REG_H15_SEC_TYPE_TO_STR(rd_cnt-40), \
						p_sec_data[rd_cnt-40].header.version, p_sec_data[rd_cnt-40].header.sec_type, \
						p_sec_data[rd_cnt-40].header.item_num, p_sec_data[rd_cnt-40].header.item_size, \
						p_sec_data[rd_cnt-40].data_base);
					PE_INF_H15_CASE_SPRINT(44,rd_cnt++,buffer, \
						"[%02d]%s: ver:%08x s_t:%08x i_n:%08x i_s:%08x d_b:%08x\n", rd_cnt, \
						PE_DDR_REG_H15_SEC_TYPE_TO_STR(rd_cnt-40), \
						p_sec_data[rd_cnt-40].header.version, p_sec_data[rd_cnt-40].header.sec_type, \
						p_sec_data[rd_cnt-40].header.item_num, p_sec_data[rd_cnt-40].header.item_size, \
						p_sec_data[rd_cnt-40].data_base);
					PE_INF_H15_CASE_SPRINT(45,rd_cnt++,buffer, \
						"[%02d]%s: ver:%08x s_t:%08x i_n:%08x i_s:%08x d_b:%08x\n", rd_cnt, \
						PE_DDR_REG_H15_SEC_TYPE_TO_STR(rd_cnt-40), \
						p_sec_data[rd_cnt-40].header.version, p_sec_data[rd_cnt-40].header.sec_type, \
						p_sec_data[rd_cnt-40].header.item_num, p_sec_data[rd_cnt-40].header.item_size, \
						p_sec_data[rd_cnt-40].data_base);
					PE_INF_H15_CASE_SPRINT(46,rd_cnt++,buffer, \
						"[%02d]%s: ver:%08x s_t:%08x i_n:%08x i_s:%08x d_b:%08x\n", rd_cnt, \
						PE_DDR_REG_H15_SEC_TYPE_TO_STR(rd_cnt-40), \
						p_sec_data[rd_cnt-40].header.version, p_sec_data[rd_cnt-40].header.sec_type, \
						p_sec_data[rd_cnt-40].header.item_num, p_sec_data[rd_cnt-40].header.item_size, \
						p_sec_data[rd_cnt-40].data_base);
					PE_INF_H15_CASE_SPRINT(47,rd_cnt++,buffer, \
						"[%02d]%s: ver:%08x s_t:%08x i_n:%08x i_s:%08x d_b:%08x\n", rd_cnt, \
						PE_DDR_REG_H15_SEC_TYPE_TO_STR(rd_cnt-40), \
						p_sec_data[rd_cnt-40].header.version, p_sec_data[rd_cnt-40].header.sec_type, \
						p_sec_data[rd_cnt-40].header.item_num, p_sec_data[rd_cnt-40].header.item_size, \
						p_sec_data[rd_cnt-40].data_base);
					PE_INF_H15_CASE_SPRINT(48,rd_cnt++,buffer, \
						"[%02d]%s: ver:%08x s_t:%08x i_n:%08x i_s:%08x d_b:%08x\n", rd_cnt, \
						PE_DDR_REG_H15_SEC_TYPE_TO_STR(rd_cnt-40), \
						p_sec_data[rd_cnt-40].header.version, p_sec_data[rd_cnt-40].header.sec_type, \
						p_sec_data[rd_cnt-40].header.item_num, p_sec_data[rd_cnt-40].header.item_size, \
						p_sec_data[rd_cnt-40].data_base);
					PE_INF_H15_CASE_SPRINT(49,rd_cnt++,buffer, \
						"[%02d]%s: ver:%08x s_t:%08x i_n:%08x i_s:%08x d_b:%08x\n", rd_cnt, \
						PE_DDR_REG_H15_SEC_TYPE_TO_STR(rd_cnt-40), \
						p_sec_data[rd_cnt-40].header.version, p_sec_data[rd_cnt-40].header.sec_type, \
						p_sec_data[rd_cnt-40].header.item_num, p_sec_data[rd_cnt-40].header.item_size, \
						p_sec_data[rd_cnt-40].data_base);
					PE_INF_H15_CASE_SPRINT(50,rd_cnt++,buffer,\
						"--------------------------------------------------------------------------------\n");
					PE_INF_H15_DFLT_SPRINT(rd_cnt=0,buffer,PE_INF_H15_PRINT_END);
				}
				if (rd_cnt==0)
				{
					if (p_h_top)	{OS_Free(p_h_top);}
					if (p_sec_data)	{OS_Free(p_sec_data);}
				}
			}
			else
			{
				sprintf(buffer, PE_INF_H15_PRINT_EXIT);
				rd_cnt=0;
			}
			memcpy(pstParams->data,buffer,sizeof(char)*PE_TRACE_STR_SIZE);
		}
		else
		{
			PE_INF_H15_DBG_PRINT("nothing to do.\n");
		}
	}while(0);
	if (win_set)	{OS_Free(win_set);}
	if (shp_set)	{OS_Free(shp_set);}
	if (ccm_set)	{OS_Free(ccm_set);}
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
static int PE_INF_H15_SetPathMonitor(LX_PE_ETC_TBL_T *pstParams)
{
    int ret = RET_OK;
	UINT32 w_data[3], sel = 0;
	/*
	UINT32 yuv08_data[6][3] = {\
		{0x00, 0x08, 0x08}, {0x80, 0x08, 0x08}, {0xff, 0x08, 0x08}, \
		{0x00, 0x08, 0x0f}, {0x80, 0x00, 0x00}, {0x00, 0x0f, 0x08}\
		};//0:black, 1:gray, 2:white, 3:R, 4:G, 5:B
	*/
	UINT32 rgb10_data[6][3] = {\
		{0x000, 0x000, 0x000}, {0x200, 0x200, 0x200}, {0x3ff, 0x3ff, 0x3ff}, \
		{0x3ff, 0x000, 0x000}, {0x000, 0x3ff, 0x000}, {0x000, 0x000, 0x3ff}\
		};//0:black, 1:gray, 2:white, 3:R, 4:G, 5:B
	do{
		CHECK_KNULL(pstParams);
		CHECK_KNULL(pstParams->data);
		if(PE_KDRV_VER_H15)
		{
			w_data[0] = pstParams->data[1];
			switch (pstParams->data[0])
			{
				case PE_INF_HW_H15A_M_CVA_TPG___TYPE:
					PE_CVI_A_H15A0_QWr01(misc_ctrl,pattern_type,w_data[0]);
					break;
				case PE_INF_HW_H15A_M_CVA_TPG__C_S_C:
					PE_CVI_A_H15A0_QWr01(misc_ctrl,pattern_csc,w_data[0]);
					break;
				case PE_INF_HW_H15A_M_CVA_TPG_DETAIL:
					PE_CVI_A_H15A0_QWr01(misc_ctrl,pattern_detail,w_data[0]);
					break;
				case PE_INF_HW_H15A_M_CVB_TPG___TYPE:
					PE_CVI_B_H15A0_QWr01(misc_ctrl,pattern_type,w_data[0]);
					break;
				case PE_INF_HW_H15A_M_CVB_TPG__C_S_C:
					PE_CVI_B_H15A0_QWr01(misc_ctrl,pattern_csc,w_data[0]);
					break;
				case PE_INF_HW_H15A_M_CVB_TPG_DETAIL:
					PE_CVI_B_H15A0_QWr01(misc_ctrl,pattern_detail,w_data[0]);
					break;
				case PE_INF_HW_H15A_M_CVC_TPG___TYPE:
					PE_CVI_C_H15A0_QWr01(misc_ctrl,pattern_type,w_data[0]);
					break;
				case PE_INF_HW_H15A_M_CVC_TPG__C_S_C:
					PE_CVI_C_H15A0_QWr01(misc_ctrl,pattern_csc,w_data[0]);
					break;
				case PE_INF_HW_H15A_M_CVC_TPG_DETAIL:
					PE_CVI_C_H15A0_QWr01(misc_ctrl,pattern_detail,w_data[0]);
					break;
				case PE_INF_HW_H15A_M_CVD_TPG___TYPE:
					PE_CVI_D_H15A0_QWr01(misc_ctrl,pattern_type,w_data[0]);
					break;
				case PE_INF_HW_H15A_M_CVD_TPG__C_S_C:
					PE_CVI_D_H15A0_QWr01(misc_ctrl,pattern_csc,w_data[0]);
					break;
				case PE_INF_HW_H15A_M_CVD_TPG_DETAIL:
					PE_CVI_D_H15A0_QWr01(misc_ctrl,pattern_detail,w_data[0]);
					break;
				case PE_INF_HW_H15A_M_ND1_DETOUR_ENB:
					PE_ND_ND1_H15A0_QWr01(pe0_opmode,detour_enable,w_data[0]);
					break;
				case PE_INF_HW_H15A_M_ND1_TNR_ENABLE:
					PE_ND_ND1_H15A0_QWr01(tnr_ctrl_00,reg_tnr_en,w_data[0]);
					break;
				case PE_INF_HW_H15A_M_ND1_IPC_Y_MODE:
					PE_ND_ND1_H15A0_QWr01(ipc_ctrl_00,cr_luma_mode,w_data[0]);
					break;
				case PE_INF_HW_H15A_M_ND1_IPC_C_MODE:
					PE_ND_ND1_H15A0_QWr01(ipc_ctrl_00,cr_chroma_mode,w_data[0]);
					break;
				case PE_INF_HW_H15A_M_ND1_FMD_ENABLE:
					PE_ND_ND1_H15A0_QWr01(ipc_ctrl_00,cr_film_mode_enable,w_data[0]);
					break;
				case PE_INF_HW_H15A_M_ND1_CLC_FLT_EN:
					PE_ND_ND1_H15A0_QWr01(clc_ctrl_00,cl_filter_enable,w_data[0]);
					break;
				case PE_INF_HW_H15A_M_ND2_DETOUR_ENB:
					PE_ND_ND2_H15A0_QWr01(pe0_opmode,detour_enable,w_data[0]);
					break;
				case PE_INF_HW_H15A_M_ND2_TNR_ENABLE:
					PE_ND_ND2_H15A0_QWr01(tnr_ctrl_00,reg_tnr_en,w_data[0]);
					break;
				case PE_INF_HW_H15A_M_ND2_IPC_Y_MODE:
					PE_ND_ND2_H15A0_QWr01(ipc_ctrl_00,cr_luma_mode,w_data[0]);
					break;
				case PE_INF_HW_H15A_M_ND2_IPC_C_MODE:
					PE_ND_ND2_H15A0_QWr01(ipc_ctrl_00,cr_chroma_mode,w_data[0]);
					break;
				case PE_INF_HW_H15A_M_ND2_FMD_ENABLE:
					PE_ND_ND2_H15A0_QWr01(ipc_ctrl_00,cr_film_mode_enable,w_data[0]);
					break;
				case PE_INF_HW_H15A_M_ND2_CLC_FLT_EN:
					PE_ND_ND2_H15A0_QWr01(clc_ctrl_00,cl_filter_enable,w_data[0]);
					break;
				case PE_INF_HW_H15A_M_NDS_DETOUR_ENB:
					PE_ND_NDS_H15A0_QWr01(pe0_opmode,detour_enable,w_data[0]);
					break;
				case PE_INF_HW_H15A_M_NDS_TNR_ENABLE:
					PE_ND_NDS_H15A0_QWr01(tnr_ctrl_00,reg_tnr_en,w_data[0]);
					break;
				case PE_INF_HW_H15A_M_NDS_IPC_Y_MODE:
					PE_ND_NDS_H15A0_QWr01(ipc_ctrl_00,cr_luma_mode,w_data[0]);
					break;
				case PE_INF_HW_H15A_M_NDS_IPC_C_MODE:
					PE_ND_NDS_H15A0_QWr01(ipc_ctrl_00,cr_chroma_mode,w_data[0]);
					break;
				case PE_INF_HW_H15A_M_NDS_FMD_ENABLE:
					PE_ND_NDS_H15A0_QWr01(ipc_ctrl_00,cr_film_mode_enable,w_data[0]);
					break;
				case PE_INF_HW_H15A_M_M1Y_BILIN_ENBL:
					PE_MSC_Y_H15A0_QWr01(msc_m1_y_ctrl,reg_bilinear_on_m1,w_data[0]);
					break;
				case PE_INF_HW_H15A_M_M1Y_SAMPL_MODE:
					PE_MSC_Y_H15A0_QWr01(msc_m1_y_ctrl,reg_sampling_mode_m1,w_data[0]);
					break;
				case PE_INF_HW_H15A_M_M2Y_BILIN_ENBL:
					PE_MSC_Y_H15A0_QWr01(msc_m2_y_ctrl,reg_bilinear_on_m2,w_data[0]);
					break;
				case PE_INF_HW_H15A_M_M2Y_SAMPL_MODE:
					PE_MSC_Y_H15A0_QWr01(msc_m2_y_ctrl,reg_sampling_mode_m2,w_data[0]);
					break;
				case PE_INF_HW_H15A_M_M1C_BILIN_ENBL:
					PE_MSC_C_H15A0_QWr01(msc_m1_c_ctrl,reg_bilinear_on_m1,w_data[0]);
					break;
				case PE_INF_HW_H15A_M_M1C_SAMPL_MODE:
					PE_MSC_C_H15A0_QWr01(msc_m1_c_ctrl,reg_sampling_mode_m1,w_data[0]);
					break;
				case PE_INF_HW_H15A_M_M2C_BILIN_ENBL:
					PE_MSC_C_H15A0_QWr01(msc_m2_c_ctrl,reg_bilinear_m2,w_data[0]);
					break;
				case PE_INF_HW_H15A_M_M2C_SAMPL_MODE:
					PE_MSC_C_H15A0_QWr01(msc_m2_c_ctrl,reg_sampling_mode_m2,w_data[0]);
					break;
				case PE_INF_HW_H15A_M_SSC_BILIN_ENBL:
					PE_SSC_H15A0_QWr01(ssc_ctrl,filter_type_sel,w_data[0]);
					break;
				case PE_INF_HW_H15A_M_SRE_SHP_DETOUR:
					PE_SR_SHP_H15A0_QWr01(pe1_top_ctrl_02,pe1_detour,w_data[0]);
					break;
				case PE_INF_HW_H15A_M_SRE_GSC_BILINE:
					PE_SR_GSC_H15A0_QWr01(sr_gsc_y_ctrl,reg_bilinear_on_m1,w_data[0]);
					break;
				case PE_INF_HW_H15A_M_SRE_GSC_SAMPLE:
					PE_SR_GSC_H15A0_QWr01(sr_gsc_y_ctrl,reg_sampling_mode_m1,w_data[0]);
					break;
				case PE_INF_HW_H15A_M_SHPL_DETOUR_EN:
					PE_SHPL_H15A0_QWr01(pe1_top_ctrl_02,pe1_detour,w_data[0]);
					break;
				case PE_INF_HW_H15A_M_SHPR_DETOUR_EN:
					PE_SHPR_H15A0_QWr01(pe1_top_ctrl_02,pe1_detour,w_data[0]);
					break;
				case PE_INF_HW_H15A_M_CE_DETOUR_ENBL:
					PE_CE_PE1_H15A0_QWr01(pe1_top_ctrl_02,pe1_detour,w_data[0]);
					break;
				case PE_INF_HW_H15A_M_CE_MUTE_L_ENBL:
					PE_CE_PE1_H15A0_QWr01(pe1_mute_ctrl_00,pxl_rep_in,w_data[0]);
					break;
				case PE_INF_HW_H15A_M_CE_MUTE_L_COLR:
					sel = (w_data[0]<6)? w_data[0]:5;
					w_data[0] = rgb10_data[sel][0];	// r
					w_data[1] = rgb10_data[sel][1];	// g
					w_data[2] = rgb10_data[sel][2];	// b
					PE_CE_PE1_H15A0_QWr01(pe1_mute_ctrl_02,pxl_rep_value_r,w_data[0]);
					PE_CE_PE1_H15A0_QWr01(pe1_mute_ctrl_02,pxl_rep_value_g,w_data[1]);
					PE_CE_PE1_H15A0_QWr01(pe1_mute_ctrl_02,pxl_rep_value_b,w_data[2]);
					break;
				case PE_INF_HW_H15A_M_CE_MUTE_R_ENBL:
					PE_CE_PE1_H15A0_QWr01(pe1_mute_ctrl_05,pxl_rep_in,w_data[0]);
					break;
				case PE_INF_HW_H15A_M_CE_MUTE_R_COLR:
					sel = (w_data[0]<6)? w_data[0]:5;
					w_data[0] = rgb10_data[sel][0];	// r
					w_data[1] = rgb10_data[sel][1];	// g
					w_data[2] = rgb10_data[sel][2];	// b
					PE_CE_PE1_H15A0_QWr01(pe1_mute_ctrl_07,pxl_rep_value_r,w_data[0]);
					PE_CE_PE1_H15A0_QWr01(pe1_mute_ctrl_07,pxl_rep_value_g,w_data[1]);
					PE_CE_PE1_H15A0_QWr01(pe1_mute_ctrl_07,pxl_rep_value_b,w_data[2]);
					break;
				case PE_INF_HW_H15A_M_CO_DETOUR_ENBL:
					PE_CO_PE1_H15A0_QWr01(pe1_top_ctrl_02,pe1_detour,w_data[0]);
					break;
				case PE_INF_HW_H15A_M_CO_MUTE_L_ENBL:
					PE_CO_PE1_H15A0_QWr01(pe1_mute_ctrl_00,pxl_rep_in,w_data[0]);
					break;
				case PE_INF_HW_H15A_M_CO_MUTE_L_COLR:
					sel = (w_data[0]<6)? w_data[0]:5;
					w_data[0] = rgb10_data[sel][0];	// r
					w_data[1] = rgb10_data[sel][1];	// g
					w_data[2] = rgb10_data[sel][2];	// b
					PE_CO_PE1_H15A0_QWr01(pe1_mute_ctrl_02,pxl_rep_value_r,w_data[0]);
					PE_CO_PE1_H15A0_QWr01(pe1_mute_ctrl_02,pxl_rep_value_g,w_data[1]);
					PE_CO_PE1_H15A0_QWr01(pe1_mute_ctrl_02,pxl_rep_value_b,w_data[2]);
					break;
				case PE_INF_HW_H15A_M_CO_MUTE_R_ENBL:
					PE_CO_PE1_H15A0_QWr01(pe1_mute_ctrl_05,pxl_rep_in,w_data[0]);
					break;
				case PE_INF_HW_H15A_M_CO_MUTE_R_COLR:
					sel = (w_data[0]<6)? w_data[0]:5;
					w_data[0] = rgb10_data[sel][0];	// r
					w_data[1] = rgb10_data[sel][1];	// g
					w_data[2] = rgb10_data[sel][2];	// b
					PE_CO_PE1_H15A0_QWr01(pe1_mute_ctrl_07,pxl_rep_value_r,w_data[0]);
					PE_CO_PE1_H15A0_QWr01(pe1_mute_ctrl_07,pxl_rep_value_g,w_data[1]);
					PE_CO_PE1_H15A0_QWr01(pe1_mute_ctrl_07,pxl_rep_value_b,w_data[2]);
					break;
				case PE_INF_HW_H15A_M_DISPLAY:
				default:
					break;
			}
		}
		else
		{
			PE_INF_H15_DBG_PRINT("nothing to do.\n");
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
static int PE_INF_H15_GetPathMonitor(LX_PE_ETC_TBL_T *pstParams)
{
	int ret = RET_OK;
	UINT32 r_data[3];
	char buffer[PE_TRACE_STR_SIZE];
	static UINT32 rd_cnt=0;

#define PE_INF_H15_PRINT_START		"START_OF_PRINT"
#define PE_INF_H15_PRINT_RESERVED	"RESERVED"
#define PE_INF_H15_PRINT_END		"END_OF_PRINT"
#define PE_INF_H15_GETPM1_SPRINT(_buf, _type, _r1)	\
	sprintf(_buf, "   [0x%02x][%s] : %d\n", \
	(_type), #_type, (_r1))
#define PE_INF_H15_GETPM3_SPRINT(_buf, _type, _r1, _r2, _r3)	\
	sprintf(_buf, "   [0x%02x][%s] : 0x%03x 0x%03x 0x%03x\n", \
	(_type), #_type, (_r1), (_r2), (_r3))

	do{
		CHECK_KNULL(pstParams);
		CHECK_KNULL(pstParams->data);
		if(PE_KDRV_VER_H15)
		{
			switch (rd_cnt)
			{
				case PE_INF_HW_H15A_M_DISPLAY:
					sprintf(buffer, "%s", PE_INF_H15_PRINT_START);
					break;
				case PE_INF_HW_H15A_M_CVA_TPG___TYPE:
					PE_CVI_A_H15A0_QRd01(misc_ctrl,pattern_type,r_data[0]);
					PE_INF_H15_GETPM1_SPRINT(buffer, PE_INF_HW_H15A_M_CVA_TPG___TYPE, r_data[0]);
					break;
				case PE_INF_HW_H15A_M_CVA_TPG__C_S_C:
					PE_CVI_A_H15A0_QRd01(misc_ctrl,pattern_csc,r_data[0]);
					PE_INF_H15_GETPM1_SPRINT(buffer, PE_INF_HW_H15A_M_CVA_TPG__C_S_C, r_data[0]);
					break;
				case PE_INF_HW_H15A_M_CVA_TPG_DETAIL:
					PE_CVI_A_H15A0_QRd01(misc_ctrl,pattern_detail,r_data[0]);
					PE_INF_H15_GETPM1_SPRINT(buffer, PE_INF_HW_H15A_M_CVA_TPG_DETAIL, r_data[0]);
					break;
				case PE_INF_HW_H15A_M_CVB_TPG___TYPE:
					PE_CVI_B_H15A0_QRd01(misc_ctrl,pattern_type,r_data[0]);
					PE_INF_H15_GETPM1_SPRINT(buffer, PE_INF_HW_H15A_M_CVB_TPG___TYPE, r_data[0]);
					break;
				case PE_INF_HW_H15A_M_CVB_TPG__C_S_C:
					PE_CVI_B_H15A0_QRd01(misc_ctrl,pattern_csc,r_data[0]);
					PE_INF_H15_GETPM1_SPRINT(buffer, PE_INF_HW_H15A_M_CVB_TPG__C_S_C, r_data[0]);
					break;
				case PE_INF_HW_H15A_M_CVB_TPG_DETAIL:
					PE_CVI_B_H15A0_QRd01(misc_ctrl,pattern_detail,r_data[0]);
					PE_INF_H15_GETPM1_SPRINT(buffer, PE_INF_HW_H15A_M_CVB_TPG_DETAIL, r_data[0]);
					break;
				case PE_INF_HW_H15A_M_CVC_TPG___TYPE:
					PE_CVI_C_H15A0_QRd01(misc_ctrl,pattern_type,r_data[0]);
					PE_INF_H15_GETPM1_SPRINT(buffer, PE_INF_HW_H15A_M_CVC_TPG___TYPE, r_data[0]);
					break;
				case PE_INF_HW_H15A_M_CVC_TPG__C_S_C:
					PE_CVI_C_H15A0_QRd01(misc_ctrl,pattern_csc,r_data[0]);
					PE_INF_H15_GETPM1_SPRINT(buffer, PE_INF_HW_H15A_M_CVC_TPG__C_S_C, r_data[0]);
					break;
				case PE_INF_HW_H15A_M_CVC_TPG_DETAIL:
					PE_CVI_C_H15A0_QRd01(misc_ctrl,pattern_detail,r_data[0]);
					PE_INF_H15_GETPM1_SPRINT(buffer, PE_INF_HW_H15A_M_CVC_TPG_DETAIL, r_data[0]);
					break;
				case PE_INF_HW_H15A_M_CVD_TPG___TYPE:
					PE_CVI_D_H15A0_QRd01(misc_ctrl,pattern_type,r_data[0]);
					PE_INF_H15_GETPM1_SPRINT(buffer, PE_INF_HW_H15A_M_CVD_TPG___TYPE, r_data[0]);
					break;
				case PE_INF_HW_H15A_M_CVD_TPG__C_S_C:
					PE_CVI_D_H15A0_QRd01(misc_ctrl,pattern_csc,r_data[0]);
					PE_INF_H15_GETPM1_SPRINT(buffer, PE_INF_HW_H15A_M_CVD_TPG__C_S_C, r_data[0]);
					break;
				case PE_INF_HW_H15A_M_CVD_TPG_DETAIL:
					PE_CVI_D_H15A0_QRd01(misc_ctrl,pattern_detail,r_data[0]);
					PE_INF_H15_GETPM1_SPRINT(buffer, PE_INF_HW_H15A_M_CVD_TPG_DETAIL, r_data[0]);
					break;
				case PE_INF_HW_H15A_M_ND1_DETOUR_ENB:
					PE_ND_ND1_H15A0_QRd01(pe0_opmode,detour_enable,r_data[0]);
					PE_INF_H15_GETPM1_SPRINT(buffer, PE_INF_HW_H15A_M_ND1_DETOUR_ENB, r_data[0]);
					break;
				case PE_INF_HW_H15A_M_ND1_TNR_ENABLE:
					PE_ND_ND1_H15A0_QRd01(tnr_ctrl_00,reg_tnr_en,r_data[0]);
					PE_INF_H15_GETPM1_SPRINT(buffer, PE_INF_HW_H15A_M_ND1_TNR_ENABLE, r_data[0]);
					break;
				case PE_INF_HW_H15A_M_ND1_IPC_Y_MODE:
					PE_ND_ND1_H15A0_QRd01(ipc_ctrl_00,cr_luma_mode,r_data[0]);
					PE_INF_H15_GETPM1_SPRINT(buffer, PE_INF_HW_H15A_M_ND1_IPC_Y_MODE, r_data[0]);
					break;
				case PE_INF_HW_H15A_M_ND1_IPC_C_MODE:
					PE_ND_ND1_H15A0_QRd01(ipc_ctrl_00,cr_chroma_mode,r_data[0]);
					PE_INF_H15_GETPM1_SPRINT(buffer, PE_INF_HW_H15A_M_ND1_IPC_C_MODE, r_data[0]);
					break;
				case PE_INF_HW_H15A_M_ND1_FMD_ENABLE:
					PE_ND_ND1_H15A0_QRd01(ipc_ctrl_00,cr_film_mode_enable,r_data[0]);
					PE_INF_H15_GETPM1_SPRINT(buffer, PE_INF_HW_H15A_M_ND1_FMD_ENABLE, r_data[0]);
					break;
				case PE_INF_HW_H15A_M_ND1_CLC_FLT_EN:
					PE_ND_ND1_H15A0_QRd01(clc_ctrl_00,cl_filter_enable,r_data[0]);
					PE_INF_H15_GETPM1_SPRINT(buffer, PE_INF_HW_H15A_M_ND1_CLC_FLT_EN, r_data[0]);
					break;
				case PE_INF_HW_H15A_M_ND2_DETOUR_ENB:
					PE_ND_ND2_H15A0_QRd01(pe0_opmode,detour_enable,r_data[0]);
					PE_INF_H15_GETPM1_SPRINT(buffer, PE_INF_HW_H15A_M_ND2_DETOUR_ENB, r_data[0]);
					break;
				case PE_INF_HW_H15A_M_ND2_TNR_ENABLE:
					PE_ND_ND2_H15A0_QRd01(tnr_ctrl_00,reg_tnr_en,r_data[0]);
					PE_INF_H15_GETPM1_SPRINT(buffer, PE_INF_HW_H15A_M_ND2_TNR_ENABLE, r_data[0]);
					break;
				case PE_INF_HW_H15A_M_ND2_IPC_Y_MODE:
					PE_ND_ND2_H15A0_QRd01(ipc_ctrl_00,cr_luma_mode,r_data[0]);
					PE_INF_H15_GETPM1_SPRINT(buffer, PE_INF_HW_H15A_M_ND2_IPC_Y_MODE, r_data[0]);
					break;
				case PE_INF_HW_H15A_M_ND2_IPC_C_MODE:
					PE_ND_ND2_H15A0_QRd01(ipc_ctrl_00,cr_chroma_mode,r_data[0]);
					PE_INF_H15_GETPM1_SPRINT(buffer, PE_INF_HW_H15A_M_ND2_IPC_C_MODE, r_data[0]);
					break;
				case PE_INF_HW_H15A_M_ND2_FMD_ENABLE:
					PE_ND_ND2_H15A0_QRd01(ipc_ctrl_00,cr_film_mode_enable,r_data[0]);
					PE_INF_H15_GETPM1_SPRINT(buffer, PE_INF_HW_H15A_M_ND2_FMD_ENABLE, r_data[0]);
					break;
				case PE_INF_HW_H15A_M_ND2_CLC_FLT_EN:
					PE_ND_ND2_H15A0_QRd01(clc_ctrl_00,cl_filter_enable,r_data[0]);
					PE_INF_H15_GETPM1_SPRINT(buffer, PE_INF_HW_H15A_M_ND2_CLC_FLT_EN, r_data[0]);
					break;
				case PE_INF_HW_H15A_M_NDS_DETOUR_ENB:
					PE_ND_NDS_H15A0_QRd01(pe0_opmode,detour_enable,r_data[0]);
					PE_INF_H15_GETPM1_SPRINT(buffer, PE_INF_HW_H15A_M_NDS_DETOUR_ENB, r_data[0]);
					break;
				case PE_INF_HW_H15A_M_NDS_TNR_ENABLE:
					PE_ND_NDS_H15A0_QRd01(tnr_ctrl_00,reg_tnr_en,r_data[0]);
					PE_INF_H15_GETPM1_SPRINT(buffer, PE_INF_HW_H15A_M_NDS_TNR_ENABLE, r_data[0]);
					break;
				case PE_INF_HW_H15A_M_NDS_IPC_Y_MODE:
					PE_ND_NDS_H15A0_QRd01(ipc_ctrl_00,cr_luma_mode,r_data[0]);
					PE_INF_H15_GETPM1_SPRINT(buffer, PE_INF_HW_H15A_M_NDS_IPC_Y_MODE, r_data[0]);
					break;
				case PE_INF_HW_H15A_M_NDS_IPC_C_MODE:
					PE_ND_NDS_H15A0_QRd01(ipc_ctrl_00,cr_chroma_mode,r_data[0]);
					PE_INF_H15_GETPM1_SPRINT(buffer, PE_INF_HW_H15A_M_NDS_IPC_C_MODE, r_data[0]);
					break;
				case PE_INF_HW_H15A_M_NDS_FMD_ENABLE:
					PE_ND_NDS_H15A0_QRd01(ipc_ctrl_00,cr_film_mode_enable,r_data[0]);
					PE_INF_H15_GETPM1_SPRINT(buffer, PE_INF_HW_H15A_M_NDS_FMD_ENABLE, r_data[0]);
					break;
				case PE_INF_HW_H15A_M_M1Y_BILIN_ENBL:
					PE_MSC_Y_H15A0_QRd01(msc_m1_y_ctrl,reg_bilinear_on_m1,r_data[0]);
					PE_INF_H15_GETPM1_SPRINT(buffer, PE_INF_HW_H15A_M_M1Y_BILIN_ENBL, r_data[0]);
					break;
				case PE_INF_HW_H15A_M_M1Y_SAMPL_MODE:
					PE_MSC_Y_H15A0_QRd01(msc_m1_y_ctrl,reg_sampling_mode_m1,r_data[0]);
					PE_INF_H15_GETPM1_SPRINT(buffer, PE_INF_HW_H15A_M_M1Y_SAMPL_MODE, r_data[0]);
					break;
				case PE_INF_HW_H15A_M_M2Y_BILIN_ENBL:
					PE_MSC_Y_H15A0_QRd01(msc_m2_y_ctrl,reg_bilinear_on_m2,r_data[0]);
					PE_INF_H15_GETPM1_SPRINT(buffer, PE_INF_HW_H15A_M_M2Y_BILIN_ENBL, r_data[0]);
					break;
				case PE_INF_HW_H15A_M_M2Y_SAMPL_MODE:
					PE_MSC_Y_H15A0_QRd01(msc_m2_y_ctrl,reg_sampling_mode_m2,r_data[0]);
					PE_INF_H15_GETPM1_SPRINT(buffer, PE_INF_HW_H15A_M_M2Y_SAMPL_MODE, r_data[0]);
					break;
				case PE_INF_HW_H15A_M_M1C_BILIN_ENBL:
					PE_MSC_C_H15A0_QRd01(msc_m1_c_ctrl,reg_bilinear_on_m1,r_data[0]);
					PE_INF_H15_GETPM1_SPRINT(buffer, PE_INF_HW_H15A_M_M1C_BILIN_ENBL, r_data[0]);
					break;
				case PE_INF_HW_H15A_M_M1C_SAMPL_MODE:
					PE_MSC_C_H15A0_QRd01(msc_m1_c_ctrl,reg_sampling_mode_m1,r_data[0]);
					PE_INF_H15_GETPM1_SPRINT(buffer, PE_INF_HW_H15A_M_M1C_SAMPL_MODE, r_data[0]);
					break;
				case PE_INF_HW_H15A_M_M2C_BILIN_ENBL:
					PE_MSC_C_H15A0_QRd01(msc_m2_c_ctrl,reg_bilinear_m2,r_data[0]);
					PE_INF_H15_GETPM1_SPRINT(buffer, PE_INF_HW_H15A_M_M2C_BILIN_ENBL, r_data[0]);
					break;
				case PE_INF_HW_H15A_M_M2C_SAMPL_MODE:
					PE_MSC_C_H15A0_QRd01(msc_m2_c_ctrl,reg_sampling_mode_m2,r_data[0]);
					PE_INF_H15_GETPM1_SPRINT(buffer, PE_INF_HW_H15A_M_M2C_SAMPL_MODE, r_data[0]);
					break;
				case PE_INF_HW_H15A_M_SSC_BILIN_ENBL:
					PE_SSC_H15A0_QRd01(ssc_ctrl,filter_type_sel,r_data[0]);
					PE_INF_H15_GETPM1_SPRINT(buffer, PE_INF_HW_H15A_M_SSC_BILIN_ENBL, r_data[0]);
					break;
				case PE_INF_HW_H15A_M_SRE_SHP_DETOUR:
					PE_SR_SHP_H15A0_QRd01(pe1_top_ctrl_02,pe1_detour,r_data[0]);
					PE_INF_H15_GETPM1_SPRINT(buffer, PE_INF_HW_H15A_M_SRE_SHP_DETOUR, r_data[0]);
					break;
				case PE_INF_HW_H15A_M_SRE_GSC_BILINE:
					PE_SR_GSC_H15A0_QRd01(sr_gsc_y_ctrl,reg_bilinear_on_m1,r_data[0]);
					PE_INF_H15_GETPM1_SPRINT(buffer, PE_INF_HW_H15A_M_SRE_GSC_BILINE, r_data[0]);
					break;
				case PE_INF_HW_H15A_M_SRE_GSC_SAMPLE:
					PE_SR_GSC_H15A0_QRd01(sr_gsc_y_ctrl,reg_sampling_mode_m1,r_data[0]);
					PE_INF_H15_GETPM1_SPRINT(buffer, PE_INF_HW_H15A_M_SRE_GSC_SAMPLE, r_data[0]);
					break;
				case PE_INF_HW_H15A_M_SHPL_DETOUR_EN:
					PE_SHPL_H15A0_QRd01(pe1_top_ctrl_02,pe1_detour,r_data[0]);
					PE_INF_H15_GETPM1_SPRINT(buffer, PE_INF_HW_H15A_M_SHPL_DETOUR_EN, r_data[0]);
					break;
				case PE_INF_HW_H15A_M_SHPR_DETOUR_EN:
					PE_SHPR_H15A0_QRd01(pe1_top_ctrl_02,pe1_detour,r_data[0]);
					PE_INF_H15_GETPM1_SPRINT(buffer, PE_INF_HW_H15A_M_SHPR_DETOUR_EN, r_data[0]);
					break;
				case PE_INF_HW_H15A_M_CE_DETOUR_ENBL:
					PE_CE_PE1_H15A0_QRd01(pe1_top_ctrl_02,pe1_detour,r_data[0]);
					PE_INF_H15_GETPM1_SPRINT(buffer, PE_INF_HW_H15A_M_CE_DETOUR_ENBL, r_data[0]);
					break;
				case PE_INF_HW_H15A_M_CE_MUTE_L_ENBL:
					PE_CE_PE1_H15A0_QRd01(pe1_mute_ctrl_00,pxl_rep_in,r_data[0]);
					PE_INF_H15_GETPM1_SPRINT(buffer, PE_INF_HW_H15A_M_CE_MUTE_L_ENBL, r_data[0]);
					break;
				case PE_INF_HW_H15A_M_CE_MUTE_L_COLR:
					PE_CE_PE1_H15A0_QRd01(pe1_mute_ctrl_02,pxl_rep_value_r,r_data[0]);
					PE_CE_PE1_H15A0_QRd01(pe1_mute_ctrl_02,pxl_rep_value_g,r_data[1]);
					PE_CE_PE1_H15A0_QRd01(pe1_mute_ctrl_02,pxl_rep_value_b,r_data[2]);
					PE_INF_H15_GETPM3_SPRINT(buffer, PE_INF_HW_H15A_M_CE_MUTE_L_COLR, r_data[0], r_data[1], r_data[2]);
					break;
				case PE_INF_HW_H15A_M_CE_MUTE_R_ENBL:
					PE_CE_PE1_H15A0_QRd01(pe1_mute_ctrl_05,pxl_rep_in,r_data[0]);
					PE_INF_H15_GETPM1_SPRINT(buffer, PE_INF_HW_H15A_M_CE_MUTE_R_ENBL, r_data[0]);
					break;
				case PE_INF_HW_H15A_M_CE_MUTE_R_COLR:
					PE_CE_PE1_H15A0_QRd01(pe1_mute_ctrl_07,pxl_rep_value_r,r_data[0]);
					PE_CE_PE1_H15A0_QRd01(pe1_mute_ctrl_07,pxl_rep_value_g,r_data[1]);
					PE_CE_PE1_H15A0_QRd01(pe1_mute_ctrl_07,pxl_rep_value_b,r_data[2]);
					PE_INF_H15_GETPM3_SPRINT(buffer, PE_INF_HW_H15A_M_CE_MUTE_R_COLR, r_data[0], r_data[1], r_data[2]);
					break;
				case PE_INF_HW_H15A_M_CO_DETOUR_ENBL:
					PE_CO_PE1_H15A0_QRd01(pe1_top_ctrl_02,pe1_detour,r_data[0]);
					PE_INF_H15_GETPM1_SPRINT(buffer, PE_INF_HW_H15A_M_CO_DETOUR_ENBL, r_data[0]);
					break;
				case PE_INF_HW_H15A_M_CO_MUTE_L_ENBL:
					PE_CO_PE1_H15A0_QRd01(pe1_mute_ctrl_00,pxl_rep_in,r_data[0]);
					PE_INF_H15_GETPM1_SPRINT(buffer, PE_INF_HW_H15A_M_CO_MUTE_L_ENBL, r_data[0]);
					break;
				case PE_INF_HW_H15A_M_CO_MUTE_L_COLR:
					PE_CO_PE1_H15A0_QRd01(pe1_mute_ctrl_02,pxl_rep_value_r,r_data[0]);
					PE_CO_PE1_H15A0_QRd01(pe1_mute_ctrl_02,pxl_rep_value_g,r_data[1]);
					PE_CO_PE1_H15A0_QRd01(pe1_mute_ctrl_02,pxl_rep_value_b,r_data[2]);
					PE_INF_H15_GETPM3_SPRINT(buffer, PE_INF_HW_H15A_M_CO_MUTE_L_COLR, r_data[0], r_data[1], r_data[2]);
					break;
				case PE_INF_HW_H15A_M_CO_MUTE_R_ENBL:
					PE_CO_PE1_H15A0_QRd01(pe1_mute_ctrl_05,pxl_rep_in,r_data[0]);
					PE_INF_H15_GETPM1_SPRINT(buffer, PE_INF_HW_H15A_M_CO_MUTE_R_ENBL, r_data[0]);
					break;
				case PE_INF_HW_H15A_M_CO_MUTE_R_COLR:
					PE_CO_PE1_H15A0_QRd01(pe1_mute_ctrl_07,pxl_rep_value_r,r_data[0]);
					PE_CO_PE1_H15A0_QRd01(pe1_mute_ctrl_07,pxl_rep_value_g,r_data[1]);
					PE_CO_PE1_H15A0_QRd01(pe1_mute_ctrl_07,pxl_rep_value_b,r_data[2]);
					PE_INF_H15_GETPM3_SPRINT(buffer, PE_INF_HW_H15A_M_CO_MUTE_R_COLR, r_data[0], r_data[1], r_data[2]);
					break;
				default:
					sprintf(buffer, "%s", PE_INF_H15_PRINT_END);
					break;
			}
			if (rd_cnt<PE_INF_HW_H15A_M_MAX)
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
			PE_INF_H15_DBG_PRINT("nothing to do.\n");
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
int PE_INF_H15_GetLevelInfo(LX_PE_INF_LEVEL_T *pstParams)
{
	int ret = RET_OK;
	UINT32 r_data[4], sum[2];
	LX_PE_WIN_ID win_id;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		win_id = PE_GET_CHECKED_WINID(pstParams->win_id);
		if(PE_KDRV_VER_H15)
		{
			if(PE_CHECK_WIN0(win_id))
			{
				if(pstParams->sel&LX_PE_LEVEL_MOTION)
				{
					PE_ND_ND1_H15A0_QRd01(tnr_status_01,w_apl_motion,pstParams->motion_level);
				}
				if (pstParams->sel&(LX_PE_LEVEL_NOISE|LX_PE_LEVEL_PEAKPOS))
				{
					PE_ND_ND1_H15A0_QRd01(tnr_ne_stat_00,stat_f_ncnt_th0,r_data[0]);
					PE_ND_ND1_H15A0_QRd01(tnr_ne_stat_01,stat_f_ncnt_th1,r_data[1]);
					PE_ND_ND1_H15A0_QRd01(tnr_ne_stat_02,stat_f_ncnt_th2,r_data[2]);
					PE_ND_ND1_H15A0_QRd01(tnr_ne_stat_03,stat_f_ncnt_th3,r_data[3]);
					r_data[0] = (r_data[0]|0x1);	//to avoid zero
					r_data[1] = (r_data[1]|0x1);	//to avoid zero
					r_data[2] = (r_data[2]|0x1);	//to avoid zero
					r_data[3] = (r_data[3]|0x1);	//to avoid zero
					sum[0] = (r_data[1]+r_data[2]+r_data[3]);
					sum[1] = (r_data[0]+r_data[1]+r_data[2]+r_data[3]);
					pstParams->noise_level = (sum[0]<<8)/sum[1];
					if (pstParams->noise_level>255)
						pstParams->noise_level = 255;
					pstParams->peakpos_level = (pstParams->noise_level<<2);
					PE_INF_H15_DBG_PRINT("ne_stat_00~03 : %d,%d,%d,%d, nlvl:%d\n",\
						r_data[0], r_data[1], r_data[2], r_data[3], pstParams->noise_level);
				}
			}
		}
		else
		{
			PE_INF_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
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
int PE_INF_H15_SetMiscTbl(LX_PE_ETC_TBL_T *pstParams)
{
    int ret = RET_OK;
	UINT32 max_num = 0;
	do{
		CHECK_KNULL(pstParams);
		if (PE_KDRV_VER_H15)
		{
			max_num = sizeof(_g_pe_inf_h15_dbg_set_fn)/sizeof(PE_ETC_MISC_TBL_T);
			PE_INF_H15_CHECK_CODE(pstParams->func_num>=max_num, break, \
				"[%s,%d] invalid func_num[%d].\n", __F__, __L__, pstParams->func_num);
			CHECK_KNULL(_g_pe_inf_h15_dbg_set_fn[pstParams->func_num].func);
			ret = _g_pe_inf_h15_dbg_set_fn[pstParams->func_num].func(pstParams);
			PE_INF_H15_CHECK_CODE(ret, break, "[%s,%d] %s() error.\n", \
				__F__, __L__, _g_pe_inf_h15_dbg_set_fn[pstParams->func_num].fn_name);
		}
		else
		{
			PE_INF_H15_DBG_PRINT("nothing to do.\n");
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
int PE_INF_H15_GetMiscTbl(LX_PE_ETC_TBL_T *pstParams)
{
    int ret = RET_OK;
	UINT32 max_num = 0;
	do{
		CHECK_KNULL(pstParams);
		if (PE_KDRV_VER_H15)
		{
			max_num = sizeof(_g_pe_inf_h15_dbg_get_fn)/sizeof(PE_ETC_MISC_TBL_T);
			PE_INF_H15_CHECK_CODE(pstParams->func_num>=max_num, break, \
				"[%s,%d] invalid func_num[%d].\n", __F__, __L__, pstParams->func_num);
			CHECK_KNULL(_g_pe_inf_h15_dbg_get_fn[pstParams->func_num].func);
			ret = _g_pe_inf_h15_dbg_get_fn[pstParams->func_num].func(pstParams);
			PE_INF_H15_CHECK_CODE(ret, break, "[%s,%d] %s() error.\n", \
				__F__, __L__, _g_pe_inf_h15_dbg_get_fn[pstParams->func_num].fn_name);
		}
		else
		{
			PE_INF_H15_DBG_PRINT("nothing to do.\n");
			ret = RET_OK;
		}
	}while (0);
	return ret;
}
/**
 * sort trace data buf to dst buf
 * - ascending, bubble sorting
 *
 * @param   *dst [in/out] PE_INF_H15_DATA_BUF_T
 * @return  void
 * @see
 * @author	sj.youm@lge.com
 */
static void PE_INF_H15_SortTraceDataBuf(PE_INF_H15_DATA_BUF_T *dst)
{
	int ret = RET_OK;
	UINT32 size=0, i=0, max=0;
	UINT32 j=0, s0=0, s1=0, us0=0, us1=0;
	PE_INF_H15_DATA_BUF_T temp;
	PE_INF_H15_DATA_BUF_T *src = _g_pe_inf_h15_buf;

	do{
		CHECK_KNULL(dst);
		max = PE_INF_H15_BUF_MAX;
		size = sizeof(PE_INF_H15_DATA_BUF_T);
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
static int PE_INF_H15_GetTraceDataBuf(LX_PE_ETC_TBL_T *pstParams)
{
	int ret = RET_OK;
	char buffer[PE_TRACE_STR_SIZE];
	static UINT32 rd_cnt=0;
	PE_INF_H15_DATA_BUF_T *p_buf = _g_pe_inf_h15_buf_temp;
	UINT32 *p_data = NULL;
	PE_INF_H15_TIME_T *p_time = NULL;
	char *p_str = NULL;

#define PE_INF_H15_PRINT_RESERVED	"RESERVED"
#define PE_INF_H15_PRINT_END		"END_OF_PRINT"

	do{
		CHECK_KNULL(pstParams);
		CHECK_KNULL(pstParams->data);
		if (rd_cnt<PE_INF_H15_BUF_MAX)
		{
			if(rd_cnt==0)
			{
				PE_INF_H15_SortTraceDataBuf(p_buf);
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
				sprintf(buffer, "%s", PE_INF_H15_PRINT_RESERVED);
			}
			rd_cnt++;
		}
		else
		{
			sprintf(buffer, "%s", PE_INF_H15_PRINT_END);
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
int PE_INF_H15_SetTraceDataBuf(char *data)
{
    int ret = RET_OK;
	UINT32 i = 0;
	char *p_str = NULL;
	PE_INF_H15_TIME_T *p_time = NULL;
	static UINT32 cnt = 0;
	PE_INF_H15_DATA_BUF_T *p_buf = _g_pe_inf_h15_buf;
	do{
		CHECK_KNULL(data);
		if(cnt>=PE_INF_H15_BUF_MAX)	cnt = 0;
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
static int PE_INF_H15_SetPixelRead(LX_PE_ETC_TBL_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		CHECK_KNULL(pstParams->data);
		if (PE_KDRV_VER_H15)
		{
			switch (pstParams->data[0])
			{
				case PE_INF_HW_H15A_RD_PXL_WCP_HOR_POS:
					PE_VCP_DISP_H15A0_QWr01(vcp_wcp_m_pixel_read_ctrl,rd_pel_pos_x,pstParams->data[1]);
					break;
				case PE_INF_HW_H15A_RD_PXL_WCP_VER_POS:
					PE_VCP_DISP_H15A0_QWr01(vcp_wcp_m_pixel_read_ctrl,rd_pel_pos_y,pstParams->data[1]);
					break;
				case PE_INF_HW_H15A_RD_PXL_WCP_MARK_EN:
					PE_VCP_DISP_H15A0_QWr01(vcp_wcp_m_pixel_read_ctrl,rd_pel_pat_en,pstParams->data[1]);
					break;
				case PE_INF_HW_H15A_RD_PXL_LED_HOR_POS:
					PE_LED_RP_H15A0_QWr01(ad_rp_hv_pos,rp_h_pos,pstParams->data[1]);
					break;
				case PE_INF_HW_H15A_RD_PXL_LED_VER_POS:
					PE_LED_RP_H15A0_QWr01(ad_rp_hv_pos,rp_v_pos,pstParams->data[1]);
					break;
				case PE_INF_HW_H15A_RD_PXL_LED_MARK_EN:
					PE_LED_RP_H15A0_QWr01(ad_rp_make_en,mark_en,pstParams->data[1]);
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
static int PE_INF_H15_GetPixelRead(LX_PE_ETC_TBL_T *pstParams)
{
	int ret = RET_OK;
	char buffer[PE_TRACE_STR_SIZE];
	static UINT32 rd_cnt=0;
	UINT32 rd_data[15];

#define PE_INF_H15_RD_NO_PRINT			0
#define PE_INF_H15_RD_PRINT_START		"START_OF_PRINT"
#define PE_INF_H15_RD_PRINT_END			"END_OF_PRINT"
#define PE_INF_H15_RD_PRINT_EXIT		"EXIT_OF_PRINT"
#define PE_INF_H15_RD_PRINT_RESERV		"RESERVED"
#define PE_INF_H15_RD_CASE_PRMENU(_cnt, _action, _buf, _item, _val)	\
	case (_cnt):sprintf(_buf, "   [0x%02x] %s: %d\n", (_item), #_item, (_val));_action;break
#define PE_INF_H15_RD_CASE_SPRINT(_cnt, _action, _buf, fmt, args...)	\
	case (_cnt):sprintf(_buf, fmt, ##args);_action;break
#define PE_INF_H15_RD_DFLT_SPRINT(_action,_buf,fmt,args...)		\
	default:sprintf(_buf,fmt,##args);_action;break

	do{
		CHECK_KNULL(pstParams);
		CHECK_KNULL(pstParams->data);
		if (PE_KDRV_VER_H15)
		{
			if (pstParams->data[0]==PE_INF_HW_H15A_RD_PXL_DISPLAY)
			{
				PE_VCP_DISP_H15A0_QRd03(vcp_wcp_m_pixel_read_ctrl,rd_pel_pos_x,rd_data[0],\
                                                                  rd_pel_pos_y,rd_data[1],\
                                                                  rd_pel_pat_en,rd_data[2]);
				PE_VCP_DISP_H15A0_QRd03(vcp_wcp_m_pixel_read_data_e,msb8_rd_pel_y,rd_data[3],\
                                                                  msb8_rd_pel_cb,rd_data[4],\
                                                                  msb8_rd_pel_cr,rd_data[5]);
				PE_VCP_DISP_H15A0_QRd03(vcp_wcp_m_pixel_read_data_o,msb8_rd_pel_y,rd_data[6],\
                                                                  msb8_rd_pel_cb,rd_data[7],\
                                                                  msb8_rd_pel_cr,rd_data[8]);
				PE_LED_RP_H15A0_QRd02(ad_rp_hv_pos,rp_h_pos,rd_data[9],rp_v_pos,rd_data[10]);
				PE_LED_RP_H15A0_QRd01(ad_rp_make_en,mark_en,rd_data[11]);
				PE_LED_RP_H15A0_QRd01(ad_rp_r_value,rd_pixel_r,rd_data[12]);
				PE_LED_RP_H15A0_QRd01(ad_rp_g_value,rd_pixel_g,rd_data[13]);
				PE_LED_RP_H15A0_QRd01(ad_rp_b_value,rd_pixel_b,rd_data[14]);
				switch (rd_cnt)
				{
					PE_INF_H15_RD_CASE_SPRINT( 0, rd_cnt++, buffer, PE_INF_H15_RD_PRINT_START);
					PE_INF_H15_RD_CASE_SPRINT( 1, rd_cnt++, buffer, "   * get pixel value *\n");
					PE_INF_H15_RD_CASE_PRMENU( 2, rd_cnt++, buffer, PE_INF_HW_H15A_RD_PXL_WCP_HOR_POS, rd_data[0]);
					PE_INF_H15_RD_CASE_PRMENU( 3, rd_cnt++, buffer, PE_INF_HW_H15A_RD_PXL_WCP_VER_POS, rd_data[1]);
					PE_INF_H15_RD_CASE_PRMENU( 4, rd_cnt++, buffer, PE_INF_HW_H15A_RD_PXL_WCP_MARK_EN, rd_data[2]);
					PE_INF_H15_RD_CASE_PRMENU( 5, rd_cnt++, buffer, PE_INF_HW_H15A_RD_PXL_WCP_PXL_E_Y, rd_data[3]);
					PE_INF_H15_RD_CASE_PRMENU( 6, rd_cnt++, buffer, PE_INF_HW_H15A_RD_PXL_WCP_PXL_E_B, rd_data[4]);
					PE_INF_H15_RD_CASE_PRMENU( 7, rd_cnt++, buffer, PE_INF_HW_H15A_RD_PXL_WCP_PXL_E_R, rd_data[5]);
					PE_INF_H15_RD_CASE_PRMENU( 8, rd_cnt++, buffer, PE_INF_HW_H15A_RD_PXL_WCP_PXL_O_Y, rd_data[6]);
					PE_INF_H15_RD_CASE_PRMENU( 9, rd_cnt++, buffer, PE_INF_HW_H15A_RD_PXL_WCP_PXL_O_B, rd_data[7]);
					PE_INF_H15_RD_CASE_PRMENU(10, rd_cnt++, buffer, PE_INF_HW_H15A_RD_PXL_WCP_PXL_O_R, rd_data[8]);
					PE_INF_H15_RD_CASE_PRMENU(11, rd_cnt++, buffer, PE_INF_HW_H15A_RD_PXL_LED_HOR_POS, rd_data[9]);
					PE_INF_H15_RD_CASE_PRMENU(12, rd_cnt++, buffer, PE_INF_HW_H15A_RD_PXL_LED_VER_POS, rd_data[10]);
					PE_INF_H15_RD_CASE_PRMENU(13, rd_cnt++, buffer, PE_INF_HW_H15A_RD_PXL_LED_MARK_EN, rd_data[11]);
					PE_INF_H15_RD_CASE_PRMENU(14, rd_cnt++, buffer, PE_INF_HW_H15A_RD_PXL_LED_PIXEL_R, rd_data[12]);
					PE_INF_H15_RD_CASE_PRMENU(15, rd_cnt++, buffer, PE_INF_HW_H15A_RD_PXL_LED_PIXEL_G, rd_data[13]);
					PE_INF_H15_RD_CASE_PRMENU(16, rd_cnt++, buffer, PE_INF_HW_H15A_RD_PXL_LED_PIXEL_B, rd_data[14]);
					PE_INF_H15_RD_CASE_SPRINT(17, rd_cnt++, buffer, PE_INF_H15_RD_PRINT_RESERV);
					PE_INF_H15_RD_CASE_SPRINT(18, rd_cnt++, buffer, PE_INF_H15_RD_PRINT_RESERV);
					PE_INF_H15_RD_DFLT_SPRINT(rd_cnt=0, buffer, PE_INF_H15_RD_PRINT_END);
				}
			}
			else
			{
				sprintf(buffer, PE_INF_H15_RD_PRINT_EXIT);
				rd_cnt=0;
			}
			memcpy(pstParams->data,buffer,sizeof(char)*PE_TRACE_STR_SIZE);
		}
		else
		{
			sprintf(buffer, PE_INF_H15_RD_PRINT_EXIT);
			memcpy(pstParams->data,buffer,sizeof(char)*PE_TRACE_STR_SIZE);
		}
	}while (0);
	return ret;
}

/**
 * get current ddr top header.
 *
 * @param   *pstParams [in/out] void, see PE_DDR_H15_TOP_HEADER_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_INF_H15_GetCurDdrTopHeader(void *pstParams)
{
	int ret = RET_OK;
	UINT32 i, base, size;
	volatile PE_DDR_H15_TOP_HEADER_T *p_top  = NULL;
	PE_DDR_H15_TOP_HEADER_T *p_dst  = NULL;
	PE_DDR_DB_SHDW_REG_H15_T *p_shdw_db = gPE_DDR_DB_H15.shdw.data;
	do {
		CHECK_KNULL(pstParams);
		base = p_shdw_db->top_h.db_base;
		size = sizeof(PE_DDR_H15_TOP_HEADER_T);
		p_top = (volatile PE_DDR_H15_TOP_HEADER_T *)ioremap(base, size);
		CHECK_KNULL(p_top);
		p_dst = (PE_DDR_H15_TOP_HEADER_T *)pstParams;
		p_dst->version = p_top->version;
		p_dst->db_base = p_top->db_base;
		p_dst->sec_num = p_top->sec_num;
		for (i=0; i<PE_DDR_H15_SEC_MAX_SIZE; i++)
		{
			p_dst->sec_base[i] = p_top->sec_base[i];
			p_dst->sec_size[i] = p_top->sec_size[i];
		}
	}while(0);
	if (p_top) iounmap((void *)p_top);
	return ret;
}
/**
 * get current ddr section header.
 *
 * @param   *top_h [in] void, see PE_DDR_H15_TOP_HEADER_T
 * @param   *pstParams [in/out] void, see PE_DDR_H15_HEADER_T
 * @param   sec_type [in] UINT32, see PE_DDR_H15_SEC_TYPE
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_INF_H15_GetCurDdrSecHeader(void *top_h, void *pstParams, UINT32 sec_type)
{
	int ret = RET_OK;
	UINT32 base, size;
	PE_DDR_H15_TOP_HEADER_T *p_top = NULL;
	PE_DDR_H15_HEADER_T *p_dst = NULL;
	volatile PE_DDR_H15_HEADER_T *p_sec  = NULL;
	do {
		CHECK_KNULL(top_h);
		CHECK_KNULL(pstParams);
		PE_INF_H15_CHECK_CODE(sec_type>=PE_DDR_H15_SEC_NUM,break,\
			"[%s,%d] sec_type(%d) is over.\n", __F__, __L__, sec_type);
		p_top = (PE_DDR_H15_TOP_HEADER_T *)top_h;
		base = p_top->sec_base[sec_type];
		size = sizeof(PE_DDR_H15_HEADER_T);
		p_sec  = (volatile PE_DDR_H15_HEADER_T *)ioremap(base, size);
		CHECK_KNULL(p_sec);
		p_dst = (PE_DDR_H15_HEADER_T *)pstParams;
		p_dst->version = p_sec->version;
		p_dst->sec_type = p_sec->sec_type;
		p_dst->item_num = p_sec->item_num;
		p_dst->item_size = p_sec->item_size;
	}while(0);
	if (p_sec) iounmap((void *)p_sec);
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
int PE_INF_H15_SetTracePacketData(LX_PE_PKT_T *pstParams)
{
    int ret = RET_OK;
	char str[PE_TRACE_STR_SIZE];
	UINT32 rw_t=0, mdl_t=0, fn_t=0, valid=0, d_size=0;
	UINT32 *d_arg = NULL;

#define PE_INF_H15_CPY_PARAM_TO_STR(_struct, _fmt, _args...)	\
	{\
		_struct p;\
		ret = copy_from_user(&p, (void __user *)d_arg, d_size);\
		PE_INF_H15_CHECK_CODE(ret, break, "copy_from_user error.\n");\
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
				PE_INF_H15_CPY_PARAM_TO_STR(LX_PE_INIT_SETTINS_T, \
					"%d, %d", p.win_id, p.suspend_mode);
				break;
			case PE_ITEM_PKTRW(SET):
				switch (mdl_t)
				{
					case PE_ITEM_PKTMODL(DEFAULT):
						PE_INF_H15_CPY_PARAM_TO_STR(LX_PE_DEFAULT_SETTINGS_T, \
							"%d", p.win_id);
						break;
					case PE_ITEM_PKTMODL(CMN):
						switch (fn_t)
						{
							#ifdef PE_INF_H15_DETAIL_TRACE_LOG
							case PE_ITEM_PKTFUNC(LX_PE_CMN_CONTRAST_T):
								PE_INF_H15_CPY_PARAM_TO_STR(LX_PE_CMN_CONTRAST_T, \
									"%d, %d", p.win_id, p.contrast);
								break;
							case PE_ITEM_PKTFUNC(LX_PE_CMN_BRIGHTNESS_T):
								PE_INF_H15_CPY_PARAM_TO_STR(LX_PE_CMN_BRIGHTNESS_T, \
									"%d, %d", p.win_id, p.brightness);
								break;
							case PE_ITEM_PKTFUNC(LX_PE_CMN_SATURATION_T):
								PE_INF_H15_CPY_PARAM_TO_STR(LX_PE_CMN_SATURATION_T, \
									"%d, %d", p.win_id, p.saturation);
								break;
							case PE_ITEM_PKTFUNC(LX_PE_CMN_HUE_T):
								PE_INF_H15_CPY_PARAM_TO_STR(LX_PE_CMN_HUE_T, \
									"%d, %d", p.win_id, p.hue);
								break;
							case PE_ITEM_PKTFUNC(LX_PE_CMN_LEVEL_CTRL_T):
								PE_INF_H15_CPY_PARAM_TO_STR(LX_PE_CMN_LEVEL_CTRL_T, \
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
								PE_INF_H15_CPY_PARAM_TO_STR(LX_PE_CSC_POST_T, \
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
								PE_INF_H15_CPY_PARAM_TO_STR(LX_PE_CSC_INPUT_T, \
									"%d, %d, %x,%x,%x,%x,%x,%x,%x,%x,%x, "\
									"%d,%d,%d,%d,%d,%d", p.win_id, p.enable, \
									p.matrix[0], p.matrix[1], p.matrix[2], \
									p.matrix[3], p.matrix[4], p.matrix[5], \
									p.matrix[6], p.matrix[7], p.matrix[8], \
									(SINT16)p.offset[0], (SINT16)p.offset[1], \
									(SINT16)p.offset[2], (SINT16)p.offset[3], \
									(SINT16)p.offset[4], (SINT16)p.offset[5]);
								break;
							#ifdef PE_INF_H15_DETAIL_TRACE_LOG
							case PE_ITEM_PKTFUNC(LX_PE_CSC_XVYCC_T):
								PE_INF_H15_CPY_PARAM_TO_STR(LX_PE_CSC_XVYCC_T, \
									"%d, %d,%d", p.win_id, p.enable, p.scaler);
								break;
							case PE_ITEM_PKTFUNC(LX_PE_CSC_GAMUT_T):
								PE_INF_H15_CPY_PARAM_TO_STR(LX_PE_CSC_GAMUT_T, \
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
							#ifdef PE_INF_H15_DETAIL_TRACE_LOG
							case PE_ITEM_PKTFUNC(LX_PE_CMG_ENABLE_T):
								PE_INF_H15_CPY_PARAM_TO_STR(LX_PE_CMG_ENABLE_T, \
									"%d, %d", p.win_id, p.enable);
								break;
							case PE_ITEM_PKTFUNC(LX_PE_CMG_GLOBAL_CTRL_T):
								PE_INF_H15_CPY_PARAM_TO_STR(LX_PE_CMG_GLOBAL_CTRL_T, \
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
							#ifdef PE_INF_H15_DETAIL_TRACE_LOG
							case PE_ITEM_PKTFUNC(LX_PE_NRD_DNR2_CMN_T):
								PE_INF_H15_CPY_PARAM_TO_STR(LX_PE_NRD_DNR2_CMN_T, \
									"%d, %d,%d,%d", p.win_id, \
									p.enable_ac_bnr, p.enable_mnr, p.enable_dc_bnr);
								break;
							case PE_ITEM_PKTFUNC(LX_PE_NRD_DNR2_DETAIL_T):
								PE_INF_H15_CPY_PARAM_TO_STR(LX_PE_NRD_DNR2_DETAIL_T, \
									"%d", p.win_id);
								break;
							case PE_ITEM_PKTFUNC(LX_PE_NRD_TNR2_CMN_T):
								PE_INF_H15_CPY_PARAM_TO_STR(LX_PE_NRD_TNR2_CMN_T, \
									"%d, %d,%d,%d,%d,%d", p.win_id, \
									p.auto_mode_en, p.adjust_nt_lvl, \
									p.adjust_nt_lvl_val, p.y_gain, p.c_gain);
								break;
							case PE_ITEM_PKTFUNC(LX_PE_NRD_TNR2_DETAIL_T):
								PE_INF_H15_CPY_PARAM_TO_STR(LX_PE_NRD_TNR2_DETAIL_T, \
									"%d", p.win_id);
								break;
							case PE_ITEM_PKTFUNC(LX_PE_NRD_DNR4_CMN_T):
								PE_INF_H15_CPY_PARAM_TO_STR(LX_PE_NRD_DNR4_CMN_T, \
									"%d, %d,%d,%d", p.win_id, \
									p.enable_ac_bnr, p.enable_mnr, p.enable_dc_bnr);
								break;
							case PE_ITEM_PKTFUNC(LX_PE_NRD_DNR4_DETAIL_T):
								PE_INF_H15_CPY_PARAM_TO_STR(LX_PE_NRD_DNR4_DETAIL_T, \
									"%d", p.win_id);
								break;
							case PE_ITEM_PKTFUNC(LX_PE_NRD_TNR4_CMN_T):
								PE_INF_H15_CPY_PARAM_TO_STR(LX_PE_NRD_TNR4_CMN_T, \
									"%d, %d,%d,%d,%d,%d", p.win_id, \
									p.auto_mode_en, p.adjust_nt_lvl, \
									p.adjust_nt_lvl_val, p.y_gain, p.c_gain);
								break;
							case PE_ITEM_PKTFUNC(LX_PE_NRD_TNR4_DETAIL_T):
								PE_INF_H15_CPY_PARAM_TO_STR(LX_PE_NRD_TNR4_DETAIL_T, \
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
								PE_INF_H15_CPY_PARAM_TO_STR(LX_PE_DNT_FILMMODE_T, \
									"%d, %d", p.win_id, p.enable);
								break;
							case PE_ITEM_PKTFUNC(LX_PE_DNT_LD_MODE_T):
								PE_INF_H15_CPY_PARAM_TO_STR(LX_PE_DNT_LD_MODE_T, \
									"%d, %d", p.win_id, p.enable);
								break;
							default:
								break;
						}
						break;
					case PE_ITEM_PKTMODL(SHP):
						switch (fn_t)
						{
						#ifdef PE_SHP_PARAM_CHANGE_FOR_H15
							case PE_ITEM_PKTFUNC(LX_PE_SHP_SCLFILTER_T):
								PE_INF_H15_CPY_PARAM_TO_STR(LX_PE_SHP_SCLFILTER_T, \
									"%d, %d,%d,%d,%d", p.win_id, \
									p.h_luma_id, p.v_luma_id, p.h_chrm_id, p.v_chrm_id);
								break;
							case PE_ITEM_PKTFUNC(LX_PE_SHP_SR0_T):
								PE_INF_H15_CPY_PARAM_TO_STR(LX_PE_SHP_SR0_T, \
									"%d, %d,%d,%d,0x%x,0x%x,%d", p.win_id, \
									p.reg_out_mux_sel, p.reg_sc_gs_sra, p.reg_sc_gs_srb, \
									p.reg_sc_ls_lv_sra, p.reg_sc_ls_lv_srb, p.reg_mode_wei);
								break;
							#ifdef PE_INF_H15_DETAIL_TRACE_LOG
							case PE_ITEM_PKTFUNC(LX_PE_SHP_EE1_T):
								PE_INF_H15_CPY_PARAM_TO_STR(LX_PE_SHP_EE1_T, \
									"%d, %d,%d,%d,%d", p.win_id, \
									p.mp_edge_gain_b, p.mp_edge_gain_w, \
									p.sp_edge_gain_b, p.sp_edge_gain_w);
								break;
							case PE_ITEM_PKTFUNC(LX_PE_SHP_DE1_T):
								PE_INF_H15_CPY_PARAM_TO_STR(LX_PE_SHP_DE1_T, \
									"%d, %d,%d,%d,%d", p.win_id, \
									p.mp_texture_gain_b, p.mp_texture_gain_w, \
									p.sp_texture_gain_b, p.sp_texture_gain_w);
								break;
							case PE_ITEM_PKTFUNC(LX_PE_SHP_RE3_CMN_T):
								PE_INF_H15_CPY_PARAM_TO_STR(LX_PE_SHP_RE3_CMN_T, \
									"%d", p.win_id);
								break;
							case PE_ITEM_PKTFUNC(LX_PE_SHP_RE2_HOR_T):
								PE_INF_H15_CPY_PARAM_TO_STR(LX_PE_SHP_RE3_HOR_T, \
									"%d", p.win_id);
								break;
							case PE_ITEM_PKTFUNC(LX_PE_SHP_RE2_VER_T):
								PE_INF_H15_CPY_PARAM_TO_STR(LX_PE_SHP_RE3_VER_T, \
									"%d", p.win_id);
								break;
							case PE_ITEM_PKTFUNC(LX_PE_SHP_RE2_MISC_T):
								PE_INF_H15_CPY_PARAM_TO_STR(LX_PE_SHP_RE3_MISC_T, \
									"%d", p.win_id);
								break;
							case PE_ITEM_PKTFUNC(LX_PE_SHP_RE3_MISC_T):
								PE_INF_H15_CPY_PARAM_TO_STR(LX_PE_SHP_RE3_MISC_T, \
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
						#else
							case PE_ITEM_PKTFUNC(LX_PE_SHP_SCLFILTER_T):
								PE_INF_H15_CPY_PARAM_TO_STR(LX_PE_SHP_SCLFILTER_T, \
									"%d, %d,%d,%d,%d", p.win_id, \
									p.h_luma_id, p.v_luma_id, p.h_chrm_id, p.v_chrm_id);
								break;
							case PE_ITEM_PKTFUNC(LX_PE_SHP_SR0_T):
								PE_INF_H15_CPY_PARAM_TO_STR(LX_PE_SHP_SR0_T, \
									"%d, %d,%d,%d,0x%x,0x%x,%d", p.win_id, \
									p.reg_out_mux_sel, p.reg_sc_gs_sra, p.reg_sc_gs_srb, \
									p.reg_sc_ls_lv_sra, p.reg_sc_ls_lv_srb, p.reg_mode_wei);
								break;
							#ifdef PE_INF_H15_DETAIL_TRACE_LOG
							case PE_ITEM_PKTFUNC(LX_PE_SHP_EE1_T):
								PE_INF_H15_CPY_PARAM_TO_STR(LX_PE_SHP_EE1_T, \
									"%d, %d,%d,%d,%d", p.win_id, \
									p.mp_edge_gain_b, p.mp_edge_gain_w, \
									p.sp_edge_gain_b, p.sp_edge_gain_w);
								break;
							case PE_ITEM_PKTFUNC(LX_PE_SHP_DE1_T):
								PE_INF_H15_CPY_PARAM_TO_STR(LX_PE_SHP_DE1_T, \
									"%d, %d,%d,%d,%d", p.win_id, \
									p.mp_texture_gain_b, p.mp_texture_gain_w, \
									p.sp_texture_gain_b, p.sp_texture_gain_w);
								break;
							case PE_ITEM_PKTFUNC(LX_PE_SHP_RE2_CMN_T):
								PE_INF_H15_CPY_PARAM_TO_STR(LX_PE_SHP_RE2_CMN_T, \
									"%d", p.win_id);
								break;
							case PE_ITEM_PKTFUNC(LX_PE_SHP_RE2_HOR_T):
								PE_INF_H15_CPY_PARAM_TO_STR(LX_PE_SHP_RE2_HOR_T, \
									"%d", p.win_id);
								break;
							case PE_ITEM_PKTFUNC(LX_PE_SHP_RE2_VER_T):
								PE_INF_H15_CPY_PARAM_TO_STR(LX_PE_SHP_RE2_VER_T, \
									"%d", p.win_id);
								break;
							case PE_ITEM_PKTFUNC(LX_PE_SHP_RE2_MISC_T):
								PE_INF_H15_CPY_PARAM_TO_STR(LX_PE_SHP_RE2_MISC_T, \
									"%d", p.win_id);
								break;
							case PE_ITEM_PKTFUNC(LX_PE_SHP_RE3_MISC_T):
								PE_INF_H15_CPY_PARAM_TO_STR(LX_PE_SHP_RE3_MISC_T, \
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
						#endif
							
						}
						break;
					case PE_ITEM_PKTMODL(CCM):
						switch (fn_t)
						{
							case PE_ITEM_PKTFUNC(LX_PE_CCM_AUTO_CR_T):
								PE_INF_H15_CPY_PARAM_TO_STR(LX_PE_CCM_AUTO_CR_T, \
									"%d,%d,%d,%d,%d", p.enable, p.th_max_hist, \
									p.th_valid_bins, p.adj_th0, p.adj_th1);
								break;
							#ifdef PE_INF_H15_DETAIL_TRACE_LOG
							case PE_ITEM_PKTFUNC(LX_PE_CCM_GAMMALUT_T):
								PE_INF_H15_CPY_PARAM_TO_STR(LX_PE_CCM_GAMMALUT_T, \
									"%d, %d", p.win_id, p.size);
								break;
							case PE_ITEM_PKTFUNC(LX_PE_CCM_PIXEL_REPLACE_T):
								PE_INF_H15_CPY_PARAM_TO_STR(LX_PE_CCM_PIXEL_REPLACE_T, \
									"%d, %d,%d,%d", p.win_id, \
									p.pxl_rep_r, p.pxl_rep_g, p.pxl_rep_b);
								break;
							case PE_ITEM_PKTFUNC(LX_PE_CCM_WB_T):
								PE_INF_H15_CPY_PARAM_TO_STR(LX_PE_CCM_WB_T, \
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
							#ifdef PE_INF_H15_DETAIL_TRACE_LOG
							case PE_ITEM_PKTFUNC(LX_PE_DCM_DCE_CONF_T):
								PE_INF_H15_CPY_PARAM_TO_STR(LX_PE_DCM_DCE_CONF_T, \
									"%d, %d,%d,%d", p.win_id, \
									p.domain, p.min_pos, p.max_pos);
								break;
							case PE_ITEM_PKTFUNC(LX_PE_DCM_BLENDING_T):
								PE_INF_H15_CPY_PARAM_TO_STR(LX_PE_DCM_BLENDING_T, \
									"%d, %d,0x%x", p.win_id, \
									p.color_out_gain, p.region_sel);
								break;
							case PE_ITEM_PKTFUNC(LX_PE_DCM_DCE_SMOOTH0_T):
								PE_INF_H15_CPY_PARAM_TO_STR(LX_PE_DCM_DCE_SMOOTH0_T, \
									"%d", p.win_id);
								break;
							case PE_ITEM_PKTFUNC(LX_PE_DCM_DCE_SMOOTH1_T):
								PE_INF_H15_CPY_PARAM_TO_STR(LX_PE_DCM_DCE_SMOOTH1_T, \
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
							#ifdef PE_INF_H15_DETAIL_TRACE_LOG
							case PE_ITEM_PKTFUNC(LX_PE_HST_HISTO_CFG_T):
								PE_INF_H15_CPY_PARAM_TO_STR(LX_PE_HST_HISTO_CFG_T, \
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
								PE_INF_H15_CPY_PARAM_TO_STR(LX_PE_INF_DISPLAY_T, \
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
			ret = PE_INF_H15_SetTraceDataBuf(str);
			PE_INF_H15_CHECK_CODE(ret, break, \
				"PE_INF_H15_SetTraceDataBuf() error.\n");
		}
	}while (0);
	return ret;
}

