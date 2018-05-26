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
#ifndef _MCU_TEST_h
#define _MCU_TEST_h
/*----------------------------------------------------------------------------------------
 *   Control Constants
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/

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
int VIDEO_MCU_ISR_FROM_CPU(void);
int MCU_VIDEO_IPC_CHECK2RUN(void);
int VIDEO_MCU_ClearIPC(void);
int MCU_VIDEO_IPC_RUN2CLI(char *cliBuff, UINT32 strLen);
char **MCU_VIDEO_GetArgFromStr(char *cmdline, int *argc);
BOOLEAN VIDEO_IPC_GetData(char *pBuff, UINT32 *pLen, MCU_VIDEO_EXCHANGE_T *pIpcQue);
int TEST_CPU_WakeUp(void);
void VIDEO_MCU_RUN(struct work_struct *work);
int MCU_TEST_Init(void);
int MCU_TEST_Destory(void);
void VIDEO_MCU_SendMesg2IPC(struct work_struct *work);
int VIDEO_MCU_PutData(char *msgBuff, UINT32 msgCnt);
int VIDEO_MCU_WakeUpIPC(void);
int VIDEO_MCU_WakeUpDMA(void);
#ifdef USE_VIDEO_TEST_MCU_IN_CPU
#ifdef USE_QEMU_SYSTEM
int VIDEO_MCU_IPC_Init(UINT32 addrVideoBuffFrCPU, UINT32 addrVideoBuffToCPU );
#endif
#endif

/*----------------------------------------------------------------------------------------
 *   External Variables
 *---------------------------------------------------------------------------------------*/

#endif
/**  @} */
