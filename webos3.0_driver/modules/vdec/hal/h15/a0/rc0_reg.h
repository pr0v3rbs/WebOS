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
 *  #MOD# register details. ( used only within kdriver )
 *
 *  author     user name (user_name@lge.com)
 *  version    1.0
 *  date       2010.xx.xx
 *
 */

#ifndef _RC0_REG_H_
#define _RC0_REG_H_

/*----------------------------------------------------------------------------------------
    Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/


#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------------
	0x0000 version ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	yyyymmdd                        ;   	// 31: 0
} RC0_VERSION;

/*-----------------------------------------------------------------------------
	0x0004 sw_reset ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_st_mach0_rst_ctrl           : 4,	//  0: 3
	reg_st_mach1_rst_ctrl           : 4,	//  4: 7
	reg_st_pdec_rst_ctrl            : 4,	//  8:11
	reg_st_vdo_rst_ctrl				: 4,	// 12:15
	reg_mach0_intr_ack_en           : 1,	//    16
	reg_mach1_intr_ack_en           : 1,	//    17
	reg_pdec_intr_ack_en            : 1,	//    18
	reg_vdo_intr_ack_en             : 1,	//    19
	reg_mach0_srst_without_fdone    : 1,	//    20
	reg_mach1_srst_without_fdone    : 1,	//    21
	reg_pdec_srst_without_fdone     : 1,	//    22
	reg_vdo_srst_without_fdone      : 1,	//    23
	sw_reset_pdec_mux               : 1,	//    24
	sw_reset_stcc_discont           : 1,	//    25
	sw_reset_pmau                   : 1,	//    26
	sw_reset_gstcc                  : 1,	//    27
	sw_reset_mach0                  : 1,	//    28
	sw_reset_mach1                  : 1,	//    29
	sw_reset_pdec_all               : 1,	//    30
	sw_reset_vdo                	: 1;	//    31
} SW_RESET;

/*-----------------------------------------------------------------------------
	0x0008 apb_conf ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_apb_s0_prt                  : 4,	//  0: 3
	reg_apb_s1_prt                  : 4,	//  4: 7
	reg_apb_s2_prt                  : 4,	//  8:11
	                                : 1,	//    12 reserved
	reg_apb_lq_offset               : 3,	// 13:15
	                                : 1,	//    16 reserved
	reg_apb_mach0_offset            : 7,	// 17:23
						            : 5,	// 24:28
	reg_apb_vdo_offset              : 3;	// 29:31
} APB_CONF;

/*-----------------------------------------------------------------------------
	0x0010 pdec_in_conf ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_pdec0_in_sel                : 2,	//  0: 1
	reg_pdec1_in_sel                : 2,	//  2: 3
	reg_pdec2_in_sel                : 2,	//  4: 5
	                                : 2,	//  6: 7 reserved
	reg_pdec0_in_dis                : 1,	//     8
	reg_pdec1_in_dis                : 1,	//     9
	reg_pdec2_in_dis                : 1,	//    10
	                                : 1,	//    11 reserved
	reg_vid0_rdy_dis                : 1,	//    12
	reg_vid1_rdy_dis                : 1,	//    13
	reg_vid2_rdy_dis                : 1;	//    14
} PDEC_IN_CONF;

/*-----------------------------------------------------------------------------
	0x0014 lq_in_conf ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 8,	//  0: 7 reserved
	reg_lqc0_sync_mode              : 2,	//  8: 9
	reg_lqc1_sync_mode              : 2,	// 10:11
	reg_lqc2_sync_mode              : 2,	// 12:13
	reg_lqc3_sync_mode              : 2,	// 14:15
	reg_lq0_in_sel                  : 2,	// 16:17
	reg_lq1_in_sel                  : 2,	// 18:19
	reg_lq2_in_sel                  : 2,	// 20:21
	reg_lq3_in_sel                  : 2,	// 22:23
	reg_lq0_in_dis                  : 1,	//    24
	reg_lq1_in_dis                  : 1,	//    25
	reg_lq2_in_dis                  : 1,	//    26
	reg_lq3_in_dis                  : 1;	//    27
} LQ_IN_CONF;

/*-----------------------------------------------------------------------------
	0x0018 mach_conf ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_mach0_intr_mode             : 1,	//     0
	reg_mach1_intr_mode             : 1,	//     1
	                                : 2,	//  2: 3 reserved
	reg_mach0_vpu_idle              : 1,	//     4
	reg_mach0_vpu_underrun          : 1,	//     5
	                                : 2,	//  6: 7 reserved
	reg_mach1_vpu_idle              : 1,	//     8
	reg_mach1_vpu_underrun          : 1;	//     9
} MACH_CONF;

/*-----------------------------------------------------------------------------
	0x001c sync_conf ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_sync_sel	                : 8;	//  0: 7 sync selection for the vsync4
} SYNC_CONF;

/*-----------------------------------------------------------------------------
	0x0020 pdec0_mau_conf ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_pdec0_cmd_dly_cnt           :16;	//  0:15
} PDEC0_MAU_CONF;

/*-----------------------------------------------------------------------------
	0x0024 pdec1_mau_conf ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_pdec1_cmd_dly_cnt           :16;	//  0:15
} PDEC1_MAU_CONF;

/*-----------------------------------------------------------------------------
	0x0028 pdec2_mau_conf ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_pdec2_cmd_dly_cnt           :16;	//  0:15
} PDEC2_MAU_CONF;

/*-----------------------------------------------------------------------------
	0x002c pdec3_mau_conf ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_pdec3_cmd_dly_cnt           :16;	//  0:15
} PDEC3_MAU_CONF;

/*-----------------------------------------------------------------------------
	0x0030 intr_e_en ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	mach0                           : 1,	//     0
	mach1                           : 1,	//     1
	nama                            : 1,	//     2
		                        : 1,	//     3 reserved
	vsync0                          : 1,	//     4
	vsync1                          : 1,	//     5
	vsync2                          : 1,	//     6
	vsync3                          : 1,	//     7
	vsync4				: 1,    //     8
	vsync5				: 1,    //     9
		                        : 1,	//    10 reserved
		                        : 1,	//    11 reserved
	pdec0_au_detect                 : 1,	//    12
	pdec1_au_detect                 : 1,	//    13
	pdec2_au_detect                 : 1,	//    14
	                                : 1,	//    15 reserved
	bst                             : 1,	//    16
	vdo_gdi_err                     : 1,	//    17
	mach0_srst_done                 : 1,	//    18
	mach1_srst_done                 : 1,	//    19
	pdec_srst_done                  : 1,	//    20
	vdo_srst_done                   : 1,	//    21
	venc                            : 1,	//    22
	vsync0_ready                    : 1,	//    23
	vsync1_ready                    : 1,	//    24
	monitor0                        : 1,	//    25
	monitor1                        : 1,	//    26
		                        : 1,	//    27 reserved
	timer0                          : 1,	//    28
	timer1                          : 1,	//    29
	timer2                          : 1,	//    30
	timer3                          : 1;	//    31
} INTR_E_EN;

/*-----------------------------------------------------------------------------
	0x0034 intr_e_st ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	mach0                           : 1,	//     0
	mach1                           : 1,	//     1
	nama                            : 1,	//     2
	                                : 1,	//     3 reserved
	vsync0                          : 1,	//     4
	vsync1                          : 1,	//     5
	vsync2                          : 1,	//     6
	vsync3                          : 1,	//     7
	vsync4                          : 1,	//     8
	vsync5                          : 1,	//     9
		                        : 1,	//    10 reserved
		                        : 1,	//    11 reserved
	pdec0_au_detect                 : 1,	//    12
	pdec1_au_detect                 : 1,	//    13
	pdec2_au_detect                 : 1,	//    14
	                                : 1,	//    15 reserved
	bst                             : 1,	//    16
	vdo_gdi_err                     : 1,	//    17
	mach0_srst_done                 : 1,	//    18
	mach1_srst_done                 : 1,	//    19
	pdec_srst_done                  : 1,	//    20
	vdo_srst_done                   : 1,	//    21
	venc                            : 1,	//    22
	vsync0_ready                    : 1,	//    23
	vsync1_ready                    : 1,	//    24
	monitor0                        : 1,	//    25
	monitor1                        : 1,	//    26
		                        : 1,	//    27 reserved
	timer0                          : 1,	//    28
	timer1                          : 1,	//    29
	timer2                          : 1,	//    30
	timer3                          : 1;	//    31
} INTR_E_ST;

/*-----------------------------------------------------------------------------
	0x0038 intr_e_cl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	mach0                           : 1,	//     0
	mach1                           : 1,	//     1
	nama                            : 1,	//     2
		                        : 1,	//     3 reserved
	vsync0                          : 1,	//     4
	vsync1                          : 1,	//     5
	vsync2                          : 1,	//     6
	vsync3                          : 1,	//     7
	vsync4                          : 1,	//     8
	vsync5                          : 1,	//     9
	        	                : 1,	//    10 reserved
		                        : 1,	//    11 reserved
	pdec0_au_detect                 : 1,	//    12
	pdec1_au_detect                 : 1,	//    13
	pdec2_au_detect                 : 1,	//    14
	                                : 1,	//    15 reserved
	bst                             : 1,	//    16
	vdo_gdi_err                     : 1,	//    17
	mach0_srst_done                 : 1,	//    18
	mach1_srst_done                 : 1,	//    19
	pdec_srst_done                  : 1,	//    20
	vdo_srst_done                   : 1,	//    21
	venc                            : 1,	//    22
	vsync0_ready                    : 1,	//    23
	vsync1_ready                    : 1,	//    24
	monitor0                        : 1,	//    25
	monitor1                        : 1,	//    26
		                        : 1,	//    27 rsvd
	timer0                          : 1,	//    28
	timer1                          : 1,	//    29
	timer2                          : 1,	//    30
	timer3                          : 1;	//    31
} INTR_E_CL;

/*-----------------------------------------------------------------------------
	0x0040 intr_i_en ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	mach0                           : 1,	//     0
	mach1                           : 1,	//     1
	nama                            : 1,	//     2
	                                : 1,	//     3 reserved
	vsync0                          : 1,	//     4
	vsync1                          : 1,	//     5
	vsync2                          : 1,	//     6
	vsync3                          : 1,	//     7
	vsync4                          : 1,	//     8
	vsync5                          : 1,	//     9
		                        : 1,	//    10 reserved
		                        : 1,	//    11 reserved
	pdec0_au_detect                 : 1,	//    12
	pdec1_au_detect                 : 1,	//    13
	pdec2_au_detect                 : 1,	//    14
	                                : 1,	//    15 reserved
	bst                             : 1,	//    16
	vdo_gdi_err                     : 1,	//    17
	mach0_srst_done                 : 1,	//    18
	mach1_srst_done                 : 1,	//    19
	pdec_srst_done                  : 1,	//    20
	vdo_srst_done                   : 1,	//    21
	venc                            : 1,	//    22
	vsync0_ready                    : 1,	//    23
	vsync1_ready                    : 1,	//    24
	monitor0                        : 1,	//    25
	monitor1                        : 1,	//    26
		                        : 1,	//    27 rsvd
	timer0                          : 1,	//    28
	timer1                          : 1,	//    29
	timer2                          : 1,	//    30
	timer3                          : 1;	//    31
} INTR_I_EN;

/*-----------------------------------------------------------------------------
	0x0044 intr_i_st ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	mach0                           : 1,	//     0
	mach1                           : 1,	//     1
	nama                            : 1,	//     2
	                                : 1,	//     3 reserved
	vsync0                          : 1,	//     4
	vsync1                          : 1,	//     5
	vsync2                          : 1,	//     6
	vsync3                          : 1,	//     7
	vsync4                          : 1,	//     8
	vsync5                          : 1,	//     9
		                        : 1,	//    10 reserved
		                        : 1,	//    11 reserved
	pdec0_au_detect                 : 1,	//    12
	pdec1_au_detect                 : 1,	//    13
	pdec2_au_detect                 : 1,	//    14
	                                : 1,	//    15 reserved
	bst                             : 1,	//    16
	vdo_gdi_err                     : 1,	//    17
	mach0_srst_done                 : 1,	//    18
	mach1_srst_done                 : 1,	//    19
	pdec_srst_done                  : 1,	//    20
	vdo_srst_done                   : 1,	//    21
	venc                            : 1,	//    22
	vsync0_ready                    : 1,	//    23
	vsync1_ready                    : 1,	//    24
	monitor0                        : 1,	//    25
	monitor1                        : 1,	//    26
		                        : 1,	//    27 rsvd
	timer0                          : 1,	//    28
	timer1                          : 1,	//    29
	timer2                          : 1,	//    30
	timer3                          : 1;	//    31
} INTR_I_ST;

/*-----------------------------------------------------------------------------
	0x0048 intr_i_cl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	mach0                           : 1,	//     0
	mach1                           : 1,	//     1
	nama                            : 1,	//     2
  	                                : 1,	//     3 reserved
	vsync0                          : 1,	//     4
	vsync1                          : 1,	//     5
	vsync2                          : 1,	//     6
	vsync3                          : 1,	//     7
	vsync4                          : 1,	//     8
	vsync5                          : 1,	//     9
		                        : 1,	//    10 reserved
		                        : 1,	//    11 reserved
	pdec0_au_detect                 : 1,	//    12
	pdec1_au_detect                 : 1,	//    13
	pdec2_au_detect                 : 1,	//    14
	                                : 1,	//    15 reserved
	bst                             : 1,	//    16
	vdo_gdi_err                     : 1,	//    17
	mach0_srst_done                 : 1,	//    18
	mach1_srst_done                 : 1,	//    19
	pdec_srst_done                  : 1,	//    20
	vdo_srst_done                   : 1,	//    21
	venc                            : 1,	//    22
	vsync0_ready                    : 1,	//    23
	vsync1_ready                    : 1,	//    24
	monitor0                        : 1,	//    25
	monitor1                        : 1,	//    26
		                        : 1,	//    27 rsvd
	timer0                          : 1,	//    28
	timer1                          : 1,	//    29
	timer2                          : 1,	//    30
	timer3                          : 1;	//    31
} INTR_I_CL;

/*-----------------------------------------------------------------------------
	0x0050 bst_intr_en ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pdec0_cpb_almost_full           : 1,	//     0
	pdec0_cpb_almost_empty          : 1,	//     1
	pdec0_aub_almost_full           : 1,	//     2
	pdec0_aub_almost_empty          : 1,	//     3
	pdec0_bdrc_almost_full          : 1,	//     4
	pdec0_bdrc_almost_empty         : 1,	//     5
	pdec0_ies_almost_full           : 1,	//     6
	                                : 1,	//     7 reserved
	pdec1_cpb_almost_full           : 1,	//     8
	pdec1_cpb_almost_empty          : 1,	//     9
	pdec1_aub_almost_full           : 1,	//    10
	pdec1_aub_almost_empty          : 1,	//    11
	pdec1_bdrc_almost_full          : 1,	//    12
	pdec1_bdrc_almost_empty         : 1,	//    13
	pdec1_ies_almost_full           : 1,	//    14
	                                : 1,	//    15 reserved
	pdec2_cpb_almost_full           : 1,	//    16
	pdec2_cpb_almost_empty          : 1,	//    17
	pdec2_aub_almost_full           : 1,	//    18
	pdec2_aub_almost_empty          : 1,	//    19
	pdec2_bdrc_almost_full          : 1,	//    20
	pdec2_bdrc_almost_empty         : 1,	//    21
	pdec2_ies_almost_full           : 1,	//    22
	                                : 1,	//    23 reserved
	feed_timer				        : 1;	//    24
} BST_INTR_EN;

/*-----------------------------------------------------------------------------
	0x0054 bst_intr_st ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pdec0_cpb_almost_full           : 1,	//     0
	pdec0_cpb_almost_empty          : 1,	//     1
	pdec0_aub_almost_full           : 1,	//     2
	pdec0_aub_almost_empty          : 1,	//     3
	pdec0_bdrc_almost_full          : 1,	//     4
	pdec0_bdrc_almost_empty         : 1,	//     5
	pdec0_ies_almost_full           : 1,	//     6
	                                : 1,	//     7 reserved
	pdec1_cpb_almost_full           : 1,	//     8
	pdec1_cpb_almost_empty          : 1,	//     9
	pdec1_aub_almost_full           : 1,	//    10
	pdec1_aub_almost_empty          : 1,	//    11
	pdec1_bdrc_almost_full          : 1,	//    12
	pdec1_bdrc_almost_empty         : 1,	//    13
	pdec1_ies_almost_full           : 1,	//    14
	                                : 1,	//    15 reserved
	pdec2_cpb_almost_full           : 1,	//    16
	pdec2_cpb_almost_empty          : 1,	//    17
	pdec2_aub_almost_full           : 1,	//    18
	pdec2_aub_almost_empty          : 1,	//    19
	pdec2_bdrc_almost_full          : 1,	//    20
	pdec2_bdrc_almost_empty         : 1,	//    21
	pdec2_ies_almost_full           : 1,	//    22
	                                : 1,	//    23 reserved
	feed_timer				        : 1;	//    24
} BST_INTR_ST;

/*-----------------------------------------------------------------------------
	0x0058 bst_intr_cl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pdec0_cpb_almost_full           : 1,	//     0
	pdec0_cpb_almost_empty          : 1,	//     1
	pdec0_aub_almost_full           : 1,	//     2
	pdec0_aub_almost_empty          : 1,	//     3
	pdec0_bdrc_almost_full          : 1,	//     4
	pdec0_bdrc_almost_empty         : 1,	//     5
	pdec0_ies_almost_full           : 1,	//     6
	                                : 1,	//     7 reserved
	pdec1_cpb_almost_full           : 1,	//     8
	pdec1_cpb_almost_empty          : 1,	//     9
	pdec1_aub_almost_full           : 1,	//    10
	pdec1_aub_almost_empty          : 1,	//    11
	pdec1_bdrc_almost_full          : 1,	//    12
	pdec1_bdrc_almost_empty         : 1,	//    13
	pdec1_ies_almost_full           : 1,	//    14
	                                : 1,	//    15 reserved
	pdec2_cpb_almost_full           : 1,	//    16
	pdec2_cpb_almost_empty          : 1,	//    17
	pdec2_aub_almost_full           : 1,	//    18
	pdec2_aub_almost_empty          : 1,	//    19
	pdec2_bdrc_almost_full          : 1,	//    20
	pdec2_bdrc_almost_empty         : 1,	//    21
	pdec2_ies_almost_full           : 1,	//    22
	                                : 1,	//    23 reserved
	feed_timer				        : 1;	//    24
} BST_INTR_CL;

/*-----------------------------------------------------------------------------
	0x0060 gstc0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_gstcc0                      ;   	// 31: 0
} GSTC0;

/*-----------------------------------------------------------------------------
	0x0064 gstc1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_gstcc1                      ;   	// 31: 0
} GSTC1;

/*-----------------------------------------------------------------------------
	0x0070 mach0_srst_count ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	mach0_srst_count                ;   	// 31: 0
} MACH0_SRST_COUNT;

/*-----------------------------------------------------------------------------
	0x0074 mach1_srst_count ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	mach1_srst_count                ;   	// 31: 0
} MACH1_SRST_COUNT;

/*-----------------------------------------------------------------------------
	0x0078 pdec_srst_count ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	mach2_srst_count                ;   	// 31: 0
} PDEC_SRST_COUNT;

/*-----------------------------------------------------------------------------
	0x007c vdo_srst_count ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	mach3_srst_count                ;   	// 31: 0
} VDO_SRST_COUNT;

/*-----------------------------------------------------------------------------
	0x0080 srst_mask ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	mach0_srst_mask                 : 3,	//  0: 2
	                                : 1,	//     3 reserved
	mach1_srst_mask                 : 3,	//  4: 6
	                                : 1,	//     7 reserved
	pdec_srst_mask                  : 3,	//  8:10
	                                : 1,	//    11 reserved
	vdo_srst_mask                   : 3;	// 12:14
} SRST_MASK;

/*-----------------------------------------------------------------------------
	0x00A0 stc0_discont ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	stcc0_discont_latch				: 1,	// 0
									: 3,	// 1: 3 rsvd
	stcc0_discont					: 1,	// 4
									:26,	// 5:30 rsvd
	stcc0_en						: 1;	// 31
} STC0_DISCONT;

/*-----------------------------------------------------------------------------
	0x00A4 stc1_discont ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	stcc1_discont_latch				: 1,	// 0
									: 3,	// 1: 3 rsvd
	stcc1_discont					: 1,	// 4
									:26,	// 5:30 rsvd
	stcc1_en						: 1;	// 31
} STC1_DISCONT;

/*-----------------------------------------------------------------------------
	0x00A8 stc2_discont ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	stcc2_discont_latch				: 1,	// 0
									: 3,	// 1: 3 rsvd
	stcc2_discont					: 1,	// 4
									:26,	// 5:30 rsvd
	stcc2_en						: 1;	// 31
} STC2_DISCONT;

/*-----------------------------------------------------------------------------
	0x00AC stc3_discont ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	stcc3_discont_latch				: 1,	// 0
									: 3,	// 1: 3 rsvd
	stcc3_discont					: 1,	// 4
									:26,	// 5:30 rsvd
	stcc3_en						: 1;	// 31
} STC3_DISCONT;

/*-----------------------------------------------------------------------------
	0x00B0 feed_timer_conf ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	feed_timer						:28,	// 0: 27	timer value
									: 2,	// 28: 29 rsvd
	feed_timer_rst					: 1,	// 30
	feed_timer_enable				: 1;	// 31
} FEED_TIMER_CONF;

typedef struct {
	RC0_VERSION                     	version                         ;	// 0x0000 : ''
	SW_RESET                        	sw_reset                        ;	// 0x0004 : ''
	APB_CONF                        	apb_conf                        ;	// 0x0008 : ''
	UINT32                          	                 __rsvd_00[   1];	// 0x000c
	PDEC_IN_CONF                    	pdec_in_conf                    ;	// 0x0010 : ''
	LQ_IN_CONF                      	lq_in_conf                      ;	// 0x0014 : ''
	MACH_CONF                       	mach_conf                       ;	// 0x0018 : ''
	SYNC_CONF				sync_conf			;	// 0x001c : ''
	PDEC0_MAU_CONF                  	pdec0_mau_conf                  ;	// 0x0020 : ''
	PDEC1_MAU_CONF                  	pdec1_mau_conf                  ;	// 0x0024 : ''
	PDEC2_MAU_CONF                  	pdec2_mau_conf                  ;	// 0x0028 : ''
	PDEC3_MAU_CONF                  	pdec3_mau_conf                  ;	// 0x002c : ''
	/*INTR_E_EN*/UINT32                       	intr_e_en                       ;	// 0x0030 : ''
	/*INTR_E_ST*/UINT32                       	intr_e_st                       ;	// 0x0034 : ''
	/*INTR_E_CL*/UINT32                       	intr_e_cl                       ;	// 0x0038 : ''
	UINT32                          	                 __rsvd_01[   1];	// 0x003c
	/*INTR_I_EN*/UINT32                       	intr_i_en                       ;	// 0x0040 : ''
	/*INTR_I_ST*/UINT32                       	intr_i_st                       ;	// 0x0044 : ''
	/*INTR_I_CL*/UINT32                       	intr_i_cl                       ;	// 0x0048 : ''
	UINT32                          	                 __rsvd_02[   1];	// 0x004c
	BST_INTR_EN                     	bst_intr_en                     ;	// 0x0050 : ''
	BST_INTR_ST                     	bst_intr_st                     ;	// 0x0054 : ''
	BST_INTR_CL                     	bst_intr_cl                     ;	// 0x0058 : ''
	UINT32                          	                 __rsvd_03[   1];	// 0x005c
	GSTC0                           	gstc0                           ;	// 0x0060 : ''
	GSTC1                           	gstc1                           ;	// 0x0064 : ''
	UINT32                          	                 __rsvd_04[   2];	// 0x0068 ~ 0x006c
	MACH0_SRST_COUNT                	mach0_srst_count                ;	// 0x0070 : ''
	MACH1_SRST_COUNT                	mach1_srst_count                ;	// 0x0074 : ''
	PDEC_SRST_COUNT                 	pdec_srst_count                 ;	// 0x0078 : ''
	VDO_SRST_COUNT                  	vdo_srst_count                  ;	// 0x007c : ''
	SRST_MASK                       	srst_mask                       ;	// 0x0080 : ''
	UINT32												 __rsvd_05[	  7];	// 0x0084 ~ 0x009C
	STC0_DISCONT						stc0_discont					;	// 0x00A0 : ''
	STC1_DISCONT						stc1_discont					;	// 0x00A4 : ''
	STC2_DISCONT						stc2_discont					;	// 0x00A8	: ''
	STC3_DISCONT						stc3_discont					;	// 0x00AC : ''
	FEED_TIMER_CONF						feed_timer_conf					;	// 0x00B0 : ''
} RC0_REG_T;
/* 27 regs, 27 types */

/* 28 regs, 28 types in Total*/

/*
 * @{
 * Naming for register pointer.
 * gpRealRegRC0 : real register of RC0.
 * gpRegRC0     : shadow register.
 *
 * @def RC0_RdFL: Read  FLushing : Shadow <- Real.
 * @def RC0_WrFL: Write FLushing : Shadow -> Real.
 * @def RC0_Rd  : Read  whole register(UINT32) from Shadow register.
 * @def RC0_Wr  : Write whole register(UINT32) from Shadow register.
 * @def RC0_Rd01 ~ RC0_Rdnn: Read  given '01~nn' fields from Shadow register.
 * @def RC0_Wr01 ~ RC0_Wrnn: Write given '01~nn' fields to   Shadow register.
 * */
#define RC0_RdFL(_r)			((gpRegRC0->_r)=(gpRealRegRC0->_r))
#define RC0_WrFL(_r)			((gpRealRegRC0->_r)=(gpRegRC0->_r))

#define RC0_Rd(_r)			*((UINT32*)(&(gpRegRC0->_r)))
#define RC0_Wr(_r,_v)			((RC0_Rd(_r))=((UINT32)(_v)))

#define RC0_Rd01(_r,_f01,_v01)													\
								do { 											\
									(_v01) = (gpRegRC0->_r._f01);				\
								} while(0)

#define RC0_Rd02(_r,_f01,_v01,_f02,_v02)										\
								do { 											\
									(_v01) = (gpRegRC0->_r._f01);				\
									(_v02) = (gpRegRC0->_r._f02);				\
								} while(0)

#define RC0_Rd03(_r,_f01,_v01,_f02,_v02,_f03,_v03)								\
								do { 											\
									(_v01) = (gpRegRC0->_r._f01);				\
									(_v02) = (gpRegRC0->_r._f02);				\
									(_v03) = (gpRegRC0->_r._f03);				\
								} while(0)

#define RC0_Rd04(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04)					\
								do { 											\
									(_v01) = (gpRegRC0->_r._f01);				\
									(_v02) = (gpRegRC0->_r._f02);				\
									(_v03) = (gpRegRC0->_r._f03);				\
									(_v04) = (gpRegRC0->_r._f04);				\
								} while(0)

#define RC0_Rd05(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05)													\
								do { 											\
									(_v01) = (gpRegRC0->_r._f01);				\
									(_v02) = (gpRegRC0->_r._f02);				\
									(_v03) = (gpRegRC0->_r._f03);				\
									(_v04) = (gpRegRC0->_r._f04);				\
									(_v05) = (gpRegRC0->_r._f05);				\
								} while(0)

#define RC0_Rd06(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06)										\
								do { 											\
									(_v01) = (gpRegRC0->_r._f01);				\
									(_v02) = (gpRegRC0->_r._f02);				\
									(_v03) = (gpRegRC0->_r._f03);				\
									(_v04) = (gpRegRC0->_r._f04);				\
									(_v05) = (gpRegRC0->_r._f05);				\
									(_v06) = (gpRegRC0->_r._f06);				\
								} while(0)

#define RC0_Rd07(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07)								\
								do { 											\
									(_v01) = (gpRegRC0->_r._f01);				\
									(_v02) = (gpRegRC0->_r._f02);				\
									(_v03) = (gpRegRC0->_r._f03);				\
									(_v04) = (gpRegRC0->_r._f04);				\
									(_v05) = (gpRegRC0->_r._f05);				\
									(_v06) = (gpRegRC0->_r._f06);				\
									(_v07) = (gpRegRC0->_r._f07);				\
								} while(0)

#define RC0_Rd08(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08)					\
								do { 											\
									(_v01) = (gpRegRC0->_r._f01);				\
									(_v02) = (gpRegRC0->_r._f02);				\
									(_v03) = (gpRegRC0->_r._f03);				\
									(_v04) = (gpRegRC0->_r._f04);				\
									(_v05) = (gpRegRC0->_r._f05);				\
									(_v06) = (gpRegRC0->_r._f06);				\
									(_v07) = (gpRegRC0->_r._f07);				\
									(_v08) = (gpRegRC0->_r._f08);				\
								} while(0)

#define RC0_Rd09(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09)													\
								do { 											\
									(_v01) = (gpRegRC0->_r._f01);				\
									(_v02) = (gpRegRC0->_r._f02);				\
									(_v03) = (gpRegRC0->_r._f03);				\
									(_v04) = (gpRegRC0->_r._f04);				\
									(_v05) = (gpRegRC0->_r._f05);				\
									(_v06) = (gpRegRC0->_r._f06);				\
									(_v07) = (gpRegRC0->_r._f07);				\
									(_v08) = (gpRegRC0->_r._f08);				\
									(_v09) = (gpRegRC0->_r._f09);				\
								} while(0)

#define RC0_Rd10(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10)										\
								do { 											\
									(_v01) = (gpRegRC0->_r._f01);				\
									(_v02) = (gpRegRC0->_r._f02);				\
									(_v03) = (gpRegRC0->_r._f03);				\
									(_v04) = (gpRegRC0->_r._f04);				\
									(_v05) = (gpRegRC0->_r._f05);				\
									(_v06) = (gpRegRC0->_r._f06);				\
									(_v07) = (gpRegRC0->_r._f07);				\
									(_v08) = (gpRegRC0->_r._f08);				\
									(_v09) = (gpRegRC0->_r._f09);				\
									(_v10) = (gpRegRC0->_r._f10);				\
								} while(0)

#define RC0_Rd11(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11)								\
								do { 											\
									(_v01) = (gpRegRC0->_r._f01);				\
									(_v02) = (gpRegRC0->_r._f02);				\
									(_v03) = (gpRegRC0->_r._f03);				\
									(_v04) = (gpRegRC0->_r._f04);				\
									(_v05) = (gpRegRC0->_r._f05);				\
									(_v06) = (gpRegRC0->_r._f06);				\
									(_v07) = (gpRegRC0->_r._f07);				\
									(_v08) = (gpRegRC0->_r._f08);				\
									(_v09) = (gpRegRC0->_r._f09);				\
									(_v10) = (gpRegRC0->_r._f10);				\
									(_v11) = (gpRegRC0->_r._f11);				\
								} while(0)

#define RC0_Rd12(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12)					\
								do { 											\
									(_v01) = (gpRegRC0->_r._f01);				\
									(_v02) = (gpRegRC0->_r._f02);				\
									(_v03) = (gpRegRC0->_r._f03);				\
									(_v04) = (gpRegRC0->_r._f04);				\
									(_v05) = (gpRegRC0->_r._f05);				\
									(_v06) = (gpRegRC0->_r._f06);				\
									(_v07) = (gpRegRC0->_r._f07);				\
									(_v08) = (gpRegRC0->_r._f08);				\
									(_v09) = (gpRegRC0->_r._f09);				\
									(_v10) = (gpRegRC0->_r._f10);				\
									(_v11) = (gpRegRC0->_r._f11);				\
									(_v12) = (gpRegRC0->_r._f12);				\
								} while(0)

#define RC0_Rd13(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13)													\
								do { 											\
									(_v01) = (gpRegRC0->_r._f01);				\
									(_v02) = (gpRegRC0->_r._f02);				\
									(_v03) = (gpRegRC0->_r._f03);				\
									(_v04) = (gpRegRC0->_r._f04);				\
									(_v05) = (gpRegRC0->_r._f05);				\
									(_v06) = (gpRegRC0->_r._f06);				\
									(_v07) = (gpRegRC0->_r._f07);				\
									(_v08) = (gpRegRC0->_r._f08);				\
									(_v09) = (gpRegRC0->_r._f09);				\
									(_v10) = (gpRegRC0->_r._f10);				\
									(_v11) = (gpRegRC0->_r._f11);				\
									(_v12) = (gpRegRC0->_r._f12);				\
									(_v13) = (gpRegRC0->_r._f13);				\
								} while(0)

#define RC0_Rd14(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14)										\
								do { 											\
									(_v01) = (gpRegRC0->_r._f01);				\
									(_v02) = (gpRegRC0->_r._f02);				\
									(_v03) = (gpRegRC0->_r._f03);				\
									(_v04) = (gpRegRC0->_r._f04);				\
									(_v05) = (gpRegRC0->_r._f05);				\
									(_v06) = (gpRegRC0->_r._f06);				\
									(_v07) = (gpRegRC0->_r._f07);				\
									(_v08) = (gpRegRC0->_r._f08);				\
									(_v09) = (gpRegRC0->_r._f09);				\
									(_v10) = (gpRegRC0->_r._f10);				\
									(_v11) = (gpRegRC0->_r._f11);				\
									(_v12) = (gpRegRC0->_r._f12);				\
									(_v13) = (gpRegRC0->_r._f13);				\
									(_v14) = (gpRegRC0->_r._f14);				\
								} while(0)

#define RC0_Rd15(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15)								\
								do { 											\
									(_v01) = (gpRegRC0->_r._f01);				\
									(_v02) = (gpRegRC0->_r._f02);				\
									(_v03) = (gpRegRC0->_r._f03);				\
									(_v04) = (gpRegRC0->_r._f04);				\
									(_v05) = (gpRegRC0->_r._f05);				\
									(_v06) = (gpRegRC0->_r._f06);				\
									(_v07) = (gpRegRC0->_r._f07);				\
									(_v08) = (gpRegRC0->_r._f08);				\
									(_v09) = (gpRegRC0->_r._f09);				\
									(_v10) = (gpRegRC0->_r._f10);				\
									(_v11) = (gpRegRC0->_r._f11);				\
									(_v12) = (gpRegRC0->_r._f12);				\
									(_v13) = (gpRegRC0->_r._f13);				\
									(_v14) = (gpRegRC0->_r._f14);				\
									(_v15) = (gpRegRC0->_r._f15);				\
								} while(0)

#define RC0_Rd16(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15,_f16,_v16)					\
								do { 											\
									(_v01) = (gpRegRC0->_r._f01);				\
									(_v02) = (gpRegRC0->_r._f02);				\
									(_v03) = (gpRegRC0->_r._f03);				\
									(_v04) = (gpRegRC0->_r._f04);				\
									(_v05) = (gpRegRC0->_r._f05);				\
									(_v06) = (gpRegRC0->_r._f06);				\
									(_v07) = (gpRegRC0->_r._f07);				\
									(_v08) = (gpRegRC0->_r._f08);				\
									(_v09) = (gpRegRC0->_r._f09);				\
									(_v10) = (gpRegRC0->_r._f10);				\
									(_v11) = (gpRegRC0->_r._f11);				\
									(_v12) = (gpRegRC0->_r._f12);				\
									(_v13) = (gpRegRC0->_r._f13);				\
									(_v14) = (gpRegRC0->_r._f14);				\
									(_v15) = (gpRegRC0->_r._f15);				\
									(_v16) = (gpRegRC0->_r._f16);				\
								} while(0)


#define RC0_Wr01(_r,_f01,_v01)													\
								do { 											\
									(gpRegRC0->_r._f01) = (_v01);				\
								} while(0)

#define RC0_Wr02(_r,_f01,_v01,_f02,_v02)										\
								do { 											\
									(gpRegRC0->_r._f01) = (_v01);				\
									(gpRegRC0->_r._f02) = (_v02);				\
								} while(0)

#define RC0_Wr03(_r,_f01,_v01,_f02,_v02,_f03,_v03)								\
								do { 											\
									(gpRegRC0->_r._f01) = (_v01);				\
									(gpRegRC0->_r._f02) = (_v02);				\
									(gpRegRC0->_r._f03) = (_v03);				\
								} while(0)

#define RC0_Wr04(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04)					\
								do { 											\
									(gpRegRC0->_r._f01) = (_v01);				\
									(gpRegRC0->_r._f02) = (_v02);				\
									(gpRegRC0->_r._f03) = (_v03);				\
									(gpRegRC0->_r._f04) = (_v04);				\
								} while(0)

#define RC0_Wr05(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05)													\
								do { 											\
									(gpRegRC0->_r._f01) = (_v01);				\
									(gpRegRC0->_r._f02) = (_v02);				\
									(gpRegRC0->_r._f03) = (_v03);				\
									(gpRegRC0->_r._f04) = (_v04);				\
									(gpRegRC0->_r._f05) = (_v05);				\
								} while(0)

#define RC0_Wr06(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06)										\
								do { 											\
									(gpRegRC0->_r._f01) = (_v01);				\
									(gpRegRC0->_r._f02) = (_v02);				\
									(gpRegRC0->_r._f03) = (_v03);				\
									(gpRegRC0->_r._f04) = (_v04);				\
									(gpRegRC0->_r._f05) = (_v05);				\
									(gpRegRC0->_r._f06) = (_v06);				\
								} while(0)

#define RC0_Wr07(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07)								\
								do { 											\
									(gpRegRC0->_r._f01) = (_v01);				\
									(gpRegRC0->_r._f02) = (_v02);				\
									(gpRegRC0->_r._f03) = (_v03);				\
									(gpRegRC0->_r._f04) = (_v04);				\
									(gpRegRC0->_r._f05) = (_v05);				\
									(gpRegRC0->_r._f06) = (_v06);				\
									(gpRegRC0->_r._f07) = (_v07);				\
								} while(0)

#define RC0_Wr08(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08)					\
								do { 											\
									(gpRegRC0->_r._f01) = (_v01);				\
									(gpRegRC0->_r._f02) = (_v02);				\
									(gpRegRC0->_r._f03) = (_v03);				\
									(gpRegRC0->_r._f04) = (_v04);				\
									(gpRegRC0->_r._f05) = (_v05);				\
									(gpRegRC0->_r._f06) = (_v06);				\
									(gpRegRC0->_r._f07) = (_v07);				\
									(gpRegRC0->_r._f08) = (_v08);				\
								} while(0)

#define RC0_Wr09(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09)													\
								do { 											\
									(gpRegRC0->_r._f01) = (_v01);				\
									(gpRegRC0->_r._f02) = (_v02);				\
									(gpRegRC0->_r._f03) = (_v03);				\
									(gpRegRC0->_r._f04) = (_v04);				\
									(gpRegRC0->_r._f05) = (_v05);				\
									(gpRegRC0->_r._f06) = (_v06);				\
									(gpRegRC0->_r._f07) = (_v07);				\
									(gpRegRC0->_r._f08) = (_v08);				\
									(gpRegRC0->_r._f09) = (_v09);				\
								} while(0)

#define RC0_Wr10(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10)										\
								do { 											\
									(gpRegRC0->_r._f01) = (_v01);				\
									(gpRegRC0->_r._f02) = (_v02);				\
									(gpRegRC0->_r._f03) = (_v03);				\
									(gpRegRC0->_r._f04) = (_v04);				\
									(gpRegRC0->_r._f05) = (_v05);				\
									(gpRegRC0->_r._f06) = (_v06);				\
									(gpRegRC0->_r._f07) = (_v07);				\
									(gpRegRC0->_r._f08) = (_v08);				\
									(gpRegRC0->_r._f09) = (_v09);				\
									(gpRegRC0->_r._f10) = (_v10);				\
								} while(0)

#define RC0_Wr11(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11)								\
								do { 											\
									(gpRegRC0->_r._f01) = (_v01);				\
									(gpRegRC0->_r._f02) = (_v02);				\
									(gpRegRC0->_r._f03) = (_v03);				\
									(gpRegRC0->_r._f04) = (_v04);				\
									(gpRegRC0->_r._f05) = (_v05);				\
									(gpRegRC0->_r._f06) = (_v06);				\
									(gpRegRC0->_r._f07) = (_v07);				\
									(gpRegRC0->_r._f08) = (_v08);				\
									(gpRegRC0->_r._f09) = (_v09);				\
									(gpRegRC0->_r._f10) = (_v10);				\
									(gpRegRC0->_r._f11) = (_v11);				\
								} while(0)

#define RC0_Wr12(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12)					\
								do { 											\
									(gpRegRC0->_r._f01) = (_v01);				\
									(gpRegRC0->_r._f02) = (_v02);				\
									(gpRegRC0->_r._f03) = (_v03);				\
									(gpRegRC0->_r._f04) = (_v04);				\
									(gpRegRC0->_r._f05) = (_v05);				\
									(gpRegRC0->_r._f06) = (_v06);				\
									(gpRegRC0->_r._f07) = (_v07);				\
									(gpRegRC0->_r._f08) = (_v08);				\
									(gpRegRC0->_r._f09) = (_v09);				\
									(gpRegRC0->_r._f10) = (_v10);				\
									(gpRegRC0->_r._f11) = (_v11);				\
									(gpRegRC0->_r._f12) = (_v12);				\
								} while(0)

#define RC0_Wr13(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13)													\
								do { 											\
									(gpRegRC0->_r._f01) = (_v01);				\
									(gpRegRC0->_r._f02) = (_v02);				\
									(gpRegRC0->_r._f03) = (_v03);				\
									(gpRegRC0->_r._f04) = (_v04);				\
									(gpRegRC0->_r._f05) = (_v05);				\
									(gpRegRC0->_r._f06) = (_v06);				\
									(gpRegRC0->_r._f07) = (_v07);				\
									(gpRegRC0->_r._f08) = (_v08);				\
									(gpRegRC0->_r._f09) = (_v09);				\
									(gpRegRC0->_r._f10) = (_v10);				\
									(gpRegRC0->_r._f11) = (_v11);				\
									(gpRegRC0->_r._f12) = (_v12);				\
									(gpRegRC0->_r._f13) = (_v13);				\
								} while(0)

#define RC0_Wr14(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14)										\
								do { 											\
									(gpRegRC0->_r._f01) = (_v01);				\
									(gpRegRC0->_r._f02) = (_v02);				\
									(gpRegRC0->_r._f03) = (_v03);				\
									(gpRegRC0->_r._f04) = (_v04);				\
									(gpRegRC0->_r._f05) = (_v05);				\
									(gpRegRC0->_r._f06) = (_v06);				\
									(gpRegRC0->_r._f07) = (_v07);				\
									(gpRegRC0->_r._f08) = (_v08);				\
									(gpRegRC0->_r._f09) = (_v09);				\
									(gpRegRC0->_r._f10) = (_v10);				\
									(gpRegRC0->_r._f11) = (_v11);				\
									(gpRegRC0->_r._f12) = (_v12);				\
									(gpRegRC0->_r._f13) = (_v13);				\
									(gpRegRC0->_r._f14) = (_v14);				\
								} while(0)

#define RC0_Wr15(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15)								\
								do { 											\
									(gpRegRC0->_r._f01) = (_v01);				\
									(gpRegRC0->_r._f02) = (_v02);				\
									(gpRegRC0->_r._f03) = (_v03);				\
									(gpRegRC0->_r._f04) = (_v04);				\
									(gpRegRC0->_r._f05) = (_v05);				\
									(gpRegRC0->_r._f06) = (_v06);				\
									(gpRegRC0->_r._f07) = (_v07);				\
									(gpRegRC0->_r._f08) = (_v08);				\
									(gpRegRC0->_r._f09) = (_v09);				\
									(gpRegRC0->_r._f10) = (_v10);				\
									(gpRegRC0->_r._f11) = (_v11);				\
									(gpRegRC0->_r._f12) = (_v12);				\
									(gpRegRC0->_r._f13) = (_v13);				\
									(gpRegRC0->_r._f14) = (_v14);				\
									(gpRegRC0->_r._f15) = (_v15);				\
								} while(0)

#define RC0_Wr16(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15,_f16,_v16)					\
								do { 											\
									(gpRegRC0->_r._f01) = (_v01);				\
									(gpRegRC0->_r._f02) = (_v02);				\
									(gpRegRC0->_r._f03) = (_v03);				\
									(gpRegRC0->_r._f04) = (_v04);				\
									(gpRegRC0->_r._f05) = (_v05);				\
									(gpRegRC0->_r._f06) = (_v06);				\
									(gpRegRC0->_r._f07) = (_v07);				\
									(gpRegRC0->_r._f08) = (_v08);				\
									(gpRegRC0->_r._f09) = (_v09);				\
									(gpRegRC0->_r._f10) = (_v10);				\
									(gpRegRC0->_r._f11) = (_v11);				\
									(gpRegRC0->_r._f12) = (_v12);				\
									(gpRegRC0->_r._f13) = (_v13);				\
									(gpRegRC0->_r._f14) = (_v14);				\
									(gpRegRC0->_r._f15) = (_v15);				\
									(gpRegRC0->_r._f16) = (_v16);				\
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
 * _rwname: name of rw    bit field : shall be 0 after RC0_Wind(), 1 for RC0_Rind()
 * _iname : name of index bit field
 * _ival  : index value
 * _fname : field name
 * _fval  : field variable that field value shall be stored.
 *
 * RC0_Rind : General indexed register Read.(
 * RC0_Wind : General indexed register Read.
 *
 * RC0_Ridx : For 'index', 'rw', 'load' field name
 * RC0_Widx : For 'index', 'rw', 'load' field name and NO_LOAD.
 */
#define RC0_Rind(_r, _lname, _rwname, _iname, _ival, _fname, _fval)				\
							do {												\
								RC0_Wr03(_r,_lname,0,_rwname,1,_iname,_ival);	\
								RC0_WrFL(_r);									\
								RC0_RdFL(_r);									\
								RC0_Rd01(_r,_fname,_fval);						\
							} while (0)

#define RC0_Wind(_r, _lname, _rwname, _iname, _ival, _fname, _fval)				\
				RC0_Wr04(_r, _lname,0, _rwname,0, _iname,_ival, _fname,_fval)


#define RC0_Ridx(_r, _ival, _fname, _fval)	RC0_Rind(_r,load,rw,index,_ival,_fname,_fval)

#define RC0_Widx(_r, _ival, _fname, _fval)	RC0_Wind(_r,load,rw,index,_ival,_fname,_fval)

/** @} *//* end of macro documentation */

#define gpRealRegRC0		stpRC0_Reg
#define gpRegRC0			stpRC0_RegShadow

#ifdef __cplusplus
}
#endif

#endif	/* _RC0_REG_H_ */
