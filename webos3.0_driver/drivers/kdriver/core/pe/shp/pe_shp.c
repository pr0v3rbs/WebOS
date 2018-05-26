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

/** @file pe_shp.c
 *
 *  driver for picture enhance sharpness functions.
 *  ( used only within kdriver )
 *
 *	@author		Seung-Jun, Youm(sj.youm@lge.com)
 *	@version	0.1
 *	@note
 *	@date		2011.06.11
 *	@see
 */

/*----------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------*/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/semaphore.h>
#include <linux/interrupt.h>
#include <asm/io.h>

#include "base_types.h"
#include "os_util.h"
#include "debug_util.h"

#include "pe_cfg.h"
#include "pe_reg.h"
#include "pe_def.h"
#include "pe_etc.h"
#include "pe_cmn.h"
#include "pe_shp.h"
#include "pe_etc_hw.h"
#include "pe_shp_hw.h"
#include "pe_shp_hw_def.h"
#include "pe_shp_scl_hw.h"

/*----------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------*/
#define PE_SHP_DBG_PRINT(fmt, args...)	\
	if (_g_shp_trace)\
	{\
		printk("[%x,%x][%s,%d] "fmt, \
			PE_CHIP_VER, g_pe_kdrv_ver_mask, __F__, __L__, ##args);\
	}

/*----------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------*/
typedef struct
{
	int (*hw_init)(PE_CFG_CTRL_T *pstParams);
	int (*set_default)(LX_PE_DEFAULT_SETTINGS_T *pstParams);
	int (*set_debug)(LX_PE_DBG_SETTINGS_T *pstParams);
	int (*set_scl_filter)(LX_PE_SHP_SCLFILTER_T *pstParams);
	int (*get_scl_filter)(LX_PE_SHP_SCLFILTER_T *pstParams);
	int (*set_res_cmn)(void *pstParams);
	int (*get_res_cmn)(void *pstParams);
	int (*set_res_hor)(void *pstParams);
	int (*get_res_hor)(void *pstParams);
	int (*set_res_ver)(void *pstParams);
	int (*get_res_ver)(void *pstParams);
	int (*set_res_misc)(void *pstParams);
	int (*get_res_misc)(void *pstParams);
	int (*set_edge_enh)(void *pstParams);
	int (*get_edge_enh)(void *pstParams);
	int (*set_detail_enh)(void *pstParams);
	int (*get_detail_enh)(void *pstParams);
	int (*set_sre)(void *pstParams);
	int (*get_sre)(void *pstParams);
	#ifdef PE_SHP_PARAM_CHANGE_FOR_H15
	int (*set_sre_scl_filter)(LX_PE_SHP_SR_SCLFILTER_T *pstParams);
	int (*get_sre_scl_filter)(LX_PE_SHP_SR_SCLFILTER_T *pstParams);
	int (*set_sre_cmn)(void *pstParams);
	int (*get_sre_cmn)(void *pstParams);
	int (*set_sre_sclflt)(void *pstParams);
	int (*get_sre_sclflt)(void *pstParams);
	int (*set_sre_res_cmn)(void *pstParams);
	int (*get_sre_res_cmn)(void *pstParams);
	int (*set_sre_res_hor)(void *pstParams);
	int (*get_sre_res_hor)(void *pstParams);
	int (*set_sre_res_ver)(void *pstParams);
	int (*get_sre_res_ver)(void *pstParams);
	int (*set_sre_res_misc)(void *pstParams);
	int (*get_sre_res_misc)(void *pstParams);
	int (*set_sre_edge_enh)(void *pstParams);
	int (*get_sre_edge_enh)(void *pstParams);
	int (*set_sre_detail_enh)(void *pstParams);
	int (*get_sre_detail_enh)(void *pstParams);
	#endif
}
PE_SHP_CB_FUNC_T;

/*----------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
	global Functions
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------*/
static int PE_SHP_SetDefaultSettings(LX_PE_DEFAULT_SETTINGS_T *pstParams);
static int PE_SHP_SetDebugSettings(LX_PE_DBG_SETTINGS_T *pstParams);
static int PE_SHP_SetScalerFilterCoeff(LX_PE_SHP_SCLFILTER_T *pstParams);
static int PE_SHP_GetScalerFilterCoeff(LX_PE_SHP_SCLFILTER_T *pstParams);
static int PE_SHP_SetReCmnCtrl(void *pstParams);
static int PE_SHP_GetReCmnCtrl(void *pstParams);
static int PE_SHP_SetReHorCtrl(void *pstParams);
static int PE_SHP_GetReHorCtrl(void *pstParams);
static int PE_SHP_SetReVerCtrl(void *pstParams);
static int PE_SHP_GetReVerCtrl(void *pstParams);
static int PE_SHP_SetReMiscCtrl(void *pstParams);
static int PE_SHP_GetReMiscCtrl(void *pstParams);
static int PE_SHP_SetEdgeEnhanceCtrl(void *pstParams);
static int PE_SHP_GetEdgeEnhanceCtrl(void *pstParams);
static int PE_SHP_SetDetailEnhanceCtrl(void *pstParams);
static int PE_SHP_GetDetailEnhanceCtrl(void *pstParams);
static int PE_SHP_SetSreCtrl(void *pstParams);
static int PE_SHP_GetSreCtrl(void *pstParams);
#ifdef PE_SHP_PARAM_CHANGE_FOR_H15
static int PE_SHP_SetSreScalerFilterCoeff(LX_PE_SHP_SR_SCLFILTER_T *pstParams);
static int PE_SHP_GetSreScalerFilterCoeff(LX_PE_SHP_SR_SCLFILTER_T *pstParams);
static int PE_SHP_SetSreCmnCtrl(void *pstParams);
static int PE_SHP_GetSreCmnCtrl(void *pstParams);
static int PE_SHP_SetSreSclFlt(void *pstParams);
static int PE_SHP_GetSreSclFlt(void *pstParams);
static int PE_SHP_SetSreReCmnCtrl(void *pstParams);
static int PE_SHP_GetSreReCmnCtrl(void *pstParams);
static int PE_SHP_SetSreReHorCtrl(void *pstParams);
static int PE_SHP_GetSreReHorCtrl(void *pstParams);
static int PE_SHP_SetSreReVerCtrl(void *pstParams);
static int PE_SHP_GetSreReVerCtrl(void *pstParams);
static int PE_SHP_SetSreReMiscCtrl(void *pstParams);
static int PE_SHP_GetSreReMiscCtrl(void *pstParams);
static int PE_SHP_SetSreEdgeEnhanceCtrl(void *pstParams);
static int PE_SHP_GetSreEdgeEnhanceCtrl(void *pstParams);
static int PE_SHP_SetSreDetailEnhanceCtrl(void *pstParams);
static int PE_SHP_GetSreDetailEnhanceCtrl(void *pstParams);
#endif


/*----------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------*/
static PE_SHP_CB_FUNC_T _g_pe_shp_cb_fn;
static UINT32 _g_shp_trace=0x0;	//default should be off.

/*============================================================================
	Implementation Group
============================================================================*/
/**
 * register call back functions
 *
 * @param   void
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_SHP_RegisterCbFunc(void)
{
	int ret = RET_OK;

	memset(&_g_pe_shp_cb_fn, 0, sizeof(PE_SHP_CB_FUNC_T));
	do{
		#ifdef USE_PE_KDRV_CODES_FOR_H15
		if (PE_KDRV_VER_H15)
		{
			_g_pe_shp_cb_fn.hw_init				= PE_SHP_HW_H15_Init;
			_g_pe_shp_cb_fn.set_default			= PE_SHP_HW_H15_SetDefault;
			_g_pe_shp_cb_fn.set_debug			= PE_SHP_HW_H15_SetDebugSettings;
			_g_pe_shp_cb_fn.set_scl_filter		= PE_SHP_HW_H15_SetScalerFilterCoeff;
			_g_pe_shp_cb_fn.get_scl_filter		= PE_SHP_HW_H15_GetScalerFilterCoeff;
			_g_pe_shp_cb_fn.set_res_cmn			= PE_SHP_HW_H15_SetReCmnCtrl;
			_g_pe_shp_cb_fn.get_res_cmn			= PE_SHP_HW_H15_GetReCmnCtrl;
			_g_pe_shp_cb_fn.set_res_hor			= PE_SHP_HW_H15_SetReHorCtrl;
			_g_pe_shp_cb_fn.get_res_hor			= PE_SHP_HW_H15_GetReHorCtrl;
			_g_pe_shp_cb_fn.set_res_ver			= PE_SHP_HW_H15_SetReVerCtrl;
			_g_pe_shp_cb_fn.get_res_ver			= PE_SHP_HW_H15_GetReVerCtrl;
			_g_pe_shp_cb_fn.set_res_misc		= PE_SHP_HW_H15_SetReMiscCtrl;
			_g_pe_shp_cb_fn.get_res_misc		= PE_SHP_HW_H15_GetReMiscCtrl;
			_g_pe_shp_cb_fn.set_edge_enh		= PE_SHP_HW_H15_SetEdgeEnhanceCtrl;
			_g_pe_shp_cb_fn.get_edge_enh		= PE_SHP_HW_H15_GetEdgeEnhanceCtrl;
			_g_pe_shp_cb_fn.set_detail_enh		= PE_SHP_HW_H15_SetDetailEnhanceCtrl;
			_g_pe_shp_cb_fn.get_detail_enh		= PE_SHP_HW_H15_GetDetailEnhanceCtrl;
			_g_pe_shp_cb_fn.set_sre				= PE_SHP_HW_H15_SetSreCtrl;
			_g_pe_shp_cb_fn.get_sre				= PE_SHP_HW_H15_GetSreCtrl;
			#ifdef PE_SHP_PARAM_CHANGE_FOR_H15
			_g_pe_shp_cb_fn.set_sre_scl_filter	= PE_SHP_HW_H15_SetSreScalerFilterCoeff;
			_g_pe_shp_cb_fn.get_sre_scl_filter	= PE_SHP_HW_H15_GetSreScalerFilterCoeff;
			_g_pe_shp_cb_fn.set_sre_cmn			= PE_SHP_HW_H15_SetSreCmnCtrl;
			_g_pe_shp_cb_fn.get_sre_cmn			= PE_SHP_HW_H15_GetSreCmnCtrl;
			_g_pe_shp_cb_fn.set_sre_sclflt		= PE_SHP_HW_H15_SetSreSclFlt;
			_g_pe_shp_cb_fn.get_sre_sclflt		= PE_SHP_HW_H15_GetSreSclFlt;
			_g_pe_shp_cb_fn.set_sre_res_cmn		= PE_SHP_HW_H15_SetSreReCmnCtrl;
			_g_pe_shp_cb_fn.get_sre_res_cmn		= PE_SHP_HW_H15_GetSreReCmnCtrl;
			_g_pe_shp_cb_fn.set_sre_res_hor		= PE_SHP_HW_H15_SetSreReHorCtrl;
			_g_pe_shp_cb_fn.get_sre_res_hor		= PE_SHP_HW_H15_GetSreReHorCtrl;
			_g_pe_shp_cb_fn.set_sre_res_ver		= PE_SHP_HW_H15_SetSreReVerCtrl;
			_g_pe_shp_cb_fn.get_sre_res_ver		= PE_SHP_HW_H15_GetSreReVerCtrl;
			_g_pe_shp_cb_fn.set_sre_res_misc	= PE_SHP_HW_H15_SetSreReMiscCtrl;
			_g_pe_shp_cb_fn.get_sre_res_misc	= PE_SHP_HW_H15_GetSreReMiscCtrl;
			_g_pe_shp_cb_fn.set_sre_edge_enh	= PE_SHP_HW_H15_SetSreEdgeEnhanceCtrl;
			_g_pe_shp_cb_fn.get_sre_edge_enh	= PE_SHP_HW_H15_GetSreEdgeEnhanceCtrl;
			_g_pe_shp_cb_fn.set_sre_detail_enh	= PE_SHP_HW_H15_SetSreDetailEnhanceCtrl;
			_g_pe_shp_cb_fn.get_sre_detail_enh	= PE_SHP_HW_H15_GetSreDetailEnhanceCtrl;
			#endif
		}
		else
		#endif
		#ifdef USE_PE_KDRV_CODES_FOR_M14
		if (PE_KDRV_VER_M14)
		{
			_g_pe_shp_cb_fn.hw_init				= PE_SHP_HW_M14_Init;
			_g_pe_shp_cb_fn.set_default			= PE_SHP_HW_M14_SetDefault;
			_g_pe_shp_cb_fn.set_debug			= PE_SHP_HW_M14_SetDebugSettings;
			_g_pe_shp_cb_fn.set_scl_filter		= PE_SHP_HW_M14_SetScalerFilterCoeff;
			_g_pe_shp_cb_fn.get_scl_filter		= PE_SHP_HW_M14_GetScalerFilterCoeff;
			_g_pe_shp_cb_fn.set_res_cmn			= PE_SHP_HW_M14_SetReCmnCtrl;
			_g_pe_shp_cb_fn.get_res_cmn			= PE_SHP_HW_M14_GetReCmnCtrl;
			_g_pe_shp_cb_fn.set_res_hor			= PE_SHP_HW_M14_SetReHorCtrl;
			_g_pe_shp_cb_fn.get_res_hor			= PE_SHP_HW_M14_GetReHorCtrl;
			_g_pe_shp_cb_fn.set_res_ver			= PE_SHP_HW_M14_SetReVerCtrl;
			_g_pe_shp_cb_fn.get_res_ver			= PE_SHP_HW_M14_GetReVerCtrl;
			_g_pe_shp_cb_fn.set_res_misc		= PE_SHP_HW_M14_SetReMiscCtrl;
			_g_pe_shp_cb_fn.get_res_misc		= PE_SHP_HW_M14_GetReMiscCtrl;
			_g_pe_shp_cb_fn.set_edge_enh		= PE_SHP_HW_M14_SetEdgeEnhanceCtrl;
			_g_pe_shp_cb_fn.get_edge_enh		= PE_SHP_HW_M14_GetEdgeEnhanceCtrl;
			_g_pe_shp_cb_fn.set_detail_enh		= PE_SHP_HW_M14_SetDetailEnhanceCtrl;
			_g_pe_shp_cb_fn.get_detail_enh		= PE_SHP_HW_M14_GetDetailEnhanceCtrl;
			_g_pe_shp_cb_fn.set_sre				= PE_SHP_HW_M14_SetSreCtrl;
			_g_pe_shp_cb_fn.get_sre				= PE_SHP_HW_M14_GetSreCtrl;
		}
		else
		#endif
		#ifdef USE_PE_KDRV_CODES_FOR_H14
		if (PE_KDRV_VER_H14)
		{
			_g_pe_shp_cb_fn.hw_init				= PE_SHP_HW_H14_Init;
			_g_pe_shp_cb_fn.set_default			= PE_SHP_HW_H14_SetDefault;
			_g_pe_shp_cb_fn.set_debug			= PE_SHP_HW_H14_SetDebugSettings;
			_g_pe_shp_cb_fn.set_scl_filter		= PE_SHP_HW_H14_SetScalerFilterCoeff;
			_g_pe_shp_cb_fn.get_scl_filter		= PE_SHP_HW_H14_GetScalerFilterCoeff;
			_g_pe_shp_cb_fn.set_res_cmn			= PE_SHP_HW_H14_SetReCmnCtrl;
			_g_pe_shp_cb_fn.get_res_cmn			= PE_SHP_HW_H14_GetReCmnCtrl;
			_g_pe_shp_cb_fn.set_res_hor			= PE_SHP_HW_H14_SetReHorCtrl;
			_g_pe_shp_cb_fn.get_res_hor			= PE_SHP_HW_H14_GetReHorCtrl;
			_g_pe_shp_cb_fn.set_res_ver			= PE_SHP_HW_H14_SetReVerCtrl;
			_g_pe_shp_cb_fn.get_res_ver			= PE_SHP_HW_H14_GetReVerCtrl;
			_g_pe_shp_cb_fn.set_res_misc		= PE_SHP_HW_H14_SetReMiscCtrl;
			_g_pe_shp_cb_fn.get_res_misc		= PE_SHP_HW_H14_GetReMiscCtrl;
			_g_pe_shp_cb_fn.set_edge_enh		= PE_SHP_HW_H14_SetEdgeEnhanceCtrl;
			_g_pe_shp_cb_fn.get_edge_enh		= PE_SHP_HW_H14_GetEdgeEnhanceCtrl;
			_g_pe_shp_cb_fn.set_detail_enh		= PE_SHP_HW_H14_SetDetailEnhanceCtrl;
			_g_pe_shp_cb_fn.get_detail_enh		= PE_SHP_HW_H14_GetDetailEnhanceCtrl;
			_g_pe_shp_cb_fn.set_sre				= NULL;
			_g_pe_shp_cb_fn.get_sre				= NULL;
		}
		else
		#endif
		#ifdef USE_PE_KDRV_CODES_FOR_H13
		if (PE_KDRV_VER_H13)
		{
			_g_pe_shp_cb_fn.hw_init				= PE_SHP_HW_H13_Init;
			_g_pe_shp_cb_fn.set_default			= PE_SHP_HW_H13_SetDefault;
			_g_pe_shp_cb_fn.set_debug			= PE_SHP_HW_H13_SetDebugSettings;
			_g_pe_shp_cb_fn.set_scl_filter		= PE_SHP_HW_H13_SetScalerFilterCoeff;
			_g_pe_shp_cb_fn.get_scl_filter		= PE_SHP_HW_H13_GetScalerFilterCoeff;
			_g_pe_shp_cb_fn.set_res_cmn			= PE_SHP_HW_H13_SetReCmnCtrl;
			_g_pe_shp_cb_fn.get_res_cmn			= PE_SHP_HW_H13_GetReCmnCtrl;
			_g_pe_shp_cb_fn.set_res_hor			= PE_SHP_HW_H13_SetReHorCtrl;
			_g_pe_shp_cb_fn.get_res_hor			= PE_SHP_HW_H13_GetReHorCtrl;
			_g_pe_shp_cb_fn.set_res_ver			= PE_SHP_HW_H13_SetReVerCtrl;
			_g_pe_shp_cb_fn.get_res_ver			= PE_SHP_HW_H13_GetReVerCtrl;
			_g_pe_shp_cb_fn.set_res_misc		= PE_SHP_HW_H13_SetReMiscCtrl;
			_g_pe_shp_cb_fn.get_res_misc		= PE_SHP_HW_H13_GetReMiscCtrl;
			_g_pe_shp_cb_fn.set_edge_enh		= PE_SHP_HW_H13_SetEdgeEnhanceCtrl;
			_g_pe_shp_cb_fn.get_edge_enh		= PE_SHP_HW_H13_GetEdgeEnhanceCtrl;
			_g_pe_shp_cb_fn.set_detail_enh		= PE_SHP_HW_H13_SetDetailEnhanceCtrl;
			_g_pe_shp_cb_fn.get_detail_enh		= PE_SHP_HW_H13_GetDetailEnhanceCtrl;
			_g_pe_shp_cb_fn.set_sre				= NULL;
			_g_pe_shp_cb_fn.get_sre				= NULL;
		}
		else
		#endif
		{
			PE_PRINT_ERROR("[%x,%x] kdrv chip ver is wrong.\n", \
				PE_CHIP_VER, g_pe_kdrv_ver_mask);
			ret = RET_ERROR;
		}
	}while (0);
	return ret;
}

/**
 * init shp
 *
 * @param   *pstParams [in] PE_CFG_CTRL_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_SHP_Init(PE_CFG_CTRL_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(_g_pe_shp_cb_fn.hw_init)
		{
			ret = _g_pe_shp_cb_fn.hw_init(pstParams);
			PE_CHECK_CODE(ret, break, "hw_init() error.\n");
		}
	}while (0);
	return ret;
}

/**
 * standard function of shp setting
 *
 * @param   data [in/out] unsigned int
 * @param   functype [in/out] unsigned int
 * @param   datasize [in/out] unsigned int
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_SHP_SetFunction(unsigned int *data, unsigned int functype, unsigned int datasize)
{
	int ret = RET_ERROR;

#define _VOID___INPUT_		1
#define _STRUCT_INPUT_		0

#define PE_SHP_SET_FUNC(_struct, _func, _void_input)	\
	case PE_ITEM_PKTFUNC(_struct):\
	{\
		_struct *p_rcv, param;\
		do{\
			PE_SHP_DBG_PRINT("S %s\n", #_struct);\
			PE_CHECK_CODE(datasize!=sizeof(_struct), break, \
				"[%s,%d] size mismatch.\n", __F__, __L__);\
			p_rcv = (_struct *)data;\
			memcpy(&param, p_rcv, datasize);\
			ret = (_void_input)? (_func((void *)&param)):(_func(&param));\
			PE_CHECK_CODE(ret, break, \
				"[%s,%d] %s() error.\n", __F__, __L__, #_func);\
			PE_SHP_DBG_PRINT("E %s\n", #_struct);\
		}while (0);\
	}\
	break

	/* data null check */
	PE_CHECK_CODE(!data, return RET_ERROR, \
		"[%s,%d] data is null.\n", __F__, __L__);
	switch (functype)
	{
		PE_SHP_SET_FUNC(LX_PE_SHP_SCLFILTER_T,		PE_SHP_SetScalerFilterCoeff,_STRUCT_INPUT_);
		PE_SHP_SET_FUNC(LX_PE_SHP_RE1_CMN_T,		PE_SHP_SetReCmnCtrl,		_VOID___INPUT_);
		PE_SHP_SET_FUNC(LX_PE_SHP_RE1_HOR_T,		PE_SHP_SetReHorCtrl,		_VOID___INPUT_);
		PE_SHP_SET_FUNC(LX_PE_SHP_RE1_VER_T,		PE_SHP_SetReVerCtrl,		_VOID___INPUT_);
		PE_SHP_SET_FUNC(LX_PE_SHP_RE1_MISC_T,		PE_SHP_SetReMiscCtrl,		_VOID___INPUT_);
		PE_SHP_SET_FUNC(LX_PE_SHP_EE1_T,			PE_SHP_SetEdgeEnhanceCtrl,	_VOID___INPUT_);
		PE_SHP_SET_FUNC(LX_PE_SHP_DE1_T,			PE_SHP_SetDetailEnhanceCtrl,_VOID___INPUT_);
		PE_SHP_SET_FUNC(LX_PE_SHP_RE2_CMN_T,		PE_SHP_SetReCmnCtrl,		_VOID___INPUT_);
		PE_SHP_SET_FUNC(LX_PE_SHP_RE2_HOR_T,		PE_SHP_SetReHorCtrl,		_VOID___INPUT_);
		PE_SHP_SET_FUNC(LX_PE_SHP_RE2_VER_T,		PE_SHP_SetReVerCtrl,		_VOID___INPUT_);
		PE_SHP_SET_FUNC(LX_PE_SHP_RE2_MISC_T,		PE_SHP_SetReMiscCtrl,		_VOID___INPUT_);
		PE_SHP_SET_FUNC(LX_PE_SHP_RE3_MISC_T,		PE_SHP_SetReMiscCtrl,		_VOID___INPUT_);
		PE_SHP_SET_FUNC(LX_PE_SHP_SR0_T,			PE_SHP_SetSreCtrl,			_VOID___INPUT_);
		PE_SHP_SET_FUNC(LX_PE_SHP_SR1_T,			PE_SHP_SetSreCtrl,			_VOID___INPUT_);
		#ifdef PE_SHP_PARAM_CHANGE_FOR_H15
		PE_SHP_SET_FUNC(LX_PE_SHP_SR_SCLFILTER_T,	PE_SHP_SetSreScalerFilterCoeff,_STRUCT_INPUT_);
		PE_SHP_SET_FUNC(LX_PE_SHP_RE3_CMN_T,		PE_SHP_SetReCmnCtrl,		_VOID___INPUT_);
		PE_SHP_SET_FUNC(LX_PE_SHP_RE4_MISC_T,		PE_SHP_SetReMiscCtrl,		_VOID___INPUT_);
		PE_SHP_SET_FUNC(LX_PE_SHP_SR_MAIN_T,		PE_SHP_SetSreCtrl,			_VOID___INPUT_);
		PE_SHP_SET_FUNC(LX_PE_SHP_SR_CMN_T,			PE_SHP_SetSreCmnCtrl,		_VOID___INPUT_);
		PE_SHP_SET_FUNC(LX_PE_SHP_SR_SCLFILTER_COEFF_T,	PE_SHP_SetSreSclFlt,	_VOID___INPUT_);
		PE_SHP_SET_FUNC(LX_PE_SHP_SR_RE_CMN_T,		PE_SHP_SetSreReCmnCtrl,		_VOID___INPUT_);
		PE_SHP_SET_FUNC(LX_PE_SHP_SR_RE_HOR_T,		PE_SHP_SetSreReHorCtrl,		_VOID___INPUT_);
		PE_SHP_SET_FUNC(LX_PE_SHP_SR_RE_VER_T,		PE_SHP_SetSreReVerCtrl,		_VOID___INPUT_);
		PE_SHP_SET_FUNC(LX_PE_SHP_SR_RE_MISC_T,		PE_SHP_SetSreReMiscCtrl,	_VOID___INPUT_);
		PE_SHP_SET_FUNC(LX_PE_SHP_SR_EE_T,			PE_SHP_SetSreEdgeEnhanceCtrl,	_VOID___INPUT_);
		PE_SHP_SET_FUNC(LX_PE_SHP_SR_DE_T,			PE_SHP_SetSreDetailEnhanceCtrl,_VOID___INPUT_);
		#endif
		PE_SHP_SET_FUNC(LX_PE_DEFAULT_SETTINGS_T,	PE_SHP_SetDefaultSettings,	_STRUCT_INPUT_);
		PE_SHP_SET_FUNC(LX_PE_DBG_SETTINGS_T,		PE_SHP_SetDebugSettings,	_STRUCT_INPUT_);
		default:
		{
			PE_PRINT_ERROR("functype(%d) default case.\n", functype);
			ret = RET_ERROR;
		}
		break;
	}
	return ret;
}

/**
 * standard function of shp getting
 *
 * @param   data [in/out] unsigned int
 * @param   functype [in/out] unsigned int
 * @param   datasize [in/out] unsigned int
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_SHP_GetFunction(unsigned int *data, unsigned int functype, unsigned int datasize)
{
	int ret = RET_ERROR;

#define _VOID___INPUT_		1
#define _STRUCT_INPUT_		0

#define PE_SHP_GET_FUNC(_struct, _func, _void_input)	\
	case PE_ITEM_PKTFUNC(_struct):\
	{\
		_struct *p_rcv, param;\
		do{\
			PE_SHP_DBG_PRINT("S %s\n", #_struct);\
			PE_CHECK_CODE(datasize!=sizeof(_struct), break, \
				"[%s,%d] size mismatch.\n", __F__, __L__);\
			p_rcv = (_struct *)data;\
			memcpy(&param, p_rcv, datasize);\
			ret = (_void_input)? (_func((void *)&param)):(_func(&param));\
			PE_CHECK_CODE(ret, break, \
				"[%s,%d] %s() error.\n", __F__, __L__, #_func);\
			memcpy(p_rcv, &param, datasize);\
			PE_SHP_DBG_PRINT("E %s\n", #_struct);\
		}while (0);\
	}\
	break

	/* data null check */
	PE_CHECK_CODE(!data, return RET_ERROR, \
		"[%s,%d] data is null.\n", __F__, __L__);
	switch (functype)
	{
		PE_SHP_GET_FUNC(LX_PE_SHP_SCLFILTER_T,		PE_SHP_GetScalerFilterCoeff,_STRUCT_INPUT_);
		PE_SHP_GET_FUNC(LX_PE_SHP_RE1_CMN_T,		PE_SHP_GetReCmnCtrl,		_VOID___INPUT_);
		PE_SHP_GET_FUNC(LX_PE_SHP_RE1_HOR_T,		PE_SHP_GetReHorCtrl,		_VOID___INPUT_);
		PE_SHP_GET_FUNC(LX_PE_SHP_RE1_VER_T,		PE_SHP_GetReVerCtrl,		_VOID___INPUT_);
		PE_SHP_GET_FUNC(LX_PE_SHP_RE1_MISC_T,		PE_SHP_GetReMiscCtrl,		_VOID___INPUT_);
		PE_SHP_GET_FUNC(LX_PE_SHP_EE1_T,			PE_SHP_GetEdgeEnhanceCtrl,	_VOID___INPUT_);
		PE_SHP_GET_FUNC(LX_PE_SHP_DE1_T,			PE_SHP_GetDetailEnhanceCtrl,_VOID___INPUT_);
		PE_SHP_GET_FUNC(LX_PE_SHP_RE2_CMN_T,		PE_SHP_GetReCmnCtrl,		_VOID___INPUT_);
		PE_SHP_GET_FUNC(LX_PE_SHP_RE2_HOR_T,		PE_SHP_GetReHorCtrl,		_VOID___INPUT_);
		PE_SHP_GET_FUNC(LX_PE_SHP_RE2_VER_T,		PE_SHP_GetReVerCtrl,		_VOID___INPUT_);
		PE_SHP_GET_FUNC(LX_PE_SHP_RE2_MISC_T,		PE_SHP_GetReMiscCtrl,		_VOID___INPUT_);
		PE_SHP_GET_FUNC(LX_PE_SHP_RE3_MISC_T,		PE_SHP_GetReMiscCtrl,		_VOID___INPUT_);
		PE_SHP_GET_FUNC(LX_PE_SHP_SR0_T,			PE_SHP_GetSreCtrl,			_VOID___INPUT_);
		PE_SHP_GET_FUNC(LX_PE_SHP_SR1_T,			PE_SHP_GetSreCtrl,			_VOID___INPUT_);
		#ifdef PE_SHP_PARAM_CHANGE_FOR_H15
		PE_SHP_GET_FUNC(LX_PE_SHP_SR_SCLFILTER_T,		PE_SHP_GetSreScalerFilterCoeff,_STRUCT_INPUT_);
		PE_SHP_GET_FUNC(LX_PE_SHP_RE3_CMN_T,		PE_SHP_GetReCmnCtrl,		_VOID___INPUT_);
		PE_SHP_GET_FUNC(LX_PE_SHP_RE4_MISC_T,		PE_SHP_GetReMiscCtrl,		_VOID___INPUT_);
		PE_SHP_GET_FUNC(LX_PE_SHP_SR_MAIN_T,		PE_SHP_GetSreCtrl,			_VOID___INPUT_);
		PE_SHP_GET_FUNC(LX_PE_SHP_SR_CMN_T,			PE_SHP_GetSreCmnCtrl,		_VOID___INPUT_);
		PE_SHP_GET_FUNC(LX_PE_SHP_SR_SCLFILTER_COEFF_T,	PE_SHP_GetSreSclFlt,		_VOID___INPUT_);
		PE_SHP_GET_FUNC(LX_PE_SHP_SR_RE_CMN_T,		PE_SHP_GetSreReCmnCtrl,		_VOID___INPUT_);
		PE_SHP_GET_FUNC(LX_PE_SHP_SR_RE_HOR_T,		PE_SHP_GetSreReHorCtrl,		_VOID___INPUT_);
		PE_SHP_GET_FUNC(LX_PE_SHP_SR_RE_VER_T,		PE_SHP_GetSreReVerCtrl,		_VOID___INPUT_);
		PE_SHP_GET_FUNC(LX_PE_SHP_SR_RE_MISC_T,		PE_SHP_GetSreReMiscCtrl,	_VOID___INPUT_);
		PE_SHP_GET_FUNC(LX_PE_SHP_SR_EE_T,			PE_SHP_GetSreEdgeEnhanceCtrl,	_VOID___INPUT_);
		PE_SHP_GET_FUNC(LX_PE_SHP_SR_DE_T,			PE_SHP_GetSreDetailEnhanceCtrl, _VOID___INPUT_);
		#endif
		default:
		{
			PE_PRINT_ERROR("functype(%d) default case.\n", functype);
			ret = RET_ERROR;
		}
		break;
	}
	return ret;
}

/**
 * set default settings
 *
 * @param   *pstParams [in] LX_PE_DEFAULT_SETTINGS_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_SHP_SetDefaultSettings(LX_PE_DEFAULT_SETTINGS_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(_g_pe_shp_cb_fn.set_default)
		{
			ret = _g_pe_shp_cb_fn.set_default(pstParams);
			PE_CHECK_CODE(ret, break, "set_default() error.\n");
		}
	}while (0);
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
static int PE_SHP_SetDebugSettings(LX_PE_DBG_SETTINGS_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		/* set print level */
		if(pstParams->type&LX_PE_DBG_LV)
		{
			_g_shp_trace = (pstParams->print_lvl & \
				(0x1<<PE_ITEM_PKTMODL(SHP)))? 0x1:0x0;
		}
		if(_g_pe_shp_cb_fn.set_debug)
		{
			ret = _g_pe_shp_cb_fn.set_debug(pstParams);
			PE_CHECK_CODE(ret, break, "set_debug() error.\n");
		}
	}while (0);
	return ret;
}
/**
 * set scaler filter coeff
 *
 * @param   *pstParams [in] LX_PE_SHP_SCLFILTER_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_SHP_SetScalerFilterCoeff(LX_PE_SHP_SCLFILTER_T *pstParams)
{
	int ret = RET_ERROR;
	do{
		CHECK_KNULL(pstParams);
		if(_g_pe_shp_cb_fn.set_scl_filter)
		{
			ret = _g_pe_shp_cb_fn.set_scl_filter(pstParams);
			PE_CHECK_CODE(ret, break, "set_scl_filter() error.\n");
		}
	}while (0);
	return ret;
}
/**
 * get scaler filter coeff
 *
 * @param   *pstParams [in/out] LX_PE_SHP_SCLFILTER_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_SHP_GetScalerFilterCoeff(LX_PE_SHP_SCLFILTER_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(_g_pe_shp_cb_fn.get_scl_filter)
		{
			ret = _g_pe_shp_cb_fn.get_scl_filter(pstParams);
			PE_CHECK_CODE(ret, break, "get_scl_filter() error.\n");
		}
	}while (0);
	return ret;
}
/**
 * set resolution common control
 * - use input struct LX_PE_SHP_RE2_CMN_T for H13BX and H14A
 * - use input struct LX_PE_SHP_RE1_CMN_T for H13AX
 *
 * @param   *pstParams [in] void
 * @return  OK if success, ERROR otherwise.
 * @see		function description
 * @author
 */
static int PE_SHP_SetReCmnCtrl(void *pstParams)
{
	int ret = RET_OK;
	do {
		CHECK_KNULL(pstParams);
		if(_g_pe_shp_cb_fn.set_res_cmn)
		{
			ret = _g_pe_shp_cb_fn.set_res_cmn(pstParams);
			PE_CHECK_CODE(ret, break, "set_res_cmn() error.\n");
		}
	}while (0);
	return ret;
}
/**
 * get resolution common control
 * - use input struct LX_PE_SHP_RE2_CMN_T for H13BX and H14A
 * - use input struct LX_PE_SHP_RE1_CMN_T for H13AX
 *
 * @param   *pstParams [in/out] void
 * @return  OK if success, ERROR otherwise.
 * @see		function description
 * @author
 */
static int PE_SHP_GetReCmnCtrl(void *pstParams)
{
	int ret = RET_OK;
	do {
		CHECK_KNULL(pstParams);
		if(_g_pe_shp_cb_fn.get_res_cmn)
		{
			ret = _g_pe_shp_cb_fn.get_res_cmn(pstParams);
			PE_CHECK_CODE(ret, break, "get_res_cmn() error.\n");
		}
	}while (0);
	return ret;
}
/**
 * set resolution horizontal control
 * - use input struct LX_PE_SHP_RE2_HOR_T for H13BX and H14A
 * - use input struct LX_PE_SHP_RE1_HOR_T for H13AX
 *
 * @param   *pstParams [in/out] void
 * @return  OK if success, ERROR otherwise.
 * @see		function description
 * @author
 */
static int PE_SHP_SetReHorCtrl(void *pstParams)
{
	int ret = RET_OK;
	do {
		CHECK_KNULL(pstParams);
		if(_g_pe_shp_cb_fn.set_res_hor)
		{
			ret = _g_pe_shp_cb_fn.set_res_hor(pstParams);
			PE_CHECK_CODE(ret, break, "set_res_hor() error.\n");
		}
	}while (0);
	return ret;
}
/**
 * get resolution horizontal control
 * - use input struct LX_PE_SHP_RE1_HOR_T for H13 and H14A
 *
 * @param   *pstParams [in/out] void
 * @return  OK if success, ERROR otherwise.
 * @see		function description
 * @author
 */
static int PE_SHP_GetReHorCtrl(void *pstParams)
{
	int ret = RET_OK;
	do {
		CHECK_KNULL(pstParams);
		if(_g_pe_shp_cb_fn.get_res_hor)
		{
			ret = _g_pe_shp_cb_fn.get_res_hor(pstParams);
			PE_CHECK_CODE(ret, break, "get_res_hor() error.\n");
		}
	}while (0);
	return ret;
}
/**
 * set resolution vertical control
 * - use input struct LX_PE_SHP_RE2_VER_T for H13BX and H14A
 * - use input struct LX_PE_SHP_RE1_VER_T for H13AX
 *
 * @param   *pstParams [in] void
 * @return  OK if success, ERROR otherwise.
 * @see		function description
 * @author
 */
static int PE_SHP_SetReVerCtrl(void *pstParams)
{
	int ret = RET_OK;
	do {
		CHECK_KNULL(pstParams);
		if(_g_pe_shp_cb_fn.set_res_ver)
		{
			ret = _g_pe_shp_cb_fn.set_res_ver(pstParams);
			PE_CHECK_CODE(ret, break, "set_res_ver() error.\n");
		}
	}while (0);
	return ret;
}
/**
 * get resolution vertical control
 * - use input struct LX_PE_SHP_RE2_VER_T for H13BX and H14A
 * - use input struct LX_PE_SHP_RE1_VER_T for H13AX
 *
 * @param   *pstParams [in/out] void
 * @return  OK if success, ERROR otherwise.
 * @see		function description
 * @author
 */
static int PE_SHP_GetReVerCtrl(void *pstParams)
{
	int ret = RET_OK;
	do {
		CHECK_KNULL(pstParams);
		if(_g_pe_shp_cb_fn.get_res_ver)
		{
			ret = _g_pe_shp_cb_fn.get_res_ver(pstParams);
			PE_CHECK_CODE(ret, break, "get_res_ver() error.\n");
		}
	}while (0);
	return ret;
}
/**
 * set resolution misc control
 * - use input struct LX_PE_SHP_RE2_MISC_T for H13BX and H14A
 * - use input struct LX_PE_SHP_RE1_MISC_T for H13AX
 *
 * @param   *pstParams [in] void
 * @return  OK if success, ERROR otherwise.
 * @see		function description
 * @author
 */
static int PE_SHP_SetReMiscCtrl(void *pstParams)
{
	int ret = RET_OK;
	do {
		CHECK_KNULL(pstParams);
		if(_g_pe_shp_cb_fn.set_res_misc)
		{
			ret = _g_pe_shp_cb_fn.set_res_misc(pstParams);
			PE_CHECK_CODE(ret, break, "set_res_misc() error.\n");
		}
	}while (0);
	return ret;
}
/**
 * get resolution misc control
 * - use input struct LX_PE_SHP_RE2_MISC_T for H13BX and H14A
 * - use input struct LX_PE_SHP_RE1_MISC_T for H13AX
 *
 * @param   *pstParams [in/out] void
 * @return  OK if success, ERROR otherwise.
 * @see		function description
 * @author
 */
static int PE_SHP_GetReMiscCtrl(void *pstParams)
{
	int ret = RET_OK;
	do {
		CHECK_KNULL(pstParams);
		if(_g_pe_shp_cb_fn.get_res_misc)
		{
			ret = _g_pe_shp_cb_fn.get_res_misc(pstParams);
			PE_CHECK_CODE(ret, break, "get_res_misc() error.\n");
		}
	}while (0);
	return ret;
}
/**
 * set edge enhance control
 * - use input struct LX_PE_SHP_EE1_T for H13 and H14A
 *
 * @param   *pstParams [in] void
 * @return  OK if success, ERROR otherwise.
 * @see		function description
 * @author
 */
static int PE_SHP_SetEdgeEnhanceCtrl(void *pstParams)
{
	int ret = RET_OK;
	do {
		CHECK_KNULL(pstParams);
		if(_g_pe_shp_cb_fn.set_edge_enh)
		{
			ret = _g_pe_shp_cb_fn.set_edge_enh(pstParams);
			PE_CHECK_CODE(ret, break, "set_edge_enh() error.\n");
		}
	}while (0);
	return ret;
}
/**
 * get edge enhance control
 * - use input struct LX_PE_SHP_EE1_T for H13 and H14A
 *
 * @param   *pstParams [in/out] void
 * @return  OK if success, ERROR otherwise.
 * @see		function description
 * @author
 */
static int PE_SHP_GetEdgeEnhanceCtrl(void *pstParams)
{
	int ret = RET_OK;
	do {
		CHECK_KNULL(pstParams);
		if(_g_pe_shp_cb_fn.get_edge_enh)
		{
			ret = _g_pe_shp_cb_fn.get_edge_enh(pstParams);
			PE_CHECK_CODE(ret, break, "get_edge_enh() error.\n");
		}
	}while (0);
	return ret;
}
/**
 * set detail enhance control
 * - use input struct LX_PE_SHP_DE1_T for H13 and H14A
 *
 * @param   *pstParams [in] void
 * @return  OK if success, ERROR otherwise.
 * @see		function description
 * @author
 */
static int PE_SHP_SetDetailEnhanceCtrl(void *pstParams)
{
	int ret = RET_OK;
	do {
		CHECK_KNULL(pstParams);
		if(_g_pe_shp_cb_fn.set_detail_enh)
		{
			ret = _g_pe_shp_cb_fn.set_detail_enh(pstParams);
			PE_CHECK_CODE(ret, break, "set_detail_enh() error.\n");
		}
	}while (0);
	return ret;
}
/**
 * get detail enhance control
 * - use input struct LX_PE_SHP_DE1_T for H13 and H14A
 *
 * @param   *pstParams [in/out] void
 * @return  OK if success, ERROR otherwise.
 * @see		function description
 * @author
 */
static int PE_SHP_GetDetailEnhanceCtrl(void *pstParams)
{
	int ret = RET_OK;
	do {
		CHECK_KNULL(pstParams);
		if(_g_pe_shp_cb_fn.get_detail_enh)
		{
			ret = _g_pe_shp_cb_fn.get_detail_enh(pstParams);
			PE_CHECK_CODE(ret, break, "get_detail_enh() error.\n");
		}
	}while (0);
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
static int PE_SHP_SetSreCtrl(void *pstParams)
{
	int ret = RET_OK;
	do {
		CHECK_KNULL(pstParams);
		if(_g_pe_shp_cb_fn.set_sre)
		{
			ret = _g_pe_shp_cb_fn.set_sre(pstParams);
			PE_CHECK_CODE(ret, break, "set_sre() error.\n");
		}
	}while (0);
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
static int PE_SHP_GetSreCtrl(void *pstParams)
{
	int ret = RET_OK;
	do {
		CHECK_KNULL(pstParams);
		if(_g_pe_shp_cb_fn.get_sre)
		{
			ret = _g_pe_shp_cb_fn.get_sre(pstParams);
			PE_CHECK_CODE(ret, break, "get_sre() error.\n");
		}
	}while (0);
	return ret;
}

#ifdef PE_SHP_PARAM_CHANGE_FOR_H15
/**
 * set sre scaler filter coeff
 *
 * @param   *pstParams [in] LX_PE_SHP_SR_SCLFILTER_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_SHP_SetSreScalerFilterCoeff(LX_PE_SHP_SR_SCLFILTER_T *pstParams)
{
	int ret = RET_ERROR;
	do{
		CHECK_KNULL(pstParams);
		if(_g_pe_shp_cb_fn.set_sre_scl_filter)
		{
			ret = _g_pe_shp_cb_fn.set_sre_scl_filter(pstParams);
			PE_CHECK_CODE(ret, break, "set_scl_filter() error.\n");
		}
	}while (0);
	return ret;
}
/**
 * get sre  scaler filter coeff
 *
 * @param   *pstParams [in/out] LX_PE_SHP_SR_SCLFILTER_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_SHP_GetSreScalerFilterCoeff(LX_PE_SHP_SR_SCLFILTER_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(_g_pe_shp_cb_fn.get_sre_scl_filter)
		{
			ret = _g_pe_shp_cb_fn.get_sre_scl_filter(pstParams);
			PE_CHECK_CODE(ret, break, "get_scl_filter() error.\n");
		}
	}while (0);
	return ret;
}

/**
 * set super resolution cmn control
 * - use input struct LX_PE_SHP_SR_CMN_T for H15
 *
 * @param   *pstParams [in] void
 * @return  OK if success, ERROR otherwise.
 * @see		function description
 * @author
 */
static int PE_SHP_SetSreCmnCtrl(void *pstParams)
{
	int ret = RET_OK;
	do {
		CHECK_KNULL(pstParams);
		if(_g_pe_shp_cb_fn.set_sre_cmn)
		{
			ret = _g_pe_shp_cb_fn.set_sre_cmn(pstParams);
			PE_CHECK_CODE(ret, break, "set_sre_cmn() error.\n");
		}
	}while (0);
	return ret;
}
/**
 * get  super resolution cmn control
 * - use input struct LX_PE_SHP_SR_CMN_T for H15
 *
 * @param   *pstParams [in/out] void
 * @return  OK if success, ERROR otherwise.
 * @see		function description
 * @author
 */
static int PE_SHP_GetSreCmnCtrl(void *pstParams)
{
	int ret = RET_OK;
	do {
		CHECK_KNULL(pstParams);
		if(_g_pe_shp_cb_fn.get_sre_cmn)
		{
			ret = _g_pe_shp_cb_fn.get_sre_cmn(pstParams);
			PE_CHECK_CODE(ret, break, "get_sre_cmn() error.\n");
		}
	}while (0);
	return ret;
}
/**
 * set sr scale filter coeffient parameter
 * - use input struct LX_PE_SHP_SR_SCLFILTER_COEFF_T for H15
 *
 * @param   *pstParams [in] void
 * @return  OK if success, ERROR otherwise.
 * @see		function description
 * @author
 */
static int PE_SHP_SetSreSclFlt(void *pstParams)
{
	int ret = RET_OK;
	do {
		CHECK_KNULL(pstParams);
		if(_g_pe_shp_cb_fn.set_sre_sclflt)
		{
			ret = _g_pe_shp_cb_fn.set_sre_sclflt(pstParams);
			PE_CHECK_CODE(ret, break, "set_sre_sclflt() error.\n");
		}
	}while (0);
	return ret;
}
/**
 * get sr scale filter coeffient parameter
 * - use input struct LX_PE_SHP_SR_SCLFILTER_COEFF_T for H15
 *
 * @param   *pstParams [in/out] void
 * @return  OK if success, ERROR otherwise.
 * @see		function description
 * @author
 */
static int PE_SHP_GetSreSclFlt(void *pstParams)
{
	int ret = RET_OK;
	do {
		CHECK_KNULL(pstParams);
		if(_g_pe_shp_cb_fn.get_sre_sclflt)
		{
			ret = _g_pe_shp_cb_fn.get_sre_sclflt(pstParams);
			PE_CHECK_CODE(ret, break, "get_sre_sclflt() error.\n");
		}
	}while (0);
	return ret;
}
/**
 * set sr resolution enhance common parameter
 * - use input struct LX_PE_SHP_SR_RE_CMN_T for H15
 *
 * @param   *pstParams [in] void
 * @return  OK if success, ERROR otherwise.
 * @see		function description
 * @author
 */
static int PE_SHP_SetSreReCmnCtrl(void *pstParams)
{
	int ret = RET_OK;
	do {
		CHECK_KNULL(pstParams);
		if(_g_pe_shp_cb_fn.set_sre_res_cmn)
		{
			ret = _g_pe_shp_cb_fn.set_sre_res_cmn(pstParams);
			PE_CHECK_CODE(ret, break, "set_sre_res_cmn() error.\n");
		}
	}while (0);
	return ret;
}
/**
 * get sr resolution enhance common parameter
 * - use input struct LX_PE_SHP_SR_RE_CMN_T for H15
 *
 * @param   *pstParams [in/out] void
 * @return  OK if success, ERROR otherwise.
 * @see		function description
 * @author
 */
static int PE_SHP_GetSreReCmnCtrl(void *pstParams)
{
	int ret = RET_OK;
	do {
		CHECK_KNULL(pstParams);
		if(_g_pe_shp_cb_fn.get_sre_res_cmn)
		{
			ret = _g_pe_shp_cb_fn.get_sre_res_cmn(pstParams);
			PE_CHECK_CODE(ret, break, "get_sre_res_cmn() error.\n");
		}
	}while (0);
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
static int PE_SHP_SetSreReHorCtrl(void *pstParams)
{
	int ret = RET_OK;
	do {
		CHECK_KNULL(pstParams);
		if(_g_pe_shp_cb_fn.set_res_hor)
		{
			ret = _g_pe_shp_cb_fn.set_sre_res_hor(pstParams);
			PE_CHECK_CODE(ret, break, "set_sre_res_hor() error.\n");
		}
	}while (0);
	return ret;
}
/**
 * get sr resolution horizontal control
 * - use input struct LX_PE_SHP_SR_RE_HOR_T for H13 and H14A
 *
 * @param   *pstParams [in/out] void
 * @return  OK if success, ERROR otherwise.
 * @see		function description
 * @author
 */
static int PE_SHP_GetSreReHorCtrl(void *pstParams)
{
	int ret = RET_OK;
	do {
		CHECK_KNULL(pstParams);
		if(_g_pe_shp_cb_fn.get_res_hor)
		{
			ret = _g_pe_shp_cb_fn.get_sre_res_hor(pstParams);
			PE_CHECK_CODE(ret, break, "get_sre_res_hor() error.\n");
		}
	}while (0);
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
static int PE_SHP_SetSreReVerCtrl(void *pstParams)
{
	int ret = RET_OK;
	do {
		CHECK_KNULL(pstParams);
		if(_g_pe_shp_cb_fn.set_sre_res_ver)
		{
			ret = _g_pe_shp_cb_fn.set_sre_res_ver(pstParams);
			PE_CHECK_CODE(ret, break, "set_sre_res_ver() error.\n");
		}
	}while (0);
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
static int PE_SHP_GetSreReVerCtrl(void *pstParams)
{
	int ret = RET_OK;
	do {
		CHECK_KNULL(pstParams);
		if(_g_pe_shp_cb_fn.get_sre_res_ver)
		{
			ret = _g_pe_shp_cb_fn.get_sre_res_ver(pstParams);
			PE_CHECK_CODE(ret, break, "get_sre_res_ver() error.\n");
		}
	}while (0);
	return ret;
}
/**
 * set sr resolution misc control
 * - use input struct LX_PE_SHP_RE1_MISC_T for H15X
 *
 * @param   *pstParams [in] void
 * @return  OK if success, ERROR otherwise.
 * @see		function description
 * @author
 */
static int PE_SHP_SetSreReMiscCtrl(void *pstParams)
{
	int ret = RET_OK;
	do {
		CHECK_KNULL(pstParams);
		if(_g_pe_shp_cb_fn.set_sre_res_misc)
		{
			ret = _g_pe_shp_cb_fn.set_sre_res_misc(pstParams);
			PE_CHECK_CODE(ret, break, "set_sre_res_misc() error.\n");
		}
	}while (0);
	return ret;
}
/**
 * get sr resolution misc control
 * - use input struct LX_PE_SHP_RE1_MISC_T for H15X
 *
 * @param   *pstParams [in/out] void
 * @return  OK if success, ERROR otherwise.
 * @see		function description
 * @author
 */
static int PE_SHP_GetSreReMiscCtrl(void *pstParams)
{
	int ret = RET_OK;
	do {
		CHECK_KNULL(pstParams);
		if(_g_pe_shp_cb_fn.get_sre_res_misc)
		{
			ret = _g_pe_shp_cb_fn.get_sre_res_misc(pstParams);
			PE_CHECK_CODE(ret, break, "get_sre_res_misc() error.\n");
		}
	}while (0);
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
static int PE_SHP_SetSreEdgeEnhanceCtrl(void *pstParams)
{
	int ret = RET_OK;
	do {
		CHECK_KNULL(pstParams);
		if(_g_pe_shp_cb_fn.set_sre_edge_enh)
		{
			ret = _g_pe_shp_cb_fn.set_sre_edge_enh(pstParams);
			PE_CHECK_CODE(ret, break, "set_sre_edge_enh() error.\n");
		}
	}while (0);
	return ret;
}
/**
 * get sr edge enhance control
 * - use input struct LX_PE_SHP_SR_EE_T for H15X
 *
 * @param   *pstParams [in/out] void
 * @return  OK if success, ERROR otherwise.
 * @see		function description
 * @author
 */
static int PE_SHP_GetSreEdgeEnhanceCtrl(void *pstParams)
{
	int ret = RET_OK;
	do {
		CHECK_KNULL(pstParams);
		if(_g_pe_shp_cb_fn.get_sre_edge_enh)
		{
			ret = _g_pe_shp_cb_fn.get_sre_edge_enh(pstParams);
			PE_CHECK_CODE(ret, break, "get_sre_edge_enh() error.\n");
		}
	}while (0);
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
static int PE_SHP_SetSreDetailEnhanceCtrl(void *pstParams)
{
	int ret = RET_OK;
	do {
		CHECK_KNULL(pstParams);
		if(_g_pe_shp_cb_fn.set_sre_detail_enh)
		{
			ret = _g_pe_shp_cb_fn.set_sre_detail_enh(pstParams);
			PE_CHECK_CODE(ret, break, "set_sre_detail_enh() error.\n");
		}
	}while (0);
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
static int PE_SHP_GetSreDetailEnhanceCtrl(void *pstParams)
{
	int ret = RET_OK;
	do {
		CHECK_KNULL(pstParams);
		if(_g_pe_shp_cb_fn.get_sre_detail_enh)
		{
			ret = _g_pe_shp_cb_fn.get_sre_detail_enh(pstParams);
			PE_CHECK_CODE(ret, break, "get_sre_detail_enh() error.\n");
		}
	}while (0);
	return ret;
}

#endif
