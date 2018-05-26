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

#ifndef __MODULE_CMD_BYPASS_H__
#define __MODULE_CMD_BYPASS_H__

#include "imc/adec_imc_cmd.h"

// ENUM
/**
 * BYPASS Module의 mode를 정의한 구조체.
 */
typedef enum _BYPASS_OP_MODE
{
    BYPASS_OP_BASED_BYTE,       ///< Byte 단위로 동작.
    BYPASS_OP_BASED_AU,         ///< AUI 단위로 동작.
    BYPASS_OP_BASED_ASPOSSIBLE,	///< 가능한 모든 data 
}BYPASS_OP_MODE;

/**
 * Bypass module 의 protection mode 를 정의한 구조체  
 */
typedef enum _BYPASS_OVER_PROTECT
{
    BYPASS_DISABLE_OVERFLOW_PROTECTION,       ///< disable overflow protection.
    BYPASS_ENABLE_OVERFLOW_PROTECTION,         ///< enable overflow protection.
}BYPASS_OVER_PROTECT;

// CMD 
#define BYPASS_CMD_SET_MODE         ADEC_CMD(ADEC_CMD_MOD_ID(CMD_BYPASS, 0x00), BypassCmdSetMode)
#define BYPASS_CMD_SET_TRANSBYTE    ADEC_CMD(ADEC_CMD_MOD_ID(CMD_BYPASS, 0x01), BypassCmdSetTransbyte)

/**
 * Set mode command를 정의한 구조체.
 */
typedef struct _BypassCmdSetMode
{
	// 0 - Byte Based Mode
	// 1 - AU Based Mode
	BYPASS_OP_MODE		mode;				///< Bypass mode

	// 0 - No Overflow Protection
	// 1 - Enable Overflow Protection
	BYPASS_OVER_PROTECT	over_protect;		///< Overflow protection
}BypassCmdSetMode;

/**
 * Set mode command를 정의한 구조체.
 */
typedef struct _BypassCmdSetTransbyte
{
	unsigned int		bytes;		///< bypass transfer bytes on 'byte based mode' 
}BypassCmdSetTransbyte;

#endif
