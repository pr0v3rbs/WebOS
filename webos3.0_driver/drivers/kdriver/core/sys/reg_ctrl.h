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

#ifndef	__REG_CTRL_H__
#define	__REG_CTRL_H__

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
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

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/
extern	int		REG_CTRL_Init(void);
extern	int		REG_CTRL_Free(void);

extern	int		REG_WriteI2C(UINT32 idx, UINT8 slave, UINT8 reg, UINT8 data);
extern	int		REG_ReadI2C(UINT32 idx, UINT8 slave, UINT8 reg, UINT8* data);
extern	int		ACE_RegWrite(UINT8 slave, UINT8 reg, UINT8 data);
extern	int		ACE_RegRead(UINT8 slave, UINT8 reg, UINT8* data);
/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* __REG_CTRL_H__ */

/** @} */

