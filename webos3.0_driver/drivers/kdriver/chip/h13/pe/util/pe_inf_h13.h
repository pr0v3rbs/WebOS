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

/** @file pe_inf_h13.h
 *
 *  information management for picture enhance. ( used only within kdriver )
 *	
 *	@author		Seung-Jun,Youm(sj.youm@lge.com)
 *	@version	0.1
 *	@note		
 *	@date		2012.05.02
 *	@see		
 */

#ifndef	_PE_INF_H13_H_
#define	_PE_INF_H13_H_

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
	PE_INF_HW_H13_M_DISPLAY = 0,			///< menu display
	PE_INF_HW_H13_M_CVA_PATTERN_TYPE,		///< cva pattern_type
	PE_INF_HW_H13_M_CVA_PATTERN_CSC,		///< cva pattern_csc
	PE_INF_HW_H13_M_CVA_PATTERN_DETAIL,		///< cva pattern_detail
	PE_INF_HW_H13_M_CVA_FIR_EN,				///< cva detour_enable
	PE_INF_HW_H13_M_CVA_FIR_Y_EN,			///< cva cr_luma_mode
	PE_INF_HW_H13_M_CVA_FIR_C_EN,			///< cva cr_chroma_mode
	PE_INF_HW_H13_M_CVB_PATTERN_TYPE,		///< cvb pattern_type
	PE_INF_HW_H13_M_CVB_PATTERN_CSC,		///< cvb pattern_csc
	PE_INF_HW_H13_M_CVB_PATTERN_DETAIL,		///< cvb pattern_detail
	PE_INF_HW_H13_M_CVB_FIR_EN,				///< cvb detour_enable
	PE_INF_HW_H13_M_CVB_FIR_Y_EN,			///< cvb cr_luma_mode
	PE_INF_HW_H13_M_CVB_FIR_C_EN,			///< cvb cr_chroma_mode
	PE_INF_HW_H13_M_P0L_DETOUR,				///< p0l detour_enable
	PE_INF_HW_H13_M_P0L_IPC_Y,				///< p0l cr_luma_mode
	PE_INF_HW_H13_M_P0L_IPC_C,				///< p0l cr_chroma_mode
	PE_INF_HW_H13_M_P0R_DETOUR,				///< p0r detour_enable
	PE_INF_HW_H13_M_P0R_IPC_Y,				///< p0r cr_luma_mode
	PE_INF_HW_H13_M_P0R_IPC_C,				///< p0r cr_chroma_mode
	PE_INF_HW_H13_M_MSL_FLT_SEL_Y,			///< msl filter_type_sel_luma
	PE_INF_HW_H13_M_MSR_FLT_SEL_Y,			///< msr filter_type_sel_luma
	PE_INF_HW_H13_M_OVL_DARK_W0_EN,			///< ovl w0_dark_en
	PE_INF_HW_H13_M_OVL_DARK_W1_EN,			///< ovl w1_dark_en
	PE_INF_HW_H13_M_OVL_DARK_COLOR,			///< ovl dark_color
	PE_INF_HW_H13_M_OVL_BACK_COLOR,			///< ovl back_color
	PE_INF_HW_H13_M_OVR_DARK_W0_EN,			///< ovr w0_dark_en
	PE_INF_HW_H13_M_OVR_DARK_W1_EN,			///< ovr w1_dark_en
	PE_INF_HW_H13_M_OVR_DARK_COLOR,			///< ovr dark_color
	PE_INF_HW_H13_M_OVR_BACK_COLOR,			///< ovr back_color
	PE_INF_HW_H13_M_PE1_DETOUR,				///< pe1 detour_enable
	PE_INF_HW_H13_M_PE1_REP0_IN_EN,			///< pe1 pxl_rep0_in
	PE_INF_HW_H13_M_PE1_REP0_COLOR,			///< pe1 pxl_rep0_value
	PE_INF_HW_H13_M_PE1_REP1_IN_EN,			///< pe1 pxl_rep1_in
	PE_INF_HW_H13_M_PE1_REP1_COLOR,			///< pe1 pxl_rep1_value
	PE_INF_HW_H13_M_MAX						///< max num
}
PE_INF_HW_H13_MONITOR_MENU;

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
	UINT32 det_3df;			///< detected 3d format, see PE_HST_HW_H13_DET_3D_FMT
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
}
PE_INF_H13_CTRL_T;

/**
 *	pe inf settings type
 */
typedef struct
{
	LX_PE_INF_DISPLAY_T disp_info[LX_PE_WIN_NUM];	///< display info
	PE_INF_H13_CTRL_T ctrl_info[LX_PE_WIN_NUM];		///< control info
}
PE_INF_H13_SETTINGS_T;

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/
int PE_INF_H13_Init(PE_CFG_CTRL_T *pstParams);
int PE_INF_H13_SetDebugSettings(LX_PE_DBG_SETTINGS_T *pstParams);
int PE_INF_H13_SetDisplayInfo(LX_PE_INF_DISPLAY_T *pstParams);
int PE_INF_H13_GetDisplayInfo(LX_PE_INF_DISPLAY_T *pstParams);
int PE_INF_H13_CheckSclInfo(void);
int PE_INF_H13_GetCurInfSettings(PE_INF_H13_SETTINGS_T *pstParams);
int PE_INF_H13_GetLevelInfo(LX_PE_INF_LEVEL_T *pstParams);
int PE_INF_H13_SetMiscTbl(LX_PE_ETC_TBL_T *pstParams);
int PE_INF_H13_GetMiscTbl(LX_PE_ETC_TBL_T *pstParams);

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _PE_INF_H13_H_ */

