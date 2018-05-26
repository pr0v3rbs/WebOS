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

/** @file pe_cmn_hw_h15.h
 *
 *  driver header for picture enhance common functions. ( used only within kdriver )
 *	
 *	@author		Seung-Jun,Youm(sj.youm@lge.com)
 *	@version	0.1
 *	@note		
 *	@date		2012.03.15
 *	@see		
 */

#ifndef	_PE_CMN_HW_H15_H_
#define	_PE_CMN_HW_H15_H_

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

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/
/**
* get db info type enum
* - SEC_XXX needs section type number
*/
typedef enum
{
	PE_CMN_HW_H15_DB_INF_IS_DB_INIT =  0, ///< is initialized or not
	PE_CMN_HW_H15_DB_INF_TOP_VERSION,     ///< top version
	PE_CMN_HW_H15_DB_INF_TOP_BASE,        ///< top base addr
	PE_CMN_HW_H15_DB_INF_TOP_SEC_NUM,     ///< top section total number
	PE_CMN_HW_H15_DB_INF_SEC_BASE,        ///< section base address
	PE_CMN_HW_H15_DB_INF_SEC_SIZE,        ///< section size
	PE_CMN_HW_H15_DB_INF_SEC_VERSION,     ///< section version
	PE_CMN_HW_H15_DB_INF_SEC_TYPE,        ///< section type
	PE_CMN_HW_H15_DB_INF_SEC_ITEM_NUM,    ///< section item total number
	PE_CMN_HW_H15_DB_INF_SEC_ITEM_SIZE,   ///< section item size
	PE_CMN_HW_H15_DB_INF_SEC_DATA_BASE,   ///< section data base
	PE_CMN_HW_H15_DB_INF_MD0_SHP_FW_EN,   ///< md0 shp fw en
	PE_CMN_HW_H15_DB_INF_MD0_HST_FW_EN,   ///< md0 hst fw en
	PE_CMN_HW_H15_DB_INF_MD0_SCL_FW_EN,   ///< md0 scl fw en
	PE_CMN_HW_H15_DB_INF_MD0_TNR_FW_EN,   ///< md0 tnr fw en
	PE_CMN_HW_H15_DB_INF_MD0_DNR_FW_EN,   ///< md0 dnr fw en
	PE_CMN_HW_H15_DB_INF_NUM              ///< max num
}
PE_CMN_HW_H15_DB_INF_TYPE;

/**
 *	pe cmn operation mode type
 */
typedef struct{
	UINT32 pe1_mode_ud;		///< pe1 ud mode, 1:yes,0:no, not support on H13a
	UINT32 pe1_mode_3d;		///< pe1 3d mode, 1:yes,0:no
}
PE_CMN_HW_H15_OPR_MODE_T;
/**
 *	pe common settings type
 */
typedef struct
{
	PE_CMN_HW_H15_OPR_MODE_T opr_inf;	///< operation info
	UINT32 pre_ihsv_v_gain;				///< pre_ihsv_v_gain
	UINT32 pre_ihsv_v_ofst;				///< pre_ihsv_v_ofst
	UINT32 cur_lvctrl_gain;				///< cur_lvctrl_gain
	UINT32 cur_lvctrl_ofst;				///< cur_lvctrl_ofst
	UINT32 pre_lvctrl_gain;				///< pre_lvctrl_gain
	UINT32 pre_lvctrl_ofst;				///< pre_lvctrl_ofst
	UINT32 dbg_print_lvl;				///< pe debug print level
	UINT32 dbg_bypass[LX_PE_WIN_NUM];	///< bypass each module
	UINT32 db_initialized;				///< db_initialized
	UINT32 db_mode_data[1];				///< db_mode_data, see PE_DDR_H15_MD0_T 
}
PE_CMN_HW_H15_SETTINGS_T;

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/
int PE_CMN_HW_H15_Init(PE_CFG_CTRL_T *pstParams);
int PE_CMN_HW_H15_SetDebugSettings(LX_PE_DBG_SETTINGS_T *pstParams);
int PE_CMN_HW_H15_GetDebugSettings(LX_PE_DBG_SETTINGS_T *pstParams);
int PE_CMN_HW_H15_SetOperationMode(PE_TSK_H15_CFG_T *pCfg);
int PE_CMN_HW_H15_SetContrast(LX_PE_CMN_CONTRAST_T *pstParams);
int PE_CMN_HW_H15_GetContrast(LX_PE_CMN_CONTRAST_T *pstParams);
int PE_CMN_HW_H15_SetBrightness(LX_PE_CMN_BRIGHTNESS_T *pstParams);
int PE_CMN_HW_H15_GetBrightness(LX_PE_CMN_BRIGHTNESS_T *pstParams);
int PE_CMN_HW_H15_SetSaturation(LX_PE_CMN_SATURATION_T *pstParams);
int PE_CMN_HW_H15_GetSaturation(LX_PE_CMN_SATURATION_T *pstParams);
int PE_CMN_HW_H15_SetHue(LX_PE_CMN_HUE_T *pstParams);
int PE_CMN_HW_H15_GetHue(LX_PE_CMN_HUE_T *pstParams);
int PE_CMN_HW_H15_SetLevelCtrl(LX_PE_CMN_LEVEL_CTRL_T *pstParams);
int PE_CMN_HW_H15_GetLevelCtrl(LX_PE_CMN_LEVEL_CTRL_T *pstParams);
int PE_CMN_HW_H15_SetLevelCtrlMode(PE_TSK_H15_CFG_T *pCfg);
int PE_CMN_HW_H15_DownloadDb(PE_CFG_CTRL_T *pstParams);
UINT32 PE_CMN_HW_H15_GetDbInfo(UINT32 inf_type, UINT32 sec_type);
UINT32 PE_CMN_HW_H15_SetDbInfo(UINT32 inf_type, UINT32 sec_type, UINT32 value);

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _PE_CMN_HW_H15_H_ */
