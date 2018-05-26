/*
	SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
	Copyright(c) 2013 by LG Electronics Inc.

	This program is free software; you can redistribute it and/or 
	modify it under the terms of the GNU General Public License
	version 2 as published by the Free Software Foundation.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of 
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
	GNU General Public License for more details.
*/ 


/** @file
 *
 *  ipc interface file for BE device
 *	
 *  author		justine.jeong
 *  version		1.0
 *  date		2012.03.05
 *  note		Additional information.
 *
 *  @addtogroup BE
 *	@{
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include <linux/sched.h>
#include <linux/wait.h>
#include <linux/semaphore.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/version.h>
#include <asm/uaccess.h>

#include "os_util.h"
#include "be_dbg.h"

#include "be_kapi.h"
#include "be_def.h"
#include "be_reg.h"
#include "ipc_def.h"
#include "ipc_drv.h"
#include "ipc_hal.h"
#include "int_hal.h"
#include "pwm_hal.h"
#include "be_top_hal.h"
#ifdef MCU_IPC_DEBUG
#include "mcu/mcu_ipc_test.h"
#endif

#include "../de/hal/ipc_hal.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
#undef  USE_FWI_CPU_IPC_WKQ_SND
#define USE_FWI_CPU_IPC_WKQ_RCV
#define FWI_IPC_TRY_CNT 100
#define FWI_IPC_WAIT_TIME_MSEC  10
#define FWI_IPC_TRY_CNT_FOR_JPG 10
#define FWI_IPC_TRY_CNT_FOR_USB 10

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define	FWI_IPC_LOCK()			do { spin_lock_irqsave(&_g_fwi_ipc_lock, flags);	  } while(0)
#define FWI_IPC_UNLOCK()			do { spin_unlock_irqrestore(&_g_fwi_ipc_lock, flags); } while(0)

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/
#if defined(USE_FWI_CPU_IPC_WKQ_SND) || defined(USE_FWI_CPU_IPC_WKQ_RCV)
typedef struct {
	struct workqueue_struct *wrq;
#ifdef USE_FWI_CPU_IPC_WKQ_SND
	struct work_struct wrkSnd;
#endif
#ifdef USE_FWI_CPU_IPC_WKQ_RCV
	struct work_struct wrkRcv;
#endif
	OS_SEM_T mtxSnd;
} FWI_WORKQUEUE_T;
#endif

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/
static int FWI_CPU_Send2MCU(void);
#ifdef USE_FWI_CPU_IPC_WKQ_RCV
static void FWI_IPC_RcvMsg(struct work_struct *work);
#endif

int FWI_CPU_PutData(char *msgBuff, UINT32 msgCnt);
int FWI_CPU_GetData(char *msgBuff, UINT32 msgCnt);
int FWI_CPU_PutStr(char *msgBuff);
int FWI_IPC_CopyStr2Queue(FWI_EXCHANGE_T *pIpcQue, char *pBuff);
int FWI_IPC_CopyStr(char *pdBuff, char *psBuff);
int FWI_IPC_PutQue2IPC(char *pBuff, FWI_EXCHANGE_T *pIpcQue, FWI_IPC_STATUS_T *pStatus);
int FWI_CPU_WakeUpIPC(void);
BOOLEAN FWI_IPC_Gets(char *pBuff, UINT32 *pLen, FWI_EXCHANGE_T *pIpcQue);
void FWI_CPU_ISR_FROM_MCU(unsigned long temp);
void FWI_CPU_ACK_FROM_MCU(unsigned long temp);
void FWI_CPU_WDG_FROM_MCU(unsigned long temp);
int FWI_WorkQueue_Init(void);
int FWI_WorkQueue_Destory(void);

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,39)
static	spinlock_t		_g_fwi_ipc_lock = SPIN_LOCK_UNLOCKED;
#else
static	DEFINE_SPINLOCK(_g_fwi_ipc_lock);
#endif

#if defined(USE_FWI_CPU_IPC_WKQ_SND) || defined(USE_FWI_CPU_IPC_WKQ_RCV)
static FWI_WORKQUEUE_T gwrqBe;
#endif

static char *gpIPCBuffToMCU;
static char *gpIPCBuffFrMCU;

static DECLARE_WAIT_QUEUE_HEAD(wqAckFrMCU);
static DECLARE_WAIT_QUEUE_HEAD(wqIpcFrMCU);
static DECLARE_WAIT_QUEUE_HEAD(wqIpcForJPG);
static DECLARE_WAIT_QUEUE_HEAD(wqIpcForUSB);
static DEFINE_SPINLOCK(spinWDog);

static FWI_EXCHANGE_T stBeIpcQueToMCU;
static FWI_EXCHANGE_T stBeIpcQueFrMCU;

//static char saBeIpcSendBuff[FWI_MSG_STR_MAX];
static char saBeIpcRecvBuff[MSG_STR_MAX];

static SINT32 sWDogCount = -1;
static int fwi_wakeup_send2mcu_timeout;
static int fwi_wakeup_recv4mcu_timeout;

OS_SEM_T	_g_fwi_ipc_snd_sem;
OS_SEM_T	_g_fwi_ipc_rcv_sem;

struct ipc_handle *h_mcu1_ipc = NULL;

/*========================================================================================
	Implementation Group
========================================================================================*/
/**
 * @callgraph
 * @callergraph
 *
 * @brief initialize IPC
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int BE_IPC_Init(void)
{
	int ret = RET_OK;
	UINT32 addrIPCBuffToMCU = 0;
	UINT32 addrIPCBuffFrMCU = 0;

	do {
		h_mcu1_ipc = ipc_hal_open("be", MCU_ID_1);
		if(!h_mcu1_ipc)
		{
			ret = RET_ERROR;
			break;
		}
		ipc_hal_start(1, TRUE);

		OS_InitMutex(&_g_fwi_ipc_snd_sem, OS_SEM_ATTR_DEFAULT);
		OS_InitMutex(&_g_fwi_ipc_rcv_sem, OS_SEM_ATTR_DEFAULT);
		ret = BE_IPC_HAL_Init(&addrIPCBuffToMCU, &addrIPCBuffFrMCU);
		gpIPCBuffToMCU = (char *)addrIPCBuffToMCU;
		gpIPCBuffFrMCU = (char *)addrIPCBuffFrMCU;
#ifdef MCU_IPC_DEBUG
		MCU_IPC_Init(&addrIPCBuffToMCU, &addrIPCBuffFrMCU);
#endif
	} while (0);

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief uninitialize IPC
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int BE_IPC_Free(void)
{
	int ret = RET_OK;
	UINT32 addrIPCBuffToMCU = 0;
	UINT32 addrIPCBuffFrMCU = 0;

	do {
		ipc_hal_close(h_mcu1_ipc);

		addrIPCBuffToMCU = (UINT32)gpIPCBuffToMCU;
		addrIPCBuffFrMCU = (UINT32)gpIPCBuffFrMCU;

		ret = BE_IPC_HAL_Close(&addrIPCBuffToMCU, &addrIPCBuffFrMCU);
		if (ret) break;
		
		gpIPCBuffToMCU = (char *)addrIPCBuffToMCU;
		gpIPCBuffFrMCU = (char *)addrIPCBuffFrMCU;
	} while (0);

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief Send command to MCU via IPC
 *
 * @param ipcCmd [IN] command for IPC
 * @param pMsg [IN] kernel buffer pointer to delever Message to MCU
 * @param msgCnt [IN] Bytes to transfer to MCU
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int BE_MCU_SetData(UINT32 ipcCmd, void *pMsg, UINT32 msgCnt)
{
	int ret = RET_OK;
	FWI_IPC_MSG_T ipcMsgBuff;

	do {
		ipcMsgBuff.u16From = FWI_IPC_FROM_HOST;
		ipcMsgBuff.u16Type = FWI_IPC_CMD_VAL;
		ipcMsgBuff.u32Cmd = ipcCmd;
		ipcMsgBuff.u32Data = 0x0;
		if (msgCnt > 0)
		{
			CHECK_NULL(pMsg);

			if ((msgCnt > sizeof(ipcMsgBuff.u32Data)) || (!memcpy(&ipcMsgBuff.u32Data, pMsg, msgCnt)))
			{
				BREAK_WRONG(msgCnt);
			}
		}

		BE_DEBUG("%s:%d:%s, IPC CMD length(%d)  data = [0x%x, 0x%x, 0x%x, 0x%x]\n", 
			KBUILD_BASENAME, __LINE__, __FUNCTION__, sizeof(ipcMsgBuff), ipcMsgBuff.u16From, ipcMsgBuff.u16Type, ipcMsgBuff.u32Cmd, ipcMsgBuff.u32Data);
		ret = FWI_CPU_PutData((char *)&ipcMsgBuff, sizeof(ipcMsgBuff));
	} while (0);

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief Receive Message from MCU via IPC
 *
 * @param pMsg [OUT] kernel buffer pointer to delever Message from MCU
 * @param msgCnt [IN] Bytes to get from MCU
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int BE_MCU_GetData(UINT32 mcu_id, void *pMsg, UINT32 msgCnt)
{
	int ret = RET_OK;

	do {
        	CHECK_NULL(pMsg);
		ret = OS_LockMutex(&_g_fwi_ipc_rcv_sem);
        	if (ret) BREAK_WRONG(ret);
		if ( msgCnt != FWI_CPU_GetData((char *)pMsg, msgCnt) ) BREAK_WRONG(msgCnt);
	} while (0);
	OS_UnlockMutex(&_g_fwi_ipc_rcv_sem);

	return ret;
}



/**
 * @callgraph
 * @callergraph
 *
 * @brief Send command to MCU via IPC
 *
 * @param ipcCmd [IN] command for IPC
 * @param pMsg [IN] kernel buffer pointer to delever Message to MCU
 * @param msgCnt [IN] Bytes to transfer to MCU
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int BE_KIPC_SetData(UINT32 ipcCmd, void *pMsg, UINT32 msgCnt)
{
	int ret = RET_OK;
	ret = h_mcu1_ipc->m_ipc_client->write(ipcCmd, pMsg, msgCnt);
	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief Receive Message from MCU via IPC
 *
 * @param pMsg [OUT] kernel buffer pointer to delever Message from MCU
 * @param msgCnt [IN] Bytes to get from MCU
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int BE_KIPC_GetData(void *pMsg, UINT32 msgCnt)
{
	int ret = RET_OK;
	ret = h_mcu1_ipc->m_ipc_client->read(1/*mcu_id=1*/, pMsg, msgCnt);
	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief Send command to MCU via IPC
 *
 * @param ipcCmd [IN] command for IPC
 * @param pMsg [IN] pointer to delever Message to MCU
 * @param msgCnt [IN] Bytes to transfer to MCU
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int FWI_IPC_SetData(UINT32 ipcCmd, void *pMsg, UINT32 msgCnt)
{
	int ret = RET_OK;
	FWI_IPC_MSG_T ipcMsgBuff;

	BE_TRACE("Begin");
	
	do {
		ipcMsgBuff.u16From = FWI_IPC_FROM_HOST;
		ipcMsgBuff.u16Type = FWI_IPC_CMD_VAL;
		ipcMsgBuff.u32Cmd = ipcCmd;
		ipcMsgBuff.u32Data = 0x0;
		if (msgCnt > 0)
		{
			CHECK_NULL(pMsg);

			if ((msgCnt > sizeof(ipcMsgBuff.u32Data)) || (copy_from_user(&ipcMsgBuff.u32Data, (void __user *)pMsg, msgCnt)))
			{
				BREAK_WRONG(msgCnt);
			}
		}
		BE_TRACE("Enter");
		BE_DEBUG("%s:%d:%s, IPC CMD length(%d)  data = [0x%x, 0x%x, 0x%x, 0x%x]\n", 
			KBUILD_BASENAME, __LINE__, __FUNCTION__, sizeof(ipcMsgBuff), ipcMsgBuff.u16From, ipcMsgBuff.u16Type, ipcMsgBuff.u32Cmd, ipcMsgBuff.u32Data);
		ret = FWI_CPU_PutData((char *)&ipcMsgBuff, sizeof(ipcMsgBuff));
		BE_TRACE("Exit");
	} while (0);
	BE_TRACE("End");

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief Receive Message from MCU via IPC
 *
 * @param pMsg [OUT] pointer to delever Message to MCU
 * @param msgCnt [IN] Bytes to get from MCU
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int FWI_IPC_GetData(void *pMsg, UINT32 msgCnt)
{
	int ret = RET_OK;
	char *plBuff = NULL;

	do {
		CHECK_NULL(pMsg);
		ret = OS_LockMutex(&_g_fwi_ipc_rcv_sem);
		if (ret) BREAK_WRONG(ret);
		plBuff = OS_Malloc(msgCnt);
       	CHECK_NULL(plBuff);
		if ( msgCnt != FWI_CPU_GetData(plBuff, msgCnt) ) BREAK_WRONG(msgCnt);
		
		ret = copy_to_user((void __user *)pMsg, plBuff, msgCnt);
        if (ret) BREAK_WRONG(msgCnt);
	} while (0);
	if (plBuff) OS_Free(plBuff);
	OS_UnlockMutex(&_g_fwi_ipc_rcv_sem);

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief Send CLI from CPU to MCU via IPC
 *
 * @param pMsg [IN] pointer to delever Message to MCU
 * @param msgCnt [IN] Bytes to transfer to MCU
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int FWI_IPC_SetStr(void *pMsg, UINT32 msgCnt)
{
	int ret = RET_ERROR;
	FWI_IPC_MSG_T ipcMsgBuff;

	do {
		ipcMsgBuff.u16From = FWI_IPC_FROM_HOST;
		ipcMsgBuff.u16Type = FWI_IPC_CMD_VAL;
		ipcMsgBuff.u32Cmd = FWI_IPC_CMD_CLI;
		ipcMsgBuff.u32Data = 0x0;
		if (msgCnt > 0)
		{
			CHECK_NULL(pMsg);

			if ((msgCnt > sizeof(ipcMsgBuff.u32Data)) || (copy_from_user(&ipcMsgBuff.u32Data, (void __user *)pMsg, msgCnt)))
			{
				BREAK_WRONG(msgCnt);
			}
		}
		ret = FWI_CPU_PutStr((char *)&ipcMsgBuff);
	} while (0);

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief Put string of cli to queue of IPC
 *
 * @param msgBuff [IN] message buffer pointer
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int FWI_CPU_PutStr(char *msgBuff)
{
    int ret = RET_ERROR;

	ret = OS_LockMutex(&_g_fwi_ipc_snd_sem);
    do {
        	CHECK_NULL(msgBuff);
		ret = FWI_IPC_CopyStr2Queue(&stBeIpcQueToMCU, msgBuff);
		if (ret) break;
		ret = FWI_CPU_Send2MCU();
    } while (0);
	OS_UnlockMutex(&_g_fwi_ipc_snd_sem);

    return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief Put data to queue of IPC
 *
 * @param msgBuff [IN] data buffer pointer
 * @param msgCnt [IN] bytes to copy to msgBuff
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int FWI_CPU_PutData(char *msgBuff, UINT32 msgCnt)
{
    int ret = RET_ERROR;

	BE_TRACE("lock _g_fwi_ipc_snd_sem");
	ret = OS_LockMutex(&_g_fwi_ipc_snd_sem);
    do {
        	CHECK_NULL(msgBuff);
		
		BE_TRACE("Enter");
		ret = FWI_IPC_CopyStr2Queue(&stBeIpcQueToMCU, msgBuff);
		BE_TRACE("Exit");
		if (ret) break;
		BE_TRACE("Enter");
		ret = FWI_CPU_Send2MCU();
		BE_TRACE("Exit");
    } while (0);
	OS_UnlockMutex(&_g_fwi_ipc_snd_sem);
	BE_TRACE("unlock _g_fwi_ipc_snd_sem");

    return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief  Copy data to IPC queue
 *
 * @param pIpcQue [OUT] pointer of IPC Queue
 * @param pBuff [IN] pointer of data
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int FWI_IPC_CopyStr2Queue(FWI_EXCHANGE_T *pIpcQue, char *pBuff)
{
	int ret = RET_OK;
	UINT8 *pStr;
	int num;
	unsigned long flags;

	BE_TRACE("Begin");
	do {
        	CHECK_NULL(pBuff);

       	FWI_IPC_LOCK();
		num = pIpcQue->wInx - pIpcQue->rInx;
        	FWI_IPC_UNLOCK();
		if (num<0) num += IPC_BUFF_NUM;
		if (num>(IPC_BUFF_NUM-2)) {
			pIpcQue->queOverflow++;
			BE_ERROR("pIpcQue Buffer Overflow\n");
			ret = RET_ERROR;
			break;
		}
		if (pIpcQue->queMax < (num+1)) pIpcQue->queMax = (num+1);

		pStr = pIpcQue->str[pIpcQue->wInx];
		BE_TRACE("Enter");
		ret = FWI_IPC_CopyStr(pStr, pBuff);
		BE_TRACE("Exit");
		if (ret) break;
        	FWI_IPC_LOCK();
        	pIpcQue->wInx++;
		if (pIpcQue->wInx >= IPC_BUFF_NUM) pIpcQue->wInx = 0;
        	FWI_IPC_UNLOCK();
	} while (0);
	BE_TRACE("End");

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief Copy between buffers
 *
 * @param pdBuff [OUT] destination buffer pointer
 * @param psBuff [IN] source buffer pointer
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int FWI_IPC_CopyStr(char *pdBuff, char *psBuff)
{
	int ret = RET_OK;
	FWI_IPC_MSG_T *ptrDst = (FWI_IPC_MSG_T *)pdBuff;
	FWI_IPC_MSG_T *ptrSrc = (FWI_IPC_MSG_T *)psBuff;

	do {
		*ptrDst = *ptrSrc;
	} while (0);

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief Send Messag from IPC Queue to MCU memory for IPC which is located DDR memory
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
static int FWI_CPU_Send2MCU(void)
{
	int ret;
	FWI_IPC_STATUS_T ipcStatus;
	UINT32 tryCount;
	BOOLEAN sendingMesg2MCU = TRUE;

	BE_TRACE("Begin");
	tryCount = 0;
	do {
		BE_TRACE("Enter");
		ret = FWI_IPC_PutQue2IPC(gpIPCBuffToMCU, &stBeIpcQueToMCU, &ipcStatus);
		BE_TRACE("Exit");
		if (ret) break;
		switch (ipcStatus) {
			case FWI_IPC_LAST :
			case FWI_IPC_OK :
				fwi_wakeup_send2mcu_timeout = 1;
				#ifdef MCU_IPC_DEBUG
				MCU_ISR_FROM_CPU();
				#else
				FWI_CPU_WakeUpIPC();
				#endif
				tryCount = 0;
				// don't insert break;
			case FWI_IPC_BUSY :
				wait_event_interruptible_timeout(wqAckFrMCU, !fwi_wakeup_send2mcu_timeout, 1);
				if (fwi_wakeup_send2mcu_timeout) { tryCount++; break; }
				if (FWI_IPC_LAST != ipcStatus) break;
				// don't insert break;
			default :
				sendingMesg2MCU = FALSE;
				break;
		}
		if (tryCount > FWI_IPC_TRY_CNT) BREAK_WRONG(tryCount);
	} while (sendingMesg2MCU);
	
	if (sendingMesg2MCU) 
	{ 
		fwi_wakeup_send2mcu_timeout = 0;
	}
	BE_TRACE("End");

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief Copy data form IPC queue to memory for IPC
 *
 * @param pBuff [OUT] buffer pointer for IPC which is located in DDR memory
 * @param pIpcQue [IN] buffer pointer for IPC Queue
 * @param pStatus [OUT] status to indicate transmition from IPC Queue to IPC memory
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int FWI_IPC_PutQue2IPC(char *pBuff, FWI_EXCHANGE_T *pIpcQue, FWI_IPC_STATUS_T *pStatus)
{
	int ret = RET_OK;
	UINT8 *pStr;
	unsigned long flags;

	BE_TRACE("Begin");
	*pStatus = FWI_IPC_OK;
	do {
       	CHECK_NULL(pBuff);
        	CHECK_NULL(pIpcQue);

		if(fwi_wakeup_send2mcu_timeout) {
			*pStatus = FWI_IPC_BUSY;
			break;
		}
		
        	FWI_IPC_LOCK();
		if (pIpcQue->wInx == pIpcQue->rInx) {
			*pStatus = FWI_IPC_EMPTY;
           		FWI_IPC_UNLOCK();
			break;
		}
        	FWI_IPC_UNLOCK();
		
		pStr = pIpcQue->str[pIpcQue->rInx];
        	FWI_IPC_LOCK();
		pIpcQue->rInx++;
		if (pIpcQue->rInx >= IPC_BUFF_NUM) pIpcQue->rInx = 0;
        	FWI_IPC_UNLOCK();
		
		*pStatus = FWI_IPC_LAST;
		
		BE_TRACE("Enter");
		ret = FWI_IPC_CopyStr(pBuff, pStr);
		if (ret) break;
		BE_TRACE("Exit");
	} while (0);
	BE_TRACE("End");

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief make IPC interrupt of MCU be happen
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int FWI_CPU_WakeUpIPC(void)
{
	int ret = RET_OK;

	ret = BE_INT_HAL_WakeUpReg(INT_CPU_MCU_IPC, TRUE);

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief make DAM interrupt of MCU be happen
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int FWI_CPU_WakeUpDMA(void)
{
	return RET_OK;//FWI_IPC_WakeUpReg(FWI_DMA_MCU, TRUE);
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief make JPG interrupt of MCU be happen
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int FWI_CPU_WakeUpJPG(void)
{
	return RET_OK;//FWI_IPC_WakeUpReg(FWI_JPG_MCU, TRUE);
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief Get data from queue of IPC
 *
 * @param msgBuff [OUT] data buffer pointer
 *
 * @return length of transfered data
 */
int FWI_CPU_GetData(char *msgBuff, UINT32 msgCnt)
{
	UINT32  totalLen;
    	BOOLEAN isCompleted;

	totalLen = 0;
    do {
		isCompleted = FWI_IPC_Gets(msgBuff, &totalLen, &stBeIpcQueFrMCU);
		if (totalLen != msgCnt || !isCompleted) {
			BE_ERROR("Received(%d) is different from Request(%d)\n", totalLen, msgCnt);
			totalLen = 0;
			break;
		}
    } while (0);

	return totalLen;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief Get data from IPC queue
 *
 * @param pBuff [OUT] buffer pointer to get data from IPC queue
 * @param pLen [OUT] Bytes to be copied
 * @param pIpcQue [IN] buffer pointer of IPC Queue
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
BOOLEAN FWI_IPC_Gets(char *pBuff, UINT32 *pLen, FWI_EXCHANGE_T *pIpcQue)
{
	BOOLEAN isCompleted = FALSE;
	UINT32  rdSize;
	FWI_IPC_MSG_T *ipcMsg;

	*pLen = 0;
	do {
		if (pIpcQue->wInx == pIpcQue->rInx) break;
		ipcMsg = (FWI_IPC_MSG_T *)(pIpcQue->str[pIpcQue->rInx]);
		rdSize = sizeof(ipcMsg->u32Data);
		if (!rdSize) break;
		memcpy(pBuff, &(ipcMsg->u32Data), rdSize);
		*pLen += rdSize;
		pIpcQue->rInx++;
		if (pIpcQue->rInx >= IPC_BUFF_NUM) pIpcQue->rInx = 0;
		
		isCompleted = TRUE;
	} while (0);

	return isCompleted;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief interrupt IPC handler to get data from MCU
 *
 * @param temp [IN] dummy argument
 */
void FWI_CPU_ISR_FROM_MCU(unsigned long temp)
{
	FWI_IPC_MSG_T *ipcMsgFrMcu = (FWI_IPC_MSG_T *)gpIPCBuffFrMCU;
	
	FWI_IPC_CopyStr2Queue(&stBeIpcQueFrMCU, gpIPCBuffFrMCU);
	
	switch (ipcMsgFrMcu->u16Type) {
		case FWI_IPC_CMD_VAL :
			fwi_wakeup_recv4mcu_timeout = 0;
			wake_up_interruptible(&wqIpcFrMCU);
			break;
		case FWI_IPC_CMD_CLI :
		default :
			queue_work(gwrqBe.wrq, &gwrqBe.wrkRcv);
			break;
	} 
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief interrupt ACK handler to get data from MCU
 *
 * @param temp [IN] dummy argument
 */
void FWI_CPU_ACK_FROM_MCU(unsigned long temp)
{
	FWI_IPC_MSG_T *ipcMsgFrMcu = (FWI_IPC_MSG_T *)gpIPCBuffFrMCU;

	switch (ipcMsgFrMcu->u16Type) {
		case FWI_IPC_CMD_SYNC :
			BE_PWM_HAL_SetPwmSync(ipcMsgFrMcu->u32Data);
			break;
		case FWI_IPC_CMD_DBG :
			if(ipcMsgFrMcu->u32Cmd == FWI_IPC_CMD_DBG_RESET)
			{
				BE_TOP_HAL_ResetSW();
			}
			else
			{
				BE_TOP_HAL_SetUartForMcu(FALSE);
			}
			break;
		case FWI_IPC_CMD_RET :
			FWI_IPC_CopyStr2Queue(&stBeIpcQueFrMCU, gpIPCBuffFrMCU);
		case FWI_IPC_CMD_VAL :
		default :
			fwi_wakeup_send2mcu_timeout = 0;
			wake_up_interruptible(&wqAckFrMCU);
			break;
	}
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief interrupt handler to get event about watch dog from MCU
 *
 * @param temp [IN] dummy argument
 */
void FWI_CPU_WDG_FROM_MCU(unsigned long temp)
{
	unsigned long flags;

	spin_lock_irqsave(&spinWDog, flags);
	sWDogCount = 0;
	spin_unlock_irqrestore(&spinWDog, flags);
	
	BE_ERROR("CPU received Wachdog Interrupt from DE MCU\n");
}

int FWI_WorkQueue_Init(void)
{
#if defined(USE_FWI_CPU_IPC_WKQ_SND) || defined(USE_FWI_CPU_IPC_WKQ_RCV)
	gwrqBe.wrq = create_workqueue("FWI_IPC_SEND_RCV");

	if(!gwrqBe.wrq) {
		BE_ERROR("create work queue failed");
		return RET_ERROR;
	}

#ifdef USE_FWI_CPU_IPC_WKQ_RCV
	INIT_WORK(&gwrqBe.wrkRcv, FWI_IPC_RcvMsg);
#endif
#endif

#ifdef USE_FWI_TEST_MCU_IN_CPU
	MCU_TEST_Init();
#endif

	return RET_OK;
}

int FWI_WorkQueue_Destory(void)
{
#if defined(USE_FWI_CPU_IPC_WKQ_SND) || defined(USE_FWI_CPU_IPC_WKQ_RCV)
	destroy_workqueue(gwrqBe.wrq);
#endif
#ifdef USE_FWI_TEST_MCU_IN_CPU
	MCU_TEST_Destory();
#endif

	return RET_OK;
}

#ifdef USE_FWI_CPU_IPC_WKQ_RCV
/**
 * @callgraph
 * @callergraph
 *
 * @brief Receive Messag from IPC Queue for IPC memory which is located DDR memory
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
void FWI_IPC_RcvMsg(struct work_struct *work)
{
	static UINT32 totalLen = 0;
	UINT32 checkLen;
	BOOLEAN isCompleted;
	UINT32 readLen;
	char readBuf[IPC_STR_MAX];

	do {
		isCompleted = FWI_IPC_Gets(readBuf, &readLen, &stBeIpcQueFrMCU);
		if (!readLen) break;
		checkLen = totalLen + readLen;
		if (MSG_STR_MAX <= checkLen) {
			BE_ERROR("Received(%d) is over than Max(%d)\n", checkLen, MSG_STR_MAX);
			totalLen = 0;
			break;
		}
		memcpy(&saBeIpcRecvBuff[totalLen], readBuf, readLen);
		totalLen = checkLen;
		if (!isCompleted) break;
		totalLen = 0;
	} while (0);
}
#endif

