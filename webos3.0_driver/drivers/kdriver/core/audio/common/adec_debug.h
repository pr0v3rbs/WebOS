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
 *	@file adec_debug.h
 *
 *
 *	@author		Jung, Kyung Soon (kyungsoon.jung@leg.com)
 *	@version	0.0.1
 *	@date		2012-03-28
 *	@note
 *	@see		http://www.lge.com
 *	@addtogroup ADEC
 *	@{
 */


/******************************************************************************
  Header File Guarder
 ******************************************************************************/
#ifndef __ADEC_DEBUG_H__
#define __ADEC_DEBUG_H__

#include "adec_common.h"
#include "adec_debug_index.h"


#if (ADEC_OPT_DBG_ENABLE == 1)
#define ADEC_DBG			__ADEC_DBG
#else
#define ADEC_DBG(...)
#endif

#if (ADEC_OPT_DBG_ERROR_ENABLE == 1)
#define ADEC_DBG_ERR		__ADEC_DBG_ERR
#else
#define ADEC_DBG_ERR(...)
#endif

#if (ADEC_OPT_DBG_LOW_ENABLE == 1)
#define ADEC_DBG_LOW		__ADEC_DBG_LOW
#else
#define ADEC_DBG_LOW(...)
#endif

#if (ADEC_OPT_DBG_ASSERT_ENABLE == 1)
/* F/W ASSERT 검증을 위해 임시추가. Tenslica 이외(ARM, WIN) 사용금지. */
#define ADEC_DBG_ASSERT			__ADEC_DBG_ASSERT
#define ADEC_DBG_ASSERT_STR		__ADEC_DBG_ASSERT_STR
#else
#define ADEC_DBG_ASSERT(...)
#define ADEC_DBG_ASSERT_STR(...)
#endif

#if (ADEC_OPT_ASSERT_ENABLE == 1)
#define ADEC_ASSERT			__ADEC_ASSERT
#define ADEC_ASSERT_STR		__ADEC_ASSERT_STR
#else
#define ADEC_ASSERT(...)
#define ADEC_ASSERT_STR(...)
#endif

#if (ADEC_OPT_DBG_WARNING_ENABLE == 1)
#define ADEC_DBG_WARN		__ADEC_DBG_WARN
#else
#define ADEC_DBG_WARN(...)
#endif



#endif //__ADEC_DEBUG_H__
/** @} */
