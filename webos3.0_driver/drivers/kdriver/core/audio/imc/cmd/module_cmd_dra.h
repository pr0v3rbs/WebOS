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


#ifndef __MODULE_CMD_DRA_H__
#define __MODULE_CMD_DRA_H__

#include "imc/adec_imc_cmd.h"

#define DRA_CMD_SET_PARAM       ADEC_CMD(ADEC_CMD_MOD_ID(CMD_DRA, 0x00), DraCmdSetParam)

/**
 * Set parameter command를 정의한 구조체.
 */
typedef struct _DraCmdSetParam
{
    unsigned int bit_resolution;       ///< Bit Per Sample      : 0 = 16bit, 1 = 24bit
} DraCmdSetParam;

#endif // #ifndef __MODULE_CMD_DRA_H__

