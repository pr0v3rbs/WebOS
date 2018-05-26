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

/** @file pe_fwi_h15.h
 *
 *  fw ipc interface header for pe device. ( used only within kdriver )
 *
 *	@author
 *	@version	0.1
 *	@note
 *	@date		2011.09.20
 *	@see
 */

#ifndef	_PE_FWI_H15_H_
#define	_PE_FWI_H15_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include "pe_kapi.h"
#include "pe_tsk_h15.h"

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
/** 20140110, sj.youm, define for fw dnr data, */
#define PE_FWI_H15_ADD_FW_DNR_DATA

#define PE_FWI_H15_BASE_OFFSET				0
#define PE_FWI_H15_TNR_CTRL_OFFSET			PE_FWI_H15_BASE_OFFSET
#define PE_FWI_H15_FW_CTRL_OFFSET			(PE_FWI_H15_TNR_CTRL_OFFSET+sizeof(PE_FWI_H15_TNR_CTRL))
#define PE_FWI_H15_SRC_INFO_OFFSET			(PE_FWI_H15_FW_CTRL_OFFSET+sizeof(PE_FWI_H15_FW_CTRL))
#define PE_FWI_H15_DNR_COMMON_OFFSET		(PE_FWI_H15_SRC_INFO_OFFSET+sizeof(PE_FWI_H15_SRC_INFO))
#define PE_FWI_H15_DNR_DETAIL_OFFSET		(PE_FWI_H15_DNR_COMMON_OFFSET+sizeof(PE_FWI_H15_DNR_COMMON))

#define PE_FWI_H15_REFRESH_TBL_OFST			PE_FWI_H15_BASE_OFFSET
#ifdef PE_FWI_H15_ADD_FW_DNR_DATA
#define PE_FWI_H15_REFRESH_TBL_SIZE			(PE_FWI_H15_DNR_DETAIL_OFFSET+sizeof(PE_FWI_H15_DNR_DETAIL))
#else
#define PE_FWI_H15_REFRESH_TBL_SIZE			(PE_FWI_H15_SRC_INFO_OFFSET+sizeof(PE_FWI_H15_SRC_INFO))
#endif

#if 0	//not_used
#define PE_FWI_H15_HOR_CTRL_OFFSET			(PE_FWI_H15_FW_CTRL_OFFSET+sizeof(PE_FWI_H15_FW_CTRL))
#define PE_FWI_H15_MISC_CTRL_OFFSET			(PE_FWI_H15_HOR_CTRL_OFFSET+sizeof(PE_FWI_H15_HOR_CTRL))
#define PE_FWI_H15_EDGE_GAIN_OFFSET			(PE_FWI_H15_MISC_CTRL_OFFSET+sizeof(PE_FWI_H15_MISC_CTRL))
#define PE_FWI_H15_TEXTURE_GAIN_OFFSET		(PE_FWI_H15_EDGE_GAIN_OFFSET+sizeof(PE_FWI_H15_EDGE_GAIN))
#define PE_FWI_H15_CEN_CTRL_OFFSET			(PE_FWI_H15_TEXTURE_GAIN_OFFSET+sizeof(PE_FWI_H15_TEXTURE_GAIN))
#endif

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
PE_FWI_H15_TNR_CTRL;

/**
 * PE FW ctrl
 * PQ Firmware IPC Control
 */
typedef struct {
	UINT8 enable;			///< fw control enable, 1:on, 0:off
}
PE_FWI_H15_FW_CTRL;

/**
 * PE SRC Info
 * PQ Firmware IPC Control
 */
typedef struct {
	UINT8 dtv_mode;	//0:normal,1:file,2:hdd,3:photo,4:tp,5:camera,6:invalid, see LX_PE_DTV_TYPE
	UINT8 hdd_mode;	//0:dtv, 1,2:atv60,50, 3,4:av60,50, 5:scartrgb, 6:invalid, see LX_PE_HDD_SRC_TYPE
}
PE_FWI_H15_SRC_INFO;

/**
 * PE Sharpness horizontal control
 * PQ Firmware IPC Control
 */
typedef struct {
	UINT8 e_gain_th1;		///< 0x5474 pe1_derh_ctrl_0b[7:0],	e_gain_th1
	UINT8 e_gain_th2;		///< 0x5474 pe1_derh_ctrl_0b[15:8],	e_gain_th2
}
PE_FWI_H15_HOR_CTRL;

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
PE_FWI_H15_MISC_CTRL;

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
PE_FWI_H15_EDGE_GAIN;

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
PE_FWI_H15_TEXTURE_GAIN;

#if 1	//not_used
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
PE_FWI_H15_CEN_CTRL;

/* see LX_PE_NRD_DNR6_CMN_DATA_SIZE */
#define PE_FWI_H15_DNR_CMN_SIZE	10
/**
 *	pe digital noise reduction common control parameter type
 *	ver.6, for H15
 *  001._[MAX_CTRL]_0xC8007194_[0____]_1_UINT8__common[00]:_dnr_max_ctrl,__reg_dnr_max_enable,
 *  002._[DC_BNR__]_0xC80071BC_[0____]_1_UINT8__common[01]:_dc_bnr_ctrl_0,_reg_dc_bnr_enable,
 *  003._[DC_BNR__]_0xC80071C4_[15:10]_6_UINT8__common[02]:_dc_bnr_ctrl_2,_reg_dc_bnr_mastergain,
 *  004._[DC_BNR__]_0xC80071C4_[23:16]_8_UINT8__common[03]:_dc_bnr_ctrl_2,_reg_dc_bnr_chromagain,
 *  005._[AC_BNR__]_0xC80071D4_[0____]_1_UINT8__common[04]:_ac_bnr_ctrl_0,_reg_bnr_ac_h_en,
 *  006._[AC_BNR__]_0xC80071D4_[1____]_1_UINT8__common[05]:_ac_bnr_ctrl_0,_reg_bnr_ac_v_en,
 *  007._[AC_BNR__]_0xC80071D4_[2____]_1_UINT8__common[06]:_ac_bnr_ctrl_0,_reg_bnr_ac_h_chroma_en,
 *  008._[AC_BNR__]_0xC80071D4_[3____]_1_UINT8__common[07]:_ac_bnr_ctrl_0,_reg_bnr_ac_v_chroma_en,
 *  009._[AC_BNR__]_0xC80071D4_[5:4__]_2_UINT8__common[08]:_ac_bnr_ctrl_0,_reg_bnr_ac_acness_resol_h,
 *  010._[AC_BNR__]_0xC8007204_[15:10]_6_UINT8__common[09]:_ac_bnr_ctrl_9,_reg_ac_master_gain,
 *  011._[MNR_____]_0xC80071A0_[0____]_1_UINT8__common[10]:_mnr_ctrl_0,____reg_mnr_enable,
 *  012._[MNR_____]_0xC80071A0_[15:8_]_8_UINT8__common[11]:_mnr_ctrl_0,____reg_mnr_master_gain,
 *  013._[MNR_____]_0xC80071A0_[23:16]_8_UINT8__common[12]:_mnr_ctrl_0,____reg_chroma_master_gain,
 *  014._[MNR_____]_0xC80071A8_[23:16]_8_UINT8__common[13]:_mnr_ctrl_2,____reg_mnr_v_gain,
 *  015._[MNR_____]_0xC80071A8_[31:24]_8_UINT8__common[14]:_mnr_ctrl_2,____reg_mnr_h_gain,
 */
typedef struct
{
	UINT8 data[PE_FWI_H15_DNR_CMN_SIZE];	///< dnr data
}
PE_FWI_H15_DNR_COMMON;

/* see LX_PE_NRD_DNR6_DTL_DCBNR_SIZE, LX_PE_NRD_DNR6_DTL_ACBNR_SIZE, LX_PE_NRD_DNR6_DTL_MSQNR_SIZE */
#define PE_FWI_H15_DNR_DCB_SIZE	30
#define PE_FWI_H15_DNR_ACB_SIZE	28
/**
 *	pe digital noise reduction detail control parameter type
 *	ver.6, for H15
 *  016._[DC_BNR__]_0xC80071C4_[0____]_1_UINT8__dc_bnr[00]:_dc_bnr_ctrl_2,_reg_dc_var_en,
 *  017._[DC_BNR__]_0xC80071C4_[1____]_1_UINT8__dc_bnr[01]:_dc_bnr_ctrl_2,_reg_dc_motion_en,
 *  018._[DC_BNR__]_0xC80071C4_[2____]_1_UINT8__dc_bnr[02]:_dc_bnr_ctrl_2,_reg_dc_protection_en,
 *  019._[DC_BNR__]_0xC80071C4_[3____]_1_UINT8__dc_bnr[03]:_dc_bnr_ctrl_2,_reg_dc_detail_en,
 *  020._[DC_BNR__]_0xC80071BC_[3:2__]_2_UINT8__dc_bnr[04]:_dc_bnr_ctrl_0,_reg_dc_blur_sel,
 *  021._[DC_BNR__]_0xC80071BC_[23:16]_8_UINT8__dc_bnr[05]:_dc_bnr_ctrl_0,_reg_dc_motion_max,
 *  022._[DC_BNR__]_0xC80071BC_[31:24]_8_UINT8__dc_bnr[06]:_dc_bnr_ctrl_0,_reg_dc_motion_min,
 *  023._[DC_BNR__]_0xC80071C0_[7:0__]_8_UINT8__dc_bnr[07]:_dc_bnr_ctrl_1,_reg_dc_detail_max,
 *  024._[DC_BNR__]_0xC80071C0_[15:8_]_8_UINT8__dc_bnr[08]:_dc_bnr_ctrl_1,_reg_dc_detail_min,
 *  025._[DC_BNR__]_0xC80071C0_[23:20]_4_UINT8__dc_bnr[09]:_dc_bnr_ctrl_1,_reg_var_v_gain,
 *  026._[DC_BNR__]_0xC80071C0_[27:24]_4_UINT8__dc_bnr[10]:_dc_bnr_ctrl_1,_reg_var_h_gain,
 *  027._[DC_BNR__]_0xC80071C0_[31:28]_4_UINT8__dc_bnr[11]:_dc_bnr_ctrl_1,_reg_var_cut_resolution,
 *  028._[DC_BNR__]_0xC80071C4_[7:4__]_4_UINT8__dc_bnr[12]:_dc_bnr_ctrl_2,_reg_dc_global_motion_th,
 *  029._[DC_BNR__]_0xC80071C4_[31:24]_8_UINT8__dc_bnr[13]:_dc_bnr_ctrl_2,_reg_dc_protection_th,
 *  030._[DC_BNR__]_0xC80071C8_[7:0__]_8_UINT8__dc_bnr[14]:_dc_bnr_ctrl_3,_reg_dc_bnr_var_th3,
 *  031._[DC_BNR__]_0xC80071C8_[15:8_]_8_UINT8__dc_bnr[15]:_dc_bnr_ctrl_3,_reg_dc_bnr_var_th2,
 *  032._[DC_BNR__]_0xC80071C8_[23:16]_8_UINT8__dc_bnr[16]:_dc_bnr_ctrl_3,_reg_dc_bnr_var_th1,
 *  033._[DC_BNR__]_0xC80071C8_[31:24]_8_UINT8__dc_bnr[17]:_dc_bnr_ctrl_3,_reg_dc_bnr_var_th0,
 *  034._[DC_BNR__]_0xC80071C4_[8____]_1_UINT8__dc_bnr[18]:_dc_bnr_ctrl_2,_reg_detail_sel,
 *  035._[DETAIL__]_0xC80071B8_[7:0__]_8_UINT8__dc_bnr[19]:_detail_ctrl,___reg_bnr_ac_detail_max,
 *  036._[DETAIL__]_0xC80071B8_[15:8_]_8_UINT8__dc_bnr[20]:_detail_ctrl,___reg_bnr_ac_detail_min,
 *  037._[DETAIL__]_0xC80071B8_[23:16]_8_UINT8__dc_bnr[21]:_detail_ctrl,___reg_bnr_diff_l,
 *  038._[DETAIL__]_0xC80071B8_[31:24]_8_UINT8__dc_bnr[22]:_detail_ctrl,___reg_bnr_diff_p,
 *  039._[SC_BNR__]_0xC800726C_[0____]_1_UINT8__dc_bnr[23]:_sc_bnr_ctrl_0,_reg_sc_bnr_en,
 *  040._[SC_BNR__]_0xC8007280_[7:0__]_8_UINT8__dc_bnr[24]:_sc_bnr_ctrl_5,_reg_pos_gain_3,
 *  041._[SC_BNR__]_0xC8007280_[15:8_]_8_UINT8__dc_bnr[25]:_sc_bnr_ctrl_5,_reg_pos_gain_2,
 *  042._[SC_BNR__]_0xC8007280_[23:16]_8_UINT8__dc_bnr[26]:_sc_bnr_ctrl_5,_reg_pos_gain_1,
 *  043._[TEXTURE_]_0xC800728C_[2:0__]_3_UINT8__dc_bnr[27]:_texture_ctrl_0,reg_texture_cnt_div,
 *  044._[TEXTURE_]_0xC800728C_[15:8_]_8_UINT8__dc_bnr[28]:_texture_ctrl_0,reg_texture_cnt_mul,
 *  045._[TEXTURE_]_0xC800728C_[31:24]_8_UINT8__dc_bnr[29]:_texture_ctrl_0,reg_cross_th,
 *  046._[AC_BNR__]_0xC80071D4_[15:8_]_8_UINT8__ac_bnr[00]:_ac_bnr_ctrl_0,_reg_bnr_ac_diff_min_v_th,
 *  047._[AC_BNR__]_0xC80071D4_[23:16]_8_UINT8__ac_bnr[01]:_ac_bnr_ctrl_0,_reg_bnr_ac_diff_min_h_th,
 *  048._[AC_BNR__]_0xC80071D8_[7:0__]_8_UINT8__ac_bnr[02]:_ac_bnr_ctrl_1,_reg_bnr_ac_global_motion_th,
 *  049._[AC_BNR__]_0xC80071D8_[15:8_]_8_UINT8__ac_bnr[03]:_ac_bnr_ctrl_1,_reg_bnr_ac_h_acness_max,
 *  050._[AC_BNR__]_0xC80071E8_[23:16]_8_UINT8__ac_bnr[04]:_ac_bnr_ctrl_4,_reg_bnr_ac_v_acness_max,
 *  051._[AC_BNR__]_0xC80071D8_[23:16]_8_UINT8__ac_bnr[05]:_ac_bnr_ctrl_1,_reg_bnr_ac_h_acness_min,
 *  052._[AC_BNR__]_0xC80071E8_[31:24]_8_UINT8__ac_bnr[06]:_ac_bnr_ctrl_4,_reg_bnr_ac_v_acness_min,
 *  053._[AC_BNR__]_0xC80071E8_[7:6__]_2_UINT8__ac_bnr[07]:_ac_bnr_ctrl_4,_reg_bnr_ac_acness_resol_v,
 *  054._[AC_BNR__]_0xC80071DC_[7:0__]_8_UINT8__ac_bnr[08]:_ac_bnr_ctrl_2,_reg_bnr_ac_motion_3,
 *  055._[AC_BNR__]_0xC80071DC_[15:8_]_8_UINT8__ac_bnr[09]:_ac_bnr_ctrl_2,_reg_bnr_ac_motion_2,
 *  056._[AC_BNR__]_0xC80071DC_[23:16]_8_UINT8__ac_bnr[10]:_ac_bnr_ctrl_2,_reg_bnr_ac_motion_1,
 *  057._[AC_BNR__]_0xC80071DC_[31:24]_8_UINT8__ac_bnr[11]:_ac_bnr_ctrl_2,_reg_bnr_ac_motion_0,
 *  058._[AC_BNR__]_0xC80071E4_[7:0__]_8_UINT8__ac_bnr[12]:_ac_bnr_ctrl_3,_reg_bnr_ac_motion_y_3,
 *  059._[AC_BNR__]_0xC80071E4_[15:8_]_8_UINT8__ac_bnr[13]:_ac_bnr_ctrl_3,_reg_bnr_ac_motion_y_2,
 *  060._[AC_BNR__]_0xC80071E4_[23:16]_8_UINT8__ac_bnr[14]:_ac_bnr_ctrl_3,_reg_bnr_ac_motion_y_1,
 *  061._[AC_BNR__]_0xC80071E4_[31:24]_8_UINT8__ac_bnr[15]:_ac_bnr_ctrl_3,_reg_bnr_ac_motion_y_0,
 *  062._[AC_BNR__]_0xC80071F4_[31:24]_8_UINT8__ac_bnr[16]:_ac_bnr_ctrl_6,_reg_bnr_ac_detail_gain_th,
 *  063._[AC_BNR__]_0xC80071F0_[7:0__]_8_UINT8__ac_bnr[17]:_ac_bnr_ctrl_5,_reg_bnr_ac_detail_th4,
 *  064._[AC_BNR__]_0xC80071F0_[15:8_]_8_UINT8__ac_bnr[18]:_ac_bnr_ctrl_5,_reg_bnr_ac_detail_th3,
 *  065._[AC_BNR__]_0xC80071F0_[23:16]_8_UINT8__ac_bnr[19]:_ac_bnr_ctrl_5,_reg_bnr_ac_detail_th2,
 *  066._[AC_BNR__]_0xC80071F0_[31:24]_8_UINT8__ac_bnr[20]:_ac_bnr_ctrl_5,_reg_bnr_ac_detail_th1,
 *  067._[AC_BNR__]_0xC80071F8_[31:24]_8_UINT8__ac_bnr[21]:_ac_bnr_ctrl_7,_reg_bnr_ac_pos_gain_h3,
 *  068._[AC_BNR__]_0xC80071F4_[7:0__]_8_UINT8__ac_bnr[22]:_ac_bnr_ctrl_6,_reg_bnr_ac_pos_gain_h2,
 *  069._[AC_BNR__]_0xC80071F4_[15:8_]_8_UINT8__ac_bnr[23]:_ac_bnr_ctrl_6,_reg_bnr_ac_pos_gain_h1,
 *  070._[AC_BNR__]_0xC80071F4_[23:16]_8_UINT8__ac_bnr[24]:_ac_bnr_ctrl_6,_reg_bnr_ac_pos_gain_h0,
 *  071._[AC_BNR__]_0xC80071F8_[7:0__]_8_UINT8__ac_bnr[25]:_ac_bnr_ctrl_7,_reg_bnr_ac_pos_gain_l2,
 *  072._[AC_BNR__]_0xC80071F8_[15:8_]_8_UINT8__ac_bnr[26]:_ac_bnr_ctrl_7,_reg_bnr_ac_pos_gain_l1,
 *  073._[AC_BNR__]_0xC80071F8_[23:16]_8_UINT8__ac_bnr[27]:_ac_bnr_ctrl_7,_reg_bnr_ac_pos_gain_l0,
 *  074._[MNR_____]_0xC80071A0_[5:4__]_2_UINT8__mosqnr[00]:_mnr_ctrl_0,____reg_h_expand,
 *  075._[MNR_____]_0xC80071A0_[6____]_1_UINT8__mosqnr[01]:_mnr_ctrl_0,____reg_gain_flt_size,
 *  076._[MNR_____]_0xC80071A4_[7:0__]_8_UINT8__mosqnr[02]:_mnr_ctrl_1,____reg_mnr_s1_mmd_min,
 *  077._[MNR_____]_0xC80071A4_[15:8_]_8_UINT8__mosqnr[03]:_mnr_ctrl_1,____reg_mnr_s2_ratio_min,
 *  078._[MNR_____]_0xC80071A4_[23:16]_8_UINT8__mosqnr[04]:_mnr_ctrl_1,____reg_mnr_s2_ratio_max,
 *  079._[MNR_____]_0xC80071A4_[31:24]_8_UINT8__mosqnr[05]:_mnr_ctrl_1,____reg_mnr_s2_mmd_min,
 *  080._[MNR_____]_0xC80071AC_[7:0__]_8_UINT8__mosqnr[06]:_mnr_ctrl_3,____reg_filter_x0,
 *  081._[MNR_____]_0xC80071AC_[15:8_]_8_UINT8__mosqnr[07]:_mnr_ctrl_3,____reg_filter_x1,
 *  082._[MNR_____]_0xC80071AC_[23:16]_8_UINT8__mosqnr[08]:_mnr_ctrl_3,____reg_filter_y0,
 *  083._[MNR_____]_0xC80071AC_[31:24]_8_UINT8__mosqnr[09]:_mnr_ctrl_3,____reg_filter_y1,
 *  084._[MNR_____]_0xC80071B0_[0____]_1_UINT8__mosqnr[10]:_mnr_ctrl_4,____reg_motion_mnr_en,
 *  085._[MNR_____]_0xC80071B0_[2:1__]_2_UINT8__mosqnr[11]:_mnr_ctrl_4,____reg_motion_mnr_filter,
 *  086._[MNR_____]_0xC80071B0_[4:3__]_2_UINT8__mosqnr[12]:_mnr_ctrl_4,____reg_motion_mnr_filter_c,
 *  087._[MNR_____]_0xC80071B0_[6:5__]_2_UINT8__mosqnr[13]:_mnr_ctrl_4,____reg_mnr_filter,
 *  088._[MNR_____]_0xC80071B0_[8:7__]_2_UINT8__mosqnr[14]:_mnr_ctrl_4,____reg_mnr_filter_c,
 *  089._[MNR_____]_0xC80071B0_[23:16]_8_UINT8__mosqnr[15]:_mnr_ctrl_4,____reg_mnr_motion_min,
 *  090._[MNR_____]_0xC80071B0_[31:24]_8_UINT8__mosqnr[16]:_mnr_ctrl_4,____reg_mnr_motion_max,
 */
typedef struct
{
	UINT8 dc_bnr[PE_FWI_H15_DNR_DCB_SIZE];	///< dc bnr
	UINT8 ac_bnr[PE_FWI_H15_DNR_ACB_SIZE];	///< ac bnr
}
PE_FWI_H15_DNR_DETAIL;

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
PE_FWI_H15_CEN_CTRL0_T;

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
PE_FWI_H15_CEN_CTRL1_T;
#endif

/**
 * PQ Firmware IPC Control
 */
typedef struct {
	PE_FWI_H15_TNR_CTRL			tnr_ctrl;		///< tnr ctrl
	PE_FWI_H15_FW_CTRL			fw_ctrl;		///< fwi ctrl
	PE_FWI_H15_SRC_INFO			src_info;		///< src info
	#ifdef PE_FWI_H15_ADD_FW_DNR_DATA
	PE_FWI_H15_DNR_COMMON		dnr_common;		///< dnr common
	PE_FWI_H15_DNR_DETAIL		dnr_detail;		///< dnr detail
	#endif
	#if 0	//not_used
	PE_FWI_H15_HOR_CTRL			shp_h_ctrl;		///< hor ctrl
	PE_FWI_H15_MISC_CTRL		shp_m_ctrl;		///< misc ctrl
	PE_FWI_H15_EDGE_GAIN		shp_e_ctrl;		///< edge ctrl
	PE_FWI_H15_TEXTURE_GAIN		shp_t_ctrl;		///< texture ctrl
	PE_FWI_H15_CEN_CTRL			cen_ctrl;		///< cen ctrl
	#endif
}
PE_FWI_H15_PQ_IPC_CTRL;

/**
 *	PE fwi status
 */
typedef struct
{
	UINT32 reset;				///< fw reset
	UINT32 src_info;			///< fw src info
}
PE_FWI_H15_STATUS_T;

/**
 *	pe fwi settings type
 */
typedef struct
{
	PE_FWI_H15_TNR_CTRL			tnr_ctrl[LX_PE_WIN_NUM];		///< tnr ctrl
	PE_FWI_H15_FW_CTRL			fw_ctrl;						///< fwi ctrl
	PE_FWI_H15_SRC_INFO			src_info;						///< src info
	PE_FWI_H15_STATUS_T			fw_status;						///< fw status
	PE_FWI_H15_HOR_CTRL			shp_h_ctrl;						///< hor ctrl
	PE_FWI_H15_MISC_CTRL		shp_m_ctrl;						///< misc ctrl
	PE_FWI_H15_EDGE_GAIN		shp_e_ctrl;						///< edge ctrl
	PE_FWI_H15_TEXTURE_GAIN		shp_t_ctrl;						///< texture ctrl
	PE_FWI_H15_CEN_CTRL			cen_ctrl[LX_PE_CMG_REGION_NUM];	///< cen ctrl
	PE_FWI_H15_DNR_COMMON		dnr_common;						///< dnr common
	PE_FWI_H15_DNR_DETAIL		dnr_detail;						///< dnr detail
}
PE_FWI_H15_SETTINGS_T;

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/
void PE_FWI_H15_SetDbgPrint(UINT32 on_off);
UINT32 PE_FWI_H15_GetDbgPrint(void);
int PE_FWI_H15_IPC_Open(void);
int PE_FWI_H15_IPC_Close(void);
int PE_FWI_H15_Init(PE_CFG_CTRL_T *pstParams);
int PE_FWI_H15_SetFwCtrl(PE_FWI_H15_FW_CTRL *pstParams, UINT32 called_on_sync);
int PE_FWI_H15_SetFwCtrlEnable(PE_TSK_H15_CFG_T *pCfg);
int PE_FWI_H15A_CheckFwStatus(PE_TSK_H15_CFG_T *pCfg);
int PE_FWI_H15_SetFwTnrEnable(UINT32 on_off);
int PE_FWI_H15_LoadDbData(UINT32 base_addr);
int PE_FWI_H15_SetCviCsc(LX_PE_CSC_INPUT_T *pstParams);
int PE_FWI_H15_SetShpHorCtrl(PE_FWI_H15_HOR_CTRL *pstParams);
int PE_FWI_H15_SetShpMiscCtrl(PE_FWI_H15_MISC_CTRL *pstParams);
int PE_FWI_H15_SetEdgeGainCtrl(PE_FWI_H15_EDGE_GAIN *pstParams);
int PE_FWI_H15_SetTextureGainCtrl(PE_FWI_H15_TEXTURE_GAIN *pstParams);
int PE_FWI_H15_SetTnrCtrl(UINT32 win_id, PE_FWI_H15_TNR_CTRL *pstParams);
int PE_FWI_H15_SetSrcInfo(PE_FWI_H15_SRC_INFO *pstParams);
int PE_FWI_H15_SetDnrCommon(PE_FWI_H15_DNR_COMMON *pstParams);
int PE_FWI_H15_SetDnrDetail(PE_FWI_H15_DNR_DETAIL *pstParams);
int PE_FWI_H15_GetCurFwiSettings(PE_FWI_H15_SETTINGS_T *pstParams);

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _PE_FWI_H15_H_ */

