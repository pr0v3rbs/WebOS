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
 *  @author		raxis.lim
 *  @version	1.0
 *  @date		2012-06-13
 *  @note		Additional information.
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>           /**< printk() */
#include <linux/slab.h>             /**< kmalloc() */
#include <linux/fs.h>               /**< everything\ldots{} */
#include <linux/types.h>            /**< size_t */
#include <linux/fcntl.h>            /**< O_ACCMODE */
#include <asm/uaccess.h>
#include <linux/ioport.h>           /**< For request_region, check_region etc */
#include <asm/io.h>                 /**< For ioremap_nocache */
#include <linux/workqueue.h>        /**< For working queue */
#include <linux/interrupt.h>
#include <linux/irq.h>
#include "gpu_drv.h"
#include "os_util.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/

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
static	int	GPU_H13_InitHW	(void);

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
static GPU_HW_CTX_T		g_gpu_hw_ctx_h13 =
{
	.InitHW		= GPU_H13_InitHW,
};

/*========================================================================================
	Implementation Group
========================================================================================*/

void	GPU_H13_InitCtx	( GPU_HW_CTX_T* ctx )
{
	g_gpu_hw_ctx_h13.priv = 0x0;

	/* copy context */
	memcpy( ctx, &g_gpu_hw_ctx_h13, sizeof(GPU_HW_CTX_T));
}

static	int	GPU_H13_InitHW	(void)
{
	unsigned long addr;

    if( lx_chip_rev( ) >= LX_CHIP_REV(H13,B0) )
    {
		int clock = 297; /* MHz */

		/* tune GPU clock */		

		switch(clock)
		{
			case 320:
				OS_WrReg(0xc001b178, 0x00000000);
				OS_WrReg(0xc001b00c, 0x00000000);
				break;

			case 297:
				OS_WrReg(0xc001b178, 0x00080000);
				OS_WrReg(0xc001b00c, 0x00000000);
				break;

			case 266:
				OS_WrReg(0xc001b00c, 0x10000000);
				break;

			case 200:
				OS_WrReg(0xc001b00c, 0x20000140);
				break;
			
			case 100:
				OS_WrReg(0xc001b00c, 0x20000120);
				break;

			default:
				printk("GPU clock : wrong value\n");
				return RET_ERROR;
		}
		printk("GPU clock : %d\n", clock);
    }
	else if( lx_chip_rev( ) >= LX_CHIP_REV(H13,A0) )
	{
#if 1
			/* tune GPU clock */
			addr = (unsigned long)ioremap(0xc001b000 , 0x10);
			__raw_writel(0x10000000 , addr + 0xc);	// 266MHz
		//	__raw_writel(0x20000120 , addr + 0xc);	// 100MHz
			iounmap((void *)addr);
		
#if 1
			/* tune GPU latency */
			addr = (unsigned long)ioremap(0xc000f110 , 0x100);
			__raw_writel(0x7ff , addr + 0x0);
			__raw_writel(0x7ff , addr + 0x80);
			iounmap((void *)addr);
#endif
#endif

	}

	return RET_OK;
}
