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
 *  Dbg header file for OVI device
 *	
 *  author		dj911.kim
 *  version		1.0
 *  date		2012.09.19
 *  note		debug print macro.
 *
 *  @addtogroup OVI
 *	@{
 */

#ifndef _OVI_DBG_H_
#define _OVI_DBG_H_

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
#define OVI_DBG_PRT_IDX_PRINT	 	LX_LOGM_LEVEL_NOTI
#define OVI_DBG_PRT_IDX_ERROR	 	LX_LOGM_LEVEL_ERROR
#define OVI_DBG_PRT_IDX_DEBUG	 	LX_LOGM_LEVEL_DEBUG

#define OVI_DBG_PRT_MSK_PRINT	(1UL << OVI_DBG_PRT_IDX_PRINT)
#define OVI_DBG_PRT_MSK_ERROR	(1UL << OVI_DBG_PRT_IDX_ERROR)
#define OVI_DBG_PRT_MSK_DEBUG	(1UL << OVI_DBG_PRT_IDX_DEBUG)

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define OVI_PRINT(format, args...)		DBG_PRINT ( g_ovi_debug_fd, OVI_DBG_PRT_IDX_PRINT, format, ##args )
#define OVI_ERROR(format, args...)		DBG_PRINTX ( g_ovi_debug_fd, OVI_DBG_PRT_IDX_ERROR, format, ##args )
#define OVI_DEBUG(format, args...)	DBG_PRINT ( g_ovi_debug_fd, OVI_DBG_PRT_IDX_DEBUG, format, ##args )

#define OVI_CHECK_NULL(ptr)  if(!ptr) { OVI_ERROR("%s is null\n", #ptr); ret = RET_ERROR; break; }
#define OVI_BREAK_WRONG(val) { OVI_ERROR("break wrong %s(0x%x)\n", #val, val); ret = RET_ERROR; break; }

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/
extern int		g_ovi_debug_fd;

#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* _OVI_DRV_H_ */

/** @} */


