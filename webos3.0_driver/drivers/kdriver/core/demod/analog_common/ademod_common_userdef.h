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
**  Name: ademod_common_userdef.h
**
**  Description:   	ABB Demodulator user-defined interface.
**				Low level functions such as reading and writing to the register
**				as well as timing delay are platform dependent and therefore
**				must be implemented by the user.
**
**  Functions
**  Implemented:    LX_ADEMOD_Result  ADEMOD_WriteAutoIncrement
**                  LX_ADEMOD_Result  ADEMOD_ReadAutoIncrement
**                  LX_ADEMOD_Result  ADEMOD_Sleep
**
**  References:     FMSW-0001: Fresco SDK. Software Design Document.
**
**  Exports:		LX_ADEMOD_Result  ADEMOD_WriteAutoIncrement
**                  LX_ADEMOD_Result  ADEMOD_ReadAutoIncrement
**                  LX_ADEMOD_Result  ADEMOD_Sleep
**
**  Dependencies:   demod_module.h   - for low level 2 wire functions.
**
**  Revision History:
**
**     Date        Author          Description
**  -------------------------------------------------------------------------
**   31-07-2013   Jeongpil Yun    Initial draft.
**
*****************************************************************************/

#ifndef _ADEMOD_COMMON_USERDEF_H_
#define _ADEMOD_COMMON_USERDEF_H_

#include "ademod_common.h"

#if defined( __cplusplus )
extern "C"                     /* Use "C" external linkage                  */
{
#endif

#define LX_ADEMOD_MAX_REGISTER_LENGTH		(5) // register's max length in bytes
#define MAX_REG_CACHE_SIZE		 (4000) // register cache max size (debug only)

/*
**  User interface data structure
*/
typedef struct ADEMOD_UserData_tag {
	UINT32 device_addr;
	UINT32 interface_id;
	Handle_t h_silent;
	UINT32 options;
	UINT32 pins[4];
	UINT32 invert[4];
	UINT32 status;
} LX_ADEMOD_UserData, *pADEMOD_UserData;


/*****************************************************************************
**
**  Name: ADEMOD_WriteAutoIncrement
**
**  Description:    Writes up to 4 bytes of data to the device using I2C Auto Increment mode.
**					In auto increment mode, the register address is automatically incremented
**					after each byte of data is written.
**
**  Parameters:     hUserData  - User-specific I/O parameter that was
**                               passed to demod's Open function.
**                  reg_addr   - Register Address.
**                  reg_data   - Data to be written to the device.
**                  cnt        - number of bytes to be written (from 1 to 4)
**
**  Returns:        status:
**                      retOK           - No errors
**
*****************************************************************************/
LX_ADEMOD_Result ADEMOD_WriteAutoIncrement(Handle_t hUserData,
                    UINT32  reg_addr,
                    UINT32  reg_data,
                    UINT32  cnt);

/*****************************************************************************
**
**  Name: ADEMOD_ReadAutoIncrement
**
**  Description:    Reads up to 4 bytes of data from the device using I2C with Auto Increment.
**
**  Parameters:     hUserData  - User-specific I/O parameter that was
**                               passed to demod's Open function.
**                  reg_addr   - Register Address.
**                  preg_data  - pointer to the Data read from the device.
**                  cnt        - number of bytes to read (from 1 to 4).
**
**  Returns:        status:
**                      retOK           - No errors
**
*****************************************************************************/
LX_ADEMOD_Result ADEMOD_ReadAutoIncrement(Handle_t hUserData,
                   UINT32 reg_addr,
                   UINT32 *preg_data,
                   UINT32 cnt);
/*****************************************************************************
**
**  Name: ADEMOD_Sleep
**
**  Description:    Delay execution for "nMinDelayTime" milliseconds
**
**  Parameters:     hUserData     - User-specific I/O parameter that was
**                                  passed to tuner's Open function.
**                  nMinDelayTime - Delay time in milliseconds
**
**  Returns:        None.
**
**  Notes:          This is a callback function that is called from the
**                  the tuning algorithm.  You MUST provide code that
**                  blocks execution for the specified period of time.
**
*****************************************************************************/
LX_ADEMOD_Result ADEMOD_Sleep(UINT32 nMinDelayTime);


#if defined( __cplusplus )
}
#endif

extern	void ADEMOD_SetGroupDelay(Handle_t ADEMOD_h, LX_ADEMOD_VideoStandard videoStd);
#endif //_ADEMOD_COMMON_USERDEF_H_
