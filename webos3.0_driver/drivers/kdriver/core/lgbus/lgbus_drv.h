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
 *  driver interface header for lgbus device. ( used only within kdriver )
 *	lgbus device will teach you how to make device driver with new platform.
 *
 *  @author		hwanwook.lee (hwanwook.lee@lge.com)
 *  @version	1.0
 *  @date		2012.07.14
 *
 *  @addtogroup lg115x_lgbus
 *	@{
 */

#ifndef	_LGBUS_DRV_H_
#define	_LGBUS_DRV_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include "debug_util.h"
#include "lgbus_cfg.h"
#include "lgbus_kapi.h"

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
#define MODULE_NAME     "LGBUS"

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define LGBUS_LOG_LEVEL_ERROR			LX_LOGM_LEVEL_ERROR
#define LGBUS_LOG_LEVEL_WARN			LX_LOGM_LEVEL_WARNING
#define LGBUS_LOG_LEVEL_INFO			LX_LOGM_LEVEL_INFO
#define LGBUS_LOG_LEVEL_DEBUG			LX_LOGM_LEVEL_DEBUG
#define LGBUS_LOG_LEVEL_TRACE			LX_LOGM_LEVEL_TRACE
#define LGBUS_LOG_LEVEL_PRINT			(LX_LOGM_LEVEL_TRACE + 1)


#define	LGBUS_PRINT(format, args...)	DBG_PRINT(g_lgbus_debug_fd, LGBUS_LOG_LEVEL_PRINT, format, ##args)
#define	LGBUS_TRACE(format, args...)	DBG_PRINT(g_lgbus_debug_fd, LGBUS_LOG_LEVEL_TRACE, format, ##args)
#define	LGBUS_DEBUG(format, args...)	DBG_PRINT(g_lgbus_debug_fd, LGBUS_LOG_LEVEL_DEBUG, format, ##args)
#define	LGBUS_ERROR(format, args...)	DBG_PRINT(g_lgbus_debug_fd, LGBUS_LOG_LEVEL_ERROR, format, ##args)
#define	LGBUS_INFO(format, args...)		DBG_PRINT(g_lgbus_debug_fd, LGBUS_LOG_LEVEL_INFO, format, ##args)



#define LGBUS_CHECK_CODE(__checker,__if_action,fmt,args...)   \
             __CHECK_IF_ERROR(__checker, LGBUS_ERROR, __if_action , fmt, ##args )

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/

extern	void     LGBUS_PreInit(void);
extern	int      LGBUS_Init(void);
extern	void     LGBUS_Cleanup(void);

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/
extern	int		g_lgbus_debug_fd;

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _LGBUS_DRV_H_ */

/** @} */
