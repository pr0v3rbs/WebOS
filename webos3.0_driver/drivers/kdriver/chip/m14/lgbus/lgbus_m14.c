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
 *  main configuration file for lgbus device
 *	lgbus device will teach you how to make device driver with new platform.
 *
 *  author		hwanwook.lee (hwanwook.lee@lge.com)
 *  author		ks.hyun (ks.hyun@lge.com) - modifier
 *  version		1.0
 *  date		2012.07.14
 *  note		Additional information.
 *
 *  @addtogroup lg115x_lgbus
 *	@{
 */


/*----------------------------------------------------------------------------------------
  Control Constants
  ----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
  File Inclusions
  ----------------------------------------------------------------------------------------*/
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>	/**< printk() */
#include <linux/slab.h> 	/**< kmalloc() */
#include <linux/fs.h> 		/**< everything\ldots{} */
#include <linux/types.h> 	/**< size_t */
#include <linux/fcntl.h>	/**< O_ACCMODE */
#include <asm/uaccess.h>
#include <linux/ioport.h>	/**< For request_region, check_region etc */
#include <asm/io.h>			/**< For ioremap_nocache */

#include <linux/interrupt.h>	/** irqreturn_t */
#include <linux/irq.h>		/** platform interrupt */

#include <linux/delay.h>

#include "os_util.h"
#include "sys_regs.h"

#include "lgbus_drv.h"
#include "lgbus_hw.h"
#include "lgbus_m14a0.h"
#include "lgbus_m14b0.h"



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

/*-----------------------------------------------------------------------------
  Static Function Prototypes Declarations
  ----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
  Static Variables
  ----------------------------------------------------------------------------------------*/

LGBUS_HW_FUNC_T* LGBUS_M14_Init(void)
{
	LGBUS_HW_FUNC_T *hw_func = NULL;

	if(lx_chip_rev() >= LX_CHIP_REV(M14, B0))
		hw_func = LGBUS_M14_B0_Init();
	else
		hw_func = LGBUS_M14_A0_Init();

	return hw_func;
}

