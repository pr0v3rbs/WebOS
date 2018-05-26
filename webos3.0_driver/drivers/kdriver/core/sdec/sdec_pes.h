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
 *  sdec driver
 *
 *  @author	Jihoon Lee ( gaius.lee@lge.com)
 *  @author	Jinhwan Bae ( jinhwan.bae@lge.com) - modifier
 *  @version	1.0
 *  @date		2010-03-30
 *  @note		Additional information.
 */


#include "sdec_io.h"
#ifndef _SDEC_PES_H
#define _SDEC_PES_H

#ifdef __cplusplus
	 extern "C" {
#endif

/*----------------------------------------------------------------------------------------
 *   Control Constants
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Constant Definitions
 *---------------------------------------------------------------------------------------*/
#define TS_PACKET_SIZE	188

/*----------------------------------------------------------------------------------------
 *   Macro Definitions
 *---------------------------------------------------------------------------------------*/
#define LX_PHYS_TO_VIRT(x)	(x + _gVirtOffset)
#define LX_VIRT_TO_PHYS(x)	(x - _gVirtOffset)

/*
#define LX_GET_TS_HEADER(src, dst, bufStart, bufEnd)	\
{ \
	dst[0] = * ((UINT8*)src ); \
	LX_ADVANCE_BUFF(src, 1, bufStart, bufEnd);	\
	dst[1] = * ((UINT8*)src ); \
	LX_ADVANCE_BUFF(src, 1, bufStart, bufEnd);	\
	dst[2] = * ((UINT8*)src ); \
	LX_ADVANCE_BUFF(src, 1, bufStart, bufEnd);	\
	dst[3] = * ((UINT8*)src ); \
	LX_ADVANCE_BUFF(src, 1, bufStart, bufEnd);	\
	dst[4] = * ((UINT8*)src ); \
};

#define LX_GET_PES_HEADER(src, dst, bufStart, bufEnd)	\
{ \
	dst[0] = * ((UINT8*)src ); \
	LX_ADVANCE_BUFF(src, 1, bufStart, bufEnd);	\
	dst[1] = * ((UINT8*)src ); \
	LX_ADVANCE_BUFF(src, 1, bufStart, bufEnd);	\
	dst[2] = * ((UINT8*)src ); \
	LX_ADVANCE_BUFF(src, 1, bufStart, bufEnd);	\
	dst[3] = * ((UINT8*)src ); \
	LX_ADVANCE_BUFF(src, 1, bufStart, bufEnd);	\
	dst[4] = * ((UINT8*)src ); \
	LX_ADVANCE_BUFF(src, 1, bufStart, bufEnd);	\
	dst[5] = * ((UINT8*)src ); \
};
*/

/*----------------------------------------------------------------------------------------
 *   Type Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   External Function Prototype Declarations
 *---------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------
   enum
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
   structre
----------------------------------------------------------------------------------------*/

typedef struct _TS_HEADER_
{
	UINT32
		sync_byte						: 8,	// (8) 0x47
	 	transport_error_indicator	: 1,		//(1)
		payload_unit_start_indicator: 1,	//(1)
		transport_priority			: 1,	//(1)
		PID								: 13, 	// (13)
		transport_scrambling_control: 2, 	//(2)
		adaptation_field_control		: 2,	//(2)
		continuity_counter			: 4; 	//(4)

	UINT8 adaptation_field_length;
}	TsHeader ;

typedef struct _PES_HEADER_
{
	UINT32
		stream_id			: 8	,	//(8)
		PES_packet_length	: 16,	//(16)
		reserved			: 8;
} PesHeader;

typedef struct
{
	UINT8		processing;
	UINT32		gpbBaseP;
	UINT32		gpbEndPt;
	UINT32		gpbRdPtr;
	UINT32		gpbWrPtr;
	UINT32		pesPacketSize;
	UINT32		copiedSize;
	UINT8		isStart;
	UINT8		continuity_count;
} PesInfo;

/*----------------------------------------------------------------------------------------
   API
----------------------------------------------------------------------------------------*/
void SDEC_PES_Init(S_SDEC_PARAM_T 	*stpSdecParam);
UINT32 SDEC_PES_IsPESFlt(void);
void SDEC_PES_SetVirtOffset(void);
void SDEC_PES_Thread(S_SDEC_PARAM_T 	*stpSdecParam);
void SDEC_PES_SetPESFlt(UINT8 ch, UINT8 pidFltIdx);
void SDEC_PES_ClearPESFlt(UINT8 ch, UINT8 pidFltIdx);
void SDEC_PES_AllClearPESFlt(UINT8 ch);
void SDEC_PES_SetDstBuf(S_SDEC_PARAM_T *stpSdecParam, UINT8 ch, UINT8 gpbIdx);
void SDEC_PES_Proc(struct work_struct *work);


/*----------------------------------------------------------------------------------------
 *   External Variables
 *---------------------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* _SDEC_PES_H */

