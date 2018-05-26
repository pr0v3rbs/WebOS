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
 *  Brief description.
 *  Detailed description starts here.
 *
 *  author      daeyoung lim ( raxis.lim@lge.com )
 *  version	1.0
 *  date		2009.11.14
 *  note
 *  @addtogroup lg1150_base
 *	@{
 */

#ifndef	_KDRV_BASE_DRV_H_
#define	_KDRV_BASE_DRV_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/sched.h>

#include <linux/module.h>
#include <linux/moduleparam.h>

#include <linux/fs.h>
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/delay.h>
#include <linux/timer.h>
#include <linux/poll.h>
#include <linux/interrupt.h>
#include <linux/workqueue.h>
#include <linux/cdev.h>
#include <linux/spinlock.h>

#include <asm/io.h>
#include <asm/atomic.h>
#include <asm/uaccess.h>

#include "os_util.h"
#include "proc_util.h"
#include "debug_util.h"

#ifdef USE_QEMU_SYSTEM
#include "qemu_drv.h"
#endif

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
//semaphore
extern spinlock_t g_Base_AVLipsync_Lock;

#define BASE_AVLIPSYNC_LOCK()		{unsigned long __avlipsync_lock_flags; spin_lock_irqsave(&g_Base_AVLipsync_Lock, __avlipsync_lock_flags);
#define BASE_AVLIPSYNC_UNLOCK()		spin_unlock_irqrestore(&g_Base_AVLipsync_Lock, __avlipsync_lock_flags);}


/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/

#if 1//default LOGM
#define _OS_DEBUG_SetPrintMask(config_data) 		(-1)
#define _OS_DEBUG_SavePrintMask(file_name) 			
#define _OS_DEBUG_DumpPrintMask(buffer) 			(0)
#define _OS_DEBUG_LoadPrintMask(file_name) 			
#else
/* dump current print mask information to buffer */
extern	int		_OS_DEBUG_DumpPrintMask 		( char* buffer );
/* set new print mask */
extern	int		_OS_DEBUG_SetPrintMask			( char* config_data );
/* load printmask from file */
extern	int		_OS_DEBUG_LoadPrintMask			( char* config_file );
/* save printmask to file */
extern	int		_OS_DEBUG_SavePrintMask			( char* config_file );
#endif

/* MEMCFG : prints memcfg setting. */
extern void 	BASE_MEMCFG_Print				(char* buffer, LX_MEMCFG_ENTRY_T	*pMemCfgEntry, UINT32 option);
extern void 	BASE_MEMCFG_GetStat				(LX_MEMCFG_STAT_T *stat);

extern	UINT32	BASE_MEMCFG_GetOSMemSize		(void);

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/


#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _BASE_DRV_H_ */

/** @} */

