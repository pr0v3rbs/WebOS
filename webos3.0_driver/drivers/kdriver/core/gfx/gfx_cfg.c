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
 *  main configuration file for gfx device
 *	gfx device will teach you how to make device driver with new platform.
 *
 *  author		raxis.lim (raxis.lim@lge.com).lim (raxis.lim@lge.com)
 *  version		1.0
 *  date		2009.12.30
 *  note		Additional information.
 *
 *  @addtogroup lg1150_gfx
 *	@{
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include "gfx_impl.h"
#include "base_device.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
#ifndef GFX_DEV_MEM_SIZE_UHD	/* GFX_DEV_MEM_SIZE_UHD is defined only in H13 platform_mem_map.h */
#define GFX_DEV_MEM_SIZE_UHD	0
#endif

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/
/** GFX memory description table
 *
 *	if base value is zero, base address is dynamicalled assigned by devmem manager ( dynamic address )
 *	if base value is non zero, devmem manager doesn't modify its address ( static address )
 *
 *	@see  GFX_InitCfg
 *	@note memory desc. is very platform dependent.
 */
LX_MEMCFG_T gfx_mem_desc_table[] =
{
	[0] = { .name = "gfx_surf", .base = GFX_DEV_MEM_BASE, 	.size = GFX_DEV_MEM_SIZE		},	/* FHD or general  */
	[1] = { .name = "gfx_surf", .base = 0, 					.size = 0						},	/* FHD or general  */

	[2] = { .name = "gfx_surf", .base = GFX_DEV_MEM_BASE,	.size = GFX_DEV_MEM_SIZE_UHD 	},	/* H13: UHD_4K specific */
	[3] = { .name = "gfx_surf", .base = GFX_DEV_MEM_BASE,	.size = GFX_DEV_MEM_SIZE_UHD 	},	/* H13: UHD_4K specific */
};

/** GFX H/W configuratin table
 *
 */
LX_GFX_REG_CFG_T	gfx_reg_cfg_desc_table[] =
{
	/* L9   */[0] = { .chip = LX_CHIP_L9,  .chip_name = "GFX-L9",   .reg_base_addr = 0xc001d000, .reg_size = 0x200, .irq_num = L9_IRQ_GFX },
	/* H13  */[1] = { .chip = LX_CHIP_H13, .chip_name = "GFX-H13",  .reg_base_addr = 0xc000b000, .reg_size = 0x200, .irq_num = (32+54) },
	/* M14Ax*/[2] = { .chip = LX_CHIP_M14, .chip_name = "GFX-M14A", .reg_base_addr = 0xc000b000, .reg_size = 0x200, .irq_num = (32+54) },
	/* M14Bx*/[3] = { .chip = LX_CHIP_M14, .chip_name = "GFX-M14B", .reg_base_addr = 0xc000b000, .reg_size = 0x200, .irq_num = (32+56) },
    /* H14Ax*/[4] = { .chip = LX_CHIP_H14, .chip_name = "GFX-H14",  .reg_base_addr = 0xc000b000, .reg_size = 0x200, .irq_num = (32+54) },
	/* H15Ax*/[5] = { .chip = LX_CHIP_H15, .chip_name = "GFX-H15",	.reg_base_addr = 0xc000b000 /*NA*/, .reg_size = 0x200, .irq_num = (UINT32)-1 },
};

LX_GFX_REG_CFG_T*	gpGfxRegCfg = NULL;
LX_GFX_MEM_CFG_T	gMemCfgGfx;

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/

/*========================================================================================
	Implementation Group
========================================================================================*/
void    GFX_InitCfg ( void )
{
	memset( &gMemCfgGfx, 0x0, sizeof(LX_GFX_MEM_CFG_T)); 			// clear memcfg
	memcpy( &g_gfx_cfg, g_gfx_hal.GetCfg(), sizeof(LX_GFX_CFG_T)); 	// get chip cfg

	/*-----------------------------------------------------------------------------------
	 * [H15] configuration
	 *----------------------------------------------------------------------------------*/
    if( !LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H15) )
	{
		gpGfxRegCfg = &gfx_reg_cfg_desc_table[5];
		memcpy( &gMemCfgGfx.surface[0], &gfx_mem_desc_table[0], sizeof(LX_MEMCFG_T));
	}
	/*-----------------------------------------------------------------------------------
	 * [H14] configuration
	 *----------------------------------------------------------------------------------*/
    else if( !LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H14) )
	{
		gpGfxRegCfg = &gfx_reg_cfg_desc_table[4];

		memcpy( &gMemCfgGfx.surface[0], &gfx_mem_desc_table[0], sizeof(LX_MEMCFG_T));
	}
	/*-----------------------------------------------------------------------------------
	 * [M14] configuration
	 *----------------------------------------------------------------------------------*/
    else if( !LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_M14) )
	{
		switch(lx_chip_rev())
		{
			case LX_CHIP_REV(M14,A0):
			case LX_CHIP_REV(M14,A1):	gpGfxRegCfg = &gfx_reg_cfg_desc_table[2]; break;
			case LX_CHIP_REV(M14,B0):
			default: 					gpGfxRegCfg = &gfx_reg_cfg_desc_table[3]; break;
		}

		memcpy( &gMemCfgGfx.surface[0], &gfx_mem_desc_table[0], sizeof(LX_MEMCFG_T));
	}
	/*-----------------------------------------------------------------------------------
	 * [H13] configuration
	 *----------------------------------------------------------------------------------*/
    else if( !LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H13) )
	{
		int	i;
		int mem_cfg_idx;
		int	num_max_rgn;	/* UHD platform require more GFX memory. we should make 2nd surface region :( */

		gpGfxRegCfg = &gfx_reg_cfg_desc_table[1];

		switch(lx_chip_plt())
		{
//			case LX_CHIP_PLT_FHD_2K:
			case LX_CHIP_PLT_UHD:	num_max_rgn=2; mem_cfg_idx=2; break; // UHD
			default:				num_max_rgn=1; mem_cfg_idx=0; break; // FHD
		}

		for (i=0; i<num_max_rgn; i++ )
		{
			memcpy( &gMemCfgGfx.surface[i], &gfx_mem_desc_table[mem_cfg_idx], sizeof(LX_MEMCFG_T));
		}
	}
	/*-----------------------------------------------------------------------------------
	 * unknown/not supported chip
	 *----------------------------------------------------------------------------------*/
    else
    {
		GFX_ERROR("unknown/not supported chip 0x%x\n", lx_chip_rev() );
    }
}

/** @} */

