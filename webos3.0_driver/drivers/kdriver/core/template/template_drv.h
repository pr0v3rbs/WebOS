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
 *  driver interface header for template device. ( used only within kdriver )
 *	template device will teach you how to make device driver with new platform.
 *
 *  @author		dae-young lim (raxis@lge.com)
 *  @version	1.0 
 *  @date		2009.11.15 
 *
 *  @addtogroup lg1150_template
 *	@{
 */

#ifndef	_TEMPLATE_DRV_H_
#define	_TEMPLATE_DRV_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include "debug_util.h"
#include "template_cfg.h"
#include "template_kapi.h"

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
#define	TEMPLATE_PRINT(format, args...)		DBG_PRINT(  g_template_debug_fd, 0, format, ##args)
#define	TEMPLATE_TRACE(format, args...)		DBG_PRINTX( g_template_debug_fd, 0, format, ##args)

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/
extern	int      TEMPLATE_Init(void);
extern	void     TEMPLATE_Cleanup(void);

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/
extern	int		g_template_debug_fd;

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _TEMPLATE_DRV_H_ */

/** @} */
