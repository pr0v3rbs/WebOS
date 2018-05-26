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

#ifndef __MODULE_CMD_PCMOUT_H__
#define __MODULE_CMD_PCMOUT_H__

#include "imc/adec_imc_cmd.h"


#define PCMOUT_CMD_SET_SPKCH	            ADEC_CMD(ADEC_CMD_MOD_ID(CMD_PCMOUT, 0x00), PcmoutCmdSetSpkch)
#define PCMOUT_CMD_SET_TEST_DBG				ADEC_CMD(ADEC_CMD_MOD_ID(CMD_PCMOUT, 0x01), PcmoutCmdSetTestDbg)
#define PCMOUT_CMD_GET_MASTERSYSTEMDELAY	ADEC_CMD(ADEC_CMD_MOD_ID(CMD_PCMOUT, 0x02), PcmoutCmdGetMasterSD)
#define PCMOUT_CMD_SET_SPKFS				ADEC_CMD(ADEC_CMD_MOD_ID(CMD_PCMOUT, 0x03), PcmoutCmdSetSpkfs)

/**
* Set speaker channel command를 정의한 구조체.
*/
typedef struct _PcmoutCmdSetSpkch
{
	unsigned int channel;			///< 2, 4
}PcmoutCmdSetSpkch;

/**
* Set test debug command를 정의한 구조체.
*/
typedef struct _PcmoutCmdSetTestDbg
{
	unsigned int index;				///< 0 : original, 1: 200ms tic 2: pcmout print
}PcmoutCmdSetTestDbg;

/**
* Set master SD command를 정의한 구조체.
*/
typedef struct _PcmoutCmdGetMasterSD
{
    unsigned int module_id;      	///< The module ID
}PcmoutCmdGetMasterSD;

/**
* Set speaker Fs command를 정의한 구조체.
*/
typedef struct _PcmoutCmdSetSpkfs
{
	unsigned int fs;			///< 48000, 96000
}PcmoutCmdSetSpkfs;

#endif // #ifndef __MODULE_CMD_SPDIF_H__

