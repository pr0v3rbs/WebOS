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

/** @file pe_shp_hw_h14.h
 *
 *  driver header for picture enhance sharpness. ( used only within kdriver )
 *	
 *	@author		Seung-Jun,Youm(sj.youm@lge.com)
 *	@version	0.1
 *	@note		
 *	@date		2012.03.15
 *	@see		
 */

#ifndef	_PE_SHP_HW_H14_H_
#define	_PE_SHP_HW_H14_H_

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
/**	20130616, sj.youm,
*	define for skip msr filter_type_luma,chroma comparison
*	because of UD R image blur(1dot1 pattern) after system on.
*/
#undef PE_SHP_HW_H14_SKIP_MSR_FLT_TYPE_COMPARISON

/**	20130622, sj.youm,
*	define for using v12t down scale filter coeff
*	because of V aliasing on UD resize.
*/
#define PE_SHP_HW_H14_USE_V12T_DN_SC_FLT

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
	PE_SHP_HW_H14_RD_DFLT_DISPLAY = 0,		///< menu display
	PE_SHP_HW_H14_RD_DFLT_SHP_L,			///< sharpness L
	PE_SHP_HW_H14_RD_DFLT_SHP_R,			///< sharpness R
	PE_SHP_HW_H14_RD_DFLT_VFC_L,			///< vfilter c L
	PE_SHP_HW_H14_RD_DFLT_VFC_R,			///< vfilter c R
	PE_SHP_HW_H14_RD_DFLT_MAX				///< max num
}
PE_SHP_HW_H14_RD_DFLT_MENU;

/**
 *	pe read scl filter coeff reg menu enumeration
 *	- msl(r):main scaler L(R), ssc:sub scaler
 */
typedef enum
{
	PE_SHP_HW_H14_RD_SCL_DISPLAY = 0,	///< menu display
	PE_SHP_HW_H14_RD_SCL_MSL_Y_H,		///< msl y hor
	PE_SHP_HW_H14_RD_SCL_MSL_Y_V,		///< msl y ver
	PE_SHP_HW_H14_RD_SCL_MSL_C_H,		///< msl c hor
	PE_SHP_HW_H14_RD_SCL_MSL_C_V,		///< msl c ver
	PE_SHP_HW_H14_RD_SCL_MSR_Y_H,		///< msr y hor
	PE_SHP_HW_H14_RD_SCL_MSR_Y_V,		///< msr y ver
	PE_SHP_HW_H14_RD_SCL_MSR_C_H,		///< msr c hor
	PE_SHP_HW_H14_RD_SCL_MSR_C_V,		///< msr c ver
	PE_SHP_HW_H14_RD_SCL_SSC_Y_H,		///< ssc y hor
	PE_SHP_HW_H14_RD_SCL_MAX			///< max num
}
PE_SHP_HW_H14_RD_SCL_MENU;

/**
 *	pe shp resolution enhance fmt(for H14)
 */
typedef enum
{
	PE_SHP_H14_RES_RF_NTSC = 0,	///< rf ntsc
	PE_SHP_H14_RES_RF_PAL,			///< rf pal
	PE_SHP_H14_RES_AV_NTSC,		///< av ntsc
	PE_SHP_H14_RES_AV_PAL,			///< av pal
	PE_SHP_H14_RES_PC,				///< pc
	PE_SHP_H14_RES_DTV_SD,			///< dtv sd
	PE_SHP_H14_RES_DTV_HD,			///< dtv hd
	PE_SHP_H14_RES_COMP480I,		///< comp 480i
	PE_SHP_H14_RES_COMP480P,		///< comp 480p
	PE_SHP_H14_RES_HDMI480I,		///< hdmi 480i
	PE_SHP_H14_RES_HDMI480P,		///< hdmi 480p
	PE_SHP_H14_RES_HDMI_HD,		///< hdmi hd
	PE_SHP_H14_RES_NUM				///< max num
}
PE_SHP_HW_H14_RES_FMT;


/**
 *	pe shp v filter c fmt(for H14)
 */
typedef enum
{
	PE_SHP_H14_VFC_HD = 0,			///< hd
	PE_SHP_H14_VFC_PC,				///< pc
	PE_SHP_H14_VFC_SD_AV,			///< sd av
	PE_SHP_H14_VFC_SD_AVRF_SECAM,	///< sd av,rf secam
	PE_SHP_H14_VFC_SD_DTV,			///< sd dtv
	PE_SHP_H14_VFC_SD_HDMI,		///< sd hdmi
	PE_SHP_H14_VFC_SD_RF_NT,		///< sd rf nt
	PE_SHP_H14_VFC_SD_RF_PAL,		///< sd rf pal
	PE_SHP_H14_VFC_NUM				///< max num
}
PE_SHP_HW_H14_VFC_FMT;


/**
 *	scaler fir filter type enumeration
 */
typedef enum
{
	PE_SHP_HW_H14_FIR_BILINEAR = 0,	///< bilinear filter
	PE_SHP_HW_H14_FIR_POLYPHASE,	///< polyphase filter
	PE_SHP_HW_H14_FIR_NUM			///< max num
}
PE_SHP_HW_H14_FIR;

/**
 *	v scaler tap mode type enumeration
 */
typedef enum
{
	PE_SHP_HW_H14_SCL_V12T_NORM = 0,	///< v12tap mode
	PE_SHP_HW_H14_SCL_V12T_DN14,		///< v12tap mode, h50%,v14% down scaling case
	PE_SHP_HW_H14_SCL_V06T_NORM,		///< v06tap mode, normal case
	PE_SHP_HW_H14_SCL_V06T_HALF,		///< v06tap mode, half case
	PE_SHP_HW_H14_SCL_V06T_DN75,		///< v06tap mode, 75% down scaling case
	PE_SHP_HW_H14_SCL_V06T_DN50,		///< v06tap mode, 50% down scaling case
	PE_SHP_HW_H14_SCL_V06T_DN25,		///< v06tap mode, 25% down scaling case
	PE_SHP_HW_H14_SCL_NUM				///< max num
}
PE_SHP_HW_H14_SCL;

/**
 *	pe sharpness module settings type
 */
typedef struct
{
	LX_PE_SHP_SCLFILTER_T cur_scl_idx[LX_PE_WIN_NUM];	///< current scaler filter index
	LX_PE_SHP_SCLFILTER_T pre_scl_idx[LX_PE_WIN_NUM];	///< previous scaler filter index
	PE_SHP_HW_H14_SCL h_y_tap[LX_PE_WIN_NUM];			///< scaler filter horizontal luma tap type
	PE_SHP_HW_H14_SCL h_c_tap[LX_PE_WIN_NUM];			///< scaler filter horizontal chroma tap type
	PE_SHP_HW_H14_SCL v_y_tap[LX_PE_WIN_NUM];			///< scaler filter vertical luma tap type
	PE_SHP_HW_H14_SCL v_c_tap[LX_PE_WIN_NUM];			///< scaler filter vertical chroma tap type
	PE_SHP_HW_H14_FIR y_fir[LX_PE_WIN_NUM];				///< fir type, luma
	PE_SHP_HW_H14_FIR c_fir[LX_PE_WIN_NUM];				///< fir type, chroma
	UINT32 pre_res_en[LX_PE_WIN_NUM];					///< previous resolution enable
}
PE_SHP_HW_H14_SETTINGS_T;

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/
int PE_SHP_HW_H14_Init(PE_CFG_CTRL_T *pstParams);
int PE_SHP_HW_H14_SetDebugSettings(LX_PE_DBG_SETTINGS_T *pstParams);
int PE_SHP_HW_H14_SetDefault(LX_PE_DEFAULT_SETTINGS_T *pstParams);
int PE_SHP_HW_H14_SetScalerFilterCoeff(LX_PE_SHP_SCLFILTER_T *pstParams);
int PE_SHP_HW_H14_GetScalerFilterCoeff(LX_PE_SHP_SCLFILTER_T *pstParams);
int PE_SHP_HW_H14_SetReEnableCtrl(PE_TSK_H14_CFG_T *pCfg);
int PE_SHP_HW_H14_SetScalerFilterCtrl(PE_TSK_H14_CFG_T *pCfg);
int PE_SHP_HW_H14_GetCurShpSettings(PE_SHP_HW_H14_SETTINGS_T *pstParams);
int PE_SHP_HW_H14_RdSclFltCoeff(LX_PE_ETC_TBL_T *pstParams);
int PE_SHP_HW_H14_RdDefault(LX_PE_ETC_TBL_T *pstParams);
int PE_SHP_HW_H14_SetReCmnCtrl(void *pstParams);
int PE_SHP_HW_H14_GetReCmnCtrl(void *pstParams);
int PE_SHP_HW_H14_SetReHorCtrl(void *pstParams);
int PE_SHP_HW_H14_GetReHorCtrl(void *pstParams);
int PE_SHP_HW_H14_SetReVerCtrl(void *pstParams);
int PE_SHP_HW_H14_GetReVerCtrl(void *pstParams);
int PE_SHP_HW_H14_SetReMiscCtrl(void *pstParams);
int PE_SHP_HW_H14_GetReMiscCtrl(void *pstParams);
int PE_SHP_HW_H14_SetEdgeEnhanceCtrl(void *pstParams);
int PE_SHP_HW_H14_GetEdgeEnhanceCtrl(void *pstParams);
int PE_SHP_HW_H14_SetDetailEnhanceCtrl(void *pstParams);
int PE_SHP_HW_H14_GetDetailEnhanceCtrl(void *pstParams);

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _PE_SHP_HW_H14_H_ */
