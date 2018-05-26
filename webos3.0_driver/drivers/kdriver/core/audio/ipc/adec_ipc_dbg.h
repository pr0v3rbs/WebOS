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

#ifndef __ADEC_IPC_DBG_H__
#define __ADEC_IPC_DBG_H__

#include "common/adec_common.h"
#include "ipc/adec_ipc.h"
#include "imc/adec_imc.h"


typedef enum _IPC_DEBUG_TYPE{
	IPC_DEBUG_LOW = 0,
	IPC_DEBUG_NORMAL,
	IPC_DEBUG_WARN,
	IPC_DEBUG_ERROR,
}IPC_DEBUG_TYPE;

typedef void (*PFN_IpcDbgPrint)(unsigned int _timeStamp, int _module, IPC_DEBUG_TYPE _flag, char *_string);


ADEC_RESULT IPC_WriteDebug(void* _ipc,
#if (ADEC_OPT_IPC_DBG_DEFAULT_INTERRUPT == 0)
                           int _sendIntrrupt,
#endif
                           int _module,
                           IPC_DEBUG_TYPE _type,
                           const char* _fmt,
                           ...);

ADEC_RESULT IPC_ReadDebug(void* _ipc,
                          PFN_IpcDbgPrint _print_func);




#endif

/** @} */
