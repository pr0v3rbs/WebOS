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
 *	@file adec_mem_if.h
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
#ifndef __ADEC_MEM_IF_H__
#define __ADEC_MEM_IF_H__

typedef void* AdecMemStat;

typedef void (*MemNodePrint)(int _size, char *_file, int _line);

void *AdecMem_Alloc(AdecMemStat _stat, int _size, char *_file, int _line);
void AdecMem_Free(AdecMemStat _stat, void* _ptr);
void *AdecMem_Set(AdecMemStat _stat, void* _ptr, int value, unsigned int _size, char *_file, int _line);
void *AdecMem_Copy(AdecMemStat _stat, void* _dst, void* _src, unsigned int _size, char *_file, int _line);
void *AdecMem_Move(AdecMemStat _stat, void *_dst, void *_src, unsigned int _size, char *_file, int _line);

unsigned long long AdecMem_GetAllocationSize(void);
unsigned long long AdecMem_GetAllocationCnt(void);
void AdecMem_Print(MemNodePrint _print_func);
int AdecMem_Validate(void);

AdecMemStat AdecMem_Create(void);
void AdecMem_Terminate(void *_stat);
void AdecMem_AddRegion(AdecMemStat _stat, void *_ptr, int _size);
void AdecMem_SetDefaultStat(AdecMemStat _stat);
AdecMemStat AdecMem_GetDefaultStat(void);

#if defined(_USE_MEM_STAT) && (_USE_MEM_STAT == 1)
#define ADEC_MALLOC(__MEM, __SIZE, __TYPE)		((__TYPE*)AdecMem_Alloc(__MEM, __SIZE, __FILE__, __LINE__))
#define ADEC_FREE(__MEM, __PTR)					(AdecMem_Free(__MEM, __PTR))
#define ADEC_MEMSET(__MEM, __DST, __VAL, __LEN)	(AdecMem_Set(__MEM, __DST, __VAL, __LEN, __FILE__, __LINE__))
#define ADEC_MEMCPY(__MEM, __DST, __SRC, __LEN)	(AdecMem_Copy(__MEM, __DST, __SRC, __LEN, __FILE__, __LINE__))
#define ADEC_MEMMOVE(__MEM, __DST, __SRC, __LEN) (AdecMem_Move(__MEM, __DST, __SRC, __LEN, __FILE__, __LINE__))
#else
#define ADEC_MALLOC(__MEM, __SIZE, __TYPE)		((__TYPE*)__STD_MALLOC(__SIZE))
#define ADEC_FREE(__MEM, __PTR)					(__STD_FREE(__PTR))
#define ADEC_MEMSET(__MEM, __DST, __VAL, __LEN)	(__STD_MEMSET(__DST,__VAL,__LEN))
#define ADEC_MEMCPY(__MEM, __DST, __SRC, __LEN)	(__STD_MEMCPY(__DST,__SRC,__LEN))
#define ADEC_MEMMOVE(__MEM, __DST, __SRC, __LEN) (__STD_MEMMOVE(__DST, __SRC, __LEN))
#endif

#endif


/** @} */
