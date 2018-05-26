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

#include "adec_common.h"
#include "adec_debug_index.h"

static const char* gDebugIndexName[ADEC_DBG_INDEX_MAX] =
{
	__ADEC_FILED_INIT([ADEC_DBG_INDEX_NOT_DEF],		"_______"),
	__ADEC_FILED_INIT([ADEC_DBG_INDEX_ASSERT],		"ASSERT"),
	__ADEC_FILED_INIT([ADEC_DBG_INDEX_IPC],			"IPC"),
	__ADEC_FILED_INIT([ADEC_DBG_INDEX_IMC],			"IMC"),
	__ADEC_FILED_INIT([ADEC_DBG_INDEX_BUFFER],		"BUFFER"),
	__ADEC_FILED_INIT([ADEC_DBG_INDEX_SCHEDULER],	"SCHED"),
	__ADEC_FILED_INIT([ADEC_DBG_INDEX_PROFILER],	"PROF"),
	__ADEC_FILED_INIT([ADEC_DBG_INDEX_MANAGER],		"MAN"),
	__ADEC_FILED_INIT([ADEC_DBG_INDEX_REFORMATTER],	"REFORM"),
	__ADEC_FILED_INIT([ADEC_DBG_INDEX_DEC],			"DEC"),
	__ADEC_FILED_INIT([ADEC_DBG_INDEX_ENC],			"ENC"),
	__ADEC_FILED_INIT([ADEC_DBG_INDEX_PDEC],		"PDEC"),
	__ADEC_FILED_INIT([ADEC_DBG_INDEX_INPUT],		"INPUT"),
	__ADEC_FILED_INIT([ADEC_DBG_INDEX_MIXER],		"MIXER"),
	__ADEC_FILED_INIT([ADEC_DBG_INDEX_LIP],			"LIP"),
	__ADEC_FILED_INIT([ADEC_DBG_INDEX_SE],			"SE"),
	__ADEC_FILED_INIT([ADEC_DBG_INDEX_SRC],			"SRC"),
	__ADEC_FILED_INIT([ADEC_DBG_INDEX_INPUTCTRL],	"INPUTCTRL"),
	__ADEC_FILED_INIT([ADEC_DBG_INDEX_OUTCTRL],		"OUTCTRL"),
	__ADEC_FILED_INIT([ADEC_DBG_INDEX_ETC],			"ETC"),
	__ADEC_FILED_INIT([ADEC_DBG_INDEX_BUF_MOR],		"BUF_MOR"),
	__ADEC_FILED_INIT([ADEC_DBG_INDEX_MODULE_MOR],	"MODULE_MOR"),
	__ADEC_FILED_INIT([ADEC_DBG_INDEX_DTORATE],		"DTO_RATE"),
	__ADEC_FILED_INIT([ADEC_DBG_INDEX_SOLA],		"SOLA"),
	__ADEC_FILED_INIT([ADEC_DBG_INDEX_CTRLSRC],		"CTRLSRC"),
	__ADEC_FILED_INIT([ADEC_DBG_INDEX_NOTI],		"NOTI"),
	__ADEC_FILED_INIT([ADEC_DBG_INDEX_CMD_TIME],	"CMD_TIME"),
	//__ADEC_FILED_INIT([ADEC_DBG_INDEX_MEM],     "MEM"),
};

const char *DbgIndex_GetIndexName(ADEC_DBG_INDEX _index)
{
	if (_index >= ADEC_DBG_INDEX_MAX || _index < 0)
	{
		return "";
	}

	return gDebugIndexName[_index];
}
