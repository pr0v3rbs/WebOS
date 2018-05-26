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
 *	@file adec_debug_index.h
 *
 *
 *	@author		Jung, Kyung Soon (kyungsoon.jung@leg.com)
 *	@version	0.0.1
 *	@date		2012-03-28
 *	@note
 *	@see		http://www.lge.com
 *	@addtogroup ADEC
 *	@{
 */


/******************************************************************************
  Header File Guarder
 ******************************************************************************/
#ifndef __ADEC_DEBUG_INDEX_H__
#define __ADEC_DEBUG_INDEX_H__

typedef enum ADEC_DBG_INDEX
{
	ADEC_DBG_INDEX_NOT_DEF,
	ADEC_DBG_INDEX_ASSERT,
	ADEC_DBG_INDEX_IPC,
	ADEC_DBG_INDEX_IMC,
	ADEC_DBG_INDEX_BUFFER,
	ADEC_DBG_INDEX_SCHEDULER,
	ADEC_DBG_INDEX_PROFILER,
	ADEC_DBG_INDEX_MANAGER,
	ADEC_DBG_INDEX_REFORMATTER,
	ADEC_DBG_INDEX_DEC,
	ADEC_DBG_INDEX_ENC,
	ADEC_DBG_INDEX_PDEC,
	ADEC_DBG_INDEX_INPUT,
	ADEC_DBG_INDEX_MIXER,
	ADEC_DBG_INDEX_LIP,
	ADEC_DBG_INDEX_SE,
	ADEC_DBG_INDEX_SRC,
	ADEC_DBG_INDEX_INPUTCTRL,
	ADEC_DBG_INDEX_OUTCTRL,
	ADEC_DBG_INDEX_ETC,
	ADEC_DBG_INDEX_BUF_MOR,
	ADEC_DBG_INDEX_MODULE_MOR,
	ADEC_DBG_INDEX_DTORATE,
	ADEC_DBG_INDEX_SOLA,
	ADEC_DBG_INDEX_CTRLSRC,
	ADEC_DBG_INDEX_NOTI,
	ADEC_DBG_INDEX_CMD_TIME,
	ADEC_DBG_INDEX_MAX,
	ADEC_DBG_INDEX_MEM = ADEC_DBG_INDEX_MODULE_MOR,
}ADEC_DBG_INDEX;

const char *DbgIndex_GetIndexName(ADEC_DBG_INDEX _index);

#endif //__ADEC_DEBUG_INDEX_H__
/** @} */
