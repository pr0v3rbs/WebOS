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

#ifndef _REG_SMX_H14A0_h
#define _REG_SMX_H14A0_h

/*-----------------------------------------------------------------------------
	0x0a00 sml_smux_ctrl0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 o_c_fmt                     : 3;   //  0: 2
	UINT32 prew_rd_422                 : 1;   //     3
	UINT32 i_c_fmt                     : 3;   //  4: 6
	UINT32                             : 1;   //     7 reserved
	UINT32 dec3d_mux_sel               : 1;   //     8
	UINT32                             : 3;   //  9:11 reserved
	UINT32 prew_mux_sel                : 1;   //    12
	UINT32 dlr_sel                     : 1;   //    13
	UINT32                             : 2;   // 14:15 reserved
	UINT32 cvi_mux_sel                 : 3;   // 16:18
	UINT32                             : 1;   //    19 reserved
	UINT32 nline_inc                   : 1;   //    20
	UINT32                             : 2;   // 21:22 reserved
	UINT32 wr_disalbe                  : 1;   //    23
	UINT32 fieldid                     : 1;   //    24
	UINT32 size                        : 1;   //    25
	UINT32 set_mvi_val                 : 1;   //    26
	UINT32                             : 1;   //    27 reserved
	UINT32 mvi_r                       : 1;   //    28
	UINT32 mvi_w                       : 1;   //    29
	UINT32                             : 1;   //    30 reserved
	UINT32 pr_sq                       : 1;   //    31
} H14A0_SML_SMUX_CTRL0_T;

/*-----------------------------------------------------------------------------
	0x0a04 sml_smux_ctrl1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 sel_buf_size                : 2;   //  0: 1
	UINT32                             : 2;   //  2: 3 reserved
	UINT32 ud_split_mode               : 1;   //     4
	UINT32                             : 3;   //  5: 7 reserved
	UINT32 debug_mode                  : 1;   //     8
	UINT32                             : 3;   //  9:11 reserved
	UINT32 pic_end_sel                 : 1;   //    12
	UINT32 hsc_enable                  : 1;   //    13
	UINT32 hsc_back                    : 1;   //    14
	UINT32 gos_enable                  : 1;   //    15
	UINT32 line_diff_en                : 1;   //    16
	UINT32                             : 3;   // 17:19 reserved
	UINT32 line_diff                   : 12;  // 20:31
} H14A0_SML_SMUX_CTRL1_T;

/*-----------------------------------------------------------------------------
	0x0a08 sml_smux_ctrl2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_req_pend_val              : 12;  //  0:11
	UINT32                             : 4;   // 12:15 reserved
	UINT32 c_req_pend_val              : 12;  // 16:27
} H14A0_SML_SMUX_CTRL2_T;

/*-----------------------------------------------------------------------------
	0x0a0c sml_smux_ctrld ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 d_o_c_fmt                   : 3;   //  0: 2
	UINT32                             : 1;   //     3 reserved
	UINT32 d_i_c_fmt                   : 3;   //  4: 6
	UINT32                             : 9;   //  7:15 reserved
	UINT32 d_ch_mux_sel                : 3;   // 16:18
	UINT32                             : 6;   // 19:24 reserved
	UINT32 d_set_in_size               : 1;   //    25
	UINT32 d_set_mvi_val               : 1;   //    26
	UINT32                             : 4;   // 27:30 reserved
	UINT32 d_pr_sq                     : 1;   //    31
} H14A0_SML_SMUX_CTRLD_T;

/*-----------------------------------------------------------------------------
	0x0a10 sml_smux_hsc_y_set_val_0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 phase_offset_y              : 6;   //  0: 5
	UINT32                             : 6;   //  6:11 reserved
	UINT32 aal_coef_0_y                : 10;  // 12:21
} H14A0_SML_SMUX_HSC_Y_SET_VAL_0_T;

/*-----------------------------------------------------------------------------
	0x0a14 sml_smux_hsc_y_set_val_1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 aal_coef_1_y                : 10;  //  0: 9
	UINT32                             : 2;   // 10:11 reserved
	UINT32 aal_coef_2_y                : 10;  // 12:21
} H14A0_SML_SMUX_HSC_Y_SET_VAL_1_T;

/*-----------------------------------------------------------------------------
	0x0a18 sml_smux_hsc_y_set_val_2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 aal_coef_3_y                : 10;  //  0: 9
	UINT32                             : 2;   // 10:11 reserved
	UINT32 aal_coef_4_y                : 10;  // 12:21
} H14A0_SML_SMUX_HSC_Y_SET_VAL_2_T;

/*-----------------------------------------------------------------------------
	0x0a1c sml_smux_hsc_c_set_val_0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 phase_offset_c              : 6;   //  0: 5
	UINT32                             : 6;   //  6:11 reserved
	UINT32 aal_coef_0_c                : 10;  // 12:21
} H14A0_SML_SMUX_HSC_C_SET_VAL_0_T;

/*-----------------------------------------------------------------------------
	0x0a20 sml_smux_hsc_c_set_val_1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 aal_coef_1_c                : 10;  //  0: 9
	UINT32                             : 2;   // 10:11 reserved
	UINT32 aal_coef_2_c                : 10;  // 12:21
} H14A0_SML_SMUX_HSC_C_SET_VAL_1_T;

/*-----------------------------------------------------------------------------
	0x0a24 sml_smux_hsc_c_set_val_2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 aal_coef_3_c                : 10;  //  0: 9
	UINT32                             : 2;   // 10:11 reserved
	UINT32 aal_coef_4_c                : 10;  // 12:21
} H14A0_SML_SMUX_HSC_C_SET_VAL_2_T;

/*-----------------------------------------------------------------------------
	0x0a28 sml_smux_size_i ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 hsize_i                     : 12;  //  0:11
	UINT32                             : 4;   // 12:15 reserved
	UINT32 vsize_i                     : 12;  // 16:27
} H14A0_SML_SMUX_SIZE_I_T;

/*-----------------------------------------------------------------------------
	0x0a2c sml_smux_size_o ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 hsize                       : 12;  //  0:11
	UINT32                             : 4;   // 12:15 reserved
	UINT32 vsize                       : 12;  // 16:27
} H14A0_SML_SMUX_SIZE_O_T;

/*-----------------------------------------------------------------------------
	0x0a30 sml_smux_size_d ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 hsize_d                     : 12;  //  0:11
	UINT32                             : 4;   // 12:15 reserved
	UINT32 vsize_d                     : 12;  // 16:27
} H14A0_SML_SMUX_SIZE_D_T;

/*-----------------------------------------------------------------------------
	0x0a34 sml_smux_hsc_offset ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 hoffset                     : 12;  //  0:11
	UINT32                             : 4;   // 12:15 reserved
	UINT32 voffset                     : 12;  // 16:27
} H14A0_SML_SMUX_HSC_OFFSET_T;

/*-----------------------------------------------------------------------------
	0x0a38 sml_smux_hsc_active ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 hactive                     : 12;  //  0:11
	UINT32                             : 4;   // 12:15 reserved
	UINT32 vactive                     : 12;  // 16:27
} H14A0_SML_SMUX_HSC_ACTIVE_T;

/*-----------------------------------------------------------------------------
	0x0a3c sml_smux_hsc_input_info ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 hsc_i_hsize                 : 12;  //  0:11
	UINT32                             : 4;   // 12:15 reserved
	UINT32 hsc_i_vsize                 : 12;  // 16:27
} H14A0_SML_SMUX_HSC_INPUT_INFO_T;

/*-----------------------------------------------------------------------------
	0x0a40 sml_smux_status ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 smux_ro_lcnt                : 12;  //  0:11
	UINT32                             : 4;   // 12:15 reserved
	UINT32 smux_pic_init_count         : 4;   // 16:19
	UINT32 smux_read_pic_init_count    : 4;   // 20:23
	UINT32 smux_debug_pic_init_count   : 4;   // 24:27
} H14A0_SML_SMUX_STATUS_T;

/*-----------------------------------------------------------------------------
	0x2a00 smr_smux_ctrl0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 o_c_fmt                     : 3;   //  0: 2
	UINT32 prew_rd_422                 : 1;   //     3
	UINT32 i_c_fmt                     : 3;   //  4: 6
	UINT32                             : 1;   //     7 reserved
	UINT32 dec3d_mux_sel               : 1;   //     8
	UINT32                             : 3;   //  9:11 reserved
	UINT32 prew_mux_sel                : 1;   //    12
	UINT32 dlr_sel                     : 1;   //    13
	UINT32                             : 2;   // 14:15 reserved
	UINT32 cvi_mux_sel                 : 3;   // 16:18
	UINT32                             : 1;   //    19 reserved
	UINT32 nline_inc                   : 1;   //    20
	UINT32                             : 2;   // 21:22 reserved
	UINT32 wr_disalbe                  : 1;   //    23
	UINT32 fieldid                     : 1;   //    24
	UINT32 size                        : 1;   //    25
	UINT32 set_mvi_val                 : 1;   //    26
	UINT32                             : 1;   //    27 reserved
	UINT32 mvi_r                       : 1;   //    28
	UINT32 mvi_w                       : 1;   //    29
	UINT32                             : 1;   //    30 reserved
	UINT32 pr_sq                       : 1;   //    31
} H14A0_SMR_SMUX_CTRL0_T;

/*-----------------------------------------------------------------------------
	0x2a04 smr_smux_ctrl1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 sel_buf_size                : 2;   //  0: 1
	UINT32                             : 2;   //  2: 3 reserved
	UINT32 ud_split_mode               : 1;   //     4
	UINT32                             : 3;   //  5: 7 reserved
	UINT32 debug_mode                  : 1;   //     8
	UINT32                             : 3;   //  9:11 reserved
	UINT32 pic_end_sel                 : 1;   //    12
	UINT32 hsc_enable                  : 1;   //    13
	UINT32 hsc_back                    : 1;   //    14
	UINT32 gos_enable                  : 1;   //    15
	UINT32 line_diff_en                : 1;   //    16
	UINT32                             : 3;   // 17:19 reserved
	UINT32 line_diff                   : 12;  // 20:31
} H14A0_SMR_SMUX_CTRL1_T;

/*-----------------------------------------------------------------------------
	0x2a08 smr_smux_ctrl2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_req_pend_val              : 12;  //  0:11
	UINT32                             : 4;   // 12:15 reserved
	UINT32 c_req_pend_val              : 12;  // 16:27
} H14A0_SMR_SMUX_CTRL2_T;

/*-----------------------------------------------------------------------------
	0x2a0c smr_smux_ctrld ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 d_o_c_fmt                   : 3;   //  0: 2
	UINT32                             : 1;   //     3 reserved
	UINT32 d_i_c_fmt                   : 3;   //  4: 6
	UINT32                             : 9;   //  7:15 reserved
	UINT32 d_ch_mux_sel                : 3;   // 16:18
	UINT32                             : 6;   // 19:24 reserved
	UINT32 d_set_in_size               : 1;   //    25
	UINT32 d_set_mvi_val               : 1;   //    26
	UINT32                             : 4;   // 27:30 reserved
	UINT32 d_pr_sq                     : 1;   //    31
} H14A0_SMR_SMUX_CTRLD_T;

/*-----------------------------------------------------------------------------
	0x2a10 smr_smux_hsc_y_set_val_0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 phase_offset_y              : 6;   //  0: 5
	UINT32                             : 6;   //  6:11 reserved
	UINT32 aal_coef_0_y                : 10;  // 12:21
} H14A0_SMR_SMUX_HSC_Y_SET_VAL_0_T;

/*-----------------------------------------------------------------------------
	0x2a14 smr_smux_hsc_y_set_val_1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 aal_coef_1_y                : 10;  //  0: 9
	UINT32                             : 2;   // 10:11 reserved
	UINT32 aal_coef_2_y                : 10;  // 12:21
} H14A0_SMR_SMUX_HSC_Y_SET_VAL_1_T;

/*-----------------------------------------------------------------------------
	0x2a18 smr_smux_hsc_y_set_val_2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 aal_coef_3_y                : 10;  //  0: 9
	UINT32                             : 2;   // 10:11 reserved
	UINT32 aal_coef_4_y                : 10;  // 12:21
} H14A0_SMR_SMUX_HSC_Y_SET_VAL_2_T;

/*-----------------------------------------------------------------------------
	0x2a1c smr_smux_hsc_c_set_val_0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 phase_offset_c              : 6;   //  0: 5
	UINT32                             : 6;   //  6:11 reserved
	UINT32 aal_coef_0_c                : 10;  // 12:21
} H14A0_SMR_SMUX_HSC_C_SET_VAL_0_T;

/*-----------------------------------------------------------------------------
	0x2a20 smr_smux_hsc_c_set_val_1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 aal_coef_1_c                : 10;  //  0: 9
	UINT32                             : 2;   // 10:11 reserved
	UINT32 aal_coef_2_c                : 10;  // 12:21
} H14A0_SMR_SMUX_HSC_C_SET_VAL_1_T;

/*-----------------------------------------------------------------------------
	0x2a24 smr_smux_hsc_c_set_val_2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 aal_coef_3_c                : 10;  //  0: 9
	UINT32                             : 2;   // 10:11 reserved
	UINT32 aal_coef_4_c                : 10;  // 12:21
} H14A0_SMR_SMUX_HSC_C_SET_VAL_2_T;

/*-----------------------------------------------------------------------------
	0x2a28 smr_smux_size_i ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 hsize_i                     : 12;  //  0:11
	UINT32                             : 4;   // 12:15 reserved
	UINT32 vsize_i                     : 12;  // 16:27
} H14A0_SMR_SMUX_SIZE_I_T;

/*-----------------------------------------------------------------------------
	0x2a2c smr_smux_size_o ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 hsize                       : 12;  //  0:11
	UINT32                             : 4;   // 12:15 reserved
	UINT32 vsize                       : 12;  // 16:27
} H14A0_SMR_SMUX_SIZE_O_T;

/*-----------------------------------------------------------------------------
	0x2a30 smr_smux_size_d ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 hsize_d                     : 12;  //  0:11
	UINT32                             : 4;   // 12:15 reserved
	UINT32 vsize_d                     : 12;  // 16:27
} H14A0_SMR_SMUX_SIZE_D_T;

/*-----------------------------------------------------------------------------
	0x2a34 smr_smux_hsc_offset ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 hoffset                     : 12;  //  0:11
	UINT32                             : 4;   // 12:15 reserved
	UINT32 voffset                     : 12;  // 16:27
} H14A0_SMR_SMUX_HSC_OFFSET_T;

/*-----------------------------------------------------------------------------
	0x2a38 smr_smux_hsc_active ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 hactive                     : 12;  //  0:11
	UINT32                             : 4;   // 12:15 reserved
	UINT32 vactive                     : 12;  // 16:27
} H14A0_SMR_SMUX_HSC_ACTIVE_T;

/*-----------------------------------------------------------------------------
	0x2a3c smr_smux_hsc_input_info ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 hsc_i_hsize                 : 12;  //  0:11
	UINT32                             : 4;   // 12:15 reserved
	UINT32 hsc_i_vsize                 : 12;  // 16:27
} H14A0_SMR_SMUX_HSC_INPUT_INFO_T;

/*-----------------------------------------------------------------------------
	0x2a40 smr_smux_status ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 smux_ro_lcnt                : 12;  //  0:11
	UINT32                             : 4;   // 12:15 reserved
	UINT32 smux_pic_init_count         : 4;   // 16:19
	UINT32 smux_read_pic_init_count    : 4;   // 20:23
	UINT32 smux_debug_pic_init_count   : 4;   // 24:27
} H14A0_SMR_SMUX_STATUS_T;

typedef struct {
	H14A0_SML_SMUX_CTRL0_T              sml_smux_ctrl0;                      // 0x0a00
	H14A0_SML_SMUX_CTRL1_T              sml_smux_ctrl1;                      // 0x0a04
	H14A0_SML_SMUX_CTRL2_T              sml_smux_ctrl2;                      // 0x0a08
	H14A0_SML_SMUX_CTRLD_T              sml_smux_ctrld;                      // 0x0a0c
	H14A0_SML_SMUX_HSC_Y_SET_VAL_0_T    sml_smux_hsc_y_set_val_0;            // 0x0a10
	H14A0_SML_SMUX_HSC_Y_SET_VAL_1_T    sml_smux_hsc_y_set_val_1;            // 0x0a14
	H14A0_SML_SMUX_HSC_Y_SET_VAL_2_T    sml_smux_hsc_y_set_val_2;            // 0x0a18
	H14A0_SML_SMUX_HSC_C_SET_VAL_0_T    sml_smux_hsc_c_set_val_0;            // 0x0a1c
	H14A0_SML_SMUX_HSC_C_SET_VAL_1_T    sml_smux_hsc_c_set_val_1;            // 0x0a20
	H14A0_SML_SMUX_HSC_C_SET_VAL_2_T    sml_smux_hsc_c_set_val_2;            // 0x0a24
	H14A0_SML_SMUX_SIZE_I_T             sml_smux_size_i;                     // 0x0a28
	H14A0_SML_SMUX_SIZE_O_T             sml_smux_size_o;                     // 0x0a2c
	H14A0_SML_SMUX_SIZE_D_T             sml_smux_size_d;                     // 0x0a30
	H14A0_SML_SMUX_HSC_OFFSET_T         sml_smux_hsc_offset;                 // 0x0a34
	H14A0_SML_SMUX_HSC_ACTIVE_T         sml_smux_hsc_active;                 // 0x0a38
	H14A0_SML_SMUX_HSC_INPUT_INFO_T     sml_smux_hsc_input_info;             // 0x0a3c
	H14A0_SML_SMUX_STATUS_T             sml_smux_status;                     // 0x0a40
} DE_SML_REG_H14A0_T;

typedef struct {
	H14A0_SMR_SMUX_CTRL0_T              smr_smux_ctrl0;                      // 0x2a00
	H14A0_SMR_SMUX_CTRL1_T              smr_smux_ctrl1;                      // 0x2a04
	H14A0_SMR_SMUX_CTRL2_T              smr_smux_ctrl2;                      // 0x2a08
	H14A0_SMR_SMUX_CTRLD_T              smr_smux_ctrld;                      // 0x2a0c
	H14A0_SMR_SMUX_HSC_Y_SET_VAL_0_T    smr_smux_hsc_y_set_val_0;            // 0x2a10
	H14A0_SMR_SMUX_HSC_Y_SET_VAL_1_T    smr_smux_hsc_y_set_val_1;            // 0x2a14
	H14A0_SMR_SMUX_HSC_Y_SET_VAL_2_T    smr_smux_hsc_y_set_val_2;            // 0x2a18
	H14A0_SMR_SMUX_HSC_C_SET_VAL_0_T    smr_smux_hsc_c_set_val_0;            // 0x2a1c
	H14A0_SMR_SMUX_HSC_C_SET_VAL_1_T    smr_smux_hsc_c_set_val_1;            // 0x2a20
	H14A0_SMR_SMUX_HSC_C_SET_VAL_2_T    smr_smux_hsc_c_set_val_2;            // 0x2a24
	H14A0_SMR_SMUX_SIZE_I_T             smr_smux_size_i;                     // 0x2a28
	H14A0_SMR_SMUX_SIZE_O_T             smr_smux_size_o;                     // 0x2a2c
	H14A0_SMR_SMUX_SIZE_D_T             smr_smux_size_d;                     // 0x2a30
	H14A0_SMR_SMUX_HSC_OFFSET_T         smr_smux_hsc_offset;                 // 0x2a34
	H14A0_SMR_SMUX_HSC_ACTIVE_T         smr_smux_hsc_active;                 // 0x2a38
	H14A0_SMR_SMUX_HSC_INPUT_INFO_T     smr_smux_hsc_input_info;             // 0x2a3c
	H14A0_SMR_SMUX_STATUS_T             smr_smux_status;                     // 0x2a40
} DE_SMR_REG_H14A0_T;

#endif
