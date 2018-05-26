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
 *	@file adec_error.c
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

#include "adec_common.h"


static const char* gAdecErrString[ADEC_ERR_MAX] =
{
	"No Error",
#ifdef ADEC_ERR_DEF
#undef ADEC_ERR_DEF
#endif
#define ADEC_ERR_DEF(_TYPE_NAME,_ERR_STR) _ERR_STR,
#include "adec_error_def.h"
#undef ADEC_ERR_DEF
};


void ADEC_PrintError(ADEC_RESULT _errorType)
{
	if(_errorType < 0 || _errorType >= ADEC_ERR_MAX)
	{
		return;
	}

	ADEC_DBG_ERR(ADEC_DBG_INDEX_ASSERT, "%s\n",gAdecErrString[_errorType]);
}

const char* ADEC_GetErrorString(ADEC_RESULT _errorType)
{
	if (_errorType >= 0 && _errorType < ADEC_ERR_MAX)
	{
		return gAdecErrString[_errorType];
	}
	return "Unknown Error Code";
}

/** @} */


