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
 * main driver implementation for vdec device.
 * vdec device will teach you how to make device driver with new platform.
 *
 * author     junghyun.son@lge.com
 * version    1.0
 * date       2013.10.29
 * note       Additional information.
 *
 */

#ifndef _VES_DRV_H_
#define _VES_DRV_H_

/*------------------------------------------------------------------------------
	Control Constants
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
    File Inclusions
------------------------------------------------------------------------------*/
#include "vdec_type_defs.h"
#include "ves_cpb.h"
#include "ves_auib.h"

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------------------------------------------------
	Constant Definitions
------------------------------------------------------------------------------*/
#define	VES_NUM_OF_CHANNEL			VDEC_NUM_OF_CHANNEL

/*------------------------------------------------------------------------------
    Type Definitions
------------------------------------------------------------------------------*/
typedef enum
{
	VES_SRC_SDEC0		= 0x00,
	VES_SRC_SDEC1		= 0x01,
	VES_SRC_SDEC2		= 0x02,
	VES_SRC_TVP		= 0x03,
	VES_SRC_FILEPLAY	= 0x04,
	VES_SRC_32bits		= 0x20120316,
} E_VES_SRC_T;

typedef enum
{
	VES_PIC_DETECTED	= 0,
	VES_CPB_ALMOST_FULL	= 1
} E_VES_UPDATED_REASON_T;

typedef void (*VES_EVENT_T) (UINT8 ch, void *arg, E_VES_UPDATED_REASON_T reason);

/*------------------------------------------------------------------------------
	Extern Function Prototype Declaration
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
	Extern Variables
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
	Generic Usage Functions
------------------------------------------------------------------------------*/
void VES_Init(void);
UINT8 VES_Open(E_VES_SRC_T ui8VesSrcCh,
		UINT8 ui8Vcodec,
		UINT32 ui32CpbBufAddr, UINT32 ui32CpbBufSize,
		VES_EVENT_T updated, void *updated_arg);
void *VES_GetVdecPriv(UINT8 ui8PdecCh);
void VES_Close(UINT8 ui8PdecCh);
void VES_Suspend(void);
void VES_Resume(void);
void VES_Reset(UINT8 ui8PesDecCh);
void VES_Start(UINT8 ui8PdecCh);
void VES_Stop(UINT8 ui8PdecCh);
void VES_Flush(UINT8 ui8PdecCh);

BOOLEAN VES_UpdatePicture(UINT8 ui8VesCh,
		E_VES_AU_T eAuType,
		UINT32 ui32UserBuf,
		UINT32 ui32UserBufSize,
		UINT32 ui32UId,
		UINT64 ui64TimeStamp);

#ifdef CPB_USE_NEW_UPDATE_BUFFER
UINT32 VES_AllocBuffer(UINT8 ui8VesCh, UINT32 ui32AuSize, CPB_MEM_BUF_T * pstBufFront, CPB_MEM_BUF_T *pstBufBack);
BOOLEAN VES_UpdatePictureEx(UINT8 ui8VesCh,
		E_VES_AU_T eAuType,
		CPB_MEM_BUF_T *pstPhysBuf0,
		CPB_MEM_BUF_T *pstPhysBuf1,
		UINT32 ui32UId,
		UINT64 ui64TimeStamp);
#endif

void VDEC_ISR_PIC_Detected(UINT8 ui8PdecCh);

#ifdef __cplusplus
}
#endif

#endif /* _VES_DRV_H_ */

