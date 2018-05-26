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


#ifndef __MODULE_CMD_SPDIF_H__
#define __MODULE_CMD_SPDIF_H__

#include "imc/adec_imc_cmd.h"



#define SPDIF_CMD_SET_OUTFORMAT             		ADEC_CMD(ADEC_CMD_MOD_ID(CMD_SPDIF_OUT, 0x00), SpdifCmdSetOutmode)
#define SPDIF_CMD_SET_FMT_FOR_SOUNDBAR	    		ADEC_CMD(ADEC_CMD_MOD_ID(CMD_SPDIF_OUT, 0x01), SpdifCmdSetFmtForSoundbar)
#define SPDIF_CMD_SET_SCMS                  		ADEC_CMD(ADEC_CMD_MOD_ID(CMD_SPDIF_OUT, 0x02), SpdifCmdSetScms)
#define SPDIF_CMD_CLEAR_FMT_FOR_SOUNDBAR			ADEC_CMD_SIMP(ADEC_CMD_MOD_ID(CMD_SPDIF_OUT, 0x03))
#define SPDIF_CMD_SET_LIGHT                 		ADEC_CMD(ADEC_CMD_MOD_ID(CMD_SPDIF_OUT, 0x04), SpdifCmdSetLight)
#define SPDIF_CMD_ONOFF_SOUNDBAR            		ADEC_CMD(ADEC_CMD_MOD_ID(CMD_SPDIF_OUT, 0x05), SpdifCmdOnoffSoundbar)
#define SPDIF_CMD_SET_CATEGORY_CODE					ADEC_CMD(ADEC_CMD_MOD_ID(CMD_SPDIF_OUT, 0x06), SpdifCmdSetCategoryCode)

#define SPDIF_CMD_SET_EQBYPASS_FOR_SOUNDBAR			ADEC_CMD(ADEC_CMD_MOD_ID(CMD_SPDIF_OUT, 0x07), SpdifCmdSetEqbypassForSoundbar)
#define SPDIF_CMD_SET_WOOFERLEVEL_FOR_SOUNDBAR      ADEC_CMD(ADEC_CMD_MOD_ID(CMD_SPDIF_OUT, 0x08), SpdifCmdSetWooferlevelForSoundbar)
#define SPDIF_CMD_SET_WIRELESSRESET_FOR_SOUNDBAR    ADEC_CMD(ADEC_CMD_MOD_ID(CMD_SPDIF_OUT, 0x09), SpdifCmdSetWirelessresetForSoundbar)

#define SPDIF_CMD_GET_MASTERSYSTEMDELAY				ADEC_CMD(ADEC_CMD_MOD_ID(CMD_SPDIF_OUT, 0x0A), SpdifCmdGetMasterSD)
#define SPDIF_CMD_SET_HDMIARC						ADEC_CMD(ADEC_CMD_MOD_ID(CMD_SPDIF_OUT, 0x0B), SpdifCmdSetHdmiarc)

#define SPDIF_CMD_SET_IEC958VALID 					ADEC_CMD(ADEC_CMD_MOD_ID(CMD_SPDIF_OUT, 0x0C), SpdifCmdSetIec958valid)

/**
* Set out mode command를 정의한 구조체.
*/
typedef struct _SpdifCmdSetOutmode
{
	// PCM  = 0,
	// ES = 1,
	unsigned int out_format;
	unsigned int Fs;					///< only ES
} SpdifCmdSetOutmode;


//typedef struct _SpdifCmdSetEsMute
//{
//	// Mute Off  = 0,
//	// Mute On   = 1,
//	unsigned int mute;
//} SpdifCmdSetEsMute;

/**
* Set format for soundbar command를 정의한 구조체.
*/
typedef struct _SpdifCmdSetFmtForSoundbar
{
    unsigned int id;
    unsigned int volume;
    unsigned int mute;
}SpdifCmdSetFmtForSoundbar;

/**
* Set scms command를 정의한 구조체.
*/
typedef struct _SpdifCmdSetScms
{
    unsigned int scms_type ; 			///< copy free : 0, copy once : 2, copy no more : 1, copy never : 3
}SpdifCmdSetScms;

/**
* Set light command를 정의한 구조체.
*/
typedef struct _SpdifCmdSetLight
{
    unsigned int onoff ;        		///< 1 is on, 0 is off
}SpdifCmdSetLight;

/**
* Set soundbar on/off command를 정의한 구조체.
*/
typedef struct _SpdifCmdOnoffSoundbar
{
    unsigned int onoff ;        		///< 1 is on, 0 is off
}SpdifCmdOnoffSoundbar;

/**
* Set category code command를 정의한 구조체.
*/
typedef struct _SpdifCmdSetCategoryCode
{
	unsigned int category_code;  		///< 7bit : US : 0x64, EU : 0x0c JP : 0x04
}SpdifCmdSetCategoryCode;

/**
* Set auto-volume for soundbar command를 정의한 구조체.
*/
typedef struct _SpdifCmdSetEqbypassForSoundbar
{
	unsigned int onoff;								///< 1bit
}SpdifCmdSetEqbypassForSoundbar;

/**
* Set woofer level for soundbar command를 정의한 구조체.
*/
typedef struct _SpdifCmdSetWooferlevelForSoundbar
{
	unsigned int level;								///< 4bit
}SpdifCmdSetWooferlevelForSoundbar;

/**
* Set wireless reset for soundbar command를 정의한 구조체.
*/
typedef struct _SpdifCmdSetWirelessresetForSoundbar
{
	unsigned int reset;								///< 1bit
} SpdifCmdSetWirelessresetForSoundbar;

/**
* Set master SD for soundbar command를 정의한 구조체.
*/
typedef struct _SpdifCmdGetMasterSD
{
    unsigned int module_id;      					///< The module ID
}SpdifCmdGetMasterSD;

/**
* hdmi arc 설정을 정의한 구조체.
*/
typedef struct _SpdifCmdSetHdmiarc
{
	unsigned int hdmi_port;				///< hdmi port number maybe 0~3
	unsigned int sel_bypass;			///< 1 is hdmi bypass, 0 is dsp out
	unsigned int enable_arc;			///< 1 is arc enable , 0 is disable
}SpdifCmdSetHdmiarc;


/**
* hdmi arc 설정을 정의한 구조체.
*/
typedef struct _SpdifCmdSetIec958valid
{
	unsigned int value;			///< 0 is pcm(DTS CD), 1 is es
}SpdifCmdSetIec958valid;







#endif // #ifndef __MODULE_CMD_SPDIF_H__

