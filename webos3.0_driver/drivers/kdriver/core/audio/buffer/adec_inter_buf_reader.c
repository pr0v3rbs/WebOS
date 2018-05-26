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
#include "adec_inter_buf_reader.h"
#include "adec_buf.h"
#include "adec_buf_dbg.h"
#include "adec_buf_reg_api.h"

///////////////////////////////////////////////////////////////////////////////
// Macro Definition
///////////////////////////////////////////////////////////////////////////////

#if (ADEC_OPT_BUF_USE_REG == 1)
#define UPDATE_STRUCT()		intBuf->adap->pfnInvalidate(intBuf->comBuf, sizeof(ADEC_COMMON_BUF_T), intBuf->adap)
#define WRITE_STRUCT()		intBuf->adap->pfnWriteback(intBuf->comBuf, sizeof(ADEC_COMMON_BUF_T), intBuf->adap)
#define STATUS_REG (&intBuf->status)
#else
#define UPDATE_STRUCT()
#define WRITE_STRUCT()
#define STATUS_REG ((ADEC_COMMON_BUF_STATUS_T*)intBuf->comBuf)
#endif // (ADEC_OPT_BUF_USE_REG == 1)

///////////////////////////////////////////////////////////////////////////////
// Type Definition
///////////////////////////////////////////////////////////////////////////////
typedef struct _ADEC_INTER_BUF_READER_T
{
	ADEC_BUF_T					super;
#if (ADEC_OPT_BUF_USE_REG == 1)
	ADEC_COMMON_BUF_STATUS_T	status;
#endif
	void*						regBase;
	ADEC_COMMON_BUF_T*			comBuf;
	ADEC_PROCESSOR_ADAP_IF_T*	adap;
}ADEC_INTER_BUF_READER_T;



///////////////////////////////////////////////////////////////////////////////
// Static variables
///////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////
// Global variables
///////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////
// Local Function Declaration
///////////////////////////////////////////////////////////////////////////////

static int InterBufReader_Init(void* _buf);

static int InterBufReader_GetFreeSize(void* _buf)
ADEC_SECT_IRAM0_TEXT;

static int InterBufReader_GetUsedSize(void* _buf)
ADEC_SECT_IRAM0_TEXT;

static int InterBufReader_GetAuCnt(void* _buf);

static int InterBufReader_GetMaxSize(void* _buf);

static int InterBufReader_GetMaxAu(void* _buf);

static int InterBufReader_GetMaxAuLength(void* _buf);

static int InterBufReader_CreateNewAu(
	ADEC_AU_INFO_T* _info,
	void* _buf);

static int InterBufReader_WriteData(
	unsigned char* _src,
	int _length,
	void* _buf);

static int InterBufReader_InsertAu(
	unsigned char* _src,
	int _length,
	ADEC_AU_INFO_T* _info,
	void* _buf);

static int InterBufReader_UpdateAu(
	ADEC_AU_INFO_T* _info,
	void* _buf);

static int InterBufReader_GetAuInfo(
	int _index,
	_OUT_ int* _offset,
	_OUT_ ADEC_AU_INFO_T* _au_info,
	void* _buf);

static int InterBufReader_GetCurrentInfo(
	_OUT_ int* _offset,
	_OUT_ ADEC_AU_INFO_T* _au_info,
	void* _buf);

static int InterBufReader_ReadData(
	_OUT_ unsigned char** _dst,
	_IN_OUT_ int* _size,
	_OUT_ int* _au_in_data,
	void* _buf);

static int InterBufReader_FlushData(
	int _size,
	void* _buf);

static int InterBufReader_ReadAu(
	_OUT_ unsigned char** _dst,
	_OUT_ int* _size,
	_IN_OUT_ int* _au_cnt,
	void* _buf);

static int InterBufReader_SetRepeat(int _cnt,
	void* _buf);

///////////////////////////////////////////////////////////////////////////////
// Implementation
///////////////////////////////////////////////////////////////////////////////
int InterBufReader_GetStructSize(void)
{
	return sizeof(ADEC_INTER_BUF_READER_T);
}

int InterBufReader_CreateFromCommonBuffer(
	ADEC_PROCESSOR_ADAP_IF_T* _adapIf,
	void* _registerAddr,
	void* _commonBufferStruct,
	void* _struct)
{
	ADEC_INTER_BUF_READER_T* intBuf;

	ADEC_BUF_ASSERT(_adapIf != NULL,
		NULL,
		NULL,
		ADEC_ERR_NULL,
		"adap should not be null.\n");
	ADEC_BUF_ASSERT(_commonBufferStruct != NULL,
		NULL,
		NULL,
		ADEC_ERR_NULL,
		"common buffer should not be null.\n");
	ADEC_BUF_ASSERT(_struct != NULL,
		NULL,
		NULL,
		ADEC_ERR_NULL,
		"struct should not be null.\n");

#if (ADEC_OPT_BUF_USE_REG == 1)
	ADEC_BUF_ASSERT(_registerAddr != NULL,
		NULL,
		NULL,
		ADEC_ERR_NULL,
		"_registerAddr should not be null when you using register for inter communication\n");
#endif

	intBuf = (ADEC_INTER_BUF_READER_T*) _struct;

	intBuf->super.create_new_au = InterBufReader_CreateNewAu;
	intBuf->super.flush_data = InterBufReader_FlushData;
	intBuf->super.get_au_cnt = InterBufReader_GetAuCnt;
	intBuf->super.get_au_info = InterBufReader_GetAuInfo;
	intBuf->super.get_current_info = InterBufReader_GetCurrentInfo;
	intBuf->super.get_free_size = InterBufReader_GetFreeSize;
	intBuf->super.get_max_au = InterBufReader_GetMaxAu;
	intBuf->super.get_max_au_length = InterBufReader_GetMaxAuLength;
	intBuf->super.get_max_size = InterBufReader_GetMaxSize;
	intBuf->super.get_used_size = InterBufReader_GetUsedSize;
	intBuf->super.init = InterBufReader_Init;
	intBuf->super.insert_au = InterBufReader_InsertAu;
	intBuf->super.read_au = InterBufReader_ReadAu;
	intBuf->super.read_data = InterBufReader_ReadData;
	intBuf->super.update_au = InterBufReader_UpdateAu;
	intBuf->super.write_data = InterBufReader_WriteData;

#if (ADEC_OPT_BUF_USE_REPEAT == 1)
	intBuf->super.set_repeat = InterBufReader_SetRepeat;
#endif

	intBuf->comBuf = (ADEC_COMMON_BUF_T*)_commonBufferStruct;
	intBuf->adap = _adapIf;
	intBuf->regBase = _registerAddr;

	// TODO : Buffer Register Initialize Routine
//	if( ADEC_REG_CheckRegister(_registerAddr) != ADEC_ERR_NONE ||
//		ADEC_REG_CheckRegister((unsigned char*)_registerAddr + 4) != ADEC_ERR_NONE ||
//		ADEC_REG_CheckRegister((unsigned char*)_registerAddr + 8) != ADEC_ERR_NONE
//		// disgusting
//#if (ADEC_OPT_BUF_USE_4REG_PER_BUF == 1)
//		|| ADEC_REG_CheckRegister((unsigned char*)_registerAddr + 8) != ADEC_ERR_NONE
//#endif
//		)
//	{
//		CommonBuffer_Create(
//			NULL,
//			0,
//			0,
//			1,
//			intBuf->adap,
//			_commonBufferStruct,
//			STATUS_REG);
//
//		CommonBuffer_Init(
//			intBuf->adap,
//			intBuf->comBuf,
//			STATUS_REG);
//
//		WRITE_WRITE_INDEX();
//		WRITE_READ_INDEX();
//		WRITE_WRITE_OFFSET();
//		WRITE_READ_OFFSET();
//		WRITE_IS_ALLOCATED();
//	}

	return ADEC_ERR_NONE;
}

static int InterBufReader_Init(void* _buf)
{
	int result;
	ADEC_INTER_BUF_READER_T* intBuf;

	ADEC_BUF_ASSERT(_buf != NULL,
		NULL,
		NULL,
		ADEC_ERR_NULL,
		"_buf should not be null.\n");

	intBuf = (ADEC_INTER_BUF_READER_T*) _buf;

	UPDATE_STRUCT();

	/*result = CommonBuffer_Init(
		intBuf->adap,
		intBuf->comBuf,
		STATUS_REG);*/

	READ_WRITE_INDEX();
	READ_WRITE_OFFSET();

	intBuf->status.readIndex = intBuf->status.writeIndex;
	intBuf->status.readOffset = intBuf->status.writeOffset;

	//WRITE_WRITE_INDEX();
	WRITE_READ_INDEX();
	//WRITE_WRITE_OFFSET();
	WRITE_READ_OFFSET();
	//WRITE_IS_ALLOCATED();

	WRITE_STRUCT();

	return result;
}

static int InterBufReader_GetFreeSize(void* _buf)
{
	int result;
	ADEC_INTER_BUF_READER_T* intBuf;

	ADEC_BUF_ASSERT(_buf != NULL,
		NULL,
		NULL,
		ADEC_ERR_NULL,
		"_buf should not be null.\n");

	intBuf = (ADEC_INTER_BUF_READER_T*) _buf;

	READ_WRITE_OFFSET();
	READ_READ_OFFSET();

	result = CommonBuffer_GetFreeSize(
		intBuf->adap,
		intBuf->comBuf,
		STATUS_REG);

	return result;
}

static int InterBufReader_GetUsedSize(void* _buf)
{
	int result;
	ADEC_INTER_BUF_READER_T* intBuf;

	ADEC_BUF_ASSERT(_buf != NULL,
		NULL,
		NULL,
		ADEC_ERR_NULL,
		"_buf should not be null.\n");

	intBuf = (ADEC_INTER_BUF_READER_T*) _buf;

	READ_WRITE_OFFSET();
	READ_READ_OFFSET();

	result = CommonBuffer_GetUsedSize(
		intBuf->adap,
		intBuf->comBuf,
		STATUS_REG);

	return result;
}

static int InterBufReader_GetAuCnt(void* _buf)
{
	int result;
	ADEC_INTER_BUF_READER_T* intBuf;

	ADEC_BUF_ASSERT(_buf != NULL,
		NULL,
		NULL,
		ADEC_ERR_NULL,
		"_buf should not be null.\n");

	intBuf = (ADEC_INTER_BUF_READER_T*) _buf;

	READ_WRITE_INDEX();
	READ_READ_INDEX();
	READ_IS_ALLOCATED();

	result = CommonBuffer_GetAuCnt(
		intBuf->adap,
		intBuf->comBuf,
		STATUS_REG);

	return result;
}

static int InterBufReader_GetMaxSize(void* _buf)
{
	int result;
	ADEC_INTER_BUF_READER_T* intBuf;

	ADEC_BUF_ASSERT(_buf != NULL,
		NULL,
		NULL,
		ADEC_ERR_NULL,
		"_buf should not be null.\n");

	intBuf = (ADEC_INTER_BUF_READER_T*) _buf;

	result = CommonBuffer_GetMaxSize(
		intBuf->adap,
		intBuf->comBuf,
		STATUS_REG);

	return result;
}

static int InterBufReader_GetMaxAu(void* _buf)
{
	int result;
	ADEC_INTER_BUF_READER_T* intBuf;

	ADEC_BUF_ASSERT(_buf != NULL,
		NULL,
		NULL,
		ADEC_ERR_NULL,
		"_buf should not be null.\n");

	intBuf = (ADEC_INTER_BUF_READER_T*) _buf;

	result = CommonBuffer_GetMaxAu(
		intBuf->adap,
		intBuf->comBuf,
		STATUS_REG);

	return result;
}

static int InterBufReader_GetMaxAuLength(void* _buf)
{
	int result;
	ADEC_INTER_BUF_READER_T* intBuf;

	ADEC_BUF_ASSERT(_buf != NULL,
		NULL,
		NULL,
		ADEC_ERR_NULL,
		"_buf should not be null.\n");

	intBuf = (ADEC_INTER_BUF_READER_T*) _buf;

	result = CommonBuffer_GetMaxAuLength(
		intBuf->adap,
		intBuf->comBuf,
		STATUS_REG);

	return result;
}

static int InterBufReader_CreateNewAu(
	ADEC_AU_INFO_T* _info,
	void* _buf)
{
	return -ADEC_ERR_PERMISSION;
}

static int InterBufReader_WriteData(
	unsigned char* _src,
	int _length,
	void* _buf)
{
	return -ADEC_ERR_PERMISSION;
}

static int InterBufReader_InsertAu(
	unsigned char* _src,
	int _length,
	ADEC_AU_INFO_T* _info,
	void* _buf)
{
	return -ADEC_ERR_PERMISSION;
}

static int InterBufReader_UpdateAu(
	ADEC_AU_INFO_T* _info,
	void* _buf)
{
	return -ADEC_ERR_PERMISSION;
}

static int InterBufReader_GetAuInfo(
	int _index,
	_OUT_ int* _offset,
	_OUT_ ADEC_AU_INFO_T* _au_info,
	void* _buf)
{
	int result;
	ADEC_INTER_BUF_READER_T* intBuf;

	ADEC_BUF_ASSERT(_buf != NULL,
		NULL,
		NULL,
		ADEC_ERR_NULL,
		"_buf should not be null.\n");

	intBuf = (ADEC_INTER_BUF_READER_T*) _buf;

	READ_READ_INDEX();
	READ_WRITE_INDEX();
	READ_IS_ALLOCATED();

	result = CommonBuffer_GetAuInfo(
		_index,
		_offset,
		_au_info,
		intBuf->adap,
		intBuf->comBuf,
		STATUS_REG);

	return result;
}

static int InterBufReader_GetCurrentInfo(
	_OUT_ int* _offset,
	_OUT_ ADEC_AU_INFO_T* _au_info,
	void* _buf)
{
	int result;
	ADEC_INTER_BUF_READER_T* intBuf;

	ADEC_BUF_ASSERT(_buf != NULL,
		NULL,
		NULL,
		ADEC_ERR_NULL,
		"_buf should not be null.\n");

	intBuf = (ADEC_INTER_BUF_READER_T*) _buf;

	READ_READ_INDEX();
	READ_WRITE_INDEX();
	READ_IS_ALLOCATED();

	result = CommonBuffer_GetCurrentInfo(
		_offset,
		_au_info,
		intBuf->adap,
		intBuf->comBuf,
		STATUS_REG);

	return result;
}

static int InterBufReader_ReadData(
	_OUT_ unsigned char** _dst,
	_IN_OUT_ int* _size,
	_OUT_ int* _au_in_data,
	void* _buf)
{
	int result;
	ADEC_INTER_BUF_READER_T* intBuf;

	ADEC_BUF_ASSERT(_buf != NULL,
		NULL,
		NULL,
		ADEC_ERR_NULL,
		"_buf should not be null.\n");

	intBuf = (ADEC_INTER_BUF_READER_T*) _buf;

	READ_WRITE_INDEX();
	READ_READ_INDEX();
	READ_IS_ALLOCATED();
	READ_WRITE_OFFSET();
	READ_READ_OFFSET();

	result = CommonBuffer_ReadData(
		_dst,
		_size,
		_au_in_data,
		intBuf->adap,
		intBuf->comBuf,
		STATUS_REG);

	return result;
}

static int InterBufReader_FlushData(
	int _size,
	void* _buf)
{
	int result;
	int previousRepeat;
	ADEC_INTER_BUF_READER_T* intBuf;

	ADEC_BUF_ASSERT(_buf != NULL,
		NULL,
		NULL,
		ADEC_ERR_NULL,
		"_buf should not be null.\n");

	intBuf = (ADEC_INTER_BUF_READER_T*) _buf;

	READ_WRITE_INDEX();
	READ_READ_INDEX();
	READ_IS_ALLOCATED();
	READ_WRITE_OFFSET();
	READ_READ_OFFSET();

#if (ADEC_OPT_BUF_USE_REPEAT == 1)
	previousRepeat = intBuf->comBuf->repeatCnt;
#endif

	result = CommonBuffer_FlushData(
		&_size,
		intBuf->adap,
		intBuf->comBuf,
		STATUS_REG);

	WRITE_READ_INDEX();
	WRITE_READ_OFFSET();

#if (ADEC_OPT_BUF_USE_REPEAT == 1)
	if(previousRepeat != intBuf->comBuf->repeatCnt)
	{
		WRITE_WRITE_INDEX();
		WRITE_WRITE_OFFSET();
		WRITE_STRUCT();
	}
#endif
	return result;
}

static int InterBufReader_ReadAu(
	_OUT_ unsigned char** _dst,
	_OUT_ int* _size,
	_IN_OUT_ int* _au_cnt,
	void* _buf)
{
	int result;
	ADEC_INTER_BUF_READER_T* intBuf;

	ADEC_BUF_ASSERT(_buf != NULL,
		NULL,
		NULL,
		ADEC_ERR_NULL,
		"_buf should not be null.\n");

	intBuf = (ADEC_INTER_BUF_READER_T*) _buf;

	READ_WRITE_INDEX();
	READ_READ_INDEX();
	READ_IS_ALLOCATED();
	READ_WRITE_OFFSET();
	READ_READ_OFFSET();

	result = CommonBuffer_ReadAu(
		_dst,
		_size,
		_au_cnt,
		intBuf->adap,
		intBuf->comBuf,
		STATUS_REG);

	return result;
}

static int InterBufReader_SetRepeat(int _cnt,
	void* _buf)
{
	return -ADEC_ERR_PERMISSION;
}
