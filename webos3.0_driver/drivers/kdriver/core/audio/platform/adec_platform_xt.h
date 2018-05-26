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



/**
 *	@file adec_platform_xt.h
 *
 *
 *	@author		Jung, Kyung Soon (kyungsoon.jung@leg.com)
 *	@version	0.0.1
 *	@date		2012-05-02
 *	@note
 *	@see		http://www.lge.com
 *	@addtogroup ADEC
 *	@{
 */


/******************************************************************************
  Header File Guarder
 ******************************************************************************/
#ifndef __ADEC_PLATFORM_XT_H__
#define __ADEC_PLATFORM_XT_H__

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <xtensa/hal.h>
#include <assert.h>
#include <stdio.h>
#include "common/adec_config.h"
#include "common/adec_log.h"
#include "common/adec_debug_print.h"

extern void Xt_AssertBreak(void);

#define ADEC_CACHE_WRITEBACK(__ADDR, __LEN) 	xthal_dcache_region_writeback(__ADDR, __LEN);
#define ADEC_CACHE_INVALIDATE(__ADDR, __LEN)	xthal_dcache_region_invalidate(__ADDR, __LEN);

#define ADEC_SLEEP_MS(x)

// Get Current Time in melesecond scale
#define ADEC_GET_CURRENT_CLOCK()	0
#define ADEC_MUTEX_DECLARE(__MUTEX)
#define ADEC_MUTEX_CREATE(__P_MUTEX)
#define ADEC_MUTEX_LOCK(__P_MUTEX)
#define ADEC_MUTEX_RELEASE(__P_MUTEX)
#define ADEC_MUTEX_DESTROY(__P_MUTEX)

#define __ADEC_DBG(__INDEX,__FMT,...)			_REMOTE_DBG_PRINT(__INDEX,IPC_DEBUG_NORMAL,__FMT,##__VA_ARGS__)
#define __ADEC_DBG_ERR(__INDEX,__FMT,...)		_REMOTE_DBG_PRINT(__INDEX,IPC_DEBUG_ERROR,__FMT,##__VA_ARGS__)
#define __ADEC_DBG_LOW(__INDEX,__FMT,...)		_REMOTE_DBG_PRINT(__INDEX,IPC_DEBUG_LOW,__FMT,##__VA_ARGS__)
#define __ADEC_DBG_WARN(__INDEX,__FMT,...)		_REMOTE_DBG_PRINT(__INDEX,IPC_DEBUG_WARN,__FMT,##__VA_ARGS__)

#if defined(ADEC_OPT_USE_ALL_FW_ASSERT) && (ADEC_OPT_USE_ALL_FW_ASSERT == 1)
#define __ADEC_ASSERT(__COND, __ACTION)		\
        if((__COND)==0){\
 			Xt_AssertBreak();\
 			__ADEC_DBG_ERR(ADEC_DBG_INDEX_ASSERT,"Exception raised by [%s][%d]\n",__FUNCTION__,__LINE__);\
 			__ADEC_DBG_ERR(ADEC_DBG_INDEX_ASSERT,"Fail : "#__COND"\n");\
 			__ACTION;}
#define __ADEC_ASSERT_STR(__COND, __ACTION, __STR,...) 	\
        if((__COND)==0){\
 			Xt_AssertBreak();\
 			__ADEC_DBG_ERR(ADEC_DBG_INDEX_ASSERT,"Exception raised by [%s][%d]\n",__FUNCTION__,__LINE__);\
 			__ADEC_DBG_ERR(ADEC_DBG_INDEX_ASSERT,"Fail : "#__COND"\n");\
			__ADEC_DBG_ERR(ADEC_DBG_INDEX_ASSERT,__STR,##__VA_ARGS__);\
 			__ACTION;}
#else
#define __ADEC_ASSERT(__COND, __ACTION)		//if((__COND)==0){Xt_AssertBreak();__ACTION;}
#define __ADEC_ASSERT_STR(__COND, __ACTION, __STR,...) 	//if((__COND)==0){Xt_AssertBreak();__ACTION;}
#endif

#define __ADEC_DBG_ASSERT(__COND, __ACTION)		\
        if((__COND)==0){\
 			Xt_AssertBreak();\
 			__ADEC_DBG_ERR(ADEC_DBG_INDEX_ASSERT,"Exception raised by [%s][%d]\n",__FUNCTION__,__LINE__);\
 			__ADEC_DBG_ERR(ADEC_DBG_INDEX_ASSERT,"Fail : "#__COND"\n");\
 			__ACTION;}
#define __ADEC_DBG_ASSERT_STR(__COND, __ACTION, __STR,...) 	\
        if((__COND)==0){\
 			Xt_AssertBreak();\
 			__ADEC_DBG_ERR(ADEC_DBG_INDEX_ASSERT,"Exception raised by [%s][%d]\n",__FUNCTION__,__LINE__);\
 			__ADEC_DBG_ERR(ADEC_DBG_INDEX_ASSERT,"Fail : "#__COND"\n");\
			__ADEC_DBG_ERR(ADEC_DBG_INDEX_ASSERT,__STR,##__VA_ARGS__);\
 			__ACTION;}

#define __STD_MALLOC		malloc
#define __STD_FREE			free
#define __STD_MEMCPY		memcpy
#define __STD_MEMMOVE		memmove

#define __STD_MEMSET		memset
#define __STD_MEMMOVE		memmove

#define ADEC_INLINE	inline

#define __PRT64X	"llX"

#define __ADEC_FILED_INIT(__FIELD,__VAL)	__FIELD = __VAL

#if (defined(_USE_INTERNAL_RAM) && (_USE_INTERNAL_RAM == 1))
#define ADEC_SECT_IRAM0_TEXT		__attribute__((section(".iram0.text")))
#define ADEC_SECT_IRAM1_TEXT		__attribute__((section(".iram1.text")))
#if _DISABLE_LOW_PRIORITY_IRAM == 1
#define ADEC_SECT_IRAM0_TEXT_LOW
#define ADEC_SECT_IRAM1_TEXT_LOW
#else
#define ADEC_SECT_IRAM0_TEXT_LOW	__attribute__((section(".iram0.text")))
#define ADEC_SECT_IRAM1_TEXT_LOW	__attribute__((section(".iram1.text")))
#endif
#define ADEC_SECT_DRAM_DATA			__attribute__((section(".dram1.data")))
#define ADEC_SECT_DRAM_BSS			__attribute__((section(".dram1.bss")))
#define ADEC_SECT_DRAM_RONLY		__attribute__((section(".dram1.rodata")))
#else
// Using internal ram with L9 configuration cuase serious problem
#define ADEC_SECT_IRAM0_TEXT
#define ADEC_SECT_IRAM1_TEXT
#define ADEC_SECT_IRAM0_TEXT_LOW
#define ADEC_SECT_IRAM1_TEXT_LOW
#define ADEC_SECT_DRAM_DATA
#define ADEC_SECT_DRAM_BSS
#define ADEC_SECT_DRAM_RONLY
#endif

#endif //__ADEC_PLATFORM_TMPL_H__
/** @} */
