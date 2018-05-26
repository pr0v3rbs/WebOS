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

/** @file pe_win_hw_h13.h
 *
 *  driver header for picture enhance window control functions. ( used only within kdriver )
 *	
 *	@author		Seung-Jun,Youm(sj.youm@lge.com)
 *	@version	0.1
 *	@note		
 *	@date		2011.06.11
 *	@see		
 */

#ifndef	_PE_WIN_HW_H13_H_
#define	_PE_WIN_HW_H13_H_

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

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/
/**
 *	pe window control type
 */
typedef struct
{
	UINT32 act0_x0;		///< pe actual window0, x0 position
	UINT32 act0_y0;		///< pe actual window0, y0 position
	UINT32 act0_x1;		///< pe actual window0, x1 position
	UINT32 act0_y1;		///< pe actual window0, y1 position
	UINT32 act1_x0;		///< pe actual window1, x0 position
	UINT32 act1_y0;		///< pe actual window1, y0 position
	UINT32 act1_x1;		///< pe actual window1, x1 position
	UINT32 act1_y1;		///< pe actual window1, y1 position
	UINT32 win0_x0;		///< pe actual window0 with offset, x0 position
	UINT32 win0_y0;		///< pe actual window0 with offset, y0 position
	UINT32 win0_x1;		///< pe actual window0 with offset, x1 position
	UINT32 win0_y1;		///< pe actual window0 with offset, y1 position
	UINT32 win1_x0;		///< pe actual window1 with offset, x0 position
	UINT32 win1_y0;		///< pe actual window1 with offset, y0 position
	UINT32 win1_x1;		///< pe actual window1 with offset, x1 position
	UINT32 win1_y1;		///< pe actual window1 with offset, y1 position
	UINT32 h_max;		///< pe horizontal max size
	UINT32 v_max;		///< pe vertical max size
	UINT32 win0_en[2];	///< pe win0 enable of [0]win#1,3(shp), [1]win#2(cen) 
	UINT32 win1_en[2];	///< pe win1 enable of [0]win#1,3(shp), [1]win#2(cen)
	UINT32 win01_en[2];	///< pe win01 enable of [0]win#1,3(shp), [1]win#2(cen)
}
PE_WIN_HW_H13_CTRL_T;

/**
 *	pe lrcr window control type
 */
typedef struct{
	UINT32 histo_l_x0;		///< lrcr reference win x0 position
	UINT32 histo_l_y0;		///< lrcr reference win y0 position
	UINT32 histo_l_x1;		///< lrcr reference win x1 position
	UINT32 histo_l_y1;		///< lrcr reference win y1 position
	UINT32 histo_r_x0;		///< lrcr destination win x0 position
	UINT32 histo_r_y0;		///< lrcr destination win y0 position
	UINT32 histo_r_x1;		///< lrcr destination win x1 position
	UINT32 histo_r_y1;		///< lrcr destination win y1 position
	UINT32 apply_r_x0;		///< lrcr apply win x0 position
	UINT32 apply_r_y0;		///< lrcr apply win y0 position
	UINT32 apply_r_x1;		///< lrcr apply win x1 position
	UINT32 apply_r_y1;		///< lrcr apply win y1 position
}
PE_WIN_HW_H13_LRCR_CTRL_T;

/**
 *	pe tnr measure window control type
 */
typedef struct{
	UINT32 h_size0;		///< h window0 size
	UINT32 v_size0;		///< v window0 size
	UINT32 h_size1;		///< h window1 size
	UINT32 v_size1;		///< v window1 size
}
PE_WIN_HW_H13_TNR_CTRL_T;

/**
 *	pe window settings type
 */
typedef struct
{
	PE_WIN_HW_H13_CTRL_T win_inf;			///< window info
	PE_WIN_HW_H13_LRCR_CTRL_T lrcr_inf;		///< lrcr win info
	PE_WIN_HW_H13_TNR_CTRL_T tnr_inf;		///< tnr measure win info
}
PE_WIN_HW_H13_SETTINGS_T;

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/
int PE_WIN_HW_H13_Init(PE_CFG_CTRL_T *pstParams);
int PE_WIN_HW_H13_SetDebugSettings(LX_PE_DBG_SETTINGS_T *pstParams);
int PE_WIN_HW_H13_SetEnhancingWindow(PE_TSK_H13_CFG_T *pCfg);
int PE_WIN_HW_H13_SetLrcrWindow(PE_TSK_H13_CFG_T *pCfg);
int PE_WIN_HW_H13_SetTnrMeasureWin(PE_TSK_H13_CFG_T *pCfg);
int PE_WIN_HW_H13_GetCurWinSettings(PE_WIN_HW_H13_SETTINGS_T *pstParams);

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _PE_WIN_HW_H13_H_ */

