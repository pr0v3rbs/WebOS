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


#ifndef __MODULE_CMD_DTSENC_H__
#define __MODULE_CMD_DTSENC_H__

#include "imc/adec_imc_cmd.h"

#define DTSENC_CMD_SET_PARAM        ADEC_CMD(ADEC_CMD_MOD_ID(CMD_DTSENC, 0x00), DtsEncCmdSetParam)

#ifdef IC_CERTIFICATION
typedef struct _CertiEncParamField
{
    unsigned int lfe            :4;
    unsigned int surround       :4;
    unsigned int front          :4;
    unsigned int quiet          :4;
    unsigned int i_pcm_wd_sz    :8;
    unsigned int reserved       :8;
} CertiEncParamField;

typedef union _CertiEncParamFlag
{
    unsigned int        value;
    CertiEncParamField  field;
} CertiEncParamFlag;
#endif // #ifdef IC_CERTIFICATION

typedef struct _DtsEncCmdSetParam
{
    unsigned int i_samp_freq;                ///< Sampling frequency of the output 

#ifndef IC_CERTIFICATION
    unsigned int i_pcm_wd_sz;                ///< Sample size of a sample in bits 
    unsigned int      ch_layout;             ///< Encoder channel layout(Front/Surround/LFE) 
#else
    CertiEncParamFlag certi_param;           ///< IC Certification parameter
#endif // #ifndef IC_CERTIFICATION
} DtsEncCmdSetParam;

#endif // #ifndef __MODULE_CMD_DTSENC_H__

