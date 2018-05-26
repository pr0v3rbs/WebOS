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
 * date       2011.04.06
 * note       Additional information.
 *
 * @addtogroup lg1152_de
 * @{
 */

#ifndef  DE_CFG_DEF_H14_INC
#define  DE_CFG_DEF_H14_INC

/*----------------------------------------------------------------------------------------
 *   Control Constants
 *---------------------------------------------------------------------------------------*/
#define PIXEL_PER_BANK	   4096

/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Constant Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Macro Definitions
 *---------------------------------------------------------------------------------------*/
#define VIDEO_MEM_FIRMWARE_ROW_SIZE          ((64*1024)/(PIXEL_PER_BANK))
#define VIDEO_MEM_ROW_STRIDE                 ((PIXEL_PER_BANK/128) * 1024)
#define CONV_MEM_ROW2BYTE(_r)                (((_r) * VIDEO_MEM_ROW_STRIDE))
#define CONV_MEM_BYTE2ROW(_r)				 (((_r) + VIDEO_MEM_ROW_STRIDE-1) / VIDEO_MEM_ROW_STRIDE)

/*----------------------------------------------------------------------------------------
 *   Type Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   External Function Prototype Declarations
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   External Variables
 *---------------------------------------------------------------------------------------*/


#endif   /* ----- #ifndef DE_CFG_DEF_H14_INC  ----- */
/**  @} */
