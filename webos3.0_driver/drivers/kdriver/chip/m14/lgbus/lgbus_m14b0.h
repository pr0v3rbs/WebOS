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
 *  author		youngjun.jang (youngjun.jang@lge.com)
 *  version		1.0
 *  date		2013.10.14
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


#define M14_B0_DDR_TOTAL_BW				6400000000	/*6.4 GB/sec*/
#define M14_B0_FPGA_DDR_TOTAL_BW		160000000	/*6.4/40=160MB/sec*/


/*
; M14_B0 Monitor Base Address
*/
#define LG1311_B0_ATOP_REG_BASE			(0xC0010000)
#define LG1311_B0_BTOP_REG_BASE			(0xC0014000)
#define LG1311_B0_TOP_REG_RGN_SIZE		(0x4000)

/*
; REGISTER_MAP (M14_B0)
*/

/*
; M14_B0 A_TOP Read Toolkit Block Offset
*/
#define M14_B0_VDEC_OFFSET				(0x0200)
#define M14_B0_HEVC0_OFFSET				(0x0400)
#define M14_B0_HEVC1_OFFSET				(0x0600)
#define M14_B0_CPU0_OFFSET				(0x0800)
#define M14_B0_GPU0_OFFSET				(0x0A00)
#define M14_B0_USB_OFFSET				(0x0C00)
#define M14_B0_GPU1_OFFSET				(0x0E00)
#define M14_B0_CPU1_OFFSET				(0x1000)
#define M14_B0_CPU_PERI_OFFSET			(0x1200)
#define M14_B0_VDEC_MCU_OFFSET			(0x1400)
#define M14_B0_IDS_MCU_OFFSET			(0x1600)

/*
; M14_B0 B_TOP Read Toolkit Block Offset
*/
#define M14_B0_DPE_OFFSET				(0x0200)
#define M14_B0_DPP_OFFSET				(0x0400)
#define M14_B0_FDA_OFFSET				(0x0600)
#define M14_B0_DEO_OFFSET				(0x0800)
#define M14_B0_DEM_OFFSET				(0x0A00)
#define M14_B0_DEI_OFFSET				(0x0C00)
#define M14_B0_CVD_OFFSET				(0x0E00)
#define M14_B0_TE_OFFSET				(0x1000)
#define M14_B0_FDD0_OFFSET				(0x1200)
#define M14_B0_FDD1_OFFSET				(0x1400)
#define M14_B0_FDB_OFFSET				(0x1600)
#define M14_B0_FDC_OFFSET				(0x1800)
#define M14_B0_VENC_OFFSET				(0x1A00)
#define M14_B0_GFX_OFFSET				(0x1C00)
#define M14_B0_AUD_OFFSET				(0x1E00)

#define M14_B0_WR_TOOLKIT_OFFSET		(0x0100)

/*
;	M14_B0 Common Toolkit Offset
*/
#define M14_B0_INTR_ENABLE				0x20
#define M14_B0_INTR_STATUS				0x24
#define M14_B0_INTR_CLEAR				0x28
#define M14_B0_TIMER_INIT_VALUE			0x30

/*
;	M14_B0 Common Toolkit Field Value
*/
#define M14_B0_DEFAULT_TIMER_VALUE		396*MHZ

#define M14_B0_INTR_STATUS_ATOP_MASK	0xFFF
#define M14_B0_INTR_STATUS_BTOP_MASK	0x7FFF

#define M14_B0_TMR_DONE_INTR_STATUS		(0x01 << 0)

#define M14_B0_INTR_STATUS_VDEC			(0x01 << 1)
#define M14_B0_INTR_STATUS_HEVC0		(0x01 << 2)
#define M14_B0_INTR_STATUS_HEVC1		(0x01 << 3)
#define M14_B0_INTR_STATUS_CPU0			(0x01 << 4)
#define M14_B0_INTR_STATUS_GPU0			(0x01 << 5)
#define M14_B0_INTR_STATUS_USB			(0x01 << 6)
#define M14_B0_INTR_STATUS_GPU1			(0x01 << 7)
#define M14_B0_INTR_STATUS_CPU1			(0x01 << 8)
#define M14_B0_INTR_STATUS_CPU_PERI		(0x01 << 9)
#define M14_B0_INTR_STATUS_VDEC_MCU		(0x01 << 10)
#define M14_B0_INTR_STATUS_IDS_MCU		(0x01 << 11)

#define M14_B0_INTR_STATUS_DPE			(0x01 << 1)
#define M14_B0_INTR_STATUS_DPP			(0x01 << 2)
#define M14_B0_INTR_STATUS_FDA			(0x01 << 3)
#define M14_B0_INTR_STATUS_DEO			(0x01 << 4)
#define M14_B0_INTR_STATUS_DEM			(0x01 << 5)
#define M14_B0_INTR_STATUS_DEI			(0x01 << 6)
#define M14_B0_INTR_STATUS_CVD			(0x01 << 7)
#define M14_B0_INTR_STATUS_TE			(0x01 << 8)
#define M14_B0_INTR_STATUS_FDD1			(0x01 << 9)
#define M14_B0_INTR_STATUS_FDB			(0x01 << 10)
#define M14_B0_INTR_STATUS_FDC			(0x01 << 11)
#define M14_B0_INTR_STATUS_VENC			(0x01 << 12)
#define M14_B0_INTR_STATUS_GFX			(0x01 << 13)
#define M14_B0_INTR_STATUS_AUD			(0x01 << 14)

#define M14_B0_INTR_STATUS_FDD0			(0x00 << 0)

#define M14_B0_TMR_ENABLE				(0x01 << 0)

/*
; M14_B0 Read/Write Toolkit Offset
*/
#define M14_B0_TOOL_INFO				0x10
#define M14_B0_DEFAULT_PORT				0x14
#define M14_B0_PERMISSION				0x18

#define M14_B0_RD_LATENCY_BUFFER_DEPTH	0x1C

#define M14_B0_DATA_WIDTH				0x20
#define M14_B0_LM_BW_VALUE				0x40
#define M14_B0_GM_BW_VALUE				0x44
#define M14_B0_ISSUED_CMD_NUM			0x48
#define M14_B0_ISSUED_CMD_MAX_NUM		0x4C
#define M14_B0_PEAK_LATENCY				0x54
#define M14_B0_REQUEST_NUM				0x5C
#define M14_B0_LATENCY					0x60

#define M14_B0_RD_BUDGET_OVER_NUM		0x64
#define M14_B0_RD_RRD_ADDR_LIMIT_CNT	0x68

#define M14_B0_WR_PROTECTED				0x68
#define M14_B0_WR_VIOLATED_ADDR			0x6C
#define M14_B0_WR_VIOLATED_ID			0x70

#define M14_B0_RD_IMR_RESET				0x80

#define M14_B0_WR_MEM_PROT_RESET		0x80

#define M14_B0_BW_BUDGET				0x88

#define M14_B0_RD_LATENCY_BUDGET		0x8C

#define M14_B0_MNT_WINDOW_SIZE			0x90

#define M14_B0_RD_CMD_INTERVAL			0x94
#define M14_B0_RD_SATURATED_CMD_NUM		0x98
#define M14_B0_RD_ISSUING_CAPA			0x9C

#define M14_B0_RTE_PATH0				0xA0
#define M14_B0_RTE_PATH1				0xA4

#define M14_B0_RD_BYPASS_REORDER_CTRL	0xA8
#define M14_B0_RD_LATENCY_LIMIT_ENABLE	0xAC

#define M14_B0_BW_LIMIT_ENABLE			0xB0

#define M14_B0_WR_PROTECTION_BASE0		0xD0
#define M14_B0_WR_PROTECTION_END0		0xD4
#define M14_B0_WR_PROTECTION_BASE1		0xD8
#define M14_B0_WR_PROTECTION_END1		0xDC

#define M14_B0_WR_PROTECTION_ADDR_MASK	0xFFFFF000

#define M14_B0_WR_PRIORITY				0xF0

/*
; M14_B0 Read/Write Toolkit QoS Offset
*/
#define M14_B0_REG_PRIORITY_OFFSET		0x08
#define M14_B0_REG_MODE_OFFSET			0x0C
#define M14_B0_REG_BANDWIDTH_OFFSET		0x10
#define M14_B0_REG_SATURATION_OFFSET	0x14
#define M14_B0_REG_EXT_CTRL_OFFSET		0x18

#define M14_B0_REG_PRIORITY_MASK		0x0C
#define M14_B0_REG_MODE_MASK			0x03
#define M14_B0_REG_BANDWIDTH_MASK		0xFFF
#define M14_B0_REG_SATURATION_MASK		0xFFF


/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/

// M14_B0

typedef enum
{
	// ATOP Tookkit
	M14_B0_VDEC			= 0,
	M14_B0_HEVC0		= 1,
	M14_B0_HEVC1		= 2,
	M14_B0_CPU0			= 3,
	M14_B0_GPU0			= 4,
	M14_B0_USB			= 5,
	M14_B0_GPU1			= 6,
	M14_B0_CPU1			= 7,
	M14_B0_CPU_PERI		= 8,
	M14_B0_VDEC_MCU		= 9,
	M14_B0_IDS_MCU		= 10,

	// BTOP Toolkit
	M14_B0_DPE			= 11,
	M14_B0_DPP			= 12,
	M14_B0_FDA			= 13,
	M14_B0_DEO			= 14,
	M14_B0_DEM			= 15,
	M14_B0_DEI			= 16,
	M14_B0_CVD			= 17,
	M14_B0_TE			= 18,
	M14_B0_FDD1			= 19,
	M14_B0_FDB			= 20,
	M14_B0_FDC			= 21,
	M14_B0_VENC			= 22,
	M14_B0_GFX			= 23,
	M14_B0_AUD			= 24,
	M14_B0_FDD0			= 25,
	M14_B0_MAX_BLK
} M14_B0_BLK_T;


typedef enum
{
	M14_B0_REG_PRIORITY = 0,
	M14_B0_REG_BANDWIDTH,
	M14_B0_MAX_REG
} M14_B0_REG_T;

typedef enum
{
	M14_B0_REG_TYPE_ATOP = 0,
	M14_B0_REG_TYPE_BTOP,
	M14_B0_REG_TYPE_MAX
} M14_B0_REG_TYPE_T;

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

