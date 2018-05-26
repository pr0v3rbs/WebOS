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

#include "imc/adec_imc_cmd.h"

#define ESOUTCTRL_CMD_SET_ALL			ADEC_CMD(ADEC_CMD_MOD_ID(CMD_ES_OUT_CTRL, 0x00), EsoutctrlCmdSetAll)
#define ESOUTCTRL_CMD_SET_MUTE			ADEC_CMD(ADEC_CMD_MOD_ID(CMD_ES_OUT_CTRL, 0x01), EsoutctrlCmdSetMute)
#define ESOUTCTRL_CMD_SET_DELAY			ADEC_CMD(ADEC_CMD_MOD_ID(CMD_ES_OUT_CTRL, 0x02), EsoutctrlCmdSetDelay)


/**
 * Set all prameter를 정의한 구조체.
 */
typedef struct _EsoutctrlCmdSetAll
{
	int Delay;              ///<
	int Fs;					///<
	int Mute;				///< 1 : Mute on, 0: Mute off
}EsoutctrlCmdSetAll;

/**
 * Set delay를 정의한 구조체.
 */
typedef struct _EsoutctrlCmdSetDelay
{
	int Delay;              ///<
	int Fs;					///<
}EsoutctrlCmdSetDelay;

/**
 * Set mute를 정의한 구조체.
 */
typedef struct _EsoutctrlCmdSetMute
{
	int Mute;              ///< 1 : Mute on, 0: Mute off
}EsoutctrlCmdSetMute;


