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


#ifndef __MODULE_CMD_AACENC_H__
#define __MODULE_CMD_AACENC_H__

#include "imc/adec_imc_cmd.h"

#define AACENC_CMD_SET_PARAM	ADEC_CMD(ADEC_CMD_MOD_ID(CMD_AACENC, 0x00), AacEncCmdSetParam)

/**
 * Set param command를 정의한 구조체.
 */
typedef struct _AacEncCmdSetParam
{
    int i_samp_freq;               	///< Sampling frequency of the output 
    int i_pcm_wd_sz;                ///< Sample size of a sample in bits 
    int i_num_chan;                 ///< Number of encoded channels in the bitstream 
    int i_bitrate;                  ///< Data rate of the input stream 
    int i_ch_mode;                  ///< Channel mode : 0 : stereo(default), 1 : dual-mono 
} AacEncCmdSetParam;

#endif // #ifndef __MODULE_CMD_AACENC_H__

