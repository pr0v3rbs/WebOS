/*
 * SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 * Copyright(c) 2013 by LG Electronics Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */


/** @file
 *
 *  sdec driver
 *
 *  @author	Kyungseok Hyun (ks.hyun@lge.com)
 *  @version	1.0
 *  @date		2014-04-08
 *  @note		Additional information.
 */


#ifndef _SDEC_DEBUG_H_
#define _SDEC_DEBUG_H_


/*----------------------------------------------------------------------------------------
 *   Control Constants
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/
#include "base_types.h"
#include "os_util.h"
#include "debug_util.h"

#include "sdec_cfg.h"
#include "sdec_kapi.h"

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/*----------------------------------------------------------------------------------------
 *   Constant Definitions
 *---------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
 *   Macro Definitions
 *---------------------------------------------------------------------------------------*/

#define NORMAL_COLOR           "\033[0m"
#define GRAY_COLOR             "\033[1;30m"
#define RED_COLOR              "\033[1;31m"
#define GREEN_COLOR            "\033[1;32m"
#define MAGENTA_COLOR          "\033[1;35m"
#define YELLOW_COLOR           "\033[1;33m"
#define BLUE_COLOR             "\033[1;34m"
#define DBLUE_COLOR            "\033[0;34m"
#define WHITE_COLOR            "\033[1;37m"
#define COLORERR_COLOR         "\033[1;37;41m"
#define COLORWRN_COLOR         "\033[0;31m"
#define BROWN_COLOR            "\033[0;40;33m"
#define CYAN_COLOR             "\033[0;40;36m"
#define LIGHTGRAY_COLOR        "\033[1;40;37m"
#define BRIGHTRED_COLOR        "\033[0;40;31m"
#define BRIGHTBLUE_COLOR       "\033[0;40;34m"
#define BRIGHTMAGENTA_COLOR    "\033[0;40;35m"
#define BRIGHTCYAN_COLOR       "\033[0;40;36m"

enum
{
	SDEC_MSG_ERROR		= LX_LOGM_LEVEL_ERROR,			/* 0 */
	SDEC_MSG_WARNING	= LX_LOGM_LEVEL_WARNING,		/* 1 */
	SDEC_MSG_NOTI		= LX_LOGM_LEVEL_NOTI,			/* 2 */
	SDEC_MSG_INFO		= LX_LOGM_LEVEL_INFO,			/* 3 */
	SDEC_MSG_DEBUG		= LX_LOGM_LEVEL_DEBUG,			/* 4 */
	SDEC_MSG_TRACE		= LX_LOGM_LEVEL_TRACE,			/* 5 */
	SDEC_MSG_DRV,			/* 6 */
	SDEC_MSG_NORMAL,		/* 7 */
	SDEC_MSG_READ,			/* 8 */
	SDEC_MSG_ISR,			/* 9 */
	SDEC_MSG_IO,			/* 10 */
	SDEC_MSG_PIDSEC,		/* 11 */
	SDEC_MSG_RESET,			/* 12 */
	SDEC_MSG_PCR,			/* 13 */
	SDEC_MSG_CIA,			/* 14 */
	SDEC_MSG_PES,			/* 15 */
	SDEC_MSG_SWPARSER,		/* 16 */
	SDEC_MSG_DESC,			/* 17 */
	SDEC_MSG_RFOUT,			/* 18 */
	SDEC_MSG_ISR_SEC_ERR,	/* 19 */
	SDEC_MSG_MM,			/* 20 */
	SDEC_MSG_TRACE_EX,		/* 21 */
} ;


#define SDEC_MSG_PRINT(type,fmt,args...)		DBG_PRINT( g_sdec_debug_fd, SDEC_MSG_##type, fmt, ##args)

#define SDEC_PRINT_ERROR(fmt,args...)			SDEC_MSG_PRINT(ERROR,fmt,##args)
#define SDEC_PRINT_WARNING(fmt,args...)			SDEC_MSG_PRINT(WARNING,fmt,##args)
#define SDEC_PRINT_NOTI(fmt,args...)			SDEC_MSG_PRINT(NOTI,fmt,##args)
#define SDEC_PRINT_INFO(fmt,args...)			SDEC_MSG_PRINT(INFO,fmt,##args)
#define SDEC_PRINT_TRACE(fmt,args...)			SDEC_MSG_PRINT(TRACE,fmt,##args)
#define SDEC_PRINT_TRACE_EX(fmt,args...)		SDEC_MSG_PRINT(TRACE_EX,fmt,##args)
#define SDEC_PRINT_DRV(fmt,args...)				SDEC_MSG_PRINT(DRV,fmt,##args)
#define SDEC_PRINT_NORMAL(fmt,args...)			SDEC_MSG_PRINT(NORMAL,fmt,##args)
#define SDEC_PRINT_READ(fmt,args...)			SDEC_MSG_PRINT(READ,fmt,##args)
#define SDEC_PRINT_ISR(fmt,args...)				SDEC_MSG_PRINT(ISR,fmt,##args)
#define SDEC_PRINT_IO(fmt,args...)				SDEC_MSG_PRINT(IO,fmt,##args)
#define SDEC_PRINT_PIDSEC(fmt,args...)			SDEC_MSG_PRINT(PIDSEC,fmt,##args)
#define SDEC_PRINT_RESET(fmt,args...)			SDEC_MSG_PRINT(RESET,fmt,##args)
#define SDEC_PRINT_PCR(fmt,args...)				SDEC_MSG_PRINT(PCR,fmt,##args)
#define SDEC_PRINT_CIA(fmt,args...)				SDEC_MSG_PRINT(CIA,fmt,##args)
#define SDEC_PRINT_PES(fmt,args...)				SDEC_MSG_PRINT(PES,fmt,##args)
#define SDEC_PRINT_SWPARSER(fmt,args...)		SDEC_MSG_PRINT(SWPARSER,fmt,##args)
#define SDEC_PRINT_DESC(fmt,args...)			SDEC_MSG_PRINT(DESC,fmt,##args)
#define SDEC_PRINT_RFOUT(fmt,args...)			SDEC_MSG_PRINT(RFOUT,fmt,##args)
#define SDEC_PRINT_ISR_SEC_ERR(fmt,args...)		SDEC_MSG_PRINT(ISR_SEC_ERR,fmt,##args)
#define SDEC_PRINT_MM(fmt,args...)				SDEC_MSG_PRINT(MM,fmt,##args)


#define SDEC_CHECK_ERROR(__checker,__if_action,fmt,args...)   \
			 __CHECK_IF_ERROR(__checker, SDEC_PRINT_ERROR, __if_action , fmt, ##args )

#define LX_SDEC_CHECK_CODE(__checker,__if_action,fmt,args...)   \
			__CHECK_IF_ERROR(__checker, SDEC_PRINT_ERROR, __if_action , fmt, ##args )

/*----------------------------------------------------------------------------------------
 *   Type Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   External Function Prototype Declarations
 *---------------------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------------------
 *   External Variables
 *---------------------------------------------------------------------------------------*/
extern	int		g_sdec_debug_fd;


#ifdef __cplusplus
}
#endif

#endif

