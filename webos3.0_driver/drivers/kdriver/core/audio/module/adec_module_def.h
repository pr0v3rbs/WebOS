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
 *	@file adec_module_def.h
 *
 *
 *	@author		Jung, Kyung Soon (kyungsoon.jung@leg.com)
 *	@version	0.0.1
 *	@date		2012-03-19
 *	@note
 *	@see		http://www.lge.com
 *	@addtogroup ADEC_COMMON
 *	@{
 */


/******************************************************************************
  Header File Guarder
 ******************************************************************************/
// This file should not use header guard
//#ifndef __ADEC_MODULE_DEF_H__
//#define __ADEC_MODULE_DEF_H__


/*-----------------------------------------------------------------------------
 * Should Define ADEC_MODULE_DEF(__MODULE_ID, \
 * 								 __MODULE_NAME, \
 * 								 __MODULE_TYPE,\
 * 								 __LOCATION, \
 * 								 __NUM_IN, \
 * 								 __NUM_OUT,\
 * 								 ...)
 *
 * Should Define __BUF_END
 -----------------------------------------------------------------------------*/
#ifndef ADEC_MODULE_DEF
#define ADEC_MODULE_DEF(...)
#endif
#ifndef __BUF_END
#define __BUF_END(...) ADEC_BUF_NOT_DEF
#endif
//             +--------------------------------------------------------------------------------------------------------
//             | Manager Module Definition
//             +------------------------+-----------+-----------------------+---------------+---------------------------
//             | ID						| NAME		| TYPE					| LOCATION		| BUFFERS
//             +------------------------+-----------+-----------------------+---------------+---------------------------
ADEC_MODULE_DEF(ADEC_MODULE_MAN_ARM,		"ARM MAN",		ADEC_MOD_TYPE_MANAGER,		ADEC_CORE_ARM,	0, 0, __BUF_END())
ADEC_MODULE_DEF(ADEC_MODULE_MAN_DSP0,		"DSP0_MAN",		ADEC_MOD_TYPE_MANAGER,		ADEC_CORE_DSP0,	0, 0, __BUF_END())
ADEC_MODULE_DEF(ADEC_MODULE_MAN_DSP1,		"DSP1_MAN",		ADEC_MOD_TYPE_MANAGER,		ADEC_CORE_DSP1,	0, 0, __BUF_END())
ADEC_MODULE_DEF(ADEC_MODULE_DBG_DSP0,		"DSP0_DBG",		ADEC_MOD_TYPE_MANAGER,		ADEC_CORE_DSP0,	0, 0, __BUF_END())
ADEC_MODULE_DEF(ADEC_MODULE_DBG_DSP1,		"DSP1_DBG",		ADEC_MOD_TYPE_MANAGER,		ADEC_CORE_DSP1,	0, 0, __BUF_END())

ADEC_MODULE_DEF(ADEC_MODULE_IMC_DSP0,		"IMC_DSP0",		ADEC_MOD_TYPE_MANAGER,		ADEC_CORE_DSP0,	0, 0, __BUF_END())
ADEC_MODULE_DEF(ADEC_MODULE_IMC_DSP1,		"IMC_DSP1",		ADEC_MOD_TYPE_MANAGER,		ADEC_CORE_DSP1,	0, 0, __BUF_END())
ADEC_MODULE_DEF(ADEC_MODULE_IMC_ARM,		"IMC_ARM",		ADEC_MOD_TYPE_MANAGER,		ADEC_CORE_ARM,	0, 0, __BUF_END())

//             +--------------------------------------------------------------------------------------------------------
//             | Source Module Definition
//             +------------------------+-----------+-----------------------+---------------+---------------------------
//             | ID						| NAME		| TYPE					| LOCATION		| BUFFERS
//             +------------------------+-----------+-----------------------+---------------+---------------------------
ADEC_MODULE_DEF(ADEC_MODULE_TP_IN_0,		"TP_IN_0",		ADEC_MOD_TYPE_SOURCE,		ADEC_CORE_DSP0,	0, 1, ADEC_BUF_MULTI_SINK, __BUF_END())
ADEC_MODULE_DEF(ADEC_MODULE_TP_IN_1,		"TP_IN_1",		ADEC_MOD_TYPE_SOURCE,		ADEC_CORE_DSP0,	0, 1, ADEC_BUF_MULTI_SINK, __BUF_END())
ADEC_MODULE_DEF(ADEC_MODULE_SIF_IN,			"SIF_IN",		ADEC_MOD_TYPE_SOURCE,		ADEC_CORE_DSP0,	0, 1, ADEC_BUF_MULTI_SINK, __BUF_END())
ADEC_MODULE_DEF(ADEC_MODULE_HDMI_IN_0,		"HDMI_IN_0",	ADEC_MOD_TYPE_SOURCE,		ADEC_CORE_DSP0,	0, 1, ADEC_BUF_MULTI_SINK, __BUF_END())
ADEC_MODULE_DEF(ADEC_MODULE_HDMI_IN_1,		"HDMI_IN_1",	ADEC_MOD_TYPE_SOURCE,		ADEC_CORE_DSP0,	0, 1, ADEC_BUF_MULTI_SINK, __BUF_END())
ADEC_MODULE_DEF(ADEC_MODULE_ADC_IN,			"ADC_IN",		ADEC_MOD_TYPE_SOURCE,		ADEC_CORE_DSP0,	0, 1, ADEC_BUF_MULTI_SINK, __BUF_END())
ADEC_MODULE_DEF(ADEC_MODULE_I2S_IN,	 		"I2S_IN",		ADEC_MOD_TYPE_SOURCE,		ADEC_CORE_DSP0,	0, 1, ADEC_BUF_MULTI_SINK, __BUF_END())

//             +--------------------------------------------------------------------------------------------------------
//             | Reformatter Module Definition
//             +------------------------+-----------+-----------------------+---------------+---------------------------
//             | ID						| NAME		| TYPE					| LOCATION		| BUFFERS
//             +------------------------+-----------+-----------------------+---------------+---------------------------
ADEC_MODULE_DEF(ADEC_MODULE_REFORM_0,		"REFORM_0",		ADEC_MOD_TYPE_REFOMATTER,	ADEC_CORE_DSP0,	1, 1, ADEC_BUF_MULTI_SINK, __BUF_END())
ADEC_MODULE_DEF(ADEC_MODULE_REFORM_1,		"REFORM_1",		ADEC_MOD_TYPE_REFOMATTER,	ADEC_CORE_DSP0,	1, 1, ADEC_BUF_MULTI_SINK, __BUF_END())

//             +--------------------------------------------------------------------------------------------------------
//             | Codec Module Definition
//             +------------------------+-----------+-----------------------+---------------+---------------------------
//             | ID						| NAME		| TYPE					| LOCATION		| BUFFERS
//             +------------------------+-----------+-----------------------+---------------+---------------------------
ADEC_MODULE_DEF(ADEC_MODULE_DEC_0,			"DEC_0",		ADEC_MOD_TYPE_DECODER,		ADEC_CORE_DSP0,	1, 2, ADEC_BUF_MULTI_SINK, ADEC_BUF_INTRA, __BUF_END())
ADEC_MODULE_DEF(ADEC_MODULE_DEC_1,			"DEC_1",		ADEC_MOD_TYPE_DECODER,		ADEC_CORE_DSP0,	1, 2, ADEC_BUF_MULTI_SINK, ADEC_BUF_INTRA, __BUF_END())
ADEC_MODULE_DEF(ADEC_MODULE_ENC_0,			"ENC_0",		ADEC_MOD_TYPE_ENCODER,		ADEC_CORE_DSP0,	1, 1, ADEC_BUF_INTRA, __BUF_END())
ADEC_MODULE_DEF(ADEC_MODULE_ENC_1,			"ENC_1",		ADEC_MOD_TYPE_ENCODER,		ADEC_CORE_DSP0,	1, 1, ADEC_BUF_INTRA, __BUF_END())

//             +--------------------------------------------------------------------------------------------------------
//             | PCMCVT(pcm converter) Module Definition
//             +------------------------+-----------+-----------------------+---------------+---------------------------
//             | ID						| NAME		| TYPE					| LOCATION		| BUFFERS
//             +------------------------+-----------+-----------------------+---------------+---------------------------
//for dsp0
ADEC_MODULE_DEF(ADEC_MODULE_PCMCVT_D0,		"PCMCVT_D0",	ADEC_MOD_TYPE_PCMCVT,		ADEC_CORE_DSP0,	1, 1, ADEC_BUF_MULTI_SINK, __BUF_END())
ADEC_MODULE_DEF(ADEC_MODULE_PCMCVT_D1,		"PCMCVT_D1",	ADEC_MOD_TYPE_PCMCVT,		ADEC_CORE_DSP0,	1, 1, ADEC_BUF_MULTI_SINK, __BUF_END())
ADEC_MODULE_DEF(ADEC_MODULE_PCMCVT_D2,		"PCMCVT_D2",	ADEC_MOD_TYPE_NOT_DEF,		ADEC_CORE_DSP0,	1, 1, ADEC_BUF_MULTI_SINK, __BUF_END())
ADEC_MODULE_DEF(ADEC_MODULE_PCMCVT_D3,		"PCMCVT_D3",	ADEC_MOD_TYPE_NOT_DEF,		ADEC_CORE_DSP0,	1, 1, ADEC_BUF_MULTI_SINK, __BUF_END())

//for dsp1
ADEC_MODULE_DEF(ADEC_MODULE_PCMCVT_P0,		"PCMCVT_P0",	ADEC_MOD_TYPE_PCMCVT,		ADEC_CORE_DSP1,	1, 1, ADEC_BUF_MULTI_SINK, __BUF_END())
ADEC_MODULE_DEF(ADEC_MODULE_PCMCVT_P1,		"PCMCVT_P1",	ADEC_MOD_TYPE_PCMCVT,		ADEC_CORE_DSP1,	1, 1, ADEC_BUF_MULTI_SINK, __BUF_END())
ADEC_MODULE_DEF(ADEC_MODULE_PCMCVT_P2,		"PCMCVT_P2",	ADEC_MOD_TYPE_PCMCVT,		ADEC_CORE_DSP1,	1, 1, ADEC_BUF_MULTI_SINK, __BUF_END())
ADEC_MODULE_DEF(ADEC_MODULE_PCMCVT_P3,		"PCMCVT_P3",	ADEC_MOD_TYPE_PCMCVT,		ADEC_CORE_DSP1,	1, 1, ADEC_BUF_MULTI_SINK, __BUF_END())
ADEC_MODULE_DEF(ADEC_MODULE_PCMCVT_P4,		"PCMCVT_P4",	ADEC_MOD_TYPE_PCMCVT,		ADEC_CORE_DSP1,	1, 1, ADEC_BUF_MULTI_SINK, __BUF_END())
ADEC_MODULE_DEF(ADEC_MODULE_PCMCVT_P5,		"PCMCVT_P5",	ADEC_MOD_TYPE_PCMCVT,		ADEC_CORE_DSP1,	1, 1, ADEC_BUF_MULTI_SINK, __BUF_END())
ADEC_MODULE_DEF(ADEC_MODULE_PCMCVT_P6,		"PCMCVT_P6",	ADEC_MOD_TYPE_PCMCVT,		ADEC_CORE_DSP1,	1, 1, ADEC_BUF_MULTI_SINK, __BUF_END())
ADEC_MODULE_DEF(ADEC_MODULE_PCMCVT_P7,		"PCMCVT_P7",	ADEC_MOD_TYPE_PCMCVT,		ADEC_CORE_DSP1,	1, 1, ADEC_BUF_MULTI_SINK, __BUF_END())
ADEC_MODULE_DEF(ADEC_MODULE_PCMCVT_P8,		"PCMCVT_P8",	ADEC_MOD_TYPE_PCMCVT,		ADEC_CORE_DSP1,	1, 1, ADEC_BUF_MULTI_SINK, __BUF_END())
ADEC_MODULE_DEF(ADEC_MODULE_PCMCVT_P9,		"PCMCVT_P9",	ADEC_MOD_TYPE_PCMCVT,		ADEC_CORE_DSP1,	1, 1, ADEC_BUF_MULTI_SINK, __BUF_END())

//SOLA
ADEC_MODULE_DEF(ADEC_MODULE_SOLA_0,			"SOLA_0",		ADEC_MOD_TYPE_SOLA,			ADEC_CORE_DSP1,	1, 1, ADEC_BUF_MULTI_SINK, __BUF_END())
ADEC_MODULE_DEF(ADEC_MODULE_SOLA_1,			"SOLA_1",		ADEC_MOD_TYPE_SOLA,			ADEC_CORE_DSP1,	1, 1, ADEC_BUF_MULTI_SINK, __BUF_END())


//inputctrl
#if 1
ADEC_MODULE_DEF(ADEC_MODULE_INPUTCTRL_0,	"INCTRL_0",		ADEC_MOD_TYPE_INPUTCTRL,	ADEC_CORE_DSP1,	1, 1, ADEC_BUF_MULTI_SINK, __BUF_END())
ADEC_MODULE_DEF(ADEC_MODULE_INPUTCTRL_1,	"INCTRL_1",		ADEC_MOD_TYPE_INPUTCTRL,	ADEC_CORE_DSP1,	1, 1, ADEC_BUF_MULTI_SINK, __BUF_END())
ADEC_MODULE_DEF(ADEC_MODULE_INPUTCTRL_2,	"INCTRL_2",		ADEC_MOD_TYPE_INPUTCTRL,	ADEC_CORE_DSP1,	1, 1, ADEC_BUF_MULTI_SINK, __BUF_END())
ADEC_MODULE_DEF(ADEC_MODULE_INPUTCTRL_3,	"INCTRL_3",		ADEC_MOD_TYPE_INPUTCTRL,	ADEC_CORE_DSP1,	1, 1, ADEC_BUF_MULTI_SINK, __BUF_END())
ADEC_MODULE_DEF(ADEC_MODULE_INPUTCTRL_4,	"INCTRL_4",		ADEC_MOD_TYPE_INPUTCTRL,	ADEC_CORE_DSP1,	1, 1, ADEC_BUF_MULTI_SINK, __BUF_END())
ADEC_MODULE_DEF(ADEC_MODULE_INPUTCTRL_5,	"INCTRL_5",		ADEC_MOD_TYPE_INPUTCTRL,	ADEC_CORE_DSP1,	1, 1, ADEC_BUF_MULTI_SINK, __BUF_END())
ADEC_MODULE_DEF(ADEC_MODULE_INPUTCTRL_6,	"INCTRL_6",		ADEC_MOD_TYPE_INPUTCTRL,	ADEC_CORE_DSP1,	1, 1, ADEC_BUF_MULTI_SINK, __BUF_END())
ADEC_MODULE_DEF(ADEC_MODULE_INPUTCTRL_7,	"INCTRL_7",		ADEC_MOD_TYPE_INPUTCTRL,	ADEC_CORE_DSP1,	1, 1, ADEC_BUF_MULTI_SINK, __BUF_END())
ADEC_MODULE_DEF(ADEC_MODULE_INPUTCTRL_8,	"INCTRL_8",		ADEC_MOD_TYPE_INPUTCTRL,	ADEC_CORE_DSP1,	1, 1, ADEC_BUF_MULTI_SINK, __BUF_END())
ADEC_MODULE_DEF(ADEC_MODULE_INPUTCTRL_9,	"INCTRL_9",		ADEC_MOD_TYPE_INPUTCTRL,	ADEC_CORE_DSP1,	1, 1, ADEC_BUF_MULTI_SINK, __BUF_END())
ADEC_MODULE_DEF(ADEC_MODULE_INPUTCTRL_10,	"INCTRL_10",	ADEC_MOD_TYPE_INPUTCTRL,	ADEC_CORE_DSP1,	1, 1, ADEC_BUF_MULTI_SINK, __BUF_END())
ADEC_MODULE_DEF(ADEC_MODULE_INPUTCTRL_11,	"INCTRL_11",	ADEC_MOD_TYPE_INPUTCTRL,	ADEC_CORE_DSP1,	1, 1, ADEC_BUF_MULTI_SINK, __BUF_END())
#endif


//             +--------------------------------------------------------------------------------------------------------
//             | Lip Sync Module Definition
//             +------------------------+-----------+-----------------------+---------------+---------------------------
//             | ID						| NAME		| TYPE					| LOCATION		| BUFFERS
//             +------------------------+-----------+-----------------------+---------------+---------------------------

#if 0
ADEC_MODULE_DEF(ADEC_MODULE_LIP_0,			"LIP_0",		ADEC_MOD_TYPE_LIPSYNC,		ADEC_CORE_DSP1,	1, 1, ADEC_BUF_MULTI_SINK, __BUF_END())
ADEC_MODULE_DEF(ADEC_MODULE_LIP_1,			"LIP_1",		ADEC_MOD_TYPE_LIPSYNC,		ADEC_CORE_DSP1,	1, 1, ADEC_BUF_MULTI_SINK, __BUF_END())
ADEC_MODULE_DEF(ADEC_MODULE_LIP_2,			"LIP_2",		ADEC_MOD_TYPE_LIPSYNC,		ADEC_CORE_DSP1,	1, 1, ADEC_BUF_MULTI_SINK, __BUF_END())
ADEC_MODULE_DEF(ADEC_MODULE_LIP_3,			"LIP_3",		ADEC_MOD_TYPE_LIPSYNC,		ADEC_CORE_DSP1,	1, 1, ADEC_BUF_MULTI_SINK, __BUF_END())
ADEC_MODULE_DEF(ADEC_MODULE_LIP_4,			"LIP_4",		ADEC_MOD_TYPE_LIPSYNC,		ADEC_CORE_DSP1,	1, 1, ADEC_BUF_MULTI_SINK, __BUF_END())
ADEC_MODULE_DEF(ADEC_MODULE_LIP_5,			"LIP_5",		ADEC_MOD_TYPE_LIPSYNC,		ADEC_CORE_DSP1,	1, 1, ADEC_BUF_MULTI_SINK, __BUF_END())
ADEC_MODULE_DEF(ADEC_MODULE_LIP_6,			"LIP_6",		ADEC_MOD_TYPE_LIPSYNC,		ADEC_CORE_DSP1,	1, 1, ADEC_BUF_MULTI_SINK, __BUF_END())
ADEC_MODULE_DEF(ADEC_MODULE_LIP_7,			"LIP_7",		ADEC_MOD_TYPE_LIPSYNC,		ADEC_CORE_DSP1,	1, 1, ADEC_BUF_MULTI_SINK, __BUF_END())
ADEC_MODULE_DEF(ADEC_MODULE_LIP_8,			"LIP_8",		ADEC_MOD_TYPE_LIPSYNC,		ADEC_CORE_DSP1,	1, 1, ADEC_BUF_MULTI_SINK, __BUF_END())
ADEC_MODULE_DEF(ADEC_MODULE_LIP_9,			"LIP_9",		ADEC_MOD_TYPE_LIPSYNC,		ADEC_CORE_DSP1,	1, 1, ADEC_BUF_MULTI_SINK, __BUF_END())
ADEC_MODULE_DEF(ADEC_MODULE_LIP_10,			"LIP_10",		ADEC_MOD_TYPE_LIPSYNC,		ADEC_CORE_DSP1,	1, 1, ADEC_BUF_MULTI_SINK, __BUF_END())
ADEC_MODULE_DEF(ADEC_MODULE_LIP_11,			"LIP_11",		ADEC_MOD_TYPE_LIPSYNC,		ADEC_CORE_DSP1,	1, 1, ADEC_BUF_MULTI_SINK, __BUF_END())
#endif
//             +--------------------------------------------------------------------------------------------------------
//             | Mixer Module Definition
//             +------------------------+-----------+-----------------------+---------------+---------------------------
//             | ID						| NAME		| TYPE					| LOCATION		| BUFFERS
//             +------------------------+-----------+-----------------------+---------------+---------------------------
ADEC_MODULE_DEF(ADEC_MODULE_MIX_0,			"MIX_0",		ADEC_MOD_TYPE_MIXER,		ADEC_CORE_DSP1,	10, 1, ADEC_BUF_MULTI_SINK, __BUF_END())
ADEC_MODULE_DEF(ADEC_MODULE_MIX_1,			"MIX_1",		ADEC_MOD_TYPE_NOT_DEF,		ADEC_CORE_DSP1,	10, 1, ADEC_BUF_MULTI_SINK, __BUF_END())
ADEC_MODULE_DEF(ADEC_MODULE_MIX_2,			"MIX_2",		ADEC_MOD_TYPE_NOT_DEF,		ADEC_CORE_DSP1,	10, 1, ADEC_BUF_MULTI_SINK, __BUF_END())
ADEC_MODULE_DEF(ADEC_MODULE_MIX_3,			"MIX_3",		ADEC_MOD_TYPE_NOT_DEF,		ADEC_CORE_DSP1,	10, 1, ADEC_BUF_MULTI_SINK, __BUF_END())
ADEC_MODULE_DEF(ADEC_MODULE_MIX_4,			"MIX_4",		ADEC_MOD_TYPE_NOT_DEF,		ADEC_CORE_DSP1,	10, 1, ADEC_BUF_MULTI_SINK, __BUF_END())

//             +--------------------------------------------------------------------------------------------------------
//             | SE(Sound Engine) Module Definition
//             +------------------------+-----------+-----------------------+---------------+---------------------------
//             | ID						| NAME		| TYPE					| LOCATION		| BUFFERS
//             +------------------------+-----------+-----------------------+---------------+---------------------------
ADEC_MODULE_DEF(ADEC_MODULE_SE,				"SE",			ADEC_MOD_TYPE_SE,			ADEC_CORE_DSP1,	1, 1, ADEC_BUF_MULTI_SINK, __BUF_END())

//             +--------------------------------------------------------------------------------------------------------
//             | Output Ctrl Module Definition
//             +------------------------+-----------+-----------------------+---------------+---------------------------
//             | ID						| NAME		| TYPE					| LOCATION		| BUFFERS
//             +------------------------+-----------+-----------------------+---------------+---------------------------
ADEC_MODULE_DEF(ADEC_MODULE_OUT_CTRL_0,		"OUT_SPK",		ADEC_MOD_TYPE_OUTCTRL,		ADEC_CORE_DSP1,	1, 1, ADEC_BUF_INTRA, __BUF_END())
ADEC_MODULE_DEF(ADEC_MODULE_OUT_CTRL_1,		"OUT_HP",		ADEC_MOD_TYPE_OUTCTRL,		ADEC_CORE_DSP1,	1, 1, ADEC_BUF_INTRA, __BUF_END())
ADEC_MODULE_DEF(ADEC_MODULE_OUT_CTRL_2,		"OUT_SCART",	ADEC_MOD_TYPE_OUTCTRL,		ADEC_CORE_DSP1,	1, 1, ADEC_BUF_INTRA, __BUF_END())
ADEC_MODULE_DEF(ADEC_MODULE_OUT_CTRL_3,		"OUT_SPDIF",	ADEC_MOD_TYPE_OUTCTRL,		ADEC_CORE_DSP1,	1, 1, ADEC_BUF_INTRA, __BUF_END())
ADEC_MODULE_DEF(ADEC_MODULE_ESOUT_CTRL,		"OUT_ES",  		ADEC_MOD_TYPE_ESOUTCTRL,	ADEC_CORE_DSP1,	1, 1, ADEC_BUF_INTRA, __BUF_END())

ADEC_MODULE_DEF(ADEC_MODULE_DTO_0,			"DTO_0",		ADEC_MOD_TYPE_DTOCTRL,		ADEC_CORE_DSP1,	0, 0, __BUF_END())
ADEC_MODULE_DEF(ADEC_MODULE_DTO_1,			"DTO_1",		ADEC_MOD_TYPE_DTOCTRL,		ADEC_CORE_DSP1,	0, 0, __BUF_END())
ADEC_MODULE_DEF(ADEC_MODULE_DTO_2,			"DTO_2",		ADEC_MOD_TYPE_NOT_DEF,		ADEC_CORE_DSP1,	0, 0, __BUF_END())
ADEC_MODULE_DEF(ADEC_MODULE_DTO_3,			"DTO_3",		ADEC_MOD_TYPE_NOT_DEF,		ADEC_CORE_DSP1,	0, 0, __BUF_END())

ADEC_MODULE_DEF(ADEC_MODULE_CTRLSRC_0,		"CTRLSRC_0",	ADEC_MOD_TYPE_CTRLSRC,		ADEC_CORE_DSP1, 0, 0, __BUF_END())
ADEC_MODULE_DEF(ADEC_MODULE_CTRLSRC_1,		"CTRLSRC_1",	ADEC_MOD_TYPE_CTRLSRC,		ADEC_CORE_DSP1, 0, 0, __BUF_END())
ADEC_MODULE_DEF(ADEC_MODULE_CTRLSRC_2,		"CTRLSRC_2",	ADEC_MOD_TYPE_CTRLSRC,		ADEC_CORE_DSP1, 0, 0, __BUF_END())

ADEC_MODULE_DEF(ADEC_MODULE_RTS,    	    "RTS",			ADEC_MOD_TYPE_RTS,  		ADEC_CORE_DSP1, 0, 0, __BUF_END())

//             +--------------------------------------------------------------------------------------------------------
//             | Sink Module Definition
//             +------------------------+-----------+-----------------------+---------------+---------------------------
//             | ID						| NAME		| TYPE					| LOCATION		| BUFFERS
//             +------------------------+-----------+-----------------------+---------------+---------------------------
ADEC_MODULE_DEF(ADEC_MODULE_OUT_PCM,		"PCM",			ADEC_MOD_TYPE_SINK,			ADEC_CORE_DSP1,	3, 0, __BUF_END())
ADEC_MODULE_DEF(ADEC_MODULE_OUT_PCMSUB,		"PCMSUB",		ADEC_MOD_TYPE_SINK,			ADEC_CORE_DSP1,	1, 0, __BUF_END())
ADEC_MODULE_DEF(ADEC_MODULE_OUT_SPDIF,		"SPDIF",		ADEC_MOD_TYPE_SINK,			ADEC_CORE_DSP1,	1, 0, __BUF_END())

// For test. DSP0 PCM/SPDIF out.
ADEC_MODULE_DEF(ADEC_MODULE_OUT_PCM_0,		"PCM_0",		ADEC_MOD_TYPE_SINK,			ADEC_CORE_DSP0,	3, 0, __BUF_END())
ADEC_MODULE_DEF(ADEC_MODULE_OUT_SPDIF_0,	"SPDIF_0",		ADEC_MOD_TYPE_SINK,			ADEC_CORE_DSP0,	1, 0, __BUF_END())

ADEC_MODULE_DEF(ADEC_MODULE_BYPASS_D0,		"BYPASS_D0",	ADEC_MOD_TYPE_BYPASS,		ADEC_CORE_DSP0,	1, 1, ADEC_BUF_MULTI_SINK, __BUF_END())
ADEC_MODULE_DEF(ADEC_MODULE_BYPASS_D1,		"BYPASS_D1",	ADEC_MOD_TYPE_BYPASS,		ADEC_CORE_DSP0,	1, 1, ADEC_BUF_MULTI_SINK, __BUF_END())
ADEC_MODULE_DEF(ADEC_MODULE_BYPASS_D2,		"BYPASS_D2",	ADEC_MOD_TYPE_BYPASS,		ADEC_CORE_DSP0,	1, 1, ADEC_BUF_MULTI_SINK, __BUF_END())
ADEC_MODULE_DEF(ADEC_MODULE_BYPASS_D3,		"BYPASS_D3",	ADEC_MOD_TYPE_BYPASS,		ADEC_CORE_DSP0,	1, 1, ADEC_BUF_MULTI_SINK, __BUF_END())
ADEC_MODULE_DEF(ADEC_MODULE_BYPASS_P0,		"BYPASS_P0",	ADEC_MOD_TYPE_BYPASS,		ADEC_CORE_DSP1,	1, 1, ADEC_BUF_MULTI_SINK, __BUF_END())
ADEC_MODULE_DEF(ADEC_MODULE_BYPASS_P1,		"BYPASS_P1",	ADEC_MOD_TYPE_BYPASS,		ADEC_CORE_DSP1,	1, 1, ADEC_BUF_MULTI_SINK, __BUF_END())


// For dump. buffer in DSP
//ADEC_MODULE_DEF(ADEC_MODULE_DUMP_DSP0,"DUMP_DSP0",		ADEC_MOD_TYPE_BYPASS,		ADEC_CORE_DSP0,	1, 1, ADEC_BUF_MULTI_SINK, __BUF_END())
//ADEC_MODULE_DEF(ADEC_MODULE_DUMP_DSP1,"DUMP_DSP1",		ADEC_MOD_TYPE_BYPASS,		ADEC_CORE_DSP1,	1, 1, ADEC_BUF_MULTI_SINK, __BUF_END())

//ADEC_MODULE_DEF(ADEC_MODULE_HWSRC_DSP0,"HWSRC_DSP0",		ADEC_MOD_TYPE_SINK,			ADEC_CORE_DSP0,	1, 1, ADEC_BUF_INTRA,__BUF_END())
//ADEC_MODULE_DEF(ADEC_MODULE_HWSRC_DSP1,"HWSRC_DSP1",		ADEC_MOD_TYPE_SINK,			ADEC_CORE_DSP1,	1, 1, ADEC_BUF_INTRA,__BUF_END())

// EXT BUF
ADEC_MODULE_DEF(ADEC_MODULE_EXT_BUF_0,	"EXT_BUF_0",	ADEC_MOD_TYPE_EXT_BUF,	ADEC_CORE_ARM,	1, 1, ADEC_BUF_INTRA, __BUF_END())
ADEC_MODULE_DEF(ADEC_MODULE_EXT_BUF_1,	"EXT_BUF_1",	ADEC_MOD_TYPE_EXT_BUF,	ADEC_CORE_ARM,	1, 1, ADEC_BUF_INTRA, __BUF_END())
ADEC_MODULE_DEF(ADEC_MODULE_EXT_BUF_2,	"EXT_BUF_2",	ADEC_MOD_TYPE_EXT_BUF,	ADEC_CORE_ARM,	1, 1, ADEC_BUF_INTRA, __BUF_END())
ADEC_MODULE_DEF(ADEC_MODULE_EXT_BUF_3,	"EXT_BUF_3",	ADEC_MOD_TYPE_EXT_BUF,	ADEC_CORE_ARM,	1, 1, ADEC_BUF_INTRA, __BUF_END())
ADEC_MODULE_DEF(ADEC_MODULE_EXT_BUF_4,	"EXT_BUF_4",	ADEC_MOD_TYPE_EXT_BUF,	ADEC_CORE_ARM,	1, 1, ADEC_BUF_INTRA, __BUF_END())
ADEC_MODULE_DEF(ADEC_MODULE_EXT_BUF_5,	"EXT_BUF_5",	ADEC_MOD_TYPE_EXT_BUF,	ADEC_CORE_ARM,	1, 1, ADEC_BUF_INTRA, __BUF_END())
ADEC_MODULE_DEF(ADEC_MODULE_EXT_BUF_6,	"EXT_BUF_6",	ADEC_MOD_TYPE_EXT_BUF,	ADEC_CORE_ARM,	1, 1, ADEC_BUF_INTRA, __BUF_END())
ADEC_MODULE_DEF(ADEC_MODULE_EXT_BUF_7,	"EXT_BUF_7",	ADEC_MOD_TYPE_EXT_BUF,	ADEC_CORE_ARM,	1, 1, ADEC_BUF_INTRA, __BUF_END())
ADEC_MODULE_DEF(ADEC_MODULE_EXT_BUF_8,	"EXT_BUF_8",	ADEC_MOD_TYPE_EXT_BUF,	ADEC_CORE_ARM,	1, 1, ADEC_BUF_INTRA, __BUF_END())
ADEC_MODULE_DEF(ADEC_MODULE_EXT_BUF_9,	"EXT_BUF_9",	ADEC_MOD_TYPE_EXT_BUF,	ADEC_CORE_ARM,	1, 1, ADEC_BUF_INTRA, __BUF_END())

ADEC_MODULE_DEF(ADEC_MODULE_FS23TIMER,	"FS23TIMER",	ADEC_MOD_TYPE_SINK,				ADEC_CORE_DSP1,	0, 0, __BUF_END())


ADEC_MODULE_DEF(ADEC_MODULE_PCMCVT_P10,		"PCMCVT_P10",	ADEC_MOD_TYPE_PCMCVT,		ADEC_CORE_DSP1,	1, 1, ADEC_BUF_MULTI_SINK, __BUF_END())
ADEC_MODULE_DEF(ADEC_MODULE_PCMCVT_P11,		"PCMCVT_P11",	ADEC_MOD_TYPE_PCMCVT,		ADEC_CORE_DSP1,	1, 1, ADEC_BUF_MULTI_SINK, __BUF_END())


//#endif //__ADEC_MODULE_DEF_H__
/** @} */
