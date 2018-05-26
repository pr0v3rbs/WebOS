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

/** @file pe_cmn_hw_h13.h
 *
 *  driver header for picture enhance common functions. ( used only within kdriver )
 *	
 *	@author		Seung-Jun,Youm(sj.youm@lge.com)
 *	@version	0.1
 *	@note		
 *	@date		2012.03.15
 *	@see		
 */

#ifndef	_PE_CMN_HW_H13_H_
#define	_PE_CMN_HW_H13_H_

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

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/
/**
 *	pe cmn operation mode type
 */
typedef struct{
	UINT32 pe1_mode_ud;		///< pe1 ud mode, 1:yes,0:no
	UINT32 pe1_mode_3d;		///< pe1 3d mode, 1:yes,0:no
}
PE_CMN_HW_H13_OPR_MODE_T;
/**
 *	pe common settings type
 */
typedef struct
{
	PE_CMN_HW_H13_OPR_MODE_T opr_inf;	///< operation info
	UINT32 dbg_print_lvl;				///< pe debug print level
	UINT32 dbg_bypass[LX_PE_WIN_NUM];	///< bypass each module
}
PE_CMN_HW_H13_SETTINGS_T;

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/
int PE_CMN_HW_H13_Init(PE_CFG_CTRL_T *pstParams);
int PE_CMN_HW_H13_SetDebugSettings(LX_PE_DBG_SETTINGS_T *pstParams);
int PE_CMN_HW_H13_GetDebugSettings(LX_PE_DBG_SETTINGS_T *pstParams);
int PE_CMN_HW_H13_SetOperationMode(PE_TSK_H13_CFG_T *pCfg);
int PE_CMN_HW_H13_SetContrast(LX_PE_CMN_CONTRAST_T *pstParams);
int PE_CMN_HW_H13_GetContrast(LX_PE_CMN_CONTRAST_T *pstParams);
int PE_CMN_HW_H13_SetBrightness(LX_PE_CMN_BRIGHTNESS_T *pstParams);
int PE_CMN_HW_H13_GetBrightness(LX_PE_CMN_BRIGHTNESS_T *pstParams);
int PE_CMN_HW_H13_SetSaturation(LX_PE_CMN_SATURATION_T *pstParams);
int PE_CMN_HW_H13_GetSaturation(LX_PE_CMN_SATURATION_T *pstParams);
int PE_CMN_HW_H13_SetHue(LX_PE_CMN_HUE_T *pstParams);
int PE_CMN_HW_H13_GetHue(LX_PE_CMN_HUE_T *pstParams);
int PE_CMN_HW_H13_SetLevelCtrl(LX_PE_CMN_LEVEL_CTRL_T *pstParams);
int PE_CMN_HW_H13_GetLevelCtrl(LX_PE_CMN_LEVEL_CTRL_T *pstParams);

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _PE_CMN_HW_H13_H_ */
