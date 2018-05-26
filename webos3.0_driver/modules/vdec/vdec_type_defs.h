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
 *	common header file for lg1152 driver
 *
 *  @author		daeyoung lim (raxis.lim@lge.com)
 *  @version	1.0
 *  @date		2009.11.04
 *
 *  @addtogroup lg1150_base
 */

#ifndef	_VDEC_TYPE_DEFS_H_
#define	_VDEC_TYPE_DEFS_H_

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


// twkim : temp... todo mark
#define TODO_FUNC(x)
#define TODO_PTR(x)	NULL
#define TODO_SINT(x)	0
#define TODO_UINT(x)	0
#define TODO_HARDCODING(x)		(x)

/*----------------------------------------------------------------------------------------
	Base Type Definitions
----------------------------------------------------------------------------------------*/
#ifndef UINT8
typedef	unsigned char			__UINT8;
#define UINT8 __UINT8
#endif

#ifndef UINT08
typedef	unsigned char			__UINT08;
#define UINT08 __UINT08
#endif

#ifndef SINT8
typedef	signed char				__SINT8;
#define SINT8 __SINT8
#endif

#ifndef SINT08
typedef	signed char				__SINT08;
#define SINT08 __SINT08
#endif

#ifndef CHAR
typedef	char					__CHAR;
#define CHAR __CHAR
#endif

#ifndef UINT16
typedef	unsigned short			__UINT16;
#define UINT16 __UINT16
#endif

#ifndef SINT16
typedef	signed short			__SINT16;
#define SINT16 __SINT16
#endif

#ifndef UINT32
typedef	unsigned int			__UINT32;
#define UINT32 __UINT32
#endif

#ifndef SINT32
typedef signed int				__SINT32;
#define SINT32 __SINT32
#endif

#ifndef BOOLEAN
#ifndef _EMUL_WIN
typedef	unsigned int			__BOOLEAN;
#define BOOLEAN __BOOLEAN
#else
typedef	unsigned char			__BOOLEAN;
#define BOOLEAN __BOOLEAN
#endif
#endif

#ifndef ULONG
typedef unsigned long			__ULONG;
#define ULONG __ULONG
#endif

#ifndef SLONG
typedef signed long				__SLONG;
#define SLONG __SLONG
#endif

#ifndef UINT64
#ifndef _EMUL_WIN
typedef	unsigned long long		__UINT64;
#else
typedef	unsigned _int64			__UINT64;
#endif
#define UINT64 __UINT64
#endif

#ifndef SINT64
#ifndef _EMUL_WIN
typedef	signed long long		__SINT64;
#else
typedef	signed _int64			__SINT64;
#endif
#define SINT64 __SINT64
#endif

#ifndef TRUE
#define TRUE					(1)
#endif

#ifndef FALSE
#define FALSE					(0)
#endif

#ifndef ON_STATE
#define ON_STATE				(1)
#endif

#ifndef OFF_STATE
#define OFF_STATE				(0)
#endif

#ifndef ON
#define ON						(1)
#endif

#ifndef OFF
#define OFF						(0)
#endif

#ifndef NULL
#define NULL					((void *)0)
#endif

#ifndef _STR
#define _STR(_x)	_VAL(_x)
#define _VAL(_x)	#_x
#endif

/** simplified form of __func__ or __FUNCTION__ */
#ifndef	__F__
#define	__F__		__func__
#endif
/** simplified form of __LINE__ */
#ifndef	__L__
#define	__L__		__LINE__
#endif
/** simplified form of __FILE__ */
#ifndef	__FL__
#define	__FL__		__FILE__
#endif

/* branch-prediction macros; they may already be defined by libc.  */
#ifndef __linux__
#ifndef likely
#if __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 96)
#define likely(cond)	__builtin_expect(!!(int)(cond), 1)
#define unlikely(cond)	__builtin_expect((int)(cond), 0)
#else
#define likely(cond)	(cond)
#define unlikely(cond)	(cond)
#endif
#endif /* !likely */
#endif	// __linux__

/*----------------------------------------------------------------------------------------
    Platform Configuration

 [note] please use lx_chip_plt() to get current chip platform
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Error Code Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Function Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    Common Type Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _VDEC_TYPE_DEFS_H_ */

