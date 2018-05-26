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

/** @file pe_ccm.c
 *
 *  driver for picture enhance color correction module functions. 
 *  ( used only within kdriver )
 *	- white balance, gamma control.
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
#include "pe_ccm.h"
#include "pe_etc_hw.h"
#include "pe_ccm_hw.h"
#include "pe_cmn_hw.h"

/*----------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------*/
#define PE_CCM_DBG_PRINT(fmt, args...)	\
	if (_g_ccm_trace)\
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
	int (*set_gamma_lut)(LX_PE_CCM_GAMMALUT_T *pstParams);
	int (*get_gamma_lut)(LX_PE_CCM_GAMMALUT_T *pstParams);
	int (*set_white_balance)(LX_PE_CCM_WB_T *pstParams);
	int (*get_white_balance)(LX_PE_CCM_WB_T *pstParams);
	int (*set_auto_correct)(LX_PE_CCM_AUTO_CR_T *pstParams);
	int (*get_auto_correct)(LX_PE_CCM_AUTO_CR_T *pstParams);
	int (*set_pxl_replace)(LX_PE_CCM_PIXEL_REPLACE_T *pstParams);
	int (*get_pxl_replace)(LX_PE_CCM_PIXEL_REPLACE_T *pstParams);
}
PE_CCM_CB_FUNC_T;

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
static int PE_CCM_SetDefaultSettings(LX_PE_DEFAULT_SETTINGS_T *pstParams);
static int PE_CCM_SetDebugSettings(LX_PE_DBG_SETTINGS_T *pstParams);
static int PE_CCM_SetGammaLut(LX_PE_CCM_GAMMALUT_T *pstParams);
static int PE_CCM_GetGammaLut(LX_PE_CCM_GAMMALUT_T *pstParams);
static int PE_CCM_SetWhiteBalance(LX_PE_CCM_WB_T *pstParams);
static int PE_CCM_GetWhiteBalance(LX_PE_CCM_WB_T *pstParams);
static int PE_CCM_SetAutoCorrection(LX_PE_CCM_AUTO_CR_T *pstParams);
static int PE_CCM_GetAutoCorrection(LX_PE_CCM_AUTO_CR_T *pstParams);
static int PE_CCM_SetPixelReplacement(LX_PE_CCM_PIXEL_REPLACE_T *pstParams);
static int PE_CCM_GetPixelReplacement(LX_PE_CCM_PIXEL_REPLACE_T *pstParams);

/*----------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------*/
static PE_CCM_CB_FUNC_T _g_pe_ccm_cb_fn;
static UINT32 _g_ccm_trace=0x0;			//default should be off.

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
int PE_CCM_RegisterCbFunc(void)
{
	int ret = RET_OK;

	memset(&_g_pe_ccm_cb_fn, 0, sizeof(PE_CCM_CB_FUNC_T));
	do{
		#ifdef USE_PE_KDRV_CODES_FOR_H15
		if (PE_KDRV_VER_H15)
		{
			_g_pe_ccm_cb_fn.hw_init				= PE_CCM_HW_H15_Init;
			_g_pe_ccm_cb_fn.set_default			= NULL;
			_g_pe_ccm_cb_fn.set_debug			= PE_CCM_HW_H15_SetDebugSettings;
			_g_pe_ccm_cb_fn.set_gamma_lut		= PE_CCM_HW_H15_SetGammaLut;
			_g_pe_ccm_cb_fn.get_gamma_lut		= PE_CCM_HW_H15_GetGammaLut;
			_g_pe_ccm_cb_fn.set_white_balance	= PE_CCM_HW_H15_SetWhiteBalance;
			_g_pe_ccm_cb_fn.get_white_balance	= PE_CCM_HW_H15_GetWhiteBalance;
			_g_pe_ccm_cb_fn.set_auto_correct	= PE_CCM_HW_H15_SetAutoCorrection;
			_g_pe_ccm_cb_fn.get_auto_correct	= PE_CCM_HW_H15_GetAutoCorrection;
			_g_pe_ccm_cb_fn.set_pxl_replace		= PE_CCM_HW_H15_SetPixelReplacement;
			_g_pe_ccm_cb_fn.get_pxl_replace		= PE_CCM_HW_H15_GetPixelReplacement;
		}
		else
		#endif
		#ifdef USE_PE_KDRV_CODES_FOR_M14
		if (PE_KDRV_VER_M14)
		{
			_g_pe_ccm_cb_fn.hw_init				= PE_CCM_HW_M14_Init;
			_g_pe_ccm_cb_fn.set_default			= NULL;
			_g_pe_ccm_cb_fn.set_debug			= PE_CCM_HW_M14_SetDebugSettings;
			_g_pe_ccm_cb_fn.set_gamma_lut		= PE_CCM_HW_M14_SetGammaLut;
			_g_pe_ccm_cb_fn.get_gamma_lut		= PE_CCM_HW_M14_GetGammaLut;
			_g_pe_ccm_cb_fn.set_white_balance	= PE_CCM_HW_M14_SetWhiteBalance;
			_g_pe_ccm_cb_fn.get_white_balance	= PE_CCM_HW_M14_GetWhiteBalance;
			_g_pe_ccm_cb_fn.set_auto_correct	= PE_CCM_HW_M14_SetAutoCorrection;
			_g_pe_ccm_cb_fn.get_auto_correct	= PE_CCM_HW_M14_GetAutoCorrection;
			_g_pe_ccm_cb_fn.set_pxl_replace		= PE_CCM_HW_M14_SetPixelReplacement;
			_g_pe_ccm_cb_fn.get_pxl_replace		= PE_CCM_HW_M14_GetPixelReplacement;
		}
		else
		#endif
		#ifdef USE_PE_KDRV_CODES_FOR_H14
		if (PE_KDRV_VER_H14)
		{
			_g_pe_ccm_cb_fn.hw_init				= PE_CCM_HW_H14_Init;
			_g_pe_ccm_cb_fn.set_default			= NULL;
			_g_pe_ccm_cb_fn.set_debug			= PE_CCM_HW_H14_SetDebugSettings;
			_g_pe_ccm_cb_fn.set_gamma_lut		= PE_CCM_HW_H14_SetGammaLut;
			_g_pe_ccm_cb_fn.get_gamma_lut		= PE_CCM_HW_H14_GetGammaLut;
			_g_pe_ccm_cb_fn.set_white_balance	= PE_CCM_HW_H14_SetWhiteBalance;
			_g_pe_ccm_cb_fn.get_white_balance	= PE_CCM_HW_H14_GetWhiteBalance;
			_g_pe_ccm_cb_fn.set_auto_correct	= PE_CCM_HW_H14_SetAutoCorrection;
			_g_pe_ccm_cb_fn.get_auto_correct	= PE_CCM_HW_H14_GetAutoCorrection;
			_g_pe_ccm_cb_fn.set_pxl_replace		= PE_CCM_HW_H14_SetPixelReplacement;
			_g_pe_ccm_cb_fn.get_pxl_replace		= PE_CCM_HW_H14_GetPixelReplacement;
		}
		else
		#endif
		#ifdef USE_PE_KDRV_CODES_FOR_H13
		if (PE_KDRV_VER_H13)
		{
			_g_pe_ccm_cb_fn.hw_init				= PE_CCM_HW_H13_Init;
			_g_pe_ccm_cb_fn.set_default			= NULL;
			_g_pe_ccm_cb_fn.set_debug			= PE_CCM_HW_H13_SetDebugSettings;
			_g_pe_ccm_cb_fn.set_gamma_lut		= PE_CCM_HW_H13_SetGammaLut;
			_g_pe_ccm_cb_fn.get_gamma_lut		= PE_CCM_HW_H13_GetGammaLut;
			_g_pe_ccm_cb_fn.set_white_balance	= PE_CCM_HW_H13_SetWhiteBalance;
			_g_pe_ccm_cb_fn.get_white_balance	= PE_CCM_HW_H13_GetWhiteBalance;
			_g_pe_ccm_cb_fn.set_auto_correct	= PE_CCM_HW_H13_SetAutoCorrection;
			_g_pe_ccm_cb_fn.get_auto_correct	= PE_CCM_HW_H13_GetAutoCorrection;
			_g_pe_ccm_cb_fn.set_pxl_replace		= PE_CCM_HW_H13_SetPixelReplacement;
			_g_pe_ccm_cb_fn.get_pxl_replace		= PE_CCM_HW_H13_GetPixelReplacement;
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
 * init color correctin module
 *
 * @param   *pstParams [in] PE_CFG_CTRL_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_CCM_Init(PE_CFG_CTRL_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(_g_pe_ccm_cb_fn.hw_init)
		{
			ret = _g_pe_ccm_cb_fn.hw_init(pstParams);
			PE_CHECK_CODE(ret, break, "hw_init() error.\n");
		}
	}while (0);
	return ret;
}

/**
 * standard function of ccm setting
 *
 * @param   data [in/out] unsigned int
 * @param   functype [in] unsigned int
 * @param   datasize [in] unsigned int
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_CCM_SetFunction(unsigned int *data, unsigned int functype, unsigned int datasize)
{
	int ret = RET_ERROR;

#define PE_CCM_SET_TABL(_struct, _func)	\
	case PE_ITEM_PKTFUNC(_struct):\
	{\
		_struct *p_rcv, param;\
		UINT32 *p_data = NULL;\
		UINT32 bufsize;\
		do{\
			PE_CCM_DBG_PRINT("S %s\n", #_struct);\
			PE_CHECK_CODE(datasize!=sizeof(_struct), break, \
				"[%s,%d] size mismatch.\n", __F__, __L__);\
			p_rcv = (_struct *)data;\
			bufsize = GET_BITS(p_rcv->size, 0, 16) * sizeof(UINT32);\
			PE_CHECK_CODE(!bufsize, break, "[%s,%d] bufsize zero.\n", __F__, __L__);\
			p_data = OS_Malloc(bufsize);\
			PE_CHECK_CODE(!p_data, break, "[%s,%d] p_data is null.\n", __F__, __L__);\
			ret = copy_from_user(p_data, (void __user *)p_rcv->data, bufsize);\
			PE_CHECK_CODE(ret, break, "[%s,%d] copy_from_user error.\n", __F__, __L__);\
			param.win_id = p_rcv->win_id;\
			param.size = p_rcv->size;\
			param.data = p_data;\
			ret = _func(&param);\
			PE_CHECK_CODE(ret, break, "[%s,%d] %s() error.\n", __F__, __L__, #_func);\
			PE_CCM_DBG_PRINT("E %s\n", #_struct);\
		}while (0);\
		if (p_data){OS_Free(p_data);}\
	}\
	break

#define PE_CCM_SET_FUNC(_struct, _func)	\
	case PE_ITEM_PKTFUNC(_struct):\
	{\
		_struct *p_rcv, param;\
		do{\
			PE_CCM_DBG_PRINT("S %s\n", #_struct);\
			PE_CHECK_CODE(datasize!=sizeof(_struct), break, \
				"[%s,%d] size mismatch.\n", __F__, __L__);\
			p_rcv = (_struct *)data;\
			memcpy(&param, p_rcv, datasize);\
			ret = _func(&param);\
			PE_CHECK_CODE(ret, break, "[%s,%d] %s() error.\n", __F__, __L__, #_func);\
			PE_CCM_DBG_PRINT("E %s\n", #_struct);\
		}while (0);\
	}\
	break

	/* data null check */
	PE_CHECK_CODE(!data, return RET_ERROR, "[%s,%d] data is null.\n", __F__, __L__);
	switch (functype)
	{
		PE_CCM_SET_FUNC(LX_PE_CCM_PIXEL_REPLACE_T,	PE_CCM_SetPixelReplacement);
		PE_CCM_SET_FUNC(LX_PE_CCM_WB_T,				PE_CCM_SetWhiteBalance);
		PE_CCM_SET_FUNC(LX_PE_CCM_AUTO_CR_T,		PE_CCM_SetAutoCorrection);
		PE_CCM_SET_FUNC(LX_PE_DEFAULT_SETTINGS_T,	PE_CCM_SetDefaultSettings);
		PE_CCM_SET_FUNC(LX_PE_DBG_SETTINGS_T,		PE_CCM_SetDebugSettings);
		PE_CCM_SET_TABL(LX_PE_CCM_GAMMALUT_T,		PE_CCM_SetGammaLut);
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
 * standard function of ccm getting
 *
 * @param   data [in/out] unsigned int
 * @param   functype [in] unsigned int
 * @param   datasize [in] unsigned int
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_CCM_GetFunction(unsigned int *data, unsigned int functype, unsigned int datasize)
{
	int ret = RET_ERROR;

#define PE_CCM_GET_TABL(_struct, _func)	\
	case PE_ITEM_PKTFUNC(_struct):\
	{\
		_struct *p_rcv, param;\
		UINT32 *p_data = NULL;\
		UINT32 bufsize;\
		memset0(param);\
		do{\
			PE_CCM_DBG_PRINT("S %s\n", #_struct);\
			PE_CHECK_CODE(datasize!=sizeof(_struct), break, \
				"[%s,%d] size mismatch.\n", __F__, __L__);\
			p_rcv = (_struct *)data;\
			bufsize = GET_BITS(p_rcv->size, 0, 16) * sizeof(UINT32);\
			PE_CHECK_CODE(!bufsize, break, "[%s,%d] bufsize zero.\n", __F__, __L__);\
			p_data = OS_Malloc(bufsize);\
			PE_CHECK_CODE(!p_data, break, "[%s,%d] p_data is null.\n", __F__, __L__);\
			param.win_id = p_rcv->win_id;\
			param.size = p_rcv->size;\
			param.data = p_data;\
			ret = _func(&param);\
			PE_CHECK_CODE(ret, break, "[%s,%d] %s() error.\n", __F__, __L__, #_func);\
			ret = copy_to_user((void __user *)p_rcv->data, p_data, bufsize);\
			PE_CHECK_CODE(ret, break, "[%s,%d] copy_to_user error.\n", __F__, __L__);\
			PE_CCM_DBG_PRINT("E %s\n", #_struct);\
		}while (0);\
		if (p_data){OS_Free(p_data);}\
	}\
	break

#define PE_CCM_GET_FUNC(_struct, _func)	\
	case PE_ITEM_PKTFUNC(_struct):\
	{\
		_struct *p_rcv, param;\
		do{\
			PE_CCM_DBG_PRINT("S %s\n", #_struct);\
			PE_CHECK_CODE(datasize!=sizeof(_struct), break, \
				"[%s,%d] size mismatch.\n", __F__, __L__);\
			p_rcv = (_struct *)data;\
			memcpy(&param, p_rcv, datasize);\
			ret = _func(&param);\
			PE_CHECK_CODE(ret, break, "[%s,%d] %s() error.\n", __F__, __L__, #_func);\
			memcpy(p_rcv, &param, datasize);\
			PE_CCM_DBG_PRINT("E %s\n", #_struct);\
		}while (0);\
	}\
	break

	/* data null check */
	PE_CHECK_CODE(!data, return RET_ERROR, "[%s,%d] data is null.\n", __F__, __L__);
	switch (functype)
	{
		PE_CCM_GET_FUNC(LX_PE_CCM_PIXEL_REPLACE_T,	PE_CCM_GetPixelReplacement);
		PE_CCM_GET_FUNC(LX_PE_CCM_WB_T,				PE_CCM_GetWhiteBalance);
		PE_CCM_GET_FUNC(LX_PE_CCM_AUTO_CR_T,		PE_CCM_GetAutoCorrection);
		PE_CCM_GET_TABL(LX_PE_CCM_GAMMALUT_T,		PE_CCM_GetGammaLut);
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
static int PE_CCM_SetDefaultSettings(LX_PE_DEFAULT_SETTINGS_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		PE_CCM_DBG_PRINT("[win id:%d]\n", pstParams->win_id);
	}while (0);
	return ret;
}
/**
 * ccm Debug setting
 *
 * @param   *pstParams [in/out] LX_PE_DBG_SETTINGS_T int
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_CCM_SetDebugSettings(LX_PE_DBG_SETTINGS_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		/* set print level */
		if (pstParams->type&LX_PE_DBG_LV)
		{
			_g_ccm_trace = (pstParams->print_lvl & \
				(0x1<<PE_ITEM_PKTMODL(CCM)))? 0x1:0x0;
		}
		if(_g_pe_ccm_cb_fn.set_debug)
		{
			ret = _g_pe_ccm_cb_fn.set_debug(pstParams);
			PE_CHECK_CODE(ret, break, "set_debug() error.\n");
		}
	}while (0);
	return ret;
}
/**
 * set gamma correction lut
 *
 * @param   *pstParams [in] LX_PE_CCM_GAMMALUT_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_CCM_SetGammaLut(LX_PE_CCM_GAMMALUT_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(_g_pe_ccm_cb_fn.set_gamma_lut)
		{
			ret = _g_pe_ccm_cb_fn.set_gamma_lut(pstParams);
			PE_CHECK_CODE(ret, break, "set_gamma_lut() error.\n");
		}
	}while (0);
	return ret;
}

/**
 * get gamma correction lut
 *
 * @param   *pstParams [in/out] LX_PE_CCM_GAMMALUT_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_CCM_GetGammaLut(LX_PE_CCM_GAMMALUT_T *pstParams)
{
	int ret = RET_OK;
	do {
		CHECK_KNULL(pstParams);
		if(_g_pe_ccm_cb_fn.get_gamma_lut)
		{
			ret = _g_pe_ccm_cb_fn.get_gamma_lut(pstParams);
			PE_CHECK_CODE(ret, break, "get_gamma_lut() error.\n");
		}
	} while (0);
	return ret;
}
/**
 * set pixel replacement
 *
 * @param   *pstParams [in/out] LX_PE_CCM_PIXEL_REPLACE_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_CCM_SetPixelReplacement(LX_PE_CCM_PIXEL_REPLACE_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(_g_pe_ccm_cb_fn.set_pxl_replace)
		{
			ret = _g_pe_ccm_cb_fn.set_pxl_replace(pstParams);
			PE_CHECK_CODE(ret, break, "set_pxl_replace() error.\n");
		}
	}while (0);
	return ret;
}
/**
 * get pixel replacement
 *
 * @param   *pstParams [in/out] LX_PE_CCM_PIXEL_REPLACE_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_CCM_GetPixelReplacement(LX_PE_CCM_PIXEL_REPLACE_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(_g_pe_ccm_cb_fn.get_pxl_replace)
		{
			ret = _g_pe_ccm_cb_fn.get_pxl_replace(pstParams);
			PE_CHECK_CODE(ret, break, "get_pxl_replace() error.\n");
		}
	}while (0);
	return ret;
}
/**
 * set white Balance
 *
 * @param   *pstParams [in/out] LX_PE_CCM_WB_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_CCM_SetWhiteBalance(LX_PE_CCM_WB_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(_g_pe_ccm_cb_fn.set_white_balance)
		{
			ret = _g_pe_ccm_cb_fn.set_white_balance(pstParams);
			PE_CHECK_CODE(ret, break, "set_white_balance() error.\n");
		}
	}while (0);
	return ret;
}
/**
 * get white Balance
 *
 * @param   *pstParams [in/out] LX_PE_CCM_WB_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_CCM_GetWhiteBalance(LX_PE_CCM_WB_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(_g_pe_ccm_cb_fn.get_white_balance)
		{
			ret = _g_pe_ccm_cb_fn.get_white_balance(pstParams);
			PE_CHECK_CODE(ret, break, "get_white_balance() error.\n");
		}
	}while (0);
	return ret;
}
/**
 * set auto correction
 *
 * @param   *pstParams [in/out] LX_PE_CCM_AUTO_CR_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_CCM_SetAutoCorrection(LX_PE_CCM_AUTO_CR_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(_g_pe_ccm_cb_fn.set_auto_correct)
		{
			ret = _g_pe_ccm_cb_fn.set_auto_correct(pstParams);
			PE_CHECK_CODE(ret, break, "set_auto_correct() error.\n");
		}
	}while (0);

	return ret;
}
/**
 * get auto correction
 *
 * @param   *pstParams [in/out] LX_PE_CCM_AUTO_CR_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_CCM_GetAutoCorrection(LX_PE_CCM_AUTO_CR_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(_g_pe_ccm_cb_fn.get_auto_correct)
		{
			ret = _g_pe_ccm_cb_fn.get_auto_correct(pstParams);
			PE_CHECK_CODE(ret, break, "get_auto_correct() error.\n");
		}
	}while (0);
	return ret;
}

