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
**  Name: ademod_m14_demod.c
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
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>			/**< printk() */
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
//#include <stdlib.h>                     /*  for NULL      */
#include "ademod_common.h"
#include "ademod_common_demod.h"
#include "ademod_common_userdef.h"
#include "ademod_m14_regaddress.h"

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_HostI2C_Write
**
**  Description:    Writes data to host I2C.
**
**  Parameters:     hDevice      - Device Handle returned by FmLg_Demod_Open,
**                  DevAddr      - Target device address,
**                  RegAddr      - Target register address,
**                  pData        - Pointer to data to be written,
**                  Size         - Size in bytes.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_HostI2C_Write(Handle_t hDevice, UINT8 DevAddr, UINT8 RegAddr, UINT8* pData, UINT32 Size)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
    pLX_ADEMOD_Demod_Instance pInstance = (pLX_ADEMOD_Demod_Instance)hDevice;
	UINT32 i;

	// write
	ADEMOD_WriteByte( pInstance->hUserData, TWA_MASTERCTRL, 0x01); // BUG1081, comment#3
	// start
	ADEMOD_WriteByte( pInstance->hUserData, TWA_MASTERSTART, 0x01);
	// device address
	ADEMOD_WriteByte( pInstance->hUserData, TWA_MASTERTXFER, DevAddr);

	// register address
	// ADEMOD_WriteByte( pInstance->hUserData, TWA_MASTERTXFER, RegAddr);
	// data
	for(i=0; i<Size; i++) {
		ADEMOD_WriteByte( pInstance->hUserData, TWA_MASTERTXFER, pData[i]);
    }
	// stop
	ADEMOD_WriteByte( pInstance->hUserData, TWA_MASTERSTART, 0x00);



    return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_HostI2C_Read(Handle_t hDevice, UINT8 DevAddr, UINT8 RegAddr, UINT8* pData, UINT32 Size)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
    pLX_ADEMOD_Demod_Instance pInstance = (pLX_ADEMOD_Demod_Instance)hDevice;
	UINT8 data;
	UINT32 i;

	// setting WrAck allows slave to ack
	ADEMOD_WriteByte( pInstance->hUserData, TWA_MASTERCTRL, 0x01);
	// start
	ADEMOD_WriteByte( pInstance->hUserData, TWA_MASTERSTART, 0x01);
	// device address
	ADEMOD_WriteByte( pInstance->hUserData, TWA_MASTERTXFER, DevAddr);
	// register address
	ADEMOD_WriteByte( pInstance->hUserData, TWA_MASTERTXFER, RegAddr);
	// start
	ADEMOD_WriteByte( pInstance->hUserData, TWA_MASTERSTART, 0x01);
	// device address with LSbit set to 1 for read
	ADEMOD_WriteByte( pInstance->hUserData, TWA_MASTERTXFER, (UINT8)(DevAddr | 0x01) );
	// clearing WrAck, means *we* ack now
	ADEMOD_WriteByte( pInstance->hUserData, TWA_MASTERCTRL, 0x00);
	// data
	for (i=0; i<Size; i++)
	{
		// when final read - need to switch back to WrAck = 1
		if (i == Size-1) {
			ADEMOD_WriteByte( pInstance->hUserData, TWA_MASTERCTRL, 0x01);
		}
		// cause the read to happen
		ADEMOD_WriteByte( pInstance->hUserData, TWA_MASTERTXFER, 0xFF);
		// read data
		ADEMOD_ReadByte( pInstance->hUserData, TWA_MASTERRDBACK, &data);
		pData[i] = data;
	}
	// stop
	ADEMOD_WriteByte( pInstance->hUserData, TWA_MASTERSTART, 0x00);

    return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_HostI2C_WriteX(Handle_t hDevice, UINT8 DevAddr, UINT32 RegAddr, UINT32 Options, UINT8* pData, UINT32 Size)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
    pLX_ADEMOD_Demod_Instance pInstance = (pLX_ADEMOD_Demod_Instance)hDevice;
	UINT32 i;


	if (!(Options & LX_ADEMOD_TWA_OPTION_IGNORE_DEV)) {
		ADEMOD_WriteByte( pInstance->hUserData, TWA_MASTERCTRL, 0x01); // BUG1081, comment#3
	}
	// start
	ADEMOD_WriteByte( pInstance->hUserData, TWA_MASTERSTART, 0x01);
	// device address
	if (!(Options & LX_ADEMOD_TWA_OPTION_IGNORE_DEV)) {
		ADEMOD_WriteByte( pInstance->hUserData, TWA_MASTERTXFER, DevAddr);
	}

	// optionally read bit 1 (RdAck) of register 0x800 (TWA_MASTERCTRL) and return the status
	if (Options & LX_ADEMOD_TWA_OPTION_SLAVE_ACK) {
		UINT8 temp = 0;
		ADEMOD_ReadByte( pInstance->hUserData, TWA_MASTERCTRL, &temp);
		if (temp & 0x2) { // write has not been acknowledged
			status = retErr;
		}
	}


	if (Options & LX_ADEMOD_TWA_OPTION_IGNORE_DEV) {
		RegAddr = (RegAddr << 2) + 0x01;
		ADEMOD_WriteByte( pInstance->hUserData, TWA_MASTERTXFER, (UINT8)RegAddr);
	}
	// data
	for(i=0; i<Size; i++) {
		ADEMOD_WriteByte( pInstance->hUserData, TWA_MASTERTXFER, pData[i]);
    }
	// stop
	ADEMOD_WriteByte( pInstance->hUserData, TWA_MASTERSTART, 0x00);



    return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_HostI2C_ReadX
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
**                  pData        - Pointer to data to be read,
**                  Size         - Size in bytes.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_HostI2C_ReadX(Handle_t hDevice, UINT8 DevAddr, UINT32 RegAddr, UINT32 Options, UINT8* pData, UINT32 Size)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
    pLX_ADEMOD_Demod_Instance pInstance = (pLX_ADEMOD_Demod_Instance)hDevice;
	UINT8 data;
	UINT32 i;

	// setting WrAck allows slave to ack
	if (!(Options & LX_ADEMOD_TWA_OPTION_IGNORE_DEV)) {
		ADEMOD_WriteByte( pInstance->hUserData, TWA_MASTERCTRL, 0x01); // BUG1081, comment#3
	}
	// start
	ADEMOD_WriteByte( pInstance->hUserData, TWA_MASTERSTART, 0x01);
	// device address - specifically for Cayman
	if (Options & LX_ADEMOD_TWA_OPTION_IGNORE_DEV) {
		RegAddr = (RegAddr << 2) + 0x03;
	}
	else {
		ADEMOD_WriteByte( pInstance->hUserData, TWA_MASTERTXFER, DevAddr);
	}

	if (Options & LX_ADEMOD_TWA_OPTION_SPC_RD_REG) {
		// Special I2C read requires writing 0xFB before register address
		ADEMOD_WriteByte( pInstance->hUserData, TWA_MASTERTXFER, 0xFB);
	}

	// register address
	ADEMOD_WriteByte( pInstance->hUserData, TWA_MASTERTXFER, (UINT8)RegAddr);
	// start
	if (!(Options & LX_ADEMOD_TWA_OPTION_IGNORE_DEV)) {
		ADEMOD_WriteByte( pInstance->hUserData, TWA_MASTERSTART, 0x01);
		// device address with LSbit set to 1 for read
		ADEMOD_WriteByte( pInstance->hUserData, TWA_MASTERTXFER, (UINT8)(DevAddr | 0x01) );
		// clearing WrAck, means *we* ack now
		ADEMOD_WriteByte( pInstance->hUserData, TWA_MASTERCTRL, 0x00);
	}
	if (Options & LX_ADEMOD_TWA_OPTION_IGNORE_DEV) {
		ADEMOD_WriteByte( pInstance->hUserData, TWA_MASTERCTRL, 0x00);
	}
	// data
	for (i=0; i<Size; i++)
	{
		// when final read - need to switch back to WrAck = 1
		if (i == Size-1) {
			ADEMOD_WriteByte( pInstance->hUserData, TWA_MASTERCTRL, 0x01);
		}
		// cause the read to happen
		ADEMOD_WriteByte( pInstance->hUserData, TWA_MASTERTXFER, 0xFF);
		// read data
		ADEMOD_ReadByte( pInstance->hUserData, TWA_MASTERRDBACK, &data);
		pData[i] = data;
	}
	// stop
	ADEMOD_WriteByte( pInstance->hUserData, TWA_MASTERSTART, 0x00);

    return status;
}
