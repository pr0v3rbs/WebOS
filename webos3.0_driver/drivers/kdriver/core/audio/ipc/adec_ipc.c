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


/**
 *  @file adec_ipc.c
 *
 *
 *  @author     Jung, Kyung Soon (kyungsoon.jung@leg.com)
 *  @version    0.0.1
 *  @date       2012-03-15
 *  @note
 *  @see        http://www.lge.com
 *  @addtogroup ADEC_COMMON
 *  @{
 */

#include "common/adec_common.h"
#include "common/adec_reg_api.h"

#include "ipc/adec_ipc.h"
#include "ipc/adec_ipc_internal.h"

static void *IPC_MEM_MOD = NULL;

ADEC_MUTEX_DECLARE(gIpcGlobalMutex);
static int gMutexInitialized = 0;

#if 1
#define IPC_LOCK()      ADEC_MUTEX_LOCK(&gIpcGlobalMutex)
#define IPC_RELEASE()   ADEC_MUTEX_RELEASE(&gIpcGlobalMutex)
#else
#define IPC_LOCK()
#define IPC_RELEASE()
#endif


int IPC_GetStructSize()
{
    return sizeof(IpcInfo);
}

ADEC_RESULT IPC_Init(
    void* _ipc,
    IpcRegister _writeAddr,
    IpcRegister _readAddr,
    unsigned int _syncWord,
    unsigned int _memSize,
    void* _physicalAddr,
    void* _logicalAddr,
    IpcRole _ipcRole)
{
    IpcInfo* ipc;

    IPC_ASSERT_STR(
        _ipc != NULL,
        return ADEC_ERR_NULL,
        "_ipc should not be null\n");

    ipc = (IpcInfo*)_ipc;

    ipc->writeOffsetAddr    = _writeAddr;
    ipc->readOffsetAddr     = _readAddr;
    ipc->syncWord           = _syncWord;
    ipc->maxSize            = _memSize;
    ipc->physicalAddr       = _physicalAddr;
    ipc->logicalAddr        = _logicalAddr;
    ipc->role               = _ipcRole;
    ipc->interruptFunc      = NULL;
    ipc->interruptParam     = NULL;

    ADEC_MUTEX_CREATE(&gIpcGlobalMutex);
    gMutexInitialized = 1;

    return ADEC_ERR_NONE;
}

ADEC_RESULT IPC_ClearRegister(void* _ipc, int _force)
{
    IpcInfo* ipc;
    unsigned int readOffset;
    unsigned int writeOffset;

    IPC_ASSERT_STR(
        _ipc != NULL,
        return ADEC_ERR_NULL,
        "_ipc should not be null\n");

    ipc = (IpcInfo*)_ipc;

    writeOffset = ADEC_ReadRegister(ipc->writeOffsetAddr);
    readOffset = ADEC_ReadRegister(ipc->readOffsetAddr);

    if (writeOffset != readOffset && _force == 0)
    {
        return ADEC_ERR_GENERAL;
    }

    ADEC_WriteRegister(ipc->writeOffsetAddr, 0);
    ADEC_WriteRegister(ipc->readOffsetAddr , 0);

    return ADEC_ERR_NONE;
}

enum {
    VALIDATE_CNT = 100,
};


ADEC_RESULT IPC_WriteCommand(
    void* _ipc,
    IpcHeader* _ipcHeader,
    void* _param,
    int _sendIntrrupt)
{
    IpcInfo* ipc;
    unsigned int *startAddr;
    unsigned int *ipcStartAddr;
    unsigned int writeOffset;
    unsigned int readOffset;
    int i, isValid, cnt;            /* For New Defense Code for IPC ERROR */

    // sizeof(unsigned int) is eqaul to 4
    // One is for sync byte
    // Another is for ignoring alignment
    /* For New Defense Code for IPC ERROR */
    unsigned int tmpBuffer[(ADEC_CNST_IPC_MAX_SIZE >> 2) + 2];

    int freeSize;
    int cmdLength;

    IPC_ASSERT_STR(
        _ipc != NULL && _ipcHeader != NULL,
        return ADEC_ERR_NULL,
        "_ipc & _ipcHeader should not be null\n");

    ipc = (IpcInfo*)_ipc;

    if (IPC_ROLE_WRITER != ipc->role)
    {
        return ADEC_ERR_PERMISSION;
    }

    IPC_LOCK();

    writeOffset = ADEC_ReadRegister(ipc->writeOffsetAddr);
    readOffset  = ADEC_ReadRegister(ipc->readOffsetAddr);

    if ( writeOffset < readOffset)
    {
        freeSize = (int)readOffset - (int)writeOffset;
    }
    else
    {
        freeSize = (int)(ipc->maxSize) - (int)writeOffset + (int)readOffset;
    }

    cmdLength = sizeof(IpcHeader) + (int)(_ipcHeader->paramLength) + 4;

    if( cmdLength >= freeSize)
    {
        IPC_RELEASE();
        return ADEC_ERR_OVERFLOW;
    }

    if (cmdLength > ADEC_CNST_IPC_MAX_SIZE) {
        IPC_RELEASE();
        return ADEC_ERR_RANGE;
    }

    startAddr = (unsigned int*)ipc->logicalAddr;

    IPC_ASSERT(writeOffset < ipc->maxSize, IPC_RELEASE(); return ADEC_ERR_DATA_CORRUPTED);

    if (writeOffset + (unsigned int)cmdLength >= ipc->maxSize)
    {
        if (readOffset <= (unsigned int)cmdLength)
        {
            IPC_RELEASE();
            return ADEC_ERR_OVERFLOW;
        }
        ipcStartAddr = &startAddr[writeOffset >> 2];
        ipcStartAddr[0] = IPC_END_FLAG;
        ADEC_CACHE_WRITEBACK(ipcStartAddr, 4);
        writeOffset = 0;
    }

    ipcStartAddr = &startAddr[writeOffset >> 2];

    /* Start of the New Defense Code for IPC Write Error(DDR Access Error). 2013.12.26.
        _ipc ==> tmpBuffer ==> ipcStartAddr(startAddr)
     */
    // Build IPC block
    tmpBuffer[0] = ipc->syncWord;
    ADEC_MEMCPY(IPC_MEM_MOD, tmpBuffer + 1, _ipcHeader, sizeof(IpcHeader));
    cnt = 1 + (sizeof(IpcHeader) >> 2);
    if (_ipcHeader->paramLength > 0) {
        IPC_ASSERT(_param != NULL, IPC_RELEASE(); return ADEC_ERR_NULL;);
        ADEC_MEMCPY(IPC_MEM_MOD, tmpBuffer +  cnt, _param, _ipcHeader->paramLength);
        cnt += (_ipcHeader->paramLength + 3) >> 2;
    }
    IPC_ASSERT(cmdLength == (cnt << 2),;);

    // Copy IPC to memory
    ADEC_MEMCPY(IPC_MEM_MOD, ipcStartAddr, tmpBuffer, cmdLength);
    ADEC_CACHE_WRITEBACK(ipcStartAddr, cmdLength);

    // Validate writing
    isValid = 0;
    for (i = 0; i < VALIDATE_CNT ; i++) {
        if (memcmp((void*)tmpBuffer, (void*)ipcStartAddr, cmdLength) == 0) {
            isValid = 1;
            break;
        }
        ADEC_DBG_ERR(ADEC_DBG_INDEX_IPC, "IPC writing ERR [CMD:%d][param_len:%d][offset:%d]\n",
                     _ipcHeader->cmd,
                     _ipcHeader->paramLength,
                     writeOffset);
        ADEC_MEMCPY(IPC_MEM_MOD, ipcStartAddr, tmpBuffer, cmdLength);
        ADEC_CACHE_WRITEBACK(ipcStartAddr, cmdLength);
        //ADEC_SLEEP_MS(1);
    }

    // Atomic.
    if (isValid == 0) {
        ADEC_DBG_ERR(ADEC_DBG_INDEX_IPC, "DDR_ERROR : Fail to write data %dtimes\n",VALIDATE_CNT);
        IPC_RELEASE();
        return ADEC_ERR_CORRUPTED;
    }
    /* End of the New Defense Code for IPC Write Error(DDR Access Error) */

    writeOffset += cmdLength;
    ADEC_WriteRegister(ipc->writeOffsetAddr, writeOffset);

    if(_sendIntrrupt && ipc->interruptFunc)
    {
        ipc->interruptFunc(ipc->interruptParam);
    }

    IPC_RELEASE();
    return ADEC_ERR_NONE;
}


ADEC_RESULT IPC_SetInterrupt(
    void* _ipc,
    IpcSendIntFunc _func,
    void* _param)
{
    IpcInfo* ipc;

    IPC_ASSERT_STR(
        _ipc != NULL,
        return ADEC_ERR_NULL,
        "_ipc should not be null\n");

    ipc = (IpcInfo*)_ipc;

    ipc->interruptFunc = _func;
    ipc->interruptParam = _param;

    return ADEC_ERR_NONE;
}

ADEC_RESULT IPC_ReadCommand(
    void* _ipc,
    IpcHeader** _ipcHeader,
    unsigned int* _paramLength)
{
    IpcInfo* ipc;
    IpcHeader* param;
    unsigned int* readPosition;
    unsigned int* endPosition;
    unsigned int writeOffset;
    unsigned int readOffset;
    int usedSize;
    int i, isValid;         /* For New Defense Code for IPC ERROR */
#ifdef CACHE_CONTROL_ENABLE
    unsigned int cacheStart;
    unsigned int cacheEnd;
#endif


    IPC_ASSERT_STR(
        _ipc != NULL && _ipcHeader != NULL && _paramLength != NULL,
        return ADEC_ERR_NULL,
        "_ipc & _ipcHeader should not be null\n");

    ipc = (IpcInfo*)_ipc;

    if (IPC_ROLE_READER != ipc->role)
    {
        return ADEC_ERR_PERMISSION;
    }

    writeOffset = ADEC_ReadRegister(ipc->writeOffsetAddr);
    readOffset  = ADEC_ReadRegister(ipc->readOffsetAddr);

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
        "usedSize cannot be smaller than 0\n");

    if (usedSize == 0)
    {
        return ADEC_ERR_UNDERFLOW;
    }

    readPosition = &(((unsigned int*)ipc->logicalAddr)[readOffset >> 2]);
    endPosition = &(((unsigned int*)ipc->logicalAddr)[ipc->maxSize >> 2]);

    ADEC_CACHE_INVALIDATE(readPosition, ADEC_CNST_IPC_CACHE_SIZE);
#ifdef CACHE_CONTROL_ENABLE
    cacheStart = ((unsigned int)readPosition) & ADEC_CNST_IPC_CACHE_MASK;
    cacheEnd = (((unsigned int)readPosition + ADEC_CNST_IPC_CACHE_SIZE - 1)
                & ADEC_CNST_IPC_CACHE_MASK)
               + ADEC_CNST_IPC_CACHE_SIZE;
#endif

    if (*readPosition != ipc->syncWord)
    {
        if (*readPosition == IPC_END_FLAG)
        {
            readOffset = 0;
            if (ipc->wraparoundAction == IPC_WRAPAROUND_REGARD_AS_ERROR)
            {
                ADEC_WriteRegister(ipc->readOffsetAddr, readOffset);
                return ADEC_ERR_WRAPAROUND;
            }
            usedSize = (int)(writeOffset);
            readPosition = (unsigned int*)ipc->logicalAddr;
#ifdef CACHE_CONTROL_ENABLE
            ADEC_CACHE_INVALIDATE((void*)readPosition, ADEC_CNST_IPC_CACHE_SIZE);
            cacheStart = ((unsigned int)readPosition) & ADEC_CNST_IPC_CACHE_MASK;
            cacheEnd = (((unsigned int)readPosition + ADEC_CNST_IPC_CACHE_SIZE - 1)
                        & ADEC_CNST_IPC_CACHE_MASK)
                       + ADEC_CNST_IPC_CACHE_SIZE;
#endif
        }
        while(*readPosition != ipc->syncWord && usedSize > 0)
        {
            readPosition ++;
            usedSize -= 4;
            readOffset += 4;
            if (readPosition >= endPosition) {
                IPC_DBG_LOW("Wrap-around detected during finding sync\n");
                readPosition = &(((unsigned int*)ipc->logicalAddr)[0]);
            }
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

    if(usedSize == 0)
    {
        ADEC_WriteRegister(ipc->readOffsetAddr, readOffset);
        readOffset = ADEC_ReadRegister(ipc->readOffsetAddr);
        IPC_DBG_ERR("Fail to find sync at [WO:%d][RO:%d][SIZE:%d]\n",
                    writeOffset, readOffset, ipc->maxSize);
        // Try to recover IPC
        ADEC_WriteRegister(ipc->readOffsetAddr, writeOffset);
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
#else
    ADEC_CACHE_INVALIDATE((void*)readPosition, ADEC_CNST_IPC_MAX_SIZE);
#endif


    param = (IpcHeader*)readPosition;

    /* Start of the New Defense Code for IPC Read Error(DDR Access Error). 2013.12.26. */
    for (i = 0, isValid = 0; i < VALIDATE_CNT ; i++) {
        if (param->paramLength < ADEC_CNST_IPC_MAX_SIZE) {
            unsigned int nextOffset;
            unsigned int *nextPosition;
            nextOffset = readOffset + param->paramLength + sizeof(IpcHeader);
            // Validate next packet
            if (nextOffset == writeOffset) {
                isValid = 1;
                break;
            }
            nextPosition = &(((unsigned int*)ipc->logicalAddr)[nextOffset >> 2]);
            if (*nextPosition == ipc->syncWord || *nextPosition == IPC_END_FLAG) {
                isValid = 1;
                break;
            }

            // If fail to validate, break;
            ADEC_DBG_ERR(ADEC_DBG_INDEX_IPC, "Broken next IPC around %d : %08X %08X %08X %08X",
                         nextOffset,
                         nextPosition[0],
                         nextPosition[1],
                         nextPosition[2],
                         nextPosition[3]);
            break;

        }
        ADEC_DBG_ERR(ADEC_DBG_INDEX_IPC, "IPC Reading ERR [CMD:%d][param_len:%d][offset:%d]\n",
                     param->cmd,
                     param->paramLength,
                     readOffset);
        ADEC_CACHE_INVALIDATE((void*)param, ADEC_CNST_IPC_MAX_SIZE);
        ADEC_SLEEP_MS(1);
    }

    if (isValid == 0) {
        ADEC_DBG_ERR(ADEC_DBG_INDEX_IPC, "Fail to validate IPC\n");
        // Try to recover IPC
        ADEC_WriteRegister(ipc->readOffsetAddr, writeOffset);
        return ADEC_ERR_CORRUPTED;
    }
    /* End of the New Defense Code for IPC Read Error(DDR Access Error) */

    *_ipcHeader = param;
    *_paramLength = param->paramLength;

#ifdef CACHE_CONTROL_ENABLE
    readPosition += sizeof(IpcHeader) >> 2;

    while((unsigned int)readPosition + param->paramLength >= cacheEnd)
    {
        ADEC_CACHE_INVALIDATE((void*)cacheEnd, ADEC_CNST_IPC_CACHE_SIZE);
        cacheEnd += ADEC_CNST_IPC_CACHE_SIZE;
    }
#endif

    readOffset += param->paramLength + sizeof(IpcHeader);

    ADEC_WriteRegister(ipc->readOffsetAddr, readOffset);

    return ADEC_ERR_NONE;
}

int IPC_CheckHasIpc(void* _ipc)
{
    if(IPC_GetUsedSize(_ipc) > 0)
    {
        return 1;
    }

    return 0;
}

int IPC_GetUsedSize(void* _ipc)
{
    IpcInfo* ipc;
    unsigned int writeOffset;
    unsigned int readOffset;
    int usedSize;

    ipc = (IpcInfo*)_ipc;

    IPC_ASSERT_STR(
        _ipc != NULL,
        return -ADEC_ERR_NULL,
        "_ipc should not be null\n");

    writeOffset = ADEC_ReadRegister(ipc->writeOffsetAddr);
    readOffset  = ADEC_ReadRegister(ipc->readOffsetAddr);

    if ( writeOffset < readOffset)
    {
        usedSize = (int)(ipc->maxSize) - (int)readOffset + (int)writeOffset;
    }
    else
    {
        usedSize = (int)writeOffset - (int)readOffset;
    }

    return usedSize;
}

int IPC_GetFreeSize(void* _ipc)
{
    IpcInfo* ipc;
    unsigned int writeOffset;
    unsigned int readOffset;

    int freeSize;

    IPC_ASSERT_STR(
        _ipc != NULL,
        return -ADEC_ERR_NULL,
        "_ipc should not be null\n");

    ipc = (IpcInfo*)_ipc;

    writeOffset = ADEC_ReadRegister(ipc->writeOffsetAddr);
    readOffset  = ADEC_ReadRegister(ipc->readOffsetAddr);

    if ( writeOffset < readOffset)
    {
        freeSize = (int)readOffset - (int)writeOffset;
    }
    else
    {
        freeSize = (int)(ipc->maxSize) - (int)writeOffset + (int)readOffset;
    }

    return freeSize;
}

ADEC_RESULT IPC_SetWraparoundAction(void* _ipc, IpcWraparoundAction _wraparoundAction)
{
    IpcInfo* ipc;

    IPC_ASSERT_STR(
        _ipc != NULL,
        return ADEC_ERR_NULL;,
        "_ipc should not be null\n");

    ipc = (IpcInfo*)_ipc;

    IPC_ASSERT(
        _wraparoundAction == IPC_WRAPAROUND_CONTINUE || _wraparoundAction == IPC_WRAPAROUND_REGARD_AS_ERROR,
        return ADEC_ERR_RANGE;);

    ipc->wraparoundAction = _wraparoundAction;

    return ADEC_ERR_NONE;
}

void IPC_SetIpcMemMode(void* _memmode) {
    IPC_MEM_MOD = _memmode;
}

/** @} */


