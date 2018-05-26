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


#ifndef _SDEC_IMPL_H_
#define _SDEC_IMPL_H_


/*----------------------------------------------------------------------------------------
 *   Control Constants
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <linux/kthread.h>
#include <asm/io.h>

#include "base_types.h"
#include "os_util.h"
#include "debug_util.h"

#include "sdec_cfg.h"
#include "sdec_kapi.h"
#include "sdec_debug.h"

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/*----------------------------------------------------------------------------------------
 *   Constant Definitions
 *---------------------------------------------------------------------------------------*/
#define SDEC_IO_CH_NUM		8

#define SDEC_IO_FLT_NUM		64
#define SDEC_CORE_CH_NUM	4

#define _SDEC_CORE_0		0
#define _SDEC_CORE_1		1

#define LX_SDEC_MAX_PESINFO_DATA		16


/*----------------------------------------------------------------------------------------
 *   Macro Definitions
 *---------------------------------------------------------------------------------------*/
#ifndef MIN
#define MIN( a, b )  ( (a) < (b) ? (a) : (b) )
#endif
#ifndef MAX
#define MAX( a, b )  ( (a) > (b) ? (a) : (b) )
#endif
#ifndef ABS
#define ABS( a )     ( (a) < 0 ? -(a) : (a) )
#endif
#define LX_IS_ERR(e)  ((e) != (0) ? (1) : (0))


//for bit perator
//one bit clear
#define clear__bit(data, loc)			((data) &= ~(0x1<<(loc)))
//serious many bit clear
#define clear_bits(data, area, loc)		((data) &= ~((area)<<(loc)))
//one bit set
#define set__bit(data, loc)				((data) |= (0x1<<(loc)))
//series many bit set
#define set_bits(data, area, loc)		((data) |= ((area)<<(loc)))
//one bit inversion
#define invert_bit(data, loc)			((data) ^= (0x1<<(loc)))
//series many bit inversion
#define inver_bits(data, area, loc)		((data) ^= ((area)<<(loc)))
//bit inspection
#define check_bit(data, loc)			((data) & (0x1<<(loc)))
#define check_one_bit(data, loc)		((0x1) & (data >> (loc)))
//bit extraction
#define extract_bits(data, area, loc)	((data) >> (loc)) & (area)


/* Register Access Macros */
#define SDEC_WRITE8(addr, val)		__raw_writeb(val , (void*)(addr))
#define SDEC_READ8(addr)			__raw_readb((void*)(addr))

#define SDEC_WRITE16(addr, val)		__raw_writew(val , (void*)(addr))
#define SDEC_READ16(addr)			__raw_readw((void*)(addr))

#define SDEC_WRITE32(addr, val)		__raw_writel(val , (void*)(addr))
#define SDEC_READ32(addr)			__raw_readl((void*)(addr))




#define SDEC_CONVERT_CORE_CH(core, ch)  do{ \
											if (lx_chip_rev() >= LX_CHIP_REV(M14, B0))	\
											{											\
												if(ch >= SDEC_CORE_CH_NUM)				\
												{										\
													core = 1;							\
													ch -= SDEC_CORE_CH_NUM;			\
												}										\
											}											\
										}while(0)


#define SDEC_LXCH_TO_CORE_CH(lxch, core, ch)	\
	do {										\
		core	= lxch >> 2; 					\
		ch		= lxch & 0x3;					\
	} while(0)



#define SDEC_CHECK_CHANNEL(ch, action)				\
	do {											\
		if(!SDEC_CFG_IsValidChannel(ch))			\
		{											\
			SDEC_PRINT_ERROR("invalid ch[%d]",ch);	\
			action; 								\
		}											\
	} while(0)

#define SDEC_CHECK_NORMAL_CHANNEL(ch, action)		\
	do {											\
		if(!SDEC_CFG_IsNormalChannel(ch))			\
		{											\
			SDEC_PRINT_ERROR("invalid ch[%d]",ch);	\
			action; 								\
		}											\
	} while(0)


/*----------------------------------------------------------------------------------------
 *   Type Definitions
 *---------------------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------------------
 *   External Function Prototype Declarations
 *---------------------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------------------
 *   External Variables
 *---------------------------------------------------------------------------------------*/



#ifdef __cplusplus
}
#endif

#endif

