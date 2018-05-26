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
 *  pvr driver
 *
 *  @author	Kyungseok Hyun (ks.hyun@lge.com)
 *  @version	1.0
 *  @date		2014-04-08
 *  @note		Additional information.
 */


#ifndef _PVR_IMPL_H_
#define _PVR_IMPL_H_


/*----------------------------------------------------------------------------------------
 *   Control Constants
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <asm/io.h>
#include <asm/uaccess.h>

#include "base_types.h"
#include "os_util.h"
#include "debug_util.h"

#include "pvr_cfg.h"
#include "pvr_kapi.h"


#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/*----------------------------------------------------------------------------------------
 *   Constant Definitions
 *---------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
 *   Macro Definitions
 *---------------------------------------------------------------------------------------*/
#define PVR_OK			0
#define PVR_FAILURE		1


#define PVR_COLOR_NONE		"\x1b[0m"
#define PVR_COLOR_RED		"\x1b[1;31m"
#define PVR_COLOR_GREEN		"\x1b[1;32m"
#define PVR_COLOR_YELLOW	"\x1b[1;33m"
#define PVR_COLOR_BLUE		"\x1b[1;34m"
#define PVR_COLOR_PURPLE	"\x1b[1;35m"
#define PVR_COLOR_CYAN		"\x1b[1;36m"


typedef enum
{
	PVR_ERROR		= LX_LOGM_LEVEL_ERROR,		/* 0 */
	PVR_WARNING		= LX_LOGM_LEVEL_WARNING,	/* 1 */
	PVR_NOTI		= LX_LOGM_LEVEL_NOTI,		/* 2 */
	PVR_INFO		= LX_LOGM_LEVEL_INFO,		/* 3 */
	PVR_DEBUG		= LX_LOGM_LEVEL_DEBUG,		/* 4 */
	PVR_TRACE		= LX_LOGM_LEVEL_TRACE,		/* 5 */
	PVR_TRACE_EX,		/* 6 */
	PVR_DRV,			/* 7 */
	PVR_NORMAL, 		/* 8 */
	PVR_ISR,			/* 9 */
	PVR_UPLOAD, 		/* 10 */
	PVR_PIE,			/* 11 */
	PVR_REG,			/* 12 */
	PVR_REG_EX, 		/* 13 */
	PVR_PIE_DEBUG,		/* 14 */
	PVR_MSG_IOCTL,		/* 15 */

} PVR_MSG_TYPE_T;

#define PVR_KDRV_PRINT(type,fmt,args...)		DBG_PRINT( g_pvr_debug_fd, PVR_##type, fmt, ##args)
#define PVR_KDRV_LOG(_idx, format, args...) 	DBG_PRINT( g_pvr_debug_fd, 	_idx, 			format, ##args)



#define PVR_PRINT_ERROR(fmt,args...)			PVR_KDRV_PRINT(ERROR, PVR_COLOR_RED fmt PVR_COLOR_NONE,##args)
#define PVR_PRINT_WARNING(fmt,args...)			PVR_KDRV_PRINT(WARNING,PVR_COLOR_YELLOW fmt PVR_COLOR_NONE,##args)
#define PVR_PRINT_NOTI(fmt,args...)				PVR_KDRV_PRINT(NOTI,fmt,##args)
#define PVR_PRINT_INFO(fmt,args...)				PVR_KDRV_PRINT(INFO,fmt,##args)
#define PVR_PRINT_DEBUG(fmt,args...)			PVR_KDRV_PRINT(DEBUG,fmt,##args)
#define PVR_PRINT_TRACE(fmt,args...)			PVR_KDRV_PRINT(TRACE,fmt,##args)
#define PVR_PRINT_TRACE_EX(fmt,args...)			PVR_KDRV_PRINT(TRACE_EX,fmt,##args)
#define PVR_PRINT_DRV(fmt,args...)				PVR_KDRV_PRINT(DRV,fmt,##args)
#define PVR_PRINT_NORMAL(fmt,args...)			PVR_KDRV_PRINT(NORMAL,fmt,##args)
#define PVR_PRINT_ISR(fmt,args...)				PVR_KDRV_PRINT(ISR,fmt,##args)
#define PVR_PRINT_UPLOAD(fmt,args...)			PVR_KDRV_PRINT(UPLOAD,fmt,##args)
#define PVR_PRINT_PIE(fmt,args...)				PVR_KDRV_PRINT(PIE,fmt,##args)
#define PVR_PRINT_REG(fmt,args...)				PVR_KDRV_PRINT(REG,fmt,##args)
#define PVR_PRINT_REG_EX(fmt,args...)			PVR_KDRV_PRINT(REG_EX,fmt,##args)
#define PVR_PRINT_PIE_DEBUG(fmt,args...)		PVR_KDRV_PRINT(PIE_DEBUG,fmt,##args)
#define PVR_PRINT_IOCTL(fmt,args...)			PVR_KDRV_PRINT(IOCTL,fmt,##args)


#define PVR_CHECK_ERROR(__checker,__if_action,fmt,args...)   \
			 __CHECK_IF_ERROR(__checker, PVR_PRINT_ERROR, __if_action , fmt, ##args )


#define PVR_CH_NAME(c)		((c) ? 'B' : 'A')
/*----------------------------------------------------------------------------------------
 *   Type Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   External Function Prototype Declarations
 *---------------------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------------------
 *   External Variables
 *---------------------------------------------------------------------------------------*/
extern	int 	g_pvr_debug_fd;



#ifdef __cplusplus
}
#endif

#endif

