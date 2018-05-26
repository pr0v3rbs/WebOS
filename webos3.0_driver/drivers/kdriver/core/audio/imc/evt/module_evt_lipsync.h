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




/******************************************************************************
  Header File Guarder
 ******************************************************************************/
#ifndef __MODULE_EVT_LIPSYNC_H__
#define __MODULE_EVT_LIPSYNC_H__

#include "common/adec_media_type.h"
#include "imc/adec_imc_evt.h"


#define LIPSYNC_EVT_NOPCR_BASELINE			ADEC_EVT_MOD_ID(EVT_LIPSYNC, 0x00)
#define LIPSYNC_EVT_PRESENT_END             ADEC_EVT_MOD_ID(EVT_LIPSYNC, 0x01)
#define LIPSYNC_EVT_UNDERFLOW				ADEC_EVT_MOD_ID(EVT_LIPSYNC, 0x02)
#define LIPSYNC_EVT_PRESENTINDEX			ADEC_EVT_MOD_ID(EVT_LIPSYNC, 0x03)

/**
* No PCR Base Line Event 정의한 구조체.
*/
typedef struct _LipsyncEvtNopcrBaseline
{
	unsigned int		baseST;				///< steam time
	unsigned int		baseCT;				///< clock time
}LipsyncEvtNopcrBaseline;

/**
* Present End Event 정의한 구조체.
*/
typedef struct _LipsyncEvtPresentEnd
{
	unsigned int		remain_ms;
}LipsyncEvtPresentEnd;

/**
* Underflow Event 정의한 구조체.
*/
typedef struct _LipsyncEvtUnderflow
{
	unsigned int		tmp;
}LipsyncEvtUnderflow;

/**
* Present Index Event 정의한 구조체.
*/
typedef struct _LipsyncEvtPresentIndex
{
	unsigned int		index;				// Presented Index
	unsigned int		timestamp;			// PTS
}LipsyncEvtPresentIndex;


#endif //__MODULE_EVT_SPDIF_H__

