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

/** @file pe_nrd_hw_h13.h
 *
 *  driver header for picture enhance noise reduction. ( used only within kdriver )
 *
 *	@author		Seung-Jun,Youm(sj.youm@lge.com)
 *	@version	0.1
 *	@note
 *	@date		2012.03.15
 *	@see
 */

#ifndef	_PE_NRD_HW_H13_H_
#define	_PE_NRD_HW_H13_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include "pe_kapi.h"
#include "pe_tsk_h13.h"

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
/* define for bnr_ac_h_en 0x0(protection) for small size file play */
#define PE_HW_H13_USE_DNR_MODE_ON_TASK

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
	PE_NRD_HW_H13_RD_DFLT_DISPLAY = 0,		///< menu display
	PE_NRD_HW_H13_RD_DFLT_TNR_L,			///< tnr L
	PE_NRD_HW_H13_RD_DFLT_TNR_R,			///< tnr R
	PE_NRD_HW_H13_RD_DFLT_DNR_L,			///< dnr L
	PE_NRD_HW_H13_RD_DFLT_DNR_R,			///< dnr R
	PE_NRD_HW_H13_RD_DFLT_MAX				///< max num
}
PE_NRD_HW_H13_RD_DFLT_MENU;

/**
 *	pe nrd tnr fmt(for H13BX)
 */
typedef enum
{
	PE_NRD_H13B_TNR_SD = 0,			///< sd
	PE_NRD_H13B_TNR_SD_MC_OFF,		///< sd of mc off
	PE_NRD_H13B_TNR_ATV,			///< atv
	PE_NRD_H13B_TNR_DVR_ATV,		///< dvr atv
	PE_NRD_H13B_TNR_ATV_SECAM,		///< atv secam
	PE_NRD_H13B_TNR_AV,				///< av
	PE_NRD_H13B_TNR_AV_SECAM,		///< av secam
	PE_NRD_H13B_TNR_HD,				///< hd dtv, hdmi p, comp p
	PE_NRD_H13B_TNR_HD_I,			///< hd i
	PE_NRD_H13B_TNR_UD,				///< ud
	PE_NRD_H13B_TNR_NUM				///< max num
}
PE_NRD_HW_H13B_TNR_FMT;

/**
 *	pe nrd dnr fmt(for H13BX)
 */
typedef enum
{
	PE_NRD_H13B_DNR_SD = 0,		///< sd
	PE_NRD_H13B_DNR_HD,			///< hd
	PE_NRD_H13B_DNR_TP,			///< test pattern mode
	PE_NRD_H13B_DNR_NUM			///< max num
}
PE_NRD_HW_H13B_DNR_FMT;

/**
 *	pe nrd tnr fmt(for H13AX)
 */
typedef enum
{
	PE_NRD_H13A_TNR_SD_I = 0,	///< sd interlace
	PE_NRD_H13A_TNR_SD_P,		///< sd progress
	PE_NRD_H13A_TNR_HD_I,		///< hd interlace
	PE_NRD_H13A_TNR_HD_P,		///< hd progress
	PE_NRD_H13A_TNR_NUM			///< max num
}
PE_NRD_HW_H13A_TNR_FMT;

/**
 *	pe nrd dnr fmt(for H13AX)
 */
typedef enum
{
	PE_NRD_H13A_DNR_SD_I = 0,	///< sd interlace
	PE_NRD_H13A_DNR_SD_P,		///< sd progress
	PE_NRD_H13A_DNR_HD_I,		///< hd interlace
	PE_NRD_H13A_DNR_HD_P,		///< hd progress
	PE_NRD_H13A_DNR_PC,			///< pc
	PE_NRD_H13A_DNR_AV,			///< av
	PE_NRD_H13A_DNR_DTV_SD,		///< dtv sd
	PE_NRD_H13A_DNR_NUM			///< max num
}
PE_NRD_HW_H13A_DNR_FMT;

/**
 *	pe nrd module settings type
 */
typedef struct
{
	UINT32 reg_bnr_ac_h_en0;			///< previous bnr_ac_h_en0
	UINT32 reg_bnr_ac_h_en1;			///< previous bnr_ac_h_en1
}
PE_NRD_HW_H13_SETTINGS_T;

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/
int PE_NRD_HW_H13_Init(PE_CFG_CTRL_T *pstParams);
int PE_NRD_HW_H13_SetDebugSettings(LX_PE_DBG_SETTINGS_T *pstParams);
int PE_NRD_HW_H13_SetDefault(LX_PE_DEFAULT_SETTINGS_T *pstParams);
int PE_NRD_HW_H13_RdDefault(LX_PE_ETC_TBL_T *pstParams);
int PE_NRD_HW_H13_SetDnrCmnCtrl(void *pstParams);
int PE_NRD_HW_H13_GetDnrCmnCtrl(void *pstParams);
int PE_NRD_HW_H13_SetDnrDetailCtrl(void *pstParams);
int PE_NRD_HW_H13_GetDnrDetailCtrl(void *pstParams);
int PE_NRD_HW_H13_SetTnrCmnCtrl(void *pstParams);
int PE_NRD_HW_H13_GetTnrCmnCtrl(void *pstParams);
int PE_NRD_HW_H13_SetTnrDetailCtrl(void *pstParams);
int PE_NRD_HW_H13_GetTnrDetailCtrl(void *pstParams);
int PE_NRD_HW_H13_SetDnrMode(PE_TSK_H13_CFG_T *pCfg);

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _PE_NRD_HW_H13_H_ */
