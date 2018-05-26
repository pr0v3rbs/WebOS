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

/** @file
 *
 *  hal api for venc device.
 *	venc device will teach you how to make device driver with new platform.
 *
 *  @author		jaeseop.so (jaeseop.so@lge.com)
 *  version		1.0
 *  date		2012.07.23
 *  note		Additional information.
 *
 *  @addtogroup lg1154_venc
 *	@{
 */

#ifndef	_VENC_REG_H_
#define	_VENC_REG_H_

/*-----------------------------------------------------------------------------
	Control Constants
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
    File Inclusions
-----------------------------------------------------------------------------*/

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/*-----------------------------------------------------------------------------
	Constant Definitions
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	Macro Definitions
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
    Type Definitions
-----------------------------------------------------------------------------*/
typedef enum
{
	LX_VENC_TIMESTAMP_GSTC = 0,		// Global STC
	LX_VENC_TIMESTAMP_PTS  = 1,		// pts from de (if atv/external input, de local stc value)
#ifdef H1ENCODE_USE_PREP_STC
	LX_VENC_TIMESTAMP_STC  = 2,		// Read stc from stc register
#endif
} LX_VENC_TIMESTAMP_TYPE_T;

typedef enum
{
	LX_VENC_FRAME_TYPE_INVALID = 0,
	LX_VENC_FRAME_TYPE_TOP = 1,
	LX_VENC_FRAME_TYPE_BOTTOM = 2,
	LX_VENC_FRAME_TYPE_PROGRESSIVE = 3
} LX_VENC_FRAME_TYPE_T;

// H/W에서 설정하는 값.(PREP Register에 기록된 것을 읽어옴)
typedef struct
{
	// Read from PREP Register
	u32 yBase;
	u32 cBase;
	u16 inWidth;
	u16 inHeight;

	u16 decWidth;
	u16 decHeight;

	u16 yStride;
	u16 cStride;
	LX_VENC_FRAME_TYPE_T frameType;
	BOOLEAN	topFieldFirst;

	u16 frameRateCode;
} LX_VENC_REG_IPC_STATUS_T;

/*-----------------------------------------------------------------------------
	External Variables
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
	Generic Usage Functions
-----------------------------------------------------------------------------*/
int VENC_REG_Init( int id, LX_VENC_CFG_CORE_T cfg_core );
int VENC_REG_Cleanup( int id );
int VENC_REG_CopyExternalRegs( int id );
void VENC_REG_SetTime( int id, UINT32 clock );
UINT32 VENC_REG_GetTime( int id );
UINT32 VENC_REG_GetSTC( int id, LX_VENC_TIMESTAMP_TYPE_T eTimestamp );
UINT32 VENC_REG_ReadIPCStatus( int id, LX_VENC_REG_IPC_STATUS_T *pstIPCStatus );
UINT32 VENC_REG_PrintIPCStatus( int id );
void VENC_REG_InterruptEnable( int id, BOOLEAN enable );
UINT32 VENC_REG_InterruptRead( int id );
void VENC_REG_InterruptClear( int id );
void VENC_REG_InterruptEnableVsync( int id, BOOLEAN enable );
int VENC_REG_DebugPrintRegs( int id );

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _VENC_REG_H_ */

/** @} */

