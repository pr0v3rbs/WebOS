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


#ifndef __MODULE_CMD_REFORM_H__
#define __MODULE_CMD_REFORM_H__

#include "imc/adec_imc_cmd.h"

#define REFORM_CMD_SET_PARAM        ADEC_CMD(ADEC_CMD_MOD_ID(CMD_REFORM, 0x00), ReformCmdSetParam)

/**
* Set parameters command를 정의한 구조체.
*/
typedef struct _ReformCmdSetParam
{
    unsigned int codec_type;            ///< Codec Type
                                        ///< ADEC_MEDIA_MS10_DDC = 2,
                                        ///< ADEC_MEDIA_MS10_DDT = 3,
                                        ///< ADEC_MEDIA_DRA      = 4,
                                        ///< ADEC_MEDIA_MP3      = 5,
} ReformCmdSetParam;


#endif // #ifndef __MODULE_CMD_REFORM_H__

