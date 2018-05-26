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

#ifndef _ADEC_PROCESSOR_ADAP_H_
#define _ADEC_PROCESSOR_ADAP_H_

typedef unsigned char* (*PFN_ADEC_ADDR_TRANSLATOR)(unsigned char* _addr, void* _adap);
typedef void (*PFN_ADEC_CACHE_CONTROL)(void* _addr, unsigned int _length, void* _adap);

typedef struct _ADEC_PROCESSOR_ADAP_IF_T
{
	PFN_ADEC_CACHE_CONTROL		pfnInvalidate;
	PFN_ADEC_CACHE_CONTROL		pfnWriteback;
	PFN_ADEC_ADDR_TRANSLATOR	pfnAddrTrans;
}ADEC_PROCESSOR_ADAP_IF_T;

extern ADEC_PROCESSOR_ADAP_IF_T* DEFAULT_PROCESSOR_ADAP;

#endif
