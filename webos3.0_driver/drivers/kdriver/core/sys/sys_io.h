/*
 * SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 * Copyright(c) 2013 by LG Electronics Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */


/** @file
 *
 *  driver interface header for sys device. ( used only within kdriver )
 *	sys device will teach you how to make device driver with new platform.
 *
 *  @author		ks.hyun (ks.hyun@lge.com)
 *  @version	1.0
 *  @date		2010.12.13
 *
 *  @addtogroup lg1150_sys
 *	@{
 */

#ifndef	_SYS_IO_H_
#define	_SYS_IO_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include "debug_util.h"
#include "sys_cfg.h"
#include "sys_kapi.h"

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
#define SYS_WRITE32(addr,value)	(*(volatile UINT32 *)(addr)) = (volatile UINT32)(value)
#define SYS_READ32(addr)		(*(volatile UINT32 *)(addr))

#define SYS_WRITE16(addr,value)	(*(volatile UINT16 *)(addr)) = (volatile UINT16)(value)
#define SYS_READ16(addr)		(*(volatile UINT16 *)(addr))

#define SYS_WRITE8(addr,value)	(*(volatile UINT8 *)(addr)) = (volatile UINT8)(value)
#define SYS_READ8(addr)			(*(volatile UINT8 *)(addr))


/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/
extern	int		SYS_IO_ReadMemArray(UINT32 addr, UINT32 size, void *data);
extern	int		SYS_IO_WriteMemArray(UINT32 addr, UINT32 size, const void *data);
extern	int		SYS_IO_ReadRegArray(UINT32 addr, UINT32 size, void *data);
extern	int		SYS_IO_WriteRegArray(UINT32 addr, UINT32 size, const void *data);
extern	int		SYS_IO_SetSpreadSpectrum(LX_SPREAD_SPECTRUM_T *ss);
/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/


#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _SYS_DRV_H_ */

/** @} */
