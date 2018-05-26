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

/** @file pe_inf_m14.h
 *
 *  information management for picture enhance. ( used only within kdriver )
 *
 *	@author		Seung-Jun,Youm(sj.youm@lge.com)
 *	@version	0.1
 *	@note
 *	@date		2012.05.02
 *	@see
 */

#ifndef	_PE_INF_M14_H_
#define	_PE_INF_M14_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include "pe_kapi.h"

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/
/**
 *	pe read default reg menu enumeration
 */
typedef enum
{
	PE_INF_HW_M14B_M_DISPLAY = 0,			///< menu display
	PE_INF_HW_M14B_M_CVA_TPG___TYPE,		///< cva pattern_type
	PE_INF_HW_M14B_M_CVA_TPG__C_S_C,		///< cva pattern_csc
	PE_INF_HW_M14B_M_CVA_TPG_DETAIL,		///< cva pattern_detail
	PE_INF_HW_M14B_M_CVB_TPG___TYPE,		///< cvb pattern_type
	PE_INF_HW_M14B_M_CVB_TPG__C_S_C,		///< cvb pattern_csc
	PE_INF_HW_M14B_M_CVB_TPG_DETAIL,		///< cvb pattern_detail
	PE_INF_HW_M14B_M_P0M_DETOUR_ENB,		///< p0m detour_enable
	PE_INF_HW_M14B_M_P0M_TNR_ENABLE,		///< p0m tnr enable
	PE_INF_HW_M14B_M_P0M_IPC_Y_MODE,		///< p0m cr_luma_mode
	PE_INF_HW_M14B_M_P0M_IPC_C_MODE,		///< p0m cr_chroma_mode
	PE_INF_HW_M14B_M_P0M_FMD_ENABLE,		///< p0m film mode enable
	PE_INF_HW_M14B_M_P0M_CLC_FLT_EN,		///< p0m clc filter enable
	PE_INF_HW_M14B_M_P0D_DETOUR_ENB,		///< p0d tnr enable
	PE_INF_HW_M14B_M_P0D_VFILTER_EN,		///< p0d vfilter enable
	PE_INF_HW_M14B_M_P0D_CTI_ENABLE,		///< p0d cti enable
	PE_INF_HW_M14B_M_P0D_DNR_MAX_EN,		///< p0d dnr max enable
	PE_INF_HW_M14B_M_P0D_IFC_MAX_EN,		///< p0d ifc max enable
	PE_INF_HW_M14B_M_P0D_DNR_IFC_SL,		///< p0d dnr ifc selection
	PE_INF_HW_M14B_M_P0S_DETOUR_ENB,		///< p0s detour_enable
	PE_INF_HW_M14B_M_P0S_IPC_Y_MODE,		///< p0s cr_luma_mode
	PE_INF_HW_M14B_M_P0S_IPC_C_MODE,		///< p0s cr_chroma_mode
	PE_INF_HW_M14B_M_P0S_FMD_ENABLE,		///< p0s film mode enable
	PE_INF_HW_M14B_M_MSC_FILT_SEL_Y,		///< msc filter_type_sel_luma
	PE_INF_HW_M14B_M_MSC_FILT_SEL_C,		///< msc filter_type_sel_chroma
	PE_INF_HW_M14B_M_MSC_SAMPL_MODE,		///< msc sampling mode
	PE_INF_HW_M14B_M_SSC_FILT_SEL_Y,		///< ssc filter_type_sel
	#ifdef PE_HW_M14_USE_M14B_WCP_REG
	PE_INF_HW_M14B_M_OVL_DARK_W0_EN,		///< ovl w0_dark_en
	PE_INF_HW_M14B_M_OVL_DARK_W1_EN,		///< ovl w1_dark_en
	PE_INF_HW_M14B_M_OVL_DARK_COLOR,		///< ovl dark_color
	PE_INF_HW_M14B_M_OVL_BACK_COLOR,		///< ovl back_color
	PE_INF_HW_M14B_M_OVR_DARK_W0_EN,		///< ovr w0_dark_en
	PE_INF_HW_M14B_M_OVR_DARK_W1_EN,		///< ovr w1_dark_en
	PE_INF_HW_M14B_M_OVR_DARK_COLOR,		///< ovr dark_color
	PE_INF_HW_M14B_M_OVR_BACK_COLOR,		///< ovr back_color
	#endif
	PE_INF_HW_M14B_M_PE1_DETOUR_ENB,		///< pe1 detour_enable
	PE_INF_HW_M14B_M_PE1_WIN1_W0_EN,		///< pe1 winodw1 w0 enable
	PE_INF_HW_M14B_M_PE1_WIN1_W1_EN,		///< pe1 winodw1 w1 enable
	PE_INF_HW_M14B_M_PE1_WIN1_01_EN,		///< pe1 winodw1 w01 enable
	PE_INF_HW_M14B_M_PE1_WIN2_W0_EN,		///< pe1 winodw2 w0 enable
	PE_INF_HW_M14B_M_PE1_WIN2_W1_EN,		///< pe1 winodw2 w1 enable
	PE_INF_HW_M14B_M_PE1_WIN2_01_EN,		///< pe1 winodw2 w01 enable
	PE_INF_HW_M14B_M_PE1_WIN3_W0_EN,		///< pe1 winodw3 w0 enable
	PE_INF_HW_M14B_M_PE1_WIN3_W1_EN,		///< pe1 winodw3 w1 enable
	PE_INF_HW_M14B_M_PE1_WIN3_01_EN,		///< pe1 winodw3 w01 enable
	PE_INF_HW_M14B_M_PE1_PMUX3D_ENB,		///< pe1 pip mux 3d enable
	PE_INF_HW_M14B_M_PE1_PMUXOUT_SL,		///< pe1 pip mux out selection
	PE_INF_HW_M14B_M_PE1_DR_H_Y_ENB,		///< pe1 derh y enable
	PE_INF_HW_M14B_M_PE1_DR_H_C_ENB,		///< pe1 derh c enable
	PE_INF_HW_M14B_M_PE1_DR_V__ENAB,		///< pe1 derv enable
	PE_INF_HW_M14B_M_PE1_MP__ENABLE,		///< pe1 mp enable
	PE_INF_HW_M14B_M_PE1_SP__ENABLE,		///< pe1 sp enable
	PE_INF_HW_M14B_M_PE1_SR_OUT_MUX,		///< pe1 sr out mux selection
	PE_INF_HW_M14B_M_PE1_TI__ENABLE,		///< pe1 ti enable
	PE_INF_HW_M14B_M_PE1_DJ__ENABLE,		///< pe1 ti enable
	PE_INF_HW_M14B_M_PE1_CTI_ENABLE,		///< pe1 cti enable
	PE_INF_HW_M14B_M_PE1_YCM_ENABLE,		///< pe1 ycm enable
	PE_INF_HW_M14B_M_PE1_DCE_ENABLE,		///< pe1 dce enable
	PE_INF_HW_M14B_M_PE1_LR_CR_ENAB,		///< pe1 lrcr enable
	PE_INF_HW_M14B_M_PE1_CEN_DETOUR,		///< pe1 cen detour
	PE_INF_HW_M14B_M_PE1_DSE_ENABLE,		///< pe1 dse enable
	PE_INF_HW_M14B_M_PE1_DRC_ENABLE,		///< pe1 drc enable
	PE_INF_HW_M14B_M_PE1_CW__ENABLE,		///< pe1 cw enable
	PE_INF_HW_M14B_M_PE1_CONV_L_ENB,		///< pe1 conv L pxl_rep0_in
	PE_INF_HW_M14B_M_PE1_CONV_L_COL,		///< pe1 conv L pxl_rep0_value
	PE_INF_HW_M14B_M_PE1_CONV_R_ENB,		///< pe1 conv R pxl_rep1_in
	PE_INF_HW_M14B_M_PE1_CONV_R_COL,		///< pe1 conv R pxl_rep1_value
	PE_INF_HW_M14B_M_PE1_CMUX3D_ENB,		///< pe1 conv mux 3d enable
	PE_INF_HW_M14B_M_PE1_CMUX3D_COL,		///< pe1 conv mux 3d enable
	PE_INF_HW_M14B_M_PE1_MUTE_L_ENB,		///< pe1 mute L pxl_rep0_in
	PE_INF_HW_M14B_M_PE1_MUTE_L_COL,		///< pe1 mute L pxl_rep0_value
	PE_INF_HW_M14B_M_PE1_MUTE_R_ENB,		///< pe1 mute R pxl_rep1_in
	PE_INF_HW_M14B_M_PE1_MUTE_R_COL,		///< pe1 mute R pxl_rep1_value
	PE_INF_HW_M14B_M_PE1_MMUX3D_ENB,		///< pe1 mute mux 3d enable
	PE_INF_HW_M14B_M_PE1_MMUX3D_COL,		///< pe1 mute mux 3d enable
	PE_INF_HW_M14B_M_MAX					///< max num
}
PE_INF_HW_M14B_MONITOR_MENU;

/**
 *	pe read default reg menu enumeration
 */
typedef enum
{
	PE_INF_HW_M14A_M_DISPLAY = 0,			///< menu display
	PE_INF_HW_M14A_M_CVA_PATTERN_TYPE,		///< cva pattern_type
	PE_INF_HW_M14A_M_CVA_PATTERN_CSC,		///< cva pattern_csc
	PE_INF_HW_M14A_M_CVA_PATTERN_DETAIL,	///< cva pattern_detail
	PE_INF_HW_M14A_M_CVA_FIR_EN,			///< cva detour_enable
	PE_INF_HW_M14A_M_CVA_FIR_Y_EN,			///< cva cr_luma_mode
	PE_INF_HW_M14A_M_CVA_FIR_C_EN,			///< cva cr_chroma_mode
	PE_INF_HW_M14A_M_CVB_PATTERN_TYPE,		///< cvb pattern_type
	PE_INF_HW_M14A_M_CVB_PATTERN_CSC,		///< cvb pattern_csc
	PE_INF_HW_M14A_M_CVB_PATTERN_DETAIL,	///< cvb pattern_detail
	PE_INF_HW_M14A_M_CVB_FIR_EN,			///< cvb detour_enable
	PE_INF_HW_M14A_M_CVB_FIR_Y_EN,			///< cvb cr_luma_mode
	PE_INF_HW_M14A_M_CVB_FIR_C_EN,			///< cvb cr_chroma_mode
	PE_INF_HW_M14A_M_P0L_DETOUR,			///< p0l detour_enable
	PE_INF_HW_M14A_M_P0L_IPC_Y,				///< p0l cr_luma_mode
	PE_INF_HW_M14A_M_P0L_IPC_C,				///< p0l cr_chroma_mode
	PE_INF_HW_M14A_M_P0R_DETOUR,			///< p0r detour_enable
	PE_INF_HW_M14A_M_P0R_IPC_Y,				///< p0r cr_luma_mode
	PE_INF_HW_M14A_M_P0R_IPC_C,				///< p0r cr_chroma_mode
	PE_INF_HW_M14A_M_MSL_FLT_SEL_Y,			///< msl filter_type_sel_luma
	PE_INF_HW_M14A_M_MSR_FLT_SEL_Y,			///< msr filter_type_sel_luma
	PE_INF_HW_M14A_M_OVL_DARK_W0_EN,		///< ovl w0_dark_en
	PE_INF_HW_M14A_M_OVL_DARK_W1_EN,		///< ovl w1_dark_en
	PE_INF_HW_M14A_M_OVL_DARK_COLOR,		///< ovl dark_color
	PE_INF_HW_M14A_M_OVL_BACK_COLOR,		///< ovl back_color
	PE_INF_HW_M14A_M_OVR_DARK_W0_EN,		///< ovr w0_dark_en
	PE_INF_HW_M14A_M_OVR_DARK_W1_EN,		///< ovr w1_dark_en
	PE_INF_HW_M14A_M_OVR_DARK_COLOR,		///< ovr dark_color
	PE_INF_HW_M14A_M_OVR_BACK_COLOR,		///< ovr back_color
	PE_INF_HW_M14A_M_PE1_DETOUR,			///< pe1 detour_enable
	PE_INF_HW_M14A_M_PE1_REP0_IN_EN,		///< pe1 pxl_rep0_in
	PE_INF_HW_M14A_M_PE1_REP0_COLOR,		///< pe1 pxl_rep0_value
	PE_INF_HW_M14A_M_PE1_REP1_IN_EN,		///< pe1 pxl_rep1_in
	PE_INF_HW_M14A_M_PE1_REP1_COLOR,		///< pe1 pxl_rep1_value
	PE_INF_HW_M14A_M_MAX					///< max num
}
PE_INF_HW_M14A_MONITOR_MENU;

/**
 *	pe read led pixel value menu enumeration
 */
typedef enum
{
	PE_INF_HW_M14B_LPV_DISPLAY = 0,			///< menu display
	PE_INF_HW_M14B_LPV_HOR_POS,				///< horizontal position
	PE_INF_HW_M14B_LPV_VER_POS,				///< vertical position
	PE_INF_HW_M14B_LPV_MARK_EN,				///< mark enable
	PE_INF_HW_M14B_LPV_PIXEL_R,				///< pixel red
	PE_INF_HW_M14B_LPV_PIXEL_G,				///< pixel green
	PE_INF_HW_M14B_LPV_PIXEL_B,				///< pixel blue
	PE_INF_HW_M14B_LPV_MAX					///< max num
}
PE_INF_HW_M14A_RD_LED_PV_MENU;

/**
 *	pe control inf
 */
typedef struct
{
	UINT32 pe0_src_h;		///< pe input horizontal size
	UINT32 pe0_src_v;		///< pe input vertical size
	UINT32 scl_src_h;		///< scaler input horizontal size
	UINT32 scl_src_v;		///< scaler input vertical size
	UINT32 scl_resize;		///< scaler resize case, 1:yes, 0:no
	UINT32 scl_hd;			///< scaler hd case, 1:yes, 0:no
	UINT32 i_win_h_ofst;	///< scaler input window horizontal offset
	UINT32 i_win_v_ofst;	///< scaler input window vertical offset
	UINT32 i_win_h_size;	///< scaler input window horizontal size
	UINT32 i_win_v_size;	///< scaler input window vertical size
	UINT32 o_win_h_size;	///< scaler output window horizontal size
	UINT32 o_win_v_size;	///< scaler output window vertical size
	UINT32 m_win_h_ofst;	///< ov m window horizontal offset
	UINT32 m_win_v_ofst;	///< ov m window vertical offset
	UINT32 m_win_h_size;	///< ov m window horizontal size
	UINT32 m_win_v_size;	///< ov m window vertical size
	UINT32 is_yuv444;		///< scaler input color format, 0x1:444,0x0:422or420
	UINT32 det_3df;			///< detected 3d format, see PE_HST_HW_M14_DET_3D_FMT
	UINT32 auto_cr_en;		///< enable LRCR, 1:on, 0:off
	UINT32 sat_status;		///< saturation status
	UINT32 mute_on;			///< mute on, 1:on, 0:off
	UINT32 hv_same;			///< ov m win h,v same for w0, exception case for w1, 1:yes,0:no
	UINT32 ssc_src_h;		///< sub scaler input horizontal size
	UINT32 ssc_src_v;		///< sub scaler input vertical size
	UINT32 ssc_i_win_x;		///< sub scaler input window horizontal offset
	UINT32 ssc_i_win_y;		///< sub scaler input window vertical offset
	UINT32 ssc_i_win_h;		///< sub scaler input window horizontal size
	UINT32 ssc_i_win_v;		///< sub scaler input window vertical size
	UINT32 ssc_o_win_h;		///< sub scaler output window horizontal size
	UINT32 ssc_o_win_v;		///< sub scaler output window vertical size
	UINT32 pxl_rp_st;		///< pixel replace state, 0x0~0x2:off,r,g,b
	UINT32 is_src_frz;		///< src_freeze_state, 0x1:freeze,0x0:normal
	UINT32 pmux_pos;		///< pip mux position, 0x0:after shp, 0x1:before shp
	UINT32 fw_src;			///< fw_src, 0x0:fw off, 0x1:fw on, 0x2~ :see reg menual
}
PE_INF_M14_CTRL_T;

/**
 *	pe inf settings type
 */
typedef struct
{
	LX_PE_INF_DISPLAY_T disp_info[LX_PE_WIN_NUM];		///< display info
	PE_INF_M14_CTRL_T ctrl_info[LX_PE_WIN_NUM];			///< control info
}
PE_INF_M14_SETTINGS_T;

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/
int PE_INF_M14_Init(PE_CFG_CTRL_T *pstParams);
int PE_INF_M14_SetDebugSettings(LX_PE_DBG_SETTINGS_T *pstParams);
int PE_INF_M14_SetDisplayInfo(LX_PE_INF_DISPLAY_T *pstParams);
int PE_INF_M14_GetDisplayInfo(LX_PE_INF_DISPLAY_T *pstParams);
int PE_INF_M14_CheckSclInfo(void);
int PE_INF_M14_GetCurInfSettings(PE_INF_M14_SETTINGS_T *pstParams);
int PE_INF_M14_GetLevelInfo(LX_PE_INF_LEVEL_T *pstParams);
int PE_INF_M14_SetMiscTbl(LX_PE_ETC_TBL_T *pstParams);
int PE_INF_M14_GetMiscTbl(LX_PE_ETC_TBL_T *pstParams);
int PE_INF_M14_SetTraceDataBuf(char *data);
int PE_INF_M14_SetTracePacketData(LX_PE_PKT_T *pstParams);

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _PE_INF_M14_H_ */

