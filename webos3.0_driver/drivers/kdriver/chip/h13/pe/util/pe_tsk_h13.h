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

/** @file pe_tsk_h13.h
 *
 *  task func. for picture enhance. ( used only within kdriver )
 *	
 *	@author		Seung-Jun,Youm(sj.youm@lge.com)
 *	@version	0.1
 *	@note		
 *	@date		2012.04.09
 *	@see		
 */

#ifndef	_PE_TSK_H13_H_
#define	_PE_TSK_H13_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include "pe_kapi.h"
#include "pe_inf_h13.h"

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
 *	pe task h13 operation type
 */
typedef struct
{
	UINT32 pause_task;		///< 1:pause, 0:task run
	UINT32 timeout;			///< task timeout, msec
	UINT32 dbgmode;			///< debug mode
}
PE_TSK_H13_OPR_T;

/**
 *	pe lrcr histo control type
 */
typedef struct
{
	UINT32 win_mod;			///< window mode, see PE_HST_HW_H13_LRC_HST_WIN_MOD
	UINT32 tb0_t_pos[4];	///< tb0 t position, [0]x0,[1]y0,[2]x1,[3]y1
	UINT32 tb0_b_pos[4];	///< tb0 b position, [0]x0,[1]y0,[2]x1,[3]y1
	UINT32 tb1_t_pos[4];	///< tb1 t position, [0]x0,[1]y0,[2]x1,[3]y1
	UINT32 tb1_b_pos[4];	///< tb1 b position, [0]x0,[1]y0,[2]x1,[3]y1
	UINT32 ss0_l_pos[4];	///< ss0 l position, [0]x0,[1]y0,[2]x1,[3]y1
	UINT32 ss0_r_pos[4];	///< ss0 r position, [0]x0,[1]y0,[2]x1,[3]y1
	UINT32 ss1_l_pos[4];	///< ss1 l position, [0]x0,[1]y0,[2]x1,[3]y1
	UINT32 ss1_r_pos[4];	///< ss1 r position, [0]x0,[1]y0,[2]x1,[3]y1
	UINT32 pxl_cnt[4];		///< pixel count, [0]tb0,[1]ss0,[2]tb1,[3]ss1
}
PE_TSK_H13_LRC_HST_CTRL_T;

/**
 *	pe task h13 cfg type
 */
typedef struct {
	PE_INF_H13_SETTINGS_T inf;
	PE_TSK_H13_OPR_T opr;
	PE_TSK_H13_LRC_HST_CTRL_T lrc;
}
PE_TSK_H13_CFG_T;

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/
int PE_TSK_H13_SetDebugSettings(LX_PE_DBG_SETTINGS_T *pstParams);
int PE_TSK_H13_SetTaskOperation(LX_PE_ETC_TBL_T *pstParams);
int PE_TSK_H13_GetTaskOperation(LX_PE_ETC_TBL_T *pstParams);
int PE_TSK_H13_CreatePicTask(void);
int PE_TSK_H13_StartPicTask(PE_CFG_CTRL_T *pstParams);
int PE_TSK_H13_StopPicTask(void);

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _PE_TSK_H13_H_ */
