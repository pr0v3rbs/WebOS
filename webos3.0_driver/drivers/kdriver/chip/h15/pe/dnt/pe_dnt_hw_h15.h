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

/** @file pe_dnt_hw_h15.h
 *
 *  driver header for picture enhance deinterlacer. ( used only within kdriver )
 *
 *	@author		Seung-Jun,Youm(sj.youm@lge.com)
 *	@version	0.1
 *	@note
 *	@date		2012.04.12
 *	@see
 */

#ifndef	_PE_DNT_HW_H15_H_
#define	_PE_DNT_HW_H15_H_

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
#define PE_DNT_HW_H15_TNR_DBG_OFF	0x0
#define PE_DNT_HW_H15_TNR_DBG__ON	0x1
#define PE_DNT_HW_H15_CLC_OFF		0x0
#define PE_DNT_HW_H15_CLC__ON		0x1
#define PE_DNT_HW_H15_FMD_OFF		0x0
#define PE_DNT_HW_H15_FMD__ON		0x1
#define PE_DNT_HW_H15_TAVA_OFF		0x0
#define PE_DNT_HW_H15_TAVA__ON		0x1
/* vfilter enable protection */
#define PE_DNT_HW_H15_VFY_EN_PROTECT_OFF	0x0
#define PE_DNT_HW_H15_VFT_EN_PROTECT__ON	0x1
/* vfilter hmc protect min, max th enable protection(pt) */
#define PE_DNT_HW_H15_VFY_MIN_MAX_PT_OFF	0x0
#define PE_DNT_HW_H15_VFT_MIN_MAX_PT__ON	0x1

/**	20131108, sj.youm,
*	define for set game mode by fw and clc,fmd on,off
*   this is valid for PE_HW_H15A_USE_REG_CTRL defined.
*/
#define PE_DNT_HW_H15A_SET_GAME_MODE_BY_FW

/**	20131220, sj.youm,
*	define for set use tnr mode by fw
*   this is valid for PE_HW_H15A_USE_REG_CTRL defined.
*/
#define PE_DNT_HW_H15A_SET_USE_TNR_M_BY_FW

/**	20140104, sj.youm,
*	define for force vfilter y min,max protection on drv
*	(because th_min,max are not controled by fw at all)
*   this is valid for PE_HW_H15A_USE_REG_CTRL defined.
*/
#define PE_DNT_HW_H15A_FORCE_VFY_MIN_MAX_PT

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/
#define PE_DNT_HW_H15A_IPC_FMT_TO_STR(_x)		(\
	((_x)==PE_DNT_H15A_IPC_ATV_480)? \
	"PE_DNT_H15A_IPC_ATV_480":\
	((_x)==PE_DNT_H15A_IPC_ATV_576)? \
	"PE_DNT_H15A_IPC_ATV_576":\
	((_x)==PE_DNT_H15A_IPC_HD)? \
	"PE_DNT_H15A_IPC_HD":\
	((_x)==PE_DNT_H15A_IPC_SD)? \
	"PE_DNT_H15A_IPC_SD":\
	((_x)==PE_DNT_H15A_IPC_DTV_480)? \
	"PE_DNT_H15A_IPC_DTV_480":\
	((_x)==PE_DNT_H15A_IPC_DTV_576)? \
	"PE_DNT_H15A_IPC_DTV_576":\
	((_x)==PE_DNT_H15A_IPC_AV_480)? \
	"PE_DNT_H15A_IPC_AV_480":\
	"PE_DNT_H15A_IPC_NUM")

#define PE_DNT_HW_H15A_CLC_FMT_TO_STR(_x)		(\
	((_x)==PE_DNT_H15A_CLC_SD_50)? \
	"PE_DNT_H15A_CLC_SD_50":\
	((_x)==PE_DNT_H15A_CLC_SD_60)? \
	"PE_DNT_H15A_CLC_SD_60":\
	((_x)==PE_DNT_H15A_CLC_HD)? \
	"PE_DNT_H15A_CLC_HD":\
	((_x)==PE_DNT_H15A_CLC_PC)? \
	"PE_DNT_H15A_CLC_PC":"PE_DNT_H15A_CLC_NUM")

#define PE_DNT_HW_H15A_VFY_FMT_TO_STR(_x)		(\
	((_x)==PE_DNT_H15A_VFY_ATV)? \
	"PE_DNT_H15A_VFY_ATV":\
	((_x)==PE_DNT_H15A_VFY_AV)? \
	"PE_DNT_H15A_VFY_AV":\
	((_x)==PE_DNT_H15A_VFY_HD)? \
	"PE_DNT_H15A_VFY_HD":\
	((_x)==PE_DNT_H15A_VFY_PC)? \
	"PE_DNT_H15A_VFY_PC":\
	((_x)==PE_DNT_H15A_VFY_SD)? \
	"PE_DNT_H15A_VFY_SD":\
	((_x)==PE_DNT_H15A_VFY_SD_DTV)? \
	"PE_DNT_H15A_VFY_SD_DTV":\
	((_x)==PE_DNT_H15A_VFY_SECAM)? \
	"PE_DNT_H15A_VFY_SECAM":"PE_DNT_H15A_VFY_NUM")

#define PE_DNT_HW_H15A_TBL_IDX_TO_STR(_x)		(\
	((_x)==PE_DNT_HW_H15A_TBL_IDX_IPC)? "IPC":\
	((_x)==PE_DNT_HW_H15A_TBL_IDX_CLC)? "CLC":\
	((_x)==PE_DNT_HW_H15A_TBL_IDX_VFY)? "VFY":\
	"NUM")

/**
 *	pe table index enumeration(for H15AX)
 */
typedef enum
{
	PE_DNT_HW_H15A_TBL_IDX_IPC = 0,		///< ipc
	PE_DNT_HW_H15A_TBL_IDX_CLC,			///< clc
	PE_DNT_HW_H15A_TBL_IDX_VFY,			///< vfy
	PE_DNT_HW_H15A_TBL_IDX_NUM			///< max num
}
PE_DNT_HW_H15A_TBL_IDX;

/**
 *	pe read default reg menu enumeration(for H15AX)
 */
typedef enum
{
	PE_DNT_HW_H15A_RD_MENU_DISP = 0,	///< menu display
	PE_DNT_HW_H15A_RD_IPC1_DISP,		///< ipc1 display
	PE_DNT_HW_H15A_RD_IPC2_DISP,		///< ipc2 display
	PE_DNT_HW_H15A_RD_IPC1_DFLT,		///< ipc1 default
	PE_DNT_HW_H15A_RD_IPC1_USER,		///< ipc1 user
	PE_DNT_HW_H15A_RD_IPC1_MASK,		///< ipc1 mask
	PE_DNT_HW_H15A_RD_IPC1_DATA,		///< ipc1 data
	PE_DNT_HW_H15A_RD_CLC1_DISP,		///< clc1 display
	PE_DNT_HW_H15A_RD_CLC1_DFLT,		///< clc1 default
	PE_DNT_HW_H15A_RD_CLC1_USER,		///< clc1 user
	PE_DNT_HW_H15A_RD_CLC1_MASK,		///< clc1 mask
	PE_DNT_HW_H15A_RD_CLC1_DATA,		///< clc1 data
	PE_DNT_HW_H15A_RD_VFY1_DISP,		///< vfy1 display
	PE_DNT_HW_H15A_RD_VFY2_DISP,		///< vfy2 display
	PE_DNT_HW_H15A_RD_VFY1_DFLT,		///< vfy1 default
	PE_DNT_HW_H15A_RD_VFY1_USER,		///< vfy1 user
	PE_DNT_HW_H15A_RD_VFY1_MASK,		///< vfy1 mask
	PE_DNT_HW_H15A_RD_VFY1_DATA,		///< vfy1 data
	PE_DNT_HW_H15A_RD_MENU_MAX			///< max num
}
PE_DNT_HW_H15A_RD_DFLT_MENU;

/**
 *	pe dnt ipc fmt(for H15AX)
 */
typedef enum
{
	PE_DNT_H15A_IPC_ATV_480 = 0,	///< atv 480
	PE_DNT_H15A_IPC_ATV_576,		///< atv 576
	PE_DNT_H15A_IPC_HD,				///< hd
	PE_DNT_H15A_IPC_SD,				///< sd
	PE_DNT_H15A_IPC_DTV_480,		///< dtv 480
	PE_DNT_H15A_IPC_DTV_576,		///< dtv 576
	PE_DNT_H15A_IPC_AV_480,			///< av 480
	PE_DNT_H15A_IPC_NUM				///< max num
}
PE_DNT_HW_H15A_IPC_FMT;
/**
 *	pe dnt clc fmt(for H15AX)
 */
typedef enum
{
	PE_DNT_H15A_CLC_SD_50 = 0,		///< sd 50hz
	PE_DNT_H15A_CLC_SD_60,			///< sd 60hz
	PE_DNT_H15A_CLC_HD,				///< hd
	PE_DNT_H15A_CLC_PC,				///< pc(clc off)
	PE_DNT_H15A_CLC_NUM				///< max num
}
PE_DNT_HW_H15A_CLC_FMT;
/**
 *	pe dnt v filter y fmt(for H15AX)
 */
typedef enum
{
	PE_DNT_H15A_VFY_ATV = 0,		///< atv
	PE_DNT_H15A_VFY_AV,				///< av
	PE_DNT_H15A_VFY_HD,				///< hd
	PE_DNT_H15A_VFY_PC,				///< pc
	PE_DNT_H15A_VFY_SD,				///< sd
	PE_DNT_H15A_VFY_SD_DTV,			///< sd dtv
	PE_DNT_H15A_VFY_SECAM,			///< secam
	PE_DNT_H15A_VFY_NUM				///< max num
}
PE_DNT_HW_H15A_VFY_FMT;

/**
 *	pe dnt module settings type
 */
typedef struct
{
	#ifdef PE_HW_H15_FOR_BRINGUP
	PE_DNT_HW_H15A_IPC_FMT ipc0a_fmt;	///< previous nd1 ipc0a fmt for h15a
	PE_DNT_HW_H15A_VFY_FMT vfy0a_fmt;	///< previous dnr vfy0a_fmt for h15a
	PE_DNT_HW_H15A_CLC_FMT clc0a_fmt;	///< previous nd1 clc0a_fmt for h15a
	UINT32 force_cbcr_enb;				///< force_cbcr_enable
	UINT32 force_y_max_th;				///< force_y_max_th
	#else
	UINT32 cur_p0m_ipc_ctrl_00;			///< current p0m_ipc_ctrl_00 for h15a
	UINT32 pre_p0m_ipc_ctrl_00;			///< previous p0m_ipc_ctrl_00 for h15a
	UINT32 cur_p0m_ipc_ctrl_01;			///< current p0m_ipc_ctrl_01 for h15a
	UINT32 pre_p0m_ipc_ctrl_01;			///< previous p0m_ipc_ctrl_01 for h15a
	UINT32 cur_p0m_ipc_ctrl_06;			///< current p0m_ipc_ctrl_06 for h15a
	UINT32 pre_p0m_ipc_ctrl_06;			///< previous p0m_ipc_ctrl_06 for h15a
	UINT32 cur_p0m_clc_ctrl_00;			///< current p0m_clc_ctrl_00 for h15a
	UINT32 pre_p0m_clc_ctrl_00;			///< previous p0m_clc_ctrl_00 for h15a
	UINT32 cur_p0d_vfy_ctrl_00;			///< current p0d_vfy_ctrl_00 for h15a
	UINT32 cur_p0d_vfy_ctrl_02;			///< current p0d_vfy_ctrl_00 for h15a
	PE_DNT_HW_H15A_IPC_FMT ipc0b_fmt;	///< previous p0m ipc0b_fmt for h15a
	PE_DNT_HW_H15A_IPC_FMT ipc1b_fmt;	///< previous p0m ipc1b_fmt for h15a
	PE_DNT_HW_H15A_CLC_FMT clc0b_fmt;	///< previous p0m clc0b_fmt for h15a
	PE_DNT_HW_H15A_VFY_FMT vfy0b_fmt;	///< previous p0d vfy0b_fmt for h15a
	UINT32 force_cbcr_enb;				///< force_cbcr_enable
	UINT32 force_y_max_th;				///< force_y_max_th
	#endif
}
PE_DNT_HW_H15_SETTINGS_T;

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/
int PE_DNT_HW_H15_Init(PE_CFG_CTRL_T *pstParams);
int PE_DNT_HW_H15_SetDebugSettings(LX_PE_DBG_SETTINGS_T *pstParams);
int PE_DNT_HW_H15_SetDefault(LX_PE_DEFAULT_SETTINGS_T *pstParams);
int PE_DNT_HW_H15_RdDefault(LX_PE_ETC_TBL_T *pstParams);
int PE_DNT_HW_H15_SetFilmMode(LX_PE_DNT_FILMMODE_T *pstParams);
int PE_DNT_HW_H15_GetFilmMode(LX_PE_DNT_FILMMODE_T *pstParams);
int PE_DNT_HW_H15_SetLowDelayMode(LX_PE_DNT_LD_MODE_T *pstParams);
int PE_DNT_HW_H15_GetLowDelayMode(LX_PE_DNT_LD_MODE_T *pstParams);
int PE_DNT_HW_H15_SetClcMode(PE_TSK_H15_CFG_T *pCfg);
int PE_DNT_HW_H15_GetCurDntSettings(PE_DNT_HW_H15_SETTINGS_T *pstParams);

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _PE_DNT_HW_H15_H_ */
