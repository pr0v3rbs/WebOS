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


#ifndef __MODULE_CMD_DDC_H__
#define __MODULE_CMD_DDC_H__

#include "imc/adec_imc_cmd.h"

#define DDC_CMD_SET_PARAM       ADEC_CMD(ADEC_CMD_MOD_ID(CMD_DDC, 0x00), DdcCmdSetParam)

#define DDC_CMD_DESTROY_LIB     ADEC_CMD_SIMP(ADEC_CMD_MOD_ID(CMD_DDC, 0x10))

/**
 * Set parameter command를 정의한 구조체.
 */
typedef struct _DdcCmdSetParam
{
    unsigned int drc_mode;             ///< DRC mode               : 0 = Line mode(default),  1 = RF mode
    unsigned int downmix_mode;         ///< Downmix mode           : 0 = Lo/Ro(default),      1 = Lt/Rt
    unsigned int bypass_mode;          ///< Bypass mode            : 0 = DD(default), 1 = DD+
    unsigned int isSubDec;             ///< Sub Decoding           : 0 = Main Dec(SPDIF ES Support), 1 = Sub Dec(SPDIF ES Not Support)
    unsigned int isADMode;             ///< AD mode                : 0 = Off(default), 1 = On (DEC0, DEC1 must set)

#ifdef IC_CERTIFICATION
    unsigned int certi_param;          ///< IC Certification parameter
    unsigned int drc_cut_scl_factor;   ///< DRC Cut scale factor   : 0 ~ 100 (default)
    unsigned int drc_boost_scl_factor; ///< DRC Boost scale factor : 0 ~ 100 (default)
    unsigned int dual_mono_mode;       ///< Dual Mono mode         : 0 : Stereo(default), 1 : Left, 2 : Right
    unsigned int karaoke_mode;         ///< Karaoke mode           : 0 : GBL_NO_VOCALS, 1 : GBL_VOCAL1, 2 : GBL_VOCAL2, 3 : GBL_BOTH_VOCALS
    unsigned int audiocoding_mode;     ///< AudioCoding mode       : 2 : 2/0 L,R  7 : 3/2 L,C,R,Ls,Rs (default)
    unsigned int substream_id;         ///< Substream ID select    : 0 : Undefined(default), 1 : ID1, 2 : ID2, 3 : ID3
#endif // #ifdef IC_CERTIFICATION
} DdcCmdSetParam;

#endif // #ifndef __MODULE_CMD_DDC_H__

