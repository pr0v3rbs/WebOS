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

/** @file pe_nrd.c
 *
 *  driver for picture enhance noise reduction functions.
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
#include "pe_def.h"
#include "pe_spd.h"
#include "pe_etc.h"
#include "pe_cmn.h"
#include "pe_nrd.h"
#include "pe_etc_hw.h"
#include "pe_nrd_hw.h"

/*----------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------*/
#define PE_NRD_DBG_PRINT(fmt, args...)	\
	if (_g_nrd_trace)\
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
	int (*set_dnr_cmn)(void *pstParams);
	int (*get_dnr_cmn)(void *pstParams);
	int (*set_dnr_detail)(void *pstParams);
	int (*get_dnr_detail)(void *pstParams);
	int (*set_tnr_cmn)(void *pstParams);
	int (*get_tnr_cmn)(void *pstParams);
	int (*set_tnr_detail)(void *pstParams);
	int (*get_tnr_detail)(void *pstParams);
}
PE_NRD_CB_FUNC_T;

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
static int PE_NRD_SetDefaultSettings(LX_PE_DEFAULT_SETTINGS_T *pstParams);
static int PE_NRD_SetDebugSettings(LX_PE_DBG_SETTINGS_T *pstParams);
static int PE_NRD_SetDnrCmnCtrl(void *pstParams);
static int PE_NRD_GetDnrCmnCtrl(void *pstParams);
static int PE_NRD_SetDnrDetailCtrl(void *pstParams);
static int PE_NRD_GetDnrDetailCtrl(void *pstParams);
static int PE_NRD_SetTnrCmnCtrl(void *pstParams);
static int PE_NRD_GetTnrCmnCtrl(void *pstParams);
static int PE_NRD_SetTnrDetailCtrl(void *pstParams);
static int PE_NRD_GetTnrDetailCtrl(void *pstParams);

/*----------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------*/
static PE_NRD_CB_FUNC_T _g_pe_nrd_cb_fn;
static UINT32 _g_nrd_trace=0x0;	//default should be off.

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
int PE_NRD_RegisterCbFunc(void)
{
	int ret = RET_OK;

	memset(&_g_pe_nrd_cb_fn, 0, sizeof(PE_NRD_CB_FUNC_T));
	do{
		#ifdef USE_PE_KDRV_CODES_FOR_H15
		if (PE_KDRV_VER_H15)
		{
			_g_pe_nrd_cb_fn.hw_init				= PE_NRD_HW_H15_Init;
			_g_pe_nrd_cb_fn.set_default			= PE_NRD_HW_H15_SetDefault;
			_g_pe_nrd_cb_fn.set_debug			= PE_NRD_HW_H15_SetDebugSettings;
			_g_pe_nrd_cb_fn.set_dnr_cmn			= PE_NRD_HW_H15_SetDnrCmnCtrl;
			_g_pe_nrd_cb_fn.get_dnr_cmn			= PE_NRD_HW_H15_GetDnrCmnCtrl;
			_g_pe_nrd_cb_fn.set_dnr_detail		= PE_NRD_HW_H15_SetDnrDetailCtrl;
			_g_pe_nrd_cb_fn.get_dnr_detail		= PE_NRD_HW_H15_GetDnrDetailCtrl;
			_g_pe_nrd_cb_fn.set_tnr_cmn			= PE_NRD_HW_H15_SetTnrCmnCtrl;
			_g_pe_nrd_cb_fn.get_tnr_cmn			= PE_NRD_HW_H15_GetTnrCmnCtrl;
			_g_pe_nrd_cb_fn.set_tnr_detail		= PE_NRD_HW_H15_SetTnrDetailCtrl;
			_g_pe_nrd_cb_fn.get_tnr_detail		= PE_NRD_HW_H15_GetTnrDetailCtrl;
		}
		else
		#endif
		#ifdef USE_PE_KDRV_CODES_FOR_M14
		if (PE_KDRV_VER_M14)
		{
			_g_pe_nrd_cb_fn.hw_init				= PE_NRD_HW_M14_Init;
			_g_pe_nrd_cb_fn.set_default			= PE_NRD_HW_M14_SetDefault;
			_g_pe_nrd_cb_fn.set_debug			= PE_NRD_HW_M14_SetDebugSettings;
			_g_pe_nrd_cb_fn.set_dnr_cmn			= PE_NRD_HW_M14_SetDnrCmnCtrl;
			_g_pe_nrd_cb_fn.get_dnr_cmn			= PE_NRD_HW_M14_GetDnrCmnCtrl;
			_g_pe_nrd_cb_fn.set_dnr_detail		= PE_NRD_HW_M14_SetDnrDetailCtrl;
			_g_pe_nrd_cb_fn.get_dnr_detail		= PE_NRD_HW_M14_GetDnrDetailCtrl;
			_g_pe_nrd_cb_fn.set_tnr_cmn			= PE_NRD_HW_M14_SetTnrCmnCtrl;
			_g_pe_nrd_cb_fn.get_tnr_cmn			= PE_NRD_HW_M14_GetTnrCmnCtrl;
			_g_pe_nrd_cb_fn.set_tnr_detail		= PE_NRD_HW_M14_SetTnrDetailCtrl;
			_g_pe_nrd_cb_fn.get_tnr_detail		= PE_NRD_HW_M14_GetTnrDetailCtrl;
		}
		else
		#endif
		#ifdef USE_PE_KDRV_CODES_FOR_H14
		if (PE_KDRV_VER_H14)
		{
			_g_pe_nrd_cb_fn.hw_init				= PE_NRD_HW_H14_Init;
			_g_pe_nrd_cb_fn.set_default			= PE_NRD_HW_H14_SetDefault;
			_g_pe_nrd_cb_fn.set_debug			= PE_NRD_HW_H14_SetDebugSettings;
			_g_pe_nrd_cb_fn.set_dnr_cmn			= PE_NRD_HW_H14_SetDnrCmnCtrl;
			_g_pe_nrd_cb_fn.get_dnr_cmn			= PE_NRD_HW_H14_GetDnrCmnCtrl;
			_g_pe_nrd_cb_fn.set_dnr_detail		= PE_NRD_HW_H14_SetDnrDetailCtrl;
			_g_pe_nrd_cb_fn.get_dnr_detail		= PE_NRD_HW_H14_GetDnrDetailCtrl;
			_g_pe_nrd_cb_fn.set_tnr_cmn			= PE_NRD_HW_H14_SetTnrCmnCtrl;
			_g_pe_nrd_cb_fn.get_tnr_cmn			= PE_NRD_HW_H14_GetTnrCmnCtrl;
			_g_pe_nrd_cb_fn.set_tnr_detail		= PE_NRD_HW_H14_SetTnrDetailCtrl;
			_g_pe_nrd_cb_fn.get_tnr_detail		= PE_NRD_HW_H14_GetTnrDetailCtrl;
		}
		else
		#endif
		#ifdef USE_PE_KDRV_CODES_FOR_H13
		if (PE_KDRV_VER_H13)
		{
			_g_pe_nrd_cb_fn.hw_init				= PE_NRD_HW_H13_Init;
			_g_pe_nrd_cb_fn.set_default			= PE_NRD_HW_H13_SetDefault;
			_g_pe_nrd_cb_fn.set_debug			= PE_NRD_HW_H13_SetDebugSettings;
			_g_pe_nrd_cb_fn.set_dnr_cmn			= PE_NRD_HW_H13_SetDnrCmnCtrl;
			_g_pe_nrd_cb_fn.get_dnr_cmn			= PE_NRD_HW_H13_GetDnrCmnCtrl;
			_g_pe_nrd_cb_fn.set_dnr_detail		= PE_NRD_HW_H13_SetDnrDetailCtrl;
			_g_pe_nrd_cb_fn.get_dnr_detail		= PE_NRD_HW_H13_GetDnrDetailCtrl;
			_g_pe_nrd_cb_fn.set_tnr_cmn			= PE_NRD_HW_H13_SetTnrCmnCtrl;
			_g_pe_nrd_cb_fn.get_tnr_cmn			= PE_NRD_HW_H13_GetTnrCmnCtrl;
			_g_pe_nrd_cb_fn.set_tnr_detail		= PE_NRD_HW_H13_SetTnrDetailCtrl;
			_g_pe_nrd_cb_fn.get_tnr_detail		= PE_NRD_HW_H13_GetTnrDetailCtrl;
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
 * init nrd init
 * 	- h14, h13, l9 tnr alpha lut loading sequence
 *		: load time on -> tnr off -> loading alpha lut -> tnr on -> load time off
 *
 * @param   *pstParams [in] PE_CFG_CTRL_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_NRD_Init(PE_CFG_CTRL_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(_g_pe_nrd_cb_fn.hw_init)
		{
			ret = _g_pe_nrd_cb_fn.hw_init(pstParams);
			PE_CHECK_CODE(ret, break, "hw_init() error.\n");
		}
	}while (0);
	return ret;
}

/**
 * standard function of nrd setting
 *
 * @param   data [in/out] unsigned int
 * @param   functype [in/out] unsigned int
 * @param   datasize [in/out] unsigned int
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_NRD_SetFunction(unsigned int *data, unsigned int functype, unsigned int datasize)
{
	int ret = RET_ERROR;

#define _VOID___INPUT_		1
#define _STRUCT_INPUT_		0

#define PE_NRD_SET_FUNC(_struct, _func, _void_input)	\
	case PE_ITEM_PKTFUNC(_struct):\
	{\
		_struct *p_rcv, param;\
		do{\
			PE_NRD_DBG_PRINT("S %s\n", #_struct);\
			PE_CHECK_CODE(datasize!=sizeof(_struct), break, \
				"[%s,%d] size mismatch.\n", __F__, __L__);\
			p_rcv = (_struct *)data;\
			memcpy(&param, p_rcv, datasize);\
			ret = (_void_input)? (_func((void *)&param)):(_func(&param));\
			PE_CHECK_CODE(ret, break, \
				"[%s,%d] %s() error.\n", __F__, __L__, #_func);\
			PE_NRD_DBG_PRINT("E %s\n", #_struct);\
		}while (0);\
	}\
	break

	/* data null check */
	PE_CHECK_CODE(!data, return RET_ERROR, \
		"[%s,%d] data is null.\n", __F__, __L__);
	switch (functype)
	{
		PE_NRD_SET_FUNC(LX_PE_NRD_DNR0_CMN_T,		PE_NRD_SetDnrCmnCtrl,		_VOID___INPUT_);
		PE_NRD_SET_FUNC(LX_PE_NRD_TNR1_CMN_T,		PE_NRD_SetTnrCmnCtrl,		_VOID___INPUT_);
		PE_NRD_SET_FUNC(LX_PE_NRD_DNR2_CMN_T,		PE_NRD_SetDnrCmnCtrl,		_VOID___INPUT_);
		PE_NRD_SET_FUNC(LX_PE_NRD_DNR3_CMN_T,		PE_NRD_SetDnrCmnCtrl,		_VOID___INPUT_);
		PE_NRD_SET_FUNC(LX_PE_NRD_DNR4_CMN_T,		PE_NRD_SetDnrCmnCtrl,		_VOID___INPUT_);
		PE_NRD_SET_FUNC(LX_PE_NRD_DNR6_CMN_T,		PE_NRD_SetDnrCmnCtrl,		_VOID___INPUT_);
		PE_NRD_SET_FUNC(LX_PE_NRD_DNR2_DETAIL_T,	PE_NRD_SetDnrDetailCtrl,	_VOID___INPUT_);
		PE_NRD_SET_FUNC(LX_PE_NRD_DNR4_DETAIL_T,	PE_NRD_SetDnrDetailCtrl,	_VOID___INPUT_);
		PE_NRD_SET_FUNC(LX_PE_NRD_DNR5_DETAIL_T,	PE_NRD_SetDnrDetailCtrl,	_VOID___INPUT_);
		PE_NRD_SET_FUNC(LX_PE_NRD_DNR6_DETAIL_T,	PE_NRD_SetDnrDetailCtrl,	_VOID___INPUT_);
		PE_NRD_SET_FUNC(LX_PE_NRD_TNR2_CMN_T,		PE_NRD_SetTnrCmnCtrl,		_VOID___INPUT_);
		PE_NRD_SET_FUNC(LX_PE_NRD_TNR3_CMN_T,		PE_NRD_SetTnrCmnCtrl,		_VOID___INPUT_);
		PE_NRD_SET_FUNC(LX_PE_NRD_TNR4_CMN_T,		PE_NRD_SetTnrCmnCtrl,		_VOID___INPUT_);
		PE_NRD_SET_FUNC(LX_PE_NRD_TNR5_CMN_T,		PE_NRD_SetTnrCmnCtrl,		_VOID___INPUT_);
		PE_NRD_SET_FUNC(LX_PE_NRD_TNR2_DETAIL_T,	PE_NRD_SetTnrDetailCtrl,	_VOID___INPUT_);
		PE_NRD_SET_FUNC(LX_PE_NRD_TNR4_DETAIL_T,	PE_NRD_SetTnrDetailCtrl,	_VOID___INPUT_);
		PE_NRD_SET_FUNC(LX_PE_NRD_TNR5_DETAIL_T,	PE_NRD_SetTnrDetailCtrl,	_VOID___INPUT_);
		PE_NRD_SET_FUNC(LX_PE_DEFAULT_SETTINGS_T,	PE_NRD_SetDefaultSettings,	_STRUCT_INPUT_);
		PE_NRD_SET_FUNC(LX_PE_DBG_SETTINGS_T,		PE_NRD_SetDebugSettings,	_STRUCT_INPUT_);
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
 * standard function of nrd getting
 *
 * @param   data [in/out] unsigned int
 * @param   functype [in/out] unsigned int
 * @param   datasize [in/out] unsigned int
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_NRD_GetFunction(unsigned int *data, unsigned int functype, unsigned int datasize)
{
	int ret = RET_ERROR;

#define _VOID___INPUT_		1
#define _STRUCT_INPUT_		0

#define PE_NRD_GET_FUNC(_struct, _func, _void_input)	\
	case PE_ITEM_PKTFUNC(_struct):\
	{\
		_struct *p_rcv, param;\
		do{\
			PE_NRD_DBG_PRINT("S %s\n", #_struct);\
			PE_CHECK_CODE(datasize!=sizeof(_struct), break, \
				"[%s,%d] size mismatch.\n", __F__, __L__);\
			p_rcv = (_struct *)data;\
			memcpy(&param, p_rcv, datasize);\
			ret = (_void_input)? (_func((void *)&param)):(_func(&param));\
			PE_CHECK_CODE(ret, break, \
				"[%s,%d] %s() error.\n", __F__, __L__, #_func);\
			memcpy(p_rcv, &param, datasize);\
			PE_NRD_DBG_PRINT("E %s\n", #_struct);\
		}while (0);\
	}\
	break

	/* data null check */
	PE_CHECK_CODE(!data, return RET_ERROR, \
		"[%s,%d] data is null.\n", __F__, __L__);
	switch (functype)
	{
		PE_NRD_GET_FUNC(LX_PE_NRD_DNR0_CMN_T,	PE_NRD_GetDnrCmnCtrl,		_VOID___INPUT_);
		PE_NRD_GET_FUNC(LX_PE_NRD_TNR1_CMN_T,	PE_NRD_GetTnrCmnCtrl,		_VOID___INPUT_);
		PE_NRD_GET_FUNC(LX_PE_NRD_DNR2_CMN_T,	PE_NRD_GetDnrCmnCtrl,		_VOID___INPUT_);
		PE_NRD_GET_FUNC(LX_PE_NRD_DNR3_CMN_T,	PE_NRD_GetDnrCmnCtrl,		_VOID___INPUT_);
		PE_NRD_GET_FUNC(LX_PE_NRD_DNR4_CMN_T,	PE_NRD_GetDnrCmnCtrl,		_VOID___INPUT_);
		PE_NRD_GET_FUNC(LX_PE_NRD_DNR6_CMN_T,	PE_NRD_GetDnrCmnCtrl,		_VOID___INPUT_);
		PE_NRD_GET_FUNC(LX_PE_NRD_DNR2_DETAIL_T,PE_NRD_GetDnrDetailCtrl,	_VOID___INPUT_);
		PE_NRD_GET_FUNC(LX_PE_NRD_DNR4_DETAIL_T,PE_NRD_GetDnrDetailCtrl,	_VOID___INPUT_);
		PE_NRD_GET_FUNC(LX_PE_NRD_DNR5_DETAIL_T,PE_NRD_GetDnrDetailCtrl,	_VOID___INPUT_);
		PE_NRD_GET_FUNC(LX_PE_NRD_DNR6_DETAIL_T,PE_NRD_GetDnrDetailCtrl,	_VOID___INPUT_);
		PE_NRD_GET_FUNC(LX_PE_NRD_TNR2_CMN_T,	PE_NRD_GetTnrCmnCtrl,		_VOID___INPUT_);
		PE_NRD_GET_FUNC(LX_PE_NRD_TNR3_CMN_T,	PE_NRD_GetTnrCmnCtrl,		_VOID___INPUT_);
		PE_NRD_GET_FUNC(LX_PE_NRD_TNR4_CMN_T,	PE_NRD_GetTnrCmnCtrl,		_VOID___INPUT_);
		PE_NRD_GET_FUNC(LX_PE_NRD_TNR5_CMN_T,	PE_NRD_GetTnrCmnCtrl,		_VOID___INPUT_);
		PE_NRD_GET_FUNC(LX_PE_NRD_TNR2_DETAIL_T,PE_NRD_GetTnrDetailCtrl,	_VOID___INPUT_);
		PE_NRD_GET_FUNC(LX_PE_NRD_TNR4_DETAIL_T,PE_NRD_GetTnrDetailCtrl,	_VOID___INPUT_);
		PE_NRD_GET_FUNC(LX_PE_NRD_TNR5_DETAIL_T,PE_NRD_GetTnrDetailCtrl,	_VOID___INPUT_);
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
static int PE_NRD_SetDefaultSettings(LX_PE_DEFAULT_SETTINGS_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(_g_pe_nrd_cb_fn.set_default)
		{
			ret = _g_pe_nrd_cb_fn.set_default(pstParams);
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
static int PE_NRD_SetDebugSettings(LX_PE_DBG_SETTINGS_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		/* set print level */
		if (pstParams->type&LX_PE_DBG_LV)
		{
			_g_nrd_trace = (pstParams->print_lvl & \
				(0x1<<PE_ITEM_PKTMODL(NRD)))? 0x1:0x0;
		}
		if(_g_pe_nrd_cb_fn.set_debug)
		{
			ret = _g_pe_nrd_cb_fn.set_debug(pstParams);
			PE_CHECK_CODE(ret, break, "set_debug() error.\n");
		}
	}while (0);
	return ret;
}
/**
 * set dnr common ctrl
 * - use input struct LX_PE_NRD_DNR3_CMN_T for H14AX
 * - use input struct LX_PE_NRD_DNR2_CMN_T for H13BX and M14A
 * - use input struct LX_PE_NRD_DNR0_CMN_T for H13AX
 *
 * @param   *pstParams [in] void
 * @return  OK if success, ERROR otherwise.
 * @see		set discription
 * @author
 */
static int PE_NRD_SetDnrCmnCtrl(void *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(_g_pe_nrd_cb_fn.set_dnr_cmn)
		{
			ret = _g_pe_nrd_cb_fn.set_dnr_cmn(pstParams);
			PE_CHECK_CODE(ret, break, "set_dnr_cmn() error.\n");
		}
	}while (0);
	return ret;
}
/**
 * get dnr common ctrl
 * - use input struct LX_PE_NRD_DNR3_CMN_T for H14AX
 * - use input struct LX_PE_NRD_DNR2_CMN_T for H13BX and M14A
 * - use input struct LX_PE_NRD_DNR0_CMN_T for H13AX
 *
 * @param   *pstParams [in/out] void
 * @return  OK if success, ERROR otherwise.
 * @see		set discription
 * @author
 */
static int PE_NRD_GetDnrCmnCtrl(void *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(_g_pe_nrd_cb_fn.get_dnr_cmn)
		{
			ret = _g_pe_nrd_cb_fn.get_dnr_cmn(pstParams);
			PE_CHECK_CODE(ret, break, "get_dnr_cmn() error.\n");
		}
	}while (0);
	return ret;
}
/**
 * set dnr detail ctrl
 * - use input struct LX_PE_NRD_DNR2_DETAIL_T for H14BX and H14A
 *
 * @param   *pstParams [in] void
 * @return  OK if success, ERROR otherwise.
 * @see		set discription
 * @author
 */
static int PE_NRD_SetDnrDetailCtrl(void *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(_g_pe_nrd_cb_fn.set_dnr_detail)
		{
			ret = _g_pe_nrd_cb_fn.set_dnr_detail(pstParams);
			PE_CHECK_CODE(ret, break, "set_dnr_detail() error.\n");
		}
	}while (0);
	return ret;
}
/**
 * get dnr detail ctrl
 * - use input struct LX_PE_NRD_DNR2_DETAIL_T for H13BX and H14A
 *
 * @param   *pstParams [in/out] void
 * @return  OK if success, ERROR otherwise.
 * @see		set discription
 * @author
 */
static int PE_NRD_GetDnrDetailCtrl(void *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(_g_pe_nrd_cb_fn.get_dnr_detail)
		{
			ret = _g_pe_nrd_cb_fn.get_dnr_detail(pstParams);
			PE_CHECK_CODE(ret, break, "get_dnr_detail() error.\n");
		}
	}while (0);
	return ret;
}
/**
 * set tnr common ctrl
 * - use input struct LX_PE_NRD_TNR3_CMN_T for H14A
 * - use input struct LX_PE_NRD_TNR2_CMN_T for H13BX
 * - use input struct LX_PE_NRD_TNR1_CMN_T for H13AX
 *
 * @param   *pstParams [in] void
 * @return  OK if success, ERROR otherwise.
 * @see		set discription
 * @author
 */
static int PE_NRD_SetTnrCmnCtrl(void *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(_g_pe_nrd_cb_fn.set_tnr_cmn)
		{
			ret = _g_pe_nrd_cb_fn.set_tnr_cmn(pstParams);
			PE_CHECK_CODE(ret, break, "set_tnr_cmn() error.\n");
		}
	}while (0);
	return ret;
}
/**
 * get tnr common ctrl
 * - use input struct LX_PE_NRD_TNR3_CMN_T for H14A
 * - use input struct LX_PE_NRD_TNR2_CMN_T for H13BX
 * - use input struct LX_PE_NRD_TNR1_CMN_T for H13AX
 *
 * @param   *pstParams [in/out] void
 * @return  OK if success, ERROR otherwise.
 * @see		set discription
 * @author
 */
static int PE_NRD_GetTnrCmnCtrl(void *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(_g_pe_nrd_cb_fn.get_tnr_cmn)
		{
			ret = _g_pe_nrd_cb_fn.get_tnr_cmn(pstParams);
			PE_CHECK_CODE(ret, break, "get_tnr_cmn() error.\n");
		}
	}while (0);
	return ret;
}
/**
 * set tnr detail ctrl
 * - use input struct LX_PE_NRD_TNR2_DETAIL_T for H13BX and H14A
 *
 * @param   *pstParams [in] void
 * @return  OK if success, ERROR otherwise.
 * @see		set discription
 * @author
 */
static int PE_NRD_SetTnrDetailCtrl(void *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(_g_pe_nrd_cb_fn.set_tnr_detail)
		{
			ret = _g_pe_nrd_cb_fn.set_tnr_detail(pstParams);
			PE_CHECK_CODE(ret, break, "set_tnr_detail() error.\n");
		}
	}while (0);
	return ret;
}
/**
 * get tnr detail ctrl
 * - use input struct LX_PE_NRD_TNR2_DETAIL_T for H13BX and H14A
 *
 * @param   *pstParams [in/out] void
 * @return  OK if success, ERROR otherwise.
 * @see		set discription
 * @author
 */
static int PE_NRD_GetTnrDetailCtrl(void *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(_g_pe_nrd_cb_fn.get_tnr_detail)
		{
			ret = _g_pe_nrd_cb_fn.get_tnr_detail(pstParams);
			PE_CHECK_CODE(ret, break, "get_tnr_detail() error.\n");
		}
	}while (0);
	return ret;
}

