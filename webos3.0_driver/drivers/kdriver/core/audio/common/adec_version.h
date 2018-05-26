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

#ifndef __ADEC_VERSION_H__
#define __ADEC_VERSION_H__

#define _ADEC_VER_STR(_MAJOR, _MINOR, _FIX, _FLAG, _EXT)	(#_MAJOR"."#_MINOR"."#_FIX"("#_FLAG #_EXT")")
#define _ADEC_VER_INT(_MAJOR, _MINOR, _FIX, _FLAG, _EXT)	(((_MAJOR & 0x0f) << 28) | \
															 ((_MINOR & 0xff) << 20) | \
															 ((_FIX & 0xff) << 12 ) | \
															 ((_FLAG & 0xff) << 4) |\
															 ((_EXT & 0xf)))

#endif
