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
 *	@file adec_platform_win.c
 *
 *
 *	@author		Jung, Kyung Soon (kyungsoon.jung@leg.com)
 *	@version	0.0.1
 *	@date		2012-03-23
 *	@note
 *	@see		http://www.lge.com
 *	@addtogroup ADEC
 *	@{
 */

#ifdef _WIN32

#include "platform/adec_platform_win.h"
#include <time.h>


long long _ADEC_WIN_GetCurrentClock()
{
	clock_t currentClock;

	currentClock = clock();

	return currentClock / (CLOCKS_PER_SEC / 1000);
}

#endif

/** @} */
