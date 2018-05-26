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

/** @file pe_io.c
 *
 *  io interface functions for pe device. ( used only within kdriver )
 *	
 *	@author		
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
#include <linux/sched.h>
#include <linux/wait.h>
#include <linux/ctype.h>
#include <linux/semaphore.h>
#include <asm/uaccess.h>
#include <asm/io.h>


#include "base_types.h"
#include "os_util.h"
#include "debug_util.h"

#include "pe_cfg.h"
#include "pe_def.h"
#include "pe_io.h"
#include "pe_cmn.h"
#include "pe_csc.h"
#include "pe_cmg.h"
#include "pe_nrd.h"
#include "pe_dnt.h"
#include "pe_shp.h"
#include "pe_ccm.h"
#include "pe_dcm.h"
#include "pe_win.h"
#include "pe_spd.h"
#include "pe_etc.h"
#include "pe_hst.h"
#include "pe_reg.h"

/*----------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------*/
#define PE_IO_INIT__ON	1
#define PE_IO_INIT_OFF	0
#define PE_IO_OPEN__ON	1
#define PE_IO_OPEN_OFF	0

/*----------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------*/
#define PE_IO_CHECK_CODE(_checker, _action, fmt, args...)	\
	if (_checker)\
	{\
		PE_PRINT_ERROR(fmt, ##args);\
		_action;\
	}

/*----------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------*/

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

/*----------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------*/
static PE_IO_INF_T _g_pe_io_inf;	// pe io info

/*============================================================================
	Implementation Group
============================================================================*/

/**
 * process packet
 *
 * @param   rcvdata [in] unsigned int
 * @param   recvpkt [in] LX_PE_PKT_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author	sj.youm@lge.com
 */ 
int PE_IO_ProcessPkt(unsigned int *rcvdata, LX_PE_PKT_T *rcvpkt)
{
	int ret = RET_OK;
	unsigned int rw_type, module_type, func_type, data_size;

	PE_IO_CHECK_CODE(!rcvdata, return RET_ERROR, \
		"[%s,%d]rcvdata is null. \n", __F__, __L__);
	PE_IO_CHECK_CODE(!rcvpkt, return RET_ERROR, \
		"[%s,%d]rcvpkt is null. \n", __F__, __L__);

	PE_ETC_SetTracePacketData(rcvpkt);
	rw_type = PE_GET_PKTINFO_RWTYPE(rcvpkt->info);
	module_type = PE_GET_PKTINFO_MODLTYPE(rcvpkt->info);
	func_type = PE_GET_PKTINFO_FUNCTYPE(rcvpkt->info);
	data_size = rcvpkt->size;
	PE_PRINT_TRACE("type:rw:%d, modl:%d, func:%d, size:%d\n", \
		rw_type, module_type, func_type, data_size);

	switch (rw_type)
	{
		case PE_ITEM_PKTRW(SET):
		case PE_ITEM_PKTRW(GET):
		{
			switch (module_type)
			{
				case PE_ITEM_PKTMODL(CMN):
					if (rw_type==PE_ITEM_PKTRW(SET))
						ret=PE_CMN_SetFunction(rcvdata, func_type, data_size);
					else
						ret=PE_CMN_GetFunction(rcvdata, func_type, data_size);
				break;
				case PE_ITEM_PKTMODL(CSC):
					if (rw_type==PE_ITEM_PKTRW(SET))
						ret=PE_CSC_SetFunction(rcvdata, func_type, data_size);
					else
						ret=PE_CSC_GetFunction(rcvdata, func_type, data_size);
				break;
				case PE_ITEM_PKTMODL(CMG):
					if (rw_type==PE_ITEM_PKTRW(SET))
						ret=PE_CMG_SetFunction(rcvdata, func_type, data_size);
					else
						ret=PE_CMG_GetFunction(rcvdata, func_type, data_size);
				break;
				case PE_ITEM_PKTMODL(NRD):
					if (rw_type==PE_ITEM_PKTRW(SET))
						ret=PE_NRD_SetFunction(rcvdata, func_type, data_size);
					else
						ret=PE_NRD_GetFunction(rcvdata, func_type, data_size);
				break;
				case PE_ITEM_PKTMODL(DNT):
					if (rw_type==PE_ITEM_PKTRW(SET))
						ret=PE_DNT_SetFunction(rcvdata, func_type, data_size);
					else
						ret=PE_DNT_GetFunction(rcvdata, func_type, data_size);
				break;
				case PE_ITEM_PKTMODL(SHP):
					if (rw_type==PE_ITEM_PKTRW(SET))
						ret=PE_SHP_SetFunction(rcvdata, func_type, data_size);
					else
						ret=PE_SHP_GetFunction(rcvdata, func_type, data_size);
				break;
				case PE_ITEM_PKTMODL(CCM):
					if (rw_type==PE_ITEM_PKTRW(SET))
						ret=PE_CCM_SetFunction(rcvdata, func_type, data_size);
					else
						ret=PE_CCM_GetFunction(rcvdata, func_type, data_size);
				break;
				case PE_ITEM_PKTMODL(DCM):
					if (rw_type==PE_ITEM_PKTRW(SET))
						ret=PE_DCM_SetFunction(rcvdata, func_type, data_size);
					else
						ret=PE_DCM_GetFunction(rcvdata, func_type, data_size);
				break;
				case PE_ITEM_PKTMODL(HST):
					if (rw_type==PE_ITEM_PKTRW(SET))
						ret=PE_HST_SetFunction(rcvdata, func_type, data_size);
					else
						ret=PE_HST_GetFunction(rcvdata, func_type, data_size);
				break;
				case PE_ITEM_PKTMODL(WIN):
					if (rw_type==PE_ITEM_PKTRW(SET))
						ret=PE_WIN_SetFunction(rcvdata, func_type, data_size);
					else
						ret=PE_WIN_GetFunction(rcvdata, func_type, data_size);
				break;
				case PE_ITEM_PKTMODL(ETC):
					if (rw_type==PE_ITEM_PKTRW(SET))
						ret=PE_ETC_SetFunction(rcvdata, func_type, data_size);
					else
						ret=PE_ETC_GetFunction(rcvdata, func_type, data_size);
				break;
				case PE_ITEM_PKTMODL(DEFAULT):
				case PE_ITEM_PKTMODL(DBG):
				{
					if (rw_type==PE_ITEM_PKTRW(SET))
					{
						do{
							ret=PE_CMN_SetFunction(rcvdata, func_type, data_size);
							PE_IO_CHECK_CODE(ret, break, \
								"[%s,%d]PE_CMN_SetFunction() error..\n", __F__, __L__);
							ret=PE_CSC_SetFunction(rcvdata, func_type, data_size);
							PE_IO_CHECK_CODE(ret, break, \
								"[%s,%d]PE_CSC_SetFunction() error..\n", __F__, __L__);
							ret=PE_CMG_SetFunction(rcvdata, func_type, data_size);
							PE_IO_CHECK_CODE(ret, break, \
								"[%s,%d]PE_CMG_SetFunction() error..\n", __F__, __L__);
							ret=PE_NRD_SetFunction(rcvdata, func_type, data_size);
							PE_IO_CHECK_CODE(ret, break, \
								"[%s,%d]PE_NRD_SetFunction() error..\n", __F__, __L__);
							ret=PE_DNT_SetFunction(rcvdata, func_type, data_size);
							PE_IO_CHECK_CODE(ret, break, \
								"[%s,%d]PE_DNT_SetFunction() error..\n", __F__, __L__);
							ret=PE_SHP_SetFunction(rcvdata, func_type, data_size);
							PE_IO_CHECK_CODE(ret, break, \
								"[%s,%d]PE_SHP_SetFunction() error..\n", __F__, __L__);
							ret=PE_CCM_SetFunction(rcvdata, func_type, data_size);
							PE_IO_CHECK_CODE(ret, break, \
								"[%s,%d]PE_CCM_SetFunction() error..\n", __F__, __L__);
							ret=PE_DCM_SetFunction(rcvdata, func_type, data_size);
							PE_IO_CHECK_CODE(ret, break, \
								"[%s,%d]PE_DCM_SetFunction() error..\n", __F__, __L__);
							ret=PE_HST_SetFunction(rcvdata, func_type, data_size);
							PE_IO_CHECK_CODE(ret, break, \
								"[%s,%d]PE_HST_SetFunction() error..\n", __F__, __L__);
							ret=PE_WIN_SetFunction(rcvdata, func_type, data_size);
							PE_IO_CHECK_CODE(ret, break, \
								"[%s,%d]PE_WIN_SetFunction() error..\n", __F__, __L__);
							ret=PE_ETC_SetFunction(rcvdata, func_type, data_size);
							PE_IO_CHECK_CODE(ret, break, \
								"[%s,%d]PE_ETC_SetFunction() error..\n", __F__, __L__);
						}while (0);
					}
					else
					{
						ret=PE_CMN_GetFunction(rcvdata, func_type, data_size);
					}
				}
				break;
				default:
					PE_PRINT_WARN("[%s,%d]module_type default case...\n", __F__, __L__);
					ret = RET_ERROR;
				break;
			}
		}
		break;
		case PE_ITEM_PKTRW(INIT):
		{
			PE_CFG_CTRL_T init_param;
			init_param.resume_mode = 0;		//resume off
			PE_IO_Init(&init_param);
			if (PE_SPD_CheckInitResume(rcvdata, data_size))
			{
				PE_SPD_resume();
			}
		}
		break;
		default:
		{
			PE_PRINT_WARN("[%s,%d]rw_type default case...\n", __F__, __L__);
			ret = RET_ERROR;
		}
		break;
	}
	return ret;
}

/**
 * init picture enhance module
 *
 * @param   void
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author	sj.youm@lge.com
 */
int PE_IO_InitPicEnhanceModule(void)
{
	int ret = RET_OK;

	PE_PRINT_TRACE("S\n");
	do{
		/* 1. check chip rev. */
		ret = PE_CMN_CheckChipRev();
		PE_IO_CHECK_CODE(ret, break, "PE_CMN_CheckChipRev() error..\n");
		/* 2. register call-back functions */
		ret = PE_CMN_RegisterCbFunc();
		PE_IO_CHECK_CODE(ret, break, "PE_CMN_RegisterCbFunc() error..\n");
		ret = PE_REG_RegisterCbFunc();
		PE_IO_CHECK_CODE(ret, break, "PE_REG_RegisterCbFunc() error..\n");
		ret = PE_CSC_RegisterCbFunc();
		PE_IO_CHECK_CODE(ret, break, "PE_CSC_RegisterCbFunc() error..\n");
		ret = PE_CMG_RegisterCbFunc();
		PE_IO_CHECK_CODE(ret, break, "PE_CMG_RegisterCbFunc() error..\n");
		ret = PE_NRD_RegisterCbFunc();
		PE_IO_CHECK_CODE(ret, break, "PE_NRD_RegisterCbFunc() error..\n");
		ret = PE_DNT_RegisterCbFunc();
		PE_IO_CHECK_CODE(ret, break, "PE_DNT_RegisterCbFunc() error..\n");
		ret = PE_SHP_RegisterCbFunc();
		PE_IO_CHECK_CODE(ret, break, "PE_SHP_RegisterCbFunc() error..\n");
		ret = PE_CCM_RegisterCbFunc();
		PE_IO_CHECK_CODE(ret, break, "PE_CCM_RegisterCbFunc() error..\n");
		ret = PE_DCM_RegisterCbFunc();
		PE_IO_CHECK_CODE(ret, break, "PE_DCM_RegisterCbFunc() error..\n");
		ret = PE_HST_RegisterCbFunc();
		PE_IO_CHECK_CODE(ret, break, "PE_HST_RegisterCbFunc() error..\n");
		ret = PE_WIN_RegisterCbFunc();
		PE_IO_CHECK_CODE(ret, break, "PE_WIN_RegisterCbFunc() error..\n");
		ret = PE_ETC_RegisterCbFunc();
		PE_IO_CHECK_CODE(ret, break, "PE_ETC_RegisterCbFunc() error..\n");
		/* 3. alloc register */
		ret = PE_REG_InitPhyToVirt();
		PE_IO_CHECK_CODE(ret, break, "PE_REG_InitPhyToVirt() error.\n");
		/* 4. init global value */
		memset(&_g_pe_io_inf, 0x0, sizeof(PE_IO_INF_T));
	}while (0);
	PE_PRINT_TRACE("E\n");

	return ret;
}
/**
 * clean up picture enhance module
 *
 * @param   void
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author	sj.youm@lge.com
 */
int PE_IO_CleanupPicEnhanceModule(void)
{
	int ret = RET_OK;
	PE_PRINT_TRACE("S\n");
	do{
		/* 1. free register */
		ret = PE_REG_FreePhyToVirt();
		PE_IO_CHECK_CODE(ret, break, "PE_REG_FreePhyToVirt() error.\n");
	}while (0);
	PE_PRINT_TRACE("E\n");
	return ret;
}
/**
 * open picture enhance module
 *
 * @param   void
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author	sj.youm@lge.com
 */
int PE_IO_OpenPicEnhanceModule(void)
{
	int ret = RET_OK;
	PE_IO_INF_T *io_info = &_g_pe_io_inf;

	/* this func. is called when my_dev->dev_open_count == 0. */
	PE_PRINT_TRACE("S\n");
	do{
		if (io_info->modl_opened>=PE_IO_OPEN__ON)
		{
			PE_PRINT_NOTI("pic_opened was already opened(%d).\n", \
				io_info->modl_opened);
			break;
		}
		ret = PE_CMN_OpenPicEnhanceModule();
		PE_IO_CHECK_CODE(ret, break, \
			"[%s,%d]PE_CMN_OpenPicEnhanceModule() error..\n", __F__, __L__);
		io_info->modl_opened = PE_IO_OPEN__ON;
		io_info->initialized = PE_IO_INIT_OFF;
	}while (0);
	PE_PRINT_TRACE("E\n");

	return ret;
}
/**
 * close picture enhance module
 *
 * @param   void
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author	sj.youm@lge.com
 */
int PE_IO_ClosePicEnhanceModule(void)
{
	int ret = RET_OK;
	PE_IO_INF_T *io_info = &_g_pe_io_inf;

	PE_PRINT_TRACE("S\n");
	do{
		if (io_info->modl_opened<=PE_IO_OPEN_OFF)
		{
			PE_PRINT_NOTI("pic_closed was already closed(%d).\n", \
				io_info->modl_opened);
			break;
		}
		ret = PE_CMN_ClosePicEnhanceModule();
		PE_IO_CHECK_CODE(ret, break, \
			"[%s,%d]PE_CMN_ClosePicEnhanceModule() error..\n", __F__, __L__);
		io_info->modl_opened = PE_IO_OPEN_OFF;
	}while (0);
	PE_PRINT_TRACE("E\n");

	return ret;
}
/**
 * pe io init
 *
 * @param   *pstParams [in] PE_CFG_CTRL_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author	sj.youm@lge.com
 */
int PE_IO_Init(PE_CFG_CTRL_T *pstParams)
{
	int ret = RET_OK;
	PE_IO_INF_T *io_info = &_g_pe_io_inf;
	do{
		CHECK_KNULL(pstParams);
		if (!pstParams->resume_mode && io_info->initialized>=PE_IO_INIT__ON)
		{
			PE_PRINT_NOTI("pe was already initialized(resume:%d,init:%d).\n", \
				pstParams->resume_mode, io_info->initialized);
			break;
		}
		PE_PRINT_NOTI("pe setting init start(resume:%d,init:%d).\n", \
			pstParams->resume_mode, io_info->initialized);
		ret = PE_CMN_Init(pstParams);
		PE_IO_CHECK_CODE(ret, break, \
			"[%s,%d]PE_CMN_Init() error..\n", __F__, __L__);
		ret = PE_CSC_Init(pstParams);
		PE_IO_CHECK_CODE(ret, break, \
			"[%s,%d]PE_CSC_Init() error..\n", __F__, __L__);
		ret = PE_CMG_Init(pstParams);
		PE_IO_CHECK_CODE(ret, break, \
			"[%s,%d]PE_CMG_Init() error..\n", __F__, __L__);
		ret = PE_NRD_Init(pstParams);
		PE_IO_CHECK_CODE(ret, break, \
			"[%s,%d]PE_NRD_Init() error..\n", __F__, __L__);
		ret = PE_DNT_Init(pstParams);
		PE_IO_CHECK_CODE(ret, break, \
			"[%s,%d]PE_DNT_Init() error..\n", __F__, __L__);
		ret = PE_SHP_Init(pstParams);
		PE_IO_CHECK_CODE(ret, break, \
			"[%s,%d]PE_SHP_Init() error..\n", __F__, __L__);
		ret = PE_CCM_Init(pstParams);
		PE_IO_CHECK_CODE(ret, break, \
			"[%s,%d]PE_CCM_Init() error..\n", __F__, __L__);
		ret = PE_DCM_Init(pstParams);
		PE_IO_CHECK_CODE(ret, break, \
			"[%s,%d]PE_DCM_Init() error..\n", __F__, __L__);
		ret = PE_HST_Init(pstParams);
		PE_IO_CHECK_CODE(ret, break, \
			"[%s,%d]PE_HST_Init() error..\n", __F__, __L__);
		ret = PE_WIN_Init(pstParams);
		PE_IO_CHECK_CODE(ret, break, \
			"[%s,%d]PE_WIN_Init() error..\n", __F__, __L__);
		/* etc init should be the last one */
		ret = PE_ETC_Init(pstParams);
		PE_IO_CHECK_CODE(ret, break, \
			"[%s,%d]PE_ETC_Init() error..\n", __F__, __L__);
		io_info->initialized=PE_IO_INIT__ON;
		PE_PRINT_NOTI("pe setting init done.\n");
	}while (0);
	return ret;
}
/**
 * pe io init on resume
 *
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author	sj.youm@lge.com
 */
int PE_IO_ResumeInit(void)
{
	int ret = RET_OK;
	PE_CFG_CTRL_T init_param;
	PE_PRINT_NOTI("pe setting resume init.\n");
	init_param.resume_mode = 1;		//resume on
	PE_IO_OpenPicEnhanceModule();
	PE_IO_Init(&init_param);
	return ret;
}

