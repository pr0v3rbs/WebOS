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
 *
 *  author		Murugan
 *				ks.hyun (ks.hyun@lge.com)
 *  				jun.kong (jun.kong@lge.com)
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
#include <linux/interrupt.h>
#include <linux/version.h>

#include "base_types.h"
#include "os_util.h"
#include "i2c_kapi.h"
#include "i2c_regs.h"
#include "i2c_core.h"
#include "i2c_drv.h"
#include "i2c_cfg.h"
#include "sys_regs.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

#define USE_I2C_IRQ				1		/* use i2c interrupt */
#define USE_INTERNAL_I2C_IRQ	1		/* use i2c interrupt for internal i2c channel */

#define USE_I2C_NEW_TRANSFER			/* process the i2c xfer in irq handler */


#define I2C_MAX_FIFO_SIZE		4		/* HW FIFO Size */

#define I2C_POLLING_INTERVAL	50		/* in usecs */
#define I2C_MIN_WAIT_TIME		100		/* 100ms */

#if 0
#define I2C_CORE_DEBUG(format, args...)		I2C_PRINT_ERROR(format, ##args)
#else
#define I2C_CORE_DEBUG(format, args...)		do{}while(0)
#endif

#define I2C_CHECK_INITED(h)												\
	do{																	\
		if(h == NULL) return -1;										\
		if (!h->inited )												\
		{																\
			DBG_PRINT_ERROR("%s is not inited \n", h->name);			\
			return -1;													\
		}																\
	} while(0)


#define I2C_CHECK_PARAM(subAddrSize, bufSize) 											\
	do{																					\
		if ((subAddrSize > 8) || (bufSize == 0) )										\
		{																				\
			DBG_PRINT_ERROR("Invalid parameter : SubAddrLen[%d], BufferLen[%d]\n",		\
				subAddrSize, bufSize );													\
			return -1;																	\
		}																				\
	} while(0)


#define I2C_MAX_NAME		15

#define I2C_MUTEX_INIT(h)		OS_InitMutex(&h->mutex, OS_SEM_ATTR_DEFAULT)
#define I2C_MUTEX_LOCK(h)							\
	do{												\
		if(h->func.lock(h) < 0)						\
		{											\
			DBG_PRINT_ERROR("Mutex Lock Fail\n");	\
			return -1;								\
		}											\
	}while(0)
#define I2C_MUTEX_UNLOCK(h)							\
	do{ 											\
		if(h->func.unlock(h) < 0)					\
		{											\
			DBG_PRINT_ERROR("Mutex Unlock Fail\n");	\
		}											\
	}while(0)


#define I2C_REG_WRITE(h, reg, value)		(h->reg_base->reg = value)
#define I2C_REG_READ(h, reg)				(h->reg_base->reg)

#define I2C_STATUS_RUNNING		1
#define I2C_STATUS_DONE			0
#define I2C_STATUS_ERROR		-1


#define I2C_HW_TYPE_NORMAL		0
#define I2C_HW_TYPE_INTERNAL	1


/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/
typedef struct I2C_HW*	I2C_HW_HANDLE;

typedef struct I2C_HW_XFER
{
	UINT32				mode;
	LX_I2C_RW_DATA_T	*data;

	UINT32				step;
	UINT32				fifoSize;
	UINT32				index;
	UINT32				started;

	UINT32				xferMode;
	int 				res;
} I2C_HW_XFER_T;


typedef struct I2C_HW_FUNC
{
	int 	(*open)(I2C_HW_HANDLE h);
	int 	(*close)(I2C_HW_HANDLE h);
	int 	(*setclock)(I2C_HW_HANDLE h, UINT32 clock);
	int 	(*write)(I2C_HW_HANDLE h, LX_I2C_RW_DATA_T *data);
	int 	(*read)(I2C_HW_HANDLE h, LX_I2C_RW_DATA_T *data);

	int 	(*lock)(I2C_HW_HANDLE h);
	int 	(*unlock)(I2C_HW_HANDLE h);
} I2C_HW_FUNC_T;


typedef struct I2C_HW
{
	UINT8				ch;
	UINT8				inited;
	char				name[I2C_MAX_NAME+1];

	UINT32				type;

	UINT8				use_irq;

	UINT8				clock;
	UINT16				clock_prescale;
	UINT32				clock_value;
	I2C_REG_ADDR_T		reg_base;
	OS_SEM_T			mutex;

	UINT16				poll_interval;

	char				irq_name[I2C_MAX_NAME+1];
	UINT32				irq_num;
	struct completion	irq_completion;

	spinlock_t			irq_lock;
	spinlock_t			xfer_lock;
	UINT32				xfer_started;

	I2C_HW_FUNC_T		func;
#ifdef USE_I2C_NEW_TRANSFER
	I2C_HW_XFER_T		xfer;
#endif

} I2C_HW_T;





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

/* For quick boot implementation */
/* Below register array is for copying the register values when going to suspend mode */
static I2C_REG_T	*astI2CRegArray;

static LX_I2C_CFG_T *_pI2cConfig;
static I2C_HW_T *_pI2cHw;


/* i2c handler for internal register control */
//static LX_I2C_DEV_HANDLE _pInternalHandle;

#ifdef USE_I2C_NEW_TRANSFER
static int _I2C_WriteProcess(I2C_HW_HANDLE h);
static int _I2C_ReadProcess(I2C_HW_HANDLE h);
#endif

/*========================================================================================
	Implementation Group
========================================================================================*/

static void _I2C_Reset(I2C_HW_HANDLE h)
{
	I2C_REG_WRITE(h, I2CMCTR1, SOFT_RESET);
	I2C_REG_WRITE(h, I2CMCTR1, CLEAR_FIFOS);
}

static void _I2C_HwRecovery(I2C_HW_HANDLE h)
{
	I2C_REG_WRITE(h, I2CMCTR1, SOFT_RESET);
	I2C_REG_WRITE(h, I2CMCTR1, CLEAR_FIFOS);

	I2C_REG_WRITE (h, I2CMPREH, MSB8(h->clock_prescale));
	I2C_REG_WRITE (h, I2CMPREL, LSB8(h->clock_prescale));
}

static inline void _I2C_WakeUpEvent(I2C_HW_HANDLE h)
{
	complete(&h->irq_completion);
}

static inline int _I2C_WaitforEvent (I2C_HW_HANDLE h, UINT32 timeout)
{
	int rc;
	rc = wait_for_completion_timeout( &h->irq_completion, timeout);
	if(rc == 0) return -ETIME;
	return rc;
}

static inline UINT32 _I2C_GetSubAddr(UINT8 subAddrSize, UINT8 *subAddr)
{
	UINT32 addr = 0;
	UINT8 index = 0;

	for(index=0; index<subAddrSize; index++)
		addr = (addr << 8) | subAddr[index];

	return addr;
}


static irqreturn_t _I2C_IrqHandler(int irq, void *dev)
{
	UINT8 ctr0;
	I2C_HW_HANDLE h = (I2C_HW_HANDLE)dev;
	unsigned long flags;
#ifdef USE_I2C_NEW_TRANSFER
	int done;
	UINT8 status;
	I2C_HW_XFER_T	*xfer;
#endif

	if(h == NULL)
	{
		I2C_PRINT_ERROR("Undefined I2C Interrupt !!! [%d]\n", irq);
		return IRQ_HANDLED;
	}

	ctr0 = I2C_REG_READ(h, I2CMCTR0);
	I2C_REG_WRITE(h, I2CMCTR0, ctr0 | INT_ACK);

	spin_lock_irqsave(&h->xfer_lock, flags);
	if(!h->xfer_started)
	{
		spin_unlock_irqrestore(&h->xfer_lock, flags);
		return IRQ_HANDLED;
	}

#ifdef USE_I2C_NEW_TRANSFER
	done = 0;
	xfer = &h->xfer;

	status = I2C_REG_READ (h, I2CMMS);
	if(xfer->xferMode == I2C_WRITE_MODE)
	{
		if(status & MASTER_STAT_SNB)
		{
			I2C_CORE_DEBUG("No Ack. status=0x%x !!!\n", status);
			xfer->res = I2C_ERR_NOACK;
			done = 1;
		}
	}
	else
	{
		if((status & MASTER_STAT_MFS) != (xfer->fifoSize << 5))
		{
			I2C_PRINT_ERROR("Not matched received buffer. status=0x%x !!!\n", status);
			xfer->res = I2C_ERR_TIMEOUT;
			done = 1;
		}
	}

	if(!done)
	{
		int rc;

		if(xfer->mode == I2C_WRITE_MODE)
			rc = _I2C_WriteProcess(h);
		else
			rc = _I2C_ReadProcess(h);

		if(rc != I2C_STATUS_RUNNING)
		{
			if(rc == I2C_STATUS_ERROR)
				xfer->res = -1;
			else
				xfer->res = I2C_OK;

			done = 1;
		}
	}

	if(done)
	{
		_I2C_WakeUpEvent ( h );
	}
#else
	_I2C_WakeUpEvent ( h );
#endif
	spin_unlock_irqrestore(&h->xfer_lock, flags);

	return IRQ_HANDLED;
}

static void _I2C_UninitISR(I2C_HW_HANDLE h)
{
	//free_irq (h->irq_num, NULL);
	free_irq (h->irq_num, h);
}

/*
 * Murugan -24.09.2010 - Added for suspend & resume support
 */
int I2C_DevSuspendAll(void)
{
	SINT16	i16Count;

	/* Memorize the clock speed for all channels and restore them back when resume  */
	for (i16Count = 0; i16Count < _pI2cConfig->num_hw_devices; i16Count++)
	{
		I2C_REG_ADDR_T reg_addr = _pI2cHw[i16Count].reg_base;
		//Copy to the global array
		memcpy ( (char *)&astI2CRegArray[i16Count], (char *)reg_addr, sizeof(I2C_REG_T) );

		/* Nothing to do for disable i2c */
	}
	return 0;
}

int I2C_DevResumeAll(void)
{
	SINT16	i16Count;

	/* Memorize the clock speed for all channels and restore them back when resume  */
	for (i16Count = 0; i16Count < _pI2cConfig->num_hw_devices; i16Count++)
	{
		I2C_REG_ADDR_T reg_addr = _pI2cHw[i16Count].reg_base;

		//Copy to the global array
		memcpy ( (char *)reg_addr, (char *)&astI2CRegArray[i16Count], sizeof(I2C_REG_T) );

		/* Nothing to do for enable i2c */
	}
	return 0;
}



static int _I2C_WaitForCompletion(I2C_HW_HANDLE h, UINT8 numBytes, UINT8 mode)
{
	UINT8 status;
	UINT32 timeout = (numBytes * 9)/h->clock_value + I2C_MIN_WAIT_TIME;

	if(h->use_irq)
	{
		unsigned long flags;

		int rc = _I2C_WaitforEvent(h, timeout);
		spin_lock_irqsave(&h->xfer_lock, flags);
		h->xfer_started = 0;
		spin_unlock_irqrestore(&h->xfer_lock, flags);

		if(rc < 0)
		{
			I2C_PRINT_ERROR("rc=%d, status=0x%x!!!\n", rc, I2C_REG_READ(h, I2CMMS));
			_I2C_HwRecovery(h);
			return I2C_ERR_TIMEOUT;
		}
		status = I2C_REG_READ(h, I2CMMS);
	}
	else
	{
		UINT32 count = (timeout * 1000) / h->poll_interval + 1;

		while(1)
		{
			status = I2C_REG_READ (h, I2CMMS);

			if(status & MASTER_STAT_MIS)
			{
				UINT8 ctr0 = I2C_REG_READ(h, I2CMCTR0);
				I2C_REG_WRITE(h, I2CMCTR0, ctr0 | INT_ACK);
				break;
			}

			if(count == 0)
			{
				I2C_PRINT_ERROR("Timeout. status=0x%x !!!\n", status);
				_I2C_HwRecovery(h);
				return I2C_ERR_TIMEOUT;
			}

			OS_UsecDelay(h->poll_interval);
			count--;
		}
	}

	if(mode == I2C_WRITE_MODE)
	{
		if(status & MASTER_STAT_SNB)
		{
			I2C_CORE_DEBUG("No Ack. status=0x%x !!!\n", status);
			return I2C_ERR_NOACK;
		}

		if((status & MASTER_STAT_MFS))
		{
			I2C_CORE_DEBUG("Buffer is not empty. status=0x%x !!!\n", status);
		}
	}
	else
	{
		if((status & MASTER_STAT_MFS) != (numBytes << 5))
		{
			I2C_CORE_DEBUG("Buffer is not correct. status=0x%x !!!\n", status);
		}
	}

	return 0;
}



static int _I2C_FifoStart(I2C_HW_HANDLE h, UINT8 size, UINT8 ctr0)
{
	UINT8 ntb;

	switch(size)
	{
		case 1: ntb = NTB_1_BYTE; break;
		case 2: ntb = NTB_2_BYTE; break;
		case 3: ntb = NTB_3_BYTE; break;
		case 4: ntb = NTB_4_BYTE; break;
		default: return -1;
	}

	I2C_REG_WRITE (h, I2CMCTR0, ntb | ctr0);
	return 0;
}

#ifdef USE_I2C_NEW_TRANSFER
static int _I2C_ReadProcess(I2C_HW_HANDLE h)
{
	I2C_HW_XFER_T *xfer = &h->xfer;
	LX_I2C_RW_DATA_T *data = xfer->data;
	UINT32	status;
	UINT8	ctr0;
	int		res = I2C_STATUS_RUNNING;

start:
	switch(xfer->step)
	{
		case 0:
			status = I2C_REG_READ (h, I2CMMS);
			if((status & MASTER_STAT_MTB))
			{
				// If there's no interrupt in previous cmd then this status is happend,
				// but have to ignore this.
				I2C_CORE_DEBUG("master is busy... ch=%d, slave=0x%02x, status=0x%x\n",
								h->ch, data->slaveAddr, status);
			}

			//Set the mode to master
			I2C_REG_WRITE (h, I2CMCTR1, CLEAR_FIFOS |INT_ENABLE | CORE_ENABLE);

			xfer->fifoSize = 0;
			xfer->index = 0;
			xfer->started = 0;

			if(data->subAddrSize == 0)
			{
				xfer->step = 2;
				goto start;
			}

			/* Send the slave address along with the write mode bit set */
			I2C_REG_WRITE (h, I2CMMD, data->slaveAddr | I2C_WRITE_MODE);
			xfer->fifoSize++;
			xfer->step++;

		case 1:
			/* send the sub address */
			while(xfer->index < data->subAddrSize)
			{
				//Write the register address to read from
				I2C_REG_WRITE (h, I2CMMD, data->subAddr[xfer->index]);
				xfer->fifoSize++;
				xfer->index++;

				if(xfer->fifoSize == I2C_MAX_FIFO_SIZE ||
					xfer->index == data->subAddrSize)
				{
					ctr0 = INT_ACK | TRANSFER_GO;
					if(xfer->index == data->subAddrSize)
					{
						if(data->flag & I2C_STOP_START_BEFORE_READ)
							ctr0 |= SET_STOP_SEQ;
						else if(data->flag & I2C_START_BEFORE_READ)
							ctr0 |= RESTART_SEQ;
					}

					if(!xfer->started)
					{
						ctr0 |= SET_START_SEQ;
						xfer->started = 1;
					}

					xfer->xferMode = I2C_WRITE_MODE;
					if(_I2C_FifoStart(h, xfer->fifoSize, ctr0) < 0)
						goto error;

					xfer->fifoSize = 0;
					goto end;
				}
			}

			xfer->index = 0;
			xfer->step++;

		case 2:
			if(data->subAddrSize > 0)
			{
				/* Send the slave address along with the readmode bit set */
				I2C_REG_WRITE (h, I2CMMD, data->slaveAddr | I2C_READ_MODE);

				ctr0 = INT_ACK | TRANSFER_GO;

				if( data->flag & (I2C_STOP_START_BEFORE_READ) )
				{
					ctr0 |= SET_START_SEQ;
				}

			}
			else
			{
				/* Send the slave address along with the readmode bit set */
				I2C_REG_WRITE (h, I2CMMD, data->slaveAddr | I2C_READ_MODE);
				ctr0 = INT_ACK | TRANSFER_GO | SET_START_SEQ;
			}
			xfer->fifoSize = 1;
			xfer->xferMode = I2C_WRITE_MODE;
			if(_I2C_FifoStart(h, xfer->fifoSize, ctr0) < 0)
				goto error;

			xfer->fifoSize = 0;
			xfer->step++;
			break;

		case 3:
			/* Change the I2C ip to read mode to read from the slave */
			I2C_REG_WRITE (h, I2CMCTR1, TRANS_MASTER_RX | INT_ENABLE | CORE_ENABLE | CLEAR_FIFOS);
			xfer->step++;

		case 4:
			while(xfer->index < data->bufSize)
			{
				if((xfer->index+I2C_MAX_FIFO_SIZE) < data->bufSize)
				{
					xfer->fifoSize = I2C_MAX_FIFO_SIZE;
					ctr0 = INT_ACK | TRANSFER_GO;
				}
				else
				{
					xfer->fifoSize = data->bufSize - xfer->index;
					ctr0 = INT_ACK | SET_STOP_SEQ | TRANSFER_GO;
				}

				xfer->xferMode = I2C_READ_MODE;
				if(_I2C_FifoStart(h, xfer->fifoSize, ctr0) < 0)
					goto error;

				xfer->step = 5;
				goto end;
			}

		case 5:
			while(xfer->fifoSize--)
			{
				data->buf[xfer->index++] = I2C_REG_READ (h, I2CMMD);
			}

			if(xfer->index < data->bufSize)
			{
				xfer->step = 4;
				goto start;
			}
			res = I2C_STATUS_DONE;
			break;

	}
end:
	return res;

error:
	I2C_PRINT_ERROR("step[%d], slave[%02x]\n", xfer->step, data->slaveAddr);
	return I2C_STATUS_ERROR;
}


static int _I2C_WriteProcess(I2C_HW_HANDLE h)
{
	I2C_HW_XFER_T *xfer = &h->xfer;
	LX_I2C_RW_DATA_T *data = xfer->data;
	UINT32	status;
	UINT8	ctr0;
	int		res = I2C_STATUS_RUNNING;

	switch(xfer->step)
	{
		case 0:
			status = I2C_REG_READ (h, I2CMMS);
			if((status & MASTER_STAT_MTB))
			{
				// If there's no interrupt in previous cmd then this status is happend,
				// but have to ignore this.
				I2C_CORE_DEBUG("master is busy... ch=%d, slave=0x%02x, status=0x%x\n",
								h->ch, data->slaveAddr, status);
			}
			//Set the mode to master tx, clear fifos and enable int & core
			I2C_REG_WRITE (h, I2CMCTR1, CLEAR_FIFOS |INT_ENABLE | CORE_ENABLE);

			/* Send the slave address along with the write mode bit set */
			I2C_REG_WRITE (h, I2CMMD, data->slaveAddr | I2C_WRITE_MODE);
			xfer->fifoSize = 1;
			xfer->index = 0;
			xfer->started = 0;
			xfer->xferMode = I2C_WRITE_MODE;
			xfer->step++;

		case 1:
			/* Send sub address if it has. */
			while(xfer->index < data->subAddrSize)
			{
				//Write the register address to write
				I2C_REG_WRITE (h, I2CMMD, data->subAddr[xfer->index]);
				xfer->fifoSize++;
				xfer->index++;
				if(xfer->fifoSize == I2C_MAX_FIFO_SIZE)
				{
					ctr0 = INT_ACK | TRANSFER_GO;
					if(!xfer->started)
					{
						ctr0 |= SET_START_SEQ;
						xfer->started = 1;
					}

					if(_I2C_FifoStart(h, xfer->fifoSize, ctr0) < 0)
						goto error;

					xfer->fifoSize = 0;
					goto end;
				}
			}
			xfer->index = 0;
			xfer->step++;

		case 2:
			/* Send the data here */
			while(xfer->index < data->bufSize)
			{
				//Write the data
				I2C_REG_WRITE (h, I2CMMD, data->buf[xfer->index]);
				xfer->fifoSize++;
				xfer->index++;
				if(xfer->fifoSize == I2C_MAX_FIFO_SIZE ||
					xfer->index == data->bufSize)
				{
					if(xfer->index == data->bufSize)
						ctr0 = INT_ACK | TRANSFER_GO | SET_STOP_SEQ;
					else
						ctr0 = INT_ACK | TRANSFER_GO;

					if(!xfer->started)
					{
						ctr0 |= SET_START_SEQ;
						xfer->started = 1;
					}

					if(_I2C_FifoStart(h, xfer->fifoSize, ctr0) < 0)
						goto error;

					xfer->fifoSize = 0;
					goto end;
				}
			}

			res = I2C_STATUS_DONE;
			break;

		default:
			goto error;
	}
end:
	return res;

error:
	I2C_PRINT_ERROR("step[%d], slave[%02x]\n", xfer->step, data->slaveAddr);
	return I2C_STATUS_ERROR;
}

static int _I2C_NewTransfer(I2C_HW_HANDLE h, LX_I2C_RW_DATA_T *data, UINT8 mode)
{
	int rc;
	I2C_HW_XFER_T *xfer = &h->xfer;
	unsigned long flags;

	spin_lock_irqsave(&h->xfer_lock, flags);

	memset(xfer, 0, sizeof(*xfer));
	xfer->data = data;
	xfer->mode = mode;

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,13,0))
        reinit_completion(&(h->irq_completion));
#else
	INIT_COMPLETION(h->irq_completion);
#endif


//I2C_PRINT_ERROR("%s.CH[%d], Slave:%02x\n", mode == I2C_WRITE_MODE ? "write" : "read", h->ch, data->slaveAddr);

	rc = (mode == I2C_WRITE_MODE) ?	_I2C_WriteProcess(h) : _I2C_ReadProcess(h);

	if(rc == I2C_STATUS_RUNNING)
	{
		UINT32 timeout = ((data->subAddrSize + data->bufSize) * 9)/h->clock_value + I2C_MIN_WAIT_TIME;

		h->xfer_started = 1;
		spin_unlock_irqrestore(&h->xfer_lock, flags);
		/* it will wait until the end of operation */
		rc = _I2C_WaitforEvent(h, timeout);

		spin_lock_irqsave(&h->xfer_lock, flags);
		h->xfer_started = 0;
		spin_unlock_irqrestore(&h->xfer_lock, flags);

		if(rc < 0)
		{
			_I2C_HwRecovery(h);
			return -1;
		}

		rc = (xfer->res < 0) ? xfer->res : data->bufSize;
		return rc;
	}
	else
	{
		spin_unlock_irqrestore(&h->xfer_lock, flags);

		I2C_PRINT_ERROR("_I2C_NewTransfer....\n");
		return -1;
	}
}
#endif

static int _I2C_ProcessFifoData(I2C_HW_HANDLE h, UINT8 size, UINT8 ctr0, UINT8 mode)
{
	unsigned long flags;

	if(h->use_irq)
	{
		spin_lock_irqsave(&h->xfer_lock, flags);

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,13,0))
        reinit_completion(&(h->irq_completion));
#else
	INIT_COMPLETION(h->irq_completion);
#endif

		h->xfer_started = 1;
		_I2C_FifoStart(h, size, ctr0);
		spin_unlock_irqrestore(&h->xfer_lock, flags);
	}
	else
	{
		_I2C_FifoStart(h, size, ctr0);
	}

	//Wait for interrupt
	if(_I2C_WaitForCompletion( h, size, mode ) < 0)
	{
		return -1;
	}

	return 0;
}


static int _I2C_ReadCmd(I2C_HW_HANDLE h, UINT8 slaveAddr, UINT8 subAddrSize,
						UINT8 *subAddr, UINT8 *buf, UINT32 bufSize, UINT32 option)
{
	int res = -1;
	UINT32 index;
	UINT8 status;
	UINT8 fifoSize = 0;
	UINT8 sta = SET_START_SEQ;
	UINT8 ctr0;

	status = I2C_REG_READ (h, I2CMMS);
	if((status & MASTER_STAT_MTB))
	{
		// If there's no interrupt in previous cmd then this status is happend,
		// but have to ignore this.
		I2C_CORE_DEBUG("master is busy... ch=%d, slave=0x%02x, status=0x%x\n",
						h->ch, slaveAddr, status);
	}

	//Set the mode to master
	I2C_REG_WRITE (h, I2CMCTR1,CLEAR_FIFOS |INT_ENABLE | CORE_ENABLE);


	if (subAddrSize > 0)
	{
		/* Send the slave address along with the write mode bit set */
		I2C_REG_WRITE (h, I2CMMD, slaveAddr | I2C_WRITE_MODE);
		fifoSize++;

		for(index = 0; index<subAddrSize; index++)
		{
			//Write the register address to read from
			I2C_REG_WRITE (h, I2CMMD, subAddr[index]);
			fifoSize++;

			if(fifoSize == 4 || (index+1) == subAddrSize)
			{
				ctr0 = INT_ACK | TRANSFER_GO;
				if((index+1) == subAddrSize)
				{
					if(option & I2C_STOP_START_BEFORE_READ)
						ctr0 |= SET_STOP_SEQ;
					else if(option & I2C_START_BEFORE_READ)
						ctr0 |= RESTART_SEQ;
				}

				if(sta)
				{
					ctr0 |= sta;
					sta = 0;
				}

				//Wait for interrupt
				if(_I2C_ProcessFifoData(h, fifoSize, ctr0, I2C_WRITE_MODE) < 0)
				{
					I2C_CORE_DEBUG("SlaveAddr=0x%02x, subAddrSize=%d, subAddr=0x%0x, bufSize=%d, index=%d\n",
						slaveAddr, subAddrSize, _I2C_GetSubAddr(subAddrSize, subAddr), bufSize, index);
					goto end;
				}
				fifoSize = 0;
			}
		}


		/* Send the slave address along with the readmode bit set */
		I2C_REG_WRITE (h, I2CMMD, slaveAddr | I2C_READ_MODE);

		ctr0 = INT_ACK | TRANSFER_GO;
		if(option & I2C_STOP_START_BEFORE_READ)
			ctr0 |= SET_START_SEQ;

	}
	else
	{
		/* Send the slave address along with the readmode bit set */
		I2C_REG_WRITE (h, I2CMMD, slaveAddr | I2C_READ_MODE);
		ctr0 = INT_ACK | TRANSFER_GO | SET_START_SEQ;
	}

	if(_I2C_ProcessFifoData(h, 1, ctr0, I2C_WRITE_MODE) < 0)
	{
		I2C_CORE_DEBUG("SlaveAddr=0x%02x, subAddrSize=%d, subAddr=0x%0x, bufSize=%d, index=%d\n",
						slaveAddr, subAddrSize, _I2C_GetSubAddr(subAddrSize, subAddr), bufSize, index);
		goto end;
	}

	/* Change the I2C ip to read mode to read from the slave */
	I2C_REG_WRITE (h, I2CMCTR1, TRANS_MASTER_RX | INT_ENABLE | CORE_ENABLE | CLEAR_FIFOS);

	index = 0;
	while(index < bufSize)
	{
		if((index+I2C_MAX_FIFO_SIZE) < bufSize)
		{
			fifoSize = I2C_MAX_FIFO_SIZE;
			ctr0 = INT_ACK | TRANSFER_GO;
		}
		else
		{
			fifoSize = bufSize - index;
			ctr0 = INT_ACK | SET_STOP_SEQ | TRANSFER_GO;
		}

		if(_I2C_ProcessFifoData(h, fifoSize, ctr0, I2C_READ_MODE) < 0)
		{
			I2C_PRINT_ERROR("SlaveAddr=0x%02x, subAddrSize=%d, subAddr=0x%0x, bufSize=%d, index=%d\n",
					slaveAddr, subAddrSize, _I2C_GetSubAddr(subAddrSize, subAddr), bufSize, index);
			goto end;
		}

		while(fifoSize--)
		{
			buf [index++] = I2C_REG_READ (h, I2CMMD);
		}
	}

	res = bufSize;

end:
	return res;
}


static int _I2C_WriteCmd(I2C_HW_HANDLE h, UINT8 slaveAddr, UINT8 subAddrSize,
							UINT8 *subAddr, UINT8 *buf, UINT32 bufSize, UINT32 option)
{
	int res = -1;
	UINT32 index;
	UINT8 status;
	UINT8 fifoSize = 0;
	UINT8 sta = SET_START_SEQ;
	UINT8 ctr0;

	status = I2C_REG_READ (h, I2CMMS);
	if((status & MASTER_STAT_MTB))
	{
		// If there's no interrupt in previous cmd then this status is happend,
		// but have to ignore this.
		I2C_CORE_DEBUG("master is busy... ch=%d, slave=0x%02x, status=0x%x\n",
						h->ch, slaveAddr, status);
	}

	//Set the mode to master tx, clear fifos and enable int & core
	I2C_REG_WRITE (h, I2CMCTR1, CLEAR_FIFOS |INT_ENABLE | CORE_ENABLE);

	/* Send the slave address along with the write mode bit set */
	I2C_REG_WRITE (h, I2CMMD, slaveAddr | I2C_WRITE_MODE);
	fifoSize++;

	//Send sub address if it has.
	if(subAddrSize > 0)
	{
		for(index = 0; index<subAddrSize; index++)
		{
			//Write the register address to write
			I2C_REG_WRITE (h, I2CMMD, subAddr[index]);
			fifoSize++;

			if(fifoSize == I2C_MAX_FIFO_SIZE)
			{
				ctr0 = INT_ACK | TRANSFER_GO;
				if(sta)
				{
					ctr0 |= sta;
					sta = 0;
				}

				if(_I2C_ProcessFifoData(h, fifoSize, ctr0, I2C_WRITE_MODE) < 0)
				{
					I2C_CORE_DEBUG("Slave=0x%02x, SubAddrSize=%d, SubAddr=0x%0x, BufSize=%d, index=%d\n",
							slaveAddr, subAddrSize, _I2C_GetSubAddr(subAddrSize, subAddr), bufSize, index);
					goto end;
				}
				fifoSize = 0;
			}
		}
	}

	/* Send the data here */
	for(index = 0; index<bufSize; index++)
	{
		//Write the data
		I2C_REG_WRITE (h, I2CMMD, buf[index]);
		fifoSize++;

		if(fifoSize == 4 || (index+1) == bufSize)
		{
			if((index+1) == bufSize)
				ctr0 = INT_ACK | TRANSFER_GO | SET_STOP_SEQ;
			else
				ctr0 = INT_ACK | TRANSFER_GO;

			if(sta)
			{
				ctr0 |= sta;
				sta = 0;
			}

			//Wait for interrupt
			if(_I2C_ProcessFifoData(h, fifoSize, ctr0, I2C_WRITE_MODE) < 0)
			{
				I2C_CORE_DEBUG("Slave=0x%02x, SubAddrSize=%d, SubAddr=0x%0x, BufSize=%d, index=%d\n",
					slaveAddr, subAddrSize, _I2C_GetSubAddr(subAddrSize, subAddr), bufSize, index);
				goto end;
			}
			fifoSize = 0;
		}
	}
	res = bufSize;

end:
	return res;
}

static int _I2C_HwOpen(I2C_HW_HANDLE h)
{
	h->reg_base = (I2C_REG_ADDR_T)ioremap(_pI2cConfig->hw_map[h->ch].reg_addr, I2C_REG_SIZE);
	_I2C_Reset(h);

	spin_lock_init(&h->irq_lock);
	spin_lock_init(&h->xfer_lock);
	h->xfer_started = 0;

	if(h->use_irq)
	{
		/* CAUTION !!!.
		 * if the i2c controller was used before kernel booting like uboot,
		 * the irq handler is called during rquesting irq.
		 */
		I2C_REG_WRITE (h, I2CMCTR0, INT_ACK);

		h->irq_num = _pI2cConfig->hw_map[h->ch].irq;
		sprintf(h->irq_name, "I2C IRQ[%d]", h->ch);
		if(request_irq( h->irq_num, _I2C_IrqHandler, IRQF_DISABLED, h->irq_name, h))
		{
			I2C_PRINT_ERROR("IRQ[%d] request error\n", h->irq_num);
			return -1;
		}

		init_completion(&h->irq_completion);
	}

	return 0;
}


static int _I2C_HwClose(I2C_HW_HANDLE h)
{
	if(h->use_irq)
	{
		_I2C_UninitISR(h);
	}
	_I2C_Reset(h);

	return 0;
}

static int _I2C_HwSetClock_L9(I2C_HW_HANDLE h, UINT32 clock)
{
	UINT16 clockPrescale;

	switch ( clock )
	{
		case I2C_CLOCK_50KHZ:
			clockPrescale = 0xFF; /*real i2c minimal clock is 80MHz*/
			h->clock_value = 50;
			break;
		case I2C_CLOCK_200KHZ:
			clockPrescale = 0x68;
			h->clock_value = 200;
			break;
		case I2C_CLOCK_400KHZ:
			clockPrescale = 0x37;
			h->clock_value = 400;
			break;
		case I2C_CLOCK_1MHZ:
			clockPrescale = 0x14;
			h->clock_value = 1000;
			break;
		case I2C_CLOCK_2MHZ:
			clockPrescale = 0xA;
			h->clock_value = 2000;
			break;
		case I2C_CLOCK_4MHZ:
			clockPrescale = 0x5;
			h->clock_value = 4000;
			break;
		case I2C_CLOCK_8MHZ:
			clockPrescale = 0x3;
			h->clock_value = 8000;
			break;
		case I2C_CLOCK_100KHZ:
		default:
			clockPrescale = 0xD5;
			h->clock_value = 100;
			break;
	}
	h->clock_prescale = clockPrescale;
	h->poll_interval = 1;

	//I2C_PRINT_ERROR("CH[%d], CLOCK[%d], PRESCALE[%d][0x%x:0x%x]\n", h->ch, h->clock_value, clockPrescale, MSB8(clockPrescale), LSB8(clockPrescale));
	I2C_REG_WRITE (h, I2CMPREH, MSB8(clockPrescale));
	I2C_REG_WRITE (h, I2CMPREL, LSB8(clockPrescale));

	return 0;
}

static int _I2C_HwSetClock(I2C_HW_HANDLE h, UINT32 clock)
{
	UINT16 clockPrescale;
	UINT32 numerator;
	UINT32 clockKHz;

	/* How to calculate the presacle value ? (단순히 정리하고 나며 다음과 같은 식이 존재한다.)
	 *  prescale = PCLK(MHZ) * 100 / i2c clock(KHZ)
	 *
	 * if pclk = 198MHz, i2c clock = 100Khz then
	 * prescale = 198 * 100 / 100 = 198 = 0xC6
	 *
	 * 주의) prescale 값이 작을 수록 i2c clock은 더 크다.
	 * 즉 소수점 이하에 대하여 버리게 될 경우 더 큰 clock 값이 됨으로 이에 대한 고려가 필요하다.
	 */
	switch ( clock )
	{
		case I2C_CLOCK_4MHZ:	clockKHz = 3800; break;
		case I2C_CLOCK_2MHZ:	clockKHz = 1900; break;
		case I2C_CLOCK_1MHZ:	clockKHz = 900; break;
		case I2C_CLOCK_700KHZ: clockKHz = 660; break;
		case I2C_CLOCK_800KHZ: clockKHz = 760; break;
		case I2C_CLOCK_50KHZ:	clockKHz = 45; break;
		case I2C_CLOCK_200KHZ:	clockKHz = 190; break;
		case I2C_CLOCK_400KHZ:	clockKHz = 380; break;
		case I2C_CLOCK_100KHZ:
		default:				clockKHz = 90; break;
	}
	h->clock_value = clockKHz;


	numerator = (_pI2cConfig->clock_freq/(1000*1000)) * 100;
	clockPrescale = (UINT16)(numerator/clockKHz + ((numerator%clockKHz) ? 1 : 0));

	h->clock_prescale = clockPrescale;
	h->poll_interval = 1;

//	I2C_PRINT_ERROR("CH[%d], CLOCK[%d], PRESCALE[0x%x][0x%x:0x%x]\n", h->ch, h->clock_value, clockPrescale, MSB8(clockPrescale), LSB8(clockPrescale));

	I2C_REG_WRITE (h, I2CMPREH, MSB8(clockPrescale));
	I2C_REG_WRITE (h, I2CMPREL, LSB8(clockPrescale));

	return 0;
}


#define HW_START_XMIT(h,flag)			\
	do{									\
		flag = h->use_irq;				\
		if(in_interrupt() && flag){		\
			disable_irq(h->irq_num);	\
			h->use_irq = 0;				\
		}								\
	}while(0)

#define HW_END_XMIT(h,flag)				\
	do{ 								\
		if(in_interrupt() && flag){ 	\
			h->use_irq = 1; 			\
			enable_irq(h->irq_num);		\
		}								\
	}while(0)

static int _I2C_HwWrite(I2C_HW_HANDLE h, LX_I2C_RW_DATA_T *data)
{
	int res;
	UINT32 flag;

	HW_START_XMIT(h,flag);
#ifdef	USE_I2C_NEW_TRANSFER
	if(h->use_irq)
		res = _I2C_NewTransfer(h, data, I2C_WRITE_MODE);
	else
		res = _I2C_WriteCmd(h, data->slaveAddr, data->subAddrSize, data->subAddr,
							data->buf, data->bufSize, data->flag);
#else
	res = _I2C_WriteCmd(h, data->slaveAddr, data->subAddrSize, data->subAddr,
						data->buf, data->bufSize, data->flag);
#endif
	HW_END_XMIT(h,flag);

	return res;
}

static int _I2C_HwRead(I2C_HW_HANDLE h, LX_I2C_RW_DATA_T *data)
{
	int res;
	UINT32 flag;

	HW_START_XMIT(h,flag);
#ifdef	USE_I2C_NEW_TRANSFER
	if(h->use_irq)
		res = _I2C_NewTransfer(h, data, I2C_READ_MODE);
	else
		res = _I2C_ReadCmd(h, data->slaveAddr, data->subAddrSize, data->subAddr,
							data->buf, data->bufSize, data->flag);
#else
	res = _I2C_ReadCmd(h, data->slaveAddr, data->subAddrSize, data->subAddr,
						data->buf, data->bufSize, data->flag);
#endif
	HW_END_XMIT(h,flag);

	return res;
}

static int _I2C_HwLock(I2C_HW_HANDLE h)
{
	return OS_LockMutex(&h->mutex);
}

static int _I2C_HwUnlock(I2C_HW_HANDLE h)
{
	OS_UnlockMutex(&h->mutex);
	return 0;
}

static int _I2C_HwInternalLock_L9(I2C_HW_HANDLE h)
{
	int rc = 0;

	if(in_interrupt())
	{
		/* irqs be able to enter here must be disabled in normal mode */
		spin_lock(&h->irq_lock);
	}
	else
	{
		rc = OS_LockMutex(&h->mutex);

		/* disable_irq waits for any peding IRQ handlers for this interrupt to complete before returning */
		disable_irq(L9_IRQ_HDMI1);
	}

	return rc;
}

static int _I2C_HwInternalUnlock_L9(I2C_HW_HANDLE h)
{
	if(in_interrupt())
	{
		spin_unlock(&h->irq_lock);
	}
	else
	{
		enable_irq(L9_IRQ_HDMI1);
		OS_UnlockMutex(&h->mutex);
	}
	return 0;
}



static void _I2C_SetFunctions(I2C_HW_HANDLE h)
{
	h->func.open		= _I2C_HwOpen;
	h->func.close		= _I2C_HwClose;

	if(!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_L9))
		h->func.setclock = _I2C_HwSetClock_L9;
	else
		h->func.setclock = _I2C_HwSetClock;

	h->func.write		= _I2C_HwWrite;
	h->func.read		= _I2C_HwRead;

	if(h->type == I2C_HW_TYPE_INTERNAL)
	{
		if(!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_L9))
		{
			h->func.lock	= _I2C_HwInternalLock_L9;
			h->func.unlock	= _I2C_HwInternalUnlock_L9;
		}
		else
		{
			h->func.lock	= _I2C_HwLock;
			h->func.unlock	= _I2C_HwUnlock;
		}
	}
	else
	{
		h->func.lock	= _I2C_HwLock;
		h->func.unlock	= _I2C_HwUnlock;
	}
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
LX_I2C_DEV_HANDLE I2C_DevOpenPriv(UINT8 ch, UINT8 internal)
{
	I2C_HW_HANDLE h;

	if(ch >= _pI2cConfig->num_hw_devices)
	{
		I2C_PRINT_ERROR("CH[%d] is not exist !!!\n", ch);
		return NULL;
	}

	h = &_pI2cHw[ch];
	if(h->inited)
	{
		return (LX_I2C_DEV_HANDLE)h;
	}

	h->type = internal ? I2C_HW_TYPE_INTERNAL : I2C_HW_TYPE_NORMAL;

	h->ch = ch;
	h->clock = I2C_CLOCK_INVALID;
	h->use_irq = internal ? USE_INTERNAL_I2C_IRQ : USE_I2C_IRQ;
	snprintf(h->name, I2C_MAX_NAME, "I2C[%d]", ch);

	I2C_MUTEX_INIT(h);

	_I2C_SetFunctions(h);

	if(h->func.open(h) < 0)
	{
		I2C_PRINT_ERROR("CH[%d] device open fail !!!\n", ch);
		return NULL;
	}

	h->inited = TRUE;
	return (LX_I2C_DEV_HANDLE)h;
}

LX_I2C_DEV_HANDLE I2C_DevOpen(UINT8 ch)
{
	return I2C_DevOpenPriv(ch, 0);
}

int I2C_DevClose(LX_I2C_DEV_HANDLE dev)
{
	int res = -1;
	I2C_HW_HANDLE h = (I2C_HW_HANDLE)dev;

	I2C_CHECK_INITED(h);

	I2C_MUTEX_LOCK(h);
	res = h->func.close(h);
	I2C_MUTEX_UNLOCK(h);

	h->inited = FALSE;
	return res;
}

static inline int _I2C_SetClock(I2C_HW_HANDLE h, UINT32 clock)
{
	int res;
	if(clock != I2C_CLOCK_INVALID && h->clock != clock)
	{
		res = h->func.setclock(h, clock);
		if(res == 0) h->clock = clock;
	}
	else
	{
		res = 0;
	}
	return res;
}

int I2C_DevSetClock(LX_I2C_DEV_HANDLE dev, UINT32 clock)
{
	int res = -1;
	I2C_HW_HANDLE h = (I2C_HW_HANDLE)dev;

	I2C_CHECK_INITED(h);

	I2C_MUTEX_LOCK(h);
	res = _I2C_SetClock(h, clock);
	I2C_MUTEX_UNLOCK(h);

	return res;
}

int I2C_DevTransfer(LX_I2C_DEV_HANDLE dev, LX_I2C_RW_DATA_T *data, UINT8 mode)
{
	int res = -1;
	I2C_HW_HANDLE h = (I2C_HW_HANDLE)dev;

	I2C_CHECK_INITED(h);
	I2C_CHECK_PARAM(data->subAddrSize, data->bufSize);

	I2C_MUTEX_LOCK(h);

	res = _I2C_SetClock(h, data->clock);

	if(mode == I2C_WRITE_MODE)
	{
		res = h->func.write(h, data);
	}
	else
	{
		data->flag |= I2C_START_BEFORE_READ;
		res = h->func.read(h, data);
	}

	I2C_MUTEX_UNLOCK(h);

	return res;
}


int I2C_DevInit(void)
{
	LX_I2C_CFG_T* config = I2C_GetConfig();

	if(config == NULL)
	{
		DBG_PRINT_ERROR("Not supported chip !!!\n");
		return -1;
	}

	_pI2cHw = OS_Malloc(sizeof(I2C_HW_T) * config->num_hw_devices);
	memset(_pI2cHw, 0, sizeof(I2C_HW_T) * config->num_hw_devices);
	_pI2cConfig = config;


	astI2CRegArray = OS_Malloc(sizeof(I2C_REG_T) * config->num_hw_devices);
	memset(astI2CRegArray, 0, sizeof(I2C_REG_T) * config->num_hw_devices);

	return 0;
}

int I2C_DevGetDeviceNums(void)
{
	if(_pI2cConfig == NULL) return -1;

	return _pI2cConfig->num_hw_devices;
}

/** @} */
