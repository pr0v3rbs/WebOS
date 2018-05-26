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


/*****************************************************************************
**
**  Name: ademod_common_Paramtable.h
**
**  Description:    Parameter table template.
**
**  Dependencies:   TBD.
**
**  Revision History:
**
**     Date        Author          Description
**  -------------------------------------------------------------------------
**   31-07-2013   Jeongpil Yun    Initial draft.
**
*****************************************************************************/

#ifndef _ADEMOD_COMMON_PARAMTABLE_H_
#define _ADEMOD_COMMON_PARAMTABLE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ademod_common.h"
#include "ademod_common_demod.h"

#define END_OF_PARAM_LIST { 0, "", 0, 0, 0, ""}

// Register flags:
// bit 0 - Read Only,
// bit 1 - Write Only
#define PARM_RW		(0x0)
#define PARM_RO		(0x1)
#define PARM_WO		(0x2)

typedef struct LX_ADEMOD_tag_PARMENTRY
{
    UINT32 id;
    char   *desc;
    int    flags;
	int    size;
	int    format;
    char   *attr;
} LX_ADEMOD_PARMENTRY;

#ifdef __cplusplus
}
#endif

#endif /* _ADEMOD_COMMON_PARAMTABLE_H_ */
