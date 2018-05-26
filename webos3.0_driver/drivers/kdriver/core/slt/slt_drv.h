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
 *  driver interface header for slt device. ( used only within kdriver )
 *	slt device will teach you how to make device driver with new platform.
 *
 *  @author		wunseo.yeo@lge.com
 *  @version	1.0
 *  @date		2014.08
 *
 *  @addtogroup lg115x_slt
 *	@{
 */

#ifndef	_SLT_DRV_H_
#define	_SLT_DRV_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include "debug_util.h"
#include "slt_cfg.h"
#include "slt_kapi.h"

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
//#define MODULE_NAME     "SLT"

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define SLT_LOG_LEVEL_ERROR			LX_LOGM_LEVEL_ERROR
#define SLT_LOG_LEVEL_WARN			LX_LOGM_LEVEL_WARNING
#define SLT_LOG_LEVEL_NOTI			LX_LOGM_LEVEL_NOTI
#define SLT_LOG_LEVEL_INFO			LX_LOGM_LEVEL_INFO
#define SLT_LOG_LEVEL_DEBUG			LX_LOGM_LEVEL_DEBUG
#define SLT_LOG_LEVEL_TRACE			LX_LOGM_LEVEL_TRACE
#define SLT_LOG_LEVEL_PRINT			(LX_LOGM_LEVEL_TRACE + 1)

#define	SLT_ERROR(format, args...)	DBG_PRINT(g_slt_debug_fd, SLT_LOG_LEVEL_ERROR, format, ##args)
#define	SLT_WARN(format, args...)	DBG_PRINT(g_slt_debug_fd, SLT_LOG_LEVEL_WARN, format, ##args)
#define	SLT_NOTI(format, args...)	DBG_PRINT(g_slt_debug_fd, SLT_LOG_LEVEL_NOTI, format, ##args)
#define	SLT_INFO(format, args...)	DBG_PRINT(g_slt_debug_fd, SLT_LOG_LEVEL_INFO, format, ##args)
#define	SLT_DEBUG(format, args...)	DBG_PRINT(g_slt_debug_fd, SLT_LOG_LEVEL_DEBUG, format, ##args)
#define	SLT_TRACE(format, args...)	DBG_PRINT(g_slt_debug_fd, SLT_LOG_LEVEL_TRACE, format, ##args)
#define	SLT_PRINT(format, args...)	DBG_PRINT(g_slt_debug_fd, SLT_LOG_LEVEL_PRINT, format, ##args)


#define SLT_CHECK_CODE(__checker,__if_action,fmt,args...)   \
             __CHECK_IF_ERROR(__checker, SLT_ERROR, __if_action , fmt, ##args )

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/
void  SLT_PreInit(void);
int   SLT_Init(void);
void  SLT_Cleanup(void);

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/
extern	int		g_slt_debug_fd;

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _SLT_DRV_H_ */

/** @} */
