#ifndef _IDS_TOPCTRL_CVI_H_
#define _IDS_TOPCTRL_CVI_H_
/*-----------------------------------------------------------------------------
		0xc8000000L pic_init ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 g0_pic_init                        : 1 ;  //     0     
	UINT32 g1_pic_init                        : 1 ;  //     1     
	UINT32 g2_pic_init                        : 1 ;  //     2     
	UINT32 g3_pic_init                        : 1 ;  //     3     
	UINT32 g4_pic_init                        : 1 ;  //     4     
	UINT32 g5_pic_init                        : 1 ;  //     5     
	UINT32 g6_pic_init                        : 1 ;  //     6     
	UINT32 g7_pic_init                        : 1 ;  //     7     
	UINT32                                    : 8 ;  // 15: 8     reserved
	UINT32 g0_auto_init                       : 1 ;  //    16     
	UINT32 g1_auto_init                       : 1 ;  //    17     
	UINT32 g2_auto_init                       : 1 ;  //    18     
	UINT32 g3_auto_init                       : 1 ;  //    19     
	UINT32 g4_auto_init                       : 1 ;  //    20     
	UINT32 g5_auto_init                       : 1 ;  //    21     
	UINT32 g6_auto_init                       : 1 ;  //    22     
	UINT32 g7_auto_init                       : 1 ;  //    23     
} H15A0_CVC_PIC_INIT_T;

/*-----------------------------------------------------------------------------
		0xc8000004L pic_start ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 g0_pic_start                       : 1 ;  //     0     
	UINT32 g1_pic_start                       : 1 ;  //     1     
	UINT32 g2_pic_start                       : 1 ;  //     2     
	UINT32 g3_pic_start                       : 1 ;  //     3     
	UINT32 g4_pic_start                       : 1 ;  //     4     
	UINT32 g5_pic_start                       : 1 ;  //     5     
	UINT32 g6_pic_start                       : 1 ;  //     6     
	UINT32 g7_pic_start                       : 1 ;  //     7     
} H15A0_CVC_PIC_START_T;

/*-----------------------------------------------------------------------------
		0xc8000008L ctrl_auto_init ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 idx_src_g0_auto_init               : 3 ;  //  2: 0     
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 idx_src_g1_auto_init               : 3 ;  //  6: 4     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 idx_src_g2_auto_init               : 3 ;  // 10: 8     
	UINT32                                    : 1 ;  //    11     reserved
	UINT32 idx_src_g3_auto_init               : 3 ;  // 14:12     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 idx_src_g4_auto_init               : 3 ;  // 18:16     
	UINT32                                    : 1 ;  //    19     reserved
	UINT32 idx_src_g5_auto_init               : 3 ;  // 22:20     
	UINT32                                    : 1 ;  //    23     reserved
	UINT32 idx_src_g6_auto_init               : 3 ;  // 26:24     
	UINT32                                    : 1 ;  //    27     reserved
	UINT32 idx_src_g7_auto_init               : 3 ;  // 30:28     
} H15A0_CVC_CTRL_AUTO_INIT_T;

/*-----------------------------------------------------------------------------
		0xc800000cL ctrl_intr_pulse ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 idx_src_i0_intr_pulse              : 3 ;  //  2: 0     
	UINT32 sel_src_i0_intr_pulse              : 1 ;  //     3     
	UINT32 idx_src_i1_intr_pulse              : 3 ;  //  6: 4     
	UINT32 sel_src_i1_intr_pulse              : 1 ;  //     7     
	UINT32 idx_src_i2_intr_pulse              : 3 ;  // 10: 8     
	UINT32 sel_src_i2_intr_pulse              : 1 ;  //    11     
	UINT32 idx_src_i3_intr_pulse              : 3 ;  // 14:12     
	UINT32 sel_src_i3_intr_pulse              : 1 ;  //    15     
	UINT32 idx_src_i4_intr_pulse              : 3 ;  // 18:16     
	UINT32 sel_src_i4_intr_pulse              : 1 ;  //    19     
	UINT32 idx_src_i5_intr_pulse              : 3 ;  // 22:20     
	UINT32 sel_src_i5_intr_pulse              : 1 ;  //    23     
	UINT32 idx_src_i6_intr_pulse              : 3 ;  // 26:24     
	UINT32 sel_src_i6_intr_pulse              : 1 ;  //    27     
	UINT32 idx_src_i7_intr_pulse              : 3 ;  // 30:28     
	UINT32 sel_src_i7_intr_pulse              : 1 ;  //    31     
} H15A0_CVC_CTRL_INTR_PULSE_T;

/*-----------------------------------------------------------------------------
		0xc8000010L intr_reg ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 intr_i0_intr_pulse                 : 1 ;  //     0     
	UINT32 intr_i1_intr_pulse                 : 1 ;  //     1     
	UINT32 intr_i2_intr_pulse                 : 1 ;  //     2     
	UINT32 intr_i3_intr_pulse                 : 1 ;  //     3     
	UINT32 intr_i4_intr_pulse                 : 1 ;  //     4     
	UINT32 intr_i5_intr_pulse                 : 1 ;  //     5     
	UINT32 intr_i6_intr_pulse                 : 1 ;  //     6     
	UINT32 intr_i7_intr_pulse                 : 1 ;  //     7     
	UINT32 intr_g0_pic_end                    : 1 ;  //     8     
	UINT32 intr_g1_pic_end                    : 1 ;  //     9     
	UINT32 intr_g2_pic_end                    : 1 ;  //    10     
	UINT32 intr_g3_pic_end                    : 1 ;  //    11     
	UINT32 intr_g4_pic_end                    : 1 ;  //    12     
	UINT32 intr_g5_pic_end                    : 1 ;  //    13     
	UINT32 intr_g6_pic_end                    : 1 ;  //    14     
	UINT32 intr_g7_pic_end                    : 1 ;  //    15     
	UINT32 intr_cvi0                          : 1 ;  //    16     
	UINT32 intr_cvi1                          : 1 ;  //    17     
	UINT32 intr_cvi2                          : 1 ;  //    18     
	UINT32 intr_cvi3                          : 1 ;  //    19     
	UINT32 intr_cvi0_field                    : 1 ;  //    20     
	UINT32 intr_cvi1_field                    : 1 ;  //    21     
	UINT32 intr_cvi2_field                    : 1 ;  //    22     
	UINT32 intr_cvi3_field                    : 1 ;  //    23     
	UINT32 intr_cvi0_frame_lr                 : 1 ;  //    24     
	UINT32 intr_cvi1_frame_lr                 : 1 ;  //    25     
	UINT32 intr_cvi2_frame_lr                 : 1 ;  //    26     
	UINT32 intr_cvi3_frame_lr                 : 1 ;  //    27     
	UINT32 intr_cvd                           : 1 ;  //    28     
	UINT32 intr_wss                           : 1 ;  //    29     
	UINT32 intr_cgms                          : 1 ;  //    30     
} H15A0_CVC_INTR_REG_T;

/*-----------------------------------------------------------------------------
		0xc8000014L intr_mask ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mask_i0_src_pulse                  : 1 ;  //     0     
	UINT32 mask_i1_src_pulse                  : 1 ;  //     1     
	UINT32 mask_i2_src_pulse                  : 1 ;  //     2     
	UINT32 mask_i3_src_pulse                  : 1 ;  //     3     
	UINT32 mask_i4_src_pulse                  : 1 ;  //     4     
	UINT32 mask_i5_src_pulse                  : 1 ;  //     5     
	UINT32 mask_i6_src_pulse                  : 1 ;  //     6     
	UINT32 mask_i7_src_pulse                  : 1 ;  //     7     
	UINT32 mask_g0_pic_end                    : 1 ;  //     8     
	UINT32 mask_g1_pic_end                    : 1 ;  //     9     
	UINT32 mask_g2_pic_end                    : 1 ;  //    10     
	UINT32 mask_g3_pic_end                    : 1 ;  //    11     
	UINT32 mask_g4_pic_end                    : 1 ;  //    12     
	UINT32 mask_g5_pic_end                    : 1 ;  //    13     
	UINT32 mask_g6_pic_end                    : 1 ;  //    14     
	UINT32 mask_g7_pic_end                    : 1 ;  //    15     
	UINT32 mask_cvi0                          : 1 ;  //    16     
	UINT32 mask_cvi1                          : 1 ;  //    17     
	UINT32 mask_cvi2                          : 1 ;  //    18     
	UINT32 mask_cvi3                          : 1 ;  //    19     
	UINT32 mask_cvi0_field                    : 1 ;  //    20     
	UINT32 mask_cvi1_field                    : 1 ;  //    21     
	UINT32 mask_cvi2_field                    : 1 ;  //    22     
	UINT32 mask_cvi3_field                    : 1 ;  //    23     
	UINT32 mask_cvi0_frame_lr                 : 1 ;  //    24     
	UINT32 mask_cvi1_frame_lr                 : 1 ;  //    25     
	UINT32 mask_cvi2_frame_lr                 : 1 ;  //    26     
	UINT32 mask_cvi3_frame_lr                 : 1 ;  //    27     
	UINT32 mask_cvd                           : 1 ;  //    28     
	UINT32 mask_wss                           : 1 ;  //    29     
	UINT32 mask_cgms                          : 1 ;  //    30     
} H15A0_CVC_INTR_MASK_T;

/*-----------------------------------------------------------------------------
		0xc8000018L intr_mux ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mux_i0_src_pulse                   : 1 ;  //     0     
	UINT32 mux_i1_src_pulse                   : 1 ;  //     1     
	UINT32 mux_i2_src_pulse                   : 1 ;  //     2     
	UINT32 mux_i3_src_pulse                   : 1 ;  //     3     
	UINT32 mux_i4_src_pulse                   : 1 ;  //     4     
	UINT32 mux_i5_src_pulse                   : 1 ;  //     5     
	UINT32 mux_i6_src_pulse                   : 1 ;  //     6     
	UINT32 mux_i7_src_pulse                   : 1 ;  //     7     
	UINT32 mux_g0_pic_end                     : 1 ;  //     8     
	UINT32 mux_g1_pic_end                     : 1 ;  //     9     
	UINT32 mux_g2_pic_end                     : 1 ;  //    10     
	UINT32 mux_g3_pic_end                     : 1 ;  //    11     
	UINT32 mux_g4_pic_end                     : 1 ;  //    12     
	UINT32 mux_g5_pic_end                     : 1 ;  //    13     
	UINT32 mux_g6_pic_end                     : 1 ;  //    14     
	UINT32 mux_g7_pic_end                     : 1 ;  //    15     
	UINT32 mux_cvi0                           : 1 ;  //    16     
	UINT32 mux_cvi1                           : 1 ;  //    17     
	UINT32 mux_cvi2                           : 1 ;  //    18     
	UINT32 mux_cvi3                           : 1 ;  //    19     
	UINT32 mux_cvi0_field                     : 1 ;  //    20     
	UINT32 mux_cvi1_field                     : 1 ;  //    21     
	UINT32 mux_cvi2_field                     : 1 ;  //    22     
	UINT32 mux_cvi3_field                     : 1 ;  //    23     
	UINT32 mux_cvi0_frame_lr                  : 1 ;  //    24     
	UINT32 mux_cvi1_frame_lr                  : 1 ;  //    25     
	UINT32 mux_cvi2_frame_lr                  : 1 ;  //    26     
	UINT32 mux_cvi3_frame_lr                  : 1 ;  //    27     
	UINT32 mux_cvd                            : 1 ;  //    28     
	UINT32 mux_wss                            : 1 ;  //    29     
	UINT32 mux_cgms                           : 1 ;  //    30     
} H15A0_CVC_INTR_MUX_T;

/*-----------------------------------------------------------------------------
		0xc800001cL intr_en ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 en_i0_src_pulse                    : 1 ;  //     0     
	UINT32 en_i1_src_pulse                    : 1 ;  //     1     
	UINT32 en_i2_src_pulse                    : 1 ;  //     2     
	UINT32 en_i3_src_pulse                    : 1 ;  //     3     
	UINT32 en_i4_src_pulse                    : 1 ;  //     4     
	UINT32 en_i5_src_pulse                    : 1 ;  //     5     
	UINT32 en_i6_src_pulse                    : 1 ;  //     6     
	UINT32 en_i7_src_pulse                    : 1 ;  //     7     
	UINT32 en_g0_pic_end                      : 1 ;  //     8     
	UINT32 en_g1_pic_end                      : 1 ;  //     9     
	UINT32 en_g2_pic_end                      : 1 ;  //    10     
	UINT32 en_g3_pic_end                      : 1 ;  //    11     
	UINT32 en_g4_pic_end                      : 1 ;  //    12     
	UINT32 en_g5_pic_end                      : 1 ;  //    13     
	UINT32 en_g6_pic_end                      : 1 ;  //    14     
	UINT32 en_g7_pic_end                      : 1 ;  //    15     
	UINT32 en_cvi0                            : 1 ;  //    16     
	UINT32 en_cvi1                            : 1 ;  //    17     
	UINT32 en_cvi2                            : 1 ;  //    18     
	UINT32 en_cvi3                            : 1 ;  //    19     
	UINT32 en_cvi0_field                      : 1 ;  //    20     
	UINT32 en_cvi1_field                      : 1 ;  //    21     
	UINT32 en_cvi2_field                      : 1 ;  //    22     
	UINT32 en_cvi3_field                      : 1 ;  //    23     
	UINT32 en_cvi0_frame_lr                   : 1 ;  //    24     
	UINT32 en_cvi1_frame_lr                   : 1 ;  //    25     
	UINT32 en_cvi2_frame_lr                   : 1 ;  //    26     
	UINT32 en_cvi3_frame_lr                   : 1 ;  //    27     
	UINT32 en_cvd                             : 1 ;  //    28     
	UINT32 en_wss                             : 1 ;  //    29     
	UINT32 en_cgms                            : 1 ;  //    30     
} H15A0_CVC_INTR_EN_T;

/*-----------------------------------------------------------------------------
		0xc8000020L info_cvi0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 valid_d                            : 1 ;  //     0     
	UINT32 valid_h                            : 1 ;  //     1     
	UINT32 valid_v                            : 1 ;  //     2     
	UINT32 valid_s                            : 1 ;  //     3     
	UINT32 scan                               : 1 ;  //     4     
	UINT32 bit_mode                           : 1 ;  //     5     
	UINT32 aspect                             : 1 ;  //     6     
	UINT32 sel_src                            : 4 ;  // 10: 7     
	UINT32 frame_lr                           : 1 ;  //    11     
	UINT32 field_id                           : 1 ;  //    12     
	UINT32 vactive                            : 1 ;  //    13     
} H15A0_CVC_INFO_CVI0_T;

/*-----------------------------------------------------------------------------
		0xc8000024L info_cvi1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 valid_d                            : 1 ;  //     0     
	UINT32 valid_h                            : 1 ;  //     1     
	UINT32 valid_v                            : 1 ;  //     2     
	UINT32 valid_s                            : 1 ;  //     3     
	UINT32 scan                               : 1 ;  //     4     
	UINT32 bit_mode                           : 1 ;  //     5     
	UINT32 aspect                             : 1 ;  //     6     
	UINT32 sel_src                            : 4 ;  // 10: 7     
	UINT32 frame_lr                           : 1 ;  //    11     
	UINT32 field_id                           : 1 ;  //    12     
	UINT32 vactive                            : 1 ;  //    13     
} H15A0_CVC_INFO_CVI1_T;

/*-----------------------------------------------------------------------------
		0xc8000028L info_cvi2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 valid_d                            : 1 ;  //     0     
	UINT32 valid_h                            : 1 ;  //     1     
	UINT32 valid_v                            : 1 ;  //     2     
	UINT32 valid_s                            : 1 ;  //     3     
	UINT32 scan                               : 1 ;  //     4     
	UINT32 bit_mode                           : 1 ;  //     5     
	UINT32 aspect                             : 1 ;  //     6     
	UINT32 sel_src                            : 4 ;  // 10: 7     
	UINT32 frame_lr                           : 1 ;  //    11     
	UINT32 field_id                           : 1 ;  //    12     
	UINT32 vactive                            : 1 ;  //    13     
} H15A0_CVC_INFO_CVI2_T;

/*-----------------------------------------------------------------------------
		0xc800002cL info_cvi3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 valid_d                            : 1 ;  //     0     
	UINT32 valid_h                            : 1 ;  //     1     
	UINT32 valid_v                            : 1 ;  //     2     
	UINT32 valid_s                            : 1 ;  //     3     
	UINT32 scan                               : 1 ;  //     4     
	UINT32 bit_mode                           : 1 ;  //     5     
	UINT32 aspect                             : 1 ;  //     6     
	UINT32 sel_src                            : 4 ;  // 10: 7     
	UINT32 frame_lr                           : 1 ;  //    11     
	UINT32 field_id                           : 1 ;  //    12     
	UINT32 vactive                            : 1 ;  //    13     
} H15A0_CVC_INFO_CVI3_T;

/*-----------------------------------------------------------------------------
		0xc8000080L ctrl_block_mux ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cvd_scart_en                   : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_lvds_type_sel                  : 1 ;  //     4     
	UINT32                                    : 2 ;  //  6: 5     reserved
	UINT32 reg_ch3_input_type                 : 1 ;  //     7     
	UINT32                                    : 4 ;  // 11: 8     reserved
	UINT32 reg_hdmi_input_type                : 1 ;  //    12     
	UINT32                                    : 11;  // 23:13     reserved
	UINT32 reg_cvd_input_mux_sel              : 1 ;  //    24     
} H15A0_CVC_CTRL_BLOCK_MUX_T;

/*-----------------------------------------------------------------------------
		0xc80000c0L pic_init_g0_timer ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 start_interval_mode_en             : 1 ;  //     0     
	UINT32 start_interval_mode                : 3 ;  //  3: 1     
	UINT32 g0_pic_init_timer                  : 28;  // 31: 4     
} H15A0_CVC_PIC_INIT_G0_TIMER_T;

/*-----------------------------------------------------------------------------
		0xc80000c4L pic_init_g1_timer ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 start_interval_mode_en             : 1 ;  //     0     
	UINT32 start_interval_mode                : 3 ;  //  3: 1     
	UINT32 g1_pic_init_timer                  : 28;  // 31: 4     
} H15A0_CVC_PIC_INIT_G1_TIMER_T;

/*-----------------------------------------------------------------------------
		0xc80000c8L pic_init_g2_timer ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 start_interval_mode_en             : 1 ;  //     0     
	UINT32 start_interval_mode                : 3 ;  //  3: 1     
	UINT32 g2_pic_init_timer                  : 28;  // 31: 4     
} H15A0_CVC_PIC_INIT_G2_TIMER_T;

/*-----------------------------------------------------------------------------
		0xc80000ccL pic_init_g3_timer ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 start_interval_mode_en             : 1 ;  //     0     
	UINT32 start_interval_mode                : 3 ;  //  3: 1     
	UINT32 g3_pic_init_timer                  : 28;  // 31: 4     
} H15A0_CVC_PIC_INIT_G3_TIMER_T;

/*-----------------------------------------------------------------------------
		0xc80000d0L pic_init_g4_timer ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 start_interval_mode_en             : 1 ;  //     0     
	UINT32 start_interval_mode                : 3 ;  //  3: 1     
	UINT32 g4_pic_init_timer                  : 28;  // 31: 4     
} H15A0_CVC_PIC_INIT_G4_TIMER_T;

/*-----------------------------------------------------------------------------
		0xc80000d4L pic_init_g5_timer ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 start_interval_mode_en             : 1 ;  //     0     
	UINT32 start_interval_mode                : 3 ;  //  3: 1     
	UINT32 g5_pic_init_timer                  : 28;  // 31: 4     
} H15A0_CVC_PIC_INIT_G5_TIMER_T;

/*-----------------------------------------------------------------------------
		0xc80000d8L pic_init_g6_timer ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 start_interval_mode_en             : 1 ;  //     0     
	UINT32 start_interval_mode                : 3 ;  //  3: 1     
	UINT32 g6_pic_init_timer                  : 28;  // 31: 4     
} H15A0_CVC_PIC_INIT_G6_TIMER_T;

/*-----------------------------------------------------------------------------
		0xc80000dcL pic_init_g7_timer ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 start_interval_mode_en             : 1 ;  //     0     
	UINT32 start_interval_mode                : 3 ;  //  3: 1     
	UINT32 g7_pic_init_timer                  : 28;  // 31: 4     
} H15A0_CVC_PIC_INIT_G7_TIMER_T;

/*-----------------------------------------------------------------------------
		0xc80000e4L dbg_timer_run ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 info_timer_run                     : 24;  // 23: 0     
	UINT32 ctrl_timer_run                     : 8 ;  // 31:24     
} H15A0_CVC_DBG_TIMER_RUN_T;

/*-----------------------------------------------------------------------------
		0xc80000e8L dbg_timer_blk ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 info_timer_blk                     : 24;  // 23: 0     
	UINT32 ctrl_timer_blk                     : 8 ;  // 31:24     
} H15A0_CVC_DBG_TIMER_BLK_T;

/*-----------------------------------------------------------------------------
		0xc80000ecL ctrl_monitor ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ctrl_monitor                       : 32;  // 31: 0     
} H15A0_CVC_CTRL_MONITOR_T;

/*-----------------------------------------------------------------------------
		0xc80000f0L ctrl_pic_end ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 b0_pic_end_en                      : 1 ;  //     0     
	UINT32 b1_pic_end_en                      : 1 ;  //     1     
	UINT32 b2_pic_end_en                      : 1 ;  //     2     
	UINT32 b3_pic_end_en                      : 1 ;  //     3     
	UINT32 b4_pic_end_en                      : 1 ;  //     4     
	UINT32 b5_pic_end_en                      : 1 ;  //     5     
	UINT32 b6_pic_end_en                      : 1 ;  //     6     
	UINT32 b7_pic_end_en                      : 1 ;  //     7     
	UINT32 b8_pic_end_en                      : 1 ;  //     8     
	UINT32 b9_pic_end_en                      : 1 ;  //     9     
	UINT32 b10_pic_end_en                     : 1 ;  //    10     
	UINT32 b11_pic_end_en                     : 1 ;  //    11     
	UINT32 b12_pic_end_en                     : 1 ;  //    12     
	UINT32 b13_pic_end_en                     : 1 ;  //    13     
	UINT32 b14_pic_end_en                     : 1 ;  //    14     
	UINT32 b15_pic_end_en                     : 1 ;  //    15     
	UINT32 pic_end_g0_force_en                : 1 ;  //    16     
	UINT32 pic_end_g1_force_en                : 1 ;  //    17     
	UINT32 pic_end_g2_force_en                : 1 ;  //    18     
	UINT32 pic_end_g3_force_en                : 1 ;  //    19     
	UINT32 pic_end_g4_force_en                : 1 ;  //    20     
	UINT32 pic_end_g5_force_en                : 1 ;  //    21     
	UINT32 pic_end_g6_force_en                : 1 ;  //    22     
	UINT32 pic_end_g7_force_en                : 1 ;  //    23     
	UINT32 pic_end_g0_force_value             : 1 ;  //    24     
	UINT32 pic_end_g1_force_value             : 1 ;  //    25     
	UINT32 pic_end_g2_force_value             : 1 ;  //    26     
	UINT32 pic_end_g3_force_value             : 1 ;  //    27     
	UINT32 pic_end_g4_force_value             : 1 ;  //    28     
	UINT32 pic_end_g5_force_value             : 1 ;  //    29     
	UINT32 pic_end_g6_force_value             : 1 ;  //    30     
	UINT32 pic_end_g7_force_value             : 1 ;  //    31     
} H15A0_CVC_CTRL_PIC_END_T;

/*-----------------------------------------------------------------------------
		0xc80000f4L info_pic_end ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 g0_pic_end                         : 1 ;  //     0     
	UINT32 g1_pic_end                         : 1 ;  //     1     
	UINT32 g2_pic_end                         : 1 ;  //     2     
	UINT32 g3_pic_end                         : 1 ;  //     3     
	UINT32 g4_pic_end                         : 1 ;  //     4     
	UINT32 g5_pic_end                         : 1 ;  //     5     
	UINT32 g6_pic_end                         : 1 ;  //     6     
	UINT32 g7_pic_end                         : 1 ;  //     7     
	UINT32                                    : 8 ;  // 15: 8     reserved
	UINT32 g0_pic_turn                        : 1 ;  //    16     
	UINT32 g1_pic_turn                        : 1 ;  //    17     
	UINT32 g2_pic_turn                        : 1 ;  //    18     
	UINT32 g3_pic_turn                        : 1 ;  //    19     
	UINT32 g4_pic_turn                        : 1 ;  //    20     
	UINT32 g5_pic_turn                        : 1 ;  //    21     
	UINT32 g6_pic_turn                        : 1 ;  //    22     
	UINT32 g7_pic_turn                        : 1 ;  //    23     
} H15A0_CVC_INFO_PIC_END_T;

/*-----------------------------------------------------------------------------
		0xc80000f8L ctrl_gpio ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ctrl_gpio                      : 32;  // 31: 0     
} H15A0_CVC_CTRL_GPIO_T;

/*-----------------------------------------------------------------------------
		0xc80000fcL ctrl_gmau ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ask_cmd_dly_cnt                    : 8 ;  //  7: 0     
	UINT32 smx_aruser                         : 4 ;  // 11: 8     
	UINT32 smx_awuser                         : 4 ;  // 15:12     
	UINT32 ask_to_cnt                         : 10;  // 25:16     
} H15A0_CVC_CTRL_GMAU_T;

/*-----------------------------------------------------------------------------
		0xc800b000L pic_init ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 g0_pic_init                        : 1 ;  //     0     
	UINT32 g1_pic_init                        : 1 ;  //     1     
	UINT32 g2_pic_init                        : 1 ;  //     2     
	UINT32 g3_pic_init                        : 1 ;  //     3     
	UINT32 g4_pic_init                        : 1 ;  //     4     
	UINT32 g5_pic_init                        : 1 ;  //     5     
	UINT32 g6_pic_init                        : 1 ;  //     6     
	UINT32 g7_pic_init                        : 1 ;  //     7     
	UINT32                                    : 8 ;  // 15: 8     reserved
	UINT32 g0_auto_init                       : 1 ;  //    16     
	UINT32 g1_auto_init                       : 1 ;  //    17     
	UINT32 g2_auto_init                       : 1 ;  //    18     
	UINT32 g3_auto_init                       : 1 ;  //    19     
	UINT32 g4_auto_init                       : 1 ;  //    20     
	UINT32 g5_auto_init                       : 1 ;  //    21     
	UINT32 g6_auto_init                       : 1 ;  //    22     
	UINT32 g7_auto_init                       : 1 ;  //    23     
} H15A0_VSD_PIC_INIT_T;

/*-----------------------------------------------------------------------------
		0xc800b004L pic_start ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 g0_pic_start                       : 1 ;  //     0     
	UINT32 g1_pic_start                       : 1 ;  //     1     
	UINT32 g2_pic_start                       : 1 ;  //     2     
	UINT32 g3_pic_start                       : 1 ;  //     3     
	UINT32 g4_pic_start                       : 1 ;  //     4     
	UINT32 g5_pic_start                       : 1 ;  //     5     
	UINT32 g6_pic_start                       : 1 ;  //     6     
	UINT32 g7_pic_start                       : 1 ;  //     7     
	UINT32                                    : 8 ;  // 15: 8     reserved
	UINT32 g0_auto_2nd                        : 1 ;  //    16     
	UINT32 g1_auto_2nd                        : 1 ;  //    17     
	UINT32 g2_auto_2nd                        : 1 ;  //    18     
	UINT32 g3_auto_2nd                        : 1 ;  //    19     
	UINT32 g4_auto_2nd                        : 1 ;  //    20     
	UINT32 g5_auto_2nd                        : 1 ;  //    21     
	UINT32 g6_auto_2nd                        : 1 ;  //    22     
	UINT32 g7_auto_2nd                        : 1 ;  //    23     
} H15A0_VSD_PIC_START_T;

/*-----------------------------------------------------------------------------
		0xc800b008L ctrl_auto_init ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 idx_src_g0_auto_init               : 3 ;  //  2: 0     
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 idx_src_g1_auto_init               : 3 ;  //  6: 4     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 idx_src_g2_auto_init               : 3 ;  // 10: 8     
	UINT32                                    : 1 ;  //    11     reserved
	UINT32 idx_src_g3_auto_init               : 3 ;  // 14:12     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 idx_src_g4_auto_init               : 3 ;  // 18:16     
	UINT32                                    : 1 ;  //    19     reserved
	UINT32 idx_src_g5_auto_init               : 3 ;  // 22:20     
	UINT32                                    : 1 ;  //    23     reserved
	UINT32 idx_src_g6_auto_init               : 3 ;  // 26:24     
	UINT32                                    : 1 ;  //    27     reserved
	UINT32 idx_src_g7_auto_init               : 3 ;  // 30:28     
} H15A0_VSD_CTRL_AUTO_INIT_T;

/*-----------------------------------------------------------------------------
		0xc800b00cL ctrl_intr_pulse ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 idx_src_i0_intr_pulse              : 3 ;  //  2: 0     
	UINT32 sel_src_i0_intr_pulse              : 1 ;  //     3     
	UINT32 idx_src_i1_intr_pulse              : 3 ;  //  6: 4     
	UINT32 sel_src_i1_intr_pulse              : 1 ;  //     7     
	UINT32 idx_src_i2_intr_pulse              : 3 ;  // 10: 8     
	UINT32 sel_src_i2_intr_pulse              : 1 ;  //    11     
	UINT32 idx_src_i3_intr_pulse              : 3 ;  // 14:12     
	UINT32 sel_src_i3_intr_pulse              : 1 ;  //    15     
	UINT32 idx_src_i4_intr_pulse              : 3 ;  // 18:16     
	UINT32 sel_src_i4_intr_pulse              : 1 ;  //    19     
	UINT32 idx_src_i5_intr_pulse              : 3 ;  // 22:20     
	UINT32 sel_src_i5_intr_pulse              : 1 ;  //    23     
	UINT32 idx_src_i6_intr_pulse              : 3 ;  // 26:24     
	UINT32 sel_src_i6_intr_pulse              : 1 ;  //    27     
	UINT32 idx_src_i7_intr_pulse              : 3 ;  // 30:28     
	UINT32 sel_src_i7_intr_pulse              : 1 ;  //    31     
} H15A0_VSD_CTRL_INTR_PULSE_T;

/*-----------------------------------------------------------------------------
		0xc800b010L intr_reg ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 intr_i0_intr_pulse                 : 1 ;  //     0     
	UINT32 intr_i1_intr_pulse                 : 1 ;  //     1     
	UINT32 intr_i2_intr_pulse                 : 1 ;  //     2     
	UINT32 intr_i3_intr_pulse                 : 1 ;  //     3     
	UINT32 intr_i4_intr_pulse                 : 1 ;  //     4     
	UINT32 intr_i5_intr_pulse                 : 1 ;  //     5     
	UINT32 intr_i6_intr_pulse                 : 1 ;  //     6     
	UINT32 intr_i7_intr_pulse                 : 1 ;  //     7     
	UINT32 intr_g0_pic_end                    : 1 ;  //     8     
	UINT32 intr_g1_pic_end                    : 1 ;  //     9     
	UINT32 intr_g2_pic_end                    : 1 ;  //    10     
	UINT32 intr_g3_pic_end                    : 1 ;  //    11     
	UINT32 intr_g4_pic_end                    : 1 ;  //    12     
	UINT32 intr_g5_pic_end                    : 1 ;  //    13     
	UINT32 intr_g6_pic_end                    : 1 ;  //    14     
	UINT32 intr_g7_pic_end                    : 1 ;  //    15     
	UINT32 intr_vcp                           : 1 ;  //    16     
} H15A0_VSD_INTR_REG_T;

/*-----------------------------------------------------------------------------
		0xc800b014L intr_mask ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mask_i0_src_pulse                  : 1 ;  //     0     
	UINT32 mask_i1_src_pulse                  : 1 ;  //     1     
	UINT32 mask_i2_src_pulse                  : 1 ;  //     2     
	UINT32 mask_i3_src_pulse                  : 1 ;  //     3     
	UINT32 mask_i4_src_pulse                  : 1 ;  //     4     
	UINT32 mask_i5_src_pulse                  : 1 ;  //     5     
	UINT32 mask_i6_src_pulse                  : 1 ;  //     6     
	UINT32 mask_i7_src_pulse                  : 1 ;  //     7     
	UINT32 mask_g0_pic_end                    : 1 ;  //     8     
	UINT32 mask_g1_pic_end                    : 1 ;  //     9     
	UINT32 mask_g2_pic_end                    : 1 ;  //    10     
	UINT32 mask_g3_pic_end                    : 1 ;  //    11     
	UINT32 mask_g4_pic_end                    : 1 ;  //    12     
	UINT32 mask_g5_pic_end                    : 1 ;  //    13     
	UINT32 mask_g6_pic_end                    : 1 ;  //    14     
	UINT32 mask_g7_pic_end                    : 1 ;  //    15     
	UINT32 mask_vcp                           : 1 ;  //    16     
} H15A0_VSD_INTR_MASK_T;

/*-----------------------------------------------------------------------------
		0xc800b018L intr_mux ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mux_i0_src_pulse                   : 1 ;  //     0     
	UINT32 mux_i1_src_pulse                   : 1 ;  //     1     
	UINT32 mux_i2_src_pulse                   : 1 ;  //     2     
	UINT32 mux_i3_src_pulse                   : 1 ;  //     3     
	UINT32 mux_i4_src_pulse                   : 1 ;  //     4     
	UINT32 mux_i5_src_pulse                   : 1 ;  //     5     
	UINT32 mux_i6_src_pulse                   : 1 ;  //     6     
	UINT32 mux_i7_src_pulse                   : 1 ;  //     7     
	UINT32 mux_g0_pic_end                     : 1 ;  //     8     
	UINT32 mux_g1_pic_end                     : 1 ;  //     9     
	UINT32 mux_g2_pic_end                     : 1 ;  //    10     
	UINT32 mux_g3_pic_end                     : 1 ;  //    11     
	UINT32 mux_g4_pic_end                     : 1 ;  //    12     
	UINT32 mux_g5_pic_end                     : 1 ;  //    13     
	UINT32 mux_g6_pic_end                     : 1 ;  //    14     
	UINT32 mux_g7_pic_end                     : 1 ;  //    15     
	UINT32 mux_vcp                            : 1 ;  //    16     
} H15A0_VSD_INTR_MUX_T;

/*-----------------------------------------------------------------------------
		0xc800b01cL intr_en ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 en_i0_src_pulse                    : 1 ;  //     0     
	UINT32 en_i1_src_pulse                    : 1 ;  //     1     
	UINT32 en_i2_src_pulse                    : 1 ;  //     2     
	UINT32 en_i3_src_pulse                    : 1 ;  //     3     
	UINT32 en_i4_src_pulse                    : 1 ;  //     4     
	UINT32 en_i5_src_pulse                    : 1 ;  //     5     
	UINT32 en_i6_src_pulse                    : 1 ;  //     6     
	UINT32 en_i7_src_pulse                    : 1 ;  //     7     
	UINT32 en_g0_pic_end                      : 1 ;  //     8     
	UINT32 en_g1_pic_end                      : 1 ;  //     9     
	UINT32 en_g2_pic_end                      : 1 ;  //    10     
	UINT32 en_g3_pic_end                      : 1 ;  //    11     
	UINT32 en_g4_pic_end                      : 1 ;  //    12     
	UINT32 en_g5_pic_end                      : 1 ;  //    13     
	UINT32 en_g6_pic_end                      : 1 ;  //    14     
	UINT32 en_g7_pic_end                      : 1 ;  //    15     
	UINT32 en_vcp                             : 1 ;  //    16     
} H15A0_VSD_INTR_EN_T;

/*-----------------------------------------------------------------------------
		0xc800b020L ctrl_sync_gen_mv0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vbp                                : 16;  // 15: 0     
	UINT32 vfp                                : 16;  // 31:16     
} H15A0_VSD_CTRL_SYNC_GEN_MV0_T;

/*-----------------------------------------------------------------------------
		0xc800b024L ctrl_sync_gen_mv1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vactv                              : 16;  // 15: 0     
	UINT32 vsync                              : 16;  // 31:16     
} H15A0_VSD_CTRL_SYNC_GEN_MV1_T;

/*-----------------------------------------------------------------------------
		0xc800b028L ctrl_sync_gen_mv2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hbp                                : 16;  // 15: 0     
	UINT32 hfp                                : 16;  // 31:16     
} H15A0_VSD_CTRL_SYNC_GEN_MV2_T;

/*-----------------------------------------------------------------------------
		0xc800b02cL ctrl_sync_gen_mv3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hactv                              : 16;  // 15: 0     
	UINT32 hsync                              : 16;  // 31:16     
} H15A0_VSD_CTRL_SYNC_GEN_MV3_T;

/*-----------------------------------------------------------------------------
		0xc800b030L ctrl_sync_gen_mv4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hoffset0                           : 16;  // 15: 0     
	UINT32 force_update_auto_en               : 1 ;  //    16     
	UINT32                                    : 3 ;  // 19:17     reserved
	UINT32 force_update_src_sel               : 2 ;  // 21:20     
	UINT32                                    : 1 ;  //    22     reserved
	UINT32 sync_polarity                      : 1 ;  //    23     
	UINT32                                    : 3 ;  // 26:24     reserved
	UINT32 disp_frame_lr                      : 1 ;  //    27     
	UINT32 disp_pulse_src_sel                 : 1 ;  //    28     
	UINT32                                    : 2 ;  // 30:29     reserved
	UINT32 force_update                       : 1 ;  //    31     
} H15A0_VSD_CTRL_SYNC_GEN_MV4_T;

/*-----------------------------------------------------------------------------
		0xc800b034L ctrl_sync_gen_mv5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 dly_hs                             : 16;  // 15: 0     
	UINT32 dly_vs                             : 16;  // 31:16     
} H15A0_VSD_CTRL_SYNC_GEN_MV5_T;

/*-----------------------------------------------------------------------------
		0xc800b038L ctrl_sync_gen_mv6 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 dly_disp_pulse                     : 28;  // 27: 0     
} H15A0_VSD_CTRL_SYNC_GEN_MV6_T;

/*-----------------------------------------------------------------------------
		0xc800b040L ctrl_sync_gen_so0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vbp                                : 16;  // 15: 0     
	UINT32 vfp                                : 16;  // 31:16     
} H15A0_VSD_CTRL_SYNC_GEN_SO0_T;

/*-----------------------------------------------------------------------------
		0xc800b044L ctrl_sync_gen_so1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vactv                              : 16;  // 15: 0     
	UINT32 vsync                              : 16;  // 31:16     
} H15A0_VSD_CTRL_SYNC_GEN_SO1_T;

/*-----------------------------------------------------------------------------
		0xc800b048L ctrl_sync_gen_so2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hbp                                : 16;  // 15: 0     
	UINT32 hfp                                : 16;  // 31:16     
} H15A0_VSD_CTRL_SYNC_GEN_SO2_T;

/*-----------------------------------------------------------------------------
		0xc800b04cL ctrl_sync_gen_so3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hactv                              : 16;  // 15: 0     
	UINT32 hsync                              : 16;  // 31:16     
} H15A0_VSD_CTRL_SYNC_GEN_SO3_T;

/*-----------------------------------------------------------------------------
		0xc800b050L ctrl_sync_gen_so4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hoffset0                           : 16;  // 15: 0     
	UINT32 force_update_auto_en               : 1 ;  //    16     
	UINT32                                    : 3 ;  // 19:17     reserved
	UINT32 force_update_src_sel               : 2 ;  // 21:20     
	UINT32                                    : 1 ;  //    22     reserved
	UINT32 sync_polarity                      : 1 ;  //    23     
	UINT32                                    : 3 ;  // 26:24     reserved
	UINT32 disp_frame_lr                      : 1 ;  //    27     
	UINT32 disp_pulse_src_sel                 : 1 ;  //    28     
	UINT32                                    : 2 ;  // 30:29     reserved
	UINT32 force_update                       : 1 ;  //    31     
} H15A0_VSD_CTRL_SYNC_GEN_SO4_T;

/*-----------------------------------------------------------------------------
		0xc800b054L ctrl_sync_gen_so5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 dly_hs                             : 16;  // 15: 0     
	UINT32 dly_vs                             : 16;  // 31:16     
} H15A0_VSD_CTRL_SYNC_GEN_SO5_T;

/*-----------------------------------------------------------------------------
		0xc800b058L ctrl_sync_gen_so6 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 dly_disp_pulse                     : 32;  // 31: 0     
} H15A0_VSD_CTRL_SYNC_GEN_SO6_T;

/*-----------------------------------------------------------------------------
		0xc800b060L ctrl_sync_gen_ht0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vbp                                : 16;  // 15: 0     
	UINT32 vfp                                : 16;  // 31:16     
} H15A0_VSD_CTRL_SYNC_GEN_HT0_T;

/*-----------------------------------------------------------------------------
		0xc800b064L ctrl_sync_gen_ht1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vactv                              : 16;  // 15: 0     
	UINT32 vsync                              : 16;  // 31:16     
} H15A0_VSD_CTRL_SYNC_GEN_HT1_T;

/*-----------------------------------------------------------------------------
		0xc800b068L ctrl_sync_gen_ht2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hbp                                : 16;  // 15: 0     
	UINT32 hfp                                : 16;  // 31:16     
} H15A0_VSD_CTRL_SYNC_GEN_HT2_T;

/*-----------------------------------------------------------------------------
		0xc800b06cL ctrl_sync_gen_ht3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hactv                              : 16;  // 15: 0     
	UINT32 hsync                              : 16;  // 31:16     
} H15A0_VSD_CTRL_SYNC_GEN_HT3_T;

/*-----------------------------------------------------------------------------
		0xc800b070L ctrl_sync_gen_ht4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hoffset0                           : 16;  // 15: 0     
	UINT32 force_update_auto_en               : 1 ;  //    16     
	UINT32                                    : 3 ;  // 19:17     reserved
	UINT32 force_update_src_sel               : 2 ;  // 21:20     
	UINT32                                    : 1 ;  //    22     reserved
	UINT32 sync_polarity                      : 1 ;  //    23     
	UINT32                                    : 3 ;  // 26:24     reserved
	UINT32 disp_frame_lr                      : 1 ;  //    27     
	UINT32 disp_pulse_src_sel                 : 1 ;  //    28     
	UINT32                                    : 2 ;  // 30:29     reserved
	UINT32 force_update                       : 1 ;  //    31     
} H15A0_VSD_CTRL_SYNC_GEN_HT4_T;

/*-----------------------------------------------------------------------------
		0xc800b074L ctrl_sync_gen_ht5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 dly_hs                             : 16;  // 15: 0     
	UINT32 dly_vs                             : 16;  // 31:16     
} H15A0_VSD_CTRL_SYNC_GEN_HT5_T;

/*-----------------------------------------------------------------------------
		0xc800b078L ctrl_sync_gen_ht6 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 dly_disp_pulse                     : 32;  // 31: 0     
} H15A0_VSD_CTRL_SYNC_GEN_HT6_T;

/*-----------------------------------------------------------------------------
		0xc800b080L info_dco_mfc ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 dco_mfc                            : 32;  // 31: 0     
} H15A0_VSD_INFO_DCO_MFC_T;

/*-----------------------------------------------------------------------------
		0xc800b084L info_dco_dfc ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 dco_dfc                            : 32;  // 31: 0     
} H15A0_VSD_INFO_DCO_DFC_T;

/*-----------------------------------------------------------------------------
		0xc800b088L info_dco_dflc ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 dco_dflc                           : 32;  // 31: 0     
} H15A0_VSD_INFO_DCO_DFLC_T;

/*-----------------------------------------------------------------------------
		0xc800b08cL ctrl_dco_num ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_o_vsync_num                    : 8 ;  //  7: 0     
	UINT32 reg_disable_gated_o_vsync          : 1 ;  //     8     
	UINT32                                    : 7 ;  // 15: 9     reserved
	UINT32 reg_i_vsync_num                    : 8 ;  // 23:16     
	UINT32 reg_disable_gated_i_vsync          : 1 ;  //    24     
} H15A0_VSD_CTRL_DCO_NUM_T;

/*-----------------------------------------------------------------------------
		0xc800b090L ctrl_dco_fcw ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 dco_fcw                            : 22;  // 21: 0     
} H15A0_VSD_CTRL_DCO_FCW_T;

/*-----------------------------------------------------------------------------
		0xc800b0a0L ctrl_sr_pic_init ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 enable_sr_pic_init                 : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 idx_sr_src                         : 3 ;  //  6: 4     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 idx_sr_init                        : 2 ;  //  9: 8     
	UINT32                                    : 2 ;  // 11:10     reserved
	UINT32 idx_sr_start                       : 2 ;  // 13:12     
} H15A0_VSD_CTRL_SR_PIC_INIT_T;

/*-----------------------------------------------------------------------------
		0xc800b0a4L pic_init_sr_timer ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sr_pic_init_timer                  : 32;  // 31: 0     
} H15A0_VSD_PIC_INIT_SR_TIMER_T;

/*-----------------------------------------------------------------------------
		0xc800b0a8L ctrl_ht_pic_init ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 enable_ht_pic_init                 : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 idx_ht_src                         : 3 ;  //  6: 4     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 idx_ht_init                        : 2 ;  //  9: 8     
	UINT32                                    : 2 ;  // 11:10     reserved
	UINT32 idx_ht_start                       : 2 ;  // 13:12     
} H15A0_VSD_CTRL_HT_PIC_INIT_T;

/*-----------------------------------------------------------------------------
		0xc800b0acL pic_init_ht_timer ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ht_pic_init_timer                  : 32;  // 31: 0     
} H15A0_VSD_PIC_INIT_HT_TIMER_T;

/*-----------------------------------------------------------------------------
		0xc800b0c0L pic_init_g0_timer ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 start_interval_mode_en             : 1 ;  //     0     
	UINT32 start_interval_mode                : 3 ;  //  3: 1     
	UINT32 g0_pic_init_timer                  : 28;  // 31: 4     
} H15A0_VSD_PIC_INIT_G0_TIMER_T;

/*-----------------------------------------------------------------------------
		0xc800b0c4L pic_init_g1_timer ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 start_interval_mode_en             : 1 ;  //     0     
	UINT32 start_interval_mode                : 3 ;  //  3: 1     
	UINT32 g1_pic_init_timer                  : 28;  // 31: 4     
} H15A0_VSD_PIC_INIT_G1_TIMER_T;

/*-----------------------------------------------------------------------------
		0xc800b0c8L pic_init_g2_timer ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 start_interval_mode_en             : 1 ;  //     0     
	UINT32 start_interval_mode                : 3 ;  //  3: 1     
	UINT32 g2_pic_init_timer                  : 28;  // 31: 4     
} H15A0_VSD_PIC_INIT_G2_TIMER_T;

/*-----------------------------------------------------------------------------
		0xc800b0ccL pic_init_g3_timer ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 start_interval_mode_en             : 1 ;  //     0     
	UINT32 start_interval_mode                : 3 ;  //  3: 1     
	UINT32 g3_pic_init_timer                  : 28;  // 31: 4     
} H15A0_VSD_PIC_INIT_G3_TIMER_T;

/*-----------------------------------------------------------------------------
		0xc800b0d0L pic_init_g4_timer ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 start_interval_mode_en             : 1 ;  //     0     
	UINT32 start_interval_mode                : 3 ;  //  3: 1     
	UINT32 g4_pic_init_timer                  : 28;  // 31: 4     
} H15A0_VSD_PIC_INIT_G4_TIMER_T;

/*-----------------------------------------------------------------------------
		0xc800b0d4L pic_init_g5_timer ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 start_interval_mode_en             : 1 ;  //     0     
	UINT32 start_interval_mode                : 3 ;  //  3: 1     
	UINT32 g5_pic_init_timer                  : 28;  // 31: 4     
} H15A0_VSD_PIC_INIT_G5_TIMER_T;

/*-----------------------------------------------------------------------------
		0xc800b0d8L pic_init_g6_timer ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 start_interval_mode_en             : 1 ;  //     0     
	UINT32 start_interval_mode                : 3 ;  //  3: 1     
	UINT32 g6_pic_init_timer                  : 28;  // 31: 4     
} H15A0_VSD_PIC_INIT_G6_TIMER_T;

/*-----------------------------------------------------------------------------
		0xc800b0dcL pic_init_g7_timer ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 start_interval_mode_en             : 1 ;  //     0     
	UINT32 start_interval_mode                : 3 ;  //  3: 1     
	UINT32 g7_pic_init_timer                  : 28;  // 31: 4     
} H15A0_VSD_PIC_INIT_G7_TIMER_T;

/*-----------------------------------------------------------------------------
		0xc800b0e4L dbg_timer_run ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 info_timer_run                     : 24;  // 23: 0     
	UINT32 ctrl_timer_run                     : 8 ;  // 31:24     
} H15A0_VSD_DBG_TIMER_RUN_T;

/*-----------------------------------------------------------------------------
		0xc800b0e8L dbg_timer_blk ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 info_timer_blk                     : 24;  // 23: 0     
	UINT32 ctrl_timer_blk                     : 8 ;  // 31:24     
} H15A0_VSD_DBG_TIMER_BLK_T;

/*-----------------------------------------------------------------------------
		0xc800b0ecL ctrl_monitor ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ctrl_monitor                       : 4 ;  //  3: 0     
} H15A0_VSD_CTRL_MONITOR_T;

/*-----------------------------------------------------------------------------
		0xc800b0f0L ctrl_pic_end ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 b0_pic_end_en                      : 1 ;  //     0     
	UINT32 b1_pic_end_en                      : 1 ;  //     1     
	UINT32 b2_pic_end_en                      : 1 ;  //     2     
	UINT32 b3_pic_end_en                      : 1 ;  //     3     
	UINT32 b4_pic_end_en                      : 1 ;  //     4     
	UINT32 b5_pic_end_en                      : 1 ;  //     5     
	UINT32 b6_pic_end_en                      : 1 ;  //     6     
	UINT32 b7_pic_end_en                      : 1 ;  //     7     
	UINT32 b8_pic_end_en                      : 1 ;  //     8     
	UINT32 b9_pic_end_en                      : 1 ;  //     9     
	UINT32 b10_pic_end_en                     : 1 ;  //    10     
	UINT32 b11_pic_end_en                     : 1 ;  //    11     
	UINT32 b12_pic_end_en                     : 1 ;  //    12     
	UINT32 b13_pic_end_en                     : 1 ;  //    13     
	UINT32 b14_pic_end_en                     : 1 ;  //    14     
	UINT32 b15_pic_end_en                     : 1 ;  //    15     
	UINT32 pic_end_g0_force_en                : 1 ;  //    16     
	UINT32 pic_end_g1_force_en                : 1 ;  //    17     
	UINT32 pic_end_g2_force_en                : 1 ;  //    18     
	UINT32 pic_end_g3_force_en                : 1 ;  //    19     
	UINT32 pic_end_g4_force_en                : 1 ;  //    20     
	UINT32 pic_end_g5_force_en                : 1 ;  //    21     
	UINT32 pic_end_g6_force_en                : 1 ;  //    22     
	UINT32 pic_end_g7_force_en                : 1 ;  //    23     
	UINT32 pic_end_g0_force_value             : 1 ;  //    24     
	UINT32 pic_end_g1_force_value             : 1 ;  //    25     
	UINT32 pic_end_g2_force_value             : 1 ;  //    26     
	UINT32 pic_end_g3_force_value             : 1 ;  //    27     
	UINT32 pic_end_g4_force_value             : 1 ;  //    28     
	UINT32 pic_end_g5_force_value             : 1 ;  //    29     
	UINT32 pic_end_g6_force_value             : 1 ;  //    30     
	UINT32 pic_end_g7_force_value             : 1 ;  //    31     
} H15A0_VSD_CTRL_PIC_END_T;

/*-----------------------------------------------------------------------------
		0xc800b0f4L info_pic_end ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 g0_pic_end                         : 1 ;  //     0     
	UINT32 g1_pic_end                         : 1 ;  //     1     
	UINT32 g2_pic_end                         : 1 ;  //     2     
	UINT32 g3_pic_end                         : 1 ;  //     3     
	UINT32 g4_pic_end                         : 1 ;  //     4     
	UINT32 g5_pic_end                         : 1 ;  //     5     
	UINT32 g6_pic_end                         : 1 ;  //     6     
	UINT32 g7_pic_end                         : 1 ;  //     7     
	UINT32                                    : 8 ;  // 15: 8     reserved
	UINT32 g0_pic_turn                        : 1 ;  //    16     
	UINT32 g1_pic_turn                        : 1 ;  //    17     
	UINT32 g2_pic_turn                        : 1 ;  //    18     
	UINT32 g3_pic_turn                        : 1 ;  //    19     
	UINT32 g4_pic_turn                        : 1 ;  //    20     
	UINT32 g5_pic_turn                        : 1 ;  //    21     
	UINT32 g6_pic_turn                        : 1 ;  //    22     
	UINT32 g7_pic_turn                        : 1 ;  //    23     
} H15A0_VSD_INFO_PIC_END_T;

/*-----------------------------------------------------------------------------
		0xc800b0f8L ctrl_gpio ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 rst_async_bridge_0                 : 1 ;  //     0     
	UINT32 rst_async_bridge_1                 : 1 ;  //     1     
	UINT32 rst_async_bridge_2                 : 1 ;  //     2     
	UINT32                                    : 2 ;  //  4: 3     reserved
	UINT32 sel_input_of_csc_for_sosd          : 1 ;  //     5     
	UINT32 en_csc_for_sosd                    : 1 ;  //     6     
	UINT32 sel_output1                        : 1 ;  //     7     
	UINT32                                    : 1 ;  //     8     reserved
	UINT32 sel_input_of_csc_for_video         : 1 ;  //     9     
	UINT32 en_csc_for_video                   : 1 ;  //    10     
	UINT32 sel_output0                        : 1 ;  //    11     
} H15A0_VSD_CTRL_GPIO_T;

/*-----------------------------------------------------------------------------
		0xc800b0fcL ctrl_gmau ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ask_cmd_dly_cnt                    : 8 ;  //  7: 0     
	UINT32 smx_aruser                         : 4 ;  // 11: 8     
	UINT32 smx_awuser                         : 4 ;  // 15:12     
	UINT32 ask_to_cnt                         : 10;  // 25:16     
} H15A0_VSD_CTRL_GMAU_T;

/*-----------------------------------------------------------------------------
		0xc8024000L pic_init ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 g0_pic_init                        : 1 ;  //     0     
	UINT32 g1_pic_init                        : 1 ;  //     1     
	UINT32 g2_pic_init                        : 1 ;  //     2     
	UINT32 g3_pic_init                        : 1 ;  //     3     
	UINT32 g4_pic_init                        : 1 ;  //     4     
	UINT32 g5_pic_init                        : 1 ;  //     5     
	UINT32 g6_pic_init                        : 1 ;  //     6     
	UINT32 g7_pic_init                        : 1 ;  //     7     
	UINT32                                    : 8 ;  // 15: 8     reserved
	UINT32 g0_auto_init                       : 1 ;  //    16     
	UINT32 g1_auto_init                       : 1 ;  //    17     
	UINT32 g2_auto_init                       : 1 ;  //    18     
	UINT32 g3_auto_init                       : 1 ;  //    19     
	UINT32 g4_auto_init                       : 1 ;  //    20     
	UINT32 g5_auto_init                       : 1 ;  //    21     
	UINT32 g6_auto_init                       : 1 ;  //    22     
	UINT32 g7_auto_init                       : 1 ;  //    23     
} H15A0_CCO_PIC_INIT_T;

/*-----------------------------------------------------------------------------
		0xc8024004L pic_start ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 g0_pic_start                       : 1 ;  //     0     
	UINT32 g1_pic_start                       : 1 ;  //     1     
	UINT32 g2_pic_start                       : 1 ;  //     2     
	UINT32 g3_pic_start                       : 1 ;  //     3     
	UINT32 g4_pic_start                       : 1 ;  //     4     
	UINT32 g5_pic_start                       : 1 ;  //     5     
	UINT32 g6_pic_start                       : 1 ;  //     6     
	UINT32 g7_pic_start                       : 1 ;  //     7     
} H15A0_CCO_PIC_START_T;

/*-----------------------------------------------------------------------------
		0xc8024008L ctrl_auto_init ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 idx_src_g0_auto_init               : 3 ;  //  2: 0     
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 idx_src_g1_auto_init               : 3 ;  //  6: 4     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 idx_src_g2_auto_init               : 3 ;  // 10: 8     
	UINT32                                    : 1 ;  //    11     reserved
	UINT32 idx_src_g3_auto_init               : 3 ;  // 14:12     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 idx_src_g4_auto_init               : 3 ;  // 18:16     
	UINT32                                    : 1 ;  //    19     reserved
	UINT32 idx_src_g5_auto_init               : 3 ;  // 22:20     
	UINT32                                    : 1 ;  //    23     reserved
	UINT32 idx_src_g6_auto_init               : 3 ;  // 26:24     
	UINT32                                    : 1 ;  //    27     reserved
	UINT32 idx_src_g7_auto_init               : 3 ;  // 30:28     
} H15A0_CCO_CTRL_AUTO_INIT_T;

/*-----------------------------------------------------------------------------
		0xc802400cL ctrl_intr_pulse ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 idx_src_i0_intr_pulse              : 3 ;  //  2: 0     
	UINT32 sel_src_i0_intr_pulse              : 1 ;  //     3     
	UINT32 idx_src_i1_intr_pulse              : 3 ;  //  6: 4     
	UINT32 sel_src_i1_intr_pulse              : 1 ;  //     7     
	UINT32 idx_src_i2_intr_pulse              : 3 ;  // 10: 8     
	UINT32 sel_src_i2_intr_pulse              : 1 ;  //    11     
	UINT32 idx_src_i3_intr_pulse              : 3 ;  // 14:12     
	UINT32 sel_src_i3_intr_pulse              : 1 ;  //    15     
	UINT32 idx_src_i4_intr_pulse              : 3 ;  // 18:16     
	UINT32 sel_src_i4_intr_pulse              : 1 ;  //    19     
	UINT32 idx_src_i5_intr_pulse              : 3 ;  // 22:20     
	UINT32 sel_src_i5_intr_pulse              : 1 ;  //    23     
	UINT32 idx_src_i6_intr_pulse              : 3 ;  // 26:24     
	UINT32 sel_src_i6_intr_pulse              : 1 ;  //    27     
	UINT32 idx_src_i7_intr_pulse              : 3 ;  // 30:28     
	UINT32 sel_src_i7_intr_pulse              : 1 ;  //    31     
} H15A0_CCO_CTRL_INTR_PULSE_T;

/*-----------------------------------------------------------------------------
		0xc8024010L intr_reg ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 intr_i0_intr_pulse                 : 1 ;  //     0     
	UINT32 intr_i1_intr_pulse                 : 1 ;  //     1     
	UINT32 intr_i2_intr_pulse                 : 1 ;  //     2     
	UINT32 intr_i3_intr_pulse                 : 1 ;  //     3     
	UINT32 intr_i4_intr_pulse                 : 1 ;  //     4     
	UINT32 intr_i5_intr_pulse                 : 1 ;  //     5     
	UINT32 intr_i6_intr_pulse                 : 1 ;  //     6     
	UINT32 intr_i7_intr_pulse                 : 1 ;  //     7     
	UINT32 intr_g0_pic_end                    : 1 ;  //     8     
	UINT32 intr_g1_pic_end                    : 1 ;  //     9     
	UINT32 intr_g2_pic_end                    : 1 ;  //    10     
	UINT32 intr_g3_pic_end                    : 1 ;  //    11     
	UINT32 intr_g4_pic_end                    : 1 ;  //    12     
	UINT32 intr_g5_pic_end                    : 1 ;  //    13     
	UINT32 intr_g6_pic_end                    : 1 ;  //    14     
	UINT32 intr_g7_pic_end                    : 1 ;  //    15     
	UINT32 intr_osd                           : 1 ;  //    16     
} H15A0_CCO_INTR_REG_T;

/*-----------------------------------------------------------------------------
		0xc8024014L intr_mask ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mask_i0_src_pulse                  : 1 ;  //     0     
	UINT32 mask_i1_src_pulse                  : 1 ;  //     1     
	UINT32 mask_i2_src_pulse                  : 1 ;  //     2     
	UINT32 mask_i3_src_pulse                  : 1 ;  //     3     
	UINT32 mask_i4_src_pulse                  : 1 ;  //     4     
	UINT32 mask_i5_src_pulse                  : 1 ;  //     5     
	UINT32 mask_i6_src_pulse                  : 1 ;  //     6     
	UINT32 mask_i7_src_pulse                  : 1 ;  //     7     
	UINT32 mask_g0_pic_end                    : 1 ;  //     8     
	UINT32 mask_g1_pic_end                    : 1 ;  //     9     
	UINT32 mask_g2_pic_end                    : 1 ;  //    10     
	UINT32 mask_g3_pic_end                    : 1 ;  //    11     
	UINT32 mask_g4_pic_end                    : 1 ;  //    12     
	UINT32 mask_g5_pic_end                    : 1 ;  //    13     
	UINT32 mask_g6_pic_end                    : 1 ;  //    14     
	UINT32 mask_g7_pic_end                    : 1 ;  //    15     
	UINT32 mask_osd                           : 1 ;  //    16     
} H15A0_CCO_INTR_MASK_T;

/*-----------------------------------------------------------------------------
		0xc8024018L intr_mux ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mux_i0_src_pulse                   : 1 ;  //     0     
	UINT32 mux_i1_src_pulse                   : 1 ;  //     1     
	UINT32 mux_i2_src_pulse                   : 1 ;  //     2     
	UINT32 mux_i3_src_pulse                   : 1 ;  //     3     
	UINT32 mux_i4_src_pulse                   : 1 ;  //     4     
	UINT32 mux_i5_src_pulse                   : 1 ;  //     5     
	UINT32 mux_i6_src_pulse                   : 1 ;  //     6     
	UINT32 mux_i7_src_pulse                   : 1 ;  //     7     
	UINT32 mux_g0_pic_end                     : 1 ;  //     8     
	UINT32 mux_g1_pic_end                     : 1 ;  //     9     
	UINT32 mux_g2_pic_end                     : 1 ;  //    10     
	UINT32 mux_g3_pic_end                     : 1 ;  //    11     
	UINT32 mux_g4_pic_end                     : 1 ;  //    12     
	UINT32 mux_g5_pic_end                     : 1 ;  //    13     
	UINT32 mux_g6_pic_end                     : 1 ;  //    14     
	UINT32 mux_g7_pic_end                     : 1 ;  //    15     
	UINT32 mux_osd                            : 1 ;  //    16     
} H15A0_CCO_INTR_MUX_T;

/*-----------------------------------------------------------------------------
		0xc802401cL intr_en ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 en_i0_src_pulse                    : 1 ;  //     0     
	UINT32 en_i1_src_pulse                    : 1 ;  //     1     
	UINT32 en_i2_src_pulse                    : 1 ;  //     2     
	UINT32 en_i3_src_pulse                    : 1 ;  //     3     
	UINT32 en_i4_src_pulse                    : 1 ;  //     4     
	UINT32 en_i5_src_pulse                    : 1 ;  //     5     
	UINT32 en_i6_src_pulse                    : 1 ;  //     6     
	UINT32 en_i7_src_pulse                    : 1 ;  //     7     
	UINT32 en_g0_pic_end                      : 1 ;  //     8     
	UINT32 en_g1_pic_end                      : 1 ;  //     9     
	UINT32 en_g2_pic_end                      : 1 ;  //    10     
	UINT32 en_g3_pic_end                      : 1 ;  //    11     
	UINT32 en_g4_pic_end                      : 1 ;  //    12     
	UINT32 en_g5_pic_end                      : 1 ;  //    13     
	UINT32 en_g6_pic_end                      : 1 ;  //    14     
	UINT32 en_g7_pic_end                      : 1 ;  //    15     
	UINT32 en_osd                             : 1 ;  //    16     
} H15A0_CCO_INTR_EN_T;

/*-----------------------------------------------------------------------------
		0xc8024020L ctrl_dpath ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd_pe_path_sel                    : 2 ;  //  1: 0     
	UINT32 cco_fb_mux                         : 2 ;  //  3: 2     
	UINT32 rgb2yc_o_en                        : 1 ;  //     4     
	UINT32 rgb2yc_e_en                        : 1 ;  //     5     
	UINT32 yc2rgb_o_en                        : 1 ;  //     6     
	UINT32 yc2rgb_e_en                        : 1 ;  //     7     
	UINT32 gcf_rgb2yc_o_en                    : 1 ;  //     8     
	UINT32 gcf_rgb2yc_e_en                    : 1 ;  //     9     
	UINT32                                    : 2 ;  // 11:10     reserved
	UINT32 sel_src_dco_dfc                    : 1 ;  //    12     
	UINT32 sel_osd_src_bosd0                  : 1 ;  //    13     
	UINT32 sel_osd_src_bosd1                  : 1 ;  //    14     
	UINT32 sel_osd_src_sosd                   : 1 ;  //    15     
	UINT32 enable_dco_dfc_on_gcf              : 1 ;  //    16     
	UINT32                                    : 3 ;  // 19:17     reserved
	UINT32 osd0_afbc_en                       : 1 ;  //    20     
	UINT32                                    : 3 ;  // 23:21     reserved
	UINT32 osd1_afbc_en                       : 1 ;  //    24     
	UINT32 cc_eo_hif_en                       : 1 ;  //    25     
	UINT32 cc_eo_hif_data                     : 1 ;  //    26     
	UINT32 cc_eo_hif_ready                    : 1 ;  //    27     
	UINT32 detour_en                          : 1 ;  //    28     
} H15A0_CCO_CTRL_DPATH_T;

/*-----------------------------------------------------------------------------
		0xc8024024L ctrl_spath ''
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
} H15A0_CCO_CTRL_SPATH_T;

/*-----------------------------------------------------------------------------
		0xc8024028L ctrl_3d_flag ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_flag_3d_lr_force_value         : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_flag_3d_lr_force_en            : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 reg_flag_3d_force_value            : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 reg_flag_3d_force_en               : 1 ;  //    12     
} H15A0_CCO_CTRL_3D_FLAG_T;

/*-----------------------------------------------------------------------------
		0xc8024030L ctrl_bosd_pulse ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_disp_pulse_timer               : 28;  // 27: 0     
	UINT32 reg_disp_pulse_src_sel             : 1 ;  //    28     
	UINT32 reg_disp_pulse_src_inv             : 1 ;  //    29     
} H15A0_CCO_CTRL_BOSD_PULSE_T;

/*-----------------------------------------------------------------------------
		0xc8024034L ctrl_sosd_pulse ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_disp_pulse_timer               : 28;  // 27: 0     
	UINT32 reg_disp_pulse_src_sel             : 1 ;  //    28     
	UINT32 reg_disp_pulse_src_inv             : 1 ;  //    29     
} H15A0_CCO_CTRL_SOSD_PULSE_T;

/*-----------------------------------------------------------------------------
		0xc8024038L ctrl_auto_init_afbc ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_afbc0_init_en                  : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_afbc0_auto_init_en             : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 reg_afbc0_auto_init_src            : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 idx_afbc0_pic_init_src             : 3 ;  // 14:12     
	UINT32 reg_afbc0_partial_ack_en           : 1 ;  //    15     
	UINT32 reg_afbc1_init_en                  : 1 ;  //    16     
	UINT32                                    : 3 ;  // 19:17     reserved
	UINT32 reg_afbc1_auto_init_en             : 1 ;  //    20     
	UINT32                                    : 3 ;  // 23:21     reserved
	UINT32 reg_afbc1_auto_init_src            : 1 ;  //    24     
	UINT32                                    : 3 ;  // 27:25     reserved
	UINT32 idx_afbc1_pic_init_src             : 3 ;  // 30:28     
	UINT32 reg_afbc1_partial_ack_en           : 1 ;  //    31     
} H15A0_CCO_CTRL_AUTO_INIT_AFBC_T;

/*-----------------------------------------------------------------------------
		0xc802403cL ctrl_auto_init_lencon ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_lencon0_init_en                : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_lencon0_auto_init_en           : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 reg_lencon0_auto_init_src          : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 idx_lencon0_pic_init_src           : 3 ;  // 14:12     
	UINT32 sel_lencon0_pic_init_src           : 1 ;  //    15     
	UINT32 reg_lencon1_init_en                : 1 ;  //    16     
	UINT32                                    : 3 ;  // 19:17     reserved
	UINT32 reg_lencon1_auto_init_en           : 1 ;  //    20     
	UINT32                                    : 3 ;  // 23:21     reserved
	UINT32 reg_lencon1_auto_init_src          : 1 ;  //    24     
	UINT32                                    : 3 ;  // 27:25     reserved
	UINT32 idx_lencon1_pic_init_src           : 3 ;  // 30:28     
	UINT32 sel_lencon1_pic_init_src           : 1 ;  //    31     
} H15A0_CCO_CTRL_AUTO_INIT_LENCON_T;

/*-----------------------------------------------------------------------------
		0xc8024044L ctrl_osd_init_2nd ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd0_init_2nd_en                   : 1 ;  //     0     
	UINT32 osd0_init_2nd_sel                  : 1 ;  //     1     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 osd1_init_2nd_en                   : 1 ;  //     4     
	UINT32 osd1_init_2nd_sel                  : 1 ;  //     5     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 osd2_init_2nd_en                   : 1 ;  //     8     
	UINT32 osd2_init_2nd_sel                  : 1 ;  //     9     
	UINT32                                    : 2 ;  // 11:10     reserved
	UINT32 osd3_init_2nd_en                   : 1 ;  //    12     
	UINT32 osd3_init_2nd_sel                  : 1 ;  //    13     
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 osd4_init_2nd_en                   : 1 ;  //    16     
	UINT32 osd4_init_2nd_sel                  : 1 ;  //    17     
	UINT32                                    : 2 ;  // 19:18     reserved
	UINT32 osd5_init_2nd_en                   : 1 ;  //    20     
	UINT32 osd5_init_2nd_sel                  : 1 ;  //    21     
	UINT32                                    : 2 ;  // 23:22     reserved
	UINT32 osd6_init_2nd_en                   : 1 ;  //    24     
	UINT32 osd6_init_2nd_sel                  : 1 ;  //    25     
	UINT32                                    : 2 ;  // 27:26     reserved
	UINT32 osd7_init_2nd_en                   : 1 ;  //    28     
	UINT32 osd7_init_2nd_sel                  : 1 ;  //    29     
} H15A0_CCO_CTRL_OSD_INIT_2ND_T;

/*-----------------------------------------------------------------------------
		0xc8024060L ctrl_fbdc0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_osd0_1x2_bypass_en             : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_osd0_1x2_sel_id                : 4 ;  //  7: 4     
	UINT32 reg_fbdc0_ctrl_path                : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 reg_fbdc0_ctrl_comp                : 4 ;  // 15:12     
	UINT32 reg_fbdc0_ctrl_alpha               : 4 ;  // 19:16     
	UINT32                                    : 4 ;  // 23:20     reserved
	UINT32 reg_fbdc0_ctrl_dbg_sel             : 6 ;  // 29:24     
} H15A0_CCO_CTRL_FBDC0_T;

/*-----------------------------------------------------------------------------
		0xc8024064L info_fbdc0_dbg ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 24;  // 23: 0     reserved
	UINT32 info_fbdc0_dbg                     : 6 ;  // 29:24     
} H15A0_CCO_INFO_FBDC0_DBG_T;

/*-----------------------------------------------------------------------------
		0xc8024068L ctrl_fbdc1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_osd1_1x2_bypass_en             : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_osd1_1x2_sel_id                : 4 ;  //  7: 4     
	UINT32 reg_fbdc1_ctrl_path                : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 reg_fbdc1_ctrl_comp                : 4 ;  // 15:12     
	UINT32 reg_fbdc1_ctrl_alpha               : 4 ;  // 19:16     
	UINT32                                    : 4 ;  // 23:20     reserved
	UINT32 reg_fbdc1_ctrl_dbg_sel             : 6 ;  // 29:24     
} H15A0_CCO_CTRL_FBDC1_T;

/*-----------------------------------------------------------------------------
		0xc802406cL info_fbdc1_dbg ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 24;  // 23: 0     reserved
	UINT32 info_fbdc1_dbg                     : 6 ;  // 29:24     
} H15A0_CCO_INFO_FBDC1_DBG_T;

/*-----------------------------------------------------------------------------
		0xc8024070L ctrl_dly_disp_sync_param0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vsync                              : 16;  // 15: 0     
	UINT32 vfp                                : 16;  // 31:16     
} H15A0_CCO_CTRL_DLY_DISP_SYNC_PARAM0_T;

/*-----------------------------------------------------------------------------
		0xc8024074L ctrl_dly_disp_sync_param1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 va                                 : 16;  // 15: 0     
	UINT32 vbp                                : 16;  // 31:16     
} H15A0_CCO_CTRL_DLY_DISP_SYNC_PARAM1_T;

/*-----------------------------------------------------------------------------
		0xc8024078L ctrl_dly_disp_sync_param2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hsync                              : 16;  // 15: 0     
	UINT32 hfp                                : 16;  // 31:16     
} H15A0_CCO_CTRL_DLY_DISP_SYNC_PARAM2_T;

/*-----------------------------------------------------------------------------
		0xc802407cL ctrl_dly_disp_sync_param3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ha                                 : 16;  // 15: 0     
	UINT32 hbp                                : 16;  // 31:16     
} H15A0_CCO_CTRL_DLY_DISP_SYNC_PARAM3_T;

/*-----------------------------------------------------------------------------
		0xc8024080L ctrl_dly_disp_sync_param4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 line_start                         : 16;  // 15: 0     
	UINT32 sel_in_h_src                       : 1 ;  //    16     
	UINT32 sel_in_v_src                       : 1 ;  //    17     
	UINT32 sel_in_h_inv                       : 1 ;  //    18     
	UINT32 sel_in_v_inv                       : 1 ;  //    19     
	UINT32 sel_out_ha                         : 1 ;  //    20     
	UINT32 sel_out_va                         : 1 ;  //    21     
	UINT32 sel_out_hs                         : 1 ;  //    22     
	UINT32 sel_out_vs                         : 1 ;  //    23     
	UINT32 tp_update_hv_zero                  : 1 ;  //    24     
	UINT32                                    : 3 ;  // 27:25     reserved
	UINT32 en_cnt_reset_h_value               : 1 ;  //    28     
	UINT32 en_cnt_reset_v_value               : 1 ;  //    29     
	UINT32                                    : 1 ;  //    30     reserved
	UINT32 force_update                       : 1 ;  //    31     
} H15A0_CCO_CTRL_DLY_DISP_SYNC_PARAM4_T;

/*-----------------------------------------------------------------------------
		0xc8024084L ctrl_dly_disp_sync_param5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 h_dly                              : 16;  // 15: 0     
	UINT32 v_dly                              : 16;  // 31:16     
} H15A0_CCO_CTRL_DLY_DISP_SYNC_PARAM5_T;

/*-----------------------------------------------------------------------------
		0xc8024088L ctrl_dly_disp_sync_param6 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reset_h_value                      : 16;  // 15: 0     
	UINT32 reset_v_value                      : 16;  // 31:16     
} H15A0_CCO_CTRL_DLY_DISP_SYNC_PARAM6_T;

/*-----------------------------------------------------------------------------
		0xc802408cL ctrl_dly_disp_sync_param7 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 auto_delay_ha                      : 16;  // 15: 0     
	UINT32 auto_delay_va                      : 16;  // 31:16     
} H15A0_CCO_CTRL_DLY_DISP_SYNC_PARAM7_T;

/*-----------------------------------------------------------------------------
		0xc8024090L ctrl_dly_disp_sync_param8 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 auto_delay_hs                      : 16;  // 15: 0     
	UINT32 auto_delay_vs                      : 16;  // 31:16     
} H15A0_CCO_CTRL_DLY_DISP_SYNC_PARAM8_T;

/*-----------------------------------------------------------------------------
		0xc8024094L ctrl_dly_disp_sync_param9 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 line_cnt_threshold                 : 16;  // 15: 0     
} H15A0_CCO_CTRL_DLY_DISP_SYNC_PARAM9_T;

/*-----------------------------------------------------------------------------
		0xc8024098L ctrl_dly_disp_sync_parama ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 auto_delay_hs                      : 16;  // 15: 0     
	UINT32 auto_delay_vs                      : 16;  // 31:16     
} H15A0_CCO_CTRL_DLY_DISP_SYNC_PARAMA_T;

/*-----------------------------------------------------------------------------
		0xc802409cL ctrl_dly_disp_sync_paramb ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 line_cnt_threshold                 : 16;  // 15: 0     
} H15A0_CCO_CTRL_DLY_DISP_SYNC_PARAMB_T;

/*-----------------------------------------------------------------------------
		0xc80240a0L osd_sync_gen_ctrl0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vsync                              : 16;  // 15: 0     
	UINT32 vfp                                : 16;  // 31:16     
} H15A0_CCO_OSD_SYNC_GEN_CTRL0_T;

/*-----------------------------------------------------------------------------
		0xc80240a4L osd_sync_gen_ctrl1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 va                                 : 16;  // 15: 0     
	UINT32 vbp                                : 16;  // 31:16     
} H15A0_CCO_OSD_SYNC_GEN_CTRL1_T;

/*-----------------------------------------------------------------------------
		0xc80240a8L osd_sync_gen_ctrl2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hsync                              : 16;  // 15: 0     
	UINT32 hfp                                : 16;  // 31:16     
} H15A0_CCO_OSD_SYNC_GEN_CTRL2_T;

/*-----------------------------------------------------------------------------
		0xc80240acL osd_sync_gen_ctrl3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ha                                 : 16;  // 15: 0     
	UINT32 hbp                                : 16;  // 31:16     
} H15A0_CCO_OSD_SYNC_GEN_CTRL3_T;

/*-----------------------------------------------------------------------------
		0xc80240b0L osd_sync_gen_ctrl4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 prev_vsync                         : 16;  // 15: 0     
	UINT32 prev_vfp                           : 16;  // 31:16     
} H15A0_CCO_OSD_SYNC_GEN_CTRL4_T;

/*-----------------------------------------------------------------------------
		0xc80240b4L osd_sync_gen_ctrl5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 prev_va                            : 16;  // 15: 0     
	UINT32 prev_vbp                           : 16;  // 31:16     
} H15A0_CCO_OSD_SYNC_GEN_CTRL5_T;

/*-----------------------------------------------------------------------------
		0xc80240b8L osd_sync_gen_ctrl6 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 line_start                         : 16;  // 15: 0     
	UINT32 sel_in_h_src                       : 1 ;  //    16     
	UINT32 sel_in_v_src                       : 1 ;  //    17     
	UINT32 sel_in_h_inv                       : 1 ;  //    18     
	UINT32 sel_in_v_inv                       : 1 ;  //    19     
	UINT32 sel_out_ha                         : 1 ;  //    20     
	UINT32 sel_out_va                         : 1 ;  //    21     
	UINT32 sel_out_hs                         : 1 ;  //    22     
	UINT32 sel_out_vs                         : 1 ;  //    23     
	UINT32 tp_update_hv_zero                  : 1 ;  //    24     
	UINT32                                    : 3 ;  // 27:25     reserved
	UINT32 cnt_reset_h_value_en               : 1 ;  //    28     
	UINT32 cnt_reset_v_value_en               : 1 ;  //    29     
	UINT32                                    : 1 ;  //    30     reserved
	UINT32 force_update                       : 1 ;  //    31     
} H15A0_CCO_OSD_SYNC_GEN_CTRL6_T;

/*-----------------------------------------------------------------------------
		0xc80240bcL osd_sync_gen_ctrl7 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 manual_h                           : 16;  // 15: 0     
	UINT32 manual_v                           : 16;  // 31:16     
} H15A0_CCO_OSD_SYNC_GEN_CTRL7_T;

/*-----------------------------------------------------------------------------
		0xc80240c0L pic_init_g0_timer ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 start_interval_mode_en             : 1 ;  //     0     
	UINT32 start_interval_mode                : 3 ;  //  3: 1     
	UINT32 g0_pic_init_timer                  : 28;  // 31: 4     
} H15A0_CCO_PIC_INIT_G0_TIMER_T;

/*-----------------------------------------------------------------------------
		0xc80240c4L pic_init_g1_timer ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 start_interval_mode_en             : 1 ;  //     0     
	UINT32 start_interval_mode                : 3 ;  //  3: 1     
	UINT32 g1_pic_init_timer                  : 28;  // 31: 4     
} H15A0_CCO_PIC_INIT_G1_TIMER_T;

/*-----------------------------------------------------------------------------
		0xc80240c8L pic_init_g2_timer ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 start_interval_mode_en             : 1 ;  //     0     
	UINT32 start_interval_mode                : 3 ;  //  3: 1     
	UINT32 g2_pic_init_timer                  : 28;  // 31: 4     
} H15A0_CCO_PIC_INIT_G2_TIMER_T;

/*-----------------------------------------------------------------------------
		0xc80240ccL pic_init_g3_timer ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 start_interval_mode_en             : 1 ;  //     0     
	UINT32 start_interval_mode                : 3 ;  //  3: 1     
	UINT32 g3_pic_init_timer                  : 28;  // 31: 4     
} H15A0_CCO_PIC_INIT_G3_TIMER_T;

/*-----------------------------------------------------------------------------
		0xc80240d0L pic_init_g4_timer ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 start_interval_mode_en             : 1 ;  //     0     
	UINT32 start_interval_mode                : 3 ;  //  3: 1     
	UINT32 g4_pic_init_timer                  : 28;  // 31: 4     
} H15A0_CCO_PIC_INIT_G4_TIMER_T;

/*-----------------------------------------------------------------------------
		0xc80240d4L pic_init_g5_timer ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 start_interval_mode_en             : 1 ;  //     0     
	UINT32 start_interval_mode                : 3 ;  //  3: 1     
	UINT32 g5_pic_init_timer                  : 28;  // 31: 4     
} H15A0_CCO_PIC_INIT_G5_TIMER_T;

/*-----------------------------------------------------------------------------
		0xc80240d8L pic_init_g6_timer ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 start_interval_mode_en             : 1 ;  //     0     
	UINT32 start_interval_mode                : 3 ;  //  3: 1     
	UINT32 g6_pic_init_timer                  : 28;  // 31: 4     
} H15A0_CCO_PIC_INIT_G6_TIMER_T;

/*-----------------------------------------------------------------------------
		0xc80240dcL pic_init_g7_timer ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 start_interval_mode_en             : 1 ;  //     0     
	UINT32 start_interval_mode                : 3 ;  //  3: 1     
	UINT32 g7_pic_init_timer                  : 28;  // 31: 4     
} H15A0_CCO_PIC_INIT_G7_TIMER_T;

/*-----------------------------------------------------------------------------
		0xc80240e4L dbg_timer_run ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 info_timer_run                     : 24;  // 23: 0     
	UINT32 ctrl_timer_run                     : 8 ;  // 31:24     
} H15A0_CCO_DBG_TIMER_RUN_T;

/*-----------------------------------------------------------------------------
		0xc80240e8L dbg_timer_blk ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 info_timer_blk                     : 24;  // 23: 0     
	UINT32 ctrl_timer_blk                     : 8 ;  // 31:24     
} H15A0_CCO_DBG_TIMER_BLK_T;

/*-----------------------------------------------------------------------------
		0xc80240ecL ctrl_monitor ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ctrl_monitor                       : 32;  // 31: 0     
} H15A0_CCO_CTRL_MONITOR_T;

/*-----------------------------------------------------------------------------
		0xc80240f0L ctrl_pic_end ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 b0_pic_end_en                      : 1 ;  //     0     
	UINT32 b1_pic_end_en                      : 1 ;  //     1     
	UINT32 b2_pic_end_en                      : 1 ;  //     2     
	UINT32 b3_pic_end_en                      : 1 ;  //     3     
	UINT32 b4_pic_end_en                      : 1 ;  //     4     
	UINT32 b5_pic_end_en                      : 1 ;  //     5     
	UINT32 b6_pic_end_en                      : 1 ;  //     6     
	UINT32 b7_pic_end_en                      : 1 ;  //     7     
	UINT32 b8_pic_end_en                      : 1 ;  //     8     
	UINT32 b9_pic_end_en                      : 1 ;  //     9     
	UINT32 b10_pic_end_en                     : 1 ;  //    10     
	UINT32 b11_pic_end_en                     : 1 ;  //    11     
	UINT32 b12_pic_end_en                     : 1 ;  //    12     
	UINT32 b13_pic_end_en                     : 1 ;  //    13     
	UINT32 b14_pic_end_en                     : 1 ;  //    14     
	UINT32 b15_pic_end_en                     : 1 ;  //    15     
	UINT32 pic_end_g0_force_en                : 1 ;  //    16     
	UINT32 pic_end_g1_force_en                : 1 ;  //    17     
	UINT32 pic_end_g2_force_en                : 1 ;  //    18     
	UINT32 pic_end_g3_force_en                : 1 ;  //    19     
	UINT32 pic_end_g4_force_en                : 1 ;  //    20     
	UINT32 pic_end_g5_force_en                : 1 ;  //    21     
	UINT32 pic_end_g6_force_en                : 1 ;  //    22     
	UINT32 pic_end_g7_force_en                : 1 ;  //    23     
	UINT32 pic_end_g0_force_value             : 1 ;  //    24     
	UINT32 pic_end_g1_force_value             : 1 ;  //    25     
	UINT32 pic_end_g2_force_value             : 1 ;  //    26     
	UINT32 pic_end_g3_force_value             : 1 ;  //    27     
	UINT32 pic_end_g4_force_value             : 1 ;  //    28     
	UINT32 pic_end_g5_force_value             : 1 ;  //    29     
	UINT32 pic_end_g6_force_value             : 1 ;  //    30     
	UINT32 pic_end_g7_force_value             : 1 ;  //    31     
} H15A0_CCO_CTRL_PIC_END_T;

/*-----------------------------------------------------------------------------
		0xc80240f4L info_pic_end ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 g0_pic_end                         : 1 ;  //     0     
	UINT32 g1_pic_end                         : 1 ;  //     1     
	UINT32 g2_pic_end                         : 1 ;  //     2     
	UINT32 g3_pic_end                         : 1 ;  //     3     
	UINT32 g4_pic_end                         : 1 ;  //     4     
	UINT32 g5_pic_end                         : 1 ;  //     5     
	UINT32 g6_pic_end                         : 1 ;  //     6     
	UINT32 g7_pic_end                         : 1 ;  //     7     
	UINT32                                    : 8 ;  // 15: 8     reserved
	UINT32 g0_pic_turn                        : 1 ;  //    16     
	UINT32 g1_pic_turn                        : 1 ;  //    17     
	UINT32 g2_pic_turn                        : 1 ;  //    18     
	UINT32 g3_pic_turn                        : 1 ;  //    19     
	UINT32 g4_pic_turn                        : 1 ;  //    20     
	UINT32 g5_pic_turn                        : 1 ;  //    21     
	UINT32 g6_pic_turn                        : 1 ;  //    22     
	UINT32 g7_pic_turn                        : 1 ;  //    23     
} H15A0_CCO_INFO_PIC_END_T;

/*-----------------------------------------------------------------------------
		0xc80240f8L ctrl_gpio ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ctrl_gpio                      : 32;  // 31: 0     
} H15A0_CCO_CTRL_GPIO_T;

/*-----------------------------------------------------------------------------
		0xc80240fcL ctrl_gmau ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ask_cmd_dly_cnt                    : 8 ;  //  7: 0     
	UINT32 smx_aruser                         : 4 ;  // 11: 8     
	UINT32 smx_awuser                         : 4 ;  // 15:12     
	UINT32 ask_to_cnt                         : 10;  // 25:16     
} H15A0_CCO_CTRL_GMAU_T;

/*-----------------------------------------------------------------------------
		0xc8001000L pic_init ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 g0_pic_init                        : 1 ;  //     0     
	UINT32 g1_pic_init                        : 1 ;  //     1     
	UINT32 g2_pic_init                        : 1 ;  //     2     
	UINT32 g3_pic_init                        : 1 ;  //     3     
	UINT32 g4_pic_init                        : 1 ;  //     4     
	UINT32 g5_pic_init                        : 1 ;  //     5     
	UINT32 g6_pic_init                        : 1 ;  //     6     
	UINT32 g7_pic_init                        : 1 ;  //     7     
	UINT32                                    : 8 ;  // 15: 8     reserved
	UINT32 g0_auto_init                       : 1 ;  //    16     
	UINT32 g1_auto_init                       : 1 ;  //    17     
	UINT32 g2_auto_init                       : 1 ;  //    18     
	UINT32 g3_auto_init                       : 1 ;  //    19     
	UINT32 g4_auto_init                       : 1 ;  //    20     
	UINT32 g5_auto_init                       : 1 ;  //    21     
	UINT32 g6_auto_init                       : 1 ;  //    22     
	UINT32 g7_auto_init                       : 1 ;  //    23     
} H15A0_IMX_PIC_INIT_T;

/*-----------------------------------------------------------------------------
		0xc8001004L pic_start ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 g0_pic_start                       : 1 ;  //     0     
	UINT32 g1_pic_start                       : 1 ;  //     1     
	UINT32 g2_pic_start                       : 1 ;  //     2     
	UINT32 g3_pic_start                       : 1 ;  //     3     
	UINT32 g4_pic_start                       : 1 ;  //     4     
	UINT32 g5_pic_start                       : 1 ;  //     5     
	UINT32 g6_pic_start                       : 1 ;  //     6     
	UINT32 g7_pic_start                       : 1 ;  //     7     
} H15A0_IMX_PIC_START_T;

/*-----------------------------------------------------------------------------
		0xc8001008L ctrl_auto_init ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 idx_src_g0_auto_init               : 3 ;  //  2: 0     
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 idx_src_g1_auto_init               : 3 ;  //  6: 4     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 idx_src_g2_auto_init               : 3 ;  // 10: 8     
	UINT32                                    : 1 ;  //    11     reserved
	UINT32 idx_src_g3_auto_init               : 3 ;  // 14:12     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 idx_src_g4_auto_init               : 3 ;  // 18:16     
	UINT32                                    : 1 ;  //    19     reserved
	UINT32 idx_src_g5_auto_init               : 3 ;  // 22:20     
	UINT32                                    : 1 ;  //    23     reserved
	UINT32 idx_src_g6_auto_init               : 3 ;  // 26:24     
	UINT32                                    : 1 ;  //    27     reserved
	UINT32 idx_src_g7_auto_init               : 3 ;  // 30:28     
} H15A0_IMX_CTRL_AUTO_INIT_T;

/*-----------------------------------------------------------------------------
		0xc800100cL ctrl_intr_pulse ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 idx_src_i0_intr_pulse              : 3 ;  //  2: 0     
	UINT32 sel_src_i0_intr_pulse              : 1 ;  //     3     
	UINT32 idx_src_i1_intr_pulse              : 3 ;  //  6: 4     
	UINT32 sel_src_i1_intr_pulse              : 1 ;  //     7     
	UINT32 idx_src_i2_intr_pulse              : 3 ;  // 10: 8     
	UINT32 sel_src_i2_intr_pulse              : 1 ;  //    11     
	UINT32 idx_src_i3_intr_pulse              : 3 ;  // 14:12     
	UINT32 sel_src_i3_intr_pulse              : 1 ;  //    15     
	UINT32 idx_src_i4_intr_pulse              : 3 ;  // 18:16     
	UINT32 sel_src_i4_intr_pulse              : 1 ;  //    19     
	UINT32 idx_src_i5_intr_pulse              : 3 ;  // 22:20     
	UINT32 sel_src_i5_intr_pulse              : 1 ;  //    23     
	UINT32 idx_src_i6_intr_pulse              : 3 ;  // 26:24     
	UINT32 sel_src_i6_intr_pulse              : 1 ;  //    27     
	UINT32 idx_src_i7_intr_pulse              : 3 ;  // 30:28     
	UINT32 sel_src_i7_intr_pulse              : 1 ;  //    31     
} H15A0_IMX_CTRL_INTR_PULSE_T;

/*-----------------------------------------------------------------------------
		0xc8001010L intr_reg ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 intr_i0_intr_pulse                 : 1 ;  //     0     
	UINT32 intr_i1_intr_pulse                 : 1 ;  //     1     
	UINT32 intr_i2_intr_pulse                 : 1 ;  //     2     
	UINT32 intr_i3_intr_pulse                 : 1 ;  //     3     
	UINT32 intr_i4_intr_pulse                 : 1 ;  //     4     
	UINT32 intr_i5_intr_pulse                 : 1 ;  //     5     
	UINT32 intr_i6_intr_pulse                 : 1 ;  //     6     
	UINT32 intr_i7_intr_pulse                 : 1 ;  //     7     
	UINT32 intr_g0_pic_end                    : 1 ;  //     8     
	UINT32 intr_g1_pic_end                    : 1 ;  //     9     
	UINT32 intr_g2_pic_end                    : 1 ;  //    10     
	UINT32 intr_g3_pic_end                    : 1 ;  //    11     
	UINT32 intr_g4_pic_end                    : 1 ;  //    12     
	UINT32 intr_g5_pic_end                    : 1 ;  //    13     
	UINT32 intr_g6_pic_end                    : 1 ;  //    14     
	UINT32 intr_g7_pic_end                    : 1 ;  //    15     
	UINT32 intr_vdo                           : 1 ;  //    16     
	UINT32 intr_fd3                           : 1 ;  //    17     
} H15A0_IMX_INTR_REG_T;

/*-----------------------------------------------------------------------------
		0xc8001014L intr_mask ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mask_i0_src_pulse                  : 1 ;  //     0     
	UINT32 mask_i1_src_pulse                  : 1 ;  //     1     
	UINT32 mask_i2_src_pulse                  : 1 ;  //     2     
	UINT32 mask_i3_src_pulse                  : 1 ;  //     3     
	UINT32 mask_i4_src_pulse                  : 1 ;  //     4     
	UINT32 mask_i5_src_pulse                  : 1 ;  //     5     
	UINT32 mask_i6_src_pulse                  : 1 ;  //     6     
	UINT32 mask_i7_src_pulse                  : 1 ;  //     7     
	UINT32 mask_g0_pic_end                    : 1 ;  //     8     
	UINT32 mask_g1_pic_end                    : 1 ;  //     9     
	UINT32 mask_g2_pic_end                    : 1 ;  //    10     
	UINT32 mask_g3_pic_end                    : 1 ;  //    11     
	UINT32 mask_g4_pic_end                    : 1 ;  //    12     
	UINT32 mask_g5_pic_end                    : 1 ;  //    13     
	UINT32 mask_g6_pic_end                    : 1 ;  //    14     
	UINT32 mask_g7_pic_end                    : 1 ;  //    15     
	UINT32 mask_vdo                           : 1 ;  //    16     
	UINT32 mask_fd3                           : 1 ;  //    17     
} H15A0_IMX_INTR_MASK_T;

/*-----------------------------------------------------------------------------
		0xc8001018L intr_mux ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mux_i0_src_pulse                   : 1 ;  //     0     
	UINT32 mux_i1_src_pulse                   : 1 ;  //     1     
	UINT32 mux_i2_src_pulse                   : 1 ;  //     2     
	UINT32 mux_i3_src_pulse                   : 1 ;  //     3     
	UINT32 mux_i4_src_pulse                   : 1 ;  //     4     
	UINT32 mux_i5_src_pulse                   : 1 ;  //     5     
	UINT32 mux_i6_src_pulse                   : 1 ;  //     6     
	UINT32 mux_i7_src_pulse                   : 1 ;  //     7     
	UINT32 mux_g0_pic_end                     : 1 ;  //     8     
	UINT32 mux_g1_pic_end                     : 1 ;  //     9     
	UINT32 mux_g2_pic_end                     : 1 ;  //    10     
	UINT32 mux_g3_pic_end                     : 1 ;  //    11     
	UINT32 mux_g4_pic_end                     : 1 ;  //    12     
	UINT32 mux_g5_pic_end                     : 1 ;  //    13     
	UINT32 mux_g6_pic_end                     : 1 ;  //    14     
	UINT32 mux_g7_pic_end                     : 1 ;  //    15     
	UINT32 mux_vdo                            : 1 ;  //    16     
	UINT32 mux_fd3                            : 1 ;  //    17     
} H15A0_IMX_INTR_MUX_T;

/*-----------------------------------------------------------------------------
		0xc800101cL intr_en ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 en_i0_src_pulse                    : 1 ;  //     0     
	UINT32 en_i1_src_pulse                    : 1 ;  //     1     
	UINT32 en_i2_src_pulse                    : 1 ;  //     2     
	UINT32 en_i3_src_pulse                    : 1 ;  //     3     
	UINT32 en_i4_src_pulse                    : 1 ;  //     4     
	UINT32 en_i5_src_pulse                    : 1 ;  //     5     
	UINT32 en_i6_src_pulse                    : 1 ;  //     6     
	UINT32 en_i7_src_pulse                    : 1 ;  //     7     
	UINT32 en_g0_pic_end                      : 1 ;  //     8     
	UINT32 en_g1_pic_end                      : 1 ;  //     9     
	UINT32 en_g2_pic_end                      : 1 ;  //    10     
	UINT32 en_g3_pic_end                      : 1 ;  //    11     
	UINT32 en_g4_pic_end                      : 1 ;  //    12     
	UINT32 en_g5_pic_end                      : 1 ;  //    13     
	UINT32 en_g6_pic_end                      : 1 ;  //    14     
	UINT32 en_g7_pic_end                      : 1 ;  //    15     
	UINT32 en_vdo                             : 1 ;  //    16     
	UINT32 en_fd3                             : 1 ;  //    17     
} H15A0_IMX_INTR_EN_T;

/*-----------------------------------------------------------------------------
		0xc8001020L s0_src_sync_mux_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sel_sync_src                       : 2 ;  //  1: 0     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 idx_sync_src                       : 2 ;  //  5: 4     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 sel_sync_inv_vs                    : 1 ;  //     8     
	UINT32 sel_sync_inv_hs                    : 1 ;  //     9     
	UINT32 sel_sync_vs                        : 1 ;  //    10     
	UINT32 sel_sync_hs                        : 1 ;  //    11     
	UINT32 en_auto_src_pulse_half_va          : 1 ;  //    12     
	UINT32 src_fall_pos_sel                   : 3 ;  // 15:13     
	UINT32 src_pulse_pos_sel                  : 8 ;  // 23:16     
	UINT32                                    : 4 ;  // 27:24     reserved
	UINT32 en_manual_delay_of_src_pulse       : 1 ;  //    28     
	UINT32 en_line_delay_of_src_pulse         : 1 ;  //    29     
	UINT32 en_trans_mask                      : 1 ;  //    30     
	UINT32 ignore_src_vs_fal                  : 1 ;  //    31     
} H15A0_IMX_S0_SRC_SYNC_MUX_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc8001024L s0_src_sync_dly ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 src_sync_dly                       : 28;  // 27: 0     
} H15A0_IMX_S0_SRC_SYNC_DLY_T;

/*-----------------------------------------------------------------------------
		0xc8001028L s0_src_sync_dly_line ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 src_sync_dly_line                  : 16;  // 15: 0     
	UINT32                                    : 12;  // 27:16     reserved
	UINT32 cnt_src_pulse                      : 4 ;  // 31:28     
} H15A0_IMX_S0_SRC_SYNC_DLY_LINE_T;

/*-----------------------------------------------------------------------------
		0xc800102cL s1_src_sync_mux_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sel_sync_src                       : 2 ;  //  1: 0     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 idx_sync_src                       : 2 ;  //  5: 4     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 sel_sync_inv                       : 1 ;  //     8     
	UINT32 sel_sync_vs                        : 1 ;  //     9     
	UINT32                                    : 2 ;  // 11:10     reserved
	UINT32 en_auto_src_pulse_half_va          : 1 ;  //    12     
	UINT32 src_fall_pos_sel                   : 3 ;  // 15:13     
	UINT32 src_pulse_pos_sel                  : 8 ;  // 23:16     
	UINT32                                    : 4 ;  // 27:24     reserved
	UINT32 en_manual_delay_of_src_pulse       : 1 ;  //    28     
	UINT32 en_line_delay_of_src_pulse         : 1 ;  //    29     
	UINT32 en_trans_mask                      : 1 ;  //    30     
	UINT32 ignore_src_vs_fal                  : 1 ;  //    31     
} H15A0_IMX_S1_SRC_SYNC_MUX_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc8001030L s1_src_sync_dly ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 src_sync_dly                       : 28;  // 27: 0     
} H15A0_IMX_S1_SRC_SYNC_DLY_T;

/*-----------------------------------------------------------------------------
		0xc8001034L s1_src_sync_dly_line ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 src_sync_dly_line                  : 16;  // 15: 0     
	UINT32                                    : 12;  // 27:16     reserved
	UINT32 cnt_src_pulse                      : 4 ;  // 31:28     
} H15A0_IMX_S1_SRC_SYNC_DLY_LINE_T;

/*-----------------------------------------------------------------------------
		0xc8001038L s2_src_sync_mux_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sel_sync_src                       : 2 ;  //  1: 0     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 idx_sync_src                       : 2 ;  //  5: 4     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 sel_sync_inv                       : 1 ;  //     8     
	UINT32 sel_sync_vs                        : 1 ;  //     9     
	UINT32                                    : 2 ;  // 11:10     reserved
	UINT32 en_auto_src_pulse_half_va          : 1 ;  //    12     
	UINT32 src_fall_pos_sel                   : 3 ;  // 15:13     
	UINT32 src_pulse_pos_sel                  : 8 ;  // 23:16     
	UINT32                                    : 4 ;  // 27:24     reserved
	UINT32 en_manual_delay_of_src_pulse       : 1 ;  //    28     
	UINT32 en_line_delay_of_src_pulse         : 1 ;  //    29     
	UINT32 en_trans_mask                      : 1 ;  //    30     
	UINT32 ignore_src_vs_fal                  : 1 ;  //    31     
} H15A0_IMX_S2_SRC_SYNC_MUX_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc800103cL s2_src_sync_dly ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 src_sync_dly                       : 28;  // 27: 0     
} H15A0_IMX_S2_SRC_SYNC_DLY_T;

/*-----------------------------------------------------------------------------
		0xc8001040L s2_src_sync_dly_line ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 src_sync_dly_line                  : 16;  // 15: 0     
	UINT32                                    : 12;  // 27:16     reserved
	UINT32 cnt_src_pulse                      : 4 ;  // 31:28     
} H15A0_IMX_S2_SRC_SYNC_DLY_LINE_T;

/*-----------------------------------------------------------------------------
		0xc8001044L s3_src_sync_mux_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sel_sync_src                       : 2 ;  //  1: 0     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 idx_sync_src                       : 2 ;  //  5: 4     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 sel_sync_inv                       : 1 ;  //     8     
	UINT32 sel_sync_vs                        : 1 ;  //     9     
	UINT32                                    : 2 ;  // 11:10     reserved
	UINT32 en_auto_src_pulse_half_va          : 1 ;  //    12     
	UINT32 src_fall_pos_sel                   : 3 ;  // 15:13     
	UINT32 src_pulse_pos_sel                  : 8 ;  // 23:16     
	UINT32                                    : 4 ;  // 27:24     reserved
	UINT32 en_manual_delay_of_src_pulse       : 1 ;  //    28     
	UINT32 en_line_delay_of_src_pulse         : 1 ;  //    29     
	UINT32 en_trans_mask                      : 1 ;  //    30     
	UINT32 ignore_src_vs_fal                  : 1 ;  //    31     
} H15A0_IMX_S3_SRC_SYNC_MUX_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc8001048L s3_src_sync_dly ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 src_sync_dly                       : 28;  // 27: 0     
} H15A0_IMX_S3_SRC_SYNC_DLY_T;

/*-----------------------------------------------------------------------------
		0xc800104cL s3_src_sync_dly_line ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 src_sync_dly_line                  : 16;  // 15: 0     
	UINT32                                    : 12;  // 27:16     reserved
	UINT32 cnt_src_pulse                      : 4 ;  // 31:28     
} H15A0_IMX_S3_SRC_SYNC_DLY_LINE_T;

/*-----------------------------------------------------------------------------
		0xc8001050L s4_src_sync_mux_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sel_sync_src                       : 2 ;  //  1: 0     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 idx_sync_src                       : 2 ;  //  5: 4     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 sel_sync_inv                       : 1 ;  //     8     
	UINT32 sel_sync_vs                        : 1 ;  //     9     
	UINT32                                    : 2 ;  // 11:10     reserved
	UINT32 en_auto_src_pulse_half_va          : 1 ;  //    12     
	UINT32 src_fall_pos_sel                   : 3 ;  // 15:13     
	UINT32 src_pulse_pos_sel                  : 8 ;  // 23:16     
	UINT32                                    : 4 ;  // 27:24     reserved
	UINT32 en_manual_delay_of_src_pulse       : 1 ;  //    28     
	UINT32 en_line_delay_of_src_pulse         : 1 ;  //    29     
	UINT32 en_trans_mask                      : 1 ;  //    30     
	UINT32 ignore_src_vs_fal                  : 1 ;  //    31     
} H15A0_IMX_S4_SRC_SYNC_MUX_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc8001054L s4_src_sync_dly ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 src_sync_dly                       : 28;  // 27: 0     
} H15A0_IMX_S4_SRC_SYNC_DLY_T;

/*-----------------------------------------------------------------------------
		0xc8001058L s4_src_sync_dly_line ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 src_sync_dly_line                  : 16;  // 15: 0     
	UINT32                                    : 12;  // 27:16     reserved
	UINT32 cnt_src_pulse                      : 4 ;  // 31:28     
} H15A0_IMX_S4_SRC_SYNC_DLY_LINE_T;

/*-----------------------------------------------------------------------------
		0xc800105cL s5_src_sync_mux_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sel_sync_src                       : 2 ;  //  1: 0     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 idx_sync_src                       : 2 ;  //  5: 4     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 sel_sync_inv                       : 1 ;  //     8     
	UINT32 sel_sync_vs                        : 1 ;  //     9     
	UINT32                                    : 2 ;  // 11:10     reserved
	UINT32 en_auto_src_pulse_half_va          : 1 ;  //    12     
	UINT32 src_fall_pos_sel                   : 3 ;  // 15:13     
	UINT32 src_pulse_pos_sel                  : 8 ;  // 23:16     
	UINT32                                    : 4 ;  // 27:24     reserved
	UINT32 en_manual_delay_of_src_pulse       : 1 ;  //    28     
	UINT32 en_line_delay_of_src_pulse         : 1 ;  //    29     
	UINT32 en_trans_mask                      : 1 ;  //    30     
	UINT32 ignore_src_vs_fal                  : 1 ;  //    31     
} H15A0_IMX_S5_SRC_SYNC_MUX_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc8001060L s5_src_sync_dly ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 src_sync_dly                       : 28;  // 27: 0     
} H15A0_IMX_S5_SRC_SYNC_DLY_T;

/*-----------------------------------------------------------------------------
		0xc8001064L s5_src_sync_dly_line ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 src_sync_dly_line                  : 16;  // 15: 0     
	UINT32                                    : 12;  // 27:16     reserved
	UINT32 cnt_src_pulse                      : 4 ;  // 31:28     
} H15A0_IMX_S5_SRC_SYNC_DLY_LINE_T;

/*-----------------------------------------------------------------------------
		0xc8001068L s6_src_sync_mux_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sel_sync_src                       : 2 ;  //  1: 0     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 idx_sync_src                       : 2 ;  //  5: 4     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 sel_sync_inv                       : 1 ;  //     8     
	UINT32 sel_sync_vs                        : 1 ;  //     9     
	UINT32                                    : 2 ;  // 11:10     reserved
	UINT32 en_auto_src_pulse_half_va          : 1 ;  //    12     
	UINT32 src_fall_pos_sel                   : 3 ;  // 15:13     
	UINT32 src_pulse_pos_sel                  : 8 ;  // 23:16     
	UINT32                                    : 4 ;  // 27:24     reserved
	UINT32 en_manual_delay_of_src_pulse       : 1 ;  //    28     
	UINT32 en_line_delay_of_src_pulse         : 1 ;  //    29     
	UINT32 en_trans_mask                      : 1 ;  //    30     
	UINT32 ignore_src_vs_fal                  : 1 ;  //    31     
} H15A0_IMX_S6_SRC_SYNC_MUX_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc800106cL s6_src_sync_dly ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 src_sync_dly                       : 28;  // 27: 0     
} H15A0_IMX_S6_SRC_SYNC_DLY_T;

/*-----------------------------------------------------------------------------
		0xc8001070L s6_src_sync_dly_line ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 src_sync_dly_line                  : 16;  // 15: 0     
	UINT32                                    : 12;  // 27:16     reserved
	UINT32 cnt_src_pulse                      : 4 ;  // 31:28     
} H15A0_IMX_S6_SRC_SYNC_DLY_LINE_T;

/*-----------------------------------------------------------------------------
		0xc8001074L s7_src_sync_mux_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sel_sync_src                       : 2 ;  //  1: 0     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 idx_sync_src                       : 2 ;  //  5: 4     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 sel_sync_inv_vs                    : 1 ;  //     8     
	UINT32 sel_sync_inv_hs                    : 1 ;  //     9     
	UINT32 sel_sync_vs                        : 1 ;  //    10     
	UINT32 sel_sync_hs                        : 1 ;  //    11     
	UINT32 en_auto_src_pulse_half_va          : 1 ;  //    12     
	UINT32 src_fall_pos_sel                   : 3 ;  // 15:13     
	UINT32 src_pulse_pos_sel                  : 8 ;  // 23:16     
	UINT32                                    : 4 ;  // 27:24     reserved
	UINT32 en_manual_delay_of_src_pulse       : 1 ;  //    28     
	UINT32 en_line_delay_of_src_pulse         : 1 ;  //    29     
	UINT32 en_trans_mask                      : 1 ;  //    30     
	UINT32 ignore_src_vs_fal                  : 1 ;  //    31     
} H15A0_IMX_S7_SRC_SYNC_MUX_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc8001078L s7_src_sync_dly ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 src_sync_dly                       : 28;  // 27: 0     
} H15A0_IMX_S7_SRC_SYNC_DLY_T;

/*-----------------------------------------------------------------------------
		0xc800107cL s7_src_sync_dly_line ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 src_sync_dly_line                  : 16;  // 15: 0     
	UINT32                                    : 12;  // 27:16     reserved
	UINT32 cnt_src_pulse                      : 4 ;  // 31:28     
} H15A0_IMX_S7_SRC_SYNC_DLY_LINE_T;

/*-----------------------------------------------------------------------------
		0xc8001080L mem0_sync_period ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mem_sync_period                    : 32;  // 31: 0     
} H15A0_IMX_MEM0_SYNC_PERIOD_T;

/*-----------------------------------------------------------------------------
		0xc8001084L mem1_sync_period ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mem_sync_period                    : 32;  // 31: 0     
} H15A0_IMX_MEM1_SYNC_PERIOD_T;

/*-----------------------------------------------------------------------------
		0xc8001088L mem2_sync_period ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mem_sync_period                    : 32;  // 31: 0     
} H15A0_IMX_MEM2_SYNC_PERIOD_T;

/*-----------------------------------------------------------------------------
		0xc800108cL mem3_sync_period ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mem_sync_period                    : 32;  // 31: 0     
} H15A0_IMX_MEM3_SYNC_PERIOD_T;

/*-----------------------------------------------------------------------------
		0xc8001090L mem4_sync_period ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mem_sync_period                    : 32;  // 31: 0     
} H15A0_IMX_MEM4_SYNC_PERIOD_T;

/*-----------------------------------------------------------------------------
		0xc8001094L mem5_sync_period ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mem_sync_period                    : 32;  // 31: 0     
} H15A0_IMX_MEM5_SYNC_PERIOD_T;

/*-----------------------------------------------------------------------------
		0xc8001098L mem6_sync_period ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mem_sync_period                    : 32;  // 31: 0     
} H15A0_IMX_MEM6_SYNC_PERIOD_T;

/*-----------------------------------------------------------------------------
		0xc800109cL mem7_sync_period ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mem_sync_period                    : 32;  // 31: 0     
} H15A0_IMX_MEM7_SYNC_PERIOD_T;

/*-----------------------------------------------------------------------------
		0xc80010a0L ctrl_src_dco_mfc ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 idx_src_dco_mfc                    : 3 ;  //  2: 0     
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 sel_src_dco_mfc                    : 1 ;  //     4     
} H15A0_IMX_CTRL_SRC_DCO_MFC_T;

/*-----------------------------------------------------------------------------
		0xc80010a4L ctrl_cvi_src_of_smux ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sel_cvi_src_of_smux_m1             : 2 ;  //  1: 0     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 sel_cvi_src_of_smux_m2             : 2 ;  //  5: 4     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 sel_cvi_src_of_smux_s1             : 2 ;  //  9: 8     
	UINT32                                    : 2 ;  // 11:10     reserved
	UINT32 sel_cvi_src_of_smux_s2             : 2 ;  // 13:12     
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 sel_cvi_src_of_smux_c1             : 2 ;  // 17:16     
	UINT32                                    : 2 ;  // 19:18     reserved
	UINT32 sel_cvi_src_of_smux_c2             : 2 ;  // 21:20     
} H15A0_IMX_CTRL_CVI_SRC_OF_SMUX_T;

/*-----------------------------------------------------------------------------
		0xc80010a8L ctrl_mvi_t_src_of_smux ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 en_mvi_tm1                         : 1 ;  //     0     
	UINT32 sel_mvi_tm1                        : 2 ;  //  2: 1     
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 en_mvi_tm2                         : 1 ;  //     4     
	UINT32 sel_mvi_tm2                        : 2 ;  //  6: 5     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 en_mvi_ts1                         : 1 ;  //     8     
	UINT32 sel_mvi_ts1                        : 2 ;  // 10: 9     
	UINT32                                    : 1 ;  //    11     reserved
	UINT32 en_mvi_ts2                         : 1 ;  //    12     
	UINT32 sel_mvi_ts2                        : 2 ;  // 14:13     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 en_mvi_tc1                         : 1 ;  //    16     
	UINT32 sel_mvi_tc1                        : 2 ;  // 18:17     
	UINT32                                    : 1 ;  //    19     reserved
	UINT32 en_mvi_tc2                         : 1 ;  //    20     
	UINT32 sel_mvi_tc2                        : 2 ;  // 22:21     
} H15A0_IMX_CTRL_MVI_T_SRC_OF_SMUX_T;

/*-----------------------------------------------------------------------------
		0xc80010acL ctrl_mvi_r_src_of_smux ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 en_mvi_rm1                         : 1 ;  //     0     
	UINT32 sel_mvi_rm1                        : 2 ;  //  2: 1     
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 en_mvi_rm2                         : 1 ;  //     4     
	UINT32 sel_mvi_rm2                        : 2 ;  //  6: 5     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 en_mvi_rs1                         : 1 ;  //     8     
	UINT32 sel_mvi_rs1                        : 2 ;  // 10: 9     
	UINT32                                    : 1 ;  //    11     reserved
	UINT32 en_mvi_rs2                         : 1 ;  //    12     
	UINT32 sel_mvi_rs2                        : 2 ;  // 14:13     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 en_mvi_rc1                         : 1 ;  //    16     
	UINT32 sel_mvi_rc1                        : 2 ;  // 18:17     
	UINT32                                    : 1 ;  //    19     reserved
	UINT32 en_mvi_rc2                         : 1 ;  //    20     
	UINT32 sel_mvi_rc2                        : 2 ;  // 22:21     
} H15A0_IMX_CTRL_MVI_R_SRC_OF_SMUX_T;

/*-----------------------------------------------------------------------------
		0xc80010b0L ctrl_block_mux ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 smx1_awuser                        : 4 ;  //  3: 0     
	UINT32 reg_src_sel_fd3                    : 3 ;  //  6: 4     
	UINT32 reg_src_dual_fd3                   : 1 ;  //     7     
	UINT32 reg_smux_m1_fb_src                 : 1 ;  //     8     
	UINT32 reg_smux_m2_fb_src                 : 1 ;  //     9     
	UINT32 reg_smux_s1_fb_src                 : 1 ;  //    10     
	UINT32 reg_smux_c1_fb_src                 : 1 ;  //    11     
	UINT32 reg_smux_c2_fb_src                 : 1 ;  //    12     
	UINT32 reg_reset_en_m1_y                  : 1 ;  //    13     
	UINT32 reg_reset_en_m1_c                  : 1 ;  //    14     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 reg_mvi_tab_init_src               : 2 ;  // 17:16     
	UINT32 reg_reset_en_m2_y                  : 1 ;  //    18     
	UINT32 reg_reset_en_m2_c                  : 1 ;  //    19     
	UINT32 reg_mvi_tcd_init_src               : 2 ;  // 21:20     
	UINT32 reg_reset_en_s1_y                  : 1 ;  //    22     
	UINT32 reg_reset_en_s1_c                  : 1 ;  //    23     
	UINT32 reg_mvi_rab_init_src               : 2 ;  // 25:24     
} H15A0_IMX_CTRL_BLOCK_MUX_T;

/*-----------------------------------------------------------------------------
		0xc80010c0L pic_init_g0_timer ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 start_interval_mode_en             : 1 ;  //     0     
	UINT32 start_interval_mode                : 3 ;  //  3: 1     
	UINT32 g0_pic_init_timer                  : 28;  // 31: 4     
} H15A0_IMX_PIC_INIT_G0_TIMER_T;

/*-----------------------------------------------------------------------------
		0xc80010c4L pic_init_g1_timer ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 start_interval_mode_en             : 1 ;  //     0     
	UINT32 start_interval_mode                : 3 ;  //  3: 1     
	UINT32 g1_pic_init_timer                  : 28;  // 31: 4     
} H15A0_IMX_PIC_INIT_G1_TIMER_T;

/*-----------------------------------------------------------------------------
		0xc80010c8L pic_init_g2_timer ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 start_interval_mode_en             : 1 ;  //     0     
	UINT32 start_interval_mode                : 3 ;  //  3: 1     
	UINT32 g2_pic_init_timer                  : 28;  // 31: 4     
} H15A0_IMX_PIC_INIT_G2_TIMER_T;

/*-----------------------------------------------------------------------------
		0xc80010ccL pic_init_g3_timer ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 start_interval_mode_en             : 1 ;  //     0     
	UINT32 start_interval_mode                : 3 ;  //  3: 1     
	UINT32 g3_pic_init_timer                  : 28;  // 31: 4     
} H15A0_IMX_PIC_INIT_G3_TIMER_T;

/*-----------------------------------------------------------------------------
		0xc80010d0L pic_init_g4_timer ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 start_interval_mode_en             : 1 ;  //     0     
	UINT32 start_interval_mode                : 3 ;  //  3: 1     
	UINT32 g4_pic_init_timer                  : 28;  // 31: 4     
} H15A0_IMX_PIC_INIT_G4_TIMER_T;

/*-----------------------------------------------------------------------------
		0xc80010d4L pic_init_g5_timer ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 start_interval_mode_en             : 1 ;  //     0     
	UINT32 start_interval_mode                : 3 ;  //  3: 1     
	UINT32 g5_pic_init_timer                  : 28;  // 31: 4     
} H15A0_IMX_PIC_INIT_G5_TIMER_T;

/*-----------------------------------------------------------------------------
		0xc80010d8L pic_init_g6_timer ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 start_interval_mode_en             : 1 ;  //     0     
	UINT32 start_interval_mode                : 3 ;  //  3: 1     
	UINT32 g6_pic_init_timer                  : 28;  // 31: 4     
} H15A0_IMX_PIC_INIT_G6_TIMER_T;

/*-----------------------------------------------------------------------------
		0xc80010dcL pic_init_g7_timer ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 start_interval_mode_en             : 1 ;  //     0     
	UINT32 start_interval_mode                : 3 ;  //  3: 1     
	UINT32 g7_pic_init_timer                  : 28;  // 31: 4     
} H15A0_IMX_PIC_INIT_G7_TIMER_T;

/*-----------------------------------------------------------------------------
		0xc80010e4L dbg_timer_run ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 info_timer_run                     : 24;  // 23: 0     
	UINT32 ctrl_timer_run                     : 8 ;  // 31:24     
} H15A0_IMX_DBG_TIMER_RUN_T;

/*-----------------------------------------------------------------------------
		0xc80010e8L dbg_timer_blk ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 info_timer_blk                     : 24;  // 23: 0     
	UINT32 ctrl_timer_blk                     : 8 ;  // 31:24     
} H15A0_IMX_DBG_TIMER_BLK_T;

/*-----------------------------------------------------------------------------
		0xc80010ecL ctrl_monitor ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ctrl_monitor                       : 4 ;  //  3: 0     
} H15A0_IMX_CTRL_MONITOR_T;

/*-----------------------------------------------------------------------------
		0xc80010f0L ctrl_pic_end ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 b0_pic_end_en                      : 1 ;  //     0     
	UINT32 b1_pic_end_en                      : 1 ;  //     1     
	UINT32 b2_pic_end_en                      : 1 ;  //     2     
	UINT32 b3_pic_end_en                      : 1 ;  //     3     
	UINT32 b4_pic_end_en                      : 1 ;  //     4     
	UINT32 b5_pic_end_en                      : 1 ;  //     5     
	UINT32 b6_pic_end_en                      : 1 ;  //     6     
	UINT32 b7_pic_end_en                      : 1 ;  //     7     
	UINT32 b8_pic_end_en                      : 1 ;  //     8     
	UINT32 b9_pic_end_en                      : 1 ;  //     9     
	UINT32 b10_pic_end_en                     : 1 ;  //    10     
	UINT32 b11_pic_end_en                     : 1 ;  //    11     
	UINT32 b12_pic_end_en                     : 1 ;  //    12     
	UINT32 b13_pic_end_en                     : 1 ;  //    13     
	UINT32 b14_pic_end_en                     : 1 ;  //    14     
	UINT32 b15_pic_end_en                     : 1 ;  //    15     
	UINT32 pic_end_g0_force_en                : 1 ;  //    16     
	UINT32 pic_end_g1_force_en                : 1 ;  //    17     
	UINT32 pic_end_g2_force_en                : 1 ;  //    18     
	UINT32 pic_end_g3_force_en                : 1 ;  //    19     
	UINT32 pic_end_g4_force_en                : 1 ;  //    20     
	UINT32 pic_end_g5_force_en                : 1 ;  //    21     
	UINT32 pic_end_g6_force_en                : 1 ;  //    22     
	UINT32 pic_end_g7_force_en                : 1 ;  //    23     
	UINT32 pic_end_g0_force_value             : 1 ;  //    24     
	UINT32 pic_end_g1_force_value             : 1 ;  //    25     
	UINT32 pic_end_g2_force_value             : 1 ;  //    26     
	UINT32 pic_end_g3_force_value             : 1 ;  //    27     
	UINT32 pic_end_g4_force_value             : 1 ;  //    28     
	UINT32 pic_end_g5_force_value             : 1 ;  //    29     
	UINT32 pic_end_g6_force_value             : 1 ;  //    30     
	UINT32 pic_end_g7_force_value             : 1 ;  //    31     
} H15A0_IMX_CTRL_PIC_END_T;

/*-----------------------------------------------------------------------------
		0xc80010f4L info_pic_end ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 g0_pic_end                         : 1 ;  //     0     
	UINT32 g1_pic_end                         : 1 ;  //     1     
	UINT32 g2_pic_end                         : 1 ;  //     2     
	UINT32 g3_pic_end                         : 1 ;  //     3     
	UINT32 g4_pic_end                         : 1 ;  //     4     
	UINT32 g5_pic_end                         : 1 ;  //     5     
	UINT32 g6_pic_end                         : 1 ;  //     6     
	UINT32 g7_pic_end                         : 1 ;  //     7     
	UINT32                                    : 8 ;  // 15: 8     reserved
	UINT32 g0_pic_turn                        : 1 ;  //    16     
	UINT32 g1_pic_turn                        : 1 ;  //    17     
	UINT32 g2_pic_turn                        : 1 ;  //    18     
	UINT32 g3_pic_turn                        : 1 ;  //    19     
	UINT32 g4_pic_turn                        : 1 ;  //    20     
	UINT32 g5_pic_turn                        : 1 ;  //    21     
	UINT32 g6_pic_turn                        : 1 ;  //    22     
	UINT32 g7_pic_turn                        : 1 ;  //    23     
} H15A0_IMX_INFO_PIC_END_T;

/*-----------------------------------------------------------------------------
		0xc80010f8L ctrl_gpio ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 rst_async_bridge_m0_y              : 1 ;  //     0     
	UINT32 rst_async_bridge_m0_c              : 1 ;  //     1     
	UINT32 rst_async_bridge_m1_y              : 1 ;  //     2     
	UINT32 rst_async_bridge_m1_c              : 1 ;  //     3     
	UINT32 rst_async_bridge_s0_y              : 1 ;  //     4     
	UINT32 rst_async_bridge_s0_c              : 1 ;  //     5     
	UINT32                                    : 10;  // 15: 6     reserved
	UINT32 mon_m0_y_en                        : 1 ;  //    16     
	UINT32 mon_m0_y_const                     : 1 ;  //    17     
	UINT32 mon_m0_c_en                        : 1 ;  //    18     
	UINT32 mon_m0_c_const                     : 1 ;  //    19     
	UINT32 mon_m1_y_en                        : 1 ;  //    20     
	UINT32 mon_m1_y_const                     : 1 ;  //    21     
	UINT32 mon_m1_c_en                        : 1 ;  //    22     
	UINT32 mon_m1_c_const                     : 1 ;  //    23     
	UINT32 mon_s0_y_en                        : 1 ;  //    24     
	UINT32 mon_s0_y_const                     : 1 ;  //    25     
	UINT32 mon_s0_c_en                        : 1 ;  //    26     
	UINT32 mon_s0_c_const                     : 1 ;  //    27     
} H15A0_IMX_CTRL_GPIO_T;

/*-----------------------------------------------------------------------------
		0xc80010fcL ctrl_gmau ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ask_cmd_dly_cnt                    : 8 ;  //  7: 0     
	UINT32 smx_aruser                         : 4 ;  // 11: 8     
	UINT32 smx_awuser                         : 4 ;  // 15:12     
	UINT32 ask_to_cnt                         : 10;  // 25:16     
} H15A0_IMX_CTRL_GMAU_T;

/*-----------------------------------------------------------------------------
		0xc8004000L pic_init ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 g0_pic_init                        : 1 ;  //     0     
	UINT32 g1_pic_init                        : 1 ;  //     1     
	UINT32 g2_pic_init                        : 1 ;  //     2     
	UINT32 g3_pic_init                        : 1 ;  //     3     
	UINT32 g4_pic_init                        : 1 ;  //     4     
	UINT32 g5_pic_init                        : 1 ;  //     5     
	UINT32 g6_pic_init                        : 1 ;  //     6     
	UINT32 g7_pic_init                        : 1 ;  //     7     
	UINT32                                    : 8 ;  // 15: 8     reserved
	UINT32 g0_auto_init                       : 1 ;  //    16     
	UINT32 g1_auto_init                       : 1 ;  //    17     
	UINT32 g2_auto_init                       : 1 ;  //    18     
	UINT32 g3_auto_init                       : 1 ;  //    19     
	UINT32 g4_auto_init                       : 1 ;  //    20     
	UINT32 g5_auto_init                       : 1 ;  //    21     
	UINT32 g6_auto_init                       : 1 ;  //    22     
	UINT32 g7_auto_init                       : 1 ;  //    23     
} H15A0_ND0_PIC_INIT_T;

/*-----------------------------------------------------------------------------
		0xc8004004L pic_start ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 g0_pic_start                       : 1 ;  //     0     
	UINT32 g1_pic_start                       : 1 ;  //     1     
	UINT32 g2_pic_start                       : 1 ;  //     2     
	UINT32 g3_pic_start                       : 1 ;  //     3     
	UINT32 g4_pic_start                       : 1 ;  //     4     
	UINT32 g5_pic_start                       : 1 ;  //     5     
	UINT32 g6_pic_start                       : 1 ;  //     6     
	UINT32 g7_pic_start                       : 1 ;  //     7     
} H15A0_ND0_PIC_START_T;

/*-----------------------------------------------------------------------------
		0xc8004008L ctrl_auto_init ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 idx_src_g0_auto_init               : 3 ;  //  2: 0     
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 idx_src_g1_auto_init               : 3 ;  //  6: 4     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 idx_src_g2_auto_init               : 3 ;  // 10: 8     
	UINT32                                    : 1 ;  //    11     reserved
	UINT32 idx_src_g3_auto_init               : 3 ;  // 14:12     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 idx_src_g4_auto_init               : 3 ;  // 18:16     
	UINT32                                    : 1 ;  //    19     reserved
	UINT32 idx_src_g5_auto_init               : 3 ;  // 22:20     
	UINT32                                    : 1 ;  //    23     reserved
	UINT32 idx_src_g6_auto_init               : 3 ;  // 26:24     
	UINT32                                    : 1 ;  //    27     reserved
	UINT32 idx_src_g7_auto_init               : 3 ;  // 30:28     
} H15A0_ND0_CTRL_AUTO_INIT_T;

/*-----------------------------------------------------------------------------
		0xc800400cL ctrl_intr_pulse ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 idx_src_i0_intr_pulse              : 3 ;  //  2: 0     
	UINT32 sel_src_i0_intr_pulse              : 1 ;  //     3     
	UINT32 idx_src_i1_intr_pulse              : 3 ;  //  6: 4     
	UINT32 sel_src_i1_intr_pulse              : 1 ;  //     7     
	UINT32 idx_src_i2_intr_pulse              : 3 ;  // 10: 8     
	UINT32 sel_src_i2_intr_pulse              : 1 ;  //    11     
	UINT32 idx_src_i3_intr_pulse              : 3 ;  // 14:12     
	UINT32 sel_src_i3_intr_pulse              : 1 ;  //    15     
	UINT32 idx_src_i4_intr_pulse              : 3 ;  // 18:16     
	UINT32 sel_src_i4_intr_pulse              : 1 ;  //    19     
	UINT32 idx_src_i5_intr_pulse              : 3 ;  // 22:20     
	UINT32 sel_src_i5_intr_pulse              : 1 ;  //    23     
	UINT32 idx_src_i6_intr_pulse              : 3 ;  // 26:24     
	UINT32 sel_src_i6_intr_pulse              : 1 ;  //    27     
	UINT32 idx_src_i7_intr_pulse              : 3 ;  // 30:28     
	UINT32 sel_src_i7_intr_pulse              : 1 ;  //    31     
} H15A0_ND0_CTRL_INTR_PULSE_T;

/*-----------------------------------------------------------------------------
		0xc8004010L intr_reg ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 intr_i0_intr_pulse                 : 1 ;  //     0     
	UINT32 intr_i1_intr_pulse                 : 1 ;  //     1     
	UINT32 intr_i2_intr_pulse                 : 1 ;  //     2     
	UINT32 intr_i3_intr_pulse                 : 1 ;  //     3     
	UINT32 intr_i4_intr_pulse                 : 1 ;  //     4     
	UINT32 intr_i5_intr_pulse                 : 1 ;  //     5     
	UINT32 intr_i6_intr_pulse                 : 1 ;  //     6     
	UINT32 intr_i7_intr_pulse                 : 1 ;  //     7     
	UINT32 intr_g0_pic_end                    : 1 ;  //     8     
	UINT32 intr_g1_pic_end                    : 1 ;  //     9     
	UINT32 intr_g2_pic_end                    : 1 ;  //    10     
	UINT32 intr_g3_pic_end                    : 1 ;  //    11     
	UINT32 intr_g4_pic_end                    : 1 ;  //    12     
	UINT32 intr_g5_pic_end                    : 1 ;  //    13     
	UINT32 intr_g6_pic_end                    : 1 ;  //    14     
	UINT32 intr_g7_pic_end                    : 1 ;  //    15     
	UINT32 intr_nd0                           : 1 ;  //    16     
} H15A0_ND0_INTR_REG_T;

/*-----------------------------------------------------------------------------
		0xc8004014L intr_mask ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mask_i0_src_pulse                  : 1 ;  //     0     
	UINT32 mask_i1_src_pulse                  : 1 ;  //     1     
	UINT32 mask_i2_src_pulse                  : 1 ;  //     2     
	UINT32 mask_i3_src_pulse                  : 1 ;  //     3     
	UINT32 mask_i4_src_pulse                  : 1 ;  //     4     
	UINT32 mask_i5_src_pulse                  : 1 ;  //     5     
	UINT32 mask_i6_src_pulse                  : 1 ;  //     6     
	UINT32 mask_i7_src_pulse                  : 1 ;  //     7     
	UINT32 mask_g0_pic_end                    : 1 ;  //     8     
	UINT32 mask_g1_pic_end                    : 1 ;  //     9     
	UINT32 mask_g2_pic_end                    : 1 ;  //    10     
	UINT32 mask_g3_pic_end                    : 1 ;  //    11     
	UINT32 mask_g4_pic_end                    : 1 ;  //    12     
	UINT32 mask_g5_pic_end                    : 1 ;  //    13     
	UINT32 mask_g6_pic_end                    : 1 ;  //    14     
	UINT32 mask_g7_pic_end                    : 1 ;  //    15     
	UINT32 mask_nd0                           : 1 ;  //    16     
} H15A0_ND0_INTR_MASK_T;

/*-----------------------------------------------------------------------------
		0xc8004018L intr_mux ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mux_i0_src_pulse                   : 1 ;  //     0     
	UINT32 mux_i1_src_pulse                   : 1 ;  //     1     
	UINT32 mux_i2_src_pulse                   : 1 ;  //     2     
	UINT32 mux_i3_src_pulse                   : 1 ;  //     3     
	UINT32 mux_i4_src_pulse                   : 1 ;  //     4     
	UINT32 mux_i5_src_pulse                   : 1 ;  //     5     
	UINT32 mux_i6_src_pulse                   : 1 ;  //     6     
	UINT32 mux_i7_src_pulse                   : 1 ;  //     7     
	UINT32 mux_g0_pic_end                     : 1 ;  //     8     
	UINT32 mux_g1_pic_end                     : 1 ;  //     9     
	UINT32 mux_g2_pic_end                     : 1 ;  //    10     
	UINT32 mux_g3_pic_end                     : 1 ;  //    11     
	UINT32 mux_g4_pic_end                     : 1 ;  //    12     
	UINT32 mux_g5_pic_end                     : 1 ;  //    13     
	UINT32 mux_g6_pic_end                     : 1 ;  //    14     
	UINT32 mux_g7_pic_end                     : 1 ;  //    15     
	UINT32 mux_nd0                            : 1 ;  //    16     
} H15A0_ND0_INTR_MUX_T;

/*-----------------------------------------------------------------------------
		0xc800401cL intr_en ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 en_i0_src_pulse                    : 1 ;  //     0     
	UINT32 en_i1_src_pulse                    : 1 ;  //     1     
	UINT32 en_i2_src_pulse                    : 1 ;  //     2     
	UINT32 en_i3_src_pulse                    : 1 ;  //     3     
	UINT32 en_i4_src_pulse                    : 1 ;  //     4     
	UINT32 en_i5_src_pulse                    : 1 ;  //     5     
	UINT32 en_i6_src_pulse                    : 1 ;  //     6     
	UINT32 en_i7_src_pulse                    : 1 ;  //     7     
	UINT32 en_g0_pic_end                      : 1 ;  //     8     
	UINT32 en_g1_pic_end                      : 1 ;  //     9     
	UINT32 en_g2_pic_end                      : 1 ;  //    10     
	UINT32 en_g3_pic_end                      : 1 ;  //    11     
	UINT32 en_g4_pic_end                      : 1 ;  //    12     
	UINT32 en_g5_pic_end                      : 1 ;  //    13     
	UINT32 en_g6_pic_end                      : 1 ;  //    14     
	UINT32 en_g7_pic_end                      : 1 ;  //    15     
	UINT32 en_nd0                             : 1 ;  //    16     
} H15A0_ND0_INTR_EN_T;

/*-----------------------------------------------------------------------------
		0xc80040b0L ctrl_block_mux ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_path_reset_en_m1_y             : 1 ;  //     0     
	UINT32 reg_path_reset_en_m1_c             : 1 ;  //     1     
} H15A0_ND0_CTRL_BLOCK_MUX_T;

/*-----------------------------------------------------------------------------
		0xc80040c0L pic_init_g0_timer ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 start_interval_mode_en             : 1 ;  //     0     
	UINT32 start_interval_mode                : 3 ;  //  3: 1     
	UINT32 g0_pic_init_timer                  : 28;  // 31: 4     
} H15A0_ND0_PIC_INIT_G0_TIMER_T;

/*-----------------------------------------------------------------------------
		0xc80040c4L pic_init_g1_timer ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 start_interval_mode_en             : 1 ;  //     0     
	UINT32 start_interval_mode                : 3 ;  //  3: 1     
	UINT32 g1_pic_init_timer                  : 28;  // 31: 4     
} H15A0_ND0_PIC_INIT_G1_TIMER_T;

/*-----------------------------------------------------------------------------
		0xc80040c8L pic_init_g2_timer ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 start_interval_mode_en             : 1 ;  //     0     
	UINT32 start_interval_mode                : 3 ;  //  3: 1     
	UINT32 g2_pic_init_timer                  : 28;  // 31: 4     
} H15A0_ND0_PIC_INIT_G2_TIMER_T;

/*-----------------------------------------------------------------------------
		0xc80040ccL pic_init_g3_timer ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 start_interval_mode_en             : 1 ;  //     0     
	UINT32 start_interval_mode                : 3 ;  //  3: 1     
	UINT32 g3_pic_init_timer                  : 28;  // 31: 4     
} H15A0_ND0_PIC_INIT_G3_TIMER_T;

/*-----------------------------------------------------------------------------
		0xc80040d0L pic_init_g4_timer ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 start_interval_mode_en             : 1 ;  //     0     
	UINT32 start_interval_mode                : 3 ;  //  3: 1     
	UINT32 g4_pic_init_timer                  : 28;  // 31: 4     
} H15A0_ND0_PIC_INIT_G4_TIMER_T;

/*-----------------------------------------------------------------------------
		0xc80040d4L pic_init_g5_timer ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 start_interval_mode_en             : 1 ;  //     0     
	UINT32 start_interval_mode                : 3 ;  //  3: 1     
	UINT32 g5_pic_init_timer                  : 28;  // 31: 4     
} H15A0_ND0_PIC_INIT_G5_TIMER_T;

/*-----------------------------------------------------------------------------
		0xc80040d8L pic_init_g6_timer ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 start_interval_mode_en             : 1 ;  //     0     
	UINT32 start_interval_mode                : 3 ;  //  3: 1     
	UINT32 g6_pic_init_timer                  : 28;  // 31: 4     
} H15A0_ND0_PIC_INIT_G6_TIMER_T;

/*-----------------------------------------------------------------------------
		0xc80040dcL pic_init_g7_timer ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 start_interval_mode_en             : 1 ;  //     0     
	UINT32 start_interval_mode                : 3 ;  //  3: 1     
	UINT32 g7_pic_init_timer                  : 28;  // 31: 4     
} H15A0_ND0_PIC_INIT_G7_TIMER_T;

/*-----------------------------------------------------------------------------
		0xc80040e4L dbg_timer_run ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 info_timer_run                     : 24;  // 23: 0     
	UINT32 ctrl_timer_run                     : 8 ;  // 31:24     
} H15A0_ND0_DBG_TIMER_RUN_T;

/*-----------------------------------------------------------------------------
		0xc80040e8L dbg_timer_blk ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 info_timer_blk                     : 24;  // 23: 0     
	UINT32 ctrl_timer_blk                     : 8 ;  // 31:24     
} H15A0_ND0_DBG_TIMER_BLK_T;

/*-----------------------------------------------------------------------------
		0xc80040ecL ctrl_monitor ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ctrl_monitor                       : 32;  // 31: 0     
} H15A0_ND0_CTRL_MONITOR_T;

/*-----------------------------------------------------------------------------
		0xc80040f0L ctrl_pic_end ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 b0_pic_end_en                      : 1 ;  //     0     
	UINT32 b1_pic_end_en                      : 1 ;  //     1     
	UINT32 b2_pic_end_en                      : 1 ;  //     2     
	UINT32 b3_pic_end_en                      : 1 ;  //     3     
	UINT32 b4_pic_end_en                      : 1 ;  //     4     
	UINT32 b5_pic_end_en                      : 1 ;  //     5     
	UINT32 b6_pic_end_en                      : 1 ;  //     6     
	UINT32 b7_pic_end_en                      : 1 ;  //     7     
	UINT32 b8_pic_end_en                      : 1 ;  //     8     
	UINT32 b9_pic_end_en                      : 1 ;  //     9     
	UINT32 b10_pic_end_en                     : 1 ;  //    10     
	UINT32 b11_pic_end_en                     : 1 ;  //    11     
	UINT32 b12_pic_end_en                     : 1 ;  //    12     
	UINT32 b13_pic_end_en                     : 1 ;  //    13     
	UINT32 b14_pic_end_en                     : 1 ;  //    14     
	UINT32 b15_pic_end_en                     : 1 ;  //    15     
	UINT32 pic_end_g0_force_en                : 1 ;  //    16     
	UINT32 pic_end_g1_force_en                : 1 ;  //    17     
	UINT32 pic_end_g2_force_en                : 1 ;  //    18     
	UINT32 pic_end_g3_force_en                : 1 ;  //    19     
	UINT32 pic_end_g4_force_en                : 1 ;  //    20     
	UINT32 pic_end_g5_force_en                : 1 ;  //    21     
	UINT32 pic_end_g6_force_en                : 1 ;  //    22     
	UINT32 pic_end_g7_force_en                : 1 ;  //    23     
	UINT32 pic_end_g0_force_value             : 1 ;  //    24     
	UINT32 pic_end_g1_force_value             : 1 ;  //    25     
	UINT32 pic_end_g2_force_value             : 1 ;  //    26     
	UINT32 pic_end_g3_force_value             : 1 ;  //    27     
	UINT32 pic_end_g4_force_value             : 1 ;  //    28     
	UINT32 pic_end_g5_force_value             : 1 ;  //    29     
	UINT32 pic_end_g6_force_value             : 1 ;  //    30     
	UINT32 pic_end_g7_force_value             : 1 ;  //    31     
} H15A0_ND0_CTRL_PIC_END_T;

/*-----------------------------------------------------------------------------
		0xc80040f4L info_pic_end ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 g0_pic_end                         : 1 ;  //     0     
	UINT32 g1_pic_end                         : 1 ;  //     1     
	UINT32 g2_pic_end                         : 1 ;  //     2     
	UINT32 g3_pic_end                         : 1 ;  //     3     
	UINT32 g4_pic_end                         : 1 ;  //     4     
	UINT32 g5_pic_end                         : 1 ;  //     5     
	UINT32 g6_pic_end                         : 1 ;  //     6     
	UINT32 g7_pic_end                         : 1 ;  //     7     
	UINT32                                    : 8 ;  // 15: 8     reserved
	UINT32 g0_pic_turn                        : 1 ;  //    16     
	UINT32 g1_pic_turn                        : 1 ;  //    17     
	UINT32 g2_pic_turn                        : 1 ;  //    18     
	UINT32 g3_pic_turn                        : 1 ;  //    19     
	UINT32 g4_pic_turn                        : 1 ;  //    20     
	UINT32 g5_pic_turn                        : 1 ;  //    21     
	UINT32 g6_pic_turn                        : 1 ;  //    22     
	UINT32 g7_pic_turn                        : 1 ;  //    23     
} H15A0_ND0_INFO_PIC_END_T;

/*-----------------------------------------------------------------------------
		0xc80040f8L ctrl_gpio ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 rst_async_bridge_m0_y              : 1 ;  //     0     
	UINT32 rst_async_bridge_m0_c              : 1 ;  //     1     
	UINT32                                    : 14;  // 15: 2     reserved
	UINT32 mon_m0_y_en                        : 1 ;  //    16     
	UINT32 mon_m0_y_const                     : 1 ;  //    17     
	UINT32 mon_m0_c_en                        : 1 ;  //    18     
	UINT32 mon_m0_c_const                     : 1 ;  //    19     
} H15A0_ND0_CTRL_GPIO_T;

/*-----------------------------------------------------------------------------
		0xc80040fcL ctrl_gmau ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ask_cmd_dly_cnt                    : 8 ;  //  7: 0     
	UINT32 smx_aruser                         : 4 ;  // 11: 8     
	UINT32 smx_awuser                         : 4 ;  // 15:12     
	UINT32 ask_to_cnt                         : 10;  // 25:16     
} H15A0_ND0_CTRL_GMAU_T;

/*-----------------------------------------------------------------------------
		0xc8005000L pic_init ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 g0_pic_init                        : 1 ;  //     0     
	UINT32 g1_pic_init                        : 1 ;  //     1     
	UINT32 g2_pic_init                        : 1 ;  //     2     
	UINT32 g3_pic_init                        : 1 ;  //     3     
	UINT32 g4_pic_init                        : 1 ;  //     4     
	UINT32 g5_pic_init                        : 1 ;  //     5     
	UINT32 g6_pic_init                        : 1 ;  //     6     
	UINT32 g7_pic_init                        : 1 ;  //     7     
	UINT32                                    : 8 ;  // 15: 8     reserved
	UINT32 g0_auto_init                       : 1 ;  //    16     
	UINT32 g1_auto_init                       : 1 ;  //    17     
	UINT32 g2_auto_init                       : 1 ;  //    18     
	UINT32 g3_auto_init                       : 1 ;  //    19     
	UINT32 g4_auto_init                       : 1 ;  //    20     
	UINT32 g5_auto_init                       : 1 ;  //    21     
	UINT32 g6_auto_init                       : 1 ;  //    22     
	UINT32 g7_auto_init                       : 1 ;  //    23     
} H15A0_ND1_PIC_INIT_T;

/*-----------------------------------------------------------------------------
		0xc8005004L pic_start ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 g0_pic_start                       : 1 ;  //     0     
	UINT32 g1_pic_start                       : 1 ;  //     1     
	UINT32 g2_pic_start                       : 1 ;  //     2     
	UINT32 g3_pic_start                       : 1 ;  //     3     
	UINT32 g4_pic_start                       : 1 ;  //     4     
	UINT32 g5_pic_start                       : 1 ;  //     5     
	UINT32 g6_pic_start                       : 1 ;  //     6     
	UINT32 g7_pic_start                       : 1 ;  //     7     
} H15A0_ND1_PIC_START_T;

/*-----------------------------------------------------------------------------
		0xc8005008L ctrl_auto_init ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 idx_src_g0_auto_init               : 3 ;  //  2: 0     
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 idx_src_g1_auto_init               : 3 ;  //  6: 4     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 idx_src_g2_auto_init               : 3 ;  // 10: 8     
	UINT32                                    : 1 ;  //    11     reserved
	UINT32 idx_src_g3_auto_init               : 3 ;  // 14:12     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 idx_src_g4_auto_init               : 3 ;  // 18:16     
	UINT32                                    : 1 ;  //    19     reserved
	UINT32 idx_src_g5_auto_init               : 3 ;  // 22:20     
	UINT32                                    : 1 ;  //    23     reserved
	UINT32 idx_src_g6_auto_init               : 3 ;  // 26:24     
	UINT32                                    : 1 ;  //    27     reserved
	UINT32 idx_src_g7_auto_init               : 3 ;  // 30:28     
} H15A0_ND1_CTRL_AUTO_INIT_T;

/*-----------------------------------------------------------------------------
		0xc800500cL ctrl_intr_pulse ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 idx_src_i0_intr_pulse              : 3 ;  //  2: 0     
	UINT32 sel_src_i0_intr_pulse              : 1 ;  //     3     
	UINT32 idx_src_i1_intr_pulse              : 3 ;  //  6: 4     
	UINT32 sel_src_i1_intr_pulse              : 1 ;  //     7     
	UINT32 idx_src_i2_intr_pulse              : 3 ;  // 10: 8     
	UINT32 sel_src_i2_intr_pulse              : 1 ;  //    11     
	UINT32 idx_src_i3_intr_pulse              : 3 ;  // 14:12     
	UINT32 sel_src_i3_intr_pulse              : 1 ;  //    15     
	UINT32 idx_src_i4_intr_pulse              : 3 ;  // 18:16     
	UINT32 sel_src_i4_intr_pulse              : 1 ;  //    19     
	UINT32 idx_src_i5_intr_pulse              : 3 ;  // 22:20     
	UINT32 sel_src_i5_intr_pulse              : 1 ;  //    23     
	UINT32 idx_src_i6_intr_pulse              : 3 ;  // 26:24     
	UINT32 sel_src_i6_intr_pulse              : 1 ;  //    27     
	UINT32 idx_src_i7_intr_pulse              : 3 ;  // 30:28     
	UINT32 sel_src_i7_intr_pulse              : 1 ;  //    31     
} H15A0_ND1_CTRL_INTR_PULSE_T;

/*-----------------------------------------------------------------------------
		0xc8005010L intr_reg ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 intr_i0_intr_pulse                 : 1 ;  //     0     
	UINT32 intr_i1_intr_pulse                 : 1 ;  //     1     
	UINT32 intr_i2_intr_pulse                 : 1 ;  //     2     
	UINT32 intr_i3_intr_pulse                 : 1 ;  //     3     
	UINT32 intr_i4_intr_pulse                 : 1 ;  //     4     
	UINT32 intr_i5_intr_pulse                 : 1 ;  //     5     
	UINT32 intr_i6_intr_pulse                 : 1 ;  //     6     
	UINT32 intr_i7_intr_pulse                 : 1 ;  //     7     
	UINT32 intr_g0_pic_end                    : 1 ;  //     8     
	UINT32 intr_g1_pic_end                    : 1 ;  //     9     
	UINT32 intr_g2_pic_end                    : 1 ;  //    10     
	UINT32 intr_g3_pic_end                    : 1 ;  //    11     
	UINT32 intr_g4_pic_end                    : 1 ;  //    12     
	UINT32 intr_g5_pic_end                    : 1 ;  //    13     
	UINT32 intr_g6_pic_end                    : 1 ;  //    14     
	UINT32 intr_g7_pic_end                    : 1 ;  //    15     
	UINT32 intr_nd0                           : 1 ;  //    16     
} H15A0_ND1_INTR_REG_T;

/*-----------------------------------------------------------------------------
		0xc8005014L intr_mask ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mask_i0_src_pulse                  : 1 ;  //     0     
	UINT32 mask_i1_src_pulse                  : 1 ;  //     1     
	UINT32 mask_i2_src_pulse                  : 1 ;  //     2     
	UINT32 mask_i3_src_pulse                  : 1 ;  //     3     
	UINT32 mask_i4_src_pulse                  : 1 ;  //     4     
	UINT32 mask_i5_src_pulse                  : 1 ;  //     5     
	UINT32 mask_i6_src_pulse                  : 1 ;  //     6     
	UINT32 mask_i7_src_pulse                  : 1 ;  //     7     
	UINT32 mask_g0_pic_end                    : 1 ;  //     8     
	UINT32 mask_g1_pic_end                    : 1 ;  //     9     
	UINT32 mask_g2_pic_end                    : 1 ;  //    10     
	UINT32 mask_g3_pic_end                    : 1 ;  //    11     
	UINT32 mask_g4_pic_end                    : 1 ;  //    12     
	UINT32 mask_g5_pic_end                    : 1 ;  //    13     
	UINT32 mask_g6_pic_end                    : 1 ;  //    14     
	UINT32 mask_g7_pic_end                    : 1 ;  //    15     
	UINT32 mask_nd0                           : 1 ;  //    16     
} H15A0_ND1_INTR_MASK_T;

/*-----------------------------------------------------------------------------
		0xc8005018L intr_mux ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mux_i0_src_pulse                   : 1 ;  //     0     
	UINT32 mux_i1_src_pulse                   : 1 ;  //     1     
	UINT32 mux_i2_src_pulse                   : 1 ;  //     2     
	UINT32 mux_i3_src_pulse                   : 1 ;  //     3     
	UINT32 mux_i4_src_pulse                   : 1 ;  //     4     
	UINT32 mux_i5_src_pulse                   : 1 ;  //     5     
	UINT32 mux_i6_src_pulse                   : 1 ;  //     6     
	UINT32 mux_i7_src_pulse                   : 1 ;  //     7     
	UINT32 mux_g0_pic_end                     : 1 ;  //     8     
	UINT32 mux_g1_pic_end                     : 1 ;  //     9     
	UINT32 mux_g2_pic_end                     : 1 ;  //    10     
	UINT32 mux_g3_pic_end                     : 1 ;  //    11     
	UINT32 mux_g4_pic_end                     : 1 ;  //    12     
	UINT32 mux_g5_pic_end                     : 1 ;  //    13     
	UINT32 mux_g6_pic_end                     : 1 ;  //    14     
	UINT32 mux_g7_pic_end                     : 1 ;  //    15     
	UINT32 mux_nd0                            : 1 ;  //    16     
} H15A0_ND1_INTR_MUX_T;

/*-----------------------------------------------------------------------------
		0xc800501cL intr_en ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 en_i0_src_pulse                    : 1 ;  //     0     
	UINT32 en_i1_src_pulse                    : 1 ;  //     1     
	UINT32 en_i2_src_pulse                    : 1 ;  //     2     
	UINT32 en_i3_src_pulse                    : 1 ;  //     3     
	UINT32 en_i4_src_pulse                    : 1 ;  //     4     
	UINT32 en_i5_src_pulse                    : 1 ;  //     5     
	UINT32 en_i6_src_pulse                    : 1 ;  //     6     
	UINT32 en_i7_src_pulse                    : 1 ;  //     7     
	UINT32 en_g0_pic_end                      : 1 ;  //     8     
	UINT32 en_g1_pic_end                      : 1 ;  //     9     
	UINT32 en_g2_pic_end                      : 1 ;  //    10     
	UINT32 en_g3_pic_end                      : 1 ;  //    11     
	UINT32 en_g4_pic_end                      : 1 ;  //    12     
	UINT32 en_g5_pic_end                      : 1 ;  //    13     
	UINT32 en_g6_pic_end                      : 1 ;  //    14     
	UINT32 en_g7_pic_end                      : 1 ;  //    15     
	UINT32 en_nd0                             : 1 ;  //    16     
} H15A0_ND1_INTR_EN_T;

/*-----------------------------------------------------------------------------
		0xc80050b0L ctrl_block_mux ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_path_reset_en_m2_y             : 1 ;  //     0     
	UINT32 reg_path_reset_en_m2_c             : 1 ;  //     1     
} H15A0_ND1_CTRL_BLOCK_MUX_T;

/*-----------------------------------------------------------------------------
		0xc80050c0L pic_init_g0_timer ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 start_interval_mode_en             : 1 ;  //     0     
	UINT32 start_interval_mode                : 3 ;  //  3: 1     
	UINT32 g0_pic_init_timer                  : 28;  // 31: 4     
} H15A0_ND1_PIC_INIT_G0_TIMER_T;

/*-----------------------------------------------------------------------------
		0xc80050c4L pic_init_g1_timer ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 start_interval_mode_en             : 1 ;  //     0     
	UINT32 start_interval_mode                : 3 ;  //  3: 1     
	UINT32 g1_pic_init_timer                  : 28;  // 31: 4     
} H15A0_ND1_PIC_INIT_G1_TIMER_T;

/*-----------------------------------------------------------------------------
		0xc80050c8L pic_init_g2_timer ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 start_interval_mode_en             : 1 ;  //     0     
	UINT32 start_interval_mode                : 3 ;  //  3: 1     
	UINT32 g2_pic_init_timer                  : 28;  // 31: 4     
} H15A0_ND1_PIC_INIT_G2_TIMER_T;

/*-----------------------------------------------------------------------------
		0xc80050ccL pic_init_g3_timer ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 start_interval_mode_en             : 1 ;  //     0     
	UINT32 start_interval_mode                : 3 ;  //  3: 1     
	UINT32 g3_pic_init_timer                  : 28;  // 31: 4     
} H15A0_ND1_PIC_INIT_G3_TIMER_T;

/*-----------------------------------------------------------------------------
		0xc80050d0L pic_init_g4_timer ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 start_interval_mode_en             : 1 ;  //     0     
	UINT32 start_interval_mode                : 3 ;  //  3: 1     
	UINT32 g4_pic_init_timer                  : 28;  // 31: 4     
} H15A0_ND1_PIC_INIT_G4_TIMER_T;

/*-----------------------------------------------------------------------------
		0xc80050d4L pic_init_g5_timer ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 start_interval_mode_en             : 1 ;  //     0     
	UINT32 start_interval_mode                : 3 ;  //  3: 1     
	UINT32 g5_pic_init_timer                  : 28;  // 31: 4     
} H15A0_ND1_PIC_INIT_G5_TIMER_T;

/*-----------------------------------------------------------------------------
		0xc80050d8L pic_init_g6_timer ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 start_interval_mode_en             : 1 ;  //     0     
	UINT32 start_interval_mode                : 3 ;  //  3: 1     
	UINT32 g6_pic_init_timer                  : 28;  // 31: 4     
} H15A0_ND1_PIC_INIT_G6_TIMER_T;

/*-----------------------------------------------------------------------------
		0xc80050dcL pic_init_g7_timer ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 start_interval_mode_en             : 1 ;  //     0     
	UINT32 start_interval_mode                : 3 ;  //  3: 1     
	UINT32 g7_pic_init_timer                  : 28;  // 31: 4     
} H15A0_ND1_PIC_INIT_G7_TIMER_T;

/*-----------------------------------------------------------------------------
		0xc80050e4L dbg_timer_run ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 info_timer_run                     : 24;  // 23: 0     
	UINT32 ctrl_timer_run                     : 8 ;  // 31:24     
} H15A0_ND1_DBG_TIMER_RUN_T;

/*-----------------------------------------------------------------------------
		0xc80050e8L dbg_timer_blk ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 info_timer_blk                     : 24;  // 23: 0     
	UINT32 ctrl_timer_blk                     : 8 ;  // 31:24     
} H15A0_ND1_DBG_TIMER_BLK_T;

/*-----------------------------------------------------------------------------
		0xc80050ecL ctrl_monitor ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ctrl_monitor                       : 32;  // 31: 0     
} H15A0_ND1_CTRL_MONITOR_T;

/*-----------------------------------------------------------------------------
		0xc80050f0L ctrl_pic_end ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 b0_pic_end_en                      : 1 ;  //     0     
	UINT32 b1_pic_end_en                      : 1 ;  //     1     
	UINT32 b2_pic_end_en                      : 1 ;  //     2     
	UINT32 b3_pic_end_en                      : 1 ;  //     3     
	UINT32 b4_pic_end_en                      : 1 ;  //     4     
	UINT32 b5_pic_end_en                      : 1 ;  //     5     
	UINT32 b6_pic_end_en                      : 1 ;  //     6     
	UINT32 b7_pic_end_en                      : 1 ;  //     7     
	UINT32 b8_pic_end_en                      : 1 ;  //     8     
	UINT32 b9_pic_end_en                      : 1 ;  //     9     
	UINT32 b10_pic_end_en                     : 1 ;  //    10     
	UINT32 b11_pic_end_en                     : 1 ;  //    11     
	UINT32 b12_pic_end_en                     : 1 ;  //    12     
	UINT32 b13_pic_end_en                     : 1 ;  //    13     
	UINT32 b14_pic_end_en                     : 1 ;  //    14     
	UINT32 b15_pic_end_en                     : 1 ;  //    15     
	UINT32 pic_end_g0_force_en                : 1 ;  //    16     
	UINT32 pic_end_g1_force_en                : 1 ;  //    17     
	UINT32 pic_end_g2_force_en                : 1 ;  //    18     
	UINT32 pic_end_g3_force_en                : 1 ;  //    19     
	UINT32 pic_end_g4_force_en                : 1 ;  //    20     
	UINT32 pic_end_g5_force_en                : 1 ;  //    21     
	UINT32 pic_end_g6_force_en                : 1 ;  //    22     
	UINT32 pic_end_g7_force_en                : 1 ;  //    23     
	UINT32 pic_end_g0_force_value             : 1 ;  //    24     
	UINT32 pic_end_g1_force_value             : 1 ;  //    25     
	UINT32 pic_end_g2_force_value             : 1 ;  //    26     
	UINT32 pic_end_g3_force_value             : 1 ;  //    27     
	UINT32 pic_end_g4_force_value             : 1 ;  //    28     
	UINT32 pic_end_g5_force_value             : 1 ;  //    29     
	UINT32 pic_end_g6_force_value             : 1 ;  //    30     
	UINT32 pic_end_g7_force_value             : 1 ;  //    31     
} H15A0_ND1_CTRL_PIC_END_T;

/*-----------------------------------------------------------------------------
		0xc80050f4L info_pic_end ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 g0_pic_end                         : 1 ;  //     0     
	UINT32 g1_pic_end                         : 1 ;  //     1     
	UINT32 g2_pic_end                         : 1 ;  //     2     
	UINT32 g3_pic_end                         : 1 ;  //     3     
	UINT32 g4_pic_end                         : 1 ;  //     4     
	UINT32 g5_pic_end                         : 1 ;  //     5     
	UINT32 g6_pic_end                         : 1 ;  //     6     
	UINT32 g7_pic_end                         : 1 ;  //     7     
	UINT32                                    : 8 ;  // 15: 8     reserved
	UINT32 g0_pic_turn                        : 1 ;  //    16     
	UINT32 g1_pic_turn                        : 1 ;  //    17     
	UINT32 g2_pic_turn                        : 1 ;  //    18     
	UINT32 g3_pic_turn                        : 1 ;  //    19     
	UINT32 g4_pic_turn                        : 1 ;  //    20     
	UINT32 g5_pic_turn                        : 1 ;  //    21     
	UINT32 g6_pic_turn                        : 1 ;  //    22     
	UINT32 g7_pic_turn                        : 1 ;  //    23     
} H15A0_ND1_INFO_PIC_END_T;

/*-----------------------------------------------------------------------------
		0xc80050f8L ctrl_gpio ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 rst_async_bridge_m1_y              : 1 ;  //     0     
	UINT32 rst_async_bridge_m1_c              : 1 ;  //     1     
	UINT32                                    : 14;  // 15: 2     reserved
	UINT32 mon_m1_y_en                        : 1 ;  //    16     
	UINT32 mon_m1_y_const                     : 1 ;  //    17     
	UINT32 mon_m1_c_en                        : 1 ;  //    18     
	UINT32 mon_m1_c_const                     : 1 ;  //    19     
} H15A0_ND1_CTRL_GPIO_T;

/*-----------------------------------------------------------------------------
		0xc80050fcL ctrl_gmau ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ask_cmd_dly_cnt                    : 8 ;  //  7: 0     
	UINT32 smx_aruser                         : 4 ;  // 11: 8     
	UINT32 smx_awuser                         : 4 ;  // 15:12     
	UINT32 ask_to_cnt                         : 10;  // 25:16     
} H15A0_ND1_CTRL_GMAU_T;

/*-----------------------------------------------------------------------------
		0xc800600cL ctrl_intr_pulse ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 idx_src_i0_intr_pulse              : 3 ;  //  2: 0     
	UINT32 sel_src_i0_intr_pulse              : 1 ;  //     3     
	UINT32 idx_src_i1_intr_pulse              : 3 ;  //  6: 4     
	UINT32 sel_src_i1_intr_pulse              : 1 ;  //     7     
	UINT32 idx_src_i2_intr_pulse              : 3 ;  // 10: 8     
	UINT32 sel_src_i2_intr_pulse              : 1 ;  //    11     
	UINT32 idx_src_i3_intr_pulse              : 3 ;  // 14:12     
	UINT32 sel_src_i3_intr_pulse              : 1 ;  //    15     
	UINT32 idx_src_i4_intr_pulse              : 3 ;  // 18:16     
	UINT32 sel_src_i4_intr_pulse              : 1 ;  //    19     
	UINT32 idx_src_i5_intr_pulse              : 3 ;  // 22:20     
	UINT32 sel_src_i5_intr_pulse              : 1 ;  //    23     
	UINT32 idx_src_i6_intr_pulse              : 3 ;  // 26:24     
	UINT32 sel_src_i6_intr_pulse              : 1 ;  //    27     
	UINT32 idx_src_i7_intr_pulse              : 3 ;  // 30:28     
	UINT32 sel_src_i7_intr_pulse              : 1 ;  //    31     
} H15A0_SUB_CTRL_INTR_PULSE_T;

/*-----------------------------------------------------------------------------
		0xc8006010L intr_reg ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 intr_i0_intr_pulse                 : 1 ;  //     0     
	UINT32 intr_i1_intr_pulse                 : 1 ;  //     1     
	UINT32 intr_i2_intr_pulse                 : 1 ;  //     2     
	UINT32 intr_i3_intr_pulse                 : 1 ;  //     3     
	UINT32 intr_i4_intr_pulse                 : 1 ;  //     4     
	UINT32 intr_i5_intr_pulse                 : 1 ;  //     5     
	UINT32 intr_i6_intr_pulse                 : 1 ;  //     6     
	UINT32 intr_i7_intr_pulse                 : 1 ;  //     7     
	UINT32 intr_g0_pic_end                    : 1 ;  //     8     
	UINT32 intr_g1_pic_end                    : 1 ;  //     9     
	UINT32 intr_g2_pic_end                    : 1 ;  //    10     
	UINT32 intr_g3_pic_end                    : 1 ;  //    11     
	UINT32 intr_g4_pic_end                    : 1 ;  //    12     
	UINT32 intr_g5_pic_end                    : 1 ;  //    13     
	UINT32 intr_g6_pic_end                    : 1 ;  //    14     
	UINT32 intr_g7_pic_end                    : 1 ;  //    15     
	UINT32 intr_nds                           : 1 ;  //    16     
	UINT32 intr_cve                           : 1 ;  //    17  
	UINT32 intr_cve_va_fal                    : 1 ;  //    18
	UINT32 intr_cve_va_ris                    : 1 ;  //    19
} H15A0_SUB_INTR_REG_T;

/*-----------------------------------------------------------------------------
		0xc8006014L intr_mask ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mask_i0_src_pulse                  : 1 ;  //     0     
	UINT32 mask_i1_src_pulse                  : 1 ;  //     1     
	UINT32 mask_i2_src_pulse                  : 1 ;  //     2     
	UINT32 mask_i3_src_pulse                  : 1 ;  //     3     
	UINT32 mask_i4_src_pulse                  : 1 ;  //     4     
	UINT32 mask_i5_src_pulse                  : 1 ;  //     5     
	UINT32 mask_i6_src_pulse                  : 1 ;  //     6     
	UINT32 mask_i7_src_pulse                  : 1 ;  //     7     
	UINT32 mask_g0_pic_end                    : 1 ;  //     8     
	UINT32 mask_g1_pic_end                    : 1 ;  //     9     
	UINT32 mask_g2_pic_end                    : 1 ;  //    10     
	UINT32 mask_g3_pic_end                    : 1 ;  //    11     
	UINT32 mask_g4_pic_end                    : 1 ;  //    12     
	UINT32 mask_g5_pic_end                    : 1 ;  //    13     
	UINT32 mask_g6_pic_end                    : 1 ;  //    14     
	UINT32 mask_g7_pic_end                    : 1 ;  //    15     
	UINT32 mask_nds                           : 1 ;  //    16     
	UINT32                                    : 1 ;  //    17    reserved
	UINT32 mask_cve_va_fal                    : 1 ;  //    18
	UINT32 mask_cve_va_ris                    : 1 ;  //    19
} H15A0_SUB_INTR_MASK_T;

/*-----------------------------------------------------------------------------
		0xc8006018L intr_mux ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mux_i0_src_pulse                   : 1 ;  //     0     
	UINT32 mux_i1_src_pulse                   : 1 ;  //     1     
	UINT32 mux_i2_src_pulse                   : 1 ;  //     2     
	UINT32 mux_i3_src_pulse                   : 1 ;  //     3     
	UINT32 mux_i4_src_pulse                   : 1 ;  //     4     
	UINT32 mux_i5_src_pulse                   : 1 ;  //     5     
	UINT32 mux_i6_src_pulse                   : 1 ;  //     6     
	UINT32 mux_i7_src_pulse                   : 1 ;  //     7     
	UINT32 mux_g0_pic_end                     : 1 ;  //     8     
	UINT32 mux_g1_pic_end                     : 1 ;  //     9     
	UINT32 mux_g2_pic_end                     : 1 ;  //    10     
	UINT32 mux_g3_pic_end                     : 1 ;  //    11     
	UINT32 mux_g4_pic_end                     : 1 ;  //    12     
	UINT32 mux_g5_pic_end                     : 1 ;  //    13     
	UINT32 mux_g6_pic_end                     : 1 ;  //    14     
	UINT32 mux_g7_pic_end                     : 1 ;  //    15     
	UINT32 mux_nds                            : 1 ;  //    16
	UINT32                                    : 1 ;  //    17    reserved
	UINT32 mux_cve_va_fal                     : 1 ;  //    18
	UINT32 mux_cve_va_ris                     : 1 ;  //    19
} H15A0_SUB_INTR_MUX_T;

typedef struct {
	H15A0_CVC_PIC_INIT_T                         	pic_init;                     //0xc8000000L
	H15A0_CVC_PIC_START_T                        	pic_start;                    //0xc8000004L
	H15A0_CVC_CTRL_AUTO_INIT_T                   	ctrl_auto_init;               //0xc8000008L
	H15A0_CVC_CTRL_INTR_PULSE_T                  	ctrl_intr_pulse;              //0xc800000cL
	H15A0_CVC_INTR_REG_T                         	intr_reg;                     //0xc8000010L
	H15A0_CVC_INTR_MASK_T                        	intr_mask;                    //0xc8000014L
	H15A0_CVC_INTR_MUX_T                         	intr_mux;                     //0xc8000018L
	H15A0_CVC_INTR_EN_T                          	intr_en;                      //0xc800001cL
	H15A0_CVC_INFO_CVI0_T                        	info_cvi0;                    //0xc8000020L
	H15A0_CVC_INFO_CVI1_T                        	info_cvi1;                    //0xc8000024L
	H15A0_CVC_INFO_CVI2_T                        	info_cvi2;                    //0xc8000028L
	H15A0_CVC_INFO_CVI3_T                        	info_cvi3;                    //0xc800002cL
	UINT32                                  	reserved00[20];               //0xc8000030L~0xc800007cL
	H15A0_CVC_CTRL_BLOCK_MUX_T                   	ctrl_block_mux;               //0xc8000080L
	UINT32                                  	reserved01[15];                //0xc8000084L~0xc80000bcL
	H15A0_CVC_PIC_INIT_G0_TIMER_T                	pic_init_g0_timer;            //0xc80000c0L
	H15A0_CVC_PIC_INIT_G1_TIMER_T                	pic_init_g1_timer;            //0xc80000c4L
	H15A0_CVC_PIC_INIT_G2_TIMER_T                	pic_init_g2_timer;            //0xc80000c8L
	H15A0_CVC_PIC_INIT_G3_TIMER_T                	pic_init_g3_timer;            //0xc80000ccL
	H15A0_CVC_PIC_INIT_G4_TIMER_T                	pic_init_g4_timer;            //0xc80000d0L
	H15A0_CVC_PIC_INIT_G5_TIMER_T                	pic_init_g5_timer;            //0xc80000d4L
	H15A0_CVC_PIC_INIT_G6_TIMER_T                	pic_init_g6_timer;            //0xc80000d8L
	H15A0_CVC_PIC_INIT_G7_TIMER_T                	pic_init_g7_timer;            //0xc80000dcL
	UINT32                                  	reserved02;                   //0xc80000e0L
	H15A0_CVC_DBG_TIMER_RUN_T                    	dbg_timer_run;                //0xc80000e4L
	H15A0_CVC_DBG_TIMER_BLK_T                    	dbg_timer_blk;                //0xc80000e8L
	H15A0_CVC_CTRL_MONITOR_T                     	ctrl_monitor;                 //0xc80000ecL
	H15A0_CVC_CTRL_PIC_END_T                     	ctrl_pic_end;                 //0xc80000f0L
	H15A0_CVC_INFO_PIC_END_T                     	info_pic_end;                 //0xc80000f4L
	H15A0_CVC_CTRL_GPIO_T                        	ctrl_gpio;                    //0xc80000f8L
	H15A0_CVC_CTRL_GMAU_T                        	ctrl_gmau;                    //0xc80000fcL
} DE_CVC_REG_H15A0_T;

typedef struct {
	H15A0_VSD_PIC_INIT_T                         	pic_init;                     //0xc800b000L
	H15A0_VSD_PIC_START_T                        	pic_start;                    //0xc800b004L
	H15A0_VSD_CTRL_AUTO_INIT_T                   	ctrl_auto_init;               //0xc800b008L
	H15A0_VSD_CTRL_INTR_PULSE_T                  	ctrl_intr_pulse;              //0xc800b00cL
	H15A0_VSD_INTR_REG_T                         	intr_reg;                     //0xc800b010L
	H15A0_VSD_INTR_MASK_T                        	intr_mask;                    //0xc800b014L
	H15A0_VSD_INTR_MUX_T                         	intr_mux;                     //0xc800b018L
	H15A0_VSD_INTR_EN_T                          	intr_en;                      //0xc800b01cL
	H15A0_VSD_CTRL_SYNC_GEN_MV0_T                	ctrl_sync_gen_mv0;            //0xc800b020L
	H15A0_VSD_CTRL_SYNC_GEN_MV1_T                	ctrl_sync_gen_mv1;            //0xc800b024L
	H15A0_VSD_CTRL_SYNC_GEN_MV2_T                	ctrl_sync_gen_mv2;            //0xc800b028L
	H15A0_VSD_CTRL_SYNC_GEN_MV3_T                	ctrl_sync_gen_mv3;            //0xc800b02cL
	H15A0_VSD_CTRL_SYNC_GEN_MV4_T                	ctrl_sync_gen_mv4;            //0xc800b030L
	H15A0_VSD_CTRL_SYNC_GEN_MV5_T                	ctrl_sync_gen_mv5;            //0xc800b034L
	H15A0_VSD_CTRL_SYNC_GEN_MV6_T                	ctrl_sync_gen_mv6;            //0xc800b038L
	UINT32                                  	reserved00;                   //0xc800b03cL
	H15A0_VSD_CTRL_SYNC_GEN_SO0_T                	ctrl_sync_gen_so0;            //0xc800b040L
	H15A0_VSD_CTRL_SYNC_GEN_SO1_T                	ctrl_sync_gen_so1;            //0xc800b044L
	H15A0_VSD_CTRL_SYNC_GEN_SO2_T                	ctrl_sync_gen_so2;            //0xc800b048L
	H15A0_VSD_CTRL_SYNC_GEN_SO3_T                	ctrl_sync_gen_so3;            //0xc800b04cL
	H15A0_VSD_CTRL_SYNC_GEN_SO4_T                	ctrl_sync_gen_so4;            //0xc800b050L
	H15A0_VSD_CTRL_SYNC_GEN_SO5_T                	ctrl_sync_gen_so5;            //0xc800b054L
	H15A0_VSD_CTRL_SYNC_GEN_SO6_T                	ctrl_sync_gen_so6;            //0xc800b058L
	UINT32                                  	reserved01;                   //0xc800b05cL
	H15A0_VSD_CTRL_SYNC_GEN_HT0_T                	ctrl_sync_gen_ht0;            //0xc800b060L
	H15A0_VSD_CTRL_SYNC_GEN_HT1_T                	ctrl_sync_gen_ht1;            //0xc800b064L
	H15A0_VSD_CTRL_SYNC_GEN_HT2_T                	ctrl_sync_gen_ht2;            //0xc800b068L
	H15A0_VSD_CTRL_SYNC_GEN_HT3_T                	ctrl_sync_gen_ht3;            //0xc800b06cL
	H15A0_VSD_CTRL_SYNC_GEN_HT4_T                	ctrl_sync_gen_ht4;            //0xc800b070L
	H15A0_VSD_CTRL_SYNC_GEN_HT5_T                	ctrl_sync_gen_ht5;            //0xc800b074L
	H15A0_VSD_CTRL_SYNC_GEN_HT6_T                	ctrl_sync_gen_ht6;            //0xc800b078L
	UINT32                                  	reserved02;                   //0xc800b07cL
	H15A0_VSD_INFO_DCO_MFC_T                     	info_dco_mfc;                 //0xc800b080L
	H15A0_VSD_INFO_DCO_DFC_T                     	info_dco_dfc;                 //0xc800b084L
	H15A0_VSD_INFO_DCO_DFLC_T                    	info_dco_dflc;                //0xc800b088L
	H15A0_VSD_CTRL_DCO_NUM_T                     	ctrl_dco_num;                 //0xc800b08cL
	H15A0_VSD_CTRL_DCO_FCW_T                     	ctrl_dco_fcw;                 //0xc800b090L
	UINT32                                  	reserved03[3];                //0xc800b094L~0xc800b09cL
	H15A0_VSD_CTRL_SR_PIC_INIT_T                 	ctrl_sr_pic_init;             //0xc800b0a0L
	H15A0_VSD_PIC_INIT_SR_TIMER_T                	pic_init_sr_timer;            //0xc800b0a4L
	H15A0_VSD_CTRL_HT_PIC_INIT_T                 	ctrl_ht_pic_init;             //0xc800b0a8L
	H15A0_VSD_PIC_INIT_HT_TIMER_T                	pic_init_ht_timer;            //0xc800b0acL
	UINT32                                  	reserved04[4];                //0xc800b0b0L~0xc800b0bcL
	H15A0_VSD_PIC_INIT_G0_TIMER_T                	pic_init_g0_timer;            //0xc800b0c0L
	H15A0_VSD_PIC_INIT_G1_TIMER_T                	pic_init_g1_timer;            //0xc800b0c4L
	H15A0_VSD_PIC_INIT_G2_TIMER_T                	pic_init_g2_timer;            //0xc800b0c8L
	H15A0_VSD_PIC_INIT_G3_TIMER_T                	pic_init_g3_timer;            //0xc800b0ccL
	H15A0_VSD_PIC_INIT_G4_TIMER_T                	pic_init_g4_timer;            //0xc800b0d0L
	H15A0_VSD_PIC_INIT_G5_TIMER_T                	pic_init_g5_timer;            //0xc800b0d4L
	H15A0_VSD_PIC_INIT_G6_TIMER_T                	pic_init_g6_timer;            //0xc800b0d8L
	H15A0_VSD_PIC_INIT_G7_TIMER_T                	pic_init_g7_timer;            //0xc800b0dcL
	UINT32                                  	reserved05;                   //0xc800b0e0L
	H15A0_VSD_DBG_TIMER_RUN_T                    	dbg_timer_run;                //0xc800b0e4L
	H15A0_VSD_DBG_TIMER_BLK_T                    	dbg_timer_blk;                //0xc800b0e8L
	H15A0_VSD_CTRL_MONITOR_T                     	ctrl_monitor;                 //0xc800b0ecL
	H15A0_VSD_CTRL_PIC_END_T                     	ctrl_pic_end;                 //0xc800b0f0L
	H15A0_VSD_INFO_PIC_END_T                     	info_pic_end;                 //0xc800b0f4L
	H15A0_VSD_CTRL_GPIO_T                        	ctrl_gpio;                    //0xc800b0f8L
	H15A0_VSD_CTRL_GMAU_T                        	ctrl_gmau;                    //0xc800b0fcL
} DE_VSD_REG_H15A0_T;

typedef struct {
	H15A0_CCO_PIC_INIT_T                         	pic_init;                     //0xc8024000L
	H15A0_CCO_PIC_START_T                        	pic_start;                    //0xc8024004L
	H15A0_CCO_CTRL_AUTO_INIT_T                   	ctrl_auto_init;               //0xc8024008L
	H15A0_CCO_CTRL_INTR_PULSE_T                  	ctrl_intr_pulse;              //0xc802400cL
	H15A0_CCO_INTR_REG_T                         	intr_reg;                     //0xc8024010L
	H15A0_CCO_INTR_MASK_T                        	intr_mask;                    //0xc8024014L
	H15A0_CCO_INTR_MUX_T                         	intr_mux;                     //0xc8024018L
	H15A0_CCO_INTR_EN_T                          	intr_en;                      //0xc802401cL
	H15A0_CCO_CTRL_DPATH_T                       	ctrl_dpath;                   //0xc8024020L
	H15A0_CCO_CTRL_SPATH_T                       	ctrl_spath;                   //0xc8024024L
	H15A0_CCO_CTRL_3D_FLAG_T                     	ctrl_3d_flag;                 //0xc8024028L
	UINT32                                  	reserved00;                   //0xc802402cL
	H15A0_CCO_CTRL_BOSD_PULSE_T                  	ctrl_bosd_pulse;              //0xc8024030L
	H15A0_CCO_CTRL_SOSD_PULSE_T                  	ctrl_sosd_pulse;              //0xc8024034L
	H15A0_CCO_CTRL_AUTO_INIT_AFBC_T              	ctrl_auto_init_afbc;          //0xc8024038L
	H15A0_CCO_CTRL_AUTO_INIT_LENCON_T            	ctrl_auto_init_lencon;        //0xc802403cL
	UINT32                                  	reserved01;                   //0xc8024040L
	H15A0_CCO_CTRL_OSD_INIT_2ND_T                	ctrl_osd_init_2nd;            //0xc8024044L
	UINT32                                  	reserved02[6];                //0xc8024048L~0xc802405cL
	H15A0_CCO_CTRL_FBDC0_T                       	ctrl_fbdc0;                   //0xc8024060L
	H15A0_CCO_INFO_FBDC0_DBG_T                   	info_fbdc0_dbg;               //0xc8024064L
	H15A0_CCO_CTRL_FBDC1_T                       	ctrl_fbdc1;                   //0xc8024068L
	H15A0_CCO_INFO_FBDC1_DBG_T                   	info_fbdc1_dbg;               //0xc802406cL
	H15A0_CCO_CTRL_DLY_DISP_SYNC_PARAM0_T        	ctrl_dly_disp_sync_param0;    //0xc8024070L
	H15A0_CCO_CTRL_DLY_DISP_SYNC_PARAM1_T        	ctrl_dly_disp_sync_param1;    //0xc8024074L
	H15A0_CCO_CTRL_DLY_DISP_SYNC_PARAM2_T        	ctrl_dly_disp_sync_param2;    //0xc8024078L
	H15A0_CCO_CTRL_DLY_DISP_SYNC_PARAM3_T        	ctrl_dly_disp_sync_param3;    //0xc802407cL
	H15A0_CCO_CTRL_DLY_DISP_SYNC_PARAM4_T        	ctrl_dly_disp_sync_param4;    //0xc8024080L
	H15A0_CCO_CTRL_DLY_DISP_SYNC_PARAM5_T        	ctrl_dly_disp_sync_param5;    //0xc8024084L
	H15A0_CCO_CTRL_DLY_DISP_SYNC_PARAM6_T        	ctrl_dly_disp_sync_param6;    //0xc8024088L
	H15A0_CCO_CTRL_DLY_DISP_SYNC_PARAM7_T        	ctrl_dly_disp_sync_param7;    //0xc802408cL
	H15A0_CCO_CTRL_DLY_DISP_SYNC_PARAM8_T        	ctrl_dly_disp_sync_param8;    //0xc8024090L
	H15A0_CCO_CTRL_DLY_DISP_SYNC_PARAM9_T        	ctrl_dly_disp_sync_param9;    //0xc8024094L
	H15A0_CCO_CTRL_DLY_DISP_SYNC_PARAMA_T        	ctrl_dly_disp_sync_parama;    //0xc8024098L
	H15A0_CCO_CTRL_DLY_DISP_SYNC_PARAMB_T        	ctrl_dly_disp_sync_paramb;    //0xc802409cL
	H15A0_CCO_OSD_SYNC_GEN_CTRL0_T               	osd_sync_gen_ctrl0;           //0xc80240a0L
	H15A0_CCO_OSD_SYNC_GEN_CTRL1_T               	osd_sync_gen_ctrl1;           //0xc80240a4L
	H15A0_CCO_OSD_SYNC_GEN_CTRL2_T               	osd_sync_gen_ctrl2;           //0xc80240a8L
	H15A0_CCO_OSD_SYNC_GEN_CTRL3_T               	osd_sync_gen_ctrl3;           //0xc80240acL
	H15A0_CCO_OSD_SYNC_GEN_CTRL4_T               	osd_sync_gen_ctrl4;           //0xc80240b0L
	H15A0_CCO_OSD_SYNC_GEN_CTRL5_T               	osd_sync_gen_ctrl5;           //0xc80240b4L
	H15A0_CCO_OSD_SYNC_GEN_CTRL6_T               	osd_sync_gen_ctrl6;           //0xc80240b8L
	H15A0_CCO_OSD_SYNC_GEN_CTRL7_T               	osd_sync_gen_ctrl7;           //0xc80240bcL
	H15A0_CCO_PIC_INIT_G0_TIMER_T                	pic_init_g0_timer;            //0xc80240c0L
	H15A0_CCO_PIC_INIT_G1_TIMER_T                	pic_init_g1_timer;            //0xc80240c4L
	H15A0_CCO_PIC_INIT_G2_TIMER_T                	pic_init_g2_timer;            //0xc80240c8L
	H15A0_CCO_PIC_INIT_G3_TIMER_T                	pic_init_g3_timer;            //0xc80240ccL
	H15A0_CCO_PIC_INIT_G4_TIMER_T                	pic_init_g4_timer;            //0xc80240d0L
	H15A0_CCO_PIC_INIT_G5_TIMER_T                	pic_init_g5_timer;            //0xc80240d4L
	H15A0_CCO_PIC_INIT_G6_TIMER_T                	pic_init_g6_timer;            //0xc80240d8L
	H15A0_CCO_PIC_INIT_G7_TIMER_T                	pic_init_g7_timer;            //0xc80240dcL
	UINT32                                  	reserved03;                   //0xc80240e0L
	H15A0_CCO_DBG_TIMER_RUN_T                    	dbg_timer_run;                //0xc80240e4L
	H15A0_CCO_DBG_TIMER_BLK_T                    	dbg_timer_blk;                //0xc80240e8L
	H15A0_CCO_CTRL_MONITOR_T                     	ctrl_monitor;                 //0xc80240ecL
	H15A0_CCO_CTRL_PIC_END_T                     	ctrl_pic_end;                 //0xc80240f0L
	H15A0_CCO_INFO_PIC_END_T                     	info_pic_end;                 //0xc80240f4L
	H15A0_CCO_CTRL_GPIO_T                        	ctrl_gpio;                    //0xc80240f8L
	H15A0_CCO_CTRL_GMAU_T                        	ctrl_gmau;                    //0xc80240fcL
} DE_CCO_REG_H15A0_T;

typedef struct {
	H15A0_IMX_PIC_INIT_T                         	pic_init;                     //0xc8001000L
	H15A0_IMX_PIC_START_T                        	pic_start;                    //0xc8001004L
	H15A0_IMX_CTRL_AUTO_INIT_T                   	ctrl_auto_init;               //0xc8001008L
	H15A0_IMX_CTRL_INTR_PULSE_T                  	ctrl_intr_pulse;              //0xc800100cL
	H15A0_IMX_INTR_REG_T                         	intr_reg;                     //0xc8001010L
	H15A0_IMX_INTR_MASK_T                        	intr_mask;                    //0xc8001014L
	H15A0_IMX_INTR_MUX_T                         	intr_mux;                     //0xc8001018L
	H15A0_IMX_INTR_EN_T                          	intr_en;                      //0xc800101cL
	H15A0_IMX_S0_SRC_SYNC_MUX_CTRL_T             	s0_src_sync_mux_ctrl;         //0xc8001020L
	H15A0_IMX_S0_SRC_SYNC_DLY_T                  	s0_src_sync_dly;              //0xc8001024L
	H15A0_IMX_S0_SRC_SYNC_DLY_LINE_T             	s0_src_sync_dly_line;         //0xc8001028L
	H15A0_IMX_S1_SRC_SYNC_MUX_CTRL_T             	s1_src_sync_mux_ctrl;         //0xc800102cL
	H15A0_IMX_S1_SRC_SYNC_DLY_T                  	s1_src_sync_dly;              //0xc8001030L
	H15A0_IMX_S1_SRC_SYNC_DLY_LINE_T             	s1_src_sync_dly_line;         //0xc8001034L
	H15A0_IMX_S2_SRC_SYNC_MUX_CTRL_T             	s2_src_sync_mux_ctrl;         //0xc8001038L
	H15A0_IMX_S2_SRC_SYNC_DLY_T                  	s2_src_sync_dly;              //0xc800103cL
	H15A0_IMX_S2_SRC_SYNC_DLY_LINE_T             	s2_src_sync_dly_line;         //0xc8001040L
	H15A0_IMX_S3_SRC_SYNC_MUX_CTRL_T             	s3_src_sync_mux_ctrl;         //0xc8001044L
	H15A0_IMX_S3_SRC_SYNC_DLY_T                  	s3_src_sync_dly;              //0xc8001048L
	H15A0_IMX_S3_SRC_SYNC_DLY_LINE_T             	s3_src_sync_dly_line;         //0xc800104cL
	H15A0_IMX_S4_SRC_SYNC_MUX_CTRL_T             	s4_src_sync_mux_ctrl;         //0xc8001050L
	H15A0_IMX_S4_SRC_SYNC_DLY_T                  	s4_src_sync_dly;              //0xc8001054L
	H15A0_IMX_S4_SRC_SYNC_DLY_LINE_T             	s4_src_sync_dly_line;         //0xc8001058L
	H15A0_IMX_S5_SRC_SYNC_MUX_CTRL_T             	s5_src_sync_mux_ctrl;         //0xc800105cL
	H15A0_IMX_S5_SRC_SYNC_DLY_T                  	s5_src_sync_dly;              //0xc8001060L
	H15A0_IMX_S5_SRC_SYNC_DLY_LINE_T             	s5_src_sync_dly_line;         //0xc8001064L
	H15A0_IMX_S6_SRC_SYNC_MUX_CTRL_T             	s6_src_sync_mux_ctrl;         //0xc8001068L
	H15A0_IMX_S6_SRC_SYNC_DLY_T                  	s6_src_sync_dly;              //0xc800106cL
	H15A0_IMX_S6_SRC_SYNC_DLY_LINE_T             	s6_src_sync_dly_line;         //0xc8001070L
	H15A0_IMX_S7_SRC_SYNC_MUX_CTRL_T             	s7_src_sync_mux_ctrl;         //0xc8001074L
	H15A0_IMX_S7_SRC_SYNC_DLY_T                  	s7_src_sync_dly;              //0xc8001078L
	H15A0_IMX_S7_SRC_SYNC_DLY_LINE_T             	s7_src_sync_dly_line;         //0xc800107cL
	H15A0_IMX_MEM0_SYNC_PERIOD_T                 	mem0_sync_period;             //0xc8001080L
	H15A0_IMX_MEM1_SYNC_PERIOD_T                 	mem1_sync_period;             //0xc8001084L
	H15A0_IMX_MEM2_SYNC_PERIOD_T                 	mem2_sync_period;             //0xc8001088L
	H15A0_IMX_MEM3_SYNC_PERIOD_T                 	mem3_sync_period;             //0xc800108cL
	H15A0_IMX_MEM4_SYNC_PERIOD_T                 	mem4_sync_period;             //0xc8001090L
	H15A0_IMX_MEM5_SYNC_PERIOD_T                 	mem5_sync_period;             //0xc8001094L
	H15A0_IMX_MEM6_SYNC_PERIOD_T                 	mem6_sync_period;             //0xc8001098L
	H15A0_IMX_MEM7_SYNC_PERIOD_T                 	mem7_sync_period;             //0xc800109cL
	H15A0_IMX_CTRL_SRC_DCO_MFC_T                 	ctrl_src_dco_mfc;             //0xc80010a0L
	H15A0_IMX_CTRL_CVI_SRC_OF_SMUX_T             	ctrl_cvi_src_of_smux;         //0xc80010a4L
	H15A0_IMX_CTRL_MVI_T_SRC_OF_SMUX_T           	ctrl_mvi_t_src_of_smux;       //0xc80010a8L
	H15A0_IMX_CTRL_MVI_R_SRC_OF_SMUX_T           	ctrl_mvi_r_src_of_smux;       //0xc80010acL
	H15A0_IMX_CTRL_BLOCK_MUX_T                   	ctrl_block_mux;               //0xc80010b0L
	UINT32                                  	reserved00[3];                //0xc80010b4L~0xc80010bcL
	H15A0_IMX_PIC_INIT_G0_TIMER_T                	pic_init_g0_timer;            //0xc80010c0L
	H15A0_IMX_PIC_INIT_G1_TIMER_T                	pic_init_g1_timer;            //0xc80010c4L
	H15A0_IMX_PIC_INIT_G2_TIMER_T                	pic_init_g2_timer;            //0xc80010c8L
	H15A0_IMX_PIC_INIT_G3_TIMER_T                	pic_init_g3_timer;            //0xc80010ccL
	H15A0_IMX_PIC_INIT_G4_TIMER_T                	pic_init_g4_timer;            //0xc80010d0L
	H15A0_IMX_PIC_INIT_G5_TIMER_T                	pic_init_g5_timer;            //0xc80010d4L
	H15A0_IMX_PIC_INIT_G6_TIMER_T                	pic_init_g6_timer;            //0xc80010d8L
	H15A0_IMX_PIC_INIT_G7_TIMER_T                	pic_init_g7_timer;            //0xc80010dcL
	UINT32                                  	reserved01;                   //0xc80010e0L
	H15A0_IMX_DBG_TIMER_RUN_T                    	dbg_timer_run;                //0xc80010e4L
	H15A0_IMX_DBG_TIMER_BLK_T                    	dbg_timer_blk;                //0xc80010e8L
	H15A0_IMX_CTRL_MONITOR_T                     	ctrl_monitor;                 //0xc80010ecL
	H15A0_IMX_CTRL_PIC_END_T                     	ctrl_pic_end;                 //0xc80010f0L
	H15A0_IMX_INFO_PIC_END_T                     	info_pic_end;                 //0xc80010f4L
	H15A0_IMX_CTRL_GPIO_T                        	ctrl_gpio;                    //0xc80010f8L
	H15A0_IMX_CTRL_GMAU_T                        	ctrl_gmau;                    //0xc80010fcL
} DE_IMX_REG_H15A0_T;

typedef struct {
	H15A0_ND0_PIC_INIT_T                         	pic_init;                     //0xc8004000L
	H15A0_ND0_PIC_START_T                        	pic_start;                    //0xc8004004L
	H15A0_ND0_CTRL_AUTO_INIT_T                   	ctrl_auto_init;               //0xc8004008L
	H15A0_ND0_CTRL_INTR_PULSE_T                  	ctrl_intr_pulse;              //0xc800400cL
	H15A0_ND0_INTR_REG_T                         	intr_reg;                     //0xc8004010L
	H15A0_ND0_INTR_MASK_T                        	intr_mask;                    //0xc8004014L
	H15A0_ND0_INTR_MUX_T                         	intr_mux;                     //0xc8004018L
	H15A0_ND0_INTR_EN_T                          	intr_en;                      //0xc800401cL
	UINT32                                  	reserved00[36];               //0xc8004020L~0xc80040acL
	H15A0_ND0_CTRL_BLOCK_MUX_T                   	ctrl_block_mux;               //0xc80040b0L
	UINT32                                  	reserved01[3];                //0xc80040b4L~0xc80040bcL
	H15A0_ND0_PIC_INIT_G0_TIMER_T                	pic_init_g0_timer;            //0xc80040c0L
	H15A0_ND0_PIC_INIT_G1_TIMER_T                	pic_init_g1_timer;            //0xc80040c4L
	H15A0_ND0_PIC_INIT_G2_TIMER_T                	pic_init_g2_timer;            //0xc80040c8L
	H15A0_ND0_PIC_INIT_G3_TIMER_T                	pic_init_g3_timer;            //0xc80040ccL
	H15A0_ND0_PIC_INIT_G4_TIMER_T                	pic_init_g4_timer;            //0xc80040d0L
	H15A0_ND0_PIC_INIT_G5_TIMER_T                	pic_init_g5_timer;            //0xc80040d4L
	H15A0_ND0_PIC_INIT_G6_TIMER_T                	pic_init_g6_timer;            //0xc80040d8L
	H15A0_ND0_PIC_INIT_G7_TIMER_T                	pic_init_g7_timer;            //0xc80040dcL
	UINT32                                  	reserved02;                   //0xc80040e0L
	H15A0_ND0_DBG_TIMER_RUN_T                    	dbg_timer_run;                //0xc80040e4L
	H15A0_ND0_DBG_TIMER_BLK_T                    	dbg_timer_blk;                //0xc80040e8L
	H15A0_ND0_CTRL_MONITOR_T                     	ctrl_monitor;                 //0xc80040ecL
	H15A0_ND0_CTRL_PIC_END_T                     	ctrl_pic_end;                 //0xc80040f0L
	H15A0_ND0_INFO_PIC_END_T                     	info_pic_end;                 //0xc80040f4L
	H15A0_ND0_CTRL_GPIO_T                        	ctrl_gpio;                    //0xc80040f8L
	H15A0_ND0_CTRL_GMAU_T                        	ctrl_gmau;                    //0xc80040fcL
} DE_ND0_REG_H15A0_T;

typedef struct {
	H15A0_ND1_PIC_INIT_T                         	pic_init;                     //0xc8005000L
	H15A0_ND1_PIC_START_T                        	pic_start;                    //0xc8005004L
	H15A0_ND1_CTRL_AUTO_INIT_T                   	ctrl_auto_init;               //0xc8005008L
	H15A0_ND1_CTRL_INTR_PULSE_T                  	ctrl_intr_pulse;              //0xc800500cL
	H15A0_ND1_INTR_REG_T                         	intr_reg;                     //0xc8005010L
	H15A0_ND1_INTR_MASK_T                        	intr_mask;                    //0xc8005014L
	H15A0_ND1_INTR_MUX_T                         	intr_mux;                     //0xc8005018L
	H15A0_ND1_INTR_EN_T                          	intr_en;                      //0xc800501cL
	UINT32                                  	reserved00[36];               //0xc8005020L~0xc80050acL
	H15A0_ND1_CTRL_BLOCK_MUX_T                   	ctrl_block_mux;               //0xc80050b0L
	UINT32                                  	reserved01[3];                //0xc80050b4L~0xc80050bcL
	H15A0_ND1_PIC_INIT_G0_TIMER_T                	pic_init_g0_timer;            //0xc80050c0L
	H15A0_ND1_PIC_INIT_G1_TIMER_T                	pic_init_g1_timer;            //0xc80050c4L
	H15A0_ND1_PIC_INIT_G2_TIMER_T                	pic_init_g2_timer;            //0xc80050c8L
	H15A0_ND1_PIC_INIT_G3_TIMER_T                	pic_init_g3_timer;            //0xc80050ccL
	H15A0_ND1_PIC_INIT_G4_TIMER_T                	pic_init_g4_timer;            //0xc80050d0L
	H15A0_ND1_PIC_INIT_G5_TIMER_T                	pic_init_g5_timer;            //0xc80050d4L
	H15A0_ND1_PIC_INIT_G6_TIMER_T                	pic_init_g6_timer;            //0xc80050d8L
	H15A0_ND1_PIC_INIT_G7_TIMER_T                	pic_init_g7_timer;            //0xc80050dcL
	UINT32                                  	reserved02;                   //0xc80050e0L
	H15A0_ND1_DBG_TIMER_RUN_T                    	dbg_timer_run;                //0xc80050e4L
	H15A0_ND1_DBG_TIMER_BLK_T                    	dbg_timer_blk;                //0xc80050e8L
	H15A0_ND1_CTRL_MONITOR_T                     	ctrl_monitor;                 //0xc80050ecL
	H15A0_ND1_CTRL_PIC_END_T                     	ctrl_pic_end;                 //0xc80050f0L
	H15A0_ND1_INFO_PIC_END_T                     	info_pic_end;                 //0xc80050f4L
	H15A0_ND1_CTRL_GPIO_T                        	ctrl_gpio;                    //0xc80050f8L
	H15A0_ND1_CTRL_GMAU_T                        	ctrl_gmau;                    //0xc80050fcL
} DE_ND1_REG_H15A0_T;

typedef struct {
	UINT32                                  		reserved00[3];                //0xc8006000L~0xc8006008L
	H15A0_SUB_CTRL_INTR_PULSE_T                  	ctrl_intr_pulse;              //0xc800600cL
	H15A0_SUB_INTR_REG_T                         	intr_reg;                     //0xc8006010L
	H15A0_SUB_INTR_MASK_T                        	intr_mask;                    //0xc8006014L
	H15A0_SUB_INTR_MUX_T                         	intr_mux;                     //0xc8006018L
	UINT32                                  		reserved01[56];               //0xc800601cL~0xc80060fcL	
} DE_SUB_REG_H15A0_T;

#endif
