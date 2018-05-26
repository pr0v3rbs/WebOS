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

/** @file pe_etc.c
 *
 *  driver for picture enhance etc functions. ( used only within kdriver )
 *	- functional functions regardless of modules
 *	
 *	@author		Seung-Jun, Youm(sj.youm@lge.com)
 *	@version	0.1
 *	@note		
 *	@date		2011.07.16
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
#include "pe_spd.h"
#include "pe_etc.h"
#include "pe_reg_hw.h"
#include "pe_etc_hw.h"
#include "pe_cmn_hw.h"

/*----------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------*/
#define PE_ETC_DBG_PRINT(fmt, args...)	\
	if (_g_etc_trace)\
	{\
		printk("[%x,%x][%s,%d] "fmt, \
			PE_CHIP_VER, g_pe_kdrv_ver_mask, __F__, __L__, ##args);\
	}

/*----------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------*/
typedef struct
{
	int (*hw_inf_init)(PE_CFG_CTRL_T *pstParams);
	int (*start_pic_task)(PE_CFG_CTRL_T *pstParams);
	int (*download_db)(PE_CFG_CTRL_T *pstParams);
	int (*set_default)(LX_PE_DEFAULT_SETTINGS_T *pstParams);
	int (*set_tsk_debug)(LX_PE_DBG_SETTINGS_T *pstParams);
	int (*set_inf_debug)(LX_PE_DBG_SETTINGS_T *pstParams);
	int (*set_misc_tbl)(LX_PE_ETC_TBL_T *pstParams);
	int (*get_misc_tbl)(LX_PE_ETC_TBL_T *pstParams);
	int (*set_disp_info)(LX_PE_INF_DISPLAY_T *pstParams);
	int (*get_disp_info)(LX_PE_INF_DISPLAY_T *pstParams);
	int (*get_level_info)(LX_PE_INF_LEVEL_T *pstParams);
	int (*set_trace_pkt)(LX_PE_PKT_T *pstParams);
}
PE_ETC_CB_FUNC_T;

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
static int PE_ETC_SetDebugSettings(LX_PE_DBG_SETTINGS_T *pstParams);
static int PE_ETC_SetDefaultSettings(LX_PE_DEFAULT_SETTINGS_T *pstParams);
static int PE_ETC_SetDisplayInfo(LX_PE_INF_DISPLAY_T *pstParams);
static int PE_ETC_GetDisplayInfo(LX_PE_INF_DISPLAY_T *pstParams);
static int PE_ETC_SetMiscTbl(LX_PE_ETC_TBL_T *pstParams);
static int PE_ETC_GetMiscTbl(LX_PE_ETC_TBL_T *pstParams);
static int PE_ETC_GetLevelInfo(LX_PE_INF_LEVEL_T *pstParams);

/*----------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------*/
static PE_ETC_CB_FUNC_T _g_pe_etc_cb_fn;
static UINT32 _g_etc_trace=0x0;	//default should be off.

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
int PE_ETC_RegisterCbFunc(void)
{
	int ret = RET_OK;

	memset(&_g_pe_etc_cb_fn, 0, sizeof(PE_ETC_CB_FUNC_T));
	do{
		#ifdef USE_PE_KDRV_CODES_FOR_H15
		if (PE_KDRV_VER_H15)
		{
			_g_pe_etc_cb_fn.hw_inf_init			= PE_INF_H15_Init;
			_g_pe_etc_cb_fn.start_pic_task		= PE_TSK_H15_StartPicTask;
			_g_pe_etc_cb_fn.download_db			= PE_CMN_HW_H15_DownloadDb;
			_g_pe_etc_cb_fn.set_default			= NULL;
			_g_pe_etc_cb_fn.set_tsk_debug		= PE_TSK_H15_SetDebugSettings;
			_g_pe_etc_cb_fn.set_inf_debug		= PE_INF_H15_SetDebugSettings;
			_g_pe_etc_cb_fn.set_misc_tbl		= PE_INF_H15_SetMiscTbl;
			_g_pe_etc_cb_fn.get_misc_tbl		= PE_INF_H15_GetMiscTbl;
			_g_pe_etc_cb_fn.set_disp_info		= PE_INF_H15_SetDisplayInfo;
			_g_pe_etc_cb_fn.get_disp_info		= PE_INF_H15_GetDisplayInfo;
			_g_pe_etc_cb_fn.get_level_info		= PE_INF_H15_GetLevelInfo;
			_g_pe_etc_cb_fn.set_trace_pkt		= PE_INF_H15_SetTracePacketData;
		}
		else
		#endif
		#ifdef USE_PE_KDRV_CODES_FOR_M14
		if (PE_KDRV_VER_M14)
		{
			_g_pe_etc_cb_fn.hw_inf_init			= PE_INF_M14_Init;
			_g_pe_etc_cb_fn.start_pic_task		= PE_TSK_M14_StartPicTask;
			_g_pe_etc_cb_fn.download_db			= NULL;
			_g_pe_etc_cb_fn.set_default			= NULL;
			_g_pe_etc_cb_fn.set_tsk_debug		= PE_TSK_M14_SetDebugSettings;
			_g_pe_etc_cb_fn.set_inf_debug		= PE_INF_M14_SetDebugSettings;
			_g_pe_etc_cb_fn.set_misc_tbl		= PE_INF_M14_SetMiscTbl;
			_g_pe_etc_cb_fn.get_misc_tbl		= PE_INF_M14_GetMiscTbl;
			_g_pe_etc_cb_fn.set_disp_info		= PE_INF_M14_SetDisplayInfo;
			_g_pe_etc_cb_fn.get_disp_info		= PE_INF_M14_GetDisplayInfo;
			_g_pe_etc_cb_fn.get_level_info		= PE_INF_M14_GetLevelInfo;
			_g_pe_etc_cb_fn.set_trace_pkt		= PE_INF_M14_SetTracePacketData;
		}
		else
		#endif
		#ifdef USE_PE_KDRV_CODES_FOR_H14
		if (PE_KDRV_VER_H14)
		{
			_g_pe_etc_cb_fn.hw_inf_init			= PE_INF_H14_Init;
			_g_pe_etc_cb_fn.start_pic_task		= PE_TSK_H14_StartPicTask;
			_g_pe_etc_cb_fn.download_db			= NULL;
			_g_pe_etc_cb_fn.set_default			= NULL;
			_g_pe_etc_cb_fn.set_tsk_debug		= PE_TSK_H14_SetDebugSettings;
			_g_pe_etc_cb_fn.set_inf_debug		= PE_INF_H14_SetDebugSettings;
			_g_pe_etc_cb_fn.set_misc_tbl		= PE_INF_H14_SetMiscTbl;
			_g_pe_etc_cb_fn.get_misc_tbl		= PE_INF_H14_GetMiscTbl;
			_g_pe_etc_cb_fn.set_disp_info		= PE_INF_H14_SetDisplayInfo;
			_g_pe_etc_cb_fn.get_disp_info		= PE_INF_H14_GetDisplayInfo;
			_g_pe_etc_cb_fn.get_level_info		= PE_INF_H14_GetLevelInfo;
			_g_pe_etc_cb_fn.set_trace_pkt		= NULL;
		}
		else
		#endif
		#ifdef USE_PE_KDRV_CODES_FOR_H13
		if (PE_KDRV_VER_H13)
		{
			_g_pe_etc_cb_fn.hw_inf_init			= PE_INF_H13_Init;
			_g_pe_etc_cb_fn.start_pic_task		= PE_TSK_H13_StartPicTask;
			_g_pe_etc_cb_fn.download_db			= NULL;
			_g_pe_etc_cb_fn.set_default			= NULL;
			_g_pe_etc_cb_fn.set_tsk_debug		= PE_TSK_H13_SetDebugSettings;
			_g_pe_etc_cb_fn.set_inf_debug		= PE_INF_H13_SetDebugSettings;
			_g_pe_etc_cb_fn.set_misc_tbl		= PE_INF_H13_SetMiscTbl;
			_g_pe_etc_cb_fn.get_misc_tbl		= PE_INF_H13_GetMiscTbl;
			_g_pe_etc_cb_fn.set_disp_info		= PE_INF_H13_SetDisplayInfo;
			_g_pe_etc_cb_fn.get_disp_info		= PE_INF_H13_GetDisplayInfo;
			_g_pe_etc_cb_fn.get_level_info		= PE_INF_H13_GetLevelInfo;
			_g_pe_etc_cb_fn.set_trace_pkt		= NULL;
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
 * set trace packet data
 *
 * @param   *pstParams [in] LX_PE_PKT_T int
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author	sj.youm@lge.com
 */ 
int PE_ETC_SetTracePacketData(LX_PE_PKT_T *pstParams)
{
    int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(_g_pe_etc_cb_fn.set_trace_pkt)
		{
			ret = _g_pe_etc_cb_fn.set_trace_pkt(pstParams);
			PE_CHECK_CODE(ret, break, "set_trace_pkt() error.\n");
		}
	}while (0);
	return ret;
}
/**
 * misc. module
 *
 * @param   *pstParams [in] PE_CFG_CTRL_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_ETC_Init(PE_CFG_CTRL_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(_g_pe_etc_cb_fn.hw_inf_init)
		{
			ret = _g_pe_etc_cb_fn.hw_inf_init(pstParams);
			PE_CHECK_CODE(ret, break, "hw_inf_init() error.\n");
		}
		/* download db should be the last in the pe initialization */
		if(_g_pe_etc_cb_fn.download_db)
		{
			ret = _g_pe_etc_cb_fn.download_db(pstParams);
			PE_CHECK_CODE(ret, break, "download_db() error.\n");
		}
		/* Starting task should be the last in the pe initialization */
		if(_g_pe_etc_cb_fn.start_pic_task)
		{
			ret = _g_pe_etc_cb_fn.start_pic_task(pstParams);
			PE_CHECK_CODE(ret, break, "start_pic_task() error.\n");
		}
	}while (0);
	return ret;
}

/**
 * standard function of etc setting
 *
 * @param   data [in/out] unsigned int
 * @param   functype [in] unsigned int
 * @param   datasize [in] unsigned int
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_ETC_SetFunction(unsigned int *data, unsigned int functype, unsigned int datasize)
{
	int ret = RET_ERROR;

#ifdef PE_KDRV_USE_SPD_FUNC
#define PE_ETC_SET_FSDP(_struct, _func, _sdp)	\
	case PE_ITEM_PKTFUNC(_struct):\
	{\
		_struct *p_rcv, param;\
		do{\
			PE_ETC_DBG_PRINT("S %s\n", #_struct);\
			PE_CHECK_CODE(datasize!=sizeof(_struct), break, \
				"[%s,%d] size mismatch.\n", __F__, __L__);\
			p_rcv = (_struct *)data;\
			ret = PE_COPY_FROM_USER(_sdp, (void *)&param, (void *)p_rcv, datasize);\
			PE_CHECK_CODE(ret, break, \
				"[%s,%d] %s error.\n", __F__, __L__, #_sdp);\
			ret = _func(&param);\
			PE_CHECK_CODE(ret, break, \
				"[%s,%d] %s() error.\n", __F__, __L__, #_func);\
			PE_ETC_DBG_PRINT("E %s\n", #_struct);\
		}while (0);\
	}\
	break
#endif

#define PE_ETC_SET_FUNC(_struct, _func)	\
	case PE_ITEM_PKTFUNC(_struct):\
	{\
		_struct *p_rcv, param;\
		do{\
			PE_ETC_DBG_PRINT("S %s\n", #_struct);\
			PE_CHECK_CODE(datasize!=sizeof(_struct), break, \
				"[%s,%d] size mismatch.\n", __F__, __L__);\
			p_rcv = (_struct *)data;\
			memcpy(&param, p_rcv, datasize);\
			ret = _func(&param);\
			PE_CHECK_CODE(ret, break, \
				"[%s,%d] %s() error.\n", __F__, __L__, #_func);\
			PE_ETC_DBG_PRINT("E %s\n", #_struct);\
		}while (0);\
	}\
	break

#define PE_ETC_SET_TABL(_struct, _func)	\
	case PE_ITEM_PKTFUNC(_struct):\
	{\
		_struct *p_rcv, param;\
		UINT32 *p_data = NULL;\
		UINT32 bufsize;\
		do{\
			PE_ETC_DBG_PRINT("S %s\n", #_struct);\
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
			param.func_num = p_rcv->func_num;\
			param.size = p_rcv->size;\
			param.data = p_data;\
			ret = _func(&param);\
			PE_CHECK_CODE(ret, break, \
				"[%s,%d] %s() error.\n", __F__, __L__, #_func);\
			PE_ETC_DBG_PRINT("E %s\n", #_struct);\
		}while (0);\
		if (p_data){OS_Free(p_data);}\
	}\
	break

	/* data null check */
	PE_CHECK_CODE(!data, return RET_ERROR, 
		"[%s,%d] data is null.\n", __F__, __L__);
	switch (functype)
	{
		PE_ETC_SET_TABL(LX_PE_ETC_TBL_T, 			PE_ETC_SetMiscTbl);
		PE_ETC_SET_FUNC(LX_PE_INF_DISPLAY_T, 		PE_ETC_SetDisplayInfo);
		PE_ETC_SET_FUNC(LX_PE_DBG_SETTINGS_T, 		PE_ETC_SetDebugSettings);
		PE_ETC_SET_FUNC(LX_PE_DEFAULT_SETTINGS_T, 	PE_ETC_SetDefaultSettings);
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
 * standard function of etc getting
 *
 * @param   data [in/out] unsigned int
 * @param   functype [in] unsigned int
 * @param   datasize [in] unsigned int
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_ETC_GetFunction(unsigned int *data, unsigned int functype, unsigned int datasize)
{
	int ret = RET_ERROR;

#define PE_ETC_GET_FUNC(_struct, _func)	\
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

#define PE_ETC_GET_TABL(_struct, _func)	\
	case PE_ITEM_PKTFUNC(_struct):\
	{\
		_struct *p_rcv, param;\
		UINT32 *p_data = NULL;\
		UINT32 bufsize;\
		memset0(param);\
		do{\
			PE_ETC_DBG_PRINT("S %s\n", #_struct);\
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
			param.func_num = p_rcv->func_num;\
			param.size = p_rcv->size;\
			param.data = p_data;\
			ret = _func(&param);\
			PE_CHECK_CODE(ret, break, \
				"[%s,%d] %s() error.\n", __F__, __L__, #_func);\
			ret = copy_to_user((void __user *)p_rcv->data, p_data, bufsize);\
			PE_CHECK_CODE(ret, break, \
				"[%s,%d] copy_to_user error.\n", __F__, __L__);\
			PE_ETC_DBG_PRINT("E %s\n", #_struct);\
		}while (0);\
		if (p_data){OS_Free(p_data);}\
	}\
	break

	/* data null check */
	PE_CHECK_CODE(!data, return RET_ERROR, \
		"[%s,%d] data is null.\n", __F__, __L__);
	switch (functype)
	{
		PE_ETC_GET_TABL(LX_PE_ETC_TBL_T, 		PE_ETC_GetMiscTbl);
		PE_ETC_GET_FUNC(LX_PE_INF_DISPLAY_T, 	PE_ETC_GetDisplayInfo);
		PE_ETC_GET_FUNC(LX_PE_INF_LEVEL_T, 		PE_ETC_GetLevelInfo);
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
 * debug setting
 *
 * @param   *pstParams [in] LX_PE_DBG_SETTINGS_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_ETC_SetDebugSettings(LX_PE_DBG_SETTINGS_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		/* set print level */
		if (pstParams->type&LX_PE_DBG_LV)
		{
			_g_etc_trace = (pstParams->print_lvl & \
				(0x1<<PE_ITEM_PKTMODL(ETC)))? 0x1:0x0;
		}
		if(_g_pe_etc_cb_fn.set_tsk_debug)
		{
			ret = _g_pe_etc_cb_fn.set_tsk_debug(pstParams);
			PE_CHECK_CODE(ret, break, "set_tsk_debug() error.\n");
		}
		if(_g_pe_etc_cb_fn.set_inf_debug)
		{
			ret = _g_pe_etc_cb_fn.set_inf_debug(pstParams);
			PE_CHECK_CODE(ret, break, "set_inf_debug() error.\n");
		}
	}while (0);
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
static int PE_ETC_SetDefaultSettings(LX_PE_DEFAULT_SETTINGS_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		PE_ETC_DBG_PRINT("[win id:%d]\n", pstParams->win_id);
	}while (0);
	return ret;
}
/**
 * set misc table
 *
 * @param   *pstParams [in] LX_PE_ETC_TBL_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_ETC_SetMiscTbl(LX_PE_ETC_TBL_T *pstParams)
{
    int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(_g_pe_etc_cb_fn.set_misc_tbl)
		{
			ret = _g_pe_etc_cb_fn.set_misc_tbl(pstParams);
			PE_CHECK_CODE(ret, break, "set_misc_tbl() error.\n");
		}
	}while (0);
	return ret;
}
/**
 * get misc debug(for debug)
 *
 * @param   *pstParams [in/out] LX_PE_ETC_TBL_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_ETC_GetMiscTbl(LX_PE_ETC_TBL_T *pstParams)
{
    int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(_g_pe_etc_cb_fn.get_misc_tbl)
		{
			ret = _g_pe_etc_cb_fn.get_misc_tbl(pstParams);
			PE_CHECK_CODE(ret, break, "get_misc_tbl() error.\n");
		}
	}while (0);
	return ret;
}
/**
 * set display info
 *
 * @param   *pstParams [in] LX_PE_INF_DISPLAY_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_ETC_SetDisplayInfo(LX_PE_INF_DISPLAY_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(_g_pe_etc_cb_fn.set_disp_info)
		{
			ret = _g_pe_etc_cb_fn.set_disp_info(pstParams);
			PE_CHECK_CODE(ret, break, "set_disp_info() error.\n");
		}
	}while (0);
	return ret;
}
/**
 * get display info
 *
 * @param   *pstParams [in/out] LX_PE_INF_DISPLAY_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_ETC_GetDisplayInfo(LX_PE_INF_DISPLAY_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(_g_pe_etc_cb_fn.get_disp_info)
		{
			ret = _g_pe_etc_cb_fn.get_disp_info(pstParams);
			PE_CHECK_CODE(ret, break, "get_disp_info() error.\n");
		}
	}while (0);
	return ret;
}
/**
 * get level info
 *
 * @param   *pstParams [in/out] LX_PE_INF_LEVEL_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_ETC_GetLevelInfo(LX_PE_INF_LEVEL_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(_g_pe_etc_cb_fn.get_level_info)
		{
			ret = _g_pe_etc_cb_fn.get_level_info(pstParams);
			PE_CHECK_CODE(ret, break, "get_level_info() error.\n");
		}
	}while (0);
	return ret;
}

