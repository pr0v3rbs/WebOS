/*
 * SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 * Copyright(c) 2014 by LG Electronics Inc.
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
 *  main driver implementation for vision processor device.
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

/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/
#include <linux/compiler.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/interrupt.h>
#include <linux/irqreturn.h>
#include <linux/mm.h>
#include <linux/rmap.h>
#include <linux/poll.h>
#include <linux/version.h>
#include <linux/smp.h>
#include <linux/vmalloc.h>
#include <linux/file.h>
#include <linux/clk.h>
#include <linux/input.h>
#include <linux/delay.h>
#include <linux/kthread.h>
#include <linux/pm_runtime.h>
#include <linux/suspend.h>
#include <asm/page.h>
#include <asm/uaccess.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <asm/memory.h>

#ifdef KDRV_CONFIG_PM
#include <linux/platform_device.h>
#endif

#include "os_util.h"
#include "proc_util.h"
#include "base_device.h"

#include "debug_util.h"

#include "vip_drv.h"
#include "vip_core.h"
#include "vip_mem.h"
#include "vip_proc.h"

#include "vip_drv_hal.h"
#include "vip_drv_img.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/
enum {
	PROC_ID_AUTHOR	= 0,
	PROC_ID_COMMAND,
	PROC_ID_MAX,
};

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
static OS_PROC_DESC_TABLE_T g_vip_device_proc_table[] =
{
	{ "author",		PROC_ID_AUTHOR, 	OS_PROC_FLAG_READ },
	{ "command",	PROC_ID_COMMAND, 	OS_PROC_FLAG_WRITE },
	{ NULL, 		PROC_ID_MAX, 		0 }
};

static unsigned int g_vip_proc_flag = 0;

/*========================================================================================
	Implementation Group
========================================================================================*/

/*
 * read_proc implementation of vip device
 *
*/
static int	_VIP_ReadProcFunction(unsigned int procId, char* buffer)
{
	int	ret;

	/* TODO: add your proc_write implementation */
	switch (procId) {
		case PROC_ID_AUTHOR:
			ret = sprintf(buffer, "%s\n", "hyunho.kim (hyunho747.kim@lge.com)");
			break;

		default:
			ret = sprintf(buffer, "%s(%d)\n", "unimplemented read proc", procId);
			break;
	}

	return ret;
}

/*
 * write_proc implementation of vip device
 *
*/
static int _VIP_WriteProcFunction(unsigned int procId, char* command)
{
	/* TODO: add your proc_write implementation */
	switch (procId) {
		case PROC_ID_COMMAND:
			vipm_err("command string : %s\n", command);
			break;

		default:
			break;
	}

	return strlen(command);
}

/**
 * initialize proc utility for vip device
 *
 * @see VIP_Add_SubDev
*/
void VIP_PROC_Init(char* procName, int minor)
{
	int ret;

	if (g_vip_proc_flag & (1<<minor)){
		return;
	}

	ret = OS_PROC_CreateEntryEx(procName,
								g_vip_device_proc_table,
								_VIP_ReadProcFunction,
				 				_VIP_WriteProcFunction);
	if (!ret) {
		g_vip_proc_flag |= (1<<minor);
	}
}

/**
 * cleanup proc utility for vip device
 *
 * @see VIP_Del_SubDev
*/
void VIP_PROC_Cleanup(char* procName, int minor)
{
	if (g_vip_proc_flag & (1<<minor)){
		OS_PROC_RemoveEntry(procName);
		g_vip_proc_flag &= ~(1<<minor);
	}
}





