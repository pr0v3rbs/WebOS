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


#ifndef __MODULE_CMD_DDT_H__
#define __MODULE_CMD_DDT_H__

#include "imc/adec_imc_cmd.h"

#define DDT_CMD_SET_PARAM       ADEC_CMD(ADEC_CMD_MOD_ID(CMD_DDT, 0x00), DdtCmdSetParam)

#define DDT_CMD_DESTROY_LIB     ADEC_CMD_SIMP(ADEC_CMD_MOD_ID(CMD_DDT, 0x10))

 /**
  * MS10 DDT Input Type를 정의한 enum.
  */
 typedef enum {
    MS10_DDT_INPUT_NONE,              ///< 0
    MS10_DDT_INPUT_TP0,               ///< 1
    MS10_DDT_INPUT_TP1,               ///< 2
    MS10_DDT_INPUT_SIF,               ///< 3
    MS10_DDT_INPUT_ADC,               ///< 4
    MS10_DDT_INPUT_HDMI,              ///< 5
    MS10_DDT_INPUT_I2S,               ///< 6
    MS10_DDT_INPUT_SYSTEM,            ///< 7
    MS10_DDT_INPUT_MAX
} MS10_DDT_INPUT_T;

/**
 * Set parameter command를 정의한 구조체.
 */
typedef struct _DdtCmdSetParam
{
    unsigned int     drc_mode;         ///< DRC mode               : 0 = Line mode(default),  1 = RF mode
    unsigned int     downmix_mode;     ///< Downmix mode           : 0 = Lo/Ro(default),      1 = Lt/Rt
    unsigned int     isSubDec;         ///< Sub Decoding           : 0 = Main Dec(SPDIF ES Support), 1 = Sub Dec(SPDIF ES Not Support)
    unsigned int     isADMode;         ///< AD mode                : 0 = Off(default), 1 = On (DEC0, DEC1 must set)
    unsigned int     iec_fmt;          ///< IEC Format             : 0 = AC3 (default),       1 = AAC
    MS10_DDT_INPUT_T inputtype;        ///< Input type             : MS10_DDT_INPUT_T
    unsigned int     transcoding;      ///< Transcoding            : 0 = Enable(default), 1 = Disable

#ifdef IC_CERTIFICATION
    unsigned int certi_param;          ///< IC Certification parameter
    unsigned int drc_cut_scl_factor;   ///< DRC Cut scale factor   : 0 ~ 100 (default)
    unsigned int drc_boost_scl_factor; ///< DRC Boost scale factor : 0 ~ 100 (default)
    unsigned int dual_mono_mode;       ///< Dual Mono mode         : 0 : Stereo(default), 1 : Left, 2 : Right
#endif // #ifdef IC_CERTIFICATION
} DdtCmdSetParam;

#endif // #ifndef __MODULE_CMD_DDT_H__

