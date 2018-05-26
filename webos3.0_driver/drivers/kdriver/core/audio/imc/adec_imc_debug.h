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
 *	@file adec_imc_debug.h
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
#ifndef __ADEC_IMC_DEBUG_H__
#define __ADEC_IMC_DEBUG_H__

#include "common/adec_debug.h"
#include "common/adec_debug_index.h"

#ifndef _XTENSA
#define IMC_PLATFORM_NAME			"ARM"
#define IMC_DBG(__FMT,...)          ADEC_DBG(ADEC_DBG_INDEX_IMC,__FMT,##__VA_ARGS__)
#define IMC_DBG_LOW(__FMT,...)      ADEC_DBG_LOW(ADEC_DBG_INDEX_IMC,__FMT,##__VA_ARGS__)
#define IMC_DBG_TRACE(__FMT,...)	ADEC_DBG_TRACE(ADEC_DBG_INDEX_IMC,__FMT,##__VA_ARGS__)
#define IMC_DBG_ERR(__FMT,...)      ADEC_DBG_ERR(ADEC_DBG_INDEX_IMC,__FMT,##__VA_ARGS__)
#ifdef _WIN32
#define NOTI_DBG(__FMT,...)			ADEC_DBG_ERR(ADEC_DBG_INDEX_NOTI,__FMT,##__VA_ARGS__)
#define NOTI_DBG_LOW(__FMT,...)		ADEC_DBG_ERR(ADEC_DBG_INDEX_NOTI,__FMT,##__VA_ARGS__)
#define NOTI_DBG_TRACE(__FMT,...)	ADEC_DBG_ERR(ADEC_DBG_INDEX_NOTI,__FMT,##__VA_ARGS__)
#define NOTI_DBG_ERR(__FMT,...)		ADEC_DBG_ERR(ADEC_DBG_INDEX_NOTI,__FMT,##__VA_ARGS__)
#else
#include "audio_drv.h"
#define NOTI_DBG(__FMT,...)			AUD_KDRV_MIP_I2C(__FMT,##__VA_ARGS__)
#define NOTI_DBG_LOW(__FMT,...)		AUD_KDRV_MIP_I2C(__FMT,##__VA_ARGS__)
#define NOTI_DBG_TRACE(__FMT,...)	AUD_KDRV_MIP_I2C(__FMT,##__VA_ARGS__)
#define NOTI_DBG_ERR(__FMT,...)		AUD_KDRV_MIP_I2C(__FMT,##__VA_ARGS__)
#endif

#else
#define IMC_PLATFORM_NAME			"DSP"
#define IMC_DBG(__FMT,...)          LOG_IMC_DBG(__FMT,##__VA_ARGS__)
#define IMC_DBG_LOW(__FMT,...)      LOG_IMC_LOW(__FMT,##__VA_ARGS__)
#define IMC_DBG_TRACE(__FMT,...)	LOG_IMC_LOW(__FMT,##__VA_ARGS__)
#define IMC_DBG_ERR(__FMT,...)      LOG_IMC_ERR(__FMT,##__VA_ARGS__)
#define NOTI_DBG(__FMT,...)         LOG_NOTI_DBG(__FMT,##__VA_ARGS__)
#define NOTI_DBG_LOW(__FMT,...)     LOG_NOTI_LOW(__FMT,##__VA_ARGS__)
#define NOTI_DBG_TRACE(__FMT,...)	LOG_NOTI_LOW(__FMT,##__VA_ARGS__)
#define NOTI_DBG_ERR(__FMT,...)     LOG_NOTI_ERR(__FMT,##__VA_ARGS__)

#endif





#endif //__ADEC_IMC_DEBUG_H__
/** @} */
