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

/** @file pe_nrd_hw_m14.c
 *
 *  driver for picture enhance noise reduction functions. ( used only within kdriver )
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
#include "pe_nrd_hw_m14.h"
#include "pe_nrd_hw_m14a0.h"
#include "pe_nrd_hw_m14b0.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
/* 352,288 = 704/2,576/2 */
#define PE_NRD_HW_M14_H_MIN			352
#define PE_NRD_HW_M14_V_MIN			288

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define PE_NRD_HW_M14_ERROR		PE_PRINT_ERROR

#define PE_NRD_HW_M14_DBG_PRINT(fmt,args...)	\
	if(_g_nrd_hw_m14_trace)\
	{\
		printk("[%x,%x][%s,%d] "fmt, \
			PE_CHIP_VER, g_pe_kdrv_ver_mask, __F__, __L__, ##args);\
	}

#define PE_NRD_HW_M14_CHECK_CODE(_checker,_action,fmt,args...)	\
	{if(_checker){PE_NRD_HW_M14_ERROR(fmt,##args);_action;}}

#define PE_NRD_HW_M14C_DBG_PRINT_DNR_FMT_STATE(_pre, _cur)	\
	{\
		char _str_buf[PE_TRACE_STR_SIZE];\
		sprintf(_str_buf, "%s(%s)->(%s):%s", #_cur, \
			PE_NRD_HW_M14C_DNR_FMT_TO_STR(_pre), \
			PE_NRD_HW_M14C_DNR_FMT_TO_STR(_cur), \
			((_pre)!=(_cur))? "different":"same");\
		PE_NRD_HW_M14_DBG_PRINT("%s\n", _str_buf);\
		if((_pre)!=(_cur))\
		{\
			PE_INF_M14_SetTraceDataBuf(_str_buf);\
		}\
	}
#define PE_NRD_HW_M14B_DBG_PRINT_TNR_FMT_STATE(_pre, _cur)	\
	{\
		char _str_buf[PE_TRACE_STR_SIZE];\
		sprintf(_str_buf, "%s(%s)->(%s):%s", #_cur, \
			PE_NRD_HW_M14B_TNR_FMT_TO_STR(_pre), \
			PE_NRD_HW_M14B_TNR_FMT_TO_STR(_cur), \
			((_pre)!=(_cur))? "different":"same");\
		PE_NRD_HW_M14_DBG_PRINT("%s\n", _str_buf);\
		if((_pre)!=(_cur))\
		{\
			PE_INF_M14_SetTraceDataBuf(_str_buf);\
		}\
	}
#define PE_NRD_HW_M14B_DBG_PRINT_TAL_FMT_STATE(_pre, _cur)	\
	{\
		char _str_buf[PE_TRACE_STR_SIZE];\
		sprintf(_str_buf, "%s(%s)->(%s):%s", #_cur, \
			PE_NRD_HW_M14B_TAL_FMT_TO_STR(_pre), \
			PE_NRD_HW_M14B_TAL_FMT_TO_STR(_cur), \
			((_pre)!=(_cur))? "different":"same");\
		PE_NRD_HW_M14_DBG_PRINT("%s\n", _str_buf);\
		if((_pre)!=(_cur))\
		{\
			PE_INF_M14_SetTraceDataBuf(_str_buf);\
		}\
	}
#define PE_NRD_HW_M14B_DBG_PRINT_DNR_FMT_STATE(_pre, _cur)	\
	{\
		char _str_buf[PE_TRACE_STR_SIZE];\
		sprintf(_str_buf, "%s(%s)->(%s):%s", #_cur, \
			PE_NRD_HW_M14B_DNR_FMT_TO_STR(_pre), \
			PE_NRD_HW_M14B_DNR_FMT_TO_STR(_cur), \
			((_pre)!=(_cur))? "different":"same");\
		PE_NRD_HW_M14_DBG_PRINT("%s\n", _str_buf);\
		if((_pre)!=(_cur))\
		{\
			PE_INF_M14_SetTraceDataBuf(_str_buf);\
		}\
	}
#define PE_NRD_HW_M14B_DBG_PRINT_IFC_FMT_STATE(_pre, _cur)	\
	{\
		char _str_buf[PE_TRACE_STR_SIZE];\
		sprintf(_str_buf, "%s(%s)->(%s):%s", #_cur, \
			PE_NRD_HW_M14B_IFC_FMT_TO_STR(_pre), \
			PE_NRD_HW_M14B_IFC_FMT_TO_STR(_cur), \
			((_pre)!=(_cur))? "different":"same");\
		PE_NRD_HW_M14_DBG_PRINT("%s\n", _str_buf);\
		if((_pre)!=(_cur))\
		{\
			PE_INF_M14_SetTraceDataBuf(_str_buf);\
		}\
	}

#define PE_NRD_HW_M14A_DBG_PRINT_TNR_FMT_STATE(_pre, _cur)	\
	{\
		char _str_buf[PE_TRACE_STR_SIZE];\
		sprintf(_str_buf, "%s(%s)->(%s):%s", #_cur, \
			PE_NRD_HW_M14A_TNR_FMT_TO_STR(_pre), \
			PE_NRD_HW_M14A_TNR_FMT_TO_STR(_cur), \
			((_pre)!=(_cur))? "different":"same");\
		PE_NRD_HW_M14_DBG_PRINT("%s\n", _str_buf);\
		if((_pre)!=(_cur))\
		{\
			PE_INF_M14_SetTraceDataBuf(_str_buf);\
		}\
	}
#define PE_NRD_HW_M14A_DBG_PRINT_DNR_FMT_STATE(_pre, _cur)	\
	{\
		char _str_buf[PE_TRACE_STR_SIZE];\
		sprintf(_str_buf, "%s(%s)->(%s):%s", #_cur, \
			PE_NRD_HW_M14A_DNR_FMT_TO_STR(_pre), \
			PE_NRD_HW_M14A_DNR_FMT_TO_STR(_cur), \
			((_pre)!=(_cur))? "different":"same");\
		PE_NRD_HW_M14_DBG_PRINT("%s\n", _str_buf);\
		if((_pre)!=(_cur))\
		{\
			PE_INF_M14_SetTraceDataBuf(_str_buf);\
		}\
	}

#ifdef PE_HW_M14B_USE_REG_CTRL
#define PE_NRD_HW_M14B_P0M_ENUM(_x)		PE_NRD_HW_M14B_P0M_ENUM_##_x
#define PE_NRD_HW_M14B_P0D_ENUM(_x)		PE_NRD_HW_M14B_P0D_ENUM_##_x

#define PE_NRD_HW_M14B_P0M_SET(_r, _f, _d)	\
	{	_g_pe_nrd_hw_m14b_p0m_reg_ctrl.data._r._f = (_d);\
		_g_pe_nrd_hw_m14b_p0m_reg_ctrl.mask._r._f = 0;}

#define PE_NRD_HW_M14B_P0D_SET(_r, _f, _d)	\
	{	_g_pe_nrd_hw_m14b_p0d_reg_ctrl.data._r._f = (_d);\
		_g_pe_nrd_hw_m14b_p0d_reg_ctrl.mask._r._f = 0;}

#define PE_NRD_HW_M14B_P0M_WR01(_r, _f1, _d1)	\
	{	PE_P0M_M14B0_QWr01(_r, _f1, _d1);\
		PE_NRD_HW_M14B_P0M_SET(_r, _f1, _d1);}
#define PE_NRD_HW_M14B_P0M_WR02(_r, _f1, _d1, _f2, _d2)	\
	{	PE_P0M_M14B0_QWr02(_r, _f1, _d1, _f2, _d2);\
		PE_NRD_HW_M14B_P0M_SET(_r, _f1, _d1);\
		PE_NRD_HW_M14B_P0M_SET(_r, _f2, _d2);}
#define PE_NRD_HW_M14B_P0M_WR03(_r, _f1, _d1, _f2, _d2, _f3, _d3)	\
	{	PE_P0M_M14B0_QWr03(_r, _f1, _d1, _f2, _d2, _f3, _d3);\
		PE_NRD_HW_M14B_P0M_SET(_r, _f1, _d1);\
		PE_NRD_HW_M14B_P0M_SET(_r, _f2, _d2);\
		PE_NRD_HW_M14B_P0M_SET(_r, _f3, _d3);}
#define PE_NRD_HW_M14B_P0M_WR04(_r, _f1, _d1, _f2, _d2, _f3, _d3, _f4, _d4)	\
	{	PE_P0M_M14B0_QWr04(_r, _f1, _d1, _f2, _d2, _f3, _d3, _f4, _d4);\
		PE_NRD_HW_M14B_P0M_SET(_r, _f1, _d1);\
		PE_NRD_HW_M14B_P0M_SET(_r, _f2, _d2);\
		PE_NRD_HW_M14B_P0M_SET(_r, _f3, _d3);\
		PE_NRD_HW_M14B_P0M_SET(_r, _f4, _d4);}
#define PE_NRD_HW_M14B_P0M_WR05(_r, _f1, _d1, _f2, _d2, _f3, _d3, _f4, _d4, _f5, _d5)	\
	{	PE_P0M_M14B0_QWr05(_r, _f1, _d1, _f2, _d2, _f3, _d3, _f4, _d4, _f5, _d5);\
		PE_NRD_HW_M14B_P0M_SET(_r, _f1, _d1);\
		PE_NRD_HW_M14B_P0M_SET(_r, _f2, _d2);\
		PE_NRD_HW_M14B_P0M_SET(_r, _f3, _d3);\
		PE_NRD_HW_M14B_P0M_SET(_r, _f4, _d4);\
		PE_NRD_HW_M14B_P0M_SET(_r, _f5, _d5);}

#define PE_NRD_HW_M14B_P0D_WR01(_r, _f1, _d1)	\
	{	PE_P0D_M14B0_QWr01(_r, _f1, _d1);\
		PE_NRD_HW_M14B_P0D_SET(_r, _f1, _d1);}
#define PE_NRD_HW_M14B_P0D_WR02(_r, _f1, _d1, _f2, _d2)	\
	{	PE_P0D_M14B0_QWr02(_r, _f1, _d1, _f2, _d2);\
		PE_NRD_HW_M14B_P0D_SET(_r, _f1, _d1);\
		PE_NRD_HW_M14B_P0D_SET(_r, _f2, _d2);}
#define PE_NRD_HW_M14B_P0D_WR03(_r, _f1, _d1, _f2, _d2, _f3, _d3)	\
	{	PE_P0D_M14B0_QWr03(_r, _f1, _d1, _f2, _d2, _f3, _d3);\
		PE_NRD_HW_M14B_P0D_SET(_r, _f1, _d1);\
		PE_NRD_HW_M14B_P0D_SET(_r, _f2, _d2);\
		PE_NRD_HW_M14B_P0D_SET(_r, _f3, _d3);}
#define PE_NRD_HW_M14B_P0D_WR04(_r, _f1, _d1, _f2, _d2, _f3, _d3, _f4, _d4)	\
	{	PE_P0D_M14B0_QWr04(_r, _f1, _d1, _f2, _d2, _f3, _d3, _f4, _d4);\
		PE_NRD_HW_M14B_P0D_SET(_r, _f1, _d1);\
		PE_NRD_HW_M14B_P0D_SET(_r, _f2, _d2);\
		PE_NRD_HW_M14B_P0D_SET(_r, _f3, _d3);\
		PE_NRD_HW_M14B_P0D_SET(_r, _f4, _d4);}
#define PE_NRD_HW_M14B_P0D_WR05(_r, _f1, _d1, _f2, _d2, _f3, _d3, _f4, _d4, _f5, _d5)	\
	{	PE_P0D_M14B0_QWr05(_r, _f1, _d1, _f2, _d2, _f3, _d3, _f4, _d4, _f5, _d5);\
		PE_NRD_HW_M14B_P0D_SET(_r, _f1, _d1);\
		PE_NRD_HW_M14B_P0D_SET(_r, _f2, _d2);\
		PE_NRD_HW_M14B_P0D_SET(_r, _f3, _d3);\
		PE_NRD_HW_M14B_P0D_SET(_r, _f4, _d4);\
		PE_NRD_HW_M14B_P0D_SET(_r, _f5, _d5);}

#define PE_NRD_HW_M14B_TNR_REG_TABLE(_dflt, _base)	\
	do{\
		PE_REG_PARAM_T *p_##_dflt = (PE_REG_PARAM_T *)OS_Malloc(sizeof(_dflt));\
		PE_NRD_HW_M14_CHECK_CODE(!(p_##_dflt), break, \
			"[%s,%d] p_tbl is null.\n", __F__, __L__);\
		do{\
			ret = PE_NRD_HW_M14B_GetTnrDfltTable(p_##_dflt, _dflt, sizeof(_dflt));\
			PE_NRD_HW_M14_CHECK_CODE(ret, break, \
				"[%s,%d] PE_NRD_HW_M14B_GetTnrDfltTable() error.\n", __F__, __L__);\
			PE_NRD_HW_M14_DBG_PRINT("write %s(+ 0x%08x).\n", #_dflt, (_base));\
			ret = PE_REG_SetTraceTable(__F__,__L__, #_dflt, (_base));\
			ret = PE_REG_SetTable(p_##_dflt, (_base), sizeof(_dflt));\
			PE_NRD_HW_M14_CHECK_CODE(ret, break, \
				"[%s,%d] PE_REG_SetTable() error.\n", __F__, __L__);\
		}while (0);\
		if (p_##_dflt)	{OS_Free(p_##_dflt);}\
	}while (0)

#define PE_NRD_HW_M14B_DNR_REG_TABLE(_dflt, _base)	\
	do{\
		PE_REG_PARAM_T *p_##_dflt = (PE_REG_PARAM_T *)OS_Malloc(sizeof(_dflt));\
		PE_NRD_HW_M14_CHECK_CODE(!(p_##_dflt), break, \
			"[%s,%d] p_tbl is null.\n", __F__, __L__);\
		do{\
			ret = PE_NRD_HW_M14B_GetDnrDfltTable(p_##_dflt, _dflt, sizeof(_dflt));\
			PE_NRD_HW_M14_CHECK_CODE(ret, break, \
				"[%s,%d] PE_NRD_HW_M14B_GetDnrDfltTable() error.\n", __F__, __L__);\
			PE_NRD_HW_M14_DBG_PRINT("write %s(+ 0x%08x).\n", #_dflt, (_base));\
			ret = PE_REG_SetTraceTable(__F__,__L__, #_dflt, (_base));\
			ret = PE_REG_SetTable(p_##_dflt, (_base), sizeof(_dflt));\
			PE_NRD_HW_M14_CHECK_CODE(ret, break, \
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
 *	pe p0m register num
 */
typedef enum
{
	PE_NRD_HW_M14B_P0M_ENUM(PE_M14B0_P0M_TNR_CTRL_00_T) = 0,	///< [0]  0xc0022140L
	PE_NRD_HW_M14B_P0M_ENUM(PE_M14B0_P0M_TNR_CTRL_04_T),        ///< [1]  0xc0022150L
	PE_NRD_HW_M14B_P0M_ENUM(PE_M14B0_P0M_TNR_CTRL_05_T),        ///< [2]  0xc0022154L
	PE_NRD_HW_M14B_P0M_ENUM(PE_M14B0_P0M_TNR_CTRL_07_T),        ///< [3]  0xc002215cL
	PE_NRD_HW_M14B_P0M_ENUM(PE_M14B0_P0M_TNR_CTRL_08_T),        ///< [4]  0xc0022160L
	PE_NRD_HW_M14B_P0M_ENUM(PE_M14B0_P0M_TNR_CTRL_09_T),        ///< [5]  0xc0022164L
	PE_NRD_HW_M14B_P0M_ENUM(PE_M14B0_P0M_TNR_CTRL_12_T),        ///< [6]  0xc0022170L
	PE_NRD_HW_M14B_P0M_ENUM(PE_M14B0_P0M_TNR_CTRL_16_T),        ///< [7]  0xc0022180L
	PE_NRD_HW_M14B_P0M_ENUM(PE_M14B0_P0M_TNR_CTRL_19_T),        ///< [8]  0xc002218cL
	PE_NRD_HW_M14B_P0M_ENUM(PE_M14B0_P0M_TNR_CTRL_20_T),        ///< [9]  0xc0022190L
	PE_NRD_HW_M14B_P0M_ENUM(PE_M14B0_P0M_TNR_CTRL_25_T),        ///< [10] 0xc00221a4L
	PE_NRD_HW_M14B_P0M_ENUM(PE_M14B0_P0M_TNR_CTRL_31_T),        ///< [11] 0xc00221bcL
	PE_NRD_HW_M14B_P0M_ENUM(PE_M14B0_P0M_TNR_CTRL_32_T),        ///< [12] 0xc00221e0L
	PE_NRD_HW_M14B_P0M_ENUM(PE_M14B0_P0M_TNR_CTRL_33_T),        ///< [13] 0xc00221e4L
	PE_NRD_HW_M14B_P0M_ENUM(PE_M14B0_P0M_TNR_CTRL_34_T),        ///< [14] 0xc00221e8L
	PE_NRD_HW_M14B_P0M_ENUM(PE_M14B0_P0M_TNR_CTRL_35_T),        ///< [15] 0xc00221ecL
	PE_NRD_HW_M14B_P0M_ENUM(PE_M14B0_P0M_TNR_CTRL_36_T),        ///< [16] 0xc00221f0L
	PE_NRD_HW_M14B_P0M_ENUM(PE_M14B0_P0M_TNR_CTRL_37_T),        ///< [17] 0xc00221f4L
	PE_NRD_HW_M14B_P0M_ENUM(PE_M14B0_P0M_TNR_CTRL_38_T),        ///< [18] 0xc00221f8L
	PE_NRD_HW_M14B_P0M_ENUM(PE_M14B0_P0M_TNR_CTRL_39_T),        ///< [19] 0xc00221fcL
	PE_NRD_HW_M14B_P0M_ENUM(PE_M14B0_P0M_TNR_CTRL_42_T),        ///< [20] 0xc00223d8L
	PE_NRD_HW_M14B_P0M_ENUM(PE_M14B0_P0M_TNR_CTRL_43_T),        ///< [21] 0xc00223dcL
	PE_NRD_HW_M14B_P0M_ENUM(PE_M14B0_P0M_TNR_CTRL_48_T),        ///< [22] 0xc00223f0L
	PE_NRD_HW_M14B_P0M_ENUM(PE_M14B0_P0M_TNR_CTRL_50_T),        ///< [23] 0xc00223f8L
	PE_NRD_HW_M14B_P0M_ENUM(MAX)								///< max num
}
PE_NRD_HW_M14B_P0M_REG_NUM;

/**
 *	pe p0d register num
 */
typedef enum
{
	PE_NRD_HW_M14B_P0D_ENUM(PE_M14B0_P0D_DNR_MAX_CTRL_T) = 0,   ///< [0]  0xc0023a94L
	PE_NRD_HW_M14B_P0D_ENUM(PE_M14B0_P0D_MNR_CTRL_0_T),         ///< [1]  0xc0023aa0L
	PE_NRD_HW_M14B_P0D_ENUM(PE_M14B0_P0D_MNR_CTRL_1_T),         ///< [2]  0xc0023aa4L
	PE_NRD_HW_M14B_P0D_ENUM(PE_M14B0_P0D_MNR_CTRL_2_T),         ///< [3]  0xc0023aa8L
	PE_NRD_HW_M14B_P0D_ENUM(PE_M14B0_P0D_MNR_CTRL_3_T),         ///< [4]  0xc0023aacL
	PE_NRD_HW_M14B_P0D_ENUM(PE_M14B0_P0D_MNR_CTRL_4_T),         ///< [5]  0xc0023ab0L
	PE_NRD_HW_M14B_P0D_ENUM(PE_M14B0_P0D_MNR_CTRL_5_T),         ///< [6]  0xc0023ab4L
	PE_NRD_HW_M14B_P0D_ENUM(PE_M14B0_P0D_DETAIL_CTRL_T),        ///< [7]  0xc0023ab8L
	PE_NRD_HW_M14B_P0D_ENUM(PE_M14B0_P0D_DC_BNR_CTRL_0_T),      ///< [8]  0xc0023abcL
	PE_NRD_HW_M14B_P0D_ENUM(PE_M14B0_P0D_DC_BNR_CTRL_1_T),      ///< [9]  0xc0023ac0L
	PE_NRD_HW_M14B_P0D_ENUM(PE_M14B0_P0D_DC_BNR_CTRL_2_T),      ///< [10] 0xc0023ac4L
	PE_NRD_HW_M14B_P0D_ENUM(PE_M14B0_P0D_DC_BNR_CTRL_3_T),      ///< [11] 0xc0023ac8L
	PE_NRD_HW_M14B_P0D_ENUM(PE_M14B0_P0D_AC_BNR_CTRL_0_T),      ///< [12] 0xc0023ad4L
	PE_NRD_HW_M14B_P0D_ENUM(PE_M14B0_P0D_AC_BNR_CTRL_1_T),      ///< [13] 0xc0023ad8L
	PE_NRD_HW_M14B_P0D_ENUM(PE_M14B0_P0D_AC_BNR_CTRL_4_T),      ///< [14] 0xc0023ae8L
	PE_NRD_HW_M14B_P0D_ENUM(PE_M14B0_P0D_AC_BNR_CTRL_5_T),      ///< [15] 0xc0023af0L
	PE_NRD_HW_M14B_P0D_ENUM(PE_M14B0_P0D_AC_BNR_CTRL_6_T),      ///< [16] 0xc0023af4L
	PE_NRD_HW_M14B_P0D_ENUM(PE_M14B0_P0D_AC_BNR_CTRL_7_T),      ///< [17] 0xc0023af8L
	PE_NRD_HW_M14B_P0D_ENUM(PE_M14B0_P0D_AC_BNR_CTRL_8_T),      ///< [18] 0xc0023b00L
	PE_NRD_HW_M14B_P0D_ENUM(PE_M14B0_P0D_AC_BNR_CTRL_9_T),      ///< [19] 0xc0023b04L
	PE_NRD_HW_M14B_P0D_ENUM(MAX)								///< max num
}
PE_NRD_HW_M14B_P0D_REG_NUM;

/**
 *	pe p0m register control type
 */
typedef struct
{
	PE_M14B0_P0M_TNR_CTRL_00_T         	p0m_tnr_ctrl_00;              ///< [0]  0xc0022140L
	PE_M14B0_P0M_TNR_CTRL_04_T         	p0m_tnr_ctrl_04;              ///< [1]  0xc0022150L
	PE_M14B0_P0M_TNR_CTRL_05_T         	p0m_tnr_ctrl_05;              ///< [2]  0xc0022154L
	PE_M14B0_P0M_TNR_CTRL_07_T         	p0m_tnr_ctrl_07;              ///< [3]  0xc002215cL
	PE_M14B0_P0M_TNR_CTRL_08_T         	p0m_tnr_ctrl_08;              ///< [4]  0xc0022160L
	PE_M14B0_P0M_TNR_CTRL_09_T         	p0m_tnr_ctrl_09;              ///< [5]  0xc0022164L
	PE_M14B0_P0M_TNR_CTRL_12_T         	p0m_tnr_ctrl_12;              ///< [6]  0xc0022170L
	PE_M14B0_P0M_TNR_CTRL_16_T         	p0m_tnr_ctrl_16;              ///< [7]  0xc0022180L
	PE_M14B0_P0M_TNR_CTRL_19_T         	p0m_tnr_ctrl_19;              ///< [8]  0xc002218cL
	PE_M14B0_P0M_TNR_CTRL_20_T         	p0m_tnr_ctrl_20;              ///< [9]  0xc0022190L
	PE_M14B0_P0M_TNR_CTRL_25_T         	p0m_tnr_ctrl_25;              ///< [10] 0xc00221a4L
	PE_M14B0_P0M_TNR_CTRL_31_T         	p0m_tnr_ctrl_31;              ///< [11] 0xc00221bcL
	PE_M14B0_P0M_TNR_CTRL_32_T         	p0m_tnr_ctrl_32;              ///< [12] 0xc00221e0L
	PE_M14B0_P0M_TNR_CTRL_33_T         	p0m_tnr_ctrl_33;              ///< [13] 0xc00221e4L
	PE_M14B0_P0M_TNR_CTRL_34_T         	p0m_tnr_ctrl_34;              ///< [14] 0xc00221e8L
	PE_M14B0_P0M_TNR_CTRL_35_T         	p0m_tnr_ctrl_35;              ///< [15] 0xc00221ecL
	PE_M14B0_P0M_TNR_CTRL_36_T         	p0m_tnr_ctrl_36;              ///< [16] 0xc00221f0L
	PE_M14B0_P0M_TNR_CTRL_37_T         	p0m_tnr_ctrl_37;              ///< [17] 0xc00221f4L
	PE_M14B0_P0M_TNR_CTRL_38_T         	p0m_tnr_ctrl_38;              ///< [18] 0xc00221f8L
	PE_M14B0_P0M_TNR_CTRL_39_T         	p0m_tnr_ctrl_39;              ///< [19] 0xc00221fcL
	PE_M14B0_P0M_TNR_CTRL_42_T         	p0m_tnr_ctrl_42;              ///< [20] 0xc00223d8L
	PE_M14B0_P0M_TNR_CTRL_43_T         	p0m_tnr_ctrl_43;              ///< [21] 0xc00223dcL
	PE_M14B0_P0M_TNR_CTRL_48_T         	p0m_tnr_ctrl_48;              ///< [22] 0xc00223f0L
	PE_M14B0_P0M_TNR_CTRL_50_T         	p0m_tnr_ctrl_50;              ///< [23] 0xc00223f8L
}
PE_NRD_HW_M14B_P0M_REG_T;

/**
 *	pe p0d register control type
 */
typedef struct
{
	PE_M14B0_P0D_DNR_MAX_CTRL_T      	p0d_dnr_max_ctrl;             ///< [0]  0xc0023a94L
	PE_M14B0_P0D_MNR_CTRL_0_T        	p0d_mnr_ctrl_0;               ///< [1]  0xc0023aa0L
	PE_M14B0_P0D_MNR_CTRL_1_T        	p0d_mnr_ctrl_1;               ///< [2]  0xc0023aa4L
	PE_M14B0_P0D_MNR_CTRL_2_T        	p0d_mnr_ctrl_2;               ///< [3]  0xc0023aa8L
	PE_M14B0_P0D_MNR_CTRL_3_T        	p0d_mnr_ctrl_3;               ///< [4]  0xc0023aacL
	PE_M14B0_P0D_MNR_CTRL_4_T        	p0d_mnr_ctrl_4;               ///< [5]  0xc0023ab0L
	PE_M14B0_P0D_MNR_CTRL_5_T        	p0d_mnr_ctrl_5;               ///< [6]  0xc0023ab4L
	PE_M14B0_P0D_DETAIL_CTRL_T       	p0d_detail_ctrl;              ///< [7]  0xc0023ab8L
	PE_M14B0_P0D_DC_BNR_CTRL_0_T     	p0d_dc_bnr_ctrl_0;            ///< [8]  0xc0023abcL
	PE_M14B0_P0D_DC_BNR_CTRL_1_T     	p0d_dc_bnr_ctrl_1;            ///< [9]  0xc0023ac0L
	PE_M14B0_P0D_DC_BNR_CTRL_2_T     	p0d_dc_bnr_ctrl_2;            ///< [10] 0xc0023ac4L
	PE_M14B0_P0D_DC_BNR_CTRL_3_T     	p0d_dc_bnr_ctrl_3;            ///< [11] 0xc0023ac8L
	PE_M14B0_P0D_AC_BNR_CTRL_0_T     	p0d_ac_bnr_ctrl_0;            ///< [12] 0xc0023ad4L
	PE_M14B0_P0D_AC_BNR_CTRL_1_T     	p0d_ac_bnr_ctrl_1;            ///< [13] 0xc0023ad8L
	PE_M14B0_P0D_AC_BNR_CTRL_4_T     	p0d_ac_bnr_ctrl_4;            ///< [14] 0xc0023ae8L
	PE_M14B0_P0D_AC_BNR_CTRL_5_T     	p0d_ac_bnr_ctrl_5;            ///< [15] 0xc0023af0L
	PE_M14B0_P0D_AC_BNR_CTRL_6_T     	p0d_ac_bnr_ctrl_6;            ///< [16] 0xc0023af4L
	PE_M14B0_P0D_AC_BNR_CTRL_7_T     	p0d_ac_bnr_ctrl_7;            ///< [17] 0xc0023af8L
	PE_M14B0_P0D_AC_BNR_CTRL_8_T     	p0d_ac_bnr_ctrl_8;            ///< [18] 0xc0023b00L
	PE_M14B0_P0D_AC_BNR_CTRL_9_T     	p0d_ac_bnr_ctrl_9;            ///< [19] 0xc0023b04L
}
PE_NRD_HW_M14B_P0D_REG_T;

/**
 *	pe p0m register control type
 */
typedef struct
{
	UINT32 addr[PE_NRD_HW_M14B_P0M_ENUM(MAX)];	///< ctrl addr
	PE_NRD_HW_M14B_P0M_REG_T data;				///< ctrl data
	PE_NRD_HW_M14B_P0M_REG_T mask;				///< ctrl mask
}
PE_NRD_HW_M14B_P0M_REG_CTRL_T;

/**
 *	pe p0d register control type
 */
typedef struct
{
	UINT32 addr[PE_NRD_HW_M14B_P0D_ENUM(MAX)];	///< ctrl addr
	PE_NRD_HW_M14B_P0D_REG_T data;				///< ctrl data
	PE_NRD_HW_M14B_P0D_REG_T mask;				///< ctrl mask
}
PE_NRD_HW_M14B_P0D_REG_CTRL_T;

/**
 *	pe p0m register match type
 */
typedef struct
{
	UINT32 tnr_size;				///< tnr size
	PE_REG_PARAM_T *tnr_mtbl;		///< tnr match table
}
PE_NRD_HW_M14B_P0M_REG_MATCH_T;

/**
 *	pe p0d register match type
 */
typedef struct
{
	UINT32 dnr_size;				///< dnr size
	PE_REG_PARAM_T *dnr_mtbl;		///< dnr match table
}
PE_NRD_HW_M14B_P0D_REG_MATCH_T;
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
static int PE_NRD_HW_M14B_GetTnrDfltTable(PE_REG_PARAM_T *dst_dflt, const PE_REG_PARAM_T *src_dflt, UINT32 dflt_size);
static int PE_NRD_HW_M14B_GetDnrDfltTable(PE_REG_PARAM_T *dst_dflt, const PE_REG_PARAM_T *src_dflt, UINT32 dflt_size);
#endif
static int PE_NRD_HW_M14B_SetDefault(LX_PE_DEFAULT_SETTINGS_T *pstParams);
static int PE_NRD_HW_M14A_SetDefault(LX_PE_DEFAULT_SETTINGS_T *pstParams);
static PE_NRD_HW_M14C_DNR_FMT PE_NRD_HW_M14C_ConvDispInfoToDnrFmt(LX_PE_INF_DISPLAY_T *disp_inf);
static PE_NRD_HW_M14B_TNR_FMT PE_NRD_HW_M14B_ConvDispInfoToTnrFmt(LX_PE_INF_DISPLAY_T *disp_inf);
static PE_NRD_HW_M14B_TAL_FMT PE_NRD_HW_M14B_ConvDispInfoToTalFmt(LX_PE_INF_DISPLAY_T *disp_inf);
static PE_NRD_HW_M14B_DNR_FMT PE_NRD_HW_M14B_ConvDispInfoToDnrFmt(LX_PE_INF_DISPLAY_T *disp_inf);
static PE_NRD_HW_M14B_IFC_FMT PE_NRD_HW_M14B_ConvDispInfoToIfcFmt(LX_PE_INF_DISPLAY_T *disp_inf);
static PE_NRD_HW_M14A_TNR_FMT PE_NRD_HW_M14A_ConvDispInfoToTnrFmt(LX_PE_INF_DISPLAY_T *disp_inf);
static PE_NRD_HW_M14A_DNR_FMT PE_NRD_HW_M14A_ConvDispInfoToDnrFmt(LX_PE_INF_DISPLAY_T *disp_inf);

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
static UINT8 _g_nrd_hw_m14_trace=0x0;	//default should be off.
static PE_NRD_HW_M14_SETTINGS_T _g_pe_nrd_hw_m14_info;
#ifdef PE_HW_M14B_USE_REG_CTRL
static PE_NRD_HW_M14B_P0M_REG_CTRL_T _g_pe_nrd_hw_m14b_p0m_reg_ctrl;
static PE_NRD_HW_M14B_P0D_REG_CTRL_T _g_pe_nrd_hw_m14b_p0d_reg_ctrl;
static PE_NRD_HW_M14B_P0M_REG_MATCH_T _g_pe_nrd_hw_m14b_p0m_reg_match = {0, NULL};
static PE_NRD_HW_M14B_P0D_REG_MATCH_T _g_pe_nrd_hw_m14b_p0d_reg_match = {0, NULL};
const UINT32 g_pe_nrd_hw_m14b_p0m_reg_addr[PE_NRD_HW_M14B_P0M_ENUM(MAX)] = {
	0x2140,              ///< [0]  0xc0022140L
	0x2150,              ///< [1]  0xc0022150L
	0x2154,              ///< [2]  0xc0022154L
	0x215c,              ///< [3]  0xc002215cL
	0x2160,              ///< [4]  0xc0022160L
	0x2164,              ///< [5]  0xc0022164L
	0x2170,              ///< [6]  0xc0022170L
	0x2180,              ///< [7]  0xc0022180L
	0x218c,              ///< [8]  0xc002218cL
	0x2190,              ///< [9]  0xc0022190L
	0x21a4,              ///< [10] 0xc00221a4L
	0x21bc,              ///< [11] 0xc00221bcL
	0x21e0,              ///< [12] 0xc00221e0L
	0x21e4,              ///< [13] 0xc00221e4L
	0x21e8,              ///< [14] 0xc00221e8L
	0x21ec,              ///< [15] 0xc00221ecL
	0x21f0,              ///< [16] 0xc00221f0L
	0x21f4,              ///< [17] 0xc00221f4L
	0x21f8,              ///< [18] 0xc00221f8L
	0x21fc,              ///< [19] 0xc00221fcL
	0x23d8,              ///< [20] 0xc00223d8L
	0x23dc,              ///< [21] 0xc00223dcL
	0x23f0,              ///< [22] 0xc00223f0L
	0x23f8               ///< [23] 0xc00223f8L
};
const UINT32 g_pe_nrd_hw_m14b_p0d_reg_addr[PE_NRD_HW_M14B_P0D_ENUM(MAX)] = {
	0x3a94,            ///< [0]  0xc0023a94L
	0x3aa0,            ///< [1]  0xc0023aa0L
	0x3aa4,            ///< [2]  0xc0023aa4L
	0x3aa8,            ///< [3]  0xc0023aa8L
	0x3aac,            ///< [4]  0xc0023aacL
	0x3ab0,            ///< [5]  0xc0023ab0L
	0x3ab4,            ///< [6]  0xc0023ab4L
	0x3ab8,            ///< [7]  0xc0023ab8L
	0x3abc,            ///< [8]  0xc0023abcL
	0x3ac0,            ///< [9]  0xc0023ac0L
	0x3ac4,            ///< [10] 0xc0023ac4L
	0x3ac8,            ///< [11] 0xc0023ac8L
	0x3ad4,            ///< [12] 0xc0023ad4L
	0x3ad8,            ///< [13] 0xc0023ad8L
	0x3ae8,            ///< [14] 0xc0023ae8L
	0x3af0,            ///< [15] 0xc0023af0L
	0x3af4,            ///< [16] 0xc0023af4L
	0x3af8,            ///< [17] 0xc0023af8L
	0x3b00,            ///< [18] 0xc0023b00L
	0x3b04             ///< [19] 0xc0023b04L
};
#endif

/*----------------------------------------------------------------------------------------
	Implementation Group
----------------------------------------------------------------------------------------*/
/**
 * init nrd
 *
 * @param   *pstParams [in] PE_CFG_CTRL_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_NRD_HW_M14_Init(PE_CFG_CTRL_T *pstParams)
{
	int ret = RET_OK;
	#ifdef PE_HW_M14B_USE_REG_CTRL
	UINT32 i = 0, j = 0, tbl_size = 0, tbl_num = 0;
	PE_NRD_HW_M14B_P0M_REG_CTRL_T *p0m_reg_ctrl = &_g_pe_nrd_hw_m14b_p0m_reg_ctrl;
	PE_NRD_HW_M14B_P0M_REG_MATCH_T *p0m_reg_match = &_g_pe_nrd_hw_m14b_p0m_reg_match;
	PE_NRD_HW_M14B_P0D_REG_CTRL_T *p0d_reg_ctrl = &_g_pe_nrd_hw_m14b_p0d_reg_ctrl;
	PE_NRD_HW_M14B_P0D_REG_MATCH_T *p0d_reg_match = &_g_pe_nrd_hw_m14b_p0d_reg_match;
	#endif
	do{
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_M14)
		{
			_g_pe_nrd_hw_m14_info.tnr0a_fmt = PE_NRD_M14A_TNR_NUM;
			_g_pe_nrd_hw_m14_info.tnr1a_fmt = PE_NRD_M14A_TNR_NUM;
			_g_pe_nrd_hw_m14_info.dnr0a_fmt = PE_NRD_M14A_DNR_NUM;
			_g_pe_nrd_hw_m14_info.dnr1a_fmt = PE_NRD_M14A_DNR_NUM;
			_g_pe_nrd_hw_m14_info.tnr0b_fmt = PE_NRD_M14B_TNR_NUM;
			_g_pe_nrd_hw_m14_info.tal0b_fmt = PE_NRD_M14B_TAL_NUM;
			_g_pe_nrd_hw_m14_info.dnr0b_fmt = PE_NRD_M14B_DNR_NUM;
			_g_pe_nrd_hw_m14_info.ifc0b_fmt = PE_NRD_M14B_IFC_NUM;
			_g_pe_nrd_hw_m14_info.dnr0c_fmt = PE_NRD_M14C_DNR_NUM;
			if(PE_KDRV_VER_M14BX)
			{
				#ifdef PE_HW_M14B_USE_DNR_MODE_ON_TASK
				_g_pe_nrd_hw_m14_info.reg_bnr_ac_h_en0 = 0;
				#endif
				#ifdef PE_HW_M14_USE_TNR_GAIN_ON_TASK
				_g_pe_nrd_hw_m14_info.cur_tnr_auto_enable = 0;
				_g_pe_nrd_hw_m14_info.cur_tnr_master_gain = 0xc0;	// sync with dflt
				_g_pe_nrd_hw_m14_info.pre_tnr_master_gain = 0xc0;	// sync with dflt
				#endif
				PE_SET_REG_TABLE(NRD_HW_M14,nr_l_init_m14b0,PE_REG_M14B_WRITE_BASE);
				PE_SET_REG_TABLE(NRD_HW_M14,tnr_l_alphalut_init_ai_m14b0,PE_REG_M14B_WRITE_BASE);
				ret = PE_FWI_M14_SetFwTnrEnable(TRUE);
				PE_NRD_HW_M14_CHECK_CODE(ret, break, \
					"[%s,%d] PE_FWI_M14_SetFwTnrEnable() error.\n",__F__,__L__);
				#ifdef PE_HW_M14B_USE_REG_CTRL
				/* init reg ctrl data and mask */
				memset(&(p0m_reg_ctrl->data), 0, sizeof(PE_NRD_HW_M14B_P0M_REG_T));	// 0x0
				memset(&(p0m_reg_ctrl->mask), -1, sizeof(PE_NRD_HW_M14B_P0M_REG_T));// 0xffffffff
				/* init reg ctrl addr */
				for (i=0; i<PE_NRD_HW_M14B_P0M_ENUM(MAX); i++)
				{
					p0m_reg_ctrl->addr[i] = g_pe_nrd_hw_m14b_p0m_reg_addr[i];
				}
				/* res matching table */
				if (p0m_reg_match->tnr_size==0 && p0m_reg_match->tnr_mtbl==NULL)
				{
					tbl_size = sizeof(tnr_l_pc_default_m14b0);
					tbl_num = tbl_size/sizeof(PE_REG_PARAM_T);
					/* alloc matching table */
					p0m_reg_match->tnr_mtbl = (PE_REG_PARAM_T *)OS_Malloc(tbl_size);
					PE_NRD_HW_M14_CHECK_CODE(!(p0m_reg_match->tnr_mtbl), break, \
						"[%s,%d] p0m_reg_match->tnr_mtbl is null.\n", __F__, __L__);
					p0m_reg_match->tnr_size = tbl_size;

					for (i=0; i<tbl_num; i++)
					{
						/* set addr and data of matching table */
						p0m_reg_match->tnr_mtbl[i].addr = tnr_l_pc_default_m14b0[i].addr;
						p0m_reg_match->tnr_mtbl[i].data = PE_NRD_HW_M14B_P0M_ENUM(MAX);
						/* check matching number */
						for (j=0; j<PE_NRD_HW_M14B_P0M_ENUM(MAX); j++)
						{
							if (p0m_reg_match->tnr_mtbl[i].addr == p0m_reg_ctrl->addr[j])
							{
								p0m_reg_match->tnr_mtbl[i].data = j;
								break;
							}
						}
					}
					PE_PRINT_NOTI("tnr matching table created.\n");
				}
				/* init reg ctrl data and mask */
				memset(&(p0d_reg_ctrl->data), 0, sizeof(PE_NRD_HW_M14B_P0D_REG_T));	// 0x0
				memset(&(p0d_reg_ctrl->mask), -1, sizeof(PE_NRD_HW_M14B_P0D_REG_T));// 0xffffffff
				/* init reg ctrl addr */
				for (i=0; i<PE_NRD_HW_M14B_P0D_ENUM(MAX); i++)
				{
					p0d_reg_ctrl->addr[i] = g_pe_nrd_hw_m14b_p0d_reg_addr[i];
				}
				/* res matching table */
				if (p0d_reg_match->dnr_size==0 && p0d_reg_match->dnr_mtbl==NULL)
				{
					tbl_size = sizeof(dnr_l_pc_default_m14b0);
					tbl_num = tbl_size/sizeof(PE_REG_PARAM_T);
					/* alloc matching table */
					p0d_reg_match->dnr_mtbl = (PE_REG_PARAM_T *)OS_Malloc(tbl_size);
					PE_NRD_HW_M14_CHECK_CODE(!(p0d_reg_match->dnr_mtbl), break, \
						"[%s,%d] p0d_reg_match->dnr_mtbl is null.\n", __F__, __L__);
					p0d_reg_match->dnr_size = tbl_size;

					for (i=0; i<tbl_num; i++)
					{
						/* set addr and data of matching table */
						p0d_reg_match->dnr_mtbl[i].addr = dnr_l_pc_default_m14b0[i].addr;
						p0d_reg_match->dnr_mtbl[i].data = PE_NRD_HW_M14B_P0D_ENUM(MAX);
						/* check matching number */
						for (j=0; j<PE_NRD_HW_M14B_P0D_ENUM(MAX); j++)
						{
							if (p0d_reg_match->dnr_mtbl[i].addr == p0d_reg_ctrl->addr[j])
							{
								p0d_reg_match->dnr_mtbl[i].data = j;
								break;
							}
						}
					}
					PE_PRINT_NOTI("dnr matching table created.\n");
				}
				#endif
			}
			else if(PE_KDRV_VER_M14AX)
			{
				#ifdef PE_HW_M14A_USE_DNR_MODE_ON_TASK
				_g_pe_nrd_hw_m14_info.reg_bnr_ac_h_en0 = 0;
				_g_pe_nrd_hw_m14_info.reg_bnr_ac_h_en1 = 0;
				#endif
				PE_P0L_M14A0_QWr01(p0l_pe0_load,	load_time,	0x1);
				PE_P0R_M14A0_QWr01(p0r_pe0_load,	load_time,	0x1);
				ret = PE_FWI_M14_SetFwTnrEnable(FALSE);
				PE_NRD_HW_M14_CHECK_CODE(ret,break,"[%s,%d] PE_FWI_M14_SetFwTnrEnable() error.\n",__F__,__L__);
				PE_SET_REG_TABLE(NRD_HW_M14,tnr_l_crg_init_ia_m14a0,PE_REG_M14_WRITE_BASE);
				PE_SET_REG_TABLE(NRD_HW_M14,tnr_l_alphalut_init_ia_m14a0,PE_REG_M14_WRITE_BASE);
				PE_SET_REG_TABLE(NRD_HW_M14,tnr_l_crg_init_ia_m14a0,PE_PE0_REG_M14_LRDIFF);
				PE_SET_REG_TABLE(NRD_HW_M14,tnr_l_alphalut_init_ia_m14a0,PE_PE0_REG_M14_LRDIFF);
				PE_SET_REG_TABLE(NRD_HW_M14,nr_l_init_m14a0,PE_REG_M14_WRITE_BASE);
				PE_SET_REG_TABLE(NRD_HW_M14,nr_l_init_m14a0,PE_PE0_REG_M14_LRDIFF);
				ret = PE_FWI_M14_SetFwTnrEnable(TRUE);
				PE_NRD_HW_M14_CHECK_CODE(ret,break,"[%s,%d] PE_FWI_M14_SetFwTnrEnable() error.\n",__F__,__L__);
				PE_P0L_M14A0_QWr01(p0l_pe0_load,	load_time,	0x0);
				PE_P0R_M14A0_QWr01(p0r_pe0_load,	load_time,	0x0);
				/* enable tnr noise measure window */
				PE_P0L_M14A0_QWr01(p0l_tnr_ctrl_14,	noise_measure_win_en,	0x1);
				PE_P0R_M14A0_QWr01(p0r_tnr_ctrl_14,	noise_measure_win_en,	0x1);
			}
			else
			{
				PE_NRD_HW_M14_DBG_PRINT("nothing to do\n");
			}
		}
		else
		{
			PE_NRD_HW_M14_DBG_PRINT("nothing to do\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * set debug settings
 *
 * @param   *pstParams [in] LX_PE_DBG_SETTINGS_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_NRD_HW_M14_SetDebugSettings(LX_PE_DBG_SETTINGS_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		PE_NRD_HW_M14_DBG_PRINT("wid:%d,type:0x%x,[0x%x]print:0x%x,bypass:0x%x\n",\
			pstParams->win_id,pstParams->type,(0x1<<PE_ITEM_PKTMODL(NRD)),\
			pstParams->print_lvl,pstParams->bypass);
		/* set print level */
		if(pstParams->type&LX_PE_DBG_LV)
		{
			_g_nrd_hw_m14_trace = (pstParams->print_lvl & (0x1<<PE_ITEM_PKTMODL(NRD)))? 0x1:0x0;
		}
		/* set bypass */
		if(pstParams->type&LX_PE_DBG_BY)
		{
			if(PE_KDRV_VER_M14BX)
			{
				if(pstParams->bypass & (0x1<<PE_ITEM_PKTMODL(NRD)))
				{
					PE_NRD_HW_M14_DBG_PRINT("tnr,dnr : disable.\n");
					if(PE_CHECK_WIN0(pstParams->win_id))
					{
						PE_P0M_M14B0_QWr01(p0m_tnr_ctrl_00,	tnr_enable, 	0x0);
						PE_P0D_M14B0_QWr01(p0d_dnr_max_ctrl,reg_dnr_max_enable, 0x0);
					}
				}
				else
				{
					PE_NRD_HW_M14_DBG_PRINT("tnr,dnr : enable.\n");
					if(PE_CHECK_WIN0(pstParams->win_id))
					{
						PE_P0M_M14B0_QWr01(p0m_tnr_ctrl_00,	tnr_enable, 	0x1);
						PE_P0D_M14B0_QWr01(p0d_dnr_max_ctrl,reg_dnr_max_enable, 0x1);
					}
				}
			}
			else if(PE_KDRV_VER_M14AX)
			{
				if(pstParams->bypass & (0x1<<PE_ITEM_PKTMODL(NRD)))
				{
					PE_NRD_HW_M14_DBG_PRINT("tnr,dnr : disable.\n");
					if(PE_CHECK_WIN0(pstParams->win_id))
					{
						PE_P0L_M14A0_QWr01(p0l_tnr_ctrl_00,	tnr_enable, 	0x0);
						PE_P0L_M14A0_QWr03(p0l_dnr_max_ctrl,enable_mnr, 	0x0,\
															enable_dc_bnr, 	0x0,\
															enable_ac_bnr, 	0x0);
					}
					if(PE_CHECK_WIN1(pstParams->win_id))
					{
						PE_P0R_M14A0_QWr01(p0r_tnr_ctrl_00,	tnr_enable, 	0x0);
						PE_P0R_M14A0_QWr03(p0r_dnr_max_ctrl,enable_mnr, 	0x0,\
															enable_dc_bnr, 	0x0,\
															enable_ac_bnr, 	0x0);
					}
				}
				else
				{
					PE_NRD_HW_M14_DBG_PRINT("tnr,dnr : enable.\n");
					if(PE_CHECK_WIN0(pstParams->win_id))
					{
						PE_P0L_M14A0_QWr01(p0l_tnr_ctrl_00,	tnr_enable, 	0x1);
						PE_P0L_M14A0_QWr03(p0l_dnr_max_ctrl,enable_mnr, 	0x1,\
															enable_dc_bnr, 	0x1,\
															enable_ac_bnr, 	0x1);
					}
					if(PE_CHECK_WIN1(pstParams->win_id))
					{
						PE_P0R_M14A0_QWr01(p0r_tnr_ctrl_00,	tnr_enable, 	0x1);
						PE_P0R_M14A0_QWr03(p0r_dnr_max_ctrl,enable_mnr, 	0x1,\
															enable_dc_bnr, 	0x1,\
															enable_ac_bnr, 	0x1);
					}
				}
			}
			else
			{
				PE_NRD_HW_M14_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
			}
		}
	}while(0);
	return ret;
}
/**
 * set nr default setting
 *
 * @param   *pstParams [in] LX_PE_DEFAULT_SETTINGS_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_NRD_HW_M14_SetDefault(LX_PE_DEFAULT_SETTINGS_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_M14BX)
		{
			ret = PE_NRD_HW_M14B_SetDefault(pstParams);
			PE_NRD_HW_M14_CHECK_CODE(ret, break, \
				"[%s,%d] PE_NRD_HW_M14B_SetDefault() error.\n", __F__, __L__);
		}
		else if(PE_KDRV_VER_M14AX)
		{
			ret = PE_NRD_HW_M14A_SetDefault(pstParams);
			PE_NRD_HW_M14_CHECK_CODE(ret, break, \
				"[%s,%d] PE_NRD_HW_M14A_SetDefault() error.\n", __F__, __L__);
		}
		else
		{
			PE_NRD_HW_M14_DBG_PRINT("nothing to do\n");	ret = RET_OK;
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
static int PE_NRD_HW_M14B_GetTnrDfltTable(PE_REG_PARAM_T *dst_dflt, const PE_REG_PARAM_T *src_dflt, UINT32 dflt_size)
{
	int ret = RET_OK;
	UINT32 dflt_num = 0, match_num = 0, i = 0, r_data = 0;
	UINT32 match_size = _g_pe_nrd_hw_m14b_p0m_reg_match.tnr_size;
	PE_REG_PARAM_T *p_match_tbl = _g_pe_nrd_hw_m14b_p0m_reg_match.tnr_mtbl;
	UINT32 *p_reg_data = (UINT32 *)&(_g_pe_nrd_hw_m14b_p0m_reg_ctrl.data);
	UINT32 *p_reg_mask = (UINT32 *)&(_g_pe_nrd_hw_m14b_p0m_reg_ctrl.mask);

	PE_NRD_HW_M14_CHECK_CODE(!dst_dflt, return RET_ERROR, \
		"[%s,%d] dst_dflt is null, not ready.\n", __F__, __L__);
	PE_NRD_HW_M14_CHECK_CODE(!src_dflt, return RET_ERROR, \
		"[%s,%d] src_dflt is null, not ready.\n", __F__, __L__);
	PE_NRD_HW_M14_CHECK_CODE(!match_size, return RET_ERROR, \
		"[%s,%d] match size is zero.\n", __F__, __L__);
	PE_NRD_HW_M14_CHECK_CODE(!p_match_tbl, return RET_ERROR, \
		"[%s,%d] match tbl is null, not ready.\n", __F__, __L__);
	PE_NRD_HW_M14_CHECK_CODE(match_size!=dflt_size, return RET_ERROR, \
		"[%s,%d] size mismatch(%d vs. %d).\n", __F__, __L__, match_size, dflt_size);

	dflt_num = dflt_size/sizeof(PE_REG_PARAM_T);
	for (i=0; i<(dflt_num); i++)
	{
		dst_dflt[i].addr = src_dflt[i].addr;
		match_num = p_match_tbl[i].data;
		if (match_num<PE_NRD_HW_M14B_P0M_ENUM(MAX))
		{
			/* apply tnr_enable by f/w */
			if (src_dflt[i].addr==0x2140)
			{
				PE_P0M_M14B0_RdFL(p0m_tnr_ctrl_00);
				PE_P0M_M14B0_Rd01(p0m_tnr_ctrl_00,tnr_enable,r_data);
				PE_NRD_HW_M14B_P0M_SET(p0m_tnr_ctrl_00,tnr_enable,r_data);
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
static int PE_NRD_HW_M14B_GetDnrDfltTable(PE_REG_PARAM_T *dst_dflt, const PE_REG_PARAM_T *src_dflt, UINT32 dflt_size)
{
	int ret = RET_OK;
	UINT32 dflt_num = 0, match_num = 0, i = 0;
	UINT32 match_size = _g_pe_nrd_hw_m14b_p0d_reg_match.dnr_size;
	PE_REG_PARAM_T *p_match_tbl = _g_pe_nrd_hw_m14b_p0d_reg_match.dnr_mtbl;
	UINT32 *p_reg_data = (UINT32 *)&(_g_pe_nrd_hw_m14b_p0d_reg_ctrl.data);
	UINT32 *p_reg_mask = (UINT32 *)&(_g_pe_nrd_hw_m14b_p0d_reg_ctrl.mask);

	PE_NRD_HW_M14_CHECK_CODE(!dst_dflt, return RET_ERROR, \
		"[%s,%d] dst_dflt is null, not ready.\n", __F__, __L__);
	PE_NRD_HW_M14_CHECK_CODE(!src_dflt, return RET_ERROR, \
		"[%s,%d] src_dflt is null, not ready.\n", __F__, __L__);
	PE_NRD_HW_M14_CHECK_CODE(!match_size, return RET_ERROR, \
		"[%s,%d] match size is zero.\n", __F__, __L__);
	PE_NRD_HW_M14_CHECK_CODE(!p_match_tbl, return RET_ERROR, \
		"[%s,%d] match tbl is null, not ready.\n", __F__, __L__);
	PE_NRD_HW_M14_CHECK_CODE(match_size!=dflt_size, return RET_ERROR, \
		"[%s,%d] size mismatch(%d vs. %d).\n", __F__, __L__, match_size, dflt_size);

	dflt_num = dflt_size/sizeof(PE_REG_PARAM_T);
	for (i=0; i<(dflt_num); i++)
	{
		dst_dflt[i].addr = src_dflt[i].addr;
		match_num = p_match_tbl[i].data;
		if (match_num<PE_NRD_HW_M14B_P0D_ENUM(MAX))
		{
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
 * set nr default setting(for M14BX)
 *
 * @param   *pstParams [in] LX_PE_DEFAULT_SETTINGS_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_NRD_HW_M14B_SetDefault(LX_PE_DEFAULT_SETTINGS_T *pstParams)
{
	int ret = RET_OK;
	PE_FWI_M14_TNR_CTRL fwi_tnr;
	PE_FWI_M14_SETTINGS_T fwi_inf;
	PE_INF_M14_SETTINGS_T inf_set;
	LX_PE_INF_DISPLAY_T *cur0_inf = NULL;
	PE_NRD_HW_M14B_TNR_FMT cur0_tnr_fmt;
	PE_NRD_HW_M14B_TAL_FMT cur0_tal_fmt;
	PE_NRD_HW_M14B_IFC_FMT cur0_ifc_fmt;
	PE_NRD_HW_M14_SETTINGS_T *pInfo = &_g_pe_nrd_hw_m14_info;

	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		ret = PE_FWI_M14_GetCurFwiSettings(&fwi_inf);
		PE_NRD_HW_M14_CHECK_CODE(ret,break,"[%s,%d] PE_FWI_M14_GetCurFwiSettings() error.\n",__F__,__L__);
		ret = PE_INF_M14_GetCurInfSettings(&inf_set);
		PE_NRD_HW_M14_CHECK_CODE(ret,break,"[%s,%d] PE_INF_M14_GetCurInfSettings() error.\n",__F__,__L__);
		cur0_inf=&inf_set.disp_info[LX_PE_WIN_0];
		CHECK_KNULL(cur0_inf);
		/* tnr */
		if(PE_CHECK_WIN0(pstParams->win_id))
		{
			cur0_tnr_fmt = PE_NRD_HW_M14B_ConvDispInfoToTnrFmt(cur0_inf);
			PE_NRD_HW_M14B_DBG_PRINT_TNR_FMT_STATE(pInfo->tnr0b_fmt,cur0_tnr_fmt);
			if(pInfo->tnr0b_fmt!=cur0_tnr_fmt)
			{
				#ifdef PE_HW_M14B_USE_REG_CTRL
				if(PE_KDRV_VER_M14CX)
				{
					switch(cur0_tnr_fmt)
					{
						case PE_NRD_M14B_TNR_HD_I:
							PE_NRD_HW_M14B_TNR_REG_TABLE(tnr_l_hd_i_default_m14c0, PE_REG_M14_WRITE_BASE);
							break;
						case PE_NRD_M14B_TNR_HD_P:
							PE_NRD_HW_M14B_TNR_REG_TABLE(tnr_l_hd_p_default_m14c0, PE_REG_M14_WRITE_BASE);
							break;
						case PE_NRD_M14B_TNR_HD_I_OLED:
							PE_NRD_HW_M14B_TNR_REG_TABLE(tnr_l_hd_i_default_for_oled_m14c0,PE_REG_M14_WRITE_BASE);
							break;
						case PE_NRD_M14B_TNR_HD_P_OLED:
							PE_NRD_HW_M14B_TNR_REG_TABLE(tnr_l_hd_p_default_for_oled_m14c0,PE_REG_M14_WRITE_BASE);
							break;
						case PE_NRD_M14B_TNR_COMP_HD_I_3D:
							PE_NRD_HW_M14B_TNR_REG_TABLE(tnr_l_comp_hd_i_3d_default_m14c0,PE_REG_M14_WRITE_BASE);
							break;
						case PE_NRD_M14B_TNR_COMP_HD_P_3D:
							PE_NRD_HW_M14B_TNR_REG_TABLE(tnr_l_comp_hd_p_3d_default_m14c0,PE_REG_M14_WRITE_BASE);
							break;
						case PE_NRD_M14B_TNR_PC:
							PE_NRD_HW_M14B_TNR_REG_TABLE(tnr_l_pc_default_m14c0, PE_REG_M14_WRITE_BASE);
							break;
						case PE_NRD_M14B_TNR_SD_I:
							PE_NRD_HW_M14B_TNR_REG_TABLE(tnr_l_sd_i_default_m14c0, PE_REG_M14_WRITE_BASE);
							break;
						case PE_NRD_M14B_TNR_DTV_SD_480_I:
							PE_NRD_HW_M14B_TNR_REG_TABLE(tnr_l_dtv_sd_480_i_default_m14c0,PE_REG_M14_WRITE_BASE);
							break;
						case PE_NRD_M14B_TNR_DTV_SD_576_I:
							PE_NRD_HW_M14B_TNR_REG_TABLE(tnr_l_dtv_sd_576_i_default_m14c0,PE_REG_M14_WRITE_BASE);
							break;
						case PE_NRD_M14B_TNR_SD_P:
							PE_NRD_HW_M14B_TNR_REG_TABLE(tnr_l_sd_p_default_m14c0, PE_REG_M14_WRITE_BASE);
							break;
						case PE_NRD_M14B_TNR_DTV_SD_P:
							PE_NRD_HW_M14B_TNR_REG_TABLE(tnr_l_dtv_sd_p_default_m14c0,PE_REG_M14_WRITE_BASE);
							break;
						case PE_NRD_M14B_TNR_AV_NTSC:
							PE_NRD_HW_M14B_TNR_REG_TABLE(tnr_l_av_ntsc_default_m14c0, PE_REG_M14_WRITE_BASE);
							break;
						case PE_NRD_M14B_TNR_AV_PAL:
							PE_NRD_HW_M14B_TNR_REG_TABLE(tnr_l_av_pal_default_m14c0, PE_REG_M14_WRITE_BASE);
							break;
						case PE_NRD_M14B_TNR_AV_SECAM:
							PE_NRD_HW_M14B_TNR_REG_TABLE(tnr_l_av_secam_default_m14c0, PE_REG_M14_WRITE_BASE);
							break;
						default:
							break;
					}
				}
				else
				{
					switch(cur0_tnr_fmt)
					{
						case PE_NRD_M14B_TNR_HD_I:
							PE_NRD_HW_M14B_TNR_REG_TABLE(tnr_l_hd_i_default_m14b0, PE_REG_M14_WRITE_BASE);
							break;
						case PE_NRD_M14B_TNR_HD_P:
							PE_NRD_HW_M14B_TNR_REG_TABLE(tnr_l_hd_p_default_m14b0, PE_REG_M14_WRITE_BASE);
							break;
						case PE_NRD_M14B_TNR_HD_I_OLED:
							PE_NRD_HW_M14B_TNR_REG_TABLE(tnr_l_hd_i_default_for_oled_m14b0,PE_REG_M14_WRITE_BASE);
							break;
						case PE_NRD_M14B_TNR_HD_P_OLED:
							PE_NRD_HW_M14B_TNR_REG_TABLE(tnr_l_hd_p_default_for_oled_m14b0,PE_REG_M14_WRITE_BASE);
							break;
						case PE_NRD_M14B_TNR_COMP_HD_I_3D:
							PE_NRD_HW_M14B_TNR_REG_TABLE(tnr_l_comp_hd_i_3d_default_m14b0,PE_REG_M14_WRITE_BASE);
							break;
						case PE_NRD_M14B_TNR_COMP_HD_P_3D:
							PE_NRD_HW_M14B_TNR_REG_TABLE(tnr_l_comp_hd_p_3d_default_m14b0,PE_REG_M14_WRITE_BASE);
							break;
						case PE_NRD_M14B_TNR_PC:
							PE_NRD_HW_M14B_TNR_REG_TABLE(tnr_l_pc_default_m14b0, PE_REG_M14_WRITE_BASE);
							break;
						case PE_NRD_M14B_TNR_SD_I:
							PE_NRD_HW_M14B_TNR_REG_TABLE(tnr_l_sd_i_default_m14b0, PE_REG_M14_WRITE_BASE);
							break;
						case PE_NRD_M14B_TNR_DTV_SD_480_I:
							PE_NRD_HW_M14B_TNR_REG_TABLE(tnr_l_dtv_sd_480_i_default_m14b0,PE_REG_M14_WRITE_BASE);
							break;
						case PE_NRD_M14B_TNR_DTV_SD_576_I:
							PE_NRD_HW_M14B_TNR_REG_TABLE(tnr_l_dtv_sd_576_i_default_m14b0,PE_REG_M14_WRITE_BASE);
							break;
						case PE_NRD_M14B_TNR_SD_P:
							PE_NRD_HW_M14B_TNR_REG_TABLE(tnr_l_sd_p_default_m14b0, PE_REG_M14_WRITE_BASE);
							break;
						case PE_NRD_M14B_TNR_DTV_SD_P:
							PE_NRD_HW_M14B_TNR_REG_TABLE(tnr_l_dtv_sd_p_default_m14b0,PE_REG_M14_WRITE_BASE);
							break;
						case PE_NRD_M14B_TNR_AV_NTSC:
							PE_NRD_HW_M14B_TNR_REG_TABLE(tnr_l_av_ntsc_default_m14b0, PE_REG_M14_WRITE_BASE);
							break;
						case PE_NRD_M14B_TNR_AV_PAL:
							PE_NRD_HW_M14B_TNR_REG_TABLE(tnr_l_av_pal_default_m14b0, PE_REG_M14_WRITE_BASE);
							break;
						case PE_NRD_M14B_TNR_AV_SECAM:
							PE_NRD_HW_M14B_TNR_REG_TABLE(tnr_l_av_secam_default_m14b0, PE_REG_M14_WRITE_BASE);
							break;
						default:
							break;
					}
				}
				#else
				switch(cur0_tnr_fmt)
				{
					case PE_NRD_M14B_TNR_HD_I:
						PE_SET_REG_TABLE(NRD_HW_M14,tnr_l_hd_i_default_m14b0,	PE_REG_M14_WRITE_BASE);
						break;
					case PE_NRD_M14B_TNR_HD_P:
						PE_SET_REG_TABLE(NRD_HW_M14,tnr_l_hd_p_default_m14b0,	PE_REG_M14_WRITE_BASE);
						break;
					case PE_NRD_M14B_TNR_HD_I_OLED:
						PE_SET_REG_TABLE(NRD_HW_M14,tnr_l_hd_i_default_for_oled_m14b0,PE_REG_M14_WRITE_BASE);
						break;
					case PE_NRD_M14B_TNR_HD_P_OLED:
						PE_SET_REG_TABLE(NRD_HW_M14,tnr_l_hd_p_default_for_oled_m14b0,PE_REG_M14_WRITE_BASE);
						break;
					case PE_NRD_M14B_TNR_COMP_HD_I_3D:
						PE_SET_REG_TABLE(NRD_HW_M14,tnr_l_comp_hd_i_3d_default_m14b0,PE_REG_M14_WRITE_BASE);
						break;
					case PE_NRD_M14B_TNR_COMP_HD_P_3D:
						PE_SET_REG_TABLE(NRD_HW_M14,tnr_l_comp_hd_p_3d_default_m14b0,PE_REG_M14_WRITE_BASE);
						break;
					case PE_NRD_M14B_TNR_PC:
						PE_SET_REG_TABLE(NRD_HW_M14,tnr_l_pc_default_m14b0,		PE_REG_M14_WRITE_BASE);
						break;
					case PE_NRD_M14B_TNR_SD_I:
						PE_SET_REG_TABLE(NRD_HW_M14,tnr_l_sd_i_default_m14b0,	PE_REG_M14_WRITE_BASE);
						break;
					case PE_NRD_M14B_TNR_DTV_SD_480_I:
						PE_SET_REG_TABLE(NRD_HW_M14,tnr_l_dtv_sd_480_i_default_m14b0,PE_REG_M14_WRITE_BASE);
						break;
					case PE_NRD_M14B_TNR_DTV_SD_576_I:
						PE_SET_REG_TABLE(NRD_HW_M14,tnr_l_dtv_sd_576_i_default_m14b0,PE_REG_M14_WRITE_BASE);
						break;
					case PE_NRD_M14B_TNR_SD_P:
						PE_SET_REG_TABLE(NRD_HW_M14,tnr_l_sd_p_default_m14b0,	PE_REG_M14_WRITE_BASE);
						break;
					case PE_NRD_M14B_TNR_DTV_SD_P:
						PE_SET_REG_TABLE(NRD_HW_M14,tnr_l_dtv_sd_p_default_m14b0,PE_REG_M14_WRITE_BASE);
						break;
					case PE_NRD_M14B_TNR_AV_NTSC:
						PE_SET_REG_TABLE(NRD_HW_M14,tnr_l_av_ntsc_default_m14b0, PE_REG_M14_WRITE_BASE);
						break;
					case PE_NRD_M14B_TNR_AV_PAL:
						PE_SET_REG_TABLE(NRD_HW_M14,tnr_l_av_pal_default_m14b0, PE_REG_M14_WRITE_BASE);
						break;
					case PE_NRD_M14B_TNR_AV_SECAM:
						PE_SET_REG_TABLE(NRD_HW_M14,tnr_l_av_secam_default_m14b0, PE_REG_M14_WRITE_BASE);
						break;
					default:
						break;
				}
				#endif
				pInfo->tnr0b_fmt=cur0_tnr_fmt;
			}
			/* fwi tnr ctrl */
			fwi_tnr.tnr_en = 1;
			fwi_tnr.m_fir_en = fwi_inf.tnr_ctrl[LX_PE_WIN_0].m_fir_en;
			/* sync with default, if sd or hd&interlace, turn on m_fir_en. otherwise,off */
			/* 20130511, sj.youm, add ext_frc,3dfmt case(such as ud model), always turn on m_fir_en. */
			/* 20130605, sj.youm, off m_fir_en for ud input for ext_frc,3dfmt case(such as ud model) */
			if(cur0_inf->mode.is_ext_frc)	/* external frc */
			{
				if(cur0_inf->out_type==LX_PE_OUT_UD)
					fwi_tnr.m_fir_en = 0;
				else
					fwi_tnr.m_fir_en = 1;
			}
			else	/* internal frc */
			{
				switch(cur0_inf->src_type)
				{
					case LX_PE_SRC_ATV:
					case LX_PE_SRC_CVBS:
					case LX_PE_SRC_SCART:
						fwi_tnr.m_fir_en = 1;
						break;
					case LX_PE_SRC_COMP:
					case LX_PE_SRC_DTV:
					case LX_PE_SRC_HDMI:
					case LX_PE_SRC_RGBPC:
					default:
						if(cur0_inf->fmt_type==LX_PE_FMT_SD)
						{
							fwi_tnr.m_fir_en = 1;
						}
						else
						{
							if(cur0_inf->scan_type==LX_PE_SCAN_INTERLACE)
								fwi_tnr.m_fir_en = 1;
							else
								fwi_tnr.m_fir_en = 0;
						}
						break;
				}
			}
			ret = PE_FWI_M14_SetTnrCtrl(LX_PE_WIN_0,&fwi_tnr);
			PE_NRD_HW_M14_CHECK_CODE(ret,break,"[%s,%d] PE_FWI_M14_SetTnrCtrl() error.\n",__F__,__L__);
			PE_NRD_HW_M14_DBG_PRINT("set[0]tnr_en:%d,m_fir_en:%d\n",fwi_tnr.tnr_en,fwi_tnr.m_fir_en);
		}
		/* tnr alpha lut */
		if(PE_CHECK_WIN0(pstParams->win_id))
		{
			cur0_tal_fmt = PE_NRD_HW_M14B_ConvDispInfoToTalFmt(cur0_inf);
			PE_NRD_HW_M14B_DBG_PRINT_TAL_FMT_STATE(pInfo->tal0b_fmt,cur0_tal_fmt);
			if(pInfo->tal0b_fmt!=cur0_tal_fmt)
			{
				switch(cur0_tal_fmt)
				{
					case PE_NRD_M14B_TAL_HD_I:
						PE_SET_REG_TABLE(NRD_HW_M14,tnr_alphalut_l_hd_i_default_m14b0,	PE_REG_M14B_WRITE_BASE);
						break;
					case PE_NRD_M14B_TAL_HD_P:
						PE_SET_REG_TABLE(NRD_HW_M14,tnr_alphalut_l_hd_p_default_m14b0,	PE_REG_M14B_WRITE_BASE);
						break;
					case PE_NRD_M14B_TAL_SD_I:
						PE_SET_REG_TABLE(NRD_HW_M14,tnr_alphalut_l_sd_i_default_m14b0,	PE_REG_M14B_WRITE_BASE);
						break;
					case PE_NRD_M14B_TAL_SD_P:
						PE_SET_REG_TABLE(NRD_HW_M14,tnr_alphalut_l_sd_p_default_m14b0,	PE_REG_M14B_WRITE_BASE);
						break;
					case PE_NRD_M14B_TAL_SECAM:
						PE_SET_REG_TABLE(NRD_HW_M14,tnr_alphalut_l_secam_default_m14b0,	PE_REG_M14B_WRITE_BASE);
						break;
					default:
						break;
				}
				pInfo->tal0b_fmt=cur0_tal_fmt;
			}
		}
		/* dnr */
		if(PE_CHECK_WIN0(pstParams->win_id))
		{
			if(PE_KDRV_VER_M14CX)
			{
				PE_NRD_HW_M14C_DNR_FMT cur0_dnr_fmt;
				cur0_dnr_fmt = PE_NRD_HW_M14C_ConvDispInfoToDnrFmt(cur0_inf);
				PE_NRD_HW_M14C_DBG_PRINT_DNR_FMT_STATE(pInfo->dnr0c_fmt,cur0_dnr_fmt);
				if(pInfo->dnr0c_fmt!=cur0_dnr_fmt)
				{
					#ifdef PE_HW_M14C_USE_REG_CTRL
					switch(cur0_dnr_fmt)
					{
						case PE_NRD_M14C_DNR_ATV:
							PE_NRD_HW_M14B_DNR_REG_TABLE(dnr_l_atv_default_m14c0,PE_REG_M14B_WRITE_BASE);
							break;
						case PE_NRD_M14C_DNR_AV:
							PE_NRD_HW_M14B_DNR_REG_TABLE(dnr_l_av_default_m14c0,PE_REG_M14B_WRITE_BASE);
							break;
						case PE_NRD_M14C_DNR_HD_I:
							PE_NRD_HW_M14B_DNR_REG_TABLE(dnr_l_hd_i_default_m14c0,PE_REG_M14B_WRITE_BASE);
							break;
						case PE_NRD_M14C_DNR_HD_P:
							PE_NRD_HW_M14B_DNR_REG_TABLE(dnr_l_hd_p_default_m14c0,PE_REG_M14B_WRITE_BASE);
							break;
						case PE_NRD_M14C_DNR_PC:
							PE_NRD_HW_M14B_DNR_REG_TABLE(dnr_l_pc_default_m14c0,PE_REG_M14B_WRITE_BASE);
							break;
						case PE_NRD_M14C_DNR_SD_I:
							PE_NRD_HW_M14B_DNR_REG_TABLE(dnr_l_sd_i_default_m14c0,PE_REG_M14B_WRITE_BASE);
							break;
						case PE_NRD_M14C_DNR_SD_P:
							PE_NRD_HW_M14B_DNR_REG_TABLE(dnr_l_sd_p_default_m14c0,PE_REG_M14B_WRITE_BASE);
							break;
						default:
							break;
					}
					#else
					switch(cur0_dnr_fmt)
					{
						case PE_NRD_M14C_DNR_ATV:
							PE_SET_REG_TABLE(NRD_HW_M14,dnr_l_atv_default_m14c0,PE_REG_M14B_WRITE_BASE);
							break;
						case PE_NRD_M14C_DNR_AV:
							PE_SET_REG_TABLE(NRD_HW_M14,dnr_l_av_default_m14c0,	PE_REG_M14B_WRITE_BASE);
							break;
						case PE_NRD_M14C_DNR_HD_I:
							PE_SET_REG_TABLE(NRD_HW_M14,dnr_l_hd_i_default_m14c0,	PE_REG_M14B_WRITE_BASE);
							break;
						case PE_NRD_M14C_DNR_HD_P:
							PE_SET_REG_TABLE(NRD_HW_M14,dnr_l_hd_p_default_m14c0,	PE_REG_M14B_WRITE_BASE);
							break;
						case PE_NRD_M14C_DNR_PC:
							PE_SET_REG_TABLE(NRD_HW_M14,dnr_l_pc_default_m14c0,	PE_REG_M14B_WRITE_BASE);
							break;
						case PE_NRD_M14C_DNR_SD_I:
							PE_SET_REG_TABLE(NRD_HW_M14,dnr_l_sd_i_default_m14c0,	PE_REG_M14B_WRITE_BASE);
							break;
						case PE_NRD_M14C_DNR_SD_P:
							PE_SET_REG_TABLE(NRD_HW_M14,dnr_l_sd_p_default_m14c0,	PE_REG_M14B_WRITE_BASE);
							break;
						default:
							break;
					}
					#endif
					pInfo->dnr0c_fmt=cur0_dnr_fmt;
				}
			}
			else
			{
				PE_NRD_HW_M14B_DNR_FMT cur0_dnr_fmt;
				cur0_dnr_fmt = PE_NRD_HW_M14B_ConvDispInfoToDnrFmt(cur0_inf);
				PE_NRD_HW_M14B_DBG_PRINT_DNR_FMT_STATE(pInfo->dnr0b_fmt,cur0_dnr_fmt);
				if(pInfo->dnr0b_fmt!=cur0_dnr_fmt)
				{
					#ifdef PE_HW_M14B_USE_REG_CTRL
					switch(cur0_dnr_fmt)
					{
						case PE_NRD_M14B_DNR_ATV:
							PE_NRD_HW_M14B_DNR_REG_TABLE(dnr_l_atv_default_m14b0,PE_REG_M14B_WRITE_BASE);
							break;
						case PE_NRD_M14B_DNR_AV:
							PE_NRD_HW_M14B_DNR_REG_TABLE(dnr_l_av_default_m14b0,PE_REG_M14B_WRITE_BASE);
							break;
						case PE_NRD_M14B_DNR_HD:
							PE_NRD_HW_M14B_DNR_REG_TABLE(dnr_l_hd_default_m14b0,PE_REG_M14B_WRITE_BASE);
							break;
						case PE_NRD_M14B_DNR_PC:
							PE_NRD_HW_M14B_DNR_REG_TABLE(dnr_l_pc_default_m14b0,PE_REG_M14B_WRITE_BASE);
							break;
						case PE_NRD_M14B_DNR_SD:
							PE_NRD_HW_M14B_DNR_REG_TABLE(dnr_l_sd_default_m14b0,PE_REG_M14B_WRITE_BASE);
							break;
						default:
							break;
					}
					#else
					switch(cur0_dnr_fmt)
					{
						case PE_NRD_M14B_DNR_ATV:
							PE_SET_REG_TABLE(NRD_HW_M14,dnr_l_atv_default_m14b0,PE_REG_M14B_WRITE_BASE);
							break;
						case PE_NRD_M14B_DNR_AV:
							PE_SET_REG_TABLE(NRD_HW_M14,dnr_l_av_default_m14b0,	PE_REG_M14B_WRITE_BASE);
							break;
						case PE_NRD_M14B_DNR_HD:
							PE_SET_REG_TABLE(NRD_HW_M14,dnr_l_hd_default_m14b0,	PE_REG_M14B_WRITE_BASE);
							break;
						case PE_NRD_M14B_DNR_PC:
							PE_SET_REG_TABLE(NRD_HW_M14,dnr_l_pc_default_m14b0,	PE_REG_M14B_WRITE_BASE);
							break;
						case PE_NRD_M14B_DNR_SD:
							PE_SET_REG_TABLE(NRD_HW_M14,dnr_l_sd_default_m14b0,	PE_REG_M14B_WRITE_BASE);
							break;
						default:
							break;
					}
					#endif
					pInfo->dnr0b_fmt=cur0_dnr_fmt;
				}
			}
		}
		/* ifc */
		if(PE_CHECK_WIN0(pstParams->win_id))
		{
			cur0_ifc_fmt = PE_NRD_HW_M14B_ConvDispInfoToIfcFmt(cur0_inf);
			PE_NRD_HW_M14B_DBG_PRINT_IFC_FMT_STATE(pInfo->ifc0b_fmt,cur0_ifc_fmt);
			if(pInfo->ifc0b_fmt!=cur0_ifc_fmt)
			{
				switch(cur0_ifc_fmt)
				{
					case PE_NRD_M14B_IFC_ATV_NTSC:
						PE_SET_REG_TABLE(NRD_HW_M14,ifc_l_atv_ntsc_default_m14b0,	PE_REG_M14B_WRITE_BASE);
						break;
					case PE_NRD_M14B_IFC_PC:
						PE_SET_REG_TABLE(NRD_HW_M14,ifc_l_pc_default_m14b0,			PE_REG_M14B_WRITE_BASE);
						break;
					default:
						break;
				}
				pInfo->ifc0b_fmt=cur0_ifc_fmt;
			}
		}
	}while(0);
	return ret;
}
/**
 * set nr default setting(for M14AX)
 *
 * @param   *pstParams [in] LX_PE_DEFAULT_SETTINGS_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_NRD_HW_M14A_SetDefault(LX_PE_DEFAULT_SETTINGS_T *pstParams)
{
	int ret = RET_OK;
	PE_FWI_M14_TNR_CTRL fwi_tnr;
	PE_FWI_M14_SETTINGS_T fwi_inf;
	PE_INF_M14_SETTINGS_T inf_set;
	LX_PE_INF_DISPLAY_T *cur0_inf = NULL;
	LX_PE_INF_DISPLAY_T *cur1_inf = NULL;
	PE_NRD_HW_M14A_TNR_FMT cur0_tnr_fmt,cur1_tnr_fmt;
	PE_NRD_HW_M14A_DNR_FMT cur0_dnr_fmt,cur1_dnr_fmt;
	PE_NRD_HW_M14_SETTINGS_T *pInfo = &_g_pe_nrd_hw_m14_info;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		ret = PE_FWI_M14_GetCurFwiSettings(&fwi_inf);
		PE_NRD_HW_M14_CHECK_CODE(ret,break,"[%s,%d] PE_FWI_M14_GetCurFwiSettings() error.\n",__F__,__L__);
		ret = PE_INF_M14_GetCurInfSettings(&inf_set);
		PE_NRD_HW_M14_CHECK_CODE(ret,break,"[%s,%d] PE_INF_M14_GetCurInfSettings() error.\n",__F__,__L__);
		cur0_inf=&inf_set.disp_info[LX_PE_WIN_0];
		cur1_inf=&inf_set.disp_info[LX_PE_WIN_1];
		CHECK_KNULL(cur0_inf);
		CHECK_KNULL(cur1_inf);
		/* L : tnr */
		if(PE_CHECK_WIN0(pstParams->win_id))
		{
			cur0_tnr_fmt = PE_NRD_HW_M14A_ConvDispInfoToTnrFmt(cur0_inf);
			PE_NRD_HW_M14A_DBG_PRINT_TNR_FMT_STATE(pInfo->tnr0a_fmt,cur0_tnr_fmt);
			if(pInfo->tnr0a_fmt!=cur0_tnr_fmt)
			{
				switch(cur0_tnr_fmt)
				{
					case PE_NRD_M14A_TNR_SD:
						PE_SET_REG_TABLE(NRD_HW_M14,tnr_l_sd_default_m14a0,			PE_REG_M14_WRITE_BASE);
						break;
					case PE_NRD_M14A_TNR_SD_MC_OFF:
						PE_SET_REG_TABLE(NRD_HW_M14,tnr_l_sd_mc_off_default_m14a0,	PE_REG_M14_WRITE_BASE);
						break;
					case PE_NRD_M14A_TNR_ATV:
						PE_SET_REG_TABLE(NRD_HW_M14,tnr_l_atv_default_m14a0,		PE_REG_M14_WRITE_BASE);
						break;
					case PE_NRD_M14A_TNR_DVR_ATV:
						PE_SET_REG_TABLE(NRD_HW_M14,tnr_l_dvr_atv_default_m14a0,	PE_REG_M14_WRITE_BASE);
						break;
					case PE_NRD_M14A_TNR_ATV_SECAM:
						PE_SET_REG_TABLE(NRD_HW_M14,tnr_l_atv_secam_default_m14a0,	PE_REG_M14_WRITE_BASE);
						break;
					case PE_NRD_M14A_TNR_AV:
						PE_SET_REG_TABLE(NRD_HW_M14,tnr_l_av_default_m14a0,			PE_REG_M14_WRITE_BASE);
						break;
					case PE_NRD_M14A_TNR_AV_SECAM:
						PE_SET_REG_TABLE(NRD_HW_M14,tnr_l_av_secam_default_m14a0,	PE_REG_M14_WRITE_BASE);
						break;
					case PE_NRD_M14A_TNR_HD:
						if(cur0_inf->mode.is_ext_frc)	/* external frc,3dfmt such as udtv */
						{
							PE_SET_REG_TABLE(NRD_HW_M14,tnr_l_hd_default_for_udtv_m14a0,PE_REG_M14_WRITE_BASE);
						}
						else	/* internal frc,3dfmt */
						{
							PE_SET_REG_TABLE(NRD_HW_M14,tnr_l_hd_default_m14a0,			PE_REG_M14_WRITE_BASE);
						}
						break;
					case PE_NRD_M14A_TNR_UD:
						PE_SET_REG_TABLE(NRD_HW_M14,tnr_l_ud_default_m14a0,			PE_REG_M14_WRITE_BASE);
						break;
					default:
						break;
				}
				pInfo->tnr0a_fmt=cur0_tnr_fmt;
			}
			/* fwi tnr ctrl */
			fwi_tnr.tnr_en = 1;
			fwi_tnr.m_fir_en = fwi_inf.tnr_ctrl[LX_PE_WIN_0].m_fir_en;
			/* sync with default, if sd or hd&interlace, turn on m_fir_en. otherwise,off */
			/* 20130511, sj.youm, add ext_frc,3dfmt case(such as ud model), always turn on m_fir_en. */
			/* 20130605, sj.youm, off m_fir_en for ud input for ext_frc,3dfmt case(such as ud model) */
			if(cur0_inf->mode.is_ext_frc)	/* external frc */
			{
				if(cur0_inf->out_type==LX_PE_OUT_UD)
					fwi_tnr.m_fir_en = 0;
				else
					fwi_tnr.m_fir_en = 1;
			}
			else	/* internal frc */
			{
				switch(cur0_inf->src_type)
				{
					case LX_PE_SRC_ATV:
					case LX_PE_SRC_CVBS:
					case LX_PE_SRC_SCART:
						fwi_tnr.m_fir_en = 1;
						break;
					case LX_PE_SRC_COMP:
					case LX_PE_SRC_DTV:
					case LX_PE_SRC_HDMI:
					case LX_PE_SRC_RGBPC:
					default:
						if(cur0_inf->fmt_type==LX_PE_FMT_SD)
						{
							fwi_tnr.m_fir_en = 1;
						}
						else
						{
							if(cur0_inf->scan_type==LX_PE_SCAN_INTERLACE)
								fwi_tnr.m_fir_en = 1;
							else
								fwi_tnr.m_fir_en = 0;
						}
						break;
				}
			}
			ret = PE_FWI_M14_SetTnrCtrl(LX_PE_WIN_0,&fwi_tnr);
			PE_NRD_HW_M14_CHECK_CODE(ret,break,"[%s,%d] PE_FWI_M14_SetTnrCtrl() error.\n",__F__,__L__);
			PE_NRD_HW_M14_DBG_PRINT("set[0]tnr_en:%d,m_fir_en:%d\n",fwi_tnr.tnr_en,fwi_tnr.m_fir_en);
		}
		/* R : tnr */
		if(PE_CHECK_WIN1(pstParams->win_id))
		{
			cur1_tnr_fmt = PE_NRD_HW_M14A_ConvDispInfoToTnrFmt(cur1_inf);
			PE_NRD_HW_M14A_DBG_PRINT_TNR_FMT_STATE(pInfo->tnr1a_fmt,cur1_tnr_fmt);
			if(pInfo->tnr1a_fmt!=cur1_tnr_fmt)
			{
				switch(cur1_tnr_fmt)
				{
					case PE_NRD_M14A_TNR_SD:
						PE_SET_REG_TABLE(NRD_HW_M14,tnr_l_sd_default_m14a0,			PE_PE0_REG_M14_LRDIFF);
						break;
					case PE_NRD_M14A_TNR_SD_MC_OFF:
						PE_SET_REG_TABLE(NRD_HW_M14,tnr_l_sd_mc_off_default_m14a0,	PE_PE0_REG_M14_LRDIFF);
						break;
					case PE_NRD_M14A_TNR_ATV:
						PE_SET_REG_TABLE(NRD_HW_M14,tnr_l_atv_default_m14a0,		PE_PE0_REG_M14_LRDIFF);
						break;
					case PE_NRD_M14A_TNR_DVR_ATV:
						PE_SET_REG_TABLE(NRD_HW_M14,tnr_l_dvr_atv_default_m14a0,	PE_PE0_REG_M14_LRDIFF);
						break;
					case PE_NRD_M14A_TNR_ATV_SECAM:
						PE_SET_REG_TABLE(NRD_HW_M14,tnr_l_atv_secam_default_m14a0,	PE_PE0_REG_M14_LRDIFF);
						break;
					case PE_NRD_M14A_TNR_AV:
						PE_SET_REG_TABLE(NRD_HW_M14,tnr_l_av_default_m14a0,			PE_PE0_REG_M14_LRDIFF);
						break;
					case PE_NRD_M14A_TNR_AV_SECAM:
						PE_SET_REG_TABLE(NRD_HW_M14,tnr_l_av_secam_default_m14a0,	PE_PE0_REG_M14_LRDIFF);
						break;
					case PE_NRD_M14A_TNR_HD:
						if(cur1_inf->mode.is_ext_frc)	/* external frc,3dfmt such as udtv */
						{
							PE_SET_REG_TABLE(NRD_HW_M14,tnr_l_hd_default_for_udtv_m14a0,PE_PE0_REG_M14_LRDIFF);
						}
						else	/* internal frc,3dfmt */
						{
							PE_SET_REG_TABLE(NRD_HW_M14,tnr_l_hd_default_m14a0,			PE_PE0_REG_M14_LRDIFF);
						}
						break;
					case PE_NRD_M14A_TNR_UD:
						PE_SET_REG_TABLE(NRD_HW_M14,tnr_l_ud_default_m14a0,			PE_PE0_REG_M14_LRDIFF);
						break;
					default:
						break;
				}
				pInfo->tnr1a_fmt=cur1_tnr_fmt;
			}
			/* fwi tnr ctrl */
			fwi_tnr.tnr_en = 1;
			fwi_tnr.m_fir_en = fwi_inf.tnr_ctrl[LX_PE_WIN_1].m_fir_en;
			/* sync with default, if sd or hd&interlace, turn on m_fir_en. otherwise,off */
			/* 20130511, sj.youm, add ext_frc,3dfmt case(such as ud model), always turn on m_fir_en. */
			/* 20130605, sj.youm, off m_fir_en for ud input for ext_frc,3dfmt case(such as ud model) */
			if(cur1_inf->mode.is_ext_frc)	/* external frc */
			{
				if(cur1_inf->out_type==LX_PE_OUT_UD)
					fwi_tnr.m_fir_en = 0;
				else
					fwi_tnr.m_fir_en = 1;
			}
			else	/* internal frc */
			{
				switch(cur1_inf->src_type)
				{
					case LX_PE_SRC_ATV:
					case LX_PE_SRC_CVBS:
					case LX_PE_SRC_SCART:
						fwi_tnr.m_fir_en = 1;
						break;
					case LX_PE_SRC_COMP:
					case LX_PE_SRC_DTV:
					case LX_PE_SRC_HDMI:
					case LX_PE_SRC_RGBPC:
					default:
						if(cur1_inf->fmt_type==LX_PE_FMT_SD)
						{
							fwi_tnr.m_fir_en = 1;
						}
						else
						{
							if(cur1_inf->scan_type==LX_PE_SCAN_INTERLACE)
								fwi_tnr.m_fir_en = 1;
							else
								fwi_tnr.m_fir_en = 0;
						}
						break;
				}
			}
			ret = PE_FWI_M14_SetTnrCtrl(LX_PE_WIN_1,&fwi_tnr);
			PE_NRD_HW_M14_CHECK_CODE(ret,break,"[%s,%d] PE_FWI_M14_SetTnrCtrl() error.\n",__F__,__L__);
			PE_NRD_HW_M14_DBG_PRINT("set[1]tnr_en:%d,m_fir_en:%d\n",fwi_tnr.tnr_en,fwi_tnr.m_fir_en);
		}
		/* L : dnr */
		if(PE_CHECK_WIN0(pstParams->win_id))
		{
			cur0_dnr_fmt = PE_NRD_HW_M14A_ConvDispInfoToDnrFmt(cur0_inf);
			PE_NRD_HW_M14A_DBG_PRINT_DNR_FMT_STATE(pInfo->dnr0a_fmt,cur0_dnr_fmt);
			if(pInfo->dnr0a_fmt!=cur0_dnr_fmt)
			{
				switch(cur0_dnr_fmt)
				{
					case PE_NRD_M14A_DNR_SD:
						PE_SET_REG_TABLE(NRD_HW_M14,dnr_sd_l_default_m14a0,		PE_REG_M14_WRITE_BASE);
						break;
					case PE_NRD_M14A_DNR_HD:
						PE_SET_REG_TABLE(NRD_HW_M14,dnr_hd_l_default_m14a0,		PE_REG_M14_WRITE_BASE);
						break;
					case PE_NRD_M14A_DNR_TP:
						PE_SET_REG_TABLE(NRD_HW_M14,dnr_tp_l_default_m14a0,		PE_REG_M14_WRITE_BASE);
						break;
					default:
						break;
				}
				pInfo->dnr0a_fmt=cur0_dnr_fmt;
			}
		}
		/* R : dnr */
		if(PE_CHECK_WIN1(pstParams->win_id))
		{
			cur1_dnr_fmt = PE_NRD_HW_M14A_ConvDispInfoToDnrFmt(cur1_inf);
			PE_NRD_HW_M14A_DBG_PRINT_DNR_FMT_STATE(pInfo->dnr1a_fmt,cur1_dnr_fmt);
			if(pInfo->dnr1a_fmt!=cur1_dnr_fmt)
			{
				switch(cur1_dnr_fmt)
				{
					case PE_NRD_M14A_DNR_SD:
						PE_SET_REG_TABLE(NRD_HW_M14,dnr_sd_l_default_m14a0,		PE_PE0_REG_M14_LRDIFF);
						break;
					case PE_NRD_M14A_DNR_HD:
						PE_SET_REG_TABLE(NRD_HW_M14,dnr_hd_l_default_m14a0,		PE_PE0_REG_M14_LRDIFF);
						break;
					case PE_NRD_M14A_DNR_TP:
						PE_SET_REG_TABLE(NRD_HW_M14,dnr_tp_l_default_m14a0,		PE_PE0_REG_M14_LRDIFF);
						break;
					default:
						break;
				}
				pInfo->dnr1a_fmt=cur1_dnr_fmt;
			}
		}
	}while(0);
	return ret;
}
/**
 * get tnr fmt from display info.(for M14BX)
 *
 * @param   *disp_inf [in] LX_PE_INF_DISPLAY_T
 * @return  PE_NRD_HW_M14B_TNR_FMT
 * @see
 * @author
 */
static PE_NRD_HW_M14B_TNR_FMT PE_NRD_HW_M14B_ConvDispInfoToTnrFmt(LX_PE_INF_DISPLAY_T *disp_inf)
{
	PE_NRD_HW_M14B_TNR_FMT tnr_fmt = PE_NRD_M14B_TNR_NUM;
	switch(disp_inf->src_type)
	{
		case LX_PE_SRC_ATV:
		case LX_PE_SRC_CVBS:
		case LX_PE_SRC_SCART:
			if(disp_inf->cstd_type == LX_PE_CSTD_NTSC)
			{
				tnr_fmt = PE_NRD_M14B_TNR_AV_NTSC;
			}
			else if(disp_inf->cstd_type == LX_PE_CSTD_SECAM)
			{
				tnr_fmt = PE_NRD_M14B_TNR_AV_SECAM;
			}
			else
			{
				tnr_fmt = PE_NRD_M14B_TNR_AV_PAL;
			}
			break;
		case LX_PE_SRC_RGBPC:
			{
				tnr_fmt = PE_NRD_M14B_TNR_PC;
			}
			break;
		case LX_PE_SRC_DTV:
		case LX_PE_SRC_COMP:
		case LX_PE_SRC_HDMI:
		default:
			if(disp_inf->src_type==LX_PE_SRC_HDMI && \
				disp_inf->hdmi_type==LX_PE_HDMI_PC)
			{
				tnr_fmt = PE_NRD_M14B_TNR_PC;
			}
			else if(disp_inf->fmt_type==LX_PE_FMT_SD)
			{
				if(disp_inf->scan_type==LX_PE_SCAN_INTERLACE)
				{
					if (disp_inf->src_type==LX_PE_SRC_DTV)
					{
						if (disp_inf->fr_type==LX_PE_FR_60HZ)
						{
							tnr_fmt = PE_NRD_M14B_TNR_DTV_SD_480_I;
						}
						else
						{
							tnr_fmt = PE_NRD_M14B_TNR_DTV_SD_576_I;
						}
					}
					else
					{
						tnr_fmt = PE_NRD_M14B_TNR_SD_I;
					}
				}
				else
				{
					if (disp_inf->src_type==LX_PE_SRC_DTV)
					{
						tnr_fmt = PE_NRD_M14B_TNR_DTV_SD_P;
					}
					else
					{
						tnr_fmt = PE_NRD_M14B_TNR_SD_P;
					}
				}
			}
			else
			{
				if(disp_inf->src_type==LX_PE_SRC_COMP)
				{
					/* for 2dto3d and 3d */
					if(disp_inf->out_type==LX_PE_OUT_2D_3D || \
						disp_inf->out_type==LX_PE_OUT_UD || \
						disp_inf->out_type==LX_PE_OUT_TB || \
						disp_inf->out_type==LX_PE_OUT_SS || \
						disp_inf->out_type==LX_PE_OUT_FS || \
						disp_inf->out_type==LX_PE_OUT_DUAL_TB || \
						disp_inf->out_type==LX_PE_OUT_DUAL_SS || \
						disp_inf->out_type==LX_PE_OUT_DUAL_FULL)
					{
						if(disp_inf->scan_type==LX_PE_SCAN_INTERLACE)
						{
							tnr_fmt = PE_NRD_M14B_TNR_COMP_HD_I_3D;
						}
						else
						{
							tnr_fmt = PE_NRD_M14B_TNR_COMP_HD_P_3D;
						}
					}
					else
					{
						if(disp_inf->scan_type==LX_PE_SCAN_INTERLACE)
						{
							tnr_fmt = (disp_inf->mode.is_oled)? \
								PE_NRD_M14B_TNR_HD_I_OLED:PE_NRD_M14B_TNR_HD_I;
						}
						else
						{
							tnr_fmt = (disp_inf->mode.is_oled)? \
								PE_NRD_M14B_TNR_HD_P_OLED:PE_NRD_M14B_TNR_HD_P;
						}
					}
				}
				else
				{
					if(disp_inf->scan_type==LX_PE_SCAN_INTERLACE)
					{
						tnr_fmt = (disp_inf->mode.is_oled)? \
							PE_NRD_M14B_TNR_HD_I_OLED:PE_NRD_M14B_TNR_HD_I;
					}
					else
					{
						tnr_fmt = (disp_inf->mode.is_oled)? \
							PE_NRD_M14B_TNR_HD_P_OLED:PE_NRD_M14B_TNR_HD_P;
					}
				}
			}
			break;
	}
	return tnr_fmt;
}
/**
 * get tnr alpha lut fmt from display info.(for M14BX)
 *
 * @param   *disp_inf [in] LX_PE_INF_DISPLAY_T
 * @return  PE_NRD_HW_M14B_TAL_FMT
 * @see
 * @author
 */
static PE_NRD_HW_M14B_TAL_FMT PE_NRD_HW_M14B_ConvDispInfoToTalFmt(LX_PE_INF_DISPLAY_T *disp_inf)
{
	PE_NRD_HW_M14B_TAL_FMT tal_fmt = PE_NRD_M14B_TAL_NUM;
	switch(disp_inf->src_type)
	{
		case LX_PE_SRC_ATV:
		case LX_PE_SRC_CVBS:
		case LX_PE_SRC_SCART:
			if(disp_inf->cstd_type == LX_PE_CSTD_SECAM)
			{
				tal_fmt = PE_NRD_M14B_TAL_SECAM;
			}
			else
			{
				tal_fmt = PE_NRD_M14B_TAL_SD_I;
			}
			break;
		case LX_PE_SRC_RGBPC:
			{
				tal_fmt = PE_NRD_M14B_TAL_SD_I;
			}
			break;
		case LX_PE_SRC_DTV:
		case LX_PE_SRC_COMP:
		case LX_PE_SRC_HDMI:
		default:
			if(disp_inf->fmt_type==LX_PE_FMT_SD)
			{
				if(disp_inf->scan_type==LX_PE_SCAN_INTERLACE)
				{
					tal_fmt = PE_NRD_M14B_TAL_SD_I;
				}
				else
				{
					tal_fmt = PE_NRD_M14B_TAL_SD_P;
				}
			}
			else
			{
				if(disp_inf->scan_type==LX_PE_SCAN_INTERLACE)
				{
					tal_fmt = PE_NRD_M14B_TAL_HD_I;
				}
				else
				{
					tal_fmt = PE_NRD_M14B_TAL_HD_P;
				}
			}
			break;
	}
	return tal_fmt;
}
/**
 * get dnr fmt from display info.(for M14CX)
 *
 * @param   *disp_inf [in] LX_PE_INF_DISPLAY_T
 * @return  PE_NRD_HW_M14C_DNR_FMT
 * @see
 * @author
 */
static PE_NRD_HW_M14C_DNR_FMT PE_NRD_HW_M14C_ConvDispInfoToDnrFmt(LX_PE_INF_DISPLAY_T *disp_inf)
{
	PE_NRD_HW_M14C_DNR_FMT dnr_fmt = PE_NRD_M14C_DNR_NUM;
	switch(disp_inf->src_type)
	{
		case LX_PE_SRC_ATV:
			{
				dnr_fmt = PE_NRD_M14C_DNR_ATV;
			}
			break;
		case LX_PE_SRC_CVBS:
		case LX_PE_SRC_SCART:
			{
				dnr_fmt = PE_NRD_M14C_DNR_AV;
			}
			break;
		case LX_PE_SRC_RGBPC:
			{
				dnr_fmt = PE_NRD_M14C_DNR_PC;
			}
			break;
		case LX_PE_SRC_DTV:
		case LX_PE_SRC_COMP:
		case LX_PE_SRC_HDMI:
		default:
			if(disp_inf->src_type==LX_PE_SRC_HDMI && \
				disp_inf->hdmi_type==LX_PE_HDMI_PC)
			{
				dnr_fmt = PE_NRD_M14C_DNR_PC;
			}
			else if(disp_inf->fmt_type==LX_PE_FMT_SD)
			{
				if(disp_inf->scan_type==LX_PE_SCAN_INTERLACE)
				{
					dnr_fmt = PE_NRD_M14C_DNR_SD_I;
				}
				else
				{
					dnr_fmt = PE_NRD_M14C_DNR_SD_P;
				}
			}
			else
			{
				if(disp_inf->scan_type==LX_PE_SCAN_INTERLACE)
				{
					dnr_fmt = PE_NRD_M14C_DNR_HD_I;
				}
				else
				{
					dnr_fmt = PE_NRD_M14C_DNR_HD_P;
				}
			}
			break;
	}
	return dnr_fmt;
}
/**
 * get dnr fmt from display info.(for M14BX)
 *
 * @param   *disp_inf [in] LX_PE_INF_DISPLAY_T
 * @return  PE_NRD_HW_M14B_DNR_FMT
 * @see
 * @author
 */
static PE_NRD_HW_M14B_DNR_FMT PE_NRD_HW_M14B_ConvDispInfoToDnrFmt(LX_PE_INF_DISPLAY_T *disp_inf)
{
	PE_NRD_HW_M14B_DNR_FMT dnr_fmt = PE_NRD_M14B_DNR_NUM;
	switch(disp_inf->src_type)
	{
		case LX_PE_SRC_ATV:
			{
				dnr_fmt = PE_NRD_M14B_DNR_ATV;
			}
			break;
		case LX_PE_SRC_CVBS:
		case LX_PE_SRC_SCART:
			{
				dnr_fmt = PE_NRD_M14B_DNR_AV;
			}
			break;
		case LX_PE_SRC_RGBPC:
			{
				dnr_fmt = PE_NRD_M14B_DNR_PC;
			}
			break;
		case LX_PE_SRC_DTV:
		case LX_PE_SRC_COMP:
		case LX_PE_SRC_HDMI:
		default:
			if(disp_inf->src_type==LX_PE_SRC_HDMI && \
				disp_inf->hdmi_type==LX_PE_HDMI_PC)
			{
				dnr_fmt = PE_NRD_M14B_DNR_PC;
			}
			else if(disp_inf->fmt_type==LX_PE_FMT_SD)
			{
				dnr_fmt = PE_NRD_M14B_DNR_SD;
			}
			else
			{
				dnr_fmt = PE_NRD_M14B_DNR_HD;
			}
			break;
	}
	return dnr_fmt;
}
/**
 * get ifc fmt from display info.(for M14BX)
 *
 * @param   *disp_inf [in] LX_PE_INF_DISPLAY_T
 * @return  PE_NRD_HW_M14B_IFC_FMT
 * @see
 * @author
 */
static PE_NRD_HW_M14B_IFC_FMT PE_NRD_HW_M14B_ConvDispInfoToIfcFmt(LX_PE_INF_DISPLAY_T *disp_inf)
{
	PE_NRD_HW_M14B_IFC_FMT ifc_fmt = PE_NRD_M14B_IFC_NUM;
	switch(disp_inf->src_type)
	{
		case LX_PE_SRC_ATV:
			if(disp_inf->cstd_type==LX_PE_CSTD_NTSC)
			{
				ifc_fmt = PE_NRD_M14B_IFC_ATV_NTSC;
			}
			else
			{
				ifc_fmt = PE_NRD_M14B_IFC_PC;
			}
			break;
		case LX_PE_SRC_CVBS:
		case LX_PE_SRC_SCART:
		case LX_PE_SRC_RGBPC:
		case LX_PE_SRC_DTV:
		case LX_PE_SRC_COMP:
		case LX_PE_SRC_HDMI:
		default:
			{
				ifc_fmt = PE_NRD_M14B_IFC_PC;
			}
			break;
	}
	return ifc_fmt;
}
/**
 * get tnr fmt from display info.(for M14AX)
 *
 * @param   *disp_inf [in] LX_PE_INF_DISPLAY_T
 * @return  PE_NRD_HW_M14A_TNR_FMT
 * @see
 * @author
 */
static PE_NRD_HW_M14A_TNR_FMT PE_NRD_HW_M14A_ConvDispInfoToTnrFmt(LX_PE_INF_DISPLAY_T *disp_inf)
{
	PE_NRD_HW_M14A_TNR_FMT tnr_fmt = PE_NRD_M14A_TNR_NUM;
	if(disp_inf->out_type==LX_PE_OUT_UD)
	{
		tnr_fmt = PE_NRD_M14A_TNR_UD;
	}
	else
	{
		switch(disp_inf->src_type)
		{
			case LX_PE_SRC_ATV:
				if(disp_inf->cstd_type==LX_PE_CSTD_SECAM)
				{
					tnr_fmt = PE_NRD_M14A_TNR_ATV_SECAM;
				}
				else
				{
					tnr_fmt = PE_NRD_M14A_TNR_ATV;
				}
				break;
			case LX_PE_SRC_CVBS:
			case LX_PE_SRC_SCART:
				if(disp_inf->cstd_type==LX_PE_CSTD_SECAM)
				{
					tnr_fmt = PE_NRD_M14A_TNR_AV_SECAM;
				}
				else
				{
					tnr_fmt = PE_NRD_M14A_TNR_AV;
				}
				break;
			case LX_PE_SRC_DTV:
				if(disp_inf->fmt_type==LX_PE_FMT_SD)
				{
					if(disp_inf->dtv_type==LX_PE_DTV_HDDPLAY && (disp_inf->hdd_type==LX_PE_HDD_SRC_ATV60 || \
						disp_inf->hdd_type==LX_PE_HDD_SRC_ATV50 || disp_inf->hdd_type==LX_PE_HDD_SRC_AV60 || \
						disp_inf->hdd_type==LX_PE_HDD_SRC_AV50 || disp_inf->hdd_type==LX_PE_HDD_SRC_SCARTRGB))
					{
						if(disp_inf->hdd_type==LX_PE_HDD_SRC_ATV60 || disp_inf->hdd_type==LX_PE_HDD_SRC_ATV50)
						{
							tnr_fmt = PE_NRD_M14A_TNR_DVR_ATV;
						}
						else
						{
							tnr_fmt = PE_NRD_M14A_TNR_AV;
						}
					}
					else if(disp_inf->dtv_type==LX_PE_DTV_FILEPLAY && \
						(disp_inf->in_h_size<=PE_NRD_HW_M14_H_MIN && disp_inf->in_v_size<=PE_NRD_HW_M14_V_MIN))
					{
						tnr_fmt = PE_NRD_M14A_TNR_SD_MC_OFF;	//because of boundary noise on divx(<=352x288)
					}
					else
					{
						tnr_fmt = PE_NRD_M14A_TNR_SD;
					}
				}
				else
				{
					tnr_fmt = PE_NRD_M14A_TNR_HD;
				}
				break;
			case LX_PE_SRC_RGBPC:
			case LX_PE_SRC_COMP:
			case LX_PE_SRC_HDMI:
			default:
				if(disp_inf->fmt_type==LX_PE_FMT_SD)
				{
					tnr_fmt = PE_NRD_M14A_TNR_SD;
				}
				else
				{
					tnr_fmt = PE_NRD_M14A_TNR_HD;
				}
				break;
		}
	}
	return tnr_fmt;
}
/**
 * get dnr fmt from display info.(for M14AX)
 *
 * @param   *disp_inf [in] LX_PE_INF_DISPLAY_T
 * @return  PE_NRD_HW_M14A_DNR_FMT
 * @see
 * @author
 */
static PE_NRD_HW_M14A_DNR_FMT PE_NRD_HW_M14A_ConvDispInfoToDnrFmt(LX_PE_INF_DISPLAY_T *disp_inf)
{
	PE_NRD_HW_M14A_DNR_FMT dnr_fmt = PE_NRD_M14A_DNR_NUM;
	switch(disp_inf->src_type)
	{
		case LX_PE_SRC_ATV:
		case LX_PE_SRC_CVBS:
		case LX_PE_SRC_SCART:
			{
				dnr_fmt = PE_NRD_M14A_DNR_SD;
			}
			break;
		case LX_PE_SRC_DTV:
			if (disp_inf->dtv_type==LX_PE_DTV_TESTPIC)
			{
				dnr_fmt = PE_NRD_M14A_DNR_TP;
			}
			else if (disp_inf->fmt_type==LX_PE_FMT_SD)
			{
				dnr_fmt = PE_NRD_M14A_DNR_SD;
			}
			else
			{
				dnr_fmt = PE_NRD_M14A_DNR_HD;
			}
			break;
		case LX_PE_SRC_RGBPC:
		case LX_PE_SRC_COMP:
		case LX_PE_SRC_HDMI:
		default:
			if(disp_inf->fmt_type==LX_PE_FMT_SD)
			{
				dnr_fmt = PE_NRD_M14A_DNR_SD;
			}
			else
			{
				dnr_fmt = PE_NRD_M14A_DNR_HD;
			}
			break;
	}
	return dnr_fmt;
}

/**
 * read default setting
 *
 * @param   *pstParams [in/out] LX_PE_ETC_TBL_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_NRD_HW_M14_RdDefault(LX_PE_ETC_TBL_T *pstParams)
{
	int ret = RET_OK;
	char buffer[PE_TRACE_STR_SIZE];
	UINT32 param_size=0, table_size=0, is_valid=0;
	static UINT32 rd_cnt=0;
	PE_REG_PARAM_T param;
	PE_NRD_HW_M14_SETTINGS_T *pInfo=&_g_pe_nrd_hw_m14_info;
	#ifdef PE_HW_M14B_USE_REG_CTRL
	UINT32 *p_data = NULL;
	PE_NRD_HW_M14B_P0M_REG_CTRL_T *p0m_reg_ctrl = &_g_pe_nrd_hw_m14b_p0m_reg_ctrl;
	PE_NRD_HW_M14B_P0M_REG_MATCH_T *p0m_reg_match = &_g_pe_nrd_hw_m14b_p0m_reg_match;
	PE_NRD_HW_M14B_P0D_REG_CTRL_T *p0d_reg_ctrl = &_g_pe_nrd_hw_m14b_p0d_reg_ctrl;
	PE_NRD_HW_M14B_P0D_REG_MATCH_T *p0d_reg_match = &_g_pe_nrd_hw_m14b_p0d_reg_match;
	#endif

#define PE_NRD_M14_NO_PRINT			0
#define PE_NRD_M14_RD_N_HEX_PRINT	1
#define PE_NRD_M14_HEX_PRINT_ONLY	2
#define PE_NRD_M14_DEC_PRINT_ONLY	3
#define PE_NRD_M14_PRINT_START		"START_OF_PRINT"
#define PE_NRD_M14_PRINT_END		"END_OF_PRINT"
#define PE_NRD_M14_PRINT_EXIT		"EXIT_OF_PRINT"
#define PE_NRD_M14_PRINT_RESRV		"RESERVED"
#define PE_NRD_M14_CASE_PRMENU(_cnt, _action, _buf, _item)	\
	case (_cnt):sprintf(_buf, "   [0x%02x] %s\n", (_item), #_item);_action;break
#define PE_NRD_M14_CASE_SPRINT(_cnt, _action, _buf, fmt, args...)	\
	case (_cnt):sprintf(_buf, fmt, ##args);_action;break
#define PE_NRD_M14_DFLT_SPRINT(_action, _buf, fmt, args...)		\
	default:sprintf(_buf, fmt, ##args);_action;break

	do{
		CHECK_KNULL(pstParams);
		CHECK_KNULL(pstParams->data);
		if (PE_KDRV_VER_M14BX)
		{
			if (pstParams->data[0]>=PE_NRD_HW_M14B_RD_DFLT_MAX)
			{
				sprintf(buffer, "%s", PE_NRD_M14_PRINT_EXIT);
				rd_cnt=0;
			}
			else if (pstParams->data[0]==PE_NRD_HW_M14B_RD_DFLT_DISPLAY)
			{
				switch (rd_cnt)
				{
					PE_NRD_M14_CASE_SPRINT( 0, rd_cnt++, buffer, "%s", PE_NRD_M14_PRINT_START);
					PE_NRD_M14_CASE_SPRINT( 1, rd_cnt++, buffer, "   ** current default **\n");
					PE_NRD_M14_CASE_SPRINT( 2, rd_cnt++, buffer, \
						"   tnr0b_fmt : %s\n", PE_NRD_HW_M14B_TNR_FMT_TO_STR(pInfo->tnr0b_fmt));
					PE_NRD_M14_CASE_SPRINT( 3, rd_cnt++, buffer, \
						"   tal0b_fmt : %s\n", PE_NRD_HW_M14B_TAL_FMT_TO_STR(pInfo->tal0b_fmt));
					PE_NRD_M14_CASE_SPRINT( 4, rd_cnt++, buffer, \
						"   dnr0c_fmt : %s\n", PE_NRD_HW_M14C_DNR_FMT_TO_STR(pInfo->dnr0c_fmt));
					PE_NRD_M14_CASE_SPRINT( 5, rd_cnt++, buffer, \
						"   ifc0b_fmt : %s\n", PE_NRD_HW_M14B_IFC_FMT_TO_STR(pInfo->ifc0b_fmt));
					PE_NRD_M14_CASE_SPRINT( 6, rd_cnt++, buffer, "   *********************\n");
					PE_NRD_M14_CASE_SPRINT( 7, rd_cnt++, buffer, PE_NRD_M14_PRINT_RESRV);
					PE_NRD_M14_CASE_SPRINT( 8, rd_cnt++, buffer, PE_NRD_M14_PRINT_RESRV);
					PE_NRD_M14_CASE_SPRINT( 9, rd_cnt++, buffer, PE_NRD_M14_PRINT_RESRV);
					PE_NRD_M14_CASE_PRMENU(10, rd_cnt++, buffer, PE_NRD_HW_M14B_RD_DFLT_TNR);
					PE_NRD_M14_CASE_PRMENU(11, rd_cnt++, buffer, PE_NRD_HW_M14B_RD_DFLT_DNR);
					PE_NRD_M14_CASE_PRMENU(12, rd_cnt++, buffer, PE_NRD_HW_M14B_RD_DFLT_IFC);
					#ifdef PE_HW_M14B_USE_REG_CTRL
					PE_NRD_M14_CASE_PRMENU(13, rd_cnt++, buffer, PE_NRD_HW_M14B_RD_TNR_MNUM);
					PE_NRD_M14_CASE_PRMENU(14, rd_cnt++, buffer, PE_NRD_HW_M14B_RD_P0M_DATA);
					PE_NRD_M14_CASE_PRMENU(15, rd_cnt++, buffer, PE_NRD_HW_M14B_RD_P0M_MASK);
					PE_NRD_M14_CASE_PRMENU(16, rd_cnt++, buffer, PE_NRD_HW_M14B_RD_DNR_MNUM);
					PE_NRD_M14_CASE_PRMENU(17, rd_cnt++, buffer, PE_NRD_HW_M14B_RD_P0D_DATA);
					PE_NRD_M14_CASE_PRMENU(18, rd_cnt++, buffer, PE_NRD_HW_M14B_RD_P0D_MASK);
					#endif
					PE_NRD_M14_DFLT_SPRINT(rd_cnt=0, buffer, "%s", PE_NRD_M14_PRINT_END);
				}
			}
			else
			{
				is_valid = 0;
				switch (pstParams->data[0])
				{
					case PE_NRD_HW_M14B_RD_DFLT_TNR:
						table_size = sizeof(tnr_l_pc_default_m14b0);
						param_size = table_size/sizeof(PE_REG_PARAM_T);
						if (rd_cnt<param_size)
						{
							param.addr = tnr_l_pc_default_m14b0[rd_cnt].addr;
							is_valid = 1;
						}
						break;
					case PE_NRD_HW_M14B_RD_DFLT_DNR:
						table_size = sizeof(dnr_l_pc_default_m14b0);
						param_size = table_size/sizeof(PE_REG_PARAM_T);
						if (rd_cnt<param_size)
						{
							param.addr = dnr_l_pc_default_m14b0[rd_cnt].addr;
							is_valid = 1;
						}
						break;
					case PE_NRD_HW_M14B_RD_DFLT_IFC:
						table_size = sizeof(ifc_l_pc_default_m14b0);
						param_size = table_size/sizeof(PE_REG_PARAM_T);
						if (rd_cnt<param_size)
						{
							param.addr = ifc_l_pc_default_m14b0[rd_cnt].addr;
							is_valid = 1;
						}
						break;
					#ifdef PE_HW_M14B_USE_REG_CTRL
					case PE_NRD_HW_M14B_RD_TNR_MNUM:
						table_size = p0m_reg_match->tnr_size;
						param_size = table_size/sizeof(PE_REG_PARAM_T);
						if (rd_cnt<param_size)
						{
							param.addr = p0m_reg_match->tnr_mtbl[rd_cnt].addr;
							param.data = p0m_reg_match->tnr_mtbl[rd_cnt].data;
							is_valid = PE_NRD_M14_DEC_PRINT_ONLY;
						}
						break;
					case PE_NRD_HW_M14B_RD_P0M_DATA:
						if (rd_cnt<PE_NRD_HW_M14B_P0M_ENUM(MAX))
						{
							param.addr = p0m_reg_ctrl->addr[rd_cnt];
							p_data = (UINT32 *)&(p0m_reg_ctrl->data);
							param.data = p_data[rd_cnt];
							is_valid = PE_NRD_M14_HEX_PRINT_ONLY;
						}
						break;
					case PE_NRD_HW_M14B_RD_P0M_MASK:
						if (rd_cnt<PE_NRD_HW_M14B_P0M_ENUM(MAX))
						{
							param.addr = p0m_reg_ctrl->addr[rd_cnt];
							p_data = (UINT32 *)&(p0m_reg_ctrl->mask);
							param.data = p_data[rd_cnt];
							is_valid = PE_NRD_M14_HEX_PRINT_ONLY;
						}
						break;
					case PE_NRD_HW_M14B_RD_DNR_MNUM:
						table_size = p0d_reg_match->dnr_size;
						param_size = table_size/sizeof(PE_REG_PARAM_T);
						if (rd_cnt<param_size)
						{
							param.addr = p0d_reg_match->dnr_mtbl[rd_cnt].addr;
							param.data = p0d_reg_match->dnr_mtbl[rd_cnt].data;
							is_valid = PE_NRD_M14_DEC_PRINT_ONLY;
						}
						break;
					case PE_NRD_HW_M14B_RD_P0D_DATA:
						if (rd_cnt<PE_NRD_HW_M14B_P0D_ENUM(MAX))
						{
							param.addr = p0d_reg_ctrl->addr[rd_cnt];
							p_data = (UINT32 *)&(p0d_reg_ctrl->data);
							param.data = p_data[rd_cnt];
							is_valid = PE_NRD_M14_HEX_PRINT_ONLY;
						}
						break;
					case PE_NRD_HW_M14B_RD_P0D_MASK:
						if (rd_cnt<PE_NRD_HW_M14B_P0D_ENUM(MAX))
						{
							param.addr = p0d_reg_ctrl->addr[rd_cnt];
							p_data = (UINT32 *)&(p0d_reg_ctrl->mask);
							param.data = p_data[rd_cnt];
							is_valid = PE_NRD_M14_HEX_PRINT_ONLY;
						}
						break;
					#endif
					default:
						break;
				}
				if (is_valid == PE_NRD_M14_RD_N_HEX_PRINT)
				{
					param.data = PE_REG_M14_RD(param.addr);
					sprintf(buffer, "   [%04d] {0x%04X, 0x%08X},\n", \
						rd_cnt, param.addr, param.data);
					rd_cnt++;
				}
				else if (is_valid == PE_NRD_M14_HEX_PRINT_ONLY)
				{
					sprintf(buffer, "   [%04d] {0x%04X, 0x%08X},\n", \
						rd_cnt, param.addr, param.data);
					rd_cnt++;
				}
				else if (is_valid == PE_NRD_M14_DEC_PRINT_ONLY)
				{
					sprintf(buffer, "   [%04d] {0x%04X, %04d},\n", \
						rd_cnt, param.addr, param.data);
					rd_cnt++;
				}
				else
				{
					sprintf(buffer, "%s", PE_NRD_M14_PRINT_END);
					rd_cnt = 0;
				}
			}
			memcpy(pstParams->data, buffer, sizeof(char)*PE_TRACE_STR_SIZE);
		}
		else if (PE_KDRV_VER_M14AX)
		{
			if (pstParams->data[0]>=PE_NRD_HW_M14A_RD_DFLT_MAX)
			{
				sprintf(buffer, "%s", PE_NRD_M14_PRINT_EXIT);
				rd_cnt=0;
			}
			else if (pstParams->data[0]==PE_NRD_HW_M14A_RD_DFLT_DISPLAY)
			{
				switch (rd_cnt)
				{
					PE_NRD_M14_CASE_SPRINT( 0, rd_cnt++, buffer, "%s", PE_NRD_M14_PRINT_START);
					PE_NRD_M14_CASE_SPRINT( 1, rd_cnt++, buffer, "   ** current default **\n");
					PE_NRD_M14_CASE_SPRINT( 2, rd_cnt++, buffer, \
						"   tnr0a_fmt : %s\n", PE_NRD_HW_M14A_TNR_FMT_TO_STR(pInfo->tnr0a_fmt));
					PE_NRD_M14_CASE_SPRINT( 3, rd_cnt++, buffer, \
						"   tnr1a_fmt : %s\n", PE_NRD_HW_M14A_TNR_FMT_TO_STR(pInfo->tnr1a_fmt));
					PE_NRD_M14_CASE_SPRINT( 4, rd_cnt++, buffer, \
						"   dnr0a_fmt : %s\n", PE_NRD_HW_M14A_DNR_FMT_TO_STR(pInfo->dnr0a_fmt));
					PE_NRD_M14_CASE_SPRINT( 5, rd_cnt++, buffer, \
						"   dnr1a_fmt : %s\n", PE_NRD_HW_M14A_DNR_FMT_TO_STR(pInfo->dnr1a_fmt));
					PE_NRD_M14_CASE_SPRINT( 6, rd_cnt++, buffer, "   *********************\n");
					PE_NRD_M14_CASE_SPRINT( 7, rd_cnt++, buffer, PE_NRD_M14_PRINT_RESRV);
					PE_NRD_M14_CASE_SPRINT( 8, rd_cnt++, buffer, PE_NRD_M14_PRINT_RESRV);
					PE_NRD_M14_CASE_SPRINT( 9, rd_cnt++, buffer, PE_NRD_M14_PRINT_RESRV);
					PE_NRD_M14_CASE_PRMENU(10, rd_cnt++, buffer, PE_NRD_HW_M14A_RD_DFLT_TNR_L);
					PE_NRD_M14_CASE_PRMENU(11, rd_cnt++, buffer, PE_NRD_HW_M14A_RD_DFLT_TNR_R);
					PE_NRD_M14_CASE_PRMENU(12, rd_cnt++, buffer, PE_NRD_HW_M14A_RD_DFLT_DNR_L);
					PE_NRD_M14_CASE_PRMENU(13, rd_cnt++, buffer, PE_NRD_HW_M14A_RD_DFLT_DNR_R);
					PE_NRD_M14_DFLT_SPRINT(rd_cnt=0, buffer, "%s", PE_NRD_M14_PRINT_END);
				}
			}
			else
			{
				is_valid = 0;
				switch (pstParams->data[0])
				{
					case PE_NRD_HW_M14A_RD_DFLT_TNR_L:
					case PE_NRD_HW_M14A_RD_DFLT_TNR_R:
						table_size = sizeof(tnr_l_sd_default_m14a0);
						param_size = table_size/sizeof(PE_REG_PARAM_T);
						if (rd_cnt<param_size)
						{
							param.addr = tnr_l_sd_default_m14a0[rd_cnt].addr;
							if(pstParams->data[0]==PE_NRD_HW_M14A_RD_DFLT_TNR_R)
								param.addr += PE_PE0_REG_M14_LRDIFF;
							is_valid = 1;
						}
						break;
					case PE_NRD_HW_M14A_RD_DFLT_DNR_L:
					case PE_NRD_HW_M14A_RD_DFLT_DNR_R:
					default:
						table_size = sizeof(dnr_sd_l_default_m14a0);
						param_size = table_size/sizeof(PE_REG_PARAM_T);
						if (rd_cnt<param_size)
						{
							param.addr = dnr_sd_l_default_m14a0[rd_cnt].addr;
							if(pstParams->data[0]==PE_NRD_HW_M14A_RD_DFLT_DNR_R)
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
					sprintf(buffer, "%s", PE_NRD_M14_PRINT_END);
					rd_cnt = 0;
				}
			}
			memcpy(pstParams->data, buffer, sizeof(char)*PE_TRACE_STR_SIZE);
		}
	}while (0);
	return ret;
}
/**
 * set dnr common ctrl
 * - use input struct LX_PE_NRD_DNR2_CMN_T for M14AX
 * - use input struct LX_PE_NRD_DNR0_CMN_T for M14AX
 *
 * @param   *pstParams [in] void
 * @return  OK if success, ERROR otherwise.
 * @see		set discription
 * @author
 */
int PE_NRD_HW_M14_SetDnrCmnCtrl(void *pstParams)
{
	int ret = RET_OK;
	PE_NRD_HW_M14_SETTINGS_T *pInfo = &_g_pe_nrd_hw_m14_info;
	do{
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_M14BX)
		{
			LX_PE_NRD_DNR4_CMN_T *pp=(LX_PE_NRD_DNR4_CMN_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			PE_NRD_HW_M14_DBG_PRINT("set[%d] : dnr_max_enable:0x%02x\n"\
				"dc_bnr : 0x%02x,0x%02x,0x%02x\n"\
				"ac_bnr : 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				"mnr    : 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n", pp->win_id, pp->reg_dnr_max_enable, \
				/* dc bnr 3 */
				pp->reg_dc_bnr_enable, pp->reg_dc_bnr_mastergain, pp->reg_dc_bnr_chromagain, \
				/* ac bnr 7 */
				pp->reg_bnr_ac_h_en, pp->reg_bnr_ac_v_en, pp->reg_bnr_ac_h_chroma_en, \
				pp->reg_bnr_ac_v_chroma_en, pp->reg_bnr_ac_acness_resol_h, \
				pp->reg_bnr_ac_acness_resol_v, pp->reg_ac_master_gain, \
				/* mnr 5 */
				pp->reg_mnr_enable, pp->reg_mnr_master_gain, pp->reg_chroma_master_gain, \
				pp->reg_mnr_h_gain, pp->reg_mnr_v_gain);
			if(PE_CHECK_WIN0(pp->win_id))
			{
				#ifdef PE_HW_M14B_USE_REG_CTRL
				#ifdef PE_FWI_M14_ADD_FW_DNR_DATA
				PE_FWI_M14_DNR_COMMON fw_param;
				fw_param.reg_dc_bnr_chromagain  = GET_BITS(pp->reg_dc_bnr_chromagain,0,8);
				fw_param.reg_bnr_ac_h_en        = GET_BITS(pp->reg_bnr_ac_h_en,0,1);
				fw_param.reg_bnr_ac_v_en        = GET_BITS(pp->reg_bnr_ac_v_en,0,1);
				fw_param.reg_bnr_ac_h_chroma_en = GET_BITS(pp->reg_bnr_ac_h_chroma_en,0,1);
				fw_param.reg_bnr_ac_v_chroma_en = GET_BITS(pp->reg_bnr_ac_v_chroma_en,0,1);
				ret = PE_FWI_M14_SetDnrCommon(&fw_param);
				PE_NRD_HW_M14_CHECK_CODE(ret, break, \
					"[%s,%d] PE_FWI_M14_SetDnrCommon() error.\n", __F__, __L__);
				PE_NRD_HW_M14B_P0D_WR01(p0d_dnr_max_ctrl,	reg_dnr_max_enable,		GET_BITS(pp->reg_dnr_max_enable,0,1));
				PE_NRD_HW_M14B_P0D_WR01(p0d_dc_bnr_ctrl_0,	reg_dc_bnr_enable,		GET_BITS(pp->reg_dc_bnr_enable,0,1));
				PE_NRD_HW_M14B_P0D_WR01(p0d_dc_bnr_ctrl_2,	reg_dc_bnr_mastergain,	GET_BITS(pp->reg_dc_bnr_mastergain,0,6));
				PE_NRD_HW_M14B_P0D_SET(p0d_dc_bnr_ctrl_2,	reg_dc_bnr_chromagain,	GET_BITS(pp->reg_dc_bnr_chromagain,0,8))
				PE_NRD_HW_M14B_P0D_WR01(p0d_ac_bnr_ctrl_0,	reg_bnr_ac_acness_resol_h,GET_BITS(pp->reg_bnr_ac_acness_resol_h,0,2));
				PE_NRD_HW_M14B_P0D_SET(p0d_ac_bnr_ctrl_0,	reg_bnr_ac_h_en,		GET_BITS(pp->reg_bnr_ac_h_en,0,1));
				PE_NRD_HW_M14B_P0D_SET(p0d_ac_bnr_ctrl_0,	reg_bnr_ac_v_en,		GET_BITS(pp->reg_bnr_ac_v_en,0,1));
				PE_NRD_HW_M14B_P0D_SET(p0d_ac_bnr_ctrl_0,	reg_bnr_ac_h_chroma_en,	GET_BITS(pp->reg_bnr_ac_h_chroma_en,0,1));
				PE_NRD_HW_M14B_P0D_SET(p0d_ac_bnr_ctrl_0,	reg_bnr_ac_v_chroma_en,	GET_BITS(pp->reg_bnr_ac_v_chroma_en,0,1));
				PE_NRD_HW_M14B_P0D_WR01(p0d_ac_bnr_ctrl_4,	reg_bnr_ac_acness_resol_v,GET_BITS(pp->reg_bnr_ac_acness_resol_v,0,2));
				PE_NRD_HW_M14B_P0D_WR01(p0d_ac_bnr_ctrl_9,	reg_ac_master_gain,		GET_BITS(pp->reg_ac_master_gain,0,6));
				PE_NRD_HW_M14B_P0D_WR03(p0d_mnr_ctrl_0,		reg_mnr_enable,			GET_BITS(pp->reg_mnr_enable,0,1),\
														reg_mnr_master_gain,	GET_BITS(pp->reg_mnr_master_gain,0,8),\
														reg_chroma_master_gain,	GET_BITS(pp->reg_chroma_master_gain,0,8));
				PE_NRD_HW_M14B_P0D_WR02(p0d_mnr_ctrl_2,		reg_mnr_h_gain,			GET_BITS(pp->reg_mnr_h_gain,0,8),\
														reg_mnr_v_gain,			GET_BITS(pp->reg_mnr_v_gain,0,8));
				#else	//PE_FWI_M14_ADD_FW_DNR_DATA
				PE_NRD_HW_M14B_P0D_WR01(p0d_dnr_max_ctrl,	reg_dnr_max_enable,		GET_BITS(pp->reg_dnr_max_enable,0,1));
				PE_NRD_HW_M14B_P0D_WR01(p0d_dc_bnr_ctrl_0,	reg_dc_bnr_enable,		GET_BITS(pp->reg_dc_bnr_enable,0,1));
				PE_NRD_HW_M14B_P0D_WR02(p0d_dc_bnr_ctrl_2,	reg_dc_bnr_mastergain,	GET_BITS(pp->reg_dc_bnr_mastergain,0,6),\
														reg_dc_bnr_chromagain,	GET_BITS(pp->reg_dc_bnr_chromagain,0,8));
				#ifdef PE_HW_M14B_USE_DNR_MODE_ON_TASK
				PE_NRD_HW_M14B_P0D_WR04(p0d_ac_bnr_ctrl_0,	reg_bnr_ac_v_en,		GET_BITS(pp->reg_bnr_ac_v_en,0,1),\
														reg_bnr_ac_h_chroma_en,	GET_BITS(pp->reg_bnr_ac_h_chroma_en,0,1),\
														reg_bnr_ac_v_chroma_en,	GET_BITS(pp->reg_bnr_ac_v_chroma_en,0,1),\
														reg_bnr_ac_acness_resol_h,GET_BITS(pp->reg_bnr_ac_acness_resol_h,0,2));
				pInfo->reg_bnr_ac_h_en0 = GET_BITS(pp->reg_bnr_ac_h_en,0,1);
				#else
				PE_NRD_HW_M14B_P0D_WR05(p0d_ac_bnr_ctrl_0,	reg_bnr_ac_h_en,		GET_BITS(pp->reg_bnr_ac_h_en,0,1),\
														reg_bnr_ac_v_en,		GET_BITS(pp->reg_bnr_ac_v_en,0,1),\
														reg_bnr_ac_h_chroma_en,	GET_BITS(pp->reg_bnr_ac_h_chroma_en,0,1),\
														reg_bnr_ac_v_chroma_en,	GET_BITS(pp->reg_bnr_ac_v_chroma_en,0,1),\
														reg_bnr_ac_acness_resol_h,GET_BITS(pp->reg_bnr_ac_acness_resol_h,0,2));
				#endif
				PE_NRD_HW_M14B_P0D_WR01(p0d_ac_bnr_ctrl_4,	reg_bnr_ac_acness_resol_v,GET_BITS(pp->reg_bnr_ac_acness_resol_v,0,2));
				PE_NRD_HW_M14B_P0D_WR01(p0d_ac_bnr_ctrl_9,	reg_ac_master_gain,		GET_BITS(pp->reg_ac_master_gain,0,6));
				PE_NRD_HW_M14B_P0D_WR03(p0d_mnr_ctrl_0,		reg_mnr_enable,			GET_BITS(pp->reg_mnr_enable,0,1),\
														reg_mnr_master_gain,	GET_BITS(pp->reg_mnr_master_gain,0,8),\
														reg_chroma_master_gain,	GET_BITS(pp->reg_chroma_master_gain,0,8));
				PE_NRD_HW_M14B_P0D_WR02(p0d_mnr_ctrl_2,		reg_mnr_h_gain,			GET_BITS(pp->reg_mnr_h_gain,0,8),\
														reg_mnr_v_gain,			GET_BITS(pp->reg_mnr_v_gain,0,8));
				#endif	//PE_FWI_M14_ADD_FW_DNR_DATA
				#else
				PE_P0D_M14B0_QWr01(p0d_dnr_max_ctrl,	reg_dnr_max_enable,		GET_BITS(pp->reg_dnr_max_enable,0,1));
				PE_P0D_M14B0_QWr01(p0d_dc_bnr_ctrl_0,	reg_dc_bnr_enable,		GET_BITS(pp->reg_dc_bnr_enable,0,1));
				PE_P0D_M14B0_QWr02(p0d_dc_bnr_ctrl_2,	reg_dc_bnr_mastergain,	GET_BITS(pp->reg_dc_bnr_mastergain,0,6),\
														reg_dc_bnr_chromagain,	GET_BITS(pp->reg_dc_bnr_chromagain,0,8));
				PE_P0D_M14B0_QWr05(p0d_ac_bnr_ctrl_0,	reg_bnr_ac_h_en,		GET_BITS(pp->reg_bnr_ac_h_en,0,1),\
														reg_bnr_ac_v_en,		GET_BITS(pp->reg_bnr_ac_v_en,0,1),\
														reg_bnr_ac_h_chroma_en,	GET_BITS(pp->reg_bnr_ac_h_chroma_en,0,1),\
														reg_bnr_ac_v_chroma_en,	GET_BITS(pp->reg_bnr_ac_v_chroma_en,0,1),\
														reg_bnr_ac_acness_resol_h,GET_BITS(pp->reg_bnr_ac_acness_resol_h,0,2));
				PE_P0D_M14B0_QWr01(p0d_ac_bnr_ctrl_4,	reg_bnr_ac_acness_resol_v,GET_BITS(pp->reg_bnr_ac_acness_resol_v,0,2));
				PE_P0D_M14B0_QWr01(p0d_ac_bnr_ctrl_9,	reg_ac_master_gain,		GET_BITS(pp->reg_ac_master_gain,0,6));
				PE_P0D_M14B0_QWr03(p0d_mnr_ctrl_0,		reg_mnr_enable,			GET_BITS(pp->reg_mnr_enable,0,1),\
														reg_mnr_master_gain,	GET_BITS(pp->reg_mnr_master_gain,0,8),\
														reg_chroma_master_gain,	GET_BITS(pp->reg_chroma_master_gain,0,8));
				PE_P0D_M14B0_QWr02(p0d_mnr_ctrl_2,		reg_mnr_h_gain,			GET_BITS(pp->reg_mnr_h_gain,0,8),\
														reg_mnr_v_gain,			GET_BITS(pp->reg_mnr_v_gain,0,8));
				#endif
			}
		}
		else if(PE_KDRV_VER_M14AX)
		{
			LX_PE_NRD_DNR2_CMN_T *pp=(LX_PE_NRD_DNR2_CMN_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			PE_NRD_HW_M14_DBG_PRINT("set[%d] : dnr main : en: ac:%d, mnr:%d, dc:%d\n"\
				"mnr : en:%d, master_gain%d, chroma_gain:%d,h_gain:%d, v_gain:%d\n"\
				"ac_bnr: h,v_en:%d,%d,  mo_max,min:%d,%d,  h,v_bmax:%d,%d,  str_h,v_max:%d,%d\n"\
				"dc_bnr: en:%d,master_gain%d, chroma_gain:%d\n",pp->win_id,\
				/* dnr main */
				pp->enable_mnr,pp->enable_ac_bnr,pp->enable_dc_bnr,\
				/* mnr */
				pp->reg_mnr_enable,pp->reg_mnr_master_gain,pp->reg_chroma_master_gain,pp->reg_mnr_h_gain,pp->reg_mnr_v_gain,\
				/* ac */
				pp->bnr_h_en,pp->bnr_v_en,pp->ac_bnr_protect_motion_max,pp->ac_bnr_protect_motion_min,\
				pp->hbmax_gain,pp->vbmax_gain,pp->strength_h_max,pp->strength_v_max,\
				/* dc */
				pp->reg_dc_bnr_enable,pp->reg_dc_bnr_mastergain,pp->reg_dc_bnr_chromagain);
			if(PE_CHECK_WIN0(pp->win_id))
			{
				PE_P0L_M14A0_QWr03(p0l_dnr_max_ctrl,	enable_ac_bnr,			GET_BITS(pp->enable_ac_bnr,0,1),\
														enable_mnr,				GET_BITS(pp->enable_mnr,0,1),\
														enable_dc_bnr,			GET_BITS(pp->enable_dc_bnr,0,1));
				PE_P0L_M14A0_QWr03(p0l_mnr_ctrl_00,		reg_mnr_enable,			GET_BITS(pp->reg_mnr_enable,0,1),\
														reg_mnr_master_gain,	GET_BITS(pp->reg_mnr_master_gain,0,8),\
														reg_chroma_master_gain,	GET_BITS(pp->reg_chroma_master_gain,0,8));
				PE_P0L_M14A0_QWr02(p0l_mnr_ctrl_02,		reg_mnr_h_gain,			GET_BITS(pp->reg_mnr_h_gain,0,8),\
														reg_mnr_v_gain,			GET_BITS(pp->reg_mnr_v_gain,0,8));
				#ifdef PE_HW_M14A_USE_DNR_MODE_ON_TASK
				PE_P0L_M14A0_QWr03(p0l_bnr_ac_ctrl_00,	reg_bnr_ac_v_en,		GET_BITS(pp->bnr_v_en,0,1),\
														reg_bnr_ac_gain_h_max,	GET_BITS(pp->hbmax_gain,0,4),\
														reg_bnr_ac_gain_v_max,	GET_BITS(pp->vbmax_gain,0,4));
				pInfo->reg_bnr_ac_h_en0 = GET_BITS(pp->bnr_h_en,0,1);
				#else
				PE_P0L_M14A0_QWr04(p0l_bnr_ac_ctrl_00,	reg_bnr_ac_h_en,		GET_BITS(pp->bnr_h_en,0,1),\
														reg_bnr_ac_v_en,		GET_BITS(pp->bnr_v_en,0,1),\
														reg_bnr_ac_gain_h_max,	GET_BITS(pp->hbmax_gain,0,4),\
														reg_bnr_ac_gain_v_max,	GET_BITS(pp->vbmax_gain,0,4));
				#endif
				PE_P0L_M14A0_QWr02(p0l_bnr_ac_ctrl_06,	ac_bnr_protect_motion_max,	GET_BITS(pp->ac_bnr_protect_motion_max,0,8),\
														ac_bnr_protect_motion_min,	GET_BITS(pp->ac_bnr_protect_motion_min,0,8));
				PE_P0L_M14A0_QWr01(p0l_bnr_ac_ctrl_01,	strength_h_max,			GET_BITS(pp->strength_h_max,0,8));
				PE_P0L_M14A0_QWr01(p0l_bnr_ac_ctrl_02,	strength_v_max,			GET_BITS(pp->strength_v_max,0,8));
				PE_P0L_M14A0_QWr03(p0l_bnr_dc_ctrl_00,	reg_dc_bnr_enable,		GET_BITS(pp->reg_dc_bnr_enable,0,1),\
														reg_dc_bnr_mastergain,	GET_BITS(pp->reg_dc_bnr_mastergain,0,8),\
														reg_dc_bnr_chromagain,	GET_BITS(pp->reg_dc_bnr_chromagain,0,8));
			}
			if(PE_CHECK_WIN1(pp->win_id))
			{
				PE_P0R_M14A0_QWr03(p0r_dnr_max_ctrl,	enable_ac_bnr,			GET_BITS(pp->enable_ac_bnr,0,1),\
														enable_mnr,				GET_BITS(pp->enable_mnr,0,1),\
														enable_dc_bnr,			GET_BITS(pp->enable_dc_bnr,0,1));
				PE_P0R_M14A0_QWr03(p0r_mnr_ctrl_00,		reg_mnr_enable,			GET_BITS(pp->reg_mnr_enable,0,1),\
														reg_mnr_master_gain,	GET_BITS(pp->reg_mnr_master_gain,0,8),\
														reg_chroma_master_gain,	GET_BITS(pp->reg_chroma_master_gain,0,8));
				PE_P0R_M14A0_QWr02(p0r_mnr_ctrl_02,		reg_mnr_h_gain,			GET_BITS(pp->reg_mnr_h_gain,0,8),\
														reg_mnr_v_gain,			GET_BITS(pp->reg_mnr_v_gain,0,8));
				#ifdef PE_HW_M14A_USE_DNR_MODE_ON_TASK
				PE_P0R_M14A0_QWr03(p0r_bnr_ac_ctrl_00,	reg_bnr_ac_v_en,		GET_BITS(pp->bnr_v_en,0,1),\
														reg_bnr_ac_gain_h_max,	GET_BITS(pp->hbmax_gain,0,4),\
														reg_bnr_ac_gain_v_max,	GET_BITS(pp->vbmax_gain,0,4));
				pInfo->reg_bnr_ac_h_en1 = GET_BITS(pp->bnr_h_en,0,1);
				#else
				PE_P0R_M14A0_QWr04(p0r_bnr_ac_ctrl_00,	reg_bnr_ac_h_en,		GET_BITS(pp->bnr_h_en,0,1),\
														reg_bnr_ac_v_en,		GET_BITS(pp->bnr_v_en,0,1),\
														reg_bnr_ac_gain_h_max,	GET_BITS(pp->hbmax_gain,0,4),\
														reg_bnr_ac_gain_v_max,	GET_BITS(pp->vbmax_gain,0,4));
				#endif
				PE_P0R_M14A0_QWr02(p0r_bnr_ac_ctrl_06,	ac_bnr_protect_motion_max,	GET_BITS(pp->ac_bnr_protect_motion_max,0,8),\
														ac_bnr_protect_motion_min,	GET_BITS(pp->ac_bnr_protect_motion_min,0,8));
				PE_P0R_M14A0_QWr01(p0r_bnr_ac_ctrl_01,	strength_h_max,			GET_BITS(pp->strength_h_max,0,8));
				PE_P0R_M14A0_QWr01(p0r_bnr_ac_ctrl_02,	strength_v_max,			GET_BITS(pp->strength_v_max,0,8));
				PE_P0R_M14A0_QWr03(p0r_bnr_dc_ctrl_00,	reg_dc_bnr_enable,		GET_BITS(pp->reg_dc_bnr_enable,0,1),\
														reg_dc_bnr_mastergain,	GET_BITS(pp->reg_dc_bnr_mastergain,0,8),\
														reg_dc_bnr_chromagain,	GET_BITS(pp->reg_dc_bnr_chromagain,0,8));
			}
		}
		else
		{
			PE_NRD_HW_M14_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * get dnr common ctrl
 * - use input struct LX_PE_NRD_DNR2_CMN_T for M14AX
 * - use input struct LX_PE_NRD_DNR0_CMN_T for M14AX
 *
 * @param   *pstParams [in/out] void
 * @return  OK if success, ERROR otherwise.
 * @see		set discription
 * @author
 */
int PE_NRD_HW_M14_GetDnrCmnCtrl(void *pstParams)
{
	int ret = RET_OK;
	LX_PE_WIN_ID win_id;
	do{
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_M14BX)
		{
			LX_PE_NRD_DNR4_CMN_T *pp = (LX_PE_NRD_DNR4_CMN_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			win_id = PE_GET_CHECKED_WINID(pp->win_id);
			if(PE_CHECK_WIN0(win_id))
			{
				#ifdef PE_FWI_M14_ADD_FW_DNR_DATA
				PE_FWI_M14_SETTINGS_T fw_inf;
				ret = PE_FWI_M14_GetCurFwiSettings(&fw_inf);
				PE_NRD_HW_M14_CHECK_CODE(ret, break, \
					"[%s,%d] PE_FWI_M14_GetCurFwiSettings() error.\n", __F__, __L__);
				pp->reg_dc_bnr_chromagain  = fw_inf.dnr_common.reg_dc_bnr_chromagain;
				pp->reg_bnr_ac_h_en        = fw_inf.dnr_common.reg_bnr_ac_h_en;
				pp->reg_bnr_ac_v_en        = fw_inf.dnr_common.reg_bnr_ac_v_en;
				pp->reg_bnr_ac_h_chroma_en = fw_inf.dnr_common.reg_bnr_ac_h_chroma_en;
				pp->reg_bnr_ac_v_chroma_en = fw_inf.dnr_common.reg_bnr_ac_v_chroma_en;
				PE_P0D_M14B0_QRd01(p0d_dnr_max_ctrl,	reg_dnr_max_enable,		pp->reg_dnr_max_enable);
				PE_P0D_M14B0_QRd01(p0d_dc_bnr_ctrl_0,	reg_dc_bnr_enable,		pp->reg_dc_bnr_enable);
				PE_P0D_M14B0_QRd01(p0d_dc_bnr_ctrl_2,	reg_dc_bnr_mastergain,	pp->reg_dc_bnr_mastergain);
				PE_P0D_M14B0_QRd01(p0d_ac_bnr_ctrl_0,	reg_bnr_ac_acness_resol_h,pp->reg_bnr_ac_acness_resol_h);
				PE_P0D_M14B0_QRd01(p0d_ac_bnr_ctrl_4,	reg_bnr_ac_acness_resol_v,pp->reg_bnr_ac_acness_resol_v);
				PE_P0D_M14B0_QRd01(p0d_ac_bnr_ctrl_9,	reg_ac_master_gain,		pp->reg_ac_master_gain);
				PE_P0D_M14B0_QRd03(p0d_mnr_ctrl_0,		reg_mnr_enable,			pp->reg_mnr_enable,\
														reg_mnr_master_gain,	pp->reg_mnr_master_gain,\
														reg_chroma_master_gain,	pp->reg_chroma_master_gain);
				PE_P0D_M14B0_QRd02(p0d_mnr_ctrl_2,		reg_mnr_h_gain,			pp->reg_mnr_h_gain,\
														reg_mnr_v_gain,			pp->reg_mnr_v_gain);
				#else	//PE_FWI_M14_ADD_FW_DNR_DATA
				PE_P0D_M14B0_QRd01(p0d_dnr_max_ctrl,	reg_dnr_max_enable,		pp->reg_dnr_max_enable);
				PE_P0D_M14B0_QRd01(p0d_dc_bnr_ctrl_0,	reg_dc_bnr_enable,		pp->reg_dc_bnr_enable);
				PE_P0D_M14B0_QRd02(p0d_dc_bnr_ctrl_2,	reg_dc_bnr_mastergain,	pp->reg_dc_bnr_mastergain,\
														reg_dc_bnr_chromagain,	pp->reg_dc_bnr_chromagain);
				PE_P0D_M14B0_QRd05(p0d_ac_bnr_ctrl_0,	reg_bnr_ac_h_en,		pp->reg_bnr_ac_h_en,\
														reg_bnr_ac_v_en,		pp->reg_bnr_ac_v_en,\
														reg_bnr_ac_h_chroma_en,	pp->reg_bnr_ac_h_chroma_en,\
														reg_bnr_ac_v_chroma_en,	pp->reg_bnr_ac_v_chroma_en,\
														reg_bnr_ac_acness_resol_h,pp->reg_bnr_ac_acness_resol_h);
				PE_P0D_M14B0_QRd01(p0d_ac_bnr_ctrl_4,	reg_bnr_ac_acness_resol_v,pp->reg_bnr_ac_acness_resol_v);
				PE_P0D_M14B0_QRd01(p0d_ac_bnr_ctrl_9,	reg_ac_master_gain,		pp->reg_ac_master_gain);
				PE_P0D_M14B0_QRd03(p0d_mnr_ctrl_0,		reg_mnr_enable,			pp->reg_mnr_enable,\
														reg_mnr_master_gain,	pp->reg_mnr_master_gain,\
														reg_chroma_master_gain,	pp->reg_chroma_master_gain);
				PE_P0D_M14B0_QRd02(p0d_mnr_ctrl_2,		reg_mnr_h_gain,			pp->reg_mnr_h_gain,\
														reg_mnr_v_gain,			pp->reg_mnr_v_gain);
				#endif	//PE_FWI_M14_ADD_FW_DNR_DATA
			}
			PE_NRD_HW_M14_DBG_PRINT("get[%d] : dnr_max_enable:0x%02x\n"\
				"dc_bnr : 0x%02x,0x%02x,0x%02x\n"\
				"ac_bnr : 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				"mnr    : 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n", pp->win_id, pp->reg_dnr_max_enable, \
				/* dc bnr 3 */
				pp->reg_dc_bnr_enable, pp->reg_dc_bnr_mastergain, pp->reg_dc_bnr_chromagain, \
				/* ac bnr 7 */
				pp->reg_bnr_ac_h_en, pp->reg_bnr_ac_v_en, pp->reg_bnr_ac_h_chroma_en, \
				pp->reg_bnr_ac_v_chroma_en, pp->reg_bnr_ac_acness_resol_h, \
				pp->reg_bnr_ac_acness_resol_v, pp->reg_ac_master_gain, \
				/* mnr 5 */
				pp->reg_mnr_enable, pp->reg_mnr_master_gain, pp->reg_chroma_master_gain, \
				pp->reg_mnr_h_gain, pp->reg_mnr_v_gain);
		}
		else if(PE_KDRV_VER_M14AX)
		{
			LX_PE_NRD_DNR2_CMN_T *pp = (LX_PE_NRD_DNR2_CMN_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			win_id = PE_GET_CHECKED_WINID(pp->win_id);
			if(PE_CHECK_WIN0(win_id))
			{
				PE_P0L_M14A0_QRd03(p0l_dnr_max_ctrl,	enable_ac_bnr,			pp->enable_ac_bnr,\
														enable_mnr,				pp->enable_mnr,\
														enable_dc_bnr,			pp->enable_dc_bnr);
				PE_P0L_M14A0_QRd03(p0l_mnr_ctrl_00,		reg_mnr_enable,			pp->reg_mnr_enable,\
														reg_mnr_master_gain,	pp->reg_mnr_master_gain,\
														reg_chroma_master_gain,	pp->reg_chroma_master_gain);
				PE_P0L_M14A0_QRd02(p0l_mnr_ctrl_02,		reg_mnr_h_gain,			pp->reg_mnr_h_gain,\
														reg_mnr_v_gain,			pp->reg_mnr_v_gain);
				PE_P0L_M14A0_QRd04(p0l_bnr_ac_ctrl_00,	reg_bnr_ac_h_en,		pp->bnr_h_en,\
														reg_bnr_ac_v_en,		pp->bnr_v_en,\
														reg_bnr_ac_gain_h_max,	pp->hbmax_gain,\
														reg_bnr_ac_gain_v_max,	pp->vbmax_gain);
				PE_P0L_M14A0_QRd02(p0l_bnr_ac_ctrl_06,	ac_bnr_protect_motion_max,	pp->ac_bnr_protect_motion_max,\
														ac_bnr_protect_motion_min,	pp->ac_bnr_protect_motion_min);
				PE_P0L_M14A0_QRd01(p0l_bnr_ac_ctrl_01,	strength_h_max,			pp->strength_h_max);
				PE_P0L_M14A0_QRd01(p0l_bnr_ac_ctrl_02,	strength_v_max,			pp->strength_v_max);
				PE_P0L_M14A0_QRd03(p0l_bnr_dc_ctrl_00,	reg_dc_bnr_enable,		pp->reg_dc_bnr_enable,\
														reg_dc_bnr_mastergain,	pp->reg_dc_bnr_mastergain,\
														reg_dc_bnr_chromagain,	pp->reg_dc_bnr_chromagain);
			}
			if(PE_CHECK_WIN1(win_id))
			{
				PE_P0R_M14A0_QRd03(p0r_dnr_max_ctrl,	enable_ac_bnr,			pp->enable_ac_bnr,\
														enable_mnr,				pp->enable_mnr,\
														enable_dc_bnr,			pp->enable_dc_bnr);
				PE_P0R_M14A0_QRd03(p0r_mnr_ctrl_00,		reg_mnr_enable,			pp->reg_mnr_enable,\
														reg_mnr_master_gain,	pp->reg_mnr_master_gain,\
														reg_chroma_master_gain,	pp->reg_chroma_master_gain);
				PE_P0R_M14A0_QRd02(p0r_mnr_ctrl_02,		reg_mnr_h_gain,			pp->reg_mnr_h_gain,\
														reg_mnr_v_gain,			pp->reg_mnr_v_gain);
				PE_P0R_M14A0_QRd04(p0r_bnr_ac_ctrl_00,	reg_bnr_ac_h_en,		pp->bnr_h_en,\
														reg_bnr_ac_v_en,		pp->bnr_v_en,\
														reg_bnr_ac_gain_h_max,	pp->hbmax_gain,\
														reg_bnr_ac_gain_v_max,	pp->vbmax_gain);
				PE_P0R_M14A0_QRd02(p0r_bnr_ac_ctrl_06,	ac_bnr_protect_motion_max,	pp->ac_bnr_protect_motion_max,\
														ac_bnr_protect_motion_min,	pp->ac_bnr_protect_motion_min);
				PE_P0R_M14A0_QRd01(p0r_bnr_ac_ctrl_01,	strength_h_max,			pp->strength_h_max);
				PE_P0R_M14A0_QRd01(p0r_bnr_ac_ctrl_02,	strength_v_max,			pp->strength_v_max);
				PE_P0R_M14A0_QRd03(p0r_bnr_dc_ctrl_00,	reg_dc_bnr_enable,		pp->reg_dc_bnr_enable,\
														reg_dc_bnr_mastergain,	pp->reg_dc_bnr_mastergain,\
														reg_dc_bnr_chromagain,	pp->reg_dc_bnr_chromagain);
			}
			PE_NRD_HW_M14_DBG_PRINT("get[%d] : dnr main : en: ac:%d, mnr:%d, dc:%d\n"\
						  	 "          mnr      : en:%d, master_gain%d, chroma_gain:%d,h_gain:%d, v_gain:%d\n"\
						 	 "          ac_bnr   : h,v_en:%d,%d,  mo_max,min:%d,%d,  h,v_bmax:%d,%d,  str_h,v_max:%d,%d\n"\
						  	 "          dc_bnr   : en:%d,master_gain%d, chroma_gain:%d\n",pp->win_id,\
				/* dnr main */
				pp->enable_mnr,pp->enable_ac_bnr,pp->enable_dc_bnr,\
				/* mnr */
				pp->reg_mnr_enable,pp->reg_mnr_master_gain,pp->reg_chroma_master_gain,pp->reg_mnr_h_gain,pp->reg_mnr_v_gain,\
				/* ac */
				pp->bnr_h_en,pp->bnr_v_en,pp->ac_bnr_protect_motion_max,pp->ac_bnr_protect_motion_min,\
				pp->hbmax_gain,pp->vbmax_gain,pp->strength_h_max,pp->strength_v_max,\
				/* dc */
				pp->reg_dc_bnr_enable,pp->reg_dc_bnr_mastergain,pp->reg_dc_bnr_chromagain);
		}
		else
		{
			PE_NRD_HW_M14_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * set dnr detail ctrl
 * - use input struct LX_PE_NRD_DNR2_DETAIL_T for M14AX
 *
 * @param   *pstParams [in] void
 * @return  OK if success, ERROR otherwise.
 * @see		set discription
 * @author
 */
int PE_NRD_HW_M14_SetDnrDetailCtrl(void *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_M14BX)
		{
			LX_PE_NRD_DNR4_DETAIL_T *pp=(LX_PE_NRD_DNR4_DETAIL_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			PE_NRD_HW_M14_DBG_PRINT("set[%d] \n"\
				"dc_bnr: 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				"dc_bnr: 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				"ac_bnr: 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				"ac_bnr: 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				"ac_bnr: 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				"mnr   : 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				"mnr   : 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n", pp->win_id,\
				/* dc bnr 18 */
				pp->reg_dc_var_en, pp->reg_dc_motion_en, pp->reg_dc_protection_en, \
				pp->reg_dc_detail_en, pp->reg_dc_blur_sel, pp->reg_dc_motion_max, \
				pp->reg_dc_motion_min, pp->reg_dc_detail_max, pp->reg_dc_detail_min, \
				pp->reg_var_v_gain, pp->reg_var_h_gain, pp->reg_var_cut_resolution, \
				pp->reg_dc_global_motion_th, pp->reg_dc_protection_th, pp->reg_dc_bnr_var_th0, \
				pp->reg_dc_bnr_var_th1, pp->reg_dc_bnr_var_th2, pp->reg_dc_bnr_var_th3, \
				/* ac bnr 23 */
				pp->reg_bnr_ac_diff_min_v_th, pp->reg_bnr_ac_diff_min_h_th, \
				pp->reg_bnr_ac_global_motion_th, pp->reg_bnr_ac_acness_max, \
				pp->reg_bnr_ac_acness_min, pp->reg_bnr_ac_motion_max, pp->reg_bnr_ac_motion_min, \
				pp->reg_bnr_ac_detail_th1, pp->reg_bnr_ac_detail_th2, pp->reg_bnr_ac_detail_th3, \
				pp->reg_bnr_ac_detail_th4, pp->reg_bnr_ac_pos_gain_h0, pp->reg_bnr_ac_pos_gain_h1, \
				pp->reg_bnr_ac_pos_gain_h2, pp->reg_bnr_ac_pos_gain_h3, pp->reg_bnr_ac_pos_gain_l0, \
				pp->reg_bnr_ac_pos_gain_l1, pp->reg_bnr_ac_pos_gain_l2, pp->reg_bnr_ac_pos_gain_l3, \
				pp->reg_bnr_ac_detail_max, pp->reg_bnr_ac_detail_min, \
				pp->reg_bnr_diff_l, pp->reg_bnr_diff_p, \
				/* mnr 18 */
				pp->reg_h_expend, pp->reg_gain_flt_size, pp->reg_mnr_s1_mmd_min, \
				pp->reg_mnr_s2_ratio_min, pp->reg_mnr_s2_ratio_max, pp->reg_mnr_s2_mmd_min, \
				pp->reg_filter_x0, pp->reg_filter_x1, pp->reg_filter_y0, pp->reg_filter_y1, \
				pp->reg_motion_mnr_en, pp->reg_motion_mnr_filter, pp->reg_mnr_motion_min, \
				pp->reg_mnr_motion_max, pp->reg_motion_mnr_x0, pp->reg_motion_mnr_x1, \
				pp->reg_motion_mnr_y0, pp->reg_motion_mnr_y1);
			if(PE_CHECK_WIN0(pp->win_id))
			{
				#ifdef PE_HW_M14B_USE_REG_CTRL
				#ifdef PE_FWI_M14_ADD_FW_DNR_DATA
				PE_FWI_M14_DNR_DETAIL fw_param;
				fw_param.reg_dc_detail_en       = GET_BITS(pp->reg_dc_detail_en,0,1);
				fw_param.reg_dc_blur_sel        = GET_BITS(pp->reg_dc_blur_sel,0,1);
				fw_param.reg_var_v_gain         = GET_BITS(pp->reg_var_v_gain,0,4);
				fw_param.reg_var_h_gain         = GET_BITS(pp->reg_var_h_gain,0,4);
				fw_param.reg_var_cut_resolution = GET_BITS(pp->reg_var_cut_resolution,0,4);
				fw_param.reg_dc_protection_th   = GET_BITS(pp->reg_dc_protection_th,0,8);
				fw_param.reg_bnr_ac_detail_th1  = GET_BITS(pp->reg_bnr_ac_detail_th1,0,8);
				fw_param.reg_bnr_ac_detail_th2  = GET_BITS(pp->reg_bnr_ac_detail_th2,0,8);
				fw_param.reg_bnr_ac_detail_th3  = GET_BITS(pp->reg_bnr_ac_detail_th3,0,8);
				fw_param.reg_bnr_ac_detail_th4  = GET_BITS(pp->reg_bnr_ac_detail_th4,0,8);
				fw_param.reg_bnr_ac_pos_gain_h0 = GET_BITS(pp->reg_bnr_ac_pos_gain_h0,0,8);
				fw_param.reg_bnr_ac_pos_gain_h1 = GET_BITS(pp->reg_bnr_ac_pos_gain_h1,0,8);
				fw_param.reg_bnr_ac_pos_gain_h2 = GET_BITS(pp->reg_bnr_ac_pos_gain_h2,0,8);
				fw_param.reg_bnr_ac_pos_gain_h3 = GET_BITS(pp->reg_bnr_ac_pos_gain_h3,0,8);
				fw_param.reg_bnr_ac_pos_gain_l0 = GET_BITS(pp->reg_bnr_ac_pos_gain_l0,0,8);
				fw_param.reg_bnr_ac_pos_gain_l1 = GET_BITS(pp->reg_bnr_ac_pos_gain_l1,0,8);
				fw_param.reg_bnr_ac_pos_gain_l2 = GET_BITS(pp->reg_bnr_ac_pos_gain_l2,0,8);
				fw_param.reg_bnr_ac_pos_gain_l3 = GET_BITS(pp->reg_bnr_ac_pos_gain_l3,0,8);
				fw_param.reg_bnr_ac_detail_max  = GET_BITS(pp->reg_bnr_ac_detail_max,0,8);
				fw_param.reg_bnr_ac_detail_min  = GET_BITS(pp->reg_bnr_ac_detail_min,0,8);
				fw_param.reg_bnr_diff_l         = GET_BITS(pp->reg_bnr_diff_l,0,8);
				fw_param.reg_bnr_diff_p         = GET_BITS(pp->reg_bnr_diff_p,0,8);
				ret = PE_FWI_M14_SetDnrDetail(&fw_param);
				PE_NRD_HW_M14_CHECK_CODE(ret, break, \
					"[%s,%d] PE_FWI_M14_SetDnrDetail() error.\n", __F__, __L__);
				/* dc bnr 18 */
				PE_NRD_HW_M14B_P0D_WR03(p0d_dc_bnr_ctrl_2,reg_dc_var_en,		GET_BITS(pp->reg_dc_var_en,0,1),\
														reg_dc_motion_en,		GET_BITS(pp->reg_dc_motion_en,0,1),\
														reg_dc_protection_en,	GET_BITS(pp->reg_dc_protection_en,0,1));
				PE_NRD_HW_M14B_P0D_SET(p0d_dc_bnr_ctrl_2,reg_dc_detail_en,		GET_BITS(pp->reg_dc_detail_en,0,1));
				PE_NRD_HW_M14B_P0D_WR02(p0d_dc_bnr_ctrl_0,reg_dc_motion_max,	GET_BITS(pp->reg_dc_motion_max,0,8),\
														reg_dc_motion_min,		GET_BITS(pp->reg_dc_motion_min,0,8));
				PE_NRD_HW_M14B_P0D_SET(p0d_dc_bnr_ctrl_0,reg_dc_blur_sel,		GET_BITS(pp->reg_dc_blur_sel,0,1));
				PE_NRD_HW_M14B_P0D_WR02(p0d_dc_bnr_ctrl_1,reg_dc_detail_max,	GET_BITS(pp->reg_dc_detail_max,0,8),\
														reg_dc_detail_min,		GET_BITS(pp->reg_dc_detail_min,0,8));
				PE_NRD_HW_M14B_P0D_SET(p0d_dc_bnr_ctrl_1,reg_var_v_gain,		GET_BITS(pp->reg_var_v_gain,0,4));
				PE_NRD_HW_M14B_P0D_SET(p0d_dc_bnr_ctrl_1,reg_var_h_gain,		GET_BITS(pp->reg_var_h_gain,0,4));
				PE_NRD_HW_M14B_P0D_SET(p0d_dc_bnr_ctrl_1,reg_var_cut_resolution,GET_BITS(pp->reg_var_cut_resolution,0,4));
				PE_NRD_HW_M14B_P0D_WR01(p0d_dc_bnr_ctrl_2,reg_dc_global_motion_th,GET_BITS(pp->reg_dc_global_motion_th,0,4));
				PE_NRD_HW_M14B_P0D_SET(p0d_dc_bnr_ctrl_2,reg_dc_protection_th,	GET_BITS(pp->reg_dc_protection_th,0,8));
				PE_NRD_HW_M14B_P0D_WR04(p0d_dc_bnr_ctrl_3,reg_dc_bnr_var_th0,	GET_BITS(pp->reg_dc_bnr_var_th0,0,8),\
														reg_dc_bnr_var_th1,		GET_BITS(pp->reg_dc_bnr_var_th1,0,8),\
														reg_dc_bnr_var_th2,		GET_BITS(pp->reg_dc_bnr_var_th2,0,8),\
														reg_dc_bnr_var_th3,		GET_BITS(pp->reg_dc_bnr_var_th3,0,8));
				/* ac bnr 23 */
				PE_NRD_HW_M14B_P0D_WR02(p0d_ac_bnr_ctrl_0,reg_bnr_ac_diff_min_v_th,	GET_BITS(pp->reg_bnr_ac_diff_min_v_th,0,8),\
														reg_bnr_ac_diff_min_h_th,	GET_BITS(pp->reg_bnr_ac_diff_min_h_th,0,8));
				PE_NRD_HW_M14B_P0D_WR03(p0d_ac_bnr_ctrl_1,reg_bnr_ac_global_motion_th,GET_BITS(pp->reg_bnr_ac_global_motion_th,0,8),\
														reg_bnr_ac_acness_max,		GET_BITS(pp->reg_bnr_ac_acness_max,0,8),\
														reg_bnr_ac_acness_min,		GET_BITS(pp->reg_bnr_ac_acness_min,0,8));
				PE_NRD_HW_M14B_P0D_WR02(p0d_ac_bnr_ctrl_4,reg_bnr_ac_motion_max,	GET_BITS(pp->reg_bnr_ac_motion_max,0,8),\
														reg_bnr_ac_motion_min,		GET_BITS(pp->reg_bnr_ac_motion_min,0,8));
				PE_NRD_HW_M14B_P0D_SET(p0d_ac_bnr_ctrl_5,reg_bnr_ac_detail_th1,		GET_BITS(pp->reg_bnr_ac_detail_th1,0,8));
				PE_NRD_HW_M14B_P0D_SET(p0d_ac_bnr_ctrl_5,reg_bnr_ac_detail_th2,		GET_BITS(pp->reg_bnr_ac_detail_th2,0,8));
				PE_NRD_HW_M14B_P0D_SET(p0d_ac_bnr_ctrl_5,reg_bnr_ac_detail_th3,		GET_BITS(pp->reg_bnr_ac_detail_th3,0,8));
				PE_NRD_HW_M14B_P0D_SET(p0d_ac_bnr_ctrl_5,reg_bnr_ac_detail_th4,		GET_BITS(pp->reg_bnr_ac_detail_th4,0,8));
				PE_NRD_HW_M14B_P0D_SET(p0d_ac_bnr_ctrl_6,reg_bnr_ac_pos_gain_h0,	GET_BITS(pp->reg_bnr_ac_pos_gain_h0,0,8));
				PE_NRD_HW_M14B_P0D_SET(p0d_ac_bnr_ctrl_6,reg_bnr_ac_pos_gain_h1,	GET_BITS(pp->reg_bnr_ac_pos_gain_h1,0,8));
				PE_NRD_HW_M14B_P0D_SET(p0d_ac_bnr_ctrl_6,reg_bnr_ac_pos_gain_h2,	GET_BITS(pp->reg_bnr_ac_pos_gain_h2,0,8));
				PE_NRD_HW_M14B_P0D_SET(p0d_ac_bnr_ctrl_7,reg_bnr_ac_pos_gain_h3,	GET_BITS(pp->reg_bnr_ac_pos_gain_h3,0,8));
				PE_NRD_HW_M14B_P0D_SET(p0d_ac_bnr_ctrl_7,reg_bnr_ac_pos_gain_l0,	GET_BITS(pp->reg_bnr_ac_pos_gain_l0,0,8));
				PE_NRD_HW_M14B_P0D_SET(p0d_ac_bnr_ctrl_7,reg_bnr_ac_pos_gain_l1,	GET_BITS(pp->reg_bnr_ac_pos_gain_l1,0,8));
				PE_NRD_HW_M14B_P0D_SET(p0d_ac_bnr_ctrl_7,reg_bnr_ac_pos_gain_l2,	GET_BITS(pp->reg_bnr_ac_pos_gain_l2,0,8));
				PE_NRD_HW_M14B_P0D_SET(p0d_ac_bnr_ctrl_8,reg_bnr_ac_pos_gain_l3,	GET_BITS(pp->reg_bnr_ac_pos_gain_l3,0,8));
				PE_NRD_HW_M14B_P0D_SET(p0d_detail_ctrl,reg_bnr_ac_detail_max,		GET_BITS(pp->reg_bnr_ac_detail_max,0,8));
				PE_NRD_HW_M14B_P0D_SET(p0d_detail_ctrl,reg_bnr_ac_detail_min,		GET_BITS(pp->reg_bnr_ac_detail_min,0,8));
				PE_NRD_HW_M14B_P0D_SET(p0d_detail_ctrl,reg_bnr_diff_l,				GET_BITS(pp->reg_bnr_diff_l,0,8));
				PE_NRD_HW_M14B_P0D_SET(p0d_detail_ctrl,reg_bnr_diff_p,				GET_BITS(pp->reg_bnr_diff_p,0,8));
				/* mnr 8 */
				PE_NRD_HW_M14B_P0D_WR02(p0d_mnr_ctrl_0,reg_h_expend,			GET_BITS(pp->reg_h_expend,0,1),\
													reg_gain_flt_size,		GET_BITS(pp->reg_gain_flt_size,0,1));
				PE_NRD_HW_M14B_P0D_WR04(p0d_mnr_ctrl_1,reg_mnr_s1_mmd_min,		GET_BITS(pp->reg_mnr_s1_mmd_min,0,8),\
													reg_mnr_s2_ratio_min,	GET_BITS(pp->reg_mnr_s2_ratio_min,0,8),\
													reg_mnr_s2_ratio_max,	GET_BITS(pp->reg_mnr_s2_ratio_max,0,8),\
													reg_mnr_s2_mmd_min,		GET_BITS(pp->reg_mnr_s2_mmd_min,0,8));
				PE_NRD_HW_M14B_P0D_WR04(p0d_mnr_ctrl_3,reg_filter_x0,			GET_BITS(pp->reg_filter_x0,0,8),\
													reg_filter_x1,			GET_BITS(pp->reg_filter_x1,0,8),\
													reg_filter_y0,			GET_BITS(pp->reg_filter_y0,0,8),\
													reg_filter_y1,			GET_BITS(pp->reg_filter_y1,0,8));
				PE_NRD_HW_M14B_P0D_WR04(p0d_mnr_ctrl_4,reg_motion_mnr_en,		GET_BITS(pp->reg_motion_mnr_en,0,1),\
													reg_motion_mnr_filter,	GET_BITS(pp->reg_motion_mnr_filter,0,1),\
													reg_mnr_motion_min,		GET_BITS(pp->reg_mnr_motion_min,0,8),\
													reg_mnr_motion_max,		GET_BITS(pp->reg_mnr_motion_max,0,8));
				PE_NRD_HW_M14B_P0D_WR04(p0d_mnr_ctrl_5,reg_motion_mnr_x0,		GET_BITS(pp->reg_motion_mnr_x0,0,8),\
													reg_motion_mnr_x1,		GET_BITS(pp->reg_motion_mnr_x1,0,8),\
													reg_motion_mnr_y0,		GET_BITS(pp->reg_motion_mnr_y0,0,8),\
													reg_motion_mnr_y1,		GET_BITS(pp->reg_motion_mnr_y1,0,8));
				#else	//PE_FWI_M14_ADD_FW_DNR_DATA
				/* dc bnr 18 */
				PE_NRD_HW_M14B_P0D_WR04(p0d_dc_bnr_ctrl_2,reg_dc_var_en,			GET_BITS(pp->reg_dc_var_en,0,1),\
														reg_dc_motion_en,		GET_BITS(pp->reg_dc_motion_en,0,1),\
														reg_dc_protection_en,	GET_BITS(pp->reg_dc_protection_en,0,1),
														reg_dc_detail_en,		GET_BITS(pp->reg_dc_detail_en,0,1));
				PE_NRD_HW_M14B_P0D_WR03(p0d_dc_bnr_ctrl_0,reg_dc_blur_sel,		GET_BITS(pp->reg_dc_blur_sel,0,1),\
														reg_dc_motion_max,		GET_BITS(pp->reg_dc_motion_max,0,8),\
														reg_dc_motion_min,		GET_BITS(pp->reg_dc_motion_min,0,8));
				PE_NRD_HW_M14B_P0D_WR05(p0d_dc_bnr_ctrl_1,reg_dc_detail_max,		GET_BITS(pp->reg_dc_detail_max,0,8),\
														reg_dc_detail_min,		GET_BITS(pp->reg_dc_detail_min,0,8),\
														reg_var_v_gain,			GET_BITS(pp->reg_var_v_gain,0,4),\
														reg_var_h_gain,			GET_BITS(pp->reg_var_h_gain,0,4),\
														reg_var_cut_resolution,	GET_BITS(pp->reg_var_cut_resolution,0,4));
				PE_NRD_HW_M14B_P0D_WR02(p0d_dc_bnr_ctrl_2,reg_dc_global_motion_th,GET_BITS(pp->reg_dc_global_motion_th,0,4),\
														reg_dc_protection_th,	GET_BITS(pp->reg_dc_protection_th,0,8));
				PE_NRD_HW_M14B_P0D_WR04(p0d_dc_bnr_ctrl_3,reg_dc_bnr_var_th0,		GET_BITS(pp->reg_dc_bnr_var_th0,0,8),\
														reg_dc_bnr_var_th1,		GET_BITS(pp->reg_dc_bnr_var_th1,0,8),\
														reg_dc_bnr_var_th2,		GET_BITS(pp->reg_dc_bnr_var_th2,0,8),\
														reg_dc_bnr_var_th3,		GET_BITS(pp->reg_dc_bnr_var_th3,0,8));
				/* ac bnr 23 */
				PE_NRD_HW_M14B_P0D_WR02(p0d_ac_bnr_ctrl_0,reg_bnr_ac_diff_min_v_th,	GET_BITS(pp->reg_bnr_ac_diff_min_v_th,0,8),\
														reg_bnr_ac_diff_min_h_th,	GET_BITS(pp->reg_bnr_ac_diff_min_h_th,0,8));
				PE_NRD_HW_M14B_P0D_WR03(p0d_ac_bnr_ctrl_1,reg_bnr_ac_global_motion_th,GET_BITS(pp->reg_bnr_ac_global_motion_th,0,8),\
														reg_bnr_ac_acness_max,		GET_BITS(pp->reg_bnr_ac_acness_max,0,8),\
														reg_bnr_ac_acness_min,		GET_BITS(pp->reg_bnr_ac_acness_min,0,8));
				PE_NRD_HW_M14B_P0D_WR02(p0d_ac_bnr_ctrl_4,reg_bnr_ac_motion_max,		GET_BITS(pp->reg_bnr_ac_motion_max,0,8),\
														reg_bnr_ac_motion_min,		GET_BITS(pp->reg_bnr_ac_motion_min,0,8));
				PE_NRD_HW_M14B_P0D_WR04(p0d_ac_bnr_ctrl_5,reg_bnr_ac_detail_th1,		GET_BITS(pp->reg_bnr_ac_detail_th1,0,8),\
														reg_bnr_ac_detail_th2,		GET_BITS(pp->reg_bnr_ac_detail_th2,0,8),\
														reg_bnr_ac_detail_th3,		GET_BITS(pp->reg_bnr_ac_detail_th3,0,8),\
														reg_bnr_ac_detail_th4,		GET_BITS(pp->reg_bnr_ac_detail_th4,0,8));
				PE_NRD_HW_M14B_P0D_WR03(p0d_ac_bnr_ctrl_6,reg_bnr_ac_pos_gain_h0,		GET_BITS(pp->reg_bnr_ac_pos_gain_h0,0,8),\
														reg_bnr_ac_pos_gain_h1,		GET_BITS(pp->reg_bnr_ac_pos_gain_h1,0,8),\
														reg_bnr_ac_pos_gain_h2,		GET_BITS(pp->reg_bnr_ac_pos_gain_h2,0,8));
				PE_NRD_HW_M14B_P0D_WR04(p0d_ac_bnr_ctrl_7,reg_bnr_ac_pos_gain_h3,		GET_BITS(pp->reg_bnr_ac_pos_gain_h3,0,8),\
														reg_bnr_ac_pos_gain_l0,		GET_BITS(pp->reg_bnr_ac_pos_gain_l0,0,8),\
														reg_bnr_ac_pos_gain_l1,		GET_BITS(pp->reg_bnr_ac_pos_gain_l1,0,8),\
														reg_bnr_ac_pos_gain_l2,		GET_BITS(pp->reg_bnr_ac_pos_gain_l2,0,8));
				PE_NRD_HW_M14B_P0D_WR01(p0d_ac_bnr_ctrl_8,reg_bnr_ac_pos_gain_l3,		GET_BITS(pp->reg_bnr_ac_pos_gain_l3,0,8));
				PE_NRD_HW_M14B_P0D_WR04(p0d_detail_ctrl,reg_bnr_ac_detail_max,		GET_BITS(pp->reg_bnr_ac_detail_max,0,8),\
														reg_bnr_ac_detail_min,		GET_BITS(pp->reg_bnr_ac_detail_min,0,8),\
														reg_bnr_diff_l,				GET_BITS(pp->reg_bnr_diff_l,0,8),\
														reg_bnr_diff_p,				GET_BITS(pp->reg_bnr_diff_p,0,8));
				/* mnr 8 */
				PE_NRD_HW_M14B_P0D_WR02(p0d_mnr_ctrl_0,reg_h_expend,			GET_BITS(pp->reg_h_expend,0,1),\
													reg_gain_flt_size,		GET_BITS(pp->reg_gain_flt_size,0,1));
				PE_NRD_HW_M14B_P0D_WR04(p0d_mnr_ctrl_1,reg_mnr_s1_mmd_min,		GET_BITS(pp->reg_mnr_s1_mmd_min,0,8),\
													reg_mnr_s2_ratio_min,	GET_BITS(pp->reg_mnr_s2_ratio_min,0,8),\
													reg_mnr_s2_ratio_max,	GET_BITS(pp->reg_mnr_s2_ratio_max,0,8),\
													reg_mnr_s2_mmd_min,		GET_BITS(pp->reg_mnr_s2_mmd_min,0,8));
				PE_NRD_HW_M14B_P0D_WR04(p0d_mnr_ctrl_3,reg_filter_x0,			GET_BITS(pp->reg_filter_x0,0,8),\
													reg_filter_x1,			GET_BITS(pp->reg_filter_x1,0,8),\
													reg_filter_y0,			GET_BITS(pp->reg_filter_y0,0,8),\
													reg_filter_y1,			GET_BITS(pp->reg_filter_y1,0,8));
				PE_NRD_HW_M14B_P0D_WR04(p0d_mnr_ctrl_4,reg_motion_mnr_en,		GET_BITS(pp->reg_motion_mnr_en,0,1),\
													reg_motion_mnr_filter,	GET_BITS(pp->reg_motion_mnr_filter,0,1),\
													reg_mnr_motion_min,		GET_BITS(pp->reg_mnr_motion_min,0,8),\
													reg_mnr_motion_max,		GET_BITS(pp->reg_mnr_motion_max,0,8));
				PE_NRD_HW_M14B_P0D_WR04(p0d_mnr_ctrl_5,reg_motion_mnr_x0,		GET_BITS(pp->reg_motion_mnr_x0,0,8),\
													reg_motion_mnr_x1,		GET_BITS(pp->reg_motion_mnr_x1,0,8),\
													reg_motion_mnr_y0,		GET_BITS(pp->reg_motion_mnr_y0,0,8),\
													reg_motion_mnr_y1,		GET_BITS(pp->reg_motion_mnr_y1,0,8));
				#endif	//PE_FWI_M14_ADD_FW_DNR_DATA
				#else
				/* dc bnr 18 */
				PE_P0D_M14B0_QWr04(p0d_dc_bnr_ctrl_2,	reg_dc_var_en,			GET_BITS(pp->reg_dc_var_en,0,1),\
														reg_dc_motion_en,		GET_BITS(pp->reg_dc_motion_en,0,1),\
														reg_dc_protection_en,	GET_BITS(pp->reg_dc_protection_en,0,1),
														reg_dc_detail_en,		GET_BITS(pp->reg_dc_detail_en,0,1));
				PE_P0D_M14B0_QWr03(p0d_dc_bnr_ctrl_0,	reg_dc_blur_sel,		GET_BITS(pp->reg_dc_blur_sel,0,1),\
														reg_dc_motion_max,		GET_BITS(pp->reg_dc_motion_max,0,8),\
														reg_dc_motion_min,		GET_BITS(pp->reg_dc_motion_min,0,8));
				PE_P0D_M14B0_QWr05(p0d_dc_bnr_ctrl_1,	reg_dc_detail_max,		GET_BITS(pp->reg_dc_detail_max,0,8),\
														reg_dc_detail_min,		GET_BITS(pp->reg_dc_detail_min,0,8),\
														reg_var_v_gain,			GET_BITS(pp->reg_var_v_gain,0,4),\
														reg_var_h_gain,			GET_BITS(pp->reg_var_h_gain,0,4),\
														reg_var_cut_resolution,	GET_BITS(pp->reg_var_cut_resolution,0,4));
				PE_P0D_M14B0_QWr02(p0d_dc_bnr_ctrl_2,	reg_dc_global_motion_th,GET_BITS(pp->reg_dc_global_motion_th,0,4),\
														reg_dc_protection_th,	GET_BITS(pp->reg_dc_protection_th,0,8));
				PE_P0D_M14B0_QWr04(p0d_dc_bnr_ctrl_3,	reg_dc_bnr_var_th0,		GET_BITS(pp->reg_dc_bnr_var_th0,0,8),\
														reg_dc_bnr_var_th1,		GET_BITS(pp->reg_dc_bnr_var_th1,0,8),\
														reg_dc_bnr_var_th2,		GET_BITS(pp->reg_dc_bnr_var_th2,0,8),\
														reg_dc_bnr_var_th3,		GET_BITS(pp->reg_dc_bnr_var_th3,0,8));
				/* ac bnr 23 */
				PE_P0D_M14B0_QWr02(p0d_ac_bnr_ctrl_0,	reg_bnr_ac_diff_min_v_th,	GET_BITS(pp->reg_bnr_ac_diff_min_v_th,0,8),\
														reg_bnr_ac_diff_min_h_th,	GET_BITS(pp->reg_bnr_ac_diff_min_h_th,0,8));
				PE_P0D_M14B0_QWr03(p0d_ac_bnr_ctrl_1,	reg_bnr_ac_global_motion_th,GET_BITS(pp->reg_bnr_ac_global_motion_th,0,8),\
														reg_bnr_ac_acness_max,		GET_BITS(pp->reg_bnr_ac_acness_max,0,8),\
														reg_bnr_ac_acness_min,		GET_BITS(pp->reg_bnr_ac_acness_min,0,8));
				PE_P0D_M14B0_QWr02(p0d_ac_bnr_ctrl_4,	reg_bnr_ac_motion_max,		GET_BITS(pp->reg_bnr_ac_motion_max,0,8),\
														reg_bnr_ac_motion_min,		GET_BITS(pp->reg_bnr_ac_motion_min,0,8));
				PE_P0D_M14B0_QWr04(p0d_ac_bnr_ctrl_5,	reg_bnr_ac_detail_th1,		GET_BITS(pp->reg_bnr_ac_detail_th1,0,8),\
														reg_bnr_ac_detail_th2,		GET_BITS(pp->reg_bnr_ac_detail_th2,0,8),\
														reg_bnr_ac_detail_th3,		GET_BITS(pp->reg_bnr_ac_detail_th3,0,8),\
														reg_bnr_ac_detail_th4,		GET_BITS(pp->reg_bnr_ac_detail_th4,0,8));
				PE_P0D_M14B0_QWr03(p0d_ac_bnr_ctrl_6,	reg_bnr_ac_pos_gain_h0,		GET_BITS(pp->reg_bnr_ac_pos_gain_h0,0,8),\
														reg_bnr_ac_pos_gain_h1,		GET_BITS(pp->reg_bnr_ac_pos_gain_h1,0,8),\
														reg_bnr_ac_pos_gain_h2,		GET_BITS(pp->reg_bnr_ac_pos_gain_h2,0,8));
				PE_P0D_M14B0_QWr04(p0d_ac_bnr_ctrl_7,	reg_bnr_ac_pos_gain_h3,		GET_BITS(pp->reg_bnr_ac_pos_gain_h3,0,8),\
														reg_bnr_ac_pos_gain_l0,		GET_BITS(pp->reg_bnr_ac_pos_gain_l0,0,8),\
														reg_bnr_ac_pos_gain_l1,		GET_BITS(pp->reg_bnr_ac_pos_gain_l1,0,8),\
														reg_bnr_ac_pos_gain_l2,		GET_BITS(pp->reg_bnr_ac_pos_gain_l2,0,8));
				PE_P0D_M14B0_QWr01(p0d_ac_bnr_ctrl_8,	reg_bnr_ac_pos_gain_l3,		GET_BITS(pp->reg_bnr_ac_pos_gain_l3,0,8));
				PE_P0D_M14B0_QWr04(p0d_detail_ctrl,		reg_bnr_ac_detail_max,		GET_BITS(pp->reg_bnr_ac_detail_max,0,8),\
														reg_bnr_ac_detail_min,		GET_BITS(pp->reg_bnr_ac_detail_min,0,8),\
														reg_bnr_diff_l,				GET_BITS(pp->reg_bnr_diff_l,0,8),\
														reg_bnr_diff_p,				GET_BITS(pp->reg_bnr_diff_p,0,8));
				/* mnr 8 */
				PE_P0D_M14B0_QWr02(p0d_mnr_ctrl_0,	reg_h_expend,			GET_BITS(pp->reg_h_expend,0,1),\
													reg_gain_flt_size,		GET_BITS(pp->reg_gain_flt_size,0,1));
				PE_P0D_M14B0_QWr04(p0d_mnr_ctrl_1,	reg_mnr_s1_mmd_min,		GET_BITS(pp->reg_mnr_s1_mmd_min,0,8),\
													reg_mnr_s2_ratio_min,	GET_BITS(pp->reg_mnr_s2_ratio_min,0,8),\
													reg_mnr_s2_ratio_max,	GET_BITS(pp->reg_mnr_s2_ratio_max,0,8),\
													reg_mnr_s2_mmd_min,		GET_BITS(pp->reg_mnr_s2_mmd_min,0,8));
				PE_P0D_M14B0_QWr04(p0d_mnr_ctrl_3,	reg_filter_x0,			GET_BITS(pp->reg_filter_x0,0,8),\
													reg_filter_x1,			GET_BITS(pp->reg_filter_x1,0,8),\
													reg_filter_y0,			GET_BITS(pp->reg_filter_y0,0,8),\
													reg_filter_y1,			GET_BITS(pp->reg_filter_y1,0,8));
				PE_P0D_M14B0_QWr04(p0d_mnr_ctrl_4,	reg_motion_mnr_en,		GET_BITS(pp->reg_motion_mnr_en,0,1),\
													reg_motion_mnr_filter,	GET_BITS(pp->reg_motion_mnr_filter,0,1),\
													reg_mnr_motion_min,		GET_BITS(pp->reg_mnr_motion_min,0,8),\
													reg_mnr_motion_max,		GET_BITS(pp->reg_mnr_motion_max,0,8));
				PE_P0D_M14B0_QWr04(p0d_mnr_ctrl_5,	reg_motion_mnr_x0,		GET_BITS(pp->reg_motion_mnr_x0,0,8),\
													reg_motion_mnr_x1,		GET_BITS(pp->reg_motion_mnr_x1,0,8),\
													reg_motion_mnr_y0,		GET_BITS(pp->reg_motion_mnr_y0,0,8),\
													reg_motion_mnr_y1,		GET_BITS(pp->reg_motion_mnr_y1,0,8));
				#endif
			}
		}
		else if(PE_KDRV_VER_M14AX)
		{
			LX_PE_NRD_DNR2_DETAIL_T *pp=(LX_PE_NRD_DNR2_DETAIL_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			PE_NRD_HW_M14_DBG_PRINT("set[%d] \n"\
				"mnr   : h_expend:%d,  gain_flt_size:%d,  mmd_min s1,s2:%d,%d ratio_min,max: %d,%d \n"\
				"mnr   : filter x0,x1,y0,y1:%d,%d,%d,%d\n"\
				"mnr   : motion_en:%d, filter:%d, min:%d, max:%d\n"\
				"mnr   : motion x0,x1,y0,y1:%d,%d,%d,%d\n"\
				"ac_bnr: protect_en:%d, sdhd_sel(source type):%d, flt_thpe:%d,\n"\
				"ac_bnr: strength h_x0,h_x1,v_x0,v_x1:%d,%d,%d,%d, detect_min_th:%d\n"\
				"dc_bnr: var_en:%d,  motion_en:%d,  acadaptive:%d, sdhd_sel:%d\n"\
				"dc_bnr: blur_sel:%d, protection_en:%d \n"\
				"dc_bnr: var_th0,th1,th2,th3%d,%d,%d,%d, motion_th0,th1,th2,th3%d,%d,%d,%d\n"\
				"dc_bnr: chroma_var:%d, var_h_gain:%d, protection_th:%d\n",pp->win_id,\
				/* mnr */
				pp->reg_h_expend,pp->reg_gain_flt_size,\
				pp->reg_mnr_s1_mmd_min,pp->reg_mnr_s2_ratio_min,pp->reg_mnr_s2_ratio_max,pp->reg_mnr_s2_mmd_min,\
				pp->reg_filter_x0,pp->reg_filter_x1,pp->reg_filter_y0,pp->reg_filter_y1,\
				pp->reg_motion_mnr_en,pp->reg_motion_mnr_filter,pp->reg_mnr_motion_min,pp->reg_mnr_motion_max,\
				pp->reg_motion_mnr_x0,pp->reg_motion_mnr_x1,pp->reg_motion_mnr_y0,pp->reg_motion_mnr_y1,\
				/* ac_bnr */
				pp->motion_protect_enable,pp->source_type,pp->fiter_type,\
				pp->strength_h_x0,pp->strength_h_x1,pp->strength_v_x0,pp->strength_v_x1,\
				pp->detect_min_th,\
				/* dc_bnr */
				pp->reg_dc_bnr_var_en,pp->reg_dc_bnr_motion_en,pp->reg_dc_bnr_acadaptive,pp->reg_dc_bnr_sdhd_sel,\
				pp->reg_dc_blur_sel,pp->reg_dc_protection_en,\
				pp->reg_dc_bnr_var_th0,pp->reg_dc_bnr_var_th1,\
				pp->reg_dc_bnr_var_th2,pp->reg_dc_bnr_var_th3,\
				pp->reg_dc_bnr_motion_th0,pp->reg_dc_bnr_motion_th1,\
				pp->reg_dc_bnr_motion_th2,pp->reg_dc_bnr_motion_th3,\
				pp->reg_dc_chroma_variance,pp->reg_dc_var_h_gain,pp->reg_dc_protection_th);
			if(PE_CHECK_WIN0(pp->win_id))
			{
				/* mnr */
				PE_P0L_M14A0_QWr02(p0l_mnr_ctrl_00,	reg_h_expend,		GET_BITS(pp->reg_h_expend,0,1),\
													reg_gain_flt_size,	GET_BITS(pp->reg_gain_flt_size,0,1));

				PE_P0L_M14A0_QWr04(p0l_mnr_ctrl_01,	reg_mnr_s1_mmd_min,		GET_BITS(pp->reg_mnr_s1_mmd_min,0,8),\
													reg_mnr_s2_ratio_min,	GET_BITS(pp->reg_mnr_s2_ratio_min,0,8),\
													reg_mnr_s2_ratio_max,	GET_BITS(pp->reg_mnr_s2_ratio_max,0,8),\
													reg_mnr_s2_mmd_min,		GET_BITS(pp->reg_mnr_s2_mmd_min,0,8));
				PE_P0L_M14A0_QWr04(p0l_mnr_ctrl_03,	reg_filter_x0,	GET_BITS(pp->reg_filter_x0,0,8),\
													reg_filter_x1,	GET_BITS(pp->reg_filter_x1,0,8),\
													reg_filter_y0,	GET_BITS(pp->reg_filter_y0,0,8),\
													reg_filter_y1,	GET_BITS(pp->reg_filter_y1,0,8));
				PE_P0L_M14A0_QWr04(p0l_mnr_ctrl_04,	reg_motion_mnr_en,		GET_BITS(pp->reg_motion_mnr_en,0,1),\
													reg_motion_mnr_filter,	GET_BITS(pp->reg_motion_mnr_filter,0,1),\
													reg_mnr_motion_min,		GET_BITS(pp->reg_mnr_motion_min,0,8),\
													reg_mnr_motion_max,		GET_BITS(pp->reg_mnr_motion_max,0,8));
				PE_P0L_M14A0_QWr04(p0l_mnr_ctrl_05,	reg_motion_mnr_x0,		GET_BITS(pp->reg_motion_mnr_x0,0,8),\
													reg_motion_mnr_x1,		GET_BITS(pp->reg_motion_mnr_x1,0,8),\
													reg_motion_mnr_y0,		GET_BITS(pp->reg_motion_mnr_y0,0,8),\
													reg_motion_mnr_y1,		GET_BITS(pp->reg_motion_mnr_y1,0,8));
				/* ac_bnr */
				PE_P0L_M14A0_QWr03(p0l_bnr_ac_ctrl_00,	reg_bnr_ac_protect_en,	GET_BITS(pp->motion_protect_enable,0,1),\
														reg_bnr_ac_sdhd_sel,	GET_BITS(pp->source_type,0,1),\
														reg_bnr_ac_flt_type,	GET_BITS(pp->fiter_type,0,1));
				PE_P0L_M14A0_QWr03(p0l_bnr_ac_ctrl_01,	strength_h_x0,	GET_BITS(pp->strength_h_x0,0,8),\
														strength_h_x1,	GET_BITS(pp->strength_h_x1,0,8),\
														detect_min_th,	GET_BITS(pp->detect_min_th,0,8));
				PE_P0L_M14A0_QWr02(p0l_bnr_ac_ctrl_02,	strength_v_x0,	GET_BITS(pp->strength_v_x0,0,8),\
														strength_v_x1,	GET_BITS(pp->strength_v_x1,0,8));
				/* dc_bnr */
				PE_P0L_M14A0_QWr04(p0l_bnr_dc_ctrl_00,	reg_dc_bnr_var_en,		GET_BITS(pp->reg_dc_bnr_var_en,0,1),\
														reg_dc_bnr_motion_en,	GET_BITS(pp->reg_dc_bnr_motion_en,0,1),\
														reg_dc_bnr_acadaptive,	GET_BITS(pp->reg_dc_bnr_acadaptive,0,1),
														reg_dc_bnr_sdhd_sel,	GET_BITS(pp->reg_dc_bnr_sdhd_sel,0,1));
				PE_P0L_M14A0_QWr02(p0l_bnr_dc_ctrl_00,	reg_dc_blur_sel,		GET_BITS(pp->reg_dc_blur_sel,0,2),\
														reg_dc_protection_en,	GET_BITS(pp->reg_dc_protection_en,0,1));
				PE_P0L_M14A0_QWr04(p0l_bnr_dc_ctrl_01,	reg_dc_bnr_var_th0,		GET_BITS(pp->reg_dc_bnr_var_th0,0,8),\
														reg_dc_bnr_var_th1,		GET_BITS(pp->reg_dc_bnr_var_th1,0,8),\
														reg_dc_bnr_var_th2,		GET_BITS(pp->reg_dc_bnr_var_th2,0,8),\
														reg_dc_bnr_var_th3,		GET_BITS(pp->reg_dc_bnr_var_th3,0,8));
				PE_P0L_M14A0_QWr04(p0l_bnr_dc_ctrl_02,	reg_dc_bnr_motion_th0,	GET_BITS(pp->reg_dc_bnr_motion_th0,0,8),\
														reg_dc_bnr_motion_th1,	GET_BITS(pp->reg_dc_bnr_motion_th1,0,8),\
														reg_dc_bnr_motion_th2,	GET_BITS(pp->reg_dc_bnr_motion_th2,0,8),\
														reg_dc_bnr_motion_th3,	GET_BITS(pp->reg_dc_bnr_motion_th3,0,8));
				PE_P0L_M14A0_QWr03(p0l_bnr_dc_ctrl_03,	reg_dc_chroma_variance,	GET_BITS(pp->reg_dc_chroma_variance,0,8),\
														reg_dc_var_h_gain,		GET_BITS(pp->reg_dc_var_h_gain,0,8),\
														reg_dc_protection_th,	GET_BITS(pp->reg_dc_protection_th,0,8));
			}
			if(PE_CHECK_WIN1(pp->win_id))
			{
				/* mnr */
				PE_P0R_M14A0_QWr02(p0r_mnr_ctrl_00,	reg_h_expend,		GET_BITS(pp->reg_h_expend,0,1),\
													reg_gain_flt_size,	GET_BITS(pp->reg_gain_flt_size,0,1));

				PE_P0R_M14A0_QWr04(p0r_mnr_ctrl_01,	reg_mnr_s1_mmd_min,		GET_BITS(pp->reg_mnr_s1_mmd_min,0,8),\
													reg_mnr_s2_ratio_min,	GET_BITS(pp->reg_mnr_s2_ratio_min,0,8),\
													reg_mnr_s2_ratio_max,	GET_BITS(pp->reg_mnr_s2_ratio_max,0,8),\
													reg_mnr_s2_mmd_min,		GET_BITS(pp->reg_mnr_s2_mmd_min,0,8));
				PE_P0R_M14A0_QWr04(p0r_mnr_ctrl_03,	reg_filter_x0,	GET_BITS(pp->reg_filter_x0,0,8),\
													reg_filter_x1,	GET_BITS(pp->reg_filter_x1,0,8),\
													reg_filter_y0,	GET_BITS(pp->reg_filter_y0,0,8),\
													reg_filter_y1,	GET_BITS(pp->reg_filter_y1,0,8));
				PE_P0R_M14A0_QWr04(p0r_mnr_ctrl_04,	reg_motion_mnr_en,		GET_BITS(pp->reg_motion_mnr_en,0,1),\
													reg_motion_mnr_filter,	GET_BITS(pp->reg_motion_mnr_filter,0,1),\
													reg_mnr_motion_min,		GET_BITS(pp->reg_mnr_motion_min,0,8),\
													reg_mnr_motion_max,		GET_BITS(pp->reg_mnr_motion_max,0,8));
				PE_P0R_M14A0_QWr04(p0r_mnr_ctrl_05,	reg_motion_mnr_x0,		GET_BITS(pp->reg_motion_mnr_x0,0,8),\
													reg_motion_mnr_x1,		GET_BITS(pp->reg_motion_mnr_x1,0,8),\
													reg_motion_mnr_y0,		GET_BITS(pp->reg_motion_mnr_y0,0,8),\
													reg_motion_mnr_y1,		GET_BITS(pp->reg_motion_mnr_y1,0,8));
				/* ac_bnr */
				PE_P0R_M14A0_QWr03(p0r_bnr_ac_ctrl_00,	reg_bnr_ac_protect_en,	GET_BITS(pp->motion_protect_enable,0,1),\
														reg_bnr_ac_sdhd_sel,	GET_BITS(pp->source_type,0,1),\
														reg_bnr_ac_flt_type,	GET_BITS(pp->fiter_type,0,1));
				PE_P0R_M14A0_QWr03(p0r_bnr_ac_ctrl_01,	strength_h_x0,	GET_BITS(pp->strength_h_x0,0,8),\
														strength_h_x1,	GET_BITS(pp->strength_h_x1,0,8),\
														detect_min_th,	GET_BITS(pp->detect_min_th,0,8));
				PE_P0R_M14A0_QWr02(p0r_bnr_ac_ctrl_02,	strength_v_x0,	GET_BITS(pp->strength_v_x0,0,8),\
														strength_v_x1,	GET_BITS(pp->strength_v_x1,0,8));
				/* dc_bnr */
				PE_P0R_M14A0_QWr04(p0r_bnr_dc_ctrl_00,	reg_dc_bnr_var_en,		GET_BITS(pp->reg_dc_bnr_var_en,0,1),\
														reg_dc_bnr_motion_en,	GET_BITS(pp->reg_dc_bnr_motion_en,0,1),\
														reg_dc_bnr_acadaptive,	GET_BITS(pp->reg_dc_bnr_acadaptive,0,1),
														reg_dc_bnr_sdhd_sel,	GET_BITS(pp->reg_dc_bnr_sdhd_sel,0,1));
				PE_P0R_M14A0_QWr02(p0r_bnr_dc_ctrl_00,	reg_dc_blur_sel,		GET_BITS(pp->reg_dc_blur_sel,0,2),\
														reg_dc_protection_en,	GET_BITS(pp->reg_dc_protection_en,0,1));
				PE_P0R_M14A0_QWr04(p0r_bnr_dc_ctrl_01,	reg_dc_bnr_var_th0,		GET_BITS(pp->reg_dc_bnr_var_th0,0,8),\
														reg_dc_bnr_var_th1,		GET_BITS(pp->reg_dc_bnr_var_th1,0,8),\
														reg_dc_bnr_var_th2,		GET_BITS(pp->reg_dc_bnr_var_th2,0,8),\
														reg_dc_bnr_var_th3,		GET_BITS(pp->reg_dc_bnr_var_th3,0,8));
				PE_P0R_M14A0_QWr04(p0r_bnr_dc_ctrl_02,	reg_dc_bnr_motion_th0,	GET_BITS(pp->reg_dc_bnr_motion_th0,0,8),\
														reg_dc_bnr_motion_th1,	GET_BITS(pp->reg_dc_bnr_motion_th1,0,8),\
														reg_dc_bnr_motion_th2,	GET_BITS(pp->reg_dc_bnr_motion_th2,0,8),\
														reg_dc_bnr_motion_th3,	GET_BITS(pp->reg_dc_bnr_motion_th3,0,8));
				PE_P0R_M14A0_QWr03(p0r_bnr_dc_ctrl_03,	reg_dc_chroma_variance,	GET_BITS(pp->reg_dc_chroma_variance,0,8),\
														reg_dc_var_h_gain,		GET_BITS(pp->reg_dc_var_h_gain,0,8),\
														reg_dc_protection_th,	GET_BITS(pp->reg_dc_protection_th,0,8));
			}
		}
		else
		{
			PE_NRD_HW_M14_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * get dnr detail ctrl
 * - use input struct LX_PE_NRD_DNR2_DETAIL_T for M14AX
 *
 * @param   *pstParams [in/out] void
 * @return  OK if success, ERROR otherwise.
 * @see		set discription
 * @author
 */
int PE_NRD_HW_M14_GetDnrDetailCtrl(void *pstParams)
{
	int ret = RET_OK;
	LX_PE_WIN_ID win_id;
	do{
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_M14BX)
		{
			LX_PE_NRD_DNR4_DETAIL_T *pp=(LX_PE_NRD_DNR4_DETAIL_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			win_id = PE_GET_CHECKED_WINID(pp->win_id);
			if(PE_CHECK_WIN0(win_id))
			{
				#ifdef PE_FWI_M14_ADD_FW_DNR_DATA
				PE_FWI_M14_SETTINGS_T fw_inf;
				ret = PE_FWI_M14_GetCurFwiSettings(&fw_inf);
				PE_NRD_HW_M14_CHECK_CODE(ret, break, \
					"[%s,%d] PE_FWI_M14_GetCurFwiSettings() error.\n", __F__, __L__);
				pp->reg_dc_detail_en       = fw_inf.dnr_detail.reg_dc_detail_en;
				pp->reg_dc_blur_sel        = fw_inf.dnr_detail.reg_dc_blur_sel;
				pp->reg_var_v_gain         = fw_inf.dnr_detail.reg_var_v_gain;
				pp->reg_var_h_gain         = fw_inf.dnr_detail.reg_var_h_gain;
				pp->reg_var_cut_resolution = fw_inf.dnr_detail.reg_var_cut_resolution;
				pp->reg_dc_protection_th   = fw_inf.dnr_detail.reg_dc_protection_th;
				pp->reg_bnr_ac_detail_th1  = fw_inf.dnr_detail.reg_bnr_ac_detail_th1;
				pp->reg_bnr_ac_detail_th2  = fw_inf.dnr_detail.reg_bnr_ac_detail_th2;
				pp->reg_bnr_ac_detail_th3  = fw_inf.dnr_detail.reg_bnr_ac_detail_th3;
				pp->reg_bnr_ac_detail_th4  = fw_inf.dnr_detail.reg_bnr_ac_detail_th4;
				pp->reg_bnr_ac_pos_gain_h0 = fw_inf.dnr_detail.reg_bnr_ac_pos_gain_h0;
				pp->reg_bnr_ac_pos_gain_h1 = fw_inf.dnr_detail.reg_bnr_ac_pos_gain_h1;
				pp->reg_bnr_ac_pos_gain_h2 = fw_inf.dnr_detail.reg_bnr_ac_pos_gain_h2;
				pp->reg_bnr_ac_pos_gain_h3 = fw_inf.dnr_detail.reg_bnr_ac_pos_gain_h3;
				pp->reg_bnr_ac_pos_gain_l0 = fw_inf.dnr_detail.reg_bnr_ac_pos_gain_l0;
				pp->reg_bnr_ac_pos_gain_l1 = fw_inf.dnr_detail.reg_bnr_ac_pos_gain_l1;
				pp->reg_bnr_ac_pos_gain_l2 = fw_inf.dnr_detail.reg_bnr_ac_pos_gain_l2;
				pp->reg_bnr_ac_pos_gain_l3 = fw_inf.dnr_detail.reg_bnr_ac_pos_gain_l3;
				pp->reg_bnr_ac_detail_max  = fw_inf.dnr_detail.reg_bnr_ac_detail_max;
				pp->reg_bnr_ac_detail_min  = fw_inf.dnr_detail.reg_bnr_ac_detail_min;
				pp->reg_bnr_diff_l         = fw_inf.dnr_detail.reg_bnr_diff_l;
				pp->reg_bnr_diff_p         = fw_inf.dnr_detail.reg_bnr_diff_p;
				/* dc bnr 18 */
				PE_P0D_M14B0_QRd03(p0d_dc_bnr_ctrl_2,	reg_dc_var_en,			pp->reg_dc_var_en,\
														reg_dc_motion_en,		pp->reg_dc_motion_en,\
														reg_dc_protection_en,	pp->reg_dc_protection_en);
				PE_P0D_M14B0_QRd02(p0d_dc_bnr_ctrl_0,	reg_dc_motion_max,		pp->reg_dc_motion_max,\
														reg_dc_motion_min,		pp->reg_dc_motion_min);
				PE_P0D_M14B0_QRd02(p0d_dc_bnr_ctrl_1,	reg_dc_detail_max,		pp->reg_dc_detail_max,\
														reg_dc_detail_min,		pp->reg_dc_detail_min);
				PE_P0D_M14B0_QRd01(p0d_dc_bnr_ctrl_2,	reg_dc_global_motion_th,pp->reg_dc_global_motion_th);
				PE_P0D_M14B0_QRd04(p0d_dc_bnr_ctrl_3,	reg_dc_bnr_var_th0,		pp->reg_dc_bnr_var_th0,\
														reg_dc_bnr_var_th1,		pp->reg_dc_bnr_var_th1,\
														reg_dc_bnr_var_th2,		pp->reg_dc_bnr_var_th2,\
														reg_dc_bnr_var_th3,		pp->reg_dc_bnr_var_th3);
				/* ac bnr 23 */
				PE_P0D_M14B0_QRd02(p0d_ac_bnr_ctrl_0,	reg_bnr_ac_diff_min_v_th,	pp->reg_bnr_ac_diff_min_v_th,\
														reg_bnr_ac_diff_min_h_th,	pp->reg_bnr_ac_diff_min_h_th);
				PE_P0D_M14B0_QRd03(p0d_ac_bnr_ctrl_1,	reg_bnr_ac_global_motion_th,pp->reg_bnr_ac_global_motion_th,\
														reg_bnr_ac_acness_max,		pp->reg_bnr_ac_acness_max,\
														reg_bnr_ac_acness_min,		pp->reg_bnr_ac_acness_min);
				PE_P0D_M14B0_QRd02(p0d_ac_bnr_ctrl_4,	reg_bnr_ac_motion_max,		pp->reg_bnr_ac_motion_max,\
														reg_bnr_ac_motion_min,		pp->reg_bnr_ac_motion_min);
				/* mnr 8 */
				PE_P0D_M14B0_QRd02(p0d_mnr_ctrl_0,	reg_h_expend,			pp->reg_h_expend,\
													reg_gain_flt_size,		pp->reg_gain_flt_size);
				PE_P0D_M14B0_QRd04(p0d_mnr_ctrl_1,	reg_mnr_s1_mmd_min,		pp->reg_mnr_s1_mmd_min,\
													reg_mnr_s2_ratio_min,	pp->reg_mnr_s2_ratio_min,\
													reg_mnr_s2_ratio_max,	pp->reg_mnr_s2_ratio_max,\
													reg_mnr_s2_mmd_min,		pp->reg_mnr_s2_mmd_min);
				PE_P0D_M14B0_QRd04(p0d_mnr_ctrl_3,	reg_filter_x0,			pp->reg_filter_x0,\
													reg_filter_x1,			pp->reg_filter_x1,\
													reg_filter_y0,			pp->reg_filter_y0,\
													reg_filter_y1,			pp->reg_filter_y1);
				PE_P0D_M14B0_QRd04(p0d_mnr_ctrl_4,	reg_motion_mnr_en,		pp->reg_motion_mnr_en,\
													reg_motion_mnr_filter,	pp->reg_motion_mnr_filter,\
													reg_mnr_motion_min,		pp->reg_mnr_motion_min,\
													reg_mnr_motion_max,		pp->reg_mnr_motion_max);
				PE_P0D_M14B0_QRd04(p0d_mnr_ctrl_5,	reg_motion_mnr_x0,		pp->reg_motion_mnr_x0,\
													reg_motion_mnr_x1,		pp->reg_motion_mnr_x1,\
													reg_motion_mnr_y0,		pp->reg_motion_mnr_y0,\
													reg_motion_mnr_y1,		pp->reg_motion_mnr_y1);
				#else	//PE_FWI_M14_ADD_FW_DNR_DATA
				/* dc bnr 18 */
				PE_P0D_M14B0_QRd04(p0d_dc_bnr_ctrl_2,	reg_dc_var_en,			pp->reg_dc_var_en,\
														reg_dc_motion_en,		pp->reg_dc_motion_en,\
														reg_dc_protection_en,	pp->reg_dc_protection_en,
														reg_dc_detail_en,		pp->reg_dc_detail_en);
				PE_P0D_M14B0_QRd03(p0d_dc_bnr_ctrl_0,	reg_dc_blur_sel,		pp->reg_dc_blur_sel,\
														reg_dc_motion_max,		pp->reg_dc_motion_max,\
														reg_dc_motion_min,		pp->reg_dc_motion_min);
				PE_P0D_M14B0_QRd05(p0d_dc_bnr_ctrl_1,	reg_dc_detail_max,		pp->reg_dc_detail_max,\
														reg_dc_detail_min,		pp->reg_dc_detail_min,\
														reg_var_v_gain,			pp->reg_var_v_gain,\
														reg_var_h_gain,			pp->reg_var_h_gain,\
														reg_var_cut_resolution,	pp->reg_var_cut_resolution);
				PE_P0D_M14B0_QRd02(p0d_dc_bnr_ctrl_2,	reg_dc_global_motion_th,pp->reg_dc_global_motion_th,\
														reg_dc_protection_th,	pp->reg_dc_protection_th);
				PE_P0D_M14B0_QRd04(p0d_dc_bnr_ctrl_3,	reg_dc_bnr_var_th0,		pp->reg_dc_bnr_var_th0,\
														reg_dc_bnr_var_th1,		pp->reg_dc_bnr_var_th1,\
														reg_dc_bnr_var_th2,		pp->reg_dc_bnr_var_th2,\
														reg_dc_bnr_var_th3,		pp->reg_dc_bnr_var_th3);
				/* ac bnr 23 */
				PE_P0D_M14B0_QRd02(p0d_ac_bnr_ctrl_0,	reg_bnr_ac_diff_min_v_th,	pp->reg_bnr_ac_diff_min_v_th,\
														reg_bnr_ac_diff_min_h_th,	pp->reg_bnr_ac_diff_min_h_th);
				PE_P0D_M14B0_QRd03(p0d_ac_bnr_ctrl_1,	reg_bnr_ac_global_motion_th,pp->reg_bnr_ac_global_motion_th,\
														reg_bnr_ac_acness_max,		pp->reg_bnr_ac_acness_max,\
														reg_bnr_ac_acness_min,		pp->reg_bnr_ac_acness_min);
				PE_P0D_M14B0_QRd02(p0d_ac_bnr_ctrl_4,	reg_bnr_ac_motion_max,		pp->reg_bnr_ac_motion_max,\
														reg_bnr_ac_motion_min,		pp->reg_bnr_ac_motion_min);
				PE_P0D_M14B0_QRd04(p0d_ac_bnr_ctrl_5,	reg_bnr_ac_detail_th1,		pp->reg_bnr_ac_detail_th1,\
														reg_bnr_ac_detail_th2,		pp->reg_bnr_ac_detail_th2,\
														reg_bnr_ac_detail_th3,		pp->reg_bnr_ac_detail_th3,\
														reg_bnr_ac_detail_th4,		pp->reg_bnr_ac_detail_th4);
				PE_P0D_M14B0_QRd03(p0d_ac_bnr_ctrl_6,	reg_bnr_ac_pos_gain_h0,		pp->reg_bnr_ac_pos_gain_h0,\
														reg_bnr_ac_pos_gain_h1,		pp->reg_bnr_ac_pos_gain_h1,\
														reg_bnr_ac_pos_gain_h2,		pp->reg_bnr_ac_pos_gain_h2);
				PE_P0D_M14B0_QRd04(p0d_ac_bnr_ctrl_7,	reg_bnr_ac_pos_gain_h3,		pp->reg_bnr_ac_pos_gain_h3,\
														reg_bnr_ac_pos_gain_l0,		pp->reg_bnr_ac_pos_gain_l0,\
														reg_bnr_ac_pos_gain_l1,		pp->reg_bnr_ac_pos_gain_l1,\
														reg_bnr_ac_pos_gain_l2,		pp->reg_bnr_ac_pos_gain_l2);
				PE_P0D_M14B0_QRd01(p0d_ac_bnr_ctrl_8,	reg_bnr_ac_pos_gain_l3,		pp->reg_bnr_ac_pos_gain_l3);
				PE_P0D_M14B0_QRd04(p0d_detail_ctrl,		reg_bnr_ac_detail_max,		pp->reg_bnr_ac_detail_max,\
														reg_bnr_ac_detail_min,		pp->reg_bnr_ac_detail_min,\
														reg_bnr_diff_l,				pp->reg_bnr_diff_l,\
														reg_bnr_diff_p,				pp->reg_bnr_diff_p);
				/* mnr 8 */
				PE_P0D_M14B0_QRd02(p0d_mnr_ctrl_0,	reg_h_expend,			pp->reg_h_expend,\
													reg_gain_flt_size,		pp->reg_gain_flt_size);
				PE_P0D_M14B0_QRd04(p0d_mnr_ctrl_1,	reg_mnr_s1_mmd_min,		pp->reg_mnr_s1_mmd_min,\
													reg_mnr_s2_ratio_min,	pp->reg_mnr_s2_ratio_min,\
													reg_mnr_s2_ratio_max,	pp->reg_mnr_s2_ratio_max,\
													reg_mnr_s2_mmd_min,		pp->reg_mnr_s2_mmd_min);
				PE_P0D_M14B0_QRd04(p0d_mnr_ctrl_3,	reg_filter_x0,			pp->reg_filter_x0,\
													reg_filter_x1,			pp->reg_filter_x1,\
													reg_filter_y0,			pp->reg_filter_y0,\
													reg_filter_y1,			pp->reg_filter_y1);
				PE_P0D_M14B0_QRd04(p0d_mnr_ctrl_4,	reg_motion_mnr_en,		pp->reg_motion_mnr_en,\
													reg_motion_mnr_filter,	pp->reg_motion_mnr_filter,\
													reg_mnr_motion_min,		pp->reg_mnr_motion_min,\
													reg_mnr_motion_max,		pp->reg_mnr_motion_max);
				PE_P0D_M14B0_QRd04(p0d_mnr_ctrl_5,	reg_motion_mnr_x0,		pp->reg_motion_mnr_x0,\
													reg_motion_mnr_x1,		pp->reg_motion_mnr_x1,\
													reg_motion_mnr_y0,		pp->reg_motion_mnr_y0,\
													reg_motion_mnr_y1,		pp->reg_motion_mnr_y1);
				#endif	//PE_FWI_M14_ADD_FW_DNR_DATA
			}
			PE_NRD_HW_M14_DBG_PRINT("get[%d] \n"\
				"dc_bnr: 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				"dc_bnr: 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				"ac_bnr: 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				"ac_bnr: 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				"ac_bnr: 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				"mnr   : 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				"mnr   : 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n", pp->win_id,\
				/* dc bnr 18 */
				pp->reg_dc_var_en, pp->reg_dc_motion_en, pp->reg_dc_protection_en, \
				pp->reg_dc_detail_en, pp->reg_dc_blur_sel, pp->reg_dc_motion_max, \
				pp->reg_dc_motion_min, pp->reg_dc_detail_max, pp->reg_dc_detail_min, \
				pp->reg_var_v_gain, pp->reg_var_h_gain, pp->reg_var_cut_resolution, \
				pp->reg_dc_global_motion_th, pp->reg_dc_protection_th, pp->reg_dc_bnr_var_th0, \
				pp->reg_dc_bnr_var_th1, pp->reg_dc_bnr_var_th2, pp->reg_dc_bnr_var_th3, \
				/* ac bnr 23 */
				pp->reg_bnr_ac_diff_min_v_th, pp->reg_bnr_ac_diff_min_h_th, \
				pp->reg_bnr_ac_global_motion_th, pp->reg_bnr_ac_acness_max, \
				pp->reg_bnr_ac_acness_min, pp->reg_bnr_ac_motion_max, pp->reg_bnr_ac_motion_min, \
				pp->reg_bnr_ac_detail_th1, pp->reg_bnr_ac_detail_th2, pp->reg_bnr_ac_detail_th3, \
				pp->reg_bnr_ac_detail_th4, pp->reg_bnr_ac_pos_gain_h0, pp->reg_bnr_ac_pos_gain_h1, \
				pp->reg_bnr_ac_pos_gain_h2, pp->reg_bnr_ac_pos_gain_h3, pp->reg_bnr_ac_pos_gain_l0, \
				pp->reg_bnr_ac_pos_gain_l1, pp->reg_bnr_ac_pos_gain_l2, pp->reg_bnr_ac_pos_gain_l3, \
				pp->reg_bnr_ac_detail_max, pp->reg_bnr_ac_detail_min, \
				pp->reg_bnr_diff_l, pp->reg_bnr_diff_p, \
				/* mnr 18 */
				pp->reg_h_expend, pp->reg_gain_flt_size, pp->reg_mnr_s1_mmd_min, \
				pp->reg_mnr_s2_ratio_min, pp->reg_mnr_s2_ratio_max, pp->reg_mnr_s2_mmd_min, \
				pp->reg_filter_x0, pp->reg_filter_x1, pp->reg_filter_y0, pp->reg_filter_y1, \
				pp->reg_motion_mnr_en, pp->reg_motion_mnr_filter, pp->reg_mnr_motion_min, \
				pp->reg_mnr_motion_max, pp->reg_motion_mnr_x0, pp->reg_motion_mnr_x1, \
				pp->reg_motion_mnr_y0, pp->reg_motion_mnr_y1);
		}
		else if(PE_KDRV_VER_M14AX)
		{
			LX_PE_NRD_DNR2_DETAIL_T *pp=(LX_PE_NRD_DNR2_DETAIL_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			win_id = PE_GET_CHECKED_WINID(pp->win_id);
			if(PE_CHECK_WIN0(win_id))
			{
				/* mnr */
				PE_P0L_M14A0_QRd02(p0l_mnr_ctrl_00,	reg_h_expend,			pp->reg_h_expend,\
													reg_gain_flt_size,		pp->reg_gain_flt_size);
				PE_P0L_M14A0_QRd04(p0l_mnr_ctrl_01,	reg_mnr_s1_mmd_min,		pp->reg_mnr_s1_mmd_min,\
													reg_mnr_s2_ratio_min,	pp->reg_mnr_s2_ratio_min,\
													reg_mnr_s2_ratio_max,	pp->reg_mnr_s2_ratio_max,\
													reg_mnr_s2_mmd_min,		pp->reg_mnr_s2_mmd_min);
				PE_P0L_M14A0_QRd04(p0l_mnr_ctrl_03,	reg_filter_x0,			pp->reg_filter_x0,\
													reg_filter_x1,			pp->reg_filter_x1,\
													reg_filter_y0,			pp->reg_filter_y0,\
													reg_filter_y1,			pp->reg_filter_y1);
				PE_P0L_M14A0_QRd04(p0l_mnr_ctrl_04,	reg_motion_mnr_en,		pp->reg_motion_mnr_en,\
													reg_motion_mnr_filter,	pp->reg_motion_mnr_filter,\
													reg_mnr_motion_min,		pp->reg_mnr_motion_min,\
													reg_mnr_motion_max,		pp->reg_mnr_motion_max);
				PE_P0L_M14A0_QRd04(p0l_mnr_ctrl_05,	reg_motion_mnr_x0,		pp->reg_motion_mnr_x0,\
													reg_motion_mnr_x1,		pp->reg_motion_mnr_x1,\
													reg_motion_mnr_y0,		pp->reg_motion_mnr_y0,\
													reg_motion_mnr_y1,		pp->reg_motion_mnr_y1);
				/* ac_bnr */
				PE_P0L_M14A0_QRd03(p0l_bnr_ac_ctrl_00,	reg_bnr_ac_protect_en,	pp->motion_protect_enable,\
														reg_bnr_ac_sdhd_sel,	pp->source_type,\
														reg_bnr_ac_flt_type,	pp->fiter_type);
				PE_P0L_M14A0_QRd03(p0l_bnr_ac_ctrl_01,	strength_h_x0,		pp->strength_h_x0,\
														strength_h_x1,		pp->strength_h_x1,\
														detect_min_th,		pp->detect_min_th);
				PE_P0L_M14A0_QRd02(p0l_bnr_ac_ctrl_02,	strength_v_x0,		pp->strength_v_x0,\
														strength_v_x1,		pp->strength_v_x1);
				/* dc_bnr */
				PE_P0L_M14A0_QRd04(p0l_bnr_dc_ctrl_00,	reg_dc_bnr_var_en,		pp->reg_dc_bnr_var_en,\
														reg_dc_bnr_motion_en,	pp->reg_dc_bnr_motion_en,\
														reg_dc_bnr_acadaptive,	pp->reg_dc_bnr_acadaptive,\
														reg_dc_bnr_sdhd_sel,	pp->reg_dc_bnr_sdhd_sel);
				PE_P0L_M14A0_QRd02(p0l_bnr_dc_ctrl_00,	reg_dc_blur_sel,		pp->reg_dc_blur_sel,\
														reg_dc_protection_en,	pp->reg_dc_protection_en);
				PE_P0L_M14A0_QRd04(p0l_bnr_dc_ctrl_01,	reg_dc_bnr_var_th0,		pp->reg_dc_bnr_var_th0,\
														reg_dc_bnr_var_th1,		pp->reg_dc_bnr_var_th1,\
														reg_dc_bnr_var_th2,		pp->reg_dc_bnr_var_th2,\
														reg_dc_bnr_var_th3,		pp->reg_dc_bnr_var_th3);
				PE_P0L_M14A0_QRd04(p0l_bnr_dc_ctrl_02,	reg_dc_bnr_motion_th0,	pp->reg_dc_bnr_motion_th0,\
														reg_dc_bnr_motion_th1,	pp->reg_dc_bnr_motion_th1,\
														reg_dc_bnr_motion_th2,	pp->reg_dc_bnr_motion_th2,\
														reg_dc_bnr_motion_th3,	pp->reg_dc_bnr_motion_th3);
				PE_P0L_M14A0_QRd03(p0l_bnr_dc_ctrl_03,	reg_dc_chroma_variance,	pp->reg_dc_chroma_variance,\
														reg_dc_var_h_gain,		pp->reg_dc_var_h_gain,\
														reg_dc_protection_th,	pp->reg_dc_protection_th);
			}
			if(PE_CHECK_WIN1(win_id))
			{
				/* mnr */
				PE_P0R_M14A0_QRd02(p0r_mnr_ctrl_00,	reg_h_expend,			pp->reg_h_expend,\
													reg_gain_flt_size,		pp->reg_gain_flt_size);

				PE_P0R_M14A0_QRd04(p0r_mnr_ctrl_01,	reg_mnr_s1_mmd_min,		pp->reg_mnr_s1_mmd_min,\
													reg_mnr_s2_ratio_min,	pp->reg_mnr_s2_ratio_min,\
													reg_mnr_s2_ratio_max,	pp->reg_mnr_s2_ratio_max,\
													reg_mnr_s2_mmd_min,		pp->reg_mnr_s2_mmd_min);
				PE_P0R_M14A0_QRd04(p0r_mnr_ctrl_03,	reg_filter_x0,			pp->reg_filter_x0,\
													reg_filter_x1,			pp->reg_filter_x1,\
													reg_filter_y0,			pp->reg_filter_y0,\
													reg_filter_y1,			pp->reg_filter_y1);
				PE_P0R_M14A0_QRd04(p0r_mnr_ctrl_04,	reg_motion_mnr_en,		pp->reg_motion_mnr_en,\
													reg_motion_mnr_filter,	pp->reg_motion_mnr_filter,\
													reg_mnr_motion_min,		pp->reg_mnr_motion_min,\
													reg_mnr_motion_max,		pp->reg_mnr_motion_max);
				PE_P0R_M14A0_QRd04(p0r_mnr_ctrl_05,	reg_motion_mnr_x0,		pp->reg_motion_mnr_x0,\
													reg_motion_mnr_x1,		pp->reg_motion_mnr_x1,\
													reg_motion_mnr_y0,		pp->reg_motion_mnr_y0,\
													reg_motion_mnr_y1,		pp->reg_motion_mnr_y1);
				/* ac_bnr */
				PE_P0R_M14A0_QRd03(p0r_bnr_ac_ctrl_00,	reg_bnr_ac_protect_en,	pp->motion_protect_enable,\
														reg_bnr_ac_sdhd_sel,	pp->source_type,\
														reg_bnr_ac_flt_type,	pp->fiter_type);
				PE_P0R_M14A0_QRd03(p0r_bnr_ac_ctrl_01,	strength_h_x0,		pp->strength_h_x0,\
														strength_h_x1,		pp->strength_h_x1,\
														detect_min_th,		pp->detect_min_th);
				PE_P0R_M14A0_QRd02(p0r_bnr_ac_ctrl_02,	strength_v_x0,		pp->strength_v_x0,\
														strength_v_x1,		pp->strength_v_x1);
				/* dc_bnr */
				PE_P0R_M14A0_QRd04(p0r_bnr_dc_ctrl_00,	reg_dc_bnr_var_en,		pp->reg_dc_bnr_var_en,\
														reg_dc_bnr_motion_en,	pp->reg_dc_bnr_motion_en,\
														reg_dc_bnr_acadaptive,	pp->reg_dc_bnr_acadaptive,\
														reg_dc_bnr_sdhd_sel,	pp->reg_dc_bnr_sdhd_sel);
				PE_P0R_M14A0_QRd02(p0r_bnr_dc_ctrl_00,	reg_dc_blur_sel,		pp->reg_dc_blur_sel,\
														reg_dc_protection_en,	pp->reg_dc_protection_en);
				PE_P0R_M14A0_QRd04(p0r_bnr_dc_ctrl_01,	reg_dc_bnr_var_th0,		pp->reg_dc_bnr_var_th0,\
														reg_dc_bnr_var_th1,		pp->reg_dc_bnr_var_th1,\
														reg_dc_bnr_var_th2,		pp->reg_dc_bnr_var_th2,\
														reg_dc_bnr_var_th3,		pp->reg_dc_bnr_var_th3);
				PE_P0R_M14A0_QRd04(p0r_bnr_dc_ctrl_02,	reg_dc_bnr_motion_th0,	pp->reg_dc_bnr_motion_th0,\
														reg_dc_bnr_motion_th1,	pp->reg_dc_bnr_motion_th1,\
														reg_dc_bnr_motion_th2,	pp->reg_dc_bnr_motion_th2,\
														reg_dc_bnr_motion_th3,	pp->reg_dc_bnr_motion_th3);
				PE_P0R_M14A0_QRd03(p0r_bnr_dc_ctrl_03,	reg_dc_chroma_variance,	pp->reg_dc_chroma_variance,\
														reg_dc_var_h_gain,		pp->reg_dc_var_h_gain,\
														reg_dc_protection_th,	pp->reg_dc_protection_th);
			}
			PE_NRD_HW_M14_DBG_PRINT("get[%d] \n"\
				"mnr   : h_expend:%d,  gain_flt_size:%d,  mmd_min s1,s2:%d,%d ratio_min,max: %d,%d \n"\
				"mnr   : filter x0,x1,y0,y1:%d,%d,%d,%d\n"\
				"mnr   : motion_en:%d, filter:%d, min:%d, max:%d\n"\
				"mnr   : motion x0,x1,y0,y1:%d,%d,%d,%d\n"\
				"ac_bnr: protect_en:%d, sdhd_sel(source type):%d, flt_thpe:%d,\n"\
				"ac_bnr: strength h_x0,h_x1,v_x0,v_x1:%d,%d,%d,%d, detect_min_th:%d\n"\
				"dc_bnr: var_en:%d,  motion_en:%d,  acadaptive:%d, sdhd_sel:%d\n"\
				"dc_bnr: blur_sel:%d, protection_en:%d \n"\
				"dc_bnr: var_th0,th1,th2,th3%d,%d,%d,%d, motion_th0,th1,th2,th3%d,%d,%d,%d\n"\
				"dc_bnr: chroma_var:%d, var_h_gain:%d, protection_th:%d\n",pp->win_id,\
				/* mnr */
				pp->reg_h_expend,pp->reg_gain_flt_size,\
				pp->reg_mnr_s1_mmd_min,pp->reg_mnr_s2_ratio_min,pp->reg_mnr_s2_ratio_max,pp->reg_mnr_s2_mmd_min,\
				pp->reg_filter_x0,pp->reg_filter_x1,pp->reg_filter_y0,pp->reg_filter_y1,\
				pp->reg_motion_mnr_en,pp->reg_motion_mnr_filter,pp->reg_mnr_motion_min,pp->reg_mnr_motion_max,\
				pp->reg_motion_mnr_x0,pp->reg_motion_mnr_x1,pp->reg_motion_mnr_y0,pp->reg_motion_mnr_y1,\
				/* ac_bnr */
				pp->motion_protect_enable,pp->source_type,pp->fiter_type,\
				pp->strength_h_x0,pp->strength_h_x1,pp->strength_v_x0,pp->strength_v_x1,\
				pp->detect_min_th,\
				/* dc_bnr */
				pp->reg_dc_bnr_var_en,pp->reg_dc_bnr_motion_en,pp->reg_dc_bnr_acadaptive,pp->reg_dc_bnr_sdhd_sel,\
				pp->reg_dc_blur_sel,pp->reg_dc_protection_en,\
				pp->reg_dc_bnr_var_th0,pp->reg_dc_bnr_var_th1,\
				pp->reg_dc_bnr_var_th2,pp->reg_dc_bnr_var_th3,\
				pp->reg_dc_bnr_motion_th0,pp->reg_dc_bnr_motion_th1,\
				pp->reg_dc_bnr_motion_th2,pp->reg_dc_bnr_motion_th3,\
				pp->reg_dc_chroma_variance,pp->reg_dc_var_h_gain,pp->reg_dc_protection_th);
		}
		else
		{
			PE_NRD_HW_M14_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * set tnr common ctrl
 * - use input struct LX_PE_NRD_TNR2_CMN_T for M14AX
 * - use input struct LX_PE_NRD_TNR1_CMN_T for M14AX
 *
 * @param   *pstParams [in] void
 * @return  OK if success, ERROR otherwise.
 * @see		set discription
 * @author
 */
int PE_NRD_HW_M14_SetTnrCmnCtrl(void *pstParams)
{
	int ret = RET_OK;
	#ifdef PE_HW_M14_USE_TNR_GAIN_ON_TASK
	PE_NRD_HW_M14_SETTINGS_T *pInfo = &_g_pe_nrd_hw_m14_info;
	#endif
	do{
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_M14BX)
		{
			LX_PE_NRD_TNR4_CMN_T *pp=(LX_PE_NRD_TNR4_CMN_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			PE_NRD_HW_M14_DBG_PRINT("set[%d] : tnr_master_gain:%d\n", \
				pp->win_id, pp->tnr_master_gain);
			if(PE_CHECK_WIN0(pp->win_id))
			{
				#ifdef PE_HW_M14B_USE_REG_CTRL
				#ifdef PE_HW_M14_USE_TNR_GAIN_ON_TASK
				pInfo->cur_tnr_auto_enable = GET_BITS(pp->tnr_master_gain,0,1);
				pInfo->cur_tnr_master_gain = GET_BITS(pp->tnr_master_gain,1,7)<<1;
				PE_NRD_HW_M14_DBG_PRINT("tnr_mode : auto_en,mst_gain:0x%x, 0x%x\n", \
					pInfo->cur_tnr_auto_enable, pInfo->cur_tnr_master_gain);
				#else
				PE_NRD_HW_M14B_P0M_WR01(p0m_tnr_ctrl_00,tnr_master_gain,(GET_BITS(pp->tnr_master_gain,1,7)<<1));
				/*PE_NRD_HW_M14B_P0M_WR02(p0m_tnr_ctrl_20,alpha_gain_y,GET_BITS(pp->tnr_master_gain,2,6),\
													alpha_gain_c,GET_BITS(pp->tnr_master_gain,2,6));*/
				#endif
				#else
				PE_P0M_M14B0_QWr01(p0m_tnr_ctrl_00,tnr_master_gain,GET_BITS(pp->tnr_master_gain,0,8));
				/*PE_P0M_M14B0_QWr02(p0m_tnr_ctrl_20,	alpha_gain_y,GET_BITS(pp->tnr_master_gain,2,6),\
													alpha_gain_c,GET_BITS(pp->tnr_master_gain,2,6));*/
				#endif
			}
		}
		else if(PE_KDRV_VER_M14AX)
		{
			LX_PE_NRD_TNR2_CMN_T *pp=(LX_PE_NRD_TNR2_CMN_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			PE_NRD_HW_M14_DBG_PRINT("set[%d] : auto_en:%d, y,c_g:%d,%d, adj_nt_en,val:%d,%d\n",pp->win_id,\
				pp->auto_mode_en,pp->y_gain,pp->c_gain,pp->adjust_nt_lvl,pp->adjust_nt_lvl_val);
			if(PE_CHECK_WIN0(pp->win_id))
			{
				PE_P0L_M14A0_QWr02(p0l_tnr_ctrl_13,	adjust_nt_lvl,		GET_BITS(pp->adjust_nt_lvl,0,1),\
													adjust_nt_lvl_val,	GET_BITS(pp->adjust_nt_lvl_val,0,8));
				PE_P0L_M14A0_QWr02(p0l_tnr_ctrl_18,	y_gain,				GET_BITS(pp->y_gain,0,8),\
													c_gain,				GET_BITS(pp->c_gain,0,8));
				PE_P1L_M14A0_QWr01(pe1_dse_ctrl_01,	reg_tnr_auto_en,	(pp->auto_mode_en)? 0x1:0x0);
			}
			if(PE_CHECK_WIN1(pp->win_id))
			{
				PE_P0R_M14A0_QWr02(p0r_tnr_ctrl_13,	adjust_nt_lvl,		GET_BITS(pp->adjust_nt_lvl,0,1),\
													adjust_nt_lvl_val,	GET_BITS(pp->adjust_nt_lvl_val,0,8));
				PE_P0R_M14A0_QWr02(p0r_tnr_ctrl_18,	y_gain,				GET_BITS(pp->y_gain,0,8),\
													c_gain,				GET_BITS(pp->c_gain,0,8));
			}
		}
		else
		{
			PE_NRD_HW_M14_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * get tnr common ctrl
 * - use input struct LX_PE_NRD_TNR2_CMN_T for M14AX
 * - use input struct LX_PE_NRD_TNR1_CMN_T for M14AX
 *
 * @param   *pstParams [in/out] void
 * @return  OK if success, ERROR otherwise.
 * @see		set discription
 * @author
 */
int PE_NRD_HW_M14_GetTnrCmnCtrl(void *pstParams)
{
	int ret = RET_OK;
	LX_PE_WIN_ID win_id;
	do{
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_M14BX)
		{
			LX_PE_NRD_TNR4_CMN_T *pp=(LX_PE_NRD_TNR4_CMN_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			win_id = PE_GET_CHECKED_WINID(pp->win_id);
			if(PE_CHECK_WIN0(win_id))
			{
				PE_P0M_M14B0_QRd01(p0m_tnr_ctrl_00,tnr_master_gain,pp->tnr_master_gain);
			}
			PE_NRD_HW_M14_DBG_PRINT("get[%d] : tnr_master_gain:%d\n", \
				pp->win_id, pp->tnr_master_gain);
		}
		else if(PE_KDRV_VER_M14AX)
		{
			LX_PE_NRD_TNR2_CMN_T *pp=(LX_PE_NRD_TNR2_CMN_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			win_id = PE_GET_CHECKED_WINID(pp->win_id);
			if(PE_CHECK_WIN0(win_id))
			{
				PE_P0L_M14A0_QRd02(p0l_tnr_ctrl_13,	adjust_nt_lvl,		pp->adjust_nt_lvl,\
													adjust_nt_lvl_val,	pp->adjust_nt_lvl_val);
				PE_P0L_M14A0_QRd02(p0l_tnr_ctrl_18,	y_gain,				pp->y_gain,\
													c_gain,				pp->c_gain);
				PE_P1L_M14A0_QRd01(pe1_dse_ctrl_01,	reg_tnr_auto_en,	pp->auto_mode_en);
			}
			if(PE_CHECK_WIN1(win_id))
			{
				PE_P0R_M14A0_QRd02(p0r_tnr_ctrl_13,	adjust_nt_lvl,		pp->adjust_nt_lvl,\
													adjust_nt_lvl_val,	pp->adjust_nt_lvl_val);
				PE_P0R_M14A0_QRd02(p0r_tnr_ctrl_18,	y_gain,				pp->y_gain,\
													c_gain,				pp->c_gain);
				pp->auto_mode_en = 0xff;
			}
			PE_NRD_HW_M14_DBG_PRINT("get[%d] : auto_en:%d, y,c_g:%d,%d, adj_nt_en,val:%d,%d\n",pp->win_id,\
				pp->auto_mode_en,pp->y_gain,pp->c_gain,pp->adjust_nt_lvl,pp->adjust_nt_lvl_val);
		}
		else
		{
			PE_NRD_HW_M14_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * set tnr detail ctrl
 * - use input struct LX_PE_NRD_TNR2_DETAIL_T for M14AX
 *
 * @param   *pstParams [in] void
 * @return  OK if success, ERROR otherwise.
 * @see		set discription
 * @author
 */
int PE_NRD_HW_M14_SetTnrDetailCtrl(void *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_M14BX)
		{
			LX_PE_NRD_TNR4_DETAIL_T *pp=(LX_PE_NRD_TNR4_DETAIL_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			PE_NRD_HW_M14_DBG_PRINT("set[%d] \n"\
				"ma     : 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				"ma     : 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				"ma     : 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				"ma     : 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				"me     : 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				"me     : 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				"me     : 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				"me     : 0x%02x,0x%02x,0x%02x,0x%02x\n"\
				"mc     : 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				"ma post: 0x%02x,0x%02x,0x%02x,0x%02x\n"\
				"mc post: 0x%02x,0x%02x,0x%02x,0x%02x\n"\
				"snr    : 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n", pp->win_id, \
				/*ma 26*/
				pp->reg_g_blend_a, pp->motion_sc_reset_en, pp->luma_gain_ctrl_x0, \
				pp->luma_gain_ctrl_x1, pp->luma_gain_ctrl_x2, pp->luma_gain_ctrl_x3, \
				pp->luma_gain_ctrl_y0, pp->luma_gain_ctrl_y1, pp->luma_gain_ctrl_y2, \
				pp->luma_gain_ctrl_y3, pp->skingain_motioncontrol_x0, \
				pp->skingain_motioncontrol_x1, pp->skingain_motioncontrol_y0, \
				pp->skingain_motioncontrol_y1, pp->skin_lut_y0, \
				pp->skin_lut_y1, pp->skin_lut_y2, pp->skin_lut_y3, pp->skin_lut_cb0, \
				pp->skin_lut_cb1, pp->skin_lut_cb2, pp->skin_lut_cb3, pp->skin_lut_cr0, \
				pp->skin_lut_cr1, pp->skin_lut_cr2, pp->skin_lut_cr3, \
				/*me 22*/
				pp->past_refine_ratio, pp->global_past_refine_ratio, pp->mv_refine_ratio, \
				pp->temporal_refine_adj1, pp->temporal_refine_adj2, pp->zero_refine_ratio, \
				pp->me_conf_scale_ma, pp->me_conf_scale_mc, pp->past_x0, pp->past_x1, \
				pp->past_x2, pp->past_y0, pp->past_y1, pp->past_y2, pp->past_y3, \
				pp->global_x0, pp->global_x1, pp->global_x2, pp->global_y0, \
				pp->global_y1, pp->global_y2, pp->global_y3, \
				/*mc 5*/
				pp->am_th_mode, pp->am_th_val, pp->mcblnd_th_x0, \
				pp->mcblnd_th_x1, pp->sc_alpha_mode, \
				/*ma post 4*/
				pp->reg_ma_conf_x0, pp->reg_ma_conf_x1, pp->reg_ma_conf_y0, pp->reg_ma_conf_y1, \
				/*mc post 4*/
				pp->reg_mc_conf_x0, pp->reg_mc_conf_x1, pp->reg_mc_conf_y0, pp->reg_mc_conf_y1, \
				/*snr 7*/
				pp->reg_snr_master_gain, pp->reg_snr_c_blur_gain, pp->reg_snr_y_blur_gain, \
				pp->snr_blendgain_x0, pp->snr_blendgain_x1, pp->snr_blendgain_y0, \
				pp->snr_blendgain_y1);
			if(PE_CHECK_WIN0(pp->win_id))
			{
				#ifdef PE_HW_M14B_USE_REG_CTRL
				/*ma 26*/
				PE_NRD_HW_M14B_P0M_WR01(p0m_tnr_ctrl_25,reg_g_blend_a,			GET_BITS(pp->reg_g_blend_a,0,8));
				PE_NRD_HW_M14B_P0M_WR01(p0m_tnr_ctrl_31,motion_sc_reset_en,		GET_BITS(pp->motion_sc_reset_en,0,1));
				PE_NRD_HW_M14B_P0M_WR04(p0m_tnr_ctrl_33,luma_gain_ctrl_x0,		GET_BITS(pp->luma_gain_ctrl_x0,0,8), \
													luma_gain_ctrl_x1,		GET_BITS(pp->luma_gain_ctrl_x1,0,8), \
													luma_gain_ctrl_x2,		GET_BITS(pp->luma_gain_ctrl_x2,0,8), \
													luma_gain_ctrl_x3,		GET_BITS(pp->luma_gain_ctrl_x3,0,8));
				PE_NRD_HW_M14B_P0M_WR04(p0m_tnr_ctrl_34,luma_gain_ctrl_y0,		GET_BITS(pp->luma_gain_ctrl_y0,0,8), \
													luma_gain_ctrl_y1,		GET_BITS(pp->luma_gain_ctrl_y1,0,8), \
													luma_gain_ctrl_y2,		GET_BITS(pp->luma_gain_ctrl_y2,0,8), \
													luma_gain_ctrl_y3,		GET_BITS(pp->luma_gain_ctrl_y3,0,8));
				PE_NRD_HW_M14B_P0M_WR04(p0m_tnr_ctrl_32,skingain_motioncontrol_x0,GET_BITS(pp->skingain_motioncontrol_x0,0,8), \
													skingain_motioncontrol_x1,GET_BITS(pp->skingain_motioncontrol_x1,0,8), \
													skingain_motioncontrol_y0,GET_BITS(pp->skingain_motioncontrol_y0,0,5), \
													skingain_motioncontrol_y1,GET_BITS(pp->skingain_motioncontrol_y1,0,5));
				PE_NRD_HW_M14B_P0M_WR04(p0m_tnr_ctrl_35,skin_lut_y0,			GET_BITS(pp->skin_lut_y0,0,8), \
													skin_lut_y1,			GET_BITS(pp->skin_lut_y1,0,8), \
													skin_lut_y2,			GET_BITS(pp->skin_lut_y2,0,8), \
													skin_lut_y3,			GET_BITS(pp->skin_lut_y3,0,8));
				PE_NRD_HW_M14B_P0M_WR04(p0m_tnr_ctrl_36,skin_lut_cb0,			GET_BITS(pp->skin_lut_cb0,0,8), \
													skin_lut_cb1,			GET_BITS(pp->skin_lut_cb1,0,8), \
													skin_lut_cb2,			GET_BITS(pp->skin_lut_cb2,0,8), \
													skin_lut_cb3,			GET_BITS(pp->skin_lut_cb3,0,8));
				PE_NRD_HW_M14B_P0M_WR04(p0m_tnr_ctrl_37,skin_lut_cr0,			GET_BITS(pp->skin_lut_cr0,0,8), \
													skin_lut_cr1,			GET_BITS(pp->skin_lut_cr1,0,8), \
													skin_lut_cr2,			GET_BITS(pp->skin_lut_cr2,0,8), \
													skin_lut_cr3,			GET_BITS(pp->skin_lut_cr3,0,8));
				/*me 22*/
				PE_NRD_HW_M14B_P0M_WR02(p0m_tnr_ctrl_07,past_refine_ratio,		GET_BITS(pp->past_refine_ratio,0,4), \
													global_past_refine_ratio,GET_BITS(pp->global_past_refine_ratio,0,4));
				PE_NRD_HW_M14B_P0M_WR01(p0m_tnr_ctrl_08,mv_refine_ratio,		GET_BITS(pp->mv_refine_ratio,0,4));
				PE_NRD_HW_M14B_P0M_WR03(p0m_tnr_ctrl_09,temporal_refine_adj1,	GET_BITS(pp->temporal_refine_adj1,0,3), \
													temporal_refine_adj2,	GET_BITS(pp->temporal_refine_adj2,0,3), \
													zero_refine_ratio,		GET_BITS(pp->zero_refine_ratio,0,4));
				PE_NRD_HW_M14B_P0M_WR02(p0m_tnr_ctrl_12,me_conf_scale_ma,		GET_BITS(pp->me_conf_scale_ma,0,3), \
													me_conf_scale_mc,		GET_BITS(pp->me_conf_scale_mc,0,3));
				PE_NRD_HW_M14B_P0M_WR05(p0m_tnr_ctrl_39,past_x0,				GET_BITS(pp->past_x0,0,8), \
													past_x1,				GET_BITS(pp->past_x1,0,8), \
													past_x2,				GET_BITS(pp->past_x2,0,8), \
													past_y0,				GET_BITS(pp->past_y0,0,8), \
													past_y1,				GET_BITS(pp->past_y1,0,8));
				PE_NRD_HW_M14B_P0M_WR02(p0m_tnr_ctrl_38,past_y2,				GET_BITS(pp->past_y2,0,4), \
													past_y3,				GET_BITS(pp->past_y3,0,4));
				PE_NRD_HW_M14B_P0M_WR05(p0m_tnr_ctrl_04, global_x0,				GET_BITS(pp->global_x0,0,8), \
													global_x1,				GET_BITS(pp->global_x1,0,8), \
													global_x2,				GET_BITS(pp->global_x2,0,8), \
													global_y0,				GET_BITS(pp->global_y0,0,4), \
													global_y1,				GET_BITS(pp->global_y1,0,4));
				PE_NRD_HW_M14B_P0M_WR02(p0m_tnr_ctrl_05,global_y2,				GET_BITS(pp->global_y2,0,4), \
													global_y3,				GET_BITS(pp->global_y3,0,4));
				/*mc 5*/
				PE_NRD_HW_M14B_P0M_WR02(p0m_tnr_ctrl_16,am_th_mode,			GET_BITS(pp->am_th_mode,0,1), \
													am_th_val,			GET_BITS(pp->am_th_val,0,8));
				PE_NRD_HW_M14B_P0M_WR02(p0m_tnr_ctrl_19,mcblnd_th_x0,		GET_BITS(pp->mcblnd_th_x0,0,8), \
													mcblnd_th_x1,		GET_BITS(pp->mcblnd_th_x1,0,8));
				PE_NRD_HW_M14B_P0M_WR01(p0m_tnr_ctrl_20,sc_alpha_mode,		GET_BITS(pp->sc_alpha_mode,0,2));
				/*ma post 4*/
				PE_NRD_HW_M14B_P0M_WR04(p0m_tnr_ctrl_43,reg_ma_conf_x0,		GET_BITS(pp->reg_ma_conf_x0,0,5), \
													reg_ma_conf_x1,		GET_BITS(pp->reg_ma_conf_x1,0,5), \
													reg_ma_conf_y0,		GET_BITS(pp->reg_ma_conf_y0,0,8), \
													reg_ma_conf_y1,		GET_BITS(pp->reg_ma_conf_y1,0,8));
				/*mc post 4*/
				PE_NRD_HW_M14B_P0M_WR04(p0m_tnr_ctrl_42,reg_mc_conf_x0,		GET_BITS(pp->reg_mc_conf_x0,0,8), \
													reg_mc_conf_x1,		GET_BITS(pp->reg_mc_conf_x1,0,8), \
													reg_mc_conf_y0,		GET_BITS(pp->reg_mc_conf_y0,0,5), \
													reg_mc_conf_y1,		GET_BITS(pp->reg_mc_conf_y1,0,5));
				/*snr 7*/
				PE_NRD_HW_M14B_P0M_WR03(p0m_tnr_ctrl_48,reg_snr_master_gain,	GET_BITS(pp->reg_snr_master_gain,0,8), \
													reg_snr_c_blur_gain,	GET_BITS(pp->reg_snr_c_blur_gain,0,8), \
													reg_snr_y_blur_gain,	GET_BITS(pp->reg_snr_y_blur_gain,0,8));
				PE_NRD_HW_M14B_P0M_WR04(p0m_tnr_ctrl_50,snr_blendgain_x0,		GET_BITS(pp->snr_blendgain_x0,0,8), \
													snr_blendgain_x1,		GET_BITS(pp->snr_blendgain_x1,0,8), \
													snr_blendgain_y0,		GET_BITS(pp->snr_blendgain_y0,0,8), \
													snr_blendgain_y1,		GET_BITS(pp->snr_blendgain_y1,0,8));
				#else
				/*ma 26*/
				PE_P0M_M14B0_QWr01(p0m_tnr_ctrl_25, reg_g_blend_a,			GET_BITS(pp->reg_g_blend_a,0,8));
				PE_P0M_M14B0_QWr01(p0m_tnr_ctrl_31, motion_sc_reset_en,		GET_BITS(pp->motion_sc_reset_en,0,1));
				PE_P0M_M14B0_QWr04(p0m_tnr_ctrl_33, luma_gain_ctrl_x0,		GET_BITS(pp->luma_gain_ctrl_x0,0,8), \
													luma_gain_ctrl_x1,		GET_BITS(pp->luma_gain_ctrl_x1,0,8), \
													luma_gain_ctrl_x2,		GET_BITS(pp->luma_gain_ctrl_x2,0,8), \
													luma_gain_ctrl_x3,		GET_BITS(pp->luma_gain_ctrl_x3,0,8));
				PE_P0M_M14B0_QWr04(p0m_tnr_ctrl_34, luma_gain_ctrl_y0,		GET_BITS(pp->luma_gain_ctrl_y0,0,8), \
													luma_gain_ctrl_y1,		GET_BITS(pp->luma_gain_ctrl_y1,0,8), \
													luma_gain_ctrl_y2,		GET_BITS(pp->luma_gain_ctrl_y2,0,8), \
													luma_gain_ctrl_y3,		GET_BITS(pp->luma_gain_ctrl_y3,0,8));
				PE_P0M_M14B0_QWr04(p0m_tnr_ctrl_32, skingain_motioncontrol_x0,GET_BITS(pp->skingain_motioncontrol_x0,0,8), \
													skingain_motioncontrol_x1,GET_BITS(pp->skingain_motioncontrol_x1,0,8), \
													skingain_motioncontrol_y0,GET_BITS(pp->skingain_motioncontrol_y0,0,5), \
													skingain_motioncontrol_y1,GET_BITS(pp->skingain_motioncontrol_y1,0,5));
				PE_P0M_M14B0_QWr04(p0m_tnr_ctrl_35, skin_lut_y0,			GET_BITS(pp->skin_lut_y0,0,8), \
													skin_lut_y1,			GET_BITS(pp->skin_lut_y1,0,8), \
													skin_lut_y2,			GET_BITS(pp->skin_lut_y2,0,8), \
													skin_lut_y3,			GET_BITS(pp->skin_lut_y3,0,8));
				PE_P0M_M14B0_QWr04(p0m_tnr_ctrl_36, skin_lut_cb0,			GET_BITS(pp->skin_lut_cb0,0,8), \
													skin_lut_cb1,			GET_BITS(pp->skin_lut_cb1,0,8), \
													skin_lut_cb2,			GET_BITS(pp->skin_lut_cb2,0,8), \
													skin_lut_cb3,			GET_BITS(pp->skin_lut_cb3,0,8));
				PE_P0M_M14B0_QWr04(p0m_tnr_ctrl_37, skin_lut_cr0,			GET_BITS(pp->skin_lut_cr0,0,8), \
													skin_lut_cr1,			GET_BITS(pp->skin_lut_cr1,0,8), \
													skin_lut_cr2,			GET_BITS(pp->skin_lut_cr2,0,8), \
													skin_lut_cr3,			GET_BITS(pp->skin_lut_cr3,0,8));
				/*me 22*/
				PE_P0M_M14B0_QWr02(p0m_tnr_ctrl_07, past_refine_ratio,		GET_BITS(pp->past_refine_ratio,0,4), \
													global_past_refine_ratio,GET_BITS(pp->global_past_refine_ratio,0,4));
				PE_P0M_M14B0_QWr01(p0m_tnr_ctrl_08, mv_refine_ratio,		GET_BITS(pp->mv_refine_ratio,0,4));
				PE_P0M_M14B0_QWr03(p0m_tnr_ctrl_09, temporal_refine_adj1,	GET_BITS(pp->temporal_refine_adj1,0,3), \
													temporal_refine_adj2,	GET_BITS(pp->temporal_refine_adj2,0,3), \
													zero_refine_ratio,		GET_BITS(pp->zero_refine_ratio,0,4));
				PE_P0M_M14B0_QWr02(p0m_tnr_ctrl_12, me_conf_scale_ma,		GET_BITS(pp->me_conf_scale_ma,0,3), \
													me_conf_scale_mc,		GET_BITS(pp->me_conf_scale_mc,0,3));
				PE_P0M_M14B0_QWr05(p0m_tnr_ctrl_39, past_x0,				GET_BITS(pp->past_x0,0,8), \
													past_x1,				GET_BITS(pp->past_x1,0,8), \
													past_x2,				GET_BITS(pp->past_x2,0,8), \
													past_y0,				GET_BITS(pp->past_y0,0,8), \
													past_y1,				GET_BITS(pp->past_y1,0,8));
				PE_P0M_M14B0_QWr02(p0m_tnr_ctrl_38, past_y2,				GET_BITS(pp->past_y2,0,4), \
													past_y3,				GET_BITS(pp->past_y3,0,4));
				PE_P0M_M14B0_QWr05(p0m_tnr_ctrl_04, global_x0,				GET_BITS(pp->global_x0,0,8), \
													global_x1,				GET_BITS(pp->global_x1,0,8), \
													global_x2,				GET_BITS(pp->global_x2,0,8), \
													global_y0,				GET_BITS(pp->global_y0,0,4), \
													global_y1,				GET_BITS(pp->global_y1,0,4));
				PE_P0M_M14B0_QWr02(p0m_tnr_ctrl_05, global_y2,				GET_BITS(pp->global_y2,0,4), \
													global_y3,				GET_BITS(pp->global_y3,0,4));
				/*mc 5*/
				PE_P0M_M14B0_QWr02(p0m_tnr_ctrl_16, am_th_mode,			GET_BITS(pp->am_th_mode,0,1), \
													am_th_val,			GET_BITS(pp->am_th_val,0,8));
				PE_P0M_M14B0_QWr02(p0m_tnr_ctrl_19, mcblnd_th_x0,		GET_BITS(pp->mcblnd_th_x0,0,8), \
													mcblnd_th_x1,		GET_BITS(pp->mcblnd_th_x1,0,8));
				PE_P0M_M14B0_QWr01(p0m_tnr_ctrl_20, sc_alpha_mode,		GET_BITS(pp->sc_alpha_mode,0,2));
				/*ma post 4*/
				PE_P0M_M14B0_QWr04(p0m_tnr_ctrl_43, reg_ma_conf_x0,		GET_BITS(pp->reg_ma_conf_x0,0,5), \
													reg_ma_conf_x1,		GET_BITS(pp->reg_ma_conf_x1,0,5), \
													reg_ma_conf_y0,		GET_BITS(pp->reg_ma_conf_y0,0,8), \
													reg_ma_conf_y1,		GET_BITS(pp->reg_ma_conf_y1,0,8));
				/*mc post 4*/
				PE_P0M_M14B0_QWr04(p0m_tnr_ctrl_42, reg_mc_conf_x0,		GET_BITS(pp->reg_mc_conf_x0,0,8), \
													reg_mc_conf_x1,		GET_BITS(pp->reg_mc_conf_x1,0,8), \
													reg_mc_conf_y0,		GET_BITS(pp->reg_mc_conf_y0,0,5), \
													reg_mc_conf_y1,		GET_BITS(pp->reg_mc_conf_y1,0,5));
				/*snr 7*/
				PE_P0M_M14B0_QWr03(p0m_tnr_ctrl_48, reg_snr_master_gain,	GET_BITS(pp->reg_snr_master_gain,0,8), \
													reg_snr_c_blur_gain,	GET_BITS(pp->reg_snr_c_blur_gain,0,8), \
													reg_snr_y_blur_gain,	GET_BITS(pp->reg_snr_y_blur_gain,0,8));
				PE_P0M_M14B0_QWr04(p0m_tnr_ctrl_50, snr_blendgain_x0,		GET_BITS(pp->snr_blendgain_x0,0,8), \
													snr_blendgain_x1,		GET_BITS(pp->snr_blendgain_x1,0,8), \
													snr_blendgain_y0,		GET_BITS(pp->snr_blendgain_y0,0,8), \
													snr_blendgain_y1,		GET_BITS(pp->snr_blendgain_y1,0,8));
				#endif
			}
		}
		else if(PE_KDRV_VER_M14AX)
		{
			LX_PE_NRD_TNR2_DETAIL_T *pp=(LX_PE_NRD_TNR2_DETAIL_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			PE_NRD_HW_M14_DBG_PRINT("set[%d] \n"\
				"ma:\nmotion: 	 coring_en (y,c): %d,%d, gain (y,c): %d,%d,\n"\
					 "         	 coring_th: %d, offset_en: %d, gain_en_var (m,a): %d,%d\n"\
					 "         	 wa_y(0,1,2,3):%d,%d,%d,%d, wa_x(0,1,2,3):%d,%d,%d,%d\n"\
					 "alph_gain: gain_x(p0,p1,p2,p3):%d,%d,%d,%d, gain_y(p0,p1,p2,p3):%d,%d,%d,%d\n"\
					 "alph_remap:ave_en:%d, mapping_y_en:%d, mapping_c_en:%d\n"\
					 "           map_y(x0,x1,x2,x3):%d,%d,%d,%d, map_y(y0,y1,y2,y3):%d,%d,%d,%d\n"\
					 "           map_c(x0,x1,x2,x3):%d,%d,%d,%d, map_c(y0,y1,y2,y3):%d,%d,%d,%d\n"\
				     "adj_gain:  gm_en:%d, lpct:%d, avg_ts_en:%d, lpct_sel:%d\n"\
				     "           gm(x0,x1,x2,x3):%d,%d,%d,%d, gm(y0,y1,y2,y3):%d,%d,%d,%d\n"\
				     "           st(x0,x1,x2,x3):%d,%d,%d,%d, st(y0,y1,y2,y3):%d,%d,%d,%d\n"\
				     "           lpct(x0,x1,x2,x3):%d,%d,%d,%d, lpct(y0,y1,y2,y3):%d,%d,%d,%d\n"\
				     "           m_gain_en_var2:%d,\n"\
				     "           ctrl(x0,x1,y0,y1):%d,%d,%d,%d\n"\
				     "           sf_map_flt_en:%d, map_tap:%d, map_gain:%d\n"\
				     "           sf_th(0,1,2,3,4):%d,%d,%d,%d,%d\n"\
				     "           sf_mb(x0,x1,x2,x3):%d,%d,%d,%d, sf_mb(y0,y1,y2,y3):%d,%d,%d,%d\n"\
				     "           sf_gain_md:%d, sf_gain:%d, sf_en(y,c):%d,%d, sf_mo_exp_md:%d\n"\
				"mc:\n           sel_pels:%d, mamc_blend:%d, blend_sel:%d\n"\
				     "           blend(x0,x1,y0,y1):%d,%d,%d,%d\n"\
				"me:\n           hlf_pel_en:%d, smth(gain,en):%d,%d, gmv_smth(gain,en):%d,%d\n"\
				     "           mv0_th:%d, mv0_en:%d, sad_en:%d, sad_gm_en:%d\n"\
				     "           ctrl(x0,x1,y0,y1):%d,%d,%d,%d\n",\
				pp->win_id,\
				// ma: motion
				pp->m_coring_en_y,pp->m_coring_en_c,pp->m_gain_y,pp->m_gain_c,\
				pp->m_coring_th,pp->m_offset_mode_en,pp->m_gain_en_var,pp->a_gain_en_var,\
				pp->wa_y0,pp->wa_y1,pp->wa_y2,pp->wa_y3,pp->wa_x0,pp->wa_x1,pp->wa_x2,pp->wa_x3,\
				// ma: alpha gain
				pp->luma_gain_p0_x,pp->luma_gain_p1_x,pp->luma_gain_p2_x,pp->luma_gain_p3_x,\
				pp->luma_gain_p0_y,pp->luma_gain_p1_y,pp->luma_gain_p2_y,pp->luma_gain_p3_y,\
				// alpha remapping
				pp->alpha_avg_en,pp->alpha_mapping_y_en,pp->alpha_mapping_c_en,\
				pp->alpha_mapping_y_x0,pp->alpha_mapping_y_x1,pp->alpha_mapping_y_x2,pp->alpha_mapping_y_x3,\
				pp->alpha_mapping_y_y0,pp->alpha_mapping_y_y1,pp->alpha_mapping_y_y2,pp->alpha_mapping_y_y3,\
				pp->alpha_mapping_c_x0,pp->alpha_mapping_c_x1,pp->alpha_mapping_c_x2,pp->alpha_mapping_c_x3,\
				pp->alpha_mapping_c_y0,pp->alpha_mapping_c_y1,pp->alpha_mapping_c_y2,pp->alpha_mapping_c_y3,\
				// adjust gain
				pp->nt_lvl_adjust_gm_enable,pp->nt_lvl_adjust_lpct_enable,\
				pp->nt_lvl_adjust_avg_ts_enable,pp->nt_lvl_adjust_lpct_sel,\
				pp->nt_lvl_gain_gm_x0,pp->nt_lvl_gain_gm_x1,pp->nt_lvl_gain_gm_x2,pp->nt_lvl_gain_gm_x3,\
				pp->nt_lvl_gain_gm_y0,pp->nt_lvl_gain_gm_y1,pp->nt_lvl_gain_gm_y2,pp->nt_lvl_gain_gm_y3,\
				pp->nt_lvl_gain_st_x0,pp->nt_lvl_gain_st_x1,pp->nt_lvl_gain_st_x2,pp->nt_lvl_gain_st_x3,\
				pp->nt_lvl_gain_st_y0,pp->nt_lvl_gain_st_y1,pp->nt_lvl_gain_st_y2,pp->nt_lvl_gain_st_y3,\
				pp->nt_lvl_gain_lpct_x0,pp->nt_lvl_gain_lpct_x1,pp->nt_lvl_gain_lpct_x2,pp->nt_lvl_gain_lpct_x3,\
				pp->nt_lvl_gain_lpct_y0,pp->nt_lvl_gain_lpct_y1,pp->nt_lvl_gain_lpct_y2,pp->nt_lvl_gain_lpct_y3,\
				pp->m_gain_en_var2,\
				pp->m_gain_ctrl2_x0,pp->m_gain_ctrl2_x1,pp->m_gain_ctrl2_y0,pp->m_gain_ctrl2_y1,\
				pp->sf_map_flt_en,pp->sf_map_tap,pp->sf_map_gain,\
				pp->sf_th0,pp->sf_th1,pp->sf_th2,pp->sf_th3,pp->sf_th4,\
				pp->sf_mb_x0,pp->sf_mb_x1,pp->sf_mb_x2,pp->sf_mb_x3,\
				pp->sf_mb_y0,pp->sf_mb_y1,pp->sf_mb_y2,pp->sf_mb_y3,\
				pp->sf_gain_mode,pp->sf_gain,pp->sf_blend_en_y,pp->sf_blend_en_c,pp->sf_blend_motion_exp_mode,\
				// mc:
				pp->sad_sel_pels,pp->sad_mamc_blend,pp->mamc_blend_sel,\
				pp->mamc_blend_x0,pp->mamc_blend_x1,pp->mamc_blend_y0,pp->mamc_blend_y1,\
				// me:
				pp->hme_half_pel_en,pp->mv_cost_smooth_gain,pp->mv_cost_smooth_en,\
				pp->mv_cost_gmv_smooth_gain,pp->mv_cost_gmv_smooth_en,\
				pp->mv0_protect_th,pp->mv0_protect_en,pp->sad_protect_en,pp->sad_protect_gm_en,\
				pp->mv_protect_control_x0,pp->mv_protect_control_x1,\
				pp->mv_protect_control_y0,pp->mv_protect_control_y1);
			if(PE_CHECK_WIN0(pp->win_id))
			{
				/* ma: motion */
				PE_P0L_M14A0_QWr02(p0l_tnr_ctrl_38,	m_coring_en_c,		GET_BITS(pp->m_coring_en_c,0,1),\
													m_coring_en_y,		GET_BITS(pp->m_coring_en_y,0,1));
				PE_P0L_M14A0_QWr03(p0l_tnr_ctrl_38,	m_gain_c,			GET_BITS(pp->m_gain_c,0,4),\
													m_gain_y,			GET_BITS(pp->m_gain_y,0,4),\
													m_coring_th,		GET_BITS(pp->m_coring_th,0,6));
				PE_P0L_M14A0_QWr03(p0l_tnr_ctrl_02,	m_offset_mode_en,	GET_BITS(pp->m_offset_mode_en,0,1),\
													m_gain_en_var,		GET_BITS(pp->m_gain_en_var,0,1),\
													a_gain_en_var,		GET_BITS(pp->a_gain_en_var,0,1));
				PE_P0L_M14A0_QWr04(p0l_tnr_ctrl_03,	wa_y0,				GET_BITS(pp->wa_y0,0,8),\
													wa_y1,				GET_BITS(pp->wa_y1,0,8),\
													wa_x0,				GET_BITS(pp->wa_x0,0,8),\
													wa_x1,				GET_BITS(pp->wa_x1,0,8));
				PE_P0L_M14A0_QWr04(p0l_tnr_ctrl_29,	wa_y2,				GET_BITS(pp->wa_y2,0,8),\
													wa_y3,				GET_BITS(pp->wa_y3,0,8),\
													wa_x2,				GET_BITS(pp->wa_x2,0,8),\
													wa_x3,				GET_BITS(pp->wa_x3,0,8));
				/* ma: alpha gain */
				PE_P0L_M14A0_QWr04(p0l_tnr_ctrl_19,	luma_gain_p0_x,			GET_BITS(pp->luma_gain_p0_x,0,8),\
													luma_gain_p1_x,			GET_BITS(pp->luma_gain_p1_x,0,8),\
													luma_gain_p2_x,			GET_BITS(pp->luma_gain_p2_x,0,8),\
													luma_gain_p3_x,			GET_BITS(pp->luma_gain_p3_x,0,8));
				PE_P0L_M14A0_QWr04(p0l_tnr_ctrl_20,	luma_gain_p0_y,			GET_BITS(pp->luma_gain_p0_y,0,8),\
													luma_gain_p1_y,			GET_BITS(pp->luma_gain_p1_y,0,8),\
													luma_gain_p2_y,			GET_BITS(pp->luma_gain_p2_y,0,8),\
													luma_gain_p3_y,			GET_BITS(pp->luma_gain_p3_y,0,8));
				/* ma: alhpa remapping*/
				PE_P0L_M14A0_QWr03(p0l_tnr_ctrl_00,	alpha_avg_en,		GET_BITS(pp->alpha_avg_en,0,1),\
													alpha_mapping_y_en,	GET_BITS(pp->alpha_mapping_y_en,0,8),\
													alpha_mapping_c_en,	GET_BITS(pp->alpha_mapping_c_en,0,8));
				PE_P0L_M14A0_QWr04(p0l_tnr_ctrl_23,	alpha_mapping_y_y0,	GET_BITS(pp->alpha_mapping_y_y0,0,8),\
													alpha_mapping_y_x0,	GET_BITS(pp->alpha_mapping_y_x0,0,8),\
													alpha_mapping_y_y1,	GET_BITS(pp->alpha_mapping_y_y1,0,8),\
													alpha_mapping_y_x1,	GET_BITS(pp->alpha_mapping_y_x1,0,8));
				PE_P0L_M14A0_QWr04(p0l_tnr_ctrl_24,	alpha_mapping_y_y2,	GET_BITS(pp->alpha_mapping_y_y2,0,8),\
													alpha_mapping_y_x2,	GET_BITS(pp->alpha_mapping_y_x2,0,8),\
													alpha_mapping_y_y3,	GET_BITS(pp->alpha_mapping_y_y3,0,8),\
													alpha_mapping_y_x3,	GET_BITS(pp->alpha_mapping_y_x3,0,8));
				PE_P0L_M14A0_QWr04(p0l_tnr_ctrl_25,	alpha_mapping_c_y0,	GET_BITS(pp->alpha_mapping_c_y0,0,8),\
													alpha_mapping_c_x0,	GET_BITS(pp->alpha_mapping_c_x0,0,8),\
													alpha_mapping_c_y1,	GET_BITS(pp->alpha_mapping_c_y1,0,8),\
													alpha_mapping_c_x1,	GET_BITS(pp->alpha_mapping_c_x1,0,8));
				PE_P0L_M14A0_QWr04(p0l_tnr_ctrl_26,	alpha_mapping_c_y2,	GET_BITS(pp->alpha_mapping_c_y2,0,8),\
													alpha_mapping_c_x2,	GET_BITS(pp->alpha_mapping_c_x2,0,8),\
													alpha_mapping_c_y3,	GET_BITS(pp->alpha_mapping_c_y3,0,8),\
													alpha_mapping_c_x3,	GET_BITS(pp->alpha_mapping_c_x3,0,8));
				/*ma: noise level adjust gain*/
				PE_P0L_M14A0_QWr04(p0l_tnr_ctrl_00,	nt_lvl_adjust_gm_enable,		GET_BITS(pp->nt_lvl_adjust_gm_enable,0,1),\
													nt_lvl_adjust_lpct_enable,		GET_BITS(pp->nt_lvl_adjust_lpct_enable,0,1),\
													nt_lvl_adjust_avg_ts_enable,	GET_BITS(pp->nt_lvl_adjust_avg_ts_enable,0,1),\
													nt_lvl_adjust_lpct_sel,			GET_BITS(pp->nt_lvl_adjust_lpct_sel,0,1));
				PE_P0L_M14A0_QWr04(p0l_tnr_ctrl_04,	nt_lvl_gain_gm_y0,		GET_BITS(pp->nt_lvl_gain_gm_y0,0,8),\
													nt_lvl_gain_gm_x0,		GET_BITS(pp->nt_lvl_gain_gm_x0,0,8),\
													nt_lvl_gain_gm_y1,		GET_BITS(pp->nt_lvl_gain_gm_y1,0,8),\
													nt_lvl_gain_gm_x1,		GET_BITS(pp->nt_lvl_gain_gm_x1,0,8));
				PE_P0L_M14A0_QWr04(p0l_tnr_ctrl_12,	nt_lvl_gain_gm_y2,		GET_BITS(pp->nt_lvl_gain_gm_y2,0,8),\
													nt_lvl_gain_gm_x2,		GET_BITS(pp->nt_lvl_gain_gm_x2,0,8),\
													nt_lvl_gain_gm_y3,		GET_BITS(pp->nt_lvl_gain_gm_y3,0,8),\
													nt_lvl_gain_gm_x3,		GET_BITS(pp->nt_lvl_gain_gm_x3,0,8));
				PE_P0L_M14A0_QWr04(p0l_tnr_ctrl_28,	nt_lvl_gain_st_y0,		GET_BITS(pp->nt_lvl_gain_st_y0,0,8),\
													nt_lvl_gain_st_x0,		GET_BITS(pp->nt_lvl_gain_st_x0,0,8),\
													nt_lvl_gain_st_y1,		GET_BITS(pp->nt_lvl_gain_st_y1,0,8),\
													nt_lvl_gain_st_x1,		GET_BITS(pp->nt_lvl_gain_st_x1,0,8));
				PE_P0L_M14A0_QWr04(p0l_tnr_ctrl_30,	nt_lvl_gain_st_y2,		GET_BITS(pp->nt_lvl_gain_st_y2,0,8),\
													nt_lvl_gain_st_x2,		GET_BITS(pp->nt_lvl_gain_st_x2,0,8),\
													nt_lvl_gain_st_y3,		GET_BITS(pp->nt_lvl_gain_st_y3,0,8),\
													nt_lvl_gain_st_x3,		GET_BITS(pp->nt_lvl_gain_st_x3,0,8));
				PE_P0L_M14A0_QWr04(p0l_tnr_ctrl_32,	nt_lvl_gain_lpct_y0,	GET_BITS(pp->nt_lvl_gain_lpct_y0,0,8),\
													nt_lvl_gain_lpct_x0,	GET_BITS(pp->nt_lvl_gain_lpct_x0,0,8),\
													nt_lvl_gain_lpct_y1,	GET_BITS(pp->nt_lvl_gain_lpct_y1,0,8),\
													nt_lvl_gain_lpct_x1,	GET_BITS(pp->nt_lvl_gain_lpct_x1,0,8));
				PE_P0L_M14A0_QWr04(p0l_tnr_ctrl_33,	nt_lvl_gain_lpct_y2,	GET_BITS(pp->nt_lvl_gain_lpct_y2,0,8),\
													nt_lvl_gain_lpct_x2,	GET_BITS(pp->nt_lvl_gain_lpct_x2,0,8),\
													nt_lvl_gain_lpct_y3,	GET_BITS(pp->nt_lvl_gain_lpct_y3,0,8),\
													nt_lvl_gain_lpct_x3,	GET_BITS(pp->nt_lvl_gain_lpct_x3,0,8));
				PE_P0L_M14A0_QWr01(p0l_tnr_ctrl_02,	m_gain_en_var2,			GET_BITS(pp->m_gain_en_var2,0,1));
				PE_P0L_M14A0_QWr04(p0l_tnr_ctrl_48,	m_gain_ctrl2_y0,		GET_BITS(pp->m_gain_ctrl2_y0,0,8),\
													m_gain_ctrl2_x0,		GET_BITS(pp->m_gain_ctrl2_x0,0,8),\
													m_gain_ctrl2_y1,		GET_BITS(pp->m_gain_ctrl2_y1,0,8),\
													m_gain_ctrl2_x1,		GET_BITS(pp->m_gain_ctrl2_x1,0,8));
				PE_P0L_M14A0_QWr03(p0l_tnr_ctrl_40,	sf_map_flt_en,			GET_BITS(pp->sf_map_flt_en,0,1),\
													sf_map_tap,				GET_BITS(pp->sf_map_tap,0,2),\
													sf_map_gain,			GET_BITS(pp->sf_map_gain,0,6));
				PE_P0L_M14A0_QWr02(p0l_tnr_ctrl_40,	sf_th0,					GET_BITS(pp->sf_th0,0,8),\
													sf_th1,					GET_BITS(pp->sf_th1,0,8));
				PE_P0L_M14A0_QWr03(p0l_tnr_ctrl_41,	sf_th2,					GET_BITS(pp->sf_th2,0,8),\
													sf_th3,					GET_BITS(pp->sf_th3,0,8),\
													sf_th4,					GET_BITS(pp->sf_th4,0,8));
				PE_P0L_M14A0_QWr04(p0l_tnr_ctrl_42,	sf_mb_x0,				GET_BITS(pp->sf_mb_x0,0,8),\
													sf_mb_x1,				GET_BITS(pp->sf_mb_x1,0,8),\
													sf_mb_y0,				GET_BITS(pp->sf_mb_y0,0,8),\
													sf_mb_y1,				GET_BITS(pp->sf_mb_y1,0,8));
				PE_P0L_M14A0_QWr04(p0l_tnr_ctrl_44,	sf_mb_x2,				GET_BITS(pp->sf_mb_x2,0,8),\
													sf_mb_x3,				GET_BITS(pp->sf_mb_x3,0,8),\
													sf_mb_y2,				GET_BITS(pp->sf_mb_y2,0,8),\
													sf_mb_y3,				GET_BITS(pp->sf_mb_y3,0,8));
				PE_P0L_M14A0_QWr02(p0l_tnr_ctrl_45,	sf_gain_mode,			GET_BITS(pp->sf_gain_mode,0,1),\
													sf_gain,				GET_BITS(pp->sf_gain,0,4));
				PE_P0L_M14A0_QWr04(p0l_tnr_ctrl_36,	sf_blend_en_y,			GET_BITS(pp->sf_blend_en_y,0,1),\
													sf_blend_en_c,			GET_BITS(pp->sf_blend_en_c,0,1),\
													sf_blend_motion_exp_mode,	GET_BITS(pp->sf_blend_motion_exp_mode,0,2),\
													sad_sel_pels,			GET_BITS(pp->sad_sel_pels,0,1));
				/* mc */
				PE_P0L_M14A0_QWr01(p0l_tnr_ctrl_21,	sad_mamc_blend,		GET_BITS(pp->sad_mamc_blend,0,8));
				PE_P0L_M14A0_QWr01(p0l_tnr_ctrl_00,	mamc_blend_sel,		GET_BITS(pp->mamc_blend_sel,0,1));
				PE_P0L_M14A0_QWr04(p0l_tnr_ctrl_46,	mamc_blend_x0,		GET_BITS(pp->mamc_blend_x0,0,8),\
													mamc_blend_x1,		GET_BITS(pp->mamc_blend_x1,0,8),\
													mamc_blend_y0,		GET_BITS(pp->mamc_blend_y0,0,8),\
													mamc_blend_y1,		GET_BITS(pp->mamc_blend_y1,0,8));
				/* me */
				PE_P0L_M14A0_QWr03(p0l_tnr_ctrl_36,	hme_half_pel_en,		GET_BITS(pp->hme_half_pel_en,0,1),\
													mv_cost_smooth_gain,	GET_BITS(pp->mv_cost_smooth_gain,0,5),\
													mv_cost_smooth_en,		GET_BITS(pp->mv_cost_smooth_en,0,1));
				PE_P0L_M14A0_QWr02(p0l_tnr_ctrl_36,	mv0_protect_th,			GET_BITS(pp->mv0_protect_th,0,7),\
													mv0_protect_en,			GET_BITS(pp->mv0_protect_en,0,1));
				PE_P0L_M14A0_QWr04(p0l_tnr_ctrl_38,	mv_cost_gmv_smooth_gain,	GET_BITS(pp->mv_cost_gmv_smooth_gain,0,5),\
													mv_cost_gmv_smooth_en,		GET_BITS(pp->mv_cost_gmv_smooth_en,0,1),\
													sad_protect_en,				GET_BITS(pp->sad_protect_en,0,1),\
													sad_protect_gm_en,			GET_BITS(pp->sad_protect_gm_en,0,1));
				PE_P0L_M14A0_QWr04(p0l_tnr_ctrl_22,	mv_protect_control_x0,		GET_BITS(pp->mv_protect_control_x0,0,8),\
													mv_protect_control_x1,		GET_BITS(pp->mv_protect_control_x1,0,8),\
													mv_protect_control_y0,		GET_BITS(pp->mv_protect_control_y0,0,8),\
													mv_protect_control_y1,		GET_BITS(pp->mv_protect_control_y1,0,8));
			}
			if(PE_CHECK_WIN1(pp->win_id))
			{
				/* ma: motion */
				PE_P0R_M14A0_QWr02(p0r_tnr_ctrl_38,	m_coring_en_c,		GET_BITS(pp->m_coring_en_c,0,1),\
													m_coring_en_y,		GET_BITS(pp->m_coring_en_y,0,1));
				PE_P0R_M14A0_QWr03(p0r_tnr_ctrl_38,	m_gain_c,			GET_BITS(pp->m_gain_c,0,4),\
													m_gain_y,			GET_BITS(pp->m_gain_y,0,4),\
													m_coring_th,		GET_BITS(pp->m_coring_th,0,6));
				PE_P0R_M14A0_QWr03(p0r_tnr_ctrl_02,	m_offset_mode_en,	GET_BITS(pp->m_offset_mode_en,0,1),\
													m_gain_en_var,		GET_BITS(pp->m_gain_en_var,0,1),\
													a_gain_en_var,		GET_BITS(pp->a_gain_en_var,0,1));
				PE_P0R_M14A0_QWr04(p0r_tnr_ctrl_03,	wa_y0,				GET_BITS(pp->wa_y0,0,8),\
													wa_y1,				GET_BITS(pp->wa_y1,0,8),\
													wa_x0,				GET_BITS(pp->wa_x0,0,8),\
													wa_x1,				GET_BITS(pp->wa_x1,0,8));
				PE_P0R_M14A0_QWr04(p0r_tnr_ctrl_29,	wa_y2,				GET_BITS(pp->wa_y2,0,8),\
													wa_y3,				GET_BITS(pp->wa_y3,0,8),\
													wa_x2,				GET_BITS(pp->wa_x2,0,8),\
													wa_x3,				GET_BITS(pp->wa_x3,0,8));
				/* ma: alpha gain */
				PE_P0R_M14A0_QWr04(p0r_tnr_ctrl_19,	luma_gain_p0_x,			GET_BITS(pp->luma_gain_p0_x,0,8),\
													luma_gain_p1_x,			GET_BITS(pp->luma_gain_p1_x,0,8),\
													luma_gain_p2_x,			GET_BITS(pp->luma_gain_p2_x,0,8),\
													luma_gain_p3_x,			GET_BITS(pp->luma_gain_p3_x,0,8));
				PE_P0R_M14A0_QWr04(p0r_tnr_ctrl_20,	luma_gain_p0_y,			GET_BITS(pp->luma_gain_p0_y,0,8),\
													luma_gain_p1_y,			GET_BITS(pp->luma_gain_p1_y,0,8),\
													luma_gain_p2_y,			GET_BITS(pp->luma_gain_p2_y,0,8),\
													luma_gain_p3_y,			GET_BITS(pp->luma_gain_p3_y,0,8));
				/* ma: alhpa remapping*/
				PE_P0R_M14A0_QWr03(p0r_tnr_ctrl_00,	alpha_avg_en,		GET_BITS(pp->alpha_avg_en,0,1),\
													alpha_mapping_y_en,	GET_BITS(pp->alpha_mapping_y_en,0,8),\
													alpha_mapping_c_en,	GET_BITS(pp->alpha_mapping_c_en,0,8));
				PE_P0R_M14A0_QWr04(p0r_tnr_ctrl_23,	alpha_mapping_y_y0,	GET_BITS(pp->alpha_mapping_y_y0,0,8),\
													alpha_mapping_y_x0,	GET_BITS(pp->alpha_mapping_y_x0,0,8),\
													alpha_mapping_y_y1,	GET_BITS(pp->alpha_mapping_y_y1,0,8),\
													alpha_mapping_y_x1,	GET_BITS(pp->alpha_mapping_y_x1,0,8));
				PE_P0R_M14A0_QWr04(p0r_tnr_ctrl_24,	alpha_mapping_y_y2,	GET_BITS(pp->alpha_mapping_y_y2,0,8),\
													alpha_mapping_y_x2,	GET_BITS(pp->alpha_mapping_y_x2,0,8),\
													alpha_mapping_y_y3,	GET_BITS(pp->alpha_mapping_y_y3,0,8),\
													alpha_mapping_y_x3,	GET_BITS(pp->alpha_mapping_y_x3,0,8));
				PE_P0R_M14A0_QWr04(p0r_tnr_ctrl_25,	alpha_mapping_c_y0,	GET_BITS(pp->alpha_mapping_c_y0,0,8),\
													alpha_mapping_c_x0,	GET_BITS(pp->alpha_mapping_c_x0,0,8),\
													alpha_mapping_c_y1,	GET_BITS(pp->alpha_mapping_c_y1,0,8),\
													alpha_mapping_c_x1,	GET_BITS(pp->alpha_mapping_c_x1,0,8));
				PE_P0R_M14A0_QWr04(p0r_tnr_ctrl_26,	alpha_mapping_c_y2,	GET_BITS(pp->alpha_mapping_c_y2,0,8),\
													alpha_mapping_c_x2,	GET_BITS(pp->alpha_mapping_c_x2,0,8),\
													alpha_mapping_c_y3,	GET_BITS(pp->alpha_mapping_c_y3,0,8),\
													alpha_mapping_c_x3,	GET_BITS(pp->alpha_mapping_c_x3,0,8));
				/*ma: noise level adjust gain*/
				PE_P0R_M14A0_QWr04(p0r_tnr_ctrl_00,	nt_lvl_adjust_gm_enable,		GET_BITS(pp->nt_lvl_adjust_gm_enable,0,1),\
													nt_lvl_adjust_lpct_enable,		GET_BITS(pp->nt_lvl_adjust_lpct_enable,0,1),\
													nt_lvl_adjust_avg_ts_enable,	GET_BITS(pp->nt_lvl_adjust_avg_ts_enable,0,1),\
													nt_lvl_adjust_lpct_sel,			GET_BITS(pp->nt_lvl_adjust_lpct_sel,0,1));
				PE_P0R_M14A0_QWr04(p0r_tnr_ctrl_04,	nt_lvl_gain_gm_y0,		GET_BITS(pp->nt_lvl_gain_gm_y0,0,8),\
													nt_lvl_gain_gm_x0,		GET_BITS(pp->nt_lvl_gain_gm_x0,0,8),\
													nt_lvl_gain_gm_y1,		GET_BITS(pp->nt_lvl_gain_gm_y1,0,8),\
													nt_lvl_gain_gm_x1,		GET_BITS(pp->nt_lvl_gain_gm_x1,0,8));
				PE_P0R_M14A0_QWr04(p0r_tnr_ctrl_12,	nt_lvl_gain_gm_y2,		GET_BITS(pp->nt_lvl_gain_gm_y2,0,8),\
													nt_lvl_gain_gm_x2,		GET_BITS(pp->nt_lvl_gain_gm_x2,0,8),\
													nt_lvl_gain_gm_y3,		GET_BITS(pp->nt_lvl_gain_gm_y3,0,8),\
													nt_lvl_gain_gm_x3,		GET_BITS(pp->nt_lvl_gain_gm_x3,0,8));
				PE_P0R_M14A0_QWr04(p0r_tnr_ctrl_28,	nt_lvl_gain_st_y0,		GET_BITS(pp->nt_lvl_gain_st_y0,0,8),\
													nt_lvl_gain_st_x0,		GET_BITS(pp->nt_lvl_gain_st_x0,0,8),\
													nt_lvl_gain_st_y1,		GET_BITS(pp->nt_lvl_gain_st_y1,0,8),\
													nt_lvl_gain_st_x1,		GET_BITS(pp->nt_lvl_gain_st_x1,0,8));
				PE_P0R_M14A0_QWr04(p0r_tnr_ctrl_30,	nt_lvl_gain_st_y2,		GET_BITS(pp->nt_lvl_gain_st_y2,0,8),\
													nt_lvl_gain_st_x2,		GET_BITS(pp->nt_lvl_gain_st_x2,0,8),\
													nt_lvl_gain_st_y3,		GET_BITS(pp->nt_lvl_gain_st_y3,0,8),\
													nt_lvl_gain_st_x3,		GET_BITS(pp->nt_lvl_gain_st_x3,0,8));
				PE_P0R_M14A0_QWr04(p0r_tnr_ctrl_32,	nt_lvl_gain_lpct_y0,	GET_BITS(pp->nt_lvl_gain_lpct_y0,0,8),\
													nt_lvl_gain_lpct_x0,	GET_BITS(pp->nt_lvl_gain_lpct_x0,0,8),\
													nt_lvl_gain_lpct_y1,	GET_BITS(pp->nt_lvl_gain_lpct_y1,0,8),\
													nt_lvl_gain_lpct_x1,	GET_BITS(pp->nt_lvl_gain_lpct_x1,0,8));
				PE_P0R_M14A0_QWr04(p0r_tnr_ctrl_33,	nt_lvl_gain_lpct_y2,	GET_BITS(pp->nt_lvl_gain_lpct_y2,0,8),\
													nt_lvl_gain_lpct_x2,	GET_BITS(pp->nt_lvl_gain_lpct_x2,0,8),\
													nt_lvl_gain_lpct_y3,	GET_BITS(pp->nt_lvl_gain_lpct_y3,0,8),\
													nt_lvl_gain_lpct_x3,	GET_BITS(pp->nt_lvl_gain_lpct_x3,0,8));
				PE_P0R_M14A0_QWr01(p0r_tnr_ctrl_02,	m_gain_en_var2,			GET_BITS(pp->m_gain_en_var2,0,1));
				PE_P0R_M14A0_QWr04(p0r_tnr_ctrl_48,	m_gain_ctrl2_y0,		GET_BITS(pp->m_gain_ctrl2_y0,0,8),\
													m_gain_ctrl2_x0,		GET_BITS(pp->m_gain_ctrl2_x0,0,8),\
													m_gain_ctrl2_y1,		GET_BITS(pp->m_gain_ctrl2_y1,0,8),\
													m_gain_ctrl2_x1,		GET_BITS(pp->m_gain_ctrl2_x1,0,8));
				PE_P0R_M14A0_QWr03(p0r_tnr_ctrl_40,	sf_map_flt_en,			GET_BITS(pp->sf_map_flt_en,0,1),\
													sf_map_tap,				GET_BITS(pp->sf_map_tap,0,2),\
													sf_map_gain,			GET_BITS(pp->sf_map_gain,0,6));
				PE_P0R_M14A0_QWr02(p0r_tnr_ctrl_40,	sf_th0,					GET_BITS(pp->sf_th0,0,8),\
													sf_th1,					GET_BITS(pp->sf_th1,0,8));
				PE_P0R_M14A0_QWr03(p0r_tnr_ctrl_41,	sf_th2,					GET_BITS(pp->sf_th2,0,8),\
													sf_th3,					GET_BITS(pp->sf_th3,0,8),\
													sf_th4,					GET_BITS(pp->sf_th4,0,8));
				PE_P0R_M14A0_QWr04(p0r_tnr_ctrl_42,	sf_mb_x0,				GET_BITS(pp->sf_mb_x0,0,8),\
													sf_mb_x1,				GET_BITS(pp->sf_mb_x1,0,8),\
													sf_mb_y0,				GET_BITS(pp->sf_mb_y0,0,8),\
													sf_mb_y1,				GET_BITS(pp->sf_mb_y1,0,8));
				PE_P0R_M14A0_QWr04(p0r_tnr_ctrl_44,	sf_mb_x2,				GET_BITS(pp->sf_mb_x2,0,8),\
													sf_mb_x3,				GET_BITS(pp->sf_mb_x3,0,8),\
													sf_mb_y2,				GET_BITS(pp->sf_mb_y2,0,8),\
													sf_mb_y3,				GET_BITS(pp->sf_mb_y3,0,8));
				PE_P0R_M14A0_QWr02(p0r_tnr_ctrl_45,	sf_gain_mode,			GET_BITS(pp->sf_gain_mode,0,1),\
													sf_gain,				GET_BITS(pp->sf_gain,0,4));
				PE_P0R_M14A0_QWr04(p0r_tnr_ctrl_36,	sf_blend_en_y,			GET_BITS(pp->sf_blend_en_y,0,1),\
													sf_blend_en_c,			GET_BITS(pp->sf_blend_en_c,0,1),\
													sf_blend_motion_exp_mode,	GET_BITS(pp->sf_blend_motion_exp_mode,0,2),\
													sad_sel_pels,			GET_BITS(pp->sad_sel_pels,0,1));
				/* mc */
				PE_P0R_M14A0_QWr01(p0r_tnr_ctrl_21,	sad_mamc_blend,		GET_BITS(pp->sad_mamc_blend,0,8));
				PE_P0R_M14A0_QWr01(p0r_tnr_ctrl_00,	mamc_blend_sel,		GET_BITS(pp->mamc_blend_sel,0,1));
				PE_P0R_M14A0_QWr04(p0r_tnr_ctrl_46,	mamc_blend_x0,		GET_BITS(pp->mamc_blend_x0,0,8),\
													mamc_blend_x1,		GET_BITS(pp->mamc_blend_x1,0,8),\
													mamc_blend_y0,		GET_BITS(pp->mamc_blend_y0,0,8),\
													mamc_blend_y1,		GET_BITS(pp->mamc_blend_y1,0,8));
				/* me */
				PE_P0R_M14A0_QWr03(p0r_tnr_ctrl_36,	hme_half_pel_en,		GET_BITS(pp->hme_half_pel_en,0,1),\
													mv_cost_smooth_gain,	GET_BITS(pp->mv_cost_smooth_gain,0,5),\
													mv_cost_smooth_en,		GET_BITS(pp->mv_cost_smooth_en,0,1));
				PE_P0R_M14A0_QWr02(p0r_tnr_ctrl_36,	mv0_protect_th,			GET_BITS(pp->mv0_protect_th,0,7),\
													mv0_protect_en,			GET_BITS(pp->mv0_protect_en,0,1));
				PE_P0R_M14A0_QWr04(p0r_tnr_ctrl_38,	mv_cost_gmv_smooth_gain,	GET_BITS(pp->mv_cost_gmv_smooth_gain,0,5),\
													mv_cost_gmv_smooth_en,		GET_BITS(pp->mv_cost_gmv_smooth_en,0,1),\
													sad_protect_en,				GET_BITS(pp->sad_protect_en,0,1),\
													sad_protect_gm_en,			GET_BITS(pp->sad_protect_gm_en,0,1));
				PE_P0R_M14A0_QWr04(p0r_tnr_ctrl_22,	mv_protect_control_x0,		GET_BITS(pp->mv_protect_control_x0,0,8),\
													mv_protect_control_x1,		GET_BITS(pp->mv_protect_control_x1,0,8),\
													mv_protect_control_y0,		GET_BITS(pp->mv_protect_control_y0,0,8),\
													mv_protect_control_y1,		GET_BITS(pp->mv_protect_control_y1,0,8));
				}
		}
		else
		{
			PE_NRD_HW_M14_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * get tnr detail ctrl
 * - use input struct LX_PE_NRD_TNR2_DETAIL_T for M14AX
 *
 * @param   *pstParams [in/out] void
 * @return  OK if success, ERROR otherwise.
 * @see		set discription
 * @author
 */
int PE_NRD_HW_M14_GetTnrDetailCtrl(void *pstParams)
{
	int ret = RET_OK;
	LX_PE_WIN_ID win_id;
	do{
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_M14BX)
		{
			LX_PE_NRD_TNR4_DETAIL_T *pp=(LX_PE_NRD_TNR4_DETAIL_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			win_id = PE_GET_CHECKED_WINID(pp->win_id);
			if(PE_CHECK_WIN0(win_id))
			{
				/*ma 26*/
				PE_P0M_M14B0_QRd01(p0m_tnr_ctrl_25, reg_g_blend_a,			pp->reg_g_blend_a);
				PE_P0M_M14B0_QRd01(p0m_tnr_ctrl_31, motion_sc_reset_en,		pp->motion_sc_reset_en);
				PE_P0M_M14B0_QRd04(p0m_tnr_ctrl_33, luma_gain_ctrl_x0,		pp->luma_gain_ctrl_x0, \
													luma_gain_ctrl_x1,		pp->luma_gain_ctrl_x1, \
													luma_gain_ctrl_x2,		pp->luma_gain_ctrl_x2, \
													luma_gain_ctrl_x3,		pp->luma_gain_ctrl_x3);
				PE_P0M_M14B0_QRd04(p0m_tnr_ctrl_34, luma_gain_ctrl_y0,		pp->luma_gain_ctrl_y0, \
													luma_gain_ctrl_y1,		pp->luma_gain_ctrl_y1, \
													luma_gain_ctrl_y2,		pp->luma_gain_ctrl_y2, \
													luma_gain_ctrl_y3,		pp->luma_gain_ctrl_y3);
				PE_P0M_M14B0_QRd04(p0m_tnr_ctrl_32, skingain_motioncontrol_x0,pp->skingain_motioncontrol_x0, \
													skingain_motioncontrol_x1,pp->skingain_motioncontrol_x1, \
													skingain_motioncontrol_y0,pp->skingain_motioncontrol_y0, \
													skingain_motioncontrol_y1,pp->skingain_motioncontrol_y1);
				PE_P0M_M14B0_QRd04(p0m_tnr_ctrl_35, skin_lut_y0,			pp->skin_lut_y0, \
													skin_lut_y1,			pp->skin_lut_y1, \
													skin_lut_y2,			pp->skin_lut_y2, \
													skin_lut_y3,			pp->skin_lut_y3);
				PE_P0M_M14B0_QRd04(p0m_tnr_ctrl_36, skin_lut_cb0,			pp->skin_lut_cb0, \
													skin_lut_cb1,			pp->skin_lut_cb1, \
													skin_lut_cb2,			pp->skin_lut_cb2, \
													skin_lut_cb3,			pp->skin_lut_cb3);
				PE_P0M_M14B0_QRd04(p0m_tnr_ctrl_37, skin_lut_cr0,			pp->skin_lut_cr0, \
													skin_lut_cr1,			pp->skin_lut_cr1, \
													skin_lut_cr2,			pp->skin_lut_cr2, \
													skin_lut_cr3,			pp->skin_lut_cr3);
				/*me 22*/
				PE_P0M_M14B0_QRd02(p0m_tnr_ctrl_07, past_refine_ratio,		pp->past_refine_ratio, \
													global_past_refine_ratio,pp->global_past_refine_ratio);
				PE_P0M_M14B0_QRd01(p0m_tnr_ctrl_08, mv_refine_ratio,		pp->mv_refine_ratio);
				PE_P0M_M14B0_QRd03(p0m_tnr_ctrl_09, temporal_refine_adj1,	pp->temporal_refine_adj1, \
													temporal_refine_adj2,	pp->temporal_refine_adj2, \
													zero_refine_ratio,		pp->zero_refine_ratio);
				PE_P0M_M14B0_QRd02(p0m_tnr_ctrl_12, me_conf_scale_ma,		pp->me_conf_scale_ma, \
													me_conf_scale_mc,		pp->me_conf_scale_mc);
				PE_P0M_M14B0_QRd05(p0m_tnr_ctrl_39, past_x0,				pp->past_x0, \
													past_x1,				pp->past_x1, \
													past_x2,				pp->past_x2, \
													past_y0,				pp->past_y0, \
													past_y1,				pp->past_y1);
				PE_P0M_M14B0_QRd02(p0m_tnr_ctrl_38, past_y2,				pp->past_y2, \
													past_y3,				pp->past_y3);
				PE_P0M_M14B0_QRd05(p0m_tnr_ctrl_04, global_x0,				pp->global_x0, \
													global_x1,				pp->global_x1, \
													global_x2,				pp->global_x2, \
													global_y0,				pp->global_y0, \
													global_y1,				pp->global_y1);
				PE_P0M_M14B0_QRd02(p0m_tnr_ctrl_05, global_y2,				pp->global_y2, \
													global_y3,				pp->global_y3);
				/*mc 5*/
				PE_P0M_M14B0_QRd02(p0m_tnr_ctrl_16, am_th_mode,				pp->am_th_mode, \
													am_th_val,				pp->am_th_val);
				PE_P0M_M14B0_QRd02(p0m_tnr_ctrl_19, mcblnd_th_x0,			pp->mcblnd_th_x0, \
													mcblnd_th_x1,			pp->mcblnd_th_x1);
				PE_P0M_M14B0_QRd01(p0m_tnr_ctrl_20, sc_alpha_mode,			pp->sc_alpha_mode);
				/*ma post 4*/
				PE_P0M_M14B0_QRd04(p0m_tnr_ctrl_43, reg_ma_conf_x0,			pp->reg_ma_conf_x0, \
													reg_ma_conf_x1,			pp->reg_ma_conf_x1, \
													reg_ma_conf_y0,			pp->reg_ma_conf_y0, \
													reg_ma_conf_y1,			pp->reg_ma_conf_y1);
				/*mc post 4*/
				PE_P0M_M14B0_QRd04(p0m_tnr_ctrl_42, reg_mc_conf_x0,			pp->reg_mc_conf_x0, \
													reg_mc_conf_x1,			pp->reg_mc_conf_x1, \
													reg_mc_conf_y0,			pp->reg_mc_conf_y0, \
													reg_mc_conf_y1,			pp->reg_mc_conf_y1);
				/*snr 7*/
				PE_P0M_M14B0_QRd03(p0m_tnr_ctrl_48, reg_snr_master_gain,	pp->reg_snr_master_gain, \
													reg_snr_c_blur_gain,	pp->reg_snr_c_blur_gain, \
													reg_snr_y_blur_gain,	pp->reg_snr_y_blur_gain);
				PE_P0M_M14B0_QRd04(p0m_tnr_ctrl_50, snr_blendgain_x0,		pp->snr_blendgain_x0, \
													snr_blendgain_x1,		pp->snr_blendgain_x1, \
													snr_blendgain_y0,		pp->snr_blendgain_y0, \
													snr_blendgain_y1,		pp->snr_blendgain_y1);
			}
			PE_NRD_HW_M14_DBG_PRINT("set[%d] \n"\
				"ma     : 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				"ma     : 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				"ma     : 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				"ma     : 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				"me     : 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				"me     : 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				"me     : 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				"me     : 0x%02x,0x%02x,0x%02x,0x%02x\n"\
				"mc     : 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				"ma post: 0x%02x,0x%02x,0x%02x,0x%02x\n"\
				"mc post: 0x%02x,0x%02x,0x%02x,0x%02x\n"\
				"snr    : 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n", pp->win_id, \
				/*ma 26*/
				pp->reg_g_blend_a, pp->motion_sc_reset_en, pp->luma_gain_ctrl_x0, \
				pp->luma_gain_ctrl_x1, pp->luma_gain_ctrl_x2, pp->luma_gain_ctrl_x3, \
				pp->luma_gain_ctrl_y0, pp->luma_gain_ctrl_y1, pp->luma_gain_ctrl_y2, \
				pp->luma_gain_ctrl_y3, pp->skingain_motioncontrol_x0, \
				pp->skingain_motioncontrol_x1, pp->skingain_motioncontrol_y0, \
				pp->skingain_motioncontrol_y1, pp->skin_lut_y0, \
				pp->skin_lut_y1, pp->skin_lut_y2, pp->skin_lut_y3, pp->skin_lut_cb0, \
				pp->skin_lut_cb1, pp->skin_lut_cb2, pp->skin_lut_cb3, pp->skin_lut_cr0, \
				pp->skin_lut_cr1, pp->skin_lut_cr2, pp->skin_lut_cr3, \
				/*me 22*/
				pp->past_refine_ratio, pp->global_past_refine_ratio, pp->mv_refine_ratio, \
				pp->temporal_refine_adj1, pp->temporal_refine_adj2, pp->zero_refine_ratio, \
				pp->me_conf_scale_ma, pp->me_conf_scale_mc, pp->past_x0, pp->past_x1, \
				pp->past_x2, pp->past_y0, pp->past_y1, pp->past_y2, pp->past_y3, \
				pp->global_x0, pp->global_x1, pp->global_x2, pp->global_y0, \
				pp->global_y1, pp->global_y2, pp->global_y3, \
				/*mc 5*/
				pp->am_th_mode, pp->am_th_val, pp->mcblnd_th_x0, \
				pp->mcblnd_th_x1, pp->sc_alpha_mode, \
				/*ma post 4*/
				pp->reg_ma_conf_x0, pp->reg_ma_conf_x1, pp->reg_ma_conf_y0, pp->reg_ma_conf_y1, \
				/*mc post 4*/
				pp->reg_mc_conf_x0, pp->reg_mc_conf_x1, pp->reg_mc_conf_y0, pp->reg_mc_conf_y1, \
				/*snr 7*/
				pp->reg_snr_master_gain, pp->reg_snr_c_blur_gain, pp->reg_snr_y_blur_gain, \
				pp->snr_blendgain_x0, pp->snr_blendgain_x1, pp->snr_blendgain_y0, \
				pp->snr_blendgain_y1);
		}
		else if(PE_KDRV_VER_M14AX)
		{
			LX_PE_NRD_TNR2_DETAIL_T *pp=(LX_PE_NRD_TNR2_DETAIL_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			win_id = PE_GET_CHECKED_WINID(pp->win_id);
			if(PE_CHECK_WIN0(win_id))
			{
				// ma: motion
				PE_P0L_M14A0_QRd02(p0l_tnr_ctrl_38,	m_coring_en_c,		pp->m_coring_en_c,\
													m_coring_en_y,		pp->m_coring_en_y);
				PE_P0L_M14A0_QRd03(p0l_tnr_ctrl_38,	m_gain_c,			pp->m_gain_c,\
													m_gain_y,			pp->m_gain_y,\
													m_coring_th,		pp->m_coring_th);
				PE_P0L_M14A0_QRd03(p0l_tnr_ctrl_02,	m_offset_mode_en,	pp->m_offset_mode_en,\
													m_gain_en_var,		pp->m_gain_en_var,\
													a_gain_en_var,		pp->a_gain_en_var);
				PE_P0L_M14A0_QRd04(p0l_tnr_ctrl_03,	wa_y0,				pp->wa_y0,\
													wa_y1,				pp->wa_y1,\
													wa_x0,				pp->wa_x0,\
													wa_x1,				pp->wa_x1);
				PE_P0L_M14A0_QRd04(p0l_tnr_ctrl_29,	wa_y2,				pp->wa_y2,\
													wa_y3,				pp->wa_y3,\
													wa_x2,				pp->wa_x2,\
													wa_x3,				pp->wa_x3);
				// ma: alpha gain
				PE_P0L_M14A0_QRd04(p0l_tnr_ctrl_19,	luma_gain_p0_x,		pp->luma_gain_p0_x,\
													luma_gain_p1_x,		pp->luma_gain_p1_x,\
													luma_gain_p2_x,		pp->luma_gain_p2_x,\
													luma_gain_p3_x,		pp->luma_gain_p3_x);
				PE_P0L_M14A0_QRd04(p0l_tnr_ctrl_20,	luma_gain_p0_y,		pp->luma_gain_p0_y,\
													luma_gain_p1_y,		pp->luma_gain_p1_y,\
													luma_gain_p2_y,		pp->luma_gain_p2_y,\
													luma_gain_p3_y,		pp->luma_gain_p3_y);
				// ma: alhpa remapping
				PE_P0L_M14A0_QRd03(p0l_tnr_ctrl_00,	alpha_avg_en,		pp->alpha_avg_en,\
													alpha_mapping_y_en,	pp->alpha_mapping_y_en,\
													alpha_mapping_c_en,	pp->alpha_mapping_c_en);
				PE_P0L_M14A0_QRd04(p0l_tnr_ctrl_23,	alpha_mapping_y_y0,	pp->alpha_mapping_y_y0,\
													alpha_mapping_y_x0,	pp->alpha_mapping_y_x0,\
													alpha_mapping_y_y1,	pp->alpha_mapping_y_y1,\
													alpha_mapping_y_x1,	pp->alpha_mapping_y_x1);
				PE_P0L_M14A0_QRd04(p0l_tnr_ctrl_24,	alpha_mapping_y_y2,	pp->alpha_mapping_y_y2,\
													alpha_mapping_y_x2,	pp->alpha_mapping_y_x2,\
													alpha_mapping_y_y3,	pp->alpha_mapping_y_y3,\
													alpha_mapping_y_x3,	pp->alpha_mapping_y_x3);
				PE_P0L_M14A0_QRd04(p0l_tnr_ctrl_25,	alpha_mapping_c_y0,	pp->alpha_mapping_c_y0,\
													alpha_mapping_c_x0,	pp->alpha_mapping_c_x0,\
													alpha_mapping_c_y1,	pp->alpha_mapping_c_y1,\
													alpha_mapping_c_x1,	pp->alpha_mapping_c_x1);
				PE_P0L_M14A0_QRd04(p0l_tnr_ctrl_26,	alpha_mapping_c_y2,	pp->alpha_mapping_c_y2,\
													alpha_mapping_c_x2,	pp->alpha_mapping_c_x2,\
													alpha_mapping_c_y3,	pp->alpha_mapping_c_y3,\
													alpha_mapping_c_x3,	pp->alpha_mapping_c_x3);
				//ma: noise level adjust gain
				PE_P0L_M14A0_QRd04(p0l_tnr_ctrl_00,	nt_lvl_adjust_gm_enable,		pp->nt_lvl_adjust_gm_enable,\
													nt_lvl_adjust_lpct_enable,		pp->nt_lvl_adjust_lpct_enable,\
													nt_lvl_adjust_avg_ts_enable,	pp->nt_lvl_adjust_avg_ts_enable,\
													nt_lvl_adjust_lpct_sel,			pp->nt_lvl_adjust_lpct_sel);
				PE_P0L_M14A0_QRd04(p0l_tnr_ctrl_04,	nt_lvl_gain_gm_y0,		pp->nt_lvl_gain_gm_y0,\
													nt_lvl_gain_gm_x0,		pp->nt_lvl_gain_gm_x0,\
													nt_lvl_gain_gm_y1,		pp->nt_lvl_gain_gm_y1,\
													nt_lvl_gain_gm_x1,		pp->nt_lvl_gain_gm_x1);
				PE_P0L_M14A0_QRd04(p0l_tnr_ctrl_12,	nt_lvl_gain_gm_y2,		pp->nt_lvl_gain_gm_y2,\
													nt_lvl_gain_gm_x2,		pp->nt_lvl_gain_gm_x2,\
													nt_lvl_gain_gm_y3,		pp->nt_lvl_gain_gm_y3,\
													nt_lvl_gain_gm_x3,		pp->nt_lvl_gain_gm_x3);
				PE_P0L_M14A0_QRd04(p0l_tnr_ctrl_28,	nt_lvl_gain_st_y0,		pp->nt_lvl_gain_st_y0,\
													nt_lvl_gain_st_x0,		pp->nt_lvl_gain_st_x0,\
													nt_lvl_gain_st_y1,		pp->nt_lvl_gain_st_y1,\
													nt_lvl_gain_st_x1,		pp->nt_lvl_gain_st_x1);
				PE_P0L_M14A0_QRd04(p0l_tnr_ctrl_30,	nt_lvl_gain_st_y2,		pp->nt_lvl_gain_st_y2,\
													nt_lvl_gain_st_x2,		pp->nt_lvl_gain_st_x2,\
													nt_lvl_gain_st_y3,		pp->nt_lvl_gain_st_y3,\
													nt_lvl_gain_st_x3,		pp->nt_lvl_gain_st_x3);
				PE_P0L_M14A0_QRd04(p0l_tnr_ctrl_32,	nt_lvl_gain_lpct_y0,	pp->nt_lvl_gain_lpct_y0,\
													nt_lvl_gain_lpct_x0,	pp->nt_lvl_gain_lpct_x0,\
													nt_lvl_gain_lpct_y1,	pp->nt_lvl_gain_lpct_y1,\
													nt_lvl_gain_lpct_x1,	pp->nt_lvl_gain_lpct_x1);
				PE_P0L_M14A0_QRd04(p0l_tnr_ctrl_33,	nt_lvl_gain_lpct_y2,	pp->nt_lvl_gain_lpct_y2,\
													nt_lvl_gain_lpct_x2,	pp->nt_lvl_gain_lpct_x2,\
													nt_lvl_gain_lpct_y3,	pp->nt_lvl_gain_lpct_y3,\
													nt_lvl_gain_lpct_x3,	pp->nt_lvl_gain_lpct_x3);
				PE_P0L_M14A0_QRd01(p0l_tnr_ctrl_02,	m_gain_en_var2,		pp->m_gain_en_var2);
				PE_P0L_M14A0_QRd04(p0l_tnr_ctrl_48,	m_gain_ctrl2_y0,	pp->m_gain_ctrl2_y0,\
													m_gain_ctrl2_x0,	pp->m_gain_ctrl2_x0,\
													m_gain_ctrl2_y1,	pp->m_gain_ctrl2_y1,\
													m_gain_ctrl2_x1,	pp->m_gain_ctrl2_x1);
				PE_P0L_M14A0_QRd03(p0l_tnr_ctrl_40,	sf_map_flt_en,		pp->sf_map_flt_en,\
													sf_map_tap,			pp->sf_map_tap,\
													sf_map_gain,		pp->sf_map_gain);
				PE_P0L_M14A0_QRd02(p0l_tnr_ctrl_40,	sf_th0,				pp->sf_th0,\
													sf_th1,				pp->sf_th1);
				PE_P0L_M14A0_QRd03(p0l_tnr_ctrl_41,	sf_th2,				pp->sf_th2,\
													sf_th3,				pp->sf_th3,\
													sf_th4,				pp->sf_th4);
				PE_P0L_M14A0_QRd04(p0l_tnr_ctrl_42,	sf_mb_x0,			pp->sf_mb_x0,\
													sf_mb_x1,			pp->sf_mb_x1,\
													sf_mb_y0,			pp->sf_mb_y0,\
													sf_mb_y1,			pp->sf_mb_y1);
				PE_P0L_M14A0_QRd04(p0l_tnr_ctrl_44,	sf_mb_x2,			pp->sf_mb_x2,\
													sf_mb_x3,			pp->sf_mb_x3,\
													sf_mb_y2,			pp->sf_mb_y2,\
													sf_mb_y3,			pp->sf_mb_y3);
				PE_P0L_M14A0_QRd02(p0l_tnr_ctrl_45,	sf_gain_mode,		pp->sf_gain_mode,\
													sf_gain,			pp->sf_gain);
				PE_P0L_M14A0_QRd04(p0l_tnr_ctrl_36,	sf_blend_en_y,		pp->sf_blend_en_y,\
													sf_blend_en_c,		pp->sf_blend_en_c,\
													sf_blend_motion_exp_mode,	pp->sf_blend_motion_exp_mode,\
													sad_sel_pels,				pp->sad_sel_pels);
				//mc
				PE_P0L_M14A0_QRd01(p0l_tnr_ctrl_21,	sad_mamc_blend,		pp->sad_mamc_blend);
				PE_P0L_M14A0_QRd01(p0l_tnr_ctrl_00,	mamc_blend_sel,		pp->mamc_blend_sel);
				PE_P0L_M14A0_QRd04(p0l_tnr_ctrl_46,	mamc_blend_x0,		pp->mamc_blend_x0,\
													mamc_blend_x1,		pp->mamc_blend_x1,\
													mamc_blend_y0,		pp->mamc_blend_y0,\
													mamc_blend_y1,		pp->mamc_blend_y1);
				// me
				PE_P0L_M14A0_QRd03(p0l_tnr_ctrl_36,	hme_half_pel_en,		pp->hme_half_pel_en,\
													mv_cost_smooth_gain,	pp->mv_cost_smooth_gain,\
													mv_cost_smooth_en,		pp->mv_cost_smooth_en);
				PE_P0L_M14A0_QRd02(p0l_tnr_ctrl_36,	mv0_protect_th,			pp->mv0_protect_th,\
													mv0_protect_en,			pp->mv0_protect_en);
				PE_P0L_M14A0_QRd04(p0l_tnr_ctrl_38,	mv_cost_gmv_smooth_gain,pp->mv_cost_gmv_smooth_gain,\
													mv_cost_gmv_smooth_en,	pp->mv_cost_gmv_smooth_en,\
													sad_protect_en,			pp->sad_protect_en,\
													sad_protect_gm_en,		pp->sad_protect_gm_en);
				PE_P0L_M14A0_QRd04(p0l_tnr_ctrl_22,	mv_protect_control_x0,	pp->mv_protect_control_x0,\
													mv_protect_control_x1,	pp->mv_protect_control_x1,\
													mv_protect_control_y0,	pp->mv_protect_control_y0,\
													mv_protect_control_y1,	pp->mv_protect_control_y1);

			}
			if(PE_CHECK_WIN1(win_id))
			{
				/* ma: motion */
				PE_P0R_M14A0_QRd02(p0r_tnr_ctrl_38,	m_coring_en_c,		pp->m_coring_en_c,\
													m_coring_en_y,		pp->m_coring_en_y);
				PE_P0R_M14A0_QRd03(p0r_tnr_ctrl_38,	m_gain_c,			pp->m_gain_c,\
													m_gain_y,			pp->m_gain_y,\
													m_coring_th,		pp->m_coring_th);
				PE_P0R_M14A0_QRd03(p0r_tnr_ctrl_02,	m_offset_mode_en,	pp->m_offset_mode_en,\
													m_gain_en_var,		pp->m_gain_en_var,\
													a_gain_en_var,		pp->a_gain_en_var);
				PE_P0R_M14A0_QRd04(p0r_tnr_ctrl_03,	wa_y0,				pp->wa_y0,\
													wa_y1,				pp->wa_y1,\
													wa_x0,				pp->wa_x0,\
													wa_x1,				pp->wa_x1);
				PE_P0R_M14A0_QRd04(p0r_tnr_ctrl_29,	wa_y2,				pp->wa_y2,\
													wa_y3,				pp->wa_y3,\
													wa_x2,				pp->wa_x2,\
													wa_x3,				pp->wa_x3);
				/* ma: alpha gain */
				PE_P0R_M14A0_QRd04(p0r_tnr_ctrl_19,	luma_gain_p0_x,		pp->luma_gain_p0_x,\
													luma_gain_p1_x,		pp->luma_gain_p1_x,\
													luma_gain_p2_x,		pp->luma_gain_p2_x,\
													luma_gain_p3_x,		pp->luma_gain_p3_x);
				PE_P0R_M14A0_QRd04(p0r_tnr_ctrl_20,	luma_gain_p0_y,		pp->luma_gain_p0_y,\
													luma_gain_p1_y,		pp->luma_gain_p1_y,\
													luma_gain_p2_y,		pp->luma_gain_p2_y,\
													luma_gain_p3_y,		pp->luma_gain_p3_y);
				/* ma: alhpa remapping*/
				PE_P0R_M14A0_QRd03(p0r_tnr_ctrl_00,	alpha_avg_en,		pp->alpha_avg_en,\
													alpha_mapping_y_en,	pp->alpha_mapping_y_en,\
													alpha_mapping_c_en,	pp->alpha_mapping_c_en);
				PE_P0R_M14A0_QRd04(p0r_tnr_ctrl_23,	alpha_mapping_y_y0,	pp->alpha_mapping_y_y0,\
													alpha_mapping_y_x0,	pp->alpha_mapping_y_x0,\
													alpha_mapping_y_y1,	pp->alpha_mapping_y_y1,\
													alpha_mapping_y_x1,	pp->alpha_mapping_y_x1);
				PE_P0R_M14A0_QRd04(p0r_tnr_ctrl_24,	alpha_mapping_y_y2,	pp->alpha_mapping_y_y2,\
													alpha_mapping_y_x2,	pp->alpha_mapping_y_x2,\
													alpha_mapping_y_y3,	pp->alpha_mapping_y_y3,\
													alpha_mapping_y_x3,	pp->alpha_mapping_y_x3);
				PE_P0R_M14A0_QRd04(p0r_tnr_ctrl_25,	alpha_mapping_c_y0,	pp->alpha_mapping_c_y0,\
													alpha_mapping_c_x0,	pp->alpha_mapping_c_x0,\
													alpha_mapping_c_y1,	pp->alpha_mapping_c_y1,\
													alpha_mapping_c_x1,	pp->alpha_mapping_c_x1);
				PE_P0R_M14A0_QRd04(p0r_tnr_ctrl_26,	alpha_mapping_c_y2,	pp->alpha_mapping_c_y2,\
													alpha_mapping_c_x2,	pp->alpha_mapping_c_x2,\
													alpha_mapping_c_y3,	pp->alpha_mapping_c_y3,\
													alpha_mapping_c_x3,	pp->alpha_mapping_c_x3);
				/*ma: noise level adjust gain*/
				PE_P0R_M14A0_QRd04(p0r_tnr_ctrl_00,	nt_lvl_adjust_gm_enable,		pp->nt_lvl_adjust_gm_enable,\
													nt_lvl_adjust_lpct_enable,		pp->nt_lvl_adjust_lpct_enable,\
													nt_lvl_adjust_avg_ts_enable,	pp->nt_lvl_adjust_avg_ts_enable,\
													nt_lvl_adjust_lpct_sel,			pp->nt_lvl_adjust_lpct_sel);
				PE_P0R_M14A0_QRd04(p0r_tnr_ctrl_04,	nt_lvl_gain_gm_y0,		pp->nt_lvl_gain_gm_y0,\
													nt_lvl_gain_gm_x0,		pp->nt_lvl_gain_gm_x0,\
													nt_lvl_gain_gm_y1,		pp->nt_lvl_gain_gm_y1,\
													nt_lvl_gain_gm_x1,		pp->nt_lvl_gain_gm_x1);
				PE_P0R_M14A0_QRd04(p0r_tnr_ctrl_12,	nt_lvl_gain_gm_y2,		pp->nt_lvl_gain_gm_y2,\
													nt_lvl_gain_gm_x2,		pp->nt_lvl_gain_gm_x2,\
													nt_lvl_gain_gm_y3,		pp->nt_lvl_gain_gm_y3,\
													nt_lvl_gain_gm_x3,		pp->nt_lvl_gain_gm_x3);
				PE_P0R_M14A0_QRd04(p0r_tnr_ctrl_28,	nt_lvl_gain_st_y0,		pp->nt_lvl_gain_st_y0,\
													nt_lvl_gain_st_x0,		pp->nt_lvl_gain_st_x0,\
													nt_lvl_gain_st_y1,		pp->nt_lvl_gain_st_y1,\
													nt_lvl_gain_st_x1,		pp->nt_lvl_gain_st_x1);
				PE_P0R_M14A0_QRd04(p0r_tnr_ctrl_30,	nt_lvl_gain_st_y2,		pp->nt_lvl_gain_st_y2,\
													nt_lvl_gain_st_x2,		pp->nt_lvl_gain_st_x2,\
													nt_lvl_gain_st_y3,		pp->nt_lvl_gain_st_y3,\
													nt_lvl_gain_st_x3,		pp->nt_lvl_gain_st_x3);
				PE_P0R_M14A0_QRd04(p0r_tnr_ctrl_32,	nt_lvl_gain_lpct_y0,	pp->nt_lvl_gain_lpct_y0,\
													nt_lvl_gain_lpct_x0,	pp->nt_lvl_gain_lpct_x0,\
													nt_lvl_gain_lpct_y1,	pp->nt_lvl_gain_lpct_y1,\
													nt_lvl_gain_lpct_x1,	pp->nt_lvl_gain_lpct_x1);
				PE_P0R_M14A0_QRd04(p0r_tnr_ctrl_33,	nt_lvl_gain_lpct_y2,	pp->nt_lvl_gain_lpct_y2,\
													nt_lvl_gain_lpct_x2,	pp->nt_lvl_gain_lpct_x2,\
													nt_lvl_gain_lpct_y3,	pp->nt_lvl_gain_lpct_y3,\
													nt_lvl_gain_lpct_x3,	pp->nt_lvl_gain_lpct_x3);
				PE_P0R_M14A0_QRd01(p0r_tnr_ctrl_02,	m_gain_en_var2,			pp->m_gain_en_var2);
				PE_P0R_M14A0_QRd04(p0r_tnr_ctrl_48,	m_gain_ctrl2_y0,		pp->m_gain_ctrl2_y0,\
													m_gain_ctrl2_x0,		pp->m_gain_ctrl2_x0,\
													m_gain_ctrl2_y1,		pp->m_gain_ctrl2_y1,\
													m_gain_ctrl2_x1,		pp->m_gain_ctrl2_x1);
				PE_P0R_M14A0_QRd03(p0r_tnr_ctrl_40,	sf_map_flt_en,			pp->sf_map_flt_en,\
													sf_map_tap,				pp->sf_map_tap,\
													sf_map_gain,			pp->sf_map_gain);
				PE_P0R_M14A0_QRd02(p0r_tnr_ctrl_40,	sf_th0,					pp->sf_th0,\
													sf_th1,					pp->sf_th1);
				PE_P0R_M14A0_QRd03(p0r_tnr_ctrl_41,	sf_th2,					pp->sf_th2,\
													sf_th3,					pp->sf_th3,\
													sf_th4,					pp->sf_th4);
				PE_P0R_M14A0_QRd04(p0r_tnr_ctrl_42,	sf_mb_x0,				pp->sf_mb_x0,\
													sf_mb_x1,				pp->sf_mb_x1,\
													sf_mb_y0,				pp->sf_mb_y0,\
													sf_mb_y1,				pp->sf_mb_y1);
				PE_P0R_M14A0_QRd04(p0r_tnr_ctrl_44,	sf_mb_x2,				pp->sf_mb_x2,\
													sf_mb_x3,				pp->sf_mb_x3,\
													sf_mb_y2,				pp->sf_mb_y2,\
													sf_mb_y3,				pp->sf_mb_y3);
				PE_P0R_M14A0_QRd02(p0r_tnr_ctrl_45,	sf_gain_mode,			pp->sf_gain_mode,\
													sf_gain,				pp->sf_gain);
				PE_P0R_M14A0_QRd04(p0r_tnr_ctrl_36,	sf_blend_en_y,			pp->sf_blend_en_y,\
													sf_blend_en_c,			pp->sf_blend_en_c,\
													sf_blend_motion_exp_mode,	pp->sf_blend_motion_exp_mode,\
													sad_sel_pels,			pp->sad_sel_pels);
				/* mc */
				PE_P0R_M14A0_QRd01(p0r_tnr_ctrl_21,	sad_mamc_blend,		pp->sad_mamc_blend);
				PE_P0R_M14A0_QRd01(p0r_tnr_ctrl_00,	mamc_blend_sel,		pp->mamc_blend_sel);
				PE_P0R_M14A0_QRd04(p0r_tnr_ctrl_46,	mamc_blend_x0,		pp->mamc_blend_x0,\
													mamc_blend_x1,		pp->mamc_blend_x1,\
													mamc_blend_y0,		pp->mamc_blend_y0,\
													mamc_blend_y1,		pp->mamc_blend_y1);
				/* me */
				PE_P0R_M14A0_QRd03(p0r_tnr_ctrl_36,	hme_half_pel_en,		pp->hme_half_pel_en,\
													mv_cost_smooth_gain,	pp->mv_cost_smooth_gain,\
													mv_cost_smooth_en,		pp->mv_cost_smooth_en);
				PE_P0R_M14A0_QRd02(p0r_tnr_ctrl_36,	mv0_protect_th,			pp->mv0_protect_th,\
													mv0_protect_en,			pp->mv0_protect_en);
				PE_P0R_M14A0_QRd04(p0r_tnr_ctrl_38,	mv_cost_gmv_smooth_gain,pp->mv_cost_gmv_smooth_gain,\
													mv_cost_gmv_smooth_en,	pp->mv_cost_gmv_smooth_en,\
													sad_protect_en,			pp->sad_protect_en,\
													sad_protect_gm_en,		pp->sad_protect_gm_en);
				PE_P0R_M14A0_QRd04(p0r_tnr_ctrl_22,	mv_protect_control_x0,	pp->mv_protect_control_x0,\
													mv_protect_control_x1,	pp->mv_protect_control_x1,\
													mv_protect_control_y0,	pp->mv_protect_control_y0,\
													mv_protect_control_y1,	pp->mv_protect_control_y1);
			}
			PE_NRD_HW_M14_DBG_PRINT("get[%d] \n"\
				"ma:\nmotion: 	 coring_en (y,c): %d,%d, gain (y,c): %d,%d,\n"\
					 "         	 coring_th: %d, offset_en: %d, gain_en_var (m,a): %d,%d\n"\
					 "         	 wa_y(0,1,2,3):%d,%d,%d,%d, wa_x(0,1,2,3):%d,%d,%d,%d\n"\
					 "alph_gain: gain_x(p0,p1,p2,p3):%d,%d,%d,%d, gain_y(p0,p1,p2,p3):%d,%d,%d,%d\n"\
					 "alph_remap:ave_en:%d, mapping_y_en:%d, mapping_c_en:%d\n"\
					 "           map_y(x0,x1,x2,x3):%d,%d,%d,%d, map_y(y0,y1,y2,y3):%d,%d,%d,%d\n"\
					 "           map_c(x0,x1,x2,x3):%d,%d,%d,%d, map_c(y0,y1,y2,y3):%d,%d,%d,%d\n"\
				     "adj_gain:  gm_en:%d, lpct:%d, avg_ts_en:%d, lpct_sel:%d\n"\
				     "           gm(x0,x1,x2,x3):%d,%d,%d,%d, gm(y0,y1,y2,y3):%d,%d,%d,%d\n"\
				     "           st(x0,x1,x2,x3):%d,%d,%d,%d, st(y0,y1,y2,y3):%d,%d,%d,%d\n"\
				     "           lpct(x0,x1,x2,x3):%d,%d,%d,%d, lpct(y0,y1,y2,y3):%d,%d,%d,%d\n"\
				     "           m_gain_en_var2:%d,\n"\
				     "           ctrl(x0,x1,y0,y1):%d,%d,%d,%d\n"\
				     "           sf_map_flt_en:%d, map_tap:%d, map_gain:%d\n"\
				     "           sf_th(0,1,2,3,4):%d,%d,%d,%d,%d\n"\
				     "           sf_mb(x0,x1,x2,x3):%d,%d,%d,%d, sf_mb(y0,y1,y2,y3):%d,%d,%d,%d\n"\
				     "           sf_gain_md:%d, sf_gain:%d, sf_en(y,c):%d,%d, sf_mo_exp_md:%d\n"\
				"mc:\n           sel_pels:%d, mamc_blend:%d, blend_sel:%d\n"\
				     "           blend(x0,x1,y0,y1):%d,%d,%d,%d\n"\
				"me:\n           hlf_pel_en:%d, smth(gain,en):%d,%d, gmv_smth(gain,en):%d,%d\n"\
				     "           mv0_th:%d, mv0_en:%d, sad_en:%d, sad_gm_en:%d\n"\
				     "           ctrl(x0,x1,y0,y1):%d,%d,%d,%d\n",\
				pp->win_id,\
				// ma: motion
				pp->m_coring_en_y,pp->m_coring_en_c,pp->m_gain_y,pp->m_gain_c,\
				pp->m_coring_th,pp->m_offset_mode_en,pp->m_gain_en_var,pp->a_gain_en_var,\
				pp->wa_y0,pp->wa_y1,pp->wa_y2,pp->wa_y3,pp->wa_x0,pp->wa_x1,pp->wa_x2,pp->wa_x3,\
				// ma: alpha gain
				pp->luma_gain_p0_x,pp->luma_gain_p1_x,pp->luma_gain_p2_x,pp->luma_gain_p3_x,\
				pp->luma_gain_p0_y,pp->luma_gain_p1_y,pp->luma_gain_p2_y,pp->luma_gain_p3_y,\
				// alpha remapping
				pp->alpha_avg_en,pp->alpha_mapping_y_en,pp->alpha_mapping_c_en,\
				pp->alpha_mapping_y_x0,pp->alpha_mapping_y_x1,pp->alpha_mapping_y_x2,pp->alpha_mapping_y_x3,\
				pp->alpha_mapping_y_y0,pp->alpha_mapping_y_y1,pp->alpha_mapping_y_y2,pp->alpha_mapping_y_y3,\
				pp->alpha_mapping_c_x0,pp->alpha_mapping_c_x1,pp->alpha_mapping_c_x2,pp->alpha_mapping_c_x3,\
				pp->alpha_mapping_c_y0,pp->alpha_mapping_c_y1,pp->alpha_mapping_c_y2,pp->alpha_mapping_c_y3,\
				// adjust gain
				pp->nt_lvl_adjust_gm_enable,pp->nt_lvl_adjust_lpct_enable,\
				pp->nt_lvl_adjust_avg_ts_enable,pp->nt_lvl_adjust_lpct_sel,\
				pp->nt_lvl_gain_gm_x0,pp->nt_lvl_gain_gm_x1,pp->nt_lvl_gain_gm_x2,pp->nt_lvl_gain_gm_x3,\
				pp->nt_lvl_gain_gm_y0,pp->nt_lvl_gain_gm_y1,pp->nt_lvl_gain_gm_y2,pp->nt_lvl_gain_gm_y3,\
				pp->nt_lvl_gain_st_x0,pp->nt_lvl_gain_st_x1,pp->nt_lvl_gain_st_x2,pp->nt_lvl_gain_st_x3,\
				pp->nt_lvl_gain_st_y0,pp->nt_lvl_gain_st_y1,pp->nt_lvl_gain_st_y2,pp->nt_lvl_gain_st_y3,\
				pp->nt_lvl_gain_lpct_x0,pp->nt_lvl_gain_lpct_x1,pp->nt_lvl_gain_lpct_x2,pp->nt_lvl_gain_lpct_x3,\
				pp->nt_lvl_gain_lpct_y0,pp->nt_lvl_gain_lpct_y1,pp->nt_lvl_gain_lpct_y2,pp->nt_lvl_gain_lpct_y3,\
				pp->m_gain_en_var2,\
				pp->m_gain_ctrl2_x0,pp->m_gain_ctrl2_x1,pp->m_gain_ctrl2_y0,pp->m_gain_ctrl2_y1,\
				pp->sf_map_flt_en,pp->sf_map_tap,pp->sf_map_gain,\
				pp->sf_th0,pp->sf_th1,pp->sf_th2,pp->sf_th3,pp->sf_th4,\
				pp->sf_mb_x0,pp->sf_mb_x1,pp->sf_mb_x2,pp->sf_mb_x3,\
				pp->sf_mb_y0,pp->sf_mb_y1,pp->sf_mb_y2,pp->sf_mb_y3,\
				pp->sf_gain_mode,pp->sf_gain,pp->sf_blend_en_y,pp->sf_blend_en_c,pp->sf_blend_motion_exp_mode,\
				// mc:
				pp->sad_sel_pels,pp->sad_mamc_blend,pp->mamc_blend_sel,\
				pp->mamc_blend_x0,pp->mamc_blend_x1,pp->mamc_blend_y0,pp->mamc_blend_y1,\
				// me:
				pp->hme_half_pel_en,pp->mv_cost_smooth_gain,pp->mv_cost_smooth_en,\
				pp->mv_cost_gmv_smooth_gain,pp->mv_cost_gmv_smooth_en,\
				pp->mv0_protect_th,pp->mv0_protect_en,pp->sad_protect_en,pp->sad_protect_gm_en,\
				pp->mv_protect_control_x0,pp->mv_protect_control_x1,\
				pp->mv_protect_control_y0,pp->mv_protect_control_y1);

		}
		else
		{
			PE_NRD_HW_M14_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * set tnr gain(m14b only)
 *
 * @param   *pCfg [in] PE_TSK_M14_CFG_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_NRD_HW_M14_SetTnrGain(PE_TSK_M14_CFG_T *pCfg)
{
	int ret = RET_OK;
	#ifdef PE_HW_M14B_USE_REG_CTRL
	static UINT32 count = 0;
	static UINT32 pre_sat_status = 0;
	static UINT32 pre_is_pattern = 0;
	UINT32 motion_level = 999;
	UINT32 cur_is_pattern = 9, cur_is_still = 9;
	UINT32 cur_gain, apply_high_gain;
	PE_INF_M14_CTRL_T *ctrl0_info = &pCfg->inf.ctrl_info[LX_PE_WIN_0];
	LX_PE_INF_DISPLAY_T *disp0_info = &pCfg->inf.disp_info[LX_PE_WIN_0];
	PE_NRD_HW_M14_SETTINGS_T *pInfo = &_g_pe_nrd_hw_m14_info;
	PE_TSK_M14_OPR_T *p_opr = &(pCfg->opr);
	do{
		CHECK_KNULL(pCfg);
		if(PE_KDRV_VER_M14BX)
		{
			/************************************/
			/* check high tnr gain case
			*	- not_OVWR(for debug, see PE_TSK_M14_RUN_STOP_OVWR(3)) and
			*		ATV & PAL & mute_off & tnr_auto & NOT MOVING & PATTERN */
			if (p_opr->pause_task != 3 && \
				disp0_info->src_type==LX_PE_SRC_ATV && \
				disp0_info->cstd_type==LX_PE_CSTD_PAL && \
				ctrl0_info->mute_on==0 && pInfo->cur_tnr_auto_enable==1)
			{
				/* check motion */
				PE_P0M_M14B0_RdFL(p0m_tnr_status_02);	// motion status
				PE_P0M_M14B0_Rd01(p0m_tnr_status_02,  ma_motion,  motion_level);
				cur_is_still = (motion_level<PE_NRD_HW_M14_MA_MOTION_ATV_TH)? 1:0;
				/* check pattern */
				if (pre_sat_status < ctrl0_info->sat_status)	//increase
				{
					/* if higher than hi_th, it's pattern(1), else keep pre condition */
					cur_is_pattern = (ctrl0_info->sat_status>=PE_NRD_HW_M14_SAT_STATUS_ATV_HIGH_TH)? \
									0x1:pre_is_pattern;
				}
				else	//decrease
				{
					/* if lower than lo_th, it's not pattern(0), else keep pre condition */
					cur_is_pattern = (ctrl0_info->sat_status<PE_NRD_HW_M14_SAT_STATUS_ATV_LOW_TH)? \
									0x0:pre_is_pattern;
				}
				apply_high_gain = ((cur_is_still==1) && (cur_is_pattern==1))?
					PE_NRD_HW_M14_APPLY_TNR_HIGH_GAIN__ON:PE_NRD_HW_M14_APPLY_TNR_HIGH_GAIN_OFF;
				pre_sat_status = ctrl0_info->sat_status;
				pre_is_pattern = cur_is_pattern;
			}
			else
			{
				apply_high_gain = PE_NRD_HW_M14_APPLY_TNR_HIGH_GAIN_OFF;
				pre_sat_status = 0;
				pre_is_pattern = 0;
			}
			/* if high tnr gain case, use high gain(0xc0) */
			if (apply_high_gain==PE_NRD_HW_M14_APPLY_TNR_HIGH_GAIN__ON)
			{
				cur_gain = PE_NRD_HW_M14_TNR_HIGH_GAIN;
			}
			else
			{
				cur_gain = pInfo->cur_tnr_master_gain;
			}
			/* compare pre vs. cur setting */
			if (pInfo->pre_tnr_master_gain != cur_gain)
			{
				PE_NRD_HW_M14_DBG_PRINT("[inf]opr:%d, src,cstd,mute:%d,%d,%d, auto:%d\n", \
					p_opr->pause_task, disp0_info->src_type, disp0_info->cstd_type, \
					ctrl0_info->mute_on, pInfo->cur_tnr_auto_enable);
				PE_NRD_HW_M14_DBG_PRINT("[inf]motion:%3d(th:7), is_still:%d\n", \
					motion_level, cur_is_still);
				PE_NRD_HW_M14_DBG_PRINT("[inf]sat:%4d->%4d(th:194~200), is_pattern:%d->%d\n", \
					pre_sat_status, ctrl0_info->sat_status, pre_is_pattern, cur_is_pattern);
				PE_NRD_HW_M14_DBG_PRINT("[set][algo:%d]gain(0x%02x ->): 0x%02x -> 0x%02x\n", \
					apply_high_gain, pInfo->cur_tnr_master_gain, pInfo->pre_tnr_master_gain, cur_gain);
				/* set reg. and udpate mask */
				PE_NRD_HW_M14B_P0M_WR01(p0m_tnr_ctrl_00, tnr_master_gain, GET_BITS(cur_gain,0,8));
				/* update pre setting */
				pInfo->pre_tnr_master_gain = cur_gain;
			}
			/************************************/
			if(_g_nrd_hw_m14_trace)
			{
				if(count>PE_PRINT_COUNT_NUM)
				{
					PE_NRD_HW_M14_DBG_PRINT("[state]opr:%d, src,cstd,mute:%d,%d,%d, auto:%d\n", \
						p_opr->pause_task, disp0_info->src_type, disp0_info->cstd_type, \
						ctrl0_info->mute_on, pInfo->cur_tnr_auto_enable);
					PE_NRD_HW_M14_DBG_PRINT("[state]motion:%3d(th:7), is_still:%d\n", \
						motion_level, cur_is_still);
					PE_NRD_HW_M14_DBG_PRINT("[state]sat:%4d->%4d(th:194~200), is_pattern:%d->%d\n", \
						pre_sat_status, ctrl0_info->sat_status, pre_is_pattern, cur_is_pattern);
					PE_NRD_HW_M14_DBG_PRINT("[state][algo:%d]gain(0x%02x ->): 0x%02x\n", \
						apply_high_gain, pInfo->cur_tnr_master_gain, cur_gain);
					count=0;
				}
				count++;
			}
		}
		else
		{
			PE_NRD_HW_M14_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	#else
	PE_NRD_HW_M14_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
	#endif
	return ret;
}
/**
 * set dnr mode
 *
 * @param   *pCfg [in] PE_TSK_M14_CFG_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_NRD_HW_M14_SetDnrMode(PE_TSK_M14_CFG_T *pCfg)
{
	int ret = RET_OK;
	static UINT32 count = 0;
	static UINT32 pre_bnr_ac_h_en0 = 0xff;		//pre bnr_ac_h_en0
	static UINT32 pre_bnr_ac_h_en1 = 0xff;		//pre bnr_ac_h_en1
	UINT32 cur_bnr_ac_h_en0 = 0;
	UINT32 cur_bnr_ac_h_en1 = 0;
	PE_INF_M14_CTRL_T *ctrl0_info = &pCfg->inf.ctrl_info[LX_PE_WIN_0];
	PE_INF_M14_CTRL_T *ctrl1_info = &pCfg->inf.ctrl_info[LX_PE_WIN_1];
	LX_PE_INF_DISPLAY_T *disp0_info = &pCfg->inf.disp_info[LX_PE_WIN_0];
	LX_PE_INF_DISPLAY_T *disp1_info = &pCfg->inf.disp_info[LX_PE_WIN_1];
	PE_NRD_HW_M14_SETTINGS_T *pInfo = &_g_pe_nrd_hw_m14_info;
	do{
		CHECK_KNULL(pCfg);
		if(PE_KDRV_VER_M14BX)
		{
			cur_bnr_ac_h_en0 = (disp0_info->src_type == LX_PE_SRC_DTV && \
				disp0_info->dtv_type == LX_PE_DTV_FILEPLAY && \
				ctrl0_info->pe0_src_h <= PE_NRD_HW_M14_H_MIN && \
				ctrl0_info->pe0_src_v <= PE_NRD_HW_M14_V_MIN)? \
				0x0:pInfo->reg_bnr_ac_h_en0;
			/* compare pre vs. cur bnr_ac_h_en */
			if (pre_bnr_ac_h_en0 != cur_bnr_ac_h_en0)
			{
				PE_NRD_HW_M14_DBG_PRINT("[inf0] src:%d,dtv:%d, h,v:%d,%d, en:%d\n",\
					disp0_info->src_type, disp0_info->dtv_type, \
					ctrl0_info->pe0_src_h, ctrl0_info->pe0_src_v, \
					pInfo->reg_bnr_ac_h_en0);
				PE_NRD_HW_M14_DBG_PRINT("[set0] bnr_ac_h_en : %d -> %d\n",\
					pre_bnr_ac_h_en0, cur_bnr_ac_h_en0);
				#ifdef PE_HW_M14B_USE_REG_CTRL
				PE_NRD_HW_M14B_P0D_WR01(p0d_ac_bnr_ctrl_0,reg_bnr_ac_h_en,cur_bnr_ac_h_en0);
				#else
				PE_P0D_M14B0_QWr01(p0d_ac_bnr_ctrl_0,reg_bnr_ac_h_en,cur_bnr_ac_h_en0);
				#endif
				pre_bnr_ac_h_en0 = cur_bnr_ac_h_en0;
			}
			if(_g_nrd_hw_m14_trace)
			{
				if(count>PE_PRINT_COUNT_NUM)
				{
					PE_NRD_HW_M14_DBG_PRINT("[state0] src:%d,dtv:%d, "\
						"pe0 h,v:%d,%d, en:%d, bnr_ac_h_en:%d\n", \
						disp0_info->src_type, disp0_info->dtv_type, \
						ctrl0_info->pe0_src_h, ctrl0_info->pe0_src_v, \
						pInfo->reg_bnr_ac_h_en0, cur_bnr_ac_h_en0);
					count=0;
				}
				count++;
			}
		}
		else if(PE_KDRV_VER_M14AX)
		{
			/* win 0 */
			cur_bnr_ac_h_en0 = (disp0_info->src_type == LX_PE_SRC_DTV && \
				disp0_info->dtv_type == LX_PE_DTV_FILEPLAY && \
				ctrl0_info->pe0_src_h <= PE_NRD_HW_M14_H_MIN && \
				ctrl0_info->pe0_src_v <= PE_NRD_HW_M14_V_MIN)? \
				0x0:pInfo->reg_bnr_ac_h_en0;
			/* compare pre vs. cur bnr_ac_h_en */
			if (pre_bnr_ac_h_en0 != cur_bnr_ac_h_en0)
			{
				PE_NRD_HW_M14_DBG_PRINT("[inf0] src:%d,dtv:%d, h,v:%d,%d, en:%d\n",\
					disp0_info->src_type, disp0_info->dtv_type, \
					ctrl0_info->pe0_src_h, ctrl0_info->pe0_src_v, \
					pInfo->reg_bnr_ac_h_en0);
				PE_NRD_HW_M14_DBG_PRINT("[set0] bnr_ac_h_en : %d -> %d\n",\
					pre_bnr_ac_h_en0, cur_bnr_ac_h_en0);
				PE_P0L_M14A0_QWr01(p0l_bnr_ac_ctrl_00,reg_bnr_ac_h_en,cur_bnr_ac_h_en0);
				pre_bnr_ac_h_en0 = cur_bnr_ac_h_en0;
			}
			/* win 1 */
			cur_bnr_ac_h_en1 = (disp1_info->src_type == LX_PE_SRC_DTV && \
				disp1_info->dtv_type == LX_PE_DTV_FILEPLAY && \
				ctrl1_info->pe0_src_h <= PE_NRD_HW_M14_H_MIN && \
				ctrl1_info->pe0_src_v <= PE_NRD_HW_M14_V_MIN)? \
				0x0:pInfo->reg_bnr_ac_h_en1;
			/* compare pre vs. cur bnr_ac_h_en */
			if (pre_bnr_ac_h_en1 != cur_bnr_ac_h_en1)
			{
				PE_NRD_HW_M14_DBG_PRINT("[inf1] src:%d,dtv:%d, h,v:%d,%d, en:%d\n",\
					disp1_info->src_type, disp1_info->dtv_type, \
					ctrl1_info->pe0_src_h, ctrl1_info->pe0_src_v, \
					pInfo->reg_bnr_ac_h_en1);
				PE_NRD_HW_M14_DBG_PRINT("[set1] bnr_ac_h_en : %d -> %d\n",\
					pre_bnr_ac_h_en1, cur_bnr_ac_h_en1);
				PE_P0R_M14A0_QWr01(p0r_bnr_ac_ctrl_00,reg_bnr_ac_h_en,cur_bnr_ac_h_en1);
				pre_bnr_ac_h_en1 = cur_bnr_ac_h_en1;
			}
			if(_g_nrd_hw_m14_trace)
			{
				if(count>PE_PRINT_COUNT_NUM)
				{
					PE_NRD_HW_M14_DBG_PRINT("[state0] src:%d,dtv:%d, "\
						"pe0 h,v:%d,%d, en:%d, bnr_ac_h_en:%d\n", \
						disp0_info->src_type, disp0_info->dtv_type, \
						ctrl0_info->pe0_src_h, ctrl0_info->pe0_src_v, \
						pInfo->reg_bnr_ac_h_en0, cur_bnr_ac_h_en0);
					PE_NRD_HW_M14_DBG_PRINT("[state1] src:%d,dtv:%d, "\
						"pe0 h,v:%d,%d, en:%d, bnr_ac_h_en:%d\n", \
						disp1_info->src_type, disp1_info->dtv_type, \
						ctrl1_info->pe0_src_h, ctrl1_info->pe0_src_v, \
						pInfo->reg_bnr_ac_h_en1, cur_bnr_ac_h_en1);
					count=0;
				}
				count++;
			}
		}
		else
		{
			PE_NRD_HW_M14_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}

