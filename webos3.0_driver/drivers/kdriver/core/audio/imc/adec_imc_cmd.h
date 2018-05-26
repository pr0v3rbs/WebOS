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
 *	@file adec_imc_cmd.h
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
#ifndef __ADEC_IMC_CMD_H__
#define __ADEC_IMC_CMD_H__

#include "common/adec_config.h"

#define ADEC_CMD(_ID, _PARAM_TYPE)	((sizeof(_PARAM_TYPE) & 0xFFFF) | ((_ID&0xFFFF)<<16))
#define ADEC_CMD_SIMP(_ID)			((_ID&0xFFFF)<<16)
#define ADEC_CMD_PARAM_LEN(_CMD)	((_CMD&0xFFFF))
#define ADEC_CMD_TAG(_CMD)			((_CMD>>16) & 0xFFFF)
#define ADEC_CMD_MOD_ID(_MOD_ID, _NUM)	(((_NUM) & 0xFF) | ((_MOD_ID&0xFF)<<8))
/**
* CMD Format
*                8               16              24             32
* +--------------+---------------+---------------+--------------+
* |    CMD_TAG                   |                 PARAM_LENGTH |
* +--------------+---------------+---------------+--------------+

* PARAM_LENGTH : Length of the parameter.
* CMD_TAG : CMD type indicator.
*/

/**
 * MOD_ID Format
 *                8               16              24             32
 * +--------------+---------------+---------------+--------------+
 * |      Size of Parameters      | Num of Param  |    Module    |
 * +--------------+---------------+---------------+--------------+
 *
 * Module: Type of modules
 */

/**
 * IMC_CMD 관련 codec module의 numbering을 정의한 ENUM.
 */
typedef enum _CMD_CODEC_MODULE_ID{
	CMD_PCMDEC			= 0x20,
	CMD_DDC				= 0x21,
	CMD_DDT				= 0x22,
	CMD_DRA				= 0x23,
	CMD_MP3				= 0x24,
	CMD_WMA				= 0x25,
	CMD_DTS				= 0x26,
	CMD_DTS_M6			= 0x27,
	CMD_OGG				= 0x28,
	CMD_AMRWB			= 0x29,
	CMD_AMRNB			= 0x2A,
	CMD_RA8				= 0x2B,
	CMD_FLAC			= 0x2C,
	CMD_AACENC			= 0x2D,
	CMD_AC3ENC			= 0x2E,
	CMD_DTSENC			= 0x2F,
	CMD_CODEC_MAX		= 0x3F,
}CMD_CODEC_MODULE_ID;

/**
 * IMC_CMD 관련 codec 외 나머지 module의 numbering을 정의한 ENUM.
 */
typedef enum _CMD_GENERAL_MODULE_ID{
	CMD_SIF				= 0x40,
	CMD_ADC				= 0x41,
	CMD_HDMI			= 0x42,
	CMD_I2S				= 0x43,
	CMD_TP				= 0x44,
	CMD_REFORM			= 0x45,
	CMD_BYPASS			= 0x46,
	CMD_PCMCVT			= 0x47,
	CMD_SOLA			= 0x48,
	CMD_LIPSYNC			= 0x49,
	CMD_INPUT_CTRL		= 0x4A,
	CMD_MIXER			= 0x4B,
	CMD_SE				= 0x4C,
	CMD_OUT_CTRL		= 0x4D,
	CMD_ES_OUT_CTRL		= 0x4E,
	CMD_PCMOUT			= 0x4F,
	CMD_SPDIF_OUT		= 0x50,
	CMD_CTRL_SRC		= 0x51,
	CMD_DTO_CTRL		= 0x52,
	CMD_RTS				= 0x53,
	CMD_TIMER			= 0x54,
	CMD_DEBUG			= 0x55,
	CMD_GENERAL_MAX		= 0x9F,
}CMD_GENERAL_MODULE_ID;

#define ADEC_CMD_INIT			ADEC_CMD_SIMP(0x1000)					///< Initialize module
#define ADEC_CMD_START			ADEC_CMD_SIMP(0x1001)					///< Start module
#define ADEC_CMD_STOP			ADEC_CMD_SIMP(0x1002)					///< Stop module
#define ADEC_CMD_FLUSH			ADEC_CMD(0x1003, ImcCmdFlushParam)		///< Flush input buffer of module
#define ADEC_CMD_GET_DECINFO	ADEC_CMD_SIMP(0x1004) 					///< For DEC module only.
#define ADEC_CMD_SET_OUTMODE	ADEC_CMD(0x1005, ImcCmdSetOutputMode) 	///< For DEC module only.
#define ADEC_CMD_SET_DEBUGCNT   ADEC_CMD(0x1006, ImcCmdSetDebugCount) 	///< For DEC module only.
#define ADEC_CMD_MODULE_STATUS	ADEC_CMD_SIMP(0x1007)	 				///< Print debug status of module
#define ADEC_CMD_SET_DEBUGTIME  ADEC_CMD(0x1008, ImcCmdSetDebugTime) 	///< For Input module only.
#define ADEC_CMD_MULTI			ADEC_CMD(0x1010, ImcCmdMultiParam)
#define ADEC_CMD_RESET_PROCESSING_CNT	ADEC_CMD_SIMP(0x1011)
#define ADEC_CMD_REPORT_PROCESSING_CNT	ADEC_CMD_SIMP(0x1012)

/**
 * Flush command에 필요한 parameter를 정의한 구조체.
 */
typedef struct _ImcCmdFlushParam
{
	unsigned int	num_of_port;
	unsigned int	port_list[ADEC_CNST_IMC_MAX_PORT_COUNT];
}ImcCmdFlushParam;

/**
 * Set output mode command에 필요한 parameter를 정의한 구조체.
 */
typedef struct _ImcCmdSetOutputMode
{
	unsigned int output_mode;			///< LR : 0, LL : 1, RR : 2, L+R : 3
}ImcCmdSetOutputMode;

/**
 * Set debug count command에 필요한 parameter를 정의한 구조체.
 */
typedef struct _ImcCmdSetDebugCount
{
    unsigned int debug_cnt;     		///< 0 : Every 1 sec, N : Every N frame.
}ImcCmdSetDebugCount;

/**
 * Set debug time command에 필요한 parameter를 정의한 구조체.
 */
typedef struct _ImcCmdSetDebugTime
{
    unsigned int debug_time_interval;     		///< 0 : off, M : ms.
}ImcCmdSetDebugTime;

#endif //__ADEC_IMC_CMD_H__
/** @} */
