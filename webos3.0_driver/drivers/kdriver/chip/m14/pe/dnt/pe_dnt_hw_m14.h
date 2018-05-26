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

/** @file pe_dnt_hw_m14.h
 *
 *  driver header for picture enhance deinterlacer. ( used only within kdriver )
 *
 *	@author		Seung-Jun,Youm(sj.youm@lge.com)
 *	@version	0.1
 *	@note
 *	@date		2012.04.12
 *	@see
 */

#ifndef	_PE_DNT_HW_M14_H_
#define	_PE_DNT_HW_M14_H_

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
#define PE_DNT_HW_M14_TNR_DBG_OFF	0x0
#define PE_DNT_HW_M14_TNR_DBG__ON	0x1
#define PE_DNT_HW_M14_CLC_OFF		0x0
#define PE_DNT_HW_M14_CLC__ON		0x1
#define PE_DNT_HW_M14_FMD_OFF		0x0
#define PE_DNT_HW_M14_FMD__ON		0x1
#define PE_DNT_HW_M14_TAVA_OFF		0x0
#define PE_DNT_HW_M14_TAVA__ON		0x1
/* vfilter enable protection */
#define PE_DNT_HW_M14_VFY_EN_PROTECT_OFF	0x0
#define PE_DNT_HW_M14_VFT_EN_PROTECT__ON	0x1
/* vfilter hmc protect min, max th enable protection(pt) */
#define PE_DNT_HW_M14_VFY_MIN_MAX_PT_OFF	0x0
#define PE_DNT_HW_M14_VFT_MIN_MAX_PT__ON	0x1

/**	20131108, sj.youm,
*	define for set game mode by fw and clc,tnr on,off
*	need to test for enable define
*/
#define PE_DNT_HW_M14A_SET_GAME_MODE_BY_FW

/**	20131108, sj.youm,
*	define for set game mode by fw and clc,fmd on,off
*   this is valid for PE_HW_M14B_USE_REG_CTRL defined.
*/
#ifdef PE_HW_M14B_USE_REG_CTRL
#define PE_DNT_HW_M14B_SET_GAME_MODE_BY_FW
#else
#undef PE_DNT_HW_M14B_SET_GAME_MODE_BY_FW
#endif

/**	20131220, sj.youm,
*	define for set use tnr mode by fw
*   this is valid for PE_HW_M14B_USE_REG_CTRL defined.
*/
#ifdef PE_HW_M14B_USE_REG_CTRL
#define PE_DNT_HW_M14B_SET_USE_TNR_M_BY_FW
#else
#undef PE_DNT_HW_M14B_SET_USE_TNR_M_BY_FW
#endif

/**	20140104, sj.youm,
*	define for force vfilter y min,max protection on drv
*	(because th_min,max are not controled by fw at all)
*   this is valid for PE_HW_M14B_USE_REG_CTRL defined.
*/
#ifdef PE_HW_M14B_USE_REG_CTRL
#define PE_DNT_HW_M14B_FORCE_VFY_MIN_MAX_PT
#else
#undef PE_DNT_HW_M14B_FORCE_VFY_MIN_MAX_PT
#endif

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/
#define PE_DNT_HW_M14B_IPC_FMT_TO_STR(_x)		(\
	((_x)==PE_DNT_M14B_IPC_ATV_480)? \
	"PE_DNT_M14B_IPC_ATV_480":\
	((_x)==PE_DNT_M14B_IPC_ATV_576)? \
	"PE_DNT_M14B_IPC_ATV_576":\
	((_x)==PE_DNT_M14B_IPC_AV_480)? \
	"PE_DNT_M14B_IPC_AV_480":\
	((_x)==PE_DNT_M14B_IPC_AV_576)? \
	"PE_DNT_M14B_IPC_AV_576":\
	((_x)==PE_DNT_M14B_IPC_DTV_480)? \
	"PE_DNT_M14B_IPC_DTV_480":\
	((_x)==PE_DNT_M14B_IPC_DTV_576)? \
	"PE_DNT_M14B_IPC_DTV_576":\
	((_x)==PE_DNT_M14B_IPC_DTV_HD)? \
	"PE_DNT_M14B_IPC_DTV_HD":\
	((_x)==PE_DNT_M14B_IPC_DTV_HD_TP)? \
	"PE_DNT_M14B_IPC_DTV_HD_TP":\
	((_x)==PE_DNT_M14B_IPC_HD)? \
	"PE_DNT_M14B_IPC_HD":\
	((_x)==PE_DNT_M14B_IPC_PC)? \
	"PE_DNT_M14B_IPC_PC":\
	((_x)==PE_DNT_M14B_IPC_SD_480)? \
	"PE_DNT_M14B_IPC_SD_480":\
	((_x)==PE_DNT_M14B_IPC_SD_576)? \
	"PE_DNT_M14B_IPC_SD_576":\
	((_x)==PE_DNT_M14B_IPC_SECAM)? \
	"PE_DNT_M14B_IPC_SECAM":"PE_DNT_M14B_IPC_NUM")

#define PE_DNT_HW_M14B_CLC_FMT_TO_STR(_x)		(\
	((_x)==PE_DNT_M14B_CLC_SD_50)? \
	"PE_DNT_M14B_CLC_SD_50":\
	((_x)==PE_DNT_M14B_CLC_SD_60)? \
	"PE_DNT_M14B_CLC_SD_60":\
	((_x)==PE_DNT_M14B_CLC_HD)? \
	"PE_DNT_M14B_CLC_HD":\
	((_x)==PE_DNT_M14B_CLC_PC)? \
	"PE_DNT_M14B_CLC_PC":"PE_DNT_M14B_CLC_NUM")

#define PE_DNT_HW_M14B_VFY_FMT_TO_STR(_x)		(\
	((_x)==PE_DNT_M14B_VFY_ATV)? \
	"PE_DNT_M14B_VFY_ATV":\
	((_x)==PE_DNT_M14B_VFY_AV)? \
	"PE_DNT_M14B_VFY_AV":\
	((_x)==PE_DNT_M14B_VFY_HD)? \
	"PE_DNT_M14B_VFY_HD":\
	((_x)==PE_DNT_M14B_VFY_PC)? \
	"PE_DNT_M14B_VFY_PC":\
	((_x)==PE_DNT_M14B_VFY_SD)? \
	"PE_DNT_M14B_VFY_SD":\
	((_x)==PE_DNT_M14B_VFY_SD_DTV)? \
	"PE_DNT_M14B_VFY_SD_DTV":\
	((_x)==PE_DNT_M14B_VFY_SECAM)? \
	"PE_DNT_M14B_VFY_SECAM":"PE_DNT_M14B_VFY_NUM")

#define PE_DNT_HW_M14A_IPC_FMT_TO_STR(_x)		(\
	((_x)==PE_DNT_M14A_IPC_ATV_480)? \
	"PE_DNT_M14A_IPC_ATV_480":\
	((_x)==PE_DNT_M14A_IPC_ATV_576)? \
	"PE_DNT_M14A_IPC_ATV_576":\
	((_x)==PE_DNT_M14A_IPC_AV_480)? \
	"PE_DNT_M14A_IPC_AV_480":\
	((_x)==PE_DNT_M14A_IPC_AV_576)? \
	"PE_DNT_M14A_IPC_AV_576":\
	((_x)==PE_DNT_M14A_IPC_SCARTRGB)? \
	"PE_DNT_M14A_IPC_SCARTRGB":\
	((_x)==PE_DNT_M14A_IPC_DTV_480)? \
	"PE_DNT_M14A_IPC_DTV_480":\
	((_x)==PE_DNT_M14A_IPC_DTV_576)? \
	"PE_DNT_M14A_IPC_DTV_576":\
	((_x)==PE_DNT_M14A_IPC_DTV_HD)? \
	"PE_DNT_M14A_IPC_DTV_HD":\
	((_x)==PE_DNT_M14A_IPC_SD_480)? \
	"PE_DNT_M14A_IPC_SD_480":\
	((_x)==PE_DNT_M14A_IPC_SD_576)? \
	"PE_DNT_M14A_IPC_SD_576":\
	((_x)==PE_DNT_M14A_IPC_HD)? \
	"PE_DNT_M14A_IPC_HD":\
	((_x)==PE_DNT_M14A_IPC_PC)? \
	"PE_DNT_M14A_IPC_PC":"PE_DNT_M14A_IPC_NUM")

#define PE_DNT_HW_M14A_CLC_FMT_TO_STR(_x)		(\
	((_x)==PE_DNT_M14A_CLC_AV)? \
	"PE_DNT_M14A_CLC_AV":\
	((_x)==PE_DNT_M14A_CLC_SD)? \
	"PE_DNT_M14A_CLC_SD":\
	((_x)==PE_DNT_M14A_CLC_HD)? \
	"PE_DNT_M14A_CLC_HD":\
	((_x)==PE_DNT_M14A_CLC_PC)? \
	"PE_DNT_M14A_CLC_PC":"PE_DNT_M14A_CLC_NUM")

#define PE_DNT_HW_M14A_VFY_FMT_TO_STR(_x)		(\
	((_x)==PE_DNT_M14A_VFY_SD)? \
	"PE_DNT_M14A_VFY_SD":\
	((_x)==PE_DNT_M14A_VFY_HD_I)? \
	"PE_DNT_M14A_VFY_HD_I":\
	((_x)==PE_DNT_M14A_VFY_HD_P)? \
	"PE_DNT_M14A_VFY_HD_P":\
	((_x)==PE_DNT_M14A_VFY_PC)? \
	"PE_DNT_M14A_VFY_PC":"PE_DNT_M14A_VFY_NUM")

/**
 *	pe read default reg menu enumeration(for M14BX)
 */
typedef enum
{
	PE_DNT_HW_M14B_RD_DFLT_DISPLAY = 0,		///< menu display
	PE_DNT_HW_M14B_RD_DFLT_IPC_M,				///< ipc_m
	PE_DNT_HW_M14B_RD_DFLT_IPC_S,				///< ipc_s
	PE_DNT_HW_M14B_RD_DFLT_CLC,				///< clc
	PE_DNT_HW_M14B_RD_DFLT_VFY,				///< vfilter y
	PE_DNT_HW_M14B_RD_IPC_MNUM,				///< ipc matching number
	PE_DNT_HW_M14B_RD_CLC_MNUM,				///< clc matching number
	PE_DNT_HW_M14B_RD_P0M_DATA,				///< p0m reg ctrl
	PE_DNT_HW_M14B_RD_P0M_MASK,				///< p0m reg mask
	PE_DNT_HW_M14B_RD_VFY_MNUM,				///< vfy matching number
	PE_DNT_HW_M14B_RD_P0D_DATA,				///< p0d reg ctrl
	PE_DNT_HW_M14B_RD_P0D_MASK,				///< p0d reg mask
	PE_DNT_HW_M14B_RD_DFLT_MAX				///< max num
}
PE_DNT_HW_M14B_RD_DFLT_MENU;

/**
 *	pe read default reg menu enumeration(for M14AX)
 */
typedef enum
{
	PE_DNT_HW_M14A_RD_DFLT_DISPLAY = 0,		///< menu display
	PE_DNT_HW_M14A_RD_DFLT_IPC_L,			///< ipc L
	PE_DNT_HW_M14A_RD_DFLT_IPC_R,			///< ipc R
	PE_DNT_HW_M14A_RD_DFLT_CLC_L,			///< clc L
	PE_DNT_HW_M14A_RD_DFLT_VFY_L,			///< vfilter y L
	PE_DNT_HW_M14A_RD_DFLT_VFY_R,			///< vfilter y R
	PE_DNT_HW_M14A_RD_DFLT_MAX				///< max num
}
PE_DNT_HW_M14A_RD_DFLT_MENU;

/**
 *	pe dnt ipc fmt(for M14BX)
 */
typedef enum
{
	PE_DNT_M14B_IPC_ATV_480 = 0,	///< atv 480
	PE_DNT_M14B_IPC_ATV_576,		///< atv 576
	PE_DNT_M14B_IPC_AV_480,			///< av 480
	PE_DNT_M14B_IPC_AV_576,			///< av 576
	PE_DNT_M14B_IPC_DTV_480,		///< dtv 480
	PE_DNT_M14B_IPC_DTV_576,		///< dtv 576
	PE_DNT_M14B_IPC_DTV_HD,			///< dtv hd
	PE_DNT_M14B_IPC_DTV_HD_TP,		///< dtv hd tp
	PE_DNT_M14B_IPC_HD,				///< hd
	PE_DNT_M14B_IPC_PC,				///< pc
	PE_DNT_M14B_IPC_SD_480,			///< sd 480
	PE_DNT_M14B_IPC_SD_576,			///< sd 576
	PE_DNT_M14B_IPC_SECAM,			///< secam
	PE_DNT_M14B_IPC_NUM				///< max num
}
PE_DNT_HW_M14B_IPC_FMT;
/**
 *	pe dnt clc fmt(for M14BX)
 */
typedef enum
{
	PE_DNT_M14B_CLC_SD_50 = 0,		///< sd 50hz
	PE_DNT_M14B_CLC_SD_60,			///< sd 60hz
	PE_DNT_M14B_CLC_HD,				///< hd
	PE_DNT_M14B_CLC_PC,				///< pc(clc off)
	PE_DNT_M14B_CLC_NUM				///< max num
}
PE_DNT_HW_M14B_CLC_FMT;
/**
 *	pe dnt v filter y fmt(for M14BX)
 */
typedef enum
{
	PE_DNT_M14B_VFY_ATV = 0,		///< atv
	PE_DNT_M14B_VFY_AV,				///< av
	PE_DNT_M14B_VFY_HD,				///< hd
	PE_DNT_M14B_VFY_PC,				///< pc
	PE_DNT_M14B_VFY_SD,				///< sd
	PE_DNT_M14B_VFY_SD_DTV,			///< sd dtv
	PE_DNT_M14B_VFY_SECAM,			///< secam
	PE_DNT_M14B_VFY_NUM				///< max num
}
PE_DNT_HW_M14B_VFY_FMT;

/**
 *	pe dnt ipc fmt(for M14AX)
 */
typedef enum
{
	PE_DNT_M14A_IPC_ATV_480 = 0,	///< atv 480
	PE_DNT_M14A_IPC_ATV_576,		///< atv 576
	PE_DNT_M14A_IPC_AV_480,			///< av(cvd) 480
	PE_DNT_M14A_IPC_AV_576,			///< av(cvd) 576
	PE_DNT_M14A_IPC_SCARTRGB,		///< scart rgb
	PE_DNT_M14A_IPC_DTV_480,		///< dtv 480
	PE_DNT_M14A_IPC_DTV_576,		///< dtv 576
	PE_DNT_M14A_IPC_DTV_HD,			///< dtv hd
	PE_DNT_M14A_IPC_SD_480,			///< sd 480
	PE_DNT_M14A_IPC_SD_576,			///< sd 576
	PE_DNT_M14A_IPC_HD,				///< hd
	PE_DNT_M14A_IPC_PC,				///< pc(vflt hmc protect enable off)
	PE_DNT_M14A_IPC_NUM				///< max num
}
PE_DNT_HW_M14A_IPC_FMT;

/**
 *	pe dnt clc fmt(for M14AX)
 */
typedef enum
{
	PE_DNT_M14A_CLC_AV = 0,		///< av(cvd)
	PE_DNT_M14A_CLC_SD,			///< sd
	PE_DNT_M14A_CLC_HD,			///< hd
	PE_DNT_M14A_CLC_PC,			///< pc(clc off)
	PE_DNT_M14A_CLC_NUM			///< max num
}
PE_DNT_HW_M14A_CLC_FMT;

/**
 *	pe dnt v filter y fmt(for M14AX)
 */
typedef enum
{
	PE_DNT_M14A_VFY_SD = 0,			///< sd
	PE_DNT_M14A_VFY_HD_I,			///< hd i
	PE_DNT_M14A_VFY_HD_P,			///< hd p
	PE_DNT_M14A_VFY_PC,				///< pc
	PE_DNT_M14A_VFY_NUM				///< max num
}
PE_DNT_HW_M14A_VFY_FMT;

/**
 *	pe dnt module settings type
 */
typedef struct
{
	UINT32 pre_p0l_ipc_ctrl_01;			///< previous p0l_ipc_ctrl_01 for m14a
	UINT32 pre_p0r_ipc_ctrl_01;			///< previous p0r_ipc_ctrl_01 for m14a
	UINT32 cur_p0l_clc_ctrl_00;			///< current p0l_clc_ctrl_00 for m14a
	UINT32 pre_p0l_clc_ctrl_00;			///< previous p0l_clc_ctrl_00 for m14a
	UINT32 pre_p0l_tnr_ctrl_10;			///< previous p0l_tnr_ctrl_10
	UINT32 pre_p0r_tnr_ctrl_10;			///< previous p0r_tnr_ctrl_10
	UINT32 cur_p0m_ipc_ctrl_00;			///< current p0m_ipc_ctrl_00 for m14b
	UINT32 pre_p0m_ipc_ctrl_00;			///< previous p0m_ipc_ctrl_00 for m14b
	UINT32 cur_p0m_ipc_ctrl_01;			///< current p0m_ipc_ctrl_01 for m14b
	UINT32 pre_p0m_ipc_ctrl_01;			///< previous p0m_ipc_ctrl_01 for m14b
	UINT32 cur_p0m_ipc_ctrl_06;			///< current p0m_ipc_ctrl_06 for m14b
	UINT32 pre_p0m_ipc_ctrl_06;			///< previous p0m_ipc_ctrl_06 for m14b
	UINT32 cur_p0m_clc_ctrl_00;			///< current p0m_clc_ctrl_00 for m14b
	UINT32 pre_p0m_clc_ctrl_00;			///< previous p0m_clc_ctrl_00 for m14b
	UINT32 cur_p0d_vfy_ctrl_00;			///< current p0d_vfy_ctrl_00 for m14b
	UINT32 cur_p0d_vfy_ctrl_02;			///< current p0d_vfy_ctrl_00 for m14b
	PE_DNT_HW_M14A_IPC_FMT ipc0a_fmt;	///< previous p0l ipc0a_fmt for m14a
	PE_DNT_HW_M14A_IPC_FMT ipc1a_fmt;	///< previous p0r ipc1a_fmt for m14a
	PE_DNT_HW_M14A_CLC_FMT clc0a_fmt;	///< previous p0l clc0a_fmt for m14a
	PE_DNT_HW_M14A_VFY_FMT vfy0a_fmt;	///< previous p0l vfy0a_fmt for m14a
	PE_DNT_HW_M14A_VFY_FMT vfy1a_fmt;	///< previous p0r vfy1a_fmt for m14a
	PE_DNT_HW_M14B_IPC_FMT ipc0b_fmt;	///< previous p0m ipc0b_fmt for m14b
	PE_DNT_HW_M14B_IPC_FMT ipc1b_fmt;	///< previous p0m ipc1b_fmt for m14b
	PE_DNT_HW_M14B_CLC_FMT clc0b_fmt;	///< previous p0m clc0b_fmt for m14b
	PE_DNT_HW_M14B_VFY_FMT vfy0b_fmt;	///< previous p0d vfy0b_fmt for m14b
	UINT32 force_cbcr_enb;				///< force_cbcr_enable
	UINT32 force_y_max_th;				///< force_y_max_th
}
PE_DNT_HW_M14_SETTINGS_T;

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/
int PE_DNT_HW_M14_Init(PE_CFG_CTRL_T *pstParams);
int PE_DNT_HW_M14_SetDebugSettings(LX_PE_DBG_SETTINGS_T *pstParams);
int PE_DNT_HW_M14_SetDefault(LX_PE_DEFAULT_SETTINGS_T *pstParams);
int PE_DNT_HW_M14_RdDefault(LX_PE_ETC_TBL_T *pstParams);
int PE_DNT_HW_M14_SetFilmMode(LX_PE_DNT_FILMMODE_T *pstParams);
int PE_DNT_HW_M14_GetFilmMode(LX_PE_DNT_FILMMODE_T *pstParams);
int PE_DNT_HW_M14_SetLowDelayMode(LX_PE_DNT_LD_MODE_T *pstParams);
int PE_DNT_HW_M14_GetLowDelayMode(LX_PE_DNT_LD_MODE_T *pstParams);
int PE_DNT_HW_M14B_SetClcFmdApply(PE_TSK_M14_CFG_T *pCfg);
int PE_DNT_HW_M14B_SetVfyApply(PE_TSK_M14_CFG_T *pCfg);
int PE_DNT_HW_M14A_SetClcApply(PE_TSK_M14_CFG_T *pCfg);
int PE_DNT_HW_M14_GetCurDntSettings(PE_DNT_HW_M14_SETTINGS_T *pstParams);

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _PE_DNT_HW_M14_H_ */
