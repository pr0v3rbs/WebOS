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

#ifndef __MODULE_CMD_SRC_H__
#define __MODULE_CMD_SRC_H__

#include "imc/adec_imc_cmd.h"

#define PCMCVT_CMD_SET_FMT				ADEC_CMD(ADEC_CMD_MOD_ID(CMD_PCMCVT, 0x00), PcmcvtCmdSetFmt)
#define PCMCVT_CMD_SET_OVERPROTECT		ADEC_CMD(ADEC_CMD_MOD_ID(CMD_PCMCVT, 0x01), PcmcvtCmdSetOverProtect)
#define PCMCVT_CMD_SET_ALLFMT			ADEC_CMD(ADEC_CMD_MOD_ID(CMD_PCMCVT, 0x02), PcmcvtCmdSetAllfmt)
#define PCMCVT_CMD_CHANGE_FS			ADEC_CMD(ADEC_CMD_MOD_ID(CMD_PCMCVT, 0x03), PcmcvtCmdChangeFs)
#define PCMCVT_CMD_SET_GAIN				ADEC_CMD(ADEC_CMD_MOD_ID(CMD_PCMCVT, 0x04), PcmcvtCmdSetGain)
#define PCMCVT_CMD_SET_OUT_FS   		ADEC_CMD(ADEC_CMD_MOD_ID(CMD_PCMCVT, 0x05), PcmcvtCmdSetOutFs)
#define PCMCVT_CMD_SET_SRC_RUNNING_MODE ADEC_CMD(ADEC_CMD_MOD_ID(CMD_PCMCVT, 0x06), PcmcvtCmdSetSrcRunningMode)

#if defined(_DETECT_TONE_ENABLE) && (_DETECT_TONE_ENABLE == 1)
#define PCMCVT_CMD_SET_DETECT_TONE 		ADEC_CMD(ADEC_CMD_MOD_ID(CMD_PCMCVT, 0x07), PcmcvtCmdSetDetectTone)
#endif

#define PCMCVT_CMD_SET_OUT_SAMPLE		ADEC_CMD(ADEC_CMD_MOD_ID(CMD_PCMCVT, 0x08), PcmcvtCmdSetOutSample)

/**
* Set format command를 정의한 구조체.
*/
typedef struct _PcmcvtCmdSetFmt
{
	unsigned int InFs;					///< kHz : 8000 16000 24000 32000 44100 48000 96000 etc ...
	unsigned int OutFs;					///< kHz
	unsigned int InCh;					///< channel :
	unsigned int OutCh;					///< channel :
	unsigned int InFormat; 				///< pcm format : 8 16 24 32
	unsigned int OutFormat; 			///< pcm format : 8 16 24 32
}PcmcvtCmdSetFmt;

/**
* Set all format command를 정의한 구조체.
*/
typedef struct _PcmcvtCmdSetAllfmt
{
	unsigned int InFs;					///< kHz : 8000 16000 24000 32000 44100 48000 96000 etc ...
	unsigned int InCh;					///< channel : mono stereo & multi (Max 8ch)
	unsigned int InFormat; 				///< pcm format : 8 16 24 32
	unsigned int InEndian;  			///< 0 is little , 1 is big
    unsigned int InSigned ; 			///< 0 is signed , 1 is unsigned

	unsigned int OutFs;					///< kHz
	unsigned int OutCh;					///< channel : mono stereo
	unsigned int OutFormat; 			///< pcm format : 8 16 24 32
}PcmcvtCmdSetAllfmt;

/**
* Set overflow protection command를 정의한 구조체.
*/
typedef struct _PcmcvtCmdSetOverProtect
{
	// 0 - No Overflow Protection     (default)
	// 1 - Enable Overflow Protection (to capture)
	unsigned int over_protect;
}PcmcvtCmdSetOverProtect;

/**
* Set number of output samples command를 정의한 구조체.
*/
typedef struct _PcmcvtCmdSetOutSample
{
	unsigned int samples;				///< number of samples per 1ch
}PcmcvtCmdSetOutSample;

/**
* Set Fs change command를 정의한 구조체.
*/
typedef struct _PcmcvtCmdChangeFs
{
	unsigned int InFs;
	unsigned int OutFs;
}PcmcvtCmdChangeFs;

/**
* Set output Fs command를 정의한 구조체.
*/
typedef struct _PcmcvtCmdSetOutFs
{
    unsigned int OutFs;
}PcmcvtCmdSetOutFs;

/**
* Set gain command를 정의한 구조체.
*/
typedef struct _PcmcvtCmdSetGain
{
	int Gain;
    int GainEnable;
}PcmcvtCmdSetGain;

/**
* Set Fs of output command를 정의한 구조체.
*/
typedef struct _PcmcvtCmdSetOutFreq
{
    unsigned int out_freq;
}PcmcvtCmdSetOutFreq;

/**
* Set running mode command를 정의한 구조체.
*/
typedef struct _PcmcvtCmdSetRunningMode
{
    unsigned int mode;
}PcmcvtCmdSetSrcRunningMode;


#if defined(_DETECT_TONE_ENABLE) && (_DETECT_TONE_ENABLE == 1)
/**
* Set detect tone command를 정의한 구조체.
*/
typedef struct _PcmcvtCmdSetDetectTone
{
	unsigned int onoff;					///< 1 is on, 0 is off
}PcmcvtCmdSetDetectTone;
#endif

#endif
