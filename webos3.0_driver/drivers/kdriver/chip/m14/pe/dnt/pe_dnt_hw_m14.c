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

/** @file pe_dnt_hw_m14.c
 *
 *  driver for picture enhance deinterlacer functions. ( used only within kdriver )
 *	
 *	@author		Seung-Jun,Youm(sj.youm@lge.com)
 *	@version	0.1
 *	@note		
 *	@date		2012.04.12
 *	@see		
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/semaphore.h>
#include <linux/interrupt.h>
#include <asm/io.h>

#include "base_types.h"
#include "os_util.h"
#include "debug_util.h"

#include "pe_hw_m14.h"
#include "pe_reg_m14.h"
#include "pe_fwi_m14.h"
#include "pe_dnt_hw_m14.h"
#include "pe_dnt_hw_m14a0.h"
#include "pe_dnt_hw_m14b0.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
/* usd define until film mode f/w ready, M14B0 */
#define PE_DNT_HW_M14B_SET_FMD_PARAM_TO_REG_DIRECTLY

/* 20140218, sj.youm, use clc_xxxx_en_stat on fsw reg.(pe1)  */
#define PE_HW_M14B_USE_FSW_CLC_EN_STAT

/* 20140618, sj.youm, apply vfilter y on dtv 1080p  */
#define PE_DNT_HW_M14B_APPLY_VFLT_Y_ON_DTV_1080P

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define PE_DNT_HW_M14_ERROR		PE_PRINT_ERROR

#define PE_DNT_HW_M14_DBG_PRINT(fmt, args...)	\
	if(_g_dnt_hw_m14_trace)\
	{\
		printk("[%x,%x][%s,%d] "fmt, \
			PE_CHIP_VER, g_pe_kdrv_ver_mask, __F__, __L__, ##args);\
	}

#define PE_DNT_HW_M14_CHECK_CODE(_checker, _action, fmt, args...)	\
	{if(_checker){PE_DNT_HW_M14_ERROR(fmt, ##args);_action;}}

#define PE_DNT_HW_M14B_DBG_PRINT_IPC_FMT_STATE(_pre, _cur)	\
	{\
		char _str_buf[PE_TRACE_STR_SIZE];\
		sprintf(_str_buf, "%s(%s)->(%s):%s", #_cur, \
			PE_DNT_HW_M14B_IPC_FMT_TO_STR(_pre), \
			PE_DNT_HW_M14B_IPC_FMT_TO_STR(_cur), \
			((_pre)!=(_cur))? "different":"same");\
		PE_DNT_HW_M14_DBG_PRINT("%s\n", _str_buf);\
		if((_pre)!=(_cur))\
		{\
			PE_INF_M14_SetTraceDataBuf(_str_buf);\
		}\
	}
#define PE_DNT_HW_M14B_DBG_PRINT_CLC_FMT_STATE(_pre, _cur)	\
	{\
		char _str_buf[PE_TRACE_STR_SIZE];\
		sprintf(_str_buf, "%s(%s)->(%s):%s", #_cur, \
			PE_DNT_HW_M14B_CLC_FMT_TO_STR(_pre), \
			PE_DNT_HW_M14B_CLC_FMT_TO_STR(_cur), \
			((_pre)!=(_cur))? "different":"same");\
		PE_DNT_HW_M14_DBG_PRINT("%s\n", _str_buf);\
		if((_pre)!=(_cur))\
		{\
			PE_INF_M14_SetTraceDataBuf(_str_buf);\
		}\
	}
#define PE_DNT_HW_M14B_DBG_PRINT_VFY_FMT_STATE(_pre, _cur)	\
	{\
		char _str_buf[PE_TRACE_STR_SIZE];\
		sprintf(_str_buf, "%s(%s)->(%s):%s", #_cur, \
			PE_DNT_HW_M14B_VFY_FMT_TO_STR(_pre), \
			PE_DNT_HW_M14B_VFY_FMT_TO_STR(_cur), \
			((_pre)!=(_cur))? "different":"same");\
		PE_DNT_HW_M14_DBG_PRINT("%s\n", _str_buf);\
		if((_pre)!=(_cur))\
		{\
			PE_INF_M14_SetTraceDataBuf(_str_buf);\
		}\
	}

#define PE_DNT_HW_M14A_DBG_PRINT_IPC_FMT_STATE(_pre, _cur)	\
	{\
		char _str_buf[PE_TRACE_STR_SIZE];\
		sprintf(_str_buf, "%s(%s)->(%s):%s", #_cur, \
			PE_DNT_HW_M14A_IPC_FMT_TO_STR(_pre), \
			PE_DNT_HW_M14A_IPC_FMT_TO_STR(_cur), \
			((_pre)!=(_cur))? "different":"same");\
		PE_DNT_HW_M14_DBG_PRINT("%s\n", _str_buf);\
		if((_pre)!=(_cur))\
		{\
			PE_INF_M14_SetTraceDataBuf(_str_buf);\
		}\
	}
#define PE_DNT_HW_M14A_DBG_PRINT_CLC_FMT_STATE(_pre, _cur)	\
	{\
		char _str_buf[PE_TRACE_STR_SIZE];\
		sprintf(_str_buf, "%s(%s)->(%s):%s", #_cur, \
			PE_DNT_HW_M14A_CLC_FMT_TO_STR(_pre), \
			PE_DNT_HW_M14A_CLC_FMT_TO_STR(_cur), \
			((_pre)!=(_cur))? "different":"same");\
		PE_DNT_HW_M14_DBG_PRINT("%s\n", _str_buf);\
		if((_pre)!=(_cur))\
		{\
			PE_INF_M14_SetTraceDataBuf(_str_buf);\
		}\
	}
#define PE_DNT_HW_M14A_DBG_PRINT_VFY_FMT_STATE(_pre,  _cur)	\
	{\
		char _str_buf[PE_TRACE_STR_SIZE];\
		sprintf(_str_buf, "%s(%s)->(%s):%s", #_cur, \
			PE_DNT_HW_M14A_VFY_FMT_TO_STR(_pre),  \
			PE_DNT_HW_M14A_VFY_FMT_TO_STR(_cur),  \
			((_pre)!=(_cur))? "different":"same");\
		PE_DNT_HW_M14_DBG_PRINT("%s\n",  _str_buf);\
		if((_pre)!=(_cur))\
		{\
			PE_INF_M14_SetTraceDataBuf(_str_buf);\
		}\
	}

#ifdef PE_HW_M14B_USE_REG_CTRL
#else
#define PE_DNT_HW_M14B_SET_CLC_REG_TABLE(_mdl_, _tbl, _ofs)	\
	do{\
		PE_DNT_HW_M14_DBG_PRINT("write %s.\n", #_tbl);\
		ret = PE_DNT_HW_M14B_SetClcDfltTbl((_tbl), (_ofs), sizeof(_tbl));\
		PE_DNT_HW_M14_CHECK_CODE(ret, break, \
			"[%s, %d] PE_DNT_HW_M14B_SetClcDfltTbl() error.\n", __F__, __L__);\
	}while(0)
#endif

#define PE_DNT_HW_M14A_SET_CLC_REG_TABLE(_mdl_, _tbl, _ofs)	\
	do{\
		PE_DNT_HW_M14_DBG_PRINT("write %s.\n", #_tbl);\
		ret = PE_DNT_HW_M14A_SetClcDfltTbl((_tbl), (_ofs), sizeof(_tbl));\
		PE_DNT_HW_M14_CHECK_CODE(ret, break, \
			"[%s, %d] PE_DNT_HW_M14A_SetClcDfltTbl() error.\n", __F__, __L__);\
	}while(0)

#define PE_DNT_HW_M14A_SET_IPC_REG_TABLE(_tbl, _ofs)	\
	do{\
		PE_DNT_HW_M14_DBG_PRINT("write %s.\n", #_tbl);\
		ret = PE_DNT_HW_M14A_SetIpcDfltTbl((_tbl), (_ofs), sizeof(_tbl));\
		PE_DNT_HW_M14_CHECK_CODE(ret, break, \
			"[%s, %d] PE_DNT_HW_M14A_SetIpcDfltTbl() error.\n", __F__, __L__);\
	}while(0)

#ifdef PE_HW_M14B_USE_REG_CTRL
/* p0m */
#define PE_DNT_HW_M14B_P0M_ENUM(_x)		PE_DNT_HW_M14B_P0M_ENUM_##_x

#define PE_DNT_HW_M14B_P0M_SET(_r, _f, _d)	\
	{	_g_pe_dnt_hw_m14b_p0m_reg_ctrl.data._r._f = (_d);\
		_g_pe_dnt_hw_m14b_p0m_reg_ctrl.mask._r._f = 0;}

#define PE_DNT_HW_M14B_P0M_WR01(_r, _f1, _d1)	\
	{	PE_P0M_M14B0_QWr01(_r, _f1, _d1);\
		PE_DNT_HW_M14B_P0M_SET(_r, _f1, _d1);}
#define PE_DNT_HW_M14B_P0M_WR02(_r, _f1, _d1, _f2, _d2)	\
	{	PE_P0M_M14B0_QWr02(_r, _f1, _d1, _f2, _d2);\
		PE_DNT_HW_M14B_P0M_SET(_r, _f1, _d1);\
		PE_DNT_HW_M14B_P0M_SET(_r, _f2, _d2);}
#define PE_DNT_HW_M14B_P0M_WR03(_r, _f1, _d1, _f2, _d2, _f3, _d3)	\
	{	PE_P0M_M14B0_QWr03(_r, _f1, _d1, _f2, _d2, _f3, _d3);\
		PE_DNT_HW_M14B_P0M_SET(_r, _f1, _d1);\
		PE_DNT_HW_M14B_P0M_SET(_r, _f2, _d2);\
		PE_DNT_HW_M14B_P0M_SET(_r, _f3, _d3);}

/* p0d */
#define PE_DNT_HW_M14B_P0D_ENUM(_x)		PE_DNT_HW_M14B_P0D_ENUM_##_x

#define PE_DNT_HW_M14B_P0D_SET(_r, _f, _d)	\
	{	_g_pe_dnt_hw_m14b_p0d_reg_ctrl.data._r._f = (_d);\
		_g_pe_dnt_hw_m14b_p0d_reg_ctrl.mask._r._f = 0;}

#define PE_DNT_HW_M14B_P0D_WR01(_r, _f1, _d1)	\
	{	PE_P0D_M14B0_QWr01(_r, _f1, _d1);\
		PE_DNT_HW_M14B_P0D_SET(_r, _f1, _d1);}
#define PE_DNT_HW_M14B_P0D_WR02(_r, _f1, _d1, _f2, _d2)	\
	{	PE_P0D_M14B0_QWr02(_r, _f1, _d1, _f2, _d2);\
		PE_DNT_HW_M14B_P0D_SET(_r, _f1, _d1);\
		PE_DNT_HW_M14B_P0D_SET(_r, _f2, _d2);}

/* ipc */
#define PE_DNT_HW_M14B_IPC_REG_TABLE(_dflt, _base)	\
	do{\
		PE_REG_PARAM_T *p_##_dflt = (PE_REG_PARAM_T *)OS_Malloc(sizeof(_dflt));\
		PE_DNT_HW_M14_CHECK_CODE(!(p_##_dflt), break, \
			"[%s,%d] p_tbl is null.\n", __F__, __L__);\
		do{\
			ret = PE_DNT_HW_M14B_GetIpcDfltTable(p_##_dflt, _dflt, sizeof(_dflt));\
			PE_DNT_HW_M14_CHECK_CODE(ret, break, \
				"[%s,%d] PE_DNT_HW_M14B_GetIpcDfltTable() error.\n", __F__, __L__);\
			PE_DNT_HW_M14_DBG_PRINT("write %s(+ 0x%08x).\n", #_dflt, (_base));\
			ret = PE_REG_SetTraceTable(__F__,__L__, #_dflt, (_base));\
			ret = PE_REG_SetTable(p_##_dflt, (_base), sizeof(_dflt));\
			PE_DNT_HW_M14_CHECK_CODE(ret, break, \
				"[%s,%d] PE_REG_SetTable() error.\n", __F__, __L__);\
		}while (0);\
		if (p_##_dflt)	{OS_Free(p_##_dflt);}\
	}while (0)

/* clc */
#define PE_DNT_HW_M14B_CLC_REG_TABLE(_dflt, _base)	\
	do{\
		PE_REG_PARAM_T *p_##_dflt = (PE_REG_PARAM_T *)OS_Malloc(sizeof(_dflt));\
		PE_DNT_HW_M14_CHECK_CODE(!(p_##_dflt), break, \
			"[%s,%d] p_tbl is null.\n", __F__, __L__);\
		do{\
			ret = PE_DNT_HW_M14B_GetClcDfltTable(p_##_dflt, _dflt, sizeof(_dflt));\
			PE_DNT_HW_M14_CHECK_CODE(ret, break, \
				"[%s,%d] PE_DNT_HW_M14B_GetClcDfltTable() error.\n", __F__, __L__);\
			PE_DNT_HW_M14_DBG_PRINT("write %s(+ 0x%08x).\n", #_dflt, (_base));\
			ret = PE_REG_SetTraceTable(__F__,__L__, #_dflt, (_base));\
			ret = PE_REG_SetTable(p_##_dflt, (_base), sizeof(_dflt));\
			PE_DNT_HW_M14_CHECK_CODE(ret, break, \
				"[%s,%d] PE_REG_SetTable() error.\n", __F__, __L__);\
		}while (0);\
		if (p_##_dflt)	{OS_Free(p_##_dflt);}\
	}while (0)

/* vfy */
#define PE_DNT_HW_M14B_VFY_REG_TABLE(_dflt, _base)	\
	do{\
		PE_REG_PARAM_T *p_##_dflt = (PE_REG_PARAM_T *)OS_Malloc(sizeof(_dflt));\
		PE_DNT_HW_M14_CHECK_CODE(!(p_##_dflt), break, \
			"[%s,%d] p_tbl is null.\n", __F__, __L__);\
		do{\
			ret = PE_DNT_HW_M14B_GetVfyDfltTable(p_##_dflt, _dflt, sizeof(_dflt));\
			PE_DNT_HW_M14_CHECK_CODE(ret, break, \
				"[%s,%d] PE_DNT_HW_M14B_GetVfyDfltTable() error.\n", __F__, __L__);\
			PE_DNT_HW_M14_DBG_PRINT("write %s(+ 0x%08x).\n", #_dflt, (_base));\
			ret = PE_REG_SetTraceTable(__F__,__L__, #_dflt, (_base));\
			ret = PE_REG_SetTable(p_##_dflt, (_base), sizeof(_dflt));\
			PE_DNT_HW_M14_CHECK_CODE(ret, break, \
				"[%s,%d] PE_REG_SetTable() error.\n", __F__, __L__);\
		}while (0);\
		if (p_##_dflt)	{OS_Free(p_##_dflt);}\
	}while (0)
#endif

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/
#ifdef PE_HW_M14B_USE_REG_CTRL
/**
 *	pe register num
 */
typedef enum
{
	PE_DNT_HW_M14B_P0M_ENUM(PE_M14B0_P0M_IPC_CTRL_00_T) = 0,	///< [0]  0xc0022200L
	PE_DNT_HW_M14B_P0M_ENUM(PE_M14B0_P0M_IPC_CTRL_01_T),		///< [1]  0xc0022204L
	PE_DNT_HW_M14B_P0M_ENUM(PE_M14B0_P0M_IPC_CTRL_06_T),        ///< [2]  0xc0022218L
	PE_DNT_HW_M14B_P0M_ENUM(PE_M14B0_P0M_CLC_CTRL_00_T),		///< [3]  0xc0022260L
	PE_DNT_HW_M14B_P0M_ENUM(MAX)								///< max num
}
PE_DNT_HW_M14B_P0M_REG_NUM;

/**
 *	pe register control type
 */
typedef struct
{
	PE_M14B0_P0M_IPC_CTRL_00_T         	p0m_ipc_ctrl_00;              ///< [0]  0xc0022200L
	PE_M14B0_P0M_IPC_CTRL_01_T         	p0m_ipc_ctrl_01;              ///< [1]  0xc0022204L
	PE_M14B0_P0M_IPC_CTRL_06_T         	p0m_ipc_ctrl_06;              ///< [2]  0xc0022218L
	PE_M14B0_P0M_CLC_CTRL_00_T         	p0m_clc_ctrl_00;              ///< [3]  0xc0022260L
}
PE_DNT_HW_M14B_P0M_REG_T;

/**
 *	pe register control type
 */
typedef struct
{
	UINT32 addr[PE_DNT_HW_M14B_P0M_ENUM(MAX)];	///< ctrl addr
	PE_DNT_HW_M14B_P0M_REG_T data;				///< ctrl data
	PE_DNT_HW_M14B_P0M_REG_T mask;				///< ctrl mask
}
PE_DNT_HW_M14B_P0M_REG_CTRL_T;

/**
 *	pe register match type
 */
typedef struct
{
	UINT32 ipc_size;				///< ipc size
	PE_REG_PARAM_T *ipc_mtbl;		///< ipc match table
	UINT32 clc_size;				///< clc size
	PE_REG_PARAM_T *clc_mtbl;		///< clc match table
}
PE_DNT_HW_M14B_P0M_REG_MATCH_T;

/**
 *	pe register num
 */
typedef enum
{
	PE_DNT_HW_M14B_P0D_ENUM(PE_M14B0_P0D_VFILTER_CTRL_0_T) = 0, ///< [0]  0xc0023a80L
	PE_DNT_HW_M14B_P0D_ENUM(PE_M14B0_P0D_VFILTER_CTRL_2_T),     ///< [1]  0xc0023a88L
	PE_DNT_HW_M14B_P0D_ENUM(MAX)                                ///< max num
}
PE_DNT_HW_M14B_P0D_REG_NUM;

/**
 *	pe register control type
 */
typedef struct
{
	PE_M14B0_P0D_VFILTER_CTRL_0_T    	p0d_vfilter_ctrl_0;     ///< [0]  0xc0023a80L
	PE_M14B0_P0D_VFILTER_CTRL_2_T    	p0d_vfilter_ctrl_2;     ///< [1]  0xc0023a88L
}
PE_DNT_HW_M14B_P0D_REG_T;

/**
 *	pe register control type
 */
typedef struct
{
	UINT32 addr[PE_DNT_HW_M14B_P0D_ENUM(MAX)];	///< ctrl addr
	PE_DNT_HW_M14B_P0D_REG_T data;				///< ctrl data
	PE_DNT_HW_M14B_P0D_REG_T mask;				///< ctrl mask
}
PE_DNT_HW_M14B_P0D_REG_CTRL_T;

/**
 *	pe register match type
 */
typedef struct
{
	UINT32 vfy_size;				///< vfy size
	PE_REG_PARAM_T *vfy_mtbl;		///< vfy match table
}
PE_DNT_HW_M14B_P0D_REG_MATCH_T;
#endif

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Functions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/
#ifdef PE_HW_M14B_USE_REG_CTRL
static int PE_DNT_HW_M14B_GetIpcDfltTable(PE_REG_PARAM_T *dst_dflt, const PE_REG_PARAM_T *src_dflt, UINT32 dflt_size);
static int PE_DNT_HW_M14B_GetClcDfltTable(PE_REG_PARAM_T *dst_dflt, const PE_REG_PARAM_T *src_dflt, UINT32 dflt_size);
static int PE_DNT_HW_M14B_GetVfyDfltTable(PE_REG_PARAM_T *dst_dflt, const PE_REG_PARAM_T *src_dflt, UINT32 dflt_size);
#endif
static int PE_DNT_HW_M14B_SetDefault(LX_PE_DEFAULT_SETTINGS_T *pstParams);
static int PE_DNT_HW_M14A_SetDefault(LX_PE_DEFAULT_SETTINGS_T *pstParams);
static PE_DNT_HW_M14B_IPC_FMT PE_DNT_HW_M14B_ConvDispInfoToIpcFmt(LX_PE_INF_DISPLAY_T *disp_inf);
static PE_DNT_HW_M14B_CLC_FMT PE_DNT_HW_M14B_ConvDispInfoToClcFmt(LX_PE_INF_DISPLAY_T *disp_inf);
static PE_DNT_HW_M14B_VFY_FMT PE_DNT_HW_M14B_ConvDispInfoToVfyFmt(LX_PE_INF_DISPLAY_T *disp_inf);
static PE_DNT_HW_M14A_IPC_FMT PE_DNT_HW_M14A_ConvDispInfoToIpcFmt(LX_PE_INF_DISPLAY_T *disp_inf);
static PE_DNT_HW_M14A_CLC_FMT PE_DNT_HW_M14A_ConvDispInfoToClcFmt(LX_PE_INF_DISPLAY_T *disp_inf);
static PE_DNT_HW_M14A_VFY_FMT PE_DNT_HW_M14A_ConvDispInfoToVfyFmt(LX_PE_INF_DISPLAY_T *disp_inf);
#ifdef PE_HW_M14B_USE_REG_CTRL
#else
static int PE_DNT_HW_M14B_SetClcDfltTbl(const PE_REG_PARAM_T *table, UINT32 offset, UINT32 size);
#endif
static int PE_DNT_HW_M14A_SetIpcDfltTbl(const PE_REG_PARAM_T *table, UINT32 offset, UINT32 size);
static int PE_DNT_HW_M14A_SetClcDfltTbl(const PE_REG_PARAM_T *table, UINT32 offset, UINT32 size);

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
static UINT32 _g_dnt_hw_m14_trace=0x0;		//default should be off.
static PE_DNT_HW_M14_SETTINGS_T _g_pe_dnt_hw_m14_info;
#ifdef PE_HW_M14B_USE_REG_CTRL
/* p0m */
static PE_DNT_HW_M14B_P0M_REG_CTRL_T _g_pe_dnt_hw_m14b_p0m_reg_ctrl;
static PE_DNT_HW_M14B_P0M_REG_MATCH_T _g_pe_dnt_hw_m14b_p0m_reg_match = {0, NULL, 0, NULL};
const UINT32 g_pe_dnt_hw_m14b_p0m_reg_addr[PE_DNT_HW_M14B_P0M_ENUM(MAX)] = {
	0x2200,           ///< [0]  0xc0022200L
	0x2204,           ///< [1]  0xc0022204L
	0x2218,           ///< [2]  0xc0022218L
	0x2260            ///< [3]  0xc0022260L
};
/* p0d */
static PE_DNT_HW_M14B_P0D_REG_CTRL_T _g_pe_dnt_hw_m14b_p0d_reg_ctrl;
static PE_DNT_HW_M14B_P0D_REG_MATCH_T _g_pe_dnt_hw_m14b_p0d_reg_match = {0, NULL};
const UINT32 g_pe_dnt_hw_m14b_p0d_reg_addr[PE_DNT_HW_M14B_P0D_ENUM(MAX)] = {
	0x3a80,           ///< [0]  0xc0023a80L
	0x3a88            ///< [1]  0xc0023a88L
};
#endif

/*========================================================================================
	Implementation Group
========================================================================================*/
/**
 * init dnt
 *
 * @param   *pstParams [in] PE_CFG_CTRL_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_DNT_HW_M14_Init(PE_CFG_CTRL_T *pstParams)
{
	int ret = RET_OK;
	#ifdef PE_HW_M14B_USE_REG_CTRL
	UINT32 i = 0, j = 0, tbl_size = 0, tbl_num = 0;
	PE_DNT_HW_M14B_P0M_REG_CTRL_T *p0m_reg_ctrl = &_g_pe_dnt_hw_m14b_p0m_reg_ctrl;
	PE_DNT_HW_M14B_P0M_REG_MATCH_T *p0m_reg_match = &_g_pe_dnt_hw_m14b_p0m_reg_match;
	PE_DNT_HW_M14B_P0D_REG_CTRL_T *p0d_reg_ctrl = &_g_pe_dnt_hw_m14b_p0d_reg_ctrl;
	PE_DNT_HW_M14B_P0D_REG_MATCH_T *p0d_reg_match = &_g_pe_dnt_hw_m14b_p0d_reg_match;
	#endif
	do{
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_M14)
		{
			_g_pe_dnt_hw_m14_info.pre_p0l_ipc_ctrl_01 = 0x0;
			_g_pe_dnt_hw_m14_info.pre_p0r_ipc_ctrl_01 = 0x0;
			_g_pe_dnt_hw_m14_info.cur_p0l_clc_ctrl_00 = 0x0;
			_g_pe_dnt_hw_m14_info.pre_p0l_clc_ctrl_00 = 0x0;
			_g_pe_dnt_hw_m14_info.pre_p0l_tnr_ctrl_10 = 0x0;
			_g_pe_dnt_hw_m14_info.pre_p0r_tnr_ctrl_10 = 0x0;
			_g_pe_dnt_hw_m14_info.cur_p0m_ipc_ctrl_00 = 0x0;
			_g_pe_dnt_hw_m14_info.pre_p0m_ipc_ctrl_00 = 0x0;
			_g_pe_dnt_hw_m14_info.cur_p0m_ipc_ctrl_01 = 0x0;
			_g_pe_dnt_hw_m14_info.pre_p0m_ipc_ctrl_01 = 0x0;
			_g_pe_dnt_hw_m14_info.cur_p0m_ipc_ctrl_06 = 0x0;
			_g_pe_dnt_hw_m14_info.pre_p0m_ipc_ctrl_06 = 0x0;
			_g_pe_dnt_hw_m14_info.cur_p0m_clc_ctrl_00 = 0x0;
			_g_pe_dnt_hw_m14_info.pre_p0m_clc_ctrl_00 = 0x0;
			_g_pe_dnt_hw_m14_info.cur_p0d_vfy_ctrl_00 = 0x0;
			_g_pe_dnt_hw_m14_info.cur_p0d_vfy_ctrl_02 = 0x0;
			_g_pe_dnt_hw_m14_info.ipc0a_fmt = PE_DNT_M14A_IPC_NUM;
			_g_pe_dnt_hw_m14_info.ipc1a_fmt = PE_DNT_M14A_IPC_NUM;
			_g_pe_dnt_hw_m14_info.clc0a_fmt = PE_DNT_M14A_CLC_NUM;
			_g_pe_dnt_hw_m14_info.vfy0a_fmt = PE_DNT_M14A_VFY_NUM;
			_g_pe_dnt_hw_m14_info.vfy1a_fmt = PE_DNT_M14A_VFY_NUM;
			_g_pe_dnt_hw_m14_info.ipc0b_fmt = PE_DNT_M14B_IPC_NUM;
			_g_pe_dnt_hw_m14_info.ipc1b_fmt = PE_DNT_M14B_IPC_NUM;
			_g_pe_dnt_hw_m14_info.clc0b_fmt = PE_DNT_M14B_CLC_NUM;
			_g_pe_dnt_hw_m14_info.vfy0b_fmt = PE_DNT_M14B_VFY_NUM;
			_g_pe_dnt_hw_m14_info.force_cbcr_enb = 0x0;
			_g_pe_dnt_hw_m14_info.force_y_max_th = 0x5f;
			if(PE_KDRV_VER_M14BX)
			{
				PE_DNT_HW_M14_DBG_PRINT("init ipc, tpd.\n");
				/* init ipc */
				PE_SET_REG_TABLE(DNT_HW_M14,ipc_l_init_m14b0,PE_REG_M14B_WRITE_BASE);
				/* init clc */
				PE_SET_REG_TABLE(DNT_HW_M14,clc_l_init_m14b0,PE_REG_M14B_WRITE_BASE);
				/* init vflt y */
				PE_SET_REG_TABLE(DNT_HW_M14,vflt_y_l_init_m14b0,PE_REG_M14B_WRITE_BASE);
				/* init tpd */
				PE_P0M_M14B0_RdFL(p0m_tpd_ctrl_07);
				PE_P0M_M14B0_RdFL(p0m_tpd_ctrl_08);
				PE_P0M_M14B0_RdFL(p0m_tpd_ctrl_09);
				PE_P0M_M14B0_Wr01(p0m_tpd_ctrl_07, reg_force_y,			0x0);
				PE_P0M_M14B0_Wr01(p0m_tpd_ctrl_07, reg_force_y_max_th,	_g_pe_dnt_hw_m14_info.force_y_max_th);
				PE_P0M_M14B0_Wr01(p0m_tpd_ctrl_07, reg_force_y_min_th,	0x0);
				PE_P0M_M14B0_Wr01(p0m_tpd_ctrl_07, reg_force_y_enable,	0x0);
				PE_P0M_M14B0_Wr01(p0m_tpd_ctrl_08, reg_force_cb,		0x200);
				PE_P0M_M14B0_Wr01(p0m_tpd_ctrl_08, reg_force_cb_max_th,	0x205);
				PE_P0M_M14B0_Wr01(p0m_tpd_ctrl_08, reg_force_cb_min_th,	0x1e0);
				PE_P0M_M14B0_Wr01(p0m_tpd_ctrl_08, reg_force_cb_enable,	_g_pe_dnt_hw_m14_info.force_cbcr_enb);
				PE_P0M_M14B0_Wr01(p0m_tpd_ctrl_09, reg_force_cr,		0x200);
				PE_P0M_M14B0_Wr01(p0m_tpd_ctrl_09, reg_force_cr_max_th,	0x205);
				PE_P0M_M14B0_Wr01(p0m_tpd_ctrl_09, reg_force_cr_min_th,	0x1e0);
				PE_P0M_M14B0_Wr01(p0m_tpd_ctrl_09, reg_force_cr_enable,	_g_pe_dnt_hw_m14_info.force_cbcr_enb);
				PE_P0M_M14B0_WrFL(p0m_tpd_ctrl_07);
				PE_P0M_M14B0_WrFL(p0m_tpd_ctrl_08);
				PE_P0M_M14B0_WrFL(p0m_tpd_ctrl_09);
				/* 20140607, sj.youm, turn off c_tava because of capture quality(1080i, color delay) */
				PE_P0S_M14B0_QWr01(p0s_ipc_ctrl_00,cr_chroma_tava_only,0x0);
				#ifdef PE_HW_M14B_USE_REG_CTRL
				/* p0m init reg ctrl data, mask and addr */
				memset(&(p0m_reg_ctrl->data), 0, sizeof(PE_DNT_HW_M14B_P0M_REG_T));	// 0x0
				memset(&(p0m_reg_ctrl->mask), -1, sizeof(PE_DNT_HW_M14B_P0M_REG_T));// 0xffffffff
				for (i=0; i<PE_DNT_HW_M14B_P0M_ENUM(MAX); i++)
				{
					p0m_reg_ctrl->addr[i] = g_pe_dnt_hw_m14b_p0m_reg_addr[i];
				}
				/* p0d init reg ctrl data, mask and addr */
				memset(&(p0d_reg_ctrl->data), 0, sizeof(PE_DNT_HW_M14B_P0D_REG_T));	// 0x0
				memset(&(p0d_reg_ctrl->mask), -1, sizeof(PE_DNT_HW_M14B_P0D_REG_T));// 0xffffffff
				for (i=0; i<PE_DNT_HW_M14B_P0D_ENUM(MAX); i++)
				{
					p0d_reg_ctrl->addr[i] = g_pe_dnt_hw_m14b_p0d_reg_addr[i];
				}
				/* ipc matching table */
				if (p0m_reg_match->ipc_size==0 && p0m_reg_match->ipc_mtbl==NULL)
				{
					tbl_size = sizeof(ipc_l_pc_default_m14b0);
					tbl_num = tbl_size/sizeof(PE_REG_PARAM_T);
					/* alloc matching table */
					p0m_reg_match->ipc_mtbl = (PE_REG_PARAM_T *)OS_Malloc(tbl_size);
					PE_DNT_HW_M14_CHECK_CODE(!(p0m_reg_match->ipc_mtbl), break, \
						"[%s,%d] p0m_reg_match->ipc_mtbl is null.\n", __F__, __L__);
					p0m_reg_match->ipc_size = tbl_size;

					for (i=0; i<tbl_num; i++)
					{
						/* set addr and data of matching table */
						p0m_reg_match->ipc_mtbl[i].addr = ipc_l_pc_default_m14b0[i].addr;
						p0m_reg_match->ipc_mtbl[i].data = PE_DNT_HW_M14B_P0M_ENUM(MAX);
						/* check matching number */
						for (j=0; j<PE_DNT_HW_M14B_P0M_ENUM(MAX); j++)
						{
							if (p0m_reg_match->ipc_mtbl[i].addr == p0m_reg_ctrl->addr[j])
							{
								p0m_reg_match->ipc_mtbl[i].data = j;
								break;
							}
						}
					}
					PE_PRINT_NOTI("ipc matching table created.\n");
				}
				/* clc matching table */
				if (p0m_reg_match->clc_size==0 && p0m_reg_match->clc_mtbl==NULL)
				{
					tbl_size = sizeof(clc_l_pc_default_m14b0);
					tbl_num = tbl_size/sizeof(PE_REG_PARAM_T);
					/* alloc matching table */
					p0m_reg_match->clc_mtbl = (PE_REG_PARAM_T *)OS_Malloc(tbl_size);
					PE_DNT_HW_M14_CHECK_CODE(!(p0m_reg_match->clc_mtbl), break, \
						"[%s,%d] p0m_reg_match->clc_mtbl is null.\n", __F__, __L__);
					p0m_reg_match->clc_size = tbl_size;

					for (i=0; i<tbl_num; i++)
					{
						/* set addr and data of matching table */
						p0m_reg_match->clc_mtbl[i].addr = clc_l_pc_default_m14b0[i].addr;
						p0m_reg_match->clc_mtbl[i].data = PE_DNT_HW_M14B_P0M_ENUM(MAX);
						/* check matching number */
						for (j=0; j<PE_DNT_HW_M14B_P0M_ENUM(MAX); j++)
						{
							if (p0m_reg_match->clc_mtbl[i].addr == p0m_reg_ctrl->addr[j])
							{
								p0m_reg_match->clc_mtbl[i].data = j;
								break;
							}
						}
					}
					PE_PRINT_NOTI("clc matching table created.\n");
				}
				/* vfy matching table */
				if (p0d_reg_match->vfy_size==0 && p0d_reg_match->vfy_mtbl==NULL)
				{
					tbl_size = sizeof(vflt_y_l_pc_default_m14b0);
					tbl_num = tbl_size/sizeof(PE_REG_PARAM_T);
					/* alloc matching table */
					p0d_reg_match->vfy_mtbl = (PE_REG_PARAM_T *)OS_Malloc(tbl_size);
					PE_DNT_HW_M14_CHECK_CODE(!(p0d_reg_match->vfy_mtbl), break, \
						"[%s,%d] p0d_reg_match->vfy_mtbl is null.\n", __F__, __L__);
					p0d_reg_match->vfy_size = tbl_size;

					for (i=0; i<tbl_num; i++)
					{
						/* set addr and data of matching table */
						p0d_reg_match->vfy_mtbl[i].addr = vflt_y_l_pc_default_m14b0[i].addr;
						p0d_reg_match->vfy_mtbl[i].data = PE_DNT_HW_M14B_P0D_ENUM(MAX);
						/* check matching number */
						for (j=0; j<PE_DNT_HW_M14B_P0D_ENUM(MAX); j++)
						{
							if (p0d_reg_match->vfy_mtbl[i].addr == p0d_reg_ctrl->addr[j])
							{
								p0d_reg_match->vfy_mtbl[i].data = j;
								break;
							}
						}
					}
					PE_PRINT_NOTI("vfy matching table created.\n");
				}
				#endif
			}
			else if(PE_KDRV_VER_M14AX)
			{
				PE_DNT_HW_M14_DBG_PRINT("init ipc, tpd.\n");
				/* init ipc */
				PE_SET_REG_TABLE(DNT_HW_M14,ipc_l_init_m14a0,PE_REG_M14_WRITE_BASE);
				PE_SET_REG_TABLE(DNT_HW_M14,ipc_l_init_m14a0,PE_PE0_REG_M14_LRDIFF);
				/* init clc */
				PE_SET_REG_TABLE(DNT_HW_M14,clc_l_init_m14a0,PE_REG_M14_WRITE_BASE);
				/* init vflt y */
				PE_SET_REG_TABLE(DNT_HW_M14,vflt_y_l_init_m14a0,PE_REG_M14_WRITE_BASE);
				PE_SET_REG_TABLE(DNT_HW_M14,vflt_y_l_init_m14a0,PE_PE0_REG_M14_LRDIFF);
				/* init tpd */
				PE_P0L_M14A0_RdFL(p0l_tpd_ctrl_05);
				PE_P0L_M14A0_RdFL(p0l_tpd_ctrl_06);
				PE_P0L_M14A0_RdFL(p0l_tpd_ctrl_07);
				PE_P0L_M14A0_Wr01(p0l_tpd_ctrl_05, reg_force_y,					0x0);
				PE_P0L_M14A0_Wr01(p0l_tpd_ctrl_05, reg_force_y_max_th,			0x5f);
				PE_P0L_M14A0_Wr01(p0l_tpd_ctrl_05, reg_force_y_min_th,			0x0);
				PE_P0L_M14A0_Wr01(p0l_tpd_ctrl_05, reg_force_y_enable,			0x0);
				PE_P0L_M14A0_Wr01(p0l_tpd_ctrl_06, reg_force_cb,				0x200);
				PE_P0L_M14A0_Wr01(p0l_tpd_ctrl_06, reg_force_cb_max_th,			0x205);
				PE_P0L_M14A0_Wr01(p0l_tpd_ctrl_06, reg_force_cb_min_th,			0x1e0);
				PE_P0L_M14A0_Wr01(p0l_tpd_ctrl_06, reg_force_cb_enable,			0x0);
				PE_P0L_M14A0_Wr01(p0l_tpd_ctrl_07, reg_force_cr,				0x200);
				PE_P0L_M14A0_Wr01(p0l_tpd_ctrl_07, reg_force_cr_max_th,			0x205);
				PE_P0L_M14A0_Wr01(p0l_tpd_ctrl_07, reg_force_cr_min_th,			0x1e0);
				PE_P0L_M14A0_Wr01(p0l_tpd_ctrl_07, reg_force_cr_enable,			0x0);
				PE_P0L_M14A0_WrFL(p0l_tpd_ctrl_05);
				PE_P0L_M14A0_WrFL(p0l_tpd_ctrl_06);
				PE_P0L_M14A0_WrFL(p0l_tpd_ctrl_07);
			}
			else
			{
				PE_DNT_HW_M14_DBG_PRINT("nothing to do\n");
			}
		}
		else
		{
			PE_DNT_HW_M14_DBG_PRINT("nothing to do\n");	ret = RET_OK;
		}
	}while(0);	
	return ret;
}
/**
 * debug setting
 *
 * @param   *pstParams [in]LX_PE_DBG_SETTINGS_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_DNT_HW_M14_SetDebugSettings(LX_PE_DBG_SETTINGS_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		PE_DNT_HW_M14_DBG_PRINT("wid:%d,type:0x%x,[0x%x]print:0x%x,bypass:0x%x\n",\
			pstParams->win_id,pstParams->type,(0x1<<PE_ITEM_PKTMODL(DNT)),\
			pstParams->print_lvl,pstParams->bypass);
		/* set print level */
		if(pstParams->type&LX_PE_DBG_LV)
		{
			_g_dnt_hw_m14_trace = (pstParams->print_lvl & (0x1<<PE_ITEM_PKTMODL(DNT)))? 0x1:0x0;
		}
	}while(0);
	return ret;
}
/**
 * set dnt default
 *
 * @param   *pstParams [in] LX_PE_DEFAULT_SETTINGS_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_DNT_HW_M14_SetDefault(LX_PE_DEFAULT_SETTINGS_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_M14BX)
		{
			ret = PE_DNT_HW_M14B_SetDefault(pstParams);
			PE_DNT_HW_M14_CHECK_CODE(ret, break, \
				"[%s,%d] PE_DNT_HW_M14B_SetDefault() error.\n", __F__, __L__);
		}
		else if(PE_KDRV_VER_M14AX)
		{
			ret = PE_DNT_HW_M14A_SetDefault(pstParams);
			PE_DNT_HW_M14_CHECK_CODE(ret, break, \
				"[%s,%d] PE_DNT_HW_M14A_SetDefault() error.\n", __F__, __L__);
		}
		else
		{
			PE_DNT_HW_M14_DBG_PRINT("nothing to do\n");	ret = RET_OK;
		}
	}while(0);	
	return ret;
}
#ifdef PE_HW_M14B_USE_REG_CTRL
/**
 * get new default table from original one(for M14BX)
 *
 * @param   *dst_dflt [in/out] PE_REG_PARAM_T
 * @param   *src_dflt [in] const PE_REG_PARAM_T
 * @param   dflt_size [in] UINT32
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_DNT_HW_M14B_GetIpcDfltTable(PE_REG_PARAM_T *dst_dflt, const PE_REG_PARAM_T *src_dflt, UINT32 dflt_size)
{
	int ret = RET_OK;
	UINT32 dflt_num = 0, match_num = 0, i = 0, r_data = 0;
	UINT32 match_size = _g_pe_dnt_hw_m14b_p0m_reg_match.ipc_size;
	PE_REG_PARAM_T *p_match_tbl = _g_pe_dnt_hw_m14b_p0m_reg_match.ipc_mtbl;
	UINT32 *p_reg_data = (UINT32 *)&(_g_pe_dnt_hw_m14b_p0m_reg_ctrl.data);
	UINT32 *p_reg_mask = (UINT32 *)&(_g_pe_dnt_hw_m14b_p0m_reg_ctrl.mask);
	PE_DNT_HW_M14_SETTINGS_T *pInfo = &_g_pe_dnt_hw_m14_info;

	PE_DNT_HW_M14_CHECK_CODE(!dst_dflt, return RET_ERROR, \
		"[%s,%d] dst_dflt is null, not ready.\n", __F__, __L__);
	PE_DNT_HW_M14_CHECK_CODE(!src_dflt, return RET_ERROR, \
		"[%s,%d] src_dflt is null, not ready.\n", __F__, __L__);
	PE_DNT_HW_M14_CHECK_CODE(!match_size, return RET_ERROR, \
		"[%s,%d] match size is zero.\n", __F__, __L__);
	PE_DNT_HW_M14_CHECK_CODE(!p_match_tbl, return RET_ERROR, \
		"[%s,%d] match tbl is null, not ready.\n", __F__, __L__);
	PE_DNT_HW_M14_CHECK_CODE(match_size!=dflt_size, return RET_ERROR, \
		"[%s,%d] size mismatch(%d vs. %d).\n", __F__, __L__, match_size, dflt_size);

	dflt_num = dflt_size/sizeof(PE_REG_PARAM_T);
	for (i=0; i<(dflt_num); i++)
	{
		dst_dflt[i].addr = src_dflt[i].addr;
		match_num = p_match_tbl[i].data;
		if (match_num<PE_DNT_HW_M14B_P0M_ENUM(MAX))
		{
			/* store p0m_ipc_ctrl_00,01,06 default for setting clc,fmd apply on task */
			if (src_dflt[i].addr==0x2200)
			{
				pInfo->cur_p0m_ipc_ctrl_00 = src_dflt[i].data;
			}
			else if (src_dflt[i].addr==0x2204)
			{
				PE_P0M_M14B0_RdFL(p0m_ipc_ctrl_01);
				PE_P0M_M14B0_Rd01(p0m_ipc_ctrl_01,cr_game_mode,r_data);
				PE_DNT_HW_M14B_P0M_SET(p0m_ipc_ctrl_01,cr_game_mode,r_data);
				pInfo->cur_p0m_ipc_ctrl_01 = src_dflt[i].data;
			}
			else if (src_dflt[i].addr==0x2218)
			{
				PE_P0M_M14B0_RdFL(p0m_ipc_ctrl_06);
				PE_P0M_M14B0_Rd01(p0m_ipc_ctrl_06,use_tnr_motion,r_data);
				PE_DNT_HW_M14B_P0M_SET(p0m_ipc_ctrl_06,use_tnr_motion,r_data);
				pInfo->cur_p0m_ipc_ctrl_06 = src_dflt[i].data;
			}
			dst_dflt[i].data = (src_dflt[i].data & p_reg_mask[match_num])|(p_reg_data[match_num]);
		}
		else
		{
			dst_dflt[i].data = src_dflt[i].data;
		}
	}
	return ret;
}
/**
 * get new default table from original one(for M14BX)
 *
 * @param   *dst_dflt [in/out] PE_REG_PARAM_T
 * @param   *src_dflt [in] const PE_REG_PARAM_T
 * @param   dflt_size [in] UINT32
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_DNT_HW_M14B_GetClcDfltTable(PE_REG_PARAM_T *dst_dflt, const PE_REG_PARAM_T *src_dflt, UINT32 dflt_size)
{
	int ret = RET_OK;
	UINT32 dflt_num = 0, match_num = 0, i = 0;
	UINT32 match_size = _g_pe_dnt_hw_m14b_p0m_reg_match.clc_size;
	PE_REG_PARAM_T *p_match_tbl = _g_pe_dnt_hw_m14b_p0m_reg_match.clc_mtbl;
	UINT32 *p_reg_data = (UINT32 *)&(_g_pe_dnt_hw_m14b_p0m_reg_ctrl.data);
	UINT32 *p_reg_mask = (UINT32 *)&(_g_pe_dnt_hw_m14b_p0m_reg_ctrl.mask);
	PE_DNT_HW_M14_SETTINGS_T *pInfo = &_g_pe_dnt_hw_m14_info;

	PE_DNT_HW_M14_CHECK_CODE(!dst_dflt, return RET_ERROR, \
		"[%s,%d] dst_dflt is null, not ready.\n", __F__, __L__);
	PE_DNT_HW_M14_CHECK_CODE(!src_dflt, return RET_ERROR, \
		"[%s,%d] src_dflt is null, not ready.\n", __F__, __L__);
	PE_DNT_HW_M14_CHECK_CODE(!match_size, return RET_ERROR, \
		"[%s,%d] match size is zero.\n", __F__, __L__);
	PE_DNT_HW_M14_CHECK_CODE(!p_match_tbl, return RET_ERROR, \
		"[%s,%d] match tbl is null, not ready.\n", __F__, __L__);
	PE_DNT_HW_M14_CHECK_CODE(match_size!=dflt_size, return RET_ERROR, \
		"[%s,%d] size mismatch(%d vs. %d).\n", __F__, __L__, match_size, dflt_size);

	dflt_num = dflt_size/sizeof(PE_REG_PARAM_T);
	for (i=0; i<(dflt_num); i++)
	{
		dst_dflt[i].addr = src_dflt[i].addr;
		match_num = p_match_tbl[i].data;
		if (match_num<PE_DNT_HW_M14B_P0M_ENUM(MAX))
		{
			/* store p0m_clc_ctrl_00 default for setting clc,fmd apply on task */
			if (src_dflt[i].addr==0x2260)
			{
				pInfo->cur_p0m_clc_ctrl_00 = src_dflt[i].data;
			}
			dst_dflt[i].data = (src_dflt[i].data & p_reg_mask[match_num])|(p_reg_data[match_num]);
		}
		else
		{
			dst_dflt[i].data = src_dflt[i].data;
		}
	}
	return ret;
}
/**
 * get new default table from original one(for M14BX)
 *
 * @param   *dst_dflt [in/out] PE_REG_PARAM_T
 * @param   *src_dflt [in] const PE_REG_PARAM_T
 * @param   dflt_size [in] UINT32
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_DNT_HW_M14B_GetVfyDfltTable(PE_REG_PARAM_T *dst_dflt, const PE_REG_PARAM_T *src_dflt, UINT32 dflt_size)
{
	int ret = RET_OK;
	UINT32 dflt_num = 0, match_num = 0, i = 0;
	UINT32 match_size = _g_pe_dnt_hw_m14b_p0d_reg_match.vfy_size;
	PE_REG_PARAM_T *p_match_tbl = _g_pe_dnt_hw_m14b_p0d_reg_match.vfy_mtbl;
	UINT32 *p_reg_data = (UINT32 *)&(_g_pe_dnt_hw_m14b_p0d_reg_ctrl.data);
	UINT32 *p_reg_mask = (UINT32 *)&(_g_pe_dnt_hw_m14b_p0d_reg_ctrl.mask);
	PE_DNT_HW_M14_SETTINGS_T *pInfo = &_g_pe_dnt_hw_m14_info;

	PE_DNT_HW_M14_CHECK_CODE(!dst_dflt, return RET_ERROR, \
		"[%s,%d] dst_dflt is null, not ready.\n", __F__, __L__);
	PE_DNT_HW_M14_CHECK_CODE(!src_dflt, return RET_ERROR, \
		"[%s,%d] src_dflt is null, not ready.\n", __F__, __L__);
	PE_DNT_HW_M14_CHECK_CODE(!match_size, return RET_ERROR, \
		"[%s,%d] match size is zero.\n", __F__, __L__);
	PE_DNT_HW_M14_CHECK_CODE(!p_match_tbl, return RET_ERROR, \
		"[%s,%d] match tbl is null, not ready.\n", __F__, __L__);
	PE_DNT_HW_M14_CHECK_CODE(match_size!=dflt_size, return RET_ERROR, \
		"[%s,%d] size mismatch(%d vs. %d).\n", __F__, __L__, match_size, dflt_size);

	dflt_num = dflt_size/sizeof(PE_REG_PARAM_T);
	for (i=0; i<(dflt_num); i++)
	{
		dst_dflt[i].addr = src_dflt[i].addr;
		match_num = p_match_tbl[i].data;
		if (match_num<PE_DNT_HW_M14B_P0D_ENUM(MAX))
		{
			/* store p0d_vfilter_ctrl_0,2 default for setting vfy apply on task */
			if (src_dflt[i].addr==0x3a80)
			{
				pInfo->cur_p0d_vfy_ctrl_00 = src_dflt[i].data;
			}
			else if (src_dflt[i].addr==0x3a88)
			{
				pInfo->cur_p0d_vfy_ctrl_02 = src_dflt[i].data;
			}
			dst_dflt[i].data = (src_dflt[i].data & p_reg_mask[match_num])|(p_reg_data[match_num]);
		}
		else
		{
			dst_dflt[i].data = src_dflt[i].data;
		}
	}
	return ret;
}
#endif
/**
 * set dnt default(for M14BX)
 *
 * @param   *pstParams [in] LX_PE_DEFAULT_SETTINGS_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_DNT_HW_M14B_SetDefault(LX_PE_DEFAULT_SETTINGS_T *pstParams)
{
	int ret = RET_OK;
	PE_INF_M14_SETTINGS_T inf_set;
	LX_PE_INF_DISPLAY_T *cur0_inf = NULL;
	LX_PE_INF_DISPLAY_T *cur1_inf = NULL;
	PE_DNT_HW_M14B_IPC_FMT cur0_ipc_fmt, cur1_ipc_fmt;
	PE_DNT_HW_M14B_CLC_FMT cur0_clc_fmt;
	PE_DNT_HW_M14B_VFY_FMT cur0_vfy_fmt;
	UINT32 cur0_force_cbcr_enb = 0, cur0_force_y_max_th = 0;
	PE_DNT_HW_M14_SETTINGS_T *pInfo = &_g_pe_dnt_hw_m14_info;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		ret = PE_INF_M14_GetCurInfSettings(&inf_set);
		PE_DNT_HW_M14_CHECK_CODE(ret, break, \
			"[%s,%d] PE_INF_M14_GetCurInfSettings() error.\n", __F__, __L__);
		cur0_inf=&inf_set.disp_info[LX_PE_WIN_0];
		cur1_inf=&inf_set.disp_info[LX_PE_WIN_1];
		CHECK_KNULL(cur0_inf);
		CHECK_KNULL(cur1_inf);
		/* pe0m tpd */
		if(PE_CHECK_WIN0(pstParams->win_id))
		{
			if(cur0_inf->src_type==LX_PE_SRC_DTV && \
				cur0_inf->fmt_type==LX_PE_FMT_HD && \
				cur0_inf->scan_type==LX_PE_SCAN_INTERLACE)
			{
				cur0_force_cbcr_enb = 0x1;
				cur0_force_y_max_th = 0x5f;
			}
			else if(cur0_inf->src_type==LX_PE_SRC_ATV || \
				cur0_inf->src_type==LX_PE_SRC_CVBS || \
				cur0_inf->src_type==LX_PE_SRC_SCART || \
				cur0_inf->src_type==LX_PE_SRC_COMP)
			{
				cur0_force_cbcr_enb = 0x1;
				cur0_force_y_max_th = 0x5f;
			}
			else
			{
				cur0_force_cbcr_enb = 0x0;
				cur0_force_y_max_th = 0x5f;
			}
			if ((pInfo->force_cbcr_enb != cur0_force_cbcr_enb) || \
				(pInfo->force_y_max_th != cur0_force_y_max_th))
			{
				PE_P0M_M14B0_QWr01(p0m_tpd_ctrl_08, reg_force_cb_enable, cur0_force_cbcr_enb);
				PE_P0M_M14B0_QWr01(p0m_tpd_ctrl_09, reg_force_cr_enable, cur0_force_cbcr_enb);
				PE_P0M_M14B0_QWr01(p0m_tpd_ctrl_07, reg_force_y_max_th, cur0_force_y_max_th);
				/* for debug */
				do {
					char _str_buf[PE_TRACE_STR_SIZE];
					sprintf(_str_buf, "[%s,%d]force_en,th:%d,%d -> %d,%d(sr,fmt,sc:%d,%d,%d)", \
						__F__, __L__, pInfo->force_cbcr_enb, pInfo->force_y_max_th, \
						cur0_force_cbcr_enb, cur0_force_y_max_th, cur0_inf->src_type, \
						cur0_inf->fmt_type, cur0_inf->scan_type);
					ret = PE_INF_M14_SetTraceDataBuf(_str_buf);
					PE_DNT_HW_M14_CHECK_CODE(ret, break, \
						"PE_INF_M14_SetTraceDataBuf() error.\n");
				} while(0);
				pInfo->force_cbcr_enb = cur0_force_cbcr_enb;
				pInfo->force_y_max_th = cur0_force_y_max_th;
			}
		}
		/* pe0m ipc */
		if(PE_CHECK_WIN0(pstParams->win_id))
		{
			cur0_ipc_fmt = PE_DNT_HW_M14B_ConvDispInfoToIpcFmt(cur0_inf);
			PE_DNT_HW_M14B_DBG_PRINT_IPC_FMT_STATE(pInfo->ipc0b_fmt,cur0_ipc_fmt);
			if(pInfo->ipc0b_fmt!=cur0_ipc_fmt)
			{
				#ifdef PE_HW_M14B_USE_REG_CTRL
				if(PE_KDRV_VER_M14CX)
				{
					switch(cur0_ipc_fmt)
					{
						case PE_DNT_M14B_IPC_ATV_480:
							PE_DNT_HW_M14B_IPC_REG_TABLE(ipc_l_atv_480_default_m14c0,PE_REG_M14B_WRITE_BASE);
							break;
						case PE_DNT_M14B_IPC_ATV_576:
							PE_DNT_HW_M14B_IPC_REG_TABLE(ipc_l_atv_576_default_m14c0,PE_REG_M14B_WRITE_BASE);
							break;
						case PE_DNT_M14B_IPC_AV_480:
							PE_DNT_HW_M14B_IPC_REG_TABLE(ipc_l_av_480_default_m14c0,PE_REG_M14B_WRITE_BASE);
							break;
						case PE_DNT_M14B_IPC_AV_576:
							PE_DNT_HW_M14B_IPC_REG_TABLE(ipc_l_av_576_default_m14c0,PE_REG_M14B_WRITE_BASE);
							break;
						case PE_DNT_M14B_IPC_DTV_480:
							PE_DNT_HW_M14B_IPC_REG_TABLE(ipc_l_dtv_480_default_m14c0,PE_REG_M14B_WRITE_BASE);
							break;
						case PE_DNT_M14B_IPC_DTV_576:
							PE_DNT_HW_M14B_IPC_REG_TABLE(ipc_l_dtv_576_default_m14c0,PE_REG_M14B_WRITE_BASE);
							break;
						case PE_DNT_M14B_IPC_DTV_HD:
							PE_DNT_HW_M14B_IPC_REG_TABLE(ipc_l_dtv_hd_default_m14c0,PE_REG_M14B_WRITE_BASE);
							break;
						case PE_DNT_M14B_IPC_DTV_HD_TP:
							PE_DNT_HW_M14B_IPC_REG_TABLE(ipc_l_dtv_hd_tp_default_m14c0,PE_REG_M14B_WRITE_BASE);
							/* for pe0 ~ dvo pic update on dtv tp */
							ret = PE_FWI_M14_SetPushMode(LX_PE_WIN_0);
							PE_DNT_HW_M14_CHECK_CODE(ret, break, \
								"[%s,%d] PE_FWI_M14_SetPushMode() error.\n",__F__,__L__);
							break;
						case PE_DNT_M14B_IPC_HD:
							PE_DNT_HW_M14B_IPC_REG_TABLE(ipc_l_hd_default_m14c0,PE_REG_M14B_WRITE_BASE);
							break;
						case PE_DNT_M14B_IPC_PC:
							PE_DNT_HW_M14B_IPC_REG_TABLE(ipc_l_pc_default_m14c0,PE_REG_M14B_WRITE_BASE);
							break;
						case PE_DNT_M14B_IPC_SD_480:
							PE_DNT_HW_M14B_IPC_REG_TABLE(ipc_l_sd_480_default_m14c0,PE_REG_M14B_WRITE_BASE);
							break;
						case PE_DNT_M14B_IPC_SD_576:
							PE_DNT_HW_M14B_IPC_REG_TABLE(ipc_l_sd_576_default_m14c0,PE_REG_M14B_WRITE_BASE);
							break;
						case PE_DNT_M14B_IPC_SECAM:
							PE_DNT_HW_M14B_IPC_REG_TABLE(ipc_l_secam_default_m14c0,PE_REG_M14B_WRITE_BASE);
							break;
						default:
							break;
					}
				}
				else
				{
					switch(cur0_ipc_fmt)
					{
						case PE_DNT_M14B_IPC_ATV_480:
							PE_DNT_HW_M14B_IPC_REG_TABLE(ipc_l_atv_480_default_m14b0,PE_REG_M14B_WRITE_BASE);
							break;
						case PE_DNT_M14B_IPC_ATV_576:
							PE_DNT_HW_M14B_IPC_REG_TABLE(ipc_l_atv_576_default_m14b0,PE_REG_M14B_WRITE_BASE);
							break;
						case PE_DNT_M14B_IPC_AV_480:
							PE_DNT_HW_M14B_IPC_REG_TABLE(ipc_l_av_480_default_m14b0,PE_REG_M14B_WRITE_BASE);
							break;
						case PE_DNT_M14B_IPC_AV_576:
							PE_DNT_HW_M14B_IPC_REG_TABLE(ipc_l_av_576_default_m14b0,PE_REG_M14B_WRITE_BASE);
							break;
						case PE_DNT_M14B_IPC_DTV_480:
							PE_DNT_HW_M14B_IPC_REG_TABLE(ipc_l_dtv_480_default_m14b0,PE_REG_M14B_WRITE_BASE);
							break;
						case PE_DNT_M14B_IPC_DTV_576:
							PE_DNT_HW_M14B_IPC_REG_TABLE(ipc_l_dtv_576_default_m14b0,PE_REG_M14B_WRITE_BASE);
							break;
						case PE_DNT_M14B_IPC_DTV_HD:
							PE_DNT_HW_M14B_IPC_REG_TABLE(ipc_l_dtv_hd_default_m14b0,PE_REG_M14B_WRITE_BASE);
							break;
						case PE_DNT_M14B_IPC_DTV_HD_TP:
							PE_DNT_HW_M14B_IPC_REG_TABLE(ipc_l_dtv_hd_tp_default_m14b0,PE_REG_M14B_WRITE_BASE);
							/* for pe0 ~ dvo pic update on dtv tp */
							ret = PE_FWI_M14_SetPushMode(LX_PE_WIN_0);
							PE_DNT_HW_M14_CHECK_CODE(ret, break, \
								"[%s,%d] PE_FWI_M14_SetPushMode() error.\n",__F__,__L__);
							break;
						case PE_DNT_M14B_IPC_HD:
							PE_DNT_HW_M14B_IPC_REG_TABLE(ipc_l_hd_default_m14b0,PE_REG_M14B_WRITE_BASE);
							break;
						case PE_DNT_M14B_IPC_PC:
							PE_DNT_HW_M14B_IPC_REG_TABLE(ipc_l_pc_default_m14b0,PE_REG_M14B_WRITE_BASE);
							break;
						case PE_DNT_M14B_IPC_SD_480:
							PE_DNT_HW_M14B_IPC_REG_TABLE(ipc_l_sd_480_default_m14b0,PE_REG_M14B_WRITE_BASE);
							break;
						case PE_DNT_M14B_IPC_SD_576:
							PE_DNT_HW_M14B_IPC_REG_TABLE(ipc_l_sd_576_default_m14b0,PE_REG_M14B_WRITE_BASE);
							break;
						case PE_DNT_M14B_IPC_SECAM:
							PE_DNT_HW_M14B_IPC_REG_TABLE(ipc_l_secam_default_m14b0,PE_REG_M14B_WRITE_BASE);
							break;
						default:
							break;
					}
				}
				#else
				switch(cur0_ipc_fmt)
				{
					case PE_DNT_M14B_IPC_ATV_480:
						PE_SET_REG_TABLE(DNT_HW_M14,ipc_l_atv_480_default_m14b0,PE_REG_M14B_WRITE_BASE);
						break;
					case PE_DNT_M14B_IPC_ATV_576:
						PE_SET_REG_TABLE(DNT_HW_M14,ipc_l_atv_576_default_m14b0,PE_REG_M14B_WRITE_BASE);
						break;
					case PE_DNT_M14B_IPC_AV_480:
						PE_SET_REG_TABLE(DNT_HW_M14,ipc_l_av_480_default_m14b0,PE_REG_M14B_WRITE_BASE);
						break;
					case PE_DNT_M14B_IPC_AV_576:
						PE_SET_REG_TABLE(DNT_HW_M14,ipc_l_av_576_default_m14b0,PE_REG_M14B_WRITE_BASE);
						break;
					case PE_DNT_M14B_IPC_DTV_480:
						PE_SET_REG_TABLE(DNT_HW_M14,ipc_l_dtv_480_default_m14b0,PE_REG_M14B_WRITE_BASE);
						break;
					case PE_DNT_M14B_IPC_DTV_576:
						PE_SET_REG_TABLE(DNT_HW_M14,ipc_l_dtv_576_default_m14b0,PE_REG_M14B_WRITE_BASE);
						break;
					case PE_DNT_M14B_IPC_DTV_HD:
						PE_SET_REG_TABLE(DNT_HW_M14,ipc_l_dtv_hd_default_m14b0,PE_REG_M14B_WRITE_BASE);
						break;
					case PE_DNT_M14B_IPC_DTV_HD_TP:
						PE_SET_REG_TABLE(DNT_HW_M14,ipc_l_dtv_hd_tp_default_m14b0,PE_REG_M14B_WRITE_BASE);
						break;
					case PE_DNT_M14B_IPC_HD:
						PE_SET_REG_TABLE(DNT_HW_M14,ipc_l_hd_default_m14b0,PE_REG_M14B_WRITE_BASE);
						break;
					case PE_DNT_M14B_IPC_PC:
						PE_SET_REG_TABLE(DNT_HW_M14,ipc_l_pc_default_m14b0,PE_REG_M14B_WRITE_BASE);
						break;
					case PE_DNT_M14B_IPC_SD_480:
						PE_SET_REG_TABLE(DNT_HW_M14,ipc_l_sd_480_default_m14b0,PE_REG_M14B_WRITE_BASE);
						break;
					case PE_DNT_M14B_IPC_SD_576:
						PE_SET_REG_TABLE(DNT_HW_M14,ipc_l_sd_576_default_m14b0,PE_REG_M14B_WRITE_BASE);
						break;
					case PE_DNT_M14B_IPC_SECAM:
						PE_SET_REG_TABLE(DNT_HW_M14,ipc_l_secam_default_m14b0,PE_REG_M14B_WRITE_BASE);
						break;
					default:
						break;
				}
				#endif
				pInfo->ipc0b_fmt=cur0_ipc_fmt;
			}
		}
		/* pe0s ipc */
		if(PE_CHECK_WIN1(pstParams->win_id))
		{
			cur1_ipc_fmt = PE_DNT_HW_M14B_ConvDispInfoToIpcFmt(cur1_inf);
			PE_DNT_HW_M14B_DBG_PRINT_IPC_FMT_STATE(pInfo->ipc1b_fmt,cur1_ipc_fmt);
			if(pInfo->ipc1b_fmt!=cur1_ipc_fmt)
			{
				switch(cur1_ipc_fmt)
				{
					case PE_DNT_M14B_IPC_ATV_480:
						PE_SET_REG_TABLE(DNT_HW_M14,ipc_l_atv_480_default_m14b0,PE_PE0_REG_M14B_MSDIFF);
						break;
					case PE_DNT_M14B_IPC_ATV_576:
						PE_SET_REG_TABLE(DNT_HW_M14,ipc_l_atv_576_default_m14b0,PE_PE0_REG_M14B_MSDIFF);
						break;
					case PE_DNT_M14B_IPC_AV_480:
						PE_SET_REG_TABLE(DNT_HW_M14,ipc_l_av_480_default_m14b0,PE_PE0_REG_M14B_MSDIFF);
						break;
					case PE_DNT_M14B_IPC_AV_576:
						PE_SET_REG_TABLE(DNT_HW_M14,ipc_l_av_576_default_m14b0,PE_PE0_REG_M14B_MSDIFF);
						break;
					case PE_DNT_M14B_IPC_DTV_480:
						PE_SET_REG_TABLE(DNT_HW_M14,ipc_l_dtv_480_default_m14b0,PE_PE0_REG_M14B_MSDIFF);
						break;
					case PE_DNT_M14B_IPC_DTV_576:
						PE_SET_REG_TABLE(DNT_HW_M14,ipc_l_dtv_576_default_m14b0,PE_PE0_REG_M14B_MSDIFF);
						break;
					case PE_DNT_M14B_IPC_DTV_HD:
						PE_SET_REG_TABLE(DNT_HW_M14,ipc_l_dtv_hd_default_m14b0,PE_PE0_REG_M14B_MSDIFF);
						break;
					case PE_DNT_M14B_IPC_DTV_HD_TP:
						PE_SET_REG_TABLE(DNT_HW_M14,ipc_l_dtv_hd_tp_default_m14b0,PE_PE0_REG_M14B_MSDIFF);
						break;
					case PE_DNT_M14B_IPC_HD:
						PE_SET_REG_TABLE(DNT_HW_M14,ipc_l_hd_default_m14b0,PE_PE0_REG_M14B_MSDIFF);
						break;
					case PE_DNT_M14B_IPC_PC:
						PE_SET_REG_TABLE(DNT_HW_M14,ipc_l_pc_default_m14b0,PE_PE0_REG_M14B_MSDIFF);
						break;
					case PE_DNT_M14B_IPC_SD_480:
						PE_SET_REG_TABLE(DNT_HW_M14,ipc_l_sd_480_default_m14b0,PE_PE0_REG_M14B_MSDIFF);
						break;
					case PE_DNT_M14B_IPC_SD_576:
						PE_SET_REG_TABLE(DNT_HW_M14,ipc_l_sd_576_default_m14b0,PE_PE0_REG_M14B_MSDIFF);
						break;
					case PE_DNT_M14B_IPC_SECAM:
						PE_SET_REG_TABLE(DNT_HW_M14,ipc_l_secam_default_m14b0,PE_PE0_REG_M14B_MSDIFF);
						break;
					default:
						break;
				}
				pInfo->ipc1b_fmt=cur1_ipc_fmt;
			}
			/* 20140519, sj.youm, div_mode off for sub path because of fw not working */
			PE_P0S_M14B0_QWr04(p0s_ipc_ctrl_35, reg_manual_mode1, 0x0, \
												reg_manual_mode2, 0x0, \
												reg_manual_en, 0x0, \
												reg_div_en, 0x0);
		}
		/* pe0m clc */
		if(PE_CHECK_WIN0(pstParams->win_id))
		{
			cur0_clc_fmt = PE_DNT_HW_M14B_ConvDispInfoToClcFmt(cur0_inf);
			PE_DNT_HW_M14B_DBG_PRINT_CLC_FMT_STATE(pInfo->clc0b_fmt,cur0_clc_fmt);
			if(pInfo->clc0b_fmt!=cur0_clc_fmt)
			{
				#ifdef PE_HW_M14B_USE_REG_CTRL
				switch(cur0_clc_fmt)
				{
					case PE_DNT_M14B_CLC_SD_50:
						PE_DNT_HW_M14B_CLC_REG_TABLE(clc_l_sd_50_default_m14b0,PE_REG_M14B_WRITE_BASE);
						break;
					case PE_DNT_M14B_CLC_SD_60:
						PE_DNT_HW_M14B_CLC_REG_TABLE(clc_l_sd_60_default_m14b0,PE_REG_M14B_WRITE_BASE);
						break;
					case PE_DNT_M14B_CLC_HD:
						PE_DNT_HW_M14B_CLC_REG_TABLE(clc_l_hd_default_m14b0,PE_REG_M14B_WRITE_BASE);
						break;
					case PE_DNT_M14B_CLC_PC:
						PE_DNT_HW_M14B_CLC_REG_TABLE(clc_l_pc_default_m14b0,PE_REG_M14B_WRITE_BASE);
						break;
					default:
						break;
				}
				#else
				switch(cur0_clc_fmt)
				{
					case PE_DNT_M14B_CLC_SD_50:
						PE_DNT_HW_M14B_SET_CLC_REG_TABLE(DNT_HW_M14,clc_l_sd_50_default_m14b0,	PE_REG_M14B_WRITE_BASE);
						break;
					case PE_DNT_M14B_CLC_SD_60:
						PE_DNT_HW_M14B_SET_CLC_REG_TABLE(DNT_HW_M14,clc_l_sd_60_default_m14b0,	PE_REG_M14B_WRITE_BASE);
						break;
					case PE_DNT_M14B_CLC_HD:
						PE_DNT_HW_M14B_SET_CLC_REG_TABLE(DNT_HW_M14,clc_l_hd_default_m14b0,		PE_REG_M14B_WRITE_BASE);
						break;
					case PE_DNT_M14B_CLC_PC:
						PE_DNT_HW_M14B_SET_CLC_REG_TABLE(DNT_HW_M14,clc_l_pc_default_m14b0,		PE_REG_M14B_WRITE_BASE);
						break;
					default:
						break;
				}
				#endif
				pInfo->clc0b_fmt=cur0_clc_fmt;
			}
		}
		/* pe0d vfy */
		if(PE_CHECK_WIN0(pstParams->win_id))
		{
			cur0_vfy_fmt = PE_DNT_HW_M14B_ConvDispInfoToVfyFmt(cur0_inf);
			PE_DNT_HW_M14B_DBG_PRINT_VFY_FMT_STATE(pInfo->vfy0b_fmt,cur0_vfy_fmt);
			if(pInfo->vfy0b_fmt!=cur0_vfy_fmt)
			{
				#ifdef PE_HW_M14B_USE_REG_CTRL
				switch(cur0_vfy_fmt)
				{
					case PE_DNT_M14B_VFY_ATV:
						PE_DNT_HW_M14B_VFY_REG_TABLE(vflt_y_l_atv_default_m14b0,PE_REG_M14B_WRITE_BASE);
						break;
					case PE_DNT_M14B_VFY_AV:
						PE_DNT_HW_M14B_VFY_REG_TABLE(vflt_y_l_av_default_m14b0,PE_REG_M14B_WRITE_BASE);
						break;
					case PE_DNT_M14B_VFY_HD:
						PE_DNT_HW_M14B_VFY_REG_TABLE(vflt_y_l_hd_default_m14b0,PE_REG_M14B_WRITE_BASE);
						break;
					case PE_DNT_M14B_VFY_PC:
						PE_DNT_HW_M14B_VFY_REG_TABLE(vflt_y_l_pc_default_m14b0,PE_REG_M14B_WRITE_BASE);
						break;
					case PE_DNT_M14B_VFY_SD:
						PE_DNT_HW_M14B_VFY_REG_TABLE(vflt_y_l_sd_default_m14b0,PE_REG_M14B_WRITE_BASE);
						break;
					case PE_DNT_M14B_VFY_SD_DTV:
						PE_DNT_HW_M14B_VFY_REG_TABLE(vflt_y_l_sd_dtv_default_m14b0,PE_REG_M14B_WRITE_BASE);
						break;
					case PE_DNT_M14B_VFY_SECAM:
						PE_DNT_HW_M14B_VFY_REG_TABLE(vflt_y_l_secam_default_m14b0,PE_REG_M14B_WRITE_BASE);
						break;
					default:
						break;
				}
				#else
				switch(cur0_vfy_fmt)
				{
					case PE_DNT_M14B_VFY_ATV:
						PE_SET_REG_TABLE(DNT_HW_M14,vflt_y_l_atv_default_m14b0,PE_REG_M14B_WRITE_BASE);
						break;
					case PE_DNT_M14B_VFY_AV:
						PE_SET_REG_TABLE(DNT_HW_M14,vflt_y_l_av_default_m14b0,PE_REG_M14B_WRITE_BASE);
						break;
					case PE_DNT_M14B_VFY_HD:
						PE_SET_REG_TABLE(DNT_HW_M14,vflt_y_l_hd_default_m14b0,PE_REG_M14B_WRITE_BASE);
						break;
					case PE_DNT_M14B_VFY_PC:
						PE_SET_REG_TABLE(DNT_HW_M14,vflt_y_l_pc_default_m14b0,PE_REG_M14B_WRITE_BASE);
						break;
					case PE_DNT_M14B_VFY_SD:
						PE_SET_REG_TABLE(DNT_HW_M14,vflt_y_l_sd_default_m14b0,PE_REG_M14B_WRITE_BASE);
						break;
					case PE_DNT_M14B_VFY_SD_DTV:
						PE_SET_REG_TABLE(DNT_HW_M14,vflt_y_l_sd_dtv_default_m14b0,PE_REG_M14B_WRITE_BASE);
						break;
					case PE_DNT_M14B_VFY_SECAM:
						PE_SET_REG_TABLE(DNT_HW_M14,vflt_y_l_secam_default_m14b0,PE_REG_M14B_WRITE_BASE);
						break;
					default:
						break;
				}
				#endif
				pInfo->vfy0b_fmt=cur0_vfy_fmt;
			}
		}
	}while(0);	
	return ret;
}
/**
 * set dnt default(for M14AX)
 *
 * @param   *pstParams [in] LX_PE_DEFAULT_SETTINGS_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_DNT_HW_M14A_SetDefault(LX_PE_DEFAULT_SETTINGS_T *pstParams)
{
	int ret = RET_OK;
	PE_INF_M14_SETTINGS_T inf_set;
	LX_PE_INF_DISPLAY_T *cur0_inf = NULL;
	LX_PE_INF_DISPLAY_T *cur1_inf = NULL;
	PE_DNT_HW_M14A_IPC_FMT cur0_ipc_fmt,cur1_ipc_fmt;
	PE_DNT_HW_M14A_CLC_FMT cur0_clc_fmt;
	PE_DNT_HW_M14A_VFY_FMT cur0_vfy_fmt,cur1_vfy_fmt;
	PE_DNT_HW_M14_SETTINGS_T *pInfo = &_g_pe_dnt_hw_m14_info;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		ret = PE_INF_M14_GetCurInfSettings(&inf_set);
		PE_DNT_HW_M14_CHECK_CODE(ret,break,"[%s,%d] PE_INF_M14_GetCurInfSettings() error.\n",__F__,__L__);
		cur0_inf=&inf_set.disp_info[LX_PE_WIN_0];
		cur1_inf=&inf_set.disp_info[LX_PE_WIN_1];
		CHECK_KNULL(cur0_inf);
		CHECK_KNULL(cur1_inf);
		/* L : tpd */
		if(PE_CHECK_WIN0(pstParams->win_id))
		{
			if(cur0_inf->src_type==LX_PE_SRC_DTV && cur0_inf->fmt_type==LX_PE_FMT_HD && \
				cur0_inf->scan_type==LX_PE_SCAN_INTERLACE)
			{
				PE_P0L_M14A0_RdFL(p0l_tpd_ctrl_06);
				PE_P0L_M14A0_RdFL(p0l_tpd_ctrl_07);
				PE_P0L_M14A0_Wr01(p0l_tpd_ctrl_06, reg_force_cb_enable,			0x1);
				PE_P0L_M14A0_Wr01(p0l_tpd_ctrl_07, reg_force_cr_enable,			0x1);
				PE_P0L_M14A0_WrFL(p0l_tpd_ctrl_06);
				PE_P0L_M14A0_WrFL(p0l_tpd_ctrl_07);
			}
			else
			{
				PE_P0L_M14A0_RdFL(p0l_tpd_ctrl_06);
				PE_P0L_M14A0_RdFL(p0l_tpd_ctrl_07);
				PE_P0L_M14A0_Wr01(p0l_tpd_ctrl_06, reg_force_cb_enable,			0x0);
				PE_P0L_M14A0_Wr01(p0l_tpd_ctrl_07, reg_force_cr_enable,			0x0);
				PE_P0L_M14A0_WrFL(p0l_tpd_ctrl_06);
				PE_P0L_M14A0_WrFL(p0l_tpd_ctrl_07);
			}
		}
		/* R : tpd */
		if(PE_CHECK_WIN1(pstParams->win_id))
		{
			if(cur1_inf->src_type==LX_PE_SRC_DTV && cur1_inf->fmt_type==LX_PE_FMT_HD && \
				cur1_inf->scan_type==LX_PE_SCAN_INTERLACE)
			{
				PE_P0R_M14A0_RdFL(p0r_tpd_ctrl_06);
				PE_P0R_M14A0_RdFL(p0r_tpd_ctrl_07);
				PE_P0R_M14A0_Wr01(p0r_tpd_ctrl_06, reg_force_cb_enable,			0x1);
				PE_P0R_M14A0_Wr01(p0r_tpd_ctrl_07, reg_force_cr_enable,			0x1);
				PE_P0R_M14A0_WrFL(p0r_tpd_ctrl_06);
				PE_P0R_M14A0_WrFL(p0r_tpd_ctrl_07);
			}
			else
			{
				PE_P0R_M14A0_RdFL(p0r_tpd_ctrl_06);
				PE_P0R_M14A0_RdFL(p0r_tpd_ctrl_07);
				PE_P0R_M14A0_Wr01(p0r_tpd_ctrl_06, reg_force_cb_enable,			0x0);
				PE_P0R_M14A0_Wr01(p0r_tpd_ctrl_07, reg_force_cr_enable,			0x0);
				PE_P0R_M14A0_WrFL(p0r_tpd_ctrl_06);
				PE_P0R_M14A0_WrFL(p0r_tpd_ctrl_07);
			}
		}
		/* L : ipc */
		if(PE_CHECK_WIN0(pstParams->win_id))
		{
			cur0_ipc_fmt = PE_DNT_HW_M14A_ConvDispInfoToIpcFmt(cur0_inf);
			PE_DNT_HW_M14A_DBG_PRINT_IPC_FMT_STATE(pInfo->ipc0a_fmt,cur0_ipc_fmt);
			if(pInfo->ipc0a_fmt!=cur0_ipc_fmt)
			{
				switch(cur0_ipc_fmt)
				{
					case PE_DNT_M14A_IPC_ATV_480:
						PE_DNT_HW_M14A_SET_IPC_REG_TABLE(ipc_l_atv_480_default_m14a0,	PE_REG_M14_WRITE_BASE);
						break;
					case PE_DNT_M14A_IPC_ATV_576:
						PE_DNT_HW_M14A_SET_IPC_REG_TABLE(ipc_l_atv_576_default_m14a0,	PE_REG_M14_WRITE_BASE);
						break;
					case PE_DNT_M14A_IPC_DTV_480:
						PE_DNT_HW_M14A_SET_IPC_REG_TABLE(ipc_l_dtv_480_default_m14a0,	PE_REG_M14_WRITE_BASE);
						break;
					case PE_DNT_M14A_IPC_DTV_576:
						PE_DNT_HW_M14A_SET_IPC_REG_TABLE(ipc_l_dtv_576_default_m14a0,	PE_REG_M14_WRITE_BASE);
						break;
					case PE_DNT_M14A_IPC_DTV_HD:
						if(cur0_inf->mode.is_ext_frc)	/* external frc,3dfmt such as udtv */
						{
							PE_DNT_HW_M14A_SET_IPC_REG_TABLE(ipc_l_dtv_hd_default_for_udtv_m14a0,	PE_REG_M14_WRITE_BASE);
						}
						else	/* internal frc,3dfmt */
						{
							PE_DNT_HW_M14A_SET_IPC_REG_TABLE(ipc_l_dtv_hd_default_m14a0,	PE_REG_M14_WRITE_BASE);
						}
						break;
					case PE_DNT_M14A_IPC_AV_480:
						PE_DNT_HW_M14A_SET_IPC_REG_TABLE(ipc_l_av_480_default_m14a0,	PE_REG_M14_WRITE_BASE);
						break;
					case PE_DNT_M14A_IPC_AV_576:
						PE_DNT_HW_M14A_SET_IPC_REG_TABLE(ipc_l_av_576_default_m14a0,	PE_REG_M14_WRITE_BASE);
						break;
					case PE_DNT_M14A_IPC_SCARTRGB:
						PE_DNT_HW_M14A_SET_IPC_REG_TABLE(ipc_l_scartrgb_default_m14a0,	PE_REG_M14_WRITE_BASE);
						break;
					case PE_DNT_M14A_IPC_SD_480:
						PE_DNT_HW_M14A_SET_IPC_REG_TABLE(ipc_l_sd_480_default_m14a0,	PE_REG_M14_WRITE_BASE);
						break;
					case PE_DNT_M14A_IPC_SD_576:
						PE_DNT_HW_M14A_SET_IPC_REG_TABLE(ipc_l_sd_576_default_m14a0,	PE_REG_M14_WRITE_BASE);
						break;
					case PE_DNT_M14A_IPC_HD:
						PE_DNT_HW_M14A_SET_IPC_REG_TABLE(ipc_l_hd_default_m14a0,		PE_REG_M14_WRITE_BASE);
						break;
					case PE_DNT_M14A_IPC_PC:
						PE_DNT_HW_M14A_SET_IPC_REG_TABLE(ipc_l_pc_default_m14a0,		PE_REG_M14_WRITE_BASE);
						break;
					default:
						break;
				}
				#if 0	// changed_to_fw_control
				PE_P1L_M14A0_RdFL(pe1_dse_ctrl_01);
				PE_P1L_M14A0_Wr01(pe1_dse_ctrl_01, reg_fmd_en_0,	0x1);
				PE_P1L_M14A0_WrFL(pe1_dse_ctrl_01);
				#endif
				pInfo->ipc0a_fmt=cur0_ipc_fmt;
			}
		}
		/* R : ipc */
		if(PE_CHECK_WIN1(pstParams->win_id))
		{
			cur1_ipc_fmt = PE_DNT_HW_M14A_ConvDispInfoToIpcFmt(cur1_inf);
			PE_DNT_HW_M14A_DBG_PRINT_IPC_FMT_STATE(pInfo->ipc1a_fmt,cur1_ipc_fmt);
			if(pInfo->ipc1a_fmt!=cur1_ipc_fmt)
			{
				switch(cur1_ipc_fmt)
				{
					case PE_DNT_M14A_IPC_ATV_480:
						PE_DNT_HW_M14A_SET_IPC_REG_TABLE(ipc_l_atv_480_default_m14a0,	PE_PE0_REG_M14_LRDIFF);
						break;
					case PE_DNT_M14A_IPC_ATV_576:
						PE_DNT_HW_M14A_SET_IPC_REG_TABLE(ipc_l_atv_576_default_m14a0,	PE_PE0_REG_M14_LRDIFF);
						break;
					case PE_DNT_M14A_IPC_DTV_480:
						PE_DNT_HW_M14A_SET_IPC_REG_TABLE(ipc_l_dtv_480_default_m14a0,	PE_PE0_REG_M14_LRDIFF);
						break;
					case PE_DNT_M14A_IPC_DTV_576:
						PE_DNT_HW_M14A_SET_IPC_REG_TABLE(ipc_l_dtv_576_default_m14a0,	PE_PE0_REG_M14_LRDIFF);
						break;
					case PE_DNT_M14A_IPC_DTV_HD:
						if(cur0_inf->mode.is_ext_frc)	/* external frc,3dfmt such as udtv */
						{
							PE_DNT_HW_M14A_SET_IPC_REG_TABLE(ipc_l_dtv_hd_default_for_udtv_m14a0,	PE_PE0_REG_M14_LRDIFF);
						}
						else	/* internal frc,3dfmt */
						{
						PE_DNT_HW_M14A_SET_IPC_REG_TABLE(ipc_l_dtv_hd_default_m14a0,	PE_PE0_REG_M14_LRDIFF);
						}
						break;
					case PE_DNT_M14A_IPC_AV_480:
						PE_DNT_HW_M14A_SET_IPC_REG_TABLE(ipc_l_av_480_default_m14a0,	PE_PE0_REG_M14_LRDIFF);
						break;
					case PE_DNT_M14A_IPC_AV_576:
						PE_DNT_HW_M14A_SET_IPC_REG_TABLE(ipc_l_av_576_default_m14a0,	PE_PE0_REG_M14_LRDIFF);
						break;
					case PE_DNT_M14A_IPC_SCARTRGB:
						PE_DNT_HW_M14A_SET_IPC_REG_TABLE(ipc_l_scartrgb_default_m14a0,	PE_PE0_REG_M14_LRDIFF);
						break;
					case PE_DNT_M14A_IPC_SD_480:
						PE_DNT_HW_M14A_SET_IPC_REG_TABLE(ipc_l_sd_480_default_m14a0,	PE_PE0_REG_M14_LRDIFF);
						break;
					case PE_DNT_M14A_IPC_SD_576:
						PE_DNT_HW_M14A_SET_IPC_REG_TABLE(ipc_l_sd_576_default_m14a0,	PE_PE0_REG_M14_LRDIFF);
						break;
					case PE_DNT_M14A_IPC_HD:
						PE_DNT_HW_M14A_SET_IPC_REG_TABLE(ipc_l_hd_default_m14a0,		PE_PE0_REG_M14_LRDIFF);
						break;
					case PE_DNT_M14A_IPC_PC:
						PE_DNT_HW_M14A_SET_IPC_REG_TABLE(ipc_l_pc_default_m14a0,		PE_PE0_REG_M14_LRDIFF);
						break;
					default:
						break;
				}
				#if 0	// changed_to_fw_control
				PE_P1L_M14A0_RdFL(pe1_dse_ctrl_01);
				PE_P1L_M14A0_Wr01(pe1_dse_ctrl_01, reg_fmd_en_1,	0x1);
				PE_P1L_M14A0_WrFL(pe1_dse_ctrl_01);
				#endif
				pInfo->ipc1a_fmt=cur1_ipc_fmt;
			}
		}
		/* L : clc */
		if(PE_CHECK_WIN0(pstParams->win_id))
		{
			cur0_clc_fmt = PE_DNT_HW_M14A_ConvDispInfoToClcFmt(cur0_inf);
			PE_DNT_HW_M14A_DBG_PRINT_CLC_FMT_STATE(pInfo->clc0a_fmt,cur0_clc_fmt);
			if(pInfo->clc0a_fmt!=cur0_clc_fmt)
			{
				switch(cur0_clc_fmt)
				{
					case PE_DNT_M14A_CLC_AV:
						PE_DNT_HW_M14A_SET_CLC_REG_TABLE(DNT_HW_M14,clc_l_av_default_m14a0,		PE_REG_M14_WRITE_BASE);
						break;
					case PE_DNT_M14A_CLC_SD:
						PE_DNT_HW_M14A_SET_CLC_REG_TABLE(DNT_HW_M14,clc_l_sd_default_m14a0,		PE_REG_M14_WRITE_BASE);
						break;
					case PE_DNT_M14A_CLC_HD:
						PE_DNT_HW_M14A_SET_CLC_REG_TABLE(DNT_HW_M14,clc_l_hd_default_m14a0,		PE_REG_M14_WRITE_BASE);
						break;
					case PE_DNT_M14A_CLC_PC:
						PE_DNT_HW_M14A_SET_CLC_REG_TABLE(DNT_HW_M14,clc_l_pc_default_m14a0,		PE_REG_M14_WRITE_BASE);
						break;
					default:
						break;
				}
				pInfo->clc0a_fmt=cur0_clc_fmt;
			}
		}
		/* L : vflt y */
		if(PE_CHECK_WIN0(pstParams->win_id))
		{
			cur0_vfy_fmt = PE_DNT_HW_M14A_ConvDispInfoToVfyFmt(cur0_inf);
			PE_DNT_HW_M14A_DBG_PRINT_VFY_FMT_STATE(pInfo->vfy0a_fmt,cur0_vfy_fmt);
			if(pInfo->vfy0a_fmt!=cur0_vfy_fmt)
			{
				switch(cur0_vfy_fmt)
				{
					case PE_DNT_M14A_VFY_SD:
						PE_SET_REG_TABLE(DNT_HW_M14,vflt_y_l_sd_default_m14a0,		PE_REG_M14_WRITE_BASE);
						break;
					case PE_DNT_M14A_VFY_HD_I:
						PE_SET_REG_TABLE(DNT_HW_M14,vflt_y_l_hd_i_default_m14a0,	PE_REG_M14_WRITE_BASE);
						break;
					case PE_DNT_M14A_VFY_HD_P:
						PE_SET_REG_TABLE(DNT_HW_M14,vflt_y_l_hd_p_default_m14a0,	PE_REG_M14_WRITE_BASE);
						break;
					case PE_DNT_M14A_VFY_PC:
						PE_SET_REG_TABLE(DNT_HW_M14,vflt_y_l_pc_default_m14a0,		PE_REG_M14_WRITE_BASE);
						break;
					default:
						break;
				}
				pInfo->vfy0a_fmt=cur0_vfy_fmt;
			}
		}
		/* R : vflt y */
		if(PE_CHECK_WIN1(pstParams->win_id))
		{
			cur1_vfy_fmt = PE_DNT_HW_M14A_ConvDispInfoToVfyFmt(cur1_inf);
			PE_DNT_HW_M14A_DBG_PRINT_VFY_FMT_STATE(pInfo->vfy1a_fmt,cur1_vfy_fmt);
			if(pInfo->vfy1a_fmt!=cur1_vfy_fmt)
			{
				switch(cur1_vfy_fmt)
				{
					case PE_DNT_M14A_VFY_SD:
						PE_SET_REG_TABLE(DNT_HW_M14,vflt_y_l_sd_default_m14a0,		PE_PE0_REG_M14_LRDIFF);
						break;
					case PE_DNT_M14A_VFY_HD_I:
						PE_SET_REG_TABLE(DNT_HW_M14,vflt_y_l_hd_i_default_m14a0,	PE_PE0_REG_M14_LRDIFF);
						break;
					case PE_DNT_M14A_VFY_HD_P:
						PE_SET_REG_TABLE(DNT_HW_M14,vflt_y_l_hd_p_default_m14a0,	PE_PE0_REG_M14_LRDIFF);
						break;
					case PE_DNT_M14A_VFY_PC:
						PE_SET_REG_TABLE(DNT_HW_M14,vflt_y_l_pc_default_m14a0,		PE_PE0_REG_M14_LRDIFF);
						break;
					default:
						break;
				}
				pInfo->vfy1a_fmt=cur1_vfy_fmt;
			}
		}
	}while(0);	
	return ret;
}
/**
 * get ipc fmt using display info(for M14BX)
 *
 * @param   *disp_inf [in] LX_PE_INF_DISPLAY_T
 * @return  PE_DNT_HW_M14B_IPC_FMT
 * @see
 * @author
 */
static PE_DNT_HW_M14B_IPC_FMT PE_DNT_HW_M14B_ConvDispInfoToIpcFmt(LX_PE_INF_DISPLAY_T *disp_inf)
{
	PE_DNT_HW_M14B_IPC_FMT ipc_fmt = PE_DNT_M14B_IPC_NUM;
	switch(disp_inf->src_type)
	{
		case LX_PE_SRC_ATV:
			if(disp_inf->cstd_type==LX_PE_CSTD_SECAM)
			{
				ipc_fmt = PE_DNT_M14B_IPC_SECAM;
			}
			else if(disp_inf->cstd_type==LX_PE_CSTD_NTSC)
			{
				ipc_fmt = PE_DNT_M14B_IPC_ATV_480;
			}
			else
			{
				ipc_fmt = PE_DNT_M14B_IPC_ATV_576;
			}
			break;
		case LX_PE_SRC_CVBS:
		case LX_PE_SRC_SCART:
			if(disp_inf->cstd_type==LX_PE_CSTD_SECAM)
			{
				ipc_fmt = PE_DNT_M14B_IPC_SECAM;
			}
			else if(disp_inf->cstd_type==LX_PE_CSTD_NTSC)
			{
				ipc_fmt = PE_DNT_M14B_IPC_AV_480;
			}
			else
			{
				ipc_fmt = PE_DNT_M14B_IPC_AV_576;
			}
			break;
		case LX_PE_SRC_RGBPC:
			{
				ipc_fmt = PE_DNT_M14B_IPC_PC;
			}
			break;
		case LX_PE_SRC_DTV:
			if(disp_inf->scan_type==LX_PE_SCAN_INTERLACE)
			{
				if(disp_inf->fmt_type==LX_PE_FMT_SD)
				{
					if(disp_inf->fr_type==LX_PE_FR_60HZ)
					{
						ipc_fmt = PE_DNT_M14B_IPC_DTV_480;
					}
					else
					{
						ipc_fmt = PE_DNT_M14B_IPC_DTV_576;
					}
				}
				else
				{
					if(disp_inf->dtv_type==LX_PE_DTV_TESTPIC)
					{
						ipc_fmt = PE_DNT_M14B_IPC_DTV_HD_TP;
					}
					else
					{
						ipc_fmt = PE_DNT_M14B_IPC_DTV_HD;
					}
				}
			}
			else
			{
				ipc_fmt = PE_DNT_M14B_IPC_PC;
			}
			break;
		case LX_PE_SRC_COMP:
		case LX_PE_SRC_HDMI:
		default:
			if(disp_inf->src_type==LX_PE_SRC_HDMI && \
				disp_inf->hdmi_type==LX_PE_HDMI_PC)
			{
				ipc_fmt = PE_DNT_M14B_IPC_PC;
			}
			else if(disp_inf->scan_type==LX_PE_SCAN_INTERLACE)
			{
				if(disp_inf->fmt_type==LX_PE_FMT_SD)
				{
					if(disp_inf->fr_type==LX_PE_FR_60HZ)
					{
						ipc_fmt = PE_DNT_M14B_IPC_SD_480;
					}
					else
					{
						ipc_fmt = PE_DNT_M14B_IPC_SD_576;
					}
				}
				else
				{
					ipc_fmt = PE_DNT_M14B_IPC_HD;
				}
			}
			else
			{
				ipc_fmt = PE_DNT_M14B_IPC_PC;
			}
			break;
	}
	return ipc_fmt;
}
/**
 * get clc fmt using display info(for M14BX)
 *
 * @param   *disp_inf [in] LX_PE_INF_DISPLAY_T
 * @return  PE_DNT_HW_M14B_CLC_FMT
 * @see
 * @author
 */
static PE_DNT_HW_M14B_CLC_FMT PE_DNT_HW_M14B_ConvDispInfoToClcFmt(LX_PE_INF_DISPLAY_T *disp_inf)
{
	PE_DNT_HW_M14B_CLC_FMT clc_fmt = PE_DNT_M14B_CLC_NUM;
	switch(disp_inf->src_type)
	{
		case LX_PE_SRC_ATV:
		case LX_PE_SRC_CVBS:
		case LX_PE_SRC_SCART:
		case LX_PE_SRC_RGBPC:
			{
				clc_fmt = PE_DNT_M14B_CLC_PC;
			}
			break;
		case LX_PE_SRC_DTV:
		case LX_PE_SRC_COMP:
		case LX_PE_SRC_HDMI:
		default:
			if(disp_inf->src_type==LX_PE_SRC_HDMI && \
				disp_inf->hdmi_type==LX_PE_HDMI_PC)
			{
				clc_fmt = PE_DNT_M14B_CLC_PC;
			}
			else if(disp_inf->scan_type==LX_PE_SCAN_INTERLACE)
			{
				if(disp_inf->fmt_type==LX_PE_FMT_SD)
				{
					if(disp_inf->fr_type==LX_PE_FR_60HZ)
					{
						clc_fmt = PE_DNT_M14B_CLC_SD_60;
					}
					else
					{
						clc_fmt = PE_DNT_M14B_CLC_SD_50;
					}
				}
				else
				{
					clc_fmt = PE_DNT_M14B_CLC_HD;
				}
			}
			else
			{
				clc_fmt = PE_DNT_M14B_CLC_PC;
			}
			break;
	}
	return clc_fmt;
}
/**
 * get vfy fmt using display info(for M14BX)
 *
 * @param   *disp_inf [in] LX_PE_INF_DISPLAY_T
 * @return  PE_DNT_HW_M14B_VFY_FMT
 * @see
 * @author
 */
static PE_DNT_HW_M14B_VFY_FMT PE_DNT_HW_M14B_ConvDispInfoToVfyFmt(LX_PE_INF_DISPLAY_T *disp_inf)
{
	PE_DNT_HW_M14B_VFY_FMT vfy_fmt = PE_DNT_M14B_VFY_NUM;
	switch(disp_inf->src_type)
	{
		case LX_PE_SRC_ATV:
			if(disp_inf->cstd_type==LX_PE_CSTD_SECAM)
			{
				vfy_fmt = PE_DNT_M14B_VFY_SECAM;
			}
			else
			{
				vfy_fmt = PE_DNT_M14B_VFY_ATV;
			}
			break;
		case LX_PE_SRC_CVBS:
		case LX_PE_SRC_SCART:
			if(disp_inf->cstd_type==LX_PE_CSTD_SECAM)
			{
				vfy_fmt = PE_DNT_M14B_VFY_SECAM;
			}
			else
			{
				vfy_fmt = PE_DNT_M14B_VFY_AV;
			}
			break;
		case LX_PE_SRC_RGBPC:
			{
				vfy_fmt = PE_DNT_M14B_VFY_PC;
			}
			break;
		case LX_PE_SRC_DTV:
		case LX_PE_SRC_COMP:
		case LX_PE_SRC_HDMI:
		default:
			if(disp_inf->src_type==LX_PE_SRC_HDMI && \
				disp_inf->hdmi_type==LX_PE_HDMI_PC)
			{
				vfy_fmt = PE_DNT_M14B_VFY_PC;
			}
			else if(disp_inf->scan_type==LX_PE_SCAN_INTERLACE)
			{
				if(disp_inf->fmt_type==LX_PE_FMT_SD)
				{
					if(disp_inf->src_type==LX_PE_SRC_DTV)
					{
						vfy_fmt = PE_DNT_M14B_VFY_SD_DTV;
					}
					else
					{
						vfy_fmt = PE_DNT_M14B_VFY_SD;
					}
				}
				else
				{
					vfy_fmt = PE_DNT_M14B_VFY_HD;
				}
			}
			else
			{
				#ifdef PE_DNT_HW_M14B_APPLY_VFLT_Y_ON_DTV_1080P
				if(disp_inf->src_type == LX_PE_SRC_DTV && \
					(disp_inf->dtv_type == LX_PE_DTV_NORMAL || \
					disp_inf->dtv_type == LX_PE_DTV_HDDPLAY) && \
					disp_inf->fmt_type == LX_PE_FMT_HD && \
					disp_inf->in_h_size > 1900 && \
					disp_inf->in_v_size > 1000 && \
					disp_inf->fr_type == LX_PE_FR_50HZ)
				{
					vfy_fmt = PE_DNT_M14B_VFY_HD;
				}
				else
				{
					vfy_fmt = PE_DNT_M14B_VFY_PC;
				}
				#else
				vfy_fmt = PE_DNT_M14B_VFY_PC;
				#endif
			}
			break;
	}
	return vfy_fmt;
}
/**
 * get ipc fmt using display info(for M14AX)
 *
 * @param   *disp_inf [in] LX_PE_INF_DISPLAY_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static PE_DNT_HW_M14A_IPC_FMT PE_DNT_HW_M14A_ConvDispInfoToIpcFmt(LX_PE_INF_DISPLAY_T *disp_inf)
{
	PE_DNT_HW_M14A_IPC_FMT ipc_fmt = PE_DNT_M14A_IPC_NUM;
	switch(disp_inf->src_type)
	{
		case LX_PE_SRC_ATV:
			if(disp_inf->cstd_type==LX_PE_CSTD_NTSC)
			{
				ipc_fmt = PE_DNT_M14A_IPC_ATV_480;
			}
			else
			{
				ipc_fmt = PE_DNT_M14A_IPC_ATV_576;
			}
			break;
		case LX_PE_SRC_CVBS:
		case LX_PE_SRC_SCART:
			if(disp_inf->src_type==LX_PE_SRC_SCART && \
				disp_inf->scart_type==LX_PE_SCART_RGB)
			{
				ipc_fmt = PE_DNT_M14A_IPC_SCARTRGB;
			}
			else if(disp_inf->cstd_type==LX_PE_CSTD_NTSC)
			{
				ipc_fmt = PE_DNT_M14A_IPC_AV_480;
			}
			else
			{
				ipc_fmt = PE_DNT_M14A_IPC_AV_576;
			}
			break;
		case LX_PE_SRC_RGBPC:
			{
				ipc_fmt = PE_DNT_M14A_IPC_PC;
			}
			break;
		case LX_PE_SRC_DTV:
			if(disp_inf->scan_type==LX_PE_SCAN_INTERLACE)
			{
				if(disp_inf->fmt_type==LX_PE_FMT_SD)
				{
					if(disp_inf->fr_type==LX_PE_FR_60HZ)
					{
						ipc_fmt = PE_DNT_M14A_IPC_DTV_480;
					}
					else
					{
						ipc_fmt = PE_DNT_M14A_IPC_DTV_576;
					}
				}
				else
				{
					ipc_fmt = PE_DNT_M14A_IPC_DTV_HD;
				}
			}
			else
			{
				ipc_fmt = PE_DNT_M14A_IPC_PC;
			}
			break;
		case LX_PE_SRC_COMP:
		case LX_PE_SRC_HDMI:
		default:
			if(disp_inf->src_type==LX_PE_SRC_HDMI && \
				disp_inf->hdmi_type==LX_PE_HDMI_PC)
			{
				ipc_fmt = PE_DNT_M14A_IPC_PC;
			}
			else if(disp_inf->scan_type==LX_PE_SCAN_INTERLACE)
			{
				if(disp_inf->fmt_type==LX_PE_FMT_SD)
				{
					if(disp_inf->fr_type==LX_PE_FR_60HZ)
					{
						ipc_fmt = PE_DNT_M14A_IPC_SD_480;
					}
					else
					{
						ipc_fmt = PE_DNT_M14A_IPC_SD_576;
					}
				}
				else
				{
					ipc_fmt = PE_DNT_M14A_IPC_HD;
				}
			}
			else
			{
				ipc_fmt = PE_DNT_M14A_IPC_PC;
			}
			break;
	}
	return ipc_fmt;
}
/**
 * get clc fmt using display info(for M14AX)
 *
 * @param   *disp_inf [in] LX_PE_INF_DISPLAY_T
 * @return  PE_DNT_HW_M14A_CLC_FMT
 * @see
 * @author
 */
static PE_DNT_HW_M14A_CLC_FMT PE_DNT_HW_M14A_ConvDispInfoToClcFmt(LX_PE_INF_DISPLAY_T *disp_inf)
{
	PE_DNT_HW_M14A_CLC_FMT clc_fmt = PE_DNT_M14A_CLC_NUM;
	if((disp_inf->out_type>=LX_PE_OUT_UD && disp_inf->out_type<=LX_PE_OUT_DUAL_FULL) || \
		(disp_inf->out_type==LX_PE_OUT_3D_2D))
	{
		clc_fmt = PE_DNT_M14A_CLC_PC;
	}
	else	//2d,2d_3d,pip
	{
		switch(disp_inf->src_type)
		{
			case LX_PE_SRC_ATV:
			case LX_PE_SRC_CVBS:
			case LX_PE_SRC_SCART:
				{
					clc_fmt = PE_DNT_M14A_CLC_AV;
				}
				break;
			case LX_PE_SRC_RGBPC:
				{
					clc_fmt = PE_DNT_M14A_CLC_PC;
				}
				break;
			case LX_PE_SRC_DTV:
			case LX_PE_SRC_COMP:
			case LX_PE_SRC_HDMI:
			default:
				if(disp_inf->src_type==LX_PE_SRC_HDMI && \
					disp_inf->hdmi_type==LX_PE_HDMI_PC)
				{
					clc_fmt = PE_DNT_M14A_CLC_PC;
				}
				else if(disp_inf->scan_type==LX_PE_SCAN_INTERLACE)
				{
					if(disp_inf->fmt_type==LX_PE_FMT_SD)
					{
						clc_fmt = PE_DNT_M14A_CLC_SD;
					}
					else
					{
						clc_fmt = PE_DNT_M14A_CLC_HD;
					}
				}
				else
				{
					clc_fmt = PE_DNT_M14A_CLC_PC;
				}
				break;
		}
	}
	return clc_fmt;
}
/**
 * get vflt y fmt using display info(for M14AX)
 *
 * @param   *disp_inf [in] LX_PE_INF_DISPLAY_T
 * @return  PE_DNT_HW_M14A_VFY_FMT
 * @see
 * @author
 */
static PE_DNT_HW_M14A_VFY_FMT PE_DNT_HW_M14A_ConvDispInfoToVfyFmt(LX_PE_INF_DISPLAY_T *disp_inf)
{
	PE_DNT_HW_M14A_VFY_FMT vfy_fmt = PE_DNT_M14A_VFY_NUM;
	switch(disp_inf->src_type)
	{
		case LX_PE_SRC_ATV:
		case LX_PE_SRC_CVBS:
		case LX_PE_SRC_SCART:
			{
				vfy_fmt = PE_DNT_M14A_VFY_SD;
			}
			break;
		case LX_PE_SRC_RGBPC:
			{
				vfy_fmt = PE_DNT_M14A_VFY_PC;
			}
			break;
		case LX_PE_SRC_DTV:
		case LX_PE_SRC_COMP:
		case LX_PE_SRC_HDMI:
		default:
			if(disp_inf->src_type==LX_PE_SRC_HDMI && \
				disp_inf->hdmi_type==LX_PE_HDMI_PC)
			{
				vfy_fmt = PE_DNT_M14A_VFY_PC;
			}
			else if(disp_inf->fmt_type==LX_PE_FMT_SD)
			{
				vfy_fmt = PE_DNT_M14A_VFY_SD;
			}
			else
			{
				if(disp_inf->scan_type==LX_PE_SCAN_INTERLACE)
				{
					vfy_fmt = PE_DNT_M14A_VFY_HD_I;
				}
				else
				{
					vfy_fmt = PE_DNT_M14A_VFY_HD_P;
				}
			}
			break;
	}
	return vfy_fmt;
}

/**
 * set ipc default settings(for M14AX)
 *	- keep 0x0500,0x2500[1:0]cr_luma_mode,[5:4]cr_chroma_mode,[31]cr_film_mode_enable
 *
 * @param   *table [in] PE_REG_PARAM_T
 * @param   offset [in] UINT32
 * @param   size [in] UINT32
 * @return  void
 * @see
 * @author
 */
static int PE_DNT_HW_M14A_SetIpcDfltTbl(const PE_REG_PARAM_T *table, UINT32 offset, UINT32 size)
{
	int ret = RET_OK;
	UINT32 table_size,i,r_data;
	PE_REG_PARAM_T *p_data = NULL;
	PE_M14A0_P0L_IPC_CTRL_00_T *p0l_rd_ipc_ctrl_00 = NULL;
	PE_M14A0_P0L_IPC_CTRL_00_T *p0l_wr_ipc_ctrl_00 = NULL;
	PE_M14A0_P0R_IPC_CTRL_00_T *p0r_rd_ipc_ctrl_00 = NULL;
	PE_M14A0_P0R_IPC_CTRL_00_T *p0r_wr_ipc_ctrl_00 = NULL;
	#ifdef PE_DNT_HW_M14A_SET_GAME_MODE_BY_FW
	PE_M14A0_P0L_IPC_CTRL_01_T *p0l_rd_ipc_ctrl_01 = NULL;
	PE_M14A0_P0L_IPC_CTRL_01_T *p0l_wr_ipc_ctrl_01 = NULL;
	PE_M14A0_P0R_IPC_CTRL_01_T *p0r_rd_ipc_ctrl_01 = NULL;
	PE_M14A0_P0R_IPC_CTRL_01_T *p0r_wr_ipc_ctrl_01 = NULL;
	PE_DNT_HW_M14_SETTINGS_T *pInfo = &_g_pe_dnt_hw_m14_info;
	#endif
	do {
		PE_DNT_HW_M14_CHECK_CODE(!table,break,"[%s,%d] table is null.\n",__F__,__L__);
		PE_DNT_HW_M14_CHECK_CODE(!size,break,"[%s,%d] size is zero.\n",__F__,__L__);
		p_data = (PE_REG_PARAM_T *)OS_Malloc(size);
		PE_DNT_HW_M14_CHECK_CODE(!p_data,break,"[%s,%d] p_data is null.\n",__F__,__L__);
		table_size = size/sizeof(PE_REG_PARAM_T);
		for(i=0;i<table_size;i++)
		{
			p_data[i].addr = table[i].addr;
			if((table[i].addr+offset)==0x0500)
			{
				p_data[i].data = table[i].data;
				PE_P0L_M14A0_RdFL(p0l_ipc_ctrl_00);
				r_data = PE_P0L_M14A0_Rd(p0l_ipc_ctrl_00);
				p0l_rd_ipc_ctrl_00 = (PE_M14A0_P0L_IPC_CTRL_00_T *)&(r_data);
				p0l_wr_ipc_ctrl_00 = (PE_M14A0_P0L_IPC_CTRL_00_T *)&(p_data[i].data);
				p0l_wr_ipc_ctrl_00->cr_film_mode_enable = p0l_rd_ipc_ctrl_00->cr_film_mode_enable;
			}
			else if((table[i].addr+offset)==0x2500)
			{
				p_data[i].data = table[i].data;
				PE_P0R_M14A0_RdFL(p0r_ipc_ctrl_00);
				r_data = PE_P0R_M14A0_Rd(p0r_ipc_ctrl_00);
				p0r_rd_ipc_ctrl_00 = (PE_M14A0_P0R_IPC_CTRL_00_T *)&(r_data);
				p0r_wr_ipc_ctrl_00 = (PE_M14A0_P0R_IPC_CTRL_00_T *)&(p_data[i].data);
				p0r_wr_ipc_ctrl_00->cr_film_mode_enable = p0r_rd_ipc_ctrl_00->cr_film_mode_enable;
			}
			#ifdef PE_DNT_HW_M14A_SET_GAME_MODE_BY_FW
			else if((table[i].addr+offset)==0x0504)
			{
				p_data[i].data = table[i].data;
				PE_P0L_M14A0_RdFL(p0l_ipc_ctrl_01);
				r_data = PE_P0L_M14A0_Rd(p0l_ipc_ctrl_01);
				p0l_rd_ipc_ctrl_01 = (PE_M14A0_P0L_IPC_CTRL_01_T *)&(r_data);
				p0l_wr_ipc_ctrl_01 = (PE_M14A0_P0L_IPC_CTRL_01_T *)&(p_data[i].data);
				p0l_wr_ipc_ctrl_01->cr_game_mode = p0l_rd_ipc_ctrl_01->cr_game_mode;
				/* store default */
				pInfo->pre_p0l_ipc_ctrl_01 = table[i].data;
			}
			else if((table[i].addr+offset)==0x2504)
			{
				p_data[i].data = table[i].data;
				PE_P0R_M14A0_RdFL(p0r_ipc_ctrl_01);
				r_data = PE_P0R_M14A0_Rd(p0r_ipc_ctrl_01);
				p0r_rd_ipc_ctrl_01 = (PE_M14A0_P0R_IPC_CTRL_01_T *)&(r_data);
				p0r_wr_ipc_ctrl_01 = (PE_M14A0_P0R_IPC_CTRL_01_T *)&(p_data[i].data);
				p0r_wr_ipc_ctrl_01->cr_game_mode = p0r_rd_ipc_ctrl_01->cr_game_mode;
				/* store default */
				pInfo->pre_p0r_ipc_ctrl_01 = table[i].data;
			}
			#endif
			else
			{
				p_data[i].data = table[i].data;
			}
		}
		ret = PE_REG_SetTable(p_data,offset,size);
		PE_DNT_HW_M14_CHECK_CODE(ret,break,"[%s,%d] PE_REG_SetTable() error.\n",__F__,__L__);
	}while(0);
	if(p_data){OS_Free(p_data);}
	return ret;
}

#ifdef PE_HW_M14B_USE_REG_CTRL
#else
/**
 * set clc default settings(for M14BX)
 *	- keep 0x2260,clc_ctrl_00 [1]cl_filter_enable,[2]clc_detection_enable
 *
 * @param   *table [in] PE_REG_PARAM_T
 * @param   offset [in] UINT32
 * @param   size [in] UINT32
 * @return  void
 * @see
 * @author
 */
static int PE_DNT_HW_M14B_SetClcDfltTbl(const PE_REG_PARAM_T *table, UINT32 offset, UINT32 size)
{
	int ret = RET_OK;
	UINT32 table_size,i,r_data;
	PE_REG_PARAM_T *p_data = NULL;
	PE_M14B0_P0M_CLC_CTRL_00_T *p0m_rd_clc_ctrl_00 = NULL;
	PE_M14B0_P0M_CLC_CTRL_00_T *p0m_wr_clc_ctrl_00 = NULL;
	PE_DNT_HW_M14_SETTINGS_T *pInfo = &_g_pe_dnt_hw_m14_info;
	do {
		PE_DNT_HW_M14_CHECK_CODE(!table,break,"[%s,%d] table is null.\n",__F__,__L__);
		PE_DNT_HW_M14_CHECK_CODE(!size,break,"[%s,%d] size is zero.\n",__F__,__L__);
		p_data = (PE_REG_PARAM_T *)OS_Malloc(size);
		PE_DNT_HW_M14_CHECK_CODE(!p_data,break,"[%s,%d] p_data is null.\n",__F__,__L__);
		table_size = size/sizeof(PE_REG_PARAM_T);
		for(i=0;i<table_size;i++)
		{
			p_data[i].addr = table[i].addr;
			if((table[i].addr+offset)==0x2260)
			{
				p_data[i].data = table[i].data;
				PE_P0M_M14B0_RdFL(p0m_clc_ctrl_00);
				r_data = PE_P0M_M14B0_Rd(p0m_clc_ctrl_00);
				p0m_rd_clc_ctrl_00 = (PE_M14B0_P0M_CLC_CTRL_00_T *)&(r_data);
				p0m_wr_clc_ctrl_00 = (PE_M14B0_P0M_CLC_CTRL_00_T *)&(p_data[i].data);
				p0m_wr_clc_ctrl_00->cl_filter_enable = p0m_rd_clc_ctrl_00->cl_filter_enable;
				p0m_wr_clc_ctrl_00->clc_detection_enable = p0m_rd_clc_ctrl_00->clc_detection_enable;
				/* store default */
				pInfo->cur_p0m_clc_ctrl_00 = table[i].data;
			}
			else
			{
				p_data[i].data = table[i].data;
			}
		}
		ret = PE_REG_SetTable(p_data,offset,size);
		PE_DNT_HW_M14_CHECK_CODE(ret,break,"[%s,%d] PE_REG_SetTable() error.\n",__F__,__L__);
	}while(0);
	if(p_data){OS_Free(p_data);}
	return ret;
}
#endif
/**
 * set clc default settings(for M14AX)
 *	- keep 0x0560 [1]cl_filter_enable
 *
 * @param   *table [in] PE_REG_PARAM_T
 * @param   offset [in] UINT32
 * @param   size [in] UINT32
 * @return  void
 * @see
 * @author
 */
static int PE_DNT_HW_M14A_SetClcDfltTbl(const PE_REG_PARAM_T *table, UINT32 offset, UINT32 size)
{
	int ret = RET_OK;
	UINT32 table_size,i,r_data;
	PE_REG_PARAM_T *p_data = NULL;
	PE_M14A0_P0L_CLC_CTRL_00_T *p0l_rd_clc_ctrl_00 = NULL;
	PE_M14A0_P0L_CLC_CTRL_00_T *p0l_wr_clc_ctrl_00 = NULL;
	PE_DNT_HW_M14_SETTINGS_T *pInfo = &_g_pe_dnt_hw_m14_info;
	do {
		PE_DNT_HW_M14_CHECK_CODE(!table,break,"[%s,%d] table is null.\n",__F__,__L__);
		PE_DNT_HW_M14_CHECK_CODE(!size,break,"[%s,%d] size is zero.\n",__F__,__L__);
		p_data = (PE_REG_PARAM_T *)OS_Malloc(size);
		PE_DNT_HW_M14_CHECK_CODE(!p_data,break,"[%s,%d] p_data is null.\n",__F__,__L__);
		table_size = size/sizeof(PE_REG_PARAM_T);
		for(i=0;i<table_size;i++)
		{
			p_data[i].addr = table[i].addr;
			if((table[i].addr+offset)==0x0560)
			{
				p_data[i].data = table[i].data;
				PE_P0L_M14A0_RdFL(p0l_clc_ctrl_00);
				r_data = PE_P0L_M14A0_Rd(p0l_clc_ctrl_00);
				p0l_rd_clc_ctrl_00 = (PE_M14A0_P0L_CLC_CTRL_00_T *)&(r_data);
				p0l_wr_clc_ctrl_00 = (PE_M14A0_P0L_CLC_CTRL_00_T *)&(p_data[i].data);
				p0l_wr_clc_ctrl_00->cl_filter_enable = p0l_rd_clc_ctrl_00->cl_filter_enable;
				p0l_wr_clc_ctrl_00->clc_detection_enable = p0l_rd_clc_ctrl_00->clc_detection_enable;
				/* store default */
				pInfo->cur_p0l_clc_ctrl_00 = table[i].data;
			}
			else
			{
				p_data[i].data = table[i].data;
			}
		}
		ret = PE_REG_SetTable(p_data,offset,size);
		PE_DNT_HW_M14_CHECK_CODE(ret,break,"[%s,%d] PE_REG_SetTable() error.\n",__F__,__L__);
	}while(0);
	if(p_data){OS_Free(p_data);}
	return ret;
}

/**
 * set film mode
 *
 * @param   *pstParams [in]LX_PE_DNT_FILMMODE_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_DNT_HW_M14_SetFilmMode(LX_PE_DNT_FILMMODE_T *pstParams)
{
	int ret = RET_OK;
	do {
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		PE_DNT_HW_M14_DBG_PRINT("set pstParams[%d] : en:%d\n", \
			pstParams->win_id, pstParams->enable);
		if(PE_KDRV_VER_M14BX)
		{
			if(PE_CHECK_WIN0(pstParams->win_id))
			{
				PE_PE1_M14B0_QWr01(pe1_fsw_ctrl_00, reg_fmd_en_0,	(pstParams->enable)? 0x1:0x0);
				#ifdef PE_DNT_HW_M14B_SET_FMD_PARAM_TO_REG_DIRECTLY
				#ifdef PE_HW_M14B_USE_REG_CTRL
				PE_DNT_HW_M14B_P0M_WR01(p0m_ipc_ctrl_00, cr_film_mode_enable,(pstParams->enable)? 0x1:0x0);
				#else
				PE_P0M_M14B0_QWr01(p0m_ipc_ctrl_00, cr_film_mode_enable,(pstParams->enable)? 0x1:0x0);
				#endif
				#endif
			}
			if(PE_CHECK_WIN1(pstParams->win_id))
			{
				PE_PE1_M14B0_QWr01(pe1_fsw_ctrl_00, reg_fmd_en_1,	(pstParams->enable)? 0x1:0x0);
				#ifdef PE_DNT_HW_M14B_SET_FMD_PARAM_TO_REG_DIRECTLY
				PE_P0S_M14B0_QWr01(p0s_ipc_ctrl_00, cr_film_mode_enable,(pstParams->enable)? 0x1:0x0);
				#endif
			}
		}
		else if(PE_KDRV_VER_M14AX)
		{
			if(PE_CHECK_WIN0(pstParams->win_id))
			{
				#if 1	// changed_to_fw_control
				PE_P1L_M14A0_RdFL(pe1_dse_ctrl_01);
				PE_P1L_M14A0_Wr01(pe1_dse_ctrl_01, reg_fmd_en_0,	(pstParams->enable)? 0x1:0x0);
				PE_P1L_M14A0_WrFL(pe1_dse_ctrl_01);
				#else
				PE_P0L_M14A0_RdFL(p0l_ipc_ctrl_00);
				PE_P0L_M14A0_Wr01(p0l_ipc_ctrl_00, cr_film_mode_enable,	(pstParams->enable)? 0x1:0x0);
				PE_P0L_M14A0_WrFL(p0l_ipc_ctrl_00);
				#endif
			}
			if(PE_CHECK_WIN1(pstParams->win_id))
			{
				#if 1	// changed_to_fw_control
				PE_P1L_M14A0_RdFL(pe1_dse_ctrl_01);
				PE_P1L_M14A0_Wr01(pe1_dse_ctrl_01, reg_fmd_en_1,	(pstParams->enable)? 0x1:0x0);
				PE_P1L_M14A0_WrFL(pe1_dse_ctrl_01);
				#else
				PE_P0R_M14A0_RdFL(p0r_ipc_ctrl_00);
				PE_P0R_M14A0_Wr01(p0r_ipc_ctrl_00, cr_film_mode_enable,	(pstParams->enable)? 0x1:0x0);
				PE_P0R_M14A0_WrFL(p0r_ipc_ctrl_00);
				#endif
			}
		}
		else
		{
			PE_DNT_HW_M14_DBG_PRINT("nothing to do\n");	ret = RET_OK;
		}
	} while (0);
	return ret;
}
/**
 * get film mode
 *
 * @param   *pstParams [in/out]LX_PE_DNT_FILMMODE_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_DNT_HW_M14_GetFilmMode(LX_PE_DNT_FILMMODE_T *pstParams)
{
	int ret = RET_OK;
	LX_PE_WIN_ID win_id;
	do {
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		win_id = PE_GET_CHECKED_WINID(pstParams->win_id);
		if(PE_KDRV_VER_M14BX)
		{
			if(PE_CHECK_WIN0(win_id))
			{
				#if 1	// changed_to_fw_control
				PE_PE1_M14B0_QRd01(pe1_fsw_ctrl_00, reg_fmd_en_0,	pstParams->enable);
				#else
				PE_P0M_M14B0_QRd01(p0m_ipc_ctrl_00, cr_film_mode_enable,	pstParams->enable);
				#endif
			}
			if(PE_CHECK_WIN1(win_id))
			{
				#if 1	// changed_to_fw_control
				PE_PE1_M14B0_QRd01(pe1_fsw_ctrl_00, reg_fmd_en_1,	pstParams->enable);
				#else
				PE_P0S_M14B0_QRd01(p0s_ipc_ctrl_00, cr_film_mode_enable,	pstParams->enable);
				#endif
			}
		}
		else if(PE_KDRV_VER_M14AX)
		{
			if(PE_CHECK_WIN0(win_id))
			{
				#if 1	// changed_to_fw_control
				PE_P1L_M14A0_RdFL(pe1_dse_ctrl_01);
				PE_P1L_M14A0_Rd01(pe1_dse_ctrl_01, reg_fmd_en_0,	pstParams->enable);
				#else
				PE_P0L_M14A0_RdFL(p0l_ipc_ctrl_00);
				PE_P0L_M14A0_Rd01(p0l_ipc_ctrl_00, cr_film_mode_enable,	pstParams->enable);
				#endif
			}
			if(PE_CHECK_WIN1(win_id))
			{
				#if 1	// changed_to_fw_control
				PE_P1L_M14A0_RdFL(pe1_dse_ctrl_01);
				PE_P1L_M14A0_Rd01(pe1_dse_ctrl_01, reg_fmd_en_1,	pstParams->enable);
				#else
				PE_P0R_M14A0_RdFL(p0r_ipc_ctrl_00);
				PE_P0R_M14A0_Rd01(p0r_ipc_ctrl_00, cr_film_mode_enable,	pstParams->enable);
				#endif
			}
		}
		else
		{
			PE_DNT_HW_M14_DBG_PRINT("nothing to do\n");	ret = RET_OK;
		}
		PE_DNT_HW_M14_DBG_PRINT("get pstParams[%d] : en:%d\n",pstParams->win_id,pstParams->enable);
	} while (0);
	return ret;
}
/**
 * set low delay mode : control game_en,clc_en
 *
 * @param   *pstParams [in] LX_PE_DNT_LD_MODE_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_DNT_HW_M14_SetLowDelayMode(LX_PE_DNT_LD_MODE_T *pstParams)
{
	int ret = RET_OK;
	UINT32 cr_game_en;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		PE_DNT_HW_M14_DBG_PRINT("set[%d]:en:%d\n", \
			pstParams->win_id, pstParams->enable);
		if(PE_KDRV_VER_M14BX)
		{
			#ifdef PE_DNT_HW_M14B_SET_GAME_MODE_BY_FW
			if(PE_CHECK_WIN0(pstParams->win_id))
			{
				cr_game_en = (pstParams->enable)? 0x1:0x0;
				PE_DNT_HW_M14_DBG_PRINT("set0: not support, game(%d) by fw\n", cr_game_en);
			}
			if(PE_CHECK_WIN1(pstParams->win_id))
			{
				cr_game_en = (pstParams->enable)? 0x1:0x0;
				PE_DNT_HW_M14_DBG_PRINT("set1: not support, game(%d) by fw\n", cr_game_en);
			}
			#else
			PE_DNT_HW_M14_SETTINGS_T *pInfo = &_g_pe_dnt_hw_m14_info;
			PE_M14B0_P0M_IPC_CTRL_01_T *p0m_rd_ipc_ctrl_01 = NULL;	//m14b
			if(PE_CHECK_WIN0(pstParams->win_id))
			{
				cr_game_en = (pstParams->enable)? 0x1:0x0;
				#ifdef PE_HW_M14B_USE_REG_CTRL
				PE_DNT_HW_M14B_P0M_WR01(p0m_ipc_ctrl_01, cr_game_mode,	cr_game_en);
				#else
				PE_P0M_M14B0_QWr01(p0m_ipc_ctrl_01, cr_game_mode,	cr_game_en);
				#endif
				p0m_rd_ipc_ctrl_01 = (PE_M14B0_P0M_IPC_CTRL_01_T *)&(pInfo->pre_p0m_ipc_ctrl_01);
				p0m_rd_ipc_ctrl_01->cr_game_mode = cr_game_en;
			}
			if(PE_CHECK_WIN1(pstParams->win_id))
			{
				cr_game_en = (pstParams->enable)? 0x1:0x0;
				PE_P0S_M14B0_QWr01(p0s_ipc_ctrl_01, cr_game_mode,	cr_game_en);
			}
			#endif
		}
		else if(PE_KDRV_VER_M14AX)
		{
			UINT32 blk_lin_en,dbg_map_en;
			#ifdef PE_DNT_HW_M14A_SET_GAME_MODE_BY_FW
			if(PE_CHECK_WIN0(pstParams->win_id))
			{
				cr_game_en = (pstParams->enable)? 0x1:0x0;
				blk_lin_en = 0x0;
				dbg_map_en = (pstParams->enable)? 0x1:0x0;
				PE_DNT_HW_M14_DBG_PRINT("set0: not support, game(%d) by fw\n", cr_game_en);
			}
			if(PE_CHECK_WIN1(pstParams->win_id))
			{
				cr_game_en = (pstParams->enable)? 0x1:0x0;
				blk_lin_en = 0x0;
				dbg_map_en = (pstParams->enable)? 0x1:0x0;
				PE_DNT_HW_M14_DBG_PRINT("set1: not support, game(%d) by fw\n", cr_game_en);
			}
			#else
			PE_DNT_HW_M14_SETTINGS_T *pInfo = &_g_pe_dnt_hw_m14_info;
			PE_M14A0_P0L_IPC_CTRL_01_T *p0l_rd_ipc_ctrl_01 = NULL;
			if(PE_CHECK_WIN0(pstParams->win_id))
			{
				cr_game_en = (pstParams->enable)? 0x1:0x0;
				blk_lin_en = 0x0;
				dbg_map_en = (pstParams->enable)? 0x1:0x0;
				PE_P0L_M14A0_RdFL(p0l_ipc_ctrl_01);
				PE_P0L_M14A0_Wr01(p0l_ipc_ctrl_01, cr_game_mode,			cr_game_en);
				PE_P0L_M14A0_WrFL(p0l_ipc_ctrl_01);
				PE_P0L_M14A0_RdFL(p0l_tnr_ctrl_10);
				PE_P0L_M14A0_Wr01(p0l_tnr_ctrl_10, tnr_black_line_en_for_i,	blk_lin_en);
				PE_P0L_M14A0_Wr01(p0l_tnr_ctrl_10, tnr_debug_map_en_for_i,	dbg_map_en);
				PE_P0L_M14A0_WrFL(p0l_tnr_ctrl_10);
				p0l_rd_ipc_ctrl_01 = (PE_M14A0_P0L_IPC_CTRL_01_T *)&(pInfo->pre_p0l_ipc_ctrl_01);
				p0l_rd_ipc_ctrl_01->cr_game_mode = cr_game_en;
				PE_DNT_HW_M14_DBG_PRINT("set0:game:0x%x,tnr_line(dbg):0x%x(0x%x)\n",\
					cr_game_en,blk_lin_en,dbg_map_en);
			}
			if(PE_CHECK_WIN1(pstParams->win_id))
			{
				cr_game_en = (pstParams->enable)? 0x1:0x0;
				blk_lin_en = 0x0;
				dbg_map_en = (pstParams->enable)? 0x1:0x0;
				PE_P0R_M14A0_RdFL(p0r_ipc_ctrl_01);
				PE_P0R_M14A0_Wr01(p0r_ipc_ctrl_01, cr_game_mode,			cr_game_en);
				PE_P0R_M14A0_WrFL(p0r_ipc_ctrl_01);
				PE_P0R_M14A0_RdFL(p0r_tnr_ctrl_10);
				PE_P0R_M14A0_Wr01(p0r_tnr_ctrl_10, tnr_black_line_en_for_i,	blk_lin_en);
				PE_P0R_M14A0_Wr01(p0r_tnr_ctrl_10, tnr_debug_map_en_for_i,	dbg_map_en);
				PE_P0R_M14A0_WrFL(p0r_tnr_ctrl_10);
				PE_DNT_HW_M14_DBG_PRINT("set1:game:0x%x,tnr_line(dbg):0x%x(0x%x)\n",\
					cr_game_en,blk_lin_en,dbg_map_en);
			}
			#endif
		}
		else
		{
			PE_DNT_HW_M14_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * get low delay mode : game_en
 *
 * @param   *pstParams [in] LX_PE_DNT_LD_MODE_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_DNT_HW_M14_GetLowDelayMode(LX_PE_DNT_LD_MODE_T *pstParams)
{
	int ret = RET_OK;
	LX_PE_WIN_ID win_id;
	PE_DNT_HW_M14_SETTINGS_T *pInfo = &_g_pe_dnt_hw_m14_info;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		win_id = PE_GET_CHECKED_WINID(pstParams->win_id);
		if(PE_KDRV_VER_M14BX)
		{
			if(PE_CHECK_WIN0(win_id))
			{
				PE_P0M_M14B0_QRd01(p0m_ipc_ctrl_01, cr_game_mode,	pstParams->enable);
				PE_DNT_HW_M14_DBG_PRINT("inf0:clc_ctrl_00:0x%08x\n",pInfo->cur_p0l_clc_ctrl_00);
			}
			if(PE_CHECK_WIN1(win_id))
			{
				PE_P0S_M14B0_QRd01(p0s_ipc_ctrl_01, cr_game_mode,	pstParams->enable);
			}
		}
		else if(PE_KDRV_VER_M14AX)
		{
			if(PE_CHECK_WIN0(win_id))
			{
				PE_P0L_M14A0_RdFL(p0l_ipc_ctrl_01);
				PE_P0L_M14A0_Rd01(p0l_ipc_ctrl_01, cr_game_mode,	pstParams->enable);
				PE_DNT_HW_M14_DBG_PRINT("inf0:clc_ctrl_00:0x%08x\n",pInfo->cur_p0l_clc_ctrl_00);
			}
			if(PE_CHECK_WIN1(win_id))
			{
				PE_P0R_M14A0_RdFL(p0r_ipc_ctrl_01);
				PE_P0R_M14A0_Rd01(p0r_ipc_ctrl_01, cr_game_mode,	pstParams->enable);
			}
		}
		else
		{
			PE_DNT_HW_M14_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
		PE_DNT_HW_M14_DBG_PRINT("get[%d]:en:%d\n", \
			pstParams->win_id, pstParams->enable);
	}while(0);
	return ret;
}

/**
 * set clc, fmd apply
 *
 * @param   *pCfg [in] PE_TSK_M14_CFG_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_DNT_HW_M14B_SetClcFmdApply(PE_TSK_M14_CFG_T *pCfg)
{
	int ret = RET_OK;
	static UINT32 count = 0;
	UINT32 cur_clc_flt_en,cur_clc_det_en;
	UINT32 cur_film_apply_y, cur_film_apply_c;
	PE_INF_M14_CTRL_T *ctrl0_info = &pCfg->inf.ctrl_info[LX_PE_WIN_0];
	LX_PE_INF_DISPLAY_T *disp0_info = &pCfg->inf.disp_info[LX_PE_WIN_0];
	PE_DNT_HW_M14_SETTINGS_T *pInfo = &_g_pe_dnt_hw_m14_info;
	PE_M14B0_P0M_CLC_CTRL_00_T *cur_p0m_clc_ctrl_00 = NULL;		//m14b
	PE_M14B0_P0M_CLC_CTRL_00_T *pre_p0m_clc_ctrl_00 = NULL;		//m14b
	PE_M14B0_P0M_IPC_CTRL_00_T *cur_p0m_ipc_ctrl_00 = NULL;		//m14b
	PE_M14B0_P0M_IPC_CTRL_00_T *pre_p0m_ipc_ctrl_00 = NULL;		//m14b
	PE_M14B0_P0M_IPC_CTRL_01_T *pre_p0m_ipc_ctrl_01 = NULL;		//m14b
	#ifdef PE_DNT_HW_M14B_SET_GAME_MODE_BY_FW
	PE_M14B0_P0M_IPC_CTRL_01_T *cur_p0m_ipc_ctrl_01 = NULL;		//m14b
	PE_M14B0_P0M_IPC_CTRL_01_T *reg_p0m_ipc_ctrl_01 = NULL;		//m14b
	UINT32 reg_ipc1_data, cur_tava_en;
	#endif
	#ifdef PE_DNT_HW_M14B_SET_USE_TNR_M_BY_FW
	PE_M14B0_P0M_IPC_CTRL_06_T *cur_p0m_ipc_ctrl_06 = NULL;		//m14b
	PE_M14B0_P0M_IPC_CTRL_06_T *reg_p0m_ipc_ctrl_06 = NULL;		//m14b
	UINT32 reg_ipc6_data;
	#endif
	do{
		CHECK_KNULL(pCfg);
		if(PE_KDRV_VER_M14BX)
		{
			cur_p0m_clc_ctrl_00 = (PE_M14B0_P0M_CLC_CTRL_00_T *)&(pInfo->cur_p0m_clc_ctrl_00);
			pre_p0m_clc_ctrl_00 = (PE_M14B0_P0M_CLC_CTRL_00_T *)&(pInfo->pre_p0m_clc_ctrl_00);
			cur_p0m_ipc_ctrl_00 = (PE_M14B0_P0M_IPC_CTRL_00_T *)&(pInfo->cur_p0m_ipc_ctrl_00);
			pre_p0m_ipc_ctrl_00 = (PE_M14B0_P0M_IPC_CTRL_00_T *)&(pInfo->pre_p0m_ipc_ctrl_00);
			pre_p0m_ipc_ctrl_01 = (PE_M14B0_P0M_IPC_CTRL_01_T *)&(pInfo->pre_p0m_ipc_ctrl_01);
			#ifdef PE_DNT_HW_M14B_SET_GAME_MODE_BY_FW
			cur_p0m_ipc_ctrl_01 = (PE_M14B0_P0M_IPC_CTRL_01_T *)&(pInfo->cur_p0m_ipc_ctrl_01);
			reg_p0m_ipc_ctrl_01 = (PE_M14B0_P0M_IPC_CTRL_01_T *)&(reg_ipc1_data);
			#endif
			#ifdef PE_DNT_HW_M14B_SET_USE_TNR_M_BY_FW
			cur_p0m_ipc_ctrl_06 = (PE_M14B0_P0M_IPC_CTRL_06_T *)&(pInfo->cur_p0m_ipc_ctrl_06);
			reg_p0m_ipc_ctrl_06 = (PE_M14B0_P0M_IPC_CTRL_06_T *)&(reg_ipc6_data);
			/* read current p0m reg. */
			PE_P0M_M14B0_RdFL(p0m_ipc_ctrl_06);
			reg_ipc6_data = PE_P0M_M14B0_Rd(p0m_ipc_ctrl_06);
			/* compare p0m reg. */
			if (reg_p0m_ipc_ctrl_06->cr_normalize_en != cur_p0m_ipc_ctrl_06->cr_normalize_en || \
				reg_p0m_ipc_ctrl_06->cr_flat_only != cur_p0m_ipc_ctrl_06->cr_flat_only || \
				reg_p0m_ipc_ctrl_06->cr_tearing_normalize_en != cur_p0m_ipc_ctrl_06->cr_tearing_normalize_en || \
				reg_p0m_ipc_ctrl_06->cr_normalize_base != cur_p0m_ipc_ctrl_06->cr_normalize_base || \
				reg_p0m_ipc_ctrl_06->cr_normalize_center != cur_p0m_ipc_ctrl_06->cr_normalize_center || \
				reg_p0m_ipc_ctrl_06->cr_motion_coring != cur_p0m_ipc_ctrl_06->cr_motion_coring)
			{
				PE_DNT_HW_M14_DBG_PRINT("[set]p0m ipc06 reg:0x%08x->0x%08x, use_tnr_m:%d\n",\
					reg_ipc6_data, pInfo->cur_p0m_ipc_ctrl_06, reg_p0m_ipc_ctrl_06->use_tnr_motion);
				reg_p0m_ipc_ctrl_06->cr_normalize_en = cur_p0m_ipc_ctrl_06->cr_normalize_en;
				reg_p0m_ipc_ctrl_06->cr_flat_only = cur_p0m_ipc_ctrl_06->cr_flat_only;
				reg_p0m_ipc_ctrl_06->cr_tearing_normalize_en = cur_p0m_ipc_ctrl_06->cr_tearing_normalize_en;
				reg_p0m_ipc_ctrl_06->cr_normalize_base = cur_p0m_ipc_ctrl_06->cr_normalize_base;
				reg_p0m_ipc_ctrl_06->cr_normalize_center = cur_p0m_ipc_ctrl_06->cr_normalize_center;
				reg_p0m_ipc_ctrl_06->cr_motion_coring = cur_p0m_ipc_ctrl_06->cr_motion_coring;
				PE_P0M_M14B0_Wr(p0m_ipc_ctrl_06, reg_ipc6_data);
				PE_P0M_M14B0_WrFL(p0m_ipc_ctrl_06);
				/* udpate reg. mask */
				PE_DNT_HW_M14B_P0M_SET(p0m_ipc_ctrl_06, use_tnr_motion, reg_p0m_ipc_ctrl_06->use_tnr_motion);
			}
			#endif
			#ifdef PE_DNT_HW_M14B_SET_GAME_MODE_BY_FW
			/* read current p0m reg. */
			PE_P0M_M14B0_RdFL(p0m_ipc_ctrl_01);
			reg_ipc1_data = PE_P0M_M14B0_Rd(p0m_ipc_ctrl_01);
			/* compare p0m reg. */
			if (reg_p0m_ipc_ctrl_01->cr_hmc_en_sel1 != cur_p0m_ipc_ctrl_01->cr_hmc_en_sel1 || \
				reg_p0m_ipc_ctrl_01->cr_hmc_en_sel2 != cur_p0m_ipc_ctrl_01->cr_hmc_en_sel2 || \
				reg_p0m_ipc_ctrl_01->cr_fmd_vfilter_en != cur_p0m_ipc_ctrl_01->cr_fmd_vfilter_en || \
				reg_p0m_ipc_ctrl_01->cr_fend_auto != cur_p0m_ipc_ctrl_01->cr_fend_auto || \
				reg_p0m_ipc_ctrl_01->cr_st_flt_enable != cur_p0m_ipc_ctrl_01->cr_st_flt_enable || \
				reg_p0m_ipc_ctrl_01->cr_st_2d_v_flt_en != cur_p0m_ipc_ctrl_01->cr_st_2d_v_flt_en || \
				reg_p0m_ipc_ctrl_01->cr_hmc_flt_enable != cur_p0m_ipc_ctrl_01->cr_hmc_flt_enable || \
				reg_p0m_ipc_ctrl_01->cr_st_2d_v_flt_range != cur_p0m_ipc_ctrl_01->cr_st_2d_v_flt_range || \
				reg_p0m_ipc_ctrl_01->cr_black_mask != cur_p0m_ipc_ctrl_01->cr_black_mask || \
				reg_p0m_ipc_ctrl_01->cr_5f_mode != cur_p0m_ipc_ctrl_01->cr_5f_mode)
			{
				PE_DNT_HW_M14_DBG_PRINT("[set]p0m ipc01 reg:0x%08x->0x%08x, game:%d\n",\
					reg_ipc1_data, pInfo->cur_p0m_ipc_ctrl_01, reg_p0m_ipc_ctrl_01->cr_game_mode);
				reg_p0m_ipc_ctrl_01->cr_hmc_en_sel1 = cur_p0m_ipc_ctrl_01->cr_hmc_en_sel1;
				reg_p0m_ipc_ctrl_01->cr_hmc_en_sel2 = cur_p0m_ipc_ctrl_01->cr_hmc_en_sel2;
				reg_p0m_ipc_ctrl_01->cr_fmd_vfilter_en = cur_p0m_ipc_ctrl_01->cr_fmd_vfilter_en;
				reg_p0m_ipc_ctrl_01->cr_fend_auto = cur_p0m_ipc_ctrl_01->cr_fend_auto;
				reg_p0m_ipc_ctrl_01->cr_st_flt_enable = cur_p0m_ipc_ctrl_01->cr_st_flt_enable;
				reg_p0m_ipc_ctrl_01->cr_st_2d_v_flt_en = cur_p0m_ipc_ctrl_01->cr_st_2d_v_flt_en;
				reg_p0m_ipc_ctrl_01->cr_hmc_flt_enable = cur_p0m_ipc_ctrl_01->cr_hmc_flt_enable;
				reg_p0m_ipc_ctrl_01->cr_st_2d_v_flt_range = cur_p0m_ipc_ctrl_01->cr_st_2d_v_flt_range;
				reg_p0m_ipc_ctrl_01->cr_black_mask = cur_p0m_ipc_ctrl_01->cr_black_mask;
				reg_p0m_ipc_ctrl_01->cr_5f_mode = cur_p0m_ipc_ctrl_01->cr_5f_mode;
				PE_P0M_M14B0_Wr(p0m_ipc_ctrl_01, reg_ipc1_data);
				PE_P0M_M14B0_WrFL(p0m_ipc_ctrl_01);
				/* udpate reg. mask */
				PE_DNT_HW_M14B_P0M_SET(p0m_ipc_ctrl_01, cr_game_mode, reg_p0m_ipc_ctrl_01->cr_game_mode);
			}
			/* check clc off case */
			if(ctrl0_info->is_src_frz && !ctrl0_info->mute_on && \
				disp0_info->src_type==LX_PE_SRC_DTV && \
				disp0_info->dtv_type==LX_PE_DTV_FILEPLAY && \
				disp0_info->fmt_type==LX_PE_FMT_HD && \
				disp0_info->scan_type==LX_PE_SCAN_INTERLACE)
			{
				cur_clc_flt_en = PE_DNT_HW_M14_CLC_OFF;		//[1]
				cur_clc_det_en = PE_DNT_HW_M14_CLC_OFF;		//[2]
			}
			else
			{
				/* clc off when game mode on */
				if (reg_p0m_ipc_ctrl_01->cr_game_mode)
				{
					cur_clc_flt_en = PE_DNT_HW_M14_CLC_OFF;		//[1]
					cur_clc_det_en = PE_DNT_HW_M14_CLC_OFF;		//[2]
				}
				else
				{
					cur_clc_flt_en = cur_p0m_clc_ctrl_00->cl_filter_enable;
					cur_clc_det_en = cur_p0m_clc_ctrl_00->clc_detection_enable;
				}
			}
			/* film apply off when game mode on */
			if (reg_p0m_ipc_ctrl_01->cr_game_mode)
			{
				cur_film_apply_y = PE_DNT_HW_M14_FMD_OFF;		//[30]
				cur_film_apply_c = PE_DNT_HW_M14_FMD_OFF;		//[29]
				cur_tava_en = PE_DNT_HW_M14_TAVA_OFF;			//[23]
			}
			else
			{
				cur_film_apply_y = cur_p0m_ipc_ctrl_00->cr_film_apply_y;
				cur_film_apply_c = cur_p0m_ipc_ctrl_00->cr_film_apply_c;
				cur_tava_en = cur_p0m_ipc_ctrl_00->cr_tava_en;
			}
			/* apply clc */
			if (pre_p0m_clc_ctrl_00->cl_filter_enable != cur_clc_flt_en || \
				pre_p0m_clc_ctrl_00->clc_detection_enable != cur_clc_det_en)
			{
				PE_DNT_HW_M14_DBG_PRINT("[inf][clc]frz:%d,mute:%d,src:%d,dtv:%d\n",\
					ctrl0_info->is_src_frz, ctrl0_info->mute_on,\
					disp0_info->src_type, disp0_info->dtv_type);
				PE_DNT_HW_M14_DBG_PRINT("[inf][clc]fmt:%d,scan:%d,out:%d,game:%d\n",\
					disp0_info->fmt_type, disp0_info->scan_type,\
					disp0_info->out_type, reg_p0m_ipc_ctrl_01->cr_game_mode);
				PE_DNT_HW_M14_DBG_PRINT("[set][clc]flt:%d->%d, det:%d->%d\n",\
					pre_p0m_clc_ctrl_00->cl_filter_enable, cur_clc_flt_en,\
					pre_p0m_clc_ctrl_00->clc_detection_enable, cur_clc_det_en);
				#ifdef PE_HW_M14B_USE_REG_CTRL
				#ifdef PE_HW_M14B_USE_FSW_CLC_EN_STAT
				PE_PE1_M14B0_QWr02(pe1_fsw_ctrl_00,	cl_filter_en_stat,	cur_clc_flt_en, \
													clc_detection_en_stat,cur_clc_det_en);
				#else
				PE_DNT_HW_M14B_P0M_WR02(p0m_clc_ctrl_00, cl_filter_enable,	cur_clc_flt_en, \
														clc_detection_enable,cur_clc_det_en);
				#endif
				#else
				PE_P0M_M14B0_QWr02(p0m_clc_ctrl_00, cl_filter_enable,	cur_clc_flt_en, \
													clc_detection_enable,cur_clc_det_en);
				#endif
				pre_p0m_clc_ctrl_00->cl_filter_enable = cur_clc_flt_en;
				pre_p0m_clc_ctrl_00->clc_detection_enable = cur_clc_det_en;
			}
			/* apply fmd */
			if(pre_p0m_ipc_ctrl_00->cr_film_apply_y != cur_film_apply_y || \
				pre_p0m_ipc_ctrl_00->cr_film_apply_c != cur_film_apply_c || \
				pre_p0m_ipc_ctrl_00->cr_tava_en != cur_tava_en)
			{
				PE_DNT_HW_M14_DBG_PRINT("[inf][fmd]game:%d\n", \
					reg_p0m_ipc_ctrl_01->cr_game_mode);
				PE_DNT_HW_M14_DBG_PRINT("[set][fmd]y:%d->%d, c:%d->%d, [tava]%d->%d\n", \
					pre_p0m_ipc_ctrl_00->cr_film_apply_y, cur_film_apply_y, \
					pre_p0m_ipc_ctrl_00->cr_film_apply_c, cur_film_apply_c, \
					pre_p0m_ipc_ctrl_00->cr_tava_en, cur_tava_en);
				#ifdef PE_HW_M14B_USE_REG_CTRL
				PE_DNT_HW_M14B_P0M_WR03(p0m_ipc_ctrl_00, cr_film_apply_y,cur_film_apply_y, \
														cr_film_apply_c,cur_film_apply_c, \
														cr_tava_en,cur_tava_en);
				#else
				PE_P0M_M14B0_QWr03(p0m_ipc_ctrl_00, cr_film_apply_y,cur_film_apply_y, \
													cr_film_apply_c,cur_film_apply_c, \
													cr_tava_en,cur_tava_en);
				#endif
				pre_p0m_ipc_ctrl_00->cr_film_apply_y = cur_film_apply_y;
				pre_p0m_ipc_ctrl_00->cr_film_apply_c = cur_film_apply_c;
				pre_p0m_ipc_ctrl_00->cr_tava_en = cur_tava_en;
			}
			if(_g_dnt_hw_m14_trace)
			{
				if(count>PE_PRINT_COUNT_NUM)
				{
					PE_DNT_HW_M14_DBG_PRINT("[state][inf]frz:%d,mute:%d,src:%d,dtv:%d\n",\
						ctrl0_info->is_src_frz, ctrl0_info->mute_on,\
						disp0_info->src_type, disp0_info->dtv_type);
					PE_DNT_HW_M14_DBG_PRINT("[state][inf]fmt:%d,scan:%d,out:%d,game:%d\n",\
						disp0_info->fmt_type, disp0_info->scan_type,\
						disp0_info->out_type, reg_p0m_ipc_ctrl_01->cr_game_mode);
					PE_DNT_HW_M14_DBG_PRINT("[state][clc]flt:%d,det:%d [fmd]y:%d,c:%d [tava]%d\n",\
						cur_clc_flt_en, cur_clc_det_en, cur_film_apply_y, \
						cur_film_apply_c, cur_tava_en);
					count=0;
				}
				count++;
			}
			#else
			/* check clc off case */
			if(ctrl0_info->is_src_frz && !ctrl0_info->mute_on && \
				disp0_info->src_type==LX_PE_SRC_DTV && \
				disp0_info->dtv_type==LX_PE_DTV_FILEPLAY && \
				disp0_info->fmt_type==LX_PE_FMT_HD && \
				disp0_info->scan_type==LX_PE_SCAN_INTERLACE)
			{
				cur_clc_flt_en = PE_DNT_HW_M14_CLC_OFF;		//[1]
				cur_clc_det_en = PE_DNT_HW_M14_CLC_OFF;		//[2]
			}
			else
			{
				/* clc off when game mode on */
				if (pre_p0m_ipc_ctrl_01->cr_game_mode)
				{
					cur_clc_flt_en = PE_DNT_HW_M14_CLC_OFF;		//[1]
					cur_clc_det_en = PE_DNT_HW_M14_CLC_OFF;		//[2]
				}
				else
				{
					cur_clc_flt_en = cur_p0m_clc_ctrl_00->cl_filter_enable;
					cur_clc_det_en = cur_p0m_clc_ctrl_00->clc_detection_enable;
				}
			}
			/* film apply off when game mode on */
			if (pre_p0m_ipc_ctrl_01->cr_game_mode)
			{
				cur_film_apply_y = PE_DNT_HW_M14_FMD_OFF;		//[1]
				cur_film_apply_c = PE_DNT_HW_M14_FMD_OFF;		//[2]
			}
			else
			{
				cur_film_apply_y = cur_p0m_ipc_ctrl_00->cr_film_apply_y;
				cur_film_apply_c = cur_p0m_ipc_ctrl_00->cr_film_apply_c;
			}
			/* apply clc */
			if (pre_p0m_clc_ctrl_00->cl_filter_enable != cur_clc_flt_en || \
				pre_p0m_clc_ctrl_00->clc_detection_enable != cur_clc_det_en)
			{
				PE_DNT_HW_M14_DBG_PRINT("[inf][clc]frz:%d,mute:%d,src:%d,dtv:%d\n",\
					ctrl0_info->is_src_frz, ctrl0_info->mute_on,\
					disp0_info->src_type, disp0_info->dtv_type);
				PE_DNT_HW_M14_DBG_PRINT("[inf][clc]fmt:%d,scan:%d,out:%d,game:%d\n",\
					disp0_info->fmt_type, disp0_info->scan_type,\
					disp0_info->out_type, pre_p0m_ipc_ctrl_01->cr_game_mode);
				PE_DNT_HW_M14_DBG_PRINT("[set][clc]flt:%d->%d, det:%d->%d\n",\
					pre_p0m_clc_ctrl_00->cl_filter_enable, cur_clc_flt_en,\
					pre_p0m_clc_ctrl_00->clc_detection_enable, cur_clc_det_en);
				#ifdef PE_HW_M14B_USE_REG_CTRL
				PE_DNT_HW_M14B_P0M_WR02(p0m_clc_ctrl_00, cl_filter_enable,	cur_clc_flt_en, \
														clc_detection_enable,cur_clc_det_en);
				#else
				PE_P0M_M14B0_QWr02(p0m_clc_ctrl_00, cl_filter_enable,	cur_clc_flt_en, \
													clc_detection_enable,cur_clc_det_en);
				#endif
				pre_p0m_clc_ctrl_00->cl_filter_enable = cur_clc_flt_en;
				pre_p0m_clc_ctrl_00->clc_detection_enable = cur_clc_det_en;
			}
			/* apply fmd */
			if(pre_p0m_ipc_ctrl_00->cr_film_apply_y != cur_film_apply_y || \
				pre_p0m_ipc_ctrl_00->cr_film_apply_c != cur_film_apply_c)
			{
				PE_DNT_HW_M14_DBG_PRINT("[inf][fmd]game:%d\n", \
					pre_p0m_ipc_ctrl_01->cr_game_mode);
				PE_DNT_HW_M14_DBG_PRINT("[set][fmd]y:%d->%d, c:%d->%d\n", \
					pre_p0m_ipc_ctrl_00->cr_film_apply_y, cur_film_apply_y, \
					pre_p0m_ipc_ctrl_00->cr_film_apply_c, cur_film_apply_c);
				#ifdef PE_HW_M14B_USE_REG_CTRL
				PE_DNT_HW_M14B_P0M_WR02(p0m_ipc_ctrl_00, cr_film_apply_y,cur_film_apply_y, \
														cr_film_apply_c,cur_film_apply_c);
				#else
				PE_P0M_M14B0_QWr02(p0m_ipc_ctrl_00, cr_film_apply_y,cur_film_apply_y, \
													cr_film_apply_c,cur_film_apply_c);
				#endif
				pre_p0m_ipc_ctrl_00->cr_film_apply_y = cur_film_apply_y;
				pre_p0m_ipc_ctrl_00->cr_film_apply_c = cur_film_apply_c;
			}
			if(_g_dnt_hw_m14_trace)
			{
				if(count>PE_PRINT_COUNT_NUM)
				{
					PE_DNT_HW_M14_DBG_PRINT("[state][inf]frz:%d,mute:%d,src:%d,dtv:%d\n",\
						ctrl0_info->is_src_frz, ctrl0_info->mute_on,\
						disp0_info->src_type, disp0_info->dtv_type);
					PE_DNT_HW_M14_DBG_PRINT("[state][inf]fmt:%d,scan:%d,out:%d,game:%d\n",\
						disp0_info->fmt_type, disp0_info->scan_type,\
						disp0_info->out_type, pre_p0m_ipc_ctrl_01->cr_game_mode);
					PE_DNT_HW_M14_DBG_PRINT("[state][clc]flt:%d,det:%d [fmd]y:%d,c:%d\n",\
						cur_clc_flt_en, cur_clc_det_en, cur_film_apply_y, cur_film_apply_c);
					count=0;
				}
				count++;
			}
			#endif
		}
		else
		{
			PE_DNT_HW_M14_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * set vfy(v filter y) apply
 *
 * @param   *pCfg [in] PE_TSK_M14_CFG_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_DNT_HW_M14B_SetVfyApply(PE_TSK_M14_CFG_T *pCfg)
{
	int ret = RET_OK;
	#ifdef PE_HW_M14B_USE_REG_CTRL
	static UINT32 count = 0;
	UINT32 reg_data0 = 0x0, reg_data2 = 0x0;
	UINT32 apply_en_protect = PE_DNT_HW_M14_VFY_EN_PROTECT_OFF;
	UINT32 apply_min_max_protect = PE_DNT_HW_M14_VFY_MIN_MAX_PT_OFF;
	LX_PE_INF_DISPLAY_T *disp0_info = &pCfg->inf.disp_info[LX_PE_WIN_0];
	PE_DNT_HW_M14_SETTINGS_T *pInfo = &_g_pe_dnt_hw_m14_info;
	PE_M14B0_P0D_VFILTER_CTRL_0_T *cur_p0d_vfilter_ctrl_0 = NULL;		//m14b
	PE_M14B0_P0D_VFILTER_CTRL_0_T *reg_p0d_vfilter_ctrl_0 = NULL;		//m14b
	PE_M14B0_P0D_VFILTER_CTRL_2_T *cur_p0d_vfilter_ctrl_2 = NULL;		//m14b
	PE_M14B0_P0D_VFILTER_CTRL_2_T *reg_p0d_vfilter_ctrl_2 = NULL;		//m14b
	do{
		CHECK_KNULL(pCfg);
		if(PE_KDRV_VER_M14BX)
		{
			/************************************/
			/* vfilter enable protection        */
			/************************************/
			cur_p0d_vfilter_ctrl_0 = (PE_M14B0_P0D_VFILTER_CTRL_0_T *)&(pInfo->cur_p0d_vfy_ctrl_00);
			reg_p0d_vfilter_ctrl_0 = (PE_M14B0_P0D_VFILTER_CTRL_0_T *)&(reg_data0);
			/** check vfilter enable protection(off) case
			*	- except for fmt:hd, scan:i, src:dtv,comp,hdmi */
			if(disp0_info->fmt_type==LX_PE_FMT_HD && \
				disp0_info->scan_type==LX_PE_SCAN_INTERLACE && \
				(disp0_info->src_type==LX_PE_SRC_DTV || \
				disp0_info->src_type==LX_PE_SRC_COMP || \
				disp0_info->src_type==LX_PE_SRC_HDMI))
			{
				/* f/w may deal with enable, so keep reg setting */
				apply_en_protect = PE_DNT_HW_M14_VFY_EN_PROTECT_OFF;
			}
			#ifdef PE_DNT_HW_M14B_APPLY_VFLT_Y_ON_DTV_1080P
			else if(disp0_info->src_type == LX_PE_SRC_DTV && \
					(disp0_info->dtv_type == LX_PE_DTV_NORMAL || \
					disp0_info->dtv_type == LX_PE_DTV_HDDPLAY) && \
					disp0_info->fmt_type == LX_PE_FMT_HD && \
					disp0_info->in_h_size > 1900 && \
					disp0_info->in_v_size > 1000 && \
					disp0_info->fr_type == LX_PE_FR_50HZ)
			{
				/* f/w may deal with enable, so keep reg setting */
				apply_en_protect = PE_DNT_HW_M14_VFY_EN_PROTECT_OFF;
			}
			#endif
			else
			{
				/* protection on */
				apply_en_protect = PE_DNT_HW_M14_VFT_EN_PROTECT__ON;
				/* read current p0d reg. */
				PE_P0D_M14B0_RdFL(p0d_vfilter_ctrl_0);
				reg_data0 = PE_P0D_M14B0_Rd(p0d_vfilter_ctrl_0);
			}
			/* compare cur reg vs. default setting */
			if (apply_en_protect == PE_DNT_HW_M14_VFT_EN_PROTECT__ON && \
				(reg_p0d_vfilter_ctrl_0->cr_vfilter_enable != \
				cur_p0d_vfilter_ctrl_0->cr_vfilter_enable))
			{
				PE_DNT_HW_M14_DBG_PRINT("[inf]fmt:%d,sc:%d,src:%d\n", \
					disp0_info->fmt_type, disp0_info->scan_type, disp0_info->src_type);
				PE_DNT_HW_M14_DBG_PRINT("[set]vfy0 reg:0x%08x->0x%08x\n", \
					reg_data0, pInfo->cur_p0d_vfy_ctrl_00);
				/* set reg. and udpate mask */
				PE_DNT_HW_M14B_P0D_WR01(p0d_vfilter_ctrl_0, \
					cr_vfilter_enable, cur_p0d_vfilter_ctrl_0->cr_vfilter_enable);
			}
			/************************************/
			/* vfilter hmc min,max protection   */
			/************************************/
			cur_p0d_vfilter_ctrl_2 = (PE_M14B0_P0D_VFILTER_CTRL_2_T *)&(pInfo->cur_p0d_vfy_ctrl_02);
			reg_p0d_vfilter_ctrl_2 = (PE_M14B0_P0D_VFILTER_CTRL_2_T *)&(reg_data2);
			#ifdef PE_DNT_HW_M14B_FORCE_VFY_MIN_MAX_PT
			/* protection on */
			apply_min_max_protect = PE_DNT_HW_M14_VFT_MIN_MAX_PT__ON;
			/* read current p0d reg. */
			PE_P0D_M14B0_RdFL(p0d_vfilter_ctrl_2);
			reg_data2 = PE_P0D_M14B0_Rd(p0d_vfilter_ctrl_2);
			#else
			/** check vfilter hmc min,max protection(off) case
			*	- except for src:atv,cvbs,scart, cstd:nt,pal */
			if((disp0_info->src_type==LX_PE_SRC_ATV || \
				disp0_info->src_type==LX_PE_SRC_CVBS || \
				disp0_info->src_type==LX_PE_SRC_SCART) && \
				(disp0_info->cstd_type==LX_PE_CSTD_NTSC || \
				disp0_info->cstd_type==LX_PE_CSTD_PAL))
			{
				/* f/w may deal with enable, so keep reg setting */
				apply_min_max_protect = PE_DNT_HW_M14_VFY_MIN_MAX_PT_OFF;
			}
			else
			{
				/* protection on */
				apply_min_max_protect = PE_DNT_HW_M14_VFT_MIN_MAX_PT__ON;
				/* read current p0d reg. */
				PE_P0D_M14B0_RdFL(p0d_vfilter_ctrl_2);
				reg_data2 = PE_P0D_M14B0_Rd(p0d_vfilter_ctrl_2);
			}
			#endif
			/* compare cur reg vs. default setting */
			if (apply_min_max_protect == PE_DNT_HW_M14_VFT_MIN_MAX_PT__ON && \
				(reg_p0d_vfilter_ctrl_2->cr_hmc_protect_th_max != \
				cur_p0d_vfilter_ctrl_2->cr_hmc_protect_th_max || \
				reg_p0d_vfilter_ctrl_2->cr_hmc_protect_th_min != \
				cur_p0d_vfilter_ctrl_2->cr_hmc_protect_th_min))
			{
				PE_DNT_HW_M14_DBG_PRINT("[inf]src:%d,cstd:%d\n", \
					disp0_info->src_type, disp0_info->cstd_type);
				PE_DNT_HW_M14_DBG_PRINT("[set]vfy2 reg:0x%08x->0x%08x\n", \
					reg_data2, pInfo->cur_p0d_vfy_ctrl_02);
				/* set reg. and udpate mask */
				PE_DNT_HW_M14B_P0D_WR02(p0d_vfilter_ctrl_2, \
					cr_hmc_protect_th_max, cur_p0d_vfilter_ctrl_2->cr_hmc_protect_th_max, \
					cr_hmc_protect_th_min, cur_p0d_vfilter_ctrl_2->cr_hmc_protect_th_min);
			}
			/************************************/
			if(_g_dnt_hw_m14_trace)
			{
				if(count>PE_PRINT_COUNT_NUM)
				{
					PE_DNT_HW_M14_DBG_PRINT("[state]fmt:%d,sc:%d,src:%d,cstd:%d,dtv,i_h,v,fr:%d,%d,%d,%d\n", \
						disp0_info->fmt_type, disp0_info->scan_type, disp0_info->src_type, \
						disp0_info->cstd_type, disp0_info->dtv_type, disp0_info->in_h_size, \
						disp0_info->in_v_size, disp0_info->fr_type);
					if (apply_en_protect == PE_DNT_HW_M14_VFT_EN_PROTECT__ON)
					{
						PE_DNT_HW_M14_DBG_PRINT("[state]prot_on:vfy0 reg:0x%08x\n", \
							reg_data0);
					}
					if (apply_min_max_protect == PE_DNT_HW_M14_VFT_MIN_MAX_PT__ON)
					{
						PE_DNT_HW_M14_DBG_PRINT("[state]prot_on:vfy2 reg:0x%08x\n", \
							reg_data2);
					}
					count=0;
				}
				count++;
			}
		}
		else
		{
			PE_DNT_HW_M14_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	#else
	PE_DNT_HW_M14_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
	#endif
	return ret;
}
/**
 * set clc apply
 *
 * @param   *pCfg [in] PE_TSK_M14_CFG_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_DNT_HW_M14A_SetClcApply(PE_TSK_M14_CFG_T *pCfg)
{
	int ret = RET_OK;
	static UINT32 count = 0;
	UINT32 cur_clc_flt_en,cur_clc_det_en;
	PE_INF_M14_CTRL_T *ctrl0_info = &pCfg->inf.ctrl_info[LX_PE_WIN_0];
	LX_PE_INF_DISPLAY_T *disp0_info = &pCfg->inf.disp_info[LX_PE_WIN_0];
	PE_DNT_HW_M14_SETTINGS_T *pInfo = &_g_pe_dnt_hw_m14_info;
	PE_M14A0_P0L_CLC_CTRL_00_T *cur_p0l_clc_ctrl_00 = NULL;		//m14a
	PE_M14A0_P0L_CLC_CTRL_00_T *pre_p0l_clc_ctrl_00 = NULL;		//m14a
	PE_M14A0_P0L_IPC_CTRL_01_T *pre_p0l_ipc_ctrl_01 = NULL;		//m14a
	#ifdef PE_DNT_HW_M14A_SET_GAME_MODE_BY_FW
	PE_M14A0_P0R_IPC_CTRL_01_T *pre_p0r_ipc_ctrl_01 = NULL;
	PE_M14A0_P0L_IPC_CTRL_01_T *cur_p0l_ipc_ctrl_01 = NULL;
	PE_M14A0_P0R_IPC_CTRL_01_T *cur_p0r_ipc_ctrl_01 = NULL;
	PE_M14A0_P0L_TNR_CTRL_10_T *pre_p0l_tnr_ctrl_10 = NULL;
	PE_M14A0_P0R_TNR_CTRL_10_T *pre_p0r_tnr_ctrl_10 = NULL;
	UINT32 l_data, r_data, dbg_map_en0, dbg_map_en1;
	#endif
	do{
		CHECK_KNULL(pCfg);
		if(PE_KDRV_VER_M14AX)
		{
			pre_p0l_ipc_ctrl_01 = (PE_M14A0_P0L_IPC_CTRL_01_T *)&(pInfo->pre_p0l_ipc_ctrl_01);
			cur_p0l_clc_ctrl_00 = (PE_M14A0_P0L_CLC_CTRL_00_T *)&(pInfo->cur_p0l_clc_ctrl_00);
			pre_p0l_clc_ctrl_00 = (PE_M14A0_P0L_CLC_CTRL_00_T *)&(pInfo->pre_p0l_clc_ctrl_00);
			#ifdef PE_DNT_HW_M14A_SET_GAME_MODE_BY_FW
			pre_p0r_ipc_ctrl_01 = (PE_M14A0_P0R_IPC_CTRL_01_T *)&(pInfo->pre_p0r_ipc_ctrl_01);
			cur_p0l_ipc_ctrl_01 = (PE_M14A0_P0L_IPC_CTRL_01_T *)&(l_data);
			cur_p0r_ipc_ctrl_01 = (PE_M14A0_P0R_IPC_CTRL_01_T *)&(r_data);
			pre_p0l_tnr_ctrl_10 = (PE_M14A0_P0L_TNR_CTRL_10_T *)&(pInfo->pre_p0l_tnr_ctrl_10);
			pre_p0r_tnr_ctrl_10 = (PE_M14A0_P0R_TNR_CTRL_10_T *)&(pInfo->pre_p0r_tnr_ctrl_10);
			/* read current p0l reg. */
			PE_P0L_M14A0_RdFL(p0l_ipc_ctrl_01);
			l_data = PE_P0L_M14A0_Rd(p0l_ipc_ctrl_01);
			/* read current p0r reg. */
			PE_P0R_M14A0_RdFL(p0r_ipc_ctrl_01);
			r_data = PE_P0R_M14A0_Rd(p0r_ipc_ctrl_01);
			/* compare p0l reg. */
			if (cur_p0l_ipc_ctrl_01->cr_st_flt_enable != pre_p0l_ipc_ctrl_01->cr_st_flt_enable || \
				cur_p0l_ipc_ctrl_01->cr_st_2d_v_flt_en != pre_p0l_ipc_ctrl_01->cr_st_2d_v_flt_en || \
				cur_p0l_ipc_ctrl_01->cr_hmc_flt_enable != pre_p0l_ipc_ctrl_01->cr_hmc_flt_enable || \
				cur_p0l_ipc_ctrl_01->cr_st_2d_v_flt_range != pre_p0l_ipc_ctrl_01->cr_st_2d_v_flt_range || \
				cur_p0l_ipc_ctrl_01->cr_black_mask != pre_p0l_ipc_ctrl_01->cr_black_mask)
			{
				PE_DNT_HW_M14_DBG_PRINT("[set]p0l ipc01 reg:0x%08x->0x%08x, game:%d\n",\
					l_data, pInfo->pre_p0l_ipc_ctrl_01, cur_p0l_ipc_ctrl_01->cr_game_mode);
				/* not control cr_game_mode, cr_5f_mode */
				cur_p0l_ipc_ctrl_01->cr_st_flt_enable = pre_p0l_ipc_ctrl_01->cr_st_flt_enable;
				cur_p0l_ipc_ctrl_01->cr_st_2d_v_flt_en = pre_p0l_ipc_ctrl_01->cr_st_2d_v_flt_en;
				cur_p0l_ipc_ctrl_01->cr_hmc_flt_enable = pre_p0l_ipc_ctrl_01->cr_hmc_flt_enable;
				cur_p0l_ipc_ctrl_01->cr_st_2d_v_flt_range = pre_p0l_ipc_ctrl_01->cr_st_2d_v_flt_range;
				cur_p0l_ipc_ctrl_01->cr_black_mask = pre_p0l_ipc_ctrl_01->cr_black_mask;
				PE_P0L_M14A0_Wr(p0l_ipc_ctrl_01, l_data);
				PE_P0L_M14A0_WrFL(p0l_ipc_ctrl_01);
			}
			/* compare p0r reg. */
			if (cur_p0r_ipc_ctrl_01->cr_st_flt_enable != pre_p0r_ipc_ctrl_01->cr_st_flt_enable || \
				cur_p0r_ipc_ctrl_01->cr_st_2d_v_flt_en != pre_p0r_ipc_ctrl_01->cr_st_2d_v_flt_en || \
				cur_p0r_ipc_ctrl_01->cr_hmc_flt_enable != pre_p0r_ipc_ctrl_01->cr_hmc_flt_enable || \
				cur_p0r_ipc_ctrl_01->cr_st_2d_v_flt_range != pre_p0r_ipc_ctrl_01->cr_st_2d_v_flt_range || \
				cur_p0r_ipc_ctrl_01->cr_black_mask != pre_p0r_ipc_ctrl_01->cr_black_mask)
			{
				PE_DNT_HW_M14_DBG_PRINT("[set]p0r ipc01 reg:0x%08x->0x%08x, game:%d\n",\
					r_data, pInfo->pre_p0r_ipc_ctrl_01, cur_p0r_ipc_ctrl_01->cr_game_mode);
				/* not control cr_game_mode, cr_5f_mode */
				cur_p0r_ipc_ctrl_01->cr_st_flt_enable = pre_p0r_ipc_ctrl_01->cr_st_flt_enable;
				cur_p0r_ipc_ctrl_01->cr_st_2d_v_flt_en = pre_p0r_ipc_ctrl_01->cr_st_2d_v_flt_en;
				cur_p0r_ipc_ctrl_01->cr_hmc_flt_enable = pre_p0r_ipc_ctrl_01->cr_hmc_flt_enable;
				cur_p0r_ipc_ctrl_01->cr_st_2d_v_flt_range = pre_p0r_ipc_ctrl_01->cr_st_2d_v_flt_range;
				cur_p0r_ipc_ctrl_01->cr_black_mask = pre_p0r_ipc_ctrl_01->cr_black_mask;
				PE_P0R_M14A0_Wr(p0r_ipc_ctrl_01, r_data);
				PE_P0R_M14A0_WrFL(p0r_ipc_ctrl_01);
			}
			/* L : check tnr debug i on,off based on game mode */
			dbg_map_en0 = (cur_p0l_ipc_ctrl_01->cr_game_mode)? \
				PE_DNT_HW_M14_TNR_DBG__ON:PE_DNT_HW_M14_TNR_DBG_OFF;
			if (pre_p0l_tnr_ctrl_10->tnr_debug_map_en_for_i != dbg_map_en0)
			{
				PE_DNT_HW_M14_DBG_PRINT("[set][tnr]game:%d, dbg_map_en0:%d->%d\n",\
					cur_p0l_ipc_ctrl_01->cr_game_mode, \
					pre_p0l_tnr_ctrl_10->tnr_debug_map_en_for_i, dbg_map_en0);
				PE_P0L_M14A0_RdFL(p0l_tnr_ctrl_10);
				PE_P0L_M14A0_Wr01(p0l_tnr_ctrl_10, tnr_black_line_en_for_i,	0x0);
				PE_P0L_M14A0_Wr01(p0l_tnr_ctrl_10, tnr_debug_map_en_for_i,	dbg_map_en0);
				PE_P0L_M14A0_WrFL(p0l_tnr_ctrl_10);
				pre_p0l_tnr_ctrl_10->tnr_debug_map_en_for_i = dbg_map_en0;
			}
			/* R : check tnr debug i on,off based on game mode */
			dbg_map_en1 = (cur_p0r_ipc_ctrl_01->cr_game_mode)? \
				PE_DNT_HW_M14_TNR_DBG__ON:PE_DNT_HW_M14_TNR_DBG_OFF;
			if (pre_p0r_tnr_ctrl_10->tnr_debug_map_en_for_i != dbg_map_en1)
			{
				PE_DNT_HW_M14_DBG_PRINT("[set][tnr]game:%d, dbg_map_en1:%d->%d\n",\
					cur_p0r_ipc_ctrl_01->cr_game_mode, \
					pre_p0r_tnr_ctrl_10->tnr_debug_map_en_for_i, dbg_map_en1);
				PE_P0R_M14A0_RdFL(p0r_tnr_ctrl_10);
				PE_P0R_M14A0_Wr01(p0r_tnr_ctrl_10, tnr_black_line_en_for_i,	0x0);
				PE_P0R_M14A0_Wr01(p0r_tnr_ctrl_10, tnr_debug_map_en_for_i,	dbg_map_en1);
				PE_P0R_M14A0_WrFL(p0r_tnr_ctrl_10);
				pre_p0r_tnr_ctrl_10->tnr_debug_map_en_for_i = dbg_map_en1;
			}
			/* check ipc,clc off case */
			if (ctrl0_info->is_src_frz && !ctrl0_info->mute_on && \
				disp0_info->src_type==LX_PE_SRC_DTV && \
				disp0_info->dtv_type==LX_PE_DTV_FILEPLAY && \
				disp0_info->fmt_type==LX_PE_FMT_HD && \
				disp0_info->scan_type==LX_PE_SCAN_INTERLACE)
			{
				cur_clc_flt_en = PE_DNT_HW_M14_CLC_OFF;		//[1]
				cur_clc_det_en = PE_DNT_HW_M14_CLC_OFF;		//[2]
			}
			else
			{
				/* clc off when current game mode on(set by fw) */
				if(cur_p0l_ipc_ctrl_01->cr_game_mode)
				{
					cur_clc_flt_en = PE_DNT_HW_M14_CLC_OFF;		//[1]
					cur_clc_det_en = PE_DNT_HW_M14_CLC_OFF;		//[2]
				}
				else
				{
					cur_clc_flt_en = cur_p0l_clc_ctrl_00->cl_filter_enable;
					cur_clc_det_en = cur_p0l_clc_ctrl_00->clc_detection_enable;
				}
			}
			if (pre_p0l_clc_ctrl_00->cl_filter_enable != cur_clc_flt_en || \
				pre_p0l_clc_ctrl_00->clc_detection_enable != cur_clc_det_en)
			{
				PE_DNT_HW_M14_DBG_PRINT("[inf][clc]frz:%d,mute:%d,src:%d,dtv:%d\n",\
					ctrl0_info->is_src_frz, ctrl0_info->mute_on,\
					disp0_info->src_type, disp0_info->dtv_type);
				PE_DNT_HW_M14_DBG_PRINT("[inf][clc]fmt:%d,scan:%d,out:%d,game:%d\n",\
					disp0_info->fmt_type, disp0_info->scan_type,\
					disp0_info->out_type, pre_p0l_ipc_ctrl_01->cr_game_mode);
				PE_DNT_HW_M14_DBG_PRINT("[set][clc]flt:%d->%d, det:%d->%d\n",\
					pre_p0l_clc_ctrl_00->cl_filter_enable, cur_clc_flt_en,\
					pre_p0l_clc_ctrl_00->clc_detection_enable, cur_clc_det_en);
				PE_P0L_M14A0_RdFL(p0l_clc_ctrl_00);
				PE_P0L_M14A0_Wr01(p0l_clc_ctrl_00, cl_filter_enable,		cur_clc_flt_en);
				PE_P0L_M14A0_Wr01(p0l_clc_ctrl_00, clc_detection_enable,	cur_clc_det_en);
				PE_P0L_M14A0_WrFL(p0l_clc_ctrl_00);
				pre_p0l_clc_ctrl_00->cl_filter_enable = cur_clc_flt_en;
				pre_p0l_clc_ctrl_00->clc_detection_enable = cur_clc_det_en;
			}
			if(_g_dnt_hw_m14_trace)
			{
				if(count>PE_PRINT_COUNT_NUM)
				{
					PE_DNT_HW_M14_DBG_PRINT("[state][inf]frz:%d,mute:%d,src:%d,dtv:%d\n",\
						ctrl0_info->is_src_frz, ctrl0_info->mute_on,\
						disp0_info->src_type, disp0_info->dtv_type);
					PE_DNT_HW_M14_DBG_PRINT("[state][inf]fmt:%d,scan:%d,out:%d,game:%d\n",\
						disp0_info->fmt_type, disp0_info->scan_type,\
						disp0_info->out_type, pre_p0l_ipc_ctrl_01->cr_game_mode);
					PE_DNT_HW_M14_DBG_PRINT("[state][clc]flt:%d,det:%d,[tnr]dbg0,1:%d,%d\n",\
						cur_clc_flt_en, cur_clc_det_en, dbg_map_en0, dbg_map_en1);
					count=0;
				}
				count++;
			}
			#else
			/* check clc off case */
			if(ctrl0_info->is_src_frz && !ctrl0_info->mute_on && \
				disp0_info->src_type==LX_PE_SRC_DTV && \
				disp0_info->dtv_type==LX_PE_DTV_FILEPLAY && \
				disp0_info->fmt_type==LX_PE_FMT_HD && \
				disp0_info->scan_type==LX_PE_SCAN_INTERLACE)
			{
				cur_clc_flt_en = PE_DNT_HW_M14_CLC_OFF;		//[1]
				cur_clc_det_en = PE_DNT_HW_M14_CLC_OFF;		//[2]
			}
			else
			{
				/* clc off when game mode on */
				if (pre_p0l_ipc_ctrl_01->cr_game_mode)
				{
					cur_clc_flt_en = PE_DNT_HW_M14_CLC_OFF;		//[1]
					cur_clc_det_en = PE_DNT_HW_M14_CLC_OFF;		//[2]
				}
				else
				{
					cur_clc_flt_en = cur_p0l_clc_ctrl_00->cl_filter_enable;
					cur_clc_det_en = cur_p0l_clc_ctrl_00->clc_detection_enable;
				}
			}
			if (pre_p0l_clc_ctrl_00->cl_filter_enable != cur_clc_flt_en || \
				pre_p0l_clc_ctrl_00->clc_detection_enable != cur_clc_det_en)
			{
				PE_DNT_HW_M14_DBG_PRINT("[inf][clc]frz:%d,mute:%d,src:%d,dtv:%d\n",\
					ctrl0_info->is_src_frz, ctrl0_info->mute_on,\
					disp0_info->src_type, disp0_info->dtv_type);
				PE_DNT_HW_M14_DBG_PRINT("[inf][clc]fmt:%d,scan:%d,out:%d,game:%d\n",\
					disp0_info->fmt_type, disp0_info->scan_type,\
					disp0_info->out_type, pre_p0l_ipc_ctrl_01->cr_game_mode);
				PE_DNT_HW_M14_DBG_PRINT("[set][clc]flt:%d->%d, det:%d->%d\n",\
					pre_p0l_clc_ctrl_00->cl_filter_enable, cur_clc_flt_en,\
					pre_p0l_clc_ctrl_00->clc_detection_enable, cur_clc_det_en);
				PE_P0L_M14A0_RdFL(p0l_clc_ctrl_00);
				PE_P0L_M14A0_Wr01(p0l_clc_ctrl_00,  cl_filter_enable, 		cur_clc_flt_en);
				PE_P0L_M14A0_Wr01(p0l_clc_ctrl_00,  clc_detection_enable,	cur_clc_det_en);
				PE_P0L_M14A0_WrFL(p0l_clc_ctrl_00);
				pre_p0l_clc_ctrl_00->cl_filter_enable = cur_clc_flt_en;
				pre_p0l_clc_ctrl_00->clc_detection_enable = cur_clc_det_en;
			}
			if(_g_dnt_hw_m14_trace)
			{
				if(count>PE_PRINT_COUNT_NUM)
				{
					PE_DNT_HW_M14_DBG_PRINT("[state][inf]frz:%d,mute:%d,src:%d,dtv:%d\n",\
						ctrl0_info->is_src_frz, ctrl0_info->mute_on,\
						disp0_info->src_type, disp0_info->dtv_type);
					PE_DNT_HW_M14_DBG_PRINT("[state][inf]fmt:%d,scan:%d,out:%d,game:%d\n",\
						disp0_info->fmt_type, disp0_info->scan_type,\
						disp0_info->out_type, pre_p0l_ipc_ctrl_01->cr_game_mode);
					PE_DNT_HW_M14_DBG_PRINT("[state][clc]flt:%d, det:%d\n",\
						cur_clc_flt_en, cur_clc_det_en);
					count=0;
				}
				count++;
			}
			#endif
		}
		else
		{
			PE_DNT_HW_M14_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}

/**
 * get current dnt settings
 *
 * @param   *pstParams [in/out] PE_DNT_HW_M14_SETTINGS_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_DNT_HW_M14_GetCurDntSettings(PE_DNT_HW_M14_SETTINGS_T *pstParams)
{
	int ret = RET_OK;
	PE_DNT_HW_M14_SETTINGS_T *pInfo = &_g_pe_dnt_hw_m14_info;
	do{

		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_M14BX)
		{
			memcpy(pstParams,pInfo,sizeof(PE_DNT_HW_M14_SETTINGS_T));
			PE_DNT_HW_M14_DBG_PRINT("get ipc0(cp):0x%08x,0x%08x, "\
				"ipc1:0x%08x, clc0(cp):0x%08x,0x%08x, vfy0:0x%08x\n", \
				pInfo->cur_p0m_ipc_ctrl_00,	pInfo->pre_p0m_ipc_ctrl_00, \
				pInfo->pre_p0m_ipc_ctrl_01, pInfo->cur_p0m_clc_ctrl_00,	\
				pInfo->pre_p0m_clc_ctrl_00, pInfo->cur_p0d_vfy_ctrl_00);
		}
		else if(PE_KDRV_VER_M14AX)
		{
			memcpy(pstParams,pInfo,sizeof(PE_DNT_HW_M14_SETTINGS_T));
			PE_DNT_HW_M14_DBG_PRINT("get ipc1(lr):0x%08x,0x%08x, "\
				"clc0(cp):0x%08x,0x%08x, tnr10(lr):0x%08x,0x%08x\n", \
				pInfo->pre_p0l_ipc_ctrl_01, pInfo->pre_p0r_ipc_ctrl_01, \
				pInfo->cur_p0l_clc_ctrl_00, pInfo->pre_p0l_clc_ctrl_00, \
				pInfo->pre_p0l_tnr_ctrl_10, pInfo->pre_p0r_tnr_ctrl_10);
		}
		else
		{
			PE_DNT_HW_M14_DBG_PRINT("nothing to do.\n");
		}
	}while(0);
	return ret;
}
/**
 * read default setting
 *
 * @param   *pstParams [in/out] LX_PE_ETC_TBL_T
 * @return  OK if success, ERROR otherwise.
 * @see     
 * @author
 */
int PE_DNT_HW_M14_RdDefault(LX_PE_ETC_TBL_T *pstParams)
{
	int ret = RET_OK;
	char buffer[PE_TRACE_STR_SIZE];
	UINT32 param_size=0, table_size=0, is_valid=0;
	static UINT32 rd_cnt=0;
	PE_REG_PARAM_T param;
	PE_DNT_HW_M14_SETTINGS_T *pInfo=&_g_pe_dnt_hw_m14_info;
	#ifdef PE_HW_M14B_USE_REG_CTRL
	UINT32 *p_data = NULL;
	PE_DNT_HW_M14B_P0M_REG_CTRL_T *p0m_reg_ctrl = &_g_pe_dnt_hw_m14b_p0m_reg_ctrl;
	PE_DNT_HW_M14B_P0M_REG_MATCH_T *p0m_reg_match = &_g_pe_dnt_hw_m14b_p0m_reg_match;
	PE_DNT_HW_M14B_P0D_REG_CTRL_T *p0d_reg_ctrl = &_g_pe_dnt_hw_m14b_p0d_reg_ctrl;
	PE_DNT_HW_M14B_P0D_REG_MATCH_T *p0d_reg_match = &_g_pe_dnt_hw_m14b_p0d_reg_match;
	#endif

#define PE_DNT_M14_NO_PRINT			0
#define PE_DNT_M14_RD_N_HEX_PRINT	1
#define PE_DNT_M14_HEX_PRINT_ONLY	2
#define PE_DNT_M14_DEC_PRINT_ONLY	3
#define PE_DNT_M14_PRINT_START		"START_OF_PRINT"
#define PE_DNT_M14_PRINT_END		"END_OF_PRINT"
#define PE_DNT_M14_PRINT_EXIT		"EXIT_OF_PRINT"
#define PE_DNT_M14_PRINT_RESRV		"RESERVED"
#define PE_DNT_M14_CASE_PRMENU(_cnt, _action, _buf, _item)	\
	case (_cnt):sprintf(_buf, "   [0x%02x] %s\n", (_item), #_item);_action;break
#define PE_DNT_M14_CASE_SPRINT(_cnt, _action, _buf, fmt, args...)	\
	case (_cnt):sprintf(_buf, fmt, ##args);_action;break
#define PE_DNT_M14_DFLT_SPRINT(_action, _buf, fmt, args...)		\
	default:sprintf(_buf, fmt, ##args);_action;break

	do{
		CHECK_KNULL(pstParams);
		CHECK_KNULL(pstParams->data);
		if (PE_KDRV_VER_M14BX)
		{
			if (pstParams->data[0]>=PE_DNT_HW_M14B_RD_DFLT_MAX)
			{
				sprintf(buffer, "%s", PE_DNT_M14_PRINT_EXIT);
				rd_cnt=0;
			}
			else if (pstParams->data[0]==PE_DNT_HW_M14B_RD_DFLT_DISPLAY)
			{
				switch (rd_cnt)
				{
					PE_DNT_M14_CASE_SPRINT( 0, rd_cnt++, buffer, "%s", PE_DNT_M14_PRINT_START);
					PE_DNT_M14_CASE_SPRINT( 1, rd_cnt++, buffer, "   ** current default **\n");
					PE_DNT_M14_CASE_SPRINT( 2, rd_cnt++, buffer, \
						"   ipc0b_fmt : %s\n", PE_DNT_HW_M14B_IPC_FMT_TO_STR(pInfo->ipc0b_fmt));
					PE_DNT_M14_CASE_SPRINT( 3, rd_cnt++, buffer, \
						"   ipc1b_fmt : %s\n", PE_DNT_HW_M14B_IPC_FMT_TO_STR(pInfo->ipc1b_fmt));
					PE_DNT_M14_CASE_SPRINT( 4, rd_cnt++, buffer, \
						"   clc0b_fmt : %s\n", PE_DNT_HW_M14B_CLC_FMT_TO_STR(pInfo->clc0b_fmt));
					PE_DNT_M14_CASE_SPRINT( 5, rd_cnt++, buffer, \
						"   vfy0b_fmt : %s\n", PE_DNT_HW_M14B_VFY_FMT_TO_STR(pInfo->vfy0b_fmt));
					PE_DNT_M14_CASE_SPRINT( 6, rd_cnt++, buffer, "   *********************\n");
					PE_DNT_M14_CASE_SPRINT( 7, rd_cnt++, buffer, PE_DNT_M14_PRINT_RESRV);
					PE_DNT_M14_CASE_SPRINT( 8, rd_cnt++, buffer, PE_DNT_M14_PRINT_RESRV);
					PE_DNT_M14_CASE_SPRINT( 9, rd_cnt++, buffer, PE_DNT_M14_PRINT_RESRV);
					PE_DNT_M14_CASE_SPRINT(10, rd_cnt++, buffer, PE_DNT_M14_PRINT_RESRV);
					PE_DNT_M14_CASE_PRMENU(11, rd_cnt++, buffer, PE_DNT_HW_M14B_RD_DFLT_IPC_M);
					PE_DNT_M14_CASE_PRMENU(12, rd_cnt++, buffer, PE_DNT_HW_M14B_RD_DFLT_IPC_S);
					PE_DNT_M14_CASE_PRMENU(13, rd_cnt++, buffer, PE_DNT_HW_M14B_RD_DFLT_CLC);
					PE_DNT_M14_CASE_PRMENU(14, rd_cnt++, buffer, PE_DNT_HW_M14B_RD_DFLT_VFY);
					PE_DNT_M14_CASE_SPRINT(15, rd_cnt++, buffer, PE_DNT_M14_PRINT_RESRV);
					PE_DNT_M14_CASE_SPRINT(16, rd_cnt++, buffer, PE_DNT_M14_PRINT_RESRV);
					PE_DNT_M14_CASE_SPRINT(17, rd_cnt++, buffer, PE_DNT_M14_PRINT_RESRV);
					PE_DNT_M14_CASE_SPRINT(18, rd_cnt++, buffer, PE_DNT_M14_PRINT_RESRV);
					PE_DNT_M14_CASE_SPRINT(19, rd_cnt++, buffer, PE_DNT_M14_PRINT_RESRV);
					#ifdef PE_HW_M14B_USE_REG_CTRL
					PE_DNT_M14_CASE_PRMENU(20, rd_cnt++, buffer, PE_DNT_HW_M14B_RD_IPC_MNUM);
					PE_DNT_M14_CASE_PRMENU(21, rd_cnt++, buffer, PE_DNT_HW_M14B_RD_CLC_MNUM);
					PE_DNT_M14_CASE_PRMENU(22, rd_cnt++, buffer, PE_DNT_HW_M14B_RD_P0M_DATA);
					PE_DNT_M14_CASE_PRMENU(23, rd_cnt++, buffer, PE_DNT_HW_M14B_RD_P0M_MASK);
					PE_DNT_M14_CASE_PRMENU(24, rd_cnt++, buffer, PE_DNT_HW_M14B_RD_VFY_MNUM);
					PE_DNT_M14_CASE_PRMENU(25, rd_cnt++, buffer, PE_DNT_HW_M14B_RD_P0D_DATA);
					PE_DNT_M14_CASE_PRMENU(26, rd_cnt++, buffer, PE_DNT_HW_M14B_RD_P0D_MASK);
					#endif
					PE_DNT_M14_DFLT_SPRINT(rd_cnt=0, buffer, "%s", PE_DNT_M14_PRINT_END);
				}
			}
			else
			{
				is_valid = PE_DNT_M14_NO_PRINT;
				switch (pstParams->data[0])
				{
					case PE_DNT_HW_M14B_RD_DFLT_IPC_M:
					case PE_DNT_HW_M14B_RD_DFLT_IPC_S:
						table_size = sizeof(ipc_l_pc_default_m14b0);
						param_size = table_size/sizeof(PE_REG_PARAM_T);
						if (rd_cnt<param_size)
						{
							param.addr = ipc_l_pc_default_m14b0[rd_cnt].addr;
							if(pstParams->data[0]==PE_DNT_HW_M14B_RD_DFLT_IPC_S)
								param.addr += PE_PE0_REG_M14B_MSDIFF;
							is_valid = PE_DNT_M14_RD_N_HEX_PRINT;
						}
						break;
					case PE_DNT_HW_M14B_RD_DFLT_CLC:
						table_size = sizeof(clc_l_pc_default_m14b0);
						param_size = table_size/sizeof(PE_REG_PARAM_T);
						if (rd_cnt<param_size)
						{
							param.addr = clc_l_pc_default_m14b0[rd_cnt].addr;
							is_valid = PE_DNT_M14_RD_N_HEX_PRINT;
						}
						break;
					case PE_DNT_HW_M14B_RD_DFLT_VFY:
						table_size = sizeof(vflt_y_l_pc_default_m14b0);
						param_size = table_size/sizeof(PE_REG_PARAM_T);
						if (rd_cnt<param_size)
						{
							param.addr = vflt_y_l_pc_default_m14b0[rd_cnt].addr;
							is_valid = PE_DNT_M14_RD_N_HEX_PRINT;
						}
						break;
					#ifdef PE_HW_M14B_USE_REG_CTRL
					case PE_DNT_HW_M14B_RD_IPC_MNUM:
						table_size = p0m_reg_match->ipc_size;
						param_size = table_size/sizeof(PE_REG_PARAM_T);
						if (rd_cnt<param_size)
						{
							param.addr = p0m_reg_match->ipc_mtbl[rd_cnt].addr;
							param.data = p0m_reg_match->ipc_mtbl[rd_cnt].data;
							is_valid = PE_DNT_M14_DEC_PRINT_ONLY;
						}
						break;
					case PE_DNT_HW_M14B_RD_CLC_MNUM:
						table_size = p0m_reg_match->clc_size;
						param_size = table_size/sizeof(PE_REG_PARAM_T);
						if (rd_cnt<param_size)
						{
							param.addr = p0m_reg_match->clc_mtbl[rd_cnt].addr;
							param.data = p0m_reg_match->clc_mtbl[rd_cnt].data;
							is_valid = PE_DNT_M14_DEC_PRINT_ONLY;
						}
						break;
					case PE_DNT_HW_M14B_RD_P0M_DATA:
						if (rd_cnt<PE_DNT_HW_M14B_P0M_ENUM(MAX))
						{
							param.addr = p0m_reg_ctrl->addr[rd_cnt];
							p_data = (UINT32 *)&(p0m_reg_ctrl->data);
							param.data = p_data[rd_cnt];
							is_valid = PE_DNT_M14_HEX_PRINT_ONLY;
						}
						break;
					case PE_DNT_HW_M14B_RD_P0M_MASK:
						if (rd_cnt<PE_DNT_HW_M14B_P0M_ENUM(MAX))
						{
							param.addr = p0m_reg_ctrl->addr[rd_cnt];
							p_data = (UINT32 *)&(p0m_reg_ctrl->mask);
							param.data = p_data[rd_cnt];
							is_valid = PE_DNT_M14_HEX_PRINT_ONLY;
						}
						break;
					case PE_DNT_HW_M14B_RD_VFY_MNUM:
						table_size = p0d_reg_match->vfy_size;
						param_size = table_size/sizeof(PE_REG_PARAM_T);
						if (rd_cnt<param_size)
						{
							param.addr = p0d_reg_match->vfy_mtbl[rd_cnt].addr;
							param.data = p0d_reg_match->vfy_mtbl[rd_cnt].data;
							is_valid = PE_DNT_M14_DEC_PRINT_ONLY;
						}
						break;
					case PE_DNT_HW_M14B_RD_P0D_DATA:
						if (rd_cnt<PE_DNT_HW_M14B_P0D_ENUM(MAX))
						{
							param.addr = p0d_reg_ctrl->addr[rd_cnt];
							p_data = (UINT32 *)&(p0d_reg_ctrl->data);
							param.data = p_data[rd_cnt];
							is_valid = PE_DNT_M14_HEX_PRINT_ONLY;
						}
						break;
					case PE_DNT_HW_M14B_RD_P0D_MASK:
						if (rd_cnt<PE_DNT_HW_M14B_P0D_ENUM(MAX))
						{
							param.addr = p0d_reg_ctrl->addr[rd_cnt];
							p_data = (UINT32 *)&(p0d_reg_ctrl->mask);
							param.data = p_data[rd_cnt];
							is_valid = PE_DNT_M14_HEX_PRINT_ONLY;
						}
						break;
					#endif
					default:
						break;
				}
				if (is_valid == PE_DNT_M14_RD_N_HEX_PRINT)
				{
					param.data = PE_REG_M14_RD(param.addr);
					sprintf(buffer, "   [%04d] {0x%04X, 0x%08X},\n", \
						rd_cnt, param.addr, param.data);
					rd_cnt++;
				}
				else if (is_valid == PE_DNT_M14_HEX_PRINT_ONLY)
				{
					sprintf(buffer, "   [%04d] {0x%04X, 0x%08X},\n", \
						rd_cnt, param.addr, param.data);
					rd_cnt++;
				}
				else if (is_valid == PE_DNT_M14_DEC_PRINT_ONLY)
				{
					sprintf(buffer, "   [%04d] {0x%04X, %04d},\n", \
						rd_cnt, param.addr, param.data);
					rd_cnt++;
				}
				else
				{
					sprintf(buffer, "%s", PE_DNT_M14_PRINT_END);
					rd_cnt = 0;
				}
			}
			memcpy(pstParams->data, buffer, sizeof(char)*PE_TRACE_STR_SIZE);
		}
		else if (PE_KDRV_VER_M14AX)
		{
			if (pstParams->data[0]>=PE_DNT_HW_M14A_RD_DFLT_MAX)
			{
				sprintf(buffer, "%s", PE_DNT_M14_PRINT_EXIT);
				rd_cnt=0;
			}
			else if (pstParams->data[0]==PE_DNT_HW_M14A_RD_DFLT_DISPLAY)
			{
				switch (rd_cnt)
				{
					PE_DNT_M14_CASE_SPRINT( 0, rd_cnt++, buffer, "%s", PE_DNT_M14_PRINT_START);
					PE_DNT_M14_CASE_SPRINT( 1, rd_cnt++, buffer, "   ** current default **\n");
					PE_DNT_M14_CASE_SPRINT( 2, rd_cnt++, buffer, \
						"   ipc0a_fmt : %s\n", PE_DNT_HW_M14A_IPC_FMT_TO_STR(pInfo->ipc0a_fmt));
					PE_DNT_M14_CASE_SPRINT( 3, rd_cnt++, buffer, \
						"   ipc1a_fmt : %s\n", PE_DNT_HW_M14A_IPC_FMT_TO_STR(pInfo->ipc1a_fmt));
					PE_DNT_M14_CASE_SPRINT( 4, rd_cnt++, buffer, \
						"   clc0a_fmt : %s\n", PE_DNT_HW_M14A_CLC_FMT_TO_STR(pInfo->clc0a_fmt));
					PE_DNT_M14_CASE_SPRINT( 5, rd_cnt++, buffer, \
						"   vfy0a_fmt : %s\n", PE_DNT_HW_M14A_VFY_FMT_TO_STR(pInfo->vfy0a_fmt));
					PE_DNT_M14_CASE_SPRINT( 6, rd_cnt++, buffer, \
						"   vfy1a_fmt : %s\n", PE_DNT_HW_M14A_VFY_FMT_TO_STR(pInfo->vfy1a_fmt));
					PE_DNT_M14_CASE_SPRINT( 7, rd_cnt++, buffer, "   *********************\n");
					PE_DNT_M14_CASE_SPRINT( 8, rd_cnt++, buffer, PE_DNT_M14_PRINT_RESRV);
					PE_DNT_M14_CASE_SPRINT( 9, rd_cnt++, buffer, PE_DNT_M14_PRINT_RESRV);
					PE_DNT_M14_CASE_PRMENU(10, rd_cnt++, buffer, PE_DNT_HW_M14A_RD_DFLT_IPC_L);
					PE_DNT_M14_CASE_PRMENU(11, rd_cnt++, buffer, PE_DNT_HW_M14A_RD_DFLT_IPC_R);
					PE_DNT_M14_CASE_PRMENU(12, rd_cnt++, buffer, PE_DNT_HW_M14A_RD_DFLT_CLC_L);
					PE_DNT_M14_CASE_PRMENU(13, rd_cnt++, buffer, PE_DNT_HW_M14A_RD_DFLT_VFY_L);
					PE_DNT_M14_CASE_PRMENU(14, rd_cnt++, buffer, PE_DNT_HW_M14A_RD_DFLT_VFY_R);
					PE_DNT_M14_DFLT_SPRINT(rd_cnt=0, buffer, "%s", PE_DNT_M14_PRINT_END);
				}
			}
			else
			{
				is_valid = 0;
				switch (pstParams->data[0])
				{
					case PE_DNT_HW_M14A_RD_DFLT_IPC_L:
					case PE_DNT_HW_M14A_RD_DFLT_IPC_R:
						table_size = sizeof(ipc_l_pc_default_m14a0);
						param_size = table_size/sizeof(PE_REG_PARAM_T);
						if (rd_cnt<param_size)
						{
							param.addr = ipc_l_pc_default_m14a0[rd_cnt].addr;
							if(pstParams->data[0]==PE_DNT_HW_M14A_RD_DFLT_IPC_R)
								param.addr += PE_PE0_REG_M14_LRDIFF;
							is_valid = 1;
						}
						break;
					case PE_DNT_HW_M14A_RD_DFLT_CLC_L:
						table_size = sizeof(clc_l_pc_default_m14a0);
						param_size = table_size/sizeof(PE_REG_PARAM_T);
						if (rd_cnt<param_size)
						{
							param.addr = clc_l_pc_default_m14a0[rd_cnt].addr;
							is_valid = 1;
						}
						break;
					case PE_DNT_HW_M14A_RD_DFLT_VFY_L:
					case PE_DNT_HW_M14A_RD_DFLT_VFY_R:
					default:
						table_size = sizeof(vflt_y_l_pc_default_m14a0);
						param_size = table_size/sizeof(PE_REG_PARAM_T);
						if (rd_cnt<param_size)
						{
							param.addr = vflt_y_l_pc_default_m14a0[rd_cnt].addr;
							if(pstParams->data[0]==PE_DNT_HW_M14A_RD_DFLT_VFY_R)
								param.addr += PE_PE0_REG_M14_LRDIFF;
							is_valid = 1;
						}
						break;
				}
				if (is_valid)
				{
					param.data = PE_REG_M14_RD(param.addr);
					sprintf(buffer, "   {0x%04X, 0x%08X},\n", param.addr, param.data);
					rd_cnt++;
				}
				else
				{
					sprintf(buffer, "%s", PE_DNT_M14_PRINT_END);
					rd_cnt = 0;
				}
			}
			memcpy(pstParams->data, buffer, sizeof(char)*PE_TRACE_STR_SIZE);
		}
	}while (0);
	return ret;
}

