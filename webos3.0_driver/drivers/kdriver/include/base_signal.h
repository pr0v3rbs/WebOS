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

/** @file base_signal.h
 *
 *	BSP signal broadcaster between module (EXPERIMENTAL)
 *	current exprimental signal is very simple.
 *	sender calls the receiver's handler via lx_raise_signal.
 *	so sender will stop its execution untiil receiver's handler returns.
 *	receiver's signal handler should not waste much time to prevent the unexpected execution delay.
 *	I recommend that signal handler should be like interrupt handler.
 *
 *  @author     raxis.lim
 *  @version    1.0
 *  @date       2014-11-12
 *  @note       Additional information.
 */
#ifndef	_BASE_SIGNAL_H_
#define	_BASE_SIGNAL_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

#include "base_types.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/
void	lx_set_signal_handler	(LX_SIG_HANDLER_T sighandler);
void	lx_raise_signal			(UINT32 signum, UINT32 arg[3]);

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _BASE_SIGNAL_H_ */

