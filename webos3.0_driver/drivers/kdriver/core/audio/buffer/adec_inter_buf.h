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

#ifndef _ADEC_INTER_BUF_H_
#define _ADEC_INTER_BUF_H_

#ifdef __cplusplus
extern "C" {
#endif
	typedef enum _ADEC_PROCESSOR_TYPE_T
	{
		ADEC_PROCESSOR_NOT_DEF = 0,
		ADEC_PROCESSOR_TEST,
		ADEC_PROCESSOR_ARM,
		ADEC_PROCESSOR_DSP0,
		ADEC_PROCESSOR_DSP1,
	}ADEC_PROCESSOR_TYPE_T;

	typedef enum _ADEC_INTER_BUF_TYPE_T
	{
		ADEC_INTER_BUF_NOT_DEF,
		ADEC_INTER_BUF_READER,
		ADEC_INTER_BUF_WRITER,
	}ADEC_INTER_BUF_TYPE_T;

	int IntBuf_GetStructSize(ADEC_INTER_BUF_TYPE_T _bufType);

	int IntBuf_Create(
		ADEC_PROCESSOR_TYPE_T _currentProcessor,
		ADEC_PROCESSOR_TYPE_T _comBufOwner,
		ADEC_INTER_BUF_TYPE_T _bufType,
		void* _commonBufAddr,
		void* _struct);

#ifdef __cplusplus
}
#endif

#endif
