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

#ifndef _ADEC_COMMON_BUF_H_
#define _ADEC_COMMON_BUF_H_

#include "adec_buf.h"
#include "common/adec_common.h"
#include "common/adec_processor_adap.h"


///////////////////////////////////////////////////////////////////////////////
// Type Definition
///////////////////////////////////////////////////////////////////////////////

typedef struct _ADEC_COM_BUF_AU_INFO_T
{
	ADEC_AU_INFO_T			base;
	int						bufOffset;
	int						complete;
}ADEC_COM_BUF_AU_INFO_T;

typedef struct _ADEC_COMMON_BUF_STATUS_T
{
	// Status Variable
	int						indexAllocated;
	int						readOffset;
	int						writeOffset;
	short					readIndex;
	short					writeIndex;
	int						totalReadCnt;
	int						totalWriteCnt;
}ADEC_COMMON_BUF_STATUS_T;

typedef struct _ADEC_COMMON_BUF_T
{
	unsigned char*			bufferStart;
	// Characteristic Variable
	int						maxSize;
	int						accessSize;
	int						maxIndex;

#if (ADEC_OPT_BUF_USE_REPEAT == 1)
	// Repeat support
	int						repeatCnt;
	int						repeatStartOffset;
	int						repeatEndOffset;
	int						repeatLength;
	int						repeatStartIndex;
	int						repeatEndIndex;
	int						repeatIndexCnt;
#endif

#if (ADEC_OPT_BUF_USE_REWIND == 1)
	// Rewind support
	int						totalConsumed;
	int						totalIndex;
#endif

	ADEC_COM_BUF_AU_INFO_T	indexTable[1];
}ADEC_COMMON_BUF_T;

#if (ADEC_OPT_BUF_USE_4REG_PER_BUF == 1)
typedef union _ADEC_COMMON_BUF_STATUS_REG_T
{
	struct
	{
		unsigned int	writeOffset	:28;
		unsigned int	chkBit1		:4;

		unsigned int	readOffset	:28;
		unsigned int	chkBit2		:4;

		unsigned int	writeIndex	:27;
		unsigned int	isAllocated	:1;
		unsigned int	chkBit3		:4;

		unsigned int	readIndex	:27;
		unsigned int	reserved	:1;
		unsigned int	chkBit4		:4;
	}field;
	struct
	{
		unsigned int	reg[4];
	}reg;
}ADEC_COMMON_BUF_STATUS_REG_T;
#else
typedef union _ADEC_COMMON_BUF_STATUS_REG_T
{
	struct
	{
		unsigned int	writeOffset	:27;
		unsigned int	isAllocated	:1;
		unsigned int	chkBit1		:4;

		unsigned int	readOffset	:27;
		unsigned int	reserved1	:1;
		unsigned int	chkBit2		:4;

		unsigned int	writeIndex	:14;
		unsigned int	readIndex	:14;
		unsigned int	chkBit3		:4;
	}field;
	struct
	{
		unsigned int	reg[3];
	}reg;
}ADEC_COMMON_BUF_STATUS_REG_T;
#endif

///////////////////////////////////////////////////////////////////////////////
// Extern variables
///////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////
// Function Declaration
///////////////////////////////////////////////////////////////////////////////

//=============================================================================
// Caution
//=============================================================================
// Do not use bellow functions directly.
// Please use ADEC_BUF_T implementations such as ADEC_INTERNAL_BUF_T.
//-----------------------------------------------------------------------------

int CommonBuffer_GetStructSize(int _indexCount);

int CommonBuffer_GetStatusStructSize(void);

int CommonBuffer_Create(
	unsigned char* _baseAddr,
	int _bufferSize,
	int _maxAuSize,
	int _indexCount,
	ADEC_PROCESSOR_ADAP_IF_T* _adap,
	void* _buf,
	void* _status);

int CommonBuffer_Init(
	ADEC_PROCESSOR_ADAP_IF_T* _adap,
	ADEC_COMMON_BUF_T* _buf,
	ADEC_COMMON_BUF_STATUS_T* _status);

int CommonBuffer_GetFreeSize(
	ADEC_PROCESSOR_ADAP_IF_T* _adap,
	ADEC_COMMON_BUF_T* _buf,
	ADEC_COMMON_BUF_STATUS_T* _status)
ADEC_SECT_IRAM0_TEXT;

int CommonBuffer_GetUsedSize(
	ADEC_PROCESSOR_ADAP_IF_T* _adap,
	ADEC_COMMON_BUF_T* _buf,
	ADEC_COMMON_BUF_STATUS_T* _status)
ADEC_SECT_IRAM0_TEXT;

int CommonBuffer_GetAuCnt(
	ADEC_PROCESSOR_ADAP_IF_T* _adap,
	ADEC_COMMON_BUF_T* _buf,
	ADEC_COMMON_BUF_STATUS_T* _status);

int CommonBuffer_GetMaxSize(
	ADEC_PROCESSOR_ADAP_IF_T* _adap,
	ADEC_COMMON_BUF_T* _buf,
	ADEC_COMMON_BUF_STATUS_T* _status);

int CommonBuffer_GetMaxAu(
	ADEC_PROCESSOR_ADAP_IF_T* _adap,
	ADEC_COMMON_BUF_T* _buf,
	ADEC_COMMON_BUF_STATUS_T* _status);

int CommonBuffer_GetMaxAuLength(
	ADEC_PROCESSOR_ADAP_IF_T* _adap,
	ADEC_COMMON_BUF_T* _buf,
	ADEC_COMMON_BUF_STATUS_T* _status);

int CommonBuffer_CreateNewAu(
	ADEC_AU_INFO_T* _au,
	ADEC_PROCESSOR_ADAP_IF_T* _adap,
	ADEC_COMMON_BUF_T* _buf,
	ADEC_COMMON_BUF_STATUS_T* _status);

int CommonBuffer_WriteData(
	unsigned char* _src,
	int _length,
	ADEC_PROCESSOR_ADAP_IF_T* _adap,
	ADEC_COMMON_BUF_T* _buf,
	ADEC_COMMON_BUF_STATUS_T* _status);

int CommonBuffer_InsertAu(
	unsigned char* _src,
	int _length,
	ADEC_AU_INFO_T* _au,
	ADEC_PROCESSOR_ADAP_IF_T* _adap,
	ADEC_COMMON_BUF_T* _buf,
	ADEC_COMMON_BUF_STATUS_T* _status);

int CommonBuffer_UpdateAu(
	ADEC_AU_INFO_T* _au,
	ADEC_PROCESSOR_ADAP_IF_T* _adap,
	ADEC_COMMON_BUF_T* _buf,
	ADEC_COMMON_BUF_STATUS_T* _status);

int CommonBuffer_GetAuInfo(
	int index,
	_OUT_ int* _offset,
	_OUT_ ADEC_AU_INFO_T* _au,
	ADEC_PROCESSOR_ADAP_IF_T* _adap,
	ADEC_COMMON_BUF_T* _buf,
	ADEC_COMMON_BUF_STATUS_T* _status);

int CommonBuffer_GetCurrentInfo(
	_OUT_ int* _offset,
	_OUT_ ADEC_AU_INFO_T* _au,
	ADEC_PROCESSOR_ADAP_IF_T* _adap,
	ADEC_COMMON_BUF_T* _buf,
	ADEC_COMMON_BUF_STATUS_T* _status);

int CommonBuffer_ReadData(
	_OUT_ unsigned char** _dst,
	_IN_OUT_ int* _size,
	_OUT_ int* _au_in_data,
	ADEC_PROCESSOR_ADAP_IF_T* _adap,
	ADEC_COMMON_BUF_T* _buf,
	ADEC_COMMON_BUF_STATUS_T* _status);

int CommonBuffer_FlushData(
	int* _size,
	ADEC_PROCESSOR_ADAP_IF_T* _adap,
	ADEC_COMMON_BUF_T* _buf,
	ADEC_COMMON_BUF_STATUS_T* _status);

int CommonBuffer_ReadAu(
	_OUT_ unsigned char** _dst,
	_OUT_ int* _size,
	_IN_OUT_ int* _au_cnt,
	ADEC_PROCESSOR_ADAP_IF_T* _adap,
	ADEC_COMMON_BUF_T* _buf,
	ADEC_COMMON_BUF_STATUS_T* _status);

int CommonBuffer_SetRepeat(
	_OUT_ int _cnt,
	ADEC_PROCESSOR_ADAP_IF_T* _adap,
	ADEC_COMMON_BUF_T* _buf,
	ADEC_COMMON_BUF_STATUS_T* _status);
#endif
