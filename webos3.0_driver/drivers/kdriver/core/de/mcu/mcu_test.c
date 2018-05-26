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
 * date       2010.02.26
 * note       Additional information.
 *
 * @addtogroup lg1150_de
 * @{
 */

/*----------------------------------------------------------------------------------------
 *   Control Constants
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include "os_util.h"
#include "de_drv.h"

#include "base_types.h"

#include "de_model.h"
#include "de_def.h"
#include "de_int_def.h"
#include "hal/de_hal_def.h"
#include "de_ipc_def.h"
#include "de_ipc.h"
#include "mcu/mcu_test.h"
#include "mcu/io_video.h"

#ifdef USE_VIDEO_TEST_MCU_IN_CPU
/*----------------------------------------------------------------------------------------
 *   Constant Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Macro Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Type Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   External Function Prototype Declarations
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   External Variables
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   global Functions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   global Variables
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Static Function Prototypes Declarations
 *---------------------------------------------------------------------------------------*/
typedef struct {
	struct workqueue_struct *wrq;
	struct work_struct wrkRcv;
	struct work_struct wrkSnd;
	//struct mutex mtxRcv;
} MCU_WORKQUEUE_T;

/*----------------------------------------------------------------------------------------
 *   Static Variables
 *---------------------------------------------------------------------------------------*/
//static DECLARE_WAIT_QUEUE_HEAD(wqIpcFrCPU);
static MCU_VIDEO_EXCHANGE_T saIpcQueFrCPU;
static MCU_VIDEO_EXCHANGE_T saIpcQueToCPU;
#ifdef USE_QEMU_SYSTEM
static char *gpVideoBuffFrCPU;
static char *gpVideoBuffToCPU;
#else
static char *gpVideoBuffFrCPU = (char *)DTVSOC_IPC_FROM_CPU_BASE;
static char *gpVideoBuffToCPU = (char *)DTVSOC_IPC_FROM_MCU_BASE;
#endif
static DECLARE_WAIT_QUEUE_HEAD(wqDmaFrCPU);
static char saReadBuffOfUART[VIDEO_MSG_STR_MAX];
static char saReadBuffOfIPC[VIDEO_MSG_STR_MAX];
//static char saSendBuffOfIPC[VIDEO_MSG_STR_MAX];
static BOOLEAN sVideoPrintOfIPC = FALSE;
#ifdef USE_VIDEO_TEST_MCU_IN_CPU
static MCU_WORKQUEUE_T gwrqMCU;
#endif

/*========================================================================================
 *   Implementation Group
 *=======================================================================================*/

#ifdef USE_VIDEO_TEST_MCU_IN_CPU
void VIDEO_MCU_RUN(struct work_struct *work)
{
    int ret;
	//MCU_WORKQUEUE_T *lwrqMCU = container_of(work, MCU_WORKQUEUE_T, wrkRcv);

	do {
		//interruptible_sleep_on(&wqIpcFrCPU);
		//mutex_lock(&lwrqMCU->mtxRcv);
		ret = VIDEO_MCU_ISR_FROM_CPU();
		if (ret) break;
		ret = MCU_VIDEO_IPC_CHECK2RUN();
	} while(0);
	//mutex_unlock(&lwrqMCU->mtxRcv);
}
#endif

int VIDEO_MCU_ISR_FROM_CPU(void)
{
	int ret;

	do {
		ret = VIDEO_IPC_CopyStr2Queue(&saIpcQueFrCPU, gpVideoBuffFrCPU);
		if (ret) break;
		gpVideoBuffFrCPU[VIDEO_IPC_SIZE] = 0;
#ifdef USE_QEMU_SYSTEM
		VIDEO_CPU_DMA_FROM_MCU(0);
#else
		ret = VIDEO_MCU_WakeUpDMA();
		if (ret) break;
#endif
		ret = VIDEO_MCU_ClearIPC();
	} while (0);

	return ret;
}

int VIDEO_MCU_ClearIPC(void)
{
    return VIDEO_IPC_WakeUpReg(VIDEO_IPC_MCU, FALSE);
}

int MCU_VIDEO_IPC_CHECK2RUN(void)
{
	int ret = RET_OK;
	char readBuf[VIDEO_IPC_STR_MAX];
	static UINT32 totalLenOfUART = 0;
	static UINT32 totalLenOfIPC = 0;
	UINT32 *pTotalLen = NULL;
	char *pReadBuff = NULL;
	UINT32 checkLen;
	BOOLEAN isReady;
	UINT32 ipcHead;
	UINT32 readLen;
    int (* func) (char *cliBuff, UINT32 strLen);

	do {
		isReady = VIDEO_IPC_GetData(readBuf, &ipcHead, &saIpcQueFrCPU);
		readLen = GET_BITS(ipcHead, VIDEO_IPC_SIZE*8, 8);
		if (!readLen) break;

		switch (GET_BITS(ipcHead, VIDEO_IPC_FROM*8, 8)) {
			case VIDEO_IPC_FROM_UART :
				pReadBuff = saReadBuffOfUART;
				pTotalLen = &totalLenOfUART;
				break;
			case VIDEO_IPC_FROM_HOST :
				pReadBuff = saReadBuffOfIPC;
				pTotalLen = &totalLenOfIPC;
				break;
			default :
				ret = RET_ERROR;
				ffprintk("Wrong Head(0x%08x)\n", ipcHead);
				break;
		}
		if (ret) break;

		checkLen = *pTotalLen + readLen;
		if (checkLen >= VIDEO_MSG_STR_MAX-1) {
			ffprintk("checkLen(%d) is over\n",checkLen);
			*pTotalLen = 0;
			break;
		}
		memcpy(&pReadBuff[*pTotalLen], readBuf, readLen);
		*pTotalLen = checkLen;
		if (isReady != TRUE) break;
		*pTotalLen = 0;
#ifdef USE_DE_TRACE_IPC_MSG
		MCU_VIDEO_IPC_PRINT(__FILE__,__LINE__, __FUNCTION__, pReadBuff, checkLen);
#endif
		func = (GET_BITS(ipcHead, VIDEO_IPC_TYPE*8, 8) == VIDEO_IPC_CMD_CLI)?MCU_VIDEO_IPC_RUN2CLI:MCU_VIDEO_IPC_IOCTL;
		ret = func(pReadBuff, checkLen);
	} while (0);

	return ret;
}

int MCU_VIDEO_IPC_RUN2CLI(char *cliBuff, UINT32 strLen)
{
	int ret = RET_ERROR;
	int argc;
	char *tok;
#ifdef USE_LINUX_KERNEL
	char **argv = NULL;
#else
	char *argv[128];
#endif

	do {
		if (VIDEO_GetPrintOfIPC()) ffprintq("Message from CPU is %s\n", cliBuff);

		if ((tok = strstr(cliBuff, "#" )) != NULL) *tok = 0;
		if ((tok = strstr(cliBuff, "--")) != NULL) *tok = 0;
#ifdef USE_LINUX_KERNEL
		argv = MCU_VIDEO_GetArgFromStr(cliBuff, &argc);
#else
		argc = MCU_VIDEO_ParseFromStr(cliBuff, argv);
#endif
		//if (argc > 0) ret = MEM_VIDEO_Execute(argc, argv);
#ifdef USE_XTENSA
		MCU_PromptConsole();
#endif
#ifdef USE_LINUX_KERNEL
		if (argv) argv_free(argv);
#endif
	} while (0);

	return ret;
}

#ifdef USE_LINUX_KERNEL
char **MCU_VIDEO_GetArgFromStr(char *cmdline, int *argc)
{
	return argv_split(GFP_KERNEL, cmdline, argc);
}
#else
int MCU_VIDEO_ParseFromStr(char *cmdline, char **argv)
{
	const char *delim = " \f\n\r\t\v";
	char *tok;
	int argc = 0;

	argv[argc] = NULL;

	for (tok = (char *) strtok(cmdline, delim); tok; tok = (char *) strtok(NULL, delim))
	{
		argv[argc++] = tok;
	}

	return argc;
}
#endif

BOOLEAN VIDEO_IPC_GetData(char *pBuff, UINT32 *pLen, MCU_VIDEO_EXCHANGE_T *pIpcQue)
{
	BOOLEAN isReady = FALSE;
	UINT8 *pStr;

	*pLen = 0;
	do {
		if (pIpcQue->wInx == pIpcQue->rInx) break;
		pStr = pIpcQue->str[pIpcQue->rInx];
		if (!pStr[VIDEO_IPC_SIZE]) break;
		memcpy(pBuff, &pStr[VIDEO_IPC_DATA], pStr[VIDEO_IPC_SIZE]);
		*pLen = *(UINT32*)pStr;
		pIpcQue->rInx++;
		if (pIpcQue->rInx >= VIDEO_IPC_BUFF_NUM) pIpcQue->rInx = 0;
		if(pStr[VIDEO_IPC_CONT]) break;
		isReady = TRUE;
	} while (0);

	return isReady;
}

BOOLEAN VIDEO_GetPrintOfIPC(void)
{
    return sVideoPrintOfIPC;
}

void VIDEO_MCU_DMA_FROM_CPU(unsigned long temp)
{
	wake_up_interruptible(&wqDmaFrCPU);
}

int MCU_TEST_Init(void)
{
#ifdef USE_VIDEO_TEST_MCU_IN_CPU
	gwrqMCU.wrq = create_workqueue("MCU_IPC_SEND");

	if(!gwrqMCU.wrq) {
		ffprintk("create work queue failed");
		return RET_ERROR;
	}

	INIT_WORK(&gwrqMCU.wrkRcv, VIDEO_MCU_RUN);
	INIT_WORK(&gwrqMCU.wrkSnd, VIDEO_MCU_SendMesg2IPC);
	//mutex_init(&gwrqMCU.mtxRcv);
	//queue_work(gwrqMCU.wrq, &gwrqMCU.wrkRcv);
#endif

	return RET_OK;
}

int TEST_CPU_WakeUp(void)
{
#ifndef USE_VIDEO_TEST_MCU_IN_CPU
	//wake_up_interruptible(&wqIpcFrCPU);
#else
	//mutex_lock(&gwrqMCU.mtxRcv);
	queue_work(gwrqMCU.wrq, &gwrqMCU.wrkRcv);
#endif

	return RET_OK;
}

int MCU_TEST_Destory(void)
{
#ifdef USE_VIDEO_TEST_MCU_IN_CPU
	destroy_workqueue(gwrqMCU.wrq);
#endif

	return RET_OK;
}

int VIDEO_MCU_PutData(char *msgBuff, UINT32 msgCnt)
{
    int ret = RET_ERROR;

    do {
        if (!msgBuff) {
            ffprintk("msgBuff is Null\n");
            break;
        }
#ifdef USE_DE_TRACE_IPC_MSG
		MCU_VIDEO_IPC_PRINT(__FILE__,__LINE__, __FUNCTION__, msgBuff, msgCnt);
#endif
		ret = VIDEO_IPC_PutData2Queue(&saIpcQueToCPU, msgBuff, msgCnt);
		if (ret) break;
		queue_work(gwrqMCU.wrq, &gwrqMCU.wrkSnd);
    } while (0);

    return ret;
}

void VIDEO_MCU_SendMesg2IPC(struct work_struct *work)
{
	int ret = RET_OK;
	VIDEO_IPC_STATUS_T ipcStatus;

	do {
		ret = VIDEO_IPC_PutQue2IPC(gpVideoBuffToCPU, &saIpcQueToCPU, &ipcStatus);
		if (ret) break;
		if (ipcStatus != VIDEO_IPC_OK) break;
		//printk("MCU sent to CPU %s\n", &gpVideoBuffToCPU[2]);
		ret = VIDEO_MCU_WakeUpIPC();
		VIDEO_CPU_ISR_FROM_MCU(0);
	} while (0);
}

int VIDEO_MCU_WakeUpIPC(void)
{
    return VIDEO_IPC_WakeUpReg(VIDEO_IPC_CPU, TRUE);
}

int VIDEO_MCU_WakeUpDMA(void)
{
	return VIDEO_IPC_WakeUpReg(VIDEO_DMA_CPU, TRUE);
}

#ifdef USE_QEMU_SYSTEM
int VIDEO_MCU_IPC_Init(UINT32 addrVideoBuffFrCPU, UINT32 addrVideoBuffToCPU )
{
	gpVideoBuffFrCPU = (char *)addrVideoBuffFrCPU;
	gpVideoBuffToCPU = (char *)addrVideoBuffToCPU;

	return OK;
}		/* -----  end of function VIDEO_MCU_IPC_Init  ----- */
#endif

#endif

/**  @} */

