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
/** @file gpio_kapi.h
 *
 *  application interface header for gpio device
 *
 *  @author		ingyu.yang (ingyu.yang@lge.com)
 *  @version		1.0
 *  @date		2009.01.06
 *
 *  @addtogroup lg1150_gpio
 *	@{
 */

#ifndef	_GPIO_KAPI_H_
#define	_GPIO_KAPI_H_

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
#define GPIO_IOC_MAGIC               'g'
/**
@name GPIO IOCTL List
ioctl list for gpio device.

@{
@def GPIO_IOW_SET_PIN_MUX
Set pin to gpio.
When gpio device receives above IOCTL with pin index, then set the pin to gpio.
In this IOCTL, just use the u8Pinnumber parameter in Gpio_Pinset_Param_t structure.

@def GPIO_IOW_SET_MODE
Set pin to input or output.
When gpio device receives above IOCTL with Gpio_Pinset_Param_t, then set the pin to input or output.
If u8flag is "1" then pin is set to the output port.

@def GPIO_IORW_GET_VALUE
Get information from external chip.
When gpio device receives above IOCTL with Gpio_Pinset_Param_t, then get the information from external chip.
In this IOCTL, just use the u8Pinnumber parameter in Gpio_Pinset_Param_t structure.

@def GPIO_IOW_SET_VALUE
Set pin to pull high or low.
When gpio device receives above IOCTL with Gpio_Pinset_Param_t, then set the pin to pull high or low.
If u8flag is "1" then pin is set to the pull high.
*/

/*----------------------------------------------------------------------------------------
	IO comand naming rule  : MODULE_IO[R][W]_COMMAND
----------------------------------------------------------------------------------------*/
#define GPIO_IOW_COMMAND_SET		_IOW(GPIO_IOC_MAGIC,	0, LX_GPIO_PARAM_T)
#define GPIO_IORW_COMMAND_GET		_IOW(GPIO_IOC_MAGIC,	1, LX_GPIO_PARAM_T)

#define GPIO_IOW_EX_COMMAND_SET		_IOW(GPIO_IOC_MAGIC,	10, LX_GPIO_PARAM_T)
#define GPIO_IORW_EX_COMMAND_GET	_IOW(GPIO_IOC_MAGIC,	11, LX_GPIO_PARAM_T)


#define GPIO_IOC_MAXNR               20
#define GPIO_NUM_IN_ARRAY			8


#define CALLBACK_DISABLE 		0
#define CALLBACK_ENABLE 		1
#define COUNT_DISABLE 			2
#define COUNT_ENABLE 			3


#define INT_ARRAY_00_02 	0
#define INT_ARRAY_03_05 	1
#define INT_ARRAY_06_08 	2
#define INT_ARRAY_09_11 	3
#define INT_ARRAY_12_14 	4
#define INT_ARRAY_15_17 	5
#define GPIO_NUM_IN_INT_ARRAY 		24			//gpio 24 pin share 1 interrupt

#define CREATE_TASK		1
#define NOT_CREATE			0

#define INT_POLL_CLEAR 	250


#define INT_ARRAY_NUM 	6

#define GPIO_IRQ_NUM_NR 6
#define GPIO_PIN_MAX 150
/** @} */

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/
typedef enum
{
	LX_GPIO_COMMAND_PIN_MUX = 0,
	LX_GPIO_COMMAND_MODE,
	LX_GPIO_COMMAND_VALUE,
	LX_GPIO_COMMAND_INTR_VALUE,
	LX_GPIO_COMMAND_ISR,
	LX_GPIO_COMMAND_INTR_ARRAY_NUM,
	LX_GPIO_COMMAND_INTR_COUNT,

} LX_GPIO_COMMAND_T;


typedef enum
{
	LX_GPIO_MODE_INPUT				= 0,
	LX_GPIO_MODE_OUTPUT				= 1,
	LX_GPIO_MODE_OUTPUT_OPENDRAIN	= 2,

} LX_GPIO_MODE_T;

typedef enum
{
	LX_GPIO_MODE_INTR_DISABLE				= 0,
	LX_GPIO_MODE_INTR_ENABLE				= 1,


} LX_GPIO_INTR_T;

typedef enum
{
	LX_GPIO_VALUE_LOW				= 0,
	LX_GPIO_VALUE_HIGH				= 1,
	LX_GPIO_VALUE_INVALID			= 0xFF,
} LX_GPIO_VALUE_T;

typedef struct GPIO_INTR_CALLBACK{
	void (*pfnGPIO_CB)	(UINT32 value);		///< The audio event message callback.
} GPIO_INTR_CALLBACK_T;

typedef struct GPIO_INTR_INFO{
	UINT32	count_enable;
	UINT32	intr_count;
} GPIO_INTR_INFO_T;

typedef struct GPIO_INTR_COUNT{
	UINT32	msec;
	UINT32	count;
} GPIO_INTR_COUNT_T;

typedef struct
{
	UINT32	task;
} LX_GPIO_INFO_T;

typedef struct
{
	UINT32	command;
    	UINT32	pin_number;
    	UINT32	data;
    	UINT32	pin_value;
    	UINT32	intr_enable;
	LX_GPIO_INFO_T		* info;
	GPIO_INTR_CALLBACK_T	cb;
	GPIO_INTR_COUNT_T intr_count;
} LX_GPIO_PARAM_T;



/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _GPIO_DRV_H_ */

/** @} */
