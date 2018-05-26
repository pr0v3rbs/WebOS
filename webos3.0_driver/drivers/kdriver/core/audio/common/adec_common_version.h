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

#ifndef __ADEC_COMMON_VERSION_H__
#define __ADEC_COMMON_VERSION_H__

#include "adec_version.h"

#define ADEC_COMMON_VER_MAJOR		0x1
#define ADEC_COMMON_VER_MINOR		0x0
#define ADEC_COMMON_VER_FIX			0x30
#define ADEC_COMMON_VER_FLAG		(' ')	// ('T') for Test
											// ('D') for Debug
											// ('G') for google only (not recommended)
											// ('N') for NC (not recommended)
											// etc...
#define ADEC_COMMON_VER_EXT			0x0		// Extended identifier for special usage

#define ADEC_COMMON_VER			_ADEC_VER_INT(ADEC_COMMON_VER_MAJOR,\
                       			              ADEC_COMMON_VER_MINOR,\
                       			              ADEC_COMMON_VER_FIX,\
                       			              ADEC_COMMON_VER_FLAG,\
                       			              ADEC_COMMON_VER_EXT)

#define ADEC_COMMON_VER_STR		_ADEC_VER_STR(ADEC_COMMON_VER_MAJOR,\
                       			              ADEC_COMMON_VER_MINOR,\
                       			              ADEC_COMMON_VER_FIX,\
                       			              ADEC_COMMON_VER_FLAG,\
                       			              ADEC_COMMON_VER_EXT)

#endif
