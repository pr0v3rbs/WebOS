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

#ifndef _ADEC_AU_INFO_H_
#define _ADEC_AU_INFO_H_

typedef struct _ADEC_AU_INFO_T
{
	long long size;
	unsigned int timestamp;
	unsigned int gstc;
	short index;
	char error;
	char discontinuity;
	unsigned int lastAu;
	unsigned int first_module_gstc;
}ADEC_AU_INFO_T;


#endif
