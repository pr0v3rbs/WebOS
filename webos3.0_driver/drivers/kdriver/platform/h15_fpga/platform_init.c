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
#define	DDR_GFX_BUS_ID		__L_BUS__		/* GFX is located in GBus */
#define	DDR_OSD_BUS_ID		__L_BUS__		/* OSD is located in GBus */

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
#define	MAX_DDR_NUM		3

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define LX_MEMCFG_ENTRY(_m_entry)	{ .name= _STR(_m_entry), .pM=(LX_MEMCFG_T*)&(_m_entry), .nM=sizeof(_m_entry)/sizeof(LX_MEMCFG_T) }
#define LX_MEMCFG_ENTRY_END			{ .name="Total", .pM=NULL, .nM=0}

#define _MB_(_sz)	(( _sz)>>20)
#define _KB_(_sz)	(((_sz)>>10)&0x3ff)
#define _BB_(_sz)	(( _sz)     &0x3ff)


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

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
static LX_MEMCFG_T	g_memory_desc_table[MAX_DDR_NUM]	= { { .name = "DDR0", .base = 0x0,	.size = 0 },
															{ .name = "DDR1", .base = 0x0,	.size = 0 },
															{ .name = "DDR2", .base = 0x0,	.size = 0 } };

static LX_MEMCFG_T	sMemCfgHdr[MAX_DDR_NUM]={ 	{ .name = "[header] DDR0", .base = 0x0, .size = 0 /* do not allocate */ },
												{ .name = "[header] DDR1", .base = 0x0, .size = 0 /* do not allocate */ },
												{ .name = "[header] DDR2", .base = 0x0, .size = 0 /* do not allocate */ } };

static LX_MEMCFG_T	gMemCfgSYS			= 	{ .name = "SYS", .base = 0x0, 	.size = 0x100000 };

static LX_MEMCFG_T	vdec_hma 			= 	{ .name = "ddr0", .base = 0x0,	.size = 0 };

/*---------------------------------------------------------------------------------------
 * Global Memory Configuration data base (H13/GP)
 *--------------------------------------------------------------------------------------*/
static LX_MEMCFG_ENTRY_T	g_mem_desc_table_DDR_1GB[] =
{
	LX_MEMCFG_ENTRY(sMemCfgHdr[0]),				/* DDR0 START */

	LX_MEMCFG_ENTRY(gMemCfgSYS),

#ifdef INCLUDE_KDRV_SE
	LX_MEMCFG_ENTRY(gMemCfgSE[1]),				/* SE */
	LX_MEMCFG_ENTRY(gMemCfgSE_TZFW[1]),			/* SE/TZ */
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

#ifdef INCLUDE_KDRV_PNG
	LX_MEMCFG_ENTRY(gMemCfgPng[1]),				/* PNG */
#endif
#ifdef INCLUDE_KDRV_SDEC
	LX_MEMCFG_ENTRY(gMemCfgSDEC),			/* SDEC/TE */
#endif
#ifdef INCLUDE_KDRV_VENC
	LX_MEMCFG_ENTRY(gMemCfgVenc),			/* VENC */
#endif

#ifdef INCLUDE_KDRV_AFE
	LX_MEMCFG_ENTRY(gCvdMemCfg),				/* CVD (3DComb H/W Memory) */
#endif

#ifdef INCLUDE_KDRV_MJPEG
	LX_MEMCFG_ENTRY(gMemCfgMjpeg),				/* MJPEG */
#endif

#ifdef INCLUDE_KDRV_AUDIO
	LX_MEMCFG_ENTRY(g_stMemCfgAud[0]),			/* AUDIO */
#endif
#ifdef INCLUDE_KDRV_VBI
	LX_MEMCFG_ENTRY(gstBufMemCfgVBIRev[1]),		/* VBI Buffer */
#endif
#ifdef INCLUDE_KDRV_OVI
	LX_MEMCFG_ENTRY(gMemCfgOviTcon),			/* OVI TCON */
#endif

#ifdef INCLUDE_KDRV_PVR
	LX_MEMCFG_ENTRY(gMemCfgPvr),				/* SDEC/PVR */
#endif
	LX_MEMCFG_ENTRY(vdec_hma),

	LX_MEMCFG_ENTRY(sMemCfgHdr[1]), 			 /* DDR1 START */

#ifdef INCLUDE_KDRV_DE
	LX_MEMCFG_ENTRY(gMemCfgDe[3]),				/* DE */
#endif
#ifdef INCLUDE_KDRV_DE
	LX_MEMCFG_ENTRY(gMemCfgDeGrap[3]),			/* DE */
#endif
#ifdef INCLUDE_KDRV_DE
	LX_MEMCFG_ENTRY(gMemCfgDePreW[3]),			/* DE */
#endif
#ifdef INCLUDE_KDRV_DE
	LX_MEMCFG_ENTRY(gMemCfgDeFW[3]),			/* DE F/W - temporary */
#endif
#ifdef INCLUDE_KDRV_BE
	LX_MEMCFG_ENTRY(gMemCfgBe[2]),				/* BE */
#endif
	LX_MEMCFG_ENTRY_END
};

static LX_MEMCFG_T	__3GB_L_bus_gfx_start		= { .name = "gfx_l_bus_start",  .base = 0x0,	.size = 0 };	/* L Bus fixed */
static LX_MEMCFG_T	__3GB_L_bus_osd_start		= { .name = "osd_l_bus_start",  .base = 0x0,	.size = 0 };	/* L Bus fixed (test) */
static LX_MEMCFG_T	__3GB_L_bus_sys_start		= { .name = "sys_l_bus_start",  .base = 0x0,	.size = 0 };	/* L Bus fixed (test) */
static LX_MEMCFG_T	__3GB_L_bus_etc_start		= { .name = "etc_l_etc_start",  .base = 0x0,	.size = 0 };	/* L Bus fixed (test) */
static LX_MEMCFG_T	__3GB_L_bus_venc_start		= { .name = "venc_l_bus_start", .base = 0x0,	.size = 0 };	/* L Bus fixed */
static LX_MEMCFG_T	__3GB_L_bus_vip_start		= { .name = "vip_l_bus_start",	.base = 0x0,	.size = 0 };	/* L Bus fixed */
static LX_MEMCFG_T	__3GB_L_bus_aud_start		= { .name = "aud_l_bus_start", 	.base = 0x0,	.size = 0 };	/* L Bus fixed */
static LX_MEMCFG_T	__3GB_L_bus_te_start		= { .name = "te_l_bus_start", 	.base = 0x0,	.size = 0 };	/* L Bus fixed */

static LX_MEMCFG_T	__3GB_G_bus_gfx_start		= { .name = "gfx_g_bus_start",  .base = 0x0,	.size = 0 };	/* G Bus fixed */
static LX_MEMCFG_T	__3GB_G_bus_vdec_start		= { .name = "vdec_g_bus_start", .base = 0x0,	.size = 0 };	/* G Bus fixed */

static LX_MEMCFG_ENTRY_T g_mem_desc_table_DDR_3GB[] =
{
	LX_MEMCFG_ENTRY(sMemCfgHdr[0]),				/* DDR0 START */

	LX_MEMCFG_ENTRY(__3GB_L_bus_gfx_start),

#if DDR_GFX_BUS_ID==__L_BUS__
#ifdef INCLUDE_KDRV_GFX
	LX_MEMCFG_ENTRY(gMemCfgGfx.surface[0]),		/* GFX */
#endif
#endif

#if DDR_OSD_BUS_ID==__L_BUS__
	LX_MEMCFG_ENTRY(__3GB_L_bus_osd_start),
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
#endif

	LX_MEMCFG_ENTRY(__3GB_L_bus_etc_start),

#ifdef INCLUDE_KDRV_PNG
	LX_MEMCFG_ENTRY(gMemCfgPng[1]),				/* PNG */
#endif
#ifdef INCLUDE_KDRV_MJPEG
	LX_MEMCFG_ENTRY(gMemCfgMjpeg),				/* MJPEG */
#endif

#ifdef INCLUDE_KDRV_DE
	LX_MEMCFG_ENTRY(gMemCfgDeFW[4]),			/* DE F/W - temporary */
#endif
	LX_MEMCFG_ENTRY(__3GB_L_bus_sys_start),

	LX_MEMCFG_ENTRY(gMemCfgSYS),

#ifdef INCLUDE_KDRV_SE
	LX_MEMCFG_ENTRY(gMemCfgSE[1]),				/* SE */
	LX_MEMCFG_ENTRY(gMemCfgSE_TZFW[1]),			/* SE/TZ */
#endif

	LX_MEMCFG_ENTRY(__3GB_L_bus_venc_start),

#ifdef INCLUDE_KDRV_VENC
	LX_MEMCFG_ENTRY(gMemCfgVenc),			/* VENC */
#endif

	LX_MEMCFG_ENTRY(__3GB_L_bus_vip_start),

	LX_MEMCFG_ENTRY(__3GB_L_bus_aud_start),

#ifdef INCLUDE_KDRV_AUDIO
	LX_MEMCFG_ENTRY(g_stMemCfgAud[0]),			/* AUDIO */
#endif

	LX_MEMCFG_ENTRY(__3GB_L_bus_te_start),
#ifdef INCLUDE_KDRV_SDEC
	LX_MEMCFG_ENTRY(gMemCfgSDEC),			/* SDEC/TE */
#endif
#ifdef INCLUDE_KDRV_PVR
	LX_MEMCFG_ENTRY(gMemCfgPvr),				/* SDEC/PVR */
#endif

	LX_MEMCFG_ENTRY(sMemCfgHdr[1]), 			 /* DDR1 START */
	LX_MEMCFG_ENTRY(__3GB_G_bus_gfx_start),
#if DDR_GFX_BUS_ID==__G_BUS__
  #ifdef INCLUDE_KDRV_GFX
	LX_MEMCFG_ENTRY(gMemCfgGfx.surface[0]),		/* GFX */
  #endif
#endif

#if DDR_OSD_BUS_ID==__G_BUS__
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
#endif

	LX_MEMCFG_ENTRY(__3GB_G_bus_vdec_start),
	LX_MEMCFG_ENTRY(vdec_hma),

#ifdef INCLUDE_KDRV_VBI
	LX_MEMCFG_ENTRY(gstBufMemCfgVBIRev[1]),		/* VBI Buffer -- FIXME DDR1 ??? */
#endif
#ifdef INCLUDE_KDRV_OVI
	LX_MEMCFG_ENTRY(gMemCfgOviTcon),			/* OVI TCON -- FIXME DDR1 ??? */
#endif

#ifdef INCLUDE_KDRV_AFE
	LX_MEMCFG_ENTRY(gCvdMemCfg),				/* CVD (3DComb H/W Memory) -- FIXME DDR1 ??? */
#endif

	LX_MEMCFG_ENTRY(sMemCfgHdr[2]), 			 /* DDR2 START */

#ifdef INCLUDE_KDRV_DE
    LX_MEMCFG_ENTRY(gMemCfgDe[4]),				/* DE */
#endif
	LX_MEMCFG_ENTRY_END
};



/*========================================================================================
	Implementation Group
========================================================================================*/
static void	BASE_PLT_1GB_Init   ( void )
{
	int	i;
	int	ret;

	/* determine memory layout */
	g_memory_desc_table[0].base = 0x12000000;	// 0x1200_0000 ~ 0x4000_0000
	g_memory_desc_table[0].size = 736<<20;		// 0x2e000000

	g_memory_desc_table[1].base = 0x34000000;
	g_memory_desc_table[1].size = 192<<20;

	vdec_hma.base = 0x28000000;
	vdec_hma.size = 192<<20;

	/* make sMemCfgHdr */
	for (i=0; i<MAX_DDR_NUM; i++ )
	{
		sMemCfgHdr[i].base	= g_memory_desc_table[i].base;
		sMemCfgHdr[i].size	= g_memory_desc_table[i].size;
	}

	ret = BASE_DEVMEM_InitDevMem( g_mem_desc_table_DDR_1GB );
	if ( ret != RET_OK ) DBG_PRINT_ERROR("fatal error during making device memory layout\n");

	/* register vdec_hma memory pool */
	ret = hma_pool_register ("ddr0", vdec_hma.base, vdec_hma.size);
	if (ret < 0)
		printk (KERN_ERR "hma_pool_register failed\n");
}

static void	BASE_PLT_3GB_Init   ( void )
{
	int	i;
	int	ret;

	/* determine memory layout */
	printk("H15 FPGA 3G DDR - ADDR SWITCH <ON> !!!\n");

	g_memory_desc_table[0].base = 0x18000000;
	g_memory_desc_table[0].size = 640<<20;

	g_memory_desc_table[1].base = 0x54000000;
	g_memory_desc_table[1].size = 704<<20;

	g_memory_desc_table[2].base = 0x80000000;
	g_memory_desc_table[2].size = 1024<<20;

	__3GB_L_bus_gfx_start.base	= 0x18000000;	/* fixed by FPGA verification mem map + 16MB offset */
	__3GB_L_bus_osd_start.base	= 0x20000000;	/* etc */
	__3GB_L_bus_etc_start.base	= 0x2e000000;	/* etc */
	__3GB_L_bus_sys_start.base	= 0x2f000000;	/* etc */
	__3GB_L_bus_venc_start.base	= 0x30000000;	/* fixed by FPGA verification mem map */
	__3GB_L_bus_vip_start.base	= 0x34000000;	/* fixed by FPGA verification mem map */
	__3GB_L_bus_aud_start.base	= 0x38000000;	/* fixed by FPGA verification mem map */
	__3GB_L_bus_te_start.base	= 0x3c000000;	/* fixed by FPGA verification mem map */

	__3GB_G_bus_gfx_start.base	= 0x54000000;	/* fixed by FPGA verification mem map */
	__3GB_G_bus_vdec_start.base	= 0x64000000;	/* fixed by FPGA verification mem map */

	vdec_hma.base = 0x64000000;					/* fixed by FPGA verification mem map */
	vdec_hma.size = 192<<20;

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
		printk (KERN_ERR "hma_pool_register failed\n");
}

void	BASE_PLT_Init (void)
{
#if 	FPGA_DDR_MEM_SIZE == 0x40000000
	BASE_PLT_1GB_Init( );
#elif	FPGA_DDR_MEM_SIZE == 0xc0000000
	BASE_PLT_3GB_Init( );
#else
	#error unknown memory size !!!
#endif
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

