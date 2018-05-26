/*
SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
Copyright(c) 1998-2013 by LG Electronics Inc.
 
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
 *  driver interface header for mjpeg device. ( used only within kdriver )
 *	MJPEG device will teach you how to make device driver with new platform.
 *
 *  @author		youngwoo.jin (youngwoo.jin@lge.com)
 *  @version	1.0
 *  @date		2011.04.09
 *
 *  @addtogroup lg1152_mjpeg
 *	@{
 */

#ifndef	_MJPEG_DRV_H_
#define	_MJPEG_DRV_H_

/*-----------------------------------------------------------------------------
	Control Constants
-----------------------------------------------------------------------------*/
#define MJPEG_USE_ENCODE_QUALITY_PARAM
//#define MJPEG_DEBUG_REG_DUMP
//#define MJPEG_DEBUG_TRACE_DUMP
//#define MJPEG_DEBUG_INPUT_DUMP

#if defined(MJPEG_DEBUG_INPUT_DUMP) || defined(MJPEG_DEBUG_REG_DUMP) || defined(MJPEG_DEBUG_TRACE_DUMP)
#define MJPEG_USE_INTERNAL_LOCK
#else
#undef  MJPEG_USE_INTERNAL_LOCK
#endif

/*-----------------------------------------------------------------------------
    File Inclusions
-----------------------------------------------------------------------------*/
#include "debug_util.h"
#include "mjpeg_cfg.h"
#include "mjpeg_kapi.h"
#include "mjpeg_reg_l9.h"

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/*-----------------------------------------------------------------------------
	Constant Definitions
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	Macro Definitions
-----------------------------------------------------------------------------*/
#define	MJPEG_ERROR(format, args...)		DBG_PRINT( g_MJPEG_debug_fd, LX_LOGM_LEVEL_ERROR, format, ##args)
#define	MJPEG_WARN(format, args...)			DBG_PRINT( g_MJPEG_debug_fd, LX_LOGM_LEVEL_WARNING, format, ##args)
#define	MJPEG_NOTI(format, args...)			DBG_PRINT( g_MJPEG_debug_fd, LX_LOGM_LEVEL_NOTI, format, ##args)
#define	MJPEG_INFO(format, args...)			DBG_PRINT( g_MJPEG_debug_fd, LX_LOGM_LEVEL_INFO, format, ##args)
#define	MJPEG_DEBUG(format, args...)		DBG_PRINT( g_MJPEG_debug_fd, LX_LOGM_LEVEL_DEBUG, format, ##args)
#define	MJPEG_TRACE(format, args...)		DBG_PRINT( g_MJPEG_debug_fd, LX_LOGM_LEVEL_TRACE, format, ##args)
#define	MJPEG_PRINT(format, args...)		DBG_PRINT( g_MJPEG_debug_fd, LX_LOGM_LEVEL_TRACE + 1, format, ##args)

/*-----------------------------------------------------------------------------
    Type Definitions
-----------------------------------------------------------------------------*/
typedef enum {
	LX_MJPEG_PARTIAL_BUFFER0 = 0,
	LX_MJPEG_PARTIAL_BUFFER1 = 1,
	LX_MJPEG_PARTIAL_BUFFER2 = 2,
	LX_MJPEG_PARTIAL_BUFFER3 = 3,
} LX_MJPEG_PARTIAL_BUFFER_T;

/*-----------------------------------------------------------------------------
	Extern Function Prototype Declaration
-----------------------------------------------------------------------------*/
extern 	void 	 MJPEG_PreInit(void);
extern	int      MJPEG_Init(void);
extern	void     MJPEG_Cleanup(void);

/*-----------------------------------------------------------------------------
	Extern Variables
-----------------------------------------------------------------------------*/
extern	int						g_MJPEG_debug_fd;

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _MJPEG_DRV_H_ */

/** @} */
