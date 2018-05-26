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

#include "common/adec_common.h"
#include "common/adec_reg_api.h"

#include "ipc/adec_ipc_dbg.h"
#include "ipc/adec_ipc.h"
#include "ipc/adec_ipc_internal.h"

#include <stdarg.h>

static void *IPC_MEM_MOD = NULL;
#define	IPC_DBG_MAX_STRING_LENGTH	(ADEC_CNST_IPC_MAX_PARAM_SIZE - 4)
#define IPC_DBG_CMD					((' '<<24) | ('G'<< 16) | ('B' << 8) | 'D')

typedef union _IpcDbgFlag
{
	struct
	{
		unsigned short 	module_id;
		unsigned short 	debug_flag;
	}				str_avl;
	unsigned int	int_val;
}IpcDbgFlag;

typedef struct _IpcDbgHeader{
	unsigned int	sync_word;
	IpcDbgFlag		dbg_flag;
	unsigned int	dbg_length;
}IpcDbgHeader;


ADEC_RESULT IPC_WriteDebug(void* _ipc,
#if (ADEC_OPT_IPC_DBG_DEFAULT_INTERRUPT == 0)
                           int _sendIntrrupt,
#endif
                           int _module,
                           IPC_DEBUG_TYPE _type,
                           const char* _fmt,
                           ...)
{
	static char		 dbgBuffer[IPC_DBG_MAX_STRING_LENGTH+4];
	IpcHeader		 header;
	va_list			 ap;
	IpcInfo			*ipc;
	unsigned int	*startAddr;
	unsigned int	*ipcStartAddr;
	unsigned int	 writeOffset;
	unsigned int	 readOffset;
	int				 cmdLength;
	int				 freeSize;
	int				 dbgLength;

	IPC_ASSERT_STR(
		_ipc != NULL && _fmt != NULL,
		return ADEC_ERR_NULL,
		"_ipc & _fmt should not be null\n");

	ipc = (IpcInfo*)_ipc;

	if (IPC_ROLE_DBG_WRITER != ipc->role)
	{
		return ADEC_ERR_PERMISSION;
	}

	writeOffset = ADEC_ReadRegister(ipc->writeOffsetAddr);
	readOffset 	= ADEC_ReadRegister(ipc->readOffsetAddr);

	IPC_ASSERT(writeOffset < ipc->maxSize, return ADEC_ERR_DATA_CORRUPTED;);
	IPC_ASSERT(readOffset < ipc->maxSize, return ADEC_ERR_DATA_CORRUPTED;);

	va_start(ap, _fmt);
	dbgLength = vsnprintf(dbgBuffer, IPC_DBG_MAX_STRING_LENGTH, _fmt, ap);
	va_end(ap);

	IPC_ASSERT_STR(dbgLength != -1 ,
	               return ADEC_ERR_OVERFLOW,
	               "Debug buffer overflow\n");

	dbgBuffer[dbgLength] = '\0';
	dbgLength = (dbgLength + 4) & 0xFFFFFFFC;

	if ( writeOffset < readOffset)
	{
		freeSize = (int)readOffset - (int)writeOffset;
	}
	else
	{
		freeSize = (int)(ipc->maxSize) - (int)writeOffset + (int)readOffset;
	}

	cmdLength = sizeof(IpcHeader) + dbgLength + 4;

	if( cmdLength >= freeSize)
	{
		return ADEC_ERR_OVERFLOW;
	}

	startAddr = (unsigned int*)ipc->logicalAddr;

	IPC_ASSERT(writeOffset < ipc->maxSize, return ADEC_ERR_DATA_CORRUPTED);

	if (writeOffset + (unsigned int)cmdLength >= ipc->maxSize - 4)
	{
		if (readOffset <= (unsigned int)cmdLength)
		{
			return ADEC_ERR_OVERFLOW;
		}
		ipcStartAddr = &startAddr[writeOffset >> 2];

		ipcStartAddr[0] = IPC_END_FLAG;
		ADEC_CACHE_WRITEBACK(ipcStartAddr, 4);
		writeOffset = 0;

	}

	ipcStartAddr = &startAddr[writeOffset >> 2];

	ipcStartAddr[0] = ipc->syncWord;
	writeOffset += 4;

	header.cmd = IPC_DBG_CMD;
	header.paramLength = (unsigned int)dbgLength;
	//Fill the identifier as the timestamp
	header.senderInfo.strVal.identifier = (unsigned int)ADEC_GET_CURRENT_CLOCK();
	header.senderInfo.strVal.sender = (unsigned int)_module;
	header.targetInfo.strVal.optionFlag = _type;
	header.targetInfo.strVal.actionFlag = 0xDEB;
	header.targetInfo.strVal.target = 0;
	header.extField	= IPC_DBG_CMD;

	ADEC_MEMCPY(IPC_MEM_MOD, &startAddr[writeOffset >> 2], &header, sizeof(IpcHeader));
	writeOffset += sizeof(IpcHeader);
	IPC_ASSERT((dbgLength & 0x03) == 0,
	           return ADEC_ERR_ALIGNMENT);
	if (dbgLength > 0)
	{
		IPC_ASSERT(dbgBuffer != NULL,
		           return ADEC_ERR_NULL;);
		ADEC_MEMCPY(IPC_MEM_MOD, &startAddr[writeOffset >> 2], dbgBuffer, dbgLength);
		writeOffset += (unsigned int)dbgLength;
	}
	ADEC_CACHE_WRITEBACK(ipcStartAddr, cmdLength);

	IPC_ASSERT(writeOffset < ipc->maxSize, return ADEC_ERR_DATA_CORRUPTED;);
	ADEC_WriteRegister(ipc->writeOffsetAddr, writeOffset);

#if (ADEC_OPT_IPC_DBG_DEFAULT_INTERRUPT == 1)
   	if(ipc->interruptFunc)
#else
	if(_sendIntrrupt && ipc->interruptFunc)
#endif
	{
		ipc->interruptFunc(ipc->interruptParam);
	}

	return ADEC_ERR_NONE;
}


ADEC_RESULT IPC_ReadDebug(void* _ipc,
                          PFN_IpcDbgPrint _print_func)
{
	IpcInfo			*ipc;
	IpcHeader		*header;
	char			*dbg;
	unsigned int	*readPosition;
	unsigned int	*endPosition;
	unsigned int	 writeOffset;
	unsigned int	 readOffset;
#ifdef CACHE_CONTROL_ENABLE
	unsigned int	 cacheStart;
	unsigned int	 cacheEnd;
#endif
	int				 usedSize;

	IPC_ASSERT_STR(
		_ipc != NULL,
		return ADEC_ERR_NULL,
		"_ipc should not be null\n");

	ipc = (IpcInfo*)_ipc;

	if (IPC_ROLE_DBG_READER != ipc->role)
	{
		IPC_DBG_ERR("Wrong Defined IPC Role : [Expected %d] - [Real %d]\n",IPC_ROLE_DBG_READER, ipc->role);
		return ADEC_ERR_PERMISSION;
	}

	writeOffset = ADEC_ReadRegister(ipc->writeOffsetAddr);
	readOffset 	= ADEC_ReadRegister(ipc->readOffsetAddr);

	IPC_ASSERT(writeOffset < ipc->maxSize, return ADEC_ERR_DATA_CORRUPTED;);
	IPC_ASSERT(readOffset < ipc->maxSize, return ADEC_ERR_DATA_CORRUPTED;);

	if ( writeOffset < readOffset)
	{
		usedSize = (int)(ipc->maxSize) - (int)readOffset + (int)writeOffset;
	}
	else
	{
		usedSize = (int)writeOffset - (int)readOffset;
	}

	IPC_ASSERT_STR(
		usedSize >= 0,
		return ADEC_ERR_DATA_CORRUPTED,
		"usedSize cannot be smaller than 0 - [used %d][wo %d][ro %d][max %d]\n",
		usedSize,
		writeOffset,
		readOffset,
		ipc->maxSize);

	if (usedSize == 0)
	{
		return ADEC_ERR_UNDERFLOW;
	}

	readPosition = &(((unsigned int*)ipc->logicalAddr)[readOffset >> 2]);

	ADEC_CACHE_INVALIDATE(readPosition, ADEC_CNST_IPC_CACHE_SIZE);
#ifdef CACHE_CONTROL_ENABLE
	cacheStart = ((unsigned int)readPosition) & ADEC_CNST_IPC_CACHE_MASK;
	cacheEnd = (((unsigned int)readPosition + ADEC_CNST_IPC_CACHE_SIZE - 1)
		        & ADEC_CNST_IPC_CACHE_MASK)
		       + ADEC_CNST_IPC_CACHE_SIZE;
#endif

	if (*readPosition != ipc->syncWord)
	{
		if (*readPosition != IPC_END_FLAG)
		{
			IPC_DBG_ERR("Sync fail (wo = %d, ro = %d). Try to find sync.\n", writeOffset, readOffset);
		}
		endPosition = &(((unsigned int*)ipc->logicalAddr)[ipc->maxSize >> 2]);
		// Find Sync
		while(*readPosition != ipc->syncWord && usedSize > 0)
		{
			if (*readPosition == IPC_END_FLAG || readPosition >= endPosition)
			{
				readOffset = 0;
				if (ipc->wraparoundAction == IPC_WRAPAROUND_REGARD_AS_ERROR)
				{
					IPC_ASSERT(readOffset < ipc->maxSize, return ADEC_ERR_DATA_CORRUPTED;);
					ADEC_WriteRegister(ipc->readOffsetAddr, readOffset);
					return ADEC_ERR_WRAPAROUND;
				}
				usedSize = (int)writeOffset;
				readPosition = (unsigned int*)ipc->logicalAddr;
#ifdef CACHE_CONTROL_ENABLE
				ADEC_CACHE_INVALIDATE((void*)readPosition, ADEC_CNST_IPC_CACHE_SIZE);
				cacheStart = ((unsigned int)readPosition) & ADEC_CNST_IPC_CACHE_MASK;
				cacheEnd = (((unsigned int)readPosition + ADEC_CNST_IPC_CACHE_SIZE - 1)
					        & ADEC_CNST_IPC_CACHE_MASK)
					       + ADEC_CNST_IPC_CACHE_SIZE;
#endif
			}
			else
			{
				readPosition ++;
				usedSize -= 4;
				readOffset += 4;
#ifdef CACHE_CONTROL_ENABLE
				IPC_ASSERT_STR((unsigned int)readPosition >= cacheStart,
				               return ADEC_ERR_GENERAL;,
				               "Cache Start : %08X\tRead Position %08X\n",
				               cacheStart,
				               (unsigned int)readPosition);
				if ((unsigned int)readPosition >= cacheEnd)
				{
					ADEC_CACHE_INVALIDATE((void*)cacheEnd, ADEC_CNST_IPC_CACHE_SIZE);
					cacheEnd += ADEC_CNST_IPC_CACHE_SIZE;
					IPC_ASSERT_STR((unsigned int)readPosition < cacheEnd,
					                return ADEC_ERR_GENERAL;,
					                "In this condition readPosition(%08X) should be less than cacheEnd(%08X)\n",
					                (unsigned int)readPosition,
					                cacheStart);
				}
#endif
			}
		}
	}

	if(usedSize == 0)
	{
		IPC_ASSERT(readOffset < ipc->maxSize, return ADEC_ERR_DATA_CORRUPTED;);
		ADEC_WriteRegister(ipc->readOffsetAddr, readOffset);
		IPC_DBG_ERR("Fail to find sync : 0x%08X\n",ipc->syncWord);
		return ADEC_ERR_DATA_CORRUPTED;
	}

	IPC_ASSERT_STR(*readPosition == ipc->syncWord,
	                return ADEC_ERR_GENERAL;,
	                "If reach to this code, Threre has to be sync word at read position\n");

	readPosition ++;
	readOffset += 4;

#ifdef CACHE_CONTROL_ENABLE
	// Validation cached area
	IPC_ASSERT_STR((unsigned int)readPosition >= cacheStart,
	               return ADEC_ERR_GENERAL;,
	               "Cache Start : %08X\tRead Position %08X\n",
	               cacheStart,
	               (unsigned int)readPosition);

	while((unsigned int)readPosition + sizeof(IpcHeader) >= cacheEnd)
	{
		ADEC_CACHE_INVALIDATE((void*)cacheEnd, ADEC_CNST_IPC_CACHE_SIZE);
		cacheEnd += ADEC_CNST_IPC_CACHE_SIZE;
	}
#endif


	header = (IpcHeader*)readPosition;
	dbg = (char*)&header[1];		// (char*)(&readPosition[sizeof(IpcHeader)>>2])

#ifdef CACHE_CONTROL_ENABLE
	readPosition += sizeof(IpcHeader) >> 2;

	while((unsigned int)readPosition + header->paramLength >= cacheEnd)
	{
		ADEC_CACHE_INVALIDATE((void*)cacheEnd, ADEC_CNST_IPC_CACHE_SIZE);
		cacheEnd += ADEC_CNST_IPC_CACHE_SIZE;
	}
#endif

	if (header->cmd != IPC_DBG_CMD)
	{
		IPC_DBG_ERR("CMD type doesn't match to the IPC_DBG_CMD\n");
		readOffset += 4;
		IPC_ASSERT_STR(readOffset < ipc->maxSize,
               ADEC_WriteRegister(ipc->readOffsetAddr, writeOffset); return ADEC_ERR_DATA_CORRUPTED;,
               "readOffset : %08X    writeOffset : %08X\n", readOffset, writeOffset);
		ADEC_WriteRegister(ipc->readOffsetAddr, readOffset);
		return ADEC_ERR_DATA_CORRUPTED;
	}
	else
	{
		if (_print_func)
		{
			_print_func(header->senderInfo.strVal.identifier,
				header->senderInfo.strVal.sender,
				(IPC_DEBUG_TYPE)header->targetInfo.strVal.optionFlag,
				dbg);
		}

		readOffset += header->paramLength + sizeof(IpcHeader);
		IPC_ASSERT_STR(readOffset < ipc->maxSize,
               ADEC_WriteRegister(ipc->readOffsetAddr, writeOffset); return ADEC_ERR_DATA_CORRUPTED;,
               "readOffset : %08X    writeOffset : %08X\n", readOffset, writeOffset);
		ADEC_WriteRegister(ipc->readOffsetAddr, readOffset);
		return ADEC_ERR_NONE;
	}
}



/** @} */


