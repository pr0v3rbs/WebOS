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


#ifndef __MODULE_CMD_WMA_H__
#define __MODULE_CMD_WMA_H__

#include "imc/adec_imc_cmd.h"

#define WMA_CMD_SET_PARAM		ADEC_CMD(ADEC_CMD_MOD_ID(CMD_WMA, 0x00), WmaCmdSetParam)

/**
* Set parameters command를 정의한 구조체.
*/
typedef struct _WmaCmdSetParam
{
    unsigned int samp_freq;
    unsigned int num_chan;
    unsigned int bits_per_sampS;
    unsigned int fmt_tag;
    unsigned int blk_align;
    unsigned int encode_opt;
    unsigned int avg_byterate;
    unsigned int ch_mask;
    unsigned int adv_encode_opt;
    unsigned int adv_encode_opt2;
} WmaCmdSetParam;

#endif // #ifndef __MODULE_CMD_WMA_H__

