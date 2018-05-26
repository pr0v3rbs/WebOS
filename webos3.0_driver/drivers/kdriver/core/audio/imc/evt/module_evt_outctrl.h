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
#ifndef __MODULE_EVT_OUTCTRL_H__
#define __MODULE_EVT_OUTCTRL_H__

#include "common/adec_media_type.h"
#include "imc/adec_imc_evt.h"

#define OUTCTRL_EVT_SYSTEM_DELAY			ADEC_EVT_MOD_ID(EVT_OUT_CTRL, 0x00)

/**
* Master System Delay Event 정의한 구조체.
*/
typedef struct _OutctrlEvtMasterSystemDelay
{
   unsigned int         master_system_delay;
}OutctrlEvtMasterSystemDelay;

#endif //__MODULE_EVT_OUTCTRL_H__
