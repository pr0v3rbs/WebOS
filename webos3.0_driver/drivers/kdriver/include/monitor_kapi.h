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
 *  application interface header for monitor device
 *
 *  author		raxis.lim@lge.com
 *  version		1.0
 *  date		2011.03.18
 *
 *  @addtogroup lg1150_monitor
 *	@{
 */

#ifndef	_MONITOR_KAPI_H_
#define	_MONITOR_KAPI_H_

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
#define	MONITOR_IOC_MAGIC		'L'
/**
@name MONITOR IOCTL List
ioctl list for monitor device.

@{
@def MONITOR_IOW_WRITE_REG
write register with UINT32 value

@def MONITOR_IORW_READ_REG
read register and return UINT32 value

*/
/*----------------------------------------------------------------------------------------
	IO comand naming rule  : MODULE_IO[R][W]_COMMAND
----------------------------------------------------------------------------------------*/
#define MONITOR_IOW_WRITE_REG			_IOW(MONITOR_IOC_MAGIC, 0, LX_REGISTER_T )
#define MONITOR_IORW_READ_REG			_IOWR(MONITOR_IOC_MAGIC, 1, LX_REGISTER_T )

#define DOUMI_I2C_INIT_CONFIG			_IO(MONITOR_IOC_MAGIC, 2 ) // for SIC doumi
#define DOUMI_I2C_IOR_READ_REG			_IOWR(MONITOR_IOC_MAGIC, 3, LX_REGISTER_T ) //for SIC doumi
#define DOUMI_I2C_IOW_WRITE_REG			_IOW(MONITOR_IOC_MAGIC, 4, LX_REGISTER_T ) //for SIC doumi

#define MONITOR_IOC_MAXNR				5
/** @} */

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _MONITOR_DRV_H_ */

/** @} */
