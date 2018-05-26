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

/** @file pe_shp_hw_h13.c
 *
 *  driver for picture enhance sharpness functions. ( used only within kdriver )
 *
 *	@author		Seung-Jun,Youm(sj.youm@lge.com)
 *	@version	0.1
 *	@note
 *	@date		2011.04.12
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
#include "pe_shp_hw_def.h"
#include "pe_shp_hw_h13.h"
#include "pe_shp_hw_h13a0.h"
#include "pe_shp_hw_h13b0.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
#define PE_SHP_HW_H13_FHD_H_MAX			1920
#define PE_SHP_HW_H13_FHD_V_MAX			1080
#define PE_SHP_HW_H13_SCL_IN_H_MIN		160
#define PE_SHP_HW_H13_SCL_IN_V_MIN		120
#define PE_SHP_HW_H13_IDX_Y_DFLT		15
#define PE_SHP_HW_H13_IDX_C_DFLT		39

/* h,v only(define) vs. both(undef), Now we use both mode until getting c08t scl has no problem. */
#undef PE_SHP_HW_H13_USE_V12TM_C08T_SEPARATE_HV

/* use MSR vs. SSC on venc mode, We are going to use SSC from H13B0. */
#undef PE_SHP_HW_H13_USE_MSR_FOR_VENC_ON

/* use shp mode. */
#define PE_SHP_HW_H13_USE_SHP_MODE

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define PE_SHP_HW_H13_ERROR	printk

#define PE_SHP_HW_H13_DBG_PRINT(fmt,args...)	\
	if(_g_shp_hw_h13_trace) printk("[%x,%x][%s,%d] "fmt,PE_CHIP_VER,g_pe_kdrv_ver_mask,__F__,__L__,##args)
#define PE_SHP_HW_H13_CHECK_CODE(_checker,_action,fmt,args...)	\
	{if(_checker){PE_SHP_HW_H13_ERROR(fmt,##args);_action;}}
#define PE_SHP_HW_H13_DBG_PRINT_FMT_STATE(_pre,_cur)	\
	PE_SHP_HW_H13_DBG_PRINT("%s(%d),%s(%d) : %s.\n",\
	#_pre,(_pre),#_cur,(_cur),((_pre)!=(_cur))? "different":"same")
#define PE_SHP_HW_H13_DBG_CHECK_CODE(_cnt,_checker,_action,fmt,args...)	\
	{\
		if(_checker){\
			if(_g_shp_hw_h13_trace&&_cnt>PE_PRINT_COUNT_NUM)\
			{\
				PE_SHP_HW_H13_DBG_PRINT(fmt,##args);\
			}\
			_action;\
		}\
	}

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/
/**
* scaler wr tap type
*	MSC_CTRL_L 0x1100 [2]v_scaler_config
* - v12t mode '0' : Luminance H12tap/V12tap, Chrominance H08tap/V08tap (SD input stream)
* - v06t mode '1' : Luminance H08tap/V06tap, Chrominance H08tap/V04tap (HD and SD input stream both)
*/
typedef enum
{
	PE_SHP_HW_H13_SCL_WR_TAP_V12TM_Y_H_12T = 0,		///< v12tap mode and luma horizontal 12tap
	PE_SHP_HW_H13_SCL_WR_TAP_V12TM_Y_V_12T,			///< v12tap mode and luma vertical 12tap
	PE_SHP_HW_H13_SCL_WR_TAP_V12TM_C_H_08T,			///< v12tap mode and chroma horizontal 08tap
	PE_SHP_HW_H13_SCL_WR_TAP_V12TM_C_V_08T,			///< v12tap mode and chroma vertical 08tap
	PE_SHP_HW_H13_SCL_WR_TAP_V06TM_Y_H_08T,			///< v06tap mode and luma horizontal 08tap
	PE_SHP_HW_H13_SCL_WR_TAP_V06TM_Y_V_06T,			///< v06tap mode and luma vertical 06tap
	PE_SHP_HW_H13_SCL_WR_TAP_V06TM_C_H_08T,			///< v06tap mode and chroma horizontal 08tap
	PE_SHP_HW_H13_SCL_WR_TAP_V06TM_C_V_04T,			///< v06tap mode and chroma vertical 04tap
	PE_SHP_HW_H13_SCL_WR_TAP_NUM					///< max num
}
PE_SHP_HW_H13_SCL_WR_TAP;

/**
* scl setting flag type
*	- to avoid double setting scaler filter
*/
typedef struct
{
	UINT32 h_y;	///< horizontal luma
	UINT32 v_y;	///< vertical luma
	UINT32 h_c;	///< horizontal chroma
	UINT32 v_c;	///< vertical chroma
}
PE_SHP_HW_H13_FLAG_T;

/**
* scaler wr tap control type
*/
typedef struct
{
	UINT32 base_addr;	///< base address
	UINT32 tbl_size;	///< table size
	UINT32 tap_type;	///< tap type, see PE_SHP_HW_H13_SCL_WR_TAP
}
PE_SHP_HW_H13_SCL_WR_CTRL_T;

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
static int PE_SHP_HW_H13B_SetDefault(LX_PE_DEFAULT_SETTINGS_T *pstParams);
static int PE_SHP_HW_H13A_SetDefault(LX_PE_DEFAULT_SETTINGS_T *pstParams);
static PE_SHP_HW_H13B_RES_FMT PE_SHP_HW_H13B_ConvDispInfoToResFmt(LX_PE_INF_DISPLAY_T *disp_inf);
static PE_SHP_HW_H13A_RES_FMT PE_SHP_HW_H13A_ConvDispInfoToResFmt(LX_PE_INF_DISPLAY_T *disp_inf);
static PE_SHP_HW_H13B_VFC_FMT PE_SHP_HW_H13B_ConvDispInfoToVfcFmt(LX_PE_INF_DISPLAY_T *disp_inf);
static PE_SHP_HW_H13A_VFC_FMT PE_SHP_HW_H13A_ConvDispInfoToVfcFmt(LX_PE_INF_DISPLAY_T *disp_inf);
static int PE_SHP_HW_H13_GetScalerFilterValue(PE_TSK_H13_CFG_T *pCfg,PE_SHP_HW_H13_SETTINGS_T *pstParams);
static void PE_SHP_HW_H13_CheckValidScalerFilterValue(PE_SHP_HW_H13_SETTINGS_T *pstParams);
static PE_SHP_HW_H13_FLAG_T PE_SHP_HW_H13_GetDoubleSetFlag(LX_PE_WIN_ID win_id,LX_PE_SHP_SCLFILTER_T pre_idx,PE_SHP_HW_H13_SETTINGS_T *cur_param);
static int PE_SHP_HW_H13_SetSclCoeff(LX_PE_WIN_ID win_id, PE_SHP_HW_H13_FLAG_T flag, PE_SHP_HW_H13_SETTINGS_T *pstParams);
static void PE_SHP_HW_H13_WrSclCoeff(PE_SHP_HW_H13_SCL_WR_CTRL_T wr_ctrl, const PE_REG_PARAM_T *reg);
static void PE_SHP_HW_H13_SetSclFltRnwMode(UINT32 rnw_mode);
static UINT32 PE_SHP_HW_H13_RdLumaSclFltCoeff(LX_PE_WIN_ID win_id, UINT32 config, UINT32 *data);
static UINT32 PE_SHP_HW_H13_RdChromaSclFltCoeff(LX_PE_WIN_ID win_id, UINT32 config, UINT32 *data);
static UINT32 PE_SHP_HW_H13_RdSubSclFltCoeff(UINT32 config, UINT32 *data);
#ifdef PE_SHP_HW_H13_USE_SHP_MODE
static int PE_SHP_HW_H13_SetShpMode(PE_TSK_H13_CFG_T *pCfg);
#endif

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
static UINT8 _g_shp_hw_h13_trace=0x0;	//default should be off.
static PE_SHP_HW_H13_SETTINGS_T _g_pe_shp_hw_h13_info;

/*----------------------------------------------------------------------------------------
	Implementation Group
----------------------------------------------------------------------------------------*/
/**
 * init shp
 *
 * @param   *pstParams [in] PE_CFG_CTRL_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_SHP_HW_H13_Init(PE_CFG_CTRL_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H13)
		{
			PE_SHP_HW_H13_DBG_PRINT("init param.\n");
			/* 20130613, sj.youm, remove memset0 and add tap and fir init */
			_g_pe_shp_hw_h13_info.cur_scl_idx[LX_PE_WIN_0].win_id = LX_PE_WIN_0;
			_g_pe_shp_hw_h13_info.cur_scl_idx[LX_PE_WIN_0].h_luma_id = PE_SHP_HW_H13_IDX_Y_DFLT;
			_g_pe_shp_hw_h13_info.cur_scl_idx[LX_PE_WIN_0].v_luma_id = PE_SHP_HW_H13_IDX_Y_DFLT;
			_g_pe_shp_hw_h13_info.cur_scl_idx[LX_PE_WIN_0].h_chrm_id = PE_SHP_HW_H13_IDX_C_DFLT;
			_g_pe_shp_hw_h13_info.cur_scl_idx[LX_PE_WIN_0].v_chrm_id = PE_SHP_HW_H13_IDX_C_DFLT;
			_g_pe_shp_hw_h13_info.cur_scl_idx[LX_PE_WIN_1].win_id = LX_PE_WIN_1;
			_g_pe_shp_hw_h13_info.cur_scl_idx[LX_PE_WIN_1].h_luma_id = PE_SHP_HW_H13_IDX_Y_DFLT;
			_g_pe_shp_hw_h13_info.cur_scl_idx[LX_PE_WIN_1].v_luma_id = PE_SHP_HW_H13_IDX_Y_DFLT;
			_g_pe_shp_hw_h13_info.cur_scl_idx[LX_PE_WIN_1].h_chrm_id = PE_SHP_HW_H13_IDX_C_DFLT;
			_g_pe_shp_hw_h13_info.cur_scl_idx[LX_PE_WIN_1].v_chrm_id = PE_SHP_HW_H13_IDX_C_DFLT;
			_g_pe_shp_hw_h13_info.pre_scl_idx[LX_PE_WIN_0].win_id = LX_PE_WIN_0;
			_g_pe_shp_hw_h13_info.pre_scl_idx[LX_PE_WIN_0].h_luma_id = 0xff;
			_g_pe_shp_hw_h13_info.pre_scl_idx[LX_PE_WIN_0].v_luma_id = 0xff;
			_g_pe_shp_hw_h13_info.pre_scl_idx[LX_PE_WIN_0].h_chrm_id = 0xff;
			_g_pe_shp_hw_h13_info.pre_scl_idx[LX_PE_WIN_0].v_chrm_id = 0xff;
			_g_pe_shp_hw_h13_info.pre_scl_idx[LX_PE_WIN_1].win_id = LX_PE_WIN_1;
			_g_pe_shp_hw_h13_info.pre_scl_idx[LX_PE_WIN_1].h_luma_id = 0xff;
			_g_pe_shp_hw_h13_info.pre_scl_idx[LX_PE_WIN_1].v_luma_id = 0xff;
			_g_pe_shp_hw_h13_info.pre_scl_idx[LX_PE_WIN_1].h_chrm_id = 0xff;
			_g_pe_shp_hw_h13_info.pre_scl_idx[LX_PE_WIN_1].v_chrm_id = 0xff;
			_g_pe_shp_hw_h13_info.h_y_tap[LX_PE_WIN_0] = PE_SHP_HW_H13_SCL_NUM;
			_g_pe_shp_hw_h13_info.h_c_tap[LX_PE_WIN_0] = PE_SHP_HW_H13_SCL_NUM;
			_g_pe_shp_hw_h13_info.v_y_tap[LX_PE_WIN_0] = PE_SHP_HW_H13_SCL_NUM;
			_g_pe_shp_hw_h13_info.v_c_tap[LX_PE_WIN_0] = PE_SHP_HW_H13_SCL_NUM;
			_g_pe_shp_hw_h13_info.h_y_tap[LX_PE_WIN_1] = PE_SHP_HW_H13_SCL_NUM;
			_g_pe_shp_hw_h13_info.h_c_tap[LX_PE_WIN_1] = PE_SHP_HW_H13_SCL_NUM;
			_g_pe_shp_hw_h13_info.v_y_tap[LX_PE_WIN_1] = PE_SHP_HW_H13_SCL_NUM;
			_g_pe_shp_hw_h13_info.v_c_tap[LX_PE_WIN_1] = PE_SHP_HW_H13_SCL_NUM;
			_g_pe_shp_hw_h13_info.y_fir[LX_PE_WIN_0] = PE_SHP_HW_H13_FIR_NUM;
			_g_pe_shp_hw_h13_info.c_fir[LX_PE_WIN_0] = PE_SHP_HW_H13_FIR_NUM;
			_g_pe_shp_hw_h13_info.y_fir[LX_PE_WIN_1] = PE_SHP_HW_H13_FIR_NUM;
			_g_pe_shp_hw_h13_info.c_fir[LX_PE_WIN_1] = PE_SHP_HW_H13_FIR_NUM;
			_g_pe_shp_hw_h13_info.pre_res_en[LX_PE_WIN_0] = 0xff;
			_g_pe_shp_hw_h13_info.pre_res_en[LX_PE_WIN_1] = 0xff;
			if(PE_KDRV_VER_H13BX)
			{
				PE_SET_REG_TABLE(SHP_HW_H13,shp_l_init_h13b0,PE_REG_H13_WRITE_BASE);
				PE_SET_REG_TABLE(SHP_HW_H13,shp_r_init_h13b0,PE_REG_H13_WRITE_BASE);
			}
			else if(PE_KDRV_VER_H13AX)
			{
				PE_SET_REG_TABLE(SHP_HW_H13,shp_l_init_h13a0,0);
			}
		}
		else
		{
			PE_SHP_HW_H13_DBG_PRINT("nothing to do\n");	ret = RET_OK;
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
int PE_SHP_HW_H13_SetDebugSettings(LX_PE_DBG_SETTINGS_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		PE_SHP_HW_H13_DBG_PRINT("wid:%d,type:0x%x,[0x%x]print:0x%x,bypass:0x%x\n",\
			pstParams->win_id,pstParams->type,(0x1<<PE_ITEM_PKTMODL(SHP)),\
			pstParams->print_lvl,pstParams->bypass);
		/* set print level */
		if(pstParams->type&LX_PE_DBG_LV)
		{
			_g_shp_hw_h13_trace = (pstParams->print_lvl & (0x1<<PE_ITEM_PKTMODL(SHP)))? 0x1:0x0;
		}
	}while(0);
	return ret;
}
/**
 * default shp setting
 *
 * @param   *pstParams [in] LX_PE_DEFAULT_SETTINGS_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_SHP_HW_H13_SetDefault(LX_PE_DEFAULT_SETTINGS_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H13BX)
		{
			ret = PE_SHP_HW_H13B_SetDefault(pstParams);
			PE_SHP_HW_H13_CHECK_CODE(ret,break,"[%s,%d] PE_SHP_HW_H13B_SetDefault() error.\n",__F__,__L__);
		}
		else if(PE_KDRV_VER_H13AX)
		{
			ret = PE_SHP_HW_H13A_SetDefault(pstParams);
			PE_SHP_HW_H13_CHECK_CODE(ret,break,"[%s,%d] PE_SHP_HW_H13A_SetDefault() error.\n",__F__,__L__);
		}
		else
		{
			PE_SHP_HW_H13_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * default shp setting(for H13BX)
 *
 * @param   *pstParams [in] LX_PE_DEFAULT_SETTINGS_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_SHP_HW_H13B_SetDefault(LX_PE_DEFAULT_SETTINGS_T *pstParams)
{
	int ret = RET_OK;
	PE_INF_H13_SETTINGS_T inf_set;
	LX_PE_INF_DISPLAY_T *cur0_inf = NULL;
	LX_PE_INF_DISPLAY_T *cur1_inf = NULL;
	PE_SHP_HW_H13B_RES_FMT cur0_res_fmt,cur1_res_fmt;
	PE_SHP_HW_H13B_VFC_FMT cur0_vfc_fmt,cur1_vfc_fmt;
	static PE_SHP_HW_H13B_RES_FMT pre0_res_fmt = PE_SHP_H13B_RES_NUM;
	static PE_SHP_HW_H13B_RES_FMT pre1_res_fmt = PE_SHP_H13B_RES_NUM;
	static PE_SHP_HW_H13B_VFC_FMT pre0_vfc_fmt = PE_SHP_H13B_VFC_NUM;
	static PE_SHP_HW_H13B_VFC_FMT pre1_vfc_fmt = PE_SHP_H13B_VFC_NUM;
	PE_SHP_HW_H13_SETTINGS_T *pInfo=&_g_pe_shp_hw_h13_info;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		ret = PE_INF_H13_GetCurInfSettings(&inf_set);
		PE_SHP_HW_H13_CHECK_CODE(ret,break,"[%s,%d] PE_INF_H13_GetCurInfSettings() error.\n",__F__,__L__);
		cur0_inf=&inf_set.disp_info[LX_PE_WIN_0];
		cur1_inf=&inf_set.disp_info[LX_PE_WIN_1];
		CHECK_KNULL(cur0_inf);
		CHECK_KNULL(cur1_inf);
		/* L : resolution enhance */
		if(PE_CHECK_WIN0(pstParams->win_id))
		{
			cur0_res_fmt = PE_SHP_HW_H13B_ConvDispInfoToResFmt(cur0_inf);
			PE_SHP_HW_H13_DBG_PRINT_FMT_STATE(pre0_res_fmt,cur0_res_fmt);
			if(pre0_res_fmt!=cur0_res_fmt)
			{
				switch(cur0_res_fmt)
				{
					case PE_SHP_H13B_RES_RF_NTSC:
						PE_SET_REG_TABLE(SHP_HW_H13,shp_l_rf_ntsc_default_h13b0,	PE_REG_H13_WRITE_BASE);
						break;
					case PE_SHP_H13B_RES_RF_PAL:
						PE_SET_REG_TABLE(SHP_HW_H13,shp_l_rf_pal_default_h13b0,		PE_REG_H13_WRITE_BASE);
						break;
					case PE_SHP_H13B_RES_AV_NTSC:
						PE_SET_REG_TABLE(SHP_HW_H13,shp_l_av_ntsc_default_h13b0,	PE_REG_H13_WRITE_BASE);
						break;
					case PE_SHP_H13B_RES_AV_PAL:
						PE_SET_REG_TABLE(SHP_HW_H13,shp_l_av_pal_default_h13b0,		PE_REG_H13_WRITE_BASE);
						break;
					case PE_SHP_H13B_RES_PC:
						PE_SET_REG_TABLE(SHP_HW_H13,shp_l_pc_default_h13b0,			PE_REG_H13_WRITE_BASE);
						break;
					case PE_SHP_H13B_RES_DTV_SD:
						PE_SET_REG_TABLE(SHP_HW_H13,shp_l_dtv_sd_default_h13b0,		PE_REG_H13_WRITE_BASE);
						break;
					case PE_SHP_H13B_RES_DTV_HD:
						PE_SET_REG_TABLE(SHP_HW_H13,shp_l_dtv_hd_default_h13b0,		PE_REG_H13_WRITE_BASE);
						break;
					case PE_SHP_H13B_RES_DTV_PHOTO:
						PE_SET_REG_TABLE(SHP_HW_H13,shp_l_dtv_photo_default_h13b0,	PE_REG_H13_WRITE_BASE);
						break;
					case PE_SHP_H13B_RES_COMP480I:
						PE_SET_REG_TABLE(SHP_HW_H13,shp_l_comp480i_default_h13b0,	PE_REG_H13_WRITE_BASE);
						break;
					case PE_SHP_H13B_RES_COMP480P:
						PE_SET_REG_TABLE(SHP_HW_H13,shp_l_comp480p_default_h13b0,	PE_REG_H13_WRITE_BASE);
						break;
					case PE_SHP_H13B_RES_HDMI480I:
						PE_SET_REG_TABLE(SHP_HW_H13,shp_l_hdmi480i_default_h13b0,	PE_REG_H13_WRITE_BASE);
						break;
					case PE_SHP_H13B_RES_HDMI480P:
						PE_SET_REG_TABLE(SHP_HW_H13,shp_l_hdmi480p_default_h13b0,	PE_REG_H13_WRITE_BASE);
						break;
					case PE_SHP_H13B_RES_HDMI_HD:
						PE_SET_REG_TABLE(SHP_HW_H13,shp_l_hdmi_hd_default_h13b0,	PE_REG_H13_WRITE_BASE);
						break;
					case PE_SHP_H13B_RES_3D_TB_SS:
						PE_SET_REG_TABLE(SHP_HW_H13,shp_l_3d_tb_ss_default_h13b0,	PE_REG_H13_WRITE_BASE);
						break;
					default:
						break;
				}
				pre0_res_fmt=cur0_res_fmt;
				/* reset the previous resolution enable to avoid overlapping. */
				pInfo->pre_res_en[LX_PE_WIN_0]=0xff;
			}
		}
		/* R : resolution enhance */
		if(PE_CHECK_WIN1(pstParams->win_id))
		{
			cur1_res_fmt = PE_SHP_HW_H13B_ConvDispInfoToResFmt(cur1_inf);
			PE_SHP_HW_H13_DBG_PRINT_FMT_STATE(pre1_res_fmt,cur1_res_fmt);
			if(pre1_res_fmt!=cur1_res_fmt)
			{
				switch(cur1_res_fmt)
				{
					case PE_SHP_H13B_RES_RF_NTSC:
						PE_SET_REG_TABLE(SHP_HW_H13,shp_r_rf_ntsc_default_h13b0,	PE_REG_H13_WRITE_BASE);
						break;
					case PE_SHP_H13B_RES_RF_PAL:
						PE_SET_REG_TABLE(SHP_HW_H13,shp_r_rf_pal_default_h13b0,		PE_REG_H13_WRITE_BASE);
						break;
					case PE_SHP_H13B_RES_AV_NTSC:
						PE_SET_REG_TABLE(SHP_HW_H13,shp_r_av_ntsc_default_h13b0,	PE_REG_H13_WRITE_BASE);
						break;
					case PE_SHP_H13B_RES_AV_PAL:
						PE_SET_REG_TABLE(SHP_HW_H13,shp_r_av_pal_default_h13b0,		PE_REG_H13_WRITE_BASE);
						break;
					case PE_SHP_H13B_RES_PC:
						PE_SET_REG_TABLE(SHP_HW_H13,shp_r_pc_default_h13b0,			PE_REG_H13_WRITE_BASE);
						break;
					case PE_SHP_H13B_RES_DTV_SD:
						PE_SET_REG_TABLE(SHP_HW_H13,shp_r_dtv_sd_default_h13b0,		PE_REG_H13_WRITE_BASE);
						break;
					case PE_SHP_H13B_RES_DTV_HD:
						PE_SET_REG_TABLE(SHP_HW_H13,shp_r_dtv_hd_default_h13b0,		PE_REG_H13_WRITE_BASE);
						break;
					case PE_SHP_H13B_RES_DTV_PHOTO:
						PE_SET_REG_TABLE(SHP_HW_H13,shp_r_dtv_photo_default_h13b0,	PE_REG_H13_WRITE_BASE);
						break;
					case PE_SHP_H13B_RES_COMP480I:
						PE_SET_REG_TABLE(SHP_HW_H13,shp_r_comp480i_default_h13b0,	PE_REG_H13_WRITE_BASE);
						break;
					case PE_SHP_H13B_RES_COMP480P:
						PE_SET_REG_TABLE(SHP_HW_H13,shp_r_comp480p_default_h13b0,	PE_REG_H13_WRITE_BASE);
						break;
					case PE_SHP_H13B_RES_HDMI480I:
						PE_SET_REG_TABLE(SHP_HW_H13,shp_r_hdmi480i_default_h13b0,	PE_REG_H13_WRITE_BASE);
						break;
					case PE_SHP_H13B_RES_HDMI480P:
						PE_SET_REG_TABLE(SHP_HW_H13,shp_r_hdmi480p_default_h13b0,	PE_REG_H13_WRITE_BASE);
						break;
					case PE_SHP_H13B_RES_HDMI_HD:
						PE_SET_REG_TABLE(SHP_HW_H13,shp_r_hdmi_hd_default_h13b0,	PE_REG_H13_WRITE_BASE);
						break;
					case PE_SHP_H13B_RES_3D_TB_SS:
						PE_SET_REG_TABLE(SHP_HW_H13,shp_r_3d_tb_ss_default_h13b0,	PE_REG_H13_WRITE_BASE);
						break;
					default:
						break;
				}
				pre1_res_fmt=cur1_res_fmt;
			}
		}
		/* L : vertical filter chroma */
		if(PE_CHECK_WIN0(pstParams->win_id))
		{
			cur0_vfc_fmt = PE_SHP_HW_H13B_ConvDispInfoToVfcFmt(cur0_inf);
			PE_SHP_HW_H13_DBG_PRINT_FMT_STATE(pre0_vfc_fmt,cur0_vfc_fmt);
			if(pre0_vfc_fmt!=cur0_vfc_fmt)
			{
				switch(cur0_vfc_fmt)
				{
					case PE_SHP_H13B_VFC_HD:
						PE_SET_REG_TABLE(SHP_HW_H13,vflt_c_l_hd_default_h13b0,				PE_REG_H13_WRITE_BASE);
						break;
					case PE_SHP_H13B_VFC_PC:
						PE_SET_REG_TABLE(SHP_HW_H13,vflt_c_l_pc_default_h13b0,				PE_REG_H13_WRITE_BASE);
						break;
					case PE_SHP_H13B_VFC_SD_AV:
						PE_SET_REG_TABLE(SHP_HW_H13,vflt_c_l_sd_av_default_h13b0,			PE_REG_H13_WRITE_BASE);
						break;
					case PE_SHP_H13B_VFC_SD_DTV:
						PE_SET_REG_TABLE(SHP_HW_H13,vflt_c_l_sd_dtv_default_h13b0,			PE_REG_H13_WRITE_BASE);
						break;
					case PE_SHP_H13B_VFC_SD_AVRF_SECAM:
						PE_SET_REG_TABLE(SHP_HW_H13,vflt_c_l_sd_avrf_secam_default_h13b0,	PE_REG_H13_WRITE_BASE);
						break;
					case PE_SHP_H13B_VFC_SD_HDMI:
						PE_SET_REG_TABLE(SHP_HW_H13,vflt_c_l_sd_hdmi_default_h13b0,			PE_REG_H13_WRITE_BASE);
						break;
					case PE_SHP_H13B_VFC_SD_RF_NT:
						PE_SET_REG_TABLE(SHP_HW_H13,vflt_c_l_sd_rf_nt_default_h13b0,		PE_REG_H13_WRITE_BASE);
						break;
					case PE_SHP_H13B_VFC_SD_RF_PAL:
						PE_SET_REG_TABLE(SHP_HW_H13,vflt_c_l_sd_rf_pal_default_h13b0,		PE_REG_H13_WRITE_BASE);
						break;
					default:
						break;
				}
				pre0_vfc_fmt=cur0_vfc_fmt;
			}
		}
		/* R : vertical filter chroma */
		if(PE_CHECK_WIN1(pstParams->win_id))
		{
			cur1_vfc_fmt = PE_SHP_HW_H13B_ConvDispInfoToVfcFmt(cur1_inf);
			PE_SHP_HW_H13_DBG_PRINT_FMT_STATE(pre1_vfc_fmt,cur1_vfc_fmt);
			if(pre1_vfc_fmt!=cur1_vfc_fmt)
			{
				switch(cur1_vfc_fmt)
				{
					case PE_SHP_H13B_VFC_HD:
						PE_SET_REG_TABLE(SHP_HW_H13,vflt_c_l_hd_default_h13b0,				PE_MSC_REG_H13_LRDIFF);
						break;
					case PE_SHP_H13B_VFC_PC:
						PE_SET_REG_TABLE(SHP_HW_H13,vflt_c_l_pc_default_h13b0,				PE_MSC_REG_H13_LRDIFF);
						break;
					case PE_SHP_H13B_VFC_SD_AV:
						PE_SET_REG_TABLE(SHP_HW_H13,vflt_c_l_sd_av_default_h13b0,			PE_MSC_REG_H13_LRDIFF);
						break;
					case PE_SHP_H13B_VFC_SD_DTV:
						PE_SET_REG_TABLE(SHP_HW_H13,vflt_c_l_sd_dtv_default_h13b0,			PE_MSC_REG_H13_LRDIFF);
						break;
					case PE_SHP_H13B_VFC_SD_AVRF_SECAM:
						PE_SET_REG_TABLE(SHP_HW_H13,vflt_c_l_sd_avrf_secam_default_h13b0,	PE_MSC_REG_H13_LRDIFF);
						break;
					case PE_SHP_H13B_VFC_SD_HDMI:
						PE_SET_REG_TABLE(SHP_HW_H13,vflt_c_l_sd_hdmi_default_h13b0,			PE_MSC_REG_H13_LRDIFF);
						break;
					case PE_SHP_H13B_VFC_SD_RF_NT:
						PE_SET_REG_TABLE(SHP_HW_H13,vflt_c_l_sd_rf_nt_default_h13b0,		PE_MSC_REG_H13_LRDIFF);
						break;
					case PE_SHP_H13B_VFC_SD_RF_PAL:
						PE_SET_REG_TABLE(SHP_HW_H13,vflt_c_l_sd_rf_pal_default_h13b0,		PE_MSC_REG_H13_LRDIFF);
						break;
					default:
						break;
				}
				pre1_vfc_fmt=cur1_vfc_fmt;
			}
		}
	}while(0);
	return ret;
}
/**
 * default shp setting(for H13AX)
 *
 * @param   *pstParams [in] LX_PE_DEFAULT_SETTINGS_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_SHP_HW_H13A_SetDefault(LX_PE_DEFAULT_SETTINGS_T *pstParams)
{
	int ret = RET_OK;
	PE_INF_H13_SETTINGS_T inf_set;
	LX_PE_INF_DISPLAY_T *cur0_inf = NULL;
	LX_PE_INF_DISPLAY_T *cur1_inf = NULL;
	PE_SHP_HW_H13A_RES_FMT cur0_res_fmt,cur1_res_fmt;
	PE_SHP_HW_H13A_VFC_FMT cur0_vfc_fmt,cur1_vfc_fmt;
	static PE_SHP_HW_H13A_RES_FMT pre0_res_fmt = PE_SHP_H13A_RES_NUM;
	static PE_SHP_HW_H13A_RES_FMT pre1_res_fmt = PE_SHP_H13A_RES_NUM;
	static PE_SHP_HW_H13A_VFC_FMT pre0_vfc_fmt = PE_SHP_H13A_VFC_NUM;
	static PE_SHP_HW_H13A_VFC_FMT pre1_vfc_fmt = PE_SHP_H13A_VFC_NUM;
	PE_SHP_HW_H13_SETTINGS_T *pInfo=&_g_pe_shp_hw_h13_info;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		ret = PE_INF_H13_GetCurInfSettings(&inf_set);
		PE_SHP_HW_H13_CHECK_CODE(ret,break,"[%s,%d] PE_INF_H13_GetCurInfSettings() error.\n",__F__,__L__);
		cur0_inf=&inf_set.disp_info[LX_PE_WIN_0];
		cur1_inf=&inf_set.disp_info[LX_PE_WIN_1];
		CHECK_KNULL(cur0_inf);
		CHECK_KNULL(cur1_inf);
		/* L : resolution enhance */
		if(PE_CHECK_WIN0(pstParams->win_id))
		{
			cur0_res_fmt = PE_SHP_HW_H13A_ConvDispInfoToResFmt(cur0_inf);
			PE_SHP_HW_H13_DBG_PRINT_FMT_STATE(pre0_res_fmt,cur0_res_fmt);
			if(pre0_res_fmt!=cur0_res_fmt)
			{
				switch(cur0_res_fmt)
				{
					case PE_SHP_H13A_RES_3D:
						PE_SET_REG_TABLE(SHP_HW_H13,shp_3d_l_default_h13a0,0);
						break;
					case PE_SHP_H13A_RES_ATV_NTSC:
						PE_SET_REG_TABLE(SHP_HW_H13,shp_rf_ntsc_l_default_h13a0,0);
						break;
					case PE_SHP_H13A_RES_ATV_PAL:
						PE_SET_REG_TABLE(SHP_HW_H13,shp_rf_pal_l_default_h13a0,0);
						break;
					case PE_SHP_H13A_RES_AV_NTSC:
						PE_SET_REG_TABLE(SHP_HW_H13,shp_av_ntsc_l_default_h13a0,0);
						break;
					case PE_SHP_H13A_RES_AV_SCARTRGB:
						PE_SET_REG_TABLE(SHP_HW_H13,shp_av_scartrgb_l_default_h13a0,0);
						break;
					case PE_SHP_H13A_RES_AV_PAL:
						PE_SET_REG_TABLE(SHP_HW_H13,shp_av_pal_l_default_h13a0,0);
						break;
					case PE_SHP_H13A_RES_PC:
						PE_SET_REG_TABLE(SHP_HW_H13,shp_pc_l_default_h13a0,0);
						break;
					case PE_SHP_H13A_RES_DTV_SD:
						PE_SET_REG_TABLE(SHP_HW_H13,shp_dtv_sd_l_default_h13a0,0);
						break;
					case PE_SHP_H13A_RES_DTV_HD:
						PE_SET_REG_TABLE(SHP_HW_H13,shp_dtv_hd_l_default_h13a0,0);
						break;
					case PE_SHP_H13A_RES_COMP_SD:
						PE_SET_REG_TABLE(SHP_HW_H13,shp_comp_sd_l_default_h13a0,0);
						break;
					case PE_SHP_H13A_RES_COMP_HD:
						PE_SET_REG_TABLE(SHP_HW_H13,shp_comp_hd_l_default_h13a0,0);
						break;
					case PE_SHP_H13A_RES_HDMI_SD:
						PE_SET_REG_TABLE(SHP_HW_H13,shp_hdmi_sd_l_default_h13a0,0);
						break;
					case PE_SHP_H13A_RES_HDMI_HD:
						PE_SET_REG_TABLE(SHP_HW_H13,shp_hdmi_hd_l_default_h13a0,0);
						break;
					default:
						break;
				}
				pre0_res_fmt=cur0_res_fmt;
				/* reset the previous resolution enable to avoid overlapping. */
				pInfo->pre_res_en[LX_PE_WIN_0]=0xff;
			}
		}
		/* R : resolution enhance */
		if(PE_CHECK_WIN1(pstParams->win_id))
		{
			cur1_res_fmt = PE_SHP_HW_H13A_ConvDispInfoToResFmt(cur1_inf);
			PE_SHP_HW_H13_DBG_PRINT_FMT_STATE(pre1_res_fmt,cur1_res_fmt);
			if(pre1_res_fmt!=cur1_res_fmt)
			{
				switch(cur1_res_fmt)
				{
					case PE_SHP_H13A_RES_3D:
						PE_SET_REG_TABLE(SHP_HW_H13,shp_3d_r_default_h13a0,0);
						break;
					case PE_SHP_H13A_RES_ATV_NTSC:
						PE_SET_REG_TABLE(SHP_HW_H13,shp_rf_ntsc_r_default_h13a0,0);
						break;
					case PE_SHP_H13A_RES_ATV_PAL:
						PE_SET_REG_TABLE(SHP_HW_H13,shp_rf_pal_r_default_h13a0,0);
						break;
					case PE_SHP_H13A_RES_AV_NTSC:
						PE_SET_REG_TABLE(SHP_HW_H13,shp_av_ntsc_r_default_h13a0,0);
						break;
					case PE_SHP_H13A_RES_AV_SCARTRGB:
						PE_SET_REG_TABLE(SHP_HW_H13,shp_av_scartrgb_r_default_h13a0,0);
						break;
					case PE_SHP_H13A_RES_AV_PAL:
						PE_SET_REG_TABLE(SHP_HW_H13,shp_av_pal_r_default_h13a0,0);
						break;
					case PE_SHP_H13A_RES_PC:
						PE_SET_REG_TABLE(SHP_HW_H13,shp_pc_r_default_h13a0,0);
						break;
					case PE_SHP_H13A_RES_DTV_SD:
						PE_SET_REG_TABLE(SHP_HW_H13,shp_dtv_sd_r_default_h13a0,0);
						break;
					case PE_SHP_H13A_RES_DTV_HD:
						PE_SET_REG_TABLE(SHP_HW_H13,shp_dtv_hd_r_default_h13a0,0);
						break;
					case PE_SHP_H13A_RES_COMP_SD:
						PE_SET_REG_TABLE(SHP_HW_H13,shp_comp_sd_r_default_h13a0,0);
						break;
					case PE_SHP_H13A_RES_COMP_HD:
						PE_SET_REG_TABLE(SHP_HW_H13,shp_comp_hd_r_default_h13a0,0);
						break;
					case PE_SHP_H13A_RES_HDMI_SD:
						PE_SET_REG_TABLE(SHP_HW_H13,shp_hdmi_sd_r_default_h13a0,0);
						break;
					case PE_SHP_H13A_RES_HDMI_HD:
						PE_SET_REG_TABLE(SHP_HW_H13,shp_hdmi_hd_r_default_h13a0,0);
						break;
					default:
						break;
				}
				pre1_res_fmt=cur1_res_fmt;
			}
		}
		/* L : vertical filter chroma */
		if(PE_CHECK_WIN0(pstParams->win_id))
		{
			cur0_vfc_fmt = PE_SHP_HW_H13A_ConvDispInfoToVfcFmt(cur0_inf);
			PE_SHP_HW_H13_DBG_PRINT_FMT_STATE(pre0_vfc_fmt,cur0_vfc_fmt);
			if(pre0_vfc_fmt!=cur0_vfc_fmt)
			{
				switch(cur0_vfc_fmt)
				{
					case PE_SHP_H13A_VFC_RF_SD:
						PE_SET_REG_TABLE(SHP_HW_H13,vflt_c_rf_sd_l_default_h13a0,0);
						break;
					case PE_SHP_H13A_VFC_AV_SECAM:
						PE_SET_REG_TABLE(SHP_HW_H13,vflt_c_secam_l_default_h13a0,0);
						break;
					case PE_SHP_H13A_VFC_AV_SD:
						PE_SET_REG_TABLE(SHP_HW_H13,vflt_c_av_l_default_h13a0,0);
						break;
					case PE_SHP_H13A_VFC_PC:
						PE_SET_REG_TABLE(SHP_HW_H13,vflt_c_pc_l_default_h13a0,0);
						break;
					case PE_SHP_H13A_VFC_HDMI_SD:
						PE_SET_REG_TABLE(SHP_HW_H13,vflt_c_hdmi_sd_l_default_h13a0,0);
						break;
					case PE_SHP_H13A_VFC_HD:
						PE_SET_REG_TABLE(SHP_HW_H13,vflt_c_hd_l_default_h13a0,0);
						break;
					default:
						break;
				}
				pre0_vfc_fmt=cur0_vfc_fmt;
			}
		}
		/* R : vertical filter chroma */
		if(PE_CHECK_WIN1(pstParams->win_id))
		{
			cur1_vfc_fmt = PE_SHP_HW_H13A_ConvDispInfoToVfcFmt(cur1_inf);
			PE_SHP_HW_H13_DBG_PRINT_FMT_STATE(pre1_vfc_fmt,cur1_vfc_fmt);
			if(pre1_vfc_fmt!=cur1_vfc_fmt)
			{
				switch(cur1_vfc_fmt)
				{
					case PE_SHP_H13A_VFC_RF_SD:
						PE_SET_REG_TABLE(SHP_HW_H13,vflt_c_rf_sd_r_default_h13a0,0);
						break;
					case PE_SHP_H13A_VFC_AV_SECAM:
						PE_SET_REG_TABLE(SHP_HW_H13,vflt_c_secam_r_default_h13a0,0);
						break;
					case PE_SHP_H13A_VFC_AV_SD:
						PE_SET_REG_TABLE(SHP_HW_H13,vflt_c_av_r_default_h13a0,0);
						break;
					case PE_SHP_H13A_VFC_PC:
						PE_SET_REG_TABLE(SHP_HW_H13,vflt_c_pc_r_default_h13a0,0);
						break;
					case PE_SHP_H13A_VFC_HDMI_SD:
						PE_SET_REG_TABLE(SHP_HW_H13,vflt_c_hdmi_sd_r_default_h13a0,0);
						break;
					case PE_SHP_H13A_VFC_HD:
						PE_SET_REG_TABLE(SHP_HW_H13,vflt_c_hd_r_default_h13a0,0);
						break;
					default:
						break;
				}
				pre1_vfc_fmt=cur1_vfc_fmt;
			}
		}
	}while(0);
	return ret;
}
/**
 * get resolution fmt using display info(for H13BX)
 *
 * @param   *disp_inf [in] LX_PE_INF_DISPLAY_T
 * @return  PE_SHP_HW_H13B_RES_FMT.
 * @see
 * @author
 */
static PE_SHP_HW_H13B_RES_FMT PE_SHP_HW_H13B_ConvDispInfoToResFmt(LX_PE_INF_DISPLAY_T *disp_inf)
{
	PE_SHP_HW_H13B_RES_FMT res_fmt = PE_SHP_H13B_RES_NUM;
	if(disp_inf->out_type == LX_PE_OUT_TB || disp_inf->out_type == LX_PE_OUT_SS)
	{
		res_fmt = PE_SHP_H13B_RES_3D_TB_SS;
	}
	else
	{
		switch(disp_inf->src_type)
		{
			case LX_PE_SRC_ATV:
				if(disp_inf->cstd_type==LX_PE_CSTD_NTSC)
				{
					res_fmt = PE_SHP_H13B_RES_RF_NTSC;
				}
				else
				{
					res_fmt = PE_SHP_H13B_RES_RF_PAL;
				}
				break;
			case LX_PE_SRC_CVBS:
			case LX_PE_SRC_SCART:
				if(disp_inf->cstd_type==LX_PE_CSTD_NTSC)
				{
					res_fmt = PE_SHP_H13B_RES_AV_NTSC;
				}
				else
				{
					res_fmt = PE_SHP_H13B_RES_AV_PAL;
				}
				break;
			case LX_PE_SRC_RGBPC:
				{
					res_fmt = PE_SHP_H13B_RES_PC;
				}
				break;
			case LX_PE_SRC_DTV:
				if(disp_inf->dtv_type == LX_PE_DTV_PHOTOPLAY)
				{
					res_fmt = PE_SHP_H13B_RES_DTV_PHOTO;
				}
				else
				{
					if(disp_inf->fmt_type==LX_PE_FMT_SD)
					{
						res_fmt = PE_SHP_H13B_RES_DTV_SD;
					}
					else
					{
						res_fmt = PE_SHP_H13B_RES_DTV_HD;
					}
				}				
				break;
			case LX_PE_SRC_COMP:
				if(disp_inf->fmt_type==LX_PE_FMT_SD)
				{
					if(disp_inf->scan_type==LX_PE_SCAN_INTERLACE)
					{
						res_fmt = PE_SHP_H13B_RES_COMP480I;
					}
					else
					{
						res_fmt = PE_SHP_H13B_RES_COMP480P;
					}
				}
				else
				{
					res_fmt = PE_SHP_H13B_RES_HDMI_HD;
				}
				break;
			case LX_PE_SRC_HDMI:
			default:
				if(disp_inf->hdmi_type==LX_PE_HDMI_PC)
				{
					res_fmt = PE_SHP_H13B_RES_PC;
				}
				else if(disp_inf->fmt_type==LX_PE_FMT_SD)
				{
					if(disp_inf->scan_type==LX_PE_SCAN_INTERLACE)
					{
						res_fmt = PE_SHP_H13B_RES_HDMI480I;
					}
					else
					{
						res_fmt = PE_SHP_H13B_RES_HDMI480P;
					}
				}
				else
				{
					res_fmt = PE_SHP_H13B_RES_HDMI_HD;
				}
				break;
		}
	}
	return res_fmt;
}
/**
 * get resolution fmt using display info(for H13AX)
 *
 * @param   *disp_inf [in] LX_PE_INF_DISPLAY_T
 * @return  PE_SHP_HW_H13A_RES_FMT.
 * @see
 * @author
 */
static PE_SHP_HW_H13A_RES_FMT PE_SHP_HW_H13A_ConvDispInfoToResFmt(LX_PE_INF_DISPLAY_T *disp_inf)
{
	PE_SHP_HW_H13A_RES_FMT res_fmt = PE_SHP_H13A_RES_NUM;
	if((disp_inf->out_type>=LX_PE_OUT_TB) && (disp_inf->out_type<=LX_PE_OUT_DUAL_FULL))
	{
		res_fmt = PE_SHP_H13A_RES_3D;
	}
	else	//2D
	{
		switch(disp_inf->src_type)
		{
			case LX_PE_SRC_ATV:
				if(disp_inf->cstd_type==LX_PE_CSTD_NTSC)
				{
					res_fmt = PE_SHP_H13A_RES_ATV_NTSC;
				}
				else	//pal, secam
				{
					res_fmt = PE_SHP_H13A_RES_ATV_PAL;
				}
				break;
			case LX_PE_SRC_CVBS:
			case LX_PE_SRC_SCART:
				if(disp_inf->src_type==LX_PE_SRC_SCART && \
					disp_inf->scart_type==LX_PE_SCART_RGB)
				{
					res_fmt = PE_SHP_H13A_RES_AV_SCARTRGB;
				}
				else if(disp_inf->cstd_type==LX_PE_CSTD_NTSC)
				{
					res_fmt = PE_SHP_H13A_RES_AV_NTSC;
				}
				else	//pal, secam
				{
					res_fmt = PE_SHP_H13A_RES_AV_PAL;
				}
				break;
			case LX_PE_SRC_RGBPC:
				{
					res_fmt = PE_SHP_H13A_RES_PC;
				}
				break;
			case LX_PE_SRC_DTV:
				if(disp_inf->fmt_type==LX_PE_FMT_SD)
				{
					res_fmt = PE_SHP_H13A_RES_DTV_SD;
				}
				else
				{
					res_fmt = PE_SHP_H13A_RES_DTV_HD;
				}
				break;
			case LX_PE_SRC_COMP:
				if(disp_inf->fmt_type==LX_PE_FMT_SD)
				{
					res_fmt = PE_SHP_H13A_RES_COMP_SD;
				}
				else
				{
					res_fmt = PE_SHP_H13A_RES_COMP_HD;
				}
				break;
			case LX_PE_SRC_HDMI:
			default:
				if(disp_inf->hdmi_type==LX_PE_HDMI_PC)
				{
					res_fmt = PE_SHP_H13A_RES_PC;
				}
				else if(disp_inf->fmt_type==LX_PE_FMT_SD)
				{
					res_fmt = PE_SHP_H13A_RES_HDMI_SD;
				}
				else
				{
					res_fmt = PE_SHP_H13A_RES_HDMI_HD;
				}
				break;
		}
	}
	return res_fmt;
}
/**
 * get vertical filter(chroma) fmt using display info(for H13BX)
 *
 * @param   *disp_inf [in] LX_PE_INF_DISPLAY_T
 * @return  PE_SHP_HW_H13B_VFC_FMT
 * @see
 * @author
 */
static PE_SHP_HW_H13B_VFC_FMT PE_SHP_HW_H13B_ConvDispInfoToVfcFmt(LX_PE_INF_DISPLAY_T *disp_inf)
{
	PE_SHP_HW_H13B_VFC_FMT vfc_fmt = PE_SHP_H13B_VFC_NUM;
	switch(disp_inf->src_type)
	{
		case LX_PE_SRC_ATV:
			if(disp_inf->cstd_type==LX_PE_CSTD_SECAM)
			{
				vfc_fmt = PE_SHP_H13B_VFC_SD_AVRF_SECAM;
			}
			else if(disp_inf->cstd_type==LX_PE_CSTD_PAL)
			{
				vfc_fmt = PE_SHP_H13B_VFC_SD_RF_PAL;
			}
			else
			{
				vfc_fmt = PE_SHP_H13B_VFC_SD_RF_NT;
			}
			break;
		case LX_PE_SRC_CVBS:
		case LX_PE_SRC_SCART:
			if(disp_inf->cstd_type==LX_PE_CSTD_SECAM)
			{
				vfc_fmt = PE_SHP_H13B_VFC_SD_AVRF_SECAM;
			}
			else
			{
				vfc_fmt = PE_SHP_H13B_VFC_SD_AV;
			}
			break;
		case LX_PE_SRC_RGBPC:
			{
				vfc_fmt = PE_SHP_H13B_VFC_PC;
			}
			break;
		case LX_PE_SRC_DTV:
			if(disp_inf->fmt_type==LX_PE_FMT_SD)
			{
				vfc_fmt = PE_SHP_H13B_VFC_SD_DTV;
			}
			else
			{
				vfc_fmt = PE_SHP_H13B_VFC_HD;
			}
			break;
		case LX_PE_SRC_COMP:
			if(disp_inf->fmt_type==LX_PE_FMT_SD)
			{
				vfc_fmt = PE_SHP_H13B_VFC_SD_AV;
			}
			else
			{
				vfc_fmt = PE_SHP_H13B_VFC_HD;
			}
			break;
		case LX_PE_SRC_HDMI:
		default:
			if(disp_inf->hdmi_type==LX_PE_HDMI_PC)
			{
				vfc_fmt = PE_SHP_H13B_VFC_PC;
			}
			else if(disp_inf->fmt_type==LX_PE_FMT_SD)
			{
				vfc_fmt = PE_SHP_H13B_VFC_SD_HDMI;
			}
			else
			{
				vfc_fmt = PE_SHP_H13B_VFC_HD;
			}
			break;
	}
	return vfc_fmt;
}
/**
 * get vertical filter(chroma) fmt using display info(for H13AX)
 *
 * @param   *disp_inf [in] LX_PE_INF_DISPLAY_T
 * @return  PE_SHP_HW_H13A_VFC_FMT
 * @see
 * @author
 */
static PE_SHP_HW_H13A_VFC_FMT PE_SHP_HW_H13A_ConvDispInfoToVfcFmt(LX_PE_INF_DISPLAY_T *disp_inf)
{
	PE_SHP_HW_H13A_VFC_FMT vfc_fmt = PE_SHP_H13A_VFC_NUM;
	switch(disp_inf->src_type)
	{
		case LX_PE_SRC_ATV:
			{
				vfc_fmt = PE_SHP_H13A_VFC_RF_SD;
			}
			break;
		case LX_PE_SRC_CVBS:
		case LX_PE_SRC_SCART:
			if(disp_inf->cstd_type==LX_PE_CSTD_SECAM)
			{
				vfc_fmt = PE_SHP_H13A_VFC_AV_SECAM;
			}
			else
			{
				vfc_fmt = PE_SHP_H13A_VFC_AV_SD;
			}
			break;
		case LX_PE_SRC_RGBPC:
			{
				vfc_fmt = PE_SHP_H13A_VFC_PC;
			}
			break;
		case LX_PE_SRC_DTV:
			if(disp_inf->fmt_type==LX_PE_FMT_SD)
			{
				vfc_fmt = PE_SHP_H13A_VFC_RF_SD;
			}
			else
			{
				vfc_fmt = PE_SHP_H13A_VFC_HD;
			}
			break;
		case LX_PE_SRC_COMP:
			if(disp_inf->fmt_type==LX_PE_FMT_SD)
			{
				vfc_fmt = PE_SHP_H13A_VFC_AV_SD;
			}
			else
			{
				vfc_fmt = PE_SHP_H13A_VFC_HD;
			}
			break;
		case LX_PE_SRC_HDMI:
		default:
			if(disp_inf->hdmi_type==LX_PE_HDMI_PC)
			{
				vfc_fmt = PE_SHP_H13A_VFC_PC;
			}
			else if(disp_inf->fmt_type==LX_PE_FMT_SD)
			{
				vfc_fmt = PE_SHP_H13A_VFC_HDMI_SD;
			}
			else
			{
				vfc_fmt = PE_SHP_H13A_VFC_HD;
			}
			break;
	}
	return vfc_fmt;
}
/**
 * set peaking enable by resize info
 *	- resize case : turn off peaking
 *
 * @param   *pCfg [in] PE_TSK_H13_CFG_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_SHP_HW_H13_SetReEnableCtrl(PE_TSK_H13_CFG_T *pCfg)
{
	int ret = RET_OK;
	static UINT32 count = 0;
	UINT32 cur_en[LX_PE_WIN_NUM];
	PE_INF_H13_CTRL_T *ctrl0_info = &pCfg->inf.ctrl_info[LX_PE_WIN_0];
	PE_SHP_HW_H13_SETTINGS_T *pInfo=&_g_pe_shp_hw_h13_info;
	do{
		CHECK_KNULL(pCfg);
		/* if it's a resize, turn off resolution enable. */
		cur_en[LX_PE_WIN_0] = (ctrl0_info->scl_resize)? 0:1;
		if(pInfo->pre_res_en[LX_PE_WIN_0]!=cur_en[LX_PE_WIN_0])
		{
			PE_SHP_HW_H13_DBG_PRINT("set[0] : o_h,v:%d,%d, resize:%d, en:%d -> %d\n",\
				ctrl0_info->o_win_h_size,ctrl0_info->o_win_v_size,ctrl0_info->scl_resize,\
				pInfo->pre_res_en[LX_PE_WIN_0],cur_en[LX_PE_WIN_0]);
			if(PE_KDRV_VER_H13BX)
			{
				PE_P1L_H13B0_RdFL(pe1_sp_ctrl_00);
				PE_P1L_H13B0_RdFL(pe1_mp_ctrl_00);
				PE_P1L_H13B0_Wr01(pe1_sp_ctrl_00, edge_enhance_enable,	cur_en[LX_PE_WIN_0]);
				PE_P1L_H13B0_Wr01(pe1_mp_ctrl_00, edge_enhance_enable,	cur_en[LX_PE_WIN_0]);
				PE_P1L_H13B0_WrFL(pe1_sp_ctrl_00);
				PE_P1L_H13B0_WrFL(pe1_mp_ctrl_00);
			}
			else if(PE_KDRV_VER_H13AX)
			{
				PE_P1L_H13_RdFL(pe1_sp_ctrl_00);
				PE_P1L_H13_RdFL(pe1_mp_ctrl_00);
				PE_P1L_H13_Wr01(pe1_sp_ctrl_00, edge_enhance_enable,	cur_en[LX_PE_WIN_0]);
				PE_P1L_H13_Wr01(pe1_mp_ctrl_00, edge_enhance_enable,	cur_en[LX_PE_WIN_0]);
				PE_P1L_H13_WrFL(pe1_sp_ctrl_00);
				PE_P1L_H13_WrFL(pe1_mp_ctrl_00);
			}
			else
			{
				PE_SHP_HW_H13_DBG_PRINT("nothing to do.\n");
			}
			pInfo->pre_res_en[LX_PE_WIN_0]=cur_en[LX_PE_WIN_0];
		}
		if(_g_shp_hw_h13_trace)
		{
			if(count>PE_PRINT_COUNT_NUM)
			{
				PE_SHP_HW_H13_DBG_PRINT("stat[0] : o_h,v:%d,%d, resize:%d, en:%d -> %d\n",\
					ctrl0_info->o_win_h_size,ctrl0_info->o_win_v_size,ctrl0_info->scl_resize,\
					pInfo->pre_res_en[LX_PE_WIN_0],cur_en[LX_PE_WIN_0]);
				count=0;
			}
			count++;
		}
	}while(0);
	return ret;
}
/**
 * set(store) scaler filter coeff info
 *
 * @param   *pstParams [in] LX_PE_SHP_SCLFILTER_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_SHP_HW_H13_SetScalerFilterCoeff(LX_PE_SHP_SCLFILTER_T *pstParams)
{
	int ret = RET_OK;
	PE_SHP_HW_H13_SETTINGS_T *pInfo=&_g_pe_shp_hw_h13_info;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		PE_SHP_HW_H13_DBG_PRINT("set[%d]:cur idx_y:%d,%d,idx_c:%d,%d\n",pstParams->win_id,\
			pstParams->h_luma_id,pstParams->v_luma_id,pstParams->h_chrm_id,pstParams->v_chrm_id);
		if(PE_KDRV_VER_H13)
		{
			if(PE_CHECK_WIN0(pstParams->win_id))
			{
				memcpy(&pInfo->cur_scl_idx[LX_PE_WIN_0],pstParams,sizeof(LX_PE_SHP_SCLFILTER_T));
			}
			if(PE_CHECK_WIN1(pstParams->win_id))
			{
				memcpy(&pInfo->cur_scl_idx[LX_PE_WIN_1],pstParams,sizeof(LX_PE_SHP_SCLFILTER_T));
			}
		}
		else
		{
			PE_SHP_HW_H13_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * get(store) scaler filter coeff info
 *
 * @param   *pstParams [in] LX_PE_SHP_SCLFILTER_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_SHP_HW_H13_GetScalerFilterCoeff(LX_PE_SHP_SCLFILTER_T *pstParams)
{
	int ret = RET_OK;
	LX_PE_WIN_ID win_id,temp;
	PE_SHP_HW_H13_SETTINGS_T *pInfo=&_g_pe_shp_hw_h13_info;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		win_id = PE_GET_CHECKED_WINID(pstParams->win_id);
		PE_SHP_HW_H13_DBG_PRINT("set[%d]:cur idx_y:%d,%d,idx_c:%d,%d\n",pstParams->win_id,\
			pstParams->h_luma_id,pstParams->v_luma_id,pstParams->h_chrm_id,pstParams->v_chrm_id);
		if(PE_KDRV_VER_H13)
		{
			if(PE_CHECK_WIN0(win_id))
			{
				temp = pstParams->win_id;
				memcpy(pstParams,&pInfo->cur_scl_idx[LX_PE_WIN_0],sizeof(LX_PE_SHP_SCLFILTER_T));
				pstParams->win_id = temp;
			}
			if(PE_CHECK_WIN1(win_id))
			{
				temp = pstParams->win_id;
				memcpy(pstParams,&pInfo->cur_scl_idx[LX_PE_WIN_1],sizeof(LX_PE_SHP_SCLFILTER_T));
				pstParams->win_id = temp;
			}
		}
		else
		{
			PE_SHP_HW_H13_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * set scaler filter control periodically
 *
 * @param   *pstParams [in] PE_SHP_HW_H13_SETTINGS_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_SHP_HW_H13_SetScalerFilterCtrl(PE_TSK_H13_CFG_T *pCfg)
{
	int ret = RET_OK;
	PE_SHP_HW_H13_SETTINGS_T cur_param;
	PE_SHP_HW_H13_SETTINGS_T *pInfo=&_g_pe_shp_hw_h13_info;
	PE_SHP_HW_H13_FLAG_T set_flag;
	UINT32 type_sel_luma,type_sel_chroma,scaler_config,adap_filter_en;
	do{
		CHECK_KNULL(pCfg);
		if(PE_KDRV_VER_H13)
		{
			#ifdef PE_SHP_HW_H13_USE_SHP_MODE
			PE_SHP_HW_H13_SetShpMode(pCfg);
			#endif
			memset0(cur_param);
			/* get fir,tap,index */
			ret = PE_SHP_HW_H13_GetScalerFilterValue(pCfg,&cur_param);
			PE_SHP_HW_H13_CHECK_CODE(ret,break,"[%s,%d] PE_SHP_HW_H13_GetScalerFilterValue() error.\n",__F__,__L__);
			/* check index is valid & re-setting index */
			PE_SHP_HW_H13_CheckValidScalerFilterValue(&cur_param);
			/* L : compare & filtering */
			if(pInfo->pre_scl_idx[LX_PE_WIN_0].h_luma_id!=cur_param.cur_scl_idx[LX_PE_WIN_0].h_luma_id || \
				pInfo->pre_scl_idx[LX_PE_WIN_0].v_luma_id!=cur_param.cur_scl_idx[LX_PE_WIN_0].v_luma_id || \
				pInfo->pre_scl_idx[LX_PE_WIN_0].h_chrm_id!=cur_param.cur_scl_idx[LX_PE_WIN_0].h_chrm_id || \
				pInfo->pre_scl_idx[LX_PE_WIN_0].v_chrm_id!=cur_param.cur_scl_idx[LX_PE_WIN_0].v_chrm_id || \
				pInfo->h_y_tap[LX_PE_WIN_0]!=cur_param.h_y_tap[LX_PE_WIN_0] || \
				pInfo->v_y_tap[LX_PE_WIN_0]!=cur_param.v_y_tap[LX_PE_WIN_0] || \
				pInfo->h_c_tap[LX_PE_WIN_0]!=cur_param.h_c_tap[LX_PE_WIN_0] || \
				pInfo->v_c_tap[LX_PE_WIN_0]!=cur_param.v_c_tap[LX_PE_WIN_0] || \
				pInfo->y_fir[LX_PE_WIN_0]!=cur_param.y_fir[LX_PE_WIN_0] || \
				pInfo->c_fir[LX_PE_WIN_0]!=cur_param.c_fir[LX_PE_WIN_0])
			{
				PE_SHP_HW_H13_DBG_PRINT("set[0]:[idx_hy,vy,hc,vc][pre:%d,%d,%d,%d][cur:%d,%d,%d,%d]\n",\
					pInfo->pre_scl_idx[LX_PE_WIN_0].h_luma_id,pInfo->pre_scl_idx[LX_PE_WIN_0].v_luma_id,\
					pInfo->pre_scl_idx[LX_PE_WIN_0].h_chrm_id,pInfo->pre_scl_idx[LX_PE_WIN_0].v_chrm_id,\
					cur_param.cur_scl_idx[LX_PE_WIN_0].h_luma_id,cur_param.cur_scl_idx[LX_PE_WIN_0].v_luma_id,\
					cur_param.cur_scl_idx[LX_PE_WIN_0].h_chrm_id,cur_param.cur_scl_idx[LX_PE_WIN_0].v_chrm_id);
				PE_SHP_HW_H13_DBG_PRINT("set[0]:[tap_hy,vy,hc,vc][pre:%d,%d,%d,%d][cur:%d,%d,%d,%d]\n",\
					pInfo->h_y_tap[LX_PE_WIN_0],pInfo->v_y_tap[LX_PE_WIN_0],\
					pInfo->h_c_tap[LX_PE_WIN_0],pInfo->v_c_tap[LX_PE_WIN_0],\
					cur_param.h_y_tap[LX_PE_WIN_0],cur_param.v_y_tap[LX_PE_WIN_0],\
					cur_param.h_c_tap[LX_PE_WIN_0],cur_param.v_c_tap[LX_PE_WIN_0]);
				PE_SHP_HW_H13_DBG_PRINT("set[0]:[fir_y,c][pre:%d,%d][cur:%d,%d]\n",\
					pInfo->y_fir[LX_PE_WIN_0],pInfo->c_fir[LX_PE_WIN_0],\
					cur_param.y_fir[LX_PE_WIN_0],cur_param.c_fir[LX_PE_WIN_0]);
				/* set config */
				if(pInfo->h_y_tap[LX_PE_WIN_0]!=cur_param.h_y_tap[LX_PE_WIN_0] || \
					pInfo->y_fir[LX_PE_WIN_0]!=cur_param.y_fir[LX_PE_WIN_0] || \
					pInfo->c_fir[LX_PE_WIN_0]!=cur_param.c_fir[LX_PE_WIN_0])
				{
					scaler_config	= (cur_param.h_y_tap[LX_PE_WIN_0]==PE_SHP_HW_H13_SCL_V12T_NORM || \
										cur_param.h_y_tap[LX_PE_WIN_0]==PE_SHP_HW_H13_SCL_V12T_DN14)? 0x0:0x1;
					adap_filter_en	= (cur_param.h_y_tap[LX_PE_WIN_0]==PE_SHP_HW_H13_SCL_V12T_NORM)? 0x1:0x0;
					type_sel_luma	= (cur_param.y_fir[LX_PE_WIN_0]==PE_SHP_HW_H13_FIR_BILINEAR)? 0x1:0x0;
					type_sel_chroma	= (cur_param.c_fir[LX_PE_WIN_0]==PE_SHP_HW_H13_FIR_BILINEAR)? 0x1:0x0;
					if(PE_KDRV_VER_H13BX)
					{
				        PE_MSL_H13B0_RdFL(msc_ctrl_l);
				        PE_MSL_H13B0_Wr01(msc_ctrl_l, filter_type_sel_luma       ,type_sel_luma);
				        PE_MSL_H13B0_Wr01(msc_ctrl_l, filter_type_sel_chroma     ,type_sel_chroma);
				        PE_MSL_H13B0_Wr01(msc_ctrl_l, v_scaler_config            ,scaler_config);
				        PE_MSL_H13B0_Wr01(msc_ctrl_l, adaptive_fir_filter_en     ,adap_filter_en);
				        PE_MSL_H13B0_WrFL(msc_ctrl_l);
					}
					else if(PE_KDRV_VER_H13AX)
					{
				        PE_MSL_H13_RdFL(msc_ctrl_l);
				        PE_MSL_H13_Wr01(msc_ctrl_l, filter_type_sel_luma       ,type_sel_luma);
				        PE_MSL_H13_Wr01(msc_ctrl_l, filter_type_sel_chroma     ,type_sel_chroma);
				        PE_MSL_H13_Wr01(msc_ctrl_l, v_scaler_config            ,scaler_config);
				        PE_MSL_H13_Wr01(msc_ctrl_l, adaptive_fir_filter_en     ,adap_filter_en);
				        PE_MSL_H13_WrFL(msc_ctrl_l);
					}
				}
				/* check double setting flag*/
				set_flag = PE_SHP_HW_H13_GetDoubleSetFlag(LX_PE_WIN_0,pInfo->pre_scl_idx[LX_PE_WIN_0],&cur_param);
				/* set scl coeff */
				ret = PE_SHP_HW_H13_SetSclCoeff(LX_PE_WIN_0,set_flag,&cur_param);
				PE_SHP_HW_H13_CHECK_CODE(ret,break,"[%s,%d] PE_SHP_HW_H13_SetSclCoeff() error.\n",__F__,__L__);
				pInfo->pre_scl_idx[LX_PE_WIN_0].h_luma_id=cur_param.cur_scl_idx[LX_PE_WIN_0].h_luma_id;
				pInfo->pre_scl_idx[LX_PE_WIN_0].v_luma_id=cur_param.cur_scl_idx[LX_PE_WIN_0].v_luma_id;
				pInfo->pre_scl_idx[LX_PE_WIN_0].h_chrm_id=cur_param.cur_scl_idx[LX_PE_WIN_0].h_chrm_id;
				pInfo->pre_scl_idx[LX_PE_WIN_0].v_chrm_id=cur_param.cur_scl_idx[LX_PE_WIN_0].v_chrm_id;
				pInfo->h_y_tap[LX_PE_WIN_0]=cur_param.h_y_tap[LX_PE_WIN_0];
				pInfo->v_y_tap[LX_PE_WIN_0]=cur_param.v_y_tap[LX_PE_WIN_0];
				pInfo->h_c_tap[LX_PE_WIN_0]=cur_param.h_c_tap[LX_PE_WIN_0];
				pInfo->v_c_tap[LX_PE_WIN_0]=cur_param.v_c_tap[LX_PE_WIN_0];
				pInfo->y_fir[LX_PE_WIN_0]=cur_param.y_fir[LX_PE_WIN_0];
				pInfo->c_fir[LX_PE_WIN_0]=cur_param.c_fir[LX_PE_WIN_0];
			}
			/* R : compare & filtering */
			if(pInfo->pre_scl_idx[LX_PE_WIN_1].h_luma_id!=cur_param.cur_scl_idx[LX_PE_WIN_1].h_luma_id || \
				pInfo->pre_scl_idx[LX_PE_WIN_1].v_luma_id!=cur_param.cur_scl_idx[LX_PE_WIN_1].v_luma_id || \
				pInfo->pre_scl_idx[LX_PE_WIN_1].h_chrm_id!=cur_param.cur_scl_idx[LX_PE_WIN_1].h_chrm_id || \
				pInfo->pre_scl_idx[LX_PE_WIN_1].v_chrm_id!=cur_param.cur_scl_idx[LX_PE_WIN_1].v_chrm_id || \
				pInfo->h_y_tap[LX_PE_WIN_1]!=cur_param.h_y_tap[LX_PE_WIN_1] || \
				pInfo->v_y_tap[LX_PE_WIN_1]!=cur_param.v_y_tap[LX_PE_WIN_1] || \
				pInfo->h_c_tap[LX_PE_WIN_1]!=cur_param.h_c_tap[LX_PE_WIN_1] || \
				pInfo->v_c_tap[LX_PE_WIN_1]!=cur_param.v_c_tap[LX_PE_WIN_1] || \
				pInfo->y_fir[LX_PE_WIN_1]!=cur_param.y_fir[LX_PE_WIN_1] || \
				pInfo->c_fir[LX_PE_WIN_1]!=cur_param.c_fir[LX_PE_WIN_1])
			{
				PE_SHP_HW_H13_DBG_PRINT("set[1]:[idx_hy,vy,hc,vc][pre:%d,%d,%d,%d][cur:%d,%d,%d,%d]\n",\
					pInfo->pre_scl_idx[LX_PE_WIN_1].h_luma_id,pInfo->pre_scl_idx[LX_PE_WIN_1].v_luma_id,\
					pInfo->pre_scl_idx[LX_PE_WIN_1].h_chrm_id,pInfo->pre_scl_idx[LX_PE_WIN_1].v_chrm_id,\
					cur_param.cur_scl_idx[LX_PE_WIN_1].h_luma_id,cur_param.cur_scl_idx[LX_PE_WIN_1].v_luma_id,\
					cur_param.cur_scl_idx[LX_PE_WIN_1].h_chrm_id,cur_param.cur_scl_idx[LX_PE_WIN_1].v_chrm_id);
				PE_SHP_HW_H13_DBG_PRINT("set[1]:[tap_hy,vy,hc,vc][pre:%d,%d,%d,%d][cur:%d,%d,%d,%d]\n",\
					pInfo->h_y_tap[LX_PE_WIN_1],pInfo->v_y_tap[LX_PE_WIN_1],\
					pInfo->h_c_tap[LX_PE_WIN_1],pInfo->v_c_tap[LX_PE_WIN_1],\
					cur_param.h_y_tap[LX_PE_WIN_1],cur_param.v_y_tap[LX_PE_WIN_1],\
					cur_param.h_c_tap[LX_PE_WIN_1],cur_param.v_c_tap[LX_PE_WIN_1]);
				PE_SHP_HW_H13_DBG_PRINT("set[1]:[fir_y,c][pre:%d,%d][cur:%d,%d]\n",\
					pInfo->y_fir[LX_PE_WIN_1],pInfo->c_fir[LX_PE_WIN_1],\
					cur_param.y_fir[LX_PE_WIN_1],cur_param.c_fir[LX_PE_WIN_1]);
				/* set config */
				#ifndef PE_SHP_HW_H13_SKIP_MSR_FLT_TYPE_COMPARISON
				if(pInfo->h_y_tap[LX_PE_WIN_1]!=cur_param.h_y_tap[LX_PE_WIN_1] || \
					pInfo->y_fir[LX_PE_WIN_1]!=cur_param.y_fir[LX_PE_WIN_1] || \
					pInfo->c_fir[LX_PE_WIN_1]!=cur_param.c_fir[LX_PE_WIN_1])
				#endif
				{
					adap_filter_en	= (cur_param.h_y_tap[LX_PE_WIN_1]==PE_SHP_HW_H13_SCL_V12T_NORM)? 0x1:0x0;
					type_sel_luma	= (cur_param.y_fir[LX_PE_WIN_1]==PE_SHP_HW_H13_FIR_BILINEAR)? 0x1:0x0;
					type_sel_chroma	= (cur_param.c_fir[LX_PE_WIN_1]==PE_SHP_HW_H13_FIR_BILINEAR)? 0x1:0x0;
					if(PE_KDRV_VER_H13BX)
					{
				        PE_MSR_H13B0_RdFL(msc_ctrl_r);
				        PE_MSR_H13B0_Wr01(msc_ctrl_r, filter_type_sel_luma       ,type_sel_luma);
				        PE_MSR_H13B0_Wr01(msc_ctrl_r, filter_type_sel_chroma     ,type_sel_chroma);
				        PE_MSR_H13B0_Wr01(msc_ctrl_r, adaptive_fir_filter_en     ,adap_filter_en);
				        PE_MSR_H13B0_WrFL(msc_ctrl_r);
					}
					else if(PE_KDRV_VER_H13AX)
					{
				        PE_MSR_H13_RdFL(msc_ctrl_r);
				        PE_MSR_H13_Wr01(msc_ctrl_r, filter_type_sel_luma      ,type_sel_luma);
				        PE_MSR_H13_Wr01(msc_ctrl_r, filter_type_sel_chroma    ,type_sel_chroma);
				        PE_MSR_H13_Wr01(msc_ctrl_r, adaptive_fir_filter_en    ,adap_filter_en);
				        PE_MSR_H13_WrFL(msc_ctrl_r);
					}
				}
				/* check double setting flag*/
				set_flag = PE_SHP_HW_H13_GetDoubleSetFlag(LX_PE_WIN_1,pInfo->pre_scl_idx[LX_PE_WIN_1],&cur_param);
				/* set scl coeff */
				ret = PE_SHP_HW_H13_SetSclCoeff(LX_PE_WIN_1,set_flag,&cur_param);
				PE_SHP_HW_H13_CHECK_CODE(ret,break,"[%s,%d] PE_SHP_HW_H13_SetSclCoeff() error.\n",__F__,__L__);
				pInfo->pre_scl_idx[LX_PE_WIN_1].h_luma_id=cur_param.cur_scl_idx[LX_PE_WIN_1].h_luma_id;
				pInfo->pre_scl_idx[LX_PE_WIN_1].v_luma_id=cur_param.cur_scl_idx[LX_PE_WIN_1].v_luma_id;
				pInfo->pre_scl_idx[LX_PE_WIN_1].h_chrm_id=cur_param.cur_scl_idx[LX_PE_WIN_1].h_chrm_id;
				pInfo->pre_scl_idx[LX_PE_WIN_1].v_chrm_id=cur_param.cur_scl_idx[LX_PE_WIN_1].v_chrm_id;
				pInfo->h_y_tap[LX_PE_WIN_1]=cur_param.h_y_tap[LX_PE_WIN_1];
				pInfo->v_y_tap[LX_PE_WIN_1]=cur_param.v_y_tap[LX_PE_WIN_1];
				pInfo->h_c_tap[LX_PE_WIN_1]=cur_param.h_c_tap[LX_PE_WIN_1];
				pInfo->v_c_tap[LX_PE_WIN_1]=cur_param.v_c_tap[LX_PE_WIN_1];
				pInfo->y_fir[LX_PE_WIN_1]=cur_param.y_fir[LX_PE_WIN_1];
				pInfo->c_fir[LX_PE_WIN_1]=cur_param.c_fir[LX_PE_WIN_1];
			}
		}
		else
		{
			PE_SHP_HW_H13_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
#ifdef PE_SHP_HW_H13_USE_SHP_MODE
/**
 * set shp mode
 *
 * @param   *pCfg [in] PE_TSK_H13_CFG_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_SHP_HW_H13_SetShpMode(PE_TSK_H13_CFG_T *pCfg)
{
	int ret = RET_OK;
	static UINT32 count = 0;
	static UINT32 pre_sp_lap_v_mode = 0xff;		//pre sp_lap_v_mode
	UINT32 cur_sp_lap_v_mode = 0, is_seamless_internal = 0;
	PE_INF_H13_CTRL_T *ctrl0_info = &pCfg->inf.ctrl_info[LX_PE_WIN_0];
	LX_PE_INF_DISPLAY_T *disp0_info = &pCfg->inf.disp_info[LX_PE_WIN_0];
	PE_SHP_HW_H13_SETTINGS_T *pInfo = &_g_pe_shp_hw_h13_info;
	do{
		CHECK_KNULL(pCfg);
		if(PE_KDRV_VER_H13BX)
		{
			is_seamless_internal = \
				((ctrl0_info->m_win_v_size==(PE_SHP_HW_H13_FHD_V_MAX<<1)) && \
				(disp0_info->fmt_type==LX_PE_FMT_UHD) && \
				(disp0_info->out_type==LX_PE_OUT_2D))? 1:0;
			cur_sp_lap_v_mode = (!is_seamless_internal)? pInfo->sp_lap_v_mode:0x0;
			/* compare pre vs. cur sp_lap_v_mode */
			if (pre_sp_lap_v_mode != cur_sp_lap_v_mode)
			{
				PE_SHP_HW_H13_DBG_PRINT("[inf] m0_v:%d,fmt:%d,out:%d,smless:%d,lap_v:%d\n", \
					ctrl0_info->m_win_v_size, disp0_info->fmt_type, \
					disp0_info->out_type, is_seamless_internal, pInfo->sp_lap_v_mode);
				PE_SHP_HW_H13_DBG_PRINT("[set] sp_lap_v_mode : %d -> %d\n", \
					pre_sp_lap_v_mode, cur_sp_lap_v_mode);
				PE_P1L_H13B0_QWr01(pe1_sp_ctrl_06,lap_v_mode,GET_BITS(cur_sp_lap_v_mode,0,3));
				pre_sp_lap_v_mode = cur_sp_lap_v_mode;
			}
			if(_g_shp_hw_h13_trace)
			{
				if(count>PE_PRINT_COUNT_NUM)
				{
					PE_SHP_HW_H13_DBG_PRINT("[state] m0_v:%d,fmt:%d,out:%d,smless:%d,lap_v:%d\n", \
						ctrl0_info->m_win_v_size, disp0_info->fmt_type, \
						disp0_info->out_type, is_seamless_internal, pInfo->sp_lap_v_mode);
					PE_SHP_HW_H13_DBG_PRINT("[state] sp_lap_v_mode : %d\n", cur_sp_lap_v_mode);
					count=0;
				}
				count++;
			}
		}
		else
		{
			PE_SHP_HW_H13_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
#endif

/**
 * get scaler filter control value
 *	- tap type, fir type, indx
 *
 * @param   *pCfg [in] PE_TSK_H13_CFG_T
 * @param   *pstParams [in] PE_SHP_HW_H13_SETTINGS_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_SHP_HW_H13_GetScalerFilterValue(PE_TSK_H13_CFG_T *pCfg,PE_SHP_HW_H13_SETTINGS_T *pstParams)
{
	int ret = RET_OK;
	UINT32 type6Tap[LX_PE_WIN_NUM],h_ratio[LX_PE_WIN_NUM],v_ratio[LX_PE_WIN_NUM],down12tMode;
	LX_PE_INF_DISPLAY_T *disp0_info = &pCfg->inf.disp_info[LX_PE_WIN_0];
	PE_INF_H13_CTRL_T *ctrl0_info = &pCfg->inf.ctrl_info[LX_PE_WIN_0];
	PE_INF_H13_CTRL_T *ctrl1_info = &pCfg->inf.ctrl_info[LX_PE_WIN_1];
	PE_SHP_HW_H13_SETTINGS_T *pIdx_Info=&_g_pe_shp_hw_h13_info;
	do{
		CHECK_KNULL(pCfg);
		#ifdef PE_SHP_HW_H13_USE_V12T_DN_SC_FLT
		/* v12tap down scl case:hdmi(pc) & 2d & not_444 & not_scl_hd & resize & in_win_2160 */
		down12tMode = (disp0_info->src_type==LX_PE_SRC_HDMI && \
					disp0_info->out_type==LX_PE_OUT_2D && disp0_info->user_o_type==LX_PE_OUT_2D && \
					!ctrl0_info->is_yuv444 && !ctrl0_info->scl_hd && ctrl0_info->scl_resize && \
					ctrl0_info->i_win_v_size==(PE_SHP_HW_H13_FHD_V_MAX<<1))? 1:0;
		#else
		down12tMode = 0;
		#endif
		/* L,R : check fir type(if pixel by pixel case : bilinear) */
		if((ctrl0_info->i_win_h_size==ctrl0_info->o_win_h_size) && \
			(ctrl0_info->i_win_v_size==ctrl0_info->o_win_v_size))
		{
			pstParams->y_fir[LX_PE_WIN_0] = PE_SHP_HW_H13_FIR_BILINEAR;
			pstParams->c_fir[LX_PE_WIN_0] = PE_SHP_HW_H13_FIR_BILINEAR;
		}
		else
		{
			/* 20130131, sj.youm, set y_fir bilinear because of color filter issue */
			pstParams->y_fir[LX_PE_WIN_0] = (ctrl0_info->is_yuv444 && ctrl0_info->pxl_rp_st)? \
				PE_SHP_HW_H13_FIR_BILINEAR:PE_SHP_HW_H13_FIR_POLYPHASE;
			pstParams->c_fir[LX_PE_WIN_0] = (ctrl0_info->is_yuv444)? \
				PE_SHP_HW_H13_FIR_BILINEAR:PE_SHP_HW_H13_FIR_POLYPHASE;
		}
		if((ctrl1_info->i_win_h_size==ctrl1_info->o_win_h_size) && \
			(ctrl1_info->i_win_v_size==ctrl1_info->o_win_v_size))
		{
			pstParams->y_fir[LX_PE_WIN_1] = PE_SHP_HW_H13_FIR_BILINEAR;
			pstParams->c_fir[LX_PE_WIN_1] = PE_SHP_HW_H13_FIR_BILINEAR;
		}
		else
		{
			pstParams->y_fir[LX_PE_WIN_1] = PE_SHP_HW_H13_FIR_POLYPHASE;
			pstParams->c_fir[LX_PE_WIN_1] = PE_SHP_HW_H13_FIR_POLYPHASE;	/* no 444 case */
		}
		/* 6tap case:src_hsize:0(abnormal),over1024(2_line_memory),resize(with not dn12t),yuv444,3d,adaptive_stream */
		type6Tap[LX_PE_WIN_0] = (!ctrl0_info->scl_src_h || ctrl0_info->scl_hd || ctrl0_info->scl_resize || \
								ctrl0_info->is_yuv444 || disp0_info->mode.is_adpt_st || \
								disp0_info->out_type==LX_PE_OUT_TB || disp0_info->out_type==LX_PE_OUT_SS || \
								disp0_info->out_type==LX_PE_OUT_FS || disp0_info->out_type==LX_PE_OUT_DUAL_TB || \
								disp0_info->out_type==LX_PE_OUT_DUAL_SS || disp0_info->out_type==LX_PE_OUT_DUAL_FULL)? \
								1:0;
		#ifdef PE_SHP_HW_H13_USE_V12T_DN_SC_FLT
		if(type6Tap[LX_PE_WIN_0] && down12tMode)
		{
			type6Tap[LX_PE_WIN_0]=0;
		}
		#endif
		type6Tap[LX_PE_WIN_1] = 1;
		/* L : check y tap */
		if(type6Tap[LX_PE_WIN_0])
		{
			/* check in vs. out ratio */
			if(ctrl0_info->o_win_h_size >= PE_SHP_HW_H13_SCL_IN_H_MIN && \
				ctrl0_info->o_win_v_size >= PE_SHP_HW_H13_SCL_IN_V_MIN && \
				ctrl0_info->i_win_h_size && ctrl0_info->i_win_v_size)
			{
				h_ratio[LX_PE_WIN_0] = (ctrl0_info->o_win_h_size<<10)/ctrl0_info->i_win_h_size;
				v_ratio[LX_PE_WIN_0] = (ctrl0_info->o_win_v_size<<10)/ctrl0_info->i_win_v_size;
			}
			else
			{
				h_ratio[LX_PE_WIN_0] = 1024;	//1.0
				v_ratio[LX_PE_WIN_0] = 1024;	//1.0
			}
			/* tap */
			if( h_ratio[LX_PE_WIN_0] >= 875 )		{	pstParams->h_y_tap[LX_PE_WIN_0] = PE_SHP_HW_H13_SCL_V06T_NORM;}
			else if ( h_ratio[LX_PE_WIN_0] >= 625 )	{	pstParams->h_y_tap[LX_PE_WIN_0] = PE_SHP_HW_H13_SCL_V06T_DN75;}
			else if ( h_ratio[LX_PE_WIN_0] >= 375 )	{	pstParams->h_y_tap[LX_PE_WIN_0] = PE_SHP_HW_H13_SCL_V06T_DN50;}
			else									{	pstParams->h_y_tap[LX_PE_WIN_0] = PE_SHP_HW_H13_SCL_V06T_DN25;}
			if( v_ratio[LX_PE_WIN_0] >= 875 )		{	pstParams->v_y_tap[LX_PE_WIN_0] = PE_SHP_HW_H13_SCL_V06T_NORM;}
			else if ( v_ratio[LX_PE_WIN_0] >= 625 )	{	pstParams->v_y_tap[LX_PE_WIN_0] = PE_SHP_HW_H13_SCL_V06T_DN75;}
			else if ( v_ratio[LX_PE_WIN_0] >= 375 )	{	pstParams->v_y_tap[LX_PE_WIN_0] = PE_SHP_HW_H13_SCL_V06T_DN50;}
			else									{	pstParams->v_y_tap[LX_PE_WIN_0] = PE_SHP_HW_H13_SCL_V06T_DN25;}
			/* check vertical half mode */
			if((pstParams->v_y_tap[LX_PE_WIN_0]==PE_SHP_HW_H13_SCL_V06T_NORM) && \
				(ctrl0_info->o_win_v_size<=(ctrl0_info->i_win_v_size>>1)))
			{
				pstParams->v_y_tap[LX_PE_WIN_0]=PE_SHP_HW_H13_SCL_V06T_HALF;
			}
		}
		else
		{
			pstParams->h_y_tap[LX_PE_WIN_0] = (down12tMode)? PE_SHP_HW_H13_SCL_V12T_DN14:PE_SHP_HW_H13_SCL_V12T_NORM;
			pstParams->v_y_tap[LX_PE_WIN_0] = (down12tMode)? PE_SHP_HW_H13_SCL_V12T_DN14:PE_SHP_HW_H13_SCL_V12T_NORM;
		}
		/* R : check y tap */
		if(type6Tap[LX_PE_WIN_1])
		{
			/* check in vs. out ratio */
			if(ctrl1_info->o_win_h_size >= PE_SHP_HW_H13_SCL_IN_H_MIN && \
				ctrl1_info->o_win_v_size >= PE_SHP_HW_H13_SCL_IN_V_MIN && \
				ctrl1_info->i_win_h_size && ctrl1_info->i_win_v_size)
			{
				h_ratio[LX_PE_WIN_1] = (ctrl1_info->o_win_h_size<<10)/ctrl1_info->i_win_h_size;
				v_ratio[LX_PE_WIN_1] = (ctrl1_info->o_win_v_size<<10)/ctrl1_info->i_win_v_size;
			}
			else
			{
				h_ratio[LX_PE_WIN_1] = 1024;	//1.0
				v_ratio[LX_PE_WIN_1] = 1024;	//1.0
			}
			/* tap */
			if( h_ratio[LX_PE_WIN_1] >= 875 )		{	pstParams->h_y_tap[LX_PE_WIN_1] = PE_SHP_HW_H13_SCL_V06T_NORM;}
			else if ( h_ratio[LX_PE_WIN_1] >= 625 )	{	pstParams->h_y_tap[LX_PE_WIN_1] = PE_SHP_HW_H13_SCL_V06T_DN75;}
			else if ( h_ratio[LX_PE_WIN_1] >= 375 )	{	pstParams->h_y_tap[LX_PE_WIN_1] = PE_SHP_HW_H13_SCL_V06T_DN50;}
			else									{	pstParams->h_y_tap[LX_PE_WIN_1] = PE_SHP_HW_H13_SCL_V06T_DN25;}
			if( v_ratio[LX_PE_WIN_1] >= 875 )		{	pstParams->v_y_tap[LX_PE_WIN_1] = PE_SHP_HW_H13_SCL_V06T_NORM;}
			else if ( v_ratio[LX_PE_WIN_1] >= 625 )	{	pstParams->v_y_tap[LX_PE_WIN_1] = PE_SHP_HW_H13_SCL_V06T_DN75;}
			else if ( v_ratio[LX_PE_WIN_1] >= 375 )	{	pstParams->v_y_tap[LX_PE_WIN_1] = PE_SHP_HW_H13_SCL_V06T_DN50;}
			else									{	pstParams->v_y_tap[LX_PE_WIN_1] = PE_SHP_HW_H13_SCL_V06T_DN25;}
			/* check pip same h,v case(=split window), because of pre h scl down */
			if(disp0_info->out_type==LX_PE_OUT_PIP && (ctrl0_info->hv_same || ctrl1_info->hv_same))
			{
				pstParams->h_y_tap[LX_PE_WIN_1] = pstParams->v_y_tap[LX_PE_WIN_1];
			}
			/* check vertical half mode */
			if((pstParams->v_y_tap[LX_PE_WIN_1]==PE_SHP_HW_H13_SCL_V06T_NORM) && \
				(ctrl1_info->o_win_v_size<=(ctrl1_info->i_win_v_size>>1)))
			{
				pstParams->v_y_tap[LX_PE_WIN_1]=PE_SHP_HW_H13_SCL_V06T_HALF;
			}
		}
		else
		{
			pstParams->h_y_tap[LX_PE_WIN_1] = PE_SHP_HW_H13_SCL_V06T_NORM;	//6tap only for R
			pstParams->v_y_tap[LX_PE_WIN_1] = PE_SHP_HW_H13_SCL_V06T_NORM;	//6tap only for R
		}
		/* L : double-check y,c tap & idx */
		if(disp0_info->src_type==LX_PE_SRC_DTV && disp0_info->mode.is_adpt_st)
		{
			/* 20111203, sj.youm, fix 6tap for adaptive stream */
			if(pstParams->h_y_tap[LX_PE_WIN_0]==PE_SHP_HW_H13_SCL_V12T_NORM)
			{
				pstParams->h_y_tap[LX_PE_WIN_0] = PE_SHP_HW_H13_SCL_V06T_NORM;
				pstParams->h_c_tap[LX_PE_WIN_0] = pstParams->h_y_tap[LX_PE_WIN_0];
			}
			if(pstParams->v_y_tap[LX_PE_WIN_0]==PE_SHP_HW_H13_SCL_V12T_NORM)
			{
				pstParams->v_y_tap[LX_PE_WIN_0] = PE_SHP_HW_H13_SCL_V06T_NORM;
				pstParams->v_c_tap[LX_PE_WIN_0] = pstParams->v_y_tap[LX_PE_WIN_0];
			}
			pstParams->cur_scl_idx[LX_PE_WIN_0].h_luma_id = PE_SHP_HW_H13_IDX_Y_DFLT;
			pstParams->cur_scl_idx[LX_PE_WIN_0].v_luma_id = PE_SHP_HW_H13_IDX_Y_DFLT;
			pstParams->cur_scl_idx[LX_PE_WIN_0].h_chrm_id = PE_SHP_HW_H13_IDX_C_DFLT;
			pstParams->cur_scl_idx[LX_PE_WIN_0].v_chrm_id = PE_SHP_HW_H13_IDX_C_DFLT;
		}
		else
		{
			pstParams->h_c_tap[LX_PE_WIN_0] = pstParams->h_y_tap[LX_PE_WIN_0];
			pstParams->v_c_tap[LX_PE_WIN_0] = pstParams->v_y_tap[LX_PE_WIN_0];
			pstParams->cur_scl_idx[LX_PE_WIN_0].h_luma_id = pIdx_Info->cur_scl_idx[LX_PE_WIN_0].h_luma_id;
			pstParams->cur_scl_idx[LX_PE_WIN_0].v_luma_id = pIdx_Info->cur_scl_idx[LX_PE_WIN_0].v_luma_id;
			pstParams->cur_scl_idx[LX_PE_WIN_0].h_chrm_id = pIdx_Info->cur_scl_idx[LX_PE_WIN_0].h_chrm_id;
			pstParams->cur_scl_idx[LX_PE_WIN_0].v_chrm_id = pIdx_Info->cur_scl_idx[LX_PE_WIN_0].v_chrm_id;
		}
		/* R : double-check y,c tap & idx */
		#ifdef PE_SHP_HW_H13_USE_MSR_FOR_VENC_ON
		if(disp0_info->mode.is_venc)
		{
			pstParams->h_c_tap[LX_PE_WIN_1] = pstParams->h_y_tap[LX_PE_WIN_1];
			pstParams->v_c_tap[LX_PE_WIN_1] = pstParams->v_y_tap[LX_PE_WIN_1];
			pstParams->cur_scl_idx[LX_PE_WIN_1].h_luma_id = PE_SHP_HW_H13_IDX_Y_DFLT;
			pstParams->cur_scl_idx[LX_PE_WIN_1].v_luma_id = PE_SHP_HW_H13_IDX_Y_DFLT;
			pstParams->cur_scl_idx[LX_PE_WIN_1].h_chrm_id = PE_SHP_HW_H13_IDX_C_DFLT;
			pstParams->cur_scl_idx[LX_PE_WIN_1].v_chrm_id = PE_SHP_HW_H13_IDX_C_DFLT;
		}
		else
		{
			pstParams->h_c_tap[LX_PE_WIN_1] = pstParams->h_y_tap[LX_PE_WIN_1];
			pstParams->v_c_tap[LX_PE_WIN_1] = pstParams->v_y_tap[LX_PE_WIN_1];
			pstParams->cur_scl_idx[LX_PE_WIN_1].h_luma_id = pIdx_Info->cur_scl_idx[LX_PE_WIN_1].h_luma_id;
			pstParams->cur_scl_idx[LX_PE_WIN_1].v_luma_id = pIdx_Info->cur_scl_idx[LX_PE_WIN_1].v_luma_id;
			pstParams->cur_scl_idx[LX_PE_WIN_1].h_chrm_id = pIdx_Info->cur_scl_idx[LX_PE_WIN_1].h_chrm_id;
			pstParams->cur_scl_idx[LX_PE_WIN_1].v_chrm_id = pIdx_Info->cur_scl_idx[LX_PE_WIN_1].v_chrm_id;
		}
		#else
		pstParams->h_c_tap[LX_PE_WIN_1] = pstParams->h_y_tap[LX_PE_WIN_1];
		pstParams->v_c_tap[LX_PE_WIN_1] = pstParams->v_y_tap[LX_PE_WIN_1];
		pstParams->cur_scl_idx[LX_PE_WIN_1].h_luma_id = pIdx_Info->cur_scl_idx[LX_PE_WIN_1].h_luma_id;
		pstParams->cur_scl_idx[LX_PE_WIN_1].v_luma_id = pIdx_Info->cur_scl_idx[LX_PE_WIN_1].v_luma_id;
		pstParams->cur_scl_idx[LX_PE_WIN_1].h_chrm_id = pIdx_Info->cur_scl_idx[LX_PE_WIN_1].h_chrm_id;
		pstParams->cur_scl_idx[LX_PE_WIN_1].v_chrm_id = pIdx_Info->cur_scl_idx[LX_PE_WIN_1].v_chrm_id;
		#endif
	}while(0);
	return ret;
}
/**
 * check the scl value is valid or not
 *
 * @param   *pstParams [in] PE_SHP_HW_H13_SETTINGS_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static void PE_SHP_HW_H13_CheckValidScalerFilterValue(PE_SHP_HW_H13_SETTINGS_T *pstParams)
{
	static UINT32 count = 0;
	LX_PE_SHP_SCLFILTER_T *idx0 = &pstParams->cur_scl_idx[LX_PE_WIN_0];
	LX_PE_SHP_SCLFILTER_T *idx1 = &pstParams->cur_scl_idx[LX_PE_WIN_1];
	PE_SHP_HW_H13_SCL hy_tap0 = pstParams->h_y_tap[LX_PE_WIN_0];
	PE_SHP_HW_H13_SCL hc_tap0 = pstParams->h_c_tap[LX_PE_WIN_0];
	PE_SHP_HW_H13_SCL vy_tap0 = pstParams->v_y_tap[LX_PE_WIN_0];
	PE_SHP_HW_H13_SCL vc_tap0 = pstParams->v_c_tap[LX_PE_WIN_0];
	PE_SHP_HW_H13_SCL hy_tap1 = pstParams->h_y_tap[LX_PE_WIN_1];
	PE_SHP_HW_H13_SCL hc_tap1 = pstParams->h_c_tap[LX_PE_WIN_1];
	PE_SHP_HW_H13_SCL vy_tap1 = pstParams->v_y_tap[LX_PE_WIN_1];
	PE_SHP_HW_H13_SCL vc_tap1 = pstParams->v_c_tap[LX_PE_WIN_1];
	do{
		if(PE_KDRV_VER_H13)
		{
			/* L : h luma id check */
			PE_SHP_HW_H13_DBG_CHECK_CODE(count,(hy_tap0==PE_SHP_HW_H13_SCL_V12T_NORM&&\
				idx0->h_luma_id>=PE_SHP_SCL_HW_Y_H_12T_COEF_NUM),\
				idx0->h_luma_id=PE_SHP_HW_H13_IDX_Y_DFLT,\
				"[%s,%d] hy_tap0(%d), h_luma_id(%d) is out of range.\n",\
				__F__,__L__,hy_tap0,idx0->h_luma_id);
			PE_SHP_HW_H13_DBG_CHECK_CODE(count,(hy_tap0==PE_SHP_HW_H13_SCL_V06T_NORM&&\
				idx0->h_luma_id>=PE_SHP_SCL_HW_Y_H_08T_COEF_NUM),\
				idx0->h_luma_id=PE_SHP_HW_H13_IDX_Y_DFLT,\
				"[%s,%d] hy_tap0(%d), h_luma_id(%d) is out of range.\n",\
				__F__,__L__,hy_tap0,idx0->h_luma_id);
			/* L : v luma id check */
			PE_SHP_HW_H13_DBG_CHECK_CODE(count,(vy_tap0==PE_SHP_HW_H13_SCL_V12T_NORM&&\
				idx0->v_luma_id>=PE_SHP_SCL_HW_Y_V_12T_COEF_NUM),\
				idx0->v_luma_id=PE_SHP_HW_H13_IDX_Y_DFLT,\
				"[%s,%d] vy_tap0(%d), v_luma_id(%d) is out of range.\n",\
				__F__,__L__,vy_tap0,idx0->v_luma_id);
			PE_SHP_HW_H13_DBG_CHECK_CODE(count,((vy_tap0==PE_SHP_HW_H13_SCL_V06T_NORM||\
				vy_tap0==PE_SHP_HW_H13_SCL_V06T_HALF)&&\
				idx0->v_luma_id>=PE_SHP_SCL_HW_Y_V_06T_COEF_NUM),\
				idx0->v_luma_id=PE_SHP_HW_H13_IDX_Y_DFLT,\
				"[%s,%d] vy_tap0(%d), v_luma_id(%d) is out of range.\n",\
				__F__,__L__,vy_tap0,idx0->v_luma_id);
			/* L : h chroma id check */
			PE_SHP_HW_H13_DBG_CHECK_CODE(count,((hc_tap0==PE_SHP_HW_H13_SCL_V12T_NORM||\
				hc_tap0==PE_SHP_HW_H13_SCL_V06T_NORM)&&\
				idx0->h_chrm_id>=PE_SHP_SCL_HW_C_08T_COEF_NUM),\
				idx0->h_chrm_id=PE_SHP_HW_H13_IDX_C_DFLT,\
				"[%s,%d] hc_tap0(%d), h_chrm_id(%d) is out of range.\n",\
				__F__,__L__,hc_tap0,idx0->h_chrm_id);
			/* L : v chroma id check */
			PE_SHP_HW_H13_DBG_CHECK_CODE(count,((vc_tap0==PE_SHP_HW_H13_SCL_V12T_NORM||\
				vc_tap0==PE_SHP_HW_H13_SCL_V06T_NORM||\
				vc_tap0==PE_SHP_HW_H13_SCL_V06T_HALF)&&\
				idx0->v_chrm_id>=PE_SHP_SCL_HW_C_04T_COEF_NUM),\
				idx0->v_chrm_id=PE_SHP_HW_H13_IDX_C_DFLT,\
				"[%s,%d] vc_tap0(%d), v_chrm_id(%d) is out of range.\n",\
				__F__,__L__,vc_tap0,idx0->v_chrm_id);
			/* R : h luma id check */
			PE_SHP_HW_H13_DBG_CHECK_CODE(count,(hy_tap1==PE_SHP_HW_H13_SCL_V12T_NORM),\
				hy_tap1=PE_SHP_HW_H13_SCL_V06T_NORM,\
				"[%s,%d] hy_tap1(%d), is not support.\n",\
				__F__,__L__,hy_tap1);
			PE_SHP_HW_H13_DBG_CHECK_CODE(count,(hy_tap1==PE_SHP_HW_H13_SCL_V06T_NORM&&\
				idx1->h_luma_id>=PE_SHP_SCL_HW_Y_H_08T_COEF_NUM),\
				idx1->h_luma_id=PE_SHP_HW_H13_IDX_Y_DFLT,\
				"[%s,%d] hy_tap1(%d), h_luma_id(%d) is out of range.\n",\
				__F__,__L__,hy_tap1,idx1->h_luma_id);
			/* R : v luma id check */
			PE_SHP_HW_H13_DBG_CHECK_CODE(count,(vy_tap1==PE_SHP_HW_H13_SCL_V12T_NORM),\
				vy_tap1=PE_SHP_HW_H13_SCL_V06T_NORM,\
				"[%s,%d] vy_tap1(%d), is not support.\n",\
				__F__,__L__,vy_tap1);
			PE_SHP_HW_H13_DBG_CHECK_CODE(count,((vy_tap1==PE_SHP_HW_H13_SCL_V06T_NORM||\
				vy_tap1==PE_SHP_HW_H13_SCL_V06T_HALF)&&\
				idx1->v_luma_id>=PE_SHP_SCL_HW_Y_V_06T_COEF_NUM),\
				idx1->v_luma_id=PE_SHP_HW_H13_IDX_Y_DFLT,\
				"[%s,%d] vy_tap1(%d), v_luma_id(%d) is out of range.\n",\
				__F__,__L__,vy_tap1,idx1->v_luma_id);
			/* R : h chroma id check */
			PE_SHP_HW_H13_DBG_CHECK_CODE(count,(hc_tap1==PE_SHP_HW_H13_SCL_V12T_NORM),\
				hc_tap1=PE_SHP_HW_H13_SCL_V06T_NORM,\
				"[%s,%d] hc_tap1(%d), is not support.\n",\
				__F__,__L__,hc_tap1);
			PE_SHP_HW_H13_DBG_CHECK_CODE(count,(hc_tap1==PE_SHP_HW_H13_SCL_V06T_NORM&&\
				idx1->h_chrm_id>=PE_SHP_SCL_HW_C_08T_COEF_NUM),\
				idx1->h_chrm_id=PE_SHP_HW_H13_IDX_C_DFLT,\
				"[%s,%d] hc_tap0(%d), h_chrm_id(%d) is out of range.\n",\
				__F__,__L__,hc_tap1,idx1->h_chrm_id);
			/* R : v chroma id check */
			PE_SHP_HW_H13_DBG_CHECK_CODE(count,(vc_tap1==PE_SHP_HW_H13_SCL_V12T_NORM),\
				vc_tap1=PE_SHP_HW_H13_SCL_V06T_NORM,\
				"[%s,%d] vc_tap1(%d), is not support.\n",\
				__F__,__L__,vc_tap1);
			PE_SHP_HW_H13_DBG_CHECK_CODE(count,((vc_tap1==PE_SHP_HW_H13_SCL_V06T_NORM||\
				vc_tap1==PE_SHP_HW_H13_SCL_V06T_HALF)&&\
				idx1->v_chrm_id>=PE_SHP_SCL_HW_C_04T_COEF_NUM),\
				idx1->v_chrm_id=PE_SHP_HW_H13_IDX_C_DFLT,\
				"[%s,%d] vc_tap1(%d), v_chrm_id(%d) is out of range.\n",\
				__F__,__L__,vc_tap1,idx1->v_chrm_id);

			/* idx control, down scale case */
			PE_SHP_HW_H13_DBG_CHECK_CODE(count,\
				(hy_tap0==PE_SHP_HW_H13_SCL_V12T_DN14||hy_tap0==PE_SHP_HW_H13_SCL_V06T_DN75 ||\
				hy_tap0==PE_SHP_HW_H13_SCL_V06T_DN50||hy_tap0==PE_SHP_HW_H13_SCL_V06T_DN25),\
				idx0->h_luma_id=0,"[%s,%d]hy_tap0(DOWN):%d,h_luma_id=0\n",__F__,__L__,hy_tap0);
			PE_SHP_HW_H13_DBG_CHECK_CODE(count,\
				(vy_tap0==PE_SHP_HW_H13_SCL_V12T_DN14||vy_tap0==PE_SHP_HW_H13_SCL_V06T_DN75||\
				vy_tap0==PE_SHP_HW_H13_SCL_V06T_DN50||vy_tap0==PE_SHP_HW_H13_SCL_V06T_DN25),\
				idx0->v_luma_id=0,"[%s,%d]vy_tap0(DOWN):%d,v_luma_id=0\n",__F__,__L__,vy_tap0);
			PE_SHP_HW_H13_DBG_CHECK_CODE(count,\
				(hc_tap0==PE_SHP_HW_H13_SCL_V12T_DN14||hc_tap0==PE_SHP_HW_H13_SCL_V06T_DN75||\
				hc_tap0==PE_SHP_HW_H13_SCL_V06T_DN50||hc_tap0==PE_SHP_HW_H13_SCL_V06T_DN25),\
				idx0->h_chrm_id=0,"[%s,%d]hc_tap0(DOWN):%d,h_chrm_id=0\n",__F__,__L__,hc_tap0);
			PE_SHP_HW_H13_DBG_CHECK_CODE(count,\
				(vc_tap0==PE_SHP_HW_H13_SCL_V12T_DN14||vc_tap0==PE_SHP_HW_H13_SCL_V06T_DN75||\
				vc_tap0==PE_SHP_HW_H13_SCL_V06T_DN50||vc_tap0==PE_SHP_HW_H13_SCL_V06T_DN25),\
				idx0->v_chrm_id=0,"[%s,%d]vc_tap0(DOWN):%d,v_chrm_id=0\n",__F__,__L__,vc_tap0);
			PE_SHP_HW_H13_DBG_CHECK_CODE(count,\
				(hy_tap1==PE_SHP_HW_H13_SCL_V12T_DN14||hy_tap1==PE_SHP_HW_H13_SCL_V06T_DN75||\
				hy_tap1==PE_SHP_HW_H13_SCL_V06T_DN50||hy_tap1==PE_SHP_HW_H13_SCL_V06T_DN25),\
				idx1->h_luma_id=0,"[%s,%d]hy_tap1(DOWN):%d,h_luma_id=0\n",__F__,__L__,hy_tap1);
			PE_SHP_HW_H13_DBG_CHECK_CODE(count,\
				(vy_tap1==PE_SHP_HW_H13_SCL_V12T_DN14||vy_tap1==PE_SHP_HW_H13_SCL_V06T_DN75||\
				vy_tap1==PE_SHP_HW_H13_SCL_V06T_DN50||vy_tap1==PE_SHP_HW_H13_SCL_V06T_DN25),\
				idx1->v_luma_id=0,"[%s,%d]vy_tap1(DOWN):%d,v_luma_id=0\n",__F__,__L__,vy_tap1);
			PE_SHP_HW_H13_DBG_CHECK_CODE(count,\
				(hc_tap1==PE_SHP_HW_H13_SCL_V12T_DN14||hc_tap1==PE_SHP_HW_H13_SCL_V06T_DN75||\
				hc_tap1==PE_SHP_HW_H13_SCL_V06T_DN50||hc_tap1==PE_SHP_HW_H13_SCL_V06T_DN25),\
				idx1->h_chrm_id=0,"[%s,%d]hc_tap1(DOWN):%d,h_chrm_id=0\n",__F__,__L__,hc_tap1);
			PE_SHP_HW_H13_DBG_CHECK_CODE(count,\
				(vc_tap1==PE_SHP_HW_H13_SCL_V12T_DN14||vc_tap1==PE_SHP_HW_H13_SCL_V06T_DN75||\
				vc_tap1==PE_SHP_HW_H13_SCL_V06T_DN50||vc_tap1==PE_SHP_HW_H13_SCL_V06T_DN25),\
				idx1->v_chrm_id=0,"[%s,%d]vc_tap1(DOWN):%d,v_chrm_id=0\n",__F__,__L__,vc_tap1);
			if(_g_shp_hw_h13_trace)
			{
				if(count>PE_PRINT_COUNT_NUM)
				{
					count=0;
				}
				count++;
			}
		}
		else
		{
			PE_SHP_HW_H13_DBG_PRINT("nothing to do.\n");
			idx0->h_luma_id=idx0->v_luma_id=idx0->h_chrm_id=idx0->v_chrm_id=0;
			idx1->h_luma_id=idx1->v_luma_id=idx1->h_chrm_id=idx1->v_chrm_id=0;
		}
	}while(0);
	return;
}
/**
 * check duplicating settings or not
 *
 * @param   win_id [in] LX_PE_WIN_ID
 * @param   pre_idx [in] LX_PE_SHP_SCLFILTER_T
 * @param   *cur_param [in] PE_SHP_HW_H13_SETTINGS_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static PE_SHP_HW_H13_FLAG_T PE_SHP_HW_H13_GetDoubleSetFlag(LX_PE_WIN_ID win_id,LX_PE_SHP_SCLFILTER_T pre_idx,PE_SHP_HW_H13_SETTINGS_T *cur_param)
{
	PE_SHP_HW_H13_FLAG_T flag = {0,0,0,0};
	PE_SHP_HW_H13_SETTINGS_T *pre_tap=&_g_pe_shp_hw_h13_info;
	PE_SHP_HW_H13_CHECK_CODE(win_id>=LX_PE_WIN_NUM,return flag,"[%s,%d] win_id(%d) error.\n",__F__,__L__,win_id);
	if(pre_tap->h_y_tap[win_id]!=cur_param->h_y_tap[win_id])
	{
		flag.h_y=1;
	}
	else if((cur_param->h_y_tap[win_id]==PE_SHP_HW_H13_SCL_V12T_NORM) ||
		(cur_param->h_y_tap[win_id]==PE_SHP_HW_H13_SCL_V06T_NORM) ||
		(cur_param->h_y_tap[win_id]==PE_SHP_HW_H13_SCL_V06T_HALF))
	{
		if(pre_idx.h_luma_id!=cur_param->cur_scl_idx[win_id].h_luma_id)
			flag.h_y=1;
	}
	if(pre_tap->h_c_tap[win_id]!=cur_param->h_c_tap[win_id])
	{
		flag.h_c=1;
	}
	else if((cur_param->h_c_tap[win_id]==PE_SHP_HW_H13_SCL_V12T_NORM) ||
		(cur_param->h_c_tap[win_id]==PE_SHP_HW_H13_SCL_V06T_NORM) ||
		(cur_param->h_c_tap[win_id]==PE_SHP_HW_H13_SCL_V06T_HALF))
	{
		if(pre_idx.h_chrm_id!=cur_param->cur_scl_idx[win_id].h_chrm_id)
			flag.h_c=1;
	}
	if(pre_tap->v_y_tap[win_id]!=cur_param->v_y_tap[win_id])
	{
		flag.v_y=1;
	}
	else if((cur_param->v_y_tap[win_id]==PE_SHP_HW_H13_SCL_V12T_NORM) ||
		(cur_param->v_y_tap[win_id]==PE_SHP_HW_H13_SCL_V06T_NORM) ||
		(cur_param->v_y_tap[win_id]==PE_SHP_HW_H13_SCL_V06T_HALF))
	{
		if(pre_idx.v_luma_id!=cur_param->cur_scl_idx[win_id].v_luma_id)
			flag.v_y=1;
	}
	if(pre_tap->v_c_tap[win_id]!=cur_param->v_c_tap[win_id])
	{
		flag.v_c=1;
	}
	else if((cur_param->v_c_tap[win_id]==PE_SHP_HW_H13_SCL_V12T_NORM) ||
		(cur_param->v_c_tap[win_id]==PE_SHP_HW_H13_SCL_V06T_NORM) ||
		(cur_param->v_c_tap[win_id]==PE_SHP_HW_H13_SCL_V06T_HALF))
	{
		if(pre_idx.v_chrm_id!=cur_param->cur_scl_idx[win_id].v_chrm_id)
			flag.v_c=1;
	}
	PE_SHP_HW_H13_DBG_PRINT("set[%d] flag y(h,v):%d,%d, c(h,v):%d,%d\n",win_id,flag.h_y,flag.v_y,flag.h_c,flag.v_c);
	return flag;
}
/**
 * check duplicating settings or not
 *
 * @param   win_id [in] LX_PE_WIN_ID
 * @param   flag [in] PE_SHP_HW_H13_FLAG_T
 * @param   *pstParams [in] PE_SHP_HW_H13_SETTINGS_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_SHP_HW_H13_SetSclCoeff(LX_PE_WIN_ID win_id, PE_SHP_HW_H13_FLAG_T flag, PE_SHP_HW_H13_SETTINGS_T *pstParams)
{
	int ret = RET_OK;
	UINT8 h_y_idx,h_c_idx,v_y_idx,v_c_idx,is_v12tap;
	PE_SHP_HW_H13_SCL h_y_tap,h_c_tap,v_y_tap,v_c_tap;
	PE_SHP_HW_H13_SCL_WR_CTRL_T wr_ctrl;
	PE_SHP_HW_H13_CHECK_CODE(win_id>=LX_PE_WIN_NUM,return RET_ERROR,"[%s,%d] win_id(%d) error.\n",__F__,__L__,win_id);
	wr_ctrl.base_addr = (win_id==LX_PE_WIN_0)? 0x0:0x2000;
	h_y_idx = pstParams->cur_scl_idx[win_id].h_luma_id;
	h_c_idx = pstParams->cur_scl_idx[win_id].h_chrm_id;
	v_y_idx = pstParams->cur_scl_idx[win_id].v_luma_id;
	v_c_idx = pstParams->cur_scl_idx[win_id].v_chrm_id;
	h_y_tap = pstParams->h_y_tap[win_id];
	h_c_tap = pstParams->h_c_tap[win_id];
	v_y_tap = pstParams->v_y_tap[win_id];
	v_c_tap = pstParams->v_c_tap[win_id];
	if(flag.h_y)
	{
		is_v12tap = (h_y_tap==PE_SHP_HW_H13_SCL_V12T_NORM || h_y_tap==PE_SHP_HW_H13_SCL_V12T_DN14)? 1:0;
		wr_ctrl.tbl_size = (is_v12tap)? \
			PE_SHP_SCL_HW_Y_H_12T_NUM:PE_SHP_SCL_HW_Y_H_08T_NUM;
		wr_ctrl.tap_type = (is_v12tap)? \
			PE_SHP_HW_H13_SCL_WR_TAP_V12TM_Y_H_12T:PE_SHP_HW_H13_SCL_WR_TAP_V06TM_Y_H_08T;
		switch(h_y_tap)
		{
			case PE_SHP_HW_H13_SCL_V12T_NORM:
				PE_SHP_HW_H13_DBG_PRINT("set shp_scl_y_h_12tap_coeff[%d]\n",h_y_idx);
				PE_SHP_HW_H13_WrSclCoeff(wr_ctrl,&(shp_scl_y_h_12tap_coeff[h_y_idx][0]));
				break;
			case PE_SHP_HW_H13_SCL_V12T_DN14:
				PE_SHP_HW_H13_DBG_PRINT("set shp_scl_y_h_12tap_dn50_coeff\n");
				PE_SHP_HW_H13_WrSclCoeff(wr_ctrl,&(shp_scl_y_h_12tap_dn50_coeff[0]));
				break;
			case PE_SHP_HW_H13_SCL_V06T_DN75:
				PE_SHP_HW_H13_DBG_PRINT("set shp_scl_y_h_08tap_dn75_coeff\n");
				PE_SHP_HW_H13_WrSclCoeff(wr_ctrl,&(shp_scl_y_h_08tap_dn75_coeff[0]));
				break;
			case PE_SHP_HW_H13_SCL_V06T_DN50:
				PE_SHP_HW_H13_DBG_PRINT("set shp_scl_y_h_08tap_dn50_coeff\n");
				PE_SHP_HW_H13_WrSclCoeff(wr_ctrl,&(shp_scl_y_h_08tap_dn50_coeff[0]));
				break;
			case PE_SHP_HW_H13_SCL_V06T_DN25:
				PE_SHP_HW_H13_DBG_PRINT("set shp_scl_y_h_08tap_dn25_coeff\n");
				PE_SHP_HW_H13_WrSclCoeff(wr_ctrl,&(shp_scl_y_h_08tap_dn25_coeff[0]));
				break;
			case PE_SHP_HW_H13_SCL_V06T_NORM:
			case PE_SHP_HW_H13_SCL_V06T_HALF:
			default:
				PE_SHP_HW_H13_DBG_PRINT("set shp_scl_y_h_08tap_coeff[%d]\n",h_y_idx);
				PE_SHP_HW_H13_WrSclCoeff(wr_ctrl,&(shp_scl_y_h_08tap_coeff[h_y_idx][0]));
				break;
		}
	}
	if(flag.h_c)
	{
		is_v12tap = (h_c_tap==PE_SHP_HW_H13_SCL_V12T_NORM || h_c_tap==PE_SHP_HW_H13_SCL_V12T_DN14)? 1:0;
		wr_ctrl.tbl_size = PE_SHP_SCL_HW_C_08T_NUM;
		wr_ctrl.tap_type = (is_v12tap)? \
			PE_SHP_HW_H13_SCL_WR_TAP_V12TM_C_H_08T:PE_SHP_HW_H13_SCL_WR_TAP_V06TM_C_H_08T;
		switch(h_c_tap)
		{
			case PE_SHP_HW_H13_SCL_V12T_NORM:
				PE_SHP_HW_H13_DBG_PRINT("set shp_scl_c_08tap_coeff[%d]\n",h_c_idx);
				PE_SHP_HW_H13_WrSclCoeff(wr_ctrl,&(shp_scl_c_08tap_coeff[h_c_idx][0]));
				break;
			case PE_SHP_HW_H13_SCL_V12T_DN14:
				PE_SHP_HW_H13_DBG_PRINT("set shp_scl_c_08tap_dn25_coeff\n");
				PE_SHP_HW_H13_WrSclCoeff(wr_ctrl,&(shp_scl_c_08tap_dn25_coeff[0]));
				break;
			case PE_SHP_HW_H13_SCL_V06T_DN75:
				PE_SHP_HW_H13_DBG_PRINT("set shp_scl_c_08tap_dn75_coeff\n");
				PE_SHP_HW_H13_WrSclCoeff(wr_ctrl,&(shp_scl_c_08tap_dn75_coeff[0]));
				break;
			case PE_SHP_HW_H13_SCL_V06T_DN50:
				PE_SHP_HW_H13_DBG_PRINT("set shp_scl_c_08tap_dn50_coeff\n");
				PE_SHP_HW_H13_WrSclCoeff(wr_ctrl,&(shp_scl_c_08tap_dn50_coeff[0]));
				break;
			case PE_SHP_HW_H13_SCL_V06T_DN25:
				PE_SHP_HW_H13_DBG_PRINT("set shp_scl_c_08tap_dn25_coeff\n");
				PE_SHP_HW_H13_WrSclCoeff(wr_ctrl,&(shp_scl_c_08tap_dn25_coeff[0]));
				break;
			case PE_SHP_HW_H13_SCL_V06T_NORM:
			case PE_SHP_HW_H13_SCL_V06T_HALF:
			default:
				PE_SHP_HW_H13_DBG_PRINT("set shp_scl_c_08tap_coeff[%d]\n",h_c_idx);
				PE_SHP_HW_H13_WrSclCoeff(wr_ctrl,&(shp_scl_c_08tap_coeff[h_c_idx][0]));
				break;
		}
		#ifndef PE_SHP_HW_H13_USE_V12TM_C08T_SEPARATE_HV
		if(flag.v_c && is_v12tap)
		{
			flag.v_c = 0;	// avoid double setting
		}
		#endif
	}
	if(flag.v_y)
	{
		is_v12tap = (v_y_tap==PE_SHP_HW_H13_SCL_V12T_NORM || v_y_tap==PE_SHP_HW_H13_SCL_V12T_DN14)? 1:0;
		wr_ctrl.tbl_size = (is_v12tap)? \
			PE_SHP_SCL_HW_Y_V_12T_NUM:PE_SHP_SCL_HW_Y_V_06T_NUM;
		wr_ctrl.tap_type = (is_v12tap)? \
			PE_SHP_HW_H13_SCL_WR_TAP_V12TM_Y_V_12T:PE_SHP_HW_H13_SCL_WR_TAP_V06TM_Y_V_06T;
		switch(v_y_tap)
		{
			case PE_SHP_HW_H13_SCL_V12T_NORM:
				PE_SHP_HW_H13_DBG_PRINT("set shp_scl_y_v_12tap_coeff[%d]\n",v_y_idx);
				PE_SHP_HW_H13_WrSclCoeff(wr_ctrl,&(shp_scl_y_v_12tap_coeff[v_y_idx][0]));
				break;
			case PE_SHP_HW_H13_SCL_V12T_DN14:
				PE_SHP_HW_H13_DBG_PRINT("set shp_scl_y_v_12tap_dn14_coeff\n");
				PE_SHP_HW_H13_WrSclCoeff(wr_ctrl,&(shp_scl_y_v_12tap_dn14_coeff[0]));
				break;
			case PE_SHP_HW_H13_SCL_V06T_HALF:
				PE_SHP_HW_H13_DBG_PRINT("set shp_scl_y_v_06tap_half_coeff[%d]\n",v_y_idx);
				PE_SHP_HW_H13_WrSclCoeff(wr_ctrl,&(shp_scl_y_v_06tap_half_coeff[v_y_idx][0]));
				break;
			case PE_SHP_HW_H13_SCL_V06T_DN75:
				PE_SHP_HW_H13_DBG_PRINT("set shp_scl_y_v_06tap_dn75_coeff\n");
				PE_SHP_HW_H13_WrSclCoeff(wr_ctrl,&(shp_scl_y_v_06tap_dn75_coeff[0]));
				break;
			case PE_SHP_HW_H13_SCL_V06T_DN50:
				PE_SHP_HW_H13_DBG_PRINT("set shp_scl_y_v_06tap_dn50_coeff\n");
				PE_SHP_HW_H13_WrSclCoeff(wr_ctrl,&(shp_scl_y_v_06tap_dn50_coeff[0]));
				break;
			case PE_SHP_HW_H13_SCL_V06T_DN25:
				PE_SHP_HW_H13_DBG_PRINT("set shp_scl_y_v_06tap_dn25_coeff\n");
				PE_SHP_HW_H13_WrSclCoeff(wr_ctrl,&(shp_scl_y_v_06tap_dn25_coeff[0]));
			break;
			case PE_SHP_HW_H13_SCL_V06T_NORM:
			default:
				PE_SHP_HW_H13_DBG_PRINT("set shp_scl_y_v_06tap_coeff[%d]\n",v_y_idx);
				PE_SHP_HW_H13_WrSclCoeff(wr_ctrl,&(shp_scl_y_v_06tap_coeff[v_y_idx][0]));
				break;
		}
	}
	if(flag.v_c)
	{
		is_v12tap = (v_c_tap==PE_SHP_HW_H13_SCL_V12T_NORM || v_c_tap==PE_SHP_HW_H13_SCL_V12T_DN14)? 1:0;
		wr_ctrl.tbl_size = (is_v12tap)? \
			PE_SHP_SCL_HW_C_08T_NUM:PE_SHP_SCL_HW_C_04T_NUM;
		wr_ctrl.tap_type = (is_v12tap)? \
			PE_SHP_HW_H13_SCL_WR_TAP_V12TM_C_V_08T:PE_SHP_HW_H13_SCL_WR_TAP_V06TM_C_V_04T;
		switch(v_c_tap)
		{
			case PE_SHP_HW_H13_SCL_V12T_NORM:
				PE_SHP_HW_H13_DBG_PRINT("set shp_scl_c_08tap_coeff[%d]\n",v_c_idx);
				PE_SHP_HW_H13_WrSclCoeff(wr_ctrl,&(shp_scl_c_08tap_coeff[v_c_idx][0]));
				break;
			case PE_SHP_HW_H13_SCL_V12T_DN14:
				PE_SHP_HW_H13_DBG_PRINT("set shp_scl_c_08tap_dn25_coeff\n");
				PE_SHP_HW_H13_WrSclCoeff(wr_ctrl,&(shp_scl_c_08tap_dn25_coeff[0]));
				break;
			case PE_SHP_HW_H13_SCL_V06T_DN75:
				PE_SHP_HW_H13_DBG_PRINT("set shp_scl_c_04tap_dn75_coeff\n");
				PE_SHP_HW_H13_WrSclCoeff(wr_ctrl,&(shp_scl_c_04tap_dn75_coeff[0]));
				break;
			case PE_SHP_HW_H13_SCL_V06T_DN50:
				PE_SHP_HW_H13_DBG_PRINT("set shp_scl_c_04tap_dn50_coeff\n");
				PE_SHP_HW_H13_WrSclCoeff(wr_ctrl,&(shp_scl_c_04tap_dn50_coeff[0]));
				break;
			case PE_SHP_HW_H13_SCL_V06T_DN25:
				PE_SHP_HW_H13_DBG_PRINT("set shp_scl_c_04tap_dn25_coeff\n");
				PE_SHP_HW_H13_WrSclCoeff(wr_ctrl,&(shp_scl_c_04tap_dn25_coeff[0]));
				break;
			case PE_SHP_HW_H13_SCL_V06T_NORM:
			case PE_SHP_HW_H13_SCL_V06T_HALF:
			default:
				PE_SHP_HW_H13_DBG_PRINT("set shp_scl_c_04tap_coeff[%d]\n",v_c_idx);
				PE_SHP_HW_H13_WrSclCoeff(wr_ctrl,&(shp_scl_c_04tap_coeff[v_c_idx][0]));
				break;
		}
	}
	return ret;
}
/**
 * write scl coeff data
 *	- to avoid using PE_REG_WD()(ioremap and iounmap) every time
 *
 * @param   wr_ctrl [in] PE_SHP_HW_H13_SCL_WR_CTRL_T
 * @param   *reg [in] PE_REG_PARAM_T
 * @return  void
 * @see
 * @author
 */
static void PE_SHP_HW_H13_WrSclCoeff(PE_SHP_HW_H13_SCL_WR_CTRL_T wr_ctrl, const PE_REG_PARAM_T *reg)
{
	UINT32 i;
	UINT32 wr_addr,wr_data;

#define PE_SHP_HW_H13_MSL_WR(_a,_d)	\
	do{\
		if(PE_KDRV_VER_H13BX)\
		{PE_MSL_H13B0_Wr(_a,_d);PE_MSL_H13B0_WrFL(_a);}\
		else if(PE_KDRV_VER_H13AX)\
		{PE_MSL_H13_Wr(_a,_d);PE_MSL_H13_WrFL(_a);}\
	}while(0)
#define PE_SHP_HW_H13_MSR_WR(_a,_d)	\
	do{\
		if(PE_KDRV_VER_H13BX)\
		{PE_MSR_H13B0_Wr(_a,_d);PE_MSR_H13B0_WrFL(_a);}\
		else if(PE_KDRV_VER_H13AX)\
		{PE_MSR_H13_Wr(_a,_d);PE_MSR_H13_WrFL(_a);}\
	}while(0)

	for(i=0;i<wr_ctrl.tbl_size;i++)
	{
		wr_addr = reg[i].addr+wr_ctrl.base_addr;
		#ifdef PE_SHP_HW_H13_USE_V12TM_C08T_SEPARATE_HV
		if(wr_addr==0x1160)
		{
			/* modify MSL MSC_C_FIR_COEF_CTRL_L(0x1160) [6:4]h_v_sel */
			if(wr_ctrl.tap_type==PE_SHP_HW_H13_SCL_WR_TAP_V12TM_C_H_08T)
				wr_data = (reg[i].data&(~0x70))|0x40;
			else if(wr_ctrl.tap_type==PE_SHP_HW_H13_SCL_WR_TAP_V12TM_C_V_08T)
				wr_data = (reg[i].data&(~0x70))|0x20;
			else
				wr_data = reg[i].data;
		}
		else
		{
			wr_data = reg[i].data;
		}
		#else
		wr_data = reg[i].data;
		#endif
		switch(wr_addr)
		{
			case 0x1130:	PE_SHP_HW_H13_MSL_WR(msc_y_fir_coef_data0_l,	wr_data);	break;
			case 0x1134:	PE_SHP_HW_H13_MSL_WR(msc_y_fir_coef_data1_l,	wr_data);	break;
			case 0x1138:	PE_SHP_HW_H13_MSL_WR(msc_y_fir_coef_data2_l,	wr_data);	break;
			case 0x113c:	PE_SHP_HW_H13_MSL_WR(msc_y_fir_coef_data3_l,	wr_data);	break;
			case 0x1140:	PE_SHP_HW_H13_MSL_WR(msc_y_fir_coef_data4_l,	wr_data);	break;
			case 0x1144:	PE_SHP_HW_H13_MSL_WR(msc_y_fir_coef_data5_l,	wr_data);	break;
			case 0x1148:	PE_SHP_HW_H13_MSL_WR(msc_y_fir_coef_ctrl_l,		wr_data);	break;
			case 0x1150:	PE_SHP_HW_H13_MSL_WR(msc_c_fir_coef_data0_l,	wr_data);	break;
			case 0x1154:	PE_SHP_HW_H13_MSL_WR(msc_c_fir_coef_data1_l,	wr_data);	break;
			case 0x1158:	PE_SHP_HW_H13_MSL_WR(msc_c_fir_coef_data2_l,	wr_data);	break;
			case 0x115c:	PE_SHP_HW_H13_MSL_WR(msc_c_fir_coef_data3_l,	wr_data);	break;
			case 0x1160:	PE_SHP_HW_H13_MSL_WR(msc_c_fir_coef_ctrl_l,		wr_data);	break;
			case 0x3130:	PE_SHP_HW_H13_MSR_WR(msc_y_fir_coef_data0_r,	wr_data);	break;
			case 0x3134:	PE_SHP_HW_H13_MSR_WR(msc_y_fir_coef_data1_r,	wr_data);	break;
			case 0x3138:	PE_SHP_HW_H13_MSR_WR(msc_y_fir_coef_data2_r,	wr_data);	break;
			case 0x313c:	PE_SHP_HW_H13_MSR_WR(msc_y_fir_coef_data3_r,	wr_data);	break;
			case 0x3140:	PE_SHP_HW_H13_MSR_WR(msc_y_fir_coef_data4_r,	wr_data);	break;
			case 0x3144:	PE_SHP_HW_H13_MSR_WR(msc_y_fir_coef_data5_r,	wr_data);	break;
			case 0x3148:	PE_SHP_HW_H13_MSR_WR(msc_y_fir_coef_ctrl_r,		wr_data);	break;
			case 0x3150:	PE_SHP_HW_H13_MSR_WR(msc_c_fir_coef_data0_r,	wr_data);	break;
			case 0x3154:	PE_SHP_HW_H13_MSR_WR(msc_c_fir_coef_data1_r,	wr_data);	break;
			case 0x3158:	PE_SHP_HW_H13_MSR_WR(msc_c_fir_coef_data2_r,	wr_data);	break;
			case 0x315c:	PE_SHP_HW_H13_MSR_WR(msc_c_fir_coef_data3_r,	wr_data);	break;
			case 0x3160:	PE_SHP_HW_H13_MSR_WR(msc_c_fir_coef_ctrl_r,		wr_data);	break;
			default:
				PE_SHP_HW_H13_ERROR("[%s,%d]wrong case. [%03d]addr:0x%08x,base:0x%08x,data:0x%08x\n",\
					__F__,__L__,i,reg[i].addr,wr_ctrl.base_addr,wr_data);
				break;
		}
	}
	return;
}
/**
 * get current shp settings
 *
 * @param   *pstParams [in] PE_SHP_HW_H13_SETTINGS_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_SHP_HW_H13_GetCurShpSettings(PE_SHP_HW_H13_SETTINGS_T *pstParams)
{
	int ret = RET_OK;
	static UINT32 count = 0;
	PE_SHP_HW_H13_SETTINGS_T *pInfo = &_g_pe_shp_hw_h13_info;
	LX_PE_SHP_SCLFILTER_T *cur_scl0_info = &_g_pe_shp_hw_h13_info.cur_scl_idx[LX_PE_WIN_0];
	LX_PE_SHP_SCLFILTER_T *cur_scl1_info = &_g_pe_shp_hw_h13_info.cur_scl_idx[LX_PE_WIN_1];
	LX_PE_SHP_SCLFILTER_T *pre_scl0_info = &_g_pe_shp_hw_h13_info.pre_scl_idx[LX_PE_WIN_0];
	LX_PE_SHP_SCLFILTER_T *pre_scl1_info = &_g_pe_shp_hw_h13_info.pre_scl_idx[LX_PE_WIN_1];
	do{
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H13)
		{
			memcpy(pstParams,pInfo,sizeof(PE_SHP_HW_H13_SETTINGS_T));
			if(_g_shp_hw_h13_trace)
			{
				if(count>PE_PRINT_COUNT_NUM)
				{
					PE_SHP_HW_H13_DBG_PRINT("get cur_scl_idx[0] : wid:%d, luma h,v:%d,%d, chrm h,v:%d,%d\n",\
						cur_scl0_info->win_id,cur_scl0_info->h_luma_id,cur_scl0_info->v_luma_id,\
						cur_scl0_info->h_chrm_id,cur_scl0_info->v_chrm_id);
					PE_SHP_HW_H13_DBG_PRINT("get cur_scl_idx[1] : wid:%d, luma h,v:%d,%d, chrm h,v:%d,%d\n",\
						cur_scl1_info->win_id,cur_scl1_info->h_luma_id,cur_scl1_info->v_luma_id,\
						cur_scl1_info->h_chrm_id,cur_scl1_info->v_chrm_id);
					PE_SHP_HW_H13_DBG_PRINT("get pre_scl_idx[0] : wid:%d, luma h,v:%d,%d, chrm h,v:%d,%d\n",\
						pre_scl0_info->win_id,pre_scl0_info->h_luma_id,pre_scl0_info->v_luma_id,\
						pre_scl0_info->h_chrm_id,pre_scl0_info->v_chrm_id);
					PE_SHP_HW_H13_DBG_PRINT("get pre_scl_idx[1] : wid:%d, luma h,v:%d,%d, chrm h,v:%d,%d\n",\
						pre_scl1_info->win_id,pre_scl1_info->h_luma_id,pre_scl1_info->v_luma_id,\
						pre_scl1_info->h_chrm_id,pre_scl1_info->v_chrm_id);
					PE_SHP_HW_H13_DBG_PRINT("get h_y_tap:%d,%d, h_c_tap:%d,%d, v_y_tap:%d,%d, v_c_tap:%d,%d\n",\
						pInfo->h_y_tap[LX_PE_WIN_0],pInfo->h_y_tap[LX_PE_WIN_1],\
						pInfo->h_c_tap[LX_PE_WIN_0],pInfo->h_c_tap[LX_PE_WIN_1],\
						pInfo->v_y_tap[LX_PE_WIN_0],pInfo->v_y_tap[LX_PE_WIN_1],\
						pInfo->v_c_tap[LX_PE_WIN_0],pInfo->v_c_tap[LX_PE_WIN_1]);
					PE_SHP_HW_H13_DBG_PRINT("get y_fir:%d,%d, c_fir:%d,%d, pre_res_on:%d,%d\n",\
						pInfo->y_fir[LX_PE_WIN_0],pInfo->y_fir[LX_PE_WIN_1],\
						pInfo->c_fir[LX_PE_WIN_0],pInfo->c_fir[LX_PE_WIN_1],\
						pInfo->pre_res_en[LX_PE_WIN_0],pInfo->pre_res_en[LX_PE_WIN_1]);
					count=0;
				}
				count++;
			}
		}
		else
		{
			PE_SHP_HW_H13_DBG_PRINT("nothing to do.\n");
		}
	}while(0);
	return ret;
}
/**
 * read scaler filter coeff
 *
 * @param   *pstParams [in/out] LX_PE_ETC_TBL_T
 * @return  OK if success, ERROR otherwise.
 * @see     
 * @author
 */
int PE_SHP_HW_H13_RdSclFltCoeff(LX_PE_ETC_TBL_T *pstParams)
{
	int ret = RET_OK;
	char buffer[PE_TRACE_STR_SIZE];
	UINT32 sel=0, ofst=0, is_valid=0, step=0, cmd=0;
	static UINT32 rd_cnt=0;
	static UINT32 scldata[255];
	PE_SHP_HW_H13_SETTINGS_T *pInfo=&_g_pe_shp_hw_h13_info;
	PE_SHP_HW_H13_SCL h_y_tap0 = _g_pe_shp_hw_h13_info.h_y_tap[LX_PE_WIN_0];

#define WIN0	LX_PE_WIN_0
#define WIN1	LX_PE_WIN_1
#define PE_SHP_H13_PRINT_START		"START_OF_PRINT"
#define PE_SHP_H13_PRINT_END		"END_OF_PRINT"
#define PE_SHP_H13_PRINT_EXIT		"EXIT_OF_PRINT"
#define PE_SHP_H13_PRINT_RESRV		"RESERVED"
#define PE_SHP_H13_CASE_PRMENU(_cnt, _action, _buf, _item)	\
	case (_cnt):sprintf(_buf, "   [0x%02x] %s\n", (_item), #_item);_action;break
#define PE_SHP_H13_CASE_SPRINT(_cnt, _action, _buf, fmt, args...)	\
	case (_cnt):sprintf(_buf, fmt, ##args);_action;break
#define PE_SHP_H13_DFLT_SPRINT(_action, _buf, fmt, args...)		\
	default:sprintf(_buf, fmt, ##args);_action;break

	do{
		CHECK_KNULL(pstParams);
		CHECK_KNULL(pstParams->data);
		if (PE_KDRV_VER_H13)
		{
			if (pstParams->data[0]>=PE_SHP_HW_H13_RD_SCL_MAX)
			{
				sprintf(buffer, "%s", PE_SHP_H13_PRINT_EXIT);
				rd_cnt = 0;
			}
			else if (pstParams->data[0]==PE_SHP_HW_H13_RD_SCL_DISPLAY)
			{
				switch (rd_cnt)
				{
					PE_SHP_H13_CASE_SPRINT( 0, rd_cnt++, buffer, "%s", PE_SHP_H13_PRINT_START);
					PE_SHP_H13_CASE_SPRINT( 1, rd_cnt++, buffer,\
						"   tap_type(0~6:12t,14dn,6t,6t_hf,75dn,50dn,25dn)\n");
					PE_SHP_H13_CASE_SPRINT( 2, rd_cnt++, buffer,\
						"   MSL hy,vy,hc,vc tap:%d,%d,%d,%d\n", \
						pInfo->h_y_tap[WIN0], pInfo->v_y_tap[WIN0], \
						pInfo->h_c_tap[WIN0], pInfo->h_c_tap[WIN0]);
					PE_SHP_H13_CASE_SPRINT( 3, rd_cnt++, buffer,\
						"   MSL hy,vy,hc,vc idx:%d,%d,%d,%d\n", \
						pInfo->pre_scl_idx[WIN0].h_luma_id, pInfo->pre_scl_idx[WIN0].v_luma_id, \
						pInfo->pre_scl_idx[WIN0].h_chrm_id, pInfo->pre_scl_idx[WIN0].v_chrm_id);
					PE_SHP_H13_CASE_SPRINT( 4, rd_cnt++, buffer,\
						"   MSR hy,vy,hc,vc tap:%d,%d,%d,%d\n", \
						pInfo->h_y_tap[WIN1], pInfo->v_y_tap[WIN1], \
						pInfo->h_c_tap[WIN1], pInfo->h_c_tap[WIN1]);
					PE_SHP_H13_CASE_SPRINT( 5, rd_cnt++, buffer,\
						"   MSR hy,vy,hc,vc idx:%d,%d,%d,%d\n", \
						pInfo->pre_scl_idx[WIN1].h_luma_id, pInfo->pre_scl_idx[WIN1].v_luma_id, \
						pInfo->pre_scl_idx[WIN1].h_chrm_id, pInfo->pre_scl_idx[WIN1].v_chrm_id);
					PE_SHP_H13_CASE_SPRINT( 6, rd_cnt++, buffer, "%s", PE_SHP_H13_PRINT_RESRV);
					PE_SHP_H13_CASE_SPRINT( 7, rd_cnt++, buffer, "%s", PE_SHP_H13_PRINT_RESRV);
					PE_SHP_H13_CASE_SPRINT( 8, rd_cnt++, buffer, "%s", PE_SHP_H13_PRINT_RESRV);
					PE_SHP_H13_CASE_SPRINT( 9, rd_cnt++, buffer, "%s", PE_SHP_H13_PRINT_RESRV);
					PE_SHP_H13_CASE_SPRINT(10, rd_cnt++, buffer, "%s", PE_SHP_H13_PRINT_RESRV);
					PE_SHP_H13_CASE_PRMENU(11, rd_cnt++, buffer, PE_SHP_HW_H13_RD_SCL_MSL_Y_H);
					PE_SHP_H13_CASE_PRMENU(12, rd_cnt++, buffer, PE_SHP_HW_H13_RD_SCL_MSL_Y_V);
					PE_SHP_H13_CASE_PRMENU(13, rd_cnt++, buffer, PE_SHP_HW_H13_RD_SCL_MSL_C_H);
					PE_SHP_H13_CASE_PRMENU(14, rd_cnt++, buffer, PE_SHP_HW_H13_RD_SCL_MSL_C_V);
					PE_SHP_H13_CASE_PRMENU(15, rd_cnt++, buffer, PE_SHP_HW_H13_RD_SCL_MSR_Y_H);
					PE_SHP_H13_CASE_PRMENU(16, rd_cnt++, buffer, PE_SHP_HW_H13_RD_SCL_MSR_Y_V);
					PE_SHP_H13_CASE_PRMENU(17, rd_cnt++, buffer, PE_SHP_HW_H13_RD_SCL_MSR_C_H);
					PE_SHP_H13_CASE_PRMENU(18, rd_cnt++, buffer, PE_SHP_HW_H13_RD_SCL_MSR_C_V);
					PE_SHP_H13_CASE_PRMENU(19, rd_cnt++, buffer, PE_SHP_HW_H13_RD_SCL_SSC_Y_H);
					PE_SHP_H13_DFLT_SPRINT(rd_cnt=0, buffer, "%s", PE_SHP_H13_PRINT_END);
				}
			}
			else
			{
				is_valid = 0;
				cmd = pstParams->data[0];
				switch (cmd)
				{
					case PE_SHP_HW_H13_RD_SCL_MSL_Y_H:
					case PE_SHP_HW_H13_RD_SCL_MSL_Y_V:
						if(h_y_tap0==PE_SHP_HW_H13_SCL_V12T_NORM || \
							h_y_tap0==PE_SHP_HW_H13_SCL_V12T_DN14)	//12tap mode
						{
							if(rd_cnt==0)
							{
								PE_SHP_HW_H13_SetSclFltRnwMode(0x1);	//read mode
								sel = (cmd==PE_SHP_HW_H13_RD_SCL_MSL_Y_H)? 0x4:0x2;	//def flt h:v
								ofst = PE_SHP_HW_H13_RdLumaSclFltCoeff(WIN0,sel,&(scldata[0]));
								sel = (cmd==PE_SHP_HW_H13_RD_SCL_MSL_Y_H)? 0x5:0x3;	//sub flt h:v
								ofst = PE_SHP_HW_H13_RdLumaSclFltCoeff(WIN0,sel,&(scldata[ofst]));
								PE_SHP_HW_H13_SetSclFltRnwMode(0x0);	//write mode back
							}
							if(rd_cnt<16)
							{
								step = rd_cnt*7;
								sprintf(buffer, \
									"MSL y %s:[%02d]0x%08x,0x%08x,0x%08x,0x%08x,0x%08x,0x%08x\n", \
									(cmd==PE_SHP_HW_H13_RD_SCL_MSL_Y_H)? "h":"v", scldata[step], \
									scldata[step+1], scldata[step+2], scldata[step+3], \
									scldata[step+4], scldata[step+5], scldata[step+6]);
								is_valid = 1;
							}
							else if(rd_cnt<32)
							{
								step = rd_cnt*7;
								sprintf(buffer, \
									"MSL y %s:[%02d]0x%08x,0x%08x,0x%08x,0x%08x\n", \
									(cmd==PE_SHP_HW_H13_RD_SCL_MSL_Y_H)? "h":"v", scldata[step], \
									scldata[step+1], scldata[step+2], scldata[step+3], \
									scldata[step+4]);
								is_valid = 1;
							}
						}
						else	//6tap mode
						{
							if(rd_cnt==0)
							{
								PE_SHP_HW_H13_SetSclFltRnwMode(0x1);	//read mode
								sel = (cmd==PE_SHP_HW_H13_RD_SCL_MSL_Y_H)? 0x0:0x1;	//flt h:v
								ofst = PE_SHP_HW_H13_RdLumaSclFltCoeff(WIN0,sel,&(scldata[0]));
								PE_SHP_HW_H13_SetSclFltRnwMode(0x0);	//write mode back
							}
							if(rd_cnt<16)
							{
								step = rd_cnt*7;
								sprintf(buffer, \
									"MSL y %s:[%02d]0x%08x,0x%08x,0x%08x,0x%08x,0x%08x,0x%08x\n", \
									(cmd==PE_SHP_HW_H13_RD_SCL_MSL_Y_H)? "h":"v", scldata[step], \
									scldata[step+1], scldata[step+2], scldata[step+3], \
									scldata[step+4], scldata[step+5], scldata[step+6]);
								is_valid = 1;
							}
						}
						break;
					case PE_SHP_HW_H13_RD_SCL_MSL_C_H:
					case PE_SHP_HW_H13_RD_SCL_MSL_C_V:
						if(h_y_tap0==PE_SHP_HW_H13_SCL_V12T_NORM || \
							h_y_tap0==PE_SHP_HW_H13_SCL_V12T_DN14)	//12tap mode
						{
							if(rd_cnt==0)
							{
								PE_SHP_HW_H13_SetSclFltRnwMode(0x1);	//read mode
								#ifdef PE_SHP_HW_H13_USE_V12TM_C08T_SEPARATE_HV
								sel = (cmd==PE_SHP_HW_H13_RD_SCL_MSL_C_H)? 0x4:0x2;	//def flt h:v
								#else
								sel = 0x0;	//def flt h&v
								#endif
								ofst = PE_SHP_HW_H13_RdChromaSclFltCoeff(WIN0,sel,&(scldata[0]));
								PE_SHP_HW_H13_SetSclFltRnwMode(0x0);	//write mode back
							}
							if(rd_cnt<16)
							{
								step = rd_cnt*5;
								sprintf(buffer, \
									"MSL c %s:[%02d]0x%08x,0x%08x,0x%08x,0x%08x\n", \
									(cmd==PE_SHP_HW_H13_RD_SCL_MSL_C_H)? "h":"v", scldata[step], \
									scldata[step+1], scldata[step+2], scldata[step+3], \
									scldata[step+4]);
								is_valid = 1;
							}
						}
						else	//6tap mode
						{
							if(rd_cnt==0)
							{
								PE_SHP_HW_H13_SetSclFltRnwMode(0x1);	//read mode
								sel = (cmd==PE_SHP_HW_H13_RD_SCL_MSL_C_H)? 0x0:0x1;	//flt h:v
								ofst = PE_SHP_HW_H13_RdChromaSclFltCoeff(WIN0,sel,&(scldata[0]));
								PE_SHP_HW_H13_SetSclFltRnwMode(0x0);	//write mode back
							}
							if(rd_cnt<16)
							{
								step = rd_cnt*5;
								if(cmd==PE_SHP_HW_H13_RD_SCL_MSL_C_H)
								{
									sprintf(buffer, "MSL c h:[%02d]0x%08x,0x%08x,0x%08x,0x%08x\n", \
										scldata[step], scldata[step+1], scldata[step+2], \
										scldata[step+3], scldata[step+4]);
								}
								else
								{
									sprintf(buffer, "MSL c v:[%02d]0x%08x,0x%08x\n", \
										scldata[step], scldata[step+1], scldata[step+2]);
								}
								is_valid = 1;
							}
						}
						break;
					case PE_SHP_HW_H13_RD_SCL_MSR_Y_H:
					case PE_SHP_HW_H13_RD_SCL_MSR_Y_V:
						if(rd_cnt==0)
						{
							PE_SHP_HW_H13_SetSclFltRnwMode(0x1);	//read mode
							sel = (cmd==PE_SHP_HW_H13_RD_SCL_MSR_Y_H)? 0x0:0x1;	//flt h:v
							ofst = PE_SHP_HW_H13_RdLumaSclFltCoeff(WIN1,sel,&(scldata[0]));
							PE_SHP_HW_H13_SetSclFltRnwMode(0x0);	//write mode back
						}
						if(rd_cnt<16)
						{
							step = rd_cnt*7;
							sprintf(buffer, \
								"MSL y %s:[%02d]0x%08x,0x%08x,0x%08x,0x%08x,0x%08x,0x%08x\n", \
								(cmd==PE_SHP_HW_H13_RD_SCL_MSR_Y_H)? "h":"v", scldata[step], \
								scldata[step+1], scldata[step+2], scldata[step+3], \
								scldata[step+4], scldata[step+5], scldata[step+6]);
							is_valid = 1;
						}
						break;
					case PE_SHP_HW_H13_RD_SCL_MSR_C_H:
					case PE_SHP_HW_H13_RD_SCL_MSR_C_V:
						if(rd_cnt==0)
						{
							PE_SHP_HW_H13_SetSclFltRnwMode(0x1);	//read mode
							sel = (cmd==PE_SHP_HW_H13_RD_SCL_MSR_C_H)? 0x0:0x1;	//flt h:v
							ofst = PE_SHP_HW_H13_RdChromaSclFltCoeff(WIN1,sel,&(scldata[0]));
							PE_SHP_HW_H13_SetSclFltRnwMode(0x0);	//write mode back
						}
						if(rd_cnt<16)
						{
							step = rd_cnt*5;
							if(cmd==PE_SHP_HW_H13_RD_SCL_MSR_C_H)
							{
								sprintf(buffer, "MSL c h:[%02d]0x%08x,0x%08x,0x%08x,0x%08x\n", \
									scldata[step], scldata[step+1], scldata[step+2], \
									scldata[step+3], scldata[step+4]);
							}
							else
							{
								sprintf(buffer, "MSL c v:[%02d]0x%08x,0x%08x\n", \
									scldata[step], scldata[step+1], scldata[step+2]);
							}
							is_valid = 1;
						}
						break;
					case PE_SHP_HW_H13_RD_SCL_SSC_Y_H:
					default:
						if(rd_cnt==0)
						{
							PE_SHP_HW_H13_SetSclFltRnwMode(0x1);	//read mode
							sel = 0x0;	//h flt
							ofst = PE_SHP_HW_H13_RdSubSclFltCoeff(sel,&(scldata[0]));
							PE_SHP_HW_H13_SetSclFltRnwMode(0x0);	//write mode back
						}
						if(rd_cnt<16)
						{
							step = rd_cnt*5;
							sprintf(buffer, "SSC y h:[%02d]0x%08x,0x%08x,0x%08x,0x%08x\n", \
								scldata[step], scldata[step+1], scldata[step+2], \
								scldata[step+3], scldata[step+4]);
							is_valid = 1;
						}
						break;
				}
				if(is_valid)
				{
					rd_cnt++;
				}
				else
				{
					sprintf(buffer, "%s", PE_SHP_H13_PRINT_END);
					rd_cnt = 0;
				}
			}
			memcpy(pstParams->data, buffer, sizeof(char)*PE_TRACE_STR_SIZE);
		}
	}while (0);
	return ret;
}
/**
 * set scaler filter rd and wr mode
 *
 * @param   rnw_mode [in] UINT32
 * @return  void
 * @see     
 * @author
 */
static void PE_SHP_HW_H13_SetSclFltRnwMode(UINT32 rnw_mode)
{
	if(PE_KDRV_VER_H13BX)
	{
		PE_MSL_H13B0_RdFL(msc_y_fir_coef_ctrl_l);
		PE_MSL_H13B0_Wr01(msc_y_fir_coef_ctrl_l,	rnw,	rnw_mode? 0x1:0x0);	//0:wr, 1:rd
		PE_MSL_H13B0_WrFL(msc_y_fir_coef_ctrl_l);
		PE_MSL_H13B0_RdFL(msc_c_fir_coef_ctrl_l);
		PE_MSL_H13B0_Wr01(msc_c_fir_coef_ctrl_l,	rnw,	rnw_mode? 0x1:0x0);	//0:wr, 1:rd
		PE_MSL_H13B0_WrFL(msc_c_fir_coef_ctrl_l);
		PE_MSR_H13B0_RdFL(msc_y_fir_coef_ctrl_r);
		PE_MSR_H13B0_Wr01(msc_y_fir_coef_ctrl_r,	rnw,	rnw_mode? 0x1:0x0);	//0:wr, 1:rd
		PE_MSR_H13B0_WrFL(msc_y_fir_coef_ctrl_r);
		PE_MSR_H13B0_RdFL(msc_c_fir_coef_ctrl_r);
		PE_MSR_H13B0_Wr01(msc_c_fir_coef_ctrl_r,	rnw,	rnw_mode? 0x1:0x0);	//0:wr, 1:rd
		PE_MSR_H13B0_WrFL(msc_c_fir_coef_ctrl_r);

		PE_SSC_H13_RdFL(ssc_h_fir_coef_ctrl);
		PE_SSC_H13_Wr01(ssc_h_fir_coef_ctrl,		rnw,	rnw_mode? 0x1:0x0);	//0:wr, 1:rd
		PE_SSC_H13_WrFL(ssc_h_fir_coef_ctrl);
	}
	else if(PE_KDRV_VER_H13AX)
	{
		PE_MSL_H13_RdFL(msc_y_fir_coef_ctrl_l);
		PE_MSL_H13_Wr01(msc_y_fir_coef_ctrl_l,		rnw,	rnw_mode? 0x1:0x0);	//0:wr, 1:rd
		PE_MSL_H13_WrFL(msc_y_fir_coef_ctrl_l);
		PE_MSL_H13_RdFL(msc_c_fir_coef_ctrl_l);
		PE_MSL_H13_Wr01(msc_c_fir_coef_ctrl_l,		rnw,	rnw_mode? 0x1:0x0);	//0:wr, 1:rd
		PE_MSL_H13_WrFL(msc_c_fir_coef_ctrl_l);
		PE_MSR_H13_RdFL(msc_y_fir_coef_ctrl_r);
		PE_MSR_H13_Wr01(msc_y_fir_coef_ctrl_r,		rnw,	rnw_mode? 0x1:0x0);	//0:wr, 1:rd
		PE_MSR_H13_WrFL(msc_y_fir_coef_ctrl_r);
		PE_MSR_H13_RdFL(msc_c_fir_coef_ctrl_r);
		PE_MSR_H13_Wr01(msc_c_fir_coef_ctrl_r,		rnw,	rnw_mode? 0x1:0x0);	//0:wr, 1:rd
		PE_MSR_H13_WrFL(msc_c_fir_coef_ctrl_r);

		PE_SSC_H13_RdFL(ssc_h_fir_coef_ctrl);
		PE_SSC_H13_Wr01(ssc_h_fir_coef_ctrl,		rnw,	rnw_mode? 0x1:0x0);	//0:wr, 1:rd
		PE_SSC_H13_WrFL(ssc_h_fir_coef_ctrl);
	}
	else
	{
		PE_SHP_HW_H13_DBG_PRINT("nothing to do.\n");
	}
	return;
}
/**
 * read luma scaler filter coeff
 *
 * @param   win_id [in] LX_PE_WIN_ID
 * @param   config [in] UINT32
 * @param   *data [in/out] UINT32
 * @return  UINT32 offset
 * @see     
 * @author
 */
static UINT32 PE_SHP_HW_H13_RdLumaSclFltCoeff(LX_PE_WIN_ID win_id, UINT32 config, UINT32 *data)
{
	int ret = RET_OK;
	UINT32 i,offset;
	do{
		offset=0;
		CHECK_KNULL(data);
		if(PE_KDRV_VER_H13BX)
		{
			if(win_id == LX_PE_WIN_0)	//MSL
			{
				PE_MSL_H13B0_RdFL(msc_y_fir_coef_ctrl_l);
				PE_MSL_H13B0_Wr01(msc_y_fir_coef_ctrl_l, h_v_sel,		config);
				PE_MSL_H13B0_WrFL(msc_y_fir_coef_ctrl_l);
				for(i=0;i<16;i++)
				{
					/* set addr */
					data[offset] = i;	//coeff num 0~15
					PE_MSL_H13B0_Wr01(msc_y_fir_coef_ctrl_l, addr,		i);
					PE_MSL_H13B0_WrFL(msc_y_fir_coef_ctrl_l);
					/* set update */
					PE_MSL_H13B0_Wr01(msc_y_fir_coef_ctrl_l, update,		0x1);	//update and auto clear
					PE_MSL_H13B0_WrFL(msc_y_fir_coef_ctrl_l);
					/* read coeff */
					PE_MSL_H13B0_RdFL(msc_y_fir_coef_data0_l);
					PE_MSL_H13B0_RdFL(msc_y_fir_coef_data1_l);
					PE_MSL_H13B0_RdFL(msc_y_fir_coef_data2_l);
					PE_MSL_H13B0_RdFL(msc_y_fir_coef_data3_l);
					PE_MSL_H13B0_RdFL(msc_y_fir_coef_data4_l);
					PE_MSL_H13B0_RdFL(msc_y_fir_coef_data5_l);
					data[offset+1] = PE_MSL_H13B0_Rd(msc_y_fir_coef_data0_l);
					data[offset+2] = PE_MSL_H13B0_Rd(msc_y_fir_coef_data1_l);
					data[offset+3] = PE_MSL_H13B0_Rd(msc_y_fir_coef_data2_l);
					data[offset+4] = PE_MSL_H13B0_Rd(msc_y_fir_coef_data3_l);
					data[offset+5] = PE_MSL_H13B0_Rd(msc_y_fir_coef_data4_l);
					data[offset+6] = PE_MSL_H13B0_Rd(msc_y_fir_coef_data5_l);
					offset += 7;
				}
			}
			else	//LX_PE_WIN_1	//MSR
			{
				PE_MSR_H13B0_RdFL(msc_y_fir_coef_ctrl_r);
				PE_MSR_H13B0_Wr01(msc_y_fir_coef_ctrl_r, h_v_sel,		config);
				PE_MSR_H13B0_WrFL(msc_y_fir_coef_ctrl_r);
				for(i=0;i<16;i++)
				{
					/* set addr */
					data[offset] = i;	//coeff num 0~15
					PE_MSR_H13B0_Wr01(msc_y_fir_coef_ctrl_r, addr,		i);
					PE_MSR_H13B0_WrFL(msc_y_fir_coef_ctrl_r);
					/* set update */
					PE_MSR_H13B0_Wr01(msc_y_fir_coef_ctrl_r, update,		0x1);	//update and auto clear
					PE_MSR_H13B0_WrFL(msc_y_fir_coef_ctrl_r);
					/* read coeff */
					PE_MSR_H13B0_RdFL(msc_y_fir_coef_data0_r);
					PE_MSR_H13B0_RdFL(msc_y_fir_coef_data1_r);
					PE_MSR_H13B0_RdFL(msc_y_fir_coef_data2_r);
					PE_MSR_H13B0_RdFL(msc_y_fir_coef_data3_r);
					PE_MSR_H13B0_RdFL(msc_y_fir_coef_data4_r);
					PE_MSR_H13B0_RdFL(msc_y_fir_coef_data5_r);
					data[offset+1] = PE_MSR_H13B0_Rd(msc_y_fir_coef_data0_r);
					data[offset+2] = PE_MSR_H13B0_Rd(msc_y_fir_coef_data1_r);
					data[offset+3] = PE_MSR_H13B0_Rd(msc_y_fir_coef_data2_r);
					data[offset+4] = PE_MSR_H13B0_Rd(msc_y_fir_coef_data3_r);
					data[offset+5] = PE_MSR_H13B0_Rd(msc_y_fir_coef_data4_r);
					data[offset+6] = PE_MSR_H13B0_Rd(msc_y_fir_coef_data5_r);
					offset +=7;
				}
			}
		}
		else if(PE_KDRV_VER_H13AX)
		{
			if(win_id == LX_PE_WIN_0)	//MSL
			{
				PE_MSL_H13_RdFL(msc_y_fir_coef_ctrl_l);
				PE_MSL_H13_Wr01(msc_y_fir_coef_ctrl_l, h_v_sel,		config);
				PE_MSL_H13_WrFL(msc_y_fir_coef_ctrl_l);
				for(i=0;i<16;i++)
				{
					/* set addr */
					data[offset] = i;	//coeff num 0~15
					PE_MSL_H13_Wr01(msc_y_fir_coef_ctrl_l, addr,		i);
					PE_MSL_H13_WrFL(msc_y_fir_coef_ctrl_l);
					/* set update */
					PE_MSL_H13_Wr01(msc_y_fir_coef_ctrl_l, update,		0x1);	//update and auto clear
					PE_MSL_H13_WrFL(msc_y_fir_coef_ctrl_l);
					/* read coeff */
					PE_MSL_H13_RdFL(msc_y_fir_coef_data0_l);
					PE_MSL_H13_RdFL(msc_y_fir_coef_data1_l);
					PE_MSL_H13_RdFL(msc_y_fir_coef_data2_l);
					PE_MSL_H13_RdFL(msc_y_fir_coef_data3_l);
					PE_MSL_H13_RdFL(msc_y_fir_coef_data4_l);
					PE_MSL_H13_RdFL(msc_y_fir_coef_data5_l);
					data[offset+1] = PE_MSL_H13_Rd(msc_y_fir_coef_data0_l);
					data[offset+2] = PE_MSL_H13_Rd(msc_y_fir_coef_data1_l);
					data[offset+3] = PE_MSL_H13_Rd(msc_y_fir_coef_data2_l);
					data[offset+4] = PE_MSL_H13_Rd(msc_y_fir_coef_data3_l);
					data[offset+5] = PE_MSL_H13_Rd(msc_y_fir_coef_data4_l);
					data[offset+6] = PE_MSL_H13_Rd(msc_y_fir_coef_data5_l);
					offset += 7;
				}
			}
			else	//LX_PE_WIN_1	//MSR
			{
				PE_MSR_H13_RdFL(msc_y_fir_coef_ctrl_r);
				PE_MSR_H13_Wr01(msc_y_fir_coef_ctrl_r, h_v_sel,		config);
				PE_MSR_H13_WrFL(msc_y_fir_coef_ctrl_r);
				for(i=0;i<16;i++)
				{
					/* set addr */
					data[offset] = i;	//coeff num 0~15
					PE_MSR_H13_Wr01(msc_y_fir_coef_ctrl_r, addr,		i);
					PE_MSR_H13_WrFL(msc_y_fir_coef_ctrl_r);
					/* set update */
					PE_MSR_H13_Wr01(msc_y_fir_coef_ctrl_r, update,		0x1);	//update and auto clear
					PE_MSR_H13_WrFL(msc_y_fir_coef_ctrl_r);
					/* read coeff */
					PE_MSR_H13_RdFL(msc_y_fir_coef_data0_r);
					PE_MSR_H13_RdFL(msc_y_fir_coef_data1_r);
					PE_MSR_H13_RdFL(msc_y_fir_coef_data2_r);
					PE_MSR_H13_RdFL(msc_y_fir_coef_data3_r);
					PE_MSR_H13_RdFL(msc_y_fir_coef_data4_r);
					PE_MSR_H13_RdFL(msc_y_fir_coef_data5_r);
					data[offset+1] = PE_MSR_H13_Rd(msc_y_fir_coef_data0_r);
					data[offset+2] = PE_MSR_H13_Rd(msc_y_fir_coef_data1_r);
					data[offset+3] = PE_MSR_H13_Rd(msc_y_fir_coef_data2_r);
					data[offset+4] = PE_MSR_H13_Rd(msc_y_fir_coef_data3_r);
					data[offset+5] = PE_MSR_H13_Rd(msc_y_fir_coef_data4_r);
					data[offset+6] = PE_MSR_H13_Rd(msc_y_fir_coef_data5_r);
					offset +=7;
				}
			}
		}
		else
		{
			PE_SHP_HW_H13_DBG_PRINT("nothing to do.\n");
		}
	}while(0);
	return offset;
}
/**
 * read chroma scaler filter coeff
 *
 * @param   win_id [in] LX_PE_WIN_ID
 * @param   config [in] UINT32
 * @param   *data [in/out] UINT32
 * @return  UINT32 offset
 * @see     
 * @author
 */
static UINT32 PE_SHP_HW_H13_RdChromaSclFltCoeff(LX_PE_WIN_ID win_id, UINT32 config, UINT32 *data)
{
	int ret = RET_OK;
	UINT32 i,offset;
	do{
		offset=0;
		CHECK_KNULL(data);
		if(PE_KDRV_VER_H13BX)
		{
			if(win_id == LX_PE_WIN_0)	//MSL
			{
				PE_MSL_H13B0_RdFL(msc_c_fir_coef_ctrl_l);
				PE_MSL_H13B0_Wr01(msc_c_fir_coef_ctrl_l, h_v_sel,		config);
				PE_MSL_H13B0_WrFL(msc_c_fir_coef_ctrl_l);
				for(i=0;i<16;i++)
				{
					/* set addr */
					data[offset] = i;	//coeff num 0~15
					PE_MSL_H13B0_RdFL(msc_c_fir_coef_ctrl_l);
					PE_MSL_H13B0_Wr01(msc_c_fir_coef_ctrl_l, addr,		i);
					PE_MSL_H13B0_WrFL(msc_c_fir_coef_ctrl_l);
					/* set update */
					PE_MSL_H13B0_Wr01(msc_c_fir_coef_ctrl_l, update,	0x1);	//update and auto clear
					PE_MSL_H13B0_WrFL(msc_c_fir_coef_ctrl_l);
					/* read coeff */
					PE_MSL_H13B0_RdFL(msc_c_fir_coef_data0_l);
					PE_MSL_H13B0_RdFL(msc_c_fir_coef_data1_l);
					PE_MSL_H13B0_RdFL(msc_c_fir_coef_data2_l);
					PE_MSL_H13B0_RdFL(msc_c_fir_coef_data3_l);
					data[offset+1] = PE_MSL_H13B0_Rd(msc_c_fir_coef_data0_l);
					data[offset+2] = PE_MSL_H13B0_Rd(msc_c_fir_coef_data1_l);
					data[offset+3] = PE_MSL_H13B0_Rd(msc_c_fir_coef_data2_l);
					data[offset+4] = PE_MSL_H13B0_Rd(msc_c_fir_coef_data3_l);
					offset += 5;
				}
			}
			else	//LX_PE_WIN_1	//MSR
			{
				PE_MSR_H13B0_RdFL(msc_c_fir_coef_ctrl_r);
				PE_MSR_H13B0_Wr01(msc_c_fir_coef_ctrl_r, h_v_sel,		config);
				PE_MSR_H13B0_WrFL(msc_c_fir_coef_ctrl_r);
				for(i=0;i<16;i++)
				{
					/* set addr */
					data[offset] = i;	//coeff num 0~15
					PE_MSR_H13B0_RdFL(msc_c_fir_coef_ctrl_r);
					PE_MSR_H13B0_Wr01(msc_c_fir_coef_ctrl_r, addr,		i);
					PE_MSR_H13B0_WrFL(msc_c_fir_coef_ctrl_r);
					/* set update */
					PE_MSR_H13B0_Wr01(msc_c_fir_coef_ctrl_r, update,	0x1);	//update and auto clear
					PE_MSR_H13B0_WrFL(msc_c_fir_coef_ctrl_r);
					/* read coeff */
					PE_MSR_H13B0_RdFL(msc_c_fir_coef_data0_r);
					PE_MSR_H13B0_RdFL(msc_c_fir_coef_data1_r);
					PE_MSR_H13B0_RdFL(msc_c_fir_coef_data2_r);
					PE_MSR_H13B0_RdFL(msc_c_fir_coef_data3_r);
					data[offset+1] = PE_MSR_H13B0_Rd(msc_c_fir_coef_data0_r);
					data[offset+2] = PE_MSR_H13B0_Rd(msc_c_fir_coef_data1_r);
					data[offset+3] = PE_MSR_H13B0_Rd(msc_c_fir_coef_data2_r);
					data[offset+4] = PE_MSR_H13B0_Rd(msc_c_fir_coef_data3_r);
					offset += 5;
				}
			}
		}
		else if(PE_KDRV_VER_H13AX)
		{
			if(win_id == LX_PE_WIN_0)	//MSL
			{
				PE_MSL_H13_RdFL(msc_c_fir_coef_ctrl_l);
				PE_MSL_H13_Wr01(msc_c_fir_coef_ctrl_l, h_v_sel,		config);
				PE_MSL_H13_WrFL(msc_c_fir_coef_ctrl_l);
				for(i=0;i<16;i++)
				{
					/* set addr */
					data[offset] = i;	//coeff num 0~15
					PE_MSL_H13_RdFL(msc_c_fir_coef_ctrl_l);
					PE_MSL_H13_Wr01(msc_c_fir_coef_ctrl_l, addr,		i);
					PE_MSL_H13_WrFL(msc_c_fir_coef_ctrl_l);
					/* set update */
					PE_MSL_H13_Wr01(msc_c_fir_coef_ctrl_l, update,		0x1);	//update and auto clear
					PE_MSL_H13_WrFL(msc_c_fir_coef_ctrl_l);
					/* read coeff */
					PE_MSL_H13_RdFL(msc_c_fir_coef_data0_l);
					PE_MSL_H13_RdFL(msc_c_fir_coef_data1_l);
					PE_MSL_H13_RdFL(msc_c_fir_coef_data2_l);
					PE_MSL_H13_RdFL(msc_c_fir_coef_data3_l);
					data[offset+1] = PE_MSL_H13_Rd(msc_c_fir_coef_data0_l);
					data[offset+2] = PE_MSL_H13_Rd(msc_c_fir_coef_data1_l);
					data[offset+3] = PE_MSL_H13_Rd(msc_c_fir_coef_data2_l);
					data[offset+4] = PE_MSL_H13_Rd(msc_c_fir_coef_data3_l);
					offset += 5;
				}
			}
			else	//LX_PE_WIN_1	//MSR
			{
				PE_MSR_H13_RdFL(msc_c_fir_coef_ctrl_r);
				PE_MSR_H13_Wr01(msc_c_fir_coef_ctrl_r, h_v_sel,		config);
				PE_MSR_H13_WrFL(msc_c_fir_coef_ctrl_r);
				for(i=0;i<16;i++)
				{
					/* set addr */
					data[offset] = i;	//coeff num 0~15
					PE_MSR_H13_RdFL(msc_c_fir_coef_ctrl_r);
					PE_MSR_H13_Wr01(msc_c_fir_coef_ctrl_r, addr,		i);
					PE_MSR_H13_WrFL(msc_c_fir_coef_ctrl_r);
					/* set update */
					PE_MSR_H13_Wr01(msc_c_fir_coef_ctrl_r, update,		0x1);	//update and auto clear
					PE_MSR_H13_WrFL(msc_c_fir_coef_ctrl_r);
					/* read coeff */
					PE_MSR_H13_RdFL(msc_c_fir_coef_data0_r);
					PE_MSR_H13_RdFL(msc_c_fir_coef_data1_r);
					PE_MSR_H13_RdFL(msc_c_fir_coef_data2_r);
					PE_MSR_H13_RdFL(msc_c_fir_coef_data3_r);
					data[offset+1] = PE_MSR_H13_Rd(msc_c_fir_coef_data0_r);
					data[offset+2] = PE_MSR_H13_Rd(msc_c_fir_coef_data1_r);
					data[offset+3] = PE_MSR_H13_Rd(msc_c_fir_coef_data2_r);
					data[offset+4] = PE_MSR_H13_Rd(msc_c_fir_coef_data3_r);
					offset += 5;
				}
			}
		}
		else
		{
			PE_SHP_HW_H13_DBG_PRINT("nothing to do.\n");
		}
	}while(0);
	return offset;
}
/**
 * read sub scaler filter coeff
 *
 * @param   config [in] UINT32
 * @param   *data [in/out] UINT32
 * @return  OK if success, ERROR otherwise.
 * @see     
 * @author
 */
static UINT32 PE_SHP_HW_H13_RdSubSclFltCoeff(UINT32 config, UINT32 *data)
{
	int ret = RET_OK;
	UINT32 i,offset;
	do{
		offset=0;
		CHECK_KNULL(data);
		if(PE_KDRV_VER_H13)
		{
			PE_SSC_H13_RdFL(ssc_win_in_size);
			PE_SSC_H13_Wr01(ssc_win_in_size,	in_win_h_size,	0x0);
			PE_SSC_H13_Wr01(ssc_win_in_size,	in_win_v_size,	0x0);
			PE_SSC_H13_WrFL(ssc_win_in_size);
			
			PE_SSC_H13_RdFL(ssc_h_fir_coef_ctrl);
			PE_SSC_H13_Wr01(ssc_h_fir_coef_ctrl, h_v_sel,		config);
			PE_SSC_H13_WrFL(ssc_h_fir_coef_ctrl);
			for(i=0;i<16;i++)
			{
				/* set addr */
				data[offset] = i;	//coeff num 0~15
				PE_SSC_H13_Wr01(ssc_h_fir_coef_ctrl, addr,		i);
				PE_SSC_H13_WrFL(ssc_h_fir_coef_ctrl);
				/* set update */
				PE_SSC_H13_Wr01(ssc_h_fir_coef_ctrl, update,		0x1);	//update and auto clear
				PE_SSC_H13_WrFL(ssc_h_fir_coef_ctrl);
				/* read coeff */
				PE_SSC_H13_RdFL(ssc_h_fir_coef_data0);
				PE_SSC_H13_RdFL(ssc_h_fir_coef_data1);
				PE_SSC_H13_RdFL(ssc_h_fir_coef_data2);
				PE_SSC_H13_RdFL(ssc_h_fir_coef_data3);
				data[offset+1] = PE_SSC_H13_Rd(ssc_h_fir_coef_data0);
				data[offset+2] = PE_SSC_H13_Rd(ssc_h_fir_coef_data1);
				data[offset+3] = PE_SSC_H13_Rd(ssc_h_fir_coef_data2);
				data[offset+4] = PE_SSC_H13_Rd(ssc_h_fir_coef_data3);
				offset += 5;
			}
			PE_SSC_H13_RdFL(ssc_scr_size);
			PE_SSC_H13_RdFL(ssc_win_in_size);
			PE_SSC_H13_Wr(ssc_win_in_size,	PE_SSC_H13_Rd(ssc_scr_size));
			PE_SSC_H13_WrFL(ssc_win_in_size);
		}
	}while(0);
	return offset;
}
/**
 * read default setting
 *
 * @param   *pstParams [in/out] LX_PE_ETC_TBL_T
 * @return  OK if success, ERROR otherwise.
 * @see     
 * @author
 */
int PE_SHP_HW_H13_RdDefault(LX_PE_ETC_TBL_T *pstParams)
{
	int ret = RET_OK;
	char buffer[PE_TRACE_STR_SIZE];
	UINT32 param_size=0, table_size=0, is_valid=0;
	static UINT32 rd_cnt=0;
	PE_REG_PARAM_T param;

#define PE_SHP_H13_PRINT_START		"START_OF_PRINT"
#define PE_SHP_H13_PRINT_END		"END_OF_PRINT"
#define PE_SHP_H13_PRINT_EXIT		"EXIT_OF_PRINT"
#define PE_SHP_H13_CASE_PRMENU(_cnt, _action, _buf, _item)	\
	case (_cnt):sprintf(_buf, "   [0x%02x] %s\n", (_item), #_item);_action;break
#define PE_SHP_H13_CASE_SPRINT(_cnt, _action, _buf, fmt, args...)	\
	case (_cnt):sprintf(_buf, fmt, ##args);_action;break
#define PE_SHP_H13_DFLT_SPRINT(_action, _buf, fmt, args...)		\
	default:sprintf(_buf, fmt, ##args);_action;break

	do{
		CHECK_KNULL(pstParams);
		CHECK_KNULL(pstParams->data);
		if (PE_KDRV_VER_H13BX)
		{
			if (pstParams->data[0]>=PE_SHP_HW_H13_RD_DFLT_MAX)
			{
				sprintf(buffer, "%s", PE_SHP_H13_PRINT_EXIT);
				rd_cnt=0;
			}
			else if (pstParams->data[0]==PE_SHP_HW_H13_RD_DFLT_DISPLAY)
			{
				switch (rd_cnt)
				{
					PE_SHP_H13_CASE_SPRINT( 0, rd_cnt++, buffer, "%s", PE_SHP_H13_PRINT_START);
					PE_SHP_H13_CASE_PRMENU( 1, rd_cnt++, buffer, PE_SHP_HW_H13_RD_DFLT_SHP_L);
					PE_SHP_H13_CASE_PRMENU( 2, rd_cnt++, buffer, PE_SHP_HW_H13_RD_DFLT_SHP_R);
					PE_SHP_H13_CASE_PRMENU( 3, rd_cnt++, buffer, PE_SHP_HW_H13_RD_DFLT_VFC_L);
					PE_SHP_H13_CASE_PRMENU( 4, rd_cnt++, buffer, PE_SHP_HW_H13_RD_DFLT_VFC_R);
					PE_SHP_H13_DFLT_SPRINT(rd_cnt=0, buffer, "%s", PE_SHP_H13_PRINT_END);
				}
			}
			else
			{
				is_valid = 0;
				switch (pstParams->data[0])
				{
					case PE_SHP_HW_H13_RD_DFLT_SHP_L:
						table_size = sizeof(shp_l_pc_default_h13b0);
						param_size = table_size/sizeof(PE_REG_PARAM_T);
						if (rd_cnt<param_size)
						{
							param.addr = shp_l_pc_default_h13b0[rd_cnt].addr;
							is_valid = 1;
						}
						break;
					case PE_SHP_HW_H13_RD_DFLT_SHP_R:
						table_size = sizeof(shp_r_pc_default_h13b0);
						param_size = table_size/sizeof(PE_REG_PARAM_T);
						if (rd_cnt<param_size)
						{
							param.addr = shp_r_pc_default_h13b0[rd_cnt].addr;
							is_valid = 1;
						}
						break;
					case PE_SHP_HW_H13_RD_DFLT_VFC_L:
					case PE_SHP_HW_H13_RD_DFLT_VFC_R:
					default:
						table_size = sizeof(vflt_c_l_pc_default_h13b0);
						param_size = table_size/sizeof(PE_REG_PARAM_T);
						if (rd_cnt<param_size)
						{
							param.addr = vflt_c_l_pc_default_h13b0[rd_cnt].addr;
							if(pstParams->data[0]==PE_SHP_HW_H13_RD_DFLT_VFC_R)
								param.addr += PE_MSC_REG_H13_LRDIFF;
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
					sprintf(buffer, "%s", PE_SHP_H13_PRINT_END);
					rd_cnt = 0;
				}
			}
			memcpy(pstParams->data, buffer, sizeof(char)*PE_TRACE_STR_SIZE);
		}
	}while (0);
	return ret;
}

/**
 * set resolution common control
 * - use input struct LX_PE_SHP_RE2_CMN_T for H13BX
 * - use input struct LX_PE_SHP_RE1_CMN_T for H13AX
 *
 * @param   *pstParams [in] void
 * @return  OK if success, ERROR otherwise.
 * @see		function description
 * @author
 */
int PE_SHP_HW_H13_SetReCmnCtrl(void *pstParams)
{
	int ret = RET_OK;
	do {
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H13BX)
		{
			LX_PE_SHP_RE2_CMN_T *pp=(LX_PE_SHP_RE2_CMN_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			PE_SHP_HW_H13_DBG_PRINT("set[%d] : resolution common ctrl\n"\
				" mp w,b_gain:%d,%d,  sp w,b_gain:%d,%d\n"\
				" cti   : size:%d, g:%d,%d,%d\n",\
				pp->win_id,pp->mp_white_gain,pp->mp_black_gain,\
				pp->sp_white_gain,pp->sp_black_gain,\
				pp->tap_size,pp->cti_gain,pp->ycm_y_gain,pp->ycm_c_gain);
			if(PE_CHECK_WIN0(pp->win_id))
			{
				PE_P1L_H13B0_QWr02(pe1_mp_ctrl_01,	white_gain,			GET_BITS(pp->mp_white_gain,0,7),\
													black_gain,			GET_BITS(pp->mp_black_gain,0,7));
				PE_P1L_H13B0_QWr02(pe1_sp_ctrl_01,	white_gain,			GET_BITS(pp->sp_white_gain,0,7),\
													black_gain,			GET_BITS(pp->sp_black_gain,0,7));				
				/* cti */
				PE_P1L_H13B0_QWr02(pe1_cti_ctrl_00, filter_tap_size,	GET_BITS(pp->tap_size,0,3),\
													cti_gain,			GET_BITS(pp->cti_gain,0,8));
				PE_P1L_H13B0_QWr02(pe1_cti_ctrl_02, ycm_y_gain,			GET_BITS(pp->ycm_y_gain,0,4),\
													ycm_c_gain,			GET_BITS(pp->ycm_c_gain,0,4));
			}
		}
		else if(PE_KDRV_VER_H13AX)
		{
			LX_PE_SHP_RE1_CMN_T *pp=(LX_PE_SHP_RE1_CMN_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			PE_SHP_HW_H13_DBG_PRINT("set[%d] : resolution common ctrl\n"\
				" mp w,b_gain:%d,%d,  sp w,b_gain:%d,%d\n"\
				" cti   : en:%d, size:%d, g:%d,%d,%d\n",\
				pp->win_id,pp->mp_white_gain,pp->mp_black_gain,\
				pp->sp_white_gain,pp->sp_black_gain,\
				pp->cti_en,pp->tap_size,pp->cti_gain,pp->ycm_y_gain,pp->ycm_c_gain);
			if(PE_CHECK_WIN0(pp->win_id))
			{
				PE_P1L_H13_RdFL(pe1_mp_ctrl_01);
				PE_P1L_H13_RdFL(pe1_sp_ctrl_01);
				PE_P1L_H13_RdFL(pe1_cti_ctrl_00);
				PE_P1L_H13_RdFL(pe1_cti_ctrl_02);
				PE_P1L_H13_Wr01(pe1_mp_ctrl_01, white_gain,			GET_BITS(pp->mp_white_gain,0,7));
				PE_P1L_H13_Wr01(pe1_mp_ctrl_01, black_gain,			GET_BITS(pp->mp_black_gain,0,7));
				PE_P1L_H13_Wr01(pe1_sp_ctrl_01, white_gain,			GET_BITS(pp->sp_white_gain,0,7));
				PE_P1L_H13_Wr01(pe1_sp_ctrl_01, black_gain,			GET_BITS(pp->sp_black_gain,0,7));
				PE_P1L_H13_Wr01(pe1_cti_ctrl_00, cti_en,			GET_BITS(pp->cti_en,0,1));
				PE_P1L_H13_Wr01(pe1_cti_ctrl_00, tap_size,			GET_BITS(pp->tap_size,0,3));
				PE_P1L_H13_Wr01(pe1_cti_ctrl_00, cti_gain,			GET_BITS(pp->cti_gain,0,8));
				PE_P1L_H13_Wr01(pe1_cti_ctrl_02, ycm_y_gain,		GET_BITS(pp->ycm_y_gain,0,4));
				PE_P1L_H13_Wr01(pe1_cti_ctrl_02, ycm_c_gain,		GET_BITS(pp->ycm_c_gain,0,4));
				PE_P1L_H13_WrFL(pe1_mp_ctrl_01);
				PE_P1L_H13_WrFL(pe1_sp_ctrl_01);
				PE_P1L_H13_WrFL(pe1_cti_ctrl_00);
				PE_P1L_H13_WrFL(pe1_cti_ctrl_02);
			}
		}
		else
		{
			PE_SHP_HW_H13_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	} while (0);
	return ret;
}
/**
 * get resolution common control
 * - use input struct LX_PE_SHP_RE2_CMN_T for H13BX
 * - use input struct LX_PE_SHP_RE1_CMN_T for H13AX
 *
 * @param   *pstParams [in/out] void
 * @return  OK if success, ERROR otherwise.
 * @see		function description
 * @author
 */
int PE_SHP_HW_H13_GetReCmnCtrl(void *pstParams)
{
	int ret = RET_OK;
	LX_PE_WIN_ID win_id;
	do {
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H13BX)
		{
			LX_PE_SHP_RE2_CMN_T *pp=(LX_PE_SHP_RE2_CMN_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			win_id = PE_GET_CHECKED_WINID(pp->win_id);
			if(PE_CHECK_WIN0(win_id))
			{
				PE_P1L_H13B0_QRd02(pe1_mp_ctrl_01,	white_gain,			pp->mp_white_gain,\
													black_gain,			pp->mp_black_gain);
				PE_P1L_H13B0_QRd02(pe1_sp_ctrl_01,	white_gain,			pp->sp_white_gain,\
													black_gain,			pp->sp_black_gain);				
				/* cti */
				PE_P1L_H13B0_QRd02(pe1_cti_ctrl_00, filter_tap_size,	pp->tap_size,\
													cti_gain,			pp->cti_gain);
				PE_P1L_H13B0_QRd02(pe1_cti_ctrl_02, ycm_y_gain,			pp->ycm_y_gain,\
													ycm_c_gain,			pp->ycm_c_gain);
			}
			PE_SHP_HW_H13_DBG_PRINT("get[%d] : resolution common ctrl\n"\
				" mp w,b_gain:%d,%d,  sp w,b_gain:%d,%d\n"\
				" cti   :size:%d, g:%d,%d,%d\n",\
				pp->win_id,pp->mp_white_gain,pp->mp_black_gain,\
				pp->sp_white_gain,pp->sp_black_gain,\
				pp->tap_size,pp->cti_gain,pp->ycm_y_gain,pp->ycm_c_gain);
		}
		else if(PE_KDRV_VER_H13AX)
		{
			LX_PE_SHP_RE1_CMN_T *pp=(LX_PE_SHP_RE1_CMN_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			win_id = PE_GET_CHECKED_WINID(pp->win_id);
			if(PE_CHECK_WIN0(win_id))
			{
				PE_P1L_H13_RdFL(pe1_mp_ctrl_01);
				PE_P1L_H13_RdFL(pe1_sp_ctrl_01);
				PE_P1L_H13_RdFL(pe1_cti_ctrl_00);
				PE_P1L_H13_RdFL(pe1_cti_ctrl_02);
				PE_P1L_H13_Rd01(pe1_mp_ctrl_01, white_gain,			pp->mp_white_gain);
				PE_P1L_H13_Rd01(pe1_mp_ctrl_01, black_gain,			pp->mp_black_gain);
				PE_P1L_H13_Rd01(pe1_sp_ctrl_01, white_gain,			pp->sp_white_gain);
				PE_P1L_H13_Rd01(pe1_sp_ctrl_01, black_gain,			pp->sp_black_gain);
				PE_P1L_H13_Rd01(pe1_cti_ctrl_00, cti_en,			pp->cti_en);
				PE_P1L_H13_Rd01(pe1_cti_ctrl_00, tap_size,			pp->tap_size);
				PE_P1L_H13_Rd01(pe1_cti_ctrl_00, cti_gain,			pp->cti_gain);
				PE_P1L_H13_Rd01(pe1_cti_ctrl_02, ycm_y_gain,		pp->ycm_y_gain);
				PE_P1L_H13_Rd01(pe1_cti_ctrl_02, ycm_c_gain,		pp->ycm_c_gain);
			}
			PE_SHP_HW_H13_DBG_PRINT("get[%d] : resolution common ctrl\n"\
				" mp w,b_gain:%d,%d,  sp w,b_gain:%d,%d\n"\
				" cti   : en:%d, size:%d, g:%d,%d,%d\n",\
				pp->win_id,pp->mp_white_gain,pp->mp_black_gain,\
				pp->sp_white_gain,pp->sp_black_gain,\
				pp->cti_en,pp->tap_size,pp->cti_gain,pp->ycm_y_gain,pp->ycm_c_gain);
		}
		else
		{
			PE_SHP_HW_H13_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * set resolution horizontal control
 * - use input struct LX_PE_SHP_RE2_HOR_T for H13BX
 * - use input struct LX_PE_SHP_RE1_HOR_T for H13AX
 *
 * @param   *pstParams [in/out] void
 * @return  OK if success, ERROR otherwise.
 * @see		function description
 * @author
 */
int PE_SHP_HW_H13_SetReHorCtrl(void *pstParams)
{
	int ret = RET_OK;
	do {
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H13BX)
		{
			LX_PE_SHP_RE2_HOR_T *pp=(LX_PE_SHP_RE2_HOR_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			PE_SHP_HW_H13_DBG_PRINT("set[%d] : resolution horizontal ctrl\n"\
				"g:%d,%d,%d, a_gen_w:%d, mp,sp_gain:%d,%d\n"\
				"e_gain(1,2):%d,%d, f_gain(1,2):%d,%d\n"
				"coring_th:%d, y_gain:%d, c_gain:%d\n",\
				pp->win_id,pp->reg_csft_gain,\
				pp->edge_filter_white_gain,pp->edge_filter_black_gain,\
				pp->a_gen_width,pp->mp_horizontal_gain,pp->sp_horizontal_gain,\
				pp->e_gain_th1, pp->e_gain_th2,pp->f_gain_th1, pp->f_gain_th2,\
				pp->coring_th, pp->y_gain, pp->c_gain);
			if(PE_CHECK_WIN0(pp->win_id))
			{
				PE_FWI_H13_HOR_CTRL fw_param;
				#ifdef PE_FWI_H13_USE_SHP_CTRL_WHEN_PQFW_OFF
				fw_param.e_gain_th1 = GET_BITS(pp->e_gain_th1,0,8);
				fw_param.e_gain_th2 = GET_BITS(pp->e_gain_th2,0,8);
				ret = PE_FWI_H13_SetShpHorCtrl(&fw_param);
				PE_SHP_HW_H13_CHECK_CODE(ret,break,"[%s,%d] PE_FWI_H13_SetShpHorCtrl() error.\n",__F__,__L__);
				#else
				PE_FWI_H13_SETTINGS_T fw_inf;
				ret = PE_FWI_H13_GetCurFwiSettings(&fw_inf);
				PE_SHP_HW_H13_CHECK_CODE(ret,break,"[%s,%d] PE_FWI_H13_GetCurFwiSettings() error.\n",__F__,__L__);
				PE_SHP_HW_H13_DBG_PRINT("fw_ctrl.enable : %s.\n",fw_inf.fw_ctrl.enable? "yes":"no");
				if(fw_inf.fw_ctrl.enable)
				{
					fw_param.e_gain_th1 = GET_BITS(pp->e_gain_th1,0,8);
					fw_param.e_gain_th2 = GET_BITS(pp->e_gain_th2,0,8);
					ret = PE_FWI_H13_SetShpHorCtrl(&fw_param);
					PE_SHP_HW_H13_CHECK_CODE(ret,break,"[%s,%d] PE_FWI_H13_SetShpHorCtrl() error.\n",__F__,__L__);
				}
				else
				{
					PE_P1L_H13B0_QWr02(pe1_derh_ctrl_0b, e_gain_th1,		GET_BITS(pp->e_gain_th1,0,8),\
														 e_gain_th2,		GET_BITS(pp->e_gain_th2,0,8));
				}
				#endif
				PE_P1L_H13B0_QWr01(pe1_derh_ctrl_00, a_gen_width,			GET_BITS(pp->a_gen_width,0,4));
				PE_P1L_H13B0_QWr01(pe1_derh_ctrl_05, reg_csft_gain,			GET_BITS(pp->reg_csft_gain,0,6));
				PE_P1L_H13B0_QWr02(pe1_derh_ctrl_06, edge_filter_white_gain,	GET_BITS(pp->edge_filter_white_gain,0,6),\
													 edge_filter_black_gain,	GET_BITS(pp->edge_filter_black_gain,0,6));
				PE_P1L_H13B0_QWr01(pe1_mp_ctrl_01, 	horizontal_gain,			GET_BITS(pp->mp_horizontal_gain,0,8));
				PE_P1L_H13B0_QWr01(pe1_sp_ctrl_01, 	horizontal_gain,			GET_BITS(pp->sp_horizontal_gain,0,8));
				PE_P1L_H13B0_QWr02(pe1_derh_ctrl_0b, f_gain_th1,			GET_BITS(pp->f_gain_th1,0,8),\
													 f_gain_th2,			GET_BITS(pp->f_gain_th2,0,8));
				PE_P1L_H13B0_QWr03(pe1_ti_ctrl_0, 	coring_th,				GET_BITS(pp->coring_th,0,8),\
												  	y_gain,					GET_BITS(pp->y_gain,0,8),\
												  	c_gain,					GET_BITS(pp->c_gain,0,8));
			}
		}
		else if(PE_KDRV_VER_H13AX)
		{
			LX_PE_SHP_RE1_HOR_T *pp=(LX_PE_SHP_RE1_HOR_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			PE_SHP_HW_H13_DBG_PRINT("set[%d] : resolution horizontal ctrl\n"\
				" en:%d, g:%d,%d,%d, a_gen_w:%d, mp,sp_gain:%d,%d\n",\
				pp->win_id,pp->edge_y_filter_en,pp->reg_csft_gain,\
				pp->edge_filter_white_gain,pp->edge_filter_black_gain,\
				pp->a_gen_width,pp->mp_horizontal_gain,pp->sp_horizontal_gain);
			if(PE_CHECK_WIN0(pp->win_id))
			{
				PE_P1L_H13_RdFL(pe1_derh_ctrl_00);
				PE_P1L_H13_RdFL(pe1_derh_ctrl_05);
				PE_P1L_H13_RdFL(pe1_derh_ctrl_06);
				PE_P1L_H13_RdFL(pe1_mp_ctrl_01);
				PE_P1L_H13_RdFL(pe1_sp_ctrl_01);
				PE_P1L_H13_Wr01(pe1_derh_ctrl_00, a_gen_width,				GET_BITS(pp->a_gen_width,0,4));
				PE_P1L_H13_Wr01(pe1_derh_ctrl_05, reg_csft_gain,			GET_BITS(pp->reg_csft_gain,0,6));
				PE_P1L_H13_Wr01(pe1_derh_ctrl_06, edge_y_filter_en,			GET_BITS(pp->edge_y_filter_en,0,1));
				PE_P1L_H13_Wr01(pe1_derh_ctrl_06, edge_filter_white_gain,	GET_BITS(pp->edge_filter_white_gain,0,6));
				PE_P1L_H13_Wr01(pe1_derh_ctrl_06, edge_filter_black_gain,	GET_BITS(pp->edge_filter_black_gain,0,6));
				PE_P1L_H13_Wr01(pe1_mp_ctrl_01, horizontal_gain,			GET_BITS(pp->mp_horizontal_gain,0,8));
				PE_P1L_H13_Wr01(pe1_sp_ctrl_01, horizontal_gain,			GET_BITS(pp->sp_horizontal_gain,0,8));
				PE_P1L_H13_WrFL(pe1_derh_ctrl_00);
				PE_P1L_H13_WrFL(pe1_derh_ctrl_05);
				PE_P1L_H13_WrFL(pe1_derh_ctrl_06);
				PE_P1L_H13_WrFL(pe1_mp_ctrl_01);
				PE_P1L_H13_WrFL(pe1_sp_ctrl_01);
			}
		}
		else
		{
			PE_SHP_HW_H13_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	} while (0);
	return ret;
}
/**
 * get resolution horizontal control
 * - use input struct LX_PE_SHP_RE2_HOR_T for H13BX
 * - use input struct LX_PE_SHP_RE1_HOR_T for H13AX
 *
 * @param   *pstParams [in/out] void
 * @return  OK if success, ERROR otherwise.
 * @see		function description
 * @author
 */
int PE_SHP_HW_H13_GetReHorCtrl(void *pstParams)
{
	int ret = RET_OK;
	LX_PE_WIN_ID win_id;
	do {
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H13BX)
		{
			LX_PE_SHP_RE2_HOR_T *pp=(LX_PE_SHP_RE2_HOR_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			win_id = PE_GET_CHECKED_WINID(pp->win_id);
			if(PE_CHECK_WIN0(win_id))
			{
				#ifdef PE_FWI_H13_USE_SHP_CTRL_WHEN_PQFW_OFF
				PE_FWI_H13_SETTINGS_T fw_inf;
				ret = PE_FWI_H13_GetCurFwiSettings(&fw_inf);
				PE_SHP_HW_H13_CHECK_CODE(ret,break,"[%s,%d] PE_FWI_H13_GetCurFwiSettings() error.\n",__F__,__L__);
				PE_SHP_HW_H13_DBG_PRINT("fw_ctrl.enable : %s.\n",fw_inf.fw_ctrl.enable? "yes":"no");
				if(fw_inf.fw_ctrl.enable)
				{
					pp->e_gain_th1 = fw_inf.shp_h_ctrl.e_gain_th1;
					pp->e_gain_th2 = fw_inf.shp_h_ctrl.e_gain_th2;
				}
				else
				#endif
				{
					PE_P1L_H13B0_QRd02(pe1_derh_ctrl_0b, e_gain_th1,		pp->e_gain_th1,\
														 e_gain_th2,		pp->e_gain_th2);
				}
				PE_P1L_H13B0_QRd01(pe1_derh_ctrl_00, a_gen_width,			pp->a_gen_width);
				PE_P1L_H13B0_QRd01(pe1_derh_ctrl_05, reg_csft_gain,			pp->reg_csft_gain);
				PE_P1L_H13B0_QRd02(pe1_derh_ctrl_06, edge_filter_white_gain,	pp->edge_filter_white_gain,\
													 edge_filter_black_gain,	pp->edge_filter_black_gain);
				PE_P1L_H13B0_QRd01(pe1_mp_ctrl_01, 	 horizontal_gain,			pp->mp_horizontal_gain);
				PE_P1L_H13B0_QRd01(pe1_sp_ctrl_01, 	 horizontal_gain,			pp->sp_horizontal_gain);
				PE_P1L_H13B0_QRd02(pe1_derh_ctrl_0b, f_gain_th1,			pp->f_gain_th1,\
													 f_gain_th2,			pp->f_gain_th2);
				PE_P1L_H13B0_QRd03(pe1_ti_ctrl_0, 	 coring_th,				pp->coring_th,\
												  	 y_gain,					pp->y_gain,\
												  	 c_gain,					pp->c_gain);
			}
			PE_SHP_HW_H13_DBG_PRINT("get[%d] : resolution horizontal ctrl\n"\
				" g:%d,%d,%d, a_gen_w:%d, mp,sp_gain:%d,%d\n"\
				"e_gain(1,2):%d,%d, f_gain(1,2):%d,%d\n"
				"coring_th:%d, y_gain:%d, c_gain:%d\n",\
				pp->win_id,pp->reg_csft_gain,\
				pp->edge_filter_white_gain,pp->edge_filter_black_gain,\
				pp->a_gen_width,pp->mp_horizontal_gain,pp->sp_horizontal_gain,\
				pp->e_gain_th1, pp->e_gain_th2,pp->f_gain_th1, pp->f_gain_th2,\
				pp->coring_th, pp->y_gain, pp->c_gain);
		}
		else if(PE_KDRV_VER_H13AX)
		{
			LX_PE_SHP_RE1_HOR_T *pp=(LX_PE_SHP_RE1_HOR_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			win_id = PE_GET_CHECKED_WINID(pp->win_id);
			if(PE_CHECK_WIN0(win_id))
			{
				PE_P1L_H13_RdFL(pe1_derh_ctrl_00);
				PE_P1L_H13_RdFL(pe1_derh_ctrl_05);
				PE_P1L_H13_RdFL(pe1_derh_ctrl_06);
				PE_P1L_H13_RdFL(pe1_mp_ctrl_01);
				PE_P1L_H13_RdFL(pe1_sp_ctrl_01);
				PE_P1L_H13_Rd01(pe1_derh_ctrl_00, a_gen_width,				pp->a_gen_width);
				PE_P1L_H13_Rd01(pe1_derh_ctrl_05, reg_csft_gain,			pp->reg_csft_gain);
				PE_P1L_H13_Rd01(pe1_derh_ctrl_06, edge_y_filter_en,			pp->edge_y_filter_en);
				PE_P1L_H13_Rd01(pe1_derh_ctrl_06, edge_filter_white_gain,	pp->edge_filter_white_gain);
				PE_P1L_H13_Rd01(pe1_derh_ctrl_06, edge_filter_black_gain,	pp->edge_filter_black_gain);
				PE_P1L_H13_Rd01(pe1_mp_ctrl_01, horizontal_gain,			pp->mp_horizontal_gain);
				PE_P1L_H13_Rd01(pe1_sp_ctrl_01, horizontal_gain,			pp->sp_horizontal_gain);
			}
			PE_SHP_HW_H13_DBG_PRINT("get[%d] : resolution horizontal ctrl\n"\
				" en:%d, g:%d,%d,%d, a_gen_w:%d, mp,sp_gain:%d,%d\n",\
				pp->win_id,pp->edge_y_filter_en,pp->reg_csft_gain,\
				pp->edge_filter_white_gain,pp->edge_filter_black_gain,\
				pp->a_gen_width,pp->mp_horizontal_gain,pp->sp_horizontal_gain);
		}
		else
		{
			PE_SHP_HW_H13_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * set resolution vertical control
 * - use input struct LX_PE_SHP_RE2_VER_T for H13BX
 * - use input struct LX_PE_SHP_RE1_VER_T for H13AX
 *
 * @param   *pstParams [in] void
 * @return  OK if success, ERROR otherwise.
 * @see		function description
 * @author
 */
int PE_SHP_HW_H13_SetReVerCtrl(void *pstParams)
{
	int ret = RET_OK;
	do {
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H13BX)
		{
			LX_PE_SHP_RE2_VER_T *pp=(LX_PE_SHP_RE2_VER_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			PE_SHP_HW_H13_DBG_PRINT("set[%d] : resolution vertical ctrl\n"\
				" manual_th:%d,csft_gain:%d, gain(b,w)%d,%d,\n"\
				"mmd_sel:%d, gain(th1,th2):%d,%d mp,sp_g:%d,%d\n",\
				pp->win_id,pp->bif_manual_th,pp->csft_gain,\
				pp->gain_b,pp->gain_w,pp->mmd_sel,pp->gain_th1,pp->gain_th2,\
				pp->mp_vertical_gain,pp->sp_vertical_gain);
			if(PE_CHECK_WIN0(pp->win_id))
			{
				PE_P1L_H13B0_QWr01(pe1_derv_ctrl_0, bif_manual_th,	GET_BITS(pp->bif_manual_th,0,8));
				PE_P1L_H13B0_QWr01(pe1_derv_ctrl_1, csft_gain,		GET_BITS(pp->csft_gain,0,6));
				PE_P1L_H13B0_QWr02(pe1_derv_ctrl_3, gain_b,			GET_BITS(pp->gain_b,0,6),\
													gain_w,			GET_BITS(pp->gain_w,0,6));
				PE_P1L_H13B0_QWr03(pe1_derv_ctrl_2, mmd_sel,		GET_BITS(pp->mmd_sel,0,3),\
													gain_th1,		GET_BITS(pp->gain_th1,0,8),\
													gain_th2,		GET_BITS(pp->gain_th2,0,8));
				PE_P1L_H13B0_QWr01(pe1_mp_ctrl_01, 	vertical_gain,	GET_BITS(pp->mp_vertical_gain,0,8));
				PE_P1L_H13B0_QWr01(pe1_sp_ctrl_01, 	vertical_gain,	GET_BITS(pp->sp_vertical_gain,0,8));
			}
		
		}
		else if(PE_KDRV_VER_H13AX)
		{
			LX_PE_SHP_RE1_VER_T *pp=(LX_PE_SHP_RE1_VER_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			PE_SHP_HW_H13_DBG_PRINT("set[%d] : resolution vertical ctrl\n"\
				" en:%d, th:%d, g:%d,%d,%d, der_g:%d, mmd_sel:%d, mp,sp_g:%d,%d\n",\
				pp->win_id,pp->der_v_en,pp->bif_manual_th,pp->csft_gain,\
				pp->gain_b,pp->gain_w,pp->der_gain_mapping,pp->mmd_sel,\
				pp->mp_vertical_gain,pp->sp_vertical_gain);
			if(PE_CHECK_WIN0(pp->win_id))
			{
				PE_P1L_H13_RdFL(pe1_derv_ctrl_0);
				PE_P1L_H13_RdFL(pe1_derv_ctrl_1);
				PE_P1L_H13_RdFL(pe1_derv_ctrl_2);
				PE_P1L_H13_RdFL(pe1_derv_ctrl_3);
				PE_P1L_H13_RdFL(pe1_mp_ctrl_01);
				PE_P1L_H13_RdFL(pe1_sp_ctrl_01);
				PE_P1L_H13_Wr01(pe1_derv_ctrl_0, der_v_en,			GET_BITS(pp->der_v_en,0,1));
				PE_P1L_H13_Wr01(pe1_derv_ctrl_0, bif_manual_th,		GET_BITS(pp->bif_manual_th,0,8));
				PE_P1L_H13_Wr01(pe1_derv_ctrl_0, der_gain_mapping,	GET_BITS(pp->der_gain_mapping,0,2));
				PE_P1L_H13_Wr01(pe1_derv_ctrl_1, csft_gain,			GET_BITS(pp->csft_gain,0,6));
				PE_P1L_H13_Wr01(pe1_derv_ctrl_2, mmd_sel,			GET_BITS(pp->mmd_sel,0,3));
				PE_P1L_H13_Wr01(pe1_derv_ctrl_3, gain_b,			GET_BITS(pp->gain_b,0,7));
				PE_P1L_H13_Wr01(pe1_derv_ctrl_3, gain_w,			GET_BITS(pp->gain_w,0,7));
				PE_P1L_H13_Wr01(pe1_mp_ctrl_01, vertical_gain,		GET_BITS(pp->mp_vertical_gain,0,8));
				PE_P1L_H13_Wr01(pe1_sp_ctrl_01, vertical_gain,		GET_BITS(pp->sp_vertical_gain,0,8));
				PE_P1L_H13_WrFL(pe1_derv_ctrl_0);
				PE_P1L_H13_WrFL(pe1_derv_ctrl_1);
				PE_P1L_H13_WrFL(pe1_derv_ctrl_2);
				PE_P1L_H13_WrFL(pe1_derv_ctrl_3);
				PE_P1L_H13_WrFL(pe1_mp_ctrl_01);
				PE_P1L_H13_WrFL(pe1_sp_ctrl_01);
			}
		}
		else
		{
			PE_SHP_HW_H13_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	} while (0);
	return ret;
}
/**
 * get resolution vertical control
 * - use input struct LX_PE_SHP_RE2_VER_T for H13BX
 * - use input struct LX_PE_SHP_RE1_VER_T for H13AX
 *
 * @param   *pstParams [in/out] void
 * @return  OK if success, ERROR otherwise.
 * @see		function description
 * @author
 */
int PE_SHP_HW_H13_GetReVerCtrl(void *pstParams)
{
	int ret = RET_OK;
	LX_PE_WIN_ID win_id;
	do {
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H13BX)
		{
			LX_PE_SHP_RE2_VER_T *pp=(LX_PE_SHP_RE2_VER_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			win_id = PE_GET_CHECKED_WINID(pp->win_id);
			if(PE_CHECK_WIN0(pp->win_id))
			{
				PE_P1L_H13B0_QRd01(pe1_derv_ctrl_0, bif_manual_th,	pp->bif_manual_th);
				PE_P1L_H13B0_QRd01(pe1_derv_ctrl_1, csft_gain,		pp->csft_gain);
				PE_P1L_H13B0_QRd02(pe1_derv_ctrl_3, gain_b,			pp->gain_b,\
													gain_w,			pp->gain_w);
				PE_P1L_H13B0_QRd03(pe1_derv_ctrl_2, mmd_sel,		pp->mmd_sel,\
													gain_th1,		pp->gain_th1,\
													gain_th2,		pp->gain_th2);
				PE_P1L_H13B0_QRd01(pe1_mp_ctrl_01, 	vertical_gain,	pp->mp_vertical_gain);
				PE_P1L_H13B0_QRd01(pe1_sp_ctrl_01, 	vertical_gain,	pp->sp_vertical_gain);
			}
			PE_SHP_HW_H13_DBG_PRINT("get[%d] : resolution vertical ctrl\n"\
				" manual_th:%d,csft_gain:%d, gain(b,w)%d,%d,\n"\
				"mmd_sel:%d, gain(th1,th2):%d,%d mp,sp_g:%d,%d\n",\
				pp->win_id,pp->bif_manual_th,pp->csft_gain,\
				pp->gain_b,pp->gain_w,pp->mmd_sel,pp->gain_th1,pp->gain_th2,\
				pp->mp_vertical_gain,pp->sp_vertical_gain);
		}
		if(PE_KDRV_VER_H13AX)
		{
			LX_PE_SHP_RE1_VER_T *pp=(LX_PE_SHP_RE1_VER_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			win_id = PE_GET_CHECKED_WINID(pp->win_id);
			if(PE_CHECK_WIN0(win_id))
			{
				PE_P1L_H13_RdFL(pe1_derv_ctrl_0);
				PE_P1L_H13_RdFL(pe1_derv_ctrl_1);
				PE_P1L_H13_RdFL(pe1_derv_ctrl_2);
				PE_P1L_H13_RdFL(pe1_derv_ctrl_3);
				PE_P1L_H13_RdFL(pe1_mp_ctrl_01);
				PE_P1L_H13_RdFL(pe1_sp_ctrl_01);
				PE_P1L_H13_Rd01(pe1_derv_ctrl_0, der_v_en,			pp->der_v_en);
				PE_P1L_H13_Rd01(pe1_derv_ctrl_0, bif_manual_th,		pp->bif_manual_th);
				PE_P1L_H13_Rd01(pe1_derv_ctrl_0, der_gain_mapping,	pp->der_gain_mapping);
				PE_P1L_H13_Rd01(pe1_derv_ctrl_1, csft_gain,			pp->csft_gain);
				PE_P1L_H13_Rd01(pe1_derv_ctrl_2, mmd_sel,			pp->mmd_sel);
				PE_P1L_H13_Rd01(pe1_derv_ctrl_3, gain_b,			pp->gain_b);
				PE_P1L_H13_Rd01(pe1_derv_ctrl_3, gain_w,			pp->gain_w);
				PE_P1L_H13_Rd01(pe1_mp_ctrl_01, vertical_gain,		pp->mp_vertical_gain);
				PE_P1L_H13_Rd01(pe1_sp_ctrl_01, vertical_gain,		pp->sp_vertical_gain);
			}
			PE_SHP_HW_H13_DBG_PRINT("get[%d] : resolution vertical ctrl\n"\
				" en:%d, th:%d, g:%d,%d,%d, der_g:%d, mmd_sel:%d, mp,sp_g:%d,%d\n",\
				pp->win_id,pp->der_v_en,pp->bif_manual_th,pp->csft_gain,\
				pp->gain_b,pp->gain_w,pp->der_gain_mapping,pp->mmd_sel,\
				pp->mp_vertical_gain,pp->sp_vertical_gain);
		}
		else
		{
			PE_SHP_HW_H13_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * set resolution misc control
 * - use input struct LX_PE_SHP_RE2_MISC_T for H13BX
 * - use input struct LX_PE_SHP_RE1_MISC_T for H13AX
 *
 * @param   *pstParams [in] void
 * @return  OK if success, ERROR otherwise.
 * @see		function description
 * @author
 */
int PE_SHP_HW_H13_SetReMiscCtrl(void *pstParams)
{
	int ret = RET_OK;
	#ifdef PE_SHP_HW_H13_USE_SHP_MODE
	PE_SHP_HW_H13_SETTINGS_T *pInfo = &_g_pe_shp_hw_h13_info;
	#endif
	do {
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H13BX)
		{
			LX_PE_SHP_RE2_MISC_T *pp=(LX_PE_SHP_RE2_MISC_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			PE_SHP_HW_H13_DBG_PRINT("set[%d] :  resolution misc ctrl\n"\
				" e-map : sel:%d, ga:%d,%d,%d\n"\
				" t-map : sel:%d,%d, g:%d, gt:%d,%d,%d,%d,%d,%d,%d, cor1:%d,var_th:%d\n"\
				" d-jag : en:%d, th:%d,%d, g:%d\n"\
				"         h_cnt:min:%d,max:%d, v_cnt:min:%d,max:%d\n",pp->win_id,\
				pp->amap2_sel,pp->ga_max,pp->ga_th0,pp->ga_th1,pp->amap1_sel,\
				pp->max_sel,pp->tmap_gain,pp->gt_th0,pp->gt_th0a,pp->gt_th0b,\
				pp->gt_th1,pp->gt_gain0a,pp->gt_gain0b,pp->gt_max,pp->coring_mode1,pp->var_th,\
				pp->center_blur_en,pp->level_th,pp->protect_th,pp->n_avg_gain,\
				pp->dj_h_count_min,pp->dj_h_count_max,pp->dj_v_count_min,pp->dj_v_count_max);
			if(PE_CHECK_WIN0(pp->win_id))
			{
				PE_FWI_H13_MISC_CTRL fw_param;
				#ifdef PE_FWI_H13_USE_SHP_CTRL_WHEN_PQFW_OFF
				fw_param.gt_th1 = GET_BITS(pp->gt_th1,0,8);
				fw_param.gt_th0 = GET_BITS(pp->gt_th0,0,8);
				fw_param.gt_th0a = GET_BITS(pp->gt_th0a,0,8);
				fw_param.gt_th0b = GET_BITS(pp->gt_th0b,0,8);
				ret = PE_FWI_H13_SetShpMiscCtrl(&fw_param);
				PE_SHP_HW_H13_CHECK_CODE(ret,break,"[%s,%d] PE_FWI_H13_SetShpMiscCtrl() error.\n",__F__,__L__);
				#else
				PE_FWI_H13_SETTINGS_T fw_inf;
				ret = PE_FWI_H13_GetCurFwiSettings(&fw_inf);
				PE_SHP_HW_H13_CHECK_CODE(ret,break,"[%s,%d] PE_FWI_H13_GetCurFwiSettings() error.\n",__F__,__L__);
				PE_SHP_HW_H13_DBG_PRINT("fw_ctrl.enable : %s.\n",fw_inf.fw_ctrl.enable? "yes":"no");
				if(fw_inf.fw_ctrl.enable)
				{
					fw_param.gt_th1 = GET_BITS(pp->gt_th1,0,8);
					fw_param.gt_th0 = GET_BITS(pp->gt_th0,0,8);
					fw_param.gt_th0a = GET_BITS(pp->gt_th0a,0,8);
					fw_param.gt_th0b = GET_BITS(pp->gt_th0b,0,8);
					ret = PE_FWI_H13_SetShpMiscCtrl(&fw_param);
					PE_SHP_HW_H13_CHECK_CODE(ret,break,"[%s,%d] PE_FWI_H13_SetShpMiscCtrl() error.\n",__F__,__L__);
				}
				else
				{
					PE_P1L_H13B0_RdFL(pe1_coring_ctrl_05);
					PE_P1L_H13B0_RdFL(pe1_coring_ctrl_06);
					PE_P1L_H13B0_Wr01(pe1_coring_ctrl_05,	gt_th1,		GET_BITS(pp->gt_th1,0,8));
					PE_P1L_H13B0_Wr01(pe1_coring_ctrl_05,	gt_th0,		GET_BITS(pp->gt_th0,0,8));
					PE_P1L_H13B0_Wr01(pe1_coring_ctrl_06,	gt_th0a,	GET_BITS(pp->gt_th0a,0,8));
					PE_P1L_H13B0_Wr01(pe1_coring_ctrl_06,	gt_th0b,	GET_BITS(pp->gt_th0b,0,8));
					PE_P1L_H13B0_WrFL(pe1_coring_ctrl_05);
					PE_P1L_H13B0_WrFL(pe1_coring_ctrl_06);
				}
				#endif
				/* cit*/
				PE_P1L_H13B0_QWr01(pe1_cti_ctrl_00,	cti_en,				GET_BITS(pp->cti_en,0,1));
				PE_P1L_H13B0_QWr04(pe1_cti_ctrl_01,	coring_th0,			GET_BITS(pp->coring_th0,0,8),\
													coring_th1,			GET_BITS(pp->coring_th1,0,8),\
													coring_map_filter, 	GET_BITS(pp->coring_map_filter,0,3),\
													coring_tap_size, 	GET_BITS(pp->coring_tap_size,0,3));
				PE_P1L_H13B0_QWr03(pe1_cti_ctrl_02,	ycm_en1,			GET_BITS(pp->ycm_en1,0,1),\
													ycm_band_sel,		GET_BITS(pp->ycm_band_sel,0,3),\
													ycm_diff_th,		GET_BITS(pp->ycm_diff_th,0,8));
				/* h */
				PE_P1L_H13B0_QWr01(pe1_derh_ctrl_06, edge_y_filter_en,	GET_BITS(pp->edge_y_filter_en,0,1));
				PE_P1L_H13B0_QWr02(pe1_derh_ctrl_0c, e_gain_max,		GET_BITS(pp->e_gain_max,0,6),\
													 f_gain_max,		GET_BITS(pp->f_gain_max,0,6));
				PE_P1L_H13B0_QWr01(pe1_mp_ctrl_06,	lap_h_mode,			GET_BITS(pp->mp_lap_h_mode,0,3));
				PE_P1L_H13B0_QWr01(pe1_sp_ctrl_06,	lap_h_mode,			GET_BITS(pp->sp_lap_h_mode,0,3));
				/* v */
				PE_P1L_H13B0_QWr02(pe1_derv_ctrl_0,	der_v_en,			GET_BITS(pp->der_v_en,0,1),\
													der_gain_mapping,	GET_BITS(pp->der_gain_mapping,0,2));
				PE_P1L_H13B0_QWr01(pe1_derv_ctrl_2,	max_sel,			GET_BITS(pp->max_sel,0,3));
				PE_P1L_H13B0_QWr01(pe1_mp_ctrl_06,	lap_v_mode,			GET_BITS(pp->mp_lap_v_mode,0,3));
				#ifdef PE_SHP_HW_H13_USE_SHP_MODE
				pInfo->sp_lap_v_mode = GET_BITS(pp->sp_lap_v_mode,0,3);
				#else
				PE_P1L_H13B0_QWr01(pe1_sp_ctrl_06,	lap_v_mode,			GET_BITS(pp->sp_lap_v_mode,0,3));
				#endif
				/* cmn */
				PE_P1L_H13B0_QWr02(pe1_mp_ctrl_02,	sobel_weight,		GET_BITS(pp->mp_sobel_weight,0,8),\
													laplacian_weight,	GET_BITS(pp->mp_laplacian_weight,0,8));
				PE_P1L_H13B0_QWr02(pe1_sp_ctrl_02,	sobel_weight,		GET_BITS(pp->sp_sobel_weight,0,8),\
													laplacian_weight,	GET_BITS(pp->sp_laplacian_weight,0,8));
				
				PE_P1L_H13B0_QWr02(pe1_derh_ctrl_07, flat_filter_en,	GET_BITS(pp->flat_en,0,1),\
													 flat_filter_type,	GET_BITS(pp->flat_filter_type,0,1));
				/* d_jag */
				PE_P1L_H13B0_QWr04(pe1_dj_ctrl_00,	edf_en,				GET_BITS(pp->edf_en,0,1),\
													center_blur_en, 	GET_BITS(pp->center_blur_en,0,1),\
													count_diff_th, 		GET_BITS(pp->count_diff_th,0,5),\
													n_avg_mode,			GET_BITS(pp->n_avg_mode,0,1));
				PE_P1L_H13B0_QWr01(pe1_dj_ctrl_00,	line_variation_diff_threshold,	GET_BITS(pp->line_variation_diff_th,0,8));
				PE_P1L_H13B0_QWr03(pe1_dj_ctrl_01,	level_th,			GET_BITS(pp->level_th,0,8),\
													protect_th,			GET_BITS(pp->protect_th,0,8),\
													n_avg_gain,			GET_BITS(pp->n_avg_gain,0,8));
				PE_P1L_H13B0_QWr02(pe1_dj_ctrl_02,	edf_count_min,		GET_BITS(pp->edf_count_min,0,5),\
													edf_count_max,		GET_BITS(pp->edf_count_max,0,5));
				PE_P1L_H13B0_QWr04(pe1_dj_ctrl_03,	dj_h_count_min,		GET_BITS(pp->dj_h_count_min,0,5),\
													dj_h_count_max, 	GET_BITS(pp->dj_h_count_max,0,5),\
													dj_v_count_min, 	GET_BITS(pp->dj_v_count_min,0,5),\
													dj_v_count_max,		GET_BITS(pp->dj_v_count_max,0,5));
				/* e_map */
				PE_P1L_H13B0_QWr04(pe1_coring_ctrl_00,	amap2_sel,		GET_BITS(pp->amap2_sel,0,2),\
														ga_max,			GET_BITS(pp->ga_max,0,6),\
														ga_th0,			GET_BITS(pp->ga_th0,0,8),\
														ga_th1,			GET_BITS(pp->ga_th1,0,8));
				PE_P1L_H13B0_QWr01(pe1_coring_ctrl_07,	amap_gain,		GET_BITS(pp->amap_gain,0,7));
				/* t_map */
				PE_P1L_H13B0_QWr01(pe1_coring_ctrl_00,	amap1_sel,		GET_BITS(pp->amap1_sel,0,2));
				PE_P1L_H13B0_QWr04(pe1_coring_ctrl_01,	max_sel,		GET_BITS(pp->tmap_max_sel,0,3),\
														avg_sel,		GET_BITS(pp->avg_sel,0,2),\
														a2tw_th0,		GET_BITS(pp->a2tw_th0,0,8),\
														a2tw_th1,		GET_BITS(pp->a2tw_th1,0,8));
				PE_P1L_H13B0_QWr03(pe1_coring_ctrl_07,	tmap_gain,		GET_BITS(pp->tmap_gain,0,7),\
														g_th0,			GET_BITS(pp->g_th0,0,8),\
														g_th1,			GET_BITS(pp->g_th1,0,8));				
				PE_P1L_H13B0_QWr02(pe1_coring_ctrl_06,	gt_gain0a,		GET_BITS(pp->gt_gain0a,0,6),\
														gt_gain0b,		GET_BITS(pp->gt_gain0b,0,6));
				PE_P1L_H13B0_QWr02(pe1_coring_ctrl_05,	gt_max,			GET_BITS(pp->gt_max,0,6),\
														a2tw_en,		GET_BITS(pp->a2tw_en,0,1));
				PE_P1L_H13B0_QWr04(pe1_coring_ctrl_04,	exp_mode,		GET_BITS(pp->exp_mode,0,2),\
														coring_mode1,	GET_BITS(pp->coring_mode1,0,2),\
														coring_mode2,	GET_BITS(pp->coring_mode2,0,2),\
														var_th,			GET_BITS(pp->var_th,0,11));
				/* ti-h*/
				PE_P1L_H13B0_QWr02(pe1_ti_ctrl_0, 		enable,			GET_BITS(pp->enable, 0,1),\
												  		coring_step, 	GET_BITS(pp->coring_step,0,3));
				PE_P1L_H13B0_QWr04(pe1_ti_ctrl_1, 		gain0_en, 		GET_BITS(pp->gain0_en,0,1),\
														gain1_en, 		GET_BITS(pp->gain1_en,0,1),\
														gain0_th0,		GET_BITS(pp->gain0_th0,0,8),\
														gain1_th1,		GET_BITS(pp->gain1_th1,0,8));
				PE_P1L_H13B0_QWr01(pe1_ti_ctrl_2, 		gain1_div_mode, GET_BITS(pp->gain1_div_mode,0,1));
			}
		}	
		else if(PE_KDRV_VER_H13AX)
		{
			LX_PE_SHP_RE1_MISC_T *pp=(LX_PE_SHP_RE1_MISC_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			PE_SHP_HW_H13_DBG_PRINT("set[%d] :  resolution misc ctrl\n"\
				" e-map : sel:%d, ga:%d,%d,%d\n"\
				" t-map : sel:%d,%d, g:%d, gt:%d,%d,%d,%d,%d,%d,%d, cor1:%d,var_th:%d\n"\
				" d-jag : en:%d, th:%d,%d, g:%d\n",pp->win_id,\
				pp->amap2_sel,pp->ga_max,pp->ga_th0,pp->ga_th1,pp->amap1_sel,\
				pp->max_sel,pp->tmap_gain,pp->gt_th0,pp->gt_th0a,pp->gt_th0b,\
				pp->gt_th1,pp->gt_gain0a,pp->gt_gain0b,pp->gt_max,pp->coring_mode1,pp->var_th,\
				pp->center_blur_en,pp->level_th,pp->protect_th,pp->n_avg_gain);
			if(PE_CHECK_WIN0(pp->win_id))
			{
				PE_P1L_H13_RdFL(pe1_coring_ctrl_04);
				PE_P1L_H13_RdFL(pe1_coring_ctrl_05);
				PE_P1L_H13_RdFL(pe1_coring_ctrl_06);
				PE_P1L_H13_RdFL(pe1_coring_ctrl_00);
				PE_P1L_H13_RdFL(pe1_coring_ctrl_01);
				PE_P1L_H13_RdFL(pe1_coring_ctrl_07);
				PE_P1L_H13_RdFL(pe1_dj_ctrl_00);
				PE_P1L_H13_RdFL(pe1_dj_ctrl_01);
				PE_P1L_H13_Wr01(pe1_coring_ctrl_04,	coring_mode1,	GET_BITS(pp->coring_mode1,0,2));
				PE_P1L_H13_Wr01(pe1_coring_ctrl_04,	var_th,			GET_BITS(pp->var_th,0,11));
				PE_P1L_H13_Wr01(pe1_coring_ctrl_05,	gt_max,			GET_BITS(pp->gt_max,0,6));
				PE_P1L_H13_Wr01(pe1_coring_ctrl_05,	gt_th1,			GET_BITS(pp->gt_th1,0,8));
				PE_P1L_H13_Wr01(pe1_coring_ctrl_05,	gt_th0,			GET_BITS(pp->gt_th0,0,8));
				PE_P1L_H13_Wr01(pe1_coring_ctrl_06,	gt_gain0a,		GET_BITS(pp->gt_gain0a,0,6));
				PE_P1L_H13_Wr01(pe1_coring_ctrl_06,	gt_gain0b,		GET_BITS(pp->gt_gain0b,0,6));
				PE_P1L_H13_Wr01(pe1_coring_ctrl_06,	gt_th0a,		GET_BITS(pp->gt_th0a,0,8));
				PE_P1L_H13_Wr01(pe1_coring_ctrl_06,	gt_th0b,		GET_BITS(pp->gt_th0b,0,8));
				PE_P1L_H13_Wr01(pe1_coring_ctrl_00,	amap2_sel,		GET_BITS(pp->amap2_sel,0,2));
				PE_P1L_H13_Wr01(pe1_coring_ctrl_00,	ga_max,			GET_BITS(pp->ga_max,0,6));
				PE_P1L_H13_Wr01(pe1_coring_ctrl_00,	ga_th0,			GET_BITS(pp->ga_th0,0,8));
				PE_P1L_H13_Wr01(pe1_coring_ctrl_00,	ga_th1,			GET_BITS(pp->ga_th1,0,8));
				PE_P1L_H13_Wr01(pe1_coring_ctrl_00,	amap1_sel,		GET_BITS(pp->amap1_sel,0,2));
				PE_P1L_H13_Wr01(pe1_coring_ctrl_01,	max_sel,		GET_BITS(pp->max_sel,0,3));
				PE_P1L_H13_Wr01(pe1_coring_ctrl_07,	tmap_gain,		GET_BITS(pp->tmap_gain,0,7));
				PE_P1L_H13_Wr01(pe1_dj_ctrl_00,		center_blur_en,	GET_BITS(pp->center_blur_en,0,1));
				PE_P1L_H13_Wr01(pe1_dj_ctrl_01,		level_th,		GET_BITS(pp->level_th,0,8));
				PE_P1L_H13_Wr01(pe1_dj_ctrl_01,		protect_th,		GET_BITS(pp->protect_th,0,8));
				PE_P1L_H13_Wr01(pe1_dj_ctrl_01,		n_avg_gain,		GET_BITS(pp->n_avg_gain,0,8));
				PE_P1L_H13_WrFL(pe1_coring_ctrl_04);
				PE_P1L_H13_WrFL(pe1_coring_ctrl_05);
				PE_P1L_H13_WrFL(pe1_coring_ctrl_06);
				PE_P1L_H13_WrFL(pe1_coring_ctrl_00);
				PE_P1L_H13_WrFL(pe1_coring_ctrl_01);
				PE_P1L_H13_WrFL(pe1_coring_ctrl_07);
				PE_P1L_H13_WrFL(pe1_dj_ctrl_00);
				PE_P1L_H13_WrFL(pe1_dj_ctrl_01);
			}
		}
		else
		{
			PE_SHP_HW_H13_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	} while (0);
	return ret;
}
/**
 * get resolution misc control
 * - use input struct LX_PE_SHP_RE2_MISC_T for H13BX
 * - use input struct LX_PE_SHP_RE1_MISC_T for H13AX
 *
 * @param   *pstParams [in/out] void
 * @return  OK if success, ERROR otherwise.
 * @see		function description
 * @author
 */
int PE_SHP_HW_H13_GetReMiscCtrl(void *pstParams)
{
	int ret = RET_OK;
	LX_PE_WIN_ID win_id;
	do {
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H13BX)
		{
			LX_PE_SHP_RE2_MISC_T *pp=(LX_PE_SHP_RE2_MISC_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			win_id = PE_GET_CHECKED_WINID(pp->win_id);
			if(PE_CHECK_WIN0(win_id))
			{
				
				#ifdef PE_FWI_H13_USE_SHP_CTRL_WHEN_PQFW_OFF
				PE_FWI_H13_SETTINGS_T fw_inf;
				ret = PE_FWI_H13_GetCurFwiSettings(&fw_inf);
				PE_SHP_HW_H13_CHECK_CODE(ret,break,"[%s,%d] PE_FWI_H13_GetCurFwiSettings() error.\n",__F__,__L__);
				PE_SHP_HW_H13_DBG_PRINT("fw_ctrl.enable : %s.\n",fw_inf.fw_ctrl.enable? "yes":"no");
				if(fw_inf.fw_ctrl.enable)
				{
					pp->gt_th0 = fw_inf.shp_m_ctrl.gt_th0;
					pp->gt_th1 = fw_inf.shp_m_ctrl.gt_th1;
					pp->gt_th0a = fw_inf.shp_m_ctrl.gt_th0a;
					pp->gt_th0b = fw_inf.shp_m_ctrl.gt_th0b;
				}
				else
				#endif
				{
					PE_P1L_H13B0_RdFL(pe1_coring_ctrl_05);
					PE_P1L_H13B0_RdFL(pe1_coring_ctrl_06);
					PE_P1L_H13B0_Rd01(pe1_coring_ctrl_05,	gt_th1,		pp->gt_th1);
					PE_P1L_H13B0_Rd01(pe1_coring_ctrl_05,	gt_th0,		pp->gt_th0);
					PE_P1L_H13B0_Rd01(pe1_coring_ctrl_06,	gt_th0a,	pp->gt_th0a);
					PE_P1L_H13B0_Rd01(pe1_coring_ctrl_06,	gt_th0b,	pp->gt_th0b);
				}
				/* cit*/
				PE_P1L_H13B0_QRd01(pe1_cti_ctrl_00,	cti_en,				pp->cti_en);
				PE_P1L_H13B0_QRd04(pe1_cti_ctrl_01,	coring_th0,			pp->coring_th0,\
													coring_th1,			pp->coring_th1,\
													coring_map_filter, 	pp->coring_map_filter,\
													coring_tap_size, 	pp->coring_tap_size);
				PE_P1L_H13B0_QRd03(pe1_cti_ctrl_02,	ycm_en1,			pp->ycm_en1,\
													ycm_band_sel,		pp->ycm_band_sel,\
													ycm_diff_th,		pp->ycm_diff_th);
				/* h */
				PE_P1L_H13B0_QRd01(pe1_derh_ctrl_06, edge_y_filter_en,	pp->edge_y_filter_en);
				PE_P1L_H13B0_QRd02(pe1_derh_ctrl_0c, e_gain_max,		pp->e_gain_max,\
													 f_gain_max,		pp->f_gain_max);
				PE_P1L_H13B0_QRd01(pe1_mp_ctrl_06,	lap_h_mode,			pp->mp_lap_h_mode);
				PE_P1L_H13B0_QRd01(pe1_sp_ctrl_06,	lap_h_mode,			pp->sp_lap_h_mode);
				/* v */
				PE_P1L_H13B0_QRd02(pe1_derv_ctrl_0,	der_v_en,			pp->der_v_en,\
													der_gain_mapping,	pp->der_gain_mapping);
				PE_P1L_H13B0_QRd01(pe1_derv_ctrl_2,	max_sel,			pp->max_sel);
				PE_P1L_H13B0_QRd01(pe1_mp_ctrl_06,	lap_v_mode,			pp->mp_lap_v_mode);
				PE_P1L_H13B0_QRd01(pe1_sp_ctrl_06,	lap_v_mode,			pp->sp_lap_v_mode);
				/* cmn */
				PE_P1L_H13B0_QRd02(pe1_mp_ctrl_02,	sobel_weight,		pp->mp_sobel_weight,\
													laplacian_weight,	pp->mp_laplacian_weight);
				PE_P1L_H13B0_QRd02(pe1_sp_ctrl_02,	sobel_weight,		pp->sp_sobel_weight,\
													laplacian_weight,	pp->sp_laplacian_weight);
				
				PE_P1L_H13B0_QRd02(pe1_derh_ctrl_07, flat_filter_en,	pp->flat_en,\
													 flat_filter_type,	pp->flat_filter_type);
				/* d_jag */
				PE_P1L_H13B0_QRd04(pe1_dj_ctrl_00,	edf_en,				pp->edf_en,\
													center_blur_en, 	pp->center_blur_en,\
													count_diff_th, 		pp->count_diff_th,\
													n_avg_mode,			pp->n_avg_mode);
				PE_P1L_H13B0_QRd01(pe1_dj_ctrl_00,	line_variation_diff_threshold,	pp->line_variation_diff_th);
				PE_P1L_H13B0_QRd03(pe1_dj_ctrl_01,	level_th,			pp->level_th,\
													protect_th,			pp->protect_th,\
													n_avg_gain,			pp->n_avg_gain);
				PE_P1L_H13B0_QRd02(pe1_dj_ctrl_02,	edf_count_min,		pp->edf_count_min,\
													edf_count_max,		pp->edf_count_max);
				PE_P1L_H13B0_QRd04(pe1_dj_ctrl_03,	dj_h_count_min,		pp->dj_h_count_min,\
													dj_h_count_max, 	pp->dj_h_count_max,\
													dj_v_count_min, 	pp->dj_v_count_min,\
													dj_v_count_max,		pp->dj_v_count_max);
				/* e_map */
				PE_P1L_H13B0_QRd04(pe1_coring_ctrl_00,	amap2_sel,		pp->amap2_sel,\
														ga_max,			pp->ga_max,\
														ga_th0,			pp->ga_th0,\
														ga_th1,			pp->ga_th1);
				PE_P1L_H13B0_QRd01(pe1_coring_ctrl_07,	amap_gain,		pp->amap_gain);
				/* t_map */
				PE_P1L_H13B0_QRd01(pe1_coring_ctrl_00,	amap1_sel,		pp->amap1_sel);
				PE_P1L_H13B0_QRd04(pe1_coring_ctrl_01,	max_sel,		pp->tmap_max_sel,\
														avg_sel,		pp->avg_sel,\
														a2tw_th0,		pp->a2tw_th0,\
														a2tw_th1,		pp->a2tw_th1);
				PE_P1L_H13B0_QRd03(pe1_coring_ctrl_07,	tmap_gain,		pp->tmap_gain,\
														g_th0,			pp->g_th0,\
														g_th1,			pp->g_th1);				
				PE_P1L_H13B0_QRd02(pe1_coring_ctrl_06,	gt_gain0a,		pp->gt_gain0a,\
														gt_gain0b,		pp->gt_gain0b);
				PE_P1L_H13B0_QRd02(pe1_coring_ctrl_05,	gt_max,			pp->gt_max,\
														a2tw_en,		pp->a2tw_en);
				PE_P1L_H13B0_QRd04(pe1_coring_ctrl_04,	exp_mode,		pp->exp_mode,\
														coring_mode1,	pp->coring_mode1,\
														coring_mode2,	pp->coring_mode2,\
														var_th,			pp->var_th);
				/* ti-h*/
				PE_P1L_H13B0_QRd02(pe1_ti_ctrl_0, 		enable,			pp->enable,\
												  		coring_step, 	pp->coring_step);
				PE_P1L_H13B0_QRd04(pe1_ti_ctrl_1, 		gain0_en, 		pp->gain0_en,\
														gain1_en, 		pp->gain1_en,\
														gain0_th0,		pp->gain0_th0,\
														gain1_th1,		pp->gain1_th1);
				PE_P1L_H13B0_QRd01(pe1_ti_ctrl_2, 		gain1_div_mode, pp->gain1_div_mode);
			}
			PE_SHP_HW_H13_DBG_PRINT("get[%d] :  resolution misc ctrl\n"\
				" e-map : sel:%d, ga:%d,%d,%d\n"\
				" t-map : sel:%d,%d, g:%d, gt:%d,%d,%d,%d,%d,%d,%d, cor1:%d,var_th:%d\n"\
				" d-jag : en:%d, th:%d,%d, g:%d\n",pp->win_id,\
				pp->amap2_sel,pp->ga_max,pp->ga_th0,pp->ga_th1,pp->amap1_sel,\
				pp->max_sel,pp->tmap_gain,pp->gt_th0,pp->gt_th0a,pp->gt_th0b,\
				pp->gt_th1,pp->gt_gain0a,pp->gt_gain0b,pp->gt_max,pp->coring_mode1,pp->var_th,\
				pp->center_blur_en,pp->level_th,pp->protect_th,pp->n_avg_gain);
		}
		else if(PE_KDRV_VER_H13AX)
		{
			LX_PE_SHP_RE1_MISC_T *pp=(LX_PE_SHP_RE1_MISC_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			win_id = PE_GET_CHECKED_WINID(pp->win_id);
			if(PE_CHECK_WIN0(win_id))
			{
				PE_P1L_H13_RdFL(pe1_coring_ctrl_04);
				PE_P1L_H13_RdFL(pe1_coring_ctrl_05);
				PE_P1L_H13_RdFL(pe1_coring_ctrl_06);
				PE_P1L_H13_RdFL(pe1_coring_ctrl_00);
				PE_P1L_H13_RdFL(pe1_coring_ctrl_01);
				PE_P1L_H13_RdFL(pe1_coring_ctrl_07);
				PE_P1L_H13_RdFL(pe1_dj_ctrl_00);
				PE_P1L_H13_RdFL(pe1_dj_ctrl_01);
				PE_P1L_H13_Rd01(pe1_coring_ctrl_04,	coring_mode1,	pp->coring_mode1);
				PE_P1L_H13_Rd01(pe1_coring_ctrl_04,	var_th,			pp->var_th);
				PE_P1L_H13_Rd01(pe1_coring_ctrl_05,	gt_max,			pp->gt_max);
				PE_P1L_H13_Rd01(pe1_coring_ctrl_05,	gt_th1,			pp->gt_th1);
				PE_P1L_H13_Rd01(pe1_coring_ctrl_05,	gt_th0,			pp->gt_th0);
				PE_P1L_H13_Rd01(pe1_coring_ctrl_06,	gt_gain0a,		pp->gt_gain0a);
				PE_P1L_H13_Rd01(pe1_coring_ctrl_06,	gt_gain0b,		pp->gt_gain0b);
				PE_P1L_H13_Rd01(pe1_coring_ctrl_06,	gt_th0a,		pp->gt_th0a);
				PE_P1L_H13_Rd01(pe1_coring_ctrl_06,	gt_th0b,		pp->gt_th0b);
				PE_P1L_H13_Rd01(pe1_coring_ctrl_00,	amap2_sel,		pp->amap2_sel);
				PE_P1L_H13_Rd01(pe1_coring_ctrl_00,	ga_max,			pp->ga_max);
				PE_P1L_H13_Rd01(pe1_coring_ctrl_00,	ga_th0,			pp->ga_th0);
				PE_P1L_H13_Rd01(pe1_coring_ctrl_00,	ga_th1,			pp->ga_th1);
				PE_P1L_H13_Rd01(pe1_coring_ctrl_00,	amap1_sel,		pp->amap1_sel);
				PE_P1L_H13_Rd01(pe1_coring_ctrl_01,	max_sel,		pp->max_sel);
				PE_P1L_H13_Rd01(pe1_coring_ctrl_07,	tmap_gain,		pp->tmap_gain);
				PE_P1L_H13_Rd01(pe1_dj_ctrl_00,		center_blur_en,	pp->center_blur_en);
				PE_P1L_H13_Rd01(pe1_dj_ctrl_01,		level_th,		pp->level_th);
				PE_P1L_H13_Rd01(pe1_dj_ctrl_01,		protect_th,		pp->protect_th);
				PE_P1L_H13_Rd01(pe1_dj_ctrl_01,		n_avg_gain,		pp->n_avg_gain);
			}
			PE_SHP_HW_H13_DBG_PRINT("get[%d] :  resolution misc ctrl\n"\
				" e-map : sel:%d, ga:%d,%d,%d\n"\
				" t-map : sel:%d,%d, g:%d, gt:%d,%d,%d,%d,%d,%d,%d, cor1:%d,var_th:%d\n"\
				" d-jag : en:%d, th:%d,%d, g:%d\n",pp->win_id,\
				pp->amap2_sel,pp->ga_max,pp->ga_th0,pp->ga_th1,pp->amap1_sel,\
				pp->max_sel,pp->tmap_gain,pp->gt_th0,pp->gt_th0a,pp->gt_th0b,\
				pp->gt_th1,pp->gt_gain0a,pp->gt_gain0b,pp->gt_max,pp->coring_mode1,pp->var_th,\
				pp->center_blur_en,pp->level_th,pp->protect_th,pp->n_avg_gain);
		}
		else
		{
			PE_SHP_HW_H13_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * set edge enhance control
 * - use input struct LX_PE_SHP_EE1_T for H13
 *
 * @param   *pstParams [in] void
 * @return  OK if success, ERROR otherwise.
 * @see		function description
 * @author
 */
int PE_SHP_HW_H13_SetEdgeEnhanceCtrl(void *pstParams)
{
	int ret = RET_OK;
	do {
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H13)
		{
			LX_PE_SHP_EE1_T *pp=(LX_PE_SHP_EE1_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			PE_SHP_HW_H13_DBG_PRINT("set[%d]: mp g_b,w:%d,%d, sp g_b,w:%d,%d\n",pp->win_id,\
				pp->mp_edge_gain_b,pp->mp_edge_gain_w,pp->sp_edge_gain_b,pp->sp_edge_gain_w);
			if(PE_CHECK_WIN0(pp->win_id))
			{
				PE_FWI_H13_EDGE_GAIN fw_param;
				#ifdef PE_FWI_H13_USE_SHP_CTRL_WHEN_PQFW_OFF
				fw_param.co09_gain_b = GET_BITS(pp->mp_edge_gain_b,0,7);
				fw_param.co09_gain_w = GET_BITS(pp->mp_edge_gain_w,0,7);
				fw_param.dp01_gain_b = GET_BITS(pp->sp_edge_gain_b,0,7);
				fw_param.dp01_gain_w = GET_BITS(pp->sp_edge_gain_w,0,7);
				ret = PE_FWI_H13_SetEdgeGainCtrl(&fw_param);
				PE_SHP_HW_H13_CHECK_CODE(ret,break,"[%s,%d] PE_FWI_H13_SetEdgeGainCtrl() error.\n",__F__,__L__);
				#else
				PE_FWI_H13_SETTINGS_T fw_inf;
				ret = PE_FWI_H13_GetCurFwiSettings(&fw_inf);
				PE_SHP_HW_H13_CHECK_CODE(ret,break,"[%s,%d] PE_FWI_H13_GetCurFwiSettings() error.\n",__F__,__L__);
				PE_SHP_HW_H13_DBG_PRINT("fw_ctrl.enable : %s.\n",fw_inf.fw_ctrl.enable? "yes":"no");
				if(fw_inf.fw_ctrl.enable)
				{
					fw_param.co09_gain_b = GET_BITS(pp->mp_edge_gain_b,0,7);
					fw_param.co09_gain_w = GET_BITS(pp->mp_edge_gain_w,0,7);
					fw_param.dp01_gain_b = GET_BITS(pp->sp_edge_gain_b,0,7);
					fw_param.dp01_gain_w = GET_BITS(pp->sp_edge_gain_w,0,7);
					ret = PE_FWI_H13_SetEdgeGainCtrl(&fw_param);
					PE_SHP_HW_H13_CHECK_CODE(ret,break,"[%s,%d] PE_FWI_H13_SetEdgeGainCtrl() error.\n",__F__,__L__);
				}
				else
				{
					PE_P1L_H13_RdFL(pe1_coring_ctrl_09);
					PE_P1L_H13_RdFL(pe1_dp_ctrl_01);
					PE_P1L_H13_Wr01(pe1_coring_ctrl_09,	edge_gain_b,	GET_BITS(pp->mp_edge_gain_b,0,7));
					PE_P1L_H13_Wr01(pe1_coring_ctrl_09,	edge_gain_w,	GET_BITS(pp->mp_edge_gain_w,0,7));
					PE_P1L_H13_Wr01(pe1_dp_ctrl_01,		edge_gain_b,	GET_BITS(pp->sp_edge_gain_b,0,7));
					PE_P1L_H13_Wr01(pe1_dp_ctrl_01,		edge_gain_w,	GET_BITS(pp->sp_edge_gain_w,0,7));
					PE_P1L_H13_WrFL(pe1_coring_ctrl_09);
					PE_P1L_H13_WrFL(pe1_dp_ctrl_01);
				}
				#endif
			}
		}
		else
		{
			PE_SHP_HW_H13_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * get edge enhance control
 * - use input struct LX_PE_SHP_EE1_T for H13
 *
 * @param   *pstParams [in/out] void
 * @return  OK if success, ERROR otherwise.
 * @see		function description
 * @author
 */
int PE_SHP_HW_H13_GetEdgeEnhanceCtrl(void *pstParams)
{
	int ret = RET_OK;
	LX_PE_WIN_ID win_id;
	do {
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H13)
		{
			LX_PE_SHP_EE1_T *pp=(LX_PE_SHP_EE1_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			win_id = PE_GET_CHECKED_WINID(pp->win_id);
			if(PE_CHECK_WIN0(win_id))
			{
				PE_FWI_H13_SETTINGS_T fw_inf;
				ret = PE_FWI_H13_GetCurFwiSettings(&fw_inf);
				PE_SHP_HW_H13_CHECK_CODE(ret,break,"[%s,%d] PE_FWI_H13_GetCurFwiSettings() error.\n",__F__,__L__);
				PE_SHP_HW_H13_DBG_PRINT("fw_ctrl.enable : %s.\n",fw_inf.fw_ctrl.enable? "yes":"no");
				if(fw_inf.fw_ctrl.enable)
				{
					pp->mp_edge_gain_b = fw_inf.shp_e_ctrl.co09_gain_b;
					pp->mp_edge_gain_w = fw_inf.shp_e_ctrl.co09_gain_w;
					pp->sp_edge_gain_b = fw_inf.shp_e_ctrl.dp01_gain_b;
					pp->sp_edge_gain_w = fw_inf.shp_e_ctrl.dp01_gain_w;
				}
				else
				{
					PE_P1L_H13_RdFL(pe1_coring_ctrl_09);
					PE_P1L_H13_RdFL(pe1_dp_ctrl_01);
					PE_P1L_H13_Rd01(pe1_coring_ctrl_09,	edge_gain_b,	pp->mp_edge_gain_b);
					PE_P1L_H13_Rd01(pe1_coring_ctrl_09,	edge_gain_w,	pp->mp_edge_gain_w);
					PE_P1L_H13_Rd01(pe1_dp_ctrl_01,		edge_gain_b,	pp->sp_edge_gain_b);
					PE_P1L_H13_Rd01(pe1_dp_ctrl_01,		edge_gain_w,	pp->sp_edge_gain_w);
				}
			}
			PE_SHP_HW_H13_DBG_PRINT("get[%d]: mp g_b,w:%d,%d, sp g_b,w:%d,%d\n",\
				pp->win_id,pp->mp_edge_gain_b,pp->mp_edge_gain_w,\
				pp->sp_edge_gain_b,pp->sp_edge_gain_w);
		}
		else
		{
			PE_SHP_HW_H13_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * set detail enhance control
 * - use input struct LX_PE_SHP_DE1_T for H13
 *
 * @param   *pstParams [in] void
 * @return  OK if success, ERROR otherwise.
 * @see		function description
 * @author
 */
int PE_SHP_HW_H13_SetDetailEnhanceCtrl(void *pstParams)
{
	int ret = RET_OK;
	do {
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H13)
		{
			LX_PE_SHP_DE1_T *pp=(LX_PE_SHP_DE1_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			PE_SHP_HW_H13_DBG_PRINT("set[%d]: mp g_b,w:%d,%d, sp g_b,w:%d,%d\n",\
				pp->win_id,pp->mp_texture_gain_b,pp->mp_texture_gain_w,\
				pp->sp_texture_gain_b,pp->sp_texture_gain_w);
			if(PE_CHECK_WIN0(pp->win_id))
			{
				PE_FWI_H13_TEXTURE_GAIN fw_param;
				#ifdef PE_FWI_H13_USE_SHP_CTRL_WHEN_PQFW_OFF
				fw_param.co09_gain_b = GET_BITS(pp->mp_texture_gain_b,0,7);
				fw_param.co09_gain_w = GET_BITS(pp->mp_texture_gain_w,0,7);
				fw_param.dp01_gain_b = GET_BITS(pp->sp_texture_gain_b,0,7);
				fw_param.dp01_gain_w = GET_BITS(pp->sp_texture_gain_w,0,7);
				ret = PE_FWI_H13_SetTextureGainCtrl(&fw_param);
				PE_SHP_HW_H13_CHECK_CODE(ret,break,"[%s,%d] PE_FWI_H13_SetTextureGainCtrl() error.\n",__F__,__L__);
				#else
				PE_FWI_H13_SETTINGS_T fw_inf;
				ret = PE_FWI_H13_GetCurFwiSettings(&fw_inf);
				PE_SHP_HW_H13_CHECK_CODE(ret,break,"[%s,%d] PE_FWI_H13_GetCurFwiSettings() error.\n",__F__,__L__);
				PE_SHP_HW_H13_DBG_PRINT("fw_ctrl.enable : %s.\n",fw_inf.fw_ctrl.enable? "yes":"no");
				if(fw_inf.fw_ctrl.enable)
				{
					fw_param.co09_gain_b = GET_BITS(pp->mp_texture_gain_b,0,7);
					fw_param.co09_gain_w = GET_BITS(pp->mp_texture_gain_w,0,7);
					fw_param.dp01_gain_b = GET_BITS(pp->sp_texture_gain_b,0,7);
					fw_param.dp01_gain_w = GET_BITS(pp->sp_texture_gain_w,0,7);
					ret = PE_FWI_H13_SetTextureGainCtrl(&fw_param);
					PE_SHP_HW_H13_CHECK_CODE(ret,break,"[%s,%d] PE_FWI_H13_SetTextureGainCtrl() error.\n",__F__,__L__);
				}
				else
				{
					PE_P1L_H13_RdFL(pe1_coring_ctrl_09);
					PE_P1L_H13_RdFL(pe1_dp_ctrl_01);
					PE_P1L_H13_Wr01(pe1_coring_ctrl_09,	texture_gain_b,	GET_BITS(pp->mp_texture_gain_b,0,7));
					PE_P1L_H13_Wr01(pe1_coring_ctrl_09,	texture_gain_w,	GET_BITS(pp->mp_texture_gain_w,0,7));
					PE_P1L_H13_Wr01(pe1_dp_ctrl_01,		texture_gain_b,	GET_BITS(pp->sp_texture_gain_b,0,7));
					PE_P1L_H13_Wr01(pe1_dp_ctrl_01,		texture_gain_w,	GET_BITS(pp->sp_texture_gain_w,0,7));
					PE_P1L_H13_WrFL(pe1_coring_ctrl_09);
					PE_P1L_H13_WrFL(pe1_dp_ctrl_01);
				}
				#endif
			}
		}
		else
		{
			PE_SHP_HW_H13_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * get detail enhance control
 * - use input struct LX_PE_SHP_DE1_T for H13
 *
 * @param   *pstParams [in/out] void
 * @return  OK if success, ERROR otherwise.
 * @see		function description
 * @author
 */
int PE_SHP_HW_H13_GetDetailEnhanceCtrl(void *pstParams)
{
	int ret = RET_OK;
	LX_PE_WIN_ID win_id;
	do {
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H13)
		{
			LX_PE_SHP_DE1_T *pp=(LX_PE_SHP_DE1_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			win_id = PE_GET_CHECKED_WINID(pp->win_id);
			if(PE_CHECK_WIN0(win_id))
			{
				PE_FWI_H13_SETTINGS_T fw_inf;
				ret = PE_FWI_H13_GetCurFwiSettings(&fw_inf);
				PE_SHP_HW_H13_CHECK_CODE(ret,break,"[%s,%d] PE_FWI_H13_GetCurFwiSettings() error.\n",__F__,__L__);
				PE_SHP_HW_H13_DBG_PRINT("fw_ctrl.enable : %s.\n",fw_inf.fw_ctrl.enable? "yes":"no");
				if(fw_inf.fw_ctrl.enable)
				{
					pp->mp_texture_gain_b = fw_inf.shp_t_ctrl.co09_gain_b;
					pp->mp_texture_gain_w = fw_inf.shp_t_ctrl.co09_gain_w;
					pp->sp_texture_gain_b = fw_inf.shp_t_ctrl.dp01_gain_b;
					pp->sp_texture_gain_w = fw_inf.shp_t_ctrl.dp01_gain_w;
				}
				else
				{
					PE_P1L_H13_RdFL(pe1_coring_ctrl_09);
					PE_P1L_H13_RdFL(pe1_dp_ctrl_01);
					PE_P1L_H13_Rd01(pe1_coring_ctrl_09,	texture_gain_b,	pp->mp_texture_gain_b);
					PE_P1L_H13_Rd01(pe1_coring_ctrl_09,	texture_gain_w,	pp->mp_texture_gain_w);
					PE_P1L_H13_Rd01(pe1_dp_ctrl_01,		texture_gain_b,	pp->sp_texture_gain_b);
					PE_P1L_H13_Rd01(pe1_dp_ctrl_01,		texture_gain_w,	pp->sp_texture_gain_w);
				}
			}
			PE_SHP_HW_H13_DBG_PRINT("get[%d]: mp g_b,w:%d,%d, sp g_b,w:%d,%d\n",\
				pp->win_id,pp->mp_texture_gain_b,pp->mp_texture_gain_w,\
				pp->sp_texture_gain_b,pp->sp_texture_gain_w);
		}
		else
		{
			PE_SHP_HW_H13_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}

