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
 *  main driver implementation for i2c device.
 *	i2c device will teach you how to make device driver with new platform.
 *
 *  author		ks.hyun (ks.hyun@lge.com)
 *  version		1.0
 *  date		2009.4.6
 *  note		Additional information.
 *
 *  @addtogroup lg115x_i2c
 *	@{
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include <linux/kernel.h>
#include <linux/semaphore.h>
#include <linux/delay.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <linux/version.h>
#include "base_types.h"
#include "os_util.h"
#include "i2c_gpio.h"
#include "i2c_cfg.h"
#include "i2c_drv.h"
#include "gpio_core.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/


#define I2C_GPIO_CHECK_INITED(h)										\
	do{																	\
		if(h == NULL) return -1;										\
		if (!h->inited )												\
		{																\
			DBG_PRINT_ERROR("%s is not inited \n", h->name);			\
			return -1;													\
		}																\
	} while(0)


#define I2C_GPIO_CHECK_PARAM(subAddrSize, bufSize) 											\
	do{																					\
		if ((subAddrSize > 4) || (bufSize == 0) )										\
		{																				\
			DBG_PRINT_ERROR("Invalid parameter : SubAddrLen[%d], BufferLen[%d]\n",		\
				subAddrSize, bufSize );													\
			return -1;																	\
		}																				\
	} while(0)

#define I2C_GPIO_MAX_NAME		15

#define I2C_GPIO_MUTEX_INIT(h)		OS_InitMutex(&h->mutex, OS_SEM_ATTR_DEFAULT)
#define I2C_GPIO_MUTEX_LOCK(h)						\
	do{												\
		if(OS_LockMutex(&h->mutex) < 0)				\
		{											\
			DBG_PRINT_ERROR("Mutex Lock Fail\n");	\
			return -1;								\
		}											\
	}while(0)
#define I2C_GPIO_MUTEX_UNLOCK(h)					\
	do{ 											\
		OS_UnlockMutex(&h->mutex);					\
	}while(0)



#define I2C_ACK				1
#define I2C_NO_ACK			0
#define I2C_NACK			0

#define I2C_FLAG_START			0x01
#define I2C_FLAG_STOP			0x02
#define I2C_FLAG_RESTART		0x04
#define I2C_FLAG_ACK			0x08
#define I2C_FLAG_IGNORE_ACK		0x10


//#define I2C_GPIO_HIGH_TIMEOUT		(HZ/10)		// 100ms
#define I2C_GPIO_HIGH_TIMEOUT		(HZ)			// case for HZ is 100
#define I2C_GPIO_DELAY(h)			OS_UsecDelay(h->delay)

#define I2C_HIGH			1
#define I2C_LOW				0

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/

typedef struct I2C_GPIO
{
	UINT8				ch;
	UINT8				inited;
	UINT8				gpio_inited;

	char				name[I2C_GPIO_MAX_NAME+1];
	UINT8				clock;

	UINT32				delay;
	OS_SEM_T			mutex;

	UINT8				sda;
	UINT8				scl;
} I2C_GPIO_T, *I2C_GPIO_HANDLE;


/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Functions
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
static LX_I2C_CFG_T *_pI2cConfig;
static I2C_GPIO_T *_pI2cGpio;


/*========================================================================================
	Implementation Group
========================================================================================*/

static void _I2C_SetGpioTop(UINT8 port)
{
	GPIO_DevSetPinMux(port, TRUE);
}

static inline void _I2C_SetGpioSdaToInput(I2C_GPIO_T* h)
{
	GPIO_DevSetMode(h->sda, LX_GPIO_MODE_INPUT);	 	// set to input
}

static inline void _I2C_SetGpioSclToInput(I2C_GPIO_T* h)
{
	GPIO_DevSetMode(h->scl, LX_GPIO_MODE_INPUT);	 	// set to input
}

static inline int _I2C_GetGpioSda(I2C_GPIO_T* h)
{
	LX_GPIO_VALUE_T value;
	GPIO_DevGetValue(h->sda, &value);
	return (value == LX_GPIO_VALUE_HIGH) ? I2C_HIGH : I2C_LOW;
}

static inline int _I2C_GetGpioScl(I2C_GPIO_T* h)
{
	LX_GPIO_VALUE_T value;
	GPIO_DevGetValue(h->scl, &value);
	return (value == LX_GPIO_VALUE_HIGH) ? I2C_HIGH : I2C_LOW;
}

static int _I2C_SetGpioScl(I2C_GPIO_T* h, UINT8 clk)
{
	if(clk == I2C_HIGH)
	{
		unsigned long start = jiffies;

		_I2C_SetGpioSclToInput(h);			// set to input
		while(_I2C_GetGpioScl(h) != I2C_HIGH)
		{
			if(time_after_eq(jiffies, start + I2C_GPIO_HIGH_TIMEOUT))
			{
				I2C_PRINT_ERROR("Can't set the clock...\n");
				return -1;
			}
		}
	}
	else
	{
		/*When you chagne the in/out mode to output, the output value is the previous input value.
		  After setting the output mode, set the output value.
		*/
		GPIO_DevSetMode(h->scl, LX_GPIO_MODE_OUTPUT);	 	// set to output
		GPIO_DevSetValue(h->scl, LX_GPIO_VALUE_LOW);
	}

	return 0;
}

static int _I2C_SetGpioSda(I2C_GPIO_T* h, UINT8 data)
{
	if(data == I2C_HIGH)
	{
		unsigned long start = jiffies;

		_I2C_SetGpioSdaToInput(h);			// set to input
		while(_I2C_GetGpioSda(h) != I2C_HIGH)
		{
			if(time_after_eq(jiffies, start + I2C_GPIO_HIGH_TIMEOUT))
			{
				I2C_PRINT_ERROR("Can't set the data...\n");
				return -1;
			}
		}
	}
	else
	{
		/*When you chagne the in/out mode to output, the output value is the previous input value.
		  After setting the output mode, set the output value.
		 */
		GPIO_DevSetMode(h->sda, LX_GPIO_MODE_OUTPUT);		// set to output
		GPIO_DevSetValue(h->sda, LX_GPIO_VALUE_LOW);
	}

	return 0;
}

static void _I2C_GpioReset(I2C_GPIO_T* h)
{
	_I2C_SetGpioSclToInput(h);
	_I2C_SetGpioSdaToInput(h);
}

static int _I2C_GpioStart(I2C_GPIO_T* h)
{
	// reset clock & data
	if(_I2C_SetGpioScl(h, I2C_HIGH) < 0)
		return -1;

	if(_I2C_SetGpioSda(h, I2C_HIGH) < 0)
		return -1;

	I2C_GPIO_DELAY(h);

	_I2C_SetGpioSda(h, I2C_LOW);
	I2C_GPIO_DELAY(h);

	return 0;
}

static int _I2C_GpioRepeatStart(I2C_GPIO_T* h)
{
	if(_I2C_SetGpioScl(h, I2C_HIGH) < 0)
		return -1;
	I2C_GPIO_DELAY(h);

	_I2C_SetGpioSda(h, I2C_LOW);
	I2C_GPIO_DELAY(h);

	return 0;
}

static int _I2C_GpioStop(I2C_GPIO_T* h)
{
	_I2C_SetGpioSda(h, I2C_LOW);
	I2C_GPIO_DELAY(h);

	_I2C_SetGpioScl(h, I2C_HIGH);
	I2C_GPIO_DELAY(h);

	_I2C_SetGpioSda(h, I2C_HIGH);

	return 0;
}

static int _I2C_GpioByteWrite(I2C_GPIO_T* h, UINT8 data, UINT32 flag)
{
	UINT8 i;
	UINT8 ack;
	if((flag&I2C_FLAG_START))
	{
		if(_I2C_GpioStart(h) < 0)
			return -1;
	}
	else if((flag&I2C_FLAG_RESTART))
	{
		if(_I2C_GpioRepeatStart(h) < 0)
			return -1;
	}

	for(i=0; i<8; i++)
	{
		_I2C_SetGpioScl(h, I2C_LOW);
		_I2C_SetGpioSda(h, (data&0x80) ? I2C_HIGH : I2C_LOW);

		I2C_GPIO_DELAY(h);
		_I2C_SetGpioScl(h, I2C_HIGH);

		I2C_GPIO_DELAY(h);
		data <<= 1;
	}

	_I2C_SetGpioScl(h, I2C_LOW);
	_I2C_SetGpioSdaToInput(h);

	I2C_GPIO_DELAY(h);
	_I2C_SetGpioScl(h, I2C_HIGH);
	//read ack bit
	if((flag&I2C_FLAG_IGNORE_ACK))
		ack = I2C_ACK;
	else
		ack = _I2C_GetGpioSda(h) ? I2C_NO_ACK : I2C_ACK;

	I2C_GPIO_DELAY(h);
	_I2C_SetGpioScl(h, I2C_LOW);

	if((flag&I2C_FLAG_STOP))
	{
		if(_I2C_GpioStop(h) < 0)
			return -1;
	}
	else
	{
		_I2C_SetGpioSdaToInput(h);
	}

	I2C_GPIO_DELAY(h);

	return (ack == I2C_ACK) ? 0 : -1;
}

static int _I2C_GpioByteRead(I2C_GPIO_T* h, UINT8 *data, UINT32 flag)
{
	UINT8 i;

	*data = 0;
	for(i=0; i<8; i++)
	{
		_I2C_SetGpioScl(h, I2C_LOW);
		I2C_GPIO_DELAY(h);
		_I2C_SetGpioScl(h, I2C_HIGH);
		I2C_GPIO_DELAY(h);
		*data = (*data << 1) | _I2C_GetGpioSda(h);
	}

	//Generate Ack or Nack
	_I2C_SetGpioScl(h, I2C_LOW);
	if(flag & I2C_FLAG_ACK)
		_I2C_SetGpioSda(h, I2C_LOW);
	else
		_I2C_SetGpioSda(h, I2C_HIGH);
	I2C_GPIO_DELAY(h);

	_I2C_SetGpioScl(h, I2C_HIGH);
	I2C_GPIO_DELAY(h);


	_I2C_SetGpioScl(h, I2C_LOW);
	if((flag&I2C_FLAG_STOP))
	{
		if(_I2C_GpioStop(h) < 0)
			return -1;
	}
	else
	{
		_I2C_SetGpioSdaToInput(h);
	}
	I2C_GPIO_DELAY(h);

	return 0;
}

static int _I2C_GpioWriteCmd(I2C_GPIO_T* h, UINT8 slaveAddr, UINT8 subAddrSize,
							UINT8 *subAddr, UINT8 *buf, UINT32 bufSize, UINT32 option)
{
	UINT32 index;
	UINT32 flag;
	int res = -1;

	/* Send the slave address along with the write mode bit set */
	if(_I2C_GpioByteWrite(h, slaveAddr | I2C_WRITE_MODE, I2C_FLAG_START) < 0)
	{
		res = -1;
		goto end;
	}

	//Send sub address if it has.
	if(subAddrSize > 0)
	{
		for(index = 0; index<subAddrSize; index++)
		{
			//Write the register address to write
			if(_I2C_GpioByteWrite(h, subAddr[index], 0) < 0)
			{
				res = -2;
				goto end;
			}
		}
	}

	/* Send the data here */
	for(index = 0; index<bufSize; index++)
	{
		if(index == (bufSize-1))
			flag = I2C_FLAG_STOP;
		else
			flag = 0;

		//Write the data
		if(_I2C_GpioByteWrite (h, buf[index], flag) < 0)
		{
			res = -3;
			goto end;
		}
	}

	return bufSize;

end:
	I2C_PRINT_ERROR("_I2C_GpioWriteCmd. slave=0x%02x, res=%d\n", slaveAddr, res);
	_I2C_GpioStop(h);

	return res;
}

static int _I2C_GpioReadCmd(I2C_GPIO_T* h, UINT8 slaveAddr, UINT8 subAddrSize,
							UINT8 *subAddr, UINT8 *buf, UINT32 bufSize, UINT32 option)
{
	UINT32 index;
	UINT32 flag;
	int res = -1;

	if (subAddrSize > 0)
	{
		/* Send the slave address along with the write mode bit set */
		if(_I2C_GpioByteWrite (h, slaveAddr | I2C_WRITE_MODE, I2C_FLAG_START) < 0)
		{
			res = -1;
			goto end;
		}

		for(index = 0; index<subAddrSize; index++)
		{
			flag = 0;
			if(index == (subAddrSize-1))
			{
				if(option & I2C_STOP_START_BEFORE_READ)
					flag = I2C_FLAG_STOP;
			}

			//Write the register address to write
			if(_I2C_GpioByteWrite(h, subAddr[index], flag) < 0)
			{
				res = -2;
				goto end;
			}
		}

		if(option&I2C_STOP_START_BEFORE_READ) flag = I2C_FLAG_START;
		else if(option&I2C_START_BEFORE_READ) flag = I2C_FLAG_RESTART;
		else								  flag = 0;
	}
	else
	{
		flag = I2C_FLAG_START;
	}

	/* Send the slave address along with the read mode bit set */
	if(_I2C_GpioByteWrite (h, slaveAddr | I2C_READ_MODE, flag) < 0)
	{
		res = -3;
		goto end;
	}

	for(index=0; index<bufSize; index++)
	{
		if(index == (bufSize-1)) flag = I2C_FLAG_STOP;
		else					 flag = I2C_FLAG_ACK;

		if(_I2C_GpioByteRead(h, &buf[index], flag) < 0)
		{
			res = -4;
			goto end;
		}
	}

	return bufSize;

end:
	I2C_PRINT_ERROR("_I2C_GpioReadCmd. slave=0x%02x, res=%d\n", slaveAddr, res);
	_I2C_GpioStop(h);

	return res;
}

static int _I2C_GpioSetClock(I2C_GPIO_HANDLE h, UINT32 clock)
{
	if(clock == I2C_CLOCK_INVALID || h->clock == clock)
		return 0;

	/*****************************
		delay(uS)	freq.(Khz)
		10			47
		9			52.5
		8			57
		5			88
		4			107
		3			137
		2			185
		1			320
	******************************/
	switch ( clock )
	{
		case I2C_CLOCK_50KHZ:
			h->delay = 10;
			break;
		case I2C_CLOCK_200KHZ:
			h->delay = 2;
			break;
		case I2C_CLOCK_400KHZ:
		case I2C_CLOCK_700KHZ:
		case I2C_CLOCK_800KHZ:
			h->delay = 1;
			break;
		case I2C_CLOCK_100KHZ:
		default:
			h->delay = 5;
			break;
	}
	//DBG_PRINT_ERROR("_I2C_GpioSetClock [%d] \n",h->delay);
	h->clock = clock;

	return 0;
}


/**
********************************************************************************
* @brief
*	Initialize the i2c controller for the mode of operation passed as argument.
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*	ch : channel (0 ~ MAX)
* @return
*	-1 : error
*	others : handle
********************************************************************************
*/
LX_I2C_DEV_HANDLE I2C_GPIO_Open(UINT8 ch)
{
	I2C_GPIO_HANDLE h;

	if(ch >= _pI2cConfig->max_gpio_devices)
	{
         I2C_PRINT_ERROR("CH[%d] is not exist !!!\n", ch);
		return NULL;
	}
	h = &_pI2cGpio[ch];
	if(h->inited)
	{
		return (LX_I2C_DEV_HANDLE)h;
	}

	snprintf(h->name, I2C_GPIO_MAX_NAME, "I2C-GPIO[%d]", ch);

	h->ch = ch;
	h->clock = I2C_CLOCK_INVALID;

	I2C_GPIO_MUTEX_INIT(h);
	h->inited = TRUE;
	return (LX_I2C_DEV_HANDLE)h;
}

int I2C_GPIO_Close(LX_I2C_DEV_HANDLE dev)
{
	I2C_GPIO_HANDLE h = (I2C_GPIO_HANDLE)dev;

	I2C_GPIO_CHECK_INITED(h);

	I2C_GPIO_MUTEX_LOCK(h);
	_I2C_GpioReset(h);
	h->inited = FALSE;
	I2C_GPIO_MUTEX_UNLOCK(h);

	return 0;
}

int I2C_GPIO_SetPort(LX_I2C_DEV_HANDLE dev, LX_I2C_GPIO_MAP_T *map)
{
	I2C_GPIO_HANDLE h = (I2C_GPIO_HANDLE)dev;

	I2C_GPIO_CHECK_INITED(h);

	I2C_GPIO_MUTEX_LOCK(h);

	if(map == NULL)
	{
		I2C_GPIO_MUTEX_UNLOCK(h);
		I2C_PRINT_ERROR("gpio map is null\n");
		return -1;
	}
	if(h->gpio_inited)
	{
		I2C_GPIO_MUTEX_UNLOCK(h);
		if(map->sda == h->sda && map->scl == h->scl)
			return 0;

		I2C_PRINT_ERROR("It's already inited with a different gpio map\n");
		return -1;
	}
	//DBG_PRINT_ERROR("I2C_GPIO_SetPort 3 [%d][%d] ==[%d][%d]\n",h->sda,h->scl,map->sda,map->scl  );
	h->sda = map->sda;
	h->scl = map->scl;
	_I2C_SetGpioTop(map->sda);
	_I2C_SetGpioTop(map->scl);
	_I2C_GpioReset(h);
	h->gpio_inited = TRUE;
	I2C_GPIO_MUTEX_UNLOCK(h);
	return 0;
}

int I2C_GPIO_SetClock(LX_I2C_DEV_HANDLE dev, UINT32 clock)
{
	I2C_GPIO_HANDLE h = (I2C_GPIO_HANDLE)dev;

	I2C_GPIO_CHECK_INITED(h);
	I2C_GPIO_MUTEX_LOCK(h);
	_I2C_GpioSetClock(h, clock);
	I2C_GPIO_MUTEX_UNLOCK(h);
	return 0;
}

int I2C_GPIO_Transfer(LX_I2C_DEV_HANDLE dev, LX_I2C_RW_DATA_T *data, UINT8 mode)
{
	I2C_GPIO_HANDLE h = (I2C_GPIO_HANDLE)dev;
	int res = -1;
	I2C_GPIO_CHECK_INITED(h);
	I2C_GPIO_CHECK_PARAM(data->subAddrSize, data->bufSize);
	I2C_GPIO_MUTEX_LOCK(h);
	_I2C_GpioSetClock(h, data->clock);
	if(mode == I2C_WRITE_MODE)
	{
		//I2C_PRINT_ERROR("I2C_GPIO_Transfer write \n");
		res = _I2C_GpioWriteCmd(h, data->slaveAddr, data->subAddrSize, data->subAddr,
								data->buf, data->bufSize, data->flag);
	}
	else
	{
		//I2C_PRINT_ERROR("I2C_GPIO_Transfer read \n");
		data->flag |= I2C_START_BEFORE_READ;

		res = _I2C_GpioReadCmd(h, data->slaveAddr, data->subAddrSize, data->subAddr,
								data->buf, data->bufSize, data->flag);
	}

	I2C_GPIO_MUTEX_UNLOCK(h);

	return res;
}


int I2C_GPIO_Init(void)
{
	LX_I2C_CFG_T* config = I2C_GetConfig();
	//I2C_PRINT_ERROR("I2C_GPIO_Init \n");
	if(config == NULL)
	{
		DBG_PRINT_ERROR("Not supported chip !!!\n");
		return -1;
	}

	_pI2cGpio = (I2C_GPIO_T*)OS_Malloc(sizeof(I2C_GPIO_T) * config->max_gpio_devices);
	memset(_pI2cGpio, 0, sizeof(I2C_GPIO_T) * config->max_gpio_devices);

	_pI2cConfig = config;

	return 0;
}

int I2C_GPIO_GetDeviceNums(void)
{
	if(_pI2cConfig == NULL) return 0;
	return (_pI2cConfig->max_gpio_devices);
}

/** @} */
