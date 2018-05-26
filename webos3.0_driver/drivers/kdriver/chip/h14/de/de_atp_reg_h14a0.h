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

#ifndef _REG_ATP_H14A0_h
#define _REG_ATP_H14A0_h

/*-----------------------------------------------------------------------------
	0x3800 ttx_ver_ctrl0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 ttx_ovs                     : 8;   //  0: 7
	UINT32 ttx_ove                     : 8;   //  8:15
	UINT32 ttx_evs                     : 8;   // 16:23
	UINT32 ttx_eve                     : 8;   // 24:31
} H14A0_TTX_VER_CTRL0_T;

/*-----------------------------------------------------------------------------
	0x3804 ttx_ver_ctrl1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 ttx_ovs8                    : 1;   //     0
	UINT32 ttx_ove8                    : 1;   //     1
	UINT32 ttx_evs8                    : 1;   //     2
	UINT32 ttx_eve8                    : 1;   //     3
	UINT32 ttx_iter_cnt                : 3;   //  4: 6
	UINT32 ttx_adj                     : 1;   //     7
	UINT32 ttx_err                     : 4;   //  8:11
	UINT32                             : 2;   // 12:13 reserved
	UINT32 ttx_line                    : 18;  // 14:31
} H14A0_TTX_VER_CTRL1_T;

/*-----------------------------------------------------------------------------
	0x3808 wss_data ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 ttx_hs                      : 11;  //  0:10
	UINT32 comp_c_delay                : 5;   // 11:15
	UINT32 wss_data                    : 14;  // 16:29
	UINT32 comp_y_lpf                  : 1;   //    30
} H14A0_WSS_DATA_T;

/*-----------------------------------------------------------------------------
	0x380c cgms_data ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cgms_enc_data               : 20;  //  0:19
	UINT32 secam_burst_blank_end       : 8;   // 20:27
	UINT32 secam_preem_scaling         : 3;   // 28:30
} H14A0_CGMS_DATA_T;

/*-----------------------------------------------------------------------------
	0x3810 cve_config ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 standard                    : 3;   //  0: 2
	UINT32 enc_en                      : 1;   //     3
	UINT32 color_bar_en                : 1;   //     4
	UINT32 c_filter                    : 2;   //  5: 6
	UINT32 color_on                    : 1;   //     7
	UINT32 y_filter                    : 2;   //  8: 9
	UINT32 y_delay                     : 2;   // 10:11
	UINT32 ttx_sys                     : 3;   // 12:14
	UINT32 video_interface_sel         : 1;   //    15
	UINT32 burst_amp                   : 8;   // 16:23
	UINT32 secam_y_delay               : 5;   // 24:28
	UINT32 offset                      : 2;   // 29:30
	UINT32 invert                      : 1;   //    31
} H14A0_CVE_CONFIG_T;

/*-----------------------------------------------------------------------------
	0x3814 phase_incr ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 phase_incr                  : 32;  //  0:31
} H14A0_PHASE_INCR_T;

/*-----------------------------------------------------------------------------
	0x3818 denc_vbi_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 phase_offset                : 8;   //  0: 7
	UINT32 secam_subcarrier_scaling    : 6;   //  8:13
	UINT32 secam_c_gain                : 2;   // 14:15
	UINT32 cc1_en                      : 1;   //    16
	UINT32 cc2_en                      : 1;   //    17
	UINT32 cc1_full                    : 1;   //    18
	UINT32 cc2_full                    : 1;   //    19
	UINT32 cgms_en                     : 1;   //    20
	UINT32 wss_en                      : 1;   //    21
	UINT32                             : 1;   //    22 reserved
	UINT32 fm_reset_on                 : 1;   //    23
	UINT32 fm_reset_initial            : 1;   //    24
	UINT32 burst_phase_reset_strt      : 5;   // 25:29
	UINT32 comp_c_gain                 : 2;   // 30:31
} H14A0_DENC_VBI_CTRL_T;

/*-----------------------------------------------------------------------------
	0x381c cc_data ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cc_data_f1                  : 16;  //  0:15
	UINT32 cc_data_f2                  : 16;  // 16:31
} H14A0_CC_DATA_T;

/*-----------------------------------------------------------------------------
	0x3820 mv_n0_3 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 mv_n0                       : 6;   //  0: 5
	UINT32                             : 2;   //  6: 7 reserved
	UINT32 mv_n2                       : 6;   //  8:13
	UINT32                             : 2;   // 14:15 reserved
	UINT32 mv_n1                       : 6;   // 16:21
	UINT32                             : 2;   // 22:23 reserved
	UINT32 mv_n3                       : 6;   // 24:29
} H14A0_MV_N0_3_T;

/*-----------------------------------------------------------------------------
	0x3824 mv_n4_9 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 mv_n4                       : 6;   //  0: 5
	UINT32 mv_n22                      : 1;   //     6
	UINT32                             : 1;   //     7 reserved
	UINT32 mv_n8                       : 6;   //  8:13
	UINT32                             : 2;   // 14:15 reserved
	UINT32 mv_n7                       : 2;   // 16:17
	UINT32 mv_n6                       : 3;   // 18:20
	UINT32 mv_n5                       : 3;   // 21:23
	UINT32 mv_n9                       : 6;   // 24:29
} H14A0_MV_N4_9_T;

/*-----------------------------------------------------------------------------
	0x3828 mv_n10_11 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 mv_n10                      : 6;   //  0: 5
	UINT32 mv_n21                      : 10;  //  6:15
	UINT32 mv_n11                      : 15;  // 16:30
} H14A0_MV_N10_11_T;

/*-----------------------------------------------------------------------------
	0x382c mv_n12_14 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 mv_n12                      : 15;  //  0:14
	UINT32                             : 1;   //    15 reserved
	UINT32 mv_n14                      : 8;   // 16:23
	UINT32 mv_n13                      : 8;   // 24:31
} H14A0_MV_N12_14_T;

/*-----------------------------------------------------------------------------
	0x3830 mv_n15_20 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 mv_n15                      : 8;   //  0: 7
	UINT32                             : 8;   //  8:15 reserved
	UINT32 mv_n20                      : 3;   // 16:18
	UINT32 mv_n16                      : 1;   //    19
	UINT32 mv_n19                      : 4;   // 20:23
	UINT32 mv_n18                      : 4;   // 24:27
	UINT32 mv_n17                      : 4;   // 28:31
} H14A0_MV_N15_20_T;

/*-----------------------------------------------------------------------------
	0x3834 vps_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 index                       : 3;   //  0: 2
	UINT32                             : 1;   //     3 reserved
	UINT32 lpf_bypass                  : 1;   //     4
	UINT32 vbi_odd                     : 1;   //     5
	UINT32 vbi_even                    : 1;   //     6
	UINT32 vps_enable                  : 1;   //     7
	UINT32 vps_wr_done                 : 1;   //     8
	UINT32 c_data_delay                : 1;   //     9
	UINT32                             : 2;   // 10:11 reserved
	UINT32 wss_odd_en                  : 1;   //    12
	UINT32 wss_even_en                 : 1;   //    13
	UINT32 v_pol                       : 1;   //    14
	UINT32 h_pol                       : 1;   //    15
	UINT32 vps_data                    : 16;  // 16:31
} H14A0_VPS_CTRL_T;

/*-----------------------------------------------------------------------------
	0x3838 ttx_data ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 ttx_data                    : 32;  //  0:31
} H14A0_TTX_DATA_T;

/*-----------------------------------------------------------------------------
	0x383c ttx_load ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 ttx_load                    : 1;   //     0
	UINT32                             : 7;   //  1: 7 reserved
	UINT32 ttx_write_addr              : 8;   //  8:15
	UINT32 ttx_data_endian             : 1;   //    16
	UINT32 ttx_data_bit_order          : 1;   //    17
} H14A0_TTX_LOAD_T;

/*-----------------------------------------------------------------------------
	0x3850 ttx_ddru ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 ttx_ddru_stride             : 11;  //  0:10
	UINT32                             : 5;   // 11:15 reserved
	UINT32 ttx_ddru_line_count         : 8;   // 16:23
	UINT32                             : 6;   // 24:29 reserved
	UINT32 ttx_ddru_endian_change      : 1;   //    30
	UINT32 ttx_ddru_update_enable      : 1;   //    31
} H14A0_TTX_DDRU_T;

/*-----------------------------------------------------------------------------
	0x3880 addr_dbg ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 interlaced_mode             : 1;   //     0
	UINT32 msb_sel                     : 1;   //     1
	UINT32                             : 2;   //  2: 3 reserved
	UINT32 top_field                   : 1;   //     4
	UINT32                             : 3;   //  5: 7 reserved
	UINT32 y_delay                     : 3;   //  8:10
	UINT32                             : 1;   //    11 reserved
	UINT32 c_delay                     : 3;   // 12:14
	UINT32                             : 1;   //    15 reserved
	UINT32 pic_init_field              : 4;   // 16:19
	UINT32 vfal_field                  : 4;   // 20:23
	UINT32 cvbs_field                  : 4;   // 24:27
} H14A0_ADDR_DBG_T;

/*-----------------------------------------------------------------------------
	0x3884 a_top_status ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_status                    : 10;  //  0: 9
	UINT32                             : 6;   // 10:15 reserved
	UINT32 c_status                    : 10;  // 16:25
} H14A0_A_TOP_STATUS_T;

typedef struct {
	H14A0_TTX_VER_CTRL0_T   ttx_ver_ctrl0;           // 0x3800
	H14A0_TTX_VER_CTRL1_T   ttx_ver_ctrl1;           // 0x3804
	H14A0_WSS_DATA_T        wss_data;                // 0x3808
	H14A0_CGMS_DATA_T       cgms_data;               // 0x380c
	H14A0_CVE_CONFIG_T      cve_config;              // 0x3810
	H14A0_PHASE_INCR_T      phase_incr;              // 0x3814
	H14A0_DENC_VBI_CTRL_T   denc_vbi_ctrl;           // 0x3818
	H14A0_CC_DATA_T         cc_data;                 // 0x381c
	H14A0_MV_N0_3_T         mv_n0_3;                 // 0x3820
	H14A0_MV_N4_9_T         mv_n4_9;                 // 0x3824
	H14A0_MV_N10_11_T       mv_n10_11;               // 0x3828
	H14A0_MV_N12_14_T       mv_n12_14;               // 0x382c
	H14A0_MV_N15_20_T       mv_n15_20;               // 0x3830
	H14A0_VPS_CTRL_T        vps_ctrl;                // 0x3834
	H14A0_TTX_DATA_T        ttx_data;                // 0x3838
	H14A0_TTX_LOAD_T        ttx_load;                // 0x383c
	UINT32                  reserved0[4];            // 0x3840~0x384c
	H14A0_TTX_DDRU_T        ttx_ddru;                // 0x3850
	UINT32                  reserved1[11];           // 0x3854~0x387c
	H14A0_ADDR_DBG_T        addr_dbg;                // 0x3880
	H14A0_A_TOP_STATUS_T    a_top_status;            // 0x3884
} DE_ATP_REG_H14A0_T;

#endif
