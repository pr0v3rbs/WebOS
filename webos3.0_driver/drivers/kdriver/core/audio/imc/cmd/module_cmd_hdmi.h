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


#ifndef __MODULE_CMD_HDMI_H__
#define __MODULE_CMD_HDMI_H__

#include "imc/adec_imc_cmd.h"

#define HDMI_CMD_SET_FMT        ADEC_CMD(ADEC_CMD_MOD_ID(CMD_HDMI, 0x00), HdmiCmdSetFmt)
#define HDMI_CMD_SET_MODE       ADEC_CMD(ADEC_CMD_MOD_ID(CMD_HDMI, 0x01), HdmiCmdSetMode)

/**
 * Set format command를 정의한 구조체.
 */
typedef struct _HdmiCmdSetFmt
{
    // SAI_PCM_FMT_I2S = 0,
    // SAI_PCM_FMT_LJ = 1,
    // SAI_PCM_FMT_RJ = 2,
    unsigned int format;                    ///<

    // SAI_PCM_RES_16BIT = 0,
    // SAI_PCM_RES_18BIT = 1,
    // SAI_PCM_RES_20BIT = 2,
    // SAI_PCM_RES_24BIT = 3,
    unsigned int resolution;                ///<

    //0 : normal
    //1 : invert
    unsigned int sck_polarity_control;      ///<

    //0 : left channel data on LRCK = '0'
    //1 : left channel data on LRCK = '1'
    unsigned int lrck_polarity_control;     ///<
} HdmiCmdSetFmt;

/**
 * Set mode command를 정의한 구조체.
 */
typedef struct _HdmiCmdSetMode
{
    // SAI_HDMI_PCM = 0,
    // SAI_HDMI_BITSTREAM = 1,
    // SAI_HDMI_IEC60958_ES = 2,
    // SAI_HDMI_DTS_CD = 3,
    unsigned int mode;                      ///<

    // HDMI Input Port : 0 ~ 3
    unsigned int port;                      ///<
} HdmiCmdSetMode;

#endif // #ifndef __MODULE_CMD_HDMI_H__

