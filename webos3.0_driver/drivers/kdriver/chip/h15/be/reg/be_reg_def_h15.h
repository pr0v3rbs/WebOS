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
 *  date		2014.05.21
 *  note		Additional information.
 *
 *  @addtogroup BE
 *	@{
 */

#ifndef _BE_REG_DEF_H15_H_
#define _BE_REG_DEF_H15_H_

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
#define PE_PWM_REG_H15A0_BASE	0xC800B100
#define DPE_LED_REG_H15A0_BASE	0xC8020000

/* for  DRC contorl */
#define PE1_DRC_REG_H15A0_CONTROL_ADDR	0xC8025020

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

#endif /* _BE_REG_DEF_H15_H_ */

/** @} */






