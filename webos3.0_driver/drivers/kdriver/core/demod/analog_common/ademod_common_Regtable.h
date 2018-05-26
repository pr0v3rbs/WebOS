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
**  Name: ademod_common_Regtable.h
**
**  Description:     Register table options.
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


#ifndef _ADEMOD_COMMON_REGTABLE_H_
#define _ADEMOD_COMMON_REGTABLE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ademod_common.h"
#include "ademod_common_demod.h"

#define END_OF_REG_LIST { 0, NULL, 0 } //{ 0, "", 0, 0}

// Register flags:
// bit 0 - Read Only,
// bit 1 - Not relevant in Digital mode,
// bit 2 - Not relevant in Analog mode,
#define REG_RW		(0x0)
#define REG_RO		(0x1)
#define REG_ND		(0x2)
#define REG_NA		(0x4)
// and combinations
#define REG_NR		(0x6)
#define REG_RW_ND	(0x2)
#define REG_RO_ND	(0x3)
#define REG_RW_NA	(0x4)
#define REG_RO_NA	(0x5)
#define REG_RW_NR	(0x6)
#define REG_RO_NR	(0x7)

typedef struct LX_ADEMOD_tag_REGENTRY
{
    UINT32 addr;
    char   *desc;
    int    flags;
	int    size;
} LX_ADEMOD_REGENTRY;

#ifdef __cplusplus
}
#endif

#endif /* _ADEMOD_COMMON_REGTABLE_H_ */
