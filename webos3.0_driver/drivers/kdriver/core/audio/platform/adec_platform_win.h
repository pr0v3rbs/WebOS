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
 *	@file adec_platform_win.h
 *
 *
 *	@author		Jung, Kyung Soon (kyungsoon.jung@leg.com)
 *	@version	0.0.1
 *	@date		2012-03-19
 *	@note
 *	@see		http://www.lge.com
 *	@addtogroup ADEC
 *	@{
 */


/******************************************************************************
  Header File Guarder
 ******************************************************************************/
#ifndef __ADEC_PLATFORM_WIN_H__
#define __ADEC_PLATFORM_WIN_H__

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include "common/adec_debug_index.h"
#include "common/adec_config.h"

extern long long _ADEC_WIN_GetCurrentClock();

#define ADEC_SLEEP_MS(x)

#define ADEC_CACHE_WRITEBACK(__ADDR, __LEN)
#define ADEC_CACHE_INVALIDATE(__ADDR, __LEN)
#define ADEC_GET_CURRENT_CLOCK()		_ADEC_WIN_GetCurrentClock();
#define ADEC_MUTEX_DECLARE(__MUTEX)
#define ADEC_MUTEX_CREATE(__MUTEX)
#define ADEC_MUTEX_LOCK(__MUTEX)
#define ADEC_MUTEX_RELEASE(__MUTEX)
#define ADEC_MUTEX_DESTROY(__MUTEX)

#if 0		//20140702 damdam for TDD
#define __ADEC_DBG(__INDEX,__FMT,...)			printf("[%s : %d] "__FMT, __FILE__,__LINE__,##__VA_ARGS__)
#define __ADEC_DBG_ERR(__INDEX,__FMT,...)		printf("[%s : %d] "__FMT, __FILE__,__LINE__,##__VA_ARGS__)
#define __ADEC_DBG_LOW(__INDEX,__FMT,...)		printf("[%s : %d] "__FMT, __FILE__,__LINE__,##__VA_ARGS__)
#define __ADEC_DBG_WARN(__INDEX,__FMT,...)		printf("[%s : %d] "__FMT, __FILE__,__LINE__,##__VA_ARGS__)
#else
#define __ADEC_DBG(__INDEX,__FMT,...)			printf(__FMT,##__VA_ARGS__)
#define __ADEC_DBG_ERR(__INDEX,__FMT,...)		printf(__FMT,##__VA_ARGS__)
#define __ADEC_DBG_LOW(__INDEX,__FMT,...)		printf(__FMT,##__VA_ARGS__)
#define __ADEC_DBG_WARN(__INDEX,__FMT,...)		printf(__FMT,##__VA_ARGS__)
#endif

#define __STD_MALLOC		malloc
#define __STD_FREE			free
#define __STD_MEMCPY		memcpy
#define __STD_MEMSET		memset
#define __STD_MEMMOVE		memmove

#define ADEC_INLINE	__inline

#define __PRT64X	"llX"

// MSVC Cannot used designated initializer
#define __ADEC_FILED_INIT(__FIELD,__VAL)	__VAL

#define _USE_MEM_STAT	1

#if defined(ADEC_OPT_ASSERT_ENABLE) && (ADEC_OPT_ASSERT_ENABLE == 1)
#ifdef _CUNIT
#include "CUnit.h"
#define __ADEC_ASSERT(__COND, __ACTION) if((__COND) ==0){CU_FAIL(#__COND); __ACTION;}
#define __ADEC_ASSERT_STR(__COND, __ACTION, __STR,...) if((__COND) ==0){ADEC_DBG(ADEC_DBG_INDEX_ASSERT,__STR,##__VA_ARGS__);CU_FAIL(#__COND);__ACTION;}
#else
#if defined(ADEC_OPT_USE_SYSTEM_ASSERT) && ADEC_OPT_USE_SYSTEM_ASSERT == 1
#include <assert.h>
#define __ADEC_ASSERT(__COND, __ACTION)	assert(__COND)
#define __ADEC_ASSERT_STR(__COND, __ACTION, __STR,...) assert(__COND)
#else
#define __ADEC_ASSERT(__COND, __ACTION) if((__COND) ==0){__ACTION;}
#define __ADEC_ASSERT_STR(__COND, __ACTION, __STR,...) if((__COND) ==0){__ACTION;}
#endif
#endif
#else
#define __ADEC_ASSERT(__COND, __ACTION)
#define __ADEC_ASSERT_STR(__COND, __STR, __ACTION)
#endif

#define ADEC_SECT_IRAM0_TEXT
#define ADEC_SECT_IRAM1_TEXT
#define ADEC_SECT_IRAM0_TEXT_LOW
#define ADEC_SECT_IRAM1_TEXT_LOW
#define ADEC_SECT_DRAM_DATA
#define ADEC_SECT_DRAM_BSS
#define ADEC_SECT_DRAM_RONLY

#endif //__ADEC_PLATFORM_WIN_H__
/** @} */
