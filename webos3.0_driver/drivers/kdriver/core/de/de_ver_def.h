/*
	SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
	Copyright(c) 2013 by LG Electronics Inc.

	This program is free software; you can redistribute it and/or 
	modify it under the terms of the GNU General Public License
	version 2 as published by the Free Software Foundation.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of 
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
	GNU General Public License for more details.
*/ 


/** @file
 *
 * main driver implementation for de device.
 * de device will teach you how to make device driver with new platform.
 *
 * author     jaemo.kim (jaemo.kim@lge.com)
 * version    1.0
 * date       2011.02.21
 * note       Additional information.
 *
 * @addtogroup lg1152_de
 * @{
 */

#ifndef  DE_VER_INC
#define  DE_VER_INC

/*----------------------------------------------------------------------------------------
 *   Control Constants
 *---------------------------------------------------------------------------------------*/
#ifdef INCLUDE_H15_CHIP_KDRV
#define USE_KDRV_CODES_FOR_H15
#define _H15_FPGA_TEMPORAL_
#define USE_KDRV_CODES_FOR_H15A0
#ifndef INCLUDE_KDRV_VER_FPGA
#define USE_CTOP_CODES_FOR_H15
#endif
#endif

#ifdef INCLUDE_M14_CHIP_KDRV
#define USE_KDRV_CODES_FOR_M14
#define USE_KDRV_CODES_FOR_M14A0
#define USE_KDRV_CODES_FOR_M14B0
#define USE_KDRV_CODES_FOR_M14B2
#define USE_KDRV_CODES_FOR_M14C0
#ifndef INCLUDE_KDRV_VER_FPGA
#define USE_CTOP_CODES_FOR_M14
#endif
#endif

#ifdef INCLUDE_H14_CHIP_KDRV
#define USE_KDRV_CODES_FOR_H14
#define USE_KDRV_CODES_FOR_H14A0
#define USE_CTOP_CODES_FOR_H14
#endif

#ifdef INCLUDE_H13_CHIP_KDRV
#define USE_KDRV_CODES_FOR_H13
#define USE_KDRV_CODES_FOR_H13A0
#define USE_KDRV_CODES_FOR_H13B0
#ifndef INCLUDE_KDRV_VER_FPGA
#define USE_CTOP_CODES_FOR_H13
#endif
#endif

#ifdef INCLUDE_L9_CHIP_KDRV
//#define USE_KDRV_CODES_FOR_L9
#define CONFIG_LX_L9_CHIP_FIRMWARE 0
#ifdef  KDRV_GLOBAL_LINK
#endif
#endif

#define USE_PARM_CODES_FOR_L9
#define USE_CTOP_CODES_FOR_L9
#define USE_KDRV_CODES_FOR_CSC_n

/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/
#include "base_types.h"

/*----------------------------------------------------------------------------------------
 *   Constant Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Macro Definitions
 *---------------------------------------------------------------------------------------*/
#ifndef LX_CHIP_REV
#define LX_CHIP_L8     0x08
#define LX_CHIP_L9     0x09
#define LX_CHIP_H13    0x0D
#define lx_chip_rev() 0x09A0
#define MAKE_CHIP_REV(chp, rev) (( chp << 8) | ( rev ))
#define LX_CHIP_REV(chp, rev) MAKE_CHIP_REV( LX_CHIP_##chp, 0x##rev )
#endif

/*----------------------------------------------------------------------------------------
 *   Type Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   External Function Prototype Declarations
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   External Variables
 *---------------------------------------------------------------------------------------*/

#endif   /* ----- #ifndef DE_VER_INC  ----- */
/**  @} */
