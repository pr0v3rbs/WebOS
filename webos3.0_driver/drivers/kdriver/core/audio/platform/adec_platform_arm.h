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
 *	@file adec_platform_tmpl.h
 *
 *
 *	@author		Jung, Kyung Soon (kyungsoon.jung@leg.com)
 *	@version	0.0.1
 *	@date		2012-03-19
 *	@note
 *	@see		http://www.lge.com
 *	@addtogroup ADEC
 *	@{
 */


/******************************************************************************
  Header File Guarder
 ******************************************************************************/
#ifndef __ADEC_PLATFORM_ARM_H__
#define __ADEC_PLATFORM_ARM_H__

#include <linux/slab.h>

#include <linux/workqueue.h>
#include <linux/spinlock.h>

#include "os_util.h"
#include "../audio_drv.h"

#define ADEC_CACHE_WRITEBACK(__ADDR, __LEN)
#define ADEC_CACHE_INVALIDATE(__ADDR, __LEN)

#define ADEC_SLEEP_MS(x)                OS_MsecSleep(x) // SPT 요청으로 ARM에만 Sleep 적용.

// Get Current Time in melesecond scale
#define ADEC_GET_CURRENT_CLOCK()			(jiffies * 1000 / HZ)
#if 1
#define ADEC_MUTEX_DECLARE(__MUTEX)		spinlock_t (__MUTEX)
#define ADEC_MUTEX_CREATE(__P_MUTEX)	if (gMutexInitialized == 0) {spin_lock_init((__P_MUTEX));}
#define ADEC_MUTEX_LOCK(__P_MUTEX)		spin_lock((__P_MUTEX))
#define ADEC_MUTEX_RELEASE(__P_MUTEX)	spin_unlock((__P_MUTEX))
#define ADEC_MUTEX_DESTROY(__P_MUTEX)
#else
#define ADEC_MUTEX_DECLARE(__MUTEX)		OS_SEM_T (__MUTEX)
#define ADEC_MUTEX_CREATE(__P_MUTEX)	if (gMutexInitialized == 0) {OS_InitMutex((__P_MUTEX), OS_SEM_ATTR_DEFAULT);}
#define ADEC_MUTEX_LOCK(__P_MUTEX)		OS_LockMutex((__P_MUTEX))
#define ADEC_MUTEX_RELEASE(__P_MUTEX)	OS_UnlockMutex((__P_MUTEX))
#define ADEC_MUTEX_DESTROY(__P_MUTEX)
#endif

#define __ADEC_DBG(__INDEX,__FMT,...)			AUD_KDRV_DEBUG("[%s : %d] "__FMT, __FILE__,__LINE__,##__VA_ARGS__)
#define __ADEC_DBG_ERR(__INDEX,__FMT,...)		AUD_KDRV_PRINTF("[%s : %d] "__FMT, __FILE__,__LINE__,##__VA_ARGS__)
#define __ADEC_DBG_LOW(__INDEX,__FMT,...)		AUD_KDRV_DEBUG("[%s : %d] "__FMT, __FILE__,__LINE__,##__VA_ARGS__)
#define __ADEC_DBG_WARN(__INDEX,__FMT,...)		AUD_KDRV_PRINTF("[%s : %d] "__FMT, __FILE__,__LINE__,##__VA_ARGS__)

#define __ADEC_ASSERT(__COND, __ACTION)					if((__COND)==0){AUD_KDRV_PRINTF("[%s : %d] ASSERT!!!\n",__FILE__,__LINE__);__ACTION;}
#define __ADEC_ASSERT_STR(__COND, __ACTION, __STR,...)	if((__COND)==0){AUD_KDRV_PRINTF("[%s : %d] ASSERT: " __STR,__FILE__,__LINE__,##__VA_ARGS__);__ACTION;}

#define __STD_MALLOC(__SIZE)	(kmalloc(__SIZE, GFP_KERNEL))
#define __STD_FREE				kfree
#define __STD_MEMCPY			memcpy
#define __STD_MEMSET			memset
#define __STD_MEMMOVE			memmove
#define ADEC_IOREMAP(__ADDR, __SIZE)   (ioremap((unsigned long)__ADDR, __SIZE))
#define ADEC_IOUNMAP(__ADDR)    iounmap((void*)__ADDR)

#define ADEC_INLINE				inline

#define __PRT64X				"llX"

#define __ADEC_FILED_INIT(__FIELD,__VAL)	__FIELD = __VAL

#define ADEC_SECT_IRAM0_TEXT
#define ADEC_SECT_IRAM1_TEXT
#define ADEC_SECT_IRAM0_TEXT_LOW
#define ADEC_SECT_IRAM1_TEXT_LOW
#define ADEC_SECT_DRAM_DATA
#define ADEC_SECT_DRAM_BSS
#define ADEC_SECT_DRAM_RONLY



#endif //__ADEC_PLATFORM_ARM_H__
/** @} */
