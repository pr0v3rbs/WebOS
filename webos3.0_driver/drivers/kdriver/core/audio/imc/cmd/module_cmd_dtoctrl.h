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


#ifndef __MODULE_CMD_DTORATECTRL_H__
#define __MODULE_CMD_DTORATECTRL_H__

#include "imc/adec_imc_cmd.h"


#define DTOCTRL_CMD_SET_FS          ADEC_CMD(ADEC_CMD_MOD_ID(CMD_DTO_CTRL, 0x00), DtoCtrlCmdSetFs)
#define DTOCTRL_CMD_SET_DTORATE     ADEC_CMD(ADEC_CMD_MOD_ID(CMD_DTO_CTRL, 0x01), DtoCtrlCmdSetDtorate)
#define DTOCTRL_CMD_SET_INTINFO     ADEC_CMD(ADEC_CMD_MOD_ID(CMD_DTO_CTRL, 0x02), DtoCtrlCmdSetIntInfo)
#define DTOCTRL_CMD_SET_INTFS       ADEC_CMD(ADEC_CMD_MOD_ID(CMD_DTO_CTRL, 0x03), DtoCtrlCmdSetIntFs)
#define DTOCTRL_CMD_SET_INTNUM      ADEC_CMD(ADEC_CMD_MOD_ID(CMD_DTO_CTRL, 0x04), DtoCtrlCmdSetIntNum)
#define DTOCTRL_CMD_SET_CLOCKTYPE	ADEC_CMD(ADEC_CMD_MOD_ID(CMD_DTO_CTRL, 0x05), DtoCtrlCmdSetClockType)

/**
 * Set Fs command를 정의한 구조체.
 */
typedef struct _DtoCtrlCmdSetFs
{
	unsigned int Fs;			///< only ES
}DtoCtrlCmdSetFs;

/**
 * Set DTO rate command를 정의한 구조체.
 */
typedef struct _DtoCtrlCmdSetDtorate
{
    unsigned int dtorate;
    unsigned int force_cnt_clear;
}DtoCtrlCmdSetDtorate;

/**
 * Set interrupt Fs를 정의한 구조체.
 */
typedef struct _DtoCtrlCmdSetIntFs
{
	unsigned int RefIntFs;
	unsigned int TarIntFs;
}DtoCtrlCmdSetIntFs;

/**
 * Set interrupt number를 정의한 구조체.
 */
typedef struct _DtoCtrlCmdSetIntNum
{
	unsigned int RefInt;
	unsigned int TarInt;
}DtoCtrlCmdSetIntNum;

/**
 * Set interrupt information를 정의한 구조체.
 */
typedef struct _DtoCtrlCmdSetIntInfo
{
	unsigned int RefInt;
	unsigned int RefIntFs;
	unsigned int TarInt;
	unsigned int TarIntFs;
}DtoCtrlCmdSetIntInfo;

/**
 * Set clock type를 정의한 구조체.
 */
typedef struct _DtoCtrlCmdSetClockType
{
	unsigned int clock;			///< 0 is DTO, 1 is AAD,  2 is just clock check
}DtoCtrlCmdSetClockType;

#endif // #ifndef __MODULE_CMD_DTORATECTRL_H__

