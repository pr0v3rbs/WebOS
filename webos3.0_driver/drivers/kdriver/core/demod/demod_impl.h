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
 *  main internal header for Demod device.
 *
 *  @author		Jeongpil.Yun (jeongpil.yun@lge.com)
 *  @version	1.0
 *  @date		2011.04.03
 *
 *  @addtogroup lg1150_demod
 *	@{
 */

#ifndef	_DEMOD_IMPL_H_
#define	_DEMOD_IMPL_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <asm/uaccess.h>

#include "base_types.h"
#include "os_util.h"
#include "debug_util.h"

#include "demod_drv.h"
#include "demod_cfg.h"
#include "demod_kapi.h"
#include "demod_hal.h"
#include "demod_module.h"
#include "demod_hw.h"

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
#define LX_LOGM_LEVEL_ALWAYS 	(LX_LOGM_LEVEL_TRACE+1)	
#define DEMOD_DEBUG_PRINT		printk
//#define	DEMOD_PRINT		printk
//#define	DEMOD_TRACE		printk
#define	DEMOD_PRINT(format, args...)		DBG_PRINT( g_demod_debug_fd, LX_LOGM_LEVEL_INFO, format, ##args)
#define	DEMOD_TRACE(format, args...)		DBG_PRINT( g_demod_debug_fd, LX_LOGM_LEVEL_TRACE, format, ##args)
#define DEMOD_WARN(format, args...)			DBG_PRINT( g_demod_debug_fd, LX_LOGM_LEVEL_WARNING, format, ##args)
#define	DEMOD_ERROR(format, args...)		DBG_PRINT( g_demod_debug_fd, LX_LOGM_LEVEL_ERROR, format, ##args)
#define	DEMOD_PRINT_ALWAYS(format, args...)		DBG_PRINT( g_demod_debug_fd, LX_LOGM_LEVEL_ALWAYS, format, ##args)

#define	DEMOD_TRACE_BEGIN()				DEMOD_TRACE("[DEMODDEV:%d] BEGIN   -- %s:%d\n", g_demod_trace_depth++, __FUNCTION__, __LINE__ )
#define	DEMOD_TRACE_END()				DEMOD_TRACE("[DEMODDEV:%d] END     -- %s:%d\n", --g_demod_trace_depth, __FUNCTION__, __LINE__ )
#define	DEMOD_TRACE_MARK()				DEMOD_TRACE("[DEMODDEV] LOGGING -- %s:%d\n", __FUNCTION__, __LINE__ )

#define	DEMOD_NOT_SUPPORTED(s)			{DEMOD_PRINT("^y^[[DEMODDEV]] %s() Not supported (but OK)\n", s); return RET_OK;}
#define	DEMOD_RETURN_FAIL(s)			{DEMOD_PRINT("^r^[[DEMODDEV]] FAIL %s()\n", s); return RET_ERROR;}


#define DEMOD_CHECK_CODE(__checker,__if_action,fmt,args...)   \
			 __CHECK_IF_ERROR(__checker, DEMOD_PRINT, __if_action , fmt, ##args )

#define DEMOD_ASSERT(__checker)	\
			__CHECK_IF_ERROR( !(__checker), DEMOD_WARN, /* nop */, "[GFXDEV] ASSERT FAILED -- %s:%d\n", __FUNCTION__, __LINE__ )

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/




#ifdef KDRV_CONFIG_PM
extern int DEMOD_HW_suspend(void);
extern int DEMOD_HW_resume(void);
#endif

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/
extern	LX_DEMOD_CFG_T		g_demod_cfg;
extern	DEMOD_HAL_T			g_demod_hal;


#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _DEMOD_DRV_H_ */

/** @} */
