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
 *  main configuration file for fbdev device
 *	fbdev device will teach you how to make device driver with new platform.
 *
 *  author		raxis
 *  version		1.0
 *  date		2010.02.01
 *  note		Additional information.
 *
 *  @addtogroup lg1150_fbdev
 *	@{
 */

#ifndef	_FBDEV_CFG_H_
#define	_FBDEV_CFG_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
#define	FBDEV_MODULE				"fbdev"
#define FBDEV_MAX_DEVICE			4

/* Common Configuration
 *
 */
#undef	FBDEV_USE_DEFAULT_PALETTE 	/* if some default palette needed, define FBDEV_USE_DEFAULT_PALETTE */
#undef	FBDEV_DEBUG_VSYNC			/* when enabled, vsync timoeut is printed */
#undef	FBDEV_FLIP_LOCK				/* when enabled, flipping lock is enabled. low performance for multiple POSD update ? */
#define	FBDEV_USE_MIXER_INTR		/* when enabled, only MIXER intr is enabled, all other OSD intr is disabled */
#define	FBDEV_SUPPORT_8BPP_EMUL		/* OSD support 8bpp emulation on 32bpp POSD */

#define FBDEV_SUPPORT_OSD_BKUP		/* save/restore OSD plane during suspend/resume */
#define FBDEV_SUPPORT_OSD_CLEAR_ON_BKUP /* clear OSD plane during OSD backup */
#define FBDEV_SUPPORT_CRSR_BKUP		/* save/restore cursor image during suspend/resume */

#define	FBDEV_USE_FAKE_FBMEM		/* make fake frame buffer if OSD doesn't have any built-in frame buffer */

#define FBDEV_USE_FBMEM_BKUP		/* backup/restore osd framebuffer during suspend/resume */

/* L9 only Configuration
 *
 */
#undef	FBDEV_GP_USE_16BIT_CURSOR	/* when enabled, GP mode will use 16bit cursor */
#define	FBDEV_USE_MMIO_REG			/* when enabled, FB device uses memory based IO. when disabled, FB device uses register based IO
									 * DO NOT modify this macro. register based IO is not working now
									 */
#undef	FBDEV_USE_BLEND_MEMC_SW_FIX	/* when enabled, incorrect MEMC output is fixed by software. this workaround is chip specific */

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/
typedef struct
{
	char*			mem_frame_name;
	unsigned int	mem_frame_base;
	unsigned int	mem_frame_size;			/* osd frame buffer size */

	char*			mem_palette_name;
	unsigned int	mem_palette_base;
	unsigned int	mem_palette_size;		/* osd palette size */
}
LX_FBDEV_DEV_MEM_CFG_T;

typedef struct
{
	char*			mem_header_name;
	unsigned int	mem_header_base;
	unsigned int	mem_header_size;		/* osd header size */
}
LX_FBDEV_HDR_MEM_CFG_T;

extern LX_FBDEV_DEV_MEM_CFG_T 		gMemCfgFBDev[4];	/* memcfg for FB device memory */
extern LX_FBDEV_HDR_MEM_CFG_T		gMemCfgFBHdr[4];	/* memcfg for FB header memory */

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _FBDEV_CFG_H_ */

/** @} */

