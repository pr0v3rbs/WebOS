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
 *
 *  @author		ks.hyun (ks.hyun@lge.com)
 *  				jun.kong (jun.kong@lge.com)
 *  @version	1.0
 *  @date		2012.05.03
 *
 *  @addtogroup lg115x_i2c
 *	@{
 */

#ifndef _I2C_CORE_H_
#define _I2C_CORE_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include "base_types.h"
#include "i2c_drv.h"
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
/* Read/write mode for the command register */
#define I2C_WRITE_MODE  0x0
#define I2C_READ_MODE   0x1

#define I2C_OK					0
#define I2C_ERR_NOACK			-1
#define I2C_ERR_TIMEOUT			-2

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/
enum
{
	I2C_NACK_LAST_BYTE			= 0x0010,	/* NACK last byte read from slave   */
	I2C_START_BEFORE_READ		= 0x0020,	/* During a read transaction, gen.  */
											/* a repeated START after writing   */
											/* the slave addr(just before read) */
	I2C_STOP_START_BEFORE_READ	= 0x0040,	/* During a read transaction, gen.  */
											/* a STOP & START after writing     */
											/* the slave addr(just before read) */
};


/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/
int I2C_DevInit(void);
int I2C_DevGetDeviceNums(void);

LX_I2C_DEV_HANDLE I2C_DevOpen(UINT8 ch);
LX_I2C_DEV_HANDLE I2C_DevOpenPriv(UINT8 ch, UINT8 internal);

int I2C_DevClose(LX_I2C_DEV_HANDLE dev);

int I2C_DevSetClock(LX_I2C_DEV_HANDLE dev, UINT32 clock);
int I2C_DevTransfer(LX_I2C_DEV_HANDLE dev, LX_I2C_RW_DATA_T *data, UINT8 mode);
int I2C_DevSuspendAll(void);
int I2C_DevResumeAll(void);
/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _I2C_CORE_H_ */

/** @} */
