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
 *  driver interface header for i2c device. ( used only within kdriver )
 *	i2c device will teach you how to make device driver with new platform.
 *
 *  @author		ingyu.yang (ingyu.yang@lge.com)
 *  @version	1.0
 *  @date		2009.12.30
 *
 *  @addtogroup lg1150_i2c
 *	@{
 */

#ifndef	_I2C_DRV_H_
#define	_I2C_DRV_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include "debug_util.h"
#include "i2c_cfg.h"
#include "i2c_kapi.h"

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
#define	I2C_PRINT(format, args...)			DBG_PRINT(  g_i2c_debug_fd, I2C_MSG_NORMAL, format, ##args)
#define	I2C_PRINT_ERROR(format, args...)	DBG_PRINTX(  g_i2c_debug_fd, I2C_MSG_ERROR, format, ##args)
#define	I2C_TRACE(format, args...)			DBG_PRINTX( g_i2c_debug_fd, I2C_MSG_NORMAL, format, ##args)
#define	I2C_ISR_PRINT(format, args...)		DBG_PRINT(  g_i2c_debug_fd, I2C_MSG_NORMAL, format, ##args)
#define	I2C_DEBUG(format, args...)			DBG_PRINT( g_i2c_debug_fd, I2C_MSG_DEBUG, format, ##args)
#define	I2C_NOTI(format, args...)			DBG_PRINT( g_i2c_debug_fd, I2C_MSG_NOTI, format, ##args)

#define I2C_TYPE_HW			0
#define I2C_TYPE_GPIO		1

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/

typedef enum {
	I2C_MSG_ERROR = LX_LOGM_LEVEL_ERROR,
	I2C_MSG_WARN = LX_LOGM_LEVEL_WARNING,
	I2C_MSG_NOTI = LX_LOGM_LEVEL_NOTI,
	I2C_MSG_INFO = LX_LOGM_LEVEL_INFO,
	I2C_MSG_DEBUG = LX_LOGM_LEVEL_DEBUG,
	I2C_MSG_TRACE = LX_LOGM_LEVEL_TRACE,
	I2C_MSG_NORMAL = (LX_LOGM_LEVEL_TRACE + 1),	// print
	I2C_MSG_VSYNC = (LX_LOGM_LEVEL_TRACE + 2),		// vsync
	I2C_MSG_TIMER = (LX_LOGM_LEVEL_TRACE + 3),		// timer
	I2C_MSG_INTER = (LX_LOGM_LEVEL_TRACE + 4),		// interrupt

	I2C_MSG_INDEX_COUNT
} I2C_MSG_TYPE_T;
typedef struct I2C_DEV *LX_I2C_DEV_HANDLE;



/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/
extern	void     I2C_PreInit(void);
extern	int      I2C_Init(void);
extern	void     I2C_Cleanup(void);

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/
extern	int		g_i2c_debug_fd;

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _I2C_DRV_H_ */

/** @} */
