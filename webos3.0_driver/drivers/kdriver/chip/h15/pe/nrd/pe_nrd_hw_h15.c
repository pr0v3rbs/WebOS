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

/** @file pe_nrd_hw_h15.c
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

#include "pe_hw_h15.h"
#include "pe_reg_h15.h"
#include "pe_fwi_h15.h"
#include "pe_cmn_hw_h15.h"
#include "pe_nrd_hw_h15.h"
#include "pe_tnr_hw_param_h15.h"
#include "pe_dnr_hw_param_h15.h"
#include "pe_ifc_hw_param_h15.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
/* 352,288 = 704/2,576/2 */
#define PE_NRD_HW_H15_H_MIN                (352)
#define PE_NRD_HW_H15_V_MIN                (288)
#define PE_NRD_HW_H15_TNR_SRC_H_HD_BASE    (800)
#define PE_NRD_HW_H15_TNR_SRC_V_3K_BASE    (1600)
#define PE_NRD_HW_H15_C_FMT_444            (0x6)
#define PE_NRD_HW_H15_C_FMT_422            (0x5)
#define PE_NRD_HW_H15_C_FMT_420            (0x4)

/* use pre processing for setting tnr reg table */
#define PE_NRD_HW_H15A_USE_PRE_PROC_TNR_REG_TABLE

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define PE_NRD_HW_H15_ERROR		PE_PRINT_ERROR

#define PE_NRD_HW_H15_DBG_PRINT(fmt,args...)	\
	if(_g_nrd_hw_h15_trace)\
	{\
		printk("[%x,%x][%s,%d] "fmt, \
			PE_CHIP_VER, g_pe_kdrv_ver_mask, __F__, __L__, ##args);\
	}

#define PE_NRD_HW_H15_CHECK_CODE(_checker,_action,fmt,args...)	\
	{if(_checker){PE_NRD_HW_H15_ERROR(fmt,##args);_action;}}

#define PE_NRD_HW_H15A_DBG_PRINT_TNR_FMT_STATE(_pre, _cur)	\
	{\
		char _str_buf[PE_TRACE_STR_SIZE];\
		sprintf(_str_buf, "%s(%s)->(%s):%s", #_cur, \
			PE_NRD_HW_H15A_TNR_FMT_TO_STR(_pre), \
			PE_NRD_HW_H15A_TNR_FMT_TO_STR(_cur), \
			((_pre)!=(_cur))? "different":"same");\
		PE_NRD_HW_H15_DBG_PRINT("%s\n", _str_buf);\
		if((_pre)!=(_cur))\
		{\
			PE_INF_H15_SetTraceDataBuf(_str_buf);\
		}\
	}
/* tnr lut (tnl) */
#define PE_NRD_HW_H15A_DBG_PRINT_TNL_FMT_STATE(_pre, _cur)	\
	{\
		char _str_buf[PE_TRACE_STR_SIZE];\
		sprintf(_str_buf, "%s(%s)->(%s):%s", #_cur, \
			PE_NRD_HW_H15A_TNL_FMT_TO_STR(_pre), \
			PE_NRD_HW_H15A_TNL_FMT_TO_STR(_cur), \
			((_pre)!=(_cur))? "different":"same");\
		PE_NRD_HW_H15_DBG_PRINT("%s\n", _str_buf);\
		if((_pre)!=(_cur))\
		{\
			PE_INF_H15_SetTraceDataBuf(_str_buf);\
		}\
	}
#define PE_NRD_HW_H15A_DBG_PRINT_DNR_FMT_STATE(_pre, _cur)	\
	{\
		char _str_buf[PE_TRACE_STR_SIZE];\
		sprintf(_str_buf, "%s(%s)->(%s):%s", #_cur, \
			PE_NRD_HW_H15A_DNR_FMT_TO_STR(_pre), \
			PE_NRD_HW_H15A_DNR_FMT_TO_STR(_cur), \
			((_pre)!=(_cur))? "different":"same");\
		PE_NRD_HW_H15_DBG_PRINT("%s\n", _str_buf);\
		if((_pre)!=(_cur))\
		{\
			PE_INF_H15_SetTraceDataBuf(_str_buf);\
		}\
	}
#define PE_NRD_HW_H15A_DBG_PRINT_IFC_FMT_STATE(_pre, _cur)	\
	{\
		char _str_buf[PE_TRACE_STR_SIZE];\
		sprintf(_str_buf, "%s(%s)->(%s):%s", #_cur, \
			PE_NRD_HW_H15A_IFC_FMT_TO_STR(_pre), \
			PE_NRD_HW_H15A_IFC_FMT_TO_STR(_cur), \
			((_pre)!=(_cur))? "different":"same");\
		PE_NRD_HW_H15_DBG_PRINT("%s\n", _str_buf);\
		if((_pre)!=(_cur))\
		{\
			PE_INF_H15_SetTraceDataBuf(_str_buf);\
		}\
	}

#ifdef PE_HW_H15_PROTECT_OVERLAP_SETTINGS
#define PE_NRD_HW_H15A_TNR1_SET_USER(_r, _f, _d)	\
	{	_g_pe_tnr_hw_param_data_h15a.nd1_user.data->_r._f = (_d);\
		_g_pe_tnr_hw_param_data_h15a.nd1_mask.data->_r._f = 0;}
#define PE_NRD_HW_H15A_TNR2_SET_USER(_r, _f, _d)	\
	{	_g_pe_tnr_hw_param_data_h15a.nd2_user.data->_r._f = (_d);\
		_g_pe_tnr_hw_param_data_h15a.nd2_mask.data->_r._f = 0;}
#define PE_NRD_HW_H15A_TNR12_SET_USER(_r, _f, _d)	\
	{	_g_pe_tnr_hw_param_data_h15a.nd1_user.data->_r._f = (_d);\
		_g_pe_tnr_hw_param_data_h15a.nd1_mask.data->_r._f = 0;\
		_g_pe_tnr_hw_param_data_h15a.nd2_user.data->_r._f = (_d);\
		_g_pe_tnr_hw_param_data_h15a.nd2_mask.data->_r._f = 0;}

#define PE_NRD_HW_H15A_TNR1_WR01(_r, _f1, _d1)	\
	{	PE_ND_ND1_H15A0_QWr01(_r, _f1, _d1);\
		PE_NRD_HW_H15A_TNR1_SET_USER(_r, _f1, _d1);}
#define PE_NRD_HW_H15A_TNR1_WR02(_r, _f1, _d1, _f2, _d2)	\
	{	PE_ND_ND1_H15A0_QWr02(_r, _f1, _d1, _f2, _d2);\
		PE_NRD_HW_H15A_TNR1_SET_USER(_r, _f1, _d1);\
		PE_NRD_HW_H15A_TNR1_SET_USER(_r, _f2, _d2);}
#define PE_NRD_HW_H15A_TNR1_WR03(_r, _f1, _d1, _f2, _d2, _f3, _d3)	\
	{	PE_ND_ND1_H15A0_QWr03(_r, _f1, _d1, _f2, _d2, _f3, _d3);\
		PE_NRD_HW_H15A_TNR1_SET_USER(_r, _f1, _d1);\
		PE_NRD_HW_H15A_TNR1_SET_USER(_r, _f2, _d2);\
		PE_NRD_HW_H15A_TNR1_SET_USER(_r, _f3, _d3);}
#define PE_NRD_HW_H15A_TNR1_WR04(_r, _f1, _d1, _f2, _d2, _f3, _d3, _f4, _d4)	\
	{	PE_ND_ND1_H15A0_QWr04(_r, _f1, _d1, _f2, _d2, _f3, _d3, _f4, _d4);\
		PE_NRD_HW_H15A_TNR1_SET_USER(_r, _f1, _d1);\
		PE_NRD_HW_H15A_TNR1_SET_USER(_r, _f2, _d2);\
		PE_NRD_HW_H15A_TNR1_SET_USER(_r, _f3, _d3);\
		PE_NRD_HW_H15A_TNR1_SET_USER(_r, _f4, _d4);}
#define PE_NRD_HW_H15A_TNR1_WR05(_r, _f1, _d1, _f2, _d2, _f3, _d3, _f4, _d4, _f5, _d5)	\
	{	PE_ND_ND1_H15A0_QWr05(_r, _f1, _d1, _f2, _d2, _f3, _d3, _f4, _d4, _f5, _d5);\
		PE_NRD_HW_H15A_TNR1_SET_USER(_r, _f1, _d1);\
		PE_NRD_HW_H15A_TNR1_SET_USER(_r, _f2, _d2);\
		PE_NRD_HW_H15A_TNR1_SET_USER(_r, _f3, _d3);\
		PE_NRD_HW_H15A_TNR1_SET_USER(_r, _f4, _d4);\
		PE_NRD_HW_H15A_TNR1_SET_USER(_r, _f5, _d5);}

#define PE_NRD_HW_H15A_TNR2_WR01(_r, _f1, _d1)	\
	{	PE_ND_ND2_H15A0_QWr01(_r, _f1, _d1);\
		PE_NRD_HW_H15A_TNR2_SET_USER(_r, _f1, _d1);}
#define PE_NRD_HW_H15A_TNR2_WR02(_r, _f1, _d1, _f2, _d2)	\
	{	PE_ND_ND2_H15A0_QWr02(_r, _f1, _d1, _f2, _d2);\
		PE_NRD_HW_H15A_TNR2_SET_USER(_r, _f1, _d1);\
		PE_NRD_HW_H15A_TNR2_SET_USER(_r, _f2, _d2);}
#define PE_NRD_HW_H15A_TNR2_WR03(_r, _f1, _d1, _f2, _d2, _f3, _d3)	\
	{	PE_ND_ND2_H15A0_QWr03(_r, _f1, _d1, _f2, _d2, _f3, _d3);\
		PE_NRD_HW_H15A_TNR2_SET_USER(_r, _f1, _d1);\
		PE_NRD_HW_H15A_TNR2_SET_USER(_r, _f2, _d2);\
		PE_NRD_HW_H15A_TNR2_SET_USER(_r, _f3, _d3);}
#define PE_NRD_HW_H15A_TNR2_WR04(_r, _f1, _d1, _f2, _d2, _f3, _d3, _f4, _d4)	\
	{	PE_ND_ND2_H15A0_QWr04(_r, _f1, _d1, _f2, _d2, _f3, _d3, _f4, _d4);\
		PE_NRD_HW_H15A_TNR2_SET_USER(_r, _f1, _d1);\
		PE_NRD_HW_H15A_TNR2_SET_USER(_r, _f2, _d2);\
		PE_NRD_HW_H15A_TNR2_SET_USER(_r, _f3, _d3);\
		PE_NRD_HW_H15A_TNR2_SET_USER(_r, _f4, _d4);}
#define PE_NRD_HW_H15A_TNR2_WR05(_r, _f1, _d1, _f2, _d2, _f3, _d3, _f4, _d4, _f5, _d5)	\
	{	PE_ND_ND2_H15A0_QWr05(_r, _f1, _d1, _f2, _d2, _f3, _d3, _f4, _d4, _f5, _d5);\
		PE_NRD_HW_H15A_TNR2_SET_USER(_r, _f1, _d1);\
		PE_NRD_HW_H15A_TNR2_SET_USER(_r, _f2, _d2);\
		PE_NRD_HW_H15A_TNR2_SET_USER(_r, _f3, _d3);\
		PE_NRD_HW_H15A_TNR2_SET_USER(_r, _f4, _d4);\
		PE_NRD_HW_H15A_TNR2_SET_USER(_r, _f5, _d5);}

#define PE_NRD_HW_H15A_TNR12_WR01(_r, _f1, _d1)	\
	{	PE_ND12_H15A0_QWr01(_r, _f1, _d1);\
		PE_NRD_HW_H15A_TNR1_SET_USER(_r, _f1, _d1);\
		PE_NRD_HW_H15A_TNR2_SET_USER(_r, _f1, _d1);}
#define PE_NRD_HW_H15A_TNR12_WR02(_r, _f1, _d1, _f2, _d2)	\
	{	PE_ND12_H15A0_QWr02(_r, _f1, _d1, _f2, _d2);\
		PE_NRD_HW_H15A_TNR1_SET_USER(_r, _f1, _d1);\
		PE_NRD_HW_H15A_TNR2_SET_USER(_r, _f1, _d1);\
		PE_NRD_HW_H15A_TNR1_SET_USER(_r, _f2, _d2);\
		PE_NRD_HW_H15A_TNR2_SET_USER(_r, _f2, _d2);}
#define PE_NRD_HW_H15A_TNR12_WR03(_r, _f1, _d1, _f2, _d2, _f3, _d3)	\
	{	PE_ND12_H15A0_QWr03(_r, _f1, _d1, _f2, _d2, _f3, _d3);\
		PE_NRD_HW_H15A_TNR1_SET_USER(_r, _f1, _d1);\
		PE_NRD_HW_H15A_TNR2_SET_USER(_r, _f1, _d1);\
		PE_NRD_HW_H15A_TNR1_SET_USER(_r, _f2, _d2);\
		PE_NRD_HW_H15A_TNR2_SET_USER(_r, _f2, _d2);\
		PE_NRD_HW_H15A_TNR1_SET_USER(_r, _f3, _d3);\
		PE_NRD_HW_H15A_TNR2_SET_USER(_r, _f3, _d3);}
#define PE_NRD_HW_H15A_TNR12_WR04(_r, _f1, _d1, _f2, _d2, _f3, _d3, _f4, _d4)	\
	{	PE_ND12_H15A0_QWr04(_r, _f1, _d1, _f2, _d2, _f3, _d3, _f4, _d4);\
		PE_NRD_HW_H15A_TNR1_SET_USER(_r, _f1, _d1);\
		PE_NRD_HW_H15A_TNR2_SET_USER(_r, _f1, _d1);\
		PE_NRD_HW_H15A_TNR1_SET_USER(_r, _f2, _d2);\
		PE_NRD_HW_H15A_TNR2_SET_USER(_r, _f2, _d2);\
		PE_NRD_HW_H15A_TNR1_SET_USER(_r, _f3, _d3);\
		PE_NRD_HW_H15A_TNR2_SET_USER(_r, _f3, _d3);\
		PE_NRD_HW_H15A_TNR1_SET_USER(_r, _f4, _d4);\
		PE_NRD_HW_H15A_TNR2_SET_USER(_r, _f4, _d4);}
#define PE_NRD_HW_H15A_TNR12_WR05(_r, _f1, _d1, _f2, _d2, _f3, _d3, _f4, _d4, _f5, _d5)	\
	{	PE_ND12_H15A0_QWr05(_r, _f1, _d1, _f2, _d2, _f3, _d3, _f4, _d4, _f5, _d5);\
		PE_NRD_HW_H15A_TNR1_SET_USER(_r, _f1, _d1);\
		PE_NRD_HW_H15A_TNR2_SET_USER(_r, _f1, _d1);\
		PE_NRD_HW_H15A_TNR1_SET_USER(_r, _f2, _d2);\
		PE_NRD_HW_H15A_TNR2_SET_USER(_r, _f2, _d2);\
		PE_NRD_HW_H15A_TNR1_SET_USER(_r, _f3, _d3);\
		PE_NRD_HW_H15A_TNR2_SET_USER(_r, _f3, _d3);\
		PE_NRD_HW_H15A_TNR1_SET_USER(_r, _f4, _d4);\
		PE_NRD_HW_H15A_TNR2_SET_USER(_r, _f4, _d4);\
		PE_NRD_HW_H15A_TNR1_SET_USER(_r, _f5, _d5);\
		PE_NRD_HW_H15A_TNR2_SET_USER(_r, _f5, _d5);}

/* tnr lut (tnl) */
#define PE_NRD_HW_H15A_TNL_SET_USER32(_r, _d)	\
	{	*((UINT32 *)&(_g_pe_tnl_hw_param_data_h15a.reg_user.data->_r)) = (_d);\
		*((UINT32 *)&(_g_pe_tnl_hw_param_data_h15a.reg_mask.data->_r)) = 0;}

#define PE_NRD_HW_H15A_TNL_WR32(_num, _d1)	\
	{	PE_ND12_H15A0_QWr(tnr_main_lut_01, _d1);\
		PE_NRD_HW_H15A_TNL_SET_USER32(tnr_lut_##_num, _d1);}

/* dnr */
#define PE_NRD_HW_H15A_DNR_SET_USER(_r, _f, _d)	\
	{	_g_pe_dnr_hw_param_data_h15a.reg_user.data->_r._f = (_d);\
		_g_pe_dnr_hw_param_data_h15a.reg_mask.data->_r._f = 0;}

#define PE_NRD_HW_H15A_DNR_WR01(_r, _f1, _d1)	\
	{	PE_DNR12_H15A0_QWr01(_r, _f1, _d1);\
		PE_NRD_HW_H15A_DNR_SET_USER(_r, _f1, _d1);}
#define PE_NRD_HW_H15A_DNR_WR02(_r, _f1, _d1, _f2, _d2)	\
	{	PE_DNR12_H15A0_QWr02(_r, _f1, _d1, _f2, _d2);\
		PE_NRD_HW_H15A_DNR_SET_USER(_r, _f1, _d1);\
		PE_NRD_HW_H15A_DNR_SET_USER(_r, _f2, _d2);}
#define PE_NRD_HW_H15A_DNR_WR03(_r, _f1, _d1, _f2, _d2, _f3, _d3)	\
	{	PE_DNR12_H15A0_QWr03(_r, _f1, _d1, _f2, _d2, _f3, _d3);\
		PE_NRD_HW_H15A_DNR_SET_USER(_r, _f1, _d1);\
		PE_NRD_HW_H15A_DNR_SET_USER(_r, _f2, _d2);\
		PE_NRD_HW_H15A_DNR_SET_USER(_r, _f3, _d3);}
#define PE_NRD_HW_H15A_DNR_WR04(_r, _f1, _d1, _f2, _d2, _f3, _d3, _f4, _d4)	\
	{	PE_DNR12_H15A0_QWr04(_r, _f1, _d1, _f2, _d2, _f3, _d3, _f4, _d4);\
		PE_NRD_HW_H15A_DNR_SET_USER(_r, _f1, _d1);\
		PE_NRD_HW_H15A_DNR_SET_USER(_r, _f2, _d2);\
		PE_NRD_HW_H15A_DNR_SET_USER(_r, _f3, _d3);\
		PE_NRD_HW_H15A_DNR_SET_USER(_r, _f4, _d4);}
#define PE_NRD_HW_H15A_DNR_WR05(_r, _f1, _d1, _f2, _d2, _f3, _d3, _f4, _d4, _f5, _d5)	\
	{	PE_DNR12_H15A0_QWr05(_r, _f1, _d1, _f2, _d2, _f3, _d3, _f4, _d4, _f5, _d5);\
		PE_NRD_HW_H15A_DNR_SET_USER(_r, _f1, _d1);\
		PE_NRD_HW_H15A_DNR_SET_USER(_r, _f2, _d2);\
		PE_NRD_HW_H15A_DNR_SET_USER(_r, _f3, _d3);\
		PE_NRD_HW_H15A_DNR_SET_USER(_r, _f4, _d4);\
		PE_NRD_HW_H15A_DNR_SET_USER(_r, _f5, _d5);}

#define PE_NRD_HW_H15A_IFC_SET_USER(_r, _f, _d)	\
	{	_g_pe_ifc_hw_param_data_h15a.reg_user.data->_r._f = (_d);\
		_g_pe_ifc_hw_param_data_h15a.reg_mask.data->_r._f = 0;}

#define PE_NRD_HW_H15A_IFC_WR01(_r, _f1, _d1)	\
	{	PE_DNR1_H15A0_QWr01(_r, _f1, _d1);\
		PE_NRD_HW_H15A_IFC_SET_USER(_r, _f1, _d1);}
#define PE_NRD_HW_H15A_IFC_WR02(_r, _f1, _d1, _f2, _d2)	\
	{	PE_DNR1_H15A0_QWr02(_r, _f1, _d1, _f2, _d2);\
		PE_NRD_HW_H15A_IFC_SET_USER(_r, _f1, _d1);\
		PE_NRD_HW_H15A_IFC_SET_USER(_r, _f2, _d2);}
#define PE_NRD_HW_H15A_IFC_WR03(_r, _f1, _d1, _f2, _d2, _f3, _d3)	\
	{	PE_DNR1_H15A0_QWr03(_r, _f1, _d1, _f2, _d2, _f3, _d3);\
		PE_NRD_HW_H15A_IFC_SET_USER(_r, _f1, _d1);\
		PE_NRD_HW_H15A_IFC_SET_USER(_r, _f2, _d2);\
		PE_NRD_HW_H15A_IFC_SET_USER(_r, _f3, _d3);}
#define PE_NRD_HW_H15A_IFC_WR04(_r, _f1, _d1, _f2, _d2, _f3, _d3, _f4, _d4)	\
	{	PE_DNR1_H15A0_QWr04(_r, _f1, _d1, _f2, _d2, _f3, _d3, _f4, _d4);\
		PE_NRD_HW_H15A_IFC_SET_USER(_r, _f1, _d1);\
		PE_NRD_HW_H15A_IFC_SET_USER(_r, _f2, _d2);\
		PE_NRD_HW_H15A_IFC_SET_USER(_r, _f3, _d3);\
		PE_NRD_HW_H15A_IFC_SET_USER(_r, _f4, _d4);}
#define PE_NRD_HW_H15A_IFC_WR05(_r, _f1, _d1, _f2, _d2, _f3, _d3, _f4, _d4, _f5, _d5)	\
	{	PE_DNR1_H15A0_QWr05(_r, _f1, _d1, _f2, _d2, _f3, _d3, _f4, _d4, _f5, _d5);\
		PE_NRD_HW_H15A_IFC_SET_USER(_r, _f1, _d1);\
		PE_NRD_HW_H15A_IFC_SET_USER(_r, _f2, _d2);\
		PE_NRD_HW_H15A_IFC_SET_USER(_r, _f3, _d3);\
		PE_NRD_HW_H15A_IFC_SET_USER(_r, _f4, _d4);\
		PE_NRD_HW_H15A_IFC_SET_USER(_r, _f5, _d5);}

#define PE_NRD_HW_H15A_SET_REG_TABLE(__tbl, __base)	\
	do{\
		UINT32 _i, _size;\
		PE_NRD_HW_H15_CHECK_CODE(!_dflt, ret=RET_ERROR;break, \
			"[%s,%d] _dflt is null, not ready.\n", __F__, __L__);\
		PE_NRD_HW_H15_CHECK_CODE(!_user, ret=RET_ERROR;break, \
			"[%s,%d] _user is null, not ready.\n", __F__, __L__);\
		PE_NRD_HW_H15_CHECK_CODE(!_mask, ret=RET_ERROR;break, \
			"[%s,%d] _mask is null, not ready.\n", __F__, __L__);\
		PE_NRD_HW_H15_CHECK_CODE(!_data, ret=RET_ERROR;break, \
			"[%s,%d] _data is null, not ready.\n", __F__, __L__);\
		_size = sizeof(__tbl)/sizeof(PE_REG_PARAM_T);\
		for (_i=0; _i<_size; _i++)\
		{\
			if ((__base) == PE_H15A_REG_WRITE_BASE)\
			{\
				_dflt[_i] = __tbl[_i].data;\
			}\
			_data[_i] = (__tbl[_i].data & _mask[_i])|(_user[_i]);\
			ret = PE_REG_H15_WrAllocatedAddr((__tbl[_i].addr+(__base)), _data[_i]);\
			PE_NRD_HW_H15_CHECK_CODE(ret, break, \
				"[%s,%d] PE_REG_H15_WrAllocatedAddr() error.\n", __F__, __L__);\
		}\
		if (ret == RET_OK)\
		{\
			PE_NRD_HW_H15_DBG_PRINT("write %s(+ 0x%08x).\n", #__tbl, (__base));\
			ret = PE_REG_SetTraceTable(__F__,__L__, #__tbl, (__base));\
		}\
	}while(0)

/* pre proc tnr table, not to write some reg,bit */
#ifdef PE_NRD_HW_H15A_USE_PRE_PROC_TNR_REG_TABLE
#define PE_NRD_HW_H15A_PRE_PROC_TNR_REG_TABLE(__base)	\
	do{\
		UINT32 _rd_data;\
		if ((__base) == PE_H15A_REG_WRITE_BASE)\
		{\
			PE_ND_ND1_H15A0_RdFL(tnr_ctrl_00);\
			PE_ND_ND1_H15A0_Rd01(tnr_ctrl_00, reg_tnr_en, _rd_data);\
			PE_NRD_HW_H15A_TNR1_SET_USER(tnr_ctrl_00, reg_tnr_en, _rd_data);\
		}\
		else\
		{\
			PE_ND_ND2_H15A0_RdFL(tnr_ctrl_00);\
			PE_ND_ND2_H15A0_Rd01(tnr_ctrl_00, reg_tnr_en, _rd_data);\
			PE_NRD_HW_H15A_TNR2_SET_USER(tnr_ctrl_00, reg_tnr_en, _rd_data);\
		}\
	}while(0)
#else
#define PE_NRD_HW_H15A_PRE_PROC_TNR_REG_TABLE(__base)
#endif

/* tnr */
#define PE_NRD_HW_H15A_TNR_REG_TABLE(__tbl, __base)		\
	do{\
		UINT32 *_dflt = _g_pe_tnr_hw_param_data_h15a.reg_dflt.addr;\
		UINT32 *_user = NULL;\
		UINT32 *_mask = NULL;\
		UINT32 *_data = NULL;\
		if ((__base) == PE_H15A_REG_WRITE_BASE)\
		{\
			_user = _g_pe_tnr_hw_param_data_h15a.nd1_user.addr;\
			_mask = _g_pe_tnr_hw_param_data_h15a.nd1_mask.addr;\
			_data = _g_pe_tnr_hw_param_data_h15a.nd1_data.addr;\
		}\
		else\
		{\
			_user = _g_pe_tnr_hw_param_data_h15a.nd2_user.addr;\
			_mask = _g_pe_tnr_hw_param_data_h15a.nd2_mask.addr;\
			_data = _g_pe_tnr_hw_param_data_h15a.nd2_data.addr;\
		}\
		PE_NRD_HW_H15A_PRE_PROC_TNR_REG_TABLE(__base);\
		PE_NRD_HW_H15A_SET_REG_TABLE(__tbl, __base);\
	}while (0)

/* tnr lut (tnl) */
#define PE_NRD_HW_H15A_TNL_REG_TABLE(__tbl, __base)		\
	do{\
		UINT32 *_dflt = _g_pe_tnl_hw_param_data_h15a.reg_dflt.addr;\
		UINT32 *_user = _g_pe_tnl_hw_param_data_h15a.reg_user.addr;\
		UINT32 *_mask = _g_pe_tnl_hw_param_data_h15a.reg_mask.addr;\
		UINT32 *_data = _g_pe_tnl_hw_param_data_h15a.reg_data.addr;\
		PE_NRD_HW_H15A_SET_REG_TABLE(__tbl, __base);\
	}while (0)

/* dnr */
#define PE_NRD_HW_H15A_DNR_REG_TABLE(__tbl, __base)		\
	do{\
		UINT32 *_dflt = _g_pe_dnr_hw_param_data_h15a.reg_dflt.addr;\
		UINT32 *_user = _g_pe_dnr_hw_param_data_h15a.reg_user.addr;\
		UINT32 *_mask = _g_pe_dnr_hw_param_data_h15a.reg_mask.addr;\
		UINT32 *_data = _g_pe_dnr_hw_param_data_h15a.reg_data.addr;\
		PE_NRD_HW_H15A_SET_REG_TABLE(__tbl, __base);\
	}while (0)

/* ifc */
#define PE_NRD_HW_H15A_IFC_REG_TABLE(__tbl, __base)		\
	do{\
		UINT32 *_dflt = _g_pe_ifc_hw_param_data_h15a.reg_dflt.addr;\
		UINT32 *_user = _g_pe_ifc_hw_param_data_h15a.reg_user.addr;\
		UINT32 *_mask = _g_pe_ifc_hw_param_data_h15a.reg_mask.addr;\
		UINT32 *_data = _g_pe_ifc_hw_param_data_h15a.reg_data.addr;\
		PE_NRD_HW_H15A_SET_REG_TABLE(__tbl, __base);\
	}while (0)

#endif

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/

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
static PE_NRD_HW_H15A_TNR_FMT PE_NRD_HW_H15A_ConvDispInfoToTnrFmt(LX_PE_INF_DISPLAY_T *disp_inf);
static PE_NRD_HW_H15A_TNL_FMT PE_NRD_HW_H15A_ConvDispInfoToTnlFmt(LX_PE_INF_DISPLAY_T *disp_inf);
static PE_NRD_HW_H15A_IFC_FMT PE_NRD_HW_H15A_ConvDispInfoToIfcFmt(LX_PE_INF_DISPLAY_T *disp_inf);
static PE_NRD_HW_H15A_DNR_FMT PE_NRD_HW_H15A_ConvDispInfoToDnrFmt(LX_PE_INF_DISPLAY_T *disp_inf);
static int PE_NRD_HW_H15A_SetDefault(LX_PE_DEFAULT_SETTINGS_T *pstParams);
static int PE_NRD_HW_H15_SetTnrLbMode(PE_TSK_H15_CFG_T *pCfg);
static int PE_NRD_HW_H15_SetTnrSmoothMode(PE_TSK_H15_CFG_T *pCfg);
static int PE_NRD_HW_H15_SetTnrScMode(PE_TSK_H15_CFG_T *pCfg);
static int PE_NRD_HW_H15_SetDnrMaxMode(PE_TSK_H15_CFG_T *pCfg);
static int PE_NRD_HW_H15_SetBnrMode(PE_TSK_H15_CFG_T *pCfg);
static int PE_NRD_HW_H15_DownloadTnrUserDb(void *pstParams);
#ifdef PE_HW_H15_PROTECT_OVERLAP_SETTINGS
static int PE_NRD_HW_H15A_CreateDataTable(UINT32 index);
#endif

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
static UINT8 _g_nrd_hw_h15_trace=0x0;	//default should be off.
static PE_NRD_HW_H15_SETTINGS_T _g_pe_nrd_hw_h15_info;
#ifdef PE_HW_H15_PROTECT_OVERLAP_SETTINGS
static PE_TNR_HW_PARAM_DATA_H15A0_T _g_pe_tnr_hw_param_data_h15a = {{NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL}};
static PE_TNL_HW_PARAM_DATA_H15A0_T _g_pe_tnl_hw_param_data_h15a = {{NULL}, {NULL}, {NULL}, {NULL}};
static PE_DNR_HW_PARAM_DATA_H15A0_T _g_pe_dnr_hw_param_data_h15a = {{NULL}, {NULL}, {NULL}, {NULL}};
static PE_IFC_HW_PARAM_DATA_H15A0_T _g_pe_ifc_hw_param_data_h15a = {{NULL}, {NULL}, {NULL}, {NULL}};
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
int PE_NRD_HW_H15_Init(PE_CFG_CTRL_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H15)
		{
			_g_pe_nrd_hw_h15_info.use_manual_sc = 1;
			_g_pe_nrd_hw_h15_info.tnr_sc_en_user = 0;
			_g_pe_nrd_hw_h15_info.tnr_sc_en_mask = 0;
			_g_pe_nrd_hw_h15_info.tnr_pv_smooth_en_user = 0;
			_g_pe_nrd_hw_h15_info.tnr_pv_smooth_en_mask = 0;
			_g_pe_nrd_hw_h15_info.dnr_max_en0_user = 0;
			_g_pe_nrd_hw_h15_info.dnr_max_en0_mask = 0;
			_g_pe_nrd_hw_h15_info.bnr_ac_h_en0_user = 0;
			_g_pe_nrd_hw_h15_info.bnr_ac_h_en0_mask = 0;
			_g_pe_nrd_hw_h15_info.cur_tnr_auto_enable = 0;
			_g_pe_nrd_hw_h15_info.cur_tnr_master_gain = 0xc0;	// sync with dflt
			_g_pe_nrd_hw_h15_info.tnr0a_fmt = PE_NRD_H15A_TNR_NUM;
			_g_pe_nrd_hw_h15_info.tnl0a_fmt = PE_NRD_H15A_TNL_NUM;
			_g_pe_nrd_hw_h15_info.dnr0a_fmt = PE_NRD_H15A_DNR_NUM;
			_g_pe_nrd_hw_h15_info.ifc0a_fmt = PE_NRD_H15A_IFC_NUM;
			#ifdef PE_HW_H15_PROTECT_OVERLAP_SETTINGS
			/* init tnr(nd1,2) */
			ret = PE_NRD_HW_H15A_CreateDataTable(PE_NRD_HW_H15A_TBL_IDX_TNR);
			PE_NRD_HW_H15_CHECK_CODE(ret, break, \
				"PE_NRD_HW_H15A_CreateDataTable() error.\n");
			PE_NRD_HW_H15A_TNR_REG_TABLE(tnr_l_init_h15a0,PE_H15A_REG_WRITE_BASE);
			PE_NRD_HW_H15A_TNR_REG_TABLE(tnr_l_init_h15a0,PE_H15A_ND_ND_1_2_DIFF);
			/* init tnr lut (nd1,2) */
			ret = PE_NRD_HW_H15A_CreateDataTable(PE_NRD_HW_H15A_TBL_IDX_TNL);
			PE_NRD_HW_H15_CHECK_CODE(ret, break, \
				"PE_NRD_HW_H15A_CreateDataTable() error.\n");
			PE_ND12_H15A0_QWr(tnr_main_lut_00, 0x00001000);
			PE_NRD_HW_H15A_TNL_REG_TABLE(tnr_l_lut_init_h15a0,PE_H15A_REG_WRITE_BASE);
			PE_NRD_HW_H15A_TNL_REG_TABLE(tnr_l_lut_init_h15a0,PE_H15A_ND_ND_1_2_DIFF);
			PE_ND12_H15A0_QWr(tnr_main_lut_00, 0x00008000);
			/* init dnr(nd1,2) */
			ret = PE_NRD_HW_H15A_CreateDataTable(PE_NRD_HW_H15A_TBL_IDX_DNR);
			PE_NRD_HW_H15_CHECK_CODE(ret, break, \
				"PE_NRD_HW_H15A_CreateDataTable() error.\n");
			PE_NRD_HW_H15A_DNR_REG_TABLE(dnr_l_init_h15a0,PE_H15A_REG_WRITE_BASE);
			PE_NRD_HW_H15A_DNR_REG_TABLE(dnr_l_init_h15a0,PE_H15A_ND_DNR_12_DIFF);
			/* init ifc(nd1) */
			ret = PE_NRD_HW_H15A_CreateDataTable(PE_NRD_HW_H15A_TBL_IDX_IFC);
			PE_NRD_HW_H15_CHECK_CODE(ret, break, \
				"PE_NRD_HW_H15A_CreateDataTable() error.\n");
			PE_NRD_HW_H15A_IFC_REG_TABLE(ifc_l_init_h15a0,PE_H15A_REG_WRITE_BASE);
			#else
			PE_SET_REG_TABLE(NRD_HW_H15,tnr_l_init_h15a0,PE_H15A_REG_WRITE_BASE);
			PE_SET_REG_TABLE(NRD_HW_H15,tnr_l_init_h15a0,PE_H15A_ND_ND_1_2_DIFF);
			PE_SET_REG_TABLE(NRD_HW_H15,dnr_l_init_h15a0,PE_H15A_REG_WRITE_BASE);
			PE_SET_REG_TABLE(NRD_HW_H15,dnr_l_init_h15a0,PE_H15A_ND_DNR_12_DIFF);
			PE_SET_REG_TABLE(NRD_HW_H15,ifc_l_init_h15a0,PE_H15A_REG_WRITE_BASE);
			#endif
			/* tnr sc enable, threshold */
			PE_CE_PE1_H15A0_RdFL(pe1_fsw_ctrl_01);
			PE_CE_PE1_H15A0_Wr01(pe1_fsw_ctrl_01,tnr_sc_measure_en, _g_pe_nrd_hw_h15_info.use_manual_sc? 1:0);
			PE_CE_PE1_H15A0_Wr01(pe1_fsw_ctrl_01,tnr_sc_measure_th, 0x170);//368
			PE_CE_PE1_H15A0_WrFL(pe1_fsw_ctrl_01);
			/* tnr enable */
			ret = PE_FWI_H15_SetFwTnrEnable(TRUE);
			PE_NRD_HW_H15_CHECK_CODE(ret, break, \
				"[%s,%d] PE_FWI_H15_SetFwTnrEnable() error.\n",__F__,__L__);
		}
		else
		{
			PE_NRD_HW_H15_DBG_PRINT("nothing to do\n");
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
int PE_NRD_HW_H15_SetDebugSettings(LX_PE_DBG_SETTINGS_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		PE_NRD_HW_H15_DBG_PRINT("wid:%d,type:0x%x,[0x%x]print:0x%x,bypass:0x%x\n",\
			pstParams->win_id,pstParams->type,(0x1<<PE_ITEM_PKTMODL(NRD)),\
			pstParams->print_lvl,pstParams->bypass);
		/* set print level */
		if(pstParams->type&LX_PE_DBG_LV)
		{
			_g_nrd_hw_h15_trace = (pstParams->print_lvl & (0x1<<PE_ITEM_PKTMODL(NRD)))? 0x1:0x0;
		}
		/* set bypass */
		if(pstParams->type&LX_PE_DBG_BY)
		{
			if(PE_KDRV_VER_H15)
			{
				if(pstParams->bypass & (0x1<<PE_ITEM_PKTMODL(NRD)))
				{
					PE_NRD_HW_H15_DBG_PRINT("tnr,dnr : disable.\n");
					if(PE_CHECK_WIN0(pstParams->win_id))
					{
						PE_ND12_H15A0_QWr01(tnr_ctrl_00,reg_tnr_en,0x0);
						PE_DNR12_H15A0_QWr01(dnr_max_ctrl,reg_dnr_max_enable,0x0);
					}
					if(PE_CHECK_WIN1(pstParams->win_id))
					{
						PE_ND_NDS_H15A0_QWr01(tnr_ctrl_00,reg_tnr_en,0x0);
					}
				}
				else
				{
					PE_NRD_HW_H15_DBG_PRINT("tnr,dnr : enable.\n");
					if(PE_CHECK_WIN0(pstParams->win_id))
					{
						PE_ND12_H15A0_QWr01(tnr_ctrl_00,reg_tnr_en,0x1);
						PE_DNR12_H15A0_QWr01(dnr_max_ctrl,reg_dnr_max_enable,0x1);
					}
					if(PE_CHECK_WIN1(pstParams->win_id))
					{
						PE_ND_NDS_H15A0_QWr01(tnr_ctrl_00,reg_tnr_en,0x1);
					}
				}
			}
			else
			{
				PE_NRD_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
			}
		}
	}while(0);
	return ret;
}
#ifdef PE_HW_H15_PROTECT_OVERLAP_SETTINGS
/**
 * create data table
 *
 * @param   index [in] UINT32
 * @return  OK if success, ERROR otherwise.
 * @see		PE_NRD_HW_H15A_TBL_INDX
 * @author
 */
static int PE_NRD_HW_H15A_CreateDataTable(UINT32 index)
{
	int ret = RET_OK;
	PE_TNR_HW_PARAM_DATA_H15A0_T *p_tnr_h15a = &_g_pe_tnr_hw_param_data_h15a;
	PE_TNL_HW_PARAM_DATA_H15A0_T *p_tnl_h15a = &_g_pe_tnl_hw_param_data_h15a;
	PE_DNR_HW_PARAM_DATA_H15A0_T *p_dnr_h15a = &_g_pe_dnr_hw_param_data_h15a;
	PE_IFC_HW_PARAM_DATA_H15A0_T *p_ifc_h15a = &_g_pe_ifc_hw_param_data_h15a;

	PE_NRD_HW_H15_CHECK_CODE(index>=PE_NRD_HW_H15A_TBL_IDX_NUM, return RET_ERROR, \
		"[%s,%d] index(%d) invalid.\n", __F__, __L__, index);

	if (index == PE_NRD_HW_H15A_TBL_IDX_TNR)
	{
		/* create dflt table */
		if (p_tnr_h15a->reg_dflt.addr == NULL)
		{
			p_tnr_h15a->reg_dflt.addr = (UINT32 *)OS_KMalloc(sizeof(PE_TNR_HW_PARAM_REG_H15A0_T));
		}
		/* create user table */
		if (p_tnr_h15a->nd1_user.addr == NULL)
		{
			p_tnr_h15a->nd1_user.addr = (UINT32 *)OS_KMalloc(sizeof(PE_TNR_HW_PARAM_REG_H15A0_T));
			memset((p_tnr_h15a->nd1_user.addr), 0, sizeof(PE_TNR_HW_PARAM_REG_H15A0_T));	// 0x0
		}
		/* create mask table */
		if (p_tnr_h15a->nd1_mask.addr == NULL)
		{
			p_tnr_h15a->nd1_mask.addr = (UINT32 *)OS_KMalloc(sizeof(PE_TNR_HW_PARAM_REG_H15A0_T));
			memset((p_tnr_h15a->nd1_mask.addr), -1, sizeof(PE_TNR_HW_PARAM_REG_H15A0_T));// 0xffffffff
		}
		/* create data table */
		if (p_tnr_h15a->nd1_data.addr == NULL)
		{
			p_tnr_h15a->nd1_data.addr = (UINT32 *)OS_KMalloc(sizeof(PE_TNR_HW_PARAM_REG_H15A0_T));
			memset((p_tnr_h15a->nd1_data.addr), 0, sizeof(PE_TNR_HW_PARAM_REG_H15A0_T));	// 0x0
		}
		/* create user table */
		if (p_tnr_h15a->nd2_user.addr == NULL)
		{
			p_tnr_h15a->nd2_user.addr = (UINT32 *)OS_KMalloc(sizeof(PE_TNR_HW_PARAM_REG_H15A0_T));
			memset((p_tnr_h15a->nd2_user.addr), 0, sizeof(PE_TNR_HW_PARAM_REG_H15A0_T));	// 0x0
		}
		/* create mask table */
		if (p_tnr_h15a->nd2_mask.addr == NULL)
		{
			p_tnr_h15a->nd2_mask.addr = (UINT32 *)OS_KMalloc(sizeof(PE_TNR_HW_PARAM_REG_H15A0_T));
			memset((p_tnr_h15a->nd2_mask.addr), -1, sizeof(PE_TNR_HW_PARAM_REG_H15A0_T));// 0xffffffff
		}
		/* create data table */
		if (p_tnr_h15a->nd2_data.addr == NULL)
		{
			p_tnr_h15a->nd2_data.addr = (UINT32 *)OS_KMalloc(sizeof(PE_TNR_HW_PARAM_REG_H15A0_T));
			memset((p_tnr_h15a->nd2_data.addr), 0, sizeof(PE_TNR_HW_PARAM_REG_H15A0_T));	// 0x0
		}
		/* report result */
		if (p_tnr_h15a->reg_dflt.addr != NULL && p_tnr_h15a->nd1_user.addr != NULL && \
			p_tnr_h15a->nd1_mask.addr != NULL && p_tnr_h15a->nd1_data.addr != NULL && \
			p_tnr_h15a->nd2_user.addr != NULL && p_tnr_h15a->nd2_mask.addr != NULL && \
			p_tnr_h15a->nd2_data.addr != NULL)
		{
			PE_PRINT_NOTI("[%s]table ready.\n", PE_NRD_HW_H15A_TBL_IDX_TO_STR(index));
			ret = RET_OK;
		}
		else
		{
			PE_PRINT_NOTI("[%s]table not ready. dflt,user,mask,data:%s,%s,%s,%s,%s,%s,%s\n", \
				PE_NRD_HW_H15A_TBL_IDX_TO_STR(index), \
				(p_tnr_h15a->reg_dflt.addr == NULL)? "x":"o", \
				(p_tnr_h15a->nd1_user.addr == NULL)? "x":"o", \
				(p_tnr_h15a->nd1_mask.addr == NULL)? "x":"o", \
				(p_tnr_h15a->nd1_data.addr == NULL)? "x":"o", \
				(p_tnr_h15a->nd2_user.addr == NULL)? "x":"o", \
				(p_tnr_h15a->nd2_mask.addr == NULL)? "x":"o", \
				(p_tnr_h15a->nd2_data.addr == NULL)? "x":"o");
			ret = RET_ERROR;
		}
	}
	else if (index == PE_NRD_HW_H15A_TBL_IDX_TNL)
	{
		/* create dflt table */
		if (p_tnl_h15a->reg_dflt.addr == NULL)
		{
			p_tnl_h15a->reg_dflt.addr = (UINT32 *)OS_KMalloc(sizeof(PE_TNL_HW_PARAM_REG_H15A0_T));
		}
		/* create user table */
		if (p_tnl_h15a->reg_user.addr == NULL)
		{
			p_tnl_h15a->reg_user.addr = (UINT32 *)OS_KMalloc(sizeof(PE_TNL_HW_PARAM_REG_H15A0_T));
			memset((p_tnl_h15a->reg_user.addr), 0, sizeof(PE_TNL_HW_PARAM_REG_H15A0_T));	// 0x0
		}
		/* create mask table */
		if (p_tnl_h15a->reg_mask.addr == NULL)
		{
			p_tnl_h15a->reg_mask.addr = (UINT32 *)OS_KMalloc(sizeof(PE_TNL_HW_PARAM_REG_H15A0_T));
			memset((p_tnl_h15a->reg_mask.addr), -1, sizeof(PE_TNL_HW_PARAM_REG_H15A0_T));// 0xffffffff
		}
		/* create data table */
		if (p_tnl_h15a->reg_data.addr == NULL)
		{
			p_tnl_h15a->reg_data.addr = (UINT32 *)OS_KMalloc(sizeof(PE_TNL_HW_PARAM_REG_H15A0_T));
			memset((p_tnl_h15a->reg_data.addr), 0, sizeof(PE_TNL_HW_PARAM_REG_H15A0_T));	// 0x0
		}
		/* report result */
		if (p_tnl_h15a->reg_dflt.addr != NULL && p_tnl_h15a->reg_user.addr != NULL && \
			p_tnl_h15a->reg_mask.addr != NULL && p_tnl_h15a->reg_data.addr != NULL)
		{
			PE_PRINT_NOTI("[%s]table ready.\n", PE_NRD_HW_H15A_TBL_IDX_TO_STR(index));
			ret = RET_OK;
		}
		else
		{
			PE_PRINT_NOTI("[%s]table not ready. dflt,user,mask,data:%s,%s,%s,%s\n", \
				PE_NRD_HW_H15A_TBL_IDX_TO_STR(index), \
				(p_tnl_h15a->reg_dflt.addr == NULL)? "x":"o", \
				(p_tnl_h15a->reg_user.addr == NULL)? "x":"o", \
				(p_tnl_h15a->reg_mask.addr == NULL)? "x":"o", \
				(p_tnl_h15a->reg_data.addr == NULL)? "x":"o");
			ret = RET_ERROR;
		}
	}
	else if (index == PE_NRD_HW_H15A_TBL_IDX_DNR)
	{
		/* create dflt table */
		if (p_dnr_h15a->reg_dflt.addr == NULL)
		{
			p_dnr_h15a->reg_dflt.addr = (UINT32 *)OS_KMalloc(sizeof(PE_DNR_HW_PARAM_REG_H15A0_T));
		}
		/* create user table */
		if (p_dnr_h15a->reg_user.addr == NULL)
		{
			p_dnr_h15a->reg_user.addr = (UINT32 *)OS_KMalloc(sizeof(PE_DNR_HW_PARAM_REG_H15A0_T));
			memset((p_dnr_h15a->reg_user.addr), 0, sizeof(PE_DNR_HW_PARAM_REG_H15A0_T));	// 0x0
		}
		/* create mask table */
		if (p_dnr_h15a->reg_mask.addr == NULL)
		{
			p_dnr_h15a->reg_mask.addr = (UINT32 *)OS_KMalloc(sizeof(PE_DNR_HW_PARAM_REG_H15A0_T));
			memset((p_dnr_h15a->reg_mask.addr), -1, sizeof(PE_DNR_HW_PARAM_REG_H15A0_T));// 0xffffffff
		}
		/* create data table */
		if (p_dnr_h15a->reg_data.addr == NULL)
		{
			p_dnr_h15a->reg_data.addr = (UINT32 *)OS_KMalloc(sizeof(PE_DNR_HW_PARAM_REG_H15A0_T));
			memset((p_dnr_h15a->reg_data.addr), 0, sizeof(PE_DNR_HW_PARAM_REG_H15A0_T));	// 0x0
		}
		/* report result */
		if (p_dnr_h15a->reg_dflt.addr != NULL && p_dnr_h15a->reg_user.addr != NULL && \
			p_dnr_h15a->reg_mask.addr != NULL && p_dnr_h15a->reg_data.addr != NULL)
		{
			PE_PRINT_NOTI("[%s]table ready.\n", PE_NRD_HW_H15A_TBL_IDX_TO_STR(index));
			ret = RET_OK;
		}
		else
		{
			PE_PRINT_NOTI("[%s]table not ready. dflt,user,mask,data:%s,%s,%s,%s\n", \
				PE_NRD_HW_H15A_TBL_IDX_TO_STR(index), \
				(p_dnr_h15a->reg_dflt.addr == NULL)? "x":"o", \
				(p_dnr_h15a->reg_user.addr == NULL)? "x":"o", \
				(p_dnr_h15a->reg_mask.addr == NULL)? "x":"o", \
				(p_dnr_h15a->reg_data.addr == NULL)? "x":"o");
			ret = RET_ERROR;
		}
	}
	else if (index == PE_NRD_HW_H15A_TBL_IDX_IFC)
	{
		/* create dflt table */
		if (p_ifc_h15a->reg_dflt.addr == NULL)
		{
			p_ifc_h15a->reg_dflt.addr = (UINT32 *)OS_KMalloc(sizeof(PE_IFC_HW_PARAM_REG_H15A0_T));
		}
		/* create user table */
		if (p_ifc_h15a->reg_user.addr == NULL)
		{
			p_ifc_h15a->reg_user.addr = (UINT32 *)OS_KMalloc(sizeof(PE_IFC_HW_PARAM_REG_H15A0_T));
			memset((p_ifc_h15a->reg_user.addr), 0, sizeof(PE_IFC_HW_PARAM_REG_H15A0_T));	// 0x0
		}
		/* create mask table */
		if (p_ifc_h15a->reg_mask.addr == NULL)
		{
			p_ifc_h15a->reg_mask.addr = (UINT32 *)OS_KMalloc(sizeof(PE_IFC_HW_PARAM_REG_H15A0_T));
			memset((p_ifc_h15a->reg_mask.addr), -1, sizeof(PE_IFC_HW_PARAM_REG_H15A0_T));// 0xffffffff
		}
		/* create data table */
		if (p_ifc_h15a->reg_data.addr == NULL)
		{
			p_ifc_h15a->reg_data.addr = (UINT32 *)OS_KMalloc(sizeof(PE_IFC_HW_PARAM_REG_H15A0_T));
			memset((p_ifc_h15a->reg_data.addr), 0, sizeof(PE_IFC_HW_PARAM_REG_H15A0_T));	// 0x0
		}
		/* report result */
		if (p_ifc_h15a->reg_dflt.addr != NULL && p_ifc_h15a->reg_user.addr != NULL && \
			p_ifc_h15a->reg_mask.addr != NULL && p_ifc_h15a->reg_data.addr != NULL)
		{
			PE_PRINT_NOTI("[%s]table ready.\n", PE_NRD_HW_H15A_TBL_IDX_TO_STR(index));
			ret = RET_OK;
		}
		else
		{
			PE_PRINT_NOTI("[%s]table not ready. dflt,user,mask,data:%s,%s,%s,%s\n", \
				PE_NRD_HW_H15A_TBL_IDX_TO_STR(index), \
				(p_ifc_h15a->reg_dflt.addr == NULL)? "x":"o", \
				(p_ifc_h15a->reg_user.addr == NULL)? "x":"o", \
				(p_ifc_h15a->reg_mask.addr == NULL)? "x":"o", \
				(p_ifc_h15a->reg_data.addr == NULL)? "x":"o");
			ret = RET_ERROR;
		}
	}
	else
	{
		PE_NRD_HW_H15_DBG_PRINT("nothing to do\n");	ret = RET_OK;
	}
	return ret;
}
#endif
/**
 * set nr default setting
 *
 * @param   *pstParams [in] LX_PE_DEFAULT_SETTINGS_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_NRD_HW_H15_SetDefault(LX_PE_DEFAULT_SETTINGS_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H15)
		{
			ret = PE_NRD_HW_H15A_SetDefault(pstParams);
			PE_NRD_HW_H15_CHECK_CODE(ret, break, \
				"[%s,%d] PE_NRD_HW_H15A_SetDefault() error.\n", __F__, __L__);
		}
		else
		{
			PE_NRD_HW_H15_DBG_PRINT("nothing to do\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * set nr default setting(for H15AX)
 *
 * @param   *pstParams [in] LX_PE_DEFAULT_SETTINGS_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_NRD_HW_H15A_SetDefault(LX_PE_DEFAULT_SETTINGS_T *pstParams)
{
	int ret = RET_OK;
	//PE_FWI_H15_TNR_CTRL fwi_tnr;
	//PE_FWI_H15_SETTINGS_T fwi_inf;
	PE_INF_H15_SETTINGS_T inf_set;
	LX_PE_INF_DISPLAY_T *cur0_inf = NULL;
	PE_NRD_HW_H15A_TNR_FMT cur0_tnr_fmt;
	PE_NRD_HW_H15A_TNL_FMT cur0_tnl_fmt;
	PE_NRD_HW_H15A_DNR_FMT cur0_dnr_fmt;
	PE_NRD_HW_H15A_IFC_FMT cur0_ifc_fmt;
	PE_NRD_HW_H15_SETTINGS_T *pInfo = &_g_pe_nrd_hw_h15_info;

	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		//ret = PE_FWI_H15_GetCurFwiSettings(&fwi_inf);
		//PE_NRD_HW_H15_CHECK_CODE(ret,break,"[%s,%d] PE_FWI_H15_GetCurFwiSettings() error.\n",__F__,__L__);
		ret = PE_INF_H15_GetCurInfSettings(&inf_set);
		PE_NRD_HW_H15_CHECK_CODE(ret,break,"[%s,%d] PE_INF_H15_GetCurInfSettings() error.\n",__F__,__L__);
		cur0_inf=&inf_set.disp_info[LX_PE_WIN_0];
		CHECK_KNULL(cur0_inf);
		/* tnr(nd1,2) */
		if(PE_CHECK_WIN0(pstParams->win_id))
		{
			cur0_tnr_fmt = PE_NRD_HW_H15A_ConvDispInfoToTnrFmt(cur0_inf);
			PE_NRD_HW_H15A_DBG_PRINT_TNR_FMT_STATE(pInfo->tnr0a_fmt,cur0_tnr_fmt);
			if(pInfo->tnr0a_fmt!=cur0_tnr_fmt)
			{
				#ifdef PE_HW_H15_PROTECT_OVERLAP_SETTINGS
				switch(cur0_tnr_fmt)
				{
					case PE_NRD_H15A_TNR_HD:
						PE_NRD_HW_H15A_TNR_REG_TABLE(tnr_l_hd_default_h15a0,   PE_H15A_REG_WRITE_BASE);
						PE_NRD_HW_H15A_TNR_REG_TABLE(tnr_l_hd_default_h15a0,   PE_H15A_ND_ND_1_2_DIFF);
						break;
					case PE_NRD_H15A_TNR_SD:
						PE_NRD_HW_H15A_TNR_REG_TABLE(tnr_l_sd_default_h15a0,   PE_H15A_REG_WRITE_BASE);
						PE_NRD_HW_H15A_TNR_REG_TABLE(tnr_l_sd_default_h15a0,   PE_H15A_ND_ND_1_2_DIFF);
						break;
					case PE_NRD_H15A_TNR_UD:
						PE_NRD_HW_H15A_TNR_REG_TABLE(tnr_l_ud_default_h15a0,   PE_H15A_REG_WRITE_BASE);
						PE_NRD_HW_H15A_TNR_REG_TABLE(tnr_l_ud_default_h15a0,   PE_H15A_ND_ND_1_2_DIFF);
						break;
					case PE_NRD_H15A_TNR_SECAM:
						PE_NRD_HW_H15A_TNR_REG_TABLE(tnr_l_secam_default_h15a0,PE_H15A_REG_WRITE_BASE);
						PE_NRD_HW_H15A_TNR_REG_TABLE(tnr_l_secam_default_h15a0,PE_H15A_ND_ND_1_2_DIFF);
						break;
					case PE_NRD_H15A_TNR_CAM_HD:
						PE_NRD_HW_H15A_TNR_REG_TABLE(tnr_l_cam_hd_default_h15a0,PE_H15A_REG_WRITE_BASE);
						PE_NRD_HW_H15A_TNR_REG_TABLE(tnr_l_cam_hd_default_h15a0,PE_H15A_ND_ND_1_2_DIFF);
						break;
					case PE_NRD_H15A_TNR_ATV:
						PE_NRD_HW_H15A_TNR_REG_TABLE(tnr_l_atv_default_h15a0,   PE_H15A_REG_WRITE_BASE);
						PE_NRD_HW_H15A_TNR_REG_TABLE(tnr_l_atv_default_h15a0,   PE_H15A_ND_ND_1_2_DIFF);
						break;
					default:
						break;
				}
				#else
				switch(cur0_tnr_fmt)
				{
					case PE_NRD_H15A_TNR_HD:
						PE_SET_REG_TABLE(NRD_HW_H15,tnr_l_hd_default_h15a0,   PE_H15A_REG_WRITE_BASE);
						PE_SET_REG_TABLE(NRD_HW_H15,tnr_l_hd_default_h15a0,   PE_H15A_ND_ND_1_2_DIFF);
						break;
					case PE_NRD_H15A_TNR_SD:
						PE_SET_REG_TABLE(NRD_HW_H15,tnr_l_sd_default_h15a0,   PE_H15A_REG_WRITE_BASE);
						PE_SET_REG_TABLE(NRD_HW_H15,tnr_l_sd_default_h15a0,   PE_H15A_ND_ND_1_2_DIFF);
						break;
					case PE_NRD_H15A_TNR_UD:
						PE_SET_REG_TABLE(NRD_HW_H15,tnr_l_ud_default_h15a0,   PE_H15A_REG_WRITE_BASE);
						PE_SET_REG_TABLE(NRD_HW_H15,tnr_l_ud_default_h15a0,   PE_H15A_ND_ND_1_2_DIFF);
						break;
					case PE_NRD_H15A_TNR_SECAM:
						PE_SET_REG_TABLE(NRD_HW_H15,tnr_l_secam_default_h15a0,PE_H15A_REG_WRITE_BASE);
						PE_SET_REG_TABLE(NRD_HW_H15,tnr_l_secam_default_h15a0,PE_H15A_ND_ND_1_2_DIFF);
						break;
					case PE_NRD_H15A_TNR_CAM_HD:
						PE_SET_REG_TABLE(NRD_HW_H15,tnr_l_cam_hd_default_h15a0,PE_H15A_REG_WRITE_BASE);
						PE_SET_REG_TABLE(NRD_HW_H15,tnr_l_cam_hd_default_h15a0,PE_H15A_ND_ND_1_2_DIFF);
						break;
					case PE_NRD_H15A_TNR_ATV:
						PE_SET_REG_TABLE(NRD_HW_H15,tnr_l_atv_default_h15a0,   PE_H15A_REG_WRITE_BASE);
						PE_SET_REG_TABLE(NRD_HW_H15,tnr_l_atv_default_h15a0,   PE_H15A_ND_ND_1_2_DIFF);
						break;
					default:
						break;
				}
				#endif
				pInfo->tnr0a_fmt=cur0_tnr_fmt;
			}
		}
		/* tnr(nds) */
		/* tnr lut (tnl) */
		if(PE_CHECK_WIN0(pstParams->win_id))
		{
			cur0_tnl_fmt = PE_NRD_HW_H15A_ConvDispInfoToTnlFmt(cur0_inf);
			PE_NRD_HW_H15A_DBG_PRINT_TNL_FMT_STATE(pInfo->tnl0a_fmt,cur0_tnl_fmt);
			if(pInfo->tnl0a_fmt!=cur0_tnl_fmt)
			{
				#ifdef PE_HW_H15_PROTECT_OVERLAP_SETTINGS
				switch(cur0_tnl_fmt)
				{
					case PE_NRD_H15A_TNL_HD:
						PE_ND12_H15A0_QWr(tnr_main_lut_00, 0x00001000);
						PE_NRD_HW_H15A_TNL_REG_TABLE(tnr_l_lut_hd_default_h15a0,   PE_H15A_REG_WRITE_BASE);
						PE_NRD_HW_H15A_TNL_REG_TABLE(tnr_l_lut_hd_default_h15a0,   PE_H15A_ND_ND_1_2_DIFF);
						PE_ND12_H15A0_QWr(tnr_main_lut_00, 0x00008000);
						break;
					case PE_NRD_H15A_TNL_SD:
						PE_ND12_H15A0_QWr(tnr_main_lut_00, 0x00001000);
						PE_NRD_HW_H15A_TNL_REG_TABLE(tnr_l_lut_sd_default_h15a0,   PE_H15A_REG_WRITE_BASE);
						PE_NRD_HW_H15A_TNL_REG_TABLE(tnr_l_lut_sd_default_h15a0,   PE_H15A_ND_ND_1_2_DIFF);
						PE_ND12_H15A0_QWr(tnr_main_lut_00, 0x00008000);
						break;
					case PE_NRD_H15A_TNL_UD:
						PE_ND12_H15A0_QWr(tnr_main_lut_00, 0x00001000);
						PE_NRD_HW_H15A_TNL_REG_TABLE(tnr_l_lut_ud_default_h15a0,   PE_H15A_REG_WRITE_BASE);
						PE_NRD_HW_H15A_TNL_REG_TABLE(tnr_l_lut_ud_default_h15a0,   PE_H15A_ND_ND_1_2_DIFF);
						PE_ND12_H15A0_QWr(tnr_main_lut_00, 0x00008000);
						break;
					case PE_NRD_H15A_TNL_CAM_HD:
						PE_ND12_H15A0_QWr(tnr_main_lut_00, 0x00001000);
						PE_NRD_HW_H15A_TNL_REG_TABLE(tnr_l_lut_cam_hd_default_h15a0,PE_H15A_REG_WRITE_BASE);
						PE_NRD_HW_H15A_TNL_REG_TABLE(tnr_l_lut_cam_hd_default_h15a0,PE_H15A_ND_ND_1_2_DIFF);
						PE_ND12_H15A0_QWr(tnr_main_lut_00, 0x00008000);
						break;
					default:
						break;
				}
				#endif
				pInfo->tnl0a_fmt=cur0_tnl_fmt;
			}
		}
		/* dnr(dnr1,2) */
		if(PE_CHECK_WIN0(pstParams->win_id))
		{
			cur0_dnr_fmt = PE_NRD_HW_H15A_ConvDispInfoToDnrFmt(cur0_inf);
			PE_NRD_HW_H15A_DBG_PRINT_DNR_FMT_STATE(pInfo->dnr0a_fmt,cur0_dnr_fmt);
			if(pInfo->dnr0a_fmt!=cur0_dnr_fmt)
			{
				#ifdef PE_HW_H15_PROTECT_OVERLAP_SETTINGS
				switch(cur0_dnr_fmt)
				{
					case PE_NRD_H15A_DNR_ATV_480:
						PE_NRD_HW_H15A_DNR_REG_TABLE(dnr_l_atv_480_default_h15a0,PE_H15A_REG_WRITE_BASE);
						PE_NRD_HW_H15A_DNR_REG_TABLE(dnr_l_atv_480_default_h15a0,PE_H15A_ND_DNR_12_DIFF);
						break;
					case PE_NRD_H15A_DNR_ATV_576:
						PE_NRD_HW_H15A_DNR_REG_TABLE(dnr_l_atv_576_default_h15a0,PE_H15A_REG_WRITE_BASE);
						PE_NRD_HW_H15A_DNR_REG_TABLE(dnr_l_atv_576_default_h15a0,PE_H15A_ND_DNR_12_DIFF);
						break;
					case PE_NRD_H15A_DNR_AV:
						PE_NRD_HW_H15A_DNR_REG_TABLE(dnr_l_av_default_h15a0,     PE_H15A_REG_WRITE_BASE);
						PE_NRD_HW_H15A_DNR_REG_TABLE(dnr_l_av_default_h15a0,     PE_H15A_ND_DNR_12_DIFF);
						break;
					case PE_NRD_H15A_DNR_DTV_SD:
						PE_NRD_HW_H15A_DNR_REG_TABLE(dnr_l_dtv_sd_default_h15a0, PE_H15A_REG_WRITE_BASE);
						PE_NRD_HW_H15A_DNR_REG_TABLE(dnr_l_dtv_sd_default_h15a0, PE_H15A_ND_DNR_12_DIFF);
						break;
					case PE_NRD_H15A_DNR_HD:
						PE_NRD_HW_H15A_DNR_REG_TABLE(dnr_l_hd_default_h15a0,     PE_H15A_REG_WRITE_BASE);
						PE_NRD_HW_H15A_DNR_REG_TABLE(dnr_l_hd_default_h15a0,     PE_H15A_ND_DNR_12_DIFF);
						break;
					case PE_NRD_H15A_DNR_PC:
						PE_NRD_HW_H15A_DNR_REG_TABLE(dnr_l_pc_default_h15a0,     PE_H15A_REG_WRITE_BASE);
						PE_NRD_HW_H15A_DNR_REG_TABLE(dnr_l_pc_default_h15a0,     PE_H15A_ND_DNR_12_DIFF);
						break;
					case PE_NRD_H15A_DNR_SD:
						PE_NRD_HW_H15A_DNR_REG_TABLE(dnr_l_sd_default_h15a0,     PE_H15A_REG_WRITE_BASE);
						PE_NRD_HW_H15A_DNR_REG_TABLE(dnr_l_sd_default_h15a0,     PE_H15A_ND_DNR_12_DIFF);
						break;
					default:
						break;
				}
				#else
				switch(cur0_dnr_fmt)
				{
					case PE_NRD_H15A_DNR_ATV_480:
						PE_SET_REG_TABLE(NRD_HW_H15,dnr_l_atv_480_default_h15a0,PE_H15A_REG_WRITE_BASE);
						PE_SET_REG_TABLE(NRD_HW_H15,dnr_l_atv_480_default_h15a0,PE_H15A_ND_DNR_12_DIFF);
						break;
					case PE_NRD_H15A_DNR_ATV_576:
						PE_SET_REG_TABLE(NRD_HW_H15,dnr_l_atv_576_default_h15a0,PE_H15A_REG_WRITE_BASE);
						PE_SET_REG_TABLE(NRD_HW_H15,dnr_l_atv_576_default_h15a0,PE_H15A_ND_DNR_12_DIFF);
						break;
					case PE_NRD_H15A_DNR_AV:
						PE_SET_REG_TABLE(NRD_HW_H15,dnr_l_av_default_h15a0,     PE_H15A_REG_WRITE_BASE);
						PE_SET_REG_TABLE(NRD_HW_H15,dnr_l_av_default_h15a0,     PE_H15A_ND_DNR_12_DIFF);
						break;
					case PE_NRD_H15A_DNR_DTV_SD:
						PE_SET_REG_TABLE(NRD_HW_H15,dnr_l_dtv_sd_default_h15a0, PE_H15A_REG_WRITE_BASE);
						PE_SET_REG_TABLE(NRD_HW_H15,dnr_l_dtv_sd_default_h15a0, PE_H15A_ND_DNR_12_DIFF);
						break;
					case PE_NRD_H15A_DNR_HD:
						PE_SET_REG_TABLE(NRD_HW_H15,dnr_l_hd_default_h15a0,     PE_H15A_REG_WRITE_BASE);
						PE_SET_REG_TABLE(NRD_HW_H15,dnr_l_hd_default_h15a0,     PE_H15A_ND_DNR_12_DIFF);
						break;
					case PE_NRD_H15A_DNR_PC:
						PE_SET_REG_TABLE(NRD_HW_H15,dnr_l_pc_default_h15a0,     PE_H15A_REG_WRITE_BASE);
						PE_SET_REG_TABLE(NRD_HW_H15,dnr_l_pc_default_h15a0,     PE_H15A_ND_DNR_12_DIFF);
						break;
					case PE_NRD_H15A_DNR_SD:
						PE_SET_REG_TABLE(NRD_HW_H15,dnr_l_sd_default_h15a0,     PE_H15A_REG_WRITE_BASE);
						PE_SET_REG_TABLE(NRD_HW_H15,dnr_l_sd_default_h15a0,     PE_H15A_ND_DNR_12_DIFF);
						break;
					default:
						break;
				}
				#endif
				pInfo->dnr0a_fmt=cur0_dnr_fmt;
			}
		}
		/* ifc(dnr1 only) */
		if(PE_CHECK_WIN0(pstParams->win_id))
		{
			cur0_ifc_fmt = PE_NRD_HW_H15A_ConvDispInfoToIfcFmt(cur0_inf);
			PE_NRD_HW_H15A_DBG_PRINT_IFC_FMT_STATE(pInfo->ifc0a_fmt,cur0_ifc_fmt);
			if(pInfo->ifc0a_fmt!=cur0_ifc_fmt)
			{
				#ifdef PE_HW_H15_PROTECT_OVERLAP_SETTINGS
				switch(cur0_ifc_fmt)
				{
					case PE_NRD_H15A_IFC_ATV_NTSC:
						PE_NRD_HW_H15A_IFC_REG_TABLE(ifc_l_atv_ntsc_default_h15a0,PE_H15A_REG_WRITE_BASE);
						break;
					case PE_NRD_H15A_IFC_PC:
						PE_NRD_HW_H15A_IFC_REG_TABLE(ifc_l_pc_default_h15a0,      PE_H15A_REG_WRITE_BASE);
						break;
					default:
						break;
				}
				#else
				switch(cur0_ifc_fmt)
				{
					case PE_NRD_H15A_IFC_ATV_NTSC:
						PE_SET_REG_TABLE(NRD_HW_H15,ifc_l_atv_ntsc_default_h15a0,PE_H15A_REG_WRITE_BASE);
						break;
					case PE_NRD_H15A_IFC_PC:
						PE_SET_REG_TABLE(NRD_HW_H15,ifc_l_pc_default_h15a0,      PE_H15A_REG_WRITE_BASE);
						break;
					default:
						break;
				}
				#endif
				pInfo->ifc0a_fmt=cur0_ifc_fmt;
			}
		}
	}while(0);
	return ret;
}
/**
 * get tnr fmt from display info.(for H15AX)
 *
 * @param   *disp_inf [in] LX_PE_INF_DISPLAY_T
 * @return  PE_NRD_HW_H15A_TNR_FMT
 * @see
 * @author
 */
static PE_NRD_HW_H15A_TNR_FMT PE_NRD_HW_H15A_ConvDispInfoToTnrFmt(LX_PE_INF_DISPLAY_T *disp_inf)
{
	PE_NRD_HW_H15A_TNR_FMT tnr_fmt = PE_NRD_H15A_TNR_NUM;
	switch(disp_inf->src_type)
	{
		case LX_PE_SRC_ATV:
			if(disp_inf->cstd_type==LX_PE_CSTD_SECAM)
			{
				tnr_fmt = PE_NRD_H15A_TNR_SECAM;
			}
			else
			{
				tnr_fmt = PE_NRD_H15A_TNR_ATV;
			}
			break;
		case LX_PE_SRC_CVBS:
		case LX_PE_SRC_SCART:
			if(disp_inf->cstd_type==LX_PE_CSTD_SECAM)
			{
				tnr_fmt = PE_NRD_H15A_TNR_SECAM;
			}
			else
			{
				tnr_fmt = PE_NRD_H15A_TNR_SD;
			}
			break;
		case LX_PE_SRC_RGBPC:
			{
				tnr_fmt = PE_NRD_H15A_TNR_SD;
			}
			break;
		case LX_PE_SRC_DTV:
		case LX_PE_SRC_COMP:
		case LX_PE_SRC_HDMI:
		default:
			if(disp_inf->src_type==LX_PE_SRC_HDMI && \
				disp_inf->hdmi_type==LX_PE_HDMI_PC)
			{
				tnr_fmt = PE_NRD_H15A_TNR_SD;
			}
			else if(disp_inf->fmt_type==LX_PE_FMT_SD)
			{
				tnr_fmt = PE_NRD_H15A_TNR_SD;
			}
			else if(disp_inf->fmt_type==LX_PE_FMT_HD)
			{
				if(disp_inf->src_type==LX_PE_SRC_DTV && \
					disp_inf->dtv_type==LX_PE_DTV_CAMERA)
				{
					tnr_fmt = PE_NRD_H15A_TNR_CAM_HD;
				}
				else
				{
					tnr_fmt = PE_NRD_H15A_TNR_HD;
				}
			}
			else
			{
				tnr_fmt = PE_NRD_H15A_TNR_UD;
			}
			break;
	}
	return tnr_fmt;
}
/**
 * get tnr lut (tnl) fmt from display info.(for H15AX)
 *
 * @param   *disp_inf [in] LX_PE_INF_DISPLAY_T
 * @return  PE_NRD_HW_H15A_TNL_FMT
 * @see
 * @author
 */
static PE_NRD_HW_H15A_TNL_FMT PE_NRD_HW_H15A_ConvDispInfoToTnlFmt(LX_PE_INF_DISPLAY_T *disp_inf)
{
	PE_NRD_HW_H15A_TNL_FMT tnl_fmt = PE_NRD_H15A_TNL_NUM;
	/* skip drv tnr ctrl, if fw tnr on. */
	if (PE_CMN_HW_H15_GetDbInfo(PE_CMN_HW_H15_DB_INF_MD0_TNR_FW_EN, 0))
	{
		tnl_fmt = PE_NRD_H15A_TNL_NUM;	//NA
	}
	else
	{
		switch(disp_inf->src_type)
		{
			case LX_PE_SRC_ATV:
			case LX_PE_SRC_CVBS:
			case LX_PE_SRC_SCART:
			case LX_PE_SRC_RGBPC:
				{
					tnl_fmt = PE_NRD_H15A_TNL_SD;
				}
				break;
			case LX_PE_SRC_DTV:
			case LX_PE_SRC_COMP:
			case LX_PE_SRC_HDMI:
			default:
				if(disp_inf->src_type==LX_PE_SRC_HDMI && \
					disp_inf->hdmi_type==LX_PE_HDMI_PC)
				{
					tnl_fmt = PE_NRD_H15A_TNL_SD;
				}
				else if(disp_inf->fmt_type==LX_PE_FMT_SD)
				{
					tnl_fmt = PE_NRD_H15A_TNL_SD;
				}
				else if(disp_inf->fmt_type==LX_PE_FMT_HD)
				{
					if(disp_inf->src_type==LX_PE_SRC_DTV && \
						disp_inf->dtv_type==LX_PE_DTV_CAMERA)
					{
						tnl_fmt = PE_NRD_H15A_TNL_CAM_HD;
					}
					else
					{
						tnl_fmt = PE_NRD_H15A_TNL_HD;
					}
				}
				else
				{
					tnl_fmt = PE_NRD_H15A_TNL_UD;
				}
				break;
		}
	}
	return tnl_fmt;
}
/**
 * get dnr fmt from display info.(for H15AX)
 *
 * @param   *disp_inf [in] LX_PE_INF_DISPLAY_T
 * @return  PE_NRD_HW_H15A_DNR_FMT
 * @see
 * @author
 */
static PE_NRD_HW_H15A_DNR_FMT PE_NRD_HW_H15A_ConvDispInfoToDnrFmt(LX_PE_INF_DISPLAY_T *disp_inf)
{
	PE_NRD_HW_H15A_DNR_FMT dnr_fmt = PE_NRD_H15A_DNR_NUM;
	switch(disp_inf->src_type)
	{
		case LX_PE_SRC_ATV:
			if(disp_inf->cstd_type==LX_PE_CSTD_NTSC)
			{
				dnr_fmt = PE_NRD_H15A_DNR_ATV_480;
			}
			else
			{
				dnr_fmt = PE_NRD_H15A_DNR_ATV_576;
			}
			break;
		case LX_PE_SRC_CVBS:
		case LX_PE_SRC_SCART:
			{
				dnr_fmt = PE_NRD_H15A_DNR_AV;
			}
			break;
		case LX_PE_SRC_RGBPC:
			{
				dnr_fmt = PE_NRD_H15A_DNR_PC;
			}
			break;
		case LX_PE_SRC_DTV:
			if(disp_inf->fmt_type==LX_PE_FMT_SD)
			{
				dnr_fmt = PE_NRD_H15A_DNR_DTV_SD;
			}
			else
			{
				dnr_fmt = PE_NRD_H15A_DNR_HD;
			}
			break;
		case LX_PE_SRC_COMP:
		case LX_PE_SRC_HDMI:
		default:
			if(disp_inf->src_type==LX_PE_SRC_HDMI && \
				disp_inf->hdmi_type==LX_PE_HDMI_PC)
			{
				dnr_fmt = PE_NRD_H15A_DNR_PC;
			}
			else if(disp_inf->fmt_type==LX_PE_FMT_SD)
			{
				dnr_fmt = PE_NRD_H15A_DNR_SD;
			}
			else
			{
				dnr_fmt = PE_NRD_H15A_DNR_HD;
			}
			break;
	}
	return dnr_fmt;
}
/**
 * get ifc fmt from display info.(for H15AX)
 *
 * @param   *disp_inf [in] LX_PE_INF_DISPLAY_T
 * @return  PE_NRD_HW_H15A_IFC_FMT
 * @see
 * @author
 */
static PE_NRD_HW_H15A_IFC_FMT PE_NRD_HW_H15A_ConvDispInfoToIfcFmt(LX_PE_INF_DISPLAY_T *disp_inf)
{
	PE_NRD_HW_H15A_IFC_FMT ifc_fmt = PE_NRD_H15A_IFC_NUM;
	switch(disp_inf->src_type)
	{
		case LX_PE_SRC_ATV:
			if(disp_inf->cstd_type==LX_PE_CSTD_NTSC)
			{
				ifc_fmt = PE_NRD_H15A_IFC_ATV_NTSC;
			}
			else
			{
				ifc_fmt = PE_NRD_H15A_IFC_PC;
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
				ifc_fmt = PE_NRD_H15A_IFC_PC;
			}
			break;
	}
	return ifc_fmt;
}
/**
 * read default setting
 *
 * @param   *pstParams [in/out] LX_PE_ETC_TBL_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_NRD_HW_H15_RdDefault(LX_PE_ETC_TBL_T *pstParams)
{
	int ret = RET_OK;
	#ifdef PE_HW_H15_PROTECT_OVERLAP_SETTINGS
	char buffer[PE_TRACE_STR_SIZE];
	UINT32 param_size=0, table_size=0, is_valid=0;
	static UINT32 rd_cnt=0;
	PE_REG_PARAM_T param;
	PE_NRD_HW_H15_SETTINGS_T *pInfo=&_g_pe_nrd_hw_h15_info;
	UINT32 *p_tnr1_dflt = _g_pe_tnr_hw_param_data_h15a.reg_dflt.addr;
	UINT32 *p_tnr1_user = _g_pe_tnr_hw_param_data_h15a.nd1_user.addr;
	UINT32 *p_tnr1_mask = _g_pe_tnr_hw_param_data_h15a.nd1_mask.addr;
	UINT32 *p_tnr1_data = _g_pe_tnr_hw_param_data_h15a.nd1_data.addr;
	UINT32 *p_tnr2_user = _g_pe_tnr_hw_param_data_h15a.nd2_user.addr;
	UINT32 *p_tnr2_mask = _g_pe_tnr_hw_param_data_h15a.nd2_mask.addr;
	UINT32 *p_tnr2_data = _g_pe_tnr_hw_param_data_h15a.nd2_data.addr;
	UINT32 *p_dnr1_dflt = _g_pe_dnr_hw_param_data_h15a.reg_dflt.addr;
	UINT32 *p_dnr1_user = _g_pe_dnr_hw_param_data_h15a.reg_user.addr;
	UINT32 *p_dnr1_mask = _g_pe_dnr_hw_param_data_h15a.reg_mask.addr;
	UINT32 *p_dnr1_data = _g_pe_dnr_hw_param_data_h15a.reg_data.addr;
	UINT32 *p_ifc1_dflt = _g_pe_ifc_hw_param_data_h15a.reg_dflt.addr;
	UINT32 *p_ifc1_user = _g_pe_ifc_hw_param_data_h15a.reg_user.addr;
	UINT32 *p_ifc1_mask = _g_pe_ifc_hw_param_data_h15a.reg_mask.addr;
	UINT32 *p_ifc1_data = _g_pe_ifc_hw_param_data_h15a.reg_data.addr;
	UINT32 *p_tnl1_dflt = _g_pe_tnl_hw_param_data_h15a.reg_dflt.addr;
	UINT32 *p_tnl1_user = _g_pe_tnl_hw_param_data_h15a.reg_user.addr;
	UINT32 *p_tnl1_mask = _g_pe_tnl_hw_param_data_h15a.reg_mask.addr;
	UINT32 *p_tnl1_data = _g_pe_tnl_hw_param_data_h15a.reg_data.addr;

#define PE_NRD_H15_NO_PRINT			0
#define PE_NRD_H15_RD_N_HEX_PRINT	1
#define PE_NRD_H15_HEX_PRINT_ONLY	2
#define PE_NRD_H15_PRINT_START		"START_OF_PRINT"
#define PE_NRD_H15_PRINT_END		"END_OF_PRINT"
#define PE_NRD_H15_PRINT_EXIT		"EXIT_OF_PRINT"
#define PE_NRD_H15_PRINT_RESRV		"RESERVED"
#define PE_NRD_H15_CASE_PRMENU(_cnt, _action, _buf, _item)	\
	case (_cnt):sprintf(_buf, "   [0x%02x] %s\n", (_item), #_item);_action;break
#define PE_NRD_H15_CASE_SPRINT(_cnt, _action, _buf, fmt, args...)	\
	case (_cnt):sprintf(_buf, fmt, ##args);_action;break
#define PE_NRD_H15_DFLT_SPRINT(_action, _buf, fmt, args...)		\
	default:sprintf(_buf, fmt, ##args);_action;break

	do{
		CHECK_KNULL(pstParams);
		CHECK_KNULL(pstParams->data);
		CHECK_KNULL(p_tnr1_dflt);
		CHECK_KNULL(p_tnr1_user);
		CHECK_KNULL(p_tnr1_mask);
		CHECK_KNULL(p_tnr1_data);
		CHECK_KNULL(p_tnr2_user);
		CHECK_KNULL(p_tnr2_mask);
		CHECK_KNULL(p_tnr2_data);
		CHECK_KNULL(p_dnr1_dflt);
		CHECK_KNULL(p_dnr1_user);
		CHECK_KNULL(p_dnr1_mask);
		CHECK_KNULL(p_dnr1_data);
		CHECK_KNULL(p_ifc1_dflt);
		CHECK_KNULL(p_ifc1_user);
		CHECK_KNULL(p_ifc1_mask);
		CHECK_KNULL(p_ifc1_data);
		if (PE_KDRV_VER_H15)
		{
			if (pstParams->data[0]>=PE_NRD_HW_H15A_RD_MENU_MAX)
			{
				sprintf(buffer, "%s", PE_NRD_H15_PRINT_EXIT);
				rd_cnt=0;
			}
			else if (pstParams->data[0]==PE_NRD_HW_H15A_RD_MENU_DISP)
			{
				switch (rd_cnt)
				{
					PE_NRD_H15_CASE_SPRINT( 0, rd_cnt++, buffer, PE_NRD_H15_PRINT_START);
					PE_NRD_H15_CASE_SPRINT( 1, rd_cnt++, buffer, "   ** current default **\n");
					PE_NRD_H15_CASE_SPRINT( 2, rd_cnt++, buffer, \
						"   tnr0a_fmt : %s\n", PE_NRD_HW_H15A_TNR_FMT_TO_STR(pInfo->tnr0a_fmt));
					PE_NRD_H15_CASE_SPRINT( 3, rd_cnt++, buffer, \
						"   dnr0a_fmt : %s\n", PE_NRD_HW_H15A_DNR_FMT_TO_STR(pInfo->dnr0a_fmt));
					PE_NRD_H15_CASE_SPRINT( 4, rd_cnt++, buffer, \
						"   ifc0a_fmt : %s\n", PE_NRD_HW_H15A_IFC_FMT_TO_STR(pInfo->ifc0a_fmt));
					PE_NRD_H15_CASE_SPRINT( 5, rd_cnt++, buffer, \
						"   tnl0a_fmt : %s\n", PE_NRD_HW_H15A_TNL_FMT_TO_STR(pInfo->tnl0a_fmt));
					PE_NRD_H15_CASE_SPRINT( 6, rd_cnt++, buffer, PE_NRD_H15_PRINT_RESRV);
					PE_NRD_H15_CASE_SPRINT( 7, rd_cnt++, buffer, PE_NRD_H15_PRINT_RESRV);
					PE_NRD_H15_CASE_SPRINT( 8, rd_cnt++, buffer, PE_NRD_H15_PRINT_RESRV);
					PE_NRD_H15_CASE_SPRINT( 9, rd_cnt++, buffer, "   *********************\n");
					PE_NRD_H15_CASE_PRMENU(10, rd_cnt++, buffer, PE_NRD_HW_H15A_RD_TNR1_DISP);
					PE_NRD_H15_CASE_PRMENU(11, rd_cnt++, buffer, PE_NRD_HW_H15A_RD_TNR2_DISP);
					PE_NRD_H15_CASE_PRMENU(12, rd_cnt++, buffer, PE_NRD_HW_H15A_RD_TNR1_DFLT);
					PE_NRD_H15_CASE_PRMENU(13, rd_cnt++, buffer, PE_NRD_HW_H15A_RD_TNR1_USER);
					PE_NRD_H15_CASE_PRMENU(14, rd_cnt++, buffer, PE_NRD_HW_H15A_RD_TNR1_MASK);
					PE_NRD_H15_CASE_PRMENU(15, rd_cnt++, buffer, PE_NRD_HW_H15A_RD_TNR1_DATA);
					PE_NRD_H15_CASE_PRMENU(16, rd_cnt++, buffer, PE_NRD_HW_H15A_RD_TNR2_USER);
					PE_NRD_H15_CASE_PRMENU(17, rd_cnt++, buffer, PE_NRD_HW_H15A_RD_TNR2_MASK);
					PE_NRD_H15_CASE_PRMENU(18, rd_cnt++, buffer, PE_NRD_HW_H15A_RD_TNR2_DATA);
					PE_NRD_H15_CASE_SPRINT(19, rd_cnt++, buffer, PE_NRD_H15_PRINT_RESRV);
					PE_NRD_H15_CASE_PRMENU(20, rd_cnt++, buffer, PE_NRD_HW_H15A_RD_DNR1_DISP);
					PE_NRD_H15_CASE_PRMENU(21, rd_cnt++, buffer, PE_NRD_HW_H15A_RD_DNR2_DISP);
					PE_NRD_H15_CASE_PRMENU(22, rd_cnt++, buffer, PE_NRD_HW_H15A_RD_DNR1_DFLT);
					PE_NRD_H15_CASE_PRMENU(23, rd_cnt++, buffer, PE_NRD_HW_H15A_RD_DNR1_USER);
					PE_NRD_H15_CASE_PRMENU(24, rd_cnt++, buffer, PE_NRD_HW_H15A_RD_DNR1_MASK);
					PE_NRD_H15_CASE_PRMENU(25, rd_cnt++, buffer, PE_NRD_HW_H15A_RD_DNR1_DATA);
					PE_NRD_H15_CASE_SPRINT(26, rd_cnt++, buffer, PE_NRD_H15_PRINT_RESRV);
					PE_NRD_H15_CASE_SPRINT(27, rd_cnt++, buffer, PE_NRD_H15_PRINT_RESRV);
					PE_NRD_H15_CASE_SPRINT(28, rd_cnt++, buffer, PE_NRD_H15_PRINT_RESRV);
					PE_NRD_H15_CASE_SPRINT(29, rd_cnt++, buffer, PE_NRD_H15_PRINT_RESRV);
					PE_NRD_H15_CASE_PRMENU(30, rd_cnt++, buffer, PE_NRD_HW_H15A_RD_IFC1_DISP);
					PE_NRD_H15_CASE_PRMENU(31, rd_cnt++, buffer, PE_NRD_HW_H15A_RD_IFC1_DFLT);
					PE_NRD_H15_CASE_PRMENU(32, rd_cnt++, buffer, PE_NRD_HW_H15A_RD_IFC1_USER);
					PE_NRD_H15_CASE_PRMENU(33, rd_cnt++, buffer, PE_NRD_HW_H15A_RD_IFC1_MASK);
					PE_NRD_H15_CASE_PRMENU(34, rd_cnt++, buffer, PE_NRD_HW_H15A_RD_IFC1_DATA);
					PE_NRD_H15_CASE_SPRINT(35, rd_cnt++, buffer, PE_NRD_H15_PRINT_RESRV);
					PE_NRD_H15_CASE_SPRINT(36, rd_cnt++, buffer, PE_NRD_H15_PRINT_RESRV);
					PE_NRD_H15_CASE_SPRINT(37, rd_cnt++, buffer, PE_NRD_H15_PRINT_RESRV);
					PE_NRD_H15_CASE_SPRINT(38, rd_cnt++, buffer, PE_NRD_H15_PRINT_RESRV);
					PE_NRD_H15_CASE_SPRINT(39, rd_cnt++, buffer, "   *********************\n");
					PE_NRD_H15_CASE_PRMENU(40, rd_cnt++, buffer, PE_NRD_HW_H15A_RD_TNR1_MLUT);
					PE_NRD_H15_CASE_PRMENU(41, rd_cnt++, buffer, PE_NRD_HW_H15A_RD_TNR2_MLUT);
					PE_NRD_H15_CASE_PRMENU(42, rd_cnt++, buffer, PE_NRD_HW_H15A_RD_TNL1_DFLT);
					PE_NRD_H15_CASE_PRMENU(43, rd_cnt++, buffer, PE_NRD_HW_H15A_RD_TNL1_USER);
					PE_NRD_H15_CASE_PRMENU(44, rd_cnt++, buffer, PE_NRD_HW_H15A_RD_TNL1_MASK);
					PE_NRD_H15_CASE_PRMENU(45, rd_cnt++, buffer, PE_NRD_HW_H15A_RD_TNL1_DATA);
					PE_NRD_H15_CASE_SPRINT(46, rd_cnt++, buffer, PE_NRD_H15_PRINT_RESRV);
					PE_NRD_H15_CASE_SPRINT(47, rd_cnt++, buffer, PE_NRD_H15_PRINT_RESRV);
					PE_NRD_H15_CASE_SPRINT(48, rd_cnt++, buffer, PE_NRD_H15_PRINT_RESRV);
					PE_NRD_H15_CASE_SPRINT(49, rd_cnt++, buffer, PE_NRD_H15_PRINT_RESRV);
					PE_NRD_H15_DFLT_SPRINT(rd_cnt=0, buffer, PE_NRD_H15_PRINT_END);
				}
			}
			else
			{
				is_valid = PE_NRD_H15_NO_PRINT;
				switch (pstParams->data[0])
				{
					case PE_NRD_HW_H15A_RD_TNR1_DISP:
					case PE_NRD_HW_H15A_RD_TNR2_DISP:
						table_size = sizeof(tnr_l_init_h15a0);
						param_size = table_size/sizeof(PE_REG_PARAM_T);
						if (rd_cnt<param_size)
						{
							param.addr = (pstParams->data[0]==PE_NRD_HW_H15A_RD_TNR1_DISP)? \
								tnr_l_init_h15a0[rd_cnt].addr:\
								tnr_l_init_h15a0[rd_cnt].addr+PE_H15A_ND_ND_1_2_DIFF;
							is_valid = PE_NRD_H15_RD_N_HEX_PRINT;
						}
						break;
					case PE_NRD_HW_H15A_RD_TNR1_DFLT:
						table_size = sizeof(PE_TNR_HW_PARAM_REG_H15A0_T);
						param_size = table_size/sizeof(UINT32);
						if (rd_cnt<param_size)
						{
							param.addr = tnr_l_init_h15a0[rd_cnt].addr;
							param.data = p_tnr1_dflt[rd_cnt];
							is_valid = PE_NRD_H15_HEX_PRINT_ONLY;
						}
						break;
					case PE_NRD_HW_H15A_RD_TNR1_USER:
					case PE_NRD_HW_H15A_RD_TNR2_USER:
						table_size = sizeof(PE_TNR_HW_PARAM_REG_H15A0_T);
						param_size = table_size/sizeof(UINT32);
						if (rd_cnt<param_size)
						{
							param.addr = tnr_l_init_h15a0[rd_cnt].addr;
							param.data = (pstParams->data[0]==PE_NRD_HW_H15A_RD_TNR1_USER)? \
								p_tnr1_user[rd_cnt]:p_tnr2_user[rd_cnt];
							is_valid = PE_NRD_H15_HEX_PRINT_ONLY;
						}
						break;
					case PE_NRD_HW_H15A_RD_TNR1_MASK:
					case PE_NRD_HW_H15A_RD_TNR2_MASK:
						table_size = sizeof(PE_TNR_HW_PARAM_REG_H15A0_T);
						param_size = table_size/sizeof(UINT32);
						if (rd_cnt<param_size)
						{
							param.addr = tnr_l_init_h15a0[rd_cnt].addr;
							param.data = (pstParams->data[0]==PE_NRD_HW_H15A_RD_TNR1_MASK)? \
								p_tnr1_mask[rd_cnt]:p_tnr2_mask[rd_cnt];
							is_valid = PE_NRD_H15_HEX_PRINT_ONLY;
						}
						break;
					case PE_NRD_HW_H15A_RD_TNR1_DATA:
					case PE_NRD_HW_H15A_RD_TNR2_DATA:
						table_size = sizeof(PE_TNR_HW_PARAM_REG_H15A0_T);
						param_size = table_size/sizeof(UINT32);
						if (rd_cnt<param_size)
						{
							param.addr = tnr_l_init_h15a0[rd_cnt].addr;
							param.data = (pstParams->data[0]==PE_NRD_HW_H15A_RD_TNR1_DATA)? \
								p_tnr1_data[rd_cnt]:p_tnr2_data[rd_cnt];
							is_valid = PE_NRD_H15_HEX_PRINT_ONLY;
						}
						break;
					case PE_NRD_HW_H15A_RD_DNR1_DISP:
					case PE_NRD_HW_H15A_RD_DNR2_DISP:
						table_size = sizeof(dnr_l_init_h15a0);
						param_size = table_size/sizeof(PE_REG_PARAM_T);
						if (rd_cnt<param_size)
						{
							param.addr = (pstParams->data[0]==PE_NRD_HW_H15A_RD_DNR1_DISP)? \
								dnr_l_init_h15a0[rd_cnt].addr:\
								dnr_l_init_h15a0[rd_cnt].addr+PE_H15A_ND_DNR_12_DIFF;
							is_valid = PE_NRD_H15_RD_N_HEX_PRINT;
						}
						break;
					case PE_NRD_HW_H15A_RD_DNR1_DFLT:
						table_size = sizeof(PE_DNR_HW_PARAM_REG_H15A0_T);
						param_size = table_size/sizeof(UINT32);
						if (rd_cnt<param_size)
						{
							param.addr = dnr_l_init_h15a0[rd_cnt].addr;
							param.data = p_dnr1_dflt[rd_cnt];
							is_valid = PE_NRD_H15_HEX_PRINT_ONLY;
						}
						break;
					case PE_NRD_HW_H15A_RD_DNR1_USER:
						table_size = sizeof(PE_DNR_HW_PARAM_REG_H15A0_T);
						param_size = table_size/sizeof(UINT32);
						if (rd_cnt<param_size)
						{
							param.addr = dnr_l_init_h15a0[rd_cnt].addr;
							param.data = p_dnr1_user[rd_cnt];
							is_valid = PE_NRD_H15_HEX_PRINT_ONLY;
						}
						break;
					case PE_NRD_HW_H15A_RD_DNR1_MASK:
						table_size = sizeof(PE_DNR_HW_PARAM_REG_H15A0_T);
						param_size = table_size/sizeof(UINT32);
						if (rd_cnt<param_size)
						{
							param.addr = dnr_l_init_h15a0[rd_cnt].addr;
							param.data = p_dnr1_mask[rd_cnt];
							is_valid = PE_NRD_H15_HEX_PRINT_ONLY;
						}
						break;
					case PE_NRD_HW_H15A_RD_DNR1_DATA:
						table_size = sizeof(PE_DNR_HW_PARAM_REG_H15A0_T);
						param_size = table_size/sizeof(UINT32);
						if (rd_cnt<param_size)
						{
							param.addr = dnr_l_init_h15a0[rd_cnt].addr;
							param.data = p_dnr1_data[rd_cnt];
							is_valid = PE_NRD_H15_HEX_PRINT_ONLY;
						}
						break;
					case PE_NRD_HW_H15A_RD_IFC1_DISP:
						table_size = sizeof(ifc_l_init_h15a0);
						param_size = table_size/sizeof(PE_REG_PARAM_T);
						if (rd_cnt<param_size)
						{
							param.addr = ifc_l_init_h15a0[rd_cnt].addr;
							is_valid = PE_NRD_H15_RD_N_HEX_PRINT;
						}
						break;
					case PE_NRD_HW_H15A_RD_IFC1_DFLT:
						table_size = sizeof(PE_IFC_HW_PARAM_REG_H15A0_T);
						param_size = table_size/sizeof(UINT32);
						if (rd_cnt<param_size)
						{
							param.addr = ifc_l_init_h15a0[rd_cnt].addr;
							param.data = p_ifc1_dflt[rd_cnt];
							is_valid = PE_NRD_H15_HEX_PRINT_ONLY;
						}
						break;
					case PE_NRD_HW_H15A_RD_IFC1_USER:
						table_size = sizeof(PE_IFC_HW_PARAM_REG_H15A0_T);
						param_size = table_size/sizeof(UINT32);
						if (rd_cnt<param_size)
						{
							param.addr = ifc_l_init_h15a0[rd_cnt].addr;
							param.data = p_ifc1_user[rd_cnt];
							is_valid = PE_NRD_H15_HEX_PRINT_ONLY;
						}
						break;
					case PE_NRD_HW_H15A_RD_IFC1_MASK:
						table_size = sizeof(PE_IFC_HW_PARAM_REG_H15A0_T);
						param_size = table_size/sizeof(UINT32);
						if (rd_cnt<param_size)
						{
							param.addr = ifc_l_init_h15a0[rd_cnt].addr;
							param.data = p_ifc1_mask[rd_cnt];
							is_valid = PE_NRD_H15_HEX_PRINT_ONLY;
						}
						break;
					case PE_NRD_HW_H15A_RD_IFC1_DATA:
						table_size = sizeof(PE_IFC_HW_PARAM_REG_H15A0_T);
						param_size = table_size/sizeof(UINT32);
						if (rd_cnt<param_size)
						{
							param.addr = ifc_l_init_h15a0[rd_cnt].addr;
							param.data = p_ifc1_data[rd_cnt];
							is_valid = PE_NRD_H15_HEX_PRINT_ONLY;
						}
						break;
					case PE_NRD_HW_H15A_RD_TNR1_MLUT:
					case PE_NRD_HW_H15A_RD_TNR2_MLUT:
						table_size = sizeof(tnr_l_lut_init_h15a0);
						param_size = table_size/sizeof(PE_REG_PARAM_T);
						if (rd_cnt<param_size)
						{
							if (rd_cnt==0)
							{
								if (pstParams->data[0]==PE_NRD_HW_H15A_RD_TNR1_MLUT)
									PE_REG_H15_WD(0x41D0, 0x00001000);
								else
									PE_REG_H15_WD(0x41D0+PE_H15A_ND_ND_1_2_DIFF, 0x00001000);
							}
							param.addr = (pstParams->data[0]==PE_NRD_HW_H15A_RD_TNR1_MLUT)? \
								tnr_l_lut_init_h15a0[rd_cnt].addr:\
								tnr_l_lut_init_h15a0[rd_cnt].addr+PE_H15A_ND_ND_1_2_DIFF;
							is_valid = PE_NRD_H15_RD_N_HEX_PRINT;
						}
						break;
					case PE_NRD_HW_H15A_RD_TNL1_DFLT:
						table_size = sizeof(PE_TNL_HW_PARAM_REG_H15A0_T);
						param_size = table_size/sizeof(UINT32);
						if (rd_cnt<param_size)
						{
							param.addr = tnr_l_lut_init_h15a0[rd_cnt].addr;
							param.data = p_tnl1_dflt[rd_cnt];
							is_valid = PE_NRD_H15_HEX_PRINT_ONLY;
						}
						break;
					case PE_NRD_HW_H15A_RD_TNL1_USER:
						table_size = sizeof(PE_TNL_HW_PARAM_REG_H15A0_T);
						param_size = table_size/sizeof(UINT32);
						if (rd_cnt<param_size)
						{
							param.addr = tnr_l_lut_init_h15a0[rd_cnt].addr;
							param.data = p_tnl1_user[rd_cnt];
							is_valid = PE_NRD_H15_HEX_PRINT_ONLY;
						}
						break;
					case PE_NRD_HW_H15A_RD_TNL1_MASK:
						table_size = sizeof(PE_TNL_HW_PARAM_REG_H15A0_T);
						param_size = table_size/sizeof(UINT32);
						if (rd_cnt<param_size)
						{
							param.addr = tnr_l_lut_init_h15a0[rd_cnt].addr;
							param.data = p_tnl1_mask[rd_cnt];
							is_valid = PE_NRD_H15_HEX_PRINT_ONLY;
						}
						break;
					case PE_NRD_HW_H15A_RD_TNL1_DATA:
						table_size = sizeof(PE_TNL_HW_PARAM_REG_H15A0_T);
						param_size = table_size/sizeof(UINT32);
						if (rd_cnt<param_size)
						{
							param.addr = tnr_l_lut_init_h15a0[rd_cnt].addr;
							param.data = p_tnl1_data[rd_cnt];
							is_valid = PE_NRD_H15_HEX_PRINT_ONLY;
						}
						break;
					default:
						break;
				}
				if (is_valid == PE_NRD_H15_RD_N_HEX_PRINT)
				{
					param.data = PE_REG_H15_RD(param.addr);
					sprintf(buffer, "   [%04d] {0x%04X, 0x%08X},\n", \
						rd_cnt, param.addr, param.data);
					rd_cnt++;
				}
				else if (is_valid == PE_NRD_H15_HEX_PRINT_ONLY)
				{
					sprintf(buffer, "   [%04d] {0x%04X, 0x%08X},\n", \
						rd_cnt, param.addr, param.data);
					rd_cnt++;
				}
				else
				{
					sprintf(buffer, "%s", PE_NRD_H15_PRINT_END);
					rd_cnt = 0;
				}
			}
			memcpy(pstParams->data, buffer, sizeof(char)*PE_TRACE_STR_SIZE);
		}
	}while (0);
	#endif
	return ret;
}

/**
 * set dnr common ctrl
 * - use input struct LX_PE_NRD_DNR6_CMN_T
 *  001._[MAX_CTRL]_0xC8007194_[0____]_1_UINT8__common[00]:_dnr_max_ctrl,__reg_dnr_max_enable,
 *  002._[DC_BNR__]_0xC80071BC_[0____]_1_UINT8__common[01]:_dc_bnr_ctrl_0,_reg_dc_bnr_enable,
 *  003._[DC_BNR__]_0xC80071C4_[15:10]_6_UINT8__common[02]:_dc_bnr_ctrl_2,_reg_dc_bnr_mastergain,
 *  004._[DC_BNR__]_0xC80071C4_[23:16]_8_UINT8__common[03]:_dc_bnr_ctrl_2,_reg_dc_bnr_chromagain,
 *  005._[AC_BNR__]_0xC80071D4_[0____]_1_UINT8__common[04]:_ac_bnr_ctrl_0,_reg_bnr_ac_h_en,
 *  006._[AC_BNR__]_0xC80071D4_[1____]_1_UINT8__common[05]:_ac_bnr_ctrl_0,_reg_bnr_ac_v_en,
 *  007._[AC_BNR__]_0xC80071D4_[2____]_1_UINT8__common[06]:_ac_bnr_ctrl_0,_reg_bnr_ac_h_chroma_en,
 *  008._[AC_BNR__]_0xC80071D4_[3____]_1_UINT8__common[07]:_ac_bnr_ctrl_0,_reg_bnr_ac_v_chroma_en,
 *  009._[AC_BNR__]_0xC80071D4_[5:4__]_2_UINT8__common[08]:_ac_bnr_ctrl_0,_reg_bnr_ac_acness_resol_h,
 *  010._[AC_BNR__]_0xC8007204_[15:10]_6_UINT8__common[09]:_ac_bnr_ctrl_9,_reg_ac_master_gain,
 *  011._[MNR_____]_0xC80071A0_[0____]_1_UINT8__common[10]:_mnr_ctrl_0,____reg_mnr_enable,
 *  012._[MNR_____]_0xC80071A0_[15:8_]_8_UINT8__common[11]:_mnr_ctrl_0,____reg_mnr_master_gain,
 *  013._[MNR_____]_0xC80071A0_[23:16]_8_UINT8__common[12]:_mnr_ctrl_0,____reg_chroma_master_gain,
 *  014._[MNR_____]_0xC80071A8_[23:16]_8_UINT8__common[13]:_mnr_ctrl_2,____reg_mnr_v_gain,
 *  015._[MNR_____]_0xC80071A8_[31:24]_8_UINT8__common[14]:_mnr_ctrl_2,____reg_mnr_h_gain,
 *
 * @param   *pstParams [in] void
 * @return  OK if success, ERROR otherwise.
 * @see		set discription
 * @author
 */
int PE_NRD_HW_H15_SetDnrCmnCtrl(void *pstParams)
{
	int ret = RET_OK;
	#ifdef PE_HW_H15_PROTECT_OVERLAP_SETTINGS
	#ifdef PE_USE_H15_NEW_DTNR_STRUCT
	PE_FWI_H15_DNR_COMMON fw_param;
	PE_NRD_HW_H15_SETTINGS_T *pInfo = &_g_pe_nrd_hw_h15_info;
	do{
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H15)
		{
			LX_PE_NRD_DNR6_CMN_T *pp=(LX_PE_NRD_DNR6_CMN_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			if (_g_nrd_hw_h15_trace)
			{
				UINT8 *pd = pp->data;
				PE_NRD_HW_H15_DBG_PRINT("[%d]:\n"\
					"data[00]0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,\n"\
					"data[10]0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,\n", \
					pp->win_id, \
					pd[0],  pd[1],  pd[2],  pd[3],  pd[4],  pd[5],  pd[6],  pd[7],  pd[8],  pd[9], \
					pd[10], pd[11], pd[12], pd[13], pd[14]);
			}
			if(PE_CHECK_WIN0(pp->win_id))
			{
				//PE_DNR12_H15A0_RdFL(dnr_max_ctrl);
				PE_DNR12_H15A0_RdFL(dc_bnr_ctrl_0);
				PE_DNR12_H15A0_RdFL(dc_bnr_ctrl_2);
				PE_DNR12_H15A0_RdFL(ac_bnr_ctrl_0);
				PE_DNR12_H15A0_RdFL(ac_bnr_ctrl_4);
				PE_DNR12_H15A0_RdFL(ac_bnr_ctrl_9);
				PE_DNR12_H15A0_RdFL(mnr_ctrl_0);
				PE_DNR12_H15A0_RdFL(mnr_ctrl_2);
				//PE_DNR12_H15A0_Wr01(dnr_max_ctrl,  reg_dnr_max_enable,        GET_BITS(pp->data[0], 0, 1));
				PE_DNR12_H15A0_Wr01(dc_bnr_ctrl_0, reg_dc_bnr_enable,         GET_BITS(pp->data[1], 0, 1));
				PE_DNR12_H15A0_Wr01(dc_bnr_ctrl_2, reg_dc_bnr_mastergain,     GET_BITS(pp->data[2], 0, 6));
				PE_DNR12_H15A0_Wr01(dc_bnr_ctrl_2, reg_dc_bnr_chromagain,     GET_BITS(pp->data[3], 0, 8));
				//PE_DNR12_H15A0_Wr01(ac_bnr_ctrl_0, reg_bnr_ac_h_en,           GET_BITS(pp->data[4], 0, 1));
				PE_DNR12_H15A0_Wr01(ac_bnr_ctrl_0, reg_bnr_ac_v_en,           GET_BITS(pp->data[5], 0, 1));
				PE_DNR12_H15A0_Wr01(ac_bnr_ctrl_0, reg_bnr_ac_h_chroma_en,    GET_BITS(pp->data[6], 0, 1));
				PE_DNR12_H15A0_Wr01(ac_bnr_ctrl_0, reg_bnr_ac_v_chroma_en,    GET_BITS(pp->data[7], 0, 1));
				PE_DNR12_H15A0_Wr01(ac_bnr_ctrl_0, reg_bnr_ac_acness_resol_h, GET_BITS(pp->data[8], 0, 2));
				PE_DNR12_H15A0_Wr01(ac_bnr_ctrl_9, reg_ac_master_gain,        GET_BITS(pp->data[9], 0, 6));
				PE_DNR12_H15A0_Wr01(mnr_ctrl_0,    reg_mnr_enable,            GET_BITS(pp->data[10],0, 1));
				PE_DNR12_H15A0_Wr01(mnr_ctrl_0,    reg_mnr_master_gain,       GET_BITS(pp->data[11],0, 8));
				PE_DNR12_H15A0_Wr01(mnr_ctrl_0,    reg_chroma_master_gain,    GET_BITS(pp->data[12],0, 8));
				PE_DNR12_H15A0_Wr01(mnr_ctrl_2,    reg_mnr_v_gain,            GET_BITS(pp->data[13],0, 8));
				PE_DNR12_H15A0_Wr01(mnr_ctrl_2,    reg_mnr_h_gain,            GET_BITS(pp->data[14],0, 8));
				//PE_DNR12_H15A0_WrFL(dnr_max_ctrl);
				PE_DNR12_H15A0_WrFL(dc_bnr_ctrl_0);
				PE_DNR12_H15A0_WrFL(dc_bnr_ctrl_2);
				PE_DNR12_H15A0_WrFL(ac_bnr_ctrl_0);
				PE_DNR12_H15A0_WrFL(ac_bnr_ctrl_4);
				PE_DNR12_H15A0_WrFL(ac_bnr_ctrl_9);
				PE_DNR12_H15A0_WrFL(mnr_ctrl_0);
				PE_DNR12_H15A0_WrFL(mnr_ctrl_2);
				/* update user data */
				//PE_NRD_HW_H15A_DNR_SET_USER(dnr_max_ctrl,  reg_dnr_max_enable,        GET_BITS(pp->data[0], 0, 1));
				pInfo->dnr_max_en0_user = GET_BITS(pp->data[0], 0, 1);
				pInfo->dnr_max_en0_mask = 1;
				PE_NRD_HW_H15A_DNR_SET_USER(dc_bnr_ctrl_0, reg_dc_bnr_enable,         GET_BITS(pp->data[1], 0, 1));
				PE_NRD_HW_H15A_DNR_SET_USER(dc_bnr_ctrl_2, reg_dc_bnr_mastergain,     GET_BITS(pp->data[2], 0, 6));
				PE_NRD_HW_H15A_DNR_SET_USER(dc_bnr_ctrl_2, reg_dc_bnr_chromagain,     GET_BITS(pp->data[3], 0, 8));
				//PE_NRD_HW_H15A_DNR_SET_USER(ac_bnr_ctrl_0, reg_bnr_ac_h_en,           GET_BITS(pp->data[4], 0, 1));
				pInfo->bnr_ac_h_en0_user = GET_BITS(pp->data[4], 0, 1);
				pInfo->bnr_ac_h_en0_mask = 1;
				PE_NRD_HW_H15A_DNR_SET_USER(ac_bnr_ctrl_0, reg_bnr_ac_v_en,           GET_BITS(pp->data[5], 0, 1));
				PE_NRD_HW_H15A_DNR_SET_USER(ac_bnr_ctrl_0, reg_bnr_ac_h_chroma_en,    GET_BITS(pp->data[6], 0, 1));
				PE_NRD_HW_H15A_DNR_SET_USER(ac_bnr_ctrl_0, reg_bnr_ac_v_chroma_en,    GET_BITS(pp->data[7], 0, 1));
				PE_NRD_HW_H15A_DNR_SET_USER(ac_bnr_ctrl_0, reg_bnr_ac_acness_resol_h, GET_BITS(pp->data[8], 0, 2));
				PE_NRD_HW_H15A_DNR_SET_USER(ac_bnr_ctrl_9, reg_ac_master_gain,        GET_BITS(pp->data[9], 0, 6));
				PE_NRD_HW_H15A_DNR_SET_USER(mnr_ctrl_0,    reg_mnr_enable,            GET_BITS(pp->data[10],0, 1));
				PE_NRD_HW_H15A_DNR_SET_USER(mnr_ctrl_0,    reg_mnr_master_gain,       GET_BITS(pp->data[11],0, 8));
				PE_NRD_HW_H15A_DNR_SET_USER(mnr_ctrl_0,    reg_chroma_master_gain,    GET_BITS(pp->data[12],0, 8));
				PE_NRD_HW_H15A_DNR_SET_USER(mnr_ctrl_2,    reg_mnr_v_gain,            GET_BITS(pp->data[13],0, 8));
				PE_NRD_HW_H15A_DNR_SET_USER(mnr_ctrl_2,    reg_mnr_h_gain,            GET_BITS(pp->data[14],0, 8));
				if (PE_CMN_HW_H15_GetDbInfo(PE_CMN_HW_H15_DB_INF_MD0_DNR_FW_EN, 0))
				{
					memcpy(fw_param.data, pp->data, sizeof(UINT8)*PE_FWI_H15_DNR_CMN_SIZE);
					ret = PE_FWI_H15_SetDnrCommon(&fw_param);
					PE_NRD_HW_H15_CHECK_CODE(ret, break, \
						"[%s,%d] PE_FWI_H15_SetDnrCommon() error.\n", __F__, __L__);
				}
			}
		}
		else
		{
			PE_NRD_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	#endif
	#endif
	return ret;
}
/**
 * get dnr common ctrl
 * - use input struct LX_PE_NRD_DNR6_CMN_T
 *  001._[MAX_CTRL]_0xC8007194_[0____]_1_UINT8__common[00]:_dnr_max_ctrl,__reg_dnr_max_enable,
 *  002._[DC_BNR__]_0xC80071BC_[0____]_1_UINT8__common[01]:_dc_bnr_ctrl_0,_reg_dc_bnr_enable,
 *  003._[DC_BNR__]_0xC80071C4_[15:10]_6_UINT8__common[02]:_dc_bnr_ctrl_2,_reg_dc_bnr_mastergain,
 *  004._[DC_BNR__]_0xC80071C4_[23:16]_8_UINT8__common[03]:_dc_bnr_ctrl_2,_reg_dc_bnr_chromagain,
 *  005._[AC_BNR__]_0xC80071D4_[0____]_1_UINT8__common[04]:_ac_bnr_ctrl_0,_reg_bnr_ac_h_en,
 *  006._[AC_BNR__]_0xC80071D4_[1____]_1_UINT8__common[05]:_ac_bnr_ctrl_0,_reg_bnr_ac_v_en,
 *  007._[AC_BNR__]_0xC80071D4_[2____]_1_UINT8__common[06]:_ac_bnr_ctrl_0,_reg_bnr_ac_h_chroma_en,
 *  008._[AC_BNR__]_0xC80071D4_[3____]_1_UINT8__common[07]:_ac_bnr_ctrl_0,_reg_bnr_ac_v_chroma_en,
 *  009._[AC_BNR__]_0xC80071D4_[5:4__]_2_UINT8__common[08]:_ac_bnr_ctrl_0,_reg_bnr_ac_acness_resol_h,
 *  010._[AC_BNR__]_0xC8007204_[15:10]_6_UINT8__common[09]:_ac_bnr_ctrl_9,_reg_ac_master_gain,
 *  011._[MNR_____]_0xC80071A0_[0____]_1_UINT8__common[10]:_mnr_ctrl_0,____reg_mnr_enable,
 *  012._[MNR_____]_0xC80071A0_[15:8_]_8_UINT8__common[11]:_mnr_ctrl_0,____reg_mnr_master_gain,
 *  013._[MNR_____]_0xC80071A0_[23:16]_8_UINT8__common[12]:_mnr_ctrl_0,____reg_chroma_master_gain,
 *  014._[MNR_____]_0xC80071A8_[23:16]_8_UINT8__common[13]:_mnr_ctrl_2,____reg_mnr_v_gain,
 *  015._[MNR_____]_0xC80071A8_[31:24]_8_UINT8__common[14]:_mnr_ctrl_2,____reg_mnr_h_gain,
 *
 * @param   *pstParams [in/out] void
 * @return  OK if success, ERROR otherwise.
 * @see		set discription
 * @author
 */
int PE_NRD_HW_H15_GetDnrCmnCtrl(void *pstParams)
{
	int ret = RET_OK;
	#ifdef PE_USE_H15_NEW_DTNR_STRUCT
	LX_PE_WIN_ID win_id;
	do{
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H15)
		{
			LX_PE_NRD_DNR6_CMN_T *pp = (LX_PE_NRD_DNR6_CMN_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			win_id = PE_GET_CHECKED_WINID(pp->win_id);
			if(PE_CHECK_WIN0(win_id))
			{
				PE_DNR1_H15A0_RdFL(dnr_max_ctrl);
				PE_DNR1_H15A0_RdFL(dc_bnr_ctrl_0);
				PE_DNR1_H15A0_RdFL(dc_bnr_ctrl_2);
				PE_DNR1_H15A0_RdFL(ac_bnr_ctrl_0);
				PE_DNR1_H15A0_RdFL(ac_bnr_ctrl_4);
				PE_DNR1_H15A0_RdFL(ac_bnr_ctrl_9);
				PE_DNR1_H15A0_RdFL(mnr_ctrl_0);
				PE_DNR1_H15A0_RdFL(mnr_ctrl_2);
				PE_DNR1_H15A0_Rd01(dnr_max_ctrl,  reg_dnr_max_enable,        pp->data[0]);
				PE_DNR1_H15A0_Rd01(dc_bnr_ctrl_0, reg_dc_bnr_enable,         pp->data[1]);
				PE_DNR1_H15A0_Rd01(dc_bnr_ctrl_2, reg_dc_bnr_mastergain,     pp->data[2]);
				PE_DNR1_H15A0_Rd01(dc_bnr_ctrl_2, reg_dc_bnr_chromagain,     pp->data[3]);
				PE_DNR1_H15A0_Rd01(ac_bnr_ctrl_0, reg_bnr_ac_h_en,           pp->data[4]);
				PE_DNR1_H15A0_Rd01(ac_bnr_ctrl_0, reg_bnr_ac_v_en,           pp->data[5]);
				PE_DNR1_H15A0_Rd01(ac_bnr_ctrl_0, reg_bnr_ac_h_chroma_en,    pp->data[6]);
				PE_DNR1_H15A0_Rd01(ac_bnr_ctrl_0, reg_bnr_ac_v_chroma_en,    pp->data[7]);
				PE_DNR1_H15A0_Rd01(ac_bnr_ctrl_0, reg_bnr_ac_acness_resol_h, pp->data[8]);
				PE_DNR1_H15A0_Rd01(ac_bnr_ctrl_9, reg_ac_master_gain,        pp->data[9]);
				PE_DNR1_H15A0_Rd01(mnr_ctrl_0,    reg_mnr_enable,            pp->data[10]);
				PE_DNR1_H15A0_Rd01(mnr_ctrl_0,    reg_mnr_master_gain,       pp->data[11]);
				PE_DNR1_H15A0_Rd01(mnr_ctrl_0,    reg_chroma_master_gain,    pp->data[12]);
				PE_DNR1_H15A0_Rd01(mnr_ctrl_2,    reg_mnr_v_gain,            pp->data[13]);
				PE_DNR1_H15A0_Rd01(mnr_ctrl_2,    reg_mnr_h_gain,            pp->data[14]);
			}
			if(PE_CHECK_WIN1(win_id))
			{
				PE_DNR2_H15A0_RdFL(dnr_max_ctrl);
				PE_DNR2_H15A0_RdFL(dc_bnr_ctrl_0);
				PE_DNR2_H15A0_RdFL(dc_bnr_ctrl_2);
				PE_DNR2_H15A0_RdFL(ac_bnr_ctrl_0);
				PE_DNR2_H15A0_RdFL(ac_bnr_ctrl_4);
				PE_DNR2_H15A0_RdFL(ac_bnr_ctrl_9);
				PE_DNR2_H15A0_RdFL(mnr_ctrl_0);
				PE_DNR2_H15A0_RdFL(mnr_ctrl_2);
				PE_DNR2_H15A0_Rd01(dnr_max_ctrl,  reg_dnr_max_enable,        pp->data[0]);
				PE_DNR2_H15A0_Rd01(dc_bnr_ctrl_0, reg_dc_bnr_enable,         pp->data[1]);
				PE_DNR2_H15A0_Rd01(dc_bnr_ctrl_2, reg_dc_bnr_mastergain,     pp->data[2]);
				PE_DNR2_H15A0_Rd01(dc_bnr_ctrl_2, reg_dc_bnr_chromagain,     pp->data[3]);
				PE_DNR2_H15A0_Rd01(ac_bnr_ctrl_0, reg_bnr_ac_h_en,           pp->data[4]);
				PE_DNR2_H15A0_Rd01(ac_bnr_ctrl_0, reg_bnr_ac_v_en,           pp->data[5]);
				PE_DNR2_H15A0_Rd01(ac_bnr_ctrl_0, reg_bnr_ac_h_chroma_en,    pp->data[6]);
				PE_DNR2_H15A0_Rd01(ac_bnr_ctrl_0, reg_bnr_ac_v_chroma_en,    pp->data[7]);
				PE_DNR2_H15A0_Rd01(ac_bnr_ctrl_0, reg_bnr_ac_acness_resol_h, pp->data[8]);
				PE_DNR2_H15A0_Rd01(ac_bnr_ctrl_9, reg_ac_master_gain,        pp->data[9]);
				PE_DNR2_H15A0_Rd01(mnr_ctrl_0,    reg_mnr_enable,            pp->data[10]);
				PE_DNR2_H15A0_Rd01(mnr_ctrl_0,    reg_mnr_master_gain,       pp->data[11]);
				PE_DNR2_H15A0_Rd01(mnr_ctrl_0,    reg_chroma_master_gain,    pp->data[12]);
				PE_DNR2_H15A0_Rd01(mnr_ctrl_2,    reg_mnr_v_gain,            pp->data[13]);
				PE_DNR2_H15A0_Rd01(mnr_ctrl_2,    reg_mnr_h_gain,            pp->data[14]);
			}
			if (_g_nrd_hw_h15_trace)
			{
				UINT8 *pd = pp->data;
				PE_NRD_HW_H15_DBG_PRINT("[%d]:\n"\
					"data[00]0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,\n"\
					"data[10]0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,\n", \
					pp->win_id, \
					pd[0],  pd[1],  pd[2],  pd[3],  pd[4],  pd[5],  pd[6],  pd[7],  pd[8],  pd[9], \
					pd[10], pd[11], pd[12], pd[13], pd[14]);
			}
		}
		else
		{
			PE_NRD_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	#endif
	return ret;
}

/**
 * set dnr detail ctrl
 * - use input struct LX_PE_NRD_DNR6_DETAIL_T
 *  016._[DC_BNR__]_0xC80071C4_[0____]_1_UINT8__dc_bnr[00]:_dc_bnr_ctrl_2,_reg_dc_var_en,
 *  017._[DC_BNR__]_0xC80071C4_[1____]_1_UINT8__dc_bnr[01]:_dc_bnr_ctrl_2,_reg_dc_motion_en,
 *  018._[DC_BNR__]_0xC80071C4_[2____]_1_UINT8__dc_bnr[02]:_dc_bnr_ctrl_2,_reg_dc_protection_en,
 *  019._[DC_BNR__]_0xC80071C4_[3____]_1_UINT8__dc_bnr[03]:_dc_bnr_ctrl_2,_reg_dc_detail_en,
 *  020._[DC_BNR__]_0xC80071BC_[3:2__]_2_UINT8__dc_bnr[04]:_dc_bnr_ctrl_0,_reg_dc_blur_sel,
 *  021._[DC_BNR__]_0xC80071BC_[23:16]_8_UINT8__dc_bnr[05]:_dc_bnr_ctrl_0,_reg_dc_motion_max,
 *  022._[DC_BNR__]_0xC80071BC_[31:24]_8_UINT8__dc_bnr[06]:_dc_bnr_ctrl_0,_reg_dc_motion_min,
 *  023._[DC_BNR__]_0xC80071C0_[7:0__]_8_UINT8__dc_bnr[07]:_dc_bnr_ctrl_1,_reg_dc_detail_max,
 *  024._[DC_BNR__]_0xC80071C0_[15:8_]_8_UINT8__dc_bnr[08]:_dc_bnr_ctrl_1,_reg_dc_detail_min,
 *  025._[DC_BNR__]_0xC80071C0_[23:20]_4_UINT8__dc_bnr[09]:_dc_bnr_ctrl_1,_reg_var_v_gain,
 *  026._[DC_BNR__]_0xC80071C0_[27:24]_4_UINT8__dc_bnr[10]:_dc_bnr_ctrl_1,_reg_var_h_gain,
 *  027._[DC_BNR__]_0xC80071C0_[31:28]_4_UINT8__dc_bnr[11]:_dc_bnr_ctrl_1,_reg_var_cut_resolution,
 *  028._[DC_BNR__]_0xC80071C4_[7:4__]_4_UINT8__dc_bnr[12]:_dc_bnr_ctrl_2,_reg_dc_global_motion_th,
 *  029._[DC_BNR__]_0xC80071C4_[31:24]_8_UINT8__dc_bnr[13]:_dc_bnr_ctrl_2,_reg_dc_protection_th,
 *  030._[DC_BNR__]_0xC80071C8_[7:0__]_8_UINT8__dc_bnr[14]:_dc_bnr_ctrl_3,_reg_dc_bnr_var_th3,
 *  031._[DC_BNR__]_0xC80071C8_[15:8_]_8_UINT8__dc_bnr[15]:_dc_bnr_ctrl_3,_reg_dc_bnr_var_th2,
 *  032._[DC_BNR__]_0xC80071C8_[23:16]_8_UINT8__dc_bnr[16]:_dc_bnr_ctrl_3,_reg_dc_bnr_var_th1,
 *  033._[DC_BNR__]_0xC80071C8_[31:24]_8_UINT8__dc_bnr[17]:_dc_bnr_ctrl_3,_reg_dc_bnr_var_th0,
 *  034._[DC_BNR__]_0xC80071C4_[8____]_1_UINT8__dc_bnr[18]:_dc_bnr_ctrl_2,_reg_detail_sel,
 *  035._[DETAIL__]_0xC80071B8_[7:0__]_8_UINT8__dc_bnr[19]:_detail_ctrl,___reg_bnr_ac_detail_max,
 *  036._[DETAIL__]_0xC80071B8_[15:8_]_8_UINT8__dc_bnr[20]:_detail_ctrl,___reg_bnr_ac_detail_min,
 *  037._[DETAIL__]_0xC80071B8_[23:16]_8_UINT8__dc_bnr[21]:_detail_ctrl,___reg_bnr_diff_l,
 *  038._[DETAIL__]_0xC80071B8_[31:24]_8_UINT8__dc_bnr[22]:_detail_ctrl,___reg_bnr_diff_p,
 *  039._[SC_BNR__]_0xC800726C_[0____]_1_UINT8__dc_bnr[23]:_sc_bnr_ctrl_0,_reg_sc_bnr_en,
 *  040._[SC_BNR__]_0xC8007280_[7:0__]_8_UINT8__dc_bnr[24]:_sc_bnr_ctrl_5,_reg_pos_gain_3,
 *  041._[SC_BNR__]_0xC8007280_[15:8_]_8_UINT8__dc_bnr[25]:_sc_bnr_ctrl_5,_reg_pos_gain_2,
 *  042._[SC_BNR__]_0xC8007280_[23:16]_8_UINT8__dc_bnr[26]:_sc_bnr_ctrl_5,_reg_pos_gain_1,
 *  043._[TEXTURE_]_0xC800728C_[2:0__]_3_UINT8__dc_bnr[27]:_texture_ctrl_0,reg_texture_cnt_div,
 *  044._[TEXTURE_]_0xC800728C_[15:8_]_8_UINT8__dc_bnr[28]:_texture_ctrl_0,reg_texture_cnt_mul,
 *  045._[TEXTURE_]_0xC800728C_[31:24]_8_UINT8__dc_bnr[29]:_texture_ctrl_0,reg_cross_th,
 *  046._[AC_BNR__]_0xC80071D4_[15:8_]_8_UINT8__ac_bnr[00]:_ac_bnr_ctrl_0,_reg_bnr_ac_diff_min_v_th,
 *  047._[AC_BNR__]_0xC80071D4_[23:16]_8_UINT8__ac_bnr[01]:_ac_bnr_ctrl_0,_reg_bnr_ac_diff_min_h_th,
 *  048._[AC_BNR__]_0xC80071D8_[7:0__]_8_UINT8__ac_bnr[02]:_ac_bnr_ctrl_1,_reg_bnr_ac_global_motion_th,
 *  049._[AC_BNR__]_0xC80071D8_[15:8_]_8_UINT8__ac_bnr[03]:_ac_bnr_ctrl_1,_reg_bnr_ac_h_acness_max,
 *  050._[AC_BNR__]_0xC80071E8_[23:16]_8_UINT8__ac_bnr[04]:_ac_bnr_ctrl_4,_reg_bnr_ac_v_acness_max,
 *  051._[AC_BNR__]_0xC80071D8_[23:16]_8_UINT8__ac_bnr[05]:_ac_bnr_ctrl_1,_reg_bnr_ac_h_acness_min,
 *  052._[AC_BNR__]_0xC80071E8_[31:24]_8_UINT8__ac_bnr[06]:_ac_bnr_ctrl_4,_reg_bnr_ac_v_acness_min,
 *  053._[AC_BNR__]_0xC80071E8_[7:6__]_2_UINT8__ac_bnr[07]:_ac_bnr_ctrl_4,_reg_bnr_ac_acness_resol_v,
 *  054._[AC_BNR__]_0xC80071DC_[7:0__]_8_UINT8__ac_bnr[08]:_ac_bnr_ctrl_2,_reg_bnr_ac_motion_3,
 *  055._[AC_BNR__]_0xC80071DC_[15:8_]_8_UINT8__ac_bnr[09]:_ac_bnr_ctrl_2,_reg_bnr_ac_motion_2,
 *  056._[AC_BNR__]_0xC80071DC_[23:16]_8_UINT8__ac_bnr[10]:_ac_bnr_ctrl_2,_reg_bnr_ac_motion_1,
 *  057._[AC_BNR__]_0xC80071DC_[31:24]_8_UINT8__ac_bnr[11]:_ac_bnr_ctrl_2,_reg_bnr_ac_motion_0,
 *  058._[AC_BNR__]_0xC80071E4_[7:0__]_8_UINT8__ac_bnr[12]:_ac_bnr_ctrl_3,_reg_bnr_ac_motion_y_3,
 *  059._[AC_BNR__]_0xC80071E4_[15:8_]_8_UINT8__ac_bnr[13]:_ac_bnr_ctrl_3,_reg_bnr_ac_motion_y_2,
 *  060._[AC_BNR__]_0xC80071E4_[23:16]_8_UINT8__ac_bnr[14]:_ac_bnr_ctrl_3,_reg_bnr_ac_motion_y_1,
 *  061._[AC_BNR__]_0xC80071E4_[31:24]_8_UINT8__ac_bnr[15]:_ac_bnr_ctrl_3,_reg_bnr_ac_motion_y_0,
 *  062._[AC_BNR__]_0xC80071F4_[31:24]_8_UINT8__ac_bnr[16]:_ac_bnr_ctrl_6,_reg_bnr_ac_detail_gain_th,
 *  063._[AC_BNR__]_0xC80071F0_[7:0__]_8_UINT8__ac_bnr[17]:_ac_bnr_ctrl_5,_reg_bnr_ac_detail_th4,
 *  064._[AC_BNR__]_0xC80071F0_[15:8_]_8_UINT8__ac_bnr[18]:_ac_bnr_ctrl_5,_reg_bnr_ac_detail_th3,
 *  065._[AC_BNR__]_0xC80071F0_[23:16]_8_UINT8__ac_bnr[19]:_ac_bnr_ctrl_5,_reg_bnr_ac_detail_th2,
 *  066._[AC_BNR__]_0xC80071F0_[31:24]_8_UINT8__ac_bnr[20]:_ac_bnr_ctrl_5,_reg_bnr_ac_detail_th1,
 *  067._[AC_BNR__]_0xC80071F8_[31:24]_8_UINT8__ac_bnr[21]:_ac_bnr_ctrl_7,_reg_bnr_ac_pos_gain_h3,
 *  068._[AC_BNR__]_0xC80071F4_[7:0__]_8_UINT8__ac_bnr[22]:_ac_bnr_ctrl_6,_reg_bnr_ac_pos_gain_h2,
 *  069._[AC_BNR__]_0xC80071F4_[15:8_]_8_UINT8__ac_bnr[23]:_ac_bnr_ctrl_6,_reg_bnr_ac_pos_gain_h1,
 *  070._[AC_BNR__]_0xC80071F4_[23:16]_8_UINT8__ac_bnr[24]:_ac_bnr_ctrl_6,_reg_bnr_ac_pos_gain_h0,
 *  071._[AC_BNR__]_0xC80071F8_[7:0__]_8_UINT8__ac_bnr[25]:_ac_bnr_ctrl_7,_reg_bnr_ac_pos_gain_l2,
 *  072._[AC_BNR__]_0xC80071F8_[15:8_]_8_UINT8__ac_bnr[26]:_ac_bnr_ctrl_7,_reg_bnr_ac_pos_gain_l1,
 *  073._[AC_BNR__]_0xC80071F8_[23:16]_8_UINT8__ac_bnr[27]:_ac_bnr_ctrl_7,_reg_bnr_ac_pos_gain_l0,
 *  074._[MNR_____]_0xC80071A0_[5:4__]_2_UINT8__mosqnr[00]:_mnr_ctrl_0,____reg_h_expand,
 *  075._[MNR_____]_0xC80071A0_[6____]_1_UINT8__mosqnr[01]:_mnr_ctrl_0,____reg_gain_flt_size,
 *  076._[MNR_____]_0xC80071A4_[7:0__]_8_UINT8__mosqnr[02]:_mnr_ctrl_1,____reg_mnr_s1_mmd_min,
 *  077._[MNR_____]_0xC80071A4_[15:8_]_8_UINT8__mosqnr[03]:_mnr_ctrl_1,____reg_mnr_s2_ratio_min,
 *  078._[MNR_____]_0xC80071A4_[23:16]_8_UINT8__mosqnr[04]:_mnr_ctrl_1,____reg_mnr_s2_ratio_max,
 *  079._[MNR_____]_0xC80071A4_[31:24]_8_UINT8__mosqnr[05]:_mnr_ctrl_1,____reg_mnr_s2_mmd_min,
 *  080._[MNR_____]_0xC80071AC_[7:0__]_8_UINT8__mosqnr[06]:_mnr_ctrl_3,____reg_filter_x0,
 *  081._[MNR_____]_0xC80071AC_[15:8_]_8_UINT8__mosqnr[07]:_mnr_ctrl_3,____reg_filter_x1,
 *  082._[MNR_____]_0xC80071AC_[23:16]_8_UINT8__mosqnr[08]:_mnr_ctrl_3,____reg_filter_y0,
 *  083._[MNR_____]_0xC80071AC_[31:24]_8_UINT8__mosqnr[09]:_mnr_ctrl_3,____reg_filter_y1,
 *  084._[MNR_____]_0xC80071B0_[0____]_1_UINT8__mosqnr[10]:_mnr_ctrl_4,____reg_motion_mnr_en,
 *  085._[MNR_____]_0xC80071B0_[2:1__]_2_UINT8__mosqnr[11]:_mnr_ctrl_4,____reg_motion_mnr_filter,
 *  086._[MNR_____]_0xC80071B0_[4:3__]_2_UINT8__mosqnr[12]:_mnr_ctrl_4,____reg_motion_mnr_filter_c,
 *  087._[MNR_____]_0xC80071B0_[6:5__]_2_UINT8__mosqnr[13]:_mnr_ctrl_4,____reg_mnr_filter,
 *  088._[MNR_____]_0xC80071B0_[8:7__]_2_UINT8__mosqnr[14]:_mnr_ctrl_4,____reg_mnr_filter_c,
 *  089._[MNR_____]_0xC80071B0_[23:16]_8_UINT8__mosqnr[15]:_mnr_ctrl_4,____reg_mnr_motion_min,
 *  090._[MNR_____]_0xC80071B0_[31:24]_8_UINT8__mosqnr[16]:_mnr_ctrl_4,____reg_mnr_motion_max,
 *
 * @param   *pstParams [in] void
 * @return  OK if success, ERROR otherwise.
 * @see		set discription
 * @author
 */
int PE_NRD_HW_H15_SetDnrDetailCtrl(void *pstParams)
{
	int ret = RET_OK;
	#ifdef PE_HW_H15_PROTECT_OVERLAP_SETTINGS
	#ifdef PE_USE_H15_NEW_DTNR_STRUCT
	PE_FWI_H15_DNR_DETAIL fw_param;
	do{
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H15)
		{
			LX_PE_NRD_DNR6_DETAIL_T *pp=(LX_PE_NRD_DNR6_DETAIL_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			if (_g_nrd_hw_h15_trace)
			{
				UINT8 *pd = pp->dc_bnr;
				PE_NRD_HW_H15_DBG_PRINT("[%d]:\n"\
					"dc_bnr[00]0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,\n"\
					"dc_bnr[10]0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,\n"\
					"dc_bnr[20]0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,\n", \
					pp->win_id, \
					pd[0],  pd[1],  pd[2],  pd[3],  pd[4],  pd[5],  pd[6],  pd[7],  pd[8],  pd[9], \
					pd[10], pd[11], pd[12], pd[13], pd[14], pd[15], pd[16], pd[17], pd[18], pd[19], \
					pd[20], pd[21], pd[22], pd[23], pd[24], pd[25], pd[26], pd[27], pd[28], pd[29]);
				pd = pp->ac_bnr;
				PE_NRD_HW_H15_DBG_PRINT("[%d]:\n"\
					"ac_bnr[00]0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,\n"\
					"ac_bnr[10]0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,\n"\
					"ac_bnr[20]0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,\n", \
					pp->win_id, \
					pd[0],  pd[1],  pd[2],  pd[3],  pd[4],  pd[5],  pd[6],  pd[7],  pd[8],  pd[9], \
					pd[10], pd[11], pd[12], pd[13], pd[14], pd[15], pd[16], pd[17], pd[18], pd[19], \
					pd[20], pd[21], pd[22], pd[23], pd[24], pd[25], pd[26], pd[27]);
				pd = pp->mosqnr;
				PE_NRD_HW_H15_DBG_PRINT("[%d]:\n"\
					"mosqnr[00]0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,\n"\
					"mosqnr[10]0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,\n", \
					pp->win_id, \
					pd[0],  pd[1],  pd[2],  pd[3],  pd[4],  pd[5],  pd[6],  pd[7],  pd[8],  pd[9], \
					pd[10], pd[11], pd[12], pd[13], pd[14], pd[15], pd[16]);
			}
			if(PE_CHECK_WIN0(pp->win_id))
			{
				/* bnr */
				PE_DNR12_H15A0_RdFL(dc_bnr_ctrl_2);
				PE_DNR12_H15A0_RdFL(dc_bnr_ctrl_0);
				PE_DNR12_H15A0_RdFL(dc_bnr_ctrl_1);
				PE_DNR12_H15A0_RdFL(dc_bnr_ctrl_3);
				PE_DNR12_H15A0_RdFL(detail_ctrl);
				PE_DNR12_H15A0_RdFL(sc_bnr_ctrl_0);
				PE_DNR12_H15A0_RdFL(sc_bnr_ctrl_5);
				PE_DNR12_H15A0_RdFL(texture_ctrl_0);
				PE_DNR12_H15A0_RdFL(ac_bnr_ctrl_0);
				PE_DNR12_H15A0_RdFL(ac_bnr_ctrl_1);
				PE_DNR12_H15A0_RdFL(ac_bnr_ctrl_4);
				PE_DNR12_H15A0_RdFL(ac_bnr_ctrl_2);
				PE_DNR12_H15A0_RdFL(ac_bnr_ctrl_3);
				PE_DNR12_H15A0_RdFL(ac_bnr_ctrl_6);
				PE_DNR12_H15A0_RdFL(ac_bnr_ctrl_5);
				PE_DNR12_H15A0_RdFL(ac_bnr_ctrl_7);
				PE_DNR12_H15A0_Wr01(dc_bnr_ctrl_2, reg_dc_var_en,               GET_BITS(pp->dc_bnr[0],  0, 1));
				PE_DNR12_H15A0_Wr01(dc_bnr_ctrl_2, reg_dc_motion_en,            GET_BITS(pp->dc_bnr[1],  0, 1));
				PE_DNR12_H15A0_Wr01(dc_bnr_ctrl_2, reg_dc_protection_en,        GET_BITS(pp->dc_bnr[2],  0, 1));
				PE_DNR12_H15A0_Wr01(dc_bnr_ctrl_2, reg_dc_detail_en,            GET_BITS(pp->dc_bnr[3],  0, 1));
				PE_DNR12_H15A0_Wr01(dc_bnr_ctrl_0, reg_dc_blur_sel,             GET_BITS(pp->dc_bnr[4],  0, 2));
				PE_DNR12_H15A0_Wr01(dc_bnr_ctrl_0, reg_dc_motion_max,           GET_BITS(pp->dc_bnr[5],  0, 8));
				PE_DNR12_H15A0_Wr01(dc_bnr_ctrl_0, reg_dc_motion_min,           GET_BITS(pp->dc_bnr[6],  0, 8));
				PE_DNR12_H15A0_Wr01(dc_bnr_ctrl_1, reg_dc_detail_max,           GET_BITS(pp->dc_bnr[7],  0, 8));
				PE_DNR12_H15A0_Wr01(dc_bnr_ctrl_1, reg_dc_detail_min,           GET_BITS(pp->dc_bnr[8],  0, 8));
				PE_DNR12_H15A0_Wr01(dc_bnr_ctrl_1, reg_var_v_gain,              GET_BITS(pp->dc_bnr[9],  0, 4));
				PE_DNR12_H15A0_Wr01(dc_bnr_ctrl_1, reg_var_h_gain,              GET_BITS(pp->dc_bnr[10], 0, 4));
				PE_DNR12_H15A0_Wr01(dc_bnr_ctrl_1, reg_var_cut_resolution,      GET_BITS(pp->dc_bnr[11], 0, 4));
				PE_DNR12_H15A0_Wr01(dc_bnr_ctrl_2, reg_dc_global_motion_th,     GET_BITS(pp->dc_bnr[12], 0, 4));
				PE_DNR12_H15A0_Wr01(dc_bnr_ctrl_2, reg_dc_protection_th,        GET_BITS(pp->dc_bnr[13], 0, 8));
				PE_DNR12_H15A0_Wr01(dc_bnr_ctrl_3, reg_dc_bnr_var_th3,          GET_BITS(pp->dc_bnr[14], 0, 8));
				PE_DNR12_H15A0_Wr01(dc_bnr_ctrl_3, reg_dc_bnr_var_th2,          GET_BITS(pp->dc_bnr[15], 0, 8));
				PE_DNR12_H15A0_Wr01(dc_bnr_ctrl_3, reg_dc_bnr_var_th1,          GET_BITS(pp->dc_bnr[16], 0, 8));
				PE_DNR12_H15A0_Wr01(dc_bnr_ctrl_3, reg_dc_bnr_var_th0,          GET_BITS(pp->dc_bnr[17], 0, 8));
				PE_DNR12_H15A0_Wr01(dc_bnr_ctrl_2, reg_detail_sel,              GET_BITS(pp->dc_bnr[18], 0, 1));
				PE_DNR12_H15A0_Wr01(detail_ctrl,   reg_bnr_ac_detail_max,       GET_BITS(pp->dc_bnr[19], 0, 8));
				PE_DNR12_H15A0_Wr01(detail_ctrl,   reg_bnr_ac_detail_min,       GET_BITS(pp->dc_bnr[20], 0, 8));
				PE_DNR12_H15A0_Wr01(detail_ctrl,   reg_bnr_diff_l,              GET_BITS(pp->dc_bnr[21], 0, 8));
				PE_DNR12_H15A0_Wr01(detail_ctrl,   reg_bnr_diff_p,              GET_BITS(pp->dc_bnr[22], 0, 8));
				PE_DNR12_H15A0_Wr01(sc_bnr_ctrl_0, reg_sc_bnr_en,               GET_BITS(pp->dc_bnr[23], 0, 1));
				PE_DNR12_H15A0_Wr01(sc_bnr_ctrl_5, reg_pos_gain_3,              GET_BITS(pp->dc_bnr[24], 0, 8));
				PE_DNR12_H15A0_Wr01(sc_bnr_ctrl_5, reg_pos_gain_2,              GET_BITS(pp->dc_bnr[25], 0, 8));
				PE_DNR12_H15A0_Wr01(sc_bnr_ctrl_5, reg_pos_gain_1,              GET_BITS(pp->dc_bnr[26], 0, 8));
				PE_DNR12_H15A0_Wr01(texture_ctrl_0,reg_texture_cnt_div,         GET_BITS(pp->dc_bnr[27], 0, 3));
				PE_DNR12_H15A0_Wr01(texture_ctrl_0,reg_texture_cnt_mul,         GET_BITS(pp->dc_bnr[28], 0, 8));
				PE_DNR12_H15A0_Wr01(texture_ctrl_0,reg_cross_th,                GET_BITS(pp->dc_bnr[29], 0, 8));
				PE_DNR12_H15A0_Wr01(ac_bnr_ctrl_0, reg_bnr_ac_diff_min_v_th,    GET_BITS(pp->ac_bnr[0],  0, 8));
				PE_DNR12_H15A0_Wr01(ac_bnr_ctrl_0, reg_bnr_ac_diff_min_h_th,    GET_BITS(pp->ac_bnr[1],  0, 8));
				PE_DNR12_H15A0_Wr01(ac_bnr_ctrl_1, reg_bnr_ac_global_motion_th, GET_BITS(pp->ac_bnr[2],  0, 8));
				PE_DNR12_H15A0_Wr01(ac_bnr_ctrl_1, reg_bnr_ac_h_acness_max,     GET_BITS(pp->ac_bnr[3],  0, 8));
				PE_DNR12_H15A0_Wr01(ac_bnr_ctrl_4, reg_bnr_ac_v_acness_max,     GET_BITS(pp->ac_bnr[4],  0, 8));
				PE_DNR12_H15A0_Wr01(ac_bnr_ctrl_1, reg_bnr_ac_h_acness_min,     GET_BITS(pp->ac_bnr[5],  0, 8));
				PE_DNR12_H15A0_Wr01(ac_bnr_ctrl_4, reg_bnr_ac_v_acness_min,     GET_BITS(pp->ac_bnr[6],  0, 8));
				PE_DNR12_H15A0_Wr01(ac_bnr_ctrl_4, reg_bnr_ac_acness_resol_v,   GET_BITS(pp->ac_bnr[7],  0, 2));
				PE_DNR12_H15A0_Wr01(ac_bnr_ctrl_2, reg_bnr_ac_motion_3,         GET_BITS(pp->ac_bnr[8],  0, 8));
				PE_DNR12_H15A0_Wr01(ac_bnr_ctrl_2, reg_bnr_ac_motion_2,         GET_BITS(pp->ac_bnr[9],  0, 8));
				PE_DNR12_H15A0_Wr01(ac_bnr_ctrl_2, reg_bnr_ac_motion_1,         GET_BITS(pp->ac_bnr[10], 0, 8));
				PE_DNR12_H15A0_Wr01(ac_bnr_ctrl_2, reg_bnr_ac_motion_0,         GET_BITS(pp->ac_bnr[11], 0, 8));
				PE_DNR12_H15A0_Wr01(ac_bnr_ctrl_3, reg_bnr_ac_motion_y_3,       GET_BITS(pp->ac_bnr[12], 0, 8));
				PE_DNR12_H15A0_Wr01(ac_bnr_ctrl_3, reg_bnr_ac_motion_y_2,       GET_BITS(pp->ac_bnr[13], 0, 8));
				PE_DNR12_H15A0_Wr01(ac_bnr_ctrl_3, reg_bnr_ac_motion_y_1,       GET_BITS(pp->ac_bnr[14], 0, 8));
				PE_DNR12_H15A0_Wr01(ac_bnr_ctrl_3, reg_bnr_ac_motion_y_0,       GET_BITS(pp->ac_bnr[15], 0, 8));
				PE_DNR12_H15A0_Wr01(ac_bnr_ctrl_6, reg_bnr_ac_detail_gain_th,   GET_BITS(pp->ac_bnr[16], 0, 8));
				PE_DNR12_H15A0_Wr01(ac_bnr_ctrl_5, reg_bnr_ac_detail_th4,       GET_BITS(pp->ac_bnr[17], 0, 8));
				PE_DNR12_H15A0_Wr01(ac_bnr_ctrl_5, reg_bnr_ac_detail_th3,       GET_BITS(pp->ac_bnr[18], 0, 8));
				PE_DNR12_H15A0_Wr01(ac_bnr_ctrl_5, reg_bnr_ac_detail_th2,       GET_BITS(pp->ac_bnr[19], 0, 8));
				PE_DNR12_H15A0_Wr01(ac_bnr_ctrl_5, reg_bnr_ac_detail_th1,       GET_BITS(pp->ac_bnr[20], 0, 8));
				PE_DNR12_H15A0_Wr01(ac_bnr_ctrl_7, reg_bnr_ac_pos_gain_h3,      GET_BITS(pp->ac_bnr[21], 0, 8));
				PE_DNR12_H15A0_Wr01(ac_bnr_ctrl_6, reg_bnr_ac_pos_gain_h2,      GET_BITS(pp->ac_bnr[22], 0, 8));
				PE_DNR12_H15A0_Wr01(ac_bnr_ctrl_6, reg_bnr_ac_pos_gain_h1,      GET_BITS(pp->ac_bnr[23], 0, 8));
				PE_DNR12_H15A0_Wr01(ac_bnr_ctrl_6, reg_bnr_ac_pos_gain_h0,      GET_BITS(pp->ac_bnr[24], 0, 8));
				PE_DNR12_H15A0_Wr01(ac_bnr_ctrl_7, reg_bnr_ac_pos_gain_l2,      GET_BITS(pp->ac_bnr[25], 0, 8));
				PE_DNR12_H15A0_Wr01(ac_bnr_ctrl_7, reg_bnr_ac_pos_gain_l1,      GET_BITS(pp->ac_bnr[26], 0, 8));
				PE_DNR12_H15A0_Wr01(ac_bnr_ctrl_7, reg_bnr_ac_pos_gain_l0,      GET_BITS(pp->ac_bnr[27], 0, 8));
				PE_DNR12_H15A0_WrFL(dc_bnr_ctrl_2);
				PE_DNR12_H15A0_WrFL(dc_bnr_ctrl_0);
				PE_DNR12_H15A0_WrFL(dc_bnr_ctrl_1);
				PE_DNR12_H15A0_WrFL(dc_bnr_ctrl_3);
				PE_DNR12_H15A0_WrFL(detail_ctrl);
				PE_DNR12_H15A0_WrFL(sc_bnr_ctrl_0);
				PE_DNR12_H15A0_WrFL(sc_bnr_ctrl_5);
				PE_DNR12_H15A0_WrFL(texture_ctrl_0);
				PE_DNR12_H15A0_WrFL(ac_bnr_ctrl_0);
				PE_DNR12_H15A0_WrFL(ac_bnr_ctrl_1);
				PE_DNR12_H15A0_WrFL(ac_bnr_ctrl_4);
				PE_DNR12_H15A0_WrFL(ac_bnr_ctrl_2);
				PE_DNR12_H15A0_WrFL(ac_bnr_ctrl_3);
				PE_DNR12_H15A0_WrFL(ac_bnr_ctrl_6);
				PE_DNR12_H15A0_WrFL(ac_bnr_ctrl_5);
				PE_DNR12_H15A0_WrFL(ac_bnr_ctrl_7);
				/* mnr */
				PE_DNR12_H15A0_RdFL(mnr_ctrl_0);
				PE_DNR12_H15A0_RdFL(mnr_ctrl_1);
				PE_DNR12_H15A0_RdFL(mnr_ctrl_3);
				PE_DNR12_H15A0_RdFL(mnr_ctrl_4);
				PE_DNR12_H15A0_Wr01(mnr_ctrl_0,    reg_h_expand,                GET_BITS(pp->mosqnr[0],  0, 2));
				PE_DNR12_H15A0_Wr01(mnr_ctrl_0,    reg_gain_flt_size,           GET_BITS(pp->mosqnr[1],  0, 1));
				PE_DNR12_H15A0_Wr01(mnr_ctrl_1,    reg_mnr_s1_mmd_min,          GET_BITS(pp->mosqnr[2],  0, 8));
				PE_DNR12_H15A0_Wr01(mnr_ctrl_1,    reg_mnr_s2_ratio_min,        GET_BITS(pp->mosqnr[3],  0, 8));
				PE_DNR12_H15A0_Wr01(mnr_ctrl_1,    reg_mnr_s2_ratio_max,        GET_BITS(pp->mosqnr[4],  0, 8));
				PE_DNR12_H15A0_Wr01(mnr_ctrl_1,    reg_mnr_s2_mmd_min,          GET_BITS(pp->mosqnr[5],  0, 8));
				PE_DNR12_H15A0_Wr01(mnr_ctrl_3,    reg_filter_x0,               GET_BITS(pp->mosqnr[6],  0, 8));
				PE_DNR12_H15A0_Wr01(mnr_ctrl_3,    reg_filter_x1,               GET_BITS(pp->mosqnr[7],  0, 8));
				PE_DNR12_H15A0_Wr01(mnr_ctrl_3,    reg_filter_y0,               GET_BITS(pp->mosqnr[8],  0, 8));
				PE_DNR12_H15A0_Wr01(mnr_ctrl_3,    reg_filter_y1,               GET_BITS(pp->mosqnr[9],  0, 8));
				PE_DNR12_H15A0_Wr01(mnr_ctrl_4,    reg_motion_mnr_en,           GET_BITS(pp->mosqnr[10], 0, 1));
				PE_DNR12_H15A0_Wr01(mnr_ctrl_4,    reg_motion_mnr_filter,       GET_BITS(pp->mosqnr[11], 0, 2));
				PE_DNR12_H15A0_Wr01(mnr_ctrl_4,    reg_motion_mnr_filter_c,     GET_BITS(pp->mosqnr[12], 0, 2));
				PE_DNR12_H15A0_Wr01(mnr_ctrl_4,    reg_mnr_filter,              GET_BITS(pp->mosqnr[13], 0, 2));
				PE_DNR12_H15A0_Wr01(mnr_ctrl_4,    reg_mnr_filter_c,            GET_BITS(pp->mosqnr[14], 0, 2));
				PE_DNR12_H15A0_Wr01(mnr_ctrl_4,    reg_mnr_motion_min,          GET_BITS(pp->mosqnr[15], 0, 8));
				PE_DNR12_H15A0_Wr01(mnr_ctrl_4,    reg_mnr_motion_max,          GET_BITS(pp->mosqnr[16], 0, 8));
				PE_DNR12_H15A0_WrFL(mnr_ctrl_0);
				PE_DNR12_H15A0_WrFL(mnr_ctrl_1);
				PE_DNR12_H15A0_WrFL(mnr_ctrl_3);
				PE_DNR12_H15A0_WrFL(mnr_ctrl_4);
				/* set user data */
				PE_NRD_HW_H15A_DNR_SET_USER(dc_bnr_ctrl_2, reg_dc_var_en,               GET_BITS(pp->dc_bnr[0],  0, 1));
				PE_NRD_HW_H15A_DNR_SET_USER(dc_bnr_ctrl_2, reg_dc_motion_en,            GET_BITS(pp->dc_bnr[1],  0, 1));
				PE_NRD_HW_H15A_DNR_SET_USER(dc_bnr_ctrl_2, reg_dc_protection_en,        GET_BITS(pp->dc_bnr[2],  0, 1));
				PE_NRD_HW_H15A_DNR_SET_USER(dc_bnr_ctrl_2, reg_dc_detail_en,            GET_BITS(pp->dc_bnr[3],  0, 1));
				PE_NRD_HW_H15A_DNR_SET_USER(dc_bnr_ctrl_0, reg_dc_blur_sel,             GET_BITS(pp->dc_bnr[4],  0, 2));
				PE_NRD_HW_H15A_DNR_SET_USER(dc_bnr_ctrl_0, reg_dc_motion_max,           GET_BITS(pp->dc_bnr[5],  0, 8));
				PE_NRD_HW_H15A_DNR_SET_USER(dc_bnr_ctrl_0, reg_dc_motion_min,           GET_BITS(pp->dc_bnr[6],  0, 8));
				PE_NRD_HW_H15A_DNR_SET_USER(dc_bnr_ctrl_1, reg_dc_detail_max,           GET_BITS(pp->dc_bnr[7],  0, 8));
				PE_NRD_HW_H15A_DNR_SET_USER(dc_bnr_ctrl_1, reg_dc_detail_min,           GET_BITS(pp->dc_bnr[8],  0, 8));
				PE_NRD_HW_H15A_DNR_SET_USER(dc_bnr_ctrl_1, reg_var_v_gain,              GET_BITS(pp->dc_bnr[9],  0, 4));
				PE_NRD_HW_H15A_DNR_SET_USER(dc_bnr_ctrl_1, reg_var_h_gain,              GET_BITS(pp->dc_bnr[10], 0, 4));
				PE_NRD_HW_H15A_DNR_SET_USER(dc_bnr_ctrl_1, reg_var_cut_resolution,      GET_BITS(pp->dc_bnr[11], 0, 4));
				PE_NRD_HW_H15A_DNR_SET_USER(dc_bnr_ctrl_2, reg_dc_global_motion_th,     GET_BITS(pp->dc_bnr[12], 0, 4));
				PE_NRD_HW_H15A_DNR_SET_USER(dc_bnr_ctrl_2, reg_dc_protection_th,        GET_BITS(pp->dc_bnr[13], 0, 8));
				PE_NRD_HW_H15A_DNR_SET_USER(dc_bnr_ctrl_3, reg_dc_bnr_var_th3,          GET_BITS(pp->dc_bnr[14], 0, 8));
				PE_NRD_HW_H15A_DNR_SET_USER(dc_bnr_ctrl_3, reg_dc_bnr_var_th2,          GET_BITS(pp->dc_bnr[15], 0, 8));
				PE_NRD_HW_H15A_DNR_SET_USER(dc_bnr_ctrl_3, reg_dc_bnr_var_th1,          GET_BITS(pp->dc_bnr[16], 0, 8));
				PE_NRD_HW_H15A_DNR_SET_USER(dc_bnr_ctrl_3, reg_dc_bnr_var_th0,          GET_BITS(pp->dc_bnr[17], 0, 8));
				PE_NRD_HW_H15A_DNR_SET_USER(dc_bnr_ctrl_2, reg_detail_sel,              GET_BITS(pp->dc_bnr[18], 0, 1));
				PE_NRD_HW_H15A_DNR_SET_USER(detail_ctrl,   reg_bnr_ac_detail_max,       GET_BITS(pp->dc_bnr[19], 0, 8));
				PE_NRD_HW_H15A_DNR_SET_USER(detail_ctrl,   reg_bnr_ac_detail_min,       GET_BITS(pp->dc_bnr[20], 0, 8));
				PE_NRD_HW_H15A_DNR_SET_USER(detail_ctrl,   reg_bnr_diff_l,              GET_BITS(pp->dc_bnr[21], 0, 8));
				PE_NRD_HW_H15A_DNR_SET_USER(detail_ctrl,   reg_bnr_diff_p,              GET_BITS(pp->dc_bnr[22], 0, 8));
				PE_NRD_HW_H15A_DNR_SET_USER(sc_bnr_ctrl_0, reg_sc_bnr_en,               GET_BITS(pp->dc_bnr[23], 0, 1));
				PE_NRD_HW_H15A_DNR_SET_USER(sc_bnr_ctrl_5, reg_pos_gain_3,              GET_BITS(pp->dc_bnr[24], 0, 8));
				PE_NRD_HW_H15A_DNR_SET_USER(sc_bnr_ctrl_5, reg_pos_gain_2,              GET_BITS(pp->dc_bnr[25], 0, 8));
				PE_NRD_HW_H15A_DNR_SET_USER(sc_bnr_ctrl_5, reg_pos_gain_1,              GET_BITS(pp->dc_bnr[26], 0, 8));
				PE_NRD_HW_H15A_DNR_SET_USER(texture_ctrl_0,reg_texture_cnt_div,         GET_BITS(pp->dc_bnr[27], 0, 3));
				PE_NRD_HW_H15A_DNR_SET_USER(texture_ctrl_0,reg_texture_cnt_mul,         GET_BITS(pp->dc_bnr[28], 0, 8));
				PE_NRD_HW_H15A_DNR_SET_USER(texture_ctrl_0,reg_cross_th,                GET_BITS(pp->dc_bnr[29], 0, 8));
				PE_NRD_HW_H15A_DNR_SET_USER(ac_bnr_ctrl_0, reg_bnr_ac_diff_min_v_th,    GET_BITS(pp->ac_bnr[0],  0, 8));
				PE_NRD_HW_H15A_DNR_SET_USER(ac_bnr_ctrl_0, reg_bnr_ac_diff_min_h_th,    GET_BITS(pp->ac_bnr[1],  0, 8));
				PE_NRD_HW_H15A_DNR_SET_USER(ac_bnr_ctrl_1, reg_bnr_ac_global_motion_th, GET_BITS(pp->ac_bnr[2],  0, 8));
				PE_NRD_HW_H15A_DNR_SET_USER(ac_bnr_ctrl_1, reg_bnr_ac_h_acness_max,     GET_BITS(pp->ac_bnr[3],  0, 8));
				PE_NRD_HW_H15A_DNR_SET_USER(ac_bnr_ctrl_4, reg_bnr_ac_v_acness_max,     GET_BITS(pp->ac_bnr[4],  0, 8));
				PE_NRD_HW_H15A_DNR_SET_USER(ac_bnr_ctrl_1, reg_bnr_ac_h_acness_min,     GET_BITS(pp->ac_bnr[5],  0, 8));
				PE_NRD_HW_H15A_DNR_SET_USER(ac_bnr_ctrl_4, reg_bnr_ac_v_acness_min,     GET_BITS(pp->ac_bnr[6],  0, 8));
				PE_NRD_HW_H15A_DNR_SET_USER(ac_bnr_ctrl_4, reg_bnr_ac_acness_resol_v,   GET_BITS(pp->ac_bnr[7],  0, 2));
				PE_NRD_HW_H15A_DNR_SET_USER(ac_bnr_ctrl_2, reg_bnr_ac_motion_3,         GET_BITS(pp->ac_bnr[8],  0, 8));
				PE_NRD_HW_H15A_DNR_SET_USER(ac_bnr_ctrl_2, reg_bnr_ac_motion_2,         GET_BITS(pp->ac_bnr[9],  0, 8));
				PE_NRD_HW_H15A_DNR_SET_USER(ac_bnr_ctrl_2, reg_bnr_ac_motion_1,         GET_BITS(pp->ac_bnr[10], 0, 8));
				PE_NRD_HW_H15A_DNR_SET_USER(ac_bnr_ctrl_2, reg_bnr_ac_motion_0,         GET_BITS(pp->ac_bnr[11], 0, 8));
				PE_NRD_HW_H15A_DNR_SET_USER(ac_bnr_ctrl_3, reg_bnr_ac_motion_y_3,       GET_BITS(pp->ac_bnr[12], 0, 8));
				PE_NRD_HW_H15A_DNR_SET_USER(ac_bnr_ctrl_3, reg_bnr_ac_motion_y_2,       GET_BITS(pp->ac_bnr[13], 0, 8));
				PE_NRD_HW_H15A_DNR_SET_USER(ac_bnr_ctrl_3, reg_bnr_ac_motion_y_1,       GET_BITS(pp->ac_bnr[14], 0, 8));
				PE_NRD_HW_H15A_DNR_SET_USER(ac_bnr_ctrl_3, reg_bnr_ac_motion_y_0,       GET_BITS(pp->ac_bnr[15], 0, 8));
				PE_NRD_HW_H15A_DNR_SET_USER(ac_bnr_ctrl_6, reg_bnr_ac_detail_gain_th,   GET_BITS(pp->ac_bnr[16], 0, 8));
				PE_NRD_HW_H15A_DNR_SET_USER(ac_bnr_ctrl_5, reg_bnr_ac_detail_th4,       GET_BITS(pp->ac_bnr[17], 0, 8));
				PE_NRD_HW_H15A_DNR_SET_USER(ac_bnr_ctrl_5, reg_bnr_ac_detail_th3,       GET_BITS(pp->ac_bnr[18], 0, 8));
				PE_NRD_HW_H15A_DNR_SET_USER(ac_bnr_ctrl_5, reg_bnr_ac_detail_th2,       GET_BITS(pp->ac_bnr[19], 0, 8));
				PE_NRD_HW_H15A_DNR_SET_USER(ac_bnr_ctrl_5, reg_bnr_ac_detail_th1,       GET_BITS(pp->ac_bnr[20], 0, 8));
				PE_NRD_HW_H15A_DNR_SET_USER(ac_bnr_ctrl_7, reg_bnr_ac_pos_gain_h3,      GET_BITS(pp->ac_bnr[21], 0, 8));
				PE_NRD_HW_H15A_DNR_SET_USER(ac_bnr_ctrl_6, reg_bnr_ac_pos_gain_h2,      GET_BITS(pp->ac_bnr[22], 0, 8));
				PE_NRD_HW_H15A_DNR_SET_USER(ac_bnr_ctrl_6, reg_bnr_ac_pos_gain_h1,      GET_BITS(pp->ac_bnr[23], 0, 8));
				PE_NRD_HW_H15A_DNR_SET_USER(ac_bnr_ctrl_6, reg_bnr_ac_pos_gain_h0,      GET_BITS(pp->ac_bnr[24], 0, 8));
				PE_NRD_HW_H15A_DNR_SET_USER(ac_bnr_ctrl_7, reg_bnr_ac_pos_gain_l2,      GET_BITS(pp->ac_bnr[25], 0, 8));
				PE_NRD_HW_H15A_DNR_SET_USER(ac_bnr_ctrl_7, reg_bnr_ac_pos_gain_l1,      GET_BITS(pp->ac_bnr[26], 0, 8));
				PE_NRD_HW_H15A_DNR_SET_USER(ac_bnr_ctrl_7, reg_bnr_ac_pos_gain_l0,      GET_BITS(pp->ac_bnr[27], 0, 8));
				PE_NRD_HW_H15A_DNR_SET_USER(mnr_ctrl_0,    reg_h_expand,                GET_BITS(pp->mosqnr[0],  0, 2));
				PE_NRD_HW_H15A_DNR_SET_USER(mnr_ctrl_0,    reg_gain_flt_size,           GET_BITS(pp->mosqnr[1],  0, 1));
				PE_NRD_HW_H15A_DNR_SET_USER(mnr_ctrl_1,    reg_mnr_s1_mmd_min,          GET_BITS(pp->mosqnr[2],  0, 8));
				PE_NRD_HW_H15A_DNR_SET_USER(mnr_ctrl_1,    reg_mnr_s2_ratio_min,        GET_BITS(pp->mosqnr[3],  0, 8));
				PE_NRD_HW_H15A_DNR_SET_USER(mnr_ctrl_1,    reg_mnr_s2_ratio_max,        GET_BITS(pp->mosqnr[4],  0, 8));
				PE_NRD_HW_H15A_DNR_SET_USER(mnr_ctrl_1,    reg_mnr_s2_mmd_min,          GET_BITS(pp->mosqnr[5],  0, 8));
				PE_NRD_HW_H15A_DNR_SET_USER(mnr_ctrl_3,    reg_filter_x0,               GET_BITS(pp->mosqnr[6],  0, 8));
				PE_NRD_HW_H15A_DNR_SET_USER(mnr_ctrl_3,    reg_filter_x1,               GET_BITS(pp->mosqnr[7],  0, 8));
				PE_NRD_HW_H15A_DNR_SET_USER(mnr_ctrl_3,    reg_filter_y0,               GET_BITS(pp->mosqnr[8],  0, 8));
				PE_NRD_HW_H15A_DNR_SET_USER(mnr_ctrl_3,    reg_filter_y1,               GET_BITS(pp->mosqnr[9],  0, 8));
				PE_NRD_HW_H15A_DNR_SET_USER(mnr_ctrl_4,    reg_motion_mnr_en,           GET_BITS(pp->mosqnr[10], 0, 1));
				PE_NRD_HW_H15A_DNR_SET_USER(mnr_ctrl_4,    reg_motion_mnr_filter,       GET_BITS(pp->mosqnr[11], 0, 2));
				PE_NRD_HW_H15A_DNR_SET_USER(mnr_ctrl_4,    reg_motion_mnr_filter_c,     GET_BITS(pp->mosqnr[12], 0, 2));
				PE_NRD_HW_H15A_DNR_SET_USER(mnr_ctrl_4,    reg_mnr_filter,              GET_BITS(pp->mosqnr[13], 0, 2));
				PE_NRD_HW_H15A_DNR_SET_USER(mnr_ctrl_4,    reg_mnr_filter_c,            GET_BITS(pp->mosqnr[14], 0, 2));
				PE_NRD_HW_H15A_DNR_SET_USER(mnr_ctrl_4,    reg_mnr_motion_min,          GET_BITS(pp->mosqnr[15], 0, 8));
				PE_NRD_HW_H15A_DNR_SET_USER(mnr_ctrl_4,    reg_mnr_motion_max,          GET_BITS(pp->mosqnr[16], 0, 8));
				if (PE_CMN_HW_H15_GetDbInfo(PE_CMN_HW_H15_DB_INF_MD0_DNR_FW_EN, 0))
				{
					memcpy(fw_param.dc_bnr, pp->dc_bnr, sizeof(UINT8)*PE_FWI_H15_DNR_DCB_SIZE);
					memcpy(fw_param.ac_bnr, pp->ac_bnr, sizeof(UINT8)*PE_FWI_H15_DNR_ACB_SIZE);
					ret = PE_FWI_H15_SetDnrDetail(&fw_param);
					PE_NRD_HW_H15_CHECK_CODE(ret, break, \
						"[%s,%d] PE_FWI_H15_SetDnrDetail() error.\n", __F__, __L__);
				}
			}
		}
		else
		{
			PE_NRD_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	#endif
	#endif
	return ret;
}
/**
 * get dnr detail ctrl
 * - use input struct LX_PE_NRD_DNR6_DETAIL_T
 *  016._[DC_BNR__]_0xC80071C4_[0____]_1_UINT8__dc_bnr[00]:_dc_bnr_ctrl_2,_reg_dc_var_en,
 *  017._[DC_BNR__]_0xC80071C4_[1____]_1_UINT8__dc_bnr[01]:_dc_bnr_ctrl_2,_reg_dc_motion_en,
 *  018._[DC_BNR__]_0xC80071C4_[2____]_1_UINT8__dc_bnr[02]:_dc_bnr_ctrl_2,_reg_dc_protection_en,
 *  019._[DC_BNR__]_0xC80071C4_[3____]_1_UINT8__dc_bnr[03]:_dc_bnr_ctrl_2,_reg_dc_detail_en,
 *  020._[DC_BNR__]_0xC80071BC_[3:2__]_2_UINT8__dc_bnr[04]:_dc_bnr_ctrl_0,_reg_dc_blur_sel,
 *  021._[DC_BNR__]_0xC80071BC_[23:16]_8_UINT8__dc_bnr[05]:_dc_bnr_ctrl_0,_reg_dc_motion_max,
 *  022._[DC_BNR__]_0xC80071BC_[31:24]_8_UINT8__dc_bnr[06]:_dc_bnr_ctrl_0,_reg_dc_motion_min,
 *  023._[DC_BNR__]_0xC80071C0_[7:0__]_8_UINT8__dc_bnr[07]:_dc_bnr_ctrl_1,_reg_dc_detail_max,
 *  024._[DC_BNR__]_0xC80071C0_[15:8_]_8_UINT8__dc_bnr[08]:_dc_bnr_ctrl_1,_reg_dc_detail_min,
 *  025._[DC_BNR__]_0xC80071C0_[23:20]_4_UINT8__dc_bnr[09]:_dc_bnr_ctrl_1,_reg_var_v_gain,
 *  026._[DC_BNR__]_0xC80071C0_[27:24]_4_UINT8__dc_bnr[10]:_dc_bnr_ctrl_1,_reg_var_h_gain,
 *  027._[DC_BNR__]_0xC80071C0_[31:28]_4_UINT8__dc_bnr[11]:_dc_bnr_ctrl_1,_reg_var_cut_resolution,
 *  028._[DC_BNR__]_0xC80071C4_[7:4__]_4_UINT8__dc_bnr[12]:_dc_bnr_ctrl_2,_reg_dc_global_motion_th,
 *  029._[DC_BNR__]_0xC80071C4_[31:24]_8_UINT8__dc_bnr[13]:_dc_bnr_ctrl_2,_reg_dc_protection_th,
 *  030._[DC_BNR__]_0xC80071C8_[7:0__]_8_UINT8__dc_bnr[14]:_dc_bnr_ctrl_3,_reg_dc_bnr_var_th3,
 *  031._[DC_BNR__]_0xC80071C8_[15:8_]_8_UINT8__dc_bnr[15]:_dc_bnr_ctrl_3,_reg_dc_bnr_var_th2,
 *  032._[DC_BNR__]_0xC80071C8_[23:16]_8_UINT8__dc_bnr[16]:_dc_bnr_ctrl_3,_reg_dc_bnr_var_th1,
 *  033._[DC_BNR__]_0xC80071C8_[31:24]_8_UINT8__dc_bnr[17]:_dc_bnr_ctrl_3,_reg_dc_bnr_var_th0,
 *  034._[DC_BNR__]_0xC80071C4_[8____]_1_UINT8__dc_bnr[18]:_dc_bnr_ctrl_2,_reg_detail_sel,
 *  035._[DETAIL__]_0xC80071B8_[7:0__]_8_UINT8__dc_bnr[19]:_detail_ctrl,___reg_bnr_ac_detail_max,
 *  036._[DETAIL__]_0xC80071B8_[15:8_]_8_UINT8__dc_bnr[20]:_detail_ctrl,___reg_bnr_ac_detail_min,
 *  037._[DETAIL__]_0xC80071B8_[23:16]_8_UINT8__dc_bnr[21]:_detail_ctrl,___reg_bnr_diff_l,
 *  038._[DETAIL__]_0xC80071B8_[31:24]_8_UINT8__dc_bnr[22]:_detail_ctrl,___reg_bnr_diff_p,
 *  039._[SC_BNR__]_0xC800726C_[0____]_1_UINT8__dc_bnr[23]:_sc_bnr_ctrl_0,_reg_sc_bnr_en,
 *  040._[SC_BNR__]_0xC8007280_[7:0__]_8_UINT8__dc_bnr[24]:_sc_bnr_ctrl_5,_reg_pos_gain_3,
 *  041._[SC_BNR__]_0xC8007280_[15:8_]_8_UINT8__dc_bnr[25]:_sc_bnr_ctrl_5,_reg_pos_gain_2,
 *  042._[SC_BNR__]_0xC8007280_[23:16]_8_UINT8__dc_bnr[26]:_sc_bnr_ctrl_5,_reg_pos_gain_1,
 *  043._[TEXTURE_]_0xC800728C_[2:0__]_3_UINT8__dc_bnr[27]:_texture_ctrl_0,reg_texture_cnt_div,
 *  044._[TEXTURE_]_0xC800728C_[15:8_]_8_UINT8__dc_bnr[28]:_texture_ctrl_0,reg_texture_cnt_mul,
 *  045._[TEXTURE_]_0xC800728C_[31:24]_8_UINT8__dc_bnr[29]:_texture_ctrl_0,reg_cross_th,
 *  046._[AC_BNR__]_0xC80071D4_[15:8_]_8_UINT8__ac_bnr[00]:_ac_bnr_ctrl_0,_reg_bnr_ac_diff_min_v_th,
 *  047._[AC_BNR__]_0xC80071D4_[23:16]_8_UINT8__ac_bnr[01]:_ac_bnr_ctrl_0,_reg_bnr_ac_diff_min_h_th,
 *  048._[AC_BNR__]_0xC80071D8_[7:0__]_8_UINT8__ac_bnr[02]:_ac_bnr_ctrl_1,_reg_bnr_ac_global_motion_th,
 *  049._[AC_BNR__]_0xC80071D8_[15:8_]_8_UINT8__ac_bnr[03]:_ac_bnr_ctrl_1,_reg_bnr_ac_h_acness_max,
 *  050._[AC_BNR__]_0xC80071E8_[23:16]_8_UINT8__ac_bnr[04]:_ac_bnr_ctrl_4,_reg_bnr_ac_v_acness_max,
 *  051._[AC_BNR__]_0xC80071D8_[23:16]_8_UINT8__ac_bnr[05]:_ac_bnr_ctrl_1,_reg_bnr_ac_h_acness_min,
 *  052._[AC_BNR__]_0xC80071E8_[31:24]_8_UINT8__ac_bnr[06]:_ac_bnr_ctrl_4,_reg_bnr_ac_v_acness_min,
 *  053._[AC_BNR__]_0xC80071E8_[7:6__]_2_UINT8__ac_bnr[07]:_ac_bnr_ctrl_4,_reg_bnr_ac_acness_resol_v,
 *  054._[AC_BNR__]_0xC80071DC_[7:0__]_8_UINT8__ac_bnr[08]:_ac_bnr_ctrl_2,_reg_bnr_ac_motion_3,
 *  055._[AC_BNR__]_0xC80071DC_[15:8_]_8_UINT8__ac_bnr[09]:_ac_bnr_ctrl_2,_reg_bnr_ac_motion_2,
 *  056._[AC_BNR__]_0xC80071DC_[23:16]_8_UINT8__ac_bnr[10]:_ac_bnr_ctrl_2,_reg_bnr_ac_motion_1,
 *  057._[AC_BNR__]_0xC80071DC_[31:24]_8_UINT8__ac_bnr[11]:_ac_bnr_ctrl_2,_reg_bnr_ac_motion_0,
 *  058._[AC_BNR__]_0xC80071E4_[7:0__]_8_UINT8__ac_bnr[12]:_ac_bnr_ctrl_3,_reg_bnr_ac_motion_y_3,
 *  059._[AC_BNR__]_0xC80071E4_[15:8_]_8_UINT8__ac_bnr[13]:_ac_bnr_ctrl_3,_reg_bnr_ac_motion_y_2,
 *  060._[AC_BNR__]_0xC80071E4_[23:16]_8_UINT8__ac_bnr[14]:_ac_bnr_ctrl_3,_reg_bnr_ac_motion_y_1,
 *  061._[AC_BNR__]_0xC80071E4_[31:24]_8_UINT8__ac_bnr[15]:_ac_bnr_ctrl_3,_reg_bnr_ac_motion_y_0,
 *  062._[AC_BNR__]_0xC80071F4_[31:24]_8_UINT8__ac_bnr[16]:_ac_bnr_ctrl_6,_reg_bnr_ac_detail_gain_th,
 *  063._[AC_BNR__]_0xC80071F0_[7:0__]_8_UINT8__ac_bnr[17]:_ac_bnr_ctrl_5,_reg_bnr_ac_detail_th4,
 *  064._[AC_BNR__]_0xC80071F0_[15:8_]_8_UINT8__ac_bnr[18]:_ac_bnr_ctrl_5,_reg_bnr_ac_detail_th3,
 *  065._[AC_BNR__]_0xC80071F0_[23:16]_8_UINT8__ac_bnr[19]:_ac_bnr_ctrl_5,_reg_bnr_ac_detail_th2,
 *  066._[AC_BNR__]_0xC80071F0_[31:24]_8_UINT8__ac_bnr[20]:_ac_bnr_ctrl_5,_reg_bnr_ac_detail_th1,
 *  067._[AC_BNR__]_0xC80071F8_[31:24]_8_UINT8__ac_bnr[21]:_ac_bnr_ctrl_7,_reg_bnr_ac_pos_gain_h3,
 *  068._[AC_BNR__]_0xC80071F4_[7:0__]_8_UINT8__ac_bnr[22]:_ac_bnr_ctrl_6,_reg_bnr_ac_pos_gain_h2,
 *  069._[AC_BNR__]_0xC80071F4_[15:8_]_8_UINT8__ac_bnr[23]:_ac_bnr_ctrl_6,_reg_bnr_ac_pos_gain_h1,
 *  070._[AC_BNR__]_0xC80071F4_[23:16]_8_UINT8__ac_bnr[24]:_ac_bnr_ctrl_6,_reg_bnr_ac_pos_gain_h0,
 *  071._[AC_BNR__]_0xC80071F8_[7:0__]_8_UINT8__ac_bnr[25]:_ac_bnr_ctrl_7,_reg_bnr_ac_pos_gain_l2,
 *  072._[AC_BNR__]_0xC80071F8_[15:8_]_8_UINT8__ac_bnr[26]:_ac_bnr_ctrl_7,_reg_bnr_ac_pos_gain_l1,
 *  073._[AC_BNR__]_0xC80071F8_[23:16]_8_UINT8__ac_bnr[27]:_ac_bnr_ctrl_7,_reg_bnr_ac_pos_gain_l0,
 *  074._[MNR_____]_0xC80071A0_[5:4__]_2_UINT8__mosqnr[00]:_mnr_ctrl_0,____reg_h_expand,
 *  075._[MNR_____]_0xC80071A0_[6____]_1_UINT8__mosqnr[01]:_mnr_ctrl_0,____reg_gain_flt_size,
 *  076._[MNR_____]_0xC80071A4_[7:0__]_8_UINT8__mosqnr[02]:_mnr_ctrl_1,____reg_mnr_s1_mmd_min,
 *  077._[MNR_____]_0xC80071A4_[15:8_]_8_UINT8__mosqnr[03]:_mnr_ctrl_1,____reg_mnr_s2_ratio_min,
 *  078._[MNR_____]_0xC80071A4_[23:16]_8_UINT8__mosqnr[04]:_mnr_ctrl_1,____reg_mnr_s2_ratio_max,
 *  079._[MNR_____]_0xC80071A4_[31:24]_8_UINT8__mosqnr[05]:_mnr_ctrl_1,____reg_mnr_s2_mmd_min,
 *  080._[MNR_____]_0xC80071AC_[7:0__]_8_UINT8__mosqnr[06]:_mnr_ctrl_3,____reg_filter_x0,
 *  081._[MNR_____]_0xC80071AC_[15:8_]_8_UINT8__mosqnr[07]:_mnr_ctrl_3,____reg_filter_x1,
 *  082._[MNR_____]_0xC80071AC_[23:16]_8_UINT8__mosqnr[08]:_mnr_ctrl_3,____reg_filter_y0,
 *  083._[MNR_____]_0xC80071AC_[31:24]_8_UINT8__mosqnr[09]:_mnr_ctrl_3,____reg_filter_y1,
 *  084._[MNR_____]_0xC80071B0_[0____]_1_UINT8__mosqnr[10]:_mnr_ctrl_4,____reg_motion_mnr_en,
 *  085._[MNR_____]_0xC80071B0_[2:1__]_2_UINT8__mosqnr[11]:_mnr_ctrl_4,____reg_motion_mnr_filter,
 *  086._[MNR_____]_0xC80071B0_[4:3__]_2_UINT8__mosqnr[12]:_mnr_ctrl_4,____reg_motion_mnr_filter_c,
 *  087._[MNR_____]_0xC80071B0_[6:5__]_2_UINT8__mosqnr[13]:_mnr_ctrl_4,____reg_mnr_filter,
 *  088._[MNR_____]_0xC80071B0_[8:7__]_2_UINT8__mosqnr[14]:_mnr_ctrl_4,____reg_mnr_filter_c,
 *  089._[MNR_____]_0xC80071B0_[23:16]_8_UINT8__mosqnr[15]:_mnr_ctrl_4,____reg_mnr_motion_min,
 *  090._[MNR_____]_0xC80071B0_[31:24]_8_UINT8__mosqnr[16]:_mnr_ctrl_4,____reg_mnr_motion_max,
 *
 * @param   *pstParams [in/out] void
 * @return  OK if success, ERROR otherwise.
 * @see		set discription
 * @author
 */
int PE_NRD_HW_H15_GetDnrDetailCtrl(void *pstParams)
{
	int ret = RET_OK;
	#ifdef PE_USE_H15_NEW_DTNR_STRUCT
	LX_PE_WIN_ID win_id;
	do{
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H15)
		{
			LX_PE_NRD_DNR6_DETAIL_T *pp=(LX_PE_NRD_DNR6_DETAIL_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			win_id = PE_GET_CHECKED_WINID(pp->win_id);
			if(PE_CHECK_WIN0(win_id))
			{
				PE_DNR1_H15A0_RdFL(dc_bnr_ctrl_2);
				PE_DNR1_H15A0_RdFL(dc_bnr_ctrl_0);
				PE_DNR1_H15A0_RdFL(dc_bnr_ctrl_1);
				PE_DNR1_H15A0_RdFL(dc_bnr_ctrl_3);
				PE_DNR1_H15A0_RdFL(detail_ctrl);
				PE_DNR1_H15A0_RdFL(sc_bnr_ctrl_0);
				PE_DNR1_H15A0_RdFL(sc_bnr_ctrl_5);
				PE_DNR1_H15A0_RdFL(texture_ctrl_0);
				PE_DNR1_H15A0_RdFL(ac_bnr_ctrl_0);
				PE_DNR1_H15A0_RdFL(ac_bnr_ctrl_1);
				PE_DNR1_H15A0_RdFL(ac_bnr_ctrl_4);
				PE_DNR1_H15A0_RdFL(ac_bnr_ctrl_2);
				PE_DNR1_H15A0_RdFL(ac_bnr_ctrl_3);
				PE_DNR1_H15A0_RdFL(ac_bnr_ctrl_6);
				PE_DNR1_H15A0_RdFL(ac_bnr_ctrl_5);
				PE_DNR1_H15A0_RdFL(ac_bnr_ctrl_7);
				PE_DNR1_H15A0_RdFL(mnr_ctrl_0);
				PE_DNR1_H15A0_RdFL(mnr_ctrl_1);
				PE_DNR1_H15A0_RdFL(mnr_ctrl_3);
				PE_DNR1_H15A0_RdFL(mnr_ctrl_4);
				PE_DNR1_H15A0_Rd01(dc_bnr_ctrl_2, reg_dc_var_en,               pp->dc_bnr[0]);
				PE_DNR1_H15A0_Rd01(dc_bnr_ctrl_2, reg_dc_motion_en,            pp->dc_bnr[1]);
				PE_DNR1_H15A0_Rd01(dc_bnr_ctrl_2, reg_dc_protection_en,        pp->dc_bnr[2]);
				PE_DNR1_H15A0_Rd01(dc_bnr_ctrl_2, reg_dc_detail_en,            pp->dc_bnr[3]);
				PE_DNR1_H15A0_Rd01(dc_bnr_ctrl_0, reg_dc_blur_sel,             pp->dc_bnr[4]);
				PE_DNR1_H15A0_Rd01(dc_bnr_ctrl_0, reg_dc_motion_max,           pp->dc_bnr[5]);
				PE_DNR1_H15A0_Rd01(dc_bnr_ctrl_0, reg_dc_motion_min,           pp->dc_bnr[6]);
				PE_DNR1_H15A0_Rd01(dc_bnr_ctrl_1, reg_dc_detail_max,           pp->dc_bnr[7]);
				PE_DNR1_H15A0_Rd01(dc_bnr_ctrl_1, reg_dc_detail_min,           pp->dc_bnr[8]);
				PE_DNR1_H15A0_Rd01(dc_bnr_ctrl_1, reg_var_v_gain,              pp->dc_bnr[9]);
				PE_DNR1_H15A0_Rd01(dc_bnr_ctrl_1, reg_var_h_gain,              pp->dc_bnr[10]);
				PE_DNR1_H15A0_Rd01(dc_bnr_ctrl_1, reg_var_cut_resolution,      pp->dc_bnr[11]);
				PE_DNR1_H15A0_Rd01(dc_bnr_ctrl_2, reg_dc_global_motion_th,     pp->dc_bnr[12]);
				PE_DNR1_H15A0_Rd01(dc_bnr_ctrl_2, reg_dc_protection_th,        pp->dc_bnr[13]);
				PE_DNR1_H15A0_Rd01(dc_bnr_ctrl_3, reg_dc_bnr_var_th3,          pp->dc_bnr[14]);
				PE_DNR1_H15A0_Rd01(dc_bnr_ctrl_3, reg_dc_bnr_var_th2,          pp->dc_bnr[15]);
				PE_DNR1_H15A0_Rd01(dc_bnr_ctrl_3, reg_dc_bnr_var_th1,          pp->dc_bnr[16]);
				PE_DNR1_H15A0_Rd01(dc_bnr_ctrl_3, reg_dc_bnr_var_th0,          pp->dc_bnr[17]);
				PE_DNR1_H15A0_Rd01(dc_bnr_ctrl_2, reg_detail_sel,              pp->dc_bnr[18]);
				PE_DNR1_H15A0_Rd01(detail_ctrl,   reg_bnr_ac_detail_max,       pp->dc_bnr[19]);
				PE_DNR1_H15A0_Rd01(detail_ctrl,   reg_bnr_ac_detail_min,       pp->dc_bnr[20]);
				PE_DNR1_H15A0_Rd01(detail_ctrl,   reg_bnr_diff_l,              pp->dc_bnr[21]);
				PE_DNR1_H15A0_Rd01(detail_ctrl,   reg_bnr_diff_p,              pp->dc_bnr[22]);
				PE_DNR1_H15A0_Rd01(sc_bnr_ctrl_0, reg_sc_bnr_en,               pp->dc_bnr[23]);
				PE_DNR1_H15A0_Rd01(sc_bnr_ctrl_5, reg_pos_gain_3,              pp->dc_bnr[24]);
				PE_DNR1_H15A0_Rd01(sc_bnr_ctrl_5, reg_pos_gain_2,              pp->dc_bnr[25]);
				PE_DNR1_H15A0_Rd01(sc_bnr_ctrl_5, reg_pos_gain_1,              pp->dc_bnr[26]);
				PE_DNR1_H15A0_Rd01(texture_ctrl_0,reg_texture_cnt_div,         pp->dc_bnr[27]);
				PE_DNR1_H15A0_Rd01(texture_ctrl_0,reg_texture_cnt_mul,         pp->dc_bnr[28]);
				PE_DNR1_H15A0_Rd01(texture_ctrl_0,reg_cross_th,                pp->dc_bnr[29]);
				PE_DNR1_H15A0_Rd01(ac_bnr_ctrl_0, reg_bnr_ac_diff_min_v_th,    pp->ac_bnr[0]);
				PE_DNR1_H15A0_Rd01(ac_bnr_ctrl_0, reg_bnr_ac_diff_min_h_th,    pp->ac_bnr[1]);
				PE_DNR1_H15A0_Rd01(ac_bnr_ctrl_1, reg_bnr_ac_global_motion_th, pp->ac_bnr[2]);
				PE_DNR1_H15A0_Rd01(ac_bnr_ctrl_1, reg_bnr_ac_h_acness_max,     pp->ac_bnr[3]);
				PE_DNR1_H15A0_Rd01(ac_bnr_ctrl_4, reg_bnr_ac_v_acness_max,     pp->ac_bnr[4]);
				PE_DNR1_H15A0_Rd01(ac_bnr_ctrl_1, reg_bnr_ac_h_acness_min,     pp->ac_bnr[5]);
				PE_DNR1_H15A0_Rd01(ac_bnr_ctrl_4, reg_bnr_ac_v_acness_min,     pp->ac_bnr[6]);
				PE_DNR1_H15A0_Rd01(ac_bnr_ctrl_4, reg_bnr_ac_acness_resol_v,   pp->ac_bnr[7]);
				PE_DNR1_H15A0_Rd01(ac_bnr_ctrl_2, reg_bnr_ac_motion_3,         pp->ac_bnr[8]);
				PE_DNR1_H15A0_Rd01(ac_bnr_ctrl_2, reg_bnr_ac_motion_2,         pp->ac_bnr[9]);
				PE_DNR1_H15A0_Rd01(ac_bnr_ctrl_2, reg_bnr_ac_motion_1,         pp->ac_bnr[10]);
				PE_DNR1_H15A0_Rd01(ac_bnr_ctrl_2, reg_bnr_ac_motion_0,         pp->ac_bnr[11]);
				PE_DNR1_H15A0_Rd01(ac_bnr_ctrl_3, reg_bnr_ac_motion_y_3,       pp->ac_bnr[12]);
				PE_DNR1_H15A0_Rd01(ac_bnr_ctrl_3, reg_bnr_ac_motion_y_2,       pp->ac_bnr[13]);
				PE_DNR1_H15A0_Rd01(ac_bnr_ctrl_3, reg_bnr_ac_motion_y_1,       pp->ac_bnr[14]);
				PE_DNR1_H15A0_Rd01(ac_bnr_ctrl_3, reg_bnr_ac_motion_y_0,       pp->ac_bnr[15]);
				PE_DNR1_H15A0_Rd01(ac_bnr_ctrl_6, reg_bnr_ac_detail_gain_th,   pp->ac_bnr[16]);
				PE_DNR1_H15A0_Rd01(ac_bnr_ctrl_5, reg_bnr_ac_detail_th4,       pp->ac_bnr[17]);
				PE_DNR1_H15A0_Rd01(ac_bnr_ctrl_5, reg_bnr_ac_detail_th3,       pp->ac_bnr[18]);
				PE_DNR1_H15A0_Rd01(ac_bnr_ctrl_5, reg_bnr_ac_detail_th2,       pp->ac_bnr[19]);
				PE_DNR1_H15A0_Rd01(ac_bnr_ctrl_5, reg_bnr_ac_detail_th1,       pp->ac_bnr[20]);
				PE_DNR1_H15A0_Rd01(ac_bnr_ctrl_7, reg_bnr_ac_pos_gain_h3,      pp->ac_bnr[21]);
				PE_DNR1_H15A0_Rd01(ac_bnr_ctrl_6, reg_bnr_ac_pos_gain_h2,      pp->ac_bnr[22]);
				PE_DNR1_H15A0_Rd01(ac_bnr_ctrl_6, reg_bnr_ac_pos_gain_h1,      pp->ac_bnr[23]);
				PE_DNR1_H15A0_Rd01(ac_bnr_ctrl_6, reg_bnr_ac_pos_gain_h0,      pp->ac_bnr[24]);
				PE_DNR1_H15A0_Rd01(ac_bnr_ctrl_7, reg_bnr_ac_pos_gain_l2,      pp->ac_bnr[25]);
				PE_DNR1_H15A0_Rd01(ac_bnr_ctrl_7, reg_bnr_ac_pos_gain_l1,      pp->ac_bnr[26]);
				PE_DNR1_H15A0_Rd01(ac_bnr_ctrl_7, reg_bnr_ac_pos_gain_l0,      pp->ac_bnr[27]);
				PE_DNR1_H15A0_Rd01(mnr_ctrl_0,    reg_h_expand,                pp->mosqnr[0]);
				PE_DNR1_H15A0_Rd01(mnr_ctrl_0,    reg_gain_flt_size,           pp->mosqnr[1]);
				PE_DNR1_H15A0_Rd01(mnr_ctrl_1,    reg_mnr_s1_mmd_min,          pp->mosqnr[2]);
				PE_DNR1_H15A0_Rd01(mnr_ctrl_1,    reg_mnr_s2_ratio_min,        pp->mosqnr[3]);
				PE_DNR1_H15A0_Rd01(mnr_ctrl_1,    reg_mnr_s2_ratio_max,        pp->mosqnr[4]);
				PE_DNR1_H15A0_Rd01(mnr_ctrl_1,    reg_mnr_s2_mmd_min,          pp->mosqnr[5]);
				PE_DNR1_H15A0_Rd01(mnr_ctrl_3,    reg_filter_x0,               pp->mosqnr[6]);
				PE_DNR1_H15A0_Rd01(mnr_ctrl_3,    reg_filter_x1,               pp->mosqnr[7]);
				PE_DNR1_H15A0_Rd01(mnr_ctrl_3,    reg_filter_y0,               pp->mosqnr[8]);
				PE_DNR1_H15A0_Rd01(mnr_ctrl_3,    reg_filter_y1,               pp->mosqnr[9]);
				PE_DNR1_H15A0_Rd01(mnr_ctrl_4,    reg_motion_mnr_en,           pp->mosqnr[10]);
				PE_DNR1_H15A0_Rd01(mnr_ctrl_4,    reg_motion_mnr_filter,       pp->mosqnr[11]);
				PE_DNR1_H15A0_Rd01(mnr_ctrl_4,    reg_motion_mnr_filter_c,     pp->mosqnr[12]);
				PE_DNR1_H15A0_Rd01(mnr_ctrl_4,    reg_mnr_filter,              pp->mosqnr[13]);
				PE_DNR1_H15A0_Rd01(mnr_ctrl_4,    reg_mnr_filter_c,            pp->mosqnr[14]);
				PE_DNR1_H15A0_Rd01(mnr_ctrl_4,    reg_mnr_motion_min,          pp->mosqnr[15]);
				PE_DNR1_H15A0_Rd01(mnr_ctrl_4,    reg_mnr_motion_max,          pp->mosqnr[16]);
			}
			if(PE_CHECK_WIN1(win_id))
			{
				PE_DNR2_H15A0_RdFL(dc_bnr_ctrl_2);
				PE_DNR2_H15A0_RdFL(dc_bnr_ctrl_0);
				PE_DNR2_H15A0_RdFL(dc_bnr_ctrl_1);
				PE_DNR2_H15A0_RdFL(dc_bnr_ctrl_3);
				PE_DNR2_H15A0_RdFL(detail_ctrl);
				PE_DNR2_H15A0_RdFL(sc_bnr_ctrl_0);
				PE_DNR2_H15A0_RdFL(sc_bnr_ctrl_5);
				PE_DNR2_H15A0_RdFL(texture_ctrl_0);
				PE_DNR2_H15A0_RdFL(ac_bnr_ctrl_0);
				PE_DNR2_H15A0_RdFL(ac_bnr_ctrl_1);
				PE_DNR2_H15A0_RdFL(ac_bnr_ctrl_4);
				PE_DNR2_H15A0_RdFL(ac_bnr_ctrl_2);
				PE_DNR2_H15A0_RdFL(ac_bnr_ctrl_3);
				PE_DNR2_H15A0_RdFL(ac_bnr_ctrl_6);
				PE_DNR2_H15A0_RdFL(ac_bnr_ctrl_5);
				PE_DNR2_H15A0_RdFL(ac_bnr_ctrl_7);
				PE_DNR2_H15A0_RdFL(mnr_ctrl_0);
				PE_DNR2_H15A0_RdFL(mnr_ctrl_1);
				PE_DNR2_H15A0_RdFL(mnr_ctrl_3);
				PE_DNR2_H15A0_RdFL(mnr_ctrl_4);
				PE_DNR2_H15A0_Rd01(dc_bnr_ctrl_2, reg_dc_var_en,               pp->dc_bnr[0]);
				PE_DNR2_H15A0_Rd01(dc_bnr_ctrl_2, reg_dc_motion_en,            pp->dc_bnr[1]);
				PE_DNR2_H15A0_Rd01(dc_bnr_ctrl_2, reg_dc_protection_en,        pp->dc_bnr[2]);
				PE_DNR2_H15A0_Rd01(dc_bnr_ctrl_2, reg_dc_detail_en,            pp->dc_bnr[3]);
				PE_DNR2_H15A0_Rd01(dc_bnr_ctrl_0, reg_dc_blur_sel,             pp->dc_bnr[4]);
				PE_DNR2_H15A0_Rd01(dc_bnr_ctrl_0, reg_dc_motion_max,           pp->dc_bnr[5]);
				PE_DNR2_H15A0_Rd01(dc_bnr_ctrl_0, reg_dc_motion_min,           pp->dc_bnr[6]);
				PE_DNR2_H15A0_Rd01(dc_bnr_ctrl_1, reg_dc_detail_max,           pp->dc_bnr[7]);
				PE_DNR2_H15A0_Rd01(dc_bnr_ctrl_1, reg_dc_detail_min,           pp->dc_bnr[8]);
				PE_DNR2_H15A0_Rd01(dc_bnr_ctrl_1, reg_var_v_gain,              pp->dc_bnr[9]);
				PE_DNR2_H15A0_Rd01(dc_bnr_ctrl_1, reg_var_h_gain,              pp->dc_bnr[10]);
				PE_DNR2_H15A0_Rd01(dc_bnr_ctrl_1, reg_var_cut_resolution,      pp->dc_bnr[11]);
				PE_DNR2_H15A0_Rd01(dc_bnr_ctrl_2, reg_dc_global_motion_th,     pp->dc_bnr[12]);
				PE_DNR2_H15A0_Rd01(dc_bnr_ctrl_2, reg_dc_protection_th,        pp->dc_bnr[13]);
				PE_DNR2_H15A0_Rd01(dc_bnr_ctrl_3, reg_dc_bnr_var_th3,          pp->dc_bnr[14]);
				PE_DNR2_H15A0_Rd01(dc_bnr_ctrl_3, reg_dc_bnr_var_th2,          pp->dc_bnr[15]);
				PE_DNR2_H15A0_Rd01(dc_bnr_ctrl_3, reg_dc_bnr_var_th1,          pp->dc_bnr[16]);
				PE_DNR2_H15A0_Rd01(dc_bnr_ctrl_3, reg_dc_bnr_var_th0,          pp->dc_bnr[17]);
				PE_DNR2_H15A0_Rd01(dc_bnr_ctrl_2, reg_detail_sel,              pp->dc_bnr[18]);
				PE_DNR2_H15A0_Rd01(detail_ctrl,   reg_bnr_ac_detail_max,       pp->dc_bnr[19]);
				PE_DNR2_H15A0_Rd01(detail_ctrl,   reg_bnr_ac_detail_min,       pp->dc_bnr[20]);
				PE_DNR2_H15A0_Rd01(detail_ctrl,   reg_bnr_diff_l,              pp->dc_bnr[21]);
				PE_DNR2_H15A0_Rd01(detail_ctrl,   reg_bnr_diff_p,              pp->dc_bnr[22]);
				PE_DNR2_H15A0_Rd01(sc_bnr_ctrl_0, reg_sc_bnr_en,               pp->dc_bnr[23]);
				PE_DNR2_H15A0_Rd01(sc_bnr_ctrl_5, reg_pos_gain_3,              pp->dc_bnr[24]);
				PE_DNR2_H15A0_Rd01(sc_bnr_ctrl_5, reg_pos_gain_2,              pp->dc_bnr[25]);
				PE_DNR2_H15A0_Rd01(sc_bnr_ctrl_5, reg_pos_gain_1,              pp->dc_bnr[26]);
				PE_DNR2_H15A0_Rd01(texture_ctrl_0,reg_texture_cnt_div,         pp->dc_bnr[27]);
				PE_DNR2_H15A0_Rd01(texture_ctrl_0,reg_texture_cnt_mul,         pp->dc_bnr[28]);
				PE_DNR2_H15A0_Rd01(texture_ctrl_0,reg_cross_th,                pp->dc_bnr[29]);
				PE_DNR2_H15A0_Rd01(ac_bnr_ctrl_0, reg_bnr_ac_diff_min_v_th,    pp->ac_bnr[0]);
				PE_DNR2_H15A0_Rd01(ac_bnr_ctrl_0, reg_bnr_ac_diff_min_h_th,    pp->ac_bnr[1]);
				PE_DNR2_H15A0_Rd01(ac_bnr_ctrl_1, reg_bnr_ac_global_motion_th, pp->ac_bnr[2]);
				PE_DNR2_H15A0_Rd01(ac_bnr_ctrl_1, reg_bnr_ac_h_acness_max,     pp->ac_bnr[3]);
				PE_DNR2_H15A0_Rd01(ac_bnr_ctrl_4, reg_bnr_ac_v_acness_max,     pp->ac_bnr[4]);
				PE_DNR2_H15A0_Rd01(ac_bnr_ctrl_1, reg_bnr_ac_h_acness_min,     pp->ac_bnr[5]);
				PE_DNR2_H15A0_Rd01(ac_bnr_ctrl_4, reg_bnr_ac_v_acness_min,     pp->ac_bnr[6]);
				PE_DNR2_H15A0_Rd01(ac_bnr_ctrl_4, reg_bnr_ac_acness_resol_v,   pp->ac_bnr[7]);
				PE_DNR2_H15A0_Rd01(ac_bnr_ctrl_2, reg_bnr_ac_motion_3,         pp->ac_bnr[8]);
				PE_DNR2_H15A0_Rd01(ac_bnr_ctrl_2, reg_bnr_ac_motion_2,         pp->ac_bnr[9]);
				PE_DNR2_H15A0_Rd01(ac_bnr_ctrl_2, reg_bnr_ac_motion_1,         pp->ac_bnr[10]);
				PE_DNR2_H15A0_Rd01(ac_bnr_ctrl_2, reg_bnr_ac_motion_0,         pp->ac_bnr[11]);
				PE_DNR2_H15A0_Rd01(ac_bnr_ctrl_3, reg_bnr_ac_motion_y_3,       pp->ac_bnr[12]);
				PE_DNR2_H15A0_Rd01(ac_bnr_ctrl_3, reg_bnr_ac_motion_y_2,       pp->ac_bnr[13]);
				PE_DNR2_H15A0_Rd01(ac_bnr_ctrl_3, reg_bnr_ac_motion_y_1,       pp->ac_bnr[14]);
				PE_DNR2_H15A0_Rd01(ac_bnr_ctrl_3, reg_bnr_ac_motion_y_0,       pp->ac_bnr[15]);
				PE_DNR2_H15A0_Rd01(ac_bnr_ctrl_6, reg_bnr_ac_detail_gain_th,   pp->ac_bnr[16]);
				PE_DNR2_H15A0_Rd01(ac_bnr_ctrl_5, reg_bnr_ac_detail_th4,       pp->ac_bnr[17]);
				PE_DNR2_H15A0_Rd01(ac_bnr_ctrl_5, reg_bnr_ac_detail_th3,       pp->ac_bnr[18]);
				PE_DNR2_H15A0_Rd01(ac_bnr_ctrl_5, reg_bnr_ac_detail_th2,       pp->ac_bnr[19]);
				PE_DNR2_H15A0_Rd01(ac_bnr_ctrl_5, reg_bnr_ac_detail_th1,       pp->ac_bnr[20]);
				PE_DNR2_H15A0_Rd01(ac_bnr_ctrl_7, reg_bnr_ac_pos_gain_h3,      pp->ac_bnr[21]);
				PE_DNR2_H15A0_Rd01(ac_bnr_ctrl_6, reg_bnr_ac_pos_gain_h2,      pp->ac_bnr[22]);
				PE_DNR2_H15A0_Rd01(ac_bnr_ctrl_6, reg_bnr_ac_pos_gain_h1,      pp->ac_bnr[23]);
				PE_DNR2_H15A0_Rd01(ac_bnr_ctrl_6, reg_bnr_ac_pos_gain_h0,      pp->ac_bnr[24]);
				PE_DNR2_H15A0_Rd01(ac_bnr_ctrl_7, reg_bnr_ac_pos_gain_l2,      pp->ac_bnr[25]);
				PE_DNR2_H15A0_Rd01(ac_bnr_ctrl_7, reg_bnr_ac_pos_gain_l1,      pp->ac_bnr[26]);
				PE_DNR2_H15A0_Rd01(ac_bnr_ctrl_7, reg_bnr_ac_pos_gain_l0,      pp->ac_bnr[27]);
				PE_DNR2_H15A0_Rd01(mnr_ctrl_0,    reg_h_expand,                pp->mosqnr[0]);
				PE_DNR2_H15A0_Rd01(mnr_ctrl_0,    reg_gain_flt_size,           pp->mosqnr[1]);
				PE_DNR2_H15A0_Rd01(mnr_ctrl_1,    reg_mnr_s1_mmd_min,          pp->mosqnr[2]);
				PE_DNR2_H15A0_Rd01(mnr_ctrl_1,    reg_mnr_s2_ratio_min,        pp->mosqnr[3]);
				PE_DNR2_H15A0_Rd01(mnr_ctrl_1,    reg_mnr_s2_ratio_max,        pp->mosqnr[4]);
				PE_DNR2_H15A0_Rd01(mnr_ctrl_1,    reg_mnr_s2_mmd_min,          pp->mosqnr[5]);
				PE_DNR2_H15A0_Rd01(mnr_ctrl_3,    reg_filter_x0,               pp->mosqnr[6]);
				PE_DNR2_H15A0_Rd01(mnr_ctrl_3,    reg_filter_x1,               pp->mosqnr[7]);
				PE_DNR2_H15A0_Rd01(mnr_ctrl_3,    reg_filter_y0,               pp->mosqnr[8]);
				PE_DNR2_H15A0_Rd01(mnr_ctrl_3,    reg_filter_y1,               pp->mosqnr[9]);
				PE_DNR2_H15A0_Rd01(mnr_ctrl_4,    reg_motion_mnr_en,           pp->mosqnr[10]);
				PE_DNR2_H15A0_Rd01(mnr_ctrl_4,    reg_motion_mnr_filter,       pp->mosqnr[11]);
				PE_DNR2_H15A0_Rd01(mnr_ctrl_4,    reg_motion_mnr_filter_c,     pp->mosqnr[12]);
				PE_DNR2_H15A0_Rd01(mnr_ctrl_4,    reg_mnr_filter,              pp->mosqnr[13]);
				PE_DNR2_H15A0_Rd01(mnr_ctrl_4,    reg_mnr_filter_c,            pp->mosqnr[14]);
				PE_DNR2_H15A0_Rd01(mnr_ctrl_4,    reg_mnr_motion_min,          pp->mosqnr[15]);
				PE_DNR2_H15A0_Rd01(mnr_ctrl_4,    reg_mnr_motion_max,          pp->mosqnr[16]);
			}
			if (_g_nrd_hw_h15_trace)
			{
				UINT8 *pd = pp->dc_bnr;
				PE_NRD_HW_H15_DBG_PRINT("[%d]:\n"\
					"dc_bnr[00]0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,\n"\
					"dc_bnr[10]0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,\n"\
					"dc_bnr[20]0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,\n", \
					pp->win_id, \
					pd[0],  pd[1],  pd[2],  pd[3],  pd[4],  pd[5],  pd[6],  pd[7],  pd[8],  pd[9], \
					pd[10], pd[11], pd[12], pd[13], pd[14], pd[15], pd[16], pd[17], pd[18], pd[19], \
					pd[20], pd[21], pd[22], pd[23], pd[24], pd[25], pd[26], pd[27], pd[28], pd[29]);
				pd = pp->ac_bnr;
				PE_NRD_HW_H15_DBG_PRINT("[%d]:\n"\
					"ac_bnr[00]0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,\n"\
					"ac_bnr[10]0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,\n"\
					"ac_bnr[20]0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,\n", \
					pp->win_id, \
					pd[0],  pd[1],  pd[2],  pd[3],  pd[4],  pd[5],  pd[6],  pd[7],  pd[8],  pd[9], \
					pd[10], pd[11], pd[12], pd[13], pd[14], pd[15], pd[16], pd[17], pd[18], pd[19], \
					pd[20], pd[21], pd[22], pd[23], pd[24], pd[25], pd[26], pd[27]);
				pd = pp->mosqnr;
				PE_NRD_HW_H15_DBG_PRINT("[%d]:\n"\
					"mosqnr[00]0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,\n"\
					"mosqnr[10]0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,\n", \
					pp->win_id, \
					pd[0],  pd[1],  pd[2],  pd[3],  pd[4],  pd[5],  pd[6],  pd[7],  pd[8],  pd[9], \
					pd[10], pd[11], pd[12], pd[13], pd[14], pd[15], pd[16]);
			}
		}
		else
		{
			PE_NRD_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	#endif
	return ret;
}

/**
 * set tnr common ctrl
 * - use input struct LX_PE_NRD_TNR5_CMN_T
 *  001._[TNR_MC__]_0xC8004140_[0____]_UINT8__tnr_en_____:__________________reg_tnr_en
 *  002._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_lm[00]_:_tnr_main_lut_01,_eg_detail_iir_pt_luma_76,
 *  003._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_lm[01]_:_tnr_main_lut_01,_eg_detail_iir_pt_luma_54,
 *  004._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_lm[02]_:_tnr_main_lut_01,_eg_detail_iir_pt_luma_32,
 *  005._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_lm[03]_:_tnr_main_lut_01,_eg_detail_iir_pt_luma_10,
 *  006._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_lm[04]_:_tnr_main_lut_01,_eg_detail_iir_pt_chroma_76,
 *  007._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_lm[05]_:_tnr_main_lut_01,_eg_detail_iir_pt_chroma_54,
 *  008._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_lm[06]_:_tnr_main_lut_01,_eg_detail_iir_pt_chroma_32,
 *  009._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_lm[07]_:_tnr_main_lut_01,_eg_detail_iir_pt_chroma_10,
 *  010._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_lm[08]_:_tnr_main_lut_01,_reg_flat_iir_pt_luma_76,
 *  011._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_lm[09]_:_tnr_main_lut_01,_reg_flat_iir_pt_luma_54,
 *  012._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_lm[10]_:_tnr_main_lut_01,_reg_flat_iir_pt_luma_32,
 *  013._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_lm[11]_:_tnr_main_lut_01,_reg_flat_iir_pt_luma_10,
 *  014._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_lm[12]_:_tnr_main_lut_01,_reg_flat_iir_pt_chroma_76,
 *  015._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_lm[13]_:_tnr_main_lut_01,_reg_flat_iir_pt_chroma_54,
 *  016._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_lm[14]_:_tnr_main_lut_01,_reg_flat_iir_pt_chroma_32,
 *  017._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_lm[15]_:_tnr_main_lut_01,_reg_flat_iir_pt_chroma_10,
 *  018._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_ui[00]_:_tnr_main_lut_01,_reg_detail_iir_pt_luma_76,
 *  019._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_ui[01]_:_tnr_main_lut_01,_reg_detail_iir_pt_luma_54,
 *  020._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_ui[02]_:_tnr_main_lut_01,_reg_detail_iir_pt_luma_32,
 *  021._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_ui[03]_:_tnr_main_lut_01,_reg_detail_iir_pt_luma_10,
 *  022._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_ui[04]_:_tnr_main_lut_01,_reg_detail_iir_pt_chroma_76,
 *  023._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_ui[05]_:_tnr_main_lut_01,_reg_detail_iir_pt_chroma_54,
 *  024._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_ui[06]_:_tnr_main_lut_01,_reg_detail_iir_pt_chroma_32,
 *  025._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_ui[07]_:_tnr_main_lut_01,_reg_detail_iir_pt_chroma_10,
 *  026._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_ui[08]_:_tnr_main_lut_01,_reg_flat_iir_pt_luma_76,
 *  027._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_ui[09]_:_tnr_main_lut_01,_reg_flat_iir_pt_luma_54,
 *  028._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_ui[10]_:_tnr_main_lut_01,_reg_flat_iir_pt_luma_32,
 *  029._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_ui[11]_:_tnr_main_lut_01,_reg_flat_iir_pt_luma_10,
 *  030._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_ui[12]_:_tnr_main_lut_01,_reg_flat_iir_pt_chroma_76,
 *  031._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_ui[13]_:_tnr_main_lut_01,_reg_flat_iir_pt_chroma_54,
 *  032._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_ui[14]_:_tnr_main_lut_01,_reg_flat_iir_pt_chroma_32,
 *  033._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_ui[15]_:_tnr_main_lut_01,_reg_flat_iir_pt_chroma_10,
 *  034._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_hm[00]_:_tnr_main_lut_01,_reg_detail_iir_pt_luma_76,
 *  035._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_hm[01]_:_tnr_main_lut_01,_reg_detail_iir_pt_luma_54,
 *  036._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_hm[02]_:_tnr_main_lut_01,_reg_detail_iir_pt_luma_32,
 *  037._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_hm[03]_:_tnr_main_lut_01,_reg_detail_iir_pt_luma_10,
 *  038._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_hm[04]_:_tnr_main_lut_01,_reg_detail_iir_pt_chroma_76,
 *  039._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_hm[05]_:_tnr_main_lut_01,_reg_detail_iir_pt_chroma_54,
 *  040._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_hm[06]_:_tnr_main_lut_01,_reg_detail_iir_pt_chroma_32,
 *  041._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_hm[07]_:_tnr_main_lut_01,_reg_detail_iir_pt_chroma_10,
 *  042._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_hm[08]_:_tnr_main_lut_01,_reg_flat_iir_pt_luma_76,
 *  043._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_hm[09]_:_tnr_main_lut_01,_reg_flat_iir_pt_luma_54,
 *  044._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_hm[10]_:_tnr_main_lut_01,_reg_flat_iir_pt_luma_32,
 *  045._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_hm[11]_:_tnr_main_lut_01,_reg_flat_iir_pt_luma_10,
 *  046._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_hm[12]_:_tnr_main_lut_01,_reg_flat_iir_pt_chroma_76,
 *  047._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_hm[13]_:_tnr_main_lut_01,_reg_flat_iir_pt_chroma_54,
 *  048._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_hm[14]_:_tnr_main_lut_01,_reg_flat_iir_pt_chroma_32,
 *  049._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_hm[15]_:_tnr_main_lut_01,_reg_flat_iir_pt_chroma_10,
 *
 * @param   *pstParams [in] void
 * @return  OK if success, ERROR otherwise.
 * @see		set discription
 * @author
 */
int PE_NRD_HW_H15_SetTnrCmnCtrl(void *pstParams)
{
	int ret = RET_OK;
	#ifdef PE_USE_H15_NEW_DTNR_STRUCT
	const PE_REG_PARAM_T *p_db = NULL;
	PE_INF_H15_SETTINGS_T inf_set;
	LX_PE_INF_DISPLAY_T *disp0_inf = NULL;
	PE_TNL_HW_PARAM_REG_H15A0_T *p_tnl_dflt = _g_pe_tnl_hw_param_data_h15a.reg_dflt.data;
	PE_NRD_HW_H15_SETTINGS_T *pInfo = &_g_pe_nrd_hw_h15_info;
	#define H15_TNL_DFT(_num)	(p_tnl_dflt->tnr_lut_##_num.main_lut_indir_data0)
	do{
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H15)
		{
			LX_PE_NRD_TNR5_CMN_T *pp=(LX_PE_NRD_TNR5_CMN_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			if (_g_nrd_hw_h15_trace)
			{
				UINT32 *pd = pp->lut_lm;
				PE_NRD_HW_H15_DBG_PRINT("[%d]tnr_en:%d\n", pp->win_id, pp->tnr_en);
				PE_NRD_HW_H15_DBG_PRINT("[%d]:\n"\
					"lut_lm[00]0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,\n"\
					"lut_lm[08]0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,\n", \
					pp->win_id, \
					pd[0],  pd[1],  pd[2],  pd[3],  pd[4],  pd[5],  pd[6],  pd[7], \
					pd[8],  pd[9], pd[10], pd[11], pd[12], pd[13], pd[14], pd[15]);
				pd = pp->lut_ui;
				PE_NRD_HW_H15_DBG_PRINT("[%d]:\n"\
					"lut_ui[00]0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,\n"\
					"lut_ui[08]0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,\n", \
					pp->win_id, \
					pd[0],  pd[1],  pd[2],  pd[3],  pd[4],  pd[5],  pd[6],  pd[7], \
					pd[8],  pd[9], pd[10], pd[11], pd[12], pd[13], pd[14], pd[15]);
				pd = pp->lut_hm;
				PE_NRD_HW_H15_DBG_PRINT("[%d]:\n"\
					"lut_hm[00]0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,\n"\
					"lut_hm[08]0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,\n", \
					pp->win_id, \
					pd[0],  pd[1],  pd[2],  pd[3],  pd[4],  pd[5],  pd[6],  pd[7], \
					pd[8],  pd[9], pd[10], pd[11], pd[12], pd[13], pd[14], pd[15]);
			}
			if(PE_CHECK_WIN0(pp->win_id))
			{
				/* skip drv tnr ctrl, if fw tnr on. */
				if (PE_CMN_HW_H15_GetDbInfo(PE_CMN_HW_H15_DB_INF_MD0_TNR_FW_EN, 0))
				{
					/* save prev values */
					memcpy(&(pInfo->tnr_cmn), pp, sizeof(LX_PE_NRD_TNR5_CMN_T));
					/* download tnr user db */
					ret = PE_NRD_HW_H15_DownloadTnrUserDb((void *)pp);
					PE_NRD_HW_H15_CHECK_CODE(ret, break, \
						"[%s,%d] PE_NRD_HW_H15_DownloadTnrUserDb() error.\n",__F__,__L__);
				}
				else
				{
					UINT32 *p_lut = pp->lut_ui;
					if (pp->tnr_en == 3)					//high
					{
						p_db = tnr_l_lut_user_high_h15a0;
					}
					else if (pp->tnr_en == 2 || pp->tnr_en == 4)//medium, auto
					{
						p_db = tnr_l_lut_user_med_h15a0;
					}
					else if (pp->tnr_en == 1)				//low
					{
						p_db = tnr_l_lut_user_low_h15a0;
					}
					else									//off
					{
						p_db = tnr_l_lut_user_off_h15a0;
					}
					CHECK_KNULL(p_db);
					ret = PE_INF_H15_GetCurInfSettings(&inf_set);
					PE_NRD_HW_H15_CHECK_CODE(ret,break,"[%s,%d] PE_INF_H15_GetCurInfSettings() error.\n",__F__,__L__);
					disp0_inf=&inf_set.disp_info[LX_PE_WIN_0];
					PE_ND12_H15A0_QWr(tnr_main_lut_00, 0x00001000);
					if (pp->tnr_en==0)	//off
					{
						PE_NRD_HW_H15A_TNL_WR32(00, p_db[0].data );// 00. x7y7 x6y6 reg_p2_blend_ctrl_pt
						PE_NRD_HW_H15A_TNL_WR32(01, p_db[1].data );// 01. x5y5 x4y4 reg_p2_blend_ctrl_pt
						PE_NRD_HW_H15A_TNL_WR32(02, p_db[2].data );// 02. x3y3 x2y2 reg_p2_blend_ctrl_pt
						PE_NRD_HW_H15A_TNL_WR32(03, p_db[3].data );// 03. x1y1 x0y0 reg_p2_blend_ctrl_pt
						PE_NRD_HW_H15A_TNL_WR32(04, p_db[4].data );// 04. x5y5 x4y4 reg_p2_dist_max_pt
						PE_NRD_HW_H15A_TNL_WR32(05, p_db[5].data );// 05. x3y3 x2y2 reg_p2_dist_max_pt
						PE_NRD_HW_H15A_TNL_WR32(06, p_db[6].data );// 06. x1y1 x0y0 reg_p2_dist_max_pt
						PE_NRD_HW_H15A_TNL_WR32(07, p_lut[0]     );// 07. x7y7 x6y6 reg_detail_iir_pt
						PE_NRD_HW_H15A_TNL_WR32(08, p_lut[1]     );// 08. x5y5 x4y4 reg_detail_iir_pt
						PE_NRD_HW_H15A_TNL_WR32(09, p_lut[2]     );// 09. x3y3 x2y2 reg_detail_iir_pt
						PE_NRD_HW_H15A_TNL_WR32(10, p_lut[3]     );// 10. x1y1 x0y0 reg_detail_iir_pt
						PE_NRD_HW_H15A_TNL_WR32(11, p_lut[4]     );// 11. x7y7 x6y6 reg_detail_cir_pt
						PE_NRD_HW_H15A_TNL_WR32(12, p_lut[5]     );// 12. x5y5 x4y4 reg_detail_cir_pt
						PE_NRD_HW_H15A_TNL_WR32(13, p_lut[6]     );// 13. x3y3 x2y2 reg_detail_cir_pt
						PE_NRD_HW_H15A_TNL_WR32(14, p_lut[7]     );// 14. x1y1 x0y0 reg_detail_cir_pt
						PE_NRD_HW_H15A_TNL_WR32(15, p_lut[8]     );// 15. x7y7 x6y6 reg_flat_iir_pt
						PE_NRD_HW_H15A_TNL_WR32(16, p_lut[9]     );// 16. x5y5 x4y4 reg_flat_iir_pt
						PE_NRD_HW_H15A_TNL_WR32(17, p_lut[10]    );// 17. x3y3 x2y2 reg_flat_iir_pt
						PE_NRD_HW_H15A_TNL_WR32(18, p_lut[11]    );// 18. x1y1 x0y0 reg_flat_iir_pt
						PE_NRD_HW_H15A_TNL_WR32(19, p_lut[12]    );// 19. x7y7 x6y6 reg_flat_cir_pt
						PE_NRD_HW_H15A_TNL_WR32(20, p_lut[13]    );// 20. x5y5 x4y4 reg_flat_cir_pt
						PE_NRD_HW_H15A_TNL_WR32(21, p_lut[14]    );// 21. x3y3 x2y2 reg_flat_cir_pt
						PE_NRD_HW_H15A_TNL_WR32(22, p_lut[15]    );// 22. x1y1 x0y0 reg_flat_cir_pt
						PE_NRD_HW_H15A_TNL_WR32(23, p_db[23].data);// 23. x1y1 x0y0 reg_luma_gain_ctrl
						PE_NRD_HW_H15A_TNL_WR32(24, p_db[24].data);// 24. x3x2 x1x0 reg_skin_gain_ctrl_yy
						PE_NRD_HW_H15A_TNL_WR32(25, p_db[25].data);// 25. x3x2 x1x0 reg_skin_gain_ctrl_cb
						PE_NRD_HW_H15A_TNL_WR32(26, p_db[26].data);// 26. x3x2 x1x0 reg_skin_gain_ctrl_cr
						PE_NRD_HW_H15A_TNL_WR32(27, p_db[27].data);// 27. x1y1 x0y0 reg_skin_motion_ctrl
						PE_NRD_HW_H15A_TNL_WR32(28, p_db[28].data);// 28. reg_detail_luma_ctrl
						PE_NRD_HW_H15A_TNL_WR32(29, p_db[29].data);// 29. x3y3 x2y2 reg_NLM_Protect
						PE_NRD_HW_H15A_TNL_WR32(30, p_db[30].data);// 30. x1y1 x0y0 reg_NLM_Protect
						PE_NRD_HW_H15A_TNL_WR32(31, p_db[31].data);// 31. x5y5 x4y4 reg_cc_dist_max_pt
						PE_NRD_HW_H15A_TNL_WR32(32, p_db[32].data);// 32. x3y3 x2y2 reg_cc_dist_max_pt
						PE_NRD_HW_H15A_TNL_WR32(33, p_db[33].data);// 33. x1y1 x0y0 reg_cc_dist_max_pt
						PE_NRD_HW_H15A_TNL_WR32(34, p_db[34].data);// 34. x5y5 x4y4 reg_ne_dist_max_pt
						PE_NRD_HW_H15A_TNL_WR32(35, p_db[35].data);// 35. x3y3 x2y2 reg_ne_dist_max_pt
						PE_NRD_HW_H15A_TNL_WR32(36, p_db[36].data);// 36. x1y1 x0y0 reg_ne_dist_max_pt
						PE_NRD_HW_H15A_TNL_WR32(37, p_db[37].data);// 37. reserved
						PE_NRD_HW_H15A_TNL_WR32(38, p_db[38].data);// 38. reserved
						PE_NRD_HW_H15A_TNL_WR32(39, p_db[39].data);// 39. x7y7 x6y6 reg_simple_ma_iir_pt
						PE_NRD_HW_H15A_TNL_WR32(40, p_db[40].data);// 40. x5y5 x4y4 reg_simple_ma_iir_pt
						PE_NRD_HW_H15A_TNL_WR32(41, p_db[41].data);// 41. x3y3 x2y2 reg_simple_ma_iir_pt
						PE_NRD_HW_H15A_TNL_WR32(42, p_db[42].data);// 42. x1y1 x0y0 reg_simple_ma_iir_pt
					}
					else
					{
						if(disp0_inf->src_type==LX_PE_SRC_DTV && \
							disp0_inf->fmt_type==LX_PE_FMT_HD && \
							disp0_inf->dtv_type==LX_PE_DTV_CAMERA)
						{
							PE_NRD_HW_H15A_TNL_WR32(00, H15_TNL_DFT(00));// 00. x7y7 x6y6 reg_p2_blend_ctrl_pt
							PE_NRD_HW_H15A_TNL_WR32(01, H15_TNL_DFT(01));// 01. x5y5 x4y4 reg_p2_blend_ctrl_pt
							PE_NRD_HW_H15A_TNL_WR32(02, H15_TNL_DFT(02));// 02. x3y3 x2y2 reg_p2_blend_ctrl_pt
							PE_NRD_HW_H15A_TNL_WR32(03, H15_TNL_DFT(03));// 03. x1y1 x0y0 reg_p2_blend_ctrl_pt
							PE_NRD_HW_H15A_TNL_WR32(04, H15_TNL_DFT(04));// 04. x5y5 x4y4 reg_p2_dist_max_pt
							PE_NRD_HW_H15A_TNL_WR32(05, H15_TNL_DFT(05));// 05. x3y3 x2y2 reg_p2_dist_max_pt
							PE_NRD_HW_H15A_TNL_WR32(06, H15_TNL_DFT(06));// 06. x1y1 x0y0 reg_p2_dist_max_pt
							PE_NRD_HW_H15A_TNL_WR32(07, H15_TNL_DFT(07));// 07. x7y7 x6y6 reg_detail_iir_pt
							PE_NRD_HW_H15A_TNL_WR32(08, H15_TNL_DFT(08));// 08. x5y5 x4y4 reg_detail_iir_pt
							PE_NRD_HW_H15A_TNL_WR32(09, H15_TNL_DFT(09));// 09. x3y3 x2y2 reg_detail_iir_pt
							PE_NRD_HW_H15A_TNL_WR32(10, H15_TNL_DFT(10));// 10. x1y1 x0y0 reg_detail_iir_pt
							PE_NRD_HW_H15A_TNL_WR32(11, H15_TNL_DFT(11));// 11. x7y7 x6y6 reg_detail_cir_pt
							PE_NRD_HW_H15A_TNL_WR32(12, H15_TNL_DFT(12));// 12. x5y5 x4y4 reg_detail_cir_pt
							PE_NRD_HW_H15A_TNL_WR32(13, H15_TNL_DFT(13));// 13. x3y3 x2y2 reg_detail_cir_pt
							PE_NRD_HW_H15A_TNL_WR32(14, H15_TNL_DFT(14));// 14. x1y1 x0y0 reg_detail_cir_pt
							PE_NRD_HW_H15A_TNL_WR32(15, H15_TNL_DFT(15));// 15. x7y7 x6y6 reg_flat_iir_pt
							PE_NRD_HW_H15A_TNL_WR32(16, H15_TNL_DFT(16));// 16. x5y5 x4y4 reg_flat_iir_pt
							PE_NRD_HW_H15A_TNL_WR32(17, H15_TNL_DFT(17));// 17. x3y3 x2y2 reg_flat_iir_pt
							PE_NRD_HW_H15A_TNL_WR32(18, H15_TNL_DFT(18));// 18. x1y1 x0y0 reg_flat_iir_pt
							PE_NRD_HW_H15A_TNL_WR32(19, H15_TNL_DFT(19));// 19. x7y7 x6y6 reg_flat_cir_pt
							PE_NRD_HW_H15A_TNL_WR32(20, H15_TNL_DFT(20));// 20. x5y5 x4y4 reg_flat_cir_pt
							PE_NRD_HW_H15A_TNL_WR32(21, H15_TNL_DFT(21));// 21. x3y3 x2y2 reg_flat_cir_pt
							PE_NRD_HW_H15A_TNL_WR32(22, H15_TNL_DFT(22));// 22. x1y1 x0y0 reg_flat_cir_pt
							PE_NRD_HW_H15A_TNL_WR32(23, H15_TNL_DFT(23));// 23. x1y1 x0y0 reg_luma_gain_ctrl
							PE_NRD_HW_H15A_TNL_WR32(24, H15_TNL_DFT(24));// 24. x3x2 x1x0 reg_skin_gain_ctrl_yy
							PE_NRD_HW_H15A_TNL_WR32(25, H15_TNL_DFT(25));// 25. x3x2 x1x0 reg_skin_gain_ctrl_cb
							PE_NRD_HW_H15A_TNL_WR32(26, H15_TNL_DFT(26));// 26. x3x2 x1x0 reg_skin_gain_ctrl_cr
							PE_NRD_HW_H15A_TNL_WR32(27, H15_TNL_DFT(27));// 27. x1y1 x0y0 reg_skin_motion_ctrl
							PE_NRD_HW_H15A_TNL_WR32(28, H15_TNL_DFT(28));// 28. reg_detail_luma_ctrl
							PE_NRD_HW_H15A_TNL_WR32(29, H15_TNL_DFT(29));// 29. x3y3 x2y2 reg_NLM_Protect
							PE_NRD_HW_H15A_TNL_WR32(30, H15_TNL_DFT(30));// 30. x1y1 x0y0 reg_NLM_Protect
							PE_NRD_HW_H15A_TNL_WR32(31, H15_TNL_DFT(31));// 31. x5y5 x4y4 reg_cc_dist_max_pt
							PE_NRD_HW_H15A_TNL_WR32(32, H15_TNL_DFT(32));// 32. x3y3 x2y2 reg_cc_dist_max_pt
							PE_NRD_HW_H15A_TNL_WR32(33, H15_TNL_DFT(33));// 33. x1y1 x0y0 reg_cc_dist_max_pt
							PE_NRD_HW_H15A_TNL_WR32(34, H15_TNL_DFT(34));// 34. x5y5 x4y4 reg_ne_dist_max_pt
							PE_NRD_HW_H15A_TNL_WR32(35, H15_TNL_DFT(35));// 35. x3y3 x2y2 reg_ne_dist_max_pt
							PE_NRD_HW_H15A_TNL_WR32(36, H15_TNL_DFT(36));// 36. x1y1 x0y0 reg_ne_dist_max_pt
							PE_NRD_HW_H15A_TNL_WR32(37, H15_TNL_DFT(37));// 37. reserved
							PE_NRD_HW_H15A_TNL_WR32(38, H15_TNL_DFT(38));// 38. reserved
							PE_NRD_HW_H15A_TNL_WR32(39, p_db[39].data  );// 39. x7y7 x6y6 reg_simple_ma_iir_pt
							PE_NRD_HW_H15A_TNL_WR32(40, p_db[40].data  );// 40. x5y5 x4y4 reg_simple_ma_iir_pt
							PE_NRD_HW_H15A_TNL_WR32(41, p_db[41].data  );// 41. x3y3 x2y2 reg_simple_ma_iir_pt
							PE_NRD_HW_H15A_TNL_WR32(42, p_db[42].data  );// 42. x1y1 x0y0 reg_simple_ma_iir_pt
						}
						else
						{
							PE_NRD_HW_H15A_TNL_WR32(00, H15_TNL_DFT(00));// 00. x7y7 x6y6 reg_p2_blend_ctrl_pt
							PE_NRD_HW_H15A_TNL_WR32(01, H15_TNL_DFT(01));// 01. x5y5 x4y4 reg_p2_blend_ctrl_pt
							PE_NRD_HW_H15A_TNL_WR32(02, H15_TNL_DFT(02));// 02. x3y3 x2y2 reg_p2_blend_ctrl_pt
							PE_NRD_HW_H15A_TNL_WR32(03, H15_TNL_DFT(03));// 03. x1y1 x0y0 reg_p2_blend_ctrl_pt
							PE_NRD_HW_H15A_TNL_WR32(04, H15_TNL_DFT(04));// 04. x5y5 x4y4 reg_p2_dist_max_pt
							PE_NRD_HW_H15A_TNL_WR32(05, H15_TNL_DFT(05));// 05. x3y3 x2y2 reg_p2_dist_max_pt
							PE_NRD_HW_H15A_TNL_WR32(06, H15_TNL_DFT(06));// 06. x1y1 x0y0 reg_p2_dist_max_pt
							PE_NRD_HW_H15A_TNL_WR32(07, p_lut[0]       );// 07. x7y7 x6y6 reg_detail_iir_pt
							PE_NRD_HW_H15A_TNL_WR32(08, p_lut[1]       );// 08. x5y5 x4y4 reg_detail_iir_pt
							PE_NRD_HW_H15A_TNL_WR32(09, p_lut[2]       );// 09. x3y3 x2y2 reg_detail_iir_pt
							PE_NRD_HW_H15A_TNL_WR32(10, p_lut[3]       );// 10. x1y1 x0y0 reg_detail_iir_pt
							PE_NRD_HW_H15A_TNL_WR32(11, p_lut[4]       );// 11. x7y7 x6y6 reg_detail_cir_pt
							PE_NRD_HW_H15A_TNL_WR32(12, p_lut[5]       );// 12. x5y5 x4y4 reg_detail_cir_pt
							PE_NRD_HW_H15A_TNL_WR32(13, p_lut[6]       );// 13. x3y3 x2y2 reg_detail_cir_pt
							PE_NRD_HW_H15A_TNL_WR32(14, p_lut[7]       );// 14. x1y1 x0y0 reg_detail_cir_pt
							PE_NRD_HW_H15A_TNL_WR32(15, p_lut[8]       );// 15. x7y7 x6y6 reg_flat_iir_pt
							PE_NRD_HW_H15A_TNL_WR32(16, p_lut[9]       );// 16. x5y5 x4y4 reg_flat_iir_pt
							PE_NRD_HW_H15A_TNL_WR32(17, p_lut[10]      );// 17. x3y3 x2y2 reg_flat_iir_pt
							PE_NRD_HW_H15A_TNL_WR32(18, p_lut[11]      );// 18. x1y1 x0y0 reg_flat_iir_pt
							PE_NRD_HW_H15A_TNL_WR32(19, p_lut[12]      );// 19. x7y7 x6y6 reg_flat_cir_pt
							PE_NRD_HW_H15A_TNL_WR32(20, p_lut[13]      );// 20. x5y5 x4y4 reg_flat_cir_pt
							PE_NRD_HW_H15A_TNL_WR32(21, p_lut[14]      );// 21. x3y3 x2y2 reg_flat_cir_pt
							PE_NRD_HW_H15A_TNL_WR32(22, p_lut[15]      );// 22. x1y1 x0y0 reg_flat_cir_pt
							PE_NRD_HW_H15A_TNL_WR32(23, H15_TNL_DFT(23));// 23. x1y1 x0y0 reg_luma_gain_ctrl
							PE_NRD_HW_H15A_TNL_WR32(24, H15_TNL_DFT(24));// 24. x3x2 x1x0 reg_skin_gain_ctrl_yy
							PE_NRD_HW_H15A_TNL_WR32(25, H15_TNL_DFT(25));// 25. x3x2 x1x0 reg_skin_gain_ctrl_cb
							PE_NRD_HW_H15A_TNL_WR32(26, H15_TNL_DFT(26));// 26. x3x2 x1x0 reg_skin_gain_ctrl_cr
							PE_NRD_HW_H15A_TNL_WR32(27, H15_TNL_DFT(27));// 27. x1y1 x0y0 reg_skin_motion_ctrl
							PE_NRD_HW_H15A_TNL_WR32(28, H15_TNL_DFT(28));// 28. reg_detail_luma_ctrl
							PE_NRD_HW_H15A_TNL_WR32(29, H15_TNL_DFT(29));// 29. x3y3 x2y2 reg_NLM_Protect
							PE_NRD_HW_H15A_TNL_WR32(30, H15_TNL_DFT(30));// 30. x1y1 x0y0 reg_NLM_Protect
							PE_NRD_HW_H15A_TNL_WR32(31, H15_TNL_DFT(31));// 31. x5y5 x4y4 reg_cc_dist_max_pt
							PE_NRD_HW_H15A_TNL_WR32(32, H15_TNL_DFT(32));// 32. x3y3 x2y2 reg_cc_dist_max_pt
							PE_NRD_HW_H15A_TNL_WR32(33, H15_TNL_DFT(33));// 33. x1y1 x0y0 reg_cc_dist_max_pt
							PE_NRD_HW_H15A_TNL_WR32(34, H15_TNL_DFT(34));// 34. x5y5 x4y4 reg_ne_dist_max_pt
							PE_NRD_HW_H15A_TNL_WR32(35, H15_TNL_DFT(35));// 35. x3y3 x2y2 reg_ne_dist_max_pt
							PE_NRD_HW_H15A_TNL_WR32(36, H15_TNL_DFT(36));// 36. x1y1 x0y0 reg_ne_dist_max_pt
							PE_NRD_HW_H15A_TNL_WR32(37, H15_TNL_DFT(37));// 37. reserved
							PE_NRD_HW_H15A_TNL_WR32(38, H15_TNL_DFT(38));// 38. reserved
							PE_NRD_HW_H15A_TNL_WR32(39, p_db[39].data  );// 39. x7y7 x6y6 reg_simple_ma_iir_pt
							PE_NRD_HW_H15A_TNL_WR32(40, p_db[40].data  );// 40. x5y5 x4y4 reg_simple_ma_iir_pt
							PE_NRD_HW_H15A_TNL_WR32(41, p_db[41].data  );// 41. x3y3 x2y2 reg_simple_ma_iir_pt
							PE_NRD_HW_H15A_TNL_WR32(42, p_db[42].data  );// 42. x1y1 x0y0 reg_simple_ma_iir_pt
						}
					}
					PE_ND12_H15A0_QWr(tnr_main_lut_00, 0x00008000);
					/* save prev values */
					memcpy(&(pInfo->tnr_cmn), pp, sizeof(LX_PE_NRD_TNR5_CMN_T));
					/* download tnr user db */
					ret = PE_NRD_HW_H15_DownloadTnrUserDb((void *)pp);
					PE_NRD_HW_H15_CHECK_CODE(ret, break, \
						"[%s,%d] PE_NRD_HW_H15_DownloadTnrUserDb() error.\n",__F__,__L__);
				}
			}
		}
		else
		{
			PE_NRD_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	#endif
	return ret;
}
/**
 * get tnr common ctrl
 * - use input struct LX_PE_NRD_TNR5_CMN_T
 *  001._[TNR_MC__]_0xC8004140_[0____]_UINT8__tnr_en_____:__________________reg_tnr_en
 *  002._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_lm[00]_:_tnr_main_lut_01,_eg_detail_iir_pt_luma_76,
 *  003._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_lm[01]_:_tnr_main_lut_01,_eg_detail_iir_pt_luma_54,
 *  004._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_lm[02]_:_tnr_main_lut_01,_eg_detail_iir_pt_luma_32,
 *  005._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_lm[03]_:_tnr_main_lut_01,_eg_detail_iir_pt_luma_10,
 *  006._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_lm[04]_:_tnr_main_lut_01,_eg_detail_iir_pt_chroma_76,
 *  007._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_lm[05]_:_tnr_main_lut_01,_eg_detail_iir_pt_chroma_54,
 *  008._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_lm[06]_:_tnr_main_lut_01,_eg_detail_iir_pt_chroma_32,
 *  009._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_lm[07]_:_tnr_main_lut_01,_eg_detail_iir_pt_chroma_10,
 *  010._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_lm[08]_:_tnr_main_lut_01,_reg_flat_iir_pt_luma_76,
 *  011._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_lm[09]_:_tnr_main_lut_01,_reg_flat_iir_pt_luma_54,
 *  012._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_lm[10]_:_tnr_main_lut_01,_reg_flat_iir_pt_luma_32,
 *  013._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_lm[11]_:_tnr_main_lut_01,_reg_flat_iir_pt_luma_10,
 *  014._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_lm[12]_:_tnr_main_lut_01,_reg_flat_iir_pt_chroma_76,
 *  015._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_lm[13]_:_tnr_main_lut_01,_reg_flat_iir_pt_chroma_54,
 *  016._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_lm[14]_:_tnr_main_lut_01,_reg_flat_iir_pt_chroma_32,
 *  017._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_lm[15]_:_tnr_main_lut_01,_reg_flat_iir_pt_chroma_10,
 *  018._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_ui[00]_:_tnr_main_lut_01,_reg_detail_iir_pt_luma_76,
 *  019._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_ui[01]_:_tnr_main_lut_01,_reg_detail_iir_pt_luma_54,
 *  020._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_ui[02]_:_tnr_main_lut_01,_reg_detail_iir_pt_luma_32,
 *  021._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_ui[03]_:_tnr_main_lut_01,_reg_detail_iir_pt_luma_10,
 *  022._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_ui[04]_:_tnr_main_lut_01,_reg_detail_iir_pt_chroma_76,
 *  023._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_ui[05]_:_tnr_main_lut_01,_reg_detail_iir_pt_chroma_54,
 *  024._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_ui[06]_:_tnr_main_lut_01,_reg_detail_iir_pt_chroma_32,
 *  025._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_ui[07]_:_tnr_main_lut_01,_reg_detail_iir_pt_chroma_10,
 *  026._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_ui[08]_:_tnr_main_lut_01,_reg_flat_iir_pt_luma_76,
 *  027._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_ui[09]_:_tnr_main_lut_01,_reg_flat_iir_pt_luma_54,
 *  028._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_ui[10]_:_tnr_main_lut_01,_reg_flat_iir_pt_luma_32,
 *  029._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_ui[11]_:_tnr_main_lut_01,_reg_flat_iir_pt_luma_10,
 *  030._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_ui[12]_:_tnr_main_lut_01,_reg_flat_iir_pt_chroma_76,
 *  031._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_ui[13]_:_tnr_main_lut_01,_reg_flat_iir_pt_chroma_54,
 *  032._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_ui[14]_:_tnr_main_lut_01,_reg_flat_iir_pt_chroma_32,
 *  033._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_ui[15]_:_tnr_main_lut_01,_reg_flat_iir_pt_chroma_10,
 *  034._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_hm[00]_:_tnr_main_lut_01,_reg_detail_iir_pt_luma_76,
 *  035._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_hm[01]_:_tnr_main_lut_01,_reg_detail_iir_pt_luma_54,
 *  036._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_hm[02]_:_tnr_main_lut_01,_reg_detail_iir_pt_luma_32,
 *  037._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_hm[03]_:_tnr_main_lut_01,_reg_detail_iir_pt_luma_10,
 *  038._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_hm[04]_:_tnr_main_lut_01,_reg_detail_iir_pt_chroma_76,
 *  039._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_hm[05]_:_tnr_main_lut_01,_reg_detail_iir_pt_chroma_54,
 *  040._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_hm[06]_:_tnr_main_lut_01,_reg_detail_iir_pt_chroma_32,
 *  041._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_hm[07]_:_tnr_main_lut_01,_reg_detail_iir_pt_chroma_10,
 *  042._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_hm[08]_:_tnr_main_lut_01,_reg_flat_iir_pt_luma_76,
 *  043._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_hm[09]_:_tnr_main_lut_01,_reg_flat_iir_pt_luma_54,
 *  044._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_hm[10]_:_tnr_main_lut_01,_reg_flat_iir_pt_luma_32,
 *  045._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_hm[11]_:_tnr_main_lut_01,_reg_flat_iir_pt_luma_10,
 *  046._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_hm[12]_:_tnr_main_lut_01,_reg_flat_iir_pt_chroma_76,
 *  047._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_hm[13]_:_tnr_main_lut_01,_reg_flat_iir_pt_chroma_54,
 *  048._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_hm[14]_:_tnr_main_lut_01,_reg_flat_iir_pt_chroma_32,
 *  049._[TNR_MC__]_0xc80041d4_[31:0_]_UINT32_lut_hm[15]_:_tnr_main_lut_01,_reg_flat_iir_pt_chroma_10,
 *
 * @param   *pstParams [in/out] void
 * @return  OK if success, ERROR otherwise.
 * @see		set discription
 * @author
 */
int PE_NRD_HW_H15_GetTnrCmnCtrl(void *pstParams)
{
	int ret = RET_OK;
	#ifdef PE_USE_H15_NEW_DTNR_STRUCT
	LX_PE_WIN_ID win_id;
	PE_NRD_HW_H15_SETTINGS_T *pInfo = &_g_pe_nrd_hw_h15_info;
	do{
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H15)
		{
			LX_PE_NRD_TNR5_CMN_T *pp=(LX_PE_NRD_TNR5_CMN_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			win_id = PE_GET_CHECKED_WINID(pp->win_id);
			if(PE_CHECK_WIN0(win_id))
			{
				memcpy(pp, &(pInfo->tnr_cmn), sizeof(LX_PE_NRD_TNR5_CMN_T));
			}
			if(PE_CHECK_WIN1(win_id))
			{
				memcpy(pp, &(pInfo->tnr_cmn), sizeof(LX_PE_NRD_TNR5_CMN_T));
			}
			if (_g_nrd_hw_h15_trace)
			{
				UINT32 *pd = pp->lut_lm;
				PE_NRD_HW_H15_DBG_PRINT("[%d]tnr_en:%d\n", pp->win_id, pp->tnr_en);
				PE_NRD_HW_H15_DBG_PRINT("[%d]:\n"\
					"lut_lm[00]0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,\n"\
					"lut_lm[08]0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,\n", \
					pp->win_id, \
					pd[0],  pd[1],  pd[2],  pd[3],  pd[4],  pd[5],  pd[6],  pd[7], \
					pd[8],  pd[9], pd[10], pd[11], pd[12], pd[13], pd[14], pd[15]);
				pd = pp->lut_ui;
				PE_NRD_HW_H15_DBG_PRINT("[%d]:\n"\
					"lut_ui[00]0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,\n"\
					"lut_ui[08]0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,\n", \
					pp->win_id, \
					pd[0],  pd[1],  pd[2],  pd[3],  pd[4],  pd[5],  pd[6],  pd[7], \
					pd[8],  pd[9], pd[10], pd[11], pd[12], pd[13], pd[14], pd[15]);
				pd = pp->lut_hm;
				PE_NRD_HW_H15_DBG_PRINT("[%d]:\n"\
					"lut_hm[00]0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,\n"\
					"lut_hm[08]0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,\n", \
					pp->win_id, \
					pd[0],  pd[1],  pd[2],  pd[3],  pd[4],  pd[5],  pd[6],  pd[7], \
					pd[8],  pd[9], pd[10], pd[11], pd[12], pd[13], pd[14], pd[15]);
			}
		}
		else
		{
			PE_NRD_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	#endif
	return ret;
}

/**
 * set tnr detail ctrl
 * - use input struct LX_PE_NRD_TNR5_DETAIL_T
 *  050._[TNR_ME__]_0xC8004140_[7:6__]_2_UINT8__s_m[00]_:_tnr_ctrl_00,_reg_sad_8x3_res,
 *  051._[TNR_ME__]_0xC8004140_[8____]_1_UINT8__s_m[01]_:_tnr_ctrl_00,_reg_sad_8x3_iir_en,
 *  052._[TNR_ME__]_0xC8004140_[31:24]_8_UINT8__s_m[02]_:_tnr_ctrl_00,_reg_sad_8x3_iir_alpha,
 *  053._[TNR_SC__]_0xC8004148_[0____]_1_UINT8__s_m[03]_:_tnr_ctrl_02,_sc_en,
 *  054._[TNR_SC__]_0xC8004148_[23:16]_8_UINT8__s_m[04]_:_tnr_ctrl_02,_sc_th_ma,
 *  055._[TNR_SM__]_0xc8004164_[6:0__]_7_UINT8__s_m[05]_:_tnr_ctrl_09,_sad_base,
 *  056._[TNR_SM__]_0xc800416C_[0____]_1_UINT8__s_m[06]_:_tnr_ctrl_11,_lb_smooth_en,
 *  057._[TNR_SM__]_0xc8004188_[2:0__]_3_UINT8__s_m[07]_:_tnr_ctrl_18,_bsad_scale,
 *  058._[TNR_SM__]_0xc8004188_[6:4__]_3_UINT8__s_m[08]_:_tnr_ctrl_18,_gsad_scale,
 *  059._[TNR_SM__]_0xc8004188_[15___]_1_UINT8__s_m[09]_:_tnr_ctrl_18,_gmv_gmv_tl_en,
 *  060._[TNR_SM__]_0xc8004188_[23___]_1_UINT8__s_m[10]_:_tnr_ctrl_18,_bmv_gmv_tl_en,
 *  061._[TNR_SM__]_0xc8004188_[31___]_1_UINT8__s_m[11]_:_tnr_ctrl_18,_bmv_pmv_tl_en,
 *  062._[TNR_SM__]_0xc8004188_[14:8_]_7_UINT8__s_m[12]_:_tnr_ctrl_18,_gmv_gmv_tl,
 *  063._[TNR_SM__]_0xc8004188_[22:16]_7_UINT8__s_m[13]_:_tnr_ctrl_18,_bmv_gmv_tl,
 *  064._[TNR_SM__]_0xc8004188_[30:24]_7_UINT8__s_m[14]_:_tnr_ctrl_18,_bmv_pmv_tl,
 *  065._[TNR_SM__]_0xc800418c_[23___]_1_UINT8__s_m[15]_:_tnr_ctrl_19,_gmv_0mv_tl_en,
 *  066._[TNR_SM__]_0xc800418c_[31___]_1_UINT8__s_m[16]_:_tnr_ctrl_19,_bmv_0mv_tl_en,
 *  067._[TNR_SM__]_0xc800418c_[22:16]_7_UINT8__s_m[17]_:_tnr_ctrl_19,_gmv_0mv_tl,
 *  068._[TNR_SM__]_0xc800418c_[30:24]_7_UINT8__s_m[18]_:_tnr_ctrl_19,_bmv_0mv_tl,
 *  069._[TNR_SM__]_0xc8004164_[13:8_]_6_UINT8__s_m[19]_:_tnr_ctrl_09,_lb_smooth_clip,
 *  070._[TNR_SM__]_0xc8004158_[7:0__]_8_UINT8__s_m[20]_:_tnr_ctrl_06,_me_lb_sad_base,
 *  071._[TNR_SM__]_0xc800416C_[1____]_1_UINT8__s_m[21]_:_tnr_ctrl_11,_pv_smooth_en,
 *  072._[TNR_SM__]_0xc8004164_[21:16]_6_UINT8__s_m[22]_:_tnr_ctrl_09,_pv_smooth_clip,
 *  073._[TNR_SM__]_0xc8004158_[15:8_]_8_UINT8__s_m[23]_:_tnr_ctrl_06,_pv_smooth_sad_base,
 *  074._[TNR_EDF_]_0xc8004190_[31:24]_8_UINT8__edf[00]_:_tnr_ctrl_20,_reg_edge_alpha_mul,
 *  075._[TNR_EDF_]_0xc8004190_[23:16]_8_UINT8__edf[01]_:_tnr_ctrl_20,_reg_edge_beta_mul,
 *  076._[TNR_EDF_]_0xc8004190_[15:8_]_8_UINT8__edf[02]_:_tnr_ctrl_20,_reg_detail_alpha_mul,
 *  077._[TNR_EDF_]_0xc8004190_[7:0__]_8_UINT8__edf[03]_:_tnr_ctrl_20,_reg_detail_beta_mul,
 *  078._[TNR_EDF_]_0xc80043d0_[5:4__]_2_UINT8__edf[04]_:_tnr_ctrl_40,_reg_t_cut_resolution,
 *  079._[TNR_EDF_]_0xc80043d8_[31:24]_8_UINT8__edf[05]_:_tnr_ctrl_42,_reg_a_lut_edge_x1,
 *  080._[TNR_EDF_]_0xc80043d8_[23:16]_8_UINT8__edf[06]_:_tnr_ctrl_42,_reg_a_lut_edge_y1,
 *  081._[TNR_EDF_]_0xc80043d8_[15:8_]_8_UINT8__edf[07]_:_tnr_ctrl_42,_reg_a_lut_edge_x0,
 *  082._[TNR_EDF_]_0xc80043d8_[7:0__]_8_UINT8__edf[08]_:_tnr_ctrl_42,_reg_a_lut_edge_y0,
 *  083._[TNR_EDF_]_0xc80043dc_[31:24]_8_UINT8__edf[09]_:_tnr_ctrl_43,_reg_a_lut_detail_x1,
 *  084._[TNR_EDF_]_0xc80043dc_[23:16]_8_UINT8__edf[10]_:_tnr_ctrl_43,_reg_a_lut_detail_y1,
 *  085._[TNR_EDF_]_0xc80043dc_[15:8_]_8_UINT8__edf[11]_:_tnr_ctrl_43,_reg_a_lut_detail_x0,
 *  086._[TNR_EDF_]_0xc80043dc_[7:0__]_8_UINT8__edf[12]_:_tnr_ctrl_43,_reg_a_lut_detail_y0,
 *  087._[TNR_EDF_]_0xc80043e0_[31:24]_8_UINT8__edf[13]_:_tnr_ctrl_44,_reg_t_lut_edge_x1,
 *  088._[TNR_EDF_]_0xc80043e0_[23:16]_8_UINT8__edf[14]_:_tnr_ctrl_44,_reg_t_lut_edge_y1,
 *  089._[TNR_EDF_]_0xc80043e0_[15:8_]_8_UINT8__edf[15]_:_tnr_ctrl_44,_reg_t_lut_edge_x0,
 *  090._[TNR_EDF_]_0xc80043e0_[7:0__]_8_UINT8__edf[16]_:_tnr_ctrl_44,_reg_t_lut_edge_y0,
 *  091._[TNR_EDF_]_0xc80043e4_[31:24]_8_UINT8__edf[17]_:_tnr_ctrl_45,_reg_t_lut_detail_x1,
 *  092._[TNR_EDF_]_0xc80043e4_[23:16]_8_UINT8__edf[18]_:_tnr_ctrl_45,_reg_t_lut_detail_y1,
 *  093._[TNR_EDF_]_0xc80043e4_[15:8_]_8_UINT8__edf[19]_:_tnr_ctrl_45,_reg_t_lut_detail_x0,
 *  094._[TNR_EDF_]_0xc80043e4_[7:0__]_8_UINT8__edf[20]_:_tnr_ctrl_45,_reg_t_lut_detail_y0,
 *  095._[TNR_EDF_]_0xc80043e8_[31:24]_8_UINT8__edf[21]_:_tnr_ctrl_46,_reg_final_lut_edge_x1,
 *  096._[TNR_EDF_]_0xc80043e8_[23:16]_8_UINT8__edf[22]_:_tnr_ctrl_46,_reg_final_lut_edge_y1,
 *  097._[TNR_EDF_]_0xc80043e8_[15:8_]_8_UINT8__edf[23]_:_tnr_ctrl_46,_reg_final_lut_edge_x0,
 *  098._[TNR_EDF_]_0xc80043e8_[7:0__]_8_UINT8__edf[24]_:_tnr_ctrl_46,_reg_final_lut_edge_y0,
 *  099._[TNR_EDF_]_0xc80043ec_[31:24]_8_UINT8__edf[25]_:_tnr_ctrl_47,_reg_final_lut_detail_x1,
 *  100._[TNR_EDF_]_0xc80043ec_[23:16]_8_UINT8__edf[26]_:_tnr_ctrl_47,_reg_final_lut_detail_y1,
 *  101._[TNR_EDF_]_0xc80043ec_[15:8_]_8_UINT8__edf[27]_:_tnr_ctrl_47,_reg_final_lut_detail_x0,
 *  102._[TNR_EDF_]_0xc80043ec_[7:0__]_8_UINT8__edf[28]_:_tnr_ctrl_47,_reg_final_lut_detail_y0,
 *  103._[TNR_MC__]_0xc80041a0_[4____]_1_UINT8__m_c[00]_:_tnr_ctrl_24,_reg_luma_gain_en,
 *  104._[TNR_MC__]_0xc80041a0_[5____]_1_UINT8__m_c[01]_:_tnr_ctrl_24,_reg_skin_gain_en,
 *  105._[TNR_MC__]_0xc80041b4_[0____]_1_UINT8__m_c[02]_:_tnr_ctrl_29,_reg_dir_blur_en,
 *  106._[TNR_MC__]_0xc80041b4_[15:8_]_8_UINT8__m_c[03]_:_tnr_ctrl_29,_reg_dir_blur_gain,
 *  107._[TNR_MC__]_0xc80041b4_[31:24]_8_UINT8__m_c[04]_:_tnr_ctrl_29,_reg_diff_limit,
 *  108._[TNR_MC__]_0xc80041b8_[0____]_1_UINT8__m_c[05]_:_tnr_ctrl_30,_reg_denoise_filter_en,
 *  109._[TNR_MC__]_0xc80041b8_[15:8_]_8_UINT8__m_c[06]_:_tnr_ctrl_30,_reg_denoise_gain,
 *  110._[TNR_MC__]_0xc80041b8_[5:4__]_2_UINT8__m_c[07]_:_tnr_ctrl_30,_reg_denoise_cut_resolution,
 *  111._[TNR_MC__]_0xc80041b8_[22:16]_7_UINT8__m_c[08]_:_tnr_ctrl_30,_reg_denoise_G0,
 *  112._[TNR_MC__]_0xc80041bc_[28:24]_5_UINT8__m_c[09]_:_tnr_ctrl_31,_reg_denoise_G1,
 *  113._[TNR_MC__]_0xc80041bc_[20:16]_5_UINT8__m_c[10]_:_tnr_ctrl_31,_reg_denoise_G2,
 *  114._[TNR_MC__]_0xc80041bc_[12:8_]_5_UINT8__m_c[11]_:_tnr_ctrl_31,_reg_denoise_G3,
 *  115._[TNR_MC__]_0xc80041bc_[4:0__]_5_UINT8__m_c[12]_:_tnr_ctrl_31,_reg_denoise_G4,
 *  116._[TNR_MC__]_0xc80041a4_[0____]_1_UINT8__m_c[13]_:_tnr_ctrl_25,_reg_flat_y_blur_en,
 *  117._[TNR_MC__]_0xc80041a4_[5:4__]_2_UINT8__m_c[14]_:_tnr_ctrl_25,_reg_flat_y_cut_resolution,
 *  118._[TNR_MC__]_0xc80041a4_[13:8_]_6_UINT8__m_c[15]_:_tnr_ctrl_25,_reg_flat_y_G0,
 *  119._[TNR_MC__]_0xc80041a4_[31:28]_4_UINT8__m_c[16]_:_tnr_ctrl_25,_reg_flat_y_G1,
 *  120._[TNR_MC__]_0xc80041a4_[27:24]_4_UINT8__m_c[17]_:_tnr_ctrl_25,_reg_flat_y_G2,
 *  121._[TNR_MC__]_0xc80041a4_[23:20]_4_UINT8__m_c[18]_:_tnr_ctrl_25,_reg_flat_y_G3,
 *  122._[TNR_MC__]_0xc80041a4_[19:16]_4_UINT8__m_c[19]_:_tnr_ctrl_25,_reg_flat_y_G4,
 *  123._[TNR_MC__]_0xc80041a8_[0____]_1_UINT8__m_c[20]_:_tnr_ctrl_26,_reg_flat_c_blur_en,
 *  124._[TNR_MC__]_0xc80041a8_[5:4__]_2_UINT8__m_c[21]_:_tnr_ctrl_26,_reg_flat_c_cut_resolution,
 *  125._[TNR_MC__]_0xc80041a8_[13:8_]_6_UINT8__m_c[22]_:_tnr_ctrl_26,_reg_flat_c_G0,
 *  126._[TNR_MC__]_0xc80041a8_[31:28]_4_UINT8__m_c[23]_:_tnr_ctrl_26,_reg_flat_c_G1,
 *  127._[TNR_MC__]_0xc80041a8_[27:24]_4_UINT8__m_c[24]_:_tnr_ctrl_26,_reg_flat_c_G2,
 *  128._[TNR_MC__]_0xc80041a8_[23:20]_4_UINT8__m_c[25]_:_tnr_ctrl_26,_reg_flat_c_G3,
 *  129._[TNR_MC__]_0xc80041a8_[19:16]_4_UINT8__m_c[26]_:_tnr_ctrl_26,_reg_flat_c_G4,
 *
 * @param   *pstParams [in] void
 * @return  OK if success, ERROR otherwise.
 * @see		set discription
 * @author
 */
int PE_NRD_HW_H15_SetTnrDetailCtrl(void *pstParams)
{
	int ret = RET_OK;
	#ifdef PE_USE_H15_NEW_DTNR_STRUCT
	PE_NRD_HW_H15_SETTINGS_T *pInfo = &_g_pe_nrd_hw_h15_info;
	do{
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H15)
		{
			LX_PE_NRD_TNR5_DETAIL_T *pp=(LX_PE_NRD_TNR5_DETAIL_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			if (_g_nrd_hw_h15_trace)
			{
				UINT8 *pd = pp->s_m;
				PE_NRD_HW_H15_DBG_PRINT("[%d]:\n"\
					"s_m[00]0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,\n"\
					"s_m[10]0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,\n"\
					"s_m[20]0x%02X,0x%02X,0x%02X,0x%02X,\n", \
					pp->win_id, \
					pd[0],  pd[1],  pd[2],  pd[3],  pd[4],  pd[5],  pd[6],  pd[7],  pd[8],  pd[9], \
					pd[10], pd[11], pd[12], pd[13], pd[14], pd[15], pd[16], pd[17], pd[18], pd[19], \
					pd[20], pd[21], pd[22], pd[23]);
				pd = pp->edf;
				PE_NRD_HW_H15_DBG_PRINT("[%d]:\n"\
					"edf[00]0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,\n"\
					"edf[10]0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,\n"\
					"edf[20]0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,\n", \
					pp->win_id, \
					pd[0],  pd[1],  pd[2],  pd[3],  pd[4],  pd[5],  pd[6],  pd[7],  pd[8],  pd[9], \
					pd[10], pd[11], pd[12], pd[13], pd[14], pd[15], pd[16], pd[17], pd[18], pd[19], \
					pd[20], pd[21], pd[22], pd[23], pd[24], pd[25], pd[26], pd[27], pd[28]);
				pd = pp->m_c;
				PE_NRD_HW_H15_DBG_PRINT("[%d]:\n"\
					"m_c[00]0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,\n"\
					"m_c[10]0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,\n"\
					"m_c[20]0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,\n", \
					pp->win_id, \
					pd[0],  pd[1],  pd[2],  pd[3],  pd[4],  pd[5],  pd[6],  pd[7],  pd[8],  pd[9], \
					pd[10], pd[11], pd[12], pd[13], pd[14], pd[15], pd[16], pd[17], pd[18], pd[19], \
					pd[20], pd[21], pd[22], pd[23], pd[24], pd[25], pd[26]);
			}
			if(PE_CHECK_WIN0(pp->win_id))
			{
				/* set reg */
				PE_ND12_H15A0_RdFL(tnr_ctrl_00);
				PE_ND12_H15A0_RdFL(tnr_ctrl_02);
				PE_ND12_H15A0_RdFL(tnr_ctrl_09);
				PE_ND12_H15A0_RdFL(tnr_ctrl_11);
				PE_ND12_H15A0_RdFL(tnr_ctrl_18);
				PE_ND12_H15A0_RdFL(tnr_ctrl_19);
				PE_ND12_H15A0_RdFL(tnr_ctrl_06);
				PE_ND12_H15A0_RdFL(tnr_ctrl_20);
				PE_ND12_H15A0_RdFL(tnr_ctrl_40);
				PE_ND12_H15A0_RdFL(tnr_ctrl_42);
				PE_ND12_H15A0_RdFL(tnr_ctrl_43);
				PE_ND12_H15A0_RdFL(tnr_ctrl_44);
				PE_ND12_H15A0_RdFL(tnr_ctrl_45);
				PE_ND12_H15A0_RdFL(tnr_ctrl_46);
				PE_ND12_H15A0_RdFL(tnr_ctrl_47);
				PE_ND12_H15A0_RdFL(tnr_ctrl_24);
				PE_ND12_H15A0_RdFL(tnr_ctrl_29);
				PE_ND12_H15A0_RdFL(tnr_ctrl_30);
				PE_ND12_H15A0_RdFL(tnr_ctrl_31);
				PE_ND12_H15A0_RdFL(tnr_ctrl_25);
				PE_ND12_H15A0_RdFL(tnr_ctrl_26);
				PE_ND12_H15A0_Wr01(tnr_ctrl_00, reg_sad_8x3_res,            GET_BITS(pp->s_m[0],  0, 2));
				PE_ND12_H15A0_Wr01(tnr_ctrl_00, reg_sad_8x3_iir_en,         GET_BITS(pp->s_m[1],  0, 1));
				PE_ND12_H15A0_Wr01(tnr_ctrl_00, reg_sad_8x3_iir_alpha,      GET_BITS(pp->s_m[2],  0, 8));
				//PE_ND12_H15A0_Wr01(tnr_ctrl_02, sc_en,                      GET_BITS(pp->s_m[3],  0, 1));
				PE_ND12_H15A0_Wr01(tnr_ctrl_02, sc_th_ma,                   GET_BITS(pp->s_m[4],  0, 8));
				PE_ND12_H15A0_Wr01(tnr_ctrl_09, sad_base,                   GET_BITS(pp->s_m[5],  0, 7));
				PE_ND12_H15A0_Wr01(tnr_ctrl_11, lb_smooth_en,               GET_BITS(pp->s_m[6],  0, 1));
				PE_ND12_H15A0_Wr01(tnr_ctrl_18, bsad_scale,                 GET_BITS(pp->s_m[7],  0, 3));
				PE_ND12_H15A0_Wr01(tnr_ctrl_18, gsad_scale,                 GET_BITS(pp->s_m[8],  0, 3));
				PE_ND12_H15A0_Wr01(tnr_ctrl_18, gmv_gmv_tl_en,              GET_BITS(pp->s_m[9],  0, 1));
				PE_ND12_H15A0_Wr01(tnr_ctrl_18, bmv_gmv_tl_en,              GET_BITS(pp->s_m[10], 0, 1));
				PE_ND12_H15A0_Wr01(tnr_ctrl_18, bmv_pmv_tl_en,              GET_BITS(pp->s_m[11], 0, 1));
				PE_ND12_H15A0_Wr01(tnr_ctrl_18, gmv_gmv_tl,                 GET_BITS(pp->s_m[12], 0, 7));
				PE_ND12_H15A0_Wr01(tnr_ctrl_18, bmv_gmv_tl,                 GET_BITS(pp->s_m[13], 0, 7));
				PE_ND12_H15A0_Wr01(tnr_ctrl_18, bmv_pmv_tl,                 GET_BITS(pp->s_m[14], 0, 7));
				PE_ND12_H15A0_Wr01(tnr_ctrl_19, gmv_0mv_tl_en,              GET_BITS(pp->s_m[15], 0, 1));
				PE_ND12_H15A0_Wr01(tnr_ctrl_19, bmv_0mv_tl_en,              GET_BITS(pp->s_m[16], 0, 1));
				PE_ND12_H15A0_Wr01(tnr_ctrl_19, gmv_0mv_tl,                 GET_BITS(pp->s_m[17], 0, 7));
				PE_ND12_H15A0_Wr01(tnr_ctrl_19, bmv_0mv_tl,                 GET_BITS(pp->s_m[18], 0, 7));
				PE_ND12_H15A0_Wr01(tnr_ctrl_09, lb_smooth_clip,             GET_BITS(pp->s_m[19], 0, 6));
				PE_ND12_H15A0_Wr01(tnr_ctrl_06, me_lb_sad_base,             GET_BITS(pp->s_m[20], 0, 8));
				//PE_ND12_H15A0_Wr01(tnr_ctrl_11, pv_smooth_en,               GET_BITS(pp->s_m[21], 0, 1));
				PE_ND12_H15A0_Wr01(tnr_ctrl_09, pv_smooth_clip,             GET_BITS(pp->s_m[22], 0, 6));
				PE_ND12_H15A0_Wr01(tnr_ctrl_06, pv_smooth_sad_base,         GET_BITS(pp->s_m[23], 0, 8));
				PE_ND12_H15A0_Wr01(tnr_ctrl_20, reg_edge_alpha_mul,         GET_BITS(pp->edf[0],  0, 8));
				PE_ND12_H15A0_Wr01(tnr_ctrl_20, reg_edge_beta_mul,          GET_BITS(pp->edf[1],  0, 8));
				PE_ND12_H15A0_Wr01(tnr_ctrl_20, reg_detail_alpha_mul,       GET_BITS(pp->edf[2],  0, 8));
				PE_ND12_H15A0_Wr01(tnr_ctrl_20, reg_detail_beta_mul,        GET_BITS(pp->edf[3],  0, 8));
				PE_ND12_H15A0_Wr01(tnr_ctrl_40, reg_t_cut_resolution,       GET_BITS(pp->edf[4],  0, 2));
				PE_ND12_H15A0_Wr01(tnr_ctrl_42, reg_a_lut_edge_x1,          GET_BITS(pp->edf[5],  0, 8));
				PE_ND12_H15A0_Wr01(tnr_ctrl_42, reg_a_lut_edge_y1,          GET_BITS(pp->edf[6],  0, 8));
				PE_ND12_H15A0_Wr01(tnr_ctrl_42, reg_a_lut_edge_x0,          GET_BITS(pp->edf[7],  0, 8));
				PE_ND12_H15A0_Wr01(tnr_ctrl_42, reg_a_lut_edge_y0,          GET_BITS(pp->edf[8],  0, 8));
				PE_ND12_H15A0_Wr01(tnr_ctrl_43, reg_a_lut_detail_x1,        GET_BITS(pp->edf[9],  0, 8));
				PE_ND12_H15A0_Wr01(tnr_ctrl_43, reg_a_lut_detail_y1,        GET_BITS(pp->edf[10], 0, 8));
				PE_ND12_H15A0_Wr01(tnr_ctrl_43, reg_a_lut_detail_x0,        GET_BITS(pp->edf[11], 0, 8));
				PE_ND12_H15A0_Wr01(tnr_ctrl_43, reg_a_lut_detail_y0,        GET_BITS(pp->edf[12], 0, 8));
				PE_ND12_H15A0_Wr01(tnr_ctrl_44, reg_t_lut_edge_x1,          GET_BITS(pp->edf[13], 0, 8));
				PE_ND12_H15A0_Wr01(tnr_ctrl_44, reg_t_lut_edge_y1,          GET_BITS(pp->edf[14], 0, 8));
				PE_ND12_H15A0_Wr01(tnr_ctrl_44, reg_t_lut_edge_x0,          GET_BITS(pp->edf[15], 0, 8));
				PE_ND12_H15A0_Wr01(tnr_ctrl_44, reg_t_lut_edge_y0,          GET_BITS(pp->edf[16], 0, 8));
				PE_ND12_H15A0_Wr01(tnr_ctrl_45, reg_t_lut_detail_x1,        GET_BITS(pp->edf[17], 0, 8));
				PE_ND12_H15A0_Wr01(tnr_ctrl_45, reg_t_lut_detail_y1,        GET_BITS(pp->edf[18], 0, 8));
				PE_ND12_H15A0_Wr01(tnr_ctrl_45, reg_t_lut_detail_x0,        GET_BITS(pp->edf[19], 0, 8));
				PE_ND12_H15A0_Wr01(tnr_ctrl_45, reg_t_lut_detail_y0,        GET_BITS(pp->edf[20], 0, 8));
				PE_ND12_H15A0_Wr01(tnr_ctrl_46, reg_final_lut_edge_x1,      GET_BITS(pp->edf[21], 0, 8));
				PE_ND12_H15A0_Wr01(tnr_ctrl_46, reg_final_lut_edge_y1,      GET_BITS(pp->edf[22], 0, 8));
				PE_ND12_H15A0_Wr01(tnr_ctrl_46, reg_final_lut_edge_x0,      GET_BITS(pp->edf[23], 0, 8));
				PE_ND12_H15A0_Wr01(tnr_ctrl_46, reg_final_lut_edge_y0,      GET_BITS(pp->edf[24], 0, 8));
				PE_ND12_H15A0_Wr01(tnr_ctrl_47, reg_final_lut_detail_x1,    GET_BITS(pp->edf[25], 0, 8));
				PE_ND12_H15A0_Wr01(tnr_ctrl_47, reg_final_lut_detail_y1,    GET_BITS(pp->edf[26], 0, 8));
				PE_ND12_H15A0_Wr01(tnr_ctrl_47, reg_final_lut_detail_x0,    GET_BITS(pp->edf[27], 0, 8));
				PE_ND12_H15A0_Wr01(tnr_ctrl_47, reg_final_lut_detail_y0,    GET_BITS(pp->edf[28], 0, 8));
				PE_ND12_H15A0_Wr01(tnr_ctrl_24, reg_luma_gain_en,           GET_BITS(pp->m_c[0],  0, 1));
				PE_ND12_H15A0_Wr01(tnr_ctrl_24, reg_skin_gain_en,           GET_BITS(pp->m_c[1],  0, 1));
				PE_ND12_H15A0_Wr01(tnr_ctrl_29, reg_dir_blur_en,            GET_BITS(pp->m_c[2],  0, 1));
				PE_ND12_H15A0_Wr01(tnr_ctrl_29, reg_dir_blur_gain,          GET_BITS(pp->m_c[3],  0, 8));
				PE_ND12_H15A0_Wr01(tnr_ctrl_29, reg_diff_limit,             GET_BITS(pp->m_c[4],  0, 8));
				PE_ND12_H15A0_Wr01(tnr_ctrl_30, reg_denoise_filter_en,      GET_BITS(pp->m_c[5],  0, 1));
				PE_ND12_H15A0_Wr01(tnr_ctrl_30, reg_denoise_gain,           GET_BITS(pp->m_c[6],  0, 8));
				PE_ND12_H15A0_Wr01(tnr_ctrl_30, reg_denoise_cut_resolution, GET_BITS(pp->m_c[7],  0, 2));
				PE_ND12_H15A0_Wr01(tnr_ctrl_30, reg_denoise_G0,             GET_BITS(pp->m_c[8],  0, 7));
				PE_ND12_H15A0_Wr01(tnr_ctrl_31, reg_denoise_G1,             GET_BITS(pp->m_c[9],  0, 5));
				PE_ND12_H15A0_Wr01(tnr_ctrl_31, reg_denoise_G2,             GET_BITS(pp->m_c[10], 0, 5));
				PE_ND12_H15A0_Wr01(tnr_ctrl_31, reg_denoise_G3,             GET_BITS(pp->m_c[11], 0, 5));
				PE_ND12_H15A0_Wr01(tnr_ctrl_31, reg_denoise_G4,             GET_BITS(pp->m_c[12], 0, 5));
				PE_ND12_H15A0_Wr01(tnr_ctrl_25, reg_flat_y_blur_en,         GET_BITS(pp->m_c[13], 0, 1));
				PE_ND12_H15A0_Wr01(tnr_ctrl_25, reg_flat_y_cut_resolution,  GET_BITS(pp->m_c[14], 0, 2));
				PE_ND12_H15A0_Wr01(tnr_ctrl_25, reg_flat_y_G0,              GET_BITS(pp->m_c[15], 0, 6));
				PE_ND12_H15A0_Wr01(tnr_ctrl_25, reg_flat_y_G1,              GET_BITS(pp->m_c[16], 0, 4));
				PE_ND12_H15A0_Wr01(tnr_ctrl_25, reg_flat_y_G2,              GET_BITS(pp->m_c[17], 0, 4));
				PE_ND12_H15A0_Wr01(tnr_ctrl_25, reg_flat_y_G3,              GET_BITS(pp->m_c[18], 0, 4));
				PE_ND12_H15A0_Wr01(tnr_ctrl_25, reg_flat_y_G4,              GET_BITS(pp->m_c[19], 0, 4));
				PE_ND12_H15A0_Wr01(tnr_ctrl_26, reg_flat_c_blur_en,         GET_BITS(pp->m_c[20], 0, 1));
				PE_ND12_H15A0_Wr01(tnr_ctrl_26, reg_flat_c_cut_resolution,  GET_BITS(pp->m_c[21], 0, 2));
				PE_ND12_H15A0_Wr01(tnr_ctrl_26, reg_flat_c_G0,              GET_BITS(pp->m_c[22], 0, 6));
				PE_ND12_H15A0_Wr01(tnr_ctrl_26, reg_flat_c_G1,              GET_BITS(pp->m_c[23], 0, 4));
				PE_ND12_H15A0_Wr01(tnr_ctrl_26, reg_flat_c_G2,              GET_BITS(pp->m_c[24], 0, 4));
				PE_ND12_H15A0_Wr01(tnr_ctrl_26, reg_flat_c_G3,              GET_BITS(pp->m_c[25], 0, 4));
				PE_ND12_H15A0_Wr01(tnr_ctrl_26, reg_flat_c_G4,              GET_BITS(pp->m_c[26], 0, 4));
				PE_ND12_H15A0_WrFL(tnr_ctrl_00);
				PE_ND12_H15A0_WrFL(tnr_ctrl_02);
				PE_ND12_H15A0_WrFL(tnr_ctrl_09);
				PE_ND12_H15A0_WrFL(tnr_ctrl_11);
				PE_ND12_H15A0_WrFL(tnr_ctrl_18);
				PE_ND12_H15A0_WrFL(tnr_ctrl_19);
				PE_ND12_H15A0_WrFL(tnr_ctrl_06);
				PE_ND12_H15A0_WrFL(tnr_ctrl_20);
				PE_ND12_H15A0_WrFL(tnr_ctrl_40);
				PE_ND12_H15A0_WrFL(tnr_ctrl_42);
				PE_ND12_H15A0_WrFL(tnr_ctrl_43);
				PE_ND12_H15A0_WrFL(tnr_ctrl_44);
				PE_ND12_H15A0_WrFL(tnr_ctrl_45);
				PE_ND12_H15A0_WrFL(tnr_ctrl_46);
				PE_ND12_H15A0_WrFL(tnr_ctrl_47);
				PE_ND12_H15A0_WrFL(tnr_ctrl_24);
				PE_ND12_H15A0_WrFL(tnr_ctrl_29);
				PE_ND12_H15A0_WrFL(tnr_ctrl_30);
				PE_ND12_H15A0_WrFL(tnr_ctrl_31);
				PE_ND12_H15A0_WrFL(tnr_ctrl_25);
				PE_ND12_H15A0_WrFL(tnr_ctrl_26);
				/* set user data */
				PE_NRD_HW_H15A_TNR12_SET_USER(tnr_ctrl_00, reg_sad_8x3_res,            GET_BITS(pp->s_m[0],  0, 2));
				PE_NRD_HW_H15A_TNR12_SET_USER(tnr_ctrl_00, reg_sad_8x3_iir_en,         GET_BITS(pp->s_m[1],  0, 1));
				PE_NRD_HW_H15A_TNR12_SET_USER(tnr_ctrl_00, reg_sad_8x3_iir_alpha,      GET_BITS(pp->s_m[2],  0, 8));
				//PE_NRD_HW_H15A_TNR12_SET_USER(tnr_ctrl_02, sc_en,                      GET_BITS(pp->s_m[3],  0, 1));
				pInfo->tnr_sc_en_user = GET_BITS(pp->s_m[3], 0, 1);
				pInfo->tnr_sc_en_mask = 1;
				PE_NRD_HW_H15A_TNR12_SET_USER(tnr_ctrl_02, sc_th_ma,                   GET_BITS(pp->s_m[4],  0, 8));
				PE_NRD_HW_H15A_TNR12_SET_USER(tnr_ctrl_09, sad_base,                   GET_BITS(pp->s_m[5],  0, 7));
				PE_NRD_HW_H15A_TNR12_SET_USER(tnr_ctrl_11, lb_smooth_en,               GET_BITS(pp->s_m[6],  0, 1));
				PE_NRD_HW_H15A_TNR12_SET_USER(tnr_ctrl_18, bsad_scale,                 GET_BITS(pp->s_m[7],  0, 3));
				PE_NRD_HW_H15A_TNR12_SET_USER(tnr_ctrl_18, gsad_scale,                 GET_BITS(pp->s_m[8],  0, 3));
				PE_NRD_HW_H15A_TNR12_SET_USER(tnr_ctrl_18, gmv_gmv_tl_en,              GET_BITS(pp->s_m[9],  0, 1));
				PE_NRD_HW_H15A_TNR12_SET_USER(tnr_ctrl_18, bmv_gmv_tl_en,              GET_BITS(pp->s_m[10], 0, 1));
				PE_NRD_HW_H15A_TNR12_SET_USER(tnr_ctrl_18, bmv_pmv_tl_en,              GET_BITS(pp->s_m[11], 0, 1));
				PE_NRD_HW_H15A_TNR12_SET_USER(tnr_ctrl_18, gmv_gmv_tl,                 GET_BITS(pp->s_m[12], 0, 7));
				PE_NRD_HW_H15A_TNR12_SET_USER(tnr_ctrl_18, bmv_gmv_tl,                 GET_BITS(pp->s_m[13], 0, 7));
				PE_NRD_HW_H15A_TNR12_SET_USER(tnr_ctrl_18, bmv_pmv_tl,                 GET_BITS(pp->s_m[14], 0, 7));
				PE_NRD_HW_H15A_TNR12_SET_USER(tnr_ctrl_19, gmv_0mv_tl_en,              GET_BITS(pp->s_m[15], 0, 1));
				PE_NRD_HW_H15A_TNR12_SET_USER(tnr_ctrl_19, bmv_0mv_tl_en,              GET_BITS(pp->s_m[16], 0, 1));
				PE_NRD_HW_H15A_TNR12_SET_USER(tnr_ctrl_19, gmv_0mv_tl,                 GET_BITS(pp->s_m[17], 0, 7));
				PE_NRD_HW_H15A_TNR12_SET_USER(tnr_ctrl_19, bmv_0mv_tl,                 GET_BITS(pp->s_m[18], 0, 7));
				PE_NRD_HW_H15A_TNR12_SET_USER(tnr_ctrl_09, lb_smooth_clip,             GET_BITS(pp->s_m[19], 0, 6));
				PE_NRD_HW_H15A_TNR12_SET_USER(tnr_ctrl_06, me_lb_sad_base,             GET_BITS(pp->s_m[20], 0, 8));
				//PE_NRD_HW_H15A_TNR12_SET_USER(tnr_ctrl_11, pv_smooth_en,               GET_BITS(pp->s_m[21], 0, 1));
				pInfo->tnr_pv_smooth_en_user = GET_BITS(pp->s_m[21], 0, 1);
				pInfo->tnr_pv_smooth_en_mask = 1;
				PE_NRD_HW_H15A_TNR12_SET_USER(tnr_ctrl_09, pv_smooth_clip,             GET_BITS(pp->s_m[22], 0, 6));
				PE_NRD_HW_H15A_TNR12_SET_USER(tnr_ctrl_06, pv_smooth_sad_base,         GET_BITS(pp->s_m[23], 0, 8));
				PE_NRD_HW_H15A_TNR12_SET_USER(tnr_ctrl_20, reg_edge_alpha_mul,         GET_BITS(pp->edf[0],  0, 8));
				PE_NRD_HW_H15A_TNR12_SET_USER(tnr_ctrl_20, reg_edge_beta_mul,          GET_BITS(pp->edf[1],  0, 8));
				PE_NRD_HW_H15A_TNR12_SET_USER(tnr_ctrl_20, reg_detail_alpha_mul,       GET_BITS(pp->edf[2],  0, 8));
				PE_NRD_HW_H15A_TNR12_SET_USER(tnr_ctrl_20, reg_detail_beta_mul,        GET_BITS(pp->edf[3],  0, 8));
				PE_NRD_HW_H15A_TNR12_SET_USER(tnr_ctrl_40, reg_t_cut_resolution,       GET_BITS(pp->edf[4],  0, 2));
				PE_NRD_HW_H15A_TNR12_SET_USER(tnr_ctrl_42, reg_a_lut_edge_x1,          GET_BITS(pp->edf[5],  0, 8));
				PE_NRD_HW_H15A_TNR12_SET_USER(tnr_ctrl_42, reg_a_lut_edge_y1,          GET_BITS(pp->edf[6],  0, 8));
				PE_NRD_HW_H15A_TNR12_SET_USER(tnr_ctrl_42, reg_a_lut_edge_x0,          GET_BITS(pp->edf[7],  0, 8));
				PE_NRD_HW_H15A_TNR12_SET_USER(tnr_ctrl_42, reg_a_lut_edge_y0,          GET_BITS(pp->edf[8],  0, 8));
				PE_NRD_HW_H15A_TNR12_SET_USER(tnr_ctrl_43, reg_a_lut_detail_x1,        GET_BITS(pp->edf[9],  0, 8));
				PE_NRD_HW_H15A_TNR12_SET_USER(tnr_ctrl_43, reg_a_lut_detail_y1,        GET_BITS(pp->edf[10], 0, 8));
				PE_NRD_HW_H15A_TNR12_SET_USER(tnr_ctrl_43, reg_a_lut_detail_x0,        GET_BITS(pp->edf[11], 0, 8));
				PE_NRD_HW_H15A_TNR12_SET_USER(tnr_ctrl_43, reg_a_lut_detail_y0,        GET_BITS(pp->edf[12], 0, 8));
				PE_NRD_HW_H15A_TNR12_SET_USER(tnr_ctrl_44, reg_t_lut_edge_x1,          GET_BITS(pp->edf[13], 0, 8));
				PE_NRD_HW_H15A_TNR12_SET_USER(tnr_ctrl_44, reg_t_lut_edge_y1,          GET_BITS(pp->edf[14], 0, 8));
				PE_NRD_HW_H15A_TNR12_SET_USER(tnr_ctrl_44, reg_t_lut_edge_x0,          GET_BITS(pp->edf[15], 0, 8));
				PE_NRD_HW_H15A_TNR12_SET_USER(tnr_ctrl_44, reg_t_lut_edge_y0,          GET_BITS(pp->edf[16], 0, 8));
				PE_NRD_HW_H15A_TNR12_SET_USER(tnr_ctrl_45, reg_t_lut_detail_x1,        GET_BITS(pp->edf[17], 0, 8));
				PE_NRD_HW_H15A_TNR12_SET_USER(tnr_ctrl_45, reg_t_lut_detail_y1,        GET_BITS(pp->edf[18], 0, 8));
				PE_NRD_HW_H15A_TNR12_SET_USER(tnr_ctrl_45, reg_t_lut_detail_x0,        GET_BITS(pp->edf[19], 0, 8));
				PE_NRD_HW_H15A_TNR12_SET_USER(tnr_ctrl_45, reg_t_lut_detail_y0,        GET_BITS(pp->edf[20], 0, 8));
				PE_NRD_HW_H15A_TNR12_SET_USER(tnr_ctrl_46, reg_final_lut_edge_x1,      GET_BITS(pp->edf[21], 0, 8));
				PE_NRD_HW_H15A_TNR12_SET_USER(tnr_ctrl_46, reg_final_lut_edge_y1,      GET_BITS(pp->edf[22], 0, 8));
				PE_NRD_HW_H15A_TNR12_SET_USER(tnr_ctrl_46, reg_final_lut_edge_x0,      GET_BITS(pp->edf[23], 0, 8));
				PE_NRD_HW_H15A_TNR12_SET_USER(tnr_ctrl_46, reg_final_lut_edge_y0,      GET_BITS(pp->edf[24], 0, 8));
				PE_NRD_HW_H15A_TNR12_SET_USER(tnr_ctrl_47, reg_final_lut_detail_x1,    GET_BITS(pp->edf[25], 0, 8));
				PE_NRD_HW_H15A_TNR12_SET_USER(tnr_ctrl_47, reg_final_lut_detail_y1,    GET_BITS(pp->edf[26], 0, 8));
				PE_NRD_HW_H15A_TNR12_SET_USER(tnr_ctrl_47, reg_final_lut_detail_x0,    GET_BITS(pp->edf[27], 0, 8));
				PE_NRD_HW_H15A_TNR12_SET_USER(tnr_ctrl_47, reg_final_lut_detail_y0,    GET_BITS(pp->edf[28], 0, 8));
				PE_NRD_HW_H15A_TNR12_SET_USER(tnr_ctrl_24, reg_luma_gain_en,           GET_BITS(pp->m_c[0],  0, 1));
				PE_NRD_HW_H15A_TNR12_SET_USER(tnr_ctrl_24, reg_skin_gain_en,           GET_BITS(pp->m_c[1],  0, 1));
				PE_NRD_HW_H15A_TNR12_SET_USER(tnr_ctrl_29, reg_dir_blur_en,            GET_BITS(pp->m_c[2],  0, 1));
				PE_NRD_HW_H15A_TNR12_SET_USER(tnr_ctrl_29, reg_dir_blur_gain,          GET_BITS(pp->m_c[3],  0, 8));
				PE_NRD_HW_H15A_TNR12_SET_USER(tnr_ctrl_29, reg_diff_limit,             GET_BITS(pp->m_c[4],  0, 8));
				PE_NRD_HW_H15A_TNR12_SET_USER(tnr_ctrl_30, reg_denoise_filter_en,      GET_BITS(pp->m_c[5],  0, 1));
				PE_NRD_HW_H15A_TNR12_SET_USER(tnr_ctrl_30, reg_denoise_gain,           GET_BITS(pp->m_c[6],  0, 8));
				PE_NRD_HW_H15A_TNR12_SET_USER(tnr_ctrl_30, reg_denoise_cut_resolution, GET_BITS(pp->m_c[7],  0, 2));
				PE_NRD_HW_H15A_TNR12_SET_USER(tnr_ctrl_30, reg_denoise_G0,             GET_BITS(pp->m_c[8],  0, 7));
				PE_NRD_HW_H15A_TNR12_SET_USER(tnr_ctrl_31, reg_denoise_G1,             GET_BITS(pp->m_c[9],  0, 5));
				PE_NRD_HW_H15A_TNR12_SET_USER(tnr_ctrl_31, reg_denoise_G2,             GET_BITS(pp->m_c[10], 0, 5));
				PE_NRD_HW_H15A_TNR12_SET_USER(tnr_ctrl_31, reg_denoise_G3,             GET_BITS(pp->m_c[11], 0, 5));
				PE_NRD_HW_H15A_TNR12_SET_USER(tnr_ctrl_31, reg_denoise_G4,             GET_BITS(pp->m_c[12], 0, 5));
				PE_NRD_HW_H15A_TNR12_SET_USER(tnr_ctrl_25, reg_flat_y_blur_en,         GET_BITS(pp->m_c[13], 0, 1));
				PE_NRD_HW_H15A_TNR12_SET_USER(tnr_ctrl_25, reg_flat_y_cut_resolution,  GET_BITS(pp->m_c[14], 0, 2));
				PE_NRD_HW_H15A_TNR12_SET_USER(tnr_ctrl_25, reg_flat_y_G0,              GET_BITS(pp->m_c[15], 0, 6));
				PE_NRD_HW_H15A_TNR12_SET_USER(tnr_ctrl_25, reg_flat_y_G1,              GET_BITS(pp->m_c[16], 0, 4));
				PE_NRD_HW_H15A_TNR12_SET_USER(tnr_ctrl_25, reg_flat_y_G2,              GET_BITS(pp->m_c[17], 0, 4));
				PE_NRD_HW_H15A_TNR12_SET_USER(tnr_ctrl_25, reg_flat_y_G3,              GET_BITS(pp->m_c[18], 0, 4));
				PE_NRD_HW_H15A_TNR12_SET_USER(tnr_ctrl_25, reg_flat_y_G4,              GET_BITS(pp->m_c[19], 0, 4));
				PE_NRD_HW_H15A_TNR12_SET_USER(tnr_ctrl_26, reg_flat_c_blur_en,         GET_BITS(pp->m_c[20], 0, 1));
				PE_NRD_HW_H15A_TNR12_SET_USER(tnr_ctrl_26, reg_flat_c_cut_resolution,  GET_BITS(pp->m_c[21], 0, 2));
				PE_NRD_HW_H15A_TNR12_SET_USER(tnr_ctrl_26, reg_flat_c_G0,              GET_BITS(pp->m_c[22], 0, 6));
				PE_NRD_HW_H15A_TNR12_SET_USER(tnr_ctrl_26, reg_flat_c_G1,              GET_BITS(pp->m_c[23], 0, 4));
				PE_NRD_HW_H15A_TNR12_SET_USER(tnr_ctrl_26, reg_flat_c_G2,              GET_BITS(pp->m_c[24], 0, 4));
				PE_NRD_HW_H15A_TNR12_SET_USER(tnr_ctrl_26, reg_flat_c_G3,              GET_BITS(pp->m_c[25], 0, 4));
				PE_NRD_HW_H15A_TNR12_SET_USER(tnr_ctrl_26, reg_flat_c_G4,              GET_BITS(pp->m_c[26], 0, 4));
			}
		}
		else
		{
			PE_NRD_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	#endif
	return ret;
}
/**
 * get tnr detail ctrl
 * - use input struct LX_PE_NRD_TNR5_DETAIL_T
 *  050._[TNR_ME__]_0xC8004140_[7:6__]_2_UINT8__s_m[00]_:_tnr_ctrl_00,_reg_sad_8x3_res,
 *  051._[TNR_ME__]_0xC8004140_[8____]_1_UINT8__s_m[01]_:_tnr_ctrl_00,_reg_sad_8x3_iir_en,
 *  052._[TNR_ME__]_0xC8004140_[31:24]_8_UINT8__s_m[02]_:_tnr_ctrl_00,_reg_sad_8x3_iir_alpha,
 *  053._[TNR_SC__]_0xC8004148_[0____]_1_UINT8__s_m[03]_:_tnr_ctrl_02,_sc_en,
 *  054._[TNR_SC__]_0xC8004148_[23:16]_8_UINT8__s_m[04]_:_tnr_ctrl_02,_sc_th_ma,
 *  055._[TNR_SM__]_0xc8004164_[6:0__]_7_UINT8__s_m[05]_:_tnr_ctrl_09,_sad_base,
 *  056._[TNR_SM__]_0xc800416C_[0____]_1_UINT8__s_m[06]_:_tnr_ctrl_11,_lb_smooth_en,
 *  057._[TNR_SM__]_0xc8004188_[2:0__]_3_UINT8__s_m[07]_:_tnr_ctrl_18,_bsad_scale,
 *  058._[TNR_SM__]_0xc8004188_[6:4__]_3_UINT8__s_m[08]_:_tnr_ctrl_18,_gsad_scale,
 *  059._[TNR_SM__]_0xc8004188_[15___]_1_UINT8__s_m[09]_:_tnr_ctrl_18,_gmv_gmv_tl_en,
 *  060._[TNR_SM__]_0xc8004188_[23___]_1_UINT8__s_m[10]_:_tnr_ctrl_18,_bmv_gmv_tl_en,
 *  061._[TNR_SM__]_0xc8004188_[31___]_1_UINT8__s_m[11]_:_tnr_ctrl_18,_bmv_pmv_tl_en,
 *  062._[TNR_SM__]_0xc8004188_[14:8_]_7_UINT8__s_m[12]_:_tnr_ctrl_18,_gmv_gmv_tl,
 *  063._[TNR_SM__]_0xc8004188_[22:16]_7_UINT8__s_m[13]_:_tnr_ctrl_18,_bmv_gmv_tl,
 *  064._[TNR_SM__]_0xc8004188_[30:24]_7_UINT8__s_m[14]_:_tnr_ctrl_18,_bmv_pmv_tl,
 *  065._[TNR_SM__]_0xc800418c_[23___]_1_UINT8__s_m[15]_:_tnr_ctrl_19,_gmv_0mv_tl_en,
 *  066._[TNR_SM__]_0xc800418c_[31___]_1_UINT8__s_m[16]_:_tnr_ctrl_19,_bmv_0mv_tl_en,
 *  067._[TNR_SM__]_0xc800418c_[22:16]_7_UINT8__s_m[17]_:_tnr_ctrl_19,_gmv_0mv_tl,
 *  068._[TNR_SM__]_0xc800418c_[30:24]_7_UINT8__s_m[18]_:_tnr_ctrl_19,_bmv_0mv_tl,
 *  069._[TNR_SM__]_0xc8004164_[13:8_]_6_UINT8__s_m[19]_:_tnr_ctrl_09,_lb_smooth_clip,
 *  070._[TNR_SM__]_0xc8004158_[7:0__]_8_UINT8__s_m[20]_:_tnr_ctrl_06,_me_lb_sad_base,
 *  071._[TNR_SM__]_0xc800416C_[1____]_1_UINT8__s_m[21]_:_tnr_ctrl_11,_pv_smooth_en,
 *  072._[TNR_SM__]_0xc8004164_[21:16]_6_UINT8__s_m[22]_:_tnr_ctrl_09,_pv_smooth_clip,
 *  073._[TNR_SM__]_0xc8004158_[15:8_]_8_UINT8__s_m[23]_:_tnr_ctrl_06,_pv_smooth_sad_base,
 *  074._[TNR_EDF_]_0xc8004190_[31:24]_8_UINT8__edf[00]_:_tnr_ctrl_20,_reg_edge_alpha_mul,
 *  075._[TNR_EDF_]_0xc8004190_[23:16]_8_UINT8__edf[01]_:_tnr_ctrl_20,_reg_edge_beta_mul,
 *  076._[TNR_EDF_]_0xc8004190_[15:8_]_8_UINT8__edf[02]_:_tnr_ctrl_20,_reg_detail_alpha_mul,
 *  077._[TNR_EDF_]_0xc8004190_[7:0__]_8_UINT8__edf[03]_:_tnr_ctrl_20,_reg_detail_beta_mul,
 *  078._[TNR_EDF_]_0xc80043d0_[5:4__]_2_UINT8__edf[04]_:_tnr_ctrl_40,_reg_t_cut_resolution,
 *  079._[TNR_EDF_]_0xc80043d8_[31:24]_8_UINT8__edf[05]_:_tnr_ctrl_42,_reg_a_lut_edge_x1,
 *  080._[TNR_EDF_]_0xc80043d8_[23:16]_8_UINT8__edf[06]_:_tnr_ctrl_42,_reg_a_lut_edge_y1,
 *  081._[TNR_EDF_]_0xc80043d8_[15:8_]_8_UINT8__edf[07]_:_tnr_ctrl_42,_reg_a_lut_edge_x0,
 *  082._[TNR_EDF_]_0xc80043d8_[7:0__]_8_UINT8__edf[08]_:_tnr_ctrl_42,_reg_a_lut_edge_y0,
 *  083._[TNR_EDF_]_0xc80043dc_[31:24]_8_UINT8__edf[09]_:_tnr_ctrl_43,_reg_a_lut_detail_x1,
 *  084._[TNR_EDF_]_0xc80043dc_[23:16]_8_UINT8__edf[10]_:_tnr_ctrl_43,_reg_a_lut_detail_y1,
 *  085._[TNR_EDF_]_0xc80043dc_[15:8_]_8_UINT8__edf[11]_:_tnr_ctrl_43,_reg_a_lut_detail_x0,
 *  086._[TNR_EDF_]_0xc80043dc_[7:0__]_8_UINT8__edf[12]_:_tnr_ctrl_43,_reg_a_lut_detail_y0,
 *  087._[TNR_EDF_]_0xc80043e0_[31:24]_8_UINT8__edf[13]_:_tnr_ctrl_44,_reg_t_lut_edge_x1,
 *  088._[TNR_EDF_]_0xc80043e0_[23:16]_8_UINT8__edf[14]_:_tnr_ctrl_44,_reg_t_lut_edge_y1,
 *  089._[TNR_EDF_]_0xc80043e0_[15:8_]_8_UINT8__edf[15]_:_tnr_ctrl_44,_reg_t_lut_edge_x0,
 *  090._[TNR_EDF_]_0xc80043e0_[7:0__]_8_UINT8__edf[16]_:_tnr_ctrl_44,_reg_t_lut_edge_y0,
 *  091._[TNR_EDF_]_0xc80043e4_[31:24]_8_UINT8__edf[17]_:_tnr_ctrl_45,_reg_t_lut_detail_x1,
 *  092._[TNR_EDF_]_0xc80043e4_[23:16]_8_UINT8__edf[18]_:_tnr_ctrl_45,_reg_t_lut_detail_y1,
 *  093._[TNR_EDF_]_0xc80043e4_[15:8_]_8_UINT8__edf[19]_:_tnr_ctrl_45,_reg_t_lut_detail_x0,
 *  094._[TNR_EDF_]_0xc80043e4_[7:0__]_8_UINT8__edf[20]_:_tnr_ctrl_45,_reg_t_lut_detail_y0,
 *  095._[TNR_EDF_]_0xc80043e8_[31:24]_8_UINT8__edf[21]_:_tnr_ctrl_46,_reg_final_lut_edge_x1,
 *  096._[TNR_EDF_]_0xc80043e8_[23:16]_8_UINT8__edf[22]_:_tnr_ctrl_46,_reg_final_lut_edge_y1,
 *  097._[TNR_EDF_]_0xc80043e8_[15:8_]_8_UINT8__edf[23]_:_tnr_ctrl_46,_reg_final_lut_edge_x0,
 *  098._[TNR_EDF_]_0xc80043e8_[7:0__]_8_UINT8__edf[24]_:_tnr_ctrl_46,_reg_final_lut_edge_y0,
 *  099._[TNR_EDF_]_0xc80043ec_[31:24]_8_UINT8__edf[25]_:_tnr_ctrl_47,_reg_final_lut_detail_x1,
 *  100._[TNR_EDF_]_0xc80043ec_[23:16]_8_UINT8__edf[26]_:_tnr_ctrl_47,_reg_final_lut_detail_y1,
 *  101._[TNR_EDF_]_0xc80043ec_[15:8_]_8_UINT8__edf[27]_:_tnr_ctrl_47,_reg_final_lut_detail_x0,
 *  102._[TNR_EDF_]_0xc80043ec_[7:0__]_8_UINT8__edf[28]_:_tnr_ctrl_47,_reg_final_lut_detail_y0,
 *  103._[TNR_MC__]_0xc80041a0_[4____]_1_UINT8__m_c[00]_:_tnr_ctrl_24,_reg_luma_gain_en,
 *  104._[TNR_MC__]_0xc80041a0_[5____]_1_UINT8__m_c[01]_:_tnr_ctrl_24,_reg_skin_gain_en,
 *  105._[TNR_MC__]_0xc80041b4_[0____]_1_UINT8__m_c[02]_:_tnr_ctrl_29,_reg_dir_blur_en,
 *  106._[TNR_MC__]_0xc80041b4_[15:8_]_8_UINT8__m_c[03]_:_tnr_ctrl_29,_reg_dir_blur_gain,
 *  107._[TNR_MC__]_0xc80041b4_[31:24]_8_UINT8__m_c[04]_:_tnr_ctrl_29,_reg_diff_limit,
 *  108._[TNR_MC__]_0xc80041b8_[0____]_1_UINT8__m_c[05]_:_tnr_ctrl_30,_reg_denoise_filter_en,
 *  109._[TNR_MC__]_0xc80041b8_[15:8_]_8_UINT8__m_c[06]_:_tnr_ctrl_30,_reg_denoise_gain,
 *  110._[TNR_MC__]_0xc80041b8_[5:4__]_2_UINT8__m_c[07]_:_tnr_ctrl_30,_reg_denoise_cut_resolution,
 *  111._[TNR_MC__]_0xc80041b8_[22:16]_7_UINT8__m_c[08]_:_tnr_ctrl_30,_reg_denoise_G0,
 *  112._[TNR_MC__]_0xc80041bc_[28:24]_5_UINT8__m_c[09]_:_tnr_ctrl_31,_reg_denoise_G1,
 *  113._[TNR_MC__]_0xc80041bc_[20:16]_5_UINT8__m_c[10]_:_tnr_ctrl_31,_reg_denoise_G2,
 *  114._[TNR_MC__]_0xc80041bc_[12:8_]_5_UINT8__m_c[11]_:_tnr_ctrl_31,_reg_denoise_G3,
 *  115._[TNR_MC__]_0xc80041bc_[4:0__]_5_UINT8__m_c[12]_:_tnr_ctrl_31,_reg_denoise_G4,
 *  116._[TNR_MC__]_0xc80041a4_[0____]_1_UINT8__m_c[13]_:_tnr_ctrl_25,_reg_flat_y_blur_en,
 *  117._[TNR_MC__]_0xc80041a4_[5:4__]_2_UINT8__m_c[14]_:_tnr_ctrl_25,_reg_flat_y_cut_resolution,
 *  118._[TNR_MC__]_0xc80041a4_[13:8_]_6_UINT8__m_c[15]_:_tnr_ctrl_25,_reg_flat_y_G0,
 *  119._[TNR_MC__]_0xc80041a4_[31:28]_4_UINT8__m_c[16]_:_tnr_ctrl_25,_reg_flat_y_G1,
 *  120._[TNR_MC__]_0xc80041a4_[27:24]_4_UINT8__m_c[17]_:_tnr_ctrl_25,_reg_flat_y_G2,
 *  121._[TNR_MC__]_0xc80041a4_[23:20]_4_UINT8__m_c[18]_:_tnr_ctrl_25,_reg_flat_y_G3,
 *  122._[TNR_MC__]_0xc80041a4_[19:16]_4_UINT8__m_c[19]_:_tnr_ctrl_25,_reg_flat_y_G4,
 *  123._[TNR_MC__]_0xc80041a8_[0____]_1_UINT8__m_c[20]_:_tnr_ctrl_26,_reg_flat_c_blur_en,
 *  124._[TNR_MC__]_0xc80041a8_[5:4__]_2_UINT8__m_c[21]_:_tnr_ctrl_26,_reg_flat_c_cut_resolution,
 *  125._[TNR_MC__]_0xc80041a8_[13:8_]_6_UINT8__m_c[22]_:_tnr_ctrl_26,_reg_flat_c_G0,
 *  126._[TNR_MC__]_0xc80041a8_[31:28]_4_UINT8__m_c[23]_:_tnr_ctrl_26,_reg_flat_c_G1,
 *  127._[TNR_MC__]_0xc80041a8_[27:24]_4_UINT8__m_c[24]_:_tnr_ctrl_26,_reg_flat_c_G2,
 *  128._[TNR_MC__]_0xc80041a8_[23:20]_4_UINT8__m_c[25]_:_tnr_ctrl_26,_reg_flat_c_G3,
 *  129._[TNR_MC__]_0xc80041a8_[19:16]_4_UINT8__m_c[26]_:_tnr_ctrl_26,_reg_flat_c_G4,
 *
 * @param   *pstParams [in/out] void
 * @return  OK if success, ERROR otherwise.
 * @see		set discription
 * @author
 */
int PE_NRD_HW_H15_GetTnrDetailCtrl(void *pstParams)
{
	int ret = RET_OK;
	#ifdef PE_USE_H15_NEW_DTNR_STRUCT
	LX_PE_WIN_ID win_id;
	do{
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H15)
		{
			LX_PE_NRD_TNR5_DETAIL_T *pp=(LX_PE_NRD_TNR5_DETAIL_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			win_id = PE_GET_CHECKED_WINID(pp->win_id);
			if(PE_CHECK_WIN0(win_id))
			{
				PE_ND_ND1_H15A0_RdFL(tnr_ctrl_00);
				PE_ND_ND1_H15A0_RdFL(tnr_ctrl_02);
				PE_ND_ND1_H15A0_RdFL(tnr_ctrl_09);
				PE_ND_ND1_H15A0_RdFL(tnr_ctrl_11);
				PE_ND_ND1_H15A0_RdFL(tnr_ctrl_18);
				PE_ND_ND1_H15A0_RdFL(tnr_ctrl_19);
				PE_ND_ND1_H15A0_RdFL(tnr_ctrl_06);
				PE_ND_ND1_H15A0_RdFL(tnr_ctrl_20);
				PE_ND_ND1_H15A0_RdFL(tnr_ctrl_40);
				PE_ND_ND1_H15A0_RdFL(tnr_ctrl_42);
				PE_ND_ND1_H15A0_RdFL(tnr_ctrl_43);
				PE_ND_ND1_H15A0_RdFL(tnr_ctrl_44);
				PE_ND_ND1_H15A0_RdFL(tnr_ctrl_45);
				PE_ND_ND1_H15A0_RdFL(tnr_ctrl_46);
				PE_ND_ND1_H15A0_RdFL(tnr_ctrl_47);
				PE_ND_ND1_H15A0_RdFL(tnr_ctrl_24);
				PE_ND_ND1_H15A0_RdFL(tnr_ctrl_29);
				PE_ND_ND1_H15A0_RdFL(tnr_ctrl_30);
				PE_ND_ND1_H15A0_RdFL(tnr_ctrl_31);
				PE_ND_ND1_H15A0_RdFL(tnr_ctrl_25);
				PE_ND_ND1_H15A0_RdFL(tnr_ctrl_26);
				PE_ND_ND1_H15A0_Rd01(tnr_ctrl_00, reg_sad_8x3_res,            pp->s_m[0] );
				PE_ND_ND1_H15A0_Rd01(tnr_ctrl_00, reg_sad_8x3_iir_en,         pp->s_m[1] );
				PE_ND_ND1_H15A0_Rd01(tnr_ctrl_00, reg_sad_8x3_iir_alpha,      pp->s_m[2] );
				PE_ND_ND1_H15A0_Rd01(tnr_ctrl_02, sc_en,                      pp->s_m[3] );
				PE_ND_ND1_H15A0_Rd01(tnr_ctrl_02, sc_th_ma,                   pp->s_m[4] );
				PE_ND_ND1_H15A0_Rd01(tnr_ctrl_09, sad_base,                   pp->s_m[5] );
				PE_ND_ND1_H15A0_Rd01(tnr_ctrl_11, lb_smooth_en,               pp->s_m[6] );
				PE_ND_ND1_H15A0_Rd01(tnr_ctrl_18, bsad_scale,                 pp->s_m[7] );
				PE_ND_ND1_H15A0_Rd01(tnr_ctrl_18, gsad_scale,                 pp->s_m[8] );
				PE_ND_ND1_H15A0_Rd01(tnr_ctrl_18, gmv_gmv_tl_en,              pp->s_m[9] );
				PE_ND_ND1_H15A0_Rd01(tnr_ctrl_18, bmv_gmv_tl_en,              pp->s_m[10]);
				PE_ND_ND1_H15A0_Rd01(tnr_ctrl_18, bmv_pmv_tl_en,              pp->s_m[11]);
				PE_ND_ND1_H15A0_Rd01(tnr_ctrl_18, gmv_gmv_tl,                 pp->s_m[12]);
				PE_ND_ND1_H15A0_Rd01(tnr_ctrl_18, bmv_gmv_tl,                 pp->s_m[13]);
				PE_ND_ND1_H15A0_Rd01(tnr_ctrl_18, bmv_pmv_tl,                 pp->s_m[14]);
				PE_ND_ND1_H15A0_Rd01(tnr_ctrl_19, gmv_0mv_tl_en,              pp->s_m[15]);
				PE_ND_ND1_H15A0_Rd01(tnr_ctrl_19, bmv_0mv_tl_en,              pp->s_m[16]);
				PE_ND_ND1_H15A0_Rd01(tnr_ctrl_19, gmv_0mv_tl,                 pp->s_m[17]);
				PE_ND_ND1_H15A0_Rd01(tnr_ctrl_19, bmv_0mv_tl,                 pp->s_m[18]);
				PE_ND_ND1_H15A0_Rd01(tnr_ctrl_09, lb_smooth_clip,             pp->s_m[19]);
				PE_ND_ND1_H15A0_Rd01(tnr_ctrl_06, me_lb_sad_base,             pp->s_m[20]);
				PE_ND_ND1_H15A0_Rd01(tnr_ctrl_11, pv_smooth_en,               pp->s_m[21]);
				PE_ND_ND1_H15A0_Rd01(tnr_ctrl_09, pv_smooth_clip,             pp->s_m[22]);
				PE_ND_ND1_H15A0_Rd01(tnr_ctrl_06, pv_smooth_sad_base,         pp->s_m[23]);
				PE_ND_ND1_H15A0_Rd01(tnr_ctrl_20, reg_edge_alpha_mul,         pp->edf[0] );
				PE_ND_ND1_H15A0_Rd01(tnr_ctrl_20, reg_edge_beta_mul,          pp->edf[1] );
				PE_ND_ND1_H15A0_Rd01(tnr_ctrl_20, reg_detail_alpha_mul,       pp->edf[2] );
				PE_ND_ND1_H15A0_Rd01(tnr_ctrl_20, reg_detail_beta_mul,        pp->edf[3] );
				PE_ND_ND1_H15A0_Rd01(tnr_ctrl_40, reg_t_cut_resolution,       pp->edf[4] );
				PE_ND_ND1_H15A0_Rd01(tnr_ctrl_42, reg_a_lut_edge_x1,          pp->edf[5] );
				PE_ND_ND1_H15A0_Rd01(tnr_ctrl_42, reg_a_lut_edge_y1,          pp->edf[6] );
				PE_ND_ND1_H15A0_Rd01(tnr_ctrl_42, reg_a_lut_edge_x0,          pp->edf[7] );
				PE_ND_ND1_H15A0_Rd01(tnr_ctrl_42, reg_a_lut_edge_y0,          pp->edf[8] );
				PE_ND_ND1_H15A0_Rd01(tnr_ctrl_43, reg_a_lut_detail_x1,        pp->edf[9] );
				PE_ND_ND1_H15A0_Rd01(tnr_ctrl_43, reg_a_lut_detail_y1,        pp->edf[10]);
				PE_ND_ND1_H15A0_Rd01(tnr_ctrl_43, reg_a_lut_detail_x0,        pp->edf[11]);
				PE_ND_ND1_H15A0_Rd01(tnr_ctrl_43, reg_a_lut_detail_y0,        pp->edf[12]);
				PE_ND_ND1_H15A0_Rd01(tnr_ctrl_44, reg_t_lut_edge_x1,          pp->edf[13]);
				PE_ND_ND1_H15A0_Rd01(tnr_ctrl_44, reg_t_lut_edge_y1,          pp->edf[14]);
				PE_ND_ND1_H15A0_Rd01(tnr_ctrl_44, reg_t_lut_edge_x0,          pp->edf[15]);
				PE_ND_ND1_H15A0_Rd01(tnr_ctrl_44, reg_t_lut_edge_y0,          pp->edf[16]);
				PE_ND_ND1_H15A0_Rd01(tnr_ctrl_45, reg_t_lut_detail_x1,        pp->edf[17]);
				PE_ND_ND1_H15A0_Rd01(tnr_ctrl_45, reg_t_lut_detail_y1,        pp->edf[18]);
				PE_ND_ND1_H15A0_Rd01(tnr_ctrl_45, reg_t_lut_detail_x0,        pp->edf[19]);
				PE_ND_ND1_H15A0_Rd01(tnr_ctrl_45, reg_t_lut_detail_y0,        pp->edf[20]);
				PE_ND_ND1_H15A0_Rd01(tnr_ctrl_46, reg_final_lut_edge_x1,      pp->edf[21]);
				PE_ND_ND1_H15A0_Rd01(tnr_ctrl_46, reg_final_lut_edge_y1,      pp->edf[22]);
				PE_ND_ND1_H15A0_Rd01(tnr_ctrl_46, reg_final_lut_edge_x0,      pp->edf[23]);
				PE_ND_ND1_H15A0_Rd01(tnr_ctrl_46, reg_final_lut_edge_y0,      pp->edf[24]);
				PE_ND_ND1_H15A0_Rd01(tnr_ctrl_47, reg_final_lut_detail_x1,    pp->edf[25]);
				PE_ND_ND1_H15A0_Rd01(tnr_ctrl_47, reg_final_lut_detail_y1,    pp->edf[26]);
				PE_ND_ND1_H15A0_Rd01(tnr_ctrl_47, reg_final_lut_detail_x0,    pp->edf[27]);
				PE_ND_ND1_H15A0_Rd01(tnr_ctrl_47, reg_final_lut_detail_y0,    pp->edf[28]);
				PE_ND_ND1_H15A0_Rd01(tnr_ctrl_24, reg_luma_gain_en,           pp->m_c[0] );
				PE_ND_ND1_H15A0_Rd01(tnr_ctrl_24, reg_skin_gain_en,           pp->m_c[1] );
				PE_ND_ND1_H15A0_Rd01(tnr_ctrl_29, reg_dir_blur_en,            pp->m_c[2] );
				PE_ND_ND1_H15A0_Rd01(tnr_ctrl_29, reg_dir_blur_gain,          pp->m_c[3] );
				PE_ND_ND1_H15A0_Rd01(tnr_ctrl_29, reg_diff_limit,             pp->m_c[4] );
				PE_ND_ND1_H15A0_Rd01(tnr_ctrl_30, reg_denoise_filter_en,      pp->m_c[5] );
				PE_ND_ND1_H15A0_Rd01(tnr_ctrl_30, reg_denoise_gain,           pp->m_c[6] );
				PE_ND_ND1_H15A0_Rd01(tnr_ctrl_30, reg_denoise_cut_resolution, pp->m_c[7] );
				PE_ND_ND1_H15A0_Rd01(tnr_ctrl_30, reg_denoise_G0,             pp->m_c[8] );
				PE_ND_ND1_H15A0_Rd01(tnr_ctrl_31, reg_denoise_G1,             pp->m_c[9] );
				PE_ND_ND1_H15A0_Rd01(tnr_ctrl_31, reg_denoise_G2,             pp->m_c[10]);
				PE_ND_ND1_H15A0_Rd01(tnr_ctrl_31, reg_denoise_G3,             pp->m_c[11]);
				PE_ND_ND1_H15A0_Rd01(tnr_ctrl_31, reg_denoise_G4,             pp->m_c[12]);
				PE_ND_ND1_H15A0_Rd01(tnr_ctrl_25, reg_flat_y_blur_en,         pp->m_c[13]);
				PE_ND_ND1_H15A0_Rd01(tnr_ctrl_25, reg_flat_y_cut_resolution,  pp->m_c[14]);
				PE_ND_ND1_H15A0_Rd01(tnr_ctrl_25, reg_flat_y_G0,              pp->m_c[15]);
				PE_ND_ND1_H15A0_Rd01(tnr_ctrl_25, reg_flat_y_G1,              pp->m_c[16]);
				PE_ND_ND1_H15A0_Rd01(tnr_ctrl_25, reg_flat_y_G2,              pp->m_c[17]);
				PE_ND_ND1_H15A0_Rd01(tnr_ctrl_25, reg_flat_y_G3,              pp->m_c[18]);
				PE_ND_ND1_H15A0_Rd01(tnr_ctrl_25, reg_flat_y_G4,              pp->m_c[19]);
				PE_ND_ND1_H15A0_Rd01(tnr_ctrl_26, reg_flat_c_blur_en,         pp->m_c[20]);
				PE_ND_ND1_H15A0_Rd01(tnr_ctrl_26, reg_flat_c_cut_resolution,  pp->m_c[21]);
				PE_ND_ND1_H15A0_Rd01(tnr_ctrl_26, reg_flat_c_G0,              pp->m_c[22]);
				PE_ND_ND1_H15A0_Rd01(tnr_ctrl_26, reg_flat_c_G1,              pp->m_c[23]);
				PE_ND_ND1_H15A0_Rd01(tnr_ctrl_26, reg_flat_c_G2,              pp->m_c[24]);
				PE_ND_ND1_H15A0_Rd01(tnr_ctrl_26, reg_flat_c_G3,              pp->m_c[25]);
				PE_ND_ND1_H15A0_Rd01(tnr_ctrl_26, reg_flat_c_G4,              pp->m_c[26]);
			}
			if(PE_CHECK_WIN1(win_id))
			{
				PE_ND_ND2_H15A0_RdFL(tnr_ctrl_00);
				PE_ND_ND2_H15A0_RdFL(tnr_ctrl_02);
				PE_ND_ND2_H15A0_RdFL(tnr_ctrl_09);
				PE_ND_ND2_H15A0_RdFL(tnr_ctrl_11);
				PE_ND_ND2_H15A0_RdFL(tnr_ctrl_18);
				PE_ND_ND2_H15A0_RdFL(tnr_ctrl_19);
				PE_ND_ND2_H15A0_RdFL(tnr_ctrl_06);
				PE_ND_ND2_H15A0_RdFL(tnr_ctrl_20);
				PE_ND_ND2_H15A0_RdFL(tnr_ctrl_40);
				PE_ND_ND2_H15A0_RdFL(tnr_ctrl_42);
				PE_ND_ND2_H15A0_RdFL(tnr_ctrl_43);
				PE_ND_ND2_H15A0_RdFL(tnr_ctrl_44);
				PE_ND_ND2_H15A0_RdFL(tnr_ctrl_45);
				PE_ND_ND2_H15A0_RdFL(tnr_ctrl_46);
				PE_ND_ND2_H15A0_RdFL(tnr_ctrl_47);
				PE_ND_ND2_H15A0_RdFL(tnr_ctrl_24);
				PE_ND_ND2_H15A0_RdFL(tnr_ctrl_29);
				PE_ND_ND2_H15A0_RdFL(tnr_ctrl_30);
				PE_ND_ND2_H15A0_RdFL(tnr_ctrl_31);
				PE_ND_ND2_H15A0_RdFL(tnr_ctrl_25);
				PE_ND_ND2_H15A0_RdFL(tnr_ctrl_26);
				PE_ND_ND2_H15A0_Rd01(tnr_ctrl_00, reg_sad_8x3_res,            pp->s_m[0] );
				PE_ND_ND2_H15A0_Rd01(tnr_ctrl_00, reg_sad_8x3_iir_en,         pp->s_m[1] );
				PE_ND_ND2_H15A0_Rd01(tnr_ctrl_00, reg_sad_8x3_iir_alpha,      pp->s_m[2] );
				PE_ND_ND2_H15A0_Rd01(tnr_ctrl_02, sc_en,                      pp->s_m[3] );
				PE_ND_ND2_H15A0_Rd01(tnr_ctrl_02, sc_th_ma,                   pp->s_m[4] );
				PE_ND_ND2_H15A0_Rd01(tnr_ctrl_09, sad_base,                   pp->s_m[5] );
				PE_ND_ND2_H15A0_Rd01(tnr_ctrl_11, lb_smooth_en,               pp->s_m[6] );
				PE_ND_ND2_H15A0_Rd01(tnr_ctrl_18, bsad_scale,                 pp->s_m[7] );
				PE_ND_ND2_H15A0_Rd01(tnr_ctrl_18, gsad_scale,                 pp->s_m[8] );
				PE_ND_ND2_H15A0_Rd01(tnr_ctrl_18, gmv_gmv_tl_en,              pp->s_m[9] );
				PE_ND_ND2_H15A0_Rd01(tnr_ctrl_18, bmv_gmv_tl_en,              pp->s_m[10]);
				PE_ND_ND2_H15A0_Rd01(tnr_ctrl_18, bmv_pmv_tl_en,              pp->s_m[11]);
				PE_ND_ND2_H15A0_Rd01(tnr_ctrl_18, gmv_gmv_tl,                 pp->s_m[12]);
				PE_ND_ND2_H15A0_Rd01(tnr_ctrl_18, bmv_gmv_tl,                 pp->s_m[13]);
				PE_ND_ND2_H15A0_Rd01(tnr_ctrl_18, bmv_pmv_tl,                 pp->s_m[14]);
				PE_ND_ND2_H15A0_Rd01(tnr_ctrl_19, gmv_0mv_tl_en,              pp->s_m[15]);
				PE_ND_ND2_H15A0_Rd01(tnr_ctrl_19, bmv_0mv_tl_en,              pp->s_m[16]);
				PE_ND_ND2_H15A0_Rd01(tnr_ctrl_19, gmv_0mv_tl,                 pp->s_m[17]);
				PE_ND_ND2_H15A0_Rd01(tnr_ctrl_19, bmv_0mv_tl,                 pp->s_m[18]);
				PE_ND_ND2_H15A0_Rd01(tnr_ctrl_09, lb_smooth_clip,             pp->s_m[19]);
				PE_ND_ND2_H15A0_Rd01(tnr_ctrl_06, me_lb_sad_base,             pp->s_m[20]);
				PE_ND_ND2_H15A0_Rd01(tnr_ctrl_11, pv_smooth_en,               pp->s_m[21]);
				PE_ND_ND2_H15A0_Rd01(tnr_ctrl_09, pv_smooth_clip,             pp->s_m[22]);
				PE_ND_ND2_H15A0_Rd01(tnr_ctrl_06, pv_smooth_sad_base,         pp->s_m[23]);
				PE_ND_ND2_H15A0_Rd01(tnr_ctrl_20, reg_edge_alpha_mul,         pp->edf[0] );
				PE_ND_ND2_H15A0_Rd01(tnr_ctrl_20, reg_edge_beta_mul,          pp->edf[1] );
				PE_ND_ND2_H15A0_Rd01(tnr_ctrl_20, reg_detail_alpha_mul,       pp->edf[2] );
				PE_ND_ND2_H15A0_Rd01(tnr_ctrl_20, reg_detail_beta_mul,        pp->edf[3] );
				PE_ND_ND2_H15A0_Rd01(tnr_ctrl_40, reg_t_cut_resolution,       pp->edf[4] );
				PE_ND_ND2_H15A0_Rd01(tnr_ctrl_42, reg_a_lut_edge_x1,          pp->edf[5] );
				PE_ND_ND2_H15A0_Rd01(tnr_ctrl_42, reg_a_lut_edge_y1,          pp->edf[6] );
				PE_ND_ND2_H15A0_Rd01(tnr_ctrl_42, reg_a_lut_edge_x0,          pp->edf[7] );
				PE_ND_ND2_H15A0_Rd01(tnr_ctrl_42, reg_a_lut_edge_y0,          pp->edf[8] );
				PE_ND_ND2_H15A0_Rd01(tnr_ctrl_43, reg_a_lut_detail_x1,        pp->edf[9] );
				PE_ND_ND2_H15A0_Rd01(tnr_ctrl_43, reg_a_lut_detail_y1,        pp->edf[10]);
				PE_ND_ND2_H15A0_Rd01(tnr_ctrl_43, reg_a_lut_detail_x0,        pp->edf[11]);
				PE_ND_ND2_H15A0_Rd01(tnr_ctrl_43, reg_a_lut_detail_y0,        pp->edf[12]);
				PE_ND_ND2_H15A0_Rd01(tnr_ctrl_44, reg_t_lut_edge_x1,          pp->edf[13]);
				PE_ND_ND2_H15A0_Rd01(tnr_ctrl_44, reg_t_lut_edge_y1,          pp->edf[14]);
				PE_ND_ND2_H15A0_Rd01(tnr_ctrl_44, reg_t_lut_edge_x0,          pp->edf[15]);
				PE_ND_ND2_H15A0_Rd01(tnr_ctrl_44, reg_t_lut_edge_y0,          pp->edf[16]);
				PE_ND_ND2_H15A0_Rd01(tnr_ctrl_45, reg_t_lut_detail_x1,        pp->edf[17]);
				PE_ND_ND2_H15A0_Rd01(tnr_ctrl_45, reg_t_lut_detail_y1,        pp->edf[18]);
				PE_ND_ND2_H15A0_Rd01(tnr_ctrl_45, reg_t_lut_detail_x0,        pp->edf[19]);
				PE_ND_ND2_H15A0_Rd01(tnr_ctrl_45, reg_t_lut_detail_y0,        pp->edf[20]);
				PE_ND_ND2_H15A0_Rd01(tnr_ctrl_46, reg_final_lut_edge_x1,      pp->edf[21]);
				PE_ND_ND2_H15A0_Rd01(tnr_ctrl_46, reg_final_lut_edge_y1,      pp->edf[22]);
				PE_ND_ND2_H15A0_Rd01(tnr_ctrl_46, reg_final_lut_edge_x0,      pp->edf[23]);
				PE_ND_ND2_H15A0_Rd01(tnr_ctrl_46, reg_final_lut_edge_y0,      pp->edf[24]);
				PE_ND_ND2_H15A0_Rd01(tnr_ctrl_47, reg_final_lut_detail_x1,    pp->edf[25]);
				PE_ND_ND2_H15A0_Rd01(tnr_ctrl_47, reg_final_lut_detail_y1,    pp->edf[26]);
				PE_ND_ND2_H15A0_Rd01(tnr_ctrl_47, reg_final_lut_detail_x0,    pp->edf[27]);
				PE_ND_ND2_H15A0_Rd01(tnr_ctrl_47, reg_final_lut_detail_y0,    pp->edf[28]);
				PE_ND_ND2_H15A0_Rd01(tnr_ctrl_24, reg_luma_gain_en,           pp->m_c[0] );
				PE_ND_ND2_H15A0_Rd01(tnr_ctrl_24, reg_skin_gain_en,           pp->m_c[1] );
				PE_ND_ND2_H15A0_Rd01(tnr_ctrl_29, reg_dir_blur_en,            pp->m_c[2] );
				PE_ND_ND2_H15A0_Rd01(tnr_ctrl_29, reg_dir_blur_gain,          pp->m_c[3] );
				PE_ND_ND2_H15A0_Rd01(tnr_ctrl_29, reg_diff_limit,             pp->m_c[4] );
				PE_ND_ND2_H15A0_Rd01(tnr_ctrl_30, reg_denoise_filter_en,      pp->m_c[5] );
				PE_ND_ND2_H15A0_Rd01(tnr_ctrl_30, reg_denoise_gain,           pp->m_c[6] );
				PE_ND_ND2_H15A0_Rd01(tnr_ctrl_30, reg_denoise_cut_resolution, pp->m_c[7] );
				PE_ND_ND2_H15A0_Rd01(tnr_ctrl_30, reg_denoise_G0,             pp->m_c[8] );
				PE_ND_ND2_H15A0_Rd01(tnr_ctrl_31, reg_denoise_G1,             pp->m_c[9] );
				PE_ND_ND2_H15A0_Rd01(tnr_ctrl_31, reg_denoise_G2,             pp->m_c[10]);
				PE_ND_ND2_H15A0_Rd01(tnr_ctrl_31, reg_denoise_G3,             pp->m_c[11]);
				PE_ND_ND2_H15A0_Rd01(tnr_ctrl_31, reg_denoise_G4,             pp->m_c[12]);
				PE_ND_ND2_H15A0_Rd01(tnr_ctrl_25, reg_flat_y_blur_en,         pp->m_c[13]);
				PE_ND_ND2_H15A0_Rd01(tnr_ctrl_25, reg_flat_y_cut_resolution,  pp->m_c[14]);
				PE_ND_ND2_H15A0_Rd01(tnr_ctrl_25, reg_flat_y_G0,              pp->m_c[15]);
				PE_ND_ND2_H15A0_Rd01(tnr_ctrl_25, reg_flat_y_G1,              pp->m_c[16]);
				PE_ND_ND2_H15A0_Rd01(tnr_ctrl_25, reg_flat_y_G2,              pp->m_c[17]);
				PE_ND_ND2_H15A0_Rd01(tnr_ctrl_25, reg_flat_y_G3,              pp->m_c[18]);
				PE_ND_ND2_H15A0_Rd01(tnr_ctrl_25, reg_flat_y_G4,              pp->m_c[19]);
				PE_ND_ND2_H15A0_Rd01(tnr_ctrl_26, reg_flat_c_blur_en,         pp->m_c[20]);
				PE_ND_ND2_H15A0_Rd01(tnr_ctrl_26, reg_flat_c_cut_resolution,  pp->m_c[21]);
				PE_ND_ND2_H15A0_Rd01(tnr_ctrl_26, reg_flat_c_G0,              pp->m_c[22]);
				PE_ND_ND2_H15A0_Rd01(tnr_ctrl_26, reg_flat_c_G1,              pp->m_c[23]);
				PE_ND_ND2_H15A0_Rd01(tnr_ctrl_26, reg_flat_c_G2,              pp->m_c[24]);
				PE_ND_ND2_H15A0_Rd01(tnr_ctrl_26, reg_flat_c_G3,              pp->m_c[25]);
				PE_ND_ND2_H15A0_Rd01(tnr_ctrl_26, reg_flat_c_G4,              pp->m_c[26]);
			}
			if (_g_nrd_hw_h15_trace)
			{
				UINT8 *pd = pp->s_m;
				PE_NRD_HW_H15_DBG_PRINT("[%d]:\n"\
					"s_m[00]0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,\n"\
					"s_m[10]0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,\n"\
					"s_m[20]0x%02X,0x%02X,0x%02X,0x%02X,\n", \
					pp->win_id, \
					pd[0],  pd[1],  pd[2],  pd[3],  pd[4],  pd[5],  pd[6],  pd[7],  pd[8],  pd[9], \
					pd[10], pd[11], pd[12], pd[13], pd[14], pd[15], pd[16], pd[17], pd[18], pd[19], \
					pd[20], pd[21], pd[22], pd[23]);
				pd = pp->edf;
				PE_NRD_HW_H15_DBG_PRINT("[%d]:\n"\
					"edf[00]0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,\n"\
					"edf[10]0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,\n"\
					"edf[20]0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,\n", \
					pp->win_id, \
					pd[0],  pd[1],  pd[2],  pd[3],  pd[4],  pd[5],  pd[6],  pd[7],  pd[8],  pd[9], \
					pd[10], pd[11], pd[12], pd[13], pd[14], pd[15], pd[16], pd[17], pd[18], pd[19], \
					pd[20], pd[21], pd[22], pd[23], pd[24], pd[25], pd[26], pd[27], pd[28]);
				pd = pp->m_c;
				PE_NRD_HW_H15_DBG_PRINT("[%d]:\n"\
					"m_c[00]0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,\n"\
					"m_c[10]0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,\n"\
					"m_c[20]0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,\n", \
					pp->win_id, \
					pd[0],  pd[1],  pd[2],  pd[3],  pd[4],  pd[5],  pd[6],  pd[7],  pd[8],  pd[9], \
					pd[10], pd[11], pd[12], pd[13], pd[14], pd[15], pd[16], pd[17], pd[18], pd[19], \
					pd[20], pd[21], pd[22], pd[23], pd[24], pd[25], pd[26]);
			}
		}
		else
		{
			PE_NRD_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	#endif
	return ret;
}
/**
 * set tnr lb ctrl
 *
 * @param   *pCfg [in] PE_TSK_H15_CFG_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_NRD_HW_H15_SetTnrLbMode(PE_TSK_H15_CFG_T *pCfg)
{
	int ret = RET_OK;
	#ifdef PE_HW_H15_PROTECT_OVERLAP_SETTINGS
	static UINT32 count = 0;
	static PE_H15A0_ND1_PE0_SRC_SIZE_T pre_nd1_pe0_src_size = {
		.hsize = 0,
		.vsize = 0,
		.scan_type = 0,
		};
	static PE_H15A0_ND2_PE0_SRC_SIZE_T pre_nd2_pe0_src_size = {
		.hsize = 0,
		.vsize = 0,
		.scan_type = 0,
		};
	static PE_H15A0_ND1_TNR_CTRL_16_T pre_nd1_tnr_ctrl_16 = {
		.reg_n_hblk = 0,
		.reg_n_vblk = 0,
		};
	static PE_H15A0_ND1_TNR_CTRL_17_T pre_nd1_tnr_ctrl_17 = {
		.reg_blk_vsize = 0,
		.reg_blk_hsize = 0,
		};
	static PE_H15A0_ND2_TNR_CTRL_16_T pre_nd2_tnr_ctrl_16 = {
		.reg_n_hblk = 0,
		.reg_n_vblk = 0,
		};
	static PE_H15A0_ND2_TNR_CTRL_17_T pre_nd2_tnr_ctrl_17 = {
		.reg_blk_vsize = 0,
		.reg_blk_hsize = 0,
		};
	PE_H15A0_ND1_TNR_CTRL_16_T cur_nd1_tnr_ctrl_16;
	PE_H15A0_ND1_TNR_CTRL_17_T cur_nd1_tnr_ctrl_17;
	PE_H15A0_ND2_TNR_CTRL_16_T cur_nd2_tnr_ctrl_16;
	PE_H15A0_ND2_TNR_CTRL_17_T cur_nd2_tnr_ctrl_17;
	PE_INF_H15_CTRL_T *ctrl0_info = &pCfg->inf.ctrl_info[LX_PE_WIN_0];
	PE_INF_H15_CTRL_T *ctrl1_info = &pCfg->inf.ctrl_info[LX_PE_WIN_1];
	LX_PE_INF_DISPLAY_T *disp0_info = &pCfg->inf.disp_info[LX_PE_WIN_0];
	do{
		CHECK_KNULL(pCfg);
		if(PE_KDRV_VER_H15)
		{
			/* nd1 nblk setting *****************************************/
			if (pre_nd1_pe0_src_size.hsize != ctrl0_info->pe0_src_h || \
				pre_nd1_pe0_src_size.vsize != ctrl0_info->pe0_src_v || \
				pre_nd1_pe0_src_size.scan_type != disp0_info->scan_type)
			{
				PE_NRD_HW_H15_DBG_PRINT("[inf] nd1 tnr src h:%d->%d, v:%d->%d, sc:%d->%d\n",\
					pre_nd1_pe0_src_size.hsize, ctrl0_info->pe0_src_h, \
					pre_nd1_pe0_src_size.vsize, ctrl0_info->pe0_src_v, \
					pre_nd1_pe0_src_size.scan_type, disp0_info->scan_type);
				/*hblk*/
				if (ctrl0_info->pe0_src_h >= PE_NRD_HW_H15_TNR_SRC_H_HD_BASE)
				{
					/*2048(4k),1568(3k),1920,1360,1280,1024,800*/
					cur_nd1_tnr_ctrl_16.reg_n_hblk = 0x8;// h%8==0:
					cur_nd1_tnr_ctrl_17.reg_blk_hsize = ctrl0_info->pe0_src_h>>3;// h/8
				}
				else//720,704,640
				{
					if (ctrl0_info->pe0_src_h == 704 || \
						ctrl0_info->pe0_src_h <= 640)
					{
						cur_nd1_tnr_ctrl_16.reg_n_hblk = 0x4;// h%4==0
						cur_nd1_tnr_ctrl_17.reg_blk_hsize = ctrl0_info->pe0_src_h>>2;// h/4
					}
					else//720
					{
						cur_nd1_tnr_ctrl_16.reg_n_hblk = 0x6;// h%6==0
						cur_nd1_tnr_ctrl_17.reg_blk_hsize = ctrl0_info->pe0_src_h/6;// h/6
					}
				}
				/*vblk*/
				if (ctrl0_info->pe0_src_v >= PE_NRD_HW_H15_TNR_SRC_V_3K_BASE)
				{
					/*2160(4k),1620(3k)*/
					cur_nd1_tnr_ctrl_16.reg_n_vblk = 0xc;// h%12==0
					if (disp0_info->scan_type == LX_PE_SCAN_PROGRESS)//progressive
					{
						cur_nd1_tnr_ctrl_17.reg_blk_vsize = ctrl0_info->pe0_src_v/12;
					}
					else//interlace
					{
						cur_nd1_tnr_ctrl_17.reg_blk_vsize = ctrl0_info->pe0_src_v/24;
					}
				}
				else//1080,720,480
				{
					cur_nd1_tnr_ctrl_16.reg_n_vblk = 0x6;// h%6==0
					if (disp0_info->scan_type == LX_PE_SCAN_PROGRESS)//progressive
					{
						cur_nd1_tnr_ctrl_17.reg_blk_vsize = ctrl0_info->pe0_src_v/6;
					}
					else//interlace
					{
						cur_nd1_tnr_ctrl_17.reg_blk_vsize = ctrl0_info->pe0_src_v/12;
					}
				}
				if (pre_nd1_tnr_ctrl_16.reg_n_hblk != cur_nd1_tnr_ctrl_16.reg_n_hblk || \
					pre_nd1_tnr_ctrl_16.reg_n_vblk != cur_nd1_tnr_ctrl_16.reg_n_vblk)
				{
					PE_NRD_HW_H15_DBG_PRINT("[set] nd1 tnr n_blk h:0x%x->0x%x, v:0x%x->0x%x\n",\
						pre_nd1_tnr_ctrl_16.reg_n_hblk, cur_nd1_tnr_ctrl_16.reg_n_hblk, \
						pre_nd1_tnr_ctrl_16.reg_n_vblk, cur_nd1_tnr_ctrl_16.reg_n_vblk);
					PE_NRD_HW_H15A_TNR1_WR02(tnr_ctrl_16,reg_n_hblk,cur_nd1_tnr_ctrl_16.reg_n_hblk, \
					                                     reg_n_vblk,cur_nd1_tnr_ctrl_16.reg_n_vblk);
					pre_nd1_tnr_ctrl_16.reg_n_hblk = cur_nd1_tnr_ctrl_16.reg_n_hblk;
					pre_nd1_tnr_ctrl_16.reg_n_vblk = cur_nd1_tnr_ctrl_16.reg_n_vblk;
				}
				if (pre_nd1_tnr_ctrl_17.reg_blk_vsize != cur_nd1_tnr_ctrl_17.reg_blk_vsize || \
					pre_nd1_tnr_ctrl_17.reg_blk_hsize != cur_nd1_tnr_ctrl_17.reg_blk_hsize)
				{
					PE_NRD_HW_H15_DBG_PRINT("[set] nd1 tnr blk_size h:%d->%d, v:%d->%d\n",\
						pre_nd1_tnr_ctrl_17.reg_blk_hsize, cur_nd1_tnr_ctrl_17.reg_blk_hsize, \
						pre_nd1_tnr_ctrl_17.reg_blk_vsize, cur_nd1_tnr_ctrl_17.reg_blk_vsize);
					PE_NRD_HW_H15A_TNR1_WR02(tnr_ctrl_17,reg_blk_vsize,cur_nd1_tnr_ctrl_17.reg_blk_vsize, \
					                                     reg_blk_hsize,cur_nd1_tnr_ctrl_17.reg_blk_hsize);
					pre_nd1_tnr_ctrl_17.reg_blk_vsize = cur_nd1_tnr_ctrl_17.reg_blk_vsize;
					pre_nd1_tnr_ctrl_17.reg_blk_hsize = cur_nd1_tnr_ctrl_17.reg_blk_hsize;
				}
				pre_nd1_pe0_src_size.hsize = ctrl0_info->pe0_src_h;
				pre_nd1_pe0_src_size.vsize = ctrl0_info->pe0_src_v;
				pre_nd1_pe0_src_size.scan_type = disp0_info->scan_type;
			}
			/* nd2 nblk setting *****************************************/
			if (pre_nd2_pe0_src_size.hsize != ctrl1_info->pe0_src_h || \
				pre_nd2_pe0_src_size.vsize != ctrl1_info->pe0_src_v || \
				pre_nd2_pe0_src_size.scan_type != disp0_info->scan_type)
			{
				PE_NRD_HW_H15_DBG_PRINT("[inf] nd2 tnr src h:%d->%d, v:%d->%d, sc:%d->%d\n",\
					pre_nd2_pe0_src_size.hsize, ctrl1_info->pe0_src_h, \
					pre_nd2_pe0_src_size.vsize, ctrl1_info->pe0_src_v, \
					pre_nd2_pe0_src_size.scan_type, disp0_info->scan_type);
				/*hblk*/
				if (ctrl1_info->pe0_src_h >= PE_NRD_HW_H15_TNR_SRC_H_HD_BASE)
				{
					/*2048(4k),1568(3k),1920,1360,1280,1024,800*/
					cur_nd2_tnr_ctrl_16.reg_n_hblk = 0x8;// h%8==0:
					cur_nd2_tnr_ctrl_17.reg_blk_hsize = ctrl1_info->pe0_src_h>>3;// h/8
				}
				else//720,704,640
				{
					if (ctrl1_info->pe0_src_h == 704 || \
						ctrl1_info->pe0_src_h <= 640)
					{
						cur_nd2_tnr_ctrl_16.reg_n_hblk = 0x4;// h%4==0
						cur_nd2_tnr_ctrl_17.reg_blk_hsize = ctrl1_info->pe0_src_h>>2;// h/4
					}
					else//720
					{
						cur_nd2_tnr_ctrl_16.reg_n_hblk = 0x6;// h%6==0
						cur_nd2_tnr_ctrl_17.reg_blk_hsize = ctrl1_info->pe0_src_h/6;// h/6
					}
				}
				/*vblk*/
				if (ctrl0_info->pe0_src_v >= PE_NRD_HW_H15_TNR_SRC_V_3K_BASE)
				{
					/*2160(4k),1620(3k)*/
					cur_nd2_tnr_ctrl_16.reg_n_vblk = 0xc;// h%12==0
					if (disp0_info->scan_type == LX_PE_SCAN_PROGRESS)//progressive
					{
						cur_nd2_tnr_ctrl_17.reg_blk_vsize = ctrl0_info->pe0_src_v/12;
					}
					else//interlace
					{
						cur_nd2_tnr_ctrl_17.reg_blk_vsize = ctrl0_info->pe0_src_v/24;
					}
				}
				else//1080,720,480
				{
					cur_nd2_tnr_ctrl_16.reg_n_vblk = 0x6;// h%6==0
					if (disp0_info->scan_type == LX_PE_SCAN_PROGRESS)//progressive
					{
						cur_nd2_tnr_ctrl_17.reg_blk_vsize = ctrl0_info->pe0_src_v/6;
					}
					else//interlace
					{
						cur_nd2_tnr_ctrl_17.reg_blk_vsize = ctrl0_info->pe0_src_v/12;
					}
				}
				if (pre_nd2_tnr_ctrl_16.reg_n_hblk != cur_nd2_tnr_ctrl_16.reg_n_hblk || \
					pre_nd2_tnr_ctrl_16.reg_n_vblk != cur_nd2_tnr_ctrl_16.reg_n_vblk)
				{
					PE_NRD_HW_H15_DBG_PRINT("[set] nd2 tnr n_blk h:0x%x->0x%x, v:0x%x->0x%x\n",\
						pre_nd2_tnr_ctrl_16.reg_n_hblk, cur_nd2_tnr_ctrl_16.reg_n_hblk, \
						pre_nd2_tnr_ctrl_16.reg_n_vblk, cur_nd2_tnr_ctrl_16.reg_n_vblk);
					PE_NRD_HW_H15A_TNR2_WR02(tnr_ctrl_16,reg_n_hblk,cur_nd2_tnr_ctrl_16.reg_n_hblk, \
					                                     reg_n_vblk,cur_nd2_tnr_ctrl_16.reg_n_vblk);
					pre_nd2_tnr_ctrl_16.reg_n_hblk = cur_nd2_tnr_ctrl_16.reg_n_hblk;
					pre_nd2_tnr_ctrl_16.reg_n_vblk = cur_nd2_tnr_ctrl_16.reg_n_vblk;
				}
				if (pre_nd2_tnr_ctrl_17.reg_blk_vsize != cur_nd2_tnr_ctrl_17.reg_blk_vsize || \
					pre_nd2_tnr_ctrl_17.reg_blk_hsize != cur_nd2_tnr_ctrl_17.reg_blk_hsize)
				{
					PE_NRD_HW_H15_DBG_PRINT("[set] nd2 tnr blk_size h:%d->%d, v:%d->%d\n",\
						pre_nd2_tnr_ctrl_17.reg_blk_hsize, cur_nd2_tnr_ctrl_17.reg_blk_hsize, \
						pre_nd2_tnr_ctrl_17.reg_blk_vsize, cur_nd2_tnr_ctrl_17.reg_blk_vsize);
					PE_NRD_HW_H15A_TNR2_WR02(tnr_ctrl_17,reg_blk_vsize,cur_nd2_tnr_ctrl_17.reg_blk_vsize, \
					                                     reg_blk_hsize,cur_nd2_tnr_ctrl_17.reg_blk_hsize);
					pre_nd2_tnr_ctrl_17.reg_blk_vsize = cur_nd2_tnr_ctrl_17.reg_blk_vsize;
					pre_nd2_tnr_ctrl_17.reg_blk_hsize = cur_nd2_tnr_ctrl_17.reg_blk_hsize;
				}
				pre_nd2_pe0_src_size.hsize = ctrl1_info->pe0_src_h;
				pre_nd2_pe0_src_size.vsize = ctrl1_info->pe0_src_v;
				pre_nd2_pe0_src_size.scan_type = disp0_info->scan_type;
			}
			/* **********************************************************/
			if(_g_nrd_hw_h15_trace)
			{
				if(count>PE_PRINT_COUNT_NUM)
				{
					PE_NRD_HW_H15_DBG_PRINT("[state] nd1 tnr src h:%d, v:%d, sc:%d\n",\
						pre_nd1_pe0_src_size.hsize, pre_nd1_pe0_src_size.vsize, \
						pre_nd1_pe0_src_size.scan_type);
					PE_NRD_HW_H15_DBG_PRINT("[state] nd1 tnr n_blk h:0x%x, v:0x%x\n",\
						pre_nd1_tnr_ctrl_16.reg_n_hblk, pre_nd1_tnr_ctrl_16.reg_n_vblk);
					PE_NRD_HW_H15_DBG_PRINT("[state] nd1 tnr blk_size h:%d, v:%d\n",\
						pre_nd1_tnr_ctrl_17.reg_blk_hsize, pre_nd1_tnr_ctrl_17.reg_blk_vsize);
					PE_NRD_HW_H15_DBG_PRINT("[state] nd2 tnr src h:%d, v:%d, sc:%d\n",\
						pre_nd2_pe0_src_size.hsize, pre_nd2_pe0_src_size.vsize, \
						pre_nd2_pe0_src_size.scan_type);
					PE_NRD_HW_H15_DBG_PRINT("[state] nd2 tnr n_blk h:0x%x, v:0x%x\n",\
						pre_nd2_tnr_ctrl_16.reg_n_hblk, pre_nd2_tnr_ctrl_16.reg_n_vblk);
					PE_NRD_HW_H15_DBG_PRINT("[state] nd2 tnr blk_size h:%d, v:%d\n",\
						pre_nd2_tnr_ctrl_17.reg_blk_hsize, pre_nd2_tnr_ctrl_17.reg_blk_vsize);
					count=0;
				}
				count++;
			}
		}
		else
		{
			PE_NRD_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	#endif
	return ret;
}
/**
 * set tnr pv smooth en mode
 *
 * @param   *pCfg [in] PE_TSK_H15_CFG_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_NRD_HW_H15_SetTnrSmoothMode(PE_TSK_H15_CFG_T *pCfg)
{
	int ret = RET_OK;
	#ifdef PE_HW_H15_PROTECT_OVERLAP_SETTINGS
	static UINT32 count = 0;
	static UINT32 pre_pv_smooth_en = 0xff;
	UINT32 cur_pv_smooth_en, cur_m_en;
	PE_TNR_HW_PARAM_REG_H15A0_T *p_tnr_dflt = _g_pe_tnr_hw_param_data_h15a.reg_dflt.data;
	PE_NRD_HW_H15_SETTINGS_T *pInfo = &_g_pe_nrd_hw_h15_info;
	do{
		CHECK_KNULL(pCfg);
		if(PE_KDRV_VER_H15)
		{
			/* if m_en off, pv_smooth_en off, otherwise, refer default or user */
			PE_ND_ND1_H15A0_QRd01(pe0_opmode,tnr_m_en,cur_m_en);
			cur_pv_smooth_en = (cur_m_en==0)? 0x0:\
				(pInfo->tnr_pv_smooth_en_mask)? \
				pInfo->tnr_pv_smooth_en_user:\
				p_tnr_dflt->tnr_ctrl_11.pv_smooth_en;
			/* compare pre vs. cur */
			if (pre_pv_smooth_en != cur_pv_smooth_en)
			{
				PE_NRD_HW_H15_DBG_PRINT("[inf] tnr_m_en:%d\n", cur_m_en);
				PE_NRD_HW_H15_DBG_PRINT("[set] u,m,d:%d,%d,%d, pv_smooth_en:%d -> %d\n", \
					pInfo->tnr_pv_smooth_en_user, pInfo->tnr_pv_smooth_en_mask, \
					p_tnr_dflt->tnr_ctrl_11.pv_smooth_en, \
					pre_pv_smooth_en, cur_pv_smooth_en);
				PE_NRD_HW_H15A_TNR12_WR01(tnr_ctrl_11,pv_smooth_en,cur_pv_smooth_en);
				pre_pv_smooth_en = cur_pv_smooth_en;
			}
			if(_g_nrd_hw_h15_trace)
			{
				if(count>PE_PRINT_COUNT_NUM)
				{
					PE_NRD_HW_H15_DBG_PRINT("[state] tnr_m_en:%d, u,m,d:%d,%d,%d, pv_smooth_en:%d\n",\
						cur_m_en, pInfo->tnr_pv_smooth_en_user, pInfo->tnr_pv_smooth_en_mask, \
						p_tnr_dflt->tnr_ctrl_11.pv_smooth_en, cur_pv_smooth_en);
					count=0;
				}
				count++;
			}
		}
		else
		{
			PE_NRD_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	#endif
	return ret;
}
/**
 * set tnr sc en mode
 *
 * @param   *pCfg [in] PE_TSK_H15_CFG_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_NRD_HW_H15_SetTnrScMode(PE_TSK_H15_CFG_T *pCfg)
{
	int ret = RET_OK;
	#ifdef PE_HW_H15_PROTECT_OVERLAP_SETTINGS
	static UINT32 count = 0;
	static UINT32 pre_sc_en = 0xff;
	UINT32 cur_sc_en;
	PE_TNR_HW_PARAM_REG_H15A0_T *p_tnr_dflt = _g_pe_tnr_hw_param_data_h15a.reg_dflt.data;
	PE_NRD_HW_H15_SETTINGS_T *pInfo = &_g_pe_nrd_hw_h15_info;
	do{
		CHECK_KNULL(pCfg);
		if(PE_KDRV_VER_H15)
		{
			/* if ud & use_manual_sc on, sc_en off, otherwise, refer default or user */
			cur_sc_en = (pInfo->use_manual_sc==1&&pInfo->tnr0a_fmt==PE_NRD_H15A_TNR_UD)? 0x0:\
				(pInfo->tnr_sc_en_mask)? pInfo->tnr_sc_en_user:p_tnr_dflt->tnr_ctrl_02.sc_en;
			/* compare pre vs. cur */
			if (pre_sc_en != cur_sc_en)
			{
				PE_NRD_HW_H15_DBG_PRINT("[inf] man_sc,tnr0a:%d,%d\n", \
					pInfo->use_manual_sc, pInfo->tnr0a_fmt);
				PE_NRD_HW_H15_DBG_PRINT("[set] u,m,d:%d,%d,%d, sc_en:%d -> %d\n", \
					pInfo->tnr_sc_en_user, pInfo->tnr_sc_en_mask, \
					p_tnr_dflt->tnr_ctrl_02.sc_en, pre_sc_en, cur_sc_en);
				PE_NRD_HW_H15A_TNR12_WR01(tnr_ctrl_02,sc_en,cur_sc_en);
				pre_sc_en = cur_sc_en;
			}
			if(_g_nrd_hw_h15_trace)
			{
				if(count>PE_PRINT_COUNT_NUM)
				{
					PE_NRD_HW_H15_DBG_PRINT("[state] man_sc,tnr0a:%d,%d, u,m,d:%d,%d,%d, sc_en:%d\n",\
						pInfo->use_manual_sc, pInfo->tnr0a_fmt, pInfo->tnr_sc_en_user, \
						pInfo->tnr_sc_en_mask, p_tnr_dflt->tnr_ctrl_02.sc_en, cur_sc_en);
					count=0;
				}
				count++;
			}
		}
		else
		{
			PE_NRD_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	#endif
	return ret;
}
/**
 * set tnr mode
 *
 * @param   *pCfg [in] PE_TSK_H15_CFG_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_NRD_HW_H15_SetTnrMode(PE_TSK_H15_CFG_T *pCfg)
{
    int ret = RET_OK;
	do{
		CHECK_KNULL(pCfg);
		if(PE_KDRV_VER_H15)
		{
			ret = PE_NRD_HW_H15_SetTnrLbMode(pCfg);
			PE_NRD_HW_H15_CHECK_CODE(ret, break, \
				"[%s,%d] PE_NRD_HW_H15_SetTnrLbMode() error.\n",__F__,__L__);
			ret = PE_NRD_HW_H15_SetTnrSmoothMode(pCfg);
			PE_NRD_HW_H15_CHECK_CODE(ret, break, \
				"[%s,%d] PE_NRD_HW_H15_SetTnrSmoothMode() error.\n",__F__,__L__);
			ret = PE_NRD_HW_H15_SetTnrScMode(pCfg);
			PE_NRD_HW_H15_CHECK_CODE(ret, break, \
				"[%s,%d] PE_NRD_HW_H15_SetTnrScMode() error.\n",__F__,__L__);
		}
		else
		{
			PE_NRD_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * set dnr max mode
 *
 * @param   *pCfg [in] PE_TSK_H15_CFG_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_NRD_HW_H15_SetDnrMaxMode(PE_TSK_H15_CFG_T *pCfg)
{
	int ret = RET_OK;
	#ifdef PE_HW_H15_PROTECT_OVERLAP_SETTINGS
	static UINT32 count = 0;
	static UINT32 pre_dnr_max_en0 = 0xff;
	UINT32 cur_dnr_max_en0 = 0;
	PE_INF_H15_CTRL_T *ctrl0_info = &pCfg->inf.ctrl_info[LX_PE_WIN_0];
	PE_DNR_HW_PARAM_REG_H15A0_T *p_dnr_dflt = _g_pe_dnr_hw_param_data_h15a.reg_dflt.data;
	PE_NRD_HW_H15_SETTINGS_T *pInfo = &_g_pe_nrd_hw_h15_info;
	do{
		CHECK_KNULL(pCfg);
		if(PE_KDRV_VER_H15)
		{
			/* if mask on, refer default, otherwise, user data */
			cur_dnr_max_en0 = (ctrl0_info->i_c_format==PE_NRD_HW_H15_C_FMT_444)? 0x0:\
				(pInfo->dnr_max_en0_mask)? pInfo->dnr_max_en0_user:\
				p_dnr_dflt->dnr_max_ctrl.reg_dnr_max_enable;
			/* compare pre vs. cur */
			if (pre_dnr_max_en0 != cur_dnr_max_en0)
			{
				PE_NRD_HW_H15_DBG_PRINT("[inf] i_c0:%d\n", ctrl0_info->i_c_format);
				PE_NRD_HW_H15_DBG_PRINT("[set] u,m,d:%d,%d,%d, bnr_ac_h_en0 : %d -> %d\n",\
					pInfo->dnr_max_en0_user, pInfo->dnr_max_en0_mask, \
					p_dnr_dflt->dnr_max_ctrl.reg_dnr_max_enable, pre_dnr_max_en0, cur_dnr_max_en0);
				PE_NRD_HW_H15A_DNR_WR01(dnr_max_ctrl,reg_dnr_max_enable,cur_dnr_max_en0);
				pre_dnr_max_en0 = cur_dnr_max_en0;
			}
			if(_g_nrd_hw_h15_trace)
			{
				if(count>PE_PRINT_COUNT_NUM)
				{
					PE_NRD_HW_H15_DBG_PRINT("[state] i_c0:%d\n", ctrl0_info->i_c_format);
					PE_NRD_HW_H15_DBG_PRINT("[state] u,m,d:%d,%d,%d, bnr_ac_h_en0 : %d\n",\
						pInfo->dnr_max_en0_user, pInfo->dnr_max_en0_mask, \
						p_dnr_dflt->dnr_max_ctrl.reg_dnr_max_enable, cur_dnr_max_en0);
					count=0;
				}
				count++;
			}
		}
		else
		{
			PE_NRD_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	#endif
	return ret;
}
/**
 * set bnr mode
 *
 * @param   *pCfg [in] PE_TSK_H15_CFG_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_NRD_HW_H15_SetBnrMode(PE_TSK_H15_CFG_T *pCfg)
{
	int ret = RET_OK;
	#ifdef PE_HW_H15_PROTECT_OVERLAP_SETTINGS
	static UINT32 count = 0;
	static UINT32 pre_bnr_ac_h_en0 = 0xff;		//pre bnr_ac_h_en0
	UINT32 cur_bnr_ac_h_en0 = 0;
	PE_INF_H15_CTRL_T *ctrl0_info = &pCfg->inf.ctrl_info[LX_PE_WIN_0];
	static UINT32 pre_sc_bnr_en0 = 0xff;		//pre sc_bnr_en0
	UINT32 cur_sc_bnr_en0 = 0;
	LX_PE_INF_DISPLAY_T *disp0_info = &pCfg->inf.disp_info[LX_PE_WIN_0];
	PE_DNR_HW_PARAM_REG_H15A0_T *p_dnr_dflt = _g_pe_dnr_hw_param_data_h15a.reg_dflt.data;
	PE_NRD_HW_H15_SETTINGS_T *pInfo = &_g_pe_nrd_hw_h15_info;
	do{
		CHECK_KNULL(pCfg);
		if(PE_KDRV_VER_H15)
		{
			/* if mask on, refer default, otherwise, user data */
			/* skip drv dnr ctrl, if fw dnr on. */
			if (!PE_CMN_HW_H15_GetDbInfo(PE_CMN_HW_H15_DB_INF_MD0_DNR_FW_EN, 0))
			{
				cur_bnr_ac_h_en0 = (disp0_info->src_type == LX_PE_SRC_DTV && \
					disp0_info->dtv_type == LX_PE_DTV_FILEPLAY && \
					ctrl0_info->pe0_src_h <= PE_NRD_HW_H15_H_MIN && \
					ctrl0_info->pe0_src_v <= PE_NRD_HW_H15_V_MIN)? 0x0:\
					(pInfo->bnr_ac_h_en0_mask)? pInfo->bnr_ac_h_en0_user:\
					p_dnr_dflt->ac_bnr_ctrl_0.reg_bnr_ac_h_en;
				/* compare pre vs. cur */
				if (pre_bnr_ac_h_en0 != cur_bnr_ac_h_en0)
				{
					PE_NRD_HW_H15_DBG_PRINT("[inf] src0:%d,dtv0:%d, h0,v0:%d,%d\n",\
						disp0_info->src_type, disp0_info->dtv_type, \
						ctrl0_info->pe0_src_h, ctrl0_info->pe0_src_v);
					PE_NRD_HW_H15_DBG_PRINT("[set] u,m,d:%d,%d,%d, bnr_ac_h_en0 : %d -> %d\n",\
						pInfo->bnr_ac_h_en0_user, pInfo->bnr_ac_h_en0_mask, \
						p_dnr_dflt->ac_bnr_ctrl_0.reg_bnr_ac_h_en, \
						pre_bnr_ac_h_en0, cur_bnr_ac_h_en0);
					PE_NRD_HW_H15A_DNR_WR01(ac_bnr_ctrl_0,reg_bnr_ac_h_en,cur_bnr_ac_h_en0);
					pre_bnr_ac_h_en0 = cur_bnr_ac_h_en0;
				}
				if(_g_nrd_hw_h15_trace && count>PE_PRINT_COUNT_NUM)
				{
					PE_NRD_HW_H15_DBG_PRINT("[state] src0:%d,dtv0:%d, h0,v0:%d,%d\n",\
						disp0_info->src_type, disp0_info->dtv_type, \
						ctrl0_info->pe0_src_h, ctrl0_info->pe0_src_v);
					PE_NRD_HW_H15_DBG_PRINT("[state] u,m,d:%d,%d,%d, bnr_ac_h_en0:%d\n",\
						pInfo->bnr_ac_h_en0_user, pInfo->bnr_ac_h_en0_mask, \
						p_dnr_dflt->ac_bnr_ctrl_0.reg_bnr_ac_h_en, cur_bnr_ac_h_en0);
				}
			}
			/* if multi path, off sc bnr */
			cur_sc_bnr_en0 = (PE_HW_H15_CHK_MULTI_PATH_FMT(disp0_info->fmt_type) || \
				PE_HW_H15_CHK_MULTI_PATH_OUT(disp0_info->out_type))? 0x0:\
				p_dnr_dflt->sc_bnr_ctrl_0.reg_sc_bnr_en;
			/* compare pre vs. cur */
			if (pre_sc_bnr_en0 != cur_sc_bnr_en0)
			{
				PE_NRD_HW_H15_DBG_PRINT("[inf] out:%d, dflt:%d\n",\
					disp0_info->out_type, \
					p_dnr_dflt->sc_bnr_ctrl_0.reg_sc_bnr_en);
				PE_NRD_HW_H15_DBG_PRINT("[set] sc_bnr_en : %d -> %d\n",\
					pre_sc_bnr_en0, cur_sc_bnr_en0);
				PE_DNR1_H15A0_QWr01(sc_bnr_ctrl_0,reg_sc_bnr_en,cur_sc_bnr_en0);
				PE_NRD_HW_H15A_DNR_SET_USER(sc_bnr_ctrl_0,reg_sc_bnr_en,cur_sc_bnr_en0);
				pre_sc_bnr_en0 = cur_sc_bnr_en0;
			}
			if(_g_nrd_hw_h15_trace)
			{
				if(count>PE_PRINT_COUNT_NUM)
				{
					PE_NRD_HW_H15_DBG_PRINT("[state] out:%d, dflt:%d, sc_bnr_en:%d\n",\
						disp0_info->out_type, p_dnr_dflt->sc_bnr_ctrl_0.reg_sc_bnr_en, \
						cur_sc_bnr_en0);
					count=0;
				}
				count++;
			}
		}
		else
		{
			PE_NRD_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	#endif
	return ret;
}
/**
 * set dnr mode
 *
 * @param   *pCfg [in] PE_TSK_H15_CFG_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_NRD_HW_H15_SetDnrMode(PE_TSK_H15_CFG_T *pCfg)
{
    int ret = RET_OK;
	do{
		CHECK_KNULL(pCfg);
		if(PE_KDRV_VER_H15)
		{
			ret = PE_NRD_HW_H15_SetDnrMaxMode(pCfg);
			PE_NRD_HW_H15_CHECK_CODE(ret, break, \
				"[%s,%d] PE_NRD_HW_H15_SetDnrMaxMode() error.\n",__F__,__L__);
			ret = PE_NRD_HW_H15_SetBnrMode(pCfg);
			PE_NRD_HW_H15_CHECK_CODE(ret, break, \
				"[%s,%d] PE_NRD_HW_H15_SetBnrMode() error.\n",__F__,__L__);
		}
		else
		{
			PE_NRD_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * set ifc mode
 *
 * @param   *pCfg [in] PE_TSK_H15_CFG_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_NRD_HW_H15_SetIfcMode(PE_TSK_H15_CFG_T *pCfg)
{
	int ret = RET_OK;
	#ifdef PE_HW_H15_PROTECT_OVERLAP_SETTINGS
	static UINT32 count = 0;
	static UINT32 pre_ifc_en0 = 0xff;		//pre ifc_en0
	UINT32 cur_ifc_en0 = 0;
	LX_PE_INF_DISPLAY_T *disp0_info = &pCfg->inf.disp_info[LX_PE_WIN_0];
	PE_IFC_HW_PARAM_REG_H15A0_T *p_ifc_dflt = _g_pe_ifc_hw_param_data_h15a.reg_dflt.data;
	do{
		CHECK_KNULL(pCfg);
		if(PE_KDRV_VER_H15)
		{
			/* if mask on, refer default, otherwise, user data */
			cur_ifc_en0 = (PE_HW_H15_CHK_MULTI_PATH_FMT(disp0_info->fmt_type) || \
				PE_HW_H15_CHK_MULTI_PATH_OUT(disp0_info->out_type))? 0x0:\
				p_ifc_dflt->ifc_ctrl_8.reg_ifc_en;
			/* compare pre vs. cur */
			if (pre_ifc_en0 != cur_ifc_en0)
			{
				PE_NRD_HW_H15_DBG_PRINT("[inf] out:%d, dflt:%d\n",\
					disp0_info->out_type, p_ifc_dflt->ifc_ctrl_8.reg_ifc_en);
				PE_NRD_HW_H15_DBG_PRINT("[set] ifc_en : %d -> %d\n",\
					pre_ifc_en0, cur_ifc_en0);
				PE_DNR1_H15A0_QWr01(ifc_ctrl_8,reg_ifc_en,cur_ifc_en0);
				PE_NRD_HW_H15A_IFC_SET_USER(ifc_ctrl_8,reg_ifc_en,cur_ifc_en0);
				pre_ifc_en0 = cur_ifc_en0;
			}
			if(_g_nrd_hw_h15_trace)
			{
				if(count>PE_PRINT_COUNT_NUM)
				{
					PE_NRD_HW_H15_DBG_PRINT("[state] out:%d, dflt:%d, ifc_en:%d\n",\
						disp0_info->out_type, p_ifc_dflt->ifc_ctrl_8.reg_ifc_en, \
						cur_ifc_en0);
					count=0;
				}
				count++;
			}
		}
		else
		{
			PE_NRD_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	#endif
	return ret;
}

/**
 * download tnr db
 *
 * @param   void
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_NRD_HW_H15_DownloadTnrDb(void)
{
	int ret = RET_OK;
	UINT32 i, size, sec_typ;
	UINT32 base_lut[PE_NRD_H15A_TNL_NUM];
	UINT32 base_lm, base_ui, base_hm, base_init;
	PE_DDR_DB_SHDW_REG_H15_T *p_shdw_db = gPE_DDR_DB_H15.shdw.data;
	volatile PE_DDR_DB_PHYS_REG_H15_T *p_phys_db = gPE_DDR_DB_H15.phys.data;
	volatile UINT32 *p_tnr_lut_data  = p_phys_db->tnr_lut.data;
	volatile UINT32 *p_tnr_user_data = p_phys_db->tnr_user.data;
	do{
		CHECK_KNULL(p_shdw_db);
		CHECK_KNULL(p_phys_db);
		CHECK_KNULL(p_tnr_lut_data );
		CHECK_KNULL(p_tnr_user_data);
		if (PE_KDRV_VER_H15)
		{
			/* tnr_lut **************************************************************/
			sec_typ = PE_DDR_H15_SEC_TNR_LUT;
			/* header */
			p_shdw_db->sec_d[sec_typ].header.version   = 0x1;
			p_shdw_db->sec_d[sec_typ].header.sec_type  = sec_typ;
			p_shdw_db->sec_d[sec_typ].header.item_num  = PE_DDR_H15_TNR_LUT_ITEM_NUM;
			p_shdw_db->sec_d[sec_typ].header.item_size = PE_DDR_H15_TNR_LUT_SIZE;
			p_phys_db->tnr_lut.header.version   = p_shdw_db->sec_d[sec_typ].header.version;
			p_phys_db->tnr_lut.header.sec_type  = p_shdw_db->sec_d[sec_typ].header.sec_type;
			p_phys_db->tnr_lut.header.item_num  = p_shdw_db->sec_d[sec_typ].header.item_num;
			p_phys_db->tnr_lut.header.item_size = p_shdw_db->sec_d[sec_typ].header.item_size;
			/* data */
			size = (PE_DDR_H15_TNR_LUT_ITEM_NUM * PE_DDR_H15_TNR_LUT_SIZE * sizeof(UINT32));
			p_shdw_db->top_h.sec_base[sec_typ] = \
				p_shdw_db->top_h.sec_base[sec_typ-1]+p_shdw_db->top_h.sec_size[sec_typ-1];
			p_shdw_db->top_h.sec_size[sec_typ] = size + sizeof(PE_DDR_H15_HEADER_T);
			p_phys_db->top_h.sec_base[sec_typ] = p_shdw_db->top_h.sec_base[sec_typ];
			p_phys_db->top_h.sec_size[sec_typ] = p_shdw_db->top_h.sec_size[sec_typ];
			p_shdw_db->sec_d[sec_typ].data_base = \
				p_shdw_db->top_h.sec_base[sec_typ] + sizeof(PE_DDR_H15_HEADER_T);
			/* wr ddr */
			base_lut[0] = 0;									//PE_NRD_H15A_TNL_HD
			base_lut[1] = base_lut[0]+PE_DDR_H15_TNR_LUT_SIZE;	//PE_NRD_H15A_TNL_SD
			base_lut[2] = base_lut[1]+PE_DDR_H15_TNR_LUT_SIZE;	//PE_NRD_H15A_TNL_UD
			base_lut[3] = base_lut[2]+PE_DDR_H15_TNR_LUT_SIZE;	//PE_NRD_H15A_TNL_CAM_HD
			for (i=0; i<PE_DDR_H15_TNR_LUT_SIZE; i++)
			{
				p_tnr_lut_data[base_lut[0]+i] = tnr_l_lut_hd_default_h15a0[i].data;
				p_tnr_lut_data[base_lut[1]+i] = tnr_l_lut_sd_default_h15a0[i].data;
				p_tnr_lut_data[base_lut[2]+i] = tnr_l_lut_ud_default_h15a0[i].data;
				p_tnr_lut_data[base_lut[3]+i] = tnr_l_lut_cam_hd_default_h15a0[i].data;
			}
			/* noti */
			PE_PRINT_NOTI("%s: DL done.(base:0x%08x)\n", \
				PE_DDR_REG_H15_SEC_TYPE_TO_STR(sec_typ), p_shdw_db->top_h.sec_base[sec_typ]);
			/* tnr_user **************************************************************/
			sec_typ = PE_DDR_H15_SEC_TNR_USER;
			/* header */
			p_shdw_db->sec_d[sec_typ].header.version   = 0x1;
			p_shdw_db->sec_d[sec_typ].header.sec_type  = sec_typ;
			p_shdw_db->sec_d[sec_typ].header.item_num  = PE_DDR_H15_TNR_USER_ITEM_NUM;
			p_shdw_db->sec_d[sec_typ].header.item_size = PE_DDR_H15_TNR_USER_SIZE;
			p_phys_db->tnr_user.header.version   = p_shdw_db->sec_d[sec_typ].header.version;
			p_phys_db->tnr_user.header.sec_type  = p_shdw_db->sec_d[sec_typ].header.sec_type;
			p_phys_db->tnr_user.header.item_num  = p_shdw_db->sec_d[sec_typ].header.item_num;
			p_phys_db->tnr_user.header.item_size = p_shdw_db->sec_d[sec_typ].header.item_size;
			/* data */
			size = (PE_DDR_H15_TNR_USER_ITEM_NUM * PE_DDR_H15_TNR_USER_SIZE * sizeof(UINT32));
			p_shdw_db->top_h.sec_base[sec_typ] = \
				p_shdw_db->top_h.sec_base[sec_typ-1]+p_shdw_db->top_h.sec_size[sec_typ-1];
			p_shdw_db->top_h.sec_size[sec_typ] = size + sizeof(PE_DDR_H15_HEADER_T);
			p_phys_db->top_h.sec_base[sec_typ] = p_shdw_db->top_h.sec_base[sec_typ];
			p_phys_db->top_h.sec_size[sec_typ] = p_shdw_db->top_h.sec_size[sec_typ];
			p_shdw_db->sec_d[sec_typ].data_base = \
				p_shdw_db->top_h.sec_base[sec_typ] + sizeof(PE_DDR_H15_HEADER_T);
			/* wr ddr */
			p_tnr_user_data[0]  = 0x0;
			p_tnr_user_data[1]  = 0x0;
			base_lm = 2;
			base_ui = base_lm+LX_PE_NRD_TNR5_CMN_LUT_SIZE;
			base_hm = base_ui+LX_PE_NRD_TNR5_CMN_LUT_SIZE;
			base_init = 7;
			for (i=0; i<LX_PE_NRD_TNR5_CMN_LUT_SIZE; i++)
			{
				p_tnr_user_data[base_lm+i] = tnr_l_lut_init_h15a0[base_init+i].data;
				p_tnr_user_data[base_ui+i] = tnr_l_lut_init_h15a0[base_init+i].data;
				p_tnr_user_data[base_hm+i] = tnr_l_lut_init_h15a0[base_init+i].data;
			}
			/* noti */
			PE_PRINT_NOTI("%s: DL done.(base:0x%08x)\n", \
				PE_DDR_REG_H15_SEC_TYPE_TO_STR(sec_typ), p_shdw_db->top_h.sec_base[sec_typ]);
		}
		else
		{
			PE_NRD_HW_H15_DBG_PRINT("nothing to do.\n");
			ret = RET_OK;
		}
	}while (0);
	return ret;
}

/**
 * download tnr user db
 *
 * @param   *pstParams [in] void
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_NRD_HW_H15_DownloadTnrUserDb(void *pstParams)
{
	int ret = RET_OK;
	UINT32 i, size, sec_typ;
	#ifdef PE_USE_H15_NEW_DTNR_STRUCT
	UINT32 base;
	LX_PE_NRD_TNR5_CMN_T *pp = NULL;
	#else
	UINT32 *pp = NULL;
	#endif
	static UINT32 count = 0;
	PE_DDR_DB_SHDW_REG_H15_T *p_shdw_db = gPE_DDR_DB_H15.shdw.data;
	volatile PE_DDR_H15_TNR_USER_T *p_phys_tnr_user_db = NULL;
	volatile UINT32 *p_db_data = NULL;
	do{
		CHECK_KNULL(pstParams);
		if (PE_KDRV_VER_H15)
		{
			sec_typ = PE_DDR_H15_SEC_TNR_USER;
			base = p_shdw_db->top_h.sec_base[sec_typ];
			size = sizeof(PE_DDR_H15_TNR_USER_T);
			p_phys_tnr_user_db  = (volatile PE_DDR_H15_TNR_USER_T *)ioremap(base, size);
			CHECK_KNULL(p_phys_tnr_user_db);
			p_db_data = p_phys_tnr_user_db->data;
			#ifdef PE_USE_H15_NEW_DTNR_STRUCT
			pp = (LX_PE_NRD_TNR5_CMN_T *)pstParams;
			p_db_data[1] = (UINT32)pp->tnr_en;
			base = 2;
			for (i=0; i<LX_PE_NRD_TNR5_CMN_LUT_SIZE; i++)
			{
				p_db_data[base+i] = pp->lut_lm[i];
			}
			base += LX_PE_NRD_TNR5_CMN_LUT_SIZE;
			for (i=0; i<LX_PE_NRD_TNR5_CMN_LUT_SIZE; i++)
			{
				p_db_data[base+i] = pp->lut_ui[i];
			}
			base += LX_PE_NRD_TNR5_CMN_LUT_SIZE;
			for (i=0; i<LX_PE_NRD_TNR5_CMN_LUT_SIZE; i++)
			{
				p_db_data[base+i] = pp->lut_hm[i];
			}
			#else
			pp = (UINT32 *)pstParams;
			for (i=1; i<PE_DDR_H15_TNR_USER_SIZE; i++)
			{
				p_db_data[i] = pp[i];
			}
			#endif
			count = (count < 0xff)? (count+1):0x0;
			p_db_data[0] = count;
			PE_NRD_HW_H15_DBG_PRINT("%s: DL done.(base:0x%08x)(cnt:%4d)\n", \
				PE_DDR_REG_H15_SEC_TYPE_TO_STR(sec_typ), \
				p_shdw_db->top_h.sec_base[sec_typ], count);
		}
		else
		{
			PE_NRD_HW_H15_DBG_PRINT("nothing to do.\n");
			ret = RET_OK;
		}
	}while (0);
	if (p_phys_tnr_user_db) iounmap((void *)p_phys_tnr_user_db);
	return ret;
}

