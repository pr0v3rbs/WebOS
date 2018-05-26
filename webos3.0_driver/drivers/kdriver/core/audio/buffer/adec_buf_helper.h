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

#ifndef _ADEC_BUF_HELPER_H_
#define _ADEC_BUF_HELPER_H_

#include "adec_buf.h"

extern int BufHelper_IsLast(void* _buf);
extern unsigned int BufHelper_GetTimestamp(void* _buf);
extern unsigned int BufHelper_GetGstc(void* _buf);
extern int BufHelper_SetLast(void* _buf);

#endif
