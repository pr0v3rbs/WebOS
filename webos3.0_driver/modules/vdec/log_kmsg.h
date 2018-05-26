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
#ifndef _LOG_OLD_H
#define _LOG_OLD_H

#ifdef	__linux__
#include <linux/kernel.h>
#include <linux/module.h>
#else
#include <stdio.h>
#define KERN_EMERG  "<0>"   /* system is unusable           */
#define KERN_ALERT  "<1>"   /* action must be taken immediately */
#define KERN_CRIT   "<2>"   /* critical conditions          */
#define KERN_ERR    "<3>"   /* error conditions         */
#define KERN_WARNING    "<4>"   /* warning conditions           */
#define KERN_NOTICE "<5>"   /* normal but significant condition */
#define KERN_INFO   "<6>"   /* informational            */
#define KERN_DEBUG  "<7>"   /* debug-level messages         */

#define printk(format, args...)			printf(format, ##args)
#endif	//__linux__

enum
{
	log_level_none = 0,
	log_level_error,
	log_level_warning,
	log_level_noti,
	log_level_info,
	log_level_debug,
	log_level_trace,

	log_level_user1,
	log_level_user2,
	log_level_user3,

	log_level_max = log_level_user3,
};


#define _log_print(level, name, levelstr, format, args...)	\
	printk (level " %10s %s %28s %-4d:" format, name, levelstr, __func__, __LINE__, ##args)



#ifdef LOG_NAME
/* we have onlf one log module */

#ifndef LOG_LEVEL
#define LOG_LEVEL	log_level_warning
#endif

#ifndef _TO_STR
#define _TO_STR(str) #str
#ifndef TO_STR
#define TO_STR(str) _TO_STR(str)
#endif
#endif


#define _TO_LOGLEVEL(n)	log_level_##n
#define TO_LOGLEVEL(n)	_TO_LOGLEVEL(n)

#define _define_log(name) \
static int _TO_LOGLEVEL(name) __attribute__((unused)) = LOG_LEVEL; \
static char *log_name __attribute__((unused)) = _TO_STR(name)

_define_log(LOG_NAME);


#define _TO_LOGNAME(n)	log_##n
#define TO_LOGNAME(n)	_TO_LOGNAME(n)
#ifdef	__linux__
module_param_named (TO_LOGNAME(LOG_NAME), TO_LOGLEVEL(LOG_NAME), int, 0640);
#endif	//__linux__


#define MAKE_FNAME(t)	_vdec_set_log_level_ ##t
#define VDEC_SET_LOG_LV_FUNC_NAME(t)	MAKE_FNAME(t)

// function name will be _vdec_set_log_level_(LOG_NAME)
void VDEC_SET_LOG_LV_FUNC_NAME(LOG_NAME) (int level)
{
	TO_LOGLEVEL(LOG_NAME) = level;
	return;
}

#define log_print(format, args...)		\
	printk (KERN_ERR format, ##args)


#define log_error(format, args...)	do{ \
	if ((TO_LOGLEVEL(LOG_NAME)>=log_level_error  ) || (TO_LOGLEVEL(LOG_NAME)==-1)) \
	_log_print (KERN_ERR,     log_name, "ERROR", format, ##args); }while(0)
#define log_warning(format, args...)	do{ \
	if ((TO_LOGLEVEL(LOG_NAME)>=log_level_warning) || (TO_LOGLEVEL(LOG_NAME)==-2)) \
	_log_print (KERN_WARNING, log_name, "WARN ", format, ##args); }while(0)
#define log_noti(format, args...)	do{ \
	if ((TO_LOGLEVEL(LOG_NAME)>=log_level_noti   ) || (TO_LOGLEVEL(LOG_NAME)==-3)) \
	_log_print (KERN_NOTICE,  log_name, "NOTI ", format, ##args); }while(0)
#define log_info(format, args...)	do{ \
	if ((TO_LOGLEVEL(LOG_NAME)>=log_level_info   ) || (TO_LOGLEVEL(LOG_NAME)==-4)) \
	_log_print (KERN_INFO,    log_name, "INFO ", format, ##args); }while(0)
#define log_debug(format, args...)	do{ \
	if ((TO_LOGLEVEL(LOG_NAME)>=log_level_debug  ) || (TO_LOGLEVEL(LOG_NAME)==-5)) \
	_log_print (KERN_DEBUG,   log_name, "DEBUG", format, ##args); }while(0)
#define log_trace(format, args...)	do{ \
	if ((TO_LOGLEVEL(LOG_NAME)>=log_level_trace  ) || (TO_LOGLEVEL(LOG_NAME)==-6)) \
	_log_print (KERN_DEBUG,   log_name, "TRACE", format, ##args); }while(0)

#define log_user1(format, args...)	do{ \
	if ((TO_LOGLEVEL(LOG_NAME)>=log_level_user1  ) || (TO_LOGLEVEL(LOG_NAME)==-7)) \
	_log_print (KERN_ERR,     log_name, "USER1", format, ##args); }while(0)
#define log_user2(format, args...)	do{ \
	if ((TO_LOGLEVEL(LOG_NAME)>=log_level_user2  ) || (TO_LOGLEVEL(LOG_NAME)==-8)) \
	_log_print (KERN_ERR,     log_name, "USER2", format, ##args); }while(0)
#define log_user3(format, args...)	do{ \
	if ((TO_LOGLEVEL(LOG_NAME)>=log_level_user3  ) || (TO_LOGLEVEL(LOG_NAME)==-9)) \
	_log_print (KERN_ERR,     log_name, "USER3", format, ##args); }while(0)

#define log_enabled(l)		((TO_LOGLEVEL(LOG_NAME)>=l) || (TO_LOGLEVEL(LOG_NAME) == -l))

#endif

/* we can have multiple log molule */

#define logm_define(name,level) \
	int log_level_##name __attribute__((unused)) = level; \
	EXPORT_SYMBOL(log_level_##name); \
	module_param_named (log_##name, log_level_##name, int, 0644); \
	void _vdec_set_log_level_##name (int _level) \
	{ log_level_##name = _level; }


#define logm_error(name,format, args...)	do{ \
	extern int log_level_##name; \
	if ((log_level_##name>=log_level_error  ) || (log_level_##name==-1)) \
	_log_print (KERN_ERR,     #name, "ERROR", format, ##args); }while(0)
#define logm_warning(name,format, args...)	do{ \
	extern int log_level_##name; \
	if ((log_level_##name>=log_level_warning) || (log_level_##name==-2)) \
	_log_print (KERN_WARNING, #name, "WARN ", format, ##args); }while(0)
#define logm_noti(name,format, args...)	do{ \
	extern int log_level_##name; \
	if ((log_level_##name>=log_level_noti   ) || (log_level_##name==-3)) \
	_log_print (KERN_NOTICE,  #name, "NOTI ", format, ##args); }while(0)
#define logm_info(name,format, args...)	do{ \
	extern int log_level_##name; \
	if ((log_level_##name>=log_level_info   ) || (log_level_##name==-4)) \
	_log_print (KERN_INFO,    #name, "INFO ", format, ##args); }while(0)
#define logm_debug(name,format, args...)	do{ \
	extern int log_level_##name; \
	if ((log_level_##name>=log_level_debug  ) || (log_level_##name==-5)) \
	_log_print (KERN_DEBUG,   #name, "DEBUG", format, ##args); }while(0)
#define logm_trace(name,format, args...)	do{ \
	extern int log_level_##name; \
	if ((log_level_##name>=log_level_trace  ) || (log_level_##name==-6)) \
	_log_print (KERN_DEBUG,   #name, "TRACE", format, ##args); }while(0)

#define logm_user(name,level,format, args...)   do{ \
    extern int log_level_##name; \
    if (log_level_##name>=log_level_user1 ) \
    _log_print (KERN_DEBUG,  #name, "USER", format, ##args); }while(0)

#define logm_enabled(name,level)	({ \
	extern int log_level_##name; \
	((log_level_##name>=level) || (log_level_##name == -level)); \
	})


#endif
