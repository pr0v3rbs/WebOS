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

#ifndef __ADEC_IPC_INTERNAL_H__
#define __ADEC_IPC_INTERNAL_H__

#include "common/adec_config.h"
#include "common/adec_debug.h"

#define IPC_END_FLAG		0x00444E45

#define IPC_DBG_ERR(_FMT,...)	ADEC_DBG_ERR(ADEC_DBG_INDEX_IPC, _FMT, ##__VA_ARGS__)
#define IPC_DBG_LOW(_FMT,...)	ADEC_DBG_LOW(ADEC_DBG_INDEX_IPC, _FMT, ##__VA_ARGS__)
#define IPC_DBG(_FMT,...)		ADEC_DBG(ADEC_DBG_INDEX_IPC, _FMT, ##__VA_ARGS__)

#if defined(ADEC_OPT_IPC_USE_ASSERT) && (ADEC_OPT_IPC_USE_ASSERT == 1)
#define IPC_ASSERT			ADEC_ASSERT
#define IPC_ASSERT_STR		ADEC_ASSERT_STR
#else
#define IPC_ASSERT(...)
#define IPC_ASSERT_STR(...)
#endif

typedef struct _IpcInfo{
	IpcRegister			 writeOffsetAddr;
	IpcRegister			 readOffsetAddr;
	IpcSendIntFunc		 interruptFunc;
	void				*interruptParam;
	void				*physicalAddr;
	void				*logicalAddr;
	unsigned int		 syncWord;
	unsigned int		 maxSize;
	IpcRole				 role;
	IpcWraparoundAction	 wraparoundAction;
}IpcInfo;



#endif
