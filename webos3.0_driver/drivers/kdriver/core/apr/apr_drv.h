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
 *  drv header file for APR device
 *	
 *  author		justine.jeong
 *  version		1.0
 *  date		2012.07.06
 *  note		Additional information.
 *
 *  @addtogroup APR
 *	@{
 */

#ifndef _APR_DRV_H_
#define _APR_DRV_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include "debug_util.h"

#ifdef  __cplusplus
extern "C"
{
#endif /* __cplusplus */
	
/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/
extern void	APR_PreInit(void);
extern int	APR_Init(void);
extern void	APR_Cleanup(void);

extern int  APR_Initialize(unsigned long arg);
extern int  APR_SetControl(unsigned long arg);
extern int  APR_GetInfo(unsigned long arg);
extern int  APR_SetCommand(unsigned long arg);

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* _APR_DRV_H_ */

/** @} */



