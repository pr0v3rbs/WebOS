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

#ifndef _DE_IPC_h
#define _DE_IPC_h

#include "mcu/de_vbi_def.h"

#define SOC_MCU_MAX  2
typedef enum {
	SOC_MCU_0,
	SOC_MCU_1,
//	SOC_MCU_MAX
} SOC_MCU_NUM;

int DE_IPC_Init(void);
int DE_IPC_Free(void);

int DE_KIPC_SetData(UINT32 ipcCmd, void *pMsg, UINT32 msgCnt);
int DE_KIPC_GetData(UINT32 mcu_id, void *pMsg, UINT32 msgCnt);
int DE_KIPC_SetMem(UINT32 ipcCmd, void *pMsg, UINT32 msgCnt);

void VIDEO_CPU_ISR_FROM_MCU(unsigned long temp);
void VIDEO_CPU_DMA_FROM_MCU(unsigned long temp);
void VIDEO_CPU_WDG_FROM_MCU(unsigned long temp);
void VIDEO_CPU_JPG_FROM_MCU(unsigned long temp);
void VIDEO_CPU_USB_FROM_MCU(unsigned long temp);
void VIDEO_CPU_WEL_FROM_MCU(unsigned long temp);
void VIDEO_CPU_WER_FROM_MCU(unsigned long temp);

void VIDEO_CPU_WDG_FROM_MCU_1(unsigned long temp);
void VIDEO_CPU_ISR_FROM_MCU_1(unsigned long temp);
void VIDEO_CPU_ACK_FROM_MCU_1(unsigned long temp);

	
int VIDEO_JPG_WaitVsync(void);
int VIDEO_USB_WaitVsync(void);
int VIDEO_WEL_WaitVsync(void);
int VIDEO_WER_WaitVsync(void);
int VIDEO_DENC_SetTtx(VIDEO_TTX_PACK_T *pTtxPack);


#ifdef USE_DE_TRACE_IPC_MSG
int MCU_VIDEO_IPC_PRINT(const char *file, const int line, const char *fn, char *cliBuff, UINT32 strLen);
int MCU_VIDEO_IPC_CHECK(const char *file, const int line, const char *fn, char *cliBuff);
#endif


BOOLEAN VIDEO_IPC_GetData(char *pBuff, UINT32 *pLen, MCU_VIDEO_EXCHANGE_T *pIpcQue);
BOOLEAN VIDEO_GetPrintOfIPC(void);
int VIDEO_CPU_PutStr(char *msgBuff, VIDEO_IPC_DATA_TYPE_T ipcType);
int VIDEO_IPC_MakeArg2Str(int argc, char **argv, char *pStr);
int DBI_VIDEO_FIRWARE(int argc, char **argv);
#endif
