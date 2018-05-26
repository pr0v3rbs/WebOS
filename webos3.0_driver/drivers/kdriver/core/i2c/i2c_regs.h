/*
 * SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 * Copyright(c) 2013 by LG Electronics Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

/** @file
 *
 * main driver implementation for de device.
 * de device will teach you how to make device driver with new platform.
 *
 * author     ks.hyun (ks.hyun@lge.com)
 * version    0.1
 * date       2010.11.05
 * note       Additional information.
 *
 */

#ifndef _I2C_REGS_H_
#define _I2C_REGS_H_

#include "base_types.h"

/* Control Register 0 bit definitions I2CMSCTR0 */
#define SET_START_SEQ	0x80
#define SET_STOP_SEQ  	0x40
#define RESTART_SEQ		0x20
#define NTB_1_BYTE		0x00
#define NTB_2_BYTE		0x08
#define NTB_3_BYTE		0x10
#define NTB_4_BYTE		0x18
#define INT_ACK			0x02
#define TRANSFER_GO		0x01

/* Control Register 1 bit definitions I2CMSCTR0 */
#define TRANS_MASTER_TX	0x00
#define TRANS_MASTER_RX	0x40
#define SLAVE_MODE_OP	0x80
#define CORE_ENABLE		0x20
#define INT_ENABLE		0x10
#define CLR_MASTER_FIFO	0x08
#define CLR_SLAVE_FIFO	0x04
#define SOFT_RESET		0x02
#define CLEAR_FIFOS		0x0C
#define CLEAR_REG       0x00

/* FIFO defines */
#define FIFO_EMPTY		0x0
#define FIFO_1BYTE_DATA	0x20 /* 0010-0000 */
#define FIFO_2BYTE_DATA	0x40 /* 0100-0000 */
#define FIFO_3BYTE_DATA	0x60 /* 0110-0000 */
#define FIFO_FULL		0x80 /* 1000-0000 */

/* Master status register */
#define MASTER_STAT_MFS	0xE0
#define MASTER_STAT_MIS	0x04
#define MASTER_STAT_SNB	0x02
#define MASTER_STAT_MTB	0x01


/* Slave status register */
#define SLAVE_STAT_STB	0x01
#define SLAVE_STAT_MNB	0x02
#define SLAVE_STAT_SIS	0x04
#define SLAVE_STAT_TRM	0x08
#define SLAVE_STAT_SAR	0x10
#define SLAVE_STAT_SFS	0xE0


#define LSB8(value) 	(value & 0x00FF)
#define MSB8(value) 	((value & 0xFF00) >> 8)


#define I2C_CLOCK_FREQUENCY_L8		174000000	/* CPU Clock(696Mhz)/4 */
#define I2C_CLOCK_FREQUENCY_L9		198000000	/* DDR Clock(792Mhz)/4 */
#define I2C_CLOCK_FREQUENCY_H13		198000000	/* DDR Clock(792Mhz)/4 */
#define I2C_CLOCK_FREQUENCY_M14		198000000	/* DDR Clock(792Mhz)/4 */
#define I2C_CLOCK_FREQUENCY_H14		198000000	/* DDR Clock(792Mhz)/4 */
#define I2C_CLOCK_FREQUENCY_H15		198000000	/* DDR Clock(792Mhz)/4 */

#define I2C_REG_SIZE				0x30



/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/
/* I2CMS registers typedefined structure */
typedef struct I2C_REG
{
	volatile UINT32 I2CMMD;		/* Master Data Register */
	volatile UINT32 I2CMSSD;	/* Slave Data Register */
	volatile UINT32 I2CMSSA;	/* Slave Address Register */
	volatile UINT32 I2CMCTR0;	/* Control Register 0 */
	volatile UINT32 I2CMCTR1;	/* Control Register 1 */
	volatile UINT32 I2CMMS;		/* Master Status Register */
	volatile UINT32 I2CMSSS;	/* Slave Status Register */
	volatile UINT32 reserved;	/* Reserved */
	volatile UINT32 I2CMPREH;	/* Clock Pre-Scale Register High */
	volatile UINT32 I2CMPREL;	/* Clock Pre-Scale Register Low */
} I2C_REG_T, *I2C_REG_ADDR_T;


#endif

