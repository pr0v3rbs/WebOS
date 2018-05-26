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

#ifndef _REG_CTR_M14B0_H_
#define _REG_CTR_M14B0_H_
/*-----------------------------------------------------------------------------
		0xc0020000L pic_init ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 g0_pic_init                        : 1 ;  //     0     
	UINT32 g1_pic_init                        : 1 ;  //     1     
	UINT32 g2_pic_init                        : 1 ;  //     2     
	UINT32 g3_pic_init                        : 1 ;  //     3     
	UINT32 g4_pic_init                        : 1 ;  //     4     
	UINT32                                    : 11;  // 15: 5     reserved
	UINT32 g0_auto_init                       : 1 ;  //    16     
	UINT32 g0_auto_2nd                        : 1 ;  //    17     
	UINT32 g4_auto_init                       : 1 ;  //    18     
	UINT32 g4_auto_2nd                        : 1 ;  //    19     
	UINT32 g1_auto_init                       : 1 ;  //    20     
	UINT32 g1_auto_2nd                        : 1 ;  //    21     
	UINT32                                    : 2 ;  // 23:22     reserved
	UINT32 g2_auto_init                       : 1 ;  //    24     
	UINT32 g2_auto_2nd                        : 1 ;  //    25     
	UINT32                                    : 2 ;  // 27:26     reserved
	UINT32 g3_auto_init                       : 1 ;  //    28     
	UINT32 g3_auto_2nd                        : 1 ;  //    29     
} M14B0_DEI_PIC_INIT_T;

/*-----------------------------------------------------------------------------
		0xc0020004L pic_start ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 g0_pic_start                       : 1 ;  //     0     
	UINT32 g1_pic_start                       : 1 ;  //     1     
	UINT32 g2_pic_start                       : 1 ;  //     2     
	UINT32 g3_pic_start                       : 1 ;  //     3     
	UINT32 g4_pic_start                       : 1 ;  //     4     
} M14B0_DEI_PIC_START_T;

/*-----------------------------------------------------------------------------
		0xc0020008L ctrl_src_pic_init ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 idx_src_g0_init                    : 2 ;  //  1: 0     
	UINT32 idx_src_g1_init                    : 2 ;  //  3: 2     
	UINT32 idx_src_g2_init                    : 2 ;  //  5: 4     
	UINT32 idx_src_g3_init                    : 2 ;  //  7: 6     
	UINT32 idx_src_g0_start                   : 2 ;  //  9: 8     
	UINT32 idx_src_g1_start                   : 2 ;  // 11:10     
	UINT32 idx_src_g2_start                   : 2 ;  // 13:12     
	UINT32 idx_src_g3_start                   : 2 ;  // 15:14     
	UINT32 idx_src_g0_src_pulse               : 2 ;  // 17:16     
	UINT32 idx_src_g1_src_pulse               : 2 ;  // 19:18     
	UINT32 idx_src_g2_src_pulse               : 2 ;  // 21:20     
	UINT32 idx_src_g3_src_pulse               : 2 ;  // 23:22     
	UINT32 idx_src_dei_dem_msrc               : 2 ;  // 25:24     
	UINT32 idx_src_dei_dem_ssrc               : 2 ;  // 27:26     
	UINT32 idx_src_dei_deo_msrc               : 2 ;  // 29:28     
	UINT32 idx_src_dei_deo_ssrc               : 2 ;  // 31:30     
} M14B0_DEI_CTRL_SRC_PIC_INIT_T;

/*-----------------------------------------------------------------------------
		0xc002000cL ctrl_gap_irq_to_pic_init ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 gap_src_pulse_to_pic_init          : 28;  // 27: 0     
	UINT32 end_pic_init                       : 2 ;  // 29:28     
	UINT32 str_src_pulse                      : 2 ;  // 31:30     
} M14B0_DEI_CTRL_GAP_IRQ_TO_PIC_INIT_T;

/*-----------------------------------------------------------------------------
		0xc0020010L intr_reg ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 intr_src_pulse0                    : 1 ;  //     0     
	UINT32 intr_src_pulse1                    : 1 ;  //     1     
	UINT32 intr_src_pulse2                    : 1 ;  //     2     
	UINT32 intr_src_pulse3                    : 1 ;  //     3     
	UINT32 intr_pic_end_g0                    : 1 ;  //     4     
	UINT32 intr_pic_end_g1                    : 1 ;  //     5     
	UINT32 intr_pic_end_g2                    : 1 ;  //     6     
	UINT32 intr_pic_end_g3                    : 1 ;  //     7     
	UINT32 intr_cha_field                     : 1 ;  //     8     
	UINT32 intr_cha_frame_lr                  : 1 ;  //     9     
	UINT32 intr_chb_field                     : 1 ;  //    10     
	UINT32 intr_chb_frame_lr                  : 1 ;  //    11     
	UINT32 intr_src_pulse4                    : 1 ;  //    12     
	UINT32 intr_pic_end_g4                    : 1 ;  //    13     
	UINT32 intr_gmau_flush_rd_done            : 1 ;  //    14     
	UINT32 intr_gmau_flush_wr_done            : 1 ;  //    15     
	UINT32 intr_cha_input                     : 1 ;  //    16     
	UINT32 intr_chb_input                     : 1 ;  //    17     
	UINT32 intr_smux_m_ow                     : 1 ;  //    18     
	UINT32 intr_smux_s_ow                     : 1 ;  //    19     
	UINT32 intr_fd3                           : 1 ;  //    20     
	UINT32 intr_vdo                           : 1 ;  //    21     
	UINT32 intr_cvd                           : 1 ;  //    22     
	UINT32 intr_wss                           : 1 ;  //    23     
	UINT32 intr_cgms                          : 1 ;  //    24     
	UINT32 intr_src_fall0                     : 1 ;  //    25     
	UINT32 intr_src_fall1                     : 1 ;  //    26     
	UINT32 intr_src_fall2                     : 1 ;  //    27     
	UINT32 intr_src_fall3                     : 1 ;  //    28     
} M14B0_DEI_INTR_REG_T;

/*-----------------------------------------------------------------------------
		0xc0020014L intr_mask ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mask_src_pulse0                    : 1 ;  //     0     
	UINT32 mask_src_pulse1                    : 1 ;  //     1     
	UINT32 mask_src_pulse2                    : 1 ;  //     2     
	UINT32 mask_src_pulse3                    : 1 ;  //     3     
	UINT32 mask_pic_end_g0                    : 1 ;  //     4     
	UINT32 mask_pic_end_g1                    : 1 ;  //     5     
	UINT32 mask_pic_end_g2                    : 1 ;  //     6     
	UINT32 mask_pic_end_g3                    : 1 ;  //     7     
	UINT32 mask_cha_field                     : 1 ;  //     8     
	UINT32 mask_cha_frame_lr                  : 1 ;  //     9     
	UINT32 mask_chb_field                     : 1 ;  //    10     
	UINT32 mask_chb_frame_lr                  : 1 ;  //    11     
	UINT32                                    : 2 ;  // 13:12     reserved
	UINT32 mask_gmau_flush_rd_done            : 1 ;  //    14     
	UINT32 mask_gmau_flush_wr_done            : 1 ;  //    15     
	UINT32 mask_cha_input                     : 1 ;  //    16     
	UINT32 mask_chb_input                     : 1 ;  //    17     
	UINT32 mask_smux_m_ow                     : 1 ;  //    18     
	UINT32 mask_smux_s_ow                     : 1 ;  //    19     
	UINT32 mask_fd3                           : 1 ;  //    20     
	UINT32 mask_vdo                           : 1 ;  //    21     
	UINT32 mask_cvd                           : 1 ;  //    22     
	UINT32 mask_wss                           : 1 ;  //    23     
	UINT32 mask_cgms                          : 1 ;  //    24     
	UINT32 mask_src_fall0                     : 1 ;  //    25     
	UINT32 mask_src_fall1                     : 1 ;  //    26     
	UINT32 mask_src_fall2                     : 1 ;  //    27     
	UINT32 mask_src_fall3                     : 1 ;  //    28     
} M14B0_DEI_INTR_MASK_T;

/*-----------------------------------------------------------------------------
		0xc0020018L intr_mux ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mux_src_pulse0                     : 1 ;  //     0     
	UINT32 mux_src_pulse1                     : 1 ;  //     1     
	UINT32 mux_src_pulse2                     : 1 ;  //     2     
	UINT32 mux_src_pulse3                     : 1 ;  //     3     
	UINT32 mux_pic_end_g0                     : 1 ;  //     4     
	UINT32 mux_pic_end_g1                     : 1 ;  //     5     
	UINT32 mux_pic_end_g2                     : 1 ;  //     6     
	UINT32 mux_pic_end_g3                     : 1 ;  //     7     
	UINT32 mux_cha_field                      : 1 ;  //     8     
	UINT32 mux_cha_frame_lr                   : 1 ;  //     9     
	UINT32 mux_chb_field                      : 1 ;  //    10     
	UINT32 mux_chb_frame_lr                   : 1 ;  //    11     
	UINT32                                    : 2 ;  // 13:12     reserved
	UINT32 mux_gmau_flush_rd_done             : 1 ;  //    14     
	UINT32 mux_gmau_flush_wr_done             : 1 ;  //    15     
	UINT32 mux_cha_input                      : 1 ;  //    16     
	UINT32 mux_chb_input                      : 1 ;  //    17     
	UINT32 mux_smux_m_ow                      : 1 ;  //    18     
	UINT32 mux_smux_s_ow                      : 1 ;  //    19     
	UINT32 mux_fd3                            : 1 ;  //    20     
	UINT32 mux_vdo                            : 1 ;  //    21     
	UINT32 mux_cvd                            : 1 ;  //    22     
	UINT32 mux_wss                            : 1 ;  //    23     
	UINT32 mux_cgms                           : 1 ;  //    24     
	UINT32 mux_src_fall0                      : 1 ;  //    25     
	UINT32 mux_src_fall1                      : 1 ;  //    26     
	UINT32 mux_src_fall2                      : 1 ;  //    27     
	UINT32 mux_src_fall3                      : 1 ;  //    28     
} M14B0_DEI_INTR_MUX_T;

/*-----------------------------------------------------------------------------
		0xc002001cL intr_en ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 en_src_pulse0                      : 1 ;  //     0     
	UINT32 en_src_pulse1                      : 1 ;  //     1     
	UINT32 en_src_pulse2                      : 1 ;  //     2     
	UINT32 en_src_pulse3                      : 1 ;  //     3     
	UINT32 en_pic_end_g0                      : 1 ;  //     4     
	UINT32 en_pic_end_g1                      : 1 ;  //     5     
	UINT32 en_pic_end_g2                      : 1 ;  //     6     
	UINT32 en_pic_end_g3                      : 1 ;  //     7     
	UINT32 en_cha_field                       : 1 ;  //     8     
	UINT32 en_cha_frame_lr                    : 1 ;  //     9     
	UINT32 en_chb_field                       : 1 ;  //    10     
	UINT32 en_chb_frame_lr                    : 1 ;  //    11     
	UINT32                                    : 2 ;  // 13:12     reserved
	UINT32 en_gmau_flush_rd_done              : 1 ;  //    14     
	UINT32 en_gmau_flush_wr_done              : 1 ;  //    15     
	UINT32 en_cha_input                       : 1 ;  //    16     
	UINT32 en_chb_input                       : 1 ;  //    17     
	UINT32 en_smux_m_ow                       : 1 ;  //    18     
	UINT32 en_smux_s_ow                       : 1 ;  //    19     
	UINT32 en_fd3                             : 1 ;  //    20     
	UINT32 en_vdo                             : 1 ;  //    21     
	UINT32 en_cvd                             : 1 ;  //    22     
	UINT32 en_wss                             : 1 ;  //    23     
	UINT32 en_cgms                            : 1 ;  //    24     
	UINT32 en_src_fall0                       : 1 ;  //    25     
	UINT32 en_src_fall1                       : 1 ;  //    26     
	UINT32 en_src_fall2                       : 1 ;  //    27     
	UINT32 en_src_fall3                       : 1 ;  //    28     
} M14B0_DEI_INTR_EN_T;

/*-----------------------------------------------------------------------------
		0xc0020020L msrc0_sync_mux_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 trans_mask_en                      : 1 ;  //     0     
	UINT32 disp_sync_sel                      : 1 ;  //     1     
	UINT32 vdec_vsync_inv                     : 1 ;  //     2     
	UINT32 mem_vsync_inv                      : 1 ;  //     3     
	UINT32 cvi_select                         : 1 ;  //     4     
	UINT32 src_fall_pos_sel                   : 3 ;  //  7: 5     
	UINT32 mvi_select                         : 1 ;  //     8     
	UINT32 mvi_sync_select                    : 2 ;  // 10: 9     
	UINT32 src_pulse_base_pos_half_va         : 1 ;  //    11     
	UINT32 mvi_cvi_select                     : 1 ;  //    12     
	UINT32 cvia_vsync_inv                     : 1 ;  //    13     
	UINT32 cvib_vsync_inv                     : 1 ;  //    14     
	UINT32 disp_sync_inv                      : 1 ;  //    15     
	UINT32 src_pulse_pos_sel                  : 8 ;  // 23:16     
	UINT32 disp_select                        : 1 ;  //    24     
	UINT32 cvia_sync_sel                      : 1 ;  //    25     
	UINT32 cvib_sync_sel                      : 1 ;  //    26     
	UINT32 mvi_sync_src_sel                   : 1 ;  //    27     
	UINT32 sync_manual_delay_en               : 1 ;  //    28     
	UINT32 cvi_pulse_dly_line                 : 1 ;  //    29     
	UINT32 cvi_pulse_dly_line_auto            : 1 ;  //    30     
	UINT32 ignore_src_vsync_fal               : 1 ;  //    31     
} M14B0_DEI_MSRC0_SYNC_MUX_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc0020024L msrc0_sync_dly ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 msrc_sync_dly                      : 28;  // 27: 0     
	UINT32 cnt_msrc_pulse                     : 4 ;  // 31:28     
} M14B0_DEI_MSRC0_SYNC_DLY_T;

/*-----------------------------------------------------------------------------
		0xc0020028L msrc0_sync_dly_line ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 msrc_sync_dly_line                 : 16;  // 15: 0     
} M14B0_DEI_MSRC0_SYNC_DLY_LINE_T;

/*-----------------------------------------------------------------------------
		0xc002002cL msrc0_ro_vblk_cnt ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 msrc_ro_vblk_cnt                   : 32;  // 31: 0     
} M14B0_DEI_MSRC0_RO_VBLK_CNT_T;

/*-----------------------------------------------------------------------------
		0xc0020030L msrc1_sync_mux_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 trans_mask_en                      : 1 ;  //     0     
	UINT32 disp_sync_sel                      : 1 ;  //     1     
	UINT32 vdec_vsync_inv                     : 1 ;  //     2     
	UINT32 mem_vsync_inv                      : 1 ;  //     3     
	UINT32 cvi_select                         : 1 ;  //     4     
	UINT32 src_fall_pos_sel                   : 3 ;  //  7: 5     
	UINT32 mvi_select                         : 1 ;  //     8     
	UINT32 mvi_sync_select                    : 2 ;  // 10: 9     
	UINT32 src_pulse_base_pos_half_va         : 1 ;  //    11     
	UINT32 mvi_cvi_select                     : 1 ;  //    12     
	UINT32 cvia_vsync_inv                     : 1 ;  //    13     
	UINT32 cvib_vsync_inv                     : 1 ;  //    14     
	UINT32 disp_sync_inv                      : 1 ;  //    15     
	UINT32 src_pulse_pos_sel                  : 8 ;  // 23:16     
	UINT32 disp_select                        : 1 ;  //    24     
	UINT32 cvia_sync_sel                      : 1 ;  //    25     
	UINT32 cvib_sync_sel                      : 1 ;  //    26     
	UINT32 mvi_sync_src_sel                   : 1 ;  //    27     
	UINT32 sync_manual_delay_en               : 1 ;  //    28     
	UINT32 cvi_pulse_dly_line                 : 1 ;  //    29     
	UINT32 cvi_pulse_dly_line_auto            : 1 ;  //    30     
	UINT32 ignore_src_vsync_fal               : 1 ;  //    31     
} M14B0_DEI_MSRC1_SYNC_MUX_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc0020034L msrc1_sync_dly ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 msrc_sync_dly                      : 28;  // 27: 0     
	UINT32 cnt_msrc_pulse                     : 4 ;  // 31:28     
} M14B0_DEI_MSRC1_SYNC_DLY_T;

/*-----------------------------------------------------------------------------
		0xc0020038L msrc1_sync_dly_line ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 msrc_sync_dly_line                 : 16;  // 15: 0     
} M14B0_DEI_MSRC1_SYNC_DLY_LINE_T;

/*-----------------------------------------------------------------------------
		0xc002003cL msrc1_ro_vblk_cnt ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 msrc_ro_vblk_cnt                   : 32;  // 31: 0     
} M14B0_DEI_MSRC1_RO_VBLK_CNT_T;

/*-----------------------------------------------------------------------------
		0xc0020040L msrc2_sync_mux_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 trans_mask_en                      : 1 ;  //     0     
	UINT32 disp_sync_sel                      : 1 ;  //     1     
	UINT32 vdec_vsync_inv                     : 1 ;  //     2     
	UINT32 mem_vsync_inv                      : 1 ;  //     3     
	UINT32 cvi_select                         : 1 ;  //     4     
	UINT32 src_fall_pos_sel                   : 3 ;  //  7: 5     
	UINT32 mvi_select                         : 1 ;  //     8     
	UINT32 mvi_sync_select                    : 2 ;  // 10: 9     
	UINT32 src_pulse_base_pos_half_va         : 1 ;  //    11     
	UINT32 mvi_cvi_select                     : 1 ;  //    12     
	UINT32 cvia_vsync_inv                     : 1 ;  //    13     
	UINT32 cvib_vsync_inv                     : 1 ;  //    14     
	UINT32 disp_sync_inv                      : 1 ;  //    15     
	UINT32 src_pulse_pos_sel                  : 8 ;  // 23:16     
	UINT32 disp_select                        : 1 ;  //    24     
	UINT32 cvia_sync_sel                      : 1 ;  //    25     
	UINT32 cvib_sync_sel                      : 1 ;  //    26     
	UINT32 mvi_sync_src_sel                   : 1 ;  //    27     
	UINT32 sync_manual_delay_en               : 1 ;  //    28     
	UINT32 cvi_pulse_dly_line                 : 1 ;  //    29     
	UINT32 cvi_pulse_dly_line_auto            : 1 ;  //    30     
	UINT32 ignore_src_vsync_fal               : 1 ;  //    31     
} M14B0_DEI_MSRC2_SYNC_MUX_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc0020044L msrc2_sync_dly ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 msrc_sync_dly                      : 28;  // 27: 0     
	UINT32 cnt_msrc_pulse                     : 4 ;  // 31:28     
} M14B0_DEI_MSRC2_SYNC_DLY_T;

/*-----------------------------------------------------------------------------
		0xc0020048L msrc2_sync_dly_line ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 msrc_sync_dly_line                 : 16;  // 15: 0     
} M14B0_DEI_MSRC2_SYNC_DLY_LINE_T;

/*-----------------------------------------------------------------------------
		0xc002004cL msrc2_ro_vblk_cnt ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 msrc_ro_vblk_cnt                   : 32;  // 31: 0     
} M14B0_DEI_MSRC2_RO_VBLK_CNT_T;

/*-----------------------------------------------------------------------------
		0xc0020050L msrc3_sync_mux_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 trans_mask_en                      : 1 ;  //     0     
	UINT32 disp_sync_sel                      : 1 ;  //     1     
	UINT32 vdec_vsync_inv                     : 1 ;  //     2     
	UINT32 mem_vsync_inv                      : 1 ;  //     3     
	UINT32 cvi_select                         : 1 ;  //     4     
	UINT32 src_fall_pos_sel                   : 3 ;  //  7: 5     
	UINT32 mvi_select                         : 1 ;  //     8     
	UINT32 mvi_sync_select                    : 2 ;  // 10: 9     
	UINT32 src_pulse_base_pos_half_va         : 1 ;  //    11     
	UINT32 mvi_cvi_select                     : 1 ;  //    12     
	UINT32 cvia_vsync_inv                     : 1 ;  //    13     
	UINT32 cvib_vsync_inv                     : 1 ;  //    14     
	UINT32 disp_sync_inv                      : 1 ;  //    15     
	UINT32 src_pulse_pos_sel                  : 8 ;  // 23:16     
	UINT32 disp_select                        : 1 ;  //    24     
	UINT32 cvia_sync_sel                      : 1 ;  //    25     
	UINT32 cvib_sync_sel                      : 1 ;  //    26     
	UINT32 mvi_sync_src_sel                   : 1 ;  //    27     
	UINT32 sync_manual_delay_en               : 1 ;  //    28     
	UINT32 cvi_pulse_dly_line                 : 1 ;  //    29     
	UINT32 cvi_pulse_dly_line_auto            : 1 ;  //    30     
	UINT32 ignore_src_vsync_fal               : 1 ;  //    31     
} M14B0_DEI_MSRC3_SYNC_MUX_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc0020054L msrc3_sync_dly ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 msrc_sync_dly                      : 28;  // 27: 0     
	UINT32 cnt_msrc_pulse                     : 4 ;  // 31:28     
} M14B0_DEI_MSRC3_SYNC_DLY_T;

/*-----------------------------------------------------------------------------
		0xc0020058L msrc3_sync_dly_line ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 msrc_sync_dly_line                 : 16;  // 15: 0     
} M14B0_DEI_MSRC3_SYNC_DLY_LINE_T;

/*-----------------------------------------------------------------------------
		0xc002005cL msrc3_ro_vblk_cnt ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 msrc_ro_vblk_cnt                   : 32;  // 31: 0     
} M14B0_DEI_MSRC3_RO_VBLK_CNT_T;

/*-----------------------------------------------------------------------------
		0xc0020060L msrc4_sync_mux_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 trans_mask_en                      : 1 ;  //     0     
	UINT32 disp_sync_sel                      : 1 ;  //     1     
	UINT32 vdec_vsync_inv                     : 1 ;  //     2     
	UINT32 mem_vsync_inv                      : 1 ;  //     3     
	UINT32 cvi_select                         : 1 ;  //     4     
	UINT32 src_fall_pos_sel                   : 3 ;  //  7: 5     
	UINT32 mvi_select                         : 1 ;  //     8     
	UINT32 mvi_sync_select                    : 2 ;  // 10: 9     
	UINT32 src_pulse_base_pos_half_va         : 1 ;  //    11     
	UINT32 mvi_cvi_select                     : 1 ;  //    12     
	UINT32 cvia_vsync_inv                     : 1 ;  //    13     
	UINT32 cvib_vsync_inv                     : 1 ;  //    14     
	UINT32 disp_sync_inv                      : 1 ;  //    15     
	UINT32 src_pulse_pos_sel                  : 8 ;  // 23:16     
	UINT32 disp_select                        : 1 ;  //    24     
	UINT32 cvia_sync_sel                      : 1 ;  //    25     
	UINT32 cvib_sync_sel                      : 1 ;  //    26     
	UINT32 mvi_sync_src_sel                   : 1 ;  //    27     
	UINT32 sync_manual_delay_en               : 1 ;  //    28     
	UINT32 cvi_pulse_dly_line                 : 1 ;  //    29     
	UINT32 cvi_pulse_dly_line_auto            : 1 ;  //    30     
	UINT32 ignore_src_vsync_fal               : 1 ;  //    31     
} M14B0_DEI_MSRC4_SYNC_MUX_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc0020064L msrc4_sync_dly ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 msrc_sync_dly                      : 28;  // 27: 0     
	UINT32 cnt_msrc_pulse                     : 4 ;  // 31:28     
} M14B0_DEI_MSRC4_SYNC_DLY_T;

/*-----------------------------------------------------------------------------
		0xc0020068L msrc4_sync_dly_line ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 msrc_sync_dly_line                 : 16;  // 15: 0     
} M14B0_DEI_MSRC4_SYNC_DLY_LINE_T;

/*-----------------------------------------------------------------------------
		0xc002006cL msrc4_ro_vblk_cnt ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 msrc_ro_vblk_cnt                   : 32;  // 31: 0     
} M14B0_DEI_MSRC4_RO_VBLK_CNT_T;

/*-----------------------------------------------------------------------------
		0xc0020070L info_cha ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cha_hsize                          : 12;  // 11: 0     
	UINT32                                    : 1 ;  //    12     reserved
	UINT32 cha_csample                        : 1 ;  //    13     
	UINT32 cha_aspect                         : 1 ;  //    14     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 cha_vsize                          : 12;  // 27:16     
	UINT32                                    : 1 ;  //    28     reserved
	UINT32 cha_frame_lr                       : 1 ;  //    29     
	UINT32 cha_field_id                       : 1 ;  //    30     
	UINT32 cha_pr_sq                          : 1 ;  //    31     
} M14B0_DEI_INFO_CHA_T;

/*-----------------------------------------------------------------------------
		0xc0020074L info_chb ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 chb_hsize                          : 12;  // 11: 0     
	UINT32                                    : 1 ;  //    12     reserved
	UINT32 chb_csample                        : 1 ;  //    13     
	UINT32 chb_aspect                         : 1 ;  //    14     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 chb_vsize                          : 12;  // 27:16     
	UINT32                                    : 1 ;  //    28     reserved
	UINT32 chb_frame_lr                       : 1 ;  //    29     
	UINT32 chb_field_id                       : 1 ;  //    30     
	UINT32 chb_pr_sq                          : 1 ;  //    31     
} M14B0_DEI_INFO_CHB_T;

/*-----------------------------------------------------------------------------
		0xc0020080L ctrl_block_input ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 4 ;  //  3: 0     reserved
	UINT32 test_mode_vdo                      : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 sel_type_3ch                       : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 sel_type_hdmi                      : 1 ;  //    12     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 sel_src_fd3                        : 1 ;  //    16     
	UINT32                                    : 7 ;  // 23:17     reserved
	UINT32 sel_src_cvd                        : 1 ;  //    24     
} M14B0_DEI_CTRL_BLOCK_INPUT_T;

/*-----------------------------------------------------------------------------
		0xc0020084L ctrl_mux_cv ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sel_cvib_input                     : 3 ;  //  2: 0     
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 sel_cvia_input                     : 3 ;  //  6: 4     
} M14B0_DEI_CTRL_MUX_CV_T;

/*-----------------------------------------------------------------------------
		0xc0020088L ctrl_mux_mvi_t ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sel_mvi_tm                         : 1 ;  //     0     
	UINT32                                    : 1 ;  //     1     reserved
	UINT32 ignore_mvi_tm                      : 1 ;  //     2     
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 sel_mvi_ts                         : 1 ;  //     4     
	UINT32                                    : 1 ;  //     5     reserved
	UINT32 ignore_mvi_ts                      : 1 ;  //     6     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 sel_mvi_tw                         : 1 ;  //     8     
	UINT32                                    : 1 ;  //     9     reserved
	UINT32 ignore_mvi_tw                      : 1 ;  //    10     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 en_auto_init_ta                    : 1 ;  //    16     
	UINT32 sel_src_auto_init_ta               : 1 ;  //    17     
	UINT32                                    : 2 ;  // 19:18     reserved
	UINT32 en_auto_init_tb                    : 1 ;  //    20     
	UINT32 sel_src_auto_init_tb               : 1 ;  //    21     
	UINT32                                    : 2 ;  // 23:22     reserved
	UINT32 sw_reset_ta                        : 1 ;  //    24     
	UINT32 sw_reset_tb                        : 1 ;  //    25     
	UINT32                                    : 2 ;  // 27:26     reserved
	UINT32 enable_detour                      : 1 ;  //    28     
} M14B0_DEI_CTRL_MUX_MVI_T_T;

/*-----------------------------------------------------------------------------
		0xc002008cL ctrl_mux_mvi_r ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sel_mvi_rm                         : 1 ;  //     0     
	UINT32                                    : 1 ;  //     1     reserved
	UINT32 ignore_mvi_rm                      : 1 ;  //     2     
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 sel_mvi_rs                         : 1 ;  //     4     
	UINT32                                    : 1 ;  //     5     reserved
	UINT32 ignore_mvi_rs                      : 1 ;  //     6     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 sel_mvi_rw                         : 1 ;  //     8     
	UINT32                                    : 1 ;  //     9     reserved
	UINT32 ignore_mvi_rw                      : 1 ;  //    10     
} M14B0_DEI_CTRL_MUX_MVI_R_T;

/*-----------------------------------------------------------------------------
		0xc0020090L mvi_ta_v_clip_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 v_offset                           : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 v_active                           : 12;  // 27:16     
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 v_clip_en                          : 1 ;  //    31     
} M14B0_DEI_MVI_TA_V_CLIP_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc0020094L mvi_ta_bridge_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sw_reset                           : 1 ;  //     0     
	UINT32 nonmask_src_i_ha                   : 1 ;  //     1     
	UINT32 nonmask_src_i_va                   : 1 ;  //     2     
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 auto_init_en                       : 1 ;  //     4     
	UINT32 auto_init_src_sel                  : 1 ;  //     5     
	UINT32                                    : 10;  // 15: 6     reserved
	UINT32 debug_pixel_value                  : 8 ;  // 23:16     
} M14B0_DEI_MVI_TA_BRIDGE_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc0020098L mvi_tb_v_clip_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 v_offset                           : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 v_active                           : 12;  // 27:16     
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 v_clip_en                          : 1 ;  //    31     
} M14B0_DEI_MVI_TB_V_CLIP_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc002009cL mvi_tb_bridge_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sw_reset                           : 1 ;  //     0     
	UINT32 nonmask_src_i_ha                   : 1 ;  //     1     
	UINT32 nonmask_src_i_va                   : 1 ;  //     2     
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 auto_init_en                       : 1 ;  //     4     
	UINT32 auto_init_src_sel                  : 1 ;  //     5     
	UINT32                                    : 10;  // 15: 6     reserved
	UINT32 debug_pixel_value                  : 8 ;  // 23:16     
} M14B0_DEI_MVI_TB_BRIDGE_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc00200a0L ctrl_vdo_vs_rdy ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sel_src_mvi_tm_vs_rdy              : 3 ;  //  2: 0     
	UINT32 sel_src_mvi_ts_vs_rdy              : 3 ;  //  5: 3     
	UINT32 sel_src_mvi_tw_vs_rdy              : 3 ;  //  8: 6     
	UINT32                                    : 1 ;  //     9     reserved
	UINT32 mvi_tm_vs_rdy_force_en             : 1 ;  //    10     
	UINT32 mvi_tm_vs_rdy_value                : 1 ;  //    11     
	UINT32 mvi_ts_vs_rdy_force_en             : 1 ;  //    12     
	UINT32 mvi_ts_vs_rdy_value                : 1 ;  //    13     
	UINT32 mvi_tw_vs_rdy_force_en             : 1 ;  //    14     
	UINT32 mvi_tw_vs_rdy_value                : 1 ;  //    15     
	UINT32 sel_src_vdo_vsync_0                : 2 ;  // 17:16     
	UINT32 sel_src_vdo_vsync_0_ids            : 1 ;  //    18     
	UINT32                                    : 1 ;  //    19     reserved
	UINT32 sel_src_vdo_vsync_1                : 2 ;  // 21:20     
	UINT32 sel_src_vdo_vsync_1_ids            : 1 ;  //    22     
	UINT32                                    : 1 ;  //    23     reserved
	UINT32 sel_src_vdo_vsync_2                : 2 ;  // 25:24     
	UINT32 sel_src_vdo_vsync_2_ids            : 1 ;  //    26     
	UINT32                                    : 1 ;  //    27     reserved
	UINT32 sel_src_vdo_vsync_3                : 2 ;  // 29:28     
	UINT32 sel_src_vdo_vsync_3_ids            : 1 ;  //    30     
} M14B0_DEI_CTRL_VDO_VS_RDY_T;

/*-----------------------------------------------------------------------------
		0xc00200a4L ctrl_vdo_vs_rdy_length ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vdec_vsync_ready_length        : 28;  // 27: 0     
} M14B0_DEI_CTRL_VDO_VS_RDY_LENGTH_T;

/*-----------------------------------------------------------------------------
		0xc00200a8L ctrl_vdo_vs_rdy_timer0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vdo_vs_rdy_timer0              : 28;  // 27: 0     
} M14B0_DEI_CTRL_VDO_VS_RDY_TIMER0_T;

/*-----------------------------------------------------------------------------
		0xc00200acL ctrl_vdo_vs_rdy_timer1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vdo_vs_rdy_timer1              : 28;  // 27: 0     
} M14B0_DEI_CTRL_VDO_VS_RDY_TIMER1_T;

/*-----------------------------------------------------------------------------
		0xc00200b0L mem0_sync_period ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mem_sync_period                    : 32;  // 31: 0     
} M14B0_DEI_MEM0_SYNC_PERIOD_T;

/*-----------------------------------------------------------------------------
		0xc00200b4L mem1_sync_period ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mem_sync_period                    : 32;  // 31: 0     
} M14B0_DEI_MEM1_SYNC_PERIOD_T;

/*-----------------------------------------------------------------------------
		0xc00200b8L mem2_sync_period ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mem_sync_period                    : 32;  // 31: 0     
} M14B0_DEI_MEM2_SYNC_PERIOD_T;

/*-----------------------------------------------------------------------------
		0xc00200bcL mem3_sync_period ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mem_sync_period                    : 32;  // 31: 0     
} M14B0_DEI_MEM3_SYNC_PERIOD_T;

/*-----------------------------------------------------------------------------
		0xc00200c0L mem4_sync_period ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mem_sync_period                    : 32;  // 31: 0     
} M14B0_DEI_MEM4_SYNC_PERIOD_T;

/*-----------------------------------------------------------------------------
		0xc00200c4L pic_init_g4_timer ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pic_init_g3_timer                  : 28;  // 27: 0     
} M14B0_DEI_PIC_INIT_G4_TIMER_T;

/*-----------------------------------------------------------------------------
		0xc00200c8L ctrl_vdo_vs_rdy_timer2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vdo_vs_rdy_timer1              : 28;  // 27: 0     
} M14B0_DEI_CTRL_VDO_VS_RDY_TIMER2_T;

/*-----------------------------------------------------------------------------
		0xc00200ccL ctrl_src_dco_mfc ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 idx_src_dco_mfc                    : 2 ;  //  1: 0     
	UINT32 sel_src_dco_mfc                    : 1 ;  //     2     
} M14B0_DEI_CTRL_SRC_DCO_MFC_T;

/*-----------------------------------------------------------------------------
		0xc00200d0L ctrl_processing_timer ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sel_src_processing_timer_g0        : 1 ;  //     0     
	UINT32 sel_src_processing_timer_g1        : 1 ;  //     1     
	UINT32 sel_src_processing_timer_g2        : 1 ;  //     2     
	UINT32 sel_src_processing_timer_g3        : 1 ;  //     3     
} M14B0_DEI_CTRL_PROCESSING_TIMER_T;

/*-----------------------------------------------------------------------------
		0xc00200d4L info_processing_timer ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 info_processing_timer              : 32;  // 31: 0     
} M14B0_DEI_INFO_PROCESSING_TIMER_T;

/*-----------------------------------------------------------------------------
		0xc00200d8L ctrl_rst_async_bridge ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ctrl_rst_async_bridge              : 16;  // 15: 0     
} M14B0_DEI_CTRL_RST_ASYNC_BRIDGE_T;

/*-----------------------------------------------------------------------------
		0xc00200dcL de_version ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 final_release_date                 : 32;  // 31: 0     
} M14B0_DEI_DE_VERSION_T;

/*-----------------------------------------------------------------------------
		0xc00200e0L pic_init_g0_timer ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pic_init_g0_timer                  : 28;  // 27: 0     
} M14B0_DEI_PIC_INIT_G0_TIMER_T;

/*-----------------------------------------------------------------------------
		0xc00200e4L pic_init_g1_timer ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pic_init_g1_timer                  : 28;  // 27: 0     
} M14B0_DEI_PIC_INIT_G1_TIMER_T;

/*-----------------------------------------------------------------------------
		0xc00200e8L pic_init_g2_timer ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pic_init_g2_timer                  : 28;  // 27: 0     
} M14B0_DEI_PIC_INIT_G2_TIMER_T;

/*-----------------------------------------------------------------------------
		0xc00200ecL pic_init_g3_timer ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pic_init_g3_timer                  : 28;  // 27: 0     
} M14B0_DEI_PIC_INIT_G3_TIMER_T;

/*-----------------------------------------------------------------------------
		0xc00200f0L ctrl_pic_end ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mvi_ra_en                          : 1 ;  //     0     
	UINT32 smux_m_w_en                        : 1 ;  //     1     
	UINT32 fd3_m_en                           : 1 ;  //     2     
	UINT32 smux_m_r_en                        : 1 ;  //     3     
	UINT32 mvi_rb_en                          : 1 ;  //     4     
	UINT32 smux_s_w_en                        : 1 ;  //     5     
	UINT32 fd3_s_en                           : 1 ;  //     6     
	UINT32 smux_s_r_en                        : 1 ;  //     7     
	UINT32                                    : 8 ;  // 15: 8     reserved
	UINT32 pic_end_g0_force_value             : 1 ;  //    16     
	UINT32 pic_end_g0_force_en                : 1 ;  //    17     
	UINT32 pic_end_g1_force_value             : 1 ;  //    18     
	UINT32 pic_end_g1_force_en                : 1 ;  //    19     
	UINT32 pic_end_g2_force_value             : 1 ;  //    20     
	UINT32 pic_end_g2_force_en                : 1 ;  //    21     
	UINT32 pic_end_g3_force_value             : 1 ;  //    22     
	UINT32 pic_end_g3_force_en                : 1 ;  //    23     
} M14B0_DEI_CTRL_PIC_END_T;

/*-----------------------------------------------------------------------------
		0xc00200f4L info_pic_end ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pic_end_g0                         : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 pic_turn_g0                        : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 pic_end_g1                         : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 pic_turn_g1                        : 1 ;  //    12     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 pic_end_g2                         : 1 ;  //    16     
	UINT32                                    : 3 ;  // 19:17     reserved
	UINT32 pic_turn_g2                        : 1 ;  //    20     
	UINT32                                    : 3 ;  // 23:21     reserved
	UINT32 pic_end_g3                         : 1 ;  //    24     
	UINT32                                    : 3 ;  // 27:25     reserved
	UINT32 pic_turn_g3                        : 1 ;  //    28     
} M14B0_DEI_INFO_PIC_END_T;

/*-----------------------------------------------------------------------------
		0xc00200f8L ctrl_fpga ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ctrl_fpga                      : 32;  // 31: 0     
} M14B0_DEI_CTRL_FPGA_T;

/*-----------------------------------------------------------------------------
		0xc00200fcL ctrl_gmau ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ask_cmd_dly_cnt                    : 8 ;  //  7: 0     
	UINT32 dei_aruser                         : 3 ;  // 10: 8     
	UINT32 dei_awuser                         : 3 ;  // 13:11     
	UINT32 dei_wuser                          : 1 ;  //    14     
	UINT32 gmau_flush_cvd_done                : 1 ;  //    15     
	UINT32 ask_to_cnt                         : 10;  // 25:16     
	UINT32 cvd_wuser                          : 1 ;  //    26     
	UINT32 gmau_flush_cvd_en                  : 1 ;  //    27     
	UINT32 gmau_flush_rd_en                   : 1 ;  //    28     
	UINT32 gmau_flush_wr_en                   : 1 ;  //    29     
	UINT32 gmau_flush_rd_done                 : 1 ;  //    30     
	UINT32 gmau_flush_wr_done                 : 1 ;  //    31     
} M14B0_DEI_CTRL_GMAU_T;

/*-----------------------------------------------------------------------------
		0xc0022000L pic_init ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 g0_pic_init                        : 1 ;  //     0     
	UINT32 g1_pic_init                        : 1 ;  //     1     
	UINT32 g2_pic_init                        : 1 ;  //     2     
	UINT32 g3_pic_init                        : 1 ;  //     3     
	UINT32                                    : 12;  // 15: 4     reserved
	UINT32 g0_auto_init                       : 1 ;  //    16     
	UINT32 g0_auto_2nd                        : 1 ;  //    17     
	UINT32                                    : 2 ;  // 19:18     reserved
	UINT32 g1_auto_init                       : 1 ;  //    20     
	UINT32 g1_auto_2nd                        : 1 ;  //    21     
	UINT32                                    : 2 ;  // 23:22     reserved
	UINT32 g2_auto_init                       : 1 ;  //    24     
	UINT32 g2_auto_2nd                        : 1 ;  //    25     
	UINT32                                    : 2 ;  // 27:26     reserved
	UINT32 g3_auto_init                       : 1 ;  //    28     
	UINT32 g3_auto_2nd                        : 1 ;  //    29     
} M14B0_DEM_PIC_INIT_T;

/*-----------------------------------------------------------------------------
		0xc0022004L pic_start ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 g0_pic_start                       : 1 ;  //     0     
	UINT32 g1_pic_start                       : 1 ;  //     1     
	UINT32 g2_pic_start                       : 1 ;  //     2     
	UINT32 g3_pic_start                       : 1 ;  //     3     
} M14B0_DEM_PIC_START_T;

/*-----------------------------------------------------------------------------
		0xc0022008L ctrl_src_pic_init ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 idx_src_g0_init                    : 2 ;  //  1: 0     
	UINT32 idx_src_g1_init                    : 2 ;  //  3: 2     
	UINT32 idx_src_g2_init                    : 2 ;  //  5: 4     
	UINT32 idx_src_g3_init                    : 2 ;  //  7: 6     
	UINT32 idx_src_g0_start                   : 2 ;  //  9: 8     
	UINT32 idx_src_g1_start                   : 2 ;  // 11:10     
	UINT32 idx_src_g2_start                   : 2 ;  // 13:12     
	UINT32 idx_src_g3_start                   : 2 ;  // 15:14     
	UINT32 idx_src_g0_src_pulse               : 2 ;  // 17:16     
	UINT32 idx_src_g1_src_pulse               : 2 ;  // 19:18     
	UINT32 idx_src_g2_src_pulse               : 2 ;  // 21:20     
	UINT32 idx_src_g3_src_pulse               : 2 ;  // 23:22     
} M14B0_DEM_CTRL_SRC_PIC_INIT_T;

/*-----------------------------------------------------------------------------
		0xc002200cL ctrl_gap_irq_to_pic_init ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 gap_src_pulse_to_pic_init          : 28;  // 27: 0     
	UINT32 end_pic_init                       : 2 ;  // 29:28     
	UINT32 str_src_pulse                      : 2 ;  // 31:30     
} M14B0_DEM_CTRL_GAP_IRQ_TO_PIC_INIT_T;

/*-----------------------------------------------------------------------------
		0xc0022010L intr_reg ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 intr_src_pulse0                    : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 intr_pic_end_g0                    : 1 ;  //     4     
	UINT32                                    : 9 ;  // 13: 5     reserved
	UINT32 intr_gmau_flush_rd_done            : 1 ;  //    14     
	UINT32 intr_gmau_flush_wr_done            : 1 ;  //    15     
	UINT32 intr_pe0_m_intr                    : 1 ;  //    16     
} M14B0_DEM_INTR_REG_T;

/*-----------------------------------------------------------------------------
		0xc0022014L intr_mask ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mask_src_pulse0                    : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 mask_pic_end_g0                    : 1 ;  //     4     
	UINT32                                    : 9 ;  // 13: 5     reserved
	UINT32 mask_gmau_flush_rd_done            : 1 ;  //    14     
	UINT32 mask_gmau_flush_wr_done            : 1 ;  //    15     
	UINT32 mask_pe0_m_intr                    : 1 ;  //    16     
} M14B0_DEM_INTR_MASK_T;

/*-----------------------------------------------------------------------------
		0xc0022018L intr_mux ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mux_src_pulse0                     : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 mux_pic_end_g0                     : 1 ;  //     4     
	UINT32                                    : 9 ;  // 13: 5     reserved
	UINT32 mux_gmau_flush_rd_done             : 1 ;  //    14     
	UINT32 mux_gmau_flush_wr_done             : 1 ;  //    15     
	UINT32 mux_pe0_m_intr                     : 1 ;  //    16     
} M14B0_DEM_INTR_MUX_T;

/*-----------------------------------------------------------------------------
		0xc002201cL intr_en ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 en_src_pulse0                      : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 en_pic_end_g0                      : 1 ;  //     4     
	UINT32                                    : 9 ;  // 13: 5     reserved
	UINT32 en_gmau_flush_rd_done              : 1 ;  //    14     
	UINT32 en_gmau_flush_wr_done              : 1 ;  //    15     
	UINT32 en_pe0_m_intr                      : 1 ;  //    16     
} M14B0_DEM_INTR_EN_T;

/*-----------------------------------------------------------------------------
		0xc00220d0L ctrl_processing_timer ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sel_src_processing_timer_g0        : 1 ;  //     0     
	UINT32 sel_src_processing_timer_g1        : 1 ;  //     1     
	UINT32 sel_src_processing_timer_g2        : 1 ;  //     2     
	UINT32 sel_src_processing_timer_g3        : 1 ;  //     3     
} M14B0_DEM_CTRL_PROCESSING_TIMER_T;

/*-----------------------------------------------------------------------------
		0xc00220d4L info_processing_timer ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 info_processing_timer              : 32;  // 31: 0     
} M14B0_DEM_INFO_PROCESSING_TIMER_T;

/*-----------------------------------------------------------------------------
		0xc00220d8L ctrl_rst_async_bridge ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ctrl_rst_async_bridge              : 16;  // 15: 0     
} M14B0_DEM_CTRL_RST_ASYNC_BRIDGE_T;

/*-----------------------------------------------------------------------------
		0xc00220e0L pic_init_g0_timer ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pic_init_g0_timer                  : 28;  // 27: 0     
} M14B0_DEM_PIC_INIT_G0_TIMER_T;

/*-----------------------------------------------------------------------------
		0xc00220e4L pic_init_g1_timer ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pic_init_g1_timer                  : 28;  // 27: 0     
} M14B0_DEM_PIC_INIT_G1_TIMER_T;

/*-----------------------------------------------------------------------------
		0xc00220e8L pic_init_g2_timer ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pic_init_g2_timer                  : 28;  // 27: 0     
} M14B0_DEM_PIC_INIT_G2_TIMER_T;

/*-----------------------------------------------------------------------------
		0xc00220ecL pic_init_g3_timer ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pic_init_g3_timer                  : 28;  // 27: 0     
} M14B0_DEM_PIC_INIT_G3_TIMER_T;

/*-----------------------------------------------------------------------------
		0xc00220f0L ctrl_pic_end ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pe0_en                             : 1 ;  //     0     
	UINT32 ipc0_r_en                          : 1 ;  //     1     
	UINT32 ipc1_r_en                          : 1 ;  //     2     
	UINT32 tnr0_r_en                          : 1 ;  //     3     
	UINT32 tnr_r_en                           : 1 ;  //     4     
	UINT32 tnr1_r_en                          : 1 ;  //     5     
	UINT32 ipc_r_m_en                         : 1 ;  //     6     
	UINT32 tnr_w_en                           : 1 ;  //     7     
	UINT32 tnr_w_m_en                         : 1 ;  //     8     
	UINT32                                    : 7 ;  // 15: 9     reserved
	UINT32 pic_end_g0_force_value             : 1 ;  //    16     
	UINT32 pic_end_g0_force_en                : 1 ;  //    17     
	UINT32 pic_end_g1_force_value             : 1 ;  //    18     
	UINT32 pic_end_g1_force_en                : 1 ;  //    19     
	UINT32 pic_end_g2_force_value             : 1 ;  //    20     
	UINT32 pic_end_g2_force_en                : 1 ;  //    21     
	UINT32 pic_end_g3_force_value             : 1 ;  //    22     
	UINT32 pic_end_g3_force_en                : 1 ;  //    23     
} M14B0_DEM_CTRL_PIC_END_T;

/*-----------------------------------------------------------------------------
		0xc00220f4L info_pic_end ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pic_end_g0                         : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 pic_turn_g0                        : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 pic_end_g1                         : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 pic_turn_g1                        : 1 ;  //    12     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 pic_end_g2                         : 1 ;  //    16     
	UINT32                                    : 3 ;  // 19:17     reserved
	UINT32 pic_turn_g2                        : 1 ;  //    20     
	UINT32                                    : 3 ;  // 23:21     reserved
	UINT32 pic_end_g3                         : 1 ;  //    24     
	UINT32                                    : 3 ;  // 27:25     reserved
	UINT32 pic_turn_g3                        : 1 ;  //    28     
} M14B0_DEM_INFO_PIC_END_T;

/*-----------------------------------------------------------------------------
		0xc00220f8L ctrl_fpga ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ctrl_fpga                      : 32;  // 31: 0     
} M14B0_DEM_CTRL_FPGA_T;

/*-----------------------------------------------------------------------------
		0xc00220fcL ctrl_gmau ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ask_cmd_dly_cnt                    : 8 ;  //  7: 0     
	UINT32                                    : 8 ;  // 15: 8     reserved
	UINT32 ask_to_cnt                         : 10;  // 25:16     
	UINT32                                    : 2 ;  // 27:26     reserved
	UINT32 gmau_flush_rd_en                   : 1 ;  //    28     
	UINT32 gmau_flush_wr_en                   : 1 ;  //    29     
	UINT32 gmau_flush_rd_done                 : 1 ;  //    30     
	UINT32 gmau_flush_wr_done                 : 1 ;  //    31     
} M14B0_DEM_CTRL_GMAU_T;

/*-----------------------------------------------------------------------------
		0xc0023000L pic_init ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 g0_pic_init                        : 1 ;  //     0     
	UINT32 g1_pic_init                        : 1 ;  //     1     
	UINT32 g2_pic_init                        : 1 ;  //     2     
	UINT32 g3_pic_init                        : 1 ;  //     3     
	UINT32                                    : 12;  // 15: 4     reserved
	UINT32 g0_auto_init                       : 1 ;  //    16     
	UINT32 g0_auto_2nd                        : 1 ;  //    17     
	UINT32                                    : 2 ;  // 19:18     reserved
	UINT32 g1_auto_init                       : 1 ;  //    20     
	UINT32 g1_auto_2nd                        : 1 ;  //    21     
	UINT32                                    : 2 ;  // 23:22     reserved
	UINT32 g2_auto_init                       : 1 ;  //    24     
	UINT32 g2_auto_2nd                        : 1 ;  //    25     
	UINT32                                    : 2 ;  // 27:26     reserved
	UINT32 g3_auto_init                       : 1 ;  //    28     
	UINT32 g3_auto_2nd                        : 1 ;  //    29     
} M14B0_DEO_PIC_INIT_T;

/*-----------------------------------------------------------------------------
		0xc0023004L pic_start ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 g0_pic_start                       : 1 ;  //     0     
	UINT32 g1_pic_start                       : 1 ;  //     1     
	UINT32 g2_pic_start                       : 1 ;  //     2     
	UINT32 g3_pic_start                       : 1 ;  //     3     
} M14B0_DEO_PIC_START_T;

/*-----------------------------------------------------------------------------
		0xc0023008L ctrl_src_pic_init ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 idx_src_g0_init                    : 2 ;  //  1: 0     
	UINT32 idx_src_g1_init                    : 2 ;  //  3: 2     
	UINT32 idx_src_g2_init                    : 2 ;  //  5: 4     
	UINT32 idx_src_g3_init                    : 2 ;  //  7: 6     
	UINT32 idx_src_g0_start                   : 2 ;  //  9: 8     
	UINT32 idx_src_g1_start                   : 2 ;  // 11:10     
	UINT32 idx_src_g2_start                   : 2 ;  // 13:12     
	UINT32 idx_src_g3_start                   : 2 ;  // 15:14     
	UINT32 idx_src_g0_src_pulse               : 2 ;  // 17:16     
	UINT32 idx_src_g1_src_pulse               : 2 ;  // 19:18     
	UINT32 idx_src_g2_src_pulse               : 2 ;  // 21:20     
	UINT32 idx_src_g3_src_pulse               : 2 ;  // 23:22     
} M14B0_DEO_CTRL_SRC_PIC_INIT_T;

/*-----------------------------------------------------------------------------
		0xc002300cL ctrl_gap_irq_to_pic_init ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 gap_src_pulse_to_pic_init          : 28;  // 27: 0     
	UINT32 end_pic_init                       : 2 ;  // 29:28     
	UINT32 str_src_pulse                      : 2 ;  // 31:30     
} M14B0_DEO_CTRL_GAP_IRQ_TO_PIC_INIT_T;

/*-----------------------------------------------------------------------------
		0xc0023010L intr_reg ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 intr_src_pulse0                    : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 intr_pic_end_g0                    : 1 ;  //     4     
	UINT32                                    : 9 ;  // 13: 5     reserved
	UINT32 intr_gmau_flush_rd_done            : 1 ;  //    14     
	UINT32 intr_gmau_flush_wr_done            : 1 ;  //    15     
	UINT32 intr_pe0_m_intr                    : 1 ;  //    16     
} M14B0_DEO_INTR_REG_T;

/*-----------------------------------------------------------------------------
		0xc0023014L intr_mask ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mask_src_pulse0                    : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 mask_pic_end_g0                    : 1 ;  //     4     
	UINT32                                    : 9 ;  // 13: 5     reserved
	UINT32 mask_gmau_flush_rd_done            : 1 ;  //    14     
	UINT32 mask_gmau_flush_wr_done            : 1 ;  //    15     
	UINT32 mask_pe0_m_intr                    : 1 ;  //    16     
} M14B0_DEO_INTR_MASK_T;

/*-----------------------------------------------------------------------------
		0xc0023018L intr_mux ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mux_src_pulse0                     : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 mux_pic_end_g0                     : 1 ;  //     4     
	UINT32                                    : 9 ;  // 13: 5     reserved
	UINT32 mux_gmau_flush_rd_done             : 1 ;  //    14     
	UINT32 mux_gmau_flush_wr_done             : 1 ;  //    15     
	UINT32 mux_pe0_m_intr                     : 1 ;  //    16     
} M14B0_DEO_INTR_MUX_T;

/*-----------------------------------------------------------------------------
		0xc002301cL intr_en ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 en_src_pulse0                      : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 en_pic_end_g0                      : 1 ;  //     4     
	UINT32                                    : 9 ;  // 13: 5     reserved
	UINT32 en_gmau_flush_rd_done              : 1 ;  //    14     
	UINT32 en_gmau_flush_wr_done              : 1 ;  //    15     
	UINT32 en_pe0_m_intr                      : 1 ;  //    16     
} M14B0_DEO_INTR_EN_T;

/*-----------------------------------------------------------------------------
		0xc0023020L ctrl_venc_if ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 venc_vsync                         : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 venc_sel                           : 1 ;  //     4     
} M14B0_DEO_CTRL_VENC_IF_T;

/*-----------------------------------------------------------------------------
		0xc00230c0L cve_param0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vertical_back_period               : 8 ;  //  7: 0     
	UINT32 vertical_sync_period               : 8 ;  // 15: 8     
	UINT32 vertical_front_period              : 8 ;  // 23:16     
	UINT32 vsync_polarity                     : 1 ;  //    24     
} M14B0_DEO_CVE_PARAM0_T;

/*-----------------------------------------------------------------------------
		0xc00230c4L cve_param1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 horizontal_back_period             : 8 ;  //  7: 0     
	UINT32 horizontal_sync_period             : 8 ;  // 15: 8     
	UINT32 horizontal_front_period            : 8 ;  // 23:16     
	UINT32 hsync_polarity                     : 1 ;  //    24     
} M14B0_DEO_CVE_PARAM1_T;

/*-----------------------------------------------------------------------------
		0xc00230c8L cve_param2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 v_hoffset1                         : 11;  // 10: 0     
	UINT32 v_hoffset0                         : 11;  // 21:11     
	UINT32                                    : 2 ;  // 23:22     reserved
	UINT32 field_id_polarity                  : 1 ;  //    24     
} M14B0_DEO_CVE_PARAM2_T;

/*-----------------------------------------------------------------------------
		0xc00230ccL cve_param3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cve_sync_ha                        : 11;  // 10: 0     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 cve_sync_va                        : 11;  // 26:16     
} M14B0_DEO_CVE_PARAM3_T;

/*-----------------------------------------------------------------------------
		0xc00230d0L ctrl_processing_timer ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sel_src_processing_timer_g0        : 1 ;  //     0     
	UINT32 sel_src_processing_timer_g1        : 1 ;  //     1     
	UINT32 sel_src_processing_timer_g2        : 1 ;  //     2     
	UINT32 sel_src_processing_timer_g3        : 1 ;  //     3     
} M14B0_DEO_CTRL_PROCESSING_TIMER_T;

/*-----------------------------------------------------------------------------
		0xc00230d4L info_processing_timer ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 info_processing_timer              : 32;  // 31: 0     
} M14B0_DEO_INFO_PROCESSING_TIMER_T;

/*-----------------------------------------------------------------------------
		0xc00230d8L ctrl_rst_async_bridge ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ctrl_rst_async_bridge              : 16;  // 15: 0     
} M14B0_DEO_CTRL_RST_ASYNC_BRIDGE_T;

/*-----------------------------------------------------------------------------
		0xc00230dcL cve_pulse_dly ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cve_pulse_dly                      : 28;  // 27: 0     
} M14B0_DEO_CVE_PULSE_DLY_T;

/*-----------------------------------------------------------------------------
		0xc00230e0L pic_init_g0_timer ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pic_init_g0_timer                  : 28;  // 27: 0     
} M14B0_DEO_PIC_INIT_G0_TIMER_T;

/*-----------------------------------------------------------------------------
		0xc00230e4L pic_init_g1_timer ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pic_init_g1_timer                  : 28;  // 27: 0     
} M14B0_DEO_PIC_INIT_G1_TIMER_T;

/*-----------------------------------------------------------------------------
		0xc00230e8L pic_init_g2_timer ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pic_init_g2_timer                  : 28;  // 27: 0     
} M14B0_DEO_PIC_INIT_G2_TIMER_T;

/*-----------------------------------------------------------------------------
		0xc00230ecL pic_init_g3_timer ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pic_init_g3_timer                  : 28;  // 27: 0     
} M14B0_DEO_PIC_INIT_G3_TIMER_T;

/*-----------------------------------------------------------------------------
		0xc00230f0L ctrl_pic_end ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 dnr_m_en                           : 1 ;  //     0     
	UINT32 sm_en                              : 1 ;  //     1     
	UINT32 pe0_s_en                           : 1 ;  //     2     
	UINT32 ipc0_en                            : 1 ;  //     3     
	UINT32 ipc1_en                            : 1 ;  //     4     
	UINT32 tnr0_en                            : 1 ;  //     5     
	UINT32 tnr_w_en                           : 1 ;  //     6     
	UINT32 ss_en                              : 1 ;  //     7     
	UINT32 ss_w_en                            : 1 ;  //     8     
	UINT32 atop_en                            : 1 ;  //     9     
	UINT32 cve_r_en                           : 1 ;  //    10     
	UINT32 dvi_en                             : 1 ;  //    11     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 pic_end_g0_force_value             : 1 ;  //    16     
	UINT32 pic_end_g0_force_en                : 1 ;  //    17     
	UINT32 pic_end_g1_force_value             : 1 ;  //    18     
	UINT32 pic_end_g1_force_en                : 1 ;  //    19     
	UINT32 pic_end_g2_force_value             : 1 ;  //    20     
	UINT32 pic_end_g2_force_en                : 1 ;  //    21     
	UINT32 pic_end_g3_force_value             : 1 ;  //    22     
	UINT32 pic_end_g3_force_en                : 1 ;  //    23     
} M14B0_DEO_CTRL_PIC_END_T;

/*-----------------------------------------------------------------------------
		0xc00230f4L info_pic_end ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pic_end_g0                         : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 pic_turn_g0                        : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 pic_end_g1                         : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 pic_turn_g1                        : 1 ;  //    12     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 pic_end_g2                         : 1 ;  //    16     
	UINT32                                    : 3 ;  // 19:17     reserved
	UINT32 pic_turn_g2                        : 1 ;  //    20     
	UINT32                                    : 3 ;  // 23:21     reserved
	UINT32 pic_end_g3                         : 1 ;  //    24     
	UINT32                                    : 3 ;  // 27:25     reserved
	UINT32 pic_turn_g3                        : 1 ;  //    28     
} M14B0_DEO_INFO_PIC_END_T;

/*-----------------------------------------------------------------------------
		0xc00230f8L ctrl_fpga ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ctrl_fpga                      : 32;  // 31: 0     
} M14B0_DEO_CTRL_FPGA_T;

/*-----------------------------------------------------------------------------
		0xc00230fcL ctrl_gmau ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ask_cmd_dly_cnt                    : 8 ;  //  7: 0     
	UINT32                                    : 8 ;  // 15: 8     reserved
	UINT32 ask_to_cnt                         : 10;  // 25:16     
	UINT32                                    : 2 ;  // 27:26     reserved
	UINT32 gmau_flush_rd_en                   : 1 ;  //    28     
	UINT32 gmau_flush_wr_en                   : 1 ;  //    29     
	UINT32 gmau_flush_rd_done                 : 1 ;  //    30     
	UINT32 gmau_flush_wr_done                 : 1 ;  //    31     
} M14B0_DEO_CTRL_GMAU_T;

/*-----------------------------------------------------------------------------
		0xc0024000L pic_init ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 g0_pic_init                        : 1 ;  //     0     
	UINT32 g1_pic_init                        : 1 ;  //     1     
	UINT32 g2_pic_init                        : 1 ;  //     2     
	UINT32 g3_pic_init                        : 1 ;  //     3     
	UINT32                                    : 12;  // 15: 4     reserved
	UINT32 g0_auto_init                       : 1 ;  //    16     
	UINT32                                    : 3 ;  // 19:17     reserved
	UINT32 g1_auto_init                       : 1 ;  //    20     
	UINT32                                    : 3 ;  // 23:21     reserved
	UINT32 g2_auto_init                       : 1 ;  //    24     
	UINT32                                    : 3 ;  // 27:25     reserved
	UINT32 g3_auto_init                       : 1 ;  //    28     
} M14B0_DEP_PIC_INIT_T;

/*-----------------------------------------------------------------------------
		0xc0024004L pic_start ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 g0_pic_start                       : 1 ;  //     0     
	UINT32 g1_pic_start                       : 1 ;  //     1     
	UINT32 g2_pic_start                       : 1 ;  //     2     
	UINT32 g3_pic_start                       : 1 ;  //     3     
} M14B0_DEP_PIC_START_T;

/*-----------------------------------------------------------------------------
		0xc0024008L ctrl_src_pic_init ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 idx_src_g0_init                    : 2 ;  //  1: 0     
	UINT32 idx_src_g1_init                    : 2 ;  //  3: 2     
	UINT32 idx_src_g2_init                    : 2 ;  //  5: 4     
	UINT32 idx_src_g3_init                    : 2 ;  //  7: 6     
	UINT32 idx_src_g0_start                   : 2 ;  //  9: 8     
	UINT32 idx_src_g1_start                   : 2 ;  // 11:10     
	UINT32 idx_src_g2_start                   : 2 ;  // 13:12     
	UINT32 idx_src_g3_start                   : 2 ;  // 15:14     
	UINT32 idx_src_g0_src_pulse               : 2 ;  // 17:16     
	UINT32 idx_src_g1_src_pulse               : 2 ;  // 19:18     
	UINT32 idx_src_g2_src_pulse               : 2 ;  // 21:20     
	UINT32 idx_src_g3_src_pulse               : 2 ;  // 23:22     
	UINT32 idx_src_dei_dem_msrc               : 2 ;  // 25:24     
	UINT32 idx_src_dei_dem_ssrc               : 2 ;  // 27:26     
	UINT32 idx_src_dei_deo_msrc               : 2 ;  // 29:28     
	UINT32 idx_src_dei_deo_ssrc               : 2 ;  // 31:30     
} M14B0_DEP_CTRL_SRC_PIC_INIT_T;

/*-----------------------------------------------------------------------------
		0xc002400cL ctrl_gap_irq_to_pic_init ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 gap_src_pulse_to_pic_init          : 28;  // 27: 0     
	UINT32 end_pic_init                       : 2 ;  // 29:28     
	UINT32 str_src_pulse                      : 2 ;  // 31:30     
} M14B0_DEP_CTRL_GAP_IRQ_TO_PIC_INIT_T;

/*-----------------------------------------------------------------------------
		0xc0024010L intr_reg ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 intr_disp_pulse                    : 1 ;  //     0     
	UINT32                                    : 5 ;  //  5: 1     reserved
	UINT32 intr_pic_end_g2                    : 1 ;  //     6     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 intr_sync_i_vs_ris                 : 1 ;  //     8     
	UINT32 intr_sync_i_va_ris                 : 1 ;  //     9     
	UINT32 intr_sync_i_vs_fal                 : 1 ;  //    10     
	UINT32 intr_sync_i_va_fal                 : 1 ;  //    11     
	UINT32                                    : 2 ;  // 13:12     reserved
	UINT32 intr_gmau_flush_rd_done            : 1 ;  //    14     
	UINT32 intr_gmau_flush_wr_done            : 1 ;  //    15     
} M14B0_DEP_INTR_REG_T;

/*-----------------------------------------------------------------------------
		0xc0024014L intr_mask ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 2 ;  //  1: 0     reserved
	UINT32 mask_pic_end_g2                    : 1 ;  //     2     
	UINT32                                    : 5 ;  //  7: 3     reserved
	UINT32 mask_sync_i_vs_ris                 : 1 ;  //     8     
	UINT32 mask_sync_i_va_ris                 : 1 ;  //     9     
	UINT32 mask_sync_i_vs_fal                 : 1 ;  //    10     
	UINT32 mask_sync_i_va_fal                 : 1 ;  //    11     
	UINT32 mask_gmau_flush_wr_done_ris        : 1 ;  //    12     
	UINT32 mask_gmau_flush_rd_done_ris        : 1 ;  //    13     
	UINT32                                    : 1 ;  //    14     reserved
	UINT32 mask_disp60hz_pulse                : 1 ;  //    15     
} M14B0_DEP_INTR_MASK_T;

/*-----------------------------------------------------------------------------
		0xc0024018L intr_mux ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 2 ;  //  1: 0     reserved
	UINT32 mux_pic_end_g2                     : 1 ;  //     2     
	UINT32                                    : 5 ;  //  7: 3     reserved
	UINT32 mux_sync_i_vs_ris                  : 1 ;  //     8     
	UINT32 mux_sync_i_va_ris                  : 1 ;  //     9     
	UINT32 mux_sync_i_vs_fal                  : 1 ;  //    10     
	UINT32 mux_sync_i_va_fal                  : 1 ;  //    11     
	UINT32 mux_gmau_flush_wr_done_ris         : 1 ;  //    12     
	UINT32 mux_gmau_flush_rd_done_ris         : 1 ;  //    13     
	UINT32                                    : 1 ;  //    14     reserved
	UINT32 mux_disp60hz_pulse                 : 1 ;  //    15     
} M14B0_DEP_INTR_MUX_T;

/*-----------------------------------------------------------------------------
		0xc002401cL intr_en ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 2 ;  //  1: 0     reserved
	UINT32 en_pic_end_g2                      : 1 ;  //     2     
	UINT32                                    : 5 ;  //  7: 3     reserved
	UINT32 en_sync_i_vs_ris                   : 1 ;  //     8     
	UINT32 en_sync_i_va_ris                   : 1 ;  //     9     
	UINT32 en_sync_i_vs_fal                   : 1 ;  //    10     
	UINT32 en_sync_i_va_fal                   : 1 ;  //    11     
	UINT32 en_gmau_flush_wr_done_ris          : 1 ;  //    12     
	UINT32 en_gmau_flush_rd_done_ris          : 1 ;  //    13     
	UINT32                                    : 1 ;  //    14     reserved
	UINT32 en_disp60hz_pulse                  : 1 ;  //    15     
} M14B0_DEP_INTR_EN_T;

/*-----------------------------------------------------------------------------
		0xc0024020L ctrl_dpath ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd_pe_path_sel                    : 2 ;  //  1: 0     
	UINT32 force_dppa_out_pe1                 : 1 ;  //     2     
	UINT32 de_into_osd                        : 1 ;  //     3     
	UINT32 fb_mux_sel                         : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 pe1_60hz_en                        : 1 ;  //     8     
	UINT32 pe1_60hz_de_r                      : 1 ;  //     9     
	UINT32                                    : 2 ;  // 11:10     reserved
	UINT32 dppa_detour_1d                     : 1 ;  //    12     
} M14B0_DEP_CTRL_DPATH_T;

/*-----------------------------------------------------------------------------
		0xc0024024L ctrl_sync ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 dppa_dppb_ha_sel                   : 3 ;  //  2: 0     
	UINT32 sep_osd_i_ha_sel                   : 1 ;  //     3     
	UINT32 dppa_dppb_va_sel                   : 3 ;  //  6: 4     
	UINT32 sep_osd_i_va_sel                   : 1 ;  //     7     
	UINT32 dppa_dppb_hs_sel                   : 3 ;  // 10: 8     
	UINT32 sep_osd_o_hs_sel                   : 1 ;  //    11     
	UINT32 dppa_dppb_vs_sel                   : 3 ;  // 14:12     
	UINT32 sep_osd_o_vs_sel                   : 1 ;  //    15     
	UINT32 osd_i_va_prev_sel                  : 3 ;  // 18:16     
	UINT32 sep_osd_o_ha_sel                   : 1 ;  //    19     
	UINT32 osd_sync_gen_vs_in                 : 1 ;  //    20     
	UINT32 osd_sync_for_ud                    : 1 ;  //    21     
	UINT32 sync_pe1_out                       : 1 ;  //    22     
	UINT32 osd_sync_gen_hs_in                 : 1 ;  //    23     
	UINT32 mof_sync_mask_en                   : 1 ;  //    24     
	UINT32 sep_osd_o_va_sel                   : 1 ;  //    25     
	UINT32 osd_sync_gen_ha_in                 : 1 ;  //    26     
	UINT32 osd_sync_gen_va_in                 : 1 ;  //    27     
	UINT32 mof_sync_mask_sel                  : 2 ;  // 29:28     
	UINT32 ctrl_sync_src_va                   : 1 ;  //    30     
	UINT32 ctrl_sync_src_vs                   : 1 ;  //    31     
} M14B0_DEP_CTRL_SYNC_T;

/*-----------------------------------------------------------------------------
		0xc0024028L ctrl_3d_flag ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_flag_3d_lr_force_value         : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_flag_3d_lr_force_en            : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 reg_flag_3d_force_value            : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 reg_flag_3d_force_en               : 1 ;  //    12     
} M14B0_DEP_CTRL_3D_FLAG_T;

/*-----------------------------------------------------------------------------
		0xc002402cL ctrl_disp_pulse ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_disp_pulse_timer               : 28;  // 27: 0     
	UINT32 reg_disp_pulse_src_sel             : 1 ;  //    28     
	UINT32 reg_disp_pulse_src_inv             : 1 ;  //    29     
} M14B0_DEP_CTRL_DISP_PULSE_T;

/*-----------------------------------------------------------------------------
		0xc0024090L disp_sync_dly_ctrl0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 dly_sync_actv_vsync                : 16;  // 15: 0     
	UINT32 dly_sync_actv_vfp                  : 16;  // 31:16     
} M14B0_DEP_DISP_SYNC_DLY_CTRL0_T;

/*-----------------------------------------------------------------------------
		0xc0024094L disp_sync_dly_ctrl1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 dly_sync_actv_va                   : 16;  // 15: 0     
	UINT32 dly_sync_actv_vbp                  : 16;  // 31:16     
} M14B0_DEP_DISP_SYNC_DLY_CTRL1_T;

/*-----------------------------------------------------------------------------
		0xc0024098L disp_sync_dly_ctrl2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 dly_sync_actv_hsync                : 16;  // 15: 0     
	UINT32 dly_sync_actv_hfp                  : 16;  // 31:16     
} M14B0_DEP_DISP_SYNC_DLY_CTRL2_T;

/*-----------------------------------------------------------------------------
		0xc002409cL disp_sync_dly_ctrl3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 dly_sync_actv_ha                   : 16;  // 15: 0     
	UINT32 dly_sync_actv_hbp                  : 16;  // 31:16     
} M14B0_DEP_DISP_SYNC_DLY_CTRL3_T;

/*-----------------------------------------------------------------------------
		0xc00240a0L disp_sync_dly_ctrl4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 dly_sync_actv_line_start           : 16;  // 15: 0     
	UINT32 dly_sync_actv_sel_in_h_src         : 1 ;  //    16     
	UINT32 dly_sync_actv_sel_in_v_src         : 1 ;  //    17     
	UINT32 dly_sync_actv_sel_in_h_inv         : 1 ;  //    18     
	UINT32 dly_sync_actv_sel_in_v_inv         : 1 ;  //    19     
	UINT32 dly_sync_actv_sel_out_ha           : 1 ;  //    20     
	UINT32 dly_sync_actv_sel_out_va           : 1 ;  //    21     
	UINT32 dly_sync_actv_sel_out_hs           : 1 ;  //    22     
	UINT32 dly_sync_actv_sel_out_vs           : 1 ;  //    23     
	UINT32 tp_update_hv_zero                  : 1 ;  //    24     
	UINT32                                    : 3 ;  // 27:25     reserved
	UINT32 cnt_reset_h_value_en               : 1 ;  //    28     
	UINT32 cnt_reset_v_value_en               : 1 ;  //    29     
	UINT32                                    : 1 ;  //    30     reserved
	UINT32 dly_sync_actv_force_update         : 1 ;  //    31     
} M14B0_DEP_DISP_SYNC_DLY_CTRL4_T;

/*-----------------------------------------------------------------------------
		0xc00240a4L disp_sync_dly_ctrl5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 dly_sync_actv_h_dly                : 16;  // 15: 0     
	UINT32 dly_sync_actv_v_dly                : 16;  // 31:16     
} M14B0_DEP_DISP_SYNC_DLY_CTRL5_T;

/*-----------------------------------------------------------------------------
		0xc00240a8L disp_prev_dly_ctrl0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 dly_prev_vsync                     : 16;  // 15: 0     
	UINT32 dly_prev_vfp                       : 16;  // 31:16     
} M14B0_DEP_DISP_PREV_DLY_CTRL0_T;

/*-----------------------------------------------------------------------------
		0xc00240acL disp_prev_dly_ctrl1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 dly_prev_va                        : 16;  // 15: 0     
	UINT32 dly_prev_vbp                       : 16;  // 31:16     
} M14B0_DEP_DISP_PREV_DLY_CTRL1_T;

/*-----------------------------------------------------------------------------
		0xc00240b0L disp_prev_dly_ctrl2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 dly_prev_hsync                     : 16;  // 15: 0     
	UINT32 dly_prev_hfp                       : 16;  // 31:16     
} M14B0_DEP_DISP_PREV_DLY_CTRL2_T;

/*-----------------------------------------------------------------------------
		0xc00240b4L disp_prev_dly_ctrl3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 dly_prev_ha                        : 16;  // 15: 0     
	UINT32 dly_prev_hbp                       : 16;  // 31:16     
} M14B0_DEP_DISP_PREV_DLY_CTRL3_T;

/*-----------------------------------------------------------------------------
		0xc00240b8L disp_prev_dly_ctrl4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 dly_prev_line_start                : 16;  // 15: 0     
	UINT32 dly_prev_sel_in_h_src              : 1 ;  //    16     
	UINT32 dly_prev_sel_in_v_src              : 1 ;  //    17     
	UINT32 dly_prev_sel_in_h_inv              : 1 ;  //    18     
	UINT32 dly_prev_sel_in_v_inv              : 1 ;  //    19     
	UINT32 dly_prev_sel_out_ha                : 1 ;  //    20     
	UINT32 dly_prev_sel_out_va                : 1 ;  //    21     
	UINT32 dly_prev_sel_out_hs                : 1 ;  //    22     
	UINT32 dly_prev_sel_out_vs                : 1 ;  //    23     
	UINT32 tp_update_hv_zero                  : 1 ;  //    24     
	UINT32                                    : 3 ;  // 27:25     reserved
	UINT32 cnt_reset_h_value_en               : 1 ;  //    28     
	UINT32 cnt_reset_v_value_en               : 1 ;  //    29     
	UINT32                                    : 1 ;  //    30     reserved
	UINT32 dly_prev_force_update              : 1 ;  //    31     
} M14B0_DEP_DISP_PREV_DLY_CTRL4_T;

/*-----------------------------------------------------------------------------
		0xc00240bcL disp_prev_dly_ctrl5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 dly_prev_h_dly                     : 16;  // 15: 0     
	UINT32 dly_prev_v_dly                     : 16;  // 31:16     
} M14B0_DEP_DISP_PREV_DLY_CTRL5_T;

/*-----------------------------------------------------------------------------
		0xc00240d8L ctrl_rst_async_bridge ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ctrl_rst_async_bridge              : 16;  // 15: 0     
} M14B0_DEP_CTRL_RST_ASYNC_BRIDGE_T;

/*-----------------------------------------------------------------------------
		0xc00240e0L pic_init_g0_timer ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pic_init_g0_timer                  : 28;  // 27: 0     
} M14B0_DEP_PIC_INIT_G0_TIMER_T;

/*-----------------------------------------------------------------------------
		0xc00240e4L pic_init_g1_timer ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pic_init_g1_timer                  : 28;  // 27: 0     
} M14B0_DEP_PIC_INIT_G1_TIMER_T;

/*-----------------------------------------------------------------------------
		0xc00240e8L pic_init_g2_timer ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pic_init_g2_timer                  : 28;  // 27: 0     
} M14B0_DEP_PIC_INIT_G2_TIMER_T;

/*-----------------------------------------------------------------------------
		0xc00240ecL pic_init_g3_timer ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pic_init_g3_timer                  : 28;  // 27: 0     
} M14B0_DEP_PIC_INIT_G3_TIMER_T;

/*-----------------------------------------------------------------------------
		0xc00240f0L ctrl_pic_end ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 4 ;  //  3: 0     reserved
	UINT32 osd_en                             : 1 ;  //     4     
	UINT32                                    : 11;  // 15: 5     reserved
	UINT32 pic_end_g0_force_value             : 1 ;  //    16     
	UINT32 pic_end_g0_force_en                : 1 ;  //    17     
	UINT32 pic_end_g1_force_value             : 1 ;  //    18     
	UINT32 pic_end_g1_force_en                : 1 ;  //    19     
	UINT32 pic_end_g2_force_value             : 1 ;  //    20     
	UINT32 pic_end_g2_force_en                : 1 ;  //    21     
	UINT32 pic_end_g3_force_value             : 1 ;  //    22     
	UINT32 pic_end_g3_force_en                : 1 ;  //    23     
} M14B0_DEP_CTRL_PIC_END_T;

/*-----------------------------------------------------------------------------
		0xc00240f4L info_pic_end ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pic_end_g0                         : 1 ;  //     0     
	UINT32                                    : 7 ;  //  7: 1     reserved
	UINT32 pic_end_g1                         : 1 ;  //     8     
	UINT32                                    : 7 ;  // 15: 9     reserved
	UINT32 pic_end_g2                         : 1 ;  //    16     
	UINT32                                    : 7 ;  // 23:17     reserved
	UINT32 pic_end_g3                         : 1 ;  //    24     
} M14B0_DEP_INFO_PIC_END_T;

/*-----------------------------------------------------------------------------
		0xc00240f8L ctrl_fpga ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ctrl_fpga                      : 32;  // 31: 0     
} M14B0_DEP_CTRL_FPGA_T;

/*-----------------------------------------------------------------------------
		0xc00240fcL ctrl_gmau ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ask_cmd_dly_cnt                    : 8 ;  //  7: 0     
	UINT32                                    : 8 ;  // 15: 8     reserved
	UINT32 ask_to_cnt                         : 10;  // 25:16     
	UINT32                                    : 2 ;  // 27:26     reserved
	UINT32 gmau_flush_rd_en                   : 1 ;  //    28     
	UINT32 gmau_flush_wr_en                   : 1 ;  //    29     
	UINT32 gmau_flush_rd_done                 : 1 ;  //    30     
	UINT32 gmau_flush_wr_done                 : 1 ;  //    31     
} M14B0_DEP_CTRL_GMAU_T;

typedef struct {
	M14B0_DEI_PIC_INIT_T                   	pic_init;                     //0xc0020000L
	M14B0_DEI_PIC_START_T                  	pic_start;                    //0xc0020004L
	M14B0_DEI_CTRL_SRC_PIC_INIT_T          	ctrl_src_pic_init;            //0xc0020008L
	M14B0_DEI_CTRL_GAP_IRQ_TO_PIC_INIT_T   	ctrl_gap_irq_to_pic_init;     //0xc002000cL
	M14B0_DEI_INTR_REG_T                   	intr_reg;                     //0xc0020010L
	M14B0_DEI_INTR_MASK_T                  	intr_mask;                    //0xc0020014L
	M14B0_DEI_INTR_MUX_T                   	intr_mux;                     //0xc0020018L
	M14B0_DEI_INTR_EN_T                    	intr_en;                      //0xc002001cL
	M14B0_DEI_MSRC0_SYNC_MUX_CTRL_T        	msrc0_sync_mux_ctrl;          //0xc0020020L
	M14B0_DEI_MSRC0_SYNC_DLY_T             	msrc0_sync_dly;               //0xc0020024L
	M14B0_DEI_MSRC0_SYNC_DLY_LINE_T        	msrc0_sync_dly_line;          //0xc0020028L
	M14B0_DEI_MSRC0_RO_VBLK_CNT_T          	msrc0_ro_vblk_cnt;            //0xc002002cL
	M14B0_DEI_MSRC1_SYNC_MUX_CTRL_T        	msrc1_sync_mux_ctrl;          //0xc0020030L
	M14B0_DEI_MSRC1_SYNC_DLY_T             	msrc1_sync_dly;               //0xc0020034L
	M14B0_DEI_MSRC1_SYNC_DLY_LINE_T        	msrc1_sync_dly_line;          //0xc0020038L
	M14B0_DEI_MSRC1_RO_VBLK_CNT_T          	msrc1_ro_vblk_cnt;            //0xc002003cL
	M14B0_DEI_MSRC2_SYNC_MUX_CTRL_T        	msrc2_sync_mux_ctrl;          //0xc0020040L
	M14B0_DEI_MSRC2_SYNC_DLY_T             	msrc2_sync_dly;               //0xc0020044L
	M14B0_DEI_MSRC2_SYNC_DLY_LINE_T        	msrc2_sync_dly_line;          //0xc0020048L
	M14B0_DEI_MSRC2_RO_VBLK_CNT_T          	msrc2_ro_vblk_cnt;            //0xc002004cL
	M14B0_DEI_MSRC3_SYNC_MUX_CTRL_T        	msrc3_sync_mux_ctrl;          //0xc0020050L
	M14B0_DEI_MSRC3_SYNC_DLY_T             	msrc3_sync_dly;               //0xc0020054L
	M14B0_DEI_MSRC3_SYNC_DLY_LINE_T        	msrc3_sync_dly_line;          //0xc0020058L
	M14B0_DEI_MSRC3_RO_VBLK_CNT_T          	msrc3_ro_vblk_cnt;            //0xc002005cL
	M14B0_DEI_MSRC4_SYNC_MUX_CTRL_T        	msrc4_sync_mux_ctrl;          //0xc0020060L
	M14B0_DEI_MSRC4_SYNC_DLY_T             	msrc4_sync_dly;               //0xc0020064L
	M14B0_DEI_MSRC4_SYNC_DLY_LINE_T        	msrc4_sync_dly_line;          //0xc0020068L
	M14B0_DEI_MSRC4_RO_VBLK_CNT_T          	msrc4_ro_vblk_cnt;            //0xc002006cL
	M14B0_DEI_INFO_CHA_T                   	info_cha;                     //0xc0020070L
	M14B0_DEI_INFO_CHB_T                   	info_chb;                     //0xc0020074L
	UINT32                             	reserved00[2];                //0xc0020078L~0xc002007cL
	M14B0_DEI_CTRL_BLOCK_INPUT_T           	ctrl_block_input;             //0xc0020080L
	M14B0_DEI_CTRL_MUX_CV_T                	ctrl_mux_cv;                  //0xc0020084L
	M14B0_DEI_CTRL_MUX_MVI_T_T             	ctrl_mux_mvi_t;               //0xc0020088L
	M14B0_DEI_CTRL_MUX_MVI_R_T             	ctrl_mux_mvi_r;               //0xc002008cL
	M14B0_DEI_MVI_TA_V_CLIP_CTRL_T         	mvi_ta_v_clip_ctrl;           //0xc0020090L
	M14B0_DEI_MVI_TA_BRIDGE_CTRL_T         	mvi_ta_bridge_ctrl;           //0xc0020094L
	M14B0_DEI_MVI_TB_V_CLIP_CTRL_T         	mvi_tb_v_clip_ctrl;           //0xc0020098L
	M14B0_DEI_MVI_TB_BRIDGE_CTRL_T         	mvi_tb_bridge_ctrl;           //0xc002009cL
	M14B0_DEI_CTRL_VDO_VS_RDY_T            	ctrl_vdo_vs_rdy;              //0xc00200a0L
	M14B0_DEI_CTRL_VDO_VS_RDY_LENGTH_T     	ctrl_vdo_vs_rdy_length;       //0xc00200a4L
	M14B0_DEI_CTRL_VDO_VS_RDY_TIMER0_T     	ctrl_vdo_vs_rdy_timer0;       //0xc00200a8L
	M14B0_DEI_CTRL_VDO_VS_RDY_TIMER1_T     	ctrl_vdo_vs_rdy_timer1;       //0xc00200acL
	M14B0_DEI_MEM0_SYNC_PERIOD_T           	mem0_sync_period;             //0xc00200b0L
	M14B0_DEI_MEM1_SYNC_PERIOD_T           	mem1_sync_period;             //0xc00200b4L
	M14B0_DEI_MEM2_SYNC_PERIOD_T           	mem2_sync_period;             //0xc00200b8L
	M14B0_DEI_MEM3_SYNC_PERIOD_T           	mem3_sync_period;             //0xc00200bcL
	M14B0_DEI_MEM4_SYNC_PERIOD_T           	mem4_sync_period;             //0xc00200c0L
	M14B0_DEI_PIC_INIT_G4_TIMER_T          	pic_init_g4_timer;            //0xc00200c4L
	M14B0_DEI_CTRL_VDO_VS_RDY_TIMER2_T     	ctrl_vdo_vs_rdy_timer2;       //0xc00200c8L
	M14B0_DEI_CTRL_SRC_DCO_MFC_T           	ctrl_src_dco_mfc;             //0xc00200ccL
	M14B0_DEI_CTRL_PROCESSING_TIMER_T      	ctrl_processing_timer;        //0xc00200d0L
	M14B0_DEI_INFO_PROCESSING_TIMER_T      	info_processing_timer;        //0xc00200d4L
	M14B0_DEI_CTRL_RST_ASYNC_BRIDGE_T      	ctrl_rst_async_bridge;        //0xc00200d8L
	M14B0_DEI_DE_VERSION_T                 	de_version;                   //0xc00200dcL
	M14B0_DEI_PIC_INIT_G0_TIMER_T          	pic_init_g0_timer;            //0xc00200e0L
	M14B0_DEI_PIC_INIT_G1_TIMER_T          	pic_init_g1_timer;            //0xc00200e4L
	M14B0_DEI_PIC_INIT_G2_TIMER_T          	pic_init_g2_timer;            //0xc00200e8L
	M14B0_DEI_PIC_INIT_G3_TIMER_T          	pic_init_g3_timer;            //0xc00200ecL
	M14B0_DEI_CTRL_PIC_END_T               	ctrl_pic_end;                 //0xc00200f0L
	M14B0_DEI_INFO_PIC_END_T               	info_pic_end;                 //0xc00200f4L
	M14B0_DEI_CTRL_FPGA_T                  	ctrl_fpga;                    //0xc00200f8L
	M14B0_DEI_CTRL_GMAU_T                  	ctrl_gmau;                    //0xc00200fcL
} DE_DEI_REG_M14B0_T;

typedef struct {
	M14B0_DEM_PIC_INIT_T                   	pic_init;                     //0xc0022000L
	M14B0_DEM_PIC_START_T                  	pic_start;                    //0xc0022004L
	M14B0_DEM_CTRL_SRC_PIC_INIT_T          	ctrl_src_pic_init;            //0xc0022008L
	M14B0_DEM_CTRL_GAP_IRQ_TO_PIC_INIT_T   	ctrl_gap_irq_to_pic_init;     //0xc002200cL
	M14B0_DEM_INTR_REG_T                   	intr_reg;                     //0xc0022010L
	M14B0_DEM_INTR_MASK_T                  	intr_mask;                    //0xc0022014L
	M14B0_DEM_INTR_MUX_T                   	intr_mux;                     //0xc0022018L
	M14B0_DEM_INTR_EN_T                    	intr_en;                      //0xc002201cL
	UINT32                             	reserved00[44];               //0xc0022020L~0xc00220ccL
	M14B0_DEM_CTRL_PROCESSING_TIMER_T      	ctrl_processing_timer;        //0xc00220d0L
	M14B0_DEM_INFO_PROCESSING_TIMER_T      	info_processing_timer;        //0xc00220d4L
	M14B0_DEM_CTRL_RST_ASYNC_BRIDGE_T      	ctrl_rst_async_bridge;        //0xc00220d8L
	UINT32                             	reserved01;                   //0xc00220dcL
	M14B0_DEM_PIC_INIT_G0_TIMER_T          	pic_init_g0_timer;            //0xc00220e0L
	M14B0_DEM_PIC_INIT_G1_TIMER_T          	pic_init_g1_timer;            //0xc00220e4L
	M14B0_DEM_PIC_INIT_G2_TIMER_T          	pic_init_g2_timer;            //0xc00220e8L
	M14B0_DEM_PIC_INIT_G3_TIMER_T          	pic_init_g3_timer;            //0xc00220ecL
	M14B0_DEM_CTRL_PIC_END_T               	ctrl_pic_end;                 //0xc00220f0L
	M14B0_DEM_INFO_PIC_END_T               	info_pic_end;                 //0xc00220f4L
	M14B0_DEM_CTRL_FPGA_T                  	ctrl_fpga;                    //0xc00220f8L
	M14B0_DEM_CTRL_GMAU_T                  	ctrl_gmau;                    //0xc00220fcL
} DE_DEM_REG_M14B0_T;

typedef struct {
	M14B0_DEO_PIC_INIT_T                   	pic_init;                     //0xc0023000L
	M14B0_DEO_PIC_START_T                  	pic_start;                    //0xc0023004L
	M14B0_DEO_CTRL_SRC_PIC_INIT_T          	ctrl_src_pic_init;            //0xc0023008L
	M14B0_DEO_CTRL_GAP_IRQ_TO_PIC_INIT_T   	ctrl_gap_irq_to_pic_init;     //0xc002300cL
	M14B0_DEO_INTR_REG_T                   	intr_reg;                     //0xc0023010L
	M14B0_DEO_INTR_MASK_T                  	intr_mask;                    //0xc0023014L
	M14B0_DEO_INTR_MUX_T                   	intr_mux;                     //0xc0023018L
	M14B0_DEO_INTR_EN_T                    	intr_en;                      //0xc002301cL
	M14B0_DEO_CTRL_VENC_IF_T               	ctrl_venc_if;                 //0xc0023020L
	UINT32                             	reserved00[39];               //0xc0023024L~0xc00230bcL
	M14B0_DEO_CVE_PARAM0_T                 	cve_param0;                   //0xc00230c0L
	M14B0_DEO_CVE_PARAM1_T                 	cve_param1;                   //0xc00230c4L
	M14B0_DEO_CVE_PARAM2_T                 	cve_param2;                   //0xc00230c8L
	M14B0_DEO_CVE_PARAM3_T                 	cve_param3;                   //0xc00230ccL
	M14B0_DEO_CTRL_PROCESSING_TIMER_T      	ctrl_processing_timer;        //0xc00230d0L
	M14B0_DEO_INFO_PROCESSING_TIMER_T      	info_processing_timer;        //0xc00230d4L
	M14B0_DEO_CTRL_RST_ASYNC_BRIDGE_T      	ctrl_rst_async_bridge;        //0xc00230d8L
	M14B0_DEO_CVE_PULSE_DLY_T              	cve_pulse_dly;                //0xc00230dcL
	M14B0_DEO_PIC_INIT_G0_TIMER_T          	pic_init_g0_timer;            //0xc00230e0L
	M14B0_DEO_PIC_INIT_G1_TIMER_T          	pic_init_g1_timer;            //0xc00230e4L
	M14B0_DEO_PIC_INIT_G2_TIMER_T          	pic_init_g2_timer;            //0xc00230e8L
	M14B0_DEO_PIC_INIT_G3_TIMER_T          	pic_init_g3_timer;            //0xc00230ecL
	M14B0_DEO_CTRL_PIC_END_T               	ctrl_pic_end;                 //0xc00230f0L
	M14B0_DEO_INFO_PIC_END_T               	info_pic_end;                 //0xc00230f4L
	M14B0_DEO_CTRL_FPGA_T                  	ctrl_fpga;                    //0xc00230f8L
	M14B0_DEO_CTRL_GMAU_T                  	ctrl_gmau;                    //0xc00230fcL
} DE_DEO_REG_M14B0_T;

typedef struct {
	M14B0_DEP_PIC_INIT_T                   	pic_init;                     //0xc0024000L
	M14B0_DEP_PIC_START_T                  	pic_start;                    //0xc0024004L
	M14B0_DEP_CTRL_SRC_PIC_INIT_T          	ctrl_src_pic_init;            //0xc0024008L
	M14B0_DEP_CTRL_GAP_IRQ_TO_PIC_INIT_T   	ctrl_gap_irq_to_pic_init;     //0xc002400cL
	M14B0_DEP_INTR_REG_T                   	intr_reg;                     //0xc0024010L
	M14B0_DEP_INTR_MASK_T                  	intr_mask;                    //0xc0024014L
	M14B0_DEP_INTR_MUX_T                   	intr_mux;                     //0xc0024018L
	M14B0_DEP_INTR_EN_T                    	intr_en;                      //0xc002401cL
	M14B0_DEP_CTRL_DPATH_T                 	ctrl_dpath;                   //0xc0024020L
	M14B0_DEP_CTRL_SYNC_T                  	ctrl_sync;                    //0xc0024024L
	M14B0_DEP_CTRL_3D_FLAG_T               	ctrl_3d_flag;                 //0xc0024028L
	M14B0_DEP_CTRL_DISP_PULSE_T            	ctrl_disp_pulse;              //0xc002402cL
	UINT32                             	reserved00[24];               //0xc0024030L~0xc002408cL
	M14B0_DEP_DISP_SYNC_DLY_CTRL0_T        	disp_sync_dly_ctrl0;          //0xc0024090L
	M14B0_DEP_DISP_SYNC_DLY_CTRL1_T        	disp_sync_dly_ctrl1;          //0xc0024094L
	M14B0_DEP_DISP_SYNC_DLY_CTRL2_T        	disp_sync_dly_ctrl2;          //0xc0024098L
	M14B0_DEP_DISP_SYNC_DLY_CTRL3_T        	disp_sync_dly_ctrl3;          //0xc002409cL
	M14B0_DEP_DISP_SYNC_DLY_CTRL4_T        	disp_sync_dly_ctrl4;          //0xc00240a0L
	M14B0_DEP_DISP_SYNC_DLY_CTRL5_T        	disp_sync_dly_ctrl5;          //0xc00240a4L
	M14B0_DEP_DISP_PREV_DLY_CTRL0_T        	disp_prev_dly_ctrl0;          //0xc00240a8L
	M14B0_DEP_DISP_PREV_DLY_CTRL1_T        	disp_prev_dly_ctrl1;          //0xc00240acL
	M14B0_DEP_DISP_PREV_DLY_CTRL2_T        	disp_prev_dly_ctrl2;          //0xc00240b0L
	M14B0_DEP_DISP_PREV_DLY_CTRL3_T        	disp_prev_dly_ctrl3;          //0xc00240b4L
	M14B0_DEP_DISP_PREV_DLY_CTRL4_T        	disp_prev_dly_ctrl4;          //0xc00240b8L
	M14B0_DEP_DISP_PREV_DLY_CTRL5_T        	disp_prev_dly_ctrl5;          //0xc00240bcL
	UINT32                             	reserved01[6];                //0xc00240c0L~0xc00240d4L
	M14B0_DEP_CTRL_RST_ASYNC_BRIDGE_T      	ctrl_rst_async_bridge;        //0xc00240d8L
	UINT32                             	reserved02;                   //0xc00240dcL
	M14B0_DEP_PIC_INIT_G0_TIMER_T          	pic_init_g0_timer;            //0xc00240e0L
	M14B0_DEP_PIC_INIT_G1_TIMER_T          	pic_init_g1_timer;            //0xc00240e4L
	M14B0_DEP_PIC_INIT_G2_TIMER_T          	pic_init_g2_timer;            //0xc00240e8L
	M14B0_DEP_PIC_INIT_G3_TIMER_T          	pic_init_g3_timer;            //0xc00240ecL
	M14B0_DEP_CTRL_PIC_END_T               	ctrl_pic_end;                 //0xc00240f0L
	M14B0_DEP_INFO_PIC_END_T               	info_pic_end;                 //0xc00240f4L
	M14B0_DEP_CTRL_FPGA_T                  	ctrl_fpga;                    //0xc00240f8L
	M14B0_DEP_CTRL_GMAU_T                  	ctrl_gmau;                    //0xc00240fcL
} DE_DEP_REG_M14B0_T;

#endif
