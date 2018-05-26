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
 *	@file adec_config.h
 *
 *
 *	@author		Jung, Kyung Soon (kyungsoon.jung@leg.com)
 *	@version	0.0.1
 *	@date		2012-03-23
 *	@note
 *	@see		http://www.lge.com
 *	@addtogroup ADEC
 *	@{
 */


/******************************************************************************
  Header File Guarder
 ******************************************************************************/
#ifndef __ADEC_CONFIG_H__
#define __ADEC_CONFIG_H__

/******************************************************************************
  Configuration for System
 ******************************************************************************/
#define ADEC_CNST_OUTPUT_FREQUENCY_KHZ	48
#define ADEC_CNST_OUTPUT_FREQUENCY		(ADEC_CNST_OUTPUT_FREQUENCY_KHZ * 1000)

/******************************************************************************
  Configuration for Debug
 ******************************************************************************/
#define ADEC_OPT_DBG_ENABLE					1
#define ADEC_OPT_DBG_LOW_ENABLE				1
#define ADEC_OPT_DBG_ERROR_ENABLE			1
#define ADEC_OPT_DBG_WARNING_ENABLE			1
#define ADEC_OPT_ASSERT_ENABLE				1
#define ADEC_OPT_USE_SYSTEM_ASSERT			0
#define ADEC_OPT_USE_ALL_FW_ASSERT			0
/* F/W ASSERT 검증을 위해 임시추가. Tenslica 이외(ARM, WIN) 사용금지. */
#define ADEC_OPT_DBG_ASSERT_ENABLE			0

/******************************************************************************
  Configuration for IMC
 ******************************************************************************/
#define ADEC_CNST_IMC_MAX_RESPONSE_COUNT	50
#define ADEC_CNST_IMC_MAX_NOTI_COUNT		50
#define ADEC_CNST_IMC_MAX_ACTION_COUNT		50
#define ADEC_CNST_IMC_MAX_PORT_COUNT		10

/******************************************************************************
  Configuration for IPC
 ******************************************************************************/
#define ADEC_CNST_IPC_MAX_SIZE				0x100
#define ADEC_CNST_IPC_MAX_PARAM_SIZE		(ADEC_CNST_IPC_MAX_SIZE - sizeof(IpcHeader) - 4)
#define ADEC_CNST_IPC_MAX_PARAM_CNT			((ADEC_CNST_IPC_MAX_SIZE - sizeof(IpcHeader) - 4)>>2)
#define ADEC_CNST_IPC_CACHE_SIZE			64
#define ADEC_CNST_IPC_CACHE_MASK			(~0x3f)
#define ADEC_OPT_IPC_DBG_DEFAULT_INTERRUPT	0
#define ADEC_OPT_IPC_USE_ASSERT				1


/******************************************************************************
  Configuration for Module
 ******************************************************************************/
#define ADEC_CNST_MODULE_NAME_LENGTH		15
#define ADEC_CNST_MODULE_MAX_OUT_PORT		4


/******************************************************************************
  Configuration for Buffer
 ******************************************************************************/
// Use Repeat
// This option effects on the size of structure
// All platforms, share memory using inter buffer, use same option.
#define ADEC_OPT_BUF_USE_REPEAT			1
// Use Rewind
// This option effects on the size of structure
// All platforms, share memory using inter buffer, use same option.
#define ADEC_OPT_BUF_FILL_GUARD_DURING_WRITING	0
#define ADEC_OPT_BUF_USE_REWIND			0
// Use Debug Message
#define ADEC_OPT_BUF_USE_DEBUG         1
// Use Assert
#define ADEC_OPT_BUF_USE_ASSERT			1
// If platform is windows, force assertion to be turned on.
#ifdef _WIN32
#undef ADEC_OPT_BUF_USE_ASSERT
#define ADEC_OPT_BUF_USE_ASSERT			1
#endif
// Clear memory with specified value when init
#define ADEC_OPT_BUF_CLR_MEM_INIT		1
// Set the value that filled with at initial state
#define ADEC_CNST_BUF_CLR_MEM_VAL		0xBC
// The
#define ADEC_CNST_BUF_ALIGN_GUARD		4
// Use register for buffer status value
#define ADEC_OPT_BUF_USE_REG			1
// Use 4 registers for 1 buffer
#define ADEC_OPT_BUF_USE_4REG_PER_BUF	1
// Use total read and total write register for debugging
#define ADEC_OPT_BUF_USE_DEBUG_REG		0
// Max number of client of the multi sink buffer
#define ADEC_CNST_BUF_MAX_CLIENT		8
// Update multi sink buffer status when flush.
// If the read operation is performed during ISR, this option is not recommended.
#define ADEC_OPT_BUF_MULTI_UPDATE_FLUSH	0
// Update multi sink buffer stuatus when write.
// If the write operation is performed during ISR, this option is not recommended.
#define ADEC_OPT_BUF_MULTI_UPDATE_WRITE	1
#if (ADEC_OPT_BUF_MULTI_UPDATE_FLUSH == 1)
#undef ADEC_OPT_BUF_MULTI_UPDATE_WRITE
#define ADEC_OPT_BUF_MULTI_UPDATE_WRITE 0
#endif
#if (ADEC_OPT_BUF_MULTI_UPDATE_FLUSH == 0 && ADEC_OPT_BUF_MULTI_UPDATE_WRITE == 0)
#error At least one of the MULTI_UPDATE_FLUSH and MULT_UPDATE_WRITE should be cativated
#endif


#endif //__ADEC_CONFIG_H__
/** @} */
