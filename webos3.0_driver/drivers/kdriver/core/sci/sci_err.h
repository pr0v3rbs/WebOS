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
 *  error definition header for sci device. ( used only within kdriver )
 *	sci device will teach you how to make device driver with new platform.
 *
 *  @author		juhee
 *  @version	1.0
 *  @date		2011.06.01
 *
 *  @addtogroup lg1152_sci
 *	@{
 */

#ifndef	_SCI_ERR_H_
#define	_SCI_ERR_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include "debug_util.h"

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define SCI_PRINT(fmt, args...)   DBG_PRINT( g_sci_debug_fd, LX_LOGM_LEVEL_INFO, fmt, ##args)
#define SCI_TRACE(fmt, args...)   DBG_PRINT( g_sci_debug_fd, LX_LOGM_LEVEL_TRACE, fmt, ##args)
#define SCI_WARN(fmt, args...)    DBG_PRINT( g_sci_debug_fd, LX_LOGM_LEVEL_WARNING, "^y^[SCI-WRN] %s:%d -- " fmt, __F__, __L__, ##args)
#define SCI_ERROR(fmt, args...)   DBG_PRINT( g_sci_debug_fd, LX_LOGM_LEVEL_ERROR, "^y^[SCI-ERR] %s:%d -- " fmt, __F__, __L__, ##args)
#define SCI_ISR(fmt, args...)     DBG_PRINT( g_sci_debug_fd, LX_LOGM_LEVEL_TRACE+1, fmt, ##args)

#define SCI_TRACE_BEGIN()         SCI_TRACE("[SCI:%d] BEGIN -- %s:%d\n", g_sci_trace_depth++, __F__, __L__ )
#define SCI_TRACE_END()           SCI_TRACE("[SCI:%d] END    -- %s:%d\n", --g_sci_trace_depth, __F__, __L__ )
#define SCI_TRACE_MARK()          SCI_TRACE("[SCI] LOGGING  -- %s:%d\n", __F__, __L__ )

#define SCI_CHECK_CODE(__checker,__if_action,fmt,args...)   \
             __CHECK_IF_ERROR(__checker, SCI_WARN, __if_action , fmt, ##args )

#define SCI_ASSERT(__checker)   \
            __CHECK_IF_ERROR( !(__checker), SCI_ERROR, /* nop */, "ASSERT FAILED" )

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/
typedef enum SCI_RETURN
{
	/* 0 */SCI_SUCCESS			= 0,

	/* -1 */SCI_FAIL			= -1,
	/* -2 */SCI_ERROR_CONFIG	= SCI_FAIL - 1,
	/* -3 */SCI_INVAL_PARAM		= SCI_FAIL - 2,
	/* -4 */SCI_NOMEM			= SCI_FAIL - 3,
	/* -5 */SCI_NO_CARD			= SCI_FAIL - 4,
	/* -6 */SCI_FAIL_RESET		= SCI_FAIL - 5,
	/* -7 */SCI_TRANSMIT_FAIL 	= SCI_FAIL - 6,
	/* -8 */SCI_IRQ_TIME_OUT    = SCI_FAIL - 7,
	/* -9 */SCI_OVER_MAX_ATR_DATA = SCI_FAIL - 8,
//	/* -10 */SCI = SCI_FAIL - 9,
//	/* -11 */SCI = SCI_FAIL - 10,
//	/* -12 */SCI = SCI_FAIL - 11,
//	/* -13 */SCI = SCI_FAIL - 12,
//	/* -14 */SCI = SCI_FAIL - 13
}SCI_RETURN_T;

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/
extern int	g_sci_debug_fd;
extern int	g_sci_trace_depth;


#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _SCI_ERR_H_ */

/** @} */
