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
 *  Header file of defining possible of output modes of H14Ax 	
 *
 *  author      won.hur@lge.com
 *  version		1.0
 *  date
 *  note		Additional information.
 *
 *  @addtogroup lg115x_ovi
 *	@{
 */
 
/*----------------------------------------------------------------------------------------
 *   Control Constants
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/
#include "ovi_kapi.h"

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
static LX_OVI_REG_T ovi_h14a0_detour_odc_with_odc_black[] =
{
	{0xC0040018, 0x00000001},
	{0xFD30015C, 0x00100000},
	{0xC002B054, 0xA0000000},
	{0xC0040004, 0x000000D1},
	{0xC004001C, 0x00000018},
};

static LX_OVI_REG_T ovi_h14a0_restore_odc_from_black_detour[] =
{
	{0xC0040018, 0x00000000},
	{0xFD30015C, 0x00000000},
	{0xC002B054, 0x81000000},
	{0xC0040004, 0x000000C0},
	{0xC004001C, 0x00000000},
};



/**  @} */

