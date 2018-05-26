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
#ifndef __MODULE_EVT_SE_H__
#define __MODULE_EVT_SE_H__

#include "common/adec_media_type.h"
#include "imc/adec_imc_evt.h"

#define SE_EVT_DETECT_TONE			ADEC_EVT_MOD_ID(EVT_SE, 0x00)

/**
* Detect tone Event 정의한 구조체.
*/
typedef struct _SeEvtDetectTone
{
   unsigned int         istone;
}SeEvtDetectTone;

#endif //__MODULE_EVT_SE_H__

