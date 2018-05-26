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



/**
 *	@file module_evt_hdmi.h
 *
 *
 *	@author		Jung, Kyung Soon (kyungsoon.jung@leg.com)
 *	@version	0.0.1
 *	@date		2012-07-20
 *	@note
 *	@see		http://www.lge.com
 *	@addtogroup ADEC
 *	@{
 */


/******************************************************************************
  Header File Guarder
 ******************************************************************************/
#ifndef __MODULE_EVT_HDMI_H__
#define __MODULE_EVT_HDIM_H__

#include "imc/adec_imc_evt.h"


#define HDMI_EVT_PCM_FORMAT         ADEC_EVT_MOD_ID(EVT_HDMI, 0x00)
#define HDMI_EVT_ES_DISCONTINUITY   ADEC_EVT_MOD_ID(EVT_HDMI, 0x01)


/**
* PCM(IEC 60958) Format Event를 정의한 구조체.
*/
typedef struct _HdmiEvtPCMFormatParam
{
    unsigned int        format;             ///< 0 : PCM, 1 : DTS-CD
}HdmiEvtPCMFormatParam;

/**
* HDMI Input Discontinuity Event를 정의한 구조체.
*/
typedef struct _HdmiEvtESDiscontinuityParam
{
    unsigned int        discontinuity;      ///< 0 : Normal, 1 : Discontinuity
}HdmiEvtESDiscontinuityParam;

#endif // #ifndef __MODULE_EVT_HDMI_H__
/** @} */

