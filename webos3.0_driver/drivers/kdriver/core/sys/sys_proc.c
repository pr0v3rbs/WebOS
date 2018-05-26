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
 *  Linux proc interface for sys device.
 *	sys device will teach you how to make device driver with new platform.
 *
 *  author		ks.hyun (ks.hyun@lge.com)
 *  version		1.0
 *  date		2010.12.13
 *  note		Additional information.
 *
 *  @addtogroup lg1150_sys
 *	@{
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/
#define	SOC_SCRTEST_MAGIC_CODE		0x5343524E	/* S,C,R,N */

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include "sys_drv.h"
#include "proc_util.h"
#include "debug_util.h"
#include "base_drv.h"
#ifdef INCLUDE_KDRV_GFX
#include "gfx_drv.h"
#endif

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
	PROC_ID_MEM_STAT,
	PROC_ID_SOC_SCRTEST,

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
static OS_PROC_DESC_TABLE_T	_g_sys_device_proc_table[] =
{
	{ "author",			PROC_ID_AUTHOR  , 	OS_PROC_FLAG_READ },
	{ "command",		PROC_ID_COMMAND , 	OS_PROC_FLAG_WRITE },
	{ "mem_stat",		PROC_ID_MEM_STAT, 	OS_PROC_FLAG_READ },
    { "soc_screen_test",PROC_ID_SOC_SCRTEST,OS_PROC_FLAG_WRITE },
	{ NULL, 			PROC_ID_MAX		, 0 }
};

/*========================================================================================
	Implementation Group
========================================================================================*/
static DEFINE_SPINLOCK(soc_scrtest_spinlock);

static void	SYS_SOCScreenTest (void)
{
#define M14_SOC_SCRTEST_MAGIC_PHYSADDR		0x7f4c0000	/* reseved area between ham & sys. 0x7eac8000 ~ 0x7f500000 */
#define M14_SOC_TE_PIDF_REG_BASE			0xc0008490	/* TE pidf sram reg */
#define M14_SOC_COREAPB_REG_BASE			0xfd000000	/* coreapb reg */
#define M14_SOC_WATCHDOG_REG_BASE			(M14_SOC_COREAPB_REG_BASE+0x200000) /* WD reg */

	if(!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_M14) )
	{
		int	i;
		ULONG flags;

		UINT32 magic_vaddr   = (UINT32)ioremap(M14_SOC_SCRTEST_MAGIC_PHYSADDR, sizeof(UINT32)*4);
		UINT32 te_pidf_vaddr = (UINT32)ioremap(M14_SOC_TE_PIDF_REG_BASE, sizeof(UINT32)*2);
		UINT32 watchdog_vaddr= (UINT32)ioremap(M14_SOC_WATCHDOG_REG_BASE, 0x1000 );
		
		if (!magic_vaddr)	 { printk("can't ioremap magic addr\n"); return; }
		if (!te_pidf_vaddr)	 { printk("can't ioremap te_pidf addr\n"); return; }
		if (!watchdog_vaddr) { printk("can't ioremap watchdog reg\n"); return; }

		spin_lock_irqsave(&soc_scrtest_spinlock, flags);							/* irq disable */

		for (i=0x3c;i<0x40;i++)
		{
			__raw_writel(i,						(volatile void*)(te_pidf_vaddr+0x00));	/* write index=i*/
			__raw_writel(SOC_SCRTEST_MAGIC_CODE,(volatile void*)(te_pidf_vaddr+0x04));	/* write magic  */
		}

		for (i=0;i<4;i++)
		{
			__raw_writel(SOC_SCRTEST_MAGIC_CODE,(volatile void*)(magic_vaddr+(i*4)));	/* write magic */
		}

		__raw_writel(0x2, (volatile void*)(watchdog_vaddr+0x08));					/* WD ctrl reg */
		__raw_writel(0x0, (volatile void*)(watchdog_vaddr+0x00));					/* WD load reg */

		while(1) OS_UsecDelay(100*1000);											/* wait for reset */
	}
	else
	{
     
	}
}

/*
 * read_proc implementation of sys device
 *
*/
static int	_SYS_ReadProcFunction(	UINT32 procId, char* buffer )
{
	int		ret;

	/* TODO: add your proc_write implementation */
	switch( procId )
	{
		case PROC_ID_AUTHOR:
		{
			ret = sprintf( buffer, "%s\n", "ks.hyun (ks.hyun@lge.com)" );
		}
		break;

		case PROC_ID_MEM_STAT:
		{
#if 0	/* not ready */
			LX_MEMCFG_STAT_T cfg_stat;
			LX_GFX_MEM_STAT_T gfx_stat;
			UINT32 total, alloc;

			BASE_MEMCFG_GetStat(&cfg_stat);
			GFX_GetSurfaceMemoryStat(&gfx_stat);
			total = cfg_stat.size;
			alloc = cfg_stat.alloc;
			alloc -= gfx_stat.surface_mem_free_size;
#else
			UINT32 total, alloc;
			total = 0;
			alloc = 0;	
#endif
			ret = sprintf( buffer, "total    used\n");
			ret += sprintf( buffer + ret, "%08x %08x\n", total, alloc);
		}
		break;

		default:
		{
			ret = sprintf( buffer, "%s(%d)\n", "unimplemented read proc", procId );
		}
	}

	return ret;
}

/*
 * write_proc implementation of sys device
 *
*/
static int _SYS_WriteProcFunction( UINT32 procId, char* command )
{
	/* TODO: add your proc_write implementation */
	switch( procId )
	{
		case PROC_ID_COMMAND:
		{
			printk("command string : %s\n", command );
		}
		break;

        case PROC_ID_SOC_SCRTEST:
        {
			SYS_SOCScreenTest( );
        }
        break;

		default:
		{
			/* do nothing */
		}
		break;
	}

	return strlen(command);
}

/**
 * initialize proc utility for sys device
 *
 * @see SYS_Init
*/
void	SYS_PROC_Init (void)
{
	OS_PROC_CreateEntryEx ( SYS_MODULE, _g_sys_device_proc_table,
											_SYS_ReadProcFunction,
											_SYS_WriteProcFunction );
}

/**
 * cleanup proc utility for sys device
 *
 * @see SYS_Cleanup
*/
void	SYS_PROC_Cleanup (void)
{
	OS_PROC_RemoveEntry( SYS_MODULE );
}

/** @} */


