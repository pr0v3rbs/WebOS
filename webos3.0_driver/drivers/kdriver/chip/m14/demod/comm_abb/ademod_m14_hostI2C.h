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


/*****************************************************************************
**
**  Name: ademod_m14_demod.h
**
**  Description:   ABB M14 Demodulator Host I2C communication interface.
**
**  Functions
**  Implemented:    LX_ADEMOD_Result  DEMOD_M14_Analog_HostI2C_Write
**                  LX_ADEMOD_Result  DEMOD_M14_Analog_HostI2C_Read
**
**  References:     
**
**  Exports:        LX_ADEMOD_Result  DEMOD_M14_Analog_HostI2C_Write
**                  LX_ADEMOD_Result  DEMOD_M14_Analog_HostI2C_Read
**
**  Dependencies:  ademod_m14_config.h for system configuration data.
**
**  Revision History:
**
**     Date        Author          Description
**  -------------------------------------------------------------------------
**   31-07-2013   Jeongpil Yun    Initial draft.
**
*****************************************************************************/
#ifndef _ADEMOD_M14_HOSTI2C_H_
#define _ADEMOD_M14_HOSTI2C_H_

#include "ademod_common.h"
#include "ademod_common_userdef.h"
#include "ademod_common_demod.h"


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_HostI2C_Write
**
**  Description:    Writes data to host I2C.
**
**  Parameters:     hDevice      - Device Handle returned by FmLg_Demod_Open,
**                  DevAddr      - Target device address,
**                  Options      - Options, a bitmap defined as follows:
**								   bit 0: LX_ADEMOD_TWA_OPTION_SLAVE_ACK check slave acknowledgement status,
**								   bit 1..7: reserved.
**                  pData        - Pointer to data to be written,
**                  Size         - Size in bytes.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_HostI2C_Write(Handle_t hDevice,
							   UINT8 DevAddr,
							   UINT8 Options,
							   UINT8* pData,
							   UINT32 Size);


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_HostI2C_Read
**
**  Description:    Writes data to host I2C.
**
**  Parameters:     hDevice      - Device Handle returned by FmLg_Demod_Open,
**                  DevAddr      - Target device address,
**                  RegAddr      - Target register address,
**                  pData        - Pointer to data to be read,
**                  Size         - Size in bytes.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_HostI2C_Read(Handle_t hDevice,
							  UINT8 DevAddr,
							  UINT8 RegAddr,
							  UINT8* pData,
							  UINT32 Size);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_HostI2C_WriteX
**
**  Description:    Writes data to host I2C.
**
**  Parameters:     hDevice      - Device Handle returned by FmLg_Demod_Open,
**                  DevAddr      - Target device address,
**                  RegAddr      - Target register address,
**                  Options      - Options, a bitmap defined as follows:
**								   bit 0: LX_ADEMOD_TWA_OPTION_SLAVE_ACK check slave acknowledgement status,
**								   bit 1: LX_ADEMOD_TWA_OPTION_IGNORE_DEV ignore device address,
**								   bit 2..7: reserved.
**                  pData        - Pointer to data to be written,
**                  Size         - Size in bytes.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_HostI2C_WriteX(Handle_t hDevice, UINT8 DevAddr, UINT32 RegAddr, UINT32 Options, UINT8* pData, UINT32 Size);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_HostI2C_ReadX
**
**  Description:    Writes data to host I2C.
**
**  Parameters:     hDevice      - Device Handle returned by FmLg_Demod_Open,
**                  DevAddr      - Target device address,
**                  RegAddr      - Target register address,
**                  Options      - Options, a bitmap defined as follows: TBD
**                  pData        - Pointer to data to be read,
**                  Size         - Size in bytes.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_HostI2C_ReadX(Handle_t hDevice, UINT8 DevAddr, UINT32 RegAddr, UINT32 Options, UINT8* pData, UINT32 Size);


#endif //_ADEMOD_M14_HOSTI2C_H_
