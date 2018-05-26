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


#ifndef DEMOD_MODULE_H
#define DEMOD_MODULE_H


#include "demod_kapi.h"
#include "i2c_core.h"



 /*
 	define
 */
//#define FPGA_DEMOD_TEST_H13
//#define FPGA_DEMOD_TEST_H14


/*
	Device inform
*/

/**
 * I2C .
 */


typedef struct
{
	LX_I2C_DEV_HANDLE	handle;
	UINT32				slvAddr;
	UINT8				portI2C;
} LX_DEMOD_I2C_DESCRIPT_T;


typedef struct
{
	UINT8  						id; //device minor number
	LX_DEMOD_I2C_DESCRIPT_T		handleDTV;
	LX_DEMOD_I2C_DESCRIPT_T		handleATV;
	BOOLEAN						bInit;
	UINT32						IFFrq;

	LX_DEMOD_OPER_MODE_T 		operMode;
	LX_DEMOD_RF_MODE_T			systemMode;
	LX_DEMOD_SYMRDET_MODE_T 	symbr;
	BOOLEAN 					bIsSerial;
	BOOLEAN 					bIsAbbMode;
	BOOLEAN						bIsTPoutEn;

	LX_DEMOD_CONTROL_STATE_T	controlState;
	LX_DEMOD_ACQUIRE_PROCESS_T	acqState;
	LX_DEMOD_FLAG_T				scanJobFlag;

	BOOLEAN 					bSetParams;
	LX_DEMOD_CONFIG_T			setCfgParam;
	LX_DEMOD_CONFIG_T			getCfgParam;
	LX_DEMOD_LOCK_STATE_T		lockStatus;

	LX_DEMOD_SIGNAL_CTX_T		signalStatus;
	SINT32 						freqOffset;

	BOOLEAN						bMonitorEnable;
	BOOLEAN 					bNotifiedLock;
	BOOLEAN 					bNotifiedUnlock;

	UINT8 						unLockLongCount;
	UINT8 						unLockCount;
	UINT8						lockCount;



} DEMOD_CTX_T;


#ifndef LX_ADEMOD_Result
typedef UINT32          LX_ADEMOD_Result;      /*  return codes                    */
#endif

#ifndef BOOL
typedef int				BOOL;
#endif


/*
	Enum define
*/


/*
	global variable
*/


/*
	function prototype
*/

extern int DEMOD_I2C_open(UINT8 portI2C, UINT16 deviceID);
extern int DEMOD_I2C_Close(void);
extern int DEMOD_I2C_Read( UINT16 address, UINT16 nBytes, UINT8 *pData);
extern int DEMOD_I2C_Write( UINT16 address, UINT16 nBytes, UINT8 *pData);

extern int DEMOD_ANALOG_I2C_open(UINT8 portI2C, UINT16 deviceID);
extern int DEMOD_ANALOG_I2C_Close(void);
extern int DEMOD_ANALOG_I2C_Read( UINT16 address, UINT16 nBytes, UINT8 *pData);
extern int DEMOD_ANALOG_I2C_Write( UINT16 address, UINT16 nBytes, UINT8 *pData);

extern int DEMOD_ANALOG_ResetHW(void);
extern int DEMOD_ANALOG_SetI2C(void);

extern int ADC_Test_I2C_Read( UINT8 slvAddr, UINT8 address, UINT16 nBytes, UINT8 *pData);
extern int ADC_Test_I2C_Write( UINT8 slvAddr, UINT8 address, UINT16 nBytes, UINT8 *pData);
extern int DEMOD_ADC_Control( LX_DEMOD_ADC_VOLTAGE_SEL_T votageSel);

/* HW function */
extern int DEMOD_ResetHW(void);
extern int DEMOD_SetInclk(LX_DEMOD_INCLK_SEL_T *inclk);
extern int DEMOD_InitHW(void);
extern int DEMOD_AdcInit(void);


#endif
