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

#ifndef _ADEC_INTER_BUF_READER_H_
#define _ADEC_INTER_BUF_READER_H_

#include "adec_common_buf.h"

#ifdef __cplusplus
extern "C" {
#endif

int InterBufReader_GetStructSize(void);

int InterBufReader_CreateFromCommonBuffer(
	ADEC_PROCESSOR_ADAP_IF_T* _adapIf,
	void* _registerAddr,
	void* _commonBufferStruct,
	void* _struct);

#ifdef __cplusplus
}
#endif


#endif
