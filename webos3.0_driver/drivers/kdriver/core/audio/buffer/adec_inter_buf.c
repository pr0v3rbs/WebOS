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

#include "adec_inter_buf.h"
#include "adec_inter_buf_reader.h"
#include "adec_inter_buf_writer.h"


int IntBuf_GetStructSize(ADEC_INTER_BUF_TYPE_T _bufType)
{
	return 0;
}

int IntBuf_Create(
	ADEC_PROCESSOR_TYPE_T _currentProcessor,
	ADEC_PROCESSOR_TYPE_T _comBufOwner,
	ADEC_INTER_BUF_TYPE_T _bufType,
	void* _commonBufAddr,
	void* _struct)
{
	return 0;
}
