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

/** @file pe_tsk_hw_h15.c
 *
 *  task func. for picture enhance. ( used only within kdriver )
 *
 *	@author		Seung-Jun,Youm(sj.youm@lge.com)
 *	@version	0.1
 *	@note
 *	@date		2012.04.12
 *	@see
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/semaphore.h>
#include <linux/interrupt.h>
#include <linux/kthread.h>
#include <asm/io.h>

#include "base_types.h"
#include "os_util.h"
#include "debug_util.h"

#include "pe_hw_h15.h"
#include "pe_tsk_h15.h"
#include "pe_fwi_h15.h"
#include "pe_win_hw_h15.h"
#include "pe_shp_hw_h15.h"
#include "pe_ccm_hw_h15.h"
#include "pe_dcm_hw_h15.h"
#include "pe_hst_hw_h15.h"
#include "pe_cmn_hw_h15.h"
#include "pe_dnt_hw_h15.h"
#include "pe_nrd_hw_h15.h"
#include "pe_csc_hw_h15.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
/* define for using picture sync handler */
/* 20140526, sj.youm, undef for bringup */
#define PE_TSK_H15_USE_PIC_SYNC_HANDLER

/* 20140114, sj.youm, define for picture sync handler on fast mode(120hz) */
/* see de_reg_h15.c, mem3_sync_period, mem_sync_period, 0x192D50 (0x64B540(30Hz) -> 0x192D50(120Hz)) */
/* 20140526, sj.youm, undef for bringup */
#undef PE_TSK_H15_PIC_SYNC_ON_FAST_MODE

#define PE_TSK_H15_TIME_START		(0)
#define PE_TSK_H15_TIME_END			(1)
#define PE_TSK_H15_HDLR_MIN_MSEC	(10)

/* handler count loop max(0~10003) */
#define PE_TSK_H15_HDL_CNT_LOOP_MAX		(10003)

/* check skip handler count number (120hz:0x3, 60hz:0x1, 30hz:0x0)*/
#define PE_TSK_H15_CHK_SKIP_HDL_CNT_NUM		(0x1)

/* check increase handler count number(120hz:0x3f, 60hz:0x1f, 30hz:0xf) */
#ifdef PE_TSK_H15_PIC_SYNC_ON_FAST_MODE
#define PE_TSK_H15_CHK_INC_HDL_CNT_NUM		(0x3f)
#else
#define PE_TSK_H15_CHK_INC_HDL_CNT_NUM		(0x1f)
#endif

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define PE_TSK_H15_ERROR	PE_PRINT_ERROR
#define PE_TSK_H15_PRINT	printk

#define PE_TSK_H15_DBG_PRINT(fmt,args...)	\
	if(_g_pe_tsk_h15_trace) printk("[%x,%x][%s,%d] "fmt,PE_CHIP_VER,g_pe_kdrv_ver_mask,__F__,__L__,##args)
#define PE_TSK_H15_CHECK_CODE(_checker,_action,fmt,args...)	\
	{if(_checker){PE_TSK_H15_ERROR(fmt,##args);_action;}}

#define PE_TSK_H15_SyncRegister		de_sync_register

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/
/**
 *	pe task elapsed time info type
 */
typedef struct
{
	UINT32 tick_start_time[PE_TSK_H15_TIME_MAX];	///< start time
	UINT32 tick_end_time[PE_TSK_H15_TIME_MAX];		///< end time
}
PE_TSK_H15_TIME_INFO_T;

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Functions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/
static int PE_TSK_H15_CheckTime(char *fn_str, PE_TSK_H15_TIME location, UINT32 pos);
static int PE_TSK_H15_PicTask(void *data);
static int PE_TSK_H15_PicSyncHandler(void);
static int PE_TSK_H15_PicPeriodicProcess(PE_TSK_H15_CFG_T *pCfg);

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
static DECLARE_WAIT_QUEUE_HEAD(_wq_pe_h15);
static struct task_struct *g_pstPeTaskH15 = NULL;
static OS_SEM_T	_g_pe_tsk_h15_sem;
static UINT32 _g_pe_tsk_h15_trace=0x0;		//default should be off.
static PE_TSK_H15_CFG_T _g_pe_tsk_h15_cfg;
static UINT32 _g_pe_tsk_h15_started=0x0;	//started value for resume

/*========================================================================================
	Implementation Group
========================================================================================*/
/**
 * debug setting
 *
 * @param   *pstParams [in] LX_PE_DBG_SETTINGS_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_TSK_H15_SetDebugSettings(LX_PE_DBG_SETTINGS_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		PE_TSK_H15_DBG_PRINT("wid:%d,type:0x%x,[0x%x]print:0x%x,bypass:0x%x\n",\
			pstParams->win_id,pstParams->type,(0x1<<PE_ITEM_PKTMODL(ETC)),\
			pstParams->print_lvl,pstParams->bypass);
		/* set print level */
		if(pstParams->type&LX_PE_DBG_LV)
		{
			_g_pe_tsk_h15_trace = (pstParams->print_lvl & (0x1<<PE_ITEM_PKTMODL(ETC)))? 0x1:0x0;
		}
	}while(0);
	return ret;
}
/**
 * set task operation
 *
 * @param   *pstParams [in] LX_PE_ETC_TBL_T
 * @return  void
 * @see     PE_TSK_H15_OPR_T
 * @author
 */
int PE_TSK_H15_SetTaskOperation(LX_PE_ETC_TBL_T *pstParams)
{
    int ret = RET_OK;
	PE_TSK_H15_OPR_T *pOpr = &_g_pe_tsk_h15_cfg.opr;
	do {
		CHECK_KNULL(pstParams);
		PE_TSK_H15_CHECK_CODE((sizeof(UINT32)*pstParams->size)!=sizeof(PE_TSK_H15_OPR_T),\
			break,"[%s,%d] size mismatch.\n",__F__,__L__);
		if((pOpr->pause_task==PE_TSK_H15_RUN_STOP_TASK) && \
			(pstParams->data[0]==PE_TSK_H15_RUN_START))
		{
			pOpr->pause_task=PE_TSK_H15_RUN_START;
			wake_up_interruptible(&_wq_pe_h15);
			PE_PRINT_NOTI("re-start pic_task.\n");
		}
		memcpy(pOpr,&pstParams->data[0],sizeof(PE_TSK_H15_OPR_T));
		PE_TSK_H15_DBG_PRINT("set : pause:%d, time:%d, dbg_md:%d\n", \
			pOpr->pause_task, pOpr->timeout, pOpr->dbgmode);
	} while(0);
	return ret;
}
/**
 * get task operation
 *
 * @param   *pstParams [in/out] LX_PE_ETC_TBL_T
 * @return  void
 * @see     PE_TSK_H15_OPR_T
 * @author
 */
int PE_TSK_H15_GetTaskOperation(LX_PE_ETC_TBL_T *pstParams)
{
	int ret = RET_OK;
	PE_TSK_H15_OPR_T *pOpr = &_g_pe_tsk_h15_cfg.opr;
	do{
		CHECK_KNULL(pstParams);
		PE_TSK_H15_CHECK_CODE((sizeof(UINT32)*pstParams->size)!=sizeof(PE_TSK_H15_OPR_T),\
			break,"[%s,%d] size mismatch.\n",__F__,__L__);
		memcpy(&pstParams->data[0],pOpr,sizeof(PE_TSK_H15_OPR_T));
		PE_TSK_H15_DBG_PRINT("get : pause:%d, time:%d, dbg_md:%d\n", \
			pOpr->pause_task, pOpr->timeout, pOpr->dbgmode);
	}while(0);
	return ret;
}
/**
 * create pe h15 task
 *
 * @param   void
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_TSK_H15_CreatePicTask(void)
{
	int ret = RET_OK;
	do{
		if(PE_KDRV_VER_H15)
		{
			if(g_pstPeTaskH15)
			{
				PE_PRINT_NOTI("pic_task is already created.\n");
				ret = RET_OK;
				break;
			}
			g_pstPeTaskH15 = kthread_create(PE_TSK_H15_PicTask, (void *)NULL, "pe_thread_h15");
			if(g_pstPeTaskH15)
			{
				PE_PRINT_NOTI("pic_task is created successfully.\n");
			}
			else
			{
				PE_PRINT_NOTI("pic_task creation fail.\n");
				ret = RET_ERROR;
				break;
			}
		}
		else
		{
			PE_PRINT_NOTI("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * start pe h15 task
 *
 * @param   *pstParams [in] PE_CFG_CTRL_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_TSK_H15_StartPicTask(PE_CFG_CTRL_T *pstParams)
{
	int ret = RET_OK;
	int hdlr_num;
	PE_TSK_H15_CFG_T *pCfg = &_g_pe_tsk_h15_cfg;
	do{
		CHECK_KNULL(pstParams);
		OS_InitMutex(&_g_pe_tsk_h15_sem, OS_SEM_ATTR_DEFAULT);
		if(PE_KDRV_VER_H15)
		{
			if(g_pstPeTaskH15)
			{
				if (!_g_pe_tsk_h15_started)
				{
					memset0(_g_pe_tsk_h15_cfg);
					pCfg->opr.pause_task = PE_TSK_H15_RUN_START;
					pCfg->opr.dbgmode = PE_TSK_H15_DBG_OFF;
					if (PE_KDRV_VER_H15)
					{
						#ifdef PE_TSK_H15_USE_PIC_SYNC_HANDLER
						pCfg->opr.timeout = 1000;
						if(pCfg->hdlr_opr.is_registered == FALSE)
						{
							hdlr_num = PE_TSK_H15_SyncRegister(TRUE, \
								PE_TSK_H15_PicSyncHandler, TO_STR(PE_TSK_H15_PicSyncHandler));
							PE_PRINT_NOTI("sync_hadler number : %d\n", hdlr_num);
							pCfg->hdlr_opr.is_registered = TRUE;
						}
						wake_up_process(g_pstPeTaskH15);
						PE_PRINT_NOTI("pic_task and sync_hadler start success.\n");
						#else
						pCfg->opr.timeout = 33;
						pCfg->hdlr_opr.is_registered = FALSE;
						wake_up_process(g_pstPeTaskH15);
						PE_PRINT_NOTI("pic_task start success.\n");
						#endif
					}
					else
					{
						pCfg->opr.timeout = 1000;
						pCfg->hdlr_opr.is_registered = FALSE;
						PE_PRINT_NOTI("not support pic_task for this rev.\n");
					}
					_g_pe_tsk_h15_started = 1;
				}
				else
				{
					PE_PRINT_NOTI("already pic_task and sync_hadler start.\n");
				}
			}
			else
			{
				PE_PRINT_NOTI("start fail. pic_task is not created.\n");
				ret = RET_ERROR;
				break;
			}
		}
		else
		{
			PE_PRINT_NOTI("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * stop pe h15 task
 *
 * @param   void
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_TSK_H15_StopPicTask(void)
{
	int ret = RET_OK;
	PE_TSK_H15_CFG_T *pCfg = &_g_pe_tsk_h15_cfg;
	do{
		if(PE_KDRV_VER_H15)
		{
			if(g_pstPeTaskH15)
			{
				kthread_stop(g_pstPeTaskH15);
				g_pstPeTaskH15 = NULL;
				PE_PRINT_NOTI("pic_task stopped.\n");
				if(pCfg->hdlr_opr.is_registered == TRUE)
				{
					ret = PE_TSK_H15_SyncRegister(FALSE, \
						PE_TSK_H15_PicSyncHandler, TO_STR(PE_TSK_H15_PicSyncHandler));
					PE_TSK_H15_CHECK_CODE(ret, break, \
						"[%s,%d] de_sync_register error.\n", __F__, __L__);
					pCfg->hdlr_opr.is_registered = FALSE;
					PE_PRINT_NOTI("sync_hadler unregistered.\n");
				}
			}
			else
			{
				PE_PRINT_NOTI("stop fail. pic_task is not created.\n");
				ret = RET_ERROR;
				break;
			}
		}
		else
		{
			PE_PRINT_NOTI("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
static int PE_TSK_H15_CheckTime(char *fn_str, PE_TSK_H15_TIME location, UINT32 pos)
{
	static PE_TSK_H15_TIME_INFO_T time_info;
	UINT32 time_diff = 0;
	switch(pos)
	{
		case PE_TSK_H15_TIME_START:
			time_info.tick_start_time[location] = OS_GetMsecTicks();
			break;
		case PE_TSK_H15_TIME_END:
		default:
			time_info.tick_end_time[location] = OS_GetMsecTicks();
			if(time_info.tick_end_time[location] >= time_info.tick_start_time[location])
				time_diff = time_info.tick_end_time[location] - time_info.tick_start_time[location];
			else
				time_diff = time_info.tick_start_time[location] + (0xFFFFFFFF - time_info.tick_end_time[location]);
			PE_TSK_H15_PRINT("%s :	%d msec  elapsed\n", fn_str, (time_diff==0)?1:time_diff);
			break;
	}
	return RET_OK;
}
/**
 * pe picture task
 *
 * @param   *data [in] void
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_TSK_H15_PicTask(void *data)
{
	static UINT32 task_cnt = 0;
	PE_TSK_H15_CFG_T *pCfg = &_g_pe_tsk_h15_cfg;
	PE_TSK_H15_OPR_T *pOpr = &_g_pe_tsk_h15_cfg.opr;
	static UINT32 hdlr_cnt = 0;
	PE_PRINT_NOTI("pic_task start.\n");
	while(1)
	{
		/* check stop condition when device is closed */
		if(kthread_should_stop())
		{
			PE_PRINT_NOTI("pic_task exit.\n");
			break;
		}
		if(pOpr->dbgmode==PE_TSK_H15_DBG_TIME)
		{
			PE_TSK_H15_CheckTime("[pic_task]", \
				PE_TSK_H15_TIME_TASK, PE_TSK_H15_TIME_START);
		}
		/* check pic sync handler registered. */
		if (pCfg->hdlr_opr.is_registered == TRUE)
		{
			if(pOpr->dbgmode==PE_TSK_H15_DBG_ON)
			{
				PE_TSK_H15_PRINT("[pic_task]hdlr_opr.count : %d -> %d (%s)\n", \
					hdlr_cnt, pCfg->hdlr_opr.count, \
					(hdlr_cnt == pCfg->hdlr_opr.count)? "not increased":"normal");
			}
			hdlr_cnt = pCfg->hdlr_opr.count;
		}
		else
		{
			/* picture periodic process */
			PE_TSK_H15_PicPeriodicProcess(pCfg);
		}
		if(pOpr->dbgmode==PE_TSK_H15_DBG_TIME)
		{
			PE_TSK_H15_CheckTime("[pic_task]", \
				PE_TSK_H15_TIME_TASK, PE_TSK_H15_TIME_END);
		}
		if(pOpr->dbgmode==PE_TSK_H15_DBG_CNT)
		{
			PE_TSK_H15_PRINT("[pic_task]task_cnt :%d\n", task_cnt);
		}
		if(task_cnt > 10000)
		{
			task_cnt = 1;
		}
		else
		{
			task_cnt ++;
		}
		/**********************************************/
		OS_MsecSleep(pOpr->timeout);
		if (pOpr->pause_task == PE_TSK_H15_RUN_STOP_TASK)
		{
			PE_PRINT_NOTI("pic_task is paused.\n");
			wait_event_interruptible(_wq_pe_h15, (pOpr->pause_task != PE_TSK_H15_RUN_STOP_TASK));
		}
	}
	return RET_OK;
}
/**
 * pe picture sync handler
 *
 * @param   data [in] void
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_TSK_H15_PicSyncHandler(void)
{
	PE_TSK_H15_CFG_T *pCfg = &_g_pe_tsk_h15_cfg;
	PE_TSK_H15_OPR_T *pOpr = &_g_pe_tsk_h15_cfg.opr;
	do {
		if (pCfg->hdlr_opr.is_registered == FALSE)
			break;
		if (pOpr->pause_task == PE_TSK_H15_RUN_STOP_HDLR)
			break;

		if(pOpr->dbgmode==PE_TSK_H15_DBG_TIME)
		{
			PE_TSK_H15_CheckTime("[pic_sync_handler]", \
				PE_TSK_H15_TIME_HNDL, PE_TSK_H15_TIME_START);
		}

		/* picture periodic process */
		PE_TSK_H15_PicPeriodicProcess(pCfg);

		if(pOpr->dbgmode==PE_TSK_H15_DBG_TIME)
		{
			PE_TSK_H15_CheckTime("[pic_sync_handler]", \
				PE_TSK_H15_TIME_HNDL, PE_TSK_H15_TIME_END);
		}
		if(pOpr->dbgmode==PE_TSK_H15_DBG_CNT)
		{
			PE_TSK_H15_PRINT("[pic_sync_handler]hdlr_opr.count :%d\n", \
				pCfg->hdlr_opr.count);
		}

		if(pCfg->hdlr_opr.count >= PE_TSK_H15_HDL_CNT_LOOP_MAX)
		{
			pCfg->hdlr_opr.count = 0;
		}
		else
		{
			pCfg->hdlr_opr.count ++;
		}
	}while(0);
	return RET_OK;
}
/**
 * pe picture periodic process
 *	- causion : never use kmalloc,ipc,ioremap, on pic_sync_handler
 *
 * @param   *pCfg [in] PE_TSK_H15_CFG_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_TSK_H15_PicPeriodicProcess(PE_TSK_H15_CFG_T *pCfg)
{
	int ret = RET_OK;
	PE_TSK_H15_OPR_T *p_opr = &(pCfg->opr);
	PE_TSK_H15_HDLR_OPR_T *p_hdlr_opr = &(pCfg->hdlr_opr);
	#ifdef PE_TSK_H15_PIC_SYNC_ON_FAST_MODE
	LX_PE_INF_DISPLAY_T *p_disp0_info = &(pCfg->inf.disp_info[LX_PE_WIN_0]);
	#endif
	do {
		CHECK_KNULL(pCfg);
		/**********************************************/
		/* add specific func.                         */
		/**********************************************/
		if (p_hdlr_opr->is_registered == TRUE)
		{
			p_hdlr_opr->cur_time = OS_GetMsecTicks();
			p_hdlr_opr->time_dif = (p_hdlr_opr->cur_time - p_hdlr_opr->pre_time);
			if (((p_hdlr_opr->count&PE_TSK_H15_CHK_SKIP_HDL_CNT_NUM)!=PE_TSK_H15_CHK_SKIP_HDL_CNT_NUM) || \
				(p_hdlr_opr->time_dif<=PE_TSK_H15_HDLR_MIN_MSEC))
			{
				if(p_opr->dbgmode==PE_TSK_H15_DBG_BRK)
				{
					PE_TSK_H15_PRINT("[%s,%d]cnt:%d(%d). T c,p:%d,%d(d:%d), run nothing.\n", \
						__F__, __L__, pCfg->hdlr_opr.count, \
						(pCfg->hdlr_opr.count&PE_TSK_H15_CHK_SKIP_HDL_CNT_NUM), \
						p_hdlr_opr->cur_time, p_hdlr_opr->pre_time, p_hdlr_opr->time_dif);
				}
				p_hdlr_opr->pre_time = p_hdlr_opr->cur_time;
				break;
			}
			else
			{
				if(p_opr->dbgmode==PE_TSK_H15_DBG_BRK)
				{
					PE_TSK_H15_PRINT("[%s,%d]cnt:%d(%d). T c,p:%d,%d(d:%d), run all.\n", \
						__F__, __L__, pCfg->hdlr_opr.count, \
						(pCfg->hdlr_opr.count&PE_TSK_H15_CHK_SKIP_HDL_CNT_NUM), \
						p_hdlr_opr->cur_time, p_hdlr_opr->pre_time, p_hdlr_opr->time_dif);
				}
				p_hdlr_opr->pre_time = p_hdlr_opr->cur_time;
			}
		}
		/* check info */
		ret = PE_INF_H15_CheckSclInfo();
		PE_TSK_H15_CHECK_CODE(ret, break, \
			"[%s,%d] PE_INF_H15_CheckSclInfo() error.\n", __F__, __L__);
		/* get info */
		ret = PE_INF_H15_GetCurInfSettings(&(pCfg->inf));
		PE_TSK_H15_CHECK_CODE(ret, break, \
			"[%s,%d] PE_INF_H15_GetCurInfSettings() error.\n", __F__, __L__);
		if (p_hdlr_opr->is_registered == TRUE)
		{
			#ifdef PE_TSK_H15_PIC_SYNC_ON_FAST_MODE
			if ((p_hdlr_opr->count&PE_TSK_H15_CHK_SKIP_HDL_CNT_NUM)==PE_TSK_H15_CHK_SKIP_HDL_CNT_NUM)
			{
				if(p_opr->dbgmode==PE_TSK_H15_DBG_BRK)
				{
					PE_TSK_H15_PRINT("[%s,%d]src:%d,dtv:%d, count :%d(%d). run all routine.\n", __F__, __L__, \
						p_disp0_info->src_type, p_disp0_info->dtv_type, pCfg->hdlr_opr.count, \
						(pCfg->hdlr_opr.count&PE_TSK_H15_CHK_SKIP_HDL_CNT_NUM));
				}
				/* set scaler filter */
				ret = PE_SHP_HW_H15_SetScalerFilterCtrl(pCfg);
				PE_TSK_H15_CHECK_CODE(ret, break, \
					"[%s,%d] PE_SHP_HW_H15_SetScalerFilterCtrl() error.\n", __F__, __L__);
			}
			else
			{
				if (p_disp0_info->src_type==LX_PE_SRC_DTV && p_disp0_info->dtv_type==LX_PE_DTV_FILEPLAY)
				{
					if(p_opr->dbgmode==PE_TSK_H15_DBG_BRK)
					{
						PE_TSK_H15_PRINT("[%s,%d]src:%d,dtv:%d, count :%d(%d). run scaler only(file).\n", __F__, __L__, \
							p_disp0_info->src_type, p_disp0_info->dtv_type, pCfg->hdlr_opr.count, \
							(pCfg->hdlr_opr.count&PE_TSK_H15_CHK_SKIP_HDL_CNT_NUM));
					}
					/* set scaler filter */
					ret = PE_SHP_HW_H15_SetScalerFilterCtrl(pCfg);
					PE_TSK_H15_CHECK_CODE(ret, break, \
						"[%s,%d] PE_SHP_HW_H15_SetScalerFilterCtrl() error.\n", __F__, __L__);
				}
				else
				{
					if(p_opr->dbgmode==PE_TSK_H15_DBG_BRK)
					{
						PE_TSK_H15_PRINT("[%s,%d]src:%d,dtv:%d, count :%d(%d). run nothing.\n", __F__, __L__, \
							p_disp0_info->src_type, p_disp0_info->dtv_type, pCfg->hdlr_opr.count, \
							(pCfg->hdlr_opr.count&PE_TSK_H15_CHK_SKIP_HDL_CNT_NUM));
					}
				}
				break;
			}
			#else
			/* set scaler filter */
			ret = PE_SHP_HW_H15_SetScalerFilterCtrl(pCfg);
			PE_TSK_H15_CHECK_CODE(ret, break, \
				"[%s,%d] PE_SHP_HW_H15_SetScalerFilterCtrl() error.\n", __F__, __L__);
			#endif
		}
		else
		{
			/* set scaler filter */
			ret = PE_SHP_HW_H15_SetScalerFilterCtrl(pCfg);
			PE_TSK_H15_CHECK_CODE(ret, break, \
				"[%s,%d] PE_SHP_HW_H15_SetScalerFilterCtrl() error.\n", __F__, __L__);
		}
		/* set picture window */
		ret = PE_WIN_HW_H15_SetEnhancingWindow(pCfg);
		PE_TSK_H15_CHECK_CODE(ret, break, \
			"[%s,%d] PE_WIN_HW_H15_SetEnhancingWindow() error.\n", __F__, __L__);
		/* set cmn operation */
		ret = PE_CMN_HW_H15_SetOperationMode(pCfg);
		PE_TSK_H15_CHECK_CODE(ret, break, \
			"[%s,%d] PE_CMN_HW_H15_SetOperationMode() error.\n", __F__, __L__);
		/* set lrcr config, after window setting */
		ret = PE_CCM_HW_H15_SetLrcrConfig(pCfg);
		PE_TSK_H15_CHECK_CODE(ret, break, \
			"[%s,%d] PE_CCM_HW_H15_SetLrcrConfig() error.\n", __F__, __L__);
		/* set acr gain */
		ret = PE_CCM_HW_H15_SetAutoCrGain(pCfg);
		PE_TSK_H15_CHECK_CODE(ret, break, \
			"[%s,%d] PE_CCM_HW_H15_SetAutoCrGain() error.\n", __F__, __L__);
		/* check global histo info */
		ret = PE_HST_HW_H15_CheckGlobalHistoInfo(pCfg);
		PE_TSK_H15_CHECK_CODE(ret, break, \
			"[%s,%d] PE_HST_HW_H15_CheckGlobalHistoInfo() error.\n", __F__, __L__);
		/* set lrcr window, after lrcr config and global histo */
		ret = PE_WIN_HW_H15_SetLrcrWindow(pCfg);
		PE_TSK_H15_CHECK_CODE(ret, break, \
			"[%s,%d] PE_WIN_HW_H15_SetLrcrWindow() error.\n", __F__, __L__);
		/* set pq fw ctrl enable and set shp on split mode */
		ret = PE_FWI_H15_SetFwCtrlEnable(pCfg);
		PE_TSK_H15_CHECK_CODE(ret, break, \
			"[%s,%d] PE_FWI_H15_SetFwCtrlEnable() error.\n", __F__, __L__);
		/* set blur_v_gain 0xf(protection) for pattern */
		ret = PE_DCM_HW_H15_SetBlurGain(pCfg);
		PE_TSK_H15_CHECK_CODE(ret, break, \
			"[%s,%d] PE_DCM_HW_H15_SetBlurGain() error.\n", __F__, __L__);

		if (p_opr->pause_task == PE_TSK_H15_RUN_STOP_OVWR)
			break;

		/* from here, reg. overwrite functions */
		/* check fw status */
		ret = PE_FWI_H15A_CheckFwStatus(pCfg);
		PE_TSK_H15_CHECK_CODE(ret, break, \
			"[%s,%d] PE_FWI_H15A_CheckFwStatus() error.\n", __F__, __L__);
		/* set tnr mode */
		ret = PE_NRD_HW_H15_SetTnrMode(pCfg);
		PE_TSK_H15_CHECK_CODE(ret, break, \
			"[%s,%d] PE_NRD_HW_H15_SetTnrMode() error.\n", __F__, __L__);
		/* set dnr mode */
		ret = PE_NRD_HW_H15_SetDnrMode(pCfg);
		PE_TSK_H15_CHECK_CODE(ret, break, \
			"[%s,%d] PE_NRD_HW_H15_SetDnrMode() error.\n", __F__, __L__);
		/* set ifc mode */
		ret = PE_NRD_HW_H15_SetIfcMode(pCfg);
		PE_TSK_H15_CHECK_CODE(ret, break, \
			"[%s,%d] PE_NRD_HW_H15_SetIfcMode() error.\n", __F__, __L__);
		/* set clc mode */
		PE_DNT_HW_H15_SetClcMode(pCfg);
		PE_TSK_H15_CHECK_CODE(ret, break, \
			"[%s,%d] PE_DNT_HW_H15_SetClcMode() error.\n", __F__, __L__);
		/* set pblur apply */
		ret = PE_SHP_HW_H15A_SetPblApply(pCfg);
		PE_TSK_H15_CHECK_CODE(ret, break, \
			"[%s,%d] PE_SHP_HW_H15A_SetPblApply() error.\n", __F__, __L__);
		/* set csc mode */
		ret = PE_CSC_HW_H15_SetCscMode(pCfg);
		PE_TSK_H15_CHECK_CODE(ret, break, \
			"[%s,%d] PE_CSC_HW_H15_SetCscMode() error.\n", __F__, __L__);
		/* set lvctrl mode */
		ret = PE_CMN_HW_H15_SetLevelCtrlMode(pCfg);
		PE_TSK_H15_CHECK_CODE(ret, break, \
			"[%s,%d] PE_CMN_HW_H15_SetLevelCtrlMode() error.\n", __F__, __L__);
		/**********************************************/
	}while(0);
	return ret;
}
