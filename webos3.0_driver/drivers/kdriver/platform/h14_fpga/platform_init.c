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
#define	MAX_DDR_NUM		2

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define LX_MEMCFG_ENTRY(_m_entry)	{ .name= _STR(_m_entry), .pM=(LX_MEMCFG_T*)&(_m_entry), .nM=sizeof(_m_entry)/sizeof(LX_MEMCFG_T) }
#define LX_MEMCFG_ENTRY_END			{ .name="Total", .pM=NULL, .nM=0}

#define _MB_(_sz)	(( _sz)>>20)
#define _KB_(_sz)	(((_sz)>>10)&0x3ff)
#define _BB_(_sz)	(( _sz)     &0x3ff)

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
															{ .name = "DDR1", .base = 0x0,	.size = 0 } };

static LX_MEMCFG_T	sMemCfgHdr[MAX_DDR_NUM]={ 	{ .name = "[header] DDR0", .base = 0x0, .size = 0 /* do not allocate */ },
												{ .name = "[header] DDR1", .base = 0x0, .size = 0 /* do not allocate */ } };

static LX_MEMCFG_T	gMemCfgGPU			= 	{ .name = "GPU", .base = 0x0, .size = 0 };
//static LX_MEMCFG_T gMemCfgSYS			= 	{ .name = "SYS", .base = 0x7f500000, .size = 0x100000 };
static LX_MEMCFG_T	gMemCfgSYS			= 	{ .name = "SYS", .base = 0x3f500000, .size = 0x100000 };
static LX_MEMCFG_T  gMemCfg0x7000_0000  =   { .name = "tmp spliter", .base = 0x70000000, .size = 0 };
static LX_MEMCFG_T	gMemCfgReserved_0	= 	{ .name = "RSV0", .base = 0x0, .size = 0x34000 };
static LX_MEMCFG_T	gMemCfgReserved_1	= 	{ .name = "RSV1", .base = 0x0, .size = 0xA8000 };

static LX_MEMCFG_T	hma 				= 	{ .name = "ddr0", .base = 0x0,	.size = 170*1024*1024 };

/*---------------------------------------------------------------------------------------
 * Global Memory Configuration data base (H13/GP)
 *--------------------------------------------------------------------------------------*/
static LX_MEMCFG_ENTRY_T	g_mem_desc_table[] =
{
	LX_MEMCFG_ENTRY(sMemCfgHdr[0]),				/* DDR0 START */
#ifdef INCLUDE_KDRV_GPU
	LX_MEMCFG_ENTRY(gMemCfgGPU),				/* GPU */
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

	LX_MEMCFG_ENTRY(gMemCfgReserved_0),				/* Reserved for 16MB-align */

#ifdef INCLUDE_KDRV_PNG
	LX_MEMCFG_ENTRY(gMemCfgPng[1]),				/* PNG */
#endif
#ifdef INCLUDE_KDRV_SDEC
	LX_MEMCFG_ENTRY(gMemCfgSDEC),			/* SDEC/TE */
#endif
#ifdef INCLUDE_KDRV_VENC
	LX_MEMCFG_ENTRY(gMemCfgVenc),	/* VENC */
#endif
#ifdef INCLUDE_KDRV_DE
	LX_MEMCFG_ENTRY(gMemCfgDeFW[3]),				/* DE F/W - temporary */
#endif
#ifdef INCLUDE_KDRV_DE
	LX_MEMCFG_ENTRY(gMemCfgDeGrap[3]),			/* DE */
#endif

	LX_MEMCFG_ENTRY(gMemCfgReserved_1),				/* Reserved for 16MB-align */

#ifdef INCLUDE_KDRV_AFE
	LX_MEMCFG_ENTRY(gCvdMemCfg),				/* CVD (3DComb H/W Memory) */
#endif

#ifdef INCLUDE_KDRV_MJPEG
	LX_MEMCFG_ENTRY(gMemCfgMjpeg),			/* MJPEG */
#endif

	LX_MEMCFG_ENTRY(gMemCfg0x7000_0000),		/* raxis.lim -- temporary 0x7000_0000 spliter */

#ifdef INCLUDE_KDRV_AUDIO
	LX_MEMCFG_ENTRY(g_stMemCfgAud[0]),			/* AUDIO */
#endif
#ifdef INCLUDE_KDRV_VBI
	LX_MEMCFG_ENTRY(gstBufMemCfgVBIRev[1]),		/* VBI Buffer */
#endif
#ifdef INCLUDE_KDRV_OVI
	LX_MEMCFG_ENTRY(gMemCfgOviTcon),				/* OVI TCON */
#endif

#ifdef INCLUDE_KDRV_DE
	LX_MEMCFG_ENTRY(gMemCfgDePreW[3]),			/* DE */
#endif
#ifdef INCLUDE_KDRV_PVR
	LX_MEMCFG_ENTRY(gMemCfgPvr),				/* SDEC/PVR */
#endif
	LX_MEMCFG_ENTRY(hma),

	//LX_MEMCFG_ENTRY(gMemCfgSYS),
#ifdef INCLUDE_KDRV_SE
	LX_MEMCFG_ENTRY(gMemCfgSE[1]),					/* SE */
	LX_MEMCFG_ENTRY(gMemCfgSE_TZFW[1]),			/* SE/TZ */
#endif

	LX_MEMCFG_ENTRY(sMemCfgHdr[1]), 			 /* DDR1 START */

#ifdef INCLUDE_KDRV_DE
	LX_MEMCFG_ENTRY(gMemCfgDe[3]),				/* DE */
#endif
#ifdef INCLUDE_KDRV_BE
	LX_MEMCFG_ENTRY(gMemCfgBe[2]),				/* BE */
#endif
	LX_MEMCFG_ENTRY_END

};

/*========================================================================================
	Implementation Group
========================================================================================*/
void        BASE_PLT_Init   ( void )
{
	int	i;
	int	ret = RET_ERROR;

	/* determine memory layout */
//	g_memory_desc_table[0].base = 0x5d000000;	/* 0x6000_0000 - 48MB */
//	g_memory_desc_table[0].size = 560<<20;		/* 512MB + 48MB */
//	g_memory_desc_table[1].base = 0x80000000;
//	g_memory_desc_table[1].size = 512<<20;

//	g_memory_desc_table[0].base = 0x20000000;	/* 0x6000_0000 - 48MB */
//	g_memory_desc_table[0].base = 0x5d000000;
//	g_memory_desc_table[0].size = 1536<<20;		/* 512MB + 48MB */
//	g_memory_desc_table[0].size = 560 << 20;
//	g_memory_desc_table[1].base = 0x80000000;
//	g_memory_desc_table[1].size = 512<<20;

	// 0x60000000 ~ 0x80000000 : 512M --> DDR0
	// 0x90000000 ~ 0xa0000000 : 512M --> DDR1

	g_memory_desc_table[0].base = 0x60000000;
	g_memory_desc_table[0].size = 512 << 20;

	g_memory_desc_table[1].base = 0x80000000;
	g_memory_desc_table[1].size = 768 << 20;

	/* make sMemCfgHdr */
	for (i=0; i<MAX_DDR_NUM; i++ )
	{
		sMemCfgHdr[i].base	= g_memory_desc_table[i].base;
		sMemCfgHdr[i].size	= g_memory_desc_table[i].size;
	}

	ret = BASE_DEVMEM_InitDevMem( g_mem_desc_table );

	if ( ret != RET_OK ) DBG_PRINT_ERROR("fatal error during making device memory layout\n");

	/* register hma memory pool */
	ret = hma_pool_register ("ddr0", hma.base, hma.size);
	if (ret < 0)
		printk (KERN_ERR "hma_pool_register failed\n");
}


UINT32  BASE_PLT_Probe (void)
{
	return LX_CHIP_PLT_FHD;
}

/** @} */

