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



/** @file uart_kapi.h
 *
 *  application interface header for uart device
 *
 *  @author		ingyu.yang (ingyu.yang@lge.com)
 *  @version		1.0
 *  @date		2010.05.11
 *
 *  @addtogroup lg1150_uart
 *	@{
 */

#ifndef	_UART_KAPI_H_
#define	_UART_KAPI_H_

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
#define	UART_IOC_MAGIC		'z'
/**
@name UART IOCTL List
ioctl list for uart device.

@{
@def L8DRV_UART_INIT
UART initialization.
When uart device receives above IOCTL,
Also,Called only once at system initialization.

@def L8DRV_UART_CHANGE
UART type select.
for uart type change, This IOCTL is always called for data transmission speed.

*/
/*----------------------------------------------------------------------------------------
	IO comand naming rule  : MODULE_IO[R][W]_COMMAND
----------------------------------------------------------------------------------------*/
#define L8DRV_UART_INIT               _IO(UART_IOC_MAGIC, 0)
#define L8DRV_UART_CHANGE              _IOW(UART_IOC_MAGIC, 1, LX_UART_Param_t)
#define L8DRV_UART_PORT_CTRL			_IOW(UART_IOC_MAGIC,2, LX_UART_Param_t )


#define UART_IOC_MAXNR				3
/** @} */

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/
/**
 * uart ioctl and read/write operation parameter.
 * useful when complex parameter should be passed to uart kernel driver.
 */
 //* @see UART_IOW_WRITE_STRUCT
 //*/
typedef struct LX_UART_PARAM
{
    unsigned int u8IdUART;       ///< TOP UART
    unsigned int u8typeUART;     ///< UART TYPE
    unsigned int command;			 ///< PORT OPEN/CLOSE OPTION If flag is '0' then this is close command.
                                 ///< 1 is open command and if 2 then don't care whether open or not.
} LX_UART_Param_t;


/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _UART_DRV_H_ */

/** @} */
