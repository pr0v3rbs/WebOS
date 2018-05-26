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

#ifndef __MODULE_CMD_INPUTCTRL_H__
#define __MODULE_CMD_INPUTCTRL_H__

#include "imc/adec_imc_cmd.h"

#define INPUTCTRL_CMD_SET_DELAY				ADEC_CMD(ADEC_CMD_MOD_ID(CMD_INPUT_CTRL, 0x01), InputctrlCmdSetDelay)
#define INPUTCTRL_CMD_SET_GAIN				ADEC_CMD(ADEC_CMD_MOD_ID(CMD_INPUT_CTRL, 0x02), InputctrlCmdSetGain)
#define	INPUTCTRL_CMD_SET_MUTE				ADEC_CMD(ADEC_CMD_MOD_ID(CMD_INPUT_CTRL, 0x03), InputctrlCmdSetMute)
#define INPUTCTRL_CMD_SET_CH_GAIN			ADEC_CMD(ADEC_CMD_MOD_ID(CMD_INPUT_CTRL, 0x04), InputctrlCmdSetChGain)

#define INPUTCTRL_CMD_GET_SYSTEMDELAY		ADEC_CMD_SIMP(ADEC_CMD_MOD_ID(CMD_INPUT_CTRL, 0x10))

/**
 * Set delay를 정의한 구조체.
 */
typedef struct _InputctrlCmdSetDelay{
	unsigned int delay;
}InputctrlCmdSetDelay;

/**
 * Set gain를 정의한 구조체.
 */
typedef struct _InputctrlCmdSetGain{
	unsigned int Gain;              
	unsigned int GainEnable;        
}InputctrlCmdSetGain;

/**
 * Set lip-sync mute를 정의한 구조체.
 */
typedef struct _InputctrlCmdSetMute{
	unsigned int Mute;					///< 0 is mute off, 1 is mute on
}InputctrlCmdSetMute;

/**
 * Set lip-sync channel별 gain를 정의한 구조체.
 */
typedef struct _InputctrlCmdSetChGain{
	unsigned int LeftGain;              
	unsigned int RightGain; 	       
}InputctrlCmdSetChGain;

#endif
