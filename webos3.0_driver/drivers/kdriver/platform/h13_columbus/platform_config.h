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
 * CONFIG_LX_BOARD_FHD     -- this board supports FHD
 * CONFIG_LX_BOARD_UD      -- this board supports UHD
 ----------------------------------------------------------------------------------------*/
#define CONFIG_LX_BOARD_FHD			1		/* ON  */
#define CONFIG_LX_BOARD_UD			1		/* ON  */

/*----------------------------------------------------------------------------------------
 * COMMON
 *
 * CONFIG_LX_BOOT_LOGO		-- kdriver manages lifetime of boot logo
 * CONFIG_LX_LOGM 			-- use new log system(TODO: ...)
 ----------------------------------------------------------------------------------------*/
#define	CONFIG_LX_BOOT_LOGO			1		/* ON */
#define CONFIG_LX_LOGM 				1 		/* OFF */

/*----------------------------------------------------------------------------------------
 * OSD device
 *
 * CONFIG_LX_BOOT_LOGO_FBDEV is set to POSD0
 * CONFIG_LX_BUILTIN_FBMEM is set to FALSE
 * CONFIG_LX_GRAPHIC_RES is set 0 to run auto detect.
 ----------------------------------------------------------------------------------------*/
#define CONFIG_LX_BOOT_LOGO_FBDEV	0		/* POSD 0 */
#define CONFIG_LX_BUILTIN_FBMEM		0		/* off    */
#define CONFIG_LX_GRAPHIC_RES		LX_MAKE_RES(0,0)	/* 0x0: auto detect. call lx_chip_graphic_res() to get resolution */

/*----------------------------------------------------------------------------------------
 * GFX device
 ----------------------------------------------------------------------------------------*/
#define CONFIG_LX_GFX_MEM_RGN_NUM	2	/* FHD: 1, UHD: 2 */

/*----------------------------------------------------------------------------------------
 * Built-In Firmware Selectoin
 ----------------------------------------------------------------------------------------*/
#define CONFIG_LX_H13_CHIP_FIRMWARE	1
#define CONFIG_LX_M14_CHIP_FIRMWARE	0
#define CONFIG_LX_H14_CHIP_FIRMWARE	0

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
 ----------------------------------------------------------------------------------------*/

/* include the default (common) configuration file */
#include "../common/platform_config.h"

#endif /* __PLATFORM_CONFIG_H__ */

