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
 *	@file adec_platform_tmpl.h
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
#ifndef __ADEC_PLATFORM_TMPL_H__
#define __ADEC_PLATFORM_TMPL_H__

#define ADEC_CACHE_WRITEBACK(__ADDR, __LEN)
#define ADEC_CACHE_INVALIDATE(__ADDR, __LEN)
// Get Current Time in melesecond scale
#define ADEC_GET_CURRENT_CLOCK()

#define ADEC_MUTEX_DECLARE(__MUTEX)
#define ADEC_MUTEX_CREATE(__P_MUTEX)
#define ADEC_MUTEX_LOCK(__P_MUTEX)
#define ADEC_MUTEX_RELEASE(__P_MUTEX)
#define ADEC_MUTEX_DESTROY(__P_MUTEX)

#define __STD_MALLOC(...)
#define __STD_FREE(...)
#define __STD_MEMCPY(...)
#define __STD_MEMSET(...)
#define __STD_MEMMOVE(...)

#define __ADEC_DBG(__INDEX,__FMT,...)
#define __ADEC_DBG_ERR(__INDEX,__FMT,...)
#define __ADEC_DBG_LOW(__INDEX,__FMT,...)
#define __ADEC_DBG_WARN(__INDEX,__FMT,...)

#define __ADEC_ASSERT(__COND, __ACTION)
#define __ADEC_ASSERT_STR(__COND, __ACTION, __STR,...)

#define ADEC_INLINE

#define __PRT64X

#define __ADEC_FILED_INIT(__FIELD,__VAL)

#define ADEC_SECT_IRAM0_TEXT
#define ADEC_SECT_IRAM1_TEXT
#define ADEC_SECT_IRAM0_TEXT_LOW
#define ADEC_SECT_IRAM1_TEXT_LOW
#define ADEC_SECT_DRAM_DATA
#define ADEC_SECT_DRAM_BSS
#define ADEC_SECT_DRAM_RONLY



#endif //__ADEC_PLATFORM_TMPL_H__
/** @} */
