/*
 * SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 * Copyright(c) 2013 by LG Electronics Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */


/** @file
 *
 *  driver interface header for sys device. ( used only within kdriver )
 *	sys device will teach you how to make device driver with new platform.
 *
 *  @author		ks.hyun (ks.hyun@lge.com)
 *  @version	1.0
 *  @date		2010.12.13
 *
 *  @addtogroup lg1150_sys
 *	@{
 */

#ifndef	_SYS_DRV_H_
#define	_SYS_DRV_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include "debug_util.h"
#include "sys_cfg.h"
#include "sys_kapi.h"

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define SYS_LOG_LEVEL_ERROR				LX_LOGM_LEVEL_ERROR
#define SYS_LOG_LEVEL_WARN				LX_LOGM_LEVEL_WARNING
#define SYS_LOG_LEVEL_DEBUG				LX_LOGM_LEVEL_DEBUG
#define SYS_LOG_LEVEL_TRACE				LX_LOGM_LEVEL_TRACE
#define SYS_LOG_LEVEL_PRINT				(LX_LOGM_LEVEL_TRACE + 1)

#define	SYS_PRINT(format, args...)		DBG_PRINT(  g_sys_debug_fd, SYS_LOG_LEVEL_PRINT, format, ##args)
#define	SYS_TRACE(format, args...)		DBG_PRINTX( g_sys_debug_fd, SYS_LOG_LEVEL_TRACE, format, ##args)
#define	SYS_DEBUG(format, args...)		DBG_PRINT( g_sys_debug_fd, SYS_LOG_LEVEL_DEBUG, format, ##args)
#define	SYS_ERROR(format, args...)		DBG_PRINT( g_sys_debug_fd, SYS_LOG_LEVEL_ERROR, format, ##args)

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/
extern	void     SYS_PreInit(void);
extern	int      SYS_Init(void);
extern	void     SYS_Cleanup(void);

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/
extern	int		g_sys_debug_fd;

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _SYS_DRV_H_ */

/** @} */
