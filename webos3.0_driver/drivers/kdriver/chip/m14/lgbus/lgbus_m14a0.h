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
 *  author		youngjun.jang (youngjun.jang@lge.com) - modifier
 *  version		1.0
 *  date		2012.07.14
 *  note		Additional information.
 *
 *  @addtogroup lg115x_lgbus
 *	@{
 */

#ifndef _LGBUS_M14_H_
#define _LGBUS_M14_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include "lgbus_kapi.h"
#include <linux/completion.h>


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
#ifndef MHZ
#define MHZ 1000000
#endif


#define M14_A0_DDR_TOTAL_BW				6400000000 /*6.4 GB/sec*/
#define M14_A0_FPGA_DDR_TOTAL_BW		160000000  /*6.4/40=160MB/sec*/


/*
; Register Base Address
*/
#define LG1311_A0_LBUS_REG_BASE 		(0xC000E000)
#define LG1311_A0_GBUS_REG_BASE			(0xC0010000)
#define LG1311_A0_LGBUS_REG_RGN_SIZE	(0x2000)

/*
; REGISTER_MAP (M14_A0)
*/

/*
; Monitor lbus register blk offset
*/
#define M14_A0_AUD_OFFSET        		(0x0100)
#define M14_A0_GPU0_OFFSET       		(0x0200)
#define M14_A0_GPU1_OFFSET       		(0x0300)
#define M14_A0_GFX_OFFSET        		(0x0400)
#define M14_A0_ICOD_OFFSET       		(0x0500)
#define M14_A0_TE_OFFSET         		(0x0600)
#define M14_A0_VDEC_OFFSET				(0x0700)
#define M14_A0_VDO_OFFSET     			(0x0800)
#define M14_A0_VENC_OFFSET				(0x0900)
#define M14_A0_DE_E_CVD_OFFSET   		(0x0E00)
#define M14_A0_DE_E_MCU_OFFSET   		(0x0F00)
#define M14_A0_CPU_MAIN_OFFSET   		(0x1C00)

/*
; Monitor gbus register blk offset
*/
#define M14_A0_DE_A_OFFSET       		(0x0A00)
#define M14_A0_DE_B_OFFSET       		(0x0B00)
#define M14_A0_DE_C_OFFSET       		(0x0C00)
#define M14_A0_DE_D_OFFSET       		(0x0D00)
#define M14_A0_DPPA_OFFSET   			(0x1000)
#define M14_A0_DVI_OFFSET        		(0x1100)
#define M14_A0_DVO_OFFSET        		(0x1200)
#define M14_A0_SRE_OFFSET				(0x1300)
#define M14_A0_MEPA_OFFSET       		(0x1400)
#define M14_A0_MEPB_OFFSET       		(0x1500)
#define M14_A0_BVE_MCU_OFFSET   		(0x1600)
#define M14_A0_MC_OFFSET         		(0x1700)
#define M14_A0_TCON_OFFSET      		(0x1A00)
#define M14_A0_CPU_SUB_OFFSET       	(0x1C00) /*cpu_sub use gbus*/

/*
; Monitor register offset
*/
#define M14_A0_INTR_TOP_STATUS    		0x00
#define M14_A0_TOOL_INFORMATION			0x04

#define M14_A0_DEFAULT_PORT				0x06

#define M14_A0_INTR_CTRL				0x10
#define M14_A0_TIMER_SETTING      		0x14
#define M14_A0_TIMER_VALUE        		0x18

#define M14_A0_WR_BW_VALUE        		0x20
#define M14_A0_WR_BW_BUDGET       		0x24
#define M14_A0_WR_REQ_VALUE       		0x28
#define M14_A0_WR_LATENCY_MAX     		0x2C
#define M14_A0_WR_LATENCY_SUM     		0x30
#define M14_A0_WR_LATENCY_BUDGET  		0x34

#define M14_A0_RD_BW_VALUE        		0x38
#define M14_A0_RD_BW_BUDGET       		0x3C
#define M14_A0_RD_REQ_VALUE       		0x40
#define M14_A0_RD_LATENCY_MAX     		0x44
#define M14_A0_RD_LATENCY_SUM     		0x48
#define M14_A0_RD_LATENCY_BUDGET  		0x4C

#define M14_A0_WR_OTHER_BUS_BW			0x50
#define M14_A0_RD_OTHER_BUS_BW			0x60

#define M14_A0_WR_BW_0_VALUE      		0x50
#define M14_A0_WR_BW_1_VALUE      		0x54
#define M14_A0_WR_BW_2_VALUE      		0x58
#define M14_A0_WR_BW_3_VALUE      		0x5C

#define M14_A0_RD_BW_0_VALUE      		0x60
#define M14_A0_RD_BW_1_VALUE      		0x64
#define M14_A0_RD_BW_2_VALUE      		0x68
#define M14_A0_RD_BW_3_VALUE     		0x6C

#define M14_A0_ADDR_SWITCH0				0x80
#define M14_A0_ADDR_SWITCH1				0x84


#define M14_A0_MEM_PROTECTION_BASE		0xB0
#define M14_A0_MEM_PROTECTION_END		0xB4

#define M14_A0_MEM_PROTECTION_STATUS0	0xB8
#define M14_A0_MEM_PROTECTION_STATUS1	0xBC


#define M14_A0_DEFAULT_PORT_FIELD		(0x01 << 2)

/*
; Monitor register offset & mask
*/
#define M14_A0_REG_PRIORITY_OFFSET		0xF0
#define M14_A0_REG_BANDWIDTH_OFFSET		0xF8

#define M14_A0_REG_PRIORITY_MASK		0x0000000F
#define M14_A0_REG_BANDWIDTH_MASK		0x000007FF

/*
; MNT_INTR_CTRL Register field value
*/
#define M14_A0_TMR_DONE_INTR_EN			(0x01 <<  4)
#define M14_A0_MEM_PROTECT_INTR_EN		(0x01 <<  5)

#define M14_A0_TMR_DONE_INTR_STATUS		(0x01 << 12)
#define M14_A0_MEM_PROTECT_INTR_STATUS	(0x01 << 13)
#define M14_A0_INTR_STATUS_MASK			(0x3F <<  8)


#define M14_A0_TMR_DONE_INTR_CLR		(0x01 << 20)
#define M14_A0_MEM_PROTECT_INTR_CLR		(0x01 << 21)


#define M14_A0_TMR_ENABLE				(0x1<<24)
#define M14_A0_TMR_AUTO_ENABLE			(0x1<<25)


/*
; Interrupt information for lgbus
*/
#ifndef IRQ_LBUS
#define LBUS_INTR_NUM					66
#define IRQ_LBUS						(LBUS_INTR_NUM + 32)
#endif

#ifndef IRQ_GBUS
#define GBUS_INTR_NUM					67
#define IRQ_GBUS						(GBUS_INTR_NUM + 32)
#endif

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/

// M14_A0

typedef enum
{
	M14_A0_CPU_MAIN    = 0,
	M14_A0_CPU_SUB     = 1,
	M14_A0_GPU0        = 2,
	M14_A0_GPU1        = 3,
	M14_A0_GFX         = 4,
	M14_A0_VDEC        = 5,
	M14_A0_VDO         = 6,
	M14_A0_AUD         = 7,
	M14_A0_ICOD        = 8,
	M14_A0_TE          = 9,
	M14_A0_VENC        = 10,
	M14_A0_TCON        = 11,
	M14_A0_DPPA        = 12,
	M14_A0_DE_A        = 13,
	M14_A0_DE_B        = 14,
	M14_A0_DE_C        = 15,
	M14_A0_DE_D        = 16,
	M14_A0_DE_E_CVD    = 17,
	M14_A0_DE_E_MCU    = 18,
	M14_A0_DVI         = 19,
	M14_A0_DVO         = 20,
	M14_A0_SRE         = 21,
	M14_A0_MEPA        = 22,
	M14_A0_MEPB        = 23,
	M14_A0_BVE_MCU     = 24,
	M14_A0_MC          = 25,
	M14_A0_MAX_BLK
} M14_A0_BLK_T;

typedef enum
{
	M14_A0_REG_PRIORITY = 0,
	M14_A0_REG_BANDWIDTH,
	M14_A0_MAX_REG
} M14_A0_REG_T;

typedef enum
{
	M14_A0_REG_TYPE_LBUS = 0,
	M14_A0_REG_TYPE_GBUS,
	M14_A0_REG_TYPE_MAX
} M14_A0_REG_TYPE_T;


/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */


#endif /*_LGBUS_M14_H_ */


/** @} */

