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
 *  driver interface header for clock device. ( used only within kdriver )
 *
 *  @author		gaius.lee
 *  @version	1.0
 *  @date		2011.03.07
 *
 *  @addtogroup lg1150_clock
 *	@{
 */

#ifndef	_CLK_CORE_H_
#define	_CLK_CORE_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include "clk_kapi.h"
#include "clk_drv.h"

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
#define CLK_CHECK_CODE(__checker,__if_action,fmt,args...)   \
							__CHECK_IF_ERROR(__checker, CLK_ERR_PRINT, __if_action , fmt, ##args )

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/
int	CLK_DD_GetGstc ( UINT32 arg );
int	CLK_DD_Open ( UINT32 arg );
int	CLK_DD_GetTimestamp ( UINT32 arg );
int	CLK_DD_RevertTime ( UINT32 arg );
int	CLK_DD_SlamTime ( UINT32 arg );
int	CLK_DD_SetRate ( UINT32 arg );
int	CLK_DD_GetRate( UINT32 arg );

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _CLK_CORE_H_ */

/** @} */

