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

#ifndef _REG_FRC_M14B0_h
#define _REG_FRC_M14B0_h

/*
	FRC_DVO
*/
/*-----------------------------------------------------------------------------
	0x0000 dvo_sreset0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvom_cmd_mif_sreset             : 1,	//     0
	dvom_req_mif_c_sreset           : 1,	//     1
	dvom_req_mif_y_sreset           : 1,	//     2
	dvom_rhost_c_sreset             : 1,	//     3
	dvom_rhost_y_sreset             : 1,	//     4
	dvom_mif_mv_sreset              : 1,	//     5
	_rsvd_00                                : 1,	//     6 reserved
	dvom_hsc_c_sreset               : 1,	//     7
	dvom_hsc_y_sreset               : 1,	//     8
	_rsvd_01                                : 1,	//     9 reserved
	dvom_cdc_sreset                 : 1,	//    10
	_rsvd_02                                : 5,	// 11:15 reserved
	dvos_cmd_mif_sreset             : 1,	//    16
	dvos_req_mif_c_sreset           : 1,	//    17
	dvos_req_mif_y_sreset           : 1,	//    18
	dvos_rhost_c_sreset             : 1,	//    19
	dvos_rhost_y_sreset             : 1,	//    20
	_rsvd_03                                : 5,	// 21:25 reserved
	dvos_cdc_sreset                 : 1;	//    26
} M14B0_FRC_DVO_SRESET0;

/*-----------------------------------------------------------------------------
	0x0004 dvo_sreset1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvom_mdo_sreset                 : 1,	//     0
	dvom_msg_sreset                 : 1,	//     1
	dvos_ssg0_sreset                : 1,	//     2
	dvos_ssg1_sreset                : 1,	//     3
	dvos_ssg2_sreset                : 1,	//     4
	_rsvd_00                                : 3,	//  5: 7 reserved
	dbg_dvom_sync_mux               : 3,	//  8:10
	_rsvd_01                                : 1,	//    11 reserved
	dbg_dvom_data_mux               : 3;	// 12:14
} M14B0_FRC_DVO_SRESET1;

/*-----------------------------------------------------------------------------
	0x0008 dvo_pic_init ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvom_pic_init_m                 : 1,	//     0
	_rsvd_00                                : 2,	//  1: 2 reserved
	dvom_pic_init_auto_m            : 1,	//     3
	dvos_pic_init_m                 : 1,	//     4
	_rsvd_01                                : 2,	//  5: 6 reserved
	dvos_pic_init_auto_m            : 1,	//     7
	dvom_pic_init_m_mif_mask        : 1,	//     8
	dvom_pic_init_m_hsc_mask        : 1,	//     9
	dvom_pic_init_m_cdc_mask        : 1,	//    10
	_rsvd_02                                : 1,	//    11 reserved
	dvos_pic_init_m_mif_mask        : 1,	//    12
	dvos_pic_init_m_cdc_mask        : 1;	//    13
} M14B0_FRC_DVO_PIC_INIT;

/*-----------------------------------------------------------------------------
	0x000c dvo_pic_start ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvom_pic_start_m                : 1,	//     0
	_rsvd_00                                : 3,	//  1: 3 reserved
	dvos_pic_start_m                : 1,	//     4
	_rsvd_01                                : 3,	//  5: 7 reserved
	dvom_pic_start_mif_mask         : 1,	//     8
	dvom_pic_start_hsc_mask         : 1,	//     9
	dvom_pic_start_cdc_mask         : 1,	//    10
	_rsvd_02                                : 1,	//    11 reserved
	dvos_pic_start_mif_mask         : 1,	//    12
	dvos_pic_start_cdc_mask         : 1;	//    13
} M14B0_FRC_DVO_PIC_START;

/*-----------------------------------------------------------------------------
	0x0010 dvom_pulse_m_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvom_pulse_src_m_timer          :28,	//  0:27
	dvom_pulse_src_m_sel            : 1,	//    28
	dvom_pulse_src_m_inv            : 1;	//    29
} M14B0_FRC_DVOM_PULSE_M_CTRL;

/*-----------------------------------------------------------------------------
	0x0014 dvos_pulse_m_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvos_pulse_src_m_timer          :28,	//  0:27
	dvos_pulse_src_m_sel            : 1,	//    28
	dvos_pulse_src_m_inv            : 1;	//    29
} M14B0_FRC_DVOS_PULSE_M_CTRL;

/*-----------------------------------------------------------------------------
	0x0018 dvom_pic_init_m_timer ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvom_pic_init_m_timer           :28;	//  0:27
} M14B0_FRC_DVOM_PIC_INIT_M_TIMER;

/*-----------------------------------------------------------------------------
	0x001c dvos_pic_init_m_timer ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvos_pic_init_m_timer           :28;	//  0:27
} M14B0_FRC_DVOS_PIC_INIT_M_TIMER;

/*-----------------------------------------------------------------------------
	0x0028 dvo_pic_end_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvom_pic_end_m_en               : 1,	//     0
	dvos_pic_end_m_en               : 1,	//     1
	dvom_pic_end_m_force            : 1,	//     2
	dvos_pic_end_m_force            : 1,	//     3
	dvom_pic_end_m_force_en         : 1,	//     4
	dvos_pic_end_m_force_en         : 1;	//     5
} M14B0_FRC_DVO_PIC_END_CTRL;

/*-----------------------------------------------------------------------------
	0x002c dvo_pic_end_info ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvom_pic_end_m                  : 4,	//  0: 3
	dvos_pic_end_m                  : 4;	//  4: 7
} M14B0_FRC_DVO_PIC_END_INFO;

/*-----------------------------------------------------------------------------
	0x0030 dvom_gmau_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	gmau_cmd_dly_cnt                :10,	//  0: 9
	_rsvd_00                                : 6,	// 10:15 reserved
	gmau_timeout_cnt                : 4,	// 16:19
	gmau_cmd_priority2              : 4,	// 20:23
	gmau_cmd_priority1              : 4,	// 24:27
	_rsvd_01                                : 2,	// 28:29 reserved
	gmau_flush_en                   : 1,	//    30
	gmau_arb_clear                  : 1;	//    31
} M14B0_FRC_DVOM_GMAU_CTRL;

/*-----------------------------------------------------------------------------
	0x0034 dvos_gmau_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	gmau_cmd_dly_cnt                :10,	//  0: 9
	_rsvd_00                                : 6,	// 10:15 reserved
	gmau_timeout_cnt                : 4,	// 16:19
	gmau_cmd_priority2              : 4,	// 20:23
	gmau_cmd_priority1              : 4,	// 24:27
	_rsvd_01                                : 2,	// 28:29 reserved
	gmau_flush_en                   : 1;	//    30
} M14B0_FRC_DVOS_GMAU_CTRL;

/*-----------------------------------------------------------------------------
	0x0038 dvo_rst_async_bridge ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	rst_async_bridge_ctrl           :16;	//  0:15
} M14B0_FRC_DVO_RST_ASYNC_BRIDGE;

/*-----------------------------------------------------------------------------
	0x003c dvom_intr_mask ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvom_intr0_intr_mask            : 1,	//     0
	dvom_intr1_intr_mask            : 1,	//     1
	dvom_intr2_intr_mask            : 1,	//     2
	dvom_intr3_intr_mask            : 1,	//     3
	dvom_y_underflow_intr_mask      : 1,	//     4
	dvom_c_underflow_intr_mask      : 1,	//     5
	dvom_vfall0_intr_mask           : 1,	//     6
	dvom_vfall1_intr_mask           : 1,	//     7
	dvom_vfall2_intr_mask           : 1,	//     8
	dvom_pic_end_intr_mask          : 1;	//     9
} M14B0_FRC_DVOM_INTR_MASK;

/*-----------------------------------------------------------------------------
	0x0040 dvom_intr_clear ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvom_intr0_clear                : 1,	//     0
	dvom_intr1_clear                : 1,	//     1
	dvom_intr2_clear                : 1,	//     2
	dvom_intr3_clear                : 1,	//     3
	dvom_y_underflow_intr_clear     : 1,	//     4
	dvom_c_underflow_intr_clear     : 1,	//     5
	dvom_vfall0_intr_clear          : 1,	//     6
	dvom_vfall1_intr_clear          : 1,	//     7
	dvom_vfall2_intr_clear          : 1,	//     8
	dvom_pic_end_intr_clear         : 1;	//     9
} M14B0_FRC_DVOM_INTR_CLEAR;

/*-----------------------------------------------------------------------------
	0x0044 dvom_intr_status ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvom_intr0_status               : 1,	//     0
	dvom_intr1_status               : 1,	//     1
	dvom_intr2_status               : 1,	//     2
	dvom_intr3_status               : 1,	//     3
	dvom_y_underflow_intr_status    : 1,	//     4
	dvom_c_underflow_intr_status    : 1,	//     5
	dvom_vfall0_intr_status         : 1,	//     6
	dvom_vfall1_intr_status         : 1,	//     7
	dvom_vfall2_intr_status         : 1,	//     8
	dvom_pic_end_intr_status        : 1;	//     9
} M14B0_FRC_DVOM_INTR_STATUS;

/*-----------------------------------------------------------------------------
	0x0048 dvos_intr_mask ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	_rsvd_00                                : 4,	//  0: 3 reserved
	dvos_y_underflow_intr_mask      : 1,	//     4
	dvos_c_underflow_intr_mask      : 1,	//     5
	dvos_vfall0_intr_mask           : 1,	//     6
	_rsvd_01                                : 2,	//  7: 8 reserved
	dvos_pic_end_intr_mask          : 1;	//     9
} M14B0_FRC_DVOS_INTR_MASK;

/*-----------------------------------------------------------------------------
	0x004c dvos_intr_clear ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	_rsvd_00                                : 4,	//  0: 3 reserved
	dvos_y_underflow_intr_clear     : 1,	//     4
	dvos_c_underflow_intr_clear     : 1,	//     5
	dvos_vfall0_intr_clear          : 1,	//     6
	_rsvd_01                                : 2,	//  7: 8 reserved
	dvos_pic_end_intr_clear         : 1;	//     9
} M14B0_FRC_DVOS_INTR_CLEAR;

/*-----------------------------------------------------------------------------
	0x0050 dvos_intr_status ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	_rsvd_00                                : 4,	//  0: 3 reserved
	dvos_y_underflow_intr_status    : 1,	//     4
	dvos_c_underflow_intr_status    : 1,	//     5
	dvos_vfall0_intr_status         : 1,	//     6
	_rsvd_01                                : 2,	//  7: 8 reserved
	dvos_pic_end_intr_status        : 1;	//     9
} M14B0_FRC_DVOS_INTR_STATUS;

/*-----------------------------------------------------------------------------
	0x0054 dvom_intr0_pos ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvom_intr0_vpos                 :13,	//  0:12
	_rsvd                                : 3,	// 13:15 reserved
	dvom_intr0_hpos                 :13;	// 16:28
} M14B0_FRC_DVOM_INTR0_POS;

/*-----------------------------------------------------------------------------
	0x0058 dvom_intr1_pos ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvom_intr1_vpos                 :13,	//  0:12
	_rsvd                                : 3,	// 13:15 reserved
	dvom_intr1_hpos                 :13;	// 16:28
} M14B0_FRC_DVOM_INTR1_POS;

/*-----------------------------------------------------------------------------
	0x005c dvom_intr2_pos ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvom_intr2_vpos                 :13,	//  0:12
	_rsvd                                : 3,	// 13:15 reserved
	dvom_intr2_hpos                 :13;	// 16:28
} M14B0_FRC_DVOM_INTR2_POS;

/*-----------------------------------------------------------------------------
	0x0060 dvom_intr3_pos ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvom_intr3_vpos                 :13,	//  0:12
	_rsvd                                : 3,	// 13:15 reserved
	dvom_intr3_hpos                 :13;	// 16:28
} M14B0_FRC_DVOM_INTR3_POS;

/*-----------------------------------------------------------------------------
	0x006c dvom_frame_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvom_frame_b_cnt                : 4,	//  0: 3
	dvom_frame_a_cnt                : 4,	//  4: 7
	dvom_ld_ab_cnt                  : 1,	//     8
	dvom_frame_ctrl_en              : 1;	//     9
} M14B0_FRC_DVOM_FRAME_CTRL;

/*-----------------------------------------------------------------------------
	0x0070 dvos_frame_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvos_frame_b_cnt                : 4,	//  0: 3
	dvos_frame_a_cnt                : 4,	//  4: 7
	dvos_ld_ab_cnt                  : 1,	//     8
	dvos_frame_ctrl_en              : 1;	//     9
} M14B0_FRC_DVOS_FRAME_CTRL;

/*-----------------------------------------------------------------------------
	0x0074 dvom_status ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	cdc_ack_s_b                     : 1,	//     0
	cdc_ack_s_y                     : 1,	//     1
	cdc_val_b_i1                    : 1,	//     2
	cdc_val_y_i1                    : 1,	//     3
	gp2s_b_rxbusy                   : 1,	//     4
	gp2s_y_rxbusy                   : 1,	//     5
	cdc_val_b_i0                    : 1,	//     6
	cdc_val_y_i0                    : 1,	//     7
	lvl_pic_end_dvom_c              : 1,	//     8
	lvl_pic_end_dvom_y              : 1,	//     9
	_rsvd_00                                : 2,	// 10:11 reserved
	lvl_dvom_rready                 : 1,	//    12
	lvl_dvom_rvalid                 : 1,	//    13
	lvl_dvom_arready                : 1,	//    14
	lvl_dvom_arvalid                : 1,	//    15
	lvl_pic_init_dvom_m             : 1,	//    16
	_rsvd_01                                : 1,	//    17 reserved
	lvl_dvom1_cmd_val               : 1,	//    18
	lvl_dvom1_cmd_ack               : 1,	//    19
	dvom_rd                         : 1,	//    20
	dvom_gmau_flush_done            : 1,	//    21
	dvom_gmau_flush_en              : 1;	//    22
} M14B0_FRC_DVOM_STATUS;

/*-----------------------------------------------------------------------------
	0x0078 dvos_status ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	cdc_ack_s_b                     : 1,	//     0
	cdc_ack_s_y                     : 1,	//     1
	cdc_val_b_i1                    : 1,	//     2
	cdc_val_y_i1                    : 1,	//     3
	_rsvd_00                                : 4,	//  4: 7 reserved
	lvl_pic_end_dvos_c              : 1,	//     8
	lvl_pic_end_dvos_y              : 1,	//     9
	_rsvd_01                                : 2,	// 10:11 reserved
	lvl_dvos_rready                 : 1,	//    12
	lvl_dvos_rvalid                 : 1,	//    13
	lvl_dvos_arready                : 1,	//    14
	lvl_dvos_arvalid                : 1,	//    15
	lvl_pic_init_dvos_m             : 1,	//    16
	_rsvd_02                                : 1,	//    17 reserved
	lvl_dvos_cmd_val                : 1,	//    18
	lvl_dvos_cmd_ack                : 1,	//    19
	dvos_rd                         : 1,	//    20
	dvos_gmau_flush_done            : 1,	//    21
	dvos_gmau_flush_en              : 1;	//    22
} M14B0_FRC_DVOS_STATUS;

/*-----------------------------------------------------------------------------
	0x0080 dvom_sync0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvom_vsync                      :13,	//  0:12
	_rsvd                                : 3,	// 13:15 reserved
	dvom_vfp                        :13;	// 16:28
} M14B0_FRC_DVOM_SYNC0;

/*-----------------------------------------------------------------------------
	0x0084 dvom_sync1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvom_va                         :13,	//  0:12
	_rsvd                                : 3,	// 13:15 reserved
	dvom_vbp                        :13;	// 16:28
} M14B0_FRC_DVOM_SYNC1;

/*-----------------------------------------------------------------------------
	0x0088 dvom_sync2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvom_hsync                      :13,	//  0:12
	_rsvd                                : 3,	// 13:15 reserved
	dvom_hfp                        :13;	// 16:28
} M14B0_FRC_DVOM_SYNC2;

/*-----------------------------------------------------------------------------
	0x008c dvom_sync3 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvom_ha                         :13,	//  0:12
	_rsvd                                : 3,	// 13:15 reserved
	dvom_hbp                        :13;	// 16:28
} M14B0_FRC_DVOM_SYNC3;

/*-----------------------------------------------------------------------------
	0x0090 dvos0_sync0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvos0_vsync                     :13,	//  0:12
	_rsvd                                : 3,	// 13:15 reserved
	dvos0_vfp                       :13;	// 16:28
} M14B0_FRC_DVOS0_SYNC0;

/*-----------------------------------------------------------------------------
	0x0094 dvos0_sync1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvos0_va                        :13,	//  0:12
	_rsvd                                : 3,	// 13:15 reserved
	dvos0_vbp                       :13;	// 16:28
} M14B0_FRC_DVOS0_SYNC1;

/*-----------------------------------------------------------------------------
	0x0098 dvos0_sync2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvos0_hsync                     :13,	//  0:12
	_rsvd                                : 3,	// 13:15 reserved
	dvos0_hfp                       :13;	// 16:28
} M14B0_FRC_DVOS0_SYNC2;

/*-----------------------------------------------------------------------------
	0x009c dvos0_sync3 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvos0_ha                        :13,	//  0:12
	_rsvd                                : 3,	// 13:15 reserved
	dvos0_hbp                       :13;	// 16:28
} M14B0_FRC_DVOS0_SYNC3;

/*-----------------------------------------------------------------------------
	0x00a0 dvos1_sync0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvos1_vsync                     :13,	//  0:12
	_rsvd                                : 3,	// 13:15 reserved
	dvos1_vfp                       :13;	// 16:28
} M14B0_FRC_DVOS1_SYNC0;

/*-----------------------------------------------------------------------------
	0x00a4 dvos1_sync1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvos1_va                        :13,	//  0:12
	_rsvd                                : 3,	// 13:15 reserved
	dvos1_vbp                       :13;	// 16:28
} M14B0_FRC_DVOS1_SYNC1;

/*-----------------------------------------------------------------------------
	0x00a8 dvos1_sync2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvos1_hsync                     :13,	//  0:12
	_rsvd                                : 3,	// 13:15 reserved
	dvos1_hfp                       :13;	// 16:28
} M14B0_FRC_DVOS1_SYNC2;

/*-----------------------------------------------------------------------------
	0x00ac dvos1_sync3 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvos1_ha                        :13,	//  0:12
	_rsvd                                : 3,	// 13:15 reserved
	dvos1_hbp                       :13;	// 16:28
} M14B0_FRC_DVOS1_SYNC3;

/*-----------------------------------------------------------------------------
	0x00b0 dvos2_sync0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvos_vsync                      :13,	//  0:12
	_rsvd                                : 3,	// 13:15 reserved
	dvos2_vfp                       :13;	// 16:28
} M14B0_FRC_DVOS2_SYNC0;

/*-----------------------------------------------------------------------------
	0x00b4 dvos2_sync1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvos2_va                        :13,	//  0:12
	_rsvd                                : 3,	// 13:15 reserved
	dvos2_vbp                       :13;	// 16:28
} M14B0_FRC_DVOS2_SYNC1;

/*-----------------------------------------------------------------------------
	0x00b8 dvos2_sync2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvos2_hsync                     :13,	//  0:12
	_rsvd                                : 3,	// 13:15 reserved
	dvos2_hfp                       :13;	// 16:28
} M14B0_FRC_DVOS2_SYNC2;

/*-----------------------------------------------------------------------------
	0x00bc dvos2_sync3 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvos2_ha                        :13,	//  0:12
	_rsvd                                : 3,	// 13:15 reserved
	dvos2_hbp                       :13;	// 16:28
} M14B0_FRC_DVOS2_SYNC3;

/*-----------------------------------------------------------------------------
	0x00c0 dvo_msg_ctrl0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	line_start_hpos                 :13,	//  0:12
	_rsvd_00                                :10,	// 13:22 reserved
	disp_pulse_sel                  : 1,	//    23
	sync_o_inv_sel                  : 1,	//    24
	_rsvd_01                                : 1,	//    25 reserved
	force_update_src_sel            : 2,	// 26:27
	_rsvd_02                                : 2,	// 28:29 reserved
	force_update_auto_en            : 1,	//    30
	force_update                    : 1;	//    31
} M14B0_FRC_DVO_MSG_CTRL0;

/*-----------------------------------------------------------------------------
	0x00c4 dvo_msg_ctrl1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	lr3d_sg_hpos                    :13,	//  0:12
	_rsvd_00                                : 2,	// 13:14 reserved
	lr3d_sg_sel                     : 1,	//    15
	lr3d_sg_vpos                    :13,	// 16:28
	_rsvd_01                                : 2,	// 29:30 reserved
	lr3d_sg_en                      : 1;	//    31
} M14B0_FRC_DVO_MSG_CTRL1;

/*-----------------------------------------------------------------------------
	0x00c8 dvo_msg_ctrl2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	lr3d_hpos                       :13,	//  0:12
	_rsvd_00                                : 1,	//    13 reserved
	lr3d_mode_sel                   : 1,	//    14
	lr3d_sel                        : 1,	//    15
	lr3d_vpos                       :13,	// 16:28
	_rsvd_01                                : 2,	// 29:30 reserved
	lr3d_en                         : 1;	//    31
} M14B0_FRC_DVO_MSG_CTRL2;

/*-----------------------------------------------------------------------------
	0x00cc dvo_msg_ctrl3 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	msg_update_src_dly_ctrl         :28;	//  0:27
} M14B0_FRC_DVO_MSG_CTRL3;

/*-----------------------------------------------------------------------------
	0x00d0 dvo_ssg0_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	line_start_hpos                 :13,	//  0:12
	_rsvd_00                                : 3,	// 13:15 reserved
	sync_i_hs_ha_sel                : 1,	//    16
	sync_i_vs_va_sel                : 1,	//    17
	sync_i_hs_inv_sel               : 1,	//    18
	sync_i_vs_inv_sel               : 1,	//    19
	sync_o_ha_dly_sel               : 1,	//    20
	sync_o_hs_dly_sel               : 1,	//    21
	sync_o_va_dly_sel               : 1,	//    22
	sync_o_vs_dly_sel               : 1,	//    23
	tp_update_hv_zero               : 1,	//    24
	disp_pulse_sel                  : 1,	//    25
	force_update_src_sel            : 2,	// 26:27
	_rsvd_01                                : 2,	// 28:29 reserved
	force_update_auto_en            : 1,	//    30
	force_update                    : 1;	//    31
} M14B0_FRC_DVO_SSG0_CTRL;

/*-----------------------------------------------------------------------------
	0x00d4 dvos_ssg0_dly ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvos0_sync_h_dly                :13,	//  0:12
	_rsvd                                : 3,	// 13:15 reserved
	dvos0_sync_v_dly                :13;	// 16:28
} M14B0_FRC_DVOS_SSG0_DLY;

/*-----------------------------------------------------------------------------
	0x00d8 dvos_ssg0_dly_start ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvos0_manual_cnt_h_v            :13,	//  0:12
	_rsvd_00                                : 2,	// 13:14 reserved
	dvos0_dly_start_h_v_en          : 1,	//    15
	dvos0_manual_cnt_v_v            :13,	// 16:28
	_rsvd_01                                : 2,	// 29:30 reserved
	dvos0_dly_start_v_v_en          : 1;	//    31
} M14B0_FRC_DVOS_SSG0_DLY_START;

/*-----------------------------------------------------------------------------
	0x00e0 dvo_ssg1_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	line_start_hpos                 :13,	//  0:12
	_rsvd_00                                : 3,	// 13:15 reserved
	sync_i_hs_ha_sel                : 1,	//    16
	sync_i_vs_va_sel                : 1,	//    17
	sync_i_hs_inv_sel               : 1,	//    18
	sync_i_vs_inv_sel               : 1,	//    19
	sync_o_ha_dly_sel               : 1,	//    20
	sync_o_hs_dly_sel               : 1,	//    21
	sync_o_va_dly_sel               : 1,	//    22
	sync_o_vs_dly_sel               : 1,	//    23
	tp_update_hv_zero               : 1,	//    24
	disp_pulse_sel                  : 1,	//    25
	_rsvd_01                                : 1,	//    26 reserved
	load_3dfp_parity                : 1,	//    27
	_rsvd_02                                : 3,	// 28:30 reserved
	force_update                    : 1;	//    31
} M14B0_FRC_DVO_SSG1_CTRL;

/*-----------------------------------------------------------------------------
	0x00e4 dvos_ssg1_dly ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvos1_sync_h_dly                :13,	//  0:12
	_rsvd                                : 3,	// 13:15 reserved
	dvos1_sync_v_dly                :13;	// 16:28
} M14B0_FRC_DVOS_SSG1_DLY;

/*-----------------------------------------------------------------------------
	0x00e8 dvos_ssg1_dly_start ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvos1_manual_cnt_h_v            :13,	//  0:12
	_rsvd_00                                : 2,	// 13:14 reserved
	dvos1_dly_start_h_v_en          : 1,	//    15
	dvos1_manual_cnt_v_v            :13,	// 16:28
	_rsvd_01                                : 2,	// 29:30 reserved
	dvos1_dly_start_v_v_en          : 1;	//    31
} M14B0_FRC_DVOS_SSG1_DLY_START;

/*-----------------------------------------------------------------------------
	0x00ec dvos_ssg1_param ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x00f0 dvo_ssg2_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	line_start_hpos                 :13,	//  0:12
	_rsvd_00                                : 3,	// 13:15 reserved
	sync_i_hs_ha_sel                : 1,	//    16
	sync_i_vs_va_sel                : 1,	//    17
	sync_i_hs_inv_sel               : 1,	//    18
	sync_i_vs_inv_sel               : 1,	//    19
	sync_o_ha_dly_sel               : 1,	//    20
	sync_o_hs_dly_sel               : 1,	//    21
	sync_o_va_dly_sel               : 1,	//    22
	sync_o_vs_dly_sel               : 1,	//    23
	tp_update_hv_zero               : 1,	//    24
	disp_pulse_sel                  : 1,	//    25
	force_update_src_sel            : 2,	// 26:27
	_rsvd_01                                : 2,	// 28:29 reserved
	force_update_auto_en            : 1,	//    30
	force_update                    : 1;	//    31
} M14B0_FRC_DVO_SSG2_CTRL;

/*-----------------------------------------------------------------------------
	0x00f4 dvos_ssg2_dly ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvos2_sync_h_dly                :13,	//  0:12
	_rsvd                                : 3,	// 13:15 reserved
	dvos2_sync_v_dly                :13;	// 16:28
} M14B0_FRC_DVOS_SSG2_DLY;

/*-----------------------------------------------------------------------------
	0x00f8 dvos_ssg2_dly_start ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvos2_manual_cnt_h_v            :13,	//  0:12
	_rsvd_00                                : 2,	// 13:14 reserved
	dvos2_dly_start_h_v_en          : 1,	//    15
	dvos2_manual_cnt_v_v            :13,	// 16:28
	_rsvd_01                                : 2,	// 29:30 reserved
	dvos2_dly_start_v_v_en          : 1;	//    31
} M14B0_FRC_DVOS_SSG2_DLY_START;

/*-----------------------------------------------------------------------------
	0x0100 dvom_io_type ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvom_o_format_type              : 4,	//  0: 3
	dvom_i_format_type              : 4,	//  4: 7
	dvom_i_map_type                 : 4;	//  8:11
} M14B0_FRC_DVOM_IO_TYPE;

/*-----------------------------------------------------------------------------
	0x0104 dvos_io_type ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvos_o_format_type              : 4,	//  0: 3
	dvos_i_format_type              : 4,	//  4: 7
	dvos_i_map_type                 : 4;	//  8:11
} M14B0_FRC_DVOS_IO_TYPE;

/*-----------------------------------------------------------------------------
	0x0108 dvom_disp_size ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvom_disp_ysize                 :13,	//  0:12
	_rsvd_00                                : 3,	// 13:15 reserved
	dvom_disp_xsize                 :13,	// 16:28
	_rsvd_01                                : 2,	// 29:30 reserved
	ld_dvom_disp_size               : 1;	//    31
} M14B0_FRC_DVOM_DISP_SIZE;

/*-----------------------------------------------------------------------------
	0x010c dvom_win_l_i_pos ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvom_win_l_i_ypos               :13,	//  0:12
	_rsvd_00                                : 3,	// 13:15 reserved
	dvom_win_l_i_xpos               :13,	// 16:28
	_rsvd_01                                : 2,	// 29:30 reserved
	ld_dvom_win_l_i_param           : 1;	//    31
} M14B0_FRC_DVOM_WIN_L_I_POS;

/*-----------------------------------------------------------------------------
	0x0110 dvom_win_l_i_size ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvom_win_l_i_ysize              :13,	//  0:12
	_rsvd                                : 3,	// 13:15 reserved
	dvom_win_l_i_xsize              :13;	// 16:28
} M14B0_FRC_DVOM_WIN_L_I_SIZE;

/*-----------------------------------------------------------------------------
	0x011c dvom_win_o_pos ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvom_win_o_ypos                 :13,	//  0:12
	_rsvd_00                                : 3,	// 13:15 reserved
	dvom_win_o_xpos                 :13,	// 16:28
	_rsvd_01                                : 2,	// 29:30 reserved
	ld_dvom_win_o_param             : 1;	//    31
} M14B0_FRC_DVOM_WIN_O_POS;

/*-----------------------------------------------------------------------------
	0x0120 dvom_win_o_size ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvom_win_o_ysize                :13,	//  0:12
	_rsvd                                : 3,	// 13:15 reserved
	dvom_win_o_xsize                :13;	// 16:28
} M14B0_FRC_DVOM_WIN_O_SIZE;

/*-----------------------------------------------------------------------------
	0x0124 dvos_win_i_pos ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvos_win_i_ypos                 :13,	//  0:12
	_rsvd_00                                : 3,	// 13:15 reserved
	dvos_win_i_xpos                 :13,	// 16:28
	_rsvd_01                                : 2,	// 29:30 reserved
	ld_dvos_win_i_param             : 1;	//    31
} M14B0_FRC_DVOS_WIN_I_POS;

/*-----------------------------------------------------------------------------
	0x0128 dvos_win_i_size ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvos_win_i_ysize                :13,	//  0:12
	_rsvd                                : 3,	// 13:15 reserved
	dvos_win_i_xsize                :13;	// 16:28
} M14B0_FRC_DVOS_WIN_I_SIZE;

/*-----------------------------------------------------------------------------
	0x012c dvos_win_o_pos ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvos_win_o_ypos                 :13,	//  0:12
	_rsvd_00                                : 3,	// 13:15 reserved
	dvos_win_o_xpos                 :13,	// 16:28
	_rsvd_01                                : 2,	// 29:30 reserved
	ld_dvos_win_o_param             : 1;	//    31
} M14B0_FRC_DVOS_WIN_O_POS;

/*-----------------------------------------------------------------------------
	0x0130 dvos_win_o_size ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvos_win_o_ysize                :13,	//  0:12
	_rsvd                                : 3,	// 13:15 reserved
	dvos_win_o_xsize                :13;	// 16:28
} M14B0_FRC_DVOS_WIN_O_SIZE;

/*-----------------------------------------------------------------------------
	0x0134 dvo_cdc_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvom_cdc_hsync_interval         : 2,	//  0: 1
	dvom_cdc_hsync_width            : 2,	//  2: 3
	dvom_cdc_ramp_pat_en            : 2,	//  4: 5
	_rsvd                                :10,	//  6:15 reserved
	dvos_cdc_hsync_interval         : 2,	// 16:17
	dvos_cdc_hsync_width            : 2;	// 18:19
} M14B0_FRC_DVO_CDC_CTRL;

/*-----------------------------------------------------------------------------
	0x013c dvom_hsc_ctrl0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvom_hsc_y_sampling_mode        : 1,	//     0
	dvom_hsc_c_sampling_mode        : 1,	//     1
	_rsvd_00                                : 2,	//  2: 3 reserved
	dvom_hsc_y_bnd_mode             : 1,	//     4
	dvom_hsc_c_bnd_mode             : 1,	//     5
	dvom_hsc_mode                   : 2,	//  6: 7
	dvom_hsc_bilinear_mode          : 1,	//     8
	_rsvd_01                                : 3,	//  9:11 reserved
	dvom_hsc_rpel_sel               : 3,	// 12:14
	_rsvd_02                                : 1,	//    15 reserved
	dvom_hsc_window_xsize           :13,	// 16:28
	dvom_hsc_window_mode            : 3;	// 29:31
} M14B0_FRC_DVOM_HSC_CTRL0;

/*-----------------------------------------------------------------------------
	0x0140 dvom_hsc_ctrl1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvom_hsc_i_hsize                :13,	//  0:12
	_rsvd                                : 3,	// 13:15 reserved
	dvom_hsc_o_hsize                :13;	// 16:28
} M14B0_FRC_DVOM_HSC_CTRL1;

/*-----------------------------------------------------------------------------
	0x0144 dvom_hsc_ctrl2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvom_hsc_denominator_y          :13,	//  0:12
	_rsvd                                : 3,	// 13:15 reserved
	dvom_hsc_numerator_y            :13;	// 16:28
} M14B0_FRC_DVOM_HSC_CTRL2;

/*-----------------------------------------------------------------------------
	0x0148 dvom_hsc_ctrl3 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvom_hsc_denominator_c          :13,	//  0:12
	_rsvd                                : 3,	// 13:15 reserved
	dvom_hsc_numerator_c            :13;	// 16:28
} M14B0_FRC_DVOM_HSC_CTRL3;

/*-----------------------------------------------------------------------------
	0x014c dvom_hsc_coef0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvom_hsc_coef1_y                :12,	//  0:11
	_rsvd                                : 4,	// 12:15 reserved
	dvom_hsc_coef0_y                :12;	// 16:27
} M14B0_FRC_DVOM_HSC_COEF0;

/*-----------------------------------------------------------------------------
	0x0150 dvom_hsc_coef1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvom_hsc_coef3_y                :12,	//  0:11
	_rsvd                                : 4,	// 12:15 reserved
	dvom_hsc_coef2_y                :12;	// 16:27
} M14B0_FRC_DVOM_HSC_COEF1;

/*-----------------------------------------------------------------------------
	0x0154 dvom_hsc_coef2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvom_hsc_phase_off_y            : 6,	//  0: 5
	_rsvd_00                                :10,	//  6:15 reserved
	dvom_hsc_phase_addr_y           : 5,	// 16:20
	_rsvd_01                                : 3,	// 21:23 reserved
	dvom_hsc_phase_rnw_y            : 1,	//    24
	_rsvd_02                                : 3,	// 25:27 reserved
	dvom_hsc_phase_sel_y            : 1;	//    28
} M14B0_FRC_DVOM_HSC_COEF2;

/*-----------------------------------------------------------------------------
	0x0158 dvom_hsc_coef3 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvom_hsc_coef1_cb               :12,	//  0:11
	_rsvd                                : 4,	// 12:15 reserved
	dvom_hsc_coef0_cb               :12;	// 16:27
} M14B0_FRC_DVOM_HSC_COEF3;

/*-----------------------------------------------------------------------------
	0x015c dvom_hsc_coef4 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvom_hsc_coef3_cb               :12,	//  0:11
	_rsvd                                : 4,	// 12:15 reserved
	dvom_hsc_coef2_cb               :12;	// 16:27
} M14B0_FRC_DVOM_HSC_COEF4;

/*-----------------------------------------------------------------------------
	0x0160 dvom_hsc_coef5 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvom_hsc_phase_off_cb           : 6,	//  0: 5
	_rsvd_00                                :10,	//  6:15 reserved
	dvom_hsc_phase_addr_cb          : 5,	// 16:20
	_rsvd_01                                : 3,	// 21:23 reserved
	dvom_hsc_phase_rnw_cb           : 1,	//    24
	_rsvd_02                                : 3,	// 25:27 reserved
	dvom_hsc_phase_sel_cb           : 1;	//    28
} M14B0_FRC_DVOM_HSC_COEF5;

/*-----------------------------------------------------------------------------
	0x0164 dvom_hsc_coef6 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvom_hsc_coef1_cr               :12,	//  0:11
	_rsvd                                : 4,	// 12:15 reserved
	dvom_hsc_coef0_cr               :12;	// 16:27
} M14B0_FRC_DVOM_HSC_COEF6;

/*-----------------------------------------------------------------------------
	0x0168 dvom_hsc_coef7 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvom_hsc_coef3_cr               :12,	//  0:11
	_rsvd                                : 4,	// 12:15 reserved
	dvom_hsc_coef2_cr               :12;	// 16:27
} M14B0_FRC_DVOM_HSC_COEF7;

/*-----------------------------------------------------------------------------
	0x016c dvom_hsc_coef8 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvom_hsc_phase_off_cr           : 6,	//  0: 5
	_rsvd_00                                :10,	//  6:15 reserved
	dvom_hsc_phase_addr_cr          : 5,	// 16:20
	_rsvd_01                                : 3,	// 21:23 reserved
	dvom_hsc_phase_rnw_cr           : 1,	//    24
	_rsvd_02                                : 3,	// 25:27 reserved
	dvom_hsc_phase_sel_cr           : 1;	//    28
} M14B0_FRC_DVOM_HSC_COEF8;

/*-----------------------------------------------------------------------------
	0x0170 dvos_hsc_ctrl0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvos_lsb_ctrl                   : 2,	//  0: 1
	dvos_sample_mode                : 2,	//  2: 3
	dvos_hsc_in_mode                : 1;	//     4
} M14B0_FRC_DVOS_HSC_CTRL0;

/*-----------------------------------------------------------------------------
	0x0174 dvos_hsc_coef0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvos_hsc_coef1                  :12,	//  0:11
	_rsvd                                : 4,	// 12:15 reserved
	dvos_hsc_coef0                  :12;	// 16:27
} M14B0_FRC_DVOS_HSC_COEF0;

/*-----------------------------------------------------------------------------
	0x0178 dvos_hsc_coef1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvos_hsc_coef3                  :12,	//  0:11
	_rsvd                                : 4,	// 12:15 reserved
	dvos_hsc_coef2                  :12;	// 16:27
} M14B0_FRC_DVOS_HSC_COEF1;

/*-----------------------------------------------------------------------------
	0x017c dvom_mdo_ctrl0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvom_mdo_hwp0_o_xsize           :13,	//  0:12
	_rsvd_00                                : 3,	// 13:15 reserved
	dvom_mdo_hwp0_o_xpos            :13,	// 16:28
	_rsvd_01                                : 2,	// 29:30 reserved
	ld_dvom_mdo_hwp0_o_info         : 1;	//    31
} M14B0_FRC_DVOM_MDO_CTRL0;

/*-----------------------------------------------------------------------------
	0x0180 dvom_mdo_ctrl1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvom_mdo_hwp1_o_xsize           :13,	//  0:12
	_rsvd_00                                : 3,	// 13:15 reserved
	dvom_mdo_hwp1_o_xpos            :13,	// 16:28
	_rsvd_01                                : 2,	// 29:30 reserved
	ld_dvom_mdo_hwp1_o_info         : 1;	//    31
} M14B0_FRC_DVOM_MDO_CTRL1;

/*-----------------------------------------------------------------------------
	0x0184 dvom_mdo_ctrl2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvom_mdo_nonact_fill_en         : 1,	//     0
	dvom_mdo_mode                   : 2;	//  1: 2
} M14B0_FRC_DVOM_MDO_CTRL2;

/*-----------------------------------------------------------------------------
	0x0188 dvom_mif_status1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	monitor_rhost_fill_cnt_c        :10,	//  0: 9
	_rsvd                                : 6,	// 10:15 reserved
	monitor_rhost_fill_cnt_y        :10;	// 16:25
} M14B0_FRC_DVOM_MIF_STATUS1;

/*-----------------------------------------------------------------------------
	0x018c dvos_mif_status1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	monitor_rhost_fill_cnt_c        :10,	//  0: 9
	_rsvd                                : 6,	// 10:15 reserved
	monitor_rhost_fill_cnt_y        :10;	// 16:25
} M14B0_FRC_DVOS_MIF_STATUS1;

/*-----------------------------------------------------------------------------
	0x0190 dvom_rmif_ctrl0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvom_cbcr_sw                    : 1,	//     0
	_rsvd_00                                : 7,	//  1: 7 reserved
	dvom_mif_c_lsb_ctrl             : 2,	//  8: 9
	dvom_mif_y_lsb_ctrl             : 2,	// 10:11
	_rsvd_01                                : 4,	// 12:15 reserved
	dvom_mif_10b_req_c_en           : 1,	//    16
	dvom_mif_10b_req_y_en           : 1,	//    17
	dvom_mif_block_req_c_en         : 1,	//    18
	dvom_mif_block_req_y_en         : 1,	//    19
	dvom_mif_en_stv_cnt             : 1,	//    20
	dvom_mif_fpr_parity             : 1,	//    21
	dvom_mif_fpr_parity_manual_en   : 1,	//    22
	_rsvd_02                                : 1,	//    23 reserved
	dvom_mif_3dfp_parity            : 1,	//    24
	dvom_mif_3dfp_parity_manual_en  : 1,	//    25
	dvom_mif_3dfp_en                : 1;	//    26
} M14B0_FRC_DVOM_RMIF_CTRL0;

/*-----------------------------------------------------------------------------
	0x0194 dvos_rmif_ctrl0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvos_cbcr_sw                    : 1,	//     0
	_rsvd_00                                : 7,	//  1: 7 reserved
	dvos_mif_c_lsb_ctrl             : 2,	//  8: 9
	dvos_mif_y_lsb_ctrl             : 2,	// 10:11
	_rsvd_01                                : 4,	// 12:15 reserved
	dvos_mif_10b_req_c_en           : 1,	//    16
	dvos_mif_10b_req_y_en           : 1,	//    17
	dvos_mif_block_req_c_en         : 1,	//    18
	dvos_mif_block_req_y_en         : 1,	//    19
	dvos_mif_en_stv_cnt             : 1,	//    20
	dvos_mif_fpr_parity             : 1,	//    21
	dvos_mif_fpr_parity_manual_en   : 1;	//    22
} M14B0_FRC_DVOS_RMIF_CTRL0;

/*-----------------------------------------------------------------------------
	0x0198 dvom_mif_status ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvom_rhost_c_lcount             :11,	//  0:10
	dvom_mif_req_c_lcount           : 5,	// 11:15
	dvom_rhost_y_lcount             :11,	// 16:26
	dvom_mif_req_y_lcount           : 4,	// 27:30
	dvom_gmau_arb_cleardone         : 1;	//    31
} M14B0_FRC_DVOM_MIF_STATUS;

/*-----------------------------------------------------------------------------
	0x019c dvos_mif_status ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvos_rhost_c_lcount             :11,	//  0:10
	dvos_mif_req_c_lcount           : 5,	// 11:15
	dvos_rhost_y_lcount             :11,	// 16:26
	dvos_mif_req_y_lcount           : 5;	// 27:31
} M14B0_FRC_DVOS_MIF_STATUS;

/*-----------------------------------------------------------------------------
	0x01a0 dvom_saddr_8_rd_l_y_a ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x01a4 dvom_saddr_2_rd_l_y_a ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x01a8 dvom_saddr_8_rd_l_c_a ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x01ac dvom_saddr_2_rd_l_c_a ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x01c0 dvom_saddr_8_rd_r_y_a ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x01c4 dvom_saddr_2_rd_r_y_a ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x01c8 dvom_saddr_8_rd_r_c_a ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x01cc dvom_saddr_2_rd_r_c_a ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x01d0 dvom_rmif_pat_ctrl0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvom_rmif_pat_y7                : 4,	//  0: 3
	dvom_rmif_pat_y6                : 4,	//  4: 7
	dvom_rmif_pat_y5                : 4,	//  8:11
	dvom_rmif_pat_y4                : 4,	// 12:15
	dvom_rmif_pat_y3                : 4,	// 16:19
	dvom_rmif_pat_y2                : 4,	// 20:23
	dvom_rmif_pat_y1                : 4,	// 24:27
	dvom_rmif_pat_y0                : 4;	// 28:31
} M14B0_FRC_DVOM_RMIF_PAT_CTRL0;

/*-----------------------------------------------------------------------------
	0x01d4 dvom_rmif_pat_ctrl1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvom_rmif_pat_c7                : 4,	//  0: 3
	dvom_rmif_pat_c6                : 4,	//  4: 7
	dvom_rmif_pat_c5                : 4,	//  8:11
	dvom_rmif_pat_c4                : 4,	// 12:15
	dvom_rmif_pat_c3                : 4,	// 16:19
	dvom_rmif_pat_c2                : 4,	// 20:23
	dvom_rmif_pat_c1                : 4,	// 24:27
	dvom_rmif_pat_c0                : 4;	// 28:31
} M14B0_FRC_DVOM_RMIF_PAT_CTRL1;

/*-----------------------------------------------------------------------------
	0x01d8 dvos_rmif_pat_ctrl0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvos_rmif_pat_y7                : 4,	//  0: 3
	dvos_rmif_pat_y6                : 4,	//  4: 7
	dvos_rmif_pat_y5                : 4,	//  8:11
	dvos_rmif_pat_y4                : 4,	// 12:15
	dvos_rmif_pat_y3                : 4,	// 16:19
	dvos_rmif_pat_y2                : 4,	// 20:23
	dvos_rmif_pat_y1                : 4,	// 24:27
	dvos_rmif_pat_y0                : 4;	// 28:31
} M14B0_FRC_DVOS_RMIF_PAT_CTRL0;

/*-----------------------------------------------------------------------------
	0x01dc dvos_rmif_pat_ctrl1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvos_rmif_pat_c7                : 4,	//  0: 3
	dvos_rmif_pat_c6                : 4,	//  4: 7
	dvos_rmif_pat_c5                : 4,	//  8:11
	dvos_rmif_pat_c4                : 4,	// 12:15
	dvos_rmif_pat_c3                : 4,	// 16:19
	dvos_rmif_pat_c2                : 4,	// 20:23
	dvos_rmif_pat_c1                : 4,	// 24:27
	dvos_rmif_pat_c0                : 4;	// 28:31
} M14B0_FRC_DVOS_RMIF_PAT_CTRL1;

/*-----------------------------------------------------------------------------
	0x01e0 dvos_saddr_8_rd_y_a ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x01e4 dvos_saddr_2_rd_y_a ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x01e8 dvos_saddr_8_rd_c_a ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x01ec dvos_saddr_2_rd_c_a ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x01f0 dvos_saddr_8_rd_y_b ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x01f4 dvos_saddr_2_rd_y_b ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x01f8 dvos_saddr_8_rd_c_b ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x01fc dvos_saddr_2_rd_c_b ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x0200 dvom_mif_l_end_y ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvom_mif_l_e_min_ypos           :13,	//  0:12
	_rsvd_00                                : 3,	// 13:15 reserved
	dvom_mif_l_e_max_ypos           :13,	// 16:28
	_rsvd_01                                : 2,	// 29:30 reserved
	ld_dvom_mif_l_y_end             : 1;	//    31
} M14B0_FRC_DVOM_MIF_L_END_Y;

/*-----------------------------------------------------------------------------
	0x0204 dvom_mif_r_end_y ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvom_mif_r_e_min_ypos           :13,	//  0:12
	_rsvd_00                                : 3,	// 13:15 reserved
	dvom_mif_r_e_max_ypos           :13,	// 16:28
	_rsvd_01                                : 2,	// 29:30 reserved
	ld_dvom_mif_r_y_end             : 1;	//    31
} M14B0_FRC_DVOM_MIF_R_END_Y;

/*-----------------------------------------------------------------------------
	0x0208 dvos_mif_end_y ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvos_mif_e_ypos                 :13,	//  0:12
	_rsvd                                :18,	// 13:30 reserved
	ld_dvos_mif_y_end               : 1;	//    31
} M14B0_FRC_DVOS_MIF_END_Y;

/*-----------------------------------------------------------------------------
	0x020c dvom_stride8_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvom_byte_stride8_y             :15,	//  0:14
	dvom_byte_stride8_y_en          : 1,	//    15
	dvom_byte_stride8_c             :15,	// 16:30
	dvom_byte_stride8_c_en          : 1;	//    31
} M14B0_FRC_DVOM_STRIDE8_CTRL;

/*-----------------------------------------------------------------------------
	0x0210 dvom_stride2_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvom_byte_stride2_y             :15,	//  0:14
	dvom_byte_stride2_y_en          : 1,	//    15
	dvom_byte_stride2_c             :15,	// 16:30
	dvom_byte_stride2_c_en          : 1;	//    31
} M14B0_FRC_DVOM_STRIDE2_CTRL;

/*-----------------------------------------------------------------------------
	0x0214 dvos_stride8_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvos_byte_stride8_y             :15,	//  0:14
	dvos_byte_stride8_y_en          : 1,	//    15
	dvos_byte_stride8_c             :15,	// 16:30
	dvos_byte_stride8_c_en          : 1;	//    31
} M14B0_FRC_DVOS_STRIDE8_CTRL;

/*-----------------------------------------------------------------------------
	0x0218 dvos_stride2_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvos_byte_stride2_y             :15,	//  0:14
	dvos_byte_stride2_y_en          : 1,	//    15
	dvos_byte_stride2_c             :15,	// 16:30
	dvos_byte_stride2_c_en          : 1;	//    31
} M14B0_FRC_DVOS_STRIDE2_CTRL;

/*-----------------------------------------------------------------------------
	0x021c dvom_gmau_stride_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvom_gmau_stride8_y             : 4,	//  0: 3
	dvom_gmau_stride8_c             : 4,	//  4: 7
	dvom_gmau_stride2_y             : 4,	//  8:11
	dvom_gmau_stride2_c             : 4,	// 12:15
	dvom_gmau_stride8_y_en          : 1,	//    16
	dvom_gmau_stride8_c_en          : 1,	//    17
	dvom_gmau_stride2_y_en          : 1,	//    18
	dvom_gmau_stride2_c_en          : 1;	//    19
} M14B0_FRC_DVOM_GMAU_STRIDE_CTRL;

/*-----------------------------------------------------------------------------
	0x0220 dvos_gmau_stride_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dvos_gmau_stride8_y             : 4,	//  0: 3
	dvos_gmau_stride8_c             : 4,	//  4: 7
	dvos_gmau_stride2_y             : 4,	//  8:11
	dvos_gmau_stride2_c             : 4,	// 12:15
	dvos_gmau_stride8_y_en          : 1,	//    16
	dvos_gmau_stride8_c_en          : 1,	//    17
	dvos_gmau_stride2_y_en          : 1,	//    18
	dvos_gmau_stride2_c_en          : 1;	//    19
} M14B0_FRC_DVOS_GMAU_STRIDE_CTRL;

/*-----------------------------------------------------------------------------
	0x0224 dvo_mifm_pxl_size_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	man_mif2_c_pxl_size             : 4,	//  0: 3
	man_mif2_y_pxl_size             : 4,	//  4: 7
	man_mif8_c_pxl_size             : 4,	//  8:11
	man_mif8_y_pxl_size             : 4,	// 12:15
	_rsvd                                :15,	// 16:30 reserved
	man_mif_pxl_size_en             : 1;	//    31
} M14B0_FRC_DVO_MIFM_PXL_SIZE_CTRL;

/*-----------------------------------------------------------------------------
	0x0228 dvo_mifs_pxl_size_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	man_mif2_c_pxl_size             : 4,	//  0: 3
	man_mif2_y_pxl_size             : 4,	//  4: 7
	man_mif8_c_pxl_size             : 4,	//  8:11
	man_mif8_y_pxl_size             : 4,	// 12:15
	_rsvd                                :15,	// 16:30 reserved
	man_mif_pxl_size_en             : 1;	//    31
} M14B0_FRC_DVO_MIFS_PXL_SIZE_CTRL;

/*-----------------------------------------------------------------------------
	0x0230 dvo_rhost_mif_ctrl0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	mifm_y_mif_lm_lnum              :10,	//  0: 9
	_rsvd_00                                : 6,	// 10:15 reserved
	mifs_y_mif_lm_lnum              :10,	// 16:25
	_rsvd_01                                : 3,	// 26:28 reserved
	mifs_rhost_pat_en               : 3;	// 29:31
} M14B0_FRC_DVO_RHOST_MIF_CTRL0;

/*-----------------------------------------------------------------------------
	0x0234 dvo_rhost_mif_ctrl1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	mifm_y_mif_last_mode            : 1,	//     0
	mifm_c_mif_last_mode            : 1,	//     1
	_rsvd_00                                : 2,	//  2: 3 reserved
	mifm_v_reverse_mode_en          : 1,	//     4
	_rsvd_01                                : 3,	//  5: 7 reserved
	mifm_cmd_base_msb               : 1,	//     8
	_rsvd_02                                : 2,	//  9:10 reserved
	mifm_cmd_base_msb_set           : 1,	//    11
	_rsvd_03                                : 4,	// 12:15 reserved
	mifs_y_mif_last_mode            : 1,	//    16
	mifs_c_mif_last_mode            : 1,	//    17
	_rsvd_04                                : 2,	// 18:19 reserved
	mifs_v_reverse_mode_en          : 1,	//    20
	_rsvd_05                                : 3,	// 21:23 reserved
	mifs_cmd_base_msb               : 1,	//    24
	_rsvd_06                                : 2,	// 25:26 reserved
	mifs_cmd_base_msb_set           : 1;	//    27
} M14B0_FRC_DVO_RHOST_MIF_CTRL1;

/*-----------------------------------------------------------------------------
	0x023c dvom_rhost_mif_ctrl0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	mif_line_y_start                :12,	//  0:11
	mif_line_y_inc_dec_offset       : 2,	// 12:13
	mif_line_y_inc_dec_sel          : 1,	//    14
	mif_line_y_ctrl_en              : 1,	//    15
	mif_line_c_start                :12,	// 16:27
	mif_line_c_inc_dec_offset       : 2,	// 28:29
	mif_line_c_inc_dec_sel          : 1,	//    30
	mif_line_c_ctrl_en              : 1;	//    31
} M14B0_FRC_DVOM_RHOST_MIF_CTRL0;

/*-----------------------------------------------------------------------------
	0x0240 dvom_rhost_mif_ctrl1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	mif_line_y_r_start              :12,	//  0:11
	_rsvd_00                                : 3,	// 12:14 reserved
	mif_line_y_r_ctrl_en            : 1,	//    15
	mif_line_c_r_start              :12,	// 16:27
	_rsvd_01                                : 3,	// 28:30 reserved
	mif_line_c_r_ctrl_en            : 1;	//    31
} M14B0_FRC_DVOM_RHOST_MIF_CTRL1;

/*-----------------------------------------------------------------------------
	0x0244 dvos_rhost_mif_ctrl0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	mif_line_y_start                :12,	//  0:11
	mif_line_y_inc_dec_offset       : 2,	// 12:13
	mif_line_y_inc_dec_sel          : 1,	//    14
	mif_line_y_ctrl_en              : 1,	//    15
	mif_line_c_start                :12,	// 16:27
	mif_line_c_inc_dec_offset       : 2,	// 28:29
	mif_line_c_inc_dec_sel          : 1,	//    30
	mif_line_c_ctrl_en              : 1;	//    31
} M14B0_FRC_DVOS_RHOST_MIF_CTRL0;

/*-----------------------------------------------------------------------------
	0x0248 dvos_rhost_mif_ctrl1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	mif_line_y_r_start              :12,	//  0:11
	_rsvd_00                                : 3,	// 12:14 reserved
	mif_line_y_r_ctrl_en            : 1,	//    15
	mif_line_c_r_start              :12,	// 16:27
	_rsvd_01                                : 3,	// 28:30 reserved
	mif_line_c_r_ctrl_en            : 1;	//    31
} M14B0_FRC_DVOS_RHOST_MIF_CTRL1;

/*-----------------------------------------------------------------------------
	0x0250 dvom_block_cmd_l_ypos0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	block_cmd_ypos0_s               :12,	//  0:11
	_rsvd_00                                : 4,	// 12:15 reserved
	block_cmd_ypos0_e               :12,	// 16:27
	block_cmd_l_en                  : 1,	//    28
	_rsvd_01                                : 2,	// 29:30 reserved
	block_cmd_en                    : 1;	//    31
} M14B0_FRC_DVOM_BLOCK_CMD_L_YPOS0;

/*-----------------------------------------------------------------------------
	0x0254 dvom_block_cmd_l_ypos1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	block_cmd_ypos1_s               :12,	//  0:11
	_rsvd_00                                : 4,	// 12:15 reserved
	block_cmd_ypos1_e               :12,	// 16:27
	block_cmd_r_en                  : 1,	//    28
	_rsvd_01                                : 2,	// 29:30 reserved
	block_cmd_en                    : 1;	//    31
} M14B0_FRC_DVOM_BLOCK_CMD_L_YPOS1;

/*-----------------------------------------------------------------------------
	0x0258 dvom_lm_status_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	mifm_lm_nfill_ext_c             :10,	//  0: 9
	_rsvd_00                                : 2,	// 10:11 reserved
	mifm_lm_nfill_start_line        : 4,	// 12:15
	mifm_lm_nfill_ext_y             :10,	// 16:25
	_rsvd_01                                : 1,	//    26 reserved
	mifm_lm_fill_cnt_type           : 1,	//    27
	mifm_lm_status_intr_en          : 1,	//    28
	_rsvd_02                                : 2,	// 29:30 reserved
	mifm_lm_status_view_en          : 1;	//    31
} M14B0_FRC_DVOM_LM_STATUS_CTRL;

/*-----------------------------------------------------------------------------
	0x025c dvos_lm_status_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	mifs_lm_nfill_ext_c             :10,	//  0: 9
	_rsvd_00                                : 2,	// 10:11 reserved
	mifs_lm_nfill_start_line        : 4,	// 12:15
	mifs_lm_nfill_ext_y             :10,	// 16:25
	_rsvd_01                                : 1,	//    26 reserved
	mifs_lm_fill_cnt_type           : 1,	//    27
	mifs_lm_status_intr_en          : 1,	//    28
	_rsvd_02                                : 2,	// 29:30 reserved
	mifs_lm_status_view_en          : 1;	//    31
} M14B0_FRC_DVOS_LM_STATUS_CTRL;

/*-----------------------------------------------------------------------------
	0x0260 dvos_block_cmd_ypos0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	block_cmd_ypos0_s               :12,	//  0:11
	_rsvd_00                                : 4,	// 12:15 reserved
	block_cmd_ypos0_e               :12,	// 16:27
	block_cmd_l_en                  : 1,	//    28
	_rsvd_01                                : 2,	// 29:30 reserved
	block_cmd_en                    : 1;	//    31
} M14B0_FRC_DVOS_BLOCK_CMD_YPOS0;

/*-----------------------------------------------------------------------------
	0x0264 dvos_block_cmd_ypos1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	block_cmd_ypos1_s               :12,	//  0:11
	_rsvd_00                                : 4,	// 12:15 reserved
	block_cmd_ypos1_e               :12,	// 16:27
	block_cmd_r_en                  : 1,	//    28
	_rsvd_01                                : 2,	// 29:30 reserved
	block_cmd_en                    : 1;	//    31
} M14B0_FRC_DVOS_BLOCK_CMD_YPOS1;

/*-----------------------------------------------------------------------------
	0x0268 dvom_rmif_ctrl1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	rmif_reset_to_cnt               :12,	//  0:11
	_rsvd_00                                : 4,	// 12:15 reserved
	rmif_pend_v                     : 8,	// 16:23
	rmif_bank_mode                  : 3,	// 24:26
	_rsvd_01                                : 4,	// 27:30 reserved
	rmif_rd_pat_sw_en               : 1;	//    31
} M14B0_FRC_DVOM_RMIF_CTRL1;

/*-----------------------------------------------------------------------------
	0x026c dvos_rmif_ctrl1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	rmif_reset_to_cnt               :12,	//  0:11
	_rsvd_00                                : 4,	// 12:15 reserved
	rmif_pend_v                     : 8,	// 16:23
	rmif_bank_mode                  : 3,	// 24:26
	_rsvd_01                                : 4,	// 27:30 reserved
	rmif_rd_pat_sw_en               : 1;	//    31
} M14B0_FRC_DVOS_RMIF_CTRL1;

/*-----------------------------------------------------------------------------
	0x0270 dvo_mpg_pat_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	lattice_pat_en                  : 1,	//     0
	_rsvd_00                                : 1,	//     1 reserved
	gbrw_pat_en                     : 2,	//  2: 3
	ramp_pat_en                     : 2,	//  4: 5
	mpg_o_color_sel                 : 2,	//  6: 7
	border_sel                      : 4,	//  8:11
	_rsvd_01                                : 3,	// 12:14 reserved
	status_bar_en                   : 1,	//    15
	dbg_pip_mix_enable              : 1,	//    16
	_rsvd_02                                : 1,	//    17 reserved
	dbg_svp_detour_mode             : 2;	// 18:19
} M14B0_FRC_DVO_MPG_PAT_CTRL;

/*-----------------------------------------------------------------------------
	0x0274 dvo_spg_pat_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	lattice_pat_en                  : 1,	//     0
	_rsvd                                : 3,	//  1: 3 reserved
	ramp_pat_en                     : 2,	//  4: 5
	spg_o_color_sel                 : 2,	//  6: 7
	border_sel                      : 4;	//  8:11
} M14B0_FRC_DVO_SPG_PAT_CTRL;

/*-----------------------------------------------------------------------------
	0x0278 dvo_mpg_pixel_read_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	rd_pel_ypos                     :12,	//  0:11
	_rsvd_00                                : 4,	// 12:15 reserved
	rd_pel_xpos                     :12,	// 16:27
	_rsvd_01                                : 3,	// 28:30 reserved
	rd_pel_pat_en                   : 1;	//    31
} M14B0_FRC_DVO_MPG_PIXEL_READ_CTRL;

/*-----------------------------------------------------------------------------
	0x027c dvo_mpg_pixel_read_data ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	msb8_rd_pel_cr                  : 8,	//  0: 7
	msb8_rd_pel_cb                  : 8,	//  8:15
	msb8_rd_pel_gy                  : 8,	// 16:23
	lsb2_rd_pel_cr                  : 2,	// 24:25
	lsb2_rd_pel_cb                  : 2,	// 26:27
	lsb2_rd_pel_gy                  : 2;	// 28:29
} M14B0_FRC_DVO_MPG_PIXEL_READ_DATA;

/*-----------------------------------------------------------------------------
	0x0280 dvo_spg_pixel_read_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	rd_pel_ypos                     :12,	//  0:11
	_rsvd_00                                : 4,	// 12:15 reserved
	rd_pel_xpos                     :12,	// 16:27
	_rsvd_01                                : 3,	// 28:30 reserved
	rd_pel_pat_en                   : 1;	//    31
} M14B0_FRC_DVO_SPG_PIXEL_READ_CTRL;

/*-----------------------------------------------------------------------------
	0x0284 dvo_spg_pixel_read_data ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	msb8_rd_pel_cr                  : 8,	//  0: 7
	msb8_rd_pel_cb                  : 8,	//  8:15
	msb8_rd_pel_gy                  : 8,	// 16:23
	lsb2_rd_pel_cr                  : 2,	// 24:25
	lsb2_rd_pel_cb                  : 2,	// 26:27
	lsb2_rd_pel_gy                  : 2;	// 28:29
} M14B0_FRC_DVO_SPG_PIXEL_READ_DATA;

/*-----------------------------------------------------------------------------
	0x0288 dvo_mpg_line_ctrl0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	even_line_cr                    : 8,	//  0: 7
	even_line_cb                    : 8,	//  8:15
	even_line_y                     : 8,	// 16:23
	_rsvd                                : 7,	// 24:30 reserved
	even_line_pat_en                : 1;	//    31
} M14B0_FRC_DVO_MPG_LINE_CTRL0;

/*-----------------------------------------------------------------------------
	0x028c dvo_mpg_line_ctrl1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	odd_line_cr                     : 8,	//  0: 7
	odd_line_cb                     : 8,	//  8:15
	odd_line_y                      : 8,	// 16:23
	_rsvd                                : 7,	// 24:30 reserved
	odd_line_pat_en                 : 1;	//    31
} M14B0_FRC_DVO_MPG_LINE_CTRL1;

/*-----------------------------------------------------------------------------
	0x0290 dvo_mpg_box0_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	box0_cr                         : 8,	//  0: 7
	box0_cb                         : 8,	//  8:15
	box0_y                          : 8,	// 16:23
	_rsvd                                : 7,	// 24:30 reserved
	mpg_box0_en                     : 1;	//    31
} M14B0_FRC_DVO_MPG_BOX0_CTRL;

/*-----------------------------------------------------------------------------
	0x0294 dvo_mpg_box1_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	box1_cr                         : 8,	//  0: 7
	box1_cb                         : 8,	//  8:15
	box1_y                          : 8,	// 16:23
	_rsvd                                : 7,	// 24:30 reserved
	mpg_box1_en                     : 1;	//    31
} M14B0_FRC_DVO_MPG_BOX1_CTRL;

/*-----------------------------------------------------------------------------
	0x0298 dvo_mpg_box23_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	box3_cr                         : 4,	//  0: 3
	box3_cb                         : 4,	//  4: 7
	box3_y                          : 4,	//  8:11
	_rsvd_00                                : 3,	// 12:14 reserved
	mpg_box3_en                     : 1,	//    15
	box2_cr                         : 4,	// 16:19
	box2_cb                         : 4,	// 20:23
	box2_y                          : 4,	// 24:27
	_rsvd_01                                : 3,	// 28:30 reserved
	mpg_box2_en                     : 1;	//    31
} M14B0_FRC_DVO_MPG_BOX23_CTRL;

/*-----------------------------------------------------------------------------
	0x029c dvo_mpg_box45_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	box5_cr                         : 4,	//  0: 3
	box5_cb                         : 4,	//  4: 7
	box5_y                          : 4,	//  8:11
	_rsvd_00                                : 3,	// 12:14 reserved
	mpg_box5_en                     : 1,	//    15
	box4_cr                         : 4,	// 16:19
	box4_cb                         : 4,	// 20:23
	box4_y                          : 4,	// 24:27
	_rsvd_01                                : 3,	// 28:30 reserved
	mpg_box4_en                     : 1;	//    31
} M14B0_FRC_DVO_MPG_BOX45_CTRL;

/*-----------------------------------------------------------------------------
	0x02a0 dvo_mpg_box67_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	box7_cr                         : 4,	//  0: 3
	box7_cb                         : 4,	//  4: 7
	box7_y                          : 4,	//  8:11
	_rsvd_00                                : 3,	// 12:14 reserved
	mpg_box7_en                     : 1,	//    15
	box6_cr                         : 4,	// 16:19
	box6_cb                         : 4,	// 20:23
	box6_y                          : 4,	// 24:27
	_rsvd_01                                : 3,	// 28:30 reserved
	mpg_box6_en                     : 1;	//    31
} M14B0_FRC_DVO_MPG_BOX67_CTRL;

/*-----------------------------------------------------------------------------
	0x02a4 dvo_mpg_bound_ctrl0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	bound_bottom                    :13,	//  0:12
	_rsvd_00                                : 2,	// 13:14 reserved
	mpg_bound_bottom_en             : 1,	//    15
	bound_top                       :13,	// 16:28
	_rsvd_01                                : 2,	// 29:30 reserved
	mpg_bound_top_en                : 1;	//    31
} M14B0_FRC_DVO_MPG_BOUND_CTRL0;

/*-----------------------------------------------------------------------------
	0x02a8 dvo_mpg_bound_ctrl1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	bound_right                     :13,	//  0:12
	_rsvd_00                                : 2,	// 13:14 reserved
	mpg_bound_right_en              : 1,	//    15
	bound_left                      :13,	// 16:28
	_rsvd_01                                : 2,	// 29:30 reserved
	mpg_bound_left_en               : 1;	//    31
} M14B0_FRC_DVO_MPG_BOUND_CTRL1;

/*-----------------------------------------------------------------------------
	0x02ac dvo_mpg_bound_ctrl2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	aux_bound_left                  :13,	//  0:12
	_rsvd                                : 2,	// 13:14 reserved
	mpg_aux_bound_left_en           : 1;	//    15
} M14B0_FRC_DVO_MPG_BOUND_CTRL2;

/*-----------------------------------------------------------------------------
	0x02b0 dvo_mpg_bound_ctrl3 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	bound_bot_cr                    : 4,	//  0: 3
	bound_bot_cb                    : 4,	//  4: 7
	bound_bot_y                     : 4,	//  8:11
	_rsvd                                : 4,	// 12:15 reserved
	bound_top_cr                    : 4,	// 16:19
	bound_top_cb                    : 4,	// 20:23
	bound_top_y                     : 4;	// 24:27
} M14B0_FRC_DVO_MPG_BOUND_CTRL3;

/*-----------------------------------------------------------------------------
	0x02b4 dvo_mpg_bound_ctrl4 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	bound_right_cr                  : 4,	//  0: 3
	bound_right_cb                  : 4,	//  4: 7
	bound_right_y                   : 4,	//  8:11
	_rsvd                                : 4,	// 12:15 reserved
	bound_left_cr                   : 4,	// 16:19
	bound_left_cb                   : 4,	// 20:23
	bound_left_y                    : 4;	// 24:27
} M14B0_FRC_DVO_MPG_BOUND_CTRL4;

/*-----------------------------------------------------------------------------
	0x02b8 dvo_mpg_bound_ctrl5 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	bound_aleft_cr                  : 4,	//  0: 3
	bound_aleft_cb                  : 4,	//  4: 7
	bound_aleft_y                   : 4;	//  8:11
} M14B0_FRC_DVO_MPG_BOUND_CTRL5;

/*-----------------------------------------------------------------------------
	0x02bc dvo_mpg_rec0_xpos ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	mpg_rec0_e_xpos                 :13,	//  0:12
	_rsvd_00                                : 3,	// 13:15 reserved
	mpg_rec0_s_xpos                 :13,	// 16:28
	_rsvd_01                                : 2,	// 29:30 reserved
	mpg_rec0_en                     : 1;	//    31
} M14B0_FRC_DVO_MPG_REC0_XPOS;

/*-----------------------------------------------------------------------------
	0x02c0 dvo_mpg_rec0_ypos ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	mpg_rec0_e_ypos                 :13,	//  0:12
	_rsvd_00                                : 3,	// 13:15 reserved
	mpg_rec0_s_ypos                 :13,	// 16:28
	_rsvd_01                                : 2,	// 29:30 reserved
	mpg_rec0_fill_en                : 1;	//    31
} M14B0_FRC_DVO_MPG_REC0_YPOS;

/*-----------------------------------------------------------------------------
	0x02c4 dvo_mpg_rec1_xpos ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	mpg_rec1_e_xpos                 :13,	//  0:12
	_rsvd_00                                : 3,	// 13:15 reserved
	mpg_rec1_s_xpos                 :13,	// 16:28
	_rsvd_01                                : 2,	// 29:30 reserved
	mpg_rec1_en                     : 1;	//    31
} M14B0_FRC_DVO_MPG_REC1_XPOS;

/*-----------------------------------------------------------------------------
	0x02c8 dvo_mpg_rec1_ypos ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	mpg_rec1_e_ypos                 :13,	//  0:12
	_rsvd_00                                : 3,	// 13:15 reserved
	mpg_rec1_s_ypos                 :13,	// 16:28
	_rsvd_01                                : 2,	// 29:30 reserved
	mpg_rec1_fill_en                : 1;	//    31
} M14B0_FRC_DVO_MPG_REC1_YPOS;

/*-----------------------------------------------------------------------------
	0x02cc dvo_mpg_rec2_xpos ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	mpg_rec2_e_xpos                 :13,	//  0:12
	_rsvd_00                                : 3,	// 13:15 reserved
	mpg_rec2_s_xpos                 :13,	// 16:28
	_rsvd_01                                : 2,	// 29:30 reserved
	mpg_rec2_en                     : 1;	//    31
} M14B0_FRC_DVO_MPG_REC2_XPOS;

/*-----------------------------------------------------------------------------
	0x02d0 dvo_mpg_rec2_ypos ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	mpg_rec2_e_ypos                 :13,	//  0:12
	_rsvd_00                                : 3,	// 13:15 reserved
	mpg_rec2_s_ypos                 :13,	// 16:28
	_rsvd_01                                : 2,	// 29:30 reserved
	mpg_rec2_fill_en                : 1;	//    31
} M14B0_FRC_DVO_MPG_REC2_YPOS;

/*-----------------------------------------------------------------------------
	0x02d4 dvo_mpg_rec3_xpos ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	mpg_rec3_e_xpos                 :13,	//  0:12
	_rsvd_00                                : 3,	// 13:15 reserved
	mpg_rec3_s_xpos                 :13,	// 16:28
	_rsvd_01                                : 2,	// 29:30 reserved
	mpg_rec3_en                     : 1;	//    31
} M14B0_FRC_DVO_MPG_REC3_XPOS;

/*-----------------------------------------------------------------------------
	0x02d8 dvo_mpg_rec3_ypos ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	mpg_rec3_e_ypos                 :13,	//  0:12
	_rsvd_00                                : 3,	// 13:15 reserved
	mpg_rec3_s_ypos                 :13,	// 16:28
	_rsvd_01                                : 2,	// 29:30 reserved
	mpg_rec3_fill_en                : 1;	//    31
} M14B0_FRC_DVO_MPG_REC3_YPOS;

/*-----------------------------------------------------------------------------
	0x02dc dvo_mpg_rec4_xpos ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	mpg_rec4_e_xpos                 :13,	//  0:12
	_rsvd_00                                : 3,	// 13:15 reserved
	mpg_rec4_s_xpos                 :13,	// 16:28
	_rsvd_01                                : 2,	// 29:30 reserved
	mpg_rec4_en                     : 1;	//    31
} M14B0_FRC_DVO_MPG_REC4_XPOS;

/*-----------------------------------------------------------------------------
	0x02e0 dvo_mpg_rec4_ypos ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	mpg_rec4_e_ypos                 :13,	//  0:12
	_rsvd_00                                : 3,	// 13:15 reserved
	mpg_rec4_s_ypos                 :13,	// 16:28
	_rsvd_01                                : 2,	// 29:30 reserved
	mpg_rec4_fill_en                : 1;	//    31
} M14B0_FRC_DVO_MPG_REC4_YPOS;

/*-----------------------------------------------------------------------------
	0x02e4 dvo_mpg_rec5_xpos ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	mpg_rec5_e_xpos                 :13,	//  0:12
	_rsvd_00                                : 3,	// 13:15 reserved
	mpg_rec5_s_xpos                 :13,	// 16:28
	_rsvd_01                                : 2,	// 29:30 reserved
	mpg_rec5_en                     : 1;	//    31
} M14B0_FRC_DVO_MPG_REC5_XPOS;

/*-----------------------------------------------------------------------------
	0x02e8 dvo_mpg_rec5_ypos ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	mpg_rec5_e_ypos                 :13,	//  0:12
	_rsvd_00                                : 3,	// 13:15 reserved
	mpg_rec5_s_ypos                 :13,	// 16:28
	_rsvd_01                                : 2,	// 29:30 reserved
	mpg_rec5_fill_en                : 1;	//    31
} M14B0_FRC_DVO_MPG_REC5_YPOS;

/*-----------------------------------------------------------------------------
	0x02ec dvo_mpg_rec6_xpos ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	mpg_rec6_e_xpos                 :13,	//  0:12
	_rsvd_00                                : 3,	// 13:15 reserved
	mpg_rec6_s_xpos                 :13,	// 16:28
	_rsvd_01                                : 2,	// 29:30 reserved
	mpg_rec6_en                     : 1;	//    31
} M14B0_FRC_DVO_MPG_REC6_XPOS;

/*-----------------------------------------------------------------------------
	0x02f0 dvo_mpg_rec6_ypos ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	mpg_rec6_e_ypos                 :13,	//  0:12
	_rsvd_00                                : 3,	// 13:15 reserved
	mpg_rec6_s_ypos                 :13,	// 16:28
	_rsvd_01                                : 2,	// 29:30 reserved
	mpg_rec6_fill_en                : 1;	//    31
} M14B0_FRC_DVO_MPG_REC6_YPOS;

/*-----------------------------------------------------------------------------
	0x02f4 dvo_mpg_rec7_xpos ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	mpg_rec7_e_xpos                 :13,	//  0:12
	_rsvd_00                                : 3,	// 13:15 reserved
	mpg_rec7_s_xpos                 :13,	// 16:28
	_rsvd_01                                : 2,	// 29:30 reserved
	mpg_rec7_en                     : 1;	//    31
} M14B0_FRC_DVO_MPG_REC7_XPOS;

/*-----------------------------------------------------------------------------
	0x02f8 dvo_mpg_rec7_ypos ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	mpg_rec7_e_ypos                 :13,	//  0:12
	_rsvd_00                                : 3,	// 13:15 reserved
	mpg_rec7_s_ypos                 :13,	// 16:28
	_rsvd_01                                : 2,	// 29:30 reserved
	mpg_rec7_fill_en                : 1;	//    31
} M14B0_FRC_DVO_MPG_REC7_YPOS;

/*-----------------------------------------------------------------------------
	0x02fc dvo_spg_line_ctrl0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	even_line_cr                    : 8,	//  0: 7
	even_line_cb                    : 8,	//  8:15
	even_line_y                     : 8,	// 16:23
	_rsvd                                : 7,	// 24:30 reserved
	even_line_pat_en                : 1;	//    31
} M14B0_FRC_DVO_SPG_LINE_CTRL0;

/*-----------------------------------------------------------------------------
	0x0300 dvo_spg_line_ctrl1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	odd_line_cr                     : 8,	//  0: 7
	odd_line_cb                     : 8,	//  8:15
	odd_line_y                      : 8,	// 16:23
	_rsvd                                : 7,	// 24:30 reserved
	odd_line_pat_en                 : 1;	//    31
} M14B0_FRC_DVO_SPG_LINE_CTRL1;

/*-----------------------------------------------------------------------------
	0x0310 dvo_mep_mv_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	view_melo                       : 1,	//     0
	view_melo_srch_aloc             : 1,	//     1
	view_mepo                       : 1,	//     2
	view_mero                       : 1,	//     3
	view_melp_full                  : 1,	//     4
	view_melp_fwd                   : 1,	//     5
	view_melp_map0                  : 1,	//     6
	view_melp_map1                  : 1,	//     7
	view_mero_ucc                   : 1,	//     8
	_rsvd_00                                : 5,	//  9:13 reserved
	assert_frame_rd_done            : 1,	//    14
	ignore_frame_rd_done            : 1,	//    15
	view_mepel                      : 2,	// 16:17
	_rsvd_01                                : 2,	// 18:19 reserved
	view_mepel_lv1_line_size        : 3,	// 20:22
	_rsvd_02                                : 1,	//    23 reserved
	view_mepel_lv2_line_size        : 3,	// 24:26
	_rsvd_03                                : 1,	//    27 reserved
	view_mepel_lv3_line_size        : 3,	// 28:30
	disable_mep_mv_mif              : 1;	//    31
} M14B0_FRC_DVO_MEP_MV_CTRL;

/*-----------------------------------------------------------------------------
	0x0314 dvo_mep_mv_lsize ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	view_melo_lv1_m_line_size       : 3,	//  0: 2
	_rsvd_00                                : 1,	//     3 reserved
	view_melo_lv2_m_line_size       : 3,	//  4: 6
	_rsvd_01                                : 1,	//     7 reserved
	view_melo_lv3_m_line_size       : 3,	//  8:10
	_rsvd_02                                : 1,	//    11 reserved
	view_melo_lv1_l_line_size       : 3,	// 12:14
	_rsvd_03                                : 1,	//    15 reserved
	view_melo_lv2_l_line_size       : 3,	// 16:18
	_rsvd_04                                : 1,	//    19 reserved
	view_melo_lv3_l_line_size       : 3,	// 20:22
	_rsvd_05                                : 1,	//    23 reserved
	view_mepo_line_size             : 3,	// 24:26
	_rsvd_06                                : 1,	//    27 reserved
	view_mero_line_size             : 3;	// 28:30
} M14B0_FRC_DVO_MEP_MV_LSIZE;

/*-----------------------------------------------------------------------------
	0x0318 dvo_mep_mv_msb_addr ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	view_mep_msb_addr               ;   	// 31: 0
} M14B0_FRC_DVO_MEP_MV_MSB_ADDR;

/*-----------------------------------------------------------------------------
	0x031c dvo_mep_mv_lsb_addr ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	view_mep_lsb_addr               ;   	// 31: 0
} M14B0_FRC_DVO_MEP_MV_LSB_ADDR;

/*-----------------------------------------------------------------------------
	0x0320 dvo_sg_vs_status0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	mon_msg_ssg0_vs_diff_cnt        :16,	//  0:15
	mon_msg_ssg_diff_ctrl           :16;	// 16:31
} M14B0_FRC_DVO_SG_VS_STATUS0;

/*-----------------------------------------------------------------------------
	0x0324 dvo_sg_vs_status1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	mon_msg_ssg2_vs_diff_cnt        :16,	//  0:15
	mon_msg_ssg1_vs_diff_cnt        :16;	// 16:31
} M14B0_FRC_DVO_SG_VS_STATUS1;

/*-----------------------------------------------------------------------------
	0x0328 dvo_sg_hs_status0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	mon_msg_ssg0_hs_diff_cnt        :16;	//  0:15
} M14B0_FRC_DVO_SG_HS_STATUS0;

/*-----------------------------------------------------------------------------
	0x032c dvo_sg_hs_status1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	mon_msg_ssg2_hs_diff_cnt        :16,	//  0:15
	mon_msg_ssg1_hs_diff_cnt        :16;	// 16:31
} M14B0_FRC_DVO_SG_HS_STATUS1;

/*-----------------------------------------------------------------------------
	0x0330 dvo_vernum ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x0334 dvo_current_stcc ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x0338 dvo_vs_stcc ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x033c dvo_dpwm_pin_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dpwm_pin_ctrl0                  : 5,	//  0: 4
	_rsvd_00                                : 2,	//  5: 6 reserved
	dpwm_pin_ctrl1                  : 5,	//  7:11
	_rsvd_01                                : 4,	// 12:15 reserved
	dpwm_pin_ctrl2                  : 5;	// 16:20
} M14B0_FRC_DVO_DPWM_PIN_CTRL;

/*-----------------------------------------------------------------------------
	0x0340 dvo_lr3d_sg_pin_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	lr3d_sg_pin_ctrl                : 5;	//  0: 4
} M14B0_FRC_DVO_LR3D_SG_PIN_CTRL;

/*-----------------------------------------------------------------------------
	0x0344 dvo_dco_mfc ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x0348 dvo_dco_dfc ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x034c dvo_dco_dflc ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x0350 dvo_dco_fcw ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dco_fcw                         :22;	//  0:21
} DCO_FCW;

/*-----------------------------------------------------------------------------
	0x0354 dvo_dco_num ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x0360 dvo_pwm_intr_status_en ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm0_tx_intr                    : 1,	//     0
	pwm1_tx_intr                    : 1,	//     1
	pwm2_tx_intr                    : 1,	//     2
	pwm3_rx_intr0                   : 1,	//     3
	pwm3_rx_intr1                   : 1;	//     4
} M14B0_FRC_DVO_PWM_INTR_STATUS_EN;

/*-----------------------------------------------------------------------------
	0x0364 dvo_pwm_intr_clear ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm0_tx_intr                    : 1,	//     0
	pwm1_tx_intr                    : 1,	//     1
	pwm2_tx_intr                    : 1,	//     2
	pwm3_rx_intr0                   : 1,	//     3
	pwm3_rx_intr1                   : 1;	//     4
} M14B0_FRC_DVO_PWM_INTR_CLEAR;

/*-----------------------------------------------------------------------------
	0x0368 dvo_pwm_intr_status ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm0_tx_intr                    : 1,	//     0
	pwm1_tx_intr                    : 1,	//     1
	pwm2_tx_intr                    : 1,	//     2
	pwm3_rx_intr0                   : 1,	//     3
	pwm3_rx_intr1                   : 1;	//     4
} M14B0_FRC_DVO_PWM_INTR_STATUS;

/*-----------------------------------------------------------------------------
	0x036c dvo_pwm_intr_mask ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm0_tx_intr                    : 1,	//     0
	pwm1_tx_intr                    : 1,	//     1
	pwm2_tx_intr                    : 1,	//     2
	pwm3_rx_intr0                   : 1,	//     3
	pwm3_rx_intr1                   : 1;	//     4
} M14B0_FRC_DVO_PWM_INTR_MASK;

/*-----------------------------------------------------------------------------
	0x0370 dvo_pwm0_ctrl0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm0_en                         : 1,	//     0
	pwm0_freq_mode                  : 3,	//  1: 3
	pwm0_resolution                 : 2,	//  4: 5
	pwm0_inv                        : 1,	//     6
	pwm0_sel                        : 1,	//     7
	pwm0_width_falling_pos          :24;	//  8:31
} M14B0_FRC_DVO_PWM0_CTRL0;

/*-----------------------------------------------------------------------------
	0x0374 dvo_pwm0_ctrl1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm0_free_width                 :24,	//  0:23
	_rsvd                                : 4,	// 24:27 reserved
	pwm0_end_ctrl0                  : 1,	//    28
	pwm0_end_ctrl1                  : 1;	//    29
} M14B0_FRC_DVO_PWM0_CTRL1;

/*-----------------------------------------------------------------------------
	0x0378 dvo_pwm0_ctrl2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm0_intr_mask                  :16,	//  0:15
	pwm0_method                     : 1,	//    16
	_rsvd_00                                : 3,	// 17:19 reserved
	pwm0_mux                        : 3,	// 20:22
	_rsvd_01                                : 1,	//    23 reserved
	pwm0_phase_mode                 : 2,	// 24:25
	pwm0_phase_index                : 3,	// 26:28
	_rsvd_02                                : 1,	//    29 reserved
	pwm0_fc_h_disp                  : 1,	//    30
	pwm0_fc_l_disp                  : 1;	//    31
} M14B0_FRC_DVO_PWM0_CTRL2;

/*-----------------------------------------------------------------------------
	0x037c dvo_pwm1_ctrl0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm1_en                         : 1,	//     0
	pwm1_freq_mode                  : 3,	//  1: 3
	pwm1_resolution                 : 2,	//  4: 5
	pwm1_inv                        : 1,	//     6
	pwm1_sel                        : 1,	//     7
	pwm1_width_falling_pos          :24;	//  8:31
} M14B0_FRC_DVO_PWM1_CTRL0;

/*-----------------------------------------------------------------------------
	0x0380 dvo_pwm1_ctrl1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm1_free_width                 :24,	//  0:23
	_rsvd                                : 4,	// 24:27 reserved
	pwm1_end_ctrl0                  : 1,	//    28
	pwm1_end_ctrl1                  : 1;	//    29
} M14B0_FRC_DVO_PWM1_CTRL1;

/*-----------------------------------------------------------------------------
	0x0384 dvo_pwm1_ctrl2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm1_intr_mask                  :16,	//  0:15
	pwm1_method                     : 1,	//    16
	_rsvd_00                                : 3,	// 17:19 reserved
	pwm1_mux                        : 3,	// 20:22
	_rsvd_01                                : 1,	//    23 reserved
	pwm1_phase_mode                 : 2,	// 24:25
	pwm1_phase_index                : 3,	// 26:28
	_rsvd_02                                : 1,	//    29 reserved
	pwm1_fc_h_disp                  : 1,	//    30
	pwm1_fc_l_disp                  : 1;	//    31
} M14B0_FRC_DVO_PWM1_CTRL2;

/*-----------------------------------------------------------------------------
	0x0388 dvo_pwm2_ctrl0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm2_en                         : 1,	//     0
	pwm2_freq_mode                  : 3,	//  1: 3
	pwm2_resolution                 : 2,	//  4: 5
	pwm2_inv                        : 1,	//     6
	pwm2_sel                        : 1,	//     7
	pwm2_width_falling_pos          :24;	//  8:31
} M14B0_FRC_DVO_PWM2_CTRL0;

/*-----------------------------------------------------------------------------
	0x038c dvo_pwm2_ctrl1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm2_free_width                 :24,	//  0:23
	_rsvd                                : 4,	// 24:27 reserved
	pwm2_end_ctrl0                  : 1,	//    28
	pwm2_end_ctrl1                  : 1;	//    29
} M14B0_FRC_DVO_PWM2_CTRL1;

/*-----------------------------------------------------------------------------
	0x0390 dvo_pwm2_ctrl2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm2_intr_mask                  :16,	//  0:15
	pwm2_method                     : 1,	//    16
	_rsvd_00                                : 3,	// 17:19 reserved
	pwm2_mux                        : 3,	// 20:22
	_rsvd_01                                : 1,	//    23 reserved
	pwm2_phase_mode                 : 2,	// 24:25
	pwm2_phase_index                : 3,	// 26:28
	_rsvd_02                                : 1,	//    29 reserved
	pwm2_fc_h_disp                  : 1,	//    30
	pwm2_fc_l_disp                  : 1;	//    31
} M14B0_FRC_DVO_PWM2_CTRL2;

/*-----------------------------------------------------------------------------
	0x0394 dvo_pwm3_ctrl0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm3_en                         : 1,	//     0
	pwm3_freq_mode                  : 3,	//  1: 3
	_rsvd                                : 4,	//  4: 7 reserved
	pwm3_threshold                  :24;	//  8:31
} M14B0_FRC_DVO_PWM3_CTRL0;

/*-----------------------------------------------------------------------------
	0x0398 dvo_pwm3_ctrl1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm3_intr_mask1                 : 8,	//  0: 7
	_rsvd_00                                : 8,	//  8:15 reserved
	pwm3_intr_mask0                 : 1,	//    16
	_rsvd_01                                : 3,	// 17:19 reserved
	pwm3_in_mux                     : 2;	// 20:21
} M14B0_FRC_DVO_PWM3_CTRL1;

/*-----------------------------------------------------------------------------
	0x039c dvo_pwm3_0_low ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm3_0_low                      :24;	//  0:23
} M14B0_FRC_DVO_PWM3_0_LOW;

/*-----------------------------------------------------------------------------
	0x03a0 dvo_pwm3_0_high ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm3_0_high                     :24;	//  0:23
} M14B0_FRC_DVO_PWM3_0_HIGH;

/*-----------------------------------------------------------------------------
	0x03a4 dvo_pwm3_1_low ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm3_1_low                      :24;	//  0:23
} M14B0_FRC_DVO_PWM3_1_LOW;

/*-----------------------------------------------------------------------------
	0x03a8 dvo_pwm3_1_high ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm3_1_high                     :24;	//  0:23
} M14B0_FRC_DVO_PWM3_1_HIGH;

/*-----------------------------------------------------------------------------
	0x03ac dvo_pwm3_2_low ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm3_2_low                      :24;	//  0:23
} M14B0_FRC_DVO_PWM3_2_LOW;

/*-----------------------------------------------------------------------------
	0x03b0 dvo_pwm3_2_high ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm3_2_high                     :24;	//  0:23
} M14B0_FRC_DVO_PWM3_2_HIGH;

/*-----------------------------------------------------------------------------
	0x03b4 dvo_pwm3_3_low ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm3_3_low                      :24;	//  0:23
} M14B0_FRC_DVO_PWM3_3_LOW;

/*-----------------------------------------------------------------------------
	0x03b8 dvo_pwm3_3_high ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm3_3_high                     :24;	//  0:23
} M14B0_FRC_DVO_PWM3_3_HIGH;

/*-----------------------------------------------------------------------------
	0x03bc dvo_pwm3_4_low ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm3_4_low                      :24;	//  0:23
} M14B0_FRC_DVO_PWM3_4_LOW;

/*-----------------------------------------------------------------------------
	0x03c0 dvo_pwm3_4_high ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm3_4_high                     :24;	//  0:23
} M14B0_FRC_DVO_PWM3_4_HIGH;

/*-----------------------------------------------------------------------------
	0x03c4 dvo_pwm3_5_low ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm3_5_low                      :24;	//  0:23
} M14B0_FRC_DVO_PWM3_5_LOW;

/*-----------------------------------------------------------------------------
	0x03c8 dvo_pwm3_5_high ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm3_5_high                     :24;	//  0:23
} M14B0_FRC_DVO_PWM3_5_HIGH;

/*-----------------------------------------------------------------------------
	0x03cc dvo_pwm3_6_low ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm3_6_low                      :24;	//  0:23
} M14B0_FRC_DVO_PWM3_6_LOW;

/*-----------------------------------------------------------------------------
	0x03d0 dvo_pwm3_6_high ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm3_6_high                     :24;	//  0:23
} M14B0_FRC_DVO_PWM3_6_HIGH;

/*-----------------------------------------------------------------------------
	0x03d4 dvo_pwm3_7_low ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm3_7_low                      :24;	//  0:23
} M14B0_FRC_DVO_PWM3_7_LOW;

/*-----------------------------------------------------------------------------
	0x03d8 dvo_pwm3_7_high ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm3_7_high                     :24;	//  0:23
} M14B0_FRC_DVO_PWM3_7_HIGH;

/*-----------------------------------------------------------------------------
	0x03dc dvo_pwm_v_load_write ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm0_v_we                       : 1,	//     0
	pwm1_v_we                       : 1,	//     1
	pwm2_v_we                       : 1,	//     2
	pwm0_v_sub_we                   : 1,	//     3
	pwm1_v_sub_we                   : 1,	//     4
	pwm2_v_sub_we                   : 1,	//     5
	_rsvd                                : 2,	//  6: 7 reserved
	pwm0_v_sub_f_we                 : 1,	//     8
	pwm1_v_sub_f_we                 : 1,	//     9
	pwm2_v_sub_f_we                 : 1;	//    10
} M14B0_FRC_DVO_PWM_V_LOAD_WRITE;

/*-----------------------------------------------------------------------------
	0x03e0 dvo_pwm0_v_r ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm0_v_r                        :16,	//  0:15
	_rsvd                                :12,	// 16:27 reserved
	pwm0_v_r_id                     : 4;	// 28:31
} M14B0_FRC_DVO_PWM0_V_R;

/*-----------------------------------------------------------------------------
	0x03e4 dvo_pwm0_v_f ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm0_v_f                        :16,	//  0:15
	_rsvd                                :12,	// 16:27 reserved
	pwm0_v_f_id                     : 4;	// 28:31
} M14B0_FRC_DVO_PWM0_V_F;

/*-----------------------------------------------------------------------------
	0x03e8 dvo_pwm1_v_r ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm1_v_r                        :16,	//  0:15
	_rsvd                                :12,	// 16:27 reserved
	pwm1_v_r_id                     : 4;	// 28:31
} M14B0_FRC_DVO_PWM1_V_R;

/*-----------------------------------------------------------------------------
	0x03ec dvo_pwm1_v_f ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm1_v_f                        :16,	//  0:15
	_rsvd                                :12,	// 16:27 reserved
	pwm1_v_f_id                     : 4;	// 28:31
} M14B0_FRC_DVO_PWM1_V_F;

/*-----------------------------------------------------------------------------
	0x03f0 dvo_pwm2_v_r ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm2_v_r                        :16,	//  0:15
	_rsvd                                :12,	// 16:27 reserved
	pwm2_v_r_id                     : 4;	// 28:31
} M14B0_FRC_DVO_PWM2_V_R;

/*-----------------------------------------------------------------------------
	0x03f4 dvo_pwm2_v_f ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm2_v_f                        :16,	//  0:15
	_rsvd                                :12,	// 16:27 reserved
	pwm2_v_f_id                     : 4;	// 28:31
} M14B0_FRC_DVO_PWM2_V_F;

/*-----------------------------------------------------------------------------
	0x03f8 dvo_pwm0_v_sub_r ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm0_v_sub                      :16,	//  0:15
	_rsvd                                :12,	// 16:27 reserved
	pwm0_v_sub_id                   : 4;	// 28:31
} M14B0_FRC_DVO_PWM0_V_SUB_R;

/*-----------------------------------------------------------------------------
	0x03fc dvo_pwm1_v_sub_r ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm1_v_sub                      :16,	//  0:15
	_rsvd                                :12,	// 16:27 reserved
	pwm1_v_sub_id                   : 4;	// 28:31
} M14B0_FRC_DVO_PWM1_V_SUB_R;

/*-----------------------------------------------------------------------------
	0x0400 dvo_pwm2_v_sub_r ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm2_v_sub                      :16,	//  0:15
	_rsvd                                :12,	// 16:27 reserved
	pwm2_v_sub_id                   : 4;	// 28:31
} M14B0_FRC_DVO_PWM2_V_SUB_R;

/*-----------------------------------------------------------------------------
	0x0404 dvo_pwm0_lock_pulse_xy ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm0_lock_pulse_x               :13,	//  0:12
	_rsvd_00                                : 3,	// 13:15 reserved
	pwm0_lock_pulse_y               :13,	// 16:28
	_rsvd_01                                : 2,	// 29:30 reserved
	pwm0_lock_pulse_half_en         : 1;	//    31
} M14B0_FRC_DVO_PWM0_LOCK_PULSE_XY;

/*-----------------------------------------------------------------------------
	0x0408 dvo_pwm1_lock_pulse_xy ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm1_lock_pulse_x               :13,	//  0:12
	_rsvd_00                                : 3,	// 13:15 reserved
	pwm1_lock_pulse_y               :13,	// 16:28
	_rsvd_01                                : 2,	// 29:30 reserved
	pwm1_lock_pulse_half_en         : 1;	//    31
} M14B0_FRC_DVO_PWM1_LOCK_PULSE_XY;

/*-----------------------------------------------------------------------------
	0x040c dvo_pwm2_lock_pulse_xy ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm2_lock_pulse_x               :13,	//  0:12
	_rsvd_00                                : 3,	// 13:15 reserved
	pwm2_lock_pulse_y               :13,	// 16:28
	_rsvd_01                                : 2,	// 29:30 reserved
	pwm2_lock_pulse_half_en         : 1;	//    31
} M14B0_FRC_DVO_PWM2_LOCK_PULSE_XY;

/*-----------------------------------------------------------------------------
	0x0410 dvo_pwm3_lock_pulse_xy ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm3_lock_pulse_x               :13,	//  0:12
	_rsvd_00                                : 3,	// 13:15 reserved
	pwm3_lock_pulse_y               :13,	// 16:28
	_rsvd_01                                : 2,	// 29:30 reserved
	pwm3_lock_pulse_half_en         : 1;	//    31
} M14B0_FRC_DVO_PWM3_LOCK_PULSE_XY;

/*-----------------------------------------------------------------------------
	0x0414 dvo_pwm0_width ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x0418 dvo_pwm0_offset ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x041c dvo_pwm0_offset_sum ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x0420 dvo_pwm1_width ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x0424 dvo_pwm1_offset ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x0428 dvo_pwm1_offset_sum ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x042c dvo_pwm2_width ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x0430 dvo_pwm2_offset ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x0434 dvo_pwm2_offset_sum ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x0438 dvo_pwm_master ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	_rsvd_00                                :16,	//  0:15 reserved
	pwm_master_mode                 : 3,	// 16:18
	_rsvd_01                                : 1,	//    19 reserved
	pwm2_ld_pwm                     : 1,	//    20
	_rsvd_02                                : 2,	// 21:22 reserved
	pwm2_ld_pwm_tx_mode             : 1,	//    23
	pwm1_ld_pwm                     : 1,	//    24
	_rsvd_03                                : 2,	// 25:26 reserved
	pwm1_ld_pwm_tx_mode             : 1,	//    27
	pwm0_ld_pwm                     : 1,	//    28
	_rsvd_04                                : 2,	// 29:30 reserved
	pwm0_ld_pwm_tx_mode             : 1;	//    31
} M14B0_FRC_DVO_PWM_MASTER;

/*-----------------------------------------------------------------------------
	0x043c dvo_pwm0_v_sub_f ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm0_v_sub_f                    :16,	//  0:15
	_rsvd                                :12,	// 16:27 reserved
	pwm0_v_sub_f_id                 : 4;	// 28:31
} M14B0_FRC_DVO_PWM0_V_SUB_F;

/*-----------------------------------------------------------------------------
	0x0440 dvo_pwm1_v_sub_f ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm1_v_sub_f                    :16,	//  0:15
	_rsvd                                :12,	// 16:27 reserved
	pwm1_v_sub_f_id                 : 4;	// 28:31
} M14B0_FRC_DVO_PWM1_V_SUB_F;

/*-----------------------------------------------------------------------------
	0x0444 dvo_pwm2_v_sub_f ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm2_v_sub_f                    :16,	//  0:15
	_rsvd                                :12,	// 16:27 reserved
	pwm2_v_sub_f_id                 : 4;	// 28:31
} M14B0_FRC_DVO_PWM2_V_SUB_F;

typedef struct {
	M14B0_FRC_DVO_SRESET0                     	dvo_sreset0                     ;	// 0x0000 : ''
	M14B0_FRC_DVO_SRESET1                     	dvo_sreset1                     ;	// 0x0004 : ''
	M14B0_FRC_DVO_PIC_INIT                    	dvo_pic_init                    ;	// 0x0008 : ''
	M14B0_FRC_DVO_PIC_START                   	dvo_pic_start                   ;	// 0x000c : ''
	M14B0_FRC_DVOM_PULSE_M_CTRL               	dvom_pulse_m_ctrl               ;	// 0x0010 : ''
	M14B0_FRC_DVOS_PULSE_M_CTRL               	dvos_pulse_m_ctrl               ;	// 0x0014 : ''
	M14B0_FRC_DVOM_PIC_INIT_M_TIMER           	dvom_pic_init_m_timer           ;	// 0x0018 : ''
	M14B0_FRC_DVOS_PIC_INIT_M_TIMER           	dvos_pic_init_m_timer           ;	// 0x001c : ''
	UINT32                          	                 _rsvd_00[   2];	// 0x0020 ~ 0x0024
	M14B0_FRC_DVO_PIC_END_CTRL                	dvo_pic_end_ctrl                ;	// 0x0028 : ''
	M14B0_FRC_DVO_PIC_END_INFO                	dvo_pic_end_info                ;	// 0x002c : ''
	M14B0_FRC_DVOM_GMAU_CTRL                  	dvom_gmau_ctrl                  ;	// 0x0030 : ''
	M14B0_FRC_DVOS_GMAU_CTRL                  	dvos_gmau_ctrl                  ;	// 0x0034 : ''
	M14B0_FRC_DVO_RST_ASYNC_BRIDGE          dvo_rst_async_bridge                ;	// 0x0038 : ''
	M14B0_FRC_DVOM_INTR_MASK                  	dvom_intr_mask                  ;	// 0x003c : ''
	M14B0_FRC_DVOM_INTR_CLEAR                 	dvom_intr_clear                 ;	// 0x0040 : ''
	M14B0_FRC_DVOM_INTR_STATUS                	dvom_intr_status                ;	// 0x0044 : ''
	M14B0_FRC_DVOS_INTR_MASK                  	dvos_intr_mask                  ;	// 0x0048 : ''
	M14B0_FRC_DVOS_INTR_CLEAR                 	dvos_intr_clear                 ;	// 0x004c : ''
	M14B0_FRC_DVOS_INTR_STATUS                	dvos_intr_status                ;	// 0x0050 : ''
	M14B0_FRC_DVOM_INTR0_POS                  	dvom_intr0_pos                  ;	// 0x0054 : ''
	M14B0_FRC_DVOM_INTR1_POS                  	dvom_intr1_pos                  ;	// 0x0058 : ''
	M14B0_FRC_DVOM_INTR2_POS                  	dvom_intr2_pos                  ;	// 0x005c : ''
	M14B0_FRC_DVOM_INTR3_POS                  	dvom_intr3_pos                  ;	// 0x0060 : ''
	UINT32                          	                 _rsvd_01[   2];	// 0x0064 ~ 0x0068
	M14B0_FRC_DVOM_FRAME_CTRL                 	dvom_frame_ctrl                 ;	// 0x006c : ''
	M14B0_FRC_DVOS_FRAME_CTRL                 	dvos_frame_ctrl                 ;	// 0x0070 : ''
	M14B0_FRC_DVOM_STATUS                     	dvom_status                     ;	// 0x0074 : ''
	M14B0_FRC_DVOS_STATUS                     	dvos_status                     ;	// 0x0078 : ''
	UINT32                          	                 _rsvd_02[   1];	// 0x007c
	M14B0_FRC_DVOM_SYNC0                      	dvom_sync0                      ;	// 0x0080 : ''
	M14B0_FRC_DVOM_SYNC1                      	dvom_sync1                      ;	// 0x0084 : ''
	M14B0_FRC_DVOM_SYNC2                      	dvom_sync2                      ;	// 0x0088 : ''
	M14B0_FRC_DVOM_SYNC3                      	dvom_sync3                      ;	// 0x008c : ''
	M14B0_FRC_DVOS0_SYNC0                     	dvos0_sync0                     ;	// 0x0090 : ''
	M14B0_FRC_DVOS0_SYNC1                     	dvos0_sync1                     ;	// 0x0094 : ''
	M14B0_FRC_DVOS0_SYNC2                     	dvos0_sync2                     ;	// 0x0098 : ''
	M14B0_FRC_DVOS0_SYNC3                     	dvos0_sync3                     ;	// 0x009c : ''
	M14B0_FRC_DVOS1_SYNC0                     	dvos1_sync0                     ;	// 0x00a0 : ''
	M14B0_FRC_DVOS1_SYNC1                     	dvos1_sync1                     ;	// 0x00a4 : ''
	M14B0_FRC_DVOS1_SYNC2                     	dvos1_sync2                     ;	// 0x00a8 : ''
	M14B0_FRC_DVOS1_SYNC3                     	dvos1_sync3                     ;	// 0x00ac : ''
	M14B0_FRC_DVOS2_SYNC0                     	dvos2_sync0                     ;	// 0x00b0 : ''
	M14B0_FRC_DVOS2_SYNC1                     	dvos2_sync1                     ;	// 0x00b4 : ''
	M14B0_FRC_DVOS2_SYNC2                     	dvos2_sync2                     ;	// 0x00b8 : ''
	M14B0_FRC_DVOS2_SYNC3                     	dvos2_sync3                     ;	// 0x00bc : ''
	M14B0_FRC_DVO_MSG_CTRL0                   	dvo_msg_ctrl0                   ;	// 0x00c0 : ''
	M14B0_FRC_DVO_MSG_CTRL1                   	dvo_msg_ctrl1                   ;	// 0x00c4 : ''
	M14B0_FRC_DVO_MSG_CTRL2                   	dvo_msg_ctrl2                   ;	// 0x00c8 : ''
	M14B0_FRC_DVO_MSG_CTRL3                   	dvo_msg_ctrl3                   ;	// 0x00cc : ''
	M14B0_FRC_DVO_SSG0_CTRL                   	dvo_ssg0_ctrl                   ;	// 0x00d0 : ''
	M14B0_FRC_DVOS_SSG0_DLY                   	dvos_ssg0_dly                   ;	// 0x00d4 : ''
	M14B0_FRC_DVOS_SSG0_DLY_START             	dvos_ssg0_dly_start             ;	// 0x00d8 : ''
	UINT32                          	                 _rsvd_03[   1];	// 0x00dc
	M14B0_FRC_DVO_SSG1_CTRL                   	dvo_ssg1_ctrl                   ;	// 0x00e0 : ''
	M14B0_FRC_DVOS_SSG1_DLY                   	dvos_ssg1_dly                   ;	// 0x00e4 : ''
	M14B0_FRC_DVOS_SSG1_DLY_START             	dvos_ssg1_dly_start             ;	// 0x00e8 : ''
	UINT32                          	dvos_ssg1_param                 ;	// 0x00ec : ''
	M14B0_FRC_DVO_SSG2_CTRL                   	dvo_ssg2_ctrl                   ;	// 0x00f0 : ''
	M14B0_FRC_DVOS_SSG2_DLY                   	dvos_ssg2_dly                   ;	// 0x00f4 : ''
	M14B0_FRC_DVOS_SSG2_DLY_START             	dvos_ssg2_dly_start             ;	// 0x00f8 : ''
	UINT32                          	                 _rsvd_04[   1];	// 0x00fc
	M14B0_FRC_DVOM_IO_TYPE                    	dvom_io_type                    ;	// 0x0100 : ''
	M14B0_FRC_DVOS_IO_TYPE                    	dvos_io_type                    ;	// 0x0104 : ''
	M14B0_FRC_DVOM_DISP_SIZE                  	dvom_disp_size                  ;	// 0x0108 : ''
	M14B0_FRC_DVOM_WIN_L_I_POS                	dvom_win_l_i_pos                ;	// 0x010c : ''
	M14B0_FRC_DVOM_WIN_L_I_SIZE               	dvom_win_l_i_size               ;	// 0x0110 : ''
	UINT32                          	                 _rsvd_05[   2];	// 0x0114 ~ 0x0118
	M14B0_FRC_DVOM_WIN_O_POS                  	dvom_win_o_pos                  ;	// 0x011c : ''
	M14B0_FRC_DVOM_WIN_O_SIZE                 	dvom_win_o_size                 ;	// 0x0120 : ''
	M14B0_FRC_DVOS_WIN_I_POS                  	dvos_win_i_pos                  ;	// 0x0124 : ''
	M14B0_FRC_DVOS_WIN_I_SIZE                 	dvos_win_i_size                 ;	// 0x0128 : ''
	M14B0_FRC_DVOS_WIN_O_POS                  	dvos_win_o_pos                  ;	// 0x012c : ''
	M14B0_FRC_DVOS_WIN_O_SIZE                 	dvos_win_o_size                 ;	// 0x0130 : ''
	M14B0_FRC_DVO_CDC_CTRL                    	dvo_cdc_ctrl                    ;	// 0x0134 : ''
	UINT32                          	                 _rsvd_06[   1];	// 0x0138
	M14B0_FRC_DVOM_HSC_CTRL0                  	dvom_hsc_ctrl0                  ;	// 0x013c : ''
	M14B0_FRC_DVOM_HSC_CTRL1                  	dvom_hsc_ctrl1                  ;	// 0x0140 : ''
	M14B0_FRC_DVOM_HSC_CTRL2                  	dvom_hsc_ctrl2                  ;	// 0x0144 : ''
	M14B0_FRC_DVOM_HSC_CTRL3                  	dvom_hsc_ctrl3                  ;	// 0x0148 : ''
	M14B0_FRC_DVOM_HSC_COEF0                  	dvom_hsc_coef0                  ;	// 0x014c : ''
	M14B0_FRC_DVOM_HSC_COEF1                  	dvom_hsc_coef1                  ;	// 0x0150 : ''
	M14B0_FRC_DVOM_HSC_COEF2                  	dvom_hsc_coef2                  ;	// 0x0154 : ''
	M14B0_FRC_DVOM_HSC_COEF3                  	dvom_hsc_coef3                  ;	// 0x0158 : ''
	M14B0_FRC_DVOM_HSC_COEF4                  	dvom_hsc_coef4                  ;	// 0x015c : ''
	M14B0_FRC_DVOM_HSC_COEF5                  	dvom_hsc_coef5                  ;	// 0x0160 : ''
	M14B0_FRC_DVOM_HSC_COEF6                  	dvom_hsc_coef6                  ;	// 0x0164 : ''
	M14B0_FRC_DVOM_HSC_COEF7                  	dvom_hsc_coef7                  ;	// 0x0168 : ''
	M14B0_FRC_DVOM_HSC_COEF8                  	dvom_hsc_coef8                  ;	// 0x016c : ''
	M14B0_FRC_DVOS_HSC_CTRL0                  	dvos_hsc_ctrl0                  ;	// 0x0170 : ''
	M14B0_FRC_DVOS_HSC_COEF0                  	dvos_hsc_coef0                  ;	// 0x0174 : ''
	M14B0_FRC_DVOS_HSC_COEF1                  	dvos_hsc_coef1                  ;	// 0x0178 : ''
	M14B0_FRC_DVOM_MDO_CTRL0                  	dvom_mdo_ctrl0                  ;	// 0x017c : ''
	M14B0_FRC_DVOM_MDO_CTRL1                  	dvom_mdo_ctrl1                  ;	// 0x0180 : ''
	M14B0_FRC_DVOM_MDO_CTRL2                  	dvom_mdo_ctrl2                  ;	// 0x0184 : ''
	M14B0_FRC_DVOM_MIF_STATUS1                	dvom_mif_status1                ;	// 0x0188 : ''
	M14B0_FRC_DVOS_MIF_STATUS1                	dvos_mif_status1                ;	// 0x018c : ''
	M14B0_FRC_DVOM_RMIF_CTRL0                 	dvom_rmif_ctrl0                 ;	// 0x0190 : ''
	M14B0_FRC_DVOS_RMIF_CTRL0                 	dvos_rmif_ctrl0                 ;	// 0x0194 : ''
	M14B0_FRC_DVOM_MIF_STATUS                 	dvom_mif_status                 ;	// 0x0198 : ''
	M14B0_FRC_DVOS_MIF_STATUS                 	dvos_mif_status                 ;	// 0x019c : ''
	UINT32                          	dvom_saddr_8_rd_l_y_a           ;	// 0x01a0 : ''
	UINT32                          	dvom_saddr_2_rd_l_y_a           ;	// 0x01a4 : ''
	UINT32                          	dvom_saddr_8_rd_l_c_a           ;	// 0x01a8 : ''
	UINT32                          	dvom_saddr_2_rd_l_c_a           ;	// 0x01ac : ''
	UINT32                          	                 _rsvd_07[   4];	// 0x01b0 ~ 0x01bc
	UINT32                          	dvom_saddr_8_rd_r_y_a           ;	// 0x01c0 : ''
	UINT32                          	dvom_saddr_2_rd_r_y_a           ;	// 0x01c4 : ''
	UINT32                          	dvom_saddr_8_rd_r_c_a           ;	// 0x01c8 : ''
	UINT32                          	dvom_saddr_2_rd_r_c_a           ;	// 0x01cc : ''
	M14B0_FRC_DVOM_RMIF_PAT_CTRL0             	dvom_rmif_pat_ctrl0             ;	// 0x01d0 : ''
	M14B0_FRC_DVOM_RMIF_PAT_CTRL1             	dvom_rmif_pat_ctrl1             ;	// 0x01d4 : ''
	M14B0_FRC_DVOS_RMIF_PAT_CTRL0             	dvos_rmif_pat_ctrl0             ;	// 0x01d8 : ''
	M14B0_FRC_DVOS_RMIF_PAT_CTRL1             	dvos_rmif_pat_ctrl1             ;	// 0x01dc : ''
	UINT32                          	dvos_saddr_8_rd_y_a             ;	// 0x01e0 : ''
	UINT32                          	dvos_saddr_2_rd_y_a             ;	// 0x01e4 : ''
	UINT32                          	dvos_saddr_8_rd_c_a             ;	// 0x01e8 : ''
	UINT32                          	dvos_saddr_2_rd_c_a             ;	// 0x01ec : ''
	UINT32                          	dvos_saddr_8_rd_y_b             ;	// 0x01f0 : ''
	UINT32                          	dvos_saddr_2_rd_y_b             ;	// 0x01f4 : ''
	UINT32                          	dvos_saddr_8_rd_c_b             ;	// 0x01f8 : ''
	UINT32                          	dvos_saddr_2_rd_c_b             ;	// 0x01fc : ''
	M14B0_FRC_DVOM_MIF_L_END_Y                	dvom_mif_l_end_y                ;	// 0x0200 : ''
	M14B0_FRC_DVOM_MIF_R_END_Y                	dvom_mif_r_end_y                ;	// 0x0204 : ''
	M14B0_FRC_DVOS_MIF_END_Y                  	dvos_mif_end_y                  ;	// 0x0208 : ''
	M14B0_FRC_DVOM_STRIDE8_CTRL               	dvom_stride8_ctrl               ;	// 0x020c : ''
	M14B0_FRC_DVOM_STRIDE2_CTRL               	dvom_stride2_ctrl               ;	// 0x0210 : ''
	M14B0_FRC_DVOS_STRIDE8_CTRL               	dvos_stride8_ctrl               ;	// 0x0214 : ''
	M14B0_FRC_DVOS_STRIDE2_CTRL               	dvos_stride2_ctrl               ;	// 0x0218 : ''
	M14B0_FRC_DVOM_GMAU_STRIDE_CTRL           	dvom_gmau_stride_ctrl           ;	// 0x021c : ''
	M14B0_FRC_DVOS_GMAU_STRIDE_CTRL           	dvos_gmau_stride_ctrl           ;	// 0x0220 : ''
	M14B0_FRC_DVO_MIFM_PXL_SIZE_CTRL          	dvo_mifm_pxl_size_ctrl          ;	// 0x0224 : ''
	M14B0_FRC_DVO_MIFS_PXL_SIZE_CTRL          	dvo_mifs_pxl_size_ctrl          ;	// 0x0228 : ''
	UINT32                          	                 _rsvd_08[   1];	// 0x022c
	M14B0_FRC_DVO_RHOST_MIF_CTRL0             	dvo_rhost_mif_ctrl0             ;	// 0x0230 : ''
	M14B0_FRC_DVO_RHOST_MIF_CTRL1             	dvo_rhost_mif_ctrl1             ;	// 0x0234 : ''
	UINT32                          	                 _rsvd_09[   1];	// 0x0238
	M14B0_FRC_DVOM_RHOST_MIF_CTRL0            	dvom_rhost_mif_ctrl0            ;	// 0x023c : ''
	M14B0_FRC_DVOM_RHOST_MIF_CTRL1            	dvom_rhost_mif_ctrl1            ;	// 0x0240 : ''
	M14B0_FRC_DVOS_RHOST_MIF_CTRL0            	dvos_rhost_mif_ctrl0            ;	// 0x0244 : ''
	M14B0_FRC_DVOS_RHOST_MIF_CTRL1            	dvos_rhost_mif_ctrl1            ;	// 0x0248 : ''
	UINT32                          	                 _rsvd_10[   1];	// 0x024c
	M14B0_FRC_DVOM_BLOCK_CMD_L_YPOS0          	dvom_block_cmd_l_ypos0          ;	// 0x0250 : ''
	M14B0_FRC_DVOM_BLOCK_CMD_L_YPOS1          	dvom_block_cmd_l_ypos1          ;	// 0x0254 : ''
	M14B0_FRC_DVOM_LM_STATUS_CTRL             	dvom_lm_status_ctrl             ;	// 0x0258 : ''
	M14B0_FRC_DVOS_LM_STATUS_CTRL             	dvos_lm_status_ctrl             ;	// 0x025c : ''
	M14B0_FRC_DVOS_BLOCK_CMD_YPOS0            	dvos_block_cmd_ypos0            ;	// 0x0260 : ''
	M14B0_FRC_DVOS_BLOCK_CMD_YPOS1            	dvos_block_cmd_ypos1            ;	// 0x0264 : ''
	M14B0_FRC_DVOM_RMIF_CTRL1                 	dvom_rmif_ctrl1                 ;	// 0x0268 : ''
	M14B0_FRC_DVOS_RMIF_CTRL1                 	dvos_rmif_ctrl1                 ;	// 0x026c : ''
	M14B0_FRC_DVO_MPG_PAT_CTRL                	dvo_mpg_pat_ctrl                ;	// 0x0270 : ''
	M14B0_FRC_DVO_SPG_PAT_CTRL                	dvo_spg_pat_ctrl                ;	// 0x0274 : ''
	M14B0_FRC_DVO_MPG_PIXEL_READ_CTRL         	dvo_mpg_pixel_read_ctrl         ;	// 0x0278 : ''
	M14B0_FRC_DVO_MPG_PIXEL_READ_DATA         	dvo_mpg_pixel_read_data         ;	// 0x027c : ''
	M14B0_FRC_DVO_SPG_PIXEL_READ_CTRL         	dvo_spg_pixel_read_ctrl         ;	// 0x0280 : ''
	M14B0_FRC_DVO_SPG_PIXEL_READ_DATA         	dvo_spg_pixel_read_data         ;	// 0x0284 : ''
	M14B0_FRC_DVO_MPG_LINE_CTRL0              	dvo_mpg_line_ctrl0              ;	// 0x0288 : ''
	M14B0_FRC_DVO_MPG_LINE_CTRL1              	dvo_mpg_line_ctrl1              ;	// 0x028c : ''
	M14B0_FRC_DVO_MPG_BOX0_CTRL               	dvo_mpg_box0_ctrl               ;	// 0x0290 : ''
	M14B0_FRC_DVO_MPG_BOX1_CTRL               	dvo_mpg_box1_ctrl               ;	// 0x0294 : ''
	M14B0_FRC_DVO_MPG_BOX23_CTRL              	dvo_mpg_box23_ctrl              ;	// 0x0298 : ''
	M14B0_FRC_DVO_MPG_BOX45_CTRL              	dvo_mpg_box45_ctrl              ;	// 0x029c : ''
	M14B0_FRC_DVO_MPG_BOX67_CTRL              	dvo_mpg_box67_ctrl              ;	// 0x02a0 : ''
	M14B0_FRC_DVO_MPG_BOUND_CTRL0             	dvo_mpg_bound_ctrl0             ;	// 0x02a4 : ''
	M14B0_FRC_DVO_MPG_BOUND_CTRL1             	dvo_mpg_bound_ctrl1             ;	// 0x02a8 : ''
	M14B0_FRC_DVO_MPG_BOUND_CTRL2             	dvo_mpg_bound_ctrl2             ;	// 0x02ac : ''
	M14B0_FRC_DVO_MPG_BOUND_CTRL3             	dvo_mpg_bound_ctrl3             ;	// 0x02b0 : ''
	M14B0_FRC_DVO_MPG_BOUND_CTRL4             	dvo_mpg_bound_ctrl4             ;	// 0x02b4 : ''
	M14B0_FRC_DVO_MPG_BOUND_CTRL5             	dvo_mpg_bound_ctrl5             ;	// 0x02b8 : ''
	M14B0_FRC_DVO_MPG_REC0_XPOS               	dvo_mpg_rec0_xpos               ;	// 0x02bc : ''
	M14B0_FRC_DVO_MPG_REC0_YPOS               	dvo_mpg_rec0_ypos               ;	// 0x02c0 : ''
	M14B0_FRC_DVO_MPG_REC1_XPOS               	dvo_mpg_rec1_xpos               ;	// 0x02c4 : ''
	M14B0_FRC_DVO_MPG_REC1_YPOS               	dvo_mpg_rec1_ypos               ;	// 0x02c8 : ''
	M14B0_FRC_DVO_MPG_REC2_XPOS               	dvo_mpg_rec2_xpos               ;	// 0x02cc : ''
	M14B0_FRC_DVO_MPG_REC2_YPOS               	dvo_mpg_rec2_ypos               ;	// 0x02d0 : ''
	M14B0_FRC_DVO_MPG_REC3_XPOS               	dvo_mpg_rec3_xpos               ;	// 0x02d4 : ''
	M14B0_FRC_DVO_MPG_REC3_YPOS               	dvo_mpg_rec3_ypos               ;	// 0x02d8 : ''
	M14B0_FRC_DVO_MPG_REC4_XPOS               	dvo_mpg_rec4_xpos               ;	// 0x02dc : ''
	M14B0_FRC_DVO_MPG_REC4_YPOS               	dvo_mpg_rec4_ypos               ;	// 0x02e0 : ''
	M14B0_FRC_DVO_MPG_REC5_XPOS               	dvo_mpg_rec5_xpos               ;	// 0x02e4 : ''
	M14B0_FRC_DVO_MPG_REC5_YPOS               	dvo_mpg_rec5_ypos               ;	// 0x02e8 : ''
	M14B0_FRC_DVO_MPG_REC6_XPOS               	dvo_mpg_rec6_xpos               ;	// 0x02ec : ''
	M14B0_FRC_DVO_MPG_REC6_YPOS               	dvo_mpg_rec6_ypos               ;	// 0x02f0 : ''
	M14B0_FRC_DVO_MPG_REC7_XPOS               	dvo_mpg_rec7_xpos               ;	// 0x02f4 : ''
	M14B0_FRC_DVO_MPG_REC7_YPOS               	dvo_mpg_rec7_ypos               ;	// 0x02f8 : ''
	M14B0_FRC_DVO_SPG_LINE_CTRL0              	dvo_spg_line_ctrl0              ;	// 0x02fc : ''
	M14B0_FRC_DVO_SPG_LINE_CTRL1              	dvo_spg_line_ctrl1              ;	// 0x0300 : ''
	UINT32                          	                 _rsvd_11[   3];	// 0x0304 ~ 0x030c
	M14B0_FRC_DVO_MEP_MV_CTRL                 	dvo_mep_mv_ctrl                 ;	// 0x0310 : ''
	M14B0_FRC_DVO_MEP_MV_LSIZE                	dvo_mep_mv_lsize                ;	// 0x0314 : ''
	M14B0_FRC_DVO_MEP_MV_MSB_ADDR             	dvo_mep_mv_msb_addr             ;	// 0x0318 : ''
	M14B0_FRC_DVO_MEP_MV_LSB_ADDR             	dvo_mep_mv_lsb_addr             ;	// 0x031c : ''
	M14B0_FRC_DVO_SG_VS_STATUS0               	dvo_sg_vs_status0               ;	// 0x0320 : ''
	M14B0_FRC_DVO_SG_VS_STATUS1               	dvo_sg_vs_status1               ;	// 0x0324 : ''
	M14B0_FRC_DVO_SG_HS_STATUS0               	dvo_sg_hs_status0               ;	// 0x0328 : ''
	M14B0_FRC_DVO_SG_HS_STATUS1               	dvo_sg_hs_status1               ;	// 0x032c : ''
	UINT32                          	dvo_vernum                      ;	// 0x0330 : ''
	UINT32                          	dvo_current_stcc                    ;	// 0x0334 : ''
	UINT32                          	dvo_vs_stcc                     ;	// 0x0338 : ''
	M14B0_FRC_DVO_DPWM_PIN_CTRL         dvo_dpwm_pin_ctrl                   ;	// 0x033c : ''
	M14B0_FRC_DVO_LR3D_SG_PIN_CTRL    	dvo_lr3d_sg_pin_ctrl                ;	// 0x0340 : ''
	UINT32                          	dvo_dco_mfc                         ;	// 0x0344 : ''
	UINT32                          	dvo_dco_dfc                         ;	// 0x0348 : ''
	UINT32                          	dvo_dco_dflc                        ;	// 0x034c : ''
	DCO_FCW                         	dvo_dco_fcw                         ;	// 0x0350 : ''
	UINT32                          	dvo_dco_num                         ;	// 0x0354 : ''
	UINT32                          	                 _rsvd_12[   2];	// 0x0358 ~ 0x035c
	M14B0_FRC_DVO_PWM_INTR_STATUS_EN      	dvo_pwm_intr_status_en              ;	// 0x0360 : ''
	M14B0_FRC_DVO_PWM_INTR_CLEAR              dvo_pwm_intr_clear                  ;	// 0x0364 : ''
	M14B0_FRC_DVO_PWM_INTR_STATUS           	dvo_pwm_intr_status                 ;	// 0x0368 : ''
	M14B0_FRC_DVO_PWM_INTR_MASK               	dvo_pwm_intr_mask                   ;	// 0x036c : ''
	M14B0_FRC_DVO_PWM0_CTRL0                     	dvo_pwm0_ctrl0                      ;	// 0x0370 : ''
	M14B0_FRC_DVO_PWM0_CTRL1                     	dvo_pwm0_ctrl1                      ;	// 0x0374 : ''
	M14B0_FRC_DVO_PWM0_CTRL2                     	dvo_pwm0_ctrl2                      ;	// 0x0378 : ''
	M14B0_FRC_DVO_PWM1_CTRL0                    	dvo_pwm1_ctrl0                      ;	// 0x037c : ''
	M14B0_FRC_DVO_PWM1_CTRL1                    	dvo_pwm1_ctrl1                      ;	// 0x0380 : ''
	M14B0_FRC_DVO_PWM1_CTRL2                    	dvo_pwm1_ctrl2                      ;	// 0x0384 : ''
	M14B0_FRC_DVO_PWM2_CTRL0                    	dvo_pwm2_ctrl0                      ;	// 0x0388 : ''
	M14B0_FRC_DVO_PWM2_CTRL1                    	dvo_pwm2_ctrl1                      ;	// 0x038c : ''
	M14B0_FRC_DVO_PWM2_CTRL2                     	dvo_pwm2_ctrl2                      ;	// 0x0390 : ''
	M14B0_FRC_DVO_PWM3_CTRL0                     	dvo_pwm3_ctrl0                      ;	// 0x0394 : ''
	M14B0_FRC_DVO_PWM3_CTRL1                    	dvo_pwm3_ctrl1                      ;	// 0x0398 : ''
	M14B0_FRC_DVO_PWM3_0_LOW                   	dvo_pwm3_0_low                      ;	// 0x039c : ''
	M14B0_FRC_DVO_PWM3_0_HIGH                   	dvo_pwm3_0_high                     ;	// 0x03a0 : ''
	M14B0_FRC_DVO_PWM3_1_LOW                    	dvo_pwm3_1_low                      ;	// 0x03a4 : ''
	M14B0_FRC_DVO_PWM3_1_HIGH                   	dvo_pwm3_1_high                     ;	// 0x03a8 : ''
	M14B0_FRC_DVO_PWM3_2_LOW                   	dvo_pwm3_2_low                      ;	// 0x03ac : ''
	M14B0_FRC_DVO_PWM3_2_HIGH                   	dvo_pwm3_2_high                     ;	// 0x03b0 : ''
	M14B0_FRC_DVO_PWM3_3_LOW                    	dvo_pwm3_3_low                      ;	// 0x03b4 : ''
	M14B0_FRC_DVO_PWM3_3_HIGH                  	dvo_pwm3_3_high                     ;	// 0x03b8 : ''
	M14B0_FRC_DVO_PWM3_4_LOW                    	dvo_pwm3_4_low                      ;	// 0x03bc : ''
	M14B0_FRC_DVO_PWM3_4_HIGH                  	dvo_pwm3_4_high                     ;	// 0x03c0 : ''
	M14B0_FRC_DVO_PWM3_5_LOW                    	dvo_pwm3_5_low                      ;	// 0x03c4 : ''
	M14B0_FRC_DVO_PWM3_5_HIGH                   	dvo_pwm3_5_high                     ;	// 0x03c8 : ''
	M14B0_FRC_DVO_PWM3_6_LOW                    	dvo_pwm3_6_low                      ;	// 0x03cc : ''
	M14B0_FRC_DVO_PWM3_6_HIGH                   	dvo_pwm3_6_high                     ;	// 0x03d0 : ''
	M14B0_FRC_DVO_PWM3_7_LOW                    	dvo_pwm3_7_low                      ;	// 0x03d4 : ''
	M14B0_FRC_DVO_PWM3_7_HIGH                   	dvo_pwm3_7_high                     ;	// 0x03d8 : ''
	M14B0_FRC_DVO_PWM_V_LOAD_WRITE       	dvo_pwm_v_load_write                ;	// 0x03dc : ''
	M14B0_FRC_DVO_PWM0_V_R                       	dvo_pwm0_v_r                        ;	// 0x03e0 : ''
	M14B0_FRC_DVO_PWM0_V_F                        	dvo_pwm0_v_f                        ;	// 0x03e4 : ''
	M14B0_FRC_DVO_PWM1_V_R                      	dvo_pwm1_v_r                        ;	// 0x03e8 : ''
	M14B0_FRC_DVO_PWM1_V_F                        	dvo_pwm1_v_f                        ;	// 0x03ec : ''
	M14B0_FRC_DVO_PWM2_V_R                        	dvo_pwm2_v_r                        ;	// 0x03f0 : ''
	M14B0_FRC_DVO_PWM2_V_F                        	dvo_pwm2_v_f                        ;	// 0x03f4 : ''
	M14B0_FRC_DVO_PWM0_V_SUB_R                	dvo_pwm0_v_sub_r                    ;	// 0x03f8 : ''
	M14B0_FRC_DVO_PWM1_V_SUB_R               	dvo_pwm1_v_sub_r                    ;	// 0x03fc : ''
	M14B0_FRC_DVO_PWM2_V_SUB_R              	dvo_pwm2_v_sub_r                    ;	// 0x0400 : ''
	M14B0_FRC_DVO_PWM0_LOCK_PULSE_XY    	dvo_pwm0_lock_pulse_xy              ;	// 0x0404 : ''
	M14B0_FRC_DVO_PWM1_LOCK_PULSE_XY     	dvo_pwm1_lock_pulse_xy              ;	// 0x0408 : ''
	M14B0_FRC_DVO_PWM2_LOCK_PULSE_XY     	dvo_pwm2_lock_pulse_xy              ;	// 0x040c : ''
	M14B0_FRC_DVO_PWM3_LOCK_PULSE_XY    	dvo_pwm3_lock_pulse_xy              ;	// 0x0410 : ''
	UINT32                          		dvo_pwm0_width                      ;	// 0x0414 : ''
	UINT32                          		dvo_pwm0_offset                     ;	// 0x0418 : ''
	UINT32                          		dvo_pwm0_offset_sum                 ;	// 0x041c : ''
	UINT32                          		dvo_pwm1_width                      ;	// 0x0420 : ''
	UINT32                          		dvo_pwm1_offset                     ;	// 0x0424 : ''
	UINT32                          		dvo_pwm1_offset_sum                 ;	// 0x0428 : ''
	UINT32                          		dvo_pwm2_width                      ;	// 0x042c : ''
	UINT32                          		dvo_pwm2_offset                     ;	// 0x0430 : ''
	UINT32                          		dvo_pwm2_offset_sum                 ;	// 0x0434 : ''
	M14B0_FRC_DVO_PWM_MASTER                    	dvo_pwm_master                      ;	// 0x0438 : ''
	M14B0_FRC_DVO_PWM0_V_SUB_F                 	dvo_pwm0_v_sub_f                    ;	// 0x043c : ''
	M14B0_FRC_DVO_PWM1_V_SUB_F                	dvo_pwm1_v_sub_f                    ;	// 0x0440 : ''
	M14B0_FRC_DVO_PWM2_V_SUB_F                	dvo_pwm2_v_sub_f                    ;	// 0x0444 : ''
} FRC_DVO_REG_M14B0_T;

#endif //#ifndef _REG_FRC_M14AB0_h
