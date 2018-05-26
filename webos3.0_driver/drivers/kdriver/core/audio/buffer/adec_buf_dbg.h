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

#ifndef _ADEC_BUF_DBG_H_
#define _ADEC_BUF_DBG_H_


#include "common/adec_config.h"
#include "common/adec_debug.h"

extern void BufDbg_AssertBreak(void);

#if (ADEC_OPT_BUF_USE_DEBUG == 1)
	#define ADEC_BUF_PRINT(...)			ADEC_DBG(ADEC_DBG_INDEX_NOT_DEF,##__VA_ARGS__)
	#define ADEC_BUF_LOW(...)			ADEC_DBG_LOW(ADEC_DBG_INDEX_NOT_DEF,##__VA_ARGS__)
	#define ADEC_BUF_ERR(...)			ADEC_DBG_ERR(ADEC_DBG_INDEX_NOT_DEF,##__VA_ARGS__)
#else
	#define ADEC_BUF_PRINT(...)
	#define ADEC_BUF_LOW(...)
	#define ADEC_BUF_ERR(...)
#endif

#if (ADEC_OPT_ASSERT_ENABLE == 1 && ADEC_OPT_BUF_USE_ASSERT == 1)
	#ifdef _WIN32
		#ifdef _CUNIT
			#include "CUnit.h"
			#define ADEC_BUF_ASSERT(_COND, _STR, _STAT, _RETVAL, ...)	if((_COND) ==0){CU_FAIL_FATAL(#_COND); return -_RETVAL;}
		#else
			#include <assert.h>
			#define ADEC_BUF_ASSERT(_COND, _STR, _STAT, _RETVAL, ...)   assert(_COND);
		#endif
	#else
		#define ADEC_BUF_ASSERT(_COND, _STR, _STAT, _RETVAL, ...)	\
		if((_COND) == 0){\
			BufDbg_AssertBreak();\
			ADEC_DBG_ERR(ADEC_DBG_INDEX_ASSERT,"Exception raised by [%s][%d]\n",__FUNCTION__,__LINE__);\
			ADEC_DBG_ERR(ADEC_DBG_INDEX_ASSERT,"Fail : "#_COND"\n");\
			ADEC_DBG_ERR(ADEC_DBG_INDEX_ASSERT,__VA_ARGS__);\
			if(_STR){\
				ADEC_BUF_LOW("  addr : 0x%p\n",((ADEC_COMMON_BUF_T*)_STR)->bufferStart);\
				ADEC_BUF_LOW("   max : %d\n",((ADEC_COMMON_BUF_T*)_STR)->maxSize);\
				ADEC_BUF_LOW("access : %d\n",((ADEC_COMMON_BUF_T*)_STR)->accessSize);\
				ADEC_BUF_LOW(" index : %d\n",((ADEC_COMMON_BUF_T*)_STR)->maxIndex);\
			}\
			if(_STAT){\
				ADEC_BUF_LOW("    wo : %d\n",((ADEC_COMMON_BUF_STATUS_T*)_STAT)->writeOffset);\
				ADEC_BUF_LOW("    ro : %d\n",((ADEC_COMMON_BUF_STATUS_T*)_STAT)->readOffset);\
				ADEC_BUF_LOW("    wi : %d\n",((ADEC_COMMON_BUF_STATUS_T*)_STAT)->writeIndex);\
				ADEC_BUF_LOW("    ri : %d\n",((ADEC_COMMON_BUF_STATUS_T*)_STAT)->readIndex);\
				ADEC_BUF_LOW("    ia : %d\n",((ADEC_COMMON_BUF_STATUS_T*)_STAT)->indexAllocated);\
			}\
			return -_RETVAL;\
		}
	#endif
#else
	#define ADEC_BUF_ASSERT(...)
#endif

#endif

