/*
 * SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 * Copyright(c) 2013 by LG Electronics Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */


/** @file
 *
 *  sdec driver
 *
 *  @author	Jihoon Lee ( gaius.lee@lge.com)
 *  @author	Jinhwan Bae ( jinhwan.bae@lge.com) - modifier
 *  @version	1.0
 *  @date		2010-03-30
 *  @note		Additional information.
 */


#ifndef _SDEC_MM_H
#define _SDEC_MM_H

#ifdef __cplusplus
extern "C" {
#endif

/*----------------------------------------------------------------------------------------
 *   Control Constants
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Constant Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Macro Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Type Definitions
 *---------------------------------------------------------------------------------------*/
typedef enum
{
	SDEC_MM_TYPE_NORMAL	= 0,
	SDEC_MM_TYPE_CIPLUS,
	SDEC_MM_TYPE_MAX
} SDEC_MM_TYPE_T;

typedef struct SDEC_MM_CTX SDEC_MM_CTX_T;


/*----------------------------------------------------------------------------------------
 *   External Function Prototype Declarations
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
   API
----------------------------------------------------------------------------------------*/
int		SDEC_MM_Init(void);
int		SDEC_MM_Final(void);

SDEC_MM_CTX_T* SDEC_MM_Open(void);
void	SDEC_MM_Close(SDEC_MM_CTX_T* h);

void*	SDEC_MM_Alloc(SDEC_MM_CTX_T* h, SDEC_MM_TYPE_T type, UINT32 size);
int		SDEC_MM_Free(SDEC_MM_CTX_T* h, SDEC_MM_TYPE_T type, void* ptr);

int		SDEC_MM_GetMemInfo(SDEC_MM_TYPE_T type, LX_SDEC_MEM_INFO_T *info);
int		SDEC_MM_GetMemStat(SDEC_MM_TYPE_T type, LX_SDEC_MM_STAT_T *stat);

/*----------------------------------------------------------------------------------------
 *   External Variables
 *---------------------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* _SDEC_MM_H */

