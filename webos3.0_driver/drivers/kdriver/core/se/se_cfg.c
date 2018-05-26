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

/** @file
 *
 *  se memcfg data. this code is included by base_mem_cfg.c.
 *
 *	NOTE :	TZFW uses fixed address, and SE also uses fixed address.
 *			size of TZFW is 1MBytes, and other 7MBytes are marked by TZ-ASC for Secure Memory for SE.
 */
#include <linux/kernel.h>
#include <linux/module.h>
#include "base_types.h"
#include "os_util.h"

LX_MEMCFG_T gMemCfgSE[] =
{
	//H13B0, M14
	{
		.name = "se_mem",
		.base = 0x7f600000,
		.size = 0x00200000,
	},

	//H14
	{
		.name = "se_mem",
		.base = 0x00600000,
		.size = 0x00200000,
	},
	//H15
	{
		.name = "se_mem",
		.base = 0x7f600000,
		.size = 0x00200000,
	},

};
EXPORT_SYMBOL(gMemCfgSE);

LX_MEMCFG_T gMemCfgSE_TZFW[] =
{
	//H13B0, M14
	{
		.name = "se_tzfw",
		.base = 0x7f800000,
		.size = 0x00800000,
	},

	//H14
	{
		.name = "se_tzfw",
		.base = 0x00800000,
		.size = 0x00800000,
	},
	//H15
	{
		.name = "se_tzfw",
		.base = 0x7f800000,
		.size = 0x00800000,
	},
};
EXPORT_SYMBOL(gMemCfgSE_TZFW);

LX_MEMCFG_T *gpMemCfgSE;
LX_MEMCFG_T *gpMemCfgSE_TZFW;

void SE_InitCfg(void)
{
	if(lx_chip_rev() >= LX_CHIP_REV(H15,A0))
	{
		gpMemCfgSE = &gMemCfgSE[2];
		gpMemCfgSE_TZFW = &gMemCfgSE_TZFW[2];
	}
	else if(lx_chip_rev() >= LX_CHIP_REV(H14,A0))
	{
		gpMemCfgSE = &gMemCfgSE[1];
		gpMemCfgSE_TZFW = &gMemCfgSE_TZFW[1];
	}
	else
	{
		gpMemCfgSE = &gMemCfgSE[0];
		gpMemCfgSE_TZFW = &gMemCfgSE_TZFW[0];
	}
}

