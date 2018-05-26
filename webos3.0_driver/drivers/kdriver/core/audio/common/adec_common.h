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
 *	@file adec_common.h
 *
 *
 *	@author		Jung, Kyung Soon (kyungsoon.jung@leg.com)
 *	@version	0.0.1
 *	@date		2012-03-15
 *	@note
 *	@see		http://www.lge.com
 *	@addtogroup ADEC_COMMON
 *	@{
 */

/******************************************************************************
  Header File Guarder
 ******************************************************************************/

#ifndef __ADEC_COMMOON_H__
#define __ADEC_COMMOON_H__

#include "adec_config.h"
#include "adec_error.h"
#include "adec_mem_if.h"
#include "adec_debug.h"

#ifdef _WIN32
#include "platform/adec_platform_win.h"
#else
#ifdef _XTENSA
#include "platform/adec_platform_xt.h"
#else
#ifdef _ADEC_ARM
#include "platform/adec_platform_arm.h"
#endif
#endif
#endif

#ifndef NULL
#define NULL ((void*)0)
#endif

#endif

/** @} */
