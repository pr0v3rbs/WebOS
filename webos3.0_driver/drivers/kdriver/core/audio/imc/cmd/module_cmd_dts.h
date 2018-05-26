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


#ifndef __MODULE_CMD_DTS_H__
#define __MODULE_CMD_DTS_H__

#include "imc/adec_imc_cmd.h"

#define DTS_CMD_SET_PARAM       ADEC_CMD(ADEC_CMD_MOD_ID(CMD_DTS, 0x00), DtsCmdSetParam)

/**
 * Set prameter를 정의한 구조체.
 */
typedef struct _DtsCmdSetParam
{
#ifdef IC_CERTIFICATION
    unsigned int certi_param;          ///< IC Certification parameter
    unsigned int spkrout_mode;         ///< Instructs player to output corresponding Channels : 1(C), 2(LR), 4(LsRs) 8(LFE1)
    unsigned int lfemix_mode;          ///< Instructs the decoder to mix the LFE data : 0 : OFF(default), 1 : ON
    unsigned int drc_percent;          ///< Instructs the player to use 'R'% for DRC : 0(default) ~ 100
#endif // #ifdef IC_CERTIFICATION
} DtsCmdSetParam;

#endif // #ifndef __MODULE_CMD_DTS_H__

