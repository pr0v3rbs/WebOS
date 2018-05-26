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
#undef  HWOPT_PARSE_TEST
#ifdef  HWOPT_PARSE_TEST
static char* test_command_line = "mem=1504M ddr0mem=1024M ddr1mem=1024M ethaddr=00:00:00:00:00:00 rev=1 console=ttyAMA0,115200n81 bver=3.02.38 chip=H13B0 platform=NC4 ace=H13A1 quiet loglevel=0 ip=0.0.0.0::eth0:off transserver=10.186.119.146 rootwait emmc_size=0x1d2000000 sbkey=0x7f5e0000 modelopt=00000001011  hwoption=00120400100401 hwopt=001204001004013003 portProtection sdhci-lg115x.emmc_tab=0x193A631 l2cache=512K ToolOpt=33331:0:0:0:256:0:0 countryGrp=0 debugMode=5 modelName=WEBOS1 cmdEnd";
#endif

//#define MEMMAP_16MB_ALIGNED

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

#ifdef INCLUDE_KDRV_ADEC
#include "adec_cfg.h"
#endif
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
//lgjh.kim
#define	MAX_DDR_NUM			4	/* FHD : 2, UHD : 4 */

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
static UINT32		g_grpahic_res = LX_RES_FHD;	/* 1920x1080 by default */

static LX_MEMCFG_T	g_memory_desc_table[MAX_DDR_NUM]		= { { .name = "DDR0", .base = 0x0,	.size = 0 },
																{ .name = "DDR1", .base = 0x0,	.size = 0 },
																{ .name = "DDR2", .base = 0x0,	.size = 0 },
																{ .name = "DDR3", .base = 0x0,	.size = 0 } };

static LX_MEMCFG_T	sMemCfgHdr[MAX_DDR_NUM]			={ 	{ .name = "[header] DDR0", .base = 0x0, .size = 0 /* do not allocate */ },
														{ .name = "[header] DDR1", .base = 0x0, .size = 0 /* do not allocate */ },
														{ .name = "[header] DDR2", .base = 0x0, .size = 0 /* do not allocate */ },
														{ .name = "[header] DDR2", .base = 0x0, .size = 0 /* do not allocate */ } };

static LX_MEMCFG_T	gMemCfgGPU			= 	{ .name = "GPU", .base = 0x0, .size = 0 };
static LX_MEMCFG_T	gMemCfgSYS			= 	{ .name = "SYS", .base = 0x7f500000, .size = 0x100000 };
static LX_MEMCFG_T	gMemCfg0x7000_0000	= 	{ .name = "tmp spliter", .base = 0x70000000, .size = 0 };

static LX_MEMCFG_T	hma_ddr0 			= 	{ .name = "ddr0", .base = 0x0,	.size = 170*1024*1024 };
static LX_MEMCFG_T	hma_ddr1			= 	{ .name = "ddr1", .base = 0x0,	.size = 180*1024*1024 };

/*---------------------------------------------------------------------------------------
 * Global Memory Configuration data base (H13/GP)
 *--------------------------------------------------------------------------------------*/
static LX_MEMCFG_ENTRY_T	g_mem_desc_table_FHD[] =
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
#ifdef INCLUDE_KDRV_PNG
	LX_MEMCFG_ENTRY(gMemCfgPng[1]),				/* PNG */
#endif
#ifdef INCLUDE_KDRV_VENC
	LX_MEMCFG_ENTRY(gMemCfgVenc),	/* VENC */
#endif
#ifdef INCLUDE_KDRV_SDEC
	LX_MEMCFG_ENTRY(gMemCfgSDEC),			/* SDEC/TE */
#endif
#ifdef INCLUDE_KDRV_DE
	LX_MEMCFG_ENTRY(gMemCfgDeFW[0]),				/* DE F/W - temporary */
#endif
#ifdef INCLUDE_KDRV_DE
	LX_MEMCFG_ENTRY(gMemCfgDeGrap[0]),			/* DE */
#endif
#ifdef INCLUDE_KDRV_AFE
	LX_MEMCFG_ENTRY(gCvdMemCfg),				/* CVD (3DComb H/W Memory) */
#endif
#ifdef INCLUDE_KDRV_OVI
	LX_MEMCFG_ENTRY(gMemCfgOviTcon),				/* OVI TCON */
#endif

	LX_MEMCFG_ENTRY(gMemCfg0x7000_0000),		/* raxis.lim -- temporary 0x7000_0000 spliter */

#ifdef INCLUDE_KDRV_DE
	LX_MEMCFG_ENTRY(gMemCfgDePreW[0]),			/* DE */
#endif
#ifdef INCLUDE_KDRV_AUDIO
	LX_MEMCFG_ENTRY(g_stMemCfgAud[0]),			/* AUDIO */
#endif

#ifdef INCLUDE_KDRV_PVR
	LX_MEMCFG_ENTRY(gMemCfgPvr),				/* SDEC/PVR */
 //	LX_MEMCFG_ENTRY(gMemCfgPvr),
#endif
#ifdef INCLUDE_KDRV_VBI
	LX_MEMCFG_ENTRY(gstBufMemCfgVBIRev[1]),		/* VBI Buffer */
#endif
	LX_MEMCFG_ENTRY(hma_ddr0),

#ifdef INCLUDE_KDRV_MJPEG
	LX_MEMCFG_ENTRY(gMemCfgMjpeg),			/* MJPEG */
#endif

	LX_MEMCFG_ENTRY(gMemCfgSYS),

#ifdef INCLUDE_KDRV_SE
	LX_MEMCFG_ENTRY(gMemCfgSE[0]),					/* SE */
	LX_MEMCFG_ENTRY(gMemCfgSE_TZFW[0]),			/* SE/TZ */
#endif

	LX_MEMCFG_ENTRY(sMemCfgHdr[1]), 			 /* DDR1 START */

#ifdef INCLUDE_KDRV_DE
	LX_MEMCFG_ENTRY(gMemCfgDe[0]),				/* DE */
#endif
#ifdef INCLUDE_KDRV_BE
	LX_MEMCFG_ENTRY(gMemCfgBe[0]),				/* BE */
#endif
	LX_MEMCFG_ENTRY_END

};

static LX_MEMCFG_ENTRY_T	g_mem_desc_table_UHD_4K[] =
{
	LX_MEMCFG_ENTRY(sMemCfgHdr[0]),				/* DDR0 (AV) START */
#ifdef INCLUDE_KDRV_GPU
		LX_MEMCFG_ENTRY(gMemCfgGPU),				/* GPU */
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
#ifdef INCLUDE_KDRV_VENC
	LX_MEMCFG_ENTRY(gMemCfgVenc),			/* VENC */
#endif
#ifdef INCLUDE_KDRV_SDEC
	LX_MEMCFG_ENTRY(gMemCfgSDEC),			/* SDEC */
#endif
#ifdef INCLUDE_KDRV_DE
	LX_MEMCFG_ENTRY(gMemCfgDeFW[0]),			/* DE F/W - temporary */
#endif
#ifdef INCLUDE_KDRV_DE
	LX_MEMCFG_ENTRY(gMemCfgDeGrap[0]),			/* DE */
#endif
#ifdef INCLUDE_KDRV_AFE
	LX_MEMCFG_ENTRY(gCvdMemCfg),				/* CVD (3DComb H/W Memory) */
#endif
#ifdef INCLUDE_KDRV_OVI
	LX_MEMCFG_ENTRY(gMemCfgOviTcon),			/* OVI TCON */
#endif
	LX_MEMCFG_ENTRY(hma_ddr1),					/* raxis.lim -- second hma region */

	LX_MEMCFG_ENTRY(gMemCfg0x7000_0000),		/* raxis.lim -- temporary 0x7000_0000 spliter */

#ifdef INCLUDE_KDRV_DE
	LX_MEMCFG_ENTRY(gMemCfgDePreW[0]),			/* DE */
#endif
#ifdef INCLUDE_KDRV_ADEC
	LX_MEMCFG_ENTRY(gstMemCfgADEC[1]),			/* ADEC */
#endif
#ifdef INCLUDE_KDRV_AUDIO
	LX_MEMCFG_ENTRY(g_stMemCfgAud[0]),			/* AUDIO */
#endif

#ifdef INCLUDE_KDRV_PVR
	LX_MEMCFG_ENTRY(gMemCfgPvr),				/* SDEC/PVR */
 //	LX_MEMCFG_ENTRY(gMemCfgPvr),
#endif
#ifdef INCLUDE_KDRV_VBI
	LX_MEMCFG_ENTRY(gstBufMemCfgVBIRev[1]),		/* VBI Buffer */
#endif
	LX_MEMCFG_ENTRY(hma_ddr0),

#ifdef INCLUDE_KDRV_MJPEG
	LX_MEMCFG_ENTRY(gMemCfgMjpeg),				/* MJPEG */
#endif

	LX_MEMCFG_ENTRY(gMemCfgSYS),

#ifdef INCLUDE_KDRV_SE
	LX_MEMCFG_ENTRY(gMemCfgSE[0]),					/* SE */
	LX_MEMCFG_ENTRY(gMemCfgSE_TZFW[0]),			/* SE/TZ */
#endif

	LX_MEMCFG_ENTRY(sMemCfgHdr[1]), 			 /* DDR1(DISPLAY) START */

#ifdef INCLUDE_KDRV_DE
	LX_MEMCFG_ENTRY(gMemCfgDe[1]),				/* DE */
#endif
#ifdef INCLUDE_KDRV_BE
	LX_MEMCFG_ENTRY(gMemCfgBe[0]),				/* BE */
#endif

	LX_MEMCFG_ENTRY(sMemCfgHdr[2]), 			 /* DDR2(GFX_A) START */

#ifdef INCLUDE_KDRV_GFX
		LX_MEMCFG_ENTRY(gMemCfgGfx.surface[0]),		/* GFX (Low Mem) */
#endif

	LX_MEMCFG_ENTRY(sMemCfgHdr[3]), 			 	/* DDR2(GFX_B) START */
#ifdef INCLUDE_KDRV_GFX
	#if CONFIG_LX_GFX_MEM_RGN_NUM==2
		LX_MEMCFG_ENTRY(gMemCfgGfx.surface[1]),		/* GFX (High Mem) */
	#endif
#endif

	LX_MEMCFG_ENTRY_END
};

/*========================================================================================
	Implementation Group
========================================================================================*/

/*
 *  Physical Memory Map(DDR Logical View) -- FHD model
 *
 *  _______________________
 * |						| 0xA000 0000
 * |	DISPLAY (256M)		|
 * |------------------------| 0x9000 0000
 * |						|
 * |------------------------| 0x8000 0000
 * |	AV Driver (560M)	|
 * |------------------------| 0x5D00 0000
 * |						|
 * |------------------------| 0x4D00 0000
 * |	CPU Kernel (1232M)	|
 * |________________________| 0x0000 0000
 *
 *
 *
 */

static void	BASE_PLT_FHD_Init   ( void )
{
	int	i;
	int	ret = RET_ERROR;

	/* determine memory layout */
	g_memory_desc_table[0].base = 0x5D000000; 	//0x5D00_0000 - 0x7FFF_FFFF
	g_memory_desc_table[0].size = 560<<20; 		//560MB

	g_memory_desc_table[1].base = 0x90000000;	//0x9000_0000 - 0x9FFF_FFFF
	g_memory_desc_table[1].size = 256<<20; 		//256MB

	/* make sMemCfgHdr */
	for (i=0; i<MAX_DDR_NUM; i++ )
	{
		sMemCfgHdr[i].base	= g_memory_desc_table[i].base;
		sMemCfgHdr[i].size	= g_memory_desc_table[i].size;
	}

	ret = BASE_DEVMEM_InitDevMem( g_mem_desc_table_FHD );

	if ( ret != RET_OK ) DBG_PRINT_ERROR("fatal error during making device memory layout\n");

	/* register hma memory pool */
	ret = hma_pool_register ("ddr0", hma_ddr0.base, hma_ddr0.size);
	if (ret < 0)
		printk (KERN_ERR "hma_pool_register failed\n");
}

static void	BASE_PLT_UHD_Init   ( void )
{
	int	i;
	int	ret = RET_ERROR;

	/* determine memory layout */
	g_memory_desc_table[0].base = 0x60000000;			
	g_memory_desc_table[0].size = 512<<20;	/* 512MB(AV) */ 

	g_memory_desc_table[1].base = 0x90000000;			
	g_memory_desc_table[1].size = 256<<20;	/* 256MB(DISPLAY) */ 

	g_memory_desc_table[2].base = 0x80000000;
	g_memory_desc_table[2].size = 256<<20; /* 256MB(GFX Low Mem) */ 

	g_memory_desc_table[3].base = 0xA0000000;
	g_memory_desc_table[3].size = 256<<20; /* 256MB(GFX High Mem) */ 

	/* make sMemCfgHdr */
	for (i=0; i<MAX_DDR_NUM; i++ )
	{
		sMemCfgHdr[i].base	= g_memory_desc_table[i].base;
		sMemCfgHdr[i].size	= g_memory_desc_table[i].size;
	}

	ret = BASE_DEVMEM_InitDevMem( g_mem_desc_table_UHD_4K );

	if ( ret != RET_OK ) DBG_PRINT_ERROR("fatal error during making device memory layout\n");

	/* register hma memory pool */
	ret = hma_pool_register ("ddr0", hma_ddr0.base, hma_ddr0.size);
	if (ret < 0)
		printk (KERN_ERR "hma_pool_register failed\n");

	ret = hma_pool_register ("ddr1", hma_ddr1.base, hma_ddr1.size);
	if (ret < 0)
		printk (KERN_ERR "hma_pool_register failed\n");
}

void	BASE_PLT_Init   (void)
{
	switch(lx_chip_plt())
	{
		case LX_CHIP_PLT_UHD:	printk("<!> H13 UHD (0x%x) platform init..\n",lx_chip_plt());
								BASE_PLT_UHD_Init(); break;
		case LX_CHIP_PLT_FHD:
		default:				printk("<!> H13 FHD (0x%x) platform init..\n",lx_chip_plt());
								BASE_PLT_FHD_Init(); break;
	}
}

static int BASE_PLT_ParseCmdParam( const char* fmt_str, char* val_str )
{
	int		ret = RET_ERROR;
	char*   cmd_buf = NULL;
	int     cmd_buf_sz = 1023;

	char *tok, *sav_tok;
	char delim[] = " \t\n";

	if(NULL == (cmd_buf=OS_Malloc(cmd_buf_sz+1))) {
		printk("<error> can't alloc cmd_buf\n"); goto func_exit;
	}
	#ifdef MODULE  //raxis.lim (2013/09/16) -- kernel exports saved_command_line symbol..
	{
		OS_FILE_T file;
		if( RET_OK != OS_OpenFile( &file, "/proc/cmdline", O_RDONLY|O_LARGEFILE, 0666 ) ) {
			printk("<error> can't open /proc/cmdline\n"); goto func_exit;
		}

		if ( 0 >= OS_ReadFile( &file, cmd_buf, cmd_buf_sz ) ) {
			OS_CloseFile(&file);
			printk("<error> can't read /proc/cmdline\n"); goto func_exit;
		}
		OS_CloseFile(&file);
	}
	#else
	{
#ifdef HWOPT_PARSE_TEST
		printk("hwopt test mode !!!\n");
		cmd_buf_sz = snprintf( cmd_buf, cmd_buf_sz, "%s", test_command_line );
#else
		extern char *saved_command_line;
		cmd_buf_sz = snprintf( cmd_buf, cmd_buf_sz, "%s", saved_command_line );
#endif
	}
	#endif

	tok=simple_strtok(cmd_buf, delim, &sav_tok);
	while(tok)
	{
		if (1==sscanf(tok, fmt_str, val_str)) { printk("+ %s\n", tok); ret=RET_OK; break; }
		tok=simple_strtok(NULL, delim, &sav_tok);
	}
func_exit:
	if (cmd_buf) OS_Free(cmd_buf);
	return ret;
}

/*
 platform probe instruction..

 hwopt=000x0000000000 is passed to kdriver via kernel booting paramter.

 x = 0 : HD (60hz)
 x = 1 : FHD(120hz)
 x = 2 : UHD

*/
UINT32	BASE_PLT_Probe	(void)
{
#if   CONFIG_LX_BOARD_FHD==0 && CONFIG_LX_BOARD_UD==1
	printk("<!> UHD by default.\n");
	return LX_CHIP_PLT_UHD;
#elif CONFIG_LX_BOARD_FHD==1 && CONFIG_LX_BOARD_UD==0
	printk("<!> FHD by default.\n");
	return LX_CHIP_PLT_FHD;
#elif CONFIG_LX_BOARD_FHD==0 && CONFIG_LX_BOARD_UD==0
	printk("<!> unknown. FHD by default.\n");
	return LX_CHIP_PLT_FHD;
#else
	{
		UINT32	chip_plt=LX_CHIP_PLT_FHD;
		char	hw_opt[64];

		if(RET_OK!=BASE_PLT_ParseCmdParam("hwopt=%s",hw_opt)) { printk("hwopt= not found. set FHD mode.\n"); goto func_exit; }

		switch(hw_opt[3]) {
			case '2': case '3':	chip_plt=LX_CHIP_PLT_UHD; break;	/* UHD */
			case '1': default :	chip_plt=LX_CHIP_PLT_FHD; break;	/* FHD */
		}

		switch(hw_opt[14]) {
			case '0': g_grpahic_res = LX_RES_XGA;  break;	/* 1024x768 */
			case '1': g_grpahic_res = LX_RES_WXGA; break;	/* 1366x768 */
			case '3': g_grpahic_res = LX_RES_WFHD; break;	/* 2560x1080 */
			case '4': g_grpahic_res = LX_RES_UHD;  break;	/* 3840x2160 */
			case '5': g_grpahic_res = LX_RES_WUHD; break;	/* 5120x2160 */
			default:
			case '2': g_grpahic_res = LX_RES_FHD;  break;	/* 1920x1080 (default) */
		}
func_exit:
		return chip_plt;
	}
#endif
}

/** return current detected graphic resolution
 *
 */
UINT32 lx_chip_graphic_res(void)
{
	return g_grpahic_res;
}

/** @} */

