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
#ifndef _VDEC_LOGM_H
#define _VDEC_LOGM_H

#include "logm_kapi.h"

enum
{
	log_level_error		= LX_LOGM_LEVEL_ERROR,
	log_level_warning	= LX_LOGM_LEVEL_WARNING,
	log_level_noti		= LX_LOGM_LEVEL_NOTI,
	log_level_info		= LX_LOGM_LEVEL_INFO,
	log_level_debug		= LX_LOGM_LEVEL_DEBUG,
	log_level_trace		= LX_LOGM_LEVEL_TRACE,

	log_level_user1		= LX_LOGM_LEVEL_TRACE+1,
	log_level_user2		= LX_LOGM_LEVEL_TRACE+2,
	log_level_user3		= LX_LOGM_LEVEL_TRACE+3,

	log_level_max = log_level_user3,
};


#define _log_print(level, name, levelstr, format, args...)	\
	printk (level " %10s %s %20s %-4d:" format, name, levelstr, __func__, __LINE__, ##args)



#ifdef LOG_NAME
/* we have only one log module */

#ifndef LOG_LEVEL
#define LOG_LEVEL	log_level_warning
#endif

#ifndef _TO_STR
#define _TO_STR(str) #str
#ifndef TO_STR
#define TO_STR(str) _TO_STR(str)
#endif
#endif

static int __logm = -1;
static char *__logm_name = TO_STR(LOG_NAME);


#define MAKE_FNAME(t)	_vdec_set_log_level_ ##t
#define VDEC_SET_LOG_LV_FUNC_NAME(t)	MAKE_FNAME(t)

// function name will be vdec_set_log_level_(LOG_NAME)
void VDEC_SET_LOG_LV_FUNC_NAME(LOG_NAME) (int level)
{
	unsigned int mask;

	if (__logm<0)
		__logm = LOGM_ObjRegister (__logm_name);

	if (level >= 0)
		mask = (1<<level)-1;
	else
		mask = 1<<(-level-1);

	LOGM_ObjMaskSet (__logm, mask);
	return;
}


#define log_error(format, args...)	do{ \
	if (__logm<0) \
		__logm = LOGM_ObjRegister (__logm_name); \
	LOGM_PRINT (__logm, log_level_error, \
			format, ##args); }while(0)
#define log_warning(format, args...)	do{ \
	if (__logm<0) \
		__logm = LOGM_ObjRegister (__logm_name); \
	LOGM_PRINT (__logm, log_level_warning, \
			format, ##args); }while(0)
#define log_noti(format, args...)	do{ \
	if (__logm<0) \
		__logm = LOGM_ObjRegister (__logm_name); \
	LOGM_PRINT (__logm, log_level_noti, \
			format, ##args); }while(0)
#define log_info(format, args...)	do{ \
	if (__logm<0) \
		__logm = LOGM_ObjRegister (__logm_name); \
	LOGM_PRINT (__logm, log_level_info, \
			format, ##args); }while(0)
#define log_debug(format, args...)	do{ \
	if (__logm<0) \
		__logm = LOGM_ObjRegister (__logm_name); \
	LOGM_PRINT (__logm, log_level_debug, \
			format, ##args); }while(0)
#define log_trace(format, args...)	do{ \
	if (__logm<0) \
		__logm = LOGM_ObjRegister (__logm_name); \
	LOGM_PRINT (__logm, log_level_trace, \
			format, ##args); }while(0)

#define log_user1(format, args...)	do{ \
	if (__logm<0) \
		__logm = LOGM_ObjRegister (__logm_name); \
	LOGM_PRINT (__logm, log_level_user1, \
			format, ##args); }while(0)
#define log_user2(format, args...)	do{ \
	if (__logm<0) \
		__logm = LOGM_ObjRegister (__logm_name); \
	LOGM_PRINT (__logm, log_level_user2, \
			format, ##args); }while(0)
#define log_user3(format, args...)	do{ \
	if (__logm<0) \
		__logm = LOGM_ObjRegister (__logm_name); \
	LOGM_PRINT (__logm, log_level_user3, \
			format, ##args); }while(0)

#define log_enabled(l)		({ \
		unsigned int mask; \
		if (__logm<0) \
		__logm = LOGM_ObjRegister (__logm_name); \
		LOGM_ObjMaskGet (__logm, &mask); \
		mask&(1<<l); \
		})

#endif

/* we can have multiple log molule */

#define logm_define(name,level) \
	int __logm_fd_##name = -1; \
	EXPORT_SYMBOL(__logm_fd_##name); \
	char *__logm_name_##name = #name; \
	EXPORT_SYMBOL(__logm_name_##name); \
	void _vdec_set_log_level_##name (int level) \
	{ \
		unsigned int mask; \
		if (__logm_fd_##name<0) \
			__logm_fd_##name = LOGM_ObjRegister (__logm_name_##name); \
		if (level >= 0) \
			mask = (1<<level)-1; \
		else \
			mask = 1<<(-level-1); \
		LOGM_ObjMaskSet (__logm_fd_##name, mask); \
	}


#define logm_error(m,format, args...)	do{ \
	extern int __logm_fd_##m; \
	extern char *__logm_name_##m; \
	if (__logm_fd_##m<0) \
		__logm_fd_##m = LOGM_ObjRegister (__logm_name_##m); \
	LOGM_PRINT (__logm_fd_##m, log_level_error, \
			format, ##args); }while(0)
#define logm_warning(m,format, args...)	do{ \
	extern int __logm_fd_##m; \
	extern char *__logm_name_##m; \
	if (__logm_fd_##m<0) \
		__logm_fd_##m = LOGM_ObjRegister (__logm_name_##m); \
	LOGM_PRINT (__logm_fd_##m, log_level_warning, \
			format, ##args); }while(0)
#define logm_noti(m,format, args...)	do{ \
	extern int __logm_fd_##m; \
	extern char *__logm_name_##m; \
	if (__logm_fd_##m<0) \
		__logm_fd_##m = LOGM_ObjRegister (__logm_name_##m); \
	LOGM_PRINT (__logm_fd_##m, log_level_noti, \
			format, ##args); }while(0)
#define logm_info(m,format, args...)	do{ \
	extern int __logm_fd_##m; \
	extern char *__logm_name_##m; \
	if (__logm_fd_##m<0) \
		__logm_fd_##m = LOGM_ObjRegister (__logm_name_##m); \
	LOGM_PRINT (__logm_fd_##m, log_level_info, \
			format, ##args); }while(0)
#define logm_debug(m,format, args...)	do{ \
	extern int __logm_fd_##m; \
	extern char *__logm_name_##m; \
	if (__logm_fd_##m<0) \
		__logm_fd_##m = LOGM_ObjRegister (__logm_name_##m); \
	LOGM_PRINT (__logm_fd_##m, log_level_debug, \
			format, ##args); }while(0)
#define logm_trace(m,format, args...)	do{ \
	extern int __logm_fd_##m; \
	extern char *__logm_name_##m; \
	if (__logm_fd_##m<0) \
		__logm_fd_##m = LOGM_ObjRegister (__logm_name_##m); \
	LOGM_PRINT (__logm_fd_##m, log_level_trace, \
			format, ##args); }while(0)

#define logm_user(m, l, format, args...) do { \
    extern int __logm_fd_##m; \
    extern char *__logm_name_##m; \
    if (__logm_fd_##m<0) \
        __logm_fd_##m = LOGM_ObjRegister (__logm_name_##m); \
    LOGM_PRINT (__logm_fd_##m, log_level_trace+l, \
            format, ##args); }while(0)

#define logm_enabled(m,l)		({ \
		extern int __logm_fd_##m; \
		extern char *__logm_name_##m; \
		unsigned int mask; \
		if (__logm_fd_##m<0) \
		__logm_fd_##m = LOGM_ObjRegister (__logm_name_##m); \
		LOGM_ObjMaskGet (__logm_fd_##m, &mask); \
		mask&(1<<l); \
		})

#endif
