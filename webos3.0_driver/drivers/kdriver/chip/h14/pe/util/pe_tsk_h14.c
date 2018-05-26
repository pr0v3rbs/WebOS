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

/** @file pe_tsk_hw_h14.c
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

#include "pe_hw_h14.h"
#include "pe_tsk_h14.h"
#include "pe_fwi_h14.h"
#include "pe_win_hw_h14.h"
#include "pe_shp_hw_h14.h"
#include "pe_dcm_hw_h14.h"
#include "pe_ccm_hw_h14.h"
#include "pe_hst_hw_h14.h"
#include "pe_cmn_hw_h14.h"
#include "pe_dnt_hw_h14.h"


/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
#define TSK_H14_START 0
#define TSK_H14_END 1

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define PE_TSK_H14_ERROR	PE_PRINT_ERROR
#define PE_TSK_H14_PRINT	printk

#define PE_TSK_H14_DBG_PRINT(fmt,args...)	\
	if(_g_pe_tsk_h14_trace) printk("[%x,%x][%s,%d] "fmt,PE_CHIP_VER,g_pe_kdrv_ver_mask,__F__,__L__,##args)
#define PE_TSK_H14_CHECK_CODE(_checker,_action,fmt,args...)	\
	{if(_checker){PE_TSK_H14_ERROR(fmt,##args);_action;}}

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/
/**
 *	pe task debug mode
 */
typedef enum
{
	PE_TSK_H14_DBG_OFF = 0,		///< off
	PE_TSK_H14_DBG_ON,			///< on
	PE_TSK_H14_DBG_TIME,		///< time check
	PE_TSK_H14_DBG_CNT,			///< count check
	PE_TSK_H14_DBG_MAX			///< max num
}
PE_TSK_H14_OPR_DBG_MODE;

/**
 *	pe task time mode
 */
typedef enum
{
	PE_TSK_H14_TIME_TASK = 0,	///< task time(all)
	PE_TSK_H14_TIME_SCL,		///< scaler time
	PE_TSK_H14_TIME_MAX			///< max num
}
PE_TSK_H14_TIME;

/**
 *	pe task elapsed time info type
 */
typedef struct
{
	UINT32 tick_start_time[PE_TSK_H14_TIME_MAX];	///< start time
	UINT32 tick_end_time[PE_TSK_H14_TIME_MAX];		///< end time
}
PE_TSK_H14_TIME_INFO_T;

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
static int PE_TSK_H14_CheckTime(char *fn_str, PE_TSK_H14_TIME location, UINT32 pos);
static int PE_TSK_H14_PicTask(void *data);

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
static DECLARE_WAIT_QUEUE_HEAD(_wq_pe_h14);
static OS_SEM_T	_g_pe_tsk_h14_sem;
static UINT32 _g_pe_tsk_h14_trace=0x0;		//default should be off.
static struct task_struct *g_pstPeTaskH14 = NULL;
static PE_TSK_H14_CFG_T _g_pe_tsk_h14_cfg;

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
int PE_TSK_H14_SetDebugSettings(LX_PE_DBG_SETTINGS_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		PE_TSK_H14_DBG_PRINT("wid:%d,type:0x%x,[0x%x]print:0x%x,bypass:0x%x\n",\
			pstParams->win_id,pstParams->type,(0x1<<PE_ITEM_PKTMODL(ETC)),\
			pstParams->print_lvl,pstParams->bypass);
		/* set print level */
		if(pstParams->type&LX_PE_DBG_LV)
		{
			_g_pe_tsk_h14_trace = (pstParams->print_lvl & (0x1<<PE_ITEM_PKTMODL(ETC)))? 0x1:0x0;
		}
	}while(0);
	return ret;
}
/**
 * set task operation
 *
 * @param   *pstParams [in] LX_PE_ETC_TBL_T
 * @return  void
 * @see     PE_TSK_H14_OPR_T
 * @author
 */
int PE_TSK_H14_SetTaskOperation(LX_PE_ETC_TBL_T *pstParams)
{
    int ret = RET_OK;
	PE_TSK_H14_OPR_T *pOpr = &_g_pe_tsk_h14_cfg.opr;
	do {
		CHECK_KNULL(pstParams);
		PE_TSK_H14_CHECK_CODE((sizeof(UINT32)*pstParams->size)!=sizeof(PE_TSK_H14_OPR_T),\
			break,"[%s,%d] size mismatch.\n",__F__,__L__);
		if(pOpr->pause_task&&!pstParams->data[0])
		{
			pOpr->pause_task=0;
			wake_up_interruptible(&_wq_pe_h14);
			PE_TSK_H14_DBG_PRINT("re-start pic task.\n");
		}
		memcpy(pOpr,&pstParams->data[0],sizeof(PE_TSK_H14_OPR_T));
		PE_TSK_H14_DBG_PRINT("set : pause:%d, time:%d, dbg_md:%d\n",\
			pOpr->pause_task,pOpr->timeout,pOpr->dbgmode);
	} while(0);
	return ret;
}
/**
 * get task operation
 *
 * @param   *pstParams [in/out] LX_PE_ETC_TBL_T
 * @return  void
 * @see     PE_TSK_H14_OPR_T
 * @author
 */
int PE_TSK_H14_GetTaskOperation(LX_PE_ETC_TBL_T *pstParams)
{
	int ret = RET_OK;
	PE_TSK_H14_OPR_T *pOpr = &_g_pe_tsk_h14_cfg.opr;
	do{
		CHECK_KNULL(pstParams);
		PE_TSK_H14_CHECK_CODE((sizeof(UINT32)*pstParams->size)!=sizeof(PE_TSK_H14_OPR_T),\
			break,"[%s,%d] size mismatch.\n",__F__,__L__);
		memcpy(&pstParams->data[0],pOpr,sizeof(PE_TSK_H14_OPR_T));
		PE_TSK_H14_DBG_PRINT("get : pause:%d, time:%d\n",pOpr->pause_task,pOpr->timeout);
	}while(0);
	return ret;
}
/**
 * create pe h14 task
 *
 * @param   void
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_TSK_H14_CreatePicTask(void)
{
	int ret = RET_OK;
	do{
		if(PE_KDRV_VER_H14)
		{
			if(g_pstPeTaskH14)
			{
				PE_PRINT_NOTI("pic task is already created.\n");
				ret = RET_OK;
				break;
			}
			g_pstPeTaskH14 = kthread_create(PE_TSK_H14_PicTask, (void *)NULL, "pe_thread_h14");
			if(g_pstPeTaskH14)
			{
				PE_PRINT_NOTI("pic task is created successfully.\n");
			}
			else
			{
				PE_PRINT_NOTI("pic task creation fail.\n");
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
 * start pe h14 task
 *
 * @param   *pstParams [in] PE_CFG_CTRL_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_TSK_H14_StartPicTask(PE_CFG_CTRL_T *pstParams)
{
	int ret = RET_OK;
	PE_TSK_H14_OPR_T *pOpr = &_g_pe_tsk_h14_cfg.opr;
	do{
		CHECK_KNULL(pstParams);
		OS_InitMutex(&_g_pe_tsk_h14_sem, OS_SEM_ATTR_DEFAULT);
		if(PE_KDRV_VER_H14)
		{
			#ifndef PE_INIT_COMMON
			if(g_pstPeTaskH14)
			{
				memset0(_g_pe_tsk_h14_cfg);
				pOpr->pause_task = 0;
				pOpr->timeout = 30;
				wake_up_process(g_pstPeTaskH14);
				PE_PRINT_NOTI("pic task wake up successfully.\n");
			}
			else
			{
				PE_PRINT_NOTI("start fail. pic task is not created.\n");
				ret = RET_ERROR;
				break;
			}
			#endif
		}
		else
		{
			PE_PRINT_NOTI("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * stop pe h14 task
 *
 * @param   void
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_TSK_H14_StopPicTask(void)
{
	int ret = RET_OK;
	do{
		if(PE_KDRV_VER_H14)
		{
			if(g_pstPeTaskH14)
			{
				kthread_stop(g_pstPeTaskH14);
				g_pstPeTaskH14 = NULL;
				PE_PRINT_NOTI("stop pic task.\n");
			}
			else
			{
				PE_PRINT_NOTI("stop fail. pic task is not created.\n");
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
static int PE_TSK_H14_CheckTime(char *fn_str, PE_TSK_H14_TIME location, UINT32 pos)
{
	int ret = RET_OK;
	static PE_TSK_H14_TIME_INFO_T time_info;
	PE_TSK_H14_OPR_T *pOpr = &_g_pe_tsk_h14_cfg.opr;
	UINT32 time_diff;
	do {
		if(pOpr->dbgmode!=PE_TSK_H14_DBG_TIME)
		{
			ret = RET_OK;
			break;
		}
		switch(pos)
		{
			case TSK_H14_START:
				time_info.tick_start_time[location] = OS_GetMsecTicks();
				break;
			case TSK_H14_END:
			default:
				time_info.tick_end_time[location] = OS_GetMsecTicks();
				if(time_info.tick_end_time[location] >= time_info.tick_start_time[location])
					time_diff = time_info.tick_end_time[location] - time_info.tick_start_time[location];
				else
					time_diff = time_info.tick_start_time[location] + (0xFFFFFFFF - time_info.tick_end_time[location]);
				PE_TSK_H14_PRINT("%s :	%d msec  elapsed\n", fn_str, (time_diff==0)?1:time_diff);
				break;
		}
	}while(0);
	return ret;
}
/**
 * pe hw h14 task
 *
 * @param   *data [in] void
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_TSK_H14_PicTask(void *data)
{
	int ret = RET_OK;
	static unsigned int count = 0;
	PE_TSK_H14_CFG_T *pCfg = &_g_pe_tsk_h14_cfg;
	PE_INF_H14_SETTINGS_T *pInfo = &_g_pe_tsk_h14_cfg.inf;
	PE_TSK_H14_OPR_T *pOpr = &_g_pe_tsk_h14_cfg.opr;
	PE_PRINT_NOTI("pic task start.\n");
	while(1)
	{
		/* check stop condition when device is closed */
		if(kthread_should_stop())
		{
			PE_PRINT_NOTI("pic task exit.\n");
			break;
		}
		PE_TSK_H14_CheckTime("picture task", PE_TSK_H14_TIME_TASK, TSK_H14_START);
		/**********************************************/
		/* add specific func.                         */
		/**********************************************/
		/* check info */
		ret = PE_INF_H14_CheckSclInfo();
		/* get info */
		ret = PE_INF_H14_GetCurInfSettings(pInfo);
		/* set picture window */
		ret = PE_WIN_HW_H14_SetEnhancingWindow(pCfg);
		/* set cmn operation */
		ret = PE_CMN_HW_H14_SetOperationMode(pCfg);
		/* set pe tnr measure window */
		ret = PE_WIN_HW_H14_SetTnrMeasureWin(pCfg);
		/* set lrcr config, after window setting */
		ret = PE_CCM_HW_H14_SetLrcrConfig(pCfg);
		/* set scaler filter */
		ret = PE_SHP_HW_H14_SetScalerFilterCtrl(pCfg);
		/* set resolution enable : move from here to window enable control(not peaking en) */
		/*ret = PE_SHP_HW_H14_SetReEnableCtrl(pCfg);*/
		/* set acr gain */
		ret = PE_CCM_HW_H14_SetAutoCrGain(pCfg);
		/* check global histo info */
		ret = PE_HST_HW_H14_CheckGlobalHistoInfo(pCfg);
		/* set lrcr window, after lrcr config and global histo */
		ret = PE_WIN_HW_H14_SetLrcrWindow(pCfg);
		#ifdef PE_FWI_H14_USE_SHP_CTRL_WHEN_PQFW_OFF
		/* set pq fw ctrl enable and set shp on split mode */
		ret = PE_FWI_H14_SetFwCtrlEnable(pCfg);
		#endif
		/* set ipc clc mode */
		ret = PE_DNT_HW_H14_SetIpcClcMode(pCfg);
		/* set gamma,degamma enable on wb block */
		ret = PE_CCM_HW_H14_SetWbGammaDegamma(pCfg);
		#ifdef PE_DCM_HW_H14_SET_BLUR_GAIN_ON_TASK
		/* set blur_v_gain 0xf(protection) for pattern */
		ret = PE_DCM_HW_H14_SetBlurGain(pCfg);
		#endif
		/**********************************************/
		PE_TSK_H14_CheckTime("picture task", PE_TSK_H14_TIME_TASK, TSK_H14_END);
		if(pOpr->dbgmode==PE_TSK_H14_DBG_CNT)
			PE_TSK_H14_PRINT("[%s,%d]count :%d\n",__F__,__L__,count);
		if(count > 10000)	count = 0;
		else				count ++;
		/**********************************************/
		OS_MsecSleep(pOpr->timeout);
		if(pOpr->pause_task)
		{
			PE_PRINT_NOTI("pic task is paused.\n");
			wait_event_interruptible(_wq_pe_h14, (!pOpr->pause_task));
		}
	}
	return 0;
}
