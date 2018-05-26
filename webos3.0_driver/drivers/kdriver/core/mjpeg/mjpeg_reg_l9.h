/*
SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
Copyright(c) 1998-2013 by LG Electronics Inc.
 
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
 *  mjpeg register details. ( used only within kdriver )
 *
 *  author     youngwoo.jin (youngwoo.jin@lge.com)
 *  version    1.0
 *  date       2010.04.09
 *
 */

#ifndef _MJPEG_REG_H_
#define _MJPEG_REG_H_

/*-----------------------------------------------------------------------------
    Control Constants
-----------------------------------------------------------------------------*/
#define	MJPEG_VER_1_3_0

/*-----------------------------------------------------------------------------
    File Inclusions
-----------------------------------------------------------------------------*/


#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------------
	0x0000 pic_start ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
#ifdef MJPEG_VER_1_3_0
	pic_start						: 1,	//     0
	init_status						: 1,	//     1
	pic_stop						: 1,	//     2
	partial_start					: 1;	//     3
#else
	pic_start                       : 1;	//     0
#endif
} PIC_START;

/*-----------------------------------------------------------------------------
	0x0004 pic_status ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
#ifdef MJPEG_VER_1_3_0
	jpeg_done						: 1,	//     0
	jpeg_error						: 1,	//     1
	bbc_interrupt					: 1,	//     2
	jpeg_overflow					: 1,	//     3
	partial_buffer0_status			: 1,	//     4
	partial_buffer1_status			: 1,	//     5
	partial_buffer2_status			: 1,	//     6
	partial_buffer3_status			: 1,	//     7
	jpeg_stop						: 1;	//     8
#else
	jpeg_done                       : 1,	//     0
	jpeg_error                      : 1,	//     1
	bbc_interrupt                   : 1,	//     2
	jpeg_overflow                   : 1;	//     3
#endif
} PIC_STATUS;

/*-----------------------------------------------------------------------------
	0x0008 pic_errmb ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	error_mcu_position_y            :12,	//  0:11
	error_mcu_position_x            :12,	// 12:23
	error_restart_index             : 4;	// 24:27
} PIC_ERRMB;

/*-----------------------------------------------------------------------------
	0x000c pic_setmb ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	mcu_position_y                  :12,	//  0:11
	mcu_position_x                  :16;	// 12:27
} PIC_SETMB;

/*-----------------------------------------------------------------------------
	0x0010 pic_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	partial_mode                    : 2,	//  0: 1
	raw_image                       : 1,	//     2
	encoder_mode                    : 1,	//     3
	tc_direction                    : 1,	//     4
	                                : 1,	//     5 reserved
	user_huffman                    : 1,	//     6
	dc_huffman_index                : 3,	//  7: 9
	ac_huffman_index                : 3;	// 10:12
} PIC_CTRL;

/*-----------------------------------------------------------------------------
	0x0014 pic_size ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	vertical                        :16,	//  0:15
	horizontal                      :16;	// 16:31
} PIC_SIZE;

/*-----------------------------------------------------------------------------
	0x0018 mcu_info ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	sampling_factor_2               : 4,	//  0: 3
	sampling_factor_1               : 4,	//  4: 7
	sampling_factor_0               : 4,	//  8:11
	number_of_components            : 3,	// 12:14
	                                : 1,	//    15 reserved
	total_block_number              : 4;	// 16:19
} MCU_INFO;

/*-----------------------------------------------------------------------------
	0x001c rot_info ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	rotation_degree                 : 2,	//  0: 1
	vertical_mirroring              : 1,	//     2
	horitontal_mirroring            : 1,	//     3
	rotation_mirroring_info         : 1;	//     4
} ROT_INFO;

/*-----------------------------------------------------------------------------
	0x0020 scl_info ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	vertical_scale                  : 2,	//  0: 1
	horizontal_scale                : 2,	//  2: 3
	scale_enable                    : 1;	//     4
} SCL_INFO;

/*-----------------------------------------------------------------------------
	0x0024 if_info ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	display_if_clear                : 1,	//     0
	sensor_if_clear                 : 1;	//     1
} IF_INFO;

/*-----------------------------------------------------------------------------
	0x002c op_info ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	number_of_request               : 3,	//  0: 2
	number_of_partial_buffer        : 3,	//  3: 5
	                                :10,	//  6:15 reserved
	number_of_line_in_partial_buffer:16;	// 16:31
} OP_INFO;

/*-----------------------------------------------------------------------------
	0x0030 dpb_config ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	uv_interleave_mode              : 1,	//     0
	endianess                       : 2;	//  1: 2
} DPB_CONFIG;

/*-----------------------------------------------------------------------------
	0x0034 dpb_base00 ''
-----------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x0038 dpb_base01 ''
-----------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x003c dpb_base02 ''
-----------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x0040 dpb_base10 ''
-----------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x0044 dpb_base11 ''
-----------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x0048 dpb_base12 ''
-----------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x004c dpb_base20 ''
-----------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x0050 dpb_base21 ''
-----------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x0054 dpb_base22 ''
-----------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x0058 dpb_base30 ''
-----------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x005c dpb_base31 ''
-----------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x0060 dpb_base32 ''
-----------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x0064 dpb_ystride ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dpb_ystride                     :16;	//  0:15
} DPB_YSTRIDE;

/*-----------------------------------------------------------------------------
	0x0068 dpb_cstride ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dpb_cstride                     :16;	//  0:15
} DPB_CSTRIDE;

/*-----------------------------------------------------------------------------
	0x0080 huff_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	mode                            : 1,	//     0
	ctrl_auto                       : 1,	//     1
	                                : 8,	//  2: 9 reserved
	table_destination               : 2;	// 10:11
} HUFF_CTRL;

/*-----------------------------------------------------------------------------
	0x0084 huff_addr ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	base_addr                       :10,	//  0: 9
	table_destination               : 2;	// 10:11
} HUFF_ADDR;

/*-----------------------------------------------------------------------------
	0x0088 huff_data ''
-----------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x0090 qmat_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	mode                            : 1,	//     0
	ctrl_auto                       : 1,	//     1
	                                : 4,	//  2: 5 reserved
	table_destination               : 2;	//  6: 7
} QMAT_CTRL;

/*-----------------------------------------------------------------------------
	0x0094 qmat_addr ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	base_addr                       : 6,	//  0: 5
	table_destination               : 2;	//  6: 7
} QMAT_ADDR;

/*-----------------------------------------------------------------------------
	0x0098 qmat_data ''
-----------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x00a0 coef_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	mode                            : 1,	//     0
	ctrl_auto                       : 1,	//     1
	                                : 4,	//  2: 5 reserved
	memory_destination              : 3;	//  6: 8
} COEF_CTRL;

/*-----------------------------------------------------------------------------
	0x00a4 coef_addr ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	base_addr                       : 6,	//  0: 5
	addr_memory_destination         : 3;	//  6: 8
} COEF_ADDR;

/*-----------------------------------------------------------------------------
	0x00a8 coef_data ''
-----------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x00b0 rst_interval ''
-----------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x00b4 rst_index ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	rst_index                       :26;	//  0:25
} RST_INDEX;

/*-----------------------------------------------------------------------------
	0x00b8 rst_count ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	rst_count                       :26;	//  0:25
} RST_COUNT;

/*-----------------------------------------------------------------------------
	0x00c0 intr_mask ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
#ifdef MJPEG_VER_1_3_0
	jpeg_done						: 1,	//	   0
	jpeg_error						: 1,	//	   1
	bbc_interrupt					: 1,	//	   2
	jpeg_overflow					: 1,	//	   3
	partial_buffer0					: 1,	//	   4
	partial_buffer1					: 1,	//	   5
	partial_buffer2					: 1,	//	   6
	partial_buffer3					: 1,	//	   7
	jpeg_stop						: 1;	//	   8
#else
	jpeg_done                       : 1,	//     0
	jpeg_error                      : 1,	//     1
	bbc_interrupt                   : 1;	//     2
#endif
} INTR_MASK;

/*-----------------------------------------------------------------------------
	0x00c8 cycle_info ''
-----------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x00f0 dpcm_diff_y ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dpcm_diff_y                     :12;	//  0:11
} DPCM_DIFF_Y;

/*-----------------------------------------------------------------------------
	0x00f4 dpcm_diff_cb ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dpcm_diff_cb                    :12;	//  0:11
} DPCM_DIFF_CB;

/*-----------------------------------------------------------------------------
	0x00f8 dpcm_diff_cr ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dpcm_diff_cr                    :12;	//  0:11
} DPCM_DIFF_CR;

/*-----------------------------------------------------------------------------
	0x0100 gbu_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 1,	//     0 reserved
	initialize_ff_emulation_modulef : 1,	//     1
	initialize_gbu                  : 1;	//     2
} GBU_CTRL;

/*-----------------------------------------------------------------------------
	0x0110 gbu_bt_ptr ''
-----------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x0114 gbu_wd_ptr ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	gbu_wd_ptr                      : 8;	//  0: 7
} GBU_WD_PTR;

/*-----------------------------------------------------------------------------
	0x0118 gbu_tt_cnt_low ''
-----------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x011c gbu_tt_cnt_high ''
-----------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x0140 gbu_bbsr ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	gbu_bbsr                        : 8;	//  0: 7
} GBU_BBSR;

/*-----------------------------------------------------------------------------
	0x0144 gbu_bber ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	gbu_bber                        : 8;	//  0: 7
} GBU_BBER;

/*-----------------------------------------------------------------------------
	0x0148 gbu_bbir ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	gbu_bbir                        : 8;	//  0: 7
} GBU_BBIR;

/*-----------------------------------------------------------------------------
	0x014c gbu_bbhr ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	gbu_bbhr                        : 8;	//  0: 7
} GBU_BBHR;

/*-----------------------------------------------------------------------------
	0x0158 gbu_bcnt ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	gbu_bcnt                        :16;	//  0:15
} GBU_BCNT;

/*-----------------------------------------------------------------------------
	0x0160 gbu_ff_rptr ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	gbu_ff_rptr                     : 7;	//  0: 6
} GBU_FF_RPTR;

/*-----------------------------------------------------------------------------
	0x0164 gbu_ff_wptr ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	gbu_ff_wptr                     : 7;	//  0: 6
} GBU_FF_WPTR;

/*-----------------------------------------------------------------------------
	0x0208 bbc_endaddr ''
-----------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x020c bbc_wr_ptr ''
-----------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x0210 bbc_rd_ptr ''
-----------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x0214 bbc_ext_addr ''
-----------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x0218 bbc_int_addr ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	bbc_int_addr                    : 7;	//  0: 6
} BBC_INT_ADDR;

/*-----------------------------------------------------------------------------
	0x021c bbc_data_cnt ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	bbc_data_cnt                    : 8;	//  0: 7
} BBC_DATA_CNT;

/*-----------------------------------------------------------------------------
	0x0220 bbc_command ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	command                         : 1,	//     0
	endianess                       : 2;	//  1: 2
} BBC_COMMAND;

/*-----------------------------------------------------------------------------
	0x0224 bbc_busy ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	bbc_busy                        : 1;	//     0
} BBC_BUSY;

/*-----------------------------------------------------------------------------
	0x0228 bbc_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	auto_run                        : 1,	//     0
	endianess                       : 2;	//  1: 2
} BBC_CTRL;

/*-----------------------------------------------------------------------------
	0x022c bbc_cur_pos ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	bbc_cur_pos                     :24;	//  0:23
} BBC_CUR_POS;

/*-----------------------------------------------------------------------------
	0x0230 bbc_bas_addr ''
-----------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x0234 bbc_strm_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	stream_count                    :24,	//  0:23
	                                : 7,	// 24:30 reserved
	endofstream                     : 1;	//    31
} BBC_STRM_CTRL;

/*-----------------------------------------------------------------------------
	0x0238 bbc_flush_cmd ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	bbc_flush_cmd                   : 1;	//     0
} BBC_FLUSH_CMD;

typedef struct {
	PIC_START                       	pic_start                       ;	// 0x0000 : ''
	PIC_STATUS                      	pic_status                      ;	// 0x0004 : ''
	PIC_ERRMB                       	pic_errmb                       ;	// 0x0008 : ''
	PIC_SETMB                       	pic_setmb                       ;	// 0x000c : ''
	PIC_CTRL                        	pic_ctrl                        ;	// 0x0010 : ''
	PIC_SIZE                        	pic_size                        ;	// 0x0014 : ''
	MCU_INFO                        	mcu_info                        ;	// 0x0018 : ''
	ROT_INFO                        	rot_info                        ;	// 0x001c : ''
	SCL_INFO                        	scl_info                        ;	// 0x0020 : ''
	IF_INFO                         	if_info                         ;	// 0x0024 : ''
	UINT32                          	                 __rsvd_00[   1];	// 0x0028
	OP_INFO                         	op_info                         ;	// 0x002c : ''
	DPB_CONFIG                      	dpb_config                      ;	// 0x0030 : ''
	UINT32                          	dpb_base00                      ;	// 0x0034 : ''
	UINT32                          	dpb_base01                      ;	// 0x0038 : ''
	UINT32                          	dpb_base02                      ;	// 0x003c : ''
	UINT32                          	dpb_base10                      ;	// 0x0040 : ''
	UINT32                          	dpb_base11                      ;	// 0x0044 : ''
	UINT32                          	dpb_base12                      ;	// 0x0048 : ''
	UINT32                          	dpb_base20                      ;	// 0x004c : ''
	UINT32                          	dpb_base21                      ;	// 0x0050 : ''
	UINT32                          	dpb_base22                      ;	// 0x0054 : ''
	UINT32                          	dpb_base30                      ;	// 0x0058 : ''
	UINT32                          	dpb_base31                      ;	// 0x005c : ''
	UINT32                          	dpb_base32                      ;	// 0x0060 : ''
	DPB_YSTRIDE                     	dpb_ystride                     ;	// 0x0064 : ''
	DPB_CSTRIDE                     	dpb_cstride                     ;	// 0x0068 : ''
	UINT32                          	                 __rsvd_01[   5];	// 0x006c ~ 0x007c
	HUFF_CTRL                       	huff_ctrl                       ;	// 0x0080 : ''
	HUFF_ADDR                       	huff_addr                       ;	// 0x0084 : ''
	UINT32                          	huff_data                       ;	// 0x0088 : ''
	UINT32                          	                 __rsvd_02[   1];	// 0x008c
	QMAT_CTRL                       	qmat_ctrl                       ;	// 0x0090 : ''
	QMAT_ADDR                       	qmat_addr                       ;	// 0x0094 : ''
	UINT32                          	qmat_data                       ;	// 0x0098 : ''
	UINT32                          	                 __rsvd_03[   1];	// 0x009c
	COEF_CTRL                       	coef_ctrl                       ;	// 0x00a0 : ''
	COEF_ADDR                       	coef_addr                       ;	// 0x00a4 : ''
	UINT32                          	coef_data                       ;	// 0x00a8 : ''
	UINT32                          	                 __rsvd_04[   1];	// 0x00ac
	UINT32                          	rst_interval                    ;	// 0x00b0 : ''
	RST_INDEX                       	rst_index                       ;	// 0x00b4 : ''
	RST_COUNT                       	rst_count                       ;	// 0x00b8 : ''
	UINT32                          	                 __rsvd_05[   1];	// 0x00bc
	INTR_MASK                       	intr_mask                       ;	// 0x00c0 : ''
	UINT32                          	                 __rsvd_06[   1];	// 0x00c4
	UINT32                          	cycle_info                      ;	// 0x00c8 : ''
	UINT32                          	                 __rsvd_07[   9];	// 0x00cc ~ 0x00ec
	DPCM_DIFF_Y                     	dpcm_diff_y                     ;	// 0x00f0 : ''
	DPCM_DIFF_CB                    	dpcm_diff_cb                    ;	// 0x00f4 : ''
	DPCM_DIFF_CR                    	dpcm_diff_cr                    ;	// 0x00f8 : ''
	UINT32                          	                 __rsvd_08[   1];	// 0x00fc
	GBU_CTRL                        	gbu_ctrl                        ;	// 0x0100 : ''
	UINT32                          	                 __rsvd_09[   3];	// 0x0104 ~ 0x010c
	UINT32                          	gbu_bt_ptr                      ;	// 0x0110 : ''
	GBU_WD_PTR                      	gbu_wd_ptr                      ;	// 0x0114 : ''
	UINT32                          	gbu_tt_cnt_low                  ;	// 0x0118 : ''
	UINT32                          	gbu_tt_cnt_high                 ;	// 0x011c : ''
	UINT32                          	                 __rsvd_10[   8];	// 0x0120 ~ 0x013c
	GBU_BBSR                        	gbu_bbsr                        ;	// 0x0140 : ''
	GBU_BBER                        	gbu_bber                        ;	// 0x0144 : ''
	GBU_BBIR                        	gbu_bbir                        ;	// 0x0148 : ''
	GBU_BBHR                        	gbu_bbhr                        ;	// 0x014c : ''
	UINT32                          	                 __rsvd_11[   2];	// 0x0150 ~ 0x0154
	GBU_BCNT                        	gbu_bcnt                        ;	// 0x0158 : ''
	UINT32                          	                 __rsvd_12[   1];	// 0x015c
	GBU_FF_RPTR                     	gbu_ff_rptr                     ;	// 0x0160 : ''
	GBU_FF_WPTR                     	gbu_ff_wptr                     ;	// 0x0164 : ''
	UINT32                          	                 __rsvd_13[  40];	// 0x0168 ~ 0x0204
	UINT32                          	bbc_endaddr                     ;	// 0x0208 : ''
	UINT32                          	bbc_wr_ptr                      ;	// 0x020c : ''
	UINT32                          	bbc_rd_ptr                      ;	// 0x0210 : ''
	UINT32                          	bbc_ext_addr                    ;	// 0x0214 : ''
	BBC_INT_ADDR                    	bbc_int_addr                    ;	// 0x0218 : ''
	BBC_DATA_CNT                    	bbc_data_cnt                    ;	// 0x021c : ''
	BBC_COMMAND                     	bbc_command                     ;	// 0x0220 : ''
	BBC_BUSY                        	bbc_busy                        ;	// 0x0224 : ''
	BBC_CTRL                        	bbc_ctrl                        ;	// 0x0228 : ''
	BBC_CUR_POS                     	bbc_cur_pos                     ;	// 0x022c : ''
	UINT32                          	bbc_bas_addr                    ;	// 0x0230 : ''
	BBC_STRM_CTRL                   	bbc_strm_ctrl                   ;	// 0x0234 : ''
	BBC_FLUSH_CMD                   	bbc_flush_cmd                   ;	// 0x0238 : ''
} MJPEG_REG_T;
/* 68 regs, 68 types */

/* 68 regs, 68 types in Total*/

/*
 * @{
 * Naming for register pointer.
 * gpRealRegMJPEG : real register of mjpeg.
 * gpRegMJPEG     : shadow register.
 *
 * @def MJPEG_RdFL: Read  FLushing : Shadow <- Real.
 * @def MJPEG_WrFL: Write FLushing : Shadow -> Real.
 * @def MJPEG_Rd  : Read  whole register(UINT32) from Shadow register.
 * @def MJPEG_Wr  : Write whole register(UINT32) from Shadow register.
 * @def MJPEG_Rd01 ~ MJPEG_Rdnn: Read  given '01~nn' fields from Shadow register.
 * @def MJPEG_Wr01 ~ MJPEG_Wrnn: Write given '01~nn' fields to   Shadow register.
 * */

#ifdef MJPEG_DEBUG_REG_DUMP
#define MJPEG_RdFL(_r)			((gpRegMJPEG->_r)=(gpRealRegMJPEG->_r))
#define MJPEG_WrFL(_r)									\
	do { 												\
		UINT32 offset = 0;								\
		offset += (UINT32)&(gpRealRegMJPEG->_r);		\
		offset -= (UINT32)&(gpRealRegMJPEG->pic_start); \
		_logIndex += sprintf(_logBuffer + _logIndex,	\
			"[%-27s], 0x%08X, 0x%08X, %s\n", 			\
			__F__, offset, *((UINT32*)(&(gpRegMJPEG->_r))), "" #_r ""); \
		((gpRealRegMJPEG->_r)=(gpRegMJPEG->_r));	\
	} while( 0 )
#else
#define MJPEG_RdFL(_r)			((gpRegMJPEG->_r)=(gpRealRegMJPEG->_r))
#define MJPEG_WrFL(_r)			((gpRealRegMJPEG->_r)=(gpRegMJPEG->_r))
#endif

#ifdef MJPEG_DEBUG_TRACE_DUMP
#define MJPEG_LOG_TRACE()		do { 	\
		_logIndex += sprintf(_logBuffer + _logIndex, "[%-26s]\n", __F__);	\
	} while( 0 )
#else
#define MJPEG_LOG_TRACE()
#endif

#if defined(MJPEG_DEBUG_TRACE_DUMP) && !defined(MJPEG_DEBUG_REG_DUMP)
#define MJPEG_REG_LOG(_r)								\
	do { 												\
		UINT32 offset = 0;								\
		offset += (UINT32)&(gpRealRegMJPEG->_r);		\
		offset -= (UINT32)&(gpRealRegMJPEG->pic_start);	\
		_logIndex += sprintf( _logBuffer + _logIndex, 	\
			"[%-27s], 0x%08X, 0x%08X,%s\n", 			\
			__F__, offset, *((UINT32*)(&(gpRegMJPEG->_r))), "" #_r ""); \
	} while( 0 )
#else
#define MJPEG_REG_LOG(_r)
#endif

#define MJPEG_Rd(_r)			*((UINT32*)(&(gpRegMJPEG->_r)))
#define MJPEG_Wr(_r,_v)			((MJPEG_Rd(_r))=((UINT32)(_v)))

#define MJPEG_Rd01(_r,_f01,_v01)												\
								do { 											\
									(_v01) = (gpRegMJPEG->_r._f01);				\
								} while(0)

#define MJPEG_Rd02(_r,_f01,_v01,_f02,_v02)										\
								do { 											\
									(_v01) = (gpRegMJPEG->_r._f01);				\
									(_v02) = (gpRegMJPEG->_r._f02);				\
								} while(0)

#define MJPEG_Rd03(_r,_f01,_v01,_f02,_v02,_f03,_v03)								\
								do { 											\
									(_v01) = (gpRegMJPEG->_r._f01);				\
									(_v02) = (gpRegMJPEG->_r._f02);				\
									(_v03) = (gpRegMJPEG->_r._f03);				\
								} while(0)

#define MJPEG_Rd04(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04)					\
								do { 											\
									(_v01) = (gpRegMJPEG->_r._f01);				\
									(_v02) = (gpRegMJPEG->_r._f02);				\
									(_v03) = (gpRegMJPEG->_r._f03);				\
									(_v04) = (gpRegMJPEG->_r._f04);				\
								} while(0)

#define MJPEG_Rd05(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05)													\
								do { 											\
									(_v01) = (gpRegMJPEG->_r._f01);				\
									(_v02) = (gpRegMJPEG->_r._f02);				\
									(_v03) = (gpRegMJPEG->_r._f03);				\
									(_v04) = (gpRegMJPEG->_r._f04);				\
									(_v05) = (gpRegMJPEG->_r._f05);				\
								} while(0)

#define MJPEG_Rd06(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06)										\
								do { 											\
									(_v01) = (gpRegMJPEG->_r._f01);				\
									(_v02) = (gpRegMJPEG->_r._f02);				\
									(_v03) = (gpRegMJPEG->_r._f03);				\
									(_v04) = (gpRegMJPEG->_r._f04);				\
									(_v05) = (gpRegMJPEG->_r._f05);				\
									(_v06) = (gpRegMJPEG->_r._f06);				\
								} while(0)

#define MJPEG_Rd07(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07)								\
								do { 											\
									(_v01) = (gpRegMJPEG->_r._f01);				\
									(_v02) = (gpRegMJPEG->_r._f02);				\
									(_v03) = (gpRegMJPEG->_r._f03);				\
									(_v04) = (gpRegMJPEG->_r._f04);				\
									(_v05) = (gpRegMJPEG->_r._f05);				\
									(_v06) = (gpRegMJPEG->_r._f06);				\
									(_v07) = (gpRegMJPEG->_r._f07);				\
								} while(0)

#define MJPEG_Rd08(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08)					\
								do { 											\
									(_v01) = (gpRegMJPEG->_r._f01);				\
									(_v02) = (gpRegMJPEG->_r._f02);				\
									(_v03) = (gpRegMJPEG->_r._f03);				\
									(_v04) = (gpRegMJPEG->_r._f04);				\
									(_v05) = (gpRegMJPEG->_r._f05);				\
									(_v06) = (gpRegMJPEG->_r._f06);				\
									(_v07) = (gpRegMJPEG->_r._f07);				\
									(_v08) = (gpRegMJPEG->_r._f08);				\
								} while(0)

#define MJPEG_Rd09(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09)													\
								do { 											\
									(_v01) = (gpRegMJPEG->_r._f01);				\
									(_v02) = (gpRegMJPEG->_r._f02);				\
									(_v03) = (gpRegMJPEG->_r._f03);				\
									(_v04) = (gpRegMJPEG->_r._f04);				\
									(_v05) = (gpRegMJPEG->_r._f05);				\
									(_v06) = (gpRegMJPEG->_r._f06);				\
									(_v07) = (gpRegMJPEG->_r._f07);				\
									(_v08) = (gpRegMJPEG->_r._f08);				\
									(_v09) = (gpRegMJPEG->_r._f09);				\
								} while(0)

#define MJPEG_Rd10(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10)										\
								do { 											\
									(_v01) = (gpRegMJPEG->_r._f01);				\
									(_v02) = (gpRegMJPEG->_r._f02);				\
									(_v03) = (gpRegMJPEG->_r._f03);				\
									(_v04) = (gpRegMJPEG->_r._f04);				\
									(_v05) = (gpRegMJPEG->_r._f05);				\
									(_v06) = (gpRegMJPEG->_r._f06);				\
									(_v07) = (gpRegMJPEG->_r._f07);				\
									(_v08) = (gpRegMJPEG->_r._f08);				\
									(_v09) = (gpRegMJPEG->_r._f09);				\
									(_v10) = (gpRegMJPEG->_r._f10);				\
								} while(0)

#define MJPEG_Rd11(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11)								\
								do { 											\
									(_v01) = (gpRegMJPEG->_r._f01);				\
									(_v02) = (gpRegMJPEG->_r._f02);				\
									(_v03) = (gpRegMJPEG->_r._f03);				\
									(_v04) = (gpRegMJPEG->_r._f04);				\
									(_v05) = (gpRegMJPEG->_r._f05);				\
									(_v06) = (gpRegMJPEG->_r._f06);				\
									(_v07) = (gpRegMJPEG->_r._f07);				\
									(_v08) = (gpRegMJPEG->_r._f08);				\
									(_v09) = (gpRegMJPEG->_r._f09);				\
									(_v10) = (gpRegMJPEG->_r._f10);				\
									(_v11) = (gpRegMJPEG->_r._f11);				\
								} while(0)

#define MJPEG_Rd12(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12)					\
								do { 											\
									(_v01) = (gpRegMJPEG->_r._f01);				\
									(_v02) = (gpRegMJPEG->_r._f02);				\
									(_v03) = (gpRegMJPEG->_r._f03);				\
									(_v04) = (gpRegMJPEG->_r._f04);				\
									(_v05) = (gpRegMJPEG->_r._f05);				\
									(_v06) = (gpRegMJPEG->_r._f06);				\
									(_v07) = (gpRegMJPEG->_r._f07);				\
									(_v08) = (gpRegMJPEG->_r._f08);				\
									(_v09) = (gpRegMJPEG->_r._f09);				\
									(_v10) = (gpRegMJPEG->_r._f10);				\
									(_v11) = (gpRegMJPEG->_r._f11);				\
									(_v12) = (gpRegMJPEG->_r._f12);				\
								} while(0)

#define MJPEG_Rd13(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13)													\
								do { 											\
									(_v01) = (gpRegMJPEG->_r._f01);				\
									(_v02) = (gpRegMJPEG->_r._f02);				\
									(_v03) = (gpRegMJPEG->_r._f03);				\
									(_v04) = (gpRegMJPEG->_r._f04);				\
									(_v05) = (gpRegMJPEG->_r._f05);				\
									(_v06) = (gpRegMJPEG->_r._f06);				\
									(_v07) = (gpRegMJPEG->_r._f07);				\
									(_v08) = (gpRegMJPEG->_r._f08);				\
									(_v09) = (gpRegMJPEG->_r._f09);				\
									(_v10) = (gpRegMJPEG->_r._f10);				\
									(_v11) = (gpRegMJPEG->_r._f11);				\
									(_v12) = (gpRegMJPEG->_r._f12);				\
									(_v13) = (gpRegMJPEG->_r._f13);				\
								} while(0)

#define MJPEG_Rd14(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14)										\
								do { 											\
									(_v01) = (gpRegMJPEG->_r._f01);				\
									(_v02) = (gpRegMJPEG->_r._f02);				\
									(_v03) = (gpRegMJPEG->_r._f03);				\
									(_v04) = (gpRegMJPEG->_r._f04);				\
									(_v05) = (gpRegMJPEG->_r._f05);				\
									(_v06) = (gpRegMJPEG->_r._f06);				\
									(_v07) = (gpRegMJPEG->_r._f07);				\
									(_v08) = (gpRegMJPEG->_r._f08);				\
									(_v09) = (gpRegMJPEG->_r._f09);				\
									(_v10) = (gpRegMJPEG->_r._f10);				\
									(_v11) = (gpRegMJPEG->_r._f11);				\
									(_v12) = (gpRegMJPEG->_r._f12);				\
									(_v13) = (gpRegMJPEG->_r._f13);				\
									(_v14) = (gpRegMJPEG->_r._f14);				\
								} while(0)

#define MJPEG_Rd15(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15)								\
								do { 											\
									(_v01) = (gpRegMJPEG->_r._f01);				\
									(_v02) = (gpRegMJPEG->_r._f02);				\
									(_v03) = (gpRegMJPEG->_r._f03);				\
									(_v04) = (gpRegMJPEG->_r._f04);				\
									(_v05) = (gpRegMJPEG->_r._f05);				\
									(_v06) = (gpRegMJPEG->_r._f06);				\
									(_v07) = (gpRegMJPEG->_r._f07);				\
									(_v08) = (gpRegMJPEG->_r._f08);				\
									(_v09) = (gpRegMJPEG->_r._f09);				\
									(_v10) = (gpRegMJPEG->_r._f10);				\
									(_v11) = (gpRegMJPEG->_r._f11);				\
									(_v12) = (gpRegMJPEG->_r._f12);				\
									(_v13) = (gpRegMJPEG->_r._f13);				\
									(_v14) = (gpRegMJPEG->_r._f14);				\
									(_v15) = (gpRegMJPEG->_r._f15);				\
								} while(0)

#define MJPEG_Rd16(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15,_f16,_v16)					\
								do { 											\
									(_v01) = (gpRegMJPEG->_r._f01);				\
									(_v02) = (gpRegMJPEG->_r._f02);				\
									(_v03) = (gpRegMJPEG->_r._f03);				\
									(_v04) = (gpRegMJPEG->_r._f04);				\
									(_v05) = (gpRegMJPEG->_r._f05);				\
									(_v06) = (gpRegMJPEG->_r._f06);				\
									(_v07) = (gpRegMJPEG->_r._f07);				\
									(_v08) = (gpRegMJPEG->_r._f08);				\
									(_v09) = (gpRegMJPEG->_r._f09);				\
									(_v10) = (gpRegMJPEG->_r._f10);				\
									(_v11) = (gpRegMJPEG->_r._f11);				\
									(_v12) = (gpRegMJPEG->_r._f12);				\
									(_v13) = (gpRegMJPEG->_r._f13);				\
									(_v14) = (gpRegMJPEG->_r._f14);				\
									(_v15) = (gpRegMJPEG->_r._f15);				\
									(_v16) = (gpRegMJPEG->_r._f16);				\
								} while(0)


#define MJPEG_Wr01(_r,_f01,_v01)													\
								do { 											\
									(gpRegMJPEG->_r._f01) = (_v01);				\
								} while(0)

#define MJPEG_Wr02(_r,_f01,_v01,_f02,_v02)										\
								do { 											\
									(gpRegMJPEG->_r._f01) = (_v01);				\
									(gpRegMJPEG->_r._f02) = (_v02);				\
								} while(0)

#define MJPEG_Wr03(_r,_f01,_v01,_f02,_v02,_f03,_v03)								\
								do { 											\
									(gpRegMJPEG->_r._f01) = (_v01);				\
									(gpRegMJPEG->_r._f02) = (_v02);				\
									(gpRegMJPEG->_r._f03) = (_v03);				\
								} while(0)

#define MJPEG_Wr04(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04)					\
								do { 											\
									(gpRegMJPEG->_r._f01) = (_v01);				\
									(gpRegMJPEG->_r._f02) = (_v02);				\
									(gpRegMJPEG->_r._f03) = (_v03);				\
									(gpRegMJPEG->_r._f04) = (_v04);				\
								} while(0)

#define MJPEG_Wr05(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05)													\
								do { 											\
									(gpRegMJPEG->_r._f01) = (_v01);				\
									(gpRegMJPEG->_r._f02) = (_v02);				\
									(gpRegMJPEG->_r._f03) = (_v03);				\
									(gpRegMJPEG->_r._f04) = (_v04);				\
									(gpRegMJPEG->_r._f05) = (_v05);				\
								} while(0)

#define MJPEG_Wr06(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06)										\
								do { 											\
									(gpRegMJPEG->_r._f01) = (_v01);				\
									(gpRegMJPEG->_r._f02) = (_v02);				\
									(gpRegMJPEG->_r._f03) = (_v03);				\
									(gpRegMJPEG->_r._f04) = (_v04);				\
									(gpRegMJPEG->_r._f05) = (_v05);				\
									(gpRegMJPEG->_r._f06) = (_v06);				\
								} while(0)

#define MJPEG_Wr07(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07)								\
								do { 											\
									(gpRegMJPEG->_r._f01) = (_v01);				\
									(gpRegMJPEG->_r._f02) = (_v02);				\
									(gpRegMJPEG->_r._f03) = (_v03);				\
									(gpRegMJPEG->_r._f04) = (_v04);				\
									(gpRegMJPEG->_r._f05) = (_v05);				\
									(gpRegMJPEG->_r._f06) = (_v06);				\
									(gpRegMJPEG->_r._f07) = (_v07);				\
								} while(0)

#define MJPEG_Wr08(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08)					\
								do { 											\
									(gpRegMJPEG->_r._f01) = (_v01);				\
									(gpRegMJPEG->_r._f02) = (_v02);				\
									(gpRegMJPEG->_r._f03) = (_v03);				\
									(gpRegMJPEG->_r._f04) = (_v04);				\
									(gpRegMJPEG->_r._f05) = (_v05);				\
									(gpRegMJPEG->_r._f06) = (_v06);				\
									(gpRegMJPEG->_r._f07) = (_v07);				\
									(gpRegMJPEG->_r._f08) = (_v08);				\
								} while(0)

#define MJPEG_Wr09(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09)													\
								do { 											\
									(gpRegMJPEG->_r._f01) = (_v01);				\
									(gpRegMJPEG->_r._f02) = (_v02);				\
									(gpRegMJPEG->_r._f03) = (_v03);				\
									(gpRegMJPEG->_r._f04) = (_v04);				\
									(gpRegMJPEG->_r._f05) = (_v05);				\
									(gpRegMJPEG->_r._f06) = (_v06);				\
									(gpRegMJPEG->_r._f07) = (_v07);				\
									(gpRegMJPEG->_r._f08) = (_v08);				\
									(gpRegMJPEG->_r._f09) = (_v09);				\
								} while(0)

#define MJPEG_Wr10(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10)										\
								do { 											\
									(gpRegMJPEG->_r._f01) = (_v01);				\
									(gpRegMJPEG->_r._f02) = (_v02);				\
									(gpRegMJPEG->_r._f03) = (_v03);				\
									(gpRegMJPEG->_r._f04) = (_v04);				\
									(gpRegMJPEG->_r._f05) = (_v05);				\
									(gpRegMJPEG->_r._f06) = (_v06);				\
									(gpRegMJPEG->_r._f07) = (_v07);				\
									(gpRegMJPEG->_r._f08) = (_v08);				\
									(gpRegMJPEG->_r._f09) = (_v09);				\
									(gpRegMJPEG->_r._f10) = (_v10);				\
								} while(0)

#define MJPEG_Wr11(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11)								\
								do { 											\
									(gpRegMJPEG->_r._f01) = (_v01);				\
									(gpRegMJPEG->_r._f02) = (_v02);				\
									(gpRegMJPEG->_r._f03) = (_v03);				\
									(gpRegMJPEG->_r._f04) = (_v04);				\
									(gpRegMJPEG->_r._f05) = (_v05);				\
									(gpRegMJPEG->_r._f06) = (_v06);				\
									(gpRegMJPEG->_r._f07) = (_v07);				\
									(gpRegMJPEG->_r._f08) = (_v08);				\
									(gpRegMJPEG->_r._f09) = (_v09);				\
									(gpRegMJPEG->_r._f10) = (_v10);				\
									(gpRegMJPEG->_r._f11) = (_v11);				\
								} while(0)

#define MJPEG_Wr12(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12)					\
								do { 											\
									(gpRegMJPEG->_r._f01) = (_v01);				\
									(gpRegMJPEG->_r._f02) = (_v02);				\
									(gpRegMJPEG->_r._f03) = (_v03);				\
									(gpRegMJPEG->_r._f04) = (_v04);				\
									(gpRegMJPEG->_r._f05) = (_v05);				\
									(gpRegMJPEG->_r._f06) = (_v06);				\
									(gpRegMJPEG->_r._f07) = (_v07);				\
									(gpRegMJPEG->_r._f08) = (_v08);				\
									(gpRegMJPEG->_r._f09) = (_v09);				\
									(gpRegMJPEG->_r._f10) = (_v10);				\
									(gpRegMJPEG->_r._f11) = (_v11);				\
									(gpRegMJPEG->_r._f12) = (_v12);				\
								} while(0)

#define MJPEG_Wr13(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13)													\
								do { 											\
									(gpRegMJPEG->_r._f01) = (_v01);				\
									(gpRegMJPEG->_r._f02) = (_v02);				\
									(gpRegMJPEG->_r._f03) = (_v03);				\
									(gpRegMJPEG->_r._f04) = (_v04);				\
									(gpRegMJPEG->_r._f05) = (_v05);				\
									(gpRegMJPEG->_r._f06) = (_v06);				\
									(gpRegMJPEG->_r._f07) = (_v07);				\
									(gpRegMJPEG->_r._f08) = (_v08);				\
									(gpRegMJPEG->_r._f09) = (_v09);				\
									(gpRegMJPEG->_r._f10) = (_v10);				\
									(gpRegMJPEG->_r._f11) = (_v11);				\
									(gpRegMJPEG->_r._f12) = (_v12);				\
									(gpRegMJPEG->_r._f13) = (_v13);				\
								} while(0)

#define MJPEG_Wr14(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14)										\
								do { 											\
									(gpRegMJPEG->_r._f01) = (_v01);				\
									(gpRegMJPEG->_r._f02) = (_v02);				\
									(gpRegMJPEG->_r._f03) = (_v03);				\
									(gpRegMJPEG->_r._f04) = (_v04);				\
									(gpRegMJPEG->_r._f05) = (_v05);				\
									(gpRegMJPEG->_r._f06) = (_v06);				\
									(gpRegMJPEG->_r._f07) = (_v07);				\
									(gpRegMJPEG->_r._f08) = (_v08);				\
									(gpRegMJPEG->_r._f09) = (_v09);				\
									(gpRegMJPEG->_r._f10) = (_v10);				\
									(gpRegMJPEG->_r._f11) = (_v11);				\
									(gpRegMJPEG->_r._f12) = (_v12);				\
									(gpRegMJPEG->_r._f13) = (_v13);				\
									(gpRegMJPEG->_r._f14) = (_v14);				\
								} while(0)

#define MJPEG_Wr15(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15)								\
								do { 											\
									(gpRegMJPEG->_r._f01) = (_v01);				\
									(gpRegMJPEG->_r._f02) = (_v02);				\
									(gpRegMJPEG->_r._f03) = (_v03);				\
									(gpRegMJPEG->_r._f04) = (_v04);				\
									(gpRegMJPEG->_r._f05) = (_v05);				\
									(gpRegMJPEG->_r._f06) = (_v06);				\
									(gpRegMJPEG->_r._f07) = (_v07);				\
									(gpRegMJPEG->_r._f08) = (_v08);				\
									(gpRegMJPEG->_r._f09) = (_v09);				\
									(gpRegMJPEG->_r._f10) = (_v10);				\
									(gpRegMJPEG->_r._f11) = (_v11);				\
									(gpRegMJPEG->_r._f12) = (_v12);				\
									(gpRegMJPEG->_r._f13) = (_v13);				\
									(gpRegMJPEG->_r._f14) = (_v14);				\
									(gpRegMJPEG->_r._f15) = (_v15);				\
								} while(0)

#define MJPEG_Wr16(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15,_f16,_v16)					\
								do { 											\
									(gpRegMJPEG->_r._f01) = (_v01);				\
									(gpRegMJPEG->_r._f02) = (_v02);				\
									(gpRegMJPEG->_r._f03) = (_v03);				\
									(gpRegMJPEG->_r._f04) = (_v04);				\
									(gpRegMJPEG->_r._f05) = (_v05);				\
									(gpRegMJPEG->_r._f06) = (_v06);				\
									(gpRegMJPEG->_r._f07) = (_v07);				\
									(gpRegMJPEG->_r._f08) = (_v08);				\
									(gpRegMJPEG->_r._f09) = (_v09);				\
									(gpRegMJPEG->_r._f10) = (_v10);				\
									(gpRegMJPEG->_r._f11) = (_v11);				\
									(gpRegMJPEG->_r._f12) = (_v12);				\
									(gpRegMJPEG->_r._f13) = (_v13);				\
									(gpRegMJPEG->_r._f14) = (_v14);				\
									(gpRegMJPEG->_r._f15) = (_v15);				\
									(gpRegMJPEG->_r._f16) = (_v16);				\
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
 * _rwname: name of rw    bit field : shall be 0 after MJPEG_Wind(), 1 for MJPEG_Rind()
 * _iname : name of index bit field
 * _ival  : index value
 * _fname : field name
 * _fval  : field variable that field value shall be stored.
 *
 * MJPEG_Rind : General indexed register Read.(
 * MJPEG_Wind : General indexed register Read.
 *
 * MJPEG_Ridx : For 'index', 'rw', 'load' field name
 * MJPEG_Widx : For 'index', 'rw', 'load' field name and NO_LOAD.
 */
#define MJPEG_Rind(_r, _lname, _rwname, _iname, _ival, _fname, _fval)				\
							do {												\
								MJPEG_Wr03(_r,_lname,0,_rwname,1,_iname,_ival);	\
								MJPEG_WrFL(_r);									\
								MJPEG_RdFL(_r);									\
								MJPEG_Rd01(_r,_fname,_fval);						\
							} while (0)

#define MJPEG_Wind(_r, _lname, _rwname, _iname, _ival, _fname, _fval)				\
				MJPEG_Wr04(_r, _lname,0, _rwname,0, _iname,_ival, _fname,_fval)


#define MJPEG_Ridx(_r, _ival, _fname, _fval)	MJPEG_Rind(_r,load,rw,index,_ival,_fname,_fval)

#define MJPEG_Widx(_r, _ival, _fname, _fval)	MJPEG_Wind(_r,load,rw,index,_ival,_fname,_fval)

/** @} *//* end of macro documentation */

#ifdef __cplusplus
}
#endif

#endif	/* _MJPEG_REG_H_ */

/* from 'MJPEG.csv' 20110410 15:46:29   대한민국 표준시 by getregs v2.7 */
