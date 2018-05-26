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

/** @file pe_csc.c
 *
 *  driver for picture enhance color space conversion functions.
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
#include "pe_etc.h"
#include "pe_csc.h"
#include "pe_etc_hw.h"
#include "pe_csc_hw.h"

/*----------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------*/
#define PE_CSC_DBG_PRINT(fmt, args...)	\
	if (_g_csc_trace)\
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
	int (*set_color_gamut)(LX_PE_CSC_GAMUT_T *pstParams);
	int (*get_color_gamut)(LX_PE_CSC_GAMUT_T *pstParams);
	int (*set_post_csc)(LX_PE_CSC_POST_T *pstParams);
	int (*get_post_csc)(LX_PE_CSC_POST_T *pstParams);
	int (*set_input_csc)(LX_PE_CSC_INPUT_T *pstParams);
	int (*get_input_csc)(LX_PE_CSC_INPUT_T *pstParams);
	int (*set_xvycc)(LX_PE_CSC_XVYCC_T *pstParams);
	int (*get_xvycc)(LX_PE_CSC_XVYCC_T *pstParams);
}
PE_CSC_CB_FUNC_T;

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
static int PE_CSC_SetDefaultSettings(LX_PE_DEFAULT_SETTINGS_T *pstParams);
static int PE_CSC_SetDebugSettings(LX_PE_DBG_SETTINGS_T *pstParams);
static int PE_CSC_SetColorGamut(LX_PE_CSC_GAMUT_T *pstParams);
static int PE_CSC_GetColorGamut(LX_PE_CSC_GAMUT_T *pstParams);
static int PE_CSC_SetPostCsc(LX_PE_CSC_POST_T *pstParams);
static int PE_CSC_GetPostCsc(LX_PE_CSC_POST_T *pstParams);
static int PE_CSC_SetInputCsc(LX_PE_CSC_INPUT_T *pstParams);
static int PE_CSC_GetInputCsc(LX_PE_CSC_INPUT_T *pstParams);
static int PE_CSC_SetxvYCC(LX_PE_CSC_XVYCC_T *pstParams);
static int PE_CSC_GetxvYCC(LX_PE_CSC_XVYCC_T *pstParams);

/*----------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------*/
static PE_CSC_CB_FUNC_T _g_pe_csc_cb_fn;
static UINT32 _g_csc_trace=0x0;	//default should be off.

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
int PE_CSC_RegisterCbFunc(void)
{
	int ret = RET_OK;

	memset(&_g_pe_csc_cb_fn, 0, sizeof(PE_CSC_CB_FUNC_T));
	do{
		#ifdef USE_PE_KDRV_CODES_FOR_H15
		if (PE_KDRV_VER_H15)
		{
			_g_pe_csc_cb_fn.hw_init				= PE_CSC_HW_H15_Init;
			_g_pe_csc_cb_fn.set_default			= NULL;
			_g_pe_csc_cb_fn.set_debug			= PE_CSC_HW_H15_SetDebugSettings;
			_g_pe_csc_cb_fn.set_color_gamut		= PE_CSC_HW_H15_SetColorGamut;
			_g_pe_csc_cb_fn.get_color_gamut		= PE_CSC_HW_H15_GetColorGamut;
			_g_pe_csc_cb_fn.set_post_csc		= PE_CSC_HW_H15_SetPostCsc;
			_g_pe_csc_cb_fn.get_post_csc		= PE_CSC_HW_H15_GetPostCsc;
			_g_pe_csc_cb_fn.set_input_csc		= PE_CSC_HW_H15_SetInputCsc;
			_g_pe_csc_cb_fn.get_input_csc		= PE_CSC_HW_H15_GetInputCsc;
			_g_pe_csc_cb_fn.set_xvycc			= PE_CSC_HW_H15_SetxvYCC;
			_g_pe_csc_cb_fn.get_xvycc			= PE_CSC_HW_H15_GetxvYCC;
		}
		else
		#endif
		#ifdef USE_PE_KDRV_CODES_FOR_M14
		if (PE_KDRV_VER_M14)
		{
			_g_pe_csc_cb_fn.hw_init				= PE_CSC_HW_M14_Init;
			_g_pe_csc_cb_fn.set_default			= NULL;
			_g_pe_csc_cb_fn.set_debug			= PE_CSC_HW_M14_SetDebugSettings;
			_g_pe_csc_cb_fn.set_color_gamut		= PE_CSC_HW_M14_SetColorGamut;
			_g_pe_csc_cb_fn.get_color_gamut		= PE_CSC_HW_M14_GetColorGamut;
			_g_pe_csc_cb_fn.set_post_csc		= PE_CSC_HW_M14_SetPostCsc;
			_g_pe_csc_cb_fn.get_post_csc		= PE_CSC_HW_M14_GetPostCsc;
			_g_pe_csc_cb_fn.set_input_csc		= PE_CSC_HW_M14_SetInputCsc;
			_g_pe_csc_cb_fn.get_input_csc		= PE_CSC_HW_M14_GetInputCsc;
			_g_pe_csc_cb_fn.set_xvycc			= PE_CSC_HW_M14_SetxvYCC;
			_g_pe_csc_cb_fn.get_xvycc			= PE_CSC_HW_M14_GetxvYCC;
		}
		else
		#endif
		#ifdef USE_PE_KDRV_CODES_FOR_H14
		if (PE_KDRV_VER_H14)
		{
			_g_pe_csc_cb_fn.hw_init				= PE_CSC_HW_H14_Init;
			_g_pe_csc_cb_fn.set_default			= NULL;
			_g_pe_csc_cb_fn.set_debug			= PE_CSC_HW_H14_SetDebugSettings;
			_g_pe_csc_cb_fn.set_color_gamut		= PE_CSC_HW_H14_SetColorGamut;
			_g_pe_csc_cb_fn.get_color_gamut		= PE_CSC_HW_H14_GetColorGamut;
			_g_pe_csc_cb_fn.set_post_csc		= PE_CSC_HW_H14_SetPostCsc;
			_g_pe_csc_cb_fn.get_post_csc		= PE_CSC_HW_H14_GetPostCsc;
			_g_pe_csc_cb_fn.set_input_csc		= PE_CSC_HW_H14_SetInputCsc;
			_g_pe_csc_cb_fn.get_input_csc		= PE_CSC_HW_H14_GetInputCsc;
			_g_pe_csc_cb_fn.set_xvycc			= PE_CSC_HW_H14_SetxvYCC;
			_g_pe_csc_cb_fn.get_xvycc			= PE_CSC_HW_H14_GetxvYCC;
		}
		else
		#endif
		#ifdef USE_PE_KDRV_CODES_FOR_H13
		if (PE_KDRV_VER_H13)
		{
			_g_pe_csc_cb_fn.hw_init				= PE_CSC_HW_H13_Init;
			_g_pe_csc_cb_fn.set_default			= NULL;
			_g_pe_csc_cb_fn.set_debug			= PE_CSC_HW_H13_SetDebugSettings;
			_g_pe_csc_cb_fn.set_color_gamut		= PE_CSC_HW_H13_SetColorGamut;
			_g_pe_csc_cb_fn.get_color_gamut		= PE_CSC_HW_H13_GetColorGamut;
			_g_pe_csc_cb_fn.set_post_csc		= PE_CSC_HW_H13_SetPostCsc;
			_g_pe_csc_cb_fn.get_post_csc		= PE_CSC_HW_H13_GetPostCsc;
			_g_pe_csc_cb_fn.set_input_csc		= PE_CSC_HW_H13_SetInputCsc;
			_g_pe_csc_cb_fn.get_input_csc		= PE_CSC_HW_H13_GetInputCsc;
			_g_pe_csc_cb_fn.set_xvycc			= PE_CSC_HW_H13_SetxvYCC;
			_g_pe_csc_cb_fn.get_xvycc			= PE_CSC_HW_H13_GetxvYCC;
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
 * init csc
 *
 * @param   *pstParams [in] PE_CFG_CTRL_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_CSC_Init(PE_CFG_CTRL_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(_g_pe_csc_cb_fn.hw_init)
		{
			ret = _g_pe_csc_cb_fn.hw_init(pstParams);
			PE_CHECK_CODE(ret, break, "hw_init() error.\n");
		}
	}while (0);
	return ret;
}

/**
 * standard function of csc setting
 *
 * @param   data [in/out] unsigned int
 * @param   functype [in/out] unsigned int
 * @param   datasize [in/out] unsigned int
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_CSC_SetFunction(unsigned int *data, unsigned int functype, unsigned int datasize)
{
	int ret = RET_ERROR;

#define PE_CSC_SET_FUNC(_struct, _func)	\
	case PE_ITEM_PKTFUNC(_struct):\
	{\
		_struct *p_rcv, param;\
		do{\
			PE_CSC_DBG_PRINT("S %s\n", #_struct);\
			PE_CHECK_CODE(datasize!=sizeof(_struct), \
				break, "[%s,%d] size mismatch.\n", __F__, __L__);\
			p_rcv = (_struct *)data;\
			memcpy(&param, p_rcv, datasize);\
			ret = _func(&param);\
			PE_CHECK_CODE(ret, break, \
				"[%s,%d] %s() error.\n", __F__, __L__, #_func);\
			PE_CSC_DBG_PRINT("E %s\n", #_struct);\
		}while (0);\
	}\
	break

	/* data null check */
	PE_CHECK_CODE(!data, return RET_ERROR, \
		"[%s,%d] data is null.\n", __F__, __L__);
	switch (functype)
	{
		PE_CSC_SET_FUNC(LX_PE_CSC_INPUT_T, 			PE_CSC_SetInputCsc);
		PE_CSC_SET_FUNC(LX_PE_CSC_POST_T, 			PE_CSC_SetPostCsc);
		PE_CSC_SET_FUNC(LX_PE_CSC_GAMUT_T, 			PE_CSC_SetColorGamut);
		PE_CSC_SET_FUNC(LX_PE_CSC_XVYCC_T, 			PE_CSC_SetxvYCC);
		PE_CSC_SET_FUNC(LX_PE_DEFAULT_SETTINGS_T, 	PE_CSC_SetDefaultSettings);
		PE_CSC_SET_FUNC(LX_PE_DBG_SETTINGS_T, 		PE_CSC_SetDebugSettings);
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
 * standard function of csc getting
 *
 * @param   data [in/out] unsigned int
 * @param   functype [in/out] unsigned int
 * @param   datasize [in/out] unsigned int
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_CSC_GetFunction(unsigned int *data, unsigned int functype, unsigned int datasize)
{
	int ret = RET_ERROR;

#define PE_CSC_GET_FUNC(_struct, _func)	\
	case PE_ITEM_PKTFUNC(_struct):\
	{\
		_struct *p_rcv, param;\
		do{\
			PE_CSC_DBG_PRINT("S %s\n", #_struct);\
			PE_CHECK_CODE(datasize!=sizeof(_struct), break, \
				"[%s,%d] size mismatch.\n", __F__, __L__);\
			p_rcv = (_struct *)data;\
			memcpy(&param, p_rcv, datasize);\
			ret = _func(&param);\
			PE_CHECK_CODE(ret, break, \
				"[%s,%d] %s() error.\n", __F__, __L__, #_func);\
			memcpy(p_rcv, &param, datasize);\
			PE_CSC_DBG_PRINT("E %s\n", #_struct);\
		}while (0);\
	}\
	break

	/* data null check */
	PE_CHECK_CODE(!data, return RET_ERROR, \
		"[%s,%d] data is null.\n", __F__, __L__);
	switch (functype)
	{
		PE_CSC_GET_FUNC(LX_PE_CSC_INPUT_T, 	PE_CSC_GetInputCsc);
		PE_CSC_GET_FUNC(LX_PE_CSC_POST_T, 	PE_CSC_GetPostCsc);
		PE_CSC_GET_FUNC(LX_PE_CSC_GAMUT_T, 	PE_CSC_GetColorGamut);
		PE_CSC_GET_FUNC(LX_PE_CSC_XVYCC_T, 	PE_CSC_GetxvYCC);
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
static int PE_CSC_SetDefaultSettings(LX_PE_DEFAULT_SETTINGS_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		PE_CSC_DBG_PRINT("[win id:%d]\n", pstParams->win_id);
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
static int PE_CSC_SetDebugSettings(LX_PE_DBG_SETTINGS_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		/* set print level */
		if(pstParams->type&LX_PE_DBG_LV)
		{
			_g_csc_trace = (pstParams->print_lvl & \
				(0x1<<PE_ITEM_PKTMODL(CSC)))? 0x1:0x0;
		}
		if(_g_pe_csc_cb_fn.set_debug)
		{
			ret = _g_pe_csc_cb_fn.set_debug(pstParams);
			PE_CHECK_CODE(ret, break, "set_debug() error.\n");
		}
	}while (0);
	return ret;
}
/**
 * set color gamut
 *
 * @param   *pstParams [in] LX_PE_CSC_GAMUT_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_CSC_SetColorGamut(LX_PE_CSC_GAMUT_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(_g_pe_csc_cb_fn.set_color_gamut)
		{
			ret = _g_pe_csc_cb_fn.set_color_gamut(pstParams);
			PE_CHECK_CODE(ret, break, "set_color_gamut() error.\n");
		}
	}while (0);
	return ret;
}
/**
 * get color gamut
 *
 * @param   *pstParams [in/out] LX_PE_CSC_GAMUT_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_CSC_GetColorGamut(LX_PE_CSC_GAMUT_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(_g_pe_csc_cb_fn.get_color_gamut)
		{
			ret = _g_pe_csc_cb_fn.get_color_gamut(pstParams);
			PE_CHECK_CODE(ret, break, "get_color_gamut() error.\n");
		}
	}while (0);
	return ret;
}
/**
 * set post csc
 *
 * @param   *pstParams [in] LX_PE_CSC_POST_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_CSC_SetPostCsc(LX_PE_CSC_POST_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(_g_pe_csc_cb_fn.set_post_csc)
		{
			ret = _g_pe_csc_cb_fn.set_post_csc(pstParams);
			PE_CHECK_CODE(ret, break, "set_post_csc() error.\n");
		}
	}while (0);

	return ret;
}
/**
 * get post csc
 *
 * @param   *pstParams [in/out] LX_PE_CSC_POST_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_CSC_GetPostCsc(LX_PE_CSC_POST_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(_g_pe_csc_cb_fn.get_post_csc)
		{
			ret = _g_pe_csc_cb_fn.get_post_csc(pstParams);
			PE_CHECK_CODE(ret, break, "get_post_csc() error.\n");
		}
	}while (0);
	return ret;
}
/**
 * set input csc
 *
 * @param   *pstParams [in] LX_PE_CSC_INPUT_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_CSC_SetInputCsc(LX_PE_CSC_INPUT_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(_g_pe_csc_cb_fn.set_input_csc)
		{
			ret = _g_pe_csc_cb_fn.set_input_csc(pstParams);
			PE_CHECK_CODE(ret, break, "set_input_csc() error.\n");
		}
	}while (0);
	return ret;
}
/**
 * get input csc
 *
 * @param   *pstParams [in/out] LX_PE_CSC_INPUT_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_CSC_GetInputCsc(LX_PE_CSC_INPUT_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(_g_pe_csc_cb_fn.get_input_csc)
		{
			ret = _g_pe_csc_cb_fn.get_input_csc(pstParams);
			PE_CHECK_CODE(ret, break, "get_input_csc() error.\n");
		}
	}while (0);
	return ret;
}
/**
 * set xvYcc
 *
 * @param   *pstParams [in] LX_PE_CSC_XVYCC_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_CSC_SetxvYCC(LX_PE_CSC_XVYCC_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(_g_pe_csc_cb_fn.set_xvycc)
		{
			ret = _g_pe_csc_cb_fn.set_xvycc(pstParams);
			PE_CHECK_CODE(ret, break, "set_xvycc() error.\n");
		}
	}while (0);
	return ret;
}
/**
 * get xvYcc
 *
 * @param   *pstParams [in/out] LX_PE_CSC_XVYCC_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_CSC_GetxvYCC(LX_PE_CSC_XVYCC_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(_g_pe_csc_cb_fn.get_xvycc)
		{
			ret = _g_pe_csc_cb_fn.get_xvycc(pstParams);
			PE_CHECK_CODE(ret, break, "get_xvycc() error.\n");
		}
	}while (0);
	return ret;
}

