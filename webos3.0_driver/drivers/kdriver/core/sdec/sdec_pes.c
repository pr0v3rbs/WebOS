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


/*----------------------------------------------------------------------------------------
 *   Control Constants
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/
#include "sdec_impl.h"

#include "sdec_io.h"
#include "sdec_pes.h"
#include "sdec_hal.h"

/*----------------------------------------------------------------------------------------
 *   Constant Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Macro Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Type Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   External Function Prototype Declarations
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   External Variables
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   global Functions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   global Variables
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Static Function Prototypes Declarations
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Static Variables
 *---------------------------------------------------------------------------------------*/

#define NUM_OF_CH 		2
#define NUM_OF_GPB 	64
#define NUM_OF_MAP 	(NUM_OF_GPB / SIZE_OF_IDX)
#define SIZE_OF_IDX	32

/* for using physical <-> virtual address conversion */
static UINT32 _gVirtOffset = 0;

/* Information of pes GPB */
PesInfo	_gPesInfo[NUM_OF_CH][NUM_OF_GPB];

/* GPB Map. Set & check with bit */
static UINT32 _gPESGpbMap[NUM_OF_CH][NUM_OF_MAP];


static UINT8 _gPrintEnable = 0;

void SDEC_PES_Init(S_SDEC_PARAM_T 	*stpSdecParam)
{
	int ch_idx = 0, gpb_idx = 0;
	/* set virtual memory */
	SDEC_PES_SetVirtOffset();
	SDEC_PES_AllClearPESFlt(0);
	SDEC_PES_AllClearPESFlt(1);

	/* initliaze gpb info */
	for ( gpb_idx = 0 ; gpb_idx < LX_SDEC_MAX_PESINFO_DATA ; gpb_idx++ )
	{
		stpSdecParam->stPesGPBInfo[gpb_idx].ui8Channel		= 0xff;
		stpSdecParam->stPesGPBInfo[gpb_idx].ui8index 		= 0xff;
		stpSdecParam->stPesGPBInfo[gpb_idx].ui32ReadPtr 	= 0;
		stpSdecParam->stPesGPBInfo[gpb_idx].ui32WritePtr 	= 0;
	}

	/* initliaze gpb info */
	for ( ch_idx = 0 ; ch_idx < NUM_OF_CH ; ch_idx++ )
	{
		for ( gpb_idx = 0 ; gpb_idx < NUM_OF_GPB ; gpb_idx++ )
		{
			_gPesInfo[ch_idx][gpb_idx].processing 		= 0;
			_gPesInfo[ch_idx][gpb_idx].gpbBaseP 		= 0;
			_gPesInfo[ch_idx][gpb_idx].gpbEndPt 		= 0;
			_gPesInfo[ch_idx][gpb_idx].gpbRdPtr 		= 0;
			_gPesInfo[ch_idx][gpb_idx].gpbWrPtr 		= 0;
			_gPesInfo[ch_idx][gpb_idx].pesPacketSize 	= 0;
			_gPesInfo[ch_idx][gpb_idx].copiedSize 		= 0;
			_gPesInfo[ch_idx][gpb_idx].isStart 		= 0;
			_gPesInfo[ch_idx][gpb_idx].continuity_count= 0;
		}
	}

	stpSdecParam->ui8PesGpbInfoRIdx = 0;
	stpSdecParam->ui8PesGpbInfoWIdx = 0;
}

UINT32 SDEC_PES_IsPESFlt(void)
{
	UINT32	val = 0;
	int 	i = 0, j = 0;;

	for ( i = 0; i < NUM_OF_CH ; i++ )
		for ( j = 0; j < NUM_OF_MAP ; j++ )
			val += _gPESGpbMap[i][j];

	return val;
}

static BOOLEAN _SDEC_PES_IsPESFltIdx(UINT8 ch, UINT8 gpbFltIdx)
{
	UINT8	fltIdx = 0, fltPos = 0;

	/* check if over range */
	if( ch >= NUM_OF_CH ) 			return FALSE;
	if( gpbFltIdx >= NUM_OF_GPB ) 	return FALSE;

	fltIdx = gpbFltIdx / SIZE_OF_IDX;
	fltPos = gpbFltIdx % SIZE_OF_IDX;

	return (( _gPESGpbMap[ch][fltIdx] & ( 1 << fltPos ) ) ? TRUE : FALSE );
}

void SDEC_PES_SetPESFlt(UINT8 ch, UINT8 gpbFltIdx)
{
	UINT8	fltIdx = 0, fltPos = 0;

	/* check if over range */
	if( ch >= NUM_OF_CH ) 			return;
	if( gpbFltIdx >= NUM_OF_GPB ) 	return;

	fltIdx = gpbFltIdx / SIZE_OF_IDX;
	fltPos = gpbFltIdx % SIZE_OF_IDX;

	_gPESGpbMap[ch][fltIdx] |= ( 1 << fltPos );
}

void SDEC_PES_ClearPESFlt(UINT8 ch, UINT8 gpbFltIdx)
{
	UINT8	fltIdx = 0, fltPos = 0;

	/* check if over range */
	if( ch >= NUM_OF_CH ) 			return;
	if( gpbFltIdx >= NUM_OF_GPB ) 	return;

	fltIdx = gpbFltIdx / SIZE_OF_IDX;
	fltPos = gpbFltIdx % SIZE_OF_IDX;

	_gPESGpbMap[ch][fltIdx] &= ~( 1 << fltPos );
}


void SDEC_PES_AllClearPESFlt(UINT8 ch)
{
	int i = 0;

	/* check if over range */
	if( ch >= NUM_OF_CH ) return;

	for ( i = 0; i < NUM_OF_MAP ; i++ )
		_gPESGpbMap[ch][i] = 0;
}

void SDEC_PES_SetDstBuf(S_SDEC_PARAM_T *stpSdecParam, UINT8 ch, UINT8 gpbIdx)
{
	UINT32	gpbSize = 0;
	/* check if over range */
	if( ch >= NUM_OF_CH ) 			return;
	if( gpbIdx >= NUM_OF_GPB ) 	return;

	gpbSize = stpSdecParam->stSdecMeminfo[ch][gpbIdx].ui32Endptr - stpSdecParam->stSdecMeminfo[ch][gpbIdx].ui32Baseptr;

	_gPesInfo[ch][gpbIdx].gpbBaseP 	= stpSdecParam->stSdecMeminfo[ch][gpbIdx].ui32Endptr;
	_gPesInfo[ch][gpbIdx].gpbEndPt	= _gPesInfo[ch][gpbIdx].gpbBaseP  + gpbSize;
	_gPesInfo[ch][gpbIdx].gpbRdPtr 	= _gPesInfo[ch][gpbIdx].gpbBaseP;
	_gPesInfo[ch][gpbIdx].gpbWrPtr 	= _gPesInfo[ch][gpbIdx].gpbBaseP;

	_gPesInfo[ch][gpbIdx].pesPacketSize		= 0;
	_gPesInfo[ch][gpbIdx].isStart				= 0;
	_gPesInfo[ch][gpbIdx].continuity_count	= 0;
}

void SDEC_PES_SetVirtOffset(void)
{
	UINT32			baseAddr = 0, size = 0;
	UINT32			virtAddr = 0;
	LX_MEMCFG_T* memCfg = SDEC_CFG_GetMemConfig();

	baseAddr 	= memCfg->base;
	size		= memCfg->size;

	virtAddr = (UINT32) ioremap(baseAddr, size);
	_gVirtOffset = virtAddr - baseAddr;

}

int SDEC_PES_ParsePesHeader(UINT32 va_gpbRdPtr, UINT8 payloadOffset, PesHeader *pes_header, UINT32 va_gpbBaseP, UINT32 va_gpbEndPt)
{
	UINT8 	buff[PES_HEADER_SIZE];

	LX_ADVANCE_BUFF(va_gpbRdPtr, payloadOffset, va_gpbBaseP, va_gpbEndPt);
	LX_GET_PES_HEADER(buff, va_gpbRdPtr, va_gpbBaseP, va_gpbEndPt);

	// Check Start Code Prefix
	if( buff[0] != 0 || buff[1] != 0 || buff[2] != 1)
	{
		SDEC_PRINT_PES( "No Prefix!!!!!!");
		return 1;
	}

	// Stream ID
	pes_header->stream_id = buff[3];

	// PES packet length
	pes_header->PES_packet_length = ( (UINT16) (buff[4]) << 8 ) | buff[5];

	return 0;
}


void SDEC_PES_ParseTsHeader(UINT32 va_gpbRdPtr, TsHeader *ts_header, UINT32 va_gpbBaseP, UINT32 va_gpbEndPt)
{
	UINT8 buff[TS_HEADER_SIZE];
	UINT8 byte1, byte2;
	int pid;

	LX_GET_TS_HEADER(buff, va_gpbRdPtr, va_gpbBaseP, va_gpbEndPt);

	ts_header->sync_byte = buff[0];

	byte1 = buff[1];
	byte2 = buff[2];
	pid = byte1&0x1f;
	pid = (pid<<8) + byte2;

	ts_header->PID = pid;

	byte1 = byte1>>5;
	ts_header->transport_priority = byte1&0x1;
	byte1 = byte1>>1;
	ts_header->payload_unit_start_indicator = byte1&0x1;
	byte1 = byte1>>1;
	ts_header->transport_error_indicator = byte1&0x1;

	byte2 = buff[3];
	ts_header->continuity_counter = byte2&0x0f;	// 4 bits
	byte2 = byte2>>4;
	ts_header->adaptation_field_control = byte2&0x03;	// 2bits
	byte2 = byte2>>2;
	ts_header->transport_scrambling_control = byte2&0x03;	// 2bits

	ts_header->adaptation_field_length = 0;

	if(_gPrintEnable)
	{
		SDEC_PRINT_PES( "PID : 0x%x HEADER[%02x%02x%02x%02x%02x]", ts_header->PID, buff[0], buff[1], buff[2], buff[3], buff[4]);
		SDEC_PRINT_PES( "[%d]: transport_error_indicator", ts_header->transport_error_indicator);
		SDEC_PRINT_PES( "[%d]: payload_unit_start_indicator", ts_header->payload_unit_start_indicator);
		SDEC_PRINT_PES( "[%d]: transport_priority", ts_header->transport_priority);
		SDEC_PRINT_PES( "[%d]: transport_scrambling_control", ts_header->transport_scrambling_control);
		SDEC_PRINT_PES( "[%d]: adaptation_field_control", ts_header->adaptation_field_control);
		SDEC_PRINT_PES( "[%d]: continuity_counter", ts_header->continuity_counter);
	}

	if (ts_header->transport_error_indicator == 0 && (ts_header->adaptation_field_control & 0x02) ) // '10' or '11'
	{
		ts_header->adaptation_field_length =  buff[4] + 1;

		if(_gPrintEnable)
		{
			;
			SDEC_PRINT_PES( "adaptaion field length : %d", ts_header->adaptation_field_length);
		}
	}

}

void SDEC_PES_CopyPacket(S_SDEC_PARAM_T *stpSdecParam, UINT8 chIdx, UINT8 gpbIdx, UINT32 va_gpbRdPtr, UINT8 payloadOffset)
{
	/* physical address */
	UINT32  gpbBase = 0, gpbEnd = 0;

	/* virtual address */
	UINT32	va_gpbBaseP = 0, va_gpbEndPt = 0;
	UINT32  va_dstWrPtr = 0, va_dstBaseP = 0, va_dstEndPt = 0;

	UINT32	sizeToCopy = 0,remainSize = 0, wrappedSize = 0, tailSize = 0;

	/* get gpb information */
	gpbBase 	= stpSdecParam->stSdecMeminfo[chIdx][gpbIdx].ui32Baseptr;
	gpbEnd		= stpSdecParam->stSdecMeminfo[chIdx][gpbIdx].ui32Endptr;

	/* get virtual memory of gpb & destination buffer */
	va_gpbBaseP = LX_PHYS_TO_VIRT(gpbBase);
	va_gpbEndPt = LX_PHYS_TO_VIRT(gpbEnd);
	va_dstWrPtr = LX_PHYS_TO_VIRT(_gPesInfo[chIdx][gpbIdx].gpbWrPtr);
	va_dstBaseP = LX_PHYS_TO_VIRT(_gPesInfo[chIdx][gpbIdx].gpbBaseP);
	va_dstEndPt = LX_PHYS_TO_VIRT(_gPesInfo[chIdx][gpbIdx].gpbEndPt);

	//SDEC_PRINT_PES( "1 gp_rp[%08x] dt_wp[%08x]", va_gpbRdPtr,  va_dstWrPtr);

	/* check payloadOffset */
	if(payloadOffset >= TS_PACKET_SIZE)	return;

	sizeToCopy = TS_PACKET_SIZE - payloadOffset;

	LX_ADVANCE_BUFF(va_gpbRdPtr, payloadOffset, va_gpbBaseP, va_gpbEndPt);

	/* memory copy */
	if( (va_gpbRdPtr + sizeToCopy) > va_gpbEndPt)
	{
		/* wrap around happen */
		wrappedSize = va_gpbEndPt - va_gpbRdPtr;
		remainSize	= sizeToCopy;

		//SDEC_PRINT_PES( "SIZE[%4d] Wraped[%4d]", remainSize, wrappedSize);
		//SDEC_PRINT_PES( "1 gp_rp[%08x] dt_wp[%08x]", va_gpbRdPtr,  va_dstWrPtr);

		if( ( va_dstWrPtr + wrappedSize ) <= va_dstEndPt)
		{
			memcpy((void*)va_dstWrPtr, (void*)va_gpbRdPtr, wrappedSize);
			va_dstWrPtr = va_dstWrPtr + wrappedSize;
			//SDEC_PRINT_PES( "Normal copy");
		}
		else
		{
			/*
			 * wrapped around copy scheme
			 *
			 * step 1 :
			 *										 va_gpbRdPtr
			 *											  V
			 * src :  |------------------------------------------------------------|
			 *			   ^
			 *		  va_gpbWrPtr
			 *												   va_dstWrPtr	 va_dstEndPt
			 *														 V			   V
			 * dst :  |----------------------------------------------++++++++++++++|
			 *														 ^ ----------- ^
			 *															 tailSize
			 */

			tailSize = va_dstEndPt - va_dstWrPtr;
			/* copy first part to destination segment */
			memcpy((void*)va_dstWrPtr, (void*)va_gpbRdPtr, tailSize);
			/* copy second part to destination base pointer */
			memcpy((void*)va_dstBaseP, (void*)(va_gpbRdPtr + tailSize),  wrappedSize - tailSize);
			va_dstWrPtr = va_dstBaseP + wrappedSize - tailSize;

			//SDEC_PRINT_PES( "Segment Copy tailSize[%x]", tailSize);
		}

		//SDEC_PRINT_PES( "2 gp_rp[%08x] dt_wp[%08x]", va_gpbRdPtr, va_dstWrPtr);

		remainSize -= wrappedSize;

		if( ( va_dstWrPtr + remainSize ) <= va_dstEndPt)
		{
			memcpy((void*)va_dstWrPtr, (void*)va_gpbBaseP, remainSize);
			va_dstWrPtr = va_dstWrPtr + remainSize;
			//SDEC_PRINT_PES( "Normal copy");
		}
		else
		{
			tailSize = va_dstEndPt - va_dstWrPtr;

			/* copy first part to destination segment */
			memcpy((void*)va_dstWrPtr, (void*)va_gpbBaseP, tailSize);
			/* copy second part to destination base pointer */
			memcpy((void*)va_dstBaseP, (void*)(va_gpbBaseP + tailSize),  remainSize - tailSize);
			va_dstWrPtr = va_dstBaseP + remainSize - tailSize;

			//SDEC_PRINT_PES( "Segment Copy tailSize[%x]", tailSize);
		}
		//SDEC_PRINT_PES( "3 gp_rp[%08x] dt_wp[%08x]", va_gpbRdPtr, va_dstWrPtr);

		//va_gpbRdPtr = va_gpbBaseP + (sizeToCopy - wrappedSize);

	}
	else
	{
		remainSize = sizeToCopy;

		//SDEC_PRINT_PES( "SIZE[%4d]", remainSize);
		//SDEC_PRINT_PES( "1 gp_rp[%08x] dt_wp[%08x]", va_gpbRdPtr, va_dstWrPtr);

		if( ( va_dstWrPtr + remainSize ) <= va_dstEndPt)
		{
			memcpy((void*)va_dstWrPtr, (void*)va_gpbRdPtr, remainSize);
			va_dstWrPtr = va_dstWrPtr + remainSize;
			//SDEC_PRINT_PES( "Normal copy");
		}
		else
		{
			/*
			 * wrapped around copy scheme
			 *
			 * step 1 :
			 *										 va_gpbRdPtr
			 *											  V
			 * src :  |------------------------------------------------------------|
			 *														   ^
			 *													  va_gpbWrPtr
			 *												   va_dstWrPtr	 va_dstEndPt
			 *														 V			   V
			 * dst :  |----------------------------------------------++++++++++++++|
			 *														 ^ ----------- ^
			 *															 tailSize
			 *
			 */

			tailSize = va_dstEndPt - va_dstWrPtr;

			/* copy first part to destination segment */
			memcpy((void*)va_dstWrPtr, (void*)va_gpbRdPtr, tailSize);
			/* copy second part to destination base pointer */
			memcpy((void*)va_dstBaseP, (void*)(va_gpbRdPtr + tailSize),  remainSize - tailSize);
			va_dstWrPtr = va_dstBaseP + remainSize - tailSize;
			//SDEC_PRINT_PES( "Segment Copy tailSize[%x]", tailSize);
		}
		//SDEC_PRINT_PES( "2 gp_rp[%08x] dt_wp[%08x]", va_gpbRdPtr, va_dstWrPtr);

		//va_gpbRdPtr += sizeToCopy;
	}

	_gPesInfo[chIdx][gpbIdx].gpbWrPtr = (UINT32) LX_VIRT_TO_PHYS(va_dstWrPtr);

	//return va_gpbRdPtr;
}

void SDEC_PES_Proc(struct work_struct *work)
{
	S_SDEC_PARAM_T *stpSdecParam;

	UINT8	chIdx = 0, gpbIdx = 0, payloadOffset = 0;

	/* physical address */
	UINT32  gpbWrPtr = 0, gpbRdPtr = 0, gpbBase = 0, gpbEnd = 0, gpbSize = 0;

	/* virtual address */
	UINT32	va_gpbBaseP = 0, va_gpbEndPt = 0, va_gpbRdPtr = 0;
	UINT32  va_dstBaseP = 0, va_dstEndPt = 0, va_dstRdPtr = 0, 	va_dstWrPtr = 0;

	SINT32	remainSize = 0, wrappedSize = 0;//, tailSize = 0;

	TsHeader	tsHeader;
	PesHeader	pesHeader;

	LX_SDEC_NOTIFY_PARAM_T notiParam;

	UINT8 		ui8PesGpbInfoWIdx = 0, ui8PesGpbInfoRIdx = 0;
	BOOLEAN 	isFound = FALSE;

	unsigned long flags = 0;

	LX_SDEC_CHECK_CODE( work == NULL, return, "Invalid parameters" );

	stpSdecParam =  container_of (work, S_SDEC_PARAM_T, PesProc);

	LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return, "Invalid parameters" );

	SDEC_PRINT_TRACE( "<--SDEC_PES_Proc");

	ui8PesGpbInfoWIdx = stpSdecParam->ui8PesGpbInfoWIdx;
	ui8PesGpbInfoRIdx = stpSdecParam->ui8PesGpbInfoRIdx;

	//SDEC_PRINT_PES( "SDEC_PES_Proc");
	//SDEC_PRINT_PES( "ui8PesGpbInfoRIdx[%d], ui8PesGpbInfoWIdx[%d]", stpSdecParam->ui8PesGpbInfoRIdx, stpSdecParam->ui8PesGpbInfoWIdx);

	while(ui8PesGpbInfoRIdx != ui8PesGpbInfoWIdx)
	{
		chIdx		= stpSdecParam->stPesGPBInfo[ui8PesGpbInfoRIdx].ui8Channel;
		gpbIdx		= stpSdecParam->stPesGPBInfo[ui8PesGpbInfoRIdx].ui8index;
		gpbRdPtr	= stpSdecParam->stPesGPBInfo[ui8PesGpbInfoRIdx].ui32ReadPtr;
		gpbWrPtr	= stpSdecParam->stPesGPBInfo[ui8PesGpbInfoRIdx].ui32WritePtr;

		if(_SDEC_PES_IsPESFltIdx(chIdx, gpbIdx) == FALSE )
		{
			//SDEC_HAL_GPBSetReadPtr(chIdx, gpbIdx, (gpbWrPtr & ~GPB_BASE_ADDR_MASK));
			stpSdecParam->stSdecMeminfo[chIdx][gpbIdx].ui32Readptr = gpbRdPtr;
			SDEC_PRINT_ERROR("in PES_Proc, Filter Indx is not valid: ch[%d] idx[%d]", chIdx, gpbIdx);
			//continue;
			goto tail_while;

		}

		SDEC_GPB_LOCK(stpSdecParam, chIdx, gpbIdx);

		/* get gpb information */
		gpbBase 	= stpSdecParam->stSdecMeminfo[chIdx][gpbIdx].ui32Baseptr;
		gpbEnd		= stpSdecParam->stSdecMeminfo[chIdx][gpbIdx].ui32Endptr;
		gpbSize 	= stpSdecParam->stSdecMeminfo[chIdx][gpbIdx].ui32Endptr - gpbBase;

		/* get virtual memory of gpb & destination buffer */
		va_gpbRdPtr	= LX_PHYS_TO_VIRT(gpbRdPtr);
		va_gpbBaseP = LX_PHYS_TO_VIRT(gpbBase);
		va_gpbEndPt = LX_PHYS_TO_VIRT(gpbEnd);
		va_dstRdPtr	= LX_PHYS_TO_VIRT(_gPesInfo[chIdx][gpbIdx].gpbRdPtr);
		va_dstWrPtr	= LX_PHYS_TO_VIRT(_gPesInfo[chIdx][gpbIdx].gpbWrPtr);
		va_dstBaseP = LX_PHYS_TO_VIRT(_gPesInfo[chIdx][gpbIdx].gpbBaseP);
		va_dstEndPt = LX_PHYS_TO_VIRT(_gPesInfo[chIdx][gpbIdx].gpbEndPt);

		//SDEC_PRINT_PES( "CH%c GPB Idx[%d] rp[%08x] wp[%08x]", (chIdx)?'B':'A', gpbIdx , gpbRdPtr, gpbWrPtr);

		if( !gpbBase || !gpbEnd || !gpbRdPtr || !gpbWrPtr)
		{
			SDEC_PRINT_ERROR("in PES_Proc, address is not valid : B[%08x] E[%08x] R[%08x] W[%08x]", gpbBase, gpbEnd, gpbRdPtr, gpbWrPtr);
			SDEC_GPB_UNLOCK(stpSdecParam, chIdx, gpbIdx);
			//continue;
			goto tail_while;
		}

		/* get packet size */
		if( gpbRdPtr > gpbWrPtr)
		{
			/* wrap around happen */
			wrappedSize = gpbEnd - gpbRdPtr;
			remainSize	= wrappedSize + (gpbWrPtr - gpbBase );
		}
		else
		{
			wrappedSize = 0;
			remainSize 	= gpbWrPtr - gpbRdPtr;
		}

		//SDEC_PRINT_PES( "remainSize[%d] r[%08x] w[%08x]", remainSize, gpbRdPtr, gpbWrPtr);

		while(remainSize > 0)
		{
			/* 우선 TS header 가져올 시에 buffer 넘는 건 넘긴다. */
			/* cut TS header and copy */
			SDEC_PES_ParseTsHeader(va_gpbRdPtr, &tsHeader,va_gpbBaseP, va_gpbEndPt);

			payloadOffset = 4 + tsHeader.adaptation_field_length;

			if( tsHeader.sync_byte == 0x47 && payloadOffset < 188 )
			{

				if( tsHeader.payload_unit_start_indicator == 1)
				{
					if(SDEC_PES_ParsePesHeader(va_gpbRdPtr, payloadOffset, &pesHeader, va_gpbBaseP, va_gpbEndPt) == 0)
					{
						//SDEC_PRINT_PES( "PES Packet Length is %d",pesHeader.PES_packet_length);
						_gPesInfo[chIdx][gpbIdx].pesPacketSize 	= pesHeader.PES_packet_length;
						_gPesInfo[chIdx][gpbIdx].copiedSize		= 0;
						_gPesInfo[chIdx][gpbIdx].isStart 			= 1;
						_gPesInfo[chIdx][gpbIdx].continuity_count 	= tsHeader.continuity_counter;
						_gPesInfo[chIdx][gpbIdx].gpbRdPtr 			= _gPesInfo[chIdx][gpbIdx].gpbWrPtr;
					}
				}

				if(_gPesInfo[chIdx][gpbIdx].isStart == 1 && _gPesInfo[chIdx][gpbIdx].pesPacketSize)
				{
					//SDEC_PRINT_PES( "cc[%d]", tsHeader.continuity_counter);
					/* check continuity counter */
					if( tsHeader.continuity_counter == _gPesInfo[chIdx][gpbIdx].continuity_count )
					{

						SDEC_PES_CopyPacket(stpSdecParam, chIdx, gpbIdx, va_gpbRdPtr, payloadOffset);

						_gPesInfo[chIdx][gpbIdx].copiedSize += TS_PACKET_SIZE - payloadOffset;
						//SDEC_PRINT_PES( "copied %d", TS_PACKET_SIZE - payloadOffset);

						/* save cc */
						_gPesInfo[chIdx][gpbIdx].continuity_count = (tsHeader.continuity_counter + 1) % 16;

						if(_gPesInfo[chIdx][gpbIdx].copiedSize >= ( _gPesInfo[chIdx][gpbIdx].pesPacketSize + PES_HEADER_SIZE ) )
						{
							SDEC_PRINT_PES( "copied[%5d] pesPacket[%5d]", _gPesInfo[chIdx][gpbIdx].copiedSize,  _gPesInfo[chIdx][gpbIdx].pesPacketSize);

							_gPesInfo[chIdx][gpbIdx].copiedSize 		= 0;
							_gPesInfo[chIdx][gpbIdx].isStart 			= 0;
							_gPesInfo[chIdx][gpbIdx].continuity_count 	= 0;

							/* stored buf info	*/
							notiParam.channel	= chIdx;
							notiParam.index 	= gpbIdx;
							notiParam.readPtr	= _gPesInfo[chIdx][gpbIdx].gpbRdPtr;
							notiParam.writePtr	= _gPesInfo[chIdx][gpbIdx].gpbWrPtr;
							notiParam.status	= LX_SDEC_FLTSTATE_DATAREADY;
							notiParam.msec		= 0;

							isFound = SDEC_SetNotifyParam( stpSdecParam, notiParam);

							/* if write pointer arrive end, turn around to base */
							if( _gPesInfo[chIdx][gpbIdx].gpbWrPtr == _gPesInfo[chIdx][gpbIdx].gpbEndPt )
								_gPesInfo[chIdx][gpbIdx].gpbWrPtr = _gPesInfo[chIdx][gpbIdx].gpbBaseP;

							/* next read pointer update */
							_gPesInfo[chIdx][gpbIdx].gpbRdPtr = _gPesInfo[chIdx][gpbIdx].gpbWrPtr;
						}
					}
					else
					{
						SDEC_PRINT_ERROR( "Continuity counter error cc[%2d] orgin[%2d]", tsHeader.continuity_counter, _gPesInfo[chIdx][gpbIdx].continuity_count);
						_gPesInfo[chIdx][gpbIdx].copiedSize 		= 0;
						_gPesInfo[chIdx][gpbIdx].isStart 			= 0;
						_gPesInfo[chIdx][gpbIdx].continuity_count 	= 0;
						_gPesInfo[chIdx][gpbIdx].gpbRdPtr 			= _gPesInfo[chIdx][gpbIdx].gpbWrPtr;
					}
				}

			}

			/* advance gpb read pointer */
			LX_ADVANCE_BUFF(va_gpbRdPtr, TS_PACKET_SIZE, va_gpbBaseP, va_gpbEndPt);

			remainSize -= TS_PACKET_SIZE;
		}

		SDEC_GPB_UNLOCK(stpSdecParam, chIdx, gpbIdx);

		//stpSdecParam->stSDEC_MPG_Reg[chIdx]->gpb_r_ptr[gpbIdx].gpb_r_ptr =  (gpbWrPtr & ~GPB_BASE_ADDR_MASK);
		//SDEC_HAL_GPBSetReadPtr(chIdx, gpbIdx, (gpbWrPtr & ~GPB_BASE_ADDR_MASK));
		stpSdecParam->stSdecMeminfo[chIdx][gpbIdx].ui32Readptr = gpbWrPtr & ~GPB_BASE_ADDR_MASK;

	tail_while:
		ui8PesGpbInfoRIdx = (ui8PesGpbInfoRIdx + 1) % LX_SDEC_MAX_PESINFO_DATA;

		_gPesInfo[chIdx][gpbIdx].processing = 0;

	}

	stpSdecParam->ui8PesGpbInfoRIdx = ui8PesGpbInfoRIdx;

	/* if found notifying section data, notify to appl layer */
	//if(isFound)	queue_work(stpSdecParam->workqueue, &stpSdecParam->Notify);
	SDEC_Notify(&stpSdecParam->Notify);
	SDEC_PRINT_TRACE( "-->SDEC_PES_Proc");
}

static BOOLEAN _SDEC_PES_SetProcParam(S_SDEC_PARAM_T *stpSdecParam, S_GPB_INFO_T procParam)
{
	BOOLEAN isFound = FALSE;
	unsigned long flags = 0;

	LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return isFound, "Invalid parameters" );

	SDEC_PRINT_TRACE( "<--_SDEC_PES_SetProcParam");

	spin_lock_irqsave(&stpSdecParam->stSdecPesSpinlock, flags);

	/*
	 * overflow check
	 *
	 * case 1 : one more packet, then overflow will happen.
	 *	  ui8PesGpbInfoWIdx
	 *			V
	 * |---------------------------------------------------------|
	 *			 ^
	 *	  ui8PesGpbInfoRIdx
	 *
	 * case 2 : read pointer is 0, write pointer is ( LX_SDEC_MAX_PESINFO_DATA - 1 ).
	 *		  if there is one more packet, write pointer is wrapped around and same with read pointer.
	 *												   ui8PesGpbInfoWIdx
	 *															 V
	 * |---------------------------------------------------------|
	 * ^
	 * ui8PesGpbInfoRIdx
	 */

	/* stored buf info	*/
	stpSdecParam->stPesGPBInfo[stpSdecParam->ui8PesGpbInfoWIdx] = procParam;

	/* increase write pointer */
	stpSdecParam->ui8PesGpbInfoWIdx = (stpSdecParam->ui8PesGpbInfoWIdx + 1) % LX_SDEC_MAX_PESINFO_DATA;

	if( ( stpSdecParam->ui8PesGpbInfoRIdx - stpSdecParam->ui8PesGpbInfoWIdx) != 1 &&							// case 1.
		( stpSdecParam->ui8PesGpbInfoWIdx - stpSdecParam->ui8PesGpbInfoRIdx) != ( LX_SDEC_MAX_PESINFO_DATA - 1 )	// case 2.
		)
	{
		/* buffer is not full */

		/* set found flag */
		isFound = TRUE;
	}
	else
	{
		/* buffer is full, reset to 0 */
		stpSdecParam->ui8PesGpbInfoRIdx = 0;
		stpSdecParam->ui8PesGpbInfoWIdx = 0;
		SDEC_PRINT_ERROR( "SDEC_PES : Full!");
	}

	SDEC_PRINT_TRACE( "-->_SDEC_PES_SetProcParam");

	spin_unlock_irqrestore(&stpSdecParam->stSdecPesSpinlock, flags);

	return isFound;
}

void SDEC_PES_Thread(S_SDEC_PARAM_T 	*stpSdecParam)
{
	/* physical address */
	UINT32  gpbWrPtr = 0, gpbRdPtr = 0, gpbBase = 0;

	UINT8	chIdx = 0, mapIdx = 0, gpbIdx = 0, count = 0;
	UINT32	pesFltMap = 0;

	BOOLEAN		isFound = FALSE;
	S_GPB_INFO_T procParam;

	for ( chIdx = 0 ; chIdx < NUM_OF_CH ; chIdx++ )
	{
		for ( mapIdx = 0 ; mapIdx < NUM_OF_MAP ; mapIdx++ )
		{
			pesFltMap = _gPESGpbMap[chIdx][mapIdx];
			/* check if there really is available filter with bit check */
			for(count = 0 ; count < SIZE_OF_IDX && pesFltMap ; count++ )
			{
				if ( check_one_bit(pesFltMap, count) )
				{
					gpbIdx = count + mapIdx * SIZE_OF_IDX;

					if(_gPesInfo[chIdx][gpbIdx].processing == 0)
					{

						gpbBase 	= stpSdecParam->stSdecMeminfo[chIdx][gpbIdx].ui32Baseptr;
						//gpbWrPtr	= stpSdecParam->stSDEC_MPG_Reg[chIdx]->gpb_w_ptr[gpbIdx].gpb_w_ptr | (gpbBase & GPB_BASE_ADDR_MASK);
						//gpbRdPtr	= stpSdecParam->stSDEC_MPG_Reg[chIdx]->gpb_r_ptr[gpbIdx].gpb_r_ptr | (gpbBase & GPB_BASE_ADDR_MASK);
						gpbWrPtr	= SDEC_HAL_GPBGetWritePtr(chIdx, gpbIdx)	| (gpbBase & GPB_BASE_ADDR_MASK);
						//gpbRdPtr	= SDEC_HAL_GPBGetReadPtr (chIdx, gpbIdx)	| (gpbBase & GPB_BASE_ADDR_MASK);
						gpbRdPtr	= stpSdecParam->stSdecMeminfo[chIdx][gpbIdx].ui32Readptr | (gpbBase & GPB_BASE_ADDR_MASK);

						/* gpb buffer is not empty */
						if(gpbRdPtr != gpbWrPtr)
						{
							/* set notify info */
							procParam.ui8Channel 	= chIdx;
							procParam.ui16StatusInfo= 0;
							procParam.ui8index		= gpbIdx;
							procParam.ui32ReadPtr	= gpbRdPtr;
							procParam.ui32WritePtr	= gpbWrPtr;

							isFound = _SDEC_PES_SetProcParam(stpSdecParam, procParam);

							_gPesInfo[chIdx][gpbIdx].processing = 1;
						}
					}

					/* for void unnecessary loop */
					clear__bit(pesFltMap, count);
				}

			}
		}
	}

	/* if found notifying section data, notify to appl layer */
	if(isFound) queue_work(stpSdecParam->workqueue, &stpSdecParam->PesProc);

}

