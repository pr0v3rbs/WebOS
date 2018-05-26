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

/** @file pe_dnt_hw_h15.c
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

#include "pe_hw_h15.h"
#include "pe_reg_h15.h"
#include "pe_fwi_h15.h"
#include "pe_dnt_hw_h15.h"
#include "pe_ipc_hw_param_h15.h"
#include "pe_clc_hw_param_h15.h"
#include "pe_vflt_y_hw_param_h15.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
/* define until film mode f/w ready, H15A0 */
#undef PE_DNT_HW_H15A_SET_FMD_REG

/* undef until clc_en f/w ready */
#undef PE_HW_H15A_USE_FSW_CLC_ENABLE

/* use pre processing for setting ipc reg table */
#define PE_DNT_HW_H15A_USE_PRE_PROC_IPC_REG_TABLE

/* apply vfilter y on dtv 1080p  */
#define PE_DNT_HW_H15A_APPLY_VFLT_Y_ON_DTV_1080P

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define PE_DNT_HW_H15_ERROR		PE_PRINT_ERROR

#define PE_DNT_HW_H15_DBG_PRINT(fmt, args...)	\
	if(_g_dnt_hw_h15_trace)\
	{\
		printk("[%x,%x][%s,%d] "fmt, \
			PE_CHIP_VER, g_pe_kdrv_ver_mask, __F__, __L__, ##args);\
	}

#define PE_DNT_HW_H15_CHECK_CODE(_checker, _action, fmt, args...)	\
	{if(_checker){PE_DNT_HW_H15_ERROR(fmt, ##args);_action;}}

#define PE_DNT_HW_H15A_DBG_PRINT_IPC_FMT_STATE(_pre, _cur)	\
	{\
		char _str_buf[PE_TRACE_STR_SIZE];\
		sprintf(_str_buf, "%s(%s)->(%s):%s", #_cur, \
			PE_DNT_HW_H15A_IPC_FMT_TO_STR(_pre), \
			PE_DNT_HW_H15A_IPC_FMT_TO_STR(_cur), \
			((_pre)!=(_cur))? "different":"same");\
		PE_DNT_HW_H15_DBG_PRINT("%s\n", _str_buf);\
		if((_pre)!=(_cur))\
		{\
			PE_INF_H15_SetTraceDataBuf(_str_buf);\
		}\
	}
#define PE_DNT_HW_H15A_DBG_PRINT_CLC_FMT_STATE(_pre, _cur)	\
	{\
		char _str_buf[PE_TRACE_STR_SIZE];\
		sprintf(_str_buf, "%s(%s)->(%s):%s", #_cur, \
			PE_DNT_HW_H15A_CLC_FMT_TO_STR(_pre), \
			PE_DNT_HW_H15A_CLC_FMT_TO_STR(_cur), \
			((_pre)!=(_cur))? "different":"same");\
		PE_DNT_HW_H15_DBG_PRINT("%s\n", _str_buf);\
		if((_pre)!=(_cur))\
		{\
			PE_INF_H15_SetTraceDataBuf(_str_buf);\
		}\
	}
#define PE_DNT_HW_H15A_DBG_PRINT_VFY_FMT_STATE(_pre, _cur)	\
	{\
		char _str_buf[PE_TRACE_STR_SIZE];\
		sprintf(_str_buf, "%s(%s)->(%s):%s", #_cur, \
			PE_DNT_HW_H15A_VFY_FMT_TO_STR(_pre), \
			PE_DNT_HW_H15A_VFY_FMT_TO_STR(_cur), \
			((_pre)!=(_cur))? "different":"same");\
		PE_DNT_HW_H15_DBG_PRINT("%s\n", _str_buf);\
		if((_pre)!=(_cur))\
		{\
			PE_INF_H15_SetTraceDataBuf(_str_buf);\
		}\
	}

#ifdef PE_HW_H15_PROTECT_OVERLAP_SETTINGS
#define PE_DNT_HW_H15A_IPC_SET_USER(_r, _f, _d)	\
	{	_g_pe_ipc_hw_param_data_h15a.reg_user.data->_r._f = (_d);\
		_g_pe_ipc_hw_param_data_h15a.reg_mask.data->_r._f = 0;}

#define PE_DNT_HW_H15A_IPC_WR01(_r, _f1, _d1)	\
	{	PE_ND12_H15A0_QWr01(_r, _f1, _d1);\
		PE_DNT_HW_H15A_IPC_SET_USER(_r, _f1, _d1);}
#define PE_DNT_HW_H15A_IPC_WR02(_r, _f1, _d1, _f2, _d2)	\
	{	PE_ND12_H15A0_QWr02(_r, _f1, _d1, _f2, _d2);\
		PE_DNT_HW_H15A_IPC_SET_USER(_r, _f1, _d1);\
		PE_DNT_HW_H15A_IPC_SET_USER(_r, _f2, _d2);}
#define PE_DNT_HW_H15A_IPC_WR03(_r, _f1, _d1, _f2, _d2, _f3, _d3)	\
	{	PE_ND12_H15A0_QWr03(_r, _f1, _d1, _f2, _d2, _f3, _d3);\
		PE_DNT_HW_H15A_IPC_SET_USER(_r, _f1, _d1);\
		PE_DNT_HW_H15A_IPC_SET_USER(_r, _f2, _d2);\
		PE_DNT_HW_H15A_IPC_SET_USER(_r, _f3, _d3);}
#define PE_DNT_HW_H15A_IPC_WR04(_r, _f1, _d1, _f2, _d2, _f3, _d3, _f4, _d4)	\
	{	PE_ND12_H15A0_QWr04(_r, _f1, _d1, _f2, _d2, _f3, _d3, _f4, _d4);\
		PE_DNT_HW_H15A_IPC_SET_USER(_r, _f1, _d1);\
		PE_DNT_HW_H15A_IPC_SET_USER(_r, _f2, _d2);\
		PE_DNT_HW_H15A_IPC_SET_USER(_r, _f3, _d3);\
		PE_DNT_HW_H15A_IPC_SET_USER(_r, _f4, _d4);}
#define PE_DNT_HW_H15A_IPC_WR05(_r, _f1, _d1, _f2, _d2, _f3, _d3, _f4, _d4, _f5, _d5)	\
	{	PE_ND12_H15A0_QWr05(_r, _f1, _d1, _f2, _d2, _f3, _d3, _f4, _d4, _f5, _d5);\
		PE_DNT_HW_H15A_IPC_SET_USER(_r, _f1, _d1);\
		PE_DNT_HW_H15A_IPC_SET_USER(_r, _f2, _d2);\
		PE_DNT_HW_H15A_IPC_SET_USER(_r, _f3, _d3);\
		PE_DNT_HW_H15A_IPC_SET_USER(_r, _f4, _d4);\
		PE_DNT_HW_H15A_IPC_SET_USER(_r, _f5, _d5);}

#define PE_DNT_HW_H15A_CLC_SET_USER(_r, _f, _d)	\
	{	_g_pe_clc_hw_param_data_h15a.reg_user.data->_r._f = (_d);\
		_g_pe_clc_hw_param_data_h15a.reg_mask.data->_r._f = 0;}

#define PE_DNT_HW_H15A_CLC_WR01(_r, _f1, _d1)	\
	{	PE_ND12_H15A0_QWr01(_r, _f1, _d1);\
		PE_DNT_HW_H15A_CLC_SET_USER(_r, _f1, _d1);}
#define PE_DNT_HW_H15A_CLC_WR02(_r, _f1, _d1, _f2, _d2)	\
	{	PE_ND12_H15A0_QWr02(_r, _f1, _d1, _f2, _d2);\
		PE_DNT_HW_H15A_CLC_SET_USER(_r, _f1, _d1);\
		PE_DNT_HW_H15A_CLC_SET_USER(_r, _f2, _d2);}
#define PE_DNT_HW_H15A_CLC_WR03(_r, _f1, _d1, _f2, _d2, _f3, _d3)	\
	{	PE_ND12_H15A0_QWr03(_r, _f1, _d1, _f2, _d2, _f3, _d3);\
		PE_DNT_HW_H15A_CLC_SET_USER(_r, _f1, _d1);\
		PE_DNT_HW_H15A_CLC_SET_USER(_r, _f2, _d2);\
		PE_DNT_HW_H15A_CLC_SET_USER(_r, _f3, _d3);}
#define PE_DNT_HW_H15A_CLC_WR04(_r, _f1, _d1, _f2, _d2, _f3, _d3, _f4, _d4)	\
	{	PE_ND12_H15A0_QWr04(_r, _f1, _d1, _f2, _d2, _f3, _d3, _f4, _d4);\
		PE_DNT_HW_H15A_CLC_SET_USER(_r, _f1, _d1);\
		PE_DNT_HW_H15A_CLC_SET_USER(_r, _f2, _d2);\
		PE_DNT_HW_H15A_CLC_SET_USER(_r, _f3, _d3);\
		PE_DNT_HW_H15A_CLC_SET_USER(_r, _f4, _d4);}
#define PE_DNT_HW_H15A_CLC_WR05(_r, _f1, _d1, _f2, _d2, _f3, _d3, _f4, _d4, _f5, _d5)	\
	{	PE_ND12_H15A0_QWr05(_r, _f1, _d1, _f2, _d2, _f3, _d3, _f4, _d4, _f5, _d5);\
		PE_DNT_HW_H15A_CLC_SET_USER(_r, _f1, _d1);\
		PE_DNT_HW_H15A_CLC_SET_USER(_r, _f2, _d2);\
		PE_DNT_HW_H15A_CLC_SET_USER(_r, _f3, _d3);\
		PE_DNT_HW_H15A_CLC_SET_USER(_r, _f4, _d4);\
		PE_DNT_HW_H15A_CLC_SET_USER(_r, _f5, _d5);}

/* set table */
#define PE_DNT_HW_H15A_SET_REG_TABLE(__tbl, __base)	\
	do{\
		UINT32 _i, _size;\
		PE_DNT_HW_H15_CHECK_CODE(!_dflt, ret=RET_ERROR;break, \
			"[%s,%d] _dflt is null, not ready.\n", __F__, __L__);\
		PE_DNT_HW_H15_CHECK_CODE(!_user, ret=RET_ERROR;break, \
			"[%s,%d] _user is null, not ready.\n", __F__, __L__);\
		PE_DNT_HW_H15_CHECK_CODE(!_mask, ret=RET_ERROR;break, \
			"[%s,%d] _mask is null, not ready.\n", __F__, __L__);\
		PE_DNT_HW_H15_CHECK_CODE(!_data, ret=RET_ERROR;break, \
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
			PE_DNT_HW_H15_CHECK_CODE(ret, break, \
				"[%s,%d] PE_REG_H15_WrAllocatedAddr() error.\n", __F__, __L__);\
		}\
		if (ret == RET_OK)\
		{\
			PE_DNT_HW_H15_DBG_PRINT("write %s(+ 0x%08x).\n", #__tbl, (__base));\
			ret = PE_REG_SetTraceTable(__F__,__L__, #__tbl, (__base));\
		}\
	}while(0)

/* pre proc ipc table, not to write some reg,bit */
#ifdef PE_DNT_HW_H15A_USE_PRE_PROC_IPC_REG_TABLE
#define PE_DNT_HW_H15A_PRE_PROC_IPC_REG_TABLE(__base)	\
	do{\
		UINT32 _rd_data[4];\
		if ((__base) == PE_H15A_REG_WRITE_BASE)\
		{\
			PE_ND_ND1_H15A0_RdFL(ipc_ctrl_00);\
			PE_ND_ND1_H15A0_RdFL(ipc_ctrl_01);\
			PE_ND_ND1_H15A0_RdFL(ipc_ctrl_06);\
			PE_ND_ND1_H15A0_RdFL(ipc_ctrl_30);\
			PE_ND_ND1_H15A0_Rd01(ipc_ctrl_00, cr_film_mode_enable, _rd_data[0]);\
			PE_ND_ND1_H15A0_Rd01(ipc_ctrl_01, cr_game_mode, _rd_data[1]);\
			PE_ND_ND1_H15A0_Rd01(ipc_ctrl_06, cr_motion_iir_en, _rd_data[2]);\
			PE_ND_ND1_H15A0_Rd01(ipc_ctrl_30, reg_vmc_en, _rd_data[3]);\
			PE_DNT_HW_H15A_IPC_SET_USER(ipc_ctrl_00, cr_film_mode_enable, _rd_data[0]);\
			PE_DNT_HW_H15A_IPC_SET_USER(ipc_ctrl_01, cr_game_mode, _rd_data[1]);\
			PE_DNT_HW_H15A_IPC_SET_USER(ipc_ctrl_06, cr_motion_iir_en, _rd_data[2]);\
			PE_DNT_HW_H15A_IPC_SET_USER(ipc_ctrl_30, reg_vmc_en, _rd_data[3]);\
		}\
	}while(0)
#else
#define PE_DNT_HW_H15A_PRE_PROC_IPC_REG_TABLE(__base)
#endif

/* ipc */
#define PE_DNT_HW_H15A_IPC_REG_TABLE(__tbl, __base)		\
	do{\
		UINT32 *_dflt = _g_pe_ipc_hw_param_data_h15a.reg_dflt.addr;\
		UINT32 *_user = _g_pe_ipc_hw_param_data_h15a.reg_user.addr;\
		UINT32 *_mask = _g_pe_ipc_hw_param_data_h15a.reg_mask.addr;\
		UINT32 *_data = _g_pe_ipc_hw_param_data_h15a.reg_data.addr;\
		PE_DNT_HW_H15A_PRE_PROC_IPC_REG_TABLE(__base);\
		PE_DNT_HW_H15A_SET_REG_TABLE(__tbl, __base);\
	}while (0)

/* pre proc clc table, not to write some reg,bit */
#define PE_DNT_HW_H15A_PRE_PROC_CLC_REG_TABLE(__base)	\
	do{\
		UINT32 _rd_data[2];\
		if ((__base) == PE_H15A_REG_WRITE_BASE)\
		{\
			PE_ND_ND1_H15A0_RdFL(clc_ctrl_00);\
			PE_ND_ND1_H15A0_Rd01(clc_ctrl_00, cl_filter_enable, _rd_data[0]);\
			PE_ND_ND1_H15A0_Rd01(clc_ctrl_00, clc_detection_enable, _rd_data[1]);\
			PE_DNT_HW_H15A_CLC_SET_USER(clc_ctrl_00, cl_filter_enable, _rd_data[0]);\
			PE_DNT_HW_H15A_CLC_SET_USER(clc_ctrl_00, clc_detection_enable, _rd_data[1]);\
		}\
	}while(0)

/* clc */
#define PE_DNT_HW_H15A_CLC_REG_TABLE(__tbl, __base)		\
	do{\
		UINT32 *_dflt = _g_pe_clc_hw_param_data_h15a.reg_dflt.addr;\
		UINT32 *_user = _g_pe_clc_hw_param_data_h15a.reg_user.addr;\
		UINT32 *_mask = _g_pe_clc_hw_param_data_h15a.reg_mask.addr;\
		UINT32 *_data = _g_pe_clc_hw_param_data_h15a.reg_data.addr;\
		PE_DNT_HW_H15A_PRE_PROC_CLC_REG_TABLE(__base);\
		PE_DNT_HW_H15A_SET_REG_TABLE(__tbl, __base);\
	}while (0)

/* vfy */
#define PE_DNT_HW_H15A_VFY_REG_TABLE(__tbl, __base)		\
	do{\
		UINT32 *_dflt = _g_pe_vfy_hw_param_data_h15a.reg_dflt.addr;\
		UINT32 *_user = _g_pe_vfy_hw_param_data_h15a.reg_user.addr;\
		UINT32 *_mask = _g_pe_vfy_hw_param_data_h15a.reg_mask.addr;\
		UINT32 *_data = _g_pe_vfy_hw_param_data_h15a.reg_data.addr;\
		PE_DNT_HW_H15A_SET_REG_TABLE(__tbl, __base);\
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
static PE_DNT_HW_H15A_IPC_FMT PE_DNT_HW_H15A_ConvDispInfoToIpcFmt(LX_PE_INF_DISPLAY_T *disp_inf);
static PE_DNT_HW_H15A_VFY_FMT PE_DNT_HW_H15A_ConvDispInfoToVfyFmt(LX_PE_INF_DISPLAY_T *disp_inf);
static PE_DNT_HW_H15A_CLC_FMT PE_DNT_HW_H15A_ConvDispInfoToClcFmt(LX_PE_INF_DISPLAY_T *disp_inf);
static int PE_DNT_HW_H15A_SetDefault(LX_PE_DEFAULT_SETTINGS_T *pstParams);
#ifdef PE_HW_H15_PROTECT_OVERLAP_SETTINGS
static int PE_DNT_HW_H15A_CreateDataTable(UINT32 index);
#endif

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
static UINT32 _g_dnt_hw_h15_trace=0x0;		//default should be off.
static PE_DNT_HW_H15_SETTINGS_T _g_pe_dnt_hw_h15_info;
#ifdef PE_HW_H15_PROTECT_OVERLAP_SETTINGS
static PE_IPC_HW_PARAM_DATA_H15A0_T _g_pe_ipc_hw_param_data_h15a = {{NULL}, {NULL}, {NULL}, {NULL}};
static PE_CLC_HW_PARAM_DATA_H15A0_T _g_pe_clc_hw_param_data_h15a = {{NULL}, {NULL}, {NULL}, {NULL}};
static PE_VFY_HW_PARAM_DATA_H15A0_T _g_pe_vfy_hw_param_data_h15a = {{NULL}, {NULL}, {NULL}, {NULL}};
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
int PE_DNT_HW_H15_Init(PE_CFG_CTRL_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H15)
		{
			_g_pe_dnt_hw_h15_info.ipc0a_fmt = PE_DNT_H15A_IPC_NUM;
			_g_pe_dnt_hw_h15_info.clc0a_fmt = PE_DNT_H15A_CLC_NUM;
			_g_pe_dnt_hw_h15_info.vfy0a_fmt = PE_DNT_H15A_VFY_NUM;
			_g_pe_dnt_hw_h15_info.force_cbcr_enb = 0x0;
			_g_pe_dnt_hw_h15_info.force_y_max_th = 0x5f;
			PE_DNT_HW_H15_DBG_PRINT("init ipc, tpd.\n");
			#ifdef PE_HW_H15_PROTECT_OVERLAP_SETTINGS
			/* init ipc(nd1,2) */
			ret = PE_DNT_HW_H15A_CreateDataTable(PE_DNT_HW_H15A_TBL_IDX_IPC);
			PE_DNT_HW_H15_CHECK_CODE(ret, break, \
				"PE_DNT_HW_H15A_CreateDataTable() error.\n");
			PE_DNT_HW_H15A_IPC_REG_TABLE(ipc_l_init_h15a0,PE_H15A_REG_WRITE_BASE);
			PE_DNT_HW_H15A_IPC_REG_TABLE(ipc_l_init_h15a0,PE_H15A_ND_ND_1_2_DIFF);
			/* init clc(nd1 only) */
			ret = PE_DNT_HW_H15A_CreateDataTable(PE_DNT_HW_H15A_TBL_IDX_CLC);
			PE_DNT_HW_H15_CHECK_CODE(ret, break, \
				"PE_DNT_HW_H15A_CreateDataTable() error.\n");
			PE_DNT_HW_H15A_CLC_REG_TABLE(clc_l_init_h15a0,PE_H15A_REG_WRITE_BASE);
			/* init vfy(dnr1,2 only) */
			ret = PE_DNT_HW_H15A_CreateDataTable(PE_DNT_HW_H15A_TBL_IDX_VFY);
			PE_DNT_HW_H15_CHECK_CODE(ret, break, \
				"PE_DNT_HW_H15A_CreateDataTable() error.\n");
			PE_DNT_HW_H15A_VFY_REG_TABLE(vflt_y_l_init_h15a0,PE_H15A_REG_WRITE_BASE);
			PE_DNT_HW_H15A_VFY_REG_TABLE(vflt_y_l_init_h15a0,PE_H15A_ND_DNR_12_DIFF);
			#else
			/* init ipc(nd1,2) */
			PE_SET_REG_TABLE(DNT_HW_H15,ipc_l_init_h15a0,PE_H15A_REG_WRITE_BASE);
			PE_SET_REG_TABLE(DNT_HW_H15,ipc_l_init_h15a0,PE_H15A_ND_ND_1_2_DIFF);
			/* init clc(nd1 only) */
			PE_SET_REG_TABLE(DNT_HW_H15,clc_l_init_h15a0,PE_H15A_REG_WRITE_BASE);
			/* init vflt y */
			PE_SET_REG_TABLE(DNT_HW_H15,vflt_y_l_init_h15a0,PE_H15A_REG_WRITE_BASE);
			PE_SET_REG_TABLE(DNT_HW_H15,vflt_y_l_init_h15a0,PE_H15A_ND_DNR_12_DIFF);
			#endif
			/* init nd1,2 tpd force ycbcr */
			PE_ND12_H15A0_RdFL(tpd_ctrl_07);
			PE_ND12_H15A0_RdFL(tpd_ctrl_08);
			PE_ND12_H15A0_RdFL(tpd_ctrl_09);
			PE_ND12_H15A0_Wr01(tpd_ctrl_07, reg_force_y,         0x0);
			PE_ND12_H15A0_Wr01(tpd_ctrl_07, reg_force_y_max_th,  _g_pe_dnt_hw_h15_info.force_y_max_th);
			PE_ND12_H15A0_Wr01(tpd_ctrl_07, reg_force_y_min_th,  0x0);
			PE_ND12_H15A0_Wr01(tpd_ctrl_07, reg_force_y_enable,  0x0);
			PE_ND12_H15A0_Wr01(tpd_ctrl_08, reg_force_cb,        0x200);
			PE_ND12_H15A0_Wr01(tpd_ctrl_08, reg_force_cb_max_th, 0x205);
			PE_ND12_H15A0_Wr01(tpd_ctrl_08, reg_force_cb_min_th, 0x1e0);
			PE_ND12_H15A0_Wr01(tpd_ctrl_08, reg_force_cb_enable, _g_pe_dnt_hw_h15_info.force_cbcr_enb);
			PE_ND12_H15A0_Wr01(tpd_ctrl_09, reg_force_cr,        0x200);
			PE_ND12_H15A0_Wr01(tpd_ctrl_09, reg_force_cr_max_th, 0x205);
			PE_ND12_H15A0_Wr01(tpd_ctrl_09, reg_force_cr_min_th, 0x1e0);
			PE_ND12_H15A0_Wr01(tpd_ctrl_09, reg_force_cr_enable, _g_pe_dnt_hw_h15_info.force_cbcr_enb);
			PE_ND12_H15A0_WrFL(tpd_ctrl_07);
			PE_ND12_H15A0_WrFL(tpd_ctrl_08);
			PE_ND12_H15A0_WrFL(tpd_ctrl_09);
		}
		else
		{
			PE_DNT_HW_H15_DBG_PRINT("nothing to do\n");	ret = RET_OK;
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
 * @see		PE_DNT_HW_H15A_TBL_INDX
 * @author
 */
static int PE_DNT_HW_H15A_CreateDataTable(UINT32 index)
{
	int ret = RET_OK;
	PE_IPC_HW_PARAM_DATA_H15A0_T *p_ipc_h15a = &_g_pe_ipc_hw_param_data_h15a;
	PE_CLC_HW_PARAM_DATA_H15A0_T *p_clc_h15a = &_g_pe_clc_hw_param_data_h15a;
	PE_VFY_HW_PARAM_DATA_H15A0_T *p_vfy_h15a = &_g_pe_vfy_hw_param_data_h15a;

	PE_DNT_HW_H15_CHECK_CODE(index>=PE_DNT_HW_H15A_TBL_IDX_NUM, return RET_ERROR, \
		"[%s,%d] index(%d) invalid.\n", __F__, __L__, index);

	if (index == PE_DNT_HW_H15A_TBL_IDX_IPC)
	{
		/* create dflt table */
		if (p_ipc_h15a->reg_dflt.addr == NULL)
		{
			p_ipc_h15a->reg_dflt.addr = (UINT32 *)OS_KMalloc(sizeof(PE_IPC_HW_PARAM_REG_H15A0_T));
		}
		/* create user table */
		if (p_ipc_h15a->reg_user.addr == NULL)
		{
			p_ipc_h15a->reg_user.addr = (UINT32 *)OS_KMalloc(sizeof(PE_IPC_HW_PARAM_REG_H15A0_T));
			memset((p_ipc_h15a->reg_user.addr), 0, sizeof(PE_IPC_HW_PARAM_REG_H15A0_T));	// 0x0
		}
		/* create mask table */
		if (p_ipc_h15a->reg_mask.addr == NULL)
		{
			p_ipc_h15a->reg_mask.addr = (UINT32 *)OS_KMalloc(sizeof(PE_IPC_HW_PARAM_REG_H15A0_T));
			memset((p_ipc_h15a->reg_mask.addr), -1, sizeof(PE_IPC_HW_PARAM_REG_H15A0_T));// 0xffffffff
		}
		/* create data table */
		if (p_ipc_h15a->reg_data.addr == NULL)
		{
			p_ipc_h15a->reg_data.addr = (UINT32 *)OS_KMalloc(sizeof(PE_IPC_HW_PARAM_REG_H15A0_T));
			memset((p_ipc_h15a->reg_data.addr), 0, sizeof(PE_IPC_HW_PARAM_REG_H15A0_T));	// 0x0
		}
		/* report result */
		if (p_ipc_h15a->reg_dflt.addr != NULL && p_ipc_h15a->reg_user.addr != NULL && \
			p_ipc_h15a->reg_mask.addr != NULL && p_ipc_h15a->reg_data.addr != NULL)
		{
			PE_PRINT_NOTI("[%s]table ready.\n", PE_DNT_HW_H15A_TBL_IDX_TO_STR(index));
			ret = RET_OK;
		}
		else
		{
			PE_PRINT_NOTI("[%s]table not ready. dflt,user,mask,data:%s,%s,%s,%s\n", \
				PE_DNT_HW_H15A_TBL_IDX_TO_STR(index), \
				(p_ipc_h15a->reg_dflt.addr == NULL)? "x":"o", \
				(p_ipc_h15a->reg_user.addr == NULL)? "x":"o", \
				(p_ipc_h15a->reg_mask.addr == NULL)? "x":"o", \
				(p_ipc_h15a->reg_data.addr == NULL)? "x":"o");
			ret = RET_ERROR;
		}
	}
	else if (index == PE_DNT_HW_H15A_TBL_IDX_CLC)
	{
		/* create dflt table */
		if (p_clc_h15a->reg_dflt.addr == NULL)
		{
			p_clc_h15a->reg_dflt.addr = (UINT32 *)OS_KMalloc(sizeof(PE_CLC_HW_PARAM_REG_H15A0_T));
		}
		/* create user table */
		if (p_clc_h15a->reg_user.addr == NULL)
		{
			p_clc_h15a->reg_user.addr = (UINT32 *)OS_KMalloc(sizeof(PE_CLC_HW_PARAM_REG_H15A0_T));
			memset((p_clc_h15a->reg_user.addr), 0, sizeof(PE_CLC_HW_PARAM_REG_H15A0_T));	// 0x0
		}
		/* create mask table */
		if (p_clc_h15a->reg_mask.addr == NULL)
		{
			p_clc_h15a->reg_mask.addr = (UINT32 *)OS_KMalloc(sizeof(PE_CLC_HW_PARAM_REG_H15A0_T));
			memset((p_clc_h15a->reg_mask.addr), -1, sizeof(PE_CLC_HW_PARAM_REG_H15A0_T));// 0xffffffff
		}
		/* create data table */
		if (p_clc_h15a->reg_data.addr == NULL)
		{
			p_clc_h15a->reg_data.addr = (UINT32 *)OS_KMalloc(sizeof(PE_CLC_HW_PARAM_REG_H15A0_T));
			memset((p_clc_h15a->reg_data.addr), 0, sizeof(PE_CLC_HW_PARAM_REG_H15A0_T));	// 0x0
		}
		/* report result */
		if (p_clc_h15a->reg_dflt.addr != NULL && p_clc_h15a->reg_user.addr != NULL && \
			p_clc_h15a->reg_mask.addr != NULL && p_clc_h15a->reg_data.addr != NULL)
		{
			PE_PRINT_NOTI("[%s]table ready.\n", PE_DNT_HW_H15A_TBL_IDX_TO_STR(index));
			ret = RET_OK;
		}
		else
		{
			PE_PRINT_NOTI("[%s]table not ready. dflt,user,mask,data:%s,%s,%s,%s\n", \
				PE_DNT_HW_H15A_TBL_IDX_TO_STR(index), \
				(p_clc_h15a->reg_dflt.addr == NULL)? "x":"o", \
				(p_clc_h15a->reg_user.addr == NULL)? "x":"o", \
				(p_clc_h15a->reg_mask.addr == NULL)? "x":"o", \
				(p_clc_h15a->reg_data.addr == NULL)? "x":"o");
			ret = RET_ERROR;
		}
	}
	else if (index == PE_DNT_HW_H15A_TBL_IDX_VFY)
	{
		/* create dflt table */
		if (p_vfy_h15a->reg_dflt.addr == NULL)
		{
			p_vfy_h15a->reg_dflt.addr = (UINT32 *)OS_KMalloc(sizeof(PE_VFY_HW_PARAM_REG_H15A0_T));
		}
		/* create user table */
		if (p_vfy_h15a->reg_user.addr == NULL)
		{
			p_vfy_h15a->reg_user.addr = (UINT32 *)OS_KMalloc(sizeof(PE_VFY_HW_PARAM_REG_H15A0_T));
			memset((p_vfy_h15a->reg_user.addr), 0, sizeof(PE_VFY_HW_PARAM_REG_H15A0_T));	// 0x0
		}
		/* create mask table */
		if (p_vfy_h15a->reg_mask.addr == NULL)
		{
			p_vfy_h15a->reg_mask.addr = (UINT32 *)OS_KMalloc(sizeof(PE_VFY_HW_PARAM_REG_H15A0_T));
			memset((p_vfy_h15a->reg_mask.addr), -1, sizeof(PE_VFY_HW_PARAM_REG_H15A0_T));// 0xffffffff
		}
		/* create data table */
		if (p_vfy_h15a->reg_data.addr == NULL)
		{
			p_vfy_h15a->reg_data.addr = (UINT32 *)OS_KMalloc(sizeof(PE_VFY_HW_PARAM_REG_H15A0_T));
			memset((p_vfy_h15a->reg_data.addr), 0, sizeof(PE_VFY_HW_PARAM_REG_H15A0_T));	// 0x0
		}
		/* report result */
		if (p_vfy_h15a->reg_dflt.addr != NULL && p_vfy_h15a->reg_user.addr != NULL && \
			p_vfy_h15a->reg_mask.addr != NULL && p_vfy_h15a->reg_data.addr != NULL)
		{
			PE_PRINT_NOTI("[%s]table ready.\n", PE_DNT_HW_H15A_TBL_IDX_TO_STR(index));
			ret = RET_OK;
		}
		else
		{
			PE_PRINT_NOTI("[%s]table not ready. dflt,user,mask,data:%s,%s,%s,%s\n", \
				PE_DNT_HW_H15A_TBL_IDX_TO_STR(index), \
				(p_vfy_h15a->reg_dflt.addr == NULL)? "x":"o", \
				(p_vfy_h15a->reg_user.addr == NULL)? "x":"o", \
				(p_vfy_h15a->reg_mask.addr == NULL)? "x":"o", \
				(p_vfy_h15a->reg_data.addr == NULL)? "x":"o");
			ret = RET_ERROR;
		}
	}
	else
	{
		PE_DNT_HW_H15_DBG_PRINT("nothing to do\n");	ret = RET_OK;
	}
	return ret;
}
#endif
/**
 * debug setting
 *
 * @param   *pstParams [in]LX_PE_DBG_SETTINGS_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_DNT_HW_H15_SetDebugSettings(LX_PE_DBG_SETTINGS_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		PE_DNT_HW_H15_DBG_PRINT("wid:%d,type:0x%x,[0x%x]print:0x%x,bypass:0x%x\n",\
			pstParams->win_id,pstParams->type,(0x1<<PE_ITEM_PKTMODL(DNT)),\
			pstParams->print_lvl,pstParams->bypass);
		/* set print level */
		if(pstParams->type&LX_PE_DBG_LV)
		{
			_g_dnt_hw_h15_trace = (pstParams->print_lvl & (0x1<<PE_ITEM_PKTMODL(DNT)))? 0x1:0x0;
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
int PE_DNT_HW_H15_SetDefault(LX_PE_DEFAULT_SETTINGS_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H15)
		{
			ret = PE_DNT_HW_H15A_SetDefault(pstParams);
			PE_DNT_HW_H15_CHECK_CODE(ret, break, \
				"[%s,%d] PE_DNT_HW_H15A_SetDefault() error.\n", __F__, __L__);
		}
		else
		{
			PE_DNT_HW_H15_DBG_PRINT("nothing to do\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * set dnt default(for H15AX)
 *
 * @param   *pstParams [in] LX_PE_DEFAULT_SETTINGS_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_DNT_HW_H15A_SetDefault(LX_PE_DEFAULT_SETTINGS_T *pstParams)
{
	int ret = RET_OK;
	PE_INF_H15_SETTINGS_T inf_set;
	LX_PE_INF_DISPLAY_T *cur0_inf = NULL;
	LX_PE_INF_DISPLAY_T *cur1_inf = NULL;
	PE_DNT_HW_H15A_IPC_FMT cur0_ipc_fmt;
	PE_DNT_HW_H15A_CLC_FMT cur0_clc_fmt;
	PE_DNT_HW_H15A_VFY_FMT cur0_vfy_fmt;
	UINT32 cur0_force_cbcr_enb = 0, cur0_force_y_max_th = 0;
	PE_DNT_HW_H15_SETTINGS_T *pInfo = &_g_pe_dnt_hw_h15_info;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		ret = PE_INF_H15_GetCurInfSettings(&inf_set);
		PE_DNT_HW_H15_CHECK_CODE(ret, break, \
			"[%s,%d] PE_INF_H15_GetCurInfSettings() error.\n", __F__, __L__);
		cur0_inf=&inf_set.disp_info[LX_PE_WIN_0];
		cur1_inf=&inf_set.disp_info[LX_PE_WIN_1];
		CHECK_KNULL(cur0_inf);
		CHECK_KNULL(cur1_inf);
		/* nd12 tpd */
		if(PE_CHECK_WIN0(pstParams->win_id))
		{
			if((cur0_inf->src_type==LX_PE_SRC_DTV && \
				cur0_inf->fmt_type==LX_PE_FMT_HD && \
				cur0_inf->scan_type==LX_PE_SCAN_INTERLACE) || \
				cur0_inf->src_type==LX_PE_SRC_ATV || \
				cur0_inf->src_type==LX_PE_SRC_CVBS || \
				cur0_inf->src_type==LX_PE_SRC_SCART || \
				cur0_inf->src_type==LX_PE_SRC_COMP)
			{
				cur0_force_cbcr_enb = 0x1;
				cur0_force_y_max_th = 0x5f;
			}
			else if (cur0_inf->src_type==LX_PE_SRC_HDMI && cur0_inf->hdmi_type==LX_PE_HDMI_TV)
			{
				cur0_force_cbcr_enb = 0x1;
				cur0_force_y_max_th = 0x48;
			}
			else
			{
				cur0_force_cbcr_enb = 0x0;
				cur0_force_y_max_th = 0x5f;
			}
			if ((pInfo->force_cbcr_enb != cur0_force_cbcr_enb) || \
				(pInfo->force_y_max_th != cur0_force_y_max_th))
			{
				PE_ND12_H15A0_QWr01(tpd_ctrl_08,reg_force_cb_enable,cur0_force_cbcr_enb);
				PE_ND12_H15A0_QWr01(tpd_ctrl_09,reg_force_cr_enable,cur0_force_cbcr_enb);
				PE_ND12_H15A0_QWr01(tpd_ctrl_07,reg_force_y_max_th, cur0_force_y_max_th);
				/* for debug */
				do {
					char _str_buf[PE_TRACE_STR_SIZE];
					sprintf(_str_buf, "[%s,%d]force_en,th:%d,%d -> %d,%d(sr,fmt,sc:%d,%d,%d)", \
						__F__, __L__, pInfo->force_cbcr_enb, pInfo->force_y_max_th, \
						cur0_force_cbcr_enb, cur0_force_y_max_th, cur0_inf->src_type, \
						cur0_inf->fmt_type, cur0_inf->scan_type);
					ret = PE_INF_H15_SetTraceDataBuf(_str_buf);
					PE_DNT_HW_H15_CHECK_CODE(ret, break, \
						"PE_INF_H15_SetTraceDataBuf() error.\n");
				} while(0);
				pInfo->force_cbcr_enb = cur0_force_cbcr_enb;
				pInfo->force_y_max_th = cur0_force_y_max_th;
			}
		}
		/* nd12 ipc */
		if(PE_CHECK_WIN0(pstParams->win_id))
		{
			cur0_ipc_fmt = PE_DNT_HW_H15A_ConvDispInfoToIpcFmt(cur0_inf);
			PE_DNT_HW_H15A_DBG_PRINT_IPC_FMT_STATE(pInfo->ipc0a_fmt,cur0_ipc_fmt);
			if(pInfo->ipc0a_fmt!=cur0_ipc_fmt)
			{
				#ifdef PE_HW_H15_PROTECT_OVERLAP_SETTINGS
				switch(cur0_ipc_fmt)
				{
					case PE_DNT_H15A_IPC_ATV_480:
						PE_DNT_HW_H15A_IPC_REG_TABLE(ipc_l_atv_480_default_h15a0,  PE_H15A_REG_WRITE_BASE);
						PE_DNT_HW_H15A_IPC_REG_TABLE(ipc_l_atv_480_default_h15a0,  PE_H15A_ND_ND_1_2_DIFF);
						break;
					case PE_DNT_H15A_IPC_ATV_576:
						PE_DNT_HW_H15A_IPC_REG_TABLE(ipc_l_atv_576_default_h15a0,  PE_H15A_REG_WRITE_BASE);
						PE_DNT_HW_H15A_IPC_REG_TABLE(ipc_l_atv_576_default_h15a0,  PE_H15A_ND_ND_1_2_DIFF);
						break;
					case PE_DNT_H15A_IPC_SD:
						PE_DNT_HW_H15A_IPC_REG_TABLE(ipc_l_sd_default_h15a0,       PE_H15A_REG_WRITE_BASE);
						PE_DNT_HW_H15A_IPC_REG_TABLE(ipc_l_sd_default_h15a0,       PE_H15A_ND_ND_1_2_DIFF);
						break;
					case PE_DNT_H15A_IPC_HD:
						PE_DNT_HW_H15A_IPC_REG_TABLE(ipc_l_hd_default_h15a0,       PE_H15A_REG_WRITE_BASE);
						PE_DNT_HW_H15A_IPC_REG_TABLE(ipc_l_hd_default_h15a0,       PE_H15A_ND_ND_1_2_DIFF);
						break;
					case PE_DNT_H15A_IPC_DTV_480:
						PE_DNT_HW_H15A_IPC_REG_TABLE(ipc_l_dtv_480_default_h15a0,  PE_H15A_REG_WRITE_BASE);
						PE_DNT_HW_H15A_IPC_REG_TABLE(ipc_l_dtv_480_default_h15a0,  PE_H15A_ND_ND_1_2_DIFF);
						break;
					case PE_DNT_H15A_IPC_DTV_576:
						PE_DNT_HW_H15A_IPC_REG_TABLE(ipc_l_dtv_576_default_h15a0,  PE_H15A_REG_WRITE_BASE);
						PE_DNT_HW_H15A_IPC_REG_TABLE(ipc_l_dtv_576_default_h15a0,  PE_H15A_ND_ND_1_2_DIFF);
						break;
					case PE_DNT_H15A_IPC_AV_480:
						PE_DNT_HW_H15A_IPC_REG_TABLE(ipc_l_av_480_default_h15a0,  PE_H15A_REG_WRITE_BASE);
						PE_DNT_HW_H15A_IPC_REG_TABLE(ipc_l_av_480_default_h15a0,  PE_H15A_ND_ND_1_2_DIFF);
						break;
					default:
						break;
				}
				#else
				switch(cur0_ipc_fmt)
				{
					case PE_DNT_H15A_IPC_ATV_480:
						PE_SET_REG_TABLE(DNT_HW_H15,ipc_l_atv_480_default_h15a0,PE_H15A_REG_WRITE_BASE);
						PE_SET_REG_TABLE(DNT_HW_H15,ipc_l_atv_480_default_h15a0,PE_H15A_ND_ND_1_2_DIFF);
						break;
					case PE_DNT_H15A_IPC_ATV_576:
						PE_SET_REG_TABLE(DNT_HW_H15,ipc_l_atv_576_default_h15a0,PE_H15A_REG_WRITE_BASE);
						PE_SET_REG_TABLE(DNT_HW_H15,ipc_l_atv_576_default_h15a0,PE_H15A_ND_ND_1_2_DIFF);
						break;
					case PE_DNT_H15A_IPC_SD:
						PE_SET_REG_TABLE(DNT_HW_H15,ipc_l_sd_default_h15a0,     PE_H15A_REG_WRITE_BASE);
						PE_SET_REG_TABLE(DNT_HW_H15,ipc_l_sd_default_h15a0,     PE_H15A_ND_ND_1_2_DIFF);
						break;
					case PE_DNT_H15A_IPC_HD:
						PE_SET_REG_TABLE(DNT_HW_H15,ipc_l_hd_default_h15a0,     PE_H15A_REG_WRITE_BASE);
						PE_SET_REG_TABLE(DNT_HW_H15,ipc_l_hd_default_h15a0,     PE_H15A_ND_ND_1_2_DIFF);
						break;
					case PE_DNT_H15A_IPC_DTV_480:
						PE_SET_REG_TABLE(DNT_HW_H15,ipc_l_dtv_480_default_h15a0,PE_H15A_REG_WRITE_BASE);
						PE_SET_REG_TABLE(DNT_HW_H15,ipc_l_dtv_480_default_h15a0,PE_H15A_ND_ND_1_2_DIFF);
						break;
					case PE_DNT_H15A_IPC_DTV_576:
						PE_SET_REG_TABLE(DNT_HW_H15,ipc_l_dtv_576_default_h15a0,PE_H15A_REG_WRITE_BASE);
						PE_SET_REG_TABLE(DNT_HW_H15,ipc_l_dtv_576_default_h15a0,PE_H15A_ND_ND_1_2_DIFF);
						break;
					default:
						break;
				}
				#endif
				pInfo->ipc0a_fmt=cur0_ipc_fmt;
			}
		}
		/* nds ipc */
		/* nd1 clc */
		if(PE_CHECK_WIN0(pstParams->win_id))
		{
			cur0_clc_fmt = PE_DNT_HW_H15A_ConvDispInfoToClcFmt(cur0_inf);
			PE_DNT_HW_H15A_DBG_PRINT_CLC_FMT_STATE(pInfo->clc0a_fmt,cur0_clc_fmt);
			if(pInfo->clc0a_fmt!=cur0_clc_fmt)
			{
				#ifdef PE_HW_H15_PROTECT_OVERLAP_SETTINGS
				switch(cur0_clc_fmt)
				{
					case PE_DNT_H15A_CLC_SD_50:
						PE_DNT_HW_H15A_CLC_REG_TABLE(clc_l_sd_50_default_h15a0,PE_H15A_REG_WRITE_BASE);
						break;
					case PE_DNT_H15A_CLC_SD_60:
						PE_DNT_HW_H15A_CLC_REG_TABLE(clc_l_sd_60_default_h15a0,PE_H15A_REG_WRITE_BASE);
						break;
					case PE_DNT_H15A_CLC_HD:
						PE_DNT_HW_H15A_CLC_REG_TABLE(clc_l_hd_default_h15a0,   PE_H15A_REG_WRITE_BASE);
						break;
					case PE_DNT_H15A_CLC_PC:
						PE_DNT_HW_H15A_CLC_REG_TABLE(clc_l_pc_default_h15a0,   PE_H15A_REG_WRITE_BASE);
						break;
					default:
						break;
				}
				#else
				switch(cur0_clc_fmt)
				{
					case PE_DNT_H15A_CLC_SD_50:
						PE_SET_REG_TABLE(DNT_HW_H15,clc_l_sd_50_default_h15a0,PE_H15A_REG_WRITE_BASE);
						break;
					case PE_DNT_H15A_CLC_SD_60:
						PE_SET_REG_TABLE(DNT_HW_H15,clc_l_sd_60_default_h15a0,PE_H15A_REG_WRITE_BASE);
						break;
					case PE_DNT_H15A_CLC_HD:
						PE_SET_REG_TABLE(DNT_HW_H15,clc_l_hd_default_h15a0,   PE_H15A_REG_WRITE_BASE);
						break;
					case PE_DNT_H15A_CLC_PC:
						PE_SET_REG_TABLE(DNT_HW_H15,clc_l_pc_default_h15a0,   PE_H15A_REG_WRITE_BASE);
						break;
					default:
						break;
				}
				#endif
				pInfo->clc0a_fmt=cur0_clc_fmt;
			}
		}
		/* dnr12 vfy */
		if(PE_CHECK_WIN0(pstParams->win_id))
		{
			cur0_vfy_fmt = PE_DNT_HW_H15A_ConvDispInfoToVfyFmt(cur0_inf);
			PE_DNT_HW_H15A_DBG_PRINT_VFY_FMT_STATE(pInfo->vfy0a_fmt,cur0_vfy_fmt);
			if(pInfo->vfy0a_fmt!=cur0_vfy_fmt)
			{
				#ifdef PE_HW_H15_PROTECT_OVERLAP_SETTINGS
				switch(cur0_vfy_fmt)
				{
					case PE_DNT_H15A_VFY_ATV:
						PE_DNT_HW_H15A_VFY_REG_TABLE(vflt_y_l_atv_default_h15a0,   PE_H15A_REG_WRITE_BASE);
						PE_DNT_HW_H15A_VFY_REG_TABLE(vflt_y_l_atv_default_h15a0,   PE_H15A_ND_DNR_12_DIFF);
						break;
					case PE_DNT_H15A_VFY_AV:
						PE_DNT_HW_H15A_VFY_REG_TABLE(vflt_y_l_av_default_h15a0,    PE_H15A_REG_WRITE_BASE);
						PE_DNT_HW_H15A_VFY_REG_TABLE(vflt_y_l_av_default_h15a0,    PE_H15A_ND_DNR_12_DIFF);
						break;
					case PE_DNT_H15A_VFY_HD:
						PE_DNT_HW_H15A_VFY_REG_TABLE(vflt_y_l_hd_default_h15a0,    PE_H15A_REG_WRITE_BASE);
						PE_DNT_HW_H15A_VFY_REG_TABLE(vflt_y_l_hd_default_h15a0,    PE_H15A_ND_DNR_12_DIFF);
						break;
					case PE_DNT_H15A_VFY_PC:
						PE_DNT_HW_H15A_VFY_REG_TABLE(vflt_y_l_pc_default_h15a0,    PE_H15A_REG_WRITE_BASE);
						PE_DNT_HW_H15A_VFY_REG_TABLE(vflt_y_l_pc_default_h15a0,    PE_H15A_ND_DNR_12_DIFF);
						break;
					case PE_DNT_H15A_VFY_SD:
						PE_DNT_HW_H15A_VFY_REG_TABLE(vflt_y_l_sd_default_h15a0,    PE_H15A_REG_WRITE_BASE);
						PE_DNT_HW_H15A_VFY_REG_TABLE(vflt_y_l_sd_default_h15a0,    PE_H15A_ND_DNR_12_DIFF);
						break;
					case PE_DNT_H15A_VFY_SD_DTV:
						PE_DNT_HW_H15A_VFY_REG_TABLE(vflt_y_l_sd_dtv_default_h15a0,PE_H15A_REG_WRITE_BASE);
						PE_DNT_HW_H15A_VFY_REG_TABLE(vflt_y_l_sd_dtv_default_h15a0,PE_H15A_ND_DNR_12_DIFF);
						break;
					case PE_DNT_H15A_VFY_SECAM:
						PE_DNT_HW_H15A_VFY_REG_TABLE(vflt_y_l_secam_default_h15a0, PE_H15A_REG_WRITE_BASE);
						PE_DNT_HW_H15A_VFY_REG_TABLE(vflt_y_l_secam_default_h15a0, PE_H15A_ND_DNR_12_DIFF);
						break;
					default:
						break;
				}
				#else
				switch(cur0_vfy_fmt)
				{
					case PE_DNT_H15A_VFY_ATV:
						PE_SET_REG_TABLE(DNT_HW_H15,vflt_y_l_atv_default_h15a0,   PE_H15A_REG_WRITE_BASE);
						PE_SET_REG_TABLE(DNT_HW_H15,vflt_y_l_atv_default_h15a0,   PE_H15A_ND_DNR_12_DIFF);
						break;
					case PE_DNT_H15A_VFY_AV:
						PE_SET_REG_TABLE(DNT_HW_H15,vflt_y_l_av_default_h15a0,    PE_H15A_REG_WRITE_BASE);
						PE_SET_REG_TABLE(DNT_HW_H15,vflt_y_l_av_default_h15a0,    PE_H15A_ND_DNR_12_DIFF);
						break;
					case PE_DNT_H15A_VFY_HD:
						PE_SET_REG_TABLE(DNT_HW_H15,vflt_y_l_hd_default_h15a0,    PE_H15A_REG_WRITE_BASE);
						PE_SET_REG_TABLE(DNT_HW_H15,vflt_y_l_hd_default_h15a0,    PE_H15A_ND_DNR_12_DIFF);
						break;
					case PE_DNT_H15A_VFY_PC:
						PE_SET_REG_TABLE(DNT_HW_H15,vflt_y_l_pc_default_h15a0,    PE_H15A_REG_WRITE_BASE);
						PE_SET_REG_TABLE(DNT_HW_H15,vflt_y_l_pc_default_h15a0,    PE_H15A_ND_DNR_12_DIFF);
						break;
					case PE_DNT_H15A_VFY_SD:
						PE_SET_REG_TABLE(DNT_HW_H15,vflt_y_l_sd_default_h15a0,    PE_H15A_REG_WRITE_BASE);
						PE_SET_REG_TABLE(DNT_HW_H15,vflt_y_l_sd_default_h15a0,    PE_H15A_ND_DNR_12_DIFF);
						break;
					case PE_DNT_H15A_VFY_SD_DTV:
						PE_SET_REG_TABLE(DNT_HW_H15,vflt_y_l_sd_dtv_default_h15a0,PE_H15A_REG_WRITE_BASE);
						PE_SET_REG_TABLE(DNT_HW_H15,vflt_y_l_sd_dtv_default_h15a0,PE_H15A_ND_DNR_12_DIFF);
						break;
					case PE_DNT_H15A_VFY_SECAM:
						PE_SET_REG_TABLE(DNT_HW_H15,vflt_y_l_secam_default_h15a0, PE_H15A_REG_WRITE_BASE);
						PE_SET_REG_TABLE(DNT_HW_H15,vflt_y_l_secam_default_h15a0, PE_H15A_ND_DNR_12_DIFF);
						break;
					default:
						break;
				}
				#endif
				pInfo->vfy0a_fmt=cur0_vfy_fmt;
			}
		}
	}while(0);
	return ret;
}
/**
 * get ipc fmt using display info(for H15AX)
 *
 * @param   *disp_inf [in] LX_PE_INF_DISPLAY_T
 * @return  PE_DNT_HW_H15A_IPC_FMT
 * @see
 * @author
 */
static PE_DNT_HW_H15A_IPC_FMT PE_DNT_HW_H15A_ConvDispInfoToIpcFmt(LX_PE_INF_DISPLAY_T *disp_inf)
{
	PE_DNT_HW_H15A_IPC_FMT ipc_fmt = PE_DNT_H15A_IPC_NUM;
	switch(disp_inf->src_type)
	{
		case LX_PE_SRC_ATV:
			if(disp_inf->cstd_type==LX_PE_CSTD_NTSC)
			{
				ipc_fmt = PE_DNT_H15A_IPC_ATV_480;
			}
			else
			{
				ipc_fmt = PE_DNT_H15A_IPC_ATV_576;
			}
			break;
		case LX_PE_SRC_CVBS:
		case LX_PE_SRC_SCART:
			if(disp_inf->cstd_type==LX_PE_CSTD_NTSC)
			{
				ipc_fmt = PE_DNT_H15A_IPC_AV_480;
			}
			else
			{
				ipc_fmt = PE_DNT_H15A_IPC_SD;
			}
			break;
		case LX_PE_SRC_RGBPC:
			{
				ipc_fmt = PE_DNT_H15A_IPC_SD;
			}
			break;
		case LX_PE_SRC_DTV:
			{
				if(disp_inf->fmt_type==LX_PE_FMT_SD)
				{
					if(disp_inf->fr_type==LX_PE_FR_60HZ)
					{
						ipc_fmt = PE_DNT_H15A_IPC_DTV_480;
					}
					else
					{
						ipc_fmt = PE_DNT_H15A_IPC_DTV_576;
					}
				}
				else
				{
					ipc_fmt = PE_DNT_H15A_IPC_HD;
				}
			}
			break;
		case LX_PE_SRC_COMP:
		case LX_PE_SRC_HDMI:
		default:
			if(disp_inf->src_type==LX_PE_SRC_HDMI && \
				disp_inf->hdmi_type==LX_PE_HDMI_PC)
			{
				ipc_fmt = PE_DNT_H15A_IPC_SD;
			}
			else if(disp_inf->fmt_type==LX_PE_FMT_SD)
			{
				ipc_fmt = PE_DNT_H15A_IPC_SD;
			}
			else
			{
				ipc_fmt = PE_DNT_H15A_IPC_HD;
			}
			break;
	}
	return ipc_fmt;
}
/**
 * get clc fmt using display info(for H15AX)
 *
 * @param   *disp_inf [in] LX_PE_INF_DISPLAY_T
 * @return  PE_DNT_HW_H15A_CLC_FMT
 * @see
 * @author
 */
static PE_DNT_HW_H15A_CLC_FMT PE_DNT_HW_H15A_ConvDispInfoToClcFmt(LX_PE_INF_DISPLAY_T *disp_inf)
{
	PE_DNT_HW_H15A_CLC_FMT clc_fmt = PE_DNT_H15A_CLC_NUM;
	switch(disp_inf->src_type)
	{
		case LX_PE_SRC_ATV:
		case LX_PE_SRC_CVBS:
		case LX_PE_SRC_SCART:
		case LX_PE_SRC_RGBPC:
			{
				clc_fmt = PE_DNT_H15A_CLC_PC;
			}
			break;
		case LX_PE_SRC_DTV:
		case LX_PE_SRC_COMP:
		case LX_PE_SRC_HDMI:
		default:
			if(disp_inf->src_type==LX_PE_SRC_HDMI && \
				disp_inf->hdmi_type==LX_PE_HDMI_PC)
			{
				clc_fmt = PE_DNT_H15A_CLC_PC;
			}
			else if(disp_inf->scan_type==LX_PE_SCAN_INTERLACE)
			{
				if(disp_inf->fmt_type==LX_PE_FMT_SD)
				{
					if(disp_inf->fr_type==LX_PE_FR_60HZ)
					{
						clc_fmt = PE_DNT_H15A_CLC_SD_60;
					}
					else
					{
						clc_fmt = PE_DNT_H15A_CLC_SD_50;
					}
				}
				else
				{
					clc_fmt = PE_DNT_H15A_CLC_HD;
				}
			}
			else
			{
				clc_fmt = PE_DNT_H15A_CLC_PC;
			}
			break;
	}
	return clc_fmt;
}
/**
 * get vfy fmt using display info(for H15AX)
 *
 * @param   *disp_inf [in] LX_PE_INF_DISPLAY_T
 * @return  PE_DNT_HW_H15A_VFY_FMT
 * @see
 * @author
 */
static PE_DNT_HW_H15A_VFY_FMT PE_DNT_HW_H15A_ConvDispInfoToVfyFmt(LX_PE_INF_DISPLAY_T *disp_inf)
{
	PE_DNT_HW_H15A_VFY_FMT vfy_fmt = PE_DNT_H15A_VFY_NUM;
	switch(disp_inf->src_type)
	{
		case LX_PE_SRC_ATV:
			if(disp_inf->cstd_type==LX_PE_CSTD_SECAM)
			{
				vfy_fmt = PE_DNT_H15A_VFY_SECAM;
			}
			else
			{
				vfy_fmt = PE_DNT_H15A_VFY_ATV;
			}
			break;
		case LX_PE_SRC_CVBS:
		case LX_PE_SRC_SCART:
			if(disp_inf->cstd_type==LX_PE_CSTD_SECAM)
			{
				vfy_fmt = PE_DNT_H15A_VFY_SECAM;
			}
			else
			{
				vfy_fmt = PE_DNT_H15A_VFY_AV;
			}
			break;
		case LX_PE_SRC_RGBPC:
			{
				vfy_fmt = PE_DNT_H15A_VFY_PC;
			}
			break;
		case LX_PE_SRC_DTV:
		case LX_PE_SRC_COMP:
		case LX_PE_SRC_HDMI:
		default:
			if(disp_inf->src_type==LX_PE_SRC_HDMI && \
				disp_inf->hdmi_type==LX_PE_HDMI_PC)
			{
				vfy_fmt = PE_DNT_H15A_VFY_PC;
			}
			else if(disp_inf->scan_type==LX_PE_SCAN_INTERLACE)
			{
				if(disp_inf->fmt_type==LX_PE_FMT_SD)
				{
					if(disp_inf->src_type==LX_PE_SRC_DTV)
					{
						vfy_fmt = PE_DNT_H15A_VFY_SD_DTV;
					}
					else
					{
						vfy_fmt = PE_DNT_H15A_VFY_SD;
					}
				}
				else
				{
					vfy_fmt = PE_DNT_H15A_VFY_HD;
				}
			}
			#ifdef PE_DNT_HW_H15A_APPLY_VFLT_Y_ON_DTV_1080P
			else if(disp_inf->src_type == LX_PE_SRC_DTV && \
				(disp_inf->dtv_type == LX_PE_DTV_NORMAL || \
				disp_inf->dtv_type == LX_PE_DTV_HDDPLAY) && \
				disp_inf->fmt_type == LX_PE_FMT_HD && \
				disp_inf->in_h_size > 1900 && disp_inf->in_h_size < 1940 &&\
				disp_inf->in_v_size > 1060 && disp_inf->in_v_size < 1100 && \
				disp_inf->fr_type == LX_PE_FR_50HZ)
			{
				vfy_fmt = PE_DNT_H15A_VFY_HD;
			}
			#endif
			else
			{
				vfy_fmt = PE_DNT_H15A_VFY_PC;
			}
			break;
	}
	return vfy_fmt;
}

/**
 * set film mode
 *
 * @param   *pstParams [in]LX_PE_DNT_FILMMODE_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_DNT_HW_H15_SetFilmMode(LX_PE_DNT_FILMMODE_T *pstParams)
{
	int ret = RET_OK;
	do {
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		PE_DNT_HW_H15_DBG_PRINT("set pstParams[%d] : en:%d\n", \
			pstParams->win_id, pstParams->enable);
		if(PE_KDRV_VER_H15)
		{
			if(PE_CHECK_WIN0(pstParams->win_id))
			{
				PE_CE_PE1_H15A0_QWr01(pe1_fsw_ctrl_00,reg_fmd_en_0,(pstParams->enable)? 0x1:0x0);
				#ifdef PE_DNT_HW_H15A_SET_FMD_REG
				PE_ND12_H15A0_QWr01(ipc_ctrl_00,cr_film_mode_enable,(pstParams->enable)? 0x1:0x0);
				#endif
			}
			if(PE_CHECK_WIN1(pstParams->win_id))
			{
				PE_CE_PE1_H15A0_QWr01(pe1_fsw_ctrl_00,reg_fmd_en_1,(pstParams->enable)? 0x1:0x0);
				#ifdef PE_DNT_HW_H15A_SET_FMD_REG
				PE_ND_NDS_H15A0_QWr01(ipc_ctrl_00,cr_film_mode_enable,(pstParams->enable)? 0x1:0x0);
				#endif
			}
		}
		else
		{
			PE_DNT_HW_H15_DBG_PRINT("nothing to do\n");	ret = RET_OK;
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
int PE_DNT_HW_H15_GetFilmMode(LX_PE_DNT_FILMMODE_T *pstParams)
{
	int ret = RET_OK;
	LX_PE_WIN_ID win_id;
	do {
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		win_id = PE_GET_CHECKED_WINID(pstParams->win_id);
		if(PE_KDRV_VER_H15)
		{
			if(PE_CHECK_WIN0(win_id))
			{
				PE_ND_ND1_H15A0_QRd01(ipc_ctrl_00,cr_film_mode_enable,pstParams->enable);
			}
			if(PE_CHECK_WIN1(win_id))
			{
				PE_ND_ND2_H15A0_QRd01(ipc_ctrl_00,cr_film_mode_enable,pstParams->enable);
			}
			if (pstParams->win_id == LX_PE_WIN_ALL)
			{
				PE_ND_NDS_H15A0_QRd01(ipc_ctrl_00,cr_film_mode_enable,pstParams->enable);
			}
		}
		else
		{
			PE_DNT_HW_H15_DBG_PRINT("nothing to do\n");	ret = RET_OK;
		}
		PE_DNT_HW_H15_DBG_PRINT("get pstParams[%d] : en:%d\n",pstParams->win_id,pstParams->enable);
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
int PE_DNT_HW_H15_SetLowDelayMode(LX_PE_DNT_LD_MODE_T *pstParams)
{
	int ret = RET_OK;
	UINT32 cr_game_en;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		PE_DNT_HW_H15_DBG_PRINT("set[%d]:en:%d\n", \
			pstParams->win_id, pstParams->enable);
		if(PE_KDRV_VER_H15)
		{
			if(PE_CHECK_WIN0(pstParams->win_id))
			{
				#ifdef PE_DNT_HW_H15A_SET_GAME_MODE_BY_FW
				cr_game_en = (pstParams->enable)? 0x1:0x0;
				PE_DNT_HW_H15_DBG_PRINT("set0: not support, game(%d) by fw\n", cr_game_en);
				#else
				PE_ND12_H15A0_QWr01(ipc_ctrl_01, cr_game_mode, (pstParams->enable)? 0x1:0x0);
				#endif
			}
			if(PE_CHECK_WIN1(pstParams->win_id))
			{
				#ifdef PE_DNT_HW_H15A_SET_GAME_MODE_BY_FW
				cr_game_en = (pstParams->enable)? 0x1:0x0;
				PE_DNT_HW_H15_DBG_PRINT("set1: not support, game(%d) by fw\n", cr_game_en);
				#else
				PE_ND_NDS_H15A0_QWr01(ipc_ctrl_01, cr_game_mode, (pstParams->enable)? 0x1:0x0);
				#endif
			}
		}
		else
		{
			PE_DNT_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
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
int PE_DNT_HW_H15_GetLowDelayMode(LX_PE_DNT_LD_MODE_T *pstParams)
{
	int ret = RET_OK;
	LX_PE_WIN_ID win_id;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		win_id = PE_GET_CHECKED_WINID(pstParams->win_id);
		if(PE_KDRV_VER_H15)
		{
			if(PE_CHECK_WIN0(win_id))
			{
				PE_ND_ND1_H15A0_QRd01(ipc_ctrl_01, cr_game_mode, pstParams->enable);
			}
			if(PE_CHECK_WIN1(win_id))
			{
				PE_ND_ND2_H15A0_QRd01(ipc_ctrl_01, cr_game_mode, pstParams->enable);
			}
			if (pstParams->win_id == LX_PE_WIN_ALL)
			{
				PE_ND_NDS_H15A0_QRd01(ipc_ctrl_01, cr_game_mode, pstParams->enable);
			}
		}
		else
		{
			PE_DNT_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
		PE_DNT_HW_H15_DBG_PRINT("get[%d]:en:%d\n", \
			pstParams->win_id, pstParams->enable);
	}while(0);
	return ret;
}

/**
 * set clc mode
 *
 * @param   *pCfg [in] PE_TSK_H15_CFG_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_DNT_HW_H15_SetClcMode(PE_TSK_H15_CFG_T *pCfg)
{
	int ret = RET_OK;
	#ifdef PE_HW_H15_PROTECT_OVERLAP_SETTINGS
	static UINT32 frz_history = 0;
	static UINT32 count = 0;
	static UINT32 pre_flt_en0 = 0xff;		//pre clc_en0
	static UINT32 pre_det_en0 = 0xff;		//pre clc_en0
	UINT32 cur_flt_en0 = 0;
	UINT32 cur_det_en0 = 0;
	UINT32 game_md = 0;
	PE_INF_H15_CTRL_T *ctrl0_info = &pCfg->inf.ctrl_info[LX_PE_WIN_0];
	LX_PE_INF_DISPLAY_T *disp0_info = &pCfg->inf.disp_info[LX_PE_WIN_0];
	PE_CLC_HW_PARAM_REG_H15A0_T *p_clc_dflt = _g_pe_clc_hw_param_data_h15a.reg_dflt.data;
	do{
		CHECK_KNULL(pCfg);
		if(PE_KDRV_VER_H15)
		{
			/* update frz_history */
			frz_history = (frz_history<<1)|(ctrl0_info->is_src_frz? 1:0);
			/* check clc off case */
			if (PE_HW_H15_CHK_MULTI_PATH_FMT(disp0_info->fmt_type) || \
				PE_HW_H15_CHK_MULTI_PATH_OUT(disp0_info->out_type))
			{
				cur_flt_en0 = PE_DNT_HW_H15_CLC_OFF;
				cur_det_en0 = PE_DNT_HW_H15_CLC_OFF;
			}
			else if ((frz_history&0x7) && !ctrl0_info->mute_on && \
				disp0_info->src_type==LX_PE_SRC_DTV && \
				(disp0_info->dtv_type==LX_PE_DTV_FILEPLAY || \
				disp0_info->dtv_type==LX_PE_DTV_HDDPLAY) && \
				disp0_info->fmt_type==LX_PE_FMT_HD && \
				disp0_info->scan_type==LX_PE_SCAN_INTERLACE)
			{
				cur_flt_en0 = PE_DNT_HW_H15_CLC_OFF;
				cur_det_en0 = PE_DNT_HW_H15_CLC_OFF;
			}
			else
			{
				PE_ND_ND1_H15A0_QRd01(ipc_ctrl_01, cr_game_mode, game_md);
				/* clc off when game mode on */
				if (game_md)
				{
					cur_flt_en0 = PE_DNT_HW_H15_CLC_OFF;
					cur_det_en0 = PE_DNT_HW_H15_CLC_OFF;
				}
				else
				{
					cur_flt_en0 = p_clc_dflt->clc_ctrl_00.cl_filter_enable;
					cur_det_en0 = p_clc_dflt->clc_ctrl_00.clc_detection_enable;
				}
			}
			/* compare pre vs. cur */
			if (pre_flt_en0 != cur_flt_en0 || \
				pre_det_en0 != cur_det_en0)
			{
				PE_DNT_HW_H15_DBG_PRINT("[inf] fz:%d(0x%08x),mt:%d,sr,dt,fm,sc:%d,%d,%d,%d, o:%d\n",\
					ctrl0_info->is_src_frz, frz_history, ctrl0_info->mute_on, disp0_info->src_type, \
					disp0_info->dtv_type, disp0_info->fmt_type, disp0_info->scan_type, \
					disp0_info->out_type);
				PE_DNT_HW_H15_DBG_PRINT("[set] dflt:%d, flt_en0 : %d -> %d\n",\
					p_clc_dflt->clc_ctrl_00.cl_filter_enable, pre_flt_en0, cur_flt_en0);
				PE_DNT_HW_H15_DBG_PRINT("[set] dflt:%d, det_en0 : %d -> %d\n",\
					p_clc_dflt->clc_ctrl_00.clc_detection_enable, pre_det_en0, cur_det_en0);
				PE_CE_PE1_H15A0_QWr02(pe1_fsw_ctrl_00,cl_filter_en_stat,cur_flt_en0,clc_detection_en_stat,cur_det_en0);
				//PE_DNT_HW_H15A_CLC_WR02(clc_ctrl_00,cl_filter_enable,cur_flt_en0,clc_detection_enable,cur_det_en0);
				pre_flt_en0 = cur_flt_en0;
				pre_det_en0 = cur_det_en0;
			}
			if(_g_dnt_hw_h15_trace)
			{
				if(count>PE_PRINT_COUNT_NUM)
				{
					PE_DNT_HW_H15_DBG_PRINT("[state] fz:%d(0x%08x),o:%d, dflt:%d,flt_en0:%d, dflt:%d,det_en0:%d\n",\
						ctrl0_info->is_src_frz, frz_history, disp0_info->out_type, \
						p_clc_dflt->clc_ctrl_00.cl_filter_enable, cur_flt_en0, \
						p_clc_dflt->clc_ctrl_00.clc_detection_enable, cur_det_en0);
					count=0;
				}
				count++;
			}
		}
		else
		{
			PE_DNT_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	#endif
	return ret;
}
/**
 * get current dnt settings
 *
 * @param   *pstParams [in/out] PE_DNT_HW_H15_SETTINGS_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_DNT_HW_H15_GetCurDntSettings(PE_DNT_HW_H15_SETTINGS_T *pstParams)
{
	int ret = RET_OK;
	PE_DNT_HW_H15_SETTINGS_T *pInfo = &_g_pe_dnt_hw_h15_info;
	do{

		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H15)
		{
			memcpy(pstParams,pInfo,sizeof(PE_DNT_HW_H15_SETTINGS_T));
			PE_DNT_HW_H15_DBG_PRINT("get force en,y_th:%d,%d\n", \
				pInfo->force_cbcr_enb,	pInfo->force_y_max_th);
		}
		else
		{
			PE_DNT_HW_H15_DBG_PRINT("nothing to do.\n");
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
int PE_DNT_HW_H15_RdDefault(LX_PE_ETC_TBL_T *pstParams)
{
	int ret = RET_OK;
	#ifdef PE_HW_H15_PROTECT_OVERLAP_SETTINGS
	char buffer[PE_TRACE_STR_SIZE];
	UINT32 param_size=0, table_size=0, is_valid=0;
	static UINT32 rd_cnt=0;
	PE_REG_PARAM_T param;
	PE_DNT_HW_H15_SETTINGS_T *pInfo=&_g_pe_dnt_hw_h15_info;
	UINT32 *p_ipc_dflt = _g_pe_ipc_hw_param_data_h15a.reg_dflt.addr;
	UINT32 *p_ipc_user = _g_pe_ipc_hw_param_data_h15a.reg_user.addr;
	UINT32 *p_ipc_mask = _g_pe_ipc_hw_param_data_h15a.reg_mask.addr;
	UINT32 *p_ipc_data = _g_pe_ipc_hw_param_data_h15a.reg_data.addr;
	UINT32 *p_clc_dflt = _g_pe_clc_hw_param_data_h15a.reg_dflt.addr;
	UINT32 *p_clc_user = _g_pe_clc_hw_param_data_h15a.reg_user.addr;
	UINT32 *p_clc_mask = _g_pe_clc_hw_param_data_h15a.reg_mask.addr;
	UINT32 *p_clc_data = _g_pe_clc_hw_param_data_h15a.reg_data.addr;
	UINT32 *p_vfy_dflt = _g_pe_vfy_hw_param_data_h15a.reg_dflt.addr;
	UINT32 *p_vfy_user = _g_pe_vfy_hw_param_data_h15a.reg_user.addr;
	UINT32 *p_vfy_mask = _g_pe_vfy_hw_param_data_h15a.reg_mask.addr;
	UINT32 *p_vfy_data = _g_pe_vfy_hw_param_data_h15a.reg_data.addr;

#define PE_DNT_H15_NO_PRINT			0
#define PE_DNT_H15_RD_N_HEX_PRINT	1
#define PE_DNT_H15_HEX_PRINT_ONLY	2
#define PE_DNT_H15_PRINT_START		"START_OF_PRINT"
#define PE_DNT_H15_PRINT_END		"END_OF_PRINT"
#define PE_DNT_H15_PRINT_EXIT		"EXIT_OF_PRINT"
#define PE_DNT_H15_PRINT_RESRV		"RESERVED"
#define PE_DNT_H15_CASE_PRMENU(_cnt, _action, _buf, _item)	\
	case (_cnt):sprintf(_buf, "   [0x%02x] %s\n", (_item), #_item);_action;break
#define PE_DNT_H15_CASE_SPRINT(_cnt, _action, _buf, fmt, args...)	\
	case (_cnt):sprintf(_buf, fmt, ##args);_action;break
#define PE_DNT_H15_DFLT_SPRINT(_action, _buf, fmt, args...)		\
	default:sprintf(_buf, fmt, ##args);_action;break

	do{
		CHECK_KNULL(pstParams);
		CHECK_KNULL(pstParams->data);
		CHECK_KNULL(p_clc_dflt);
		CHECK_KNULL(p_clc_user);
		CHECK_KNULL(p_clc_mask);
		CHECK_KNULL(p_clc_data);
		CHECK_KNULL(p_vfy_dflt);
		CHECK_KNULL(p_vfy_user);
		CHECK_KNULL(p_vfy_mask);
		CHECK_KNULL(p_vfy_data);
		if (PE_KDRV_VER_H15)
		{
			if (pstParams->data[0]>=PE_DNT_HW_H15A_RD_MENU_MAX)
			{
				sprintf(buffer, "%s", PE_DNT_H15_PRINT_EXIT);
				rd_cnt=0;
			}
			else if (pstParams->data[0]==PE_DNT_HW_H15A_RD_MENU_DISP)
			{
				switch (rd_cnt)
				{
					PE_DNT_H15_CASE_SPRINT( 0, rd_cnt++, buffer, "%s", PE_DNT_H15_PRINT_START);
					PE_DNT_H15_CASE_SPRINT( 1, rd_cnt++, buffer, "   ** current default **\n");
					PE_DNT_H15_CASE_SPRINT( 2, rd_cnt++, buffer, \
						"   ipc0a_fmt : %s\n", PE_DNT_HW_H15A_IPC_FMT_TO_STR(pInfo->ipc0a_fmt));
					PE_DNT_H15_CASE_SPRINT( 3, rd_cnt++, buffer, \
						"   clc0a_fmt : %s\n", PE_DNT_HW_H15A_CLC_FMT_TO_STR(pInfo->clc0a_fmt));
					PE_DNT_H15_CASE_SPRINT( 4, rd_cnt++, buffer, \
						"   vfy0a_fmt : %s\n", PE_DNT_HW_H15A_VFY_FMT_TO_STR(pInfo->vfy0a_fmt));
					PE_DNT_H15_CASE_SPRINT( 5, rd_cnt++, buffer, PE_DNT_H15_PRINT_RESRV);
					PE_DNT_H15_CASE_SPRINT( 6, rd_cnt++, buffer, PE_DNT_H15_PRINT_RESRV);
					PE_DNT_H15_CASE_SPRINT( 7, rd_cnt++, buffer, PE_DNT_H15_PRINT_RESRV);
					PE_DNT_H15_CASE_SPRINT( 8, rd_cnt++, buffer, PE_DNT_H15_PRINT_RESRV);
					PE_DNT_H15_CASE_SPRINT( 9, rd_cnt++, buffer, "   *********************\n");
					PE_DNT_H15_CASE_PRMENU(10, rd_cnt++, buffer, PE_DNT_HW_H15A_RD_IPC1_DISP);
					PE_DNT_H15_CASE_PRMENU(11, rd_cnt++, buffer, PE_DNT_HW_H15A_RD_IPC2_DISP);
					PE_DNT_H15_CASE_PRMENU(12, rd_cnt++, buffer, PE_DNT_HW_H15A_RD_IPC1_DFLT);
					PE_DNT_H15_CASE_PRMENU(13, rd_cnt++, buffer, PE_DNT_HW_H15A_RD_IPC1_USER);
					PE_DNT_H15_CASE_PRMENU(14, rd_cnt++, buffer, PE_DNT_HW_H15A_RD_IPC1_MASK);
					PE_DNT_H15_CASE_PRMENU(15, rd_cnt++, buffer, PE_DNT_HW_H15A_RD_IPC1_DATA);
					PE_DNT_H15_CASE_PRMENU(16, rd_cnt++, buffer, PE_DNT_HW_H15A_RD_CLC1_DISP);
					PE_DNT_H15_CASE_PRMENU(17, rd_cnt++, buffer, PE_DNT_HW_H15A_RD_CLC1_DFLT);
					PE_DNT_H15_CASE_PRMENU(18, rd_cnt++, buffer, PE_DNT_HW_H15A_RD_CLC1_USER);
					PE_DNT_H15_CASE_PRMENU(19, rd_cnt++, buffer, PE_DNT_HW_H15A_RD_CLC1_MASK);
					PE_DNT_H15_CASE_PRMENU(20, rd_cnt++, buffer, PE_DNT_HW_H15A_RD_CLC1_DATA);
					PE_DNT_H15_CASE_PRMENU(21, rd_cnt++, buffer, PE_DNT_HW_H15A_RD_VFY1_DISP);
					PE_DNT_H15_CASE_PRMENU(22, rd_cnt++, buffer, PE_DNT_HW_H15A_RD_VFY2_DISP);
					PE_DNT_H15_CASE_PRMENU(23, rd_cnt++, buffer, PE_DNT_HW_H15A_RD_VFY1_DFLT);
					PE_DNT_H15_CASE_PRMENU(24, rd_cnt++, buffer, PE_DNT_HW_H15A_RD_VFY1_USER);
					PE_DNT_H15_CASE_PRMENU(25, rd_cnt++, buffer, PE_DNT_HW_H15A_RD_VFY1_MASK);
					PE_DNT_H15_CASE_PRMENU(26, rd_cnt++, buffer, PE_DNT_HW_H15A_RD_VFY1_DATA);
					PE_DNT_H15_DFLT_SPRINT(rd_cnt=0, buffer, "%s", PE_DNT_H15_PRINT_END);
				}
			}
			else
			{
				is_valid = PE_DNT_H15_NO_PRINT;
				switch (pstParams->data[0])
				{
					case PE_DNT_HW_H15A_RD_IPC1_DISP:
					case PE_DNT_HW_H15A_RD_IPC2_DISP:
						table_size = sizeof(ipc_l_init_h15a0);
						param_size = table_size/sizeof(PE_REG_PARAM_T);
						if (rd_cnt<param_size)
						{
							param.addr = (pstParams->data[0]==PE_DNT_HW_H15A_RD_IPC1_DISP)? \
								ipc_l_init_h15a0[rd_cnt].addr:\
								ipc_l_init_h15a0[rd_cnt].addr+PE_H15A_ND_ND_1_2_DIFF;
							is_valid = PE_DNT_H15_RD_N_HEX_PRINT;
						}
						break;
					case PE_DNT_HW_H15A_RD_IPC1_DFLT:
						table_size = sizeof(PE_IPC_HW_PARAM_REG_H15A0_T);
						param_size = table_size/sizeof(UINT32);
						if (rd_cnt<param_size)
						{
							param.addr = ipc_l_init_h15a0[rd_cnt].addr;
							param.data = p_ipc_dflt[rd_cnt];
							is_valid = PE_DNT_H15_HEX_PRINT_ONLY;
						}
						break;
					case PE_DNT_HW_H15A_RD_IPC1_USER:
						table_size = sizeof(PE_IPC_HW_PARAM_REG_H15A0_T);
						param_size = table_size/sizeof(UINT32);
						if (rd_cnt<param_size)
						{
							param.addr = ipc_l_init_h15a0[rd_cnt].addr;
							param.data = p_ipc_user[rd_cnt];
							is_valid = PE_DNT_H15_HEX_PRINT_ONLY;
						}
						break;
					case PE_DNT_HW_H15A_RD_IPC1_MASK:
						table_size = sizeof(PE_IPC_HW_PARAM_REG_H15A0_T);
						param_size = table_size/sizeof(UINT32);
						if (rd_cnt<param_size)
						{
							param.addr = ipc_l_init_h15a0[rd_cnt].addr;
							param.data = p_ipc_mask[rd_cnt];
							is_valid = PE_DNT_H15_HEX_PRINT_ONLY;
						}
						break;
					case PE_DNT_HW_H15A_RD_IPC1_DATA:
						table_size = sizeof(PE_IPC_HW_PARAM_REG_H15A0_T);
						param_size = table_size/sizeof(UINT32);
						if (rd_cnt<param_size)
						{
							param.addr = ipc_l_init_h15a0[rd_cnt].addr;
							param.data = p_ipc_data[rd_cnt];
							is_valid = PE_DNT_H15_HEX_PRINT_ONLY;
						}
						break;
					case PE_DNT_HW_H15A_RD_CLC1_DISP:
						table_size = sizeof(clc_l_init_h15a0);
						param_size = table_size/sizeof(PE_REG_PARAM_T);
						if (rd_cnt<param_size)
						{
							param.addr = clc_l_init_h15a0[rd_cnt].addr;
							is_valid = PE_DNT_H15_RD_N_HEX_PRINT;
						}
						break;
					case PE_DNT_HW_H15A_RD_CLC1_DFLT:
						table_size = sizeof(PE_CLC_HW_PARAM_REG_H15A0_T);
						param_size = table_size/sizeof(UINT32);
						if (rd_cnt<param_size)
						{
							param.addr = clc_l_init_h15a0[rd_cnt].addr;
							param.data = p_clc_dflt[rd_cnt];
							is_valid = PE_DNT_H15_HEX_PRINT_ONLY;
						}
						break;
					case PE_DNT_HW_H15A_RD_CLC1_USER:
						table_size = sizeof(PE_CLC_HW_PARAM_REG_H15A0_T);
						param_size = table_size/sizeof(UINT32);
						if (rd_cnt<param_size)
						{
							param.addr = clc_l_init_h15a0[rd_cnt].addr;
							param.data = p_clc_user[rd_cnt];
							is_valid = PE_DNT_H15_HEX_PRINT_ONLY;
						}
						break;
					case PE_DNT_HW_H15A_RD_CLC1_MASK:
						table_size = sizeof(PE_CLC_HW_PARAM_REG_H15A0_T);
						param_size = table_size/sizeof(UINT32);
						if (rd_cnt<param_size)
						{
							param.addr = clc_l_init_h15a0[rd_cnt].addr;
							param.data = p_clc_mask[rd_cnt];
							is_valid = PE_DNT_H15_HEX_PRINT_ONLY;
						}
						break;
					case PE_DNT_HW_H15A_RD_CLC1_DATA:
						table_size = sizeof(PE_CLC_HW_PARAM_REG_H15A0_T);
						param_size = table_size/sizeof(UINT32);
						if (rd_cnt<param_size)
						{
							param.addr = clc_l_init_h15a0[rd_cnt].addr;
							param.data = p_clc_data[rd_cnt];
							is_valid = PE_DNT_H15_HEX_PRINT_ONLY;
						}
						break;
					case PE_DNT_HW_H15A_RD_VFY1_DISP:
					case PE_DNT_HW_H15A_RD_VFY2_DISP:
						table_size = sizeof(vflt_y_l_init_h15a0);
						param_size = table_size/sizeof(PE_REG_PARAM_T);
						if (rd_cnt<param_size)
						{
							param.addr = (pstParams->data[0]==PE_DNT_HW_H15A_RD_VFY1_DISP)? \
								vflt_y_l_init_h15a0[rd_cnt].addr:\
								vflt_y_l_init_h15a0[rd_cnt].addr+PE_H15A_ND_DNR_12_DIFF;
							is_valid = PE_DNT_H15_RD_N_HEX_PRINT;
						}
						break;
					case PE_DNT_HW_H15A_RD_VFY1_DFLT:
						table_size = sizeof(PE_VFY_HW_PARAM_REG_H15A0_T);
						param_size = table_size/sizeof(UINT32);
						if (rd_cnt<param_size)
						{
							param.addr = vflt_y_l_init_h15a0[rd_cnt].addr;
							param.data = p_vfy_dflt[rd_cnt];
							is_valid = PE_DNT_H15_HEX_PRINT_ONLY;
						}
						break;
					case PE_DNT_HW_H15A_RD_VFY1_USER:
						table_size = sizeof(PE_VFY_HW_PARAM_REG_H15A0_T);
						param_size = table_size/sizeof(UINT32);
						if (rd_cnt<param_size)
						{
							param.addr = vflt_y_l_init_h15a0[rd_cnt].addr;
							param.data = p_vfy_user[rd_cnt];
							is_valid = PE_DNT_H15_HEX_PRINT_ONLY;
						}
						break;
					case PE_DNT_HW_H15A_RD_VFY1_MASK:
						table_size = sizeof(PE_VFY_HW_PARAM_REG_H15A0_T);
						param_size = table_size/sizeof(UINT32);
						if (rd_cnt<param_size)
						{
							param.addr = vflt_y_l_init_h15a0[rd_cnt].addr;
							param.data = p_vfy_mask[rd_cnt];
							is_valid = PE_DNT_H15_HEX_PRINT_ONLY;
						}
						break;
					case PE_DNT_HW_H15A_RD_VFY1_DATA:
						table_size = sizeof(PE_VFY_HW_PARAM_REG_H15A0_T);
						param_size = table_size/sizeof(UINT32);
						if (rd_cnt<param_size)
						{
							param.addr = vflt_y_l_init_h15a0[rd_cnt].addr;
							param.data = p_vfy_data[rd_cnt];
							is_valid = PE_DNT_H15_HEX_PRINT_ONLY;
						}
						break;
					default:
						break;
				}
				if (is_valid == PE_DNT_H15_RD_N_HEX_PRINT)
				{
					param.data = PE_REG_H15_RD(param.addr);
					sprintf(buffer, "   [%04d] {0x%04X, 0x%08X},\n", \
						rd_cnt, param.addr, param.data);
					rd_cnt++;
				}
				else if (is_valid == PE_DNT_H15_HEX_PRINT_ONLY)
				{
					sprintf(buffer, "   [%04d] {0x%04X, 0x%08X},\n", \
						rd_cnt, param.addr, param.data);
					rd_cnt++;
				}
				else
				{
					sprintf(buffer, "%s", PE_DNT_H15_PRINT_END);
					rd_cnt = 0;
				}
			}
			memcpy(pstParams->data, buffer, sizeof(char)*PE_TRACE_STR_SIZE);
		}
	}while (0);
	#endif
	return ret;
}

