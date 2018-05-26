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

/**
 * @file
 *
 *  debug implementation for vision processor device.
 *	vision processor device will teach you how to make device driver with new platform.
 *
 *  author		hyunho kim (hyunho747.kim@lge.com)
 *  version		1.0
 *  date		2014.02.06
 *  note		Additional information.
 *
 *  @addtogroup lg1210_vip
 * @{
 */

#ifndef _VIP_DEBUG_H_
#define _VIP_DEBUG_H_

//#include <linux/errno-base.h>
#include <linux/errno.h>

#include "os_util.h"
#include "base_device.h"

#include "debug_util.h"

#include "logm_kapi.h"

#include "vip_cfg.h"

extern int _vip_debug;

extern int g_vip_debug_fd;

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define VIPM_NO_MSG_FLAG		BIT(16)
#define VIPM_NO_MSG_MASK		0xFFFF

#define VIPM_DBG_SET_DRV		(1<<0)
#define VIPM_DBG_SET_CORE		(1<<1)
#define VIPM_DBG_SET_HAL		(1<<2)
#define VIPM_DBG_SET_MEM		(1<<3)
#define VIPM_DBG_SET_QUEUE		(1<<4)
#define VIPM_DBG_SET_PM			(1<<5)
#define VIPM_DBG_SET_PROC		(1<<6)
#define VIPM_DBG_SET_USB		(1<<7)

#define VIPM_DBG_SET_DRV_IMG	(1<<16)

#define VIPM_DBG_SET_IOCTL		(1<<30)

#define VIP_ERR					LX_LOGM_LEVEL_ERROR			/* 0 */
#define VIP_WARN				LX_LOGM_LEVEL_WARNING		/* 1 */
#define VIP_NOTI				LX_LOGM_LEVEL_NOTI			/* 2 */
#define VIP_INFO				LX_LOGM_LEVEL_INFO			/* 3 */
#define VIP_DEBUG				LX_LOGM_LEVEL_DEBUG			/* 4 */
#define VIP_TRACE				LX_LOGM_LEVEL_TRACE			/* 5 */
#define VIP_DBG_INFO			(LX_LOGM_LEVEL_TRACE + 1)	/* 6 */
#define VIP_DBG_INFO_2			(LX_LOGM_LEVEL_TRACE + 2)	/* 7 */

#if (__VIP_USE_LOGM_FLAG == 1)
#define _VIP_DBG_INFO_BASE		16
#define VIP_DBG_INFO_DRV		(_VIP_DBG_INFO_BASE+0)
#define VIP_DBG_INFO_CORE		(_VIP_DBG_INFO_BASE+1)
#define VIP_DBG_INFO_HAL		(_VIP_DBG_INFO_BASE+2)
#define VIP_DBG_INFO_MEM		(_VIP_DBG_INFO_BASE+3)
#define VIP_DBG_INFO_QUEUE		(_VIP_DBG_INFO_BASE+4)
#define VIP_DBG_INFO_PM			(_VIP_DBG_INFO_BASE+5)
#define VIP_DBG_INFO_PROC		(_VIP_DBG_INFO_BASE+6)
#define VIP_DBG_INFO_USB		(_VIP_DBG_INFO_BASE+7)
#define VIP_DBG_INFO_DRV_IMG	(_VIP_DBG_INFO_BASE+8)
#endif

#define	vipm_err(fmt, args...)			DBG_PRINT(g_vip_debug_fd, VIP_ERR, fmt, ##args)
#define	vipm_warn(fmt, args...)			DBG_PRINT(g_vip_debug_fd, VIP_WARN, fmt, ##args)
#define	vipm_noti(fmt, args...)			DBG_PRINT(g_vip_debug_fd, VIP_NOTI, fmt, ##args)
#define	vipm_info(fmt, args...)			DBG_PRINT(g_vip_debug_fd, VIP_INFO, fmt, ##args)
#define	vipm_trace(fmt, args...)		DBG_PRINT(g_vip_debug_fd, VIP_TRACE, fmt, ##args)

#if (__VIP_USE_LOGM_FLAG == 1)

#define vipm_debug(flag, fmt, args...)		DBG_PRINT(g_vip_debug_fd, VIP_DBG_INFO, fmt, ##args)
#define vipm_debug_2(flag, fmt, args...)	DBG_PRINT(g_vip_debug_fd, VIP_DBG_INFO_2, fmt, ##args)

#define	vipm_debug_drv(fmt, args...)		DBG_PRINT(g_vip_debug_fd, VIP_DBG_INFO_DRV, fmt, ##args)
#define	vipm_debug_core(fmt, args...)		DBG_PRINT(g_vip_debug_fd, VIP_DBG_INFO_CORE, fmt, ##args)
#define	vipm_debug_hal(fmt, args...)		DBG_PRINT(g_vip_debug_fd, VIP_DBG_INFO_HAL, fmt, ##args)
#define	vipm_debug_mem(fmt, args...)		DBG_PRINT(g_vip_debug_fd, VIP_DBG_INFO_MEM, fmt, ##args)
#define	vipm_debug_queue(fmt, args...)		DBG_PRINT(g_vip_debug_fd, VIP_DBG_INFO_QUEUE, fmt, ##args)
#define	vipm_debug_usb(fmt, args...)		DBG_PRINT(g_vip_debug_fd, VIP_DBG_INFO_USB, fmt, ##args)
#define	vipm_debug_drv_img(fmt, args...)	DBG_PRINT(g_vip_debug_fd, VIP_DBG_INFO_DRV_IMG, fmt, ##args)

#define vipm_debug_drv_enabled()			(_vip_debug & VIP_DBG_INFO_DRV)
#define vipm_debug_core_enabled()			(_vip_debug & VIP_DBG_INFO_CORE)
#define vipm_debug_hal_enabled()			(_vip_debug & VIP_DBG_INFO_HAL)
#define vipm_debug_mem_enabled()			(_vip_debug & VIP_DBG_INFO_MEM)
#define vipm_debug_queue_enabled()			(_vip_debug & VIP_DBG_INFO_QUEUE)
#define vipm_debug_usb_enabled()			(_vip_debug & VIP_DBG_INFO_USB)
#define vipm_debug_drv_img_enabled()		(_vip_debug & VIP_DBG_INFO_DRV_IMG)

#else	//__VIP_USE_LOGM_FLAG

#define _vipm_debug(flag, fmt, args...)			\
			do {								\
				if (_vip_debug & flag)			\
					DBG_PRINT(g_vip_debug_fd, VIP_DBG_INFO, fmt, ##args);	\
			} while(0)

#define _vipm_debug_2(flag, fmt, args...)		\
			do {								\
				if (_vip_debug & flag)			\
					DBG_PRINT(g_vip_debug_fd, VIP_DBG_INFO_2, fmt, ##args);	\
			} while(0)

#define vipm_debug(flag, fmt, args...)		_vipm_debug(flag, fmt, ##args)
#define vipm_debug_2(flag, fmt, args...)	_vipm_debug_2(flag, fmt, ##args)

#define vipm_debug_drv(fmt, args...)	vipm_debug(VIPM_DBG_SET_DRV, fmt, ##args)
#define vipm_debug_core(fmt, args...)	vipm_debug(VIPM_DBG_SET_CORE, fmt, ##args)
#define vipm_debug_hal(fmt, args...)	vipm_debug(VIPM_DBG_SET_HAL, fmt, ##args)
#define vipm_debug_mem(fmt, args...)	vipm_debug(VIPM_DBG_SET_MEM, fmt, ##args)
#define vipm_debug_queue(fmt, args...)	vipm_debug(VIPM_DBG_SET_QUEUE, fmt, ##args)
#define vipm_debug_usb(fmt, args...)	vipm_debug(VIPM_DBG_SET_USB, fmt, ##args)
#define vipm_debug_drv_img(fmt, args...)	vipm_debug(VIPM_DBG_SET_DRV_IMG, fmt, ##args)

#define vipm_debug_drv_enabled()		(_vip_debug & VIPM_DBG_SET_DRV)
#define vipm_debug_core_enabled()		(_vip_debug & VIPM_DBG_SET_CORE)
#define vipm_debug_hal_enabled()		(_vip_debug & VIPM_DBG_SET_HAL)
#define vipm_debug_mem_enabled()		(_vip_debug & VIPM_DBG_SET_MEM)
#define vipm_debug_queue_enabled()		(_vip_debug & VIPM_DBG_SET_QUEUE)
#define vipm_debug_usb_enabled()		(_vip_debug & VIPM_DBG_SET_USB)
#define vipm_debug_drv_img_enabled()		(_vip_debug & VIPM_DBG_SET_DRV_IMG)

#endif	//__VIP_USE_LOGM_FLAG

#ifdef __KERNEL__
#define vip_kern_debug(msg...) \
	do { \
		printk(KERN_DEBUG "vip: " msg); \
	} while (0)

#define vip_kern_info(msg...) \
	do { \
		printk(KERN_INFO "vip: " msg); \
	} while (0)
#else
#define vip_kern_debug(msg...)	do{	}while(0)
#define vip_kern_info(msg...)	do{	}while(0)
#endif

/*-----------------------------------------------------------------------------

-----------------------------------------------------------------------------*/
#define	prt_string(a)			((a)? a : "unknown")
#define prt_names(a, arr)		(((unsigned)(a)) < ARRAY_SIZE(arr)? prt_string(arr[a]) : "unknown")

#define prt_func_name(a, arr)	((arr)? prt_string(arr[a]) : "unknown");

/*
#define print_val(f,v)	printf (#v " : %" #f "\n", v)
//print_val(d, width) => printf ("width : %d\n", width);
*/

/*-----------------------------------------------------------------------------

-----------------------------------------------------------------------------*/
#if 0
#define	__vipm_debug_time(_flag, _start, _end)					\
	do {														\
		vipm_err("elapsed[%lld us (%lld ms)] (%lld ms ~ %lld ms)\n",	\
					ktime_to_us(ktime_sub(_end, _start)),		\
					ktime_to_ms(ktime_sub(_end, _start)),		\
					ktime_to_ms(_start), ktime_to_ms(_end));	\
	} while(0)
#else
#define	__vipm_debug_time(_flag, _start, _end)					\
	do {														\
		vipm_debug(_flag, "elapsed[%lld us (%lld ms)] (%lld ms ~ %lld ms)\n",	\
					ktime_to_us(ktime_sub(_end, _start)),		\
					ktime_to_ms(ktime_sub(_end, _start)),		\
					ktime_to_ms(_start), ktime_to_ms(_end));	\
	} while(0)
#endif

#define	__vipm_debug_time_noti(_start, _end)					\
	do {														\
		vipm_noti("elapsed[%lld us (%lld ms)] (%lld ms ~ %lld ms)\n",	\
				ktime_to_us(ktime_sub(_end, _start)),		\
				ktime_to_ms(ktime_sub(_end, _start)),		\
				ktime_to_ms(_start), ktime_to_ms(_end));	\
	} while(0)

#define	__vipm_debug_buffer(_flag, _buffer, _inout)		\
	do {												\
		vipm_debug(_flag,								\
				"[%s info]\n", (_inout == VIP_BUF_INPUT)? "Input" : "Output");			\
		vipm_debug(_flag,								\
				"  addr[0x%lx] size[0x%lx]\n",			\
				(_buffer)->addr, (_buffer)->size);		\
		vipm_debug(_flag,								\
				"  data[0x%lx] flags[0x%lx]\n",			\
				(_buffer)->data, (_buffer)->flags);		\
		vipm_debug(_flag,								\
				"  index[0x%08x]\n",					\
				(_buffer)->index);						\
	} while(0)

/*-----------------------------------------------------------------------------

-----------------------------------------------------------------------------*/
extern int VIP_EnableLog(unsigned int flags);

extern int VIP_Debug_Show_Queue(void *s_priv, void *data, int opt);

extern int VIP_Add_Debugfs(void *vip_priv);
extern void VIP_Remove_Debugfs(void *vip_priv);

#endif	/* _VIP_DEBUG_H_ */

