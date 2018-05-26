/*
 * SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 * Copyright(c) 2013 by LG Electronics Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

/** @file 
 *
 *  driver interface header for monitor device. ( used only within kdriver )
 *	monitor device will teach you how to make device driver with new platform.
 *
 *  @author		raxis.lim (raxis.lim@lge.com).lim (raxis.lim@lge.com)
 *  @version	1.0 
 *  @date		2011.03.18
 *
 *  @addtogroup lg1150_monitor
 *	@{
 */

#ifndef	_MONITOR_DRV_H_
#define	_MONITOR_DRV_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include "debug_util.h"
#include "monitor_cfg.h"
#include "monitor_kapi.h"

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
#define	MONITOR_PRINT(format, args...)		DBG_PRINT( g_monitor_debug_fd, LX_LOGM_LEVEL_INFO, format, ##args)
#define MONITOR_WARN(fmt, args...) 			DBG_PRINT( g_monitor_debug_fd, LX_LOGM_LEVEL_WARNING, "^y^[MON-WAN] %s:%d -- " fmt, __F__, __L__, ##args)
#define MONITOR_ERROR(fmt, args...) 		DBG_PRINT( g_monitor_debug_fd, LX_LOGM_LEVEL_ERROR, "^y^[MON-ERR] %s:%d -- " fmt, __F__, __L__, ##args)

#define MONITOR_CHECK_CODE(__checker,__if_action,fmt,args...)   \
             __CHECK_IF_ERROR(__checker, MONITOR_WARN, __if_action , fmt, ##args )

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/
extern	void	MONITOR_PreInit(void);
extern	int		MONITOR_Init(void);
extern	void	MONITOR_Cleanup(void);

extern	int		MONITOR_WriteReg ( UINT32 addr, UINT32 value );
extern	int		MONITOR_ReadReg	 ( UINT32 addr, UINT32* pValue );
extern  int     Doumi_Init_I2C_Config(void);
extern 	int 	Doumi_I2C_Read(UINT32 addr , UINT32 *value);
extern  int 	Doumi_I2C_Write(UINT32 addr , UINT32 value);

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/
extern	int		g_monitor_debug_fd;

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _MONITOR_DRV_H_ */

/** @} */
