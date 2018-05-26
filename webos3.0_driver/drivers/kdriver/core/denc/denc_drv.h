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
 *  driver interface header for denc device. ( used only within kdriver )
 *	denc device will teach you how to make device driver with new platform.
 *
 *  @author		wonsik.do (wonsik.do@lge.com)
 *  @version	1.0 
 *  @date		2010.01.06
 *
 *  @addtogroup lg1150_denc
 *	@{
 */

#ifndef	_DENC_DRV_H_
#define	_DENC_DRV_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include "debug_util.h"
#include "denc_cfg.h"
#include "denc_kapi.h"

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
#define DENC_ERROR(format, args...)		DBG_PRINT( g_denc_debug_fd,        \
													LX_LOGM_LEVEL_ERROR,   \
													format,                \
													##args )
#define DENC_WARN(format, args...)		DBG_PRINT( g_denc_debug_fd,        \
													LX_LOGM_LEVEL_WARNING, \
													format,                \
													##args )
#define DENC_PRINT(format, args...)		DBG_PRINT( g_denc_debug_fd,        \
													LX_LOGM_LEVEL_INFO,    \
													format,                \
													##args )
#define DENC_NOTI(format, args...)		DBG_PRINT( g_denc_debug_fd,        \
													LX_LOGM_LEVEL_NOTI,    \
													format,                \
													##args )
#define DENC_TRACE(format, args...)		DBG_PRINT( g_denc_debug_fd,        \
													LX_LOGM_LEVEL_TRACE,   \
													format,                \
													##args )

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/
extern	void     DENC_PreInit(void);
extern	int      DENC_Init(void);
extern	void     DENC_Cleanup(void);

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/
extern	int		g_denc_debug_fd;

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _DENC_DRV_H_ */

/** @} */
