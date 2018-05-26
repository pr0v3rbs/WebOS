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


/** @file i2c_kapi.h
 *
 *  application interface header for i2c device
 *
 *  @author		ingyu.yang (ingyu.yang@lge.com)
 *  @version		1.0
 *  @date		2009.12.30
 *
 *  @addtogroup lg1150_i2c
 *	@{
 */

#ifndef	_I2C_KAPI_H_
#define	_I2C_KAPI_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include "base_types.h"

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
#define	I2C_IOC_MAGIC		'u'
/**
@name I2C IOCTL List
ioctl list for i2c device.

@{

@def I2C_IOW_SET_CLOCK
IIC clock selection.
Before read, write operation, This IOCTL is always called for data transmission speed.
This IOCTL is set up just one channel that wish to use.

@def I2C_IOW_SET_GPIO_PORT
SW IIC initialization
When i2c device receives above IOCTL,
then enable gpio pin to SW IIC.
This IOCLL is set up just one channel that wish to use.

*/
/*----------------------------------------------------------------------------------------
	IO comand naming rule  : MODULE_IO[R][W]_COMMAND
----------------------------------------------------------------------------------------*/
#define I2C_IOW_SET_CLOCK			_IOW(I2C_IOC_MAGIC,		0, LX_I2C_CLOCK_T)
#define I2C_IOW_WRITE				_IOW(I2C_IOC_MAGIC,		1, LX_I2C_RW_DATA_T)
#define I2C_IORW_READ				_IOWR(I2C_IOC_MAGIC,	2, LX_I2C_RW_DATA_T)
#define I2C_IOW_SET_GPIO_PORT		_IOW(I2C_IOC_MAGIC,		3, LX_I2C_GPIO_MAP_T)

#define I2C_IOC_MAXNR				10
/** @} */

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/
typedef enum LX_I2C_CLOCK
{
	I2C_CLOCK_50KHZ = 0,
	I2C_CLOCK_100KHZ,
	I2C_CLOCK_200KHZ,
	I2C_CLOCK_400KHZ,
	I2C_CLOCK_700KHZ,
	I2C_CLOCK_800KHZ,
	I2C_CLOCK_1MHZ,
	I2C_CLOCK_2MHZ,
	I2C_CLOCK_4MHZ,
	I2C_CLOCK_8MHZ,

	I2C_CLOCK_INVALID = 0xFF,

} LX_I2C_CLOCK_T;


/**
 * i2c ioctl and read/write operation parameter.
 * useful when complex parameter should be passed to i2c kernel driver.
 */
 //* @see I2C_IOW_WRITE_STRUCT
 //*/
typedef struct LX_I2C_RW_DATA
{
	UINT8	slaveAddr;		// Device slave ID
	UINT8	subAddrSize;	// Address length in bytes
	UINT8	subAddr[8];		// Starting address inside the device
	UINT8	*buf;			// buffer
	UINT32	bufSize;		// size of buffer
	UINT32	clock;			// Clock
	UINT32	flag;
} LX_I2C_RW_DATA_T;

/**
 * i2c ioctl and read/write operation parameter.
 * useful when complex parameter should be passed to i2c kernel driver.
 */
 //* @see I2C_IOW_WRITE_STRUCT
 //*/
typedef struct LX_I2C_GPIO_MAP
{
	UINT8	sda;
	UINT8	scl;
} LX_I2C_GPIO_MAP_T;


/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _I2C_DRV_H_ */

/** @} */
