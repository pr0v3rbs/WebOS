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
 *	@file module_evt_sif.h
 *
 *
 *	@author		Park, Chong Gun (jake.park@leg.com)
 *	@version	0.0.1
 *	@date		2013-09-03
 *	@note
 *	@see		http://www.lge.com
 *	@addtogroup ADEC
 *	@{
 */

/******************************************************************************
  Header File Guarder
 ******************************************************************************/
#ifndef __MODULE_EVT_SIF_H__
#define __MODULE_EVT_SIF_H__

#include "common/adec_media_type.h"
#include "imc/adec_imc_evt.h"


/* HISTORY :: Unstable SIF Sampling Rate Detection Event 처리 코드 추가. 2013.09.03. */
#define SIF_EVT_UNSTABLE_FREQ_IN_SIF      ADEC_EVT_MOD_ID(EVT_SIF, 0x00)


/**
  * Unstable SIF Sampling Rate Detection Event 처리를 위한 구조체.
  */
typedef struct _SifEvtUnstableFreq
{
    unsigned int    frequency;      /**< SIF Sampling Rate */
}SifEvtUnstableFreq;


#endif // #ifndef __MODULE_EVT_SIF_H__
/** @} */

