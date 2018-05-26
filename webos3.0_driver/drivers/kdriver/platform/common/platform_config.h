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

/** platform dependent configuration header
 *	this file contains the default values ( common configuration ) for all platform
 *
 *	<note> please define the default config value here if you add new config variable !!
 *
 */
#ifndef __COMMON_PLATFORM_CONFIG_H__
#define __COMMON_PLATFORM_CONFIG_H__

/*----------------------------------------------------------------------------------------
 * ARCH
 ----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
 * BOARD
 ----------------------------------------------------------------------------------------*/

/*
 * CONFIG_LX_BOARD_FHD     -- this board supports FHD (1920x1080) display
 * CONFIG_LX_BOARD_UD      -- this board supports UD  (3840x2160) display
 * CONFIG_LX_BOARD_STBBOX  -- this board supports Settop Box
 * CONFIG_LX_BOARD_FPGA    -- this board supports FPGA environment
 */
#ifndef CONFIG_LX_BOARD_FHD
#define CONFIG_LX_BOARD_FHD			0	/* off */
#endif

#ifndef CONFIG_LX_BOARD_UD
#define CONFIG_LX_BOARD_UD			0	/* off */
#endif

#ifndef CONFIG_LX_BOARD_STBBOX
#define CONFIG_LX_BOARD_STBBOX		0	/* off */
#endif

#ifndef CONFIG_LX_BOARD_FPGA
#define CONFIG_LX_BOARD_FPGA		0	/* off */
#endif

/*----------------------------------------------------------------------------------------
 * GENERAL
 ----------------------------------------------------------------------------------------*/

/*
 * CONFIG_LX_BOOT_LOGO
 *	if non-zero, kdriver should support post processing of boot logo display by bootloader.
 *	kdriver should control the lifetime of boot logo.
 *	default = ON (all platforms support boot logo)
 *
 * CONFIG_LX_LOGM
 *	if non-zero, bsp framework uses new log system (logm).
 *	default = ON
 */
#ifndef CONFIG_LX_BOOT_LOGO
#define	CONFIG_LX_BOOT_LOGO			1	/* on */
#endif

#ifndef CONFIG_LX_LOGM
#define CONFIG_LX_LOGM 				1	/* on */
#endif

/*----------------------------------------------------------------------------------------
 * SOC COMMON
 ----------------------------------------------------------------------------------------*/
/*
 * CONFIG_LX_<chip>_CHIP_FIRMWARE
 *	removes inactive chip firmware to save kdriver memory size during compile time.
 *	by defualt, all firmwares are used
 */
#ifndef CONFIG_LX_H13_CHIP_FIRMWARE
#define CONFIG_LX_H13_CHIP_FIRMWARE 1	/* on */
#endif

#ifndef CONFIG_LX_M14_CHIP_FIRMWARE
#define CONFIG_LX_M14_CHIP_FIRMWARE 1	/* on */
#endif

#ifndef CONFIG_LX_H14_CHIP_FIRMWARE
#define CONFIG_LX_H14_CHIP_FIRMWARE 1	/* on */
#endif

/*----------------------------------------------------------------------------------------
 * EXPERIMENTAL
 ----------------------------------------------------------------------------------------*/

/* OBSOLETED. DO NOT USE */
#ifndef CONFIG_LX_CHIP_EMUL
#define CONFIG_LX_CHIP_EMUL			0	/* off */
#endif

/*----------------------------------------------------------------------------------------
 * DEVICE
 ----------------------------------------------------------------------------------------*/

/* <OSD Device>
 *
 * CONFIG_LX_BOOT_LOGO_FBDEV
 * 	this value defines specific OSD layer for boot logo display.
 *	this is valid only when CONFIG_LX_BOOT_LOGO is enabled.
 *	bootloader and kdrv should be set to the same value.
 *	default = POSD0
 *
 * CONFIG_LX_BUILTIN_FBMEM
 *	OSD driver supports builtin frame buffer for supporting linux frame buffer.
 *	In general, almost applications require that FBDEVs allocate their own frame buffer memory
 *	and application writes graphic contents to frame buffer.
 *	but some application (platform) allocates its own frame buffer and doesn't need built-in
 *	frame buffer to save memory.
 *	default = ON
 *
 * CONFIG_LX_GRAPHIC_RES
 *  maximum size supported by graphic operation.
 *  all graphic related module is configured based on this value
 *  if this value is zero, kdriver should call UINT32 lx_chip_graphics_res(void)
 *  refer to LX_MAKE_RES macro in base_types.h
 *
 * CONFIG_LX_FB[n]_[WIDTH|HEIGHT]
 * 	each platform requires its own OSD resolution.
 *	By default, each OSD resolution is set to FHD(1920x1080)
 *
 */
#ifndef CONFIG_LX_BOOT_LOGO_FBDEV
#define CONFIG_LX_BOOT_LOGO_FBDEV	0		/* POSD 0 */
#endif

#ifndef CONFIG_LX_BUILTIN_FBMEM
#define CONFIG_LX_BUILTIN_FBMEM		1		/* ON */
#endif

#ifndef CONFIG_LX_GRAPHIC_RES
#define CONFIG_LX_GRAPHIC_RES		LX_MAKE_RES(1920,1080)	/* default : FHD */
#endif

#ifndef CONFIG_LX_FB0_WIDTH
#define CONFIG_LX_FB0_WIDTH			0		/* auto detect. set by CONFIG_LX_GRAPHIC_RES */
#endif
#ifndef CONFIG_LX_FB0_HEIGHT
#define CONFIG_LX_FB0_HEIGHT		0		/* auto detect. set by CONFIG_LX_GRAPHIC_RES */
#endif
#ifndef CONFIG_LX_FB1_WIDTH
#define CONFIG_LX_FB1_WIDTH			0		/* auto detect. set by CONFIG_LX_GRAPHIC_RES */
#endif

#ifndef CONFIG_LX_FB1_HEIGHT
#define CONFIG_LX_FB1_HEIGHT		0		/* auto detect. set by CONFIG_LX_GRAPHIC_RES */
#endif

/*
 * <GFX Device>
 *
 */
#ifndef CONFIG_LX_GFX_MEM_RGN_NUM
#define CONFIG_LX_GFX_MEM_RGN_NUM   1
#endif

/*
 * <XXXX Device>
 *
 */


#endif /* __COMMON_PLATFORM_CONFIG_H__ */

