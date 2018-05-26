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
 *  BE reg def header file for BE device
 *	
 *  author		dj911.kim
 *  version		1.0
 *  date		2013.04.03
 *  note		Additional information.
 *
 *  @addtogroup BE
 *	@{
 */

#ifndef _BE_REG_DEF_H_
#define _BE_REG_DEF_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/* Comment from won.hur (2012.05.16)
 * CPU관점 메모리 주소 + 0xC002_0000 => MCU관점 메모리 주소 */
#define FRC_LRX_REG_M14A0_BASE	0xC002AE00
#define FRC_P3D_REG_M14A0_BASE	0xC002AC00
#define FRC_DVI_REG_M14A0_BASE	0xC002A000
#define FRC_DVO_REG_M14A0_BASE	0xC002B000
#define DPPB_TOP_REG_M14A0_BASE	0xC0024000
#define DPPB_TXM_REG_M14A0_BASE	0xC0034000
#define DPPB_LED_REG_M14A0_BASE	0xC0030000

#define FRC_DVO_REG_M14B0_BASE	0xC002E000
#define DPPB_LED_REG_M14B0_BASE	0xC0040000

#define DPPB_TOP_IPC_M14A0_OFFSET 0x00000080
#define DPPB_TOP_IPC_M14B0_OFFSET 0x00000080

/* for  HDR contorl */
#define PE1_HDR_REG_M14B0_CONTROL_ADDR	0xC00248c0

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

#endif /* _BE_REG_DEF_H_ */

/** @} */






