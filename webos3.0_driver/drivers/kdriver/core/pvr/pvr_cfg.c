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
 *  main configuration file for pvr device
 *	pvr device will teach you how to make device driver with new platform.
 *
 *  author		kyungbin.pak
 *  author		modified by ki beom kim (kibeom.kim@lge.com)
 *  version		1.0
 *  date		2010.02.05
 *  note		Additional information.
 *
 *  @addtogroup lg1150_pvr
 *	@{
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include <linux/sched.h>
#include <linux/semaphore.h>
#include <asm/io.h>			/**< For ioremap_nocache */
#include "os_util.h"

#include "pvr_drv.h"
#include "pvr_cfg.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/
typedef struct
{
	UINT32	base;		///< physical base address in BYTE!!! of media ip
	UINT32	size;		///< size in BYTE!!!
} LX_PVR_MEMCFG_T[LX_PVR_MEM_NUM];


/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/

/* Just define memory block name in here. Size will be defined in PVR_CFG_PreInit */
LX_MEMCFG_T	gMemCfgPvr[PVR_MAX_DEVICE][LX_PVR_MEM_NUM] =
{
	{
		{ .name = "pvr0/dn" },
		{ .name = "pvr0/up" },
	},
	{
		{ .name = "pvr1/dn" },
		{ .name = "pvr1/up" },
    }
};


/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/

static LX_PVR_CFG_T _pvrHwCfg[] =
{
	[0] = /* H13, M14Ax */
	{
		.irqNum		= H13_IRQ_DVR,
		.regBase	= H13_DVR_REG_BASE,
		.num_dndev	= 2,
		.num_updev	= 2,
	},
	[1] = /* M14Bx */
	{
		.irqNum 	= M14_B0_IRQ_DVR,
		.regBase	= M14_B0_DVR_REG_BASE,
		.num_dndev	= 2,
		.num_updev	= 2,
		.mem_shared	= 1,
	},
	[2] = /* H14 */
	{
		.irqNum 	= H14_IRQ_DVR,
		.regBase	= H14_DVR_REG_BASE,
		.num_dndev	= 2,
		.num_updev	= 2,
	},
	[3] = /* H15 */
	{
		.irqNum 	= H15_IRQ_TE1,
		.regBase	= H15_DVR_REG_BASE,
		.num_dndev	= 2,
		.num_updev	= 2,
		.mem_shared	= 1,
		.fixup_timestamp = 1,
	},
};


static LX_PVR_MEMCFG_T _pvrMemCfg[][PVR_MAX_DEVICE] =
{
	[0] = // for M14Bx
	{
		[0] =	// PVR0
		{
			[LX_PVR_MEM_DN]		= {.base = 0x0, .size = 0x006C0000},	/* dn buffer should be multuple of 384KB */
			[LX_PVR_MEM_UP]		= {.base = 0x0, .size = 0x00240000},	/* up buffer should be multuple of 384KB */
		},
		[1] =	/* for memory reservation */
		{
			[LX_PVR_MEM_DN] 	= {.base = 0x0, .size = 0x00060000},
		}
	},
	[1] = // for H13,M14Ax,H14
	{
		[0] =	// PVR0
		{
			[LX_PVR_MEM_DN]		= {.base = 0x0, .size = 0x006C0000},	/* dn buffer should be multuple of 384KB */
			[LX_PVR_MEM_UP]		= {.base = 0x0, .size = 0x00240000},	/* up buffer should be multuple of 384KB */
		},
		[1] =	// PVR1
		{
			[LX_PVR_MEM_DN]		= {.base = 0x0, .size = 0x006C0000},	/* dn buffer should be multuple of 384KB */
			[LX_PVR_MEM_UP]		= {.base = 0x0, .size = 0x00240000},	/* up buffer should be multuple of 384KB */
		},
	},
	[2] =	// for H15
	{
		[0] =	// PVR0
		{
			[LX_PVR_MEM_DN]		= {.base = 0x0, .size = 0x01800000},	/* 24MB(384KB*64). dn buffer should be multuple of 384KB */
			[LX_PVR_MEM_UP]		= {.base = 0x0, .size = 0x00600000},	/*  6MB(384KB*16). up buffer should be multuple of 384KB */
		},
		[1] =	// for memory reservation
		{
			[LX_PVR_MEM_DN]		= {.base = 0x0, .size = 0x00000000},
			[LX_PVR_MEM_UP]		= {.base = 0x0, .size = 0x00200000},
		},
	},
};


static LX_PVR_CFG_T *pvr_cfg;


/*========================================================================================
	Implementation Group
========================================================================================*/

void PVR_CFG_PreInit(void)
{
	int i;
	LX_PVR_MEMCFG_T *memCfg;

	if(0){}
	else if	(!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H15))
	{
		pvr_cfg	= &_pvrHwCfg[3];
		memCfg	= _pvrMemCfg[2];
	}
	else if	(!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H14))
	{
		pvr_cfg	= &_pvrHwCfg[2];
		memCfg	= _pvrMemCfg[1];
	}
	else if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_M14))
	{
		if (lx_chip_rev() >= LX_CHIP_REV(M14, B0))
		{
			pvr_cfg	= &_pvrHwCfg[1];
			memCfg = _pvrMemCfg[0];

			if(lx_chip_rev() >= LX_CHIP_REV(M14, C0))
			{
				pvr_cfg->num_updev = 1;		// only one upload device
			}
		}
		else
		{
			pvr_cfg	= &_pvrHwCfg[0];
			memCfg = _pvrMemCfg[1];
		}
	}
	else if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H13) )
	{
		pvr_cfg	= &_pvrHwCfg[0];
		memCfg = _pvrMemCfg[1];

	}
	else
	{
		DBG_PRINT_ERROR("Not defined CHIP !!!");
		pvr_cfg = &_pvrHwCfg[2];
		memCfg	= _pvrMemCfg[1];
	}


	for(i=0; i<PVR_MAX_DEVICE; i++)
	{
		gMemCfgPvr[i][LX_PVR_MEM_DN].base		= memCfg[i][LX_PVR_MEM_DN].base;
		gMemCfgPvr[i][LX_PVR_MEM_DN].size		= memCfg[i][LX_PVR_MEM_DN].size;

		gMemCfgPvr[i][LX_PVR_MEM_UP].base		= memCfg[i][LX_PVR_MEM_UP].base;
		gMemCfgPvr[i][LX_PVR_MEM_UP].size		= memCfg[i][LX_PVR_MEM_UP].size;
	}
}

LX_PVR_CFG_T* PVR_CFG_GetConfig(void)
{
	return pvr_cfg;
}

LX_MEMCFG_T* PVR_CFG_GetMemConfig(LX_PVR_CH_T ch)
{
	if(ch >= LX_PVR_CH_NUM)
		return NULL;

	if(pvr_cfg->mem_shared)
		return (LX_MEMCFG_T*)&gMemCfgPvr[0];

	return (LX_MEMCFG_T*)&gMemCfgPvr[ch];
}

/** @} */

