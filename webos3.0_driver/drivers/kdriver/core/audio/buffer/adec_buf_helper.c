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
#include "adec_buf_helper.h"
#include "adec_buf_dbg.h"
#include "adec_common_buf.h"

int BufHelper_IsLast(void* _buf)
{
	ADEC_AU_INFO_T auInfo;
	ADEC_BUF_T* buf;
	int result;
	int offset;

	ADEC_BUF_ASSERT(_buf != NULL,
		NULL,
		NULL,
		ADEC_ERR_NULL,
		"_buf should not be null\n");

	buf = (ADEC_BUF_T*)_buf;

	result = buf->get_au_info(-1, &offset, &auInfo, buf);

	if(result < 0)
	{
		return result;
	}

	return auInfo.lastAu == 1;
}

int BufHelper_SetLast(void* _buf)
{
	ADEC_AU_INFO_T auInfo;
	ADEC_BUF_T* buf;

	ADEC_BUF_ASSERT(_buf != NULL,
		NULL,
		NULL,
		ADEC_ERR_NULL,
		"_buf should not be null\n");

	buf = (ADEC_BUF_T*)_buf;

	auInfo.discontinuity = 0;
	auInfo.error = 0;
	auInfo.gstc = 0;
	auInfo.index = 0;
	auInfo.lastAu = 1;
	auInfo.size = 0;
	auInfo.timestamp = 0;

	return buf->create_new_au(&auInfo, buf);
}

unsigned int BufHelper_GetTimestamp(void* _buf)
{
	ADEC_AU_INFO_T auInfo;
	ADEC_BUF_T* buf;
	int result;

	ADEC_BUF_ASSERT(_buf != NULL,
		NULL,
		NULL,
		ADEC_ERR_NULL,
		"_buf should not be null\n");

	buf = (ADEC_BUF_T*)_buf;

	result = buf->get_au_info(-1, NULL, &auInfo, buf);

	if(result < 0)
	{
		return 0;
	}

	return auInfo.timestamp;
}

unsigned int BufHelper_GetGstc(void* _buf)
{
	ADEC_AU_INFO_T auInfo;
	ADEC_BUF_T* buf;
	int result;

	ADEC_BUF_ASSERT(_buf != NULL,
		NULL,
		NULL,
		ADEC_ERR_NULL,
		"_buf should not be null\n");

	buf = (ADEC_BUF_T*)_buf;

	result = buf->get_au_info(-1, NULL, &auInfo, buf);

	if(result < 0)
	{
		return 0;
	}

	return auInfo.gstc;
}

