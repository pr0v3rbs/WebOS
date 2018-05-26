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
 *  mcu ipc test file for BE device
 *	
 *  author		justine.jeong
 *  version		1.0
 *  date		2012.03.23
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
#include <asm/io.h>

#include "os_util.h"
#include "be_dbg.h"

#include "be_kapi.h"
#include "be_def.h"
#include "ipc_def.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/

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

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/

/*========================================================================================
	Implementation Group
========================================================================================*/

#ifdef MCU_IPC_DEBUG

#define GET_BITS(val,nd,wd)         (((val)>>(nd))&(0xFFFFFFFF>>(32-(wd))))

extern DPPB_TOP_REG_H13_T gDPPB_TOP_H13;

static char *gpVideoB8bFrCPU;
static char *gpVideoB8bToCPU;
static char   gpIpcTestBuffFrCPU[IPC_FROM_CPU_SIZE];
static char   gpIpcTestBuffToCPU[IPC_FROM_MCU_SIZE];

static int MCU_IPC_SetData(void);
static int MCU_IPC_GetData(void);
static int MCU_IPC_WakeUpReg(BE_INT_DIR_TYPE_T intDir, BOOLEAN turnOn);
static int MCU_IPC_ParseData(void);
static int MCU_IPC_Send(char *pMsg, UINT32 msgLen);
static int MCU_IPC_IOCTL(UINT32 ipcCmd, void *param);

int MCU_IPC_Init(UINT32 *pIPCBuffToMCU, UINT32 *pIPCBuffFrMCU)
{
   gpVideoB8bFrCPU = (char *)*pIPCBuffToMCU;
   gpVideoB8bToCPU = (char *)*pIPCBuffFrMCU;
   return RET_OK;
}

int MCU_IPC_Free(void)
{
	return RET_OK;
}

int MCU_IPC_InitInterrupt(void)
{
	return RET_OK;
}

int MCU_ISR_FROM_CPU(void)
{
	BE_PRINT("%s: enter\n", __F__);
	
	// get data from ipc register
	MCU_IPC_GetData();

	BE_PRINT("get data\n"); 
	
	// ack to cpu
	MCU_IPC_WakeUpReg(INT_MCU_CPU_ACK, TRUE);

	BE_PRINT("wakeup INT_MCU_CPU_ACK  -> TRUE\n");
	
	// parse ipc packet from received data
	MCU_IPC_ParseData();

	BE_PRINT("%s: exit\n", __F__);
	
	return RET_OK;
}

int MCU_ISR_TO_CPU(void)
{
	BE_PRINT("%s: enter\n", __F__);

	// set data to ipc register
	MCU_IPC_SetData();

	// interrupt to cpu
	MCU_IPC_WakeUpReg(INT_MCU_CPU_IPC, TRUE);

	BE_PRINT("wakeup INT_MCU_CPU_IPC  -> TRUE\n");

	BE_PRINT("%s: exit\n", __F__);

	return RET_OK;
}

static int MCU_IPC_SetData(void)
{
	int ret = RET_OK;

	do {
		memcpy(gpVideoB8bToCPU, gpIpcTestBuffToCPU, IPC_FROM_MCU_SIZE);
	} while(0);

	return ret;
}

static int MCU_IPC_GetData(void)
{
	int ret = RET_OK;

	do {
		memcpy(gpIpcTestBuffFrCPU, gpVideoB8bFrCPU, IPC_FROM_MCU_SIZE);
	} while(0);

	return ret;
}

static int MCU_IPC_WakeUpReg(BE_INT_DIR_TYPE_T intDir, BOOLEAN turnOn)
{
	int ret = RET_OK;

	turnOn &= 0x1;
	switch (intDir) {
		case INT_MCU_CPU_IPC :
			if (turnOn) {
				DPPB_TOP_H13_RdFL(top_arm_intr_status);
				DPPB_TOP_H13_Wr01(top_arm_intr_status, arm_swi_int_status, 1);
				DPPB_TOP_H13_WrFL(top_arm_intr_status);
			}
			else
			{
				DPPB_TOP_H13_RdFL(top_arm_intr_clr);
				DPPB_TOP_H13_Wr01(top_arm_intr_clr, arm_swi_int_clear, 1);
				DPPB_TOP_H13_WrFL(top_arm_intr_clr);
			}
			break;
		case INT_MCU_CPU_ACK :
			if (turnOn) {
				DPPB_TOP_H13_RdFL(top_arm_intr_status);
				DPPB_TOP_H13_Wr01(top_arm_intr_status, arm_swi_int_status, 2);
				DPPB_TOP_H13_WrFL(top_arm_intr_status);
			}
			else
			{
				DPPB_TOP_H13_RdFL(top_arm_intr_clr);
				DPPB_TOP_H13_Wr01(top_arm_intr_clr, arm_swi_int_clear, 2);
				DPPB_TOP_H13_WrFL(top_arm_intr_clr);
			}
			break;
		case INT_CPU_MCU_IPC :
			if (turnOn) {
				DPPB_TOP_H13_RdFL(top_rc_intr_status);
				DPPB_TOP_H13_Wr01(top_rc_intr_status, rc_swi_int_status, 1);
				DPPB_TOP_H13_WrFL(top_rc_intr_status);
			}
			else
			{
				DPPB_TOP_H13_RdFL(top_rc_intr_clr);
				DPPB_TOP_H13_Wr01(top_rc_intr_clr, rc_swi_int_clear, 1);
				DPPB_TOP_H13_WrFL(top_rc_intr_clr);
			}
			break;
		case INT_CPU_MCU_ACK :
			if (turnOn) {
				DPPB_TOP_H13_RdFL(top_rc_intr_status);
				DPPB_TOP_H13_Wr01(top_rc_intr_status, rc_swi_int_status, 2);
				DPPB_TOP_H13_WrFL(top_rc_intr_status);
			}
			else
			{
				DPPB_TOP_H13_RdFL(top_rc_intr_clr);
				DPPB_TOP_H13_Wr01(top_rc_intr_clr, rc_swi_int_clear, 2);
				DPPB_TOP_H13_WrFL(top_rc_intr_clr);
			}
			break;
		case INT_DE_MCU_IPC :
			if (turnOn) {
				DPPB_TOP_H13_RdFL(top_de_intr_status);
				DPPB_TOP_H13_Wr01(top_de_intr_status, de_swi_int_status, 1);
				DPPB_TOP_H13_WrFL(top_de_intr_status);
			}
			else
			{
				DPPB_TOP_H13_RdFL(top_de_intr_clr);
				DPPB_TOP_H13_Wr01(top_de_intr_clr, de_swi_int_clear, 1);
				DPPB_TOP_H13_WrFL(top_de_intr_clr);
			}
			break;
		case INT_DE_MCU_ACK :
			if (turnOn) {
				DPPB_TOP_H13_RdFL(top_de_intr_status);
				DPPB_TOP_H13_Wr01(top_de_intr_status, de_swi_int_status, 2);
				DPPB_TOP_H13_WrFL(top_de_intr_status);
			}
			else
			{
				DPPB_TOP_H13_RdFL(top_de_intr_clr);
				DPPB_TOP_H13_Wr01(top_de_intr_clr, de_swi_int_clear, 2);
				DPPB_TOP_H13_WrFL(top_de_intr_clr);
			}
			break;
		default :
			break;
	}
	return ret;
}

static int MCU_IPC_ParseData(void)
{
	int ret = RET_OK;
	FWI_IPC_MSG_T *ipcMsg = (FWI_IPC_MSG_T *)gpIpcTestBuffFrCPU;
	BOOLEAN isReady;

	do {
		//isReady = VIDEO_IPC_GetData(readBuf, &ipcHead, &saIpcQueFrCPU);
		isReady = TRUE;
		BE_PRINT("HEAD [%02x, %02x, %02x, %02x]\n", ipcMsg->u16From, ipcMsg->u16Type, ipcMsg->u32Cmd, ipcMsg->u32Data);

		BE_PRINT("Check ipc from - %s  (%d)\n", (ipcMsg->u16From==FWI_IPC_FROM_UART)?"UART":"HOST",  ipcMsg->u16From);

		if (isReady != TRUE) break;
		
		BE_PRINT("Check ipc type - %s\n", (ipcMsg->u16Type==FWI_IPC_CMD_CLI)?"CLI":"VAL");
		
		switch (ipcMsg->u16Type) {
			case FWI_IPC_CMD_CLI :
				break;
			case FWI_IPC_CMD_VAL :
				ret = MCU_IPC_IOCTL(ipcMsg->u32Cmd, (void *)&(ipcMsg->u32Data));
				break;
			default :
				break;
		}
	} while (0);

	return ret;
}

static int MCU_IPC_Send(char *pMsg, UINT32 msgLen)
{
	int ret = RET_OK;
	int totalLen;
	int totalCnt;
	int readLen;
	char *msgBuffIPC = gpIpcTestBuffToCPU;

	do {
		totalLen = msgLen;
		totalCnt = totalLen/(IPC_STR_MAX - FWI_IPC_DATA);
		while (totalLen>0) {
			readLen = (totalLen < (IPC_STR_MAX - FWI_IPC_DATA)) ? totalLen : (IPC_STR_MAX - FWI_IPC_DATA);
			memcpy(&msgBuffIPC[FWI_IPC_DATA], pMsg, readLen);
			msgBuffIPC[FWI_IPC_SIZE] = readLen;
			msgBuffIPC[FWI_IPC_CONT] = totalCnt;
			msgBuffIPC[FWI_IPC_FROM] = FWI_IPC_FROM_HOST;
			msgBuffIPC[FWI_IPC_TYPE] = FWI_IPC_CMD_VAL;
			pMsg     += readLen;
			totalLen -= readLen;
			totalCnt--;
		}
	} while (0);

	return ret;
}

static int MCU_IPC_IOCTL(UINT32 ipcCmd, void *param)
{
	int ret = RET_OK; 

	BE_PRINT("%s : enter\n", __F__);
	do {
		CHECK_NULL(param);
		
		switch (ipcCmd) {
			case BE_TOP_INIT :
			{
				BE_PRINT("BE_TOP_INIT\n");
			}
			break;
			case BE_FRC_GET_FIRMWARE_VERSION :
			{
				BE_FRC_VERSION_T stParams;
				stParams.u16Version = 0xAA;
				stParams.u16Subversion    = 0xBB;
				//MCU_IPC_Send((char*)&stParams, sizeof(BE_FRC_VERSION_T));
				BE_PRINT("BE_FRC_GET_FIRMWARE_VERSION (version[0x%02X] sub version[0x%02X])\n", stParams.u16Version, stParams.u16Subversion);
			}
			break;
			case BE_FRC_INIT_BLOCK :
			{
				BE_PRINT("BE_FRC_INIT_BLOCK\n");
			}
			break;
			case BE_FRC_SET_MEMC_MODE :
			{
				BE_FRC_MEMC_MODE_T stParams;
				memcpy((char*)&stParams, param, sizeof(BE_FRC_MEMC_MODE_T));
				BE_PRINT("BE_FRC_SET_MEMC_MODE : blur[%d] judder[%d] type[%d] \n", \
										stParams.u8Blur, stParams.u8Judder, stParams.u8MemcType);
			}
			break;
			default:
				break;
		}
	} while(0);
	BE_PRINT("%s : exit\n", __F__);
	return ret;
}

#endif //#ifdef MCU_IPC_DEBUG
