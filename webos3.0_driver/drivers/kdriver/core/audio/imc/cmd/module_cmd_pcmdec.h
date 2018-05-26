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

#ifndef __MODULE_CMD_PCMDEC_H__
#define __MODULE_CMD_PCMDEC_H__

#include "imc/adec_imc_cmd.h"

#define PCMDEC_CMD_SET_PARAM		ADEC_CMD(ADEC_CMD_MOD_ID(CMD_PCMDEC, 0x00), PcmDecCmdSetParam)

/**
* Set parameters command를 정의한 구조체.
*/
typedef struct _PcmDecCmdSetParam
{
    unsigned int i_samp_freq;                ///< Sampling frequency of the output 
    unsigned int i_num_chan;                 ///< Number of encoded channels in the bitstream 
    unsigned int i_pcm_wd_sz;                ///< Sample size of a sample in bits 
    unsigned int i_endian;
    unsigned int i_signed;
} PcmDecCmdSetParam;

#endif // #ifndef __MODULE_CMD_PCMDEC_H__

