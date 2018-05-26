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
 *  main configuration header file for ovi device
 *	
 *  author		dj911.kim
 *  version		1.0
 *  date		2012.10.08
 *  note		Additional information.
 *
 *  @addtogroup ovi
 *	@{
 */

#ifndef _OVI_CFG_H_
#define _OVI_CFG_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/
#define OVI_MODULE		"ovi"
#define OVI_MAX_DEVICE	1

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include "base_types.h"

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
void OVI_InitCfg ( void );

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/
extern LX_MEMCFG_T gMemCfgOviTcon;

#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* _OVI_CFG_H_ */

/** @} */



