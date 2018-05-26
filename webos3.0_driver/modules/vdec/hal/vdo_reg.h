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
 *  author     Youngwoo Jin (youngwoo.jin@lge.com)
 *  version    1.0
 *  date       2012.03.23
 *
 */

#ifndef _VDO_REG_H_
#define _VDO_REG_H_

/*----------------------------------------------------------------------------------------
    Control Constants
----------------------------------------------------------------------------------------*/
#if defined(CHIP_NAME_d13)
#define VDEC_VDO_BASE		(VDEC_BASE + 0x60E00)
#else
#define VDEC_VDO_BASE		(VDEC_BASE + 0x1F00)
#endif

// VDO commands
#define VDO_CMD_PIC_RUN		(0x1<<0)
#define VDO_CMD_PIC_INIT	(0x1<<4)
#define VDO_CMD_FLUSH_RUN	(0x1<<8)
#define VDO_CMD_VDO_EN		(0x1<<28)

// Chroma format
#define VDO_CHROMA_420		(0x4<<0)
#define VDO_CHROMA_422		(0x5<<0)
#define VDO_CHROMA_444		(0x6<<0)

// Scan Mode
#define VDO_SCAN_INTRLC		(0x0<<4)
#define VDO_SCAN_PROG		(0x1<<4)

#define VDO_SCAN_TOP		(0x0<<0)
#define VDO_SCAN_BOT		(0x1<<0)

static UINT32 vdo_reg_offset[4] = {0x00, 0x50, 0xA0, 0xC0};
//static UINT32 vdo_reg_scan_mode[4] = {0x2C, 0x2C, 0x1C, 0x1C};
static UINT32 vdo_reg_vact_term[4] = {0x30, 0x30, 0x10, 0x10};

/////////////////////////////////////////////////////////////////////////////////////////
// VDO Register
// VDEC_VDO_BASE    : 0xF00
// VDO              : 0xF00 - 0xFFC
/////////////////////////////////////////////////////////////////////////////////////////

#define VDO_CH_VBP_NUM(i)                  (*(volatile UINT32 *)(stpVdoReg + vdo_reg_offset[i] + 0x000))
#define VDO_CH_VFP_NUM(i)                  (*(volatile UINT32 *)(stpVdoReg + vdo_reg_offset[i] + 0x004))
#define VDO_CH_VSYNC_NUM(i)                (*(volatile UINT32 *)(stpVdoReg + vdo_reg_offset[i] + 0x008))
#define VDO_CH_VSYNC_CMD(i)                (*(volatile UINT32 *)(stpVdoReg + vdo_reg_offset[i] + 0x00C))
#define VDO_CH_WAIT_REQB(i)                (*(volatile UINT32 *)(stpVdoReg + (i*0x50) + 0x010))
#define VDO_CH_FLUSH_CONF(i)               (*(volatile UINT32 *)(stpVdoReg + (i*0x50) + 0x014))
#define VDO_CH_VDO_CONF(i)                 (*(volatile UINT32 *)(stpVdoReg + (i*0x50) + 0x018))
#define VDO_CH_VDO_CMD(i)                  (*(volatile UINT32 *)(stpVdoReg + (i*0x50) + 0x01C))
#define VDO_CH_PIC_SIZE(i)                 (*(volatile UINT32 *)(stpVdoReg + (i*0x50) + 0x020))
#define VDO_CH_BUF_IDX(i)                  (*(volatile UINT32 *)(stpVdoReg + (i*0x50) + 0x024))
#define VDO_CH_CHROMA_FORMAT(i)            (*(volatile UINT32 *)(stpVdoReg + (i*0x50) + 0x028))
#define VDO_CH_SCAN_MODE(i)                (*(volatile UINT32 *)(stpVdoReg + (i*0x50) + 0x02C))
//#define VDO_CH_SCAN_MODE(i)                (*(volatile UINT32 *)(stpVdoReg + vdo_reg_offset[i] + vdo_reg_scan_mode[i]))
#define VDO_CH_VSYNC_VACT_TERM(i)          (*(volatile UINT32 *)(stpVdoReg + vdo_reg_offset[i] + vdo_reg_vact_term[i]))
#define VDO_CH_LUMA_STALL_CNT(i)           (*(volatile UINT32 *)(stpVdoReg + (i*0x50) + 0x034))
#define VDO_CH_CHMA_STALL_CNT(i)           (*(volatile UINT32 *)(stpVdoReg + (i*0x50) + 0x038))
#define VDO_CH_MIF_LINE_CNT(i)             (*(volatile UINT32 *)(stpVdoReg + (i*0x50) + 0x040))
#define VDO_CH_MIF_STATUS(i)               (*(volatile UINT32 *)(stpVdoReg + (i*0x50) + 0x044))
#define VDO_CH_RDC_LINE_CNT(i)             (*(volatile UINT32 *)(stpVdoReg + (i*0x50) + 0x048))
#define VDO_CH_RDC_STATUS(i)               (*(volatile UINT32 *)(stpVdoReg + (i*0x50) + 0x04C))

#define VDO_VERSION                      (*(volatile UINT32 *)(stpVdoReg + 0x0FC))


/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/


#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------------
	0x0000 ch_vbp_num ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	ch_vbp_num                     :16;	//  0:15
} CH_VBP_NUM;

/*-----------------------------------------------------------------------------
	0x0004 ch_vfp_num ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	ch_vfp_num                     :16;	//  0:15
} CH_VFP_NUM;

/*-----------------------------------------------------------------------------
	0x0008 ch_vsync_num ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	ch_vsync_num                   :16;	//  0:15
} CH_VSYNC_NUM;

/*-----------------------------------------------------------------------------
	0x000c ch_vsync_cmd ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	vsync                           : 1,	//     0
	                                : 3,	//  1: 3 reserved
	vactv                           : 1,	//     4
	                                : 3,	//  5: 7 reserved
	vsync_mode                      : 1,	//     8
	vactv_mode                      : 1,	//     9
	vactv_auto                      : 1,	//    10
	                                : 5,	// 11:15 reserved
	vsync_sel                       : 2;	// 16:17
} CH_VSYNC_CMD;

/*-----------------------------------------------------------------------------
	0x0010 ch_wait_reqb ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reqb_wait_cycle                 : 8,	//  0: 7
	                                :20,	//  8:27 reserved
	reqb_wait_en                    : 1;	//    28
} CH_WAIT_REQB;

/*-----------------------------------------------------------------------------
	0x0014 ch_flush_conf ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	flush_idle_cycle                : 8,	//  0: 7
	                                :20,	//  8:27 reserved
	flush_on                        : 1,	//    28
	flush_idle_en                   : 1;	//    29
} CH_FLUSH_CONF;

/*-----------------------------------------------------------------------------
	0x0018 ch_vdo_conf ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 8,	//  0: 7 reserved
	channel_ready_mode              : 1,	//     8
	                                : 3,	//  9:11 reserved
	gdi_dma_endian                  : 2,	// 12:13
	                                : 1,	//    14 reserved
	reqb_dimension                  : 1,	//    15
	line_wait_cycle                 :16;	// 16:31
} CH_VDO_CONF;

/*-----------------------------------------------------------------------------
	0x001c ch_vdo_cmd ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pic_run                         : 1,	//     0
	                                : 3,	//  1: 3 reserved
	pic_init                        : 1,	//     4
	                                : 3,	//  5: 7 reserved
	flush_run                       : 1,	//     8
	                                :19,	//  9:27 reserved
	vdo_en                          : 1;	//    28
} CH_VDO_CMD;

/*-----------------------------------------------------------------------------
	0x0020 ch_pic_size ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pic_height                      :12,	//  0:11
	                                : 4,	// 12:15 reserved
	pic_width                       :12;	// 16:27
} CH_PIC_SIZE;

/*-----------------------------------------------------------------------------
	0x0024 ch_buf_idx ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	buf_idx                         : 6;	//  0: 5
} CH_BUF_IDX;

/*-----------------------------------------------------------------------------
	0x0028 ch_chroma_format ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	cformat                         : 3;	//  0: 2
} CH_CHROMA_FORMAT;

/*-----------------------------------------------------------------------------
	0x002c ch_scan_mode ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	field                           : 1,	//     0
	                                : 3,	//  1: 3 reserved
	prog_scan                       : 1;	//     4
} CH_SCAN_MODE;

/*-----------------------------------------------------------------------------
	0x0030 ch_vsync_vact_term ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	vsync_vactv_term                :31,	//  0:30
	mont_en                         : 1;	//    31
} CH_VSYNC_VACT_TERM;

/*-----------------------------------------------------------------------------
	0x0034 ch_luma_stall_cnt ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	luma_stall_cnt                  :24;	//  0:23
} CH_LUMA_STALL_CNT;

/*-----------------------------------------------------------------------------
	0x0038 ch_chma_stall_cnt ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	chma_stall_cnt                  :24;	//  0:23
} CH_CHMA_STALL_CNT;

/*-----------------------------------------------------------------------------
	0x0040 ch_mif_line_cnt ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reqb_ypos                       :12;	//  0:11
} CH_MIF_LINE_CNT;

/*-----------------------------------------------------------------------------
	0x0044 ch_mif_status ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	s0_curr_stat                    : 5,	//  0: 4
	                                : 3,	//  5: 7 reserved
	s1_curr_stat                    : 3;	//  8:10
} CH_MIF_STATUS;

/*-----------------------------------------------------------------------------
	0x0048 ch_rdc_line_cnt ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	line_cnt                        :12;	//  0:11
} CH_RDC_LINE_CNT;

/*-----------------------------------------------------------------------------
	0x004c ch_rdc_status ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	s0_curr_stat                    : 4,	//  0: 3
	s1_curr_stat                    : 4,	//  4: 7
	s2_curr_stat                    : 4,	//  8:11
	time_out_err                    : 1,	//    12
	                                : 3,	// 13:15 reserved
	pic_done                        : 1,	//    16
	pic_read_done                   : 1;	//    17
} CH_RDC_STATUS;


/*-----------------------------------------------------------------------------
	0x0000 ch0_vbp_num ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	ch0_vbp_num                     :16;	//  0:15
} CH0_VBP_NUM;

/*-----------------------------------------------------------------------------
	0x0004 ch0_vfp_num ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	ch0_vfp_num                     :16;	//  0:15
} CH0_VFP_NUM;

/*-----------------------------------------------------------------------------
	0x0008 ch0_vsync_num ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	ch0_vsync_num                   :16;	//  0:15
} CH0_VSYNC_NUM;

/*-----------------------------------------------------------------------------
	0x000c ch0_vsync_cmd ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	vsync                           : 1,	//     0
	                                : 3,	//  1: 3 reserved
	vactv                           : 1,	//     4
	                                : 3,	//  5: 7 reserved
	vsync_mode                      : 1,	//     8
	vactv_mode                      : 1,	//     9
	vactv_auto                      : 1,	//    10
	                                : 5,	// 11:15 reserved
	vsync_sel                       : 2;	// 16:17
} CH0_VSYNC_CMD;

/*-----------------------------------------------------------------------------
	0x0010 ch0_wait_reqb ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reqb_wait_cycle                 : 8,	//  0: 7
	                                :20,	//  8:27 reserved
	reqb_wait_en                    : 1;	//    28
} CH0_WAIT_REQB;

/*-----------------------------------------------------------------------------
	0x0014 ch0_flush_conf ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	flush_idle_cycle                : 8,	//  0: 7
	                                :20,	//  8:27 reserved
	flush_on                        : 1,	//    28
	flush_idle_en                   : 1;	//    29
} CH0_FLUSH_CONF;

/*-----------------------------------------------------------------------------
	0x0018 ch0_vdo_conf ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 8,	//  0: 7 reserved
	channel_ready_mode              : 1,	//     8
	                                : 3,	//  9:11 reserved
	gdi_dma_endian                  : 2,	// 12:13
	                                : 1,	//    14 reserved
	reqb_dimension                  : 1,	//    15
	line_wait_cycle                 :16;	// 16:31
} CH0_VDO_CONF;

/*-----------------------------------------------------------------------------
	0x001c ch0_vdo_cmd ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pic_run                         : 1,	//     0
	                                : 3,	//  1: 3 reserved
	pic_init                        : 1,	//     4
	                                : 3,	//  5: 7 reserved
	flush_run                       : 1,	//     8
	                                :19,	//  9:27 reserved
	vdo_en                          : 1;	//    28
} CH0_VDO_CMD;

/*-----------------------------------------------------------------------------
	0x0020 ch0_pic_size ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pic_height                      :12,	//  0:11
	                                : 4,	// 12:15 reserved
	pic_width                       :12;	// 16:27
} CH0_PIC_SIZE;

/*-----------------------------------------------------------------------------
	0x0024 ch0_buf_idx ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	buf_idx                         : 6;	//  0: 5
} CH0_BUF_IDX;

/*-----------------------------------------------------------------------------
	0x0028 ch0_chroma_format ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	cformat                         : 3;	//  0: 2
} CH0_CHROMA_FORMAT;

/*-----------------------------------------------------------------------------
	0x002c ch0_scan_mode ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	field                           : 1,	//     0
	                                : 3,	//  1: 3 reserved
	prog_scan                       : 1;	//     4
} CH0_SCAN_MODE;

/*-----------------------------------------------------------------------------
	0x0030 ch0_vsync_vact_term ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	vsync_vactv_term                :31,	//  0:30
	mont_en                         : 1;	//    31
} CH0_VSYNC_VACT_TERM;

/*-----------------------------------------------------------------------------
	0x0034 ch0_luma_stall_cnt ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	luma_stall_cnt                  :24;	//  0:23
} CH0_LUMA_STALL_CNT;

/*-----------------------------------------------------------------------------
	0x0038 ch0_chma_stall_cnt ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	chma_stall_cnt                  :24;	//  0:23
} CH0_CHMA_STALL_CNT;

/*-----------------------------------------------------------------------------
	0x0040 ch0_mif_line_cnt ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reqb_ypos                       :12;	//  0:11
} CH0_MIF_LINE_CNT;

/*-----------------------------------------------------------------------------
	0x0044 ch0_mif_status ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	s0_curr_stat                    : 5,	//  0: 4
	                                : 3,	//  5: 7 reserved
	s1_curr_stat                    : 3;	//  8:10
} CH0_MIF_STATUS;

/*-----------------------------------------------------------------------------
	0x0048 ch0_rdc_line_cnt ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	line_cnt                        :12;	//  0:11
} CH0_RDC_LINE_CNT;

/*-----------------------------------------------------------------------------
	0x004c ch0_rdc_status ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	s0_curr_stat                    : 4,	//  0: 3
	s1_curr_stat                    : 4,	//  4: 7
	s2_curr_stat                    : 4,	//  8:11
	time_out_err                    : 1,	//    12
	                                : 3,	// 13:15 reserved
	pic_done                        : 1,	//    16
	pic_read_done                   : 1;	//    17
} CH0_RDC_STATUS;

/*-----------------------------------------------------------------------------
	0x0050 ch1_vbp_num ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	ch0_vbp_num                     :16;	//  0:15
} CH1_VBP_NUM;

/*-----------------------------------------------------------------------------
	0x0054 ch1_vfp_num ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	ch0_vfp_num                     :16;	//  0:15
} CH1_VFP_NUM;

/*-----------------------------------------------------------------------------
	0x0058 ch1_vsync_num ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	ch0_vsync_num                   :16;	//  0:15
} CH1_VSYNC_NUM;

/*-----------------------------------------------------------------------------
	0x005c ch1_vsync_cmd ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	vsync                           : 1,	//     0
	                                : 3,	//  1: 3 reserved
	vactv                           : 1,	//     4
	                                : 3,	//  5: 7 reserved
	vsync_mode                      : 1,	//     8
	vactv_mode                      : 1,	//     9
	vactv_auto                      : 1,	//    10
	                                : 5,	// 11:15 reserved
	vsync_sel                       : 2;	// 16:17
} CH1_VSYNC_CMD;

/*-----------------------------------------------------------------------------
	0x0060 ch1_wait_reqb ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reqb_wait_cycle                 : 8,	//  0: 7
	                                :20,	//  8:27 reserved
	reqb_wait_en                    : 1;	//    28
} CH1_WAIT_REQB;

/*-----------------------------------------------------------------------------
	0x0064 ch1_flush_conf ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	flush_idle_cycle                : 8,	//  0: 7
	                                :20,	//  8:27 reserved
	flush_on                        : 1,	//    28
	flush_idle_en                   : 1;	//    29
} CH1_FLUSH_CONF;

/*-----------------------------------------------------------------------------
	0x0068 ch1_vdo_conf ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 8,	//  0: 7 reserved
	channel_ready_mode              : 1,	//     8
	                                : 3,	//  9:11 reserved
	gdi_dma_endian                  : 2,	// 12:13
	                                : 1,	//    14 reserved
	reqb_dimension                  : 1,	//    15
	line_wait_cycle                 :16;	// 16:31
} CH1_VDO_CONF;

/*-----------------------------------------------------------------------------
	0x006c ch1_vdo_cmd ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pic_run                         : 1,	//     0
	                                : 3,	//  1: 3 reserved
	pic_init                        : 1,	//     4
	                                : 3,	//  5: 7 reserved
	flush_run                       : 1,	//     8
	                                :19,	//  9:27 reserved
	vdo_en                          : 1;	//    28
} CH1_VDO_CMD;

/*-----------------------------------------------------------------------------
	0x0070 ch1_pic_size ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pic_height                      :12,	//  0:11
	                                : 4,	// 12:15 reserved
	pic_width                       :12;	// 16:27
} CH1_PIC_SIZE;

/*-----------------------------------------------------------------------------
	0x0074 ch1_buf_idx ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	buf_idx                         : 6;	//  0: 5
} CH1_BUF_IDX;

/*-----------------------------------------------------------------------------
	0x0078 ch1_chroma_format ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	cformat                         : 3;	//  0: 2
} CH1_CHROMA_FORMAT;

/*-----------------------------------------------------------------------------
	0x007c ch1_scan_mode ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	field                           : 1,	//     0
	                                : 3,	//  1: 3 reserved
	prog_scan                       : 1;	//     4
} CH1_SCAN_MODE;

/*-----------------------------------------------------------------------------
	0x0080 ch1_vsync_vact_term ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	vsync_vactv_term                :31,	//  0:30
	mont_en                         : 1;	//    31
} CH1_VSYNC_VACT_TERM;

/*-----------------------------------------------------------------------------
	0x0084 ch1_luma_stall_cnt ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	luma_stall_cnt                  :24;	//  0:23
} CH1_LUMA_STALL_CNT;

/*-----------------------------------------------------------------------------
	0x0088 ch1_chma_stall_cnt ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	chma_stall_cnt                  :24;	//  0:23
} CH1_CHMA_STALL_CNT;

/*-----------------------------------------------------------------------------
	0x0090 ch1_mif_line_cnt ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reqb_ypos                       :12;	//  0:11
} CH1_MIF_LINE_CNT;

/*-----------------------------------------------------------------------------
	0x0094 ch1_mif_status ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	s0_curr_stat                    : 5,	//  0: 4
	                                : 3,	//  5: 7 reserved
	s1_curr_stat                    : 3;	//  8:10
} CH1_MIF_STATUS;

/*-----------------------------------------------------------------------------
	0x0098 ch1_rdc_line_cnt ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	line_cnt                        :12;	//  0:11
} CH1_RDC_LINE_CNT;

/*-----------------------------------------------------------------------------
	0x009c ch1_rdc_status ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	s0_curr_stat                    : 4,	//  0: 3
	s1_curr_stat                    : 4,	//  4: 7
	s2_curr_stat                    : 4,	//  8:11
	time_out_err                    : 1,	//    12
	                                : 3,	// 13:15 reserved
	pic_done                        : 1,	//    16
	pic_read_done                   : 1;	//    17
} CH1_RDC_STATUS;

/*-----------------------------------------------------------------------------
	0x00a0 ch2_vbp_num ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	ch0_vbp_num                     :16;	//  0:15
} CH2_VBP_NUM;

/*-----------------------------------------------------------------------------
	0x00a4 ch2_vfp_num ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	ch0_vfp_num                     :16;	//  0:15
} CH2_VFP_NUM;

/*-----------------------------------------------------------------------------
	0x00a8 ch2_vsync_num ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	ch0_vsync_num                   :16;	//  0:15
} CH2_VSYNC_NUM;

/*-----------------------------------------------------------------------------
	0x00ac ch2_vsync_cmd ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	vsync                           : 1,	//     0
	                                : 3,	//  1: 3 reserved
	vactv                           : 1,	//     4
	                                : 3,	//  5: 7 reserved
	vsync_mode                      : 1,	//     8
	vactv_mode                      : 1,	//     9
	vactv_auto                      : 1,	//    10
	                                : 5,	// 11:15 reserved
	vsync_sel                       : 2;	// 16:17
} CH2_VSYNC_CMD;

/*-----------------------------------------------------------------------------
	0x00b0 ch2_vsync_vact_term ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	vsync_vactv_term                :31,	//  0:30
	mont_en                         : 1;	//    31
} CH2_VSYNC_VACT_TERM;

/*-----------------------------------------------------------------------------
	0x00bc ch2_scan_mode ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 4,	//  0: 3 reserved
	prog_scan                       : 1;	//     4
} CH2_SCAN_MODE;

/*-----------------------------------------------------------------------------
	0x00c0 ch3_vbp_num ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	ch0_vbp_num                     :16;	//  0:15
} CH3_VBP_NUM;

/*-----------------------------------------------------------------------------
	0x00c4 ch3_vfp_num ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	ch0_vfp_num                     :16;	//  0:15
} CH3_VFP_NUM;

/*-----------------------------------------------------------------------------
	0x00c8 ch3_vsync_num ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	ch0_vsync_num                   :16;	//  0:15
} CH3_VSYNC_NUM;

/*-----------------------------------------------------------------------------
	0x00cc ch3_vsync_cmd ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	vsync                           : 1,	//     0
	                                : 3,	//  1: 3 reserved
	vactv                           : 1,	//     4
	                                : 3,	//  5: 7 reserved
	vsync_mode                      : 1,	//     8
	vactv_mode                      : 1,	//     9
	vactv_auto                      : 1,	//    10
	                                : 5,	// 11:15 reserved
	vsync_sel                       : 2;	// 16:17
} CH3_VSYNC_CMD;

/*-----------------------------------------------------------------------------
	0x00d0 ch3_vsync_vact_term ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	vsync_vactv_term                :31,	//  0:30
	mont_en                         : 1;	//    31
} CH3_VSYNC_VACT_TERM;

/*-----------------------------------------------------------------------------
	0x00dc ch3_scan_mode ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 4,	//  0: 3 reserved
	prog_scan                       : 1;	//     4
} CH3_SCAN_MODE;

/*-----------------------------------------------------------------------------
	0x00fc version ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	yyyymmdd                        ;   	// 31: 0
} VERSION;

typedef struct {
	CH_VBP_NUM                      	ch_vbp_num                      ;	// 0x0000 : ''
	CH_VFP_NUM                      	ch_vfp_num                      ;	// 0x0004 : ''
	CH_VSYNC_NUM                    	ch_vsync_num                    ;	// 0x0008 : ''
	CH_VSYNC_CMD                    	ch_vsync_cmd                    ;	// 0x000c : ''
	CH_WAIT_REQB                    	ch_wait_reqb                    ;	// 0x0010 : ''
	CH_FLUSH_CONF                   	ch_flush_conf                   ;	// 0x0014 : ''
	CH_VDO_CONF                     	ch_vdo_conf                     ;	// 0x0018 : ''
	CH_VDO_CMD                      	ch_vdo_cmd                      ;	// 0x001c : ''
	CH_PIC_SIZE                     	ch_pic_size                     ;	// 0x0020 : ''
	CH_BUF_IDX                      	ch_buf_idx                      ;	// 0x0024 : ''
	CH_CHROMA_FORMAT                	ch_chroma_format                ;	// 0x0028 : ''
	CH_SCAN_MODE                    	ch_scan_mode                    ;	// 0x002c : ''
	CH_VSYNC_VACT_TERM              	ch_vsync_vact_term              ;	// 0x0030 : ''
	CH_LUMA_STALL_CNT               	ch_luma_stall_cnt               ;	// 0x0034 : ''
	CH_CHMA_STALL_CNT               	ch_chma_stall_cnt               ;	// 0x0038 : ''
	UINT32                          	                 __rsvd_00[   1];	// 0x003c
	CH_MIF_LINE_CNT                 	ch_mif_line_cnt                 ;	// 0x0040 : ''
	CH_MIF_STATUS                   	ch_mif_status                   ;	// 0x0044 : ''
	CH_RDC_LINE_CNT                 	ch_rdc_line_cnt                 ;	// 0x0048 : ''
	CH_RDC_STATUS                   	ch_rdc_status                   ;	// 0x004c : ''
} MVI_T_CH;

typedef struct {
	CH_VBP_NUM                      	ch_vbp_num                      ;	// 0x00a0 : ''
	CH_VFP_NUM                      	ch_vfp_num                      ;	// 0x00a4 : ''
	CH_VSYNC_NUM                    	ch_vsync_num                    ;	// 0x00a8 : ''
	CH_VSYNC_CMD                    	ch_vsync_cmd                    ;	// 0x00ac : ''
	CH_VSYNC_VACT_TERM              	ch_vsync_vact_term              ;	// 0x00b0 : ''
	UINT32                          	                 __rsvd_02[   2];	// 0x00b4 ~ 0x00b8
	CH_SCAN_MODE                    	ch_scan_mode                    ;	// 0x00bc : ''
} MVI_R_CH;

typedef struct {
	MVI_T_CH                        	mvi_t_ch[2]                     ;
	MVI_R_CH                        	mvi_r_ch[2]                     ;
/*	CH0_VBP_NUM                     	ch0_vbp_num                     ;	// 0x0000 : ''
	CH0_VFP_NUM                     	ch0_vfp_num                     ;	// 0x0004 : ''
	CH0_VSYNC_NUM                   	ch0_vsync_num                   ;	// 0x0008 : ''
	CH0_VSYNC_CMD                   	ch0_vsync_cmd                   ;	// 0x000c : ''
	CH0_WAIT_REQB                   	ch0_wait_reqb                   ;	// 0x0010 : ''
	CH0_FLUSH_CONF                  	ch0_flush_conf                  ;	// 0x0014 : ''
	CH0_VDO_CONF                    	ch0_vdo_conf                    ;	// 0x0018 : ''
	CH0_VDO_CMD                     	ch0_vdo_cmd                     ;	// 0x001c : ''
	CH0_PIC_SIZE                    	ch0_pic_size                    ;	// 0x0020 : ''
	CH0_BUF_IDX                     	ch0_buf_idx                     ;	// 0x0024 : ''
	CH0_CHROMA_FORMAT               	ch0_chroma_format               ;	// 0x0028 : ''
	CH0_SCAN_MODE                   	ch0_scan_mode                   ;	// 0x002c : ''
	CH0_VSYNC_VACT_TERM             	ch0_vsync_vact_term             ;	// 0x0030 : ''
	CH0_LUMA_STALL_CNT              	ch0_luma_stall_cnt              ;	// 0x0034 : ''
	CH0_CHMA_STALL_CNT              	ch0_chma_stall_cnt              ;	// 0x0038 : ''
	UINT32                          	                 __rsvd_00[   1];	// 0x003c
	CH0_MIF_LINE_CNT                	ch0_mif_line_cnt                ;	// 0x0040 : ''
	CH0_MIF_STATUS                  	ch0_mif_status                  ;	// 0x0044 : ''
	CH0_RDC_LINE_CNT                	ch0_rdc_line_cnt                ;	// 0x0048 : ''
	CH0_RDC_STATUS                  	ch0_rdc_status                  ;	// 0x004c : ''
	CH1_VBP_NUM                     	ch1_vbp_num                     ;	// 0x0050 : ''
	CH1_VFP_NUM                     	ch1_vfp_num                     ;	// 0x0054 : ''
	CH1_VSYNC_NUM                   	ch1_vsync_num                   ;	// 0x0058 : ''
	CH1_VSYNC_CMD                   	ch1_vsync_cmd                   ;	// 0x005c : ''
	CH1_WAIT_REQB                   	ch1_wait_reqb                   ;	// 0x0060 : ''
	CH1_FLUSH_CONF                  	ch1_flush_conf                  ;	// 0x0064 : ''
	CH1_VDO_CONF                    	ch1_vdo_conf                    ;	// 0x0068 : ''
	CH1_VDO_CMD                     	ch1_vdo_cmd                     ;	// 0x006c : ''
	CH1_PIC_SIZE                    	ch1_pic_size                    ;	// 0x0070 : ''
	CH1_BUF_IDX                     	ch1_buf_idx                     ;	// 0x0074 : ''
	CH1_CHROMA_FORMAT               	ch1_chroma_format               ;	// 0x0078 : ''
	CH1_SCAN_MODE                   	ch1_scan_mode                   ;	// 0x007c : ''
	CH1_VSYNC_VACT_TERM             	ch1_vsync_vact_term             ;	// 0x0080 : ''
	CH1_LUMA_STALL_CNT              	ch1_luma_stall_cnt              ;	// 0x0084 : ''
	CH1_CHMA_STALL_CNT              	ch1_chma_stall_cnt              ;	// 0x0088 : ''
	UINT32                          	                 __rsvd_01[   1];	// 0x008c
	CH1_MIF_LINE_CNT                	ch1_mif_line_cnt                ;	// 0x0090 : ''
	CH1_MIF_STATUS                  	ch1_mif_status                  ;	// 0x0094 : ''
	CH1_RDC_LINE_CNT                	ch1_rdc_line_cnt                ;	// 0x0098 : ''
	CH1_RDC_STATUS                  	ch1_rdc_status                  ;	// 0x009c : ''
	CH2_VBP_NUM                     	ch2_vbp_num                     ;	// 0x00a0 : ''
	CH2_VFP_NUM                     	ch2_vfp_num                     ;	// 0x00a4 : ''
	CH2_VSYNC_NUM                   	ch2_vsync_num                   ;	// 0x00a8 : ''
	CH2_VSYNC_CMD                   	ch2_vsync_cmd                   ;	// 0x00ac : ''
	CH2_VSYNC_VACT_TERM             	ch2_vsync_vact_term             ;	// 0x00b0 : ''
	UINT32                          	                 __rsvd_02[   2];	// 0x00b4 ~ 0x00b8
	CH2_SCAN_MODE                   	ch2_scan_mode                   ;	// 0x00bc : ''
	CH3_VBP_NUM                     	ch3_vbp_num                     ;	// 0x00c0 : ''
	CH3_VFP_NUM                     	ch3_vfp_num                     ;	// 0x00c4 : ''
	CH3_VSYNC_NUM                   	ch3_vsync_num                   ;	// 0x00c8 : ''
	CH3_VSYNC_CMD                   	ch3_vsync_cmd                   ;	// 0x00cc : ''
	CH3_VSYNC_VACT_TERM             	ch3_vsync_vact_term             ;	// 0x00d0 : ''
	UINT32                          	                 __rsvd_03[   2];	// 0x00d4 ~ 0x00d8
	CH3_SCAN_MODE                   	ch3_scan_mode                   ;	// 0x00dc : ''
*/	UINT32                          	                 __rsvd_04[   7];	// 0x00e0 ~ 0x00f8
	VERSION                         	version                         ;	// 0x00fc : ''
} VDO_REG_T;
/* 51 regs, 51 types */

/* 52 regs, 52 types in Total*/

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

#endif	/* _VDO_REG_H_ */

/* from 'vdo-REG.csv' 20120326 19:08:42   대한민국 표준시 by getregs v2.7 */
