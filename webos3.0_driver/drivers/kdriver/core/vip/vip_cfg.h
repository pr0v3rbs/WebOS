/*
 * SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 * Copyright(c) 2014 by LG Electronics Inc.
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
 * @file
 *
 *  main driver implementation for vision processor device.
 *	vision processor device will teach you how to make device driver with new platform.
 *
 *  author		hyunho kim (hyunho747.kim@lge.com)
 *  version		1.0
 *  date		2014.02.06
 *  note		Additional information.
 *
 *  @addtogroup lg1210_vip
 * @{
 */

#ifndef _VIP_CFG_H_
#define _VIP_CFG_H_

#include "base_types.h"
#include "vip_kapi.h"

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

#define CONFIG_VIP

#ifdef INCLUDE_H15_CHIP_KDRV
	#define __VIP_ENABLED
#else
	#define __VIP_DISABLED
#endif

#ifdef INCLUDE_H15_CHIP_KDRV
	#ifndef USE_KDRV_CODES_FOR_H15
	#define USE_KDRV_CODES_FOR_H15
	#endif
	#ifndef USE_KDRV_CODES_FOR_H15A0
	#define USE_KDRV_CODES_FOR_H15A0
	#endif
	#ifndef INCLUDE_KDRV_VER_FPGA
		#ifndef USE_CTOP_CODES_FOR_H15
		#define USE_CTOP_CODES_FOR_H15
		#endif
	#endif	/* INCLUDE_KDRV_VER_FPGA */
#endif	/* INCLUDE_H15_CHIP_KDRV */

#if defined(KDRV_CONFIG_PM)
#define VIP_KDRV_CONFIG_PM
#endif

#if defined(CONFIG_PM)
#define VIP_CONFIG_PM
#endif

#if !defined(INCLUDE_KDRV_VER_FPGA)
#if defined(USE_KDRV_CODES_FOR_H15)
#define VIP_PM_GATING
#endif
#endif

#if defined(CONFIG_PM_RUNTIME)
#define VIP_CONFIG_PM_RUNTIME
#endif

#ifdef INCLUDE_KDRV_VER_FPGA
#define __VIP_MEMORY_MAP_TYPE		1	//FPGA
#else
#define __VIP_MEMORY_MAP_TYPE		8	//H15A0
#endif

#define __VIP_USE_LOGM_FLAG			0

#define __VIP_USE_INTERRUPT			1

#define	__VIP_USE_BUS_TYPE			0
#define __VIP_USE_PLATFORM_ALLOC	0

#define __VIP_USE_INPUT_DEVICE		0

#define __VIP_USE_INT_ENABLE		0

#define __VIP_BUFFER_ALIGN_TEST		0

#define __VIP_BUFFER_FLUSH_ON		1

#define __VIP_POST_AT_STATE			0


//#define VIP_DBG_PRINT_ON

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/
#if (__VIP_MEMORY_MAP_TYPE == 8)
#define VIP_MEMORY_BASE_H15_A0		0x74400000	//Pioneer
#else
#define VIP_MEMORY_BASE_H15_A0		0x34000000	//FPGA
#endif
#define VIP_MEMORY_SIZE_H15_A0		0x4000000	//64MB

enum {
	VIP_MEM_CFG_IDX_H15_A0 = 0,
	VIP_MEM_CFG_IDX_MAX,
};

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/
extern const LX_MEMCFG_T gMemCfgVisionDSP[];

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif



