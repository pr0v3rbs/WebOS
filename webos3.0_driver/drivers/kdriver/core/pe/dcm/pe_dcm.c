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

/** @file pe_dcm.c
 *
 *  driver for picture enhance dynamic contrast module functions.
 *  ( used only within kdriver )
 *	- dynamic contrast, saturation enhancement.
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
#include "pe_win.h"
#include "pe_dcm.h"
#include "pe_etc_hw.h"
#include "pe_dcm_hw.h"

/*----------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------*/
#define PE_DCM_DBG_PRINT(fmt, args...)	\
	if (_g_dcm_trace)\
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
	int (*set_dce_config)(LX_PE_DCM_DCE_CONF_T *pstParams);
	int (*get_dce_config)(LX_PE_DCM_DCE_CONF_T *pstParams);
	int (*set_dce_lut)(LX_PE_DCM_DCE_LUT_T *pstParams);
	int (*get_dce_lut)(LX_PE_DCM_DCE_LUT_T *pstParams);
	int (*set_dse_lut)(LX_PE_DCM_DSE_LUT_T *pstParams);
	int (*get_dse_lut)(LX_PE_DCM_DSE_LUT_T *pstParams);
	int (*set_dce_smooth)(void *pstParams);
	int (*get_dce_smooth)(void *pstParams);
	int (*set_dce_blending)(LX_PE_DCM_BLENDING_T *pstParams);
	int (*get_dce_blending)(LX_PE_DCM_BLENDING_T *pstParams);
	int (*set_dce_drc_mode)(LX_PE_DCM_DRC_MODE_T *pstParams);
	int (*get_dce_drc_mode)(LX_PE_DCM_DRC_MODE_T *pstParams);
}
PE_DCM_CB_FUNC_T;

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
static int PE_DCM_SetDefaultSettings(LX_PE_DEFAULT_SETTINGS_T *pstParams);
static int PE_DCM_SetDebugSettings(LX_PE_DBG_SETTINGS_T *pstParams);
static int PE_DCM_SetDceConfig(LX_PE_DCM_DCE_CONF_T *pstParams);
static int PE_DCM_GetDceConfig(LX_PE_DCM_DCE_CONF_T *pstParams);
static int PE_DCM_SetDceLut(LX_PE_DCM_DCE_LUT_T *pstParams);
static int PE_DCM_GetDceLut(LX_PE_DCM_DCE_LUT_T *pstParams);
static int PE_DCM_SetDseLut(LX_PE_DCM_DSE_LUT_T *pstParams);
static int PE_DCM_GetDseLut(LX_PE_DCM_DSE_LUT_T *pstParams);
static int PE_DCM_SetDceSmoothCtrl(void *pstParams);
static int PE_DCM_GetDceSmoothCtrl(void *pstParams);
static int PE_DCM_SetDceBlendingCtrl(LX_PE_DCM_BLENDING_T *pstParams);
static int PE_DCM_GetDceBlendingCtrl(LX_PE_DCM_BLENDING_T *pstParams);
static int PE_DCM_SetDceDrcMode(LX_PE_DCM_DRC_MODE_T *pstParams);
static int PE_DCM_GetDceDrcMode(LX_PE_DCM_DRC_MODE_T *pstParams);

/*----------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------*/
static PE_DCM_CB_FUNC_T _g_pe_dcm_cb_fn;
static UINT32 _g_dcm_trace=0x0;	//default should be off.

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
int PE_DCM_RegisterCbFunc(void)
{
	int ret = RET_OK;

	memset(&_g_pe_dcm_cb_fn, 0, sizeof(PE_DCM_CB_FUNC_T));
	do{
		#ifdef USE_PE_KDRV_CODES_FOR_H15
		if (PE_KDRV_VER_H15)
		{
			_g_pe_dcm_cb_fn.hw_init				= PE_DCM_HW_H15_Init;
			_g_pe_dcm_cb_fn.set_default			= NULL;
			_g_pe_dcm_cb_fn.set_debug			= PE_DCM_HW_H15_SetDebugSettings;
			_g_pe_dcm_cb_fn.set_dce_config		= PE_DCM_HW_H15_SetDceConfig;
			_g_pe_dcm_cb_fn.get_dce_config		= PE_DCM_HW_H15_GetDceConfig;
			_g_pe_dcm_cb_fn.set_dce_lut			= PE_DCM_HW_H15_SetDceLut;
			_g_pe_dcm_cb_fn.get_dce_lut			= PE_DCM_HW_H15_GetDceLut;
			_g_pe_dcm_cb_fn.set_dse_lut			= PE_DCM_HW_H15_SetDseLut;
			_g_pe_dcm_cb_fn.get_dse_lut			= PE_DCM_HW_H15_GetDseLut;
			_g_pe_dcm_cb_fn.set_dce_smooth		= PE_DCM_HW_H15_SetDceSmoothCtrl;
			_g_pe_dcm_cb_fn.get_dce_smooth		= PE_DCM_HW_H15_GetDceSmoothCtrl;
			_g_pe_dcm_cb_fn.set_dce_blending	= PE_DCM_HW_H15_SetDceBlendingCtrl;
			_g_pe_dcm_cb_fn.get_dce_blending	= PE_DCM_HW_H15_GetDceBlendingCtrl;
			_g_pe_dcm_cb_fn.set_dce_drc_mode	= PE_DCM_HW_H15_SetDceDrcMode;
			_g_pe_dcm_cb_fn.get_dce_drc_mode	= PE_DCM_HW_H15_GetDceDrcMode;
		}
		else
		#endif
		#ifdef USE_PE_KDRV_CODES_FOR_M14
		if (PE_KDRV_VER_M14)
		{
			_g_pe_dcm_cb_fn.hw_init				= PE_DCM_HW_M14_Init;
			_g_pe_dcm_cb_fn.set_default			= NULL;
			_g_pe_dcm_cb_fn.set_debug			= PE_DCM_HW_M14_SetDebugSettings;
			_g_pe_dcm_cb_fn.set_dce_config		= PE_DCM_HW_M14_SetDceConfig;
			_g_pe_dcm_cb_fn.get_dce_config		= PE_DCM_HW_M14_GetDceConfig;
			_g_pe_dcm_cb_fn.set_dce_lut			= PE_DCM_HW_M14_SetDceLut;
			_g_pe_dcm_cb_fn.get_dce_lut			= PE_DCM_HW_M14_GetDceLut;
			_g_pe_dcm_cb_fn.set_dse_lut			= PE_DCM_HW_M14_SetDseLut;
			_g_pe_dcm_cb_fn.get_dse_lut			= PE_DCM_HW_M14_GetDseLut;
			_g_pe_dcm_cb_fn.set_dce_smooth		= PE_DCM_HW_M14_SetDceSmoothCtrl;
			_g_pe_dcm_cb_fn.get_dce_smooth		= PE_DCM_HW_M14_GetDceSmoothCtrl;
			_g_pe_dcm_cb_fn.set_dce_blending	= PE_DCM_HW_M14_SetDceBlendingCtrl;
			_g_pe_dcm_cb_fn.get_dce_blending	= PE_DCM_HW_M14_GetDceBlendingCtrl;
			_g_pe_dcm_cb_fn.set_dce_drc_mode	= NULL;
			_g_pe_dcm_cb_fn.get_dce_drc_mode	= NULL;
		}
		else
		#endif
		#ifdef USE_PE_KDRV_CODES_FOR_H14
		if (PE_KDRV_VER_H14)
		{
			_g_pe_dcm_cb_fn.hw_init				= PE_DCM_HW_H14_Init;
			_g_pe_dcm_cb_fn.set_default			= NULL;
			_g_pe_dcm_cb_fn.set_debug			= PE_DCM_HW_H14_SetDebugSettings;
			_g_pe_dcm_cb_fn.set_dce_config		= PE_DCM_HW_H14_SetDceConfig;
			_g_pe_dcm_cb_fn.get_dce_config		= PE_DCM_HW_H14_GetDceConfig;
			_g_pe_dcm_cb_fn.set_dce_lut			= PE_DCM_HW_H14_SetDceLut;
			_g_pe_dcm_cb_fn.get_dce_lut			= PE_DCM_HW_H14_GetDceLut;
			_g_pe_dcm_cb_fn.set_dse_lut			= PE_DCM_HW_H14_SetDseLut;
			_g_pe_dcm_cb_fn.get_dse_lut			= PE_DCM_HW_H14_GetDseLut;
			_g_pe_dcm_cb_fn.set_dce_smooth		= PE_DCM_HW_H14_SetDceSmoothCtrl;
			_g_pe_dcm_cb_fn.get_dce_smooth		= PE_DCM_HW_H14_GetDceSmoothCtrl;
			_g_pe_dcm_cb_fn.set_dce_blending	= PE_DCM_HW_H14_SetDceBlendingCtrl;
			_g_pe_dcm_cb_fn.get_dce_blending	= PE_DCM_HW_H14_GetDceBlendingCtrl;
			_g_pe_dcm_cb_fn.set_dce_drc_mode	= NULL;
			_g_pe_dcm_cb_fn.get_dce_drc_mode	= NULL;
		}
		else
		#endif
		#ifdef USE_PE_KDRV_CODES_FOR_H13
		if (PE_KDRV_VER_H13)
		{
			_g_pe_dcm_cb_fn.hw_init				= PE_DCM_HW_H13_Init;
			_g_pe_dcm_cb_fn.set_default			= NULL;
			_g_pe_dcm_cb_fn.set_debug			= PE_DCM_HW_H13_SetDebugSettings;
			_g_pe_dcm_cb_fn.set_dce_config		= PE_DCM_HW_H13_SetDceConfig;
			_g_pe_dcm_cb_fn.get_dce_config		= PE_DCM_HW_H13_GetDceConfig;
			_g_pe_dcm_cb_fn.set_dce_lut			= PE_DCM_HW_H13_SetDceLut;
			_g_pe_dcm_cb_fn.get_dce_lut			= PE_DCM_HW_H13_GetDceLut;
			_g_pe_dcm_cb_fn.set_dse_lut			= PE_DCM_HW_H13_SetDseLut;
			_g_pe_dcm_cb_fn.get_dse_lut			= PE_DCM_HW_H13_GetDseLut;
			_g_pe_dcm_cb_fn.set_dce_smooth		= PE_DCM_HW_H13_SetDceSmoothCtrl;
			_g_pe_dcm_cb_fn.get_dce_smooth		= PE_DCM_HW_H13_GetDceSmoothCtrl;
			_g_pe_dcm_cb_fn.set_dce_blending	= PE_DCM_HW_H13_SetDceBlendingCtrl;
			_g_pe_dcm_cb_fn.get_dce_blending	= PE_DCM_HW_H13_GetDceBlendingCtrl;
			_g_pe_dcm_cb_fn.set_dce_drc_mode	= NULL;
			_g_pe_dcm_cb_fn.get_dce_drc_mode	= NULL;
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
int PE_DCM_Init(PE_CFG_CTRL_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(_g_pe_dcm_cb_fn.hw_init)
		{
			ret = _g_pe_dcm_cb_fn.hw_init(pstParams);
			PE_CHECK_CODE(ret, break, "hw_init() error.\n");
		}
	}while (0);
	return ret;
}

/**
 * standard function of dcm setting
 *
 * @param   data [in/out] unsigned int
 * @param   functype [in] unsigned int
 * @param   datasize [in] unsigned int
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_DCM_SetFunction(unsigned int *data, unsigned int functype, unsigned int datasize)
{
	int ret = RET_ERROR;

#define _VOID___INPUT_		1
#define _STRUCT_INPUT_		0

#define PE_DCM_SET_TABL(_struct, _func)	\
	case PE_ITEM_PKTFUNC(_struct):\
	{\
		_struct *p_rcv, param;\
		UINT32 *p_data = NULL;\
		UINT32 bufsize;\
		do{\
			PE_DCM_DBG_PRINT("S %s\n", #_struct);\
			PE_CHECK_CODE(datasize!=sizeof(_struct), break, \
				"[%s,%d] size mismatch.\n", __F__, __L__);\
			p_rcv = (_struct *)data;\
			bufsize = GET_BITS(p_rcv->size, 0, 16) * sizeof(UINT32);\
			PE_CHECK_CODE(!bufsize, break, \
				"[%s,%d] bufsize zero.\n", __F__, __L__);\
			p_data = OS_Malloc(bufsize);\
			PE_CHECK_CODE(!p_data, break, \
				"[%s,%d] p_data is null.\n", __F__, __L__);\
			ret = copy_from_user(p_data, (void __user *)p_rcv->data, bufsize);\
			PE_CHECK_CODE(ret, break, \
				"[%s,%d] copy_from_user error.\n", __F__, __L__);\
			param.win_id = p_rcv->win_id;\
			param.size = p_rcv->size;\
			param.data = p_data;\
			ret = _func(&param);\
			PE_CHECK_CODE(ret, break, \
				"[%s,%d] %s() error.\n", __F__, __L__, #_func);\
			PE_DCM_DBG_PRINT("E %s\n", #_struct);\
		}while (0);\
		if (p_data){OS_Free(p_data);}\
	}\
	break

#define PE_DCM_SET_FUNC(_struct, _func, _void_input)	\
	case PE_ITEM_PKTFUNC(_struct):\
	{\
		_struct *p_rcv, param;\
		do{\
			PE_DCM_DBG_PRINT("S %s\n", #_struct);\
			PE_CHECK_CODE(datasize!=sizeof(_struct), break, \
				"[%s,%d] size mismatch.\n", __F__, __L__);\
			p_rcv = (_struct *)data;\
			memcpy(&param, p_rcv, datasize);\
			ret = (_void_input)? (_func((void *)&param)):(_func(&param));\
			PE_CHECK_CODE(ret, break, \
				"[%s,%d] %s() error.\n", __F__, __L__, #_func);\
			PE_DCM_DBG_PRINT("E %s\n", #_struct);\
		}while (0);\
	}\
	break

/** set func without start,
*   end print(WOPT:WithOut PRint) because of too many prints */
#define PE_DCM_SET_WOPR(_struct, _func)	\
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
		}while (0);\
	}\
	break

	/* data null check */
	PE_CHECK_CODE(!data, return RET_ERROR, \
		"[%s,%d] data is null.\n", __F__, __L__);
	switch (functype)
	{
		PE_DCM_SET_WOPR(LX_PE_DCM_DCE_LUT_T,		PE_DCM_SetDceLut);
		PE_DCM_SET_WOPR(LX_PE_DCM_DSE_LUT_T,		PE_DCM_SetDseLut);
		PE_DCM_SET_FUNC(LX_PE_DCM_DCE_CONF_T,		PE_DCM_SetDceConfig,		_STRUCT_INPUT_);
		PE_DCM_SET_FUNC(LX_PE_DCM_BLENDING_T,		PE_DCM_SetDceBlendingCtrl,	_STRUCT_INPUT_);
		PE_DCM_SET_FUNC(LX_PE_DCM_DRC_MODE_T,		PE_DCM_SetDceDrcMode,		_STRUCT_INPUT_);
		PE_DCM_SET_FUNC(LX_PE_DEFAULT_SETTINGS_T,	PE_DCM_SetDefaultSettings,	_STRUCT_INPUT_);
		PE_DCM_SET_FUNC(LX_PE_DBG_SETTINGS_T,		PE_DCM_SetDebugSettings,	_STRUCT_INPUT_);
		PE_DCM_SET_FUNC(LX_PE_DCM_DCE_SMOOTH0_T,	PE_DCM_SetDceSmoothCtrl,	_VOID___INPUT_);
		PE_DCM_SET_FUNC(LX_PE_DCM_DCE_SMOOTH1_T,	PE_DCM_SetDceSmoothCtrl,	_VOID___INPUT_);

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
 * standard function of dcm getting
 *
 * @param   data [in/out] unsigned int
 * @param   functype [in] unsigned int
 * @param   datasize [in] unsigned int
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_DCM_GetFunction(unsigned int *data, unsigned int functype, unsigned int datasize)
{
	int ret = RET_ERROR;

#define _VOID___INPUT_		1
#define _STRUCT_INPUT_		0

#define PE_DCM_GET_TABL(_struct, _func)	\
	case PE_ITEM_PKTFUNC(_struct):\
	{\
		_struct *p_rcv, param;\
		UINT32 *p_data = NULL;\
		UINT32 bufsize;\
		memset0(param);\
		do{\
			PE_DCM_DBG_PRINT("S %s\n", #_struct);\
			PE_CHECK_CODE(datasize!=sizeof(_struct), break, \
				"[%s,%d] size mismatch.\n", __F__, __L__);\
			p_rcv = (_struct *)data;\
			bufsize = GET_BITS(p_rcv->size, 0, 16) * sizeof(UINT32);\
			PE_CHECK_CODE(!bufsize, break, \
				"[%s,%d] bufsize zero.\n", __F__, __L__);\
			p_data = OS_Malloc(bufsize);\
			PE_CHECK_CODE(!p_data, break, \
				"[%s,%d] p_data is null.\n", __F__, __L__);\
			param.win_id = p_rcv->win_id;\
			param.size = p_rcv->size;\
			param.data = p_data;\
			ret = _func(&param);\
			PE_CHECK_CODE(ret, break, \
				"[%s,%d] %s() error.\n", __F__, __L__, #_func);\
			ret = copy_to_user((void __user *)p_rcv->data, p_data, bufsize);\
			PE_CHECK_CODE(ret, break, \
				"[%s,%d] copy_to_user error.\n", __F__, __L__);\
			PE_DCM_DBG_PRINT("E %s\n", #_struct);\
		}while (0);\
		if (p_data){OS_Free(p_data);}\
	}\
	break

#define PE_DCM_GET_FUNC(_struct, _func, _void_input)	\
	case PE_ITEM_PKTFUNC(_struct):\
	{\
		_struct *p_rcv, param;\
		do{\
			PE_DCM_DBG_PRINT("S %s\n", #_struct);\
			PE_CHECK_CODE(datasize!=sizeof(_struct), break, \
				"[%s,%d] size mismatch.\n", __F__, __L__);\
			p_rcv = (_struct *)data;\
			memcpy(&param, p_rcv, datasize);\
			ret = (_void_input)? (_func((void *)&param)):(_func(&param));\
			PE_CHECK_CODE(ret, break, \
				"[%s,%d] %s() error.\n", __F__, __L__, #_func);\
			memcpy(p_rcv, &param, datasize);\
			PE_DCM_DBG_PRINT("E %s\n", #_struct);\
		}while (0);\
	}\
	break

/** get func without start,
*   end print(WOPT:WithOut PRint) because of too many prints */
#define PE_DCM_GET_WOPR(_struct, _func)	\
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
		PE_DCM_GET_TABL(LX_PE_DCM_DCE_LUT_T,		PE_DCM_GetDceLut);
		PE_DCM_GET_TABL(LX_PE_DCM_DSE_LUT_T,		PE_DCM_GetDseLut);
		PE_DCM_GET_FUNC(LX_PE_DCM_DCE_CONF_T,		PE_DCM_GetDceConfig,		_STRUCT_INPUT_);
		PE_DCM_GET_FUNC(LX_PE_DCM_BLENDING_T,		PE_DCM_GetDceBlendingCtrl,	_STRUCT_INPUT_);
		PE_DCM_GET_FUNC(LX_PE_DCM_DRC_MODE_T,		PE_DCM_GetDceDrcMode,		_STRUCT_INPUT_);
		PE_DCM_GET_FUNC(LX_PE_DCM_DCE_SMOOTH0_T,	PE_DCM_GetDceSmoothCtrl,	_VOID___INPUT_);
		PE_DCM_GET_FUNC(LX_PE_DCM_DCE_SMOOTH1_T,	PE_DCM_GetDceSmoothCtrl,	_VOID___INPUT_);
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
static int PE_DCM_SetDefaultSettings(LX_PE_DEFAULT_SETTINGS_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		PE_DCM_DBG_PRINT("[win id:%d]\n", pstParams->win_id);
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
static int PE_DCM_SetDebugSettings(LX_PE_DBG_SETTINGS_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		/* set print level */
		if (pstParams->type&LX_PE_DBG_LV)
		{
			_g_dcm_trace = (pstParams->print_lvl & \
				(0x1<<PE_ITEM_PKTMODL(DCM)))? 0x1:0x0;
		}
		if(_g_pe_dcm_cb_fn.set_debug)
		{
			ret = _g_pe_dcm_cb_fn.set_debug(pstParams);
			PE_CHECK_CODE(ret, break, "set_debug() error.\n");
		}
	}while (0);
	return ret;
}
/**
 * set dce config
 *
 * @param   *pstParams [in] LX_PE_DCM_DCE_CONF_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_DCM_SetDceConfig(LX_PE_DCM_DCE_CONF_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(_g_pe_dcm_cb_fn.set_dce_config)
		{
			ret = _g_pe_dcm_cb_fn.set_dce_config(pstParams);
			PE_CHECK_CODE(ret, break, "set_dce_config() error.\n");
		}
	}while (0);
	return ret;
}
/**
 * get dce config
 *
 * @param   *pstParams [in/out] LX_PE_DCM_DCE_CONF_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_DCM_GetDceConfig(LX_PE_DCM_DCE_CONF_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(_g_pe_dcm_cb_fn.get_dce_config)
		{
			ret = _g_pe_dcm_cb_fn.get_dce_config(pstParams);
			PE_CHECK_CODE(ret, break, "get_dce_config() error.\n");
		}
	}while (0);
	return ret;
}
/**
 * set dce lut
 *
 * @param   *pstParams [in] LX_PE_DCM_DCE_LUT_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_DCM_SetDceLut(LX_PE_DCM_DCE_LUT_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(_g_pe_dcm_cb_fn.set_dce_lut)
		{
			ret = _g_pe_dcm_cb_fn.set_dce_lut(pstParams);
			PE_CHECK_CODE(ret, break, "set_dce_lut() error.\n");
		}
	}while (0);	
	return ret;
}
/**
 * get dce lut
 *
 * @param   *pstParams [in/out] LX_PE_DCM_DCE_LUT_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_DCM_GetDceLut(LX_PE_DCM_DCE_LUT_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(_g_pe_dcm_cb_fn.get_dce_lut)
		{
			ret = _g_pe_dcm_cb_fn.get_dce_lut(pstParams);
			PE_CHECK_CODE(ret, break, "get_dce_lut() error.\n");
		}
	}while (0);	
	return ret;
}
/**
 * set dse lut
 *
 * @param   *pstParams [in] LX_PE_DCM_DSE_LUT_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_DCM_SetDseLut(LX_PE_DCM_DSE_LUT_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(_g_pe_dcm_cb_fn.set_dse_lut)
		{
			ret = _g_pe_dcm_cb_fn.set_dse_lut(pstParams);
			PE_CHECK_CODE(ret, break, "set_dse_lut() error.\n");
		}
	}while (0);	
	return ret;
}
/**
 * get dse lut
 *
 * @param   *pstParams [in/out] LX_PE_DCM_DSE_LUT_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_DCM_GetDseLut(LX_PE_DCM_DSE_LUT_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(_g_pe_dcm_cb_fn.get_dse_lut)
		{
			ret = _g_pe_dcm_cb_fn.get_dse_lut(pstParams);
			PE_CHECK_CODE(ret, break, "get_dse_lut() error.\n");
		}
	}while (0);	
	return ret;
}
/**
 * set dce blur
 * - use input struct LX_PE_DCM_DCE_SMOOTH1_T for H14A,M14B
 * - use input struct LX_PE_DCM_DCE_SMOOTH0_T for H13B,M14A
 *
 * @param   *pstParams [in] LX_PE_DCM_DCE_SMOOTH0_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_DCM_SetDceSmoothCtrl(void *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(_g_pe_dcm_cb_fn.set_dce_smooth)
		{
			ret = _g_pe_dcm_cb_fn.set_dce_smooth(pstParams);
			PE_CHECK_CODE(ret, break, "set_dce_smooth() error.\n");
		}
	}while (0);
	return ret;
}
/**
 * get dce blur
 * - use input struct LX_PE_DCM_DCE_SMOOTH1_T for H14A,M14B
 * - use input struct LX_PE_DCM_DCE_SMOOTH0_T for H13B,M14A
 *
 * @param   *pstParams [in] LX_PE_DCM_DCE_SMOOTH0_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_DCM_GetDceSmoothCtrl(void *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(_g_pe_dcm_cb_fn.get_dce_smooth)
		{
			ret = _g_pe_dcm_cb_fn.get_dce_smooth(pstParams);
			PE_CHECK_CODE(ret, break, "get_dce_smooth() error.\n");
		}
	}while (0);	
	return ret;
}

/**
 * set dce blending control
 *
 * @param   *pstParams [in] LX_PE_DCM_BLENDING_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_DCM_SetDceBlendingCtrl(LX_PE_DCM_BLENDING_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(_g_pe_dcm_cb_fn.set_dce_blending)
		{
			ret = _g_pe_dcm_cb_fn.set_dce_blending(pstParams);
			PE_CHECK_CODE(ret, break, "set_dce_blending() error.\n");
		}
	}while (0);
	return ret;
}
/**
 * get dce blending control
 *
 * @param   *pstParams [in/out] LX_PE_DCM_BLENDING_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_DCM_GetDceBlendingCtrl(LX_PE_DCM_BLENDING_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(_g_pe_dcm_cb_fn.get_dce_blending)
		{
			ret = _g_pe_dcm_cb_fn.get_dce_blending(pstParams);
			PE_CHECK_CODE(ret, break, "get_dce_blending() error.\n");
		}
	}while (0);
	return ret;
}

/**
 * set dce drc mode
 *
 * @param   *pstParams [in] LX_PE_DCM_DRC_MODE_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_DCM_SetDceDrcMode(LX_PE_DCM_DRC_MODE_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(_g_pe_dcm_cb_fn.set_dce_drc_mode)
		{
			ret = _g_pe_dcm_cb_fn.set_dce_drc_mode(pstParams);
			PE_CHECK_CODE(ret, break, "set_dce_drc_mode() error.\n");
		}
	}while (0);
	return ret;
}
/**
 * get dce drc mode
 *
 * @param   *pstParams [in/out] LX_PE_DCM_DRC_MODE_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_DCM_GetDceDrcMode(LX_PE_DCM_DRC_MODE_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(_g_pe_dcm_cb_fn.get_dce_drc_mode)
		{
			ret = _g_pe_dcm_cb_fn.get_dce_drc_mode(pstParams);
			PE_CHECK_CODE(ret, break, "get_dce_drc_mode() error.\n");
		}
	}while (0);
	return ret;
}
