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

/** @file pe_ccm_hw_h15.h
 *
 *  driver header for picture enhance color correction module functions. ( used only within kdriver )
 *	
 *	@author		Seung-Jun,Youm(sj.youm@lge.com)
 *	@version	0.1
 *	@note		
 *	@date		2012.04.12
 *	@see		
 */

#ifndef	_PE_CCM_HW_H15_H_
#define	_PE_CCM_HW_H15_H_

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
#define PE_CCM_HW_H15_LUT_A		0
#define PE_CCM_HW_H15_LUT_B		1

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/
/**
 *	pe ccm 3d fmt check type for auto cr gain setting
 */
typedef enum
{
	PE_CCM_HW_H15_D3D_CHK_NORMAL = 0,		///< normal
	PE_CCM_HW_H15_D3D_CHK_NOT_TB,			///< not tb(not normal)
	PE_CCM_HW_H15_D3D_CHK_NOT_SS,			///< not ss(not normal)
	PE_CCM_HW_H15_D3D_CHK_MAX				///< max num
}
PE_CCM_HW_H15_D3D_CHK;
/**
 *	pe ccm control type
 */
typedef struct
{
	UINT32 r_h_select;		///< lrcr r histo selection
	UINT32 l_h_select;		///< lrcr l histo selection
	UINT32 r_p_select;		///< lrcr r proc selection
	UINT32 l_p_select;		///< lrcr l proc selection
	UINT32 r_o_select;		///< lrcr r out selection
	UINT32 l_o_select;		///< lrcr l out selection
	UINT32 ppr_bypass;		///< lrcr r bypass
	UINT32 ppl_bypass;		///< lrcr l bypass
	UINT32 ff_mode_on;		///< lrcr ff mode enable
	UINT32 ll_mode_en;		///< lrcr line by line mode enable
	UINT32 histo_mode;		///< lrcr histo mode
	UINT32 lrcr_gain;		///< lrcr gain
	UINT32 lut_type;		///< when HW is reset, table type = B. used in H15 only
}
PE_CCM_HW_H15_CTRL_T;
/**
 *	pe ccm settings type
 */
typedef struct
{
	PE_CCM_HW_H15_CTRL_T ccm_inf;	///< ccm info
	LX_PE_CCM_AUTO_CR_T auto_cr;	///< auto correction
}
PE_CCM_HW_H15_SETTINGS_T;

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/
int PE_CCM_HW_H15_Init(PE_CFG_CTRL_T *pstParams);
int PE_CCM_HW_H15_SetDebugSettings(LX_PE_DBG_SETTINGS_T *pstParams);
int PE_CCM_HW_H15_SetGammaLut(LX_PE_CCM_GAMMALUT_T *pstParams);
int PE_CCM_HW_H15_GetGammaLut(LX_PE_CCM_GAMMALUT_T *pstParams);
int PE_CCM_HW_H15_SetPixelReplacement(LX_PE_CCM_PIXEL_REPLACE_T *pstParams);
int PE_CCM_HW_H15_GetPixelReplacement(LX_PE_CCM_PIXEL_REPLACE_T *pstParams);
int PE_CCM_HW_H15_SetWhiteBalance(LX_PE_CCM_WB_T *pstParams);
int PE_CCM_HW_H15_GetWhiteBalance(LX_PE_CCM_WB_T *pstParams);
int PE_CCM_HW_H15_SetWhiteBalance(LX_PE_CCM_WB_T *pstParams);
int PE_CCM_HW_H15_SetAutoCorrection(LX_PE_CCM_AUTO_CR_T *pstParams);
int PE_CCM_HW_H15_GetAutoCorrection(LX_PE_CCM_AUTO_CR_T *pstParams);
int PE_CCM_HW_H15_SetLrcrConfig(PE_TSK_H15_CFG_T *pCfg);
int PE_CCM_HW_H15_SetAutoCrGain(PE_TSK_H15_CFG_T *pCfg);
int PE_CCM_HW_H15_GetCurCcmSettings(PE_CCM_HW_H15_SETTINGS_T *pstParams);

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _PE_CCM_HW_H15_H_ */
