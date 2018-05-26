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

/** @file pe_cmg.c
 *
 *  driver for picture enhance color management functions.
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
#include "pe_cmg.h"
#include "pe_cmg_hw.h"

/*----------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------*/
#define PE_CMG_DBG_PRINT(fmt, args...)	\
	if (_g_cmg_trace)\
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
	int (*set_cen_enable)(LX_PE_CMG_ENABLE_T *pstParams);
	int (*get_cen_enable)(LX_PE_CMG_ENABLE_T *pstParams);
	int (*set_cen_rgn_enable)(LX_PE_CMG_REGION_ENABLE_T *pstParams);
	int (*get_cen_rgn_enable)(LX_PE_CMG_REGION_ENABLE_T *pstParams);
	int (*set_cen_rgn)(LX_PE_CMG_REGION_T *pstParams);
	int (*get_cen_rgn)(LX_PE_CMG_REGION_T *pstParams);
	int (*set_cen_rgn_ctrl)(LX_PE_CMG_REGION_CTRL_T *pstParams);
	int (*get_cen_rgn_ctrl)(LX_PE_CMG_REGION_CTRL_T *pstParams);
	int (*set_cen_gbl_ctrl)(LX_PE_CMG_GLOBAL_CTRL_T *pstParams);
	int (*get_cen_gbl_ctrl)(LX_PE_CMG_GLOBAL_CTRL_T *pstParams);
	int (*set_cen_color_ctrl)(LX_PE_CMG_COLOR_CTRL_T *pstParams);
	int (*get_cen_color_ctrl)(LX_PE_CMG_COLOR_CTRL_T *pstParams);
	int (*set_cw_ctrl)(LX_PE_CMG_CW_CTRL_T *pstParams);
	int (*get_cw_ctrl)(LX_PE_CMG_CW_CTRL_T *pstParams);
	int (*set_cw_gain_ctrl)(LX_PE_CMG_CW_GAIN_CTRL_T *pstParams);
	int (*get_cw_gain_ctrl)(LX_PE_CMG_CW_GAIN_CTRL_T *pstParams);
}
PE_CMG_CB_FUNC_T;

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
static int PE_CMG_SetDefaultSettings(LX_PE_DEFAULT_SETTINGS_T *pstParams);
static int PE_CMG_SetDebugSettings(LX_PE_DBG_SETTINGS_T *pstParams);
static int PE_CMG_SetCenEnable(LX_PE_CMG_ENABLE_T *pstParams);
static int PE_CMG_GetCenEnable(LX_PE_CMG_ENABLE_T *pstParams);
static int PE_CMG_SetCenRegionEnable(LX_PE_CMG_REGION_ENABLE_T *pstParams);
static int PE_CMG_GetCenRegionEnable(LX_PE_CMG_REGION_ENABLE_T *pstParams);
static int PE_CMG_SetCenRegion(LX_PE_CMG_REGION_T *pstParams);
static int PE_CMG_GetCenRegion(LX_PE_CMG_REGION_T *pstParams);
static int PE_CMG_SetCenRegionCtrl(LX_PE_CMG_REGION_CTRL_T *pstParams);
static int PE_CMG_GetCenRegionCtrl(LX_PE_CMG_REGION_CTRL_T *pstParams);
static int PE_CMG_SetCenGlobalCtrl(LX_PE_CMG_GLOBAL_CTRL_T *pstParams);
static int PE_CMG_GetCenGlobalCtrl(LX_PE_CMG_GLOBAL_CTRL_T *pstParams);
static int PE_CMG_SetCenColorCtrl(LX_PE_CMG_COLOR_CTRL_T *pstParams);
static int PE_CMG_GetCenColorCtrl(LX_PE_CMG_COLOR_CTRL_T *pstParams);
static int PE_CMG_SetClearWhiteCtrl(LX_PE_CMG_CW_CTRL_T *pstParams);
static int PE_CMG_GetClearWhiteCtrl(LX_PE_CMG_CW_CTRL_T *pstParams);
static int PE_CMG_SetClearWhiteGainCtrl(LX_PE_CMG_CW_GAIN_CTRL_T *pstParams);
static int PE_CMG_GetClearWhiteGainCtrl(LX_PE_CMG_CW_GAIN_CTRL_T *pstParams);

/*----------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------*/
static PE_CMG_CB_FUNC_T _g_pe_cmg_cb_fn;
static UINT32 _g_cmg_trace=0x0;	//default should be off.

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
int PE_CMG_RegisterCbFunc(void)
{
	int ret = RET_OK;

	memset(&_g_pe_cmg_cb_fn, 0, sizeof(PE_CMG_CB_FUNC_T));
	do{
		#ifdef USE_PE_KDRV_CODES_FOR_H15
		if (PE_KDRV_VER_H15)
		{
			_g_pe_cmg_cb_fn.hw_init				= PE_CMG_HW_H15_Init;
			_g_pe_cmg_cb_fn.set_default			= NULL;
			_g_pe_cmg_cb_fn.set_debug			= PE_CMG_HW_H15_SetDebugSettings;
			_g_pe_cmg_cb_fn.set_cen_enable		= PE_CMG_HW_H15_SetCenEnable;
			_g_pe_cmg_cb_fn.get_cen_enable		= PE_CMG_HW_H15_GetCenEnable;
			_g_pe_cmg_cb_fn.set_cen_rgn_enable	= PE_CMG_HW_H15_SetCenRegionEnable;
			_g_pe_cmg_cb_fn.get_cen_rgn_enable	= PE_CMG_HW_H15_GetCenRegionEnable;
			_g_pe_cmg_cb_fn.set_cen_rgn			= PE_CMG_HW_H15_SetCenRegion;
			_g_pe_cmg_cb_fn.get_cen_rgn			= PE_CMG_HW_H15_GetCenRegion;
			_g_pe_cmg_cb_fn.set_cen_rgn_ctrl	= PE_CMG_HW_H15_SetCenRegionCtrl;
			_g_pe_cmg_cb_fn.get_cen_rgn_ctrl	= PE_CMG_HW_H15_GetCenRegionCtrl;
			_g_pe_cmg_cb_fn.set_cen_gbl_ctrl	= PE_CMG_HW_H15_SetCenGlobalCtrl;
			_g_pe_cmg_cb_fn.get_cen_gbl_ctrl	= PE_CMG_HW_H15_GetCenGlobalCtrl;
			_g_pe_cmg_cb_fn.set_cen_color_ctrl	= PE_CMG_HW_H15_SetCenColorCtrl;
			_g_pe_cmg_cb_fn.get_cen_color_ctrl	= PE_CMG_HW_H15_GetCenColorCtrl;
			_g_pe_cmg_cb_fn.set_cw_ctrl			= PE_CMG_HW_H15_SetClearWhiteCtrl;
			_g_pe_cmg_cb_fn.get_cw_ctrl			= PE_CMG_HW_H15_GetClearWhiteCtrl;
			_g_pe_cmg_cb_fn.set_cw_gain_ctrl	= PE_CMG_HW_H15_SetClearWhiteGainCtrl;
			_g_pe_cmg_cb_fn.get_cw_gain_ctrl	= PE_CMG_HW_H15_GetClearWhiteGainCtrl;
		}
		else
		#endif
		#ifdef USE_PE_KDRV_CODES_FOR_M14
		if (PE_KDRV_VER_M14)
		{
			_g_pe_cmg_cb_fn.hw_init				= PE_CMG_HW_M14_Init;
			_g_pe_cmg_cb_fn.set_default			= NULL;
			_g_pe_cmg_cb_fn.set_debug			= PE_CMG_HW_M14_SetDebugSettings;
			_g_pe_cmg_cb_fn.set_cen_enable		= PE_CMG_HW_M14_SetCenEnable;
			_g_pe_cmg_cb_fn.get_cen_enable		= PE_CMG_HW_M14_GetCenEnable;
			_g_pe_cmg_cb_fn.set_cen_rgn_enable	= PE_CMG_HW_M14_SetCenRegionEnable;
			_g_pe_cmg_cb_fn.get_cen_rgn_enable	= PE_CMG_HW_M14_GetCenRegionEnable;
			_g_pe_cmg_cb_fn.set_cen_rgn			= PE_CMG_HW_M14_SetCenRegion;
			_g_pe_cmg_cb_fn.get_cen_rgn			= PE_CMG_HW_M14_GetCenRegion;
			_g_pe_cmg_cb_fn.set_cen_rgn_ctrl	= PE_CMG_HW_M14_SetCenRegionCtrl;
			_g_pe_cmg_cb_fn.get_cen_rgn_ctrl	= PE_CMG_HW_M14_GetCenRegionCtrl;
			_g_pe_cmg_cb_fn.set_cen_gbl_ctrl	= PE_CMG_HW_M14_SetCenGlobalCtrl;
			_g_pe_cmg_cb_fn.get_cen_gbl_ctrl	= PE_CMG_HW_M14_GetCenGlobalCtrl;
			_g_pe_cmg_cb_fn.set_cen_color_ctrl	= PE_CMG_HW_M14_SetCenColorCtrl;
			_g_pe_cmg_cb_fn.get_cen_color_ctrl	= PE_CMG_HW_M14_GetCenColorCtrl;
			_g_pe_cmg_cb_fn.set_cw_ctrl			= PE_CMG_HW_M14_SetClearWhiteCtrl;
			_g_pe_cmg_cb_fn.get_cw_ctrl			= PE_CMG_HW_M14_GetClearWhiteCtrl;
			_g_pe_cmg_cb_fn.set_cw_gain_ctrl	= PE_CMG_HW_M14_SetClearWhiteGainCtrl;
			_g_pe_cmg_cb_fn.get_cw_gain_ctrl	= PE_CMG_HW_M14_GetClearWhiteGainCtrl;
		}
		else
		#endif
		#ifdef USE_PE_KDRV_CODES_FOR_H14
		if (PE_KDRV_VER_H14)
		{
			_g_pe_cmg_cb_fn.hw_init				= PE_CMG_HW_H14_Init;
			_g_pe_cmg_cb_fn.set_default			= NULL;
			_g_pe_cmg_cb_fn.set_debug			= PE_CMG_HW_H14_SetDebugSettings;
			_g_pe_cmg_cb_fn.set_cen_enable		= PE_CMG_HW_H14_SetCenEnable;
			_g_pe_cmg_cb_fn.get_cen_enable		= PE_CMG_HW_H14_GetCenEnable;
			_g_pe_cmg_cb_fn.set_cen_rgn_enable	= PE_CMG_HW_H14_SetCenRegionEnable;
			_g_pe_cmg_cb_fn.get_cen_rgn_enable	= PE_CMG_HW_H14_GetCenRegionEnable;
			_g_pe_cmg_cb_fn.set_cen_rgn			= PE_CMG_HW_H14_SetCenRegion;
			_g_pe_cmg_cb_fn.get_cen_rgn			= PE_CMG_HW_H14_GetCenRegion;
			_g_pe_cmg_cb_fn.set_cen_rgn_ctrl	= PE_CMG_HW_H14_SetCenRegionCtrl;
			_g_pe_cmg_cb_fn.get_cen_rgn_ctrl	= PE_CMG_HW_H14_GetCenRegionCtrl;
			_g_pe_cmg_cb_fn.set_cen_gbl_ctrl	= PE_CMG_HW_H14_SetCenGlobalCtrl;
			_g_pe_cmg_cb_fn.get_cen_gbl_ctrl	= PE_CMG_HW_H14_GetCenGlobalCtrl;
			_g_pe_cmg_cb_fn.set_cen_color_ctrl	= PE_CMG_HW_H14_SetCenColorCtrl;
			_g_pe_cmg_cb_fn.get_cen_color_ctrl	= PE_CMG_HW_H14_GetCenColorCtrl;
			_g_pe_cmg_cb_fn.set_cw_ctrl			= PE_CMG_HW_H14_SetClearWhiteCtrl;
			_g_pe_cmg_cb_fn.get_cw_ctrl			= PE_CMG_HW_H14_GetClearWhiteCtrl;
			_g_pe_cmg_cb_fn.set_cw_gain_ctrl	= PE_CMG_HW_H14_SetClearWhiteGainCtrl;
			_g_pe_cmg_cb_fn.get_cw_gain_ctrl	= PE_CMG_HW_H14_GetClearWhiteGainCtrl;
		}
		else
		#endif
		#ifdef USE_PE_KDRV_CODES_FOR_H13
		if (PE_KDRV_VER_H13)
		{
			_g_pe_cmg_cb_fn.hw_init				= PE_CMG_HW_H13_Init;
			_g_pe_cmg_cb_fn.set_default			= NULL;
			_g_pe_cmg_cb_fn.set_debug			= PE_CMG_HW_H13_SetDebugSettings;
			_g_pe_cmg_cb_fn.set_cen_enable		= PE_CMG_HW_H13_SetCenEnable;
			_g_pe_cmg_cb_fn.get_cen_enable		= PE_CMG_HW_H13_GetCenEnable;
			_g_pe_cmg_cb_fn.set_cen_rgn_enable	= PE_CMG_HW_H13_SetCenRegionEnable;
			_g_pe_cmg_cb_fn.get_cen_rgn_enable	= PE_CMG_HW_H13_GetCenRegionEnable;
			_g_pe_cmg_cb_fn.set_cen_rgn			= PE_CMG_HW_H13_SetCenRegion;
			_g_pe_cmg_cb_fn.get_cen_rgn			= PE_CMG_HW_H13_GetCenRegion;
			_g_pe_cmg_cb_fn.set_cen_rgn_ctrl	= PE_CMG_HW_H13_SetCenRegionCtrl;
			_g_pe_cmg_cb_fn.get_cen_rgn_ctrl	= PE_CMG_HW_H13_GetCenRegionCtrl;
			_g_pe_cmg_cb_fn.set_cen_gbl_ctrl	= PE_CMG_HW_H13_SetCenGlobalCtrl;
			_g_pe_cmg_cb_fn.get_cen_gbl_ctrl	= PE_CMG_HW_H13_GetCenGlobalCtrl;
			_g_pe_cmg_cb_fn.set_cen_color_ctrl	= PE_CMG_HW_H13_SetCenColorCtrl;
			_g_pe_cmg_cb_fn.get_cen_color_ctrl	= PE_CMG_HW_H13_GetCenColorCtrl;
			_g_pe_cmg_cb_fn.set_cw_ctrl			= PE_CMG_HW_H13_SetClearWhiteCtrl;
			_g_pe_cmg_cb_fn.get_cw_ctrl			= PE_CMG_HW_H13_GetClearWhiteCtrl;
			_g_pe_cmg_cb_fn.set_cw_gain_ctrl	= PE_CMG_HW_H13_SetClearWhiteGainCtrl;
			_g_pe_cmg_cb_fn.get_cw_gain_ctrl	= PE_CMG_HW_H13_GetClearWhiteGainCtrl;
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
 * init color management
 *
 * @param   *pstParams [in] PE_CFG_CTRL_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_CMG_Init(PE_CFG_CTRL_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(_g_pe_cmg_cb_fn.hw_init)
		{
			ret = _g_pe_cmg_cb_fn.hw_init(pstParams);
			PE_CHECK_CODE(ret, break, "hw_init() error.\n");
		}
	}while (0);
	return ret;
}

/**
 * standard function of cmg setting
 *
 * @param   data [in/out] unsigned int
 * @param   functype [in/out] unsigned int
 * @param   datasize [in/out] unsigned int
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_CMG_SetFunction(unsigned int *data, unsigned int functype, unsigned int datasize)
{
	int ret = RET_ERROR;

#define PE_CMG_SET_FUNC(_struct, _func)	\
	case PE_ITEM_PKTFUNC(_struct):\
	{\
		_struct *p_rcv, param;\
		do{\
			PE_CMG_DBG_PRINT("S %s\n", #_struct);\
			PE_CHECK_CODE(datasize!=sizeof(_struct), break, \
				"[%s, %d] size mismatch.\n", __F__, __L__);\
			p_rcv = (_struct *)data;\
			memcpy(&param, p_rcv, datasize);\
			ret = _func(&param);\
			PE_CHECK_CODE(ret, break, \
				"[%s, %d] %s() error.\n", __F__, __L__, #_func);\
			PE_CMG_DBG_PRINT("E %s\n", #_struct);\
		}while (0);\
	}\
	break

	/* data null check */
	PE_CHECK_CODE(!data, return RET_ERROR, \
		"[%s, %d] data is null.\n", __F__, __L__);
	switch (functype)
	{
		PE_CMG_SET_FUNC(LX_PE_CMG_COLOR_CTRL_T,		PE_CMG_SetCenColorCtrl);
		PE_CMG_SET_FUNC(LX_PE_CMG_ENABLE_T,			PE_CMG_SetCenEnable);
		PE_CMG_SET_FUNC(LX_PE_CMG_REGION_ENABLE_T,	PE_CMG_SetCenRegionEnable);
		PE_CMG_SET_FUNC(LX_PE_CMG_REGION_T,			PE_CMG_SetCenRegion);
		PE_CMG_SET_FUNC(LX_PE_CMG_REGION_CTRL_T,	PE_CMG_SetCenRegionCtrl);
		PE_CMG_SET_FUNC(LX_PE_CMG_GLOBAL_CTRL_T,	PE_CMG_SetCenGlobalCtrl);
		PE_CMG_SET_FUNC(LX_PE_DEFAULT_SETTINGS_T,	PE_CMG_SetDefaultSettings);
		PE_CMG_SET_FUNC(LX_PE_DBG_SETTINGS_T,		PE_CMG_SetDebugSettings);
		PE_CMG_SET_FUNC(LX_PE_CMG_CW_CTRL_T,		PE_CMG_SetClearWhiteCtrl);
		PE_CMG_SET_FUNC(LX_PE_CMG_CW_GAIN_CTRL_T,	PE_CMG_SetClearWhiteGainCtrl);
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
 * standard function of cmg getting
 *
 * @param   data [in/out] unsigned int
 * @param   functype [in/out] unsigned int
 * @param   datasize [in/out] unsigned int
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_CMG_GetFunction(unsigned int *data, unsigned int functype, unsigned int datasize)
{
	int ret = RET_ERROR;

#define PE_CMG_GET_FUNC(_struct, _func)	\
	case PE_ITEM_PKTFUNC(_struct):\
	{\
		_struct *p_rcv, param;\
		do{\
			PE_CMG_DBG_PRINT("S %s\n", #_struct);\
			PE_CHECK_CODE(datasize!=sizeof(_struct), break, \
				"[%s, %d] size mismatch.\n", __F__, __L__);\
			p_rcv = (_struct *)data;\
			memcpy(&param, p_rcv, datasize);\
			ret = _func(&param);\
			PE_CHECK_CODE(ret, break, \
				"[%s, %d] %s() error.\n", __F__, __L__, #_func);\
			memcpy(p_rcv, &param, datasize);\
			PE_CMG_DBG_PRINT("E %s\n", #_struct);\
		}while (0);\
	}\
	break

	/* data null check */
	PE_CHECK_CODE(!data, return RET_ERROR, \
		"[%s, %d] data is null.\n", __F__, __L__);
	switch (functype)
	{
		PE_CMG_GET_FUNC(LX_PE_CMG_COLOR_CTRL_T,		PE_CMG_GetCenColorCtrl);
		PE_CMG_GET_FUNC(LX_PE_CMG_ENABLE_T,			PE_CMG_GetCenEnable);
		PE_CMG_GET_FUNC(LX_PE_CMG_REGION_ENABLE_T,	PE_CMG_GetCenRegionEnable);
		PE_CMG_GET_FUNC(LX_PE_CMG_REGION_T,			PE_CMG_GetCenRegion);
		PE_CMG_GET_FUNC(LX_PE_CMG_REGION_CTRL_T,	PE_CMG_GetCenRegionCtrl);
		PE_CMG_GET_FUNC(LX_PE_CMG_GLOBAL_CTRL_T,	PE_CMG_GetCenGlobalCtrl);
		PE_CMG_GET_FUNC(LX_PE_CMG_CW_CTRL_T,		PE_CMG_GetClearWhiteCtrl);
		PE_CMG_GET_FUNC(LX_PE_CMG_CW_GAIN_CTRL_T,	PE_CMG_GetClearWhiteGainCtrl);
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
static int PE_CMG_SetDefaultSettings(LX_PE_DEFAULT_SETTINGS_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		PE_CMG_DBG_PRINT("[win id:%d]\n", pstParams->win_id);
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
static int PE_CMG_SetDebugSettings(LX_PE_DBG_SETTINGS_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		/* set print level */
		if (pstParams->type&LX_PE_DBG_LV)
		{
			_g_cmg_trace = (pstParams->print_lvl & \
				(0x1<<PE_ITEM_PKTMODL(CMG)))? 0x1:0x0;
		}
		if(_g_pe_cmg_cb_fn.set_debug)
		{
			ret = _g_pe_cmg_cb_fn.set_debug(pstParams);
			PE_CHECK_CODE(ret, break, "set_debug() error.\n");
		}
	}while (0);
	return ret;
}
/**
 * set cen enable
 *
 * @param   *pstParams [in] LX_PE_CMG_ENABLE_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_CMG_SetCenEnable(LX_PE_CMG_ENABLE_T *pstParams)
{
	int ret = RET_OK;
	do {
		CHECK_KNULL(pstParams);
		if(_g_pe_cmg_cb_fn.set_cen_enable)
		{
			ret = _g_pe_cmg_cb_fn.set_cen_enable(pstParams);
			PE_CHECK_CODE(ret, break, "set_cen_enable() error.\n");
		}
	}while (0);
	return ret;
}
/**
 * get cen enable
 *
 * @param   *pstParams [in/out] LX_PE_CMG_ENABLE_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_CMG_GetCenEnable(LX_PE_CMG_ENABLE_T *pstParams)
{
	int ret = RET_OK;
	do {
		CHECK_KNULL(pstParams);
		if(_g_pe_cmg_cb_fn.get_cen_enable)
		{
			ret = _g_pe_cmg_cb_fn.get_cen_enable(pstParams);
			PE_CHECK_CODE(ret, break, "get_cen_enable() error.\n");
		}
	}while (0);
	return ret;
}
/**
 * set cen region enable
 *
 * @param   *pstParams [in] LX_PE_CMG_REGION_ENABLE_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_CMG_SetCenRegionEnable(LX_PE_CMG_REGION_ENABLE_T *pstParams)
{
	int ret = RET_OK;
	do {
		CHECK_KNULL(pstParams);
		if(_g_pe_cmg_cb_fn.set_cen_rgn_enable)
		{
			ret = _g_pe_cmg_cb_fn.set_cen_rgn_enable(pstParams);
			PE_CHECK_CODE(ret, break, "set_cen_rgn_enable() error.\n");
		}
	}while (0);
	return ret;
}
/**
 * get cen region enable
 *
 * @param   *pstParams [in/out] LX_PE_CMG_REGION_ENABLE_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_CMG_GetCenRegionEnable(LX_PE_CMG_REGION_ENABLE_T *pstParams)
{
	int ret = RET_OK;
	do {
		CHECK_KNULL(pstParams);
		if(_g_pe_cmg_cb_fn.get_cen_rgn_enable)
		{
			ret = _g_pe_cmg_cb_fn.get_cen_rgn_enable(pstParams);
			PE_CHECK_CODE(ret, break, "get_cen_rgn_enable() error.\n");
		}
	}while (0);
	return ret;
}
/**
 * set cen region
 *
 * @param   *pstParams [in] LX_PE_CMG_REGION_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_CMG_SetCenRegion(LX_PE_CMG_REGION_T *pstParams)
{
	int ret = RET_OK;
	do {
		CHECK_KNULL(pstParams);
		if(_g_pe_cmg_cb_fn.set_cen_rgn)
		{
			ret = _g_pe_cmg_cb_fn.set_cen_rgn(pstParams);
			PE_CHECK_CODE(ret, break, "set_cen_rgn() error.\n");
		}
	}while (0);
	return ret;
}
/**
 * get cen region
 *
 * @param   *pstParams [in/out] LX_PE_CMG_REGION_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_CMG_GetCenRegion(LX_PE_CMG_REGION_T *pstParams)
{
	int ret = RET_OK;
	do {
		CHECK_KNULL(pstParams);
		if(_g_pe_cmg_cb_fn.get_cen_rgn)
		{
			ret = _g_pe_cmg_cb_fn.get_cen_rgn(pstParams);
			PE_CHECK_CODE(ret, break, "get_cen_rgn() error.\n");
		}
	}while (0);
	return ret;
}
/**
 * set cen region ctrl
 *
 * @param   *pstParams [in] LX_PE_CMG_REGION_CTRL_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_CMG_SetCenRegionCtrl(LX_PE_CMG_REGION_CTRL_T *pstParams)
{
	int ret = RET_OK;
	do {
		CHECK_KNULL(pstParams);
		if(_g_pe_cmg_cb_fn.set_cen_rgn_ctrl)
		{
			ret = _g_pe_cmg_cb_fn.set_cen_rgn_ctrl(pstParams);
			PE_CHECK_CODE(ret, break, "set_cen_rgn_ctrl() error.\n");
		}
	}while (0);
	return ret;
}
/**
 * get cen region ctrl
 *
 * @param   *pstParams [in/out] LX_PE_CMG_REGION_CTRL_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_CMG_GetCenRegionCtrl(LX_PE_CMG_REGION_CTRL_T *pstParams)
{
	int ret = RET_OK;
	do {
		CHECK_KNULL(pstParams);
		if(_g_pe_cmg_cb_fn.get_cen_rgn_ctrl)
		{
			ret = _g_pe_cmg_cb_fn.get_cen_rgn_ctrl(pstParams);
			PE_CHECK_CODE(ret, break, "get_cen_rgn_ctrl() error.\n");
		}
	}while (0);
	return ret;
}
/**
 * set cen global ctrl
 *
 * @param   *pstParams [in] LX_PE_CMG_GLOBAL_CTRL_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_CMG_SetCenGlobalCtrl(LX_PE_CMG_GLOBAL_CTRL_T *pstParams)
{
	int ret = RET_OK;
	do {
		CHECK_KNULL(pstParams);
		if(_g_pe_cmg_cb_fn.set_cen_gbl_ctrl)
		{
			ret = _g_pe_cmg_cb_fn.set_cen_gbl_ctrl(pstParams);
			PE_CHECK_CODE(ret, break, "set_cen_gbl_ctrl() error.\n");
		}
	}while (0);
	return ret;
}
/**
 * get cen global ctrl
 *
 * @param   *pstParams [in/out] LX_PE_CMG_GLOBAL_CTRL_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_CMG_GetCenGlobalCtrl(LX_PE_CMG_GLOBAL_CTRL_T *pstParams)
{
	int ret = RET_OK;
	do {
		CHECK_KNULL(pstParams);
		if(_g_pe_cmg_cb_fn.get_cen_gbl_ctrl)
		{
			ret = _g_pe_cmg_cb_fn.get_cen_gbl_ctrl(pstParams);
			PE_CHECK_CODE(ret, break, "get_cen_gbl_ctrl() error.\n");
		}
	}while (0);
	return ret;
}
/**
 * set cen color ctrl
 *
 * @param   *pstParams [in] LX_PE_CMG_COLOR_CTRL_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_CMG_SetCenColorCtrl(LX_PE_CMG_COLOR_CTRL_T *pstParams)
{
	int ret = RET_OK;
	do {
		CHECK_KNULL(pstParams);
		if(_g_pe_cmg_cb_fn.set_cen_color_ctrl)
		{
			ret = _g_pe_cmg_cb_fn.set_cen_color_ctrl(pstParams);
			PE_CHECK_CODE(ret, break, "set_cen_color_ctrl() error.\n");
		}
	}while (0);
	return ret;
}
/**
 * get cen color ctrl
 *
 * @param   *pstParams [in/out] LX_PE_CMG_COLOR_CTRL_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_CMG_GetCenColorCtrl(LX_PE_CMG_COLOR_CTRL_T *pstParams)
{
	int ret = RET_OK;
	do {
		CHECK_KNULL(pstParams);
		if(_g_pe_cmg_cb_fn.get_cen_color_ctrl)
		{
			ret = _g_pe_cmg_cb_fn.get_cen_color_ctrl(pstParams);
			PE_CHECK_CODE(ret, break, "get_cen_color_ctrl() error.\n");
		}
	}while (0);
	return ret;
}
/**
 * set clear white
 *
 * @param   *pstParams [in] LX_PE_CMG_CW_CTRL_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_CMG_SetClearWhiteCtrl(LX_PE_CMG_CW_CTRL_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(_g_pe_cmg_cb_fn.set_cw_ctrl)
		{
			ret = _g_pe_cmg_cb_fn.set_cw_ctrl(pstParams);
			PE_CHECK_CODE(ret, break, "set_cw_ctrl() error.\n");
		}
	}while (0);
	return ret;
}
/**
 * get clear white
 *
 * @param   *pstParams [in/out] LX_PE_CMG_CW_CTRL_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_CMG_GetClearWhiteCtrl(LX_PE_CMG_CW_CTRL_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(_g_pe_cmg_cb_fn.get_cw_ctrl)
		{
			ret = _g_pe_cmg_cb_fn.get_cw_ctrl(pstParams);
			PE_CHECK_CODE(ret, break, "get_cw_ctrl() error.\n");
		}
	}while (0);
	return ret;
}
/**
 * set clear white gain
 *
 * @param   *pstParams [in] LX_PE_CMG_CW_GAIN_CTRL_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_CMG_SetClearWhiteGainCtrl(LX_PE_CMG_CW_GAIN_CTRL_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(_g_pe_cmg_cb_fn.set_cw_gain_ctrl)
		{
			ret = _g_pe_cmg_cb_fn.set_cw_gain_ctrl(pstParams);
			PE_CHECK_CODE(ret, break, "set_cw_gain_ctrl() error.\n");
		}
	}while (0);
	return ret;
}
/**
 * get clear white gain
 *
 * @param   *pstParams [in/out] LX_PE_CMG_CW_GAIN_CTRL_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_CMG_GetClearWhiteGainCtrl(LX_PE_CMG_CW_GAIN_CTRL_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(_g_pe_cmg_cb_fn.get_cw_gain_ctrl)
		{
			ret = _g_pe_cmg_cb_fn.get_cw_gain_ctrl(pstParams);
			PE_CHECK_CODE(ret, break, "get_cw_gain_ctrl() error.\n");
		}
	}while (0);
	return ret;
}

