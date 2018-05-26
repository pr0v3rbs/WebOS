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
 *  dbg header file for BE device
 *
 *  author		dj911.kim
 *  version		1.0
 *  date		2012.08.01
 *  note		debug print macro.
 *
 *  @addtogroup BE
 *	@{
 */

#ifndef _BE_DBG_H_
#define _BE_DBG_H_

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
#define BE_DBG_PRT_IDX_PRINT	 	LX_LOGM_LEVEL_NOTI
#define BE_DBG_PRT_IDX_ERROR	 	LX_LOGM_LEVEL_ERROR
#define BE_DBG_PRT_IDX_DEBUG	 	LX_LOGM_LEVEL_DEBUG
#define BE_DBG_PRT_IDX_TRACE	 	LX_LOGM_LEVEL_TRACE

#define BE_DBG_PRT_MSK_PRINT	(1UL << BE_DBG_PRT_IDX_PRINT)
#define BE_DBG_PRT_MSK_ERROR	(1UL << BE_DBG_PRT_IDX_ERROR)
#define BE_DBG_PRT_MSK_DEBUG	(1UL << BE_DBG_PRT_IDX_DEBUG)
#define BE_DBG_PRT_MSK_TRACE	(1UL << BE_DBG_PRT_IDX_TRACE)

#define BE_DBG_LEVEL_ERROR	0
#define BE_DBG_LEVEL_WARN	1
#define BE_DBG_LEVEL_IGNORE	2

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define BE_PRINT(format, args...)		DBG_PRINT ( g_be_debug_fd, BE_DBG_PRT_IDX_PRINT, format, ##args )
#define BE_ERROR(format, args...)		DBG_PRINTX ( g_be_debug_fd, BE_DBG_PRT_IDX_ERROR, format, ##args )
#define BE_DEBUG(format, args...)		DBG_PRINT ( g_be_debug_fd, BE_DBG_PRT_IDX_DEBUG, format, ##args )
#define BE_TRACE(format, args...)		DBG_PRINTX( g_be_debug_fd, BE_DBG_PRT_IDX_TRACE, format"\n", ##args )

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/
extern int		g_be_debug_fd;
extern BOOLEAN gHalFuncSupportDbgLevel;

#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* _BE_DRV_H_ */

/** @} */


