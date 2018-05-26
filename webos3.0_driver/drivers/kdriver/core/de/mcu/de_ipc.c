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
 * main driver implementation for de device.
 * de device will teach you how to make device driver with new platform.
 *
 * author     jaemo.kim (jaemo.kim@lge.com)
 * version    1.0
 * date       2010.02.16
 * note       Additional information.
 *
 * @addtogroup lg1150_de
 * @{
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
#include <asm-generic/errno-base.h>
#include <linux/string.h>
#include <linux/workqueue.h>
#include <linux/version.h>

#include <asm/io.h>

#include "os_util.h"

#include "de_model.h"
#include "de_ver_def.h"

#include "de_kapi.h"
#include "de_def.h"
#include "de_int_def.h"
#include "de_cfg.h"
#include "de_ipc_def.h"
#include "hal/de_hal_def.h"
#include "hal/de_hal.h"
#include "hal/ipc_phys_hal.h"
#include "de_ipc.h"
#ifdef INCLUDE_KDRV_BE
#include "pwm_hal.h"
#endif
#ifdef USE_VIDEO_TEST_MCU_IN_CPU
#include "mcu/mcu_test.h"
#include "mcu/io_video.h"
#endif

/*----------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------*/
#undef  USE_DE_CPU_IPC_WKQ_SND
#define USE_DE_CPU_IPC_WKQ_RCV
#define DE_IPC_TRY_CNT 80
#define DE_IPC_TRY_CNT_THRESHOLD 60
#define DE_IPC_WAIT_TIME_MSEC  10
#define DE_IPC_TRY_CNT_FOR_JPG 10
#define DE_IPC_TRY_CNT_FOR_USB 10
#define DE_IPC_TRY_CNT_FOR_WEL 10
#define DE_IPC_TRY_CNT_FOR_WER 10

/*----------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------*/
#ifdef USE_LINUX_KERNEL
#define	DE_IPC_LOCK()		do {                                      \
							spin_lock_irqsave(&_g_de_ipc_lock, flags);\
							} while(0)
#define DE_IPC_UNLOCK()		do {                                           \
							spin_unlock_irqrestore(&_g_de_ipc_lock, flags);\
							} while(0)
#else
#define	DE_IPC_LOCK()
#define DE_IPC_UNLOCK()
#endif

#ifdef USE_DE_TRACE_IPC_MSG
#define	DE_IPC_DBG_PRT(pMsg, nCnt)	MCU_VIDEO_IPC_PRINT(__FILE__,    \
														__LINE__,    \
														__FUNCTION__,\
														pMsg,        \
														nCnt);
#else
#define	DE_IPC_DBG_PRT(pMsg, nCnt)
#endif
#ifdef USE_DE_TRACE_IPC_CHK
#define	DE_IPC_DBG_CHK(msg)			MCU_VIDEO_IPC_CHECK(__FILE__,    \
														__LINE__,    \
														__FUNCTION__,\
														msg);
#else
#define	DE_IPC_DBG_CHK(msg)
#endif

/*----------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------*/
typedef struct {
	struct workqueue_struct *wrq;
	struct work_struct wrkRcv;
	OS_SEM_T mtxSnd;
} DE_WORKQUEUE_T;

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
static int VIDEO_WorkQueue_Init(UINT32 mcu_id);
static int VIDEO_WorkQueue_Destory(UINT32 mcu_id);
static int VIDEO_CPU_Send2MCU(UINT32 mcu_id);
static void DE_IPC_RcvMsg(struct work_struct *work);
static int VIDEO_IPC_PutLine2Queue(MCU_VIDEO_EXCHANGE_T *pIpcQue,\
                                  char *pStr, VIDEO_IPC_DATA_TYPE_T ipcType);
static int VIDEO_CPU_PutData(UINT32 mcu_id, char *msgBuff, UINT32 msgCnt);
static int VIDEO_IPC_PutData2Queue(MCU_VIDEO_EXCHANGE_T *pIpcQue,\
                                   char *pStr, UINT32 msgCnt);
static int VIDEO_IPC_CopyStr2Queue(MCU_VIDEO_EXCHANGE_T *pIpcQue,\
                                   char *pBuff);
static int VIDEO_IPC_CopyStr(char *pdBuff, char *psBuff);
static int VIDEO_IPC_PutQue2IPC(char *pBuff,                  \
                                MCU_VIDEO_EXCHANGE_T *pIpcQue,\
                                VIDEO_IPC_STATUS_T *pStatus);
static int VIDEO_CPU_GetData(UINT32 mcu_id, char *msgBuff, UINT32 msgCnt);
static BOOLEAN VIDEO_IPC_Gets(UINT32 mcu_id,\
                              char *pBuff, \
                              UINT32 *pLen,\
                              MCU_VIDEO_EXCHANGE_T *pIpcQue);
UINT32 DE_REG_WB(UINT32 addr, char *pData, char *pField);
static int VIDEO_CPU_PutMem(char *msgBuff, UINT32 msgCnt);

/*----------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------*/
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,39)
static	spinlock_t		_g_de_ipc_lock = SPIN_LOCK_UNLOCKED;
#else
static	DEFINE_SPINLOCK(_g_de_ipc_lock);
#endif

static DE_WORKQUEUE_T gwrqDe[SOC_MCU_MAX];

static char *gpVideoBuffToMCU[SOC_MCU_MAX];
static char *gpVideoBuffFrMCU[SOC_MCU_MAX];
static char *gpSharedBuffToMCU;

static DECLARE_WAIT_QUEUE_HEAD(wqAckFrMCU_0);
static DECLARE_WAIT_QUEUE_HEAD(wqAckFrMCU_1);
static DECLARE_WAIT_QUEUE_HEAD(wqIpcFrMCU_0);
static DECLARE_WAIT_QUEUE_HEAD(wqIpcFrMCU_1);
static DECLARE_WAIT_QUEUE_HEAD(wqIpcForJPG);
static DECLARE_WAIT_QUEUE_HEAD(wqIpcForUSB);
static DECLARE_WAIT_QUEUE_HEAD(wqIpcForWEL);
static DECLARE_WAIT_QUEUE_HEAD(wqIpcForWER);
static DEFINE_SPINLOCK(spinWDog_0);
static DEFINE_SPINLOCK(spinWDog_1);

static MCU_VIDEO_EXCHANGE_T saIpcQueToMCU[SOC_MCU_MAX];
static MCU_VIDEO_EXCHANGE_T saIpcQueFrMCU[SOC_MCU_MAX];

//static char saSendBuff[VIDEO_MSG_STR_MAX];
static char saRecvBuff[VIDEO_MSG_STR_MAX];

static SINT32 sWDogCount = -1;
static int de_wakeup_send2mcu_timeout[SOC_MCU_MAX];
static int de_wakeup_recv4mcu_timeout[SOC_MCU_MAX];
static int de_wakeup_recv4jpg_timeout;
static int de_wakeup_recv4usb_timeout;
static int de_wakeup_recv4wel_timeout;
static int de_wakeup_recv4wer_timeout;

OS_SEM_T	_g_de_ipc_snd_sem[SOC_MCU_MAX];
OS_SEM_T	_g_de_ipc_rcv_sem[SOC_MCU_MAX];

/*============================================================================
 *   Implementation Group
 *==========================================================================*/
/**
 * @callgraph
 * @callergraph
 *
 * @brief initialize IPC
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_IPC_Init(void)
{
	int ret = RET_OK;
	int i;
	UINT32 addrVideoBuffToMCU = 0;
	UINT32 addrVideoBuffFrMCU = 0;

	do {
		for(i=0;i<SOC_MCU_MAX;i++)
		{
			VIDEO_WorkQueue_Init(i);

			OS_InitMutex(&_g_de_ipc_snd_sem[i], OS_SEM_ATTR_DEFAULT);
			OS_InitMutex(&_g_de_ipc_rcv_sem[i], OS_SEM_ATTR_DEFAULT);

			ret = ipc_phys_hal_IPCInit(i, &addrVideoBuffToMCU, \
                                          &addrVideoBuffFrMCU);
			if (ret) break;
			gpVideoBuffToMCU[i] = (char *)addrVideoBuffToMCU;
			gpVideoBuffFrMCU[i] = (char *)addrVideoBuffFrMCU;
		}
		switch(lx_chip_rev())
		{
			case LX_CHIP_REV(M14,B2):
			case LX_CHIP_REV(M14,B1):
			case LX_CHIP_REV(M14,B0):
				gpSharedBuffToMCU = (char *)ioremap(\
						gMemCfgDeFW[2].fw_base,\
						gMemCfgDeFW[2].fw_size);
				*(UINT32 *)&gpSharedBuffToMCU[0] = gMemCfgDeFW[2].fw_base;
				*(UINT32 *)&gpSharedBuffToMCU[4] = gMemCfgDeFW[2].fw_base;
				break;
			default:
				break;
		}
#ifdef USE_QEMU_SYSTEM
#ifdef USE_VIDEO_TEST_MCU_IN_CPU
		if (ret) break;
		ret = VIDEO_MCU_IPC_Init((UINT32)gpVideoBuffToMCU, \
								(UINT32)gpVideoBuffFrMCU);
#endif
#endif
	} while (0);

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief initialize IPC
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_IPC_Free(void)
{
	int ret = RET_OK;
	int i;
	UINT32 addrVideoBuffToMCU;
	UINT32 addrVideoBuffFrMCU;

	do {
		for(i=0;i<SOC_MCU_MAX;i++)
		{
			VIDEO_WorkQueue_Destory(i);

			addrVideoBuffToMCU = (UINT32)gpVideoBuffToMCU[i];
			addrVideoBuffFrMCU = (UINT32)gpVideoBuffFrMCU[i];
		
			ret = ipc_phys_hal_IPCFree(&addrVideoBuffToMCU, \
                                       &addrVideoBuffFrMCU);
			if (ret) break;

			gpVideoBuffToMCU[i] = (char *)addrVideoBuffToMCU;
			gpVideoBuffFrMCU[i] = (char *)addrVideoBuffFrMCU;
		}
	} while (0);

	return ret;
}

int DE_KIPC_SetData(UINT32 ipcCmd, void *pMsg, UINT32 msgCnt)
{
#define GET_MCU_ID(a) ((a >> 16) & 0xF)

	int ret = RET_OK;
	char *plBuff;
	int cmdSize = sizeof(ipcCmd);
	int ipcSize = cmdSize + msgCnt;
	UINT32 mcu_id = GET_MCU_ID(ipcCmd);

	do {
		if(lx_chip_rev() < LX_CHIP_REV(H14,A0) && \
			lx_chip_rev() >= LX_CHIP_REV(M14,B0)){
				if(mcu_id==1) mcu_id = 0;
		}

		plBuff = OS_Malloc(ipcSize);
        CHECK_KNULL(plBuff);
		if ( !memcpy(plBuff, &ipcCmd, cmdSize) ) 
			BREAK_WRONG(ipcCmd);
		if (msgCnt > 0)
		{
			if ( !memcpy(&plBuff[cmdSize], pMsg, msgCnt) )
				BREAK_WRONG(msgCnt);
		}
		ret = VIDEO_CPU_PutData(mcu_id, plBuff, ipcSize);
	} while (0);
	if (plBuff) OS_Free(plBuff);
	if(ret != RET_OK){
		DE_ERROR("mcu[%d] cmd[ 0x%08x ] cnt[%d] send fail.\n",\
				mcu_id, ipcCmd, msgCnt);
	}

	return ret;
}

int DE_KIPC_GetData(UINT32 mcu_id, void *pMsg, UINT32 msgCnt)
{
	int ret = RET_OK;
	char *plBuff = NULL;

	do {
		if(lx_chip_rev() < LX_CHIP_REV(H14,A0) && \
			lx_chip_rev() >= LX_CHIP_REV(M14,B0)) {
				if(mcu_id==1) mcu_id = 0;
		}

		plBuff = OS_Malloc(msgCnt);
        CHECK_KNULL(plBuff);
		ret = OS_LockMutex(&_g_de_ipc_rcv_sem[mcu_id]);
        if (ret) BREAK_WRONG(ret);
		if ( msgCnt != VIDEO_CPU_GetData(mcu_id, plBuff, msgCnt) ) 
			BREAK_WRONG(msgCnt);
		DE_IPC_DBG_PRT(plBuff, msgCnt);
		if ( !memcpy(pMsg, plBuff, msgCnt) ) 
			BREAK_WRONG(msgCnt);
	} while (0);
	OS_UnlockMutex(&_g_de_ipc_rcv_sem[mcu_id]);
	if (plBuff) OS_Free(plBuff);
	if(ret != RET_OK)
		DE_ERROR("mcu[%d] receive fail. msgCnt[%d]\n", mcu_id, msgCnt);

	return ret;
}

int DE_KIPC_SetMem(UINT32 ipcCmd, void *pMsg, UINT32 msgCnt)
{
	int ret = RET_OK;
	char *plBuff;
	int cmdSize = sizeof(ipcCmd);
	int ipcSize = cmdSize*2 + msgCnt;

	do {
		plBuff = OS_Malloc(ipcSize);
        CHECK_KNULL(plBuff);
		if ( !memcpy(plBuff, &ipcCmd, cmdSize) ) 
			BREAK_WRONG(ipcCmd);
		if (msgCnt > 0)
		{
			if ( !memcpy(&plBuff[cmdSize], &msgCnt, cmdSize) )
				BREAK_WRONG(msgCnt);
			if ( !memcpy(&plBuff[cmdSize*2], pMsg, msgCnt) )
				BREAK_WRONG(msgCnt);
		}
		ret = VIDEO_CPU_PutMem(plBuff, ipcSize);
	} while(0);
	if (plBuff) OS_Free(plBuff);
	
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
int VIDEO_CPU_PutStr(char *msgBuff, VIDEO_IPC_DATA_TYPE_T ipcType)
{
    int ret = RET_ERROR;

	ret = OS_LockMutex(&_g_de_ipc_snd_sem[SOC_MCU_0]);
    do {
        CHECK_KNULL(msgBuff);
		DE_IPC_DBG_PRT(msgBuff, strlen(msgBuff));
		ret = VIDEO_IPC_PutLine2Queue(&saIpcQueToMCU[0], msgBuff, ipcType);
		if (ret) break;
		ret = VIDEO_CPU_Send2MCU(0);
    } while (0);
	OS_UnlockMutex(&_g_de_ipc_snd_sem[SOC_MCU_0]);

    return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief Put String to IPC queue after slicing string as unit size
 *
 * @param pIpcQue [OUT] pointer of IPC queue
 * @param pStr [IN] pointer of CLI
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
static int VIDEO_IPC_PutLine2Queue(MCU_VIDEO_EXCHANGE_T *pIpcQue,\
							char *pStr,                   \
							VIDEO_IPC_DATA_TYPE_T ipcType)
{
	int ret = RET_ERROR;

	int totalLen;
	int totalCnt;
	int readLen;
	char msgBuffIPC[VIDEO_IPC_STR_MAX];

	do {
		totalLen = strlen(pStr)+1;
		DE_IPC_DBG_PRT(pStr, totalLen);
		totalCnt = totalLen/(VIDEO_IPC_STR_MAX - VIDEO_IPC_DATA);
		while (totalLen>0) {
			readLen = (totalLen < (VIDEO_IPC_STR_MAX - VIDEO_IPC_DATA)) ? \
					  totalLen :                                          \
					  (VIDEO_IPC_STR_MAX - VIDEO_IPC_DATA);
			memcpy(&msgBuffIPC[VIDEO_IPC_DATA], pStr, readLen);
			msgBuffIPC[VIDEO_IPC_SIZE] = readLen;
			msgBuffIPC[VIDEO_IPC_CONT] = totalCnt;
			msgBuffIPC[VIDEO_IPC_FROM] = VIDEO_IPC_FROM_HOST;
			msgBuffIPC[VIDEO_IPC_TYPE] = ipcType;
			pStr	 += readLen;
			totalLen -= readLen;
			totalCnt--;
			ret = VIDEO_IPC_CopyStr2Queue(pIpcQue, msgBuffIPC);
			if (ret) break;
		}
	} while (0);

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
static int VIDEO_CPU_PutData(UINT32 mcu_id, char *msgBuff, UINT32 msgCnt)
{
    int ret = RET_ERROR;

	DE_IPC_DBG_CHK("lock _g_de_ipc_snd_sem");
	ret = OS_LockMutex(&_g_de_ipc_snd_sem[mcu_id]);
    do {
        CHECK_KNULL(msgBuff);
		DE_IPC_DBG_PRT(msgBuff, msgCnt);
		DE_IPC_DBG_CHK("Enter");
		ret = VIDEO_IPC_PutData2Queue(&saIpcQueToMCU[mcu_id],\
                                       msgBuff,              \
                                       msgCnt);
		DE_IPC_DBG_CHK("Exit");
		if (ret) break;
		DE_IPC_DBG_CHK("Enter");
		ret = VIDEO_CPU_Send2MCU(mcu_id);
		DE_IPC_DBG_CHK("Exit");
    } while (0);
	OS_UnlockMutex(&_g_de_ipc_snd_sem[mcu_id]);
	DE_IPC_DBG_CHK("unlock _g_de_ipc_snd_sem");

    return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief Put data to shared memory with firmware
 *
 * @param pIpcQue [OUT] pointer of IPC queue
 * @param pStr [IN] pointer of data
 * @param msgCnt [IN] Bytes to copy from string buffer to queue buffer
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
static int VIDEO_CPU_PutMem(char *msgBuff, UINT32 msgCnt)
{
#define PRINTMEM if(0)printk

	int ret = RET_OK;
//	int i = 0, m = 0;

	char *p_new;
	UINT32 wr_ptr;
	UINT32 rd_ptr;
	UINT32 prev_size;
	UINT32 start_addr = gMemCfgDe[2].fw[2].fw_base + 8;
	UINT32 end_addr   = start_addr + (16*1024 - 8);
//	UINT32 end_addr   = start_addr + (VIDEO_M14_FIRMWARE_MEM_SIZE_OF_IPC-8);

	do {
		wr_ptr = *(UINT32 *)&gpSharedBuffToMCU[0];
		rd_ptr = *(UINT32 *)&gpSharedBuffToMCU[4];

		PRINTMEM("1:BOUND[0x%08X-0x%08X] wr/rd[0x%08X/0x%08X]\n", \
				start_addr, end_addr, wr_ptr, rd_ptr);

		prev_size = *(UINT32 *)ioremap(wr_ptr + 4, sizeof(UINT32));
		prev_size += sizeof(UINT32) * 2; // header 8 Byte + payload size
		if((wr_ptr + prev_size) >= rd_ptr)
		{
			if((wr_ptr + prev_size + msgCnt) < end_addr)
				wr_ptr = wr_ptr + prev_size;
			else
				wr_ptr = start_addr;
		}
		else
		{
			if((wr_ptr + prev_size + msgCnt) < rd_ptr)
				wr_ptr = wr_ptr + prev_size;
			else
			{
				PRINTMEM("pended-by-mcu.  expected w/r[0x%08X/0x%08X]\n",\
						wr_ptr + prev_size + msgCnt, rd_ptr);
				break;
			}
		}
		PRINTMEM("2:BOUND[0x%08X-0x%08X] wr/rd[0x%08X/0x%08X]\n", \
				start_addr, end_addr, wr_ptr, rd_ptr);

		p_new = (char *)ioremap(wr_ptr, msgCnt);
		memcpy(p_new, msgBuff, msgCnt);
#if 0
		for(i=0;i<msgCnt;i++)
		{
			PRINTMEM("%02X ", p_new[i]);
			if(m++ == 3) {PRINTMEM(" ");m=0;}
		}
		m = 0;PRINTMEM("\n");
#endif
		iounmap((void*)p_new);
		// update write pointer
		*(UINT32 *)&gpSharedBuffToMCU[0] = wr_ptr;
	} while(0);

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief Put data to IPC queue after slicing data as unit size
 *
 * @param pIpcQue [OUT] pointer of IPC queue
 * @param pStr [IN] pointer of data
 * @param msgCnt [IN] Bytes to copy from string buffer to queue buffer
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
static int VIDEO_IPC_PutData2Queue(MCU_VIDEO_EXCHANGE_T *pIpcQue, \
                                   char *pStr,                    \
                                   UINT32 msgCnt)
{
	int ret = RET_ERROR;

	int totalLen;
	int totalCnt;
	int readLen;
	char msgBuffIPC[VIDEO_IPC_STR_MAX];

	DE_IPC_DBG_CHK("Begin");
	do {
		totalLen = msgCnt;
		totalCnt = totalLen/(VIDEO_IPC_STR_MAX - VIDEO_IPC_DATA);
		while (totalLen>0) {
			readLen = (totalLen < (VIDEO_IPC_STR_MAX - VIDEO_IPC_DATA)) ? \
					  totalLen :                                          \
					  (VIDEO_IPC_STR_MAX - VIDEO_IPC_DATA);
			memcpy(&msgBuffIPC[VIDEO_IPC_DATA], pStr, readLen);
			msgBuffIPC[VIDEO_IPC_SIZE] = readLen;
			msgBuffIPC[VIDEO_IPC_CONT] = totalCnt;
			msgBuffIPC[VIDEO_IPC_FROM] = VIDEO_IPC_FROM_HOST;
			msgBuffIPC[VIDEO_IPC_TYPE] = VIDEO_IPC_CMD_VAL;
			
			pStr	 += readLen;
			totalLen -= readLen;
			totalCnt--;
			DE_IPC_DBG_CHK("Enter");
			ret = VIDEO_IPC_CopyStr2Queue(pIpcQue, msgBuffIPC);
			DE_IPC_DBG_CHK("Exit");
			DE_IPC_DBG_PRT(msgBuffIPC, readLen+4);
			if (ret) break;
		}
	} while (0);
	DE_IPC_DBG_CHK("End");

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
static int VIDEO_IPC_CopyStr2Queue(MCU_VIDEO_EXCHANGE_T *pIpcQue, char *pBuff)
{
	int ret = RET_OK;
	UINT8 *pStr;
	int num;
	unsigned long flags;

	DE_IPC_DBG_CHK("Begin");
	do {
        CHECK_KNULL(pBuff);
		if (!pBuff[VIDEO_IPC_SIZE]) break;

        DE_IPC_LOCK();
		num = pIpcQue->wInx - pIpcQue->rInx;
        DE_IPC_UNLOCK();
		if (num<0) num += VIDEO_IPC_BUFF_NUM;
		if (num>(VIDEO_IPC_BUFF_NUM-2)) {
			pIpcQue->queOverflow++;
			ffprintk("pIpcQue Buffer Overflow\n");
			ret = RET_ERROR;
			break;
		}
		if (pIpcQue->queMax < (num+1)) pIpcQue->queMax = (num+1);

		pStr = pIpcQue->str[pIpcQue->wInx];
		DE_IPC_DBG_CHK("Enter");
		ret = VIDEO_IPC_CopyStr(pStr, pBuff);
		DE_IPC_DBG_CHK("Exit");
		DE_IPC_DBG_PRT(pStr, pStr[0]+4);
		if (ret) break;
        DE_IPC_LOCK();
        pIpcQue->wInx++;
		if (pIpcQue->wInx >= VIDEO_IPC_BUFF_NUM) pIpcQue->wInx = 0;
        DE_IPC_UNLOCK();
	} while (0);
	DE_IPC_DBG_CHK("End");

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
static int VIDEO_IPC_CopyStr(char *pdBuff, char *psBuff)
{
	int ret = RET_OK;
	UINT32 *ptrDst = (UINT32 *)pdBuff;
	UINT32 *ptrSrc = (UINT32 *)psBuff;
	UINT32 ipcSize = psBuff[VIDEO_IPC_SIZE]+(VIDEO_IPC_DATA+3); 
	//     ipcSize = 4(Head) + 3 (4-byte allign)
	int i;

	do {
		ipcSize>>=2;
		if (ipcSize > (VIDEO_IPC_STR_MAX>>2)) {
			ffprintk("psBuff(%d) is over than %d\n", \
					ipcSize, VIDEO_IPC_STR_MAX);
			ret = RET_ERROR;
			break;
		}
		for (i=0;i<ipcSize;i++) *ptrDst++ = *ptrSrc++;
	} while (0);

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief Send Messag from IPC Queue to MCU memory for IPC 
 *        which is located DDR memory
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
static int VIDEO_CPU_Send2MCU(UINT32 mcu_id)
{
	int ret;
	VIDEO_IPC_STATUS_T ipcStatus;
	UINT32 tryCount;
	BOOLEAN sendingMesg2MCU = TRUE;

	DE_IPC_DBG_CHK("Begin");
	tryCount = 0;
	do {
		DE_IPC_DBG_CHK("Enter");
		ret = VIDEO_IPC_PutQue2IPC(	gpVideoBuffToMCU[mcu_id], \
									&saIpcQueToMCU[mcu_id],   \
									&ipcStatus);
		DE_IPC_DBG_CHK("Exit");
		if (ret) break;
		switch (ipcStatus) {
			case VIDEO_IPC_LAST :
			case VIDEO_IPC_OK :
				ipc_phys_hal_WakeUpIPC(mcu_id);
				tryCount = 0;
				// don't insert break;
			case VIDEO_IPC_BUSY :
				de_wakeup_send2mcu_timeout[mcu_id] = 1;
				if(mcu_id == SOC_MCU_0)
				{
					wait_event_interruptible_timeout(   \
							wqAckFrMCU_0,                 \
							!de_wakeup_send2mcu_timeout[SOC_MCU_0],\
							msecs_to_jiffies(DE_IPC_WAIT_TIME_MSEC));
				}
				else if (mcu_id == SOC_MCU_1)
				{
					//  ack from mcu 1
					wait_event_interruptible_timeout(   \
							wqAckFrMCU_1,                 \
							!de_wakeup_send2mcu_timeout[SOC_MCU_1],\
							msecs_to_jiffies(DE_IPC_WAIT_TIME_MSEC));
				}
				if (de_wakeup_send2mcu_timeout[mcu_id]) { tryCount++; break; }
				if (VIDEO_IPC_LAST != ipcStatus) break;
				
				// don't insert break;
			default :
				sendingMesg2MCU = FALSE;
				break;
		}
		if (tryCount > DE_IPC_TRY_CNT_THRESHOLD){
			if(!(tryCount%70)) DE_WARN("mcu %d  is busy.\n", mcu_id);
		}
		if (tryCount > DE_IPC_TRY_CNT) BREAK_WRONG(tryCount);
	} while (sendingMesg2MCU);
	if (sendingMesg2MCU) gpVideoBuffToMCU[mcu_id][VIDEO_IPC_SIZE] = 0;
	DE_IPC_DBG_CHK("End");

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
 * @param pStatus [OUT] status to indicate transmition 
 *                      from IPC Queue to IPC memory
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
static int VIDEO_IPC_PutQue2IPC( char *pBuff,                  \
                                 MCU_VIDEO_EXCHANGE_T *pIpcQue,\
                                 VIDEO_IPC_STATUS_T *pStatus)
{
	int ret = RET_OK;
	UINT8 *pStr;
	unsigned long flags;

	DE_IPC_DBG_CHK("Begin");
	*pStatus = VIDEO_IPC_OK;
	do {
        CHECK_KNULL(pBuff);
        CHECK_KNULL(pIpcQue);
        DE_IPC_LOCK();
		if (pIpcQue->wInx == pIpcQue->rInx) {
			*pStatus = VIDEO_IPC_EMPTY;
            DE_IPC_UNLOCK();
			break;
		}
        DE_IPC_UNLOCK();
		if(pBuff[VIDEO_IPC_SIZE]) {
			*pStatus = VIDEO_IPC_BUSY;
			break;
		}
		pStr = pIpcQue->str[pIpcQue->rInx];
        DE_IPC_LOCK();
		pIpcQue->rInx++;
		if (pIpcQue->rInx >= VIDEO_IPC_BUFF_NUM) pIpcQue->rInx = 0;
        DE_IPC_UNLOCK();
		if (!pStr[VIDEO_IPC_SIZE]) {
			*pStatus = VIDEO_IPC_EMPTY;
			break;
		}
		if (!pStr[VIDEO_IPC_CONT]) *pStatus = VIDEO_IPC_LAST;
		DE_IPC_DBG_PRT(pStr, pStr[0]+4);
		DE_IPC_DBG_CHK("Enter");
		ret = VIDEO_IPC_CopyStr(pBuff, pStr);
		DE_IPC_DBG_CHK("Exit");
	} while (0);
	DE_IPC_DBG_CHK("End");

	return ret;
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
static int VIDEO_CPU_GetData(UINT32 mcu_id, char *msgBuff, UINT32 msgCnt)
{
	UINT32  readLen;
	UINT32  totalLen;
	UINT32  tryCount;
	UINT32  checkLen;
    BOOLEAN isCompleted;
	UINT32  receivedMsg;
	char    readBuf[VIDEO_IPC_STR_MAX];

	readLen  = 0;
	totalLen = 0;
	tryCount = 0;
	checkLen = 0;
	receivedMsg = 0;
    do {
		de_wakeup_recv4mcu_timeout[mcu_id] = 1;
		if(mcu_id == SOC_MCU_0)
		{
			wait_event_interruptible_timeout(wqIpcFrMCU_0,          \
								!de_wakeup_recv4mcu_timeout[mcu_id],\
								msecs_to_jiffies(DE_IPC_WAIT_TIME_MSEC));
		}
		else if (mcu_id == SOC_MCU_1)
		{

		}
		else
		{
			ffprintf("undef_mcu_id [%d]\n", mcu_id);
			break;
		}
		do {
			isCompleted = VIDEO_IPC_Gets(mcu_id, readBuf,  \
                                         &readLen, \
                                         &saIpcQueFrMCU[mcu_id]);
			if (!readLen) break;
			checkLen = totalLen + readLen;
			if (checkLen != msgCnt) {
				ffprintk("Received(%d) is different from Request(%d)\n", \
						checkLen, msgCnt);
				totalLen = 0;
				isCompleted = FALSE;
				receivedMsg++;
				break;
			}
			memcpy(&msgBuff[totalLen], readBuf, readLen);
			totalLen = checkLen;
		} while (!isCompleted);
		if (tryCount > DE_IPC_TRY_CNT_THRESHOLD){
			if(!(tryCount%50)) DE_WARN("mcu %d  is busy.\n", mcu_id);
		}
		if (tryCount++ > DE_IPC_TRY_CNT)
		{
			ffprintk(\
				"Wrong tryCount/GetCnt/GetMsg/ReqMsg(0x%x/0x%x/0x%x/0x%x)\n",\
				tryCount, receivedMsg, checkLen, msgCnt); 
			break; }
    } while (!isCompleted);

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
static BOOLEAN VIDEO_IPC_Gets(UINT32 mcu_id,\
                              char *pBuff,  \
                              UINT32 *pLen, \
                              MCU_VIDEO_EXCHANGE_T *pIpcQue)
{
	BOOLEAN isCompleted = FALSE;
	UINT32  rdSize;
	UINT8 *pStr;

	*pLen = 0;
	do {
		if (pIpcQue->wInx == pIpcQue->rInx) break;
		pStr = pIpcQue->str[pIpcQue->rInx];
		switch(mcu_id)
		{
			case 0:
			default:
				rdSize = pStr[VIDEO_IPC_SIZE];
				break;
			case 1:
				rdSize = 8;
				break;
		}
		if (!rdSize) break;
		if (rdSize > (VIDEO_IPC_STR_MAX-VIDEO_IPC_DATA)) break;
		memcpy(pBuff, &pStr[VIDEO_IPC_DATA], rdSize);
		*pLen += rdSize;
		pIpcQue->rInx++;
		if (pIpcQue->rInx >= VIDEO_IPC_BUFF_NUM) pIpcQue->rInx = 0;
		if(pStr[VIDEO_IPC_CONT]) break;
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
void VIDEO_CPU_ISR_FROM_MCU(unsigned long temp)
{
	UINT32 mcu = SOC_MCU_0;
	char ipc_type;
	char *p = gpVideoBuffFrMCU[mcu];

#ifdef INCLUDE_KDRV_BE
	if (p[VIDEO_IPC_TYPE] == VIDEO_IPC_CMD_SYNC) // FRC frame rate  change -> PWM frequency change
	{
		UINT32 ipc_data;

		ipc_data = *((UINT32 *)&(p[VIDEO_IPC_DATA]));
		BE_PWM_HAL_SetPwmSync(ipc_data);
		p[VIDEO_IPC_SIZE] = 0;
		return;
	}
#endif	
	VIDEO_IPC_CopyStr2Queue(&saIpcQueFrMCU[mcu], p);
	p[VIDEO_IPC_SIZE] = 0;
	ipc_type = p[VIDEO_IPC_TYPE];

	switch (ipc_type) {
		case VIDEO_IPC_CMD_VAL :
			de_wakeup_recv4mcu_timeout[mcu] = 0;
			wake_up_interruptible(&wqIpcFrMCU_0);
			break;
		case VIDEO_IPC_CMD_CLI :
		default :
			queue_work(gwrqDe[mcu].wrq, &gwrqDe[mcu].wrkRcv);
			break;
	}
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief interrupt JPG handler to get data from MCU
 *
 * @param temp [IN] dummy argument
 */
void VIDEO_CPU_JPG_FROM_MCU(unsigned long temp)
{
	de_wakeup_recv4jpg_timeout = 0;
	wake_up_interruptible(&wqIpcForJPG);
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief interrupt USB handler to get data from MCU
 *
 * @param temp [IN] dummy argument
 */
void VIDEO_CPU_USB_FROM_MCU(unsigned long temp)
{
	de_wakeup_recv4usb_timeout = 0;
	wake_up_interruptible(&wqIpcForUSB);
}

/**
 * @callgraph
 * @callergraph

 * @brief interrupt WEL handler to get data from MCU
 *
 * @param temp [IN] dummy argument
 */
void VIDEO_CPU_WEL_FROM_MCU(unsigned long temp)
{
	de_wakeup_recv4wel_timeout = 0;
	wake_up_interruptible(&wqIpcForWEL);
}

/**
 * @callgraph
 * @callergraph

 * @brief interrupt WER handler to get data from MCU
 *
 * @param temp [IN] dummy argument
 */
void VIDEO_CPU_WER_FROM_MCU(unsigned long temp)
{
	de_wakeup_recv4wer_timeout = 0;
	wake_up_interruptible(&wqIpcForWER);
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief interrupt DMA handler to get data from MCU
 *
 * @param temp [IN] dummy argument
 */
void VIDEO_CPU_DMA_FROM_MCU(unsigned long temp)
{
	de_wakeup_send2mcu_timeout[SOC_MCU_0] = 0;
	wake_up_interruptible(&wqAckFrMCU_0);
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief interrupt handler to get event about watch dog from MCU
 *
 * @param temp [IN] dummy argument
 */
void VIDEO_CPU_WDG_FROM_MCU(unsigned long temp)
{
	unsigned long flags;

	spin_lock_irqsave(&spinWDog_0, flags);
	sWDogCount = 0;
	spin_unlock_irqrestore(&spinWDog_0, flags);
	ffprintk("CPU received Wachdog Interrupt from DE MCU\n");
}


/**
 * @callgraph
 * @callergraph
 *
 * @brief interrupt handler to get event about watch dog from MCU
 *
 * @param temp [IN] dummy argument
 */
void VIDEO_CPU_WDG_FROM_MCU_1(unsigned long temp)
{
	unsigned long flags;

	spin_lock_irqsave(&spinWDog_1, flags);
	sWDogCount = 0;
	spin_unlock_irqrestore(&spinWDog_1, flags);
	ffprintk("CPU received Wachdog Interrupt from DE MCU\n");
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief interrupt IPC handler to get data from MCU
 *
 * @param temp [IN] dummy argument
 */
void VIDEO_CPU_ISR_FROM_MCU_1(unsigned long temp)
{
#if 1
	ffprintk("isr-from-mcu-1 \n");
#else
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
#endif
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief interrupt ACK handler to get data from MCU
 *
 * @param temp [IN] dummy argument
 */
void VIDEO_CPU_ACK_FROM_MCU_1(unsigned long temp)
{
#if 1
	ffprintk("ack-from-mcu-1 \n");
	de_wakeup_send2mcu_timeout[SOC_MCU_1] = 0;
	wake_up_interruptible(&wqAckFrMCU_1);
#else
	FWI_IPC_MSG_T *ipcMsgFrMcu = (FWI_IPC_MSG_T *)gpIPCBuffFrMCU;

	switch (ipcMsgFrMcu->u16Type) {
		case FWI_IPC_CMD_SYNC :
			BE_PWM_HAL_SetPwmSync(ipcMsgFrMcu->u32Data);
			break;
		case FWI_IPC_CMD_DBG :
			if(ipcMsgFrMcu->u32Cmd == BE_DBG_RESET)
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
#endif
}

int VIDEO_WorkQueue_Init(UINT32 mcu_id)
{
	char str[16];
	if(mcu_id >= SOC_MCU_MAX)
		return RET_ERROR;

	sprintf(str, "MCU_%d_IPC_SEND", mcu_id);
	gwrqDe[mcu_id].wrq = create_workqueue(str);

	if(!gwrqDe[mcu_id].wrq) {
		ffprintk("create work queue failed. mcu_id %d", mcu_id);
		return RET_ERROR;
	}

	if(mcu_id == 0)
	{
		INIT_WORK(&gwrqDe[mcu_id].wrkRcv, DE_IPC_RcvMsg);
	}
	else if (mcu_id == 1)
	{
		//INIT_WORK(&gwrqDe[mcu_id].wrkRcv, DE_IPC_RcvMsg);
	}

#ifdef USE_VIDEO_TEST_MCU_IN_CPU
	MCU_TEST_Init();
#endif

	return RET_OK;
}

int VIDEO_WorkQueue_Destory(UINT32 mcu_id)
{
	destroy_workqueue(gwrqDe[mcu_id].wrq);
#ifdef USE_VIDEO_TEST_MCU_IN_CPU
	MCU_TEST_Destory();
#endif

	return RET_OK;
}

#ifdef USE_DE_TRACE_IPC_MSG
int MCU_VIDEO_IPC_PRINT(const char *file, \
						const int line,   \
						const char *fn,   \
						char *cliBuff,    \
						UINT32 strLen)
{
	int ret = RET_OK;
	int i;

	do {
		printk("%s:%d:%s, aaaaa length(%d)", file,line,fn,strLen);
		if (!strLen) break;
		printk(" data = [%02x", cliBuff[0]&0xff);
		for (i=1;i<strLen;i++) {
			printk(" %02x", cliBuff[i]&0xff);
			if ((i%4) == 3) printk(",");
		}
		printk("]");
	} while (0);
	printk("\n");

	return ret;
}

int MCU_VIDEO_IPC_CHECK(const char *file,\
						const int line,  \
						const char *fn,  \
						char *cliBuff)
{
	printk("%s:%d:%s, %s\n", file,line,fn,cliBuff);

	return RET_OK;
}
#endif

/**
 * @callgraph
 * @callergraph
 *
 * @brief Receive Messag from IPC Queue for IPC memory 
 *         which is located DDR memory
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
void DE_IPC_RcvMsg(struct work_struct *work)
{
	static UINT32 totalLen = 0;
	UINT32 checkLen;
    BOOLEAN isCompleted;
	UINT32 readLen;
	char readBuf[VIDEO_IPC_STR_MAX];
	int whichUart = 0;
	char *tok;
	int  argc;
	char **argv  = NULL;
	int  rdNum;
	int  i;
	LX_DE_REG_WR_T lReg;

	do {
		isCompleted = VIDEO_IPC_Gets(0, readBuf, &readLen, &saIpcQueFrMCU[SOC_MCU_0]);
		if (!readLen) break;
		checkLen = totalLen + readLen;
		if (VIDEO_MSG_STR_MAX <= checkLen) {
			ffprintk("Received(%d) is over than Max(%d)\n", \
					checkLen, VIDEO_MSG_STR_MAX);
			totalLen = 0;
			break;
		}
		memcpy(&saRecvBuff[totalLen], readBuf, readLen);
		totalLen = checkLen;
		if (!isCompleted) break;
		totalLen = 0;
#ifdef USE_DE_SWITCH_UART
		if ((tok = strstr(saRecvBuff, "\n" )) != NULL) *tok = 0;
		if (!strcmp(saRecvBuff, "UART_SWITCH_TO_CPU")) {
			ffprintk("%s\n", saRecvBuff);
			DE_HAL_SwitchUART0(whichUart);
		} else {
			argv = argv_split(GFP_KERNEL, saRecvBuff, &argc);
			if (!argv) break;
			if (!strcmp(argv[0], "CVRD")) {
				if      (argc == 2) rdNum = 1;
				else if (argc == 3) rdNum = simple_strtoul(argv[2], NULL, 0);
				else                break;
				lReg.wr_addr = simple_strtoul(argv[1], NULL, 0);
				for (i=0;i<rdNum;i++) {
					DE_HAL_ReadReg(lReg.wr_addr, &lReg.wr_data);
					sprintf(readBuf,                            \
							"Address[ 0x%x ], Vaule[ 0x%08x ]", \
							lReg.wr_addr, lReg.wr_data);
					VIDEO_CPU_PutStr(readBuf, VIDEO_IPC_CMD_PRT);
					lReg.wr_addr += 4;
				}
			} else if ( (!strcmp(argv[0], "CVWD")) || \
						(!strcmp(argv[0], "CVWR")) ) 
			{
				do {
					if (argc != 3) break;
					lReg.wr_addr = simple_strtoul(argv[1], NULL, 0);
					lReg.wr_data = simple_strtoul(argv[2], NULL, 0);
					sprintf(readBuf, \
							"Address[ 0x%x ], Vaule[ 0x%08x ]", \
							lReg.wr_addr, lReg.wr_data);
					VIDEO_CPU_PutStr(readBuf, VIDEO_IPC_CMD_PRT);
					DE_HAL_WriteReg(lReg.wr_addr, lReg.wr_data);
				} while (0);
			} else if ( !strcmp(argv[0], "CVBW") ) {
				if (argc != 4) break;
				lReg.wr_addr = simple_strtoul(argv[1], NULL, 16);
				lReg.wr_data = DE_REG_WB(lReg.wr_addr, argv[3], argv[2]);
				sprintf(readBuf, \
						"Address[ 0x%x ], Vaule[ 0x%08x ]",\
						lReg.wr_addr, lReg.wr_data);
				VIDEO_CPU_PutStr(readBuf, VIDEO_IPC_CMD_PRT);
			}
		}
#endif
	} while (0);
	if (argv) argv_free(argv);
}

UINT32 DE_REG_WB(UINT32 addr, char *pData, char *pField)
{
    UINT32 val = 0xdeadcafe;
    UINT32 rValue;
    UINT32 mValue;
    UINT32 nd, nw;
    UINT32 nDn, nUp;
    char *tok, *pUp, *pDn;

    do {
        if ((tok = strstr(pField, "]" )) == NULL) break;
        *tok = 0;
        if ((tok = strstr(pField, "[" )) == NULL) break;
        pUp = tok+1;
        if ((tok = strstr(pField, ":" )) != NULL) {
            pDn = tok+1;
            *tok = 0;
            if (strlen(pUp) == 0) break;
            if (strlen(pDn) == 0) break;
#ifdef USE_LINUX_KERNEL
            nUp = simple_strtoul(pUp, NULL, 10);
            nDn = simple_strtoul(pDn, NULL, 10);
#else
            nUp = strtoul(pUp, NULL, 10);
            nDn = strtoul(pDn, NULL, 10);
#endif
        } else {
            if (strlen(pUp) == 0) break;
#ifdef USE_LINUX_KERNEL
            nUp = simple_strtoul(pUp, NULL, 10);
#else
            nUp = strtoul(pUp, NULL, 10);
#endif
            nDn = nUp;
        }

        nw = 1;
        if (nUp > nDn) {
            nd = nDn;
            nw += (nUp - nDn);
        } else if (nUp < nDn) {
            nd = nUp;
            nw += (nDn - nUp);
        } else {
            nd = nUp;
        }
        if (nd + nw > 32) break;

        mValue   = GET_PVAL(0xFFFFFFFF, nd, nw);
		DE_HAL_ReadReg(addr, &rValue);
		rValue &= ~mValue;
		val = simple_strtoul(pData, NULL, 16);
        val    <<= nd;
        val     += rValue;

		DE_HAL_WriteReg(addr, val);
    } while(0);

    return val;
}



int VIDEO_DENC_SetTtx(VIDEO_TTX_PACK_T *pTtxPack)
{
    int ret = RET_OK;

	ret = DE_HAL_SetTtx(pTtxPack);

	return ret;
}

int VIDEO_JPG_WaitVsync(void)
{
	UINT32 tryCount;

	tryCount = 0;
	do {
		de_wakeup_recv4jpg_timeout = 1;
		wait_event_interruptible_timeout(   \
				wqIpcForJPG,                \
				!de_wakeup_recv4jpg_timeout,\
				msecs_to_jiffies(DE_IPC_WAIT_TIME_MSEC));
		if (!de_wakeup_recv4jpg_timeout) break;
		tryCount++;
	} while (tryCount < DE_IPC_TRY_CNT_FOR_JPG);

	return (tryCount < DE_IPC_TRY_CNT_FOR_JPG)?RET_OK:RET_TIMEOUT;
}

int VIDEO_USB_WaitVsync(void)
{
	UINT32 tryCount;

	tryCount = 0;
	do {
		de_wakeup_recv4usb_timeout = 1;
		wait_event_interruptible_timeout(   \
				wqIpcForUSB,                \
				!de_wakeup_recv4usb_timeout,\
				msecs_to_jiffies(DE_IPC_WAIT_TIME_MSEC));
		if (!de_wakeup_recv4usb_timeout) break;
		tryCount++;
	} while (tryCount < DE_IPC_TRY_CNT_FOR_USB);

	return (tryCount < DE_IPC_TRY_CNT_FOR_USB)?RET_OK:RET_TIMEOUT;
}

int VIDEO_WEL_WaitVsync(void)
{
	UINT32 tryCount;

	tryCount = 0;
	do {
		de_wakeup_recv4wel_timeout = 1;
		wait_event_interruptible_timeout(   \
				wqIpcForWEL,                \
				!de_wakeup_recv4wel_timeout,\
				msecs_to_jiffies(DE_IPC_WAIT_TIME_MSEC));
		if (!de_wakeup_recv4wel_timeout) break;
		tryCount++;
	} while (tryCount < DE_IPC_TRY_CNT_FOR_WEL);

	return (tryCount < DE_IPC_TRY_CNT_FOR_WEL)?RET_OK:RET_TIMEOUT;
}

int VIDEO_WER_WaitVsync(void)
{
	UINT32 tryCount;

	tryCount = 0;
	do {
		de_wakeup_recv4wer_timeout = 1;
		wait_event_interruptible_timeout(   \
				wqIpcForWER,                \
				!de_wakeup_recv4wer_timeout,\
				msecs_to_jiffies(DE_IPC_WAIT_TIME_MSEC));
		if (!de_wakeup_recv4wer_timeout) break;
		tryCount++;
	} while (tryCount < DE_IPC_TRY_CNT_FOR_WER);

	return (tryCount < DE_IPC_TRY_CNT_FOR_WER)?RET_OK:RET_TIMEOUT;
}

/** @} */
