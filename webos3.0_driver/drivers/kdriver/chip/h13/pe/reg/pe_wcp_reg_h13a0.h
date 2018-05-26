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

#ifndef _PE_WCP_REG_H13A0_H_
#define _PE_WCP_REG_H13A0_H_

/*-----------------------------------------------------------------------------
	0x1200 ovl_ov_ctrl0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 main_444                    : 1;   //     0
	UINT32                             : 1;   //     1 reserved
	UINT32 main_420                    : 1;   //     2
	UINT32                             : 1;   //     3 reserved
	UINT32 main_lsr_2b                 : 2;   //  4: 5
	UINT32 main_cflt_mode              : 2;   //  6: 7
	UINT32                             : 8;   //  8:15 reserved
	UINT32 mif_last_mode               : 2;   // 16:17
	UINT32                             : 2;   // 18:19 reserved
	UINT32 ov_out_sync_mux             : 2;   // 20:21
} PE_H13A0_OVL_OV_CTRL0_T;

/*-----------------------------------------------------------------------------
	0x1204 ov_ctrl1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 border_color_cr             : 4;   //  0: 3
	UINT32 border_color_cb             : 4;   //  4: 7
	UINT32 border_color_yy             : 8;   //  8:15
	UINT32 load_enable                 : 1;   //    16
	UINT32 load_type                   : 1;   //    17
} PE_H13A0_OV_CTRL1_T;

/*-----------------------------------------------------------------------------
	0x1208 ovl_ov_ctrl2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 back_color_cr               : 4;   //  0: 3
	UINT32 back_color_cb               : 4;   //  4: 7
	UINT32 back_color_yy               : 8;   //  8:15
	UINT32 dark_color_cr               : 4;   // 16:19
	UINT32 dark_color_cb               : 4;   // 23:20
	UINT32 dark_color_yy               : 8;   // 24:31
} PE_H13A0_OVL_OV_CTRL2_T;

/*-----------------------------------------------------------------------------
	0x120c ov_ctrl3 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 w0_blending_alpha           : 8;   //  0: 7
	UINT32 w1_blending_alpha           : 8;   //  8:15
	UINT32 w2_blending_alpha           : 8;   // 16:23
	UINT32 w0_blending_mode            : 2;   // 24:25
	UINT32 w1_blending_mode            : 2;   // 26:27
	UINT32 w2_blending_mode            : 2;   // 28:29
} PE_H13A0_OV_CTRL3_T;

/*-----------------------------------------------------------------------------
	0x1210 ovl_ov_moffset ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 m_hoffset                   : 12;  //  0:11
	UINT32                             : 4;   // 12:15 reserved
	UINT32 m_voffset                   : 12;  // 16:27
} PE_H13A0_OVL_OV_MOFFSET_T;

/*-----------------------------------------------------------------------------
	0x1214 ovl_ov_msize ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 m_hsize                     : 12;  //  0:11
	UINT32                             : 4;   // 12:15 reserved
	UINT32 m_vsize                     : 12;  // 16:27
} PE_H13A0_OVL_OV_MSIZE_T;

/*-----------------------------------------------------------------------------
	0x1220 ovl_ov_border_width ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 s_right_wid                 : 4;   //  0: 3
	UINT32 s_left_wid                  : 4;   //  4: 7
	UINT32 s_down_wid                  : 4;   //  8:11
	UINT32 s_up_wid                    : 4;   // 12:15
	UINT32 m_right_wid                 : 4;   // 16:19
	UINT32 m_left_wid                  : 4;   // 20:23
	UINT32 m_down_wid                  : 4;   // 24:27
	UINT32 m_up_wid                    : 4;   // 28:31
} PE_H13A0_OVL_OV_BORDER_WIDTH_T;

/*-----------------------------------------------------------------------------
	0x1224 ovl_cflt_coef0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cflt_coef_1                 : 12;  //  0:11
	UINT32                             : 4;   // 12:15 reserved
	UINT32 cflt_coef_0                 : 12;  // 16:27
} PE_H13A0_OVL_CFLT_COEF0_T;

/*-----------------------------------------------------------------------------
	0x1228 ovl_cflt_coef1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cflt_coef_3                 : 12;  //  0:11
	UINT32                             : 4;   // 12:15 reserved
	UINT32 cflt_coef_2                 : 12;  // 16:27
} PE_H13A0_OVL_CFLT_COEF1_T;

/*-----------------------------------------------------------------------------
	0x1240 ovl_ov_intr_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 intr_enable                 : 1;   //     0
	UINT32 intr_line_pos_enable        : 1;   //     1
	UINT32                             : 6;   //  2: 7 reserved
	UINT32 intr_sbuf_c_empty_en        : 1;   //     8
	UINT32 intr_sbuf_y_empty_en        : 1;   //     9
	UINT32 intr_mbuf_c_empty_en        : 1;   //    10
	UINT32 intr_mbuf_y_empty_en        : 1;   //    11
	UINT32 intr_pdb_end_en             : 1;   //    12
	UINT32                             : 3;   // 13:15 reserved
	UINT32 line_position               : 11;  // 16:26
} PE_H13A0_OVL_OV_INTR_CTRL_T;

/*-----------------------------------------------------------------------------
	0x1244 ovl_ov_intr_status ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 sbuf_c_empty                : 1;   //     0
	UINT32 sbuf_y_empty                : 1;   //     1
	UINT32 mbuf_c_empty                : 1;   //     2
	UINT32 mbuf_y_empty                : 1;   //     3
	UINT32                             : 12;  //  4:15 reserved
	UINT32 line_cnt                    : 11;  // 16:26
} PE_H13A0_OVL_OV_INTR_STATUS_T;

/*-----------------------------------------------------------------------------
	0x1248 ov_ctrl4 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 w0_imux_ctrl                : 2;   //  0: 1
	UINT32 w0_dark_en                  : 1;   //     2
	UINT32                             : 1;   //     3 reserved
	UINT32 w1_imux_ctrl                : 2;   //  4: 5
	UINT32 w1_dark_en                  : 1;   //     6
	UINT32                             : 1;   //     7 reserved
	UINT32 w2_imux_ctrl                : 2;   //  8: 9
	UINT32 w2_dark_en                  : 1;   //    10
} PE_H13A0_OV_CTRL4_T;

/*-----------------------------------------------------------------------------
	0x1258 ovl_ov_yc_delay ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 c_delay_h                   : 12;  //  0:11
	UINT32                             : 4;   // 12:15 reserved
	UINT32 c_delay_v                   : 12;  // 16:27
	UINT32                             : 2;   // 28:29 reserved
	UINT32 v_delay_mode                : 1;   //    30
	UINT32 delay_disable               : 1;   //    31
} PE_H13A0_OVL_OV_YC_DELAY_T;

/*-----------------------------------------------------------------------------
	0x125c ov_mif_ctrl0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 main_c_mif_line_offset      : 12;  //  0:11
	UINT32                             : 1;   //    12 reserved
	UINT32 main_c_mif_line_inc         : 2;   // 13:14
	UINT32 main_c_manual_mif_ctrl_en   : 1;   //    15
	UINT32 main_y_mif_line_offset      : 12;  // 16:27
	UINT32                             : 1;   //    28 reserved
	UINT32 main_y_mif_line_inc         : 2;   // 29:30
	UINT32 main_y_manual_mif_ctrl_en   : 1;   //    31
} PE_H13A0_OV_MIF_CTRL0_T;

/*-----------------------------------------------------------------------------
	0x1260 ov_mif_ctrl1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 sub_c_mif_line_offset       : 12;  //  0:11
	UINT32                             : 1;   //    12 reserved
	UINT32 sub_c_mif_line_inc          : 2;   // 13:14
	UINT32 sub_c_manual_mif_ctrl_en    : 1;   //    15
	UINT32 sub_y_mif_line_offset       : 12;  // 16:27
	UINT32                             : 1;   //    28 reserved
	UINT32 sub_y_mif_line_inc          : 2;   // 29:30
	UINT32 sub_y_manual_mif_ctrl_en    : 1;   //    31
} PE_H13A0_OV_MIF_CTRL1_T;

/*-----------------------------------------------------------------------------
	0x3200 ovr_ov_ctrl0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 main_444                    : 1;   //     0
	UINT32                             : 1;   //     1 reserved
	UINT32 main_420                    : 1;   //     2
	UINT32                             : 3;   //  3: 5 reserved
	UINT32 main_cflt_mode              : 2;   //  6: 7
	UINT32                             : 8;   //  8:15 reserved
	UINT32 mif_last_mode               : 1;   //    16
	UINT32                             : 3;   // 17:19 reserved
	UINT32 ov_out_sync_mux             : 1;   //    20
} PE_H13A0_OVR_OV_CTRL0_T;

/*-----------------------------------------------------------------------------
	0x3208 ovr_ov_ctrl2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 back_color_cr               : 4;   //  0: 3
	UINT32 back_color_cb               : 4;   //  4: 7
	UINT32 back_color_yy               : 8;   //  8:15
	UINT32 dark_color_cr               : 4;   // 16:19
	UINT32 dark_color_cb               : 4;   // 20:23
	UINT32 dark_color_yy               : 8;   // 24:31
} PE_H13A0_OVR_OV_CTRL2_T;

/*-----------------------------------------------------------------------------
	0x3210 ovr_ov_moffset ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 m_hoffset                   : 12;  //  0:11
	UINT32                             : 4;   // 12:15 reserved
	UINT32 m_voffset                   : 12;  // 16:27
} PE_H13A0_OVR_OV_MOFFSET_T;

/*-----------------------------------------------------------------------------
	0x3214 ovr_ov_msize ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 m_hsize                     : 12;  //  0:11
	UINT32                             : 4;   // 12:15 reserved
	UINT32 m_vsize                     : 12;  // 16:27
} PE_H13A0_OVR_OV_MSIZE_T;

/*-----------------------------------------------------------------------------
	0x3220 ovr_ov_border_width ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                             : 16;  //  0:15 reserved
	UINT32 m_right_wid                 : 4;   // 16:19
	UINT32 m_left_wid                  : 4;   // 20:23
	UINT32 m_down_wid                  : 4;   // 24:27
	UINT32 m_up_wid                    : 4;   // 28:31
} PE_H13A0_OVR_OV_BORDER_WIDTH_T;

/*-----------------------------------------------------------------------------
	0x3224 ovr_cflt_coef0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cflt_coef_1                 : 12;  //  0:11
	UINT32                             : 4;   // 12:15 reserved
	UINT32 cflt_coef_0                 : 12;  // 16:27
} PE_H13A0_OVR_CFLT_COEF0_T;

/*-----------------------------------------------------------------------------
	0x3228 ovr_cflt_coef1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cflt_coef_3                 : 12;  //  0:11
	UINT32                             : 4;   // 12:15 reserved
	UINT32 cflt_coef_2                 : 12;  // 16:27
} PE_H13A0_OVR_CFLT_COEF1_T;

/*-----------------------------------------------------------------------------
	0x3240 ovr_ov_intr_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 intr_enable                 : 1;   //     0
	UINT32 intr_line_pos_enable        : 1;   //     1
	UINT32                             : 6;   //  2: 7 reserved
	UINT32 intr_sbuf_c_empty_en        : 1;   //     8
	UINT32 intr_sbuf_y_empty_en        : 1;   //     9
	UINT32 intr_mbuf_c_empty_en        : 1;   //    10
	UINT32 intr_mbuf_y_empty_en        : 1;   //    11
	UINT32 intr_pdb_end_en             : 1;   //    12
	UINT32                             : 3;   // 13:15 reserved
	UINT32 line_position               : 11;  // 16:26
} PE_H13A0_OVR_OV_INTR_CTRL_T;

/*-----------------------------------------------------------------------------
	0x3244 ovr_ov_intr_status ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                             : 2;   //  0: 1 reserved
	UINT32 mbuf_c_empty                : 1;   //     2
	UINT32 mbuf_y_empty                : 1;   //     3
	UINT32                             : 12;  //  4:15 reserved
	UINT32 line_cnt                    : 11;  // 16:26
} PE_H13A0_OVR_OV_INTR_STATUS_T;

/*-----------------------------------------------------------------------------
	0x3258 ovr_ov_yc_delay ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 c_delay_h                   : 12;  //  0:11
	UINT32                             : 4;   // 12:15 reserved
	UINT32 c_delay_v                   : 12;  // 16:27
	UINT32                             : 2;   // 28:29 reserved
	UINT32 v_delay_mode                : 1;   //    30
	UINT32 delay_disable               : 1;   //    31
} PE_H13A0_OVR_OV_YC_DELAY_T;

/*-----------------------------------------------------------------------------
	0x325c ov_mif_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 main_c_mif_line_offset      : 12;  //  0:11
	UINT32                             : 1;   //    12 reserved
	UINT32 main_c_mif_line_inc         : 2;   // 13:14
	UINT32 main_c_manual_mif_ctrl_en   : 1;   //    15
	UINT32 main_y_mif_line_offset      : 12;  // 16:27
	UINT32                             : 1;   //    28 reserved
	UINT32 main_y_mif_line_inc         : 2;   // 29:30
	UINT32 main_y_manual_mif_ctrl_en   : 1;   //    31
} PE_H13A0_OV_MIF_CTRL_T;

typedef struct {
	PE_H13A0_OVL_OV_CTRL0_T            ovl_ov_ctrl0;                    // 0x1200
	PE_H13A0_OV_CTRL1_T                ov_ctrl1;                        // 0x1204
	PE_H13A0_OVL_OV_CTRL2_T            ovl_ov_ctrl2;                    // 0x1208
	PE_H13A0_OV_CTRL3_T                ov_ctrl3;                        // 0x120c
	PE_H13A0_OVL_OV_MOFFSET_T          ovl_ov_moffset;                  // 0x1210
	PE_H13A0_OVL_OV_MSIZE_T            ovl_ov_msize;                    // 0x1214
	UINT32                             reserved0[2];                    // 0x1218~0x121c
	PE_H13A0_OVL_OV_BORDER_WIDTH_T     ovl_ov_border_width;             // 0x1220
	PE_H13A0_OVL_CFLT_COEF0_T          ovl_cflt_coef0;                  // 0x1224
	PE_H13A0_OVL_CFLT_COEF1_T          ovl_cflt_coef1;                  // 0x1228
	UINT32                             reserved1[5];                    // 0x122c~0x123c
	PE_H13A0_OVL_OV_INTR_CTRL_T        ovl_ov_intr_ctrl;                // 0x1240
	PE_H13A0_OVL_OV_INTR_STATUS_T      ovl_ov_intr_status;              // 0x1244
	PE_H13A0_OV_CTRL4_T                ov_ctrl4;                        // 0x1248
	UINT32                             reserved2[3];                    // 0x124c~0x1254
	PE_H13A0_OVL_OV_YC_DELAY_T         ovl_ov_yc_delay;                 // 0x1258
	PE_H13A0_OV_MIF_CTRL0_T            ov_mif_ctrl0;                    // 0x125c
	PE_H13A0_OV_MIF_CTRL1_T            ov_mif_ctrl1;                    // 0x1260
} PE_OVL_REG_H13A0_T;

typedef struct {
	PE_H13A0_OVR_OV_CTRL0_T            ovr_ov_ctrl0;                    // 0x3200
	UINT32                             reserved3;                       // 0x3204
	PE_H13A0_OVR_OV_CTRL2_T            ovr_ov_ctrl2;                    // 0x3208
	UINT32                             reserved4;                       // 0x320c
	PE_H13A0_OVR_OV_MOFFSET_T          ovr_ov_moffset;                  // 0x3210
	PE_H13A0_OVR_OV_MSIZE_T            ovr_ov_msize;                    // 0x3214
	UINT32                             reserved5[2];                    // 0x3218~0x321c
	PE_H13A0_OVR_OV_BORDER_WIDTH_T     ovr_ov_border_width;             // 0x3220
	PE_H13A0_OVR_CFLT_COEF0_T          ovr_cflt_coef0;                  // 0x3224
	PE_H13A0_OVR_CFLT_COEF1_T          ovr_cflt_coef1;                  // 0x3228
	UINT32                             reserved6[5];                    // 0x322c~0x323c
	PE_H13A0_OVR_OV_INTR_CTRL_T        ovr_ov_intr_ctrl;                // 0x3240
	PE_H13A0_OVR_OV_INTR_STATUS_T      ovr_ov_intr_status;              // 0x3244
	UINT32                             reserved7[4];                    // 0x3248~0x3254
	PE_H13A0_OVR_OV_YC_DELAY_T         ovr_ov_yc_delay;                 // 0x3258
	PE_H13A0_OV_MIF_CTRL_T             ov_mif_ctrl;                     // 0x325c
} PE_OVR_REG_H13A0_T;

#endif
