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

/** @file pe_shp_hw_h15.c
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
/* sr cg(clock gating) test tracing */
#undef PE_SHP_HW_H15_SR_PWD_TRACE

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
#include "pe_shp_hw_def.h"
#include "pe_cmn_hw_h15.h"
#include "pe_shp_hw_h15.h"
#include "pe_msc_hw_param_h15.h"
#include "pe_res_hw_param_h15.h"
#include "pe_cti_dnr_hw_param_h15.h"
#include "pe_pblur_dnr_hw_param_h15.h"

#ifdef PE_SHP_HW_H15_SR_PWD_TRACE
#include "../../sys/sys_regs.h"
#endif

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
#define PE_SHP_HW_H15_FHD_H_MAX					1920
#define PE_SHP_HW_H15_FHD_V_MAX					1080
#define PE_SHP_HW_H15_SCL_IN_H_MIN				160
#define PE_SHP_HW_H15_SCL_IN_V_MIN				120
#define PE_SHP_HW_H15_IDX_Y_DFLT				15
#define PE_SHP_HW_H15_IDX_C_DFLT				39

#define PE_SHP_HW_H15_SCL_SIZE_MIN				(10)
#define PE_SHP_HW_H15_SCL_SIZE_1K				(1024)
#define PE_SHP_HW_H15_SCL_SIZE_2K				(2048)
#define PE_SHP_HW_H15_SCL_VSC_MD_V12TAP			(0x0)
#define PE_SHP_HW_H15_SCL_VSC_MD_V06TAP			(0x1)
#define PE_SHP_HW_H15_SCL_VSC_MD_V02TAP			(0x3)
#define PE_SHP_HW_H15_SCL_ADAPTIVE_ON			(0x1)
#define PE_SHP_HW_H15_SCL_ADAPTIVE_OFF			(0x0)
#define PE_SHP_HW_H15_SCL_BILINEAR				(0x1)
#define PE_SHP_HW_H15_SCL_POLYPHASE				(0x0)
#define PE_SHP_HW_H15_SCL_C_FMT_444				(0x6)
#define PE_SHP_HW_H15_SCL_C_FMT_422				(0x5)
#define PE_SHP_HW_H15_SCL_C_FMT_420				(0x4)
#define PE_SHP_HW_H15_SCL_PXL_RP_OFF			(0x0)
#define PE_SHP_HW_H15_SCL_PXL_RP_RED			(0x1)
#define PE_SHP_HW_H15_SCL_PXL_RP_GRN			(0x2)
#define PE_SHP_HW_H15_SCL_PXL_RP_BLU			(0x3)
#define PE_SHP_HW_H15_SCL_CO_REG_INC_MAX		(128)
#define PE_SHP_HW_H15_SCL_M1Y					0
#define PE_SHP_HW_H15_SCL_M2Y					1
#define PE_SHP_HW_H15_SCL_M1C					2
#define PE_SHP_HW_H15_SCL_M2C					3
#define PE_SHP_HW_H15_SCL_MPY					4

/* h,v only(define) vs. both(undef), Now we use both mode until getting c08t scl has no problem. */
#define PE_SHP_HW_H15_USE_V12TM_C08T_SEPARATE_HV

/* use MSR vs. SSC on venc mode, We are going to use SSC from H15A0. */
#undef PE_SHP_HW_H15_USE_MSR_FOR_VENC_ON

/* use define until shp f/w ready, H15A0 */
#define PE_SHP_HW_H15A_SET_FW_PARAM_TO_REG_DIRECTLY

/* 20140616, sj.youm, pe1_lap_h 1,2,4 not available because of rtl bug. set valid value(0,3,5) */
#undef PE_SHP_HW_H15_PROTECT_SP_LAP_H_MODE_BUG

/* 20141006, sj.youm, sre shp write only mode */
#ifdef PE_HW_H15_PROTECT_OVERLAP_SETTINGS
#define PE_SHP_HW_H15_SRE_SHP_WR_ONLY_MD
#else
#undef PE_SHP_HW_H15_SRE_SHP_WR_ONLY_MD
#endif

/* 20141009, sj.youm, use sre shp write only mode */
#define PE_SHP_HW_H15_USE_SRE_SHP_WR_ONLY_MD

/* use internal scl filter coeff decision */
#define PE_SHP_HW_H15_SCL_CO_INTERNAL_DECISION

/* use shp L fw dummy reg for fw msc ctrl write */
#define PE_SHP_HW_H15_USE_FWS_REG_FOR_FW_MSC_WR

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define PE_SHP_HW_H15_ERROR		PE_PRINT_ERROR

#ifdef PE_SHP_HW_H15_SR_PWD_TRACE
#define PE_SHP_HW_H15_DBG_SRPD_TRACE(fmt, args...)	\
	if(_g_shp_hw_h15_trace)\
	{\
		printk("[%x,%x][%s,%d] "fmt, \
			PE_CHIP_VER, g_pe_kdrv_ver_mask, __F__, __L__, ##args);\
	}

#define PE_SHP_HW_H15_DBG_PRINT(fmt, args...)
#else
#define PE_SHP_HW_H15_DBG_PRINT(fmt, args...)	\
	if(_g_shp_hw_h15_trace)\
	{\
		printk("[%x,%x][%s,%d] "fmt, \
			PE_CHIP_VER, g_pe_kdrv_ver_mask, __F__, __L__, ##args);\
	}
#endif

#define PE_SHP_HW_H15_CHECK_CODE(_checker, _action, fmt, args...)	\
	{if(_checker){PE_SHP_HW_H15_ERROR(fmt, ##args);_action;}}

#define PE_SHP_HW_H15_DBG_CHECK_CODE(_cnt, _checker, _action, fmt, args...)	\
	{\
		if(_checker){\
			if(_g_shp_hw_h15_trace&&_cnt>PE_PRINT_COUNT_NUM)\
			{\
				PE_SHP_HW_H15_DBG_PRINT(fmt, ##args);\
			}\
			_action;\
		}\
	}

#define PE_SHP_HW_H15A_DBG_PRINT_RES_FMT_STATE(_pre, _cur)	\
	{\
		char _str_buf[PE_TRACE_STR_SIZE];\
		sprintf(_str_buf, "%s(%s)->(%s):%s", #_cur, \
			PE_SHP_HW_H15A_RES_FMT_TO_STR(_pre), \
			PE_SHP_HW_H15A_RES_FMT_TO_STR(_cur), \
			((_pre)!=(_cur))? "different":"same");\
		PE_SHP_HW_H15_DBG_PRINT("%s\n", _str_buf);\
		if((_pre)!=(_cur))\
		{\
			PE_INF_H15_SetTraceDataBuf(_str_buf);\
		}\
	}
#ifdef PE_SHP_HW_H15_SRE_SHP_WR_ONLY_MD
#define PE_SHP_HW_H15A_DBG_PRINT_SRS_FMT_STATE(_pre, _cur)	\
	{\
		char _str_buf[PE_TRACE_STR_SIZE];\
		sprintf(_str_buf, "%s(%s)->(%s):%s", #_cur, \
			PE_SHP_HW_H15A_SRS_FMT_TO_STR(_pre), \
			PE_SHP_HW_H15A_SRS_FMT_TO_STR(_cur), \
			((_pre)!=(_cur))? "different":"same");\
		PE_SHP_HW_H15_DBG_PRINT("%s\n", _str_buf);\
		if((_pre)!=(_cur))\
		{\
			PE_INF_H15_SetTraceDataBuf(_str_buf);\
		}\
	}
#endif
#define PE_SHP_HW_H15A_DBG_PRINT_SRE_FMT_STATE(_pre, _cur)	\
	{\
		char _str_buf[PE_TRACE_STR_SIZE];\
		sprintf(_str_buf, "%s(%s)->(%s):%s", #_cur, \
			PE_SHP_HW_H15A_SRE_FMT_TO_STR(_pre), \
			PE_SHP_HW_H15A_SRE_FMT_TO_STR(_cur), \
			((_pre)!=(_cur))? "different":"same");\
		PE_SHP_HW_H15_DBG_PRINT("%s\n", _str_buf);\
		if((_pre)!=(_cur))\
		{\
			PE_INF_H15_SetTraceDataBuf(_str_buf);\
		}\
	}
#define PE_SHP_HW_H15A_DBG_PRINT_SRL_FMT_STATE(_pre, _cur)	\
	{\
		char _str_buf[PE_TRACE_STR_SIZE];\
		sprintf(_str_buf, "%s(%s)->(%s):%s", #_cur, \
			PE_SHP_HW_H15A_SRL_FMT_TO_STR(_pre), \
			PE_SHP_HW_H15A_SRL_FMT_TO_STR(_cur), \
			((_pre)!=(_cur))? "different":"same");\
		PE_SHP_HW_H15_DBG_PRINT("%s\n", _str_buf);\
		if((_pre)!=(_cur))\
		{\
			PE_INF_H15_SetTraceDataBuf(_str_buf);\
		}\
	}
#define PE_SHP_HW_H15A_DBG_PRINT_PBL_FMT_STATE(_pre, _cur)	\
	{\
		char _str_buf[PE_TRACE_STR_SIZE];\
		sprintf(_str_buf, "%s(%s)->(%s):%s", #_cur, \
			PE_SHP_HW_H15A_PBL_FMT_TO_STR(_pre), \
			PE_SHP_HW_H15A_PBL_FMT_TO_STR(_cur), \
			((_pre)!=(_cur))? "different":"same");\
		PE_SHP_HW_H15_DBG_PRINT("%s\n", _str_buf);\
		if((_pre)!=(_cur))\
		{\
			PE_INF_H15_SetTraceDataBuf(_str_buf);\
		}\
	}
#define PE_SHP_HW_H15A_DBG_PRINT_CTI_FMT_STATE(_pre, _cur)	\
	{\
		char _str_buf[PE_TRACE_STR_SIZE];\
		sprintf(_str_buf, "%s(%s)->(%s):%s", #_cur, \
			PE_SHP_HW_H15A_CTI_FMT_TO_STR(_pre), \
			PE_SHP_HW_H15A_CTI_FMT_TO_STR(_cur), \
			((_pre)!=(_cur))? "different":"same");\
		PE_SHP_HW_H15_DBG_PRINT("%s\n", _str_buf);\
		if((_pre)!=(_cur))\
		{\
			PE_INF_H15_SetTraceDataBuf(_str_buf);\
		}\
	}

#define PE_SHP_HW_H15_WR_SCL_COEFF(_wid, _ctrl, _coeff, _id)	\
{\
	char _str_buf[PE_TRACE_STR_SIZE];\
	sprintf(_str_buf, "[wid:%d][idx:%3d]%s", _wid, (_id), #_coeff);\
	PE_SHP_HW_H15_DBG_PRINT("set %s\n", _str_buf);\
	PE_INF_H15_SetTraceDataBuf(_str_buf);\
	PE_SHP_HW_H15_WrSclCoeff((_ctrl),&(_coeff));\
}

#define PE_SHP_HW_H15A_SET_SHDW_MSC_Y_TO_SHP(_dst, _src)	\
	(*((UINT32 *)&(gPE_SHPL_H15A0.shdw.data->_dst))) = (*((UINT32 *)&(gPE_MSC_Y_H15A0.shdw.data->_src)))
#define PE_SHP_HW_H15A_SET_SHDW_MSC_C_TO_SHP(_dst, _src)	\
	(*((UINT32 *)&(gPE_SHPL_H15A0.shdw.data->_dst))) = (*((UINT32 *)&(gPE_MSC_C_H15A0.shdw.data->_src)))

#ifdef PE_HW_H15_PROTECT_OVERLAP_SETTINGS
#define PE_SHP_HW_H15A_RES_SET_USER(_r, _f, _d)	\
	{	_g_pe_res_hw_param_data_h15a.reg_user.data->_r._f = (_d);\
		_g_pe_res_hw_param_data_h15a.reg_mask.data->_r._f = 0;}

#define PE_SHP_HW_H15A_RES_WR01(_r, _f1, _d1)	\
	{	PE_SHPLR_H15A0_QWr01(_r, _f1, _d1);\
		PE_SHP_HW_H15A_RES_SET_USER(_r, _f1, _d1);}
#define PE_SHP_HW_H15A_RES_WR02(_r, _f1, _d1, _f2, _d2)	\
	{	PE_SHPLR_H15A0_QWr02(_r, _f1, _d1, _f2, _d2);\
		PE_SHP_HW_H15A_RES_SET_USER(_r, _f1, _d1);\
		PE_SHP_HW_H15A_RES_SET_USER(_r, _f2, _d2);}
#define PE_SHP_HW_H15A_RES_WR03(_r, _f1, _d1, _f2, _d2, _f3, _d3)	\
	{	PE_SHPLR_H15A0_QWr03(_r, _f1, _d1, _f2, _d2, _f3, _d3);\
		PE_SHP_HW_H15A_RES_SET_USER(_r, _f1, _d1);\
		PE_SHP_HW_H15A_RES_SET_USER(_r, _f2, _d2);\
		PE_SHP_HW_H15A_RES_SET_USER(_r, _f3, _d3);}
#define PE_SHP_HW_H15A_RES_WR04(_r, _f1, _d1, _f2, _d2, _f3, _d3, _f4, _d4)	\
	{	PE_SHPLR_H15A0_QWr04(_r, _f1, _d1, _f2, _d2, _f3, _d3, _f4, _d4);\
		PE_SHP_HW_H15A_RES_SET_USER(_r, _f1, _d1);\
		PE_SHP_HW_H15A_RES_SET_USER(_r, _f2, _d2);\
		PE_SHP_HW_H15A_RES_SET_USER(_r, _f3, _d3);\
		PE_SHP_HW_H15A_RES_SET_USER(_r, _f4, _d4);}
#define PE_SHP_HW_H15A_RES_WR05(_r, _f1, _d1, _f2, _d2, _f3, _d3, _f4, _d4, _f5, _d5)	\
	{	PE_SHPLR_H15A0_QWr05(_r, _f1, _d1, _f2, _d2, _f3, _d3, _f4, _d4, _f5, _d5);\
		PE_SHP_HW_H15A_RES_SET_USER(_r, _f1, _d1);\
		PE_SHP_HW_H15A_RES_SET_USER(_r, _f2, _d2);\
		PE_SHP_HW_H15A_RES_SET_USER(_r, _f3, _d3);\
		PE_SHP_HW_H15A_RES_SET_USER(_r, _f4, _d4);\
		PE_SHP_HW_H15A_RES_SET_USER(_r, _f5, _d5);}

#define PE_SHP_HW_H15A_PBL_SET_USER(_r, _f, _d)	\
	{	_g_pe_pbl_hw_param_data_h15a.reg_user.data->_r._f = (_d);\
		_g_pe_pbl_hw_param_data_h15a.reg_mask.data->_r._f = 0;}

#define PE_SHP_HW_H15A_PBL_WR01(_r, _f1, _d1)	\
	{	PE_DNR12_H15A0_QWr01(_r, _f1, _d1);\
		PE_SHP_HW_H15A_PBL_SET_USER(_r, _f1, _d1);}
#define PE_SHP_HW_H15A_PBL_WR02(_r, _f1, _d1, _f2, _d2)	\
	{	PE_DNR12_H15A0_QWr02(_r, _f1, _d1, _f2, _d2);\
		PE_SHP_HW_H15A_PBL_SET_USER(_r, _f1, _d1);\
		PE_SHP_HW_H15A_PBL_SET_USER(_r, _f2, _d2);}
#define PE_SHP_HW_H15A_PBL_WR03(_r, _f1, _d1, _f2, _d2, _f3, _d3)	\
	{	PE_DNR12_H15A0_QWr03(_r, _f1, _d1, _f2, _d2, _f3, _d3);\
		PE_SHP_HW_H15A_PBL_SET_USER(_r, _f1, _d1);\
		PE_SHP_HW_H15A_PBL_SET_USER(_r, _f2, _d2);\
		PE_SHP_HW_H15A_PBL_SET_USER(_r, _f3, _d3);}
#define PE_SHP_HW_H15A_PBL_WR04(_r, _f1, _d1, _f2, _d2, _f3, _d3, _f4, _d4)	\
	{	PE_DNR12_H15A0_QWr04(_r, _f1, _d1, _f2, _d2, _f3, _d3, _f4, _d4);\
		PE_SHP_HW_H15A_PBL_SET_USER(_r, _f1, _d1);\
		PE_SHP_HW_H15A_PBL_SET_USER(_r, _f2, _d2);\
		PE_SHP_HW_H15A_PBL_SET_USER(_r, _f3, _d3);\
		PE_SHP_HW_H15A_PBL_SET_USER(_r, _f4, _d4);}
#define PE_SHP_HW_H15A_PBL_WR05(_r, _f1, _d1, _f2, _d2, _f3, _d3, _f4, _d4, _f5, _d5)	\
	{	PE_DNR12_H15A0_QWr05(_r, _f1, _d1, _f2, _d2, _f3, _d3, _f4, _d4, _f5, _d5);\
		PE_SHP_HW_H15A_PBL_SET_USER(_r, _f1, _d1);\
		PE_SHP_HW_H15A_PBL_SET_USER(_r, _f2, _d2);\
		PE_SHP_HW_H15A_PBL_SET_USER(_r, _f3, _d3);\
		PE_SHP_HW_H15A_PBL_SET_USER(_r, _f4, _d4);\
		PE_SHP_HW_H15A_PBL_SET_USER(_r, _f5, _d5);}

#ifdef PE_SHP_PARAM_CHANGE_FOR_H15
#define PE_SHP_HW_H15A_SRE_SET_USER(_r, _f, _d)	\
	{	_g_pe_sre_hw_param_data_h15a.reg_user.data->_r._f = (_d);\
		_g_pe_sre_hw_param_data_h15a.reg_mask.data->_r._f = 0;}

#define PE_SHP_HW_H15A_SR_S1_WR01(_r, _f1, _d1)	\
	{	PE_SR_S1_H15A0_QWr01(_r, _f1, _d1);\
		PE_SHP_HW_H15A_SRE_SET_USER(_r, _f1, _d1);}
#define PE_SHP_HW_H15A_SR_S1_WR02(_r, _f1, _d1, _f2, _d2)	\
	{	PE_SR_S1_H15A0_QWr02(_r, _f1, _d1, _f2, _d2);\
		PE_SHP_HW_H15A_SRE_SET_USER(_r, _f1, _d1);\
		PE_SHP_HW_H15A_SRE_SET_USER(_r, _f2, _d2);}
#define PE_SHP_HW_H15A_SR_S1_WR03(_r, _f1, _d1, _f2, _d2, _f3, _d3)	\
	{	PE_SR_S1_H15A0_QWr03(_r, _f1, _d1, _f2, _d2, _f3, _d3);\
		PE_SHP_HW_H15A_SRE_SET_USER(_r, _f1, _d1);\
		PE_SHP_HW_H15A_SRE_SET_USER(_r, _f2, _d2);\
		PE_SHP_HW_H15A_SRE_SET_USER(_r, _f3, _d3);}
#define PE_SHP_HW_H15A_SR_S1_WR04(_r, _f1, _d1, _f2, _d2, _f3, _d3, _f4, _d4)	\
	{	PE_SR_S1_H15A0_QWr04(_r, _f1, _d1, _f2, _d2, _f3, _d3, _f4, _d4);\
		PE_SHP_HW_H15A_SRE_SET_USER(_r, _f1, _d1);\
		PE_SHP_HW_H15A_SRE_SET_USER(_r, _f2, _d2);\
		PE_SHP_HW_H15A_SRE_SET_USER(_r, _f3, _d3);\
		PE_SHP_HW_H15A_SRE_SET_USER(_r, _f4, _d4);}

#define PE_SHP_HW_H15A_SR_F1_WR01(_r, _f1, _d1)	\
	{	PE_SR_F1_H15A0_QWr01(_r, _f1, _d1);\
		PE_SHP_HW_H15A_SRE_SET_USER(_r, _f1, _d1);}
#define PE_SHP_HW_H15A_SR_F1_WR02(_r, _f1, _d1, _f2, _d2)	\
	{	PE_SR_F1_H15A0_QWr02(_r, _f1, _d1, _f2, _d2);\
		PE_SHP_HW_H15A_SRE_SET_USER(_r, _f1, _d1);\
		PE_SHP_HW_H15A_SRE_SET_USER(_r, _f2, _d2);}
#define PE_SHP_HW_H15A_SR_F1_WR03(_r, _f1, _d1, _f2, _d2, _f3, _d3)	\
	{	PE_SR_F1_H15A0_QWr03(_r, _f1, _d1, _f2, _d2, _f3, _d3);\
		PE_SHP_HW_H15A_SRE_SET_USER(_r, _f1, _d1);\
		PE_SHP_HW_H15A_SRE_SET_USER(_r, _f2, _d2);\
		PE_SHP_HW_H15A_SRE_SET_USER(_r, _f3, _d3);}
#define PE_SHP_HW_H15A_SR_F1_WR04(_r, _f1, _d1, _f2, _d2, _f3, _d3, _f4, _d4)	\
	{	PE_SR_F1_H15A0_QWr04(_r, _f1, _d1, _f2, _d2, _f3, _d3, _f4, _d4);\
		PE_SHP_HW_H15A_SRE_SET_USER(_r, _f1, _d1);\
		PE_SHP_HW_H15A_SRE_SET_USER(_r, _f2, _d2);\
		PE_SHP_HW_H15A_SRE_SET_USER(_r, _f3, _d3);\
		PE_SHP_HW_H15A_SRE_SET_USER(_r, _f4, _d4);}

/* sre shp(srs) */
#define PE_SHP_HW_H15A_SRS_SET_USER(_r, _f, _d)	\
	{	_g_pe_srs_hw_param_data_h15a.reg_user.data->_r._f = (_d);\
		_g_pe_srs_hw_param_data_h15a.reg_mask.data->_r._f = 0;}

#ifdef PE_SHP_HW_H15_USE_SRE_SHP_WR_ONLY_MD
/* for wr only mode */
#define PE_SHP_HW_H15A_SRS_SET_USER_DATA(_r, _f, _d)	\
	{	_g_pe_srs_hw_param_data_h15a.reg_data.data->_r._f = (_d);\
		_g_pe_srs_hw_param_data_h15a.reg_user.data->_r._f = (_d);\
		_g_pe_srs_hw_param_data_h15a.reg_mask.data->_r._f = 0;}
#define PE_SHP_HW_H15A_SRS_DATA_TO_U32(_r)	\
	(*((UINT32 *)&(_g_pe_srs_hw_param_data_h15a.reg_data.data->_r)))

#define PE_SHP_HW_H15A_SR_SHP_WR01(_r, _f1, _d1)	\
	{	PE_SHP_HW_H15A_SRS_SET_USER_DATA(_r, _f1, _d1);\
		PE_SR_SHP_H15A0_Wr(_r,PE_SHP_HW_H15A_SRS_DATA_TO_U32(_r));\
		PE_SR_SHP_H15A0_WrFL(_r);}
#define PE_SHP_HW_H15A_SR_SHP_WR02(_r, _f1, _d1, _f2, _d2)	\
	{	PE_SHP_HW_H15A_SRS_SET_USER_DATA(_r, _f1, _d1);\
		PE_SHP_HW_H15A_SRS_SET_USER_DATA(_r, _f2, _d2);\
		PE_SR_SHP_H15A0_Wr(_r,PE_SHP_HW_H15A_SRS_DATA_TO_U32(_r));\
		PE_SR_SHP_H15A0_WrFL(_r);}
#define PE_SHP_HW_H15A_SR_SHP_WR03(_r, _f1, _d1, _f2, _d2, _f3, _d3)	\
	{	PE_SHP_HW_H15A_SRS_SET_USER_DATA(_r, _f1, _d1);\
		PE_SHP_HW_H15A_SRS_SET_USER_DATA(_r, _f2, _d2);\
		PE_SHP_HW_H15A_SRS_SET_USER_DATA(_r, _f3, _d3);\
		PE_SR_SHP_H15A0_Wr(_r,PE_SHP_HW_H15A_SRS_DATA_TO_U32(_r));\
		PE_SR_SHP_H15A0_WrFL(_r);}
#define PE_SHP_HW_H15A_SR_SHP_WR04(_r, _f1, _d1, _f2, _d2, _f3, _d3, _f4, _d4)	\
	{	PE_SHP_HW_H15A_SRS_SET_USER_DATA(_r, _f1, _d1);\
		PE_SHP_HW_H15A_SRS_SET_USER_DATA(_r, _f2, _d2);\
		PE_SHP_HW_H15A_SRS_SET_USER_DATA(_r, _f3, _d3);\
		PE_SHP_HW_H15A_SRS_SET_USER_DATA(_r, _f4, _d4);\
		PE_SR_SHP_H15A0_Wr(_r,PE_SHP_HW_H15A_SRS_DATA_TO_U32(_r));\
		PE_SR_SHP_H15A0_WrFL(_r);}
#define PE_SHP_HW_H15A_SR_SHP_WR05(_r, _f1, _d1, _f2, _d2, _f3, _d3, _f4, _d4, _f5, _d5)	\
	{	PE_SHP_HW_H15A_SRS_SET_USER_DATA(_r, _f1, _d1);\
		PE_SHP_HW_H15A_SRS_SET_USER_DATA(_r, _f2, _d2);\
		PE_SHP_HW_H15A_SRS_SET_USER_DATA(_r, _f3, _d3);\
		PE_SHP_HW_H15A_SRS_SET_USER_DATA(_r, _f4, _d4);\
		PE_SHP_HW_H15A_SRS_SET_USER_DATA(_r, _f5, _d5);\
		PE_SR_SHP_H15A0_Wr(_r,PE_SHP_HW_H15A_SRS_DATA_TO_U32(_r));\
		PE_SR_SHP_H15A0_WrFL(_r);}
#else	//#ifdef PE_SHP_HW_H15_USE_SRE_SHP_WR_ONLY_MD
#define PE_SHP_HW_H15A_SR_SHP_WR01(_r, _f1, _d1)	\
	{	PE_SR_SHP_H15A0_QWr01(_r, _f1, _d1);\
		PE_SHP_HW_H15A_SRS_SET_USER(_r, _f1, _d1);}
#define PE_SHP_HW_H15A_SR_SHP_WR02(_r, _f1, _d1, _f2, _d2)	\
	{	PE_SR_SHP_H15A0_QWr02(_r, _f1, _d1, _f2, _d2);\
		PE_SHP_HW_H15A_SRS_SET_USER(_r, _f1, _d1);\
		PE_SHP_HW_H15A_SRS_SET_USER(_r, _f2, _d2);}
#define PE_SHP_HW_H15A_SR_SHP_WR03(_r, _f1, _d1, _f2, _d2, _f3, _d3)	\
	{	PE_SR_SHP_H15A0_QWr03(_r, _f1, _d1, _f2, _d2, _f3, _d3);\
		PE_SHP_HW_H15A_SRS_SET_USER(_r, _f1, _d1);\
		PE_SHP_HW_H15A_SRS_SET_USER(_r, _f2, _d2);\
		PE_SHP_HW_H15A_SRS_SET_USER(_r, _f3, _d3);}
#define PE_SHP_HW_H15A_SR_SHP_WR04(_r, _f1, _d1, _f2, _d2, _f3, _d3, _f4, _d4)	\
	{	PE_SR_SHP_H15A0_QWr04(_r, _f1, _d1, _f2, _d2, _f3, _d3, _f4, _d4);\
		PE_SHP_HW_H15A_SRS_SET_USER(_r, _f1, _d1);\
		PE_SHP_HW_H15A_SRS_SET_USER(_r, _f2, _d2);\
		PE_SHP_HW_H15A_SRS_SET_USER(_r, _f3, _d3);\
		PE_SHP_HW_H15A_SRS_SET_USER(_r, _f4, _d4);}
#define PE_SHP_HW_H15A_SR_SHP_WR05(_r, _f1, _d1, _f2, _d2, _f3, _d3, _f4, _d4, _f5, _d5)	\
	{	PE_SR_SHP_H15A0_QWr05(_r, _f1, _d1, _f2, _d2, _f3, _d3, _f4, _d4, _f5, _d5);\
		PE_SHP_HW_H15A_SRS_SET_USER(_r, _f1, _d1);\
		PE_SHP_HW_H15A_SRS_SET_USER(_r, _f2, _d2);\
		PE_SHP_HW_H15A_SRS_SET_USER(_r, _f3, _d3);\
		PE_SHP_HW_H15A_SRS_SET_USER(_r, _f4, _d4);\
		PE_SHP_HW_H15A_SRS_SET_USER(_r, _f5, _d5);}
#endif	//#ifdef PE_SHP_HW_H15_USE_SRE_SHP_WR_ONLY_MD

#endif
/* set table */
#define PE_SHP_HW_H15A_SET_REG_TABLE(__tbl, __base)	\
	do{\
		UINT32 _i, _size;\
		PE_SHP_HW_H15_CHECK_CODE(!_dflt, ret=RET_ERROR;break, \
			"[%s,%d] _dflt is null, not ready.\n", __F__, __L__);\
		PE_SHP_HW_H15_CHECK_CODE(!_user, ret=RET_ERROR;break, \
			"[%s,%d] _user is null, not ready.\n", __F__, __L__);\
		PE_SHP_HW_H15_CHECK_CODE(!_mask, ret=RET_ERROR;break, \
			"[%s,%d] _mask is null, not ready.\n", __F__, __L__);\
		PE_SHP_HW_H15_CHECK_CODE(!_data, ret=RET_ERROR;break, \
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
			PE_SHP_HW_H15_CHECK_CODE(ret, break, \
				"[%s,%d] PE_REG_H15_WrAllocatedAddr() error.\n", __F__, __L__);\
		}\
		if (ret == RET_OK)\
		{\
			PE_SHP_HW_H15_DBG_PRINT("write %s(+ 0x%08x).\n", #__tbl, (__base));\
			ret = PE_REG_SetTraceTable(__F__,__L__, #__tbl, (__base));\
		}\
	}while(0)

/* shp */
#define PE_SHP_HW_H15A_RES_REG_TABLE(__tbl, __base)		\
	do{\
		UINT32 *_dflt = _g_pe_res_hw_param_data_h15a.reg_dflt.addr;\
		UINT32 *_user = _g_pe_res_hw_param_data_h15a.reg_user.addr;\
		UINT32 *_mask = _g_pe_res_hw_param_data_h15a.reg_mask.addr;\
		UINT32 *_data = _g_pe_res_hw_param_data_h15a.reg_data.addr;\
		PE_SHP_HW_H15A_SET_REG_TABLE(__tbl, __base);\
	}while (0)

#ifdef PE_SHP_HW_H15_SRE_SHP_WR_ONLY_MD
/* sre shp(srs) */
#define PE_SHP_HW_H15A_SRS_REG_TABLE(__tbl, __base)		\
	do{\
		UINT32 *_dflt = _g_pe_srs_hw_param_data_h15a.reg_dflt.addr;\
		UINT32 *_user = _g_pe_srs_hw_param_data_h15a.reg_user.addr;\
		UINT32 *_mask = _g_pe_srs_hw_param_data_h15a.reg_mask.addr;\
		UINT32 *_data = _g_pe_srs_hw_param_data_h15a.reg_data.addr;\
		PE_SHP_HW_H15A_SET_REG_TABLE(__tbl, __base);\
	}while (0)
#endif

/* cti */
#define PE_SHP_HW_H15A_CTI_REG_TABLE(__tbl, __base)		\
	do{\
		UINT32 *_dflt = _g_pe_cti_hw_param_data_h15a.reg_dflt.addr;\
		UINT32 *_user = _g_pe_cti_hw_param_data_h15a.reg_user.addr;\
		UINT32 *_mask = _g_pe_cti_hw_param_data_h15a.reg_mask.addr;\
		UINT32 *_data = _g_pe_cti_hw_param_data_h15a.reg_data.addr;\
		PE_SHP_HW_H15A_SET_REG_TABLE(__tbl, __base);\
	}while (0)

/* pbl */
#define PE_SHP_HW_H15A_PBL_REG_TABLE(__tbl, __base)		\
	do{\
		UINT32 *_dflt = _g_pe_pbl_hw_param_data_h15a.reg_dflt.addr;\
		UINT32 *_user = _g_pe_pbl_hw_param_data_h15a.reg_user.addr;\
		UINT32 *_mask = _g_pe_pbl_hw_param_data_h15a.reg_mask.addr;\
		UINT32 *_data = _g_pe_pbl_hw_param_data_h15a.reg_data.addr;\
		PE_SHP_HW_H15A_SET_REG_TABLE(__tbl, __base);\
	}while (0)

/* set msc filter */
#define PE_SHP_HW_H15A_SET_MSC_FLT(__tbl, __base)	\
	do{\
		UINT32 _i, _size;\
		_size = sizeof(__tbl)/sizeof(PE_REG_PARAM_T);\
		for (_i=0; _i<_size; _i++)\
		{\
			ret = PE_REG_H15_WrAllocatedAddr((__tbl[_i].addr+(__base)), (__tbl[_i].data));\
			PE_SHP_HW_H15_CHECK_CODE(ret, break, \
				"[%s,%d] PE_REG_H15_WrAllocatedAddr() error.\n", __F__, __L__);\
		}\
		if (ret == RET_OK)\
		{\
			PE_SHP_HW_H15_DBG_PRINT("write %s(+ 0x%08x).\n", #__tbl, (__base));\
			ret = PE_REG_SetTraceTable(__F__,__L__, #__tbl, (__base));\
		}\
	}while(0)

#endif

/* hex(8bit) to dec : eg. 0xAC -> 1012 */
#define PE_SHP_HW_H15_IDX_HEX_TO_DEC(_x)	(GET_BITS((_x),4,4)*100+GET_BITS((_x),0,4))
/* dec to hex(8bit) : eg. 1012 -> 0xAC */
#define PE_SHP_HW_H15_IDX_DEC_TO_HEX(_x)	(((((_x)/100)&0xf)<<4)+(((_x)%100)&0xf))

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
static int PE_SHP_HW_H15A_SetDefault(LX_PE_DEFAULT_SETTINGS_T *pstParams);
static UINT32 PE_SHP_HW_H15_ChkValid12tMain(UINT32 co_num);
static UINT32 PE_SHP_HW_H15_ChkValid12tSub(UINT32 co_num);
static UINT32 PE_SHP_HW_H15_ChkValid06tMain(UINT32 co_num);
static void PE_SHP_HW_H15_WrMscH1Reg(UINT32 co_num, UINT32 sel);
static void PE_SHP_HW_H15_WrMscH2Reg(UINT32 co_num, UINT32 sel);
static void PE_SHP_HW_H15_WrMscV1Reg(UINT32 co_num, UINT32 sel);
static void PE_SHP_HW_H15_WrMscV2Reg(UINT32 co_num, UINT32 sel);
static UINT32 PE_SHP_HW_H15_GetMscH1CoNum(UINT32 sel);
static UINT32 PE_SHP_HW_H15_GetMscH2CoNum(UINT32 sel);
static UINT32 PE_SHP_HW_H15_GetMscV1CoNum(UINT32 sel);
static UINT32 PE_SHP_HW_H15_GetMscV2CoNum(UINT32 sel);
static int PE_SHP_HW_H15_SetMscM1Y(PE_TSK_H15_CFG_T *pCfg);
static int PE_SHP_HW_H15_SetMscM2Y(PE_TSK_H15_CFG_T *pCfg);
static int PE_SHP_HW_H15_SetMscM1C(PE_TSK_H15_CFG_T *pCfg);
static int PE_SHP_HW_H15_SetMscM2C(PE_TSK_H15_CFG_T *pCfg);
static int PE_SHP_HW_H15_SetMscMPY(PE_TSK_H15_CFG_T *pCfg);
static PE_SHP_HW_H15A_RES_FMT PE_SHP_HW_H15A_ConvDispInfoToResFmt(LX_PE_INF_DISPLAY_T *disp_inf);
#ifdef PE_SHP_HW_H15_SRE_SHP_WR_ONLY_MD
static PE_SHP_HW_H15A_SRS_FMT PE_SHP_HW_H15A_ConvDispInfoToSrsFmt(LX_PE_INF_DISPLAY_T *disp_inf);
static PE_SHP_HW_H15A_SRE_FMT PE_SHP_HW_H15A_ConvDispInfoToSreFmt(LX_PE_INF_DISPLAY_T *disp_inf);
#endif
static PE_SHP_HW_H15A_CTI_FMT PE_SHP_HW_H15A_ConvDispInfoToCtiFmt(LX_PE_INF_DISPLAY_T *disp_inf);
static PE_SHP_HW_H15A_PBL_FMT PE_SHP_HW_H15A_ConvDispInfoToPblFmt(LX_PE_INF_DISPLAY_T *disp_inf);
#ifdef PE_HW_H15_PROTECT_OVERLAP_SETTINGS
static int PE_SHP_HW_H15A_CreateDataTable(UINT32 index);
#endif
#ifdef PE_SHP_HW_H15_SCL_CO_INTERNAL_DECISION
static int PE_SHP_HW_H15_SelectScFltCoeff(PE_TSK_H15_CFG_T *pCfg);
#endif

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
#ifdef PE_SHP_HW_H15_SR_PWD_TRACE
static UINT8 _g_shp_hw_h15_trace=0x1;	//default should be off.
#else
static UINT8 _g_shp_hw_h15_trace=0x0;	//default should be off.
#endif
static PE_SHP_HW_H15_SETTINGS_T _g_pe_shp_hw_h15_info;
#ifdef PE_HW_H15_PROTECT_OVERLAP_SETTINGS
static PE_RES_HW_PARAM_DATA_H15A0_T _g_pe_res_hw_param_data_h15a = {{NULL}, {NULL}, {NULL}, {NULL}};
static PE_CTI_HW_PARAM_DATA_H15A0_T _g_pe_cti_hw_param_data_h15a = {{NULL}, {NULL}, {NULL}, {NULL}};
static PE_PBL_HW_PARAM_DATA_H15A0_T _g_pe_pbl_hw_param_data_h15a = {{NULL}, {NULL}, {NULL}, {NULL}};
#ifdef PE_SHP_HW_H15_SRE_SHP_WR_ONLY_MD
static PE_SRS_HW_PARAM_DATA_H15A0_T _g_pe_srs_hw_param_data_h15a = {{NULL}, {NULL}, {NULL}, {NULL}};
#endif
#ifdef PE_SHP_PARAM_CHANGE_FOR_H15
//static PE_SRE_HW_PARAM_DATA_H15A0_T _g_pe_sre_hw_param_data_h15a = {{NULL}, {NULL}, {NULL}, {NULL}};
//static PE_SRE_RES_HW_PARAM_DATA_H15A0_T _g_pe_sre_res_hw_param_data_h15a = {{NULL}, {NULL}, {NULL}, {NULL}};
#endif
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
int PE_SHP_HW_H15_Init(PE_CFG_CTRL_T *pstParams)
{
	int ret = RET_OK;
	#ifdef PE_SHP_HW_H15_SR_PWD_TRACE
	UINT32 r_data[2];
	#endif
	do{
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H15)
		{
			#ifdef PE_SHP_HW_H15_SR_PWD_TRACE
			CTOP_CTRL_H15A0_RdFL(SR,ctr00);
			CTOP_CTRL_H15A0_Rd01(SR,ctr00,cg_sr_en,r_data[0]);
			CTOP_CTRL_H15A0_Rd01(SR,ctr00,cg_shs_en,r_data[1]);
			PE_SHP_HW_H15_DBG_SRPD_TRACE("S cg sr,shs:%d,%d\n",r_data[0],r_data[1]);
			#endif
			memset(&(_g_pe_shp_hw_h15_info), -1, sizeof(PE_SHP_HW_H15_SETTINGS_T));
			_g_pe_shp_hw_h15_info.res0a_fmt = PE_SHP_H15A_RES_NUM;
			_g_pe_shp_hw_h15_info.cti0a_fmt = PE_SHP_H15A_CTI_NUM;
			_g_pe_shp_hw_h15_info.pbl0a_fmt = PE_SHP_H15A_PBL_NUM;
			_g_pe_shp_hw_h15_info.sre0a_fmt = PE_SHP_H15A_RES_NUM;
			#ifdef PE_SHP_HW_H15_SRE_SHP_WR_ONLY_MD
			_g_pe_shp_hw_h15_info.srs0a_fmt = PE_SHP_H15A_SRS_NUM;
			#endif
			PE_SHP_HW_H15_DBG_PRINT("init cti.\n");
			#ifdef PE_HW_H15_PROTECT_OVERLAP_SETTINGS
			/* res(pe1 shp l,r) : resolution enhance */
			ret = PE_SHP_HW_H15A_CreateDataTable(PE_SHP_HW_H15A_TBL_IDX_RES);
			PE_SHP_HW_H15_CHECK_CODE(ret, break, \
				"PE_SHP_HW_H15A_CreateDataTable() error.\n");
			PE_SHP_HW_H15A_RES_REG_TABLE(shp_l_init_h15a0,PE_H15A_REG_WRITE_BASE);
			PE_SHP_HW_H15A_RES_REG_TABLE(shp_l_init_h15a0,PE_H15A_P1_SHP_LR_DIFF);
			#ifdef PE_SHP_HW_H15_SRE_SHP_WR_ONLY_MD
			/* sre shp(srs) */
			ret = PE_SHP_HW_H15A_CreateDataTable(PE_SHP_HW_H15A_TBL_IDX_SRS);
			PE_SHP_HW_H15_CHECK_CODE(ret, break, \
				"PE_SHP_HW_H15A_CreateDataTable() error.\n");
			PE_SHP_HW_H15A_SRS_REG_TABLE(sre_res_init_h15a0,PE_H15A_REG_WRITE_BASE);
			#else
			PE_SET_REG_TABLE(SHP_HW_H15,sre_res_hd_default_h15a0,PE_H15A_REG_WRITE_BASE);
			#endif
			/* cti(dnr1,2) */
			ret = PE_SHP_HW_H15A_CreateDataTable(PE_SHP_HW_H15A_TBL_IDX_CTI);
			PE_SHP_HW_H15_CHECK_CODE(ret, break, \
				"PE_SHP_HW_H15A_CreateDataTable() error.\n");
			PE_SHP_HW_H15A_CTI_REG_TABLE(cti_dnr_l_init_h15a0,PE_H15A_REG_WRITE_BASE);
			PE_SHP_HW_H15A_CTI_REG_TABLE(cti_dnr_l_init_h15a0,PE_H15A_ND_DNR_12_DIFF);
			/* pbl(pre blur dnr1,2) */
			ret = PE_SHP_HW_H15A_CreateDataTable(PE_SHP_HW_H15A_TBL_IDX_PBL);
			PE_SHP_HW_H15_CHECK_CODE(ret, break, \
				"PE_SHP_HW_H15A_CreateDataTable() error.\n");
			PE_SHP_HW_H15A_PBL_REG_TABLE(pblur_dnr_l_init_h15a0,PE_H15A_REG_WRITE_BASE);
			PE_SHP_HW_H15A_PBL_REG_TABLE(pblur_dnr_l_init_h15a0,PE_H15A_ND_DNR_12_DIFF);
			/* sqm */
			PE_SHPLR_H15A0_QWr01(pe1_bbd_ctrl_00,reg_bbd_en,0);//for sqm win = top ctrl w,h
			PE_SHPL_H15A0_Wr(pe1_sqm_ctrl_00,0x408020E0);
			PE_SHPL_H15A0_Wr(pe1_sqm_ctrl_01,0x40806000);
			PE_SHPL_H15A0_Wr(pe1_sqm_ctrl_02,0x00000000);//0x00000780:x0,y0:1920,0
			PE_SHPL_H15A0_Wr(pe1_sqm_ctrl_03,0x00000000);//0x086F07BF:x1,y1:1983,2159
			PE_SHPL_H15A0_Wr(pe1_sqm_ctrl_04,0x00650040);
			PE_SHPL_H15A0_Wr(pe1_sqm_ctrl_05,0x00103020);
			PE_SHPL_H15A0_WrFL(pe1_sqm_ctrl_00);
			PE_SHPL_H15A0_WrFL(pe1_sqm_ctrl_01);
			PE_SHPL_H15A0_WrFL(pe1_sqm_ctrl_02);
			PE_SHPL_H15A0_WrFL(pe1_sqm_ctrl_03);
			PE_SHPL_H15A0_WrFL(pe1_sqm_ctrl_04);
			PE_SHPL_H15A0_WrFL(pe1_sqm_ctrl_05);
			PE_SHPR_H15A0_Wr(pe1_sqm_ctrl_00,0x408020E0);
			PE_SHPR_H15A0_Wr(pe1_sqm_ctrl_01,0x40806000);
			PE_SHPR_H15A0_Wr(pe1_sqm_ctrl_02,0x00000000);//0x00000000:x0,y0:0,0
			PE_SHPR_H15A0_Wr(pe1_sqm_ctrl_03,0x00000000);//0x086F003F:x1,y1:63,2159
			PE_SHPR_H15A0_Wr(pe1_sqm_ctrl_04,0x00650040);
			PE_SHPR_H15A0_Wr(pe1_sqm_ctrl_05,0x00103020);
			PE_SHPR_H15A0_WrFL(pe1_sqm_ctrl_00);
			PE_SHPR_H15A0_WrFL(pe1_sqm_ctrl_01);
			PE_SHPR_H15A0_WrFL(pe1_sqm_ctrl_02);
			PE_SHPR_H15A0_WrFL(pe1_sqm_ctrl_03);
			PE_SHPR_H15A0_WrFL(pe1_sqm_ctrl_04);
			PE_SHPR_H15A0_WrFL(pe1_sqm_ctrl_05);
			PE_SET_REG_TABLE(SHP_HW_H15,sre_sqm_init_h15a0,PE_H15A_REG_WRITE_BASE);
			/* sre */
			PE_SET_REG_TABLE(SHP_HW_H15,sre_init_h15a0,PE_H15A_REG_WRITE_BASE);
			#else
			/* shp */
			PE_SET_REG_TABLE(SHP_HW_H15,shp_l_init_h15a0,PE_H15A_REG_WRITE_BASE);
			PE_SET_REG_TABLE(SHP_HW_H15,shp_l_init_h15a0,PE_H15A_P1_SHP_LR_DIFF);
			/* cti(dnr1,2) */
			PE_SET_REG_TABLE(SHP_HW_H15,cti_dnr_l_init_h15a0,PE_H15A_REG_WRITE_BASE);
			PE_SET_REG_TABLE(SHP_HW_H15,cti_dnr_l_init_h15a0,PE_H15A_ND_DNR_12_DIFF);
			/* pbl(pre blur dnr1,2) */
			PE_SET_REG_TABLE(SHP_HW_H15,pblur_dnr_l_init_h15a0,PE_H15A_REG_WRITE_BASE);
			PE_SET_REG_TABLE(SHP_HW_H15,pblur_dnr_l_init_h15a0,PE_H15A_ND_DNR_12_DIFF);
			/* sqm */
			PE_SET_REG_TABLE(SHP_HW_H15,sqm_l_init_h15a0,PE_H15A_REG_WRITE_BASE);
			PE_SET_REG_TABLE(SHP_HW_H15,sqm_l_init_h15a0,PE_H15A_P1_SHP_LR_DIFF);
			#endif
			#ifdef PE_SHP_HW_H15_USE_FWS_REG_FOR_FW_MSC_WR
			PE_MSC_Y_H15A0_RdFL(msc_m1_y_ctrl);
			PE_MSC_Y_H15A0_RdFL(msc_m2_y_ctrl);
			PE_MSC_C_H15A0_RdFL(msc_m1_c_ctrl);
			PE_MSC_C_H15A0_RdFL(msc_m2_c_ctrl);
			PE_SHP_HW_H15A_SET_SHDW_MSC_Y_TO_SHP(pe1_msc_m1_y_ctrl, msc_m1_y_ctrl);
			PE_SHP_HW_H15A_SET_SHDW_MSC_Y_TO_SHP(pe1_msc_m2_y_ctrl, msc_m2_y_ctrl);
			PE_SHP_HW_H15A_SET_SHDW_MSC_C_TO_SHP(pe1_msc_m1_c_ctrl, msc_m1_c_ctrl);
			PE_SHP_HW_H15A_SET_SHDW_MSC_C_TO_SHP(pe1_msc_m2_c_ctrl, msc_m2_c_ctrl);
			PE_SHPL_H15A0_Wr01(pe1_msc_m1_y_ctrl,reg_mode_32p_m1,0x0);		//phase 0x0:256,0x1:32
			PE_SHPL_H15A0_Wr01(pe1_msc_m1_y_ctrl,reg_min_max_on_m1,0x1);
			PE_SHPL_H15A0_Wr01(pe1_msc_m1_y_ctrl,blending_range,0x1);		//added by inkyung.park with tuning 9/29
			PE_SHPL_H15A0_Wr01(pe1_msc_m1_y_ctrl,reg_min_max_th_m1,0x20);
			PE_SHPL_H15A0_Wr01(pe1_msc_m2_y_ctrl,reg_mode_32p_m2,0x0);		//phase 0x0:256,0x1:32
			PE_SHPL_H15A0_Wr01(pe1_msc_m2_y_ctrl,reg_min_max_on_m2,0x1);
			PE_SHPL_H15A0_Wr01(pe1_msc_m2_y_ctrl,blending_range,0x1);		//added by inkyung.park with tuning 9/29
			PE_SHPL_H15A0_Wr01(pe1_msc_m2_y_ctrl,reg_min_max_th_m2,0x20);
			PE_SHPL_H15A0_Wr01(pe1_msc_m1_c_ctrl,reg_mode_32p_m1,0x0);		//phase 0x0:256,0x1:32
			PE_SHPL_H15A0_Wr01(pe1_msc_m1_c_ctrl,reg_min_max_on_m1,0x1);
			PE_SHPL_H15A0_Wr01(pe1_msc_m1_c_ctrl,reg_min_max_th_m1,0x1C);
			PE_SHPL_H15A0_Wr01(pe1_msc_m1_c_ctrl,reg_min_max_on_h_m1,0x1);
			PE_SHPL_H15A0_Wr01(pe1_msc_m2_c_ctrl,reg_mode_32p_m2,0x0);		//phase 0x0:256,0x1:32
			PE_SHPL_H15A0_Wr01(pe1_msc_m2_c_ctrl,reg_min_max_on_m2,0x1);
			PE_SHPL_H15A0_Wr01(pe1_msc_m2_c_ctrl,reg_min_max_th_m2,0x1C);
			PE_SHPL_H15A0_Wr01(pe1_msc_m2_c_ctrl,reg_min_max_on_h_m2,0x1);
			PE_SHPL_H15A0_WrFL(pe1_msc_m1_y_ctrl);
			PE_SHPL_H15A0_WrFL(pe1_msc_m2_y_ctrl);
			PE_SHPL_H15A0_WrFL(pe1_msc_m1_c_ctrl);
			PE_SHPL_H15A0_WrFL(pe1_msc_m2_c_ctrl);
			#else
			/* msc_y */
			PE_MSC_Y_H15A0_RdFL(msc_m1_y_ctrl);
			PE_MSC_Y_H15A0_Wr01(msc_m1_y_ctrl,reg_mode_32p_m1,0x0);		//phase 0x0:256,0x1:32
			PE_MSC_Y_H15A0_Wr01(msc_m1_y_ctrl,reg_min_max_on_m1,0x1);
			PE_MSC_Y_H15A0_Wr01(msc_m1_y_ctrl,blending_range,0x1);		//added by inkyung.park with tuning 9/29
			PE_MSC_Y_H15A0_Wr01(msc_m1_y_ctrl,reg_min_max_th_m1,0x20);
			PE_MSC_Y_H15A0_WrFL(msc_m1_y_ctrl);
			PE_MSC_Y_H15A0_RdFL(msc_m2_y_ctrl);
			PE_MSC_Y_H15A0_Wr01(msc_m2_y_ctrl,reg_mode_32p_m2,0x0);		//phase 0x0:256,0x1:32
			PE_MSC_Y_H15A0_Wr01(msc_m2_y_ctrl,reg_min_max_on_m2,0x1);
			PE_MSC_Y_H15A0_Wr01(msc_m2_y_ctrl,blending_range,0x1);		//added by inkyung.park with tuning 9/29
			PE_MSC_Y_H15A0_Wr01(msc_m2_y_ctrl,reg_min_max_th_m2,0x20);
			PE_MSC_Y_H15A0_WrFL(msc_m2_y_ctrl);
			/* msc_c */
			PE_MSC_C_H15A0_RdFL(msc_m1_c_ctrl);
			PE_MSC_C_H15A0_Wr01(msc_m1_c_ctrl,reg_mode_32p_m1,0x0);		//phase 0x0:256,0x1:32
			PE_MSC_C_H15A0_Wr01(msc_m1_c_ctrl,reg_min_max_on_m1,0x1);
			PE_MSC_C_H15A0_Wr01(msc_m1_c_ctrl,reg_min_max_th_m1,0x1C);
			PE_MSC_C_H15A0_Wr01(msc_m1_c_ctrl,reg_min_max_on_h_m1,0x1);
			PE_MSC_C_H15A0_WrFL(msc_m1_c_ctrl);
			PE_MSC_C_H15A0_RdFL(msc_m2_c_ctrl);
			PE_MSC_C_H15A0_Wr01(msc_m2_c_ctrl,reg_mode_32p_m2,0x0);		//phase 0x0:256,0x1:32
			PE_MSC_C_H15A0_Wr01(msc_m2_c_ctrl,reg_min_max_on_m2,0x1);
			PE_MSC_C_H15A0_Wr01(msc_m2_c_ctrl,reg_min_max_th_m2,0x1C);
			PE_MSC_C_H15A0_Wr01(msc_m2_c_ctrl,reg_min_max_on_h_m2,0x1);
			PE_MSC_C_H15A0_WrFL(msc_m2_c_ctrl);
			#endif
			#ifdef PE_SHP_HW_H15_SR_PWD_TRACE
			CTOP_CTRL_H15A0_RdFL(SR,ctr00);
			CTOP_CTRL_H15A0_Rd01(SR,ctr00,cg_sr_en,r_data[0]);
			CTOP_CTRL_H15A0_Rd01(SR,ctr00,cg_shs_en,r_data[1]);
			PE_SHP_HW_H15_DBG_SRPD_TRACE("E cg sr,shs:%d,%d\n",r_data[0],r_data[1]);
			#endif
		}
		else
		{
			PE_SHP_HW_H15_DBG_PRINT("nothing to do\n");	ret = RET_OK;
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
 * @see		PE_SHP_HW_H15A_TBL_INDX
 * @author
 */
static int PE_SHP_HW_H15A_CreateDataTable(UINT32 index)
{
	int ret = RET_OK;
	PE_RES_HW_PARAM_DATA_H15A0_T *p_res_h15a = &_g_pe_res_hw_param_data_h15a;
	PE_CTI_HW_PARAM_DATA_H15A0_T *p_cti_h15a = &_g_pe_cti_hw_param_data_h15a;
	PE_PBL_HW_PARAM_DATA_H15A0_T *p_pbl_h15a = &_g_pe_pbl_hw_param_data_h15a;
	#ifdef PE_SHP_HW_H15_SRE_SHP_WR_ONLY_MD
	PE_SRS_HW_PARAM_DATA_H15A0_T *p_srs_h15a = &_g_pe_srs_hw_param_data_h15a;
	#endif
	#ifdef PE_SHP_PARAM_CHANGE_FOR_H15
	//PE_SRE_HW_PARAM_DATA_H15A0_T *p_sre_h15a = &_g_pe_sre_hw_param_data_h15a;
	//PE_SRE_RES_HW_PARAM_DATA_H15A0_T *p_sre_res_h15a = &_g_pe_sre_res_hw_param_data_h15a;
	#endif

	PE_SHP_HW_H15_CHECK_CODE(index>=PE_SHP_HW_H15A_TBL_IDX_NUM, return RET_ERROR, \
		"[%s,%d] index(%d) invalid.\n", __F__, __L__, index);
	if (index == PE_SHP_HW_H15A_TBL_IDX_RES)
	{
		/* create dflt table */
		if (p_res_h15a->reg_dflt.addr == NULL)
		{
			p_res_h15a->reg_dflt.addr = (UINT32 *)OS_KMalloc(sizeof(PE_RES_HW_PARAM_REG_H15A0_T));
		}
		/* create user table */
		if (p_res_h15a->reg_user.addr == NULL)
		{
			p_res_h15a->reg_user.addr = (UINT32 *)OS_KMalloc(sizeof(PE_RES_HW_PARAM_REG_H15A0_T));
			memset((p_res_h15a->reg_user.addr), 0, sizeof(PE_RES_HW_PARAM_REG_H15A0_T));	// 0x0
		}
		/* create mask table */
		if (p_res_h15a->reg_mask.addr == NULL)
		{
			p_res_h15a->reg_mask.addr = (UINT32 *)OS_KMalloc(sizeof(PE_RES_HW_PARAM_REG_H15A0_T));
			memset((p_res_h15a->reg_mask.addr), -1, sizeof(PE_RES_HW_PARAM_REG_H15A0_T));// 0xffffffff
		}
		/* create data table */
		if (p_res_h15a->reg_data.addr == NULL)
		{
			p_res_h15a->reg_data.addr = (UINT32 *)OS_KMalloc(sizeof(PE_RES_HW_PARAM_REG_H15A0_T));
			memset((p_res_h15a->reg_data.addr), 0, sizeof(PE_RES_HW_PARAM_REG_H15A0_T));	// 0x0
		}
		/* report result */
		if (p_res_h15a->reg_dflt.addr != NULL && p_res_h15a->reg_user.addr != NULL && \
			p_res_h15a->reg_mask.addr != NULL && p_res_h15a->reg_data.addr != NULL)
		{
			PE_PRINT_NOTI("[%s]table ready.\n", PE_SHP_HW_H15A_TBL_IDX_TO_STR(index));
			ret = RET_OK;
		}
		else
		{
			PE_PRINT_NOTI("[%s]table not ready. dflt,user,mask,data:%s,%s,%s,%s\n", \
				PE_SHP_HW_H15A_TBL_IDX_TO_STR(index), \
				(p_res_h15a->reg_dflt.addr == NULL)? "x":"o", \
				(p_res_h15a->reg_user.addr == NULL)? "x":"o", \
				(p_res_h15a->reg_mask.addr == NULL)? "x":"o", \
				(p_res_h15a->reg_data.addr == NULL)? "x":"o");
			ret = RET_ERROR;
		}
	}
	else if (index == PE_SHP_HW_H15A_TBL_IDX_CTI)
	{
		/* create dflt table */
		if (p_cti_h15a->reg_dflt.addr == NULL)
		{
			p_cti_h15a->reg_dflt.addr = (UINT32 *)OS_KMalloc(sizeof(PE_CTI_HW_PARAM_REG_H15A0_T));
		}
		/* create user table */
		if (p_cti_h15a->reg_user.addr == NULL)
		{
			p_cti_h15a->reg_user.addr = (UINT32 *)OS_KMalloc(sizeof(PE_CTI_HW_PARAM_REG_H15A0_T));
			memset((p_cti_h15a->reg_user.addr), 0, sizeof(PE_CTI_HW_PARAM_REG_H15A0_T));	// 0x0
		}
		/* create mask table */
		if (p_cti_h15a->reg_mask.addr == NULL)
		{
			p_cti_h15a->reg_mask.addr = (UINT32 *)OS_KMalloc(sizeof(PE_CTI_HW_PARAM_REG_H15A0_T));
			memset((p_cti_h15a->reg_mask.addr), -1, sizeof(PE_CTI_HW_PARAM_REG_H15A0_T));// 0xffffffff
		}
		/* create data table */
		if (p_cti_h15a->reg_data.addr == NULL)
		{
			p_cti_h15a->reg_data.addr = (UINT32 *)OS_KMalloc(sizeof(PE_CTI_HW_PARAM_REG_H15A0_T));
			memset((p_cti_h15a->reg_data.addr), 0, sizeof(PE_CTI_HW_PARAM_REG_H15A0_T));	// 0x0
		}
		/* report result */
		if (p_cti_h15a->reg_dflt.addr != NULL && p_cti_h15a->reg_user.addr != NULL && \
			p_cti_h15a->reg_mask.addr != NULL && p_cti_h15a->reg_data.addr != NULL)
		{
			PE_PRINT_NOTI("[%s]table ready.\n", PE_SHP_HW_H15A_TBL_IDX_TO_STR(index));
			ret = RET_OK;
		}
		else
		{
			PE_PRINT_NOTI("[%s]table not ready. dflt,user,mask,data:%s,%s,%s,%s\n", \
				PE_SHP_HW_H15A_TBL_IDX_TO_STR(index), \
				(p_cti_h15a->reg_dflt.addr == NULL)? "x":"o", \
				(p_cti_h15a->reg_user.addr == NULL)? "x":"o", \
				(p_cti_h15a->reg_mask.addr == NULL)? "x":"o", \
				(p_cti_h15a->reg_data.addr == NULL)? "x":"o");
			ret = RET_ERROR;
		}
	}
	else if (index == PE_SHP_HW_H15A_TBL_IDX_PBL)
	{
		/* create dflt table */
		if (p_pbl_h15a->reg_dflt.addr == NULL)
		{
			p_pbl_h15a->reg_dflt.addr = (UINT32 *)OS_KMalloc(sizeof(PE_PBL_HW_PARAM_REG_H15A0_T));
		}
		/* create user table */
		if (p_pbl_h15a->reg_user.addr == NULL)
		{
			p_pbl_h15a->reg_user.addr = (UINT32 *)OS_KMalloc(sizeof(PE_PBL_HW_PARAM_REG_H15A0_T));
			memset((p_pbl_h15a->reg_user.addr), 0, sizeof(PE_PBL_HW_PARAM_REG_H15A0_T));	// 0x0
		}
		/* create mask table */
		if (p_pbl_h15a->reg_mask.addr == NULL)
		{
			p_pbl_h15a->reg_mask.addr = (UINT32 *)OS_KMalloc(sizeof(PE_PBL_HW_PARAM_REG_H15A0_T));
			memset((p_pbl_h15a->reg_mask.addr), -1, sizeof(PE_PBL_HW_PARAM_REG_H15A0_T));// 0xffffffff
		}
		/* create data table */
		if (p_pbl_h15a->reg_data.addr == NULL)
		{
			p_pbl_h15a->reg_data.addr = (UINT32 *)OS_KMalloc(sizeof(PE_PBL_HW_PARAM_REG_H15A0_T));
			memset((p_pbl_h15a->reg_data.addr), 0, sizeof(PE_PBL_HW_PARAM_REG_H15A0_T));	// 0x0
		}
		/* report result */
		if (p_pbl_h15a->reg_dflt.addr != NULL && p_pbl_h15a->reg_user.addr != NULL && \
			p_pbl_h15a->reg_mask.addr != NULL && p_pbl_h15a->reg_data.addr != NULL)
		{
			PE_PRINT_NOTI("[%s]table ready.\n", PE_SHP_HW_H15A_TBL_IDX_TO_STR(index));
			ret = RET_OK;
		}
		else
		{
			PE_PRINT_NOTI("[%s]table not ready. dflt,user,mask,data:%s,%s,%s,%s\n", \
				PE_SHP_HW_H15A_TBL_IDX_TO_STR(index), \
				(p_pbl_h15a->reg_dflt.addr == NULL)? "x":"o", \
				(p_pbl_h15a->reg_user.addr == NULL)? "x":"o", \
				(p_pbl_h15a->reg_mask.addr == NULL)? "x":"o", \
				(p_pbl_h15a->reg_data.addr == NULL)? "x":"o");
			ret = RET_ERROR;
		}
	}
	#ifdef PE_SHP_HW_H15_SRE_SHP_WR_ONLY_MD
	else if (index == PE_SHP_HW_H15A_TBL_IDX_SRS)
	{
		/* create dflt table */
		if (p_srs_h15a->reg_dflt.addr == NULL)
		{
			p_srs_h15a->reg_dflt.addr = (UINT32 *)OS_KMalloc(sizeof(PE_SRS_HW_PARAM_REG_H15A0_T));
		}
		/* create user table */
		if (p_srs_h15a->reg_user.addr == NULL)
		{
			p_srs_h15a->reg_user.addr = (UINT32 *)OS_KMalloc(sizeof(PE_SRS_HW_PARAM_REG_H15A0_T));
			memset((p_srs_h15a->reg_user.addr), 0, sizeof(PE_SRS_HW_PARAM_REG_H15A0_T));	// 0x0
		}
		/* create mask table */
		if (p_srs_h15a->reg_mask.addr == NULL)
		{
			p_srs_h15a->reg_mask.addr = (UINT32 *)OS_KMalloc(sizeof(PE_SRS_HW_PARAM_REG_H15A0_T));
			memset((p_srs_h15a->reg_mask.addr), -1, sizeof(PE_SRS_HW_PARAM_REG_H15A0_T));// 0xffffffff
		}
		/* create data table */
		if (p_srs_h15a->reg_data.addr == NULL)
		{
			p_srs_h15a->reg_data.addr = (UINT32 *)OS_KMalloc(sizeof(PE_SRS_HW_PARAM_REG_H15A0_T));
			memset((p_srs_h15a->reg_data.addr), 0, sizeof(PE_SRS_HW_PARAM_REG_H15A0_T));	// 0x0
		}
		/* report srsult */
		if (p_srs_h15a->reg_dflt.addr != NULL && p_srs_h15a->reg_user.addr != NULL && \
			p_srs_h15a->reg_mask.addr != NULL && p_srs_h15a->reg_data.addr != NULL)
		{
			PE_PRINT_NOTI("[%s]table ready.\n", PE_SHP_HW_H15A_TBL_IDX_TO_STR(index));
			ret = RET_OK;
		}
		else
		{
			PE_PRINT_NOTI("[%s]table not ready. dflt,user,mask,data:%s,%s,%s,%s\n", \
				PE_SHP_HW_H15A_TBL_IDX_TO_STR(index), \
				(p_srs_h15a->reg_dflt.addr == NULL)? "x":"o", \
				(p_srs_h15a->reg_user.addr == NULL)? "x":"o", \
				(p_srs_h15a->reg_mask.addr == NULL)? "x":"o", \
				(p_srs_h15a->reg_data.addr == NULL)? "x":"o");
			ret = RET_ERROR;
		}
	}
	#endif	//#ifdef PE_SHP_HW_H15_SRE_SHP_WR_ONLY_MD
	else
	{
		PE_SHP_HW_H15_DBG_PRINT("nothing to do\n");	ret = RET_OK;
	}
	return ret;
}
#endif
/**
 * set debug settings
 *
 * @param   *pstParams [in] LX_PE_DBG_SETTINGS_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_SHP_HW_H15_SetDebugSettings(LX_PE_DBG_SETTINGS_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		PE_SHP_HW_H15_DBG_PRINT("wid:%d,type:0x%x,[0x%x]print:0x%x,bypass:0x%x\n",\
			pstParams->win_id,pstParams->type,(0x1<<PE_ITEM_PKTMODL(SHP)),\
			pstParams->print_lvl,pstParams->bypass);
		/* set print level */
		if(pstParams->type&LX_PE_DBG_LV)
		{
			_g_shp_hw_h15_trace = (pstParams->print_lvl & (0x1<<PE_ITEM_PKTMODL(SHP)))? 0x1:0x0;
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
int PE_SHP_HW_H15_SetDefault(LX_PE_DEFAULT_SETTINGS_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H15)
		{
			ret = PE_SHP_HW_H15A_SetDefault(pstParams);
			PE_SHP_HW_H15_CHECK_CODE(ret, break, \
				"[%s,%d] PE_SHP_HW_H15A_SetDefault() error.\n", __F__, __L__);
		}
		else
		{
			PE_SHP_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}

/**
 * default shp setting(for H15AX)
 *
 * @param   *pstParams [in] LX_PE_DEFAULT_SETTINGS_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_SHP_HW_H15A_SetDefault(LX_PE_DEFAULT_SETTINGS_T *pstParams)
{
	int ret = RET_OK;
	PE_INF_H15_SETTINGS_T inf_set;
	LX_PE_INF_DISPLAY_T *disp0_inf = NULL;
	LX_PE_INF_DISPLAY_T *disp1_inf = NULL;
	PE_SHP_HW_H15A_RES_FMT cur0_res_fmt;
	PE_SHP_HW_H15A_SRE_FMT cur0_sre_fmt;
	PE_SHP_HW_H15A_PBL_FMT cur0_pbl_fmt;
	PE_SHP_HW_H15A_CTI_FMT cur0_cti_fmt;
	#ifdef PE_SHP_HW_H15_SRE_SHP_WR_ONLY_MD
	PE_SHP_HW_H15A_SRS_FMT cur0_srs_fmt;
	#endif
	PE_SHP_HW_H15_SETTINGS_T *pInfo=&_g_pe_shp_hw_h15_info;
	#ifdef PE_SHP_HW_H15_SR_PWD_TRACE
	UINT32 r_data[2];
	#endif
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		ret = PE_INF_H15_GetCurInfSettings(&inf_set);
		PE_SHP_HW_H15_CHECK_CODE(ret, break, \
			"[%s,%d] PE_INF_H15_GetCurInfSettings() error.\n", __F__, __L__);
		disp0_inf=&inf_set.disp_info[LX_PE_WIN_0];
		disp1_inf=&inf_set.disp_info[LX_PE_WIN_1];
		CHECK_KNULL(disp0_inf);
		CHECK_KNULL(disp1_inf);
		/* res(pe1 shp l,r) */
		if(PE_CHECK_WIN0(pstParams->win_id))
		{
			cur0_res_fmt = PE_SHP_HW_H15A_ConvDispInfoToResFmt(disp0_inf);
			PE_SHP_HW_H15A_DBG_PRINT_RES_FMT_STATE(pInfo->res0a_fmt,cur0_res_fmt);
			if(pInfo->res0a_fmt!=cur0_res_fmt)
			{
				#ifdef PE_HW_H15_PROTECT_OVERLAP_SETTINGS
				switch(cur0_res_fmt)
				{
					case PE_SHP_H15A_RES_AV_480:
						PE_SHP_HW_H15A_RES_REG_TABLE(shp_l_atv_480_default_h15a0,   PE_H15A_REG_WRITE_BASE);
						PE_SHP_HW_H15A_RES_REG_TABLE(shp_l_atv_480_default_h15a0,   PE_H15A_P1_SHP_LR_DIFF);
						#ifndef PE_SHP_HW_H15_SRE_SHP_WR_ONLY_MD
						PE_SET_REG_TABLE(SHP_HW_H15,sre_res_atv_480_default_h15a0,  PE_H15A_REG_WRITE_BASE);
						#endif
						break;
					case PE_SHP_H15A_RES_AV_576:
						PE_SHP_HW_H15A_RES_REG_TABLE(shp_l_atv_576_default_h15a0,   PE_H15A_REG_WRITE_BASE);
						PE_SHP_HW_H15A_RES_REG_TABLE(shp_l_atv_576_default_h15a0,   PE_H15A_P1_SHP_LR_DIFF);
						#ifndef PE_SHP_HW_H15_SRE_SHP_WR_ONLY_MD
						PE_SET_REG_TABLE(SHP_HW_H15,sre_res_atv_576_default_h15a0,  PE_H15A_REG_WRITE_BASE);
						#endif
						break;
					case PE_SHP_H15A_RES_DTV_SD:
						PE_SHP_HW_H15A_RES_REG_TABLE(shp_l_dtv_sd_default_h15a0,   PE_H15A_REG_WRITE_BASE);
						PE_SHP_HW_H15A_RES_REG_TABLE(shp_l_dtv_sd_default_h15a0,   PE_H15A_P1_SHP_LR_DIFF);
						#ifndef PE_SHP_HW_H15_SRE_SHP_WR_ONLY_MD
						PE_SET_REG_TABLE(SHP_HW_H15,sre_res_dtv_sd_default_h15a0,  PE_H15A_REG_WRITE_BASE);
						#endif
						break;
					case PE_SHP_H15A_RES_SD:
						PE_SHP_HW_H15A_RES_REG_TABLE(shp_l_sd_default_h15a0,   PE_H15A_REG_WRITE_BASE);
						PE_SHP_HW_H15A_RES_REG_TABLE(shp_l_sd_default_h15a0,   PE_H15A_P1_SHP_LR_DIFF);
						#ifndef PE_SHP_HW_H15_SRE_SHP_WR_ONLY_MD
						PE_SET_REG_TABLE(SHP_HW_H15,sre_res_sd_default_h15a0,  PE_H15A_REG_WRITE_BASE);
						#endif
						break;
					case PE_SHP_H15A_RES_HD:
						PE_SHP_HW_H15A_RES_REG_TABLE(shp_l_hd_default_h15a0,   PE_H15A_REG_WRITE_BASE);
						PE_SHP_HW_H15A_RES_REG_TABLE(shp_l_hd_default_h15a0,   PE_H15A_P1_SHP_LR_DIFF);
						#ifndef PE_SHP_HW_H15_SRE_SHP_WR_ONLY_MD
						PE_SET_REG_TABLE(SHP_HW_H15,sre_res_hd_default_h15a0,  PE_H15A_REG_WRITE_BASE);
						#endif
						break;
					case PE_SHP_H15A_RES_HDMI_HD:
						PE_SHP_HW_H15A_RES_REG_TABLE(shp_l_hdmi_hd_default_h15a0,   PE_H15A_REG_WRITE_BASE);
						PE_SHP_HW_H15A_RES_REG_TABLE(shp_l_hdmi_hd_default_h15a0,   PE_H15A_P1_SHP_LR_DIFF);
						#ifndef PE_SHP_HW_H15_SRE_SHP_WR_ONLY_MD
						PE_SET_REG_TABLE(SHP_HW_H15,sre_res_hdmi_hd_default_h15a0,  PE_H15A_REG_WRITE_BASE);
						#endif
						break;
					case PE_SHP_H15A_RES_UHD:
						PE_SHP_HW_H15A_RES_REG_TABLE(shp_l_uhd_default_h15a0,   PE_H15A_REG_WRITE_BASE);
						PE_SHP_HW_H15A_RES_REG_TABLE(shp_l_uhd_default_h15a0,   PE_H15A_P1_SHP_LR_DIFF);
						break;
					case PE_SHP_H15A_RES_PC:
						PE_SHP_HW_H15A_RES_REG_TABLE(shp_l_pc_default_h15a0,   PE_H15A_REG_WRITE_BASE);
						PE_SHP_HW_H15A_RES_REG_TABLE(shp_l_pc_default_h15a0,   PE_H15A_P1_SHP_LR_DIFF);
						#ifndef PE_SHP_HW_H15_SRE_SHP_WR_ONLY_MD
						PE_SET_REG_TABLE(SHP_HW_H15,sre_res_pc_default_h15a0,  PE_H15A_REG_WRITE_BASE);
						#endif
						break;
					default:
						break;
				}
				#else
				switch(cur0_res_fmt)
				{
					case PE_SHP_H15A_RES_AV_480:
						PE_SET_REG_TABLE(SHP_HW_H15,shp_l_atv_480_default_h15a0,   PE_H15A_REG_WRITE_BASE);
						PE_SET_REG_TABLE(SHP_HW_H15,shp_l_atv_480_default_h15a0,   PE_H15A_P1_SHP_LR_DIFF);
						#ifndef PE_SHP_HW_H15_SRE_SHP_WR_ONLY_MD
						PE_SET_REG_TABLE(SHP_HW_H15,sre_res_atv_480_default_h15a0,  PE_H15A_REG_WRITE_BASE);
						#endif
						break;
					case PE_SHP_H15A_RES_AV_576:
						PE_SET_REG_TABLE(SHP_HW_H15,shp_l_atv_576_default_h15a0,   PE_H15A_REG_WRITE_BASE);
						PE_SET_REG_TABLE(SHP_HW_H15,shp_l_atv_576_default_h15a0,   PE_H15A_P1_SHP_LR_DIFF);
						#ifndef PE_SHP_HW_H15_SRE_SHP_WR_ONLY_MD
						PE_SET_REG_TABLE(SHP_HW_H15,sre_res_atv_576_default_h15a0,  PE_H15A_REG_WRITE_BASE);
						#endif
						break;
					case PE_SHP_H15A_RES_DTV_SD:
						PE_SET_REG_TABLE(SHP_HW_H15,shp_l_dtv_sd_default_h15a0,   PE_H15A_REG_WRITE_BASE);
						PE_SET_REG_TABLE(SHP_HW_H15,shp_l_dtv_sd_default_h15a0,   PE_H15A_P1_SHP_LR_DIFF);
						#ifndef PE_SHP_HW_H15_SRE_SHP_WR_ONLY_MD
						PE_SET_REG_TABLE(SHP_HW_H15,sre_res_dtv_sd_default_h15a0,  PE_H15A_REG_WRITE_BASE);
						#endif
						break;
					case PE_SHP_H15A_RES_SD:
						PE_SET_REG_TABLE(SHP_HW_H15,shp_l_sd_default_h15a0,    PE_H15A_REG_WRITE_BASE);
						PE_SET_REG_TABLE(SHP_HW_H15,shp_l_sd_default_h15a0,    PE_H15A_P1_SHP_LR_DIFF);
						#ifndef PE_SHP_HW_H15_SRE_SHP_WR_ONLY_MD
						PE_SET_REG_TABLE(SHP_HW_H15,sre_res_sd_default_h15a0,  PE_H15A_REG_WRITE_BASE);
						#endif
						break;
					case PE_SHP_H15A_RES_HD:
						PE_SET_REG_TABLE(SHP_HW_H15,shp_l_hd_default_h15a0,    PE_H15A_REG_WRITE_BASE);
						PE_SET_REG_TABLE(SHP_HW_H15,shp_l_hd_default_h15a0,    PE_H15A_P1_SHP_LR_DIFF);
						#ifndef PE_SHP_HW_H15_SRE_SHP_WR_ONLY_MD
						PE_SET_REG_TABLE(SHP_HW_H15,sre_res_hd_default_h15a0,  PE_H15A_REG_WRITE_BASE);
						#endif
						break;
					case PE_SHP_H15A_RES_HDMI_HD:
						PE_SET_REG_TABLE(SHP_HW_H15,shp_l_hdmi_hd_default_h15a0,   PE_H15A_REG_WRITE_BASE);
						PE_SET_REG_TABLE(SHP_HW_H15,shp_l_hdmi_hd_default_h15a0,   PE_H15A_P1_SHP_LR_DIFF);
						#ifndef PE_SHP_HW_H15_SRE_SHP_WR_ONLY_MD
						PE_SET_REG_TABLE(SHP_HW_H15,sre_res_hdmi_hd_default_h15a0, PE_H15A_REG_WRITE_BASE);
						#endif
						break;
					case PE_SHP_H15A_RES_HDMI_HD:
						PE_SET_REG_TABLE(SHP_HW_H15,shp_l_hdmi_hd_default_h15a0,    PE_H15A_REG_WRITE_BASE);
						PE_SET_REG_TABLE(SHP_HW_H15,shp_l_hdmi_hd_default_h15a0,    PE_H15A_P1_SHP_LR_DIFF);
						#ifndef PE_SHP_HW_H15_SRE_SHP_WR_ONLY_MD
						PE_SET_REG_TABLE(SHP_HW_H15,sre_res_hdmi_hd_default_h15a0,  PE_H15A_REG_WRITE_BASE);
						#endif
						break;
					case PE_SHP_H15A_RES_UHD:
						PE_SET_REG_TABLE(SHP_HW_H15,shp_l_uhd_default_h15a0,   PE_H15A_REG_WRITE_BASE);
						PE_SET_REG_TABLE(SHP_HW_H15,shp_l_uhd_default_h15a0,   PE_H15A_P1_SHP_LR_DIFF);
						break;
					case PE_SHP_H15A_RES_PC:
						PE_SET_REG_TABLE(SHP_HW_H15,shp_l_pc_default_h15a0,    PE_H15A_REG_WRITE_BASE);
						PE_SET_REG_TABLE(SHP_HW_H15,shp_l_pc_default_h15a0,    PE_H15A_P1_SHP_LR_DIFF);
						#ifndef PE_SHP_HW_H15_SRE_SHP_WR_ONLY_MD
						PE_SET_REG_TABLE(SHP_HW_H15,sre_res_pc_default_h15a0,  PE_H15A_REG_WRITE_BASE);
						#endif
						break;
					default:
						break;
				}
				#endif
				pInfo->res0a_fmt=cur0_res_fmt;
			}
		}
		#ifdef PE_SHP_HW_H15_SRE_SHP_WR_ONLY_MD
		/* sre shp(srs) */
		if(PE_CHECK_WIN0(pstParams->win_id))
		{
			cur0_srs_fmt = PE_SHP_HW_H15A_ConvDispInfoToSrsFmt(disp0_inf);
			PE_SHP_HW_H15A_DBG_PRINT_SRS_FMT_STATE(pInfo->srs0a_fmt,cur0_srs_fmt);
			#ifdef PE_SHP_HW_H15_SR_PWD_TRACE
			CTOP_CTRL_H15A0_RdFL(SR,ctr00);
			CTOP_CTRL_H15A0_Rd01(SR,ctr00,cg_sr_en,r_data[0]);
			CTOP_CTRL_H15A0_Rd01(SR,ctr00,cg_shs_en,r_data[1]);
			PE_SHP_HW_H15_DBG_SRPD_TRACE("S cg sr,shs:%d,%d(%d->%d)\n",r_data[0],r_data[1],pInfo->srs0a_fmt,cur0_srs_fmt);
			#endif
			if(pInfo->srs0a_fmt!=cur0_srs_fmt)
			{
				switch(cur0_srs_fmt)
				{
					case PE_SHP_H15A_SRS_AV_480:
						PE_SHP_HW_H15A_SRS_REG_TABLE(sre_res_atv_480_default_h15a0,  PE_H15A_REG_WRITE_BASE);
						break;
					case PE_SHP_H15A_SRS_AV_576:
						PE_SHP_HW_H15A_SRS_REG_TABLE(sre_res_atv_576_default_h15a0,  PE_H15A_REG_WRITE_BASE);
						break;
					case PE_SHP_H15A_SRS_DTV_SD:
						PE_SHP_HW_H15A_SRS_REG_TABLE(sre_res_dtv_sd_default_h15a0,   PE_H15A_REG_WRITE_BASE);
						break;
					case PE_SHP_H15A_SRS_SD:
						PE_SHP_HW_H15A_SRS_REG_TABLE(sre_res_sd_default_h15a0,       PE_H15A_REG_WRITE_BASE);
						break;
					case PE_SHP_H15A_SRS_HD:
						PE_SHP_HW_H15A_SRS_REG_TABLE(sre_res_hd_default_h15a0,       PE_H15A_REG_WRITE_BASE);
						break;
					case PE_SHP_H15A_SRS_HDMI_HD:
						PE_SHP_HW_H15A_SRS_REG_TABLE(sre_res_hdmi_hd_default_h15a0,  PE_H15A_REG_WRITE_BASE);
						break;
					case PE_SHP_H15A_SRS_PC:
						PE_SHP_HW_H15A_SRS_REG_TABLE(sre_res_pc_default_h15a0,       PE_H15A_REG_WRITE_BASE);
						break;
					default:
						break;
				}
				pInfo->srs0a_fmt=cur0_srs_fmt;
			}
			#ifdef PE_SHP_HW_H15_SR_PWD_TRACE
			CTOP_CTRL_H15A0_RdFL(SR,ctr00);
			CTOP_CTRL_H15A0_Rd01(SR,ctr00,cg_sr_en,r_data[0]);
			CTOP_CTRL_H15A0_Rd01(SR,ctr00,cg_shs_en,r_data[1]);
			PE_SHP_HW_H15_DBG_SRPD_TRACE("E cg sr,shs:%d,%d(%d->%d)\n",r_data[0],r_data[1],pInfo->srs0a_fmt,cur0_srs_fmt);
			#endif
		}
		#endif	//#ifdef PE_SHP_HW_H15_SRE_SHP_WR_ONLY_MD
		/* sre */
		if(PE_CHECK_WIN0(pstParams->win_id))
		{
			cur0_sre_fmt = PE_SHP_HW_H15A_ConvDispInfoToSreFmt(disp0_inf);
			PE_SHP_HW_H15A_DBG_PRINT_SRE_FMT_STATE(pInfo->sre0a_fmt,cur0_sre_fmt);
			#ifdef PE_SHP_HW_H15_SR_PWD_TRACE
			CTOP_CTRL_H15A0_RdFL(SR,ctr00);
			CTOP_CTRL_H15A0_Rd01(SR,ctr00,cg_sr_en,r_data[0]);
			CTOP_CTRL_H15A0_Rd01(SR,ctr00,cg_shs_en,r_data[1]);
			PE_SHP_HW_H15_DBG_SRPD_TRACE("S cg sr,shs:%d,%d(%d->%d)\n",r_data[0],r_data[1],pInfo->sre0a_fmt,cur0_sre_fmt);
			#endif
			if(pInfo->sre0a_fmt!=cur0_sre_fmt)
			{
				switch(cur0_sre_fmt)
				{
					case PE_SHP_H15A_SRE_SD:
						PE_SET_REG_TABLE(SHP_HW_H15,sre_sd_default_h15a0,    PE_H15A_REG_WRITE_BASE);
						break;
					case PE_SHP_H15A_SRE_HD:
						PE_SET_REG_TABLE(SHP_HW_H15,sre_hd_default_h15a0,    PE_H15A_REG_WRITE_BASE);
						break;
					case PE_SHP_H15A_SRE_COMP_HD:
						PE_SET_REG_TABLE(SHP_HW_H15,sre_comp_hd_default_h15a0,    PE_H15A_REG_WRITE_BASE);
						break;
					default:
						break;
				}
				pInfo->sre0a_fmt=cur0_sre_fmt;
			}
			#ifdef PE_SHP_HW_H15_SR_PWD_TRACE
			CTOP_CTRL_H15A0_RdFL(SR,ctr00);
			CTOP_CTRL_H15A0_Rd01(SR,ctr00,cg_sr_en,r_data[0]);
			CTOP_CTRL_H15A0_Rd01(SR,ctr00,cg_shs_en,r_data[1]);
			PE_SHP_HW_H15_DBG_SRPD_TRACE("E cg sr,shs:%d,%d(%d->%d)\n",r_data[0],r_data[1],pInfo->sre0a_fmt,cur0_sre_fmt);
			#endif
		}
		/* sre lut */
		/* pbl(pre_blur_dnr) */
		if(PE_CHECK_WIN0(pstParams->win_id))
		{
			cur0_pbl_fmt = PE_SHP_HW_H15A_ConvDispInfoToPblFmt(disp0_inf);
			PE_SHP_HW_H15A_DBG_PRINT_PBL_FMT_STATE(pInfo->pbl0a_fmt,cur0_pbl_fmt);
			if(pInfo->pbl0a_fmt!=cur0_pbl_fmt)
			{
				#ifdef PE_HW_H15_PROTECT_OVERLAP_SETTINGS
				switch(cur0_pbl_fmt)
				{
					case PE_SHP_H15A_PBL_ATV_NT:
						PE_SHP_HW_H15A_PBL_REG_TABLE(pblur_dnr_l_atv_nt_default_h15a0, PE_H15A_REG_WRITE_BASE);
						PE_SHP_HW_H15A_PBL_REG_TABLE(pblur_dnr_l_atv_nt_default_h15a0, PE_H15A_ND_DNR_12_DIFF);
						break;
					case PE_SHP_H15A_PBL_ATV_PAL:
						PE_SHP_HW_H15A_PBL_REG_TABLE(pblur_dnr_l_atv_pal_default_h15a0,PE_H15A_REG_WRITE_BASE);
						PE_SHP_HW_H15A_PBL_REG_TABLE(pblur_dnr_l_atv_pal_default_h15a0,PE_H15A_ND_DNR_12_DIFF);
						break;
					case PE_SHP_H15A_PBL_AV:
						PE_SHP_HW_H15A_PBL_REG_TABLE(pblur_dnr_l_av_default_h15a0,     PE_H15A_REG_WRITE_BASE);
						PE_SHP_HW_H15A_PBL_REG_TABLE(pblur_dnr_l_av_default_h15a0,     PE_H15A_ND_DNR_12_DIFF);
						break;
					case PE_SHP_H15A_PBL_SECAM:
						PE_SHP_HW_H15A_PBL_REG_TABLE(pblur_dnr_l_secam_default_h15a0,  PE_H15A_REG_WRITE_BASE);
						PE_SHP_HW_H15A_PBL_REG_TABLE(pblur_dnr_l_secam_default_h15a0,  PE_H15A_ND_DNR_12_DIFF);
						break;
					case PE_SHP_H15A_PBL_COMP_SD:
						PE_SHP_HW_H15A_PBL_REG_TABLE(pblur_dnr_l_comp_sd_default_h15a0,PE_H15A_REG_WRITE_BASE);
						PE_SHP_HW_H15A_PBL_REG_TABLE(pblur_dnr_l_comp_sd_default_h15a0,PE_H15A_ND_DNR_12_DIFF);
						break;
					case PE_SHP_H15A_PBL_DTV_SD:
						PE_SHP_HW_H15A_PBL_REG_TABLE(pblur_dnr_l_dtv_sd_default_h15a0, PE_H15A_REG_WRITE_BASE);
						PE_SHP_HW_H15A_PBL_REG_TABLE(pblur_dnr_l_dtv_sd_default_h15a0, PE_H15A_ND_DNR_12_DIFF);
						break;
					case PE_SHP_H15A_PBL_HDMI_SD:
						PE_SHP_HW_H15A_PBL_REG_TABLE(pblur_dnr_l_hdmi_sd_default_h15a0,PE_H15A_REG_WRITE_BASE);
						PE_SHP_HW_H15A_PBL_REG_TABLE(pblur_dnr_l_hdmi_sd_default_h15a0,PE_H15A_ND_DNR_12_DIFF);
						break;
					case PE_SHP_H15A_PBL_HD:
						PE_SHP_HW_H15A_PBL_REG_TABLE(pblur_dnr_l_hd_default_h15a0,     PE_H15A_REG_WRITE_BASE);
						PE_SHP_HW_H15A_PBL_REG_TABLE(pblur_dnr_l_hd_default_h15a0,     PE_H15A_ND_DNR_12_DIFF);
						break;
					case PE_SHP_H15A_PBL_PC:
						PE_SHP_HW_H15A_PBL_REG_TABLE(pblur_dnr_l_pc_default_h15a0,     PE_H15A_REG_WRITE_BASE);
						PE_SHP_HW_H15A_PBL_REG_TABLE(pblur_dnr_l_pc_default_h15a0,     PE_H15A_ND_DNR_12_DIFF);
						break;
					default:
						break;
				}
				#else
				switch(cur0_pbl_fmt)
				{
					case PE_SHP_H15A_PBL_ATV_NT:
						PE_SET_REG_TABLE(SHP_HW_H15,pblur_dnr_l_atv_nt_default_h15a0, PE_H15A_REG_WRITE_BASE);
						PE_SET_REG_TABLE(SHP_HW_H15,pblur_dnr_l_atv_nt_default_h15a0, PE_H15A_ND_DNR_12_DIFF);
						break;
					case PE_SHP_H15A_PBL_ATV_PAL:
						PE_SET_REG_TABLE(SHP_HW_H15,pblur_dnr_l_atv_pal_default_h15a0,PE_H15A_REG_WRITE_BASE);
						PE_SET_REG_TABLE(SHP_HW_H15,pblur_dnr_l_atv_pal_default_h15a0,PE_H15A_ND_DNR_12_DIFF);
						break;
					case PE_SHP_H15A_PBL_AV:
						PE_SET_REG_TABLE(SHP_HW_H15,pblur_dnr_l_av_default_h15a0,     PE_H15A_REG_WRITE_BASE);
						PE_SET_REG_TABLE(SHP_HW_H15,pblur_dnr_l_av_default_h15a0,     PE_H15A_ND_DNR_12_DIFF);
						break;
					case PE_SHP_H15A_PBL_SECAM:
						PE_SET_REG_TABLE(SHP_HW_H15,pblur_dnr_l_secam_default_h15a0,  PE_H15A_REG_WRITE_BASE);
						PE_SET_REG_TABLE(SHP_HW_H15,pblur_dnr_l_secam_default_h15a0,  PE_H15A_ND_DNR_12_DIFF);
						break;
					case PE_SHP_H15A_PBL_COMP_SD:
						PE_SET_REG_TABLE(SHP_HW_H15,pblur_dnr_l_comp_sd_default_h15a0,PE_H15A_REG_WRITE_BASE);
						PE_SET_REG_TABLE(SHP_HW_H15,pblur_dnr_l_comp_sd_default_h15a0,PE_H15A_ND_DNR_12_DIFF);
						break;
					case PE_SHP_H15A_PBL_DTV_SD:
						PE_SET_REG_TABLE(SHP_HW_H15,pblur_dnr_l_dtv_sd_default_h15a0, PE_H15A_REG_WRITE_BASE);
						PE_SET_REG_TABLE(SHP_HW_H15,pblur_dnr_l_dtv_sd_default_h15a0, PE_H15A_ND_DNR_12_DIFF);
						break;
					case PE_SHP_H15A_PBL_HDMI_SD:
						PE_SET_REG_TABLE(SHP_HW_H15,pblur_dnr_l_hdmi_sd_default_h15a0,PE_H15A_REG_WRITE_BASE);
						PE_SET_REG_TABLE(SHP_HW_H15,pblur_dnr_l_hdmi_sd_default_h15a0,PE_H15A_ND_DNR_12_DIFF);
						break;
					case PE_SHP_H15A_PBL_HD:
						PE_SET_REG_TABLE(SHP_HW_H15,pblur_dnr_l_hd_default_h15a0,     PE_H15A_REG_WRITE_BASE);
						PE_SET_REG_TABLE(SHP_HW_H15,pblur_dnr_l_hd_default_h15a0,     PE_H15A_ND_DNR_12_DIFF);
						break;
					case PE_SHP_H15A_PBL_PC:
						PE_SET_REG_TABLE(SHP_HW_H15,pblur_dnr_l_pc_default_h15a0,     PE_H15A_REG_WRITE_BASE);
						PE_SET_REG_TABLE(SHP_HW_H15,pblur_dnr_l_pc_default_h15a0,     PE_H15A_ND_DNR_12_DIFF);
						break;
					default:
						break;
				}
				#endif
				pInfo->pbl0a_fmt=cur0_pbl_fmt;
			}
		}
		/* cti(dnr) */
		if(PE_CHECK_WIN0(pstParams->win_id))
		{
			cur0_cti_fmt = PE_SHP_HW_H15A_ConvDispInfoToCtiFmt(disp0_inf);
			PE_SHP_HW_H15A_DBG_PRINT_CTI_FMT_STATE(pInfo->cti0a_fmt,cur0_cti_fmt);
			if(pInfo->cti0a_fmt!=cur0_cti_fmt)
			{
				#ifdef PE_HW_H15_PROTECT_OVERLAP_SETTINGS
				switch(cur0_cti_fmt)
				{
					case PE_SHP_H15A_CTI_ATV_NT:
						PE_SHP_HW_H15A_CTI_REG_TABLE(cti_dnr_l_atv_ntsc_default_h15a0,PE_H15A_REG_WRITE_BASE);
						PE_SHP_HW_H15A_CTI_REG_TABLE(cti_dnr_l_atv_ntsc_default_h15a0,PE_H15A_ND_DNR_12_DIFF);
						break;
					case PE_SHP_H15A_CTI_ATV_PAL:
						PE_SHP_HW_H15A_CTI_REG_TABLE(cti_dnr_l_atv_pal_default_h15a0, PE_H15A_REG_WRITE_BASE);
						PE_SHP_HW_H15A_CTI_REG_TABLE(cti_dnr_l_atv_pal_default_h15a0, PE_H15A_ND_DNR_12_DIFF);
						break;
					case PE_SHP_H15A_CTI_HD:
						PE_SHP_HW_H15A_CTI_REG_TABLE(cti_dnr_l_hd_default_h15a0,      PE_H15A_REG_WRITE_BASE);
						PE_SHP_HW_H15A_CTI_REG_TABLE(cti_dnr_l_hd_default_h15a0,      PE_H15A_ND_DNR_12_DIFF);
						break;
					case PE_SHP_H15A_CTI_PC:
						PE_SHP_HW_H15A_CTI_REG_TABLE(cti_dnr_l_pc_default_h15a0,      PE_H15A_REG_WRITE_BASE);
						PE_SHP_HW_H15A_CTI_REG_TABLE(cti_dnr_l_pc_default_h15a0,      PE_H15A_ND_DNR_12_DIFF);
						break;
					case PE_SHP_H15A_CTI_SD:
						PE_SHP_HW_H15A_CTI_REG_TABLE(cti_dnr_l_sd_default_h15a0,      PE_H15A_REG_WRITE_BASE);
						PE_SHP_HW_H15A_CTI_REG_TABLE(cti_dnr_l_sd_default_h15a0,      PE_H15A_ND_DNR_12_DIFF);
						break;
					default:
						break;
				}
				#else
				switch(cur0_cti_fmt)
				{
					case PE_SHP_H15A_CTI_ATV_NT:
						PE_SET_REG_TABLE(SHP_HW_H15,cti_dnr_l_atv_ntsc_default_h15a0,PE_H15A_REG_WRITE_BASE);
						PE_SET_REG_TABLE(SHP_HW_H15,cti_dnr_l_atv_ntsc_default_h15a0,PE_H15A_ND_DNR_12_DIFF);
						break;
					case PE_SHP_H15A_CTI_ATV_PAL:
						PE_SET_REG_TABLE(SHP_HW_H15,cti_dnr_l_atv_pal_default_h15a0, PE_H15A_REG_WRITE_BASE);
						PE_SET_REG_TABLE(SHP_HW_H15,cti_dnr_l_atv_pal_default_h15a0, PE_H15A_ND_DNR_12_DIFF);
						break;
					case PE_SHP_H15A_CTI_HD:
						PE_SET_REG_TABLE(SHP_HW_H15,cti_dnr_l_hd_default_h15a0,      PE_H15A_REG_WRITE_BASE);
						PE_SET_REG_TABLE(SHP_HW_H15,cti_dnr_l_hd_default_h15a0,      PE_H15A_ND_DNR_12_DIFF);
						break;
					case PE_SHP_H15A_CTI_PC:
						PE_SET_REG_TABLE(SHP_HW_H15,cti_dnr_l_pc_default_h15a0,      PE_H15A_REG_WRITE_BASE);
						PE_SET_REG_TABLE(SHP_HW_H15,cti_dnr_l_pc_default_h15a0,      PE_H15A_ND_DNR_12_DIFF);
						break;
					case PE_SHP_H15A_CTI_SD:
						PE_SET_REG_TABLE(SHP_HW_H15,cti_dnr_l_sd_default_h15a0,      PE_H15A_REG_WRITE_BASE);
						PE_SET_REG_TABLE(SHP_HW_H15,cti_dnr_l_sd_default_h15a0,      PE_H15A_ND_DNR_12_DIFF);
						break;
					default:
						break;
				}
				#endif
				pInfo->cti0a_fmt=cur0_cti_fmt;
			}
		}
		/* ssc mode */
		if(PE_CHECK_WIN1(pstParams->win_id))
		{
			UINT32 flt_type_sel;
			flt_type_sel = (disp1_inf->fmt_type == LX_PE_FMT_HD && \
				(disp1_inf->in_h_size>1900 && disp1_inf->in_h_size<1940))? \
				0x0:0x1;	//0:polyphase,1:bilinear
			PE_SSC_H15A0_QWr01(ssc_ctrl, filter_type_sel, flt_type_sel);
			PE_SHP_HW_H15_DBG_PRINT("[win1]fmt:%d, h:%d, ssc flt_type_sel:%d\n",\
				disp1_inf->fmt_type, disp1_inf->in_h_size, flt_type_sel);
		}
	}while(0);
	return ret;
}

/**
 * get resolution fmt using display info(for H15AX)
 *
 * @param   *disp_inf [in] LX_PE_INF_DISPLAY_T
 * @return  PE_SHP_HW_H15A_RES_FMT.
 * @see
 * @author
 */
static PE_SHP_HW_H15A_RES_FMT PE_SHP_HW_H15A_ConvDispInfoToResFmt(LX_PE_INF_DISPLAY_T *disp_inf)
{
	PE_SHP_HW_H15A_RES_FMT res_fmt = PE_SHP_H15A_RES_NUM;
	switch(disp_inf->src_type)
	{
		case LX_PE_SRC_ATV:
			{
				if(disp_inf->cstd_type==LX_PE_CSTD_NTSC)
				{
					res_fmt = PE_SHP_H15A_RES_AV_480;
				}
				else
				{
					res_fmt = PE_SHP_H15A_RES_AV_576;
				}
			}
			break;
		case LX_PE_SRC_CVBS:
		case LX_PE_SRC_SCART:
			{
				res_fmt = PE_SHP_H15A_RES_SD;
			}
			break;
		case LX_PE_SRC_HDMI:
			{
				if(disp_inf->hdmi_type==LX_PE_HDMI_PC)
				{
					res_fmt = PE_SHP_H15A_RES_PC;
				}
				else
				{
					if(disp_inf->fmt_type==LX_PE_FMT_SD)
					{
						res_fmt = PE_SHP_H15A_RES_SD;
					}
					else if(disp_inf->fmt_type==LX_PE_FMT_HD)
					{
						res_fmt = PE_SHP_H15A_RES_HDMI_HD;
					}
					else
					{
						res_fmt = PE_SHP_H15A_RES_UHD;
					}
				}
			}
			break;
		case LX_PE_SRC_DTV:
			{
				if(disp_inf->fmt_type==LX_PE_FMT_SD)
				{
					res_fmt = PE_SHP_H15A_RES_DTV_SD;
				}
				else if(disp_inf->fmt_type==LX_PE_FMT_HD)
				{
					res_fmt = PE_SHP_H15A_RES_HD;
				}
				else
				{
					res_fmt = PE_SHP_H15A_RES_UHD;
				}
			}
			break;
		case LX_PE_SRC_RGBPC:
			{
				res_fmt = PE_SHP_H15A_RES_PC;
			}
			break;
		case LX_PE_SRC_COMP:
		default:
			if(disp_inf->fmt_type==LX_PE_FMT_SD)
			{
				res_fmt = PE_SHP_H15A_RES_SD;
			}
			else if(disp_inf->fmt_type==LX_PE_FMT_HD)
			{
				res_fmt = PE_SHP_H15A_RES_HD;
			}
			else
			{
				res_fmt = PE_SHP_H15A_RES_UHD;
			}
			break;
	}
	return res_fmt;
}
#ifdef PE_SHP_HW_H15_SRE_SHP_WR_ONLY_MD
/**
 * get sre shp(srs) fmt using display info(for H15AX)
 *
 * @param   *disp_inf [in] LX_PE_INF_DISPLAY_T
 * @return  PE_SHP_HW_H15A_SRS_FMT.
 * @see
 * @author
 */
static PE_SHP_HW_H15A_SRS_FMT PE_SHP_HW_H15A_ConvDispInfoToSrsFmt(LX_PE_INF_DISPLAY_T *disp_inf)
{
	PE_SHP_HW_H15A_SRS_FMT srs_fmt = PE_SHP_H15A_SRS_NUM;
	switch(disp_inf->src_type)
	{
		case LX_PE_SRC_ATV:
			{
				if(disp_inf->cstd_type==LX_PE_CSTD_NTSC)
				{
					srs_fmt = PE_SHP_H15A_SRS_AV_480;
				}
				else
				{
					srs_fmt = PE_SHP_H15A_SRS_AV_576;
				}
			}
			break;
		case LX_PE_SRC_CVBS:
		case LX_PE_SRC_SCART:
			{
				srs_fmt = PE_SHP_H15A_SRS_SD;
			}
			break;
		case LX_PE_SRC_HDMI:
			{
				if(disp_inf->hdmi_type==LX_PE_HDMI_PC)
				{
					srs_fmt = PE_SHP_H15A_SRS_PC;
				}
				else
				{
					if(disp_inf->fmt_type==LX_PE_FMT_SD)
					{
						srs_fmt = PE_SHP_H15A_SRS_SD;
					}
					else if(disp_inf->fmt_type==LX_PE_FMT_HD)
					{
						srs_fmt = PE_SHP_H15A_SRS_HDMI_HD;
					}
					else
					{
						srs_fmt = PE_SHP_H15A_SRS_UHD;
					}
				}
			}
			break;
		case LX_PE_SRC_DTV:
			{
				if(disp_inf->fmt_type==LX_PE_FMT_SD)
				{
					srs_fmt = PE_SHP_H15A_SRS_DTV_SD;
				}
				else if(disp_inf->fmt_type==LX_PE_FMT_HD)
				{
					srs_fmt = PE_SHP_H15A_SRS_HD;
				}
				else
				{
					srs_fmt = PE_SHP_H15A_SRS_UHD;
				}
			}
			break;
		case LX_PE_SRC_RGBPC:
			{
				srs_fmt = PE_SHP_H15A_SRS_PC;
			}
		case LX_PE_SRC_COMP:
		default:
			if(disp_inf->fmt_type==LX_PE_FMT_SD)
			{
				srs_fmt = PE_SHP_H15A_SRS_SD;
			}
			else if(disp_inf->fmt_type==LX_PE_FMT_HD)
			{
				srs_fmt = PE_SHP_H15A_SRS_HD;
			}
			else
			{
				srs_fmt = PE_SHP_H15A_SRS_UHD;
			}
			break;
	}
	return srs_fmt;
}
/**
 * get resolution fmt using display info(for H15AX)
 *
 * @param   *disp_inf [in] LX_PE_INF_DISPLAY_T
 * @return  PE_SHP_HW_H15A_SRE_FMT.
 * @see
 * @author
 */
static PE_SHP_HW_H15A_SRE_FMT PE_SHP_HW_H15A_ConvDispInfoToSreFmt(LX_PE_INF_DISPLAY_T *disp_inf)
{
	PE_SHP_HW_H15A_SRE_FMT res_fmt = PE_SHP_H15A_SRE_NUM;
	switch(disp_inf->src_type)
	{
		case LX_PE_SRC_ATV:
		case LX_PE_SRC_CVBS:
		case LX_PE_SRC_SCART:
			{
				res_fmt = PE_SHP_H15A_SRE_SD;
			}
			break;
		case LX_PE_SRC_COMP:
			{
				if(disp_inf->fmt_type==LX_PE_FMT_SD)
				{
					res_fmt = PE_SHP_H15A_SRE_SD;
				}
				else
				{
					res_fmt = PE_SHP_H15A_SRE_COMP_HD;
				}
			}
			break;
		case LX_PE_SRC_HDMI:
		case LX_PE_SRC_DTV:
		case LX_PE_SRC_RGBPC:
		default:
			{
				if(disp_inf->fmt_type==LX_PE_FMT_SD)
				{
					res_fmt = PE_SHP_H15A_SRE_SD;
				}
				else if(disp_inf->fmt_type==LX_PE_FMT_HD)
				{
					res_fmt = PE_SHP_H15A_SRE_HD;
				}
				else
				{
					res_fmt = PE_SHP_H15A_SRE_UHD;
				}
			}
			break;
	}
	return res_fmt;
}

#endif	//#ifdef PE_SHP_HW_H15_SRE_SHP_WR_ONLY_MD
/**
 * get cti(dnr) fmt using display info(for H15AX)
 *
 * @param   *disp_inf [in] LX_PE_INF_DISPLAY_T
 * @return  PE_SHP_HW_H15A_CTI_FMT
 * @see
 * @author
 */
static PE_SHP_HW_H15A_CTI_FMT PE_SHP_HW_H15A_ConvDispInfoToCtiFmt(LX_PE_INF_DISPLAY_T *disp_inf)
{
	PE_SHP_HW_H15A_CTI_FMT cti_fmt = PE_SHP_H15A_CTI_NUM;
	switch(disp_inf->src_type)
	{
		case LX_PE_SRC_ATV:
			if(disp_inf->cstd_type==LX_PE_CSTD_NTSC)
			{
				cti_fmt = PE_SHP_H15A_CTI_ATV_NT;
			}
			else
			{
				cti_fmt = PE_SHP_H15A_CTI_ATV_PAL;
			}
			break;
		case LX_PE_SRC_CVBS:
		case LX_PE_SRC_SCART:
			{
				cti_fmt = PE_SHP_H15A_CTI_SD;
			}
			break;
		case LX_PE_SRC_RGBPC:
			{
				cti_fmt = PE_SHP_H15A_CTI_PC;
			}
			break;
		case LX_PE_SRC_DTV:
		case LX_PE_SRC_COMP:
		case LX_PE_SRC_HDMI:
		default:
			if(disp_inf->src_type==LX_PE_SRC_HDMI && \
				disp_inf->hdmi_type==LX_PE_HDMI_PC)
			{
				cti_fmt = PE_SHP_H15A_CTI_PC;
			}
			else if(disp_inf->fmt_type==LX_PE_FMT_SD)
			{
				cti_fmt = PE_SHP_H15A_CTI_SD;
			}
			else
			{
				cti_fmt = PE_SHP_H15A_CTI_HD;
			}
			break;
	}
	return cti_fmt;
}
/**
 * get pbl(pre blur dnr) fmt using display info(for H15AX)
 *
 * @param   *disp_inf [in] LX_PE_INF_DISPLAY_T
 * @return  PE_SHP_HW_H15A_CTI_FMT
 * @see
 * @author
 */
static PE_SHP_HW_H15A_PBL_FMT PE_SHP_HW_H15A_ConvDispInfoToPblFmt(LX_PE_INF_DISPLAY_T *disp_inf)
{
	PE_SHP_HW_H15A_PBL_FMT pbl_fmt = PE_SHP_H15A_PBL_NUM;
	switch(disp_inf->src_type)
	{
		case LX_PE_SRC_ATV:
			if(disp_inf->cstd_type==LX_PE_CSTD_SECAM)
			{
				pbl_fmt = PE_SHP_H15A_PBL_SECAM;
			}
			else if(disp_inf->cstd_type==LX_PE_CSTD_PAL)
			{
				pbl_fmt = PE_SHP_H15A_PBL_ATV_PAL;
			}
			else
			{
				pbl_fmt = PE_SHP_H15A_PBL_ATV_NT;
			}
			break;
		case LX_PE_SRC_CVBS:
		case LX_PE_SRC_SCART:
			if(disp_inf->cstd_type==LX_PE_CSTD_SECAM)
			{
				pbl_fmt = PE_SHP_H15A_PBL_SECAM;
			}
			else
			{
				pbl_fmt = PE_SHP_H15A_PBL_AV;
			}
			break;
		case LX_PE_SRC_RGBPC:
			{
				pbl_fmt = PE_SHP_H15A_PBL_PC;
			}
			break;
		case LX_PE_SRC_DTV:
			if(disp_inf->dtv_type==LX_PE_DTV_HDDPLAY)
			{
				if(disp_inf->hdd_type==LX_PE_HDD_SRC_ATV60)
				{
					pbl_fmt = PE_SHP_H15A_PBL_ATV_NT;
				}
				else if(disp_inf->hdd_type==LX_PE_HDD_SRC_ATV50)
				{
					pbl_fmt = PE_SHP_H15A_PBL_ATV_PAL;
				}
				else if(disp_inf->hdd_type==LX_PE_HDD_SRC_AV60 || \
					disp_inf->hdd_type==LX_PE_HDD_SRC_AV50 || \
					disp_inf->hdd_type==LX_PE_HDD_SRC_SCARTRGB)
				{
					pbl_fmt = PE_SHP_H15A_PBL_AV;
				}
				else	//LX_PE_HDD_SRC_DTV
				{
					if(disp_inf->fmt_type==LX_PE_FMT_SD)
					{
						pbl_fmt = PE_SHP_H15A_PBL_DTV_SD;
					}
					else
					{
						pbl_fmt = PE_SHP_H15A_PBL_HD;
					}
				}
			}
			else if(disp_inf->fmt_type==LX_PE_FMT_SD)
			{
				pbl_fmt = PE_SHP_H15A_PBL_DTV_SD;
			}
			else
			{
				pbl_fmt = PE_SHP_H15A_PBL_HD;
			}
			break;
		case LX_PE_SRC_COMP:
			if(disp_inf->fmt_type==LX_PE_FMT_SD)
			{
				pbl_fmt = PE_SHP_H15A_PBL_COMP_SD;
			}
			else
			{
				pbl_fmt = PE_SHP_H15A_PBL_HD;
			}
			break;
		case LX_PE_SRC_HDMI:
		default:
			if(disp_inf->hdmi_type==LX_PE_HDMI_PC)
			{
				pbl_fmt = PE_SHP_H15A_PBL_PC;
			}
			else if(disp_inf->fmt_type==LX_PE_FMT_SD)
			{
				pbl_fmt = PE_SHP_H15A_PBL_HDMI_SD;
			}
			else
			{
				pbl_fmt = PE_SHP_H15A_PBL_HD;
			}
			break;
	}
	return pbl_fmt;
}
/**
 * set(store) scaler filter coeff info
 *
 * @param   *pstParams [in] LX_PE_SHP_SCLFILTER_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_SHP_HW_H15_SetScalerFilterCoeff(LX_PE_SHP_SCLFILTER_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		PE_SHP_HW_H15_DBG_PRINT("set[%d]:cur idx_y:%d,%d,idx_c:%d,%d\n", \
			pstParams->win_id, pstParams->h_luma_id, pstParams->v_luma_id, \
			pstParams->h_chrm_id,pstParams->v_chrm_id);
		if(PE_KDRV_VER_H15)
		{
			if(PE_CHECK_WIN0(pstParams->win_id))
			{
				#ifdef PE_SHP_HW_H15_SCL_CO_INTERNAL_DECISION
				PE_SHP_HW_H15_DBG_PRINT("nothing to do.\n");ret = RET_OK;
				#else
				PE_CO_PE1_H15A0_RdFL(pe1_fsw_ctrl_00);
				PE_CO_PE1_H15A0_Wr01(pe1_fsw_ctrl_00, gsc_y_h_idx, PE_SHP_HW_H15_IDX_DEC_TO_HEX(pstParams->h_luma_id));
				PE_CO_PE1_H15A0_Wr01(pe1_fsw_ctrl_00, gsc_y_v_idx, PE_SHP_HW_H15_IDX_DEC_TO_HEX(pstParams->v_luma_id));
				PE_CO_PE1_H15A0_Wr01(pe1_fsw_ctrl_00, gsc_c_h_idx, PE_SHP_HW_H15_IDX_DEC_TO_HEX(pstParams->h_chrm_id));
				PE_CO_PE1_H15A0_Wr01(pe1_fsw_ctrl_00, gsc_c_v_idx, PE_SHP_HW_H15_IDX_DEC_TO_HEX(pstParams->v_chrm_id));
				PE_CO_PE1_H15A0_WrFL(pe1_fsw_ctrl_00);
				_g_pe_shp_hw_h15_info.user_gsc_idx.h_y_indx = pstParams->h_luma_id;
				_g_pe_shp_hw_h15_info.user_gsc_idx.v_y_indx = pstParams->v_luma_id;
				_g_pe_shp_hw_h15_info.user_gsc_idx.h_c_indx = pstParams->h_chrm_id;
				_g_pe_shp_hw_h15_info.user_gsc_idx.v_c_indx = pstParams->v_chrm_id;
				#endif
			}
			if(PE_CHECK_WIN1(pstParams->win_id))
			{
				PE_SHP_HW_H15_DBG_PRINT("nothing to do.\n");ret = RET_OK;
			}
		}
		else
		{
			PE_SHP_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
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
int PE_SHP_HW_H15_GetScalerFilterCoeff(LX_PE_SHP_SCLFILTER_T *pstParams)
{
	int ret = RET_OK;
	LX_PE_WIN_ID win_id;
	PE_SHP_HW_H15_SCL_INDX_T *p_idx = &_g_pe_shp_hw_h15_info.user_gsc_idx;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		win_id = PE_GET_CHECKED_WINID(pstParams->win_id);
		if(PE_KDRV_VER_H15)
		{
			if(PE_CHECK_WIN0(win_id))
			{
				//PE_CO_PE1_H15A0_RdFL(pe1_fsw_ctrl_00);
				//PE_CO_PE1_H15A0_Rd01(pe1_fsw_ctrl_00, gsc_y_h_idx, pstParams->h_luma_id);
				//PE_CO_PE1_H15A0_Rd01(pe1_fsw_ctrl_00, gsc_y_v_idx, pstParams->v_luma_id);
				//PE_CO_PE1_H15A0_Rd01(pe1_fsw_ctrl_00, gsc_c_h_idx, pstParams->h_chrm_id);
				//PE_CO_PE1_H15A0_Rd01(pe1_fsw_ctrl_00, gsc_c_v_idx, pstParams->v_chrm_id);
				pstParams->h_luma_id = p_idx->h_y_indx;
				pstParams->v_luma_id = p_idx->v_y_indx;
				pstParams->h_chrm_id = p_idx->h_c_indx;
				pstParams->v_chrm_id = p_idx->v_c_indx;
			}
			if(PE_CHECK_WIN1(win_id))
			{
				PE_SHP_HW_H15_DBG_PRINT("nothing to do.\n");ret = RET_OK;
			}
		}
		else
		{
			PE_SHP_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
		PE_SHP_HW_H15_DBG_PRINT("get[%d]:cur idx_y:%d,%d,idx_c:%d,%d\n", \
			pstParams->win_id, pstParams->h_luma_id, pstParams->v_luma_id, \
			pstParams->h_chrm_id,pstParams->v_chrm_id);
	}while(0);
	return ret;
}
#ifdef PE_SHP_PARAM_CHANGE_FOR_H15

/**
 * set(store) sre(post) scaler filter coeff info
 *
 * @param   *pstParams [in] LX_PE_SHP_SR_SCLFILTER_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_SHP_HW_H15_SetSreScalerFilterCoeff(LX_PE_SHP_SR_SCLFILTER_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		PE_SHP_HW_H15_DBG_PRINT("set[%d]:cur idx_y:%d,%d,idx_c:%d,%d\n", \
			pstParams->win_id, pstParams->h_luma_id, pstParams->v_luma_id,\
			pstParams->h_chrm_id, pstParams->v_chrm_id);
		if(PE_KDRV_VER_H15)
		{
			if(PE_CHECK_WIN0(pstParams->win_id))
			{
				#ifdef PE_SHP_HW_H15_SCL_CO_INTERNAL_DECISION
				PE_SHP_HW_H15_DBG_PRINT("nothing to do.\n");ret = RET_OK;
				#else
				PE_CO_PE1_H15A0_RdFL(pe1_fsw_ctrl_01);
				PE_CO_PE1_H15A0_Wr01(pe1_fsw_ctrl_01, psc_y_h_idx, PE_SHP_HW_H15_IDX_DEC_TO_HEX(pstParams->h_luma_id));
				PE_CO_PE1_H15A0_Wr01(pe1_fsw_ctrl_01, psc_y_v_idx, PE_SHP_HW_H15_IDX_DEC_TO_HEX(pstParams->v_luma_id));
				PE_CO_PE1_H15A0_WrFL(pe1_fsw_ctrl_01);
				_g_pe_shp_hw_h15_info.user_psc_idx.h_y_indx = pstParams->h_luma_id;
				_g_pe_shp_hw_h15_info.user_psc_idx.v_y_indx = pstParams->v_luma_id;
				_g_pe_shp_hw_h15_info.user_psc_idx.h_c_indx = pstParams->h_chrm_id;
				_g_pe_shp_hw_h15_info.user_psc_idx.v_c_indx = pstParams->v_chrm_id;
				#endif
			}
			if(PE_CHECK_WIN1(pstParams->win_id))
			{
				PE_SHP_HW_H15_DBG_PRINT("nothing to do.\n");ret = RET_OK;
			}
		}
		else
		{
			PE_SHP_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * get(store) sre(post)scaler filter coeff info
 *
 * @param   *pstParams [in] LX_PE_SHP_SR_SCLFILTER_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_SHP_HW_H15_GetSreScalerFilterCoeff(LX_PE_SHP_SR_SCLFILTER_T *pstParams)
{
	int ret = RET_OK;
	LX_PE_WIN_ID win_id;
	PE_SHP_HW_H15_SCL_INDX_T *p_idx = &_g_pe_shp_hw_h15_info.user_psc_idx;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		win_id = PE_GET_CHECKED_WINID(pstParams->win_id);
		if(PE_KDRV_VER_H15)
		{
			if(PE_CHECK_WIN0(win_id))
			{
				//PE_CO_PE1_H15A0_RdFL(pe1_fsw_ctrl_01);
				//PE_CO_PE1_H15A0_Rd01(pe1_fsw_ctrl_01, psc_y_h_idx, pstParams->h_luma_id);
				//PE_CO_PE1_H15A0_Rd01(pe1_fsw_ctrl_01, psc_y_v_idx, pstParams->v_luma_id);
				pstParams->h_luma_id = p_idx->h_y_indx;
				pstParams->v_luma_id = p_idx->v_y_indx;
				pstParams->h_chrm_id = p_idx->h_c_indx;
				pstParams->v_chrm_id = p_idx->v_c_indx;
			}
			if(PE_CHECK_WIN1(win_id))
			{
				PE_SHP_HW_H15_DBG_PRINT("nothing to do.\n");ret = RET_OK;
			}
		}
		else
		{
			PE_SHP_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
		PE_SHP_HW_H15_DBG_PRINT("get[%d]:cur idx_y:%d,%d,idx_c:%d,%d\n", \
			pstParams->win_id, pstParams->h_luma_id, pstParams->v_luma_id,\
			pstParams->h_chrm_id, pstParams->v_chrm_id);
	}while(0);
	return ret;
}
#endif
#ifdef PE_SHP_HW_H15_SCL_CO_INTERNAL_DECISION
/**
 * decide scaler filter coeff
 *
 * @param   *pCfg [in] PE_TSK_H15_CFG_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_SHP_HW_H15_SelectScFltCoeff(PE_TSK_H15_CFG_T *pCfg)
{
	int ret = RET_OK;
	PE_SHP_HW_H15_SCL_INDX_T cur_gsc_idx, cur_psc_idx, cur_sub_idx;
	LX_PE_INF_DISPLAY_T *disp0_info = &pCfg->inf.disp_info[LX_PE_WIN_0];
	PE_SHP_HW_H15_SCL_INDX_T *p_gsc_idx = &_g_pe_shp_hw_h15_info.user_gsc_idx;
	PE_SHP_HW_H15_SCL_INDX_T *p_psc_idx = &_g_pe_shp_hw_h15_info.user_psc_idx;
	PE_SHP_HW_H15_SCL_INDX_T *p_sub_idx = &_g_pe_shp_hw_h15_info.user_sub_idx;
	/* main */
	const PE_SHP_HW_H15_SCL_INDX_T midx_hd      = { 304, 304, 306, 306};
	const PE_SHP_HW_H15_SCL_INDX_T midx_atv     = { 404, 606, 406, 606};//atv
	const PE_SHP_HW_H15_SCL_INDX_T midx_sd_dtv  = { 304, 606, 406, 606};
	const PE_SHP_HW_H15_SCL_INDX_T midx_sd_hdmi = { 304,   3, 406, 606};//av,hdmi,comp
	const PE_SHP_HW_H15_SCL_INDX_T midx_dflt    = {   6,   6,   6,   6};
	/* sub */
	const PE_SHP_HW_H15_SCL_INDX_T sidx_atv     = {1210,1210,1210,1210};//atv
	const PE_SHP_HW_H15_SCL_INDX_T sidx_sd_dtv  = { 610,1210, 610,1210};//dtv_sd
	const PE_SHP_HW_H15_SCL_INDX_T sidx_dflt    = { 610, 610, 610, 610};//orthers
	do{
		CHECK_KNULL(pCfg);
		/* gsc */
		switch (disp0_info->src_type)
		{
			case LX_PE_SRC_ATV:
				{
					cur_gsc_idx = midx_atv;
				}
				break;
			case LX_PE_SRC_CVBS:
			case LX_PE_SRC_SCART:
				{
					cur_gsc_idx = midx_sd_hdmi;
				}
				break;
			case LX_PE_SRC_DTV:
				if(disp0_info->fmt_type==LX_PE_FMT_SD)
				{
					cur_gsc_idx = midx_sd_dtv;
				}
				else	//HD,UHD
				{
					cur_gsc_idx = midx_hd;
				}
				break;
			case LX_PE_SRC_COMP:
			case LX_PE_SRC_HDMI:
				if(disp0_info->src_type==LX_PE_SRC_HDMI && \
					disp0_info->hdmi_type==LX_PE_HDMI_PC)
				{
					cur_gsc_idx = midx_dflt;
				}
				else if(disp0_info->fmt_type==LX_PE_FMT_SD)
				{
					cur_gsc_idx = midx_sd_hdmi;
				}
				else	//HD,UHD
				{
					cur_gsc_idx = midx_hd;
				}
				break;
			case LX_PE_SRC_RGBPC:
			default:
				{
					cur_gsc_idx = midx_dflt;
				}
				break;
		}
		/* psc(fix) */
		cur_psc_idx = midx_dflt;
		/* sub */
		switch (disp0_info->src_type)
		{
			case LX_PE_SRC_ATV:
				{
					cur_sub_idx = sidx_atv;
				}
				break;
			case LX_PE_SRC_DTV:
				if(disp0_info->fmt_type==LX_PE_FMT_SD)
				{
					cur_sub_idx = sidx_sd_dtv;
				}
				else	//HD,UHD
				{
					cur_sub_idx = sidx_dflt;
				}
				break;
			case LX_PE_SRC_CVBS:
			case LX_PE_SRC_SCART:
			case LX_PE_SRC_COMP:
			case LX_PE_SRC_HDMI:
			case LX_PE_SRC_RGBPC:
			default:
				{
					cur_sub_idx = sidx_dflt;
				}
				break;
		}
		/* wr gsc idx */
		if (p_gsc_idx->h_y_indx != cur_gsc_idx.h_y_indx || \
			p_gsc_idx->v_y_indx != cur_gsc_idx.v_y_indx || \
			p_gsc_idx->h_c_indx != cur_gsc_idx.h_c_indx || \
			p_gsc_idx->v_c_indx != cur_gsc_idx.v_c_indx)
		{
			PE_SHP_HW_H15_DBG_PRINT("[inf]src:%d\n", disp0_info->src_type);
			PE_SHP_HW_H15_DBG_PRINT("[set]gsc:%04d,%04d,%04d,%04d->%04d,%04d,%04d,%04d\n",\
				p_gsc_idx->h_y_indx, p_gsc_idx->v_y_indx, \
				p_gsc_idx->h_c_indx, p_gsc_idx->v_c_indx, \
				cur_gsc_idx.h_y_indx, cur_gsc_idx.v_y_indx, \
				cur_gsc_idx.h_c_indx, cur_gsc_idx.v_c_indx);
			PE_CO_PE1_H15A0_RdFL(pe1_fsw_ctrl_00);
			PE_CO_PE1_H15A0_Wr01(pe1_fsw_ctrl_00, gsc_y_h_idx, PE_SHP_HW_H15_IDX_DEC_TO_HEX(cur_gsc_idx.h_y_indx));
			PE_CO_PE1_H15A0_Wr01(pe1_fsw_ctrl_00, gsc_y_v_idx, PE_SHP_HW_H15_IDX_DEC_TO_HEX(cur_gsc_idx.v_y_indx));
			PE_CO_PE1_H15A0_Wr01(pe1_fsw_ctrl_00, gsc_c_h_idx, PE_SHP_HW_H15_IDX_DEC_TO_HEX(cur_gsc_idx.h_c_indx));
			PE_CO_PE1_H15A0_Wr01(pe1_fsw_ctrl_00, gsc_c_v_idx, PE_SHP_HW_H15_IDX_DEC_TO_HEX(cur_gsc_idx.v_c_indx));
			PE_CO_PE1_H15A0_WrFL(pe1_fsw_ctrl_00);
			p_gsc_idx->h_y_indx = cur_gsc_idx.h_y_indx;
			p_gsc_idx->v_y_indx = cur_gsc_idx.v_y_indx;
			p_gsc_idx->h_c_indx = cur_gsc_idx.h_c_indx;
			p_gsc_idx->v_c_indx = cur_gsc_idx.v_c_indx;
		}
		/* wr psc idx */
		if (p_psc_idx->h_y_indx != cur_psc_idx.h_y_indx || \
			p_psc_idx->v_y_indx != cur_psc_idx.v_y_indx || \
			p_sub_idx->h_y_indx != cur_sub_idx.h_y_indx || \
			p_sub_idx->v_y_indx != cur_sub_idx.v_y_indx)
		{
			PE_SHP_HW_H15_DBG_PRINT("[inf]src:%d\n", disp0_info->src_type);
			PE_SHP_HW_H15_DBG_PRINT("[set]psc:%04d,%04d->%04d,%04d,sub:%04d,%04d->%04d,%04d\n",\
				p_psc_idx->h_y_indx, p_psc_idx->v_y_indx, \
				p_sub_idx->h_c_indx, p_sub_idx->v_c_indx, \
				cur_psc_idx.h_y_indx, cur_psc_idx.v_y_indx, \
				cur_sub_idx.h_c_indx, cur_sub_idx.v_c_indx);
			PE_CO_PE1_H15A0_RdFL(pe1_fsw_ctrl_01);
			PE_CO_PE1_H15A0_Wr01(pe1_fsw_ctrl_01, psc_y_h_idx, PE_SHP_HW_H15_IDX_DEC_TO_HEX(cur_psc_idx.h_y_indx));
			PE_CO_PE1_H15A0_Wr01(pe1_fsw_ctrl_01, psc_y_v_idx, PE_SHP_HW_H15_IDX_DEC_TO_HEX(cur_psc_idx.v_y_indx));
			PE_CO_PE1_H15A0_Wr01(pe1_fsw_ctrl_01, sub_y_h_idx, PE_SHP_HW_H15_IDX_DEC_TO_HEX(cur_sub_idx.h_y_indx));
			PE_CO_PE1_H15A0_Wr01(pe1_fsw_ctrl_01, sub_y_v_idx, PE_SHP_HW_H15_IDX_DEC_TO_HEX(cur_sub_idx.v_y_indx));
			PE_CO_PE1_H15A0_WrFL(pe1_fsw_ctrl_01);
			p_psc_idx->h_y_indx = cur_psc_idx.h_y_indx;
			p_psc_idx->v_y_indx = cur_psc_idx.v_y_indx;
			p_sub_idx->h_y_indx = cur_sub_idx.h_y_indx;
			p_sub_idx->v_y_indx = cur_sub_idx.v_y_indx;
		}
	}while(0);
	return ret;
}
#endif
/**
 * set scaler filter control periodically
 *
 * @param   *pstParams [in] PE_TSK_H15_CFG_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_SHP_HW_H15_SetScalerFilterCtrl(PE_TSK_H15_CFG_T *pCfg)
{
	int ret = RET_OK;
	UINT32 indx[8];
	PE_SHP_HW_H15_SETTINGS_T *pInfo = &_g_pe_shp_hw_h15_info;
	PE_SHP_HW_H15_SCL_CTRL_T *p_cur_m1_y = &pInfo->cur_info.m1_y;
	PE_SHP_HW_H15_SCL_CTRL_T *p_cur_m2_y = &pInfo->cur_info.m2_y;
	PE_SHP_HW_H15_SCL_CTRL_T *p_cur_m1_c = &pInfo->cur_info.m1_c;
	PE_SHP_HW_H15_SCL_CTRL_T *p_cur_m2_c = &pInfo->cur_info.m2_c;
	PE_SHP_HW_H15_SCL_CTRL_T *p_cur_mp_y = &pInfo->cur_info.mp_y;
	PE_INF_H15_CTRL_T *ctrl0_info = &pCfg->inf.ctrl_info[LX_PE_WIN_0];
	PE_INF_H15_CTRL_T *ctrl1_info = &pCfg->inf.ctrl_info[LX_PE_WIN_1];
	LX_PE_INF_DISPLAY_T *disp0_info = &pCfg->inf.disp_info[LX_PE_WIN_0];
	do{
		CHECK_KNULL(pCfg);
		if(PE_KDRV_VER_H15)
		{
			if (PE_CMN_HW_H15_GetDbInfo(PE_CMN_HW_H15_DB_INF_MD0_SCL_FW_EN, 0))
			{
				/* skip drv scl ctrl, if fw scl on. */
				ret = RET_OK;
				break;
			}
			#ifdef PE_SHP_HW_H15_SCL_CO_INTERNAL_DECISION
			ret = PE_SHP_HW_H15_SelectScFltCoeff(pCfg);
			PE_SHP_HW_H15_CHECK_CODE(ret, break, \
				"[%s,%d] PE_SHP_HW_H15_SelectScFltCoeff() error.\n", __F__, __L__);
			#endif
			/*******************************************************************************/
			/* get info*/
			/*******************************************************************************/
			PE_CO_PE1_H15A0_RdFL(pe1_fsw_ctrl_00);
			PE_CO_PE1_H15A0_RdFL(pe1_fsw_ctrl_01);
			PE_CO_PE1_H15A0_Rd01(pe1_fsw_ctrl_00, gsc_y_h_idx, indx[0]);
			PE_CO_PE1_H15A0_Rd01(pe1_fsw_ctrl_00, gsc_y_v_idx, indx[1]);
			PE_CO_PE1_H15A0_Rd01(pe1_fsw_ctrl_00, gsc_c_h_idx, indx[2]);
			PE_CO_PE1_H15A0_Rd01(pe1_fsw_ctrl_00, gsc_c_v_idx, indx[3]);
			PE_CO_PE1_H15A0_Rd01(pe1_fsw_ctrl_01, psc_y_h_idx, indx[4]);
			PE_CO_PE1_H15A0_Rd01(pe1_fsw_ctrl_01, psc_y_v_idx, indx[5]);
			PE_CO_PE1_H15A0_Rd01(pe1_fsw_ctrl_01, sub_y_h_idx, indx[6]);
			PE_CO_PE1_H15A0_Rd01(pe1_fsw_ctrl_01, sub_y_v_idx, indx[7]);
			/* m1_y */
			p_cur_m1_y->in_c_fmt = ctrl0_info->i_c_format;
			p_cur_m1_y->pixel_rp = ctrl0_info->pxl_rp_st;
			p_cur_m1_y->acti_fmt = disp0_info->act_fmt;
			p_cur_m1_y->h_y_indx = PE_SHP_HW_H15_IDX_HEX_TO_DEC(indx[0]);
			p_cur_m1_y->v_y_indx = PE_SHP_HW_H15_IDX_HEX_TO_DEC(indx[1]);
			p_cur_m1_y->h_s_indx = PE_SHP_HW_H15_IDX_HEX_TO_DEC(indx[6]);
			p_cur_m1_y->v_s_indx = PE_SHP_HW_H15_IDX_HEX_TO_DEC(indx[7]);
			PE_MSC_Y_H15A0_QRd02(msc_m1_y_crop_param3,reg_hisize_m1,p_cur_m1_y->h_i_size, \
                                                      reg_visize_m1,p_cur_m1_y->v_i_size);
			PE_MSC_Y_H15A0_QRd02(msc_m1_y_crop_param2,reg_hactive_m1,p_cur_m1_y->h_active, \
                                                      reg_vactive_m1,p_cur_m1_y->v_active);
			PE_MSC_Y_H15A0_QRd02(msc_m1_y_outsize,reg_hosize_m1,p_cur_m1_y->h_o_size, \
                                                  reg_vosize_m1,p_cur_m1_y->v_o_size);
			/* m2_y */
			p_cur_m2_y->in_c_fmt = ctrl1_info->i_c_format;
			p_cur_m2_y->pixel_rp = ctrl1_info->pxl_rp_st;
			p_cur_m2_y->acti_fmt = disp0_info->act_fmt;
			p_cur_m2_y->h_y_indx = PE_SHP_HW_H15_IDX_HEX_TO_DEC(indx[0]);
			p_cur_m2_y->v_y_indx = PE_SHP_HW_H15_IDX_HEX_TO_DEC(indx[1]);
			p_cur_m2_y->h_s_indx = PE_SHP_HW_H15_IDX_HEX_TO_DEC(indx[6]);
			p_cur_m2_y->v_s_indx = PE_SHP_HW_H15_IDX_HEX_TO_DEC(indx[7]);
			PE_MSC_Y_H15A0_QRd02(msc_m2_y_crop_param3,reg_hisize_r,p_cur_m2_y->h_i_size, \
                                                      reg_visize_r,p_cur_m2_y->v_i_size);
			PE_MSC_Y_H15A0_QRd02(msc_m2_y_crop_param2,reg_hactive_r,p_cur_m2_y->h_active, \
                                                      reg_vactive_r,p_cur_m2_y->v_active);
			PE_MSC_Y_H15A0_QRd02(msc_m2_y_outsize,reg_hosize_r,p_cur_m2_y->h_o_size, \
                                                  reg_vosize_r,p_cur_m2_y->v_o_size);
			/* m1_c */
			p_cur_m1_c->in_c_fmt = ctrl0_info->i_c_format;
			p_cur_m1_c->pixel_rp = ctrl0_info->pxl_rp_st;
			p_cur_m1_c->acti_fmt = disp0_info->act_fmt;
			p_cur_m1_c->h_c_indx = PE_SHP_HW_H15_IDX_HEX_TO_DEC(indx[2]);
			p_cur_m1_c->v_c_indx = PE_SHP_HW_H15_IDX_HEX_TO_DEC(indx[3]);
			PE_MSC_C_H15A0_QRd02(msc_m1_c_crop_param3,reg_hisize_m1,p_cur_m1_c->h_i_size, \
                                                      reg_visize_m1,p_cur_m1_c->v_i_size);
			PE_MSC_C_H15A0_QRd02(msc_m1_c_crop_param2,reg_hactive_m1,p_cur_m1_c->h_active, \
                                                      reg_vactive_m1,p_cur_m1_c->v_active);
			PE_MSC_C_H15A0_QRd02(msc_m1_c_outsize,reg_hosize_m1,p_cur_m1_c->h_o_size, \
                                                  reg_vosize_m1,p_cur_m1_c->v_o_size);
			/* m2_c */
			p_cur_m2_c->in_c_fmt = ctrl1_info->i_c_format;
			p_cur_m2_c->pixel_rp = ctrl1_info->pxl_rp_st;
			p_cur_m2_c->acti_fmt = disp0_info->act_fmt;
			p_cur_m2_c->h_c_indx = PE_SHP_HW_H15_IDX_HEX_TO_DEC(indx[2]);
			p_cur_m2_c->v_c_indx = PE_SHP_HW_H15_IDX_HEX_TO_DEC(indx[3]);
			PE_MSC_C_H15A0_QRd02(msc_m2_c_crop_param3,reg_hisize_m2,p_cur_m2_c->h_i_size, \
                                                      reg_visize_m2,p_cur_m2_c->v_i_size);
			PE_MSC_C_H15A0_QRd02(msc_m2_c_crop_param2,reg_hactive_m2,p_cur_m2_c->h_active, \
                                                      reg_vactive_m2,p_cur_m2_c->v_active);
			PE_MSC_C_H15A0_QRd02(msc_m2_c_outsize,reg_hosize_m2,p_cur_m2_c->h_o_size, \
                                                  reg_vosize_m2,p_cur_m2_c->v_o_size);
			/* mp_y */
			p_cur_mp_y->in_c_fmt = ctrl0_info->i_c_format;
			p_cur_mp_y->pixel_rp = ctrl0_info->pxl_rp_st;
			p_cur_mp_y->acti_fmt = disp0_info->act_fmt;
			p_cur_mp_y->h_y_indx = PE_SHP_HW_H15_IDX_HEX_TO_DEC(indx[4]);
			p_cur_mp_y->v_y_indx = PE_SHP_HW_H15_IDX_HEX_TO_DEC(indx[5]);
			p_cur_mp_y->h_s_indx = PE_SHP_HW_H15_IDX_HEX_TO_DEC(indx[6]);
			p_cur_mp_y->v_s_indx = PE_SHP_HW_H15_IDX_HEX_TO_DEC(indx[7]);
			PE_SR_GSC_H15A0_QRd02(sr_gsc_y_insize,reg_hisize_m1,p_cur_mp_y->h_i_size, \
                                                  reg_visize_m1,p_cur_mp_y->v_i_size);
			PE_SR_GSC_H15A0_QRd02(sr_gsc_y_outsize,reg_hosize_m1,p_cur_mp_y->h_o_size, \
                                                   reg_vosize_m1,p_cur_mp_y->v_o_size);
			p_cur_mp_y->h_active = p_cur_mp_y->h_i_size;
			p_cur_mp_y->v_active = p_cur_mp_y->v_i_size;
			/*******************************************************************************/
			/* ctrl */
			/*******************************************************************************/
			/* m1_y */
			ret = PE_SHP_HW_H15_SetMscM1Y(pCfg);
			PE_SHP_HW_H15_CHECK_CODE(ret, break, \
				"[%s,%d] PE_SHP_HW_H15_SetMscM1Y() error.\n", __F__, __L__);
			/* m2_y */
			ret = PE_SHP_HW_H15_SetMscM2Y(pCfg);
			PE_SHP_HW_H15_CHECK_CODE(ret, break, \
				"[%s,%d] PE_SHP_HW_H15_SetMscM2Y() error.\n", __F__, __L__);
			/* m1_c */
			ret = PE_SHP_HW_H15_SetMscM1C(pCfg);
			PE_SHP_HW_H15_CHECK_CODE(ret, break, \
				"[%s,%d] PE_SHP_HW_H15_SetMscM1C() error.\n", __F__, __L__);
			/* m2_c */
			ret = PE_SHP_HW_H15_SetMscM2C(pCfg);
			PE_SHP_HW_H15_CHECK_CODE(ret, break, \
				"[%s,%d] PE_SHP_HW_H15_SetMscM2C() error.\n", __F__, __L__);
			/* mp_y */
			ret = PE_SHP_HW_H15_SetMscMPY(pCfg);
			PE_SHP_HW_H15_CHECK_CODE(ret, break, \
				"[%s,%d] PE_SHP_HW_H15_SetMscMPY() error.\n", __F__, __L__);
		}
		else
		{
			PE_SHP_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * check valid 12t main filter
 *
 * @param   co_num [in] UINT32
 * @return  if 0, invalid, otherwise valid
 * @see
 * @author
 */
static UINT32 PE_SHP_HW_H15_ChkValid12tMain(UINT32 co_num)
{
	UINT32 valid = 1;
	switch (co_num)
	{
		case PE_SHP_HW_H15_SCL_CO_NUM_12T_0002:
		case PE_SHP_HW_H15_SCL_CO_NUM_12T_0003:
		case PE_SHP_HW_H15_SCL_CO_NUM_12T_0006:
		case PE_SHP_HW_H15_SCL_CO_NUM_12T_0304:
		case PE_SHP_HW_H15_SCL_CO_NUM_12T_0306:
		case PE_SHP_HW_H15_SCL_CO_NUM_12T_0404:
		case PE_SHP_HW_H15_SCL_CO_NUM_12T_0406:
		case PE_SHP_HW_H15_SCL_CO_NUM_12T_0606:
		case PE_SHP_HW_H15_SCL_CO_NUM_12T_0806:
		case PE_SHP_HW_H15_SCL_CO_NUM_12T_1006:
		case PE_SHP_HW_H15_SCL_CO_NUM_12T_1410:
			break;
		default:
			valid = 0;
			break;
	}
	return valid;
}
/**
 * check valid 12t sub filter
 *
 * @param   co_num [in] UINT32
 * @return  if 0, invalid, otherwise valid
 * @see
 * @author
 */
static UINT32 PE_SHP_HW_H15_ChkValid12tSub(UINT32 co_num)
{
	UINT32 valid = 1;
	switch (co_num)
	{
		case PE_SHP_HW_H15_SCL_CO_NUM_12T_0610:
		case PE_SHP_HW_H15_SCL_CO_NUM_12T_1210:
			break;
		default:
			valid = 0;
			break;
	}
	return valid;
}
/**
 * check valid 06t main filter
 *
 * @param   co_num [in] UINT32
 * @return  if 0, invalid, otherwise valid
 * @see
 * @author
 */
static UINT32 PE_SHP_HW_H15_ChkValid06tMain(UINT32 co_num)
{
	UINT32 valid = 1;
	switch (co_num)
	{
		case PE_SHP_HW_H15_SCL_CO_NUM_06T_0006:
		case PE_SHP_HW_H15_SCL_CO_NUM_06T_0306:
			break;
		default:
			valid = 0;
			break;
	}
	return valid;
}
/**
 * write msc h1 reg
 *
 * @param   co_num [in] UINT32
 * @param   sel [in] UINT32
 * @return  void
 * @see
 * @author
 */
static void PE_SHP_HW_H15_WrMscH1Reg(UINT32 co_num, UINT32 sel)
{
	int ret = RET_OK;
	UINT32 offset;
	offset = (sel == PE_SHP_HW_H15_SCL_M1Y)? PE_H15A_MSC_Y_M1_OFST:\
		(sel == PE_SHP_HW_H15_SCL_M2Y)? PE_H15A_MSC_Y_M2_OFST:\
		(sel == PE_SHP_HW_H15_SCL_M1C)? PE_H15A_MSC_C_M1_OFST:\
		(sel == PE_SHP_HW_H15_SCL_M2C)? PE_H15A_MSC_C_M2_OFST:\
		PE_H15A_SR_GSC_OFST;
	offset += PE_H15A_SC_H1_OFST;
	switch (co_num)
	{
		case PE_SHP_HW_H15_SCL_CO_NUM_12T_0002:
			PE_SHP_HW_H15A_SET_MSC_FLT(msc_12t_m_flt_0002_h15,offset);
			break;
		case PE_SHP_HW_H15_SCL_CO_NUM_12T_0003:
			PE_SHP_HW_H15A_SET_MSC_FLT(msc_12t_m_flt_0003_h15,offset);
			break;
		case PE_SHP_HW_H15_SCL_CO_NUM_12T_0006:
			PE_SHP_HW_H15A_SET_MSC_FLT(msc_12t_m_flt_0006_h15,offset);
			break;
		case PE_SHP_HW_H15_SCL_CO_NUM_12T_0304:
			PE_SHP_HW_H15A_SET_MSC_FLT(msc_12t_m_flt_0304_h15,offset);
			break;
		case PE_SHP_HW_H15_SCL_CO_NUM_12T_0306:
			PE_SHP_HW_H15A_SET_MSC_FLT(msc_12t_m_flt_0306_h15,offset);
			break;
		case PE_SHP_HW_H15_SCL_CO_NUM_12T_0404:
			PE_SHP_HW_H15A_SET_MSC_FLT(msc_12t_m_flt_0404_h15,offset);
			break;
		case PE_SHP_HW_H15_SCL_CO_NUM_12T_0406:
			PE_SHP_HW_H15A_SET_MSC_FLT(msc_12t_m_flt_0406_h15,offset);
			break;
		case PE_SHP_HW_H15_SCL_CO_NUM_12T_0606:
			PE_SHP_HW_H15A_SET_MSC_FLT(msc_12t_m_flt_0606_h15,offset);
			break;
		case PE_SHP_HW_H15_SCL_CO_NUM_12T_0806:
			PE_SHP_HW_H15A_SET_MSC_FLT(msc_12t_m_flt_0806_h15,offset);
			break;
		case PE_SHP_HW_H15_SCL_CO_NUM_12T_1006:
			PE_SHP_HW_H15A_SET_MSC_FLT(msc_12t_m_flt_1006_h15,offset);
			break;
		case PE_SHP_HW_H15_SCL_CO_NUM_12T_1410:
			PE_SHP_HW_H15A_SET_MSC_FLT(msc_12t_m_flt_1410_h15,offset);
			break;
		default:
			PE_PRINT_NOTI("not support. sel,co:%d,%05d(0x%08x).\n", \
				sel, co_num, offset);
			break;
	}
	return;
}
/**
 * write msc h2 reg
 *
 * @param   co_num [in] UINT32
 * @param   sel [in] UINT32
 * @return  void
 * @see
 * @author
 */
static void PE_SHP_HW_H15_WrMscH2Reg(UINT32 co_num, UINT32 sel)
{
	int ret = RET_OK;
	UINT32 offset;
	offset = (sel == PE_SHP_HW_H15_SCL_M1Y)? PE_H15A_MSC_Y_M1_OFST:\
		(sel == PE_SHP_HW_H15_SCL_M2Y)? PE_H15A_MSC_Y_M2_OFST:\
		PE_H15A_SR_GSC_OFST;
	offset += PE_H15A_SC_H2_OFST;
	switch (co_num)
	{
		case PE_SHP_HW_H15_SCL_CO_NUM_12T_0610:
			PE_SHP_HW_H15A_SET_MSC_FLT(msc_12t_s_flt_0610_h15,offset);
			break;
		case PE_SHP_HW_H15_SCL_CO_NUM_12T_1210:
			PE_SHP_HW_H15A_SET_MSC_FLT(msc_12t_s_flt_1210_h15,offset);
			break;
		default:
			PE_PRINT_NOTI("not support. sel,co:%d,%05d(0x%08x).\n", \
				sel, co_num, offset);
			break;
	}
	return;
}
/**
 * write msc v1 reg
 *
 * @param   co_num [in] UINT32
 * @param   sel [in] UINT32
 * @return  void
 * @see
 * @author
 */
static void PE_SHP_HW_H15_WrMscV1Reg(UINT32 co_num, UINT32 sel)
{
	int ret = RET_OK;
	UINT32 offset;
	offset = (sel == PE_SHP_HW_H15_SCL_M1Y)? PE_H15A_MSC_Y_M1_OFST:\
		(sel == PE_SHP_HW_H15_SCL_M2Y)? PE_H15A_MSC_Y_M2_OFST:\
		(sel == PE_SHP_HW_H15_SCL_M1C)? PE_H15A_MSC_C_M1_OFST:\
		(sel == PE_SHP_HW_H15_SCL_M2C)? PE_H15A_MSC_C_M2_OFST:\
		PE_H15A_SR_GSC_OFST;
	offset += PE_H15A_SC_V1_OFST;
	switch (co_num)
	{
		case PE_SHP_HW_H15_SCL_CO_NUM_06T_0006:
			PE_SHP_HW_H15A_SET_MSC_FLT(msc_06t_m_flt_0006_h15,offset);
			break;
		case PE_SHP_HW_H15_SCL_CO_NUM_06T_0306:
			PE_SHP_HW_H15A_SET_MSC_FLT(msc_06t_m_flt_0306_h15,offset);
			break;
		case PE_SHP_HW_H15_SCL_CO_NUM_12T_0002:
			PE_SHP_HW_H15A_SET_MSC_FLT(msc_12t_m_flt_0002_h15,offset);
			break;
		case PE_SHP_HW_H15_SCL_CO_NUM_12T_0003:
			PE_SHP_HW_H15A_SET_MSC_FLT(msc_12t_m_flt_0003_h15,offset);
			break;
		case PE_SHP_HW_H15_SCL_CO_NUM_12T_0006:
			PE_SHP_HW_H15A_SET_MSC_FLT(msc_12t_m_flt_0006_h15,offset);
			break;
		case PE_SHP_HW_H15_SCL_CO_NUM_12T_0304:
			PE_SHP_HW_H15A_SET_MSC_FLT(msc_12t_m_flt_0304_h15,offset);
			break;
		case PE_SHP_HW_H15_SCL_CO_NUM_12T_0306:
			PE_SHP_HW_H15A_SET_MSC_FLT(msc_12t_m_flt_0306_h15,offset);
			break;
		case PE_SHP_HW_H15_SCL_CO_NUM_12T_0404:
			PE_SHP_HW_H15A_SET_MSC_FLT(msc_12t_m_flt_0404_h15,offset);
			break;
		case PE_SHP_HW_H15_SCL_CO_NUM_12T_0406:
			PE_SHP_HW_H15A_SET_MSC_FLT(msc_12t_m_flt_0406_h15,offset);
			break;
		case PE_SHP_HW_H15_SCL_CO_NUM_12T_0606:
			PE_SHP_HW_H15A_SET_MSC_FLT(msc_12t_m_flt_0606_h15,offset);
			break;
		case PE_SHP_HW_H15_SCL_CO_NUM_12T_0806:
			PE_SHP_HW_H15A_SET_MSC_FLT(msc_12t_m_flt_0806_h15,offset);
			break;
		case PE_SHP_HW_H15_SCL_CO_NUM_12T_1006:
			PE_SHP_HW_H15A_SET_MSC_FLT(msc_12t_m_flt_1006_h15,offset);
			break;
		case PE_SHP_HW_H15_SCL_CO_NUM_12T_1410:
			PE_SHP_HW_H15A_SET_MSC_FLT(msc_12t_m_flt_1410_h15,offset);
			break;
		default:
			PE_PRINT_NOTI("not support. sel,co:%d,%05d(0x%08x).\n", \
				sel, co_num, offset);
			break;
	}
	return;
}
/**
 * write msc v2 reg
 *
 * @param   co_num [in] UINT32
 * @param   sel [in] UINT32
 * @return  void
 * @see
 * @author
 */
static void PE_SHP_HW_H15_WrMscV2Reg(UINT32 co_num, UINT32 sel)
{
	int ret = RET_OK;
	UINT32 offset;
	offset = (sel == PE_SHP_HW_H15_SCL_M1Y)? PE_H15A_MSC_Y_M1_OFST:\
		(sel == PE_SHP_HW_H15_SCL_M2Y)? PE_H15A_MSC_Y_M2_OFST:\
		PE_H15A_SR_GSC_OFST;
	offset += PE_H15A_SC_V2_OFST;
	switch (co_num)
	{
		case PE_SHP_HW_H15_SCL_CO_NUM_12T_0610:
			PE_SHP_HW_H15A_SET_MSC_FLT(msc_12t_s_flt_0610_h15,offset);
			break;
		case PE_SHP_HW_H15_SCL_CO_NUM_12T_1210:
			PE_SHP_HW_H15A_SET_MSC_FLT(msc_12t_s_flt_1210_h15,offset);
			break;
		default:
			PE_PRINT_NOTI("not support. sel,co:%d,%05d(0x%08x).\n", \
				sel, co_num, offset);
			break;
	}
	return;
}
/**
 * get msc h1 coeff num
 *	- h12t main
 *
 * @param   sel [in] UINT32
 * @return  UINT32
 * @see
 * @author
 */
static UINT32 PE_SHP_HW_H15_GetMscH1CoNum(UINT32 sel)
{
	UINT32 co_num;
	PE_SHP_HW_H15_SETTINGS_T *pInfo = &_g_pe_shp_hw_h15_info;
	PE_SHP_HW_H15_SCL_CTRL_T *p_cur = NULL;
	PE_SHP_HW_H15_SCL_CTRL_T *p_pre = NULL;
	p_cur = (sel == PE_SHP_HW_H15_SCL_M1Y)? (&pInfo->cur_info.m1_y):\
		(sel == PE_SHP_HW_H15_SCL_M2Y)? (&pInfo->cur_info.m2_y):\
		(sel == PE_SHP_HW_H15_SCL_M1C)? (&pInfo->cur_info.m1_c):\
		(sel == PE_SHP_HW_H15_SCL_M2C)? (&pInfo->cur_info.m2_c):\
		(&pInfo->cur_info.mp_y);
	p_pre = (sel == PE_SHP_HW_H15_SCL_M1Y)? (&pInfo->pre_info.m1_y):\
		(sel == PE_SHP_HW_H15_SCL_M2Y)? (&pInfo->pre_info.m2_y):\
		(sel == PE_SHP_HW_H15_SCL_M1C)? (&pInfo->pre_info.m1_c):\
		(sel == PE_SHP_HW_H15_SCL_M2C)? (&pInfo->pre_info.m2_c):\
		(&pInfo->pre_info.mp_y);
	if (p_cur->vscl_mode == PE_SHP_HW_H15_SCL_VSC_MD_V02TAP)
	{
		co_num = p_pre->h1_co_num;
	}
	else	//PE_SHP_HW_H15_SCL_VSC_MD_V12TAP,PE_SHP_HW_H15_SCL_VSC_MD_V06TAP
	{
		co_num = (sel==PE_SHP_HW_H15_SCL_M1C||sel==PE_SHP_HW_H15_SCL_M2C)? \
			(PE_SHP_HW_H15_SCL_CO_NUM_12T_BASE+p_cur->h_c_indx):\
			(PE_SHP_HW_H15_SCL_CO_NUM_12T_BASE+p_cur->h_y_indx);
		if (!PE_SHP_HW_H15_ChkValid12tMain(co_num))
			co_num = PE_SHP_HW_H15_SCL_CO_NUM_H12T_M_DFLT;		//default
	}
	return co_num;
}
/**
 * get msc h2 coeff num
 *	- h12t sub
 *
 * @param   sel [in] UINT32
 * @return  UINT32
 * @see
 * @author
 */
static UINT32 PE_SHP_HW_H15_GetMscH2CoNum(UINT32 sel)
{
	UINT32 co_num;
	PE_SHP_HW_H15_SETTINGS_T *pInfo = &_g_pe_shp_hw_h15_info;
	PE_SHP_HW_H15_SCL_CTRL_T *p_cur = NULL;
	PE_SHP_HW_H15_SCL_CTRL_T *p_pre = NULL;
	p_cur = (sel == PE_SHP_HW_H15_SCL_M1Y)? (&pInfo->cur_info.m1_y):\
		(sel == PE_SHP_HW_H15_SCL_M2Y)? (&pInfo->cur_info.m2_y):\
		(&pInfo->cur_info.mp_y);
	p_pre = (sel == PE_SHP_HW_H15_SCL_M1Y)? (&pInfo->pre_info.m1_y):\
		(sel == PE_SHP_HW_H15_SCL_M2Y)? (&pInfo->pre_info.m2_y):\
		(&pInfo->pre_info.mp_y);
	if (p_cur->adaptv_on == PE_SHP_HW_H15_SCL_ADAPTIVE_ON)
	{
		co_num = PE_SHP_HW_H15_SCL_CO_NUM_12T_BASE+p_cur->h_s_indx;
		if (!PE_SHP_HW_H15_ChkValid12tSub(co_num)||sel==PE_SHP_HW_H15_SCL_MPY)
			co_num = PE_SHP_HW_H15_SCL_CO_NUM_H12T_S_DFLT;		//default
	}
	else
	{
		co_num = p_pre->h2_co_num;
	}
	return co_num;
}
/**
 * get msc v1 coeff num
 *	- v12t main, v06t
 *
 * @param   sel [in] UINT32
 * @return  UINT32
 * @see
 * @author
 */
static UINT32 PE_SHP_HW_H15_GetMscV1CoNum(UINT32 sel)
{
	UINT32 co_num;
	PE_SHP_HW_H15_SETTINGS_T *pInfo = &_g_pe_shp_hw_h15_info;
	PE_SHP_HW_H15_SCL_CTRL_T *p_cur = NULL;
	PE_SHP_HW_H15_SCL_CTRL_T *p_pre = NULL;
	p_cur = (sel == PE_SHP_HW_H15_SCL_M1Y)? (&pInfo->cur_info.m1_y):\
		(sel == PE_SHP_HW_H15_SCL_M2Y)? (&pInfo->cur_info.m2_y):\
		(sel == PE_SHP_HW_H15_SCL_M1C)? (&pInfo->cur_info.m1_c):\
		(sel == PE_SHP_HW_H15_SCL_M2C)? (&pInfo->cur_info.m2_c):\
		(&pInfo->cur_info.mp_y);
	p_pre = (sel == PE_SHP_HW_H15_SCL_M1Y)? (&pInfo->pre_info.m1_y):\
		(sel == PE_SHP_HW_H15_SCL_M2Y)? (&pInfo->pre_info.m2_y):\
		(sel == PE_SHP_HW_H15_SCL_M1C)? (&pInfo->pre_info.m1_c):\
		(sel == PE_SHP_HW_H15_SCL_M2C)? (&pInfo->pre_info.m2_c):\
		(&pInfo->pre_info.mp_y);
	if (p_cur->vscl_mode == PE_SHP_HW_H15_SCL_VSC_MD_V02TAP)
	{
		co_num = p_pre->v1_co_num;
	}
	else if (p_cur->vscl_mode == PE_SHP_HW_H15_SCL_VSC_MD_V06TAP)
	{
		co_num = (sel==PE_SHP_HW_H15_SCL_M1C||sel==PE_SHP_HW_H15_SCL_M2C)? \
			(PE_SHP_HW_H15_SCL_CO_NUM_06T_BASE+p_cur->v_c_indx):\
			(PE_SHP_HW_H15_SCL_CO_NUM_06T_BASE+p_cur->v_y_indx);
		if (!PE_SHP_HW_H15_ChkValid06tMain(co_num))
			co_num = PE_SHP_HW_H15_SCL_CO_NUM_V06T_M_DFLT;		//default
	}
	else	//PE_SHP_HW_H15_SCL_VSC_MD_V12TAP
	{
		co_num = (sel==PE_SHP_HW_H15_SCL_M1C||sel==PE_SHP_HW_H15_SCL_M2C)? \
			(PE_SHP_HW_H15_SCL_CO_NUM_12T_BASE+p_cur->v_c_indx):\
			(PE_SHP_HW_H15_SCL_CO_NUM_12T_BASE+p_cur->v_y_indx);
		if (!PE_SHP_HW_H15_ChkValid12tMain(co_num))
			co_num = PE_SHP_HW_H15_SCL_CO_NUM_V12T_M_DFLT;		//default
	}
	return co_num;
}
/**
 * get msc v2 coeff num
 *	- v12t sub
 *
 * @param   sel [in] UINT32
 * @return  UINT32
 * @see
 * @author
 */
static UINT32 PE_SHP_HW_H15_GetMscV2CoNum(UINT32 sel)
{
	UINT32 co_num;
	PE_SHP_HW_H15_SETTINGS_T *pInfo = &_g_pe_shp_hw_h15_info;
	PE_SHP_HW_H15_SCL_CTRL_T *p_cur = NULL;
	PE_SHP_HW_H15_SCL_CTRL_T *p_pre = NULL;
	p_cur = (sel == PE_SHP_HW_H15_SCL_M1Y)? (&pInfo->cur_info.m1_y):\
		(sel == PE_SHP_HW_H15_SCL_M2Y)? (&pInfo->cur_info.m2_y):\
		(&pInfo->cur_info.mp_y);
	p_pre = (sel == PE_SHP_HW_H15_SCL_M1Y)? (&pInfo->pre_info.m1_y):\
		(sel == PE_SHP_HW_H15_SCL_M2Y)? (&pInfo->pre_info.m2_y):\
		(&pInfo->pre_info.mp_y);
	if (p_cur->adaptv_on == PE_SHP_HW_H15_SCL_ADAPTIVE_ON)
	{
		co_num = PE_SHP_HW_H15_SCL_CO_NUM_12T_BASE+p_cur->v_s_indx;
		if (!PE_SHP_HW_H15_ChkValid12tSub(co_num)||sel==PE_SHP_HW_H15_SCL_MPY)
			co_num = PE_SHP_HW_H15_SCL_CO_NUM_V12T_S_DFLT;		//default
	}
	else
	{
		co_num = p_pre->v2_co_num;
	}
	return co_num;
}
/**
 * set msc y m1
 *
 * @param   *pstParams [in] PE_TSK_H15_CFG_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_SHP_HW_H15_SetMscM1Y(PE_TSK_H15_CFG_T *pCfg)
{
	int ret = RET_OK;
	PE_SHP_HW_H15_SETTINGS_T *pInfo = &_g_pe_shp_hw_h15_info;
	PE_SHP_HW_H15_SCL_CTRL_T *p_cur_m1_y = &pInfo->cur_info.m1_y;
	PE_SHP_HW_H15_SCL_CTRL_T *p_pre_m1_y = &pInfo->pre_info.m1_y;
	do{
		CHECK_KNULL(pCfg);
		if(PE_KDRV_VER_H15)
		{
			/* m1_y */
			if (p_pre_m1_y->h_i_size != p_cur_m1_y->h_i_size || \
				p_pre_m1_y->v_i_size != p_cur_m1_y->v_i_size || \
				p_pre_m1_y->h_active != p_cur_m1_y->h_active || \
				p_pre_m1_y->v_active != p_cur_m1_y->v_active || \
				p_pre_m1_y->h_o_size != p_cur_m1_y->h_o_size || \
				p_pre_m1_y->v_o_size != p_cur_m1_y->v_o_size || \
				p_pre_m1_y->in_c_fmt != p_cur_m1_y->in_c_fmt || \
				p_pre_m1_y->pixel_rp != p_cur_m1_y->pixel_rp || \
				p_pre_m1_y->acti_fmt != p_cur_m1_y->acti_fmt || \
				p_pre_m1_y->h_y_indx != p_cur_m1_y->h_y_indx || \
				p_pre_m1_y->v_y_indx != p_cur_m1_y->v_y_indx || \
				p_pre_m1_y->h_s_indx != p_cur_m1_y->h_s_indx || \
				p_pre_m1_y->v_s_indx != p_cur_m1_y->v_s_indx)
			{
				PE_SHP_HW_H15_DBG_PRINT("[m1_y][pre]i,a,o:%04d,%04d,%04d,%04d,%04d,%04d,c,p,a:%d,%d,%d,yid:%04d,%04d,sid:%04d,%04d\n",\
					p_pre_m1_y->h_i_size, p_pre_m1_y->v_i_size, p_pre_m1_y->h_active, p_pre_m1_y->v_active, \
					p_pre_m1_y->h_o_size, p_pre_m1_y->v_o_size, p_pre_m1_y->in_c_fmt, p_pre_m1_y->pixel_rp, p_pre_m1_y->acti_fmt, \
					p_pre_m1_y->h_y_indx, p_pre_m1_y->v_y_indx, p_pre_m1_y->h_s_indx, p_pre_m1_y->v_s_indx);
				PE_SHP_HW_H15_DBG_PRINT("[m1_y][cur]i,a,o:%04d,%04d,%04d,%04d,%04d,%04d,c,p,a:%d,%d,%d,yid:%04d,%04d,sid:%04d,%04d\n",\
					p_cur_m1_y->h_i_size, p_cur_m1_y->v_i_size, p_cur_m1_y->h_active, p_cur_m1_y->v_active, \
					p_cur_m1_y->h_o_size, p_cur_m1_y->v_o_size, p_cur_m1_y->in_c_fmt, p_cur_m1_y->pixel_rp, p_cur_m1_y->acti_fmt, \
					p_cur_m1_y->h_y_indx, p_cur_m1_y->v_y_indx, p_cur_m1_y->h_s_indx, p_cur_m1_y->v_s_indx);
				/* vsc_mode, adaptive_on */
				if (p_cur_m1_y->h_i_size < PE_SHP_HW_H15_SCL_SIZE_MIN)
				{
					p_cur_m1_y->vscl_mode = p_pre_m1_y->vscl_mode;
					p_cur_m1_y->adaptv_on = p_pre_m1_y->adaptv_on;
				}
				else if (p_cur_m1_y->acti_fmt)
				{
					p_cur_m1_y->vscl_mode = PE_SHP_HW_H15_SCL_VSC_MD_V06TAP;
					p_cur_m1_y->adaptv_on = PE_SHP_HW_H15_SCL_ADAPTIVE_OFF;
				}
				else if (p_cur_m1_y->h_i_size < PE_SHP_HW_H15_SCL_SIZE_2K)
				{
					p_cur_m1_y->vscl_mode = PE_SHP_HW_H15_SCL_VSC_MD_V12TAP;
					p_cur_m1_y->adaptv_on = PE_SHP_HW_H15_SCL_ADAPTIVE_ON;
				}
				else
				{
					p_cur_m1_y->vscl_mode = PE_SHP_HW_H15_SCL_VSC_MD_V06TAP;
					p_cur_m1_y->adaptv_on = PE_SHP_HW_H15_SCL_ADAPTIVE_OFF;
				}
				/* bilinear_on */
				if (p_cur_m1_y->h_active < PE_SHP_HW_H15_SCL_SIZE_MIN || \
					p_cur_m1_y->v_active < PE_SHP_HW_H15_SCL_SIZE_MIN || \
					p_cur_m1_y->h_o_size < PE_SHP_HW_H15_SCL_SIZE_MIN || \
					p_cur_m1_y->v_o_size < PE_SHP_HW_H15_SCL_SIZE_MIN)
				{
					p_cur_m1_y->blnear_on = p_pre_m1_y->blnear_on;
				}
				else if (p_cur_m1_y->h_active == p_cur_m1_y->h_o_size && \
					p_cur_m1_y->v_active == p_cur_m1_y->v_o_size)
				{
					p_cur_m1_y->blnear_on = PE_SHP_HW_H15_SCL_BILINEAR;
				}
				else if (p_cur_m1_y->in_c_fmt == PE_SHP_HW_H15_SCL_C_FMT_444 && \
					p_cur_m1_y->pixel_rp != PE_SHP_HW_H15_SCL_PXL_RP_OFF)
				{
					p_cur_m1_y->blnear_on = PE_SHP_HW_H15_SCL_BILINEAR;
				}
				else
				{
					p_cur_m1_y->blnear_on = PE_SHP_HW_H15_SCL_POLYPHASE;
				}
				/* coef_num */
				p_cur_m1_y->h1_co_num = PE_SHP_HW_H15_GetMscH1CoNum(PE_SHP_HW_H15_SCL_M1Y);
				p_cur_m1_y->h2_co_num = PE_SHP_HW_H15_GetMscH2CoNum(PE_SHP_HW_H15_SCL_M1Y);
				p_cur_m1_y->v1_co_num = PE_SHP_HW_H15_GetMscV1CoNum(PE_SHP_HW_H15_SCL_M1Y);
				p_cur_m1_y->v2_co_num = PE_SHP_HW_H15_GetMscV2CoNum(PE_SHP_HW_H15_SCL_M1Y);
				/* wr ctrl reg. */
				if (p_pre_m1_y->vscl_mode != p_cur_m1_y->vscl_mode || \
					p_pre_m1_y->adaptv_on != p_cur_m1_y->adaptv_on || \
					p_pre_m1_y->blnear_on != p_cur_m1_y->blnear_on)
				{
					PE_SHP_HW_H15_DBG_PRINT("[m1_y]vsc,adp,bil: %d,%d,%d -> %d,%d,%d\n",\
						p_pre_m1_y->vscl_mode, p_pre_m1_y->adaptv_on, p_pre_m1_y->blnear_on, \
						p_cur_m1_y->vscl_mode, p_cur_m1_y->adaptv_on, p_cur_m1_y->blnear_on);
					#ifdef PE_SHP_HW_H15_USE_FWS_REG_FOR_FW_MSC_WR
					PE_MSC_Y_H15A0_RdFL(msc_m1_y_ctrl);
					PE_SHP_HW_H15A_SET_SHDW_MSC_Y_TO_SHP(pe1_msc_m1_y_ctrl, msc_m1_y_ctrl);
					PE_SHPL_H15A0_Wr01(pe1_msc_m1_y_ctrl,reg_vsc_mode_m1,p_cur_m1_y->vscl_mode);
					PE_SHPL_H15A0_Wr01(pe1_msc_m1_y_ctrl,reg_adaptive_on_m1,p_cur_m1_y->adaptv_on);
					PE_SHPL_H15A0_Wr01(pe1_msc_m1_y_ctrl,reg_bilinear_on_m1,p_cur_m1_y->blnear_on);
					PE_SHPL_H15A0_WrFL(pe1_msc_m1_y_ctrl);
					#else
					PE_MSC_Y_H15A0_QWr03(msc_m1_y_ctrl,reg_vsc_mode_m1,p_cur_m1_y->vscl_mode, \
                                                       reg_adaptive_on_m1,p_cur_m1_y->adaptv_on, \
                                                       reg_bilinear_on_m1,p_cur_m1_y->blnear_on);
					#endif
					p_pre_m1_y->vscl_mode = p_cur_m1_y->vscl_mode;
					p_pre_m1_y->adaptv_on = p_cur_m1_y->adaptv_on;
					p_pre_m1_y->blnear_on = p_cur_m1_y->blnear_on;
				}
				/* wr coeff */
				if (p_pre_m1_y->h1_co_num != p_cur_m1_y->h1_co_num)
				{
					PE_SHP_HW_H15_DBG_PRINT("[m1_y]h1_co_num: %05d -> %05d\n", \
						p_pre_m1_y->h1_co_num, p_cur_m1_y->h1_co_num);
					PE_SHP_HW_H15_WrMscH1Reg(p_cur_m1_y->h1_co_num, PE_SHP_HW_H15_SCL_M1Y);
					p_pre_m1_y->h1_co_num = p_cur_m1_y->h1_co_num;
				}
				if (p_pre_m1_y->h2_co_num != p_cur_m1_y->h2_co_num)
				{
					PE_SHP_HW_H15_DBG_PRINT("[m1_y]h2_co_num: %05d -> %05d\n", \
						p_pre_m1_y->h2_co_num, p_cur_m1_y->h2_co_num);
					PE_SHP_HW_H15_WrMscH2Reg(p_cur_m1_y->h2_co_num, PE_SHP_HW_H15_SCL_M1Y);
					p_pre_m1_y->h2_co_num = p_cur_m1_y->h2_co_num;
				}
				if (p_pre_m1_y->v1_co_num != p_cur_m1_y->v1_co_num)
				{
					PE_SHP_HW_H15_DBG_PRINT("[m1_y]v1_co_num: %05d -> %05d\n", \
						p_pre_m1_y->v1_co_num, p_cur_m1_y->v1_co_num);
					PE_SHP_HW_H15_WrMscV1Reg(p_cur_m1_y->v1_co_num, PE_SHP_HW_H15_SCL_M1Y);
					p_pre_m1_y->v1_co_num = p_cur_m1_y->v1_co_num;
				}
				if (p_pre_m1_y->v2_co_num != p_cur_m1_y->v2_co_num)
				{
					PE_SHP_HW_H15_DBG_PRINT("[m1_y]v2_co_num: %05d -> %05d\n", \
						p_pre_m1_y->v2_co_num, p_cur_m1_y->v2_co_num);
					PE_SHP_HW_H15_WrMscV2Reg(p_cur_m1_y->v2_co_num, PE_SHP_HW_H15_SCL_M1Y);
					p_pre_m1_y->v2_co_num = p_cur_m1_y->v2_co_num;
				}
				/* backup */
				p_pre_m1_y->h_i_size = p_cur_m1_y->h_i_size;
				p_pre_m1_y->v_i_size = p_cur_m1_y->v_i_size;
				p_pre_m1_y->h_active = p_cur_m1_y->h_active;
				p_pre_m1_y->v_active = p_cur_m1_y->v_active;
				p_pre_m1_y->h_o_size = p_cur_m1_y->h_o_size;
				p_pre_m1_y->v_o_size = p_cur_m1_y->v_o_size;
				p_pre_m1_y->in_c_fmt = p_cur_m1_y->in_c_fmt;
				p_pre_m1_y->pixel_rp = p_cur_m1_y->pixel_rp;
				p_pre_m1_y->acti_fmt = p_cur_m1_y->acti_fmt;
				p_pre_m1_y->h_y_indx = p_cur_m1_y->h_y_indx;
				p_pre_m1_y->v_y_indx = p_cur_m1_y->v_y_indx;
				p_pre_m1_y->h_s_indx = p_cur_m1_y->h_s_indx;
				p_pre_m1_y->v_s_indx = p_cur_m1_y->v_s_indx;
			}
		}
		else
		{
			PE_SHP_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * set msc y m2
 *
 * @param   *pstParams [in] PE_TSK_H15_CFG_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_SHP_HW_H15_SetMscM2Y(PE_TSK_H15_CFG_T *pCfg)
{
	int ret = RET_OK;
	PE_SHP_HW_H15_SETTINGS_T *pInfo = &_g_pe_shp_hw_h15_info;
	PE_SHP_HW_H15_SCL_CTRL_T *p_cur_m2_y = &pInfo->cur_info.m2_y;
	PE_SHP_HW_H15_SCL_CTRL_T *p_pre_m2_y = &pInfo->pre_info.m2_y;
	do{
		CHECK_KNULL(pCfg);
		if(PE_KDRV_VER_H15)
		{
			/* m2_y */
			if (p_pre_m2_y->h_i_size != p_cur_m2_y->h_i_size || \
				p_pre_m2_y->v_i_size != p_cur_m2_y->v_i_size || \
				p_pre_m2_y->h_active != p_cur_m2_y->h_active || \
				p_pre_m2_y->v_active != p_cur_m2_y->v_active || \
				p_pre_m2_y->h_o_size != p_cur_m2_y->h_o_size || \
				p_pre_m2_y->v_o_size != p_cur_m2_y->v_o_size || \
				p_pre_m2_y->in_c_fmt != p_cur_m2_y->in_c_fmt || \
				p_pre_m2_y->pixel_rp != p_cur_m2_y->pixel_rp || \
				p_pre_m2_y->acti_fmt != p_cur_m2_y->acti_fmt || \
				p_pre_m2_y->h_y_indx != p_cur_m2_y->h_y_indx || \
				p_pre_m2_y->v_y_indx != p_cur_m2_y->v_y_indx || \
				p_pre_m2_y->h_s_indx != p_cur_m2_y->h_s_indx || \
				p_pre_m2_y->v_s_indx != p_cur_m2_y->v_s_indx)
			{
				PE_SHP_HW_H15_DBG_PRINT("[m2_y][pre]i,a,o:%04d,%04d,%04d,%04d,%04d,%04d,c,p,a:%d,%d,%d,yid:%04d,%04d,sid:%04d,%04d\n",\
					p_pre_m2_y->h_i_size, p_pre_m2_y->v_i_size, p_pre_m2_y->h_active, p_pre_m2_y->v_active, \
					p_pre_m2_y->h_o_size, p_pre_m2_y->v_o_size, p_pre_m2_y->in_c_fmt, p_pre_m2_y->pixel_rp, p_pre_m2_y->acti_fmt, \
					p_pre_m2_y->h_y_indx, p_pre_m2_y->v_y_indx, p_pre_m2_y->h_s_indx, p_pre_m2_y->v_s_indx);
				PE_SHP_HW_H15_DBG_PRINT("[m2_y][cur]i,a,o:%04d,%04d,%04d,%04d,%04d,%04d,c,p,a:%d,%d,%d,yid:%04d,%04d,sid:%04d,%04d\n",\
					p_cur_m2_y->h_i_size, p_cur_m2_y->v_i_size, p_cur_m2_y->h_active, p_cur_m2_y->v_active, \
					p_cur_m2_y->h_o_size, p_cur_m2_y->v_o_size, p_cur_m2_y->in_c_fmt, p_cur_m2_y->pixel_rp, p_cur_m2_y->acti_fmt, \
					p_cur_m2_y->h_y_indx, p_cur_m2_y->v_y_indx, p_cur_m2_y->h_s_indx, p_cur_m2_y->v_s_indx);
				/* vsc_mode, adaptive_on */
				if (p_cur_m2_y->h_i_size < PE_SHP_HW_H15_SCL_SIZE_MIN)
				{
					p_cur_m2_y->vscl_mode = p_pre_m2_y->vscl_mode;
					p_cur_m2_y->adaptv_on = p_pre_m2_y->adaptv_on;
				}
				else if (p_cur_m2_y->acti_fmt)
				{
					p_cur_m2_y->vscl_mode = PE_SHP_HW_H15_SCL_VSC_MD_V06TAP;
					p_cur_m2_y->adaptv_on = PE_SHP_HW_H15_SCL_ADAPTIVE_OFF;
				}
				else if (p_cur_m2_y->h_i_size < PE_SHP_HW_H15_SCL_SIZE_2K)
				{
					p_cur_m2_y->vscl_mode = PE_SHP_HW_H15_SCL_VSC_MD_V12TAP;
					p_cur_m2_y->adaptv_on = PE_SHP_HW_H15_SCL_ADAPTIVE_ON;
				}
				else
				{
					p_cur_m2_y->vscl_mode = PE_SHP_HW_H15_SCL_VSC_MD_V06TAP;
					p_cur_m2_y->adaptv_on = PE_SHP_HW_H15_SCL_ADAPTIVE_OFF;
				}
				/* bilinear_on */
				if (p_cur_m2_y->h_active < PE_SHP_HW_H15_SCL_SIZE_MIN || \
					p_cur_m2_y->v_active < PE_SHP_HW_H15_SCL_SIZE_MIN || \
					p_cur_m2_y->h_o_size < PE_SHP_HW_H15_SCL_SIZE_MIN || \
					p_cur_m2_y->v_o_size < PE_SHP_HW_H15_SCL_SIZE_MIN)
				{
					p_cur_m2_y->blnear_on = p_pre_m2_y->blnear_on;
				}
				else if (p_cur_m2_y->h_active == p_cur_m2_y->h_o_size && \
					p_cur_m2_y->v_active == p_cur_m2_y->v_o_size)
				{
					p_cur_m2_y->blnear_on = PE_SHP_HW_H15_SCL_BILINEAR;
				}
				else if (p_cur_m2_y->in_c_fmt == PE_SHP_HW_H15_SCL_C_FMT_444 && \
					p_cur_m2_y->pixel_rp != PE_SHP_HW_H15_SCL_PXL_RP_OFF)
				{
					p_cur_m2_y->blnear_on = PE_SHP_HW_H15_SCL_BILINEAR;
				}
				else
				{
					p_cur_m2_y->blnear_on = PE_SHP_HW_H15_SCL_POLYPHASE;
				}
				/* coef_num */
				p_cur_m2_y->h1_co_num = PE_SHP_HW_H15_GetMscH1CoNum(PE_SHP_HW_H15_SCL_M2Y);
				p_cur_m2_y->h2_co_num = PE_SHP_HW_H15_GetMscH2CoNum(PE_SHP_HW_H15_SCL_M2Y);
				p_cur_m2_y->v1_co_num = PE_SHP_HW_H15_GetMscV1CoNum(PE_SHP_HW_H15_SCL_M2Y);
				p_cur_m2_y->v2_co_num = PE_SHP_HW_H15_GetMscV2CoNum(PE_SHP_HW_H15_SCL_M2Y);
				/* wr ctrl reg. */
				if (p_pre_m2_y->vscl_mode != p_cur_m2_y->vscl_mode || \
					p_pre_m2_y->adaptv_on != p_cur_m2_y->adaptv_on || \
					p_pre_m2_y->blnear_on != p_cur_m2_y->blnear_on)
				{
					PE_SHP_HW_H15_DBG_PRINT("[m2_y]vsc,adp,bil: %d,%d,%d -> %d,%d,%d\n",\
						p_pre_m2_y->vscl_mode, p_pre_m2_y->adaptv_on, p_pre_m2_y->blnear_on, \
						p_cur_m2_y->vscl_mode, p_cur_m2_y->adaptv_on, p_cur_m2_y->blnear_on);
					#ifdef PE_SHP_HW_H15_USE_FWS_REG_FOR_FW_MSC_WR
					PE_MSC_Y_H15A0_RdFL(msc_m2_y_ctrl);
					PE_SHP_HW_H15A_SET_SHDW_MSC_Y_TO_SHP(pe1_msc_m2_y_ctrl, msc_m2_y_ctrl);
					PE_SHPL_H15A0_Wr01(pe1_msc_m2_y_ctrl,reg_vsc_mode_m2,p_cur_m2_y->vscl_mode);
					PE_SHPL_H15A0_Wr01(pe1_msc_m2_y_ctrl,reg_adaptive_on_m2,p_cur_m2_y->adaptv_on);
					PE_SHPL_H15A0_Wr01(pe1_msc_m2_y_ctrl,reg_bilinear_on_m2,p_cur_m2_y->blnear_on);
					PE_SHPL_H15A0_WrFL(pe1_msc_m2_y_ctrl);
					#else
					PE_MSC_Y_H15A0_QWr03(msc_m2_y_ctrl,reg_vsc_mode_m2,p_cur_m2_y->vscl_mode, \
                                                       reg_adaptive_on_m2,p_cur_m2_y->adaptv_on, \
                                                       reg_bilinear_on_m2,p_cur_m2_y->blnear_on);
					#endif
					p_pre_m2_y->vscl_mode = p_cur_m2_y->vscl_mode;
					p_pre_m2_y->adaptv_on = p_cur_m2_y->adaptv_on;
					p_pre_m2_y->blnear_on = p_cur_m2_y->blnear_on;
				}
				/* wr coeff */
				if (p_pre_m2_y->h1_co_num != p_cur_m2_y->h1_co_num)
				{
					PE_SHP_HW_H15_DBG_PRINT("[m2_y]h1_co_num: %05d -> %05d\n", \
						p_pre_m2_y->h1_co_num, p_cur_m2_y->h1_co_num);
					PE_SHP_HW_H15_WrMscH1Reg(p_cur_m2_y->h1_co_num, PE_SHP_HW_H15_SCL_M2Y);
					p_pre_m2_y->h1_co_num = p_cur_m2_y->h1_co_num;
				}
				if (p_pre_m2_y->h2_co_num != p_cur_m2_y->h2_co_num)
				{
					PE_SHP_HW_H15_DBG_PRINT("[m2_y]h2_co_num: %05d -> %05d\n", \
						p_pre_m2_y->h2_co_num, p_cur_m2_y->h2_co_num);
					PE_SHP_HW_H15_WrMscH2Reg(p_cur_m2_y->h2_co_num, PE_SHP_HW_H15_SCL_M2Y);
					p_pre_m2_y->h2_co_num = p_cur_m2_y->h2_co_num;
				}
				if (p_pre_m2_y->v1_co_num != p_cur_m2_y->v1_co_num)
				{
					PE_SHP_HW_H15_DBG_PRINT("[m2_y]v1_co_num: %05d -> %05d\n", \
						p_pre_m2_y->v1_co_num, p_cur_m2_y->v1_co_num);
					PE_SHP_HW_H15_WrMscV1Reg(p_cur_m2_y->v1_co_num, PE_SHP_HW_H15_SCL_M2Y);
					p_pre_m2_y->v1_co_num = p_cur_m2_y->v1_co_num;
				}
				if (p_pre_m2_y->v2_co_num != p_cur_m2_y->v2_co_num)
				{
					PE_SHP_HW_H15_DBG_PRINT("[m2_y]v2_co_num: %05d -> %05d\n", \
						p_pre_m2_y->v2_co_num, p_cur_m2_y->v2_co_num);
					PE_SHP_HW_H15_WrMscV2Reg(p_cur_m2_y->v2_co_num, PE_SHP_HW_H15_SCL_M2Y);
					p_pre_m2_y->v2_co_num = p_cur_m2_y->v2_co_num;
				}
				/* backup */
				p_pre_m2_y->h_i_size = p_cur_m2_y->h_i_size;
				p_pre_m2_y->v_i_size = p_cur_m2_y->v_i_size;
				p_pre_m2_y->h_active = p_cur_m2_y->h_active;
				p_pre_m2_y->v_active = p_cur_m2_y->v_active;
				p_pre_m2_y->h_o_size = p_cur_m2_y->h_o_size;
				p_pre_m2_y->v_o_size = p_cur_m2_y->v_o_size;
				p_pre_m2_y->in_c_fmt = p_cur_m2_y->in_c_fmt;
				p_pre_m2_y->pixel_rp = p_cur_m2_y->pixel_rp;
				p_pre_m2_y->acti_fmt = p_cur_m2_y->acti_fmt;
				p_pre_m2_y->h_y_indx = p_cur_m2_y->h_y_indx;
				p_pre_m2_y->v_y_indx = p_cur_m2_y->v_y_indx;
				p_pre_m2_y->h_s_indx = p_cur_m2_y->h_s_indx;
				p_pre_m2_y->v_s_indx = p_cur_m2_y->v_s_indx;
			}
		}
		else
		{
			PE_SHP_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * set msc c m1
 *
 * @param   *pstParams [in] PE_TSK_H15_CFG_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_SHP_HW_H15_SetMscM1C(PE_TSK_H15_CFG_T *pCfg)
{
	int ret = RET_OK;
	PE_SHP_HW_H15_SETTINGS_T *pInfo = &_g_pe_shp_hw_h15_info;
	PE_SHP_HW_H15_SCL_CTRL_T *p_cur_m1_c = &pInfo->cur_info.m1_c;
	PE_SHP_HW_H15_SCL_CTRL_T *p_pre_m1_c = &pInfo->pre_info.m1_c;
	do{
		CHECK_KNULL(pCfg);
		if(PE_KDRV_VER_H15)
		{
			/* m1_c */
			if (p_pre_m1_c->h_i_size != p_cur_m1_c->h_i_size || \
				p_pre_m1_c->v_i_size != p_cur_m1_c->v_i_size || \
				p_pre_m1_c->h_active != p_cur_m1_c->h_active || \
				p_pre_m1_c->v_active != p_cur_m1_c->v_active || \
				p_pre_m1_c->h_o_size != p_cur_m1_c->h_o_size || \
				p_pre_m1_c->v_o_size != p_cur_m1_c->v_o_size || \
				p_pre_m1_c->in_c_fmt != p_cur_m1_c->in_c_fmt || \
				p_pre_m1_c->acti_fmt != p_cur_m1_c->acti_fmt || \
				p_pre_m1_c->h_c_indx != p_cur_m1_c->h_c_indx || \
				p_pre_m1_c->v_c_indx != p_cur_m1_c->v_c_indx)
			{
				PE_SHP_HW_H15_DBG_PRINT("[m1_c][pre]i,a,o:%04d,%04d,%04d,%04d,%04d,%04d,c,p,a:%d,%d,%d,cid:%04d,%04d\n",\
					p_pre_m1_c->h_i_size, p_pre_m1_c->v_i_size, p_pre_m1_c->h_active, p_pre_m1_c->v_active, \
					p_pre_m1_c->h_o_size, p_pre_m1_c->v_o_size, p_pre_m1_c->in_c_fmt, p_pre_m1_c->pixel_rp, \
					p_pre_m1_c->acti_fmt, p_pre_m1_c->h_c_indx, p_pre_m1_c->v_c_indx);
				PE_SHP_HW_H15_DBG_PRINT("[m1_c][cur]i,a,o:%04d,%04d,%04d,%04d,%04d,%04d,c,p,a:%d,%d,%d,cid:%04d,%04d\n",\
					p_cur_m1_c->h_i_size, p_cur_m1_c->v_i_size, p_cur_m1_c->h_active, p_cur_m1_c->v_active, \
					p_cur_m1_c->h_o_size, p_cur_m1_c->v_o_size, p_cur_m1_c->in_c_fmt, p_cur_m1_c->pixel_rp, \
					p_cur_m1_c->acti_fmt, p_cur_m1_c->h_c_indx, p_cur_m1_c->v_c_indx);
				/* vsc_mode */
				if (p_cur_m1_c->h_i_size < PE_SHP_HW_H15_SCL_SIZE_MIN)
				{
					p_cur_m1_c->vscl_mode = p_pre_m1_c->vscl_mode;
				}
				else if (p_cur_m1_c->acti_fmt)
				{
					p_cur_m1_c->vscl_mode = PE_SHP_HW_H15_SCL_VSC_MD_V06TAP;
				}
				else if (p_cur_m1_c->h_i_size < PE_SHP_HW_H15_SCL_SIZE_1K)
				{
					p_cur_m1_c->vscl_mode = PE_SHP_HW_H15_SCL_VSC_MD_V12TAP;
				}
				else if (p_cur_m1_c->h_i_size < PE_SHP_HW_H15_SCL_SIZE_2K)
				{
					p_cur_m1_c->vscl_mode = PE_SHP_HW_H15_SCL_VSC_MD_V06TAP;
				}
				else
				{
					p_cur_m1_c->vscl_mode = PE_SHP_HW_H15_SCL_VSC_MD_V02TAP;
				}
				/* bilinear_on */
				if (p_cur_m1_c->h_active < PE_SHP_HW_H15_SCL_SIZE_MIN || \
					p_cur_m1_c->v_active < PE_SHP_HW_H15_SCL_SIZE_MIN || \
					p_cur_m1_c->h_o_size < PE_SHP_HW_H15_SCL_SIZE_MIN || \
					p_cur_m1_c->v_o_size < PE_SHP_HW_H15_SCL_SIZE_MIN)
				{
					p_cur_m1_c->blnear_on = p_pre_m1_c->blnear_on;
				}
				else if (p_cur_m1_c->h_active == p_cur_m1_c->h_o_size && \
					p_cur_m1_c->v_active == p_cur_m1_c->v_o_size)
				{
					p_cur_m1_c->blnear_on = PE_SHP_HW_H15_SCL_BILINEAR;
				}
				else if (p_cur_m1_c->in_c_fmt == PE_SHP_HW_H15_SCL_C_FMT_444)
				{
					p_cur_m1_c->blnear_on = PE_SHP_HW_H15_SCL_BILINEAR;
				}
				else if (p_cur_m1_c->vscl_mode == PE_SHP_HW_H15_SCL_VSC_MD_V02TAP)
				{
					p_cur_m1_c->blnear_on = PE_SHP_HW_H15_SCL_BILINEAR;
				}
				else
				{
					p_cur_m1_c->blnear_on = PE_SHP_HW_H15_SCL_POLYPHASE;
				}
				/* coef_num */
				p_cur_m1_c->h1_co_num = PE_SHP_HW_H15_GetMscH1CoNum(PE_SHP_HW_H15_SCL_M1C);
				p_cur_m1_c->v1_co_num = PE_SHP_HW_H15_GetMscV1CoNum(PE_SHP_HW_H15_SCL_M1C);
				/* wr ctrl reg. */
				if (p_pre_m1_c->vscl_mode != p_cur_m1_c->vscl_mode || \
					p_pre_m1_c->blnear_on != p_cur_m1_c->blnear_on)
				{
					PE_SHP_HW_H15_DBG_PRINT("[m1_c]vsc,bil: %d,%d -> %d,%d\n",\
						p_pre_m1_c->vscl_mode, p_pre_m1_c->blnear_on, \
						p_cur_m1_c->vscl_mode, p_cur_m1_c->blnear_on);
					#ifdef PE_SHP_HW_H15_USE_FWS_REG_FOR_FW_MSC_WR
					PE_MSC_C_H15A0_RdFL(msc_m1_c_ctrl);
					PE_SHP_HW_H15A_SET_SHDW_MSC_C_TO_SHP(pe1_msc_m1_c_ctrl, msc_m1_c_ctrl);
					PE_SHPL_H15A0_Wr01(pe1_msc_m1_c_ctrl,reg_vsc_mode_m1,p_cur_m1_c->vscl_mode);
					PE_SHPL_H15A0_Wr01(pe1_msc_m1_c_ctrl,reg_bilinear_on_m1,p_cur_m1_c->blnear_on);
					PE_SHPL_H15A0_WrFL(pe1_msc_m1_c_ctrl);
					#else
					PE_MSC_C_H15A0_QWr02(msc_m1_c_ctrl,reg_vsc_mode_m1,p_cur_m1_c->vscl_mode, \
                                                       reg_bilinear_on_m1,p_cur_m1_c->blnear_on);
					#endif
					p_pre_m1_c->vscl_mode = p_cur_m1_c->vscl_mode;
					p_pre_m1_c->blnear_on = p_cur_m1_c->blnear_on;
				}
				/* wr coeff */
				if (p_pre_m1_c->h1_co_num != p_cur_m1_c->h1_co_num)
				{
					PE_SHP_HW_H15_DBG_PRINT("[m1_c]h1_co_num: %05d -> %05d\n", \
						p_pre_m1_c->h1_co_num, p_cur_m1_c->h1_co_num);
					PE_SHP_HW_H15_WrMscH1Reg(p_cur_m1_c->h1_co_num, PE_SHP_HW_H15_SCL_M1C);
					p_pre_m1_c->h1_co_num = p_cur_m1_c->h1_co_num;
				}
				if (p_pre_m1_c->v1_co_num != p_cur_m1_c->v1_co_num)
				{
					PE_SHP_HW_H15_DBG_PRINT("[m1_c]v1_co_num: %05d -> %05d\n", \
						p_pre_m1_c->v1_co_num, p_cur_m1_c->v1_co_num);
					PE_SHP_HW_H15_WrMscV1Reg(p_cur_m1_c->v1_co_num, PE_SHP_HW_H15_SCL_M1C);
					p_pre_m1_c->v1_co_num = p_cur_m1_c->v1_co_num;
				}
				/* backup */
				p_pre_m1_c->h_i_size = p_cur_m1_c->h_i_size;
				p_pre_m1_c->v_i_size = p_cur_m1_c->v_i_size;
				p_pre_m1_c->h_active = p_cur_m1_c->h_active;
				p_pre_m1_c->v_active = p_cur_m1_c->v_active;
				p_pre_m1_c->h_o_size = p_cur_m1_c->h_o_size;
				p_pre_m1_c->v_o_size = p_cur_m1_c->v_o_size;
				p_pre_m1_c->in_c_fmt = p_cur_m1_c->in_c_fmt;
				p_pre_m1_c->acti_fmt = p_cur_m1_c->acti_fmt;
				p_pre_m1_c->h_c_indx = p_cur_m1_c->h_c_indx;
				p_pre_m1_c->v_c_indx = p_cur_m1_c->v_c_indx;
			}
		}
		else
		{
			PE_SHP_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * set msc c m2
 *
 * @param   *pstParams [in] PE_TSK_H15_CFG_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_SHP_HW_H15_SetMscM2C(PE_TSK_H15_CFG_T *pCfg)
{
	int ret = RET_OK;
	PE_SHP_HW_H15_SETTINGS_T *pInfo = &_g_pe_shp_hw_h15_info;
	PE_SHP_HW_H15_SCL_CTRL_T *p_cur_m2_c = &pInfo->cur_info.m2_c;
	PE_SHP_HW_H15_SCL_CTRL_T *p_pre_m2_c = &pInfo->pre_info.m2_c;
	do{
		CHECK_KNULL(pCfg);
		if(PE_KDRV_VER_H15)
		{
			/* m2_c */
			if (p_pre_m2_c->h_i_size != p_cur_m2_c->h_i_size || \
				p_pre_m2_c->v_i_size != p_cur_m2_c->v_i_size || \
				p_pre_m2_c->h_active != p_cur_m2_c->h_active || \
				p_pre_m2_c->v_active != p_cur_m2_c->v_active || \
				p_pre_m2_c->h_o_size != p_cur_m2_c->h_o_size || \
				p_pre_m2_c->v_o_size != p_cur_m2_c->v_o_size || \
				p_pre_m2_c->in_c_fmt != p_cur_m2_c->in_c_fmt || \
				p_pre_m2_c->acti_fmt != p_cur_m2_c->acti_fmt || \
				p_pre_m2_c->h_c_indx != p_cur_m2_c->h_c_indx || \
				p_pre_m2_c->v_c_indx != p_cur_m2_c->v_c_indx)
			{
				PE_SHP_HW_H15_DBG_PRINT("[m2_c][pre]i,a,o:%04d,%04d,%04d,%04d,%04d,%04d,c,p,a:%d,%d,%d,cid:%04d,%04d\n",\
					p_pre_m2_c->h_i_size, p_pre_m2_c->v_i_size, p_pre_m2_c->h_active, p_pre_m2_c->v_active, \
					p_pre_m2_c->h_o_size, p_pre_m2_c->v_o_size, p_pre_m2_c->in_c_fmt, p_pre_m2_c->pixel_rp, \
					p_pre_m2_c->acti_fmt, p_pre_m2_c->h_c_indx, p_pre_m2_c->v_c_indx);
				PE_SHP_HW_H15_DBG_PRINT("[m2_c][cur]i,a,o:%04d,%04d,%04d,%04d,%04d,%04d,c,p,a:%d,%d,%d,cid:%04d,%04d\n",\
					p_cur_m2_c->h_i_size, p_cur_m2_c->v_i_size, p_cur_m2_c->h_active, p_cur_m2_c->v_active, \
					p_cur_m2_c->h_o_size, p_cur_m2_c->v_o_size, p_cur_m2_c->in_c_fmt, p_cur_m2_c->pixel_rp, \
					p_cur_m2_c->acti_fmt, p_cur_m2_c->h_c_indx, p_cur_m2_c->v_c_indx);
				/* vsc_mode */
				if (p_cur_m2_c->h_i_size < PE_SHP_HW_H15_SCL_SIZE_MIN)
				{
					p_cur_m2_c->vscl_mode = p_pre_m2_c->vscl_mode;
				}
				else if (p_cur_m2_c->acti_fmt)
				{
					p_cur_m2_c->vscl_mode = PE_SHP_HW_H15_SCL_VSC_MD_V06TAP;
				}
				else if (p_cur_m2_c->h_i_size < PE_SHP_HW_H15_SCL_SIZE_1K)
				{
					p_cur_m2_c->vscl_mode = PE_SHP_HW_H15_SCL_VSC_MD_V12TAP;
				}
				else if (p_cur_m2_c->h_i_size < PE_SHP_HW_H15_SCL_SIZE_2K)
				{
					p_cur_m2_c->vscl_mode = PE_SHP_HW_H15_SCL_VSC_MD_V06TAP;
				}
				else
				{
					p_cur_m2_c->vscl_mode = PE_SHP_HW_H15_SCL_VSC_MD_V02TAP;
				}
				/* bilinear_on */
				if (p_cur_m2_c->h_active < PE_SHP_HW_H15_SCL_SIZE_MIN || \
					p_cur_m2_c->v_active < PE_SHP_HW_H15_SCL_SIZE_MIN || \
					p_cur_m2_c->h_o_size < PE_SHP_HW_H15_SCL_SIZE_MIN || \
					p_cur_m2_c->v_o_size < PE_SHP_HW_H15_SCL_SIZE_MIN)
				{
					p_cur_m2_c->blnear_on = p_pre_m2_c->blnear_on;
				}
				else if (p_cur_m2_c->h_active == p_cur_m2_c->h_o_size && \
					p_cur_m2_c->v_active == p_cur_m2_c->v_o_size)
				{
					p_cur_m2_c->blnear_on = PE_SHP_HW_H15_SCL_BILINEAR;
				}
				else if (p_cur_m2_c->in_c_fmt == PE_SHP_HW_H15_SCL_C_FMT_444)
				{
					p_cur_m2_c->blnear_on = PE_SHP_HW_H15_SCL_BILINEAR;
				}
				else if (p_cur_m2_c->vscl_mode == PE_SHP_HW_H15_SCL_VSC_MD_V02TAP)
				{
					p_cur_m2_c->blnear_on = PE_SHP_HW_H15_SCL_BILINEAR;
				}
				else
				{
					p_cur_m2_c->blnear_on = PE_SHP_HW_H15_SCL_POLYPHASE;
				}
				/* coef_num */
				p_cur_m2_c->h1_co_num = PE_SHP_HW_H15_GetMscH1CoNum(PE_SHP_HW_H15_SCL_M2C);
				p_cur_m2_c->v1_co_num = PE_SHP_HW_H15_GetMscV1CoNum(PE_SHP_HW_H15_SCL_M2C);
				/* wr ctrl reg. */
				if (p_pre_m2_c->vscl_mode != p_cur_m2_c->vscl_mode || \
					p_pre_m2_c->blnear_on != p_cur_m2_c->blnear_on)
				{
					PE_SHP_HW_H15_DBG_PRINT("[m2_c]vsc,bil: %d,%d -> %d,%d\n",\
						p_pre_m2_c->vscl_mode, p_pre_m2_c->blnear_on, \
						p_cur_m2_c->vscl_mode, p_cur_m2_c->blnear_on);
					#ifdef PE_SHP_HW_H15_USE_FWS_REG_FOR_FW_MSC_WR
					PE_MSC_C_H15A0_RdFL(msc_m2_c_ctrl);
					PE_SHP_HW_H15A_SET_SHDW_MSC_C_TO_SHP(pe1_msc_m2_c_ctrl, msc_m2_c_ctrl);
					PE_SHPL_H15A0_Wr01(pe1_msc_m2_c_ctrl,reg_vsc_mode_m2,p_cur_m2_c->vscl_mode);
					PE_SHPL_H15A0_Wr01(pe1_msc_m2_c_ctrl,reg_bilinear_m2,p_cur_m2_c->blnear_on);
					PE_SHPL_H15A0_WrFL(pe1_msc_m2_c_ctrl);
					#else
					PE_MSC_C_H15A0_QWr02(msc_m2_c_ctrl,reg_vsc_mode_m2,p_cur_m2_c->vscl_mode, \
                                                       reg_bilinear_m2,p_cur_m2_c->blnear_on);
					#endif
					p_pre_m2_c->vscl_mode = p_cur_m2_c->vscl_mode;
					p_pre_m2_c->blnear_on = p_cur_m2_c->blnear_on;
				}
				/* wr coeff */
				if (p_pre_m2_c->h1_co_num != p_cur_m2_c->h1_co_num)
				{
					PE_SHP_HW_H15_DBG_PRINT("[m2_c]h1_co_num: %05d -> %05d\n", \
						p_pre_m2_c->h1_co_num, p_cur_m2_c->h1_co_num);
					PE_SHP_HW_H15_WrMscH1Reg(p_cur_m2_c->h1_co_num, PE_SHP_HW_H15_SCL_M2C);
					p_pre_m2_c->h1_co_num = p_cur_m2_c->h1_co_num;
				}
				if (p_pre_m2_c->v1_co_num != p_cur_m2_c->v1_co_num)
				{
					PE_SHP_HW_H15_DBG_PRINT("[m2_c]v1_co_num: %05d -> %05d\n", \
						p_pre_m2_c->v1_co_num, p_cur_m2_c->v1_co_num);
					PE_SHP_HW_H15_WrMscV1Reg(p_cur_m2_c->v1_co_num, PE_SHP_HW_H15_SCL_M2C);
					p_pre_m2_c->v1_co_num = p_cur_m2_c->v1_co_num;
				}
				/* backup */
				p_pre_m2_c->h_i_size = p_cur_m2_c->h_i_size;
				p_pre_m2_c->v_i_size = p_cur_m2_c->v_i_size;
				p_pre_m2_c->h_active = p_cur_m2_c->h_active;
				p_pre_m2_c->v_active = p_cur_m2_c->v_active;
				p_pre_m2_c->h_o_size = p_cur_m2_c->h_o_size;
				p_pre_m2_c->v_o_size = p_cur_m2_c->v_o_size;
				p_pre_m2_c->in_c_fmt = p_cur_m2_c->in_c_fmt;
				p_pre_m2_c->acti_fmt = p_cur_m2_c->acti_fmt;
				p_pre_m2_c->h_c_indx = p_cur_m2_c->h_c_indx;
				p_pre_m2_c->v_c_indx = p_cur_m2_c->v_c_indx;
			}
		}
		else
		{
			PE_SHP_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * set msc y mp(post)
 *
 * @param   *pstParams [in] PE_TSK_H15_CFG_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_SHP_HW_H15_SetMscMPY(PE_TSK_H15_CFG_T *pCfg)
{
	int ret = RET_OK;
	PE_SHP_HW_H15_SETTINGS_T *pInfo = &_g_pe_shp_hw_h15_info;
	PE_SHP_HW_H15_SCL_CTRL_T *p_cur_mp_y = &pInfo->cur_info.mp_y;
	PE_SHP_HW_H15_SCL_CTRL_T *p_pre_mp_y = &pInfo->pre_info.mp_y;
	#ifdef PE_SHP_HW_H15_SR_PWD_TRACE
	UINT32 r_data[2];
	#endif
	do{
		CHECK_KNULL(pCfg);
		if(PE_KDRV_VER_H15)
		{
			/* mp_y */
			if (p_pre_mp_y->h_i_size != p_cur_mp_y->h_i_size || \
				p_pre_mp_y->v_i_size != p_cur_mp_y->v_i_size || \
				p_pre_mp_y->h_o_size != p_cur_mp_y->h_o_size || \
				p_pre_mp_y->v_o_size != p_cur_mp_y->v_o_size || \
				p_pre_mp_y->in_c_fmt != p_cur_mp_y->in_c_fmt || \
				p_pre_mp_y->pixel_rp != p_cur_mp_y->pixel_rp || \
				p_pre_mp_y->acti_fmt != p_cur_mp_y->acti_fmt || \
				p_pre_mp_y->h_y_indx != p_cur_mp_y->h_y_indx || \
				p_pre_mp_y->v_y_indx != p_cur_mp_y->v_y_indx || \
				p_pre_mp_y->h_s_indx != p_cur_mp_y->h_s_indx || \
				p_pre_mp_y->v_s_indx != p_cur_mp_y->v_s_indx)
			{
				PE_SHP_HW_H15_DBG_PRINT("[m1_y][pre]i,o:%04d,%04d,%04d,%04d,c,p,a:%d,%d,%d,yid:%04d,%04d,sid:%04d,%04d\n",\
					p_pre_mp_y->h_i_size, p_pre_mp_y->v_i_size, \
					p_pre_mp_y->h_o_size, p_pre_mp_y->v_o_size, p_pre_mp_y->in_c_fmt, p_pre_mp_y->pixel_rp, \
					p_pre_mp_y->acti_fmt, p_pre_mp_y->h_y_indx, p_pre_mp_y->v_y_indx, p_pre_mp_y->h_s_indx, p_pre_mp_y->v_s_indx);
				PE_SHP_HW_H15_DBG_PRINT("[m1_y][cur]i,o:%04d,%04d,%04d,%04d,c,p,a:%d,%d,%d,yid:%04d,%04d,sid:%04d,%04d\n",\
					p_cur_mp_y->h_i_size, p_cur_mp_y->v_i_size, \
					p_cur_mp_y->h_o_size, p_cur_mp_y->v_o_size, p_cur_mp_y->in_c_fmt, p_cur_mp_y->pixel_rp, \
					p_cur_mp_y->acti_fmt, p_cur_mp_y->h_y_indx, p_cur_mp_y->v_y_indx, p_cur_mp_y->h_s_indx, p_cur_mp_y->v_s_indx);
				/* vsc_mode, adaptive_on */
				if (p_cur_mp_y->h_i_size < PE_SHP_HW_H15_SCL_SIZE_MIN)
				{
					p_cur_mp_y->vscl_mode = p_pre_mp_y->vscl_mode;
					p_cur_mp_y->adaptv_on = p_pre_mp_y->adaptv_on;
				}
				else if (p_cur_mp_y->acti_fmt)
				{
					p_cur_mp_y->vscl_mode = PE_SHP_HW_H15_SCL_VSC_MD_V06TAP;
					p_cur_mp_y->adaptv_on = PE_SHP_HW_H15_SCL_ADAPTIVE_OFF;
				}
				else if (p_cur_mp_y->h_i_size < PE_SHP_HW_H15_SCL_SIZE_2K)
				{
					p_cur_mp_y->vscl_mode = PE_SHP_HW_H15_SCL_VSC_MD_V12TAP;
					p_cur_mp_y->adaptv_on = PE_SHP_HW_H15_SCL_ADAPTIVE_ON;
				}
				else
				{
					p_cur_mp_y->vscl_mode = PE_SHP_HW_H15_SCL_VSC_MD_V06TAP;
					p_cur_mp_y->adaptv_on = PE_SHP_HW_H15_SCL_ADAPTIVE_OFF;
				}
				/* bilinear_on */
				if (p_cur_mp_y->h_active < PE_SHP_HW_H15_SCL_SIZE_MIN || \
					p_cur_mp_y->v_active < PE_SHP_HW_H15_SCL_SIZE_MIN || \
					p_cur_mp_y->h_o_size < PE_SHP_HW_H15_SCL_SIZE_MIN || \
					p_cur_mp_y->v_o_size < PE_SHP_HW_H15_SCL_SIZE_MIN)
				{
					p_cur_mp_y->blnear_on = p_pre_mp_y->blnear_on;
				}
				else if (p_cur_mp_y->h_i_size == p_cur_mp_y->h_o_size && \
					p_cur_mp_y->v_i_size == p_cur_mp_y->v_o_size)
				{
					p_cur_mp_y->blnear_on = PE_SHP_HW_H15_SCL_BILINEAR;
				}
				else if (p_cur_mp_y->in_c_fmt == PE_SHP_HW_H15_SCL_C_FMT_444 && \
					p_cur_mp_y->pixel_rp != PE_SHP_HW_H15_SCL_PXL_RP_OFF)
				{
					p_cur_mp_y->blnear_on = PE_SHP_HW_H15_SCL_BILINEAR;
				}
				else
				{
					p_cur_mp_y->blnear_on = PE_SHP_HW_H15_SCL_POLYPHASE;
				}
				/* coef_num */
				p_cur_mp_y->h1_co_num = PE_SHP_HW_H15_GetMscH1CoNum(PE_SHP_HW_H15_SCL_MPY);
				p_cur_mp_y->h2_co_num = PE_SHP_HW_H15_GetMscH2CoNum(PE_SHP_HW_H15_SCL_MPY);
				p_cur_mp_y->v1_co_num = PE_SHP_HW_H15_GetMscV1CoNum(PE_SHP_HW_H15_SCL_MPY);
				p_cur_mp_y->v2_co_num = PE_SHP_HW_H15_GetMscV2CoNum(PE_SHP_HW_H15_SCL_MPY);
				#ifdef PE_SHP_HW_H15_SR_PWD_TRACE
				CTOP_CTRL_H15A0_RdFL(SR,ctr00);
				CTOP_CTRL_H15A0_Rd01(SR,ctr00,cg_sr_en,r_data[0]);
				CTOP_CTRL_H15A0_Rd01(SR,ctr00,cg_shs_en,r_data[1]);
				PE_SHP_HW_H15_DBG_SRPD_TRACE("S cg sr,shs:%d,%d\n",r_data[0],r_data[1]);
				#endif
				/* wr ctrl reg. */
				if (p_pre_mp_y->vscl_mode != p_cur_mp_y->vscl_mode || \
					p_pre_mp_y->adaptv_on != p_cur_mp_y->adaptv_on || \
					p_pre_mp_y->blnear_on != p_cur_mp_y->blnear_on)
				{
					#ifdef PE_SHP_HW_H15_SR_PWD_TRACE
					PE_SHP_HW_H15_DBG_SRPD_TRACE("[mp_y]vsc,adp,bil: %d,%d,%d -> %d,%d,%d\n",\
						p_pre_mp_y->vscl_mode, p_pre_mp_y->adaptv_on, p_pre_mp_y->blnear_on, \
						p_cur_mp_y->vscl_mode, p_cur_mp_y->adaptv_on, p_cur_mp_y->blnear_on);
					#endif
					PE_SHP_HW_H15_DBG_PRINT("[mp_y]vsc,adp,bil: %d,%d,%d -> %d,%d,%d\n",\
						p_pre_mp_y->vscl_mode, p_pre_mp_y->adaptv_on, p_pre_mp_y->blnear_on, \
						p_cur_mp_y->vscl_mode, p_cur_mp_y->adaptv_on, p_cur_mp_y->blnear_on);
					PE_SR_GSC_H15A0_QWr03(sr_gsc_y_ctrl,reg_vsc_mode_m1,p_cur_mp_y->vscl_mode, \
                                                        reg_adaptive_on_m1,p_cur_mp_y->adaptv_on, \
                                                        reg_bilinear_on_m1,p_cur_mp_y->blnear_on);
					p_pre_mp_y->vscl_mode = p_cur_mp_y->vscl_mode;
					p_pre_mp_y->adaptv_on = p_cur_mp_y->adaptv_on;
					p_pre_mp_y->blnear_on = p_cur_mp_y->blnear_on;
				}
				/* wr coeff */
				if (p_pre_mp_y->h1_co_num != p_cur_mp_y->h1_co_num)
				{
					#ifdef PE_SHP_HW_H15_SR_PWD_TRACE
					PE_SHP_HW_H15_DBG_SRPD_TRACE("[mp_y]h1_co_num: %05d -> %05d\n", \
						p_pre_mp_y->h1_co_num, p_cur_mp_y->h1_co_num);
					#endif
					PE_SHP_HW_H15_DBG_PRINT("[mp_y]h1_co_num: %05d -> %05d\n", \
						p_pre_mp_y->h1_co_num, p_cur_mp_y->h1_co_num);
					PE_SHP_HW_H15_WrMscH1Reg(p_cur_mp_y->h1_co_num, PE_SHP_HW_H15_SCL_MPY);
					p_pre_mp_y->h1_co_num = p_cur_mp_y->h1_co_num;
				}
				if (p_pre_mp_y->h2_co_num != p_cur_mp_y->h2_co_num)
				{
					#ifdef PE_SHP_HW_H15_SR_PWD_TRACE
					PE_SHP_HW_H15_DBG_SRPD_TRACE("[mp_y]h2_co_num: %05d -> %05d\n", \
						p_pre_mp_y->h2_co_num, p_cur_mp_y->h2_co_num);
					#endif
					PE_SHP_HW_H15_DBG_PRINT("[mp_y]h2_co_num: %05d -> %05d\n", \
						p_pre_mp_y->h2_co_num, p_cur_mp_y->h2_co_num);
					PE_SHP_HW_H15_WrMscH2Reg(p_cur_mp_y->h2_co_num, PE_SHP_HW_H15_SCL_MPY);
					p_pre_mp_y->h2_co_num = p_cur_mp_y->h2_co_num;
				}
				if (p_pre_mp_y->v1_co_num != p_cur_mp_y->v1_co_num)
				{
					#ifdef PE_SHP_HW_H15_SR_PWD_TRACE
					PE_SHP_HW_H15_DBG_SRPD_TRACE("[mp_y]v1_co_num: %05d -> %05d\n", \
						p_pre_mp_y->v1_co_num, p_cur_mp_y->v1_co_num);
					#endif
					PE_SHP_HW_H15_DBG_PRINT("[mp_y]v1_co_num: %05d -> %05d\n", \
						p_pre_mp_y->v1_co_num, p_cur_mp_y->v1_co_num);
					PE_SHP_HW_H15_WrMscV1Reg(p_cur_mp_y->v1_co_num, PE_SHP_HW_H15_SCL_MPY);
					p_pre_mp_y->v1_co_num = p_cur_mp_y->v1_co_num;
				}
				if (p_pre_mp_y->v2_co_num != p_cur_mp_y->v2_co_num)
				{
					#ifdef PE_SHP_HW_H15_SR_PWD_TRACE
					PE_SHP_HW_H15_DBG_SRPD_TRACE("[mp_y]v2_co_num: %05d -> %05d\n", \
						p_pre_mp_y->v2_co_num, p_cur_mp_y->v2_co_num);
					#endif
					PE_SHP_HW_H15_DBG_PRINT("[mp_y]v2_co_num: %05d -> %05d\n", \
						p_pre_mp_y->v2_co_num, p_cur_mp_y->v2_co_num);
					PE_SHP_HW_H15_WrMscV2Reg(p_cur_mp_y->v2_co_num, PE_SHP_HW_H15_SCL_MPY);
					p_pre_mp_y->v2_co_num = p_cur_mp_y->v2_co_num;
				}
				#ifdef PE_SHP_HW_H15_SR_PWD_TRACE
				CTOP_CTRL_H15A0_RdFL(SR,ctr00);
				CTOP_CTRL_H15A0_Rd01(SR,ctr00,cg_sr_en,r_data[0]);
				CTOP_CTRL_H15A0_Rd01(SR,ctr00,cg_shs_en,r_data[1]);
				PE_SHP_HW_H15_DBG_SRPD_TRACE("E cg sr,shs:%d,%d\n",r_data[0],r_data[1]);
				#endif
				/* backup */
				p_pre_mp_y->h_i_size = p_cur_mp_y->h_i_size;
				p_pre_mp_y->v_i_size = p_cur_mp_y->v_i_size;
				p_pre_mp_y->h_active = p_cur_mp_y->h_active;
				p_pre_mp_y->v_active = p_cur_mp_y->v_active;
				p_pre_mp_y->h_o_size = p_cur_mp_y->h_o_size;
				p_pre_mp_y->v_o_size = p_cur_mp_y->v_o_size;
				p_pre_mp_y->in_c_fmt = p_cur_mp_y->in_c_fmt;
				p_pre_mp_y->pixel_rp = p_cur_mp_y->pixel_rp;
				p_pre_mp_y->acti_fmt = p_cur_mp_y->acti_fmt;
				p_pre_mp_y->h_y_indx = p_cur_mp_y->h_y_indx;
				p_pre_mp_y->v_y_indx = p_cur_mp_y->v_y_indx;
				p_pre_mp_y->h_s_indx = p_cur_mp_y->h_s_indx;
				p_pre_mp_y->v_s_indx = p_cur_mp_y->v_s_indx;
			}
		}
		else
		{
			PE_SHP_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}

/**
 * get current shp settings
 *
 * @param   *pstParams [in] PE_SHP_HW_H15_SETTINGS_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_SHP_HW_H15_GetCurShpSettings(PE_SHP_HW_H15_SETTINGS_T *pstParams)
{
	int ret = RET_OK;
	static UINT32 count = 0;
	PE_SHP_HW_H15_SETTINGS_T *pInfo = &_g_pe_shp_hw_h15_info;
	#ifndef PE_SHP_HW_H15_SR_PWD_TRACE
	PE_SHP_HW_H15_SCL_INDX_T *p_idx = &pInfo->user_gsc_idx;
	#endif
	do{
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H15)
		{
			memcpy(pstParams,pInfo,sizeof(PE_SHP_HW_H15_SETTINGS_T));
			if(_g_shp_hw_h15_trace)
			{
				if(count>PE_PRINT_COUNT_NUM)
				{
					PE_SHP_HW_H15_DBG_PRINT("get idx : h_y,v_y,h_c,v_c:%d,%d,%d,%d\n",\
						p_idx->h_y_indx, p_idx->h_c_indx, p_idx->h_c_indx, p_idx->v_c_indx);
					count=0;
				}
				count++;
			}
		}
		else
		{
			PE_SHP_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
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
int PE_SHP_HW_H15_RdSclFltCoeff(LX_PE_ETC_TBL_T *pstParams)
{
	int ret = RET_OK;
	char buffer[PE_TRACE_STR_SIZE];
	UINT32 data[6];
	static UINT32 rd_cnt = 0;
	static UINT32 rd_ready = 0;
	static UINT32 scl_fw_en = 0;
	static UINT32 v_o_size = 0;
	PE_SHP_HW_H15_SETTINGS_T *pInfo = &_g_pe_shp_hw_h15_info;
	PE_SHP_HW_H15_SCL_INDX_T *p_gsc_idx = &(pInfo->user_gsc_idx);
	PE_SHP_HW_H15_SCL_INDX_T *p_psc_idx = &(pInfo->user_psc_idx);
	PE_SHP_HW_H15_SCL_INDX_T *p_sub_idx = &(pInfo->user_sub_idx);
	PE_SHP_HW_H15_SCL_CTRL_T *p_m1_y = &(pInfo->cur_info.m1_y);
	PE_SHP_HW_H15_SCL_CTRL_T *p_m2_y = &(pInfo->cur_info.m2_y);
	PE_SHP_HW_H15_SCL_CTRL_T *p_m1_c = &(pInfo->cur_info.m1_c);
	PE_SHP_HW_H15_SCL_CTRL_T *p_m2_c = &(pInfo->cur_info.m2_c);
	PE_SHP_HW_H15_SCL_CTRL_T *p_mp_y = &(pInfo->cur_info.mp_y);

#define PE_SHP_H15_PRINT_START		"START_OF_PRINT"
#define PE_SHP_H15_PRINT_END		"END_OF_PRINT"
#define PE_SHP_H15_PRINT_EXIT		"EXIT_OF_PRINT"
#define PE_SHP_H15_PRINT_RESRV		"RESERVED"
#define PE_SHP_H15_CASE_PRMENU(_cnt, _action, _buf, _item)	\
	case (_cnt):sprintf(_buf, "   [0x%02x] %s\n", (_item), #_item);_action;break
#define PE_SHP_H15_CASE_SPRINT(_cnt, _action, _buf, fmt, args...)	\
	case (_cnt):sprintf(_buf, fmt, ##args);_action;break
#define PE_SHP_H15_DFLT_SPRINT(_action, _buf, fmt, args...)		\
	default:sprintf(_buf, fmt, ##args);_action;break

	do{
		CHECK_KNULL(pstParams);
		CHECK_KNULL(pstParams->data);
		if(PE_KDRV_VER_H15)
		{
			if (pstParams->data[0]>=PE_SHP_HW_H15_RD_SCL_MAX)
			{
				sprintf(buffer, PE_SHP_H15_PRINT_EXIT);
				rd_cnt=0;
			}
			else if (pstParams->data[0]==PE_SHP_HW_H15_RD_SCL_DISP)
			{
				switch (rd_cnt)
				{
					PE_SHP_H15_CASE_SPRINT( 0,rd_cnt++,buffer,PE_SHP_H15_PRINT_START);
					PE_SHP_H15_CASE_PRMENU( 1,rd_cnt++,buffer,PE_SHP_HW_H15_RD_SCL_INFO);
					PE_SHP_H15_CASE_PRMENU( 2,rd_cnt++,buffer,PE_SHP_HW_H15_RD_SCL_DRV_ON);
					PE_SHP_H15_CASE_PRMENU( 3,rd_cnt++,buffer,PE_SHP_HW_H15_RD_SCL_DRV_OFF);
					PE_SHP_H15_CASE_PRMENU( 4,rd_cnt++,buffer,PE_SHP_HW_H15_RD_RDY_RD_M1Y);
					PE_SHP_H15_CASE_PRMENU( 5,rd_cnt++,buffer,PE_SHP_HW_H15_RD_RDY_RD_M2Y);
					PE_SHP_H15_CASE_PRMENU( 6,rd_cnt++,buffer,PE_SHP_HW_H15_RD_RDY_RD_M1C);
					PE_SHP_H15_CASE_PRMENU( 7,rd_cnt++,buffer,PE_SHP_HW_H15_RD_RDY_RD_M2C);
					PE_SHP_H15_CASE_PRMENU( 8,rd_cnt++,buffer,PE_SHP_HW_H15_RD_RDY_RD_P1Y);
					PE_SHP_H15_CASE_SPRINT( 9,rd_cnt++,buffer,PE_SHP_H15_PRINT_RESRV);
					PE_SHP_H15_CASE_PRMENU(10,rd_cnt++,buffer,PE_SHP_HW_H15_RD_SCL_M1Y_H1);
					PE_SHP_H15_CASE_PRMENU(11,rd_cnt++,buffer,PE_SHP_HW_H15_RD_SCL_M1Y_H2);
					PE_SHP_H15_CASE_PRMENU(12,rd_cnt++,buffer,PE_SHP_HW_H15_RD_SCL_M1Y_V1);
					PE_SHP_H15_CASE_PRMENU(13,rd_cnt++,buffer,PE_SHP_HW_H15_RD_SCL_M1Y_V2);
					PE_SHP_H15_CASE_PRMENU(14,rd_cnt++,buffer,PE_SHP_HW_H15_RD_SCL_M2Y_H1);
					PE_SHP_H15_CASE_PRMENU(15,rd_cnt++,buffer,PE_SHP_HW_H15_RD_SCL_M2Y_H2);
					PE_SHP_H15_CASE_PRMENU(16,rd_cnt++,buffer,PE_SHP_HW_H15_RD_SCL_M2Y_V1);
					PE_SHP_H15_CASE_PRMENU(17,rd_cnt++,buffer,PE_SHP_HW_H15_RD_SCL_M2Y_V2);
					PE_SHP_H15_CASE_PRMENU(18,rd_cnt++,buffer,PE_SHP_HW_H15_RD_SCL_M1C_H1);
					PE_SHP_H15_CASE_PRMENU(19,rd_cnt++,buffer,PE_SHP_HW_H15_RD_SCL_M1C_V1);
					PE_SHP_H15_CASE_PRMENU(20,rd_cnt++,buffer,PE_SHP_HW_H15_RD_SCL_M2C_H1);
					PE_SHP_H15_CASE_PRMENU(21,rd_cnt++,buffer,PE_SHP_HW_H15_RD_SCL_M2C_V1);
					PE_SHP_H15_CASE_PRMENU(22,rd_cnt++,buffer,PE_SHP_HW_H15_RD_SCL_P1Y_H1);
					PE_SHP_H15_CASE_PRMENU(23,rd_cnt++,buffer,PE_SHP_HW_H15_RD_SCL_P1Y_H2);
					PE_SHP_H15_CASE_PRMENU(24,rd_cnt++,buffer,PE_SHP_HW_H15_RD_SCL_P1Y_V1);
					PE_SHP_H15_CASE_PRMENU(25,rd_cnt++,buffer,PE_SHP_HW_H15_RD_SCL_P1Y_V2);
					PE_SHP_H15_CASE_SPRINT(26,rd_cnt++,buffer,PE_SHP_H15_PRINT_RESRV);
					PE_SHP_H15_CASE_SPRINT(27,rd_cnt++,buffer,PE_SHP_H15_PRINT_RESRV);
					PE_SHP_H15_CASE_SPRINT(28,rd_cnt++,buffer,PE_SHP_H15_PRINT_RESRV);
					PE_SHP_H15_CASE_SPRINT(29,rd_cnt++,buffer,PE_SHP_H15_PRINT_RESRV);
					PE_SHP_H15_DFLT_SPRINT(rd_cnt=0, buffer, PE_SHP_H15_PRINT_END);
				}
			}
			else if (pstParams->data[0]==PE_SHP_HW_H15_RD_SCL_INFO)
			{
				switch(rd_cnt)
				{
					PE_SHP_H15_CASE_SPRINT( 0,rd_cnt++,buffer,PE_SHP_H15_PRINT_START);
					PE_SHP_H15_CASE_SPRINT( 1,rd_cnt++,buffer,\
						"   -----------------------------------------------------------\n");
					PE_SHP_H15_CASE_SPRINT( 2,rd_cnt++,buffer,\
						"   scl drv : %s\n", \
						PE_CMN_HW_H15_GetDbInfo(PE_CMN_HW_H15_DB_INF_MD0_SCL_FW_EN, 0)? "OFF":"ON");
					PE_SHP_H15_CASE_SPRINT( 3,rd_cnt++,buffer,\
						"   -----------------------------------------------------------\n");
					PE_SHP_H15_CASE_SPRINT( 4,rd_cnt++,buffer,\
						"   gsc     : h_y,v_y,h_c,v_c:%04d,%04d,%04d,%04d\n", \
						p_gsc_idx->h_y_indx, p_gsc_idx->v_y_indx, p_gsc_idx->h_c_indx, p_gsc_idx->v_c_indx);
					PE_SHP_H15_CASE_SPRINT( 5,rd_cnt++,buffer,\
						"   psc,sub : h_y,v_y:%04d,%04d, h_y,v_y:%04d,%04d\n", \
						p_psc_idx->h_y_indx, p_psc_idx->v_y_indx, p_sub_idx->h_y_indx, p_sub_idx->v_y_indx);
					PE_SHP_H15_CASE_SPRINT( 6,rd_cnt++,buffer,PE_SHP_H15_PRINT_RESRV);
					PE_SHP_H15_CASE_SPRINT( 7,rd_cnt++,buffer,PE_SHP_H15_PRINT_RESRV);
					PE_SHP_H15_CASE_SPRINT( 8,rd_cnt++,buffer,PE_SHP_H15_PRINT_RESRV);
					PE_SHP_H15_CASE_SPRINT( 9,rd_cnt++,buffer,\
						"   -----------------------------------------------------------\n");
					PE_SHP_H15_CASE_SPRINT(10,rd_cnt++,buffer,\
						"   m1_y : indx: m:%04d,%04d, s:%04d,%04d\n", \
						p_m1_y->h_y_indx, p_m1_y->v_y_indx, p_m1_y->h_s_indx, p_m1_y->v_s_indx);
					PE_SHP_H15_CASE_SPRINT(11,rd_cnt++,buffer,\
						"   m1_y : size i,a,o: %4d,%4d, %4d,%4d, %4d,%4d\n", \
						p_m1_y->h_i_size, p_m1_y->v_i_size, p_m1_y->h_active, p_m1_y->v_active, \
						p_m1_y->h_o_size, p_m1_y->v_o_size);
					PE_SHP_H15_CASE_SPRINT(12,rd_cnt++,buffer,\
						"   m1_y : in_c:%d,rp:%d,af:%d, vsc_md:%d,adp:%d,bi:%d\n", \
						p_m1_y->in_c_fmt, p_m1_y->pixel_rp, p_m1_y->acti_fmt, p_m1_y->vscl_mode, \
						p_m1_y->adaptv_on, p_m1_y->blnear_on);
					PE_SHP_H15_CASE_SPRINT(13,rd_cnt++,buffer,\
						"   m1_y : co_mun h1,h2, v1,v2: %5d,%5d, %5d,%5d\n", \
						p_m1_y->h1_co_num, p_m1_y->h2_co_num, p_m1_y->v1_co_num, p_m1_y->v2_co_num);
					PE_SHP_H15_CASE_SPRINT(14,rd_cnt++,buffer,PE_SHP_H15_PRINT_RESRV);
					PE_SHP_H15_CASE_SPRINT(15,rd_cnt++,buffer,PE_SHP_H15_PRINT_RESRV);
					PE_SHP_H15_CASE_SPRINT(16,rd_cnt++,buffer,PE_SHP_H15_PRINT_RESRV);
					PE_SHP_H15_CASE_SPRINT(17,rd_cnt++,buffer,PE_SHP_H15_PRINT_RESRV);
					PE_SHP_H15_CASE_SPRINT(18,rd_cnt++,buffer,PE_SHP_H15_PRINT_RESRV);
					PE_SHP_H15_CASE_SPRINT(19,rd_cnt++,buffer,\
						"   -----------------------------------------------------------\n");
					PE_SHP_H15_CASE_SPRINT(20,rd_cnt++,buffer,\
						"   m2_y : indx: m:%04d,%04d, s:%04d,%04d\n", \
						p_m2_y->h_y_indx, p_m2_y->v_y_indx, p_m2_y->h_s_indx, p_m2_y->v_s_indx);
					PE_SHP_H15_CASE_SPRINT(21,rd_cnt++,buffer,\
						"   m2_y : size i,a,o: %4d,%4d, %4d,%4d, %4d,%4d\n", \
						p_m2_y->h_i_size, p_m2_y->v_i_size, p_m2_y->h_active, p_m2_y->v_active, \
						p_m2_y->h_o_size, p_m2_y->v_o_size);
					PE_SHP_H15_CASE_SPRINT(22,rd_cnt++,buffer,\
						"   m2_y : in_c:%d,rp:%d,af:%d, vsc_md:%d,adp:%d,bi:%d\n", \
						p_m2_y->in_c_fmt, p_m2_y->pixel_rp, p_m2_y->acti_fmt, p_m2_y->vscl_mode, \
						p_m2_y->adaptv_on, p_m2_y->blnear_on);
					PE_SHP_H15_CASE_SPRINT(23,rd_cnt++,buffer,\
						"   m2_y : co_mun h1,h2, v1,v2: %5d,%5d, %5d,%5d\n", \
						p_m2_y->h1_co_num, p_m2_y->h2_co_num, p_m2_y->v1_co_num, p_m2_y->v2_co_num);
					PE_SHP_H15_CASE_SPRINT(24,rd_cnt++,buffer,PE_SHP_H15_PRINT_RESRV);
					PE_SHP_H15_CASE_SPRINT(25,rd_cnt++,buffer,PE_SHP_H15_PRINT_RESRV);
					PE_SHP_H15_CASE_SPRINT(26,rd_cnt++,buffer,PE_SHP_H15_PRINT_RESRV);
					PE_SHP_H15_CASE_SPRINT(27,rd_cnt++,buffer,PE_SHP_H15_PRINT_RESRV);
					PE_SHP_H15_CASE_SPRINT(28,rd_cnt++,buffer,PE_SHP_H15_PRINT_RESRV);
					PE_SHP_H15_CASE_SPRINT(29,rd_cnt++,buffer,\
						"   -----------------------------------------------------------\n");
					PE_SHP_H15_CASE_SPRINT(30,rd_cnt++,buffer,\
						"   m1_c : indx: %04d,%04d\n", p_m1_c->h_c_indx, p_m1_c->v_c_indx);
					PE_SHP_H15_CASE_SPRINT(31,rd_cnt++,buffer,\
						"   m1_c : size i,a,o: %4d,%4d, %4d,%4d, %4d,%4d\n", \
						p_m1_c->h_i_size, p_m1_c->v_i_size, p_m1_c->h_active, p_m1_c->v_active, \
						p_m1_c->h_o_size, p_m1_c->v_o_size);
					PE_SHP_H15_CASE_SPRINT(32,rd_cnt++,buffer,\
						"   m1_c : in_c:%d,rp:%d,af:%d, vsc_md:%d, adp:%d, bi:%d\n", \
						p_m1_c->in_c_fmt, p_m1_c->pixel_rp, p_m1_c->acti_fmt, p_m1_c->vscl_mode, \
						p_m1_c->adaptv_on, p_m1_c->blnear_on);
					PE_SHP_H15_CASE_SPRINT(33,rd_cnt++,buffer,\
						"   m1_c : co_mun h1,h2, v1,v2: %5d,%5d, %5d,%5d\n", \
						p_m1_c->h1_co_num, p_m1_c->h2_co_num, p_m1_c->v1_co_num, p_m1_c->v2_co_num);
					PE_SHP_H15_CASE_SPRINT(34,rd_cnt++,buffer,PE_SHP_H15_PRINT_RESRV);
					PE_SHP_H15_CASE_SPRINT(35,rd_cnt++,buffer,PE_SHP_H15_PRINT_RESRV);
					PE_SHP_H15_CASE_SPRINT(36,rd_cnt++,buffer,PE_SHP_H15_PRINT_RESRV);
					PE_SHP_H15_CASE_SPRINT(37,rd_cnt++,buffer,PE_SHP_H15_PRINT_RESRV);
					PE_SHP_H15_CASE_SPRINT(38,rd_cnt++,buffer,PE_SHP_H15_PRINT_RESRV);
					PE_SHP_H15_CASE_SPRINT(39,rd_cnt++,buffer,\
						"   -----------------------------------------------------------\n");
					PE_SHP_H15_CASE_SPRINT(40,rd_cnt++,buffer,\
						"   m2_c : indx: %04d,%04d\n", p_m2_c->h_c_indx, p_m2_c->v_c_indx);
					PE_SHP_H15_CASE_SPRINT(41,rd_cnt++,buffer,\
						"   m2_c : size i,a,o: %4d,%4d, %4d,%4d, %4d,%4d\n", \
						p_m2_c->h_i_size, p_m2_c->v_i_size, p_m2_c->h_active, p_m2_c->v_active, \
						p_m2_c->h_o_size, p_m2_c->v_o_size);
					PE_SHP_H15_CASE_SPRINT(42,rd_cnt++,buffer,\
						"   m2_c : in_c:%d,rp:%d,af:%d, vsc_md:%d, adp:%d, bi:%d\n", \
						p_m2_c->in_c_fmt, p_m2_c->pixel_rp, p_m2_c->acti_fmt, p_m2_c->vscl_mode, \
						p_m2_c->adaptv_on, p_m2_c->blnear_on);
					PE_SHP_H15_CASE_SPRINT(43,rd_cnt++,buffer,\
						"   m2_c : co_mun h1,h2, v1,v2: %5d,%5d, %5d,%5d\n", \
						p_m2_c->h1_co_num, p_m2_c->h2_co_num, p_m2_c->v1_co_num, p_m2_c->v2_co_num);
					PE_SHP_H15_CASE_SPRINT(44,rd_cnt++,buffer,PE_SHP_H15_PRINT_RESRV);
					PE_SHP_H15_CASE_SPRINT(45,rd_cnt++,buffer,PE_SHP_H15_PRINT_RESRV);
					PE_SHP_H15_CASE_SPRINT(46,rd_cnt++,buffer,PE_SHP_H15_PRINT_RESRV);
					PE_SHP_H15_CASE_SPRINT(47,rd_cnt++,buffer,PE_SHP_H15_PRINT_RESRV);
					PE_SHP_H15_CASE_SPRINT(48,rd_cnt++,buffer,PE_SHP_H15_PRINT_RESRV);
					PE_SHP_H15_CASE_SPRINT(49,rd_cnt++,buffer,\
						"   -----------------------------------------------------------\n");
					PE_SHP_H15_CASE_SPRINT(50,rd_cnt++,buffer,\
						"   mp_y : indx: m:%04d,%04d, s:%04d,%04d\n", \
						p_mp_y->h_y_indx, p_mp_y->v_y_indx, p_mp_y->h_s_indx, p_mp_y->v_s_indx);
					PE_SHP_H15_CASE_SPRINT(51,rd_cnt++,buffer,\
						"   mp_y : size i,a,o: %4d,%4d, %4d,%4d, %4d,%4d\n", \
						p_mp_y->h_i_size, p_mp_y->v_i_size, p_mp_y->h_active, p_mp_y->v_active, \
						p_mp_y->h_o_size, p_mp_y->v_o_size);
					PE_SHP_H15_CASE_SPRINT(52,rd_cnt++,buffer,\
						"   mp_y : in_c:%d,rp:%d,af:%d, vsc_md:%d, adp:%d, bi:%d\n", \
						p_mp_y->in_c_fmt, p_mp_y->pixel_rp, p_mp_y->acti_fmt, p_mp_y->vscl_mode, \
						p_mp_y->adaptv_on, p_mp_y->blnear_on);
					PE_SHP_H15_CASE_SPRINT(53,rd_cnt++,buffer,\
						"   mp_y : co_mun h1,h2, v1,v2: %5d,%5d, %5d,%5d\n", \
						p_mp_y->h1_co_num, p_mp_y->h2_co_num, p_mp_y->v1_co_num, p_mp_y->v2_co_num);
					PE_SHP_H15_CASE_SPRINT(54,rd_cnt++,buffer,PE_SHP_H15_PRINT_RESRV);
					PE_SHP_H15_CASE_SPRINT(55,rd_cnt++,buffer,PE_SHP_H15_PRINT_RESRV);
					PE_SHP_H15_CASE_SPRINT(56,rd_cnt++,buffer,PE_SHP_H15_PRINT_RESRV);
					PE_SHP_H15_CASE_SPRINT(57,rd_cnt++,buffer,PE_SHP_H15_PRINT_RESRV);
					PE_SHP_H15_CASE_SPRINT(58,rd_cnt++,buffer,PE_SHP_H15_PRINT_RESRV);
					PE_SHP_H15_CASE_SPRINT(59,rd_cnt++,buffer,\
						"   -----------------------------------------------------------\n");
					PE_SHP_H15_DFLT_SPRINT(rd_cnt=0,buffer,PE_SHP_H15_PRINT_END);
				}
			}
			else
			{
				if (rd_cnt>=PE_SHP_HW_H15_SCL_CO_REG_INC_MAX)
				{
					/* return origin value */
					switch (rd_ready)
					{
						case PE_SHP_HW_H15_RD_RDY_RD_M1Y:
							PE_CMN_HW_H15_SetDbInfo(PE_CMN_HW_H15_DB_INF_MD0_SCL_FW_EN, 0, scl_fw_en);
							PE_MSC_Y_H15A0_QWr01(msc_m1_y_outsize,reg_vosize_m1,v_o_size);
							break;
						case PE_SHP_HW_H15_RD_RDY_RD_M2Y:
							PE_CMN_HW_H15_SetDbInfo(PE_CMN_HW_H15_DB_INF_MD0_SCL_FW_EN, 0, scl_fw_en);
							PE_MSC_Y_H15A0_QWr01(msc_m2_y_outsize,reg_vosize_r,v_o_size);
							break;
						case PE_SHP_HW_H15_RD_RDY_RD_M1C:
							PE_CMN_HW_H15_SetDbInfo(PE_CMN_HW_H15_DB_INF_MD0_SCL_FW_EN, 0, scl_fw_en);
							PE_MSC_C_H15A0_QWr01(msc_m1_c_outsize,reg_vosize_m1,v_o_size);
							break;
						case PE_SHP_HW_H15_RD_RDY_RD_M2C:
							PE_CMN_HW_H15_SetDbInfo(PE_CMN_HW_H15_DB_INF_MD0_SCL_FW_EN, 0, scl_fw_en);
							PE_MSC_C_H15A0_QWr01(msc_m2_c_outsize,reg_vosize_m2,v_o_size);
							break;
						case PE_SHP_HW_H15_RD_RDY_RD_P1Y:
							PE_CMN_HW_H15_SetDbInfo(PE_CMN_HW_H15_DB_INF_MD0_SCL_FW_EN, 0, scl_fw_en);
							PE_SR_GSC_H15A0_QWr01(sr_gsc_y_outsize,reg_vosize_m1,v_o_size);
						default:
							break;
					}
					sprintf(buffer, PE_SHP_H15_PRINT_END);
					rd_ready = 0;
					rd_cnt=0;
				}
				else
				{
					switch (pstParams->data[0])
					{
						case PE_SHP_HW_H15_RD_SCL_DRV_ON:
						case PE_SHP_HW_H15_RD_SCL_DRV_OFF:
							data[0] = (pstParams->data[0]==PE_SHP_HW_H15_RD_SCL_DRV_ON)? 0:1;
							PE_CMN_HW_H15_SetDbInfo(PE_CMN_HW_H15_DB_INF_MD0_SCL_FW_EN, 0, data[0]);
							sprintf(buffer, PE_SHP_H15_PRINT_END);
							rd_cnt=0;
							break;
						case PE_SHP_HW_H15_RD_RDY_RD_M1Y:
							scl_fw_en = PE_CMN_HW_H15_GetDbInfo(PE_CMN_HW_H15_DB_INF_MD0_SCL_FW_EN, 0);
							PE_CMN_HW_H15_SetDbInfo(PE_CMN_HW_H15_DB_INF_MD0_SCL_FW_EN, 0, 1);
							PE_MSC_Y_H15A0_QRd01(msc_m1_y_outsize,reg_vosize_m1,v_o_size);
							PE_MSC_Y_H15A0_QWr01(msc_m1_y_outsize,reg_vosize_m1,0);
							rd_ready = pstParams->data[0];
							sprintf(buffer, PE_SHP_H15_PRINT_END);
							rd_cnt=0;
							break;
						case PE_SHP_HW_H15_RD_RDY_RD_M2Y:
							scl_fw_en = PE_CMN_HW_H15_GetDbInfo(PE_CMN_HW_H15_DB_INF_MD0_SCL_FW_EN, 0);
							PE_CMN_HW_H15_SetDbInfo(PE_CMN_HW_H15_DB_INF_MD0_SCL_FW_EN, 0, 1);
							PE_MSC_Y_H15A0_QRd01(msc_m2_y_outsize,reg_vosize_r,v_o_size);
							PE_MSC_Y_H15A0_QWr01(msc_m2_y_outsize,reg_vosize_r,0);
							rd_ready = pstParams->data[0];
							sprintf(buffer, PE_SHP_H15_PRINT_END);
							rd_cnt=0;
							break;
						case PE_SHP_HW_H15_RD_RDY_RD_M1C:
							scl_fw_en = PE_CMN_HW_H15_GetDbInfo(PE_CMN_HW_H15_DB_INF_MD0_SCL_FW_EN, 0);
							PE_CMN_HW_H15_SetDbInfo(PE_CMN_HW_H15_DB_INF_MD0_SCL_FW_EN, 0, 1);
							PE_MSC_C_H15A0_QRd01(msc_m1_c_outsize,reg_vosize_m1,v_o_size);
							PE_MSC_C_H15A0_QWr01(msc_m1_c_outsize,reg_vosize_m1,0);
							rd_ready = pstParams->data[0];
							sprintf(buffer, PE_SHP_H15_PRINT_END);
							rd_cnt=0;
							break;
						case PE_SHP_HW_H15_RD_RDY_RD_M2C:
							scl_fw_en = PE_CMN_HW_H15_GetDbInfo(PE_CMN_HW_H15_DB_INF_MD0_SCL_FW_EN, 0);
							PE_CMN_HW_H15_SetDbInfo(PE_CMN_HW_H15_DB_INF_MD0_SCL_FW_EN, 0, 1);
							PE_MSC_C_H15A0_QRd01(msc_m2_c_outsize,reg_vosize_m2,v_o_size);
							PE_MSC_C_H15A0_QWr01(msc_m2_c_outsize,reg_vosize_m2,0);
							rd_ready = pstParams->data[0];
							sprintf(buffer, PE_SHP_H15_PRINT_END);
							rd_cnt=0;
							break;
						case PE_SHP_HW_H15_RD_RDY_RD_P1Y:
							scl_fw_en = PE_CMN_HW_H15_GetDbInfo(PE_CMN_HW_H15_DB_INF_MD0_SCL_FW_EN, 0);
							PE_CMN_HW_H15_SetDbInfo(PE_CMN_HW_H15_DB_INF_MD0_SCL_FW_EN, 0, 1);
							PE_SR_GSC_H15A0_QRd01(sr_gsc_y_outsize,reg_vosize_m1,v_o_size);
							PE_SR_GSC_H15A0_QWr01(sr_gsc_y_outsize,reg_vosize_m1,0);
							rd_ready = pstParams->data[0];
							sprintf(buffer, PE_SHP_H15_PRINT_END);
							rd_cnt=0;
							break;
						case PE_SHP_HW_H15_RD_SCL_M1Y_H1:
							if (rd_ready == PE_SHP_HW_H15_RD_RDY_RD_M1Y)
							{
								PE_MSC_Y_H15A0_QWr03(msc_m1_y_h1_fir_coef_ctrl,update,1,rnw,1,addr,rd_cnt);
								PE_MSC_Y_H15A0_RdFL(msc_m1_y_h1_y_fir_coef_data0);
								PE_MSC_Y_H15A0_RdFL(msc_m1_y_h1_y_fir_coef_data1);
								PE_MSC_Y_H15A0_RdFL(msc_m1_y_h1_y_fir_coef_data2);
								PE_MSC_Y_H15A0_RdFL(msc_m1_y_h1_y_fir_coef_data3);
								PE_MSC_Y_H15A0_RdFL(msc_m1_y_h1_y_fir_coef_data4);
								PE_MSC_Y_H15A0_RdFL(msc_m1_y_h1_y_fir_coef_data5);
								data[0] = PE_MSC_Y_H15A0_Rd(msc_m1_y_h1_y_fir_coef_data0);
								data[1] = PE_MSC_Y_H15A0_Rd(msc_m1_y_h1_y_fir_coef_data1);
								data[2] = PE_MSC_Y_H15A0_Rd(msc_m1_y_h1_y_fir_coef_data2);
								data[3] = PE_MSC_Y_H15A0_Rd(msc_m1_y_h1_y_fir_coef_data3);
								data[4] = PE_MSC_Y_H15A0_Rd(msc_m1_y_h1_y_fir_coef_data4);
								data[5] = PE_MSC_Y_H15A0_Rd(msc_m1_y_h1_y_fir_coef_data5);
								sprintf(buffer, "[%s][%03d] 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x\n", \
									PE_SHP_HW_H15_SCL_MENU_TO_STR(pstParams->data[0]), \
									rd_cnt, data[0], data[1], data[2], data[3], data[4], data[5]);
								rd_cnt++;
							}
							else
							{
								sprintf(buffer, "Firstly, need to run PE_SHP_HW_H15_RD_RDY_RD_M1Y.\n");
								rd_cnt = PE_SHP_HW_H15_SCL_CO_REG_INC_MAX;
							}
							break;
						case PE_SHP_HW_H15_RD_SCL_M1Y_H2:
							if (rd_ready == PE_SHP_HW_H15_RD_RDY_RD_M1Y)
							{
								PE_MSC_Y_H15A0_QWr03(msc_m1_y_h2_fir_coef_ctrl,update,1,rnw,1,addr,rd_cnt);
								PE_MSC_Y_H15A0_RdFL(msc_m1_y_h2_y_fir_coef_data0);
								PE_MSC_Y_H15A0_RdFL(msc_m1_y_h2_y_fir_coef_data1);
								PE_MSC_Y_H15A0_RdFL(msc_m1_y_h2_y_fir_coef_data2);
								PE_MSC_Y_H15A0_RdFL(msc_m1_y_h2_y_fir_coef_data3);
								data[0] = PE_MSC_Y_H15A0_Rd(msc_m1_y_h2_y_fir_coef_data0);
								data[1] = PE_MSC_Y_H15A0_Rd(msc_m1_y_h2_y_fir_coef_data1);
								data[2] = PE_MSC_Y_H15A0_Rd(msc_m1_y_h2_y_fir_coef_data2);
								data[3] = PE_MSC_Y_H15A0_Rd(msc_m1_y_h2_y_fir_coef_data3);
								sprintf(buffer, "[%s][%03d] 0x%08x 0x%08x 0x%08x 0x%08x\n", \
									PE_SHP_HW_H15_SCL_MENU_TO_STR(pstParams->data[0]), \
									rd_cnt, data[0], data[1], data[2], data[3]);
								rd_cnt++;
							}
							else
							{
								sprintf(buffer, "Firstly, need to run PE_SHP_HW_H15_RD_RDY_RD_M1Y.\n");
								rd_cnt = PE_SHP_HW_H15_SCL_CO_REG_INC_MAX;
							}
							break;
						case PE_SHP_HW_H15_RD_SCL_M1Y_V1:
							if (rd_ready == PE_SHP_HW_H15_RD_RDY_RD_M1Y)
							{
								PE_MSC_Y_H15A0_QWr03(msc_m1_y_v1_fir_coef_ctrl,update,1,rnw,1,addr,rd_cnt);
								PE_MSC_Y_H15A0_RdFL(msc_m1_y_v1_y_fir_coef_data0);
								PE_MSC_Y_H15A0_RdFL(msc_m1_y_v1_y_fir_coef_data1);
								PE_MSC_Y_H15A0_RdFL(msc_m1_y_v1_y_fir_coef_data2);
								PE_MSC_Y_H15A0_RdFL(msc_m1_y_v1_y_fir_coef_data3);
								PE_MSC_Y_H15A0_RdFL(msc_m1_y_v1_y_fir_coef_data4);
								PE_MSC_Y_H15A0_RdFL(msc_m1_y_v1_y_fir_coef_data5);
								data[0] = PE_MSC_Y_H15A0_Rd(msc_m1_y_v1_y_fir_coef_data0);
								data[1] = PE_MSC_Y_H15A0_Rd(msc_m1_y_v1_y_fir_coef_data1);
								data[2] = PE_MSC_Y_H15A0_Rd(msc_m1_y_v1_y_fir_coef_data2);
								data[3] = PE_MSC_Y_H15A0_Rd(msc_m1_y_v1_y_fir_coef_data3);
								data[4] = PE_MSC_Y_H15A0_Rd(msc_m1_y_v1_y_fir_coef_data4);
								data[5] = PE_MSC_Y_H15A0_Rd(msc_m1_y_v1_y_fir_coef_data5);
								sprintf(buffer, "[%s][%03d] 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x\n", \
									PE_SHP_HW_H15_SCL_MENU_TO_STR(pstParams->data[0]), \
									rd_cnt, data[0], data[1], data[2], data[3], data[4], data[5]);
								rd_cnt++;
							}
							else
							{
								sprintf(buffer, "Firstly, need to run PE_SHP_HW_H15_RD_RDY_RD_M1Y.\n");
								rd_cnt = PE_SHP_HW_H15_SCL_CO_REG_INC_MAX;
							}
							break;
						case PE_SHP_HW_H15_RD_SCL_M1Y_V2:
							if (rd_ready == PE_SHP_HW_H15_RD_RDY_RD_M1Y)
							{
								PE_MSC_Y_H15A0_QWr03(msc_m1_y_v2_fir_coef_ctrl,update,1,rnw,1,addr,rd_cnt);
								PE_MSC_Y_H15A0_RdFL(msc_m1_y_v2_y_fir_coef_data0);
								PE_MSC_Y_H15A0_RdFL(msc_m1_y_v2_y_fir_coef_data1);
								PE_MSC_Y_H15A0_RdFL(msc_m1_y_v2_y_fir_coef_data2);
								PE_MSC_Y_H15A0_RdFL(msc_m1_y_v2_y_fir_coef_data3);
								data[0] = PE_MSC_Y_H15A0_Rd(msc_m1_y_v2_y_fir_coef_data0);
								data[1] = PE_MSC_Y_H15A0_Rd(msc_m1_y_v2_y_fir_coef_data1);
								data[2] = PE_MSC_Y_H15A0_Rd(msc_m1_y_v2_y_fir_coef_data2);
								data[3] = PE_MSC_Y_H15A0_Rd(msc_m1_y_v2_y_fir_coef_data3);
								sprintf(buffer, "[%s][%03d] 0x%08x 0x%08x 0x%08x 0x%08x\n", \
									PE_SHP_HW_H15_SCL_MENU_TO_STR(pstParams->data[0]), \
									rd_cnt, data[0], data[1], data[2], data[3]);
								rd_cnt++;
							}
							else
							{
								sprintf(buffer, "Firstly, need to run PE_SHP_HW_H15_RD_RDY_RD_M1Y.\n");
								rd_cnt = PE_SHP_HW_H15_SCL_CO_REG_INC_MAX;
							}
							break;
						case PE_SHP_HW_H15_RD_SCL_M2Y_H1:
							if (rd_ready == PE_SHP_HW_H15_RD_RDY_RD_M2Y)
							{
								PE_MSC_Y_H15A0_QWr03(msc_m2_y_h1_fir_coef_ctrl,update,1,rnw,1,addr,rd_cnt);
								PE_MSC_Y_H15A0_RdFL(msc_m2_y_h1_fir_coef_data0);
								PE_MSC_Y_H15A0_RdFL(msc_m2_y_h1_fir_coef_data1);
								PE_MSC_Y_H15A0_RdFL(msc_m2_y_h1_fir_coef_data2);
								PE_MSC_Y_H15A0_RdFL(msc_m2_y_h1_fir_coef_data3);
								PE_MSC_Y_H15A0_RdFL(msc_m2_y_h1_fir_coef_data4);
								PE_MSC_Y_H15A0_RdFL(msc_m2_y_h1_fir_coef_data5);
								data[0] = PE_MSC_Y_H15A0_Rd(msc_m2_y_h1_fir_coef_data0);
								data[1] = PE_MSC_Y_H15A0_Rd(msc_m2_y_h1_fir_coef_data1);
								data[2] = PE_MSC_Y_H15A0_Rd(msc_m2_y_h1_fir_coef_data2);
								data[3] = PE_MSC_Y_H15A0_Rd(msc_m2_y_h1_fir_coef_data3);
								data[4] = PE_MSC_Y_H15A0_Rd(msc_m2_y_h1_fir_coef_data4);
								data[5] = PE_MSC_Y_H15A0_Rd(msc_m2_y_h1_fir_coef_data5);
								sprintf(buffer, "[%s][%03d] 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x\n", \
									PE_SHP_HW_H15_SCL_MENU_TO_STR(pstParams->data[0]), \
									rd_cnt, data[0], data[1], data[2], data[3], data[4], data[5]);
								rd_cnt++;
							}
							else
							{
								sprintf(buffer, "Firstly, need to run PE_SHP_HW_H15_RD_RDY_RD_M2Y.\n");
								rd_cnt = PE_SHP_HW_H15_SCL_CO_REG_INC_MAX;
							}
							break;
						case PE_SHP_HW_H15_RD_SCL_M2Y_H2:
							if (rd_ready == PE_SHP_HW_H15_RD_RDY_RD_M2Y)
							{
								PE_MSC_Y_H15A0_QWr03(msc_m2_y_h2_fir_coef_ctrl,update,1,rnw,1,addr,rd_cnt);
								PE_MSC_Y_H15A0_RdFL(msc_m2_y_h2_fir_coef_data0);
								PE_MSC_Y_H15A0_RdFL(msc_m2_y_h2_fir_coef_data1);
								PE_MSC_Y_H15A0_RdFL(msc_m2_y_h2_fir_coef_data2);
								PE_MSC_Y_H15A0_RdFL(msc_m2_y_h2_fir_coef_data3);
								data[0] = PE_MSC_Y_H15A0_Rd(msc_m2_y_h2_fir_coef_data0);
								data[1] = PE_MSC_Y_H15A0_Rd(msc_m2_y_h2_fir_coef_data1);
								data[2] = PE_MSC_Y_H15A0_Rd(msc_m2_y_h2_fir_coef_data2);
								data[3] = PE_MSC_Y_H15A0_Rd(msc_m2_y_h2_fir_coef_data3);
								sprintf(buffer, "[%s][%03d] 0x%08x 0x%08x 0x%08x 0x%08x\n", \
									PE_SHP_HW_H15_SCL_MENU_TO_STR(pstParams->data[0]), \
									rd_cnt, data[0], data[1], data[2], data[3]);
								rd_cnt++;
							}
							else
							{
								sprintf(buffer, "Firstly, need to run PE_SHP_HW_H15_RD_RDY_RD_M2Y.\n");
								rd_cnt = PE_SHP_HW_H15_SCL_CO_REG_INC_MAX;
							}
							break;
						case PE_SHP_HW_H15_RD_SCL_M2Y_V1:
							if (rd_ready == PE_SHP_HW_H15_RD_RDY_RD_M2Y)
							{
								PE_MSC_Y_H15A0_QWr03(msc_m2_y_v1_fir_coef_ctrl,update,1,rnw,1,addr,rd_cnt);
								PE_MSC_Y_H15A0_RdFL(msc_m2_y_v1_fir_coef_data0);
								PE_MSC_Y_H15A0_RdFL(msc_m2_y_v1_fir_coef_data1);
								PE_MSC_Y_H15A0_RdFL(msc_m2_y_v1_fir_coef_data2);
								PE_MSC_Y_H15A0_RdFL(msc_m2_y_v1_fir_coef_data3);
								PE_MSC_Y_H15A0_RdFL(msc_m2_y_v1_fir_coef_data4);
								PE_MSC_Y_H15A0_RdFL(msc_m2_y_v1_fir_coef_data5);
								data[0] = PE_MSC_Y_H15A0_Rd(msc_m2_y_v1_fir_coef_data0);
								data[1] = PE_MSC_Y_H15A0_Rd(msc_m2_y_v1_fir_coef_data1);
								data[2] = PE_MSC_Y_H15A0_Rd(msc_m2_y_v1_fir_coef_data2);
								data[3] = PE_MSC_Y_H15A0_Rd(msc_m2_y_v1_fir_coef_data3);
								data[4] = PE_MSC_Y_H15A0_Rd(msc_m2_y_v1_fir_coef_data4);
								data[5] = PE_MSC_Y_H15A0_Rd(msc_m2_y_v1_fir_coef_data5);
								sprintf(buffer, "[%s][%03d] 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x\n", \
									PE_SHP_HW_H15_SCL_MENU_TO_STR(pstParams->data[0]), \
									rd_cnt, data[0], data[1], data[2], data[3], data[4], data[5]);
								rd_cnt++;
							}
							else
							{
								sprintf(buffer, "Firstly, need to run PE_SHP_HW_H15_RD_RDY_RD_M2Y.\n");
								rd_cnt = PE_SHP_HW_H15_SCL_CO_REG_INC_MAX;
							}
							break;
						case PE_SHP_HW_H15_RD_SCL_M2Y_V2:
							if (rd_ready == PE_SHP_HW_H15_RD_RDY_RD_M2Y)
							{
								PE_MSC_Y_H15A0_QWr03(msc_m2_y_v2_fir_coef_ctrl,update,1,rnw,1,addr,rd_cnt);
								PE_MSC_Y_H15A0_RdFL(msc_m2_y_v2_fir_coef_data0);
								PE_MSC_Y_H15A0_RdFL(msc_m2_y_v2_fir_coef_data1);
								PE_MSC_Y_H15A0_RdFL(msc_m2_y_v2_fir_coef_data2);
								PE_MSC_Y_H15A0_RdFL(msc_m2_y_v2_fir_coef_data3);
								data[0] = PE_MSC_Y_H15A0_Rd(msc_m2_y_v2_fir_coef_data0);
								data[1] = PE_MSC_Y_H15A0_Rd(msc_m2_y_v2_fir_coef_data1);
								data[2] = PE_MSC_Y_H15A0_Rd(msc_m2_y_v2_fir_coef_data2);
								data[3] = PE_MSC_Y_H15A0_Rd(msc_m2_y_v2_fir_coef_data3);
								sprintf(buffer, "[%s][%03d] 0x%08x 0x%08x 0x%08x 0x%08x\n", \
									PE_SHP_HW_H15_SCL_MENU_TO_STR(pstParams->data[0]), \
									rd_cnt, data[0], data[1], data[2], data[3]);
								rd_cnt++;
							}
							else
							{
								sprintf(buffer, "Firstly, need to run PE_SHP_HW_H15_RD_RDY_RD_M2Y.\n");
								rd_cnt = PE_SHP_HW_H15_SCL_CO_REG_INC_MAX;
							}
							break;
						case PE_SHP_HW_H15_RD_SCL_M1C_H1:
							if (rd_ready == PE_SHP_HW_H15_RD_RDY_RD_M1C)
							{
								PE_MSC_C_H15A0_QWr03(msc_m1_c_h1_fir_coef_ctrl,update,1,rnw,1,addr,rd_cnt);
								PE_MSC_C_H15A0_RdFL(msc_m1_c_h1_fir_coef_data0);
								PE_MSC_C_H15A0_RdFL(msc_m1_c_h1_fir_coef_data1);
								PE_MSC_C_H15A0_RdFL(msc_m1_c_h1_fir_coef_data2);
								PE_MSC_C_H15A0_RdFL(msc_m1_c_h1_fir_coef_data3);
								PE_MSC_C_H15A0_RdFL(msc_m1_c_h1_fir_coef_data4);
								PE_MSC_C_H15A0_RdFL(msc_m1_c_h1_fir_coef_data5);
								data[0] = PE_MSC_C_H15A0_Rd(msc_m1_c_h1_fir_coef_data0);
								data[1] = PE_MSC_C_H15A0_Rd(msc_m1_c_h1_fir_coef_data1);
								data[2] = PE_MSC_C_H15A0_Rd(msc_m1_c_h1_fir_coef_data2);
								data[3] = PE_MSC_C_H15A0_Rd(msc_m1_c_h1_fir_coef_data3);
								data[4] = PE_MSC_C_H15A0_Rd(msc_m1_c_h1_fir_coef_data4);
								data[5] = PE_MSC_C_H15A0_Rd(msc_m1_c_h1_fir_coef_data5);
								sprintf(buffer, "[%s][%03d] 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x\n", \
									PE_SHP_HW_H15_SCL_MENU_TO_STR(pstParams->data[0]), \
									rd_cnt, data[0], data[1], data[2], data[3], data[4], data[5]);
								rd_cnt++;
							}
							else
							{
								sprintf(buffer, "Firstly, need to run PE_SHP_HW_H15_RD_RDY_RD_M1C.\n");
								rd_cnt = PE_SHP_HW_H15_SCL_CO_REG_INC_MAX;
							}
							break;
						case PE_SHP_HW_H15_RD_SCL_M1C_V1:
							if (rd_ready == PE_SHP_HW_H15_RD_RDY_RD_M1C)
							{
								PE_MSC_C_H15A0_QWr03(msc_m1_c_v1_fir_coef_ctrl,update,1,rnw,1,addr,rd_cnt);
								PE_MSC_C_H15A0_RdFL(msc_m1_c_v1_fir_coef_data0);
								PE_MSC_C_H15A0_RdFL(msc_m1_c_v1_fir_coef_data1);
								PE_MSC_C_H15A0_RdFL(msc_m1_c_v1_fir_coef_data2);
								PE_MSC_C_H15A0_RdFL(msc_m1_c_v1_fir_coef_data3);
								PE_MSC_C_H15A0_RdFL(msc_m1_c_v1_fir_coef_data4);
								PE_MSC_C_H15A0_RdFL(msc_m1_c_v1_fir_coef_data5);
								data[0] = PE_MSC_C_H15A0_Rd(msc_m1_c_v1_fir_coef_data0);
								data[1] = PE_MSC_C_H15A0_Rd(msc_m1_c_v1_fir_coef_data1);
								data[2] = PE_MSC_C_H15A0_Rd(msc_m1_c_v1_fir_coef_data2);
								data[3] = PE_MSC_C_H15A0_Rd(msc_m1_c_v1_fir_coef_data3);
								data[4] = PE_MSC_C_H15A0_Rd(msc_m1_c_v1_fir_coef_data4);
								data[5] = PE_MSC_C_H15A0_Rd(msc_m1_c_v1_fir_coef_data5);
								sprintf(buffer, "[%s][%03d] 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x\n", \
									PE_SHP_HW_H15_SCL_MENU_TO_STR(pstParams->data[0]), \
									rd_cnt, data[0], data[1], data[2], data[3], data[4], data[5]);
								rd_cnt++;
							}
							else
							{
								sprintf(buffer, "Firstly, need to run PE_SHP_HW_H15_RD_RDY_RD_M1C.\n");
								rd_cnt = PE_SHP_HW_H15_SCL_CO_REG_INC_MAX;
							}
							break;
						case PE_SHP_HW_H15_RD_SCL_M2C_H1:
							if (rd_ready == PE_SHP_HW_H15_RD_RDY_RD_M1C)
							{
								PE_MSC_C_H15A0_QWr03(msc_m2_c_h1_fir_coef_ctrl,update,1,rnw,1,addr,rd_cnt);
								PE_MSC_C_H15A0_RdFL(msc_m2_c_h1_c_fir_coef_data0);
								PE_MSC_C_H15A0_RdFL(msc_m2_c_h1_c_fir_coef_data1);
								PE_MSC_C_H15A0_RdFL(msc_m2_c_h1_c_fir_coef_data2);
								PE_MSC_C_H15A0_RdFL(msc_m2_c_h1_c_fir_coef_data3);
								PE_MSC_C_H15A0_RdFL(msc_m2_c_h1_c_fir_coef_data4);
								PE_MSC_C_H15A0_RdFL(msc_m2_c_h1_c_fir_coef_data5);
								data[0] = PE_MSC_C_H15A0_Rd(msc_m2_c_h1_c_fir_coef_data0);
								data[1] = PE_MSC_C_H15A0_Rd(msc_m2_c_h1_c_fir_coef_data1);
								data[2] = PE_MSC_C_H15A0_Rd(msc_m2_c_h1_c_fir_coef_data2);
								data[3] = PE_MSC_C_H15A0_Rd(msc_m2_c_h1_c_fir_coef_data3);
								data[4] = PE_MSC_C_H15A0_Rd(msc_m2_c_h1_c_fir_coef_data4);
								data[5] = PE_MSC_C_H15A0_Rd(msc_m2_c_h1_c_fir_coef_data5);
								sprintf(buffer, "[%s][%03d] 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x\n", \
									PE_SHP_HW_H15_SCL_MENU_TO_STR(pstParams->data[0]), \
									rd_cnt, data[0], data[1], data[2], data[3], data[4], data[5]);
								rd_cnt++;
							}
							else
							{
								sprintf(buffer, "Firstly, need to run PE_SHP_HW_H15_RD_RDY_RD_M1C.\n");
								rd_cnt = PE_SHP_HW_H15_SCL_CO_REG_INC_MAX;
							}
							break;
						case PE_SHP_HW_H15_RD_SCL_M2C_V1:
							if (rd_ready == PE_SHP_HW_H15_RD_RDY_RD_M1C)
							{
								PE_MSC_C_H15A0_QWr03(msc_m2_c_v1_fir_coef_ctrl,update,1,rnw,1,addr,rd_cnt);
								PE_MSC_C_H15A0_RdFL(msc_m2_c_v1_c_fir_coef_data0);
								PE_MSC_C_H15A0_RdFL(msc_m2_c_v1_c_fir_coef_data1);
								PE_MSC_C_H15A0_RdFL(msc_m2_c_v1_c_fir_coef_data2);
								PE_MSC_C_H15A0_RdFL(msc_m2_c_v1_c_fir_coef_data3);
								PE_MSC_C_H15A0_RdFL(msc_m2_c_v1_c_fir_coef_data4);
								PE_MSC_C_H15A0_RdFL(msc_m2_c_v1_c_fir_coef_data5);
								data[0] = PE_MSC_C_H15A0_Rd(msc_m2_c_v1_c_fir_coef_data0);
								data[1] = PE_MSC_C_H15A0_Rd(msc_m2_c_v1_c_fir_coef_data1);
								data[2] = PE_MSC_C_H15A0_Rd(msc_m2_c_v1_c_fir_coef_data2);
								data[3] = PE_MSC_C_H15A0_Rd(msc_m2_c_v1_c_fir_coef_data3);
								data[4] = PE_MSC_C_H15A0_Rd(msc_m2_c_v1_c_fir_coef_data4);
								data[5] = PE_MSC_C_H15A0_Rd(msc_m2_c_v1_c_fir_coef_data5);
								sprintf(buffer, "[%s][%03d] 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x\n", \
									PE_SHP_HW_H15_SCL_MENU_TO_STR(pstParams->data[0]), \
									rd_cnt, data[0], data[1], data[2], data[3], data[4], data[5]);
								rd_cnt++;
							}
							else
							{
								sprintf(buffer, "Firstly, need to run PE_SHP_HW_H15_RD_RDY_RD_M1C.\n");
								rd_cnt = PE_SHP_HW_H15_SCL_CO_REG_INC_MAX;
							}
							break;
						case PE_SHP_HW_H15_RD_SCL_P1Y_H1:
							if (rd_ready == PE_SHP_HW_H15_RD_RDY_RD_P1Y)
							{
								PE_SR_GSC_H15A0_QWr03(sr_gsc_y_h1_fir_coef_ctrl,update,1,rnw,1,addr,rd_cnt);
								PE_SR_GSC_H15A0_RdFL(sr_gsc_y_h1_y_fir_coef_data0);
								PE_SR_GSC_H15A0_RdFL(sr_gsc_y_h1_y_fir_coef_data1);
								PE_SR_GSC_H15A0_RdFL(sr_gsc_y_h1_y_fir_coef_data2);
								PE_SR_GSC_H15A0_RdFL(sr_gsc_y_h1_y_fir_coef_data3);
								PE_SR_GSC_H15A0_RdFL(sr_gsc_y_h1_y_fir_coef_data4);
								PE_SR_GSC_H15A0_RdFL(sr_gsc_y_h1_y_fir_coef_data5);
								data[0] = PE_SR_GSC_H15A0_Rd(sr_gsc_y_h1_y_fir_coef_data0);
								data[1] = PE_SR_GSC_H15A0_Rd(sr_gsc_y_h1_y_fir_coef_data1);
								data[2] = PE_SR_GSC_H15A0_Rd(sr_gsc_y_h1_y_fir_coef_data2);
								data[3] = PE_SR_GSC_H15A0_Rd(sr_gsc_y_h1_y_fir_coef_data3);
								data[4] = PE_SR_GSC_H15A0_Rd(sr_gsc_y_h1_y_fir_coef_data4);
								data[5] = PE_SR_GSC_H15A0_Rd(sr_gsc_y_h1_y_fir_coef_data5);
								sprintf(buffer, "[%s][%03d] 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x\n", \
									PE_SHP_HW_H15_SCL_MENU_TO_STR(pstParams->data[0]), \
									rd_cnt, data[0], data[1], data[2], data[3], data[4], data[5]);
								rd_cnt++;
							}
							else
							{
								sprintf(buffer, "Firstly, need to run PE_SHP_HW_H15_RD_RDY_RD_P1Y.\n");
								rd_cnt = PE_SHP_HW_H15_SCL_CO_REG_INC_MAX;
							}
							break;
						case PE_SHP_HW_H15_RD_SCL_P1Y_H2:
							if (rd_ready == PE_SHP_HW_H15_RD_RDY_RD_P1Y)
							{
								PE_SR_GSC_H15A0_QWr03(sr_gsc_y_h2_fir_coef_ctrl,update,1,rnw,1,addr,rd_cnt);
								PE_SR_GSC_H15A0_RdFL(sr_gsc_y_h2_y_fir_coef_data0);
								PE_SR_GSC_H15A0_RdFL(sr_gsc_y_h2_y_fir_coef_data1);
								PE_SR_GSC_H15A0_RdFL(sr_gsc_y_h2_y_fir_coef_data2);
								PE_SR_GSC_H15A0_RdFL(sr_gsc_y_h2_y_fir_coef_data3);
								data[0] = PE_SR_GSC_H15A0_Rd(sr_gsc_y_h2_y_fir_coef_data0);
								data[1] = PE_SR_GSC_H15A0_Rd(sr_gsc_y_h2_y_fir_coef_data1);
								data[2] = PE_SR_GSC_H15A0_Rd(sr_gsc_y_h2_y_fir_coef_data2);
								data[3] = PE_SR_GSC_H15A0_Rd(sr_gsc_y_h2_y_fir_coef_data3);
								sprintf(buffer, "[%s][%03d] 0x%08x 0x%08x 0x%08x 0x%08x\n", \
									PE_SHP_HW_H15_SCL_MENU_TO_STR(pstParams->data[0]), \
									rd_cnt, data[0], data[1], data[2], data[3]);
								rd_cnt++;
							}
							else
							{
								sprintf(buffer, "Firstly, need to run PE_SHP_HW_H15_RD_RDY_RD_P1Y.\n");
								rd_cnt = PE_SHP_HW_H15_SCL_CO_REG_INC_MAX;
							}
							break;
						case PE_SHP_HW_H15_RD_SCL_P1Y_V1:
							if (rd_ready == PE_SHP_HW_H15_RD_RDY_RD_P1Y)
							{
								PE_SR_GSC_H15A0_QWr03(sr_gsc_y_v1_fir_coef_ctrl,update,1,rnw,1,addr,rd_cnt);
								PE_SR_GSC_H15A0_RdFL(sr_gsc_y_v1_y_fir_coef_data0);
								PE_SR_GSC_H15A0_RdFL(sr_gsc_y_v1_y_fir_coef_data1);
								PE_SR_GSC_H15A0_RdFL(sr_gsc_y_v1_y_fir_coef_data2);
								PE_SR_GSC_H15A0_RdFL(sr_gsc_y_v1_y_fir_coef_data3);
								PE_SR_GSC_H15A0_RdFL(sr_gsc_y_v1_y_fir_coef_data4);
								PE_SR_GSC_H15A0_RdFL(sr_gsc_y_v1_y_fir_coef_data5);
								data[0] = PE_SR_GSC_H15A0_Rd(sr_gsc_y_v1_y_fir_coef_data0);
								data[1] = PE_SR_GSC_H15A0_Rd(sr_gsc_y_v1_y_fir_coef_data1);
								data[2] = PE_SR_GSC_H15A0_Rd(sr_gsc_y_v1_y_fir_coef_data2);
								data[3] = PE_SR_GSC_H15A0_Rd(sr_gsc_y_v1_y_fir_coef_data3);
								data[4] = PE_SR_GSC_H15A0_Rd(sr_gsc_y_v1_y_fir_coef_data4);
								data[5] = PE_SR_GSC_H15A0_Rd(sr_gsc_y_v1_y_fir_coef_data5);
								sprintf(buffer, "[%s][%03d] 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x\n", \
									PE_SHP_HW_H15_SCL_MENU_TO_STR(pstParams->data[0]), \
									rd_cnt, data[0], data[1], data[2], data[3], data[4], data[5]);
								rd_cnt++;
							}
							else
							{
								sprintf(buffer, "Firstly, need to run PE_SHP_HW_H15_RD_RDY_RD_P1Y.\n");
								rd_cnt = PE_SHP_HW_H15_SCL_CO_REG_INC_MAX;
							}
							break;
						case PE_SHP_HW_H15_RD_SCL_P1Y_V2:
						default:
							if (rd_ready == PE_SHP_HW_H15_RD_RDY_RD_P1Y)
							{
								PE_SR_GSC_H15A0_QWr03(sr_gsc_y_v2_fir_coef_ctrl,update,1,rnw,1,addr,rd_cnt);
								PE_SR_GSC_H15A0_RdFL(sr_gsc_y_v2_y_fir_coef_data0);
								PE_SR_GSC_H15A0_RdFL(sr_gsc_y_v2_y_fir_coef_data1);
								PE_SR_GSC_H15A0_RdFL(sr_gsc_y_v2_y_fir_coef_data2);
								PE_SR_GSC_H15A0_RdFL(sr_gsc_y_v2_y_fir_coef_data3);
								data[0] = PE_SR_GSC_H15A0_Rd(sr_gsc_y_v2_y_fir_coef_data0);
								data[1] = PE_SR_GSC_H15A0_Rd(sr_gsc_y_v2_y_fir_coef_data1);
								data[2] = PE_SR_GSC_H15A0_Rd(sr_gsc_y_v2_y_fir_coef_data2);
								data[3] = PE_SR_GSC_H15A0_Rd(sr_gsc_y_v2_y_fir_coef_data3);
								sprintf(buffer, "[%s][%03d] 0x%08x 0x%08x 0x%08x 0x%08x\n", \
									PE_SHP_HW_H15_SCL_MENU_TO_STR(pstParams->data[0]), \
									rd_cnt, data[0], data[1], data[2], data[3]);
								rd_cnt++;
							}
							else
							{
								sprintf(buffer, "Firstly, need to run PE_SHP_HW_H15_RD_RDY_RD_P1Y.\n");
								rd_cnt = PE_SHP_HW_H15_SCL_CO_REG_INC_MAX;
							}
							break;
					}
				}
			}
			memcpy(pstParams->data,buffer,sizeof(char)*PE_TRACE_STR_SIZE);
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
int PE_SHP_HW_H15_RdDefault(LX_PE_ETC_TBL_T *pstParams)
{
	int ret = RET_OK;
	#ifdef PE_HW_H15_PROTECT_OVERLAP_SETTINGS
	char buffer[PE_TRACE_STR_SIZE];
	UINT32 param_size=0, table_size=0, is_valid=0;
	static UINT32 rd_cnt=0;
	PE_REG_PARAM_T param;
	PE_SHP_HW_H15_SETTINGS_T *pInfo=&_g_pe_shp_hw_h15_info;
	UINT32 *p_res_dflt = _g_pe_res_hw_param_data_h15a.reg_dflt.addr;
	UINT32 *p_res_user = _g_pe_res_hw_param_data_h15a.reg_user.addr;
	UINT32 *p_res_mask = _g_pe_res_hw_param_data_h15a.reg_mask.addr;
	UINT32 *p_res_data = _g_pe_res_hw_param_data_h15a.reg_data.addr;
	UINT32 *p_cti_dflt = _g_pe_cti_hw_param_data_h15a.reg_dflt.addr;
	UINT32 *p_cti_user = _g_pe_cti_hw_param_data_h15a.reg_user.addr;
	UINT32 *p_cti_mask = _g_pe_cti_hw_param_data_h15a.reg_mask.addr;
	UINT32 *p_cti_data = _g_pe_cti_hw_param_data_h15a.reg_data.addr;
	UINT32 *p_pbl_dflt = _g_pe_pbl_hw_param_data_h15a.reg_dflt.addr;
	UINT32 *p_pbl_user = _g_pe_pbl_hw_param_data_h15a.reg_user.addr;
	UINT32 *p_pbl_mask = _g_pe_pbl_hw_param_data_h15a.reg_mask.addr;
	UINT32 *p_pbl_data = _g_pe_pbl_hw_param_data_h15a.reg_data.addr;
	#ifdef PE_SHP_HW_H15_SRE_SHP_WR_ONLY_MD
	UINT32 *p_srs_dflt = _g_pe_srs_hw_param_data_h15a.reg_dflt.addr;
	UINT32 *p_srs_user = _g_pe_srs_hw_param_data_h15a.reg_user.addr;
	UINT32 *p_srs_mask = _g_pe_srs_hw_param_data_h15a.reg_mask.addr;
	UINT32 *p_srs_data = _g_pe_srs_hw_param_data_h15a.reg_data.addr;
	#endif	//#ifdef PE_SHP_HW_H15_SRE_SHP_WR_ONLY_MD

#define PE_SHP_H15_NO_PRINT			0
#define PE_SHP_H15_RD_N_HEX_PRINT	1
#define PE_SHP_H15_HEX_PRINT_ONLY	2
#define PE_SHP_H15_PRINT_START		"START_OF_PRINT"
#define PE_SHP_H15_PRINT_END		"END_OF_PRINT"
#define PE_SHP_H15_PRINT_EXIT		"EXIT_OF_PRINT"
#define PE_SHP_H15_PRINT_RESRV		"RESERVED"
#define PE_SHP_H15_CASE_PRMENU(_cnt, _action, _buf, _item)	\
	case (_cnt):sprintf(_buf, "   [0x%02x] %s\n", (_item), #_item);_action;break
#define PE_SHP_H15_CASE_SPRINT(_cnt, _action, _buf, fmt, args...)	\
	case (_cnt):sprintf(_buf, fmt, ##args);_action;break
#define PE_SHP_H15_DFLT_SPRINT(_action, _buf, fmt, args...)		\
	default:sprintf(_buf, fmt, ##args);_action;break

	do{
		CHECK_KNULL(pstParams);
		CHECK_KNULL(pstParams->data);
		CHECK_KNULL(p_res_dflt);
		CHECK_KNULL(p_res_user);
		CHECK_KNULL(p_res_mask);
		CHECK_KNULL(p_res_data);
		CHECK_KNULL(p_cti_dflt);
		CHECK_KNULL(p_cti_user);
		CHECK_KNULL(p_cti_mask);
		CHECK_KNULL(p_cti_data);
		if (PE_KDRV_VER_H15)
		{
			if (pstParams->data[0]>=PE_SHP_HW_H15A_RD_MENU_MAX)
			{
				sprintf(buffer, PE_SHP_H15_PRINT_EXIT);
				rd_cnt=0;
			}
			else if (pstParams->data[0]==PE_SHP_HW_H15A_RD_MENU_DISP)
			{
				switch (rd_cnt)
				{
					PE_SHP_H15_CASE_SPRINT( 0, rd_cnt++, buffer, PE_SHP_H15_PRINT_START);
					PE_SHP_H15_CASE_SPRINT( 1, rd_cnt++, buffer, "   ** current default **\n");
					PE_SHP_H15_CASE_SPRINT( 2, rd_cnt++, buffer, \
						"   res0a_fmt : %s\n", PE_SHP_HW_H15A_RES_FMT_TO_STR(pInfo->res0a_fmt));
					PE_SHP_H15_CASE_SPRINT( 3, rd_cnt++, buffer, \
						"   cti0a_fmt : %s\n", PE_SHP_HW_H15A_CTI_FMT_TO_STR(pInfo->cti0a_fmt));
					PE_SHP_H15_CASE_SPRINT( 4, rd_cnt++, buffer, \
						"   pbl0a_fmt : %s\n", PE_SHP_HW_H15A_PBL_FMT_TO_STR(pInfo->pbl0a_fmt));
					PE_SHP_H15_CASE_SPRINT( 5, rd_cnt++, buffer, \
						"   sre0a_fmt : %s\n", PE_SHP_HW_H15A_SRE_FMT_TO_STR(pInfo->sre0a_fmt));
					PE_SHP_H15_CASE_SPRINT( 6, rd_cnt++, buffer, \
						"   srs0a_fmt : %s\n", PE_SHP_HW_H15A_SRS_FMT_TO_STR(pInfo->srs0a_fmt));
					PE_SHP_H15_CASE_SPRINT( 7, rd_cnt++, buffer, PE_SHP_H15_PRINT_RESRV);
					PE_SHP_H15_CASE_SPRINT( 8, rd_cnt++, buffer, PE_SHP_H15_PRINT_RESRV);
					PE_SHP_H15_CASE_SPRINT( 9, rd_cnt++, buffer, PE_SHP_H15_PRINT_RESRV);
					PE_SHP_H15_CASE_SPRINT(10, rd_cnt++, buffer, "   *********************\n");
					PE_SHP_H15_CASE_PRMENU(11, rd_cnt++, buffer, PE_SHP_HW_H15A_RD_RES1_DISP);
					PE_SHP_H15_CASE_PRMENU(12, rd_cnt++, buffer, PE_SHP_HW_H15A_RD_RES2_DISP);
					PE_SHP_H15_CASE_PRMENU(13, rd_cnt++, buffer, PE_SHP_HW_H15A_RD_RES1_DFLT);
					PE_SHP_H15_CASE_PRMENU(14, rd_cnt++, buffer, PE_SHP_HW_H15A_RD_RES1_USER);
					PE_SHP_H15_CASE_PRMENU(15, rd_cnt++, buffer, PE_SHP_HW_H15A_RD_RES1_MASK);
					PE_SHP_H15_CASE_PRMENU(16, rd_cnt++, buffer, PE_SHP_HW_H15A_RD_RES1_DATA);
					PE_SHP_H15_CASE_PRMENU(17, rd_cnt++, buffer, PE_SHP_HW_H15A_RD_CTI1_DISP);
					PE_SHP_H15_CASE_PRMENU(18, rd_cnt++, buffer, PE_SHP_HW_H15A_RD_CTI2_DISP);
					PE_SHP_H15_CASE_PRMENU(19, rd_cnt++, buffer, PE_SHP_HW_H15A_RD_CTI1_DFLT);
					PE_SHP_H15_CASE_PRMENU(20, rd_cnt++, buffer, PE_SHP_HW_H15A_RD_CTI1_USER);
					PE_SHP_H15_CASE_PRMENU(21, rd_cnt++, buffer, PE_SHP_HW_H15A_RD_CTI1_MASK);
					PE_SHP_H15_CASE_PRMENU(22, rd_cnt++, buffer, PE_SHP_HW_H15A_RD_CTI1_DATA);
					PE_SHP_H15_CASE_PRMENU(23, rd_cnt++, buffer, PE_SHP_HW_H15A_RD_PBL1_DISP);
					PE_SHP_H15_CASE_PRMENU(24, rd_cnt++, buffer, PE_SHP_HW_H15A_RD_PBL2_DISP);
					PE_SHP_H15_CASE_PRMENU(25, rd_cnt++, buffer, PE_SHP_HW_H15A_RD_PBL1_DFLT);
					PE_SHP_H15_CASE_PRMENU(26, rd_cnt++, buffer, PE_SHP_HW_H15A_RD_PBL1_USER);
					PE_SHP_H15_CASE_PRMENU(27, rd_cnt++, buffer, PE_SHP_HW_H15A_RD_PBL1_MASK);
					PE_SHP_H15_CASE_PRMENU(28, rd_cnt++, buffer, PE_SHP_HW_H15A_RD_PBL1_DATA);
					#ifdef PE_SHP_HW_H15_SRE_SHP_WR_ONLY_MD
					PE_SHP_H15_CASE_SPRINT(29, rd_cnt++, buffer, PE_SHP_H15_PRINT_RESRV);
					PE_SHP_H15_CASE_SPRINT(30, rd_cnt++, buffer, PE_SHP_H15_PRINT_RESRV);
					PE_SHP_H15_CASE_PRMENU(31, rd_cnt++, buffer, PE_SHP_HW_H15A_RD_SRS1_DISP);
					PE_SHP_H15_CASE_PRMENU(32, rd_cnt++, buffer, PE_SHP_HW_H15A_RD_SRS1_DFLT);
					PE_SHP_H15_CASE_PRMENU(33, rd_cnt++, buffer, PE_SHP_HW_H15A_RD_SRS1_USER);
					PE_SHP_H15_CASE_PRMENU(34, rd_cnt++, buffer, PE_SHP_HW_H15A_RD_SRS1_MASK);
					PE_SHP_H15_CASE_PRMENU(35, rd_cnt++, buffer, PE_SHP_HW_H15A_RD_SRS1_DATA);
					#endif	//#ifdef PE_SHP_HW_H15_SRE_SHP_WR_ONLY_MD
					PE_SHP_H15_DFLT_SPRINT(rd_cnt=0, buffer, PE_SHP_H15_PRINT_END);
				}
			}
			else
			{
				is_valid = PE_SHP_H15_NO_PRINT;
				switch (pstParams->data[0])
				{
					case PE_SHP_HW_H15A_RD_RES1_DISP:
					case PE_SHP_HW_H15A_RD_RES2_DISP:
						table_size = sizeof(shp_l_init_h15a0);
						param_size = table_size/sizeof(PE_REG_PARAM_T);
						if (rd_cnt<param_size)
						{
							param.addr = (pstParams->data[0]==PE_SHP_HW_H15A_RD_RES1_DISP)? \
								shp_l_init_h15a0[rd_cnt].addr:\
								shp_l_init_h15a0[rd_cnt].addr+PE_H15A_P1_SHP_LR_DIFF;
							is_valid = PE_SHP_H15_RD_N_HEX_PRINT;
						}
						break;
					case PE_SHP_HW_H15A_RD_RES1_DFLT:
						table_size = sizeof(PE_RES_HW_PARAM_REG_H15A0_T);
						param_size = table_size/sizeof(UINT32);
						if (rd_cnt<param_size)
						{
							param.addr = shp_l_init_h15a0[rd_cnt].addr;
							param.data = p_res_dflt[rd_cnt];
							is_valid = PE_SHP_H15_HEX_PRINT_ONLY;
						}
						break;
					case PE_SHP_HW_H15A_RD_RES1_USER:
						table_size = sizeof(PE_RES_HW_PARAM_REG_H15A0_T);
						param_size = table_size/sizeof(UINT32);
						if (rd_cnt<param_size)
						{
							param.addr = shp_l_init_h15a0[rd_cnt].addr;
							param.data = p_res_user[rd_cnt];
							is_valid = PE_SHP_H15_HEX_PRINT_ONLY;
						}
						break;
					case PE_SHP_HW_H15A_RD_RES1_MASK:
						table_size = sizeof(PE_RES_HW_PARAM_REG_H15A0_T);
						param_size = table_size/sizeof(UINT32);
						if (rd_cnt<param_size)
						{
							param.addr = shp_l_init_h15a0[rd_cnt].addr;
							param.data = p_res_mask[rd_cnt];
							is_valid = PE_SHP_H15_HEX_PRINT_ONLY;
						}
						break;
					case PE_SHP_HW_H15A_RD_RES1_DATA:
						table_size = sizeof(PE_RES_HW_PARAM_REG_H15A0_T);
						param_size = table_size/sizeof(UINT32);
						if (rd_cnt<param_size)
						{
							param.addr = shp_l_init_h15a0[rd_cnt].addr;
							param.data = p_res_data[rd_cnt];
							is_valid = PE_SHP_H15_HEX_PRINT_ONLY;
						}
						break;
					case PE_SHP_HW_H15A_RD_CTI1_DISP:
					case PE_SHP_HW_H15A_RD_CTI2_DISP:
						table_size = sizeof(cti_dnr_l_init_h15a0);
						param_size = table_size/sizeof(PE_REG_PARAM_T);
						if (rd_cnt<param_size)
						{
							param.addr = (pstParams->data[0]==PE_SHP_HW_H15A_RD_CTI1_DISP)? \
								cti_dnr_l_init_h15a0[rd_cnt].addr:\
								cti_dnr_l_init_h15a0[rd_cnt].addr+PE_H15A_ND_DNR_12_DIFF;
							is_valid = PE_SHP_H15_RD_N_HEX_PRINT;
						}
						break;
					case PE_SHP_HW_H15A_RD_CTI1_DFLT:
						table_size = sizeof(PE_CTI_HW_PARAM_REG_H15A0_T);
						param_size = table_size/sizeof(UINT32);
						if (rd_cnt<param_size)
						{
							param.addr = cti_dnr_l_init_h15a0[rd_cnt].addr;
							param.data = p_cti_dflt[rd_cnt];
							is_valid = PE_SHP_H15_HEX_PRINT_ONLY;
						}
						break;
					case PE_SHP_HW_H15A_RD_CTI1_USER:
						table_size = sizeof(PE_CTI_HW_PARAM_REG_H15A0_T);
						param_size = table_size/sizeof(UINT32);
						if (rd_cnt<param_size)
						{
							param.addr = cti_dnr_l_init_h15a0[rd_cnt].addr;
							param.data = p_cti_user[rd_cnt];
							is_valid = PE_SHP_H15_HEX_PRINT_ONLY;
						}
						break;
					case PE_SHP_HW_H15A_RD_CTI1_MASK:
						table_size = sizeof(PE_CTI_HW_PARAM_REG_H15A0_T);
						param_size = table_size/sizeof(UINT32);
						if (rd_cnt<param_size)
						{
							param.addr = cti_dnr_l_init_h15a0[rd_cnt].addr;
							param.data = p_cti_mask[rd_cnt];
							is_valid = PE_SHP_H15_HEX_PRINT_ONLY;
						}
						break;
					case PE_SHP_HW_H15A_RD_CTI1_DATA:
						table_size = sizeof(PE_CTI_HW_PARAM_REG_H15A0_T);
						param_size = table_size/sizeof(UINT32);
						if (rd_cnt<param_size)
						{
							param.addr = cti_dnr_l_init_h15a0[rd_cnt].addr;
							param.data = p_cti_data[rd_cnt];
							is_valid = PE_SHP_H15_HEX_PRINT_ONLY;
						}
						break;
					case PE_SHP_HW_H15A_RD_PBL1_DISP:
					case PE_SHP_HW_H15A_RD_PBL2_DISP:
						table_size = sizeof(pblur_dnr_l_init_h15a0);
						param_size = table_size/sizeof(PE_REG_PARAM_T);
						if (rd_cnt<param_size)
						{
							param.addr = (pstParams->data[0]==PE_SHP_HW_H15A_RD_PBL1_DISP)? \
								pblur_dnr_l_init_h15a0[rd_cnt].addr:\
								pblur_dnr_l_init_h15a0[rd_cnt].addr+PE_H15A_ND_DNR_12_DIFF;
							is_valid = PE_SHP_H15_RD_N_HEX_PRINT;
						}
						break;
					case PE_SHP_HW_H15A_RD_PBL1_DFLT:
						table_size = sizeof(PE_PBL_HW_PARAM_REG_H15A0_T);
						param_size = table_size/sizeof(UINT32);
						if (rd_cnt<param_size)
						{
							param.addr = pblur_dnr_l_init_h15a0[rd_cnt].addr;
							param.data = p_pbl_dflt[rd_cnt];
							is_valid = PE_SHP_H15_HEX_PRINT_ONLY;
						}
						break;
					case PE_SHP_HW_H15A_RD_PBL1_USER:
						table_size = sizeof(PE_PBL_HW_PARAM_REG_H15A0_T);
						param_size = table_size/sizeof(UINT32);
						if (rd_cnt<param_size)
						{
							param.addr = pblur_dnr_l_init_h15a0[rd_cnt].addr;
							param.data = p_pbl_user[rd_cnt];
							is_valid = PE_SHP_H15_HEX_PRINT_ONLY;
						}
						break;
					case PE_SHP_HW_H15A_RD_PBL1_MASK:
						table_size = sizeof(PE_PBL_HW_PARAM_REG_H15A0_T);
						param_size = table_size/sizeof(UINT32);
						if (rd_cnt<param_size)
						{
							param.addr = pblur_dnr_l_init_h15a0[rd_cnt].addr;
							param.data = p_pbl_mask[rd_cnt];
							is_valid = PE_SHP_H15_HEX_PRINT_ONLY;
						}
						break;
					case PE_SHP_HW_H15A_RD_PBL1_DATA:
						table_size = sizeof(PE_PBL_HW_PARAM_REG_H15A0_T);
						param_size = table_size/sizeof(UINT32);
						if (rd_cnt<param_size)
						{
							param.addr = pblur_dnr_l_init_h15a0[rd_cnt].addr;
							param.data = p_pbl_data[rd_cnt];
							is_valid = PE_SHP_H15_HEX_PRINT_ONLY;
						}
						break;
					#ifdef PE_SHP_HW_H15_SRE_SHP_WR_ONLY_MD
					case PE_SHP_HW_H15A_RD_SRS1_DISP:
						table_size = sizeof(sre_res_init_h15a0);
						param_size = table_size/sizeof(PE_REG_PARAM_T);
						if (rd_cnt<param_size)
						{
							param.addr = sre_res_init_h15a0[rd_cnt].addr;
							is_valid = PE_SHP_H15_RD_N_HEX_PRINT;
						}
						break;
					case PE_SHP_HW_H15A_RD_SRS1_DFLT:
						table_size = sizeof(PE_SRS_HW_PARAM_REG_H15A0_T);
						param_size = table_size/sizeof(UINT32);
						if (rd_cnt<param_size)
						{
							param.addr = sre_res_init_h15a0[rd_cnt].addr;
							param.data = p_srs_dflt[rd_cnt];
							is_valid = PE_SHP_H15_HEX_PRINT_ONLY;
						}
						break;
					case PE_SHP_HW_H15A_RD_SRS1_USER:
						table_size = sizeof(PE_SRS_HW_PARAM_REG_H15A0_T);
						param_size = table_size/sizeof(UINT32);
						if (rd_cnt<param_size)
						{
							param.addr = sre_res_init_h15a0[rd_cnt].addr;
							param.data = p_srs_user[rd_cnt];
							is_valid = PE_SHP_H15_HEX_PRINT_ONLY;
						}
						break;
					case PE_SHP_HW_H15A_RD_SRS1_MASK:
						table_size = sizeof(PE_SRS_HW_PARAM_REG_H15A0_T);
						param_size = table_size/sizeof(UINT32);
						if (rd_cnt<param_size)
						{
							param.addr = sre_res_init_h15a0[rd_cnt].addr;
							param.data = p_srs_mask[rd_cnt];
							is_valid = PE_SHP_H15_HEX_PRINT_ONLY;
						}
						break;
					case PE_SHP_HW_H15A_RD_SRS1_DATA:
						table_size = sizeof(PE_SRS_HW_PARAM_REG_H15A0_T);
						param_size = table_size/sizeof(UINT32);
						if (rd_cnt<param_size)
						{
							param.addr = sre_res_init_h15a0[rd_cnt].addr;
							param.data = p_srs_data[rd_cnt];
							is_valid = PE_SHP_H15_HEX_PRINT_ONLY;
						}
						break;
					#endif	//#ifdef PE_SHP_HW_H15_SRE_SHP_WR_ONLY_MD
					default:
						break;
				}
				if (is_valid == PE_SHP_H15_RD_N_HEX_PRINT)
				{
					param.data = PE_REG_H15_RD(param.addr);
					sprintf(buffer, "   [%04d] {0x%04X, 0x%08X},\n", \
						rd_cnt, param.addr, param.data);
					rd_cnt++;
				}
				else if (is_valid == PE_SHP_H15_HEX_PRINT_ONLY)
				{
					sprintf(buffer, "   [%04d] {0x%04X, 0x%08X},\n", \
						rd_cnt, param.addr, param.data);
					rd_cnt++;
				}
				else
				{
					sprintf(buffer, PE_SHP_H15_PRINT_END);
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
 * set resolution common control
 * - use input struct LX_PE_SHP_RE2_CMN_T for H15AX
 * - use input struct LX_PE_SHP_RE1_CMN_T for H15AX
 *
 * @param   *pstParams [in] void
 * @return  OK if success, ERROR otherwise.
 * @see		function description
 * @author
 */
int PE_SHP_HW_H15_SetReCmnCtrl(void *pstParams)
{
	int ret = RET_OK;
	do {
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H15)
		{
		#ifdef PE_SHP_PARAM_CHANGE_FOR_H15
			LX_PE_SHP_RE3_CMN_T *pp=(LX_PE_SHP_RE3_CMN_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			PE_SHP_HW_H15_DBG_PRINT("set[%d] : resolution common ctrl\n"\
				"gain:%d, %d, %d, %d\n"\
				"mode:%d, %d, %d, %d\n"\
				"ti:%d, %d, %d, %d, %d\n"\
				"cti:%d, %d, %d, %d\n"\
				"tgen:%d, %d, %d, %d\n"\
				"tgen_blur x:%d, %d, %d\n"\
				"tgen_blur y:%d, %d, %d\n",\
				pp->win_id, pp->mp_white_gain, pp->mp_black_gain, \
				pp->sp_white_gain, pp->sp_black_gain,\
				pp->mp_lap_h_mode, pp->sp_lap_h_mode, \
				pp->mp_lap_v_mode, pp->sp_lap_v_mode,\
				pp->reg_master_gain_pti_v, pp->reg_master_gain_pti_h, pp->reg_master_gain_sti, \
				pp->reg_edge_gain_sti, pp->reg_texture_gain_sti,\
				pp->tap_size, pp->cti_gain, \
				pp->ycm_y_gain, pp->ycm_c_gain,\
				pp->tg_map_edge_gain, pp->tg_map_detail_gain, \
				pp->tg_map_offset, pp->tg_gs_noise, \
				pp->tg_blur_coef_x0,pp->tg_blur_coef_x1,pp->tg_blur_coef_x2, \
				pp->tg_blur_coef_y0,pp->tg_blur_coef_y1,pp->tg_blur_coef_y2);
			if(PE_CHECK_WIN0(pp->win_id))
			{
				#ifdef PE_HW_H15_PROTECT_OVERLAP_SETTINGS
				PE_SHP_HW_H15A_RES_WR02(pe1_mp_ctrl_01,white_gain,GET_BITS(pp->mp_white_gain,0,7),\
                                                       black_gain,GET_BITS(pp->mp_black_gain,0,7));
				PE_SHP_HW_H15A_RES_WR02(pe1_sp_ctrl_01,white_gain,GET_BITS(pp->sp_white_gain,0,7),\
                                                       black_gain,GET_BITS(pp->sp_black_gain,0,7));
				PE_SHP_HW_H15A_RES_WR02(pe1_mp_ctrl_06,lap_h_mode,GET_BITS(pp->mp_lap_h_mode,0,3),\
                                                       lap_v_mode,GET_BITS(pp->mp_lap_v_mode,0,3));
				#ifdef PE_SHP_HW_H15_PROTECT_SP_LAP_H_MODE_BUG
				pp->sp_lap_h_mode = (pp->sp_lap_h_mode==0x1 || pp->sp_lap_h_mode==0x2)? 0x0:\
									 (pp->sp_lap_h_mode==0x4)? 0x3:0x5;
				#endif
				PE_SHP_HW_H15A_RES_WR02(pe1_sp_ctrl_06,lap_h_mode,GET_BITS(pp->sp_lap_h_mode,0,3),\
                                                       lap_v_mode,GET_BITS(pp->sp_lap_v_mode,0,1));
				PE_SHP_HW_H15A_RES_WR01(pe1_pti_ctrl_00,reg_master_gain,GET_BITS(pp->reg_master_gain_pti_v,0,8));
				PE_SHP_HW_H15A_RES_WR01(pe1_pti_ctrl_01,reg_master_gain,GET_BITS(pp->reg_master_gain_pti_h,0,8));
				PE_SHP_HW_H15A_RES_WR03(pe1_sti_ctrl_00,reg_master_gain,GET_BITS(pp->reg_master_gain_sti,0,8),\
                                                        reg_texture_gain,GET_BITS(pp->reg_texture_gain_sti,0,8),\
                                                        reg_edge_gain,GET_BITS(pp->reg_edge_gain_sti,0,8));
				PE_SHP_HW_H15A_RES_WR04(pe1_tgen_ctrl_08,tg_map_edge_gain,GET_BITS(pp->tg_map_edge_gain,0,5),\
                                                         tg_map_detail_gain,GET_BITS(pp->tg_map_detail_gain,0,5),\
                                                         tg_map_offset,GET_BITS(pp->tg_map_offset,0,8),\
                                                         tg_gs_noise,GET_BITS(pp->tg_gs_noise,0,6));
				PE_SHP_HW_H15A_RES_WR02(pe1_cti_ctrl_00,filter_tap_size,GET_BITS(pp->tap_size,0,3),\
                                                        cti_gain,       GET_BITS(pp->cti_gain,0,8));
				PE_SHP_HW_H15A_RES_WR02(pe1_cti_ctrl_02,ycm_y_gain,GET_BITS(pp->ycm_y_gain,0,4),\
                                                        ycm_c_gain,GET_BITS(pp->ycm_c_gain,0,4));
				PE_SHP_HW_H15A_RES_WR03(pe1_tgen_ctrl_06,tg_blur_coef_x0,GET_BITS(pp->tg_blur_coef_x0,0,6),\
                                                         tg_blur_coef_x1,GET_BITS(pp->tg_blur_coef_x1,0,6),\
                                                         tg_blur_coef_x2,GET_BITS(pp->tg_blur_coef_x2,0,6));
				PE_SHP_HW_H15A_RES_WR03(pe1_tgen_ctrl_07,tg_blur_coef_y0,GET_BITS(pp->tg_blur_coef_y0,0,6),\
                                                         tg_blur_coef_y1,GET_BITS(pp->tg_blur_coef_y1,0,6),\
                                                         tg_blur_coef_y2,GET_BITS(pp->tg_blur_coef_y2,0,6));
				#else
				PE_SHPLR_H15A0_QWr02(pe1_mp_ctrl_01,white_gain,GET_BITS(pp->mp_white_gain,0,7),\
                                                    black_gain,GET_BITS(pp->mp_black_gain,0,7));
				PE_SHPLR_H15A0_QWr02(pe1_sp_ctrl_01,white_gain,GET_BITS(pp->sp_white_gain,0,7),\
                                                    black_gain,GET_BITS(pp->sp_black_gain,0,7));
				PE_SHPLR_H15A0_QWr02(pe1_mp_ctrl_06,lap_h_mode,GET_BITS(pp->mp_lap_h_mode,0,3),\
                                                    lap_v_mode,GET_BITS(pp->mp_lap_h_mode,0,3));
				#ifdef PE_SHP_HW_H15_PROTECT_SP_LAP_H_MODE_BUG
				pp->sp_lap_h_mode = (pp->sp_lap_h_mode==0x1 || pp->sp_lap_h_mode==0x2)? 0x0:\
									 (pp->sp_lap_h_mode==0x4)? 0x3:0x5;
				#endif
				PE_SHPLR_H15A0_QWr02(pe1_sp_ctrl_06,lap_h_mode,GET_BITS(pp->sp_lap_h_mode,0,3),\
                                                    lap_v_mode,GET_BITS(pp->sp_lap_v_mode,0,1));
				PE_SHPLR_H15A0_QWr01(pe1_pti_ctrl_00,reg_master_gain,GET_BITS(pp->reg_master_gain_pti_v,0,8));
				PE_SHPLR_H15A0_QWr01(pe1_pti_ctrl_01,reg_master_gain,GET_BITS(pp->reg_master_gain_pti_h,0,8));
				PE_SHPLR_H15A0_QWr03(pe1_sti_ctrl_00,reg_master_gain,GET_BITS(pp->reg_master_gain_sti,0,8),\
                                                     reg_texture_gain,GET_BITS(pp->reg_texture_gain_sti,0,8),\
                                                     reg_edge_gain,GET_BITS(pp->reg_edge_gain_sti,0,8));
				PE_SHPLR_H15A0_QWr04(pe1_tgen_ctrl_08,tg_map_edge_gain,GET_BITS(pp->tg_map_edge_gain,0,5),\
                                                      tg_map_detail_gain,GET_BITS(pp->tg_map_detail_gain,0,5),\
                                                      tg_map_offset,GET_BITS(pp->tg_map_offset,0,8),\
                                                      tg_gs_noise,GET_BITS(pp->tg_gs_noise,0,6));
				PE_SHPLR_H15A0_QWr02(pe1_cti_ctrl_00,filter_tap_size,GET_BITS(pp->tap_size,0,3),\
                                                     cti_gain,       GET_BITS(pp->cti_gain,0,8));
				PE_SHPLR_H15A0_QWr02(pe1_cti_ctrl_02,ycm_y_gain,GET_BITS(pp->ycm_y_gain,0,4),\
                                                     ycm_c_gain,GET_BITS(pp->ycm_c_gain,0,4));
				PE_SHPLR_H15A0_QWr03(pe1_tgen_ctrl_06,   tg_blur_coef_x0,GET_BITS(pp->tg_blur_coef_x0,0,6),\
                                                         tg_blur_coef_x1,GET_BITS(pp->tg_blur_coef_x1,0,6),\
                                                         tg_blur_coef_x2,GET_BITS(pp->tg_blur_coef_x2,0,6));
				PE_SHPLR_H15A0_QWr03(pe1_tgen_ctrl_07,   tg_blur_coef_y0,GET_BITS(pp->tg_blur_coef_y0,0,6),\
                                                         tg_blur_coef_y1,GET_BITS(pp->tg_blur_coef_y1,0,6),\
                                                         tg_blur_coef_y2,GET_BITS(pp->tg_blur_coef_y2,0,6));
				#endif
			}
		#else 
			LX_PE_SHP_RE2_CMN_T *pp=(LX_PE_SHP_RE2_CMN_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			PE_SHP_HW_H15_DBG_PRINT("set[%d] : resolution common ctrl\n"\
				" mp w,b_gain:%d,%d,  sp w,b_gain:%d,%d\n"\
				" cti   : size:%d, g:%d,%d,%d\n",\
				pp->win_id,pp->mp_white_gain,pp->mp_black_gain,\
				pp->sp_white_gain,pp->sp_black_gain,\
				pp->tap_size,pp->cti_gain,pp->ycm_y_gain,pp->ycm_c_gain);
			if(PE_CHECK_WIN0(pp->win_id))
			{
				#ifdef PE_HW_H15_PROTECT_OVERLAP_SETTINGS
				PE_SHP_HW_H15A_RES_WR02(pe1_mp_ctrl_01,white_gain,GET_BITS(pp->mp_white_gain,0,7),\
                                                       black_gain,GET_BITS(pp->mp_black_gain,0,7));
				PE_SHP_HW_H15A_RES_WR02(pe1_sp_ctrl_01,white_gain,GET_BITS(pp->sp_white_gain,0,7),\
                                                       black_gain,GET_BITS(pp->sp_black_gain,0,7));
				PE_SHP_HW_H15A_RES_WR02(pe1_cti_ctrl_00,filter_tap_size,GET_BITS(pp->tap_size,0,3),\
                                                        cti_gain,       GET_BITS(pp->cti_gain,0,8));
				PE_SHP_HW_H15A_RES_WR02(pe1_cti_ctrl_02,ycm_y_gain,GET_BITS(pp->ycm_y_gain,0,4),\
                                                        ycm_c_gain,GET_BITS(pp->ycm_c_gain,0,4));
				#else
				PE_SHPLR_H15A0_QWr02(pe1_mp_ctrl_01,white_gain,	GET_BITS(pp->mp_white_gain,0,7),\
													black_gain,	GET_BITS(pp->mp_black_gain,0,7));
				PE_SHPLR_H15A0_QWr02(pe1_sp_ctrl_01,white_gain,	GET_BITS(pp->sp_white_gain,0,7),\
													black_gain,	GET_BITS(pp->sp_black_gain,0,7));
				/* cti */
				PE_SHPLR_H15A0_QWr02(pe1_cti_ctrl_00,filter_tap_size,GET_BITS(pp->tap_size,0,3),\
													cti_gain,		GET_BITS(pp->cti_gain,0,8));
				PE_SHPLR_H15A0_QWr02(pe1_cti_ctrl_02,ycm_y_gain,	GET_BITS(pp->ycm_y_gain,0,4),\
													ycm_c_gain,		GET_BITS(pp->ycm_c_gain,0,4));
				#endif
			}
		#endif
		}
		else
		{
			PE_SHP_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	} while (0);
	return ret;
}
/**
 * get resolution common control
 * - use input struct LX_PE_SHP_RE2_CMN_T for H15AX
 * - use input struct LX_PE_SHP_RE1_CMN_T for H15AX
 *
 * @param   *pstParams [in/out] void
 * @return  OK if success, ERROR otherwise.
 * @see		function description
 * @author
 */
int PE_SHP_HW_H15_GetReCmnCtrl(void *pstParams)
{
	int ret = RET_OK;
	LX_PE_WIN_ID win_id;
	do {
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H15)
		{
		#ifdef PE_SHP_PARAM_CHANGE_FOR_H15	
			LX_PE_SHP_RE3_CMN_T *pp=(LX_PE_SHP_RE3_CMN_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			win_id = PE_GET_CHECKED_WINID(pp->win_id);
			if(PE_CHECK_WIN0(win_id))
			{
				PE_SHPL_H15A0_QRd02(pe1_mp_ctrl_01,white_gain,pp->mp_white_gain,\
                                                   black_gain,pp->mp_black_gain);
				PE_SHPL_H15A0_QRd02(pe1_sp_ctrl_01,white_gain,pp->sp_white_gain,\
                                                   black_gain,pp->sp_black_gain);
				PE_SHPL_H15A0_QRd02(pe1_mp_ctrl_06,lap_h_mode,pp->mp_lap_h_mode,\
                                                   lap_v_mode,pp->mp_lap_v_mode);
				PE_SHPL_H15A0_QRd02(pe1_sp_ctrl_06,lap_h_mode,pp->sp_lap_h_mode,\
                                                   lap_v_mode,pp->sp_lap_v_mode);
				PE_SHPL_H15A0_QRd01(pe1_pti_ctrl_00,reg_master_gain, pp->reg_master_gain_pti_v);
				PE_SHPL_H15A0_QRd01(pe1_pti_ctrl_01,reg_master_gain, pp->reg_master_gain_pti_h);
				PE_SHPL_H15A0_QRd03(pe1_sti_ctrl_00,reg_master_gain, pp->reg_master_gain_sti,\
                                                    reg_texture_gain,pp->reg_texture_gain_sti,\
                                                    reg_edge_gain,   pp->reg_edge_gain_sti);
				PE_SHPL_H15A0_QRd04(pe1_tgen_ctrl_08,tg_map_edge_gain,   pp->tg_map_edge_gain,\
                                                     tg_map_detail_gain, pp->tg_map_detail_gain,\
                                                     tg_map_offset,      pp->tg_map_offset,\
                                                     tg_gs_noise,        pp->tg_gs_noise);
				PE_SHPL_H15A0_QRd02(pe1_cti_ctrl_00,filter_tap_size,pp->tap_size,\
                                                    cti_gain,       pp->cti_gain);
				PE_SHPL_H15A0_QRd02(pe1_cti_ctrl_02,ycm_y_gain,     pp->ycm_y_gain,\
                                                    ycm_c_gain,     pp->ycm_c_gain);
				PE_SHPL_H15A0_QRd03(pe1_tgen_ctrl_06,tg_blur_coef_x0,pp->tg_blur_coef_x0,\
                                                     tg_blur_coef_x1,pp->tg_blur_coef_x1,\
                                                     tg_blur_coef_x2,pp->tg_blur_coef_x2);
				PE_SHPL_H15A0_QRd03(pe1_tgen_ctrl_07,tg_blur_coef_y0,pp->tg_blur_coef_y0,\
                                                     tg_blur_coef_y1,pp->tg_blur_coef_y1,\
                                                     tg_blur_coef_y2,pp->tg_blur_coef_y2);
			}
			if(PE_CHECK_WIN1(win_id))
			{
				PE_SHPR_H15A0_QRd02(pe1_mp_ctrl_01,white_gain,pp->mp_white_gain,\
                                                   black_gain,pp->mp_black_gain);
				PE_SHPR_H15A0_QRd02(pe1_sp_ctrl_01,white_gain,pp->sp_white_gain,\
                                                   black_gain,pp->sp_black_gain);
				PE_SHPR_H15A0_QRd02(pe1_mp_ctrl_06,lap_h_mode,pp->mp_lap_h_mode,\
                                                   lap_v_mode,pp->mp_lap_v_mode);
				PE_SHPR_H15A0_QRd02(pe1_sp_ctrl_06,lap_h_mode,pp->sp_lap_h_mode,\
                                                   lap_v_mode,pp->sp_lap_v_mode);
				PE_SHPR_H15A0_QRd01(pe1_pti_ctrl_00,reg_master_gain, pp->reg_master_gain_pti_v);
				PE_SHPR_H15A0_QRd01(pe1_pti_ctrl_01,reg_master_gain, pp->reg_master_gain_pti_h);
				PE_SHPR_H15A0_QRd03(pe1_sti_ctrl_00,reg_master_gain, pp->reg_master_gain_sti,\
                                                    reg_texture_gain,pp->reg_texture_gain_sti,\
                                                    reg_edge_gain,   pp->reg_edge_gain_sti);
				PE_SHPR_H15A0_QRd04(pe1_tgen_ctrl_08,tg_map_edge_gain,   pp->tg_map_edge_gain,\
                                                     tg_map_detail_gain, pp->tg_map_detail_gain,\
                                                     tg_map_offset,      pp->tg_map_offset,\
                                                     tg_gs_noise,        pp->tg_gs_noise);
				PE_SHPR_H15A0_QRd02(pe1_cti_ctrl_00,filter_tap_size,pp->tap_size,\
                                                    cti_gain,       pp->cti_gain);
				PE_SHPR_H15A0_QRd02(pe1_cti_ctrl_02,ycm_y_gain,     pp->ycm_y_gain,\
                                                    ycm_c_gain,     pp->ycm_c_gain);
				PE_SHPR_H15A0_QRd03(pe1_tgen_ctrl_06,tg_blur_coef_x0,pp->tg_blur_coef_x0,\
                                                     tg_blur_coef_x1,pp->tg_blur_coef_x1,\
                                                     tg_blur_coef_x2,pp->tg_blur_coef_x2);
				PE_SHPR_H15A0_QRd03(pe1_tgen_ctrl_07,tg_blur_coef_y0,pp->tg_blur_coef_y0,\
                                                     tg_blur_coef_y1,pp->tg_blur_coef_y1,\
                                                     tg_blur_coef_y2,pp->tg_blur_coef_y2);
			}
			PE_SHP_HW_H15_DBG_PRINT("get[%d] : resolution common ctrl\n"\
				"mp/sp gain:%d, %d, %d, %d\n"\
				"mode:%d, %d, %d, %d\n"\
				"ti:%d, %d, %d, %d, %d\n"\
				"cti:%d, %d, %d, %d\n"\
				"tgen:%d, %d, %d, %d\n"\
				"tgen_blur x:%d, %d, %d\n"\
				"tgen_blur y:%d, %d, %d\n",\
				pp->win_id, pp->mp_white_gain, pp->mp_black_gain, \
				pp->sp_white_gain, pp->sp_black_gain,\
				pp->mp_lap_h_mode, pp->sp_lap_h_mode, \
				pp->mp_lap_v_mode, pp->sp_lap_v_mode,\
				pp->reg_master_gain_pti_v, pp->reg_master_gain_pti_h, pp->reg_master_gain_sti, \
				pp->reg_edge_gain_sti, pp->reg_texture_gain_sti,\
				pp->tap_size, pp->cti_gain, \
				pp->ycm_y_gain, pp->ycm_c_gain,\
				pp->tg_map_edge_gain, pp->tg_map_detail_gain, \
				pp->tg_map_offset, pp->tg_gs_noise, \
				pp->tg_blur_coef_x0,pp->tg_blur_coef_x1,pp->tg_blur_coef_x2, \
				pp->tg_blur_coef_y0,pp->tg_blur_coef_y1,pp->tg_blur_coef_y2);
		#else
			LX_PE_SHP_RE2_CMN_T *pp=(LX_PE_SHP_RE2_CMN_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			win_id = PE_GET_CHECKED_WINID(pp->win_id);
			if(PE_CHECK_WIN0(win_id))
			{
				PE_SHPL_H15A0_QRd02(pe1_mp_ctrl_01,	white_gain,		pp->mp_white_gain,\
													black_gain,		pp->mp_black_gain);
				PE_SHPL_H15A0_QRd02(pe1_sp_ctrl_01,	white_gain,		pp->sp_white_gain,\
													black_gain,		pp->sp_black_gain);
				/* cti */
				PE_SHPL_H15A0_QRd02(pe1_cti_ctrl_00,filter_tap_size,pp->tap_size,\
													cti_gain,		pp->cti_gain);
				PE_SHPL_H15A0_QRd02(pe1_cti_ctrl_02,ycm_y_gain,		pp->ycm_y_gain,\
													ycm_c_gain,		pp->ycm_c_gain);
			}
			if(PE_CHECK_WIN1(win_id))
			{
				PE_SHPR_H15A0_QRd02(pe1_mp_ctrl_01,	white_gain,		pp->mp_white_gain,\
													black_gain,		pp->mp_black_gain);
				PE_SHPR_H15A0_QRd02(pe1_sp_ctrl_01,	white_gain,		pp->sp_white_gain,\
													black_gain,		pp->sp_black_gain);
				/* cti */
				PE_SHPR_H15A0_QRd02(pe1_cti_ctrl_00,filter_tap_size,pp->tap_size,\
													cti_gain,		pp->cti_gain);
				PE_SHPR_H15A0_QRd02(pe1_cti_ctrl_02,ycm_y_gain,		pp->ycm_y_gain,\
													ycm_c_gain,		pp->ycm_c_gain);
			}
			PE_SHP_HW_H15_DBG_PRINT("get[%d] : resolution common ctrl\n"\
				" mp w,b_gain:%d,%d,  sp w,b_gain:%d,%d\n"\
				" cti   :size:%d, g:%d,%d,%d\n",\
				pp->win_id,pp->mp_white_gain,pp->mp_black_gain,\
				pp->sp_white_gain,pp->sp_black_gain,\
				pp->tap_size,pp->cti_gain,pp->ycm_y_gain,pp->ycm_c_gain);
		#endif
		}
		else
		{
			PE_SHP_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * set resolution horizontal control
 * - use input struct LX_PE_SHP_RE2_HOR_T for H15AX
 * - use input struct LX_PE_SHP_RE1_HOR_T for H15AX
 *
 * @param   *pstParams [in/out] void
 * @return  OK if success, ERROR otherwise.
 * @see		function description
 * @author
 */
int PE_SHP_HW_H15_SetReHorCtrl(void *pstParams)
{
	int ret = RET_OK;
	do {
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H15)
		{
			LX_PE_SHP_RE2_HOR_T *pp=(LX_PE_SHP_RE2_HOR_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			PE_SHP_HW_H15_DBG_PRINT("set[%d] : resolution horizontal ctrl\n"\
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
				PE_FWI_H15_HOR_CTRL fw_param;
				fw_param.e_gain_th1 = GET_BITS(pp->e_gain_th1,0,8);
				fw_param.e_gain_th2 = GET_BITS(pp->e_gain_th2,0,8);
				ret = PE_FWI_H15_SetShpHorCtrl(&fw_param);
				PE_SHP_HW_H15_CHECK_CODE(ret, break, \
					"[%s,%d] PE_FWI_H15_SetShpHorCtrl() error.\n", __F__, __L__);
				#ifdef PE_HW_H15_PROTECT_OVERLAP_SETTINGS
				PE_SHP_HW_H15A_RES_WR01(pe1_derh_ctrl_00,a_gen_width,GET_BITS(pp->a_gen_width,0,4));
				PE_SHP_HW_H15A_RES_WR01(pe1_derh_ctrl_05,reg_csft_gain,GET_BITS(pp->reg_csft_gain,0,6));
				PE_SHP_HW_H15A_RES_WR02(pe1_derh_ctrl_06,edge_filter_white_gain,GET_BITS(pp->edge_filter_white_gain,0,6),\
                                                         edge_filter_black_gain,GET_BITS(pp->edge_filter_black_gain,0,6));
				PE_SHP_HW_H15A_RES_WR01(pe1_mp_ctrl_01,horizontal_gain,GET_BITS(pp->mp_horizontal_gain,0,8));
				PE_SHP_HW_H15A_RES_WR01(pe1_sp_ctrl_01,horizontal_gain,GET_BITS(pp->sp_horizontal_gain,0,8));
				PE_SHP_HW_H15A_RES_WR02(pe1_derh_ctrl_0b,f_gain_th1,GET_BITS(pp->f_gain_th1,0,8), \
                                                         f_gain_th2,GET_BITS(pp->f_gain_th2,0,8));
				#ifdef PE_SHP_HW_H15A_SET_FW_PARAM_TO_REG_DIRECTLY
				PE_SHP_HW_H15A_RES_WR02(pe1_derh_ctrl_0b,e_gain_th1,GET_BITS(pp->e_gain_th1,0,8), \
                                                         e_gain_th2,GET_BITS(pp->e_gain_th2,0,8));
				#else
				PE_SHP_HW_H15A_RES_SET_USER(pe1_derh_ctrl_0b,e_gain_th1,GET_BITS(pp->e_gain_th1,0,8));
				PE_SHP_HW_H15A_RES_SET_USER(pe1_derh_ctrl_0b,e_gain_th2,GET_BITS(pp->e_gain_th2,0,8));
				#endif
				#else
				PE_SHPLR_H15A0_QWr01(pe1_derh_ctrl_00,a_gen_width,  GET_BITS(pp->a_gen_width,0,4));
				PE_SHPLR_H15A0_QWr01(pe1_derh_ctrl_05,reg_csft_gain,GET_BITS(pp->reg_csft_gain,0,6));
				PE_SHPLR_H15A0_QWr02(pe1_derh_ctrl_06,edge_filter_white_gain,GET_BITS(pp->edge_filter_white_gain,0,6),\
													edge_filter_black_gain,	GET_BITS(pp->edge_filter_black_gain,0,6));
				PE_SHPLR_H15A0_QWr01(pe1_mp_ctrl_01,horizontal_gain,GET_BITS(pp->mp_horizontal_gain,0,8));
				PE_SHPLR_H15A0_QWr01(pe1_sp_ctrl_01,horizontal_gain,GET_BITS(pp->sp_horizontal_gain,0,8));
				PE_SHPLR_H15A0_QWr02(pe1_derh_ctrl_0b,f_gain_th1,GET_BITS(pp->f_gain_th1,0,8),\
													f_gain_th2,	GET_BITS(pp->f_gain_th2,0,8));
				
				#ifdef PE_HW_H15_FOR_BRINGUP
				#else
				PE_SHPLR_H15A0_QWr03(pe1_ti_ctrl_0,coring_th,	GET_BITS(pp->coring_th,0,8),\
												  	y_gain,		GET_BITS(pp->y_gain,0,8),\
												  	c_gain,		GET_BITS(pp->c_gain,0,8));
				#endif
				#ifdef PE_SHP_HW_H15A_SET_FW_PARAM_TO_REG_DIRECTLY
				PE_SHPLR_H15A0_QWr02(pe1_derh_ctrl_0b,e_gain_th1,GET_BITS(pp->e_gain_th1,0,8), \
                                                      e_gain_th2,GET_BITS(pp->e_gain_th2,0,8));
				#endif
				#endif
			}
		}
		else
		{
			PE_SHP_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	} while (0);
	return ret;
}
/**
 * get resolution horizontal control
 * - use input struct LX_PE_SHP_RE2_HOR_T for H15AX
 * - use input struct LX_PE_SHP_RE1_HOR_T for H15AX
 *
 * @param   *pstParams [in/out] void
 * @return  OK if success, ERROR otherwise.
 * @see		function description
 * @author
 */
int PE_SHP_HW_H15_GetReHorCtrl(void *pstParams)
{
	int ret = RET_OK;
	LX_PE_WIN_ID win_id;
	do {
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H15)
		{
			LX_PE_SHP_RE2_HOR_T *pp=(LX_PE_SHP_RE2_HOR_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			win_id = PE_GET_CHECKED_WINID(pp->win_id);
			if(PE_CHECK_WIN0(win_id))
			{
				PE_FWI_H15_SETTINGS_T fw_inf;
				ret = PE_FWI_H15_GetCurFwiSettings(&fw_inf);
				PE_SHP_HW_H15_CHECK_CODE(ret, break, \
					"[%s,%d] PE_FWI_H15_GetCurFwiSettings() error.\n", __F__, __L__);
				PE_SHP_HW_H15_DBG_PRINT("fw_ctrl.enable : %s.\n", fw_inf.fw_ctrl.enable? "yes":"no");
				#ifdef PE_SHP_HW_H15A_SET_FW_PARAM_TO_REG_DIRECTLY
				PE_SHPL_H15A0_QRd02(pe1_derh_ctrl_0b,e_gain_th1,pp->e_gain_th1,\
                                                     e_gain_th2,pp->e_gain_th2);
				#else
				if(fw_inf.fw_ctrl.enable)
				{
					pp->e_gain_th1 = fw_inf.shp_h_ctrl.e_gain_th1;
					pp->e_gain_th2 = fw_inf.shp_h_ctrl.e_gain_th2;
				}
				else
				{
					PE_SHPL_H15A0_QRd02(pe1_derh_ctrl_0b, e_gain_th1,	pp->e_gain_th1,\
														 e_gain_th2,	pp->e_gain_th2);
				}
				#endif
				PE_SHPL_H15A0_QRd01(pe1_derh_ctrl_00,	a_gen_width,		pp->a_gen_width);
				PE_SHPL_H15A0_QRd01(pe1_derh_ctrl_05,	reg_csft_gain,		pp->reg_csft_gain);
				PE_SHPL_H15A0_QRd02(pe1_derh_ctrl_06,	edge_filter_white_gain,pp->edge_filter_white_gain,\
													 	edge_filter_black_gain,pp->edge_filter_black_gain);
				PE_SHPL_H15A0_QRd01(pe1_mp_ctrl_01, 	horizontal_gain,	pp->mp_horizontal_gain);
				PE_SHPL_H15A0_QRd01(pe1_sp_ctrl_01, 	horizontal_gain,	pp->sp_horizontal_gain);
				PE_SHPL_H15A0_QRd02(pe1_derh_ctrl_0b, 	f_gain_th1,			pp->f_gain_th1,\
													 	f_gain_th2,			pp->f_gain_th2);				
				#ifndef PE_HW_H15_FOR_BRINGUP
				PE_SHPL_H15A0_QRd03(pe1_ti_ctrl_0, 	coring_th,		pp->coring_th,\
												  	y_gain,			pp->y_gain,\
												  	c_gain,			pp->c_gain);
			#endif
			}
			if(PE_CHECK_WIN1(win_id))
			{
				PE_FWI_H15_SETTINGS_T fw_inf;
				ret = PE_FWI_H15_GetCurFwiSettings(&fw_inf);
				PE_SHP_HW_H15_CHECK_CODE(ret, break, \
					"[%s,%d] PE_FWI_H15_GetCurFwiSettings() error.\n", __F__, __L__);
				PE_SHP_HW_H15_DBG_PRINT("fw_ctrl.enable : %s.\n", fw_inf.fw_ctrl.enable? "yes":"no");
				#ifdef PE_SHP_HW_H15A_SET_FW_PARAM_TO_REG_DIRECTLY
				PE_SHPR_H15A0_QRd02(pe1_derh_ctrl_0b, e_gain_th1,	pp->e_gain_th1,\
														e_gain_th2,	pp->e_gain_th2);
				#else
				if(fw_inf.fw_ctrl.enable)
				{
					pp->e_gain_th1 = fw_inf.shp_h_ctrl.e_gain_th1;
					pp->e_gain_th2 = fw_inf.shp_h_ctrl.e_gain_th2;
				}
				else
				{
					PE_SHPR_H15A0_QRd02(pe1_derh_ctrl_0b, e_gain_th1,	pp->e_gain_th1,\
														 e_gain_th2,	pp->e_gain_th2);
				}
				#endif
				PE_SHPR_H15A0_QRd01(pe1_derh_ctrl_00,	a_gen_width,		pp->a_gen_width);
				PE_SHPR_H15A0_QRd01(pe1_derh_ctrl_05,	reg_csft_gain,		pp->reg_csft_gain);
				PE_SHPR_H15A0_QRd02(pe1_derh_ctrl_06,	edge_filter_white_gain,pp->edge_filter_white_gain,\
													 	edge_filter_black_gain,pp->edge_filter_black_gain);
				PE_SHPR_H15A0_QRd01(pe1_mp_ctrl_01, 	horizontal_gain,	pp->mp_horizontal_gain);
				PE_SHPR_H15A0_QRd01(pe1_sp_ctrl_01, 	horizontal_gain,	pp->sp_horizontal_gain);
				PE_SHPR_H15A0_QRd02(pe1_derh_ctrl_0b, 	f_gain_th1,			pp->f_gain_th1,\
													 	f_gain_th2,			pp->f_gain_th2);				
				#ifndef PE_HW_H15_FOR_BRINGUP
				PE_SHPR_H15A0_QRd03(pe1_ti_ctrl_0, 	coring_th,		pp->coring_th,\
												  	y_gain,			pp->y_gain,\
												  	c_gain,			pp->c_gain);
				#endif
			}
			PE_SHP_HW_H15_DBG_PRINT("get[%d] : resolution horizontal ctrl\n"\
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
			PE_SHP_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * set resolution vertical control
 * - use input struct LX_PE_SHP_RE2_VER_T for H15AX
 * - use input struct LX_PE_SHP_RE1_VER_T for H15AX
 *
 * @param   *pstParams [in] void
 * @return  OK if success, ERROR otherwise.
 * @see		function description
 * @author
 */
int PE_SHP_HW_H15_SetReVerCtrl(void *pstParams)
{
	int ret = RET_OK;
	do {
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H15)
		{
			LX_PE_SHP_RE2_VER_T *pp=(LX_PE_SHP_RE2_VER_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			PE_SHP_HW_H15_DBG_PRINT("set[%d] : resolution vertical ctrl\n"\
				" manual_th:%d,csft_gain:%d, gain(b,w)%d,%d,\n"\
				"mmd_sel:%d, gain(th1,th2):%d,%d mp,sp_g:%d,%d\n",\
				pp->win_id,pp->bif_manual_th,pp->csft_gain,\
				pp->gain_b,pp->gain_w,pp->mmd_sel,pp->gain_th1,pp->gain_th2,\
				pp->mp_vertical_gain,pp->sp_vertical_gain);
			if(PE_CHECK_WIN0(pp->win_id))
			{
				#ifdef PE_HW_H15_PROTECT_OVERLAP_SETTINGS
				PE_SHP_HW_H15A_RES_WR01(pe1_derv_ctrl_0,bif_manual_th,GET_BITS(pp->bif_manual_th,0,8));
				PE_SHP_HW_H15A_RES_WR01(pe1_derv_ctrl_1,csft_gain,GET_BITS(pp->csft_gain,0,6));
				PE_SHP_HW_H15A_RES_WR02(pe1_derv_ctrl_3,gain_b,GET_BITS(pp->gain_b,0,7),\
                                                        gain_w,GET_BITS(pp->gain_w,0,7));
				PE_SHP_HW_H15A_RES_WR03(pe1_derv_ctrl_2,mmd_sel,GET_BITS(pp->mmd_sel,0,3),\
                                                        gain_th1,GET_BITS(pp->gain_th1,0,8),\
                                                        gain_th2,GET_BITS(pp->gain_th2,0,8));
				PE_SHP_HW_H15A_RES_WR01(pe1_mp_ctrl_01,vertical_gain,GET_BITS(pp->mp_vertical_gain,0,8));
				PE_SHP_HW_H15A_RES_WR01(pe1_sp_ctrl_01,vertical_gain,GET_BITS(pp->sp_vertical_gain,0,8));
				#else
				PE_SHPLR_H15A0_QWr01(pe1_derv_ctrl_0,	bif_manual_th,	GET_BITS(pp->bif_manual_th,0,8));
				PE_SHPLR_H15A0_QWr01(pe1_derv_ctrl_1,	csft_gain,		GET_BITS(pp->csft_gain,0,6));
				PE_SHPLR_H15A0_QWr02(pe1_derv_ctrl_3,	gain_b,		GET_BITS(pp->gain_b,0,6),\
														gain_w,		GET_BITS(pp->gain_w,0,6));
				PE_SHPLR_H15A0_QWr03(pe1_derv_ctrl_2,	mmd_sel,	GET_BITS(pp->mmd_sel,0,3),\
														gain_th1,	GET_BITS(pp->gain_th1,0,8),\
														gain_th2,	GET_BITS(pp->gain_th2,0,8));
				PE_SHPLR_H15A0_QWr01(pe1_mp_ctrl_01,	vertical_gain,	GET_BITS(pp->mp_vertical_gain,0,8));
				PE_SHPLR_H15A0_QWr01(pe1_sp_ctrl_01,	vertical_gain,	GET_BITS(pp->sp_vertical_gain,0,8));
				#endif
			}
		}
		else
		{
			PE_SHP_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	} while (0);
	return ret;
}
/**
 * get resolution vertical control
 * - use input struct LX_PE_SHP_RE2_VER_T for H15AX
 * - use input struct LX_PE_SHP_RE1_VER_T for H15AX
 *
 * @param   *pstParams [in/out] void
 * @return  OK if success, ERROR otherwise.
 * @see		function description
 * @author
 */
int PE_SHP_HW_H15_GetReVerCtrl(void *pstParams)
{
	int ret = RET_OK;
	LX_PE_WIN_ID win_id;
	do {
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H15)
		{
			LX_PE_SHP_RE2_VER_T *pp=(LX_PE_SHP_RE2_VER_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			win_id = PE_GET_CHECKED_WINID(pp->win_id);
			if(PE_CHECK_WIN0(win_id))
			{
				PE_SHPL_H15A0_QRd01(pe1_derv_ctrl_0, bif_manual_th,	pp->bif_manual_th);
				PE_SHPL_H15A0_QRd01(pe1_derv_ctrl_1, csft_gain,		pp->csft_gain);
				PE_SHPL_H15A0_QRd02(pe1_derv_ctrl_3, gain_b,		pp->gain_b,\
													 gain_w,		pp->gain_w);
				PE_SHPL_H15A0_QRd03(pe1_derv_ctrl_2, mmd_sel,		pp->mmd_sel,\
													 gain_th1,		pp->gain_th1,\
													 gain_th2,		pp->gain_th2);
				PE_SHPL_H15A0_QRd01(pe1_mp_ctrl_01, vertical_gain,	pp->mp_vertical_gain);
				PE_SHPL_H15A0_QRd01(pe1_sp_ctrl_01, vertical_gain,	pp->sp_vertical_gain);
			}
			if(PE_CHECK_WIN1(win_id))
			{
				PE_SHPR_H15A0_QRd01(pe1_derv_ctrl_0, bif_manual_th,	pp->bif_manual_th);
				PE_SHPR_H15A0_QRd01(pe1_derv_ctrl_1, csft_gain,		pp->csft_gain);
				PE_SHPR_H15A0_QRd02(pe1_derv_ctrl_3, gain_b,		pp->gain_b,\
													 gain_w,		pp->gain_w);
				PE_SHPR_H15A0_QRd03(pe1_derv_ctrl_2, mmd_sel,		pp->mmd_sel,\
													 gain_th1,		pp->gain_th1,\
													 gain_th2,		pp->gain_th2);
				PE_SHPR_H15A0_QRd01(pe1_mp_ctrl_01, vertical_gain,	pp->mp_vertical_gain);
				PE_SHPR_H15A0_QRd01(pe1_sp_ctrl_01, vertical_gain,	pp->sp_vertical_gain);
			}
			PE_SHP_HW_H15_DBG_PRINT("get[%d] : resolution vertical ctrl\n"\
				" manual_th:%d,csft_gain:%d, gain(b,w)%d,%d,\n"\
				"mmd_sel:%d, gain(th1,th2):%d,%d mp,sp_g:%d,%d\n",\
				pp->win_id,pp->bif_manual_th,pp->csft_gain,\
				pp->gain_b,pp->gain_w,pp->mmd_sel,pp->gain_th1,pp->gain_th2,\
				pp->mp_vertical_gain,pp->sp_vertical_gain);
		}
		else
		{
			PE_SHP_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * set resolution misc control
 * - use input struct LX_PE_SHP_RE2_MISC_T for H15AX
 * - use input struct LX_PE_SHP_RE1_MISC_T for H15AX
 *
 * @param   *pstParams [in] void
 * @return  OK if success, ERROR otherwise.
 * @see		function description
 * @author
 */
int PE_SHP_HW_H15_SetReMiscCtrl(void *pstParams)
{
	int ret = RET_OK;
	
	do {
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H15)
		{
			#ifdef PE_SHP_PARAM_CHANGE_FOR_H15
			LX_PE_SHP_RE4_MISC_T *pp=(LX_PE_SHP_RE4_MISC_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			PE_SHP_HW_H15_DBG_PRINT("set[%d] :  resolution misc ctrl\n"\
				" hor : 0x%02x,0x%02x,0x%02x\n"\
				" ver : 0x%02x,0x%02x,0x%02x\n"\
				" cmn : 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" djg : 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" djg : 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" emp : 0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" emp : 0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" emp : 0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" emp : 0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" emp : 0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" tmp : 0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" tmp : 0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" tmp : 0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" tmp : 0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" blc : 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" blc : 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" blc : 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" blc : 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" blc : 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" blc : 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" ptiv: 0x%02x,0x%02x,0x%02x\n"\
				" ptih: 0x%02x,0x%02x,0x%02x\n"\
				" sti : 0x%02x,0x%02x,0x%02x\n"\
				" cti : 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" tgen: 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" tgen: 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" tgen: 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" tgen: 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n",\
				pp->win_id, \
				/* h 3 */
				pp->edge_Y_filter_en, pp->e_gain_max, pp->f_gain_max, \
				/* v 3 */
				pp->der_v_en, pp->der_gain_mapping, pp->max_sel, \
				/* cmm 5 */
				pp->mp_laplacian_weight,pp->sp_laplacian_weight, \
				pp->flat_filter_en,pp->flat_filter_type,pp->flat_filter_gain,\
				/*djeg 12*/
				pp->edf_en,pp->center_blur_mode,pp->count_diff_th,pp->n_avg_mode,\
				pp->line_variation_diff_threshold,\
				pp->level_th,pp->protect_th,pp->n_avg_gain,\
				pp->reg_g0_cnt_min,pp->reg_g0_mul,pp->reg_g1_protect_min,pp->reg_g1_mul,\
				/*amap 20*/
				pp->reg_cross_th_sc,pp->reg_cross_th,pp->reg_mul_base_t,pp->reg_mul_base_e,\
				pp->reg_a_map_h_size,pp->reg_a_nomalize_en,pp->reg_edge_alpha_mul	,pp->reg_edge_beta_mul,\
				pp->reg_a_lut_edge_x0,pp->reg_a_lut_edge_y0,pp->reg_a_lut_edge_x1,pp->reg_a_lut_edge_y1,\
				pp->reg_a_lut_detail_x0,pp->reg_a_lut_detail_y0,pp->reg_a_lut_detail_x1,pp->reg_a_lut_detail_y1,\
				pp->reg_edge_minmax_x0,pp->reg_edge_minmax_y0,pp->reg_edge_minmax_x1,pp->reg_edge_minmax_y1,\
				/* tmap 16*/
				pp->reg_t_cut_resolution,pp->reg_t_nomalize_en,pp->reg_detail_alpha_mul,pp->reg_detail_beta_mul,\
				pp->reg_t_lut_edge_x0,pp->reg_t_lut_edge_y0,pp->reg_t_lut_edge_x1,pp->reg_t_lut_edge_y1,\
				pp->reg_t_lut_detail_x0,pp->reg_t_lut_detail_y0,pp->reg_t_lut_detail_x1,pp->reg_t_lut_detail_y1,\
				pp->reg_detail_minmax_x0,pp->reg_detail_minmax_y0,pp->reg_detail_minmax_x1,pp->reg_detail_minmax_y1,\
				/* balnce 42*/
				pp->mp_gb_en,pp->mp_gb_mode,\
				pp->mp_gb_x1,pp->mp_gb_x2,pp->mp_gb_y1,pp->mp_gb_y2,pp->mp_gb_y3,\
				pp->sp_gb_en,pp->sp_gb_mode,\
				pp->sp_gb_x1,pp->sp_gb_x2,pp->sp_gb_y1,pp->sp_gb_y2,pp->sp_gb_y3,\
				pp->mp_lum1_x_L0,pp->mp_lum1_x_L1,pp->mp_lum1_x_H0,pp->mp_lum1_x_H1,\
				pp->mp_lum1_y0,pp->mp_lum1_y1,pp->mp_lum1_y2,\
				pp->mp_lum2_x_L0,pp->mp_lum2_x_L1,pp->mp_lum2_x_H0,pp->mp_lum2_x_H1,\
				pp->mp_lum2_y0,pp->mp_lum2_y1,pp->mp_lum2_y2,\
				pp->sp_lum1_x_L0,pp->sp_lum1_x_L1,pp->sp_lum1_x_H0,pp->sp_lum1_x_H1,\
				pp->sp_lum1_y0,pp->sp_lum1_y1,pp->sp_lum1_y2,\
				pp->sp_lum2_x_L0,pp->sp_lum2_x_L1,pp->sp_lum2_x_H0,pp->sp_lum2_x_H1,\
				pp->sp_lum2_y0,pp->sp_lum2_y1,pp->sp_lum2_y2,\
				/* ti 9*/
				pp->reg_ti_enable_pti_v,pp->reg_mm_tap_size_pti_v,pp->reg_avg_tap_size_pti_v,\
				pp->reg_ti_enable_pti_h,pp->reg_mm_tap_size_pti_h,pp->reg_avg_tap_size_pti_h,\
				pp->reg_enable_sti,pp->reg_mm_tap_size_sti,pp->reg_avg_tap_size_sti,\
				/* cti 8 */
				pp->cti_en, pp->coring_th0, \
				pp->coring_th1, pp->coring_map_filter, \
				pp->coring_tap_size, pp->ycm_en1, \
				pp->ycm_band_sel, pp->ycm_diff_th,\
				/* tgen 23*/
				pp->tgen_on_off,pp->tg_delta_H_bound,pp->tg_delta_L_bound,pp->tg_delta_max,pp->tg_delta_scale,\
				pp->tg_rnd_th,pp->tg_delta_wcurr,pp->tg_lc_ldr_th,\
				pp->tg_dr_ratio,pp->tg_dr_th,pp->tg_mm_offset,pp->tg_mm_sel_mode,\
				pp->tg_lavg_th,pp->tg_gs_rnd,pp->tg_gs_mm,\
				pp->tg_gain0,pp->tg_gain1,pp->tg_gain2,pp->tg_gain3,\
				pp->tg_gain4,pp->tg_gain5,pp->tg_gain6,pp->tg_gain7);
			if(PE_CHECK_WIN0(pp->win_id))
			{
				#if 0
				PE_FWI_H15_MISC_CTRL fw_param;
				fw_param.gt_th1 = GET_BITS(pp->gt_th1,0,8);
				fw_param.gt_th0 = GET_BITS(pp->gt_th0,0,8);
				fw_param.gt_th0a = GET_BITS(pp->gt_th0a,0,8);
				fw_param.gt_th0b = GET_BITS(pp->gt_th0b,0,8);
				ret = PE_FWI_H15_SetShpMiscCtrl(&fw_param);
				PE_SHP_HW_H15_CHECK_CODE(ret, break, \
					"[%s,%d] PE_FWI_H15_SetShpMiscCtrl() error.\n", __F__, __L__);
				#endif
				#ifdef PE_HW_H15_PROTECT_OVERLAP_SETTINGS
				/* h 3 */
				PE_SHP_HW_H15A_RES_WR01(pe1_derh_ctrl_06,edge_y_filter_en,	GET_BITS(pp->edge_Y_filter_en,0,1));
				PE_SHP_HW_H15A_RES_WR02(pe1_derh_ctrl_0c,e_gain_max,		GET_BITS(pp->e_gain_max,0,6),\
														f_gain_max,			GET_BITS(pp->f_gain_max,0,6));
				/* v 3 */
				PE_SHP_HW_H15A_RES_WR02(pe1_derv_ctrl_0,der_v_en,			GET_BITS(pp->der_v_en,0,1),\
														der_gain_mapping,	GET_BITS(pp->der_gain_mapping,0,2));
				PE_SHP_HW_H15A_RES_WR01(pe1_derv_ctrl_2,max_sel,			GET_BITS(pp->max_sel,0,3));
				/* cmm 6 */
				PE_SHP_HW_H15A_RES_WR01(pe1_mp_ctrl_02,	laplacian_weight,	GET_BITS(pp->mp_laplacian_weight,0,8));
				PE_SHP_HW_H15A_RES_WR01(pe1_sp_ctrl_02,	laplacian_weight,	GET_BITS(pp->sp_laplacian_weight,0,8));
				PE_SHP_HW_H15A_RES_WR03(pe1_derh_ctrl_07,flat_filter_en,	GET_BITS(pp->flat_filter_en,0,1),\
														flat_filter_gain,	GET_BITS(pp->flat_filter_gain,0,6),\
														flat_filter_type,	GET_BITS(pp->flat_filter_type,0,1));
				/*d_jag 12 */
				PE_SHP_HW_H15A_RES_WR05(pe1_dj_ctrl_00,	edf_en,				GET_BITS(pp->edf_en,0,1),\
														center_blur_mode, 	GET_BITS(pp->center_blur_mode,0,1),\
														count_diff_th, 		GET_BITS(pp->count_diff_th,0,5),\
														n_avg_mode,			GET_BITS(pp->n_avg_mode,0,1),\
														line_variation_diff_threshold,GET_BITS(pp->line_variation_diff_threshold,0,8));
				PE_SHP_HW_H15A_RES_WR03(pe1_dj_ctrl_01,	level_th,			GET_BITS(pp->level_th,0,8),\
														protect_th,			GET_BITS(pp->protect_th,0,8),\
														n_avg_gain,			GET_BITS(pp->n_avg_gain,0,8));
				PE_SHP_HW_H15A_RES_WR02(pe1_dj_ctrl_02,	reg_g0_cnt_min,		GET_BITS(pp->reg_g0_cnt_min,0,5),\
														reg_g0_mul,			GET_BITS(pp->reg_g0_mul,0,5));
				PE_SHP_HW_H15A_RES_WR02(pe1_dj_ctrl_03,	reg_g1_protect_min,	GET_BITS(pp->reg_g1_protect_min,0,8),\
														reg_g1_mul, 		GET_BITS(pp->reg_g1_mul,0,7));				
				/*e_map 36 */
				PE_SHP_HW_H15A_RES_WR02(pe1_coring_ctrl_00, reg_cross_th_sc,	GET_BITS(pp->reg_cross_th_sc,0,8),\
														    reg_cross_th,		GET_BITS(pp->reg_cross_th,0,8));
				PE_SHP_HW_H15A_RES_WR02(pe1_coring_ctrl_08, reg_mul_base_t,		GET_BITS(pp->reg_mul_base_t,0,8),\
														    reg_mul_base_e,		GET_BITS(pp->reg_mul_base_e,0,8));
				PE_SHP_HW_H15A_RES_WR02(pe1_coring_ctrl_00, reg_a_map_h_size,	GET_BITS(pp->reg_a_map_h_size,0,8),\
														    reg_a_nomalize_en,	GET_BITS(pp->reg_a_nomalize_en,0,8));
				PE_SHP_HW_H15A_RES_WR02(pe1_coring_ctrl_01, reg_edge_alpha_mul,	GET_BITS(pp->reg_edge_alpha_mul,0,8),\
														    reg_edge_beta_mul,	GET_BITS(pp->reg_edge_beta_mul,0,8));
				PE_SHP_HW_H15A_RES_WR04(pe1_coring_ctrl_02,	reg_a_lut_edge_x0,		GET_BITS(pp->reg_a_lut_edge_x0,0,8),\
															reg_a_lut_edge_y0,		GET_BITS(pp->reg_a_lut_edge_y0,0,8),\
															reg_a_lut_edge_x1,		GET_BITS(pp->reg_a_lut_edge_x1,0,8),\
															reg_a_lut_edge_y1,		GET_BITS(pp->reg_a_lut_edge_y1,0,8));
				PE_SHP_HW_H15A_RES_WR04(pe1_coring_ctrl_03,	reg_a_lut_detail_x0,	GET_BITS(pp->reg_a_lut_detail_x0,0,8),\
															reg_a_lut_detail_y0,	GET_BITS(pp->reg_a_lut_detail_y0,0,8),\
															reg_a_lut_detail_x1,	GET_BITS(pp->reg_a_lut_detail_x1,0,8),\
															reg_a_lut_detail_y1,	GET_BITS(pp->reg_a_lut_detail_y1,0,8));
				PE_SHP_HW_H15A_RES_WR04(pe1_coring_ctrl_06, reg_edge_minmax_x0,		GET_BITS(pp->reg_edge_minmax_x0,0,8),\
															reg_edge_minmax_y0,		GET_BITS(pp->reg_edge_minmax_y0,0,8),\
															reg_edge_minmax_x1,		GET_BITS(pp->reg_edge_minmax_x1,0,8),\
															reg_edge_minmax_y1,		GET_BITS(pp->reg_edge_minmax_y1,0,8));
				PE_SHP_HW_H15A_RES_WR02(pe1_coring_ctrl_00, reg_t_cut_resolution,	GET_BITS(pp->reg_t_cut_resolution,0,2),\
														    reg_t_nomalize_en,		GET_BITS(pp->reg_t_nomalize_en,0,1));
				PE_SHP_HW_H15A_RES_WR02(pe1_coring_ctrl_01, reg_detail_alpha_mul,	GET_BITS(pp->reg_detail_alpha_mul,0,8),\
														    reg_detail_beta_mul,	GET_BITS(pp->reg_detail_beta_mul,0,8));
				PE_SHP_HW_H15A_RES_WR04(pe1_coring_ctrl_04, reg_t_lut_edge_x0,		GET_BITS(pp->reg_t_lut_edge_x0,0,8),\
															reg_t_lut_edge_y0,		GET_BITS(pp->reg_t_lut_edge_y0,0,8),\
															reg_t_lut_edge_x1,		GET_BITS(pp->reg_t_lut_edge_x1,0,8),\
															reg_t_lut_edge_y1,		GET_BITS(pp->reg_t_lut_edge_y1,0,8));
				PE_SHP_HW_H15A_RES_WR04(pe1_coring_ctrl_05, reg_t_lut_detail_x0,	GET_BITS(pp->reg_t_lut_detail_x0,0,8),\
															reg_t_lut_detail_y0,	GET_BITS(pp->reg_t_lut_detail_y0,0,8),\
															reg_t_lut_detail_x1,	GET_BITS(pp->reg_t_lut_detail_x1,0,8),\
															reg_t_lut_detail_y1,	GET_BITS(pp->reg_t_lut_detail_y1,0,8));
				PE_SHP_HW_H15A_RES_WR04(pe1_coring_ctrl_07, reg_detail_minmax_x0,	GET_BITS(pp->reg_detail_minmax_x0,0,8),\
															reg_detail_minmax_y0,	GET_BITS(pp->reg_detail_minmax_y0,0,8),\
															reg_detail_minmax_x1,	GET_BITS(pp->reg_detail_minmax_x1,0,8),\
															reg_detail_minmax_y1,	GET_BITS(pp->reg_detail_minmax_y1,0,8));
				/* blc*/
				PE_SHP_HW_H15A_RES_WR04(pe1_mp_ctrl_07, gb_en,		GET_BITS(pp->mp_gb_en,0,1),\
														gb_mode,	GET_BITS(pp->mp_gb_mode,0,1),\
														gb_x1,		GET_BITS(pp->mp_gb_x1,0,8),\
														gb_y1,		GET_BITS(pp->mp_gb_y1,0,8));
				PE_SHP_HW_H15A_RES_WR03(pe1_mp_ctrl_08 ,gb_x2,		GET_BITS(pp->mp_gb_x2,0,8),\
														gb_y2, 		GET_BITS(pp->mp_gb_y2,0,8),\
														gb_y3,		GET_BITS(pp->mp_gb_y3,0,8));
				PE_SHP_HW_H15A_RES_WR04(pe1_mp_ctrl_09, lum1_x_l0,	GET_BITS(pp->mp_lum1_x_L0,0,8),\
														lum1_x_l1,	GET_BITS(pp->mp_lum1_x_L1,0,8),\
														lum1_x_h0,	GET_BITS(pp->mp_lum1_x_H0,0,8),\
														lum1_x_h1,	GET_BITS(pp->mp_lum1_x_H1,0,8));
				PE_SHP_HW_H15A_RES_WR04(pe1_mp_ctrl_0a, lum1_y0,	GET_BITS(pp->mp_lum1_y0,0,8),\
														lum1_y1,	GET_BITS(pp->mp_lum1_y1,0,8),\
														lum1_y2,	GET_BITS(pp->mp_lum1_y2,0,8),\
														lum2_x_l0,	GET_BITS(pp->mp_lum2_x_L0,0,8));
				PE_SHP_HW_H15A_RES_WR04(pe1_mp_ctrl_0b, lum2_x_l1,	GET_BITS(pp->mp_lum2_x_L1,0,8),\
														lum2_x_h0,	GET_BITS(pp->mp_lum2_x_H0,0,8),\
														lum2_x_h1,	GET_BITS(pp->mp_lum2_x_H1,0,8),\
														lum2_y0,	GET_BITS(pp->mp_lum2_y0,0,8));
				PE_SHP_HW_H15A_RES_WR02(pe1_mp_ctrl_0c, lum2_y1,	GET_BITS(pp->mp_lum2_y1,0,8),\
														lum2_y2,	GET_BITS(pp->mp_lum2_y2,0,8));
				PE_SHP_HW_H15A_RES_WR04(pe1_sp_ctrl_07, gb_en,		GET_BITS(pp->sp_gb_en,0,1),\
														gb_mode,	GET_BITS(pp->sp_gb_mode,0,1),\
														gb_x1,		GET_BITS(pp->sp_gb_x1,0,8),\
														gb_y1,		GET_BITS(pp->sp_gb_y1,0,8));
				PE_SHP_HW_H15A_RES_WR03(pe1_sp_ctrl_08, gb_x2,		GET_BITS(pp->sp_gb_x2,0,8),\
														gb_y2, 		GET_BITS(pp->sp_gb_y2,0,8),\
														gb_y3,		GET_BITS(pp->sp_gb_y3,0,8));
				PE_SHP_HW_H15A_RES_WR04(pe1_sp_ctrl_09, lum1_x_l0,	GET_BITS(pp->sp_lum1_x_L0,0,8),\
														lum1_x_l1,	GET_BITS(pp->sp_lum1_x_L1,0,8),\
														lum1_x_h0,	GET_BITS(pp->sp_lum1_x_H0,0,8),\
														lum1_x_h1,	GET_BITS(pp->sp_lum1_x_H1,0,8));
				PE_SHP_HW_H15A_RES_WR04(pe1_sp_ctrl_0a, lum1_y0,	GET_BITS(pp->sp_lum1_y0,0,8),\
														lum1_y1,	GET_BITS(pp->sp_lum1_y1,0,8),\
														lum1_y2,	GET_BITS(pp->sp_lum1_y2,0,8),\
														lum2_x_l0,	GET_BITS(pp->sp_lum2_x_L0,0,8));
				PE_SHP_HW_H15A_RES_WR04(pe1_sp_ctrl_0b, lum2_x_l1,	GET_BITS(pp->sp_lum2_x_L1,0,8),\
														lum2_x_h0,	GET_BITS(pp->sp_lum2_x_H0,0,8),\
														lum2_x_h1,	GET_BITS(pp->sp_lum2_x_H1,0,8),\
														lum2_y0,	GET_BITS(pp->sp_lum2_y0,0,8));
				PE_SHP_HW_H15A_RES_WR02(pe1_sp_ctrl_0c, lum2_y1,	GET_BITS(pp->sp_lum2_y1,0,8),\
														lum2_y2,	GET_BITS(pp->sp_lum2_y2,0,8));
				/* ti*/
				PE_SHP_HW_H15A_RES_WR03(pe1_pti_ctrl_00, reg_ti_enable,		GET_BITS(pp->reg_ti_enable_pti_v,0,1),\
														 reg_mm_tap_size, 	GET_BITS(pp->reg_mm_tap_size_pti_v,0,2),\
														 reg_avg_tap_size,	GET_BITS(pp->reg_avg_tap_size_pti_v,0,3));
				PE_SHP_HW_H15A_RES_WR03(pe1_pti_ctrl_01, reg_ti_enable,		GET_BITS(pp->reg_ti_enable_pti_h,0,1),\
														 reg_mm_tap_size, 	GET_BITS(pp->reg_mm_tap_size_pti_h,0,2),\
														 reg_avg_tap_size,	GET_BITS(pp->reg_avg_tap_size_pti_h,0,3));
				PE_SHP_HW_H15A_RES_WR03(pe1_sti_ctrl_00, reg_ti_enable,		GET_BITS(pp->reg_enable_sti,0,1),\
														 reg_mm_tap_size, 	GET_BITS(pp->reg_mm_tap_size_sti,0,2),\
														 reg_avg_tap_size,	GET_BITS(pp->reg_avg_tap_size_sti,0,3));
				/* cti*/
				PE_SHP_HW_H15A_RES_WR01(pe1_cti_ctrl_00,cti_en,				GET_BITS(pp->cti_en,0,1));
				PE_SHP_HW_H15A_RES_WR04(pe1_cti_ctrl_01,coring_th0,			GET_BITS(pp->coring_th0,0,8),\
														coring_th1,			GET_BITS(pp->coring_th1,0,8),\
														coring_map_filter, 	GET_BITS(pp->coring_map_filter,0,3),\
														coring_tap_size, 	GET_BITS(pp->coring_tap_size,0,3));
				PE_SHP_HW_H15A_RES_WR03(pe1_cti_ctrl_02,ycm_en1,			GET_BITS(pp->ycm_en1,0,1),\
														ycm_band_sel,		GET_BITS(pp->ycm_band_sel,0,3),\
														ycm_diff_th,		GET_BITS(pp->ycm_diff_th,0,8));

				/* tgn*/
				PE_SHP_HW_H15A_RES_WR01(pe1_tgen_ctrl_00, tgen_on_off,		GET_BITS(pp->tgen_on_off,0,1));
				PE_SHP_HW_H15A_RES_WR04(pe1_tgen_ctrl_01, tg_delta_h_bound,	GET_BITS(pp->tg_delta_H_bound,0,5),\
														  tg_delta_l_bound,	GET_BITS(pp->tg_delta_L_bound,0,4),\
														  tg_delta_max,		GET_BITS(pp->tg_delta_max,0,7),\
														  tg_delta_scale,	GET_BITS(pp->tg_delta_scale,0,3));
				PE_SHP_HW_H15A_RES_WR03(pe1_tgen_ctrl_02, tg_rnd_th,		GET_BITS(pp->tg_rnd_th,0,8),\
														  tg_delta_wcurr, 	GET_BITS(pp->tg_delta_wcurr,0,4),\
														  tg_lc_ldr_th,		GET_BITS(pp->tg_lc_ldr_th,0,5));
				PE_SHP_HW_H15A_RES_WR04(pe1_tgen_ctrl_04, tg_dr_ratio,		GET_BITS(pp->tg_dr_ratio,0,3),\
														  tg_dr_th, 		GET_BITS(pp->tg_dr_th,0,6),\
														  tg_mm_offset,		GET_BITS(pp->tg_mm_offset,0,5),\
														  tg_mm_sel_mode,	GET_BITS(pp->tg_mm_sel_mode,0,2));
				PE_SHP_HW_H15A_RES_WR03(pe1_tgen_ctrl_05, tg_lavg_th,		GET_BITS(pp->tg_lavg_th,0,10),\
														  tg_gs_rnd,	 	GET_BITS(pp->tg_gs_rnd,0,5),\
														  tg_gs_mm,			GET_BITS(pp->tg_gs_mm,0,5));
				PE_SHP_HW_H15A_RES_WR04(pe1_tgen_ctrl_09, tg_gain0,		GET_BITS(pp->tg_gain0,0,4),\
														  tg_gain1,		GET_BITS(pp->tg_gain1,0,4),\
														  tg_gain2,		GET_BITS(pp->tg_gain2,0,4),\
														  tg_gain3,		GET_BITS(pp->tg_gain3,0,4));
				PE_SHP_HW_H15A_RES_WR04(pe1_tgen_ctrl_09, tg_gain4,		GET_BITS(pp->tg_gain4,0,4),\
														  tg_gain5,		GET_BITS(pp->tg_gain5,0,4),\
														  tg_gain6,		GET_BITS(pp->tg_gain6,0,4),\
														  tg_gain7,		GET_BITS(pp->tg_gain7,0,4));
			}
			#endif
		#else
			LX_PE_SHP_RE3_MISC_T *pp=(LX_PE_SHP_RE3_MISC_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			PE_SHP_HW_H15_DBG_PRINT("set[%d] :  resolution misc ctrl\n"\
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
				PE_FWI_H15_MISC_CTRL fw_param;
				fw_param.gt_th1 = GET_BITS(pp->gt_th1,0,8);
				fw_param.gt_th0 = GET_BITS(pp->gt_th0,0,8);
				fw_param.gt_th0a = GET_BITS(pp->gt_th0a,0,8);
				fw_param.gt_th0b = GET_BITS(pp->gt_th0b,0,8);
				ret = PE_FWI_H15_SetShpMiscCtrl(&fw_param);
				PE_SHP_HW_H15_CHECK_CODE(ret, break, \
					"[%s,%d] PE_FWI_H15_SetShpMiscCtrl() error.\n", __F__, __L__);
				#ifdef PE_HW_H15_PROTECT_OVERLAP_SETTINGS
				/* h 5 */
				PE_SHP_HW_H15A_RES_WR01(pe1_derh_ctrl_06,edge_y_filter_en,	GET_BITS(pp->edge_y_filter_en,0,1));
				PE_SHP_HW_H15A_RES_WR02(pe1_derh_ctrl_0c,e_gain_max,		GET_BITS(pp->e_gain_max,0,6),\
														f_gain_max,			GET_BITS(pp->f_gain_max,0,6));
				PE_SHP_HW_H15A_RES_WR01(pe1_mp_ctrl_06,	lap_h_mode,			GET_BITS(pp->mp_lap_h_mode,0,3));
				#ifdef PE_SHP_HW_H15_PROTECT_SP_LAP_H_MODE_BUG
				pp->sp_lap_h_mode = (pp->sp_lap_h_mode==0x1 || pp->sp_lap_h_mode==0x2)? 0x0:\
									 (pp->sp_lap_h_mode==0x4)? 0x3:0x5;
				#endif
				PE_SHP_HW_H15A_RES_WR01(pe1_sp_ctrl_06,	lap_h_mode,			GET_BITS(pp->sp_lap_h_mode,0,3));
				/* v 5 */
				PE_SHP_HW_H15A_RES_WR02(pe1_derv_ctrl_0,der_v_en,			GET_BITS(pp->der_v_en,0,1),\
														der_gain_mapping,	GET_BITS(pp->der_gain_mapping,0,2));
				PE_SHP_HW_H15A_RES_WR01(pe1_derv_ctrl_2,max_sel,			GET_BITS(pp->max_sel,0,3));
				PE_SHP_HW_H15A_RES_WR01(pe1_mp_ctrl_06,	lap_v_mode,			GET_BITS(pp->mp_lap_v_mode,0,3));
				PE_SHP_HW_H15A_RES_WR01(pe1_sp_ctrl_06,	lap_v_mode,			GET_BITS(pp->sp_lap_v_mode,0,3));
				/* cmm 6 */
				PE_SHP_HW_H15A_RES_WR02(pe1_mp_ctrl_02,	sobel_weight,		GET_BITS(pp->mp_sobel_weight,0,8),\
														laplacian_weight,	GET_BITS(pp->mp_laplacian_weight,0,8));
				PE_SHP_HW_H15A_RES_WR02(pe1_sp_ctrl_02,	sobel_weight,		GET_BITS(pp->sp_sobel_weight,0,8),\
														laplacian_weight,	GET_BITS(pp->sp_laplacian_weight,0,8));
				PE_SHP_HW_H15A_RES_WR02(pe1_derh_ctrl_07,	flat_filter_en,		GET_BITS(pp->flat_en,0,1),\
														flat_filter_type,	GET_BITS(pp->flat_filter_type,0,1));
				/*d_jag 12 */
				PE_SHP_HW_H15A_RES_WR05(pe1_dj_ctrl_00,	edf_en,				GET_BITS(pp->edf_en,0,1),\
														center_blur_mode, 	GET_BITS(pp->center_blur_mode,0,1),\
														count_diff_th, 		GET_BITS(pp->count_diff_th,0,5),\
														n_avg_mode,			GET_BITS(pp->n_avg_mode,0,1),\
														line_variation_diff_threshold,GET_BITS(pp->line_variation_diff_th,0,8));
				PE_SHP_HW_H15A_RES_WR03(pe1_dj_ctrl_01,	level_th,			GET_BITS(pp->level_th,0,8),\
														protect_th,			GET_BITS(pp->protect_th,0,8),\
														n_avg_gain,			GET_BITS(pp->n_avg_gain,0,8));
				PE_SHP_HW_H15A_RES_WR02(pe1_dj_ctrl_02,	reg_g0_cnt_min,		GET_BITS(pp->reg_g0_cnt_min,0,5),\
														reg_g0_mul,			GET_BITS(pp->reg_g0_mul,0,5));
				PE_SHP_HW_H15A_RES_WR02(pe1_dj_ctrl_03,	reg_g1_protect_min,	GET_BITS(pp->reg_g1_protect_min,0,8),\
														reg_g1_mul, 		GET_BITS(pp->reg_g1_mul,0,7));				
				/* cti 8 */
				PE_SHP_HW_H15A_RES_WR01(pe1_cti_ctrl_00,cti_en,				GET_BITS(pp->cti_en,0,1));
				PE_SHP_HW_H15A_RES_WR04(pe1_cti_ctrl_01,coring_th0,			GET_BITS(pp->coring_th0,0,8),\
														coring_th1,			GET_BITS(pp->coring_th1,0,8),\
														coring_map_filter, 	GET_BITS(pp->coring_map_filter,0,3),\
														coring_tap_size, 	GET_BITS(pp->coring_tap_size,0,3));
				PE_SHP_HW_H15A_RES_WR03(pe1_cti_ctrl_02,ycm_en1,			GET_BITS(pp->ycm_en1,0,1),\
														ycm_band_sel,		GET_BITS(pp->ycm_band_sel,0,3),\
														ycm_diff_th,		GET_BITS(pp->ycm_diff_th,0,8));
				#else
				/* h 5 */
				PE_SHPLR_H15A0_QWr01(pe1_derh_ctrl_06,	edge_y_filter_en,	GET_BITS(pp->edge_y_filter_en,0,1));
				PE_SHPLR_H15A0_QWr02(pe1_derh_ctrl_0c,	e_gain_max,			GET_BITS(pp->e_gain_max,0,6),\
														f_gain_max,			GET_BITS(pp->f_gain_max,0,6));
				PE_SHPLR_H15A0_QWr01(pe1_mp_ctrl_06,	lap_h_mode,			GET_BITS(pp->mp_lap_h_mode,0,3));
				PE_SHPLR_H15A0_QWr01(pe1_sp_ctrl_06,	lap_h_mode,			GET_BITS(pp->sp_lap_h_mode,0,3));
				/* v 5 */
				PE_SHPLR_H15A0_QWr02(pe1_derv_ctrl_0,	der_v_en,			GET_BITS(pp->der_v_en,0,1),\
														der_gain_mapping,	GET_BITS(pp->der_gain_mapping,0,2));
				PE_SHPLR_H15A0_QWr01(pe1_derv_ctrl_2,	max_sel,			GET_BITS(pp->max_sel,0,3));
				PE_SHPLR_H15A0_QWr01(pe1_mp_ctrl_06,	lap_v_mode,			GET_BITS(pp->mp_lap_v_mode,0,3));
				PE_SHPLR_H15A0_QWr01(pe1_sp_ctrl_06,	lap_v_mode,			GET_BITS(pp->sp_lap_v_mode,0,3));
				/* cmm 6 */
				PE_SHPLR_H15A0_QWr02(pe1_mp_ctrl_02,	sobel_weight,		GET_BITS(pp->mp_sobel_weight,0,8),\
														laplacian_weight,	GET_BITS(pp->mp_laplacian_weight,0,8));
				PE_SHPLR_H15A0_QWr02(pe1_sp_ctrl_02,	sobel_weight,		GET_BITS(pp->sp_sobel_weight,0,8),\
														laplacian_weight,	GET_BITS(pp->sp_laplacian_weight,0,8));
				PE_SHPLR_H15A0_QWr02(pe1_derh_ctrl_07,	flat_filter_en,		GET_BITS(pp->flat_en,0,1),\
														flat_filter_type,	GET_BITS(pp->flat_filter_type,0,1));
				/*d_jag 12 */
				PE_SHPLR_H15A0_QWr05(pe1_dj_ctrl_00,	edf_en,				GET_BITS(pp->edf_en,0,1),\
														center_blur_mode, 	GET_BITS(pp->center_blur_mode,0,1),\
														count_diff_th, 		GET_BITS(pp->count_diff_th,0,5),\
														n_avg_mode,			GET_BITS(pp->n_avg_mode,0,1),\
														line_variation_diff_threshold,GET_BITS(pp->line_variation_diff_th,0,8));
				PE_SHPLR_H15A0_QWr03(pe1_dj_ctrl_01,	level_th,			GET_BITS(pp->level_th,0,8),\
														protect_th,			GET_BITS(pp->protect_th,0,8),\
														n_avg_gain,			GET_BITS(pp->n_avg_gain,0,8));
				PE_SHPLR_H15A0_QWr02(pe1_dj_ctrl_02,	reg_g0_cnt_min,		GET_BITS(pp->reg_g0_cnt_min,0,5),\
														reg_g0_mul,			GET_BITS(pp->reg_g0_mul,0,5));
				PE_SHPLR_H15A0_QWr02(pe1_dj_ctrl_03,	reg_g1_protect_min,	GET_BITS(pp->reg_g1_protect_min,0,8),\
														reg_g1_mul, 		GET_BITS(pp->reg_g1_mul,0,7));				
				/* cti 8 */
				PE_SHPLR_H15A0_QWr01(pe1_cti_ctrl_00,	cti_en,				GET_BITS(pp->cti_en,0,1));
				PE_SHPLR_H15A0_QWr04(pe1_cti_ctrl_01,	coring_th0,			GET_BITS(pp->coring_th0,0,8),\
														coring_th1,			GET_BITS(pp->coring_th1,0,8),\
														coring_map_filter, 	GET_BITS(pp->coring_map_filter,0,3),\
														coring_tap_size, 	GET_BITS(pp->coring_tap_size,0,3));
				PE_SHPLR_H15A0_QWr03(pe1_cti_ctrl_02,	ycm_en1,			GET_BITS(pp->ycm_en1,0,1),\
														ycm_band_sel,		GET_BITS(pp->ycm_band_sel,0,3),\
														ycm_diff_th,		GET_BITS(pp->ycm_diff_th,0,8));
				#endif
			}	
			#endif
		}
		else
		{
			PE_SHP_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
		
	} while(0);
	
	return ret;
}
/**
 * get resolution misc control
 * - use input struct LX_PE_SHP_RE2_MISC_T for H15AX
 * - use input struct LX_PE_SHP_RE1_MISC_T for H15AX
 *
 * @param   *pstParams [in/out] void
 * @return  OK if success, ERROR otherwise.
 * @see		function description
 * @author
 */
int PE_SHP_HW_H15_GetReMiscCtrl(void *pstParams)
{
	int ret = RET_OK;
	LX_PE_WIN_ID win_id;
	do {
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H15)
		{
			#ifdef PE_SHP_PARAM_CHANGE_FOR_H15
			LX_PE_SHP_RE4_MISC_T *pp=(LX_PE_SHP_RE4_MISC_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			win_id = PE_GET_CHECKED_WINID(pp->win_id);
			if(PE_CHECK_WIN0(win_id))
			{
				#if 0
				PE_FWI_H15_SETTINGS_T fw_inf;
				ret = PE_FWI_H15_GetCurFwiSettings(&fw_inf);
				PE_SHP_HW_H15_CHECK_CODE(ret, break, \
					"[%s,%d] PE_FWI_H15_GetCurFwiSettings() error.\n", __F__, __L__);
				PE_SHP_HW_H15_DBG_PRINT("fw_ctrl.enable : %s.\n", \
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
					#ifdef PE_HW_H15_FOR_BRINGUP
					#else
					PE_SHPL_H15A0_RdFL(pe1_coring_ctrl_05);
					PE_SHPL_H15A0_RdFL(pe1_coring_ctrl_06);
					PE_SHPL_H15A0_Rd01(pe1_coring_ctrl_05,	gt_th1,		pp->gt_th1);
					PE_SHPL_H15A0_Rd01(pe1_coring_ctrl_05,	gt_th0,		pp->gt_th0);
					PE_SHPL_H15A0_Rd01(pe1_coring_ctrl_06,	gt_th0a,	pp->gt_th0a);
					PE_SHPL_H15A0_Rd01(pe1_coring_ctrl_06,	gt_th0b,	pp->gt_th0b);
					#endif
				}
				#endif
				PE_SHPL_H15A0_QRd01(pe1_derh_ctrl_06,edge_y_filter_en,	pp->edge_Y_filter_en);
				PE_SHPL_H15A0_QRd02(pe1_derh_ctrl_0c,e_gain_max,		pp->e_gain_max,\
													f_gain_max,			pp->f_gain_max);
				/* v 5 */
				PE_SHPL_H15A0_QRd02(pe1_derv_ctrl_0,der_v_en,			pp->der_v_en,\
													der_gain_mapping,	pp->der_gain_mapping);
				PE_SHPL_H15A0_QRd01(pe1_derv_ctrl_2,max_sel,			pp->max_sel);
				/* cmm 5 */
				PE_SHPL_H15A0_QRd01(pe1_mp_ctrl_02,	laplacian_weight,		pp->mp_laplacian_weight);
				PE_SHPL_H15A0_QRd01(pe1_sp_ctrl_02,	laplacian_weight,		pp->sp_laplacian_weight);
				PE_SHPL_H15A0_QRd03(pe1_derh_ctrl_07,	flat_filter_en,		pp->flat_filter_en,\
														flat_filter_gain,	pp->flat_filter_gain,\
														flat_filter_type,	pp->flat_filter_type);
				/*d_jag 12 */
				PE_SHPL_H15A0_QRd05(pe1_dj_ctrl_00,	edf_en,				pp->edf_en,\
													center_blur_mode, 	pp->center_blur_mode,\
													count_diff_th, 		pp->count_diff_th,\
													n_avg_mode,			pp->n_avg_mode,\
													line_variation_diff_threshold,pp->line_variation_diff_threshold);
				PE_SHPL_H15A0_QRd03(pe1_dj_ctrl_01,	level_th,			pp->level_th,\
													protect_th,			pp->protect_th,\
													n_avg_gain,			pp->n_avg_gain);
				PE_SHPL_H15A0_QRd02(pe1_dj_ctrl_02,	reg_g0_cnt_min,		pp->reg_g0_cnt_min,\
													reg_g0_mul,			pp->reg_g0_mul);
				PE_SHPL_H15A0_QRd02(pe1_dj_ctrl_03,	reg_g1_protect_min,	pp->reg_g1_protect_min,\
													reg_g1_mul, 		pp->reg_g1_mul);				
				/*e_map 36 */
				PE_SHPL_H15A0_QRd02(pe1_coring_ctrl_00, reg_cross_th_sc,	pp->reg_cross_th_sc,\
														reg_cross_th,		pp->reg_cross_th);
				PE_SHPL_H15A0_QRd02(pe1_coring_ctrl_08, reg_mul_base_t,		pp->reg_mul_base_t,\
														reg_mul_base_e,		pp->reg_mul_base_e);
				PE_SHPL_H15A0_QRd02(pe1_coring_ctrl_00, reg_a_map_h_size,	pp->reg_a_map_h_size,\
														reg_a_nomalize_en,	pp->reg_a_nomalize_en);
				PE_SHPL_H15A0_QRd02(pe1_coring_ctrl_01, reg_edge_alpha_mul,	pp->reg_edge_alpha_mul,\
														reg_edge_beta_mul,	pp->reg_edge_beta_mul);
				PE_SHPL_H15A0_QRd04(pe1_coring_ctrl_02,	reg_a_lut_edge_x0,	pp->reg_a_lut_edge_x0,\
														reg_a_lut_edge_y0,	pp->reg_a_lut_edge_y0,\
														reg_a_lut_edge_x1,	pp->reg_a_lut_edge_x1,\
														reg_a_lut_edge_y1,	pp->reg_a_lut_edge_y1);
				PE_SHPL_H15A0_QRd04(pe1_coring_ctrl_03,	reg_a_lut_detail_x0,pp->reg_a_lut_detail_x0,\
														reg_a_lut_detail_y0,pp->reg_a_lut_detail_y0,\
														reg_a_lut_detail_x1,pp->reg_a_lut_detail_x1,\
														reg_a_lut_detail_y1,pp->reg_a_lut_detail_y1);
				PE_SHPL_H15A0_QRd04(pe1_coring_ctrl_06, reg_edge_minmax_x0,		pp->reg_edge_minmax_x0,\
															reg_edge_minmax_y0,	pp->reg_edge_minmax_y0,\
															reg_edge_minmax_x1,	pp->reg_edge_minmax_x1,\
															reg_edge_minmax_y1,	pp->reg_edge_minmax_y1);
				PE_SHPL_H15A0_QRd02(pe1_coring_ctrl_00, reg_t_cut_resolution,	pp->reg_t_cut_resolution,\
														   reg_t_nomalize_en,	pp->reg_t_nomalize_en);
				PE_SHPL_H15A0_QRd02(pe1_coring_ctrl_01, reg_detail_alpha_mul,	pp->reg_detail_alpha_mul,\
														   reg_detail_beta_mul, pp->reg_detail_beta_mul);
				PE_SHPL_H15A0_QRd04(pe1_coring_ctrl_04, reg_t_lut_edge_x0,		pp->reg_t_lut_edge_x0,\
														reg_t_lut_edge_y0,	pp->reg_t_lut_edge_y0,\
														reg_t_lut_edge_x1,	pp->reg_t_lut_edge_x1,\
														reg_t_lut_edge_y1,	pp->reg_t_lut_edge_y1);
				PE_SHPL_H15A0_QRd04(pe1_coring_ctrl_05, reg_t_lut_detail_x0,pp->reg_t_lut_detail_x0,\
														reg_t_lut_detail_y0,pp->reg_t_lut_detail_y0,\
														reg_t_lut_detail_x1,pp->reg_t_lut_detail_x1,\
														reg_t_lut_detail_y1,pp->reg_t_lut_detail_y1);
				PE_SHPL_H15A0_QRd04(pe1_coring_ctrl_07, reg_detail_minmax_x0,	pp->reg_detail_minmax_x0,\
														reg_detail_minmax_y0,	pp->reg_detail_minmax_y0,\
														reg_detail_minmax_x1,	pp->reg_detail_minmax_x1,\
														reg_detail_minmax_y1,	pp->reg_detail_minmax_y1);
				/* blc*/
				PE_SHPL_H15A0_QRd04(pe1_mp_ctrl_07, gb_en,	pp->mp_gb_en,\
													gb_mode,pp->mp_gb_mode,\
													gb_x1,	pp->mp_gb_x1,\
													gb_y1,	pp->mp_gb_y1);
				PE_SHPL_H15A0_QRd03(pe1_mp_ctrl_08 ,gb_x2,	pp->mp_gb_x2,\
													gb_y2, 	pp->mp_gb_y2,\
													gb_y3,	pp->mp_gb_y3);
				PE_SHPL_H15A0_QRd04(pe1_mp_ctrl_09, lum1_x_l0,	pp->mp_lum1_x_L0,\
													lum1_x_l1,	pp->mp_lum1_x_L1,\
													lum1_x_h0,	pp->mp_lum1_x_H0,\
													lum1_x_h1,	pp->mp_lum1_x_H1);
				PE_SHPL_H15A0_QRd04(pe1_mp_ctrl_0a, lum1_y0,	pp->mp_lum1_y0,\
													lum1_y1,	pp->mp_lum1_y1,\
													lum1_y2,	pp->mp_lum1_y2,\
													lum2_x_l0,	pp->mp_lum2_x_L0);
				PE_SHPL_H15A0_QRd04(pe1_mp_ctrl_0b, lum2_x_l1,	pp->mp_lum2_x_L1,\
													lum2_x_h0,	pp->mp_lum2_x_H0,\
													lum2_x_h1,	pp->mp_lum2_x_H1,\
													lum2_y0,	pp->mp_lum2_y0);
				PE_SHPL_H15A0_QRd02(pe1_mp_ctrl_0c, lum2_y1,	pp->mp_lum2_y1,\
													lum2_y2,	pp->mp_lum2_y2);
				PE_SHPL_H15A0_QRd04(pe1_sp_ctrl_07, gb_en,		pp->sp_gb_en,\
													gb_mode,	pp->sp_gb_mode,\
													gb_x1,		pp->sp_gb_x1,\
													gb_y1,		pp->sp_gb_y1);
				PE_SHPL_H15A0_QRd03(pe1_sp_ctrl_08, gb_x2,		pp->sp_gb_x2,\
													gb_y2, 		pp->sp_gb_y2,\
													gb_y3,		pp->sp_gb_y3);
				PE_SHPL_H15A0_QRd04(pe1_sp_ctrl_09, lum1_x_l0,	pp->sp_lum1_x_L0,\
													lum1_x_l1,	pp->sp_lum1_x_L1,\
													lum1_x_h0,	pp->sp_lum1_x_H0,\
													lum1_x_h1,	pp->sp_lum1_x_H1);
				PE_SHPL_H15A0_QRd04(pe1_sp_ctrl_0a, lum1_y0,	pp->sp_lum1_y0,\
													lum1_y1,	pp->sp_lum1_y1,\
													lum1_y2,	pp->sp_lum1_y2,\
													lum2_x_l0,	pp->sp_lum2_x_L0);
				PE_SHPL_H15A0_QRd04(pe1_sp_ctrl_0b, lum2_x_l1,	pp->sp_lum2_x_L1,\
													lum2_x_h0,	pp->sp_lum2_x_H0,\
													lum2_x_h1,	pp->sp_lum2_x_H1,\
													lum2_y0,	pp->sp_lum2_y0);
				PE_SHPL_H15A0_QRd02(pe1_sp_ctrl_0c, lum2_y1,	pp->sp_lum2_y1,\
													lum2_y2,	pp->sp_lum2_y2);
				/* ti*/
				PE_SHPL_H15A0_QRd03(pe1_pti_ctrl_00, reg_ti_enable,		pp->reg_ti_enable_pti_v,\
													 reg_mm_tap_size, 	pp->reg_mm_tap_size_pti_v,\
													 reg_avg_tap_size,	pp->reg_avg_tap_size_pti_v);
				PE_SHPL_H15A0_QRd03(pe1_pti_ctrl_01, reg_ti_enable,		pp->reg_ti_enable_pti_h,\
													 reg_mm_tap_size, 	pp->reg_mm_tap_size_pti_h,\
													 reg_avg_tap_size,	pp->reg_avg_tap_size_pti_h);
				PE_SHPL_H15A0_QRd03(pe1_sti_ctrl_00, reg_ti_enable,		pp->reg_enable_sti,\
													 reg_mm_tap_size, 	pp->reg_mm_tap_size_sti,\
													 reg_avg_tap_size,	pp->reg_avg_tap_size_sti);
				/* cti*/
				PE_SHPL_H15A0_QRd01(pe1_cti_ctrl_00,cti_en,				pp->cti_en);
				PE_SHPL_H15A0_QRd04(pe1_cti_ctrl_01,coring_th0,			pp->coring_th0,\
													coring_th1,			pp->coring_th1,\
													coring_map_filter, 	pp->coring_map_filter,\
													coring_tap_size, 	pp->coring_tap_size);
				PE_SHPL_H15A0_QRd03(pe1_cti_ctrl_02,ycm_en1,			pp->ycm_en1,\
													ycm_band_sel,		pp->ycm_band_sel,\
													ycm_diff_th,		pp->ycm_diff_th);

				/* tgn*/
				PE_SHPL_H15A0_QRd01(pe1_tgen_ctrl_00, tgen_on_off,		pp->tgen_on_off);
				PE_SHPL_H15A0_QRd04(pe1_tgen_ctrl_01, tg_delta_h_bound,	pp->tg_delta_H_bound,\
													  tg_delta_l_bound,	pp->tg_delta_L_bound,\
													  tg_delta_max,		pp->tg_delta_max,\
													  tg_delta_scale,	pp->tg_delta_scale);
				PE_SHPL_H15A0_QRd03(pe1_tgen_ctrl_02, tg_rnd_th,		pp->tg_rnd_th,\
													  tg_delta_wcurr, 	pp->tg_delta_wcurr,\
													  tg_lc_ldr_th,		pp->tg_lc_ldr_th);
				PE_SHPL_H15A0_QRd04(pe1_tgen_ctrl_04, tg_dr_ratio,		pp->tg_dr_ratio,\
													  tg_dr_th, 		pp->tg_dr_th,\
													  tg_mm_offset,		pp->tg_mm_offset,\
													  tg_mm_sel_mode,	pp->tg_mm_sel_mode);
				PE_SHPL_H15A0_QRd03(pe1_tgen_ctrl_05, tg_lavg_th,		pp->tg_lavg_th,\
													  tg_gs_rnd,	 	pp->tg_gs_rnd,\
													  tg_gs_mm,			pp->tg_gs_mm);
				PE_SHPL_H15A0_QRd04(pe1_tgen_ctrl_09, tg_gain0,			pp->tg_gain0,\
													  tg_gain1,		pp->tg_gain1,\
													  tg_gain2,		pp->tg_gain2,\
													  tg_gain3,		pp->tg_gain3);
				PE_SHPL_H15A0_QRd04(pe1_tgen_ctrl_09, tg_gain4,		pp->tg_gain4,\
													  tg_gain5,		pp->tg_gain5,\
													  tg_gain6,		pp->tg_gain6,\
													  tg_gain7,		pp->tg_gain7);
			}
			if(PE_CHECK_WIN1(win_id))
			{
				#if 0
				PE_FWI_H15_SETTINGS_T fw_inf;
				ret = PE_FWI_H15_GetCurFwiSettings(&fw_inf);
				PE_SHP_HW_H15_CHECK_CODE(ret, break, \
					"[%s,%d] PE_FWI_H15_GetCurFwiSettings() error.\n", __F__, __L__);
				PE_SHP_HW_H15_DBG_PRINT("fw_ctrl.enable : %s.\n", \
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
					#ifdef PE_HW_H15_FOR_BRINGUP
					#else
					PE_SHPR_H15A0_RdFL(pe1_coring_ctrl_05);
					PE_SHPR_H15A0_RdFL(pe1_coring_ctrl_06);
					PE_SHPR_H15A0_Rd01(pe1_coring_ctrl_05,	gt_th1,		pp->gt_th1);
					PE_SHPR_H15A0_Rd01(pe1_coring_ctrl_05,	gt_th0,		pp->gt_th0);
					PE_SHPR_H15A0_Rd01(pe1_coring_ctrl_06,	gt_th0a,	pp->gt_th0a);
					PE_SHPR_H15A0_Rd01(pe1_coring_ctrl_06,	gt_th0b,	pp->gt_th0b);
					#endif
				}
				#endif
				/* h 5 */
				PE_SHPR_H15A0_QRd01(pe1_derh_ctrl_06,edge_y_filter_en,	pp->edge_Y_filter_en);
				PE_SHPR_H15A0_QRd02(pe1_derh_ctrl_0c,e_gain_max,		pp->e_gain_max,\
													f_gain_max,			pp->f_gain_max);
				/* v 5 */
				PE_SHPR_H15A0_QRd02(pe1_derv_ctrl_0,der_v_en,			pp->der_v_en,\
													der_gain_mapping,	pp->der_gain_mapping);
				PE_SHPR_H15A0_QRd01(pe1_derv_ctrl_2,max_sel,			pp->max_sel);
				/* cmm 5 */
				PE_SHPR_H15A0_QRd01(pe1_mp_ctrl_02,	laplacian_weight,		pp->mp_laplacian_weight);
				PE_SHPR_H15A0_QRd01(pe1_sp_ctrl_02,	laplacian_weight,		pp->sp_laplacian_weight);
				PE_SHPR_H15A0_QRd03(pe1_derh_ctrl_07,	flat_filter_en,		pp->flat_filter_en,\
														flat_filter_gain,	pp->flat_filter_gain,\
														flat_filter_type,	pp->flat_filter_type);
				/*d_jag 12 */
				PE_SHPR_H15A0_QRd05(pe1_dj_ctrl_00,	edf_en,				pp->edf_en,\
													center_blur_mode, 	pp->center_blur_mode,\
													count_diff_th, 		pp->count_diff_th,\
													n_avg_mode,			pp->n_avg_mode,\
													line_variation_diff_threshold,pp->line_variation_diff_threshold);
				PE_SHPR_H15A0_QRd03(pe1_dj_ctrl_01,	level_th,			pp->level_th,\
													protect_th,			pp->protect_th,\
													n_avg_gain,			pp->n_avg_gain);
				PE_SHPR_H15A0_QRd02(pe1_dj_ctrl_02,	reg_g0_cnt_min,		pp->reg_g0_cnt_min,\
													reg_g0_mul,			pp->reg_g0_mul);
				PE_SHPR_H15A0_QRd02(pe1_dj_ctrl_03,	reg_g1_protect_min,	pp->reg_g1_protect_min,\
													reg_g1_mul, 		pp->reg_g1_mul);				
				/*e_map 36 */
				PE_SHPR_H15A0_QRd02(pe1_coring_ctrl_00, reg_cross_th_sc,	pp->reg_cross_th_sc,\
														reg_cross_th,		pp->reg_cross_th);
				PE_SHPR_H15A0_QRd02(pe1_coring_ctrl_08, reg_mul_base_t,		pp->reg_cross_th_sc,\
														reg_mul_base_e,		pp->reg_cross_th);
				PE_SHPR_H15A0_QRd02(pe1_coring_ctrl_00, reg_a_map_h_size,	pp->reg_a_map_h_size,\
														reg_a_nomalize_en,	pp->reg_a_nomalize_en);
				PE_SHPR_H15A0_QRd02(pe1_coring_ctrl_01, reg_edge_alpha_mul,	pp->reg_edge_alpha_mul,\
														reg_edge_beta_mul,	pp->reg_edge_beta_mul);
				PE_SHPR_H15A0_QRd04(pe1_coring_ctrl_02,	reg_a_lut_edge_x0,	pp->reg_a_lut_edge_x0,\
														reg_a_lut_edge_y0,	pp->reg_a_lut_edge_y0,\
														reg_a_lut_edge_x1,	pp->reg_a_lut_edge_x1,\
														reg_a_lut_edge_y1,	pp->reg_a_lut_edge_y1);
				PE_SHPR_H15A0_QRd04(pe1_coring_ctrl_03,	reg_a_lut_detail_x0,pp->reg_a_lut_detail_x0,\
														reg_a_lut_detail_y0,pp->reg_a_lut_detail_y0,\
														reg_a_lut_detail_x1,pp->reg_a_lut_detail_x1,\
														reg_a_lut_detail_y1,pp->reg_a_lut_detail_y1);
				PE_SHPR_H15A0_QRd04(pe1_coring_ctrl_06, reg_edge_minmax_x0,		pp->reg_edge_minmax_x0,\
															reg_edge_minmax_y0,	pp->reg_edge_minmax_y0,\
															reg_edge_minmax_x1,	pp->reg_edge_minmax_x1,\
															reg_edge_minmax_y1,	pp->reg_edge_minmax_y1);
				PE_SHPR_H15A0_QRd02(pe1_coring_ctrl_00, reg_t_cut_resolution,	pp->reg_t_cut_resolution,\
														   reg_t_nomalize_en,	pp->reg_t_nomalize_en);
				PE_SHPR_H15A0_QRd02(pe1_coring_ctrl_01, reg_detail_alpha_mul,	pp->reg_detail_alpha_mul,\
														   reg_detail_beta_mul, pp->reg_detail_beta_mul);
				PE_SHPR_H15A0_QRd04(pe1_coring_ctrl_04, reg_t_lut_edge_x0,		pp->reg_t_lut_edge_x0,\
														reg_t_lut_edge_y0,	pp->reg_t_lut_edge_y0,\
														reg_t_lut_edge_x1,	pp->reg_t_lut_edge_x1,\
														reg_t_lut_edge_y1,	pp->reg_t_lut_edge_y1);
				PE_SHPR_H15A0_QRd04(pe1_coring_ctrl_05, reg_t_lut_detail_x0,pp->reg_t_lut_detail_x0,\
														reg_t_lut_detail_y0,pp->reg_t_lut_detail_y0,\
														reg_t_lut_detail_x1,pp->reg_t_lut_detail_x1,\
														reg_t_lut_detail_y1,pp->reg_t_lut_detail_y1);
				PE_SHPR_H15A0_QRd04(pe1_coring_ctrl_07, reg_detail_minmax_x0,	pp->reg_detail_minmax_x0,\
														reg_detail_minmax_y0,	pp->reg_detail_minmax_y0,\
														reg_detail_minmax_x1,	pp->reg_detail_minmax_x1,\
														reg_detail_minmax_y1,	pp->reg_detail_minmax_y1);
				/* blc*/
				PE_SHPR_H15A0_QRd04(pe1_mp_ctrl_07, gb_en,	pp->mp_gb_en,\
													gb_mode,pp->mp_gb_mode,\
													gb_x1,	pp->mp_gb_x1,\
													gb_y1,	pp->mp_gb_y1);
				PE_SHPR_H15A0_QRd03(pe1_mp_ctrl_08 ,gb_x2,	pp->mp_gb_x2,\
													gb_y2, 	pp->mp_gb_y2,\
													gb_y3,	pp->mp_gb_y3);
				PE_SHPR_H15A0_QRd04(pe1_mp_ctrl_09, lum1_x_l0,	pp->mp_lum1_x_L0,\
													lum1_x_l1,	pp->mp_lum1_x_L1,\
													lum1_x_h0,	pp->mp_lum1_x_H0,\
													lum1_x_h1,	pp->mp_lum1_x_H1);
				PE_SHPR_H15A0_QRd04(pe1_mp_ctrl_0a, lum1_y0,	pp->mp_lum1_y0,\
													lum1_y1,	pp->mp_lum1_y1,\
													lum1_y2,	pp->mp_lum1_y2,\
													lum2_x_l0,	pp->mp_lum2_x_L0);
				PE_SHPR_H15A0_QRd04(pe1_mp_ctrl_0b, lum2_x_l1,	pp->mp_lum2_x_L1,\
													lum2_x_h0,	pp->mp_lum2_x_H0,\
													lum2_x_h1,	pp->mp_lum2_x_H1,\
													lum2_y0,	pp->mp_lum2_y0);
				PE_SHPR_H15A0_QRd02(pe1_mp_ctrl_0c, lum2_y1,	pp->mp_lum2_y1,\
													lum2_y2,	pp->mp_lum2_y2);
				PE_SHPR_H15A0_QRd04(pe1_sp_ctrl_07, gb_en,		pp->sp_gb_en,\
													gb_mode,	pp->sp_gb_mode,\
													gb_x1,		pp->sp_gb_x1,\
													gb_y1,		pp->sp_gb_y1);
				PE_SHPR_H15A0_QRd03(pe1_sp_ctrl_08, gb_x2,		pp->sp_gb_x2,\
													gb_y2, 		pp->sp_gb_y2,\
													gb_y3,		pp->sp_gb_y3);
				PE_SHPR_H15A0_QRd04(pe1_sp_ctrl_09, lum1_x_l0,	pp->sp_lum1_x_L0,\
													lum1_x_l1,	pp->sp_lum1_x_L1,\
													lum1_x_h0,	pp->sp_lum1_x_H0,\
													lum1_x_h1,	pp->sp_lum1_x_H1);
				PE_SHPR_H15A0_QRd04(pe1_sp_ctrl_0a, lum1_y0,	pp->sp_lum1_y0,\
													lum1_y1,	pp->sp_lum1_y1,\
													lum1_y2,	pp->sp_lum1_y2,\
													lum2_x_l0,	pp->sp_lum2_x_L0);
				PE_SHPR_H15A0_QRd04(pe1_sp_ctrl_0b, lum2_x_l1,	pp->sp_lum2_x_L1,\
													lum2_x_h0,	pp->sp_lum2_x_H0,\
													lum2_x_h1,	pp->sp_lum2_x_H1,\
													lum2_y0,	pp->sp_lum2_y0);
				PE_SHPR_H15A0_QRd02(pe1_sp_ctrl_0c, lum2_y1,	pp->sp_lum2_y1,\
													lum2_y2,	pp->sp_lum2_y2);
				/* ti*/
				PE_SHPR_H15A0_QRd03(pe1_pti_ctrl_00, reg_ti_enable,		pp->reg_ti_enable_pti_v,\
													 reg_mm_tap_size, 	pp->reg_mm_tap_size_pti_v,\
													 reg_avg_tap_size,	pp->reg_avg_tap_size_pti_v);
				PE_SHPR_H15A0_QRd03(pe1_pti_ctrl_01, reg_ti_enable,		pp->reg_ti_enable_pti_h,\
													 reg_mm_tap_size, 	pp->reg_mm_tap_size_pti_h,\
													 reg_avg_tap_size,	pp->reg_avg_tap_size_pti_h);
				PE_SHPR_H15A0_QRd03(pe1_sti_ctrl_00, reg_ti_enable,		pp->reg_enable_sti,\
													 reg_mm_tap_size, 	pp->reg_mm_tap_size_sti,\
													 reg_avg_tap_size,	pp->reg_avg_tap_size_sti);
				/* cti*/
				PE_SHPR_H15A0_QRd01(pe1_cti_ctrl_00,cti_en,				pp->cti_en);
				PE_SHPR_H15A0_QRd04(pe1_cti_ctrl_01,coring_th0,			pp->coring_th0,\
													coring_th1,			pp->coring_th1,\
													coring_map_filter, 	pp->coring_map_filter,\
													coring_tap_size, 	pp->coring_tap_size);
				PE_SHPR_H15A0_QRd03(pe1_cti_ctrl_02,ycm_en1,			pp->ycm_en1,\
													ycm_band_sel,		pp->ycm_band_sel,\
													ycm_diff_th,		pp->ycm_diff_th);

				/* tgn*/
				PE_SHPR_H15A0_QRd01(pe1_tgen_ctrl_00, tgen_on_off,		pp->tgen_on_off);
				PE_SHPR_H15A0_QRd04(pe1_tgen_ctrl_01, tg_delta_h_bound,	pp->tg_delta_H_bound,\
													  tg_delta_l_bound,	pp->tg_delta_L_bound,\
													  tg_delta_max,		pp->tg_delta_max,\
													  tg_delta_scale,	pp->tg_delta_scale);
				PE_SHPR_H15A0_QRd03(pe1_tgen_ctrl_02, tg_rnd_th,		pp->tg_rnd_th,\
													  tg_delta_wcurr, 	pp->tg_delta_wcurr,\
													  tg_lc_ldr_th,		pp->tg_lc_ldr_th);
				PE_SHPR_H15A0_QRd02(pe1_tgen_ctrl_04, tg_mm_offset,		pp->tg_mm_offset,\
													  tg_mm_sel_mode,	pp->tg_mm_sel_mode);
				PE_SHPR_H15A0_QRd03(pe1_tgen_ctrl_05, tg_lavg_th,		pp->tg_lavg_th,\
													  tg_gs_rnd,	 	pp->tg_gs_rnd,\
													  tg_gs_mm,			pp->tg_gs_mm);
				PE_SHPR_H15A0_QRd04(pe1_tgen_ctrl_09, tg_gain0,			pp->tg_gain0,\
													  tg_gain1,		pp->tg_gain1,\
													  tg_gain2,		pp->tg_gain2,\
													  tg_gain3,		pp->tg_gain3);
				PE_SHPR_H15A0_QRd04(pe1_tgen_ctrl_09, tg_gain4,		pp->tg_gain4,\
													  tg_gain5,		pp->tg_gain5,\
													  tg_gain6,		pp->tg_gain6,\
													  tg_gain7,		pp->tg_gain7);
			}
			PE_SHP_HW_H15_DBG_PRINT("get[%d] :  resolution misc ctrl\n"\
				" hor : 0x%02x,0x%02x,0x%02x\n"\
				" ver : 0x%02x,0x%02x,0x%02x\n"\
				" cmn : 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" djg : 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" djg : 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" emp : 0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" emp : 0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" emp : 0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" emp : 0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" emp : 0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" tmp : 0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" tmp : 0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" tmp : 0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" tmp : 0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" blc : 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" blc : 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" blc : 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" blc : 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" blc : 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" blc : 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" ptiv: 0x%02x,0x%02x,0x%02x\n"\
				" ptih: 0x%02x,0x%02x,0x%02x\n"\
				" sti : 0x%02x,0x%02x,0x%02x\n"\
				" cti : 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" tgen: 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" tgen: 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" tgen: 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" tgen: 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n",\
				pp->win_id, \
				/* h 3 */
				pp->edge_Y_filter_en, pp->e_gain_max, pp->f_gain_max, \
				/* v 3 */
				pp->der_v_en, pp->der_gain_mapping, pp->max_sel, \
				/* cmm 5 */
				pp->mp_laplacian_weight,pp->sp_laplacian_weight, \
				pp->flat_filter_en,pp->flat_filter_type,pp->flat_filter_gain,\
				/*djeg 12*/
				pp->edf_en,pp->center_blur_mode,pp->count_diff_th,pp->n_avg_mode,\
				pp->line_variation_diff_threshold,\
				pp->level_th,pp->protect_th,pp->n_avg_gain,\
				pp->reg_g0_cnt_min,pp->reg_g0_mul,pp->reg_g1_protect_min,pp->reg_g1_mul,\
				/*amap 20*/
				pp->reg_cross_th_sc,pp->reg_cross_th,pp->reg_mul_base_t,pp->reg_mul_base_e,\
				pp->reg_a_map_h_size,pp->reg_a_nomalize_en,pp->reg_edge_alpha_mul	,pp->reg_edge_beta_mul,\
				pp->reg_a_lut_edge_x0,pp->reg_a_lut_edge_y0,pp->reg_a_lut_edge_x1,pp->reg_a_lut_edge_y1,\
				pp->reg_a_lut_detail_x0,pp->reg_a_lut_detail_y0,pp->reg_a_lut_detail_x1,pp->reg_a_lut_detail_y1,\
				pp->reg_edge_minmax_x0,pp->reg_edge_minmax_y0,pp->reg_edge_minmax_x1,pp->reg_edge_minmax_y1,\
				/* tmap 16*/
				pp->reg_t_cut_resolution,pp->reg_t_nomalize_en,pp->reg_detail_alpha_mul,pp->reg_detail_beta_mul,\
				pp->reg_t_lut_edge_x0,pp->reg_t_lut_edge_y0,pp->reg_t_lut_edge_x1,pp->reg_t_lut_edge_y1,\
				pp->reg_t_lut_detail_x0,pp->reg_t_lut_detail_y0,pp->reg_t_lut_detail_x1,pp->reg_t_lut_detail_y1,\
				pp->reg_detail_minmax_x0,pp->reg_detail_minmax_y0,pp->reg_detail_minmax_x1,pp->reg_detail_minmax_y1,\
				/* balnce 42*/
				pp->mp_gb_en,pp->mp_gb_mode,\
				pp->mp_gb_x1,pp->mp_gb_x2,pp->mp_gb_y1,pp->mp_gb_y2,pp->mp_gb_y3,\
				pp->sp_gb_en,pp->sp_gb_mode,\
				pp->sp_gb_x1,pp->sp_gb_x2,pp->sp_gb_y1,pp->sp_gb_y2,pp->sp_gb_y3,\
				pp->mp_lum1_x_L0,pp->mp_lum1_x_L1,pp->mp_lum1_x_H0,pp->mp_lum1_x_H1,\
				pp->mp_lum1_y0,pp->mp_lum1_y1,pp->mp_lum1_y2,\
				pp->mp_lum2_x_L0,pp->mp_lum2_x_L1,pp->mp_lum2_x_H0,pp->mp_lum2_x_H1,\
				pp->mp_lum2_y0,pp->mp_lum2_y1,pp->mp_lum2_y2,\
				pp->sp_lum1_x_L0,pp->sp_lum1_x_L1,pp->sp_lum1_x_H0,pp->sp_lum1_x_H1,\
				pp->sp_lum1_y0,pp->sp_lum1_y1,pp->sp_lum1_y2,\
				pp->sp_lum2_x_L0,pp->sp_lum2_x_L1,pp->sp_lum2_x_H0,pp->sp_lum2_x_H1,\
				pp->sp_lum2_y0,pp->sp_lum2_y1,pp->sp_lum2_y2,\
				/* ti 9*/
				pp->reg_ti_enable_pti_v,pp->reg_mm_tap_size_pti_v,pp->reg_avg_tap_size_pti_v,\
				pp->reg_ti_enable_pti_h,pp->reg_mm_tap_size_pti_h,pp->reg_avg_tap_size_pti_h,\
				pp->reg_enable_sti,pp->reg_mm_tap_size_sti,pp->reg_avg_tap_size_sti,\
				/* cti 8 */
				pp->cti_en, pp->coring_th0, \
				pp->coring_th1, pp->coring_map_filter, \
				pp->coring_tap_size, pp->ycm_en1, \
				pp->ycm_band_sel, pp->ycm_diff_th,\
				/* tgen 23*/
				pp->tgen_on_off,pp->tg_delta_H_bound,pp->tg_delta_L_bound,pp->tg_delta_max,pp->tg_delta_scale,\
				pp->tg_rnd_th,pp->tg_delta_wcurr,pp->tg_lc_ldr_th,\
				pp->tg_dr_ratio,pp->tg_dr_th,pp->tg_mm_offset,pp->tg_mm_sel_mode,\
				pp->tg_lavg_th,pp->tg_gs_rnd,pp->tg_gs_mm,\
				pp->tg_gain0,pp->tg_gain1,pp->tg_gain2,pp->tg_gain3,\
				pp->tg_gain4,pp->tg_gain5,pp->tg_gain6,pp->tg_gain7);
			#else
			LX_PE_SHP_RE3_MISC_T *pp=(LX_PE_SHP_RE3_MISC_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			win_id = PE_GET_CHECKED_WINID(pp->win_id);
			if(PE_CHECK_WIN0(win_id))
			{
				PE_FWI_H15_SETTINGS_T fw_inf;
				ret = PE_FWI_H15_GetCurFwiSettings(&fw_inf);
				PE_SHP_HW_H15_CHECK_CODE(ret, break, \
					"[%s,%d] PE_FWI_H15_GetCurFwiSettings() error.\n", __F__, __L__);
				PE_SHP_HW_H15_DBG_PRINT("fw_ctrl.enable : %s.\n", \
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
					#ifdef PE_HW_H15_FOR_BRINGUP
					#else
					PE_SHPL_H15A0_RdFL(pe1_coring_ctrl_05);
					PE_SHPL_H15A0_RdFL(pe1_coring_ctrl_06);
					PE_SHPL_H15A0_Rd01(pe1_coring_ctrl_05,	gt_th1,		pp->gt_th1);
					PE_SHPL_H15A0_Rd01(pe1_coring_ctrl_05,	gt_th0,		pp->gt_th0);
					PE_SHPL_H15A0_Rd01(pe1_coring_ctrl_06,	gt_th0a,	pp->gt_th0a);
					PE_SHPL_H15A0_Rd01(pe1_coring_ctrl_06,	gt_th0b,	pp->gt_th0b);
					#endif
				}
				/* h 5 */
				PE_SHPL_H15A0_QRd01(pe1_derh_ctrl_06, 	edge_y_filter_en,	pp->edge_y_filter_en);
				PE_SHPL_H15A0_QRd02(pe1_derh_ctrl_0c, 	e_gain_max,			pp->e_gain_max,\
												 		f_gain_max,				pp->f_gain_max);
				PE_SHPL_H15A0_QRd01(pe1_mp_ctrl_06,	lap_h_mode,			pp->mp_lap_h_mode);
				PE_SHPL_H15A0_QRd01(pe1_sp_ctrl_06,	lap_h_mode,			pp->sp_lap_h_mode);
				/* v 5 */
				PE_SHPL_H15A0_QRd02(pe1_derv_ctrl_0,	der_v_en,			pp->der_v_en,\
														der_gain_mapping,	pp->der_gain_mapping);
				PE_SHPL_H15A0_QRd01(pe1_derv_ctrl_2,	max_sel,			pp->max_sel);
				PE_SHPL_H15A0_QRd01(pe1_mp_ctrl_06,	lap_v_mode,			pp->mp_lap_v_mode);
				PE_SHPL_H15A0_QRd01(pe1_sp_ctrl_06,	lap_v_mode,			pp->sp_lap_v_mode);
				/* cmm 6 */
				PE_SHPL_H15A0_QRd02(pe1_mp_ctrl_02,	sobel_weight,		pp->mp_sobel_weight,\
													laplacian_weight,	pp->mp_laplacian_weight);
				PE_SHPL_H15A0_QRd02(pe1_sp_ctrl_02,	sobel_weight,		pp->sp_sobel_weight,\
													laplacian_weight,	pp->sp_laplacian_weight);
				PE_SHPL_H15A0_QRd02(pe1_derh_ctrl_07, 	flat_filter_en,		pp->flat_en,\
												 		flat_filter_type,	pp->flat_filter_type);
				/*d_jag 12 */
				PE_SHPL_H15A0_QRd05(pe1_dj_ctrl_00,	edf_en,				pp->edf_en,\
													center_blur_mode, 	pp->center_blur_mode,\
													count_diff_th, 		pp->count_diff_th,\
													n_avg_mode,			pp->n_avg_mode,\
													line_variation_diff_threshold, pp->line_variation_diff_th);
				PE_SHPL_H15A0_QRd03(pe1_dj_ctrl_01,	level_th,			pp->level_th,\
													protect_th,			pp->protect_th,\
													n_avg_gain,			pp->n_avg_gain);
				PE_SHPL_H15A0_QRd02(pe1_dj_ctrl_02,	reg_g0_cnt_min,		pp->reg_g0_cnt_min,\
													reg_g0_mul,			pp->reg_g0_mul);
				PE_SHPL_H15A0_QRd02(pe1_dj_ctrl_03,	reg_g1_protect_min,	pp->reg_g1_protect_min,\
													reg_g1_mul, 		pp->reg_g1_mul);
				#ifdef PE_HW_H15_FOR_BRINGUP
				#else
				/*e_map 5 */
				PE_SHPL_H15A0_QRd04(pe1_coring_ctrl_00,	amap2_sel,		pp->amap2_sel,\
														ga_max,			pp->ga_max,\
														ga_th0,			pp->ga_th0,\
														ga_th1,			pp->ga_th1);
				PE_SHPL_H15A0_QRd01(pe1_coring_ctrl_07,	amap_gain,		pp->amap_gain);
				/*t_map 22*/
				PE_SHPL_H15A0_QRd01(pe1_coring_ctrl_00,	amap1_sel,		pp->amap1_sel);
				PE_SHPL_H15A0_QRd04(pe1_coring_ctrl_01,	max_sel,		pp->tmap_max_sel,\
														avg_sel,		pp->avg_sel,\
														a2tw_th0,		pp->a2tw_th0,\
														a2tw_th1,		pp->a2tw_th1);
				PE_SHPL_H15A0_QRd03(pe1_coring_ctrl_07,	tmap_gain,		pp->tmap_gain,\
														g_th0,			pp->g_th0,\
														g_th1,			pp->g_th1);				
				PE_SHPL_H15A0_QRd02(pe1_coring_ctrl_06,	gt_gain0a,		pp->gt_gain0a,\
														gt_gain0b,		pp->gt_gain0b);
				PE_SHPL_H15A0_QRd02(pe1_coring_ctrl_05,	gt_max,			pp->gt_max,\
														a2tw_en,		pp->a2tw_en);
				PE_SHPL_H15A0_QRd05(pe1_coring_ctrl_04,	exp_mode,		pp->exp_mode,\
														coring_mode1,	pp->coring_mode1,\
														coring_mode2,	pp->coring_mode2,\
														var_v_th,		pp->var_v_th,\
														tmap_sc_var_th,	pp->tmap_sc_var_th);
				PE_SHPL_H15A0_QRd01(pe1_coring_ctrl_02,	var_h_th,		pp->var_h_th);
				/*ti-h 7 */
				PE_SHPL_H15A0_QRd02(pe1_ti_ctrl_0, 		enable,			pp->enable,\
														coring_step, 	pp->coring_step);
				PE_SHPL_H15A0_QRd04(pe1_ti_ctrl_1, 		gain0_en, 		pp->gain0_en,\
														gain1_en, 		pp->gain1_en,\
														gain0_th0,		pp->gain0_th0,\
														gain0_th1,		pp->gain0_th1);
				PE_SHPL_H15A0_QRd01(pe1_ti_ctrl_2, 		gain1_div_mode, pp->gain1_div_mode);
				#endif
				/* cti 8 */
				PE_SHPL_H15A0_QRd01(pe1_cti_ctrl_00,	cti_en,				pp->cti_en);
				PE_SHPL_H15A0_QRd04(pe1_cti_ctrl_01,	coring_th0,			pp->coring_th0,\
														coring_th1,			pp->coring_th1,\
														coring_map_filter, 	pp->coring_map_filter,\
														coring_tap_size, 	pp->coring_tap_size);
				PE_SHPL_H15A0_QRd03(pe1_cti_ctrl_02,	ycm_en1,			pp->ycm_en1,\
														ycm_band_sel,		pp->ycm_band_sel,\
														ycm_diff_th,		pp->ycm_diff_th);
			}
			if(PE_CHECK_WIN1(win_id))
			{
				PE_FWI_H15_SETTINGS_T fw_inf;
				ret = PE_FWI_H15_GetCurFwiSettings(&fw_inf);
				PE_SHP_HW_H15_CHECK_CODE(ret, break, \
					"[%s,%d] PE_FWI_H15_GetCurFwiSettings() error.\n", __F__, __L__);
				PE_SHP_HW_H15_DBG_PRINT("fw_ctrl.enable : %s.\n", \
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
					#ifdef PE_HW_H15_FOR_BRINGUP
					#else
					PE_SHPR_H15A0_RdFL(pe1_coring_ctrl_05);
					PE_SHPR_H15A0_RdFL(pe1_coring_ctrl_06);
					PE_SHPR_H15A0_Rd01(pe1_coring_ctrl_05,	gt_th1,		pp->gt_th1);
					PE_SHPR_H15A0_Rd01(pe1_coring_ctrl_05,	gt_th0,		pp->gt_th0);
					PE_SHPR_H15A0_Rd01(pe1_coring_ctrl_06,	gt_th0a,	pp->gt_th0a);
					PE_SHPR_H15A0_Rd01(pe1_coring_ctrl_06,	gt_th0b,	pp->gt_th0b);
					#endif
				}
				/* h 5 */
				PE_SHPR_H15A0_QRd01(pe1_derh_ctrl_06, 	edge_y_filter_en,	pp->edge_y_filter_en);
				PE_SHPR_H15A0_QRd02(pe1_derh_ctrl_0c, 	e_gain_max,			pp->e_gain_max,\
												 		f_gain_max,				pp->f_gain_max);
				PE_SHPR_H15A0_QRd01(pe1_mp_ctrl_06,	lap_h_mode,			pp->mp_lap_h_mode);
				PE_SHPR_H15A0_QRd01(pe1_sp_ctrl_06,	lap_h_mode,			pp->sp_lap_h_mode);
				/* v 5 */
				PE_SHPR_H15A0_QRd02(pe1_derv_ctrl_0,	der_v_en,			pp->der_v_en,\
														der_gain_mapping,	pp->der_gain_mapping);
				PE_SHPR_H15A0_QRd01(pe1_derv_ctrl_2,	max_sel,			pp->max_sel);
				PE_SHPR_H15A0_QRd01(pe1_mp_ctrl_06,	lap_v_mode,			pp->mp_lap_v_mode);
				PE_SHPR_H15A0_QRd01(pe1_sp_ctrl_06,	lap_v_mode,			pp->sp_lap_v_mode);
				/* cmm 6 */
				PE_SHPR_H15A0_QRd02(pe1_mp_ctrl_02,	sobel_weight,		pp->mp_sobel_weight,\
													laplacian_weight,	pp->mp_laplacian_weight);
				PE_SHPR_H15A0_QRd02(pe1_sp_ctrl_02,	sobel_weight,		pp->sp_sobel_weight,\
													laplacian_weight,	pp->sp_laplacian_weight);
				PE_SHPR_H15A0_QRd02(pe1_derh_ctrl_07, 	flat_filter_en,		pp->flat_en,\
												 		flat_filter_type,	pp->flat_filter_type);
				/*d_jag 12 */
				PE_SHPR_H15A0_QRd05(pe1_dj_ctrl_00,	edf_en,				pp->edf_en,\
													center_blur_mode, 	pp->center_blur_mode,\
													count_diff_th, 		pp->count_diff_th,\
													n_avg_mode,			pp->n_avg_mode,\
													line_variation_diff_threshold, pp->line_variation_diff_th);
				PE_SHPR_H15A0_QRd03(pe1_dj_ctrl_01,	level_th,			pp->level_th,\
													protect_th,			pp->protect_th,\
													n_avg_gain,			pp->n_avg_gain);
				PE_SHPR_H15A0_QRd02(pe1_dj_ctrl_02,	reg_g0_cnt_min,		pp->reg_g0_cnt_min,\
													reg_g0_mul,			pp->reg_g0_mul);
				PE_SHPR_H15A0_QRd02(pe1_dj_ctrl_03,	reg_g1_protect_min,	pp->reg_g1_protect_min,\
													reg_g1_mul, 		pp->reg_g1_mul);
				#ifdef PE_HW_H15_FOR_BRINGUP
				#else
				/*e_map 5 */
				PE_SHPR_H15A0_QRd04(pe1_coring_ctrl_00,	amap2_sel,		pp->amap2_sel,\
														ga_max,			pp->ga_max,\
														ga_th0,			pp->ga_th0,\
														ga_th1,			pp->ga_th1);
				PE_SHPR_H15A0_QRd01(pe1_coring_ctrl_07,	amap_gain,		pp->amap_gain);
				/*t_map 22*/
				PE_SHPR_H15A0_QRd01(pe1_coring_ctrl_00,	amap1_sel,		pp->amap1_sel);
				PE_SHPR_H15A0_QRd04(pe1_coring_ctrl_01,	max_sel,		pp->tmap_max_sel,\
														avg_sel,		pp->avg_sel,\
														a2tw_th0,		pp->a2tw_th0,\
														a2tw_th1,		pp->a2tw_th1);
				PE_SHPR_H15A0_QRd03(pe1_coring_ctrl_07,	tmap_gain,		pp->tmap_gain,\
														g_th0,			pp->g_th0,\
														g_th1,			pp->g_th1);				
				PE_SHPR_H15A0_QRd02(pe1_coring_ctrl_06,	gt_gain0a,		pp->gt_gain0a,\
														gt_gain0b,		pp->gt_gain0b);
				PE_SHPR_H15A0_QRd02(pe1_coring_ctrl_05,	gt_max,			pp->gt_max,\
														a2tw_en,		pp->a2tw_en);
				PE_SHPR_H15A0_QRd05(pe1_coring_ctrl_04,	exp_mode,		pp->exp_mode,\
														coring_mode1,	pp->coring_mode1,\
														coring_mode2,	pp->coring_mode2,\
														var_v_th,		pp->var_v_th,\
														tmap_sc_var_th,	pp->tmap_sc_var_th);
				PE_SHPR_H15A0_QRd01(pe1_coring_ctrl_02,	var_h_th,		pp->var_h_th);
				/*ti-h 7 */
				PE_SHPR_H15A0_QRd02(pe1_ti_ctrl_0, 		enable,			pp->enable,\
														coring_step, 	pp->coring_step);
				PE_SHPR_H15A0_QRd04(pe1_ti_ctrl_1, 		gain0_en, 		pp->gain0_en,\
														gain1_en, 		pp->gain1_en,\
														gain0_th0,		pp->gain0_th0,\
														gain0_th1,		pp->gain0_th1);
				PE_SHPR_H15A0_QRd01(pe1_ti_ctrl_2, 		gain1_div_mode, pp->gain1_div_mode);
				#endif
				/* cti 8 */
				PE_SHPR_H15A0_QRd01(pe1_cti_ctrl_00,	cti_en,				pp->cti_en);
				PE_SHPR_H15A0_QRd04(pe1_cti_ctrl_01,	coring_th0,			pp->coring_th0,\
														coring_th1,			pp->coring_th1,\
														coring_map_filter, 	pp->coring_map_filter,\
														coring_tap_size, 	pp->coring_tap_size);
				PE_SHPR_H15A0_QRd03(pe1_cti_ctrl_02,	ycm_en1,			pp->ycm_en1,\
														ycm_band_sel,		pp->ycm_band_sel,\
														ycm_diff_th,		pp->ycm_diff_th);
			}
			PE_SHP_HW_H15_DBG_PRINT("get[%d] :  resolution misc ctrl\n"\
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
			#endif
		}
		else
		{
			PE_SHP_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * set edge enhance control
 * - use input struct LX_PE_SHP_EE1_T for H15
 *
 * @param   *pstParams [in] void
 * @return  OK if success, ERROR otherwise.
 * @see		function description
 * @author
 */
int PE_SHP_HW_H15_SetEdgeEnhanceCtrl(void *pstParams)
{
	int ret = RET_OK;
	do {
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H15)
		{
			LX_PE_SHP_EE1_T *pp=(LX_PE_SHP_EE1_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			PE_SHP_HW_H15_DBG_PRINT("set[%d]: mp g_b,w:%d,%d, sp g_b,w:%d,%d\n",pp->win_id,\
				pp->mp_edge_gain_b,pp->mp_edge_gain_w,pp->sp_edge_gain_b,pp->sp_edge_gain_w);
			if(PE_CHECK_WIN0(pp->win_id))
			{
				PE_FWI_H15_EDGE_GAIN fw_param;
				fw_param.co09_gain_b = GET_BITS(pp->mp_edge_gain_b,0,7);
				fw_param.co09_gain_w = GET_BITS(pp->mp_edge_gain_w,0,7);
				fw_param.dp01_gain_b = GET_BITS(pp->sp_edge_gain_b,0,7);
				fw_param.dp01_gain_w = GET_BITS(pp->sp_edge_gain_w,0,7);
				ret = PE_FWI_H15_SetEdgeGainCtrl(&fw_param);
				PE_SHP_HW_H15_CHECK_CODE(ret, break, "[%s,%d] PE_FWI_H15_SetEdgeGainCtrl() error.\n", __F__, __L__);
				#ifdef PE_HW_H15_PROTECT_OVERLAP_SETTINGS
				#ifdef PE_SHP_HW_H15A_SET_FW_PARAM_TO_REG_DIRECTLY
				PE_SHP_HW_H15A_RES_WR02(pe1_coring_ctrl_09,edge_gain_b,GET_BITS(pp->mp_edge_gain_b,0,7), \
                                                           edge_gain_w,GET_BITS(pp->mp_edge_gain_w,0,7));
				PE_SHP_HW_H15A_RES_WR02(pe1_dp_ctrl_01,edge_gain_b,GET_BITS(pp->sp_edge_gain_b,0,7), \
                                                       edge_gain_w,GET_BITS(pp->sp_edge_gain_w,0,7));
				#else
				PE_SHP_HW_H15A_RES_SET_USER(pe1_coring_ctrl_09,edge_gain_b,GET_BITS(pp->mp_edge_gain_b,0,7));
				PE_SHP_HW_H15A_RES_SET_USER(pe1_coring_ctrl_09,edge_gain_w,GET_BITS(pp->mp_edge_gain_w,0,7));
				PE_SHP_HW_H15A_RES_SET_USER(pe1_dp_ctrl_01,edge_gain_b,GET_BITS(pp->sp_edge_gain_b,0,7));
				PE_SHP_HW_H15A_RES_SET_USER(pe1_dp_ctrl_01,edge_gain_w,GET_BITS(pp->sp_edge_gain_w,0,7));
				#endif
				#else
				#ifdef PE_SHP_HW_H15A_SET_FW_PARAM_TO_REG_DIRECTLY
				PE_SHPLR_H15A0_QWr02(pe1_coring_ctrl_09,edge_gain_b,GET_BITS(pp->mp_edge_gain_b,0,7), \
														edge_gain_w,GET_BITS(pp->mp_edge_gain_w,0,7));
				PE_SHPLR_H15A0_QWr02(pe1_dp_ctrl_01,	edge_gain_b,GET_BITS(pp->sp_edge_gain_b,0,7), \
														edge_gain_w,GET_BITS(pp->sp_edge_gain_w,0,7));
				#endif
				#endif
			}
		}
		else
		{
			PE_SHP_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * get edge enhance control
 * - use input struct LX_PE_SHP_EE1_T for H15
 *
 * @param   *pstParams [in/out] void
 * @return  OK if success, ERROR otherwise.
 * @see		function description
 * @author
 */
int PE_SHP_HW_H15_GetEdgeEnhanceCtrl(void *pstParams)
{
	int ret = RET_OK;
	LX_PE_WIN_ID win_id;
	do {
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H15)
		{
			LX_PE_SHP_EE1_T *pp=(LX_PE_SHP_EE1_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			win_id = PE_GET_CHECKED_WINID(pp->win_id);
			if(PE_CHECK_WIN0(win_id))
			{
				PE_FWI_H15_SETTINGS_T fw_inf;
				ret = PE_FWI_H15_GetCurFwiSettings(&fw_inf);
				PE_SHP_HW_H15_CHECK_CODE(ret, break, \
					"[%s,%d] PE_FWI_H15_GetCurFwiSettings() error.\n", __F__, __L__);
				PE_SHP_HW_H15_DBG_PRINT("fw_ctrl.enable : %s.\n", \
					fw_inf.fw_ctrl.enable? "yes":"no");
				#ifdef PE_SHP_HW_H15A_SET_FW_PARAM_TO_REG_DIRECTLY
				PE_SHPL_H15A0_RdFL(pe1_coring_ctrl_09);
				PE_SHPL_H15A0_RdFL(pe1_dp_ctrl_01);
				PE_SHPL_H15A0_Rd01(pe1_coring_ctrl_09,	edge_gain_b,	pp->mp_edge_gain_b);
				PE_SHPL_H15A0_Rd01(pe1_coring_ctrl_09,	edge_gain_w,	pp->mp_edge_gain_w);
				PE_SHPL_H15A0_Rd01(pe1_dp_ctrl_01,		edge_gain_b,	pp->sp_edge_gain_b);
				PE_SHPL_H15A0_Rd01(pe1_dp_ctrl_01,		edge_gain_w,	pp->sp_edge_gain_w);
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
					PE_SHPL_H15A0_RdFL(pe1_coring_ctrl_09);
					PE_SHPL_H15A0_RdFL(pe1_dp_ctrl_01);
					PE_SHPL_H15A0_Rd01(pe1_coring_ctrl_09,	edge_gain_b,	pp->mp_edge_gain_b);
					PE_SHPL_H15A0_Rd01(pe1_coring_ctrl_09,	edge_gain_w,	pp->mp_edge_gain_w);
					PE_SHPL_H15A0_Rd01(pe1_dp_ctrl_01,		edge_gain_b,	pp->sp_edge_gain_b);
					PE_SHPL_H15A0_Rd01(pe1_dp_ctrl_01,		edge_gain_w,	pp->sp_edge_gain_w);
				}
				#endif
			}
			if(PE_CHECK_WIN1(win_id))
			{
				PE_FWI_H15_SETTINGS_T fw_inf;
				ret = PE_FWI_H15_GetCurFwiSettings(&fw_inf);
				PE_SHP_HW_H15_CHECK_CODE(ret, break, \
					"[%s,%d] PE_FWI_H15_GetCurFwiSettings() error.\n", __F__, __L__);
				PE_SHP_HW_H15_DBG_PRINT("fw_ctrl.enable : %s.\n", \
					fw_inf.fw_ctrl.enable? "yes":"no");
				#ifdef PE_SHP_HW_H15A_SET_FW_PARAM_TO_REG_DIRECTLY
				PE_SHPR_H15A0_RdFL(pe1_coring_ctrl_09);
				PE_SHPR_H15A0_RdFL(pe1_dp_ctrl_01);
				PE_SHPR_H15A0_Rd01(pe1_coring_ctrl_09,	edge_gain_b,	pp->mp_edge_gain_b);
				PE_SHPR_H15A0_Rd01(pe1_coring_ctrl_09,	edge_gain_w,	pp->mp_edge_gain_w);
				PE_SHPR_H15A0_Rd01(pe1_dp_ctrl_01,		edge_gain_b,	pp->sp_edge_gain_b);
				PE_SHPR_H15A0_Rd01(pe1_dp_ctrl_01,		edge_gain_w,	pp->sp_edge_gain_w);
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
					PE_SHPR_H15A0_RdFL(pe1_coring_ctrl_09);
					PE_SHPR_H15A0_RdFL(pe1_dp_ctrl_01);
					PE_SHPR_H15A0_Rd01(pe1_coring_ctrl_09,	edge_gain_b,	pp->mp_edge_gain_b);
					PE_SHPR_H15A0_Rd01(pe1_coring_ctrl_09,	edge_gain_w,	pp->mp_edge_gain_w);
					PE_SHPR_H15A0_Rd01(pe1_dp_ctrl_01,		edge_gain_b,	pp->sp_edge_gain_b);
					PE_SHPR_H15A0_Rd01(pe1_dp_ctrl_01,		edge_gain_w,	pp->sp_edge_gain_w);
				}
				#endif
			}
			PE_SHP_HW_H15_DBG_PRINT("get[%d]: mp g_b,w:%d,%d, sp g_b,w:%d,%d\n",\
				pp->win_id,pp->mp_edge_gain_b,pp->mp_edge_gain_w,\
				pp->sp_edge_gain_b,pp->sp_edge_gain_w);
		}
		else
		{
			PE_SHP_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * set detail enhance control
 * - use input struct LX_PE_SHP_DE1_T for H15
 *
 * @param   *pstParams [in] void
 * @return  OK if success, ERROR otherwise.
 * @see		function description
 * @author
 */
int PE_SHP_HW_H15_SetDetailEnhanceCtrl(void *pstParams)
{
	int ret = RET_OK;
	do {
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H15)
		{
			LX_PE_SHP_DE1_T *pp=(LX_PE_SHP_DE1_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			PE_SHP_HW_H15_DBG_PRINT("set[%d]: mp g_b,w:%d,%d, sp g_b,w:%d,%d\n",\
				pp->win_id,pp->mp_texture_gain_b,pp->mp_texture_gain_w,\
				pp->sp_texture_gain_b,pp->sp_texture_gain_w);
			if(PE_CHECK_WIN0(pp->win_id))
			{
				PE_FWI_H15_TEXTURE_GAIN fw_param;
				fw_param.co09_gain_b = GET_BITS(pp->mp_texture_gain_b,0,7);
				fw_param.co09_gain_w = GET_BITS(pp->mp_texture_gain_w,0,7);
				fw_param.dp01_gain_b = GET_BITS(pp->sp_texture_gain_b,0,7);
				fw_param.dp01_gain_w = GET_BITS(pp->sp_texture_gain_w,0,7);
				ret = PE_FWI_H15_SetTextureGainCtrl(&fw_param);
				PE_SHP_HW_H15_CHECK_CODE(ret, break, \
					"[%s,%d] PE_FWI_H15_SetTextureGainCtrl() error.\n", __F__, __L__);
				#ifdef PE_HW_H15_PROTECT_OVERLAP_SETTINGS
				#ifdef PE_SHP_HW_H15A_SET_FW_PARAM_TO_REG_DIRECTLY
				PE_SHP_HW_H15A_RES_WR02(pe1_coring_ctrl_09,texture_gain_b,GET_BITS(pp->mp_texture_gain_b,0,7), \
                                                           texture_gain_w,GET_BITS(pp->mp_texture_gain_w,0,7));
				PE_SHP_HW_H15A_RES_WR02(pe1_dp_ctrl_01,texture_gain_b,GET_BITS(pp->sp_texture_gain_b,0,7), \
                                                       texture_gain_w,GET_BITS(pp->sp_texture_gain_w,0,7));
				#else
				PE_SHP_HW_H15A_RES_SET_USER(pe1_coring_ctrl_09,texture_gain_b,GET_BITS(pp->mp_texture_gain_b,0,7));
				PE_SHP_HW_H15A_RES_SET_USER(pe1_coring_ctrl_09,texture_gain_w,GET_BITS(pp->mp_texture_gain_w,0,7));
				PE_SHP_HW_H15A_RES_SET_USER(pe1_dp_ctrl_01,texture_gain_b,GET_BITS(pp->sp_texture_gain_b,0,7));
				PE_SHP_HW_H15A_RES_SET_USER(pe1_dp_ctrl_01,texture_gain_w,GET_BITS(pp->sp_texture_gain_w,0,7));
				#endif
				#else
				#ifdef PE_SHP_HW_H15A_SET_FW_PARAM_TO_REG_DIRECTLY
				PE_SHPLR_H15A0_QWr02(pe1_coring_ctrl_09,texture_gain_b,GET_BITS(pp->mp_texture_gain_b,0,7), \
														texture_gain_w,GET_BITS(pp->mp_texture_gain_w,0,7));
				PE_SHPLR_H15A0_QWr02(pe1_dp_ctrl_01,texture_gain_b,GET_BITS(pp->sp_texture_gain_b,0,7), \
													texture_gain_w,GET_BITS(pp->sp_texture_gain_w,0,7));
				#endif
				#endif
			}
		}
		else
		{
			PE_SHP_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * get detail enhance control
 * - use input struct LX_PE_SHP_DE1_T for H15
 *
 * @param   *pstParams [in/out] void
 * @return  OK if success, ERROR otherwise.
 * @see		function description
 * @author
 */
int PE_SHP_HW_H15_GetDetailEnhanceCtrl(void *pstParams)
{
	int ret = RET_OK;
	LX_PE_WIN_ID win_id;
	do {
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H15)
		{
			LX_PE_SHP_DE1_T *pp=(LX_PE_SHP_DE1_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			win_id = PE_GET_CHECKED_WINID(pp->win_id);
			if(PE_CHECK_WIN0(win_id))
			{
				PE_FWI_H15_SETTINGS_T fw_inf;
				ret = PE_FWI_H15_GetCurFwiSettings(&fw_inf);
				PE_SHP_HW_H15_CHECK_CODE(ret, break, \
					"[%s,%d] PE_FWI_H15_GetCurFwiSettings() error.\n", __F__, __L__);
				PE_SHP_HW_H15_DBG_PRINT("fw_ctrl.enable : %s.\n", \
					fw_inf.fw_ctrl.enable? "yes":"no");
				#ifdef PE_SHP_HW_H15A_SET_FW_PARAM_TO_REG_DIRECTLY
				PE_SHPL_H15A0_QRd02(pe1_coring_ctrl_09,texture_gain_b,pp->mp_texture_gain_b, \
														texture_gain_w,pp->mp_texture_gain_w);
				PE_SHPL_H15A0_QRd02(pe1_dp_ctrl_01,texture_gain_b,pp->sp_texture_gain_b, \
													texture_gain_w,pp->sp_texture_gain_w);
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
					PE_SHPL_H15A0_QRd02(pe1_coring_ctrl_09,texture_gain_b,pp->mp_texture_gain_b, \
														texture_gain_w,pp->mp_texture_gain_w);
					PE_SHPL_H15A0_QRd02(pe1_dp_ctrl_01,texture_gain_b,pp->sp_texture_gain_b, \
														texture_gain_w,pp->sp_texture_gain_w);
				}
				#endif
			}
			if(PE_CHECK_WIN1(win_id))
			{
				PE_FWI_H15_SETTINGS_T fw_inf;
				ret = PE_FWI_H15_GetCurFwiSettings(&fw_inf);
				PE_SHP_HW_H15_CHECK_CODE(ret, break, \
					"[%s,%d] PE_FWI_H15_GetCurFwiSettings() error.\n", __F__, __L__);
				PE_SHP_HW_H15_DBG_PRINT("fw_ctrl.enable : %s.\n", \
					fw_inf.fw_ctrl.enable? "yes":"no");
				#ifdef PE_SHP_HW_H15A_SET_FW_PARAM_TO_REG_DIRECTLY
				PE_SHPR_H15A0_QRd02(pe1_coring_ctrl_09,texture_gain_b,pp->mp_texture_gain_b, \
														texture_gain_w,pp->mp_texture_gain_w);
				PE_SHPR_H15A0_QRd02(pe1_dp_ctrl_01,texture_gain_b,pp->sp_texture_gain_b, \
													texture_gain_w,pp->sp_texture_gain_w);
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
					PE_SHPR_H15A0_QRd02(pe1_coring_ctrl_09,texture_gain_b,pp->mp_texture_gain_b, \
														texture_gain_w,pp->mp_texture_gain_w);
					PE_SHPR_H15A0_QRd02(pe1_dp_ctrl_01,texture_gain_b,pp->sp_texture_gain_b, \
														texture_gain_w,pp->sp_texture_gain_w);
				}
				#endif
			}
			PE_SHP_HW_H15_DBG_PRINT("get[%d]: mp g_b,w:%d,%d, sp g_b,w:%d,%d\n",\
				pp->win_id,pp->mp_texture_gain_b,pp->mp_texture_gain_w,\
				pp->sp_texture_gain_b,pp->sp_texture_gain_w);
		}
		else
		{
			PE_SHP_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}

/**
 * set super resolution enhancement(SRE) control
 * - use input struct LX_PE_SHP_SR1_T for H15AX
 *
 * @param   *pstParams [in] void
 * @return  OK if success, ERROR otherwise.
 * @see		function description
 * @author
 */
int PE_SHP_HW_H15_SetSreCtrl(void *pstParams)
{
	int ret = RET_OK;
	#ifdef PE_SHP_HW_H15_SR_PWD_TRACE
	UINT32 r_data[2];
	#endif
	do {
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H15)
		{
			#ifdef PE_SHP_PARAM_CHANGE_FOR_H15
			LX_PE_SHP_SR_MAIN_T *pp=(LX_PE_SHP_SR_MAIN_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			PE_SHP_HW_H15_DBG_PRINT("set[%d] : \n"\
				"iu_en:%d, 4mm_min, max:%d,%d\n"\
				"lue_delta_c:%d,%d,%d,%d,%d,%d,%d,%d,\n"\
				"lut_delta_d:%d,%d,%d,%d,%d,%d,%d,%d,\n"\
				"we_id:%d,%d,%d\n"\
				"we_range:%d,%d(uh),%d,%d(dh)\n", pp->win_id, \
				pp->use_iu_en,	pp->iu_en_4mm_min_th,pp->iu_en_4mm_max_th,\
				pp->lue_delta_c0,pp->lue_delta_c1,pp->lue_delta_c2,pp->lue_delta_c3,\
				pp->lue_delta_c4,pp->lue_delta_c5,pp->lue_delta_c6,pp->lue_delta_c7,\
				pp->lue_delta_d0,pp->lue_delta_d1,pp->lue_delta_d2,pp->lue_delta_d3,\
				pp->lue_delta_d4,pp->lue_delta_d5,pp->lue_delta_d6,pp->lue_delta_d7,\
				pp->we_id_whq_base,pp->we_id_range0,pp->we_id_range1,\
				pp->we_uh_range0,pp->we_uh_range1,pp->we_dh_range0,pp->we_dh_range1);
			if(PE_CHECK_WIN0(pp->win_id))
			{
				#ifdef PE_SHP_HW_H15_SR_PWD_TRACE
				CTOP_CTRL_H15A0_RdFL(SR,ctr00);
				CTOP_CTRL_H15A0_Rd01(SR,ctr00,cg_sr_en,r_data[0]);
				CTOP_CTRL_H15A0_Rd01(SR,ctr00,cg_shs_en,r_data[1]);
				PE_SHP_HW_H15_DBG_SRPD_TRACE("S cg sr,shs:%d,%d\n",r_data[0],r_data[1]);
				#endif
				PE_SR_S1_H15A0_QWr01(core1_use_iu_en, core1_use_iu_en, pp->use_iu_en? 0x1:0x0);
				PE_SR_S1_H15A0_QWr02(core1_iu_en_4mm_min_th, core1_iu_en_4mm_min_th, GET_BITS(pp->iu_en_4mm_min_th,0,5), \
                                                             core1_iu_en_4mm_max_th, GET_BITS(pp->iu_en_4mm_max_th,0,5));
				PE_SR_S1_H15A0_QWr04(core1_iu_delta_cross0,  core1_lue_delta_c0, GET_BITS(pp->lue_delta_c0,0,5), \
                                                             core1_lue_delta_c1, GET_BITS(pp->lue_delta_c1,0,5), \
				                                             core1_lue_delta_c2, GET_BITS(pp->lue_delta_c2,0,5), \
                                                             core1_lue_delta_c3, GET_BITS(pp->lue_delta_c3,0,5));
				PE_SR_S1_H15A0_QWr04(core1_iu_delta_cross1,  core1_lue_delta_c4, GET_BITS(pp->lue_delta_c4,0,5), \
                                                             core1_lue_delta_c5, GET_BITS(pp->lue_delta_c5,0,5), \
				                                             core1_lue_delta_c6, GET_BITS(pp->lue_delta_c6,0,5), \
                                                             core1_lue_delta_c7, GET_BITS(pp->lue_delta_c7,0,5));
				PE_SR_S1_H15A0_QWr04(core1_iu_delta_dial0,   core1_lue_delta_d0, GET_BITS(pp->lue_delta_d0,0,5), \
                                                             core1_lue_delta_d1, GET_BITS(pp->lue_delta_d1,0,5), \
				                                             core1_lue_delta_d2, GET_BITS(pp->lue_delta_d2,0,5), \
                                                             core1_lue_delta_d3, GET_BITS(pp->lue_delta_d3,0,5));
				PE_SR_S1_H15A0_QWr04(core1_iu_delta_dial1,   core1_lue_delta_d4, GET_BITS(pp->lue_delta_d4,0,5), \
                                                             core1_lue_delta_d5, GET_BITS(pp->lue_delta_d5,0,5), \
				                                             core1_lue_delta_d6, GET_BITS(pp->lue_delta_d6,0,5), \
                                                             core1_lue_delta_d7, GET_BITS(pp->lue_delta_d7,0,5));
				PE_SR_F1_H15A0_QWr02(core1_we_uh_minmax, core1_we_uh_range0, GET_BITS(pp->we_uh_range0,0,9), \
                                                         core1_we_uh_range1, GET_BITS(pp->we_uh_range1,0,9));
				PE_SR_F1_H15A0_QWr02(core1_we_dh_minmax, core1_we_dh_range0, GET_BITS(pp->we_dh_range0,0,9), \
                                                         core1_we_dh_range1, GET_BITS(pp->we_dh_range1,0,9));
				PE_SR_F1_H15A0_QWr01(core1_we_id_whq_base,  core1_we_id_whq_base, GET_BITS(pp->we_id_whq_base,0,8));
				PE_SR_S1_H15A0_QWr01(core1_we_id_0,  core1_we_id_range0, GET_BITS(pp->we_id_range0,0,8));
				PE_SR_S1_H15A0_QWr01(core1_we_id_1,  core1_we_id_range1, GET_BITS(pp->we_id_range1,0,8));
				#ifdef PE_SHP_HW_H15_SR_PWD_TRACE
				CTOP_CTRL_H15A0_RdFL(SR,ctr00);
				CTOP_CTRL_H15A0_Rd01(SR,ctr00,cg_sr_en,r_data[0]);
				CTOP_CTRL_H15A0_Rd01(SR,ctr00,cg_shs_en,r_data[1]);
				PE_SHP_HW_H15_DBG_SRPD_TRACE("E cg sr,shs:%d,%d\n",r_data[0],r_data[1]);
				#endif
			}
			#else
			LX_PE_SHP_SR1_T *pp=(LX_PE_SHP_SR1_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			PE_SHP_HW_H15_DBG_PRINT("set[%d] : iu_en:%02d, dh:%03d,%03d, uh:%03d,%03d\n", \
				pp->win_id, pp->core1_use_iu_en, pp->core1_we_dh_range0, \
				pp->core1_we_dh_range1, pp->core1_we_uh_range0, pp->core1_we_uh_range1);
			if(PE_CHECK_WIN0(pp->win_id))
			{
				PE_SR_S1_H15A0_QWr01(core1_use_iu_en, core1_use_iu_en, pp->core1_use_iu_en? 0x1:0x0);
				PE_SR_F1_H15A0_QWr02(core1_we_dh_minmax, core1_we_dh_range0, GET_BITS(pp->core1_we_dh_range0,0,9), \
                                                         core1_we_dh_range1, GET_BITS(pp->core1_we_dh_range1,0,9));
				PE_SR_F1_H15A0_QWr02(core1_we_uh_minmax, core1_we_uh_range0, GET_BITS(pp->core1_we_uh_range0,0,9), \
                                                         core1_we_uh_range1, GET_BITS(pp->core1_we_uh_range1,0,9));
			}
			#endif
		}
		else
		{
			PE_SHP_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	} while (0);
	return ret;
}
/**
 * get super resolution enhancement(SRE) control
 * - use input struct LX_PE_SHP_SR1_T for H15AX
 *
 * @param   *pstParams [in/out] void
 * @return  OK if success, ERROR otherwise.
 * @see		function description
 * @author
 */
int PE_SHP_HW_H15_GetSreCtrl(void *pstParams)
{
	int ret = RET_OK;
	LX_PE_WIN_ID win_id;
	do {
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H15)
		{
			#ifdef PE_SHP_PARAM_CHANGE_FOR_H15
			LX_PE_SHP_SR_MAIN_T *pp=(LX_PE_SHP_SR_MAIN_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			win_id = PE_GET_CHECKED_WINID(pp->win_id);
			if(PE_CHECK_WIN0(win_id))
			{
				PE_SR_S1_H15A0_QRd01(core1_use_iu_en, core1_use_iu_en, pp->use_iu_en);
				PE_SR_S1_H15A0_QRd02(core1_iu_en_4mm_min_th, core1_iu_en_4mm_min_th, pp->iu_en_4mm_min_th, \
                                                             core1_iu_en_4mm_max_th, pp->iu_en_4mm_max_th);
				PE_SR_S1_H15A0_QRd04(core1_iu_delta_cross0,  core1_lue_delta_c0, pp->lue_delta_c0, \
                                                             core1_lue_delta_c1, pp->lue_delta_c1, \
				                                             core1_lue_delta_c2, pp->lue_delta_c2, \
                                                             core1_lue_delta_c3, pp->lue_delta_c3);
				PE_SR_S1_H15A0_QRd04(core1_iu_delta_cross1,  core1_lue_delta_c4, pp->lue_delta_c4, \
                                                             core1_lue_delta_c5, pp->lue_delta_c5, \
				                                             core1_lue_delta_c6, pp->lue_delta_c6, \
                                                             core1_lue_delta_c7, pp->lue_delta_c7);
				PE_SR_S1_H15A0_QRd04(core1_iu_delta_dial0,   core1_lue_delta_d0, pp->lue_delta_d0, \
                                                             core1_lue_delta_d1, pp->lue_delta_d1, \
				                                             core1_lue_delta_d2, pp->lue_delta_d2, \
                                                             core1_lue_delta_d3, pp->lue_delta_d3);
				PE_SR_S1_H15A0_QRd04(core1_iu_delta_dial1,   core1_lue_delta_d4, pp->lue_delta_d4, \
                                                             core1_lue_delta_d5, pp->lue_delta_d5, \
				                                             core1_lue_delta_d6, pp->lue_delta_d6, \
                                                             core1_lue_delta_d7, pp->lue_delta_d7);
				PE_SR_F1_H15A0_QRd02(core1_we_uh_minmax, core1_we_uh_range0, pp->we_uh_range0, \
                                                         core1_we_uh_range1, pp->we_uh_range1);
				PE_SR_F1_H15A0_QRd02(core1_we_dh_minmax, core1_we_dh_range0, pp->we_dh_range0, \
                                                         core1_we_dh_range1, pp->we_dh_range1);
				PE_SR_F1_H15A0_QRd01(core1_we_id_whq_base,  core1_we_id_whq_base, pp->we_id_whq_base);
				#if 1
				PE_SR_S1_H15A0_QRd01(core1_we_id_0,  core1_we_id_range0, pp->we_id_range0);
				PE_SR_S1_H15A0_QRd01(core1_we_id_1,  core1_we_id_range1, pp->we_id_range1);
				#endif
			}
			PE_SHP_HW_H15_DBG_PRINT("get[%d] :  %d, %d,%d(uh),%d,%d(dh)\n", \
				pp->win_id, pp->use_iu_en, pp->we_uh_range0, \
				pp->we_uh_range1, pp->we_dh_range0, pp->we_dh_range1);
			#else
			LX_PE_SHP_SR1_T *pp=(LX_PE_SHP_SR1_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			win_id = PE_GET_CHECKED_WINID(pp->win_id);
			if(PE_CHECK_WIN0(win_id))
			{
				PE_SR_S1_H15A0_QRd01(core1_use_iu_en, core1_use_iu_en, pp->core1_use_iu_en);
				PE_SR_F1_H15A0_QRd02(core1_we_dh_minmax, core1_we_dh_range0, pp->core1_we_dh_range0, \
                                                         core1_we_dh_range1, pp->core1_we_dh_range1);
				PE_SR_F1_H15A0_QRd02(core1_we_uh_minmax, core1_we_uh_range0, pp->core1_we_uh_range0, \
                                                         core1_we_uh_range1, pp->core1_we_uh_range1);
			}
			PE_SHP_HW_H15_DBG_PRINT("get[%d] :  %d, %d,%d,%d,%d\n", \
				pp->win_id, pp->core1_use_iu_en, pp->core1_we_dh_range0, \
				pp->core1_we_dh_range1, pp->core1_we_uh_range0, pp->core1_we_uh_range1);
			#endif
		}
		else
		{
			PE_SHP_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
#ifdef PE_SHP_PARAM_CHANGE_FOR_H15

/**
 * set super resolution enhancement(SRE) control
 * - use input struct LX_PE_SHP_SR1_T for H15AX
 *
 * @param   *pstParams [in] void
 * @return  OK if success, ERROR otherwise.
 * @see		function description
 * @author
 */
int PE_SHP_HW_H15_SetSreCmnCtrl(void *pstParams)
{
	int ret = RET_OK;
	#ifdef PE_SHP_HW_H15_SR_PWD_TRACE
	UINT32 r_data[2];
	#endif
	do {
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H15)
		{
			LX_PE_SHP_SR_CMN_T *pp=(LX_PE_SHP_SR_CMN_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			PE_SHP_HW_H15_DBG_PRINT("set[%d] :\n"\
				"lut_iu :\n"\
				"%d,%d,%d,%d,%d,%d,%d,%d\n"\
				"%d,%d,%d,%d,%d,%d,%d,%d\n"\
				"lut_id :\n"\
				"%d,%d,%d,%d,%d,%d,%d,%d\n"\
				"%d,%d,%d,%d,%d,%d,%d,%d\n"\
				"jone_base, min, max: %d,%d, %d\n", pp->win_id, \
				pp->lut_iu_um_grdavg00,pp->lut_iu_um_grdavg01,pp->lut_iu_um_grdavg02,\
				pp->lut_iu_um_grdavg03,pp->lut_iu_um_grdavg04,pp->lut_iu_um_grdavg05,\
				pp->lut_iu_um_grdavg06,pp->lut_iu_um_grdavg07,pp->lut_iu_um_grdavg08,\
				pp->lut_iu_um_grdavg09,pp->lut_iu_um_grdavg10,pp->lut_iu_um_grdavg11,\
				pp->lut_iu_um_grdavg12,pp->lut_iu_um_grdavg13,pp->lut_iu_um_grdavg14,\
				pp->lut_iu_um_grdavg15,pp->lut_id_um_grdavg00,pp->lut_id_um_grdavg01,\
				pp->lut_id_um_grdavg02,pp->lut_id_um_grdavg03,pp->lut_id_um_grdavg04,\
				pp->lut_id_um_grdavg05,pp->lut_id_um_grdavg06,pp->lut_id_um_grdavg07,\
				pp->lut_id_um_grdavg08,pp->lut_id_um_grdavg09,pp->lut_id_um_grdavg10,\
				pp->lut_id_um_grdavg11,pp->lut_id_um_grdavg12,pp->lut_id_um_grdavg13,\
				pp->lut_id_um_grdavg14,pp->lut_id_um_grdavg15,pp->we_jone_base,\
				pp->we_jone_min,pp->we_jone_max);
			if(PE_CHECK_WIN0(pp->win_id))
			{
				#ifdef PE_SHP_HW_H15_SR_PWD_TRACE
				CTOP_CTRL_H15A0_RdFL(SR,ctr00);
				CTOP_CTRL_H15A0_Rd01(SR,ctr00,cg_sr_en,r_data[0]);
				CTOP_CTRL_H15A0_Rd01(SR,ctr00,cg_shs_en,r_data[1]);
				PE_SHP_HW_H15_DBG_SRPD_TRACE("S cg sr,shs:%d,%d\n",r_data[0],r_data[1]);
				#endif
				PE_SR_F1_H15A0_QWr04(core1_lut_iu_um_grdavg0, core1_lut_iu_um_grdavg0, GET_BITS(pp->lut_iu_um_grdavg00,0,5), \
				                                              core1_lut_iu_um_grdavg1, GET_BITS(pp->lut_iu_um_grdavg01,0,5), \
				                                              core1_lut_iu_um_grdavg2, GET_BITS(pp->lut_iu_um_grdavg02,0,5), \
				                                              core1_lut_iu_um_grdavg3, GET_BITS(pp->lut_iu_um_grdavg03,0,5));
				PE_SR_F1_H15A0_QWr04(core1_lut_iu_um_grdavg1, core1_lut_iu_um_grdavg4, GET_BITS(pp->lut_iu_um_grdavg04,0,5), \
				                                              core1_lut_iu_um_grdavg5, GET_BITS(pp->lut_iu_um_grdavg05,0,5), \
				                                              core1_lut_iu_um_grdavg6, GET_BITS(pp->lut_iu_um_grdavg06,0,5), \
				                                              core1_lut_iu_um_grdavg7, GET_BITS(pp->lut_iu_um_grdavg07,0,5));
				PE_SR_F1_H15A0_QWr04(core1_lut_iu_um_grdavg2, core1_lut_iu_um_grdavg8, GET_BITS(pp->lut_iu_um_grdavg08,0,5), \
				                                              core1_lut_iu_um_grdavg9, GET_BITS(pp->lut_iu_um_grdavg09,0,5), \
				                                              core1_lut_iu_um_grdavg10, GET_BITS(pp->lut_iu_um_grdavg10,0,5), \
				                                              core1_lut_iu_um_grdavg11, GET_BITS(pp->lut_iu_um_grdavg11,0,5));
				PE_SR_F1_H15A0_QWr04(core1_lut_iu_um_grdavg3, core1_lut_iu_um_grdavg12, GET_BITS(pp->lut_iu_um_grdavg12,0,5), \
				                                              core1_lut_iu_um_grdavg13, GET_BITS(pp->lut_iu_um_grdavg13,0,5), \
				                                              core1_lut_iu_um_grdavg14, GET_BITS(pp->lut_iu_um_grdavg14,0,5), \
				                                              core1_lut_iu_um_grdavg15, GET_BITS(pp->lut_iu_um_grdavg15,0,5));

				PE_SR_F1_H15A0_QWr04(core1_lut_id_um_grdavg0, core1_lut_id_um_grdavg0, GET_BITS(pp->lut_id_um_grdavg00,0,5), \
				                                              core1_lut_id_um_grdavg1, GET_BITS(pp->lut_id_um_grdavg01,0,5), \
				                                              core1_lut_id_um_grdavg2, GET_BITS(pp->lut_id_um_grdavg02,0,5), \
				                                              core1_lut_id_um_grdavg3, GET_BITS(pp->lut_id_um_grdavg03,0,5));
				PE_SR_F1_H15A0_QWr04(core1_lut_id_um_grdavg1, core1_lut_id_um_grdavg4, GET_BITS(pp->lut_id_um_grdavg04,0,5), \
				                                              core1_lut_id_um_grdavg5, GET_BITS(pp->lut_id_um_grdavg05,0,5), \
				                                              core1_lut_id_um_grdavg6, GET_BITS(pp->lut_id_um_grdavg06,0,5), \
				                                              core1_lut_id_um_grdavg7, GET_BITS(pp->lut_id_um_grdavg07,0,5));
				PE_SR_F1_H15A0_QWr04(core1_lut_id_um_grdavg2, core1_lut_id_um_grdavg8, GET_BITS(pp->lut_id_um_grdavg08,0,5), \
				                                              core1_lut_id_um_grdavg9, GET_BITS(pp->lut_id_um_grdavg09,0,5), \
				                                              core1_lut_id_um_grdavg10, GET_BITS(pp->lut_id_um_grdavg10,0,5), \
				                                              core1_lut_id_um_grdavg11, GET_BITS(pp->lut_id_um_grdavg11,0,5));
				PE_SR_F1_H15A0_QWr04(core1_lut_id_um_grdavg3, core1_lut_id_um_grdavg12, GET_BITS(pp->lut_id_um_grdavg12,0,5), \
				                                              core1_lut_id_um_grdavg13, GET_BITS(pp->lut_id_um_grdavg13,0,5), \
				                                              core1_lut_id_um_grdavg14, GET_BITS(pp->lut_id_um_grdavg14,0,5), \
				                                              core1_lut_id_um_grdavg15, GET_BITS(pp->lut_id_um_grdavg15,0,5));
				PE_SR_F1_H15A0_QWr02(core1_we_jone_minmax,    core1_we_jone_max,  GET_BITS(pp->we_jone_max,0,8),\
					                                          core1_we_jone_min,  GET_BITS(pp->we_jone_min,0,8));
				PE_SR_F1_H15A0_QWr01(core1_we_jone_base,      core1_we_jone_base, GET_BITS(pp->we_jone_base,0,8));
				#ifdef PE_SHP_HW_H15_SR_PWD_TRACE
				CTOP_CTRL_H15A0_RdFL(SR,ctr00);
				CTOP_CTRL_H15A0_Rd01(SR,ctr00,cg_sr_en,r_data[0]);
				CTOP_CTRL_H15A0_Rd01(SR,ctr00,cg_shs_en,r_data[1]);
				PE_SHP_HW_H15_DBG_SRPD_TRACE("E cg sr,shs:%d,%d\n",r_data[0],r_data[1]);
				#endif
			}
		}
		else
		{
			PE_SHP_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	} while (0);
	return ret;
}
/**
 * get super resolution enhancement(SRE) control
 * - use input struct LX_PE_SHP_SR1_T for H15AX
 *
 * @param   *pstParams [in/out] void
 * @return  OK if success, ERROR otherwise.
 * @see		function description
 * @author
 */
int PE_SHP_HW_H15_GetSreCmnCtrl(void *pstParams)
{
	int ret = RET_OK;
	LX_PE_WIN_ID win_id;
	do {
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H15)
		{
			LX_PE_SHP_SR_CMN_T *pp=(LX_PE_SHP_SR_CMN_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			win_id = PE_GET_CHECKED_WINID(pp->win_id);
			if(PE_CHECK_WIN0(win_id))
			{
				PE_SR_F1_H15A0_QRd04(core1_lut_iu_um_grdavg0, core1_lut_iu_um_grdavg0, pp->lut_iu_um_grdavg00, \
				                                              core1_lut_iu_um_grdavg1, pp->lut_iu_um_grdavg01, \
				                                              core1_lut_iu_um_grdavg2, pp->lut_iu_um_grdavg02, \
				                                              core1_lut_iu_um_grdavg3, pp->lut_iu_um_grdavg03);
				PE_SR_F1_H15A0_QRd04(core1_lut_iu_um_grdavg1, core1_lut_iu_um_grdavg4, pp->lut_iu_um_grdavg04, \
				                                              core1_lut_iu_um_grdavg5, pp->lut_iu_um_grdavg05, \
				                                              core1_lut_iu_um_grdavg6, pp->lut_iu_um_grdavg06, \
				                                              core1_lut_iu_um_grdavg7, pp->lut_iu_um_grdavg07);
				PE_SR_F1_H15A0_QRd04(core1_lut_iu_um_grdavg2, core1_lut_iu_um_grdavg8, pp->lut_iu_um_grdavg08, \
				                                              core1_lut_iu_um_grdavg9, pp->lut_iu_um_grdavg09, \
				                                              core1_lut_iu_um_grdavg10, pp->lut_iu_um_grdavg10, \
				                                              core1_lut_iu_um_grdavg11, pp->lut_iu_um_grdavg11);
				PE_SR_F1_H15A0_QRd04(core1_lut_iu_um_grdavg3, core1_lut_iu_um_grdavg12, pp->lut_iu_um_grdavg12, \
				                                              core1_lut_iu_um_grdavg13, pp->lut_iu_um_grdavg13, \
				                                              core1_lut_iu_um_grdavg14, pp->lut_iu_um_grdavg14, \
				                                              core1_lut_iu_um_grdavg15, pp->lut_iu_um_grdavg15);

				PE_SR_F1_H15A0_QRd04(core1_lut_id_um_grdavg0, core1_lut_id_um_grdavg0, pp->lut_id_um_grdavg00, \
				                                              core1_lut_id_um_grdavg1, pp->lut_id_um_grdavg01, \
				                                              core1_lut_id_um_grdavg2, pp->lut_id_um_grdavg02, \
				                                              core1_lut_id_um_grdavg3, pp->lut_id_um_grdavg03);
				PE_SR_F1_H15A0_QRd04(core1_lut_id_um_grdavg1, core1_lut_id_um_grdavg4, pp->lut_id_um_grdavg04, \
				                                              core1_lut_id_um_grdavg5, pp->lut_id_um_grdavg05, \
				                                              core1_lut_id_um_grdavg6, pp->lut_id_um_grdavg06, \
				                                              core1_lut_id_um_grdavg7, pp->lut_id_um_grdavg07);
				PE_SR_F1_H15A0_QRd04(core1_lut_id_um_grdavg2, core1_lut_id_um_grdavg8, pp->lut_id_um_grdavg08, \
				                                              core1_lut_id_um_grdavg9, pp->lut_id_um_grdavg09, \
				                                              core1_lut_id_um_grdavg10, pp->lut_id_um_grdavg10, \
				                                              core1_lut_id_um_grdavg11, pp->lut_id_um_grdavg11);
				PE_SR_F1_H15A0_QRd04(core1_lut_id_um_grdavg3, core1_lut_id_um_grdavg12, pp->lut_id_um_grdavg12, \
				                                              core1_lut_id_um_grdavg13, pp->lut_id_um_grdavg13, \
				                                              core1_lut_id_um_grdavg14, pp->lut_id_um_grdavg14, \
				                                              core1_lut_id_um_grdavg15, pp->lut_id_um_grdavg15);
				PE_SR_F1_H15A0_QRd02(core1_we_jone_minmax,    core1_we_jone_max,  pp->we_jone_max,\
					                                          core1_we_jone_min,  pp->we_jone_min);
				PE_SR_F1_H15A0_QRd01(core1_we_jone_base,      core1_we_jone_base, pp->we_jone_base);
				
			}
			PE_SHP_HW_H15_DBG_PRINT("get[%d] :\n"\
				"lut_iu :\n"\
				"%d,%d,%d,%d,%d,%d,%d,%d\n"\
				"%d,%d,%d,%d,%d,%d,%d,%d\n"\
				"lut_id :\n"\
				"%d,%d,%d,%d,%d,%d,%d,%d\n"\
				"%d,%d,%d,%d,%d,%d,%d,%d\n"\
				"jone_base, min, max: %d,%d, %d\n", pp->win_id, \
				pp->lut_iu_um_grdavg00,pp->lut_iu_um_grdavg01,pp->lut_iu_um_grdavg02,\
				pp->lut_iu_um_grdavg03,pp->lut_iu_um_grdavg04,pp->lut_iu_um_grdavg05,\
				pp->lut_iu_um_grdavg06,pp->lut_iu_um_grdavg07,pp->lut_iu_um_grdavg08,\
				pp->lut_iu_um_grdavg09,pp->lut_iu_um_grdavg10,pp->lut_iu_um_grdavg11,\
				pp->lut_iu_um_grdavg12,pp->lut_iu_um_grdavg13,pp->lut_iu_um_grdavg14,\
				pp->lut_iu_um_grdavg15,pp->lut_id_um_grdavg00,pp->lut_id_um_grdavg01,\
				pp->lut_id_um_grdavg02,pp->lut_id_um_grdavg03,pp->lut_id_um_grdavg04,\
				pp->lut_id_um_grdavg05,pp->lut_id_um_grdavg06,pp->lut_id_um_grdavg07,\
				pp->lut_id_um_grdavg08,pp->lut_id_um_grdavg09,pp->lut_id_um_grdavg10,\
				pp->lut_id_um_grdavg11,pp->lut_id_um_grdavg12,pp->lut_id_um_grdavg13,\
				pp->lut_id_um_grdavg14,pp->lut_id_um_grdavg15,pp->we_jone_base,\
				pp->we_jone_min,pp->we_jone_max);
		}
		else
		{
			PE_SHP_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * set sr scale filter coeffient parameter
 * - use input struct LX_PE_SHP_SR1_T LX_PE_SHP_SR_SCLFILTER_COEFF_T H15AX
 *
 * @param   *pstParams [in] void
 * @return  OK if success, ERROR otherwise.
 * @see		function description
 * @author
 */
int PE_SHP_HW_H15_SetSreSclFlt(void *pstParams)
{
	int ret = RET_OK;
	#ifdef PE_SHP_HW_H15_SR_PWD_TRACE
	UINT32 r_data[2];
	#endif
	do {
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H15)
		{
			LX_PE_SHP_SR_SCLFILTER_COEFF_T *pp=(LX_PE_SHP_SR_SCLFILTER_COEFF_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			PE_SHP_HW_H15_DBG_PRINT("set[%d] : \n"\
				"coeff_0: %d,%d,%d,%d\n"\
				"coeff_1: %d,%d,%d,%d\n", pp->win_id, \
				pp->iu_scale_coeff0_0,pp->iu_scale_coeff0_1,\
				pp->iu_scale_coeff0_2,pp->iu_scale_coeff0_3,\
				pp->iu_scale_coeff1_0,pp->iu_scale_coeff1_1,\
				pp->iu_scale_coeff1_2,pp->iu_scale_coeff1_2);
			if(PE_CHECK_WIN0(pp->win_id))
			{
				#ifdef PE_SHP_HW_H15_SR_PWD_TRACE
				CTOP_CTRL_H15A0_RdFL(SR,ctr00);
				CTOP_CTRL_H15A0_Rd01(SR,ctr00,cg_sr_en,r_data[0]);
				CTOP_CTRL_H15A0_Rd01(SR,ctr00,cg_shs_en,r_data[1]);
				PE_SHP_HW_H15_DBG_SRPD_TRACE("S cg sr,shs:%d,%d\n",r_data[0],r_data[1]);
				#endif
				PE_SR_S1_H15A0_QWr02(core1_scaler_coef0, core1_iu_scale_coeff0_0, GET_BITS(pp->iu_scale_coeff0_0,0,11), \
                                                         core1_iu_scale_coeff0_1, GET_BITS(pp->iu_scale_coeff0_1,0,11));
				PE_SR_S1_H15A0_QWr02(core1_scaler_coef1, core1_iu_scale_coeff0_2, GET_BITS(pp->iu_scale_coeff0_2,0,11), \
                                                         core1_iu_scale_coeff0_3, GET_BITS(pp->iu_scale_coeff0_3,0,11));
				PE_SR_S1_H15A0_QWr02(core1_scaler_coef2, core1_iu_scale_coeff1_0, GET_BITS(pp->iu_scale_coeff1_0,0,11), \
                                                         core1_iu_scale_coeff1_1, GET_BITS(pp->iu_scale_coeff1_1,0,11));
				PE_SR_S1_H15A0_QWr02(core1_scaler_coef3, core1_iu_scale_coeff1_2, GET_BITS(pp->iu_scale_coeff1_2,0,11), \
                                                         core1_iu_scale_coeff1_3, GET_BITS(pp->iu_scale_coeff1_3,0,11));
				#ifdef PE_SHP_HW_H15_SR_PWD_TRACE
				CTOP_CTRL_H15A0_RdFL(SR,ctr00);
				CTOP_CTRL_H15A0_Rd01(SR,ctr00,cg_sr_en,r_data[0]);
				CTOP_CTRL_H15A0_Rd01(SR,ctr00,cg_shs_en,r_data[1]);
				PE_SHP_HW_H15_DBG_SRPD_TRACE("E cg sr,shs:%d,%d\n",r_data[0],r_data[1]);
				#endif
			}
		}
		else
		{
			PE_SHP_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	} while (0);
	return ret;
}
/**
 * get sr scale filter coeffient parameter
 * - use input struct LX_PE_SHP_SR_SCLFILTER_COEFF_T for H15AX
 *
 * @param   *pstParams [in/out] void
 * @return  OK if success, ERROR otherwise.
 * @see		function description
 * @author
 */
int PE_SHP_HW_H15_GetSreSclFlt(void *pstParams)
{
	int ret = RET_OK;
	LX_PE_WIN_ID win_id;
	do {
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H15)
		{
			LX_PE_SHP_SR_SCLFILTER_COEFF_T *pp=(LX_PE_SHP_SR_SCLFILTER_COEFF_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			win_id = PE_GET_CHECKED_WINID(pp->win_id);
			if(PE_CHECK_WIN0(win_id))
			{
				PE_SR_S1_H15A0_QRd02(core1_scaler_coef0, core1_iu_scale_coeff0_0, pp->iu_scale_coeff0_0, \
                                                         core1_iu_scale_coeff0_1, pp->iu_scale_coeff0_1);
				PE_SR_S1_H15A0_QRd02(core1_scaler_coef1, core1_iu_scale_coeff0_2, pp->iu_scale_coeff0_2, \
                                                         core1_iu_scale_coeff0_3, pp->iu_scale_coeff0_3);
				PE_SR_S1_H15A0_QRd02(core1_scaler_coef2, core1_iu_scale_coeff1_0, pp->iu_scale_coeff1_0, \
                                                         core1_iu_scale_coeff1_1, pp->iu_scale_coeff1_1);
				PE_SR_S1_H15A0_QRd02(core1_scaler_coef3, core1_iu_scale_coeff1_2, pp->iu_scale_coeff1_2, \
                                                         core1_iu_scale_coeff1_3, pp->iu_scale_coeff1_3);
			}
			PE_SHP_HW_H15_DBG_PRINT("get[%d] : \n"\
				"coeff_0: %d,%d,%d,%d\n"\
				"coeff_1: %d,%d,%d,%d\n", pp->win_id, \
				pp->iu_scale_coeff0_0,pp->iu_scale_coeff0_1,\
				pp->iu_scale_coeff0_2,pp->iu_scale_coeff0_3,\
				pp->iu_scale_coeff1_0,pp->iu_scale_coeff1_1,\
				pp->iu_scale_coeff1_2,pp->iu_scale_coeff1_2);
		}
		else
		{
			PE_SHP_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * set sr resolution common control
 * - use input struct LX_PE_SHP_SR_RE_CMN_T for H15X
 *
 * @param   *pstParams [in] void
 * @return  OK if success, ERROR otherwise.
 * @see		function description
 * @author
 */
int PE_SHP_HW_H15_SetSreReCmnCtrl(void *pstParams)
{
	int ret = RET_OK;
	#ifdef PE_SHP_HW_H15_SR_PWD_TRACE
	UINT32 r_data[2];
	#endif
	do {
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H15)
		{
			LX_PE_SHP_SR_RE_CMN_T *pp=(LX_PE_SHP_SR_RE_CMN_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			PE_SHP_HW_H15_DBG_PRINT("set[%d] : resolution common ctrl\n"\
				"gain:%d, %d, %d, %d\n"\
				"mode:%d, %d, %d, %d\n"\
				"ti:%d, %d, %d, %d, %d\n"\
				"tgen:%d, %d, %d, %d\n"\
				"tgen_blur x:%d, %d, %d\n"\
				"tgen_blur y:%d, %d, %d\n",\
				pp->win_id, pp->mp_white_gain, pp->mp_black_gain, \
				pp->sp_white_gain, pp->sp_black_gain,\
				pp->mp_lap_h_mode, pp->sp_lap_h_mode,\
				pp->mp_lap_v_mode, pp->sp_lap_v_mode,\
				pp->reg_master_gain_pti_v, pp->reg_master_gain_pti_h, pp->reg_master_gain_sti, \
				pp->reg_edge_gain_sti, pp->reg_texture_gain_sti,\
				pp->tg_map_edge_gain, pp->tg_map_detail_gain, \
				pp->tg_map_offset, pp->tg_gs_noise, \
				pp->tg_blur_coef_x0,pp->tg_blur_coef_x1,pp->tg_blur_coef_x2, \
				pp->tg_blur_coef_y0,pp->tg_blur_coef_y1,pp->tg_blur_coef_y2);
			if(PE_CHECK_WIN0(pp->win_id))
			{
				#ifdef PE_SHP_HW_H15_SR_PWD_TRACE
				CTOP_CTRL_H15A0_RdFL(SR,ctr00);
				CTOP_CTRL_H15A0_Rd01(SR,ctr00,cg_sr_en,r_data[0]);
				CTOP_CTRL_H15A0_Rd01(SR,ctr00,cg_shs_en,r_data[1]);
				PE_SHP_HW_H15_DBG_SRPD_TRACE("S cg sr,shs:%d,%d\n",r_data[0],r_data[1]);
				#endif
				#ifdef PE_SHP_HW_H15_SRE_SHP_WR_ONLY_MD
				PE_SHP_HW_H15A_SR_SHP_WR02(pe1_mp_ctrl_01,white_gain,GET_BITS(pp->mp_white_gain,0,7),\
                                                       black_gain,GET_BITS(pp->mp_black_gain,0,7));
				PE_SHP_HW_H15A_SR_SHP_WR02(pe1_sp_ctrl_01,white_gain,GET_BITS(pp->sp_white_gain,0,7),\
                                                       black_gain,GET_BITS(pp->sp_black_gain,0,7));
				PE_SHP_HW_H15A_SR_SHP_WR02(pe1_mp_ctrl_06,lap_h_mode,GET_BITS(pp->mp_lap_h_mode,0,3),\
                                                       lap_v_mode,GET_BITS(pp->mp_lap_v_mode,0,3));
				#ifdef PE_SHP_HW_H15_PROTECT_SP_LAP_H_MODE_BUG
				pp->sp_lap_h_mode = (pp->sp_lap_h_mode==0x1 || pp->sp_lap_h_mode==0x2)? 0x0:\
									(pp->sp_lap_h_mode==0x4)? 0x3:0x5;
				#endif
				PE_SHP_HW_H15A_SR_SHP_WR02(pe1_sp_ctrl_06,lap_h_mode,GET_BITS(pp->sp_lap_h_mode,0,3),\
                                                       lap_v_mode,GET_BITS(pp->sp_lap_v_mode,0,1));
				PE_SHP_HW_H15A_SR_SHP_WR01(pe1_pti_ctrl_00,reg_master_gain,GET_BITS(pp->reg_master_gain_pti_v,0,8));
				PE_SHP_HW_H15A_SR_SHP_WR01(pe1_pti_ctrl_01,reg_master_gain,GET_BITS(pp->reg_master_gain_pti_h,0,8));
				PE_SHP_HW_H15A_SR_SHP_WR03(pe1_sti_ctrl_00,reg_master_gain,GET_BITS(pp->reg_master_gain_sti,0,8),\
                                                        reg_texture_gain,GET_BITS(pp->reg_texture_gain_sti,0,8),\
                                                        reg_edge_gain,GET_BITS(pp->reg_edge_gain_sti,0,8));
				PE_SHP_HW_H15A_SR_SHP_WR04(pe1_tgen_ctrl_08,tg_map_edge_gain,GET_BITS(pp->tg_map_edge_gain,0,5),\
                                                         tg_map_detail_gain,GET_BITS(pp->tg_map_detail_gain,0,5),\
                                                         tg_map_offset,GET_BITS(pp->tg_map_offset,0,8),\
                                                         tg_gs_noise,GET_BITS(pp->tg_gs_noise,0,6));
				PE_SHP_HW_H15A_SR_SHP_WR03(pe1_tgen_ctrl_06,tg_blur_coef_x0,GET_BITS(pp->tg_blur_coef_x0,0,6),\
                                                         tg_blur_coef_x1,GET_BITS(pp->tg_blur_coef_x1,0,6),\
                                                         tg_blur_coef_x2,GET_BITS(pp->tg_blur_coef_x2,0,6));
				PE_SHP_HW_H15A_SR_SHP_WR03(pe1_tgen_ctrl_07,tg_blur_coef_y0,GET_BITS(pp->tg_blur_coef_y0,0,6),\
                                                         tg_blur_coef_y1,GET_BITS(pp->tg_blur_coef_y1,0,6),\
                                                         tg_blur_coef_y2,GET_BITS(pp->tg_blur_coef_y2,0,6));
				#else	//#ifdef PE_SHP_HW_H15_SRE_SHP_WR_ONLY_MD
				#ifndef PE_HW_H15_PROTECT_OVERLAP_SETTINGS
				PE_SHP_HW_H15A_SR_SHP_WR02(pe1_mp_ctrl_01,white_gain,GET_BITS(pp->mp_white_gain,0,7),\
                                                       black_gain,GET_BITS(pp->mp_black_gain,0,7));
				PE_SHP_HW_H15A_SR_SHP_WR02(pe1_sp_ctrl_01,white_gain,GET_BITS(pp->sp_white_gain,0,7),\
                                                       black_gain,GET_BITS(pp->sp_black_gain,0,7));
				PE_SHP_HW_H15A_SR_SHP_WR02(pe1_mp_ctrl_06,lap_h_mode,GET_BITS(pp->mp_lap_h_mode,0,3),\
                                                       lap_v_mode,GET_BITS(pp->mp_lap_v_mode,0,3));
				#ifdef PE_SHP_HW_H15_PROTECT_SP_LAP_H_MODE_BUG
				pp->sp_lap_h_mode = (pp->sp_lap_h_mode==0x1 || pp->sp_lap_h_mode==0x2)? 0x0:\
									(pp->sp_lap_h_mode==0x4)? 0x3:0x5;
				#endif
				PE_SHP_HW_H15A_SR_SHP_WR02(pe1_sp_ctrl_06,lap_h_mode,GET_BITS(pp->sp_lap_h_mode,0,3),\
                                                       lap_v_mode,GET_BITS(pp->sp_lap_v_mode,0,1));
				PE_SHP_HW_H15A_SR_SHP_WR02(pe1_pti_ctrl_00,reg_master_gain,GET_BITS(pp->reg_master_gain_pti_v,0,8),\
				                                           reg_debug_map_en,0);
				PE_SHP_HW_H15A_SR_SHP_WR02(pe1_pti_ctrl_01,reg_master_gain,GET_BITS(pp->reg_master_gain_pti_h,0,8),\
				                                           reg_debug_map_en,0);
				PE_SHP_HW_H15A_SR_SHP_WR04(pe1_sti_ctrl_00,reg_master_gain,GET_BITS(pp->reg_master_gain_sti,0,8),\
                                                        reg_texture_gain,GET_BITS(pp->reg_texture_gain_sti,0,8),\
                                                        reg_edge_gain,GET_BITS(pp->reg_edge_gain_sti,0,8),\
				                                        reg_debug_map_en,0);
				PE_SHP_HW_H15A_SR_SHP_WR04(pe1_tgen_ctrl_08,tg_map_edge_gain,GET_BITS(pp->tg_map_edge_gain,0,5),\
                                                         tg_map_detail_gain,GET_BITS(pp->tg_map_detail_gain,0,5),\
                                                         tg_map_offset,GET_BITS(pp->tg_map_offset,0,8),\
                                                         tg_gs_noise,GET_BITS(pp->tg_gs_noise,0,6));
				PE_SHP_HW_H15A_SR_SHP_WR03(pe1_tgen_ctrl_06,tg_blur_coef_x0,GET_BITS(pp->tg_blur_coef_x0,0,6),\
                                                         tg_blur_coef_x1,GET_BITS(pp->tg_blur_coef_x1,0,6),\
                                                         tg_blur_coef_x2,GET_BITS(pp->tg_blur_coef_x2,0,6));
				PE_SHP_HW_H15A_SR_SHP_WR03(pe1_tgen_ctrl_07,tg_blur_coef_y0,GET_BITS(pp->tg_blur_coef_y0,0,6),\
                                                         tg_blur_coef_y1,GET_BITS(pp->tg_blur_coef_y1,0,6),\
                                                         tg_blur_coef_y2,GET_BITS(pp->tg_blur_coef_y2,0,6));
				#else
				PE_SR_SHP_H15A0_QWr02(pe1_mp_ctrl_01,white_gain,GET_BITS(pp->mp_white_gain,0,7),\
                                                     black_gain,GET_BITS(pp->mp_black_gain,0,7));
				PE_SR_SHP_H15A0_QWr02(pe1_sp_ctrl_01,white_gain,GET_BITS(pp->sp_white_gain,0,7),\
                                                     black_gain,GET_BITS(pp->sp_black_gain,0,7));
				PE_SR_SHP_H15A0_QWr02(pe1_mp_ctrl_06,lap_h_mode,GET_BITS(pp->mp_lap_h_mode,0,3),\
                                                     lap_v_mode,GET_BITS(pp->mp_lap_v_mode,0,3));
				PE_SR_SHP_H15A0_QWr02(pe1_sp_ctrl_06,lap_h_mode,GET_BITS(pp->sp_lap_h_mode,0,3),\
                                                     lap_v_mode,GET_BITS(pp->sp_lap_v_mode,0,1));
				PE_SR_SHP_H15A0_QWr02(pe1_pti_ctrl_00,reg_master_gain,GET_BITS(pp->reg_master_gain_pti_v,0,8),\
				                                      reg_debug_map_en,0);
				PE_SR_SHP_H15A0_QWr02(pe1_pti_ctrl_01,reg_master_gain,GET_BITS(pp->reg_master_gain_pti_h,0,8),\
				                                      reg_debug_map_en,0);
				PE_SR_SHP_H15A0_QWr04(pe1_sti_ctrl_00,reg_master_gain,GET_BITS(pp->reg_master_gain_sti,0,8),\
                                                      reg_texture_gain,GET_BITS(pp->reg_texture_gain_sti,0,8),\
                                                      reg_edge_gain,GET_BITS(pp->reg_edge_gain_sti,0,8),\
				                                      reg_debug_map_en,0);
				PE_SR_SHP_H15A0_QWr04(pe1_tgen_ctrl_08,tg_map_edge_gain,GET_BITS(pp->tg_map_edge_gain,0,5),\
                                                       tg_map_detail_gain,GET_BITS(pp->tg_map_detail_gain,0,5),\
                                                       tg_map_offset,GET_BITS(pp->tg_map_offset,0,8),\
                                                       tg_gs_noise,GET_BITS(pp->tg_gs_noise,0,6));
				PE_SR_SHP_H15A0_QWr03(pe1_tgen_ctrl_06,  tg_blur_coef_x0,GET_BITS(pp->tg_blur_coef_x0,0,6),\
                                                         tg_blur_coef_x1,GET_BITS(pp->tg_blur_coef_x1,0,6),\
                                                         tg_blur_coef_x2,GET_BITS(pp->tg_blur_coef_x2,0,6));
				PE_SR_SHP_H15A0_QWr03(pe1_tgen_ctrl_07,  tg_blur_coef_y0,GET_BITS(pp->tg_blur_coef_y0,0,6),\
                                                         tg_blur_coef_y1,GET_BITS(pp->tg_blur_coef_y1,0,6),\
                                                         tg_blur_coef_y2,GET_BITS(pp->tg_blur_coef_y2,0,6));
				#endif
				#endif	//#ifdef PE_SHP_HW_H15_SRE_SHP_WR_ONLY_MD
				#ifdef PE_SHP_HW_H15_SR_PWD_TRACE
				CTOP_CTRL_H15A0_RdFL(SR,ctr00);
				CTOP_CTRL_H15A0_Rd01(SR,ctr00,cg_sr_en,r_data[0]);
				CTOP_CTRL_H15A0_Rd01(SR,ctr00,cg_shs_en,r_data[1]);
				PE_SHP_HW_H15_DBG_SRPD_TRACE("E cg sr,shs:%d,%d\n",r_data[0],r_data[1]);
				#endif
			}
		}
		else
		{
			PE_SHP_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	} while (0);
	return ret;
}
/**
 * get sr resolution common control
 * - use input struct LX_PE_SHP_SR_RE_CMN_T for H15X
 *
 * @param   *pstParams [in/out] void
 * @return  OK if success, ERROR otherwise.
 * @see		function description
 * @author
 */
int PE_SHP_HW_H15_GetSreReCmnCtrl(void *pstParams)
{
	int ret = RET_OK;
	LX_PE_WIN_ID win_id;
	do {
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H15)
		{
			LX_PE_SHP_SR_RE_CMN_T *pp=(LX_PE_SHP_SR_RE_CMN_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			win_id = PE_GET_CHECKED_WINID(pp->win_id);
			if(PE_CHECK_WIN0(win_id))
			{
				PE_SR_SHP_H15A0_QRd02(pe1_mp_ctrl_01,white_gain,pp->mp_white_gain,\
                                                   black_gain,pp->mp_black_gain);
				PE_SR_SHP_H15A0_QRd02(pe1_sp_ctrl_01,white_gain,pp->sp_white_gain,\
                                                   black_gain,pp->sp_black_gain);
				PE_SR_SHP_H15A0_QRd02(pe1_mp_ctrl_06,lap_h_mode,pp->mp_lap_h_mode,\
                                                   lap_v_mode,pp->mp_lap_v_mode);
				PE_SR_SHP_H15A0_QRd02(pe1_sp_ctrl_06,lap_h_mode,pp->sp_lap_h_mode,\
                                                   lap_v_mode,pp->sp_lap_v_mode);
				PE_SR_SHP_H15A0_QRd01(pe1_pti_ctrl_00,reg_master_gain, pp->reg_master_gain_pti_v);
				PE_SR_SHP_H15A0_QRd01(pe1_pti_ctrl_01,reg_master_gain, pp->reg_master_gain_pti_h);
				PE_SR_SHP_H15A0_QRd03(pe1_sti_ctrl_00,reg_master_gain, pp->reg_master_gain_sti,\
                                                    reg_texture_gain,pp->reg_texture_gain_sti,\
                                                    reg_edge_gain,   pp->reg_edge_gain_sti);
				PE_SR_SHP_H15A0_QRd04(pe1_tgen_ctrl_08,tg_map_edge_gain,   pp->tg_map_edge_gain,\
                                                     tg_map_detail_gain, pp->tg_map_detail_gain,\
                                                     tg_map_offset,      pp->tg_map_offset,\
                                                     tg_gs_noise,        pp->tg_gs_noise);
				PE_SR_SHP_H15A0_QRd03(pe1_tgen_ctrl_06,tg_blur_coef_x0,pp->tg_blur_coef_x0,\
                                                     tg_blur_coef_x1,pp->tg_blur_coef_x1,\
                                                     tg_blur_coef_x2,pp->tg_blur_coef_x2);
				PE_SR_SHP_H15A0_QRd03(pe1_tgen_ctrl_07,tg_blur_coef_y0,pp->tg_blur_coef_y0,\
                                                     tg_blur_coef_y1,pp->tg_blur_coef_y1,\
                                                     tg_blur_coef_y2,pp->tg_blur_coef_y2);
			}
			PE_SHP_HW_H15_DBG_PRINT("get[%d] : resolution common ctrl\n"\
				"mp/sp gain:%d, %d, %d, %d\n"\
				"mode:%d, %d, %d, %d\n"\
				"ti:%d, %d, %d, %d, %d\n"\
				"tgen:%d, %d, %d, %d\n"\
				"tgen_blur x:%d, %d, %d\n"\
				"tgen_blur y:%d, %d, %d\n",\
				pp->win_id, pp->mp_white_gain, pp->mp_black_gain, \
				pp->sp_white_gain, pp->sp_black_gain,\
				pp->mp_lap_h_mode, pp->sp_lap_h_mode, \
				pp->mp_lap_v_mode, pp->sp_lap_v_mode,\
				pp->reg_master_gain_pti_v, pp->reg_master_gain_pti_h, pp->reg_master_gain_sti, \
				pp->reg_edge_gain_sti, pp->reg_texture_gain_sti,\
				pp->tg_map_edge_gain, pp->tg_map_detail_gain, \
				pp->tg_map_offset, pp->tg_gs_noise, \
				pp->tg_blur_coef_x0,pp->tg_blur_coef_x1,pp->tg_blur_coef_x2, \
				pp->tg_blur_coef_y0,pp->tg_blur_coef_y1,pp->tg_blur_coef_y2);
		}
		else
		{
			PE_SHP_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * set sr resolution horizontal control
 * - use input struct LX_PE_SHP_SR_RE_HOR_T for H15X
 *
 * @param   *pstParams [in/out] void
 * @return  OK if success, ERROR otherwise.
 * @see		function description
 * @author
 */
int PE_SHP_HW_H15_SetSreReHorCtrl(void *pstParams)
{
	int ret = RET_OK;
	#ifdef PE_SHP_HW_H15_SR_PWD_TRACE
	UINT32 r_data[2];
	#endif
	do {
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H15)
		{
			LX_PE_SHP_SR_RE_HOR_T *pp=(LX_PE_SHP_SR_RE_HOR_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			PE_SHP_HW_H15_DBG_PRINT("set[%d] : resolution horizontal ctrl\n"\
				"g:%d,%d,%d, a_gen_w:%d, mp,sp_gain:%d,%d\n"\
				"e_gain(1,2):%d,%d, f_gain(1,2):%d,%d\n",\
				pp->win_id,pp->reg_csft_gain,\
				pp->edge_filter_white_gain,pp->edge_filter_black_gain,\
				pp->a_gen_width,pp->mp_horizontal_gain,pp->sp_horizontal_gain,\
				pp->e_gain_th1, pp->e_gain_th2,pp->f_gain_th1, pp->f_gain_th2);
			if(PE_CHECK_WIN0(pp->win_id))
			{
				#ifdef PE_SHP_HW_H15_SR_PWD_TRACE
				CTOP_CTRL_H15A0_RdFL(SR,ctr00);
				CTOP_CTRL_H15A0_Rd01(SR,ctr00,cg_sr_en,r_data[0]);
				CTOP_CTRL_H15A0_Rd01(SR,ctr00,cg_shs_en,r_data[1]);
				PE_SHP_HW_H15_DBG_SRPD_TRACE("S cg sr,shs:%d,%d\n",r_data[0],r_data[1]);
				#endif
				#if 0
				PE_FWI_H15_HOR_CTRL fw_param;
				fw_param.e_gain_th1 = GET_BITS(pp->e_gain_th1,0,8);
				fw_param.e_gain_th2 = GET_BITS(pp->e_gain_th2,0,8);
				ret = PE_FWI_H15_SetShpHorCtrl(&fw_param);
				PE_SHP_HW_H15_CHECK_CODE(ret, break, \
					"[%s,%d] PE_FWI_H15_SetShpHorCtrl() error.\n", __F__, __L__);
				#endif
				#ifdef PE_SHP_HW_H15_SRE_SHP_WR_ONLY_MD
				PE_SHP_HW_H15A_SR_SHP_WR01(pe1_derh_ctrl_00,a_gen_width,GET_BITS(pp->a_gen_width,0,4));
				PE_SHP_HW_H15A_SR_SHP_WR01(pe1_derh_ctrl_05,reg_csft_gain,GET_BITS(pp->reg_csft_gain,0,6));
				PE_SHP_HW_H15A_SR_SHP_WR02(pe1_derh_ctrl_06,edge_filter_white_gain,GET_BITS(pp->edge_filter_white_gain,0,6),\
                                                         edge_filter_black_gain,GET_BITS(pp->edge_filter_black_gain,0,6));
				PE_SHP_HW_H15A_SR_SHP_WR01(pe1_mp_ctrl_01,horizontal_gain,GET_BITS(pp->mp_horizontal_gain,0,8));
				PE_SHP_HW_H15A_SR_SHP_WR01(pe1_sp_ctrl_01,horizontal_gain,GET_BITS(pp->sp_horizontal_gain,0,8));
				PE_SHP_HW_H15A_SR_SHP_WR02(pe1_derh_ctrl_0b,f_gain_th1,GET_BITS(pp->f_gain_th1,0,8), \
                                                         f_gain_th2,GET_BITS(pp->f_gain_th2,0,8));
				#ifdef PE_SHP_HW_H15A_SET_FW_PARAM_TO_REG_DIRECTLY
				PE_SHP_HW_H15A_SR_SHP_WR02(pe1_derh_ctrl_0b,e_gain_th1,GET_BITS(pp->e_gain_th1,0,8), \
                                                         e_gain_th2,GET_BITS(pp->e_gain_th2,0,8));
				#else
				PE_SHP_HW_H15A_SRS_SET_USER(pe1_derh_ctrl_0b,e_gain_th1,GET_BITS(pp->e_gain_th1,0,8));
				PE_SHP_HW_H15A_SRS_SET_USER(pe1_derh_ctrl_0b,e_gain_th2,GET_BITS(pp->e_gain_th2,0,8));
				#endif
				#else	//#ifdef PE_SHP_HW_H15_SRE_SHP_WR_ONLY_MD
				#ifndef PE_HW_H15_PROTECT_OVERLAP_SETTINGS
				PE_SHP_HW_H15A_SR_SHP_WR01(pe1_derh_ctrl_00,a_gen_width,GET_BITS(pp->a_gen_width,0,4));
				PE_SHP_HW_H15A_SR_SHP_WR01(pe1_derh_ctrl_05,reg_csft_gain,GET_BITS(pp->reg_csft_gain,0,6));
				PE_SHP_HW_H15A_SR_SHP_WR02(pe1_derh_ctrl_06,edge_filter_white_gain,GET_BITS(pp->edge_filter_white_gain,0,6),\
                                                         edge_filter_black_gain,GET_BITS(pp->edge_filter_black_gain,0,6));
				PE_SHP_HW_H15A_SR_SHP_WR01(pe1_mp_ctrl_01,horizontal_gain,GET_BITS(pp->mp_horizontal_gain,0,8));
				PE_SHP_HW_H15A_SR_SHP_WR01(pe1_sp_ctrl_01,horizontal_gain,GET_BITS(pp->sp_horizontal_gain,0,8));
				PE_SHP_HW_H15A_SR_SHP_WR02(pe1_derh_ctrl_0b,f_gain_th1,GET_BITS(pp->f_gain_th1,0,8), \
                                                         f_gain_th2,GET_BITS(pp->f_gain_th2,0,8));
				#ifdef PE_SHP_HW_H15A_SET_FW_PARAM_TO_REG_DIRECTLY
				PE_SHP_HW_H15A_SR_SHP_WR02(pe1_derh_ctrl_0b,e_gain_th1,GET_BITS(pp->e_gain_th1,0,8), \
                                                         e_gain_th2,GET_BITS(pp->e_gain_th2,0,8));
				#else
				PE_SHP_HW_H15A_SRS_SET_USER(pe1_derh_ctrl_0b,e_gain_th1,GET_BITS(pp->e_gain_th1,0,8));
				PE_SHP_HW_H15A_SRS_SET_USER(pe1_derh_ctrl_0b,e_gain_th2,GET_BITS(pp->e_gain_th2,0,8));
				#endif
				#else
				PE_SR_SHP_H15A0_QWr01(pe1_derh_ctrl_00,a_gen_width,  GET_BITS(pp->a_gen_width,0,4));
				PE_SR_SHP_H15A0_QWr01(pe1_derh_ctrl_05,reg_csft_gain,GET_BITS(pp->reg_csft_gain,0,6));
				PE_SR_SHP_H15A0_QWr02(pe1_derh_ctrl_06,edge_filter_white_gain,GET_BITS(pp->edge_filter_white_gain,0,6),\
													edge_filter_black_gain,	GET_BITS(pp->edge_filter_black_gain,0,6));
				PE_SR_SHP_H15A0_QWr01(pe1_mp_ctrl_01,horizontal_gain,GET_BITS(pp->mp_horizontal_gain,0,8));
				PE_SR_SHP_H15A0_QWr01(pe1_sp_ctrl_01,horizontal_gain,GET_BITS(pp->sp_horizontal_gain,0,8));
				PE_SR_SHP_H15A0_QWr02(pe1_derh_ctrl_0b,f_gain_th1,GET_BITS(pp->f_gain_th1,0,8),\
													f_gain_th2,	GET_BITS(pp->f_gain_th2,0,8));
				#ifdef PE_HW_H15_FOR_BRINGUP
				#else
				PE_SR_SHP_H15A0_QWr03(pe1_ti_ctrl_0,coring_th,	GET_BITS(pp->coring_th,0,8),\
												  	y_gain,		GET_BITS(pp->y_gain,0,8),\
												  	c_gain,		GET_BITS(pp->c_gain,0,8));
				#endif
				#ifdef PE_SHP_HW_H15A_SET_FW_PARAM_TO_REG_DIRECTLY
				PE_SR_SHP_H15A0_QWr02(pe1_derh_ctrl_0b,e_gain_th1,GET_BITS(pp->e_gain_th1,0,8), \
                                                       e_gain_th2,GET_BITS(pp->e_gain_th2,0,8));
				#endif
				#endif
				#endif	//#ifdef PE_SHP_HW_H15_SRE_SHP_WR_ONLY_MD
				#ifdef PE_SHP_HW_H15_SR_PWD_TRACE
				CTOP_CTRL_H15A0_RdFL(SR,ctr00);
				CTOP_CTRL_H15A0_Rd01(SR,ctr00,cg_sr_en,r_data[0]);
				CTOP_CTRL_H15A0_Rd01(SR,ctr00,cg_shs_en,r_data[1]);
				PE_SHP_HW_H15_DBG_SRPD_TRACE("E cg sr,shs:%d,%d\n",r_data[0],r_data[1]);
				#endif
			}
		}
		else
		{
			PE_SHP_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	} while (0);
	return ret;
}
/**
 * get sr resolution horizontal control
 * - use input struct LX_PE_SHP_SR_RE_HOR_T for H15X
 *
 * @param   *pstParams [in/out] void
 * @return  OK if success, ERROR otherwise.
 * @see		function description
 * @author
 */
int PE_SHP_HW_H15_GetSreReHorCtrl(void *pstParams)
{
	int ret = RET_OK;
	LX_PE_WIN_ID win_id;
	do {
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H15)
		{
			LX_PE_SHP_SR_RE_HOR_T *pp=(LX_PE_SHP_SR_RE_HOR_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			win_id = PE_GET_CHECKED_WINID(pp->win_id);
			if(PE_CHECK_WIN0(win_id))
			{
				#if 0
				PE_FWI_H15_SETTINGS_T fw_inf;
				ret = PE_FWI_H15_GetCurFwiSettings(&fw_inf);
				PE_SHP_HW_H15_CHECK_CODE(ret, break, \
					"[%s,%d] PE_FWI_H15_GetCurFwiSettings() error.\n", __F__, __L__);
				PE_SHP_HW_H15_DBG_PRINT("fw_ctrl.enable : %s.\n", fw_inf.fw_ctrl.enable? "yes":"no");
				#endif
				#ifdef PE_SHP_HW_H15A_SET_FW_PARAM_TO_REG_DIRECTLY
				PE_SR_SHP_H15A0_QRd02(pe1_derh_ctrl_0b,e_gain_th1,pp->e_gain_th1,\
                                                     e_gain_th2,pp->e_gain_th2);
				#else
				if(fw_inf.fw_ctrl.enable)
				{
					pp->e_gain_th1 = fw_inf.shp_h_ctrl.e_gain_th1;
					pp->e_gain_th2 = fw_inf.shp_h_ctrl.e_gain_th2;
				}
				else
				{
					PE_SR_SHP_H15A0_QRd02(pe1_derh_ctrl_0b, e_gain_th1,	pp->e_gain_th1,\
														 e_gain_th2,	pp->e_gain_th2);
				}
				#endif
				PE_SR_SHP_H15A0_QRd01(pe1_derh_ctrl_00,	a_gen_width,		pp->a_gen_width);
				PE_SR_SHP_H15A0_QRd01(pe1_derh_ctrl_05,	reg_csft_gain,		pp->reg_csft_gain);
				PE_SR_SHP_H15A0_QRd02(pe1_derh_ctrl_06,	edge_filter_white_gain,pp->edge_filter_white_gain,\
													 	edge_filter_black_gain,pp->edge_filter_black_gain);
				PE_SR_SHP_H15A0_QRd01(pe1_mp_ctrl_01, 	horizontal_gain,	pp->mp_horizontal_gain);
				PE_SR_SHP_H15A0_QRd01(pe1_sp_ctrl_01, 	horizontal_gain,	pp->sp_horizontal_gain);
				PE_SR_SHP_H15A0_QRd02(pe1_derh_ctrl_0b, 	f_gain_th1,			pp->f_gain_th1,\
													 	f_gain_th2,			pp->f_gain_th2);
				#ifndef PE_HW_H15_FOR_BRINGUP
				PE_SR_SHP_H15A0_QRd03(pe1_ti_ctrl_0, 	coring_th,		pp->coring_th,\
												  	y_gain,			pp->y_gain,\
												  	c_gain,			pp->c_gain);
				#endif
			}
			PE_SHP_HW_H15_DBG_PRINT("get[%d] : resolution horizontal ctrl\n"\
				" g:%d,%d,%d, a_gen_w:%d, mp,sp_gain:%d,%d\n"\
				"e_gain(1,2):%d,%d, f_gain(1,2):%d,%d\n",\
				pp->win_id,pp->reg_csft_gain,\
				pp->edge_filter_white_gain,pp->edge_filter_black_gain,\
				pp->a_gen_width,pp->mp_horizontal_gain,pp->sp_horizontal_gain,\
				pp->e_gain_th1, pp->e_gain_th2,pp->f_gain_th1, pp->f_gain_th2);
		}
		else
		{
			PE_SHP_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * set sr resolution vertical control
 * - use input struct LX_PE_SHP_SR_RE_VER_T for H15X
 *
 * @param   *pstParams [in] void
 * @return  OK if success, ERROR otherwise.
 * @see		function description
 * @author
 */
int PE_SHP_HW_H15_SetSreReVerCtrl(void *pstParams)
{
	int ret = RET_OK;
	#ifdef PE_SHP_HW_H15_SR_PWD_TRACE
	UINT32 r_data[2];
	#endif
	do {
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H15)
		{
			LX_PE_SHP_SR_RE_VER_T *pp=(LX_PE_SHP_SR_RE_VER_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			PE_SHP_HW_H15_DBG_PRINT("set[%d] : resolution vertical ctrl\n"\
				" manual_th:%d,csft_gain:%d, gain(b,w)%d,%d,\n"\
				"mmd_sel:%d, gain(th1,th2):%d,%d mp,sp_g:%d,%d\n",\
				pp->win_id,pp->bif_manual_th,pp->csft_gain,\
				pp->gain_b,pp->gain_w,pp->mmd_sel,pp->gain_th1,pp->gain_th2,\
				pp->mp_vertical_gain,pp->sp_vertical_gain);
			if(PE_CHECK_WIN0(pp->win_id))
			{
				#ifdef PE_SHP_HW_H15_SR_PWD_TRACE
				CTOP_CTRL_H15A0_RdFL(SR,ctr00);
				CTOP_CTRL_H15A0_Rd01(SR,ctr00,cg_sr_en,r_data[0]);
				CTOP_CTRL_H15A0_Rd01(SR,ctr00,cg_shs_en,r_data[1]);
				PE_SHP_HW_H15_DBG_SRPD_TRACE("S cg sr,shs:%d,%d\n",r_data[0],r_data[1]);
				#endif
				#ifdef PE_SHP_HW_H15_SRE_SHP_WR_ONLY_MD
				PE_SHP_HW_H15A_SR_SHP_WR01(pe1_derv_ctrl_0,bif_manual_th,GET_BITS(pp->bif_manual_th,0,8));
				PE_SHP_HW_H15A_SR_SHP_WR01(pe1_derv_ctrl_1,csft_gain,GET_BITS(pp->csft_gain,0,6));
				PE_SHP_HW_H15A_SR_SHP_WR02(pe1_derv_ctrl_3,gain_b,GET_BITS(pp->gain_b,0,7),\
                                                        gain_w,GET_BITS(pp->gain_w,0,7));
				PE_SHP_HW_H15A_SR_SHP_WR03(pe1_derv_ctrl_2,mmd_sel,GET_BITS(pp->mmd_sel,0,3),\
                                                        gain_th1,GET_BITS(pp->gain_th1,0,8),\
                                                        gain_th2,GET_BITS(pp->gain_th2,0,8));
				PE_SHP_HW_H15A_SR_SHP_WR01(pe1_mp_ctrl_01,vertical_gain,GET_BITS(pp->mp_vertical_gain,0,8));
				PE_SHP_HW_H15A_SR_SHP_WR01(pe1_sp_ctrl_01,vertical_gain,GET_BITS(pp->sp_vertical_gain,0,8));
				#else	//#ifdef PE_SHP_HW_H15_SRE_SHP_WR_ONLY_MD
				#ifndef PE_HW_H15_PROTECT_OVERLAP_SETTINGS
				PE_SHP_HW_H15A_SR_SHP_WR01(pe1_derv_ctrl_0,bif_manual_th,GET_BITS(pp->bif_manual_th,0,8));
				PE_SHP_HW_H15A_SR_SHP_WR01(pe1_derv_ctrl_1,csft_gain,GET_BITS(pp->csft_gain,0,6));
				PE_SHP_HW_H15A_SR_SHP_WR02(pe1_derv_ctrl_3,gain_b,GET_BITS(pp->gain_b,0,7),\
                                                        gain_w,GET_BITS(pp->gain_w,0,7));
				PE_SHP_HW_H15A_SR_SHP_WR03(pe1_derv_ctrl_2,mmd_sel,GET_BITS(pp->mmd_sel,0,3),\
                                                        gain_th1,GET_BITS(pp->gain_th1,0,8),\
                                                        gain_th2,GET_BITS(pp->gain_th2,0,8));
				PE_SHP_HW_H15A_SR_SHP_WR01(pe1_mp_ctrl_01,vertical_gain,GET_BITS(pp->mp_vertical_gain,0,8));
				PE_SHP_HW_H15A_SR_SHP_WR01(pe1_sp_ctrl_01,vertical_gain,GET_BITS(pp->sp_vertical_gain,0,8));
				#else
				PE_SR_SHP_H15A0_QWr01(pe1_derv_ctrl_0,	bif_manual_th,	GET_BITS(pp->bif_manual_th,0,8));
				PE_SR_SHP_H15A0_QWr01(pe1_derv_ctrl_1,	csft_gain,		GET_BITS(pp->csft_gain,0,6));
				PE_SR_SHP_H15A0_QWr02(pe1_derv_ctrl_3,	gain_b,		GET_BITS(pp->gain_b,0,6),\
														gain_w,		GET_BITS(pp->gain_w,0,6));
				PE_SR_SHP_H15A0_QWr03(pe1_derv_ctrl_2,	mmd_sel,	GET_BITS(pp->mmd_sel,0,3),\
														gain_th1,	GET_BITS(pp->gain_th1,0,8),\
														gain_th2,	GET_BITS(pp->gain_th2,0,8));
				PE_SR_SHP_H15A0_QWr01(pe1_mp_ctrl_01,	vertical_gain,	GET_BITS(pp->mp_vertical_gain,0,8));
				PE_SR_SHP_H15A0_QWr01(pe1_sp_ctrl_01,	vertical_gain,	GET_BITS(pp->sp_vertical_gain,0,8));
				#endif
				#endif	//#ifdef PE_SHP_HW_H15_SRE_SHP_WR_ONLY_MD
				#ifdef PE_SHP_HW_H15_SR_PWD_TRACE
				CTOP_CTRL_H15A0_RdFL(SR,ctr00);
				CTOP_CTRL_H15A0_Rd01(SR,ctr00,cg_sr_en,r_data[0]);
				CTOP_CTRL_H15A0_Rd01(SR,ctr00,cg_shs_en,r_data[1]);
				PE_SHP_HW_H15_DBG_SRPD_TRACE("E cg sr,shs:%d,%d\n",r_data[0],r_data[1]);
				#endif
			}
		}
		else
		{
			PE_SHP_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	} while (0);
	return ret;
}
/**
 * get sr resolution vertical control
 * - use input struct LX_PE_SHP_SR_RE_VER_T for H15X
 *
 * @param   *pstParams [in/out] void
 * @return  OK if success, ERROR otherwise.
 * @see		function description
 * @author
 */
int PE_SHP_HW_H15_GetSreReVerCtrl(void *pstParams)
{
	int ret = RET_OK;
	LX_PE_WIN_ID win_id;
	do {
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H15)
		{
			LX_PE_SHP_SR_RE_VER_T *pp=(LX_PE_SHP_SR_RE_VER_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			win_id = PE_GET_CHECKED_WINID(pp->win_id);
			if(PE_CHECK_WIN0(win_id))
			{
				PE_SR_SHP_H15A0_QRd01(pe1_derv_ctrl_0, bif_manual_th,	pp->bif_manual_th);
				PE_SR_SHP_H15A0_QRd01(pe1_derv_ctrl_1, csft_gain,		pp->csft_gain);
				PE_SR_SHP_H15A0_QRd02(pe1_derv_ctrl_3, gain_b,		pp->gain_b,\
													 gain_w,		pp->gain_w);
				PE_SR_SHP_H15A0_QRd03(pe1_derv_ctrl_2, mmd_sel,		pp->mmd_sel,\
													 gain_th1,		pp->gain_th1,\
													 gain_th2,		pp->gain_th2);
				PE_SR_SHP_H15A0_QRd01(pe1_mp_ctrl_01, vertical_gain,	pp->mp_vertical_gain);
				PE_SR_SHP_H15A0_QRd01(pe1_sp_ctrl_01, vertical_gain,	pp->sp_vertical_gain);
			}
			PE_SHP_HW_H15_DBG_PRINT("get[%d] : resolution vertical ctrl\n"\
				" manual_th:%d,csft_gain:%d, gain(b,w)%d,%d,\n"\
				"mmd_sel:%d, gain(th1,th2):%d,%d mp,sp_g:%d,%d\n",\
				pp->win_id,pp->bif_manual_th,pp->csft_gain,\
				pp->gain_b,pp->gain_w,pp->mmd_sel,pp->gain_th1,pp->gain_th2,\
				pp->mp_vertical_gain,pp->sp_vertical_gain);
		}
		else
		{
			PE_SHP_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * set sr resolution misc control
 * - use input struct LX_PE_SHP_SR_RE_MISC_T for H15X
 *
 * @param   *pstParams [in] void
 * @return  OK if success, ERROR otherwise.
 * @see		function description
 * @author
 */
int PE_SHP_HW_H15_SetSreReMiscCtrl(void *pstParams)
{
	int ret = RET_OK;
	#ifdef PE_SHP_HW_H15_SR_PWD_TRACE
	UINT32 r_data[2];
	#endif
	do {
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H15)
		{
			LX_PE_SHP_SR_RE_MISC_T *pp=(LX_PE_SHP_SR_RE_MISC_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			PE_SHP_HW_H15_DBG_PRINT("set[%d] :  resolution misc ctrl\n"\
				" hor : 0x%02x,0x%02x,0x%02x\n"\
				" ver : 0x%02x,0x%02x,0x%02x\n"\
				" cmn : 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" djg : 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" djg : 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" emp : 0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" emp : 0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" emp : 0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" emp : 0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" emp : 0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" tmp : 0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" tmp : 0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" tmp : 0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" tmp : 0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" blc : 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" blc : 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" blc : 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" blc : 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" blc : 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" blc : 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" ptiv: 0x%02x,0x%02x,0x%02x\n"\
				" ptih: 0x%02x,0x%02x,0x%02x\n"\
				" sti : 0x%02x,0x%02x,0x%02x\n"\
				" tgen: 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" tgen: 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" tgen: 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" tgen: 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n",\
				pp->win_id, \
				/* h 3 */
				pp->edge_Y_filter_en, pp->e_gain_max, pp->f_gain_max, \
				/* v 3 */
				pp->der_v_en, pp->der_gain_mapping, pp->max_sel, \
				/* cmm 5 */
				pp->mp_laplacian_weight,pp->sp_laplacian_weight, \
				pp->flat_filter_en,pp->flat_filter_type,pp->flat_filter_gain,\
				/*djeg 12*/
				pp->edf_en,pp->center_blur_mode,pp->count_diff_th,pp->n_avg_mode,\
				pp->line_variation_diff_threshold,\
				pp->level_th,pp->protect_th,pp->n_avg_gain,\
				pp->reg_g0_cnt_min,pp->reg_g0_mul,pp->reg_g1_protect_min,pp->reg_g1_mul,\
				/*amap 20*/
				pp->reg_cross_th_sc,pp->reg_cross_th,pp->reg_mul_base_t,pp->reg_mul_base_e,\
				pp->reg_a_map_h_size,pp->reg_a_nomalize_en,pp->reg_edge_alpha_mul	,pp->reg_edge_beta_mul,\
				pp->reg_a_lut_edge_x0,pp->reg_a_lut_edge_y0,pp->reg_a_lut_edge_x1,pp->reg_a_lut_edge_y1,\
				pp->reg_a_lut_detail_x0,pp->reg_a_lut_detail_y0,pp->reg_a_lut_detail_x1,pp->reg_a_lut_detail_y1,\
				pp->reg_edge_minmax_x0,pp->reg_edge_minmax_y0,pp->reg_edge_minmax_x1,pp->reg_edge_minmax_y1,\
				/* tmap 16*/
				pp->reg_t_cut_resolution,pp->reg_t_nomalize_en,pp->reg_detail_alpha_mul,pp->reg_detail_beta_mul,\
				pp->reg_t_lut_edge_x0,pp->reg_t_lut_edge_y0,pp->reg_t_lut_edge_x1,pp->reg_t_lut_edge_y1,\
				pp->reg_t_lut_detail_x0,pp->reg_t_lut_detail_y0,pp->reg_t_lut_detail_x1,pp->reg_t_lut_detail_y1,\
				pp->reg_detail_minmax_x0,pp->reg_detail_minmax_y0,pp->reg_detail_minmax_x1,pp->reg_detail_minmax_y1,\
				/* balnce 42*/
				pp->mp_gb_en,pp->mp_gb_mode,\
				pp->mp_gb_x1,pp->mp_gb_x2,pp->mp_gb_y1,pp->mp_gb_y2,pp->mp_gb_y3,\
				pp->sp_gb_en,pp->sp_gb_mode,\
				pp->sp_gb_x1,pp->sp_gb_x2,pp->sp_gb_y1,pp->sp_gb_y2,pp->sp_gb_y3,\
				pp->mp_lum1_x_L0,pp->mp_lum1_x_L1,pp->mp_lum1_x_H0,pp->mp_lum1_x_H1,\
				pp->mp_lum1_y0,pp->mp_lum1_y1,pp->mp_lum1_y2,\
				pp->mp_lum2_x_L0,pp->mp_lum2_x_L1,pp->mp_lum2_x_H0,pp->mp_lum2_x_H1,\
				pp->mp_lum2_y0,pp->mp_lum2_y1,pp->mp_lum2_y2,\
				pp->sp_lum1_x_L0,pp->sp_lum1_x_L1,pp->sp_lum1_x_H0,pp->sp_lum1_x_H1,\
				pp->sp_lum1_y0,pp->sp_lum1_y1,pp->sp_lum1_y2,\
				pp->sp_lum2_x_L0,pp->sp_lum2_x_L1,pp->sp_lum2_x_H0,pp->sp_lum2_x_H1,\
				pp->sp_lum2_y0,pp->sp_lum2_y1,pp->sp_lum2_y2,\
				/* ti 9*/
				pp->reg_ti_enable_pti_v,pp->reg_mm_tap_size_pti_v,pp->reg_avg_tap_size_pti_v,\
				pp->reg_ti_enable_pti_h,pp->reg_mm_tap_size_pti_h,pp->reg_avg_tap_size_pti_h,\
				pp->reg_enable_sti,pp->reg_mm_tap_size_sti,pp->reg_avg_tap_size_sti,\
				/* tgen 23*/
				pp->tgen_on_off,pp->tg_delta_H_bound,pp->tg_delta_L_bound,pp->tg_delta_max,pp->tg_delta_scale,\
				pp->tg_rnd_th,pp->tg_delta_wcurr,pp->tg_lc_ldr_th,\
				pp->tg_dr_ratio,pp->tg_dr_th,pp->tg_mm_offset,pp->tg_mm_sel_mode,\
				pp->tg_lavg_th,pp->tg_gs_rnd,pp->tg_gs_mm,\
				pp->tg_gain0,pp->tg_gain1,pp->tg_gain2,pp->tg_gain3,\
				pp->tg_gain4,pp->tg_gain5,pp->tg_gain6,pp->tg_gain7);
			if(PE_CHECK_WIN0(pp->win_id))
			{
				#ifdef PE_SHP_HW_H15_SR_PWD_TRACE
				CTOP_CTRL_H15A0_RdFL(SR,ctr00);
				CTOP_CTRL_H15A0_Rd01(SR,ctr00,cg_sr_en,r_data[0]);
				CTOP_CTRL_H15A0_Rd01(SR,ctr00,cg_shs_en,r_data[1]);
				PE_SHP_HW_H15_DBG_SRPD_TRACE("S cg sr,shs:%d,%d\n",r_data[0],r_data[1]);
				#endif
				#if 0
				PE_FWI_H15_MISC_CTRL fw_param;
				fw_param.gt_th1 = GET_BITS(pp->gt_th1,0,8);
				fw_param.gt_th0 = GET_BITS(pp->gt_th0,0,8);
				fw_param.gt_th0a = GET_BITS(pp->gt_th0a,0,8);
				fw_param.gt_th0b = GET_BITS(pp->gt_th0b,0,8);
				ret = PE_FWI_H15_SetShpMiscCtrl(&fw_param);
				PE_SHP_HW_H15_CHECK_CODE(ret, break, \
					"[%s,%d] PE_FWI_H15_SetShpMiscCtrl() error.\n", __F__, __L__);
				#endif
				#ifdef PE_SHP_HW_H15_SRE_SHP_WR_ONLY_MD
				/* h 3 */
				PE_SHP_HW_H15A_SR_SHP_WR01(pe1_derh_ctrl_06,edge_y_filter_en,	GET_BITS(pp->edge_Y_filter_en,0,1));
				PE_SHP_HW_H15A_SR_SHP_WR02(pe1_derh_ctrl_0c,e_gain_max,		GET_BITS(pp->e_gain_max,0,6),\
														f_gain_max,			GET_BITS(pp->f_gain_max,0,6));
				/* v 3 */
				PE_SHP_HW_H15A_SR_SHP_WR02(pe1_derv_ctrl_0,der_v_en,			GET_BITS(pp->der_v_en,0,1),\
														der_gain_mapping,	GET_BITS(pp->der_gain_mapping,0,2));
				PE_SHP_HW_H15A_SR_SHP_WR01(pe1_derv_ctrl_2,max_sel,			GET_BITS(pp->max_sel,0,3));
				/* cmm 6 */
				PE_SHP_HW_H15A_SR_SHP_WR01(pe1_mp_ctrl_02,	laplacian_weight,	GET_BITS(pp->mp_laplacian_weight,0,8));
				PE_SHP_HW_H15A_SR_SHP_WR01(pe1_sp_ctrl_02,	laplacian_weight,	GET_BITS(pp->sp_laplacian_weight,0,8));
				PE_SHP_HW_H15A_SR_SHP_WR03(pe1_derh_ctrl_07,flat_filter_en,	GET_BITS(pp->flat_filter_en,0,1),\
														flat_filter_gain,	GET_BITS(pp->flat_filter_gain,0,6),\
														flat_filter_type,	GET_BITS(pp->flat_filter_type,0,1));
				/*d_jag 12 */
				PE_SHP_HW_H15A_SR_SHP_WR05(pe1_dj_ctrl_00,	edf_en,				GET_BITS(pp->edf_en,0,1),\
														center_blur_mode, 	GET_BITS(pp->center_blur_mode,0,1),\
														count_diff_th, 		GET_BITS(pp->count_diff_th,0,5),\
														n_avg_mode,			GET_BITS(pp->n_avg_mode,0,1),\
														line_variation_diff_threshold,GET_BITS(pp->line_variation_diff_threshold,0,8));
				PE_SHP_HW_H15A_SR_SHP_WR03(pe1_dj_ctrl_01,	level_th,			GET_BITS(pp->level_th,0,8),\
														protect_th,			GET_BITS(pp->protect_th,0,8),\
														n_avg_gain,			GET_BITS(pp->n_avg_gain,0,8));
				PE_SHP_HW_H15A_SR_SHP_WR02(pe1_dj_ctrl_02,	reg_g0_cnt_min,		GET_BITS(pp->reg_g0_cnt_min,0,5),\
														reg_g0_mul,			GET_BITS(pp->reg_g0_mul,0,5));
				PE_SHP_HW_H15A_SR_SHP_WR02(pe1_dj_ctrl_03,	reg_g1_protect_min,	GET_BITS(pp->reg_g1_protect_min,0,8),\
														reg_g1_mul, 		GET_BITS(pp->reg_g1_mul,0,7));				
				/*e_map 36 */
				PE_SHP_HW_H15A_SR_SHP_WR02(pe1_coring_ctrl_00, reg_cross_th_sc,	GET_BITS(pp->reg_cross_th_sc,0,8),\
														    reg_cross_th,		GET_BITS(pp->reg_cross_th,0,8));
				PE_SHP_HW_H15A_SR_SHP_WR02(pe1_coring_ctrl_08, reg_mul_base_t,		GET_BITS(pp->reg_mul_base_t,0,8),\
														    reg_mul_base_e,		GET_BITS(pp->reg_mul_base_e,0,8));
				PE_SHP_HW_H15A_SR_SHP_WR02(pe1_coring_ctrl_00, reg_a_map_h_size,	GET_BITS(pp->reg_a_map_h_size,0,8),\
														    reg_a_nomalize_en,	GET_BITS(pp->reg_a_nomalize_en,0,8));
				PE_SHP_HW_H15A_SR_SHP_WR02(pe1_coring_ctrl_01, reg_edge_alpha_mul,	GET_BITS(pp->reg_edge_alpha_mul,0,8),\
														    reg_edge_beta_mul,	GET_BITS(pp->reg_edge_beta_mul,0,8));
				PE_SHP_HW_H15A_SR_SHP_WR04(pe1_coring_ctrl_02,	reg_a_lut_edge_x0,		GET_BITS(pp->reg_a_lut_edge_x0,0,8),\
															reg_a_lut_edge_y0,		GET_BITS(pp->reg_a_lut_edge_y0,0,8),\
															reg_a_lut_edge_x1,		GET_BITS(pp->reg_a_lut_edge_x1,0,8),\
															reg_a_lut_edge_y1,		GET_BITS(pp->reg_a_lut_edge_y1,0,8));
				PE_SHP_HW_H15A_SR_SHP_WR04(pe1_coring_ctrl_03,	reg_a_lut_detail_x0,	GET_BITS(pp->reg_a_lut_detail_x0,0,8),\
															reg_a_lut_detail_y0,	GET_BITS(pp->reg_a_lut_detail_y0,0,8),\
															reg_a_lut_detail_x1,	GET_BITS(pp->reg_a_lut_detail_x1,0,8),\
															reg_a_lut_detail_y1,	GET_BITS(pp->reg_a_lut_detail_y1,0,8));
				PE_SHP_HW_H15A_SR_SHP_WR04(pe1_coring_ctrl_06, reg_edge_minmax_x0,		GET_BITS(pp->reg_edge_minmax_x0,0,8),\
															reg_edge_minmax_y0,		GET_BITS(pp->reg_edge_minmax_y0,0,8),\
															reg_edge_minmax_x1,		GET_BITS(pp->reg_edge_minmax_x1,0,8),\
															reg_edge_minmax_y1,		GET_BITS(pp->reg_edge_minmax_y1,0,8));
				PE_SHP_HW_H15A_SR_SHP_WR02(pe1_coring_ctrl_00, reg_t_cut_resolution,	GET_BITS(pp->reg_t_cut_resolution,0,2),\
														    reg_t_nomalize_en,		GET_BITS(pp->reg_t_nomalize_en,0,1));
				PE_SHP_HW_H15A_SR_SHP_WR02(pe1_coring_ctrl_01, reg_detail_alpha_mul,	GET_BITS(pp->reg_detail_alpha_mul,0,8),\
														    reg_detail_beta_mul,	GET_BITS(pp->reg_detail_beta_mul,0,8));
				PE_SHP_HW_H15A_SR_SHP_WR04(pe1_coring_ctrl_04, reg_t_lut_edge_x0,		GET_BITS(pp->reg_t_lut_edge_x0,0,8),\
															reg_t_lut_edge_y0,		GET_BITS(pp->reg_t_lut_edge_y0,0,8),\
															reg_t_lut_edge_x1,		GET_BITS(pp->reg_t_lut_edge_x1,0,8),\
															reg_t_lut_edge_y1,		GET_BITS(pp->reg_t_lut_edge_y1,0,8));
				PE_SHP_HW_H15A_SR_SHP_WR04(pe1_coring_ctrl_05, reg_t_lut_detail_x0,	GET_BITS(pp->reg_t_lut_detail_x0,0,8),\
															reg_t_lut_detail_y0,	GET_BITS(pp->reg_t_lut_detail_y0,0,8),\
															reg_t_lut_detail_x1,	GET_BITS(pp->reg_t_lut_detail_x1,0,8),\
															reg_t_lut_detail_y1,	GET_BITS(pp->reg_t_lut_detail_y1,0,8));
				PE_SHP_HW_H15A_SR_SHP_WR04(pe1_coring_ctrl_07, reg_detail_minmax_x0,	GET_BITS(pp->reg_detail_minmax_x0,0,8),\
															reg_detail_minmax_y0,	GET_BITS(pp->reg_detail_minmax_y0,0,8),\
															reg_detail_minmax_x1,	GET_BITS(pp->reg_detail_minmax_x1,0,8),\
															reg_detail_minmax_y1,	GET_BITS(pp->reg_detail_minmax_y1,0,8));
				/* blc*/
				PE_SHP_HW_H15A_SR_SHP_WR04(pe1_mp_ctrl_07, gb_en,		GET_BITS(pp->mp_gb_en,0,1),\
														gb_mode,	GET_BITS(pp->mp_gb_mode,0,1),\
														gb_x1,		GET_BITS(pp->mp_gb_x1,0,8),\
														gb_y1,		GET_BITS(pp->mp_gb_y1,0,8));
				PE_SHP_HW_H15A_SR_SHP_WR03(pe1_mp_ctrl_08 ,gb_x2,		GET_BITS(pp->mp_gb_x2,0,8),\
														gb_y2, 		GET_BITS(pp->mp_gb_y2,0,8),\
														gb_y3,		GET_BITS(pp->mp_gb_y3,0,8));
				PE_SHP_HW_H15A_SR_SHP_WR04(pe1_mp_ctrl_09, lum1_x_l0,	GET_BITS(pp->mp_lum1_x_L0,0,8),\
														lum1_x_l1,	GET_BITS(pp->mp_lum1_x_L1,0,8),\
														lum1_x_h0,	GET_BITS(pp->mp_lum1_x_H0,0,8),\
														lum1_x_h1,	GET_BITS(pp->mp_lum1_x_H1,0,8));
				PE_SHP_HW_H15A_SR_SHP_WR04(pe1_mp_ctrl_0a, lum1_y0,	GET_BITS(pp->mp_lum1_y0,0,8),\
														lum1_y1,	GET_BITS(pp->mp_lum1_y1,0,8),\
														lum1_y2,	GET_BITS(pp->mp_lum1_y2,0,8),\
														lum2_x_l0,	GET_BITS(pp->mp_lum2_x_L0,0,8));
				PE_SHP_HW_H15A_SR_SHP_WR04(pe1_mp_ctrl_0b, lum2_x_l1,	GET_BITS(pp->mp_lum2_x_L1,0,8),\
														lum2_x_h0,	GET_BITS(pp->mp_lum2_x_H0,0,8),\
														lum2_x_h1,	GET_BITS(pp->mp_lum2_x_H1,0,8),\
														lum2_y0,	GET_BITS(pp->mp_lum2_y0,0,8));
				PE_SHP_HW_H15A_SR_SHP_WR02(pe1_mp_ctrl_0c, lum2_y1,	GET_BITS(pp->mp_lum2_y1,0,8),\
														lum2_y2,	GET_BITS(pp->mp_lum2_y2,0,8));
				PE_SHP_HW_H15A_SR_SHP_WR04(pe1_sp_ctrl_07, gb_en,		GET_BITS(pp->sp_gb_en,0,1),\
														gb_mode,	GET_BITS(pp->sp_gb_mode,0,1),\
														gb_x1,		GET_BITS(pp->sp_gb_x1,0,8),\
														gb_y1,		GET_BITS(pp->sp_gb_y1,0,8));
				PE_SHP_HW_H15A_SR_SHP_WR03(pe1_sp_ctrl_08, gb_x2,		GET_BITS(pp->sp_gb_x2,0,8),\
														gb_y2, 		GET_BITS(pp->sp_gb_y2,0,8),\
														gb_y3,		GET_BITS(pp->sp_gb_y3,0,8));
				PE_SHP_HW_H15A_SR_SHP_WR04(pe1_sp_ctrl_09, lum1_x_l0,	GET_BITS(pp->sp_lum1_x_L0,0,8),\
														lum1_x_l1,	GET_BITS(pp->sp_lum1_x_L1,0,8),\
														lum1_x_h0,	GET_BITS(pp->sp_lum1_x_H0,0,8),\
														lum1_x_h1,	GET_BITS(pp->sp_lum1_x_H1,0,8));
				PE_SHP_HW_H15A_SR_SHP_WR04(pe1_sp_ctrl_0a, lum1_y0,	GET_BITS(pp->sp_lum1_y0,0,8),\
														lum1_y1,	GET_BITS(pp->sp_lum1_y1,0,8),\
														lum1_y2,	GET_BITS(pp->sp_lum1_y2,0,8),\
														lum2_x_l0,	GET_BITS(pp->sp_lum2_x_L0,0,8));
				PE_SHP_HW_H15A_SR_SHP_WR04(pe1_sp_ctrl_0b, lum2_x_l1,	GET_BITS(pp->sp_lum2_x_L1,0,8),\
														lum2_x_h0,	GET_BITS(pp->sp_lum2_x_H0,0,8),\
														lum2_x_h1,	GET_BITS(pp->sp_lum2_x_H1,0,8),\
														lum2_y0,	GET_BITS(pp->sp_lum2_y0,0,8));
				PE_SHP_HW_H15A_SR_SHP_WR02(pe1_sp_ctrl_0c, lum2_y1,	GET_BITS(pp->sp_lum2_y1,0,8),\
														lum2_y2,	GET_BITS(pp->sp_lum2_y2,0,8));
				/* ti*/
				PE_SHP_HW_H15A_SR_SHP_WR03(pe1_pti_ctrl_00,reg_ti_enable,	GET_BITS(pp->reg_ti_enable_pti_v,0,1),\
				                                         reg_mm_tap_size, 	GET_BITS(pp->reg_mm_tap_size_pti_v,0,2),\
				                                         reg_avg_tap_size,	GET_BITS(pp->reg_avg_tap_size_pti_v,0,3));
				PE_SHP_HW_H15A_SR_SHP_WR03(pe1_pti_ctrl_01,reg_ti_enable,	GET_BITS(pp->reg_ti_enable_pti_h,0,1),\
				                                         reg_mm_tap_size, 	GET_BITS(pp->reg_mm_tap_size_pti_h,0,2),\
				                                         reg_avg_tap_size,	GET_BITS(pp->reg_avg_tap_size_pti_h,0,3));
				PE_SHP_HW_H15A_SR_SHP_WR03(pe1_sti_ctrl_00,reg_ti_enable,	GET_BITS(pp->reg_enable_sti,0,1),\
				                                         reg_mm_tap_size, 	GET_BITS(pp->reg_mm_tap_size_sti,0,2),\
				                                         reg_avg_tap_size,	GET_BITS(pp->reg_avg_tap_size_sti,0,3));
				/* tgn*/
				PE_SHP_HW_H15A_SR_SHP_WR01(pe1_tgen_ctrl_00, tgen_on_off,		GET_BITS(pp->tgen_on_off,0,1));
				PE_SHP_HW_H15A_SR_SHP_WR04(pe1_tgen_ctrl_01, tg_delta_h_bound,	GET_BITS(pp->tg_delta_H_bound,0,5),\
														  tg_delta_l_bound,	GET_BITS(pp->tg_delta_L_bound,0,4),\
														  tg_delta_max,		GET_BITS(pp->tg_delta_max,0,7),\
														  tg_delta_scale,	GET_BITS(pp->tg_delta_scale,0,3));
				PE_SHP_HW_H15A_SR_SHP_WR03(pe1_tgen_ctrl_02, tg_rnd_th,		GET_BITS(pp->tg_rnd_th,0,8),\
														  tg_delta_wcurr, 	GET_BITS(pp->tg_delta_wcurr,0,4),\
														  tg_lc_ldr_th,		GET_BITS(pp->tg_lc_ldr_th,0,5));
				PE_SHP_HW_H15A_SR_SHP_WR04(pe1_tgen_ctrl_04, tg_dr_ratio,		GET_BITS(pp->tg_dr_ratio,0,3),\
														  tg_dr_th, 		GET_BITS(pp->tg_dr_th,0,6),\
														  tg_mm_offset,		GET_BITS(pp->tg_mm_offset,0,5),\
														  tg_mm_sel_mode,	GET_BITS(pp->tg_mm_sel_mode,0,2));
				PE_SHP_HW_H15A_SR_SHP_WR03(pe1_tgen_ctrl_05, tg_lavg_th,		GET_BITS(pp->tg_lavg_th,0,10),\
														  tg_gs_rnd,	 	GET_BITS(pp->tg_gs_rnd,0,5),\
														  tg_gs_mm,			GET_BITS(pp->tg_gs_mm,0,5));
				PE_SHP_HW_H15A_SR_SHP_WR04(pe1_tgen_ctrl_09, tg_gain0,		GET_BITS(pp->tg_gain0,0,4),\
														  tg_gain1,		GET_BITS(pp->tg_gain1,0,4),\
														  tg_gain2,		GET_BITS(pp->tg_gain2,0,4),\
														  tg_gain3,		GET_BITS(pp->tg_gain3,0,4));
				PE_SHP_HW_H15A_SR_SHP_WR04(pe1_tgen_ctrl_09, tg_gain4,		GET_BITS(pp->tg_gain4,0,4),\
														  tg_gain5,		GET_BITS(pp->tg_gain5,0,4),\
														  tg_gain6,		GET_BITS(pp->tg_gain6,0,4),\
														  tg_gain7,		GET_BITS(pp->tg_gain7,0,4));
				#else	//#ifdef PE_SHP_HW_H15_SRE_SHP_WR_ONLY_MD
				#ifndef PE_HW_H15_PROTECT_OVERLAP_SETTINGS
				/* h 3 */
				PE_SHP_HW_H15A_SR_SHP_WR01(pe1_derh_ctrl_06,edge_y_filter_en,	GET_BITS(pp->edge_Y_filter_en,0,1));
				PE_SHP_HW_H15A_SR_SHP_WR02(pe1_derh_ctrl_0c,e_gain_max,		GET_BITS(pp->e_gain_max,0,6),\
														f_gain_max,			GET_BITS(pp->f_gain_max,0,6));
				/* v 3 */
				PE_SHP_HW_H15A_SR_SHP_WR02(pe1_derv_ctrl_0,der_v_en,			GET_BITS(pp->der_v_en,0,1),\
														der_gain_mapping,	GET_BITS(pp->der_gain_mapping,0,2));
				PE_SHP_HW_H15A_SR_SHP_WR01(pe1_derv_ctrl_2,max_sel,			GET_BITS(pp->max_sel,0,3));
				/* cmm 6 */
				PE_SHP_HW_H15A_SR_SHP_WR01(pe1_mp_ctrl_02,	laplacian_weight,	GET_BITS(pp->mp_laplacian_weight,0,8));
				PE_SHP_HW_H15A_SR_SHP_WR01(pe1_sp_ctrl_02,	laplacian_weight,	GET_BITS(pp->sp_laplacian_weight,0,8));
				PE_SHP_HW_H15A_SR_SHP_WR03(pe1_derh_ctrl_07,flat_filter_en,	GET_BITS(pp->flat_filter_en,0,1),\
														flat_filter_gain,	GET_BITS(pp->flat_filter_gain,0,6),\
														flat_filter_type,	GET_BITS(pp->flat_filter_type,0,1));
				/*d_jag 12 */
				PE_SHP_HW_H15A_SR_SHP_WR05(pe1_dj_ctrl_00,	edf_en,				GET_BITS(pp->edf_en,0,1),\
														center_blur_mode, 	GET_BITS(pp->center_blur_mode,0,1),\
														count_diff_th, 		GET_BITS(pp->count_diff_th,0,5),\
														n_avg_mode,			GET_BITS(pp->n_avg_mode,0,1),\
														line_variation_diff_threshold,GET_BITS(pp->line_variation_diff_threshold,0,8));
				PE_SHP_HW_H15A_SR_SHP_WR03(pe1_dj_ctrl_01,	level_th,			GET_BITS(pp->level_th,0,8),\
														protect_th,			GET_BITS(pp->protect_th,0,8),\
														n_avg_gain,			GET_BITS(pp->n_avg_gain,0,8));
				PE_SHP_HW_H15A_SR_SHP_WR02(pe1_dj_ctrl_02,	reg_g0_cnt_min,		GET_BITS(pp->reg_g0_cnt_min,0,5),\
														reg_g0_mul,			GET_BITS(pp->reg_g0_mul,0,5));
				PE_SHP_HW_H15A_SR_SHP_WR02(pe1_dj_ctrl_03,	reg_g1_protect_min,	GET_BITS(pp->reg_g1_protect_min,0,8),\
														reg_g1_mul, 		GET_BITS(pp->reg_g1_mul,0,7));				
				/*e_map 36 */
				PE_SHP_HW_H15A_SR_SHP_WR02(pe1_coring_ctrl_00, reg_cross_th_sc,	GET_BITS(pp->reg_cross_th_sc,0,8),\
														    reg_cross_th,		GET_BITS(pp->reg_cross_th,0,8));
				PE_SHP_HW_H15A_SR_SHP_WR02(pe1_coring_ctrl_08, reg_mul_base_t,		GET_BITS(pp->reg_mul_base_t,0,8),\
														    reg_mul_base_e,		GET_BITS(pp->reg_mul_base_e,0,8));
				PE_SHP_HW_H15A_SR_SHP_WR02(pe1_coring_ctrl_00, reg_a_map_h_size,	GET_BITS(pp->reg_a_map_h_size,0,8),\
														    reg_a_nomalize_en,	GET_BITS(pp->reg_a_nomalize_en,0,8));
				PE_SHP_HW_H15A_SR_SHP_WR02(pe1_coring_ctrl_01, reg_edge_alpha_mul,	GET_BITS(pp->reg_edge_alpha_mul,0,8),\
														    reg_edge_beta_mul,	GET_BITS(pp->reg_edge_beta_mul,0,8));
				PE_SHP_HW_H15A_SR_SHP_WR04(pe1_coring_ctrl_02,	reg_a_lut_edge_x0,		GET_BITS(pp->reg_a_lut_edge_x0,0,8),\
															reg_a_lut_edge_y0,		GET_BITS(pp->reg_a_lut_edge_y0,0,8),\
															reg_a_lut_edge_x1,		GET_BITS(pp->reg_a_lut_edge_x1,0,8),\
															reg_a_lut_edge_y1,		GET_BITS(pp->reg_a_lut_edge_y1,0,8));
				PE_SHP_HW_H15A_SR_SHP_WR04(pe1_coring_ctrl_03,	reg_a_lut_detail_x0,	GET_BITS(pp->reg_a_lut_detail_x0,0,8),\
															reg_a_lut_detail_y0,	GET_BITS(pp->reg_a_lut_detail_y0,0,8),\
															reg_a_lut_detail_x1,	GET_BITS(pp->reg_a_lut_detail_x1,0,8),\
															reg_a_lut_detail_y1,	GET_BITS(pp->reg_a_lut_detail_y1,0,8));
				PE_SHP_HW_H15A_SR_SHP_WR04(pe1_coring_ctrl_06, reg_edge_minmax_x0,		GET_BITS(pp->reg_edge_minmax_x0,0,8),\
															reg_edge_minmax_y0,		GET_BITS(pp->reg_edge_minmax_y0,0,8),\
															reg_edge_minmax_x1,		GET_BITS(pp->reg_edge_minmax_x1,0,8),\
															reg_edge_minmax_y1,		GET_BITS(pp->reg_edge_minmax_y1,0,8));
				PE_SHP_HW_H15A_SR_SHP_WR02(pe1_coring_ctrl_00, reg_t_cut_resolution,	GET_BITS(pp->reg_t_cut_resolution,0,2),\
														    reg_t_nomalize_en,		GET_BITS(pp->reg_t_nomalize_en,0,1));
				PE_SHP_HW_H15A_SR_SHP_WR02(pe1_coring_ctrl_01, reg_detail_alpha_mul,	GET_BITS(pp->reg_detail_alpha_mul,0,8),\
														    reg_detail_beta_mul,	GET_BITS(pp->reg_detail_beta_mul,0,8));
				PE_SHP_HW_H15A_SR_SHP_WR04(pe1_coring_ctrl_04, reg_t_lut_edge_x0,		GET_BITS(pp->reg_t_lut_edge_x0,0,8),\
															reg_t_lut_edge_y0,		GET_BITS(pp->reg_t_lut_edge_y0,0,8),\
															reg_t_lut_edge_x1,		GET_BITS(pp->reg_t_lut_edge_x1,0,8),\
															reg_t_lut_edge_y1,		GET_BITS(pp->reg_t_lut_edge_y1,0,8));
				PE_SHP_HW_H15A_SR_SHP_WR04(pe1_coring_ctrl_05, reg_t_lut_detail_x0,	GET_BITS(pp->reg_t_lut_detail_x0,0,8),\
															reg_t_lut_detail_y0,	GET_BITS(pp->reg_t_lut_detail_y0,0,8),\
															reg_t_lut_detail_x1,	GET_BITS(pp->reg_t_lut_detail_x1,0,8),\
															reg_t_lut_detail_y1,	GET_BITS(pp->reg_t_lut_detail_y1,0,8));
				PE_SHP_HW_H15A_SR_SHP_WR04(pe1_coring_ctrl_07, reg_detail_minmax_x0,	GET_BITS(pp->reg_detail_minmax_x0,0,8),\
															reg_detail_minmax_y0,	GET_BITS(pp->reg_detail_minmax_y0,0,8),\
															reg_detail_minmax_x1,	GET_BITS(pp->reg_detail_minmax_x1,0,8),\
															reg_detail_minmax_y1,	GET_BITS(pp->reg_detail_minmax_y1,0,8));
				/* blc*/
				PE_SHP_HW_H15A_SR_SHP_WR04(pe1_mp_ctrl_07, gb_en,		GET_BITS(pp->mp_gb_en,0,1),\
														gb_mode,	GET_BITS(pp->mp_gb_mode,0,1),\
														gb_x1,		GET_BITS(pp->mp_gb_x1,0,8),\
														gb_y1,		GET_BITS(pp->mp_gb_y1,0,8));
				PE_SHP_HW_H15A_SR_SHP_WR03(pe1_mp_ctrl_08 ,gb_x2,		GET_BITS(pp->mp_gb_x2,0,8),\
														gb_y2, 		GET_BITS(pp->mp_gb_y2,0,8),\
														gb_y3,		GET_BITS(pp->mp_gb_y3,0,8));
				PE_SHP_HW_H15A_SR_SHP_WR04(pe1_mp_ctrl_09, lum1_x_l0,	GET_BITS(pp->mp_lum1_x_L0,0,8),\
														lum1_x_l1,	GET_BITS(pp->mp_lum1_x_L1,0,8),\
														lum1_x_h0,	GET_BITS(pp->mp_lum1_x_H0,0,8),\
														lum1_x_h1,	GET_BITS(pp->mp_lum1_x_H1,0,8));
				PE_SHP_HW_H15A_SR_SHP_WR04(pe1_mp_ctrl_0a, lum1_y0,	GET_BITS(pp->mp_lum1_y0,0,8),\
														lum1_y1,	GET_BITS(pp->mp_lum1_y1,0,8),\
														lum1_y2,	GET_BITS(pp->mp_lum1_y2,0,8),\
														lum2_x_l0,	GET_BITS(pp->mp_lum2_x_L0,0,8));
				PE_SHP_HW_H15A_SR_SHP_WR04(pe1_mp_ctrl_0b, lum2_x_l1,	GET_BITS(pp->mp_lum2_x_L1,0,8),\
														lum2_x_h0,	GET_BITS(pp->mp_lum2_x_H0,0,8),\
														lum2_x_h1,	GET_BITS(pp->mp_lum2_x_H1,0,8),\
														lum2_y0,	GET_BITS(pp->mp_lum2_y0,0,8));
				PE_SHP_HW_H15A_SR_SHP_WR02(pe1_mp_ctrl_0c, lum2_y1,	GET_BITS(pp->mp_lum2_y1,0,8),\
														lum2_y2,	GET_BITS(pp->mp_lum2_y2,0,8));
				PE_SHP_HW_H15A_SR_SHP_WR04(pe1_sp_ctrl_07, gb_en,		GET_BITS(pp->sp_gb_en,0,1),\
														gb_mode,	GET_BITS(pp->sp_gb_mode,0,1),\
														gb_x1,		GET_BITS(pp->sp_gb_x1,0,8),\
														gb_y1,		GET_BITS(pp->sp_gb_y1,0,8));
				PE_SHP_HW_H15A_SR_SHP_WR03(pe1_sp_ctrl_08, gb_x2,		GET_BITS(pp->sp_gb_x2,0,8),\
														gb_y2, 		GET_BITS(pp->sp_gb_y2,0,8),\
														gb_y3,		GET_BITS(pp->sp_gb_y3,0,8));
				PE_SHP_HW_H15A_SR_SHP_WR04(pe1_sp_ctrl_09, lum1_x_l0,	GET_BITS(pp->sp_lum1_x_L0,0,8),\
														lum1_x_l1,	GET_BITS(pp->sp_lum1_x_L1,0,8),\
														lum1_x_h0,	GET_BITS(pp->sp_lum1_x_H0,0,8),\
														lum1_x_h1,	GET_BITS(pp->sp_lum1_x_H1,0,8));
				PE_SHP_HW_H15A_SR_SHP_WR04(pe1_sp_ctrl_0a, lum1_y0,	GET_BITS(pp->sp_lum1_y0,0,8),\
														lum1_y1,	GET_BITS(pp->sp_lum1_y1,0,8),\
														lum1_y2,	GET_BITS(pp->sp_lum1_y2,0,8),\
														lum2_x_l0,	GET_BITS(pp->sp_lum2_x_L0,0,8));
				PE_SHP_HW_H15A_SR_SHP_WR04(pe1_sp_ctrl_0b, lum2_x_l1,	GET_BITS(pp->sp_lum2_x_L1,0,8),\
														lum2_x_h0,	GET_BITS(pp->sp_lum2_x_H0,0,8),\
														lum2_x_h1,	GET_BITS(pp->sp_lum2_x_H1,0,8),\
														lum2_y0,	GET_BITS(pp->sp_lum2_y0,0,8));
				PE_SHP_HW_H15A_SR_SHP_WR02(pe1_sp_ctrl_0c, lum2_y1,	GET_BITS(pp->sp_lum2_y1,0,8),\
														lum2_y2,	GET_BITS(pp->sp_lum2_y2,0,8));
				/* ti*/
				PE_SHP_HW_H15A_SR_SHP_WR04(pe1_pti_ctrl_00,reg_ti_enable,	GET_BITS(pp->reg_ti_enable_pti_v,0,1),\
				                                         reg_mm_tap_size, 	GET_BITS(pp->reg_mm_tap_size_pti_v,0,2),\
				                                         reg_avg_tap_size,	GET_BITS(pp->reg_avg_tap_size_pti_v,0,3),\
				                                         reg_debug_map_en,0);
				PE_SHP_HW_H15A_SR_SHP_WR04(pe1_pti_ctrl_01,reg_ti_enable,	GET_BITS(pp->reg_ti_enable_pti_h,0,1),\
				                                         reg_mm_tap_size, 	GET_BITS(pp->reg_mm_tap_size_pti_h,0,2),\
				                                         reg_avg_tap_size,	GET_BITS(pp->reg_avg_tap_size_pti_h,0,3),\
				                                         reg_debug_map_en,0);
				PE_SHP_HW_H15A_SR_SHP_WR04(pe1_sti_ctrl_00,reg_ti_enable,	GET_BITS(pp->reg_enable_sti,0,1),\
				                                         reg_mm_tap_size, 	GET_BITS(pp->reg_mm_tap_size_sti,0,2),\
				                                         reg_avg_tap_size,	GET_BITS(pp->reg_avg_tap_size_sti,0,3),\
				                                         reg_debug_map_en,0);
				/* tgn*/
				PE_SHP_HW_H15A_SR_SHP_WR01(pe1_tgen_ctrl_00, tgen_on_off,		GET_BITS(pp->tgen_on_off,0,1));
				PE_SHP_HW_H15A_SR_SHP_WR04(pe1_tgen_ctrl_01, tg_delta_h_bound,	GET_BITS(pp->tg_delta_H_bound,0,5),\
														  tg_delta_l_bound,	GET_BITS(pp->tg_delta_L_bound,0,4),\
														  tg_delta_max,		GET_BITS(pp->tg_delta_max,0,7),\
														  tg_delta_scale,	GET_BITS(pp->tg_delta_scale,0,3));
				PE_SHP_HW_H15A_SR_SHP_WR03(pe1_tgen_ctrl_02, tg_rnd_th,		GET_BITS(pp->tg_rnd_th,0,8),\
														  tg_delta_wcurr, 	GET_BITS(pp->tg_delta_wcurr,0,4),\
														  tg_lc_ldr_th,		GET_BITS(pp->tg_lc_ldr_th,0,5));
				PE_SHP_HW_H15A_SR_SHP_WR04(pe1_tgen_ctrl_04, tg_dr_ratio,		GET_BITS(pp->tg_dr_ratio,0,3),\
														  tg_dr_th, 		GET_BITS(pp->tg_dr_th,0,6),\
														  tg_mm_offset,		GET_BITS(pp->tg_mm_offset,0,5),\
														  tg_mm_sel_mode,	GET_BITS(pp->tg_mm_sel_mode,0,2));
				PE_SHP_HW_H15A_SR_SHP_WR03(pe1_tgen_ctrl_05, tg_lavg_th,		GET_BITS(pp->tg_lavg_th,0,10),\
														  tg_gs_rnd,	 	GET_BITS(pp->tg_gs_rnd,0,5),\
														  tg_gs_mm,			GET_BITS(pp->tg_gs_mm,0,5));
				PE_SHP_HW_H15A_SR_SHP_WR04(pe1_tgen_ctrl_09, tg_gain0,		GET_BITS(pp->tg_gain0,0,4),\
														  tg_gain1,		GET_BITS(pp->tg_gain1,0,4),\
														  tg_gain2,		GET_BITS(pp->tg_gain2,0,4),\
														  tg_gain3,		GET_BITS(pp->tg_gain3,0,4));
				PE_SHP_HW_H15A_SR_SHP_WR04(pe1_tgen_ctrl_09, tg_gain4,		GET_BITS(pp->tg_gain4,0,4),\
														  tg_gain5,		GET_BITS(pp->tg_gain5,0,4),\
														  tg_gain6,		GET_BITS(pp->tg_gain6,0,4),\
														  tg_gain7,		GET_BITS(pp->tg_gain7,0,4));
				#else
				/* h 3 */
				PE_SR_SHP_H15A0_QWr01(pe1_derh_ctrl_06,edge_y_filter_en,	GET_BITS(pp->edge_Y_filter_en,0,1));
				PE_SR_SHP_H15A0_QWr02(pe1_derh_ctrl_0c,e_gain_max,		GET_BITS(pp->e_gain_max,0,6),\
														f_gain_max,			GET_BITS(pp->f_gain_max,0,6));
				/* v 3 */
				PE_SR_SHP_H15A0_QWr02(pe1_derv_ctrl_0,der_v_en,			GET_BITS(pp->der_v_en,0,1),\
														der_gain_mapping,	GET_BITS(pp->der_gain_mapping,0,2));
				PE_SR_SHP_H15A0_QWr01(pe1_derv_ctrl_2,max_sel,			GET_BITS(pp->max_sel,0,3));
				/* cmm 6 */
				PE_SR_SHP_H15A0_QWr01(pe1_mp_ctrl_02,	laplacian_weight,	GET_BITS(pp->mp_laplacian_weight,0,8));
				PE_SR_SHP_H15A0_QWr01(pe1_sp_ctrl_02,	laplacian_weight,	GET_BITS(pp->sp_laplacian_weight,0,8));
				PE_SR_SHP_H15A0_QWr03(pe1_derh_ctrl_07,flat_filter_en,	GET_BITS(pp->flat_filter_en,0,1),\
														flat_filter_gain,	GET_BITS(pp->flat_filter_gain,0,6),\
														flat_filter_type,	GET_BITS(pp->flat_filter_type,0,1));
				/*d_jag 12 */
				PE_SR_SHP_H15A0_QWr05(pe1_dj_ctrl_00,	edf_en,				GET_BITS(pp->edf_en,0,1),\
														center_blur_mode, 	GET_BITS(pp->center_blur_mode,0,1),\
														count_diff_th, 		GET_BITS(pp->count_diff_th,0,5),\
														n_avg_mode,			GET_BITS(pp->n_avg_mode,0,1),\
														line_variation_diff_threshold,GET_BITS(pp->line_variation_diff_threshold,0,8));
				PE_SR_SHP_H15A0_QWr03(pe1_dj_ctrl_01,	level_th,			GET_BITS(pp->level_th,0,8),\
														protect_th,			GET_BITS(pp->protect_th,0,8),\
														n_avg_gain,			GET_BITS(pp->n_avg_gain,0,8));
				PE_SR_SHP_H15A0_QWr02(pe1_dj_ctrl_02,	reg_g0_cnt_min,		GET_BITS(pp->reg_g0_cnt_min,0,5),\
														reg_g0_mul,			GET_BITS(pp->reg_g0_mul,0,5));
				PE_SR_SHP_H15A0_QWr02(pe1_dj_ctrl_03,	reg_g1_protect_min,	GET_BITS(pp->reg_g1_protect_min,0,8),\
														reg_g1_mul, 		GET_BITS(pp->reg_g1_mul,0,7));				
				/*e_map 36 */
				PE_SR_SHP_H15A0_QWr02(pe1_coring_ctrl_00, reg_cross_th_sc,	GET_BITS(pp->reg_cross_th_sc,0,8),\
														    reg_cross_th,		GET_BITS(pp->reg_cross_th,0,8));
				PE_SR_SHP_H15A0_QWr02(pe1_coring_ctrl_08, reg_mul_base_t,		GET_BITS(pp->reg_mul_base_t,0,8),\
														    reg_mul_base_e,		GET_BITS(pp->reg_mul_base_e,0,8));
				PE_SR_SHP_H15A0_QWr02(pe1_coring_ctrl_00, reg_a_map_h_size,	GET_BITS(pp->reg_a_map_h_size,0,8),\
														    reg_a_nomalize_en,	GET_BITS(pp->reg_a_nomalize_en,0,8));
				PE_SR_SHP_H15A0_QWr02(pe1_coring_ctrl_01, reg_edge_alpha_mul,	GET_BITS(pp->reg_edge_alpha_mul,0,8),\
														    reg_edge_beta_mul,	GET_BITS(pp->reg_edge_beta_mul,0,8));
				PE_SR_SHP_H15A0_QWr04(pe1_coring_ctrl_02,	reg_a_lut_edge_x0,		GET_BITS(pp->reg_a_lut_edge_x0,0,8),\
															reg_a_lut_edge_y0,		GET_BITS(pp->reg_a_lut_edge_y0,0,8),\
															reg_a_lut_edge_x1,		GET_BITS(pp->reg_a_lut_edge_x1,0,8),\
															reg_a_lut_edge_y1,		GET_BITS(pp->reg_a_lut_edge_y1,0,8));
				PE_SR_SHP_H15A0_QWr04(pe1_coring_ctrl_03,	reg_a_lut_detail_x0,	GET_BITS(pp->reg_a_lut_detail_x0,0,8),\
															reg_a_lut_detail_y0,	GET_BITS(pp->reg_a_lut_detail_y0,0,8),\
															reg_a_lut_detail_x1,	GET_BITS(pp->reg_a_lut_detail_x1,0,8),\
															reg_a_lut_detail_y1,	GET_BITS(pp->reg_a_lut_detail_y1,0,8));
				PE_SR_SHP_H15A0_QWr04(pe1_coring_ctrl_06, reg_edge_minmax_x0,		GET_BITS(pp->reg_edge_minmax_x0,0,8),\
															reg_edge_minmax_y0,		GET_BITS(pp->reg_edge_minmax_y0,0,8),\
															reg_edge_minmax_x1,		GET_BITS(pp->reg_edge_minmax_x1,0,8),\
															reg_edge_minmax_y1,		GET_BITS(pp->reg_edge_minmax_y1,0,8));
				PE_SR_SHP_H15A0_QWr02(pe1_coring_ctrl_00, reg_t_cut_resolution,	GET_BITS(pp->reg_t_cut_resolution,0,2),\
														    reg_t_nomalize_en,		GET_BITS(pp->reg_t_nomalize_en,0,1));
				PE_SR_SHP_H15A0_QWr02(pe1_coring_ctrl_01, reg_detail_alpha_mul,	GET_BITS(pp->reg_detail_alpha_mul,0,8),\
														    reg_detail_beta_mul,	GET_BITS(pp->reg_detail_beta_mul,0,8));
				PE_SR_SHP_H15A0_QWr04(pe1_coring_ctrl_04, reg_t_lut_edge_x0,		GET_BITS(pp->reg_t_lut_edge_x0,0,8),\
															reg_t_lut_edge_y0,		GET_BITS(pp->reg_t_lut_edge_y0,0,8),\
															reg_t_lut_edge_x1,		GET_BITS(pp->reg_t_lut_edge_x1,0,8),\
															reg_t_lut_edge_y1,		GET_BITS(pp->reg_t_lut_edge_y1,0,8));
				PE_SR_SHP_H15A0_QWr04(pe1_coring_ctrl_05, reg_t_lut_detail_x0,	GET_BITS(pp->reg_t_lut_detail_x0,0,8),\
															reg_t_lut_detail_y0,	GET_BITS(pp->reg_t_lut_detail_y0,0,8),\
															reg_t_lut_detail_x1,	GET_BITS(pp->reg_t_lut_detail_x1,0,8),\
															reg_t_lut_detail_y1,	GET_BITS(pp->reg_t_lut_detail_y1,0,8));
				PE_SR_SHP_H15A0_QWr04(pe1_coring_ctrl_07, reg_detail_minmax_x0,	GET_BITS(pp->reg_detail_minmax_x0,0,8),\
															reg_detail_minmax_y0,	GET_BITS(pp->reg_detail_minmax_y0,0,8),\
															reg_detail_minmax_x1,	GET_BITS(pp->reg_detail_minmax_x1,0,8),\
															reg_detail_minmax_y1,	GET_BITS(pp->reg_detail_minmax_y1,0,8));
				/* blc*/
				PE_SR_SHP_H15A0_QWr04(pe1_mp_ctrl_07, gb_en,		GET_BITS(pp->mp_gb_en,0,1),\
														gb_mode,	GET_BITS(pp->mp_gb_mode,0,1),\
														gb_x1,		GET_BITS(pp->mp_gb_x1,0,8),\
														gb_y1,		GET_BITS(pp->mp_gb_y1,0,8));
				PE_SR_SHP_H15A0_QWr03(pe1_mp_ctrl_08 ,gb_x2,		GET_BITS(pp->mp_gb_x2,0,8),\
														gb_y2, 		GET_BITS(pp->mp_gb_y2,0,8),\
														gb_y3,		GET_BITS(pp->mp_gb_y3,0,8));
				PE_SR_SHP_H15A0_QWr04(pe1_mp_ctrl_09, lum1_x_l0,	GET_BITS(pp->mp_lum1_x_L0,0,8),\
														lum1_x_l1,	GET_BITS(pp->mp_lum1_x_L1,0,8),\
														lum1_x_h0,	GET_BITS(pp->mp_lum1_x_H0,0,8),\
														lum1_x_h1,	GET_BITS(pp->mp_lum1_x_H1,0,8));
				PE_SR_SHP_H15A0_QWr04(pe1_mp_ctrl_0a, lum1_y0,	GET_BITS(pp->mp_lum1_y0,0,8),\
														lum1_y1,	GET_BITS(pp->mp_lum1_y1,0,8),\
														lum1_y2,	GET_BITS(pp->mp_lum1_y2,0,8),\
														lum2_x_l0,	GET_BITS(pp->mp_lum2_x_L0,0,8));
				PE_SR_SHP_H15A0_QWr04(pe1_mp_ctrl_0b, lum2_x_l1,	GET_BITS(pp->mp_lum2_x_L1,0,8),\
														lum2_x_h0,	GET_BITS(pp->mp_lum2_x_H0,0,8),\
														lum2_x_h1,	GET_BITS(pp->mp_lum2_x_H1,0,8),\
														lum2_y0,	GET_BITS(pp->mp_lum2_y0,0,8));
				PE_SR_SHP_H15A0_QWr02(pe1_mp_ctrl_0c, lum2_y1,	GET_BITS(pp->mp_lum2_y1,0,8),\
														lum2_y2,	GET_BITS(pp->mp_lum2_y2,0,8));
				PE_SR_SHP_H15A0_QWr04(pe1_sp_ctrl_07, gb_en,		GET_BITS(pp->sp_gb_en,0,1),\
														gb_mode,	GET_BITS(pp->sp_gb_mode,0,1),\
														gb_x1,		GET_BITS(pp->sp_gb_x1,0,8),\
														gb_y1,		GET_BITS(pp->sp_gb_y1,0,8));
				PE_SR_SHP_H15A0_QWr03(pe1_sp_ctrl_08, gb_x2,		GET_BITS(pp->sp_gb_x2,0,8),\
														gb_y2, 		GET_BITS(pp->sp_gb_y2,0,8),\
														gb_y3,		GET_BITS(pp->sp_gb_y3,0,8));
				PE_SR_SHP_H15A0_QWr04(pe1_sp_ctrl_09, lum1_x_l0,	GET_BITS(pp->sp_lum1_x_L0,0,8),\
														lum1_x_l1,	GET_BITS(pp->sp_lum1_x_L1,0,8),\
														lum1_x_h0,	GET_BITS(pp->sp_lum1_x_H0,0,8),\
														lum1_x_h1,	GET_BITS(pp->sp_lum1_x_H1,0,8));
				PE_SR_SHP_H15A0_QWr04(pe1_sp_ctrl_0a, lum1_y0,	GET_BITS(pp->sp_lum1_y0,0,8),\
														lum1_y1,	GET_BITS(pp->sp_lum1_y1,0,8),\
														lum1_y2,	GET_BITS(pp->sp_lum1_y2,0,8),\
														lum2_x_l0,	GET_BITS(pp->sp_lum2_x_L0,0,8));
				PE_SR_SHP_H15A0_QWr04(pe1_sp_ctrl_0b, lum2_x_l1,	GET_BITS(pp->sp_lum2_x_L1,0,8),\
														lum2_x_h0,	GET_BITS(pp->sp_lum2_x_H0,0,8),\
														lum2_x_h1,	GET_BITS(pp->sp_lum2_x_H1,0,8),\
														lum2_y0,	GET_BITS(pp->sp_lum2_y0,0,8));
				PE_SR_SHP_H15A0_QWr02(pe1_sp_ctrl_0c, lum2_y1,	GET_BITS(pp->sp_lum2_y1,0,8),\
														lum2_y2,	GET_BITS(pp->sp_lum2_y2,0,8));
				/* ti*/
				PE_SR_SHP_H15A0_QWr03(pe1_pti_ctrl_00, reg_ti_enable,		GET_BITS(pp->reg_ti_enable_pti_v,0,1),\
														 reg_mm_tap_size, 	GET_BITS(pp->reg_mm_tap_size_pti_v,0,2),\
														 reg_avg_tap_size,	GET_BITS(pp->reg_avg_tap_size_pti_v,0,3));
				PE_SR_SHP_H15A0_QWr03(pe1_pti_ctrl_01, reg_ti_enable,		GET_BITS(pp->reg_ti_enable_pti_h,0,1),\
														 reg_mm_tap_size, 	GET_BITS(pp->reg_mm_tap_size_pti_h,0,2),\
														 reg_avg_tap_size,	GET_BITS(pp->reg_avg_tap_size_pti_h,0,3));
				PE_SR_SHP_H15A0_QWr03(pe1_sti_ctrl_00, reg_ti_enable,		GET_BITS(pp->reg_enable_sti,0,1),\
														 reg_mm_tap_size, 	GET_BITS(pp->reg_mm_tap_size_sti,0,2),\
														 reg_avg_tap_size,	GET_BITS(pp->reg_avg_tap_size_sti,0,3));
				/* tgn*/
				PE_SR_SHP_H15A0_QWr01(pe1_tgen_ctrl_00, tgen_on_off,		GET_BITS(pp->tgen_on_off,0,1));
				PE_SR_SHP_H15A0_QWr04(pe1_tgen_ctrl_01, tg_delta_h_bound,	GET_BITS(pp->tg_delta_H_bound,0,5),\
														  tg_delta_l_bound,	GET_BITS(pp->tg_delta_L_bound,0,4),\
														  tg_delta_max,		GET_BITS(pp->tg_delta_max,0,7),\
														  tg_delta_scale,	GET_BITS(pp->tg_delta_scale,0,3));
				PE_SR_SHP_H15A0_QWr03(pe1_tgen_ctrl_02, tg_rnd_th,		GET_BITS(pp->tg_rnd_th,0,8),\
														  tg_delta_wcurr, 	GET_BITS(pp->tg_delta_wcurr,0,4),\
														  tg_lc_ldr_th,		GET_BITS(pp->tg_lc_ldr_th,0,5));
				PE_SR_SHP_H15A0_QWr04(pe1_tgen_ctrl_04, tg_dr_ratio,		GET_BITS(pp->tg_dr_ratio,0,3),\
														  tg_dr_th, 		GET_BITS(pp->tg_dr_th,0,6),\
														  tg_mm_offset,		GET_BITS(pp->tg_mm_offset,0,5),\
														  tg_mm_sel_mode,	GET_BITS(pp->tg_mm_sel_mode,0,2));
				PE_SR_SHP_H15A0_QWr03(pe1_tgen_ctrl_05, tg_lavg_th,		GET_BITS(pp->tg_lavg_th,0,10),\
														  tg_gs_rnd,	 	GET_BITS(pp->tg_gs_rnd,0,5),\
														  tg_gs_mm,			GET_BITS(pp->tg_gs_mm,0,5));
				PE_SR_SHP_H15A0_QWr04(pe1_tgen_ctrl_09, tg_gain0,		GET_BITS(pp->tg_gain0,0,4),\
														  tg_gain1,		GET_BITS(pp->tg_gain1,0,4),\
														  tg_gain2,		GET_BITS(pp->tg_gain2,0,4),\
														  tg_gain3,		GET_BITS(pp->tg_gain3,0,4));
				PE_SR_SHP_H15A0_QWr04(pe1_tgen_ctrl_09, tg_gain4,		GET_BITS(pp->tg_gain4,0,4),\
														  tg_gain5,		GET_BITS(pp->tg_gain5,0,4),\
														  tg_gain6,		GET_BITS(pp->tg_gain6,0,4),\
														  tg_gain7,		GET_BITS(pp->tg_gain7,0,4));
				#endif
				#endif	//#ifdef PE_SHP_HW_H15_SRE_SHP_WR_ONLY_MD
				#ifdef PE_SHP_HW_H15_SR_PWD_TRACE
				CTOP_CTRL_H15A0_RdFL(SR,ctr00);
				CTOP_CTRL_H15A0_Rd01(SR,ctr00,cg_sr_en,r_data[0]);
				CTOP_CTRL_H15A0_Rd01(SR,ctr00,cg_shs_en,r_data[1]);
				PE_SHP_HW_H15_DBG_SRPD_TRACE("E cg sr,shs:%d,%d\n",r_data[0],r_data[1]);
				#endif
			}
		}
		else
		{
			PE_SHP_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	} while(0);
	return ret;
}
/**
 * get sr resolution misc control
 * - use input struct LX_PE_SHP_SR_RE_MISC_T for H15X
 *
 * @param   *pstParams [in/out] void
 * @return  OK if success, ERROR otherwise.
 * @see		function description
 * @author
 */
int PE_SHP_HW_H15_GetSreReMiscCtrl(void *pstParams)
{
	int ret = RET_OK;
	LX_PE_WIN_ID win_id;
	do {
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H15)
		{
			LX_PE_SHP_SR_RE_MISC_T *pp=(LX_PE_SHP_SR_RE_MISC_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			win_id = PE_GET_CHECKED_WINID(pp->win_id);
			if(PE_CHECK_WIN0(win_id))
			{
				#if 0
				PE_FWI_H15_SETTINGS_T fw_inf;
				ret = PE_FWI_H15_GetCurFwiSettings(&fw_inf);
				PE_SHP_HW_H15_CHECK_CODE(ret, break, \
					"[%s,%d] PE_FWI_H15_GetCurFwiSettings() error.\n", __F__, __L__);
				PE_SHP_HW_H15_DBG_PRINT("fw_ctrl.enable : %s.\n", \
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
					#ifdef PE_HW_H15_FOR_BRINGUP
					#else
					PE_SHPL_H15A0_RdFL(pe1_coring_ctrl_05);
					PE_SHPL_H15A0_RdFL(pe1_coring_ctrl_06);
					PE_SHPL_H15A0_Rd01(pe1_coring_ctrl_05,	gt_th1,		pp->gt_th1);
					PE_SHPL_H15A0_Rd01(pe1_coring_ctrl_05,	gt_th0,		pp->gt_th0);
					PE_SHPL_H15A0_Rd01(pe1_coring_ctrl_06,	gt_th0a,	pp->gt_th0a);
					PE_SHPL_H15A0_Rd01(pe1_coring_ctrl_06,	gt_th0b,	pp->gt_th0b);
					#endif
				}
				#endif
				PE_SR_SHP_H15A0_QRd01(pe1_derh_ctrl_06,edge_y_filter_en,	pp->edge_Y_filter_en);
				PE_SR_SHP_H15A0_QRd02(pe1_derh_ctrl_0c,e_gain_max,		pp->e_gain_max,\
													f_gain_max,			pp->f_gain_max);
				/* v 5 */
				PE_SR_SHP_H15A0_QRd02(pe1_derv_ctrl_0,der_v_en,			pp->der_v_en,\
													der_gain_mapping,	pp->der_gain_mapping);
				PE_SR_SHP_H15A0_QRd01(pe1_derv_ctrl_2,max_sel,			pp->max_sel);
				/* cmm 5 */
				PE_SR_SHP_H15A0_QRd01(pe1_mp_ctrl_02,	laplacian_weight,		pp->mp_laplacian_weight);
				PE_SR_SHP_H15A0_QRd01(pe1_sp_ctrl_02,	laplacian_weight,		pp->sp_laplacian_weight);
				PE_SR_SHP_H15A0_QRd03(pe1_derh_ctrl_07,	flat_filter_en,		pp->flat_filter_en,\
														flat_filter_gain,	pp->flat_filter_gain,\
														flat_filter_type,	pp->flat_filter_type);
				/*d_jag 12 */
				PE_SR_SHP_H15A0_QRd05(pe1_dj_ctrl_00,	edf_en,				pp->edf_en,\
													center_blur_mode, 	pp->center_blur_mode,\
													count_diff_th, 		pp->count_diff_th,\
													n_avg_mode,			pp->n_avg_mode,\
													line_variation_diff_threshold,pp->line_variation_diff_threshold);
				PE_SR_SHP_H15A0_QRd03(pe1_dj_ctrl_01,	level_th,			pp->level_th,\
													protect_th,			pp->protect_th,\
													n_avg_gain,			pp->n_avg_gain);
				PE_SR_SHP_H15A0_QRd02(pe1_dj_ctrl_02,	reg_g0_cnt_min,		pp->reg_g0_cnt_min,\
													reg_g0_mul,			pp->reg_g0_mul);
				PE_SR_SHP_H15A0_QRd02(pe1_dj_ctrl_03,	reg_g1_protect_min,	pp->reg_g1_protect_min,\
													reg_g1_mul, 		pp->reg_g1_mul);				
				/*e_map 36 */
				PE_SR_SHP_H15A0_QRd02(pe1_coring_ctrl_00, reg_cross_th_sc,	pp->reg_cross_th_sc,\
														reg_cross_th,		pp->reg_cross_th);
				PE_SR_SHP_H15A0_QRd02(pe1_coring_ctrl_08, reg_mul_base_t,		pp->reg_mul_base_t,\
														reg_mul_base_e,		pp->reg_mul_base_e);
				PE_SR_SHP_H15A0_QRd02(pe1_coring_ctrl_00, reg_a_map_h_size,	pp->reg_a_map_h_size,\
														reg_a_nomalize_en,	pp->reg_a_nomalize_en);
				PE_SR_SHP_H15A0_QRd02(pe1_coring_ctrl_01, reg_edge_alpha_mul,	pp->reg_edge_alpha_mul,\
														reg_edge_beta_mul,	pp->reg_edge_beta_mul);
				PE_SR_SHP_H15A0_QRd04(pe1_coring_ctrl_02,	reg_a_lut_edge_x0,	pp->reg_a_lut_edge_x0,\
														reg_a_lut_edge_y0,	pp->reg_a_lut_edge_y0,\
														reg_a_lut_edge_x1,	pp->reg_a_lut_edge_x1,\
														reg_a_lut_edge_y1,	pp->reg_a_lut_edge_y1);
				PE_SR_SHP_H15A0_QRd04(pe1_coring_ctrl_03,	reg_a_lut_detail_x0,pp->reg_a_lut_detail_x0,\
														reg_a_lut_detail_y0,pp->reg_a_lut_detail_y0,\
														reg_a_lut_detail_x1,pp->reg_a_lut_detail_x1,\
														reg_a_lut_detail_y1,pp->reg_a_lut_detail_y1);
				PE_SR_SHP_H15A0_QRd04(pe1_coring_ctrl_06, reg_edge_minmax_x0,		pp->reg_edge_minmax_x0,\
															reg_edge_minmax_y0,	pp->reg_edge_minmax_y0,\
															reg_edge_minmax_x1,	pp->reg_edge_minmax_x1,\
															reg_edge_minmax_y1,	pp->reg_edge_minmax_y1);
				PE_SR_SHP_H15A0_QRd02(pe1_coring_ctrl_00, reg_t_cut_resolution,	pp->reg_t_cut_resolution,\
														   reg_t_nomalize_en,	pp->reg_t_nomalize_en);
				PE_SR_SHP_H15A0_QRd02(pe1_coring_ctrl_01, reg_detail_alpha_mul,	pp->reg_detail_alpha_mul,\
														   reg_detail_beta_mul, pp->reg_detail_beta_mul);
				PE_SR_SHP_H15A0_QRd04(pe1_coring_ctrl_04, reg_t_lut_edge_x0,		pp->reg_t_lut_edge_x0,\
														reg_t_lut_edge_y0,	pp->reg_t_lut_edge_y0,\
														reg_t_lut_edge_x1,	pp->reg_t_lut_edge_x1,\
														reg_t_lut_edge_y1,	pp->reg_t_lut_edge_y1);
				PE_SR_SHP_H15A0_QRd04(pe1_coring_ctrl_05, reg_t_lut_detail_x0,pp->reg_t_lut_detail_x0,\
														reg_t_lut_detail_y0,pp->reg_t_lut_detail_y0,\
														reg_t_lut_detail_x1,pp->reg_t_lut_detail_x1,\
														reg_t_lut_detail_y1,pp->reg_t_lut_detail_y1);
				PE_SR_SHP_H15A0_QRd04(pe1_coring_ctrl_07, reg_detail_minmax_x0,	pp->reg_detail_minmax_x0,\
														reg_detail_minmax_y0,	pp->reg_detail_minmax_y0,\
														reg_detail_minmax_x1,	pp->reg_detail_minmax_x1,\
														reg_detail_minmax_y1,	pp->reg_detail_minmax_y1);
				/* blc*/
				PE_SR_SHP_H15A0_QRd04(pe1_mp_ctrl_07, gb_en,	pp->mp_gb_en,\
													gb_mode,pp->mp_gb_mode,\
													gb_x1,	pp->mp_gb_x1,\
													gb_y1,	pp->mp_gb_y1);
				PE_SR_SHP_H15A0_QRd03(pe1_mp_ctrl_08 ,gb_x2,	pp->mp_gb_x2,\
													gb_y2, 	pp->mp_gb_y2,\
													gb_y3,	pp->mp_gb_y3);
				PE_SR_SHP_H15A0_QRd04(pe1_mp_ctrl_09, lum1_x_l0,	pp->mp_lum1_x_L0,\
													lum1_x_l1,	pp->mp_lum1_x_L1,\
													lum1_x_h0,	pp->mp_lum1_x_H0,\
													lum1_x_h1,	pp->mp_lum1_x_H1);
				PE_SR_SHP_H15A0_QRd04(pe1_mp_ctrl_0a, lum1_y0,	pp->mp_lum1_y0,\
													lum1_y1,	pp->mp_lum1_y1,\
													lum1_y2,	pp->mp_lum1_y2,\
													lum2_x_l0,	pp->mp_lum2_x_L0);
				PE_SR_SHP_H15A0_QRd04(pe1_mp_ctrl_0b, lum2_x_l1,	pp->mp_lum2_x_L1,\
													lum2_x_h0,	pp->mp_lum2_x_H0,\
													lum2_x_h1,	pp->mp_lum2_x_H1,\
													lum2_y0,	pp->mp_lum2_y0);
				PE_SR_SHP_H15A0_QRd02(pe1_mp_ctrl_0c, lum2_y1,	pp->mp_lum2_y1,\
													lum2_y2,	pp->mp_lum2_y2);
				PE_SR_SHP_H15A0_QRd04(pe1_sp_ctrl_07, gb_en,		pp->sp_gb_en,\
													gb_mode,	pp->sp_gb_mode,\
													gb_x1,		pp->sp_gb_x1,\
													gb_y1,		pp->sp_gb_y1);
				PE_SR_SHP_H15A0_QRd03(pe1_sp_ctrl_08, gb_x2,		pp->sp_gb_x2,\
													gb_y2, 		pp->sp_gb_y2,\
													gb_y3,		pp->sp_gb_y3);
				PE_SR_SHP_H15A0_QRd04(pe1_sp_ctrl_09, lum1_x_l0,	pp->sp_lum1_x_L0,\
													lum1_x_l1,	pp->sp_lum1_x_L1,\
													lum1_x_h0,	pp->sp_lum1_x_H0,\
													lum1_x_h1,	pp->sp_lum1_x_H1);
				PE_SR_SHP_H15A0_QRd04(pe1_sp_ctrl_0a, lum1_y0,	pp->sp_lum1_y0,\
													lum1_y1,	pp->sp_lum1_y1,\
													lum1_y2,	pp->sp_lum1_y2,\
													lum2_x_l0,	pp->sp_lum2_x_L0);
				PE_SR_SHP_H15A0_QRd04(pe1_sp_ctrl_0b, lum2_x_l1,	pp->sp_lum2_x_L1,\
													lum2_x_h0,	pp->sp_lum2_x_H0,\
													lum2_x_h1,	pp->sp_lum2_x_H1,\
													lum2_y0,	pp->sp_lum2_y0);
				PE_SR_SHP_H15A0_QRd02(pe1_sp_ctrl_0c, lum2_y1,	pp->sp_lum2_y1,\
													lum2_y2,	pp->sp_lum2_y2);
				/* ti*/
				PE_SR_SHP_H15A0_QRd03(pe1_pti_ctrl_00, reg_ti_enable,		pp->reg_ti_enable_pti_v,\
													 reg_mm_tap_size, 	pp->reg_mm_tap_size_pti_v,\
													 reg_avg_tap_size,	pp->reg_avg_tap_size_pti_v);
				PE_SR_SHP_H15A0_QRd03(pe1_pti_ctrl_01, reg_ti_enable,		pp->reg_ti_enable_pti_h,\
													 reg_mm_tap_size, 	pp->reg_mm_tap_size_pti_h,\
													 reg_avg_tap_size,	pp->reg_avg_tap_size_pti_h);
				PE_SR_SHP_H15A0_QRd03(pe1_sti_ctrl_00, reg_ti_enable,		pp->reg_enable_sti,\
													 reg_mm_tap_size, 	pp->reg_mm_tap_size_sti,\
													 reg_avg_tap_size,	pp->reg_avg_tap_size_sti);
				/* tgn*/
				PE_SR_SHP_H15A0_QRd01(pe1_tgen_ctrl_00, tgen_on_off,		pp->tgen_on_off);
				PE_SR_SHP_H15A0_QRd04(pe1_tgen_ctrl_01, tg_delta_h_bound,	pp->tg_delta_H_bound,\
													  tg_delta_l_bound,	pp->tg_delta_L_bound,\
													  tg_delta_max,		pp->tg_delta_max,\
													  tg_delta_scale,	pp->tg_delta_scale);
				PE_SR_SHP_H15A0_QRd03(pe1_tgen_ctrl_02, tg_rnd_th,		pp->tg_rnd_th,\
													  tg_delta_wcurr, 	pp->tg_delta_wcurr,\
													  tg_lc_ldr_th,		pp->tg_lc_ldr_th);
 				PE_SR_SHP_H15A0_QRd04(pe1_tgen_ctrl_04, tg_dr_ratio,	pp->tg_dr_ratio,\
														tg_dr_th, 		pp->tg_dr_th,\
														tg_mm_offset,	pp->tg_mm_offset,\
														tg_mm_sel_mode,	pp->tg_mm_sel_mode);				
				PE_SR_SHP_H15A0_QRd03(pe1_tgen_ctrl_05, tg_lavg_th,		pp->tg_lavg_th,\
													  tg_gs_rnd,	 	pp->tg_gs_rnd,\
													  tg_gs_mm,			pp->tg_gs_mm);
				PE_SR_SHP_H15A0_QRd04(pe1_tgen_ctrl_09, tg_gain0,			pp->tg_gain0,\
													  tg_gain1,		pp->tg_gain1,\
													  tg_gain2,		pp->tg_gain2,\
													  tg_gain3,		pp->tg_gain3);
				PE_SR_SHP_H15A0_QRd04(pe1_tgen_ctrl_09, tg_gain4,		pp->tg_gain4,\
													  tg_gain5,		pp->tg_gain5,\
													  tg_gain6,		pp->tg_gain6,\
													  tg_gain7,		pp->tg_gain7);
			}
			PE_SHP_HW_H15_DBG_PRINT("get[%d] :  resolution misc ctrl\n"\
				" hor : 0x%02x,0x%02x,0x%02x\n"\
				" ver : 0x%02x,0x%02x,0x%02x\n"\
				" cmn : 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" djg : 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" djg : 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" emp : 0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" emp : 0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" emp : 0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" emp : 0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" emp : 0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" tmp : 0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" tmp : 0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" tmp : 0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" tmp : 0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" blc : 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" blc : 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" blc : 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" blc : 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" blc : 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" blc : 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" ptiv: 0x%02x,0x%02x,0x%02x\n"\
				" ptih: 0x%02x,0x%02x,0x%02x\n"\
				" sti : 0x%02x,0x%02x,0x%02x\n"\
				" tgen: 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" tgen: 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" tgen: 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n"\
				" tgen: 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n",\
				pp->win_id, \
				/* h 3 */
				pp->edge_Y_filter_en, pp->e_gain_max, pp->f_gain_max, \
				/* v 3 */
				pp->der_v_en, pp->der_gain_mapping, pp->max_sel, \
				/* cmm 5 */
				pp->mp_laplacian_weight,pp->sp_laplacian_weight, \
				pp->flat_filter_en,pp->flat_filter_type,pp->flat_filter_gain,\
				/*djeg 12*/
				pp->edf_en,pp->center_blur_mode,pp->count_diff_th,pp->n_avg_mode,\
				pp->line_variation_diff_threshold,\
				pp->level_th,pp->protect_th,pp->n_avg_gain,\
				pp->reg_g0_cnt_min,pp->reg_g0_mul,pp->reg_g1_protect_min,pp->reg_g1_mul,\
				/*amap 20*/
				pp->reg_cross_th_sc,pp->reg_cross_th,pp->reg_mul_base_t,pp->reg_mul_base_e,\
				pp->reg_a_map_h_size,pp->reg_a_nomalize_en,pp->reg_edge_alpha_mul	,pp->reg_edge_beta_mul,\
				pp->reg_a_lut_edge_x0,pp->reg_a_lut_edge_y0,pp->reg_a_lut_edge_x1,pp->reg_a_lut_edge_y1,\
				pp->reg_a_lut_detail_x0,pp->reg_a_lut_detail_y0,pp->reg_a_lut_detail_x1,pp->reg_a_lut_detail_y1,\
				pp->reg_edge_minmax_x0,pp->reg_edge_minmax_y0,pp->reg_edge_minmax_x1,pp->reg_edge_minmax_y1,\
				/* tmap 16*/
				pp->reg_t_cut_resolution,pp->reg_t_nomalize_en,pp->reg_detail_alpha_mul,pp->reg_detail_beta_mul,\
				pp->reg_t_lut_edge_x0,pp->reg_t_lut_edge_y0,pp->reg_t_lut_edge_x1,pp->reg_t_lut_edge_y1,\
				pp->reg_t_lut_detail_x0,pp->reg_t_lut_detail_y0,pp->reg_t_lut_detail_x1,pp->reg_t_lut_detail_y1,\
				pp->reg_detail_minmax_x0,pp->reg_detail_minmax_y0,pp->reg_detail_minmax_x1,pp->reg_detail_minmax_y1,\
				/* balnce 42*/
				pp->mp_gb_en,pp->mp_gb_mode,\
				pp->mp_gb_x1,pp->mp_gb_x2,pp->mp_gb_y1,pp->mp_gb_y2,pp->mp_gb_y3,\
				pp->sp_gb_en,pp->sp_gb_mode,\
				pp->sp_gb_x1,pp->sp_gb_x2,pp->sp_gb_y1,pp->sp_gb_y2,pp->sp_gb_y3,\
				pp->mp_lum1_x_L0,pp->mp_lum1_x_L1,pp->mp_lum1_x_H0,pp->mp_lum1_x_H1,\
				pp->mp_lum1_y0,pp->mp_lum1_y1,pp->mp_lum1_y2,\
				pp->mp_lum2_x_L0,pp->mp_lum2_x_L1,pp->mp_lum2_x_H0,pp->mp_lum2_x_H1,\
				pp->mp_lum2_y0,pp->mp_lum2_y1,pp->mp_lum2_y2,\
				pp->sp_lum1_x_L0,pp->sp_lum1_x_L1,pp->sp_lum1_x_H0,pp->sp_lum1_x_H1,\
				pp->sp_lum1_y0,pp->sp_lum1_y1,pp->sp_lum1_y2,\
				pp->sp_lum2_x_L0,pp->sp_lum2_x_L1,pp->sp_lum2_x_H0,pp->sp_lum2_x_H1,\
				pp->sp_lum2_y0,pp->sp_lum2_y1,pp->sp_lum2_y2,\
				/* ti 9*/
				pp->reg_ti_enable_pti_v,pp->reg_mm_tap_size_pti_v,pp->reg_avg_tap_size_pti_v,\
				pp->reg_ti_enable_pti_h,pp->reg_mm_tap_size_pti_h,pp->reg_avg_tap_size_pti_h,\
				pp->reg_enable_sti,pp->reg_mm_tap_size_sti,pp->reg_avg_tap_size_sti,\
				/* tgen 23*/
				pp->tgen_on_off,pp->tg_delta_H_bound,pp->tg_delta_L_bound,pp->tg_delta_max,pp->tg_delta_scale,\
				pp->tg_rnd_th,pp->tg_delta_wcurr,pp->tg_lc_ldr_th,\
				pp->tg_dr_ratio,pp->tg_dr_th,pp->tg_mm_offset,pp->tg_mm_sel_mode,\
				pp->tg_lavg_th,pp->tg_gs_rnd,pp->tg_gs_mm,\
				pp->tg_gain0,pp->tg_gain1,pp->tg_gain2,pp->tg_gain3,\
				pp->tg_gain4,pp->tg_gain5,pp->tg_gain6,pp->tg_gain7);
		}
		else
		{
			PE_SHP_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * set sr edge enhance control
 * - use input struct LX_PE_SHP_SR_EE_T for H15X
 *
 * @param   *pstParams [in] void
 * @return  OK if success, ERROR otherwise.
 * @see		function description
 * @author
 */
int PE_SHP_HW_H15_SetSreEdgeEnhanceCtrl(void *pstParams)
{
	int ret = RET_OK;
	#ifdef PE_SHP_HW_H15_SR_PWD_TRACE
	UINT32 r_data[2];
	#endif
	do {
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H15)
		{
			LX_PE_SHP_SR_EE_T *pp=(LX_PE_SHP_SR_EE_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			PE_SHP_HW_H15_DBG_PRINT("set[%d]: mp g_b,w:%d,%d, sp g_b,w:%d,%d\n",pp->win_id,\
				pp->mp_edge_gain_b,pp->mp_edge_gain_w,pp->sp_edge_gain_b,pp->sp_edge_gain_w);
			if(PE_CHECK_WIN0(pp->win_id))
			{
				#ifdef PE_SHP_HW_H15_SR_PWD_TRACE
				CTOP_CTRL_H15A0_RdFL(SR,ctr00);
				CTOP_CTRL_H15A0_Rd01(SR,ctr00,cg_sr_en,r_data[0]);
				CTOP_CTRL_H15A0_Rd01(SR,ctr00,cg_shs_en,r_data[1]);
				PE_SHP_HW_H15_DBG_SRPD_TRACE("S cg sr,shs:%d,%d\n",r_data[0],r_data[1]);
				#endif
				#if 0
				PE_FWI_H15_EDGE_GAIN fw_param;
				fw_param.co09_gain_b = GET_BITS(pp->mp_edge_gain_b,0,7);
				fw_param.co09_gain_w = GET_BITS(pp->mp_edge_gain_w,0,7);
				fw_param.dp01_gain_b = GET_BITS(pp->sp_edge_gain_b,0,7);
				fw_param.dp01_gain_w = GET_BITS(pp->sp_edge_gain_w,0,7);
				ret = PE_FWI_H15_SetEdgeGainCtrl(&fw_param);
				PE_SHP_HW_H15_CHECK_CODE(ret, break, "[%s,%d] PE_FWI_H15_SetEdgeGainCtrl() error.\n", __F__, __L__);
				#endif
				#ifdef PE_SHP_HW_H15_SRE_SHP_WR_ONLY_MD
				#ifdef PE_SHP_HW_H15A_SET_FW_PARAM_TO_REG_DIRECTLY
				PE_SHP_HW_H15A_SR_SHP_WR02(pe1_coring_ctrl_09,edge_gain_b,GET_BITS(pp->mp_edge_gain_b,0,7), \
                                                              edge_gain_w,GET_BITS(pp->mp_edge_gain_w,0,7));
				PE_SHP_HW_H15A_SR_SHP_WR02(pe1_dp_ctrl_01,    edge_gain_b,GET_BITS(pp->sp_edge_gain_b,0,7), \
                                                              edge_gain_w,GET_BITS(pp->sp_edge_gain_w,0,7));
				#else
				PE_SHP_HW_H15A_SRS_SET_USER(pe1_coring_ctrl_09,edge_gain_b,GET_BITS(pp->mp_edge_gain_b,0,7));
				PE_SHP_HW_H15A_SRS_SET_USER(pe1_coring_ctrl_09,edge_gain_w,GET_BITS(pp->mp_edge_gain_w,0,7));
				PE_SHP_HW_H15A_SRS_SET_USER(pe1_dp_ctrl_01,    edge_gain_b,GET_BITS(pp->sp_edge_gain_b,0,7));
				PE_SHP_HW_H15A_SRS_SET_USER(pe1_dp_ctrl_01,    edge_gain_w,GET_BITS(pp->sp_edge_gain_w,0,7));
				#endif
				#else	//#ifdef PE_SHP_HW_H15_SRE_SHP_WR_ONLY_MD
				#ifndef PE_HW_H15_PROTECT_OVERLAP_SETTINGS
				#ifdef PE_SHP_HW_H15A_SET_FW_PARAM_TO_REG_DIRECTLY
				PE_SHP_HW_H15A_SR_SHP_WR02(pe1_coring_ctrl_09,edge_gain_b,GET_BITS(pp->mp_edge_gain_b,0,7), \
                                                              edge_gain_w,GET_BITS(pp->mp_edge_gain_w,0,7));
				PE_SHP_HW_H15A_SR_SHP_WR02(pe1_dp_ctrl_01,    edge_gain_b,GET_BITS(pp->sp_edge_gain_b,0,7), \
                                                              edge_gain_w,GET_BITS(pp->sp_edge_gain_w,0,7));
				#else
				PE_SHP_HW_H15A_SRS_SET_USER(pe1_coring_ctrl_09,edge_gain_b,GET_BITS(pp->mp_edge_gain_b,0,7));
				PE_SHP_HW_H15A_SRS_SET_USER(pe1_coring_ctrl_09,edge_gain_w,GET_BITS(pp->mp_edge_gain_w,0,7));
				PE_SHP_HW_H15A_SRS_SET_USER(pe1_dp_ctrl_01,    edge_gain_b,GET_BITS(pp->sp_edge_gain_b,0,7));
				PE_SHP_HW_H15A_SRS_SET_USER(pe1_dp_ctrl_01,    edge_gain_w,GET_BITS(pp->sp_edge_gain_w,0,7));
				#endif
				#else
				#ifdef PE_SHP_HW_H15A_SET_FW_PARAM_TO_REG_DIRECTLY
				PE_SR_SHP_H15A0_QWr02(pe1_coring_ctrl_09,edge_gain_b,GET_BITS(pp->mp_edge_gain_b,0,7), \
														 edge_gain_w,GET_BITS(pp->mp_edge_gain_w,0,7));
				PE_SR_SHP_H15A0_QWr02(pe1_dp_ctrl_01,	 edge_gain_b,GET_BITS(pp->sp_edge_gain_b,0,7), \
														 edge_gain_w,GET_BITS(pp->sp_edge_gain_w,0,7));
				#endif
				#endif
				#endif	//#ifdef PE_SHP_HW_H15_SRE_SHP_WR_ONLY_MD
				#ifdef PE_SHP_HW_H15_SR_PWD_TRACE
				CTOP_CTRL_H15A0_RdFL(SR,ctr00);
				CTOP_CTRL_H15A0_Rd01(SR,ctr00,cg_sr_en,r_data[0]);
				CTOP_CTRL_H15A0_Rd01(SR,ctr00,cg_shs_en,r_data[1]);
				PE_SHP_HW_H15_DBG_SRPD_TRACE("E cg sr,shs:%d,%d\n",r_data[0],r_data[1]);
				#endif
			}
		}
		else
		{
			PE_SHP_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * get sr edge enhance control
 * - use input struct LX_PE_SHP_EE1_T for H15
 *
 * @param   *pstParams [in/out] void
 * @return  OK if success, ERROR otherwise.
 * @see		function description
 * @author
 */
int PE_SHP_HW_H15_GetSreEdgeEnhanceCtrl(void *pstParams)
{
	int ret = RET_OK;
	LX_PE_WIN_ID win_id;
	do {
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H15)
		{
			LX_PE_SHP_SR_EE_T *pp=(LX_PE_SHP_SR_EE_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			win_id = PE_GET_CHECKED_WINID(pp->win_id);
			if(PE_CHECK_WIN0(win_id))
			{
				#if 0
				PE_FWI_H15_SETTINGS_T fw_inf;
				ret = PE_FWI_H15_GetCurFwiSettings(&fw_inf);
				PE_SHP_HW_H15_CHECK_CODE(ret, break, \
					"[%s,%d] PE_FWI_H15_GetCurFwiSettings() error.\n", __F__, __L__);
				PE_SHP_HW_H15_DBG_PRINT("fw_ctrl.enable : %s.\n", \
					fw_inf.fw_ctrl.enable? "yes":"no");
				#endif
				#ifdef PE_SHP_HW_H15A_SET_FW_PARAM_TO_REG_DIRECTLY
				PE_SR_SHP_H15A0_QRd02(pe1_coring_ctrl_09,edge_gain_b,pp->mp_edge_gain_b,\
				                                         edge_gain_w,pp->mp_edge_gain_w);
				PE_SR_SHP_H15A0_QRd02(pe1_dp_ctrl_01,edge_gain_b,pp->sp_edge_gain_b,\
				                                     edge_gain_w,pp->sp_edge_gain_w);
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
					PE_SR_SHP_H15A0_QRd02(pe1_coring_ctrl_09,edge_gain_b,pp->mp_edge_gain_b,\
					                                         edge_gain_w,pp->mp_edge_gain_w);
					PE_SR_SHP_H15A0_QRd02(pe1_dp_ctrl_01,edge_gain_b,pp->sp_edge_gain_b,\
					                                     edge_gain_w,pp->sp_edge_gain_w);
				}
				#endif
			}
			PE_SHP_HW_H15_DBG_PRINT("get[%d]: mp g_b,w:%d,%d, sp g_b,w:%d,%d\n",\
				pp->win_id,pp->mp_edge_gain_b,pp->mp_edge_gain_w,\
				pp->sp_edge_gain_b,pp->sp_edge_gain_w);
		}
		else
		{
			PE_SHP_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * set sr detail enhance control
 * - use input struct LX_PE_SHP_SR_DE_T for H15X
 *
 * @param   *pstParams [in] void
 * @return  OK if success, ERROR otherwise.
 * @see		function description
 * @author
 */
int PE_SHP_HW_H15_SetSreDetailEnhanceCtrl(void *pstParams)
{
	int ret = RET_OK;
	PE_FWI_H15_TEXTURE_GAIN fw_param;
	#ifdef PE_SHP_HW_H15_SR_PWD_TRACE
	UINT32 r_data[2];
	#endif
	do {
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H15)
		{
			LX_PE_SHP_SR_DE_T *pp=(LX_PE_SHP_SR_DE_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			PE_SHP_HW_H15_DBG_PRINT("set[%d]: mp g_b,w:%d,%d, sp g_b,w:%d,%d\n",\
				pp->win_id,pp->mp_texture_gain_b,pp->mp_texture_gain_w,\
				pp->sp_texture_gain_b,pp->sp_texture_gain_w);
			if(PE_CHECK_WIN0(pp->win_id))
			{
				#ifdef PE_SHP_HW_H15_SR_PWD_TRACE
				CTOP_CTRL_H15A0_RdFL(SR,ctr00);
				CTOP_CTRL_H15A0_Rd01(SR,ctr00,cg_sr_en,r_data[0]);
				CTOP_CTRL_H15A0_Rd01(SR,ctr00,cg_shs_en,r_data[1]);
				PE_SHP_HW_H15_DBG_SRPD_TRACE("S cg sr,shs:%d,%d\n",r_data[0],r_data[1]);
				#endif
				fw_param.co09_gain_b = GET_BITS(pp->mp_texture_gain_b,0,7);
				fw_param.co09_gain_w = GET_BITS(pp->mp_texture_gain_w,0,7);
				fw_param.dp01_gain_b = GET_BITS(pp->sp_texture_gain_b,0,7);
				fw_param.dp01_gain_w = GET_BITS(pp->sp_texture_gain_w,0,7);
				ret = PE_FWI_H15_SetTextureGainCtrl(&fw_param);
				PE_SHP_HW_H15_CHECK_CODE(ret, break, \
					"[%s,%d] PE_FWI_H15_SetTextureGainCtrl() error.\n", __F__, __L__);
				#ifdef PE_SHP_HW_H15_SRE_SHP_WR_ONLY_MD
				#ifdef PE_SHP_HW_H15A_SET_FW_PARAM_TO_REG_DIRECTLY
				PE_SHP_HW_H15A_SR_SHP_WR02(pe1_coring_ctrl_09,texture_gain_b,GET_BITS(pp->mp_texture_gain_b,0,7), \
                                                              texture_gain_w,GET_BITS(pp->mp_texture_gain_w,0,7));
				PE_SHP_HW_H15A_SR_SHP_WR02(pe1_dp_ctrl_01,texture_gain_b,GET_BITS(pp->sp_texture_gain_b,0,7), \
                                                          texture_gain_w,GET_BITS(pp->sp_texture_gain_w,0,7));
				#else
				PE_SHP_HW_H15A_SRS_SET_USER(pe1_coring_ctrl_09,texture_gain_b,GET_BITS(pp->mp_texture_gain_b,0,7));
				PE_SHP_HW_H15A_SRS_SET_USER(pe1_coring_ctrl_09,texture_gain_w,GET_BITS(pp->mp_texture_gain_w,0,7));
				PE_SHP_HW_H15A_SRS_SET_USER(pe1_dp_ctrl_01,texture_gain_b,GET_BITS(pp->sp_texture_gain_b,0,7));
				PE_SHP_HW_H15A_SRS_SET_USER(pe1_dp_ctrl_01,texture_gain_w,GET_BITS(pp->sp_texture_gain_w,0,7));
				#endif
				#else	//#ifdef PE_SHP_HW_H15_SRE_SHP_WR_ONLY_MD
				#ifndef PE_HW_H15_PROTECT_OVERLAP_SETTINGS
				#ifdef PE_SHP_HW_H15A_SET_FW_PARAM_TO_REG_DIRECTLY
				PE_SHP_HW_H15A_SR_SHP_WR02(pe1_coring_ctrl_09,texture_gain_b,GET_BITS(pp->mp_texture_gain_b,0,7), \
                                                              texture_gain_w,GET_BITS(pp->mp_texture_gain_w,0,7));
				PE_SHP_HW_H15A_SR_SHP_WR02(pe1_dp_ctrl_01,texture_gain_b,GET_BITS(pp->sp_texture_gain_b,0,7), \
                                                          texture_gain_w,GET_BITS(pp->sp_texture_gain_w,0,7));
				#else
				PE_SHP_HW_H15A_SRS_SET_USER(pe1_coring_ctrl_09,texture_gain_b,GET_BITS(pp->mp_texture_gain_b,0,7));
				PE_SHP_HW_H15A_SRS_SET_USER(pe1_coring_ctrl_09,texture_gain_w,GET_BITS(pp->mp_texture_gain_w,0,7));
				PE_SHP_HW_H15A_SRS_SET_USER(pe1_dp_ctrl_01,texture_gain_b,GET_BITS(pp->sp_texture_gain_b,0,7));
				PE_SHP_HW_H15A_SRS_SET_USER(pe1_dp_ctrl_01,texture_gain_w,GET_BITS(pp->sp_texture_gain_w,0,7));
				#endif
				#else
				#ifdef PE_SHP_HW_H15A_SET_FW_PARAM_TO_REG_DIRECTLY
				PE_SR_SHP_H15A0_QWr02(pe1_coring_ctrl_09,texture_gain_b,GET_BITS(pp->mp_texture_gain_b,0,7), \
														 texture_gain_w,GET_BITS(pp->mp_texture_gain_w,0,7));
				PE_SR_SHP_H15A0_QWr02(pe1_dp_ctrl_01,texture_gain_b,GET_BITS(pp->sp_texture_gain_b,0,7), \
													 texture_gain_w,GET_BITS(pp->sp_texture_gain_w,0,7));
				#endif
				#endif
				#endif	//#ifdef PE_SHP_HW_H15_SRE_SHP_WR_ONLY_MD
				#ifdef PE_SHP_HW_H15_SR_PWD_TRACE
				CTOP_CTRL_H15A0_RdFL(SR,ctr00);
				CTOP_CTRL_H15A0_Rd01(SR,ctr00,cg_sr_en,r_data[0]);
				CTOP_CTRL_H15A0_Rd01(SR,ctr00,cg_shs_en,r_data[1]);
				PE_SHP_HW_H15_DBG_SRPD_TRACE("E cg sr,shs:%d,%d\n",r_data[0],r_data[1]);
				#endif
			}
		}
		else
		{
			PE_SHP_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * get sr detail enhance control
 * - use input struct LX_PE_SHP_SR_DE_T for H15X
 *
 * @param   *pstParams [in/out] void
 * @return  OK if success, ERROR otherwise.
 * @see		function description
 * @author
 */
int PE_SHP_HW_H15_GetSreDetailEnhanceCtrl(void *pstParams)
{
	int ret = RET_OK;
	LX_PE_WIN_ID win_id;
	do {
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H15)
		{
			LX_PE_SHP_SR_DE_T *pp=(LX_PE_SHP_SR_DE_T *)pstParams;
			PE_CHECK_WINID(pp->win_id);
			win_id = PE_GET_CHECKED_WINID(pp->win_id);
			if(PE_CHECK_WIN0(win_id))
			{
				PE_FWI_H15_SETTINGS_T fw_inf;
				ret = PE_FWI_H15_GetCurFwiSettings(&fw_inf);
				PE_SHP_HW_H15_CHECK_CODE(ret, break, \
					"[%s,%d] PE_FWI_H15_GetCurFwiSettings() error.\n", __F__, __L__);
				PE_SHP_HW_H15_DBG_PRINT("fw_ctrl.enable : %s.\n", \
					fw_inf.fw_ctrl.enable? "yes":"no");
				#ifdef PE_SHP_HW_H15A_SET_FW_PARAM_TO_REG_DIRECTLY
				PE_SR_SHP_H15A0_QRd02(pe1_coring_ctrl_09,texture_gain_b,pp->mp_texture_gain_b, \
														texture_gain_w,pp->mp_texture_gain_w);
				PE_SR_SHP_H15A0_QRd02(pe1_dp_ctrl_01,texture_gain_b,pp->sp_texture_gain_b, \
													texture_gain_w,pp->sp_texture_gain_w);
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
					PE_SR_SHP_H15A0_QRd02(pe1_coring_ctrl_09,texture_gain_b,pp->mp_texture_gain_b, \
														texture_gain_w,pp->mp_texture_gain_w);
					PE_SR_SHP_H15A0_QRd02(pe1_dp_ctrl_01,texture_gain_b,pp->sp_texture_gain_b, \
														texture_gain_w,pp->sp_texture_gain_w);
				}
				#endif
			}
			PE_SHP_HW_H15_DBG_PRINT("get[%d]: mp g_b,w:%d,%d, sp g_b,w:%d,%d\n",\
				pp->win_id,pp->mp_texture_gain_b,pp->mp_texture_gain_w,\
				pp->sp_texture_gain_b,pp->sp_texture_gain_w);
		}
		else
		{
			PE_SHP_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
#endif
/**
 * set pbl(pre blur dnr) apply
 *
 * @param   *pCfg [in] PE_TSK_H15_CFG_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_SHP_HW_H15A_SetPblApply(PE_TSK_H15_CFG_T *pCfg)
{
	int ret = RET_OK;
	#ifdef PE_HW_H15_PROTECT_OVERLAP_SETTINGS
	static UINT32 count = 0;
	static UINT32 pre_121_blur = 0xff;
	static UINT32 pre_medi_flt = 0xff;
	UINT32 cur_121_blur, cur_medi_flt;
	#ifndef PE_SHP_HW_H15_SR_PWD_TRACE
	UINT32 tearing_st = 0x0;		//tearing count state
	UINT32 tearing_th = 0x0;		//tering threshold
	#endif
	UINT32 set_mode = PE_SHP_HW_H15A_PRE_BLUR_SET_DFLT;
	LX_PE_INF_DISPLAY_T *disp0_info = &pCfg->inf.disp_info[LX_PE_WIN_0];
	PE_INF_H15_CTRL_T *ctrl0_info = &pCfg->inf.ctrl_info[LX_PE_WIN_0];
	PE_PBL_HW_PARAM_REG_H15A0_T *p_pbl_dflt = _g_pe_pbl_hw_param_data_h15a.reg_dflt.data;
	do{
		CHECK_KNULL(pCfg);
		if(PE_KDRV_VER_H15)
		{
			/* check auto on,off */
			switch (disp0_info->src_type)
			{
				case LX_PE_SRC_CVBS:
				case LX_PE_SRC_SCART:
					if (disp0_info->cstd_type==LX_PE_CSTD_SECAM)
					{
						set_mode = PE_SHP_HW_H15A_PRE_BLUR_SET_DFLT;
					}
					else
					{
						if (ctrl0_info->sat_status>PE_SHP_HW_H15A_PBL_SAT_STATUS_TH)
						{
							set_mode = PE_SHP_HW_H15A_PRE_BLUR_SET_OFF;
						}
						else
						{
							set_mode = PE_SHP_HW_H15A_PRE_BLUR_SET_DFLT;
						}
					}
					break;
				case LX_PE_SRC_COMP:
				case LX_PE_SRC_DTV:
				case LX_PE_SRC_HDMI:
					if (disp0_info->src_type==LX_PE_SRC_DTV && \
						disp0_info->dtv_type==LX_PE_DTV_HDDPLAY && \
						(disp0_info->hdd_type==LX_PE_HDD_SRC_AV60 || \
						disp0_info->hdd_type==LX_PE_HDD_SRC_AV50 || \
						disp0_info->hdd_type==LX_PE_HDD_SRC_SCARTRGB))
					{
						if (ctrl0_info->sat_status>PE_SHP_HW_H15A_PBL_SAT_STATUS_TH)
						{
							set_mode = PE_SHP_HW_H15A_PRE_BLUR_SET_OFF;
						}
						else
						{
							set_mode = PE_SHP_HW_H15A_PRE_BLUR_SET_DFLT;
						}
					}
					else if (disp0_info->src_type==LX_PE_SRC_HDMI && \
						disp0_info->hdmi_type==LX_PE_HDMI_PC)
					{
						set_mode = PE_SHP_HW_H15A_PRE_BLUR_SET_OFF;
					}
					else
					{
						set_mode = PE_SHP_HW_H15A_PRE_BLUR_SET_DFLT;
					}
					break;
				case LX_PE_SRC_ATV:
				case LX_PE_SRC_RGBPC:
				default:
					{
						set_mode = PE_SHP_HW_H15A_PRE_BLUR_SET_DFLT;
					}
					break;
			}
			/* get pre blur ctrl */
			if (set_mode == PE_SHP_HW_H15A_PRE_BLUR_SET_OFF)
			{
				#if 0	// moved to fw ctrl
				PE_DNR1_H15A0_QRd01(dnr_stat_0,c420_tearing_cnt,tearing_st);
				PE_DNR1_H15A0_QRd01(reg_c420_ctrl,reg_tearing_count_th,tearing_th);
				if (tearing_st > tearing_th)
				{
					cur_121_blur = 0;	//temporally off, because of measure diff, i vs. p
					cur_medi_flt = 0;
				}
				else
				{
					cur_121_blur = 0;
					cur_medi_flt = 0;
				}
				#else
				cur_121_blur = 0;
				cur_medi_flt = 0;
				#endif
			}
			else
			{
				cur_121_blur = p_pbl_dflt->c_pre_blur_ctrl_0.pre_121_blur_en;
				cur_medi_flt = p_pbl_dflt->c_pre_blur_ctrl_0.pre_median_en;
			}
			/* compare pre vs. cur */
			if (pre_121_blur != cur_121_blur || pre_medi_flt != cur_medi_flt)
			{
				PE_SHP_HW_H15_DBG_PRINT("[inf] src,fmt:%d,%d, cstd,dtv,hdd,hdmi:%d,%d,%d,%d\n",\
					disp0_info->src_type, disp0_info->fmt_type, disp0_info->cstd_type, \
					disp0_info->dtv_type, disp0_info->hdd_type, disp0_info->hdmi_type);
				PE_SHP_HW_H15_DBG_PRINT("[inf] sat_st:%d, auto_md:%d, tear_st:%d, tear_th:%d\n",\
					ctrl0_info->sat_status, set_mode, tearing_st, tearing_th);
				PE_SHP_HW_H15_DBG_PRINT("[set] 121_blur:%d->%d, medi_flt:%d->%d\n",\
					pre_121_blur, cur_121_blur, pre_medi_flt, cur_medi_flt);
				PE_SHP_HW_H15A_PBL_WR02(c_pre_blur_ctrl_0,pre_121_blur_en,cur_121_blur,pre_median_en,cur_medi_flt);
				PE_CE_PE1_H15A0_QWr02(pe1_fsw_ctrl_01,pre_121_blur_en,cur_121_blur,pre_median_en,cur_medi_flt);
				pre_121_blur = cur_121_blur;
				pre_medi_flt = cur_medi_flt;
			}
			if(_g_shp_hw_h15_trace)
			{
				if(count>PE_PRINT_COUNT_NUM)
				{
					PE_SHP_HW_H15_DBG_PRINT("[inf] src,fmt:%d,%d, cstd,dtv,hdd,hdmi:%d,%d,%d,%d\n",\
						disp0_info->src_type, disp0_info->fmt_type, disp0_info->cstd_type, \
						disp0_info->dtv_type, disp0_info->hdd_type, disp0_info->hdmi_type);
					PE_SHP_HW_H15_DBG_PRINT("[inf] sat_st:%d, auto_md:%d, tear_st:%d, tear_th:%d\n",\
						ctrl0_info->sat_status, set_mode, tearing_st, tearing_th);
					PE_SHP_HW_H15_DBG_PRINT("[set] 121_blur:%d, medi_flt:%d\n",\
						cur_121_blur, cur_medi_flt);
					count=0;
				}
				count++;
			}
		}
		else
		{
			PE_SHP_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	#endif
	return ret;
}
/**
 * download scl db
 *
 * @param   void
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_SHP_HW_H15_DownloadSclDb(void)
{
	int ret = RET_OK;
	UINT32 size = 0, i = 0, sec_typ = 0;
	UINT32 base_line[20];
	PE_DDR_DB_SHDW_REG_H15_T *p_shdw_db = gPE_DDR_DB_H15.shdw.data;
	volatile PE_DDR_DB_PHYS_REG_H15_T *p_phys_db = gPE_DDR_DB_H15.phys.data;
	volatile UINT32 *p_t12_m_data = p_phys_db->t12_m.data;
	volatile UINT32 *p_t12_s_data = p_phys_db->t12_s.data;
	volatile UINT32 *p_t06_m_data = p_phys_db->t06_m.data;
	do{
		CHECK_KNULL(p_shdw_db);
		CHECK_KNULL(p_phys_db);
		CHECK_KNULL(p_t12_m_data);
		CHECK_KNULL(p_t12_s_data);
		CHECK_KNULL(p_t06_m_data);
		if (PE_KDRV_VER_H15)
		{
			/* t06_m **************************************************************/
			sec_typ = PE_DDR_H15_SEC_SCL_06T_M;
			/* header */
			p_shdw_db->sec_d[sec_typ].header.version   = 0x1;
			p_shdw_db->sec_d[sec_typ].header.sec_type  = sec_typ;
			p_shdw_db->sec_d[sec_typ].header.item_num  = PE_DDR_H15_SCL_06T_M_ITEM_NUM;
			p_shdw_db->sec_d[sec_typ].header.item_size = PE_DDR_H15_SCL_M_FLT_SIZE;
			p_phys_db->t06_m.header.version   = p_shdw_db->sec_d[sec_typ].header.version;
			p_phys_db->t06_m.header.sec_type  = p_shdw_db->sec_d[sec_typ].header.sec_type;
			p_phys_db->t06_m.header.item_num  = p_shdw_db->sec_d[sec_typ].header.item_num;
			p_phys_db->t06_m.header.item_size = p_shdw_db->sec_d[sec_typ].header.item_size;
			/* data */
			size = (PE_DDR_H15_SCL_06T_M_ITEM_NUM * PE_DDR_H15_SCL_M_FLT_SIZE * sizeof(UINT32));
			p_shdw_db->top_h.sec_base[sec_typ] = \
				p_shdw_db->top_h.sec_base[sec_typ-1]+p_shdw_db->top_h.sec_size[sec_typ-1];
			p_shdw_db->top_h.sec_size[sec_typ] = size + sizeof(PE_DDR_H15_HEADER_T);
			p_phys_db->top_h.sec_base[sec_typ] = p_shdw_db->top_h.sec_base[sec_typ];
			p_phys_db->top_h.sec_size[sec_typ] = p_shdw_db->top_h.sec_size[sec_typ];
			p_shdw_db->sec_d[sec_typ].data_base = \
				p_shdw_db->top_h.sec_base[sec_typ] + sizeof(PE_DDR_H15_HEADER_T);
			/* wr ddr */
			base_line[0] = 0;
			base_line[1] = base_line[0]+PE_DDR_H15_SCL_M_FLT_SIZE;
			for (i=0; i<PE_DDR_H15_SCL_M_FLT_SIZE; i++)
			{
				p_t06_m_data[base_line[0]+i] = msc_06t_m_flt_0006_h15[i].data;
				p_t06_m_data[base_line[1]+i] = msc_06t_m_flt_0306_h15[i].data;
			}
			/* noti */
			PE_PRINT_NOTI("%s: DL done.(base:0x%08x)\n", \
				PE_DDR_REG_H15_SEC_TYPE_TO_STR(sec_typ), p_shdw_db->top_h.sec_base[sec_typ]);
			/* t12_m **************************************************************/
			sec_typ = PE_DDR_H15_SEC_SCL_12T_M;
			/* header */
			p_shdw_db->sec_d[sec_typ].header.version   = 0x1;
			p_shdw_db->sec_d[sec_typ].header.sec_type  = sec_typ;
			p_shdw_db->sec_d[sec_typ].header.item_num  = PE_DDR_H15_SCL_12T_M_ITEM_NUM;
			p_shdw_db->sec_d[sec_typ].header.item_size = PE_DDR_H15_SCL_M_FLT_SIZE;
			p_phys_db->t12_m.header.version   = p_shdw_db->sec_d[sec_typ].header.version;
			p_phys_db->t12_m.header.sec_type  = p_shdw_db->sec_d[sec_typ].header.sec_type;
			p_phys_db->t12_m.header.item_num  = p_shdw_db->sec_d[sec_typ].header.item_num;
			p_phys_db->t12_m.header.item_size = p_shdw_db->sec_d[sec_typ].header.item_size;
			/* data */
			size = (PE_DDR_H15_SCL_12T_M_ITEM_NUM * PE_DDR_H15_SCL_M_FLT_SIZE * sizeof(UINT32));
			p_shdw_db->top_h.sec_base[sec_typ] = \
				p_shdw_db->top_h.sec_base[sec_typ-1]+p_shdw_db->top_h.sec_size[sec_typ-1];
			p_shdw_db->top_h.sec_size[sec_typ] = size + sizeof(PE_DDR_H15_HEADER_T);
			p_phys_db->top_h.sec_base[sec_typ] = p_shdw_db->top_h.sec_base[sec_typ];
			p_phys_db->top_h.sec_size[sec_typ] = p_shdw_db->top_h.sec_size[sec_typ];
			p_shdw_db->sec_d[sec_typ].data_base = \
				p_shdw_db->top_h.sec_base[sec_typ] + sizeof(PE_DDR_H15_HEADER_T);
			/* wr ddr */
			base_line[0]  = 0;
			base_line[1]  = base_line[0]+PE_DDR_H15_SCL_M_FLT_SIZE;
			base_line[2]  = base_line[1]+PE_DDR_H15_SCL_M_FLT_SIZE;
			base_line[3]  = base_line[2]+PE_DDR_H15_SCL_M_FLT_SIZE;
			base_line[4]  = base_line[3]+PE_DDR_H15_SCL_M_FLT_SIZE;
			base_line[5]  = base_line[4]+PE_DDR_H15_SCL_M_FLT_SIZE;
			base_line[6]  = base_line[5]+PE_DDR_H15_SCL_M_FLT_SIZE;
			base_line[7]  = base_line[6]+PE_DDR_H15_SCL_M_FLT_SIZE;
			base_line[8]  = base_line[7]+PE_DDR_H15_SCL_M_FLT_SIZE;
			base_line[9]  = base_line[8]+PE_DDR_H15_SCL_M_FLT_SIZE;
			base_line[10] = base_line[9]+PE_DDR_H15_SCL_M_FLT_SIZE;
			for (i=0; i<PE_DDR_H15_SCL_M_FLT_SIZE; i++)
			{
				p_t12_m_data[base_line[0]+i]  = msc_12t_m_flt_0002_h15[i].data;
				p_t12_m_data[base_line[1]+i]  = msc_12t_m_flt_0003_h15[i].data;
				p_t12_m_data[base_line[2]+i]  = msc_12t_m_flt_0006_h15[i].data;
				p_t12_m_data[base_line[3]+i]  = msc_12t_m_flt_0304_h15[i].data;
				p_t12_m_data[base_line[4]+i]  = msc_12t_m_flt_0306_h15[i].data;
				p_t12_m_data[base_line[5]+i]  = msc_12t_m_flt_0404_h15[i].data;
				p_t12_m_data[base_line[6]+i]  = msc_12t_m_flt_0406_h15[i].data;
				p_t12_m_data[base_line[7]+i]  = msc_12t_m_flt_0606_h15[i].data;
				p_t12_m_data[base_line[8]+i]  = msc_12t_m_flt_0806_h15[i].data;
				p_t12_m_data[base_line[9]+i]  = msc_12t_m_flt_1006_h15[i].data;
				p_t12_m_data[base_line[10]+i] = msc_12t_m_flt_1410_h15[i].data;
			}
			/* noti */
			PE_PRINT_NOTI("%s: DL done.(base:0x%08x)\n", \
				PE_DDR_REG_H15_SEC_TYPE_TO_STR(sec_typ), p_shdw_db->top_h.sec_base[sec_typ]);
			/* t12_s **************************************************************/
			sec_typ = PE_DDR_H15_SEC_SCL_12T_S;
			/* header */
			p_shdw_db->sec_d[sec_typ].header.version   = 0x1;
			p_shdw_db->sec_d[sec_typ].header.sec_type  = sec_typ;
			p_shdw_db->sec_d[sec_typ].header.item_num  = PE_DDR_H15_SCL_12T_S_ITEM_NUM;
			p_shdw_db->sec_d[sec_typ].header.item_size = PE_DDR_H15_SCL_S_FLT_SIZE;
			p_phys_db->t12_s.header.version   = p_shdw_db->sec_d[sec_typ].header.version;
			p_phys_db->t12_s.header.sec_type  = p_shdw_db->sec_d[sec_typ].header.sec_type;
			p_phys_db->t12_s.header.item_num  = p_shdw_db->sec_d[sec_typ].header.item_num;
			p_phys_db->t12_s.header.item_size = p_shdw_db->sec_d[sec_typ].header.item_size;
			/* data */
			size = (PE_DDR_H15_SCL_12T_S_ITEM_NUM * PE_DDR_H15_SCL_S_FLT_SIZE * sizeof(UINT32));
			p_shdw_db->top_h.sec_base[sec_typ] = \
				p_shdw_db->top_h.sec_base[sec_typ-1]+p_shdw_db->top_h.sec_size[sec_typ-1];
			p_shdw_db->top_h.sec_size[sec_typ] = size + sizeof(PE_DDR_H15_HEADER_T);
			p_phys_db->top_h.sec_base[sec_typ] = p_shdw_db->top_h.sec_base[sec_typ];
			p_phys_db->top_h.sec_size[sec_typ] = p_shdw_db->top_h.sec_size[sec_typ];
			p_shdw_db->sec_d[sec_typ].data_base = \
				p_shdw_db->top_h.sec_base[sec_typ] + sizeof(PE_DDR_H15_HEADER_T);
			/* wr ddr */
			base_line[0] = 0;
			base_line[1] = base_line[0]+PE_DDR_H15_SCL_S_FLT_SIZE;
			for (i=0; i<PE_DDR_H15_SCL_S_FLT_SIZE; i++)
			{
				p_t12_s_data[base_line[0]+i] = msc_12t_s_flt_0610_h15[i].data;
				p_t12_s_data[base_line[1]+i] = msc_12t_s_flt_1210_h15[i].data;
			}
			/* noti */
			PE_PRINT_NOTI("%s: DL done.(base:0x%08x)\n", \
				PE_DDR_REG_H15_SEC_TYPE_TO_STR(sec_typ), p_shdw_db->top_h.sec_base[sec_typ]);
		}
		else
		{
			PE_SHP_HW_H15_DBG_PRINT("nothing to do.\n");
			ret = RET_OK;
		}
	}while (0);
	return ret;
}

