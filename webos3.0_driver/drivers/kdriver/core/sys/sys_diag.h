/*
 * SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 * Copyright(c) 2013 by LG Electronics Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */


/** @file
 *
 *  driver interface header for diagnosis sysem. ( used only within kdriver )
 *
 *  @author		ks.hyun (ks.hyun@lge.com)
 *  @version	1.0
 *  @date		2010.12.13
 *
 *  @addtogroup lg1150_sys
 *	@{
 */

#ifndef __SYS_DIAGNOSYS_H__
#define __SYS_DIAGNOSYS_H__


/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include "debug_util.h"
#include "sys_kapi.h"

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

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/
struct diag_checker;

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/
extern int diag_init(void);

extern struct diag_checker* diag_register(int module, int type, int (*func)(int type, void *arg), void *arg);

extern struct diag_checker* diag_register_range(int module, void *var, int min, int max);
extern int diag_specify_range(struct diag_checker *checker, int min, int max);

extern struct diag_checker* diag_register_irq(int module, int min_freq, int max_freq, int max_latency);
extern int diag_irq_enter(struct diag_checker* checker);
extern int diag_irq_exit(struct diag_checker* checker);

extern int diag_invoke_reset(void);
extern int diag_invoke_all(void);
extern int diag_invoke_type(int type);
extern int diag_invoke_module(int module);


extern int diag_report(int type);

extern int diag_printf(const char *fmt, ...);

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/


#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* __SYS_DIAGNOSYS_H__ */

/** @} */

