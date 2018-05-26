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
 * main driver implementation for hdmi device.
 * hdmi device will teach you how to make device driver with new platform.
 *
 * author     sh.myoung (sh,myoung@lge.com)
 * version    1.0
 * date       2011.02.21
 * note       Additional information.
 *
 * @addtogroup lg1152_de
 * @{
 */

#ifndef  HDMI_VER_INC
#define  HDMI_VER_INC

/*----------------------------------------------------------------------------------------
 *   Control Constants
 *---------------------------------------------------------------------------------------*/
#ifdef INCLUDE_H13_CHIP_KDRV
#define USE_HDMI_KDRV_FOR_H13
#endif

#ifdef INCLUDE_H14_CHIP_KDRV
#define USE_HDMI_KDRV_FOR_H14
#endif

#ifdef INCLUDE_M14_CHIP_KDRV
#define USE_HDMI_KDRV_FOR_M14
#endif

#ifdef INCLUDE_H15_CHIP_KDRV
#define USE_HDMI_KDRV_FOR_H15
#endif
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

/*----------------------------------------------------------------------------------------
 *   Type Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   External Function Prototype Declarations
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   External Variables
 *---------------------------------------------------------------------------------------*/

#endif   /* ----- #ifndef HDMI_VER_INC  ----- */
/**  @} */
