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


//#define H14_SOC_DDR_TOTAL_BW		7464000000 /*7.4 GB/sec*/
#define H14_SOC_DDR_TOTAL_BW		6400000000 /*7.4 GB/sec*/

#define H14_FPGA_DDR_TOTAL_BW		124400000  /* 9330000 * 8 * 2/3 * 1/40 */



/*
; Register Base Address
*/
#define H14_LBUS_REG_BASE 		(0xC0050000)
#define H14_GBUS_REG_BASE		(0xC0058000)
#define H14_CCI_REG_BASE		(0xC000E000)


/*
; REGISTER_MAP
*/

/*
; Monitor lbus register blk offset
*/
#define MNT_AUD_OFFSET        		(0x0000)
#define MNT_GFX_OFFSET        		(0x0400)
#define MNT_GPU_OFFSET	       		(0x0800)
#define MNT_ICOD_OFFSET       		(0x0C00)
#define MNT_TCON_OFFSET      		(0x1000)
#define MNT_TE_OFFSET        	 	(0x1400)
#define MNT_VDEC_OFFSET     		(0x1800)
#define MNT_VDO_OFFSET     			(0x1C00)
#define MNT_VENC_OFFSET     		(0x2000)



/*
; Monitor gbus register blk offset
*/
#define MNT_BVE_MCU_OFFSET			(0x0000)
#define MNT_DE_A_OFFSET				(0x0400)
#define MNT_DE_B_OFFSET				(0x0800)
#define MNT_DE_C_OFFSET				(0x0C00)
#define MNT_DE_D_OFFSET				(0x1000)
#define MNT_DE_E_CVD_OFFSET			(0x1400)
#define MNT_DE_E_MCU_OFFSET			(0x1800)
#define MNT_DPPA_OFFSET				(0x1C00)
#define MNT_DVI_OFFSET				(0x2000)
#define MNT_DVO_OFFSET        		(0x2400)
#define MNT_MC_OFFSET				(0x2800)
#define MNT_MEPA_OFFSET				(0x2C00)
#define MNT_MEPB_OFFSET				(0x3000)
#define MNT_SRE_OFFSET				(0x3400)

/*
; Monitor CPU register blk offset
*/
#define MNT_CPU_MAIN_OFFSET			(0x0200)
#define MNT_CPU_PERI_OFFSET			(0x0100)
#define MNT_GPU_CCI_OFFSET			(0x0000)


/*
; Monitor register offset
*/
#define MNT_INTR_TOP_STATUS    		0x00
#define MNT_TOOL_INFORMATION		0x04

#define MNT_INTR_CTRL       		0x10
#define MNT_TIMER_SETTING      		0x14
#define MNT_TIMER_VALUE        		0x18

#define MNT_WR_BW_VALUE        		0x20
#define MNT_LATENCY_CONTROL    		0x24
#define MNT_WR_REQ_VALUE       		0x28
#define MNT_WR_LATENCY_MAX     		0x2C
#define MNT_WR_LATENCY_SUM     		0x30
#define MNT_WR_LATENCY_BUDGET  		0x34

#define MNT_RD_BW_VALUE        		0x38
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
#ifndef H14_IRQ_LBUS
#define LBUS_INTR_NUM				66
#define H14_IRQ_LBUS				(LBUS_INTR_NUM + 32)
#endif

#ifndef H14_IRQ_GBUS
#define GBUS_INTR_NUM				67
#define H14_IRQ_GBUS				(GBUS_INTR_NUM + 32)
#endif

#ifndef H14_IRQ_CCI_MON
#define CCI_MON_INTR_NUM			129
#define H14_IRQ_CCI_MON				(CCI_MON_INTR_NUM + 32)
#endif


/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/
typedef enum
{
	LGBUS_CPU_MAIN = 0,
	LGBUS_CPU_PERI,
	LGBUS_GPU_CCI,

	LGBUS_AUD,		/* 3 */
	LGBUS_GFX,
	LGBUS_GPU,
	LGBUS_ICOD,
	LGBUS_TCON,
	LGBUS_TE,
	LGBUS_VDEC,
	LGBUS_VDO,		/* 10 */
	LGBUS_VENC,

	LGBUS_BVE_MCU,	/* 12 */
	LGBUS_DE_A,
	LGBUS_DE_B,
	LGBUS_DE_C,		/* 15 */
	LGBUS_DE_D,
	LGBUS_DE_E_CVD,
	LGBUS_DE_E_MCU,
	LGBUS_DPPA,
	LGBUS_DVI,		/* 20 */
	LGBUS_DVO,
	LGBUS_MC,
	LGBUS_MEP_A,
	LGBUS_MEP_B,
	LGBUS_SRE,

	LGBUS_MAX_BLK	/* 26 */
}LGBUS_H14_BLK_T;


typedef enum
{
	LGBUS_REG_TYPE_NOCL = 0,
	LGBUS_REG_TYPE_NOCG,
	LGBUS_REG_TYPE_CCI,
	LGBUS_REG_MAX
} LGBUS_H14_REG_TYPE_T;

typedef enum
{
	LGBUS_MEM_TYPE_0,
	LGBUS_MEM_TYPE_1,
	LGBUS_MEM_TYPE_MAX
} LGBUS_H14_MEM_TYPE_T;


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

