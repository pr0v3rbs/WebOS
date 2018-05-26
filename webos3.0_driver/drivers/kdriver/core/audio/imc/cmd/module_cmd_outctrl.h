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

#define OUTCTRL_CMD_SET_ALL						ADEC_CMD(ADEC_CMD_MOD_ID(CMD_OUT_CTRL, 0x00), OutctrlCmdSetAll)
#define OUTCTRL_CMD_SET_GAIN					ADEC_CMD(ADEC_CMD_MOD_ID(CMD_OUT_CTRL, 0x01), OutctrlCmdSetGain)
#define OUTCTRL_CMD_SET_BALANCED				ADEC_CMD(ADEC_CMD_MOD_ID(CMD_OUT_CTRL, 0x02), OutctrlCmdSetBalanced)
#define OUTCTRL_CMD_SET_MUTE					ADEC_CMD(ADEC_CMD_MOD_ID(CMD_OUT_CTRL, 0x03), OutctrlCmdSetMute)
#define OUTCTRL_CMD_SET_DELAY					ADEC_CMD(ADEC_CMD_MOD_ID(CMD_OUT_CTRL, 0x04), OutctrlCmdSetDelay)
#define OUTCTRL_CMD_SET_PCMOUTMODE				ADEC_CMD(ADEC_CMD_MOD_ID(CMD_OUT_CTRL, 0x05), OutctrlCmdSetPcmoutmode)
#define OUTCTRL_CMD_SET_CHANNEL					ADEC_CMD(ADEC_CMD_MOD_ID(CMD_OUT_CTRL, 0x06), OutctrlCmdSetChannel)
#define OUTCTRL_CMD_GET_MASTERSYSTEMDELAY		ADEC_CMD_SIMP(ADEC_CMD_MOD_ID(CMD_OUT_CTRL, 0x07))

/**
* Set all parameters command를 정의한 구조체.
*/
typedef struct _OutctrlCmdSetAll
{
	int Delay;              	///< ?
	int	Fs;						///<
	int Gain;					///<
	int GainEnable ;			///< 1 : Gain Enable, 0: Gain disable
    int Balanced;           	///< -50~ 50
	int Mute;					///< 1 : Mute on, 0: Mute off
}OutctrlCmdSetAll;

/**
* Set gain command를 정의한 구조체.
*/
typedef struct _OutctrlCmdSetGain
{
	int Gain;              		///< ?
	int GainEnable;         	///<
}OutctrlCmdSetGain;

/**
* Set balance command를 정의한 구조체.
*/
typedef struct _OutctrlCmdSetBalanced
{
	int Balanced;              	///< ?
}OutctrlCmdSetBalanced;

/**
* Set delay command를 정의한 구조체.
*/
typedef struct _OutctrlCmdSetDelay
{
	int Delay;              	///< ?
	int Fs;						///<
}OutctrlCmdSetDelay;

/**
* Set mute command를 정의한 구조체.
*/
typedef struct _OutctrlCmdSetMute
{
	int Mute;              		///< 1 : Mute on, 0: Mute off
}OutctrlCmdSetMute;

/**
* Set pcm output mode command를 정의한 구조체.
*/
typedef struct _OutctrlCmdSetPcmoutmode
{
	int OutMode; 				///< 0 : LR , 1 : LL, 2: RR, 3: L+R
}OutctrlCmdSetPcmoutmode;

/**
* Set channel command를 정의한 구조체.
*/
typedef struct _OutctrlCmdSetChannel
{
	unsigned int Ch;			///< must be even 2, 4 ...
}OutctrlCmdSetChannel;


