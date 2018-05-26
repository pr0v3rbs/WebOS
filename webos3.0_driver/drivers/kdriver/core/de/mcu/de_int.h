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

#ifndef _DE_INT_h
#define _DE_INT_h

int DE_INT_Init(void);
int DE_INT_Cleanup(void);

int DE_VSYNC_Init(void);
int DE_VSYNC_Free(void);
int DE_VSYNC_WaitTimeOut(int timeout);
void DE_VSYNC_Register(char *str, int (*func)(void));

#endif
