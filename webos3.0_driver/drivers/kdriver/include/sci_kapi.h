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

/** @file sci_kapi.h
 *
 *  application interface header for sci device
 *
 *  @author      Juhee Park
 *  @version		1.0
 *  @date		2011.06.06
 *
 *  @addtogroup lg1152_sci
 *	@{
 */

#ifndef	_SCI_KAPI_H_
#define	_SCI_KAPI_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include "base_types.h"

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
#define	SCI_IOC_MAGIC		'S'
/**
@name FBDEV IOCTL List
IOCTL list for osd device.

 @{
 @def SCI_IOW_WRITE_REG
 	Reads chip revision information.
 	Just get chiop revision information. To support automatic changing behavior of device driver or middleware by this ioctl.
	@param LX_CHIP_REV_INFO_T [OUT] chip version data
	@return RET_OK

 @def SCI_IOW_READ_REG
	@param pointer to LX_FBDEV_PALETTE_T
 	@return RET_OK for success, none zero for otherwise

 @def SCI_IOW_ACTIVATE
 	get device configuration ( this IOCTL overrides FBIOGET_VSCREENINFO )
	@param [OUT] pointer to LX_FBDEV_CONFIG_T
  	@return RET_OK for success, none zero for otherwise

 @def SCI_IOW_DEACTIVATE
 	update device configuration ( this IOCTL overrides FBIOPUT_VSCREENINFO )
 	@param [IN] pointer to LX_FBDEV_CONFIG_T
 	@return RET_OK for success, none zero for otherwise

 @def SCI_IOR_GET_STATUS
	@brief get current position information of FB device.
	@param LX_POSITION_T [OUT] position information of FB device
 	@return RET_OK for success, none zero for otherwise

 @def SCI_IOW_SET_VOLTAGE
 	@brief set new position information of FB device.
	@param LX_POSITION_T [in] position information of FB device
 	get new position information of FB device.

 	@return RET_OK for success, none zero for otherwise

 @def SCI_IOW_TRANSMIT
 	@brief get the physical memory information of the FB device.
 	@param LX_FBDEV_PHY_MEMORY_T [out] memory information

 	@return RET_OK for success, none zero for otherwise

	@note Front and back screen is changed every pan_display call.
		  So you should call this ioctl when you need to access OSD.
*/

/*----------------------------------------------------------------------------------------
	IO comand naming rule  : MODULE_IO[R][W]_COMMAND
----------------------------------------------------------------------------------------*/
typedef struct
{
	UINT32 regIndex;    // register Index
	UINT32 data;        // data
} LX_SCI_REG;

typedef enum
{
	SCI_STATUS_READY,       // SCI is Ready
	SCI_STATUS_DETECTED,    // SCI detected a card inserted
	SCI_STATUS_ACTIVE,      // SCI activated
	SCI_STATUS_FAIL         // SCI failed
} LX_SCI_STATUS;

typedef enum
{
	SCI_VOLTAGE_3V,         // 3V
	SCI_VOLTAGE_5V,         // 5V
	SCI_VOLTAGGE_UNKNOWN    // Unknown
} LX_SCI_VOLTAGE;

typedef struct
{
	UINT8 nad;              // node address
	UINT8 pcb;              // program control block
	UINT8 len;              // block length
	UINT8 reseved;
	UINT8* buf;             // information buffer
} LX_SCI_BLOCK;

typedef struct
{
	LX_SCI_BLOCK txBuf;     // tx buffer
	LX_SCI_BLOCK rxBuf;     // rx buffer
} LX_SCI_TRANSMIT;


#define SCI_MAX_ATR_DATA_BUFFER_LENGTH     33

/* the ATR can be at most 1+32 characters ... we store the raw data
 * in "data" and the decoded data in the rest of the fields pulled
 * appart so that we don't have to mess with ATR logic to get at
 * the fields. There is redundant information naturally.
 */
typedef struct
{
	UINT8 data[32+1];			/* data buffer */
	//UINT8 data_count;			/* count of data */

	UINT8 TS;                   // Ts
	UINT8 T0;                   // T0

	UINT8 TCK;                  // Tck
	UINT8 TCK_sent;  /* transmitted TCK */
	UINT8 TCK_calc;  /* calculated TCK */

	/* there are limits to how many of TA[i] you can have but
	* is this documented somewhere - I'm assuming that 3 is
	* the max for any of the chars
	*
	*/
	UINT8 TA[4];						// TA data buffer
	UINT8 TA_count;						// count of TA data
	UINT8 TB[4];						// TB data buffer
	UINT8 TB_count;						// count of TB data
	UINT8 TC[4];						// TC data buffer
	UINT8 TC_count;						// count of TC data

	/* we don't store TD bytes as there isn't anything in them
	* that is of interest except the transmission protocol currently
	* in use which we extract separately - however I might just be
	* wrong as there are cards that can support lots of different
	* protocols which are probably indicated via multiple TD bytes
	*/

	/* historical bytes are implicitly limited by the bounds of
	* the total message size
	*/
	UINT8 historical[32];				// historical buffer
	UINT8 historical_count;			// count of historical buffer

	/* now for the redundant interpretation of the above data */
	UINT8 conversion;			// 0 : inverse conversion , 1 : direct conversion
	

	/* transmission protocol extracted from TD bytes */
	UINT8 transmission_protocol;		// 0 : byte mode, 1 : block mode

    UINT8 T;
	UINT16 FI;							// FI value
	UINT8 DI;							// DI value
	UINT8 baud_rate;					// baud rate
	UINT8 guard_time;                   // guard time
	UINT8 crc;							// data check for transmission_protocol = 1 mode -> 0 : LRC, 1 : CRC
	UINT8 ifsc;                         // maximum information field size for card
	UINT8 bwi;                          // block waiting time integer
	UINT8 cwi;                          // character waiting time integer
	UINT8 power;						// power voltage -> SCI_VOLTAGE_3V: 3V, SCI_VOLTAGE_5V : 5V
	UINT8 rsvd;							// reserved byte
}LX_SCI_ATR_T;

#define SCI_IO_RESET			_IO(SCI_IOC_MAGIC, 0x00)
#define SCI_IO_ACTIVATE		    _IO(SCI_IOC_MAGIC, 0x01)
#define SCI_IO_DEACTIVATE		_IO(SCI_IOC_MAGIC, 0x02)

#define SCI_IOWR_TRANSMIT		_IOWR(SCI_IOC_MAGIC, 0x03, LX_SCI_TRANSMIT)

#define SCI_IO_GET_ATR		    _IO(SCI_IOC_MAGIC, 0x04)

#define SCI_IOR_GET_STATUS 		_IOR(SCI_IOC_MAGIC, 0x05, LX_SCI_STATUS)
#define SCI_IOR_GET_VOLTAGE		_IOR(SCI_IOC_MAGIC, 0x06, LX_SCI_VOLTAGE)
#define SCI_IOW_SET_VOLTAGE		_IOW(SCI_IOC_MAGIC, 0x07, LX_SCI_VOLTAGE)

#define SCI_IO_DUMP_REG         _IO(SCI_IOC_MAGIC, 0x08)
#define SCI_IOW_WRITE_REG       _IOW(SCI_IOC_MAGIC, 0x09, LX_SCI_REG)
#define SCI_IOR_WAIT_CD         _IOR(SCI_IOC_MAGIC, 0x0A, LX_SCI_STATUS)

#define SCI_IOC_MAXNR			11
/** @} */

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _SCI_KAPI_H_ */

/** @} */
