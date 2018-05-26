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

/** @file pe_dnt.c
 *
 *  driver for picture enhance deinterlacer functions.
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
#include "pe_cmn.h"
#include "pe_dnt.h"
#include "pe_etc_hw.h"
#include "pe_dnt_hw.h"

/*----------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------*/
#define PE_DNT_DBG_PRINT(fmt, args...)	\
	if (_g_dnt_trace)\
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
	int (*set_film_mode)(LX_PE_DNT_FILMMODE_T *pstParams);
	int (*get_film_mode)(LX_PE_DNT_FILMMODE_T *pstParams);
	int (*set_low_delay)(LX_PE_DNT_LD_MODE_T *pstParams);
	int (*get_low_delay)(LX_PE_DNT_LD_MODE_T *pstParams);
}
PE_DNT_CB_FUNC_T;

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
static int PE_DNT_SetDefaultSettings(LX_PE_DEFAULT_SETTINGS_T *pstParams);
static int PE_DNT_SetDebugSettings(LX_PE_DBG_SETTINGS_T *pstParams);
static int PE_DNT_SetFilmMode(LX_PE_DNT_FILMMODE_T *pstParams);
static int PE_DNT_GetFilmMode(LX_PE_DNT_FILMMODE_T *pstParams);
static int PE_DNT_SetLowDelayMode(LX_PE_DNT_LD_MODE_T *pstParams);
static int PE_DNT_GetLowDelayMode(LX_PE_DNT_LD_MODE_T *pstParams);

/*----------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------*/
static PE_DNT_CB_FUNC_T _g_pe_dnt_cb_fn;
static UINT32 _g_dnt_trace=0x0;		//default should be off.

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
int PE_DNT_RegisterCbFunc(void)
{
	int ret = RET_OK;

	memset(&_g_pe_dnt_cb_fn, 0, sizeof(PE_DNT_CB_FUNC_T));
	do{
		#ifdef USE_PE_KDRV_CODES_FOR_H15
		if (PE_KDRV_VER_H15)
		{
			_g_pe_dnt_cb_fn.hw_init				= PE_DNT_HW_H15_Init;
			_g_pe_dnt_cb_fn.set_default			= PE_DNT_HW_H15_SetDefault;
			_g_pe_dnt_cb_fn.set_debug			= PE_DNT_HW_H15_SetDebugSettings;
			_g_pe_dnt_cb_fn.set_film_mode		= PE_DNT_HW_H15_SetFilmMode;
			_g_pe_dnt_cb_fn.get_film_mode		= PE_DNT_HW_H15_GetFilmMode;
			_g_pe_dnt_cb_fn.set_low_delay		= PE_DNT_HW_H15_SetLowDelayMode;
			_g_pe_dnt_cb_fn.get_low_delay		= PE_DNT_HW_H15_GetLowDelayMode;
		}
		else
		#endif
		#ifdef USE_PE_KDRV_CODES_FOR_M14
		if (PE_KDRV_VER_M14)
		{
			_g_pe_dnt_cb_fn.hw_init				= PE_DNT_HW_M14_Init;
			_g_pe_dnt_cb_fn.set_default			= PE_DNT_HW_M14_SetDefault;
			_g_pe_dnt_cb_fn.set_debug			= PE_DNT_HW_M14_SetDebugSettings;
			_g_pe_dnt_cb_fn.set_film_mode		= PE_DNT_HW_M14_SetFilmMode;
			_g_pe_dnt_cb_fn.get_film_mode		= PE_DNT_HW_M14_GetFilmMode;
			_g_pe_dnt_cb_fn.set_low_delay		= PE_DNT_HW_M14_SetLowDelayMode;
			_g_pe_dnt_cb_fn.get_low_delay		= PE_DNT_HW_M14_GetLowDelayMode;
		}
		else
		#endif
		#ifdef USE_PE_KDRV_CODES_FOR_H14
		if (PE_KDRV_VER_H14)
		{
			_g_pe_dnt_cb_fn.hw_init				= PE_DNT_HW_H14_Init;
			_g_pe_dnt_cb_fn.set_default			= PE_DNT_HW_H14_SetDefault;
			_g_pe_dnt_cb_fn.set_debug			= PE_DNT_HW_H14_SetDebugSettings;
			_g_pe_dnt_cb_fn.set_film_mode		= PE_DNT_HW_H14_SetFilmMode;
			_g_pe_dnt_cb_fn.get_film_mode		= PE_DNT_HW_H14_GetFilmMode;
			_g_pe_dnt_cb_fn.set_low_delay		= PE_DNT_HW_H14_SetLowDelayMode;
			_g_pe_dnt_cb_fn.get_low_delay		= PE_DNT_HW_H14_GetLowDelayMode;
		}
		else
		#endif
		#ifdef USE_PE_KDRV_CODES_FOR_H13
		if (PE_KDRV_VER_H13)
		{
			_g_pe_dnt_cb_fn.hw_init				= PE_DNT_HW_H13_Init;
			_g_pe_dnt_cb_fn.set_default			= PE_DNT_HW_H13_SetDefault;
			_g_pe_dnt_cb_fn.set_debug			= PE_DNT_HW_H13_SetDebugSettings;
			_g_pe_dnt_cb_fn.set_film_mode		= PE_DNT_HW_H13_SetFilmMode;
			_g_pe_dnt_cb_fn.get_film_mode		= PE_DNT_HW_H13_GetFilmMode;
			_g_pe_dnt_cb_fn.set_low_delay		= PE_DNT_HW_H13_SetLowDelayMode;
			_g_pe_dnt_cb_fn.get_low_delay		= PE_DNT_HW_H13_GetLowDelayMode;
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
 * init dnt
 *
 * @param   *pstParams [in] PE_CFG_CTRL_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_DNT_Init(PE_CFG_CTRL_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(_g_pe_dnt_cb_fn.hw_init)
		{
			ret = _g_pe_dnt_cb_fn.hw_init(pstParams);
			PE_CHECK_CODE(ret, break, "hw_init() error.\n");
		}
	}while (0);
	return ret;
}

/**
 * standard function of dnt setting
 *
 * @param   data [in/out] unsigned int
 * @param   functype [in/out] unsigned int
 * @param   datasize [in/out] unsigned int
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_DNT_SetFunction(unsigned int *data, unsigned int functype, unsigned int datasize)
{
	int ret = RET_ERROR;

#define PE_DNT_SET_FUNC(_struct, _func)	\
	case PE_ITEM_PKTFUNC(_struct):\
	{\
		_struct *p_rcv, param;\
		do{\
			PE_DNT_DBG_PRINT("S %s\n", #_struct);\
			PE_CHECK_CODE(datasize!=sizeof(_struct), break, \
				"[%s,%d] size mismatch.\n", __F__, __L__);\
			p_rcv = (_struct *)data;\
			memcpy(&param, p_rcv, datasize);\
			ret = _func(&param);\
			PE_CHECK_CODE(ret, break, \
				"[%s,%d] %s() error.\n", __F__, __L__, #_func);\
			PE_DNT_DBG_PRINT("E %s\n", #_struct);\
		}while (0);\
	}\
	break

	/* data null check */
	PE_CHECK_CODE(!data, return RET_ERROR, \
		"[%s,%d] data is null.\n", __F__, __L__);
	switch (functype)
	{
		PE_DNT_SET_FUNC(LX_PE_DNT_FILMMODE_T, 		PE_DNT_SetFilmMode);
		PE_DNT_SET_FUNC(LX_PE_DNT_LD_MODE_T, 		PE_DNT_SetLowDelayMode);
		PE_DNT_SET_FUNC(LX_PE_DEFAULT_SETTINGS_T, 	PE_DNT_SetDefaultSettings);
		PE_DNT_SET_FUNC(LX_PE_DBG_SETTINGS_T, 		PE_DNT_SetDebugSettings);
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
 * standard function of dnt getting
 *
 * @param   data [in/out] unsigned int
 * @param   functype [in/out] unsigned int
 * @param   datasize [in/out] unsigned int
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_DNT_GetFunction(unsigned int *data, unsigned int functype, unsigned int datasize)
{
	int ret = RET_ERROR;

#define PE_DNT_GET_FUNC(_struct, _func)	\
	case PE_ITEM_PKTFUNC(_struct):\
	{\
		_struct *p_rcv, param;\
		do{\
			PE_DNT_DBG_PRINT("S %s\n", #_struct);\
			PE_CHECK_CODE(datasize!=sizeof(_struct), break, \
				"[%s,%d] size mismatch.\n", __F__, __L__);\
			p_rcv = (_struct *)data;\
			memcpy(&param, p_rcv, datasize);\
			ret = _func(&param);\
			PE_CHECK_CODE(ret, break, \
				"[%s,%d] %s() error.\n", __F__, __L__, #_func);\
			memcpy(p_rcv, &param, datasize);\
			PE_DNT_DBG_PRINT("E %s\n", #_struct);\
		}while (0);\
	}\
	break

	/* data null check */
	PE_CHECK_CODE(!data, return RET_ERROR, \
		"[%s,%d] data is null.\n", __F__, __L__);
	switch (functype)
	{
		PE_DNT_GET_FUNC(LX_PE_DNT_FILMMODE_T, 	PE_DNT_GetFilmMode);
		PE_DNT_GET_FUNC(LX_PE_DNT_LD_MODE_T, 	PE_DNT_GetLowDelayMode);
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
 * @param   *pstParams [in]LX_PE_DEFAULT_SETTINGS_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_DNT_SetDefaultSettings(LX_PE_DEFAULT_SETTINGS_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(_g_pe_dnt_cb_fn.set_default)
		{
			ret = _g_pe_dnt_cb_fn.set_default(pstParams);
			PE_CHECK_CODE(ret, break, "set_default() error.\n");
		}
	}while (0);
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
static int PE_DNT_SetDebugSettings(LX_PE_DBG_SETTINGS_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		/* set print level */
		if (pstParams->type&LX_PE_DBG_LV)
		{
			_g_dnt_trace = (pstParams->print_lvl & \
				(0x1<<PE_ITEM_PKTMODL(DNT)))? 0x1:0x0;
		}
		if(_g_pe_dnt_cb_fn.set_debug)
		{
			ret = _g_pe_dnt_cb_fn.set_debug(pstParams);
			PE_CHECK_CODE(ret, break, "set_debug() error.\n");
		}
	}while (0);
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
static int PE_DNT_SetFilmMode(LX_PE_DNT_FILMMODE_T *pstParams)
{
	int ret = RET_OK;
	do {
		CHECK_KNULL(pstParams);
		if(_g_pe_dnt_cb_fn.set_film_mode)
		{
			ret = _g_pe_dnt_cb_fn.set_film_mode(pstParams);
			PE_CHECK_CODE(ret, break, "set_film_mode() error.\n");
		}
	}while (0);
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
static int PE_DNT_GetFilmMode(LX_PE_DNT_FILMMODE_T *pstParams)
{
	int ret = RET_OK;
	do {
		CHECK_KNULL(pstParams);
		if(_g_pe_dnt_cb_fn.get_film_mode)
		{
			ret = _g_pe_dnt_cb_fn.get_film_mode(pstParams);
			PE_CHECK_CODE(ret, break, "get_film_mode() error.\n");
		}
	}while (0);
	return ret;
}
/**
 * set low delay mode
 *
 * @param   *pstParams [in] LX_PE_DNT_LD_MODE_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_DNT_SetLowDelayMode(LX_PE_DNT_LD_MODE_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(_g_pe_dnt_cb_fn.set_low_delay)
		{
			ret = _g_pe_dnt_cb_fn.set_low_delay(pstParams);
			PE_CHECK_CODE(ret, break, "set_low_delay() error.\n");
		}
	}while (0);
	return ret;
}
/**
 * get low delay mode
 *
 * @param   *pstParams [in/out] LX_PE_DNT_LD_MODE_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_DNT_GetLowDelayMode(LX_PE_DNT_LD_MODE_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(_g_pe_dnt_cb_fn.get_low_delay)
		{
			ret = _g_pe_dnt_cb_fn.get_low_delay(pstParams);
			PE_CHECK_CODE(ret, break, "get_low_delay() error.\n");
		}
	}while (0);
	return ret;
}

