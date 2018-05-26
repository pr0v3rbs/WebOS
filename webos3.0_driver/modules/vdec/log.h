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
#ifndef LOGFUNNEL


#if !defined(__linux__)
#elif defined(CHIP_NAME_d13)
#elif defined(CHIP_NAME_d14)
#elif defined(CONFIG_XTENSA_PLATFORM_H15VDEC)
#elif defined(CONFIG_XTENSA_PLATFORM_M14VDEC)
#elif defined(CONFIG_XTENSA_PLATFORM_H15FPGAVDEC)
#else
#define LOGMAPPED
#endif


#if defined(LOGMAPPED) && !defined(LOG_PRINTK)
#include "logm.h"
#else
#include "log_kmsg.h"
#endif

#else
#include "../logfunnel/log.h"
#endif
