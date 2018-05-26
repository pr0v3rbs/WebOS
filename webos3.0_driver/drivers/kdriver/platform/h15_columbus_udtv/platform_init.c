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

#undef	USE_MEM_WR_BUFFER0					/* enable/disable mem buffer */

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
#define DDR_CALIB_RGN_SZ	__KB(16)	/* 16KB DDR calribration area */
#define SYSTEM_RGN_SZ		__MB(12)	/* system/security area. 12MB fixed */

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define LX_MEMCFG_ENTRY(_m_entry)	{ .name= _STR(_m_entry), .pM=(LX_MEMCFG_T*)&(_m_entry), .nM=sizeof(_m_entry)/sizeof(LX_MEMCFG_T) }
#define LX_MEMCFG_ENTRY_END			{ .name="Total", .pM=NULL, .nM=0}

#define	__L_BUS__			0
#define	__G_BUS__			1
#define	__E_BUS__			2


/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/
typedef enum
{
	DDR_RGN_VDEC0 			= 0,
	DDR_RGN_LBUS_DEV 		= 1,
	DDR_RGN_VDEC1			= 2,
	DDR_RGN_EBUS_DEV 		= 3,

	DDR_RGN_GBUS_CALIB		= 4,
	DDR_RGN_EBUS_CALIB		= 5,

	DDR_RGN_MAX,
}
DDR_RGN_T;

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
static LX_MEMCFG_T	g_memory_desc_table[DDR_RGN_MAX]= { { .name = "DDR0",.base = 0x0,	.size = 0 },
														{ .name = "DDR1",.base = 0x0,	.size = 0 },
														{ .name = "DDR2",.base = 0x0,	.size = 0 },
														{ .name = "DDR3",.base = 0x0,	.size = 0 },
														{ .name = "DDR4",.base = 0x0,	.size = 0 },
														{ .name = "DDR5",.base = 0x0,	.size = 0 }};

static LX_MEMCFG_T	sMemCfgHdr[DDR_RGN_MAX]={{.name = "[header] DDR0",.base = 0x0, .size = 0 /* do not allocate */ },
											{ .name = "[header] DDR1",.base = 0x0, .size = 0 /* do not allocate */ },
											{ .name = "[header] DDR2",.base = 0x0, .size = 0 /* do not allocate */ },
											{ .name = "[header] DDR3",.base = 0x0, .size = 0 /* do not allocate */ },
											{ .name = "[header] DDR4",.base = 0x0, .size = 0 /* do not allocate */ },
											{ .name = "[header] DDR5",.base = 0x0, .size = 0 /* do not allocate */ } };

static LX_MEMCFG_T	gMemCfgSYS				={ .name = "SYS", .base = 0x0, 	.size = 0x100000 };
static LX_MEMCFG_T	gMemCfgVIP				={ .name = "VIP", .base = 0x0, 	.size = __MB(64) };
static LX_MEMCFG_T	vdec_hma 				={ .name = "vdec_hma", .base = 0x0,	.size = 0 };
static LX_MEMCFG_T	vdec_uhd_dpb 			={ .name = "vdec_uhd_dpb", .base = 0x0,	.size = 0 };
static LX_MEMCFG_T	__L_bus_static_mem_start={ .name = "static_mem",  .base = 0x0,	.size = 0 }; /* mem separator */

static LX_MEMCFG_T	rsvd_G_bus_ddr_calib	={.name = "rsvd_ddr_calib(g)", .base = 0x0, .size = 0 };
static LX_MEMCFG_T	rsvd_E_bus_ddr_calib	={.name = "rsvd_ddr_calib(e)", .base = 0x0, .size = 0 };

#ifdef USE_MEM_WR_BUFFER0
static LX_MEMCFG_T	__mem_wr_buffer0		={ .name = "mem_wr_buf0",  	.base = 0x0, .size = 0 }; /* mem workaround #0 */
#endif

/*---------------------------------------------------------------------------------------
 * Global Memory Configuration data base (H13/GP)
 *--------------------------------------------------------------------------------------*/
static LX_MEMCFG_ENTRY_T g_mem_desc_table_DDR_3GB[] =
{
	//-----------------------------------------------------------------------------------
	//	DDR (RGN_VDEC0)
	//-----------------------------------------------------------------------------------
	LX_MEMCFG_ENTRY(sMemCfgHdr[DDR_RGN_VDEC0]),
	LX_MEMCFG_ENTRY(vdec_hma),

	//-----------------------------------------------------------------------------------
	//	DDR (RSVD for DDR calribration)
	//-----------------------------------------------------------------------------------
	LX_MEMCFG_ENTRY(sMemCfgHdr[DDR_RGN_GBUS_CALIB]),
	LX_MEMCFG_ENTRY(rsvd_G_bus_ddr_calib),

	//-----------------------------------------------------------------------------------
	//	DDR (LBUS_DEV)
	//
	//
	//	276MB - GFX,OSD,AUDIO,TE,ICOD,VENC
	//	 16MB - static mem
	//-----------------------------------------------------------------------------------
	LX_MEMCFG_ENTRY(sMemCfgHdr[DDR_RGN_LBUS_DEV]),

#ifdef INCLUDE_KDRV_SDEC
	LX_MEMCFG_ENTRY(gMemCfgSDEC),				/* SDEC/TE */
#endif

#ifdef INCLUDE_KDRV_GFX
	LX_MEMCFG_ENTRY(gMemCfgGfx.surface[0]),		/* GFX */
#endif

#ifdef INCLUDE_KDRV_VENC
	LX_MEMCFG_ENTRY(gMemCfgVenc),				/* VENC */
#endif

#ifdef USE_MEM_WR_BUFFER0
	LX_MEMCFG_ENTRY(__mem_wr_buffer0),
#endif

#ifdef INCLUDE_KDRV_AUDIO
	LX_MEMCFG_ENTRY(g_stMemCfgAud[0]),			/* AUDIO */
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
#ifdef INCLUDE_KDRV_MJPEG
	LX_MEMCFG_ENTRY(gMemCfgMjpeg),				/* MJPEG */
#endif


#if 0
	LX_MEMCFG_ENTRY(gMemCfgVIP),				/* VIP */
#endif
	LX_MEMCFG_ENTRY(__L_bus_static_mem_start),
	LX_MEMCFG_ENTRY(gMemCfgSYS),
#ifdef INCLUDE_KDRV_SE
	LX_MEMCFG_ENTRY(gMemCfgSE[2]),				/* SE */
	LX_MEMCFG_ENTRY(gMemCfgSE_TZFW[2]),			/* SE/TZ */
#endif

	//-----------------------------------------------------------------------------------
	//	DDR (RGN_VDEC1)
	//-----------------------------------------------------------------------------------
	LX_MEMCFG_ENTRY(sMemCfgHdr[DDR_RGN_VDEC1]),
	LX_MEMCFG_ENTRY(vdec_uhd_dpb),

	//-----------------------------------------------------------------------------------
	//	DDR (RGN_EBUS_DEV)
	//-----------------------------------------------------------------------------------
	LX_MEMCFG_ENTRY(sMemCfgHdr[DDR_RGN_EBUS_DEV]),
#ifdef INCLUDE_KDRV_PVR
	LX_MEMCFG_ENTRY(gMemCfgPvr),				/* SDEC/PVR */
#endif

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

	//-----------------------------------------------------------------------------------
	//	DDR #6 (RSVD for DDR calribration)
	//-----------------------------------------------------------------------------------
	LX_MEMCFG_ENTRY(sMemCfgHdr[DDR_RGN_EBUS_CALIB]),
	LX_MEMCFG_ENTRY(rsvd_E_bus_ddr_calib),

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

	#if 1
	// 9-2th map
	g_memory_desc_table[DDR_RGN_VDEC0].base 	= 0x4B000000;	/* VDEC FHD/UHD @ G_BUS */
	g_memory_desc_table[DDR_RGN_VDEC0].size 	= __MB(560)  - DDR_CALIB_RGN_SZ;
	#else
	// 9-1th map
	g_memory_desc_table[DDR_RGN_VDEC0].base 	= 0x46000000;	/* VDEC FHD/UHD @ G_BUS */
	g_memory_desc_table[DDR_RGN_VDEC0].size 	= __MB(640)  - DDR_CALIB_RGN_SZ;
	#endif

	g_memory_desc_table[DDR_RGN_LBUS_DEV].base 	= 0x6E000000;	/* LBUS DEV @ L_BUS */
	g_memory_desc_table[DDR_RGN_LBUS_DEV].size 	= __MB(288);

	#if 1
	// 9-2th map
	g_memory_desc_table[DDR_RGN_VDEC1].base 	= 0x93000000;	/* VDEC UHD @ E_BUS */
	g_memory_desc_table[DDR_RGN_VDEC1].size 	= __MB(208);
	#else
	// 9-1th map
	g_memory_desc_table[DDR_RGN_VDEC1].base 	= 0x92000000;	/* VDEC UHD @ E_BUS */
	g_memory_desc_table[DDR_RGN_VDEC1].size 	= __MB(224);
	#endif

	g_memory_desc_table[DDR_RGN_EBUS_DEV].base 	= 0xA0000000;	/* EBUS DEV @ E_BUS */
	g_memory_desc_table[DDR_RGN_EBUS_DEV].size 	= __MB(512) - DDR_CALIB_RGN_SZ;

	g_memory_desc_table[DDR_RGN_GBUS_CALIB].base = 0x6E000000 - DDR_CALIB_RGN_SZ;
	g_memory_desc_table[DDR_RGN_GBUS_CALIB].size = DDR_CALIB_RGN_SZ;

	g_memory_desc_table[DDR_RGN_EBUS_CALIB].base= 0xC0000000 - DDR_CALIB_RGN_SZ;
	g_memory_desc_table[DDR_RGN_EBUS_CALIB].size = DDR_CALIB_RGN_SZ;

#ifdef USE_MEM_WR_BUFFER0
	__mem_wr_buffer0.size	= __MB(6);
#endif

	/* setup HMA region for VDEC */
	vdec_hma.base = g_memory_desc_table[DDR_RGN_VDEC0].base;
	vdec_hma.size = g_memory_desc_table[DDR_RGN_VDEC0].size;

	vdec_uhd_dpb.base = g_memory_desc_table[DDR_RGN_VDEC1].base;
	vdec_uhd_dpb.size = g_memory_desc_table[DDR_RGN_VDEC1].size;

	/* static mem description - 16MB block in L_BUS */
	__L_bus_static_mem_start.base = (g_memory_desc_table[DDR_RGN_LBUS_DEV].base + g_memory_desc_table[DDR_RGN_LBUS_DEV].size - SYSTEM_RGN_SZ);

	/* setup VIP for test purpose only */
	gMemCfgVIP.base = __L_bus_static_mem_start.base - gMemCfgVIP.size;

	/* setup DDR calibration region */
	rsvd_G_bus_ddr_calib.base	= g_memory_desc_table[DDR_RGN_GBUS_CALIB].base;
	rsvd_G_bus_ddr_calib.size	= g_memory_desc_table[DDR_RGN_GBUS_CALIB].size;
	rsvd_E_bus_ddr_calib.base	= g_memory_desc_table[DDR_RGN_EBUS_CALIB].base;
	rsvd_E_bus_ddr_calib.size	= g_memory_desc_table[DDR_RGN_EBUS_CALIB].size;

	/* make sMemCfgHdr */
	for (i=0; i<DDR_RGN_MAX; i++ )
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

