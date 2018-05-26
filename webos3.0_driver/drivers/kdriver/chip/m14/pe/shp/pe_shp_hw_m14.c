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

/** @file pe_shp_hw_m14.c
 *
 *  driver for picture enhance sharpness functions. ( used only within kdriver )
 *
 *	@author		Seung-Jun,Youm(sj.youm@lge.com)
 *	@version	0.1
 *	@note
 *	@date		2011.04.12
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
#include "pe_shp_hw_def.h"
#include "pe_shp_hw_m14.h"
#include "pe_shp_hw_m14a0.h"
#include "pe_shp_hw_m14b0.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
#define PE_SHP_HW_M14_FHD_H_MAX			1920
#define PE_SHP_HW_M14_FHD_V_MAX			1080
#define PE_SHP_HW_M14_SCL_IN_H_MIN		160
#define PE_SHP_HW_M14_SCL_IN_V_MIN		120
#define PE_SHP_HW_M14_IDX_Y_DFLT		15
#define PE_SHP_HW_M14_IDX_C_DFLT		39

/* h,v only(define) vs. both(undef), Now we use both mode until getting c08t scl has no problem. */
#define PE_SHP_HW_M14_USE_V12TM_C08T_SEPARATE_HV

/* use MSR vs. SSC on venc mode, We are going to use SSC from M14A0. */
#undef PE_SHP_HW_M14_USE_MSR_FOR_VENC_ON

/* use define until shp f/w ready, M14B0 */
#undef PE_SHP_HW_M14B_SET_FW_PARAM_TO_REG_DIRECTLY

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define PE_SHP_HW_M14_ERROR		PE_PRINT_ERROR

#define PE_SHP_HW_M14_DBG_PRINT(fmt, args...)	\
	if(_g_shp_hw_m14_trace)\
	{\
		printk("[%x,%x][%s,%d] "fmt, \
			PE_CHIP_VER, g_pe_kdrv_ver_mask, __F__, __L__, ##args);\
	}

#define PE_SHP_HW_M14_CHECK_CODE(_checker, _action, fmt, args...)	\
	{if(_checker){PE_SHP_HW_M14_ERROR(fmt, ##args);_action;}}

#define PE_SHP_HW_M14_DBG_CHECK_CODE(_cnt, _checker, _action, fmt, args...)	\
	{\
		if(_checker){\
			if(_g_shp_hw_m14_trace&&_cnt>PE_PRINT_COUNT_NUM)\
			{\
				PE_SHP_HW_M14_DBG_PRINT(fmt, ##args);\
			}\
			_action;\
		}\
	}

#define PE_SHP_HW_M14B_DBG_PRINT_RES_FMT_STATE(_pre, _cur)	\
	{\
		char _str_buf[PE_TRACE_STR_SIZE];\
		sprintf(_str_buf, "%s(%s)->(%s):%s", #_cur, \
			PE_SHP_HW_M14B_RES_FMT_TO_STR(_pre), \
			PE_SHP_HW_M14B_RES_FMT_TO_STR(_cur), \
			((_pre)!=(_cur))? "different":"same");\
		PE_SHP_HW_M14_DBG_PRINT("%s\n", _str_buf);\
		if((_pre)!=(_cur))\
		{\
			PE_INF_M14_SetTraceDataBuf(_str_buf);\
		}\
	}
#define PE_SHP_HW_M14B_DBG_PRINT_SRE_FMT_STATE(_pre, _cur)	\
	{\
		char _str_buf[PE_TRACE_STR_SIZE];\
		sprintf(_str_buf, "%s(%s)->(%s):%s", #_cur, \
			PE_SHP_HW_M14B_SRE_FMT_TO_STR(_pre), \
			PE_SHP_HW_M14B_SRE_FMT_TO_STR(_cur), \
			((_pre)!=(_cur))? "different":"same");\
		PE_SHP_HW_M14_DBG_PRINT("%s\n", _str_buf);\
		if((_pre)!=(_cur))\
		{\
			PE_INF_M14_SetTraceDataBuf(_str_buf);\
		}\
	}
#define PE_SHP_HW_M14B_DBG_PRINT_SRL_FMT_STATE(_pre, _cur)	\
	{\
		char _str_buf[PE_TRACE_STR_SIZE];\
		sprintf(_str_buf, "%s(%s)->(%s):%s", #_cur, \
			PE_SHP_HW_M14B_SRL_FMT_TO_STR(_pre), \
			PE_SHP_HW_M14B_SRL_FMT_TO_STR(_cur), \
			((_pre)!=(_cur))? "different":"same");\
		PE_SHP_HW_M14_DBG_PRINT("%s\n", _str_buf);\
		if((_pre)!=(_cur))\
		{\
			PE_INF_M14_SetTraceDataBuf(_str_buf);\
		}\
	}
#define PE_SHP_HW_M14B_DBG_PRINT_VFC_FMT_STATE(_pre, _cur)	\
	{\
		char _str_buf[PE_TRACE_STR_SIZE];\
		sprintf(_str_buf, "%s(%s)->(%s):%s", #_cur, \
			PE_SHP_HW_M14B_VFC_FMT_TO_STR(_pre), \
			PE_SHP_HW_M14B_VFC_FMT_TO_STR(_cur), \
			((_pre)!=(_cur))? "different":"same");\
		PE_SHP_HW_M14_DBG_PRINT("%s\n", _str_buf);\
		if((_pre)!=(_cur))\
		{\
			PE_INF_M14_SetTraceDataBuf(_str_buf);\
		}\
	}
#define PE_SHP_HW_M14B_DBG_PRINT_CTI_FMT_STATE(_pre, _cur)	\
	{\
		char _str_buf[PE_TRACE_STR_SIZE];\
		sprintf(_str_buf, "%s(%s)->(%s):%s", #_cur, \
			PE_SHP_HW_M14B_CTI_FMT_TO_STR(_pre), \
			PE_SHP_HW_M14B_CTI_FMT_TO_STR(_cur), \
			((_pre)!=(_cur))? "different":"same");\
		PE_SHP_HW_M14_DBG_PRINT("%s\n", _str_buf);\
		if((_pre)!=(_cur))\
		{\
			PE_INF_M14_SetTraceDataBuf(_str_buf);\
		}\
	}

#define PE_SHP_HW_M14A_DBG_PRINT_RES_FMT_STATE(_pre, _cur)	\
	{\
		char _str_buf[PE_TRACE_STR_SIZE];\
		sprintf(_str_buf, "%s(%s)->(%s):%s", #_cur, \
			PE_SHP_HW_M14A_RES_FMT_TO_STR(_pre), \
			PE_SHP_HW_M14A_RES_FMT_TO_STR(_cur), \
			((_pre)!=(_cur))? "different":"same");\
		PE_SHP_HW_M14_DBG_PRINT("%s\n", _str_buf);\
		if((_pre)!=(_cur))\
		{\
			PE_INF_M14_SetTraceDataBuf(_str_buf);\
		}\
	}
#define PE_SHP_HW_M14A_DBG_PRINT_VFC_FMT_STATE(_pre, _cur)	\
	{\
		char _str_buf[PE_TRACE_STR_SIZE];\
		sprintf(_str_buf, "%s(%s)->(%s):%s", #_cur, \
			PE_SHP_HW_M14A_VFC_FMT_TO_STR(_pre), \
			PE_SHP_HW_M14A_VFC_FMT_TO_STR(_cur), \
			((_pre)!=(_cur))? "different":"same");\
		PE_SHP_HW_M14_DBG_PRINT("%s\n", _str_buf);\
		if((_pre)!=(_cur))\
		{\
			PE_INF_M14_SetTraceDataBuf(_str_buf);\
		}\
	}

#define PE_SHP_HW_M14_WR_SCL_COEFF(_wid, _ctrl, _coeff, _id)	\
{\
	char _str_buf[PE_TRACE_STR_SIZE];\
	sprintf(_str_buf, "[wid:%d][idx:%3d]%s", _wid, (_id), #_coeff);\
	PE_SHP_HW_M14_DBG_PRINT("set %s\n", _str_buf);\
	PE_INF_M14_SetTraceDataBuf(_str_buf);\
	PE_SHP_HW_M14_WrSclCoeff((_ctrl),&(_coeff));\
}

#ifdef PE_HW_M14B_USE_REG_CTRL
#define PE_SHP_HW_M14B_PE1_ENUM(_x)		PE_SHP_HW_M14B_PE1_ENUM_##_x
#define PE_SHP_HW_M14B_MSC_ENUM(_x)		PE_SHP_HW_M14B_MSC_ENUM_##_x

#define PE_SHP_HW_M14B_PE1_SET(_r, _f, _d)	\
	{	_g_pe_shp_hw_m14b_pe1_reg_ctrl.data._r._f = (_d);\
		_g_pe_shp_hw_m14b_pe1_reg_ctrl.mask._r._f = 0;}
#define PE_SHP_HW_M14B_MSC_SET(_r, _f, _d)	\
	{	_g_pe_shp_hw_m14b_msc_reg_ctrl.data._r._f = (_d);\
		_g_pe_shp_hw_m14b_msc_reg_ctrl.mask._r._f = 0;}

#define PE_SHP_HW_M14B_PE1_WR01(_r, _f1, _d1)	\
	{	PE_PE1_M14B0_QWr01(_r, _f1, _d1);\
		PE_SHP_HW_M14B_PE1_SET(_r, _f1, _d1);}
#define PE_SHP_HW_M14B_PE1_WR02(_r, _f1, _d1, _f2, _d2)	\
	{	PE_PE1_M14B0_QWr02(_r, _f1, _d1, _f2, _d2);\
		PE_SHP_HW_M14B_PE1_SET(_r, _f1, _d1);\
		PE_SHP_HW_M14B_PE1_SET(_r, _f2, _d2);}
#define PE_SHP_HW_M14B_PE1_WR03(_r, _f1, _d1, _f2, _d2, _f3, _d3)	\
	{	PE_PE1_M14B0_QWr03(_r, _f1, _d1, _f2, _d2, _f3, _d3);\
		PE_SHP_HW_M14B_PE1_SET(_r, _f1, _d1);\
		PE_SHP_HW_M14B_PE1_SET(_r, _f2, _d2);\
		PE_SHP_HW_M14B_PE1_SET(_r, _f3, _d3);}
#define PE_SHP_HW_M14B_PE1_WR04(_r, _f1, _d1, _f2, _d2, _f3, _d3, _f4, _d4)	\
	{	PE_PE1_M14B0_QWr04(_r, _f1, _d1, _f2, _d2, _f3, _d3, _f4, _d4);\
		PE_SHP_HW_M14B_PE1_SET(_r, _f1, _d1);\
		PE_SHP_HW_M14B_PE1_SET(_r, _f2, _d2);\
		PE_SHP_HW_M14B_PE1_SET(_r, _f3, _d3);\
		PE_SHP_HW_M14B_PE1_SET(_r, _f4, _d4);}
#define PE_SHP_HW_M14B_PE1_WR05(_r, _f1, _d1, _f2, _d2, _f3, _d3, _f4, _d4, _f5, _d5)	\
	{	PE_PE1_M14B0_QWr05(_r, _f1, _d1, _f2, _d2, _f3, _d3, _f4, _d4, _f5, _d5);\
		PE_SHP_HW_M14B_PE1_SET(_r, _f1, _d1);\
		PE_SHP_HW_M14B_PE1_SET(_r, _f2, _d2);\
		PE_SHP_HW_M14B_PE1_SET(_r, _f3, _d3);\
		PE_SHP_HW_M14B_PE1_SET(_r, _f4, _d4);\
		PE_SHP_HW_M14B_PE1_SET(_r, _f5, _d5);}

#define PE_SHP_HW_M14B_MSC_WR01(_r, _f1, _d1)	\
	{	PE_MSC_M14B0_QWr01(_r, _f1, _d1);\
		PE_SHP_HW_M14B_MSC_SET(_r, _f1, _d1);}
#define PE_SHP_HW_M14B_MSC_WR02(_r, _f1, _d1, _f2, _d2)	\
	{	PE_MSC_M14B0_QWr02(_r, _f1, _d1, _f2, _d2);\
		PE_SHP_HW_M14B_MSC_SET(_r, _f1, _d1);\
		PE_SHP_HW_M14B_MSC_SET(_r, _f2, _d2);}
#define PE_SHP_HW_M14B_MSC_WR03(_r, _f1, _d1, _f2, _d2, _f3, _d3)	\
	{	PE_MSC_M14B0_QWr03(_r, _f1, _d1, _f2, _d2, _f3, _d3);\
		PE_SHP_HW_M14B_MSC_SET(_r, _f1, _d1);\
		PE_SHP_HW_M14B_MSC_SET(_r, _f2, _d2);\
		PE_SHP_HW_M14B_MSC_SET(_r, _f3, _d3);}

#define PE_SHP_HW_M14B_RES_REG_TABLE(_dflt, _base)	\
	do{\
		PE_REG_PARAM_T *p_##_dflt = (PE_REG_PARAM_T *)OS_Malloc(sizeof(_dflt));\
		PE_SHP_HW_M14_CHECK_CODE(!(p_##_dflt), break, \
			"[%s,%d] p_tbl is null.\n", __F__, __L__);\
		do{\
			ret = PE_SHP_HW_M14B_GetResDfltTable(p_##_dflt, _dflt, sizeof(_dflt));\
			PE_SHP_HW_M14_CHECK_CODE(ret, break, \
				"[%s,%d] PE_SHP_HW_M14B_GetResDfltTable() error.\n", __F__, __L__);\
			PE_SHP_HW_M14_DBG_PRINT("write %s(+ 0x%08x).\n", #_dflt, (_base));\
			ret = PE_REG_SetTraceTable(__F__,__L__, #_dflt, (_base));\
			ret = PE_REG_SetTable(p_##_dflt, (_base), sizeof(_dflt));\
			PE_SHP_HW_M14_CHECK_CODE(ret, break, \
				"[%s,%d] PE_REG_SetTable() error.\n", __F__, __L__);\
		}while (0);\
		if (p_##_dflt)	{OS_Free(p_##_dflt);}\
	}while (0)

#define PE_SHP_HW_M14B_SRE_REG_TABLE(_dflt, _base)	\
	do{\
		PE_REG_PARAM_T *p_##_dflt = (PE_REG_PARAM_T *)OS_Malloc(sizeof(_dflt));\
		PE_SHP_HW_M14_CHECK_CODE(!(p_##_dflt), break, \
			"[%s,%d] p_tbl is null.\n", __F__, __L__);\
		do{\
			ret = PE_SHP_HW_M14B_GetSreDfltTable(p_##_dflt, _dflt, sizeof(_dflt));\
			PE_SHP_HW_M14_CHECK_CODE(ret, break, \
				"[%s,%d] PE_SHP_HW_M14B_GetSreDfltTable() error.\n", __F__, __L__);\
			PE_SHP_HW_M14_DBG_PRINT("write %s(+ 0x%08x).\n", #_dflt, (_base));\
			ret = PE_REG_SetTraceTable(__F__,__L__, #_dflt, (_base));\
			ret = PE_REG_SetTable(p_##_dflt, (_base), sizeof(_dflt));\
			PE_SHP_HW_M14_CHECK_CODE(ret, break, \
				"[%s,%d] PE_REG_SetTable() error.\n", __F__, __L__);\
		}while (0);\
		if (p_##_dflt)	{OS_Free(p_##_dflt);}\
	}while (0)

#define PE_SHP_HW_M14B_VFC_REG_TABLE(_dflt, _base)	\
	do{\
		PE_REG_PARAM_T *p_##_dflt = (PE_REG_PARAM_T *)OS_Malloc(sizeof(_dflt));\
		PE_SHP_HW_M14_CHECK_CODE(!(p_##_dflt), break, \
			"[%s,%d] p_tbl is null.\n", __F__, __L__);\
		do{\
			ret = PE_SHP_HW_M14B_GetVfcDfltTable(p_##_dflt, _dflt, sizeof(_dflt));\
			PE_SHP_HW_M14_CHECK_CODE(ret, break, \
				"[%s,%d] PE_SHP_HW_M14B_GetVfcDfltTable() error.\n", __F__, __L__);\
			PE_SHP_HW_M14_DBG_PRINT("write %s(+ 0x%08x).\n", #_dflt, (_base));\
			ret = PE_REG_SetTraceTable(__F__,__L__, #_dflt, (_base));\
			ret = PE_REG_SetTable(p_##_dflt, (_base), sizeof(_dflt));\
			PE_SHP_HW_M14_CHECK_CODE(ret, break, \
				"[%s,%d] PE_REG_SetTable() error.\n", __F__, __L__);\
		}while (0);\
		if (p_##_dflt)	{OS_Free(p_##_dflt);}\
	}while (0)
#endif

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/
/**
* scaler wr tap type
*	MSC_CTRL_L 0x1100 [2]v_scaler_config
* - v12t mode '0' : Luminance H12tap/V12tap, Chrominance H08tap/V08tap (SD input stream)
* - v06t mode '1' : Luminance H08tap/V06tap, Chrominance H08tap/V04tap (HD and SD input stream both)
*/
typedef enum
{
	PE_SHP_HW_M14_SCL_WR_TAP_V12TM_Y_H_12T = 0,		///< v12tap mode and luma horizontal 12tap
	PE_SHP_HW_M14_SCL_WR_TAP_V12TM_Y_V_12T,			///< v12tap mode and luma vertical 12tap
	PE_SHP_HW_M14_SCL_WR_TAP_V12TM_C_H_08T,			///< v12tap mode and chroma horizontal 08tap
	PE_SHP_HW_M14_SCL_WR_TAP_V12TM_C_V_08T,			///< v12tap mode and chroma vertical 08tap
	PE_SHP_HW_M14_SCL_WR_TAP_V06TM_Y_H_08T,			///< v06tap mode and luma horizontal 08tap
	PE_SHP_HW_M14_SCL_WR_TAP_V06TM_Y_V_06T,			///< v06tap mode and luma vertical 06tap
	PE_SHP_HW_M14_SCL_WR_TAP_V06TM_C_H_08T,			///< v06tap mode and chroma horizontal 08tap
	PE_SHP_HW_M14_SCL_WR_TAP_V06TM_C_V_04T,			///< v06tap mode and chroma vertical 04tap
	PE_SHP_HW_M14_SCL_WR_TAP_NUM					///< max num
}
PE_SHP_HW_M14_SCL_WR_TAP;

#ifdef PE_HW_M14B_USE_REG_CTRL
/**
 *	pe register num
 */
typedef enum
{
	PE_SHP_HW_M14B_PE1_ENUM(PE_M14B0_PE1_DERH_CTRL_00_T) = 0,	///< [0]  0xc00243b0L
	PE_SHP_HW_M14B_PE1_ENUM(PE_M14B0_PE1_DERH_CTRL_05_T),		///< [1]  0xc00243c4L
	PE_SHP_HW_M14B_PE1_ENUM(PE_M14B0_PE1_DERH_CTRL_06_T),		///< [2]  0xc00243c8L
	PE_SHP_HW_M14B_PE1_ENUM(PE_M14B0_PE1_DERH_CTRL_07_T),		///< [3]  0xc00243ccL
	PE_SHP_HW_M14B_PE1_ENUM(PE_M14B0_PE1_DERH_CTRL_0B_T),		///< [4]  0xc00243dcL
	PE_SHP_HW_M14B_PE1_ENUM(PE_M14B0_PE1_DERH_CTRL_0C_T),		///< [5]  0xc00243e0L
	PE_SHP_HW_M14B_PE1_ENUM(PE_M14B0_PE1_DERV_CTRL_0_T),		///< [6]  0xc00243f0L
	PE_SHP_HW_M14B_PE1_ENUM(PE_M14B0_PE1_DERV_CTRL_1_T),		///< [7]  0xc00243f4L
	PE_SHP_HW_M14B_PE1_ENUM(PE_M14B0_PE1_DERV_CTRL_2_T),		///< [8]  0xc00243f8L
	PE_SHP_HW_M14B_PE1_ENUM(PE_M14B0_PE1_DERV_CTRL_3_T),		///< [9]  0xc00243fcL
	PE_SHP_HW_M14B_PE1_ENUM(PE_M14B0_PE1_DP_CTRL_01_T),			///< [10] 0xc0024414L
	PE_SHP_HW_M14B_PE1_ENUM(PE_M14B0_PE1_SP_CTRL_01_T),			///< [11] 0xc0024434L
	PE_SHP_HW_M14B_PE1_ENUM(PE_M14B0_PE1_SP_CTRL_02_T),			///< [12] 0xc0024438L
	PE_SHP_HW_M14B_PE1_ENUM(PE_M14B0_PE1_SP_CTRL_06_T),			///< [13] 0xc0024448L
	PE_SHP_HW_M14B_PE1_ENUM(PE_M14B0_PE1_MP_CTRL_01_T),			///< [14] 0xc0024474L
	PE_SHP_HW_M14B_PE1_ENUM(PE_M14B0_PE1_MP_CTRL_02_T),			///< [15] 0xc0024478L
	PE_SHP_HW_M14B_PE1_ENUM(PE_M14B0_PE1_MP_CTRL_06_T),			///< [16] 0xc0024488L
	PE_SHP_HW_M14B_PE1_ENUM(PE_M14B0_PE1_CORING_CTRL_00_T),		///< [17] 0xc00244b0L
	PE_SHP_HW_M14B_PE1_ENUM(PE_M14B0_PE1_CORING_CTRL_01_T),		///< [18] 0xc00244b4L
	PE_SHP_HW_M14B_PE1_ENUM(PE_M14B0_PE1_CORING_CTRL_02_T),		///< [19] 0xc00244b8L
	PE_SHP_HW_M14B_PE1_ENUM(PE_M14B0_PE1_CORING_CTRL_04_T),		///< [20] 0xc00244c0L
	PE_SHP_HW_M14B_PE1_ENUM(PE_M14B0_PE1_CORING_CTRL_05_T),		///< [21] 0xc00244c4L
	PE_SHP_HW_M14B_PE1_ENUM(PE_M14B0_PE1_CORING_CTRL_06_T),		///< [22] 0xc00244c8L
	PE_SHP_HW_M14B_PE1_ENUM(PE_M14B0_PE1_CORING_CTRL_07_T),		///< [23] 0xc00244ccL
	PE_SHP_HW_M14B_PE1_ENUM(PE_M14B0_PE1_CORING_CTRL_09_T),		///< [24] 0xc00244d4L
	PE_SHP_HW_M14B_PE1_ENUM(PE_M14B0_PE1_TI_CTRL_0_T),			///< [25] 0xc0024500L
	PE_SHP_HW_M14B_PE1_ENUM(PE_M14B0_PE1_TI_CTRL_1_T),			///< [26] 0xc0024504L
	PE_SHP_HW_M14B_PE1_ENUM(PE_M14B0_PE1_TI_CTRL_2_T),			///< [27] 0xc0024508L
	PE_SHP_HW_M14B_PE1_ENUM(PE_M14B0_PE1_DJ_CTRL_00_T),			///< [28] 0xc0024520L
	PE_SHP_HW_M14B_PE1_ENUM(PE_M14B0_PE1_DJ_CTRL_01_T),			///< [29] 0xc0024524L
	PE_SHP_HW_M14B_PE1_ENUM(PE_M14B0_PE1_DJ_CTRL_02_T),			///< [30] 0xc0024528L
	PE_SHP_HW_M14B_PE1_ENUM(PE_M14B0_PE1_DJ_CTRL_03_T),			///< [31] 0xc002452cL
	PE_SHP_HW_M14B_PE1_ENUM(PE_M14B0_PE1_CTI_CTRL_00_T),		///< [32] 0xc0024530L
	PE_SHP_HW_M14B_PE1_ENUM(PE_M14B0_PE1_CTI_CTRL_01_T),		///< [33] 0xc0024534L
	PE_SHP_HW_M14B_PE1_ENUM(PE_M14B0_PE1_CTI_CTRL_02_T),		///< [34] 0xc0024538L
	PE_SHP_HW_M14B_PE1_ENUM(PE_M14B0_PE1_SRE_CTRL_00_T),		///< [35] 0xc0024860L
	PE_SHP_HW_M14B_PE1_ENUM(PE_M14B0_PE1_SRE_CTRL_06_T),		///< [36] 0xc0024878L
	PE_SHP_HW_M14B_PE1_ENUM(PE_M14B0_PE1_SRE_CTRL_07_T),		///< [37] 0xc002487cL
	PE_SHP_HW_M14B_PE1_ENUM(PE_M14B0_PE1_SRE_CTRL_08_T),		///< [38] 0xc0024880L
	PE_SHP_HW_M14B_PE1_ENUM(PE_M14B0_PE1_SRE_CTRL_11_T),		///< [39] 0xc00248a4L
	PE_SHP_HW_M14B_PE1_ENUM(MAX)								///< max num
}
PE_SHP_HW_M14B_PE1_REG_NUM;

/**
 *	pe register num
 */
typedef enum
{
	PE_SHP_HW_M14B_MSC_ENUM(PE_M14B0_MSC_CHROMA_TEAR_CTRL_T) = 0,	///< [0]  0xc0023878L
	PE_SHP_HW_M14B_MSC_ENUM(MAX)									///< max num
}
PE_SHP_HW_M14B_MSC_REG_NUM;

/**
 *	pe register control type
 */
typedef struct
{
	PE_M14B0_PE1_DERH_CTRL_00_T        	pe1_derh_ctrl_00;             ///< [0]  0xc00243b0L
	PE_M14B0_PE1_DERH_CTRL_05_T        	pe1_derh_ctrl_05;             ///< [1]  0xc00243c4L
	PE_M14B0_PE1_DERH_CTRL_06_T        	pe1_derh_ctrl_06;             ///< [2]  0xc00243c8L
	PE_M14B0_PE1_DERH_CTRL_07_T        	pe1_derh_ctrl_07;             ///< [3]  0xc00243ccL
	PE_M14B0_PE1_DERH_CTRL_0B_T        	pe1_derh_ctrl_0b;             ///< [4]  0xc00243dcL
	PE_M14B0_PE1_DERH_CTRL_0C_T        	pe1_derh_ctrl_0c;             ///< [5]  0xc00243e0L
	PE_M14B0_PE1_DERV_CTRL_0_T         	pe1_derv_ctrl_0;              ///< [6]  0xc00243f0L
	PE_M14B0_PE1_DERV_CTRL_1_T         	pe1_derv_ctrl_1;              ///< [7]  0xc00243f4L
	PE_M14B0_PE1_DERV_CTRL_2_T         	pe1_derv_ctrl_2;              ///< [8]  0xc00243f8L
	PE_M14B0_PE1_DERV_CTRL_3_T         	pe1_derv_ctrl_3;              ///< [9]  0xc00243fcL
	PE_M14B0_PE1_DP_CTRL_01_T          	pe1_dp_ctrl_01;               ///< [10] 0xc0024414L
	PE_M14B0_PE1_SP_CTRL_01_T          	pe1_sp_ctrl_01;               ///< [11] 0xc0024434L
	PE_M14B0_PE1_SP_CTRL_02_T          	pe1_sp_ctrl_02;               ///< [12] 0xc0024438L
	PE_M14B0_PE1_SP_CTRL_06_T          	pe1_sp_ctrl_06;               ///< [13] 0xc0024448L
	PE_M14B0_PE1_MP_CTRL_01_T          	pe1_mp_ctrl_01;               ///< [14] 0xc0024474L
	PE_M14B0_PE1_MP_CTRL_02_T          	pe1_mp_ctrl_02;               ///< [15] 0xc0024478L
	PE_M14B0_PE1_MP_CTRL_06_T          	pe1_mp_ctrl_06;               ///< [16] 0xc0024488L
	PE_M14B0_PE1_CORING_CTRL_00_T      	pe1_coring_ctrl_00;           ///< [17] 0xc00244b0L
	PE_M14B0_PE1_CORING_CTRL_01_T      	pe1_coring_ctrl_01;           ///< [18] 0xc00244b4L
	PE_M14B0_PE1_CORING_CTRL_02_T      	pe1_coring_ctrl_02;           ///< [19] 0xc00244b8L
	PE_M14B0_PE1_CORING_CTRL_04_T      	pe1_coring_ctrl_04;           ///< [20] 0xc00244c0L
	PE_M14B0_PE1_CORING_CTRL_05_T      	pe1_coring_ctrl_05;           ///< [21] 0xc00244c4L
	PE_M14B0_PE1_CORING_CTRL_06_T      	pe1_coring_ctrl_06;           ///< [22] 0xc00244c8L
	PE_M14B0_PE1_CORING_CTRL_07_T      	pe1_coring_ctrl_07;           ///< [23] 0xc00244ccL
	PE_M14B0_PE1_CORING_CTRL_09_T      	pe1_coring_ctrl_09;           ///< [24] 0xc00244d4L
	PE_M14B0_PE1_TI_CTRL_0_T           	pe1_ti_ctrl_0;                ///< [25] 0xc0024500L
	PE_M14B0_PE1_TI_CTRL_1_T           	pe1_ti_ctrl_1;                ///< [26] 0xc0024504L
	PE_M14B0_PE1_TI_CTRL_2_T           	pe1_ti_ctrl_2;                ///< [27] 0xc0024508L
	PE_M14B0_PE1_DJ_CTRL_00_T          	pe1_dj_ctrl_00;               ///< [28] 0xc0024520L
	PE_M14B0_PE1_DJ_CTRL_01_T          	pe1_dj_ctrl_01;               ///< [29] 0xc0024524L
	PE_M14B0_PE1_DJ_CTRL_02_T          	pe1_dj_ctrl_02;               ///< [30] 0xc0024528L
	PE_M14B0_PE1_DJ_CTRL_03_T          	pe1_dj_ctrl_03;               ///< [31] 0xc002452cL
	PE_M14B0_PE1_CTI_CTRL_00_T         	pe1_cti_ctrl_00;              ///< [32] 0xc0024530L
	PE_M14B0_PE1_CTI_CTRL_01_T         	pe1_cti_ctrl_01;              ///< [33] 0xc0024534L
	PE_M14B0_PE1_CTI_CTRL_02_T         	pe1_cti_ctrl_02;              ///< [34] 0xc0024538L
	PE_M14B0_PE1_SRE_CTRL_00_T         	pe1_sre_ctrl_00;              ///< [35] 0xc0024860L
	PE_M14B0_PE1_SRE_CTRL_06_T         	pe1_sre_ctrl_06;              ///< [36] 0xc0024878L
	PE_M14B0_PE1_SRE_CTRL_07_T         	pe1_sre_ctrl_07;              ///< [37] 0xc002487cL
	PE_M14B0_PE1_SRE_CTRL_08_T         	pe1_sre_ctrl_08;              ///< [38] 0xc0024880L
	PE_M14B0_PE1_SRE_CTRL_11_T         	pe1_sre_ctrl_11;              ///< [39] 0xc00248a4L
}
PE_SHP_HW_M14B_PE1_REG_T;

/**
 *	pe register control type
 */
typedef struct
{
	PE_M14B0_MSC_CHROMA_TEAR_CTRL_T    	msc_chroma_tear_ctrl;         ///< [0]  0xc0023878L
}
PE_SHP_HW_M14B_MSC_REG_T;

/**
 *	pe register control type
 */
typedef struct
{
	UINT32 addr[PE_SHP_HW_M14B_PE1_ENUM(MAX)];	///< ctrl addr
	PE_SHP_HW_M14B_PE1_REG_T data;				///< ctrl data
	PE_SHP_HW_M14B_PE1_REG_T mask;				///< ctrl mask
}
PE_SHP_HW_M14B_PE1_REG_CTRL_T;

/**
 *	pe register control type
 */
typedef struct
{
	UINT32 addr[PE_SHP_HW_M14B_MSC_ENUM(MAX)];	///< ctrl addr
	PE_SHP_HW_M14B_MSC_REG_T data;				///< ctrl data
	PE_SHP_HW_M14B_MSC_REG_T mask;				///< ctrl mask
}
PE_SHP_HW_M14B_MSC_REG_CTRL_T;

/**
 *	pe register match type
 */
typedef struct
{
	UINT32 res_size;				///< res size
	PE_REG_PARAM_T *res_mtbl;		///< res match table
	UINT32 sre_size;				///< sre size
	PE_REG_PARAM_T *sre_mtbl;		///< sre match table
}
PE_SHP_HW_M14B_PE1_REG_MATCH_T;

/**
 *	pe register match type
 */
typedef struct
{
	UINT32 vfc_size;				///< vfc size
	PE_REG_PARAM_T *vfc_mtbl;		///< vfc match table
}
PE_SHP_HW_M14B_MSC_REG_MATCH_T;
#endif

/**
* scl setting flag type
*	- to avoid double setting scaler filter
*/
typedef struct
{
	UINT32 h_y;	///< horizontal luma
	UINT32 v_y;	///< vertical luma
	UINT32 h_c;	///< horizontal chroma
	UINT32 v_c;	///< vertical chroma
}
PE_SHP_HW_M14_FLAG_T;

/**
* scaler wr tap control type
*/
typedef struct
{
	UINT32 base_addr;	///< base address
	UINT32 tbl_size;	///< table size
	UINT32 tap_type;	///< tap type, see PE_SHP_HW_M14_SCL_WR_TAP
}
PE_SHP_HW_M14_SCL_WR_CTRL_T;

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
static int PE_SHP_HW_M14B_GetResDfltTable(PE_REG_PARAM_T *dst_dflt, const PE_REG_PARAM_T *src_dflt, UINT32 dflt_size);
static int PE_SHP_HW_M14B_GetSreDfltTable(PE_REG_PARAM_T *dst_dflt, const PE_REG_PARAM_T *src_dflt, UINT32 dflt_size);
static int PE_SHP_HW_M14B_GetVfcDfltTable(PE_REG_PARAM_T *dst_dflt, const PE_REG_PARAM_T *src_dflt, UINT32 dflt_size);
#endif
static int PE_SHP_HW_M14B_SetDefault(LX_PE_DEFAULT_SETTINGS_T *pstParams);
static int PE_SHP_HW_M14A_SetDefault(LX_PE_DEFAULT_SETTINGS_T *pstParams);
static PE_SHP_HW_M14B_RES_FMT PE_SHP_HW_M14B_ConvDispInfoToResFmt(LX_PE_INF_DISPLAY_T *disp_inf);
static PE_SHP_HW_M14B_SRE_FMT PE_SHP_HW_M14B_ConvDispInfoToSreFmt(LX_PE_INF_DISPLAY_T *disp_inf);
static PE_SHP_HW_M14B_SRL_FMT PE_SHP_HW_M14B_ConvDispInfoToSrlFmt(LX_PE_INF_DISPLAY_T *disp_inf);
static PE_SHP_HW_M14B_VFC_FMT PE_SHP_HW_M14B_ConvDispInfoToVfcFmt(LX_PE_INF_DISPLAY_T *disp_inf);
static PE_SHP_HW_M14B_CTI_FMT PE_SHP_HW_M14B_ConvDispInfoToCtiFmt(LX_PE_INF_DISPLAY_T *disp_inf);
static PE_SHP_HW_M14A_RES_FMT PE_SHP_HW_M14A_ConvDispInfoToResFmt(LX_PE_INF_DISPLAY_T *disp_inf);
static PE_SHP_HW_M14A_VFC_FMT PE_SHP_HW_M14A_ConvDispInfoToVfcFmt(LX_PE_INF_DISPLAY_T *disp_inf);
static int PE_SHP_HW_M14_GetScalerFilterValue(PE_TSK_M14_CFG_T *pCfg,PE_SHP_HW_M14_SETTINGS_T *pstParams);
static void PE_SHP_HW_M14_CheckValidScalerFilterValue(PE_SHP_HW_M14_SETTINGS_T *pstParams);
static PE_SHP_HW_M14_FLAG_T PE_SHP_HW_M14_GetDoubleSetFlag(LX_PE_WIN_ID win_id,LX_PE_SHP_SCLFILTER_T pre_idx,PE_SHP_HW_M14_SETTINGS_T *cur_param);
static int PE_SHP_HW_M14_SetSclCoeff(LX_PE_WIN_ID win_id, PE_SHP_HW_M14_FLAG_T flag, PE_SHP_HW_M14_SETTINGS_T *pstParams);
static void PE_SHP_HW_M14_WrSclCoeff(PE_SHP_HW_M14_SCL_WR_CTRL_T wr_ctrl, const PE_REG_PARAM_T *reg);
static void PE_SHP_HW_M14_SetSclFltRnwMode(UINT32 rnw_mode);
static UINT32 PE_SHP_HW_M14_RdLumaSclFltCoeff(LX_PE_WIN_ID win_id, UINT32 config, UINT32 *data);
static UINT32 PE_SHP_HW_M14_RdChromaSclFltCoeff(LX_PE_WIN_ID win_id, UINT32 config, UINT32 *data);
static UINT32 PE_SHP_HW_M14_RdSubSclFltCoeff(UINT32 config, UINT32 *data);

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
static UINT8 _g_shp_hw_m14_trace=0x0;	//default should be off.
static PE_SHP_HW_M14_SETTINGS_T _g_pe_shp_hw_m14_info;
#ifdef PE_HW_M14B_USE_REG_CTRL
static PE_SHP_HW_M14B_PE1_REG_CTRL_T _g_pe_shp_hw_m14b_pe1_reg_ctrl;
static PE_SHP_HW_M14B_MSC_REG_CTRL_T _g_pe_shp_hw_m14b_msc_reg_ctrl;
static PE_SHP_HW_M14B_PE1_REG_MATCH_T _g_pe_shp_hw_m14b_pe1_reg_match = {0, NULL, 0, NULL};
static PE_SHP_HW_M14B_MSC_REG_MATCH_T _g_pe_shp_hw_m14b_msc_reg_match = {0, NULL};
const UINT32 g_pe_shp_hw_m14b_pe1_reg_addr[PE_SHP_HW_M14B_PE1_ENUM(MAX)] = {
	0x43b0,           ///< [0]  0xc00243b0L
	0x43c4,           ///< [1]  0xc00243c4L
	0x43c8,           ///< [2]  0xc00243c8L
	0x43cc,           ///< [3]  0xc00243ccL
	0x43dc,           ///< [4]  0xc00243dcL
	0x43e0,           ///< [5]  0xc00243e0L
	0x43f0,           ///< [6]  0xc00243f0L
	0x43f4,           ///< [7]  0xc00243f4L
	0x43f8,           ///< [8]  0xc00243f8L
	0x43fc,           ///< [9]  0xc00243fcL
	0x4414,           ///< [10] 0xc0024414L
	0x4434,           ///< [11] 0xc0024434L
	0x4438,           ///< [12] 0xc0024438L
	0x4448,           ///< [13] 0xc0024448L
	0x4474,           ///< [14] 0xc0024474L
	0x4478,           ///< [15] 0xc0024478L
	0x4488,           ///< [16] 0xc0024488L
	0x44b0,           ///< [17] 0xc00244b0L
	0x44b4,           ///< [18] 0xc00244b4L
	0x44b8,           ///< [19] 0xc00244b8L
	0x44c0,           ///< [20] 0xc00244c0L
	0x44c4,           ///< [21] 0xc00244c4L
	0x44c8,           ///< [22] 0xc00244c8L
	0x44cc,           ///< [23] 0xc00244ccL
	0x44d4,           ///< [24] 0xc00244d4L
	0x4500,           ///< [25] 0xc0024500L
	0x4504,           ///< [26] 0xc0024504L
	0x4508,           ///< [27] 0xc0024508L
	0x4520,           ///< [28] 0xc0024520L
	0x4524,           ///< [29] 0xc0024524L
	0x4528,           ///< [30] 0xc0024528L
	0x452c,           ///< [31] 0xc002452cL
	0x4530,           ///< [32] 0xc0024530L
	0x4534,           ///< [33] 0xc0024534L
	0x4538,           ///< [34] 0xc0024538L
	0x4860,           ///< [35] 0xc0024860L
	0x4878,           ///< [36] 0xc0024878L
	0x487c,           ///< [37] 0xc002487cL
	0x4880,           ///< [38] 0xc0024880L
	0x48a4            ///< [39] 0xc00248a4L
};
const UINT32 g_pe_shp_hw_m14b_msc_reg_addr[PE_SHP_HW_M14B_MSC_ENUM(MAX)] = {
	0x3878            ///< [0]  0xc0023878L
};
#endif

/*----------------------------------------------------------------------------------------
	Implementation Group
----------------------------------------------------------------------------------------*/
/**
 * init shp
 *
 * @param   *pstParams [in] PE_CFG_CTRL_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_SHP_HW_M14_Init(PE_CFG_CTRL_T *pstParams)
{
	int ret = RET_OK;
	#ifdef PE_HW_M14B_USE_REG_CTRL
	UINT32 i = 0, j = 0, tbl_size = 0, tbl_num = 0;
	PE_SHP_HW_M14B_PE1_REG_CTRL_T *pe1_reg_ctrl = &_g_pe_shp_hw_m14b_pe1_reg_ctrl;
	PE_SHP_HW_M14B_PE1_REG_MATCH_T *pe1_reg_match = &_g_pe_shp_hw_m14b_pe1_reg_match;
	PE_SHP_HW_M14B_MSC_REG_CTRL_T *msc_reg_ctrl = &_g_pe_shp_hw_m14b_msc_reg_ctrl;
	PE_SHP_HW_M14B_MSC_REG_MATCH_T *msc_reg_match = &_g_pe_shp_hw_m14b_msc_reg_match;
	#endif
	do{
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_M14)
		{
			PE_SHP_HW_M14_DBG_PRINT("init param.\n");
			/* 20130613, sj.youm, remove memset0 and add tap and fir init */
			_g_pe_shp_hw_m14_info.cur_scl_idx[LX_PE_WIN_0].win_id = LX_PE_WIN_0;
			_g_pe_shp_hw_m14_info.cur_scl_idx[LX_PE_WIN_0].h_luma_id = PE_SHP_HW_M14_IDX_Y_DFLT;
			_g_pe_shp_hw_m14_info.cur_scl_idx[LX_PE_WIN_0].v_luma_id = PE_SHP_HW_M14_IDX_Y_DFLT;
			_g_pe_shp_hw_m14_info.cur_scl_idx[LX_PE_WIN_0].h_chrm_id = PE_SHP_HW_M14_IDX_C_DFLT;
			_g_pe_shp_hw_m14_info.cur_scl_idx[LX_PE_WIN_0].v_chrm_id = PE_SHP_HW_M14_IDX_C_DFLT;
			_g_pe_shp_hw_m14_info.cur_scl_idx[LX_PE_WIN_1].win_id = LX_PE_WIN_1;
			_g_pe_shp_hw_m14_info.cur_scl_idx[LX_PE_WIN_1].h_luma_id = PE_SHP_HW_M14_IDX_Y_DFLT;
			_g_pe_shp_hw_m14_info.cur_scl_idx[LX_PE_WIN_1].v_luma_id = PE_SHP_HW_M14_IDX_Y_DFLT;
			_g_pe_shp_hw_m14_info.cur_scl_idx[LX_PE_WIN_1].h_chrm_id = PE_SHP_HW_M14_IDX_C_DFLT;
			_g_pe_shp_hw_m14_info.cur_scl_idx[LX_PE_WIN_1].v_chrm_id = PE_SHP_HW_M14_IDX_C_DFLT;
			_g_pe_shp_hw_m14_info.pre_scl_idx[LX_PE_WIN_0].win_id = LX_PE_WIN_0;
			_g_pe_shp_hw_m14_info.pre_scl_idx[LX_PE_WIN_0].h_luma_id = 0xff;
			_g_pe_shp_hw_m14_info.pre_scl_idx[LX_PE_WIN_0].v_luma_id = 0xff;
			_g_pe_shp_hw_m14_info.pre_scl_idx[LX_PE_WIN_0].h_chrm_id = 0xff;
			_g_pe_shp_hw_m14_info.pre_scl_idx[LX_PE_WIN_0].v_chrm_id = 0xff;
			_g_pe_shp_hw_m14_info.pre_scl_idx[LX_PE_WIN_1].win_id = LX_PE_WIN_1;
			_g_pe_shp_hw_m14_info.pre_scl_idx[LX_PE_WIN_1].h_luma_id = 0xff;
			_g_pe_shp_hw_m14_info.pre_scl_idx[LX_PE_WIN_1].v_luma_id = 0xff;
			_g_pe_shp_hw_m14_info.pre_scl_idx[LX_PE_WIN_1].h_chrm_id = 0xff;
			_g_pe_shp_hw_m14_info.pre_scl_idx[LX_PE_WIN_1].v_chrm_id = 0xff;
			_g_pe_shp_hw_m14_info.h_y_tap[LX_PE_WIN_0] = PE_SHP_HW_M14_SCL_NUM;
			_g_pe_shp_hw_m14_info.h_c_tap[LX_PE_WIN_0] = PE_SHP_HW_M14_SCL_NUM;
			_g_pe_shp_hw_m14_info.v_y_tap[LX_PE_WIN_0] = PE_SHP_HW_M14_SCL_NUM;
			_g_pe_shp_hw_m14_info.v_c_tap[LX_PE_WIN_0] = PE_SHP_HW_M14_SCL_NUM;
			_g_pe_shp_hw_m14_info.h_y_tap[LX_PE_WIN_1] = PE_SHP_HW_M14_SCL_NUM;
			_g_pe_shp_hw_m14_info.h_c_tap[LX_PE_WIN_1] = PE_SHP_HW_M14_SCL_NUM;
			_g_pe_shp_hw_m14_info.v_y_tap[LX_PE_WIN_1] = PE_SHP_HW_M14_SCL_NUM;
			_g_pe_shp_hw_m14_info.v_c_tap[LX_PE_WIN_1] = PE_SHP_HW_M14_SCL_NUM;
			_g_pe_shp_hw_m14_info.y_fir[LX_PE_WIN_0] = PE_SHP_HW_M14_FIR_NUM;
			_g_pe_shp_hw_m14_info.c_fir[LX_PE_WIN_0] = PE_SHP_HW_M14_FIR_NUM;
			_g_pe_shp_hw_m14_info.y_fir[LX_PE_WIN_1] = PE_SHP_HW_M14_FIR_NUM;
			_g_pe_shp_hw_m14_info.c_fir[LX_PE_WIN_1] = PE_SHP_HW_M14_FIR_NUM;
			_g_pe_shp_hw_m14_info.pre_res_en[LX_PE_WIN_0] = 0xff;
			_g_pe_shp_hw_m14_info.pre_res_en[LX_PE_WIN_1] = 0xff;
			_g_pe_shp_hw_m14_info.res0a_fmt = PE_SHP_M14A_RES_NUM;
			_g_pe_shp_hw_m14_info.res1a_fmt = PE_SHP_M14A_RES_NUM;
			_g_pe_shp_hw_m14_info.vfc0a_fmt = PE_SHP_M14A_VFC_NUM;
			_g_pe_shp_hw_m14_info.vfc1a_fmt = PE_SHP_M14A_VFC_NUM;
			_g_pe_shp_hw_m14_info.res0b_fmt = PE_SHP_M14B_RES_NUM;
			_g_pe_shp_hw_m14_info.sre0b_fmt = PE_SHP_M14B_SRE_NUM;
			_g_pe_shp_hw_m14_info.srl0b_fmt = PE_SHP_M14B_SRL_NUM;
			_g_pe_shp_hw_m14_info.vfc0b_fmt = PE_SHP_M14B_VFC_NUM;
			_g_pe_shp_hw_m14_info.cti0b_fmt = PE_SHP_M14B_CTI_NUM;
			_g_pe_shp_hw_m14_info.dft_msc_chroma_tear_ctrl = 0x0;
			_g_pe_shp_hw_m14_info.cur_msc_chroma_tear_ctrl = 0x0;
			if(PE_KDRV_VER_M14BX)
			{
				PE_SET_REG_TABLE(SHP_HW_M14,shp_l_init_m14b0,PE_REG_M14B_WRITE_BASE);
				PE_SET_REG_TABLE(SHP_HW_M14,sre_l_init_m14b0,PE_REG_M14B_WRITE_BASE);
				PE_SET_REG_TABLE(SHP_HW_M14,sre_l_lut_init_ai_m14b0,PE_REG_M14B_WRITE_BASE);
				#ifdef PE_HW_M14B_USE_REG_CTRL
				/* init reg ctrl data and mask */
				memset(&(pe1_reg_ctrl->data), 0, sizeof(PE_SHP_HW_M14B_PE1_REG_T));	// 0x0
				memset(&(pe1_reg_ctrl->mask), -1, sizeof(PE_SHP_HW_M14B_PE1_REG_T));// 0xffffffff
				memset(&(msc_reg_ctrl->data), 0, sizeof(PE_SHP_HW_M14B_MSC_REG_T));	// 0x0
				memset(&(msc_reg_ctrl->mask), -1, sizeof(PE_SHP_HW_M14B_MSC_REG_T));// 0xffffffff
				/* init reg ctrl addr */
				for (i=0; i<PE_SHP_HW_M14B_PE1_ENUM(MAX); i++)
				{
					pe1_reg_ctrl->addr[i] = g_pe_shp_hw_m14b_pe1_reg_addr[i];
				}
				for (i=0; i<PE_SHP_HW_M14B_MSC_ENUM(MAX); i++)
				{
					msc_reg_ctrl->addr[i] = g_pe_shp_hw_m14b_msc_reg_addr[i];
				}
				/* res matching table */
				if (pe1_reg_match->res_size==0 && pe1_reg_match->res_mtbl==NULL)
				{
					tbl_size = sizeof(shp_l_pc_default_m14b0);
					tbl_num = tbl_size/sizeof(PE_REG_PARAM_T);
					/* alloc matching table */
					pe1_reg_match->res_mtbl = (PE_REG_PARAM_T *)OS_Malloc(tbl_size);
					PE_SHP_HW_M14_CHECK_CODE(!(pe1_reg_match->res_mtbl), break, \
						"[%s,%d] pe1_reg_match->res_mtbl is null.\n", __F__, __L__);
					pe1_reg_match->res_size = tbl_size;

					for (i=0; i<tbl_num; i++)
					{
						/* set addr and data of matching table */
						pe1_reg_match->res_mtbl[i].addr = shp_l_pc_default_m14b0[i].addr;
						pe1_reg_match->res_mtbl[i].data = PE_SHP_HW_M14B_PE1_ENUM(MAX);
						/* check matching number */
						for (j=0; j<PE_SHP_HW_M14B_PE1_ENUM(MAX); j++)
						{
							if (pe1_reg_match->res_mtbl[i].addr == pe1_reg_ctrl->addr[j])
							{
								pe1_reg_match->res_mtbl[i].data = j;
								break;
							}
						}
					}
					PE_PRINT_NOTI("res matching table created.\n");
				}
				/* sre matching table */
				if (pe1_reg_match->sre_size==0 && pe1_reg_match->sre_mtbl==NULL)
				{
					tbl_size = sizeof(sre_l_pc_default_m14b0);
					tbl_num = tbl_size/sizeof(PE_REG_PARAM_T);
					/* alloc matching table */
					pe1_reg_match->sre_mtbl = (PE_REG_PARAM_T *)OS_Malloc(tbl_size);
					PE_SHP_HW_M14_CHECK_CODE(!(pe1_reg_match->sre_mtbl), break, \
						"[%s,%d] pe1_reg_match->sre_mtbl is null.\n", __F__, __L__);
					pe1_reg_match->sre_size = tbl_size;

					for (i=0; i<tbl_num; i++)
					{
						/* set addr and data of matching table */
						pe1_reg_match->sre_mtbl[i].addr = sre_l_pc_default_m14b0[i].addr;
						pe1_reg_match->sre_mtbl[i].data = PE_SHP_HW_M14B_PE1_ENUM(MAX);
						/* check matching number */
						for (j=0; j<PE_SHP_HW_M14B_PE1_ENUM(MAX); j++)
						{
							if (pe1_reg_match->sre_mtbl[i].addr == pe1_reg_ctrl->addr[j])
							{
								pe1_reg_match->sre_mtbl[i].data = j;
								break;
							}
						}
					}
					PE_PRINT_NOTI("sre matching table created.\n");
				}
				/* vfc matching table */
				if (msc_reg_match->vfc_size==0 && msc_reg_match->vfc_mtbl==NULL)
				{
					tbl_size = sizeof(vflt_c_l_pc_default_m14b0);
					tbl_num = tbl_size/sizeof(PE_REG_PARAM_T);
					/* alloc matching table */
					msc_reg_match->vfc_mtbl = (PE_REG_PARAM_T *)OS_Malloc(tbl_size);
					PE_SHP_HW_M14_CHECK_CODE(!(msc_reg_match->vfc_mtbl), break, \
						"[%s,%d] msc_reg_match->vfc_mtbl is null.\n", __F__, __L__);
					msc_reg_match->vfc_size = tbl_size;

					for (i=0; i<tbl_num; i++)
					{
						/* set addr and data of matching table */
						msc_reg_match->vfc_mtbl[i].addr = vflt_c_l_pc_default_m14b0[i].addr;
						msc_reg_match->vfc_mtbl[i].data = PE_SHP_HW_M14B_MSC_ENUM(MAX);
						/* check matching number */
						for (j=0; j<PE_SHP_HW_M14B_MSC_ENUM(MAX); j++)
						{
							if (msc_reg_match->vfc_mtbl[i].addr == msc_reg_ctrl->addr[j])
							{
								msc_reg_match->vfc_mtbl[i].data = j;
								break;
							}
						}
					}
					PE_PRINT_NOTI("vfc matching table created.\n");
				}
				#endif
			}
			else if(PE_KDRV_VER_M14AX)
			{
				PE_SET_REG_TABLE(SHP_HW_M14,shp_l_init_m14a0,PE_REG_M14_WRITE_BASE);
				PE_SET_REG_TABLE(SHP_HW_M14,shp_r_init_m14a0,PE_REG_M14_WRITE_BASE);
			}
			else
			{
				PE_SHP_HW_M14_DBG_PRINT("nothing to do\n");
			}
		}
		else
		{
			PE_SHP_HW_M14_DBG_PRINT("nothing to do\n");	ret = RET_OK;
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
int PE_SHP_HW_M14_SetDebugSettings(LX_PE_DBG_SETTINGS_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		PE_SHP_HW_M14_DBG_PRINT("wid:%d,type:0x%x,[0x%x]print:0x%x,bypass:0x%x\n",\
			pstParams->win_id,pstParams->type,(0x1<<PE_ITEM_PKTMODL(SHP)),\
			pstParams->print_lvl,pstParams->bypass);
		/* set print level */
		if(pstParams->type&LX_PE_DBG_LV)
		{
			_g_shp_hw_m14_trace = (pstParams->print_lvl & (0x1<<PE_ITEM_PKTMODL(SHP)))? 0x1:0x0;
		}
	}while(0);
	return ret;
}
/**
 * default shp setting
 *
 * @param   *pstParams [in] LX_PE_DEFAULT_SETTINGS_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_SHP_HW_M14_SetDefault(LX_PE_DEFAULT_SETTINGS_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_M14BX)
		{
			ret = PE_SHP_HW_M14B_SetDefault(pstParams);
			PE_SHP_HW_M14_CHECK_CODE(ret, break, \
				"[%s,%d] PE_SHP_HW_M14B_SetDefault() error.\n", __F__, __L__);
		}
		else if(PE_KDRV_VER_M14AX)
		{
			ret = PE_SHP_HW_M14A_SetDefault(pstParams);
			PE_SHP_HW_M14_CHECK_CODE(ret, break, \
				"[%s,%d] PE_SHP_HW_M14A_SetDefault() error.\n", __F__, __L__);
		}
		else
		{
			PE_SHP_HW_M14_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
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
static int PE_SHP_HW_M14B_GetResDfltTable(PE_REG_PARAM_T *dst_dflt, const PE_REG_PARAM_T *src_dflt, UINT32 dflt_size)
{
	int ret = RET_OK;
	UINT32 dflt_num = 0, match_num = 0, i = 0;
	UINT32 match_size = _g_pe_shp_hw_m14b_pe1_reg_match.res_size;
	PE_REG_PARAM_T *p_match_tbl = _g_pe_shp_hw_m14b_pe1_reg_match.res_mtbl;
	UINT32 *p_reg_data = (UINT32 *)&(_g_pe_shp_hw_m14b_pe1_reg_ctrl.data);
	UINT32 *p_reg_mask = (UINT32 *)&(_g_pe_shp_hw_m14b_pe1_reg_ctrl.mask);

	PE_SHP_HW_M14_CHECK_CODE(!dst_dflt, return RET_ERROR, \
		"[%s,%d] dst_dflt is null, not ready.\n", __F__, __L__);
	PE_SHP_HW_M14_CHECK_CODE(!src_dflt, return RET_ERROR, \
		"[%s,%d] src_dflt is null, not ready.\n", __F__, __L__);
	PE_SHP_HW_M14_CHECK_CODE(!match_size, return RET_ERROR, \
		"[%s,%d] match size is zero.\n", __F__, __L__);
	PE_SHP_HW_M14_CHECK_CODE(!p_match_tbl, return RET_ERROR, \
		"[%s,%d] match tbl is null, not ready.\n", __F__, __L__);
	PE_SHP_HW_M14_CHECK_CODE(match_size!=dflt_size, return RET_ERROR, \
		"[%s,%d] size mismatch(%d vs. %d).\n", __F__, __L__, match_size, dflt_size);

	dflt_num = dflt_size/sizeof(PE_REG_PARAM_T);
	for (i=0; i<(dflt_num); i++)
	{
		dst_dflt[i].addr = src_dflt[i].addr;
		match_num = p_match_tbl[i].data;
		if (match_num<PE_SHP_HW_M14B_PE1_ENUM(MAX))
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
static int PE_SHP_HW_M14B_GetSreDfltTable(PE_REG_PARAM_T *dst_dflt, const PE_REG_PARAM_T *src_dflt, UINT32 dflt_size)
{
	int ret = RET_OK;
	UINT32 dflt_num = 0, match_num = 0, i = 0;
	UINT32 match_size = _g_pe_shp_hw_m14b_pe1_reg_match.sre_size;
	PE_REG_PARAM_T *p_match_tbl = _g_pe_shp_hw_m14b_pe1_reg_match.sre_mtbl;
	UINT32 *p_reg_data = (UINT32 *)&(_g_pe_shp_hw_m14b_pe1_reg_ctrl.data);
	UINT32 *p_reg_mask = (UINT32 *)&(_g_pe_shp_hw_m14b_pe1_reg_ctrl.mask);

	PE_SHP_HW_M14_CHECK_CODE(!dst_dflt, return RET_ERROR, \
		"[%s,%d] dst_dflt is null, not ready.\n", __F__, __L__);
	PE_SHP_HW_M14_CHECK_CODE(!src_dflt, return RET_ERROR, \
		"[%s,%d] src_dflt is null, not ready.\n", __F__, __L__);
	PE_SHP_HW_M14_CHECK_CODE(!match_size, return RET_ERROR, \
		"[%s,%d] match size is zero.\n", __F__, __L__);
	PE_SHP_HW_M14_CHECK_CODE(!p_match_tbl, return RET_ERROR, \
		"[%s,%d] match tbl is null, not ready.\n", __F__, __L__);
	PE_SHP_HW_M14_CHECK_CODE(match_size!=dflt_size, return RET_ERROR, \
		"[%s,%d] size mismatch(%d vs. %d).\n", __F__, __L__, match_size, dflt_size);

	dflt_num = dflt_size/sizeof(PE_REG_PARAM_T);
	for (i=0; i<(dflt_num); i++)
	{
		dst_dflt[i].addr = src_dflt[i].addr;
		match_num = p_match_tbl[i].data;
		if (match_num<PE_SHP_HW_M14B_PE1_ENUM(MAX))
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
static int PE_SHP_HW_M14B_GetVfcDfltTable(PE_REG_PARAM_T *dst_dflt, const PE_REG_PARAM_T *src_dflt, UINT32 dflt_size)
{
	int ret = RET_OK;
	UINT32 dflt_num = 0, match_num = 0, i = 0;
	UINT32 match_size = _g_pe_shp_hw_m14b_msc_reg_match.vfc_size;
	PE_REG_PARAM_T *p_match_tbl = _g_pe_shp_hw_m14b_msc_reg_match.vfc_mtbl;
	UINT32 *p_reg_data = (UINT32 *)&(_g_pe_shp_hw_m14b_msc_reg_ctrl.data);
	UINT32 *p_reg_mask = (UINT32 *)&(_g_pe_shp_hw_m14b_msc_reg_ctrl.mask);
	PE_SHP_HW_M14_SETTINGS_T *pInfo = &_g_pe_shp_hw_m14_info;

	PE_SHP_HW_M14_CHECK_CODE(!dst_dflt, return RET_ERROR, \
		"[%s,%d] dst_dflt is null, not ready.\n", __F__, __L__);
	PE_SHP_HW_M14_CHECK_CODE(!src_dflt, return RET_ERROR, \
		"[%s,%d] src_dflt is null, not ready.\n", __F__, __L__);
	PE_SHP_HW_M14_CHECK_CODE(!match_size, return RET_ERROR, \
		"[%s,%d] match size is zero.\n", __F__, __L__);
	PE_SHP_HW_M14_CHECK_CODE(!p_match_tbl, return RET_ERROR, \
		"[%s,%d] match tbl is null, not ready.\n", __F__, __L__);
	PE_SHP_HW_M14_CHECK_CODE(match_size!=dflt_size, return RET_ERROR, \
		"[%s,%d] size mismatch(%d vs. %d).\n", __F__, __L__, match_size, dflt_size);

	dflt_num = dflt_size/sizeof(PE_REG_PARAM_T);
	for (i=0; i<(dflt_num); i++)
	{
		dst_dflt[i].addr = src_dflt[i].addr;
		match_num = p_match_tbl[i].data;
		if (match_num<PE_SHP_HW_M14B_MSC_ENUM(MAX))
		{
			/* store msc_chroma_tear_ctrl default for setting vfc apply on task */
			if (src_dflt[i].addr==0x3878)
			{
				pInfo->dft_msc_chroma_tear_ctrl = src_dflt[i].data;
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
 * default shp setting(for M14BX)
 *
 * @param   *pstParams [in] LX_PE_DEFAULT_SETTINGS_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_SHP_HW_M14B_SetDefault(LX_PE_DEFAULT_SETTINGS_T *pstParams)
{
	int ret = RET_OK;
	PE_INF_M14_SETTINGS_T inf_set;
	LX_PE_INF_DISPLAY_T *cur0_inf = NULL;
	PE_SHP_HW_M14B_RES_FMT cur0_res_fmt;
	PE_SHP_HW_M14B_SRE_FMT cur0_sre_fmt;
	PE_SHP_HW_M14B_SRL_FMT cur0_srl_fmt;
	PE_SHP_HW_M14B_VFC_FMT cur0_vfc_fmt;
	PE_SHP_HW_M14B_CTI_FMT cur0_cti_fmt;
	PE_SHP_HW_M14_SETTINGS_T *pInfo=&_g_pe_shp_hw_m14_info;

	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		ret = PE_INF_M14_GetCurInfSettings(&inf_set);
		PE_SHP_HW_M14_CHECK_CODE(ret, break, \
			"[%s,%d] PE_INF_M14_GetCurInfSettings() error.\n", __F__, __L__);
		cur0_inf=&inf_set.disp_info[LX_PE_WIN_0];
		CHECK_KNULL(cur0_inf);
		/* res */
		if(PE_CHECK_WIN0(pstParams->win_id))
		{
			cur0_res_fmt = PE_SHP_HW_M14B_ConvDispInfoToResFmt(cur0_inf);
			PE_SHP_HW_M14B_DBG_PRINT_RES_FMT_STATE(pInfo->res0b_fmt,cur0_res_fmt);
			if(pInfo->res0b_fmt!=cur0_res_fmt)
			{
				#ifdef PE_HW_M14B_USE_REG_CTRL
				if(PE_KDRV_VER_M14CX)
				{
					switch(cur0_res_fmt)
					{
						case PE_SHP_M14B_RES_ATV:
							PE_SHP_HW_M14B_RES_REG_TABLE(shp_l_atv_default_m14c0,PE_REG_M14B_WRITE_BASE);
							break;
						case PE_SHP_M14B_RES_AV_480:
							PE_SHP_HW_M14B_RES_REG_TABLE(shp_l_av_480_default_m14c0,PE_REG_M14B_WRITE_BASE);
							break;
						case PE_SHP_M14B_RES_AV_576:
							PE_SHP_HW_M14B_RES_REG_TABLE(shp_l_av_576_default_m14c0,PE_REG_M14B_WRITE_BASE);
							break;
						case PE_SHP_M14B_RES_HD:
							PE_SHP_HW_M14B_RES_REG_TABLE(shp_l_hd_default_m14c0,PE_REG_M14B_WRITE_BASE);
							break;
						case PE_SHP_M14B_RES_PC:
							PE_SHP_HW_M14B_RES_REG_TABLE(shp_l_pc_default_m14c0,PE_REG_M14B_WRITE_BASE);
							break;
						case PE_SHP_M14B_RES_SD_480:
							PE_SHP_HW_M14B_RES_REG_TABLE(shp_l_sd_480_default_m14c0,PE_REG_M14B_WRITE_BASE);
							break;
						case PE_SHP_M14B_RES_SD_576:
							PE_SHP_HW_M14B_RES_REG_TABLE(shp_l_sd_576_default_m14c0,PE_REG_M14B_WRITE_BASE);
							break;
						default:
							break;
					}
				}
				else
				{
					switch(cur0_res_fmt)
					{
						case PE_SHP_M14B_RES_ATV:
							PE_SHP_HW_M14B_RES_REG_TABLE(shp_l_atv_default_m14b0,PE_REG_M14B_WRITE_BASE);
							break;
						case PE_SHP_M14B_RES_AV_480:
							PE_SHP_HW_M14B_RES_REG_TABLE(shp_l_av_480_default_m14b0,PE_REG_M14B_WRITE_BASE);
							break;
						case PE_SHP_M14B_RES_AV_576:
							PE_SHP_HW_M14B_RES_REG_TABLE(shp_l_av_576_default_m14b0,PE_REG_M14B_WRITE_BASE);
							break;
						case PE_SHP_M14B_RES_HD:
							PE_SHP_HW_M14B_RES_REG_TABLE(shp_l_hd_default_m14b0,PE_REG_M14B_WRITE_BASE);
							break;
						case PE_SHP_M14B_RES_PC:
							PE_SHP_HW_M14B_RES_REG_TABLE(shp_l_pc_default_m14b0,PE_REG_M14B_WRITE_BASE);
							break;
						case PE_SHP_M14B_RES_SD_480:
							PE_SHP_HW_M14B_RES_REG_TABLE(shp_l_sd_480_default_m14b0,PE_REG_M14B_WRITE_BASE);
							break;
						case PE_SHP_M14B_RES_SD_576:
							PE_SHP_HW_M14B_RES_REG_TABLE(shp_l_sd_576_default_m14b0,PE_REG_M14B_WRITE_BASE);
							break;
						default:
							break;
					}
				}
				#else
				switch(cur0_res_fmt)
				{
					case PE_SHP_M14B_RES_ATV:
						PE_SET_REG_TABLE(SHP_HW_M14,shp_l_atv_default_m14b0,PE_REG_M14B_WRITE_BASE);
						break;
					case PE_SHP_M14B_RES_AV_480:
						PE_SET_REG_TABLE(SHP_HW_M14,shp_l_av_480_default_m14b0,PE_REG_M14B_WRITE_BASE);
						break;
					case PE_SHP_M14B_RES_AV_576:
						PE_SET_REG_TABLE(SHP_HW_M14,shp_l_av_576_default_m14b0,PE_REG_M14B_WRITE_BASE);
						break;
					case PE_SHP_M14B_RES_HD:
						PE_SET_REG_TABLE(SHP_HW_M14,shp_l_hd_default_m14b0,PE_REG_M14B_WRITE_BASE);
						break;
					case PE_SHP_M14B_RES_PC:
						PE_SET_REG_TABLE(SHP_HW_M14,shp_l_pc_default_m14b0,PE_REG_M14B_WRITE_BASE);
						break;
					case PE_SHP_M14B_RES_SD_480:
						PE_SET_REG_TABLE(SHP_HW_M14,shp_l_sd_480_default_m14b0,PE_REG_M14B_WRITE_BASE);
						break;
					case PE_SHP_M14B_RES_SD_576:
						PE_SET_REG_TABLE(SHP_HW_M14,shp_l_sd_576_default_m14b0,PE_REG_M14B_WRITE_BASE);
						break;
					default:
						break;
				}
				#endif
				pInfo->res0b_fmt=cur0_res_fmt;
				/* reset the previous resolution enable to avoid overlapping. */
				pInfo->pre_res_en[LX_PE_WIN_0]=0xff;
			}
		}
		/* sre */
		if(PE_CHECK_WIN0(pstParams->win_id))
		{
			cur0_sre_fmt = PE_SHP_HW_M14B_ConvDispInfoToSreFmt(cur0_inf);
			PE_SHP_HW_M14B_DBG_PRINT_SRE_FMT_STATE(pInfo->sre0b_fmt,cur0_sre_fmt);
			if(pInfo->sre0b_fmt!=cur0_sre_fmt)
			{
				#ifdef PE_HW_M14B_USE_REG_CTRL
				switch(cur0_sre_fmt)
				{
					case PE_SHP_M14B_SRE_ATV:
						PE_SHP_HW_M14B_SRE_REG_TABLE(sre_l_atv_default_m14b0,PE_REG_M14B_WRITE_BASE);
						break;
					case PE_SHP_M14B_SRE_AV_NTSC:
						PE_SHP_HW_M14B_SRE_REG_TABLE(sre_l_av_ntsc_default_m14b0,PE_REG_M14B_WRITE_BASE);
						break;
					case PE_SHP_M14B_SRE_AV_PAL:
						PE_SHP_HW_M14B_SRE_REG_TABLE(sre_l_av_pal_default_m14b0,PE_REG_M14B_WRITE_BASE);
						break;
					case PE_SHP_M14B_SRE_DTV_HD:
						PE_SHP_HW_M14B_SRE_REG_TABLE(sre_l_dtv_hd_default_m14b0,PE_REG_M14B_WRITE_BASE);
						break;
					case PE_SHP_M14B_SRE_DTV_SD:
						PE_SHP_HW_M14B_SRE_REG_TABLE(sre_l_dtv_sd_default_m14b0,PE_REG_M14B_WRITE_BASE);
						break;
					case PE_SHP_M14B_SRE_HDMI_HD:
						PE_SHP_HW_M14B_SRE_REG_TABLE(sre_l_hdmi_hd_default_m14b0,PE_REG_M14B_WRITE_BASE);
						break;
					case PE_SHP_M14B_SRE_HDMI_SD:
						PE_SHP_HW_M14B_SRE_REG_TABLE(sre_l_hdmi_sd_default_m14b0,PE_REG_M14B_WRITE_BASE);
						break;
					case PE_SHP_M14B_SRE_PC:
						PE_SHP_HW_M14B_SRE_REG_TABLE(sre_l_pc_default_m14b0,PE_REG_M14B_WRITE_BASE);
						break;
					default:
						break;
				}
				#else
				switch(cur0_sre_fmt)
				{
					case PE_SHP_M14B_SRE_ATV:
						PE_SET_REG_TABLE(SHP_HW_M14,sre_l_atv_default_m14b0,		PE_REG_M14B_WRITE_BASE);
						break;
					case PE_SHP_M14B_SRE_AV_NTSC:
						PE_SET_REG_TABLE(SHP_HW_M14,sre_l_av_ntsc_default_m14b0,	PE_REG_M14B_WRITE_BASE);
						break;
					case PE_SHP_M14B_SRE_AV_PAL:
						PE_SET_REG_TABLE(SHP_HW_M14,sre_l_av_pal_default_m14b0,		PE_REG_M14B_WRITE_BASE);
						break;
					case PE_SHP_M14B_SRE_DTV_HD:
						PE_SET_REG_TABLE(SHP_HW_M14,sre_l_dtv_hd_default_m14b0,		PE_REG_M14B_WRITE_BASE);
						break;
					case PE_SHP_M14B_SRE_DTV_SD:
						PE_SET_REG_TABLE(SHP_HW_M14,sre_l_dtv_sd_default_m14b0,		PE_REG_M14B_WRITE_BASE);
						break;
					case PE_SHP_M14B_SRE_HDMI_HD:
						PE_SET_REG_TABLE(SHP_HW_M14,sre_l_hdmi_hd_default_m14b0,	PE_REG_M14B_WRITE_BASE);
						break;
					case PE_SHP_M14B_SRE_HDMI_SD:
						PE_SET_REG_TABLE(SHP_HW_M14,sre_l_hdmi_sd_default_m14b0,	PE_REG_M14B_WRITE_BASE);
						break;
					case PE_SHP_M14B_SRE_PC:
						PE_SET_REG_TABLE(SHP_HW_M14,sre_l_pc_default_m14b0,			PE_REG_M14B_WRITE_BASE);
						break;
					default:
						break;
				}
				#endif
				pInfo->sre0b_fmt=cur0_sre_fmt;
			}
		}
		/* sre lut */
		if(PE_CHECK_WIN0(pstParams->win_id))
		{
			cur0_srl_fmt = PE_SHP_HW_M14B_ConvDispInfoToSrlFmt(cur0_inf);
			PE_SHP_HW_M14B_DBG_PRINT_SRL_FMT_STATE(pInfo->srl0b_fmt,cur0_srl_fmt);
			if(pInfo->srl0b_fmt!=cur0_srl_fmt)
			{
				switch(cur0_srl_fmt)
				{
					case PE_SHP_M14B_SRL_HD:
						PE_SET_REG_TABLE(SHP_HW_M14,sre_l_lut_hd_default_ai_m14b0,	PE_REG_M14B_WRITE_BASE);
						break;
					case PE_SHP_M14B_SRL_SD:
						PE_SET_REG_TABLE(SHP_HW_M14,sre_l_lut_sd_default_ai_m14b0,	PE_REG_M14B_WRITE_BASE);
						break;
					default:
						break;
				}
				pInfo->srl0b_fmt=cur0_srl_fmt;
			}
		}
		/* vertical filter chroma */
		if(PE_CHECK_WIN0(pstParams->win_id))
		{
			cur0_vfc_fmt = PE_SHP_HW_M14B_ConvDispInfoToVfcFmt(cur0_inf);
			PE_SHP_HW_M14B_DBG_PRINT_VFC_FMT_STATE(pInfo->vfc0b_fmt,cur0_vfc_fmt);
			if(pInfo->vfc0b_fmt!=cur0_vfc_fmt)
			{
				#ifdef PE_HW_M14B_USE_REG_CTRL
				switch(cur0_vfc_fmt)
				{
					case PE_SHP_M14B_VFC_ATV_NT:
						PE_SHP_HW_M14B_VFC_REG_TABLE(vflt_c_l_atv_nt_default_m14b0,PE_REG_M14B_WRITE_BASE);
						break;
					case PE_SHP_M14B_VFC_ATV_PAL:
						PE_SHP_HW_M14B_VFC_REG_TABLE(vflt_c_l_atv_pal_default_m14b0,PE_REG_M14B_WRITE_BASE);
						break;
					case PE_SHP_M14B_VFC_AV:
						PE_SHP_HW_M14B_VFC_REG_TABLE(vflt_c_l_av_default_m14b0,PE_REG_M14B_WRITE_BASE);
						break;
					case PE_SHP_M14B_VFC_COMP_SD:
						PE_SHP_HW_M14B_VFC_REG_TABLE(vflt_c_l_comp_sd_default_m14b0,PE_REG_M14B_WRITE_BASE);
						break;
					case PE_SHP_M14B_VFC_DTV_SD:
						PE_SHP_HW_M14B_VFC_REG_TABLE(vflt_c_l_dtv_sd_default_m14b0,PE_REG_M14B_WRITE_BASE);
						break;
					case PE_SHP_M14B_VFC_HD:
						PE_SHP_HW_M14B_VFC_REG_TABLE(vflt_c_l_hd_default_m14b0,PE_REG_M14B_WRITE_BASE);
						break;
					case PE_SHP_M14B_VFC_HDMI_SD:
						PE_SHP_HW_M14B_VFC_REG_TABLE(vflt_c_l_hdmi_sd_default_m14b0,PE_REG_M14B_WRITE_BASE);
						break;
					case PE_SHP_M14B_VFC_PC:
						PE_SHP_HW_M14B_VFC_REG_TABLE(vflt_c_l_pc_default_m14b0,PE_REG_M14B_WRITE_BASE);
						break;
					case PE_SHP_M14B_VFC_SECAM:
						PE_SHP_HW_M14B_VFC_REG_TABLE(vflt_c_l_secam_default_m14b0,PE_REG_M14B_WRITE_BASE);
						break;
					default:
						break;
				}
				#else
				switch(cur0_vfc_fmt)
				{
					case PE_SHP_M14B_VFC_ATV_NT:
						PE_SET_REG_TABLE(SHP_HW_M14,vflt_c_l_atv_nt_default_m14b0,PE_REG_M14B_WRITE_BASE);
						break;
					case PE_SHP_M14B_VFC_ATV_PAL:
						PE_SET_REG_TABLE(SHP_HW_M14,vflt_c_l_atv_pal_default_m14b0,PE_REG_M14B_WRITE_BASE);
						break;
					case PE_SHP_M14B_VFC_AV:
						PE_SET_REG_TABLE(SHP_HW_M14,vflt_c_l_av_default_m14b0,PE_REG_M14B_WRITE_BASE);
						break;
					case PE_SHP_M14B_VFC_COMP_SD:
						PE_SET_REG_TABLE(SHP_HW_M14,vflt_c_l_comp_sd_default_m14b0,PE_REG_M14B_WRITE_BASE);
						break;
					case PE_SHP_M14B_VFC_DTV_SD:
						PE_SET_REG_TABLE(SHP_HW_M14,vflt_c_l_dtv_sd_default_m14b0,PE_REG_M14B_WRITE_BASE);
						break;
					case PE_SHP_M14B_VFC_HD:
						PE_SET_REG_TABLE(SHP_HW_M14,vflt_c_l_hd_default_m14b0,PE_REG_M14B_WRITE_BASE);
						break;
					case PE_SHP_M14B_VFC_HDMI_SD:
						PE_SET_REG_TABLE(SHP_HW_M14,vflt_c_l_hdmi_sd_default_m14b0,PE_REG_M14B_WRITE_BASE);
						break;
					case PE_SHP_M14B_VFC_PC:
						PE_SET_REG_TABLE(SHP_HW_M14,vflt_c_l_pc_default_m14b0,PE_REG_M14B_WRITE_BASE);
						break;
					case PE_SHP_M14B_VFC_SECAM:
						PE_SET_REG_TABLE(SHP_HW_M14,vflt_c_l_secam_default_m14b0,PE_REG_M14B_WRITE_BASE);
						break;
					default:
						break;
				}
				#endif
				pInfo->vfc0b_fmt=cur0_vfc_fmt;
			}
		}
		/* cti(p0d) */
		if(PE_CHECK_WIN0(pstParams->win_id))
		{
			cur0_cti_fmt = PE_SHP_HW_M14B_ConvDispInfoToCtiFmt(cur0_inf);
			PE_SHP_HW_M14B_DBG_PRINT_CTI_FMT_STATE(pInfo->cti0b_fmt,cur0_cti_fmt);
			if(pInfo->cti0b_fmt!=cur0_cti_fmt)
			{
				switch(cur0_cti_fmt)
				{
					case PE_SHP_M14B_CTI_ATV_NT:
						PE_SET_REG_TABLE(SHP_HW_M14,cti_p0d_l_atv_ntsc_default_m14b0,PE_REG_M14B_WRITE_BASE);
						break;
					case PE_SHP_M14B_CTI_ATV_PAL:
						PE_SET_REG_TABLE(SHP_HW_M14,cti_p0d_l_atv_pal_default_m14b0,PE_REG_M14B_WRITE_BASE);
						break;
					case PE_SHP_M14B_CTI_HD:
						PE_SET_REG_TABLE(SHP_HW_M14,cti_p0d_l_hd_default_m14b0,PE_REG_M14B_WRITE_BASE);
						break;
					case PE_SHP_M14B_CTI_PC:
						PE_SET_REG_TABLE(SHP_HW_M14,cti_p0d_l_pc_default_m14b0,PE_REG_M14B_WRITE_BASE);
						break;
					case PE_SHP_M14B_CTI_SD:
						PE_SET_REG_TABLE(SHP_HW_M14,cti_p0d_l_sd_default_m14b0,PE_REG_M14B_WRITE_BASE);
						break;
					default:
						break;
				}
				pInfo->cti0b_fmt=cur0_cti_fmt;
			}
		}
	}while(0);
	return ret;
}
/**
 * default shp setting(for M14AX)
 *
 * @param   *pstParams [in] LX_PE_DEFAULT_SETTINGS_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_SHP_HW_M14A_SetDefault(LX_PE_DEFAULT_SETTINGS_T *pstParams)
{
	int ret = RET_OK;
	PE_INF_M14_SETTINGS_T inf_set;
	LX_PE_INF_DISPLAY_T *cur0_inf = NULL;
	LX_PE_INF_DISPLAY_T *cur1_inf = NULL;
	PE_SHP_HW_M14A_RES_FMT cur0_res_fmt,cur1_res_fmt;
	PE_SHP_HW_M14A_VFC_FMT cur0_vfc_fmt,cur1_vfc_fmt;
	PE_SHP_HW_M14_SETTINGS_T *pInfo=&_g_pe_shp_hw_m14_info;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		ret = PE_INF_M14_GetCurInfSettings(&inf_set);
		PE_SHP_HW_M14_CHECK_CODE(ret, break, \
			"[%s,%d] PE_INF_M14_GetCurInfSettings() error.\n", __F__, __L__);
		cur0_inf=&inf_set.disp_info[LX_PE_WIN_0];
		cur1_inf=&inf_set.disp_info[LX_PE_WIN_1];
		CHECK_KNULL(cur0_inf);
		CHECK_KNULL(cur1_inf);
		/* L : resolution enhance */
		if(PE_CHECK_WIN0(pstParams->win_id))
		{
			cur0_res_fmt = PE_SHP_HW_M14A_ConvDispInfoToResFmt(cur0_inf);
			PE_SHP_HW_M14A_DBG_PRINT_RES_FMT_STATE(pInfo->res0a_fmt,cur0_res_fmt);
			if(pInfo->res0a_fmt!=cur0_res_fmt)
			{
				switch(cur0_res_fmt)
				{
					case PE_SHP_M14A_RES_RF_NTSC:
						PE_SET_REG_TABLE(SHP_HW_M14,shp_l_rf_ntsc_default_m14a0,	PE_REG_M14_WRITE_BASE);
						break;
					case PE_SHP_M14A_RES_RF_PAL:
						PE_SET_REG_TABLE(SHP_HW_M14,shp_l_rf_pal_default_m14a0,		PE_REG_M14_WRITE_BASE);
						break;
					case PE_SHP_M14A_RES_AV_NTSC:
						PE_SET_REG_TABLE(SHP_HW_M14,shp_l_av_ntsc_default_m14a0,	PE_REG_M14_WRITE_BASE);
						break;
					case PE_SHP_M14A_RES_AV_PAL:
						PE_SET_REG_TABLE(SHP_HW_M14,shp_l_av_pal_default_m14a0,		PE_REG_M14_WRITE_BASE);
						break;
					case PE_SHP_M14A_RES_PC:
						PE_SET_REG_TABLE(SHP_HW_M14,shp_l_pc_default_m14a0,			PE_REG_M14_WRITE_BASE);
						break;
					case PE_SHP_M14A_RES_DTV_SD:
						PE_SET_REG_TABLE(SHP_HW_M14,shp_l_dtv_sd_default_m14a0,		PE_REG_M14_WRITE_BASE);
						break;
					case PE_SHP_M14A_RES_DTV_HD:
						PE_SET_REG_TABLE(SHP_HW_M14,shp_l_dtv_hd_default_m14a0,		PE_REG_M14_WRITE_BASE);
						break;
					case PE_SHP_M14A_RES_COMP480I:
						PE_SET_REG_TABLE(SHP_HW_M14,shp_l_comp480i_default_m14a0,	PE_REG_M14_WRITE_BASE);
						break;
					case PE_SHP_M14A_RES_COMP480P:
						PE_SET_REG_TABLE(SHP_HW_M14,shp_l_comp480p_default_m14a0,	PE_REG_M14_WRITE_BASE);
						break;
					case PE_SHP_M14A_RES_HDMI480I:
						PE_SET_REG_TABLE(SHP_HW_M14,shp_l_hdmi480i_default_m14a0,	PE_REG_M14_WRITE_BASE);
						break;
					case PE_SHP_M14A_RES_HDMI480P:
						PE_SET_REG_TABLE(SHP_HW_M14,shp_l_hdmi480p_default_m14a0,	PE_REG_M14_WRITE_BASE);
						break;
					case PE_SHP_M14A_RES_HDMI_HD:
						PE_SET_REG_TABLE(SHP_HW_M14,shp_l_hdmi_hd_default_m14a0,	PE_REG_M14_WRITE_BASE);
						break;
					default:
						break;
				}
				pInfo->res0a_fmt=cur0_res_fmt;
				/* reset the previous resolution enable to avoid overlapping. */
				pInfo->pre_res_en[LX_PE_WIN_0]=0xff;
			}
		}
		/* R : resolution enhance */
		if(PE_CHECK_WIN1(pstParams->win_id))
		{
			cur1_res_fmt = PE_SHP_HW_M14A_ConvDispInfoToResFmt(cur1_inf);
			PE_SHP_HW_M14A_DBG_PRINT_RES_FMT_STATE(pInfo->res1a_fmt,cur1_res_fmt);
			if(pInfo->res1a_fmt!=cur1_res_fmt)
			{
				switch(cur1_res_fmt)
				{
					case PE_SHP_M14A_RES_RF_NTSC:
						PE_SET_REG_TABLE(SHP_HW_M14,shp_r_rf_ntsc_default_m14a0,	PE_REG_M14_WRITE_BASE);
						break;
					case PE_SHP_M14A_RES_RF_PAL:
						PE_SET_REG_TABLE(SHP_HW_M14,shp_r_rf_pal_default_m14a0,		PE_REG_M14_WRITE_BASE);
						break;
					case PE_SHP_M14A_RES_AV_NTSC:
						PE_SET_REG_TABLE(SHP_HW_M14,shp_r_av_ntsc_default_m14a0,	PE_REG_M14_WRITE_BASE);
						break;
					case PE_SHP_M14A_RES_AV_PAL:
						PE_SET_REG_TABLE(SHP_HW_M14,shp_r_av_pal_default_m14a0,		PE_REG_M14_WRITE_BASE);
						break;
					case PE_SHP_M14A_RES_PC:
						PE_SET_REG_TABLE(SHP_HW_M14,shp_r_pc_default_m14a0,			PE_REG_M14_WRITE_BASE);
						break;
					case PE_SHP_M14A_RES_DTV_SD:
						PE_SET_REG_TABLE(SHP_HW_M14,shp_r_dtv_sd_default_m14a0,		PE_REG_M14_WRITE_BASE);
						break;
					case PE_SHP_M14A_RES_DTV_HD:
						PE_SET_REG_TABLE(SHP_HW_M14,shp_r_dtv_hd_default_m14a0,		PE_REG_M14_WRITE_BASE);
						break;
					case PE_SHP_M14A_RES_COMP480I:
						PE_SET_REG_TABLE(SHP_HW_M14,shp_r_comp480i_default_m14a0,	PE_REG_M14_WRITE_BASE);
						break;
					case PE_SHP_M14A_RES_COMP480P:
						PE_SET_REG_TABLE(SHP_HW_M14,shp_r_comp480p_default_m14a0,	PE_REG_M14_WRITE_BASE);
						break;
					case PE_SHP_M14A_RES_HDMI480I:
						PE_SET_REG_TABLE(SHP_HW_M14,shp_r_hdmi480i_default_m14a0,	PE_REG_M14_WRITE_BASE);
						break;
					case PE_SHP_M14A_RES_HDMI480P:
						PE_SET_REG_TABLE(SHP_HW_M14,shp_r_hdmi480p_default_m14a0,	PE_REG_M14_WRITE_BASE);
						break;
					case PE_SHP_M14A_RES_HDMI_HD:
						PE_SET_REG_TABLE(SHP_HW_M14,shp_r_hdmi_hd_default_m14a0,	PE_REG_M14_WRITE_BASE);
						break;
					default:
						break;
				}
				pInfo->res1a_fmt=cur1_res_fmt;
			}
		}
		/* L : vertical filter chroma */
		if(PE_CHECK_WIN0(pstParams->win_id))
		{
			cur0_vfc_fmt = PE_SHP_HW_M14A_ConvDispInfoToVfcFmt(cur0_inf);
			PE_SHP_HW_M14A_DBG_PRINT_VFC_FMT_STATE(pInfo->vfc0a_fmt,cur0_vfc_fmt);
			if(pInfo->vfc0a_fmt!=cur0_vfc_fmt)
			{
				switch(cur0_vfc_fmt)
				{
					case PE_SHP_M14A_VFC_HD:
						PE_SET_REG_TABLE(SHP_HW_M14,vflt_c_l_hd_default_m14a0,				PE_REG_M14_WRITE_BASE);
						break;
					case PE_SHP_M14A_VFC_PC:
						PE_SET_REG_TABLE(SHP_HW_M14,vflt_c_l_pc_default_m14a0,				PE_REG_M14_WRITE_BASE);
						break;
					case PE_SHP_M14A_VFC_SD_AV:
						PE_SET_REG_TABLE(SHP_HW_M14,vflt_c_l_sd_av_default_m14a0,			PE_REG_M14_WRITE_BASE);
						break;
					case PE_SHP_M14A_VFC_SD_DTV:
						PE_SET_REG_TABLE(SHP_HW_M14,vflt_c_l_sd_dtv_default_m14a0,			PE_REG_M14_WRITE_BASE);
						break;
					case PE_SHP_M14A_VFC_SD_AVRF_SECAM:
						PE_SET_REG_TABLE(SHP_HW_M14,vflt_c_l_sd_avrf_secam_default_m14a0,	PE_REG_M14_WRITE_BASE);
						break;
					case PE_SHP_M14A_VFC_SD_HDMI:
						PE_SET_REG_TABLE(SHP_HW_M14,vflt_c_l_sd_hdmi_default_m14a0,			PE_REG_M14_WRITE_BASE);
						break;
					case PE_SHP_M14A_VFC_SD_RF_NT:
						PE_SET_REG_TABLE(SHP_HW_M14,vflt_c_l_sd_rf_nt_default_m14a0,		PE_REG_M14_WRITE_BASE);
						break;
					case PE_SHP_M14A_VFC_SD_RF_PAL:
						PE_SET_REG_TABLE(SHP_HW_M14,vflt_c_l_sd_rf_pal_default_m14a0,		PE_REG_M14_WRITE_BASE);
						break;
					default:
						break;
				}
				pInfo->vfc0a_fmt=cur0_vfc_fmt;
			}
		}
		/* R : vertical filter chroma */
		if(PE_CHECK_WIN1(pstParams->win_id))
		{
			cur1_vfc_fmt = PE_SHP_HW_M14A_ConvDispInfoToVfcFmt(cur1_inf);
			PE_SHP_HW_M14A_DBG_PRINT_VFC_FMT_STATE(pInfo->vfc1a_fmt,cur1_vfc_fmt);
			if(pInfo->vfc1a_fmt!=cur1_vfc_fmt)
			{
				switch(cur1_vfc_fmt)
				{
					case PE_SHP_M14A_VFC_HD:
						PE_SET_REG_TABLE(SHP_HW_M14,vflt_c_l_hd_default_m14a0,				PE_MSC_REG_M14_LRDIFF);
						break;
					case PE_SHP_M14A_VFC_PC:
						PE_SET_REG_TABLE(SHP_HW_M14,vflt_c_l_pc_default_m14a0,				PE_MSC_REG_M14_LRDIFF);
						break;
					case PE_SHP_M14A_VFC_SD_AV:
						PE_SET_REG_TABLE(SHP_HW_M14,vflt_c_l_sd_av_default_m14a0,			PE_MSC_REG_M14_LRDIFF);
						break;
					case PE_SHP_M14A_VFC_SD_DTV:
						PE_SET_REG_TABLE(SHP_HW_M14,vflt_c_l_sd_dtv_default_m14a0,			PE_MSC_REG_M14_LRDIFF);
						break;
					case PE_SHP_M14A_VFC_SD_AVRF_SECAM:
						PE_SET_REG_TABLE(SHP_HW_M14,vflt_c_l_sd_avrf_secam_default_m14a0,	PE_MSC_REG_M14_LRDIFF);
						break;
					case PE_SHP_M14A_VFC_SD_HDMI:
						PE_SET_REG_TABLE(SHP_HW_M14,vflt_c_l_sd_hdmi_default_m14a0,			PE_MSC_REG_M14_LRDIFF);
						break;
					case PE_SHP_M14A_VFC_SD_RF_NT:
						PE_SET_REG_TABLE(SHP_HW_M14,vflt_c_l_sd_rf_nt_default_m14a0,		PE_MSC_REG_M14_LRDIFF);
						break;
					case PE_SHP_M14A_VFC_SD_RF_PAL:
						PE_SET_REG_TABLE(SHP_HW_M14,vflt_c_l_sd_rf_pal_default_m14a0,		PE_MSC_REG_M14_LRDIFF);
						break;
					default:
						break;
				}
				pInfo->vfc1a_fmt=cur1_vfc_fmt;
			}
		}
	}while(0);
	return ret;
}
/**
 * get resolution fmt using display info(for M14BX)
 *
 * @param   *disp_inf [in] LX_PE_INF_DISPLAY_T
 * @return  PE_SHP_HW_M14B_RES_FMT.
 * @see
 * @author
 */
static PE_SHP_HW_M14B_RES_FMT PE_SHP_HW_M14B_ConvDispInfoToResFmt(LX_PE_INF_DISPLAY_T *disp_inf)
{
	PE_SHP_HW_M14B_RES_FMT res_fmt = PE_SHP_M14B_RES_NUM;
	switch(disp_inf->src_type)
	{
		case LX_PE_SRC_ATV:
			{
				res_fmt = PE_SHP_M14B_RES_ATV;
			}
			break;
		case LX_PE_SRC_CVBS:
		case LX_PE_SRC_SCART:
			if(disp_inf->cstd_type==LX_PE_CSTD_NTSC)
			{
				res_fmt = PE_SHP_M14B_RES_AV_480;
			}
			else
			{
				res_fmt = PE_SHP_M14B_RES_AV_576;
			}
			break;
		case LX_PE_SRC_RGBPC:
			{
				res_fmt = PE_SHP_M14B_RES_PC;
			}
			break;
		case LX_PE_SRC_DTV:
		case LX_PE_SRC_COMP:
		case LX_PE_SRC_HDMI:
		default:
			if(disp_inf->src_type==LX_PE_SRC_HDMI && \
				disp_inf->hdmi_type==LX_PE_HDMI_PC)
			{
				res_fmt = PE_SHP_M14B_RES_PC;
			}
			else if(disp_inf->fmt_type==LX_PE_FMT_SD)
			{
				if(disp_inf->fr_type==LX_PE_FR_60HZ)
				{
					res_fmt = PE_SHP_M14B_RES_SD_480;
				}
				else
				{
					res_fmt = PE_SHP_M14B_RES_SD_576;
				}
			}
			else
			{
				res_fmt = PE_SHP_M14B_RES_HD;
			}
			break;
	}
	return res_fmt;
}
/**
 * get sre fmt using display info(for M14BX)
 *
 * @param   *disp_inf [in] LX_PE_INF_DISPLAY_T
 * @return  PE_SHP_HW_M14B_SRE_FMT.
 * @see
 * @author
 */
static PE_SHP_HW_M14B_SRE_FMT PE_SHP_HW_M14B_ConvDispInfoToSreFmt(LX_PE_INF_DISPLAY_T *disp_inf)
{
	PE_SHP_HW_M14B_SRE_FMT sre_fmt = PE_SHP_M14B_SRE_NUM;
	switch(disp_inf->src_type)
	{
		case LX_PE_SRC_ATV:
			{
				sre_fmt = PE_SHP_M14B_SRE_ATV;
			}
			break;
		case LX_PE_SRC_CVBS:
		case LX_PE_SRC_SCART:
			if(disp_inf->cstd_type==LX_PE_CSTD_NTSC)
			{
				sre_fmt = PE_SHP_M14B_SRE_AV_NTSC;
			}
			else
			{
				sre_fmt = PE_SHP_M14B_SRE_AV_PAL;
			}
			break;
		case LX_PE_SRC_RGBPC:
			{
				sre_fmt = PE_SHP_M14B_SRE_PC;
			}
			break;
		case LX_PE_SRC_DTV:
			if(disp_inf->fmt_type==LX_PE_FMT_SD)
			{
				sre_fmt = PE_SHP_M14B_SRE_DTV_SD;
			}
			else
			{
				sre_fmt = PE_SHP_M14B_SRE_DTV_HD;
			}
			break;
		case LX_PE_SRC_COMP:
		case LX_PE_SRC_HDMI:
		default:
			if(disp_inf->src_type==LX_PE_SRC_HDMI && \
				disp_inf->hdmi_type==LX_PE_HDMI_PC)
			{
				sre_fmt = PE_SHP_M14B_SRE_PC;
			}
			else if(disp_inf->fmt_type==LX_PE_FMT_SD)
			{
				sre_fmt = PE_SHP_M14B_SRE_HDMI_SD;
			}
			else
			{
				sre_fmt = PE_SHP_M14B_SRE_HDMI_HD;
			}
			break;
	}
	return sre_fmt;
}
/**
 * get sre lut fmt using display info(for M14BX)
 *
 * @param   *disp_inf [in] LX_PE_INF_DISPLAY_T
 * @return  PE_SHP_HW_M14B_SRL_FMT.
 * @see
 * @author
 */
static PE_SHP_HW_M14B_SRL_FMT PE_SHP_HW_M14B_ConvDispInfoToSrlFmt(LX_PE_INF_DISPLAY_T *disp_inf)
{
	PE_SHP_HW_M14B_SRL_FMT rel_fmt = PE_SHP_M14B_SRL_NUM;
	switch(disp_inf->src_type)
	{
		case LX_PE_SRC_ATV:
		case LX_PE_SRC_CVBS:
		case LX_PE_SRC_SCART:
		case LX_PE_SRC_RGBPC:
			{
				rel_fmt = PE_SHP_M14B_SRL_SD;
			}
			break;
		case LX_PE_SRC_DTV:
		case LX_PE_SRC_COMP:
		case LX_PE_SRC_HDMI:
		default:
			if(disp_inf->fmt_type==LX_PE_FMT_SD)
			{
				rel_fmt = PE_SHP_M14B_SRL_SD;
			}
			else
			{
				rel_fmt = PE_SHP_M14B_SRL_HD;
			}
			break;
	}
	return rel_fmt;
}
/**
 * get vertical filter(chroma) fmt using display info(for M14BX)
 *
 * @param   *disp_inf [in] LX_PE_INF_DISPLAY_T
 * @return  PE_SHP_HW_M14B_VFC_FMT
 * @see
 * @author
 */
static PE_SHP_HW_M14B_VFC_FMT PE_SHP_HW_M14B_ConvDispInfoToVfcFmt(LX_PE_INF_DISPLAY_T *disp_inf)
{
	PE_SHP_HW_M14B_VFC_FMT vfc_fmt = PE_SHP_M14B_VFC_NUM;
	switch(disp_inf->src_type)
	{
		case LX_PE_SRC_ATV:
			if(disp_inf->cstd_type==LX_PE_CSTD_SECAM)
			{
				vfc_fmt = PE_SHP_M14B_VFC_SECAM;
			}
			else if(disp_inf->cstd_type==LX_PE_CSTD_PAL)
			{
				vfc_fmt = PE_SHP_M14B_VFC_ATV_PAL;
			}
			else
			{
				vfc_fmt = PE_SHP_M14B_VFC_ATV_NT;
			}
			break;
		case LX_PE_SRC_CVBS:
		case LX_PE_SRC_SCART:
			if(disp_inf->cstd_type==LX_PE_CSTD_SECAM)
			{
				vfc_fmt = PE_SHP_M14B_VFC_SECAM;
			}
			else
			{
				vfc_fmt = PE_SHP_M14B_VFC_AV;
			}
			break;
		case LX_PE_SRC_RGBPC:
			{
				vfc_fmt = PE_SHP_M14B_VFC_PC;
			}
			break;
		case LX_PE_SRC_DTV:
			if(disp_inf->dtv_type==LX_PE_DTV_HDDPLAY)
			{
				if(disp_inf->hdd_type==LX_PE_HDD_SRC_ATV60)
				{
					vfc_fmt = PE_SHP_M14B_VFC_ATV_NT;
				}
				else if(disp_inf->hdd_type==LX_PE_HDD_SRC_ATV50)
				{
					vfc_fmt = PE_SHP_M14B_VFC_ATV_PAL;
				}
				else if(disp_inf->hdd_type==LX_PE_HDD_SRC_AV60 || \
					disp_inf->hdd_type==LX_PE_HDD_SRC_AV50 || \
					disp_inf->hdd_type==LX_PE_HDD_SRC_SCARTRGB)
				{
					vfc_fmt = PE_SHP_M14B_VFC_AV;
				}
				else	//LX_PE_HDD_SRC_DTV
				{
					if(disp_inf->fmt_type==LX_PE_FMT_SD)
					{
						vfc_fmt = PE_SHP_M14B_VFC_DTV_SD;
					}
					else
					{
						vfc_fmt = PE_SHP_M14B_VFC_HD;
					}
				}
			}
			else if(disp_inf->fmt_type==LX_PE_FMT_SD)
			{
				vfc_fmt = PE_SHP_M14B_VFC_DTV_SD;
			}
			else
			{
				vfc_fmt = PE_SHP_M14B_VFC_HD;
			}
			break;
		case LX_PE_SRC_COMP:
			if(disp_inf->fmt_type==LX_PE_FMT_SD)
			{
				vfc_fmt = PE_SHP_M14B_VFC_COMP_SD;
			}
			else
			{
				vfc_fmt = PE_SHP_M14B_VFC_HD;
			}
			break;
		case LX_PE_SRC_HDMI:
		default:
			if(disp_inf->hdmi_type==LX_PE_HDMI_PC)
			{
				vfc_fmt = PE_SHP_M14B_VFC_PC;
			}
			else if(disp_inf->fmt_type==LX_PE_FMT_SD)
			{
				vfc_fmt = PE_SHP_M14B_VFC_HDMI_SD;
			}
			else
			{
				vfc_fmt = PE_SHP_M14B_VFC_HD;
			}
			break;
	}
	return vfc_fmt;
}
/**
 * get cti(p0d) fmt using display info(for M14BX)
 *
 * @param   *disp_inf [in] LX_PE_INF_DISPLAY_T
 * @return  PE_SHP_HW_M14B_CTI_FMT
 * @see
 * @author
 */
static PE_SHP_HW_M14B_CTI_FMT PE_SHP_HW_M14B_ConvDispInfoToCtiFmt(LX_PE_INF_DISPLAY_T *disp_inf)
{
	PE_SHP_HW_M14B_CTI_FMT cti_fmt = PE_SHP_M14B_CTI_NUM;
	switch(disp_inf->src_type)
	{
		case LX_PE_SRC_ATV:
			if(disp_inf->cstd_type==LX_PE_CSTD_NTSC)
			{
				cti_fmt = PE_SHP_M14B_CTI_ATV_NT;
			}
			else
			{
				cti_fmt = PE_SHP_M14B_CTI_ATV_PAL;
			}
			break;
		case LX_PE_SRC_CVBS:
		case LX_PE_SRC_SCART:
			{
				cti_fmt = PE_SHP_M14B_CTI_SD;
			}
			break;
		case LX_PE_SRC_RGBPC:
			{
				cti_fmt = PE_SHP_M14B_CTI_PC;
			}
			break;
		case LX_PE_SRC_DTV:
		case LX_PE_SRC_COMP:
		case LX_PE_SRC_HDMI:
		default:
			if(disp_inf->src_type==LX_PE_SRC_HDMI && \
				disp_inf->hdmi_type==LX_PE_HDMI_PC)
			{
				cti_fmt = PE_SHP_M14B_CTI_PC;
			}
			else if(disp_inf->fmt_type==LX_PE_FMT_SD)
			{
				cti_fmt = PE_SHP_M14B_CTI_SD;
			}
			else
			{
				cti_fmt = PE_SHP_M14B_CTI_HD;
			}
			break;
	}
	return cti_fmt;
}
/**
 * get resolution fmt using display info(for M14AX)
 *
 * @param   *disp_inf [in] LX_PE_INF_DISPLAY_T
 * @return  PE_SHP_HW_M14A_RES_FMT.
 * @see
 * @author
 */
static PE_SHP_HW_M14A_RES_FMT PE_SHP_HW_M14A_ConvDispInfoToResFmt(LX_PE_INF_DISPLAY_T *disp_inf)
{
	PE_SHP_HW_M14A_RES_FMT res_fmt = PE_SHP_M14A_RES_NUM;
	switch(disp_inf->src_type)
	{
		case LX_PE_SRC_ATV:
			if(disp_inf->cstd_type==LX_PE_CSTD_NTSC)
			{
				res_fmt = PE_SHP_M14A_RES_RF_NTSC;
			}
			else
			{
				res_fmt = PE_SHP_M14A_RES_RF_PAL;
			}
			break;
		case LX_PE_SRC_CVBS:
		case LX_PE_SRC_SCART:
			if(disp_inf->cstd_type==LX_PE_CSTD_NTSC)
			{
				res_fmt = PE_SHP_M14A_RES_AV_NTSC;
			}
			else
			{
				res_fmt = PE_SHP_M14A_RES_AV_PAL;
			}
			break;
		case LX_PE_SRC_RGBPC:
			{
				res_fmt = PE_SHP_M14A_RES_PC;
			}
			break;
		case LX_PE_SRC_DTV:
			if(disp_inf->fmt_type==LX_PE_FMT_SD)
			{
				res_fmt = PE_SHP_M14A_RES_DTV_SD;
			}
			else
			{
				res_fmt = PE_SHP_M14A_RES_DTV_HD;
			}
			break;
		case LX_PE_SRC_COMP:
			if(disp_inf->fmt_type==LX_PE_FMT_SD)
			{
				if(disp_inf->scan_type==LX_PE_SCAN_INTERLACE)
				{
					res_fmt = PE_SHP_M14A_RES_COMP480I;
				}
				else
				{
					res_fmt = PE_SHP_M14A_RES_COMP480P;
				}
			}
			else
			{
				res_fmt = PE_SHP_M14A_RES_HDMI_HD;
			}
			break;
		case LX_PE_SRC_HDMI:
		default:
			if(disp_inf->hdmi_type==LX_PE_HDMI_PC)
			{
				res_fmt = PE_SHP_M14A_RES_PC;
			}
			else if(disp_inf->fmt_type==LX_PE_FMT_SD)
			{
				if(disp_inf->scan_type==LX_PE_SCAN_INTERLACE)
				{
					res_fmt = PE_SHP_M14A_RES_HDMI480I;
				}
				else
				{
					res_fmt = PE_SHP_M14A_RES_HDMI480P;
				}
			}
			else
			{
				res_fmt = PE_SHP_M14A_RES_HDMI_HD;
			}
			break;
	}
	return res_fmt;
}
/**
 * get vertical filter(chroma) fmt using display info(for M14AX)
 *
 * @param   *disp_inf [in] LX_PE_INF_DISPLAY_T
 * @return  PE_SHP_HW_M14A_VFC_FMT
 * @see
 * @author
 */
static PE_SHP_HW_M14A_VFC_FMT PE_SHP_HW_M14A_ConvDispInfoToVfcFmt(LX_PE_INF_DISPLAY_T *disp_inf)
{
	PE_SHP_HW_M14A_VFC_FMT vfc_fmt = PE_SHP_M14A_VFC_NUM;
	switch(disp_inf->src_type)
	{
		case LX_PE_SRC_ATV:
			if(disp_inf->cstd_type==LX_PE_CSTD_SECAM)
			{
				vfc_fmt = PE_SHP_M14A_VFC_SD_AVRF_SECAM;
			}
			else if(disp_inf->cstd_type==LX_PE_CSTD_PAL)
			{
				vfc_fmt = PE_SHP_M14A_VFC_SD_RF_PAL;
			}
			else
			{
				vfc_fmt = PE_SHP_M14A_VFC_SD_RF_NT;
			}
			break;
		case LX_PE_SRC_CVBS:
		case LX_PE_SRC_SCART:
			if(disp_inf->cstd_type==LX_PE_CSTD_SECAM)
			{
				vfc_fmt = PE_SHP_M14A_VFC_SD_AVRF_SECAM;
			}
			else
			{
				vfc_fmt = PE_SHP_M14A_VFC_SD_AV;
			}
			break;
		case LX_PE_SRC_RGBPC:
			{
				vfc_fmt = PE_SHP_M14A_VFC_PC;
			}
			break;
		case LX_PE_SRC_DTV:
			if(disp_inf->fmt_type==LX_PE_FMT_SD)
			{
				vfc_fmt = PE_SHP_M14A_VFC_SD_DTV;
			}
			else
			{
				vfc_fmt = PE_SHP_M14A_VFC_HD;
			}
			break;
		case LX_PE_SRC_COMP:
			if(disp_inf->fmt_type==LX_PE_FMT_SD)
			{
				vfc_fmt = PE_SHP_M14A_VFC_SD_AV;
			}
			else
			{
				vfc_fmt = PE_SHP_M14A_VFC_HD;
			}
			break;
		case LX_PE_SRC_HDMI:
		default:
			if(disp_inf->hdmi_type==LX_PE_HDMI_PC)
			{
				vfc_fmt = PE_SHP_M14A_VFC_PC;
			}
			else if(disp_inf->fmt_type==LX_PE_FMT_SD)
			{
				vfc_fmt = PE_SHP_M14A_VFC_SD_HDMI;
			}
			else
			{
				vfc_fmt = PE_SHP_M14A_VFC_HD;
			}
			break;
	}
	return vfc_fmt;
}
/**
 * set(store) scaler filter coeff info
 *
 * @param   *pstParams [in] LX_PE_SHP_SCLFILTER_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_SHP_HW_M14_SetScalerFilterCoeff(LX_PE_SHP_SCLFILTER_T *pstParams)
{
	int ret = RET_OK;
	PE_SHP_HW_M14_SETTINGS_T *pInfo=&_g_pe_shp_hw_m14_info;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		PE_SHP_HW_M14_DBG_PRINT("set[%d]:cur idx_y:%d,%d,idx_c:%d,%d\n", \
			pstParams->win_id, pstParams->h_luma_id, pstParams->v_luma_id, \
			pstParams->h_chrm_id,pstParams->v_chrm_id);
		if(PE_KDRV_VER_M14)
		{
			if(PE_CHECK_WIN0(pstParams->win_id))
			{
				memcpy(&pInfo->cur_scl_idx[LX_PE_WIN_0],pstParams,sizeof(LX_PE_SHP_SCLFILTER_T));
			}
			if(PE_CHECK_WIN1(pstParams->win_id))
			{
				memcpy(&pInfo->cur_scl_idx[LX_PE_WIN_1],pstParams,sizeof(LX_PE_SHP_SCLFILTER_T));
			}
		}
		else
		{
			PE_SHP_HW_M14_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * get(store) scaler filter coeff info
 *
 * @param   *pstParams [in] LX_PE_SHP_SCLFILTER_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_SHP_HW_M14_GetScalerFilterCoeff(LX_PE_SHP_SCLFILTER_T *pstParams)
{
	int ret = RET_OK;
	LX_PE_WIN_ID win_id,temp;
	PE_SHP_HW_M14_SETTINGS_T *pInfo=&_g_pe_shp_hw_m14_info;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		win_id = PE_GET_CHECKED_WINID(pstParams->win_id);
		if(PE_KDRV_VER_M14)
		{
			if(PE_CHECK_WIN0(win_id))
			{
				temp = pstParams->win_id;
				memcpy(pstParams,&pInfo->cur_scl_idx[LX_PE_WIN_0],sizeof(LX_PE_SHP_SCLFILTER_T));
				pstParams->win_id = temp;
			}
			if(PE_CHECK_WIN1(win_id))
			{
				temp = pstParams->win_id;
				memcpy(pstParams,&pInfo->cur_scl_idx[LX_PE_WIN_1],sizeof(LX_PE_SHP_SCLFILTER_T));
				pstParams->win_id = temp;
			}
		}
		else
		{
			PE_SHP_HW_M14_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
		PE_SHP_HW_M14_DBG_PRINT("get[%d]:cur idx_y:%d,%d,idx_c:%d,%d\n", \
			pstParams->win_id, pstParams->h_luma_id, pstParams->v_luma_id, \
			pstParams->h_chrm_id,pstParams->v_chrm_id);
	}while(0);
	return ret;
}
/**
 * set scaler filter control periodically
 *
 * @param   *pstParams [in] PE_SHP_HW_M14_SETTINGS_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_SHP_HW_M14_SetScalerFilterCtrl(PE_TSK_M14_CFG_T *pCfg)
{
	int ret = RET_OK;
	PE_SHP_HW_M14_SETTINGS_T cur_param;
	PE_SHP_HW_M14_SETTINGS_T *pInfo=&_g_pe_shp_hw_m14_info;
	PE_SHP_HW_M14_FLAG_T set_flag;
	UINT32 type_sel_luma,type_sel_chroma,scaler_config,adap_filter_en;
	do{
		CHECK_KNULL(pCfg);
		if(PE_KDRV_VER_M14BX)
		{
			memset0(cur_param);
			/* get fir,tap,index */
			ret = PE_SHP_HW_M14_GetScalerFilterValue(pCfg,&cur_param);
			PE_SHP_HW_M14_CHECK_CODE(ret, break, "[%s,%d] PE_SHP_HW_M14_GetScalerFilterValue() error.\n", __F__, __L__);
			/* check index is valid & re-setting index */
			PE_SHP_HW_M14_CheckValidScalerFilterValue(&cur_param);
			/* L : compare & filtering */
			if(pInfo->pre_scl_idx[LX_PE_WIN_0].h_luma_id!=cur_param.cur_scl_idx[LX_PE_WIN_0].h_luma_id || \
				pInfo->pre_scl_idx[LX_PE_WIN_0].v_luma_id!=cur_param.cur_scl_idx[LX_PE_WIN_0].v_luma_id || \
				pInfo->pre_scl_idx[LX_PE_WIN_0].h_chrm_id!=cur_param.cur_scl_idx[LX_PE_WIN_0].h_chrm_id || \
				pInfo->pre_scl_idx[LX_PE_WIN_0].v_chrm_id!=cur_param.cur_scl_idx[LX_PE_WIN_0].v_chrm_id || \
				pInfo->h_y_tap[LX_PE_WIN_0]!=cur_param.h_y_tap[LX_PE_WIN_0] || \
				pInfo->v_y_tap[LX_PE_WIN_0]!=cur_param.v_y_tap[LX_PE_WIN_0] || \
				pInfo->h_c_tap[LX_PE_WIN_0]!=cur_param.h_c_tap[LX_PE_WIN_0] || \
				pInfo->v_c_tap[LX_PE_WIN_0]!=cur_param.v_c_tap[LX_PE_WIN_0] || \
				pInfo->y_fir[LX_PE_WIN_0]!=cur_param.y_fir[LX_PE_WIN_0] || \
				pInfo->c_fir[LX_PE_WIN_0]!=cur_param.c_fir[LX_PE_WIN_0])
			{
				PE_SHP_HW_M14_DBG_PRINT("set[0]:[idx_hy,vy,hc,vc][pre:%d,%d,%d,%d][cur:%d,%d,%d,%d]\n",\
					pInfo->pre_scl_idx[LX_PE_WIN_0].h_luma_id,pInfo->pre_scl_idx[LX_PE_WIN_0].v_luma_id,\
					pInfo->pre_scl_idx[LX_PE_WIN_0].h_chrm_id,pInfo->pre_scl_idx[LX_PE_WIN_0].v_chrm_id,\
					cur_param.cur_scl_idx[LX_PE_WIN_0].h_luma_id,cur_param.cur_scl_idx[LX_PE_WIN_0].v_luma_id,\
					cur_param.cur_scl_idx[LX_PE_WIN_0].h_chrm_id,cur_param.cur_scl_idx[LX_PE_WIN_0].v_chrm_id);
				PE_SHP_HW_M14_DBG_PRINT("set[0]:[tap_hy,vy,hc,vc][pre:%d,%d,%d,%d][cur:%d,%d,%d,%d]\n",\
					pInfo->h_y_tap[LX_PE_WIN_0],pInfo->v_y_tap[LX_PE_WIN_0],\
					pInfo->h_c_tap[LX_PE_WIN_0],pInfo->v_c_tap[LX_PE_WIN_0],\
					cur_param.h_y_tap[LX_PE_WIN_0],cur_param.v_y_tap[LX_PE_WIN_0],\
					cur_param.h_c_tap[LX_PE_WIN_0],cur_param.v_c_tap[LX_PE_WIN_0]);
				PE_SHP_HW_M14_DBG_PRINT("set[0]:[fir_y,c][pre:%d,%d][cur:%d,%d]\n",\
					pInfo->y_fir[LX_PE_WIN_0],pInfo->c_fir[LX_PE_WIN_0],\
					cur_param.y_fir[LX_PE_WIN_0],cur_param.c_fir[LX_PE_WIN_0]);
				/* set config */
				if(pInfo->h_y_tap[LX_PE_WIN_0]!=cur_param.h_y_tap[LX_PE_WIN_0] || \
					pInfo->y_fir[LX_PE_WIN_0]!=cur_param.y_fir[LX_PE_WIN_0] || \
					pInfo->c_fir[LX_PE_WIN_0]!=cur_param.c_fir[LX_PE_WIN_0])
				{
					scaler_config	= (cur_param.h_y_tap[LX_PE_WIN_0]==PE_SHP_HW_M14_SCL_V12T_NORM || \
										cur_param.h_y_tap[LX_PE_WIN_0]==PE_SHP_HW_M14_SCL_V12T_DN14)? 0x0:0x1;
					adap_filter_en	= (cur_param.h_y_tap[LX_PE_WIN_0]==PE_SHP_HW_M14_SCL_V12T_NORM)? 0x1:0x0;
					type_sel_luma	= (cur_param.y_fir[LX_PE_WIN_0]==PE_SHP_HW_M14_FIR_BILINEAR)? 0x1:0x0;
					type_sel_chroma	= (cur_param.c_fir[LX_PE_WIN_0]==PE_SHP_HW_M14_FIR_BILINEAR)? 0x1:0x0;
			        PE_MSC_M14B0_RdFL(msc_ctrl);
			        PE_MSC_M14B0_Wr01(msc_ctrl,	filter_type_sel_luma,	type_sel_luma);
			        PE_MSC_M14B0_Wr01(msc_ctrl,	filter_type_sel_chroma,	type_sel_chroma);
			        PE_MSC_M14B0_Wr01(msc_ctrl,	v_scaler_config,		scaler_config);
			        PE_MSC_M14B0_Wr01(msc_ctrl,	adaptive_fir_filter_en,	adap_filter_en);
			        PE_MSC_M14B0_WrFL(msc_ctrl);
				}
				/* check double setting flag*/
				set_flag = PE_SHP_HW_M14_GetDoubleSetFlag(LX_PE_WIN_0,pInfo->pre_scl_idx[LX_PE_WIN_0],&cur_param);
				/* set scl coeff */
				ret = PE_SHP_HW_M14_SetSclCoeff(LX_PE_WIN_0,set_flag,&cur_param);
				PE_SHP_HW_M14_CHECK_CODE(ret, break, "[%s,%d] PE_SHP_HW_M14_SetSclCoeff() error.\n", __F__, __L__);
				pInfo->pre_scl_idx[LX_PE_WIN_0].h_luma_id=cur_param.cur_scl_idx[LX_PE_WIN_0].h_luma_id;
				pInfo->pre_scl_idx[LX_PE_WIN_0].v_luma_id=cur_param.cur_scl_idx[LX_PE_WIN_0].v_luma_id;
				pInfo->pre_scl_idx[LX_PE_WIN_0].h_chrm_id=cur_param.cur_scl_idx[LX_PE_WIN_0].h_chrm_id;
				pInfo->pre_scl_idx[LX_PE_WIN_0].v_chrm_id=cur_param.cur_scl_idx[LX_PE_WIN_0].v_chrm_id;
				pInfo->h_y_tap[LX_PE_WIN_0]=cur_param.h_y_tap[LX_PE_WIN_0];
				pInfo->v_y_tap[LX_PE_WIN_0]=cur_param.v_y_tap[LX_PE_WIN_0];
				pInfo->h_c_tap[LX_PE_WIN_0]=cur_param.h_c_tap[LX_PE_WIN_0];
				pInfo->v_c_tap[LX_PE_WIN_0]=cur_param.v_c_tap[LX_PE_WIN_0];
				pInfo->y_fir[LX_PE_WIN_0]=cur_param.y_fir[LX_PE_WIN_0];
				pInfo->c_fir[LX_PE_WIN_0]=cur_param.c_fir[LX_PE_WIN_0];
			}
		}
		else if(PE_KDRV_VER_M14AX)
		{
			memset0(cur_param);
			/* get fir,tap,index */
			ret = PE_SHP_HW_M14_GetScalerFilterValue(pCfg,&cur_param);
			PE_SHP_HW_M14_CHECK_CODE(ret, break, "[%s,%d] PE_SHP_HW_M14_GetScalerFilterValue() error.\n", __F__, __L__);
			/* check index is valid & re-setting index */
			PE_SHP_HW_M14_CheckValidScalerFilterValue(&cur_param);
			/* L : compare & filtering */
			if(pInfo->pre_scl_idx[LX_PE_WIN_0].h_luma_id!=cur_param.cur_scl_idx[LX_PE_WIN_0].h_luma_id || \
				pInfo->pre_scl_idx[LX_PE_WIN_0].v_luma_id!=cur_param.cur_scl_idx[LX_PE_WIN_0].v_luma_id || \
				pInfo->pre_scl_idx[LX_PE_WIN_0].h_chrm_id!=cur_param.cur_scl_idx[LX_PE_WIN_0].h_chrm_id || \
				pInfo->pre_scl_idx[LX_PE_WIN_0].v_chrm_id!=cur_param.cur_scl_idx[LX_PE_WIN_0].v_chrm_id || \
				pInfo->h_y_tap[LX_PE_WIN_0]!=cur_param.h_y_tap[LX_PE_WIN_0] || \
				pInfo->v_y_tap[LX_PE_WIN_0]!=cur_param.v_y_tap[LX_PE_WIN_0] || \
				pInfo->h_c_tap[LX_PE_WIN_0]!=cur_param.h_c_tap[LX_PE_WIN_0] || \
				pInfo->v_c_tap[LX_PE_WIN_0]!=cur_param.v_c_tap[LX_PE_WIN_0] || \
				pInfo->y_fir[LX_PE_WIN_0]!=cur_param.y_fir[LX_PE_WIN_0] || \
				pInfo->c_fir[LX_PE_WIN_0]!=cur_param.c_fir[LX_PE_WIN_0])
			{
				PE_SHP_HW_M14_DBG_PRINT("set[0]:[idx_hy,vy,hc,vc][pre:%d,%d,%d,%d][cur:%d,%d,%d,%d]\n",\
					pInfo->pre_scl_idx[LX_PE_WIN_0].h_luma_id,pInfo->pre_scl_idx[LX_PE_WIN_0].v_luma_id,\
					pInfo->pre_scl_idx[LX_PE_WIN_0].h_chrm_id,pInfo->pre_scl_idx[LX_PE_WIN_0].v_chrm_id,\
					cur_param.cur_scl_idx[LX_PE_WIN_0].h_luma_id,cur_param.cur_scl_idx[LX_PE_WIN_0].v_luma_id,\
					cur_param.cur_scl_idx[LX_PE_WIN_0].h_chrm_id,cur_param.cur_scl_idx[LX_PE_WIN_0].v_chrm_id);
				PE_SHP_HW_M14_DBG_PRINT("set[0]:[tap_hy,vy,hc,vc][pre:%d,%d,%d,%d][cur:%d,%d,%d,%d]\n",\
					pInfo->h_y_tap[LX_PE_WIN_0],pInfo->v_y_tap[LX_PE_WIN_0],\
					pInfo->h_c_tap[LX_PE_WIN_0],pInfo->v_c_tap[LX_PE_WIN_0],\
					cur_param.h_y_tap[LX_PE_WIN_0],cur_param.v_y_tap[LX_PE_WIN_0],\
					cur_param.h_c_tap[LX_PE_WIN_0],cur_param.v_c_tap[LX_PE_WIN_0]);
				PE_SHP_HW_M14_DBG_PRINT("set[0]:[fir_y,c][pre:%d,%d][cur:%d,%d]\n",\
					pInfo->y_fir[LX_PE_WIN_0],pInfo->c_fir[LX_PE_WIN_0],\
					cur_param.y_fir[LX_PE_WIN_0],cur_param.c_fir[LX_PE_WIN_0]);
				/* set config */
				if(pInfo->h_y_tap[LX_PE_WIN_0]!=cur_param.h_y_tap[LX_PE_WIN_0] || \
					pInfo->y_fir[LX_PE_WIN_0]!=cur_param.y_fir[LX_PE_WIN_0] || \
					pInfo->c_fir[LX_PE_WIN_0]!=cur_param.c_fir[LX_PE_WIN_0])
				{
					scaler_config	= (cur_param.h_y_tap[LX_PE_WIN_0]==PE_SHP_HW_M14_SCL_V12T_NORM || \
										cur_param.h_y_tap[LX_PE_WIN_0]==PE_SHP_HW_M14_SCL_V12T_DN14)? 0x0:0x1;
					adap_filter_en	= (cur_param.h_y_tap[LX_PE_WIN_0]==PE_SHP_HW_M14_SCL_V12T_NORM)? 0x1:0x0;
					type_sel_luma	= (cur_param.y_fir[LX_PE_WIN_0]==PE_SHP_HW_M14_FIR_BILINEAR)? 0x1:0x0;
					type_sel_chroma	= (cur_param.c_fir[LX_PE_WIN_0]==PE_SHP_HW_M14_FIR_BILINEAR)? 0x1:0x0;
			        PE_MSL_M14A0_RdFL(msc_ctrl_l);
			        PE_MSL_M14A0_Wr01(msc_ctrl_l,	filter_type_sel_luma,	type_sel_luma);
			        PE_MSL_M14A0_Wr01(msc_ctrl_l,	filter_type_sel_chroma,	type_sel_chroma);
			        PE_MSL_M14A0_Wr01(msc_ctrl_l,	v_scaler_config,		scaler_config);
			        PE_MSL_M14A0_Wr01(msc_ctrl_l,	adaptive_fir_filter_en,	adap_filter_en);
			        PE_MSL_M14A0_WrFL(msc_ctrl_l);
				}
				/* check double setting flag*/
				set_flag = PE_SHP_HW_M14_GetDoubleSetFlag(LX_PE_WIN_0,pInfo->pre_scl_idx[LX_PE_WIN_0],&cur_param);
				/* set scl coeff */
				ret = PE_SHP_HW_M14_SetSclCoeff(LX_PE_WIN_0,set_flag,&cur_param);
				PE_SHP_HW_M14_CHECK_CODE(ret, break, "[%s,%d] PE_SHP_HW_M14_SetSclCoeff() error.\n", __F__, __L__);
				pInfo->pre_scl_idx[LX_PE_WIN_0].h_luma_id=cur_param.cur_scl_idx[LX_PE_WIN_0].h_luma_id;
				pInfo->pre_scl_idx[LX_PE_WIN_0].v_luma_id=cur_param.cur_scl_idx[LX_PE_WIN_0].v_luma_id;
				pInfo->pre_scl_idx[LX_PE_WIN_0].h_chrm_id=cur_param.cur_scl_idx[LX_PE_WIN_0].h_chrm_id;
				pInfo->pre_scl_idx[LX_PE_WIN_0].v_chrm_id=cur_param.cur_scl_idx[LX_PE_WIN_0].v_chrm_id;
				pInfo->h_y_tap[LX_PE_WIN_0]=cur_param.h_y_tap[LX_PE_WIN_0];
				pInfo->v_y_tap[LX_PE_WIN_0]=cur_param.v_y_tap[LX_PE_WIN_0];
				pInfo->h_c_tap[LX_PE_WIN_0]=cur_param.h_c_tap[LX_PE_WIN_0];
				pInfo->v_c_tap[LX_PE_WIN_0]=cur_param.v_c_tap[LX_PE_WIN_0];
				pInfo->y_fir[LX_PE_WIN_0]=cur_param.y_fir[LX_PE_WIN_0];
				pInfo->c_fir[LX_PE_WIN_0]=cur_param.c_fir[LX_PE_WIN_0];
			}
			/* R : compare & filtering */
			if(pInfo->pre_scl_idx[LX_PE_WIN_1].h_luma_id!=cur_param.cur_scl_idx[LX_PE_WIN_1].h_luma_id || \
				pInfo->pre_scl_idx[LX_PE_WIN_1].v_luma_id!=cur_param.cur_scl_idx[LX_PE_WIN_1].v_luma_id || \
				pInfo->pre_scl_idx[LX_PE_WIN_1].h_chrm_id!=cur_param.cur_scl_idx[LX_PE_WIN_1].h_chrm_id || \
				pInfo->pre_scl_idx[LX_PE_WIN_1].v_chrm_id!=cur_param.cur_scl_idx[LX_PE_WIN_1].v_chrm_id || \
				pInfo->h_y_tap[LX_PE_WIN_1]!=cur_param.h_y_tap[LX_PE_WIN_1] || \
				pInfo->v_y_tap[LX_PE_WIN_1]!=cur_param.v_y_tap[LX_PE_WIN_1] || \
				pInfo->h_c_tap[LX_PE_WIN_1]!=cur_param.h_c_tap[LX_PE_WIN_1] || \
				pInfo->v_c_tap[LX_PE_WIN_1]!=cur_param.v_c_tap[LX_PE_WIN_1] || \
				pInfo->y_fir[LX_PE_WIN_1]!=cur_param.y_fir[LX_PE_WIN_1] || \
				pInfo->c_fir[LX_PE_WIN_1]!=cur_param.c_fir[LX_PE_WIN_1])
			{
				PE_SHP_HW_M14_DBG_PRINT("set[1]:[idx_hy,vy,hc,vc][pre:%d,%d,%d,%d][cur:%d,%d,%d,%d]\n",\
					pInfo->pre_scl_idx[LX_PE_WIN_1].h_luma_id,pInfo->pre_scl_idx[LX_PE_WIN_1].v_luma_id,\
					pInfo->pre_scl_idx[LX_PE_WIN_1].h_chrm_id,pInfo->pre_scl_idx[LX_PE_WIN_1].v_chrm_id,\
					cur_param.cur_scl_idx[LX_PE_WIN_1].h_luma_id,cur_param.cur_scl_idx[LX_PE_WIN_1].v_luma_id,\
					cur_param.cur_scl_idx[LX_PE_WIN_1].h_chrm_id,cur_param.cur_scl_idx[LX_PE_WIN_1].v_chrm_id);
				PE_SHP_HW_M14_DBG_PRINT("set[1]:[tap_hy,vy,hc,vc][pre:%d,%d,%d,%d][cur:%d,%d,%d,%d]\n",\
					pInfo->h_y_tap[LX_PE_WIN_1],pInfo->v_y_tap[LX_PE_WIN_1],\
					pInfo->h_c_tap[LX_PE_WIN_1],pInfo->v_c_tap[LX_PE_WIN_1],\
					cur_param.h_y_tap[LX_PE_WIN_1],cur_param.v_y_tap[LX_PE_WIN_1],\
					cur_param.h_c_tap[LX_PE_WIN_1],cur_param.v_c_tap[LX_PE_WIN_1]);
				PE_SHP_HW_M14_DBG_PRINT("set[1]:[fir_y,c][pre:%d,%d][cur:%d,%d]\n",\
					pInfo->y_fir[LX_PE_WIN_1],pInfo->c_fir[LX_PE_WIN_1],\
					cur_param.y_fir[LX_PE_WIN_1],cur_param.c_fir[LX_PE_WIN_1]);
				/* set config(skip comparison) */
				adap_filter_en	= (cur_param.h_y_tap[LX_PE_WIN_1]==PE_SHP_HW_M14_SCL_V12T_NORM)? 0x1:0x0;
				type_sel_luma	= (cur_param.y_fir[LX_PE_WIN_1]==PE_SHP_HW_M14_FIR_BILINEAR)? 0x1:0x0;
				type_sel_chroma	= (cur_param.c_fir[LX_PE_WIN_1]==PE_SHP_HW_M14_FIR_BILINEAR)? 0x1:0x0;
		        PE_MSR_M14A0_RdFL(msc_ctrl_r);
		        PE_MSR_M14A0_Wr01(msc_ctrl_r, filter_type_sel_luma       ,type_sel_luma);
		        PE_MSR_M14A0_Wr01(msc_ctrl_r, filter_type_sel_chroma     ,type_sel_chroma);
		        PE_MSR_M14A0_Wr01(msc_ctrl_r, adaptive_fir_filter_en     ,adap_filter_en);
		        PE_MSR_M14A0_WrFL(msc_ctrl_r);
				/* check double setting flag*/
				set_flag = PE_SHP_HW_M14_GetDoubleSetFlag(LX_PE_WIN_1,pInfo->pre_scl_idx[LX_PE_WIN_1],&cur_param);
				/* set scl coeff */
				ret = PE_SHP_HW_M14_SetSclCoeff(LX_PE_WIN_1,set_flag,&cur_param);
				PE_SHP_HW_M14_CHECK_CODE(ret, break, "[%s,%d] PE_SHP_HW_M14_SetSclCoeff() error.\n", __F__, __L__);
				pInfo->pre_scl_idx[LX_PE_WIN_1].h_luma_id=cur_param.cur_scl_idx[LX_PE_WIN_1].h_luma_id;
				pInfo->pre_scl_idx[LX_PE_WIN_1].v_luma_id=cur_param.cur_scl_idx[LX_PE_WIN_1].v_luma_id;
				pInfo->pre_scl_idx[LX_PE_WIN_1].h_chrm_id=cur_param.cur_scl_idx[LX_PE_WIN_1].h_chrm_id;
				pInfo->pre_scl_idx[LX_PE_WIN_1].v_chrm_id=cur_param.cur_scl_idx[LX_PE_WIN_1].v_chrm_id;
				pInfo->h_y_tap[LX_PE_WIN_1]=cur_param.h_y_tap[LX_PE_WIN_1];
				pInfo->v_y_tap[LX_PE_WIN_1]=cur_param.v_y_tap[LX_PE_WIN_1];
				pInfo->h_c_tap[LX_PE_WIN_1]=cur_param.h_c_tap[LX_PE_WIN_1];
				pInfo->v_c_tap[LX_PE_WIN_1]=cur_param.v_c_tap[LX_PE_WIN_1];
				pInfo->y_fir[LX_PE_WIN_1]=cur_param.y_fir[LX_PE_WIN_1];
				pInfo->c_fir[LX_PE_WIN_1]=cur_param.c_fir[LX_PE_WIN_1];
			}
		}
		else
		{
			PE_SHP_HW_M14_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * get scaler filter control value
 *	- tap type, fir type, indx
 *
 * @param   *pCfg [in] PE_TSK_M14_CFG_T
 * @param   *pstParams [in] PE_SHP_HW_M14_SETTINGS_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_SHP_HW_M14_GetScalerFilterValue(PE_TSK_M14_CFG_T *pCfg,PE_SHP_HW_M14_SETTINGS_T *pstParams)
{
	int ret = RET_OK;
	UINT32 type6Tap[LX_PE_WIN_NUM],h_ratio[LX_PE_WIN_NUM],v_ratio[LX_PE_WIN_NUM],down12tMode;
	LX_PE_INF_DISPLAY_T *disp0_info = &pCfg->inf.disp_info[LX_PE_WIN_0];
	PE_INF_M14_CTRL_T *ctrl0_info = &pCfg->inf.ctrl_info[LX_PE_WIN_0];
	PE_INF_M14_CTRL_T *ctrl1_info = &pCfg->inf.ctrl_info[LX_PE_WIN_1];
	PE_SHP_HW_M14_SETTINGS_T *pIdx_Info=&_g_pe_shp_hw_m14_info;
	do{
		CHECK_KNULL(pCfg);
		if(PE_KDRV_VER_M14BX)
		{
			/* v12tap down scl case:hdmi & not_scl_hd & resize & in_win_2160 */
			down12tMode = (disp0_info->src_type==LX_PE_SRC_HDMI && \
						!ctrl0_info->scl_hd && ctrl0_info->scl_resize && \
						ctrl0_info->i_win_v_size==(PE_SHP_HW_M14_FHD_V_MAX<<1))? 1:0;
			/* check fir type(if pixel by pixel case : bilinear) */
			if((ctrl0_info->i_win_h_size==ctrl0_info->o_win_h_size) && \
				(ctrl0_info->i_win_v_size==ctrl0_info->o_win_v_size))
			{
				pstParams->y_fir[LX_PE_WIN_0] = PE_SHP_HW_M14_FIR_BILINEAR;
				pstParams->c_fir[LX_PE_WIN_0] = PE_SHP_HW_M14_FIR_BILINEAR;
			}
			else
			{
				/* 20130131, sj.youm, set y_fir bilinear because of color filter issue */
				pstParams->y_fir[LX_PE_WIN_0] = (ctrl0_info->is_yuv444 && ctrl0_info->pxl_rp_st)? \
					PE_SHP_HW_M14_FIR_BILINEAR:PE_SHP_HW_M14_FIR_POLYPHASE;
				pstParams->c_fir[LX_PE_WIN_0] = (ctrl0_info->is_yuv444)? \
					PE_SHP_HW_M14_FIR_BILINEAR:PE_SHP_HW_M14_FIR_POLYPHASE;
			}
			/* 6tap case:src_hsize:0(abnormal),over1024(2_line_memory),resize(with not dn12t),adaptive_stream */
			type6Tap[LX_PE_WIN_0] = (!ctrl0_info->scl_src_h || ctrl0_info->scl_hd || ctrl0_info->scl_resize || \
									disp0_info->mode.is_adpt_st)? 1:0;
			if(type6Tap[LX_PE_WIN_0] && down12tMode)
			{
				type6Tap[LX_PE_WIN_0]=0;
			}
			/* check y tap */
			if(type6Tap[LX_PE_WIN_0])
			{
				/* check in vs. out ratio */
				if(ctrl0_info->o_win_h_size >= PE_SHP_HW_M14_SCL_IN_H_MIN && \
					ctrl0_info->o_win_v_size >= PE_SHP_HW_M14_SCL_IN_V_MIN && \
					ctrl0_info->i_win_h_size && ctrl0_info->i_win_v_size)
				{
					h_ratio[LX_PE_WIN_0] = (ctrl0_info->o_win_h_size<<10)/ctrl0_info->i_win_h_size;
					v_ratio[LX_PE_WIN_0] = (ctrl0_info->o_win_v_size<<10)/ctrl0_info->i_win_v_size;
				}
				else
				{
					h_ratio[LX_PE_WIN_0] = 1024;	//1.0
					v_ratio[LX_PE_WIN_0] = 1024;	//1.0
				}
				/* tap */
				if( h_ratio[LX_PE_WIN_0] >= 875 )		{	pstParams->h_y_tap[LX_PE_WIN_0] = PE_SHP_HW_M14_SCL_V06T_NORM;}
				else if ( h_ratio[LX_PE_WIN_0] >= 625 )	{	pstParams->h_y_tap[LX_PE_WIN_0] = PE_SHP_HW_M14_SCL_V06T_DN75;}
				else if ( h_ratio[LX_PE_WIN_0] >= 375 )	{	pstParams->h_y_tap[LX_PE_WIN_0] = PE_SHP_HW_M14_SCL_V06T_DN50;}
				else									{	pstParams->h_y_tap[LX_PE_WIN_0] = PE_SHP_HW_M14_SCL_V06T_DN25;}
				if( v_ratio[LX_PE_WIN_0] >= 875 )		{	pstParams->v_y_tap[LX_PE_WIN_0] = PE_SHP_HW_M14_SCL_V06T_NORM;}
				else if ( v_ratio[LX_PE_WIN_0] >= 625 )	{	pstParams->v_y_tap[LX_PE_WIN_0] = PE_SHP_HW_M14_SCL_V06T_DN75;}
				else if ( v_ratio[LX_PE_WIN_0] >= 375 )	{	pstParams->v_y_tap[LX_PE_WIN_0] = PE_SHP_HW_M14_SCL_V06T_DN50;}
				else									{	pstParams->v_y_tap[LX_PE_WIN_0] = PE_SHP_HW_M14_SCL_V06T_DN25;}
				/* check vertical half mode */
				if((pstParams->v_y_tap[LX_PE_WIN_0]==PE_SHP_HW_M14_SCL_V06T_NORM) && \
					(ctrl0_info->o_win_v_size<=(ctrl0_info->i_win_v_size>>1)))
				{
					pstParams->v_y_tap[LX_PE_WIN_0]=PE_SHP_HW_M14_SCL_V06T_HALF;
				}
			}
			else
			{
				pstParams->h_y_tap[LX_PE_WIN_0] = (down12tMode)? PE_SHP_HW_M14_SCL_V12T_DN14:PE_SHP_HW_M14_SCL_V12T_NORM;
				pstParams->v_y_tap[LX_PE_WIN_0] = (down12tMode)? PE_SHP_HW_M14_SCL_V12T_DN14:PE_SHP_HW_M14_SCL_V12T_NORM;
			}
			pstParams->h_c_tap[LX_PE_WIN_0] = pstParams->h_y_tap[LX_PE_WIN_0];
			pstParams->v_c_tap[LX_PE_WIN_0] = pstParams->v_y_tap[LX_PE_WIN_0];
			pstParams->cur_scl_idx[LX_PE_WIN_0].h_luma_id = pIdx_Info->cur_scl_idx[LX_PE_WIN_0].h_luma_id;
			pstParams->cur_scl_idx[LX_PE_WIN_0].v_luma_id = pIdx_Info->cur_scl_idx[LX_PE_WIN_0].v_luma_id;
			pstParams->cur_scl_idx[LX_PE_WIN_0].h_chrm_id = pIdx_Info->cur_scl_idx[LX_PE_WIN_0].h_chrm_id;
			pstParams->cur_scl_idx[LX_PE_WIN_0].v_chrm_id = pIdx_Info->cur_scl_idx[LX_PE_WIN_0].v_chrm_id;
		}
		else if(PE_KDRV_VER_M14AX)
		{
			/* v12tap down scl case:hdmi(pc) & 2d & not_444 & not_scl_hd & resize & in_win_2160 */
			down12tMode = (disp0_info->src_type==LX_PE_SRC_HDMI && \
						disp0_info->out_type==LX_PE_OUT_2D && disp0_info->user_o_type==LX_PE_OUT_2D && \
						!ctrl0_info->is_yuv444 && !ctrl0_info->scl_hd && ctrl0_info->scl_resize && \
						ctrl0_info->i_win_v_size==(PE_SHP_HW_M14_FHD_V_MAX<<1))? 1:0;
			/* L,R : check fir type(if pixel by pixel case : bilinear) */
			if((ctrl0_info->i_win_h_size==ctrl0_info->o_win_h_size) && \
				(ctrl0_info->i_win_v_size==ctrl0_info->o_win_v_size))
			{
				pstParams->y_fir[LX_PE_WIN_0] = PE_SHP_HW_M14_FIR_BILINEAR;
				pstParams->c_fir[LX_PE_WIN_0] = PE_SHP_HW_M14_FIR_BILINEAR;
			}
			else
			{
				/* 20130131, sj.youm, set y_fir bilinear because of color filter issue */
				pstParams->y_fir[LX_PE_WIN_0] = (ctrl0_info->is_yuv444 && ctrl0_info->pxl_rp_st)? \
					PE_SHP_HW_M14_FIR_BILINEAR:PE_SHP_HW_M14_FIR_POLYPHASE;
				pstParams->c_fir[LX_PE_WIN_0] = (ctrl0_info->is_yuv444)? \
					PE_SHP_HW_M14_FIR_BILINEAR:PE_SHP_HW_M14_FIR_POLYPHASE;
			}
			if((ctrl1_info->i_win_h_size==ctrl1_info->o_win_h_size) && \
				(ctrl1_info->i_win_v_size==ctrl1_info->o_win_v_size))
			{
				pstParams->y_fir[LX_PE_WIN_1] = PE_SHP_HW_M14_FIR_BILINEAR;
				pstParams->c_fir[LX_PE_WIN_1] = PE_SHP_HW_M14_FIR_BILINEAR;
			}
			else
			{
				pstParams->y_fir[LX_PE_WIN_1] = PE_SHP_HW_M14_FIR_POLYPHASE;
				pstParams->c_fir[LX_PE_WIN_1] = PE_SHP_HW_M14_FIR_POLYPHASE;	/* no 444 case */
			}
			/* 6tap case:src_hsize:0(abnormal),over1024(2_line_memory),resize(with not dn12t),yuv444,3d,adaptive_stream */
			type6Tap[LX_PE_WIN_0] = (!ctrl0_info->scl_src_h || ctrl0_info->scl_hd || ctrl0_info->scl_resize || \
									ctrl0_info->is_yuv444 || disp0_info->mode.is_adpt_st || \
									disp0_info->out_type==LX_PE_OUT_TB || disp0_info->out_type==LX_PE_OUT_SS || \
									disp0_info->out_type==LX_PE_OUT_FS || disp0_info->out_type==LX_PE_OUT_DUAL_TB || \
									disp0_info->out_type==LX_PE_OUT_DUAL_SS || disp0_info->out_type==LX_PE_OUT_DUAL_FULL)? \
									1:0;
			if(type6Tap[LX_PE_WIN_0] && down12tMode)
			{
				type6Tap[LX_PE_WIN_0]=0;
			}
			type6Tap[LX_PE_WIN_1] = 1;
			/* L : check y tap */
			if(type6Tap[LX_PE_WIN_0])
			{
				/* check in vs. out ratio */
				if(ctrl0_info->o_win_h_size >= PE_SHP_HW_M14_SCL_IN_H_MIN && \
					ctrl0_info->o_win_v_size >= PE_SHP_HW_M14_SCL_IN_V_MIN && \
					ctrl0_info->i_win_h_size && ctrl0_info->i_win_v_size)
				{
					h_ratio[LX_PE_WIN_0] = (ctrl0_info->o_win_h_size<<10)/ctrl0_info->i_win_h_size;
					v_ratio[LX_PE_WIN_0] = (ctrl0_info->o_win_v_size<<10)/ctrl0_info->i_win_v_size;
				}
				else
				{
					h_ratio[LX_PE_WIN_0] = 1024;	//1.0
					v_ratio[LX_PE_WIN_0] = 1024;	//1.0
				}
				/* tap */
				if( h_ratio[LX_PE_WIN_0] >= 875 )		{	pstParams->h_y_tap[LX_PE_WIN_0] = PE_SHP_HW_M14_SCL_V06T_NORM;}
				else if ( h_ratio[LX_PE_WIN_0] >= 625 )	{	pstParams->h_y_tap[LX_PE_WIN_0] = PE_SHP_HW_M14_SCL_V06T_DN75;}
				else if ( h_ratio[LX_PE_WIN_0] >= 375 )	{	pstParams->h_y_tap[LX_PE_WIN_0] = PE_SHP_HW_M14_SCL_V06T_DN50;}
				else									{	pstParams->h_y_tap[LX_PE_WIN_0] = PE_SHP_HW_M14_SCL_V06T_DN25;}
				if( v_ratio[LX_PE_WIN_0] >= 875 )		{	pstParams->v_y_tap[LX_PE_WIN_0] = PE_SHP_HW_M14_SCL_V06T_NORM;}
				else if ( v_ratio[LX_PE_WIN_0] >= 625 )	{	pstParams->v_y_tap[LX_PE_WIN_0] = PE_SHP_HW_M14_SCL_V06T_DN75;}
				else if ( v_ratio[LX_PE_WIN_0] >= 375 )	{	pstParams->v_y_tap[LX_PE_WIN_0] = PE_SHP_HW_M14_SCL_V06T_DN50;}
				else									{	pstParams->v_y_tap[LX_PE_WIN_0] = PE_SHP_HW_M14_SCL_V06T_DN25;}
				/* check vertical half mode */
				if((pstParams->v_y_tap[LX_PE_WIN_0]==PE_SHP_HW_M14_SCL_V06T_NORM) && \
					(ctrl0_info->o_win_v_size<=(ctrl0_info->i_win_v_size>>1)))
				{
					pstParams->v_y_tap[LX_PE_WIN_0]=PE_SHP_HW_M14_SCL_V06T_HALF;
				}
			}
			else
			{
				pstParams->h_y_tap[LX_PE_WIN_0] = (down12tMode)? PE_SHP_HW_M14_SCL_V12T_DN14:PE_SHP_HW_M14_SCL_V12T_NORM;
				pstParams->v_y_tap[LX_PE_WIN_0] = (down12tMode)? PE_SHP_HW_M14_SCL_V12T_DN14:PE_SHP_HW_M14_SCL_V12T_NORM;
			}
			/* R : check y tap */
			if(type6Tap[LX_PE_WIN_1])
			{
				/* check in vs. out ratio */
				if(ctrl1_info->o_win_h_size >= PE_SHP_HW_M14_SCL_IN_H_MIN && \
					ctrl1_info->o_win_v_size >= PE_SHP_HW_M14_SCL_IN_V_MIN && \
					ctrl1_info->i_win_h_size && ctrl1_info->i_win_v_size)
				{
					h_ratio[LX_PE_WIN_1] = (ctrl1_info->o_win_h_size<<10)/ctrl1_info->i_win_h_size;
					v_ratio[LX_PE_WIN_1] = (ctrl1_info->o_win_v_size<<10)/ctrl1_info->i_win_v_size;
				}
				else
				{
					h_ratio[LX_PE_WIN_1] = 1024;	//1.0
					v_ratio[LX_PE_WIN_1] = 1024;	//1.0
				}
				/* tap */
				if( h_ratio[LX_PE_WIN_1] >= 875 )		{	pstParams->h_y_tap[LX_PE_WIN_1] = PE_SHP_HW_M14_SCL_V06T_NORM;}
				else if ( h_ratio[LX_PE_WIN_1] >= 625 )	{	pstParams->h_y_tap[LX_PE_WIN_1] = PE_SHP_HW_M14_SCL_V06T_DN75;}
				else if ( h_ratio[LX_PE_WIN_1] >= 375 )	{	pstParams->h_y_tap[LX_PE_WIN_1] = PE_SHP_HW_M14_SCL_V06T_DN50;}
				else									{	pstParams->h_y_tap[LX_PE_WIN_1] = PE_SHP_HW_M14_SCL_V06T_DN25;}
				if( v_ratio[LX_PE_WIN_1] >= 875 )		{	pstParams->v_y_tap[LX_PE_WIN_1] = PE_SHP_HW_M14_SCL_V06T_NORM;}
				else if ( v_ratio[LX_PE_WIN_1] >= 625 )	{	pstParams->v_y_tap[LX_PE_WIN_1] = PE_SHP_HW_M14_SCL_V06T_DN75;}
				else if ( v_ratio[LX_PE_WIN_1] >= 375 )	{	pstParams->v_y_tap[LX_PE_WIN_1] = PE_SHP_HW_M14_SCL_V06T_DN50;}
				else									{	pstParams->v_y_tap[LX_PE_WIN_1] = PE_SHP_HW_M14_SCL_V06T_DN25;}
				/* check pip same h,v case(=split window), because of pre h scl down */
				if(disp0_info->out_type==LX_PE_OUT_PIP && (ctrl0_info->hv_same || ctrl1_info->hv_same))
				{
					pstParams->h_y_tap[LX_PE_WIN_1] = pstParams->v_y_tap[LX_PE_WIN_1];
				}
				/* check vertical half mode */
				if((pstParams->v_y_tap[LX_PE_WIN_1]==PE_SHP_HW_M14_SCL_V06T_NORM) && \
					(ctrl1_info->o_win_v_size<=(ctrl1_info->i_win_v_size>>1)))
				{
					pstParams->v_y_tap[LX_PE_WIN_1]=PE_SHP_HW_M14_SCL_V06T_HALF;
				}
			}
			else
			{
				pstParams->h_y_tap[LX_PE_WIN_1] = PE_SHP_HW_M14_SCL_V06T_NORM;	//6tap only for R
				pstParams->v_y_tap[LX_PE_WIN_1] = PE_SHP_HW_M14_SCL_V06T_NORM;	//6tap only for R
			}
			/* L : double-check y,c tap & idx */
			if(disp0_info->src_type==LX_PE_SRC_DTV && disp0_info->mode.is_adpt_st)
			{
				/* 20111203, sj.youm, fix 6tap for adaptive stream */
				if(pstParams->h_y_tap[LX_PE_WIN_0]==PE_SHP_HW_M14_SCL_V12T_NORM)
				{
					pstParams->h_y_tap[LX_PE_WIN_0] = PE_SHP_HW_M14_SCL_V06T_NORM;
					pstParams->h_c_tap[LX_PE_WIN_0] = pstParams->h_y_tap[LX_PE_WIN_0];
				}
				if(pstParams->v_y_tap[LX_PE_WIN_0]==PE_SHP_HW_M14_SCL_V12T_NORM)
				{
					pstParams->v_y_tap[LX_PE_WIN_0] = PE_SHP_HW_M14_SCL_V06T_NORM;
					pstParams->v_c_tap[LX_PE_WIN_0] = pstParams->v_y_tap[LX_PE_WIN_0];
				}
				pstParams->cur_scl_idx[LX_PE_WIN_0].h_luma_id = PE_SHP_HW_M14_IDX_Y_DFLT;
				pstParams->cur_scl_idx[LX_PE_WIN_0].v_luma_id = PE_SHP_HW_M14_IDX_Y_DFLT;
				pstParams->cur_scl_idx[LX_PE_WIN_0].h_chrm_id = PE_SHP_HW_M14_IDX_C_DFLT;
				pstParams->cur_scl_idx[LX_PE_WIN_0].v_chrm_id = PE_SHP_HW_M14_IDX_C_DFLT;
			}
			else
			{
				pstParams->h_c_tap[LX_PE_WIN_0] = pstParams->h_y_tap[LX_PE_WIN_0];
				pstParams->v_c_tap[LX_PE_WIN_0] = pstParams->v_y_tap[LX_PE_WIN_0];
				pstParams->cur_scl_idx[LX_PE_WIN_0].h_luma_id = pIdx_Info->cur_scl_idx[LX_PE_WIN_0].h_luma_id;
				pstParams->cur_scl_idx[LX_PE_WIN_0].v_luma_id = pIdx_Info->cur_scl_idx[LX_PE_WIN_0].v_luma_id;
				pstParams->cur_scl_idx[LX_PE_WIN_0].h_chrm_id = pIdx_Info->cur_scl_idx[LX_PE_WIN_0].h_chrm_id;
				pstParams->cur_scl_idx[LX_PE_WIN_0].v_chrm_id = pIdx_Info->cur_scl_idx[LX_PE_WIN_0].v_chrm_id;
			}
			/* R : double-check y,c tap & idx */
			#ifdef PE_SHP_HW_M14_USE_MSR_FOR_VENC_ON
			if(disp0_info->mode.is_venc)
			{
				pstParams->h_c_tap[LX_PE_WIN_1] = pstParams->h_y_tap[LX_PE_WIN_1];
				pstParams->v_c_tap[LX_PE_WIN_1] = pstParams->v_y_tap[LX_PE_WIN_1];
				pstParams->cur_scl_idx[LX_PE_WIN_1].h_luma_id = PE_SHP_HW_M14_IDX_Y_DFLT;
				pstParams->cur_scl_idx[LX_PE_WIN_1].v_luma_id = PE_SHP_HW_M14_IDX_Y_DFLT;
				pstParams->cur_scl_idx[LX_PE_WIN_1].h_chrm_id = PE_SHP_HW_M14_IDX_C_DFLT;
				pstParams->cur_scl_idx[LX_PE_WIN_1].v_chrm_id = PE_SHP_HW_M14_IDX_C_DFLT;
			}
			else
			{
				pstParams->h_c_tap[LX_PE_WIN_1] = pstParams->h_y_tap[LX_PE_WIN_1];
				pstParams->v_c_tap[LX_PE_WIN_1] = pstParams->v_y_tap[LX_PE_WIN_1];
				pstParams->cur_scl_idx[LX_PE_WIN_1].h_luma_id = pIdx_Info->cur_scl_idx[LX_PE_WIN_1].h_luma_id;
				pstParams->cur_scl_idx[LX_PE_WIN_1].v_luma_id = pIdx_Info->cur_scl_idx[LX_PE_WIN_1].v_luma_id;
				pstParams->cur_scl_idx[LX_PE_WIN_1].h_chrm_id = pIdx_Info->cur_scl_idx[LX_PE_WIN_1].h_chrm_id;
				pstParams->cur_scl_idx[LX_PE_WIN_1].v_chrm_id = pIdx_Info->cur_scl_idx[LX_PE_WIN_1].v_chrm_id;
			}
			#else
			pstParams->h_c_tap[LX_PE_WIN_1] = pstParams->h_y_tap[LX_PE_WIN_1];
			pstParams->v_c_tap[LX_PE_WIN_1] = pstParams->v_y_tap[LX_PE_WIN_1];
			pstParams->cur_scl_idx[LX_PE_WIN_1].h_luma_id = pIdx_Info->cur_scl_idx[LX_PE_WIN_1].h_luma_id;
			pstParams->cur_scl_idx[LX_PE_WIN_1].v_luma_id = pIdx_Info->cur_scl_idx[LX_PE_WIN_1].v_luma_id;
			pstParams->cur_scl_idx[LX_PE_WIN_1].h_chrm_id = pIdx_Info->cur_scl_idx[LX_PE_WIN_1].h_chrm_id;
			pstParams->cur_scl_idx[LX_PE_WIN_1].v_chrm_id = pIdx_Info->cur_scl_idx[LX_PE_WIN_1].v_chrm_id;
			#endif
		}
	}while(0);
	return ret;
}
/**
 * check the scl value is valid or not
 *
 * @param   *pstParams [in] PE_SHP_HW_M14_SETTINGS_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static void PE_SHP_HW_M14_CheckValidScalerFilterValue(PE_SHP_HW_M14_SETTINGS_T *pstParams)
{
	static UINT32 count = 0;
	LX_PE_SHP_SCLFILTER_T *idx0 = &pstParams->cur_scl_idx[LX_PE_WIN_0];
	LX_PE_SHP_SCLFILTER_T *idx1 = &pstParams->cur_scl_idx[LX_PE_WIN_1];
	PE_SHP_HW_M14_SCL hy_tap0 = pstParams->h_y_tap[LX_PE_WIN_0];
	PE_SHP_HW_M14_SCL hc_tap0 = pstParams->h_c_tap[LX_PE_WIN_0];
	PE_SHP_HW_M14_SCL vy_tap0 = pstParams->v_y_tap[LX_PE_WIN_0];
	PE_SHP_HW_M14_SCL vc_tap0 = pstParams->v_c_tap[LX_PE_WIN_0];
	PE_SHP_HW_M14_SCL hy_tap1 = pstParams->h_y_tap[LX_PE_WIN_1];
	PE_SHP_HW_M14_SCL hc_tap1 = pstParams->h_c_tap[LX_PE_WIN_1];
	PE_SHP_HW_M14_SCL vy_tap1 = pstParams->v_y_tap[LX_PE_WIN_1];
	PE_SHP_HW_M14_SCL vc_tap1 = pstParams->v_c_tap[LX_PE_WIN_1];
	do{
		if(PE_KDRV_VER_M14BX)
		{
			/* L : h luma id check */
			PE_SHP_HW_M14_DBG_CHECK_CODE(count,(hy_tap0==PE_SHP_HW_M14_SCL_V12T_NORM&&\
				idx0->h_luma_id>=PE_SHP_SCL_HW_Y_H_12T_COEF_NUM),\
				idx0->h_luma_id=PE_SHP_HW_M14_IDX_Y_DFLT,\
				"[%s,%d] hy_tap0(%d), h_luma_id(%d) is out of range.\n",\
				__F__,__L__,hy_tap0,idx0->h_luma_id);
			PE_SHP_HW_M14_DBG_CHECK_CODE(count,(hy_tap0==PE_SHP_HW_M14_SCL_V06T_NORM&&\
				idx0->h_luma_id>=PE_SHP_SCL_HW_Y_H_08T_COEF_NUM),\
				idx0->h_luma_id=PE_SHP_HW_M14_IDX_Y_DFLT,\
				"[%s,%d] hy_tap0(%d), h_luma_id(%d) is out of range.\n",\
				__F__,__L__,hy_tap0,idx0->h_luma_id);
			/* L : v luma id check */
			PE_SHP_HW_M14_DBG_CHECK_CODE(count,(vy_tap0==PE_SHP_HW_M14_SCL_V12T_NORM&&\
				idx0->v_luma_id>=PE_SHP_SCL_HW_Y_V_12T_COEF_NUM),\
				idx0->v_luma_id=PE_SHP_HW_M14_IDX_Y_DFLT,\
				"[%s,%d] vy_tap0(%d), v_luma_id(%d) is out of range.\n",\
				__F__,__L__,vy_tap0,idx0->v_luma_id);
			PE_SHP_HW_M14_DBG_CHECK_CODE(count,((vy_tap0==PE_SHP_HW_M14_SCL_V06T_NORM||\
				vy_tap0==PE_SHP_HW_M14_SCL_V06T_HALF)&&\
				idx0->v_luma_id>=PE_SHP_SCL_HW_Y_V_06T_COEF_NUM),\
				idx0->v_luma_id=PE_SHP_HW_M14_IDX_Y_DFLT,\
				"[%s,%d] vy_tap0(%d), v_luma_id(%d) is out of range.\n",\
				__F__,__L__,vy_tap0,idx0->v_luma_id);
			/* L : h chroma id check */
			PE_SHP_HW_M14_DBG_CHECK_CODE(count,((hc_tap0==PE_SHP_HW_M14_SCL_V12T_NORM||\
				hc_tap0==PE_SHP_HW_M14_SCL_V06T_NORM)&&\
				idx0->h_chrm_id>=PE_SHP_SCL_HW_C_08T_COEF_NUM),\
				idx0->h_chrm_id=PE_SHP_HW_M14_IDX_C_DFLT,\
				"[%s,%d] hc_tap0(%d), h_chrm_id(%d) is out of range.\n",\
				__F__,__L__,hc_tap0,idx0->h_chrm_id);
			/* L : v chroma id check */
			PE_SHP_HW_M14_DBG_CHECK_CODE(count,((vc_tap0==PE_SHP_HW_M14_SCL_V12T_NORM||\
				vc_tap0==PE_SHP_HW_M14_SCL_V06T_NORM||\
				vc_tap0==PE_SHP_HW_M14_SCL_V06T_HALF)&&\
				idx0->v_chrm_id>=PE_SHP_SCL_HW_C_04T_COEF_NUM),\
				idx0->v_chrm_id=PE_SHP_HW_M14_IDX_C_DFLT,\
				"[%s,%d] vc_tap0(%d), v_chrm_id(%d) is out of range.\n",\
				__F__,__L__,vc_tap0,idx0->v_chrm_id);

			/* idx control, down scale case */
			PE_SHP_HW_M14_DBG_CHECK_CODE(count,\
				(hy_tap0==PE_SHP_HW_M14_SCL_V12T_DN14||hy_tap0==PE_SHP_HW_M14_SCL_V06T_DN75 ||\
				hy_tap0==PE_SHP_HW_M14_SCL_V06T_DN50||hy_tap0==PE_SHP_HW_M14_SCL_V06T_DN25),\
				idx0->h_luma_id=0,"[%s,%d]hy_tap0(DOWN):%d,h_luma_id=0\n",__F__,__L__,hy_tap0);
			PE_SHP_HW_M14_DBG_CHECK_CODE(count,\
				(vy_tap0==PE_SHP_HW_M14_SCL_V12T_DN14||vy_tap0==PE_SHP_HW_M14_SCL_V06T_DN75||\
				vy_tap0==PE_SHP_HW_M14_SCL_V06T_DN50||vy_tap0==PE_SHP_HW_M14_SCL_V06T_DN25),\
				idx0->v_luma_id=0,"[%s,%d]vy_tap0(DOWN):%d,v_luma_id=0\n",__F__,__L__,vy_tap0);
			PE_SHP_HW_M14_DBG_CHECK_CODE(count,\
				(hc_tap0==PE_SHP_HW_M14_SCL_V12T_DN14||hc_tap0==PE_SHP_HW_M14_SCL_V06T_DN75||\
				hc_tap0==PE_SHP_HW_M14_SCL_V06T_DN50||hc_tap0==PE_SHP_HW_M14_SCL_V06T_DN25),\
				idx0->h_chrm_id=0,"[%s,%d]hc_tap0(DOWN):%d,h_chrm_id=0\n",__F__,__L__,hc_tap0);
			PE_SHP_HW_M14_DBG_CHECK_CODE(count,\
				(vc_tap0==PE_SHP_HW_M14_SCL_V12T_DN14||vc_tap0==PE_SHP_HW_M14_SCL_V06T_DN75||\
				vc_tap0==PE_SHP_HW_M14_SCL_V06T_DN50||vc_tap0==PE_SHP_HW_M14_SCL_V06T_DN25),\
				idx0->v_chrm_id=0,"[%s,%d]vc_tap0(DOWN):%d,v_chrm_id=0\n",__F__,__L__,vc_tap0);
			if(_g_shp_hw_m14_trace)
			{
				if(count>PE_PRINT_COUNT_NUM)
				{
					count=0;
				}
				count++;
			}
		}
		else if(PE_KDRV_VER_M14AX)
		{
			/* L : h luma id check */
			PE_SHP_HW_M14_DBG_CHECK_CODE(count,(hy_tap0==PE_SHP_HW_M14_SCL_V12T_NORM&&\
				idx0->h_luma_id>=PE_SHP_SCL_HW_Y_H_12T_COEF_NUM),\
				idx0->h_luma_id=PE_SHP_HW_M14_IDX_Y_DFLT,\
				"[%s,%d] hy_tap0(%d), h_luma_id(%d) is out of range.\n",\
				__F__,__L__,hy_tap0,idx0->h_luma_id);
			PE_SHP_HW_M14_DBG_CHECK_CODE(count,(hy_tap0==PE_SHP_HW_M14_SCL_V06T_NORM&&\
				idx0->h_luma_id>=PE_SHP_SCL_HW_Y_H_08T_COEF_NUM),\
				idx0->h_luma_id=PE_SHP_HW_M14_IDX_Y_DFLT,\
				"[%s,%d] hy_tap0(%d), h_luma_id(%d) is out of range.\n",\
				__F__,__L__,hy_tap0,idx0->h_luma_id);
			/* L : v luma id check */
			PE_SHP_HW_M14_DBG_CHECK_CODE(count,(vy_tap0==PE_SHP_HW_M14_SCL_V12T_NORM&&\
				idx0->v_luma_id>=PE_SHP_SCL_HW_Y_V_12T_COEF_NUM),\
				idx0->v_luma_id=PE_SHP_HW_M14_IDX_Y_DFLT,\
				"[%s,%d] vy_tap0(%d), v_luma_id(%d) is out of range.\n",\
				__F__,__L__,vy_tap0,idx0->v_luma_id);
			PE_SHP_HW_M14_DBG_CHECK_CODE(count,((vy_tap0==PE_SHP_HW_M14_SCL_V06T_NORM||\
				vy_tap0==PE_SHP_HW_M14_SCL_V06T_HALF)&&\
				idx0->v_luma_id>=PE_SHP_SCL_HW_Y_V_06T_COEF_NUM),\
				idx0->v_luma_id=PE_SHP_HW_M14_IDX_Y_DFLT,\
				"[%s,%d] vy_tap0(%d), v_luma_id(%d) is out of range.\n",\
				__F__,__L__,vy_tap0,idx0->v_luma_id);
			/* L : h chroma id check */
			PE_SHP_HW_M14_DBG_CHECK_CODE(count,((hc_tap0==PE_SHP_HW_M14_SCL_V12T_NORM||\
				hc_tap0==PE_SHP_HW_M14_SCL_V06T_NORM)&&\
				idx0->h_chrm_id>=PE_SHP_SCL_HW_C_08T_COEF_NUM),\
				idx0->h_chrm_id=PE_SHP_HW_M14_IDX_C_DFLT,\
				"[%s,%d] hc_tap0(%d), h_chrm_id(%d) is out of range.\n",\
				__F__,__L__,hc_tap0,idx0->h_chrm_id);
			/* L : v chroma id check */
			PE_SHP_HW_M14_DBG_CHECK_CODE(count,((vc_tap0==PE_SHP_HW_M14_SCL_V12T_NORM||\
				vc_tap0==PE_SHP_HW_M14_SCL_V06T_NORM||\
				vc_tap0==PE_SHP_HW_M14_SCL_V06T_HALF)&&\
				idx0->v_chrm_id>=PE_SHP_SCL_HW_C_04T_COEF_NUM),\
				idx0->v_chrm_id=PE_SHP_HW_M14_IDX_C_DFLT,\
				"[%s,%d] vc_tap0(%d), v_chrm_id(%d) is out of range.\n",\
				__F__,__L__,vc_tap0,idx0->v_chrm_id);
			/* R : h luma id check */
			PE_SHP_HW_M14_DBG_CHECK_CODE(count,(hy_tap1==PE_SHP_HW_M14_SCL_V12T_NORM),\
				hy_tap1=PE_SHP_HW_M14_SCL_V06T_NORM,\
				"[%s,%d] hy_tap1(%d), is not support.\n",\
				__F__,__L__,hy_tap1);
			PE_SHP_HW_M14_DBG_CHECK_CODE(count,(hy_tap1==PE_SHP_HW_M14_SCL_V06T_NORM&&\
				idx1->h_luma_id>=PE_SHP_SCL_HW_Y_H_08T_COEF_NUM),\
				idx1->h_luma_id=PE_SHP_HW_M14_IDX_Y_DFLT,\
				"[%s,%d] hy_tap1(%d), h_luma_id(%d) is out of range.\n",\
				__F__,__L__,hy_tap1,idx1->h_luma_id);
			/* R : v luma id check */
			PE_SHP_HW_M14_DBG_CHECK_CODE(count,(vy_tap1==PE_SHP_HW_M14_SCL_V12T_NORM),\
				vy_tap1=PE_SHP_HW_M14_SCL_V06T_NORM,\
				"[%s,%d] vy_tap1(%d), is not support.\n",\
				__F__,__L__,vy_tap1);
			PE_SHP_HW_M14_DBG_CHECK_CODE(count,((vy_tap1==PE_SHP_HW_M14_SCL_V06T_NORM||\
				vy_tap1==PE_SHP_HW_M14_SCL_V06T_HALF)&&\
				idx1->v_luma_id>=PE_SHP_SCL_HW_Y_V_06T_COEF_NUM),\
				idx1->v_luma_id=PE_SHP_HW_M14_IDX_Y_DFLT,\
				"[%s,%d] vy_tap1(%d), v_luma_id(%d) is out of range.\n",\
				__F__,__L__,vy_tap1,idx1->v_luma_id);
			/* R : h chroma id check */
			PE_SHP_HW_M14_DBG_CHECK_CODE(count,(hc_tap1==PE_SHP_HW_M14_SCL_V12T_NORM),\
				hc_tap1=PE_SHP_HW_M14_SCL_V06T_NORM,\
				"[%s,%d] hc_tap1(%d), is not support.\n",\
				__F__,__L__,hc_tap1);
			PE_SHP_HW_M14_DBG_CHECK_CODE(count,(hc_tap1==PE_SHP_HW_M14_SCL_V06T_NORM&&\
				idx1->h_chrm_id>=PE_SHP_SCL_HW_C_08T_COEF_NUM),\
				idx1->h_chrm_id=PE_SHP_HW_M14_IDX_C_DFLT,\
				"[%s,%d] hc_tap0(%d), h_chrm_id(%d) is out of range.\n",\
				__F__,__L__,hc_tap1,idx1->h_chrm_id);
			/* R : v chroma id check */
			PE_SHP_HW_M14_DBG_CHECK_CODE(count,(vc_tap1==PE_SHP_HW_M14_SCL_V12T_NORM),\
				vc_tap1=PE_SHP_HW_M14_SCL_V06T_NORM,\
				"[%s,%d] vc_tap1(%d), is not support.\n",\
				__F__,__L__,vc_tap1);
			PE_SHP_HW_M14_DBG_CHECK_CODE(count,((vc_tap1==PE_SHP_HW_M14_SCL_V06T_NORM||\
				vc_tap1==PE_SHP_HW_M14_SCL_V06T_HALF)&&\
				idx1->v_chrm_id>=PE_SHP_SCL_HW_C_04T_COEF_NUM),\
				idx1->v_chrm_id=PE_SHP_HW_M14_IDX_C_DFLT,\
				"[%s,%d] vc_tap1(%d), v_chrm_id(%d) is out of range.\n",\
				__F__,__L__,vc_tap1,idx1->v_chrm_id);

			/* idx control, down scale case */
			PE_SHP_HW_M14_DBG_CHECK_CODE(count,\
				(hy_tap0==PE_SHP_HW_M14_SCL_V12T_DN14||hy_tap0==PE_SHP_HW_M14_SCL_V06T_DN75 ||\
				hy_tap0==PE_SHP_HW_M14_SCL_V06T_DN50||hy_tap0==PE_SHP_HW_M14_SCL_V06T_DN25),\
				idx0->h_luma_id=0,"[%s,%d]hy_tap0(DOWN):%d,h_luma_id=0\n",__F__,__L__,hy_tap0);
			PE_SHP_HW_M14_DBG_CHECK_CODE(count,\
				(vy_tap0==PE_SHP_HW_M14_SCL_V12T_DN14||vy_tap0==PE_SHP_HW_M14_SCL_V06T_DN75||\
				vy_tap0==PE_SHP_HW_M14_SCL_V06T_DN50||vy_tap0==PE_SHP_HW_M14_SCL_V06T_DN25),\
				idx0->v_luma_id=0,"[%s,%d]vy_tap0(DOWN):%d,v_luma_id=0\n",__F__,__L__,vy_tap0);
			PE_SHP_HW_M14_DBG_CHECK_CODE(count,\
				(hc_tap0==PE_SHP_HW_M14_SCL_V12T_DN14||hc_tap0==PE_SHP_HW_M14_SCL_V06T_DN75||\
				hc_tap0==PE_SHP_HW_M14_SCL_V06T_DN50||hc_tap0==PE_SHP_HW_M14_SCL_V06T_DN25),\
				idx0->h_chrm_id=0,"[%s,%d]hc_tap0(DOWN):%d,h_chrm_id=0\n",__F__,__L__,hc_tap0);
			PE_SHP_HW_M14_DBG_CHECK_CODE(count,\
				(vc_tap0==PE_SHP_HW_M14_SCL_V12T_DN14||vc_tap0==PE_SHP_HW_M14_SCL_V06T_DN75||\
				vc_tap0==PE_SHP_HW_M14_SCL_V06T_DN50||vc_tap0==PE_SHP_HW_M14_SCL_V06T_DN25),\
				idx0->v_chrm_id=0,"[%s,%d]vc_tap0(DOWN):%d,v_chrm_id=0\n",__F__,__L__,vc_tap0);
			PE_SHP_HW_M14_DBG_CHECK_CODE(count,\
				(hy_tap1==PE_SHP_HW_M14_SCL_V12T_DN14||hy_tap1==PE_SHP_HW_M14_SCL_V06T_DN75||\
				hy_tap1==PE_SHP_HW_M14_SCL_V06T_DN50||hy_tap1==PE_SHP_HW_M14_SCL_V06T_DN25),\
				idx1->h_luma_id=0,"[%s,%d]hy_tap1(DOWN):%d,h_luma_id=0\n",__F__,__L__,hy_tap1);
			PE_SHP_HW_M14_DBG_CHECK_CODE(count,\
				(vy_tap1==PE_SHP_HW_M14_SCL_V12T_DN14||vy_tap1==PE_SHP_HW_M14_SCL_V06T_DN75||\
				vy_tap1==PE_SHP_HW_M14_SCL_V06T_DN50||vy_tap1==PE_SHP_HW_M14_SCL_V06T_DN25),\
				idx1->v_luma_id=0,"[%s,%d]vy_tap1(DOWN):%d,v_luma_id=0\n",__F__,__L__,vy_tap1);
			PE_SHP_HW_M14_DBG_CHECK_CODE(count,\
				(hc_tap1==PE_SHP_HW_M14_SCL_V12T_DN14||hc_tap1==PE_SHP_HW_M14_SCL_V06T_DN75||\
				hc_tap1==PE_SHP_HW_M14_SCL_V06T_DN50||hc_tap1==PE_SHP_HW_M14_SCL_V06T_DN25),\
				idx1->h_chrm_id=0,"[%s,%d]hc_tap1(DOWN):%d,h_chrm_id=0\n",__F__,__L__,hc_tap1);
			PE_SHP_HW_M14_DBG_CHECK_CODE(count,\
				(vc_tap1==PE_SHP_HW_M14_SCL_V12T_DN14||vc_tap1==PE_SHP_HW_M14_SCL_V06T_DN75||\
				vc_tap1==PE_SHP_HW_M14_SCL_V06T_DN50||vc_tap1==PE_SHP_HW_M14_SCL_V06T_DN25),\
				idx1->v_chrm_id=0,"[%s,%d]vc_tap1(DOWN):%d,v_chrm_id=0\n",__F__,__L__,vc_tap1);
			if(_g_shp_hw_m14_trace)
			{
				if(count>PE_PRINT_COUNT_NUM)
				{
					count=0;
				}
				count++;
			}
		}
		else
		{
			PE_SHP_HW_M14_DBG_PRINT("nothing to do.\n");
			idx0->h_luma_id=idx0->v_luma_id=idx0->h_chrm_id=idx0->v_chrm_id=0;
			idx1->h_luma_id=idx1->v_luma_id=idx1->h_chrm_id=idx1->v_chrm_id=0;
		}
	}while(0);
	return;
}
/**
 * check duplicating settings or not
 *
 * @param   win_id [in] LX_PE_WIN_ID
 * @param   pre_idx [in] LX_PE_SHP_SCLFILTER_T
 * @param   *cur_param [in] PE_SHP_HW_M14_SETTINGS_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static PE_SHP_HW_M14_FLAG_T PE_SHP_HW_M14_GetDoubleSetFlag(LX_PE_WIN_ID win_id,LX_PE_SHP_SCLFILTER_T pre_idx,PE_SHP_HW_M14_SETTINGS_T *cur_param)
{
	PE_SHP_HW_M14_FLAG_T flag = {0,0,0,0};
	PE_SHP_HW_M14_SETTINGS_T *pre_tap=&_g_pe_shp_hw_m14_info;
	PE_SHP_HW_M14_CHECK_CODE(win_id>=LX_PE_WIN_NUM,return flag,"[%s,%d] win_id(%d) error.\n",__F__,__L__,win_id);
	if(pre_tap->h_y_tap[win_id]!=cur_param->h_y_tap[win_id])
	{
		flag.h_y=1;
	}
	else if((cur_param->h_y_tap[win_id]==PE_SHP_HW_M14_SCL_V12T_NORM) ||
		(cur_param->h_y_tap[win_id]==PE_SHP_HW_M14_SCL_V06T_NORM) ||
		(cur_param->h_y_tap[win_id]==PE_SHP_HW_M14_SCL_V06T_HALF))
	{
		if(pre_idx.h_luma_id!=cur_param->cur_scl_idx[win_id].h_luma_id)
			flag.h_y=1;
	}
	if(pre_tap->h_c_tap[win_id]!=cur_param->h_c_tap[win_id])
	{
		flag.h_c=1;
	}
	else if((cur_param->h_c_tap[win_id]==PE_SHP_HW_M14_SCL_V12T_NORM) ||
		(cur_param->h_c_tap[win_id]==PE_SHP_HW_M14_SCL_V06T_NORM) ||
		(cur_param->h_c_tap[win_id]==PE_SHP_HW_M14_SCL_V06T_HALF))
	{
		if(pre_idx.h_chrm_id!=cur_param->cur_scl_idx[win_id].h_chrm_id)
			flag.h_c=1;
	}
	if(pre_tap->v_y_tap[win_id]!=cur_param->v_y_tap[win_id])
	{
		flag.v_y=1;
	}
	else if((cur_param->v_y_tap[win_id]==PE_SHP_HW_M14_SCL_V12T_NORM) ||
		(cur_param->v_y_tap[win_id]==PE_SHP_HW_M14_SCL_V06T_NORM) ||
		(cur_param->v_y_tap[win_id]==PE_SHP_HW_M14_SCL_V06T_HALF))
	{
		if(pre_idx.v_luma_id!=cur_param->cur_scl_idx[win_id].v_luma_id)
			flag.v_y=1;
	}
	if(pre_tap->v_c_tap[win_id]!=cur_param->v_c_tap[win_id])
	{
		flag.v_c=1;
	}
	else if((cur_param->v_c_tap[win_id]==PE_SHP_HW_M14_SCL_V12T_NORM) ||
		(cur_param->v_c_tap[win_id]==PE_SHP_HW_M14_SCL_V06T_NORM) ||
		(cur_param->v_c_tap[win_id]==PE_SHP_HW_M14_SCL_V06T_HALF))
	{
		if(pre_idx.v_chrm_id!=cur_param->cur_scl_idx[win_id].v_chrm_id)
			flag.v_c=1;
	}
	PE_SHP_HW_M14_DBG_PRINT("set[%d] flag y(h,v):%d,%d, c(h,v):%d,%d\n",win_id,flag.h_y,flag.v_y,flag.h_c,flag.v_c);
	return flag;
}
/**
 * check duplicating settings or not
 *
 * @param   win_id [in] LX_PE_WIN_ID
 * @param   flag [in] PE_SHP_HW_M14_FLAG_T
 * @param   *pstParams [in] PE_SHP_HW_M14_SETTINGS_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_SHP_HW_M14_SetSclCoeff(LX_PE_WIN_ID win_id, PE_SHP_HW_M14_FLAG_T flag, PE_SHP_HW_M14_SETTINGS_T *pstParams)
{
	int ret = RET_OK;
	UINT8 h_y_idx,h_c_idx,v_y_idx,v_c_idx,is_v12tap;
	PE_SHP_HW_M14_SCL h_y_tap,h_c_tap,v_y_tap,v_c_tap;
	PE_SHP_HW_M14_SCL_WR_CTRL_T wr_ctrl;
	PE_SHP_HW_M14_CHECK_CODE(win_id>=LX_PE_WIN_NUM, return RET_ERROR, \
		"[%s,%d] win_id(%d) error.\n", __F__, __L__, win_id);
	wr_ctrl.base_addr = (win_id==LX_PE_WIN_0)? 0x0:0x2000;
	h_y_idx = pstParams->cur_scl_idx[win_id].h_luma_id;
	h_c_idx = pstParams->cur_scl_idx[win_id].h_chrm_id;
	v_y_idx = pstParams->cur_scl_idx[win_id].v_luma_id;
	v_c_idx = pstParams->cur_scl_idx[win_id].v_chrm_id;
	h_y_tap = pstParams->h_y_tap[win_id];
	h_c_tap = pstParams->h_c_tap[win_id];
	v_y_tap = pstParams->v_y_tap[win_id];
	v_c_tap = pstParams->v_c_tap[win_id];
	if(flag.h_y)
	{
		is_v12tap = (h_y_tap==PE_SHP_HW_M14_SCL_V12T_NORM || \
			h_y_tap==PE_SHP_HW_M14_SCL_V12T_DN14)? 1:0;
		wr_ctrl.tbl_size = (is_v12tap)? \
			PE_SHP_SCL_HW_Y_H_12T_NUM:PE_SHP_SCL_HW_Y_H_08T_NUM;
		wr_ctrl.tap_type = (is_v12tap)? \
			PE_SHP_HW_M14_SCL_WR_TAP_V12TM_Y_H_12T:PE_SHP_HW_M14_SCL_WR_TAP_V06TM_Y_H_08T;
		switch(h_y_tap)
		{
			case PE_SHP_HW_M14_SCL_V12T_NORM:
				PE_SHP_HW_M14_WR_SCL_COEFF(win_id, wr_ctrl, \
					shp_scl_y_h_12tap_coeff[h_y_idx][0], h_y_idx);
				break;
			case PE_SHP_HW_M14_SCL_V12T_DN14:
				PE_SHP_HW_M14_WR_SCL_COEFF(win_id, wr_ctrl, \
					shp_scl_y_h_12tap_dn50_coeff[0], 0);
				break;
			case PE_SHP_HW_M14_SCL_V06T_DN75:
				PE_SHP_HW_M14_WR_SCL_COEFF(win_id, wr_ctrl, \
					shp_scl_y_h_08tap_dn75_coeff[0], 0);
				break;
			case PE_SHP_HW_M14_SCL_V06T_DN50:
				PE_SHP_HW_M14_WR_SCL_COEFF(win_id, wr_ctrl, \
					shp_scl_y_h_08tap_dn50_coeff[0], 0);
				break;
			case PE_SHP_HW_M14_SCL_V06T_DN25:
				PE_SHP_HW_M14_WR_SCL_COEFF(win_id, wr_ctrl, \
					shp_scl_y_h_08tap_dn25_coeff[0], 0);
				break;
			case PE_SHP_HW_M14_SCL_V06T_NORM:
			case PE_SHP_HW_M14_SCL_V06T_HALF:
			default:
				PE_SHP_HW_M14_WR_SCL_COEFF(win_id, wr_ctrl, \
					shp_scl_y_h_08tap_coeff[h_y_idx][0], h_y_idx);
				break;
		}
	}
	if(flag.h_c)
	{
		is_v12tap = (h_c_tap==PE_SHP_HW_M14_SCL_V12T_NORM || \
			h_c_tap==PE_SHP_HW_M14_SCL_V12T_DN14)? 1:0;
		wr_ctrl.tbl_size = PE_SHP_SCL_HW_C_08T_NUM;
		wr_ctrl.tap_type = (is_v12tap)? \
			PE_SHP_HW_M14_SCL_WR_TAP_V12TM_C_H_08T:PE_SHP_HW_M14_SCL_WR_TAP_V06TM_C_H_08T;
		switch(h_c_tap)
		{
			case PE_SHP_HW_M14_SCL_V12T_NORM:
				PE_SHP_HW_M14_WR_SCL_COEFF(win_id, wr_ctrl, \
					shp_scl_c_08tap_coeff[h_c_idx][0], h_c_idx);
				break;
			case PE_SHP_HW_M14_SCL_V12T_DN14:
				PE_SHP_HW_M14_WR_SCL_COEFF(win_id, wr_ctrl, \
					shp_scl_c_08tap_dn25_coeff[0], 0);
				break;
			case PE_SHP_HW_M14_SCL_V06T_DN75:
				PE_SHP_HW_M14_WR_SCL_COEFF(win_id, wr_ctrl, \
					shp_scl_c_08tap_dn75_coeff[0], 0);
				break;
			case PE_SHP_HW_M14_SCL_V06T_DN50:
				PE_SHP_HW_M14_WR_SCL_COEFF(win_id, wr_ctrl, \
					shp_scl_c_08tap_dn50_coeff[0], 0);
				break;
			case PE_SHP_HW_M14_SCL_V06T_DN25:
				PE_SHP_HW_M14_WR_SCL_COEFF(win_id, wr_ctrl, \
					shp_scl_c_08tap_dn25_coeff[0], 0);
				break;
			case PE_SHP_HW_M14_SCL_V06T_NORM:
			case PE_SHP_HW_M14_SCL_V06T_HALF:
			default:
				PE_SHP_HW_M14_WR_SCL_COEFF(win_id, wr_ctrl, \
					shp_scl_c_08tap_coeff[h_c_idx][0], h_c_idx);
				break;
		}
		#ifndef PE_SHP_HW_M14_USE_V12TM_C08T_SEPARATE_HV
		if(flag.v_c && is_v12tap)
		{
			flag.v_c = 0;	// avoid double setting
		}
		#endif
	}
	if(flag.v_y)
	{
		is_v12tap = (v_y_tap==PE_SHP_HW_M14_SCL_V12T_NORM || \
			v_y_tap==PE_SHP_HW_M14_SCL_V12T_DN14)? 1:0;
		wr_ctrl.tbl_size = (is_v12tap)? \
			PE_SHP_SCL_HW_Y_V_12T_NUM:PE_SHP_SCL_HW_Y_V_06T_NUM;
		wr_ctrl.tap_type = (is_v12tap)? \
			PE_SHP_HW_M14_SCL_WR_TAP_V12TM_Y_V_12T:PE_SHP_HW_M14_SCL_WR_TAP_V06TM_Y_V_06T;
		switch(v_y_tap)
		{
			case PE_SHP_HW_M14_SCL_V12T_NORM:
				PE_SHP_HW_M14_WR_SCL_COEFF(win_id, wr_ctrl, \
					shp_scl_y_v_12tap_coeff[v_y_idx][0], v_y_idx);
				break;
			case PE_SHP_HW_M14_SCL_V12T_DN14:
				PE_SHP_HW_M14_WR_SCL_COEFF(win_id, wr_ctrl, \
					shp_scl_y_v_12tap_dn14_coeff[0], 0);
				break;
			case PE_SHP_HW_M14_SCL_V06T_HALF:
				PE_SHP_HW_M14_WR_SCL_COEFF(win_id, wr_ctrl, \
					shp_scl_y_v_06tap_half_coeff[v_y_idx][0], v_y_idx);
				break;
			case PE_SHP_HW_M14_SCL_V06T_DN75:
				PE_SHP_HW_M14_WR_SCL_COEFF(win_id, wr_ctrl, \
					shp_scl_y_v_06tap_dn75_coeff[0], 0);
				break;
			case PE_SHP_HW_M14_SCL_V06T_DN50:
				PE_SHP_HW_M14_WR_SCL_COEFF(win_id, wr_ctrl, \
					shp_scl_y_v_06tap_dn50_coeff[0], 0);
				break;
			case PE_SHP_HW_M14_SCL_V06T_DN25:
				PE_SHP_HW_M14_WR_SCL_COEFF(win_id, wr_ctrl, \
					shp_scl_y_v_06tap_dn25_coeff[0], 0);
			break;
			case PE_SHP_HW_M14_SCL_V06T_NORM:
			default:
				PE_SHP_HW_M14_WR_SCL_COEFF(win_id, wr_ctrl, \
					shp_scl_y_v_06tap_coeff[v_y_idx][0], v_y_idx);
				break;
		}
	}
	if(flag.v_c)
	{
		is_v12tap = (v_c_tap==PE_SHP_HW_M14_SCL_V12T_NORM || \
			v_c_tap==PE_SHP_HW_M14_SCL_V12T_DN14)? 1:0;
		wr_ctrl.tbl_size = (is_v12tap)? \
			PE_SHP_SCL_HW_C_08T_NUM:PE_SHP_SCL_HW_C_04T_NUM;
		wr_ctrl.tap_type = (is_v12tap)? \
			PE_SHP_HW_M14_SCL_WR_TAP_V12TM_C_V_08T:PE_SHP_HW_M14_SCL_WR_TAP_V06TM_C_V_04T;
		switch(v_c_tap)
		{
			case PE_SHP_HW_M14_SCL_V12T_NORM:
				PE_SHP_HW_M14_WR_SCL_COEFF(win_id, wr_ctrl, \
					shp_scl_c_08tap_coeff[v_c_idx][0], v_c_idx);
				break;
			case PE_SHP_HW_M14_SCL_V12T_DN14:
				PE_SHP_HW_M14_WR_SCL_COEFF(win_id, wr_ctrl, \
					shp_scl_c_08tap_dn25_coeff[0], 0);
				break;
			case PE_SHP_HW_M14_SCL_V06T_DN75:
				PE_SHP_HW_M14_WR_SCL_COEFF(win_id, wr_ctrl, \
					shp_scl_c_04tap_dn75_coeff[0], 0);
				break;
			case PE_SHP_HW_M14_SCL_V06T_DN50:
				PE_SHP_HW_M14_WR_SCL_COEFF(win_id, wr_ctrl, \
					shp_scl_c_04tap_dn50_coeff[0], 0);
				break;
			case PE_SHP_HW_M14_SCL_V06T_DN25:
				PE_SHP_HW_M14_WR_SCL_COEFF(win_id, wr_ctrl, \
					shp_scl_c_04tap_dn25_coeff[0], 0);
				break;
			case PE_SHP_HW_M14_SCL_V06T_NORM:
			case PE_SHP_HW_M14_SCL_V06T_HALF:
			default:
				PE_SHP_HW_M14_WR_SCL_COEFF(win_id, wr_ctrl, \
					shp_scl_c_04tap_coeff[v_c_idx][0], v_c_idx);
				break;
		}
	}
	return ret;
}
/**
 * write scl coeff data
 *	- to avoid using PE_REG_WD()(ioremap and iounmap) every time
 *
 * @param   wr_ctrl [in] PE_SHP_HW_M14_SCL_WR_CTRL_T
 * @param   *reg [in] PE_REG_PARAM_T
 * @return  void
 * @see
 * @author
 */
static void PE_SHP_HW_M14_WrSclCoeff(PE_SHP_HW_M14_SCL_WR_CTRL_T wr_ctrl, const PE_REG_PARAM_T *reg)
{
	int ret = RET_OK;
	UINT32 i=0, wr_addr=0, wr_data=0;

#define PE_SHP_HW_M14B_MSC_WR(_a,_d)	\
	PE_MSC_M14B0_Wr(_a,_d);PE_MSC_M14B0_WrFL(_a)
#define PE_SHP_HW_M14A_MSL_WR(_a,_d)	\
	PE_MSL_M14A0_Wr(_a,_d);PE_MSL_M14A0_WrFL(_a)
#define PE_SHP_HW_M14A_MSR_WR(_a,_d)	\
	PE_MSR_M14A0_Wr(_a,_d);PE_MSR_M14A0_WrFL(_a)

	do{
		CHECK_KNULL(reg);
		if(PE_KDRV_VER_M14BX)
		{
			for(i=0;i<wr_ctrl.tbl_size;i++)
			{
				wr_addr = reg[i].addr+wr_ctrl.base_addr;
				#ifdef PE_SHP_HW_M14_USE_V12TM_C08T_SEPARATE_HV
				if(wr_addr==0x1160)
				{
					/* modify MSL MSC_C_FIR_COEF_CTRL_L(0x1160) [6:4]h_v_sel */
					if(wr_ctrl.tap_type==PE_SHP_HW_M14_SCL_WR_TAP_V12TM_C_H_08T)
						wr_data = (reg[i].data&(~0x70))|0x40;
					else if(wr_ctrl.tap_type==PE_SHP_HW_M14_SCL_WR_TAP_V12TM_C_V_08T)
						wr_data = (reg[i].data&(~0x70))|0x20;
					else
						wr_data = reg[i].data;
				}
				else
				{
					wr_data = reg[i].data;
				}
				#else
				wr_data = reg[i].data;
				#endif
				switch(wr_addr)
				{
					case 0x1130:	PE_SHP_HW_M14B_MSC_WR(msc_y_fir_coef_data0,	wr_data);	break;
					case 0x1134:	PE_SHP_HW_M14B_MSC_WR(msc_y_fir_coef_data1,	wr_data);	break;
					case 0x1138:	PE_SHP_HW_M14B_MSC_WR(msc_y_fir_coef_data2,	wr_data);	break;
					case 0x113c:	PE_SHP_HW_M14B_MSC_WR(msc_y_fir_coef_data3,	wr_data);	break;
					case 0x1140:	PE_SHP_HW_M14B_MSC_WR(msc_y_fir_coef_data4,	wr_data);	break;
					case 0x1144:	PE_SHP_HW_M14B_MSC_WR(msc_y_fir_coef_data5,	wr_data);	break;
					case 0x1148:	PE_SHP_HW_M14B_MSC_WR(msc_y_fir_coef_ctrl,	wr_data);	break;
					case 0x1150:	PE_SHP_HW_M14B_MSC_WR(msc_c_fir_coef_data0,	wr_data);	break;
					case 0x1154:	PE_SHP_HW_M14B_MSC_WR(msc_c_fir_coef_data1,	wr_data);	break;
					case 0x1158:	PE_SHP_HW_M14B_MSC_WR(msc_c_fir_coef_data2,	wr_data);	break;
					case 0x115c:	PE_SHP_HW_M14B_MSC_WR(msc_c_fir_coef_data3,	wr_data);	break;
					case 0x1160:	PE_SHP_HW_M14B_MSC_WR(msc_c_fir_coef_ctrl,	wr_data);	break;
					default:
						PE_SHP_HW_M14_ERROR("[%s,%d]wrong case. [%03d]addr:0x%08x,base:0x%08x,data:0x%08x\n",\
							__F__,__L__,i,reg[i].addr,wr_ctrl.base_addr,wr_data);
						break;
				}
			}
		}
		else if(PE_KDRV_VER_M14AX)
		{
			for(i=0;i<wr_ctrl.tbl_size;i++)
			{
				wr_addr = reg[i].addr+wr_ctrl.base_addr;
				#ifdef PE_SHP_HW_M14_USE_V12TM_C08T_SEPARATE_HV
				if(wr_addr==0x1160)
				{
					/* modify MSL MSC_C_FIR_COEF_CTRL_L(0x1160) [6:4]h_v_sel */
					if(wr_ctrl.tap_type==PE_SHP_HW_M14_SCL_WR_TAP_V12TM_C_H_08T)
						wr_data = (reg[i].data&(~0x70))|0x40;
					else if(wr_ctrl.tap_type==PE_SHP_HW_M14_SCL_WR_TAP_V12TM_C_V_08T)
						wr_data = (reg[i].data&(~0x70))|0x20;
					else
						wr_data = reg[i].data;
				}
				else
				{
					wr_data = reg[i].data;
				}
				#else
				wr_data = reg[i].data;
				#endif
				switch(wr_addr)
				{
					case 0x1130:	PE_SHP_HW_M14A_MSL_WR(msc_y_fir_coef_data0_l,	wr_data);	break;
					case 0x1134:	PE_SHP_HW_M14A_MSL_WR(msc_y_fir_coef_data1_l,	wr_data);	break;
					case 0x1138:	PE_SHP_HW_M14A_MSL_WR(msc_y_fir_coef_data2_l,	wr_data);	break;
					case 0x113c:	PE_SHP_HW_M14A_MSL_WR(msc_y_fir_coef_data3_l,	wr_data);	break;
					case 0x1140:	PE_SHP_HW_M14A_MSL_WR(msc_y_fir_coef_data4_l,	wr_data);	break;
					case 0x1144:	PE_SHP_HW_M14A_MSL_WR(msc_y_fir_coef_data5_l,	wr_data);	break;
					case 0x1148:	PE_SHP_HW_M14A_MSL_WR(msc_y_fir_coef_ctrl_l,	wr_data);	break;
					case 0x1150:	PE_SHP_HW_M14A_MSL_WR(msc_c_fir_coef_data0_l,	wr_data);	break;
					case 0x1154:	PE_SHP_HW_M14A_MSL_WR(msc_c_fir_coef_data1_l,	wr_data);	break;
					case 0x1158:	PE_SHP_HW_M14A_MSL_WR(msc_c_fir_coef_data2_l,	wr_data);	break;
					case 0x115c:	PE_SHP_HW_M14A_MSL_WR(msc_c_fir_coef_data3_l,	wr_data);	break;
					case 0x1160:	PE_SHP_HW_M14A_MSL_WR(msc_c_fir_coef_ctrl_l,	wr_data);	break;
					case 0x3130:	PE_SHP_HW_M14A_MSR_WR(msc_y_fir_coef_data0_r,	wr_data);	break;
					case 0x3134:	PE_SHP_HW_M14A_MSR_WR(msc_y_fir_coef_data1_r,	wr_data);	break;
					case 0x3138:	PE_SHP_HW_M14A_MSR_WR(msc_y_fir_coef_data2_r,	wr_data);	break;
					case 0x313c:	PE_SHP_HW_M14A_MSR_WR(msc_y_fir_coef_data3_r,	wr_data);	break;
					case 0x3140:	PE_SHP_HW_M14A_MSR_WR(msc_y_fir_coef_data4_r,	wr_data);	break;
					case 0x3144:	PE_SHP_HW_M14A_MSR_WR(msc_y_fir_coef_data5_r,	wr_data);	break;
					case 0x3148:	PE_SHP_HW_M14A_MSR_WR(msc_y_fir_coef_ctrl_r,	wr_data);	break;
					case 0x3150:	PE_SHP_HW_M14A_MSR_WR(msc_c_fir_coef_data0_r,	wr_data);	break;
					case 0x3154:	PE_SHP_HW_M14A_MSR_WR(msc_c_fir_coef_data1_r,	wr_data);	break;
					case 0x3158:	PE_SHP_HW_M14A_MSR_WR(msc_c_fir_coef_data2_r,	wr_data);	break;
					case 0x315c:	PE_SHP_HW_M14A_MSR_WR(msc_c_fir_coef_data3_r,	wr_data);	break;
					case 0x3160:	PE_SHP_HW_M14A_MSR_WR(msc_c_fir_coef_ctrl_r,	wr_data);	break;
					default:
						PE_SHP_HW_M14_ERROR("[%s,%d]wrong case. [%03d]addr:0x%08x,base:0x%08x,data:0x%08x\n",\
							__F__,__L__,i,reg[i].addr,wr_ctrl.base_addr,wr_data);
						break;
				}
			}
		}
	}while(0);
	return;
}
/**
 * get current shp settings
 *
 * @param   *pstParams [in] PE_SHP_HW_M14_SETTINGS_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_SHP_HW_M14_GetCurShpSettings(PE_SHP_HW_M14_SETTINGS_T *pstParams)
{
	int ret = RET_OK;
	static UINT32 count = 0;
	PE_SHP_HW_M14_SETTINGS_T *pInfo = &_g_pe_shp_hw_m14_info;
	LX_PE_SHP_SCLFILTER_T *cur_scl0_info = &_g_pe_shp_hw_m14_info.cur_scl_idx[LX_PE_WIN_0];
	LX_PE_SHP_SCLFILTER_T *cur_scl1_info = &_g_pe_shp_hw_m14_info.cur_scl_idx[LX_PE_WIN_1];
	LX_PE_SHP_SCLFILTER_T *pre_scl0_info = &_g_pe_shp_hw_m14_info.pre_scl_idx[LX_PE_WIN_0];
	LX_PE_SHP_SCLFILTER_T *pre_scl1_info = &_g_pe_shp_hw_m14_info.pre_scl_idx[LX_PE_WIN_1];
	do{
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_M14)
		{
			memcpy(pstParams,pInfo,sizeof(PE_SHP_HW_M14_SETTINGS_T));
			if(_g_shp_hw_m14_trace)
			{
				if(count>PE_PRINT_COUNT_NUM)
				{
					PE_SHP_HW_M14_DBG_PRINT("get cur_scl_idx[0] : wid:%d, luma h,v:%d,%d, chrm h,v:%d,%d\n",\
						cur_scl0_info->win_id,cur_scl0_info->h_luma_id,cur_scl0_info->v_luma_id,\
						cur_scl0_info->h_chrm_id,cur_scl0_info->v_chrm_id);
					PE_SHP_HW_M14_DBG_PRINT("get cur_scl_idx[1] : wid:%d, luma h,v:%d,%d, chrm h,v:%d,%d\n",\
						cur_scl1_info->win_id,cur_scl1_info->h_luma_id,cur_scl1_info->v_luma_id,\
						cur_scl1_info->h_chrm_id,cur_scl1_info->v_chrm_id);
					PE_SHP_HW_M14_DBG_PRINT("get pre_scl_idx[0] : wid:%d, luma h,v:%d,%d, chrm h,v:%d,%d\n",\
						pre_scl0_info->win_id,pre_scl0_info->h_luma_id,pre_scl0_info->v_luma_id,\
						pre_scl0_info->h_chrm_id,pre_scl0_info->v_chrm_id);
					PE_SHP_HW_M14_DBG_PRINT("get pre_scl_idx[1] : wid:%d, luma h,v:%d,%d, chrm h,v:%d,%d\n",\
						pre_scl1_info->win_id,pre_scl1_info->h_luma_id,pre_scl1_info->v_luma_id,\
						pre_scl1_info->h_chrm_id,pre_scl1_info->v_chrm_id);
					PE_SHP_HW_M14_DBG_PRINT("get h_y_tap:%d,%d, h_c_tap:%d,%d, v_y_tap:%d,%d, v_c_tap:%d,%d\n",\
						pInfo->h_y_tap[LX_PE_WIN_0],pInfo->h_y_tap[LX_PE_WIN_1],\
						pInfo->h_c_tap[LX_PE_WIN_0],pInfo->h_c_tap[LX_PE_WIN_1],\
						pInfo->v_y_tap[LX_PE_WIN_0],pInfo->v_y_tap[LX_PE_WIN_1],\
						pInfo->v_c_tap[LX_PE_WIN_0],pInfo->v_c_tap[LX_PE_WIN_1]);
					PE_SHP_HW_M14_DBG_PRINT("get y_fir:%d,%d, c_fir:%d,%d, pre_res_on:%d,%d\n",\
						pInfo->y_fir[LX_PE_WIN_0],pInfo->y_fir[LX_PE_WIN_1],\
						pInfo->c_fir[LX_PE_WIN_0],pInfo->c_fir[LX_PE_WIN_1],\
						pInfo->pre_res_en[LX_PE_WIN_0],pInfo->pre_res_en[LX_PE_WIN_1]);
					count=0;
				}
				count++;
			}
		}
		else
		{
			PE_SHP_HW_M14_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}

/**
 * read scaler filter coeff
 *
 * @param   *pstParams [in/out] LX_PE_ETC_TBL_T
 * @return  OK if success, ERROR otherwise.
 * @see     
 * @author
 */
int PE_SHP_HW_M14_RdSclFltCoeff(LX_PE_ETC_TBL_T *pstParams)
{
	int ret = RET_OK;
	char buffer[PE_TRACE_STR_SIZE];
	UINT32 sel=0, ofst=0, is_valid=0, step=0, cmd=0;
	static UINT32 rd_cnt=0;
	static UINT32 scldata[255];
	PE_SHP_HW_M14_SETTINGS_T *pInfo=&_g_pe_shp_hw_m14_info;
	PE_SHP_HW_M14_SCL h_y_tap0 = _g_pe_shp_hw_m14_info.h_y_tap[LX_PE_WIN_0];

#define WIN0	LX_PE_WIN_0
#define WIN1	LX_PE_WIN_1
#define PE_SHP_M14_PRINT_START		"START_OF_PRINT"
#define PE_SHP_M14_PRINT_END		"END_OF_PRINT"
#define PE_SHP_M14_PRINT_EXIT		"EXIT_OF_PRINT"
#define PE_SHP_M14_PRINT_RESRV		"RESERVED"
#define PE_SHP_M14_CASE_PRMENU(_cnt, _action, _buf, _item)	\
	case (_cnt):sprintf(_buf, "   [0x%02x] %s\n", (_item), #_item);_action;break
#define PE_SHP_M14_CASE_SPRINT(_cnt, _action, _buf, fmt, args...)	\
	case (_cnt):sprintf(_buf, fmt, ##args);_action;break
#define PE_SHP_M14_DFLT_SPRINT(_action, _buf, fmt, args...)		\
	default:sprintf(_buf, fmt, ##args);_action;break

	do{
		CHECK_KNULL(pstParams);
		CHECK_KNULL(pstParams->data);
		if (PE_KDRV_VER_M14)
		{
			if (pstParams->data[0]>=PE_SHP_HW_M14_RD_SCL_MAX)
			{
				sprintf(buffer, "%s", PE_SHP_M14_PRINT_EXIT);
				rd_cnt = 0;
			}
			else if (pstParams->data[0]==PE_SHP_HW_M14_RD_SCL_DISPLAY)
			{
				switch (rd_cnt)
				{
					PE_SHP_M14_CASE_SPRINT( 0, rd_cnt++, buffer, "%s", PE_SHP_M14_PRINT_START);
					PE_SHP_M14_CASE_SPRINT( 1, rd_cnt++, buffer,\
						"   tap_type(0~6:12t,14dn,6t,6t_hf,75dn,50dn,25dn)\n");
					PE_SHP_M14_CASE_SPRINT( 2, rd_cnt++, buffer,\
						"   MSL hy,vy,hc,vc tap:%d,%d,%d,%d\n", \
						pInfo->h_y_tap[WIN0], pInfo->v_y_tap[WIN0], \
						pInfo->h_c_tap[WIN0], pInfo->h_c_tap[WIN0]);
					PE_SHP_M14_CASE_SPRINT( 3, rd_cnt++, buffer,\
						"   MSL hy,vy,hc,vc idx:%d,%d,%d,%d\n", \
						pInfo->pre_scl_idx[WIN0].h_luma_id, pInfo->pre_scl_idx[WIN0].v_luma_id, \
						pInfo->pre_scl_idx[WIN0].h_chrm_id, pInfo->pre_scl_idx[WIN0].v_chrm_id);
					PE_SHP_M14_CASE_SPRINT( 4, rd_cnt++, buffer,\
						"   MSR hy,vy,hc,vc tap:%d,%d,%d,%d\n", \
						pInfo->h_y_tap[WIN1], pInfo->v_y_tap[WIN1], \
						pInfo->h_c_tap[WIN1], pInfo->h_c_tap[WIN1]);
					PE_SHP_M14_CASE_SPRINT( 5, rd_cnt++, buffer,\
						"   MSR hy,vy,hc,vc idx:%d,%d,%d,%d\n", \
						pInfo->pre_scl_idx[WIN1].h_luma_id, pInfo->pre_scl_idx[WIN1].v_luma_id, \
						pInfo->pre_scl_idx[WIN1].h_chrm_id, pInfo->pre_scl_idx[WIN1].v_chrm_id);
					PE_SHP_M14_CASE_SPRINT( 6, rd_cnt++, buffer, "%s", PE_SHP_M14_PRINT_RESRV);
					PE_SHP_M14_CASE_SPRINT( 7, rd_cnt++, buffer, "%s", PE_SHP_M14_PRINT_RESRV);
					PE_SHP_M14_CASE_SPRINT( 8, rd_cnt++, buffer, "%s", PE_SHP_M14_PRINT_RESRV);
					PE_SHP_M14_CASE_SPRINT( 9, rd_cnt++, buffer, "%s", PE_SHP_M14_PRINT_RESRV);
					PE_SHP_M14_CASE_SPRINT(10, rd_cnt++, buffer, "%s", PE_SHP_M14_PRINT_RESRV);
					PE_SHP_M14_CASE_PRMENU(11, rd_cnt++, buffer, PE_SHP_HW_M14_RD_SCL_MSL_Y_H);
					PE_SHP_M14_CASE_PRMENU(12, rd_cnt++, buffer, PE_SHP_HW_M14_RD_SCL_MSL_Y_V);
					PE_SHP_M14_CASE_PRMENU(13, rd_cnt++, buffer, PE_SHP_HW_M14_RD_SCL_MSL_C_H);
					PE_SHP_M14_CASE_PRMENU(14, rd_cnt++, buffer, PE_SHP_HW_M14_RD_SCL_MSL_C_V);
					PE_SHP_M14_CASE_PRMENU(15, rd_cnt++, buffer, PE_SHP_HW_M14_RD_SCL_MSR_Y_H);
					PE_SHP_M14_CASE_PRMENU(16, rd_cnt++, buffer, PE_SHP_HW_M14_RD_SCL_MSR_Y_V);
					PE_SHP_M14_CASE_PRMENU(17, rd_cnt++, buffer, PE_SHP_HW_M14_RD_SCL_MSR_C_H);
					PE_SHP_M14_CASE_PRMENU(18, rd_cnt++, buffer, PE_SHP_HW_M14_RD_SCL_MSR_C_V);
					PE_SHP_M14_CASE_PRMENU(19, rd_cnt++, buffer, PE_SHP_HW_M14_RD_SCL_SSC_Y_H);
					PE_SHP_M14_DFLT_SPRINT(rd_cnt=0, buffer, "%s", PE_SHP_M14_PRINT_END);
				}
			}
			else
			{
				is_valid = 0;
				cmd = pstParams->data[0];
				switch (cmd)
				{
					case PE_SHP_HW_M14_RD_SCL_MSL_Y_H:
					case PE_SHP_HW_M14_RD_SCL_MSL_Y_V:
						if(h_y_tap0==PE_SHP_HW_M14_SCL_V12T_NORM || \
							h_y_tap0==PE_SHP_HW_M14_SCL_V12T_DN14)	//12tap mode
						{
							if(rd_cnt==0)
							{
								PE_SHP_HW_M14_SetSclFltRnwMode(0x1);	//read mode
								sel = (cmd==PE_SHP_HW_M14_RD_SCL_MSL_Y_H)? 0x4:0x2;	//def flt h:v
								ofst = PE_SHP_HW_M14_RdLumaSclFltCoeff(WIN0,sel,&(scldata[0]));
								sel = (cmd==PE_SHP_HW_M14_RD_SCL_MSL_Y_H)? 0x5:0x3;	//sub flt h:v
								ofst = PE_SHP_HW_M14_RdLumaSclFltCoeff(WIN0,sel,&(scldata[ofst]));
								PE_SHP_HW_M14_SetSclFltRnwMode(0x0);	//write mode back
							}
							if(rd_cnt<16)
							{
								step = rd_cnt*7;
								sprintf(buffer, \
									"MSL y %s:[%02d]0x%08x,0x%08x,0x%08x,0x%08x,0x%08x,0x%08x\n", \
									(cmd==PE_SHP_HW_M14_RD_SCL_MSL_Y_H)? "h":"v", scldata[step], \
									scldata[step+1], scldata[step+2], scldata[step+3], \
									scldata[step+4], scldata[step+5], scldata[step+6]);
								is_valid = 1;
							}
							else if(rd_cnt<32)
							{
								step = rd_cnt*7;
								sprintf(buffer, \
									"MSL y %s:[%02d]0x%08x,0x%08x,0x%08x,0x%08x\n", \
									(cmd==PE_SHP_HW_M14_RD_SCL_MSL_Y_H)? "h":"v", scldata[step], \
									scldata[step+1], scldata[step+2], scldata[step+3], \
									scldata[step+4]);
								is_valid = 1;
							}
						}
						else	//6tap mode
						{
							if(rd_cnt==0)
							{
								PE_SHP_HW_M14_SetSclFltRnwMode(0x1);	//read mode
								sel = (cmd==PE_SHP_HW_M14_RD_SCL_MSL_Y_H)? 0x0:0x1;	//flt h:v
								ofst = PE_SHP_HW_M14_RdLumaSclFltCoeff(WIN0,sel,&(scldata[0]));
								PE_SHP_HW_M14_SetSclFltRnwMode(0x0);	//write mode back
							}
							if(rd_cnt<16)
							{
								step = rd_cnt*7;
								sprintf(buffer, \
									"MSL y %s:[%02d]0x%08x,0x%08x,0x%08x,0x%08x,0x%08x,0x%08x\n", \
									(cmd==PE_SHP_HW_M14_RD_SCL_MSL_Y_H)? "h":"v", scldata[step], \
									scldata[step+1], scldata[step+2], scldata[step+3], \
									scldata[step+4], scldata[step+5], scldata[step+6]);
								is_valid = 1;
							}
						}
						break;
					case PE_SHP_HW_M14_RD_SCL_MSL_C_H:
					case PE_SHP_HW_M14_RD_SCL_MSL_C_V:
						if(h_y_tap0==PE_SHP_HW_M14_SCL_V12T_NORM || \
							h_y_tap0==PE_SHP_HW_M14_SCL_V12T_DN14)	//12tap mode
						{
							if(rd_cnt==0)
							{
								PE_SHP_HW_M14_SetSclFltRnwMode(0x1);	//read mode
								#ifdef PE_SHP_HW_M14_USE_V12TM_C08T_SEPARATE_HV
								sel = (cmd==PE_SHP_HW_M14_RD_SCL_MSL_C_H)? 0x4:0x2;	//def flt h:v
								#else
								sel = 0x0;	//def flt h&v
								#endif
								ofst = PE_SHP_HW_M14_RdChromaSclFltCoeff(WIN0,sel,&(scldata[0]));
								PE_SHP_HW_M14_SetSclFltRnwMode(0x0);	//write mode back
							}
							if(rd_cnt<16)
							{
								step = rd_cnt*5;
								sprintf(buffer, \
									"MSL c %s:[%02d]0x%08x,0x%08x,0x%08x,0x%08x\n", \
									(cmd==PE_SHP_HW_M14_RD_SCL_MSL_C_H)? "h":"v", scldata[step], \
									scldata[step+1], scldata[step+2], scldata[step+3], \
									scldata[step+4]);
								is_valid = 1;
							}
						}
						else	//6tap mode
						{
							if(rd_cnt==0)
							{
								PE_SHP_HW_M14_SetSclFltRnwMode(0x1);	//read mode
								sel = (cmd==PE_SHP_HW_M14_RD_SCL_MSL_C_H)? 0x0:0x1;	//flt h:v
								ofst = PE_SHP_HW_M14_RdChromaSclFltCoeff(WIN0,sel,&(scldata[0]));
								PE_SHP_HW_M14_SetSclFltRnwMode(0x0);	//write mode back
							}
							if(rd_cnt<16)
							{
								step = rd_cnt*5;
								if(cmd==PE_SHP_HW_M14_RD_SCL_MSL_C_H)
								{
									sprintf(buffer, "MSL c h:[%02d]0x%08x,0x%08x,0x%08x,0x%08x\n", \
										scldata[step], scldata[step+1], scldata[step+2], \
										scldata[step+3], scldata[step+4]);
								}
								else
								{
									sprintf(buffer, "MSL c v:[%02d]0x%08x,0x%08x\n", \
										scldata[step], scldata[step+1], scldata[step+2]);
								}
								is_valid = 1;
							}
						}
						break;
					case PE_SHP_HW_M14_RD_SCL_MSR_Y_H:
					case PE_SHP_HW_M14_RD_SCL_MSR_Y_V:
						if(rd_cnt==0)
						{
							PE_SHP_HW_M14_SetSclFltRnwMode(0x1);	//read mode
							sel = (cmd==PE_SHP_HW_M14_RD_SCL_MSR_Y_H)? 0x0:0x1;	//flt h:v
							ofst = PE_SHP_HW_M14_RdLumaSclFltCoeff(WIN1,sel,&(scldata[0]));
							PE_SHP_HW_M14_SetSclFltRnwMode(0x0);	//write mode back
						}
						if(rd_cnt<16)
						{
							step = rd_cnt*7;
							sprintf(buffer, \
								"MSL y %s:[%02d]0x%08x,0x%08x,0x%08x,0x%08x,0x%08x,0x%08x\n", \
								(cmd==PE_SHP_HW_M14_RD_SCL_MSR_Y_H)? "h":"v", scldata[step], \
								scldata[step+1], scldata[step+2], scldata[step+3], \
								scldata[step+4], scldata[step+5], scldata[step+6]);
							is_valid = 1;
						}
						break;
					case PE_SHP_HW_M14_RD_SCL_MSR_C_H:
					case PE_SHP_HW_M14_RD_SCL_MSR_C_V:
						if(rd_cnt==0)
						{
							PE_SHP_HW_M14_SetSclFltRnwMode(0x1);	//read mode
							sel = (cmd==PE_SHP_HW_M14_RD_SCL_MSR_C_H)? 0x0:0x1;	//flt h:v
							ofst = PE_SHP_HW_M14_RdChromaSclFltCoeff(WIN1,sel,&(scldata[0]));
							PE_SHP_HW_M14_SetSclFltRnwMode(0x0);	//write mode back
						}
						if(rd_cnt<16)
						{
							step = rd_cnt*5;
							if(cmd==PE_SHP_HW_M14_RD_SCL_MSR_C_H)
							{
								sprintf(buffer, "MSL c h:[%02d]0x%08x,0x%08x,0x%08x,0x%08x\n", \
									scldata[step], scldata[step+1], scldata[step+2], \
									scldata[step+3], scldata[step+4]);
							}
							else
							{
								sprintf(buffer, "MSL c v:[%02d]0x%08x,0x%08x\n", \
									scldata[step], scldata[step+1], scldata[step+2]);
							}
							is_valid = 1;
						}
						break;
					case PE_SHP_HW_M14_RD_SCL_SSC_Y_H:
					default:
						if(rd_cnt==0)
						{
							PE_SHP_HW_M14_SetSclFltRnwMode(0x1);	//read mode
							sel = 0x0;	//h flt
							ofst = PE_SHP_HW_M14_RdSubSclFltCoeff(sel,&(scldata[0]));
							PE_SHP_HW_M14_SetSclFltRnwMode(0x0);	//write mode back
						}
						if(rd_cnt<16)
						{
							step = rd_cnt*5;
							sprintf(buffer, "SSC y h:[%02d]0x%08x,0x%08x,0x%08x,0x%08x\n", \
								scldata[step], scldata[step+1], scldata[step+2], \
								scldata[step+3], scldata[step+4]);
							is_valid = 1;
						}
						break;
				}
				if(is_valid)
				{
					rd_cnt++;
				}
				else
				{
					sprintf(buffer, "%s", PE_SHP_M14_PRINT_END);
					rd_cnt = 0;
				}
			}
			memcpy(pstParams->data, buffer, sizeof(char)*PE_TRACE_STR_SIZE);
		}
	}while (0);
	return ret;
}
/**
 * set scaler filter rd and wr mode
 *
 * @param   rnw_mode [in] UINT32
 * @return  void
 * @see     
 * @author
 */
static void PE_SHP_HW_M14_SetSclFltRnwMode(UINT32 rnw_mode)
{
	int ret = RET_OK;
	if(PE_KDRV_VER_M14BX)
	{
		PE_MSC_M14B0_RdFL(msc_y_fir_coef_ctrl);
		PE_MSC_M14B0_Wr01(msc_y_fir_coef_ctrl,	rnw,	rnw_mode? 0x1:0x0);	//0:wr, 1:rd
		PE_MSC_M14B0_WrFL(msc_y_fir_coef_ctrl);
		PE_MSC_M14B0_RdFL(msc_c_fir_coef_ctrl);
		PE_MSC_M14B0_Wr01(msc_c_fir_coef_ctrl,	rnw,	rnw_mode? 0x1:0x0);	//0:wr, 1:rd
		PE_MSC_M14B0_WrFL(msc_c_fir_coef_ctrl);

		PE_SSC_M14B0_RdFL(ssc_h_fir_coef_ctrl);
		PE_SSC_M14B0_Wr01(ssc_h_fir_coef_ctrl,	rnw,	rnw_mode? 0x1:0x0);	//0:wr, 1:rd
		PE_SSC_M14B0_WrFL(ssc_h_fir_coef_ctrl);
	}
	else if(PE_KDRV_VER_M14AX)
	{
		PE_MSL_M14A0_RdFL(msc_y_fir_coef_ctrl_l);
		PE_MSL_M14A0_Wr01(msc_y_fir_coef_ctrl_l,	rnw,	rnw_mode? 0x1:0x0);	//0:wr, 1:rd
		PE_MSL_M14A0_WrFL(msc_y_fir_coef_ctrl_l);
		PE_MSL_M14A0_RdFL(msc_c_fir_coef_ctrl_l);
		PE_MSL_M14A0_Wr01(msc_c_fir_coef_ctrl_l,	rnw,	rnw_mode? 0x1:0x0);	//0:wr, 1:rd
		PE_MSL_M14A0_WrFL(msc_c_fir_coef_ctrl_l);
		PE_MSR_M14A0_RdFL(msc_y_fir_coef_ctrl_r);
		PE_MSR_M14A0_Wr01(msc_y_fir_coef_ctrl_r,	rnw,	rnw_mode? 0x1:0x0);	//0:wr, 1:rd
		PE_MSR_M14A0_WrFL(msc_y_fir_coef_ctrl_r);
		PE_MSR_M14A0_RdFL(msc_c_fir_coef_ctrl_r);
		PE_MSR_M14A0_Wr01(msc_c_fir_coef_ctrl_r,	rnw,	rnw_mode? 0x1:0x0);	//0:wr, 1:rd
		PE_MSR_M14A0_WrFL(msc_c_fir_coef_ctrl_r);

		PE_SSC_M14A0_RdFL(ssc_h_fir_coef_ctrl);
		PE_SSC_M14A0_Wr01(ssc_h_fir_coef_ctrl,		rnw,	rnw_mode? 0x1:0x0);	//0:wr, 1:rd
		PE_SSC_M14A0_WrFL(ssc_h_fir_coef_ctrl);
	}
	else
	{
		PE_SHP_HW_M14_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
	}
	return;
}
/**
 * read luma scaler filter coeff
 *
 * @param   win_id [in] LX_PE_WIN_ID
 * @param   config [in] UINT32
 * @param   *data [in/out] UINT32
 * @return  UINT32 offset
 * @see     
 * @author
 */
static UINT32 PE_SHP_HW_M14_RdLumaSclFltCoeff(LX_PE_WIN_ID win_id, UINT32 config, UINT32 *data)
{
	int ret = RET_OK;
	UINT32 i,offset;
	do{
		offset=0;
		CHECK_KNULL(data);
		if(PE_KDRV_VER_M14BX)
		{
			if(win_id == LX_PE_WIN_0)	//MSC
			{
				PE_MSC_M14B0_RdFL(msc_y_fir_coef_ctrl);
				PE_MSC_M14B0_Wr01(msc_y_fir_coef_ctrl, h_v_sel,	config);
				PE_MSC_M14B0_WrFL(msc_y_fir_coef_ctrl);
				for(i=0;i<16;i++)
				{
					/* set addr */
					data[offset] = i;	//coeff num 0~15
					PE_MSC_M14B0_Wr01(msc_y_fir_coef_ctrl, addr,	i);
					PE_MSC_M14B0_WrFL(msc_y_fir_coef_ctrl);
					/* set update */
					PE_MSC_M14B0_Wr01(msc_y_fir_coef_ctrl, update,0x1);	//update and auto clear
					PE_MSC_M14B0_WrFL(msc_y_fir_coef_ctrl);
					/* read coeff */
					PE_MSC_M14B0_RdFL(msc_y_fir_coef_data0);
					PE_MSC_M14B0_RdFL(msc_y_fir_coef_data1);
					PE_MSC_M14B0_RdFL(msc_y_fir_coef_data2);
					PE_MSC_M14B0_RdFL(msc_y_fir_coef_data3);
					PE_MSC_M14B0_RdFL(msc_y_fir_coef_data4);
					PE_MSC_M14B0_RdFL(msc_y_fir_coef_data5);
					data[offset+1] = PE_MSC_M14B0_Rd(msc_y_fir_coef_data0);
					data[offset+2] = PE_MSC_M14B0_Rd(msc_y_fir_coef_data1);
					data[offset+3] = PE_MSC_M14B0_Rd(msc_y_fir_coef_data2);
					data[offset+4] = PE_MSC_M14B0_Rd(msc_y_fir_coef_data3);
					data[offset+5] = PE_MSC_M14B0_Rd(msc_y_fir_coef_data4);
					data[offset+6] = PE_MSC_M14B0_Rd(msc_y_fir_coef_data5);
					offset += 7;
				}
			}
			else
			{
				PE_SHP_HW_M14_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
			}
		}
		else if(PE_KDRV_VER_M14AX)
		{
			if(win_id == LX_PE_WIN_0)	//MSL
			{
				PE_MSL_M14A0_RdFL(msc_y_fir_coef_ctrl_l);
				PE_MSL_M14A0_Wr01(msc_y_fir_coef_ctrl_l, h_v_sel,		config);
				PE_MSL_M14A0_WrFL(msc_y_fir_coef_ctrl_l);
				for(i=0;i<16;i++)
				{
					/* set addr */
					data[offset] = i;	//coeff num 0~15
					PE_MSL_M14A0_Wr01(msc_y_fir_coef_ctrl_l, addr,		i);
					PE_MSL_M14A0_WrFL(msc_y_fir_coef_ctrl_l);
					/* set update */
					PE_MSL_M14A0_Wr01(msc_y_fir_coef_ctrl_l, update,	0x1);	//update and auto clear
					PE_MSL_M14A0_WrFL(msc_y_fir_coef_ctrl_l);
					/* read coeff */
					PE_MSL_M14A0_RdFL(msc_y_fir_coef_data0_l);
					PE_MSL_M14A0_RdFL(msc_y_fir_coef_data1_l);
					PE_MSL_M14A0_RdFL(msc_y_fir_coef_data2_l);
					PE_MSL_M14A0_RdFL(msc_y_fir_coef_data3_l);
					PE_MSL_M14A0_RdFL(msc_y_fir_coef_data4_l);
					PE_MSL_M14A0_RdFL(msc_y_fir_coef_data5_l);
					data[offset+1] = PE_MSL_M14A0_Rd(msc_y_fir_coef_data0_l);
					data[offset+2] = PE_MSL_M14A0_Rd(msc_y_fir_coef_data1_l);
					data[offset+3] = PE_MSL_M14A0_Rd(msc_y_fir_coef_data2_l);
					data[offset+4] = PE_MSL_M14A0_Rd(msc_y_fir_coef_data3_l);
					data[offset+5] = PE_MSL_M14A0_Rd(msc_y_fir_coef_data4_l);
					data[offset+6] = PE_MSL_M14A0_Rd(msc_y_fir_coef_data5_l);
					offset += 7;
				}
			}
			else	//LX_PE_WIN_1	//MSR
			{
				PE_MSR_M14A0_RdFL(msc_y_fir_coef_ctrl_r);
				PE_MSR_M14A0_Wr01(msc_y_fir_coef_ctrl_r, h_v_sel,		config);
				PE_MSR_M14A0_WrFL(msc_y_fir_coef_ctrl_r);
				for(i=0;i<16;i++)
				{
					/* set addr */
					data[offset] = i;	//coeff num 0~15
					PE_MSR_M14A0_Wr01(msc_y_fir_coef_ctrl_r, addr,		i);
					PE_MSR_M14A0_WrFL(msc_y_fir_coef_ctrl_r);
					/* set update */
					PE_MSR_M14A0_Wr01(msc_y_fir_coef_ctrl_r, update,	0x1);	//update and auto clear
					PE_MSR_M14A0_WrFL(msc_y_fir_coef_ctrl_r);
					/* read coeff */
					PE_MSR_M14A0_RdFL(msc_y_fir_coef_data0_r);
					PE_MSR_M14A0_RdFL(msc_y_fir_coef_data1_r);
					PE_MSR_M14A0_RdFL(msc_y_fir_coef_data2_r);
					PE_MSR_M14A0_RdFL(msc_y_fir_coef_data3_r);
					PE_MSR_M14A0_RdFL(msc_y_fir_coef_data4_r);
					PE_MSR_M14A0_RdFL(msc_y_fir_coef_data5_r);
					data[offset+1] = PE_MSR_M14A0_Rd(msc_y_fir_coef_data0_r);
					data[offset+2] = PE_MSR_M14A0_Rd(msc_y_fir_coef_data1_r);
					data[offset+3] = PE_MSR_M14A0_Rd(msc_y_fir_coef_data2_r);
					data[offset+4] = PE_MSR_M14A0_Rd(msc_y_fir_coef_data3_r);
					data[offset+5] = PE_MSR_M14A0_Rd(msc_y_fir_coef_data4_r);
					data[offset+6] = PE_MSR_M14A0_Rd(msc_y_fir_coef_data5_r);
					offset +=7;
				}
			}
		}
		else
		{
			PE_SHP_HW_M14_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return offset;
}
/**
 * read chroma scaler filter coeff
 *
 * @param   win_id [in] LX_PE_WIN_ID
 * @param   config [in] UINT32
 * @param   *data [in/out] UINT32
 * @return  UINT32 offset
 * @see     
 * @author
 */
static UINT32 PE_SHP_HW_M14_RdChromaSclFltCoeff(LX_PE_WIN_ID win_id, UINT32 config, UINT32 *data)
{
	int ret = RET_OK;
	UINT32 i,offset;
	do{
		offset=0;
		CHECK_KNULL(data);
		if(PE_KDRV_VER_M14BX)
		{
			if(win_id == LX_PE_WIN_0)	//MSL
			{
				PE_MSC_M14B0_RdFL(msc_c_fir_coef_ctrl);
				PE_MSC_M14B0_Wr01(msc_c_fir_coef_ctrl, h_v_sel,	config);
				PE_MSC_M14B0_WrFL(msc_c_fir_coef_ctrl);
				for(i=0;i<16;i++)
				{
					/* set addr */
					data[offset] = i;	//coeff num 0~15
					PE_MSC_M14B0_RdFL(msc_c_fir_coef_ctrl);
					PE_MSC_M14B0_Wr01(msc_c_fir_coef_ctrl, addr,	i);
					PE_MSC_M14B0_WrFL(msc_c_fir_coef_ctrl);
					/* set update */
					PE_MSC_M14B0_Wr01(msc_c_fir_coef_ctrl, update,0x1);	//update and auto clear
					PE_MSC_M14B0_WrFL(msc_c_fir_coef_ctrl);
					/* read coeff */
					PE_MSC_M14B0_RdFL(msc_c_fir_coef_data0);
					PE_MSC_M14B0_RdFL(msc_c_fir_coef_data1);
					PE_MSC_M14B0_RdFL(msc_c_fir_coef_data2);
					PE_MSC_M14B0_RdFL(msc_c_fir_coef_data3);
					data[offset+1] = PE_MSC_M14B0_Rd(msc_c_fir_coef_data0);
					data[offset+2] = PE_MSC_M14B0_Rd(msc_c_fir_coef_data1);
					data[offset+3] = PE_MSC_M14B0_Rd(msc_c_fir_coef_data2);
					data[offset+4] = PE_MSC_M14B0_Rd(msc_c_fir_coef_data3);
					offset += 5;
				}
			}
		}
		else if(PE_KDRV_VER_M14AX)
		{
			if(win_id == LX_PE_WIN_0)	//MSL
			{
				PE_MSL_M14A0_RdFL(msc_c_fir_coef_ctrl_l);
				PE_MSL_M14A0_Wr01(msc_c_fir_coef_ctrl_l, h_v_sel,		config);
				PE_MSL_M14A0_WrFL(msc_c_fir_coef_ctrl_l);
				for(i=0;i<16;i++)
				{
					/* set addr */
					data[offset] = i;	//coeff num 0~15
					PE_MSL_M14A0_RdFL(msc_c_fir_coef_ctrl_l);
					PE_MSL_M14A0_Wr01(msc_c_fir_coef_ctrl_l, addr,		i);
					PE_MSL_M14A0_WrFL(msc_c_fir_coef_ctrl_l);
					/* set update */
					PE_MSL_M14A0_Wr01(msc_c_fir_coef_ctrl_l, update,	0x1);	//update and auto clear
					PE_MSL_M14A0_WrFL(msc_c_fir_coef_ctrl_l);
					/* read coeff */
					PE_MSL_M14A0_RdFL(msc_c_fir_coef_data0_l);
					PE_MSL_M14A0_RdFL(msc_c_fir_coef_data1_l);
					PE_MSL_M14A0_RdFL(msc_c_fir_coef_data2_l);
					PE_MSL_M14A0_RdFL(msc_c_fir_coef_data3_l);
					data[offset+1] = PE_MSL_M14A0_Rd(msc_c_fir_coef_data0_l);
					data[offset+2] = PE_MSL_M14A0_Rd(msc_c_fir_coef_data1_l);
					data[offset+3] = PE_MSL_M14A0_Rd(msc_c_fir_coef_data2_l);
					data[offset+4] = PE_MSL_M14A0_Rd(msc_c_fir_coef_data3_l);
					offset += 5;
				}
			}
			else	//LX_PE_WIN_1	//MSR
			{
				PE_MSR_M14A0_RdFL(msc_c_fir_coef_ctrl_r);
				PE_MSR_M14A0_Wr01(msc_c_fir_coef_ctrl_r, h_v_sel,		config);
				PE_MSR_M14A0_WrFL(msc_c_fir_coef_ctrl_r);
				for(i=0;i<16;i++)
				{
					/* set addr */
					data[offset] = i;	//coeff num 0~15
					PE_MSR_M14A0_RdFL(msc_c_fir_coef_ctrl_r);
					PE_MSR_M14A0_Wr01(msc_c_fir_coef_ctrl_r, addr,		i);
					PE_MSR_M14A0_WrFL(msc_c_fir_coef_ctrl_r);
					/* set update */
					PE_MSR_M14A0_Wr01(msc_c_fir_coef_ctrl_r, update,	0x1);	//update and auto clear
					PE_MSR_M14A0_WrFL(msc_c_fir_coef_ctrl_r);
					/* read coeff */
					PE_MSR_M14A0_RdFL(msc_c_fir_coef_data0_r);
					PE_MSR_M14A0_RdFL(msc_c_fir_coef_data1_r);
					PE_MSR_M14A0_RdFL(msc_c_fir_coef_data2_r);
					PE_MSR_M14A0_RdFL(msc_c_fir_coef_data3_r);
					data[offset+1] = PE_MSR_M14A0_Rd(msc_c_fir_coef_data0_r);
					data[offset+2] = PE_MSR_M14A0_Rd(msc_c_fir_coef_data1_r);
					data[offset+3] = PE_MSR_M14A0_Rd(msc_c_fir_coef_data2_r);
					data[offset+4] = PE_MSR_M14A0_Rd(msc_c_fir_coef_data3_r);
					offset += 5;
				}
			}
		}
		else
		{
			PE_SHP_HW_M14_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return offset;
}
/**
 * read sub scaler filter coeff
 *
 * @param   config [in] UINT32
 * @param   *data [in/out] UINT32
 * @return  OK if success, ERROR otherwise.
 * @see     
 * @author
 */
static UINT32 PE_SHP_HW_M14_RdSubSclFltCoeff(UINT32 config, UINT32 *data)
{
	int ret = RET_OK;
	UINT32 i,offset;
	do{
		offset=0;
		CHECK_KNULL(data);
		if(PE_KDRV_VER_M14BX)
		{
			PE_SSC_M14B0_RdFL(ssc_win_in_size);
			PE_SSC_M14B0_Wr01(ssc_win_in_size,	in_win_h_size,	0x0);
			PE_SSC_M14B0_Wr01(ssc_win_in_size,	in_win_v_size,	0x0);
			PE_SSC_M14B0_WrFL(ssc_win_in_size);

			PE_SSC_M14B0_RdFL(ssc_h_fir_coef_ctrl);
			PE_SSC_M14B0_Wr01(ssc_h_fir_coef_ctrl, h_v_sel,		config);
			PE_SSC_M14B0_WrFL(ssc_h_fir_coef_ctrl);
			for(i=0;i<16;i++)
			{
				/* set addr */
				data[offset] = i;	//coeff num 0~15
				PE_SSC_M14B0_Wr01(ssc_h_fir_coef_ctrl, addr,		i);
				PE_SSC_M14B0_WrFL(ssc_h_fir_coef_ctrl);
				/* set update */
				PE_SSC_M14B0_Wr01(ssc_h_fir_coef_ctrl, update,		0x1);	//update and auto clear
				PE_SSC_M14B0_WrFL(ssc_h_fir_coef_ctrl);
				/* read coeff */
				PE_SSC_M14B0_RdFL(ssc_h_fir_coef_data0);
				PE_SSC_M14B0_RdFL(ssc_h_fir_coef_data1);
				PE_SSC_M14B0_RdFL(ssc_h_fir_coef_data2);
				PE_SSC_M14B0_RdFL(ssc_h_fir_coef_data3);
				data[offset+1] = PE_SSC_M14B0_Rd(ssc_h_fir_coef_data0);
				data[offset+2] = PE_SSC_M14B0_Rd(ssc_h_fir_coef_data1);
				data[offset+3] = PE_SSC_M14B0_Rd(ssc_h_fir_coef_data2);
				data[offset+4] = PE_SSC_M14B0_Rd(ssc_h_fir_coef_data3);
				offset += 5;
			}
			PE_SSC_M14B0_RdFL(ssc_scr_size);
			PE_SSC_M14B0_RdFL(ssc_win_in_size);
			PE_SSC_M14B0_Wr(ssc_win_in_size,	PE_SSC_M14B0_Rd(ssc_scr_size));
			PE_SSC_M14B0_WrFL(ssc_win_in_size);
		}
		else if(PE_KDRV_VER_M14AX)
		{
			PE_SSC_M14A0_RdFL(ssc_win_in_size);
			PE_SSC_M14A0_Wr01(ssc_win_in_size,	in_win_h_size,	0x0);
			PE_SSC_M14A0_Wr01(ssc_win_in_size,	in_win_v_size,	0x0);
			PE_SSC_M14A0_WrFL(ssc_win_in_size);

			PE_SSC_M14A0_RdFL(ssc_h_fir_coef_ctrl);
			PE_SSC_M14A0_Wr01(ssc_h_fir_coef_ctrl, h_v_sel,		config);
			PE_SSC_M14A0_WrFL(ssc_h_fir_coef_ctrl);
			for(i=0;i<16;i++)
			{
				/* set addr */
				data[offset] = i;	//coeff num 0~15
				PE_SSC_M14A0_Wr01(ssc_h_fir_coef_ctrl, addr,		i);
				PE_SSC_M14A0_WrFL(ssc_h_fir_coef_ctrl);
				/* set update */
				PE_SSC_M14A0_Wr01(ssc_h_fir_coef_ctrl, update,		0x1);	//update and auto clear
				PE_SSC_M14A0_WrFL(ssc_h_fir_coef_ctrl);
				/* read coeff */
				PE_SSC_M14A0_RdFL(ssc_h_fir_coef_data0);
				PE_SSC_M14A0_RdFL(ssc_h_fir_coef_data1);
				PE_SSC_M14A0_RdFL(ssc_h_fir_coef_data2);
				PE_SSC_M14A0_RdFL(ssc_h_fir_coef_data3);
				data[offset+1] = PE_SSC_M14A0_Rd(ssc_h_fir_coef_data0);
				data[offset+2] = PE_SSC_M14A0_Rd(ssc_h_fir_coef_data1);
				data[offset+3] = PE_SSC_M14A0_Rd(ssc_h_fir_coef_data2);
				data[offset+4] = PE_SSC_M14A0_Rd(ssc_h_fir_coef_data3);
				offset += 5;
			}
			PE_SSC_M14A0_RdFL(ssc_scr_size);
			PE_SSC_M14A0_RdFL(ssc_win_in_size);
			PE_SSC_M14A0_Wr(ssc_win_in_size,	PE_SSC_M14A0_Rd(ssc_scr_size));
			PE_SSC_M14A0_WrFL(ssc_win_in_size);
		}
	}while(0);
	return offset;
}
/**
 * read default setting
 *
 * @param   *pstParams [in/out] LX_PE_ETC_TBL_T
 * @return  OK if success, ERROR otherwise.
 * @see     
 * @author
 */
int PE_SHP_HW_M14_RdDefault(LX_PE_ETC_TBL_T *pstParams)
{
	int ret = RET_OK;
	char buffer[PE_TRACE_STR_SIZE];
	UINT32 param_size=0, table_size=0, is_valid=0;
	static UINT32 rd_cnt=0;
	PE_REG_PARAM_T param;
	PE_SHP_HW_M14_SETTINGS_T *pInfo=&_g_pe_shp_hw_m14_info;
	#ifdef PE_HW_M14B_USE_REG_CTRL
	UINT32 *p_data = NULL;
	PE_SHP_HW_M14B_PE1_REG_CTRL_T *pe1_reg_ctrl = &_g_pe_shp_hw_m14b_pe1_reg_ctrl;
	PE_SHP_HW_M14B_PE1_REG_MATCH_T *pe1_reg_match = &_g_pe_shp_hw_m14b_pe1_reg_match;
	PE_SHP_HW_M14B_MSC_REG_CTRL_T *msc_reg_ctrl = &_g_pe_shp_hw_m14b_msc_reg_ctrl;
	PE_SHP_HW_M14B_MSC_REG_MATCH_T *msc_reg_match = &_g_pe_shp_hw_m14b_msc_reg_match;
	#endif

#define PE_SHP_M14_NO_PRINT			0
#define PE_SHP_M14_RD_N_HEX_PRINT	1
#define PE_SHP_M14_HEX_PRINT_ONLY	2
#define PE_SHP_M14_DEC_PRINT_ONLY	3
#define PE_SHP_M14_PRINT_START		"START_OF_PRINT"
#define PE_SHP_M14_PRINT_END		"END_OF_PRINT"
#define PE_SHP_M14_PRINT_EXIT		"EXIT_OF_PRINT"
#define PE_SHP_M14_PRINT_RESRV		"RESERVED"
#define PE_SHP_M14_CASE_PRMENU(_cnt, _action, _buf, _item)	\
	case (_cnt):sprintf(_buf, "   [0x%02x] %s\n", (_item), #_item);_action;break
#define PE_SHP_M14_CASE_SPRINT(_cnt, _action, _buf, fmt, args...)	\
	case (_cnt):sprintf(_buf, fmt, ##args);_action;break
#define PE_SHP_M14_DFLT_SPRINT(_action, _buf, fmt, args...)		\
	default:sprintf(_buf, fmt, ##args);_action;break

	do{
		CHECK_KNULL(pstParams);
		CHECK_KNULL(pstParams->data);
		if (PE_KDRV_VER_M14BX)
		{
			if (pstParams->data[0]>=PE_SHP_HW_M14B_RD_DFLT_MAX)
			{
				sprintf(buffer, "%s", PE_SHP_M14_PRINT_EXIT);
				rd_cnt=0;
			}
			else if (pstParams->data[0]==PE_SHP_HW_M14B_RD_DFLT_DISPLAY)
			{
				switch (rd_cnt)
				{
					PE_SHP_M14_CASE_SPRINT( 0, rd_cnt++, buffer, "%s", PE_SHP_M14_PRINT_START);
					PE_SHP_M14_CASE_SPRINT( 1, rd_cnt++, buffer, "   ** current default **\n");
					PE_SHP_M14_CASE_SPRINT( 2, rd_cnt++, buffer, \
						"   res0b_fmt : %s\n", PE_SHP_HW_M14B_RES_FMT_TO_STR(pInfo->res0b_fmt));
					PE_SHP_M14_CASE_SPRINT( 3, rd_cnt++, buffer, \
						"   sre0b_fmt : %s\n", PE_SHP_HW_M14B_SRE_FMT_TO_STR(pInfo->sre0b_fmt));
					PE_SHP_M14_CASE_SPRINT( 4, rd_cnt++, buffer, \
						"   srl0b_fmt : %s\n", PE_SHP_HW_M14B_SRL_FMT_TO_STR(pInfo->srl0b_fmt));
					PE_SHP_M14_CASE_SPRINT( 5, rd_cnt++, buffer, \
						"   vfc0b_fmt : %s\n", PE_SHP_HW_M14B_VFC_FMT_TO_STR(pInfo->vfc0b_fmt));
					PE_SHP_M14_CASE_SPRINT( 6, rd_cnt++, buffer, \
						"   cti0b_fmt : %s\n", PE_SHP_HW_M14B_CTI_FMT_TO_STR(pInfo->cti0b_fmt));
					PE_SHP_M14_CASE_SPRINT( 7, rd_cnt++, buffer, "   *********************\n");
					PE_SHP_M14_CASE_SPRINT( 8, rd_cnt++, buffer, PE_SHP_M14_PRINT_RESRV);
					PE_SHP_M14_CASE_SPRINT( 9, rd_cnt++, buffer, PE_SHP_M14_PRINT_RESRV);
					PE_SHP_M14_CASE_PRMENU(10, rd_cnt++, buffer, PE_SHP_HW_M14B_RD_DFLT_SHP);
					PE_SHP_M14_CASE_PRMENU(11, rd_cnt++, buffer, PE_SHP_HW_M14B_RD_DFLT_SRE);
					PE_SHP_M14_CASE_PRMENU(12, rd_cnt++, buffer, PE_SHP_HW_M14B_RD_DFLT_VFC);
					PE_SHP_M14_CASE_PRMENU(13, rd_cnt++, buffer, PE_SHP_HW_M14B_RD_DFLT_CTI);
					PE_SHP_M14_CASE_SPRINT(14, rd_cnt++, buffer, PE_SHP_M14_PRINT_RESRV);
					PE_SHP_M14_CASE_SPRINT(15, rd_cnt++, buffer, PE_SHP_M14_PRINT_RESRV);
					PE_SHP_M14_CASE_SPRINT(16, rd_cnt++, buffer, PE_SHP_M14_PRINT_RESRV);
					PE_SHP_M14_CASE_SPRINT(17, rd_cnt++, buffer, PE_SHP_M14_PRINT_RESRV);
					PE_SHP_M14_CASE_SPRINT(18, rd_cnt++, buffer, PE_SHP_M14_PRINT_RESRV);
					PE_SHP_M14_CASE_SPRINT(19, rd_cnt++, buffer, PE_SHP_M14_PRINT_RESRV);
					#ifdef PE_HW_M14B_USE_REG_CTRL
					PE_SHP_M14_CASE_PRMENU(20, rd_cnt++, buffer, PE_SHP_HW_M14B_RD_RES_MNUM);
					PE_SHP_M14_CASE_PRMENU(21, rd_cnt++, buffer, PE_SHP_HW_M14B_RD_SRE_MNUM);
					PE_SHP_M14_CASE_PRMENU(22, rd_cnt++, buffer, PE_SHP_HW_M14B_RD_PE1_DATA);
					PE_SHP_M14_CASE_PRMENU(23, rd_cnt++, buffer, PE_SHP_HW_M14B_RD_PE1_MASK);
					PE_SHP_M14_CASE_PRMENU(24, rd_cnt++, buffer, PE_SHP_HW_M14B_RD_VFC_MNUM);
					PE_SHP_M14_CASE_PRMENU(25, rd_cnt++, buffer, PE_SHP_HW_M14B_RD_MSC_DATA);
					PE_SHP_M14_CASE_PRMENU(26, rd_cnt++, buffer, PE_SHP_HW_M14B_RD_MSC_MASK);
					#endif
					PE_SHP_M14_DFLT_SPRINT(rd_cnt=0, buffer, "%s", PE_SHP_M14_PRINT_END);
				}
			}
			else
			{
				is_valid = PE_SHP_M14_NO_PRINT;
				switch (pstParams->data[0])
				{
					case PE_SHP_HW_M14B_RD_DFLT_SHP:
						table_size = sizeof(shp_l_pc_default_m14b0);
						param_size = table_size/sizeof(PE_REG_PARAM_T);
						if (rd_cnt<param_size)
						{
							param.addr = shp_l_pc_default_m14b0[rd_cnt].addr;
							is_valid = PE_SHP_M14_RD_N_HEX_PRINT;
						}
						break;
					case PE_SHP_HW_M14B_RD_DFLT_SRE:
						table_size = sizeof(sre_l_pc_default_m14b0);
						param_size = table_size/sizeof(PE_REG_PARAM_T);
						if (rd_cnt<param_size)
						{
							param.addr = sre_l_pc_default_m14b0[rd_cnt].addr;
							is_valid = PE_SHP_M14_RD_N_HEX_PRINT;
						}
						break;
					case PE_SHP_HW_M14B_RD_DFLT_VFC:
						table_size = sizeof(vflt_c_l_pc_default_m14b0);
						param_size = table_size/sizeof(PE_REG_PARAM_T);
						if (rd_cnt<param_size)
						{
							param.addr = vflt_c_l_pc_default_m14b0[rd_cnt].addr;
							is_valid = PE_SHP_M14_RD_N_HEX_PRINT;
						}
						break;
					case PE_SHP_HW_M14B_RD_DFLT_CTI:
						table_size = sizeof(cti_p0d_l_pc_default_m14b0);
						param_size = table_size/sizeof(PE_REG_PARAM_T);
						if (rd_cnt<param_size)
						{
							param.addr = cti_p0d_l_pc_default_m14b0[rd_cnt].addr;
							is_valid = PE_SHP_M14_RD_N_HEX_PRINT;
						}
						break;
					#ifdef PE_HW_M14B_USE_REG_CTRL
					case PE_SHP_HW_M14B_RD_RES_MNUM:
						table_size = pe1_reg_match->res_size;
						param_size = table_size/sizeof(PE_REG_PARAM_T);
						if (rd_cnt<param_size)
						{
							param.addr = pe1_reg_match->res_mtbl[rd_cnt].addr;
							param.data = pe1_reg_match->res_mtbl[rd_cnt].data;
							is_valid = PE_SHP_M14_DEC_PRINT_ONLY;
						}
						break;
					case PE_SHP_HW_M14B_RD_SRE_MNUM:
						table_size = pe1_reg_match->sre_size;
						param_size = table_size/sizeof(PE_REG_PARAM_T);
						if (rd_cnt<param_size)
						{
							param.addr = pe1_reg_match->sre_mtbl[rd_cnt].addr;
							param.data = pe1_reg_match->sre_mtbl[rd_cnt].data;
							is_valid = PE_SHP_M14_DEC_PRINT_ONLY;
						}
						break;
					case PE_SHP_HW_M14B_RD_PE1_DATA:
						if (rd_cnt<PE_SHP_HW_M14B_PE1_ENUM(MAX))
						{
							param.addr = pe1_reg_ctrl->addr[rd_cnt];
							p_data = (UINT32 *)&(pe1_reg_ctrl->data);
							param.data = p_data[rd_cnt];
							is_valid = PE_SHP_M14_HEX_PRINT_ONLY;
						}
						break;
					case PE_SHP_HW_M14B_RD_PE1_MASK:
						if (rd_cnt<PE_SHP_HW_M14B_PE1_ENUM(MAX))
						{
							param.addr = pe1_reg_ctrl->addr[rd_cnt];
							p_data = (UINT32 *)&(pe1_reg_ctrl->mask);
							param.data = p_data[rd_cnt];
							is_valid = PE_SHP_M14_HEX_PRINT_ONLY;
						}
						break;
					case PE_SHP_HW_M14B_RD_VFC_MNUM:
						table_size = msc_reg_match->vfc_size;
						param_size = table_size/sizeof(PE_REG_PARAM_T);
						if (rd_cnt<param_size)
						{
							param.addr = msc_reg_match->vfc_mtbl[rd_cnt].addr;
							param.data = msc_reg_match->vfc_mtbl[rd_cnt].data;
							is_valid = PE_SHP_M14_DEC_PRINT_ONLY;
						}
						break;
					case PE_SHP_HW_M14B_RD_MSC_DATA:
						if (rd_cnt<PE_SHP_HW_M14B_MSC_ENUM(MAX))
						{
							param.addr = msc_reg_ctrl->addr[rd_cnt];
							p_data = (UINT32 *)&(msc_reg_ctrl->data);
							param.data = p_data[rd_cnt];
							is_valid = PE_SHP_M14_HEX_PRINT_ONLY;
						}
						break;
					case PE_SHP_HW_M14B_RD_MSC_MASK:
						if (rd_cnt<PE_SHP_HW_M14B_MSC_ENUM(MAX))
						{
							param.addr = msc_reg_ctrl->addr[rd_cnt];
							p_data = (UINT32 *)&(msc_reg_ctrl->mask);
							param.data = p_data[rd_cnt];
							is_valid = PE_SHP_M14_HEX_PRINT_ONLY;
						}
						break;
					#endif
					default:
						break;
				}
				if (is_valid == PE_SHP_M14_RD_N_HEX_PRINT)
				{
					param.data = PE_REG_M14_RD(param.addr);
					sprintf(buffer, "   [%04d] {0x%04X, 0x%08X},\n", \
						rd_cnt, param.addr, param.data);
					rd_cnt++;
				}
				else if (is_valid == PE_SHP_M14_HEX_PRINT_ONLY)
				{
					sprintf(buffer, "   [%04d] {0x%04X, 0x%08X},\n", \
						rd_cnt, param.addr, param.data);
					rd_cnt++;
				}
				else if (is_valid == PE_SHP_M14_DEC_PRINT_ONLY)
				{
					sprintf(buffer, "   [%04d] {0x%04X, %04d},\n", \
						rd_cnt, param.addr, param.data);
					rd_cnt++;
				}
				else
				{
					sprintf(buffer, "%s", PE_SHP_M14_PRINT_END);
					rd_cnt = 0;
				}
			}
			memcpy(pstParams->data, buffer, sizeof(char)*PE_TRACE_STR_SIZE);
		}
		else if (PE_KDRV_VER_M14AX)
		{
			if (pstParams->data[0]>=PE_SHP_HW_M14A_RD_DFLT_MAX)
			{
				sprintf(buffer, "%s", PE_SHP_M14_PRINT_EXIT);
				rd_cnt=0;
			}
			else if (pstParams->data[0]==PE_SHP_HW_M14A_RD_DFLT_DISPLAY)
			{
				switch (rd_cnt)
				{
					PE_SHP_M14_CASE_SPRINT( 0, rd_cnt++, buffer, "%s", PE_SHP_M14_PRINT_START);
					PE_SHP_M14_CASE_SPRINT( 1, rd_cnt++, buffer, "   ** current default **\n");
					PE_SHP_M14_CASE_SPRINT( 2, rd_cnt++, buffer, \
						"   res0a_fmt : %s\n", PE_SHP_HW_M14A_RES_FMT_TO_STR(pInfo->res0a_fmt));
					PE_SHP_M14_CASE_SPRINT( 3, rd_cnt++, buffer, \
						"   res1a_fmt : %s\n", PE_SHP_HW_M14A_RES_FMT_TO_STR(pInfo->res1a_fmt));
					PE_SHP_M14_CASE_SPRINT( 4, rd_cnt++, buffer, \
						"   vfc0a_fmt : %s\n", PE_SHP_HW_M14A_VFC_FMT_TO_STR(pInfo->vfc0a_fmt));
					PE_SHP_M14_CASE_SPRINT( 5, rd_cnt++, buffer, \
						"   vfc1a_fmt : %s\n", PE_SHP_HW_M14A_VFC_FMT_TO_STR(pInfo->vfc1a_fmt));
					PE_SHP_M14_CASE_SPRINT( 6, rd_cnt++, buffer, "   *********************\n");
					PE_SHP_M14_CASE_SPRINT( 7, rd_cnt++, buffer, PE_SHP_M14_PRINT_RESRV);
					PE_SHP_M14_CASE_SPRINT( 8, rd_cnt++, buffer, PE_SHP_M14_PRINT_RESRV);
					PE_SHP_M14_CASE_SPRINT( 9, rd_cnt++, buffer, PE_SHP_M14_PRINT_RESRV);
					PE_SHP_M14_CASE_PRMENU(10, rd_cnt++, buffer, PE_SHP_HW_M14A_RD_DFLT_SHP_L);
					PE_SHP_M14_CASE_PRMENU(11, rd_cnt++, buffer, PE_SHP_HW_M14A_RD_DFLT_SHP_R);
					PE_SHP_M14_CASE_PRMENU(12, rd_cnt++, buffer, PE_SHP_HW_M14A_RD_DFLT_VFC_L);
					PE_SHP_M14_CASE_PRMENU(13, rd_cnt++, buffer, PE_SHP_HW_M14A_RD_DFLT_VFC_R);
					PE_SHP_M14_DFLT_SPRINT(rd_cnt=0, buffer, "%s", PE_SHP_M14_PRINT_END);
				}
			}
			else
			{
				is_valid = PE_SHP_M14_NO_PRINT;
				switch (pstParams->data[0])
				{
					case PE_SHP_HW_M14A_RD_DFLT_SHP_L:
						table_size = sizeof(shp_l_pc_default_m14a0);
						param_size = table_size/sizeof(PE_REG_PARAM_T);
						if (rd_cnt<param_size)
						{
							param.addr = shp_l_pc_default_m14a0[rd_cnt].addr;
							is_valid = PE_SHP_M14_RD_N_HEX_PRINT;
						}
						break;
					case PE_SHP_HW_M14A_RD_DFLT_SHP_R:
						table_size = sizeof(shp_r_pc_default_m14a0);
						param_size = table_size/sizeof(PE_REG_PARAM_T);
						if (rd_cnt<param_size)
						{
							param.addr = shp_r_pc_default_m14a0[rd_cnt].addr;
							is_valid = PE_SHP_M14_RD_N_HEX_PRINT;
						}
						break;
					case PE_SHP_HW_M14A_RD_DFLT_VFC_L:
					case PE_SHP_HW_M14A_RD_DFLT_VFC_R:
					default:
						table_size = sizeof(vflt_c_l_pc_default_m14a0);
						param_size = table_size/sizeof(PE_REG_PARAM_T);
						if (rd_cnt<param_size)
						{
							param.addr = vflt_c_l_pc_default_m14a0[rd_cnt].addr;
							if(pstParams->data[0]==PE_SHP_HW_M14A_RD_DFLT_VFC_R)
								param.addr += PE_MSC_REG_M14_LRDIFF;
							is_valid = PE_SHP_M14_RD_N_HEX_PRINT;
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
					sprintf(buffer, "%s", PE_SHP_M14_PRINT_END);
					rd_cnt = 0;
				}
			}
			memcpy(pstParams->data, buffer, sizeof(char)*PE_TRACE_STR_SIZE);
		}
	}while (0);
	return ret;
}

/**
 * set resolution common control
 * - use input struct LX_PE_SHP_RE2_CMN_T for M14AX
 * - use input struct LX_PE_SHP_RE1_CMN_T for M14AX
 *
 * @param   *pstParams [in] void
 * @return  OK if success, ERROR otherwise.
 * @see		function description
 * @author
 */
int PE_SHP_HW_M14_SetReCmnCtrl(void *pstParams)
{
	int ret = RET_OK;
	do {
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_M14)
		{
			LX_PE_SHP_RE2_CMN_T *pp=(LX_PE_SHP_RE2_CMN_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			PE_SHP_HW_M14_DBG_PRINT("set[%d] : resolution common ctrl\n"\
				" mp w,b_gain:%d,%d,  sp w,b_gain:%d,%d\n"\
				" cti   : size:%d, g:%d,%d,%d\n",\
				pp->win_id,pp->mp_white_gain,pp->mp_black_gain,\
				pp->sp_white_gain,pp->sp_black_gain,\
				pp->tap_size,pp->cti_gain,pp->ycm_y_gain,pp->ycm_c_gain);
			if(PE_CHECK_WIN0(pp->win_id))
			{
				if(PE_KDRV_VER_M14BX)
				{
					#ifdef PE_HW_M14B_USE_REG_CTRL
					PE_SHP_HW_M14B_PE1_WR02(pe1_mp_ctrl_01,	white_gain,	GET_BITS(pp->mp_white_gain,0,7),\
															black_gain,	GET_BITS(pp->mp_black_gain,0,7));
					PE_SHP_HW_M14B_PE1_WR02(pe1_sp_ctrl_01,	white_gain,	GET_BITS(pp->sp_white_gain,0,7),\
															black_gain,	GET_BITS(pp->sp_black_gain,0,7));
					/* cti */
					PE_SHP_HW_M14B_PE1_WR02(pe1_cti_ctrl_00,filter_tap_size,GET_BITS(pp->tap_size,0,3),\
															cti_gain,	GET_BITS(pp->cti_gain,0,8));
					PE_SHP_HW_M14B_PE1_WR02(pe1_cti_ctrl_02,ycm_y_gain,	GET_BITS(pp->ycm_y_gain,0,4),\
															ycm_c_gain,	GET_BITS(pp->ycm_c_gain,0,4));
					#else
					PE_PE1_M14B0_QWr02(pe1_mp_ctrl_01,	white_gain,		GET_BITS(pp->mp_white_gain,0,7),\
														black_gain,		GET_BITS(pp->mp_black_gain,0,7));
					PE_PE1_M14B0_QWr02(pe1_sp_ctrl_01,	white_gain,		GET_BITS(pp->sp_white_gain,0,7),\
														black_gain,		GET_BITS(pp->sp_black_gain,0,7));
					/* cti */
					PE_PE1_M14B0_QWr02(pe1_cti_ctrl_00, filter_tap_size,GET_BITS(pp->tap_size,0,3),\
														cti_gain,		GET_BITS(pp->cti_gain,0,8));
					PE_PE1_M14B0_QWr02(pe1_cti_ctrl_02, ycm_y_gain,		GET_BITS(pp->ycm_y_gain,0,4),\
														ycm_c_gain,		GET_BITS(pp->ycm_c_gain,0,4));
					#endif
				}
				else if(PE_KDRV_VER_M14AX)
				{
					PE_P1L_M14A0_QWr02(pe1_mp_ctrl_01,	white_gain,		GET_BITS(pp->mp_white_gain,0,7),\
														black_gain,		GET_BITS(pp->mp_black_gain,0,7));
					PE_P1L_M14A0_QWr02(pe1_sp_ctrl_01,	white_gain,		GET_BITS(pp->sp_white_gain,0,7),\
														black_gain,		GET_BITS(pp->sp_black_gain,0,7));
					/* cti */
					PE_P1L_M14A0_QWr02(pe1_cti_ctrl_00, filter_tap_size,GET_BITS(pp->tap_size,0,3),\
														cti_gain,		GET_BITS(pp->cti_gain,0,8));
					PE_P1L_M14A0_QWr02(pe1_cti_ctrl_02, ycm_y_gain,		GET_BITS(pp->ycm_y_gain,0,4),\
														ycm_c_gain,		GET_BITS(pp->ycm_c_gain,0,4));
				}
			}
		}
		else
		{
			PE_SHP_HW_M14_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	} while (0);
	return ret;
}
/**
 * get resolution common control
 * - use input struct LX_PE_SHP_RE2_CMN_T for M14AX
 * - use input struct LX_PE_SHP_RE1_CMN_T for M14AX
 *
 * @param   *pstParams [in/out] void
 * @return  OK if success, ERROR otherwise.
 * @see		function description
 * @author
 */
int PE_SHP_HW_M14_GetReCmnCtrl(void *pstParams)
{
	int ret = RET_OK;
	LX_PE_WIN_ID win_id;
	do {
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_M14)
		{
			LX_PE_SHP_RE2_CMN_T *pp=(LX_PE_SHP_RE2_CMN_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			win_id = PE_GET_CHECKED_WINID(pp->win_id);
			if(PE_CHECK_WIN0(win_id))
			{
				if(PE_KDRV_VER_M14BX)
				{
					PE_PE1_M14B0_QRd02(pe1_mp_ctrl_01,	white_gain,		pp->mp_white_gain,\
														black_gain,		pp->mp_black_gain);
					PE_PE1_M14B0_QRd02(pe1_sp_ctrl_01,	white_gain,		pp->sp_white_gain,\
														black_gain,		pp->sp_black_gain);
					/* cti */
					PE_PE1_M14B0_QRd02(pe1_cti_ctrl_00, filter_tap_size,pp->tap_size,\
														cti_gain,		pp->cti_gain);
					PE_PE1_M14B0_QRd02(pe1_cti_ctrl_02, ycm_y_gain,		pp->ycm_y_gain,\
														ycm_c_gain,		pp->ycm_c_gain);
				}
				else if(PE_KDRV_VER_M14AX)
				{
					PE_P1L_M14A0_QRd02(pe1_mp_ctrl_01,	white_gain,		pp->mp_white_gain,\
														black_gain,		pp->mp_black_gain);
					PE_P1L_M14A0_QRd02(pe1_sp_ctrl_01,	white_gain,		pp->sp_white_gain,\
														black_gain,		pp->sp_black_gain);
					/* cti */
					PE_P1L_M14A0_QRd02(pe1_cti_ctrl_00, filter_tap_size,pp->tap_size,\
														cti_gain,		pp->cti_gain);
					PE_P1L_M14A0_QRd02(pe1_cti_ctrl_02, ycm_y_gain,		pp->ycm_y_gain,\
														ycm_c_gain,		pp->ycm_c_gain);
				}
			}
			PE_SHP_HW_M14_DBG_PRINT("get[%d] : resolution common ctrl\n"\
				" mp w,b_gain:%d,%d,  sp w,b_gain:%d,%d\n"\
				" cti   :size:%d, g:%d,%d,%d\n",\
				pp->win_id,pp->mp_white_gain,pp->mp_black_gain,\
				pp->sp_white_gain,pp->sp_black_gain,\
				pp->tap_size,pp->cti_gain,pp->ycm_y_gain,pp->ycm_c_gain);
		}
		else
		{
			PE_SHP_HW_M14_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * set resolution horizontal control
 * - use input struct LX_PE_SHP_RE2_HOR_T for M14AX
 * - use input struct LX_PE_SHP_RE1_HOR_T for M14AX
 *
 * @param   *pstParams [in/out] void
 * @return  OK if success, ERROR otherwise.
 * @see		function description
 * @author
 */
int PE_SHP_HW_M14_SetReHorCtrl(void *pstParams)
{
	int ret = RET_OK;
	do {
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_M14)
		{
			LX_PE_SHP_RE2_HOR_T *pp=(LX_PE_SHP_RE2_HOR_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			PE_SHP_HW_M14_DBG_PRINT("set[%d] : resolution horizontal ctrl\n"\
				"g:%d,%d,%d, a_gen_w:%d, mp,sp_gain:%d,%d\n"\
				"e_gain(1,2):%d,%d, f_gain(1,2):%d,%d\n"
				"coring_th:%d, y_gain:%d, c_gain:%d\n",\
				pp->win_id,pp->reg_csft_gain,\
				pp->edge_filter_white_gain,pp->edge_filter_black_gain,\
				pp->a_gen_width,pp->mp_horizontal_gain,pp->sp_horizontal_gain,\
				pp->e_gain_th1, pp->e_gain_th2,pp->f_gain_th1, pp->f_gain_th2,\
				pp->coring_th, pp->y_gain, pp->c_gain);
			if(PE_CHECK_WIN0(pp->win_id))
			{
				PE_FWI_M14_HOR_CTRL fw_param;
				fw_param.e_gain_th1 = GET_BITS(pp->e_gain_th1,0,8);
				fw_param.e_gain_th2 = GET_BITS(pp->e_gain_th2,0,8);
				ret = PE_FWI_M14_SetShpHorCtrl(&fw_param);
				PE_SHP_HW_M14_CHECK_CODE(ret, break, \
					"[%s,%d] PE_FWI_M14_SetShpHorCtrl() error.\n", __F__, __L__);
				if(PE_KDRV_VER_M14BX)
				{
					#ifdef PE_HW_M14B_USE_REG_CTRL
					PE_SHP_HW_M14B_PE1_WR01(pe1_derh_ctrl_00,a_gen_width,	GET_BITS(pp->a_gen_width,0,4));
					PE_SHP_HW_M14B_PE1_WR01(pe1_derh_ctrl_05,reg_csft_gain,	GET_BITS(pp->reg_csft_gain,0,6));
					PE_SHP_HW_M14B_PE1_WR02(pe1_derh_ctrl_06,edge_filter_white_gain,GET_BITS(pp->edge_filter_white_gain,0,6),\
															edge_filter_black_gain,	GET_BITS(pp->edge_filter_black_gain,0,6));
					PE_SHP_HW_M14B_PE1_WR01(pe1_mp_ctrl_01,horizontal_gain,GET_BITS(pp->mp_horizontal_gain,0,8));
					PE_SHP_HW_M14B_PE1_WR01(pe1_sp_ctrl_01,horizontal_gain,GET_BITS(pp->sp_horizontal_gain,0,8));
					PE_SHP_HW_M14B_PE1_WR02(pe1_derh_ctrl_0b,f_gain_th1,GET_BITS(pp->f_gain_th1,0,8),\
															f_gain_th2,	GET_BITS(pp->f_gain_th2,0,8));
					PE_SHP_HW_M14B_PE1_WR03(pe1_ti_ctrl_0,coring_th,	GET_BITS(pp->coring_th,0,8),\
													  	y_gain,			GET_BITS(pp->y_gain,0,8),\
													  	c_gain,			GET_BITS(pp->c_gain,0,8));
					#ifdef PE_SHP_HW_M14B_SET_FW_PARAM_TO_REG_DIRECTLY
					PE_SHP_HW_M14B_PE1_WR02(pe1_derh_ctrl_0b, e_gain_th1,GET_BITS(pp->e_gain_th1,0,8), \
															e_gain_th2,GET_BITS(pp->e_gain_th2,0,8));
					#else
					PE_SHP_HW_M14B_PE1_SET(pe1_derh_ctrl_0b,e_gain_th1,GET_BITS(pp->e_gain_th1,0,8));
					PE_SHP_HW_M14B_PE1_SET(pe1_derh_ctrl_0b,e_gain_th2,GET_BITS(pp->e_gain_th2,0,8));
					#endif
					#else
					PE_PE1_M14B0_QWr01(pe1_derh_ctrl_00, a_gen_width,	GET_BITS(pp->a_gen_width,0,4));
					PE_PE1_M14B0_QWr01(pe1_derh_ctrl_05, reg_csft_gain,	GET_BITS(pp->reg_csft_gain,0,6));
					PE_PE1_M14B0_QWr02(pe1_derh_ctrl_06, edge_filter_white_gain,GET_BITS(pp->edge_filter_white_gain,0,6),\
														 edge_filter_black_gain,GET_BITS(pp->edge_filter_black_gain,0,6));
					PE_PE1_M14B0_QWr01(pe1_mp_ctrl_01, 	horizontal_gain,GET_BITS(pp->mp_horizontal_gain,0,8));
					PE_PE1_M14B0_QWr01(pe1_sp_ctrl_01, 	horizontal_gain,GET_BITS(pp->sp_horizontal_gain,0,8));
					PE_PE1_M14B0_QWr02(pe1_derh_ctrl_0b, f_gain_th1,	GET_BITS(pp->f_gain_th1,0,8),\
														 f_gain_th2,	GET_BITS(pp->f_gain_th2,0,8));
					PE_PE1_M14B0_QWr03(pe1_ti_ctrl_0, 	coring_th,		GET_BITS(pp->coring_th,0,8),\
													  	y_gain,			GET_BITS(pp->y_gain,0,8),\
													  	c_gain,			GET_BITS(pp->c_gain,0,8));
					#ifdef PE_SHP_HW_M14B_SET_FW_PARAM_TO_REG_DIRECTLY
					PE_PE1_M14B0_QWr02(pe1_derh_ctrl_0b, e_gain_th1,GET_BITS(pp->e_gain_th1,0,8), \
														e_gain_th2,GET_BITS(pp->e_gain_th2,0,8));
					#endif
					#endif
				}
				else if(PE_KDRV_VER_M14AX)
				{
					PE_P1L_M14A0_QWr01(pe1_derh_ctrl_00, a_gen_width,	GET_BITS(pp->a_gen_width,0,4));
					PE_P1L_M14A0_QWr01(pe1_derh_ctrl_05, reg_csft_gain,	GET_BITS(pp->reg_csft_gain,0,6));
					PE_P1L_M14A0_QWr02(pe1_derh_ctrl_06, edge_filter_white_gain,GET_BITS(pp->edge_filter_white_gain,0,6),\
														 edge_filter_black_gain,GET_BITS(pp->edge_filter_black_gain,0,6));
					PE_P1L_M14A0_QWr01(pe1_mp_ctrl_01, 	horizontal_gain,GET_BITS(pp->mp_horizontal_gain,0,8));
					PE_P1L_M14A0_QWr01(pe1_sp_ctrl_01, 	horizontal_gain,GET_BITS(pp->sp_horizontal_gain,0,8));
					PE_P1L_M14A0_QWr02(pe1_derh_ctrl_0b, f_gain_th1,	GET_BITS(pp->f_gain_th1,0,8),\
														 f_gain_th2,	GET_BITS(pp->f_gain_th2,0,8));
					PE_P1L_M14A0_QWr03(pe1_ti_ctrl_0, 	coring_th,		GET_BITS(pp->coring_th,0,8),\
													  	y_gain,			GET_BITS(pp->y_gain,0,8),\
													  	c_gain,			GET_BITS(pp->c_gain,0,8));
				}
			}
		}
		else
		{
			PE_SHP_HW_M14_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	} while (0);
	return ret;
}
/**
 * get resolution horizontal control
 * - use input struct LX_PE_SHP_RE2_HOR_T for M14AX
 * - use input struct LX_PE_SHP_RE1_HOR_T for M14AX
 *
 * @param   *pstParams [in/out] void
 * @return  OK if success, ERROR otherwise.
 * @see		function description
 * @author
 */
int PE_SHP_HW_M14_GetReHorCtrl(void *pstParams)
{
	int ret = RET_OK;
	LX_PE_WIN_ID win_id;
	do {
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_M14)
		{
			LX_PE_SHP_RE2_HOR_T *pp=(LX_PE_SHP_RE2_HOR_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			win_id = PE_GET_CHECKED_WINID(pp->win_id);
			if(PE_CHECK_WIN0(win_id))
			{
				PE_FWI_M14_SETTINGS_T fw_inf;
				ret = PE_FWI_M14_GetCurFwiSettings(&fw_inf);
				PE_SHP_HW_M14_CHECK_CODE(ret, break, \
					"[%s,%d] PE_FWI_M14_GetCurFwiSettings() error.\n", __F__, __L__);
				PE_SHP_HW_M14_DBG_PRINT("fw_ctrl.enable : %s.\n", fw_inf.fw_ctrl.enable? "yes":"no");
				if(PE_KDRV_VER_M14BX)
				{
					#ifdef PE_SHP_HW_M14B_SET_FW_PARAM_TO_REG_DIRECTLY
					PE_PE1_M14B0_QRd02(pe1_derh_ctrl_0b, e_gain_th1,	pp->e_gain_th1,\
														 e_gain_th2,	pp->e_gain_th2);
					#else
					if(fw_inf.fw_ctrl.enable)
					{
						pp->e_gain_th1 = fw_inf.shp_h_ctrl.e_gain_th1;
						pp->e_gain_th2 = fw_inf.shp_h_ctrl.e_gain_th2;
					}
					else
					{
						PE_PE1_M14B0_QRd02(pe1_derh_ctrl_0b, e_gain_th1,	pp->e_gain_th1,\
															 e_gain_th2,	pp->e_gain_th2);
					}
					#endif
					PE_PE1_M14B0_QRd01(pe1_derh_ctrl_00, a_gen_width,		pp->a_gen_width);
					PE_PE1_M14B0_QRd01(pe1_derh_ctrl_05, reg_csft_gain,		pp->reg_csft_gain);
					PE_PE1_M14B0_QRd02(pe1_derh_ctrl_06, edge_filter_white_gain,pp->edge_filter_white_gain,\
														 edge_filter_black_gain,pp->edge_filter_black_gain);
					PE_PE1_M14B0_QRd01(pe1_mp_ctrl_01, 	 horizontal_gain,	pp->mp_horizontal_gain);
					PE_PE1_M14B0_QRd01(pe1_sp_ctrl_01, 	 horizontal_gain,	pp->sp_horizontal_gain);
					PE_PE1_M14B0_QRd02(pe1_derh_ctrl_0b, f_gain_th1,		pp->f_gain_th1,\
														 f_gain_th2,		pp->f_gain_th2);
					PE_PE1_M14B0_QWr03(pe1_ti_ctrl_0, 	 coring_th,			pp->coring_th,\
													  	 y_gain,			pp->y_gain,\
													  	 c_gain,			pp->c_gain);
				}
				else if(PE_KDRV_VER_M14AX)
				{
					if(fw_inf.fw_ctrl.enable)
					{
						pp->e_gain_th1 = fw_inf.shp_h_ctrl.e_gain_th1;
						pp->e_gain_th2 = fw_inf.shp_h_ctrl.e_gain_th2;
					}
					else
					{
						PE_P1L_M14A0_QRd02(pe1_derh_ctrl_0b, e_gain_th1,	pp->e_gain_th1,\
															 e_gain_th2,	pp->e_gain_th2);
					}
					PE_P1L_M14A0_QRd01(pe1_derh_ctrl_00, a_gen_width,		pp->a_gen_width);
					PE_P1L_M14A0_QRd01(pe1_derh_ctrl_05, reg_csft_gain,		pp->reg_csft_gain);
					PE_P1L_M14A0_QRd02(pe1_derh_ctrl_06, edge_filter_white_gain,pp->edge_filter_white_gain,\
														 edge_filter_black_gain,pp->edge_filter_black_gain);
					PE_P1L_M14A0_QRd01(pe1_mp_ctrl_01, 	 horizontal_gain,	pp->mp_horizontal_gain);
					PE_P1L_M14A0_QRd01(pe1_sp_ctrl_01, 	 horizontal_gain,	pp->sp_horizontal_gain);
					PE_P1L_M14A0_QRd02(pe1_derh_ctrl_0b, f_gain_th1,		pp->f_gain_th1,\
														 f_gain_th2,		pp->f_gain_th2);
					PE_P1L_M14A0_QWr03(pe1_ti_ctrl_0, 	 coring_th,			pp->coring_th,\
													  	 y_gain,			pp->y_gain,\
													  	 c_gain,			pp->c_gain);
				}
			}
			PE_SHP_HW_M14_DBG_PRINT("get[%d] : resolution horizontal ctrl\n"\
				" g:%d,%d,%d, a_gen_w:%d, mp,sp_gain:%d,%d\n"\
				"e_gain(1,2):%d,%d, f_gain(1,2):%d,%d\n"
				"coring_th:%d, y_gain:%d, c_gain:%d\n",\
				pp->win_id,pp->reg_csft_gain,\
				pp->edge_filter_white_gain,pp->edge_filter_black_gain,\
				pp->a_gen_width,pp->mp_horizontal_gain,pp->sp_horizontal_gain,\
				pp->e_gain_th1, pp->e_gain_th2,pp->f_gain_th1, pp->f_gain_th2,\
				pp->coring_th, pp->y_gain, pp->c_gain);
		}
		else
		{
			PE_SHP_HW_M14_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * set resolution vertical control
 * - use input struct LX_PE_SHP_RE2_VER_T for M14AX
 * - use input struct LX_PE_SHP_RE1_VER_T for M14AX
 *
 * @param   *pstParams [in] void
 * @return  OK if success, ERROR otherwise.
 * @see		function description
 * @author
 */
int PE_SHP_HW_M14_SetReVerCtrl(void *pstParams)
{
	int ret = RET_OK;
	do {
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_M14)
		{
			LX_PE_SHP_RE2_VER_T *pp=(LX_PE_SHP_RE2_VER_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			PE_SHP_HW_M14_DBG_PRINT("set[%d] : resolution vertical ctrl\n"\
				" manual_th:%d,csft_gain:%d, gain(b,w)%d,%d,\n"\
				"mmd_sel:%d, gain(th1,th2):%d,%d mp,sp_g:%d,%d\n",\
				pp->win_id,pp->bif_manual_th,pp->csft_gain,\
				pp->gain_b,pp->gain_w,pp->mmd_sel,pp->gain_th1,pp->gain_th2,\
				pp->mp_vertical_gain,pp->sp_vertical_gain);
			if(PE_CHECK_WIN0(pp->win_id))
			{
				if(PE_KDRV_VER_M14BX)
				{
					#ifdef PE_HW_M14B_USE_REG_CTRL
					PE_SHP_HW_M14B_PE1_WR01(pe1_derv_ctrl_0,bif_manual_th,	GET_BITS(pp->bif_manual_th,0,8));
					PE_SHP_HW_M14B_PE1_WR01(pe1_derv_ctrl_1,csft_gain,		GET_BITS(pp->csft_gain,0,6));
					PE_SHP_HW_M14B_PE1_WR02(pe1_derv_ctrl_3,gain_b,			GET_BITS(pp->gain_b,0,6),\
															gain_w,			GET_BITS(pp->gain_w,0,6));
					PE_SHP_HW_M14B_PE1_WR03(pe1_derv_ctrl_2,mmd_sel,		GET_BITS(pp->mmd_sel,0,3),\
															gain_th1,		GET_BITS(pp->gain_th1,0,8),\
															gain_th2,		GET_BITS(pp->gain_th2,0,8));
					PE_SHP_HW_M14B_PE1_WR01(pe1_mp_ctrl_01,	vertical_gain,	GET_BITS(pp->mp_vertical_gain,0,8));
					PE_SHP_HW_M14B_PE1_WR01(pe1_sp_ctrl_01,	vertical_gain,	GET_BITS(pp->sp_vertical_gain,0,8));
					#else
					PE_PE1_M14B0_QWr01(pe1_derv_ctrl_0, bif_manual_th,	GET_BITS(pp->bif_manual_th,0,8));
					PE_PE1_M14B0_QWr01(pe1_derv_ctrl_1, csft_gain,		GET_BITS(pp->csft_gain,0,6));
					PE_PE1_M14B0_QWr02(pe1_derv_ctrl_3, gain_b,			GET_BITS(pp->gain_b,0,6),\
														gain_w,			GET_BITS(pp->gain_w,0,6));
					PE_PE1_M14B0_QWr03(pe1_derv_ctrl_2, mmd_sel,		GET_BITS(pp->mmd_sel,0,3),\
														gain_th1,		GET_BITS(pp->gain_th1,0,8),\
														gain_th2,		GET_BITS(pp->gain_th2,0,8));
					PE_PE1_M14B0_QWr01(pe1_mp_ctrl_01, 	vertical_gain,	GET_BITS(pp->mp_vertical_gain,0,8));
					PE_PE1_M14B0_QWr01(pe1_sp_ctrl_01, 	vertical_gain,	GET_BITS(pp->sp_vertical_gain,0,8));
					#endif
				}
				else if(PE_KDRV_VER_M14AX)
				{
					PE_P1L_M14A0_QWr01(pe1_derv_ctrl_0, bif_manual_th,	GET_BITS(pp->bif_manual_th,0,8));
					PE_P1L_M14A0_QWr01(pe1_derv_ctrl_1, csft_gain,		GET_BITS(pp->csft_gain,0,6));
					PE_P1L_M14A0_QWr02(pe1_derv_ctrl_3, gain_b,			GET_BITS(pp->gain_b,0,6),\
														gain_w,			GET_BITS(pp->gain_w,0,6));
					PE_P1L_M14A0_QWr03(pe1_derv_ctrl_2, mmd_sel,		GET_BITS(pp->mmd_sel,0,3),\
														gain_th1,		GET_BITS(pp->gain_th1,0,8),\
														gain_th2,		GET_BITS(pp->gain_th2,0,8));
					PE_P1L_M14A0_QWr01(pe1_mp_ctrl_01, 	vertical_gain,	GET_BITS(pp->mp_vertical_gain,0,8));
					PE_P1L_M14A0_QWr01(pe1_sp_ctrl_01, 	vertical_gain,	GET_BITS(pp->sp_vertical_gain,0,8));
				}
			}
		}
		else
		{
			PE_SHP_HW_M14_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	} while (0);
	return ret;
}
/**
 * get resolution vertical control
 * - use input struct LX_PE_SHP_RE2_VER_T for M14AX
 * - use input struct LX_PE_SHP_RE1_VER_T for M14AX
 *
 * @param   *pstParams [in/out] void
 * @return  OK if success, ERROR otherwise.
 * @see		function description
 * @author
 */
int PE_SHP_HW_M14_GetReVerCtrl(void *pstParams)
{
	int ret = RET_OK;
	LX_PE_WIN_ID win_id;
	do {
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_M14)
		{
			LX_PE_SHP_RE2_VER_T *pp=(LX_PE_SHP_RE2_VER_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			win_id = PE_GET_CHECKED_WINID(pp->win_id);
			if(PE_CHECK_WIN0(pp->win_id))
			{
				if(PE_KDRV_VER_M14BX)
				{
					PE_PE1_M14B0_QRd01(pe1_derv_ctrl_0, bif_manual_th,	pp->bif_manual_th);
					PE_PE1_M14B0_QRd01(pe1_derv_ctrl_1, csft_gain,		pp->csft_gain);
					PE_PE1_M14B0_QRd02(pe1_derv_ctrl_3, gain_b,			pp->gain_b,\
														gain_w,			pp->gain_w);
					PE_PE1_M14B0_QRd03(pe1_derv_ctrl_2, mmd_sel,		pp->mmd_sel,\
														gain_th1,		pp->gain_th1,\
														gain_th2,		pp->gain_th2);
					PE_PE1_M14B0_QRd01(pe1_mp_ctrl_01, 	vertical_gain,	pp->mp_vertical_gain);
					PE_PE1_M14B0_QRd01(pe1_sp_ctrl_01, 	vertical_gain,	pp->sp_vertical_gain);
				}
				else if(PE_KDRV_VER_M14AX)
				{
					PE_P1L_M14A0_QRd01(pe1_derv_ctrl_0, bif_manual_th,	pp->bif_manual_th);
					PE_P1L_M14A0_QRd01(pe1_derv_ctrl_1, csft_gain,		pp->csft_gain);
					PE_P1L_M14A0_QRd02(pe1_derv_ctrl_3, gain_b,			pp->gain_b,\
														gain_w,			pp->gain_w);
					PE_P1L_M14A0_QRd03(pe1_derv_ctrl_2, mmd_sel,		pp->mmd_sel,\
														gain_th1,		pp->gain_th1,\
														gain_th2,		pp->gain_th2);
					PE_P1L_M14A0_QRd01(pe1_mp_ctrl_01, 	vertical_gain,	pp->mp_vertical_gain);
					PE_P1L_M14A0_QRd01(pe1_sp_ctrl_01, 	vertical_gain,	pp->sp_vertical_gain);
				}
			}
			PE_SHP_HW_M14_DBG_PRINT("get[%d] : resolution vertical ctrl\n"\
				" manual_th:%d,csft_gain:%d, gain(b,w)%d,%d,\n"\
				"mmd_sel:%d, gain(th1,th2):%d,%d mp,sp_g:%d,%d\n",\
				pp->win_id,pp->bif_manual_th,pp->csft_gain,\
				pp->gain_b,pp->gain_w,pp->mmd_sel,pp->gain_th1,pp->gain_th2,\
				pp->mp_vertical_gain,pp->sp_vertical_gain);
		}
		else
		{
			PE_SHP_HW_M14_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * set resolution misc control
 * - use input struct LX_PE_SHP_RE2_MISC_T for M14AX
 * - use input struct LX_PE_SHP_RE1_MISC_T for M14AX
 *
 * @param   *pstParams [in] void
 * @return  OK if success, ERROR otherwise.
 * @see		function description
 * @author
 */
int PE_SHP_HW_M14_SetReMiscCtrl(void *pstParams)
{
	int ret = RET_OK;
	do {
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_M14BX)
		{
			LX_PE_SHP_RE3_MISC_T *pp=(LX_PE_SHP_RE3_MISC_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			PE_SHP_HW_M14_DBG_PRINT("set[%d] :  resolution misc ctrl\n"\
				" hor : 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" ver : 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" cmn : 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" djg : 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" djg : 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" emp : 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" tmp : 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" tmp : 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" tmp : 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" tih : 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" cti : 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n", pp->win_id,\
				/* h 5 */
				pp->edge_y_filter_en, pp->e_gain_max, pp->f_gain_max, \
				pp->mp_lap_h_mode, pp->sp_lap_h_mode, \
				/* v 5 */
				pp->der_v_en, pp->der_gain_mapping, pp->max_sel, \
				pp->mp_lap_v_mode, pp->sp_lap_v_mode, \
				/* cmm 6 */
				pp->mp_sobel_weight, pp->mp_laplacian_weight, pp->sp_sobel_weight, \
				pp->sp_laplacian_weight, pp->flat_en, pp->flat_filter_type, \
				/*d_jag 12 */
				pp->edf_en, pp->center_blur_mode, pp->count_diff_th, pp->n_avg_mode, \
				pp->line_variation_diff_th, pp->level_th, pp->protect_th, pp->n_avg_gain, \
				pp->reg_g0_cnt_min, pp->reg_g0_mul, pp->reg_g1_protect_min, pp->reg_g1_mul, \
				/*e_map 5 */
				pp->amap2_sel, pp->amap_gain, pp->ga_max, pp->ga_th0, pp->ga_th1, \
				/*t_map 22*/
				pp->amap1_sel, pp->tmap_max_sel, pp->avg_sel, pp->tmap_gain, pp->gt_th0, \
				pp->gt_th0a, pp->gt_th0b, pp->gt_th1, pp->gt_gain0a, pp->gt_gain0b, \
				pp->gt_max, pp->a2tw_en, pp->a2tw_th0, pp->a2tw_th1, pp->exp_mode, \
				pp->coring_mode1, pp->coring_mode2, pp->g_th0, pp->g_th1, pp->var_h_th, \
				pp->var_v_th, pp->tmap_sc_var_th, \
				/*ti-h 7 */
				pp->enable, pp->coring_step, pp->gain0_en, pp->gain1_en, \
				pp->gain0_th0, pp->gain0_th1, pp->gain1_div_mode, \
				/* cti 8 */
				pp->cti_en, pp->coring_th0, pp->coring_th1, pp->coring_map_filter, \
				pp->coring_tap_size, pp->ycm_en1, pp->ycm_band_sel, pp->ycm_diff_th);
			if(PE_CHECK_WIN0(pp->win_id))
			{
				PE_FWI_M14_MISC_CTRL fw_param;
				fw_param.gt_th1 = GET_BITS(pp->gt_th1,0,8);
				fw_param.gt_th0 = GET_BITS(pp->gt_th0,0,8);
				fw_param.gt_th0a = GET_BITS(pp->gt_th0a,0,8);
				fw_param.gt_th0b = GET_BITS(pp->gt_th0b,0,8);
				ret = PE_FWI_M14_SetShpMiscCtrl(&fw_param);
				PE_SHP_HW_M14_CHECK_CODE(ret, break, \
					"[%s,%d] PE_FWI_M14_SetShpMiscCtrl() error.\n", __F__, __L__);

				#ifdef PE_HW_M14B_USE_REG_CTRL
				/* h 5 */
				PE_SHP_HW_M14B_PE1_WR01(pe1_derh_ctrl_06,edge_y_filter_en,	GET_BITS(pp->edge_y_filter_en,0,1));
				PE_SHP_HW_M14B_PE1_WR02(pe1_derh_ctrl_0c,e_gain_max,		GET_BITS(pp->e_gain_max,0,6),\
														f_gain_max,			GET_BITS(pp->f_gain_max,0,6));
				PE_SHP_HW_M14B_PE1_WR01(pe1_mp_ctrl_06,	lap_h_mode,			GET_BITS(pp->mp_lap_h_mode,0,3));
				PE_SHP_HW_M14B_PE1_WR01(pe1_sp_ctrl_06,	lap_h_mode,			GET_BITS(pp->sp_lap_h_mode,0,3));
				/* v 5 */
				PE_SHP_HW_M14B_PE1_WR02(pe1_derv_ctrl_0,der_v_en,			GET_BITS(pp->der_v_en,0,1),\
														der_gain_mapping,	GET_BITS(pp->der_gain_mapping,0,2));
				PE_SHP_HW_M14B_PE1_WR01(pe1_derv_ctrl_2,max_sel,			GET_BITS(pp->max_sel,0,3));
				PE_SHP_HW_M14B_PE1_WR01(pe1_mp_ctrl_06,	lap_v_mode,			GET_BITS(pp->mp_lap_v_mode,0,3));
				PE_SHP_HW_M14B_PE1_WR01(pe1_sp_ctrl_06,	lap_v_mode,			GET_BITS(pp->sp_lap_v_mode,0,3));
				/* cmm 6 */
				PE_SHP_HW_M14B_PE1_WR02(pe1_mp_ctrl_02,	sobel_weight,		GET_BITS(pp->mp_sobel_weight,0,8),\
														laplacian_weight,	GET_BITS(pp->mp_laplacian_weight,0,8));
				PE_SHP_HW_M14B_PE1_WR02(pe1_sp_ctrl_02,	sobel_weight,		GET_BITS(pp->sp_sobel_weight,0,8),\
														laplacian_weight,	GET_BITS(pp->sp_laplacian_weight,0,8));
				PE_SHP_HW_M14B_PE1_WR02(pe1_derh_ctrl_07,flat_filter_en,	GET_BITS(pp->flat_en,0,1),\
														flat_filter_type,	GET_BITS(pp->flat_filter_type,0,1));
				/*d_jag 12 */
				PE_SHP_HW_M14B_PE1_WR05(pe1_dj_ctrl_00,edf_en,				GET_BITS(pp->edf_en,0,1),\
														center_blur_mode, 	GET_BITS(pp->center_blur_mode,0,1),\
														count_diff_th, 		GET_BITS(pp->count_diff_th,0,5),\
														n_avg_mode,			GET_BITS(pp->n_avg_mode,0,1),\
														line_variation_diff_threshold,GET_BITS(pp->line_variation_diff_th,0,8));
				PE_SHP_HW_M14B_PE1_WR03(pe1_dj_ctrl_01,	level_th,			GET_BITS(pp->level_th,0,8),\
														protect_th,			GET_BITS(pp->protect_th,0,8),\
														n_avg_gain,			GET_BITS(pp->n_avg_gain,0,8));
				PE_SHP_HW_M14B_PE1_WR02(pe1_dj_ctrl_02,	reg_g0_cnt_min,		GET_BITS(pp->reg_g0_cnt_min,0,5),\
														reg_g0_mul,			GET_BITS(pp->reg_g0_mul,0,5));
				PE_SHP_HW_M14B_PE1_WR02(pe1_dj_ctrl_03,	reg_g1_protect_min,	GET_BITS(pp->reg_g1_protect_min,0,8),\
														reg_g1_mul, 		GET_BITS(pp->reg_g1_mul,0,7));
				/*e_map 5 */
				PE_SHP_HW_M14B_PE1_WR04(pe1_coring_ctrl_00,	amap2_sel,		GET_BITS(pp->amap2_sel,0,2),\
															ga_max,			GET_BITS(pp->ga_max,0,6),\
															ga_th0,			GET_BITS(pp->ga_th0,0,8),\
															ga_th1,			GET_BITS(pp->ga_th1,0,8));
				PE_SHP_HW_M14B_PE1_WR01(pe1_coring_ctrl_07,	amap_gain,		GET_BITS(pp->amap_gain,0,7));
				/*t_map 22*/
				PE_SHP_HW_M14B_PE1_WR01(pe1_coring_ctrl_00,	amap1_sel,		GET_BITS(pp->amap1_sel,0,2));
				PE_SHP_HW_M14B_PE1_WR04(pe1_coring_ctrl_01,	max_sel,		GET_BITS(pp->tmap_max_sel,0,3),\
															avg_sel,		GET_BITS(pp->avg_sel,0,2),\
															a2tw_th0,		GET_BITS(pp->a2tw_th0,0,8),\
															a2tw_th1,		GET_BITS(pp->a2tw_th1,0,8));
				PE_SHP_HW_M14B_PE1_WR03(pe1_coring_ctrl_07,	tmap_gain,		GET_BITS(pp->tmap_gain,0,7),\
															g_th0,			GET_BITS(pp->g_th0,0,8),\
															g_th1,			GET_BITS(pp->g_th1,0,8));				
				PE_SHP_HW_M14B_PE1_WR02(pe1_coring_ctrl_06,	gt_gain0a,		GET_BITS(pp->gt_gain0a,0,6),\
															gt_gain0b,		GET_BITS(pp->gt_gain0b,0,6));
				PE_SHP_HW_M14B_PE1_WR02(pe1_coring_ctrl_05,	gt_max,			GET_BITS(pp->gt_max,0,6),\
															a2tw_en,		GET_BITS(pp->a2tw_en,0,1));
				PE_SHP_HW_M14B_PE1_WR05(pe1_coring_ctrl_04,	exp_mode,		GET_BITS(pp->exp_mode,0,2),\
															coring_mode1,	GET_BITS(pp->coring_mode1,0,2),\
															coring_mode2,	GET_BITS(pp->coring_mode2,0,2),\
															var_v_th,		GET_BITS(pp->var_v_th,0,8),\
															tmap_sc_var_th,	GET_BITS(pp->tmap_sc_var_th,0,8));
				PE_SHP_HW_M14B_PE1_WR01(pe1_coring_ctrl_02,	var_h_th,		GET_BITS(pp->var_h_th,0,8));
				/*ti-h 7 */
				PE_SHP_HW_M14B_PE1_WR02(pe1_ti_ctrl_0, 	enable,			GET_BITS(pp->enable, 0,1),\
													  	coring_step, 	GET_BITS(pp->coring_step,0,3));
				PE_SHP_HW_M14B_PE1_WR04(pe1_ti_ctrl_1, 	gain0_en, 		GET_BITS(pp->gain0_en,0,1),\
														gain1_en, 		GET_BITS(pp->gain1_en,0,1),\
														gain0_th0,		GET_BITS(pp->gain0_th0,0,8),\
														gain0_th1,		GET_BITS(pp->gain0_th1,0,8));
				PE_SHP_HW_M14B_PE1_WR01(pe1_ti_ctrl_2, 	gain1_div_mode, GET_BITS(pp->gain1_div_mode,0,1));
				/* cti 8 */
				PE_SHP_HW_M14B_PE1_WR01(pe1_cti_ctrl_00,cti_en,				GET_BITS(pp->cti_en,0,1));
				PE_SHP_HW_M14B_PE1_WR04(pe1_cti_ctrl_01,coring_th0,			GET_BITS(pp->coring_th0,0,8),\
														coring_th1,			GET_BITS(pp->coring_th1,0,8),\
														coring_map_filter, 	GET_BITS(pp->coring_map_filter,0,3),\
														coring_tap_size, 	GET_BITS(pp->coring_tap_size,0,3));
				PE_SHP_HW_M14B_PE1_WR03(pe1_cti_ctrl_02,ycm_en1,			GET_BITS(pp->ycm_en1,0,1),\
														ycm_band_sel,		GET_BITS(pp->ycm_band_sel,0,3),\
														ycm_diff_th,		GET_BITS(pp->ycm_diff_th,0,8));
				#ifdef PE_SHP_HW_M14B_SET_FW_PARAM_TO_REG_DIRECTLY
				PE_SHP_HW_M14B_PE1_WR02(pe1_coring_ctrl_05,gt_th1,	GET_BITS(pp->gt_th1,0,8), \
															gt_th0,	GET_BITS(pp->gt_th0,0,8));
				PE_SHP_HW_M14B_PE1_WR02(pe1_coring_ctrl_06,gt_th0a,	GET_BITS(pp->gt_th0a,0,8), \
															gt_th0b,GET_BITS(pp->gt_th0b,0,8));
				#else
				PE_SHP_HW_M14B_PE1_SET(pe1_coring_ctrl_05,gt_th1,GET_BITS(pp->gt_th1,0,8));
				PE_SHP_HW_M14B_PE1_SET(pe1_coring_ctrl_05,gt_th0,GET_BITS(pp->gt_th0,0,8));
				PE_SHP_HW_M14B_PE1_SET(pe1_coring_ctrl_06,gt_th0a,GET_BITS(pp->gt_th0a,0,8));
				PE_SHP_HW_M14B_PE1_SET(pe1_coring_ctrl_06,gt_th0b,GET_BITS(pp->gt_th0b,0,8));
				#endif
				#else
				/* h 5 */
				PE_PE1_M14B0_QWr01(pe1_derh_ctrl_06, edge_y_filter_en,	GET_BITS(pp->edge_y_filter_en,0,1));
				PE_PE1_M14B0_QWr02(pe1_derh_ctrl_0c, e_gain_max,		GET_BITS(pp->e_gain_max,0,6),\
													 f_gain_max,		GET_BITS(pp->f_gain_max,0,6));
				PE_PE1_M14B0_QWr01(pe1_mp_ctrl_06,	lap_h_mode,			GET_BITS(pp->mp_lap_h_mode,0,3));
				PE_PE1_M14B0_QWr01(pe1_sp_ctrl_06,	lap_h_mode,			GET_BITS(pp->sp_lap_h_mode,0,3));
				/* v 5 */
				PE_PE1_M14B0_QWr02(pe1_derv_ctrl_0,	der_v_en,			GET_BITS(pp->der_v_en,0,1),\
													der_gain_mapping,	GET_BITS(pp->der_gain_mapping,0,2));
				PE_PE1_M14B0_QWr01(pe1_derv_ctrl_2,	max_sel,			GET_BITS(pp->max_sel,0,3));
				PE_PE1_M14B0_QWr01(pe1_mp_ctrl_06,	lap_v_mode,			GET_BITS(pp->mp_lap_v_mode,0,3));
				PE_PE1_M14B0_QWr01(pe1_sp_ctrl_06,	lap_v_mode,			GET_BITS(pp->sp_lap_v_mode,0,3));
				/* cmm 6 */
				PE_PE1_M14B0_QWr02(pe1_mp_ctrl_02,	sobel_weight,		GET_BITS(pp->mp_sobel_weight,0,8),\
													laplacian_weight,	GET_BITS(pp->mp_laplacian_weight,0,8));
				PE_PE1_M14B0_QWr02(pe1_sp_ctrl_02,	sobel_weight,		GET_BITS(pp->sp_sobel_weight,0,8),\
													laplacian_weight,	GET_BITS(pp->sp_laplacian_weight,0,8));
				PE_PE1_M14B0_QWr02(pe1_derh_ctrl_07, flat_filter_en,	GET_BITS(pp->flat_en,0,1),\
													 flat_filter_type,	GET_BITS(pp->flat_filter_type,0,1));
				/*d_jag 12 */
				PE_PE1_M14B0_QWr05(pe1_dj_ctrl_00,	edf_en,				GET_BITS(pp->edf_en,0,1),\
													center_blur_mode, 	GET_BITS(pp->center_blur_mode,0,1),\
													count_diff_th, 		GET_BITS(pp->count_diff_th,0,5),\
													n_avg_mode,			GET_BITS(pp->n_avg_mode,0,1),\
													line_variation_diff_threshold,GET_BITS(pp->line_variation_diff_th,0,8));
				PE_PE1_M14B0_QWr03(pe1_dj_ctrl_01,	level_th,			GET_BITS(pp->level_th,0,8),\
													protect_th,			GET_BITS(pp->protect_th,0,8),\
													n_avg_gain,			GET_BITS(pp->n_avg_gain,0,8));
				PE_PE1_M14B0_QWr02(pe1_dj_ctrl_02,	reg_g0_cnt_min,		GET_BITS(pp->reg_g0_cnt_min,0,5),\
													reg_g0_mul,			GET_BITS(pp->reg_g0_mul,0,5));
				PE_PE1_M14B0_QWr02(pe1_dj_ctrl_03,	reg_g1_protect_min,	GET_BITS(pp->reg_g1_protect_min,0,8),\
													reg_g1_mul, 		GET_BITS(pp->reg_g1_mul,0,7));
				/*e_map 5 */
				PE_PE1_M14B0_QWr04(pe1_coring_ctrl_00,	amap2_sel,		GET_BITS(pp->amap2_sel,0,2),\
														ga_max,			GET_BITS(pp->ga_max,0,6),\
														ga_th0,			GET_BITS(pp->ga_th0,0,8),\
														ga_th1,			GET_BITS(pp->ga_th1,0,8));
				PE_PE1_M14B0_QWr01(pe1_coring_ctrl_07,	amap_gain,		GET_BITS(pp->amap_gain,0,7));
				/*t_map 22*/
				PE_PE1_M14B0_QWr01(pe1_coring_ctrl_00,	amap1_sel,		GET_BITS(pp->amap1_sel,0,2));
				PE_PE1_M14B0_QWr04(pe1_coring_ctrl_01,	max_sel,		GET_BITS(pp->tmap_max_sel,0,3),\
														avg_sel,		GET_BITS(pp->avg_sel,0,2),\
														a2tw_th0,		GET_BITS(pp->a2tw_th0,0,8),\
														a2tw_th1,		GET_BITS(pp->a2tw_th1,0,8));
				PE_PE1_M14B0_QWr03(pe1_coring_ctrl_07,	tmap_gain,		GET_BITS(pp->tmap_gain,0,7),\
														g_th0,			GET_BITS(pp->g_th0,0,8),\
														g_th1,			GET_BITS(pp->g_th1,0,8));				
				PE_PE1_M14B0_QWr02(pe1_coring_ctrl_06,	gt_gain0a,		GET_BITS(pp->gt_gain0a,0,6),\
														gt_gain0b,		GET_BITS(pp->gt_gain0b,0,6));
				PE_PE1_M14B0_QWr02(pe1_coring_ctrl_05,	gt_max,			GET_BITS(pp->gt_max,0,6),\
														a2tw_en,		GET_BITS(pp->a2tw_en,0,1));
				PE_PE1_M14B0_QWr05(pe1_coring_ctrl_04,	exp_mode,		GET_BITS(pp->exp_mode,0,2),\
														coring_mode1,	GET_BITS(pp->coring_mode1,0,2),\
														coring_mode2,	GET_BITS(pp->coring_mode2,0,2),\
														var_v_th,		GET_BITS(pp->var_v_th,0,8),\
														tmap_sc_var_th,	GET_BITS(pp->tmap_sc_var_th,0,8));
				PE_PE1_M14B0_QWr01(pe1_coring_ctrl_02,	var_h_th,		GET_BITS(pp->var_h_th,0,8));
				/*ti-h 7 */
				PE_PE1_M14B0_QWr02(pe1_ti_ctrl_0, 		enable,			GET_BITS(pp->enable, 0,1),\
												  		coring_step, 	GET_BITS(pp->coring_step,0,3));
				PE_PE1_M14B0_QWr04(pe1_ti_ctrl_1, 		gain0_en, 		GET_BITS(pp->gain0_en,0,1),\
														gain1_en, 		GET_BITS(pp->gain1_en,0,1),\
														gain0_th0,		GET_BITS(pp->gain0_th0,0,8),\
														gain0_th1,		GET_BITS(pp->gain0_th1,0,8));
				PE_PE1_M14B0_QWr01(pe1_ti_ctrl_2, 		gain1_div_mode, GET_BITS(pp->gain1_div_mode,0,1));
				/* cti 8 */
				PE_PE1_M14B0_QWr01(pe1_cti_ctrl_00,	cti_en,				GET_BITS(pp->cti_en,0,1));
				PE_PE1_M14B0_QWr04(pe1_cti_ctrl_01,	coring_th0,			GET_BITS(pp->coring_th0,0,8),\
													coring_th1,			GET_BITS(pp->coring_th1,0,8),\
													coring_map_filter, 	GET_BITS(pp->coring_map_filter,0,3),\
													coring_tap_size, 	GET_BITS(pp->coring_tap_size,0,3));
				PE_PE1_M14B0_QWr03(pe1_cti_ctrl_02,	ycm_en1,			GET_BITS(pp->ycm_en1,0,1),\
													ycm_band_sel,		GET_BITS(pp->ycm_band_sel,0,3),\
													ycm_diff_th,		GET_BITS(pp->ycm_diff_th,0,8));
				#ifdef PE_SHP_HW_M14B_SET_FW_PARAM_TO_REG_DIRECTLY
				PE_PE1_M14B0_QWr02(pe1_coring_ctrl_05, 	gt_th1,		GET_BITS(pp->gt_th1,0,8), \
														gt_th0,		GET_BITS(pp->gt_th0,0,8));
				PE_PE1_M14B0_QWr02(pe1_coring_ctrl_06, 	gt_th0a,	GET_BITS(pp->gt_th0a,0,8), \
														gt_th0b,	GET_BITS(pp->gt_th0b,0,8));
				#endif
				#endif
			}
		}
		else if(PE_KDRV_VER_M14AX)
		{
			LX_PE_SHP_RE2_MISC_T *pp=(LX_PE_SHP_RE2_MISC_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			PE_SHP_HW_M14_DBG_PRINT("set[%d] :  resolution misc ctrl\n"\
				" e-map : sel:%d, ga:%d,%d,%d\n"\
				" t-map : sel:%d,%d, g:%d, gt:%d,%d,%d,%d,%d,%d,%d, cor1:%d,var_th:%d\n"\
				" d-jag : en:%d, th:%d,%d, g:%d\n"\
				"         h_cnt:min:%d,max:%d, v_cnt:min:%d,max:%d\n",pp->win_id,\
				pp->amap2_sel,pp->ga_max,pp->ga_th0,pp->ga_th1,pp->amap1_sel,\
				pp->max_sel,pp->tmap_gain,pp->gt_th0,pp->gt_th0a,pp->gt_th0b,\
				pp->gt_th1,pp->gt_gain0a,pp->gt_gain0b,pp->gt_max,pp->coring_mode1,pp->var_th,\
				pp->center_blur_en,pp->level_th,pp->protect_th,pp->n_avg_gain,\
				pp->dj_h_count_min,pp->dj_h_count_max,pp->dj_v_count_min,pp->dj_v_count_max);
			if(PE_CHECK_WIN0(pp->win_id))
			{
				PE_FWI_M14_MISC_CTRL fw_param;
				fw_param.gt_th1 = GET_BITS(pp->gt_th1,0,8);
				fw_param.gt_th0 = GET_BITS(pp->gt_th0,0,8);
				fw_param.gt_th0a = GET_BITS(pp->gt_th0a,0,8);
				fw_param.gt_th0b = GET_BITS(pp->gt_th0b,0,8);
				ret = PE_FWI_M14_SetShpMiscCtrl(&fw_param);
				PE_SHP_HW_M14_CHECK_CODE(ret, break, \
					"[%s,%d] PE_FWI_M14_SetShpMiscCtrl() error.\n", __F__, __L__);

				/* cit*/
				PE_P1L_M14A0_QWr01(pe1_cti_ctrl_00,	cti_en,				GET_BITS(pp->cti_en,0,1));
				PE_P1L_M14A0_QWr04(pe1_cti_ctrl_01,	coring_th0,			GET_BITS(pp->coring_th0,0,8),\
													coring_th1,			GET_BITS(pp->coring_th1,0,8),\
													coring_map_filter, 	GET_BITS(pp->coring_map_filter,0,3),\
													coring_tap_size, 	GET_BITS(pp->coring_tap_size,0,3));
				PE_P1L_M14A0_QWr03(pe1_cti_ctrl_02,	ycm_en1,			GET_BITS(pp->ycm_en1,0,1),\
													ycm_band_sel,		GET_BITS(pp->ycm_band_sel,0,3),\
													ycm_diff_th,		GET_BITS(pp->ycm_diff_th,0,8));
				/* h */
				PE_P1L_M14A0_QWr01(pe1_derh_ctrl_06, edge_y_filter_en,	GET_BITS(pp->edge_y_filter_en,0,1));
				PE_P1L_M14A0_QWr02(pe1_derh_ctrl_0c, e_gain_max,		GET_BITS(pp->e_gain_max,0,6),\
													 f_gain_max,		GET_BITS(pp->f_gain_max,0,6));
				PE_P1L_M14A0_QWr01(pe1_mp_ctrl_06,	lap_h_mode,			GET_BITS(pp->mp_lap_h_mode,0,3));
				PE_P1L_M14A0_QWr01(pe1_sp_ctrl_06,	lap_h_mode,			GET_BITS(pp->sp_lap_h_mode,0,3));
				/* v */
				PE_P1L_M14A0_QWr02(pe1_derv_ctrl_0,	der_v_en,			GET_BITS(pp->der_v_en,0,1),\
													der_gain_mapping,	GET_BITS(pp->der_gain_mapping,0,2));
				PE_P1L_M14A0_QWr01(pe1_derv_ctrl_2,	max_sel,			GET_BITS(pp->max_sel,0,3));
				PE_P1L_M14A0_QWr01(pe1_mp_ctrl_06,	lap_v_mode,			GET_BITS(pp->mp_lap_v_mode,0,3));
				PE_P1L_M14A0_QWr01(pe1_sp_ctrl_06,	lap_v_mode,			GET_BITS(pp->sp_lap_v_mode,0,3));
				/* cmn */
				PE_P1L_M14A0_QWr02(pe1_mp_ctrl_02,	sobel_weight,		GET_BITS(pp->mp_sobel_weight,0,8),\
													laplacian_weight,	GET_BITS(pp->mp_laplacian_weight,0,8));
				PE_P1L_M14A0_QWr02(pe1_sp_ctrl_02,	sobel_weight,		GET_BITS(pp->sp_sobel_weight,0,8),\
													laplacian_weight,	GET_BITS(pp->sp_laplacian_weight,0,8));
				
				PE_P1L_M14A0_QWr02(pe1_derh_ctrl_07, flat_filter_en,	GET_BITS(pp->flat_en,0,1),\
													 flat_filter_type,	GET_BITS(pp->flat_filter_type,0,1));
				/* d_jag */
				PE_P1L_M14A0_QWr04(pe1_dj_ctrl_00,	edf_en,				GET_BITS(pp->edf_en,0,1),\
													center_blur_en, 	GET_BITS(pp->center_blur_en,0,1),\
													count_diff_th, 		GET_BITS(pp->count_diff_th,0,5),\
													n_avg_mode,			GET_BITS(pp->n_avg_mode,0,1));
				PE_P1L_M14A0_QWr01(pe1_dj_ctrl_00,	line_variation_diff_threshold,	GET_BITS(pp->line_variation_diff_th,0,8));
				PE_P1L_M14A0_QWr03(pe1_dj_ctrl_01,	level_th,			GET_BITS(pp->level_th,0,8),\
													protect_th,			GET_BITS(pp->protect_th,0,8),\
													n_avg_gain,			GET_BITS(pp->n_avg_gain,0,8));
				PE_P1L_M14A0_QWr02(pe1_dj_ctrl_02,	edf_count_min,		GET_BITS(pp->edf_count_min,0,5),\
													edf_count_max,		GET_BITS(pp->edf_count_max,0,5));
				PE_P1L_M14A0_QWr04(pe1_dj_ctrl_03,	dj_h_count_min,		GET_BITS(pp->dj_h_count_min,0,5),\
													dj_h_count_max, 	GET_BITS(pp->dj_h_count_max,0,5),\
													dj_v_count_min, 	GET_BITS(pp->dj_v_count_min,0,5),\
													dj_v_count_max,		GET_BITS(pp->dj_v_count_max,0,5));
				/* e_map */
				PE_P1L_M14A0_QWr04(pe1_coring_ctrl_00,	amap2_sel,		GET_BITS(pp->amap2_sel,0,2),\
														ga_max,			GET_BITS(pp->ga_max,0,6),\
														ga_th0,			GET_BITS(pp->ga_th0,0,8),\
														ga_th1,			GET_BITS(pp->ga_th1,0,8));
				PE_P1L_M14A0_QWr01(pe1_coring_ctrl_07,	amap_gain,		GET_BITS(pp->amap_gain,0,7));
				/* t_map */
				PE_P1L_M14A0_QWr01(pe1_coring_ctrl_00,	amap1_sel,		GET_BITS(pp->amap1_sel,0,2));
				PE_P1L_M14A0_QWr04(pe1_coring_ctrl_01,	max_sel,		GET_BITS(pp->tmap_max_sel,0,3),\
														avg_sel,		GET_BITS(pp->avg_sel,0,2),\
														a2tw_th0,		GET_BITS(pp->a2tw_th0,0,8),\
														a2tw_th1,		GET_BITS(pp->a2tw_th1,0,8));
				PE_P1L_M14A0_QWr03(pe1_coring_ctrl_07,	tmap_gain,		GET_BITS(pp->tmap_gain,0,7),\
														g_th0,			GET_BITS(pp->g_th0,0,8),\
														g_th1,			GET_BITS(pp->g_th1,0,8));				
				PE_P1L_M14A0_QWr02(pe1_coring_ctrl_06,	gt_gain0a,		GET_BITS(pp->gt_gain0a,0,6),\
														gt_gain0b,		GET_BITS(pp->gt_gain0b,0,6));
				PE_P1L_M14A0_QWr02(pe1_coring_ctrl_05,	gt_max,			GET_BITS(pp->gt_max,0,6),\
														a2tw_en,		GET_BITS(pp->a2tw_en,0,1));
				PE_P1L_M14A0_QWr04(pe1_coring_ctrl_04,	exp_mode,		GET_BITS(pp->exp_mode,0,2),\
														coring_mode1,	GET_BITS(pp->coring_mode1,0,2),\
														coring_mode2,	GET_BITS(pp->coring_mode2,0,2),\
														var_th,			GET_BITS(pp->var_th,0,11));
				/* ti-h*/
				PE_P1L_M14A0_QWr02(pe1_ti_ctrl_0, 		enable,			GET_BITS(pp->enable, 0,1),\
												  		coring_step, 	GET_BITS(pp->coring_step,0,3));
				PE_P1L_M14A0_QWr04(pe1_ti_ctrl_1, 		gain0_en, 		GET_BITS(pp->gain0_en,0,1),\
														gain1_en, 		GET_BITS(pp->gain1_en,0,1),\
														gain0_th0,		GET_BITS(pp->gain0_th0,0,8),\
														gain1_th1,		GET_BITS(pp->gain1_th1,0,8));
				PE_P1L_M14A0_QWr01(pe1_ti_ctrl_2, 		gain1_div_mode, GET_BITS(pp->gain1_div_mode,0,1));
			}
		}
		else
		{
			PE_SHP_HW_M14_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	} while (0);
	return ret;
}
/**
 * get resolution misc control
 * - use input struct LX_PE_SHP_RE2_MISC_T for M14AX
 * - use input struct LX_PE_SHP_RE1_MISC_T for M14AX
 *
 * @param   *pstParams [in/out] void
 * @return  OK if success, ERROR otherwise.
 * @see		function description
 * @author
 */
int PE_SHP_HW_M14_GetReMiscCtrl(void *pstParams)
{
	int ret = RET_OK;
	LX_PE_WIN_ID win_id;
	do {
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_M14BX)
		{
			LX_PE_SHP_RE3_MISC_T *pp=(LX_PE_SHP_RE3_MISC_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			win_id = PE_GET_CHECKED_WINID(pp->win_id);
			if(PE_CHECK_WIN0(win_id))
			{
				PE_FWI_M14_SETTINGS_T fw_inf;
				ret = PE_FWI_M14_GetCurFwiSettings(&fw_inf);
				PE_SHP_HW_M14_CHECK_CODE(ret, break, \
					"[%s,%d] PE_FWI_M14_GetCurFwiSettings() error.\n", __F__, __L__);
				PE_SHP_HW_M14_DBG_PRINT("fw_ctrl.enable : %s.\n", \
					fw_inf.fw_ctrl.enable? "yes":"no");

				if(fw_inf.fw_ctrl.enable)
				{
					pp->gt_th0 = fw_inf.shp_m_ctrl.gt_th0;
					pp->gt_th1 = fw_inf.shp_m_ctrl.gt_th1;
					pp->gt_th0a = fw_inf.shp_m_ctrl.gt_th0a;
					pp->gt_th0b = fw_inf.shp_m_ctrl.gt_th0b;
				}
				else
				{
					PE_PE1_M14B0_RdFL(pe1_coring_ctrl_05);
					PE_PE1_M14B0_RdFL(pe1_coring_ctrl_06);
					PE_PE1_M14B0_Rd01(pe1_coring_ctrl_05,	gt_th1,		pp->gt_th1);
					PE_PE1_M14B0_Rd01(pe1_coring_ctrl_05,	gt_th0,		pp->gt_th0);
					PE_PE1_M14B0_Rd01(pe1_coring_ctrl_06,	gt_th0a,	pp->gt_th0a);
					PE_PE1_M14B0_Rd01(pe1_coring_ctrl_06,	gt_th0b,	pp->gt_th0b);
				}
				/* h 5 */
				PE_PE1_M14B0_QRd01(pe1_derh_ctrl_06, edge_y_filter_en,	pp->edge_y_filter_en);
				PE_PE1_M14B0_QRd02(pe1_derh_ctrl_0c, e_gain_max,		pp->e_gain_max,\
													 f_gain_max,		pp->f_gain_max);
				PE_PE1_M14B0_QRd01(pe1_mp_ctrl_06,	lap_h_mode,			pp->mp_lap_h_mode);
				PE_PE1_M14B0_QRd01(pe1_sp_ctrl_06,	lap_h_mode,			pp->sp_lap_h_mode);
				/* v 5 */
				PE_PE1_M14B0_QRd02(pe1_derv_ctrl_0,	der_v_en,			pp->der_v_en,\
													der_gain_mapping,	pp->der_gain_mapping);
				PE_PE1_M14B0_QRd01(pe1_derv_ctrl_2,	max_sel,			pp->max_sel);
				PE_PE1_M14B0_QRd01(pe1_mp_ctrl_06,	lap_v_mode,			pp->mp_lap_v_mode);
				PE_PE1_M14B0_QRd01(pe1_sp_ctrl_06,	lap_v_mode,			pp->sp_lap_v_mode);
				/* cmm 6 */
				PE_PE1_M14B0_QRd02(pe1_mp_ctrl_02,	sobel_weight,		pp->mp_sobel_weight,\
													laplacian_weight,	pp->mp_laplacian_weight);
				PE_PE1_M14B0_QRd02(pe1_sp_ctrl_02,	sobel_weight,		pp->sp_sobel_weight,\
													laplacian_weight,	pp->sp_laplacian_weight);
				PE_PE1_M14B0_QRd02(pe1_derh_ctrl_07, flat_filter_en,	pp->flat_en,\
													 flat_filter_type,	pp->flat_filter_type);
				/*d_jag 12 */
				PE_PE1_M14B0_QRd05(pe1_dj_ctrl_00,	edf_en,				pp->edf_en,\
													center_blur_mode, 	pp->center_blur_mode,\
													count_diff_th, 		pp->count_diff_th,\
													n_avg_mode,			pp->n_avg_mode,\
													line_variation_diff_threshold,pp->line_variation_diff_th);
				PE_PE1_M14B0_QRd03(pe1_dj_ctrl_01,	level_th,			pp->level_th,\
													protect_th,			pp->protect_th,\
													n_avg_gain,			pp->n_avg_gain);
				PE_PE1_M14B0_QRd02(pe1_dj_ctrl_02,	reg_g0_cnt_min,		pp->reg_g0_cnt_min,\
													reg_g0_mul,			pp->reg_g0_mul);
				PE_PE1_M14B0_QRd02(pe1_dj_ctrl_03,	reg_g1_protect_min,	pp->reg_g1_protect_min,\
													reg_g1_mul, 		pp->reg_g1_mul);
				/*e_map 5 */
				PE_PE1_M14B0_QRd04(pe1_coring_ctrl_00,	amap2_sel,		pp->amap2_sel,\
														ga_max,			pp->ga_max,\
														ga_th0,			pp->ga_th0,\
														ga_th1,			pp->ga_th1);
				PE_PE1_M14B0_QRd01(pe1_coring_ctrl_07,	amap_gain,		pp->amap_gain);
				/*t_map 22*/
				PE_PE1_M14B0_QRd01(pe1_coring_ctrl_00,	amap1_sel,		pp->amap1_sel);
				PE_PE1_M14B0_QRd04(pe1_coring_ctrl_01,	max_sel,		pp->tmap_max_sel,\
														avg_sel,		pp->avg_sel,\
														a2tw_th0,		pp->a2tw_th0,\
														a2tw_th1,		pp->a2tw_th1);
				PE_PE1_M14B0_QRd03(pe1_coring_ctrl_07,	tmap_gain,		pp->tmap_gain,\
														g_th0,			pp->g_th0,\
														g_th1,			pp->g_th1);				
				PE_PE1_M14B0_QRd02(pe1_coring_ctrl_06,	gt_gain0a,		pp->gt_gain0a,\
														gt_gain0b,		pp->gt_gain0b);
				PE_PE1_M14B0_QRd02(pe1_coring_ctrl_05,	gt_max,			pp->gt_max,\
														a2tw_en,		pp->a2tw_en);
				PE_PE1_M14B0_QRd05(pe1_coring_ctrl_04,	exp_mode,		pp->exp_mode,\
														coring_mode1,	pp->coring_mode1,\
														coring_mode2,	pp->coring_mode2,\
														var_v_th,		pp->var_v_th,\
														tmap_sc_var_th,	pp->tmap_sc_var_th);
				PE_PE1_M14B0_QRd01(pe1_coring_ctrl_02,	var_h_th,		pp->var_h_th);
				/*ti-h 7 */
				PE_PE1_M14B0_QRd02(pe1_ti_ctrl_0, 		enable,			pp->enable,\
												  		coring_step, 	pp->coring_step);
				PE_PE1_M14B0_QRd04(pe1_ti_ctrl_1, 		gain0_en, 		pp->gain0_en,\
														gain1_en, 		pp->gain1_en,\
														gain0_th0,		pp->gain0_th0,\
														gain0_th1,		pp->gain0_th1);
				PE_PE1_M14B0_QRd01(pe1_ti_ctrl_2, 		gain1_div_mode, pp->gain1_div_mode);
				/* cti 8 */
				PE_PE1_M14B0_QRd01(pe1_cti_ctrl_00,	cti_en,				pp->cti_en);
				PE_PE1_M14B0_QRd04(pe1_cti_ctrl_01,	coring_th0,			pp->coring_th0,\
													coring_th1,			pp->coring_th1,\
													coring_map_filter, 	pp->coring_map_filter,\
													coring_tap_size, 	pp->coring_tap_size);
				PE_PE1_M14B0_QRd03(pe1_cti_ctrl_02,	ycm_en1,			pp->ycm_en1,\
													ycm_band_sel,		pp->ycm_band_sel,\
													ycm_diff_th,		pp->ycm_diff_th);
			}
			PE_SHP_HW_M14_DBG_PRINT("get[%d] :  resolution misc ctrl\n"\
				" hor : 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" ver : 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" cmn : 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" djg : 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" djg : 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" emp : 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" tmp : 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" tmp : 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" tmp : 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" tih : 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" cti : 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n", pp->win_id,\
				/* h 5 */
				pp->edge_y_filter_en, pp->e_gain_max, pp->f_gain_max, \
				pp->mp_lap_h_mode, pp->sp_lap_h_mode, \
				/* v 5 */
				pp->der_v_en, pp->der_gain_mapping, pp->max_sel, \
				pp->mp_lap_v_mode, pp->sp_lap_v_mode, \
				/* cmm 6 */
				pp->mp_sobel_weight, pp->mp_laplacian_weight, pp->sp_sobel_weight, \
				pp->sp_laplacian_weight, pp->flat_en, pp->flat_filter_type, \
				/*d_jag 12 */
				pp->edf_en, pp->center_blur_mode, pp->count_diff_th, pp->n_avg_mode, \
				pp->line_variation_diff_th, pp->level_th, pp->protect_th, pp->n_avg_gain, \
				pp->reg_g0_cnt_min, pp->reg_g0_mul, pp->reg_g1_protect_min, pp->reg_g1_mul, \
				/*e_map 5 */
				pp->amap2_sel, pp->amap_gain, pp->ga_max, pp->ga_th0, pp->ga_th1, \
				/*t_map 22*/
				pp->amap1_sel, pp->tmap_max_sel, pp->avg_sel, pp->tmap_gain, pp->gt_th0, \
				pp->gt_th0a, pp->gt_th0b, pp->gt_th1, pp->gt_gain0a, pp->gt_gain0b, \
				pp->gt_max, pp->a2tw_en, pp->a2tw_th0, pp->a2tw_th1, pp->exp_mode, \
				pp->coring_mode1, pp->coring_mode2, pp->g_th0, pp->g_th1, pp->var_h_th, \
				pp->var_v_th, pp->tmap_sc_var_th, \
				/*ti-h 7 */
				pp->enable, pp->coring_step, pp->gain0_en, pp->gain1_en, \
				pp->gain0_th0, pp->gain0_th1, pp->gain1_div_mode, \
				/* cti 8 */
				pp->cti_en, pp->coring_th0, pp->coring_th1, pp->coring_map_filter, \
				pp->coring_tap_size, pp->ycm_en1, pp->ycm_band_sel, pp->ycm_diff_th);
		}
		else if(PE_KDRV_VER_M14AX)
		{
			LX_PE_SHP_RE2_MISC_T *pp=(LX_PE_SHP_RE2_MISC_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			win_id = PE_GET_CHECKED_WINID(pp->win_id);
			if(PE_CHECK_WIN0(win_id))
			{
				PE_FWI_M14_SETTINGS_T fw_inf;
				ret = PE_FWI_M14_GetCurFwiSettings(&fw_inf);
				PE_SHP_HW_M14_CHECK_CODE(ret, break, \
					"[%s,%d] PE_FWI_M14_GetCurFwiSettings() error.\n", __F__, __L__);
				PE_SHP_HW_M14_DBG_PRINT("fw_ctrl.enable : %s.\n", \
					fw_inf.fw_ctrl.enable? "yes":"no");

				if(fw_inf.fw_ctrl.enable)
				{
					pp->gt_th0 = fw_inf.shp_m_ctrl.gt_th0;
					pp->gt_th1 = fw_inf.shp_m_ctrl.gt_th1;
					pp->gt_th0a = fw_inf.shp_m_ctrl.gt_th0a;
					pp->gt_th0b = fw_inf.shp_m_ctrl.gt_th0b;
				}
				else
				{
					PE_P1L_M14A0_RdFL(pe1_coring_ctrl_05);
					PE_P1L_M14A0_RdFL(pe1_coring_ctrl_06);
					PE_P1L_M14A0_Rd01(pe1_coring_ctrl_05,	gt_th1,		pp->gt_th1);
					PE_P1L_M14A0_Rd01(pe1_coring_ctrl_05,	gt_th0,		pp->gt_th0);
					PE_P1L_M14A0_Rd01(pe1_coring_ctrl_06,	gt_th0a,	pp->gt_th0a);
					PE_P1L_M14A0_Rd01(pe1_coring_ctrl_06,	gt_th0b,	pp->gt_th0b);
				}
				/* cit*/
				PE_P1L_M14A0_QRd01(pe1_cti_ctrl_00,	cti_en,				pp->cti_en);
				PE_P1L_M14A0_QRd04(pe1_cti_ctrl_01,	coring_th0,			pp->coring_th0,\
													coring_th1,			pp->coring_th1,\
													coring_map_filter, 	pp->coring_map_filter,\
													coring_tap_size, 	pp->coring_tap_size);
				PE_P1L_M14A0_QRd03(pe1_cti_ctrl_02,	ycm_en1,			pp->ycm_en1,\
													ycm_band_sel,		pp->ycm_band_sel,\
													ycm_diff_th,		pp->ycm_diff_th);
				/* h */
				PE_P1L_M14A0_QRd01(pe1_derh_ctrl_06, edge_y_filter_en,	pp->edge_y_filter_en);
				PE_P1L_M14A0_QRd02(pe1_derh_ctrl_0c, e_gain_max,		pp->e_gain_max,\
													 f_gain_max,		pp->f_gain_max);
				PE_P1L_M14A0_QRd01(pe1_mp_ctrl_06,	lap_h_mode,			pp->mp_lap_h_mode);
				PE_P1L_M14A0_QRd01(pe1_sp_ctrl_06,	lap_h_mode,			pp->sp_lap_h_mode);
				/* v */
				PE_P1L_M14A0_QRd02(pe1_derv_ctrl_0,	der_v_en,			pp->der_v_en,\
													der_gain_mapping,	pp->der_gain_mapping);
				PE_P1L_M14A0_QRd01(pe1_derv_ctrl_2,	max_sel,			pp->max_sel);
				PE_P1L_M14A0_QRd01(pe1_mp_ctrl_06,	lap_v_mode,			pp->mp_lap_v_mode);
				PE_P1L_M14A0_QRd01(pe1_sp_ctrl_06,	lap_v_mode,			pp->sp_lap_v_mode);
				/* cmn */
				PE_P1L_M14A0_QRd02(pe1_mp_ctrl_02,	sobel_weight,		pp->mp_sobel_weight,\
													laplacian_weight,	pp->mp_laplacian_weight);
				PE_P1L_M14A0_QRd02(pe1_sp_ctrl_02,	sobel_weight,		pp->sp_sobel_weight,\
													laplacian_weight,	pp->sp_laplacian_weight);
				
				PE_P1L_M14A0_QRd02(pe1_derh_ctrl_07, flat_filter_en,	pp->flat_en,\
													 flat_filter_type,	pp->flat_filter_type);
				/* d_jag */
				PE_P1L_M14A0_QRd04(pe1_dj_ctrl_00,	edf_en,				pp->edf_en,\
													center_blur_en, 	pp->center_blur_en,\
													count_diff_th, 		pp->count_diff_th,\
													n_avg_mode,			pp->n_avg_mode);
				PE_P1L_M14A0_QRd01(pe1_dj_ctrl_00,	line_variation_diff_threshold,	pp->line_variation_diff_th);
				PE_P1L_M14A0_QRd03(pe1_dj_ctrl_01,	level_th,			pp->level_th,\
													protect_th,			pp->protect_th,\
													n_avg_gain,			pp->n_avg_gain);
				PE_P1L_M14A0_QRd02(pe1_dj_ctrl_02,	edf_count_min,		pp->edf_count_min,\
													edf_count_max,		pp->edf_count_max);
				PE_P1L_M14A0_QRd04(pe1_dj_ctrl_03,	dj_h_count_min,		pp->dj_h_count_min,\
													dj_h_count_max, 	pp->dj_h_count_max,\
													dj_v_count_min, 	pp->dj_v_count_min,\
													dj_v_count_max,		pp->dj_v_count_max);
				/* e_map */
				PE_P1L_M14A0_QRd04(pe1_coring_ctrl_00,	amap2_sel,		pp->amap2_sel,\
														ga_max,			pp->ga_max,\
														ga_th0,			pp->ga_th0,\
														ga_th1,			pp->ga_th1);
				PE_P1L_M14A0_QRd01(pe1_coring_ctrl_07,	amap_gain,		pp->amap_gain);
				/* t_map */
				PE_P1L_M14A0_QRd01(pe1_coring_ctrl_00,	amap1_sel,		pp->amap1_sel);
				PE_P1L_M14A0_QRd04(pe1_coring_ctrl_01,	max_sel,		pp->tmap_max_sel,\
														avg_sel,		pp->avg_sel,\
														a2tw_th0,		pp->a2tw_th0,\
														a2tw_th1,		pp->a2tw_th1);
				PE_P1L_M14A0_QRd03(pe1_coring_ctrl_07,	tmap_gain,		pp->tmap_gain,\
														g_th0,			pp->g_th0,\
														g_th1,			pp->g_th1);				
				PE_P1L_M14A0_QRd02(pe1_coring_ctrl_06,	gt_gain0a,		pp->gt_gain0a,\
														gt_gain0b,		pp->gt_gain0b);
				PE_P1L_M14A0_QRd02(pe1_coring_ctrl_05,	gt_max,			pp->gt_max,\
														a2tw_en,		pp->a2tw_en);
				PE_P1L_M14A0_QRd04(pe1_coring_ctrl_04,	exp_mode,		pp->exp_mode,\
														coring_mode1,	pp->coring_mode1,\
														coring_mode2,	pp->coring_mode2,\
														var_th,			pp->var_th);
				/* ti-h*/
				PE_P1L_M14A0_QRd02(pe1_ti_ctrl_0, 		enable,			pp->enable,\
												  		coring_step, 	pp->coring_step);
				PE_P1L_M14A0_QRd04(pe1_ti_ctrl_1, 		gain0_en, 		pp->gain0_en,\
														gain1_en, 		pp->gain1_en,\
														gain0_th0,		pp->gain0_th0,\
														gain1_th1,		pp->gain1_th1);
				PE_P1L_M14A0_QRd01(pe1_ti_ctrl_2, 		gain1_div_mode, pp->gain1_div_mode);
			}
			PE_SHP_HW_M14_DBG_PRINT("get[%d] :  resolution misc ctrl\n"\
				" e-map : sel:%d, ga:%d,%d,%d\n"\
				" t-map : sel:%d,%d, g:%d, gt:%d,%d,%d,%d,%d,%d,%d, cor1:%d,var_th:%d\n"\
				" d-jag : en:%d, th:%d,%d, g:%d\n",pp->win_id,\
				pp->amap2_sel,pp->ga_max,pp->ga_th0,pp->ga_th1,pp->amap1_sel,\
				pp->max_sel,pp->tmap_gain,pp->gt_th0,pp->gt_th0a,pp->gt_th0b,\
				pp->gt_th1,pp->gt_gain0a,pp->gt_gain0b,pp->gt_max,pp->coring_mode1,pp->var_th,\
				pp->center_blur_en,pp->level_th,pp->protect_th,pp->n_avg_gain);
		}
		else
		{
			PE_SHP_HW_M14_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * set edge enhance control
 * - use input struct LX_PE_SHP_EE1_T for M14
 *
 * @param   *pstParams [in] void
 * @return  OK if success, ERROR otherwise.
 * @see		function description
 * @author
 */
int PE_SHP_HW_M14_SetEdgeEnhanceCtrl(void *pstParams)
{
	int ret = RET_OK;
	do {
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_M14)
		{
			LX_PE_SHP_EE1_T *pp=(LX_PE_SHP_EE1_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			PE_SHP_HW_M14_DBG_PRINT("set[%d]: mp g_b,w:%d,%d, sp g_b,w:%d,%d\n",pp->win_id,\
				pp->mp_edge_gain_b,pp->mp_edge_gain_w,pp->sp_edge_gain_b,pp->sp_edge_gain_w);
			if(PE_CHECK_WIN0(pp->win_id))
			{
				PE_FWI_M14_EDGE_GAIN fw_param;
				fw_param.co09_gain_b = GET_BITS(pp->mp_edge_gain_b,0,7);
				fw_param.co09_gain_w = GET_BITS(pp->mp_edge_gain_w,0,7);
				fw_param.dp01_gain_b = GET_BITS(pp->sp_edge_gain_b,0,7);
				fw_param.dp01_gain_w = GET_BITS(pp->sp_edge_gain_w,0,7);
				ret = PE_FWI_M14_SetEdgeGainCtrl(&fw_param);
				PE_SHP_HW_M14_CHECK_CODE(ret, break, "[%s,%d] PE_FWI_M14_SetEdgeGainCtrl() error.\n", __F__, __L__);

				#ifdef PE_HW_M14B_USE_REG_CTRL
				#ifdef PE_SHP_HW_M14B_SET_FW_PARAM_TO_REG_DIRECTLY
				if(PE_KDRV_VER_M14BX)
				{
					PE_SHP_HW_M14B_PE1_WR02(pe1_coring_ctrl_09,edge_gain_b,GET_BITS(pp->mp_edge_gain_b,0,7), \
															edge_gain_w,GET_BITS(pp->mp_edge_gain_w,0,7));
					PE_SHP_HW_M14B_PE1_WR02(pe1_dp_ctrl_01,edge_gain_b,GET_BITS(pp->sp_edge_gain_b,0,7), \
														edge_gain_w,GET_BITS(pp->sp_edge_gain_w,0,7));
				}
				#else
				if(PE_KDRV_VER_M14BX)
				{
					PE_SHP_HW_M14B_PE1_SET(pe1_coring_ctrl_09,edge_gain_b,GET_BITS(pp->mp_edge_gain_b,0,7));
					PE_SHP_HW_M14B_PE1_SET(pe1_coring_ctrl_09,edge_gain_w,GET_BITS(pp->mp_edge_gain_w,0,7));
					PE_SHP_HW_M14B_PE1_SET(pe1_dp_ctrl_01,edge_gain_b,GET_BITS(pp->sp_edge_gain_b,0,7));
					PE_SHP_HW_M14B_PE1_SET(pe1_dp_ctrl_01,edge_gain_w,GET_BITS(pp->sp_edge_gain_w,0,7));
				}
				#endif
				#else
				#ifdef PE_SHP_HW_M14B_SET_FW_PARAM_TO_REG_DIRECTLY
				if(PE_KDRV_VER_M14BX)
				{
					PE_PE1_M14B0_QWr02(pe1_coring_ctrl_09,edge_gain_b,GET_BITS(pp->mp_edge_gain_b,0,7), \
														edge_gain_w,GET_BITS(pp->mp_edge_gain_w,0,7));
					PE_PE1_M14B0_QWr02(pe1_dp_ctrl_01,edge_gain_b,GET_BITS(pp->sp_edge_gain_b,0,7), \
													edge_gain_w,GET_BITS(pp->sp_edge_gain_w,0,7));
				}
				#endif
				#endif
			}
		}
		else
		{
			PE_SHP_HW_M14_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * get edge enhance control
 * - use input struct LX_PE_SHP_EE1_T for M14
 *
 * @param   *pstParams [in/out] void
 * @return  OK if success, ERROR otherwise.
 * @see		function description
 * @author
 */
int PE_SHP_HW_M14_GetEdgeEnhanceCtrl(void *pstParams)
{
	int ret = RET_OK;
	LX_PE_WIN_ID win_id;
	do {
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_M14)
		{
			LX_PE_SHP_EE1_T *pp=(LX_PE_SHP_EE1_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			win_id = PE_GET_CHECKED_WINID(pp->win_id);
			if(PE_CHECK_WIN0(win_id))
			{
				PE_FWI_M14_SETTINGS_T fw_inf;
				ret = PE_FWI_M14_GetCurFwiSettings(&fw_inf);
				PE_SHP_HW_M14_CHECK_CODE(ret, break, \
					"[%s,%d] PE_FWI_M14_GetCurFwiSettings() error.\n", __F__, __L__);
				PE_SHP_HW_M14_DBG_PRINT("fw_ctrl.enable : %s.\n", \
					fw_inf.fw_ctrl.enable? "yes":"no");
				if(PE_KDRV_VER_M14BX)
				{
					#ifdef PE_SHP_HW_M14B_SET_FW_PARAM_TO_REG_DIRECTLY
					PE_PE1_M14B0_RdFL(pe1_coring_ctrl_09);
					PE_PE1_M14B0_RdFL(pe1_dp_ctrl_01);
					PE_PE1_M14B0_Rd01(pe1_coring_ctrl_09,	edge_gain_b,	pp->mp_edge_gain_b);
					PE_PE1_M14B0_Rd01(pe1_coring_ctrl_09,	edge_gain_w,	pp->mp_edge_gain_w);
					PE_PE1_M14B0_Rd01(pe1_dp_ctrl_01,		edge_gain_b,	pp->sp_edge_gain_b);
					PE_PE1_M14B0_Rd01(pe1_dp_ctrl_01,		edge_gain_w,	pp->sp_edge_gain_w);
					#else
					if(fw_inf.fw_ctrl.enable)
					{
						pp->mp_edge_gain_b = fw_inf.shp_e_ctrl.co09_gain_b;
						pp->mp_edge_gain_w = fw_inf.shp_e_ctrl.co09_gain_w;
						pp->sp_edge_gain_b = fw_inf.shp_e_ctrl.dp01_gain_b;
						pp->sp_edge_gain_w = fw_inf.shp_e_ctrl.dp01_gain_w;
					}
					else
					{
						PE_PE1_M14B0_RdFL(pe1_coring_ctrl_09);
						PE_PE1_M14B0_RdFL(pe1_dp_ctrl_01);
						PE_PE1_M14B0_Rd01(pe1_coring_ctrl_09,	edge_gain_b,	pp->mp_edge_gain_b);
						PE_PE1_M14B0_Rd01(pe1_coring_ctrl_09,	edge_gain_w,	pp->mp_edge_gain_w);
						PE_PE1_M14B0_Rd01(pe1_dp_ctrl_01,		edge_gain_b,	pp->sp_edge_gain_b);
						PE_PE1_M14B0_Rd01(pe1_dp_ctrl_01,		edge_gain_w,	pp->sp_edge_gain_w);
					}
					#endif
				}
				else if(PE_KDRV_VER_M14AX)
				{
					if(fw_inf.fw_ctrl.enable)
					{
						pp->mp_edge_gain_b = fw_inf.shp_e_ctrl.co09_gain_b;
						pp->mp_edge_gain_w = fw_inf.shp_e_ctrl.co09_gain_w;
						pp->sp_edge_gain_b = fw_inf.shp_e_ctrl.dp01_gain_b;
						pp->sp_edge_gain_w = fw_inf.shp_e_ctrl.dp01_gain_w;
					}
					else
					{
						PE_P1L_M14A0_RdFL(pe1_coring_ctrl_09);
						PE_P1L_M14A0_RdFL(pe1_dp_ctrl_01);
						PE_P1L_M14A0_Rd01(pe1_coring_ctrl_09,	edge_gain_b,	pp->mp_edge_gain_b);
						PE_P1L_M14A0_Rd01(pe1_coring_ctrl_09,	edge_gain_w,	pp->mp_edge_gain_w);
						PE_P1L_M14A0_Rd01(pe1_dp_ctrl_01,		edge_gain_b,	pp->sp_edge_gain_b);
						PE_P1L_M14A0_Rd01(pe1_dp_ctrl_01,		edge_gain_w,	pp->sp_edge_gain_w);
					}
				}
			}
			PE_SHP_HW_M14_DBG_PRINT("get[%d]: mp g_b,w:%d,%d, sp g_b,w:%d,%d\n",\
				pp->win_id,pp->mp_edge_gain_b,pp->mp_edge_gain_w,\
				pp->sp_edge_gain_b,pp->sp_edge_gain_w);
		}
		else
		{
			PE_SHP_HW_M14_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * set detail enhance control
 * - use input struct LX_PE_SHP_DE1_T for M14
 *
 * @param   *pstParams [in] void
 * @return  OK if success, ERROR otherwise.
 * @see		function description
 * @author
 */
int PE_SHP_HW_M14_SetDetailEnhanceCtrl(void *pstParams)
{
	int ret = RET_OK;
	do {
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_M14)
		{
			LX_PE_SHP_DE1_T *pp=(LX_PE_SHP_DE1_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			PE_SHP_HW_M14_DBG_PRINT("set[%d]: mp g_b,w:%d,%d, sp g_b,w:%d,%d\n",\
				pp->win_id,pp->mp_texture_gain_b,pp->mp_texture_gain_w,\
				pp->sp_texture_gain_b,pp->sp_texture_gain_w);
			if(PE_CHECK_WIN0(pp->win_id))
			{
				PE_FWI_M14_TEXTURE_GAIN fw_param;
				fw_param.co09_gain_b = GET_BITS(pp->mp_texture_gain_b,0,7);
				fw_param.co09_gain_w = GET_BITS(pp->mp_texture_gain_w,0,7);
				fw_param.dp01_gain_b = GET_BITS(pp->sp_texture_gain_b,0,7);
				fw_param.dp01_gain_w = GET_BITS(pp->sp_texture_gain_w,0,7);
				ret = PE_FWI_M14_SetTextureGainCtrl(&fw_param);
				PE_SHP_HW_M14_CHECK_CODE(ret, break, \
					"[%s,%d] PE_FWI_M14_SetTextureGainCtrl() error.\n", __F__, __L__);

				#ifdef PE_HW_M14B_USE_REG_CTRL
				#ifdef PE_SHP_HW_M14B_SET_FW_PARAM_TO_REG_DIRECTLY
				if(PE_KDRV_VER_M14BX)
				{
					PE_SHP_HW_M14B_PE1_WR02(pe1_coring_ctrl_09,texture_gain_b,GET_BITS(pp->mp_texture_gain_b,0,7), \
															texture_gain_w,GET_BITS(pp->mp_texture_gain_w,0,7));
					PE_SHP_HW_M14B_PE1_WR02(pe1_dp_ctrl_01,texture_gain_b,GET_BITS(pp->sp_texture_gain_b,0,7), \
															texture_gain_w,GET_BITS(pp->sp_texture_gain_w,0,7));
				}
				#else
				if(PE_KDRV_VER_M14BX)
				{
					PE_SHP_HW_M14B_PE1_SET(pe1_coring_ctrl_09,texture_gain_b,GET_BITS(pp->mp_texture_gain_b,0,7));
					PE_SHP_HW_M14B_PE1_SET(pe1_coring_ctrl_09,texture_gain_w,GET_BITS(pp->mp_texture_gain_w,0,7));
					PE_SHP_HW_M14B_PE1_SET(pe1_dp_ctrl_01,texture_gain_b,GET_BITS(pp->sp_texture_gain_b,0,7));
					PE_SHP_HW_M14B_PE1_SET(pe1_dp_ctrl_01,texture_gain_w,GET_BITS(pp->sp_texture_gain_w,0,7));
				}
				#endif
				#else
				#ifdef PE_SHP_HW_M14B_SET_FW_PARAM_TO_REG_DIRECTLY
				if(PE_KDRV_VER_M14BX)
				{
					PE_PE1_M14B0_QWr02(pe1_coring_ctrl_09,texture_gain_b,GET_BITS(pp->mp_texture_gain_b,0,7), \
														texture_gain_w,GET_BITS(pp->mp_texture_gain_w,0,7));
					PE_PE1_M14B0_QWr02(pe1_dp_ctrl_01,texture_gain_b,GET_BITS(pp->sp_texture_gain_b,0,7), \
													texture_gain_w,GET_BITS(pp->sp_texture_gain_w,0,7));
				}
				#endif
				#endif
			}
		}
		else
		{
			PE_SHP_HW_M14_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * get detail enhance control
 * - use input struct LX_PE_SHP_DE1_T for M14
 *
 * @param   *pstParams [in/out] void
 * @return  OK if success, ERROR otherwise.
 * @see		function description
 * @author
 */
int PE_SHP_HW_M14_GetDetailEnhanceCtrl(void *pstParams)
{
	int ret = RET_OK;
	LX_PE_WIN_ID win_id;
	do {
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_M14)
		{
			LX_PE_SHP_DE1_T *pp=(LX_PE_SHP_DE1_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			win_id = PE_GET_CHECKED_WINID(pp->win_id);
			if(PE_CHECK_WIN0(win_id))
			{
				PE_FWI_M14_SETTINGS_T fw_inf;
				ret = PE_FWI_M14_GetCurFwiSettings(&fw_inf);
				PE_SHP_HW_M14_CHECK_CODE(ret, break, \
					"[%s,%d] PE_FWI_M14_GetCurFwiSettings() error.\n", __F__, __L__);
				PE_SHP_HW_M14_DBG_PRINT("fw_ctrl.enable : %s.\n", \
					fw_inf.fw_ctrl.enable? "yes":"no");
				if(PE_KDRV_VER_M14BX)
				{
					#ifdef PE_SHP_HW_M14B_SET_FW_PARAM_TO_REG_DIRECTLY
					PE_PE1_M14B0_RdFL(pe1_coring_ctrl_09);
					PE_PE1_M14B0_RdFL(pe1_dp_ctrl_01);
					PE_PE1_M14B0_Rd01(pe1_coring_ctrl_09,texture_gain_b,pp->mp_texture_gain_b);
					PE_PE1_M14B0_Rd01(pe1_coring_ctrl_09,texture_gain_w,pp->mp_texture_gain_w);
					PE_PE1_M14B0_Rd01(pe1_dp_ctrl_01,	texture_gain_b,	pp->sp_texture_gain_b);
					PE_PE1_M14B0_Rd01(pe1_dp_ctrl_01,	texture_gain_w,	pp->sp_texture_gain_w);
					#else
					if(fw_inf.fw_ctrl.enable)
					{
						pp->mp_texture_gain_b = fw_inf.shp_t_ctrl.co09_gain_b;
						pp->mp_texture_gain_w = fw_inf.shp_t_ctrl.co09_gain_w;
						pp->sp_texture_gain_b = fw_inf.shp_t_ctrl.dp01_gain_b;
						pp->sp_texture_gain_w = fw_inf.shp_t_ctrl.dp01_gain_w;
					}
					else
					{
						PE_PE1_M14B0_RdFL(pe1_coring_ctrl_09);
						PE_PE1_M14B0_RdFL(pe1_dp_ctrl_01);
						PE_PE1_M14B0_Rd01(pe1_coring_ctrl_09,texture_gain_b,pp->mp_texture_gain_b);
						PE_PE1_M14B0_Rd01(pe1_coring_ctrl_09,texture_gain_w,pp->mp_texture_gain_w);
						PE_PE1_M14B0_Rd01(pe1_dp_ctrl_01,	texture_gain_b,	pp->sp_texture_gain_b);
						PE_PE1_M14B0_Rd01(pe1_dp_ctrl_01,	texture_gain_w,	pp->sp_texture_gain_w);
					}
					#endif
				}
				else if(PE_KDRV_VER_M14AX)
				{
					if(fw_inf.fw_ctrl.enable)
					{
						pp->mp_texture_gain_b = fw_inf.shp_t_ctrl.co09_gain_b;
						pp->mp_texture_gain_w = fw_inf.shp_t_ctrl.co09_gain_w;
						pp->sp_texture_gain_b = fw_inf.shp_t_ctrl.dp01_gain_b;
						pp->sp_texture_gain_w = fw_inf.shp_t_ctrl.dp01_gain_w;
					}
					else
					{
						PE_P1L_M14A0_RdFL(pe1_coring_ctrl_09);
						PE_P1L_M14A0_RdFL(pe1_dp_ctrl_01);
						PE_P1L_M14A0_Rd01(pe1_coring_ctrl_09,texture_gain_b,pp->mp_texture_gain_b);
						PE_P1L_M14A0_Rd01(pe1_coring_ctrl_09,texture_gain_w,pp->mp_texture_gain_w);
						PE_P1L_M14A0_Rd01(pe1_dp_ctrl_01,	texture_gain_b,	pp->sp_texture_gain_b);
						PE_P1L_M14A0_Rd01(pe1_dp_ctrl_01,	texture_gain_w,	pp->sp_texture_gain_w);
					}
				}
			}
			PE_SHP_HW_M14_DBG_PRINT("get[%d]: mp g_b,w:%d,%d, sp g_b,w:%d,%d\n",\
				pp->win_id,pp->mp_texture_gain_b,pp->mp_texture_gain_w,\
				pp->sp_texture_gain_b,pp->sp_texture_gain_w);
		}
		else
		{
			PE_SHP_HW_M14_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}

/**
 * set super resolution enhancement(SRE) control
 * - use input struct LX_PE_SHP_SR0_T for M14BX
 *
 * @param   *pstParams [in] void
 * @return  OK if success, ERROR otherwise.
 * @see		function description
 * @author
 */
int PE_SHP_HW_M14_SetSreCtrl(void *pstParams)
{
	int ret = RET_OK;
	do {
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_M14BX)
		{
			LX_PE_SHP_SR0_T *pp=(LX_PE_SHP_SR0_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			PE_SHP_HW_M14_DBG_PRINT("set[%d] : \n"\
				" out_mux_sel,sc_gs_sra,sc_gs_srb    : %d, %d, %d\n"\
				" sc_ls_lv_sra,sc_ls_lv_srb,mode_wei : 0x%08x, 0x%08x, %d\n", \
				pp->win_id, pp->reg_out_mux_sel, pp->reg_sc_gs_sra, \
				pp->reg_sc_gs_srb, pp->reg_sc_ls_lv_sra, \
				pp->reg_sc_ls_lv_srb, pp->reg_mode_wei);
			if(PE_CHECK_WIN0(pp->win_id))
			{
				#ifdef PE_HW_M14B_USE_REG_CTRL
				PE_SHP_HW_M14B_PE1_WR01(pe1_sre_ctrl_00,reg_out_mux_sel,GET_BITS(pp->reg_out_mux_sel,0,8));
				PE_SHP_HW_M14B_PE1_WR02(pe1_sre_ctrl_06,reg_sc_gs_sra,	GET_BITS(pp->reg_sc_gs_sra,0,7), \
														reg_sc_gs_srb,	GET_BITS(pp->reg_sc_gs_srb,0,7));
				PE_SHP_HW_M14B_PE1_WR01(pe1_sre_ctrl_07,reg_sc_ls_lv_sra,pp->reg_sc_ls_lv_sra);
				PE_SHP_HW_M14B_PE1_WR01(pe1_sre_ctrl_08,reg_sc_ls_lv_srb,pp->reg_sc_ls_lv_srb);
				PE_SHP_HW_M14B_PE1_WR01(pe1_sre_ctrl_11,reg_mode_wei,	GET_BITS(pp->reg_mode_wei,0,3));
				#else
				PE_PE1_M14B0_QWr01(pe1_sre_ctrl_00,	reg_out_mux_sel,GET_BITS(pp->reg_out_mux_sel,0,8));
				PE_PE1_M14B0_QWr02(pe1_sre_ctrl_06,	reg_sc_gs_sra,	GET_BITS(pp->reg_sc_gs_sra,0,7), \
													reg_sc_gs_srb,	GET_BITS(pp->reg_sc_gs_srb,0,7));
				PE_PE1_M14B0_QWr01(pe1_sre_ctrl_07,	reg_sc_ls_lv_sra,pp->reg_sc_ls_lv_sra);
				PE_PE1_M14B0_QWr01(pe1_sre_ctrl_08,	reg_sc_ls_lv_srb,pp->reg_sc_ls_lv_srb);
				PE_PE1_M14B0_QWr01(pe1_sre_ctrl_11,	reg_mode_wei,	GET_BITS(pp->reg_mode_wei,0,3));
				#endif
			}
		}
		else
		{
			PE_SHP_HW_M14_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	} while (0);
	return ret;
}
/**
 * get super resolution enhancement(SRE) control
 * - use input struct LX_PE_SHP_SR0_T for M14BX
 *
 * @param   *pstParams [in/out] void
 * @return  OK if success, ERROR otherwise.
 * @see		function description
 * @author
 */
int PE_SHP_HW_M14_GetSreCtrl(void *pstParams)
{
	int ret = RET_OK;
	LX_PE_WIN_ID win_id;
	do {
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_M14BX)
		{
			LX_PE_SHP_SR0_T *pp=(LX_PE_SHP_SR0_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			win_id = PE_GET_CHECKED_WINID(pp->win_id);
			if(PE_CHECK_WIN0(win_id))
			{
				PE_PE1_M14B0_RdFL(pe1_sre_ctrl_00);
				PE_PE1_M14B0_Rd01(pe1_sre_ctrl_00,	reg_out_mux_sel,	pp->reg_out_mux_sel);
				PE_PE1_M14B0_RdFL(pe1_sre_ctrl_06);
				PE_PE1_M14B0_Rd01(pe1_sre_ctrl_06,	reg_sc_gs_sra,		pp->reg_sc_gs_sra);
				PE_PE1_M14B0_Rd01(pe1_sre_ctrl_06,	reg_sc_gs_srb,		pp->reg_sc_gs_srb);
				PE_PE1_M14B0_RdFL(pe1_sre_ctrl_07);
				PE_PE1_M14B0_Rd01(pe1_sre_ctrl_07,	reg_sc_ls_lv_sra,	pp->reg_sc_ls_lv_sra);
				PE_PE1_M14B0_RdFL(pe1_sre_ctrl_08);
				PE_PE1_M14B0_Rd01(pe1_sre_ctrl_08,	reg_sc_ls_lv_srb,	pp->reg_sc_ls_lv_srb);
				PE_PE1_M14B0_RdFL(pe1_sre_ctrl_11);
				PE_PE1_M14B0_Rd01(pe1_sre_ctrl_11,	reg_mode_wei,		pp->reg_mode_wei);
			}
			PE_SHP_HW_M14_DBG_PRINT("get[%d] : \n"\
				" out_mux_sel,sc_gs_sra,sc_gs_srb    : %d, %d, %d\n"\
				" sc_ls_lv_sra,sc_ls_lv_srb,mode_wei : %d, %d, %d\n", \
				pp->win_id, pp->reg_out_mux_sel, pp->reg_sc_gs_sra, \
				pp->reg_sc_gs_srb, pp->reg_sc_ls_lv_sra, \
				pp->reg_sc_ls_lv_srb, pp->reg_mode_wei);
		}
		else
		{
			PE_SHP_HW_M14_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}

/**
 * set vfc(v filter c) apply
 *
 * @param   *pCfg [in] PE_TSK_M14_CFG_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_SHP_HW_M14B_SetVfcApply(PE_TSK_M14_CFG_T *pCfg)
{
	int ret = RET_OK;
	#ifdef PE_HW_M14B_USE_REG_CTRL
	static UINT32 count = 0;
	UINT32 reg_data = 0x0, apply_algo_case = 0x0;
	LX_PE_INF_DISPLAY_T *disp0_info = &pCfg->inf.disp_info[LX_PE_WIN_0];
	PE_INF_M14_CTRL_T *ctrl0_info = &pCfg->inf.ctrl_info[LX_PE_WIN_0];
	PE_SHP_HW_M14_SETTINGS_T *pInfo = &_g_pe_shp_hw_m14_info;
	PE_M14B0_MSC_CHROMA_TEAR_CTRL_T *dft_msc_chroma_tear_ctrl = NULL;	//m14b
	PE_M14B0_MSC_CHROMA_TEAR_CTRL_T *cur_msc_chroma_tear_ctrl = NULL;	//m14b
	do{
		CHECK_KNULL(pCfg);
		if(PE_KDRV_VER_M14BX)
		{
			dft_msc_chroma_tear_ctrl = (PE_M14B0_MSC_CHROMA_TEAR_CTRL_T *)&(pInfo->dft_msc_chroma_tear_ctrl);
			cur_msc_chroma_tear_ctrl = (PE_M14B0_MSC_CHROMA_TEAR_CTRL_T *)&(pInfo->cur_msc_chroma_tear_ctrl);
			/* read current vfc reg. */
			PE_MSC_M14B0_RdFL(msc_chroma_tear_ctrl);
			reg_data = PE_MSC_M14B0_Rd(msc_chroma_tear_ctrl);
			/* check vfilter c pq algorithm case : cvbs/scart+pal/nt(or dtv_hdd+av/scartrgb) */
			apply_algo_case = (((disp0_info->src_type==LX_PE_SRC_CVBS || disp0_info->src_type==LX_PE_SRC_SCART) && \
				(disp0_info->cstd_type==LX_PE_CSTD_NTSC || disp0_info->cstd_type==LX_PE_CSTD_PAL)) || \
				(disp0_info->src_type==LX_PE_SRC_DTV && disp0_info->dtv_type==LX_PE_DTV_HDDPLAY && \
				(disp0_info->hdd_type==LX_PE_HDD_SRC_AV60 || disp0_info->hdd_type==LX_PE_HDD_SRC_AV50 || \
				disp0_info->hdd_type==LX_PE_HDD_SRC_SCARTRGB)))? 1:0;
			/* copy default value */
			pInfo->cur_msc_chroma_tear_ctrl = pInfo->dft_msc_chroma_tear_ctrl;
			/* apply algo */
			if ((apply_algo_case==1) && ctrl0_info->sat_status>PE_SHP_HW_M14B_VFC_SAT_STATUS_TH)
			{
				cur_msc_chroma_tear_ctrl->auto_chroma_filter_en = 0x1;
				cur_msc_chroma_tear_ctrl->manual_chroma_pre_filter_on = 0x0;
				cur_msc_chroma_tear_ctrl->manual_chroma_median_filter_on = 0x0;
			}
			/* compare pre vs. set */
			if (reg_data != pInfo->cur_msc_chroma_tear_ctrl)
			{
				PE_SHP_HW_M14_DBG_PRINT("[inf]src:%d,cstd:%d,dtv:%d,hdd:%d,s_st:%d,algo:%d\n",  \
					disp0_info->src_type, disp0_info->cstd_type, disp0_info->dtv_type, \
					disp0_info->hdd_type, ctrl0_info->sat_status,  apply_algo_case);
				PE_SHP_HW_M14_DBG_PRINT("[set]c_flt(0x3878) 0x%08x->0x%08x\n",  \
					reg_data, pInfo->cur_msc_chroma_tear_ctrl);
				/* wr reg. */
				PE_MSC_M14B0_Wr(msc_chroma_tear_ctrl, pInfo->cur_msc_chroma_tear_ctrl);
				PE_MSC_M14B0_WrFL(msc_chroma_tear_ctrl);
				/* udpate reg. mask */
				PE_SHP_HW_M14B_MSC_SET(msc_chroma_tear_ctrl, auto_chroma_filter_en, \
					cur_msc_chroma_tear_ctrl->auto_chroma_filter_en);
				PE_SHP_HW_M14B_MSC_SET(msc_chroma_tear_ctrl, manual_chroma_pre_filter_on, \
					cur_msc_chroma_tear_ctrl->manual_chroma_pre_filter_on);
				PE_SHP_HW_M14B_MSC_SET(msc_chroma_tear_ctrl, manual_chroma_median_filter_on, \
					cur_msc_chroma_tear_ctrl->manual_chroma_median_filter_on);
			}
			if(_g_shp_hw_m14_trace)
			{
				if(count>PE_PRINT_COUNT_NUM)
				{
					PE_SHP_HW_M14_DBG_PRINT("[state]src:%d,cstd:%d,dtv:%d,hdd:%d,s_st:%d,algo:%d\n",  \
						disp0_info->src_type,disp0_info->cstd_type,disp0_info->dtv_type,\
						disp0_info->hdd_type,ctrl0_info->sat_status, apply_algo_case);
					PE_SHP_HW_M14_DBG_PRINT("[state]c_flt(0x3878) 0x%08x\n", \
						pInfo->cur_msc_chroma_tear_ctrl);
					count=0;
				}
				count++;
			}
		}
		else
		{
			PE_SHP_HW_M14_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	#else
	PE_SHP_HW_M14_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
	#endif
	return ret;
}

