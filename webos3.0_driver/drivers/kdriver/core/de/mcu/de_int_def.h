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
 * date       2010.03.30
 * note       Additional information.
 *
 * @addtogroup lg1150_de
 * @{
 */

#ifndef  DE_INT_DEF_INC
#define  DE_INT_DEF_INC

/*----------------------------------------------------------------------------------------
 *   Control Constants
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Constant Definitions
 *---------------------------------------------------------------------------------------*/
#ifndef L8_IRQ_DE
#define L8_IRQ_DE		75
#define L8_IRQ_DE_IPC	 76
#endif
#ifndef L9_IRQ_DE_BCPU
#define L9_IRQ_DE_BCPU	IRQ_DE_BCPU
#define L9_IRQ_IPC_BCPU	IRQ_IPC_BCPU
#endif

/*----------------------------------------------------------------------------------------
 *   Macro Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Type Definitions
 *---------------------------------------------------------------------------------------*/
typedef enum {
	VIDEO_INTR_TYPE_DE0A ,
	VIDEO_INTR_TYPE_DE0B ,
	VIDEO_INTR_TYPE_DE1A ,
	VIDEO_INTR_TYPE_DE1B ,
	VIDEO_INTR_TYPE_FUNC ,
	VIDEO_INTR_TYPE_MAX
} VIDEO_INTR_TYPE_T;

/*----------------------------------------------------------------------------------------
 *   External Function Prototype Declarations
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   External Variables
 *---------------------------------------------------------------------------------------*/

#endif   /* ----- #ifndef DE_INT_DEF_INC  ----- */
/**  @} */
