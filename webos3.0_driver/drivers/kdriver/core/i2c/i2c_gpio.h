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
 *  driver interface header for gpio i2c device. ( used only within kdriver )
 *
 *  @author		ks.hyun (ks.hyun@lge.com)
 *  @version	1.0
 *  @date		2012.05.06
 *
 *  @addtogroup lg115x_i2c
 *	@{
 */


#ifndef _I2C_GPIO_H_
#define _I2C_GPIO_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include "base_types.h"
#include "i2c_core.h"
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

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/
extern	LX_I2C_DEV_HANDLE I2C_GPIO_Open(UINT8 ch);
extern	int I2C_GPIO_Close(LX_I2C_DEV_HANDLE dev);
extern	int I2C_GPIO_SetPort(LX_I2C_DEV_HANDLE dev, LX_I2C_GPIO_MAP_T *map);
extern	int I2C_GPIO_SetClock(LX_I2C_DEV_HANDLE dev, UINT32 clock);
extern	int I2C_GPIO_Transfer(LX_I2C_DEV_HANDLE dev, LX_I2C_RW_DATA_T *data, UINT8 mode);
extern	int I2C_GPIO_Init(void);
extern	int I2C_GPIO_GetDeviceNums(void);

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/


#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* __GPIO_CORE_H__ */

/** @} */

