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

/** @file pe_inf_h15.h
 *
 *  information management for picture enhance. ( used only within kdriver )
 *
 *	@author		Seung-Jun,Youm(sj.youm@lge.com)
 *	@version	0.1
 *	@note
 *	@date		2012.05.02
 *	@see
 */

#ifndef	_PE_INF_H15_H_
#define	_PE_INF_H15_H_

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
/* 20140614, sj.youm, use legacy info */
#undef PE_INF_H15_USE_LEGACY_INFO

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/
/**
 *	pe read general info menu enumeration
 */
typedef enum
{
	PE_INF_HW_H15A_GINFO_DISPLAY = 0,		///< menu display
	PE_INF_HW_H15A_GINFO__KDRV_INFO,		///< kdrv info
	PE_INF_HW_H15A_GINFO_DDR_H_PHYS,		///< ddr header phys
	PE_INF_HW_H15A_GINFO_DDR_H_SHDW,		///< ddr header shdw
	PE_INF_HW_H15A_GINFO_MAX				///< max num
}
PE_INF_HW_H15A_GINFO_MENU;

/**
 *	pe read default reg menu enumeration
 */
typedef enum
{
	PE_INF_HW_H15A_M_DISPLAY = 0,			///< menu display
	PE_INF_HW_H15A_M_CVA_TPG___TYPE,		///< pattern_type
	PE_INF_HW_H15A_M_CVA_TPG__C_S_C,		///< pattern_csc
	PE_INF_HW_H15A_M_CVA_TPG_DETAIL,		///< pattern_detail
	PE_INF_HW_H15A_M_CVB_TPG___TYPE,		///< pattern_type
	PE_INF_HW_H15A_M_CVB_TPG__C_S_C,		///< pattern_csc
	PE_INF_HW_H15A_M_CVB_TPG_DETAIL,		///< pattern_detail
	PE_INF_HW_H15A_M_CVC_TPG___TYPE,		///< pattern_type
	PE_INF_HW_H15A_M_CVC_TPG__C_S_C,		///< pattern_csc
	PE_INF_HW_H15A_M_CVC_TPG_DETAIL,		///< pattern_detail
	PE_INF_HW_H15A_M_CVD_TPG___TYPE,		///< pattern_type
	PE_INF_HW_H15A_M_CVD_TPG__C_S_C,		///< pattern_csc
	PE_INF_HW_H15A_M_CVD_TPG_DETAIL,		///< pattern_detail
	PE_INF_HW_H15A_M_ND1_DETOUR_ENB,		///< detour_enable
	PE_INF_HW_H15A_M_ND1_TNR_ENABLE,		///< tnr enable
	PE_INF_HW_H15A_M_ND1_IPC_Y_MODE,		///< cr_luma_mode
	PE_INF_HW_H15A_M_ND1_IPC_C_MODE,		///< cr_chroma_mode
	PE_INF_HW_H15A_M_ND1_FMD_ENABLE,		///< film mode enable
	PE_INF_HW_H15A_M_ND1_CLC_FLT_EN,		///< clc filter enable
	PE_INF_HW_H15A_M_ND2_DETOUR_ENB,		///< detour_enable
	PE_INF_HW_H15A_M_ND2_TNR_ENABLE,		///< tnr enable
	PE_INF_HW_H15A_M_ND2_IPC_Y_MODE,		///< cr_luma_mode
	PE_INF_HW_H15A_M_ND2_IPC_C_MODE,		///< cr_chroma_mode
	PE_INF_HW_H15A_M_ND2_FMD_ENABLE,		///< film mode enable
	PE_INF_HW_H15A_M_ND2_CLC_FLT_EN,		///< clc filter enable
	PE_INF_HW_H15A_M_NDS_DETOUR_ENB,		///< detour_enable
	PE_INF_HW_H15A_M_NDS_TNR_ENABLE,		///< tnr enable
	PE_INF_HW_H15A_M_NDS_IPC_Y_MODE,		///< cr_luma_mode
	PE_INF_HW_H15A_M_NDS_IPC_C_MODE,		///< cr_chroma_mode
	PE_INF_HW_H15A_M_NDS_FMD_ENABLE,		///< film mode enable
	PE_INF_HW_H15A_M_M1Y_BILIN_ENBL,		///< bilinear enable
	PE_INF_HW_H15A_M_M1Y_SAMPL_MODE,		///< sampling mode
	PE_INF_HW_H15A_M_M2Y_BILIN_ENBL,		///< bilinear enable
	PE_INF_HW_H15A_M_M2Y_SAMPL_MODE,		///< sampling mode
	PE_INF_HW_H15A_M_M1C_BILIN_ENBL,		///< bilinear enable
	PE_INF_HW_H15A_M_M1C_SAMPL_MODE,		///< sampling mode
	PE_INF_HW_H15A_M_M2C_BILIN_ENBL,		///< bilinear enable
	PE_INF_HW_H15A_M_M2C_SAMPL_MODE,		///< sampling mode
	PE_INF_HW_H15A_M_SSC_BILIN_ENBL,		///< bilinear enable
	PE_INF_HW_H15A_M_SRE_SHP_DETOUR,		///< detour_enable
	PE_INF_HW_H15A_M_SRE_GSC_BILINE,		///< bilinear enable
	PE_INF_HW_H15A_M_SRE_GSC_SAMPLE,		///< sampling mode
	PE_INF_HW_H15A_M_SHPL_DETOUR_EN,		///< detour_enable
	PE_INF_HW_H15A_M_SHPR_DETOUR_EN,		///< detour_enable
	PE_INF_HW_H15A_M_CE_DETOUR_ENBL,		///< detour_enable
	PE_INF_HW_H15A_M_CE_MUTE_L_ENBL,		///< mute L pxl_rep0_in
	PE_INF_HW_H15A_M_CE_MUTE_L_COLR,		///< mute L pxl_rep0_value
	PE_INF_HW_H15A_M_CE_MUTE_R_ENBL,		///< mute R pxl_rep1_in
	PE_INF_HW_H15A_M_CE_MUTE_R_COLR,		///< mute R pxl_rep1_value
	PE_INF_HW_H15A_M_CO_DETOUR_ENBL,		///< detour_enable
	PE_INF_HW_H15A_M_CO_MUTE_L_ENBL,		///< mute L pxl_rep0_in
	PE_INF_HW_H15A_M_CO_MUTE_L_COLR,		///< mute L pxl_rep0_value
	PE_INF_HW_H15A_M_CO_MUTE_R_ENBL,		///< mute R pxl_rep1_in
	PE_INF_HW_H15A_M_CO_MUTE_R_COLR,		///< mute R pxl_rep1_value
	PE_INF_HW_H15A_M_MAX					///< max num
}
PE_INF_HW_H15A_MONITOR_MENU;

/**
 *	pe read read pixel menu enumeration
 */
typedef enum
{
	PE_INF_HW_H15A_RD_PXL_DISPLAY = 0,		///< menu display
	PE_INF_HW_H15A_RD_PXL_WCP_HOR_POS,		///< wcp horizontal position
	PE_INF_HW_H15A_RD_PXL_WCP_VER_POS,		///< wcp vertical position
	PE_INF_HW_H15A_RD_PXL_WCP_MARK_EN,		///< wcp mark enable
	PE_INF_HW_H15A_RD_PXL_WCP_PXL_E_Y,		///< wcp pixel even y
	PE_INF_HW_H15A_RD_PXL_WCP_PXL_E_B,		///< wcp pixel even cb
	PE_INF_HW_H15A_RD_PXL_WCP_PXL_E_R,		///< wcp pixel even cr
	PE_INF_HW_H15A_RD_PXL_WCP_PXL_O_Y,		///< wcp pixel odd y
	PE_INF_HW_H15A_RD_PXL_WCP_PXL_O_B,		///< wcp pixel odd cb
	PE_INF_HW_H15A_RD_PXL_WCP_PXL_O_R,		///< wcp pixel odd cr
	PE_INF_HW_H15A_RD_PXL_LED_HOR_POS,		///< led horizontal position
	PE_INF_HW_H15A_RD_PXL_LED_VER_POS,		///< led vertical position
	PE_INF_HW_H15A_RD_PXL_LED_MARK_EN,		///< led mark enable
	PE_INF_HW_H15A_RD_PXL_LED_PIXEL_R,		///< led pixel red
	PE_INF_HW_H15A_RD_PXL_LED_PIXEL_G,		///< led pixel green
	PE_INF_HW_H15A_RD_PXL_LED_PIXEL_B,		///< led pixel blue
	PE_INF_HW_H15A_RD_MAX					///< max num
}
PE_INF_HW_H15A_RD_PXL_MENU;

/**
 *	pe control inf
 */
typedef struct
{
	UINT32 pe0_src_h;		///< pe input horizontal size
	UINT32 pe0_src_v;		///< pe input vertical size
	UINT32 i_sc_type;		///< pe input scan type(nd), 1:p, 0:i
	UINT32 u_i_win_h_ofst;	///< user in window horizontal offset
	UINT32 u_i_win_v_ofst;	///< user in window vertical offset
	UINT32 u_i_win_h_size;	///< user in window horizontal size
	UINT32 u_i_win_v_size;	///< user in window vertical size
	UINT32 u_o_win_h_ofst;	///< user out window horizontal offset
	UINT32 u_o_win_v_ofst;	///< user out window vertical offset
	UINT32 u_o_win_h_size;	///< user out window horizontal size
	UINT32 u_o_win_v_size;	///< user out window vertical size
	UINT32 i_c_format;		///< input color format(nd), 0x6:444,0x5:422, else 420
	UINT32 det_3df;			///< detected 3d format, see PE_HST_HW_H15_DET_3D_FMT
	UINT32 auto_cr_en;		///< enable LRCR, 1:on, 0:off
	UINT32 sat_status;		///< saturation status
	UINT32 mute_on;			///< mute on, 1:on, 0:off
	UINT32 hv_same;			///< ctrl0 is_split_mode, ctrl1 is_resize, 1:yes,
	UINT32 pxl_rp_st;		///< pixel replace state, 0x0~0x2:off,r,g,b
	UINT32 is_src_frz;		///< src_freeze_state, 0x1:freeze,0x0:normal
	UINT32 pmux_pos;		///< pip mux position, 0x0:after shp, 0x1:before shp
	UINT32 fw_src;			///< fw_src, 0x0:fw off, 0x1:fw on, 0x2~ :see reg menual
	#ifdef PE_INF_H15_USE_LEGACY_INFO
	UINT32 scl_src_h;		///< scaler input horizontal size
	UINT32 scl_src_v;		///< scaler input vertical size
	UINT32 scl_resize;		///< scaler resize case, 1:yes, 0:no
	UINT32 scl_hd;			///< scaler hd case, 1:yes, 0:no
	UINT32 i_win_h_ofst;	///< scaler input window horizontal offset
	UINT32 i_win_v_ofst;	///< scaler input window vertical offset
	UINT32 i_win_h_size;	///< scaler input window horizontal size
	UINT32 i_win_v_size;	///< scaler input window vertical size
	UINT32 o_win_h_size;	///< scaler output window horizontal size
	UINT32 o_win_v_size;	///< scaler output window vertical size0:no
	UINT32 ssc_src_h;		///< sub scaler input horizontal size
	UINT32 ssc_src_v;		///< sub scaler input vertical size
	UINT32 ssc_i_win_x;		///< sub scaler input window horizontal offset
	UINT32 ssc_i_win_y;		///< sub scaler input window vertical offset
	UINT32 ssc_i_win_h;		///< sub scaler input window horizontal size
	UINT32 ssc_i_win_v;		///< sub scaler input window vertical size
	UINT32 ssc_o_win_h;		///< sub scaler output window horizontal size
	UINT32 ssc_o_win_v;		///< sub scaler output window vertical size
	#endif
}
PE_INF_H15_CTRL_T;

/**
 *	pe inf settings type
 */
typedef struct
{
	LX_PE_INF_DISPLAY_T disp_info[LX_PE_WIN_NUM];		///< display info
	PE_INF_H15_CTRL_T ctrl_info[LX_PE_WIN_NUM];			///< control info
}
PE_INF_H15_SETTINGS_T;

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/
int PE_INF_H15_Init(PE_CFG_CTRL_T *pstParams);
int PE_INF_H15_SetDebugSettings(LX_PE_DBG_SETTINGS_T *pstParams);
int PE_INF_H15_SetDisplayInfo(LX_PE_INF_DISPLAY_T *pstParams);
int PE_INF_H15_GetDisplayInfo(LX_PE_INF_DISPLAY_T *pstParams);
int PE_INF_H15_CheckSclInfo(void);
int PE_INF_H15_GetCurInfSettings(PE_INF_H15_SETTINGS_T *pstParams);
int PE_INF_H15_GetLevelInfo(LX_PE_INF_LEVEL_T *pstParams);
int PE_INF_H15_SetMiscTbl(LX_PE_ETC_TBL_T *pstParams);
int PE_INF_H15_GetMiscTbl(LX_PE_ETC_TBL_T *pstParams);
int PE_INF_H15_SetTraceDataBuf(char *data);
int PE_INF_H15_SetTracePacketData(LX_PE_PKT_T *pstParams);
int PE_INF_H15_GetCurDdrTopHeader(void *pstParams);
int PE_INF_H15_GetCurDdrSecHeader(void *top_h, void *pstParams, UINT32 sec_type);

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _PE_INF_H15_H_ */

