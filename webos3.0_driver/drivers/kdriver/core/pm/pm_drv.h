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
 *  main driver implementation for pm device.
 *	pm device will teach you how to make device driver with new platform.
 *
 *  author		hankyung.yu (hankyung.yu@lge.com)
 *  version		1.0
 *  date			2014.07.18
 *  note			Additional information.
 *
 *  @addtogroup lg1210_pm
 */


#ifndef	_PM_DRV_H_
#define	_PM_DRV_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include "base_types.h"
#include "debug_util.h"
#include "pm_cfg.h"
#include "pm_kapi.h"

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
#define PMINFO_HEADER						0x504d4657
#define PMINFO_VERSION_1					1

#define PMINFO_SIZE							0x4000
#define PMINFO_OFFSET						(0x40000 - PMINFO_SIZE)

#define	PMGOV_DISABLE						0x0
#define	PMGOV_ENABLE						0x1

#define PM_CMD_NONE			0
#define PM_CMD_W_CONFIG		1
#define PM_CMD_R_CONFIG		2


/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define	PM_PRINT(format, args...)		DBG_PRINT(  g_pm_debug_fd, PM_MSG_TRACE, format, ##args)
#define	PM_TRACE(format, args...)		DBG_PRINTX( g_pm_debug_fd, PM_MSG_TRACE, format, ##args)
#define	PM_DEBUG(format, args...)		DBG_PRINT( g_pm_debug_fd, PM_MSG_DEBUG, format, ##args)
#define	PM_ERROR(format, args...)		DBG_PRINT( g_pm_debug_fd, PM_MSG_ERROR, format, ##args)
#define	PM_NOTI(format, args...)		DBG_PRINT( g_pm_debug_fd, PM_MSG_NOTI, format, ##args)


#define PM_CHECK_CODE(__checker,__if_action,fmt,args...)   \
			 __CHECK_IF_ERROR(__checker, PM_ERROR, __if_action , fmt, ##args )

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/
typedef enum {
	PM_MSG_ERROR = LX_LOGM_LEVEL_ERROR,
	PM_MSG_WARN = LX_LOGM_LEVEL_WARNING,
	PM_MSG_NOTI = LX_LOGM_LEVEL_NOTI,
	PM_MSG_INFO = LX_LOGM_LEVEL_INFO,
	PM_MSG_DEBUG = LX_LOGM_LEVEL_DEBUG,
	PM_MSG_TRACE = LX_LOGM_LEVEL_TRACE,
	PM_MSG_NORMAL = (LX_LOGM_LEVEL_TRACE + 1),	// print
	PM_MSG_VSYNC = (LX_LOGM_LEVEL_TRACE + 2),		// vsync
	PM_MSG_TIMER = (LX_LOGM_LEVEL_TRACE + 3),		// timer
	PM_MSG_INTER = (LX_LOGM_LEVEL_TRACE + 4),		// interrupt

	PM_MSG_INDEX_COUNT
} PM_MSG_TYPE_T;

typedef struct
{
	void			(*init)		(void);
	long			(*ioctl)	(unsigned int cmd, unsigned long arg);
	int				(*suspend)	(void);
	int				(*resume)	(void);
} pms_driver_t;

typedef struct
{
	void			(*init)		(void);
	void			(*cleanup)	(void);
} pms_proc_t;

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/
extern	void     PM_PreInit(void);
extern	int      PM_Init(void);
extern	void     PM_Cleanup(void);

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/
extern	int		g_pm_debug_fd;


extern int pm_intr_pin;
extern int pm_intr_pin_value;

extern spinlock_t pmPoll_lock;
#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _PM_DRV_H_ */

/** @} */
