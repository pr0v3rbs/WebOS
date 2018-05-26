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
/**
 *	platform dependent configuration header
 *
 */
#ifndef __PLATFORM_CONFIG_H__
#define __PLATFORM_CONFIG_H__

/*----------------------------------------------------------------------------------------
 * ARCH TYPE
 ----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
 * BOARD TYPE
 *
 * CONFIG_LX_BOARD_FHD     -- this board supports FHD (1920x1080) display
 * CONFIG_LX_BOARD_UD      -- this board supports UD  (3840x2160) display
 * CONFIG_LX_BOARD_STBBOX  -- this board supports Settop Box
 ----------------------------------------------------------------------------------------*/
#define CONFIG_LX_BOARD_FHD			1		/* ON  */
#define CONFIG_LX_BOARD_UD			1		/* ON  */
#define CONFIG_LX_BOARD_STBBOX		0		/* OFF */
#define CONFIG_LX_BOARD_FPGA		0		/* ON  */

/*----------------------------------------------------------------------------------------
 * COMMON
 *
 * CONFIG_LX_CHIP_EMUL		-- if none-zero value defined, kdriver will be configured based on this revision
 * CONFIG_LX_BOOT_LOGO		-- kdriver manages lifetime of boot logo
 *
 * CONFIG_LX_LOGM 			-- use new log system(TODO: ...)
 *
 ----------------------------------------------------------------------------------------*/
#define CONFIG_LX_CHIP_EMUL			0		/* 0x17A0 = H15A0 */
#define	CONFIG_LX_BOOT_LOGO			1		/* ON */
#define CONFIG_LX_LOGM 				1 		/* ON */

/*----------------------------------------------------------------------------------------
 * OSD device
 *
 * CONFIG_LX_BOOT_LOGO_FBDEV
 * -- OSD layer for supporting boot logo. this is valid only when CONFIG_LX_BOOT_LOGO is enabled.
 *
 * CONFIG_LX_BUILTIN_FBMEM
 * -- OSD driver supports builtin frame buffer. in general, almost applications require that FBDEVs
 *	allocate their own frame buffer memory and application writes graphic contents to frame buffer.
 *	but some application allocates its own frame buffer and doesn't want FBDev to allocates any memory
 *	to save device memory.
 *
 ----------------------------------------------------------------------------------------*/
#define CONFIG_LX_BOOT_LOGO_FBDEV	0	/* POSD 0 */
#define CONFIG_LX_BUILTIN_FBMEM		0	/* on     */
#define CONFIG_LX_GRAPHIC_RES       LX_MAKE_RES(1920,1080)  /* 0x0: auto detect. call lx_chip_graphic_res() to get resolution */

/*----------------------------------------------------------------------------------------
 * Built-In Firmware Selectoin
 * -- disable inactive chip firmware to decrease kdriver memory size
 ----------------------------------------------------------------------------------------*/
#define CONFIG_LX_H13_CHIP_FIRMWARE 0
#define CONFIG_LX_M14_CHIP_FIRMWARE 0
#define CONFIG_LX_H14_CHIP_FIRMWARE 0
#define CONFIG_LX_H15_CHIP_FIRMWARE 1

/*----------------------------------------------------------------------------------------
 * AUDIO device
 *
 * DISABLE_SCART_HW_BYPASS
 * -- The define of audio SCART HW Bypass Mode to use current platform.
 ----------------------------------------------------------------------------------------*/
//define for SCART HW Bypass Mode
#define DISABLE_SCART_HW_BYPASS		//webOS platform

/*----------------------------------------------------------------------------------------
 * XX device
 *
 ----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 * PLATFORM definition
 ----------------------------------------------------------------------------------------*/


/* include the default (common) configuration file */
#include "../common/platform_config.h"

#endif /* __PLATFORM_CONFIG_H__ */

