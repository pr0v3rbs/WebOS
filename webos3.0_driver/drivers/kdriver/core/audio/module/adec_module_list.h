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


/**
 *	@file adec_module_list.h
 *
 *
 *	@author		Jung, Kyung Soon (kyungsoon.jung@leg.com)
 *	@version	0.0.1
 *	@date		2012-03-19
 *	@note
 *	@see		http://www.lge.com
 *	@addtogroup ADEC_COMMON
 *	@{
 */


/******************************************************************************
 Header File Guarder
 ******************************************************************************/
#ifndef __ADEC_MODULE_LIST_H__
#define __ADEC_MODULE_LIST_H__

#include "common/adec_common.h"

typedef enum _ADEC_CORE_TYPE
{
	ADEC_CORE_NOT_DEF = 0,
	ADEC_CORE_ARM,
	ADEC_CORE_DSP0,
	ADEC_CORE_DSP1,
	ADEC_CORE_MAX,
}ADEC_CORE_TYPE;

typedef enum _ADEC_BUF_TYPE
{
	ADEC_BUF_NOT_DEF = 0,
	ADEC_BUF_INTRA,
	ADEC_BUF_INTER,
	ADEC_BUF_INTER_WRITER,
	ADEC_BUF_INTER_READER,
	ADEC_BUF_MULTI_SINK,
	ADEC_BUF_MULTI_CLIENT,
	ADEC_BUF_MAX,
}ADEC_BUF_TYPE;

typedef enum _ADEC_MODULE_TYPE
{
	ADEC_MOD_TYPE_NOT_DEF,
	ADEC_MOD_TYPE_SOURCE,
	ADEC_MOD_TYPE_REFOMATTER,
	ADEC_MOD_TYPE_SINK,
	ADEC_MOD_TYPE_DECODER,
	ADEC_MOD_TYPE_ENCODER,
	ADEC_MOD_TYPE_PCMCVT,
	ADEC_MOD_TYPE_MIXER,
	ADEC_MOD_TYPE_SE,
	ADEC_MOD_TYPE_LIPSYNC,
	ADEC_MOD_TYPE_INPUTCTRL,
	ADEC_MOD_TYPE_OUTCTRL,
	ADEC_MOD_TYPE_ESOUTCTRL,
	ADEC_MOD_TYPE_BYPASS,
	ADEC_MOD_TYPE_MANAGER,
	ADEC_MOD_TYPE_DBG_MOR,
	ADEC_MOD_TYPE_SOLA,
	ADEC_MOD_TYPE_CTRLSRC,
	ADEC_MOD_TYPE_DTOCTRL,
	ADEC_MOD_TYPE_PCM,
	ADEC_MOD_TYPE_SPDIF,
	ADEC_MOD_TYPE_EXT_BUF,
	ADEC_MOD_TYPE_RTS,
	ADEC_MOD_TYPE_MAX,
}ADEC_MODULE_TYPE;

typedef enum _ADEC_MODULE_ID
{
	ADEC_MODULE_NOT_DEF = 0,
#ifdef ADEC_MODULE_DEF
#undef ADEC_MODULE_DEF
#endif
#define ADEC_MODULE_DEF(__MODULE, ...) __MODULE,
#include "adec_module_def.h"
#undef ADEC_MODULE_DEF

	ADEC_MODULE_MAX
}ADEC_MODULE_ID;

typedef struct _ModuleInfo
{
	ADEC_MODULE_ID		module;
	char*				name;
	ADEC_MODULE_TYPE	type;
	ADEC_CORE_TYPE		location;
	int					num_in;
	int					num_out;
	ADEC_BUF_TYPE		out_buffer_type[ADEC_CNST_MODULE_MAX_OUT_PORT];
}ModuleInfo;

ADEC_RESULT		ModuleList_SetPlatform(ADEC_CORE_TYPE _core);
ADEC_CORE_TYPE	ModuleList_GetPlatform(void);
ADEC_CORE_TYPE	ModuleList_FindModule(ADEC_MODULE_ID _module);
ADEC_RESULT		ModuleList_GetModuleInfo(ADEC_MODULE_ID _module, ModuleInfo* _info);
int             ModuleList_GetModuleList(ADEC_MODULE_TYPE _type, ADEC_MODULE_ID *_result_array, int _array_size);
const char 	   *ModuleList_GetModuleName(ADEC_MODULE_ID _module);
const char	   *ModuleList_GetModuleTypeName(ADEC_MODULE_TYPE _module_type);


#endif //__ADEC_MODULE_LIST_H__
/** @} */
