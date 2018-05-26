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

/** @file pe_nrd_hw_h15.h
 *
 *  driver header for picture enhance noise reduction. ( used only within kdriver )
 *
 *	@author		Seung-Jun,Youm(sj.youm@lge.com)
 *	@version	0.1
 *	@note
 *	@date		2012.03.15
 *	@see
 */

#ifndef	_PE_NRD_HW_H15_H_
#define	_PE_NRD_HW_H15_H_

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
/* apply tnr high gain */
#define PE_NRD_HW_H15_APPLY_TNR_HIGH_GAIN_OFF	(0x0)
#define PE_NRD_HW_H15_APPLY_TNR_HIGH_GAIN__ON	(0x1)
#define PE_NRD_HW_H15_TNR_HIGH_GAIN				(0xc0)
#define PE_NRD_HW_H15_MA_MOTION_ATV_TH			(7)
#define PE_NRD_HW_H15_SAT_STATUS_TH_DEL			(3)
#define PE_NRD_HW_H15_SAT_STATUS_ATV_TH			(197)
#define PE_NRD_HW_H15_SAT_STATUS_ATV_HIGH_TH	(PE_NRD_HW_H15_SAT_STATUS_ATV_TH+PE_NRD_HW_H15_SAT_STATUS_TH_DEL)
#define PE_NRD_HW_H15_SAT_STATUS_ATV_LOW_TH		(PE_NRD_HW_H15_SAT_STATUS_ATV_TH-PE_NRD_HW_H15_SAT_STATUS_TH_DEL)

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/
#define PE_NRD_HW_H15A_TNR_FMT_TO_STR(_x)		(\
	((_x)==PE_NRD_H15A_TNR_HD)?    "PE_NRD_H15A_TNR_HD":\
	((_x)==PE_NRD_H15A_TNR_SD)?    "PE_NRD_H15A_TNR_SD":\
	((_x)==PE_NRD_H15A_TNR_UD)?    "PE_NRD_H15A_TNR_UD":\
	((_x)==PE_NRD_H15A_TNR_SECAM)? "PE_NRD_H15A_TNR_SECAM":\
	((_x)==PE_NRD_H15A_TNR_CAM_HD)? "PE_NRD_H15A_TNR_CAM_HD":\
	((_x)==PE_NRD_H15A_TNR_ATV)?    "PE_NRD_H15A_TNR_ATV":\
	"PE_NRD_H15A_TNR_NUM")

/* tnr lut (tnl) */
#define PE_NRD_HW_H15A_TNL_FMT_TO_STR(_x)		(\
	((_x)==PE_NRD_H15A_TNL_HD)?  "PE_NRD_H15A_TNL_HD":\
	((_x)==PE_NRD_H15A_TNL_SD)?  "PE_NRD_H15A_TNL_SD":\
	((_x)==PE_NRD_H15A_TNL_UD)?  "PE_NRD_H15A_TNL_UD":\
	((_x)==PE_NRD_H15A_TNL_CAM_HD)?  "PE_NRD_H15A_TNL_CAM_HD":\
	"PE_NRD_H15A_TNL_NUM")

#define PE_NRD_HW_H15A_DNR_FMT_TO_STR(_x)		(\
	((_x)==PE_NRD_H15A_DNR_ATV_480)?   "PE_NRD_H15A_DNR_ATV_480":\
	((_x)==PE_NRD_H15A_DNR_ATV_576)?   "PE_NRD_H15A_DNR_ATV_576":\
	((_x)==PE_NRD_H15A_DNR_AV)?        "PE_NRD_H15A_DNR_AV":\
	((_x)==PE_NRD_H15A_DNR_DTV_SD)?    "PE_NRD_H15A_DNR_DTV_SD":\
	((_x)==PE_NRD_H15A_DNR_HD)?        "PE_NRD_H15A_DNR_HD":\
	((_x)==PE_NRD_H15A_DNR_PC)?        "PE_NRD_H15A_DNR_PC":\
	((_x)==PE_NRD_H15A_DNR_SD)?        "PE_NRD_H15A_DNR_SD":\
	"PE_NRD_H15A_DNR_NUM")

#define PE_NRD_HW_H15A_IFC_FMT_TO_STR(_x)		(\
	((_x)==PE_NRD_H15A_IFC_ATV_NTSC)?  "PE_NRD_H15A_IFC_ATV_NTSC":\
	((_x)==PE_NRD_H15A_IFC_PC)?        "PE_NRD_H15A_IFC_PC":\
	"PE_NRD_H15A_IFC_NUM")

#define PE_NRD_HW_H15A_TBL_IDX_TO_STR(_x)		(\
	((_x)==PE_NRD_HW_H15A_TBL_IDX_TNR)? "TNR":\
	((_x)==PE_NRD_HW_H15A_TBL_IDX_DNR)? "DNR":\
	((_x)==PE_NRD_HW_H15A_TBL_IDX_IFC)? "IFC":\
	"NUM")

/**
 *	pe table index enumeration(for H15AX)
 */
typedef enum
{
	PE_NRD_HW_H15A_TBL_IDX_TNR = 0,		///< tnr
	PE_NRD_HW_H15A_TBL_IDX_TNL,			///< tnr lut (tnl)
	PE_NRD_HW_H15A_TBL_IDX_DNR,			///< dnr
	PE_NRD_HW_H15A_TBL_IDX_IFC,			///< ifc
	PE_NRD_HW_H15A_TBL_IDX_NUM			///< max num
}
PE_NRD_HW_H15A_TBL_IDX;

/**
 *	pe read default reg menu enumeration(for H15AX)
 */
typedef enum
{
	PE_NRD_HW_H15A_RD_MENU_DISP = 0,	///< menu display
	PE_NRD_HW_H15A_RD_TNR1_DISP,		///< tnr1 display
	PE_NRD_HW_H15A_RD_TNR2_DISP,		///< tnr2 display
	PE_NRD_HW_H15A_RD_TNR1_DFLT,		///< tnr1 default
	PE_NRD_HW_H15A_RD_TNR1_USER,		///< tnr1 user
	PE_NRD_HW_H15A_RD_TNR1_MASK,		///< tnr1 mask
	PE_NRD_HW_H15A_RD_TNR1_DATA,		///< tnr1 data
	PE_NRD_HW_H15A_RD_TNR2_USER,		///< tnr2 user
	PE_NRD_HW_H15A_RD_TNR2_MASK,		///< tnr2 mask
	PE_NRD_HW_H15A_RD_TNR2_DATA,		///< tnr2 data
	PE_NRD_HW_H15A_RD_DNR1_DISP,		///< dnr1 display
	PE_NRD_HW_H15A_RD_DNR2_DISP,		///< dnr2 display
	PE_NRD_HW_H15A_RD_DNR1_DFLT,		///< dnr1 default
	PE_NRD_HW_H15A_RD_DNR1_USER,		///< dnr1 user
	PE_NRD_HW_H15A_RD_DNR1_MASK,		///< dnr1 mask
	PE_NRD_HW_H15A_RD_DNR1_DATA,		///< dnr1 data
	PE_NRD_HW_H15A_RD_IFC1_DISP,		///< ifc1 display
	PE_NRD_HW_H15A_RD_IFC1_DFLT,		///< ifc1 default
	PE_NRD_HW_H15A_RD_IFC1_USER,		///< ifc1 user
	PE_NRD_HW_H15A_RD_IFC1_MASK,		///< ifc1 mask
	PE_NRD_HW_H15A_RD_IFC1_DATA,		///< ifc1 data
	PE_NRD_HW_H15A_RD_TNR1_MLUT,		///< tnr1 main lut
	PE_NRD_HW_H15A_RD_TNR2_MLUT,		///< tnr2 main lut
	PE_NRD_HW_H15A_RD_TNL1_DFLT,		///< tnr lut1 (tnl) default
	PE_NRD_HW_H15A_RD_TNL1_USER,		///< tnr lut1 (tnl) user
	PE_NRD_HW_H15A_RD_TNL1_MASK,		///< tnr lut1 (tnl) mask
	PE_NRD_HW_H15A_RD_TNL1_DATA,		///< tnr lut1 (tnl) data
	PE_NRD_HW_H15A_RD_MENU_MAX			///< max num
}
PE_NRD_HW_H15A_RD_DFLT_MENU;

/**
 *	pe nrd tnr fmt(for H15AX)
 */
typedef enum
{
	PE_NRD_H15A_TNR_HD = 0,			///< hd
	PE_NRD_H15A_TNR_SD,				///< sd
	PE_NRD_H15A_TNR_UD,				///< uhd
	PE_NRD_H15A_TNR_SECAM,			///< atv, av, scart secam
	PE_NRD_H15A_TNR_CAM_HD,			///< dtv camera
	PE_NRD_H15A_TNR_ATV,				///< sd
	PE_NRD_H15A_TNR_NUM				///< max num
}
PE_NRD_HW_H15A_TNR_FMT;

/**
 *	pe nrd tnr lut (tnl) fmt(for H15AX)
 */
typedef enum
{
	PE_NRD_H15A_TNL_HD = 0,			///< hd
	PE_NRD_H15A_TNL_SD,				///< sd
	PE_NRD_H15A_TNL_UD,				///< ud
	PE_NRD_H15A_TNL_CAM_HD,			///< dtv camera
	PE_NRD_H15A_TNL_NUM				///< max num
}
PE_NRD_HW_H15A_TNL_FMT;

/**
 *	pe nrd dnr fmt(for H15AX)
 */
typedef enum
{
	PE_NRD_H15A_DNR_ATV_480 = 0,	///< atv 480
	PE_NRD_H15A_DNR_ATV_576,		///< atv 576
	PE_NRD_H15A_DNR_AV,			///< av
	PE_NRD_H15A_DNR_DTV_SD,		///< dtv sd
	PE_NRD_H15A_DNR_HD,			///< hd
	PE_NRD_H15A_DNR_PC,			///< pc
	PE_NRD_H15A_DNR_SD,			///< sd
	PE_NRD_H15A_DNR_NUM			///< max num
}
PE_NRD_HW_H15A_DNR_FMT;

/**
 *	pe nrd ifc fmt(for H15AX)
 */
typedef enum
{
	PE_NRD_H15A_IFC_ATV_NTSC = 0,	///< atv ntsc
	PE_NRD_H15A_IFC_PC,				///< pc(off)
	PE_NRD_H15A_IFC_NUM				///< max num
}
PE_NRD_HW_H15A_IFC_FMT;

/**
 *	pe nrd module settings type
 */
typedef struct
{
	#ifdef PE_HW_H15_FOR_BRINGUP
	LX_PE_NRD_TNR5_CMN_T tnr_cmn;				///< tnr common
	UINT32 use_manual_sc;						///< use tnr manual sc en
	UINT32 tnr_sc_en_user;						///< tnr_sc_en_user
	UINT32 tnr_sc_en_mask;						///< tnr_sc_en_mask
	UINT32 tnr_pv_smooth_en_user;				///< tnr_pv_smooth_en_user
	UINT32 tnr_pv_smooth_en_mask;				///< tnr_pv_smooth_en_mask
	UINT32 dnr_max_en0_user;					///< reg_dnr_max_enable user
	UINT32 dnr_max_en0_mask;					///< reg_dnr_max_enable mask
	UINT32 bnr_ac_h_en0_user;					///< bnr_ac_h_en0 user
	UINT32 bnr_ac_h_en0_mask;					///< bnr_ac_h_en0 mask
	UINT32 cur_tnr_auto_enable;					///< current tnr auto enable
	UINT32 cur_tnr_master_gain;					///< current tnr_master_gain
	PE_NRD_HW_H15A_TNR_FMT tnr0a_fmt;			///< previous tnr fmt
	PE_NRD_HW_H15A_TNL_FMT tnl0a_fmt;			///< previous tnr lut (tnl) fmt
	PE_NRD_HW_H15A_DNR_FMT dnr0a_fmt;			///< previous dnr fmt
	PE_NRD_HW_H15A_IFC_FMT ifc0a_fmt;			///< previous ifc fmt
	#else
	UINT32 reg_bnr_ac_h_en0;					///< previous bnr_ac_h_en0
	UINT32 reg_bnr_ac_h_en1;					///< previous bnr_ac_h_en1
	UINT32 cur_tnr_auto_enable;					///< current tnr auto enable
	UINT32 cur_tnr_master_gain;					///< current tnr_master_gain
	UINT32 pre_tnr_master_gain;					///< previous tnr_master_gain
	PE_NRD_HW_H15A_TNR_FMT tnr0b_fmt;			///< previous p0m tnr fmt
	PE_NRD_HW_H15A_DNR_FMT dnr0b_fmt;			///< previous p0d dnr fmt
	PE_NRD_HW_H15A_IFC_FMT ifc0b_fmt;			///< previous p0d ifc fmt
	#endif
}
PE_NRD_HW_H15_SETTINGS_T;

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/
int PE_NRD_HW_H15_Init(PE_CFG_CTRL_T *pstParams);
int PE_NRD_HW_H15_SetDebugSettings(LX_PE_DBG_SETTINGS_T *pstParams);
int PE_NRD_HW_H15_SetDefault(LX_PE_DEFAULT_SETTINGS_T *pstParams);
int PE_NRD_HW_H15_RdDefault(LX_PE_ETC_TBL_T *pstParams);
int PE_NRD_HW_H15_SetDnrCmnCtrl(void *pstParams);
int PE_NRD_HW_H15_GetDnrCmnCtrl(void *pstParams);
int PE_NRD_HW_H15_SetDnrDetailCtrl(void *pstParams);
int PE_NRD_HW_H15_GetDnrDetailCtrl(void *pstParams);
int PE_NRD_HW_H15_SetTnrCmnCtrl(void *pstParams);
int PE_NRD_HW_H15_GetTnrCmnCtrl(void *pstParams);
int PE_NRD_HW_H15_SetTnrDetailCtrl(void *pstParams);
int PE_NRD_HW_H15_GetTnrDetailCtrl(void *pstParams);
int PE_NRD_HW_H15_SetTnrMode(PE_TSK_H15_CFG_T *pCfg);
int PE_NRD_HW_H15_SetDnrMode(PE_TSK_H15_CFG_T *pCfg);
int PE_NRD_HW_H15_SetIfcMode(PE_TSK_H15_CFG_T *pCfg);
int PE_NRD_HW_H15_DownloadTnrDb(void);

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _PE_NRD_HW_H15_H_ */
