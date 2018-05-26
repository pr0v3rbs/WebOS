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
 * BOARD TYPE
 *
 * CONFIG_LX_BOARD_FHD     -- this board supports FHD (1920x1080) display
 * CONFIG_LX_BOARD_UD      -- this board supports UD  (3840x2160) display
 * CONFIG_LX_BOARD_STBBOX  -- this board supports Settop Box
 ----------------------------------------------------------------------------------------*/
#define CONFIG_LX_BOARD_FHD			1		/* ON  */
#define CONFIG_LX_BOARD_UD			0		/* OFF */
#define CONFIG_LX_BOARD_STBBOX		0		/* OFF */

#define CONFIG_LX_BOARD_FPGA		1		/* ON  */

/*----------------------------------------------------------------------------------------
 * COMMON
 *
 * CONFIG_LX_BOOT_LOGO     -- kdriver manages lifetime of boot logo
 *
 ----------------------------------------------------------------------------------------*/
#define	CONFIG_LX_BOOT_LOGO			0		/* ON */

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
#define CONFIG_LX_BOOT_LOGO_FBDEV	0		/* POSD 0 */
#define CONFIG_LX_BUILTIN_FBMEM		1		/* ON     */

/*----------------------------------------------------------------------------------------
 * Built-In Firmware Selectoin
 * -- disable inactive chip firmware to decrease kdriver memory size
 ----------------------------------------------------------------------------------------*/
#define CONFIG_LX_H13_CHIP_FIRMWARE 0
#define CONFIG_LX_M14_CHIP_FIRMWARE 0
#define CONFIG_LX_H14_CHIP_FIRMWARE 1

/*----------------------------------------------------------------------------------------
 * AUDIO device
 *
 * DISABLE_SCART_HW_BYPASS
 * -- The define of audio SCART HW Bypass Mode to use current platform.
 ----------------------------------------------------------------------------------------*/
//define for SCART HW Bypass Mode
#undef DISABLE_SCART_HW_BYPASS	//PIONEER platform
//#define DISABLE_SCART_HW_BYPASS	//test only

/*----------------------------------------------------------------------------------------
 * XX device
 *
 ----------------------------------------------------------------------------------------*/


/* include the default (common) configuration file */
#include "../common/platform_config.h"

#endif /* __PLATFORM_CONFIG_H__ */

