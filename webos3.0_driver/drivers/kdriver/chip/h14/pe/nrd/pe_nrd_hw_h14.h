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

/** @file pe_nrd_hw_h14.h
 *
 *  driver header for picture enhance noise reduction. ( used only within kdriver )
 *	
 *	@author		Seung-Jun,Youm(sj.youm@lge.com)
 *	@version	0.1
 *	@note		
 *	@date		2012.03.15
 *	@see		
 */

#ifndef	_PE_NRD_HW_H14_H_
#define	_PE_NRD_HW_H14_H_

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
/* before conforming parameter set for h14 */
#define PE_USE_TEMPORAL_NRD_PARAMETER_SET

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
	PE_NRD_HW_H14_RD_DFLT_DISPLAY = 0,		///< menu display
	PE_NRD_HW_H14_RD_DFLT_TNR_L,			///< tnr L
	PE_NRD_HW_H14_RD_DFLT_TNR_R,			///< tnr R
	PE_NRD_HW_H14_RD_DFLT_DNR_L,			///< dnr L
	PE_NRD_HW_H14_RD_DFLT_DNR_R,			///< dnr R
	PE_NRD_HW_H14_RD_DFLT_MAX				///< max num
}
PE_NRD_HW_H14_RD_DFLT_MENU;

/**
 *	pe nrd tnr fmt(for H14)
 */
typedef enum
{
	PE_NRD_H14_TNR_SD_I = 0,		///< sd i
	PE_NRD_H14_TNR_SD_P,			///< sd p
	PE_NRD_H14_TNR_SD_MC_OFF,		///< sd of mc off
	PE_NRD_H14_TNR_ATV,				///< atv
	PE_NRD_H14_TNR_DVR_ATV,			///< dvr atv
	PE_NRD_H14_TNR_ATV_SECAM,		///< atv secam
	PE_NRD_H14_TNR_AV,				///< av
	PE_NRD_H14_TNR_AV_SECAM,		///< av secam
	PE_NRD_H14_TNR_HD_I,			///< hd i
	PE_NRD_H14_TNR_HD_P,			///< hd p
	PE_NRD_H14_TNR_UD,				///< ud
	PE_NRD_H14_TNR_NUM				///< max num
}
PE_NRD_HW_H14_TNR_FMT;

/**
 *	pe nrd dnr fmt(for H14)
 */
typedef enum
{
	PE_NRD_H14_DNR_SD = 0,		///< sd
	PE_NRD_H14_DNR_HD,			///< hd
	PE_NRD_H14_DNR_TP,			///< test pattern mode
	PE_NRD_H14_DNR_NUM			///< max num
}
PE_NRD_HW_H14_DNR_FMT;
/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/
int PE_NRD_HW_H14_Init(PE_CFG_CTRL_T *pstParams);
int PE_NRD_HW_H14_SetDebugSettings(LX_PE_DBG_SETTINGS_T *pstParams);
int PE_NRD_HW_H14_SetDefault(LX_PE_DEFAULT_SETTINGS_T *pstParams);
int PE_NRD_HW_H14_RdDefault(LX_PE_ETC_TBL_T *pstParams);
int PE_NRD_HW_H14_SetDnrCmnCtrl(void *pstParams);
int PE_NRD_HW_H14_GetDnrCmnCtrl(void *pstParams);
int PE_NRD_HW_H14_SetDnrDetailCtrl(void *pstParams);
int PE_NRD_HW_H14_GetDnrDetailCtrl(void *pstParams);
int PE_NRD_HW_H14_SetTnrCmnCtrl(void *pstParams);
int PE_NRD_HW_H14_GetTnrCmnCtrl(void *pstParams);
int PE_NRD_HW_H14_SetTnrDetailCtrl(void *pstParams);
int PE_NRD_HW_H14_GetTnrDetailCtrl(void *pstParams);

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _PE_NRD_HW_H14_H_ */
