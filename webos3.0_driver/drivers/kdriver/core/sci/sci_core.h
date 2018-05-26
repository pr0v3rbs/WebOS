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
 *  core driver interface header for sci device. ( used only within kdriver )
 *	this file lists exported function, types for the external modules.
 *
 *  @author		bongrae.cho (bongrae.cho@lge.com)
 *  @version	1.0
 *  @date		2012.02.23
 *
 *  @addtogroup lg1154_sci
 *	@{
 */

#ifndef	_SCI_CORE_H_
#define	_SCI_CORE_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include "sci_kapi.h"

#include "sci_err.h"





#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
#define I_BLOCK							0x00
#define I_BLOCK_NS						0x40
#define I_BLOCK_MORE_DATA				0x20
#define GET_I_BLOCK_NS(pcb)				((pcb >> 6) & 0x01)


#define R_BLOCK                         0x80
#define R_BLOCK_NR						0x10
#define GET_R_BLOCK_NS(pcb)				((pcb >> 4) & 0x01)
#define GET_R_BLOCK_ERROR(pcb)          (pcb & 0x0F)

#define S_BLOCK                         0xC0
#define S_BLOCK_RESPONSE                0x20

#define S_BLOCK_TYPE					0x1F
#define S_BLOCK_RESYNCH					0x00
#define S_BLOCK_IFS						0x01
#define S_BLOCK_ABORT					0x02
#define S_BLOCK_WTX						0x03


#define S_BLOCK_ERROR_VPP               (S_BLOCK|0x24)

#define RESYNCH_REQUEST                 (S_BLOCK|S_BLOCK_RESYNCH)
#define RESYNCH_RESPONSE                (S_BLOCK|S_BLOCK_RESYNCH|S_BLOCK_RESPONSE)

#define IFS_REQUEST                     (S_BLOCK|S_BLOCK_IFS)
#define IFS_RESPONSE                    (S_BLOCK|S_BLOCK_IFS|S_BLOCK_RESPONSE)

#define ABORT_REQUEST                   (S_BLOCK|S_BLOCK_ABORT)
#define ABORT_RESPONSE                  (S_BLOCK|S_BLOCK_ABORT|S_BLOCK_RESPONSE)

#define WTX_REQUEST                     (S_BLOCK|S_BLOCK_WTX)
#define WTX_RESPONSE                    (S_BLOCK|S_BLOCK_WTX|S_BLOCK_RESPONSE)


#define SCI_INPUT_CLK    198
#define SCI_INPUT_CLK_DIV    32
#define SCI_CRD_CLK_ADJ_MHZ_TO_KHZ    1000

#define SCI_DEFAULT_FS   372
#define SCI_DEFAULT_DI   1

#define MAX_TRANSMISSION_TIMES          55
/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/
typedef enum {
	SCI_STATUS_IDLE = 0,
	SCI_STATUS_ACTIVATED = 1,
	SCI_STATUS_TRANSMIT = 2,
	SCI_STATUS_RECEIVE = 3
} SCI_DEVICE_STATUS;

typedef enum {	
	TRANSMISSION_NEXT_IBLOCK,
	RETRANSMISSION_IBLOCK,
	READY_NEXT_IBLOCK,
	REQUEST_RETRANSMISSION,
	TRANSMISSION_DONE,
} SCI_ANALYZE_STATUS;

typedef struct
{
    UINT8               nad;
    UINT8               pcb;
    UINT8               len;
    
    UINT8               idx;
    
    UINT8               inf[255];
    UINT8               flagCompleated;
}SCI_TRANSMIT_INFO_T;


typedef struct
{
    UINT32              clock_supplied_khz;
    LX_SCI_ATR_T        atr;
	
    UINT8               isActivated;
	
	UINT8				deviceNS;
	UINT8				cardNS;
	
    UINT8               ifs;
    UINT8               wtx;

	UINT8				RxFIFODepth;
	UINT8				TxFIFODepth;
	
    SCI_DEVICE_STATUS   SciStatus;
    UINT8               SciAutoRecv;
    SINT8               NumberTransmit;
    SINT8               NumberReceive;

	SCI_TRANSMIT_INFO_T txBlock;
	SCI_TRANSMIT_INFO_T rxBlock;
}SCI_INFO_T;


/*----------------------------------------------------------------------------------------
	Global Functions Declaration
----------------------------------------------------------------------------------------*/
void SCI_Reset(SCI_INFO_T* pSCIinfo);

SCI_RETURN_T SCI_Activate(SCI_INFO_T* pSCIinfo);
void SCI_Deactivate(SCI_INFO_T* pSCIinfo);
int SCI_Transmit(SCI_INFO_T* pSCIdev, LX_SCI_TRANSMIT* transmitArg);

SCI_RETURN_T SCI_GetATR(LX_SCI_ATR_T* pATR);
LX_SCI_STATUS SCI_GetStatus(void);
LX_SCI_VOLTAGE SCI_GetVoltage(void);
SCI_RETURN_T SCI_SetVoltage(LX_SCI_VOLTAGE voltage);




#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _SCI_CORE_H_ */

/** @} */
