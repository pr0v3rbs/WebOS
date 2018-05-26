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

#include "adec_processor_adap.h"

void _CommonBuffer_CacheControl_Noop(void* _addr, unsigned int _length, void* _adap);
unsigned char* _CommonBuffer_AddrTrans_Noop(unsigned char* _addr, void* _adap);

ADEC_PROCESSOR_ADAP_IF_T gProcessorAdapNoop = {
	_CommonBuffer_CacheControl_Noop,	//pfnInvalidate
	_CommonBuffer_CacheControl_Noop,	//pfnWriteback
	_CommonBuffer_AddrTrans_Noop,		//pfnAddrTrans
};

ADEC_PROCESSOR_ADAP_IF_T* DEFAULT_PROCESSOR_ADAP = &gProcessorAdapNoop;

void _CommonBuffer_CacheControl_Noop(void* _addr, unsigned int _length, void* _adap)
{
    return;
}

unsigned char* _CommonBuffer_AddrTrans_Noop(unsigned char* _addr, void* _adap)
{
    return _addr;
}
