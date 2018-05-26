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

#ifndef __MODULE_CMD_BUF_MONITOR_H__
#define __MODULE_CMD_BUF_MONITOR_H__

#include "imc/adec_imc_cmd.h"

#define DBG_CMD_BUF_REPORT			ADEC_CMD_SIMP(ADEC_CMD_MOD_ID(CMD_DEBUG, 0x0B))
#define DBG_CMD_SYS_DELAY_REPORT	ADEC_CMD_SIMP(ADEC_CMD_MOD_ID(CMD_DEBUG, 0x0D))
#define DBG_CMD_PROF_REPORT			ADEC_CMD_SIMP(ADEC_CMD_MOD_ID(CMD_DEBUG, 0x0F))
#define DBG_CMD_ALLOC_SIZE			ADEC_CMD_SIMP(ADEC_CMD_MOD_ID(CMD_DEBUG, 0xA0))
#define DBG_CMD_ALLOC_LIST			ADEC_CMD_SIMP(ADEC_CMD_MOD_ID(CMD_DEBUG, 0xA1))
#define DBG_CMD_CHECK_MEM			ADEC_CMD_SIMP(ADEC_CMD_MOD_ID(CMD_DEBUG, 0xC0))
#define DBG_CMD_MMR_REPORT			ADEC_CMD(ADEC_CMD_MOD_ID(CMD_DEBUG, 0xC1),DbgCmdMmrReport)
#define DBG_CMD_WRITE_MMR			ADEC_CMD(ADEC_CMD_MOD_ID(CMD_DEBUG, 0xC2),DbgCmdWriteMmr)
#define DBG_CMD_RESET_ALL_PROCESS_CNT	ADEC_CMD_SIMP(ADEC_CMD_MOD_ID(CMD_DEBUG, 0xc3))
#define DBG_CMD_REPORT_ALL_PROCESS_CNT	ADEC_CMD_SIMP(ADEC_CMD_MOD_ID(CMD_DEBUG, 0xc4))

/**
 * MMR 주소 정의한 구조체.
 */
typedef struct _DbgCmdMmrReport
{
	unsigned int addr;			///< 0 ~ 0xf000   0x4fff를 제외한 하위 주소
}DbgCmdMmrReport;


/**
 * MMR에 cpu 에서 값을 적는다.
 */
typedef struct _DbgCmdWriteMmr
{
	unsigned int addr;			///< 0x0000 ~ 0xf000   0x4fffoooo 에서 0x4fff 를 제외한 하위 주소
	unsigned int value;			///< register value
}DbgCmdWriteMmr;


#endif
