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
 *  driver interface header for venc device. ( used only within kdriver )
 *	venc device will teach you how to make device driver with new platform.
 *
 *  @author		youngwoo.jin (youngwoo.jin@lge.com)
 *  @version	1.0
 *  @date		2011.05.19
 *
 *  @addtogroup lg1152_venc
 *	@{
 */

#ifndef	_VENC_DRV_H_
#define	_VENC_DRV_H_

/*-----------------------------------------------------------------------------
	Control Constants
-----------------------------------------------------------------------------*/
#undef SUPPORT_VENC_DEVICE_FASYNC_FOPS

/*-----------------------------------------------------------------------------
    File Inclusions
-----------------------------------------------------------------------------*/
#include "debug_util.h"
#include "venc_cfg.h"
#include "venc_kapi.h"

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
#define VENC_KDRV_MSG(index, format, args...)	\
	DBG_PRINT( g_venc_debug_fd, index, format, ##args )

#define	VENC_ERROR(format, args...)		VENC_KDRV_MSG( VENC_MSG_ERROR, format, ##args)
#define	VENC_DEBUG(format, args...)		VENC_KDRV_MSG( VENC_MSG_DEBUG, format, ##args)
#define	VENC_TRACE(format, args...)		VENC_KDRV_MSG( VENC_MSG_TRACE, format, ##args)
#define	VENC_INFO(format, args...)		VENC_KDRV_MSG( VENC_MSG_INFO, format, ##args)
#define	VENC_NOTI(format, args...)		VENC_KDRV_MSG( VENC_MSG_NOTI, format, ##args)
#define	VENC_WARN(format, args...)		VENC_KDRV_MSG( VENC_MSG_WARN, format, ##args)

#define SET_VENC_CFG_CTRL_INDEX( index )	(gVencCFGFlag = gVencCFGFlag | (1<<(index)))
#define UNSET_VENC_CFG_CTRL_INDEX( index ) 	(gVencCFGFlag = gVencCFGFlag & ~(1<<(index)))
#define GET_VENC_CFG_CTRL_INDEX( index )	(gVencCFGFlag & (1<<(index)))

/*-----------------------------------------------------------------------------
    Type Definitions
-----------------------------------------------------------------------------*/
typedef enum {
	VENC_MSG_ERROR = LX_LOGM_LEVEL_ERROR,
	VENC_MSG_WARN = LX_LOGM_LEVEL_WARNING,
	VENC_MSG_NOTI = LX_LOGM_LEVEL_NOTI,
	VENC_MSG_INFO = LX_LOGM_LEVEL_INFO,
	VENC_MSG_DEBUG = LX_LOGM_LEVEL_DEBUG,
	VENC_MSG_TRACE = LX_LOGM_LEVEL_TRACE,
	
	VENC_MSG_INDEX_COUNT
} VENC_MSG_TYPE_T;

typedef enum {
	VENC_CFG_CTRL_FLIP_TB = 0,
	VENC_CFG_CTRL_SCD,
	VENC_CFG_CTRL_SAR,
	VENC_CFG_CTRL_MONITOR,
	VENC_CFG_CTRL_FIELD_REPEAT,
	VENC_CFG_CTRL_DEBUG_WORST,

	VENC_CFG_CTRL_COUNT
} VENC_CFG_CTRL_TYPE_T;

/*-----------------------------------------------------------------------------
	Extern Function Prototype Declaration
-----------------------------------------------------------------------------*/
extern	void     VENC_PreInit(void);
extern	int      VENC_Init(void);
extern	void     VENC_Cleanup(void);
#ifdef SUPPORT_VENC_DEVICE_FASYNC_FOPS
extern	int      VENC_KillFasync( int signo );
#endif

/*-----------------------------------------------------------------------------
	Extern Variables
-----------------------------------------------------------------------------*/
extern	int		g_venc_debug_fd;
extern 	UINT32 	gVencCFGFlag;

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _VENC_DRV_H_ */

/** @} */
