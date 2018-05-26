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
 *	@file adec_imc_evt.h
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
#ifndef __ADEC_IMC_EVT_H__
#define __ADEC_IMC_EVT_H__

#include "common/adec_config.h"

#define ADEC_EVT_MOD_ID(_MOD_ID, _NUM)	(((_NUM) & 0xFF) | ((_MOD_ID&0xFF)<<8))

/**
 * IMC_EVT 관련 codec module의 numbering을 정의한 ENUM.
 */
typedef enum _EVT_CODEC_MODULE_ID{
	EVT_CODEC				= 0xA0,
	EVT_PCMDEC				= 0xA1,
	EVT_DDC					= 0xA2,
	EVT_DDT					= 0xA3,	
	EVT_DRA					= 0xA4,
	EVT_MP3					= 0xA5,
	EVT_WMA					= 0xA6,
	EVT_DTS					= 0xA7,
	EVT_DTS_M6				= 0xA8,
	EVT_OGG					= 0xA9,
	EVT_AMRWB				= 0xAA,
	EVT_AMRNB				= 0xAB,
	EVT_RA8					= 0xAC,
	EVT_FLAC				= 0xAD,
	EVT_AACENC				= 0xAE,
	EVT_AC3ENC				= 0xAF,
	EVT_DTSENC				= 0xB0,
	EVT_CODEC_MAX			= 0xBF,
}EVT_CODEC_MODULE_ID;

/**
 * IMC_EVT 관련 codec module의 numbering을 정의한 ENUM.
 */
typedef enum _EVT_GENERAL_MODULE_ID{
	EVT_SIF					= 0xC0,
	EVT_ADC					= 0xC1,
	EVT_HDMI				= 0xC2,
	EVT_I2S					= 0xC3,
	EVT_TP					= 0xC4,
	EVT_REFORM				= 0xC5,
	EVT_BYPASS				= 0xC6,
	EVT_PCMCVT				= 0xC7,
	EVT_SOLA				= 0xC8,
	EVT_LIPSYNC				= 0xC9,
	EVT_INPUT_CTRL			= 0xCA,
	EVT_MIXER				= 0xCB,
	EVT_SE					= 0xCC,
	EVT_OUT_CTRL			= 0xCD,
	EVT_ES_OUT_CTRL			= 0xCE,
	EVT_PCMOUT				= 0xCF,
	EVT_SPDIF_OUT			= 0xD0,
	EVT_CTRL_SRC			= 0xD1,
	EVT_DTO_CTRL			= 0xD2,
	EVT_RTS					= 0xD3,
	EVT_TIMER				= 0xD4,
	EVT_DEBUG				= 0xD5,	
	EVT_GENERAL_MAX			= 0xFF,
}EVT_GENERAL_MODULE_ID;

#define ADEC_EVT_OVERFLOW			0x0100		///< for notification overflow of output buffer
#define ADEC_EVT_UNDERFLOW			0x0101		///< for notification underflow of input buffer

#define ADEC_EVT_DSP_DN_DONE		0x0200		///< for notification DSP download done
#define ADEC_EVT_DSP_ALIVE			0x0201		///< for notification DSP alived

#endif //__ADEC_IMC_EVT_H__
/** @} */
