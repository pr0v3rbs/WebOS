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

/** @file pe_fwi_m14.h
 *
 *  fw ipc interface header for pe device. ( used only within kdriver )
 *
 *	@author
 *	@version	0.1
 *	@note
 *	@date		2011.09.20
 *	@see
 */

#ifndef	_PE_FWI_M14_H_
#define	_PE_FWI_M14_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include "pe_kapi.h"
#include "pe_tsk_m14.h"

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
/** 20140110, sj.youm, define for fw dnr data,
*	- if defined, PE_HW_M14B_USE_REG_CTRL : must be defined.
*	- if defined, PE_HW_M14B_USE_DNR_MODE_ON_TASK : not support
*/
#define PE_FWI_M14_ADD_FW_DNR_DATA

#define PE_FWI_M14_BASE_OFFSET				0
#define PE_FWI_M14_TNR_CTRL_OFFSET			PE_FWI_M14_BASE_OFFSET
#define PE_FWI_M14_FW_CTRL_OFFSET			(PE_FWI_M14_TNR_CTRL_OFFSET+sizeof(PE_FWI_M14_TNR_CTRL))
#define PE_FWI_M14_HOR_CTRL_OFFSET			(PE_FWI_M14_FW_CTRL_OFFSET+sizeof(PE_FWI_M14_FW_CTRL))
#define PE_FWI_M14_MISC_CTRL_OFFSET			(PE_FWI_M14_HOR_CTRL_OFFSET+sizeof(PE_FWI_M14_HOR_CTRL))
#define PE_FWI_M14_EDGE_GAIN_OFFSET			(PE_FWI_M14_MISC_CTRL_OFFSET+sizeof(PE_FWI_M14_MISC_CTRL))
#define PE_FWI_M14_TEXTURE_GAIN_OFFSET		(PE_FWI_M14_EDGE_GAIN_OFFSET+sizeof(PE_FWI_M14_EDGE_GAIN))
#define PE_FWI_M14_SRC_INFO_OFFSET			(PE_FWI_M14_TEXTURE_GAIN_OFFSET+sizeof(PE_FWI_M14_TEXTURE_GAIN))
#define PE_FWI_M14_CEN_CTRL_OFFSET			(PE_FWI_M14_SRC_INFO_OFFSET+sizeof(PE_FWI_M14_SRC_INFO))
#define PE_FWI_M14_DNR_COMMON_OFFSET		(PE_FWI_M14_CEN_CTRL_OFFSET+sizeof(PE_FWI_M14_CEN_CTRL))
#define PE_FWI_M14_DNR_DETAIL_OFFSET		(PE_FWI_M14_DNR_COMMON_OFFSET+sizeof(PE_FWI_M14_DNR_COMMON))

#define PE_FWI_M14_REFRESH_TBL_OFST			PE_FWI_M14_BASE_OFFSET
#ifdef PE_FWI_M14_ADD_FW_DNR_DATA
#define PE_FWI_M14_REFRESH_TBL_SIZE			(PE_FWI_M14_DNR_DETAIL_OFFSET+sizeof(PE_FWI_M14_DNR_DETAIL))
#else
#define PE_FWI_M14_REFRESH_TBL_SIZE			(PE_FWI_M14_SRC_INFO_OFFSET+sizeof(PE_FWI_M14_SRC_INFO))
#endif

#define PE_FWI_M14_CEN_RGN_NUM		16

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/

/**
 * PE tnr core ctrl
 * PQ Firmware IPC Control
 */
typedef struct {
	UINT8 tnr_en;			///< tnr core enable, 0:off, 1:on
	UINT8 m_fir_en;			///< tnr motion fir enable, 0:off, 1:on
}
PE_FWI_M14_TNR_CTRL;

/**
 * PE FW ctrl
 * PQ Firmware IPC Control
 */
typedef struct {
	UINT8 enable;			///< fw control enable, 1:on, 0:off
}
PE_FWI_M14_FW_CTRL;

/**
 * PE Sharpness horizontal control
 * PQ Firmware IPC Control
 */
typedef struct {
	UINT8 e_gain_th1;		///< 0x5474 pe1_derh_ctrl_0b[7:0],	e_gain_th1
	UINT8 e_gain_th2;		///< 0x5474 pe1_derh_ctrl_0b[15:8],	e_gain_th2
}
PE_FWI_M14_HOR_CTRL;

/**
 * PE Sharpness misc texture-to-weight control
 * PQ Firmware IPC Control
 */
typedef struct {
	UINT8 gt_th0;			///< 0x5574 pe1_coring_ctrl_05[15:8],	gt_th0
	UINT8 gt_th1;			///< 0x5574 pe1_coring_ctrl_05[23:16],	gt_th1
	UINT8 gt_th0a;			///< 0x5578 pe1_coring_ctrl_06[7:0],	gt_th0a
	UINT8 gt_th0b;			///< 0x5578 pe1_coring_ctrl_06[15:8],	gt_th0b
}
PE_FWI_M14_MISC_CTRL;

/**
 * PE Sharpness edge gain control
 * PQ Firmware IPC Control
 */
typedef struct {
	UINT8 co09_gain_b;		///< 0x5584 pe1_coring_ctrl_09[6:0],	edge_gain_b
	UINT8 co09_gain_w;		///< 0x5584 pe1_coring_ctrl_09[14:8],	edge_gain_w
	UINT8 dp01_gain_b;		///< 0x5414 pe1_dp_ctrl_01[6:0],		edge_gain_b
	UINT8 dp01_gain_w;		///< 0x5414 pe1_dp_ctrl_01[14:8],		edge_gain_w
}
PE_FWI_M14_EDGE_GAIN;

/**
 * PE Sharpness texture gain control
 * PQ Firmware IPC Control
 */
typedef struct {
	UINT8 co09_gain_b;		///< 0x5584 pe1_coring_ctrl_09[22:16],	texture_gain_b
    UINT8 co09_gain_w;		///< 0x5584 pe1_coring_ctrl_09[30:24],	texture_gain_w
	UINT8 dp01_gain_b;		///< 0x5414 pe1_dp_ctrl_01[22:16],		texture_gain_b
    UINT8 dp01_gain_w;		///< 0x5414 pe1_dp_ctrl_01[30:24],		texture_gain_w
}
PE_FWI_M14_TEXTURE_GAIN;

/**
 * PE SRC Info
 * PQ Firmware IPC Control
 */
typedef struct {
	UINT8 dtv_mode;	//0:normal,1:file,2:hdd,3:photo,4:tp,5:camera,6:invalid, see LX_PE_DTV_TYPE
	UINT8 hdd_mode;	//0:dtv, 1,2:atv60,50, 3,4:av60,50, 5:scartrgb, 6:invalid, see LX_PE_HDD_SRC_TYPE
}
PE_FWI_M14_SRC_INFO;

/**
 * PE cen Region control
 * PQ Firmware IPC Control
 */
typedef struct {
	UINT8 rgn_num;		///< region number, 0~15
	UINT8 delta_h;		///< region delta(offset), -128 ~ 127, h
	UINT8 delta_s;		///< region delta(offset), -128 ~ 127, s
	UINT8 delta_v;		///< region delta(offset), -128 ~ 127, v
	UINT8 delta_g;		///< region delta(offset), -128 ~ 127, g
	UINT8 delta_b;		///< region delta(offset), -128 ~ 127, b
	UINT8 delta_r;		///< region delta(offset), -128 ~ 127, r
	UINT8 ma_gain;		///< region master gain, 0~128~255
}
PE_FWI_M14_CEN_CTRL;

/**
 * PE dnr common control, see LX_PE_NRD_DNR4_CMN_T (for M14BX)
 * PQ Firmware IPC Control
 */
typedef struct
{
	/* dc bnr 1 */
	UINT8 reg_dc_bnr_chromagain;	///< p0d_dc_bnr_ctrl_2, 23:16, chromagain
	/* ac bnr 4 */
	UINT8 reg_bnr_ac_h_en;			///< p0d_ac_bnr_ctrl_0, 0, ac bnr h en
	UINT8 reg_bnr_ac_v_en;			///< p0d_ac_bnr_ctrl_0, 1, ac bnr v en
	UINT8 reg_bnr_ac_h_chroma_en;	///< p0d_ac_bnr_ctrl_0, 2, ac bnr h ch en
	UINT8 reg_bnr_ac_v_chroma_en;	///< p0d_ac_bnr_ctrl_0, 3, ac bnr v ch en
}
PE_FWI_M14_DNR_COMMON;

/**
 * PE dnr detail control, see LX_PE_NRD_DNR4_DETAIL_T (for M14BX)
 * PQ Firmware IPC Control
 */
typedef struct
{
	/* dc bnr 6 */
	UINT8 reg_dc_detail_en;			///< p0d_dc_bnr_ctrl_2, 3, dc_detail_en
	UINT8 reg_dc_blur_sel;			///< p0d_dc_bnr_ctrl_0, 2, dc_blur_sel
	UINT8 reg_var_v_gain;			///< p0d_dc_bnr_ctrl_1, 23:20, var_v_gain
	UINT8 reg_var_h_gain;			///< p0d_dc_bnr_ctrl_1, 27:24, var_h_gain
	UINT8 reg_var_cut_resolution;	///< p0d_dc_bnr_ctrl_1, 31:28, var_cut_resolution
	UINT8 reg_dc_protection_th;		///< p0d_dc_bnr_ctrl_2, 31:24, dc_protection_th
	/* ac bnr 16 */
	UINT8 reg_bnr_ac_detail_th1;	///< p0d_ac_bnr_ctrl_5, 31:24, bnr_ac_detail_th1
	UINT8 reg_bnr_ac_detail_th2;	///< p0d_ac_bnr_ctrl_5, 23:16, bnr_ac_detail_th2
	UINT8 reg_bnr_ac_detail_th3;	///< p0d_ac_bnr_ctrl_5, 15: 8, bnr_ac_detail_th3
	UINT8 reg_bnr_ac_detail_th4;	///< p0d_ac_bnr_ctrl_5,  7: 0, bnr_ac_detail_th4
	UINT8 reg_bnr_ac_pos_gain_h0;	///< p0d_ac_bnr_ctrl_6, 23:16, bnr_ac_pos_gain_h0
	UINT8 reg_bnr_ac_pos_gain_h1;	///< p0d_ac_bnr_ctrl_6, 15: 8, bnr_ac_pos_gain_h1
	UINT8 reg_bnr_ac_pos_gain_h2;	///< p0d_ac_bnr_ctrl_6,  7: 0, bnr_ac_pos_gain_h2
	UINT8 reg_bnr_ac_pos_gain_h3;	///< p0d_ac_bnr_ctrl_7, 31:24, bnr_ac_pos_gain_h3
	UINT8 reg_bnr_ac_pos_gain_l0;	///< p0d_ac_bnr_ctrl_7, 23:16, bnr_ac_pos_gain_l0
	UINT8 reg_bnr_ac_pos_gain_l1;	///< p0d_ac_bnr_ctrl_7, 15: 8, bnr_ac_pos_gain_l1
	UINT8 reg_bnr_ac_pos_gain_l2;	///< p0d_ac_bnr_ctrl_7,  7: 0, bnr_ac_pos_gain_l2
	UINT8 reg_bnr_ac_pos_gain_l3;	///< p0d_ac_bnr_ctrl_8, 31:24, bnr_ac_pos_gain_l3
	UINT8 reg_bnr_ac_detail_max;	///< p0d_detail_ctrl,  7: 0, bnr_ac_detail_max
	UINT8 reg_bnr_ac_detail_min;	///< p0d_detail_ctrl, 15: 8, bnr_ac_detail_min
	UINT8 reg_bnr_diff_l;			///< p0d_detail_ctrl, 23:16, bnr_diff_l
	UINT8 reg_bnr_diff_p;			///< p0d_detail_ctrl, 31:24, bnr_diff_p
}
PE_FWI_M14_DNR_DETAIL;

/**
 * PQ Firmware IPC Control
 */
typedef struct {
	PE_FWI_M14_TNR_CTRL			tnr_ctrl;		///< tnr ctrl
	PE_FWI_M14_FW_CTRL			fw_ctrl;		///< fwi ctrl
	PE_FWI_M14_HOR_CTRL			shp_h_ctrl;		///< hor ctrl
	PE_FWI_M14_MISC_CTRL		shp_m_ctrl;		///< misc ctrl
	PE_FWI_M14_EDGE_GAIN		shp_e_ctrl;		///< edge ctrl
	PE_FWI_M14_TEXTURE_GAIN		shp_t_ctrl;		///< texture ctrl
	PE_FWI_M14_SRC_INFO			src_info;		///< src info
	PE_FWI_M14_CEN_CTRL			cen_ctrl;		///< cen ctrl
	#ifdef PE_FWI_M14_ADD_FW_DNR_DATA
	PE_FWI_M14_DNR_COMMON		dnr_common;		///< dnr common
	PE_FWI_M14_DNR_DETAIL		dnr_detail;		///< dnr detail
	#endif
}
PE_FWI_M14_PQ_IPC_CTRL;

/**
 *	PE fwi status
 */
typedef struct
{
	UINT32 reset;				///< fw reset
	UINT32 src_info;			///< fw src info
}
PE_FWI_M14_STATUS_T;

/**
 *	pe fwi settings type
 */
typedef struct
{
	PE_FWI_M14_TNR_CTRL			tnr_ctrl[LX_PE_WIN_NUM];		///< tnr ctrl
	PE_FWI_M14_FW_CTRL			fw_ctrl;						///< fwi ctrl
	PE_FWI_M14_HOR_CTRL			shp_h_ctrl;						///< hor ctrl
	PE_FWI_M14_MISC_CTRL		shp_m_ctrl;						///< misc ctrl
	PE_FWI_M14_EDGE_GAIN		shp_e_ctrl;						///< edge ctrl
	PE_FWI_M14_TEXTURE_GAIN		shp_t_ctrl;						///< texture ctrl
	PE_FWI_M14_SRC_INFO			src_info;						///< src info
	PE_FWI_M14_CEN_CTRL			cen_ctrl[LX_PE_CMG_REGION_NUM];	///< cen ctrl
	PE_FWI_M14_DNR_COMMON		dnr_common;						///< dnr common
	PE_FWI_M14_DNR_DETAIL		dnr_detail;						///< dnr detail
	PE_FWI_M14_STATUS_T			fw_status;						///< fw status
}
PE_FWI_M14_SETTINGS_T;

/**
 *	BE fwi cen gain ctrl0 settings type
 *	- should be same with BE_PE_CEN_CTRL0_T struct
 */
typedef struct
{
	UINT32	u8regnNum		:	8,	//	region number 0~15
			u8deltHue		:	8,	//	delta hue, -127 ~ 127
			u8deltSat		:	8,	//	delta saturation, -127 ~ 127
			u8deltVal		:	8;	//	delta value, -127 ~ 127
}
PE_FWI_M14_CEN_CTRL0_T;

/**
 *	BE fwi cen gain ctrl1 settings type
 *	- should be same with BE_PE_CEN_CTRL1_T struct
 */
typedef struct
{
	UINT32	u8deltGrn		:	8,	//	delta green, -127 ~ 127
			u8deltBlu		:	8,	//	delta blue, -127 ~ 127
			u8deltRed		:	8,	//	delta red, -127 ~ 127
			u8gainMst		:	8;	//	master gain, 0 ~ 255
}
PE_FWI_M14_CEN_CTRL1_T;

/**
 * pe fwi push mode parameter.
 */
typedef struct
{
	LX_PE_WIN_ID win_id;	///< window index
}
PE_FWI_M14_PUSH_MODE_T;

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/
void PE_FWI_M14_SetDbgPrint(UINT32 on_off);
UINT32 PE_FWI_M14_GetDbgPrint(void);
int PE_FWI_M14_IPC_Open(void);
int PE_FWI_M14_IPC_Close(void);
int PE_FWI_M14_Init(PE_CFG_CTRL_T *pstParams);
int PE_FWI_M14_SetFwCtrl(PE_FWI_M14_FW_CTRL *pstParams, UINT32 called_on_sync);
int PE_FWI_M14_SetFwCtrlEnable(PE_TSK_M14_CFG_T *pCfg);
int PE_FWI_M14B_CheckFwStatus(PE_TSK_M14_CFG_T *pCfg);
int PE_FWI_M14_SetFwTnrEnable(UINT32 on_off);
int PE_FWI_M14_SetCviCsc(LX_PE_CSC_INPUT_T *pstParams);
int PE_FWI_M14_SetPushMode(UINT32 win_id);
int PE_FWI_M14_SetShpHorCtrl(PE_FWI_M14_HOR_CTRL *pstParams);
int PE_FWI_M14_SetShpMiscCtrl(PE_FWI_M14_MISC_CTRL *pstParams);
int PE_FWI_M14_SetEdgeGainCtrl(PE_FWI_M14_EDGE_GAIN *pstParams);
int PE_FWI_M14_SetTextureGainCtrl(PE_FWI_M14_TEXTURE_GAIN *pstParams);
int PE_FWI_M14_SetTnrCtrl(UINT32 win_id, PE_FWI_M14_TNR_CTRL *pstParams);
int PE_FWI_M14_SetSrcInfo(PE_FWI_M14_SRC_INFO *pstParams);
int PE_FWI_M14_SetCenCtrl(PE_FWI_M14_CEN_CTRL *pstParams);
int PE_FWI_M14_SetDnrCommon(PE_FWI_M14_DNR_COMMON *pstParams);
int PE_FWI_M14_SetDnrDetail(PE_FWI_M14_DNR_DETAIL *pstParams);
int PE_FWI_M14_GetCurFwiSettings(PE_FWI_M14_SETTINGS_T *pstParams);

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _PE_FWI_M14_H_ */

