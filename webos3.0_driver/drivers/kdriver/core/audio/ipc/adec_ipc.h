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


/** @file adec_ipc.h
 *  Header file for ADEC IPC
 *  This file defines basic interface of IPC.
 *
 *  @author		Jung, Kyungsoon(kyungsoon.jung@lge.com)
 *  @version	0.1
 *  @date		2012-03-08
 *  @see		http://www.lge.com
 *  @addtogroup ADEC_COMMON
 *  @{
 */

/******************************************************************************
 	Header File Guarder
******************************************************************************/
#ifndef __ADEC_IPC_H__
#define __ADEC_IPC_H__

#include "common/adec_common.h"

/******************************************************************************
 * Control Contants
 ******************************************************************************/

/******************************************************************************
 * File Inclusions
 ******************************************************************************/

/******************************************************************************
 * Constant Definitions
 ******************************************************************************/

/******************************************************************************
 * Macro Definitions
 ******************************************************************************/

/******************************************************************************
 * Type Definitions
 ******************************************************************************/

typedef void* IpcRegister;

typedef enum _IpcRole{
	IPC_ROLE_WRITER,
	IPC_ROLE_READER,
	IPC_ROLE_DBG_WRITER,
	IPC_ROLE_DBG_READER,
}IpcRole;

typedef enum _IpcWraparoundAction{
	IPC_WRAPAROUND_CONTINUE,
	IPC_WRAPAROUND_REGARD_AS_ERROR,
}IpcWraparoundAction;

typedef union _IpcSenderInfo
{
	struct
	{
		unsigned int	identifier	:24;
		unsigned int	sender		:8;
	}				strVal;
	int				intVal;
}IpcSenderInfo;

typedef union _IpcTargetInfo
{
	struct
	{
		unsigned int	actionFlag	:16;
		unsigned int	optionFlag	:8;
		unsigned int	target		:8;
	}				strVal;
	int				intVal;
}IpcTargetInfo;

typedef struct _IpcHeader{
	unsigned int	cmd;
	IpcSenderInfo	senderInfo;
	IpcTargetInfo	targetInfo;
	unsigned int	extField;
	unsigned int	paramLength;
}IpcHeader;

typedef void (*IpcSendIntFunc)(void* _param);


/******************************************************************************
	Function Definitions
******************************************************************************/
/**
 * Retrieve the size of the IPC struct.
 * The value this function returns should be used for allocating memory.
 * IPC module never allocates memory by itself.
 *
 * @return if greater than 0 - size of the IPC structure
 * @return otherwise Error with error code
 */
int IPC_GetStructSize(void);

ADEC_RESULT IPC_Init(
	void* _ipc,
	IpcRegister _writeAddr,
	IpcRegister _readAddr,
	unsigned int _syncWord,
	unsigned int _memSize,
	void* _physicalAddr,
	void* _logicalAddr,
	IpcRole _ipcRole);

ADEC_RESULT IPC_ClearRegister(void* _ipc, int _force);

ADEC_RESULT IPC_WriteCommand(
	void* _ipc,
	IpcHeader* _ipcHeader,
	void* _param,
	int _sendIntrrupt);

ADEC_RESULT IPC_SetInterrupt(
	void* _ipc,
	IpcSendIntFunc _func,
	void* _param);

ADEC_RESULT IPC_ReadCommand(
	void* _ipc,
	IpcHeader** _ipcParam,
	unsigned int* _paramLength);

int IPC_CheckHasIpc(void* _ipc);

int IPC_GetUsedSize(void* _ipc);

int IPC_GetFreeSize(void* _ipc);

void IPC_SetIpcMemMode(void* _memmode);

ADEC_RESULT IPC_SetWraparoundAction(void* _ipc, IpcWraparoundAction _wraparoundAction);

#endif

/** @} */
