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



/** @file
 *
 *  Linux proc interface for audio device.
 *  audio device will teach you how to make device driver with new platform.
 *
 *  author	jun.kong (jun.kong@lge.com)
 *  version	2.0
 *  date	2013.05.16
 *  note	Additional information.
 *
 *  @addtogroup lg1150_gpio
 *	@{
 */

#ifndef	_GPIO_DRV_HAL_H15_A0_H_
#define	_GPIO_DRV_HAL_H15_A0_H_



/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include <linux/version.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include "os_util.h" //for alloc functions

#include "base_types.h"

#include "gpio_drv.h"
#include "sys_regs.h"	//for ACE TOP CTRL Reg. map

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,28)
#include <asm/hardware.h> // For Register base address
#endif

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */


/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/
#ifdef INCLUDE_H15_CHIP_KDRV
extern UINT32 		gpio_intr_num_h15[GPIO_IRQ_NUM_NR];
extern void _GPIO_ISR_GPIO_H15( unsigned int ui32IRQNum, unsigned int IntData );
#endif //INCLUDE_H15_CHIP_KDRV

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _GPIO_DRV_HAL_H15_A0_H_ */

/** @} */
