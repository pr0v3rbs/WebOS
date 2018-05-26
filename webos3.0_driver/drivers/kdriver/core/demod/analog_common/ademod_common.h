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
**  Name: ademod_common.h
**
**  Description:     ABB demodulator's register access implementation.
**
**  Functions
**  Implemented:    LX_ADEMOD_Result  ADEMOD_WriteSub
**					LX_ADEMOD_Result  ADEMOD_ReadSub
**					LX_ADEMOD_Result  ADEMOD_WriteByte
**					LX_ADEMOD_Result  ADEMOD_ReadByte
**					LX_ADEMOD_Result  ADEMOD_WriteLong
**					LX_ADEMOD_Result  ADEMOD_ReadLong
**					LX_ADEMOD_Result  ADEMOD_WriteField
**					LX_ADEMOD_Result  ADEMOD_ReadField
**
**  References:     
**
**  Exports:        LX_ADEMOD_Result  ADEMOD_WriteSub
**					LX_ADEMOD_Result  ADEMOD_ReadSub
**					LX_ADEMOD_Result  ADEMOD_WriteByte
**					LX_ADEMOD_Result  ADEMOD_ReadByte
**					LX_ADEMOD_Result  ADEMOD_WriteLong
**					LX_ADEMOD_Result  ADEMOD_ReadLong
**					LX_ADEMOD_Result  ADEMOD_WriteField
**					LX_ADEMOD_Result  ADEMOD_ReadField
**
**  Dependencies:   ademod_common_demod.h for basic types.
**
**  Revision History:
**
**     Date        Author          Description
**  -------------------------------------------------------------------------
**   31-07-2013   Jeongpil Yun    Initial draft.
*****************************************************************************/
#ifndef _ADEMOD_COMMON_H_
#define _ADEMOD_COMMON_H_

#if defined( __cplusplus )
extern "C"                     /* Use "C" external linkage                  */
{
#endif

//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
#include "ademod_common_demod.h"
#include "demod_impl.h"

#define LX_ADEMOD_LOG_REGISTER_MAX_LIST		(50)
#define LX_ADEMOD_CACHE_FILL_BYTE			(0xFF)
#define LX_ADEMOD_CACHE_DIRECT_SET			(0x01)

/*
**  User options
*/

typedef enum LX_ADEMOD_User_Options_tag {
//	LX_ADEMOD_USER_OPTION_REG_CACHE		= 0x00000001L,
	LX_ADEMOD_USER_OPTION_BULK_WRITE_MODE	= 0x00000002L,
	LX_ADEMOD_USER_OPTION_BULK_READ_MODE	= 0x00000004L,
	LX_ADEMOD_USER_OPTION_READ_CACHE_MODE  = 0x00000008L,
	LX_ADEMOD_USER_OPTION_SLOW_SPEED_MODE  = 0x00000010L,
//	LX_ADEMOD_USER_OPTION_I2C_OPT			= 0x00000020L,
} LX_ADEMOD_User_Options;

typedef enum LX_ADEMOD_Cache_Options_tag {
	LX_ADEMOD_CACHE_OPTIONS_WRITE			= 0x00000001L,
	LX_ADEMOD_CACHE_OPTIONS_READ			= 0x00000002L,
	LX_ADEMOD_CACHE_OPTIONS_OPTIM			= 0x00000004L,
	LX_ADEMOD_CACHE_OPTIONS_I2C_MUTE		= 0x00000008L,  //jeongpil.yun for M14
} LX_ADEMOD_Cache_Options;

typedef enum LX_ADEMOD_Soft_Reset_tag {
	LX_ADEMOD_SOFTRESET_NOFIFOCLR			= 0x00000001L,	
} LX_ADEMOD_Soft_Reset;


/*****************************************************************************
**
**  Name: ADEMOD_WriteSub
**
**  Description:    Writes up to 4 bytes of data to the device using a two-wire serial bus.
**
**  Parameters:     hUserData  - User-specific I/O parameter that was
**                               passed to tuner's Open function.
**                  reg_addr   - Register Address.
**                  reg_data   - Data to be written to the device.
**                  cnt        - number of bytes to be written (from 1 to 4)
**
**  Returns:        status:
**                      retOK           - No errors
**
*****************************************************************************/
LX_ADEMOD_Result ADEMOD_WriteSub(Handle_t hUserData,
                    UINT32  reg_addr,
                    UINT32  reg_data,
                    UINT32  cnt);

LX_ADEMOD_Result ADEMOD_WriteSubX(Handle_t hUserData,
                    UINT32  reg_addr,
                    UINT32  reg_data,
                    UINT32  cnt,
					BOOL	update);

LX_ADEMOD_Result ADEMOD_WriteLongX(Handle_t hUserData,
                    UINT32  reg_addr,
                    UINT64  reg_data,
                    UINT32  cnt,
					BOOL	update);

/*****************************************************************************
**
**  Name: ADEMOD_ReadSub
**
**  Description:    Reads up to 4 bytes of data from the device using a two-wire serial bus.
**
**  Parameters:     hUserData  - User-specific I/O parameter that was
**                               passed to tuner's Open function.
**                  reg_addr   - Register Address.
**                  preg_data  - pointer to the Data read from the device.
**                  cnt        - number of bytes to read (from 1 to 4).
**
**  Returns:        status:
**                      retOK           - No errors
**
*****************************************************************************/
LX_ADEMOD_Result ADEMOD_ReadSub(Handle_t hUserData,
                    UINT32  reg_addr,
                    UINT32  *preg_data,
                    UINT32  cnt);

/*****************************************************************************
**
**  Name: ADEMOD_WriteByte
**
**  Description:    Write one byte of data to the device using a two-wire serial bus.
**
**  Parameters:     hUserData  - User-specific I/O parameter that was
**                               passed to tuner's Open function.
**                  reg_addr   - Register Address.
**                  Data       - Data to be written to the  device
**
**  Returns:        status:
**                      retOK           - No errors
**
**  Notes:          You MUST provide code for this function to write a byte of data
**                  using the device's 2-wire serial bus.
**
**                  The hUserData parameter is a user-specific argument.
**                  If additional arguments are needed for the user's
**                  serial bus read/write functions, this argument can be
**                  used to supply the necessary information.
**                  The hUserData parameter is initialized in the Open function.
**
*****************************************************************************/
LX_ADEMOD_Result ADEMOD_WriteByte(Handle_t hUserData,
                    UINT32  reg_addr,
                    UINT8   Data);

/*****************************************************************************
**
**  Name: ADEMOD_ReadByte
**
**  Description:    Reads one byte of data from the device using a two-wire serial bus.
**
**  Parameters:     hUserData  - User-specific I/O parameter that was
**                               passed to tuner's Open function.
**                  reg_addr   - Register Address.
**                  pData      - pointer to the Data to be written to the
**                               device
**
**  Returns:        status:
**                      retOK           - No errors
**
**  Notes:          You MUST provide code for this function to read a byte of data
**                  using the device's 2-wire serial bus.
**
**                  The hUserData parameter is a user-specific argument.
**                  If additional arguments are needed for the user's
**                  serial bus read/write functions, this argument can be
**                  used to supply the necessary information.
**                  The hUserData parameter is initialized in the tuner's Open
**                  function.
**
*****************************************************************************/
LX_ADEMOD_Result ADEMOD_ReadByte(Handle_t hUserData,
                    UINT32  reg_addr,
                    UINT8   *pData);

/*****************************************************************************
**
**  Name: I2C_CounterClear
**
**  Description:    Clears I2C Write counter.
**
**  Parameters:     n - counter number.
**
**  Returns:        result
**
*****************************************************************************/
LX_ADEMOD_Result I2C_CounterClear(int n);

/*****************************************************************************
**
**  Name: I2C_CounterGet
**
**  Description:    Returns I2C Write counter.
**
**  Parameters:     n - counter number,
**					val - counter value to return
**
**  Returns:        I2C Write counter
**
*****************************************************************************/
LX_ADEMOD_Result  I2C_CounterGet(int n, UINT32* val);

LX_ADEMOD_Result RegisterCacheFlagsSet(void* pFlags);

/*****************************************************************************
**
**  Name: RegisterCacheBufferSet
**
**  Description:    Sets register cache pointer.
**
**  Parameters:     pCache     - Pointer to cache buffer.
**
**  Returns:        result
**
*****************************************************************************/
LX_ADEMOD_Result RegisterCacheBufferSet(void* pCache);

/*****************************************************************************
**
**  Name: RegisterCacheOptionsSet
**
**  Description:    Sets register cache options.
**
**  Parameters:     Options     - cache options.
**
**  Returns:        result
**
*****************************************************************************/
LX_ADEMOD_Result RegisterCacheOptionsSet(UINT32 Options);

/*****************************************************************************
**
**  Name: RegisterCacheOptionsGet
**
**  Description:    Gets register cache options.
**
**  Parameters:     pOptions     - pointer to cache options.
**
**  Returns:        result
**
*****************************************************************************/
LX_ADEMOD_Result RegisterCacheOptionsGet(UINT32* Options);
/*****************************************************************************
**
**  Name: RegisterCachePostProcess
**
**  Description:    Executes appropriate reset and FIFO clear sequence.
**
**  Parameters:     hUserData  - User-specific I/O parameter that was
**                               passed to demod's Open function.
**
**  Returns:        result
**
**  Note:			When I2C optimisation through register cache is enabled, the
**					soft reset anf FIFO clear reset sequence will be blocked by optimization.
**					Therefore this function is called in the end of Demodulate to
**					execute proper soft reset and clear FIFO.
*****************************************************************************/
LX_ADEMOD_Result RegisterCachePostProcess(Handle_t hUserData);


/*****************************************************************************
**
**  Name: ADEMOD_WriteLong
**
**  Description:    Writes up to 8 bytes of data to the device using a two-wire serial bus.
**
**  Parameters:     hUserData  - User-specific I/O parameter that was
**                               passed to tuner's Open function.
**                  reg_addr   - Register Address.
**                  reg_data   - Data to be written to the device.
**                  cnt        - number of bytes to be written (from 1 to 4)
**
**  Returns:        status:
**                      retOK           - No errors
**
*****************************************************************************/
LX_ADEMOD_Result ADEMOD_WriteLong(Handle_t hUserData,
                    UINT32  reg_addr,
                    UINT64  reg_data,
                    UINT32  cnt);

/*****************************************************************************
**
**  Name: ADEMOD_ReadLong
**
**  Description:    Reads up to 8 bytes of data from the device using a two-wire serial bus.
**
**  Parameters:     hUserData  - User-specific I/O parameter that was
**                               passed to tuner's Open function.
**                  reg_addr   - Register Address.
**                  preg_data  - pointer to the Data read from the device.
**                  cnt        - number of bytes to read (from 1 to 4).
**
**  Returns:        status:
**                      retOK           - No errors
**
*****************************************************************************/
LX_ADEMOD_Result ADEMOD_ReadLong(Handle_t hUserData,
                   UINT32 reg_addr,
                   UINT64 *preg_data,
                   UINT32 cnt);



/*****************************************************************************
**
**  Name: ADEMOD_WriteField
**
**  Description:    Writes a bit field specified by the mask.
**
**  Parameters:     hUserData  - User-specific I/O parameter that was
**                               passed to tuner's Open function.
**                  reg_addr   - Register Address.
**                  Data       - Data to be written to the  device
**                  Mask       - The mask for the bit field
**
**  Returns:        status:
**                      retOK           - No errors
**
*****************************************************************************/
LX_ADEMOD_Result ADEMOD_WriteField(Handle_t hUserData,
                    UINT32  reg_addr,
                    UINT8   Data,
					UINT8	Mask);

/*****************************************************************************
**
**  Name: ADEMOD_ReadField
**
**  Description:    Reads a bit field specified by the mask.
**
**  Parameters:     hUserData  - User-specific I/O parameter that was
**                               passed to tuner's Open function.
**                  reg_addr   - Register Address.
**                  pData      - Data to be read from the  device
**                  Mask       - The mask for the bit field
**
**  Returns:        status:
**                      retOK           - No errors
**
*****************************************************************************/
LX_ADEMOD_Result ADEMOD_ReadField(Handle_t hUserData,
                    UINT32  reg_addr,
                    UINT8   *pData,
					UINT8	Mask);



INT64 div64_64(INT64 dividend, INT64 divisor);

#if 0

/*****************************************************************************
**
**  Name: RegisterLogOpen
**
**  Description:    Opens register log.
**
**  Parameters:     hUserData  - User-specific I/O parameter that was
**                               passed to demod's Open function,
**					file_name - file name,
**					include_regs - 0- terminated list of registrs included to the log.
**					When NULL - all registers will be included.
**					exclude_regs - 0- terminated list of registrs excluded from the log.
**					When NULL - no registers will be included.
**					options - log print options.
**  Returns:        result
**
*****************************************************************************/
LX_ADEMOD_Result RegisterLogOpen(Handle_t hUserData, char* file_name, int *include_regs, int *exclude_regs, int options);

/*****************************************************************************
**
**  Name: RegisterLogClose
**
**  Description:    Opens register log.
**
**  Parameters:     hUserData  - User-specific I/O parameter that was
**                               passed to demod's Open function.
**  Returns:        result
**
*****************************************************************************/
LX_ADEMOD_Result RegisterLogClose(Handle_t hUserData);



/*****************************************************************************
**
**  Name: RegisterLogWrite
**
**  Description:    Writes a byte of data to the dump file.
**
**  Parameters:     hUserData  - User-specific I/O parameter that was
**                               passed to demod's Open function,
**                  reg_addr   - Register Address,
**                  reg_val    - Register value,
**					reg_size   - Register size in bytes.
**
**  Returns:        result
**
*****************************************************************************/
LX_ADEMOD_Result RegisterLogWrite(Handle_t hUserData, int reg_addr, UINT64 reg_val, int reg_size);
#endif


#if defined( __cplusplus )
}
#endif

#endif //_ADEMOD_COMMON_H_
