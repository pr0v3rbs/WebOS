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

/** @file pe_shp_hw_m14.h
 *
 *  driver header for picture enhance sharpness. ( used only within kdriver )
 *	
 *	@author		Seung-Jun,Youm(sj.youm@lge.com)
 *	@version	0.1
 *	@note		
 *	@date		2012.03.15
 *	@see		
 */

#ifndef	_PE_SHP_HW_M14_H_
#define	_PE_SHP_HW_M14_H_

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
#define PE_SHP_HW_M14B_VFC_SAT_STATUS_TH	192

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/
#define PE_SHP_HW_M14B_RES_FMT_TO_STR(_x)		(\
	((_x)==PE_SHP_M14B_RES_ATV)? \
	"PE_SHP_M14B_RES_ATV":\
	((_x)==PE_SHP_M14B_RES_AV_480)? \
	"PE_SHP_M14B_RES_AV_480":\
	((_x)==PE_SHP_M14B_RES_AV_576)? \
	"PE_SHP_M14B_RES_AV_576":\
	((_x)==PE_SHP_M14B_RES_HD)? \
	"PE_SHP_M14B_RES_HD":\
	((_x)==PE_SHP_M14B_RES_PC)? \
	"PE_SHP_M14B_RES_PC":\
	((_x)==PE_SHP_M14B_RES_SD_480)? \
	"PE_SHP_M14B_RES_SD_480":\
	((_x)==PE_SHP_M14B_RES_SD_576)? \
	"PE_SHP_M14B_RES_SD_576":"PE_SHP_M14B_RES_NUM")

#define PE_SHP_HW_M14B_SRE_FMT_TO_STR(_x)		(\
	((_x)==PE_SHP_M14B_SRE_ATV)? \
	"PE_SHP_M14B_SRE_ATV":\
	((_x)==PE_SHP_M14B_SRE_AV_NTSC)? \
	"PE_SHP_M14B_SRE_AV_NTSC":\
	((_x)==PE_SHP_M14B_SRE_AV_PAL)? \
	"PE_SHP_M14B_SRE_AV_PAL":\
	((_x)==PE_SHP_M14B_SRE_DTV_HD)? \
	"PE_SHP_M14B_SRE_DTV_HD":\
	((_x)==PE_SHP_M14B_SRE_DTV_SD)? \
	"PE_SHP_M14B_SRE_DTV_SD":\
	((_x)==PE_SHP_M14B_SRE_HDMI_HD)? \
	"PE_SHP_M14B_SRE_HDMI_HD":\
	((_x)==PE_SHP_M14B_SRE_HDMI_SD)? \
	"PE_SHP_M14B_SRE_HDMI_SD":\
	((_x)==PE_SHP_M14B_SRE_PC)? \
	"PE_SHP_M14B_SRE_PC":"PE_SHP_M14B_SRE_NUM")

#define PE_SHP_HW_M14B_SRL_FMT_TO_STR(_x)		(\
	((_x)==PE_SHP_M14B_SRL_HD)? \
	"PE_SHP_M14B_SRL_HD":\
	((_x)==PE_SHP_M14B_SRL_SD)? \
	"PE_SHP_M14B_SRL_SD":"PE_SHP_M14B_SRE_NUM")

#define PE_SHP_HW_M14B_VFC_FMT_TO_STR(_x)		(\
	((_x)==PE_SHP_M14B_VFC_ATV_NT)? \
	"PE_SHP_M14B_VFC_ATV_NT":\
	((_x)==PE_SHP_M14B_VFC_ATV_PAL)? \
	"PE_SHP_M14B_VFC_ATV_PAL":\
	((_x)==PE_SHP_M14B_VFC_COMP_SD)? \
	"PE_SHP_M14B_VFC_COMP_SD":\
	((_x)==PE_SHP_M14B_VFC_AV)? \
	"PE_SHP_M14B_VFC_AV":\
	((_x)==PE_SHP_M14B_VFC_DTV_SD)? \
	"PE_SHP_M14B_VFC_DTV_SD":\
	((_x)==PE_SHP_M14B_VFC_HD)? \
	"PE_SHP_M14B_VFC_HD":\
	((_x)==PE_SHP_M14B_VFC_HDMI_SD)? \
	"PE_SHP_M14B_VFC_HDMI_SD":\
	((_x)==PE_SHP_M14B_VFC_PC)? \
	"PE_SHP_M14B_VFC_PC":\
	((_x)==PE_SHP_M14B_VFC_SECAM)? \
	"PE_SHP_M14B_VFC_SECAM":"PE_SHP_M14B_VFC_NUM")

#define PE_SHP_HW_M14B_CTI_FMT_TO_STR(_x)		(\
	((_x)==PE_SHP_M14B_CTI_ATV_NT)? \
	"PE_SHP_M14B_CTI_ATV_NT":\
	((_x)==PE_SHP_M14B_CTI_ATV_PAL)? \
	"PE_SHP_M14B_CTI_ATV_PAL":\
	((_x)==PE_SHP_M14B_CTI_HD)? \
	"PE_SHP_M14B_CTI_HD":\
	((_x)==PE_SHP_M14B_CTI_SD)? \
	"PE_SHP_M14B_CTI_SD":\
	((_x)==PE_SHP_M14B_CTI_PC)? \
	"PE_SHP_M14B_CTI_PC":"PE_SHP_M14B_CTI_NUM")

#define PE_SHP_HW_M14A_RES_FMT_TO_STR(_x)		(\
	((_x)==PE_SHP_M14A_RES_RF_NTSC)? \
	"PE_SHP_M14A_RES_RF_NTSC":\
	((_x)==PE_SHP_M14A_RES_RF_PAL)? \
	"PE_SHP_M14A_RES_RF_PAL":\
	((_x)==PE_SHP_M14A_RES_AV_NTSC)? \
	"PE_SHP_M14A_RES_AV_NTSC":\
	((_x)==PE_SHP_M14A_RES_AV_PAL)? \
	"PE_SHP_M14A_RES_AV_PAL":\
	((_x)==PE_SHP_M14A_RES_PC)? \
	"PE_SHP_M14A_RES_PC":\
	((_x)==PE_SHP_M14A_RES_DTV_SD)? \
	"PE_SHP_M14A_RES_DTV_SD":\
	((_x)==PE_SHP_M14A_RES_DTV_HD)? \
	"PE_SHP_M14A_RES_DTV_HD":\
	((_x)==PE_SHP_M14A_RES_COMP480I)? \
	"PE_SHP_M14A_RES_COMP480I":\
	((_x)==PE_SHP_M14A_RES_COMP480P)? \
	"PE_SHP_M14A_RES_COMP480P":\
	((_x)==PE_SHP_M14A_RES_HDMI480I)? \
	"PE_SHP_M14A_RES_HDMI480I":\
	((_x)==PE_SHP_M14A_RES_HDMI480P)? \
	"PE_SHP_M14A_RES_HDMI480P":\
	((_x)==PE_SHP_M14A_RES_HDMI_HD)? \
	"PE_SHP_M14A_RES_HDMI_HD":"PE_SHP_M14A_RES_NUM")

#define PE_SHP_HW_M14A_VFC_FMT_TO_STR(_x)		(\
	((_x)==PE_SHP_M14A_VFC_HD)? \
	"PE_SHP_M14A_VFC_HD":\
	((_x)==PE_SHP_M14A_VFC_PC)? \
	"PE_SHP_M14A_VFC_PC":\
	((_x)==PE_SHP_M14A_VFC_SD_AV)? \
	"PE_SHP_M14A_VFC_SD_AV":\
	((_x)==PE_SHP_M14A_VFC_SD_AVRF_SECAM)? \
	"PE_SHP_M14A_VFC_SD_AVRF_SECAM":\
	((_x)==PE_SHP_M14A_VFC_SD_DTV)? \
	"PE_SHP_M14A_VFC_SD_DTV":\
	((_x)==PE_SHP_M14A_VFC_SD_HDMI)? \
	"PE_SHP_M14A_VFC_SD_HDMI":\
	((_x)==PE_SHP_M14A_VFC_SD_RF_NT)? \
	"PE_SHP_M14A_VFC_SD_RF_NT":\
	((_x)==PE_SHP_M14A_VFC_SD_RF_PAL)? \
	"PE_SHP_M14A_VFC_SD_RF_PAL":"PE_SHP_M14A_VFC_NUM")

/**
 *	pe read default reg menu enumeration(for M14BX)
 */
typedef enum
{
	PE_SHP_HW_M14B_RD_DFLT_DISPLAY = 0,		///< menu display
	PE_SHP_HW_M14B_RD_DFLT_SHP,				///< sharpness
	PE_SHP_HW_M14B_RD_DFLT_SRE,				///< sre
	PE_SHP_HW_M14B_RD_DFLT_VFC,				///< vfilter c
	PE_SHP_HW_M14B_RD_DFLT_CTI,				///< cti(p0d)
	PE_SHP_HW_M14B_RD_RES_MNUM,				///< res matching number
	PE_SHP_HW_M14B_RD_SRE_MNUM,				///< sre matching number
	PE_SHP_HW_M14B_RD_PE1_DATA,				///< pe1 ctrl
	PE_SHP_HW_M14B_RD_PE1_MASK,				///< pe1 mask
	PE_SHP_HW_M14B_RD_VFC_MNUM,				///< vfc matching number
	PE_SHP_HW_M14B_RD_MSC_DATA,				///< msc ctrl
	PE_SHP_HW_M14B_RD_MSC_MASK,				///< msc mask
	PE_SHP_HW_M14B_RD_DFLT_MAX				///< max num
}
PE_SHP_HW_M14B_RD_DFLT_MENU;

/**
 *	pe read default reg menu enumeration(for M14AX)
 */
typedef enum
{
	PE_SHP_HW_M14A_RD_DFLT_DISPLAY = 0,		///< menu display
	PE_SHP_HW_M14A_RD_DFLT_SHP_L,			///< sharpness L
	PE_SHP_HW_M14A_RD_DFLT_SHP_R,			///< sharpness R
	PE_SHP_HW_M14A_RD_DFLT_VFC_L,			///< vfilter c L
	PE_SHP_HW_M14A_RD_DFLT_VFC_R,			///< vfilter c R
	PE_SHP_HW_M14A_RD_DFLT_MAX				///< max num
}
PE_SHP_HW_M14A_RD_DFLT_MENU;

/**
 *	pe read scl filter coeff reg menu enumeration
 *	- msl(r):main scaler L(R), ssc:sub scaler
 */
typedef enum
{
	PE_SHP_HW_M14_RD_SCL_DISPLAY = 0,	///< menu display
	PE_SHP_HW_M14_RD_SCL_MSL_Y_H,		///< msl y hor
	PE_SHP_HW_M14_RD_SCL_MSL_Y_V,		///< msl y ver
	PE_SHP_HW_M14_RD_SCL_MSL_C_H,		///< msl c hor
	PE_SHP_HW_M14_RD_SCL_MSL_C_V,		///< msl c ver
	PE_SHP_HW_M14_RD_SCL_MSR_Y_H,		///< msr y hor
	PE_SHP_HW_M14_RD_SCL_MSR_Y_V,		///< msr y ver
	PE_SHP_HW_M14_RD_SCL_MSR_C_H,		///< msr c hor
	PE_SHP_HW_M14_RD_SCL_MSR_C_V,		///< msr c ver
	PE_SHP_HW_M14_RD_SCL_SSC_Y_H,		///< ssc y hor
	PE_SHP_HW_M14_RD_SCL_MAX			///< max num
}
PE_SHP_HW_M14_RD_SCL_MENU;

/**
 *	pe shp resolution enhance fmt(for M14BX)
 */
typedef enum
{
	PE_SHP_M14B_RES_ATV = 0,		///< atv
	PE_SHP_M14B_RES_AV_480,			///< av 480
	PE_SHP_M14B_RES_AV_576,			///< av 576
	PE_SHP_M14B_RES_HD,				///< hd
	PE_SHP_M14B_RES_PC,				///< pc
	PE_SHP_M14B_RES_SD_480,			///< sd 480
	PE_SHP_M14B_RES_SD_576,			///< sd 576
	PE_SHP_M14B_RES_NUM				///< max num
}
PE_SHP_HW_M14B_RES_FMT;

/**
 *	pe shp sre fmt(for M14BX)
 */
typedef enum
{
	PE_SHP_M14B_SRE_ATV = 0,		///< atv
	PE_SHP_M14B_SRE_AV_NTSC,		///< av ntsc
	PE_SHP_M14B_SRE_AV_PAL,			///< av pal
	PE_SHP_M14B_SRE_DTV_HD,			///< dtv hd
	PE_SHP_M14B_SRE_DTV_SD,			///< dtv sd
	PE_SHP_M14B_SRE_HDMI_HD,		///< hdmi hd
	PE_SHP_M14B_SRE_HDMI_SD,		///< hdmi sd
	PE_SHP_M14B_SRE_PC,				///< pc
	PE_SHP_M14B_SRE_NUM				///< max num
}
PE_SHP_HW_M14B_SRE_FMT;

/**
 *	pe shp sre lut fmt(for M14BX)
 */
typedef enum
{
	PE_SHP_M14B_SRL_HD = 0,			///< hd
	PE_SHP_M14B_SRL_SD,				///< sd
	PE_SHP_M14B_SRL_NUM				///< max num
}
PE_SHP_HW_M14B_SRL_FMT;

/**
 *	pe shp v filter c fmt(for M14BX)
 */
typedef enum
{
	PE_SHP_M14B_VFC_ATV_NT = 0,		///< atv nt
	PE_SHP_M14B_VFC_ATV_PAL,		///< atv pal
	PE_SHP_M14B_VFC_AV,				///< av
	PE_SHP_M14B_VFC_COMP_SD,		///< comp sd
	PE_SHP_M14B_VFC_DTV_SD,			///< dtv sd
	PE_SHP_M14B_VFC_HD,				///< hd
	PE_SHP_M14B_VFC_HDMI_SD,		///< hdmi sd
	PE_SHP_M14B_VFC_PC,				///< pc
	PE_SHP_M14B_VFC_SECAM,			///< secam
	PE_SHP_M14B_VFC_NUM				///< max num
}
PE_SHP_HW_M14B_VFC_FMT;

/**
 *	pe cti(p0d) fmt(for M14BX)
 */
typedef enum
{
	PE_SHP_M14B_CTI_ATV_NT = 0,		///< atv nt
	PE_SHP_M14B_CTI_ATV_PAL,		///< atv pal
	PE_SHP_M14B_CTI_HD,				///< hd
	PE_SHP_M14B_CTI_SD,				///< sd
	PE_SHP_M14B_CTI_PC,				///< pc
	PE_SHP_M14B_CTI_NUM				///< max num
}
PE_SHP_HW_M14B_CTI_FMT;

/**
 *	pe shp resolution enhance fmt(for M14AX)
 */
typedef enum
{
	PE_SHP_M14A_RES_RF_NTSC = 0,	///< rf ntsc
	PE_SHP_M14A_RES_RF_PAL,			///< rf pal
	PE_SHP_M14A_RES_AV_NTSC,		///< av ntsc
	PE_SHP_M14A_RES_AV_PAL,			///< av pal
	PE_SHP_M14A_RES_PC,				///< pc
	PE_SHP_M14A_RES_DTV_SD,			///< dtv sd
	PE_SHP_M14A_RES_DTV_HD,			///< dtv hd
	PE_SHP_M14A_RES_COMP480I,		///< comp 480i
	PE_SHP_M14A_RES_COMP480P,		///< comp 480p
	PE_SHP_M14A_RES_HDMI480I,		///< hdmi 480i
	PE_SHP_M14A_RES_HDMI480P,		///< hdmi 480p
	PE_SHP_M14A_RES_HDMI_HD,		///< hdmi hd
	PE_SHP_M14A_RES_NUM				///< max num
}
PE_SHP_HW_M14A_RES_FMT;

/**
 *	pe shp v filter c fmt(for M14AX)
 */
typedef enum
{
	PE_SHP_M14A_VFC_HD = 0,			///< hd
	PE_SHP_M14A_VFC_PC,				///< pc
	PE_SHP_M14A_VFC_SD_AV,			///< sd av
	PE_SHP_M14A_VFC_SD_AVRF_SECAM,	///< sd av,rf secam
	PE_SHP_M14A_VFC_SD_DTV,			///< sd dtv
	PE_SHP_M14A_VFC_SD_HDMI,		///< sd hdmi
	PE_SHP_M14A_VFC_SD_RF_NT,		///< sd rf nt
	PE_SHP_M14A_VFC_SD_RF_PAL,		///< sd rf pal
	PE_SHP_M14A_VFC_NUM				///< max num
}
PE_SHP_HW_M14A_VFC_FMT;

/**
 *	scaler fir filter type enumeration
 */
typedef enum
{
	PE_SHP_HW_M14_FIR_BILINEAR = 0,	///< bilinear filter
	PE_SHP_HW_M14_FIR_POLYPHASE,	///< polyphase filter
	PE_SHP_HW_M14_FIR_NUM			///< max num
}
PE_SHP_HW_M14_FIR;

/**
 *	v scaler tap mode type enumeration
 */
typedef enum
{
	PE_SHP_HW_M14_SCL_V12T_NORM = 0,	///< v12tap mode
	PE_SHP_HW_M14_SCL_V12T_DN14,		///< v12tap mode, h50%,v14% down scaling case
	PE_SHP_HW_M14_SCL_V06T_NORM,		///< v06tap mode, normal case
	PE_SHP_HW_M14_SCL_V06T_HALF,		///< v06tap mode, half case
	PE_SHP_HW_M14_SCL_V06T_DN75,		///< v06tap mode, 75% down scaling case
	PE_SHP_HW_M14_SCL_V06T_DN50,		///< v06tap mode, 50% down scaling case
	PE_SHP_HW_M14_SCL_V06T_DN25,		///< v06tap mode, 25% down scaling case
	PE_SHP_HW_M14_SCL_NUM				///< max num
}
PE_SHP_HW_M14_SCL;

/**
 *	pe sharpness module settings type
 */
typedef struct
{
	LX_PE_SHP_SCLFILTER_T cur_scl_idx[LX_PE_WIN_NUM];	///< current scaler filter index
	LX_PE_SHP_SCLFILTER_T pre_scl_idx[LX_PE_WIN_NUM];	///< previous scaler filter index
	PE_SHP_HW_M14_SCL h_y_tap[LX_PE_WIN_NUM];			///< scaler filter horizontal luma tap type
	PE_SHP_HW_M14_SCL h_c_tap[LX_PE_WIN_NUM];			///< scaler filter horizontal chroma tap type
	PE_SHP_HW_M14_SCL v_y_tap[LX_PE_WIN_NUM];			///< scaler filter vertical luma tap type
	PE_SHP_HW_M14_SCL v_c_tap[LX_PE_WIN_NUM];			///< scaler filter vertical chroma tap type
	PE_SHP_HW_M14_FIR y_fir[LX_PE_WIN_NUM];				///< fir type, luma
	PE_SHP_HW_M14_FIR c_fir[LX_PE_WIN_NUM];				///< fir type, chroma
	UINT32 pre_res_en[LX_PE_WIN_NUM];					///< previous resolution enable
	PE_SHP_HW_M14A_RES_FMT res0a_fmt;					///< previous res0a fmt
	PE_SHP_HW_M14A_RES_FMT res1a_fmt;					///< previous res1a fmt
	PE_SHP_HW_M14A_VFC_FMT vfc0a_fmt;					///< previous msl vfc0a fmt
	PE_SHP_HW_M14A_VFC_FMT vfc1a_fmt;					///< previous msr vfc1a fmt
	PE_SHP_HW_M14B_RES_FMT res0b_fmt;					///< previous res0b fmt
	PE_SHP_HW_M14B_SRE_FMT sre0b_fmt;					///< previous sre0b fmt
	PE_SHP_HW_M14B_SRL_FMT srl0b_fmt;					///< previous srl0b fmt
	PE_SHP_HW_M14B_VFC_FMT vfc0b_fmt;					///< previous msc vfc0b fmt
	PE_SHP_HW_M14B_CTI_FMT cti0b_fmt;					///< previous p0d cti0b fmt
	UINT32 dft_msc_chroma_tear_ctrl;					///< default msc_chroma_tear_ctrl for m14b
	UINT32 cur_msc_chroma_tear_ctrl;					///< current msc_chroma_tear_ctrl for m14b
}
PE_SHP_HW_M14_SETTINGS_T;

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/
int PE_SHP_HW_M14_Init(PE_CFG_CTRL_T *pstParams);
int PE_SHP_HW_M14_SetDebugSettings(LX_PE_DBG_SETTINGS_T *pstParams);
int PE_SHP_HW_M14_SetDefault(LX_PE_DEFAULT_SETTINGS_T *pstParams);
int PE_SHP_HW_M14_SetScalerFilterCoeff(LX_PE_SHP_SCLFILTER_T *pstParams);
int PE_SHP_HW_M14_GetScalerFilterCoeff(LX_PE_SHP_SCLFILTER_T *pstParams);
int PE_SHP_HW_M14_SetScalerFilterCtrl(PE_TSK_M14_CFG_T *pCfg);
int PE_SHP_HW_M14_GetCurShpSettings(PE_SHP_HW_M14_SETTINGS_T *pstParams);
int PE_SHP_HW_M14_RdSclFltCoeff(LX_PE_ETC_TBL_T *pstParams);
int PE_SHP_HW_M14_RdDefault(LX_PE_ETC_TBL_T *pstParams);
int PE_SHP_HW_M14_SetReCmnCtrl(void *pstParams);
int PE_SHP_HW_M14_GetReCmnCtrl(void *pstParams);
int PE_SHP_HW_M14_SetReHorCtrl(void *pstParams);
int PE_SHP_HW_M14_GetReHorCtrl(void *pstParams);
int PE_SHP_HW_M14_SetReVerCtrl(void *pstParams);
int PE_SHP_HW_M14_GetReVerCtrl(void *pstParams);
int PE_SHP_HW_M14_SetReMiscCtrl(void *pstParams);
int PE_SHP_HW_M14_GetReMiscCtrl(void *pstParams);
int PE_SHP_HW_M14_SetEdgeEnhanceCtrl(void *pstParams);
int PE_SHP_HW_M14_GetEdgeEnhanceCtrl(void *pstParams);
int PE_SHP_HW_M14_SetDetailEnhanceCtrl(void *pstParams);
int PE_SHP_HW_M14_GetDetailEnhanceCtrl(void *pstParams);
int PE_SHP_HW_M14_SetSreCtrl(void *pstParams);
int PE_SHP_HW_M14_GetSreCtrl(void *pstParams);
int PE_SHP_HW_M14B_SetVfcApply(PE_TSK_M14_CFG_T *pCfg);

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _PE_SHP_HW_M14_H_ */
