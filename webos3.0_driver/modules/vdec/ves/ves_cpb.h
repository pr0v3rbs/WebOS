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

#ifndef _VES_CPB_H_
#define _VES_CPB_H_

/*------------------------------------------------------------------------------
	Control Constants
------------------------------------------------------------------------------*/
#define CPB_USE_NEW_UPDATE_BUFFER

/*------------------------------------------------------------------------------
    File Inclusions
------------------------------------------------------------------------------*/
#include "hal/pdec_hal_api.h"
#include "vdec_type_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------------------------------------------------
	Constant Definitions
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
	Macro Definitions
------------------------------------------------------------------------------*/
#define		VES_CPB_CEILING_512BYTES( _addr )			(((_addr) + 0x1FF) & (~0x1FF))
#define		VES_CPB_BOTTOM_512BYTES( _addr )			(((_addr) + 0x000) & (~0x1FF))


/*------------------------------------------------------------------------------
    Type Definitions
------------------------------------------------------------------------------*/
typedef struct
{
	UINT32		addr;		///< address of buffer ( physical or logical )
	UINT32		length; 	///< length of buffer
}
CPB_MEM_BUF_T;

typedef 	int (*fpCpbCopyfunc)(void *, void *, int);

/*------------------------------------------------------------------------------
	Extern Function Prototype Declaration
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
	Extern Variables
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
	Generic Usage Functions
------------------------------------------------------------------------------*/
void VES_CPB_Init(void);
void VES_CPB_Reset(UINT8 ui8VesCh);
void VES_CPB_Flush(UINT8 ui8VesCh);
BOOLEAN VES_CPB_Open(UINT8 ui8VesCh, UINT8 ui8PdecCh, UINT32 ui32CpbBufAddr, UINT32 ui32CpbBufSize, BOOLEAN bIsHwPath, BOOLEAN bFromTVP, E_PDEC_CODEC_TYPE_T eCodecType);
void VES_CPB_Close(UINT8 ui8VesCh);
UINT32 VES_CPB_GetPhyWrPtr(UINT8 ui8VesCh);
UINT32 VES_CPB_GetPhyRdPtr(UINT8 ui8VesCh);
UINT8 *VES_CPB_TranslatePhyWrPtr(UINT8 ui8VesCh, UINT32 ui32WrPhyAddr);
void VES_CPB_UpdateWrPtr(UINT8 ui8VesCh, UINT32 ui32WrPhyAddr);
void VES_CPB_UpdateRdPtr(UINT8 ui8VesCh, UINT32 ui32RdPhyAddr);
UINT32 VES_CPB_Write(	UINT8 ui8VesCh,
		UINT32 eAuType,
		UINT32 ui32AuStartAddr,
		UINT32 ui32AuSize,
		BOOLEAN b512bytesAligned,
		UINT32 *pui32AuSizeModified,
		UINT32 *pui32CpbWrPhyAddr_End);
BOOLEAN VES_CPB_Read(UINT8 ui8VesCh, UINT32 ui32Start, UINT32 ui32Size, UINT8 *ui8Buf);
UINT32 VES_CPB_GetUsedBuffer(UINT8 ui8VesCh);
UINT32 VES_CPB_GetBufferBaseAddr(UINT8 ui8VesCh);
UINT32 VES_CPB_GetBufferSize(UINT8 ui8VesCh);
UINT32 VES_CPB_GetRemainedBuffer(UINT8 ui8VesCh);
UINT32 VES_CPB_AllocBuffer(UINT8 ui8VesCh, UINT32 ui32AuSize, CPB_MEM_BUF_T * pstBufFront, CPB_MEM_BUF_T *pstBufBack, BOOLEAN b512bytesAligned);

#ifdef __cplusplus
}
#endif

#endif /* _VES_CPB_H_ */

