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
 *  version		1.0
 *  date		2012.07.14
 *  note		Additional information.
 *
 *  @addtogroup lg115x_lgbus
 *	@{
 */

#ifndef _LGBUS_H13_H_
#define _LGBUS_H13_H_

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


#define LGBUS_DDR_TOTAL_BW			6400000000 /*6.4 GB/sec*/
#define LGBUS_FPGA_DDR_TOTAL_BW		160000000  /*6.4/40=160MB/sec*/


/*
; Register Base Address
*/
#define LG1154_LBUS_REG_BASE 		(0xC000E000)
#define LG1154_GBUS_REG_BASE		(0xC0010000)
#define LG1154_LGBUS_REG_RGN_SIZE	(0x2000)


/*
; REGISTER_MAP
*/

/*
; Monitor lbus register blk offset
*/
#define MNT_AUD_OFFSET        		(0x0100)
#define MNT_GPU0_OFFSET       		(0x0200)
#define MNT_GPU1_OFFSET       		(0x0300)
#define MNT_GFX_OFFSET        		(0x0400)
#define MNT_ICOD_OFFSET       		(0x0500)
#define MNT_TE_OFFSET         		(0x0600)
#define MNT_VDEC_OFFSET     		(0x0700)
#define MNT_VDO_OFFSET     			(0x0800)
#define MNT_VENC_OFFSET     		(0x0900)
#define MNT_DE_E_CVD_OFFSET   		(0x0E00)
#define MNT_DE_E_MCU_OFFSET   		(0x0F00)
#define MNT_CPU_MAIN_OFFSET   		(0x1C00)


/*
; Monitor gbus register blk offset
*/
#define MNT_DE_A_OFFSET       		(0x0A00)
#define MNT_DE_B_OFFSET       		(0x0B00)
#define MNT_DE_C_OFFSET       		(0x0C00)
#define MNT_DE_D_OFFSET       		(0x0D00)
#define MNT_DPPA_OFFSET   			(0x1000)
#define MNT_DVI_OFFSET        		(0x1100)
#define MNT_DVO_OFFSET        		(0x1200)
#define MNT_SRE_OFFSET				(0x1300)
#define MNT_MEPA_OFFSET       		(0x1400)
#define MNT_MEPB_OFFSET       		(0x1500)
#define MNT_BVE_MCU_OFFSET   		(0x1600)
#define MNT_MC_OFFSET         		(0x1700)
#define MNT_TCON_OFFSET      		(0x1A00)
#define MNT_CPU_SUB_OFFSET       	(0x1C00) /*cpu_sub use gbus*/

/*
; Monitor register offset
*/
#define MNT_INTR_TOP_STATUS    		0x00

#define MNT_DEFAULT_PORT            0x06

#define MNT_INTR_CTRL       		0x10
#define MNT_TIMER_SETTING      		0x14
#define MNT_TIMER_VALUE        		0x18

#define MNT_WR_BW_VALUE        		0x20
#define MNT_WR_BW_BUDGET       		0x24
#define MNT_WR_REQ_VALUE       		0x28
#define MNT_WR_LATENCY_MAX     		0x2C
#define MNT_WR_LATENCY_SUM     		0x30
#define MNT_WR_LATENCY_BUDGET  		0x34

#define MNT_RD_BW_VALUE        		0x38
#define MNT_RD_BW_BUDGET       		0x3C
#define MNT_RD_REQ_VALUE       		0x40
#define MNT_RD_LATENCY_MAX     		0x44
#define MNT_RD_LATENCY_SUM     		0x48
#define MNT_RD_LATENCY_BUDGET  		0x4C

#define MNT_WR_OTHER_BUS_BW         0x50
#define MNT_RD_OTHER_BUS_BW         0x60

#define MNT_WR_BW_0_VALUE      		0x50
#define MNT_WR_BW_1_VALUE      		0x54
#define MNT_WR_BW_2_VALUE      		0x58
#define MNT_WR_BW_3_VALUE      		0x5C

#define MNT_RD_BW_0_VALUE      		0x60
#define MNT_RD_BW_1_VALUE      		0x64
#define MNT_RD_BW_2_VALUE      		0x68
#define MNT_RD_BW_3_VALUE     		0x6C

#define MNT_ADDR_SWITCH0			0x80
#define MNT_ADDR_SWITCH1			0x84


#define MNT_MEM_PROTECTION_BASE     0xB0
#define MNT_MEM_PROTECTION_END      0xB4

#define MNT_MEM_PROTECTION_STATUS0  0xB8
#define MNT_MEM_PROTECTION_STATUS1  0xBC

#define DEFAULT_PORT_FIELD          (0x01 << 2)

/*
; MNT_INTR_CTRL Register field value
*/
#define TMR_DONE_INTR_EN			(0x01 <<  4)
#define MEM_PROTECT_INTR_EN			(0x01 <<  5)

#define TMR_DONE_INTR_STATUS		(0x01 << 12)
#define MEM_PROTECT_INTR_STATUS		(0x01 << 13)
#define MNT_INTR_STATUS_MASK		(0x3F <<  8)


#define TMR_DONE_INTR_CLR			(0x01 << 20)
#define MEM_PROTECT_INTR_CLR		(0x01 << 21)


#define TMR_ENABLE					(0x1<<24)
#define TMR_AUTO_ENABLE				(0x1<<25)


/*
; Interrupt information for lgbus
*/
#ifndef IRQ_LBUS
#define LBUS_INTR_NUM				66
#define IRQ_LBUS					(LBUS_INTR_NUM + 32)
#endif

#ifndef IRQ_GBUS
#define GBUS_INTR_NUM				67
#define IRQ_GBUS					(GBUS_INTR_NUM + 32)
#endif


/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/
typedef enum
{
	LGBUS_CPU_MAIN    = 0,
	LGBUS_CPU_SUB     = 1,
	LGBUS_GPU0        = 2,
	LGBUS_GPU1        = 3,
	LGBUS_GFX         = 4,
	LGBUS_VDEC        = 5,
	LGBUS_VDO         = 6,
	LGBUS_AUD         = 7,
	LGBUS_ICOD        = 8,
	LGBUS_TE          = 9,
	LGBUS_VENC        = 10,
	LGBUS_TCON        = 11,
	LGBUS_DPPA        = 12,
	LGBUS_DE_A        = 13,
	LGBUS_DE_B        = 14,
	LGBUS_DE_C        = 15,
	LGBUS_DE_D        = 16,
	LGBUS_DE_E_CVD    = 17,
	LGBUS_DE_E_MCU    = 18,
	LGBUS_DVI         = 19,
	LGBUS_DVO         = 20,
	LGBUS_SRE         = 21,
	LGBUS_MEP_A       = 22,
	LGBUS_MEP_B       = 23,
	LGBUS_BVE_MCU     = 24,
	LGBUS_MC          = 25,
	LGBUS_MAX_BLK
}LX_LGBUS_BLK_T;

typedef struct
{
	int			idx;
	UINT32		reg_offset;
	UINT32		reg_base;
	int			bus_idx;
	UINT32		clock;
} LGBUS_MNT_BLK_INFO_T;



/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */


#endif /*_LGBUS_H13_H_ */


/** @} */

