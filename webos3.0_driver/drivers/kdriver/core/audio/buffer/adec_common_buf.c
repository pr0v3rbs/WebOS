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
#include "adec_common_buf.h"
#include "adec_buf_dbg.h"
#include "adec_buf.h"

///////////////////////////////////////////////////////////////////////////////
// Macro Definition
///////////////////////////////////////////////////////////////////////////////
#if (ADEC_OPT_BUF_USE_REG == 0)
#define UPDATE_STRUCT()         _adap->pfnInvalidate(_buf, sizeof(ADEC_COMMON_BUF_T),_adap); _adap->pfnInvalidate(_status, sizeof(ADEC_COMMON_BUF_STATUS_T),_adap);
#define WRITE_STRUCT()          _adap->pfnWriteback(_buf, sizeof(ADEC_COMMON_BUF_T),_adap);  _adap->pfnWriteback(_status, sizeof(ADEC_COMMON_BUF_STATUS_T),_adap);
#else
#define UPDATE_STRUCT()
#define WRITE_STRUCT()
#endif // (ADEC_OPT_BUF_USE_REG == 0)
#define UPDATE_INDEX(_INDEX)    _adap->pfnInvalidate(_buf->indexTable + _INDEX, sizeof(ADEC_COM_BUF_AU_INFO_T),_adap)
#define WRITE_INDEX(_INDEX)     _adap->pfnWriteback(_buf->indexTable + _INDEX, sizeof(ADEC_COM_BUF_AU_INFO_T),_adap)

///////////////////////////////////////////////////////////////////////////////
// Implementation
///////////////////////////////////////////////////////////////////////////////

int CommonBuffer_GetStructSize(int _indexCount)
{
    if(_indexCount <= 0) {
        _indexCount = 1;
    }
    return (sizeof(ADEC_COMMON_BUF_T) + sizeof(ADEC_COM_BUF_AU_INFO_T) * (_indexCount));
}

int CommonBuffer_GetStatusStructSize(void)
{
	return (sizeof(ADEC_COMMON_BUF_STATUS_T));
}

int CommonBuffer_Create(
    unsigned char* _baseAddr,
    int _bufferSize,
    int _maxAuSize,
    int _indexCount,
    ADEC_PROCESSOR_ADAP_IF_T* _adap,
    void* _buf,
	void* _status)
{
	ADEC_COMMON_BUF_T* comBuf;

    ADEC_BUF_ASSERT(_adap != NULL && _adap->pfnAddrTrans != NULL && _adap->pfnInvalidate != NULL && _adap->pfnWriteback != NULL,
		NULL, NULL, ADEC_ERR_NULL,
        "Processor Adaptation Interface should not be NULL. Use DEFAULT_PROCESSOR_ADAP.\n");
    ADEC_BUF_ASSERT(_buf != NULL,
		NULL, NULL, ADEC_ERR_NULL,
        "_struct should not be null.\n");
	ADEC_BUF_ASSERT(_bufferSize >= _maxAuSize,
		NULL, NULL, ADEC_ERR_RANGE,
		"_bufferSize should be greater than _maxAuSize.\n");

#ifdef ADEC_OPT_BUF_USE_REG
	ADEC_BUF_ASSERT(_indexCount <= 16384,
		NULL, NULL, ADEC_ERR_RANGE,
		"_indexCount should not be greater than 16384, when you use register for inter communication\n");
#endif

	if(_indexCount == 0) {
		_indexCount = 1;
	}

	comBuf = (ADEC_COMMON_BUF_T*)_buf;
    comBuf->bufferStart = _baseAddr + _maxAuSize;
    comBuf->maxSize = _bufferSize - _maxAuSize;
    comBuf->maxIndex = _indexCount;
    comBuf->accessSize = _maxAuSize;

	WRITE_STRUCT();

	CommonBuffer_Init(_adap, comBuf, (ADEC_COMMON_BUF_STATUS_T*)_status);

    return ADEC_ERR_NONE;
}

int CommonBuffer_Init(
    ADEC_PROCESSOR_ADAP_IF_T* _adap,
    ADEC_COMMON_BUF_T* _buf,
	ADEC_COMMON_BUF_STATUS_T* _status)
{
	unsigned char* bufferBase;

    ADEC_BUF_ASSERT(_adap != NULL && _adap->pfnAddrTrans != NULL && _adap->pfnInvalidate != NULL && _adap->pfnWriteback != NULL,
		NULL, NULL, ADEC_ERR_NULL,
        "Processor Adaptation Interface should not be NULL. Use DEFAULT_PROCESSOR_ADAP.\n");
    ADEC_BUF_ASSERT(_buf != NULL,
		NULL, NULL, ADEC_ERR_NULL,
        "_buf should not be null.\n");

    UPDATE_STRUCT();

	if(_status != NULL) {
		_status->writeOffset = 0;
		_status->readOffset = 0;
		_status->readIndex = 0;
		_status->writeIndex = 0;
		_status->indexAllocated = 0;
		_status->totalReadCnt = 0;
		_status->totalWriteCnt = 0;
	}
#if (ADEC_OPT_BUF_USE_REPEAT == 1)
	_buf->repeatCnt = 0;
	_buf->repeatStartOffset = 0;
	_buf->repeatEndOffset = 0;
	_buf->repeatLength = 0;
	_buf->repeatStartIndex = 0;
	_buf->repeatEndIndex = 0;
	_buf->repeatIndexCnt = 0;
#endif

#if (ADEC_OPT_BUF_USE_REWIND == 1)
	_buf->totalConsumed = 0;
	_buf->totalIndex = 0;
#endif

	// Clear first au.
	ADEC_MEMSET(NULL, &_buf->indexTable[0].base, 0, sizeof(ADEC_AU_INFO_T));
	_buf->indexTable[0].bufOffset = 0;
	_buf->indexTable[0].complete = 0;
	WRITE_INDEX(0);
	WRITE_STRUCT();

#ifdef ADEC_OPT_BUF_CLR_MEM_INIT
	// Clear buffer momory with predefined value.
	bufferBase = _adap->pfnAddrTrans(_buf->bufferStart, _adap) - _buf->accessSize;
	if((_buf->accessSize + _buf->maxSize) > 0 && (_buf->accessSize + _buf->maxSize) < 0x1000000)
		ADEC_MEMSET(NULL, bufferBase, ADEC_CNST_BUF_CLR_MEM_VAL, _buf->accessSize + _buf->maxSize);
	else
		return -ADEC_ERR_RANGE;
	_adap->pfnWriteback(bufferBase, _buf->accessSize + _buf->maxSize, _adap);
#endif

    return ADEC_ERR_NONE;
}

int CommonBuffer_GetFreeSize(
	ADEC_PROCESSOR_ADAP_IF_T* _adap,
	ADEC_COMMON_BUF_T* _buf,
	ADEC_COMMON_BUF_STATUS_T* _status)
{
    ADEC_BUF_ASSERT(_adap != NULL && _adap->pfnAddrTrans != NULL && _adap->pfnInvalidate != NULL && _adap->pfnWriteback != NULL,
		NULL, NULL, ADEC_ERR_NULL,
        "Processor Adaptation Interface should not be NULL. Use DEFAULT_PROCESSOR_ADAP.\n");
    ADEC_BUF_ASSERT(_buf != NULL && _status != NULL,
		NULL, NULL, ADEC_ERR_NULL,
		"_buf should not be null.\n");

#if (ADEC_OPT_BUF_USE_REPEAT == 1)
	if(_buf->repeatCnt > 0)
		return 0;
#endif

	if(_status->readOffset <= _status->writeOffset)
		return _buf->maxSize - _status->writeOffset + _status->readOffset - ADEC_CNST_BUF_ALIGN_GUARD;
	else
		return _status->readOffset - _status->writeOffset - ADEC_CNST_BUF_ALIGN_GUARD;
}

int CommonBuffer_GetUsedSize(
	ADEC_PROCESSOR_ADAP_IF_T* _adap,
	ADEC_COMMON_BUF_T* _buf,
	ADEC_COMMON_BUF_STATUS_T* _status)
{
	long long usedSize;

    ADEC_BUF_ASSERT(_adap != NULL && _adap->pfnAddrTrans != NULL && _adap->pfnInvalidate != NULL && _adap->pfnWriteback != NULL,
		NULL, NULL, ADEC_ERR_NULL,
        "Processor Adaptation Interface should not be NULL. Use DEFAULT_PROCESSOR_ADAP.\n");
    ADEC_BUF_ASSERT(_buf != NULL && _status != NULL,
		NULL, NULL, ADEC_ERR_NULL,
		"_buf should not be null.\n");

    //UPDATE_STRUCT();

	usedSize = (long long)(_status->writeOffset) - (long long)(_status->readOffset);
	if(usedSize < 0)
		usedSize += (long long)(_buf->maxSize);
	ADEC_BUF_ASSERT(usedSize <= (long long)(_buf->maxSize) && usedSize >= 0,
		_buf, _status, ADEC_ERR_DATA_CORRUPTED,
		"used size is invalid\n");

#if (ADEC_OPT_BUF_USE_REPEAT == 1)
	ADEC_BUF_ASSERT(_buf->repeatCnt >= 0 && _buf->repeatLength >= 0,
		_buf, _status, ADEC_ERR_DATA_CORRUPTED,
		"repeatCnt or repeatLength is smaller than 0\n");
	usedSize +=  (long long)(_buf->repeatCnt) * (long long)(_buf->repeatLength);

	if(usedSize > 0x7FFFFFFF)
		usedSize = 0x7FFFFFFF;
#endif
	return (int)usedSize;
}

int CommonBuffer_GetAuCnt(
	ADEC_PROCESSOR_ADAP_IF_T* _adap,
	ADEC_COMMON_BUF_T* _buf,
	ADEC_COMMON_BUF_STATUS_T* _status)
{
	int usedIndex;

    ADEC_BUF_ASSERT(_adap != NULL && _adap->pfnAddrTrans != NULL && _adap->pfnInvalidate != NULL && _adap->pfnWriteback != NULL,
		NULL, NULL, ADEC_ERR_NULL,
        "Processor Adaptation Interface should not be NULL. Use DEFAULT_PROCESSOR_ADAP.\n");
    ADEC_BUF_ASSERT(_buf != NULL && _status != NULL,
		NULL, NULL, ADEC_ERR_NULL,
		"_buf should not be null.\n");

    //UPDATE_STRUCT();

	usedIndex = (int)(_status->writeIndex) - (int)(_status->readIndex);
	if(usedIndex < 0)
		usedIndex += _buf->maxIndex;
	usedIndex  += _status->indexAllocated;

	return usedIndex;
}

int CommonBuffer_GetMaxSize(
	ADEC_PROCESSOR_ADAP_IF_T* _adap,
	ADEC_COMMON_BUF_T* _buf,
	ADEC_COMMON_BUF_STATUS_T* _status)
{

    ADEC_BUF_ASSERT(_adap != NULL && _adap->pfnAddrTrans != NULL && _adap->pfnInvalidate != NULL && _adap->pfnWriteback != NULL,
		NULL, NULL, ADEC_ERR_NULL,
        "Processor Adaptation Interface should not be NULL. Use DEFAULT_PROCESSOR_ADAP.\n");
    ADEC_BUF_ASSERT(_buf != NULL && _status != NULL,
		NULL, NULL, ADEC_ERR_NULL,
		"_buf should not be null.\n");

    // _adap->pfnInvalidate(&_buf->maxSize, sizeof(int), _adap);

    return _buf->maxSize;
}

int CommonBuffer_GetMaxAu(
	ADEC_PROCESSOR_ADAP_IF_T* _adap,
	ADEC_COMMON_BUF_T* _buf,
	ADEC_COMMON_BUF_STATUS_T* _status)
{
    ADEC_BUF_ASSERT(_adap != NULL && _adap->pfnAddrTrans != NULL && _adap->pfnInvalidate != NULL && _adap->pfnWriteback != NULL,
		NULL, NULL, ADEC_ERR_NULL,
        "Processor Adaptation Interface should not be NULL. Use DEFAULT_PROCESSOR_ADAP.\n");
    ADEC_BUF_ASSERT(_buf != NULL && _status != NULL,
		NULL, NULL, ADEC_ERR_NULL,
		"_buf should not be null.\n");

    //UPDATE_STRUCT();

    return _buf->maxIndex;
}

int CommonBuffer_GetMaxAuLength(
	ADEC_PROCESSOR_ADAP_IF_T* _adap,
	ADEC_COMMON_BUF_T* _buf,
	ADEC_COMMON_BUF_STATUS_T* _status)
{
    ADEC_BUF_ASSERT(_adap != NULL && _adap->pfnAddrTrans != NULL && _adap->pfnInvalidate != NULL && _adap->pfnWriteback != NULL,
        NULL, NULL, ADEC_ERR_NULL,
        "Processor Adaptation Interface should not be NULL. Use DEFAULT_PROCESSOR_ADAP.\n");
    ADEC_BUF_ASSERT(_buf != NULL && _status != NULL,
		NULL, NULL, ADEC_ERR_NULL,
		"_buf should not be null.\n");

    //UPDATE_STRUCT();

    return _buf->accessSize;
}

int CommonBuffer_CreateNewAu(
    ADEC_AU_INFO_T* _au,
    ADEC_PROCESSOR_ADAP_IF_T* _adap,
    ADEC_COMMON_BUF_T* _buf,
	ADEC_COMMON_BUF_STATUS_T* _status)
{
    int writeIndex, usedIndex;

    ADEC_BUF_ASSERT(_adap != NULL && _adap->pfnAddrTrans != NULL && _adap->pfnInvalidate != NULL && _adap->pfnWriteback != NULL,
        NULL, NULL, ADEC_ERR_NULL,
        "Processor Adaptation Interface should not be NULL. Use DEFAULT_PROCESSOR_ADAP.\n");
    ADEC_BUF_ASSERT(_buf != NULL && _status != NULL,
		NULL, NULL, ADEC_ERR_NULL,
		"_buf should not be null.\n");

    //UPDATE_STRUCT();

#if (ADEC_OPT_BUF_USE_REPEAT == 1)
	if(_buf->repeatCnt > 0)
		return -ADEC_ERR_STATE;
#endif

	usedIndex = (int)(_status->writeIndex) - (int)(_status->readIndex);
	if(usedIndex < 0)
		usedIndex += _buf->maxIndex;
	usedIndex  += _status->indexAllocated;
	ADEC_BUF_ASSERT(usedIndex <= _buf->maxIndex && usedIndex >= 0,
		_buf, _status, ADEC_ERR_DATA_CORRUPTED,
		"Invalide used index cnt");
	if(usedIndex >= _buf->maxIndex)
		return -ADEC_ERR_OVERFLOW;

	writeIndex = (int)(_status->writeIndex);
    UPDATE_INDEX(writeIndex);

	if(_status->indexAllocated != 0) {
		int nextOffset;
        _buf->indexTable[writeIndex].complete = 1;
		nextOffset = (int)_buf->indexTable[writeIndex].base.size + _buf->indexTable[writeIndex].bufOffset;
		ADEC_BUF_ASSERT(nextOffset == _status->writeOffset || nextOffset == _status->writeOffset + _buf->maxSize,
			_buf, _status, ADEC_ERR_DATA_CORRUPTED,
			"Next write offset is not reliable.\n SWO[%d], SRO[%d], SWI[%d], SRI[%d]\n IBSZ[%llu], IBO[%d]\n wIndex[%d], nextOffset[%d], maxSize[%d]\n",
			_status->writeOffset, _status->readOffset, _status->writeIndex, _status->readIndex,
			_buf->indexTable[writeIndex].base.size,
			_buf->indexTable[writeIndex].bufOffset,
			writeIndex,
			nextOffset,
			_buf->maxSize);
		if(nextOffset != _status->writeOffset && nextOffset != _status->writeOffset + _buf->maxSize) {
			int auSize;
			auSize = _status->writeOffset - _buf->indexTable[writeIndex].bufOffset;
			if(auSize < 0)
				auSize += _buf->maxSize;
			_buf->indexTable[writeIndex].base.size = auSize;
		}
        WRITE_INDEX(writeIndex);
		writeIndex = (writeIndex + 1) % _buf->maxIndex;
		UPDATE_INDEX(writeIndex);
    }

    if(_au != NULL)
        ADEC_MEMCPY(NULL, &(_buf->indexTable[writeIndex].base), _au, sizeof(ADEC_AU_INFO_T));
    else
        ADEC_MEMSET(NULL, &(_buf->indexTable[writeIndex].base), 0, sizeof(ADEC_AU_INFO_T));

    _buf->indexTable[writeIndex].bufOffset = _status->writeOffset;
    _buf->indexTable[writeIndex].complete = 0;
    _buf->indexTable[writeIndex].base.size = 0;
    _status->indexAllocated = 1;
	_status->writeIndex = writeIndex;

    WRITE_INDEX(writeIndex);
    //WRITE_STRUCT();

    return ADEC_ERR_NONE;
}

int CommonBuffer_WriteData(
    unsigned char* _src,
    int _length,
    ADEC_PROCESSOR_ADAP_IF_T* _adap,
    ADEC_COMMON_BUF_T* _buf,
	ADEC_COMMON_BUF_STATUS_T* _status)
{
    ADEC_COM_BUF_AU_INFO_T* auInfo;
    unsigned char* bufferStart;
    int boundaryWriteCnt, remainWriteCnt, maxSize, temp;
	int writeOffset, readOffset;

    ADEC_BUF_ASSERT(_adap != NULL && _adap->pfnAddrTrans != NULL && _adap->pfnInvalidate != NULL && _adap->pfnWriteback != NULL,
        NULL, NULL, ADEC_ERR_NULL,
        "Processor Adaptation Interface should not be NULL. Use DEFAULT_PROCESSOR_ADAP.\n");

    ADEC_BUF_ASSERT(_buf != NULL && _status != NULL,
		NULL, NULL, ADEC_ERR_NULL,
		"_buf should not be null.\n");

    //UPDATE_STRUCT();

#if (ADEC_OPT_BUF_USE_REPEAT == 1)
	if(_buf->repeatCnt > 0)
		return -ADEC_ERR_STATE;
#endif

	maxSize = _buf->maxSize;
	writeOffset = _status->writeOffset;
	readOffset = _status->readOffset;

	// Create new au if no au is created.
	if(_status->indexAllocated == 0)
		CommonBuffer_CreateNewAu(NULL, _adap, _buf, _status);

	if(_status->readOffset <= _status->writeOffset) {
		temp = maxSize - writeOffset + readOffset - ADEC_CNST_BUF_ALIGN_GUARD;
		if(temp < _length) {
			ADEC_BUF_ERR("Buffer overflow\n");
			return -ADEC_ERR_OVERFLOW;
		}
	} else {
		temp = readOffset - writeOffset - ADEC_CNST_BUF_ALIGN_GUARD;
		if(temp < _length) {
			ADEC_BUF_ERR("Buffer overflow\n");
			return -ADEC_ERR_OVERFLOW;
		}
	}

    bufferStart = _adap->pfnAddrTrans(_buf->bufferStart, _adap);
    UPDATE_INDEX(_status->writeIndex);
    auInfo = &(_buf->indexTable[_status->writeIndex]);

    if(writeOffset + _length < maxSize) {
		// Normal case
#ifdef _ADEC_ARM
        //Copy a ES data from user memory buffer to kernel memory.
        if(copy_from_user(bufferStart + writeOffset, _src, _length))
        {
            AUD_KDRV_ERROR("CommonBuffer_WriteData(1) : copy_from_user error!!!\n");
            return -ADEC_ERR_GENERAL;
        }
#else   // _XTENSA,  _WIN32
        ADEC_MEMCPY(NULL, bufferStart + writeOffset, _src, _length);
#endif
        _adap->pfnWriteback(bufferStart + writeOffset, _length, _adap);
        writeOffset += _length;
    } else {
		// Buffer boundary condition
        boundaryWriteCnt = maxSize - writeOffset;
        remainWriteCnt = _length - boundaryWriteCnt;
        // Write data into buffer boundary.
#ifdef _ADEC_ARM
        if(copy_from_user(bufferStart + writeOffset, _src, boundaryWriteCnt))
        {
        	AUD_KDRV_ERROR("CommonBuffer_WriteData(2) : copy_from_user error!!!\n");
        	return -ADEC_ERR_GENERAL;
        }
#else   // _XTENSA,  _WIN32
        ADEC_MEMCPY(NULL, bufferStart + writeOffset, _src, boundaryWriteCnt);
#endif
        _adap->pfnWriteback(bufferStart + writeOffset, boundaryWriteCnt, _adap);
        // Write remain data into buffer start.
#ifdef _ADEC_ARM
        if(copy_from_user(bufferStart, _src + boundaryWriteCnt, remainWriteCnt))
        {
        	AUD_KDRV_ERROR("CommonBuffer_WriteData(3) : copy_from_user error!!!\n");
        	return -ADEC_ERR_GENERAL;
        }
#else   // _XTENSA,  _WIN32
        ADEC_MEMCPY(NULL, bufferStart, _src + boundaryWriteCnt, remainWriteCnt);
#endif
        _adap->pfnWriteback(bufferStart, remainWriteCnt, _adap);
#if (ADEC_OPT_BUF_FILL_GUARD_DURING_WRITING == 1)
        // Copy mirror data to the guard buffer.
        ADEC_MEMCPY(NULL, bufferStart - _buf->accessSize, bufferStart + maxSize - _buf->accessSize, _buf->accessSize);
        _adap->pfnWriteback(bufferStart - _buf->accessSize, _buf->accessSize, _adap);
#endif
        writeOffset = remainWriteCnt;
    }

    auInfo->base.size += _length;

    ADEC_BUF_ASSERT(writeOffset <= maxSize,
		_buf, _status, ADEC_ERR_DATA_CORRUPTED,
        "Write ptr exceed buffer end. Need debugging\n");
    if(writeOffset == maxSize)
        writeOffset = 0;

	_status->writeOffset = writeOffset;
    WRITE_INDEX(_status->writeIndex);
    //WRITE_STRUCT();

    return ADEC_ERR_NONE;
}

int CommonBuffer_InsertAu(
    unsigned char* _src,
    int _length,
    ADEC_AU_INFO_T* _au,
    ADEC_PROCESSOR_ADAP_IF_T* _adap,
    ADEC_COMMON_BUF_T* _buf,
	ADEC_COMMON_BUF_STATUS_T* _status)
{
    int result;

    ADEC_BUF_ASSERT(_adap != NULL && _adap->pfnAddrTrans != NULL && _adap->pfnInvalidate != NULL && _adap->pfnWriteback != NULL,
        NULL, NULL, ADEC_ERR_NULL,
        "Processor Adaptation Interface should not be NULL. Use DEFAULT_PROCESSOR_ADAP.\n");
    ADEC_BUF_ASSERT(_buf != NULL && _status != NULL,
		NULL, NULL, ADEC_ERR_NULL,
		"_buf should not be null.\n");

    result = CommonBuffer_CreateNewAu(_au, _adap, _buf, _status);
    if(result != ADEC_ERR_NONE) {
        ADEC_BUF_ERR("Fail to create au\n");
        return result;
	}

    result = CommonBuffer_WriteData(_src, _length, _adap, _buf, _status);
    // Fail to write data. Remove previous allocated au.
    if(result != ADEC_ERR_NONE) {
        _status->writeIndex = (short)((_status->writeIndex - 1 + _buf->maxIndex) % _buf->maxIndex);
        _status->indexAllocated = 0;
        //WRITE_STRUCT();
        ADEC_BUF_ERR("Fail to write data\n");
        return result;
    }

	_buf->indexTable[_status->writeIndex].complete = 1;
	WRITE_INDEX(_status->writeIndex);
    _status->indexAllocated = 0;
	_status->writeIndex = (_status->writeIndex + 1) % _buf->maxIndex;
    //WRITE_STRUCT();

    return ADEC_ERR_NONE;
}

int CommonBuffer_UpdateAu(
    ADEC_AU_INFO_T* _au,
    ADEC_PROCESSOR_ADAP_IF_T* _adap,
    ADEC_COMMON_BUF_T* _buf,
	ADEC_COMMON_BUF_STATUS_T* _status)
{
    long long size;

    ADEC_BUF_ASSERT(_adap != NULL && _adap->pfnAddrTrans != NULL && _adap->pfnInvalidate != NULL && _adap->pfnWriteback != NULL,
        NULL, NULL, ADEC_ERR_NULL,
        "Processor Adaptation Interface should not be NULL. Use DEFAULT_PROCESSOR_ADAP.\n");
    ADEC_BUF_ASSERT(_buf != NULL && _status != NULL,
		NULL, NULL, ADEC_ERR_NULL,
		"_buf should not be null.\n");

    //UPDATE_STRUCT();

	if(_status->indexAllocated == 0)
        return -ADEC_ERR_GENERAL;

    UPDATE_INDEX(_status->writeIndex);
    size = _buf->indexTable[_status->writeIndex].base.size;
    ADEC_MEMCPY(NULL, &_buf->indexTable[_status->writeIndex], _au, sizeof(ADEC_AU_INFO_T));
    _buf->indexTable[_status->writeIndex].base.size = size;
    WRITE_INDEX(_status->writeIndex);

    return ADEC_ERR_NONE;
}

int CommonBuffer_GetAuInfo(
    int _index,
    _OUT_ int* _offset,
    _OUT_ ADEC_AU_INFO_T* _au,
    ADEC_PROCESSOR_ADAP_IF_T* _adap,
    ADEC_COMMON_BUF_T* _buf,
	ADEC_COMMON_BUF_STATUS_T* _status)
{
    long long offset;
    int index, indexCnt;

    ADEC_BUF_ASSERT(_adap != NULL && _adap->pfnAddrTrans != NULL && _adap->pfnInvalidate != NULL && _adap->pfnWriteback != NULL,
        NULL, NULL, ADEC_ERR_NULL,
        "Processor Adaptation Interface should not be NULL. Use DEFAULT_PROCESSOR_ADAP.\n");
    ADEC_BUF_ASSERT(_buf != NULL && _status != NULL,
		NULL, NULL, ADEC_ERR_NULL,
		"_buf should not be null.\n");
    ADEC_BUF_ASSERT(_au != NULL,
		NULL, NULL, ADEC_ERR_NULL,
		"_au_info should not be null.\n");

    //UPDATE_STRUCT();

	if(_status->writeIndex >= _status->readIndex)
		indexCnt = (int)(_status->writeIndex) - (int)(_status->readIndex) + _status->indexAllocated;
	else
		indexCnt = _buf->maxIndex - (int)(_status->readIndex) + (int)(_status->writeIndex) + _status->indexAllocated;

	if(_index >= 0) {
		if(indexCnt < _index)
			return -ADEC_ERR_RANGE;
	} else {
		if(indexCnt < -_index)
			return -ADEC_ERR_RANGE;
		_index = indexCnt + _index;
	}

    index = (int)(_status->readIndex);
    UPDATE_INDEX(index);

	offset = (long long)(_buf->indexTable[index].bufOffset) - (long long)(_status->readOffset);
	if(_status->readOffset < _buf->indexTable[index].bufOffset)
		offset = (long long)(_buf->indexTable[index].bufOffset) - (long long)(_status->readOffset) - (long long)(_buf->maxSize);
	if((long long)(_buf->maxSize) < _buf->indexTable[index].base.size)
		ADEC_BUF_ASSERT(_buf->indexTable[index].complete == 0 && _status->readIndex == _status->writeIndex,
			_buf, _status, ADEC_ERR_DATA_CORRUPTED,
			"The case, au size is greater than max buf size, is allowed if and only if au is not used\n");

    while(_index > 0) {
		offset += _buf->indexTable[index].base.size;
		_index--;
        index = (index + 1) % _buf->maxIndex;
        UPDATE_INDEX(index);
    }

    *_offset = (int)offset;
    ADEC_MEMCPY(NULL, _au, &(_buf->indexTable[index]), sizeof(ADEC_AU_INFO_T));

    return ADEC_ERR_NONE;
}

int CommonBuffer_GetCurrentInfo(
    _OUT_ int* _offset,
    _OUT_ ADEC_AU_INFO_T* _au_info,
    ADEC_PROCESSOR_ADAP_IF_T* _adap,
    ADEC_COMMON_BUF_T* _buf,
	ADEC_COMMON_BUF_STATUS_T* _status)
{
    return CommonBuffer_GetAuInfo(0, _offset, _au_info, _adap, _buf, _status);
}

int CommonBuffer_ReadData(
    _OUT_ unsigned char** _dst,
    _IN_OUT_ int* _size,
    _OUT_ int* _au_in_data,
    ADEC_PROCESSOR_ADAP_IF_T* _adap,
    ADEC_COMMON_BUF_T* _buf,
	ADEC_COMMON_BUF_STATUS_T* _status)
{
	long long auSize;
    unsigned char* bufferStart;
    int index, indexCnt, usedSize, size, auInData;
    int boundary_size;

    ADEC_BUF_ASSERT(_adap != NULL && _adap->pfnAddrTrans != NULL && _adap->pfnInvalidate != NULL && _adap->pfnWriteback != NULL,
		NULL, NULL, ADEC_ERR_NULL,
        "Processor Adaptation Interface should not be NULL. Use DEFAULT_PROCESSOR_ADAP.\n");
    ADEC_BUF_ASSERT(_buf != NULL && _status != NULL,
		NULL, NULL, ADEC_ERR_NULL,
		"_buf should not be null.\n");
    ADEC_BUF_ASSERT(_dst != NULL,
		NULL, NULL, ADEC_ERR_NULL,
		"_dst should not be null.\n");
    ADEC_BUF_ASSERT(_size != NULL,
		NULL, NULL, ADEC_ERR_NULL,
		"_size should not be null.\n");

    if(*_size == 0)
    	return ADEC_ERR_NONE;

    //UPDATE_STRUCT();

    if((_status->readIndex == _status->writeIndex && _status->indexAllocated == 0) ||
		_status->readOffset == _status->writeOffset) {
        ADEC_BUF_ASSERT((_status->readIndex == _status->writeIndex && _status->indexAllocated == 0) && _status->readOffset == _status->writeOffset,
			_buf, _status, ADEC_ERR_DATA_CORRUPTED,
            "if usedIndex or usedSize is 0, then both of them should be 0.\n");
        return -ADEC_ERR_UNDERFLOW;
    }

	usedSize = _status->writeOffset - _status->readOffset;
	if(usedSize < 0)
		usedSize += _buf->maxSize;

	if(_status->writeIndex >= _status->readIndex)
		indexCnt = (int)(_status->writeIndex) - (int)(_status->readIndex) + _status->indexAllocated;
	else
		indexCnt = _buf->maxIndex - (int)(_status->readIndex) + (int)(_status->writeIndex) + _status->indexAllocated;

	ADEC_BUF_ASSERT(usedSize <= _buf->maxSize,
		_buf, _status, ADEC_ERR_DATA_CORRUPTED,
		"used size exceed max size of the buffer\n");
	ADEC_BUF_ASSERT(usedSize >= 0,
		_buf, _status, ADEC_ERR_DATA_CORRUPTED,
		"usedSize is smaller than 0.\n");

    if(*_size > usedSize)
        *_size = usedSize;

    index = (int)(_status->readIndex);
    ADEC_BUF_ASSERT(_status->readOffset <= _buf->maxSize,
		_buf, _status, ADEC_ERR_DATA_CORRUPTED,
        "Current readPtr is greater than bufferEnd.\n");

#if 1   // New Buffer Read Protection Code. 2014.07.10. jake.park
    // Check Buffer start position is located at the boundary.
    if(_status->readOffset + *_size > _buf->maxSize)
    {
        boundary_size = _buf->maxSize - _status->readOffset;
        if((*_size > boundary_size) && (boundary_size > _buf->accessSize))
        {
            *_size = boundary_size;
            ADEC_BUF_LOW("Buffer Boundary Condition. ReadSize[%d] is Reduced.\n", *_size);
        }
    }
#endif

    UPDATE_INDEX(index);
	auSize = _buf->indexTable[index].base.size;

	if(auSize < (long long)(_buf->maxSize)) {
		size = (int)auSize + _buf->indexTable[index].bufOffset - _status->readOffset;
		if(size > _buf->maxSize)
			size -= _buf->maxSize;
		auInData = 0;
		while(size <= *_size && auInData < indexCnt) {
			ADEC_BUF_ASSERT(_buf->indexTable[index].base.size >= 0,
				_buf, _status, ADEC_ERR_DATA_CORRUPTED,
				"usedSize is smaller than 0.\n");
			if(_buf->indexTable[index].complete == 0) {
                ADEC_BUF_ASSERT(size >= *_size, _buf, _status,
                    ADEC_ERR_DATA_CORRUPTED,
                    "SWO[%d], SRO[%d], SWI[%d], SRI[%d]\n size : %d\n*_size : %d\nbase.size : %llu\nbufOffset : %d\ncomplete : %d\nauInData : %d\nindex : %d\n",
                    _status->writeOffset, _status->readOffset, _status->writeIndex, _status->readIndex,
                    size,
                    *_size,
                    _buf->indexTable[index].base.size,
                    _buf->indexTable[index].bufOffset,
                    _buf->indexTable[index].complete,
                    auInData,
                    index);
				break;
			}
			if(size < 0) {
				ADEC_BUF_ERR("Indextable corrupted. WO : %d. RO : %d. WI : %d. RI : %d. IO : %d.\n",
					_status->writeOffset, _status->readOffset, _status->writeIndex, _status->readIndex,
					_buf->indexTable[index].bufOffset);
				break;
			}
			auInData++;
			index = (index+1)%_buf->maxIndex;
			UPDATE_INDEX(index);
			size += (int)_buf->indexTable[index].base.size;
		}
		if(_au_in_data)
			*_au_in_data = auInData + 1;
	} else {
		ADEC_BUF_ASSERT(_status->readIndex == _status->writeIndex,
			_buf, _status, ADEC_ERR_GENERAL,
			"Currently, maximum size of a AU is should be smller than buffer max size\n");
	}
	bufferStart = _adap->pfnAddrTrans(_buf->bufferStart, _adap);

	if(_status->readOffset + *_size > _buf->maxSize) {
		// Buffer start position is located at the boundary.
    #if 0   // 2014.06.23. New Protection Code for (*_size >= accessSize).
        if(*_size > _buf->accessSize)
        {
            ADEC_BUF_ERR("Unexpected condition. ReadSize[%d] is Too Big.\n", *_size);
            *_size = _buf->accessSize ;
        }
        ADEC_BUF_ASSERT(_status->readOffset + *_size - _buf->maxSize < _buf->accessSize,
			_buf, _status, ADEC_ERR_GENERAL,
			"access size is too big, boundary protection is broken\n");
    #endif
		// Mirroring data position
		boundary_size = _buf->maxSize - _status->readOffset;
        ADEC_BUF_ASSERT(boundary_size <= _buf->accessSize,
    			_buf, _status, ADEC_ERR_GENERAL,
    			"Mirroring Data size is too big, boundary protection is broken\n");
		*_dst = bufferStart - (boundary_size);
#if (ADEC_OPT_BUF_FILL_GUARD_DURING_WRITING == 0)
		_adap->pfnInvalidate(bufferStart, *_size - boundary_size, _adap);
		_adap->pfnInvalidate(bufferStart + _status->readOffset, boundary_size, _adap);
		ADEC_MEMCPY(NULL, *_dst, bufferStart + _status->readOffset, boundary_size);
#else
		_adap->pfnInvalidate(*_dst, *_size, _adap);
#endif
	} else {
		// Normal case
		*_dst = bufferStart + _status->readOffset;
		_adap->pfnInvalidate(*_dst, *_size, _adap);
	}

    return ADEC_ERR_NONE;
}

int CommonBuffer_FlushData(
    int* _size,
    ADEC_PROCESSOR_ADAP_IF_T* _adap,
    ADEC_COMMON_BUF_T* _buf,
	ADEC_COMMON_BUF_STATUS_T* _status)
{
	long long auSize;
	int index, indexCnt, totalIndex;
	int size, usedSize, flushSize;

#if (ADEC_OPT_BUF_USE_REPEAT == 1)
	ADEC_COM_BUF_AU_INFO_T* curAu;
	int indexLoop;
	int rewindSize;
#endif

    ADEC_BUF_ASSERT(_adap != NULL && _adap->pfnAddrTrans != NULL && _adap->pfnInvalidate != NULL && _adap->pfnWriteback != NULL,
		NULL, NULL, ADEC_ERR_NULL,
        "Processor Adaptation Interface should not be NULL. Use DEFAULT_PROCESSOR_ADAP.\n");
    ADEC_BUF_ASSERT(_buf != NULL && _status != NULL,
		NULL, NULL, ADEC_ERR_NULL,
		"_buf should not be null.\n");

    if(*_size == 0)
		return ADEC_ERR_NONE;

    //UPDATE_STRUCT();

	if((_status->readIndex == _status->writeIndex && _status->indexAllocated == 0) ||
		_status->readOffset == _status->writeOffset)
        return -ADEC_ERR_UNDERFLOW;

	usedSize = _status->writeOffset - _status->readOffset;
	if(usedSize < 0)
		usedSize += _buf->maxSize;
	flushSize = *_size;
	if(usedSize <= flushSize) {
#if (ADEC_OPT_BUF_USE_REPEAT == 1)
		if(_buf->repeatCnt > 0) {
			rewindSize = _buf->repeatLength;
			for(indexLoop = _buf->repeatIndexCnt - 1 ; indexLoop >= 0 ; indexLoop --) {
				ADEC_BUF_ASSERT(rewindSize >= 0 ,
					_buf, _status, ADEC_ERR_DATA_CORRUPTED,
					"rewindSize is samller than 0\n");
				UPDATE_INDEX(indexLoop);
				curAu = _buf->indexTable + ((_buf->repeatStartIndex + indexLoop) % _buf->maxIndex);

				ADEC_BUF_ASSERT((rewindSize == 0 && curAu->base.size == 0) || rewindSize != 0,
					_buf, _status, ADEC_ERR_DATA_CORRUPTED,
					"rewindSzie 0 but au size is not 0.\n");

				if(curAu->base.size > (long long)rewindSize)
					rewindSize = 0;
				else
					rewindSize -= (int)curAu->base.size;
				//WRITE_INDEX(indexLoop);
			}
			ADEC_BUF_ASSERT(indexLoop == -1 && rewindSize ==0,
				_buf, _status, ADEC_ERR_DATA_CORRUPTED,
				"Index table is broken during rewind.\n");
			_status->readIndex = _buf->repeatStartIndex;
			_status->writeIndex = _buf->repeatEndIndex;
			_status->readOffset = _buf->repeatStartOffset;
			_status->writeOffset = _buf->repeatEndOffset;
			_buf->repeatCnt --;

			WRITE_STRUCT();
			return ADEC_ERR_NONE;
		}
#endif
		flushSize = usedSize;
	}

	if (flushSize < 0) {
		*_size = 0;
		return ADEC_ERR_NONE;
	}

    index = (int)(_status->readIndex);
    ADEC_BUF_ASSERT(_status->readOffset <= _buf->maxSize,
		_buf, _status, ADEC_ERR_DATA_CORRUPTED,
        "Current readPtr is greater than bufferEnd.\n");
    UPDATE_INDEX(index);
	auSize = _buf->indexTable[index].base.size;
	if(auSize < (long long)(_buf->maxSize)) {
        ADEC_BUF_ASSERT((_buf->indexTable[index].bufOffset <= _status->readOffset && (long long)(_buf->indexTable[index].bufOffset) + _buf->indexTable[index].base.size >= (long long)(_status->readOffset)) ||
			    (_buf->indexTable[index].bufOffset <= _status->readOffset + _buf->maxSize && (long long)(_buf->indexTable[index].bufOffset) + _buf->indexTable[index].base.size >= (long long)(_status->readOffset) + (long long)(_buf->maxSize)),
			_buf, _status, ADEC_ERR_DATA_CORRUPTED,
			"Indextable corrupted !!!\n SWO[%d], SRO[%d]\n, SWI[%d], SRI[%d]\n Index[%d], MaxSize[%d]\n IBO[%d], IBSZ[%llu]\n",
			_status->writeOffset, _status->readOffset,
			_status->writeIndex, _status->readIndex,
			index, _buf->maxSize,
			_buf->indexTable[index].bufOffset, _buf->indexTable[index].base.size);

		size = (int)auSize + _buf->indexTable[index].bufOffset - _status->readOffset;
		if(size >= _buf->maxSize)
			size -= _buf->maxSize;

		indexCnt = 0;
		if(_status->writeIndex >= _status->readIndex)
			totalIndex = (int)(_status->writeIndex) - (int)(_status->readIndex) + _status->indexAllocated;
		else
			totalIndex = _buf->maxIndex - (int)(_status->readIndex) + (int)(_status->writeIndex) + _status->indexAllocated;

		while(size <= flushSize && totalIndex > indexCnt) {
			if(size < 0) {
				ADEC_BUF_ERR("Indextable corrupted. WO : %d. RO : %d. WI : %d. RI : %d. IO : %d.\n",
					_status->writeOffset, _status->readOffset, _status->writeIndex, _status->readIndex,
					_buf->indexTable[index].bufOffset);
				break;
			}
			if(_buf->indexTable[index].complete == 0) {
				ADEC_BUF_ASSERT(size >= *_size, _buf, _status,
					ADEC_ERR_DATA_CORRUPTED,
					"SWO[%d], SRO[%d], SWI[%d], SRI[%d]\n size :%d\n*_size : %d\nmaxSize : %d\nbase.size : %llu\nbufOffset : %d\ncomplete : %d\nindexCnt : %d\nindex : %d\n",
                    _status->writeOffset, _status->readOffset, _status->writeIndex, _status->readIndex,
                    size,
                    *_size,
                    _buf->maxSize,
					_buf->indexTable[index].base.size,
					_buf->indexTable[index].bufOffset,
					_buf->indexTable[index].complete,
					indexCnt,
					index);
				break;
			}
			if((int)(_status->writeIndex) == index) {
				ADEC_BUF_ASSERT(size == flushSize,
					_buf, _status, ADEC_ERR_DATA_CORRUPTED,
					"In this case the size(%d) should be equal to flushSize(%d)\n",
					size, flushSize);
				ADEC_BUF_ERR("Unexpected condition. Error protector activated.\n");
				break;
			}
			indexCnt ++;
			index = (index+1) % _buf->maxIndex;
			UPDATE_INDEX(index);
			ADEC_BUF_ASSERT(index < _buf->maxIndex,
				_buf, _status, ADEC_ERR_DATA_CORRUPTED,
				"index err\n");
			ADEC_BUF_ASSERT(_buf->indexTable[index].base.size < (long long)(_buf->maxSize),
				_buf, _status, ADEC_ERR_DATA_CORRUPTED,
				"The case, au size is greater than max buf size, is allowed if and only if au is not used\n");
			size += (int)_buf->indexTable[index].base.size;
		}
		_status->readIndex = (short)index;
		ADEC_BUF_ASSERT(_status->readIndex < _buf->maxIndex,
			_buf, _status, ADEC_ERR_DATA_CORRUPTED,
			"index err\r");
	} else {
		ADEC_BUF_ASSERT(_status->readIndex == _status->writeIndex,
			_buf, _status, ADEC_ERR_GENERAL,
			"Currently, maximum size of a AU is should be smller than buffer max size\n");
	}

    _status->readOffset += flushSize;
    if(_status->readOffset >= _buf->maxSize)
        _status->readOffset = _status->readOffset - _buf->maxSize;

	ADEC_BUF_ASSERT(flushSize > 0 ,
		_buf, _status, ADEC_ERR_DATA_CORRUPTED,
		"Unexpected flush size\n");

	*_size = flushSize;
    //WRITE_STRUCT();

    return ADEC_ERR_NONE;
}

int CommonBuffer_ReadAu(
    _OUT_ unsigned char** _dst,
    _OUT_ int* _size,
    _IN_OUT_ int* _au_cnt,
    ADEC_PROCESSOR_ADAP_IF_T* _adap,
    ADEC_COMMON_BUF_T* _buf,
	ADEC_COMMON_BUF_STATUS_T* _status)
{
	long long auSize;
    unsigned char* bufferStart;
    int size, index, indexCnt, usedIndex;

    ADEC_BUF_ASSERT(_adap != NULL && _adap->pfnAddrTrans != NULL && _adap->pfnInvalidate != NULL && _adap->pfnWriteback != NULL,
		NULL, NULL, ADEC_ERR_NULL,
        "Processor Adaptation Interface should not be NULL. Use DEFAULT_PROCESSOR_ADAP.\n");
    ADEC_BUF_ASSERT(_buf != NULL && _status != NULL && _dst != NULL && _size != NULL,
		NULL, NULL, ADEC_ERR_NULL,
		"Some arguments, should not be null, are null.\n");

    UPDATE_STRUCT();

	if((_status->readIndex == _status->writeIndex && _status->indexAllocated == 0) ||
		_status->readOffset == _status->writeOffset) {
        ADEC_BUF_ASSERT((_status->readIndex == _status->writeIndex && _status->indexAllocated == 0) &&
			_status->readOffset == _status->writeOffset,
			_buf, _status, ADEC_ERR_DATA_CORRUPTED,
            "if usedIndex or usedSize is 0, then both of them should be 0.\n");
        return -ADEC_ERR_UNDERFLOW;
    }

	usedIndex = (int)(_status->writeIndex) - (int)(_status->readIndex) + _status->indexAllocated;
	if(usedIndex < 0)
		usedIndex += _buf->maxIndex;
	if(*_au_cnt > usedIndex)
        *_au_cnt = usedIndex;

    index = (int)(_status->readIndex);
    ADEC_BUF_ASSERT(_status->readOffset <= _buf->maxSize,
		_buf, _status, ADEC_ERR_DATA_CORRUPTED,
        "Current readPtr is greater than bufferEnd.\n");

	auSize = _buf->indexTable[index].base.size;
	if(auSize < (long long)(_buf->maxSize)) {
		size = (int)auSize + _buf->indexTable[index].bufOffset - _status->readOffset;
		if(size > _buf->maxSize)
			size -= _buf->maxSize;

		for(indexCnt = 0 ; indexCnt < *_au_cnt ; indexCnt++) {
			ADEC_BUF_ASSERT(_buf->indexTable[index].base.size <= (long long)(_buf->maxSize),
				_buf, _status, ADEC_ERR_DATA_CORRUPTED,
				"The case, au size is greater than max buf size, is allowed if and only if au is not used\n");
			size += (int)_buf->indexTable[(index+indexCnt) % _buf->maxIndex].base.size;
		}
	} else {
		ADEC_BUF_ASSERT(_status->readIndex == _status->writeIndex,
			_buf, _status, ADEC_ERR_GENERAL,
			"Currently, maximum size of a AU is should be smller than buffer max size\n");
		if(_status->readOffset <= _status->writeOffset)
			size = _status->writeOffset - _status->readOffset;
		else
			size = _buf->maxSize - _status->readOffset + _status->writeOffset;
	}

	bufferStart = _adap->pfnAddrTrans(_buf->bufferStart, _adap);
	if(_status->readOffset > _buf->maxSize - _buf->accessSize)
    {
		// Buffer start position is located at the boundary.
		ADEC_BUF_ASSERT(_status->readOffset + *_size - _buf->maxSize < _buf->accessSize,
			_buf, _status, ADEC_ERR_GENERAL,
			"access size too big, boundary protection is broken\n");
		// Mirroring data position
		*_dst = bufferStart - (_buf->maxSize - _status->readOffset);
	} else {
		// Normal case
		*_dst = bufferStart + _status->readOffset;
	}

	*_size = size;
	_adap->pfnInvalidate(*_dst, *_size, _adap);

    return ADEC_ERR_NONE;
}


int CommonBuffer_SetRepeat(
	_OUT_ int _cnt,
	ADEC_PROCESSOR_ADAP_IF_T* _adap,
	ADEC_COMMON_BUF_T* _buf,
	ADEC_COMMON_BUF_STATUS_T* _status)
{

	ADEC_BUF_ASSERT(_adap != NULL && _adap->pfnAddrTrans != NULL && _adap->pfnInvalidate != NULL && _adap->pfnWriteback != NULL,
		NULL, NULL, ADEC_ERR_NULL,
        "Processor Adaptation Interface should not be NULL. Use DEFAULT_PROCESSOR_ADAP.\n");
    ADEC_BUF_ASSERT(_buf != NULL && _status != NULL,
		NULL, NULL, ADEC_ERR_NULL,
		"_buf should not be null.\n");

	//UPDATE_STRUCT();

#if (ADEC_OPT_BUF_USE_REPEAT == 1)
	if(_buf->repeatCnt > 0) {
		_buf->repeatCnt += _cnt;
		WRITE_STRUCT();
		return ADEC_ERR_NONE;
	}

	_buf->repeatCnt = _cnt;
	_buf->repeatEndIndex = (int)(_status->writeIndex);
	_buf->repeatEndOffset = _status->writeOffset;
	if(_status->readIndex <= _status->writeIndex)
		_buf->repeatIndexCnt = (int)(_status->writeIndex) - (int)(_status->readIndex) + _status->indexAllocated;
	else
		_buf->repeatIndexCnt = _buf->maxIndex - (int)(_status->readIndex) + (int)(_status->writeIndex) + _status->indexAllocated;
	if(_status->readOffset <= _status->writeOffset)
		_buf->repeatLength = _status->writeOffset - _status->readOffset;
	else
		_buf->repeatLength = _buf->maxSize - _status->readOffset + _status->writeOffset;
	_buf->repeatStartIndex = (int)(_status->readIndex);
	_buf->repeatStartOffset = _status->readOffset;
	WRITE_STRUCT();
	return ADEC_ERR_NONE;
#else
	return -ADEC_ERR_NOT_IMPLMENTED;
#endif
}
