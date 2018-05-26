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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

/** @file
 *
 *  main driver implementation for monitor device.
 *	monitor device will teach you how to make device driver with new platform.
 *
 *  author		raxis.lim (raxis.lim@lge.com).lim (raxis.lim@lge.com)
 *  version		1.0
 *  date		2011.03.18
 *  note		Additional information.
 *
 *  @addtogroup lg1150_monitor
 *	@{
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/
#undef	SUPPORT_MONITOR_DEVICE_READ_WRITE_FOPS

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <asm/io.h>

#include "os_util.h"
#include "base_device.h"
#include "monitor_drv.h"

#include "reg_ctrl.h"
#include "i2c_kapi.h"
#include "i2c_drv.h"
#include "i2c_core.h"

#include "sys_regs.h"	// for ACE CONTROL

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define GET_BM(value, mask) 		((value) & (mask))
#define GET_CM(value, mask) 		(((value) & (mask)) == (mask))
#define GET_BITS(val,nd,wd)         (((val)>>(nd))&(0xFFFFFFFF>>(32-(wd))))
#define GET_PVAL(val,nd,wd)         (GET_BITS(val,nd,wd)<<(nd))
#define GET_PMSK(    nd,wd)         ((((0xFFFFFFFF)>>(nd))&(0xFFFFFFFF>>(32-(wd)))) << (nd))
#define GET_RMSK(    nd,wd)         (~GET_PMSK(nd,wd))
#define GET_MASK(val,mk)            ((val) & (mk))

#define REG_WD( addr, value )       ( *( volatile UINT32 * )( addr )) = ( UINT32 )( value )
#define REG_RD( addr )              ( *( volatile UINT32 * )( addr ))
#define DDR_WD( addr, value )       ( *( UINT32 * )( addr ) ) = ( UINT32 )( value )
#define DDR_RD( addr )              ( *( UINT32 * )( addr ) )

// 2013.07.11 Cheolyong
#define _M14A0_ABB_

// 0:ABB, 1:ACE
#define SELECT_M14A0_ACE_ABB( _r )	ACE_REG_M14A0_RdFL(bbdemod_control_0);				\
					ACE_REG_M14A0_Wr01(bbdemod_control_0, bbdemod_control, _r);	\
					ACE_REG_M14A0_WrFL(bbdemod_control_0);


/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/
	typedef struct{
		UINT8	ch;
		UINT8	clock;
	} LX_I2C_INTERNAL_T;
	
	typedef struct
	{
		UINT32				chip;
		UINT32				num_internals;
		LX_I2C_INTERNAL_T*	internal;		/* channels should be inited at initial time */
	} LX_REG_CTRL_CFG_T;


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
	/*static LX_I2C_INTERNAL_T _stI2cInternalH13A0[] = {
		{8, I2C_CLOCK_400KHZ},	// ACE
		{9, I2C_CLOCK_400KHZ}	// HIDMI PHY
	};
	
	
	static LX_REG_CTRL_CFG_T _stRegCtrlConfigs_monitor[] =
	{
		{
			.chip = LX_CHIP_REV(H13,A0),
			.num_internals = 2,
			.internal = _stI2cInternalH13A0,
		},
	};
	
	static LX_REG_CTRL_CFG_T _stDummyConfig_monitor =
	{
		.num_internals = 0,
	};

	static LX_I2C_DEV_HANDLE *_i2c_handle_monitor; 
	static LX_REG_CTRL_CFG_T *_config_monitor = &_stDummyConfig_monitor;*/

	static LX_I2C_DEV_HANDLE *_i2c_handle_gbb; 
	static LX_I2C_DEV_HANDLE *_i2c_handle_hdmi; 
	static LX_I2C_DEV_HANDLE *_i2c_handle_ace; 
	static LX_I2C_DEV_HANDLE *_i2c_handle_abb; 	// 2013.07.10 Cheolyong for M14A0

/*========================================================================================
	Implementation Group
========================================================================================*/
int	MONITOR_WriteReg	( UINT32 addr, UINT32 value )
{
	UINT32 recvAddr;
	UINT32 currAddr;
	UINT32 currValue;
	UINT32 nextValue;
	UINT32 dBit;
	UINT32 wBit;

	do {
		recvAddr = (UINT32)ioremap(addr,4);
		currAddr = recvAddr;
		currAddr >>= 2;
		currAddr <<= 2;


		if (currAddr == recvAddr) {
			REG_WD(recvAddr, value);
			break;
		}
		currValue = REG_RD(currAddr);
		nextValue = REG_RD((currAddr+4));

		dBit = (recvAddr - currAddr)<<3;
		wBit = (32 - dBit);

		currValue  = GET_BITS(currValue ,0	  ,dBit);
		currValue += GET_PVAL(value		,dBit ,wBit);

		nextValue  = GET_PVAL(nextValue ,dBit ,wBit);
		nextValue += GET_BITS(value		,0	  ,dBit);
		REG_WD(currAddr		,currValue);
		REG_WD((currAddr+4) ,nextValue);
		//iounmap((void *)recvAddr);
	} while (0);

	iounmap((void *)recvAddr);
	
	return RET_OK;
}

/* i2c handler for internal register control */
int	MONITOR_ReadReg		( UINT32 addr, UINT32 *pValue )
{
	UINT32 value;
	UINT32 recvAddr;
	UINT32 currAddr;
	UINT32 nextValue;
	UINT32 dBit;
	UINT32 wBit;

	do {
		recvAddr = (UINT32)ioremap(addr,4);
		currAddr = recvAddr;
		currAddr >>= 2;
		currAddr <<= 2;
		value = REG_RD(currAddr);

		if (currAddr == recvAddr) break;

		nextValue = REG_RD(currAddr+4);
		dBit = (recvAddr - currAddr)<<3;
		wBit = (32 - dBit);
		value  = GET_BITS(value, dBit, wBit);
		value += GET_PVAL(nextValue, wBit, dBit);

		//iounmap((void *)recvAddr);
	} while (0);

	iounmap((void *)recvAddr);
	
	*pValue = value;
	return RET_OK;
}



int Doumi_Init_I2C_Config(void)
{
	UINT8 i2c_ch,i2c_clock;

	/*i2c_ch = 8;
	i2c_clock = I2C_CLOCK_400KHZ;
	_config_monitor = &_stRegCtrlConfigs_monitor[0];
	_i2c_handle_monitor = (LX_I2C_DEV_HANDLE*)OS_KMalloc(sizeof(LX_I2C_DEV_HANDLE) * _config_monitor->num_internals);
	_i2c_handle_monitor[0] = I2C_DevOpenPriv(i2c_ch, 1); //open ch 8 for ACE(1) 1:internal
	I2C_DevSetClock(_i2c_handle_monitor[index], i2c_clock);*/
	
	i2c_ch = 8;
	i2c_clock = I2C_CLOCK_400KHZ;
	_i2c_handle_ace = (LX_I2C_DEV_HANDLE*)OS_KMalloc(sizeof(LX_I2C_DEV_HANDLE));
	_i2c_handle_ace[0] = I2C_DevOpenPriv(i2c_ch, 1); //open ch 8 for ACE(1) 1:internal
	I2C_DevSetClock(_i2c_handle_ace[0], i2c_clock);

	i2c_ch = 9;
	i2c_clock = I2C_CLOCK_400KHZ;
	_i2c_handle_hdmi = (LX_I2C_DEV_HANDLE*)OS_KMalloc(sizeof(LX_I2C_DEV_HANDLE));
	_i2c_handle_hdmi[0] = I2C_DevOpenPriv(i2c_ch, 1); //1:internal
	I2C_DevSetClock(_i2c_handle_hdmi[0], i2c_clock);

	i2c_ch = 6;
	i2c_clock = I2C_CLOCK_400KHZ;
	_i2c_handle_gbb = (LX_I2C_DEV_HANDLE*)OS_KMalloc(sizeof(LX_I2C_DEV_HANDLE));
	_i2c_handle_gbb[0] = I2C_DevOpenPriv(i2c_ch, 0); // 0:external
	I2C_DevSetClock(_i2c_handle_gbb[0], i2c_clock);

#ifdef _M14A0_ABB_
	if( !LX_COMP_CHIP( lx_chip_rev(), LX_CHIP_M14) )
	{
		//SELECT_M14A0_ACE_ABB(0)

		i2c_ch = 6;
		i2c_clock = I2C_CLOCK_400KHZ;
		_i2c_handle_abb = (LX_I2C_DEV_HANDLE*)OS_KMalloc(sizeof(LX_I2C_DEV_HANDLE));
		_i2c_handle_abb[0] = I2C_DevOpenPriv(i2c_ch, 0); // 0:external
		I2C_DevSetClock(_i2c_handle_abb[0], i2c_clock);

		//SELECT_M14A0_ACE_ABB(1)
	}
#endif
	return RET_OK;
}

int Doumi_I2C_Read(UINT32 addr , UINT32 *value)
{
	int rc = -1;
	LX_I2C_RW_DATA_T param;
	UINT8 slave_addr,chip_ID;
	UINT8 reg_addrB1,reg_addrB0;
	UINT8	data;
		
	chip_ID = (addr >> 24) & 0xff;
	slave_addr = (addr >> 16) & 0xff;
	reg_addrB1 = (addr >> 8) & 0xff;	
	reg_addrB0 = addr & 0xff;	

	param.slaveAddr		= slave_addr;
	if( (chip_ID == 0x20) //demod
		|| ( ( chip_ID == 0x40 ) && ( !LX_COMP_CHIP( lx_chip_rev(), LX_CHIP_M14 ) ) ) )	// M14A0 ABB
	{		
		param.subAddrSize	= 2;
		param.subAddr[0]	= reg_addrB1;
		param.subAddr[1]	= reg_addrB0;
	}
	else //others
	{
		param.subAddrSize	= 1;
		param.subAddr[0]	= reg_addrB0;
	}
	
	param.buf			= &data; //return data is one byte
	param.bufSize			= 1;
	param.clock			= I2C_CLOCK_INVALID;	//use initial clock
	param.flag			= 0;

	if( (chip_ID == 0x40) && ( !LX_COMP_CHIP( lx_chip_rev(), LX_CHIP_M14 ) ) )	//abb only for M14A0
	{
#ifdef _M14A0_ABB_
		//SELECT_M14A0_ACE_ABB(0)
		rc = I2C_DevTransfer(_i2c_handle_abb[0], &param, I2C_READ_MODE);
		//SELECT_M14A0_ACE_ABB(1)
#endif
	}
	else if(chip_ID == 0x30)//hdmi phy
		rc = I2C_DevTransfer(_i2c_handle_hdmi[0], &param, I2C_READ_MODE);
	else if(chip_ID == 0x20)//demod//gbb
		rc = I2C_DevTransfer(_i2c_handle_gbb[0], &param, I2C_READ_MODE);
	else //10 ace
		rc = I2C_DevTransfer(_i2c_handle_ace[0], &param, I2C_READ_MODE);

	if(rc < 0)
	{
		printk("\n Internal i2c read failed. chip_ID=0x%x, slave_addr=0x%x",chip_ID,slave_addr);
	}	

	*value = (UINT32)data;		

	//printk("\n slave:0x%2x,regL:0x%02x,regH:0x%02x\n",slave_addr,reg_addrB0,reg_addrB1);

	return RET_OK;
}



int Doumi_I2C_Write(UINT32 addr , UINT32 value)
{
	UINT8 slave_addr,chip_ID;
	UINT8 reg_addrB1,reg_addrB0;
	UINT8	data;
	int rc = -1;
	LX_I2C_RW_DATA_T param;

	
	data = (UINT8)(value&0x000000ff);
		
	chip_ID = (addr >> 24) & 0xff;
	slave_addr = (addr >> 16) & 0xff;
	reg_addrB1 = (addr >> 8) & 0xff; 
	reg_addrB0 = addr & 0xff;	

	param.slaveAddr 	= slave_addr;
	if( (chip_ID == 0x20) //demod
		|| ( ( chip_ID == 0x40 ) && ( !LX_COMP_CHIP( lx_chip_rev(), LX_CHIP_M14 ) ) ) )	// M14A0 ABB
	{		
		param.subAddrSize	= 2;
		param.subAddr[0]	= reg_addrB1;
		param.subAddr[1]	= reg_addrB0;
	}
	else //others
	{
		param.subAddrSize	= 1;
		param.subAddr[0]	= reg_addrB0;
	}

	param.buf			= &data;
	param.bufSize			= 1;
	param.clock			= I2C_CLOCK_INVALID;	//use initial clock
	param.flag			= 0;

	if( (chip_ID == 0x40) && ( !LX_COMP_CHIP( lx_chip_rev(), LX_CHIP_M14 ) ) )	//abb only for M14A0
	{
#ifdef _M14A0_ABB_
		//SELECT_M14A0_ACE_ABB(0)
		rc = I2C_DevTransfer(_i2c_handle_abb[0], &param, I2C_WRITE_MODE);
		//SELECT_M14A0_ACE_ABB(1)
#endif 
	}
	else if(chip_ID == 0x30)//hdmi phy
		rc = I2C_DevTransfer(_i2c_handle_hdmi[0], &param, I2C_WRITE_MODE);
	else if(chip_ID == 0x20)//demod//gbb
		rc = I2C_DevTransfer(_i2c_handle_gbb[0], &param, I2C_WRITE_MODE);
	else //10 ace
		rc = I2C_DevTransfer(_i2c_handle_ace[0], &param, I2C_WRITE_MODE);

	if(rc < 0)
	{
		printk("\n Internal i2c write failed. chip_ID=0x%x, slave_addr=0x%x",chip_ID,slave_addr);
	}

	return rc;
}
