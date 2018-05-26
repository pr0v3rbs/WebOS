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
 *  Brief description.
 *  Detailed description starts here.
 *
 *  @author	jinhwan.bae
 *  @version	1.0
 *  @date		2012-05-16
 *  @note		Additional information.
 */

#ifndef	_SDEC_SWPARSER_H_
#define	_SDEC_SWPARSER_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include "sdec_io.h"

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
/* 2012. 11. 17 PVR_RELATED To Modify DN Channel B to A, PVR Use DN Channel B , SDT will use Ch A */
#define PVR_CH_FOR_SDT				LX_PVR_CH_A

#define SWTSPARSER_THREAD_TIMEOUT	3000

#define NUM_OF_MAX_CH_C_FILTER		4		/* 6 PID Slot is Assigned in H13 CH_C->PVR  */

#define	SECTION_FILTER_LENGTH		8

#define TS_SYNC_ERROR		1
#define TP_ERROR_INDICATOR	2
#define TP_SCRAMBLE			3
#define NO_PAYLOAD			4
#define PAYLOAD_SIZE_ERR	5
#define PID_ERROR			6
#define DUPLICATED_PACKET	7
#define CC_ERROR			8
#define SECTION_CRC			9

#define TP_SYNC_BYTE		0x47

#define SDEC_SWP_TIMEOUT			100				/* 100 ms */
#define SDEC_SWP_TIMEOUT_INFINITE	-1				/* Infinite Wait */

#define SDEC_SWP_SEC_CIR_BUF_SIZE	32*1024

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/
typedef struct _TS_PACKET_
{
	UINT8 	ui8Payload_unit_start_indicator;
	UINT16	ui16PID;
	UINT8	ui8Continuity_counter;

	UINT8* 	pui8Payload;
	UINT8	ui8PayloadSize;
} TsPacket;


typedef enum
{
	SWP_FLTSTAT_SEC_HEADER_NEED = 0,
	SWP_FLTSTAT_SEC_HEADER_ING,
	SWP_FLTSTAT_SEC_HEADER_DONE,
	SWP_FLTSTAT_SEC_HEADER_FILTER_DONE,
	SWP_FLTSTAT_SEC_HEADER_COPY_DONE,
	SWP_FLTSTAT_SEC_ALL_COPY_DONE,
	SWP_FLTSTAT_COMPLETE
} TsFilterState;


typedef struct _TS_FILTER_
{
	UINT16 	ui16PID;
	UINT8	ui8WaitFirstTS;
	UINT8	ui8Continuity_counter;

	TsFilterState	eFilterState;

	UINT8 	ui8NeedHeaderSize;
	UINT8	au8HeaderBuffer[SECTION_FILTER_LENGTH];

	/* Filter Condition */
	UINT32	ui32Pattern_H;
	UINT32	ui32Pattern_L;
	UINT32	ui32Mask_H;
	UINT32	ui32Mask_L;
	UINT32	ui32Neg_H;
	UINT32	ui32Neg_L;
	UINT8	ui8CrcCheck;

	UINT32	uiSectionLength;
	UINT32	uiRemainSectionData;

	/* Section Circular Buffer Stub */
	UINT8	*pui8BufStart;
	UINT8	*pui8BufEnd;
	UINT8	*pui8WritePtr;
	UINT8	*pui8ReadPtr;

	/* Download Buffer Information Stub */
	UINT8	*pui8DNBufStart;
	UINT8	*pui8DNBufEnd;
	UINT8	*pui8DNWritePtr;
	UINT8	*pui8DNReadPtr;
} TsFilter;


typedef enum{
	SWP_EVENT_START = 0x01,			/* start thread */
	SWP_EVENT_STOP	= 0x02,			/* stop thread */
} SWPEventType;


/* LIVE_HEVC */
typedef enum{
	RFOUT_EVENT_START = 0x01,		/* start thread */
	RFOUT_EVENT_STOP  = 0x02,		/* stop thread */
} RFOUTEventType;

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/

void SDEC_SWP_Init(S_SDEC_PARAM_T *stpSdecParam);
int SDEC_SWP_EnableSWParser(S_SDEC_PARAM_T *stpSdecParam,	 LX_SDEC_SECFLT_ENABLE_SWPARSER_T *stpLXSdecSecfltEnableSWParser);
void SDEC_SWP_SetSectionBuffer(S_SDEC_PARAM_T *stpSdecParam);
void SDEC_SWP_SetSectionFilterPattern(LX_SDEC_SECFLT_PATTERN_T *stpLXSecFltPattern);
void SDEC_SWP_SetPID(UINT32 ui32PidFltId, UINT32 ui32PidValue);

// LIVE_HEVC
void SDEC_RFOUT_Init(S_SDEC_PARAM_T *stpSdecParam);
int SDEC_RFOUT_Enable(S_SDEC_PARAM_T *stpSdecParam, LX_SDEC_RFOUT_ENABLE_T *stpLXSdecRFOUTEnable);


/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/


#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _PROTOS_H_ */

