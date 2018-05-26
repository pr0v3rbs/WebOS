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
**  Name: ademod_common.c
**
**  Description:    ABB demodulator's register access implementation.
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
**					LX_ADEMOD_Result  ADEMOD_WriteArray
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
**					LX_ADEMOD_Result  ADEMOD_WriteArray
**
**  Dependencies:   ademod_common_demod.h for basic types.
**
**  Revision History:
**
**     Date        Author          Description
**  -------------------------------------------------------------------------
**   31-07-2013   Jeongpil Yun    Initial draft.
*****************************************************************************/
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>			
#include <linux/slab.h>			 	/**< kmalloc() */
#include <linux/fs.h> 				/**< everything\ldots{} */
#include <linux/types.h>		 	/**< size_t */
#include <linux/fcntl.h>			/**< O_ACCMODE */
#include <asm/uaccess.h>
#include <linux/ioport.h>			/**< For request_region, check_region etc */
#include <asm/io.h>					/**< For ioremap_nocache */
#include <linux/workqueue.h>		/**< For working queue */
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <asm/div64.h> //do_div

#include "ademod_common.h"
#include "ademod_common_userdef.h"
#include "ademod_common_device.h"

#define MAX_I2C_COUNTERS 2

//static FILE *outfile = NULL;
//static int *include_regs_list = NULL;
//static int *exclude_regs_list = NULL;
static UINT32 I2C_write_counter[MAX_I2C_COUNTERS] = { 0, 0 };
static UINT8* pCacheBuf = NULL;
static UINT8* pCacheFlags = NULL;
static UINT32 CacheOptions = 0;

/*
**  Local Function Prototypes - not available for external access.
*/
//static LX_ADEMOD_Result RegisterLogWrite(int reg_addr, UINT64 reg_val, int reg_size);
static LX_ADEMOD_Result RegisterCacheWrite(int reg_addr, UINT64 reg_val, int reg_size, BOOL* Update);
static LX_ADEMOD_Result RegisterCacheWriteX(int reg_addr, UINT64 reg_val, int reg_size, BOOL* Update);
static LX_ADEMOD_Result RegisterCacheRead(UINT32 reg_addr, UINT32* preg_val, UINT32 reg_size);
/*
**  Implementation.
*/

/*****************************************************************************
**
**  Name: ADEMOD_WriteSub
**
**  Description:    Writes up to 4 bytes of data to the device using a two-wire serial bus.
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
LX_ADEMOD_Result ADEMOD_WriteSub(Handle_t hUserData,
                    UINT32  reg_addr,
                    UINT32  reg_data,
                    UINT32  cnt)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
	int i = 0;

	BOOL Update = TRUE;
//	status |= RegisterLogWrite(reg_addr, reg_data, cnt);
	status |= RegisterCacheWrite(reg_addr, reg_data, cnt, &Update);
	if ((CacheOptions & LX_ADEMOD_CACHE_OPTIONS_OPTIM) &&
		(CacheOptions & LX_ADEMOD_CACHE_OPTIONS_WRITE) && !Update) {
		// if optimization is on and cache data did not change,
		// then skip actual I2C transaction
		return retOK;
	}
	status |= ADEMOD_WriteAutoIncrement(hUserData, reg_addr, (UINT32)reg_data, cnt);
	for (i = 0; i < MAX_I2C_COUNTERS; i++) {
		I2C_write_counter[i] += cnt;
	}

	return status;
}


/*****************************************************************************
**
**  Name: ADEMOD_WriteSubX
**
**  Description:    Writes up to 4 bytes of data to the device using a two-wire serial bus.
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
**  Note:			When called with UpdateNow = FALSE will optimize direct register
**					writes using cache.
*****************************************************************************/
LX_ADEMOD_Result ADEMOD_WriteSubX(Handle_t hUserData,
                    UINT32  reg_addr,
                    UINT32  reg_data,
                    UINT32  cnt,
					BOOL	update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
	int i = 0;

	BOOL Update = TRUE;
//	status |= RegisterLogWrite(reg_addr, reg_data, cnt);
	if (!update) {
		status |= RegisterCacheWriteX(reg_addr, reg_data, cnt, &Update);
		if ((CacheOptions & LX_ADEMOD_CACHE_OPTIONS_OPTIM) &&
			(CacheOptions & LX_ADEMOD_CACHE_OPTIONS_WRITE) && !Update) {
			// if optimization is on and cache data did not change,
			// then skip actual I2C transaction
			return retOK;
		}
	}
	status |= ADEMOD_WriteAutoIncrement(hUserData, reg_addr, (UINT32)reg_data, cnt);
	for (i = 0; i < MAX_I2C_COUNTERS; i++) {
		I2C_write_counter[i] += cnt;
	}

	return status;
}


/*****************************************************************************
**
**  Name: ADEMOD_ReadSub
**
**  Description:    Reads up to 4 bytes of data from the device using a two-wire serial bus.
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
LX_ADEMOD_Result ADEMOD_ReadSub(Handle_t hUserData,
                   UINT32 reg_addr,
                   UINT32 *preg_data,
                   UINT32 cnt)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
	UINT32 Data;

	if (cnt > LX_ADEMOD_MAX_REGISTER_LENGTH) {
		return retErrBadParam;
	}

	if (CacheOptions & LX_ADEMOD_CACHE_OPTIONS_READ) { // read from cache
		status |= RegisterCacheRead(reg_addr, preg_data, cnt);
		return status;
	}

	status = ADEMOD_ReadAutoIncrement(hUserData, reg_addr, &Data, cnt);
	if (status == retOK) {
		*preg_data = (UINT32)Data;
	}

	return status;
}


/*****************************************************************************
**
**  Name: ADEMOD_WriteByte
**
**  Description:    Write one byte of data to the device using a two-wire serial bus.
**
**  Parameters:     hUserData  - User-specific I/O parameter that was
**                               passed to demod's Open function.
**                  reg_addr   - Register Address.
**                  reg_data   - Data to be written to the  device
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
                    UINT8   reg_data)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
	int i = 0;

	BOOL Update = TRUE;
//	status |= RegisterLogWrite(reg_addr, reg_data, 1);
	status |= RegisterCacheWrite(reg_addr, reg_data, 1, &Update);
	if ((CacheOptions & LX_ADEMOD_CACHE_OPTIONS_OPTIM) &&
		(CacheOptions & LX_ADEMOD_CACHE_OPTIONS_WRITE) && !Update) {
		// if optimization is on and cache data did not change,
		// then skip actual I2C transaction
		return retOK;
	}
	status |= ADEMOD_WriteAutoIncrement(hUserData, reg_addr, (UINT32)reg_data, 1);
	for (i = 0; i < MAX_I2C_COUNTERS; i++) {
		I2C_write_counter[i] ++;
	}

	return status;
}


/*****************************************************************************
**
**  Name: ADEMOD_ReadByte
**
**  Description:    Reads one byte of data from the device using a two-wire serial bus.
**
**  Parameters:     hUserData  - User-specific I/O parameter that was
**                               passed to demod's Open function.
**                  reg_addr   - Register Address.
**                  preg_data  - pointer to the Data to be written to the device.
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
                    UINT8   *preg_data)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
	UINT32 Data;
//	BOOL done = FALSE;

	if (CacheOptions & LX_ADEMOD_CACHE_OPTIONS_READ) { // read from cache
		status |= RegisterCacheRead(reg_addr, (UINT32*)preg_data, 1);
		return status;
	}
	status |= ADEMOD_ReadAutoIncrement(hUserData, reg_addr, &Data, 1);
	if (status == retOK) {
		*preg_data = (UINT8)Data;
	}

	return status;
}

/*****************************************************************************
**
**  Name: ADEMOD_WriteField
**
**  Description:    Writes a bit field specified by the mask.
**
**  Parameters:     hUserData  - User-specific I/O parameter that was
**                               passed to demod's Open function.
**                  reg_addr   - Register Address.
**                  reg_data   - Data to be written to the register.
**                  Mask       - The mask for the bit field
**
**  Returns:        status:
**                      retOK           - No errors
**
*****************************************************************************/
LX_ADEMOD_Result ADEMOD_WriteField(Handle_t hUserData,
                    UINT32  reg_addr,
                    UINT8   reg_data,
					UINT8	Mask)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
	UINT8 CurrentData = 0;

	status |= ADEMOD_ReadByte(hUserData, reg_addr, &CurrentData);

	// Discard the bits indicated by the Mask
    CurrentData &= ~Mask;

    // Combine with the new data
    CurrentData |= (reg_data & Mask);

	status |= ADEMOD_WriteByte(hUserData, reg_addr, CurrentData);

    return status;
}


/*****************************************************************************
**
**  Name: ADEMOD_ReadField
**
**  Description:    Reads a bit field specified by the mask.
**
**  Parameters:     hUserData  - User-specific I/O parameter that was
**                               passed to demod's Open function.
**                  reg_addr   - Register Address.
**                  preg_data  - Data to be read from the device
**                  Mask       - The mask for the bit field
**
**  Returns:        status:
**                      retOK           - No errors
**
*****************************************************************************/
LX_ADEMOD_Result ADEMOD_ReadField(Handle_t hUserData,
                    UINT32  reg_addr,
                    UINT8   *preg_data,
					UINT8	Mask)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
	UINT8 CurrentData = 0;

	status |= ADEMOD_ReadByte(hUserData, reg_addr, &CurrentData);

	// Discard the other bits
	CurrentData &= Mask;

	*preg_data = CurrentData;

    return status;
}





/*****************************************************************************
**
**  Name: ADEMOD_WriteLong
**
**  Description:    Writes up to 8 bytes of data to the device using a two-wire serial bus.
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
LX_ADEMOD_Result ADEMOD_WriteLong(Handle_t hUserData,
                    UINT32  reg_addr,
                    UINT64  reg_data,
                    UINT32  cnt)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
	int i = 0;
	BOOL Update = TRUE;
//	status |= RegisterLogWrite(reg_addr, reg_data, cnt);
	if (CacheOptions & LX_ADEMOD_CACHE_OPTIONS_WRITE) { // write to cache
		status |= RegisterCacheWrite(reg_addr, reg_data, cnt, &Update);
	}
	if ((CacheOptions & LX_ADEMOD_CACHE_OPTIONS_OPTIM) && !Update) {
		// if optimization is on and cache data did not change,
		// then skip actual I2C transaction
		return retOK;
	}

	if (cnt < LX_ADEMOD_MAX_REGISTER_LENGTH)
	{
		status |= ADEMOD_WriteAutoIncrement(hUserData, reg_addr, reg_data, cnt);
	}
	else {
		// 5-byte registers require special treatment
		status |= ADEMOD_WriteAutoIncrement(hUserData, reg_addr, (UINT32)(reg_data & 0xffffffff), 4);
		// Write MSByte as a separate transaction
		status |= ADEMOD_WriteAutoIncrement(hUserData, reg_addr + 4, (UINT32)(reg_data >> 32), cnt - LX_ADEMOD_MAX_REGISTER_LENGTH + 1);
	}
	for (i = 0; i < MAX_I2C_COUNTERS; i++) {
		I2C_write_counter[i] += cnt;
	}

	return status;
}


/*****************************************************************************
**
**  Name: ADEMOD_WriteLongX
**
**  Description:    Writes up to 8 bytes of data to the device using a two-wire serial bus.
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
**  Note:			When called with UpdateNow = FALSE will optimize direct register
**					writes using cache.
*****************************************************************************/
LX_ADEMOD_Result ADEMOD_WriteLongX(Handle_t hUserData,
                    UINT32  reg_addr,
                    UINT64  reg_data,
                    UINT32  cnt,
					BOOL	update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
	int i = 0;
	BOOL Update = TRUE;
//	status |= RegisterLogWrite(reg_addr, reg_data, cnt);
	if (CacheOptions & LX_ADEMOD_CACHE_OPTIONS_WRITE) { // write to cache
		status |= RegisterCacheWrite(reg_addr, reg_data, cnt, &Update);
	}
	if ((CacheOptions & LX_ADEMOD_CACHE_OPTIONS_OPTIM) && !Update) {
		// if optimization is on and cache data did not change,
		// then skip actual I2C transaction
		return retOK;
	}

	if (cnt < LX_ADEMOD_MAX_REGISTER_LENGTH)
	{
		status |= ADEMOD_WriteAutoIncrement(hUserData, reg_addr, reg_data, cnt);
	}
	else {
		// 5-byte registers require special treatment
		status |= ADEMOD_WriteAutoIncrement(hUserData, reg_addr, (UINT32)(reg_data & 0xffffffff), 4);
		// Write MSByte as a separate transaction
		status |= ADEMOD_WriteAutoIncrement(hUserData, reg_addr + 4, (UINT32)(reg_data >> 32), cnt - LX_ADEMOD_MAX_REGISTER_LENGTH + 1);
	}
	for (i = 0; i < MAX_I2C_COUNTERS; i++) {
		I2C_write_counter[i] += cnt;
	}

	return status;
}


/*****************************************************************************
**
**  Name: ADEMOD_ReadLong
**
**  Description:    Reads up to 8 bytes of data from the device using a two-wire serial bus.
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
LX_ADEMOD_Result ADEMOD_ReadLong(Handle_t hUserData,
                   UINT32 reg_addr,
                   UINT64 *preg_data,
                   UINT32 cnt)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
	UINT32 reg_val = 0;

	if (cnt > LX_ADEMOD_MAX_REGISTER_LENGTH) {
		return retErrBadParam;
	}

	if (CacheOptions & LX_ADEMOD_CACHE_OPTIONS_READ) { // read from cache
		status |= RegisterCacheRead(reg_addr, (UINT32*)preg_data, cnt);
		return status;
	}

	if (cnt < LX_ADEMOD_MAX_REGISTER_LENGTH)
	{
		status |= ADEMOD_ReadAutoIncrement(hUserData, reg_addr, (UINT32 *)&reg_val, 4);//LGIT 091010
		*preg_data = reg_val;
	}
	else {
		UINT32 msb = 0;
		// 5-byte registers require special treatment
		status |= ADEMOD_ReadAutoIncrement(hUserData, reg_addr, (UINT32 *)&reg_val, 4);//LGIT 091010
		// Read MSByte as a separate transaction
		status |= ADEMOD_ReadAutoIncrement(hUserData, reg_addr + 4, (UINT32 *)&msb, cnt - LX_ADEMOD_MAX_REGISTER_LENGTH + 1);//LGIT 091010
		*preg_data = (UINT64)reg_val + ((UINT64)msb << 32);
	}

	return status;
}

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
LX_ADEMOD_Result RegisterLogOpen(Handle_t hUserData, char* file_name, int *include_regs, int *exclude_regs, int options)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
	if (outfile) {
		// previous file must be closed
		fclose (outfile);
		outfile = NULL;
	}

	outfile = fopen (file_name, "w");
	if (include_regs != NULL) {
		include_regs_list = include_regs;
	}
	else {
		include_regs_list = NULL;
	}
	if (exclude_regs != NULL) {
		exclude_regs_list = exclude_regs;
	}
	else {
		exclude_regs_list = NULL;
	}
	return status;
}

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
LX_ADEMOD_Result RegisterLogClose(Handle_t hUserData)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
	if (outfile != NULL) {
		fclose (outfile);
		outfile = NULL;
		include_regs_list = NULL;
		exclude_regs_list = NULL;
	}
	return status;
}
#endif

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
LX_ADEMOD_Result I2C_CounterClear(int n)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (n >= MAX_I2C_COUNTERS) {
		return retErrBadParam;
	}

	I2C_write_counter[n] = 0;

	return status;
}

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
LX_ADEMOD_Result  I2C_CounterGet(int n, UINT32* val)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (n >= MAX_I2C_COUNTERS) {
		return retErrBadParam;
	}

	*val = I2C_write_counter[n];

	return status;
}
#if 0

/*****************************************************************************
**
**  Name: RegisterLogWrite
**
**  Description:    Writes a byte of data to the log file.
**
**  Parameters:     reg_addr   - Register Address,
**                  reg_val    - Register value,
**					reg_size   - Register size in bytes.
**
**  Returns:        result
**
*****************************************************************************/
static LX_ADEMOD_Result RegisterLogWrite(int reg_addr, UINT64 reg_val, int reg_size)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */


	int i;

	if (outfile == NULL) {
		return status;
	}

	// Check if register is selected for printout
	if (include_regs_list)
	{
		for (i = 0; i < LX_ADEMOD_LOG_REGISTER_MAX_LIST; i++ ) {
			if ((include_regs_list[i] < 0) /* end of the list */ || (include_regs_list[i] == reg_addr /* register found */))
				break;
		}
		if (include_regs_list[i] < 0) {
			return status; // register is not found in include list
		}
	}

	if (exclude_regs_list)
	{
		for (i = 0; i < LX_ADEMOD_LOG_REGISTER_MAX_LIST; i++ ) {
			if ((exclude_regs_list[i] < 0) /* end of the list */ || (exclude_regs_list[i] == reg_addr /* register found */))
				break;
		}
		if (exclude_regs_list[i] == reg_addr) {
			return status; // register is found in exclude list
		}
	}

	if (outfile != NULL) {

		if (reg_size < LX_ADEMOD_MAX_REGISTER_LENGTH)
		{
			fprintf (outfile, "\t{0x%4X, 0x%X, %d},\n", reg_addr, (UINT32)reg_val, reg_size); //LGIT 091010
		}
		else {
			// 5-byte registers require special treatment
			fprintf (outfile, "\t{0x%4X, 0x%X, %d},\n", reg_addr, (UINT32)(reg_val & 0xffffffff), 4);//LGIT 091010
			// Write MSByte as a separate transaction
			fprintf (outfile, "\t{0x%4X, 0x%X, %d},\n", reg_addr + 4, (UINT32)(reg_val >> 32), reg_size - LX_ADEMOD_MAX_REGISTER_LENGTH + 1);//LGIT 091010
		}
	}
	return status;
}

#endif

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
LX_ADEMOD_Result RegisterCacheBufferSet(void* pCache)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
	if (pCache == 0) { // bad cache pointer
		return retErrBadPtr;
	}
	pCacheBuf = (UINT8*)pCache;
	return status;
}


/*****************************************************************************
**
**  Name: RegisterCacheFlagsSet
**
**  Description:    Sets register flags pointer.
**
**  Parameters:     pFlags     - Pointer to cache flags.
**
**  Returns:        result
**
*****************************************************************************/
LX_ADEMOD_Result RegisterCacheFlagsSet(void* pFlags)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
	if (pFlags == 0) { // bad cache pointer
		return retErrBadPtr;
	}
	pCacheFlags = (UINT8*)pFlags;
	return status;
}


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
LX_ADEMOD_Result RegisterCacheOptionsSet(UINT32 Options)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
	CacheOptions = Options;
	return status;
}


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
LX_ADEMOD_Result RegisterCacheOptionsGet(UINT32* Options)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
	*Options = CacheOptions;
	return status;
}


/*****************************************************************************
**
**  Name: RegisterCacheWrite
**
**  Description:    Writes register value to the register cache.
**
**  Parameters:     reg_addr   - Register Address,
**                  reg_val    - Register value,
**					reg_size   - Register size in bytes,
**					Update	   - 1 - new value written to cash, 0 - same value.
**
**  Returns:        result
**
*****************************************************************************/
LX_ADEMOD_Result RegisterCacheWrite(int reg_addr, UINT64 reg_val, int reg_size, BOOL* Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
	int count = 0;
	int i = 0;
//	int diff = 0;

	*Update = TRUE;

	if (pCacheBuf == 0) { // bad cache pointer
		return retErrBadPtr;
	}

	if ((reg_addr + reg_size) >= MAX_REG_CACHE_SIZE) {
		return retUserSizeTooBig;
	}

	*Update = FALSE;

	if (pCacheFlags[reg_addr] & LX_ADEMOD_CACHE_DIRECT_SET) {
		return status;
	}
	if ((reg_addr == 0x404) || (reg_addr == 0x041D)) {
		// there is no need to execute soft reset here since optimization might block all actual register writes,
		// so reset sequence is no longer needed. Call PostProcess in the end of the sequence if you need to execute
		// reset of FIFO clear
		return status;
	}
	for ( i = 0; i < reg_size; i++) {
		if (pCacheBuf[(reg_addr + i) & 0xffff] == (UINT8)((reg_val >> (8 * i)) & 0xff)) {
   			if (pCacheBuf[(reg_addr + i) & 0xffff] == LX_ADEMOD_CACHE_FILL_BYTE) { // Bug 1370: System initialization fails when I2C optimization is enabled
  				// do not optimize the register with the value equal to cache filler
 				count++;
			}
		}
		else {
			pCacheBuf[(reg_addr + i) & 0xffff]  = (UINT8)((reg_val >> (8 * i)) & 0xff);
			*Update = TRUE;
		}
	}
 	if (count == reg_size) {
  		*Update = TRUE;
	}

	return status;
}


/*****************************************************************************
**
**  Name: RegisterCacheWriteX
**
**  Description:    Writes register value to the register cache.
**
**  Parameters:     reg_addr   - Register Address,
**                  reg_val    - Register value,
**					reg_size   - Register size in bytes,
**					Update	   - 1 - new value written to cash, 0 - same value.
**
**  Returns:        result
**
**  Note:			When called with UpdateNow = FALSE will optimize direct register
**					writes using cache.
*****************************************************************************/
LX_ADEMOD_Result RegisterCacheWriteX(int reg_addr, UINT64 reg_val, int reg_size, BOOL* Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
	int count = 0;
	int i = 0;
//	int diff = 0;

	*Update = TRUE;

	if (pCacheBuf == 0) { // bad cache pointer
		return retErrBadPtr;
	}

	if ((reg_addr + reg_size) >= MAX_REG_CACHE_SIZE) {
		return retUserSizeTooBig;
	}

	*Update = FALSE;

	for ( i = 0; i < reg_size; i++) {
		if (pCacheFlags) {
			pCacheFlags[reg_addr + i] |= LX_ADEMOD_CACHE_DIRECT_SET;
		}
		if (pCacheBuf[(reg_addr + i) & 0xffff] == (UINT8)((reg_val >> (8 * i)) & 0xff)) {
   			if (pCacheBuf[(reg_addr + i) & 0xffff] == LX_ADEMOD_CACHE_FILL_BYTE) { // Bug 1370: System initialization fails when I2C optimization is enabled
  				// do not optimize the register with the value equal to cache filler
 				count++;
			}
		}
		else {
			pCacheBuf[(reg_addr + i) & 0xffff]  = (UINT8)((reg_val >> (8 * i)) & 0xff);
			*Update = TRUE;
		}
	}
 	if (count == reg_size) {
  		*Update = TRUE;
	}
	return status;
}

/*****************************************************************************
**
**  Name: RegisterCacheRead
**
**  Description:    Reads register value from register cache.
**
**  Parameters:     reg_addr   - Register Address,
**                  reg_val    - Register value,
**					reg_size   - Register size in bytes.
**
**  Returns:        result
**
*****************************************************************************/
LX_ADEMOD_Result RegisterCacheRead(UINT32 reg_addr, UINT32* preg_val, UINT32 reg_size)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
	UINT8 temp;
	int i = 0;

	if (pCacheBuf == 0) { // bad cache pointer
		return retErrBadPtr;
	}

	for (i = 0; i < reg_size; i++) {
		temp = (UINT8)pCacheBuf[reg_addr++ & 0xffff];
		*preg_val += temp << (8*i);
	}
	return status;
}


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
LX_ADEMOD_Result RegisterCachePostProcess(Handle_t hUserData)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
	int i = 0;
	if (I2C_write_counter[1]) { // this counter is reserved for demod sequence. If no registers have been written, do not invoke reset
		// soft reset
		status |= ADEMOD_WriteAutoIncrement(hUserData, 0x0404, 0xc0, 1);
		status |= ADEMOD_WriteAutoIncrement(hUserData, 0x0404, 0xDF, 1);
		// FIFO clear
 		status |= ADEMOD_WriteAutoIncrement(hUserData, 0x041D, 0xFF, 1); // fifo clear is ON
 		status |= ADEMOD_WriteAutoIncrement(hUserData, 0x041D, 0x00, 1); // fifo clear is OFF
		for (i = 0; i < MAX_I2C_COUNTERS; i++) {
			I2C_write_counter[i] += 4;
		}
	}
	return status;
}

INT64 div64_64(INT64 dividend, INT64 divisor)
 {
    SINT32  d;
	SINT32 high;
	unsigned int shift;
  
    high = (divisor >> 32);
	
    if (high) {
        shift = fls(high);
        d = divisor >> shift;
        dividend >>= shift;
   } else
        d = divisor;

   do_div(dividend, d);

   return dividend;
}


/*
**  Local Function implementation.
*/


