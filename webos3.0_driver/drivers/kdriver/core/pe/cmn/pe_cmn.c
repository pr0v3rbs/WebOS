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

/** @file pe_cmn.c
 *
 *  driver for picture enhance common functions.
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
#include "pe_spd.h"
#include "pe_cmn.h"
#include "pe_etc_hw.h"
#include "pe_cmn_hw.h"

/*----------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------*/
#define PE_CMN_DBG_PRINT(fmt, args...)	\
	if (_g_cmn_trace)\
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
	int (*ipc_open)(void);
	int (*ipc_close)(void);
	int (*create_pic_task)(void);
	int (*stop_pic_task)(void);
	int (*fwi_init)(PE_CFG_CTRL_T *pstParams);
	int (*set_default)(LX_PE_DEFAULT_SETTINGS_T *pstParams);
	int (*get_default)(LX_PE_DEFAULT_SETTINGS_T *pstParams);
	int (*set_debug)(LX_PE_DBG_SETTINGS_T *pstParams);
	int (*get_debug)(LX_PE_DBG_SETTINGS_T *pstParams);
	int (*set_contrast)(LX_PE_CMN_CONTRAST_T *pstParams);
	int (*get_contrast)(LX_PE_CMN_CONTRAST_T *pstParams);
	int (*set_brightness)(LX_PE_CMN_BRIGHTNESS_T *pstParams);
	int (*get_brightness)(LX_PE_CMN_BRIGHTNESS_T *pstParams);
	int (*set_saturation)(LX_PE_CMN_SATURATION_T *pstParams);
	int (*get_saturation)(LX_PE_CMN_SATURATION_T *pstParams);
	int (*set_hue)(LX_PE_CMN_HUE_T *pstParams);
	int (*get_hue)(LX_PE_CMN_HUE_T *pstParams);
	int (*set_level_ctrl)(LX_PE_CMN_LEVEL_CTRL_T *pstParams);
	int (*get_level_ctrl)(LX_PE_CMN_LEVEL_CTRL_T *pstParams);
}
PE_CMN_CB_FUNC_T;

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
UINT32 g_pe_kdrv_ver_mask = 0x0;	//default 0x0

/*----------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------*/
static int PE_CMN_SetDefaultSettings(LX_PE_DEFAULT_SETTINGS_T *pstParams);
static int PE_CMN_GetDefaultSettings(LX_PE_DEFAULT_SETTINGS_T *pstParams);
static int PE_CMN_SetDebugSettings(LX_PE_DBG_SETTINGS_T *pstParams);
static int PE_CMN_GetDebugSettings(LX_PE_DBG_SETTINGS_T *pstParams);
static int PE_CMN_SetContrast(LX_PE_CMN_CONTRAST_T *pstParams);
static int PE_CMN_GetContrast(LX_PE_CMN_CONTRAST_T *pstParams);
static int PE_CMN_SetBrightness(LX_PE_CMN_BRIGHTNESS_T *pstParams);
static int PE_CMN_GetBrightness(LX_PE_CMN_BRIGHTNESS_T *pstParams);
static int PE_CMN_SetSaturation(LX_PE_CMN_SATURATION_T *pstParams);
static int PE_CMN_GetSaturation(LX_PE_CMN_SATURATION_T *pstParams);
static int PE_CMN_SetHue(LX_PE_CMN_HUE_T *pstParams);
static int PE_CMN_GetHue(LX_PE_CMN_HUE_T *pstParams);
static int PE_CMN_SetLevelCtrl(LX_PE_CMN_LEVEL_CTRL_T *pstParams);
static int PE_CMN_GetLevelCtrl(LX_PE_CMN_LEVEL_CTRL_T *pstParams);

/*----------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------*/
static PE_CMN_CB_FUNC_T _g_pe_cmn_cb_fn;
static UINT32 _g_cmn_trace=0x0;	//default should be off.
static PE_CMN_SETTINGS_T _g_pe_cmn_info;

/*============================================================================
	Implementation Group
============================================================================*/
/**
 * check chip rev
 *
 * @param   void
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_CMN_CheckChipRev(void)
{
	/************** cfg kdrv ver **************/
	g_pe_kdrv_ver_mask = \
		PE_KDRV_CHK_VER_BIT(LX_CHIP_REV(H13, A0), LX_CHIP_REV(H13, B0), \
		PE_KDRV_VER_BIT_H13AX)|\
		PE_KDRV_CHK_VER_BIT(LX_CHIP_REV(H13, B0), LX_CHIP_REV(H13, C0), \
		PE_KDRV_VER_BIT_H13BX)|\
		PE_KDRV_CHK_VER_BIT(LX_CHIP_REV(H14, A0), LX_CHIP_REV(H14, B0), \
		PE_KDRV_VER_BIT_H14AX)|\
		PE_KDRV_CHK_VER_BIT(LX_CHIP_REV(H14, B0), LX_CHIP_REV(H14, C0), \
		PE_KDRV_VER_BIT_H14BX)|\
		PE_KDRV_CHK_VER_BIT(LX_CHIP_REV(M14, A0), LX_CHIP_REV(M14, B0), \
		PE_KDRV_VER_BIT_M14AX)|\
		PE_KDRV_CHK_VER_BIT(LX_CHIP_REV(M14, B0), LX_CHIP_REV(M14, D0), \
		PE_KDRV_VER_BIT_M14BX)|\
		PE_KDRV_CHK_VER_BIT(LX_CHIP_REV(M14, C0), LX_CHIP_REV(M14, D0), \
		PE_KDRV_VER_BIT_M14CX)|\
		PE_KDRV_CHK_VER_BIT(LX_CHIP_REV(H15, A0), LX_CHIP_REV(H15, B0), \
		PE_KDRV_VER_BIT_H15AX)|\
		PE_KDRV_CHK_VER_BIT(LX_CHIP_REV(H15, B0), LX_CHIP_REV(H15, C0), \
		PE_KDRV_VER_BIT_H15BX);
	/******************************************/
	return RET_OK;
}
/**
 * register call back functions
 *
 * @param   void
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_CMN_RegisterCbFunc(void)
{
	int ret = RET_OK;

	memset(&_g_pe_cmn_cb_fn, 0, sizeof(PE_CMN_CB_FUNC_T));
	do{
		#ifdef USE_PE_KDRV_CODES_FOR_H15
		if (PE_KDRV_VER_H15)
		{
			_g_pe_cmn_cb_fn.hw_init				= PE_CMN_HW_H15_Init;
			_g_pe_cmn_cb_fn.ipc_open			= PE_FWI_H15_IPC_Open;
			_g_pe_cmn_cb_fn.ipc_close			= PE_FWI_H15_IPC_Close;
			_g_pe_cmn_cb_fn.create_pic_task		= PE_TSK_H15_CreatePicTask;
			_g_pe_cmn_cb_fn.stop_pic_task		= PE_TSK_H15_StopPicTask;
			_g_pe_cmn_cb_fn.fwi_init			= PE_FWI_H15_Init;
			_g_pe_cmn_cb_fn.set_default			= NULL;
			_g_pe_cmn_cb_fn.get_default			= NULL;
			_g_pe_cmn_cb_fn.set_debug			= PE_CMN_HW_H15_SetDebugSettings;
			_g_pe_cmn_cb_fn.get_debug			= PE_CMN_HW_H15_GetDebugSettings;
			_g_pe_cmn_cb_fn.set_contrast		= PE_CMN_HW_H15_SetContrast;
			_g_pe_cmn_cb_fn.get_contrast		= PE_CMN_HW_H15_GetContrast;
			_g_pe_cmn_cb_fn.set_brightness		= PE_CMN_HW_H15_SetBrightness;
			_g_pe_cmn_cb_fn.get_brightness		= PE_CMN_HW_H15_GetBrightness;
			_g_pe_cmn_cb_fn.set_saturation		= PE_CMN_HW_H15_SetSaturation;
			_g_pe_cmn_cb_fn.get_saturation		= PE_CMN_HW_H15_GetSaturation;
			_g_pe_cmn_cb_fn.set_hue				= PE_CMN_HW_H15_SetHue;
			_g_pe_cmn_cb_fn.get_hue				= PE_CMN_HW_H15_GetHue;
			_g_pe_cmn_cb_fn.set_level_ctrl		= PE_CMN_HW_H15_SetLevelCtrl;
			_g_pe_cmn_cb_fn.get_level_ctrl		= PE_CMN_HW_H15_GetLevelCtrl;
		}
		else
		#endif
		#ifdef USE_PE_KDRV_CODES_FOR_M14
		if (PE_KDRV_VER_M14)
		{
			_g_pe_cmn_cb_fn.hw_init				= PE_CMN_HW_M14_Init;
			_g_pe_cmn_cb_fn.ipc_open			= PE_FWI_M14_IPC_Open;
			_g_pe_cmn_cb_fn.ipc_close			= PE_FWI_M14_IPC_Close;
			_g_pe_cmn_cb_fn.create_pic_task		= PE_TSK_M14_CreatePicTask;
			_g_pe_cmn_cb_fn.stop_pic_task		= PE_TSK_M14_StopPicTask;
			_g_pe_cmn_cb_fn.fwi_init			= PE_FWI_M14_Init;
			_g_pe_cmn_cb_fn.set_default			= NULL;
			_g_pe_cmn_cb_fn.get_default			= NULL;
			_g_pe_cmn_cb_fn.set_debug			= PE_CMN_HW_M14_SetDebugSettings;
			_g_pe_cmn_cb_fn.get_debug			= PE_CMN_HW_M14_GetDebugSettings;
			_g_pe_cmn_cb_fn.set_contrast		= PE_CMN_HW_M14_SetContrast;
			_g_pe_cmn_cb_fn.get_contrast		= PE_CMN_HW_M14_GetContrast;
			_g_pe_cmn_cb_fn.set_brightness		= PE_CMN_HW_M14_SetBrightness;
			_g_pe_cmn_cb_fn.get_brightness		= PE_CMN_HW_M14_GetBrightness;
			_g_pe_cmn_cb_fn.set_saturation		= PE_CMN_HW_M14_SetSaturation;
			_g_pe_cmn_cb_fn.get_saturation		= PE_CMN_HW_M14_GetSaturation;
			_g_pe_cmn_cb_fn.set_hue				= PE_CMN_HW_M14_SetHue;
			_g_pe_cmn_cb_fn.get_hue				= PE_CMN_HW_M14_GetHue;
			_g_pe_cmn_cb_fn.set_level_ctrl		= PE_CMN_HW_M14_SetLevelCtrl;
			_g_pe_cmn_cb_fn.get_level_ctrl		= PE_CMN_HW_M14_GetLevelCtrl;
		}
		else
		#endif
		#ifdef USE_PE_KDRV_CODES_FOR_H14
		if (PE_KDRV_VER_H14)
		{
			_g_pe_cmn_cb_fn.hw_init				= PE_CMN_HW_H14_Init;
			_g_pe_cmn_cb_fn.ipc_open			= PE_FWI_H14_IPC_Open;
			_g_pe_cmn_cb_fn.ipc_close			= PE_FWI_H14_IPC_Close;
			_g_pe_cmn_cb_fn.create_pic_task		= PE_TSK_H14_CreatePicTask;
			_g_pe_cmn_cb_fn.stop_pic_task		= PE_TSK_H14_StopPicTask;
			_g_pe_cmn_cb_fn.fwi_init			= PE_FWI_H14_Init;
			_g_pe_cmn_cb_fn.set_default			= NULL;
			_g_pe_cmn_cb_fn.get_default			= NULL;
			_g_pe_cmn_cb_fn.set_debug			= PE_CMN_HW_H14_SetDebugSettings;
			_g_pe_cmn_cb_fn.get_debug			= PE_CMN_HW_H14_GetDebugSettings;
			_g_pe_cmn_cb_fn.set_contrast		= PE_CMN_HW_H14_SetContrast;
			_g_pe_cmn_cb_fn.get_contrast		= PE_CMN_HW_H14_GetContrast;
			_g_pe_cmn_cb_fn.set_brightness		= PE_CMN_HW_H14_SetBrightness;
			_g_pe_cmn_cb_fn.get_brightness		= PE_CMN_HW_H14_GetBrightness;
			_g_pe_cmn_cb_fn.set_saturation		= PE_CMN_HW_H14_SetSaturation;
			_g_pe_cmn_cb_fn.get_saturation		= PE_CMN_HW_H14_GetSaturation;
			_g_pe_cmn_cb_fn.set_hue				= PE_CMN_HW_H14_SetHue;
			_g_pe_cmn_cb_fn.get_hue				= PE_CMN_HW_H14_GetHue;
			_g_pe_cmn_cb_fn.set_level_ctrl		= PE_CMN_HW_H14_SetLevelCtrl;
			_g_pe_cmn_cb_fn.get_level_ctrl		= PE_CMN_HW_H14_GetLevelCtrl;
		}
		else
		#endif
		#ifdef USE_PE_KDRV_CODES_FOR_H13
		if (PE_KDRV_VER_H13)
		{
			_g_pe_cmn_cb_fn.hw_init				= PE_CMN_HW_H13_Init;
			_g_pe_cmn_cb_fn.ipc_open			= PE_FWI_H13_IPC_Open;
			_g_pe_cmn_cb_fn.ipc_close			= PE_FWI_H13_IPC_Close;
			_g_pe_cmn_cb_fn.create_pic_task		= PE_TSK_H13_CreatePicTask;
			_g_pe_cmn_cb_fn.stop_pic_task		= PE_TSK_H13_StopPicTask;
			_g_pe_cmn_cb_fn.fwi_init			= PE_FWI_H13_Init;
			_g_pe_cmn_cb_fn.set_default			= NULL;
			_g_pe_cmn_cb_fn.get_default			= NULL;
			_g_pe_cmn_cb_fn.set_debug			= PE_CMN_HW_H13_SetDebugSettings;
			_g_pe_cmn_cb_fn.get_debug			= PE_CMN_HW_H13_GetDebugSettings;
			_g_pe_cmn_cb_fn.set_contrast		= PE_CMN_HW_H13_SetContrast;
			_g_pe_cmn_cb_fn.get_contrast		= PE_CMN_HW_H13_GetContrast;
			_g_pe_cmn_cb_fn.set_brightness		= PE_CMN_HW_H13_SetBrightness;
			_g_pe_cmn_cb_fn.get_brightness		= PE_CMN_HW_H13_GetBrightness;
			_g_pe_cmn_cb_fn.set_saturation		= PE_CMN_HW_H13_SetSaturation;
			_g_pe_cmn_cb_fn.get_saturation		= PE_CMN_HW_H13_GetSaturation;
			_g_pe_cmn_cb_fn.set_hue				= PE_CMN_HW_H13_SetHue;
			_g_pe_cmn_cb_fn.get_hue				= PE_CMN_HW_H13_GetHue;
			_g_pe_cmn_cb_fn.set_level_ctrl		= PE_CMN_HW_H13_SetLevelCtrl;
			_g_pe_cmn_cb_fn.get_level_ctrl		= PE_CMN_HW_H13_GetLevelCtrl;
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
 * open picture enhance module
 *
 * @param   void
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_CMN_OpenPicEnhanceModule(void)
{
	int ret = RET_OK;
	do{
		if(_g_pe_cmn_cb_fn.ipc_open)
		{
			ret = _g_pe_cmn_cb_fn.ipc_open();
			PE_CHECK_CODE(ret, break, "ipc_open() error.\n");
		}
		if(_g_pe_cmn_cb_fn.create_pic_task)
		{
			ret = _g_pe_cmn_cb_fn.create_pic_task();
			PE_CHECK_CODE(ret, break, "create_pic_task() error.\n");
		}
	}while (0);
	return ret;
}
/**
 * close picture enhance module
 *
 * @param   void
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_CMN_ClosePicEnhanceModule(void)
{
	int ret = RET_OK;
	do{
		if(_g_pe_cmn_cb_fn.stop_pic_task)
		{
			ret = _g_pe_cmn_cb_fn.stop_pic_task();
			PE_CHECK_CODE(ret, break, "stop_pic_task() error.\n");
		}
		if(_g_pe_cmn_cb_fn.ipc_close)
		{
			ret = _g_pe_cmn_cb_fn.ipc_close();
			PE_CHECK_CODE(ret, break, "ipc_close() error.\n");
		}
	}while (0);
	return ret;
}
/**
 * init common module
 *
 * @param   *pstParams [in] PE_CFG_CTRL_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_CMN_Init(PE_CFG_CTRL_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		PE_CMN_DBG_PRINT("init global values.\n");
		memset0(_g_pe_cmn_info);
		if(_g_pe_cmn_cb_fn.fwi_init)
		{
			ret = _g_pe_cmn_cb_fn.fwi_init(pstParams);
			PE_CHECK_CODE(ret, break, "fwi_init() error.\n");
		}
		if(_g_pe_cmn_cb_fn.hw_init)
		{
			ret = _g_pe_cmn_cb_fn.hw_init(pstParams);
			PE_CHECK_CODE(ret, break, "hw_init() error.\n");
		}
	}while (0);
	return ret;
}

/**
 * standard function of cmn setting
 *
 * @param   data [in/out] unsigned int
 * @param   functype [in] unsigned int
 * @param   datasize [in] unsigned int
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_CMN_SetFunction(unsigned int *data, unsigned int functype, unsigned int datasize)
{
	int ret = RET_ERROR;

#ifdef PE_KDRV_USE_SPD_FUNC
#define PE_CMN_SET_FSDP(_struct, _func, _sdp)	\
	case PE_ITEM_PKTFUNC(_struct):\
	{\
		_struct *p_rcv, param;\
		do{\
			PE_CMN_DBG_PRINT("S %s\n", #_struct);\
			PE_CHECK_CODE(datasize!=sizeof(_struct), \
				break, "[%s,%d] size mismatch.\n", __F__, __L__);\
			p_rcv = (_struct *)data;\
			ret = PE_COPY_FROM_USER(_sdp, (void *)&param, (void *)p_rcv, datasize);\
			PE_CHECK_CODE(ret, break, \
				"[%s,%d] %s error.\n", __F__, __L__, #_sdp);\
			ret = _func(&param);\
			PE_CHECK_CODE(ret, break, \
				"[%s,%d] %s() error.\n", __F__, __L__, #_func);\
			PE_CMN_DBG_PRINT("E %s\n", #_struct);\
		}while (0);\
	}\
	break
#endif

#define PE_CMN_SET_FUNC(_struct, _func)	\
	case PE_ITEM_PKTFUNC(_struct):\
	{\
		_struct *p_rcv, param;\
		do{\
			PE_CMN_DBG_PRINT("S %s\n", #_struct);\
			PE_CHECK_CODE(datasize!=sizeof(_struct), break, \
				"[%s,%d] size mismatch.\n", __F__, __L__);\
			p_rcv = (_struct *)data;\
			memcpy(&param, p_rcv, datasize);\
			ret = _func(&param);\
			PE_CHECK_CODE(ret, break, \
				"[%s,%d] %s() error.\n", __F__, __L__, #_func);\
			PE_CMN_DBG_PRINT("E %s\n", #_struct);\
		}while (0);\
	}\
	break

	/* data null check */
	PE_CHECK_CODE(!data, return RET_ERROR, \
		"[%s,%d] data is null.\n", __F__, __L__);
	switch(functype)
	{
		#ifdef PE_KDRV_USE_SPD_FUNC
		PE_CMN_SET_FSDP(LX_PE_CMN_CONTRAST_T,		PE_CMN_SetContrast,			PE_SUSPEND_CMN_SETCONTRAST);
		PE_CMN_SET_FSDP(LX_PE_CMN_BRIGHTNESS_T,		PE_CMN_SetBrightness,		PE_SUSPEND_CMN_SETBRIGHTNESS);
		PE_CMN_SET_FSDP(LX_PE_CMN_SATURATION_T,		PE_CMN_SetSaturation,		PE_SUSPEND_CMN_SETSATURATION);
		PE_CMN_SET_FSDP(LX_PE_CMN_HUE_T,			PE_CMN_SetHue,				PE_SUSPEND_CMN_SETHUE);
		PE_CMN_SET_FSDP(LX_PE_DEFAULT_SETTINGS_T,	PE_CMN_SetDefaultSettings,	PE_SUSPEND_CMN_SETDEFAULT);
		#else
		PE_CMN_SET_FUNC(LX_PE_CMN_CONTRAST_T,		PE_CMN_SetContrast);
		PE_CMN_SET_FUNC(LX_PE_CMN_BRIGHTNESS_T,		PE_CMN_SetBrightness);
		PE_CMN_SET_FUNC(LX_PE_CMN_SATURATION_T,		PE_CMN_SetSaturation);
		PE_CMN_SET_FUNC(LX_PE_CMN_HUE_T,			PE_CMN_SetHue);
		PE_CMN_SET_FUNC(LX_PE_DEFAULT_SETTINGS_T,	PE_CMN_SetDefaultSettings);
		#endif
		PE_CMN_SET_FUNC(LX_PE_CMN_LEVEL_CTRL_T,		PE_CMN_SetLevelCtrl);
		PE_CMN_SET_FUNC(LX_PE_DBG_SETTINGS_T,		PE_CMN_SetDebugSettings);
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
 * standard function of cmn getting
 *
 * @param   data [in/out] unsigned int
 * @param   functype [in] unsigned int
 * @param   datasize [in] unsigned int
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_CMN_GetFunction(unsigned int *data, unsigned int functype, unsigned int datasize)
{
	int ret = RET_ERROR;

#define PE_CMN_GET_FUNC(_struct, _func)	\
	case PE_ITEM_PKTFUNC(_struct):\
	{\
		_struct *p_rcv, param;\
		do{\
			PE_CMN_DBG_PRINT("S %s\n", #_struct);\
			PE_CHECK_CODE(datasize!=sizeof(_struct), break, \
				"[%s,%d] size mismatch.\n", __F__, __L__);\
			p_rcv = (_struct *)data;\
			memcpy(&param, p_rcv, datasize);\
			ret = _func(&param);\
			PE_CHECK_CODE(ret, break, "[%s,%d] %s() error.\n", \
				__F__, __L__, #_func);\
			memcpy(p_rcv, &param, datasize);\
			PE_CMN_DBG_PRINT("E %s\n", #_struct);\
		}while (0);\
	}\
	break

	/* data null check */
	PE_CHECK_CODE(!data, return RET_ERROR, \
		"[%s,%d] data is null.\n", __F__, __L__);
	switch(functype)
	{
		PE_CMN_GET_FUNC(LX_PE_CMN_CONTRAST_T,		PE_CMN_GetContrast);
		PE_CMN_GET_FUNC(LX_PE_CMN_BRIGHTNESS_T,		PE_CMN_GetBrightness);
		PE_CMN_GET_FUNC(LX_PE_CMN_SATURATION_T,		PE_CMN_GetSaturation);
		PE_CMN_GET_FUNC(LX_PE_CMN_HUE_T,			PE_CMN_GetHue);
		PE_CMN_GET_FUNC(LX_PE_CMN_LEVEL_CTRL_T,		PE_CMN_GetLevelCtrl);
		PE_CMN_GET_FUNC(LX_PE_DEFAULT_SETTINGS_T,	PE_CMN_GetDefaultSettings);
		PE_CMN_GET_FUNC(LX_PE_DBG_SETTINGS_T,		PE_CMN_GetDebugSettings);
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
 * set cmn fmt default settings
 *
 * @param   *pstParams [in] LX_PE_DEFAULT_SETTINGS_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_CMN_SetDefaultSettings(LX_PE_DEFAULT_SETTINGS_T *pstParams)
{
	int ret = RET_OK;
	PE_CMN_SETTINGS_T *pInfo = NULL;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		pInfo=&_g_pe_cmn_info;	/* to avoid warning(unused variable) */
		PE_CMN_DBG_PRINT("[win id:%d]\n", pstParams->win_id);
		if (PE_CHECK_WIN0(pstParams->win_id))
		{
			memcpy(&(pInfo->dflt[LX_PE_WIN_0]), pstParams, sizeof(LX_PE_DEFAULT_SETTINGS_T));
		}
		if (PE_CHECK_WIN1(pstParams->win_id))
		{
			memcpy(&(pInfo->dflt[LX_PE_WIN_1]), pstParams, sizeof(LX_PE_DEFAULT_SETTINGS_T));
		}
	}while (0);
	return ret;
}
/**
 * get cmn fmt default settings
 *
 * @param   *pstParams [in/out] LX_PE_DEFAULT_SETTINGS_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_CMN_GetDefaultSettings(LX_PE_DEFAULT_SETTINGS_T *pstParams)
{
	int ret = RET_OK;
	LX_PE_WIN_ID win_id;
	PE_CMN_SETTINGS_T *pInfo = &_g_pe_cmn_info;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		win_id = PE_GET_CHECKED_WINID(pstParams->win_id);
		if (PE_CHECK_WIN0(win_id))
		{
			memcpy(pstParams, &(pInfo->dflt[LX_PE_WIN_0]), sizeof(LX_PE_DEFAULT_SETTINGS_T));
		}
		if (PE_CHECK_WIN1(win_id))
		{
			memcpy(pstParams, &(pInfo->dflt[LX_PE_WIN_1]), sizeof(LX_PE_DEFAULT_SETTINGS_T));
		}
		PE_CMN_DBG_PRINT("[win id:%d]\n", pstParams->win_id);
	}while (0);
	return ret;
}
/**
 * set cmn debug settings
 *
 * @param   *pstParams [in] LX_PE_DBG_SETTINGS_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_CMN_SetDebugSettings(LX_PE_DBG_SETTINGS_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		/* set print level */
		if (pstParams->type&LX_PE_DBG_LV)
		{
			_g_cmn_trace = (pstParams->print_lvl & \
				(0x1<<PE_ITEM_PKTMODL(CMN)))? 0x1:0x0;
		}
		if(_g_pe_cmn_cb_fn.set_debug)
		{
			ret = _g_pe_cmn_cb_fn.set_debug(pstParams);
			PE_CHECK_CODE(ret, break, "set_debug() error.\n");
		}
	}while (0);
	return ret;
}

/**
 * get cmn debug settings
 *
 * @param   *pstParams [in] LX_PE_DBG_SETTINGS_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_CMN_GetDebugSettings(LX_PE_DBG_SETTINGS_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(_g_pe_cmn_cb_fn.get_debug)
		{
			ret = _g_pe_cmn_cb_fn.get_debug(pstParams);
			PE_CHECK_CODE(ret, break, "get_debug() error.\n");
		}
	}while (0);
	return ret;
}
/**
 * set contrast
 *
 * @param   *pstParams [in] LX_PE_CMN_CONTRAST_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_CMN_SetContrast(LX_PE_CMN_CONTRAST_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(_g_pe_cmn_cb_fn.set_contrast)
		{
			ret = _g_pe_cmn_cb_fn.set_contrast(pstParams);
			PE_CHECK_CODE(ret, break, "set_contrast() error.\n");
		}
	}while (0);
	return ret;
}

/**
 * get contrast
 *
 * @param   *pstParams [in/out] LX_PE_CMN_CONTRAST_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_CMN_GetContrast(LX_PE_CMN_CONTRAST_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(_g_pe_cmn_cb_fn.get_contrast)
		{
			ret = _g_pe_cmn_cb_fn.get_contrast(pstParams);
			PE_CHECK_CODE(ret, break, "get_contrast() error.\n");
		}
	}while (0);
	return ret;
}
/**
 * set brightness
 *
 * @param   *pstParams [in] LX_PE_CMN_BRIGHTNESS_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_CMN_SetBrightness(LX_PE_CMN_BRIGHTNESS_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(_g_pe_cmn_cb_fn.set_brightness)
		{
			ret = _g_pe_cmn_cb_fn.set_brightness(pstParams);
			PE_CHECK_CODE(ret, break, "set_brightness() error.\n");
		}
	}while (0);
	return ret;
}
/**
 * get brightness
 *
 * @param   *pstParams [in/out] LX_PE_CMN_BRIGHTNESS_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_CMN_GetBrightness(LX_PE_CMN_BRIGHTNESS_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(_g_pe_cmn_cb_fn.get_brightness)
		{
			ret = _g_pe_cmn_cb_fn.get_brightness(pstParams);
			PE_CHECK_CODE(ret, break, "get_brightness() error.\n");
		}
	}while (0);
	return ret;
}
/**
 * set saturation
 *
 * @param   *pstParams [in] LX_PE_CMN_SATURATION_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_CMN_SetSaturation(LX_PE_CMN_SATURATION_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(_g_pe_cmn_cb_fn.set_saturation)
		{
			ret = _g_pe_cmn_cb_fn.set_saturation(pstParams);
			PE_CHECK_CODE(ret, break, "set_saturation() error.\n");
		}
	}while (0);
	return ret;
}
/**
 * get saturation
 *
 * @param   *pstParams [in/out] LX_PE_CMN_SATURATION_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_CMN_GetSaturation(LX_PE_CMN_SATURATION_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(_g_pe_cmn_cb_fn.get_saturation)
		{
			ret = _g_pe_cmn_cb_fn.get_saturation(pstParams);
			PE_CHECK_CODE(ret, break, "get_saturation() error.\n");
		}
	}while (0);
	return ret;
}
/**
 * set hue
 *
 * @param   *pstParams [in] LX_PE_CMN_HUE_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_CMN_SetHue(LX_PE_CMN_HUE_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(_g_pe_cmn_cb_fn.set_hue)
		{
			ret = _g_pe_cmn_cb_fn.set_hue(pstParams);
			PE_CHECK_CODE(ret, break, "set_hue() error.\n");
		}
	}while (0);
	return ret;
}
/**
 * get hue
 *
 * @param   *pstParams [in/out] LX_PE_CMN_HUE_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_CMN_GetHue(LX_PE_CMN_HUE_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(_g_pe_cmn_cb_fn.get_hue)
		{
			ret = _g_pe_cmn_cb_fn.get_hue(pstParams);
			PE_CHECK_CODE(ret, break, "get_hue() error.\n");
		}
	}while (0);
	return ret;
}
/**
 * set level control
 *
 * @param   *pstParams [in] LX_PE_CMN_LEVEL_CTRL_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_CMN_SetLevelCtrl(LX_PE_CMN_LEVEL_CTRL_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(_g_pe_cmn_cb_fn.set_level_ctrl)
		{
			ret = _g_pe_cmn_cb_fn.set_level_ctrl(pstParams);
			PE_CHECK_CODE(ret, break, "set_level_ctrl() error.\n");
		}
	}while (0);
	return ret;
}
/**
 * get level control
 *
 * @param   *pstParams [in/out] LX_PE_CMN_LEVEL_CTRL_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_CMN_GetLevelCtrl(LX_PE_CMN_LEVEL_CTRL_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(_g_pe_cmn_cb_fn.get_level_ctrl)
		{
			ret = _g_pe_cmn_cb_fn.get_level_ctrl(pstParams);
			PE_CHECK_CODE(ret, break, "get_level_ctrl() error.\n");
		}
	}while (0);
	return ret;
}

