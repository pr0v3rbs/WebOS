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
 *  VDO register details. ( used only within kdriver )
 *
 *  author     user name (user_name@lge.com)
 *  version    1.0
 *  date       2010.xx.xx
 *
 */

#ifndef _VDO_REG_H_
#define _VDO_REG_H_

/*----------------------------------------------------------------------------------------
    Control Constants
----------------------------------------------------------------------------------------*/
#include "vdec_type_defs.h"
#include "../lg1154_vdec_base.h"


#ifdef __XTENSA__
#define VDEC_VDO_BASE		(MCU_REG_BASE + 0x1F00)
#else
#define VDEC_VDO_BASE		(H13_VDEC_BASE + 0x0F00)
#endif


/////////////////////////////////////////////////////////////////////////////////////////
// VDO Register
// VDEC_VDO_BASE    : 0xF00
// VDO              : 0xF00 - 0xFFC
/////////////////////////////////////////////////////////////////////////////////////////

#define VDO_VERSION                      (*(volatile UINT32 *)(stpVdoReg + 0x000))
#define VDO_SW_RESET                     (*(volatile UINT32 *)(stpVdoReg + 0x004))
#define VDO_CONF                         (*(volatile UINT32 *)(stpVdoReg + 0x008))
#define VDO_CMD                          (*(volatile UINT32 *)(stpVdoReg + 0x00C))
#define VDO_CH_PIC_SIZE(i)               (*(volatile UINT32 *)(stpVdoReg + (i*0x10) + 0x010))
#define VDO_CH_BUF_IDX(i)                (*(volatile UINT32 *)(stpVdoReg + (i*0x10) + 0x014))
#define VDO_CH_CHROMA_FORMAT(i)          (*(volatile UINT32 *)(stpVdoReg + (i*0x10) + 0x018))
#define VDO_CH_SCAN_MODE(i)              (*(volatile UINT32 *)(stpVdoReg + (i*0x10) + 0x01C))

#define VDO_MIF_WAIT_REQB                (*(volatile UINT32 *)(stpVdoReg + 0x050))
#define VDO_LINE_WAIT                    (*(volatile UINT32 *)(stpVdoReg + 0x054))

#define VDO_VSYNC_SEL                    (*(volatile UINT32 *)(stpVdoReg + 0x060))
#define VDO_VSYNC_MODE                   (*(volatile UINT32 *)(stpVdoReg + 0x064))
#define VDO_VSYNC_GEN                    (*(volatile UINT32 *)(stpVdoReg + 0x068))
#define VDO_VACTV_GEN                    (*(volatile UINT32 *)(stpVdoReg + 0x06C))

#define VDO_MIF_LINE_CNT                 (*(volatile UINT32 *)(stpVdoReg + 0x070))
#define VDO_MIF_STATUS                   (*(volatile UINT32 *)(stpVdoReg + 0x074))
#define VDO_RDC_LINE_CNT                 (*(volatile UINT32 *)(stpVdoReg + 0x078))
#define VDO_RDC_STATUS                   (*(volatile UINT32 *)(stpVdoReg + 0x07C))

#define VDO_FLUSH_CONF                   (*(volatile UINT32 *)(stpVdoReg + 0x080))

#define VDO_CH_VSYNC_VACTIV_CNT(i)       (*(volatile UINT32 *)(stpVdoReg + (i*0x04) + 0x090))

#define VDO_CH0_LUMA_STALL_CNT           (*(volatile UINT32 *)(stpVdoReg + 0x0A0))
#define VDO_CH0_CHMA_STALL_CNT           (*(volatile UINT32 *)(stpVdoReg + 0x0A4))
#define VDO_CH1_LUMA_STALL_CNT           (*(volatile UINT32 *)(stpVdoReg + 0x0A8))
#define VDO_CH1_CHMA_STALL_CNT           (*(volatile UINT32 *)(stpVdoReg + 0x0AC))

//#define VDO_CH_VBP_NUM(i)               (*(volatile UINT32 *)(stpVdoReg + (i*0x10) + 0x100)) // 0x100
//#define VDO_CH_VFP_NUM(i)               (*(volatile UINT32 *)(stpVdoReg + (i*0x10) + 0x104)) // 0x104
//#define VDO_CH_VSYNC_NUM(i)             (*(volatile UINT32 *)(stpVdoReg + (i*0x10) + 0x108)) // 0x108

/////////////////////////////////////////////////////////
// Must set RC0 APB_CONF register
// reg_apb_vdo_gdi_sel[   24] = 0x0
// reg_apb_vdo_offset [31:28] = 0x1
/////////////////////////////////////////////////////////
#define VDO_CH_VBP_NUM(i)               (*(volatile UINT32 *)(stpVdoReg + (i*0x10) + 0x000)) // 0x100
#define VDO_CH_VFP_NUM(i)               (*(volatile UINT32 *)(stpVdoReg + (i*0x10) + 0x004)) // 0x104
#define VDO_CH_VSYNC_NUM(i)             (*(volatile UINT32 *)(stpVdoReg + (i*0x10) + 0x008)) // 0x108

//#define VDO_CH1_VBP_NUM                  (*(volatile UINT32 *)(stpVdoReg + 0x010)) // 0x110
//#define VDO_CH1_VFP_NUM                  (*(volatile UINT32 *)(stpVdoReg + 0x014)) // 0x114
//#define VDO_CH1_VSYNC_NUM                (*(volatile UINT32 *)(stpVdoReg + 0x018)) // 0x118

// VDO GDMA Register
/////////////////////////////////////////////////////////
// Must set RC0 APB_CONF register
// reg_apb_vdo_gdi_sel[   24] = 0x1
// reg_apb_vdo_offset [31:28] = 0x0
/////////////////////////////////////////////////////////
#define VDO_GDI_PRI_RD_PRIO_L            (*(volatile UINT32 *)(stpVdoReg + 0x000))
#define VDO_GDI_PRI_RD_PRIO_H            (*(volatile UINT32 *)(stpVdoReg + 0x004))
#define VDO_GDI_PRI_WR_PRIO_L            (*(volatile UINT32 *)(stpVdoReg + 0x008))
#define VDO_GDI_PRI_WR_PRIO_H            (*(volatile UINT32 *)(stpVdoReg + 0x00c))
#define VDO_GDI_PRI_RD_LOCK_CNT          (*(volatile UINT32 *)(stpVdoReg + 0x010))
#define VDO_GDI_PRI_WR_LOCK_CNT          (*(volatile UINT32 *)(stpVdoReg + 0x014))
#define VDO_GDI_SEC_RD_PRIO_L            (*(volatile UINT32 *)(stpVdoReg + 0x018))
#define VDO_GDI_SEC_RD_PRIO_H            (*(volatile UINT32 *)(stpVdoReg + 0x01c))
#define VDO_GDI_SEC_WR_PRIO_L            (*(volatile UINT32 *)(stpVdoReg + 0x020))
#define VDO_GDI_SEC_WR_PRIO_H            (*(volatile UINT32 *)(stpVdoReg + 0x024))
#define VDO_GDI_SEC_RD_LOCK_CNT          (*(volatile UINT32 *)(stpVdoReg + 0x028))
#define VDO_GDI_SEC_WR_LOCK_CNT          (*(volatile UINT32 *)(stpVdoReg + 0x02c))
#define VDO_GDI_SEC_CLIENT_EN            (*(volatile UINT32 *)(stpVdoReg + 0x030))
#define VDO_GDI_CONTROL                  (*(volatile UINT32 *)(stpVdoReg + 0x034))
#define VDO_GDI_PIC_INIT_HOST            (*(volatile UINT32 *)(stpVdoReg + 0x038))

#define VDO_GDI_PINFO_REQ                (*(volatile UINT32 *)(stpVdoReg + 0x060))
#define VDO_GDI_PINFO_ACK                (*(volatile UINT32 *)(stpVdoReg + 0x064))
#define VDO_GDI_PINFO_ADDR               (*(volatile UINT32 *)(stpVdoReg + 0x068))
#define VDO_GDI_PINFO_DATA               (*(volatile UINT32 *)(stpVdoReg + 0x06c))
#define VDO_GDI_BWB_ENABLE               (*(volatile UINT32 *)(stpVdoReg + 0x070))
#define VDO_GDI_BWB_SIZE                 (*(volatile UINT32 *)(stpVdoReg + 0x074))
#define VDO_GDI_BWB_STD_STRUCT           (*(volatile UINT32 *)(stpVdoReg + 0x078))
#define VDO_GDI_BWB_STATUS               (*(volatile UINT32 *)(stpVdoReg + 0x07c))

#define VDO_GDI_STATUS                   (*(volatile UINT32 *)(stpVdoReg + 0x080))
#define VDO_GDI_DEBUG_0                  (*(volatile UINT32 *)(stpVdoReg + 0x084))
#define VDO_GDI_DEBUG_1                  (*(volatile UINT32 *)(stpVdoReg + 0x088))
#define VDO_GDI_DEBUG_2                  (*(volatile UINT32 *)(stpVdoReg + 0x08c))
#define VDO_GDI_DEBUG_3                  (*(volatile UINT32 *)(stpVdoReg + 0x090))
#define VDO_GDI_DEBUG_PROBE_ADDR         (*(volatile UINT32 *)(stpVdoReg + 0x094))
#define VDO_GDI_DEBUG_PROBE_DATA         (*(volatile UINT32 *)(stpVdoReg + 0x098))

#define VDO_GDI_NO_MORE_REQ              (*(volatile UINT32 *)(stpVdoReg + 0x0f0))
#define VDO_GDI_EMPTY_FLAG               (*(volatile UINT32 *)(stpVdoReg + 0x0f4))
#define VDO_GDI_DCU_PIC_SIZE             (*(volatile UINT32 *)(stpVdoReg + 0x0a8))
#define VDO_GDI_SIZE_ERR_FLAG            (*(volatile UINT32 *)(stpVdoReg + 0x0e0))

/////////////////////////////////////////////////////////
// Must set RC0 APB_CONF register
// reg_apb_vdo_gdi_sel[   24] = 0x1
// reg_apb_vdo_offset [31:28] = 0x4
/////////////////////////////////////////////////////////
#define VDO_GDI_INFO_CONTROL(frame_num)  (*(volatile UINT32 *)(stpVdoReg + frame_num*5*4 + 0x000)) // 0x400
#define VDO_GDI_INFO_PIC_SIZE(frame_num) (*(volatile UINT32 *)(stpVdoReg + frame_num*5*4 + 0x004)) // 0x400
#define VDO_GDI_INFO_BASE_Y(frame_num)   (*(volatile UINT32 *)(stpVdoReg + frame_num*5*4 + 0x008)) // 0x400
#define VDO_GDI_INFO_BASE_CB(frame_num)  (*(volatile UINT32 *)(stpVdoReg + frame_num*5*4 + 0x00C)) // 0x400
#define VDO_GDI_INFO_BASE_CR(frame_num)  (*(volatile UINT32 *)(stpVdoReg + frame_num*5*4 + 0x010)) // 0x400

/////////////////////////////////////////////////////////
// Must set RC0 APB_CONF register
// reg_apb_vdo_gdi_sel[   24] = 0x1
// reg_apb_vdo_offset [31:28] = 0x8
/////////////////////////////////////////////////////////
#define VDO_GDI_XY2_CAS_0                (*(volatile UINT32 *)(stpVdoReg + 0x000))  //800
#define VDO_GDI_XY2_CAS_1                (*(volatile UINT32 *)(stpVdoReg + 0x004))  //800
#define VDO_GDI_XY2_CAS_2                (*(volatile UINT32 *)(stpVdoReg + 0x008))  //800
#define VDO_GDI_XY2_CAS_3                (*(volatile UINT32 *)(stpVdoReg + 0x00C))  //800
#define VDO_GDI_XY2_CAS_4                (*(volatile UINT32 *)(stpVdoReg + 0x010))  //800
#define VDO_GDI_XY2_CAS_5                (*(volatile UINT32 *)(stpVdoReg + 0x014))  //800
#define VDO_GDI_XY2_CAS_6                (*(volatile UINT32 *)(stpVdoReg + 0x018))  //800
#define VDO_GDI_XY2_CAS_7                (*(volatile UINT32 *)(stpVdoReg + 0x01C))  //800
#define VDO_GDI_XY2_CAS_8                (*(volatile UINT32 *)(stpVdoReg + 0x020))  //800
#define VDO_GDI_XY2_CAS_9                (*(volatile UINT32 *)(stpVdoReg + 0x024))  //800
#define VDO_GDI_XY2_CAS_A                (*(volatile UINT32 *)(stpVdoReg + 0x028))  //800
#define VDO_GDI_XY2_CAS_B                (*(volatile UINT32 *)(stpVdoReg + 0x02C))  //800
#define VDO_GDI_XY2_CAS_C                (*(volatile UINT32 *)(stpVdoReg + 0x030))  //800
#define VDO_GDI_XY2_CAS_D                (*(volatile UINT32 *)(stpVdoReg + 0x034))  //800
#define VDO_GDI_XY2_CAS_E                (*(volatile UINT32 *)(stpVdoReg + 0x038))  //800
#define VDO_GDI_XY2_CAS_F                (*(volatile UINT32 *)(stpVdoReg + 0x03C))  //800

#define VDO_GDI_XY2_BA_0                 (*(volatile UINT32 *)(stpVdoReg + 0x040))  //800
#define VDO_GDI_XY2_BA_1                 (*(volatile UINT32 *)(stpVdoReg + 0x044))  //800
#define VDO_GDI_XY2_BA_2                 (*(volatile UINT32 *)(stpVdoReg + 0x048))  //800
#define VDO_GDI_XY2_BA_3                 (*(volatile UINT32 *)(stpVdoReg + 0x04C))  //800

#define VDO_GDI_XY2_RAS_0                (*(volatile UINT32 *)(stpVdoReg + 0x050))  //800
#define VDO_GDI_XY2_RAS_1                (*(volatile UINT32 *)(stpVdoReg + 0x054))  //800
#define VDO_GDI_XY2_RAS_2                (*(volatile UINT32 *)(stpVdoReg + 0x058))  //800
#define VDO_GDI_XY2_RAS_3                (*(volatile UINT32 *)(stpVdoReg + 0x05C))  //800
#define VDO_GDI_XY2_RAS_4                (*(volatile UINT32 *)(stpVdoReg + 0x060))  //800
#define VDO_GDI_XY2_RAS_5                (*(volatile UINT32 *)(stpVdoReg + 0x064))  //800
#define VDO_GDI_XY2_RAS_6                (*(volatile UINT32 *)(stpVdoReg + 0x068))  //800
#define VDO_GDI_XY2_RAS_7                (*(volatile UINT32 *)(stpVdoReg + 0x06C))  //800
#define VDO_GDI_XY2_RAS_8                (*(volatile UINT32 *)(stpVdoReg + 0x070))  //800
#define VDO_GDI_XY2_RAS_9                (*(volatile UINT32 *)(stpVdoReg + 0x074))  //800
#define VDO_GDI_XY2_RAS_A                (*(volatile UINT32 *)(stpVdoReg + 0x078))  //800
#define VDO_GDI_XY2_RAS_B                (*(volatile UINT32 *)(stpVdoReg + 0x07C))  //800
#define VDO_GDI_XY2_RAS_C                (*(volatile UINT32 *)(stpVdoReg + 0x080))  //800
#define VDO_GDI_XY2_RAS_D                (*(volatile UINT32 *)(stpVdoReg + 0x084))  //800
#define VDO_GDI_XY2_RAS_E                (*(volatile UINT32 *)(stpVdoReg + 0x088))  //800
#define VDO_GDI_XY2_RAS_F                (*(volatile UINT32 *)(stpVdoReg + 0x08C))  //800

#define VDO_GDI_XY2_RBC_CONFIG           (*(volatile UINT32 *)(stpVdoReg + 0x090))  //800

#define VDO_GDI_RBC2_AXI_0               (*(volatile UINT32 *)(stpVdoReg + 0x0A0))  //800
#define VDO_GDI_RBC2_AXI_1               (*(volatile UINT32 *)(stpVdoReg + 0x0A4))  //800
#define VDO_GDI_RBC2_AXI_2               (*(volatile UINT32 *)(stpVdoReg + 0x0A8))  //800
#define VDO_GDI_RBC2_AXI_3               (*(volatile UINT32 *)(stpVdoReg + 0x0AC))  //800
#define VDO_GDI_RBC2_AXI_4               (*(volatile UINT32 *)(stpVdoReg + 0x0B0))  //800
#define VDO_GDI_RBC2_AXI_5               (*(volatile UINT32 *)(stpVdoReg + 0x0B4))  //800
#define VDO_GDI_RBC2_AXI_6               (*(volatile UINT32 *)(stpVdoReg + 0x0B8))  //800
#define VDO_GDI_RBC2_AXI_7               (*(volatile UINT32 *)(stpVdoReg + 0x0BC))  //800
#define VDO_GDI_RBC2_AXI_8               (*(volatile UINT32 *)(stpVdoReg + 0x0C0))  //800
#define VDO_GDI_RBC2_AXI_9               (*(volatile UINT32 *)(stpVdoReg + 0x0C4))  //800
#define VDO_GDI_RBC2_AXI_A               (*(volatile UINT32 *)(stpVdoReg + 0x0C8))  //800
#define VDO_GDI_RBC2_AXI_B               (*(volatile UINT32 *)(stpVdoReg + 0x0CC))  //800
#define VDO_GDI_RBC2_AXI_C               (*(volatile UINT32 *)(stpVdoReg + 0x0D0))  //800
#define VDO_GDI_RBC2_AXI_D               (*(volatile UINT32 *)(stpVdoReg + 0x0D4))  //800
#define VDO_GDI_RBC2_AXI_E               (*(volatile UINT32 *)(stpVdoReg + 0x0D8))  //800
#define VDO_GDI_RBC2_AXI_F               (*(volatile UINT32 *)(stpVdoReg + 0x0DC))  //800
#define VDO_GDI_RBC2_AXI_10              (*(volatile UINT32 *)(stpVdoReg + 0x0E0))  //800
#define VDO_GDI_RBC2_AXI_11              (*(volatile UINT32 *)(stpVdoReg + 0x0E4))  //800
#define VDO_GDI_RBC2_AXI_12              (*(volatile UINT32 *)(stpVdoReg + 0x0E8))  //800
#define VDO_GDI_RBC2_AXI_13              (*(volatile UINT32 *)(stpVdoReg + 0x0EC))  //800
#define VDO_GDI_RBC2_AXI_14              (*(volatile UINT32 *)(stpVdoReg + 0x0F0))  //800
#define VDO_GDI_RBC2_AXI_15              (*(volatile UINT32 *)(stpVdoReg + 0x0F4))  //800
#define VDO_GDI_RBC2_AXI_16              (*(volatile UINT32 *)(stpVdoReg + 0x0F8))  //800
#define VDO_GDI_RBC2_AXI_17              (*(volatile UINT32 *)(stpVdoReg + 0x0FC))  //800
/////////////////////////////////////////////////////////
// Must set RC0 APB_CONF register
// reg_apb_vdo_gdi_sel[   24] = 0x1
// reg_apb_vdo_offset [31:28] = 0x9
/////////////////////////////////////////////////////////
#define VDO_GDI_RBC2_AXI_18              (*(volatile UINT32 *)(stpVdoReg + 0x000))  //900
#define VDO_GDI_RBC2_AXI_19              (*(volatile UINT32 *)(stpVdoReg + 0x004))  //900
#define VDO_GDI_RBC2_AXI_1A              (*(volatile UINT32 *)(stpVdoReg + 0x008))  //900
#define VDO_GDI_RBC2_AXI_1B              (*(volatile UINT32 *)(stpVdoReg + 0x00C))  //900
#define VDO_GDI_RBC2_AXI_1C              (*(volatile UINT32 *)(stpVdoReg + 0x010))  //900
#define VDO_GDI_RBC2_AXI_1D              (*(volatile UINT32 *)(stpVdoReg + 0x014))  //900
#define VDO_GDI_RBC2_AXI_1E              (*(volatile UINT32 *)(stpVdoReg + 0x018))  //900
#define VDO_GDI_RBC2_AXI_1F              (*(volatile UINT32 *)(stpVdoReg + 0x01C))  //900
#define VDO_GDI_TILEDBUF_BASE            (*(volatile UINT32 *)(stpVdoReg + 0x020))  //900
/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/

#if 0
#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------------
	0x0000 version ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	yyyymmdd                        ;   	// 31: 0
} VERSION;

/*-----------------------------------------------------------------------------
	0x0008 vdo_conf ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_dual_mode                   : 1,	//     0
	                                : 3,	//  1: 3 reserved
	reg_chan_sel                    : 1,	//     4
	                                : 3,	//  5: 7 reserved
	reg_vdo_ch0_vsync_sel           : 1,	//     8
	reg_vdo_ch1_vsync_sel           : 1,	//     9
	                                : 2,	// 10:11 reserved
	reg_vdo_endian                  : 2,	// 12:13
	                                : 2,	// 14:15 reserved
	reg_reqb_hlen                   : 8,	// 16:23
	reg_reqb_wlen                   : 8;	// 24:31
} VDO_CONF;

/*-----------------------------------------------------------------------------
	0x000c vdo_cmd ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pic_run                         : 1,	//     0
	                                : 3,	//  1: 3 reserved
	pic_init                        : 1,	//     4
	                                :23,	//  5:27 reserved
	vdo_en                          : 1;	//    28
} VDO_CMD;

/*-----------------------------------------------------------------------------
	0x0010 ch0_pic_size ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	ch0_pic_height                  :12,	//  0:11
	                                : 4,	// 12:15 reserved
	ch0_pic_width                   :12;	// 16:27
} CH0_PIC_SIZE;

/*-----------------------------------------------------------------------------
	0x0014 ch0_buf_idx ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	ch0_buf_idx                     : 5;	//  0: 4
} CH0_BUF_IDX;

/*-----------------------------------------------------------------------------
	0x0018 ch0_chroma_format ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	ch0_cformat                     : 3,	//  0: 2
	                                :13,	//  3:15 reserved
	ch0_cformat_int                 : 3;	// 16:18
} CH0_CHROMA_FORMAT;

/*-----------------------------------------------------------------------------
	0x001c ch0_scan_mode ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	ch0_field                       : 1,	//     0
	                                : 3,	//  1: 3 reserved
	ch0_prog_scan                   : 1;	//     4
} CH0_SCAN_MODE;

/*-----------------------------------------------------------------------------
	0x0020 ch1_pic_size ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	ch1_pic_height                  :12,	//  0:11
	                                : 4,	// 12:15 reserved
	ch1_pic_width                   :12;	// 16:27
} CH1_PIC_SIZE;

/*-----------------------------------------------------------------------------
	0x0024 ch1_buf_idx ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	ch1_buf_idx                     : 5;	//  0: 4
} CH1_BUF_IDX;

/*-----------------------------------------------------------------------------
	0x0028 ch1_chroma_format ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	ch1_cformat                     : 3,	//  0: 2
	                                :13,	//  3:15 reserved
	ch1_cformat_int                 : 3;	// 16:18
} CH1_CHROMA_FORMAT;

/*-----------------------------------------------------------------------------
	0x002c ch1_scan_mode ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	ch1_field                       : 1,	//     0
	                                : 3,	//  1: 3 reserved
	ch1_prog_scan                   : 1;	//     4
} CH1_SCAN_MODE;

/*-----------------------------------------------------------------------------
	0x0030 mif_wait_reqb ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reqb_wait_cycle                 :16,	//  0:15
	                                :12,	// 16:27 reserved
	reqb_wait_en                    : 1;	//    28
} MIF_WAIT_REQB;

/*-----------------------------------------------------------------------------
	0x0100 ch0_vbp_num ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	ch0_vbp_num                     :16;	//  0:15
} CH0_VBP_NUM;

/*-----------------------------------------------------------------------------
	0x0104 ch0_vfp_num ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	ch0_vfp_num                     :16;	//  0:15
} CH0_VFP_NUM;

/*-----------------------------------------------------------------------------
	0x0108 ch0_vsync_num ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	ch0_vsync_num                   :16;	//  0:15
} CH0_VSYNC_NUM;

/*-----------------------------------------------------------------------------
	0x0110 ch1_vbp_num ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	ch1_vbp_num                     :16;	//  0:15
} CH1_VBP_NUM;

/*-----------------------------------------------------------------------------
	0x0114 ch1_vfp_num ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	ch1_vfp_num                     :16;	//  0:15
} CH1_VFP_NUM;

/*-----------------------------------------------------------------------------
	0x0118 ch1_vsync_num ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	ch1_vsync_num                   :16;	//  0:15
} CH1_VSYNC_NUM;

typedef struct {
	VERSION                         	version                         ;	// 0x0000 : ''
	UINT32                          	                 __rsvd_00[   1];	// 0x0004
	VDO_CONF                        	vdo_conf                        ;	// 0x0008 : ''
	VDO_CMD                         	vdo_cmd                         ;	// 0x000c : ''
	CH0_PIC_SIZE                    	ch0_pic_size                    ;	// 0x0010 : ''
	CH0_BUF_IDX                     	ch0_buf_idx                     ;	// 0x0014 : ''
	CH0_CHROMA_FORMAT               	ch0_chroma_format               ;	// 0x0018 : ''
	CH0_SCAN_MODE                   	ch0_scan_mode                   ;	// 0x001c : ''
	CH1_PIC_SIZE                    	ch1_pic_size                    ;	// 0x0020 : ''
	CH1_BUF_IDX                     	ch1_buf_idx                     ;	// 0x0024 : ''
	CH1_CHROMA_FORMAT               	ch1_chroma_format               ;	// 0x0028 : ''
	CH1_SCAN_MODE                   	ch1_scan_mode                   ;	// 0x002c : ''
	MIF_WAIT_REQB                   	mif_wait_reqb                   ;	// 0x0030 : ''
	UINT32                          	                 __rsvd_01[  51];	// 0x0034 ~ 0x00fc
	CH0_VBP_NUM                     	ch0_vbp_num                     ;	// 0x0100 : ''
	CH0_VFP_NUM                     	ch0_vfp_num                     ;	// 0x0104 : ''
	CH0_VSYNC_NUM                   	ch0_vsync_num                   ;	// 0x0108 : ''
	UINT32                          	                 __rsvd_02[   1];	// 0x010c
	CH1_VBP_NUM                     	ch1_vbp_num                     ;	// 0x0110 : ''
	CH1_VFP_NUM                     	ch1_vfp_num                     ;	// 0x0114 : ''
	CH1_VSYNC_NUM                   	ch1_vsync_num                   ;	// 0x0118 : ''
} VDO_REG_T;
/* 18 regs, 18 types */

/* 18 regs, 18 types in Total*/

/*
 * @{
 * Naming for register pointer.
 * gpRealRegVDO : real register of VDO.
 * gpRegVDO     : shadow register.
 *
 * @def VDO_RdFL: Read  FLushing : Shadow <- Real.
 * @def VDO_WrFL: Write FLushing : Shadow -> Real.
 * @def VDO_Rd  : Read  whole register(UINT32) from Shadow register.
 * @def VDO_Wr  : Write whole register(UINT32) from Shadow register.
 * @def VDO_Rd01 ~ VDO_Rdnn: Read  given '01~nn' fields from Shadow register.
 * @def VDO_Wr01 ~ VDO_Wrnn: Write given '01~nn' fields to   Shadow register.
 * */
#define VDO_RdFL(_r)			((gpRegVDO->_r)=(gpRealRegVDO->_r))
#define VDO_WrFL(_r)			((gpRealRegVDO->_r)=(gpRegVDO->_r))

#define VDO_Rd(_r)			*((UINT32*)(&(gpRegVDO->_r)))
#define VDO_Wr(_r,_v)			((VDO_Rd(_r))=((UINT32)(_v)))

#define VDO_Rd01(_r,_f01,_v01)													\
								do { 											\
									(_v01) = (gpRegVDO->_r._f01);				\
								} while(0)

#define VDO_Rd02(_r,_f01,_v01,_f02,_v02)										\
								do { 											\
									(_v01) = (gpRegVDO->_r._f01);				\
									(_v02) = (gpRegVDO->_r._f02);				\
								} while(0)

#define VDO_Rd03(_r,_f01,_v01,_f02,_v02,_f03,_v03)								\
								do { 											\
									(_v01) = (gpRegVDO->_r._f01);				\
									(_v02) = (gpRegVDO->_r._f02);				\
									(_v03) = (gpRegVDO->_r._f03);				\
								} while(0)

#define VDO_Rd04(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04)					\
								do { 											\
									(_v01) = (gpRegVDO->_r._f01);				\
									(_v02) = (gpRegVDO->_r._f02);				\
									(_v03) = (gpRegVDO->_r._f03);				\
									(_v04) = (gpRegVDO->_r._f04);				\
								} while(0)

#define VDO_Rd05(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05)													\
								do { 											\
									(_v01) = (gpRegVDO->_r._f01);				\
									(_v02) = (gpRegVDO->_r._f02);				\
									(_v03) = (gpRegVDO->_r._f03);				\
									(_v04) = (gpRegVDO->_r._f04);				\
									(_v05) = (gpRegVDO->_r._f05);				\
								} while(0)

#define VDO_Rd06(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06)										\
								do { 											\
									(_v01) = (gpRegVDO->_r._f01);				\
									(_v02) = (gpRegVDO->_r._f02);				\
									(_v03) = (gpRegVDO->_r._f03);				\
									(_v04) = (gpRegVDO->_r._f04);				\
									(_v05) = (gpRegVDO->_r._f05);				\
									(_v06) = (gpRegVDO->_r._f06);				\
								} while(0)

#define VDO_Rd07(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07)								\
								do { 											\
									(_v01) = (gpRegVDO->_r._f01);				\
									(_v02) = (gpRegVDO->_r._f02);				\
									(_v03) = (gpRegVDO->_r._f03);				\
									(_v04) = (gpRegVDO->_r._f04);				\
									(_v05) = (gpRegVDO->_r._f05);				\
									(_v06) = (gpRegVDO->_r._f06);				\
									(_v07) = (gpRegVDO->_r._f07);				\
								} while(0)

#define VDO_Rd08(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08)					\
								do { 											\
									(_v01) = (gpRegVDO->_r._f01);				\
									(_v02) = (gpRegVDO->_r._f02);				\
									(_v03) = (gpRegVDO->_r._f03);				\
									(_v04) = (gpRegVDO->_r._f04);				\
									(_v05) = (gpRegVDO->_r._f05);				\
									(_v06) = (gpRegVDO->_r._f06);				\
									(_v07) = (gpRegVDO->_r._f07);				\
									(_v08) = (gpRegVDO->_r._f08);				\
								} while(0)

#define VDO_Rd09(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09)													\
								do { 											\
									(_v01) = (gpRegVDO->_r._f01);				\
									(_v02) = (gpRegVDO->_r._f02);				\
									(_v03) = (gpRegVDO->_r._f03);				\
									(_v04) = (gpRegVDO->_r._f04);				\
									(_v05) = (gpRegVDO->_r._f05);				\
									(_v06) = (gpRegVDO->_r._f06);				\
									(_v07) = (gpRegVDO->_r._f07);				\
									(_v08) = (gpRegVDO->_r._f08);				\
									(_v09) = (gpRegVDO->_r._f09);				\
								} while(0)

#define VDO_Rd10(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10)										\
								do { 											\
									(_v01) = (gpRegVDO->_r._f01);				\
									(_v02) = (gpRegVDO->_r._f02);				\
									(_v03) = (gpRegVDO->_r._f03);				\
									(_v04) = (gpRegVDO->_r._f04);				\
									(_v05) = (gpRegVDO->_r._f05);				\
									(_v06) = (gpRegVDO->_r._f06);				\
									(_v07) = (gpRegVDO->_r._f07);				\
									(_v08) = (gpRegVDO->_r._f08);				\
									(_v09) = (gpRegVDO->_r._f09);				\
									(_v10) = (gpRegVDO->_r._f10);				\
								} while(0)

#define VDO_Rd11(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11)								\
								do { 											\
									(_v01) = (gpRegVDO->_r._f01);				\
									(_v02) = (gpRegVDO->_r._f02);				\
									(_v03) = (gpRegVDO->_r._f03);				\
									(_v04) = (gpRegVDO->_r._f04);				\
									(_v05) = (gpRegVDO->_r._f05);				\
									(_v06) = (gpRegVDO->_r._f06);				\
									(_v07) = (gpRegVDO->_r._f07);				\
									(_v08) = (gpRegVDO->_r._f08);				\
									(_v09) = (gpRegVDO->_r._f09);				\
									(_v10) = (gpRegVDO->_r._f10);				\
									(_v11) = (gpRegVDO->_r._f11);				\
								} while(0)

#define VDO_Rd12(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12)					\
								do { 											\
									(_v01) = (gpRegVDO->_r._f01);				\
									(_v02) = (gpRegVDO->_r._f02);				\
									(_v03) = (gpRegVDO->_r._f03);				\
									(_v04) = (gpRegVDO->_r._f04);				\
									(_v05) = (gpRegVDO->_r._f05);				\
									(_v06) = (gpRegVDO->_r._f06);				\
									(_v07) = (gpRegVDO->_r._f07);				\
									(_v08) = (gpRegVDO->_r._f08);				\
									(_v09) = (gpRegVDO->_r._f09);				\
									(_v10) = (gpRegVDO->_r._f10);				\
									(_v11) = (gpRegVDO->_r._f11);				\
									(_v12) = (gpRegVDO->_r._f12);				\
								} while(0)

#define VDO_Rd13(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13)													\
								do { 											\
									(_v01) = (gpRegVDO->_r._f01);				\
									(_v02) = (gpRegVDO->_r._f02);				\
									(_v03) = (gpRegVDO->_r._f03);				\
									(_v04) = (gpRegVDO->_r._f04);				\
									(_v05) = (gpRegVDO->_r._f05);				\
									(_v06) = (gpRegVDO->_r._f06);				\
									(_v07) = (gpRegVDO->_r._f07);				\
									(_v08) = (gpRegVDO->_r._f08);				\
									(_v09) = (gpRegVDO->_r._f09);				\
									(_v10) = (gpRegVDO->_r._f10);				\
									(_v11) = (gpRegVDO->_r._f11);				\
									(_v12) = (gpRegVDO->_r._f12);				\
									(_v13) = (gpRegVDO->_r._f13);				\
								} while(0)

#define VDO_Rd14(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14)										\
								do { 											\
									(_v01) = (gpRegVDO->_r._f01);				\
									(_v02) = (gpRegVDO->_r._f02);				\
									(_v03) = (gpRegVDO->_r._f03);				\
									(_v04) = (gpRegVDO->_r._f04);				\
									(_v05) = (gpRegVDO->_r._f05);				\
									(_v06) = (gpRegVDO->_r._f06);				\
									(_v07) = (gpRegVDO->_r._f07);				\
									(_v08) = (gpRegVDO->_r._f08);				\
									(_v09) = (gpRegVDO->_r._f09);				\
									(_v10) = (gpRegVDO->_r._f10);				\
									(_v11) = (gpRegVDO->_r._f11);				\
									(_v12) = (gpRegVDO->_r._f12);				\
									(_v13) = (gpRegVDO->_r._f13);				\
									(_v14) = (gpRegVDO->_r._f14);				\
								} while(0)

#define VDO_Rd15(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15)								\
								do { 											\
									(_v01) = (gpRegVDO->_r._f01);				\
									(_v02) = (gpRegVDO->_r._f02);				\
									(_v03) = (gpRegVDO->_r._f03);				\
									(_v04) = (gpRegVDO->_r._f04);				\
									(_v05) = (gpRegVDO->_r._f05);				\
									(_v06) = (gpRegVDO->_r._f06);				\
									(_v07) = (gpRegVDO->_r._f07);				\
									(_v08) = (gpRegVDO->_r._f08);				\
									(_v09) = (gpRegVDO->_r._f09);				\
									(_v10) = (gpRegVDO->_r._f10);				\
									(_v11) = (gpRegVDO->_r._f11);				\
									(_v12) = (gpRegVDO->_r._f12);				\
									(_v13) = (gpRegVDO->_r._f13);				\
									(_v14) = (gpRegVDO->_r._f14);				\
									(_v15) = (gpRegVDO->_r._f15);				\
								} while(0)

#define VDO_Rd16(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15,_f16,_v16)					\
								do { 											\
									(_v01) = (gpRegVDO->_r._f01);				\
									(_v02) = (gpRegVDO->_r._f02);				\
									(_v03) = (gpRegVDO->_r._f03);				\
									(_v04) = (gpRegVDO->_r._f04);				\
									(_v05) = (gpRegVDO->_r._f05);				\
									(_v06) = (gpRegVDO->_r._f06);				\
									(_v07) = (gpRegVDO->_r._f07);				\
									(_v08) = (gpRegVDO->_r._f08);				\
									(_v09) = (gpRegVDO->_r._f09);				\
									(_v10) = (gpRegVDO->_r._f10);				\
									(_v11) = (gpRegVDO->_r._f11);				\
									(_v12) = (gpRegVDO->_r._f12);				\
									(_v13) = (gpRegVDO->_r._f13);				\
									(_v14) = (gpRegVDO->_r._f14);				\
									(_v15) = (gpRegVDO->_r._f15);				\
									(_v16) = (gpRegVDO->_r._f16);				\
								} while(0)


#define VDO_Wr01(_r,_f01,_v01)													\
								do { 											\
									(gpRegVDO->_r._f01) = (_v01);				\
								} while(0)

#define VDO_Wr02(_r,_f01,_v01,_f02,_v02)										\
								do { 											\
									(gpRegVDO->_r._f01) = (_v01);				\
									(gpRegVDO->_r._f02) = (_v02);				\
								} while(0)

#define VDO_Wr03(_r,_f01,_v01,_f02,_v02,_f03,_v03)								\
								do { 											\
									(gpRegVDO->_r._f01) = (_v01);				\
									(gpRegVDO->_r._f02) = (_v02);				\
									(gpRegVDO->_r._f03) = (_v03);				\
								} while(0)

#define VDO_Wr04(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04)					\
								do { 											\
									(gpRegVDO->_r._f01) = (_v01);				\
									(gpRegVDO->_r._f02) = (_v02);				\
									(gpRegVDO->_r._f03) = (_v03);				\
									(gpRegVDO->_r._f04) = (_v04);				\
								} while(0)

#define VDO_Wr05(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05)													\
								do { 											\
									(gpRegVDO->_r._f01) = (_v01);				\
									(gpRegVDO->_r._f02) = (_v02);				\
									(gpRegVDO->_r._f03) = (_v03);				\
									(gpRegVDO->_r._f04) = (_v04);				\
									(gpRegVDO->_r._f05) = (_v05);				\
								} while(0)

#define VDO_Wr06(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06)										\
								do { 											\
									(gpRegVDO->_r._f01) = (_v01);				\
									(gpRegVDO->_r._f02) = (_v02);				\
									(gpRegVDO->_r._f03) = (_v03);				\
									(gpRegVDO->_r._f04) = (_v04);				\
									(gpRegVDO->_r._f05) = (_v05);				\
									(gpRegVDO->_r._f06) = (_v06);				\
								} while(0)

#define VDO_Wr07(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07)								\
								do { 											\
									(gpRegVDO->_r._f01) = (_v01);				\
									(gpRegVDO->_r._f02) = (_v02);				\
									(gpRegVDO->_r._f03) = (_v03);				\
									(gpRegVDO->_r._f04) = (_v04);				\
									(gpRegVDO->_r._f05) = (_v05);				\
									(gpRegVDO->_r._f06) = (_v06);				\
									(gpRegVDO->_r._f07) = (_v07);				\
								} while(0)

#define VDO_Wr08(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08)					\
								do { 											\
									(gpRegVDO->_r._f01) = (_v01);				\
									(gpRegVDO->_r._f02) = (_v02);				\
									(gpRegVDO->_r._f03) = (_v03);				\
									(gpRegVDO->_r._f04) = (_v04);				\
									(gpRegVDO->_r._f05) = (_v05);				\
									(gpRegVDO->_r._f06) = (_v06);				\
									(gpRegVDO->_r._f07) = (_v07);				\
									(gpRegVDO->_r._f08) = (_v08);				\
								} while(0)

#define VDO_Wr09(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09)													\
								do { 											\
									(gpRegVDO->_r._f01) = (_v01);				\
									(gpRegVDO->_r._f02) = (_v02);				\
									(gpRegVDO->_r._f03) = (_v03);				\
									(gpRegVDO->_r._f04) = (_v04);				\
									(gpRegVDO->_r._f05) = (_v05);				\
									(gpRegVDO->_r._f06) = (_v06);				\
									(gpRegVDO->_r._f07) = (_v07);				\
									(gpRegVDO->_r._f08) = (_v08);				\
									(gpRegVDO->_r._f09) = (_v09);				\
								} while(0)

#define VDO_Wr10(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10)										\
								do { 											\
									(gpRegVDO->_r._f01) = (_v01);				\
									(gpRegVDO->_r._f02) = (_v02);				\
									(gpRegVDO->_r._f03) = (_v03);				\
									(gpRegVDO->_r._f04) = (_v04);				\
									(gpRegVDO->_r._f05) = (_v05);				\
									(gpRegVDO->_r._f06) = (_v06);				\
									(gpRegVDO->_r._f07) = (_v07);				\
									(gpRegVDO->_r._f08) = (_v08);				\
									(gpRegVDO->_r._f09) = (_v09);				\
									(gpRegVDO->_r._f10) = (_v10);				\
								} while(0)

#define VDO_Wr11(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11)								\
								do { 											\
									(gpRegVDO->_r._f01) = (_v01);				\
									(gpRegVDO->_r._f02) = (_v02);				\
									(gpRegVDO->_r._f03) = (_v03);				\
									(gpRegVDO->_r._f04) = (_v04);				\
									(gpRegVDO->_r._f05) = (_v05);				\
									(gpRegVDO->_r._f06) = (_v06);				\
									(gpRegVDO->_r._f07) = (_v07);				\
									(gpRegVDO->_r._f08) = (_v08);				\
									(gpRegVDO->_r._f09) = (_v09);				\
									(gpRegVDO->_r._f10) = (_v10);				\
									(gpRegVDO->_r._f11) = (_v11);				\
								} while(0)

#define VDO_Wr12(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12)					\
								do { 											\
									(gpRegVDO->_r._f01) = (_v01);				\
									(gpRegVDO->_r._f02) = (_v02);				\
									(gpRegVDO->_r._f03) = (_v03);				\
									(gpRegVDO->_r._f04) = (_v04);				\
									(gpRegVDO->_r._f05) = (_v05);				\
									(gpRegVDO->_r._f06) = (_v06);				\
									(gpRegVDO->_r._f07) = (_v07);				\
									(gpRegVDO->_r._f08) = (_v08);				\
									(gpRegVDO->_r._f09) = (_v09);				\
									(gpRegVDO->_r._f10) = (_v10);				\
									(gpRegVDO->_r._f11) = (_v11);				\
									(gpRegVDO->_r._f12) = (_v12);				\
								} while(0)

#define VDO_Wr13(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13)													\
								do { 											\
									(gpRegVDO->_r._f01) = (_v01);				\
									(gpRegVDO->_r._f02) = (_v02);				\
									(gpRegVDO->_r._f03) = (_v03);				\
									(gpRegVDO->_r._f04) = (_v04);				\
									(gpRegVDO->_r._f05) = (_v05);				\
									(gpRegVDO->_r._f06) = (_v06);				\
									(gpRegVDO->_r._f07) = (_v07);				\
									(gpRegVDO->_r._f08) = (_v08);				\
									(gpRegVDO->_r._f09) = (_v09);				\
									(gpRegVDO->_r._f10) = (_v10);				\
									(gpRegVDO->_r._f11) = (_v11);				\
									(gpRegVDO->_r._f12) = (_v12);				\
									(gpRegVDO->_r._f13) = (_v13);				\
								} while(0)

#define VDO_Wr14(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14)										\
								do { 											\
									(gpRegVDO->_r._f01) = (_v01);				\
									(gpRegVDO->_r._f02) = (_v02);				\
									(gpRegVDO->_r._f03) = (_v03);				\
									(gpRegVDO->_r._f04) = (_v04);				\
									(gpRegVDO->_r._f05) = (_v05);				\
									(gpRegVDO->_r._f06) = (_v06);				\
									(gpRegVDO->_r._f07) = (_v07);				\
									(gpRegVDO->_r._f08) = (_v08);				\
									(gpRegVDO->_r._f09) = (_v09);				\
									(gpRegVDO->_r._f10) = (_v10);				\
									(gpRegVDO->_r._f11) = (_v11);				\
									(gpRegVDO->_r._f12) = (_v12);				\
									(gpRegVDO->_r._f13) = (_v13);				\
									(gpRegVDO->_r._f14) = (_v14);				\
								} while(0)

#define VDO_Wr15(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15)								\
								do { 											\
									(gpRegVDO->_r._f01) = (_v01);				\
									(gpRegVDO->_r._f02) = (_v02);				\
									(gpRegVDO->_r._f03) = (_v03);				\
									(gpRegVDO->_r._f04) = (_v04);				\
									(gpRegVDO->_r._f05) = (_v05);				\
									(gpRegVDO->_r._f06) = (_v06);				\
									(gpRegVDO->_r._f07) = (_v07);				\
									(gpRegVDO->_r._f08) = (_v08);				\
									(gpRegVDO->_r._f09) = (_v09);				\
									(gpRegVDO->_r._f10) = (_v10);				\
									(gpRegVDO->_r._f11) = (_v11);				\
									(gpRegVDO->_r._f12) = (_v12);				\
									(gpRegVDO->_r._f13) = (_v13);				\
									(gpRegVDO->_r._f14) = (_v14);				\
									(gpRegVDO->_r._f15) = (_v15);				\
								} while(0)

#define VDO_Wr16(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15,_f16,_v16)					\
								do { 											\
									(gpRegVDO->_r._f01) = (_v01);				\
									(gpRegVDO->_r._f02) = (_v02);				\
									(gpRegVDO->_r._f03) = (_v03);				\
									(gpRegVDO->_r._f04) = (_v04);				\
									(gpRegVDO->_r._f05) = (_v05);				\
									(gpRegVDO->_r._f06) = (_v06);				\
									(gpRegVDO->_r._f07) = (_v07);				\
									(gpRegVDO->_r._f08) = (_v08);				\
									(gpRegVDO->_r._f09) = (_v09);				\
									(gpRegVDO->_r._f10) = (_v10);				\
									(gpRegVDO->_r._f11) = (_v11);				\
									(gpRegVDO->_r._f12) = (_v12);				\
									(gpRegVDO->_r._f13) = (_v13);				\
									(gpRegVDO->_r._f14) = (_v14);				\
									(gpRegVDO->_r._f15) = (_v15);				\
									(gpRegVDO->_r._f16) = (_v16);				\
								} while(0)

/* Indexed Register Access.
 *
 * There is in-direct field specified by 'index' field within a register.
 * Normally a register has only one meaning for a 'field_name', but indexed register
 * can hold several data for a 'field_name' specifed by 'index' field of indexed register.
 * When writing an 3rd data for given 'field_name' register, you need to set 'rw' = 0, 'index' = 2,
 * and 'load' = 0.
 *
 * ASSUMPTION
 * For Writing indexed register load bit
 *
 * parameter list
 * _r     : name of register
 * _lname : name of load  bit field	: shall be 0 after macro executed.
 * _rwname: name of rw    bit field : shall be 0 after VDO_Wind(), 1 for VDO_Rind()
 * _iname : name of index bit field
 * _ival  : index value
 * _fname : field name
 * _fval  : field variable that field value shall be stored.
 *
 * VDO_Rind : General indexed register Read.(
 * VDO_Wind : General indexed register Read.
 *
 * VDO_Ridx : For 'index', 'rw', 'load' field name
 * VDO_Widx : For 'index', 'rw', 'load' field name and NO_LOAD.
 */
#define VDO_Rind(_r, _lname, _rwname, _iname, _ival, _fname, _fval)				\
							do {												\
								VDO_Wr03(_r,_lname,0,_rwname,1,_iname,_ival);	\
								VDO_WrFL(_r);									\
								VDO_RdFL(_r);									\
								VDO_Rd01(_r,_fname,_fval);						\
							} while (0)

#define VDO_Wind(_r, _lname, _rwname, _iname, _ival, _fname, _fval)				\
				VDO_Wr04(_r, _lname,0, _rwname,0, _iname,_ival, _fname,_fval)


#define VDO_Ridx(_r, _ival, _fname, _fval)	VDO_Rind(_r,load,rw,index,_ival,_fname,_fval)

#define VDO_Widx(_r, _ival, _fname, _fval)	VDO_Wind(_r,load,rw,index,_ival,_fname,_fval)

/** @} *//* end of macro documentation */

#ifdef __cplusplus
}
#endif
#endif	/* if 0 */
#endif	/* _VDO_REG_H_ */

/* from 'vdo.csv' 20111019 10:32:56   대한민국 표준시 by getregs v2.7 */
