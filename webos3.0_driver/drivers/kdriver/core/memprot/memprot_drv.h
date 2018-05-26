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
 *  driver interface header for memprot device. ( used only within kdriver )
 *	memprot device will teach you how to make device driver with new platform.
 *
 *  @author		sanghyeun.kim
 *  @version	1.0
 *  @date		2012.08.07
 *
 *  @addtogroup lg1150_memprot
 *	@{
 */

#ifndef	_MEMPROT_DRV_H_
#define	_MEMPROT_DRV_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include "debug_util.h"
#include "memprot_cfg.h"
#include "memprot_kapi.h"

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
#define	MEMPROT_PRINT(format, args...)		DBG_PRINT(  g_memprot_debug_fd, 0, format, ##args)
#define	MEMPROT_TRACE(format, args...)		DBG_PRINTX( g_memprot_debug_fd, 0, format, ##args)

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/
extern	void     MEMPROT_PreInit(void);
extern	int      MEMPROT_Init(void);
extern	void     MEMPROT_Cleanup(void);

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/
extern	int		g_memprot_debug_fd;

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _MEMPROT_DRV_H_ */

/** @} */
