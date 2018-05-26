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

#ifndef _REG_MIF_M14B0_H_
#define _REG_MIF_M14B0_H_
/*-----------------------------------------------------------------------------
		0xc0020e00L mifi_rd_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 disable_rd_mif                     : 1 ;  //     0     
	UINT32 enable_rd_stv_cnt                  : 1 ;  //     1     
	UINT32 mrb_bank_mode                      : 1 ;  //     2     
	UINT32 mrbs_bank_mode                     : 1 ;  //     3     
	UINT32 m_load_rd_req_stv_v                : 1 ;  //     4     
	UINT32 m_load_rd_req_pri_v                : 1 ;  //     5     
	UINT32 m_load_rd_cmd_saddr_v              : 1 ;  //     6     
	UINT32 m_load_rd_cmd_pend_v               : 1 ;  //     7     
	UINT32 m_load_rd_cmd_pri_v                : 1 ;  //     8     
	UINT32 m_load_rd_cmd_stride_v             : 1 ;  //     9     
	UINT32 m_load_rd_pic_init                 : 1 ;  //    10     
	UINT32 reserved0                          : 5 ;  // 15:11 
	UINT32 s_load_rd_req_stv_v                : 1 ;  //    16     
	UINT32 s_load_rd_req_pri_v                : 1 ;  //    17     
	UINT32 s_load_rd_cmd_saddr_v              : 1 ;  //    18     
	UINT32 s_load_rd_cmd_pend_v               : 1 ;  //    19     
	UINT32 s_load_rd_cmd_pri_v                : 1 ;  //    20     
	UINT32 s_load_rd_cmd_stride_v             : 1 ;  //    21     
	UINT32 s_load_rd_pic_init                 : 1 ;  //    22     
	UINT32 reserved1                          : 9 ;  // 31:23     
} M14B0_MIFI_RD_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc0020e04L mifi_wd_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 disable_wd_mif                     : 1 ;  //     0     
	UINT32 enable_wd_stv_cnt                  : 1 ;  //     1     
	UINT32 reserved0                          : 2 ;  //  3: 2 
	UINT32 m_load_wd_req_stv_v                : 1 ;  //     4     
	UINT32 m_load_wd_req_pri_v                : 1 ;  //     5     
	UINT32 m_load_wd_cmd_saddr_v              : 1 ;  //     6     
	UINT32 m_load_wd_cmd_pend_v               : 1 ;  //     7     
	UINT32 m_load_wd_cmd_pri_v                : 1 ;  //     8     
	UINT32 m_load_wd_cmd_stride_v             : 1 ;  //     9     
	UINT32 m_load_wd_pic_init                 : 1 ;  //    10     
	UINT32 reserved1                          : 5 ;  // 15:11
	UINT32 s_load_wd_req_stv_v                : 1 ;  //    16     
	UINT32 s_load_wd_req_pri_v                : 1 ;  //    17     
	UINT32 s_load_wd_cmd_saddr_v              : 1 ;  //    18     
	UINT32 s_load_wd_cmd_pend_v               : 1 ;  //    19     
	UINT32 s_load_wd_cmd_pri_v                : 1 ;  //    20     
	UINT32 s_load_wd_cmd_stride_v             : 1 ;  //    21     
	UINT32 s_load_wd_pic_init                 : 1 ;  //    22     
	UINT32 reserved2                          : 1 ;  //    23
	UINT32 w_load_wd_req_stv_v                : 1 ;  //    24     
	UINT32 w_load_wd_req_pri_v                : 1 ;  //    25     
	UINT32 w_load_wd_cmd_saddr_v              : 1 ;  //    26     
	UINT32 w_load_wd_cmd_pend_v               : 1 ;  //    27     
	UINT32 w_load_wd_cmd_pri_v                : 1 ;  //    28     
	UINT32 w_load_wd_cmd_stride_v             : 1 ;  //    29     
	UINT32 w_load_wd_pic_init                 : 1 ;  //    30     
	UINT32 reserved3                          : 1 ;  //    31     
} M14B0_MIFI_WD_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc0020e08L mifi_rd_set_10b ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 set_10b_req_rd_prew_y              : 1 ;  //     0     
	UINT32 set_10b_req_rd_prew_c              : 1 ;  //     1     
	UINT32 reserved                           : 30;  // 31: 2     
} M14B0_MIFI_RD_SET_10B_T;

/*-----------------------------------------------------------------------------
		0xc0020e0cL mifi_wd_set_10b ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 set_10b_req_wd_prew_y              : 1 ;  //     0     
	UINT32 set_10b_req_wd_prew_c              : 1 ;  //     1     
	UINT32 reserved                           : 30;  // 31: 2     
} M14B0_MIFI_WD_SET_10B_T;

/*-----------------------------------------------------------------------------
		0xc0020e10L mifi_rd_cmd_pri0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_pri_v_prew_y                : 4 ;  //  3: 0     
	UINT32 cmd_rd_pri_v_prew_c                : 4 ;  //  7: 4     
	UINT32 cmd_rd_pri_v_mvi_y                 : 4 ;  // 11: 8     
	UINT32 cmd_rd_pri_v_mvi_c                 : 4 ;  // 15:12     
	UINT32 cmd_rd_pri_v_prews_y               : 4 ;  // 19:16     
	UINT32 cmd_rd_pri_v_prews_c               : 4 ;  // 23:20     
	UINT32 cmd_rd_pri_v_mvis_y                : 4 ;  // 27:24     
	UINT32 cmd_rd_pri_v_mvis_c                : 4 ;  // 31:28     
} M14B0_MIFI_RD_CMD_PRI0_T;

/*-----------------------------------------------------------------------------
		0xc0020e14L mifi_wd_cmd_pri0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_wd_pri_v_prew_y                : 4 ;  //  3: 0     
	UINT32 cmd_wd_pri_v_prew_c                : 4 ;  //  7: 4     
	UINT32 cmd_wd_pri_v_prews_y               : 4 ;  // 11: 8     
	UINT32 cmd_wd_pri_v_prews_c               : 4 ;  // 15:12     
	UINT32 cmd_wd_pri_v_preww_y               : 4 ;  // 19:16     
	UINT32 cmd_wd_pri_v_preww_c               : 4 ;  // 23:20     
	UINT32 reserved                           : 8 ;  // 31:24     
} M14B0_MIFI_WD_CMD_PRI0_T;

/*-----------------------------------------------------------------------------
		0xc0020e18L mifi_rd_req_pri0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_pri_v_prew_y                : 4 ;  //  3: 0     
	UINT32 req_rd_pri_v_prew_c                : 4 ;  //  7: 4     
	UINT32 req_rd_pri_v_mvi_y                 : 4 ;  // 11: 8     
	UINT32 req_rd_pri_v_mvi_c                 : 4 ;  // 15:12     
	UINT32 req_rd_pri_v_prews_y               : 4 ;  // 19:16     
	UINT32 req_rd_pri_v_prews_c               : 4 ;  // 23:20     
	UINT32 req_rd_pri_v_mvis_y                : 4 ;  // 27:24     
	UINT32 req_rd_pri_v_mvis_c                : 4 ;  // 31:28     
} M14B0_MIFI_RD_REQ_PRI0_T;

/*-----------------------------------------------------------------------------
		0xc0020e1cL mifi_wd_req_pri0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_wd_pri_v_prew_y                : 4 ;  //  3: 0     
	UINT32 req_wd_pri_v_prew_c                : 4 ;  //  7: 4     
	UINT32 req_wd_pri_v_prews_y               : 4 ;  // 11: 8     
	UINT32 req_wd_pri_v_prews_c               : 4 ;  // 15:12     
	UINT32 req_wd_pri_v_preww_y               : 4 ;  // 19:16     
	UINT32 req_wd_pri_v_preww_c               : 4 ;  // 23:20     
	UINT32 reserved                           : 8 ;  // 31:24     
} M14B0_MIFI_WD_REQ_PRI0_T;

/*-----------------------------------------------------------------------------
		0xc0020e20L mifi_rd_req_stv0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_stv_v_prew_y                : 4 ;  //  3: 0     
	UINT32 req_rd_stv_v_prew_c                : 4 ;  //  7: 4     
	UINT32 req_rd_stv_v_mvi_y                 : 4 ;  // 11: 8     
	UINT32 req_rd_stv_v_mvi_c                 : 4 ;  // 15:12     
	UINT32 req_rd_stv_v_prews_y               : 4 ;  // 19:16     
	UINT32 req_rd_stv_v_prews_c               : 4 ;  // 23:20     
	UINT32 req_rd_stv_v_mvis_y                : 4 ;  // 27:24     
	UINT32 req_rd_stv_v_mvis_c                : 4 ;  // 31:28     
} M14B0_MIFI_RD_REQ_STV0_T;

/*-----------------------------------------------------------------------------
		0xc0020e24L mifi_wd_req_stv0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_wd_stv_v_prew_y                : 4 ;  //  3: 0     
	UINT32 req_wd_stv_v_prew_c                : 4 ;  //  7: 4     
	UINT32 req_wd_stv_v_prews_y               : 4 ;  // 11: 8     
	UINT32 req_wd_stv_v_prews_c               : 4 ;  // 15:12     
	UINT32 req_wd_stv_v_preww_y               : 4 ;  // 19:16     
	UINT32 req_wd_stv_v_preww_c               : 4 ;  // 23:20     
	UINT32 reserved                           : 8 ;  // 31:24     
} M14B0_MIFI_WD_REQ_STV0_T;

/*-----------------------------------------------------------------------------
		0xc0020e28L mifi_rd_cmd_pend0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_pend_v_prew_y               : 4 ;  //  3: 0     
	UINT32 cmd_rd_pend_v_prew_c               : 4 ;  //  7: 4     
	UINT32 cmd_rd_pend_v_mvi_y                : 4 ;  // 11: 8     
	UINT32 cmd_rd_pend_v_mvi_c                : 4 ;  // 15:12     
	UINT32 cmd_rd_pend_v_prews_y              : 4 ;  // 19:16     
	UINT32 cmd_rd_pend_v_prews_c              : 4 ;  // 23:20     
	UINT32 cmd_rd_pend_v_mvis_y               : 4 ;  // 27:24     
	UINT32 cmd_rd_pend_v_mvis_c               : 4 ;  // 31:28     
} M14B0_MIFI_RD_CMD_PEND0_T;

/*-----------------------------------------------------------------------------
		0xc0020e2cL mifi_wd_cmd_pend0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_wd_pend_v_prew_y               : 4 ;  //  3: 0     
	UINT32 cmd_wd_pend_v_prew_c               : 4 ;  //  7: 4     
	UINT32 cmd_wd_pend_v_prews_y              : 4 ;  // 11: 8     
	UINT32 cmd_wd_pend_v_prews_c              : 4 ;  // 15:12     
	UINT32 cmd_wd_pend_v_preww_y              : 4 ;  // 19:16     
	UINT32 cmd_wd_pend_v_preww_c              : 4 ;  // 23:20     
	UINT32 reserved                           : 8 ;  // 31:24     
} M14B0_MIFI_WD_CMD_PEND0_T;

/*-----------------------------------------------------------------------------
		0xc0020e30L mifi_rd_cmd_stride0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_stride_v_2_prew_y           : 4 ;  //  3: 0     
	UINT32 cmd_rd_stride_v_8_prew_y           : 4 ;  //  7: 4     
	UINT32 cmd_rd_stride_v_2_prew_c           : 4 ;  // 11: 8     
	UINT32 cmd_rd_stride_v_8_prew_c           : 4 ;  // 15:12     
	UINT32 cmd_rd_stride_v_mvi_y              : 4 ;  // 19:16     
	UINT32 cmd_rd_stride_v_mvi_c              : 4 ;  // 23:20     
	UINT32 cmd_rd_stride_v_prews_y            : 4 ;  // 27:24     
	UINT32 cmd_rd_stride_v_prews_c            : 4 ;  // 31:28     
} M14B0_MIFI_RD_CMD_STRIDE0_T;

/*-----------------------------------------------------------------------------
		0xc0020e34L mifi_rd_cmd_stride1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_stride_v_mvis_y             : 4 ;  //  3: 0     
	UINT32 cmd_rd_stride_v_mvis_c             : 4 ;  //  7: 4     
	UINT32 reserved                           : 24;  // 31: 8     
} M14B0_MIFI_RD_CMD_STRIDE1_T;

/*-----------------------------------------------------------------------------
		0xc0020e38L mifi_rd_cmd_stride_set ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 man_cmd_rd_stride_v_prew_y         : 1 ;  //     0     
	UINT32 man_cmd_rd_stride_v_prew_c         : 1 ;  //     1     
	UINT32 man_cmd_rd_stride_v_mvi_y          : 1 ;  //     2     
	UINT32 man_cmd_rd_stride_v_mvi_c          : 1 ;  //     3     
	UINT32 man_cmd_rd_stride_v_prews_y        : 1 ;  //     4     
	UINT32 man_cmd_rd_stride_v_prews_c        : 1 ;  //     5     
	UINT32 man_cmd_rd_stride_v_mvis_y         : 1 ;  //     6     
	UINT32 man_cmd_rd_stride_v_mvis_c         : 1 ;  //     7     
	UINT32 reserved                           : 24;  // 31: 8     
} M14B0_MIFI_RD_CMD_STRIDE_SET_T;

/*-----------------------------------------------------------------------------
		0xc0020e3cL mifi_wd_cmd_stride0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_wd_stride_v_2_prew_y           : 4 ;  //  3: 0     
	UINT32 cmd_wd_stride_v_8_prew_y           : 4 ;  //  7: 4     
	UINT32 cmd_wd_stride_v_2_prew_c           : 4 ;  // 11: 8     
	UINT32 cmd_wd_stride_v_8_prew_c           : 4 ;  // 15:12     
	UINT32 cmd_wd_stride_v_prews_y            : 4 ;  // 19:16     
	UINT32 cmd_wd_stride_v_prews_c            : 4 ;  // 23:20     
	UINT32 cmd_wd_stride_v_preww_y            : 4 ;  // 27:24     
	UINT32 cmd_wd_stride_v_preww_c            : 4 ;  // 31:28     
} M14B0_MIFI_WD_CMD_STRIDE0_T;

/*-----------------------------------------------------------------------------
		0xc0020e40L mifi_wd_cmd_stride_set ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 man_cmd_wd_stride_v_prew_y         : 1 ;  //     0     
	UINT32 man_cmd_wd_stride_v_prew_c         : 1 ;  //     1     
	UINT32 man_cmd_wd_stride_v_prews_y        : 1 ;  //     2     
	UINT32 man_cmd_wd_stride_v_prews_c        : 1 ;  //     3     
	UINT32 man_cmd_wd_stride_v_preww_y        : 1 ;  //     4     
	UINT32 man_cmd_wd_stride_v_preww_c        : 1 ;  //     5     
	UINT32 reserved                           : 26;  // 31: 6     
} M14B0_MIFI_WD_CMD_STRIDE_SET_T;

/*-----------------------------------------------------------------------------
		0xc0020e44L mifi_rd_stride_prew_y ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_stride_2_prew_y             : 14;  // 13: 0     
	UINT32 reserved0                          : 2 ;  // 15:14     
	UINT32 cmd_rd_stride_8_prew_y             : 14;  // 29:16     
	UINT32 reserved1                          : 2 ;  // 31:30     
} M14B0_MIFI_RD_STRIDE_PREW_Y_T;

/*-----------------------------------------------------------------------------
		0xc0020e48L mifi_rd_stride_prew_c ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_stride_2_prew_c             : 14;  // 13: 0     
	UINT32 reserved0                          : 2 ;  // 15:14     
	UINT32 cmd_rd_stride_8_prew_c             : 14;  // 29:16     
	UINT32 reserved1                          : 2 ;  // 31:30     
} M14B0_MIFI_RD_STRIDE_PREW_C_T;

/*-----------------------------------------------------------------------------
		0xc0020e4cL mifi_rd_stride_mvi_yc ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_stride_mvi_y                : 14;  // 13: 0     
	UINT32 reserved0                          : 2 ;  // 15:14     
	UINT32 cmd_rd_stride_mvi_c                : 14;  // 29:16     
	UINT32 reserved1                          : 2 ;  // 31:30     
} M14B0_MIFI_RD_STRIDE_MVI_YC_T;

/*-----------------------------------------------------------------------------
		0xc0020e50L mifi_rd_stride_prews_yc ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_stride_prews_y              : 14;  // 13: 0     
	UINT32 reserved0                          : 2 ;  // 15:14     
	UINT32 cmd_rd_stride_prews_c              : 14;  // 29:16     
	UINT32 reserved1                          : 2 ;  // 31:30     
} M14B0_MIFI_RD_STRIDE_PREWS_YC_T;

/*-----------------------------------------------------------------------------
		0xc0020e54L mifi_rd_stride_mvis_yc ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_stride_mvis_y               : 14;  // 13: 0     
	UINT32 reserved0                          : 2 ;  // 15:14     
	UINT32 cmd_rd_stride_mvis_c               : 14;  // 29:16     
	UINT32 reserved1                          : 2 ;  // 31:30     
} M14B0_MIFI_RD_STRIDE_MVIS_YC_T;

/*-----------------------------------------------------------------------------
		0xc0020e58L mifi_rd_stride_mode0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_stride_md_prew_y            : 2 ;  //  1: 0     
	UINT32 cmd_rd_stride_md_prew_c            : 2 ;  //  3: 2     
	UINT32 cmd_rd_stride_md_mvi_y             : 2 ;  //  5: 4     
	UINT32 cmd_rd_stride_md_mvi_c             : 2 ;  //  7: 6     
	UINT32 cmd_rd_stride_md_prews_y           : 2 ;  //  9: 8     
	UINT32 cmd_rd_stride_md_prews_c           : 2 ;  // 11:10     
	UINT32 cmd_rd_stride_md_mvis_y            : 2 ;  // 13:12     
	UINT32 cmd_rd_stride_md_mvis_c            : 2 ;  // 15:14     
	UINT32 reserved                           : 16;  // 31:16     
} M14B0_MIFI_RD_STRIDE_MODE0_T;

/*-----------------------------------------------------------------------------
		0xc0020e5cL mifi_wd_stride_prew_y ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_wd_stride_2_prew_y             : 14;  // 13: 0     
	UINT32 reserved0                          : 2 ;  // 15:14     
	UINT32 cmd_wd_stride_8_prew_y             : 14;  // 29:16     
	UINT32 reserved1                          : 2 ;  // 31:30     
} M14B0_MIFI_WD_STRIDE_PREW_Y_T;

/*-----------------------------------------------------------------------------
		0xc0020e60L mifi_wd_stride_prew_c ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_wd_stride_2_prew_c             : 14;  // 13: 0     
	UINT32 reserved0                          : 2 ;  // 15:14     
	UINT32 cmd_wd_stride_8_prew_c             : 14;  // 29:16     
	UINT32 reserved1                          : 2 ;  // 31:30     
} M14B0_MIFI_WD_STRIDE_PREW_C_T;

/*-----------------------------------------------------------------------------
		0xc0020e64L mifi_wd_stride_prews_yc ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_wd_stride_prews_y              : 14;  // 13: 0     
	UINT32 reserved0                          : 2 ;  // 15:14     
	UINT32 cmd_wd_stride_prews_c              : 14;  // 29:16     
	UINT32 reserved1                          : 2 ;  // 31:30     
} M14B0_MIFI_WD_STRIDE_PREWS_YC_T;

/*-----------------------------------------------------------------------------
		0xc0020e68L mifi_wd_stride_preww_yc ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_wd_stride_preww_y              : 14;  // 13: 0     
	UINT32 reserved0                          : 2 ;  // 15:14     
	UINT32 cmd_wd_stride_preww_c              : 14;  // 29:16     
	UINT32 reserved1                          : 2 ;  // 31:30     
} M14B0_MIFI_WD_STRIDE_PREWW_YC_T;

/*-----------------------------------------------------------------------------
		0xc0020e6cL mifi_wd_stride_mode0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_wd_stride_md_prew_y            : 2 ;  //  1: 0     
	UINT32 cmd_wd_stride_md_prew_c            : 2 ;  //  3: 2     
	UINT32 cmd_wd_stride_md_prews_y           : 2 ;  //  5: 4     
	UINT32 cmd_wd_stride_md_prews_c           : 2 ;  //  7: 6     
	UINT32 cmd_wd_stride_md_preww_y           : 2 ;  //  9: 8     
	UINT32 cmd_wd_stride_md_preww_c           : 2 ;  // 11:10     
	UINT32 reserved                           : 20;  // 31:12     
} M14B0_MIFI_WD_STRIDE_MODE0_T;

/*-----------------------------------------------------------------------------
		0xc0020e70L mifi_rd_saddr_prew_y_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_saddr_2_prew_y              : 32;  // 31: 0     
} M14B0_MIFI_RD_SADDR_PREW_Y_2_T;

/*-----------------------------------------------------------------------------
		0xc0020e74L mifi_rd_saddr_prew_y_8 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_saddr_8_prew_y              : 32;  // 31: 0     
} M14B0_MIFI_RD_SADDR_PREW_Y_8_T;

/*-----------------------------------------------------------------------------
		0xc0020e78L mifi_rd_saddr_prew_c_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_saddr_2_prew_c              : 32;  // 31: 0     
} M14B0_MIFI_RD_SADDR_PREW_C_2_T;

/*-----------------------------------------------------------------------------
		0xc0020e7cL mifi_rd_saddr_prew_c_8 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_saddr_8_prew_c              : 32;  // 31: 0     
} M14B0_MIFI_RD_SADDR_PREW_C_8_T;

/*-----------------------------------------------------------------------------
		0xc0020e80L mifi_rd_saddr_mvi_y ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_saddr_mvi_y                 : 32;  // 31: 0     
} M14B0_MIFI_RD_SADDR_MVI_Y_T;

/*-----------------------------------------------------------------------------
		0xc0020e84L mifi_rd_saddr_mvi_c ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_saddr_mvi_c                 : 32;  // 31: 0     
} M14B0_MIFI_RD_SADDR_MVI_C_T;

/*-----------------------------------------------------------------------------
		0xc0020e88L mifi_rd_saddr_prews_y ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_saddr_prews_y               : 32;  // 31: 0     
} M14B0_MIFI_RD_SADDR_PREWS_Y_T;

/*-----------------------------------------------------------------------------
		0xc0020e8cL mifi_rd_saddr_prews_c ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_saddr_prews_c               : 32;  // 31: 0     
} M14B0_MIFI_RD_SADDR_PREWS_C_T;

/*-----------------------------------------------------------------------------
		0xc0020e90L mifi_rd_saddr_mvis_y ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_saddr_mvis_y                : 32;  // 31: 0     
} M14B0_MIFI_RD_SADDR_MVIS_Y_T;

/*-----------------------------------------------------------------------------
		0xc0020e94L mifi_rd_saddr_mvis_c ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_saddr_mvis_c                : 32;  // 31: 0     
} M14B0_MIFI_RD_SADDR_MVIS_C_T;

/*-----------------------------------------------------------------------------
		0xc0020e98L mifi_wd_saddr_prew_y_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_wd_saddr_2_prew_y              : 32;  // 31: 0     
} M14B0_MIFI_WD_SADDR_PREW_Y_2_T;

/*-----------------------------------------------------------------------------
		0xc0020e9cL mifi_wd_saddr_prew_y_8 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_wd_saddr_8_prew_y              : 32;  // 31: 0     
} M14B0_MIFI_WD_SADDR_PREW_Y_8_T;

/*-----------------------------------------------------------------------------
		0xc0020ea0L mifi_wd_saddr_prew_c_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_wd_saddr_2_prew_c              : 32;  // 31: 0     
} M14B0_MIFI_WD_SADDR_PREW_C_2_T;

/*-----------------------------------------------------------------------------
		0xc0020ea4L mifi_wd_saddr_prew_c_8 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_wd_saddr_8_prew_c              : 32;  // 31: 0     
} M14B0_MIFI_WD_SADDR_PREW_C_8_T;

/*-----------------------------------------------------------------------------
		0xc0020ea8L mifi_wd_saddr_prews_y ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_wd_saddr_prews_y               : 32;  // 31: 0     
} M14B0_MIFI_WD_SADDR_PREWS_Y_T;

/*-----------------------------------------------------------------------------
		0xc0020eacL mifi_wd_saddr_prews_c ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_wd_saddr_prews_c               : 32;  // 31: 0     
} M14B0_MIFI_WD_SADDR_PREWS_C_T;

/*-----------------------------------------------------------------------------
		0xc0020eb0L mifi_wd_saddr_preww_y ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_wd_saddr_preww_y               : 32;  // 31: 0     
} M14B0_MIFI_WD_SADDR_PREWW_Y_T;

/*-----------------------------------------------------------------------------
		0xc0020eb4L mifi_wd_saddr_preww_c ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_wd_saddr_preww_c               : 32;  // 31: 0     
} M14B0_MIFI_WD_SADDR_PREWW_C_T;

/*-----------------------------------------------------------------------------
		0xc0020eb8L mifi_wd_saddr_prew_y_r_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_wd_saddr_2_prew_y_r            : 32;  // 31: 0     
} M14B0_MIFI_WD_SADDR_PREW_Y_R_2_T;

/*-----------------------------------------------------------------------------
		0xc0020ebcL mifi_wd_saddr_prew_y_r_8 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_wd_saddr_8_prew_y_r            : 32;  // 31: 0     
} M14B0_MIFI_WD_SADDR_PREW_Y_R_8_T;

/*-----------------------------------------------------------------------------
		0xc0020ec0L mifi_wd_saddr_prew_c_r_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_wd_saddr_2_prew_c_r            : 32;  // 31: 0     
} M14B0_MIFI_WD_SADDR_PREW_C_R_2_T;

/*-----------------------------------------------------------------------------
		0xc0020ec4L mifi_wd_saddr_prew_c_r_8 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_wd_saddr_8_prew_c_r            : 32;  // 31: 0     
} M14B0_MIFI_WD_SADDR_PREW_C_R_8_T;

/*-----------------------------------------------------------------------------
		0xc0020ec8L mifi_status ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 fr_rd_valid_prew_y                 : 1 ;  //     0     
	UINT32 fr_rd_valid_prew_c                 : 1 ;  //     1     
	UINT32 fr_rd_valid_mvi_y                  : 1 ;  //     2     
	UINT32 fr_rd_valid_mvi_c                  : 1 ;  //     3     
	UINT32 fr_rd_valid_prews_y                : 1 ;  //     4     
	UINT32 fr_rd_valid_prews_c                : 1 ;  //     5     
	UINT32 fr_rd_valid_mvis_y                 : 1 ;  //     6     
	UINT32 fr_rd_valid_mvis_c                 : 1 ;  //     7     
	UINT32 fr_wd_q_full_prew_y                : 1 ;  //     8     
	UINT32 fr_wd_q_full_prew_c                : 1 ;  //     9     
	UINT32 fr_wd_q_full_prews_y               : 1 ;  //    10     
	UINT32 fr_wd_q_full_prews_c               : 1 ;  //    11     
	UINT32 fr_wd_q_full_preww_y               : 1 ;  //    12     
	UINT32 fr_wd_q_full_preww_c               : 1 ;  //    13     
	UINT32 reserved                           : 18;  // 31:14     
} M14B0_MIFI_STATUS_T;

/*-----------------------------------------------------------------------------
		0xc0020eccL mifi_lsb_switch ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_lsb_prew_y                  : 2 ;  //  1: 0     
	UINT32 req_rd_lsb_prew_c                  : 2 ;  //  3: 2     
	UINT32 reserved                           : 28;  // 31: 4     
} M14B0_MIFI_LSB_SWITCH_T;

/*-----------------------------------------------------------------------------
		0xc0020ed0L mifi_rd_req_pend0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_pend_v_prew_y               : 8 ;  //  7: 0     
	UINT32 req_rd_pend_v_prew_c               : 8 ;  // 15: 8     
	UINT32 req_rd_pend_v_mvi_y                : 8 ;  // 23:16     
	UINT32 req_rd_pend_v_mvi_c                : 8 ;  // 31:24     
} M14B0_MIFI_RD_REQ_PEND0_T;

/*-----------------------------------------------------------------------------
		0xc0020ed4L mifi_rd_req_pend1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_pend_v_prews_y              : 8 ;  //  7: 0     
	UINT32 req_rd_pend_v_prews_c              : 8 ;  // 15: 8     
	UINT32 req_rd_pend_v_mvis_y               : 8 ;  // 23:16     
	UINT32 req_rd_pend_v_mvis_c               : 8 ;  // 31:24     
} M14B0_MIFI_RD_REQ_PEND1_T;

/*-----------------------------------------------------------------------------
		0xc0020ed8L mifi_wd_req_pend0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_wd_pend_v_prew_y               : 8 ;  //  7: 0     
	UINT32 req_wd_pend_v_prew_c               : 8 ;  // 15: 8     
	UINT32 req_wd_pend_v_prews_y              : 8 ;  // 23:16     
	UINT32 req_wd_pend_v_prews_c              : 8 ;  // 31:24     
} M14B0_MIFI_WD_REQ_PEND0_T;

/*-----------------------------------------------------------------------------
		0xc0020edcL mifi_wd_req_pend1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_wd_pend_v_preww_y              : 8 ;  //  7: 0     
	UINT32 req_wd_pend_v_preww_c              : 8 ;  // 15: 8     
	UINT32 reserved                           : 16;  // 31:16     
} M14B0_MIFI_WD_REQ_PEND1_T;

/*-----------------------------------------------------------------------------
		0xc0020ee0L mifi_rd_cmd_pri0_s ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_pri_s_v_prew_y              : 4 ;  //  3: 0     
	UINT32 cmd_rd_pri_s_v_prew_c              : 4 ;  //  7: 4     
	UINT32 cmd_rd_pri_s_v_mvi_y               : 4 ;  // 11: 8     
	UINT32 cmd_rd_pri_s_v_mvi_c               : 4 ;  // 15:12     
	UINT32 cmd_rd_pri_s_v_prews_y             : 4 ;  // 19:16     
	UINT32 cmd_rd_pri_s_v_prews_c             : 4 ;  // 23:20     
	UINT32 cmd_rd_pri_s_v_mvis_y              : 4 ;  // 27:24     
	UINT32 cmd_rd_pri_s_v_mvis_c              : 4 ;  // 31:28     
} M14B0_MIFI_RD_CMD_PRI0_S_T;

/*-----------------------------------------------------------------------------
		0xc0020ee4L mifi_wd_cmd_pri0_s ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_wd_pri_s_v_prew_y              : 4 ;  //  3: 0     
	UINT32 cmd_wd_pri_s_v_prew_c              : 4 ;  //  7: 4     
	UINT32 cmd_wd_pri_s_v_prews_y             : 4 ;  // 11: 8     
	UINT32 cmd_wd_pri_s_v_prews_c             : 4 ;  // 15:12     
	UINT32 cmd_wd_pri_s_v_preww_y             : 4 ;  // 19:16     
	UINT32 cmd_wd_pri_s_v_preww_c             : 4 ;  // 23:20     
	UINT32 reserved                           : 8 ;  // 31:24     
} M14B0_MIFI_WD_CMD_PRI0_S_T;

/*-----------------------------------------------------------------------------
		0xc0020ee8L mifi_reset ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reset_mif_r_prew_y                 : 1 ;  //     0     
	UINT32 reset_mif_r_prew_c                 : 1 ;  //     1     
	UINT32 reset_mif_r_mvi_y                  : 1 ;  //     2     
	UINT32 reset_mif_r_mvi_c                  : 1 ;  //     3     
	UINT32 reset_mif_r_prews_y                : 1 ;  //     4     
	UINT32 reset_mif_r_prews_c                : 1 ;  //     5     
	UINT32 reset_mif_r_mvis_y                 : 1 ;  //     6     
	UINT32 reset_mif_r_mvis_c                 : 1 ;  //     7     
	UINT32 reset_mif_w_prew_y                 : 1 ;  //     8     
	UINT32 reset_mif_w_prew_c                 : 1 ;  //     9     
	UINT32 reset_mif_w_prews_y                : 1 ;  //    10     
	UINT32 reset_mif_w_prews_c                : 1 ;  //    11     
	UINT32 reset_mif_w_preww_y                : 1 ;  //    12     
	UINT32 reset_mif_w_preww_c                : 1 ;  //    13     
	UINT32 reserved                           : 6 ;  // 19:14     
	UINT32 reset_time_out_cnt                 : 12;  // 31:20     
} M14B0_MIFI_RESET_T;


/*-----------------------------------------------------------------------------
		0xc0022e00L mifm_rd_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 disable_rd_mif                     : 1 ;  //     0     
	UINT32 enable_rd_stv_cnt                  : 1 ;  //     1     
	UINT32 reserved0                          : 2 ;  //  3: 2
	UINT32 load_rd_req_stv_v                  : 1 ;  //     4     
	UINT32 load_rd_req_pri_v                  : 1 ;  //     5     
	UINT32 load_rd_cmd_saddr_v                : 1 ;  //     6     
	UINT32 load_rd_cmd_pend_v                 : 1 ;  //     7     
	UINT32 load_rd_cmd_pri_v                  : 1 ;  //     8     
	UINT32 load_rd_cmd_stride_v               : 1 ;  //     9     
	UINT32 load_rd_pic_init                   : 1 ;  //    10     
	UINT32 reserved1                          : 21;  // 31:11     
} M14B0_MIFM_RD_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc0022e04L mifm_wd_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 disable_wd_mif                     : 1 ;  //     0     
	UINT32 enable_wd_stv_cnt                  : 1 ;  //     1     
	UINT32 reserved0                          : 2 ;  //  3: 2
	UINT32 load_wd_req_stv_v                  : 1 ;  //     4     
	UINT32 load_wd_req_pri_v                  : 1 ;  //     5     
	UINT32 load_wd_cmd_saddr_v                : 1 ;  //     6     
	UINT32 load_wd_cmd_pend_v                 : 1 ;  //     7     
	UINT32 load_wd_cmd_pri_v                  : 1 ;  //     8     
	UINT32 load_wd_cmd_stride_v               : 1 ;  //     9     
	UINT32 load_wd_pic_init                   : 1 ;  //    10     
	UINT32 reserved1                          : 21;  // 31:11     
} M14B0_MIFM_WD_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc0022e08L mifm_rd_set_10b ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 set_10b_req_rd_ipc0_y              : 1 ;  //     0     
	UINT32 set_10b_req_rd_ipc0_c              : 1 ;  //     1     
	UINT32 set_10b_req_rd_ipc1_y              : 1 ;  //     2     
	UINT32 set_10b_req_rd_tnr0_y              : 1 ;  //     3     
	UINT32 set_10b_req_rd_tnr_c               : 1 ;  //     4     
	UINT32 set_10b_req_rd_tnr1_y              : 1 ;  //     5     
	UINT32 set_10b_req_rd_ipc1_c              : 1 ;  //     6     
	UINT32 reserved                           : 25;  // 31: 7     
} M14B0_MIFM_RD_SET_10B_T;

/*-----------------------------------------------------------------------------
		0xc0022e0cL mifm_wd_set_10b ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 set_10b_req_wd_tnr_y               : 1 ;  //     0     
	UINT32 set_10b_req_wd_tnr_c               : 1 ;  //     1     
	UINT32 reserved                           : 30;  // 31: 2     
} M14B0_MIFM_WD_SET_10B_T;

/*-----------------------------------------------------------------------------
		0xc0022e10L mifm_rd_cmd_pri0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_pri_v_ipc0_y                : 4 ;  //  3: 0     
	UINT32 cmd_rd_pri_v_ipc0_c                : 4 ;  //  7: 4     
	UINT32 cmd_rd_pri_v_ipc1_y                : 4 ;  // 11: 8     
	UINT32 cmd_rd_pri_v_tnr0_y                : 4 ;  // 15:12     
	UINT32 cmd_rd_pri_v_tnr_c                 : 4 ;  // 19:16     
	UINT32 cmd_rd_pri_v_tnr1_y                : 4 ;  // 23:20     
	UINT32 cmd_rd_pri_v_ipc_m                 : 4 ;  // 27:24     
	UINT32 cmd_rd_pri_v_ipc1_c                : 4 ;  // 31:28     
} M14B0_MIFM_RD_CMD_PRI0_T;

/*-----------------------------------------------------------------------------
		0xc0022e14L mifm_wd_cmd_pri0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_wd_pri_v_tnr_y                 : 4 ;  //  3: 0     
	UINT32 cmd_wd_pri_v_tnr_c                 : 4 ;  //  7: 4     
	UINT32 cmd_wd_pri_v_tnr_m                 : 4 ;  // 11: 8     
	UINT32 reserved                           : 20;  // 31:12     
} M14B0_MIFM_WD_CMD_PRI0_T;

/*-----------------------------------------------------------------------------
		0xc0022e18L mifm_rd_req_pri0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_pri_v_ipc0_y                : 4 ;  //  3: 0     
	UINT32 req_rd_pri_v_ipc0_c                : 4 ;  //  7: 4     
	UINT32 req_rd_pri_v_ipc1_y                : 4 ;  // 11: 8     
	UINT32 req_rd_pri_v_tnr0_y                : 4 ;  // 15:12     
	UINT32 req_rd_pri_v_tnr_c                 : 4 ;  // 19:16     
	UINT32 req_rd_pri_v_tnr1_y                : 4 ;  // 23:20     
	UINT32 req_rd_pri_v_ipc_m                 : 4 ;  // 27:24     
	UINT32 req_rd_pri_v_ipc1_c                : 4 ;  // 31:28     
} M14B0_MIFM_RD_REQ_PRI0_T;

/*-----------------------------------------------------------------------------
		0xc0022e1cL mifm_wd_req_pri0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_wd_pri_v_tnr_y                 : 4 ;  //  3: 0     
	UINT32 req_wd_pri_v_tnr_c                 : 4 ;  //  7: 4     
	UINT32 req_wd_pri_v_tnr_m                 : 4 ;  // 11: 8     
	UINT32 reserved                           : 20;  // 31:12     
} M14B0_MIFM_WD_REQ_PRI0_T;

/*-----------------------------------------------------------------------------
		0xc0022e20L mifm_rd_req_stv0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_stv_v_ipc0_y                : 4 ;  //  3: 0     
	UINT32 req_rd_stv_v_ipc0_c                : 4 ;  //  7: 4     
	UINT32 req_rd_stv_v_ipc1_y                : 4 ;  // 11: 8     
	UINT32 req_rd_stv_v_tnr0_y                : 4 ;  // 15:12     
	UINT32 req_rd_stv_v_tnr_c                 : 4 ;  // 19:16     
	UINT32 req_rd_stv_v_tnr1_y                : 4 ;  // 23:20     
	UINT32 req_rd_stv_v_ipc_m                 : 4 ;  // 27:24     
	UINT32 req_rd_stv_v_ipc1_c                : 4 ;  // 31:28     
} M14B0_MIFM_RD_REQ_STV0_T;

/*-----------------------------------------------------------------------------
		0xc0022e24L mifm_wd_req_stv0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_wd_stv_v_tnr_y                 : 4 ;  //  3: 0     
	UINT32 req_wd_stv_v_tnr_c                 : 4 ;  //  7: 4     
	UINT32 req_wd_stv_v_tnr_m                 : 4 ;  // 11: 8     
	UINT32 reserved                           : 20;  // 31:12     
} M14B0_MIFM_WD_REQ_STV0_T;

/*-----------------------------------------------------------------------------
		0xc0022e28L mifm_rd_cmd_pend0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_pend_v_ipc0_y               : 4 ;  //  3: 0     
	UINT32 cmd_rd_pend_v_ipc0_c               : 4 ;  //  7: 4     
	UINT32 cmd_rd_pend_v_ipc1_y               : 4 ;  // 11: 8     
	UINT32 cmd_rd_pend_v_tnr0_y               : 4 ;  // 15:12     
	UINT32 cmd_rd_pend_v_tnr_c                : 4 ;  // 19:16     
	UINT32 cmd_rd_pend_v_tnr1_y               : 4 ;  // 23:20     
	UINT32 cmd_rd_pend_v_ipc_m                : 4 ;  // 27:24     
	UINT32 cmd_rd_pend_v_ipc1_c               : 4 ;  // 31:28     
} M14B0_MIFM_RD_CMD_PEND0_T;

/*-----------------------------------------------------------------------------
		0xc0022e2cL mifm_wd_cmd_pend0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_wd_pend_v_tnr_y                : 4 ;  //  3: 0     
	UINT32 cmd_wd_pend_v_tnr_c                : 4 ;  //  7: 4     
	UINT32 cmd_wd_pend_v_tnr_m                : 4 ;  // 11: 8     
	UINT32 reserved                           : 20;  // 31:12     
} M14B0_MIFM_WD_CMD_PEND0_T;

/*-----------------------------------------------------------------------------
		0xc0022e30L mifm_rd_cmd_stride0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_stride_v_2_ipc0_y           : 4 ;  //  3: 0     
	UINT32 cmd_rd_stride_v_8_ipc0_y           : 4 ;  //  7: 4     
	UINT32 cmd_rd_stride_v_2_ipc0_c           : 4 ;  // 11: 8     
	UINT32 cmd_rd_stride_v_8_ipc0_c           : 4 ;  // 15:12     
	UINT32 cmd_rd_stride_v_2_ipc1_y           : 4 ;  // 19:16     
	UINT32 cmd_rd_stride_v_8_ipc1_y           : 4 ;  // 23:20     
	UINT32 cmd_rd_stride_v_2_tnr0_y           : 4 ;  // 27:24     
	UINT32 cmd_rd_stride_v_8_tnr0_y           : 4 ;  // 31:28     
} M14B0_MIFM_RD_CMD_STRIDE0_T;

/*-----------------------------------------------------------------------------
		0xc0022e34L mifm_rd_cmd_stride1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_stride_v_2_tnr_c            : 4 ;  //  3: 0     
	UINT32 cmd_rd_stride_v_8_tnr_c            : 4 ;  //  7: 4     
	UINT32 cmd_rd_stride_v_2_tnr1_y           : 4 ;  // 11: 8     
	UINT32 cmd_rd_stride_v_8_tnr1_y           : 4 ;  // 15:12     
	UINT32 cmd_rd_stride_v_ipc_m              : 4 ;  // 19:16     
	UINT32 cmd_rd_stride_v_2_ipc1_c           : 4 ;  // 23:20     
	UINT32 cmd_rd_stride_v_8_ipc1_c           : 4 ;  // 27:24     
	UINT32 reserved                           : 4 ;  // 31:28     
} M14B0_MIFM_RD_CMD_STRIDE1_T;

/*-----------------------------------------------------------------------------
		0xc0022e38L mifm_rd_cmd_stride_set ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 man_cmd_rd_stride_v_ipc0_y         : 1 ;  //     0     
	UINT32 man_cmd_rd_stride_v_ipc0_c         : 1 ;  //     1     
	UINT32 man_cmd_rd_stride_v_ipc1_y         : 1 ;  //     2     
	UINT32 man_cmd_rd_stride_v_tnr0_y         : 1 ;  //     3     
	UINT32 man_cmd_rd_stride_v_tnr_c          : 1 ;  //     4     
	UINT32 man_cmd_rd_stride_v_tnr1_y         : 1 ;  //     5     
	UINT32 man_cmd_rd_stride_v_ipc_m          : 1 ;  //     6     
	UINT32 man_cmd_rd_stride_v_ipc1_c         : 1 ;  //     7     
	UINT32 reserved                           : 24;  // 31: 8     
} M14B0_MIFM_RD_CMD_STRIDE_SET_T;

/*-----------------------------------------------------------------------------
		0xc0022e3cL mifm_wd_cmd_stride0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_wd_stride_v_2_tnr_y            : 4 ;  //  3: 0     
	UINT32 cmd_wd_stride_v_8_tnr_y            : 4 ;  //  7: 4     
	UINT32 cmd_wd_stride_v_2_tnr_c            : 4 ;  // 11: 8     
	UINT32 cmd_wd_stride_v_8_tnr_c            : 4 ;  // 15:12     
	UINT32 cmd_wd_stride_v_tnr_m              : 4 ;  // 19:16     
	UINT32 reserved                           : 12;  // 31:20     
} M14B0_MIFM_WD_CMD_STRIDE0_T;

/*-----------------------------------------------------------------------------
		0xc0022e40L mifm_wd_cmd_stride_set ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 man_cmd_wd_stride_v_tnr_y          : 1 ;  //     0     
	UINT32 man_cmd_wd_stride_v_tnr_c          : 1 ;  //     1     
	UINT32 man_cmd_wd_stride_v_tnr_m          : 1 ;  //     2     
	UINT32 reserved                           : 29;  // 31: 3     
} M14B0_MIFM_WD_CMD_STRIDE_SET_T;

/*-----------------------------------------------------------------------------
		0xc0022e44L mifm_rd_stride_ipc0_y ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_stride_2_ipc0_y             : 14;  // 13: 0     
	UINT32 reserved0                          : 2 ;  // 15:14     
	UINT32 cmd_rd_stride_8_ipc0_y             : 14;  // 29:16     
	UINT32 reserved1                          : 2 ;  // 31:30     
} M14B0_MIFM_RD_STRIDE_IPC0_Y_T;

/*-----------------------------------------------------------------------------
		0xc0022e48L mifm_rd_stride_ipc0_c ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_stride_2_ipc0_c             : 14;  // 13: 0     
	UINT32 reserved0                          : 2 ;  // 15:14     
	UINT32 cmd_rd_stride_8_ipc0_c             : 14;  // 29:16     
	UINT32 reserved1                          : 2 ;  // 31:30     
} M14B0_MIFM_RD_STRIDE_IPC0_C_T;

/*-----------------------------------------------------------------------------
		0xc0022e4cL mifm_rd_stride_ipc1_y ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_stride_2_ipc1_y             : 14;  // 13: 0     
	UINT32 reserved0                          : 2 ;  // 15:14     
	UINT32 cmd_rd_stride_8_ipc1_y             : 14;  // 29:16     
	UINT32 reserved1                          : 2 ;  // 31:30     
} M14B0_MIFM_RD_STRIDE_IPC1_Y_T;

/*-----------------------------------------------------------------------------
		0xc0022e50L mifm_rd_stride_tnr0_y ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_stride_2_tnr0_y             : 14;  // 13: 0     
	UINT32 reserved0                          : 2 ;  // 15:14     
	UINT32 cmd_rd_stride_8_tnr0_y             : 14;  // 29:16     
	UINT32 reserved1                          : 2 ;  // 31:30     
} M14B0_MIFM_RD_STRIDE_TNR0_Y_T;

/*-----------------------------------------------------------------------------
		0xc0022e54L mifm_rd_stride_tnr_c ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_stride_2_tnr_c              : 14;  // 13: 0     
	UINT32 reserved0                          : 2 ;  // 15:14     
	UINT32 cmd_rd_stride_8_tnr_c              : 14;  // 29:16     
	UINT32 reserved1                          : 2 ;  // 31:30     
} M14B0_MIFM_RD_STRIDE_TNR_C_T;

/*-----------------------------------------------------------------------------
		0xc0022e58L mifm_rd_stride_tnr1_y ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_stride_2_tnr1_y             : 14;  // 13: 0     
	UINT32 reserved0                          : 2 ;  // 15:14     
	UINT32 cmd_rd_stride_8_tnr1_y             : 14;  // 29:16     
	UINT32 reserved1                          : 2 ;  // 31:30     
} M14B0_MIFM_RD_STRIDE_TNR1_Y_T;

/*-----------------------------------------------------------------------------
		0xc0022e5cL mifm_rd_stride_ipc_m ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_stride_ipc_m                : 14;  // 13: 0     
	UINT32 reserved                           : 18;  // 31:14     
} M14B0_MIFM_RD_STRIDE_IPC_M_T;

/*-----------------------------------------------------------------------------
		0xc0022e60L mifm_rd_stride_ipc1_c ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_stride_2_ipc1_c             : 14;  // 13: 0     
	UINT32 reserved0                          : 2 ;  // 15:14     
	UINT32 cmd_rd_stride_8_ipc1_c             : 14;  // 29:16     
	UINT32 reserved1                          : 2 ;  // 31:30     
} M14B0_MIFM_RD_STRIDE_IPC1_C_T;

/*-----------------------------------------------------------------------------
		0xc0022e64L mifm_rd_stride_mode0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_stride_md_ipc0_y            : 2 ;  //  1: 0     
	UINT32 cmd_rd_stride_md_ipc0_c            : 2 ;  //  3: 2     
	UINT32 cmd_rd_stride_md_ipc1_y            : 2 ;  //  5: 4     
	UINT32 cmd_rd_stride_md_tnr0_y            : 2 ;  //  7: 6     
	UINT32 cmd_rd_stride_md_tnr_c             : 2 ;  //  9: 8     
	UINT32 cmd_rd_stride_md_tnr1_y            : 2 ;  // 11:10     
	UINT32 cmd_rd_stride_md_ipc_m             : 2 ;  // 13:12     
	UINT32 cmd_rd_stride_md_ipc1_c            : 2 ;  // 15:14     
	UINT32 reserved                           : 16;  // 31:16     
} M14B0_MIFM_RD_STRIDE_MODE0_T;

/*-----------------------------------------------------------------------------
		0xc0022e68L mifm_wd_stride_tnr_y ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_wd_stride_2_tnr_y              : 14;  // 13: 0     
	UINT32 reserved0                          : 2 ;  // 15:14     
	UINT32 cmd_wd_stride_8_tnr_y              : 14;  // 29:16     
	UINT32 reserved1                          : 2 ;  // 31:30     
} M14B0_MIFM_WD_STRIDE_TNR_Y_T;

/*-----------------------------------------------------------------------------
		0xc0022e6cL mifm_wd_stride_tnr_c ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_wd_stride_2_tnr_c              : 14;  // 13: 0     
	UINT32 reserved0                          : 2 ;  // 15:14     
	UINT32 cmd_wd_stride_8_tnr_c              : 14;  // 29:16     
	UINT32 reserved1                          : 2 ;  // 31:30     
} M14B0_MIFM_WD_STRIDE_TNR_C_T;

/*-----------------------------------------------------------------------------
		0xc0022e70L mifm_wd_stride_tnr_m ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_wd_stride_tnr_m                : 14;  // 13: 0     
	UINT32 reserved                           : 2 ;  // 15:14     
} M14B0_MIFM_WD_STRIDE_TNR_M_T;

/*-----------------------------------------------------------------------------
		0xc0022e74L mifm_wd_stride_mode0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_wd_stride_md_tnr_y             : 2 ;  //  1: 0     
	UINT32 cmd_wd_stride_md_tnr_c             : 2 ;  //  3: 2     
	UINT32 cmd_wd_stride_md_tnr_m             : 2 ;  //  5: 4     
	UINT32 reserved                           : 26;  // 31: 6     
} M14B0_MIFM_WD_STRIDE_MODE0_T;

/*-----------------------------------------------------------------------------
		0xc0022e78L mifm_rd_saddr_ipc0_y_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_saddr_2_ipc0_y              : 32;  // 31: 0     
} M14B0_MIFM_RD_SADDR_IPC0_Y_2_T;

/*-----------------------------------------------------------------------------
		0xc0022e7cL mifm_rd_saddr_ipc0_y_8 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_saddr_8_ipc0_y              : 32;  // 31: 0     
} M14B0_MIFM_RD_SADDR_IPC0_Y_8_T;

/*-----------------------------------------------------------------------------
		0xc0022e80L mifm_rd_saddr_ipc0_c_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_saddr_2_ipc0_c              : 32;  // 31: 0     
} M14B0_MIFM_RD_SADDR_IPC0_C_2_T;

/*-----------------------------------------------------------------------------
		0xc0022e84L mifm_rd_saddr_ipc0_c_8 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_saddr_8_ipc0_c              : 32;  // 31: 0     
} M14B0_MIFM_RD_SADDR_IPC0_C_8_T;

/*-----------------------------------------------------------------------------
		0xc0022e88L mifm_rd_saddr_ipc1_y_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_saddr_2_ipc1_y              : 32;  // 31: 0     
} M14B0_MIFM_RD_SADDR_IPC1_Y_2_T;

/*-----------------------------------------------------------------------------
		0xc0022e8cL mifm_rd_saddr_ipc1_y_8 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_saddr_8_ipc1_y              : 32;  // 31: 0     
} M14B0_MIFM_RD_SADDR_IPC1_Y_8_T;

/*-----------------------------------------------------------------------------
		0xc0022e90L mifm_rd_saddr_tnr0_y_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_saddr_2_tnr0_y              : 32;  // 31: 0     
} M14B0_MIFM_RD_SADDR_TNR0_Y_2_T;

/*-----------------------------------------------------------------------------
		0xc0022e94L mifm_rd_saddr_tnr0_y_8 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_saddr_8_tnr0_y              : 32;  // 31: 0     
} M14B0_MIFM_RD_SADDR_TNR0_Y_8_T;

/*-----------------------------------------------------------------------------
		0xc0022e98L mifm_rd_saddr_tnr_c_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_saddr_2_tnr_c               : 32;  // 31: 0     
} M14B0_MIFM_RD_SADDR_TNR_C_2_T;

/*-----------------------------------------------------------------------------
		0xc0022e9cL mifm_rd_saddr_tnr_c_8 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_saddr_8_tnr_c               : 32;  // 31: 0     
} M14B0_MIFM_RD_SADDR_TNR_C_8_T;

/*-----------------------------------------------------------------------------
		0xc0022ea0L mifm_rd_saddr_tnr1_y_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_saddr_2_tnr1_y              : 32;  // 31: 0     
} M14B0_MIFM_RD_SADDR_TNR1_Y_2_T;

/*-----------------------------------------------------------------------------
		0xc0022ea4L mifm_rd_saddr_tnr1_y_8 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_saddr_8_tnr1_y              : 32;  // 31: 0     
} M14B0_MIFM_RD_SADDR_TNR1_Y_8_T;

/*-----------------------------------------------------------------------------
		0xc0022ea8L mifm_rd_saddr_ipc_m ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_saddr_ipc_m                 : 32;  // 31: 0     
} M14B0_MIFM_RD_SADDR_IPC_M_T;

/*-----------------------------------------------------------------------------
		0xc0022eacL mifm_rd_saddr_ipc1_c_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_saddr_2_ipc1_c              : 32;  // 31: 0     
} M14B0_MIFM_RD_SADDR_IPC1_C_2_T;

/*-----------------------------------------------------------------------------
		0xc0022eb0L mifm_rd_saddr_ipc1_c_8 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_saddr_8_ipc1_c              : 32;  // 31: 0     
} M14B0_MIFM_RD_SADDR_IPC1_C_8_T;

/*-----------------------------------------------------------------------------
		0xc0022eb4L mifm_wd_saddr_tnr_y_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_wd_saddr_2_tnr_y               : 32;  // 31: 0     
} M14B0_MIFM_WD_SADDR_TNR_Y_2_T;

/*-----------------------------------------------------------------------------
		0xc0022eb8L mifm_wd_saddr_tnr_y_8 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_wd_saddr_8_tnr_y               : 32;  // 31: 0     
} M14B0_MIFM_WD_SADDR_TNR_Y_8_T;

/*-----------------------------------------------------------------------------
		0xc0022ebcL mifm_wd_saddr_tnr_c_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_wd_saddr_2_tnr_c               : 32;  // 31: 0     
} M14B0_MIFM_WD_SADDR_TNR_C_2_T;

/*-----------------------------------------------------------------------------
		0xc0022ec0L mifm_wd_saddr_tnr_c_8 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_wd_saddr_8_tnr_c               : 32;  // 31: 0     
} M14B0_MIFM_WD_SADDR_TNR_C_8_T;

/*-----------------------------------------------------------------------------
		0xc0022ec4L mifm_wd_saddr_tnr_m ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_wd_saddr_tnr_m                 : 32;  // 31: 0     
} M14B0_MIFM_WD_SADDR_TNR_M_T;

/*-----------------------------------------------------------------------------
		0xc0022ec8L mifm_status ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 fr_rd_valid_ipc0_y                 : 1 ;  //     0     
	UINT32 fr_rd_valid_ipc0_c                 : 1 ;  //     1     
	UINT32 fr_rd_valid_ipc1_y                 : 1 ;  //     2     
	UINT32 fr_rd_valid_tnr0_y                 : 1 ;  //     3     
	UINT32 fr_rd_valid_tnr_c                  : 1 ;  //     4     
	UINT32 fr_rd_valid_tnr1_y                 : 1 ;  //     5     
	UINT32 fr_rd_valid_ipc_m                  : 1 ;  //     6     
	UINT32 fr_rd_valid_ipc1_c                 : 1 ;  //     7     
	UINT32 fr_wd_q_full_tnr_y                 : 1 ;  //     8     
	UINT32 fr_wd_q_full_tnr_c                 : 1 ;  //     9     
	UINT32 fr_wd_q_full_tnr_m                 : 1 ;  //    10     
	UINT32 reserved                           : 21;  // 31:11     
} M14B0_MIFM_STATUS_T;

/*-----------------------------------------------------------------------------
		0xc0022eccL mifm_lsb_switch ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_lsb_ipc0_y                  : 2 ;  //  1: 0     
	UINT32 req_rd_lsb_ipc0_c                  : 2 ;  //  3: 2     
	UINT32 req_rd_lsb_ipc1_y                  : 2 ;  //  5: 4     
	UINT32 req_rd_lsb_tnr0_y                  : 2 ;  //  7: 6     
	UINT32 req_rd_lsb_tnr_c                   : 2 ;  //  9: 8     
	UINT32 req_rd_lsb_tnr1_y                  : 2 ;  // 11:10     
	UINT32 req_rd_lsb_ipc1_c                  : 2 ;  // 13:12     
	UINT32 reserved                           : 18;  // 31:14     
} M14B0_MIFM_LSB_SWITCH_T;

/*-----------------------------------------------------------------------------
		0xc0022ed0L mifm_rd_req_pend0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_pend_v_ipc0_y               : 8 ;  //  7: 0     
	UINT32 req_rd_pend_v_ipc0_c               : 8 ;  // 15: 8     
	UINT32 req_rd_pend_v_ipc1_y               : 8 ;  // 23:16     
	UINT32 req_rd_pend_v_tnr0_y               : 8 ;  // 31:24     
} M14B0_MIFM_RD_REQ_PEND0_T;

/*-----------------------------------------------------------------------------
		0xc0022ed4L mifm_rd_req_pend1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_pend_v_tnr_c                : 8 ;  //  7: 0     
	UINT32 req_rd_pend_v_tnr1_y               : 8 ;  // 15: 8     
	UINT32 req_rd_pend_v_ipc_m                : 8 ;  // 23:16     
	UINT32 req_rd_pend_v_ipc1_c               : 8 ;  // 31:24     
} M14B0_MIFM_RD_REQ_PEND1_T;

/*-----------------------------------------------------------------------------
		0xc0022ed8L mifm_wd_req_pend0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_wd_pend_v_tnr_y                : 8 ;  //  7: 0     
	UINT32 req_wd_pend_v_tnr_c                : 8 ;  // 15: 8     
	UINT32 req_wd_pend_v_tnr_m                : 8 ;  // 23:16     
	UINT32 reserved                           : 8 ;  // 31:24     
} M14B0_MIFM_WD_REQ_PEND0_T;

/*-----------------------------------------------------------------------------
		0xc0022edcL mifm_rd_cmd_pri0_s ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_pri_s_v_ipc0_y              : 4 ;  //  3: 0     
	UINT32 cmd_rd_pri_s_v_ipc0_c              : 4 ;  //  7: 4     
	UINT32 cmd_rd_pri_s_v_ipc1_y              : 4 ;  // 11: 8     
	UINT32 cmd_rd_pri_s_v_tnr0_y              : 4 ;  // 15:12     
	UINT32 cmd_rd_pri_s_v_tnr_c               : 4 ;  // 19:16     
	UINT32 cmd_rd_pri_s_v_tnr1_y              : 4 ;  // 23:20     
	UINT32 cmd_rd_pri_s_v_ipc_m               : 4 ;  // 27:24     
	UINT32 cmd_rd_pri_s_v_ipc1_c              : 4 ;  // 31:28     
} M14B0_MIFM_RD_CMD_PRI0_S_T;

/*-----------------------------------------------------------------------------
		0xc0022ee0L mifm_wd_cmd_pri0_s ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_wd_pri_s_v_tnr_y               : 4 ;  //  3: 0     
	UINT32 cmd_wd_pri_s_v_tnr_c               : 4 ;  //  7: 4     
	UINT32 cmd_wd_pri_s_v_tnr_m               : 4 ;  // 11: 8     
	UINT32 reserved                           : 20;  // 31:12     
} M14B0_MIFM_WD_CMD_PRI0_S_T;

/*-----------------------------------------------------------------------------
		0xc0022ee4L mifm_reset ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reset_mif_r_ipc0_y                 : 1 ;  //     0     
	UINT32 reset_mif_r_ipc0_c                 : 1 ;  //     1     
	UINT32 reset_mif_r_ipc1_y                 : 1 ;  //     2     
	UINT32 reset_mif_r_tnr0_y                 : 1 ;  //     3     
	UINT32 reset_mif_r_tnr_c                  : 1 ;  //     4     
	UINT32 reset_mif_r_tnr1_y                 : 1 ;  //     5     
	UINT32 reset_mif_r_ipc_m                  : 1 ;  //     6     
	UINT32 reset_mif_r_ipc1_c                 : 1 ;  //     7     
	UINT32 reset_mif_w_tnr_y                  : 1 ;  //     8     
	UINT32 reset_mif_w_tnr_c                  : 1 ;  //     9     
	UINT32 reset_mif_w_tnr_m                  : 1 ;  //    10     
	UINT32 reserved                           : 9 ;  // 19:11     
	UINT32 reset_time_out_cnt                 : 12;  // 31:20     
} M14B0_MIFM_RESET_T;

/*-----------------------------------------------------------------------------
		0xc0023e00L mifo_rd_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 disable_rd_mif                     : 1 ;  //     0     
	UINT32 enable_rd_stv_cnt                  : 1 ;  //     1     
	UINT32 reserved0                          : 2 ;  //  3: 2
	UINT32 load_rd_req_stv_v                  : 1 ;  //     4     
	UINT32 load_rd_req_pri_v                  : 1 ;  //     5     
	UINT32 load_rd_cmd_saddr_v                : 1 ;  //     6     
	UINT32 load_rd_cmd_pend_v                 : 1 ;  //     7     
	UINT32 load_rd_cmd_pri_v                  : 1 ;  //     8     
	UINT32 load_rd_cmd_stride_v               : 1 ;  //     9     
	UINT32 load_rd_pic_init                   : 1 ;  //    10     
	UINT32 reserved1                          : 21;  // 31:11     
} M14B0_MIFO_RD_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc0023e04L mifo_wd_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 disable_wd_mif                     : 1 ;  //     0     
	UINT32 enable_wd_stv_cnt                  : 1 ;  //     1     
	UINT32 reserved0                          : 2 ;  //  3: 2 
	UINT32 load_wd_req_stv_v                  : 1 ;  //     4     
	UINT32 load_wd_req_pri_v                  : 1 ;  //     5     
	UINT32 load_wd_cmd_saddr_v                : 1 ;  //     6     
	UINT32 load_wd_cmd_pend_v                 : 1 ;  //     7     
	UINT32 load_wd_cmd_pri_v                  : 1 ;  //     8     
	UINT32 load_wd_cmd_stride_v               : 1 ;  //     9     
	UINT32 load_wd_pic_init                   : 1 ;  //    10     
	UINT32 reserved1                          : 21;  // 31:11     
} M14B0_MIFO_WD_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc0023e08L mifo_rd_cmd_pri0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_pri_v_ipc0_y                : 4 ;  //  3: 0     
	UINT32 cmd_rd_pri_v_ipc0_c                : 4 ;  //  7: 4     
	UINT32 cmd_rd_pri_v_ipc1_y                : 4 ;  // 11: 8     
	UINT32 cmd_rd_pri_v_ipc1_c                : 4 ;  // 15:12     
	UINT32 cmd_rd_pri_v_tnr0_y                : 4 ;  // 19:16     
	UINT32 cmd_rd_pri_v_tnr_c                 : 4 ;  // 23:20     
	UINT32 cmd_rd_pri_v_cve_y                 : 4 ;  // 27:24     
	UINT32 cmd_rd_pri_v_cve_c                 : 4 ;  // 31:28     
} M14B0_MIFO_RD_CMD_PRI0_T;

/*-----------------------------------------------------------------------------
		0xc0023e0cL mifo_wd_cmd_pri0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_wd_pri_v_tnr_y                 : 4 ;  //  3: 0     
	UINT32 cmd_wd_pri_v_tnr_c                 : 4 ;  //  7: 4     
	UINT32 cmd_wd_pri_v_ss_y                  : 4 ;  // 11: 8     
	UINT32 cmd_wd_pri_v_ss_c                  : 4 ;  // 15:12     
	UINT32 reserved                           : 16;  // 31:16     
} M14B0_MIFO_WD_CMD_PRI0_T;

/*-----------------------------------------------------------------------------
		0xc0023e10L mifo_rd_req_pri0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_pri_v_ipc0_y                : 4 ;  //  3: 0     
	UINT32 req_rd_pri_v_ipc0_c                : 4 ;  //  7: 4     
	UINT32 req_rd_pri_v_ipc1_y                : 4 ;  // 11: 8     
	UINT32 req_rd_pri_v_ipc1_c                : 4 ;  // 15:12     
	UINT32 req_rd_pri_v_tnr0_y                : 4 ;  // 19:16     
	UINT32 req_rd_pri_v_tnr_c                 : 4 ;  // 23:20     
	UINT32 req_rd_pri_v_cve_y                 : 4 ;  // 27:24     
	UINT32 req_rd_pri_v_cve_c                 : 4 ;  // 31:28     
} M14B0_MIFO_RD_REQ_PRI0_T;

/*-----------------------------------------------------------------------------
		0xc0023e14L mifo_wd_req_pri0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_wd_pri_v_tnr_y                 : 4 ;  //  3: 0     
	UINT32 req_wd_pri_v_tnr_c                 : 4 ;  //  7: 4     
	UINT32 req_wd_pri_v_ss_y                  : 4 ;  // 11: 8     
	UINT32 req_wd_pri_v_ss_c                  : 4 ;  // 15:12     
	UINT32 reserved                           : 16;  // 31:16     
} M14B0_MIFO_WD_REQ_PRI0_T;

/*-----------------------------------------------------------------------------
		0xc0023e18L mifo_rd_req_stv0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_stv_v_ipc0_y                : 4 ;  //  3: 0     
	UINT32 req_rd_stv_v_ipc0_c                : 4 ;  //  7: 4     
	UINT32 req_rd_stv_v_ipc1_y                : 4 ;  // 11: 8     
	UINT32 req_rd_stv_v_ipc1_c                : 4 ;  // 15:12     
	UINT32 req_rd_stv_v_tnr0_y                : 4 ;  // 19:16     
	UINT32 req_rd_stv_v_tnr_c                 : 4 ;  // 23:20     
	UINT32 req_rd_stv_v_cve_y                 : 4 ;  // 27:24     
	UINT32 req_rd_stv_v_cve_c                 : 4 ;  // 31:28     
} M14B0_MIFO_RD_REQ_STV0_T;

/*-----------------------------------------------------------------------------
		0xc0023e1cL mifo_wd_req_stv0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_wd_stv_v_tnr_y                 : 4 ;  //  3: 0     
	UINT32 req_wd_stv_v_tnr_c                 : 4 ;  //  7: 4     
	UINT32 req_wd_stv_v_ss_y                  : 4 ;  // 11: 8     
	UINT32 req_wd_stv_v_ss_c                  : 4 ;  // 15:12     
	UINT32 reserved                           : 16;  // 31:16     
} M14B0_MIFO_WD_REQ_STV0_T;

/*-----------------------------------------------------------------------------
		0xc0023e20L mifo_rd_cmd_pend0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_pend_v_ipc0_y               : 4 ;  //  3: 0     
	UINT32 cmd_rd_pend_v_ipc0_c               : 4 ;  //  7: 4     
	UINT32 cmd_rd_pend_v_ipc1_y               : 4 ;  // 11: 8     
	UINT32 cmd_rd_pend_v_ipc1_c               : 4 ;  // 15:12     
	UINT32 cmd_rd_pend_v_tnr0_y               : 4 ;  // 19:16     
	UINT32 cmd_rd_pend_v_tnr_c                : 4 ;  // 23:20     
	UINT32 cmd_rd_pend_v_cve_y                : 4 ;  // 27:24     
	UINT32 cmd_rd_pend_v_cve_c                : 4 ;  // 31:28     
} M14B0_MIFO_RD_CMD_PEND0_T;

/*-----------------------------------------------------------------------------
		0xc0023e24L mifo_wd_cmd_pend0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_wd_pend_v_tnr_y                : 4 ;  //  3: 0     
	UINT32 cmd_wd_pend_v_tnr_c                : 4 ;  //  7: 4     
	UINT32 cmd_wd_pend_v_ss_y                 : 4 ;  // 11: 8     
	UINT32 cmd_wd_pend_v_ss_c                 : 4 ;  // 15:12     
	UINT32 reserved                           : 16;  // 31:16     
} M14B0_MIFO_WD_CMD_PEND0_T;

/*-----------------------------------------------------------------------------
		0xc0023e28L mifo_rd_cmd_stride0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_stride_v_ipc0_y             : 4 ;  //  3: 0     
	UINT32 cmd_rd_stride_v_ipc0_c             : 4 ;  //  7: 4     
	UINT32 cmd_rd_stride_v_ipc1_y             : 4 ;  // 11: 8     
	UINT32 cmd_rd_stride_v_ipc1_c             : 4 ;  // 15:12     
	UINT32 cmd_rd_stride_v_tnr0_y             : 4 ;  // 19:16     
	UINT32 cmd_rd_stride_v_tnr_c              : 4 ;  // 23:20     
	UINT32 cmd_rd_stride_v_cve_y              : 4 ;  // 27:24     
	UINT32 cmd_rd_stride_v_cve_c              : 4 ;  // 31:28     
} M14B0_MIFO_RD_CMD_STRIDE0_T;

/*-----------------------------------------------------------------------------
		0xc0023e2cL mifo_rd_cmd_stride_set ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 man_cmd_rd_stride_v_ipc0_y         : 1 ;  //     0     
	UINT32 man_cmd_rd_stride_v_ipc0_c         : 1 ;  //     1     
	UINT32 man_cmd_rd_stride_v_ipc1_y         : 1 ;  //     2     
	UINT32 man_cmd_rd_stride_v_ipc1_c         : 1 ;  //     3     
	UINT32 man_cmd_rd_stride_v_tnr0_y         : 1 ;  //     4     
	UINT32 man_cmd_rd_stride_v_tnr_c          : 1 ;  //     5     
	UINT32 man_cmd_rd_stride_v_cve_y          : 1 ;  //     6     
	UINT32 man_cmd_rd_stride_v_cve_c          : 1 ;  //     7     
	UINT32 reserved                           : 24;  // 31: 8     
} M14B0_MIFO_RD_CMD_STRIDE_SET_T;

/*-----------------------------------------------------------------------------
		0xc0023e30L mifo_wd_cmd_stride0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_wd_stride_v_tnr_y              : 4 ;  //  3: 0     
	UINT32 cmd_wd_stride_v_tnr_c              : 4 ;  //  7: 4     
	UINT32 cmd_wd_stride_v_ss_y               : 4 ;  // 11: 8     
	UINT32 cmd_wd_stride_v_ss_c               : 4 ;  // 15:12     
	UINT32 reserved                           : 16;  // 31:16     
} M14B0_MIFO_WD_CMD_STRIDE0_T;

/*-----------------------------------------------------------------------------
		0xc0023e34L mifo_wd_cmd_stride_set ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 man_cmd_wd_stride_v_tnr_y          : 1 ;  //     0     
	UINT32 man_cmd_wd_stride_v_tnr_c          : 1 ;  //     1     
	UINT32 man_cmd_wd_stride_v_ss_y           : 1 ;  //     2     
	UINT32 man_cmd_wd_stride_v_ss_c           : 1 ;  //     3     
	UINT32 reserved                           : 28;  // 31: 4     
} M14B0_MIFO_WD_CMD_STRIDE_SET_T;

/*-----------------------------------------------------------------------------
		0xc0023e38L mifo_rd_stride_ipc0_yc ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_stride_ipc0_y               : 14;  // 13: 0     
	UINT32 reserved0                          : 2 ;  // 15:14     
	UINT32 cmd_rd_stride_ipc0_c               : 14;  // 29:16     
	UINT32 reserved1                          : 2 ;  // 31:30     
} M14B0_MIFO_RD_STRIDE_IPC0_YC_T;

/*-----------------------------------------------------------------------------
		0xc0023e3cL mifo_rd_stride_ipc1_yc ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_stride_ipc1_y               : 14;  // 13: 0     
	UINT32 reserved0                          : 2 ;  // 15:14     
	UINT32 cmd_rd_stride_ipc1_c               : 14;  // 29:16     
	UINT32 reserved1                          : 2 ;  // 31:30     
} M14B0_MIFO_RD_STRIDE_IPC1_YC_T;

/*-----------------------------------------------------------------------------
		0xc0023e40L mifo_rd_stride_tnr_yc ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_stride_tnr_y                : 14;  // 13: 0     
	UINT32 reserved0                          : 2 ;  // 15:14     
	UINT32 cmd_rd_stride_tnr_c                : 14;  // 29:16     
	UINT32 reserved1                          : 2 ;  // 31:30     
} M14B0_MIFO_RD_STRIDE_TNR_YC_T;

/*-----------------------------------------------------------------------------
		0xc0023e44L mifo_rd_stride_cve_yc ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_stride_cve_y                : 14;  // 13: 0     
	UINT32 reserved0                          : 2 ;  // 15:14     
	UINT32 cmd_rd_stride_cve_c                : 14;  // 29:16     
	UINT32 reserved1                          : 2 ;  // 31:30     
} M14B0_MIFO_RD_STRIDE_CVE_YC_T;

/*-----------------------------------------------------------------------------
		0xc0023e48L mifo_rd_stride_mode0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_stride_md_ipc0_y            : 2 ;  //  1: 0     
	UINT32 cmd_rd_stride_md_ipc0_c            : 2 ;  //  3: 2     
	UINT32 cmd_rd_stride_md_ipc1_y            : 2 ;  //  5: 4     
	UINT32 cmd_rd_stride_md_ipc1_c            : 2 ;  //  7: 6     
	UINT32 cmd_rd_stride_md_tnr0_y            : 2 ;  //  9: 8     
	UINT32 cmd_rd_stride_md_tnr_c             : 2 ;  // 11:10     
	UINT32 cmd_rd_stride_md_cve_y             : 2 ;  // 13:12     
	UINT32 cmd_rd_stride_md_cve_c             : 2 ;  // 15:14     
	UINT32 reserved                           : 16;  // 31:16     
} M14B0_MIFO_RD_STRIDE_MODE0_T;

/*-----------------------------------------------------------------------------
		0xc0023e4cL mifo_wd_stride_tnr_yc ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_wd_stride_tnr_y                : 14;  // 13: 0     
	UINT32 reserved0                          : 2 ;  // 15:14     
	UINT32 cmd_wd_stride_tnr_c                : 14;  // 29:16     
	UINT32 reserved1                          : 2 ;  // 31:30     
} M14B0_MIFO_WD_STRIDE_TNR_YC_T;

/*-----------------------------------------------------------------------------
		0xc0023e50L mifo_wd_stride_ss_yc ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_wd_stride_ss_y                 : 14;  // 13: 0     
	UINT32 reserved0                          : 2 ;  // 15:14     
	UINT32 cmd_wd_stride_ss_c                 : 14;  // 29:16     
	UINT32 reserved1                          : 2 ;  // 31:30     
} M14B0_MIFO_WD_STRIDE_SS_YC_T;

/*-----------------------------------------------------------------------------
		0xc0023e54L mifo_wd_stride_mode0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_wd_stride_md_tnr_y             : 2 ;  //  1: 0     
	UINT32 cmd_wd_stride_md_tnr_c             : 2 ;  //  3: 2     
	UINT32 cmd_wd_stride_md_ss_y              : 2 ;  //  5: 4     
	UINT32 cmd_wd_stride_md_ss_c              : 2 ;  //  7: 6     
	UINT32 reserved                           : 24;  // 31: 8     
} M14B0_MIFO_WD_STRIDE_MODE0_T;

/*-----------------------------------------------------------------------------
		0xc0023e58L mifo_rd_saddr_ipc0_y ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_saddr_ipc0_y                : 32;  // 31: 0     
} M14B0_MIFO_RD_SADDR_IPC0_Y_T;

/*-----------------------------------------------------------------------------
		0xc0023e5cL mifo_rd_saddr_ipc0_c ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_saddr_ipc0_c                : 32;  // 31: 0     
} M14B0_MIFO_RD_SADDR_IPC0_C_T;

/*-----------------------------------------------------------------------------
		0xc0023e60L mifo_rd_saddr_ipc1_y ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_saddr_ipc1_y                : 32;  // 31: 0     
} M14B0_MIFO_RD_SADDR_IPC1_Y_T;

/*-----------------------------------------------------------------------------
		0xc0023e64L mifo_rd_saddr_ipc1_c ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_saddr_ipc1_c                : 32;  // 31: 0     
} M14B0_MIFO_RD_SADDR_IPC1_C_T;

/*-----------------------------------------------------------------------------
		0xc0023e68L mifo_rd_saddr_tnr0_y ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_saddr_tnr0_y                : 32;  // 31: 0     
} M14B0_MIFO_RD_SADDR_TNR0_Y_T;

/*-----------------------------------------------------------------------------
		0xc0023e6cL mifo_rd_saddr_tnr_c ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_saddr_tnr_c                 : 32;  // 31: 0     
} M14B0_MIFO_RD_SADDR_TNR_C_T;

/*-----------------------------------------------------------------------------
		0xc0023e70L mifo_rd_saddr_cve_y ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_saddr_cve_y                 : 32;  // 31: 0     
} M14B0_MIFO_RD_SADDR_CVE_Y_T;

/*-----------------------------------------------------------------------------
		0xc0023e74L mifo_rd_saddr_cve_c ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_saddr_cve_c                 : 32;  // 31: 0     
} M14B0_MIFO_RD_SADDR_CVE_C_T;

/*-----------------------------------------------------------------------------
		0xc0023e78L mifo_wd_saddr_tnr_y ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_wd_saddr_tnr_y                 : 32;  // 31: 0     
} M14B0_MIFO_WD_SADDR_TNR_Y_T;

/*-----------------------------------------------------------------------------
		0xc0023e7cL mifo_wd_saddr_tnr_c ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_wd_saddr_tnr_c                 : 32;  // 31: 0     
} M14B0_MIFO_WD_SADDR_TNR_C_T;

/*-----------------------------------------------------------------------------
		0xc0023e80L mifo_wd_saddr_ss_y ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_wd_saddr_ss_y                  : 32;  // 31: 0     
} M14B0_MIFO_WD_SADDR_SS_Y_T;

/*-----------------------------------------------------------------------------
		0xc0023e84L mifo_wd_saddr_ss_c ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_wd_saddr_ss_c                  : 32;  // 31: 0     
} M14B0_MIFO_WD_SADDR_SS_C_T;

/*-----------------------------------------------------------------------------
		0xc0023e88L mifo_status ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 fr_rd_valid_ipc0_y                 : 1 ;  //     0     
	UINT32 fr_rd_valid_ipc0_c                 : 1 ;  //     1     
	UINT32 fr_rd_valid_ipc1_y                 : 1 ;  //     2     
	UINT32 fr_rd_valid_ipc1_c                 : 1 ;  //     3     
	UINT32 fr_rd_valid_tnr0_y                 : 1 ;  //     4     
	UINT32 fr_rd_valid_tnr_c                  : 1 ;  //     5     
	UINT32 fr_rd_valid_cve_y                  : 1 ;  //     6     
	UINT32 fr_rd_valid_cve_c                  : 1 ;  //     7     
	UINT32 fr_wd_q_full_tnr_y                 : 1 ;  //     8     
	UINT32 fr_wd_q_full_tnr_c                 : 1 ;  //     9     
	UINT32 fr_wd_q_full_ss_y                  : 1 ;  //    10     
	UINT32 fr_wd_q_full_ss_c                  : 1 ;  //    11     
	UINT32 reserved                           : 20;  // 31:12     
} M14B0_MIFO_STATUS_T;

/*-----------------------------------------------------------------------------
		0xc0023e8cL mifo_rd_req_pend0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_pend_v_ipc0_y               : 8 ;  //  7: 0     
	UINT32 req_rd_pend_v_ipc0_c               : 8 ;  // 15: 8     
	UINT32 req_rd_pend_v_ipc1_y               : 8 ;  // 23:16     
	UINT32 req_rd_pend_v_ipc1_c               : 8 ;  // 31:24     
} M14B0_MIFO_RD_REQ_PEND0_T;

/*-----------------------------------------------------------------------------
		0xc0023e90L mifo_rd_req_pend1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_pend_v_tnr0_y               : 8 ;  //  7: 0     
	UINT32 req_rd_pend_v_tnr_c                : 8 ;  // 15: 8     
	UINT32 reserved                           : 16;  // 31:16     
} M14B0_MIFO_RD_REQ_PEND1_T;

/*-----------------------------------------------------------------------------
		0xc0023e94L mifo_wd_req_pend0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_wd_pend_v_tnr_y                : 8 ;  //  7: 0     
	UINT32 req_wd_pend_v_tnr_c                : 8 ;  // 15: 8     
	UINT32 req_wd_pend_v_ss_y                 : 8 ;  // 23:16     
	UINT32 req_wd_pend_v_ss_c                 : 8 ;  // 31:24     
} M14B0_MIFO_WD_REQ_PEND0_T;

/*-----------------------------------------------------------------------------
		0xc0023e98L mifo_rd_cmd_pri0_s ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_pri_s_v_ipc0_y              : 4 ;  //  3: 0     
	UINT32 cmd_rd_pri_s_v_ipc0_c              : 4 ;  //  7: 4     
	UINT32 cmd_rd_pri_s_v_ipc1_y              : 4 ;  // 11: 8     
	UINT32 cmd_rd_pri_s_v_ipc1_c              : 4 ;  // 15:12     
	UINT32 cmd_rd_pri_s_v_tnr0_y              : 4 ;  // 19:16     
	UINT32 cmd_rd_pri_s_v_tnr_c               : 4 ;  // 23:20     
	UINT32 cmd_rd_pri_s_v_cve_y               : 4 ;  // 27:24     
	UINT32 cmd_rd_pri_s_v_cve_c               : 4 ;  // 31:28     
} M14B0_MIFO_RD_CMD_PRI0_S_T;

/*-----------------------------------------------------------------------------
		0xc0023e9cL mifo_wd_cmd_pri0_s ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_wd_pri_s_v_tnr_y               : 4 ;  //  3: 0     
	UINT32 cmd_wd_pri_s_v_tnr_c               : 4 ;  //  7: 4     
	UINT32 cmd_wd_pri_s_v_ss_y                : 4 ;  // 11: 8     
	UINT32 cmd_wd_pri_s_v_ss_c                : 4 ;  // 15:12     
	UINT32 reserved                           : 16;  // 31:16     
} M14B0_MIFO_WD_CMD_PRI0_S_T;

/*-----------------------------------------------------------------------------
		0xc0023ea0L mifo_reset ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reset_mif_r_ipc0_y                 : 1 ;  //     0     
	UINT32 reset_mif_r_ipc0_c                 : 1 ;  //     1     
	UINT32 reset_mif_r_ipc1_y                 : 1 ;  //     2     
	UINT32 reset_mif_r_ipc1_c                 : 1 ;  //     3     
	UINT32 reset_mif_r_tnr0_y                 : 1 ;  //     4     
	UINT32 reset_mif_r_tnr_c                  : 1 ;  //     5     
	UINT32 reset_mif_r_cve_y                  : 1 ;  //     6     
	UINT32 reset_mif_r_cve_c                  : 1 ;  //     7     
	UINT32 reset_mif_w_tnr_y                  : 1 ;  //     8     
	UINT32 reset_mif_w_tnr_c                  : 1 ;  //     9     
	UINT32 reset_mif_w_ss_y                   : 1 ;  //    10     
	UINT32 reset_mif_w_ss_c                   : 1 ;  //    11     
	UINT32 reserved                           : 8 ;  // 19:12     
	UINT32 reset_time_out_cnt                 : 12;  // 31:20     
} M14B0_MIFO_RESET_T;

/*-----------------------------------------------------------------------------
		0xc0024e00L mifosd_rd_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reserved0                          : 12;  // 11: 0     
	UINT32 disable_rd_mif                     : 1 ;  //    12     
	UINT32 load_rd_req_stv_v                  : 1 ;  //    13     
	UINT32 load_rd_req_pri_v                  : 1 ;  //    14     
	UINT32 load_rd_cmd_saddr_v                : 1 ;  //    15     
	UINT32 load_rd_cmd_pend_v                 : 1 ;  //    16     
	UINT32 load_rd_cmd_pri_v                  : 1 ;  //    17     
	UINT32 reserved1                          : 1 ;  //    18
	UINT32 enable_rd_stv_cnt                  : 1 ;  //    19     
	UINT32 reserved2                          : 1 ;  //    20     
	UINT32 mrb_bank_mode                      : 3 ;  // 23:21     
	UINT32 reserved3                          : 8 ;  // 31:24     
} M14B0_MIFOSD_RD_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc0024e04L mifosd_rd_cmd_pri0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_pri_v_osd_0                 : 4 ;  //  3: 0     
	UINT32 cmd_rd_pri_v_osd_1                 : 4 ;  //  7: 4     
	UINT32 reserved0                          : 4 ;  // 11: 8     
	UINT32 cmd_rd_pri_v_osd_3                 : 4 ;  // 15:12     
	UINT32 reserved1                          : 16;  // 31:16     
} M14B0_MIFOSD_RD_CMD_PRI0_T;

/*-----------------------------------------------------------------------------
		0xc0024e08L mifosd_rd_req_pri0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_pri_v_osd_0                 : 4 ;  //  3: 0     
	UINT32 req_rd_pri_v_osd_1                 : 4 ;  //  7: 4     
	UINT32 reserved0                          : 4 ;  // 11: 8     
	UINT32 req_rd_pri_v_osd_3                 : 4 ;  // 15:12     
	UINT32 reserved1                          : 16;  // 31:16     
} M14B0_MIFOSD_RD_REQ_PRI0_T;

/*-----------------------------------------------------------------------------
		0xc0024e0cL mifosd_rd_req_stv0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_stv_v_osd_0                 : 4 ;  //  3: 0     
	UINT32 req_rd_stv_v_osd_1                 : 4 ;  //  7: 4     
	UINT32 reserved0                          : 4 ;  // 11: 8     
	UINT32 req_rd_stv_v_osd_3                 : 4 ;  // 15:12     
	UINT32 reserved1                          : 16;  // 31:16     
} M14B0_MIFOSD_RD_REQ_STV0_T;

/*-----------------------------------------------------------------------------
		0xc0024e10L mifosd_rd_cmd_pend0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_pend_v_osd_0                : 4 ;  //  3: 0     
	UINT32 cmd_rd_pend_v_osd_1                : 4 ;  //  7: 4     
	UINT32 reserved0                          : 4 ;  // 11: 8     
	UINT32 cmd_rd_pend_v_osd_3                : 4 ;  // 15:12     
	UINT32 reserved1                          : 16;  // 31:16     
} M14B0_MIFOSD_RD_CMD_PEND0_T;

/*-----------------------------------------------------------------------------
		0xc0024e14L mifosd_saddr_osd_0_l ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_saddr_osd_0_l               : 32;  // 31: 0     
} M14B0_MIFOSD_SADDR_OSD_0_L_T;

/*-----------------------------------------------------------------------------
		0xc0024e18L mifosd_saddr_osd_0_r ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_saddr_osd_0_r               : 32;  // 31: 0     
} M14B0_MIFOSD_SADDR_OSD_0_R_T;

/*-----------------------------------------------------------------------------
		0xc0024e1cL mifosd_saddr_osd_1_l ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_saddr_osd_1_l               : 32;  // 31: 0     
} M14B0_MIFOSD_SADDR_OSD_1_L_T;

/*-----------------------------------------------------------------------------
		0xc0024e20L mifosd_saddr_osd_1_r ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_saddr_osd_1_r               : 32;  // 31: 0     
} M14B0_MIFOSD_SADDR_OSD_1_R_T;

/*-----------------------------------------------------------------------------
		0xc0024e24L mifosd_saddr_osd_3_l ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_saddr_osd_3_l               : 32;  // 31: 0     
} M14B0_MIFOSD_SADDR_OSD_3_L_T;

/*-----------------------------------------------------------------------------
		0xc0024e28L mifosd_saddr_osd_3_r ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_saddr_osd_3_r               : 32;  // 31: 0     
} M14B0_MIFOSD_SADDR_OSD_3_R_T;

/*-----------------------------------------------------------------------------
		0xc0024e2cL mifosd_status ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 fr_rd_valid_osd_0                  : 1 ;  //     0     
	UINT32 fr_rd_valid_osd_1                  : 1 ;  //     1     
	UINT32 reserved0                          : 1 ;  //     2     
	UINT32 fr_rd_valid_osd_3                  : 1 ;  //     3     
	UINT32 reserved1                          : 28;  // 31: 4     
} M14B0_MIFOSD_STATUS_T;

/*-----------------------------------------------------------------------------
		0xc0024e30L mifosd_rd_req_pend0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_pend_v_osd_0                : 8 ;  //  7: 0     
	UINT32 req_rd_pend_v_osd_1                : 8 ;  // 15: 8     
	UINT32 reserved                           : 8 ;  // 23:16     
	UINT32 req_rd_pend_v_osd_3                : 8 ;  // 31:24     
} M14B0_MIFOSD_RD_REQ_PEND0_T;

/*-----------------------------------------------------------------------------
		0xc0024e34L mifosd_rd_cmd_pri0_s ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_pri_s_v_osd_0               : 4 ;  //  3: 0     
	UINT32 cmd_rd_pri_s_v_osd_1               : 4 ;  //  7: 4     
	UINT32 reserved0                          : 4 ;  // 11: 8     
	UINT32 cmd_rd_pri_s_v_osd_3               : 4 ;  // 15:12     
	UINT32 reserved1                          : 16;  // 31:16     
} M14B0_MIFOSD_RD_CMD_PRI0_S_T;

/*-----------------------------------------------------------------------------
		0xc0024e38L mifosd_reset ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reset_mif_r_osd_0                  : 1 ;  //     0     
	UINT32 reset_mif_r_osd_1                  : 1 ;  //     1     
	UINT32 reserved0                          : 1 ;  //     2     
	UINT32 reset_mif_r_osd_3                  : 1 ;  //     3     
	UINT32 reserved1                          : 16;  // 19: 4     
	UINT32 reset_time_out_cnt                 : 12;  // 31:20     
} M14B0_MIFOSD_RESET_T;

typedef struct {
	M14B0_MIFI_RD_CTRL_T               	mifi_rd_ctrl;                 //0xc0020e00L
	M14B0_MIFI_WD_CTRL_T               	mifi_wd_ctrl;                 //0xc0020e04L
	M14B0_MIFI_RD_SET_10B_T            	mifi_rd_set_10b;              //0xc0020e08L
	M14B0_MIFI_WD_SET_10B_T            	mifi_wd_set_10b;              //0xc0020e0cL
	M14B0_MIFI_RD_CMD_PRI0_T           	mifi_rd_cmd_pri0;             //0xc0020e10L
	M14B0_MIFI_WD_CMD_PRI0_T           	mifi_wd_cmd_pri0;             //0xc0020e14L
	M14B0_MIFI_RD_REQ_PRI0_T           	mifi_rd_req_pri0;             //0xc0020e18L
	M14B0_MIFI_WD_REQ_PRI0_T           	mifi_wd_req_pri0;             //0xc0020e1cL
	M14B0_MIFI_RD_REQ_STV0_T           	mifi_rd_req_stv0;             //0xc0020e20L
	M14B0_MIFI_WD_REQ_STV0_T           	mifi_wd_req_stv0;             //0xc0020e24L
	M14B0_MIFI_RD_CMD_PEND0_T          	mifi_rd_cmd_pend0;            //0xc0020e28L
	M14B0_MIFI_WD_CMD_PEND0_T          	mifi_wd_cmd_pend0;            //0xc0020e2cL
	M14B0_MIFI_RD_CMD_STRIDE0_T        	mifi_rd_cmd_stride0;          //0xc0020e30L
	M14B0_MIFI_RD_CMD_STRIDE1_T        	mifi_rd_cmd_stride1;          //0xc0020e34L
	M14B0_MIFI_RD_CMD_STRIDE_SET_T     	mifi_rd_cmd_stride_set;       //0xc0020e38L
	M14B0_MIFI_WD_CMD_STRIDE0_T        	mifi_wd_cmd_stride0;          //0xc0020e3cL
	M14B0_MIFI_WD_CMD_STRIDE_SET_T     	mifi_wd_cmd_stride_set;       //0xc0020e40L
	M14B0_MIFI_RD_STRIDE_PREW_Y_T      	mifi_rd_stride_prew_y;        //0xc0020e44L
	M14B0_MIFI_RD_STRIDE_PREW_C_T      	mifi_rd_stride_prew_c;        //0xc0020e48L
	M14B0_MIFI_RD_STRIDE_MVI_YC_T      	mifi_rd_stride_mvi_yc;        //0xc0020e4cL
	M14B0_MIFI_RD_STRIDE_PREWS_YC_T    	mifi_rd_stride_prews_yc;      //0xc0020e50L
	M14B0_MIFI_RD_STRIDE_MVIS_YC_T     	mifi_rd_stride_mvis_yc;       //0xc0020e54L
	M14B0_MIFI_RD_STRIDE_MODE0_T       	mifi_rd_stride_mode0;         //0xc0020e58L
	M14B0_MIFI_WD_STRIDE_PREW_Y_T      	mifi_wd_stride_prew_y;        //0xc0020e5cL
	M14B0_MIFI_WD_STRIDE_PREW_C_T      	mifi_wd_stride_prew_c;        //0xc0020e60L
	M14B0_MIFI_WD_STRIDE_PREWS_YC_T    	mifi_wd_stride_prews_yc;      //0xc0020e64L
	M14B0_MIFI_WD_STRIDE_PREWW_YC_T    	mifi_wd_stride_preww_yc;      //0xc0020e68L
	M14B0_MIFI_WD_STRIDE_MODE0_T       	mifi_wd_stride_mode0;         //0xc0020e6cL
	M14B0_MIFI_RD_SADDR_PREW_Y_2_T     	mifi_rd_saddr_prew_y_2;       //0xc0020e70L
	M14B0_MIFI_RD_SADDR_PREW_Y_8_T     	mifi_rd_saddr_prew_y_8;       //0xc0020e74L
	M14B0_MIFI_RD_SADDR_PREW_C_2_T     	mifi_rd_saddr_prew_c_2;       //0xc0020e78L
	M14B0_MIFI_RD_SADDR_PREW_C_8_T     	mifi_rd_saddr_prew_c_8;       //0xc0020e7cL
	M14B0_MIFI_RD_SADDR_MVI_Y_T        	mifi_rd_saddr_mvi_y;          //0xc0020e80L
	M14B0_MIFI_RD_SADDR_MVI_C_T        	mifi_rd_saddr_mvi_c;          //0xc0020e84L
	M14B0_MIFI_RD_SADDR_PREWS_Y_T      	mifi_rd_saddr_prews_y;        //0xc0020e88L
	M14B0_MIFI_RD_SADDR_PREWS_C_T      	mifi_rd_saddr_prews_c;        //0xc0020e8cL
	M14B0_MIFI_RD_SADDR_MVIS_Y_T       	mifi_rd_saddr_mvis_y;         //0xc0020e90L
	M14B0_MIFI_RD_SADDR_MVIS_C_T       	mifi_rd_saddr_mvis_c;         //0xc0020e94L
	M14B0_MIFI_WD_SADDR_PREW_Y_2_T     	mifi_wd_saddr_prew_y_2;       //0xc0020e98L
	M14B0_MIFI_WD_SADDR_PREW_Y_8_T     	mifi_wd_saddr_prew_y_8;       //0xc0020e9cL
	M14B0_MIFI_WD_SADDR_PREW_C_2_T     	mifi_wd_saddr_prew_c_2;       //0xc0020ea0L
	M14B0_MIFI_WD_SADDR_PREW_C_8_T     	mifi_wd_saddr_prew_c_8;       //0xc0020ea4L
	M14B0_MIFI_WD_SADDR_PREWS_Y_T      	mifi_wd_saddr_prews_y;        //0xc0020ea8L
	M14B0_MIFI_WD_SADDR_PREWS_C_T      	mifi_wd_saddr_prews_c;        //0xc0020eacL
	M14B0_MIFI_WD_SADDR_PREWW_Y_T      	mifi_wd_saddr_preww_y;        //0xc0020eb0L
	M14B0_MIFI_WD_SADDR_PREWW_C_T      	mifi_wd_saddr_preww_c;        //0xc0020eb4L
	M14B0_MIFI_WD_SADDR_PREW_Y_R_2_T   	mifi_wd_saddr_prew_y_r_2;     //0xc0020eb8L
	M14B0_MIFI_WD_SADDR_PREW_Y_R_8_T   	mifi_wd_saddr_prew_y_r_8;     //0xc0020ebcL
	M14B0_MIFI_WD_SADDR_PREW_C_R_2_T   	mifi_wd_saddr_prew_c_r_2;     //0xc0020ec0L
	M14B0_MIFI_WD_SADDR_PREW_C_R_8_T   	mifi_wd_saddr_prew_c_r_8;     //0xc0020ec4L
	M14B0_MIFI_STATUS_T                	mifi_status;                  //0xc0020ec8L
	M14B0_MIFI_LSB_SWITCH_T            	mifi_lsb_switch;              //0xc0020eccL
	M14B0_MIFI_RD_REQ_PEND0_T          	mifi_rd_req_pend0;            //0xc0020ed0L
	M14B0_MIFI_RD_REQ_PEND1_T          	mifi_rd_req_pend1;            //0xc0020ed4L
	M14B0_MIFI_WD_REQ_PEND0_T          	mifi_wd_req_pend0;            //0xc0020ed8L
	M14B0_MIFI_WD_REQ_PEND1_T          	mifi_wd_req_pend1;            //0xc0020edcL
	M14B0_MIFI_RD_CMD_PRI0_S_T         	mifi_rd_cmd_pri0_s;           //0xc0020ee0L
	M14B0_MIFI_WD_CMD_PRI0_S_T         	mifi_wd_cmd_pri0_s;           //0xc0020ee4L
	M14B0_MIFI_RESET_T                 	mifi_reset;                   //0xc0020ee8L
} DE_MII_REG_M14B0_T;

typedef struct {
	M14B0_MIFM_RD_CTRL_T               	mifm_rd_ctrl;                 //0xc0022e00L
	M14B0_MIFM_WD_CTRL_T               	mifm_wd_ctrl;                 //0xc0022e04L
	M14B0_MIFM_RD_SET_10B_T            	mifm_rd_set_10b;              //0xc0022e08L
	M14B0_MIFM_WD_SET_10B_T            	mifm_wd_set_10b;              //0xc0022e0cL
	M14B0_MIFM_RD_CMD_PRI0_T           	mifm_rd_cmd_pri0;             //0xc0022e10L
	M14B0_MIFM_WD_CMD_PRI0_T           	mifm_wd_cmd_pri0;             //0xc0022e14L
	M14B0_MIFM_RD_REQ_PRI0_T           	mifm_rd_req_pri0;             //0xc0022e18L
	M14B0_MIFM_WD_REQ_PRI0_T           	mifm_wd_req_pri0;             //0xc0022e1cL
	M14B0_MIFM_RD_REQ_STV0_T           	mifm_rd_req_stv0;             //0xc0022e20L
	M14B0_MIFM_WD_REQ_STV0_T           	mifm_wd_req_stv0;             //0xc0022e24L
	M14B0_MIFM_RD_CMD_PEND0_T          	mifm_rd_cmd_pend0;            //0xc0022e28L
	M14B0_MIFM_WD_CMD_PEND0_T          	mifm_wd_cmd_pend0;            //0xc0022e2cL
	M14B0_MIFM_RD_CMD_STRIDE0_T        	mifm_rd_cmd_stride0;          //0xc0022e30L
	M14B0_MIFM_RD_CMD_STRIDE1_T        	mifm_rd_cmd_stride1;          //0xc0022e34L
	M14B0_MIFM_RD_CMD_STRIDE_SET_T     	mifm_rd_cmd_stride_set;       //0xc0022e38L
	M14B0_MIFM_WD_CMD_STRIDE0_T        	mifm_wd_cmd_stride0;          //0xc0022e3cL
	M14B0_MIFM_WD_CMD_STRIDE_SET_T     	mifm_wd_cmd_stride_set;       //0xc0022e40L
	M14B0_MIFM_RD_STRIDE_IPC0_Y_T      	mifm_rd_stride_ipc0_y;        //0xc0022e44L
	M14B0_MIFM_RD_STRIDE_IPC0_C_T      	mifm_rd_stride_ipc0_c;        //0xc0022e48L
	M14B0_MIFM_RD_STRIDE_IPC1_Y_T      	mifm_rd_stride_ipc1_y;        //0xc0022e4cL
	M14B0_MIFM_RD_STRIDE_TNR0_Y_T      	mifm_rd_stride_tnr0_y;        //0xc0022e50L
	M14B0_MIFM_RD_STRIDE_TNR_C_T       	mifm_rd_stride_tnr_c;         //0xc0022e54L
	M14B0_MIFM_RD_STRIDE_TNR1_Y_T      	mifm_rd_stride_tnr1_y;        //0xc0022e58L
	M14B0_MIFM_RD_STRIDE_IPC_M_T       	mifm_rd_stride_ipc_m;         //0xc0022e5cL
	M14B0_MIFM_RD_STRIDE_IPC1_C_T      	mifm_rd_stride_ipc1_c;        //0xc0022e60L
	M14B0_MIFM_RD_STRIDE_MODE0_T       	mifm_rd_stride_mode0;         //0xc0022e64L
	M14B0_MIFM_WD_STRIDE_TNR_Y_T       	mifm_wd_stride_tnr_y;         //0xc0022e68L
	M14B0_MIFM_WD_STRIDE_TNR_C_T       	mifm_wd_stride_tnr_c;         //0xc0022e6cL
	M14B0_MIFM_WD_STRIDE_TNR_M_T       	mifm_wd_stride_tnr_m;         //0xc0022e70L
	M14B0_MIFM_WD_STRIDE_MODE0_T       	mifm_wd_stride_mode0;         //0xc0022e74L
	M14B0_MIFM_RD_SADDR_IPC0_Y_2_T     	mifm_rd_saddr_ipc0_y_2;       //0xc0022e78L
	M14B0_MIFM_RD_SADDR_IPC0_Y_8_T     	mifm_rd_saddr_ipc0_y_8;       //0xc0022e7cL
	M14B0_MIFM_RD_SADDR_IPC0_C_2_T     	mifm_rd_saddr_ipc0_c_2;       //0xc0022e80L
	M14B0_MIFM_RD_SADDR_IPC0_C_8_T     	mifm_rd_saddr_ipc0_c_8;       //0xc0022e84L
	M14B0_MIFM_RD_SADDR_IPC1_Y_2_T     	mifm_rd_saddr_ipc1_y_2;       //0xc0022e88L
	M14B0_MIFM_RD_SADDR_IPC1_Y_8_T     	mifm_rd_saddr_ipc1_y_8;       //0xc0022e8cL
	M14B0_MIFM_RD_SADDR_TNR0_Y_2_T     	mifm_rd_saddr_tnr0_y_2;       //0xc0022e90L
	M14B0_MIFM_RD_SADDR_TNR0_Y_8_T     	mifm_rd_saddr_tnr0_y_8;       //0xc0022e94L
	M14B0_MIFM_RD_SADDR_TNR_C_2_T      	mifm_rd_saddr_tnr_c_2;        //0xc0022e98L
	M14B0_MIFM_RD_SADDR_TNR_C_8_T      	mifm_rd_saddr_tnr_c_8;        //0xc0022e9cL
	M14B0_MIFM_RD_SADDR_TNR1_Y_2_T     	mifm_rd_saddr_tnr1_y_2;       //0xc0022ea0L
	M14B0_MIFM_RD_SADDR_TNR1_Y_8_T     	mifm_rd_saddr_tnr1_y_8;       //0xc0022ea4L
	M14B0_MIFM_RD_SADDR_IPC_M_T        	mifm_rd_saddr_ipc_m;          //0xc0022ea8L
	M14B0_MIFM_RD_SADDR_IPC1_C_2_T     	mifm_rd_saddr_ipc1_c_2;       //0xc0022eacL
	M14B0_MIFM_RD_SADDR_IPC1_C_8_T     	mifm_rd_saddr_ipc1_c_8;       //0xc0022eb0L
	M14B0_MIFM_WD_SADDR_TNR_Y_2_T      	mifm_wd_saddr_tnr_y_2;        //0xc0022eb4L
	M14B0_MIFM_WD_SADDR_TNR_Y_8_T      	mifm_wd_saddr_tnr_y_8;        //0xc0022eb8L
	M14B0_MIFM_WD_SADDR_TNR_C_2_T      	mifm_wd_saddr_tnr_c_2;        //0xc0022ebcL
	M14B0_MIFM_WD_SADDR_TNR_C_8_T      	mifm_wd_saddr_tnr_c_8;        //0xc0022ec0L
	M14B0_MIFM_WD_SADDR_TNR_M_T        	mifm_wd_saddr_tnr_m;          //0xc0022ec4L
	M14B0_MIFM_STATUS_T                	mifm_status;                  //0xc0022ec8L
	M14B0_MIFM_LSB_SWITCH_T            	mifm_lsb_switch;              //0xc0022eccL
	M14B0_MIFM_RD_REQ_PEND0_T          	mifm_rd_req_pend0;            //0xc0022ed0L
	M14B0_MIFM_RD_REQ_PEND1_T          	mifm_rd_req_pend1;            //0xc0022ed4L
	M14B0_MIFM_WD_REQ_PEND0_T          	mifm_wd_req_pend0;            //0xc0022ed8L
	M14B0_MIFM_RD_CMD_PRI0_S_T         	mifm_rd_cmd_pri0_s;           //0xc0022edcL
	M14B0_MIFM_WD_CMD_PRI0_S_T         	mifm_wd_cmd_pri0_s;           //0xc0022ee0L
	M14B0_MIFM_RESET_T                 	mifm_reset;                   //0xc0022ee4L
} DE_MIM_REG_M14B0_T;

typedef struct {
	M14B0_MIFO_RD_CTRL_T               	mifo_rd_ctrl;                 //0xc0023e00L
	M14B0_MIFO_WD_CTRL_T               	mifo_wd_ctrl;                 //0xc0023e04L
	M14B0_MIFO_RD_CMD_PRI0_T           	mifo_rd_cmd_pri0;             //0xc0023e08L
	M14B0_MIFO_WD_CMD_PRI0_T           	mifo_wd_cmd_pri0;             //0xc0023e0cL
	M14B0_MIFO_RD_REQ_PRI0_T           	mifo_rd_req_pri0;             //0xc0023e10L
	M14B0_MIFO_WD_REQ_PRI0_T           	mifo_wd_req_pri0;             //0xc0023e14L
	M14B0_MIFO_RD_REQ_STV0_T           	mifo_rd_req_stv0;             //0xc0023e18L
	M14B0_MIFO_WD_REQ_STV0_T           	mifo_wd_req_stv0;             //0xc0023e1cL
	M14B0_MIFO_RD_CMD_PEND0_T          	mifo_rd_cmd_pend0;            //0xc0023e20L
	M14B0_MIFO_WD_CMD_PEND0_T          	mifo_wd_cmd_pend0;            //0xc0023e24L
	M14B0_MIFO_RD_CMD_STRIDE0_T        	mifo_rd_cmd_stride0;          //0xc0023e28L
	M14B0_MIFO_RD_CMD_STRIDE_SET_T     	mifo_rd_cmd_stride_set;       //0xc0023e2cL
	M14B0_MIFO_WD_CMD_STRIDE0_T        	mifo_wd_cmd_stride0;          //0xc0023e30L
	M14B0_MIFO_WD_CMD_STRIDE_SET_T     	mifo_wd_cmd_stride_set;       //0xc0023e34L
	M14B0_MIFO_RD_STRIDE_IPC0_YC_T     	mifo_rd_stride_ipc0_yc;       //0xc0023e38L
	M14B0_MIFO_RD_STRIDE_IPC1_YC_T     	mifo_rd_stride_ipc1_yc;       //0xc0023e3cL
	M14B0_MIFO_RD_STRIDE_TNR_YC_T      	mifo_rd_stride_tnr_yc;        //0xc0023e40L
	M14B0_MIFO_RD_STRIDE_CVE_YC_T      	mifo_rd_stride_cve_yc;        //0xc0023e44L
	M14B0_MIFO_RD_STRIDE_MODE0_T       	mifo_rd_stride_mode0;         //0xc0023e48L
	M14B0_MIFO_WD_STRIDE_TNR_YC_T      	mifo_wd_stride_tnr_yc;        //0xc0023e4cL
	M14B0_MIFO_WD_STRIDE_SS_YC_T       	mifo_wd_stride_ss_yc;         //0xc0023e50L
	M14B0_MIFO_WD_STRIDE_MODE0_T       	mifo_wd_stride_mode0;         //0xc0023e54L
	M14B0_MIFO_RD_SADDR_IPC0_Y_T       	mifo_rd_saddr_ipc0_y;         //0xc0023e58L
	M14B0_MIFO_RD_SADDR_IPC0_C_T       	mifo_rd_saddr_ipc0_c;         //0xc0023e5cL
	M14B0_MIFO_RD_SADDR_IPC1_Y_T       	mifo_rd_saddr_ipc1_y;         //0xc0023e60L
	M14B0_MIFO_RD_SADDR_IPC1_C_T       	mifo_rd_saddr_ipc1_c;         //0xc0023e64L
	M14B0_MIFO_RD_SADDR_TNR0_Y_T       	mifo_rd_saddr_tnr0_y;         //0xc0023e68L
	M14B0_MIFO_RD_SADDR_TNR_C_T        	mifo_rd_saddr_tnr_c;          //0xc0023e6cL
	M14B0_MIFO_RD_SADDR_CVE_Y_T        	mifo_rd_saddr_cve_y;          //0xc0023e70L
	M14B0_MIFO_RD_SADDR_CVE_C_T        	mifo_rd_saddr_cve_c;          //0xc0023e74L
	M14B0_MIFO_WD_SADDR_TNR_Y_T        	mifo_wd_saddr_tnr_y;          //0xc0023e78L
	M14B0_MIFO_WD_SADDR_TNR_C_T        	mifo_wd_saddr_tnr_c;          //0xc0023e7cL
	M14B0_MIFO_WD_SADDR_SS_Y_T         	mifo_wd_saddr_ss_y;           //0xc0023e80L
	M14B0_MIFO_WD_SADDR_SS_C_T         	mifo_wd_saddr_ss_c;           //0xc0023e84L
	M14B0_MIFO_STATUS_T                	mifo_status;                  //0xc0023e88L
	M14B0_MIFO_RD_REQ_PEND0_T          	mifo_rd_req_pend0;            //0xc0023e8cL
	M14B0_MIFO_RD_REQ_PEND1_T          	mifo_rd_req_pend1;            //0xc0023e90L
	M14B0_MIFO_WD_REQ_PEND0_T          	mifo_wd_req_pend0;            //0xc0023e94L
	M14B0_MIFO_RD_CMD_PRI0_S_T         	mifo_rd_cmd_pri0_s;           //0xc0023e98L
	M14B0_MIFO_WD_CMD_PRI0_S_T         	mifo_wd_cmd_pri0_s;           //0xc0023e9cL
	M14B0_MIFO_RESET_T                 	mifo_reset;                   //0xc0023ea0L
} DE_MIO_REG_M14B0_T;

typedef struct {
	M14B0_MIFOSD_RD_CTRL_T             	mifosd_rd_ctrl;               //0xc0024e00L
	M14B0_MIFOSD_RD_CMD_PRI0_T         	mifosd_rd_cmd_pri0;           //0xc0024e04L
	M14B0_MIFOSD_RD_REQ_PRI0_T         	mifosd_rd_req_pri0;           //0xc0024e08L
	M14B0_MIFOSD_RD_REQ_STV0_T         	mifosd_rd_req_stv0;           //0xc0024e0cL
	M14B0_MIFOSD_RD_CMD_PEND0_T        	mifosd_rd_cmd_pend0;          //0xc0024e10L
	M14B0_MIFOSD_SADDR_OSD_0_L_T       	mifosd_saddr_osd_0_l;         //0xc0024e14L
	M14B0_MIFOSD_SADDR_OSD_0_R_T       	mifosd_saddr_osd_0_r;         //0xc0024e18L
	M14B0_MIFOSD_SADDR_OSD_1_L_T       	mifosd_saddr_osd_1_l;         //0xc0024e1cL
	M14B0_MIFOSD_SADDR_OSD_1_R_T       	mifosd_saddr_osd_1_r;         //0xc0024e20L
	M14B0_MIFOSD_SADDR_OSD_3_L_T       	mifosd_saddr_osd_3_l;         //0xc0024e24L
	M14B0_MIFOSD_SADDR_OSD_3_R_T       	mifosd_saddr_osd_3_r;         //0xc0024e28L
	M14B0_MIFOSD_STATUS_T              	mifosd_status;                //0xc0024e2cL
	M14B0_MIFOSD_RD_REQ_PEND0_T        	mifosd_rd_req_pend0;          //0xc0024e30L
	M14B0_MIFOSD_RD_CMD_PRI0_S_T       	mifosd_rd_cmd_pri0_s;         //0xc0024e34L
	M14B0_MIFOSD_RESET_T               	mifosd_reset;                 //0xc0024e38L
} DE_MIOSD_REG_M14B0_T;

#endif
