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

/** @file pe_hst.c
 *
 *  driver for picture enhance histogram module functions. ( used only within kdriver )
 *	- histogram information and configuration.
 *	
 *	@author		Seung-Jun, Youm(sj.youm@lge.com)
 *	@version	0.1
 *	@note		
 *	@date		2013.04.02
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
#include "pe_hst.h"
#include "pe_etc_hw.h"
#include "pe_hst_hw.h"

/*----------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------*/
#define PE_HST_DBG_PRINT(fmt, args...)	\
	if (_g_hst_trace)\
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
	int (*get_histo_info)(LX_PE_HST_HISTO_INFO_T *pstParams);
	int (*set_histo_config)(LX_PE_HST_HISTO_CFG_T *pstParams);
	int (*get_histo_config)(LX_PE_HST_HISTO_CFG_T *pstParams);
}
PE_HST_CB_FUNC_T;

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
static int PE_HST_SetDefaultSettings(LX_PE_DEFAULT_SETTINGS_T *pstParams);
static int PE_HST_SetDebugSettings(LX_PE_DBG_SETTINGS_T *pstParams);
static int PE_HST_GetHistogramInfo(LX_PE_HST_HISTO_INFO_T *pstParams);
static int PE_HST_SetHistoConfig(LX_PE_HST_HISTO_CFG_T *pstParams);
static int PE_HST_GetHistoConfig(LX_PE_HST_HISTO_CFG_T *pstParams);

/*----------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------*/
static PE_HST_CB_FUNC_T _g_pe_hst_cb_fn;
static UINT32 _g_hst_trace=0x0;	//default should be off.

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
int PE_HST_RegisterCbFunc(void)
{
	int ret = RET_OK;

	memset(&_g_pe_hst_cb_fn, 0, sizeof(PE_HST_CB_FUNC_T));
	do{
		#ifdef USE_PE_KDRV_CODES_FOR_H15
		if (PE_KDRV_VER_H15)
		{
			_g_pe_hst_cb_fn.hw_init				= PE_HST_HW_H15_Init;
			_g_pe_hst_cb_fn.set_default			= NULL;
			_g_pe_hst_cb_fn.set_debug			= PE_HST_HW_H15_SetDebugSettings;
			_g_pe_hst_cb_fn.get_histo_info		= PE_HST_HW_H15_GetHistogramInfo;
			_g_pe_hst_cb_fn.set_histo_config	= PE_HST_HW_H15_SetHistoConfig;
			_g_pe_hst_cb_fn.get_histo_config	= PE_HST_HW_H15_GetHistoConfig;
		}
		else
		#endif
		#ifdef USE_PE_KDRV_CODES_FOR_M14
		if (PE_KDRV_VER_M14)
		{
			_g_pe_hst_cb_fn.hw_init				= PE_HST_HW_M14_Init;
			_g_pe_hst_cb_fn.set_default			= NULL;
			_g_pe_hst_cb_fn.set_debug			= PE_HST_HW_M14_SetDebugSettings;
			_g_pe_hst_cb_fn.get_histo_info		= PE_HST_HW_M14_GetHistogramInfo;
			_g_pe_hst_cb_fn.set_histo_config	= PE_HST_HW_M14_SetHistoConfig;
			_g_pe_hst_cb_fn.get_histo_config	= PE_HST_HW_M14_GetHistoConfig;
		}
		else
		#endif
		#ifdef USE_PE_KDRV_CODES_FOR_H14
		if (PE_KDRV_VER_H14)
		{
			_g_pe_hst_cb_fn.hw_init				= PE_HST_HW_H14_Init;
			_g_pe_hst_cb_fn.set_default			= NULL;
			_g_pe_hst_cb_fn.set_debug			= PE_HST_HW_H14_SetDebugSettings;
			_g_pe_hst_cb_fn.get_histo_info		= PE_HST_HW_H14_GetHistogramInfo;
			_g_pe_hst_cb_fn.set_histo_config	= PE_HST_HW_H14_SetHistoConfig;
			_g_pe_hst_cb_fn.get_histo_config	= PE_HST_HW_H14_GetHistoConfig;
		}
		else
		#endif
		#ifdef USE_PE_KDRV_CODES_FOR_H13
		if (PE_KDRV_VER_H13)
		{
			_g_pe_hst_cb_fn.hw_init				= PE_HST_HW_H13_Init;
			_g_pe_hst_cb_fn.set_default			= NULL;
			_g_pe_hst_cb_fn.set_debug			= PE_HST_HW_H13_SetDebugSettings;
			_g_pe_hst_cb_fn.get_histo_info		= PE_HST_HW_H13_GetHistogramInfo;
			_g_pe_hst_cb_fn.set_histo_config	= PE_HST_HW_H13_SetHistoConfig;
			_g_pe_hst_cb_fn.get_histo_config	= PE_HST_HW_H13_GetHistoConfig;
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
 * init dynamic contrast module
 *
 * @param   *pstParams [in] PE_CFG_CTRL_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_HST_Init(PE_CFG_CTRL_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(_g_pe_hst_cb_fn.hw_init)
		{
			ret = _g_pe_hst_cb_fn.hw_init(pstParams);
			PE_CHECK_CODE(ret, break, "hw_init() error.\n");
		}
	}while (0);
	return ret;
}

/**
 * standard function of hst setting
 *
 * @param   data [in/out] unsigned int
 * @param   functype [in] unsigned int
 * @param   datasize [in] unsigned int
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_HST_SetFunction(unsigned int *data, unsigned int functype, unsigned int datasize)
{
	int ret = RET_ERROR;

#define PE_HST_SET_FUNC(_struct, _func)	\
	case PE_ITEM_PKTFUNC(_struct):\
	{\
		_struct *p_rcv, param;\
		do{\
			PE_HST_DBG_PRINT("S %s\n", #_struct);\
			PE_CHECK_CODE(datasize!=sizeof(_struct), break, \
				"[%s,%d] size mismatch.\n", __F__, __L__);\
			p_rcv = (_struct *)data;\
			memcpy(&param, p_rcv, datasize);\
			ret = _func(&param);\
			PE_CHECK_CODE(ret, break, \
				"[%s,%d] %s() error.\n", __F__, __L__, #_func);\
			PE_HST_DBG_PRINT("E %s\n", #_struct);\
		}while (0);\
	}\
	break

	/* data null check */
	PE_CHECK_CODE(!data, return RET_ERROR, \
		"[%s,%d] data is null.\n", __F__, __L__);
	switch (functype)
	{
		PE_HST_SET_FUNC(LX_PE_HST_HISTO_CFG_T,		PE_HST_SetHistoConfig);
		PE_HST_SET_FUNC(LX_PE_DEFAULT_SETTINGS_T,	PE_HST_SetDefaultSettings);
		PE_HST_SET_FUNC(LX_PE_DBG_SETTINGS_T,		PE_HST_SetDebugSettings);
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
 * standard function of hst getting
 *
 * @param   data [in/out] unsigned int
 * @param   functype [in] unsigned int
 * @param   datasize [in] unsigned int
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_HST_GetFunction(unsigned int *data, unsigned int functype, unsigned int datasize)
{
	int ret = RET_ERROR;

#define PE_HST_GET_FUNC(_struct, _func)	\
	case PE_ITEM_PKTFUNC(_struct):\
	{\
		_struct *p_rcv, param;\
		do{\
			PE_HST_DBG_PRINT("S %s\n", #_struct);\
			PE_CHECK_CODE(datasize!=sizeof(_struct), break, \
				"[%s,%d] size mismatch.\n", __F__, __L__);\
			p_rcv = (_struct *)data;\
			memcpy(&param, p_rcv, datasize);\
			ret = _func(&param);\
			PE_CHECK_CODE(ret, break, \
				"[%s,%d] %s() error.\n", __F__, __L__, #_func);\
			memcpy(p_rcv, &param, datasize);\
			PE_HST_DBG_PRINT("E %s\n", #_struct);\
		}while (0);\
	}\
	break

/** get func without start,
*   end print(WOPT:WithOut PRint) because of too many prints */
#define PE_HST_GET_WOPR(_struct, _func)	\
	case PE_ITEM_PKTFUNC(_struct):\
	{\
		_struct *p_rcv, param;\
		do{\
			PE_CHECK_CODE(datasize!=sizeof(_struct), break, \
				"[%s,%d] size mismatch.\n", __F__, __L__);\
			p_rcv = (_struct *)data;\
			memcpy(&param, p_rcv, datasize);\
			ret = _func(&param);\
			PE_CHECK_CODE(ret, break, \
				"[%s,%d] %s() error.\n", __F__, __L__, #_func);\
			memcpy(p_rcv, &param, datasize);\
		}while (0);\
	}\
	break

	/* data null check */
	PE_CHECK_CODE(!data, return RET_ERROR, \
		"[%s,%d] data is null.\n", __F__, __L__);
	switch (functype)
	{
		PE_HST_GET_WOPR(LX_PE_HST_HISTO_INFO_T,		PE_HST_GetHistogramInfo);
		PE_HST_GET_FUNC(LX_PE_HST_HISTO_CFG_T,		PE_HST_GetHistoConfig);
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
 * default setting
 *
 * @param   *pstParams [in] LX_PE_DEFAULT_SETTINGS_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_HST_SetDefaultSettings(LX_PE_DEFAULT_SETTINGS_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		PE_HST_DBG_PRINT("[win id:%d]\n", pstParams->win_id);
	}while (0);
	return ret;
}
/**
 * debug setting
 *
 * @param   *pstParams [in] LX_PE_DBG_SETTINGS_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_HST_SetDebugSettings(LX_PE_DBG_SETTINGS_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		/* set print level */
		if (pstParams->type&LX_PE_DBG_LV)
		{
			_g_hst_trace = (pstParams->print_lvl & \
				(0x1<<PE_ITEM_PKTMODL(HST)))? 0x1:0x0;
		}
		if(_g_pe_hst_cb_fn.set_debug)
		{
			ret = _g_pe_hst_cb_fn.set_debug(pstParams);
			PE_CHECK_CODE(ret, break, "set_debug() error.\n");
		}
	}while (0);
	return ret;
}
/**
 * get dce histogram info
 *
 * @param   *pstParams [in/out] LX_PE_HST_HISTO_INFO_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_HST_GetHistogramInfo(LX_PE_HST_HISTO_INFO_T *pstParams)
{
	int ret = RET_OK;
	do {
		CHECK_KNULL(pstParams);
		if(_g_pe_hst_cb_fn.get_histo_info)
		{
			ret = _g_pe_hst_cb_fn.get_histo_info(pstParams);
			PE_CHECK_CODE(ret, break, "get_histo_info() error.\n");
		}
	}while (0);
	return ret;
}
/**
 * set histogram config
 *
 * @param   *pstParams [in] LX_PE_HST_HISTO_CFG_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_HST_SetHistoConfig(LX_PE_HST_HISTO_CFG_T *pstParams)
{
	int ret = RET_OK;
	do {
		CHECK_KNULL(pstParams);
		if(_g_pe_hst_cb_fn.set_histo_config)
		{
			ret = _g_pe_hst_cb_fn.set_histo_config(pstParams);
			PE_CHECK_CODE(ret, break, "set_histo_config() error.\n");
		}
	} while (0);
	return ret;
}
/**
 * get histogram config
 *
 * @param   *pstParams [in/out] LX_PE_HST_HISTO_CFG_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_HST_GetHistoConfig(LX_PE_HST_HISTO_CFG_T *pstParams)
{
	int ret = RET_OK;
	do {
		CHECK_KNULL(pstParams);
		if(_g_pe_hst_cb_fn.get_histo_config)
		{
			ret = _g_pe_hst_cb_fn.get_histo_config(pstParams);
			PE_CHECK_CODE(ret, break, "get_histo_config() error.\n");
		}
	} while (0);
	return ret;
}

