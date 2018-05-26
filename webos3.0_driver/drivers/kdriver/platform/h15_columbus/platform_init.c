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
 *	main configuration for lg1150 driver
 *
 *  author		daeyoung lim ( raxis.lim@lge.com )
 *  version		1.0
 *  date		2009.11.04
 *
 *  @addtogroup lg1150_base
 *	@{
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/
#ifndef	FPGA_DDR_MEM_SIZE
#define FPGA_DDR_MEM_SIZE	0xc0000000		/* 3GB */
#endif

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/ctype.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/delay.h>

#include "base_types.h"
#include "base_device.h"
#include "debug_util.h"
#include "os_util.h"

#include "hma_alloc.h"

#ifdef INCLUDE_KDRV_AUDIO
#include "audio_cfg.h"
#endif
#ifdef INCLUDE_KDRV_DE
#include "de_cfg.h"
#endif
#ifdef INCLUDE_KDRV_FBDEV
#include "fbdev_cfg.h"
#endif
#ifdef INCLUDE_KDRV_GFX
#include "gfx_cfg.h"
#endif
#ifdef INCLUDE_KDRV_PVR
#include "pvr_cfg.h"
#endif
#ifdef INCLUDE_KDRV_VBI
#include "vbi_cfg.h"
#endif
#ifdef INCLUDE_KDRV_OVI
#include "ovi_cfg.h"
#endif
#ifdef INCLUDE_KDRV_BE
#include "be_cfg.h"
#endif
#ifdef INCLUDE_KDRV_SDEC
#include "sdec_cfg.h"
#endif
#ifdef INCLUDE_KDRV_SE
#include "se_cfg.h"
#endif
#ifdef INCLUDE_KDRV_VENC
#include "venc_cfg.h"
#endif
#ifdef INCLUDE_KDRV_PNG
#include "png_cfg.h"
#endif
#ifdef INCLUDE_KDRV_AFE
#include "afe_cfg.h"
#endif
#ifdef INCLUDE_KDRV_MJPEG
#include "mjpeg_cfg.h"
#endif
#ifdef INCLUDE_KDRV_DE
#include "de_cfg.h"
#endif

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
#define	MAX_DDR_NUM		4

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define LX_MEMCFG_ENTRY(_m_entry)	{ .name= _STR(_m_entry), .pM=(LX_MEMCFG_T*)&(_m_entry), .nM=sizeof(_m_entry)/sizeof(LX_MEMCFG_T) }
#define LX_MEMCFG_ENTRY_END			{ .name="Total", .pM=NULL, .nM=0}

#define	__L_BUS__	0
#define	__G_BUS__	1
#define	__E_BUS__	2

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/
extern 	char*	simple_strtok(char *s, const char *delim, char **save_ptr);
extern	int     BASE_DEVMEM_InitDevMem( LX_MEMCFG_ENTRY_T* pEntry );

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define __MB(v)		((v)<<20)
#define __KB(v)		((v)<<10)

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
static LX_MEMCFG_T	g_memory_desc_table[MAX_DDR_NUM]= { { .name = "DDR0", .base = 0x0,	.size = 0 },
														{ .name = "DDR1a",.base = 0x0,	.size = 0 },
														{ .name = "DDR1b",.base = 0x0,	.size = 0 },
														{ .name = "DDR2", .base = 0x0,	.size = 0 } };

static LX_MEMCFG_T	sMemCfgHdr[MAX_DDR_NUM]={{.name = "[header] DDR0", .base = 0x0, .size = 0 /* do not allocate */ },
											{ .name = "[header] DDR1a",.base = 0x0, .size = 0 /* do not allocate */ },
											{ .name = "[header] DDR1b",.base = 0x0, .size = 0 /* do not allocate */ },
											{ .name = "[header] DDR2", .base = 0x0, .size = 0 /* do not allocate */ } };

static LX_MEMCFG_T	gMemCfgSYS				={ .name = "SYS", .base = 0x0, 	.size = 0x100000 };
static LX_MEMCFG_T	gMemCfgVIP				={ .name = "VIP", .base = 0x0, 	.size = __MB(64) };
static LX_MEMCFG_T	vdec_hma 				={ .name = "vdec_hma", .base = 0x0,	.size = 0 };
static LX_MEMCFG_T	vdec_uhd_dpb 			={ .name = "vdec_uhd_dpb", .base = 0x0,	.size = 0 };
static LX_MEMCFG_T	__L_bus_static_mem_start={ .name = "static_mem",  .base = 0x0,	.size = 0 }; /* mem separator */

/*---------------------------------------------------------------------------------------
 * Global Memory Configuration data base (H13/GP)
 *--------------------------------------------------------------------------------------*/
static LX_MEMCFG_ENTRY_T g_mem_desc_table_DDR_3GB[] =
{
	//-----------------------------------------------------------------------------------
	//	DDR #0
	//
	//
	//	464MB - GFX,OSD,AUDIO,TE,PVR,ICOD,VENC
	//	 32MB - VIP
	//	 16MB - static mem
	//-----------------------------------------------------------------------------------
	LX_MEMCFG_ENTRY(sMemCfgHdr[0]),

#ifdef INCLUDE_KDRV_VENC
	LX_MEMCFG_ENTRY(gMemCfgVenc),				/* VENC */
#endif

#ifdef INCLUDE_KDRV_AUDIO
	LX_MEMCFG_ENTRY(g_stMemCfgAud[0]),			/* AUDIO */
#endif

#ifdef INCLUDE_KDRV_SDEC
	LX_MEMCFG_ENTRY(gMemCfgSDEC),				/* SDEC/TE */
#endif
#ifdef INCLUDE_KDRV_PVR
	LX_MEMCFG_ENTRY(gMemCfgPvr),				/* SDEC/PVR */
#endif

#ifdef INCLUDE_KDRV_PNG
	LX_MEMCFG_ENTRY(gMemCfgPng[1]),				/* PNG */
#endif
#ifdef INCLUDE_KDRV_MJPEG
	LX_MEMCFG_ENTRY(gMemCfgMjpeg),				/* MJPEG */
#endif

#ifdef INCLUDE_KDRV_GFX
	LX_MEMCFG_ENTRY(gMemCfgGfx.surface[0]),		/* GFX */
#endif

#ifdef INCLUDE_KDRV_FBDEV
	LX_MEMCFG_ENTRY(gMemCfgFBDev[0]),			/* FBDEV OSD0 */
	LX_MEMCFG_ENTRY(gMemCfgFBDev[1]),			/* FBDEV OSD1 */
	LX_MEMCFG_ENTRY(gMemCfgFBDev[2]),			/* FBDEV OSD2 */
	LX_MEMCFG_ENTRY(gMemCfgFBDev[3]),			/* FBDEV OSD3 */
	LX_MEMCFG_ENTRY(gMemCfgFBHdr[0]),			/* FBDEV OSD0 (hdr) */
	LX_MEMCFG_ENTRY(gMemCfgFBHdr[1]),			/* FBDEV OSD1 (hdr) */
	LX_MEMCFG_ENTRY(gMemCfgFBHdr[2]),			/* FBDEV OSD2 (hdr) */
	LX_MEMCFG_ENTRY(gMemCfgFBHdr[3]),			/* FBDEV OSD2 (hdr) */
#endif

	LX_MEMCFG_ENTRY(gMemCfgVIP),				/* VIP */

	LX_MEMCFG_ENTRY(__L_bus_static_mem_start),
	LX_MEMCFG_ENTRY(gMemCfgSYS),
#ifdef INCLUDE_KDRV_SE
	LX_MEMCFG_ENTRY(gMemCfgSE[2]),				/* SE */
	LX_MEMCFG_ENTRY(gMemCfgSE_TZFW[2]),			/* SE/TZ */
#endif
	//-----------------------------------------------------------------------------------
	//	DDR #1a
	//-----------------------------------------------------------------------------------
	LX_MEMCFG_ENTRY(sMemCfgHdr[1]),
	LX_MEMCFG_ENTRY(vdec_hma),

	//-----------------------------------------------------------------------------------
	//	DDR #1b
	//-----------------------------------------------------------------------------------
	LX_MEMCFG_ENTRY(sMemCfgHdr[2]),
	LX_MEMCFG_ENTRY(vdec_uhd_dpb),

	//-----------------------------------------------------------------------------------
	//	DDR #2
	//-----------------------------------------------------------------------------------
	LX_MEMCFG_ENTRY(sMemCfgHdr[3]),
#ifdef INCLUDE_KDRV_DE
    LX_MEMCFG_ENTRY(gMemCfgDe[4]),				/* DE */
#endif
#ifdef INCLUDE_KDRV_VBI
	LX_MEMCFG_ENTRY(gstBufMemCfgVBIRev[1]),		/* VBI Buffer -- FIXME DDR1 ??? */
#endif

#ifdef INCLUDE_KDRV_AFE
	LX_MEMCFG_ENTRY(gCvdMemCfg),				/* CVD (3DComb H/W Memory) -- FIXME DDR1 ??? */
#endif

#ifdef INCLUDE_KDRV_DE
	LX_MEMCFG_ENTRY(gMemCfgDeFW[4]),			/* DE F/W - temporary */
#endif

#ifdef INCLUDE_KDRV_BE
	LX_MEMCFG_ENTRY(gMemCfgBe[3]),				/* BE */
#endif

	LX_MEMCFG_ENTRY_END
};

/*========================================================================================
	Implementation Group
========================================================================================*/
static void	BASE_PLT_3GB_Init   ( void )
{
	int	i;
	int	ret;

	/* determine memory layout */
	printk("H15 3G DDR - ADDR SWITCH <ON> !!!\n");

	g_memory_desc_table[0].base = 0x4A000000;	/* L Bus Devices */
	g_memory_desc_table[0].size = __MB(512);

	g_memory_desc_table[1].base = 0x6A000000;	/* VDEC */
	g_memory_desc_table[1].size = __MB(272);

	g_memory_desc_table[2].base = 0x7B000000;	/* VDEC UHD DPB */
	g_memory_desc_table[2].size = __MB(560);

	g_memory_desc_table[3].base = 0x9E000000;	/* DE/BE */
	g_memory_desc_table[3].size = __MB(544);

	vdec_hma.base = g_memory_desc_table[1].base;
	vdec_hma.size = g_memory_desc_table[1].size;

	vdec_uhd_dpb.base = g_memory_desc_table[2].base;
	vdec_uhd_dpb.size = g_memory_desc_table[2].size;

	/* static mem description - 16MB block in L_BUS */
	__L_bus_static_mem_start.base = (g_memory_desc_table[0].base + g_memory_desc_table[0].size - __MB(16));

	/* VIP */
	gMemCfgVIP.base = __L_bus_static_mem_start.base - gMemCfgVIP.size;

	/* make sMemCfgHdr */
	for (i=0; i<MAX_DDR_NUM; i++ )
	{
		sMemCfgHdr[i].base	= g_memory_desc_table[i].base;
		sMemCfgHdr[i].size	= g_memory_desc_table[i].size;
	}

	ret = BASE_DEVMEM_InitDevMem( g_mem_desc_table_DDR_3GB );
	if ( ret != RET_OK ) DBG_PRINT_ERROR("fatal error during making device memory layout\n");

	/* register vdec_hma memory pool */
	ret = hma_pool_register ("ddr0", vdec_hma.base, vdec_hma.size);
	if (ret < 0)
		printk (KERN_ERR "hma_pool_register(vdec_hma) failed\n");

	ret = hma_pool_register ("ddr1", vdec_uhd_dpb.base, vdec_uhd_dpb.size);
	if (ret < 0)
		printk (KERN_ERR "hma_pool_register(vdec_uhd_dpb) failed\n");

}

void	BASE_PLT_Init (void)
{
	BASE_PLT_3GB_Init( );
}

UINT32  BASE_PLT_Probe (void)
{
#if 	CONFIG_LX_BOARD_FHD==1 && CONFIG_LX_BOARD_UD==0
	return LX_CHIP_PLT_UHD;
#elif 	CONFIG_LX_BOARD_FHD==0 && CONFIG_LX_BOARD_UD==1
	return LX_CHIP_PLT_FHD;
#elif 	CONFIG_LX_BOARD_FHD==1 && CONFIG_LX_BOARD_UD==1
	return LX_CHIP_PLT_UHD;
#else
	#error unknown board !!!
#endif
}

/** @} */

