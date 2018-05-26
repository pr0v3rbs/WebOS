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

#ifndef __MODULE_CMD_SE_H__
#define __MODULE_CMD_SE_H__

#include "imc/adec_imc_cmd.h"

#define SE_CMD_SET_MODE				ADEC_CMD(ADEC_CMD_MOD_ID(CMD_SE, 0x00), SeCmdSetMode)
#define SE_CMD_SET_PARAM			ADEC_CMD(ADEC_CMD_MOD_ID(CMD_SE, 0x01), SeCmdSetParam)
#define SE_CMD_SET_BYPASS			ADEC_CMD(ADEC_CMD_MOD_ID(CMD_SE, 0x02), SeCmdSetBypass)
#define SE_CMD_SET_OUTCH			ADEC_CMD(ADEC_CMD_MOD_ID(CMD_SE, 0x03), SeCmdSetOutCh)
#define SE_CMD_SET_INOUTFS			ADEC_CMD(ADEC_CMD_MOD_ID(CMD_SE, 0x04), SeCmdSetInOutFs) //added by HAM
#define SE_CMD_GET_PARAM			ADEC_CMD(ADEC_CMD_MOD_ID(CMD_SE, 0x05), SeCmdGetParam)
#define SE_CMD_SET_DETECT_TONE 		ADEC_CMD(ADEC_CMD_MOD_ID(CMD_SE, 0x06), SeCmdSetDetectTone)

#define SE_CMD_PRINT_PARAM			ADEC_CMD(ADEC_CMD_MOD_ID(CMD_SE, 0x11),SeCmdSetParam)

/**
* Set mode command를 정의한 구조체.
*/
typedef struct _SeCmdSetMode{
	unsigned int Mode;
	unsigned int DeqMode;
	unsigned int PeqMode;
}SeCmdSetMode;

/**
* Set mode type을 정의한 enum.
*/
typedef enum _PARAMUPDATEMODE_TYPE{
	INITPARAM_ONLY = 0,
	VARPARAM_ONLY = 1,
	ALLPARAM =2,
}PARAMUPDATEMODE_TYPE;

/**
* Set parameter command를 정의한 구조체.
*/
typedef struct _SeCmdSetParam{
	unsigned int FnMode;					///< fn number
	unsigned int DataOption;				///< PARAMUPDATEMODE 0 : init_only, 1 : variables, 2 : all
//	unsigned int VarOption;					///< not used
//	unsigned int AccessMode;				///< not used
//	unsigned int NoParam;					///< not used
}SeCmdSetParam;

/**
* Get parameter command를 정의한 구조체.
*/
typedef struct _SeCmdGetParam{
	unsigned int FnMode;					///< fn number ('all' mode is not available.)
	unsigned int DataOption;				///< PARAMUPDATEMODE 0 : init_only, 1 : variables ('all' mode is not available.)
//	unsigned int VarOption;					//not used
//	unsigned int AccessMode;				//not used
//	unsigned int NoParam;					//not used
}SeCmdGetParam;

/**
* Set bypass mode command를 정의한 구조체.
*/
typedef struct _SeCmdSetBypass{
	unsigned int bypass;					///< 1 is Bypass
}SeCmdSetBypass;

/**
* Set output channel command를 정의한 구조체.
*/
typedef struct _SeCmdSetOutCh{
	unsigned int ch;						///< 2 or 4 channel
}SeCmdSetOutCh;

/**
* Set output Fs command를 정의한 구조체.
*/
typedef struct _SeCmdSetInOutFs{
	unsigned int in_fs;						///< 48000 Hz
	unsigned int out_fs;					///< 48000 or 96000Hz
}SeCmdSetInOutFs;

/**
* Set detect tone command를 정의한 구조체.
*/
typedef struct _SeCmdSetDetectTone
{
	unsigned int onoff;						///< 1 is on, 0 is off
}SeCmdSetDetectTone;
#endif
