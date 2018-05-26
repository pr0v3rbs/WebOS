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

/** @file pe_nrd_hw_m14.h
 *
 *  driver header for picture enhance noise reduction. ( used only within kdriver )
 *
 *	@author		Seung-Jun,Youm(sj.youm@lge.com)
 *	@version	0.1
 *	@note
 *	@date		2012.03.15
 *	@see
 */

#ifndef	_PE_NRD_HW_M14_H_
#define	_PE_NRD_HW_M14_H_

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
/* define for bnr_ac_h_en 0x0(protection) for small size file play */
/* if PE_FWI_M14_ADD_FW_DNR_DATA defined, PE_HW_M14B_USE_DNR_MODE_ON_TASK is useless.*/
#define PE_HW_M14B_USE_DNR_MODE_ON_TASK
#define PE_HW_M14A_USE_DNR_MODE_ON_TASK

/* define for setting tnr gain on task, to increase tnr gain for atv pal & nr auto */
/* if PE_HW_M14B_USE_REG_CTRL defined, this is useless.*/
#undef PE_HW_M14_USE_TNR_GAIN_ON_TASK

/* apply tnr high gain */
#define PE_NRD_HW_M14_APPLY_TNR_HIGH_GAIN_OFF	(0x0)
#define PE_NRD_HW_M14_APPLY_TNR_HIGH_GAIN__ON	(0x1)
#define PE_NRD_HW_M14_TNR_HIGH_GAIN				(0xc0)
#define PE_NRD_HW_M14_MA_MOTION_ATV_TH			(7)
#define PE_NRD_HW_M14_SAT_STATUS_TH_DEL			(3)
#define PE_NRD_HW_M14_SAT_STATUS_ATV_TH			(197)
#define PE_NRD_HW_M14_SAT_STATUS_ATV_HIGH_TH	(PE_NRD_HW_M14_SAT_STATUS_ATV_TH+PE_NRD_HW_M14_SAT_STATUS_TH_DEL)
#define PE_NRD_HW_M14_SAT_STATUS_ATV_LOW_TH		(PE_NRD_HW_M14_SAT_STATUS_ATV_TH-PE_NRD_HW_M14_SAT_STATUS_TH_DEL)

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/
#define PE_NRD_HW_M14C_DNR_FMT_TO_STR(_x)		(\
		((_x)==PE_NRD_M14C_DNR_ATV)? \
		"PE_NRD_M14C_DNR_ATV":\
		((_x)==PE_NRD_M14C_DNR_AV)? \
		"PE_NRD_M14C_DNR_AV":\
		((_x)==PE_NRD_M14C_DNR_HD_I)? \
		"PE_NRD_M14C_DNR_HD_I":\
		((_x)==PE_NRD_M14C_DNR_HD_P)? \
		"PE_NRD_M14C_DNR_HD_P":\
		((_x)==PE_NRD_M14C_DNR_PC)? \
		"PE_NRD_M14C_DNR_PC":\
		((_x)==PE_NRD_M14C_DNR_SD_I)? \
		"PE_NRD_M14C_DNR_SD_I":\
		((_x)==PE_NRD_M14C_DNR_SD_P)? \
		"PE_NRD_M14C_DNR_SD_P":"PE_NRD_M14C_DNR_NUM")

#define PE_NRD_HW_M14B_TNR_FMT_TO_STR(_x)		(\
	((_x)==PE_NRD_M14B_TNR_HD_I)? \
	"PE_NRD_M14B_TNR_HD_I":\
	((_x)==PE_NRD_M14B_TNR_HD_P)? \
	"PE_NRD_M14B_TNR_HD_P":\
	((_x)==PE_NRD_M14B_TNR_HD_I_OLED)? \
	"PE_NRD_M14B_TNR_HD_I_OLED":\
	((_x)==PE_NRD_M14B_TNR_HD_P_OLED)? \
	"PE_NRD_M14B_TNR_HD_P_OLED":\
	((_x)==PE_NRD_M14B_TNR_COMP_HD_I_3D)? \
	"PE_NRD_M14B_TNR_COMP_HD_I_3D":\
	((_x)==PE_NRD_M14B_TNR_COMP_HD_P_3D)? \
	"PE_NRD_M14B_TNR_COMP_HD_P_3D":\
	((_x)==PE_NRD_M14B_TNR_PC)? \
	"PE_NRD_M14B_TNR_PC":\
	((_x)==PE_NRD_M14B_TNR_SD_I)? \
	"PE_NRD_M14B_TNR_SD_I":\
	((_x)==PE_NRD_M14B_TNR_DTV_SD_480_I)? \
	"PE_NRD_M14B_TNR_DTV_SD_480_I":\
	((_x)==PE_NRD_M14B_TNR_DTV_SD_576_I)? \
	"PE_NRD_M14B_TNR_DTV_SD_576_I":\
	((_x)==PE_NRD_M14B_TNR_SD_P)? \
	"PE_NRD_M14B_TNR_SD_P":\
	((_x)==PE_NRD_M14B_TNR_DTV_SD_P)? \
	"PE_NRD_M14B_TNR_DTV_SD_P":\
	((_x)==PE_NRD_M14B_TNR_AV_NTSC)? \
	"PE_NRD_M14B_TNR_AV_NTSC":\
	((_x)==PE_NRD_M14B_TNR_AV_PAL)? \
	"PE_NRD_M14B_TNR_AV_PAL":\
	((_x)==PE_NRD_M14B_TNR_AV_SECAM)? \
	"PE_NRD_M14B_TNR_AV_SECAM":"PE_NRD_M14B_TNR_NUM")

#define PE_NRD_HW_M14B_TAL_FMT_TO_STR(_x)		(\
	((_x)==PE_NRD_M14B_TAL_HD_I)? \
	"PE_NRD_M14B_TAL_HD_I":\
	((_x)==PE_NRD_M14B_TAL_HD_P)? \
	"PE_NRD_M14B_TAL_HD_P":\
	((_x)==PE_NRD_M14B_TAL_SD_I)? \
	"PE_NRD_M14B_TAL_SD_I":\
	((_x)==PE_NRD_M14B_TAL_SD_P)? \
	"PE_NRD_M14B_TAL_SD_P":\
	((_x)==PE_NRD_M14B_TAL_SECAM)? \
	"PE_NRD_M14B_TAL_SECAM":"PE_NRD_M14B_TAL_NUM")

#define PE_NRD_HW_M14B_DNR_FMT_TO_STR(_x)		(\
	((_x)==PE_NRD_M14B_DNR_ATV)? \
	"PE_NRD_M14B_DNR_ATV":\
	((_x)==PE_NRD_M14B_DNR_AV)? \
	"PE_NRD_M14B_DNR_AV":\
	((_x)==PE_NRD_M14B_DNR_HD)? \
	"PE_NRD_M14B_DNR_HD":\
	((_x)==PE_NRD_M14B_DNR_PC)? \
	"PE_NRD_M14B_DNR_PC":\
	((_x)==PE_NRD_M14B_DNR_SD)? \
	"PE_NRD_M14B_DNR_SD":"PE_NRD_M14B_DNR_NUM")

#define PE_NRD_HW_M14B_IFC_FMT_TO_STR(_x)		(\
	((_x)==PE_NRD_M14B_IFC_ATV_NTSC)? \
	"PE_NRD_M14B_IFC_ATV_NTSC":\
	((_x)==PE_NRD_M14B_IFC_PC)? \
	"PE_NRD_M14B_IFC_PC":"PE_NRD_M14B_IFC_NUM")

#define PE_NRD_HW_M14A_TNR_FMT_TO_STR(_x)		(\
	((_x)==PE_NRD_M14A_TNR_SD)? \
	"PE_NRD_M14A_TNR_SD":\
	((_x)==PE_NRD_M14A_TNR_SD_MC_OFF)? \
	"PE_NRD_M14A_TNR_SD_MC_OFF":\
	((_x)==PE_NRD_M14A_TNR_ATV)? \
	"PE_NRD_M14A_TNR_ATV":\
	((_x)==PE_NRD_M14A_TNR_DVR_ATV)? \
	"PE_NRD_M14A_TNR_DVR_ATV":\
	((_x)==PE_NRD_M14A_TNR_ATV_SECAM)? \
	"PE_NRD_M14A_TNR_ATV_SECAM":\
	((_x)==PE_NRD_M14A_TNR_AV)? \
	"PE_NRD_M14A_TNR_AV":\
	((_x)==PE_NRD_M14A_TNR_AV_SECAM)? \
	"PE_NRD_M14A_TNR_AV_SECAM":\
	((_x)==PE_NRD_M14A_TNR_HD)? \
	"PE_NRD_M14A_TNR_HD":\
	((_x)==PE_NRD_M14A_TNR_UD)? \
	"PE_NRD_M14A_TNR_UD":"PE_NRD_M14A_TNR_NUM")

#define PE_NRD_HW_M14A_DNR_FMT_TO_STR(_x)		(\
	((_x)==PE_NRD_M14A_DNR_SD)? \
	"PE_NRD_M14A_DNR_SD":\
	((_x)==PE_NRD_M14A_DNR_HD)? \
	"PE_NRD_M14A_DNR_HD":\
	((_x)==PE_NRD_M14A_DNR_TP)? \
	"PE_NRD_M14A_DNR_TP":"PE_NRD_M14A_DNR_NUM")

/**
 *	pe read default reg menu enumeration(for M14BX)
 */
typedef enum
{
	PE_NRD_HW_M14B_RD_DFLT_DISPLAY = 0,		///< menu display
	PE_NRD_HW_M14B_RD_DFLT_TNR,				///< tnr
	PE_NRD_HW_M14B_RD_DFLT_DNR,				///< dnr
	PE_NRD_HW_M14B_RD_DFLT_IFC,				///< ifc
	PE_NRD_HW_M14B_RD_TNR_MNUM,				///< tnr matching number
	PE_NRD_HW_M14B_RD_P0M_DATA,				///< p0m data
	PE_NRD_HW_M14B_RD_P0M_MASK,				///< p0m mask
	PE_NRD_HW_M14B_RD_DNR_MNUM,				///< dnr matching number
	PE_NRD_HW_M14B_RD_P0D_DATA,				///< p0d data
	PE_NRD_HW_M14B_RD_P0D_MASK,				///< p0d mask
	PE_NRD_HW_M14B_RD_DFLT_MAX				///< max num
}
PE_NRD_HW_M14B_RD_DFLT_MENU;

/**
 *	pe read default reg menu enumeration(for M14AX)
 */
typedef enum
{
	PE_NRD_HW_M14A_RD_DFLT_DISPLAY = 0,		///< menu display
	PE_NRD_HW_M14A_RD_DFLT_TNR_L,			///< tnr L
	PE_NRD_HW_M14A_RD_DFLT_TNR_R,			///< tnr R
	PE_NRD_HW_M14A_RD_DFLT_DNR_L,			///< dnr L
	PE_NRD_HW_M14A_RD_DFLT_DNR_R,			///< dnr R
	PE_NRD_HW_M14A_RD_DFLT_MAX				///< max num
}
PE_NRD_HW_M14A_RD_DFLT_MENU;

/**
 *	pe nrd tnr fmt(for M14BX)
 */
typedef enum
{
	PE_NRD_M14B_TNR_HD_I = 0,		///< hd i
	PE_NRD_M14B_TNR_HD_P,			///< hd p
	PE_NRD_M14B_TNR_HD_I_OLED,		///< hd i oled
	PE_NRD_M14B_TNR_HD_P_OLED,		///< hd p oled
	PE_NRD_M14B_TNR_COMP_HD_I_3D,	///< comp hd i 3d
	PE_NRD_M14B_TNR_COMP_HD_P_3D,	///< comp hd p 3d
	PE_NRD_M14B_TNR_PC,				///< pc
	PE_NRD_M14B_TNR_SD_I,			///< sd i
	PE_NRD_M14B_TNR_DTV_SD_480_I,	///< dtv sd 480 i
	PE_NRD_M14B_TNR_DTV_SD_576_I,	///< dtv sd 576 i
	PE_NRD_M14B_TNR_SD_P,			///< sd p
	PE_NRD_M14B_TNR_DTV_SD_P,		///< dtv sd p
	PE_NRD_M14B_TNR_AV_NTSC,		///< av, atv ntsc
	PE_NRD_M14B_TNR_AV_PAL,			///< av, atv pal
	PE_NRD_M14B_TNR_AV_SECAM,		///< av, atv secam
	PE_NRD_M14B_TNR_NUM				///< max num
}
PE_NRD_HW_M14B_TNR_FMT;

/**
 *	pe nrd tnr alpha lut fmt(for M14BX)
 */
typedef enum
{
	PE_NRD_M14B_TAL_HD_I = 0,	///< hd i
	PE_NRD_M14B_TAL_HD_P,		///< hd p
	PE_NRD_M14B_TAL_SD_I,		///< sd i
	PE_NRD_M14B_TAL_SD_P,		///< sd p
	PE_NRD_M14B_TAL_SECAM,		///< secam
	PE_NRD_M14B_TAL_NUM			///< max num
}
PE_NRD_HW_M14B_TAL_FMT;

/**
 *	pe nrd dnr fmt(for M14CX)
 */
typedef enum
{
	PE_NRD_M14C_DNR_ATV = 0,	///< atv
	PE_NRD_M14C_DNR_AV,		 	///< av
	PE_NRD_M14C_DNR_HD_I,		///< hd
	PE_NRD_M14C_DNR_HD_P,		///< hd
	PE_NRD_M14C_DNR_PC,			///< pc
	PE_NRD_M14C_DNR_SD_I,		///< sd
	PE_NRD_M14C_DNR_SD_P,		///< sd
	PE_NRD_M14C_DNR_NUM			///< max num
}
PE_NRD_HW_M14C_DNR_FMT;

/**
 *	pe nrd dnr fmt(for M14BX)
 */
typedef enum
{
	PE_NRD_M14B_DNR_ATV = 0,	///< atv
	PE_NRD_M14B_DNR_AV,			///< av
	PE_NRD_M14B_DNR_HD,			///< hd
	PE_NRD_M14B_DNR_PC,			///< pc
	PE_NRD_M14B_DNR_SD,			///< sd
	PE_NRD_M14B_DNR_NUM			///< max num
}
PE_NRD_HW_M14B_DNR_FMT;

/**
 *	pe nrd ifc fmt(for M14BX)
 */
typedef enum
{
	PE_NRD_M14B_IFC_ATV_NTSC = 0,	///< atv ntsc
	PE_NRD_M14B_IFC_PC,				///< pc(off)
	PE_NRD_M14B_IFC_NUM				///< max num
}
PE_NRD_HW_M14B_IFC_FMT;

/**
 *	pe nrd tnr fmt(for M14AX)
 */
typedef enum
{
	PE_NRD_M14A_TNR_SD = 0,			///< sd
	PE_NRD_M14A_TNR_SD_MC_OFF,		///< sd of mc off
	PE_NRD_M14A_TNR_ATV,			///< atv
	PE_NRD_M14A_TNR_DVR_ATV,		///< dvr atv
	PE_NRD_M14A_TNR_ATV_SECAM,		///< atv secam
	PE_NRD_M14A_TNR_AV,				///< av
	PE_NRD_M14A_TNR_AV_SECAM,		///< av secam
	PE_NRD_M14A_TNR_HD,				///< hd
	PE_NRD_M14A_TNR_UD,				///< ud
	PE_NRD_M14A_TNR_NUM				///< max num
}
PE_NRD_HW_M14A_TNR_FMT;

/**
 *	pe nrd dnr fmt(for M14AX)
 */
typedef enum
{
	PE_NRD_M14A_DNR_SD = 0,		///< sd
	PE_NRD_M14A_DNR_HD,			///< hd
	PE_NRD_M14A_DNR_TP,			///< test pattern mode
	PE_NRD_M14A_DNR_NUM			///< max num
}
PE_NRD_HW_M14A_DNR_FMT;

/**
 *	pe nrd module settings type
 */
typedef struct
{
	UINT32 reg_bnr_ac_h_en0;			///< previous bnr_ac_h_en0
	UINT32 reg_bnr_ac_h_en1;			///< previous bnr_ac_h_en1
	UINT32 cur_tnr_auto_enable;			///< current tnr auto enable
	UINT32 cur_tnr_master_gain;			///< current tnr_master_gain
	UINT32 pre_tnr_master_gain;			///< previous tnr_master_gain
	PE_NRD_HW_M14A_TNR_FMT tnr0a_fmt;	///< previous p0l tnr fmt
	PE_NRD_HW_M14A_TNR_FMT tnr1a_fmt;	///< previous p0r tnr fmt
	PE_NRD_HW_M14A_DNR_FMT dnr0a_fmt;	///< previous p0l dnr fmt
	PE_NRD_HW_M14A_DNR_FMT dnr1a_fmt;	///< previous p0r dnr fmt
	PE_NRD_HW_M14B_TNR_FMT tnr0b_fmt;	///< previous p0m tnr fmt
	PE_NRD_HW_M14B_TAL_FMT tal0b_fmt;	///< previous p0m tnr alpha lut fmt
	PE_NRD_HW_M14B_DNR_FMT dnr0b_fmt;	///< previous p0d dnr fmt
	PE_NRD_HW_M14B_IFC_FMT ifc0b_fmt;	///< previous p0d ifc fmt
	PE_NRD_HW_M14C_DNR_FMT dnr0c_fmt;	///< previous p0d dnr fmt
}
PE_NRD_HW_M14_SETTINGS_T;

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/
int PE_NRD_HW_M14_Init(PE_CFG_CTRL_T *pstParams);
int PE_NRD_HW_M14_SetDebugSettings(LX_PE_DBG_SETTINGS_T *pstParams);
int PE_NRD_HW_M14_SetDefault(LX_PE_DEFAULT_SETTINGS_T *pstParams);
int PE_NRD_HW_M14_RdDefault(LX_PE_ETC_TBL_T *pstParams);
int PE_NRD_HW_M14_SetDnrCmnCtrl(void *pstParams);
int PE_NRD_HW_M14_GetDnrCmnCtrl(void *pstParams);
int PE_NRD_HW_M14_SetDnrDetailCtrl(void *pstParams);
int PE_NRD_HW_M14_GetDnrDetailCtrl(void *pstParams);
int PE_NRD_HW_M14_SetTnrCmnCtrl(void *pstParams);
int PE_NRD_HW_M14_GetTnrCmnCtrl(void *pstParams);
int PE_NRD_HW_M14_SetTnrDetailCtrl(void *pstParams);
int PE_NRD_HW_M14_GetTnrDetailCtrl(void *pstParams);
int PE_NRD_HW_M14_SetTnrGain(PE_TSK_M14_CFG_T *pCfg);
int PE_NRD_HW_M14_SetDnrMode(PE_TSK_M14_CFG_T *pCfg);

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _PE_NRD_HW_M14_H_ */
