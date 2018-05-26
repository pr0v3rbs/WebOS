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

#ifndef _REG_MIF_H14A0_h
#define _REG_MIF_H14A0_h

/*-----------------------------------------------------------------------------
	0x0e00 mifa_rd_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                 : 12;  //  0:11 reserved
	UINT32 disable_rd_mif                  : 1;   //    12
	UINT32 load_rd_req_stv_v               : 1;   //    13
	UINT32 load_rd_req_pri_v               : 1;   //    14
	UINT32 load_rd_cmd_saddr_v             : 1;   //    15
	UINT32 load_rd_cmd_pend_v              : 1;   //    16
	UINT32 load_rd_cmd_pri_v               : 1;   //    17
	UINT32 load_rd_cmd_stride_v            : 1;   //    18
	UINT32 enable_rd_stv_cnt               : 1;   //    19
	UINT32                                 : 1;   //    20 reserved
	UINT32 mrb_bank_mode                   : 3;   // 21:23
} H14A0_MIFA_RD_CTRL_T;

/*-----------------------------------------------------------------------------
	0x0e04 mifa_wd_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                 : 12;  //  0:11 reserved
	UINT32 disable_wd_mif                  : 1;   //    12
	UINT32 load_wd_req_stv_v               : 1;   //    13
	UINT32 load_wd_req_pri_v               : 1;   //    14
	UINT32 load_wd_cmd_saddr_v             : 1;   //    15
	UINT32 load_wd_cmd_pend_v              : 1;   //    16
	UINT32 load_wd_cmd_pri_v               : 1;   //    17
	UINT32 load_wd_cmd_stride_v            : 1;   //    18
	UINT32 enable_wd_stv_cnt               : 1;   //    19
	UINT32 enable_prew_wd_addr_pls         : 1;   //    20
	UINT32 mwb_bank_mode                   : 3;   // 21:23
} H14A0_MIFA_WD_CTRL_T;

/*-----------------------------------------------------------------------------
	0x0e08 mifa_rd_set_10b ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 set_10b_req_rd_prew_y           : 1;   //     0
	UINT32 set_10b_req_rd_prew_c           : 1;   //     1
	UINT32 set_10b_req_rd_ipc0_y           : 1;   //     2
	UINT32 set_10b_req_rd_ipc0_c           : 1;   //     3
	UINT32 set_10b_req_rd_ipc1_y           : 1;   //     4
	UINT32 set_10b_req_rd_tnr0_y           : 1;   //     5
	UINT32 set_10b_req_rd_tnr_c            : 1;   //     6
	UINT32 set_10b_req_rd_tnr1_y           : 1;   //     7
	UINT32 set_10b_req_rd_ipc1_c           : 1;   //     8
} H14A0_MIFA_RD_SET_10B_T;

/*-----------------------------------------------------------------------------
	0x0e0c mifa_wd_set_10b ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 set_10b_req_wd_tnr_y            : 1;   //     0
	UINT32 set_10b_req_wd_tnr_c            : 1;   //     1
	UINT32 set_10b_req_wd_prew_y           : 1;   //     2
	UINT32 set_10b_req_wd_prew_c           : 1;   //     3
} H14A0_MIFA_WD_SET_10B_T;

/*-----------------------------------------------------------------------------
	0x0e10 mifa_rd_cmd_pri0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_pri_v_mvi_y              : 4;   //  0: 3
	UINT32 cmd_rd_pri_v_mvi_c              : 4;   //  4: 7
	UINT32 cmd_rd_pri_v_prew_y             : 4;   //  8:11
	UINT32 cmd_rd_pri_v_prew_c             : 4;   // 12:15
	UINT32 cmd_rd_pri_v_ipc0_y             : 4;   // 16:19
	UINT32 cmd_rd_pri_v_ipc0_c             : 4;   // 20:23
	UINT32 cmd_rd_pri_v_ipc1_y             : 4;   // 24:27
	UINT32 cmd_rd_pri_v_tnr0_y             : 4;   // 28:31
} H14A0_MIFA_RD_CMD_PRI0_T;

/*-----------------------------------------------------------------------------
	0x0e14 mifa_rd_cmd_pri1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_pri_v_tnr_c              : 4;   //  0: 3
	UINT32 cmd_rd_pri_v_tnr1_y             : 4;   //  4: 7
	UINT32 cmd_rd_pri_v_ipc_m              : 4;   //  8:11
	UINT32 cmd_rd_pri_v_ipc1_c             : 4;   // 12:15
} H14A0_MIFA_RD_CMD_PRI1_T;

/*-----------------------------------------------------------------------------
	0x0e18 mifa_wd_cmd_pri0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_wd_pri_v_tnr_y              : 4;   //  0: 3
	UINT32 cmd_wd_pri_v_tnr_c              : 4;   //  4: 7
	UINT32 cmd_wd_pri_v_prew_y             : 4;   //  8:11
	UINT32 cmd_wd_pri_v_prew_c             : 4;   // 12:15
	UINT32 cmd_wd_pri_v_tnr_m              : 4;   // 16:19
} H14A0_MIFA_WD_CMD_PRI0_T;

/*-----------------------------------------------------------------------------
	0x0e1c mifa_rd_req_pri0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_pri_v_mvi_y              : 4;   //  0: 3
	UINT32 req_rd_pri_v_mvi_c              : 4;   //  4: 7
	UINT32 req_rd_pri_v_prew_y             : 4;   //  8:11
	UINT32 req_rd_pri_v_prew_c             : 4;   // 12:15
	UINT32 req_rd_pri_v_ipc0_y             : 4;   // 16:19
	UINT32 req_rd_pri_v_ipc0_c             : 4;   // 20:23
	UINT32 req_rd_pri_v_ipc1_y             : 4;   // 24:27
	UINT32 req_rd_pri_v_tnr0_y             : 4;   // 28:31
} H14A0_MIFA_RD_REQ_PRI0_T;

/*-----------------------------------------------------------------------------
	0x0e20 mifa_rd_req_pri1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_pri_v_tnr_c              : 4;   //  0: 3
	UINT32 req_rd_pri_v_tnr1_y             : 4;   //  4: 7
	UINT32 req_rd_pri_v_ipc_m              : 4;   //  8:11
	UINT32 req_rd_pri_v_ipc1_c             : 4;   // 12:15
} H14A0_MIFA_RD_REQ_PRI1_T;

/*-----------------------------------------------------------------------------
	0x0e24 mifa_wd_req_pri0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_wd_pri_v_tnr_y              : 4;   //  0: 3
	UINT32 req_wd_pri_v_tnr_c              : 4;   //  4: 7
	UINT32 req_wd_pri_v_prew_y             : 4;   //  8:11
	UINT32 req_wd_pri_v_prew_c             : 4;   // 12:15
	UINT32 req_wd_pri_v_tnr_m              : 4;   // 16:19
} H14A0_MIFA_WD_REQ_PRI0_T;

/*-----------------------------------------------------------------------------
	0x0e28 mifa_rd_req_stv0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_stv_v_mvi_y              : 4;   //  0: 3
	UINT32 req_rd_stv_v_mvi_c              : 4;   //  4: 7
	UINT32 req_rd_stv_v_prew_y             : 4;   //  8:11
	UINT32 req_rd_stv_v_prew_c             : 4;   // 12:15
	UINT32 req_rd_stv_v_ipc0_y             : 4;   // 16:19
	UINT32 req_rd_stv_v_ipc0_c             : 4;   // 20:23
	UINT32 req_rd_stv_v_ipc1_y             : 4;   // 24:27
	UINT32 req_rd_stv_v_tnr0_y             : 4;   // 28:31
} H14A0_MIFA_RD_REQ_STV0_T;

/*-----------------------------------------------------------------------------
	0x0e2c mifa_rd_req_stv1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_stv_v_tnr_c              : 4;   //  0: 3
	UINT32 req_rd_stv_v_tnr1_y             : 4;   //  4: 7
	UINT32 req_rd_stv_v_ipc_m              : 4;   //  8:11
	UINT32 req_rd_stv_v_ipc1_c             : 4;   // 12:15
} H14A0_MIFA_RD_REQ_STV1_T;

/*-----------------------------------------------------------------------------
	0x0e30 mifa_wd_req_stv0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_wd_stv_v_tnr_y              : 4;   //  0: 3
	UINT32 req_wd_stv_v_tnr_c              : 4;   //  4: 7
	UINT32 req_wd_stv_v_prew_y             : 4;   //  8:11
	UINT32 req_wd_stv_v_prew_c             : 4;   // 12:15
	UINT32                                 : 1;   //    16 reserved
	UINT32 req_wd_stv_v_tnr_m              : 3;   // 17:19
} H14A0_MIFA_WD_REQ_STV0_T;

/*-----------------------------------------------------------------------------
	0x0e34 mifa_rd_cmd_pend0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_pend_v_mvi_y             : 4;   //  0: 3
	UINT32 cmd_rd_pend_v_mvi_c             : 4;   //  4: 7
	UINT32 cmd_rd_pend_v_prew_y            : 4;   //  8:11
	UINT32 cmd_rd_pend_v_prew_c            : 4;   // 12:15
	UINT32 cmd_rd_pend_v_ipc0_y            : 4;   // 16:19
	UINT32 cmd_rd_pend_v_ipc0_c            : 4;   // 20:23
	UINT32 cmd_rd_pend_v_ipc1_y            : 4;   // 24:27
	UINT32 cmd_rd_pend_v_tnr0_y            : 4;   // 28:31
} H14A0_MIFA_RD_CMD_PEND0_T;

/*-----------------------------------------------------------------------------
	0x0e38 mifa_rd_cmd_pend1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_pend_v_tnr_c             : 4;   //  0: 3
	UINT32 cmd_rd_pend_v_tnr1_y            : 4;   //  4: 7
	UINT32 cmd_rd_pend_v_ipc_m             : 4;   //  8:11
	UINT32 cmd_rd_pend_v_ipc1_c            : 4;   // 12:15
} H14A0_MIFA_RD_CMD_PEND1_T;

/*-----------------------------------------------------------------------------
	0x0e3c mifa_wd_cmd_pend0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_wd_pend_v_tnr_y             : 4;   //  0: 3
	UINT32 cmd_wd_pend_v_tnr_c             : 4;   //  4: 7
	UINT32 cmd_wd_pend_v_prew_y            : 4;   //  8:11
	UINT32 cmd_wd_pend_v_prew_c            : 4;   // 12:15
	UINT32 cmd_wd_pend_v_tnr_m             : 4;   // 16:19
} H14A0_MIFA_WD_CMD_PEND0_T;

/*-----------------------------------------------------------------------------
	0x0e40 mifa_rd_cmd_stride0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_stride_v_mvi_y           : 4;   //  0: 3
	UINT32 cmd_rd_stride_v_mvi_c           : 4;   //  4: 7
	UINT32 cmd_rd_stride_v_2_prew_y        : 4;   //  8:11
	UINT32 cmd_rd_stride_v_8_prew_y        : 4;   // 12:15
	UINT32 cmd_rd_stride_v_2_prew_c        : 4;   // 16:19
	UINT32 cmd_rd_stride_v_8_prew_c        : 4;   // 20:23
	UINT32 cmd_rd_stride_v_2_ipc0_y        : 4;   // 24:27
	UINT32 cmd_rd_stride_v_8_ipc0_y        : 4;   // 28:31
} H14A0_MIFA_RD_CMD_STRIDE0_T;

/*-----------------------------------------------------------------------------
	0x0e44 mifa_rd_cmd_stride1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_stride_v_2_ipc0_c        : 4;   //  0: 3
	UINT32 cmd_rd_stride_v_8_ipc0_c        : 4;   //  4: 7
	UINT32 cmd_rd_stride_v_2_ipc1_y        : 4;   //  8:11
	UINT32 cmd_rd_stride_v_8_ipc1_y        : 4;   // 12:15
	UINT32 cmd_rd_stride_v_2_tnr0_y        : 4;   // 16:19
	UINT32 cmd_rd_stride_v_8_tnr0_y        : 4;   // 20:23
	UINT32 cmd_rd_stride_v_2_tnr_c         : 4;   // 24:27
	UINT32 cmd_rd_stride_v_8_tnr_c         : 4;   // 28:31
} H14A0_MIFA_RD_CMD_STRIDE1_T;

/*-----------------------------------------------------------------------------
	0x0e48 mifa_rd_cmd_stride2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_stride_v_2_tnr1_y        : 4;   //  0: 3
	UINT32 cmd_rd_stride_v_8_tnr1_y        : 4;   //  4: 7
	UINT32 cmd_rd_stride_v_ipc_m           : 4;   //  8:11
	UINT32 cmd_rd_stride_v_2_ipc1_c        : 4;   // 12:15
	UINT32 cmd_rd_stride_v_8_ipc1_c        : 4;   // 16:19
} H14A0_MIFA_RD_CMD_STRIDE2_T;

/*-----------------------------------------------------------------------------
	0x0e4c mifa_rd_cmd_stride_set ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 man_cmd_rd_stride_v_mvi_y       : 1;   //     0
	UINT32 man_cmd_rd_stride_v_mvi_c       : 1;   //     1
	UINT32 man_cmd_rd_stride_v_prew_y      : 1;   //     2
	UINT32 man_cmd_rd_stride_v_prew_c      : 1;   //     3
	UINT32 man_cmd_rd_stride_v_ipc0_y      : 1;   //     4
	UINT32 man_cmd_rd_stride_v_ipc0_c      : 1;   //     5
	UINT32 man_cmd_rd_stride_v_ipc1_y      : 1;   //     6
	UINT32 man_cmd_rd_stride_v_tnr0_y      : 1;   //     7
	UINT32 man_cmd_rd_stride_v_tnr_c       : 1;   //     8
	UINT32 man_cmd_rd_stride_v_tnr1_y      : 1;   //     9
	UINT32 man_cmd_rd_stride_v_ipc_m       : 1;   //    10
	UINT32 man_cmd_rd_stride_v_ipc1_c      : 1;   //    11
} H14A0_MIFA_RD_CMD_STRIDE_SET_T;

/*-----------------------------------------------------------------------------
	0x0e50 mifa_wd_cmd_stride0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_wd_stride_v_2_tnr_y         : 4;   //  0: 3
	UINT32 cmd_wd_stride_v_8_tnr_y         : 4;   //  4: 7
	UINT32 cmd_wd_stride_v_2_tnr_c         : 4;   //  8:11
	UINT32 cmd_wd_stride_v_8_tnr_c         : 4;   // 12:15
	UINT32 cmd_wd_stride_v_2_prew_y        : 4;   // 16:19
	UINT32 cmd_wd_stride_v_8_prew_y        : 4;   // 20:23
	UINT32 cmd_wd_stride_v_2_prew_c        : 4;   // 24:27
	UINT32 cmd_wd_stride_v_8_prew_c        : 4;   // 28:31
} H14A0_MIFA_WD_CMD_STRIDE0_T;

/*-----------------------------------------------------------------------------
	0x0e54 mifa_wd_cmd_stride1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_wd_stride_v_tnr_m           : 4;   //  0: 3
} H14A0_MIFA_WD_CMD_STRIDE1_T;

/*-----------------------------------------------------------------------------
	0x0e58 mifa_wd_cmd_stride_set ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 man_cmd_wd_stride_v_tnr_y       : 1;   //     0
	UINT32 man_cmd_wd_stride_v_tnr_c       : 1;   //     1
	UINT32 man_cmd_wd_stride_v_prew_y      : 1;   //     2
	UINT32 man_cmd_wd_stride_v_prew_c      : 1;   //     3
	UINT32 man_cmd_wd_stride_v_tnr_m       : 1;   //     4
} H14A0_MIFA_WD_CMD_STRIDE_SET_T;

/*-----------------------------------------------------------------------------
	0x0e5c mifa_saddr_rd_mvi_y_mvi_c ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                 : 3;   //  0: 2 reserved
	UINT32 req_rd_saddr_mvi_y              : 13;  //  3:15
	UINT32                                 : 3;   // 16:18 reserved
	UINT32 req_rd_saddr_mvi_c              : 13;  // 19:31
} H14A0_MIFA_SADDR_RD_MVI_Y_MVI_C_T;

/*-----------------------------------------------------------------------------
	0x0e60 mifa_saddr_rd_prew_y ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                 : 3;   //  0: 2 reserved
	UINT32 req_rd_saddr_2_prew_y           : 13;  //  3:15
	UINT32                                 : 3;   // 16:18 reserved
	UINT32 req_rd_saddr_8_prew_y           : 13;  // 19:31
} H14A0_MIFA_SADDR_RD_PREW_Y_T;

/*-----------------------------------------------------------------------------
	0x0e64 mifa_saddr_rd_prew_c ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                 : 3;   //  0: 2 reserved
	UINT32 req_rd_saddr_2_prew_c           : 13;  //  3:15
	UINT32                                 : 3;   // 16:18 reserved
	UINT32 req_rd_saddr_8_prew_c           : 13;  // 19:31
} H14A0_MIFA_SADDR_RD_PREW_C_T;

/*-----------------------------------------------------------------------------
	0x0e68 mifa_saddr_rd_ipc0_y ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                 : 3;   //  0: 2 reserved
	UINT32 req_rd_saddr_2_ipc0_y           : 13;  //  3:15
	UINT32                                 : 3;   // 16:18 reserved
	UINT32 req_rd_saddr_8_ipc0_y           : 13;  // 19:31
} H14A0_MIFA_SADDR_RD_IPC0_Y_T;

/*-----------------------------------------------------------------------------
	0x0e6c mifa_saddr_rd_ipc0_c ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                 : 3;   //  0: 2 reserved
	UINT32 req_rd_saddr_2_ipc0_c           : 13;  //  3:15
	UINT32                                 : 3;   // 16:18 reserved
	UINT32 req_rd_saddr_8_ipc0_c           : 13;  // 19:31
} H14A0_MIFA_SADDR_RD_IPC0_C_T;

/*-----------------------------------------------------------------------------
	0x0e70 mifa_saddr_rd_ipc1_y ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                 : 3;   //  0: 2 reserved
	UINT32 req_rd_saddr_2_ipc1_y           : 13;  //  3:15
	UINT32                                 : 3;   // 16:18 reserved
	UINT32 req_rd_saddr_8_ipc1_y           : 13;  // 19:31
} H14A0_MIFA_SADDR_RD_IPC1_Y_T;

/*-----------------------------------------------------------------------------
	0x0e74 mifa_saddr_rd_tnr0_y ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                 : 3;   //  0: 2 reserved
	UINT32 req_rd_saddr_2_tnr0_y           : 13;  //  3:15
	UINT32                                 : 3;   // 16:18 reserved
	UINT32 req_rd_saddr_8_tnr0_y           : 13;  // 19:31
} H14A0_MIFA_SADDR_RD_TNR0_Y_T;

/*-----------------------------------------------------------------------------
	0x0e78 mifa_saddr_rd_tnr_c ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                 : 3;   //  0: 2 reserved
	UINT32 req_rd_saddr_2_tnr_c            : 13;  //  3:15
	UINT32                                 : 3;   // 16:18 reserved
	UINT32 req_rd_saddr_8_tnr_c            : 13;  // 19:31
} H14A0_MIFA_SADDR_RD_TNR_C_T;

/*-----------------------------------------------------------------------------
	0x0e7c mifa_saddr_rd_tnr1_y ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                 : 3;   //  0: 2 reserved
	UINT32 req_rd_saddr_2_tnr1_y           : 13;  //  3:15
	UINT32                                 : 3;   // 16:18 reserved
	UINT32 req_rd_saddr_8_tnr1_y           : 13;  // 19:31
} H14A0_MIFA_SADDR_RD_TNR1_Y_T;

/*-----------------------------------------------------------------------------
	0x0e80 mifa_saddr_rd_ipc_m ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                 : 3;   //  0: 2 reserved
	UINT32 req_rd_saddr_ipc_m              : 13;  //  3:15
} H14A0_MIFA_SADDR_RD_IPC_M_T;

/*-----------------------------------------------------------------------------
	0x0e84 mifa_saddr_wd_tnr_y ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                 : 3;   //  0: 2 reserved
	UINT32 req_wd_saddr_2_tnr_y            : 13;  //  3:15
	UINT32                                 : 3;   // 16:18 reserved
	UINT32 req_wd_saddr_8_tnr_y            : 13;  // 19:31
} H14A0_MIFA_SADDR_WD_TNR_Y_T;

/*-----------------------------------------------------------------------------
	0x0e88 mifa_saddr_wd_tnr_c ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                 : 3;   //  0: 2 reserved
	UINT32 req_wd_saddr_2_tnr_c            : 13;  //  3:15
	UINT32                                 : 3;   // 16:18 reserved
	UINT32 req_wd_saddr_8_tnr_c            : 13;  // 19:31
} H14A0_MIFA_SADDR_WD_TNR_C_T;

/*-----------------------------------------------------------------------------
	0x0e8c mifa_saddr_wd_prew_y ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                 : 3;   //  0: 2 reserved
	UINT32 req_wd_saddr_2_prew_y           : 13;  //  3:15
	UINT32                                 : 3;   // 16:18 reserved
	UINT32 req_wd_saddr_8_prew_y           : 13;  // 19:31
} H14A0_MIFA_SADDR_WD_PREW_Y_T;

/*-----------------------------------------------------------------------------
	0x0e90 mifa_saddr_wd_prew_c ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                 : 3;   //  0: 2 reserved
	UINT32 req_wd_saddr_2_prew_c           : 13;  //  3:15
	UINT32                                 : 3;   // 16:18 reserved
	UINT32 req_wd_saddr_8_prew_c           : 13;  // 19:31
} H14A0_MIFA_SADDR_WD_PREW_C_T;

/*-----------------------------------------------------------------------------
	0x0e94 mifa_saddr_wd_tnr_m ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                 : 3;   //  0: 2 reserved
	UINT32 req_wd_saddr_tnr_m              : 13;  //  3:15
} H14A0_MIFA_SADDR_WD_TNR_M_T;

/*-----------------------------------------------------------------------------
	0x0e98 mifa_saddr_rd_dl_y ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                 : 3;   //  0: 2 reserved
	UINT32 req_rd_saddr_2_dl_y             : 13;  //  3:15
	UINT32                                 : 3;   // 16:18 reserved
	UINT32 req_rd_saddr_8_dl_y             : 13;  // 19:31
} H14A0_MIFA_SADDR_RD_DL_Y_T;

/*-----------------------------------------------------------------------------
	0x0e9c mifa_saddr_rd_dl_c ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                 : 3;   //  0: 2 reserved
	UINT32 req_rd_saddr_2_dl_c             : 13;  //  3:15
	UINT32                                 : 3;   // 16:18 reserved
	UINT32 req_rd_saddr_8_dl_c             : 13;  // 19:31
} H14A0_MIFA_SADDR_RD_DL_C_T;

/*-----------------------------------------------------------------------------
	0x0ea0 mifa_saddr_rd_dr_y ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                 : 3;   //  0: 2 reserved
	UINT32 req_rd_saddr_2_dr_y             : 13;  //  3:15
	UINT32                                 : 3;   // 16:18 reserved
	UINT32 req_rd_saddr_8_dr_y             : 13;  // 19:31
} H14A0_MIFA_SADDR_RD_DR_Y_T;

/*-----------------------------------------------------------------------------
	0x0ea4 mifa_saddr_rd_dr_c ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                 : 3;   //  0: 2 reserved
	UINT32 req_rd_saddr_2_dr_c             : 13;  //  3:15
	UINT32                                 : 3;   // 16:18 reserved
	UINT32 req_rd_saddr_8_dr_c             : 13;  // 19:31
} H14A0_MIFA_SADDR_RD_DR_C_T;

/*-----------------------------------------------------------------------------
	0x0ea8 mifa_saddr_wd_dl_y ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                 : 3;   //  0: 2 reserved
	UINT32 req_wd_saddr_2_dl_y             : 13;  //  3:15
	UINT32                                 : 3;   // 16:18 reserved
	UINT32 req_wd_saddr_8_dl_y             : 13;  // 19:31
} H14A0_MIFA_SADDR_WD_DL_Y_T;

/*-----------------------------------------------------------------------------
	0x0eac mifa_saddr_wd_dl_c ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                 : 3;   //  0: 2 reserved
	UINT32 req_wd_saddr_2_dl_c             : 13;  //  3:15
	UINT32                                 : 3;   // 16:18 reserved
	UINT32 req_wd_saddr_8_dl_c             : 13;  // 19:31
} H14A0_MIFA_SADDR_WD_DL_C_T;

/*-----------------------------------------------------------------------------
	0x0eb0 mifa_saddr_wd_dr_y ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                 : 3;   //  0: 2 reserved
	UINT32 req_wd_saddr_2_dr_y             : 13;  //  3:15
	UINT32                                 : 3;   // 16:18 reserved
	UINT32 req_wd_saddr_8_dr_y             : 13;  // 19:31
} H14A0_MIFA_SADDR_WD_DR_Y_T;

/*-----------------------------------------------------------------------------
	0x0eb4 mifa_saddr_wd_dr_c ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                 : 3;   //  0: 2 reserved
	UINT32 req_wd_saddr_2_dr_c             : 13;  //  3:15
	UINT32                                 : 3;   // 16:18 reserved
	UINT32 req_wd_saddr_8_dr_c             : 13;  // 19:31
} H14A0_MIFA_SADDR_WD_DR_C_T;

/*-----------------------------------------------------------------------------
	0x0eb8 mifa_saddr_rd_ext0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_saddr_mvi_y_msb4b        : 4;   //  0: 3
	UINT32 req_rd_saddr_mvi_c_msb4b        : 4;   //  4: 7
	UINT32 req_rd_saddr_2_prew_y_msb4b     : 4;   //  8:11
	UINT32 req_rd_saddr_8_prew_y_msb4b     : 4;   // 12:15
	UINT32 req_rd_saddr_2_prew_c_msb4b     : 4;   // 16:19
	UINT32 req_rd_saddr_8_prew_c_msb4b     : 4;   // 20:23
	UINT32 req_rd_saddr_2_ipc0_y_msb4b     : 4;   // 24:27
	UINT32 req_rd_saddr_8_ipc0_y_msb4b     : 4;   // 28:31
} H14A0_MIFA_SADDR_RD_EXT0_T;

/*-----------------------------------------------------------------------------
	0x0ebc mifa_saddr_rd_ext1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_saddr_2_ipc0_c_msb4b     : 4;   //  0: 3
	UINT32 req_rd_saddr_8_ipc0_c_msb4b     : 4;   //  4: 7
	UINT32 req_rd_saddr_2_ipc1_y_msb4b     : 4;   //  8:11
	UINT32 req_rd_saddr_8_ipc1_y_msb4b     : 4;   // 12:15
	UINT32 req_rd_saddr_2_tnr0_y_msb4b     : 4;   // 16:19
	UINT32 req_rd_saddr_8_tnr0_y_msb4b     : 4;   // 20:23
	UINT32 req_rd_saddr_2_tnr_c_msb4b      : 4;   // 24:27
	UINT32 req_rd_saddr_8_tnr_c_msb4b      : 4;   // 28:31
} H14A0_MIFA_SADDR_RD_EXT1_T;

/*-----------------------------------------------------------------------------
	0x0ec0 mifa_saddr_rd_ext2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_saddr_2_tnr1_y_msb4b     : 4;   //  0: 3
	UINT32 req_rd_saddr_8_tnr1_y_msb4b     : 4;   //  4: 7
	UINT32 req_rd_saddr_ipc_m_msb4b        : 4;   //  8:11
	UINT32 req_rd_saddr_2_ipc1_c_msb4b     : 4;   // 12:15
	UINT32 req_rd_saddr_8_ipc1_c_msb4b     : 4;   // 16:19
} H14A0_MIFA_SADDR_RD_EXT2_T;

/*-----------------------------------------------------------------------------
	0x0ec4 mifa_saddr_wd_ext0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_wd_saddr_2_tnr_y_msb4b      : 4;   //  0: 3
	UINT32 req_wd_saddr_8_tnr_y_msb4b      : 4;   //  4: 7
	UINT32 req_wd_saddr_2_tnr_c_msb4b      : 4;   //  8:11
	UINT32 req_wd_saddr_8_tnr_c_msb4b      : 4;   // 12:15
	UINT32 req_wd_saddr_2_prew_y_msb4b     : 4;   // 16:19
	UINT32 req_wd_saddr_8_prew_y_msb4b     : 4;   // 20:23
	UINT32 req_wd_saddr_2_prew_c_msb4b     : 4;   // 24:27
	UINT32 req_wd_saddr_8_prew_c_msb4b     : 4;   // 28:31
} H14A0_MIFA_SADDR_WD_EXT0_T;

/*-----------------------------------------------------------------------------
	0x0ec8 mifa_saddr_wd_ext1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_wd_saddr_tnr_m_msb4b        : 4;   //  0: 3
} H14A0_MIFA_SADDR_WD_EXT1_T;

/*-----------------------------------------------------------------------------
	0x0ecc mifa_rd_dr_dl_ext ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_saddr_2_dl_y_msb4b       : 4;   //  0: 3
	UINT32 req_rd_saddr_8_dl_y_msb4b       : 4;   //  4: 7
	UINT32 req_rd_saddr_2_dl_c_msb4b       : 4;   //  8:11
	UINT32 req_rd_saddr_8_dl_c_msb4b       : 4;   // 12:15
	UINT32 req_rd_saddr_2_dr_y_msb4b       : 4;   // 16:19
	UINT32 req_rd_saddr_8_dr_y_msb4b       : 4;   // 20:23
	UINT32 req_rd_saddr_2_dr_c_msb4b       : 4;   // 24:27
	UINT32 req_rd_saddr_8_dr_c_msb4b       : 4;   // 28:31
} H14A0_MIFA_RD_DR_DL_EXT_T;

/*-----------------------------------------------------------------------------
	0x0ed0 mifa_wd_dr_dl_ext ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_wd_saddr_2_dl_y_msb4b       : 4;   //  0: 3
	UINT32 req_wd_saddr_8_dl_y_msb4b       : 4;   //  4: 7
	UINT32 req_wd_saddr_2_dl_c_msb4b       : 4;   //  8:11
	UINT32 req_wd_saddr_8_dl_c_msb4b       : 4;   // 12:15
	UINT32 req_wd_saddr_2_dr_y_msb4b       : 4;   // 16:19
	UINT32 req_wd_saddr_8_dr_y_msb4b       : 4;   // 20:23
	UINT32 req_wd_saddr_2_dr_c_msb4b       : 4;   // 24:27
	UINT32 req_wd_saddr_8_dr_c_msb4b       : 4;   // 28:31
} H14A0_MIFA_WD_DR_DL_EXT_T;

/*-----------------------------------------------------------------------------
	0x0ed4 mifa_status ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 fr_rd_valid_mvi_y               : 1;   //     0
	UINT32 fr_rd_valid_mvi_c               : 1;   //     1
	UINT32 fr_rd_valid_prew_y              : 1;   //     2
	UINT32 fr_rd_valid_prew_c              : 1;   //     3
	UINT32 fr_rd_valid_ipc0_y              : 1;   //     4
	UINT32 fr_rd_valid_ipc0_c              : 1;   //     5
	UINT32 fr_rd_valid_ipc1_y              : 1;   //     6
	UINT32 fr_rd_valid_tnr0_y              : 1;   //     7
	UINT32 fr_rd_valid_tnr_c               : 1;   //     8
	UINT32 fr_rd_valid_tnr1_y              : 1;   //     9
	UINT32 fr_rd_valid_ipc_m               : 1;   //    10
	UINT32 fr_wd_q_full_tnr_y              : 1;   //    11
	UINT32 fr_wd_q_full_tnr_c              : 1;   //    12
	UINT32 fr_wd_q_full_prew_y             : 1;   //    13
	UINT32 fr_wd_q_full_prew_c             : 1;   //    14
	UINT32 fr_wd_q_full_tnr_m              : 1;   //    15
	UINT32 fr_rd_valid_ipc1_c              : 1;   //    16
} H14A0_MIFA_STATUS_T;

/*-----------------------------------------------------------------------------
	0x0ed8 mifa_saddr_param_update0_m ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 load                            : 1;   //     0
	UINT32                                 : 10;  //  1:10 reserved
	UINT32 register_update_address0_m      : 21;  // 11:31
} H14A0_MIFA_SADDR_PARAM_UPDATE0_M_T;

/*-----------------------------------------------------------------------------
	0x0edc mifa_saddr_param_update0_m1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 load                            : 1;   //     0
	UINT32                                 : 10;  //  1:10 reserved
	UINT32 register_update_address0_m1     : 21;  // 11:31
} H14A0_MIFA_SADDR_PARAM_UPDATE0_M1_T;

/*-----------------------------------------------------------------------------
	0x0ee0 mifa_rd_bank_8_addr0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_bank_split_8_mvi_y       : 1;   //     0
	UINT32 req_rd_bank_addr_8_mvi_y        : 3;   //  1: 3
	UINT32 req_rd_bank_split_8_mvi_c       : 1;   //     4
	UINT32 req_rd_bank_addr_8_mvi_c        : 3;   //  5: 7
	UINT32 req_rd_bank_split_8_prew_y      : 1;   //     8
	UINT32 req_rd_bank_addr_8_prew_y       : 3;   //  9:11
	UINT32 req_rd_bank_split_8_prew_c      : 1;   //    12
	UINT32 req_rd_bank_addr_8_prew_c       : 3;   // 13:15
	UINT32 req_rd_bank_split_8_ipc0_y      : 1;   //    16
	UINT32 req_rd_bank_addr_8_ipc0_y       : 3;   // 17:19
	UINT32 req_rd_bank_split_8_ipc0_c      : 1;   //    20
	UINT32 req_rd_bank_addr_8_ipc0_c       : 3;   // 21:23
	UINT32 req_rd_bank_split_8_ipc1_y      : 1;   //    24
	UINT32 req_rd_bank_addr_8_ipc1_y       : 3;   // 25:27
	UINT32 req_rd_bank_split_8_tnr0_y      : 1;   //    28
	UINT32 req_rd_bank_addr_8_tnr0_y       : 3;   // 29:31
} H14A0_MIFA_RD_BANK_8_ADDR0_T;

/*-----------------------------------------------------------------------------
	0x0ee4 mifa_rd_bank_8_addr1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_bank_split_8_tnr_c       : 1;   //     0
	UINT32 req_rd_bank_addr_8_tnr_c        : 3;   //  1: 3
	UINT32 req_rd_bank_split_8_tnr1_y      : 1;   //     4
	UINT32 req_rd_bank_addr_8_tnr1_y       : 3;   //  5: 7
	UINT32 req_rd_bank_split_ipc_m         : 1;   //     8
	UINT32 req_rd_bank_addr_ipc_m          : 3;   //  9:11
	UINT32 req_rd_bank_split_8_ipc1_c      : 1;   //    12
	UINT32 req_rd_bank_addr_8_ipc1_c       : 3;   // 13:15
} H14A0_MIFA_RD_BANK_8_ADDR1_T;

/*-----------------------------------------------------------------------------
	0x0ee8 mifa_rd_bank_2_addr0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                 : 8;   //  0: 7 reserved
	UINT32 req_rd_bank_split_2_prew_y      : 1;   //     8
	UINT32 req_rd_bank_addr_2_prew_y       : 3;   //  9:11
	UINT32 req_rd_bank_split_2_prew_c      : 1;   //    12
	UINT32 req_rd_bank_addr_2_prew_c       : 3;   // 13:15
	UINT32 req_rd_bank_split_2_ipc0_y      : 1;   //    16
	UINT32 req_rd_bank_addr_2_ipc0_y       : 3;   // 17:19
	UINT32 req_rd_bank_split_2_ipc0_c      : 1;   //    20
	UINT32 req_rd_bank_addr_2_ipc0_c       : 3;   // 21:23
	UINT32 req_rd_bank_split_2_ipc1_y      : 1;   //    24
	UINT32 req_rd_bank_addr_2_ipc1_y       : 3;   // 25:27
	UINT32 req_rd_bank_split_2_tnr0_y      : 1;   //    28
	UINT32 req_rd_bank_addr_2_tnr0_y       : 3;   // 29:31
} H14A0_MIFA_RD_BANK_2_ADDR0_T;

/*-----------------------------------------------------------------------------
	0x0eec mifa_rd_bank_2_addr1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_bank_split_2_tnr_c       : 1;   //     0
	UINT32 req_rd_bank_addr_2_tnr_c        : 3;   //  1: 3
	UINT32 req_rd_bank_split_2_tnr1_y      : 1;   //     4
	UINT32 req_rd_bank_addr_2_tnr1_y       : 3;   //  5: 7
	UINT32                                 : 4;   //  8:11 reserved
	UINT32 req_rd_bank_split_2_ipc1_c      : 1;   //    12
	UINT32 req_rd_bank_addr_2_ipc1_c       : 3;   // 13:15
} H14A0_MIFA_RD_BANK_2_ADDR1_T;

/*-----------------------------------------------------------------------------
	0x0ef0 mifa_wd_bank_8_addr0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_wd_bank_split_8_tnr_y       : 1;   //     0
	UINT32 req_wd_bank_addr_8_tnr_y        : 3;   //  1: 3
	UINT32 req_wd_bank_split_8_tnr_c       : 1;   //     4
	UINT32 req_wd_bank_addr_8_tnr_c        : 3;   //  5: 7
	UINT32 req_wd_bank_split_8_prew_y      : 1;   //     8
	UINT32 req_wd_bank_addr_8_prew_y       : 3;   //  9:11
	UINT32 req_wd_bank_split_8_prew_c      : 1;   //    12
	UINT32 req_wd_bank_addr_8_prew_c       : 3;   // 13:15
	UINT32 req_wd_bank_split_tnr_m         : 1;   //    16
	UINT32 req_wd_bank_addr_tnr_m          : 3;   // 17:19
} H14A0_MIFA_WD_BANK_8_ADDR0_T;

/*-----------------------------------------------------------------------------
	0x0ef4 mifa_wd_bank_8_addr1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 mifa_wd_bank_8_addr1_f          : 32;   // 31: 0
} H14A0_MIFA_WD_BANK_8_ADDR1_T;

/*-----------------------------------------------------------------------------
	0x0ef8 mifa_wd_bank_2_addr0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_wd_bank_split_2_tnr_y       : 1;   //     0
	UINT32 req_wd_bank_addr_2_tnr_y        : 3;   //  1: 3
	UINT32 req_wd_bank_split_2_tnr_c       : 1;   //     4
	UINT32 req_wd_bank_addr_2_tnr_c        : 3;   //  5: 7
	UINT32 req_wd_bank_split_2_prew_y      : 1;   //     8
	UINT32 req_wd_bank_addr_2_prew_y       : 3;   //  9:11
	UINT32 req_wd_bank_split_2_prew_c      : 1;   //    12
	UINT32 req_wd_bank_addr_2_prew_c       : 3;   // 13:15
} H14A0_MIFA_WD_BANK_2_ADDR0_T;

/*-----------------------------------------------------------------------------
	0x0efc mifa_wd_bank_2_addr1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 mifa_wd_bank_2_addr1_f          : 32;   // 31: 0
} H14A0_MIFA_WD_BANK_2_ADDR1_T;

/*-----------------------------------------------------------------------------
	0x0f00 mifa_lsb_switch ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_lsb_prew_y               : 2;   //  0: 1
	UINT32 req_rd_lsb_prew_c               : 2;   //  2: 3
	UINT32 req_rd_lsb_ipc0_y               : 2;   //  4: 5
	UINT32 req_rd_lsb_ipc0_c               : 2;   //  6: 7
	UINT32 req_rd_lsb_ipc1_y               : 2;   //  8: 9
	UINT32 req_rd_lsb_tnr0_y               : 2;   // 10:11
	UINT32 req_rd_lsb_tnr_c                : 2;   // 12:13
	UINT32 req_rd_lsb_tnr1_y               : 2;   // 14:15
	UINT32 req_rd_lsb_ipc1_c               : 2;   // 16:17
} H14A0_MIFA_LSB_SWITCH_T;

/*-----------------------------------------------------------------------------
	0x0f04 mifa_rd_req_pend0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_pend_v_mvi_y             : 8;   //  0: 7
	UINT32 req_rd_pend_v_mvi_c             : 8;   //  8:15
	UINT32 req_rd_pend_v_prew_y            : 8;   // 16:23
	UINT32 req_rd_pend_v_prew_c            : 8;   // 24:31
} H14A0_MIFA_RD_REQ_PEND0_T;

/*-----------------------------------------------------------------------------
	0x0f08 mifa_rd_req_pend1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_pend_v_ipc0_y            : 8;   //  0: 7
	UINT32 req_rd_pend_v_ipc0_c            : 8;   //  8:15
	UINT32 req_rd_pend_v_ipc1_y            : 8;   // 16:23
	UINT32 req_rd_pend_v_tnr0_y            : 8;   // 24:31
} H14A0_MIFA_RD_REQ_PEND1_T;

/*-----------------------------------------------------------------------------
	0x0f0c mifa_rd_req_pend2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_pend_v_tnr_c             : 8;   //  0: 7
	UINT32 req_rd_pend_v_tnr1_y            : 8;   //  8:15
	UINT32 req_rd_pend_v_ipc_m             : 8;   // 16:23
	UINT32 req_rd_pend_v_ipc1_c            : 8;   // 24:31
} H14A0_MIFA_RD_REQ_PEND2_T;

/*-----------------------------------------------------------------------------
	0x0f10 mifa_wd_req_pend0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_wd_pend_v_tnr_y             : 8;   //  0: 7
	UINT32 req_wd_pend_v_tnr_c             : 8;   //  8:15
	UINT32 req_wd_pend_v_prew_y            : 8;   // 16:23
	UINT32 req_wd_pend_v_prew_c            : 8;   // 24:31
} H14A0_MIFA_WD_REQ_PEND0_T;

/*-----------------------------------------------------------------------------
	0x0f14 mifa_wd_req_pend1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_wd_pend_v_tnr_m             : 8;   //  0: 7
} H14A0_MIFA_WD_REQ_PEND1_T;

/*-----------------------------------------------------------------------------
	0x0f18 mifa_rd_cmd_pri0_s ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_pri_s_v_mvi_y            : 4;   //  0: 3
	UINT32 cmd_rd_pri_s_v_mvi_c            : 4;   //  4: 7
	UINT32 cmd_rd_pri_s_v_prew_y           : 4;   //  8:11
	UINT32 cmd_rd_pri_s_v_prew_c           : 4;   // 12:15
	UINT32 cmd_rd_pri_s_v_ipc0_y           : 4;   // 16:19
	UINT32 cmd_rd_pri_s_v_ipc0_c           : 4;   // 20:23
	UINT32 cmd_rd_pri_s_v_ipc1_y           : 4;   // 24:27
	UINT32 cmd_rd_pri_s_v_tnr0_y           : 4;   // 28:31
} H14A0_MIFA_RD_CMD_PRI0_S_T;

/*-----------------------------------------------------------------------------
	0x0f1c mifa_rd_cmd_pri1_s ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_pri_s_v_tnr_c            : 4;   //  0: 3
	UINT32 cmd_rd_pri_s_v_tnr1_y           : 4;   //  4: 7
	UINT32 cmd_rd_pri_s_v_ipc_m            : 4;   //  8:11
	UINT32 cmd_rd_pri_s_v_ipc1_c           : 4;   // 12:15
} H14A0_MIFA_RD_CMD_PRI1_S_T;

/*-----------------------------------------------------------------------------
	0x0f20 mifa_wd_cmd_pri0_s ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_wd_pri_s_v_tnr_y            : 4;   //  0: 3
	UINT32 cmd_wd_pri_s_v_tnr_c            : 4;   //  4: 7
	UINT32 cmd_wd_pri_s_v_prew_y           : 4;   //  8:11
	UINT32 cmd_wd_pri_s_v_prew_c           : 4;   // 12:15
	UINT32 cmd_wd_pri_s_v_tnr_m            : 4;   // 16:19
} H14A0_MIFA_WD_CMD_PRI0_S_T;

/*-----------------------------------------------------------------------------
	0x0f24 mifa_reset ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 reset_mif_r_mvi_y               : 1;   //     0
	UINT32 reset_mif_r_mvi_c               : 1;   //     1
	UINT32 reset_mif_r_prew_y              : 1;   //     2
	UINT32 reset_mif_r_prew_c              : 1;   //     3
	UINT32 reset_mif_r_ipc0_y              : 1;   //     4
	UINT32 reset_mif_r_ipc0_c              : 1;   //     5
	UINT32 reset_mif_r_ipc1_y              : 1;   //     6
	UINT32 reset_mif_r_tnr0_y              : 1;   //     7
	UINT32 reset_mif_r_tnr_c               : 1;   //     8
	UINT32 reset_mif_r_tnr1_y              : 1;   //     9
	UINT32 reset_mif_r_ipc_m               : 1;   //    10
	UINT32 reset_mif_w_tnr_y               : 1;   //    11
	UINT32 reset_mif_w_tnr_c               : 1;   //    12
	UINT32 reset_mif_w_prew_y              : 1;   //    13
	UINT32 reset_mif_w_prew_c              : 1;   //    14
	UINT32 reset_mif_w_tnr_m               : 1;   //    15
	UINT32 reset_mif_r_ipc1_c              : 1;   //    16
	UINT32                                 : 3;   // 17:19 reserved
	UINT32 reset_time_out_cnt              : 12;  // 20:31
} H14A0_MIFA_RESET_T;

/*-----------------------------------------------------------------------------
	0x0f28 mifa_saddr_rd_ipc1_c ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                 : 3;   //  0: 2 reserved
	UINT32 req_rd_saddr_2_ipc1_c           : 13;  //  3:15
	UINT32                                 : 3;   // 16:18 reserved
	UINT32 req_rd_saddr_8_ipc1_c           : 13;  // 19:31
} H14A0_MIFA_SADDR_RD_IPC1_C_T;

/*-----------------------------------------------------------------------------
	0x0f2c mifa_mvi_r_stride ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 mvi_ra_manual_x16_stride_size   : 13;  //  0:12
	UINT32                                 : 3;   // 13:15 reserved
	UINT32 mvi_ra_manual_x16_stride_enable : 1;   //    16
	UINT32                                 : 3;   // 17:19 reserved
	UINT32 mvi_ra_chroma_stride_size_set   : 1;   //    20
} H14A0_MIFA_MVI_R_STRIDE_T;

/*-----------------------------------------------------------------------------
	0x1e00 mifb_rd_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                 : 12;  //  0:11 reserved
	UINT32 disable_rd_mif                  : 1;   //    12
	UINT32 load_rd_req_stv_v               : 1;   //    13
	UINT32 load_rd_req_pri_v               : 1;   //    14
	UINT32 load_rd_cmd_saddr_v             : 1;   //    15
	UINT32 load_rd_cmd_pend_v              : 1;   //    16
	UINT32 load_rd_cmd_pri_v               : 1;   //    17
	UINT32 load_rd_cmd_stride_v            : 1;   //    18
	UINT32 enable_rd_stv_cnt               : 1;   //    19
	UINT32                                 : 1;   //    20 reserved
	UINT32 mrb_bank_mode_b23_21            : 3;   // 21:23
	UINT32 mrb_bank_mode_b26_24            : 3;   // 24:26
} H14A0_MIFB_RD_CTRL_T;

/*-----------------------------------------------------------------------------
	0x1e04 mifb_wd_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                 : 12;  //  0:11 reserved
	UINT32 disable_wd_mif                  : 1;   //    12
	UINT32 load_wd_req_stv_v               : 1;   //    13
	UINT32 load_wd_req_pri_v               : 1;   //    14
	UINT32 load_wd_cmd_saddr_v             : 1;   //    15
	UINT32 load_wd_cmd_pend_v              : 1;   //    16
	UINT32 load_wd_cmd_pri_v               : 1;   //    17
	UINT32 load_wd_cmd_stride_v            : 1;   //    18
	UINT32 enable_wd_stv_cnt               : 1;   //    19
} H14A0_MIFB_WD_CTRL_T;

/*-----------------------------------------------------------------------------
	0x1e08 mifb_rd_set_10b ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 set_10b_req_rd_ov_y             : 1;   //     0
	UINT32 set_10b_req_rd_ov_c             : 1;   //     1
} H14A0_MIFB_RD_SET_10B_T;

/*-----------------------------------------------------------------------------
	0x1e0c mifb_wd_set_10b ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 set_10b_req_wd_sm_y             : 1;   //     0
	UINT32 set_10b_req_wd_sm_c             : 1;   //     1
} H14A0_MIFB_WD_SET_10B_T;

/*-----------------------------------------------------------------------------
	0x1e10 mifb_rd_cmd_pri0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_pri_v_ov_y               : 4;   //  0: 3
	UINT32 cmd_rd_pri_v_ov_c               : 4;   //  4: 7
	UINT32 cmd_rd_pri_v_ovs_y              : 4;   //  8:11
	UINT32 cmd_rd_pri_v_ovs_c              : 4;   // 12:15
} H14A0_MIFB_RD_CMD_PRI0_T;

/*-----------------------------------------------------------------------------
	0x1e14 mifb_wd_cmd_pri0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_wd_pri_v_sm_y               : 4;   //  0: 3
	UINT32 cmd_wd_pri_v_sm_c               : 4;   //  4: 7
} H14A0_MIFB_WD_CMD_PRI0_T;

/*-----------------------------------------------------------------------------
	0x1e18 mifb_rd_req_pri0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_pri_v_ov_y               : 4;   //  0: 3
	UINT32 req_rd_pri_v_ov_c               : 4;   //  4: 7
	UINT32 req_rd_pri_v_ovs_y              : 4;   //  8:11
	UINT32 req_rd_pri_v_ovs_c              : 4;   // 12:15
} H14A0_MIFB_RD_REQ_PRI0_T;

/*-----------------------------------------------------------------------------
	0x1e1c mifb_wd_req_pri0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_wd_pri_v_sm_y               : 4;   //  0: 3
	UINT32 req_wd_pri_v_sm_c               : 4;   //  4: 7
} H14A0_MIFB_WD_REQ_PRI0_T;

/*-----------------------------------------------------------------------------
	0x1e20 mifb_rd_req_stv0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_stv_v_ov_y               : 4;   //  0: 3
	UINT32 req_rd_stv_v_ov_c               : 4;   //  4: 7
	UINT32 req_rd_stv_v_ovs_y              : 4;   //  8:11
	UINT32 req_rd_stv_v_ovs_c              : 4;   // 12:15
} H14A0_MIFB_RD_REQ_STV0_T;

/*-----------------------------------------------------------------------------
	0x1e24 mifb_wd_req_stv0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_wd_stv_v_sm_y               : 4;   //  0: 3
	UINT32 req_wd_stv_v_sm_c               : 4;   //  4: 7
} H14A0_MIFB_WD_REQ_STV0_T;

/*-----------------------------------------------------------------------------
	0x1e28 mifb_rd_cmd_pend0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_pend_v_ov_y              : 4;   //  0: 3
	UINT32 cmd_rd_pend_v_ov_c              : 4;   //  4: 7
	UINT32 cmd_rd_pend_v_ovs_y             : 4;   //  8:11
	UINT32 cmd_rd_pend_v_ovs_c             : 4;   // 12:15
} H14A0_MIFB_RD_CMD_PEND0_T;

/*-----------------------------------------------------------------------------
	0x1e2c mifb_wd_cmd_pend0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_wd_pend_v_sm_y              : 4;   //  0: 3
	UINT32 cmd_wd_pend_v_sm_c              : 4;   //  4: 7
} H14A0_MIFB_WD_CMD_PEND0_T;

/*-----------------------------------------------------------------------------
	0x1e30 mifb_rd_cmd_stride0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_stride_v_2_ov_y          : 4;   //  0: 3
	UINT32 cmd_rd_stride_v_8_ov_y          : 4;   //  4: 7
	UINT32 cmd_rd_stride_v_2_ov_c          : 4;   //  8:11
	UINT32 cmd_rd_stride_v_8_ov_c          : 4;   // 12:15
	UINT32 cmd_rd_stride_v_ovs_y           : 4;   // 16:19
	UINT32 cmd_rd_stride_v_ovs_c           : 4;   // 20:23
} H14A0_MIFB_RD_CMD_STRIDE0_T;

/*-----------------------------------------------------------------------------
	0x1e34 mifb_rd_cmd_stride_set ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 man_cmd_rd_stride_v_ov_y        : 1;   //     0
	UINT32 man_cmd_rd_stride_v_ov_c        : 1;   //     1
	UINT32 man_cmd_rd_stride_v_ovs_y       : 1;   //     2
	UINT32 man_cmd_rd_stride_v_ovs_c       : 1;   //     3
} H14A0_MIFB_RD_CMD_STRIDE_SET_T;

/*-----------------------------------------------------------------------------
	0x1e38 mifb_wd_cmd_stride0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_wd_stride_v_2_sm_y          : 4;   //  0: 3
	UINT32 cmd_wd_stride_v_8_sm_y          : 4;   //  4: 7
	UINT32 cmd_wd_stride_v_2_sm_c          : 4;   //  8:11
	UINT32 cmd_wd_stride_v_8_sm_c          : 4;   // 12:15
} H14A0_MIFB_WD_CMD_STRIDE0_T;

/*-----------------------------------------------------------------------------
	0x1e3c mifb_wd_cmd_stride_set ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 man_cmd_wd_stride_v_sm_y        : 1;   //     0
	UINT32 man_cmd_wd_stride_v_sm_c        : 1;   //     1
} H14A0_MIFB_WD_CMD_STRIDE_SET_T;

/*-----------------------------------------------------------------------------
	0x1e40 mifb_saddr_rd_ov_y ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                 : 3;   //  0: 2 reserved
	UINT32 req_rd_saddr_2_ov_y             : 13;  //  3:15
	UINT32                                 : 3;   // 16:18 reserved
	UINT32 req_rd_saddr_8_ov_y             : 13;  // 19:31
} H14A0_MIFB_SADDR_RD_OV_Y_T;

/*-----------------------------------------------------------------------------
	0x1e44 mifb_saddr_rd_ov_c ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                 : 3;   //  0: 2 reserved
	UINT32 req_rd_saddr_2_ov_c             : 13;  //  3:15
	UINT32                                 : 3;   // 16:18 reserved
	UINT32 req_rd_saddr_8_ov_c             : 13;  // 19:31
} H14A0_MIFB_SADDR_RD_OV_C_T;

/*-----------------------------------------------------------------------------
	0x1e48 mifb_saddr_wd_sm_y ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                 : 3;   //  0: 2 reserved
	UINT32 req_wd_saddr_2_sm_y             : 13;  //  3:15
	UINT32                                 : 3;   // 16:18 reserved
	UINT32 req_wd_saddr_8_sm_y             : 13;  // 19:31
} H14A0_MIFB_SADDR_WD_SM_Y_T;

/*-----------------------------------------------------------------------------
	0x1e4c mifb_saddr_wd_sm_c ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                 : 3;   //  0: 2 reserved
	UINT32 req_wd_saddr_2_sm_c             : 13;  //  3:15
	UINT32                                 : 3;   // 16:18 reserved
	UINT32 req_wd_saddr_8_sm_c             : 13;  // 19:31
} H14A0_MIFB_SADDR_WD_SM_C_T;

/*-----------------------------------------------------------------------------
	0x1e50 mifb_saddr_rd_ext0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_saddr_2_ov_y_msb4b       : 4;   //  0: 3
	UINT32 req_rd_saddr_8_ov_y_msb4b       : 4;   //  4: 7
	UINT32 req_rd_saddr_2_ov_c_msb4b       : 4;   //  8:11
	UINT32 req_rd_saddr_8_ov_c_msb4b       : 4;   // 12:15
	UINT32 req_rd_saddr_ovs_y_msb4b        : 4;   // 16:19
	UINT32 req_rd_saddr_ovs_c_msb4b        : 4;   // 20:23
} H14A0_MIFB_SADDR_RD_EXT0_T;

/*-----------------------------------------------------------------------------
	0x1e54 mifb_saddr_wd_ext0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_wd_saddr_2_sm_y_msb4b       : 4;   //  0: 3
	UINT32 req_wd_saddr_8_sm_y_msb4b       : 4;   //  4: 7
	UINT32 req_wd_saddr_2_sm_c_msb4b       : 4;   //  8:11
	UINT32 req_wd_saddr_8_sm_c_msb4b       : 4;   // 12:15
} H14A0_MIFB_SADDR_WD_EXT0_T;

/*-----------------------------------------------------------------------------
	0x1e58 mifb_status ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 fr_rd_valid_ov_y                : 1;   //     0
	UINT32 fr_rd_valid_ov_c                : 1;   //     1
	UINT32 fr_wd_q_full_sm_y               : 1;   //     2
	UINT32 fr_wd_q_full_sm_c               : 1;   //     3
} H14A0_MIFB_STATUS_T;

/*-----------------------------------------------------------------------------
	0x1e5c mifb_rd_bank_8_addr0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_bank_split_8_ov_y        : 1;   //     0
	UINT32 req_rd_bank_addr_8_ov_y         : 3;   //  1: 3
	UINT32 req_rd_bank_split_8_ov_c        : 1;   //     4
	UINT32 req_rd_bank_addr_8_ov_c         : 3;   //  5: 7
	UINT32 req_rd_bank_split_ovs_y         : 1;   //     8
	UINT32 req_rd_bank_addr_ovs_y          : 3;   //  9:11
	UINT32 req_rd_bank_split_ovs_c         : 1;   //    12
	UINT32 req_rd_bank_addr_ovs_c          : 3;   // 13:15
} H14A0_MIFB_RD_BANK_8_ADDR0_T;

/*-----------------------------------------------------------------------------
	0x1e60 mifb_rd_bank_8_addr1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 mifb_rd_bank_8_addr1_f          : 32;   // 31: 0
} H14A0_MIFB_RD_BANK_8_ADDR1_T;

/*-----------------------------------------------------------------------------
	0x1e64 mifb_rd_bank_2_addr0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_bank_split_2_ov_y        : 1;   //     0
	UINT32 req_rd_bank_addr_2_ov_y         : 3;   //  1: 3
	UINT32 req_rd_bank_split_2_ov_c        : 1;   //     4
	UINT32 req_rd_bank_addr_2_ov_c         : 3;   //  5: 7
} H14A0_MIFB_RD_BANK_2_ADDR0_T;

/*-----------------------------------------------------------------------------
	0x1e68 mifb_rd_bank_2_addr1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 mifb_rd_bank_2_addr1_f          : 32;   // 31: 0
} H14A0_MIFB_RD_BANK_2_ADDR1_T;

/*-----------------------------------------------------------------------------
	0x1e6c mifb_wd_bank_8_addr0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_wd_bank_split_8_sm_y        : 1;   //     0
	UINT32 req_wd_bank_addr_8_sm_y         : 3;   //  1: 3
	UINT32 req_wd_bank_split_8_sm_c        : 1;   //     4
	UINT32 req_wd_bank_addr_8_sm_c         : 3;   //  5: 7
} H14A0_MIFB_WD_BANK_8_ADDR0_T;

/*-----------------------------------------------------------------------------
	0x1e70 mifb_wd_bank_8_addr1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 mifb_wd_bank_8_addr1_f          : 32;   // 31: 0
} H14A0_MIFB_WD_BANK_8_ADDR1_T;

/*-----------------------------------------------------------------------------
	0x1e74 mifb_wd_bank_2_addr0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_wd_bank_split_2_sm_y        : 1;   //     0
	UINT32 req_wd_bank_addr_2_sm_y         : 3;   //  1: 3
	UINT32 req_wd_bank_split_2_sm_c        : 1;   //     4
	UINT32 req_wd_bank_addr_2_sm_c         : 3;   //  5: 7
} H14A0_MIFB_WD_BANK_2_ADDR0_T;

/*-----------------------------------------------------------------------------
	0x1e78 mifb_wd_bank_2_addr1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 mifb_wd_bank_2_addr1_f          : 32;   // 31: 0
} H14A0_MIFB_WD_BANK_2_ADDR1_T;

/*-----------------------------------------------------------------------------
	0x1e7c mifb_lsb_switch ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_lsb_ov_y                 : 2;   //  0: 1
	UINT32 req_rd_lsb_ov_c                 : 2;   //  2: 3
} H14A0_MIFB_LSB_SWITCH_T;

/*-----------------------------------------------------------------------------
	0x1e80 mifb_rd_req_pend0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_pend_v_ov_y              : 8;   //  0: 7
	UINT32 req_rd_pend_v_ov_c              : 8;   //  8:15
	UINT32 req_rd_pend_v_ovs_y             : 8;   // 16:23
	UINT32 req_rd_pend_v_ovs_c             : 8;   // 24:31
} H14A0_MIFB_RD_REQ_PEND0_T;

/*-----------------------------------------------------------------------------
	0x1e84 mifb_wd_req_pend0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_wd_pend_v_sm_y              : 8;   //  0: 7
	UINT32 req_wd_pend_v_sm_c              : 8;   //  8:15
} H14A0_MIFB_WD_REQ_PEND0_T;

/*-----------------------------------------------------------------------------
	0x1e88 mifb_rd_cmd_pri0_s ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_pri_s_v_ov_y             : 4;   //  0: 3
	UINT32 cmd_rd_pri_s_v_ov_c             : 4;   //  4: 7
	UINT32 cmd_rd_pri_s_v_ovs_y            : 4;   //  8:11
	UINT32 cmd_rd_pri_s_v_ovs_c            : 4;   // 12:15
} H14A0_MIFB_RD_CMD_PRI0_S_T;

/*-----------------------------------------------------------------------------
	0x1e8c mifb_wd_cmd_pri0_s ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_wd_pri_s_v_sm_y             : 4;   //  0: 3
	UINT32 cmd_wd_pri_s_v_sm_c             : 4;   //  4: 7
} H14A0_MIFB_WD_CMD_PRI0_S_T;

/*-----------------------------------------------------------------------------
	0x1e90 mifb_reset ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 reset_mif_r_ov_y                : 1;   //     0
	UINT32 reset_mif_r_ov_c                : 1;   //     1
	UINT32 reset_mif_w_sm_y                : 1;   //     2
	UINT32 reset_mif_w_sm_c                : 1;   //     3
	UINT32 reset_mif_r_ovs_y               : 1;   //     4
	UINT32 reset_mif_r_ovs_c               : 1;   //     5
	UINT32                                 : 14;  //  6:19 reserved
	UINT32 reset_time_out_cnt              : 12;  // 20:31
} H14A0_MIFB_RESET_T;

/*-----------------------------------------------------------------------------
	0x1e94 mifb_saddr_rd_ovs_y_ovs_c ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                 : 3;   //  0: 2 reserved
	UINT32 req_rd_saddr_ovs_y              : 13;  //  3:15
	UINT32                                 : 3;   // 16:18 reserved
	UINT32 req_rd_saddr_ovs_c              : 13;  // 19:31
} H14A0_MIFB_SADDR_RD_OVS_Y_OVS_C_T;

/*-----------------------------------------------------------------------------
	0x2e00 mifc_rd_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                 : 12;  //  0:11 reserved
	UINT32 disable_rd_mifc                 : 1;   //    12
	UINT32 load_rd_req_stv_v               : 1;   //    13
	UINT32 load_rd_req_pri_v               : 1;   //    14
	UINT32 load_rd_cmd_saddr_v             : 1;   //    15
	UINT32 load_rd_cmd_pend_v              : 1;   //    16
	UINT32 load_rd_cmd_pri_v               : 1;   //    17
	UINT32 load_rd_cmd_stride_v            : 1;   //    18
	UINT32 enable_rd_stv_cnt               : 1;   //    19
	UINT32                                 : 1;   //    20 reserved
	UINT32 mrb_bank_mode                   : 3;   // 21:23
} H14A0_MIFC_RD_CTRL_T;

/*-----------------------------------------------------------------------------
	0x2e04 mifc_wd_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                 : 12;  //  0:11 reserved
	UINT32 disable_wd_mifc                 : 1;   //    12
	UINT32 load_wd_req_stv_v               : 1;   //    13
	UINT32 load_wd_req_pri_v               : 1;   //    14
	UINT32 load_wd_cmd_saddr_v             : 1;   //    15
	UINT32 load_wd_cmd_pend_v              : 1;   //    16
	UINT32 load_wd_cmd_pri_v               : 1;   //    17
	UINT32 load_wd_cmd_stride_v            : 1;   //    18
	UINT32 enable_wd_stv_cnt               : 1;   //    19
	UINT32 enable_prew_wd_addr_pls         : 1;   //    20
	UINT32 mwb_bank_mode                   : 3;   // 21:23
} H14A0_MIFC_WD_CTRL_T;

/*-----------------------------------------------------------------------------
	0x2e08 mifc_rd_cmd_pri0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_pri_v_mvi_y              : 4;   //  0: 3
	UINT32 cmd_rd_pri_v_mvi_c              : 4;   //  4: 7
	UINT32 cmd_rd_pri_v_prew_y             : 4;   //  8:11
	UINT32 cmd_rd_pri_v_prew_c             : 4;   // 12:15
	UINT32 cmd_rd_pri_v_ipc0_y             : 4;   // 16:19
	UINT32 cmd_rd_pri_v_ipc0_c             : 4;   // 20:23
	UINT32 cmd_rd_pri_v_ipc1_y             : 4;   // 24:27
	UINT32 cmd_rd_pri_v_tnr0_y             : 4;   // 28:31
} H14A0_MIFC_RD_CMD_PRI0_T;

/*-----------------------------------------------------------------------------
	0x2e0c mifc_rd_cmd_pri1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_pri_v_tnr_c              : 4;   //  0: 3
	UINT32 cmd_rd_pri_v_ss_y               : 4;   //  4: 7
	UINT32 cmd_rd_pri_v_ss_c               : 4;   //  8:11
	UINT32 cmd_rd_pri_v_ipc1_c             : 4;   // 12:15
} H14A0_MIFC_RD_CMD_PRI1_T;

/*-----------------------------------------------------------------------------
	0x2e10 mifc_wd_cmd_pri0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_wd_pri_v_ss_y               : 4;   //  0: 3
	UINT32 cmd_wd_pri_v_ss_c               : 4;   //  4: 7
	UINT32 cmd_wd_pri_v_tnr_y              : 4;   //  8:11
	UINT32 cmd_wd_pri_v_tnr_c              : 4;   // 12:15
	UINT32 cmd_wd_pri_v_prew_y             : 4;   // 16:19
	UINT32 cmd_wd_pri_v_prew_c             : 4;   // 20:23
} H14A0_MIFC_WD_CMD_PRI0_T;

/*-----------------------------------------------------------------------------
	0x2e14 mifc_rd_req_pri0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_pri_v_mvi_y              : 4;   //  0: 3
	UINT32 req_rd_pri_v_mvi_c              : 4;   //  4: 7
	UINT32 req_rd_pri_v_prew_y             : 4;   //  8:11
	UINT32 req_rd_pri_v_prew_c             : 4;   // 12:15
	UINT32 req_rd_pri_v_ipc0_y             : 4;   // 16:19
	UINT32 req_rd_pri_v_ipc0_c             : 4;   // 20:23
	UINT32 req_rd_pri_v_ipc1_y             : 4;   // 24:27
	UINT32 req_rd_pri_v_tnr0_y             : 4;   // 28:31
} H14A0_MIFC_RD_REQ_PRI0_T;

/*-----------------------------------------------------------------------------
	0x2e18 mifc_rd_req_pri1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_pri_v_tnr_c              : 4;   //  0: 3
	UINT32 req_rd_pri_v_ss_y               : 4;   //  4: 7
	UINT32 req_rd_pri_v_ss_c               : 4;   //  8:11
	UINT32 req_rd_pri_v_ipc1_c             : 4;   // 12:15
} H14A0_MIFC_RD_REQ_PRI1_T;

/*-----------------------------------------------------------------------------
	0x2e1c mifc_wd_req_pri0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_wd_pri_v_ss_y               : 4;   //  0: 3
	UINT32 req_wd_pri_v_ss_c               : 4;   //  4: 7
	UINT32 req_wd_pri_v_tnr_y              : 4;   //  8:11
	UINT32 req_wd_pri_v_tnr_c              : 4;   // 12:15
	UINT32 req_wd_pri_v_prew_y             : 4;   // 16:19
	UINT32 req_wd_pri_v_prew_c             : 4;   // 20:23
} H14A0_MIFC_WD_REQ_PRI0_T;

/*-----------------------------------------------------------------------------
	0x2e20 mifc_rd_req_stv0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_stv_v_mvi_y              : 4;   //  0: 3
	UINT32 req_rd_stv_v_mvi_c              : 4;   //  4: 7
	UINT32 req_rd_stv_v_prew_y             : 4;   //  8:11
	UINT32 req_rd_stv_v_prew_c             : 4;   // 12:15
	UINT32 req_rd_stv_v_ipc0_y             : 4;   // 16:19
	UINT32 req_rd_stv_v_ipc0_c             : 4;   // 20:23
	UINT32 req_rd_stv_v_ipc1_y             : 4;   // 24:27
	UINT32 req_rd_stv_v_tnr0_y             : 4;   // 28:31
} H14A0_MIFC_RD_REQ_STV0_T;

/*-----------------------------------------------------------------------------
	0x2e24 mifc_rd_req_stv1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_pri_v_tnr_c              : 4;   //  0: 3
	UINT32 req_rd_pri_v_ss_y               : 4;   //  4: 7
	UINT32 req_rd_pri_v_ss_c               : 4;   //  8:11
	UINT32 req_rd_stv_v_ipc1_c             : 4;   // 12:15
} H14A0_MIFC_RD_REQ_STV1_T;

/*-----------------------------------------------------------------------------
	0x2e28 mifc_wd_req_stv0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_wd_stv_v_ss_y               : 4;   //  0: 3
	UINT32 req_wd_stv_v_ss_c               : 4;   //  4: 7
	UINT32 req_wd_stv_v_tnr_y              : 4;   //  8:11
	UINT32 req_wd_stv_v_tnr_c              : 4;   // 12:15
	UINT32 req_wd_stv_v_prew_y             : 4;   // 16:19
	UINT32 req_wd_stv_v_prew_c             : 4;   // 20:23
} H14A0_MIFC_WD_REQ_STV0_T;

/*-----------------------------------------------------------------------------
	0x2e2c mifc_rd_cmd_pend0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_pend_v_mvi_y             : 4;   //  0: 3
	UINT32 cmd_rd_pend_v_mvi_c             : 4;   //  4: 7
	UINT32 cmd_rd_pend_v_prew_y            : 4;   //  8:11
	UINT32 cmd_rd_pend_v_prew_c            : 4;   // 12:15
	UINT32 cmd_rd_pend_v_ipc0_y            : 4;   // 16:19
	UINT32 cmd_rd_pend_v_ipc0_c            : 4;   // 20:23
	UINT32 cmd_rd_pend_v_ipc1_y            : 4;   // 24:27
	UINT32 cmd_rd_pend_v_tnr0_y            : 4;   // 28:31
} H14A0_MIFC_RD_CMD_PEND0_T;

/*-----------------------------------------------------------------------------
	0x2e30 mifc_rd_cmd_pend1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_pend_v_tnr_c             : 4;   //  0: 3
	UINT32 cmd_rd_pend_v_ss_y              : 4;   //  4: 7
	UINT32 cmd_rd_pend_v_ss_c              : 4;   //  8:11
	UINT32 cmd_rd_pend_v_ipc1_c            : 4;   // 12:15
} H14A0_MIFC_RD_CMD_PEND1_T;

/*-----------------------------------------------------------------------------
	0x2e34 mifc_wd_cmd_pend0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_wd_pend_v_ss_y              : 4;   //  0: 3
	UINT32 cmd_wd_pend_v_ss_c              : 4;   //  4: 7
	UINT32 cmd_wd_pend_v_tnr_y             : 4;   //  8:11
	UINT32 cmd_wd_pend_v_tnr_c             : 4;   // 12:15
	UINT32 cmd_wd_pend_v_prew_y            : 4;   // 16:19
	UINT32 cmd_wd_pend_v_prew_c            : 4;   // 20:23
} H14A0_MIFC_WD_CMD_PEND0_T;

/*-----------------------------------------------------------------------------
	0x2e38 mifc_rd_cmd_stride0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_stride_v_mvi_y           : 4;   //  0: 3
	UINT32 cmd_rd_stride_v_mvi_c           : 4;   //  4: 7
	UINT32 cmd_rd_stride_v_prew_y          : 4;   //  8:11
	UINT32 cmd_rd_stride_v_prew_c          : 4;   // 12:15
	UINT32 cmd_rd_stride_v_ipc0_y          : 4;   // 16:19
	UINT32 cmd_rd_stride_v_ipc0_c          : 4;   // 20:23
	UINT32 cmd_rd_stride_v_ipc1_y          : 4;   // 24:27
	UINT32 cmd_rd_stride_v_tnr0_y          : 4;   // 28:31
} H14A0_MIFC_RD_CMD_STRIDE0_T;

/*-----------------------------------------------------------------------------
	0x2e3c mifc_rd_cmd_stride1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_stride_v_tnr_c           : 4;   //  0: 3
	UINT32 cmd_rd_stride_v_ss_y            : 4;   //  4: 7
	UINT32 cmd_rd_stride_v_ss_c            : 4;   //  8:11
	UINT32 cmd_rd_stride_v_ipc1_c          : 4;   // 12:15
} H14A0_MIFC_RD_CMD_STRIDE1_T;

/*-----------------------------------------------------------------------------
	0x2e40 mifc_rd_cmd_stride_set ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 man_cmd_rd_stride_v_mvi_y       : 1;   //     0
	UINT32 man_cmd_rd_stride_v_mvi_c       : 1;   //     1
	UINT32 man_cmd_rd_stride_v_prew_y      : 1;   //     2
	UINT32 man_cmd_rd_stride_v_prew_c      : 1;   //     3
	UINT32 man_cmd_rd_stride_v_ipc0_y      : 1;   //     4
	UINT32 man_cmd_rd_stride_v_ipc0_c      : 1;   //     5
	UINT32 man_cmd_rd_stride_v_ipc1_y      : 1;   //     6
	UINT32 man_cmd_rd_stride_v_tnr0_y      : 1;   //     7
	UINT32 man_cmd_rd_stride_v_tnr_c       : 1;   //     8
	UINT32 man_cmd_rd_stride_v_ss_y        : 1;   //     9
	UINT32 man_cmd_rd_stride_v_ss_c        : 1;   //    10
	UINT32 man_cmd_rd_stride_v_ipc1_c      : 1;   //    11
} H14A0_MIFC_RD_CMD_STRIDE_SET_T;

/*-----------------------------------------------------------------------------
	0x2e44 mifc_wd_cmd_stride0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_wd_stride_v_ss_y            : 4;   //  0: 3
	UINT32 cmd_wd_stride_v_ss_c            : 4;   //  4: 7
	UINT32 cmd_wd_stride_v_tnr_y           : 4;   //  8:11
	UINT32 cmd_wd_stride_v_tnr_c           : 4;   // 12:15
	UINT32 cmd_wd_stride_v_prew_y          : 4;   // 16:19
	UINT32 cmd_wd_stride_v_prew_c          : 4;   // 20:23
} H14A0_MIFC_WD_CMD_STRIDE0_T;

/*-----------------------------------------------------------------------------
	0x2e48 mifc_wd_cmd_stride_set ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 man_cmd_wd_stride_v_ss_y        : 1;   //     0
	UINT32 man_cmd_wd_stride_v_ss_c        : 1;   //     1
	UINT32 man_cmd_wd_stride_v_tnr_y       : 1;   //     2
	UINT32 man_cmd_wd_stride_v_tnr_c       : 1;   //     3
	UINT32 man_cmd_wd_stride_v_prew_y      : 1;   //     4
	UINT32 man_cmd_wd_stride_v_prew_c      : 1;   //     5
} H14A0_MIFC_WD_CMD_STRIDE_SET_T;

/*-----------------------------------------------------------------------------
	0x2e4c mifc_saddr_rd_mvi_y_mvi_c ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                 : 3;   //  0: 2 reserved
	UINT32 req_rd_saddr_mvi_y              : 13;  //  3:15
	UINT32                                 : 3;   // 16:18 reserved
	UINT32 req_rd_saddr_mvi_c              : 13;  // 19:31
} H14A0_MIFC_SADDR_RD_MVI_Y_MVI_C_T;

/*-----------------------------------------------------------------------------
	0x2e50 mifc_saddr_rd_prew_y_prew_c ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                 : 3;   //  0: 2 reserved
	UINT32 req_rd_saddr_prew_y             : 13;  //  3:15
	UINT32                                 : 3;   // 16:18 reserved
	UINT32 req_rd_saddr_prew_c             : 13;  // 19:31
} H14A0_MIFC_SADDR_RD_PREW_Y_PREW_C_T;

/*-----------------------------------------------------------------------------
	0x2e54 mifc_saddr_rd_ipc0_y_ipc0_c ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                 : 3;   //  0: 2 reserved
	UINT32 req_rd_saddr_ipc0_y             : 13;  //  3:15
	UINT32                                 : 3;   // 16:18 reserved
	UINT32 req_rd_saddr_ipc0_c             : 13;  // 19:31
} H14A0_MIFC_SADDR_RD_IPC0_Y_IPC0_C_T;

/*-----------------------------------------------------------------------------
	0x2e58 mifc_saddr_rd_ipc1_y_tnr0_y ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                 : 3;   //  0: 2 reserved
	UINT32 req_rd_saddr_ipc1_y             : 13;  //  3:15
	UINT32                                 : 3;   // 16:18 reserved
	UINT32 req_rd_saddr_tnr0_y             : 13;  // 19:31
} H14A0_MIFC_SADDR_RD_IPC1_Y_TNR0_Y_T;

/*-----------------------------------------------------------------------------
	0x2e5c mifc_saddr_rd_ss_y_ss_c ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                 : 3;   //  0: 2 reserved
	UINT32 req_rd_saddr_ss_y               : 13;  //  3:15
	UINT32                                 : 3;   // 16:18 reserved
	UINT32 req_rd_saddr_ss_c               : 13;  // 19:31
} H14A0_MIFC_SADDR_RD_SS_Y_SS_C_T;

/*-----------------------------------------------------------------------------
	0x2e60 mifc_saddr_rd_tnr_c_ipc1_c ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                 : 3;   //  0: 2 reserved
	UINT32 req_rd_saddr_tnr_c              : 13;  //  3:15
	UINT32                                 : 3;   // 16:18 reserved
	UINT32 req_rd_saddr_ipc1_c             : 13;  // 19:31
} H14A0_MIFC_SADDR_RD_TNR_C_IPC1_C_T;

/*-----------------------------------------------------------------------------
	0x2e64 mifc_saddr_wd_ss_y_ss_c ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                 : 3;   //  0: 2 reserved
	UINT32 req_wd_saddr_ss_y               : 13;  //  3:15
	UINT32                                 : 3;   // 16:18 reserved
	UINT32 req_wd_saddr_ss_c               : 13;  // 19:31
} H14A0_MIFC_SADDR_WD_SS_Y_SS_C_T;

/*-----------------------------------------------------------------------------
	0x2e68 mifc_saddr_wd_tnr_y_tnr_c ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                 : 3;   //  0: 2 reserved
	UINT32 req_wd_saddr_tnr_y              : 13;  //  3:15
	UINT32                                 : 3;   // 16:18 reserved
	UINT32 req_wd_saddr_tnr_c              : 13;  // 19:31
} H14A0_MIFC_SADDR_WD_TNR_Y_TNR_C_T;

/*-----------------------------------------------------------------------------
	0x2e6c mifc_saddr_wd_prew_y_prew_c ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                 : 3;   //  0: 2 reserved
	UINT32 req_wd_saddr_prew_y             : 13;  //  3:15
	UINT32                                 : 3;   // 16:18 reserved
	UINT32 req_wd_saddr_prew_c             : 13;  // 19:31
} H14A0_MIFC_SADDR_WD_PREW_Y_PREW_C_T;

/*-----------------------------------------------------------------------------
	0x2e70 mifc_saddr_rd_dl_y_dl_c ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                 : 3;   //  0: 2 reserved
	UINT32 req_rd_saddr_dl_y               : 13;  //  3:15
	UINT32                                 : 3;   // 16:18 reserved
	UINT32 req_rd_saddr_dl_c               : 13;  // 19:31
} H14A0_MIFC_SADDR_RD_DL_Y_DL_C_T;

/*-----------------------------------------------------------------------------
	0x2e74 mifc_saddr_rd_dr_y_dr_c ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                 : 3;   //  0: 2 reserved
	UINT32 req_rd_saddr_dr_y               : 13;  //  3:15
	UINT32                                 : 3;   // 16:18 reserved
	UINT32 req_rd_saddr_dr_c               : 13;  // 19:31
} H14A0_MIFC_SADDR_RD_DR_Y_DR_C_T;

/*-----------------------------------------------------------------------------
	0x2e78 mifc_saddr_wd_dl_y_dl_c ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                 : 3;   //  0: 2 reserved
	UINT32 req_wd_saddr_dl_y               : 13;  //  3:15
	UINT32                                 : 3;   // 16:18 reserved
	UINT32 req_wd_saddr_dl_c               : 13;  // 19:31
} H14A0_MIFC_SADDR_WD_DL_Y_DL_C_T;

/*-----------------------------------------------------------------------------
	0x2e7c mifc_saddr_wd_dr_y_dr_c ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                 : 3;   //  0: 2 reserved
	UINT32 req_wd_saddr_dr_y               : 13;  //  3:15
	UINT32                                 : 3;   // 16:18 reserved
	UINT32 req_wd_saddr_dr_c               : 13;  // 19:31
} H14A0_MIFC_SADDR_WD_DR_Y_DR_C_T;

/*-----------------------------------------------------------------------------
	0x2e80 mifc_saddr_rd_ext0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_saddr_mvi_y_msb4b        : 4;   //  0: 3
	UINT32 req_rd_saddr_mvi_c_msb4b        : 4;   //  4: 7
	UINT32 req_rd_saddr_prew_y_msb4b       : 4;   //  8:11
	UINT32 req_rd_saddr_prew_c_msb4b       : 4;   // 12:15
	UINT32 req_rd_saddr_ipc0_y_msb4b       : 4;   // 16:19
	UINT32 req_rd_saddr_ipc0_c_msb4b       : 4;   // 20:23
	UINT32 req_rd_saddr_ipc1_y_msb4b       : 4;   // 24:27
	UINT32 req_rd_saddr_tnr0_y_msb4b       : 4;   // 28:31
} H14A0_MIFC_SADDR_RD_EXT0_T;

/*-----------------------------------------------------------------------------
	0x2e84 mifc_saddr_rd_ext1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_saddr_tnr_c_msb4b        : 4;   //  0: 3
	UINT32 req_rd_saddr_ss_y_msb4b         : 4;   //  4: 7
	UINT32 req_rd_saddr_ss_c_msb4b         : 4;   //  8:11
	UINT32 req_rd_saddr_ipc1_c_msb4b       : 4;   // 12:15
} H14A0_MIFC_SADDR_RD_EXT1_T;

/*-----------------------------------------------------------------------------
	0x2e88 mifc_saddr_wd_ext0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_wd_saddr_ss_y_msb4b         : 4;   //  0: 3
	UINT32 req_wd_saddr_ss_c_msb4b         : 4;   //  4: 7
	UINT32 req_wd_saddr_tnr_y_msb4b        : 4;   //  8:11
	UINT32 req_wd_saddr_tnr_c_msb4b        : 4;   // 12:15
	UINT32 req_wd_saddr_prew_y_msb4b       : 4;   // 16:19
	UINT32 req_wd_saddr_prew_c_msb4b       : 4;   // 20:23
} H14A0_MIFC_SADDR_WD_EXT0_T;

/*-----------------------------------------------------------------------------
	0x2e8c mifc_rd_dr_dl_ext ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_saddr_dl_y_msb4b         : 4;   //  0: 3
	UINT32 req_rd_saddr_dl_c_msb4b         : 4;   //  4: 7
	UINT32 req_rd_saddr_dr_y_msb4b         : 4;   //  8:11
	UINT32 req_rd_saddr_dr_c_msb4b         : 4;   // 12:15
} H14A0_MIFC_RD_DR_DL_EXT_T;

/*-----------------------------------------------------------------------------
	0x2e90 mifc_wd_dr_dl_ext ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_wd_saddr_dl_y_msb4b         : 4;   //  0: 3
	UINT32 req_wd_saddr_dl_c_msb4b         : 4;   //  4: 7
	UINT32 req_wd_saddr_dr_y_msb4b         : 4;   //  8:11
	UINT32 req_wd_saddr_dr_c_msb4b         : 4;   // 12:15
} H14A0_MIFC_WD_DR_DL_EXT_T;

/*-----------------------------------------------------------------------------
	0x2e94 mifc_status ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 fr_rd_valid_mvi_y               : 1;   //     0
	UINT32 fr_rd_valid_mvi_c               : 1;   //     1
	UINT32 fr_rd_valid_prew_y              : 1;   //     2
	UINT32 fr_rd_valid_prew_c              : 1;   //     3
	UINT32 fr_rd_valid_ipc0_y              : 1;   //     4
	UINT32 fr_rd_valid_ipc0_c              : 1;   //     5
	UINT32 fr_rd_valid_ipc1_y              : 1;   //     6
	UINT32 fr_rd_valid_tnr0_y              : 1;   //     7
	UINT32 fr_rd_valid_tnr_c               : 1;   //     8
	UINT32 fr_rd_valid_ss_y                : 1;   //     9
	UINT32 fr_rd_valid_ss_c                : 1;   //    10
	UINT32 fr_wd_q_full_ss_y               : 1;   //    11
	UINT32 fr_wd_q_full_ss_c               : 1;   //    12
	UINT32 fr_wd_q_full_tnr_y              : 1;   //    13
	UINT32 fr_wd_q_full_tnr_c              : 1;   //    14
	UINT32 fr_wd_q_full_prew_y             : 1;   //    15
	UINT32 fr_wd_q_full_prew_c             : 1;   //    16
	UINT32 fr_rd_valid_ipc1_c              : 1;   //    17
} H14A0_MIFC_STATUS_T;

/*-----------------------------------------------------------------------------
	0x2e98 mifc_saddr_param_update0_m ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 load                            : 1;   //     0
	UINT32                                 : 10;  //  1:10 reserved
	UINT32 register_update_address0_m      : 21;  // 11:31
} H14A0_MIFC_SADDR_PARAM_UPDATE0_M_T;

/*-----------------------------------------------------------------------------
	0x2e9c mifc_rd_bank_8_addr0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_bank_split_8_mvi_y       : 1;   //     0
	UINT32 req_rd_bank_addr_8_mvi_y        : 3;   //  1: 3
	UINT32 req_rd_bank_split_8_mvi_c       : 1;   //     4
	UINT32 req_rd_bank_addr_8_mvi_c        : 3;   //  5: 7
	UINT32 req_rd_bank_split_8_prew_y      : 1;   //     8
	UINT32 req_rd_bank_addr_8_prew_y       : 3;   //  9:11
	UINT32 req_rd_bank_split_8_prew_c      : 1;   //    12
	UINT32 req_rd_bank_addr_8_prew_c       : 3;   // 13:15
	UINT32 req_rd_bank_split_8_ipc0_y      : 1;   //    16
	UINT32 req_rd_bank_addr_8_ipc0_y       : 3;   // 17:19
	UINT32 req_rd_bank_split_8_ipc0_c      : 1;   //    20
	UINT32 req_rd_bank_addr_8_ipc0_c       : 3;   // 21:23
	UINT32 req_rd_bank_split_8_ipc1_y      : 1;   //    24
	UINT32 req_rd_bank_addr_8_ipc1_y       : 3;   // 25:27
	UINT32 req_rd_bank_split_8_tnr0_y      : 1;   //    28
	UINT32 req_rd_bank_addr_8_tnr0_y       : 3;   // 29:31
} H14A0_MIFC_RD_BANK_8_ADDR0_T;

/*-----------------------------------------------------------------------------
	0x2ea0 mifc_rd_bank_8_addr1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_bank_split_8_tnr_c       : 1;   //     0
	UINT32 req_rd_bank_addr_8_tnr_c        : 3;   //  1: 3
	UINT32 req_rd_bank_split_8_ss_y        : 1;   //     4
	UINT32 req_rd_bank_addr_8_ss_y         : 3;   //  5: 7
	UINT32 req_rd_bank_split_8_ss_c        : 1;   //     8
	UINT32 req_rd_bank_addr_8_ss_c         : 3;   //  9:11
	UINT32 req_rd_bank_split_8_ipc1_c      : 1;   //    12
	UINT32 req_rd_bank_addr_8_ipc1_c       : 3;   // 13:15
} H14A0_MIFC_RD_BANK_8_ADDR1_T;

/*-----------------------------------------------------------------------------
	0x2ea4 mifc_wd_bank_8_addr0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_wd_bank_split_8_ss_y        : 1;   //     0
	UINT32 req_wd_bank_addr_8_ss_y         : 3;   //  1: 3
	UINT32 req_wd_bank_split_8_ss_c        : 1;   //     4
	UINT32 req_wd_bank_addr_8_ss_c         : 3;   //  5: 7
	UINT32 req_wd_bank_split_8_tnr_y       : 1;   //     8
	UINT32 req_wd_bank_addr_8_tnr_y        : 3;   //  9:11
	UINT32 req_wd_bank_split_8_tnr_c       : 1;   //    12
	UINT32 req_wd_bank_addr_8_tnr_c        : 3;   // 13:15
	UINT32 req_wd_bank_split_8_prew_y      : 1;   //    16
	UINT32 req_wd_bank_addr_8_prew_y       : 3;   // 17:19
	UINT32 req_wd_bank_split_8_prew_c      : 1;   //    20
	UINT32 req_wd_bank_addr_8_prew_c       : 3;   // 21:23
} H14A0_MIFC_WD_BANK_8_ADDR0_T;

/*-----------------------------------------------------------------------------
	0x2ea8 mifc_wd_bank_8_addr1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 mifc_wd_bank_8_addr1_f          : 32;   // 31: 0
} H14A0_MIFC_WD_BANK_8_ADDR1_T;

/*-----------------------------------------------------------------------------
	0x2eac mifc_rd_req_pend0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_pend_v_mvi_y             : 8;   //  0: 7
	UINT32 req_rd_pend_v_mvi_c             : 8;   //  8:15
	UINT32 req_rd_pend_v_prew_y            : 8;   // 16:23
	UINT32 req_rd_pend_v_prew_c            : 8;   // 24:31
} H14A0_MIFC_RD_REQ_PEND0_T;

/*-----------------------------------------------------------------------------
	0x2eb0 mifc_rd_req_pend1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_pend_v_ipc0_y            : 8;   //  0: 7
	UINT32 req_rd_pend_v_ipc0_c            : 8;   //  8:15
	UINT32 req_rd_pend_v_ipc1_y            : 8;   // 16:23
	UINT32 req_rd_pend_v_tnr0_y            : 8;   // 24:31
} H14A0_MIFC_RD_REQ_PEND1_T;

/*-----------------------------------------------------------------------------
	0x2eb4 mifc_rd_req_pend2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_pend_v_tnr_c             : 8;   //  0: 7
	UINT32 req_rd_pend_v_ss_y              : 8;   //  8:15
	UINT32 req_rd_pend_v_ss_c              : 8;   // 16:23
	UINT32 req_rd_pend_v_ipc1_c            : 8;   // 24:31
} H14A0_MIFC_RD_REQ_PEND2_T;

/*-----------------------------------------------------------------------------
	0x2eb8 mifc_wd_req_pend0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_wd_pend_v_ss_y              : 8;   //  0: 7
	UINT32 req_wd_pend_v_ss_c              : 8;   //  8:15
	UINT32 req_wd_pend_v_tnr_y             : 8;   // 16:23
	UINT32 req_wd_pend_v_tnr_c             : 8;   // 24:31
} H14A0_MIFC_WD_REQ_PEND0_T;

/*-----------------------------------------------------------------------------
	0x2ebc mifc_wd_req_pend1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_wd_pend_v_prew_y            : 8;   //  0: 7
	UINT32 req_wd_pend_v_prew_c            : 8;   //  8:15
} H14A0_MIFC_WD_REQ_PEND1_T;

/*-----------------------------------------------------------------------------
	0x2ec0 mifc_rd_cmd_pri0_s ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_pri_s_v_mvi_y            : 4;   //  0: 3
	UINT32 cmd_rd_pri_s_v_mvi_c            : 4;   //  4: 7
	UINT32 cmd_rd_pri_s_v_prew_y           : 4;   //  8:11
	UINT32 cmd_rd_pri_s_v_prew_c           : 4;   // 12:15
	UINT32 cmd_rd_pri_s_v_ipc0_y           : 4;   // 16:19
	UINT32 cmd_rd_pri_s_v_ipc0_c           : 4;   // 20:23
	UINT32 cmd_rd_pri_s_v_ipc1_y           : 4;   // 24:27
	UINT32 cmd_rd_pri_s_v_tnr0_y           : 4;   // 28:31
} H14A0_MIFC_RD_CMD_PRI0_S_T;

/*-----------------------------------------------------------------------------
	0x2ec4 mifc_rd_cmd_pri1_s ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_pri_s_v_tnr_c            : 4;   //  0: 3
	UINT32 cmd_rd_pri_s_v_ss_y             : 4;   //  4: 7
	UINT32 cmd_rd_pri_s_v_ss_c             : 4;   //  8:11
	UINT32 cmd_rd_pri_s_v_ipc1_c           : 4;   // 12:15
} H14A0_MIFC_RD_CMD_PRI1_S_T;

/*-----------------------------------------------------------------------------
	0x2ec8 mifc_wd_cmd_pri0_s ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_wd_pri_s_v_ss_y             : 4;   //  0: 3
	UINT32 cmd_wd_pri_s_v_ss_c             : 4;   //  4: 7
	UINT32 cmd_wd_pri_s_v_tnr_y            : 4;   //  8:11
	UINT32 cmd_wd_pri_s_v_tnr_c            : 4;   // 12:15
	UINT32 cmd_wd_pri_s_v_prew_y           : 4;   // 16:19
	UINT32 cmd_wd_pri_s_v_prew_c           : 4;   // 20:23
} H14A0_MIFC_WD_CMD_PRI0_S_T;

/*-----------------------------------------------------------------------------
	0x2ecc mifc_reset ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 reset_mif_r_mvi_y               : 1;   //     0
	UINT32 reset_mif_r_mvi_c               : 1;   //     1
	UINT32 reset_mif_r_prew_y              : 1;   //     2
	UINT32 reset_mif_r_prew_c              : 1;   //     3
	UINT32 reset_mif_r_ipc0_y              : 1;   //     4
	UINT32 reset_mif_r_ipc0_c              : 1;   //     5
	UINT32 reset_mif_r_ipc1_y              : 1;   //     6
	UINT32 reset_mif_r_tnr0_y              : 1;   //     7
	UINT32 reset_mif_r_tnr_c               : 1;   //     8
	UINT32 reset_mif_r_ss_y                : 1;   //     9
	UINT32 reset_mif_r_ss_c                : 1;   //    10
	UINT32 reset_mif_w_ss_y                : 1;   //    11
	UINT32 reset_mif_w_ss_c                : 1;   //    12
	UINT32 reset_mif_w_tnr_y               : 1;   //    13
	UINT32 reset_mif_w_tnr_c               : 1;   //    14
	UINT32 reset_mif_w_prew_y              : 1;   //    15
	UINT32 reset_mif_w_prew_c              : 1;   //    16
	UINT32 reset_mif_r_ipc1_c              : 1;   //    17
	UINT32                                 : 2;   // 18:19 reserved
	UINT32 reset_time_out_cnt              : 12;  // 20:31
} H14A0_MIFC_RESET_T;

/*-----------------------------------------------------------------------------
	0x2ed0 mifc_rd_set_10b ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 set_10b_req_rd_prew_y           : 1;   //     0
	UINT32 set_10b_req_rd_prew_c           : 1;   //     1
	UINT32 set_10b_req_rd_ipc0_y           : 1;   //     2
	UINT32 set_10b_req_rd_ipc0_c           : 1;   //     3
	UINT32 set_10b_req_rd_ipc1_y           : 1;   //     4
	UINT32 set_10b_req_rd_tnr0_y           : 1;   //     5
	UINT32 set_10b_req_rd_tnr_c            : 1;   //     6
	UINT32 set_10b_req_rd_ipc1_c           : 1;   //     7
} H14A0_MIFC_RD_SET_10B_T;

/*-----------------------------------------------------------------------------
	0x2ed4 mifc_wd_set_10b ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 set_10b_req_wd_tnr_y            : 1;   //     0
	UINT32 set_10b_req_wd_tnr_c            : 1;   //     1
	UINT32 set_10b_req_wd_prew_y           : 1;   //     2
	UINT32 set_10b_req_wd_prew_c           : 1;   //     3
} H14A0_MIFC_WD_SET_10B_T;

/*-----------------------------------------------------------------------------
	0x2ed8 mifc_rd_cmd_2_stride0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_stride_v_2_prew_y        : 4;   //  0: 3
	UINT32 cmd_rd_stride_v_2_prew_c        : 4;   //  4: 7
	UINT32 cmd_rd_stride_v_2_ipc0_y        : 4;   //  8:11
	UINT32 cmd_rd_stride_v_2_ipc0_c        : 4;   // 12:15
	UINT32 cmd_rd_stride_v_2_ipc1_y        : 4;   // 16:19
	UINT32 cmd_rd_stride_v_2_tnr0_y        : 4;   // 20:23
	UINT32 cmd_rd_stride_v_2_tnr_c         : 4;   // 24:27
	UINT32 cmd_rd_stride_v_2_ipc1_c        : 4;   // 28:31
} H14A0_MIFC_RD_CMD_2_STRIDE0_T;

/*-----------------------------------------------------------------------------
	0x2edc mifc_wd_cmd_2_stride0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_wd_stride_v_2_tnr_y         : 4;   //  0: 3
	UINT32 cmd_wd_stride_v_2_tnr_c         : 4;   //  4: 7
	UINT32 cmd_wd_stride_v_2_prew_y        : 4;   //  8:11
	UINT32 cmd_wd_stride_v_2_prew_c        : 4;   // 12:15
} H14A0_MIFC_WD_CMD_2_STRIDE0_T;

/*-----------------------------------------------------------------------------
	0x2ee0 mifc_saddr_rd_prew_y_prew_c_2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                 : 3;   //  0: 2 reserved
	UINT32 req_rd_saddr_2_prew_y           : 13;  //  3:15
	UINT32                                 : 3;   // 16:18 reserved
	UINT32 req_rd_saddr_2_prew_c           : 13;  // 19:31
} H14A0_MIFC_SADDR_RD_PREW_Y_PREW_C_2_T;

/*-----------------------------------------------------------------------------
	0x2ee4 mifc_saddr_rd_ipc0_y_ipc0_c_2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                 : 3;   //  0: 2 reserved
	UINT32 req_rd_saddr_2_ipc0_y           : 13;  //  3:15
	UINT32                                 : 3;   // 16:18 reserved
	UINT32 req_rd_saddr_2_ipc0_c           : 13;  // 19:31
} H14A0_MIFC_SADDR_RD_IPC0_Y_IPC0_C_2_T;

/*-----------------------------------------------------------------------------
	0x2ee8 mifc_saddr_rd_ipc1_y_tnr0_y_2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                 : 3;   //  0: 2 reserved
	UINT32 req_rd_saddr_2_ipc1_y           : 13;  //  3:15
	UINT32                                 : 3;   // 16:18 reserved
	UINT32 req_rd_saddr_2_tnr0_y           : 13;  // 19:31
} H14A0_MIFC_SADDR_RD_IPC1_Y_TNR0_Y_2_T;

/*-----------------------------------------------------------------------------
	0x2eec mifc_saddr_rd_tnr_c_ipc1_c_2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                 : 3;   //  0: 2 reserved
	UINT32 req_rd_saddr_2_tnr_c            : 13;  //  3:15
	UINT32                                 : 3;   // 16:18 reserved
	UINT32 req_rd_saddr_2_ipc1_c           : 13;  // 19:31
} H14A0_MIFC_SADDR_RD_TNR_C_IPC1_C_2_T;

/*-----------------------------------------------------------------------------
	0x2ef0 mifc_saddr_wd_tnr_y_tnr_c_2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                 : 3;   //  0: 2 reserved
	UINT32 req_wd_saddr_2_tnr_y            : 13;  //  3:15
	UINT32                                 : 3;   // 16:18 reserved
	UINT32 req_wd_saddr_2_tnr_c            : 13;  // 19:31
} H14A0_MIFC_SADDR_WD_TNR_Y_TNR_C_2_T;

/*-----------------------------------------------------------------------------
	0x2ef4 mifc_saddr_wd_prew_y_prew_c_2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                 : 3;   //  0: 2 reserved
	UINT32 req_wd_saddr_2_prew_y           : 13;  //  3:15
	UINT32                                 : 3;   // 16:18 reserved
	UINT32 req_wd_saddr_2_prew_c           : 13;  // 19:31
} H14A0_MIFC_SADDR_WD_PREW_Y_PREW_C_2_T;

/*-----------------------------------------------------------------------------
	0x2ef8 mifc_saddr_rd_2_ext0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_saddr_2_prew_y_msb4b     : 4;   //  0: 3
	UINT32 req_rd_saddr_2_prew_c_msb4b     : 4;   //  4: 7
	UINT32 req_rd_saddr_2_ipc0_y_msb4b     : 4;   //  8:11
	UINT32 req_rd_saddr_2_ipc0_c_msb4b     : 4;   // 12:15
	UINT32 req_rd_saddr_2_ipc1_y_msb4b     : 4;   // 16:19
	UINT32 req_rd_saddr_2_tnr0_y_msb4b     : 4;   // 20:23
	UINT32 req_rd_saddr_2_tnr_c_msb4b      : 4;   // 24:27
	UINT32 req_rd_saddr_2_ipc1_c_msb4b     : 4;   // 28:31
} H14A0_MIFC_SADDR_RD_2_EXT0_T;

/*-----------------------------------------------------------------------------
	0x2efc mifc_saddr_wd_2_ext0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_wd_saddr_2_tnr_y_msb4b      : 4;   //  0: 3
	UINT32 req_wd_saddr_2_tnr_c_msb4b      : 4;   //  4: 7
	UINT32 req_wd_saddr_2_prew_y_msb4b     : 4;   //  8:11
	UINT32 req_wd_saddr_2_prew_c_msb4b     : 4;   // 12:15
} H14A0_MIFC_SADDR_WD_2_EXT0_T;

/*-----------------------------------------------------------------------------
	0x2f00 mifc_rd_bank_2_addr0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                 : 8;   //  0: 7 reserved
	UINT32 req_rd_bank_split_2_prew_y      : 1;   //     8
	UINT32 req_rd_bank_addr_2_prew_y       : 3;   //  9:11
	UINT32 req_rd_bank_split_2_prew_c      : 1;   //    12
	UINT32 req_rd_bank_addr_2_prew_c       : 3;   // 13:15
	UINT32 req_rd_bank_split_2_ipc0_y      : 1;   //    16
	UINT32 req_rd_bank_addr_2_ipc0_y       : 3;   // 17:19
	UINT32 req_rd_bank_split_2_ipc0_c      : 1;   //    20
	UINT32 req_rd_bank_addr_2_ipc0_c       : 3;   // 21:23
	UINT32 req_rd_bank_split_2_ipc1_y      : 1;   //    24
	UINT32 req_rd_bank_addr_2_ipc1_y       : 3;   // 25:27
	UINT32 req_rd_bank_split_2_tnr0_y      : 1;   //    28
	UINT32 req_rd_bank_addr_2_tnr0_y       : 3;   // 29:31
} H14A0_MIFC_RD_BANK_2_ADDR0_T;

/*-----------------------------------------------------------------------------
	0x2f04 mifc_rd_bank_2_addr1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_bank_split_2_tnr_c       : 1;   //     0
	UINT32 req_rd_bank_addr_2_tnr_c        : 3;   //  1: 3
	UINT32                                 : 8;   //  4:11 reserved
	UINT32 req_rd_bank_split_2_ipc1_c      : 1;   //    12
	UINT32 req_rd_bank_addr_2_ipc1_c       : 3;   // 13:15
} H14A0_MIFC_RD_BANK_2_ADDR1_T;

/*-----------------------------------------------------------------------------
	0x2f08 mifc_wd_bank_2_addr0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                 : 8;   //  0: 7 reserved
	UINT32 req_wd_bank_split_2_tnr_y       : 1;   //     8
	UINT32 req_wd_bank_addr_2_tnr_y        : 3;   //  9:11
	UINT32 req_wd_bank_split_2_tnr_c       : 1;   //    12
	UINT32 req_wd_bank_addr_2_tnr_c        : 3;   // 13:15
	UINT32 req_wd_bank_split_2_prew_y      : 1;   //    16
	UINT32 req_wd_bank_addr_2_prew_y       : 3;   // 17:19
	UINT32 req_wd_bank_split_2_prew_c      : 1;   //    20
	UINT32 req_wd_bank_addr_2_prew_c       : 3;   // 21:23
} H14A0_MIFC_WD_BANK_2_ADDR0_T;

/*-----------------------------------------------------------------------------
	0x2f0c mifc_wd_bank_2_addr1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 mifc_wd_bank_2_addr1_f          : 32;   // 31: 0
} H14A0_MIFC_WD_BANK_2_ADDR1_T;

/*-----------------------------------------------------------------------------
	0x2f10 mifc_lsb_switch ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_lsb_prew_y               : 2;   //  0: 1
	UINT32 req_rd_lsb_prew_c               : 2;   //  2: 3
	UINT32 req_rd_lsb_ipc0_y               : 2;   //  4: 5
	UINT32 req_rd_lsb_ipc0_c               : 2;   //  6: 7
	UINT32 req_rd_lsb_ipc1_y               : 2;   //  8: 9
	UINT32 req_rd_lsb_tnr0_y               : 2;   // 10:11
	UINT32 req_rd_lsb_tnr_c                : 2;   // 12:13
	UINT32 req_rd_lsb_ipc1_c               : 2;   // 14:15
} H14A0_MIFC_LSB_SWITCH_T;

/*-----------------------------------------------------------------------------
	0x2f14 mifc_mvi_r_stride ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 mvi_rb_manual_x16_stride_size   : 13;  //  0:12
	UINT32                                 : 3;   // 13:15 reserved
	UINT32 mvi_rb_manual_x16_stride_enable : 1;   //    16
	UINT32                                 : 3;   // 17:19 reserved
	UINT32 mvi_rb_chroma_stride_size_set   : 1;   //    20
} H14A0_MIFC_MVI_R_STRIDE_T;

/*-----------------------------------------------------------------------------
	0x3e00 mifd_rd_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                 : 12;  //  0:11 reserved
	UINT32 disable_rd_mif                  : 1;   //    12
	UINT32 load_rd_req_stv_v               : 1;   //    13
	UINT32 load_rd_req_pri_v               : 1;   //    14
	UINT32 load_rd_cmd_saddr_v             : 1;   //    15
	UINT32 load_rd_cmd_pend_v              : 1;   //    16
	UINT32 load_rd_cmd_pri_v               : 1;   //    17
	UINT32 load_rd_cmd_stride_v            : 1;   //    18
	UINT32 enable_rd_stv_cnt               : 1;   //    19
	UINT32                                 : 1;   //    20 reserved
	UINT32 mrb_bank_mode                   : 3;   // 21:23
} H14A0_MIFD_RD_CTRL_T;

/*-----------------------------------------------------------------------------
	0x3e04 mifd_wd_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                 : 12;  //  0:11 reserved
	UINT32 disable_wd_mif                  : 1;   //    12
	UINT32 load_wd_req_stv_v               : 1;   //    13
	UINT32 load_wd_req_pri_v               : 1;   //    14
	UINT32 load_wd_cmd_saddr_v             : 1;   //    15
	UINT32 load_wd_cmd_pend_v              : 1;   //    16
	UINT32 load_wd_cmd_pri_v               : 1;   //    17
	UINT32 load_wd_cmd_stride_v            : 1;   //    18
	UINT32 enable_wd_stv_cnt               : 1;   //    19
} H14A0_MIFD_WD_CTRL_T;

/*-----------------------------------------------------------------------------
	0x3e08 mifd_rd_cmd_pri0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_pri_v_ov_y               : 4;   //  0: 3
	UINT32 cmd_rd_pri_v_ov_c               : 4;   //  4: 7
	UINT32 cmd_rd_pri_v_cve_y              : 4;   //  8:11
	UINT32 cmd_rd_pri_v_cve_c              : 4;   // 12:15
} H14A0_MIFD_RD_CMD_PRI0_T;

/*-----------------------------------------------------------------------------
	0x3e0c mifd_wd_cmd_pri0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_wd_pri_v_sm_y               : 4;   //  0: 3
	UINT32 cmd_wd_pri_v_sm_c               : 4;   //  4: 7
} H14A0_MIFD_WD_CMD_PRI0_T;

/*-----------------------------------------------------------------------------
	0x3e10 mifd_rd_req_pri0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_pri_v_ov_y               : 4;   //  0: 3
	UINT32 req_rd_pri_v_ov_c               : 4;   //  4: 7
	UINT32 req_rd_pri_v_cve_y              : 4;   //  8:11
	UINT32 req_rd_pri_v_cve_c              : 4;   // 12:15
} H14A0_MIFD_RD_REQ_PRI0_T;

/*-----------------------------------------------------------------------------
	0x3e14 mifd_wd_req_pri0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_wd_pri_v_sm_y               : 4;   //  0: 3
	UINT32 req_wd_pri_v_sm_c               : 4;   //  4: 7
} H14A0_MIFD_WD_REQ_PRI0_T;

/*-----------------------------------------------------------------------------
	0x3e18 mifd_rd_req_stv0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_stv_v_ov_y               : 4;   //  0: 3
	UINT32 req_rd_stv_v_ov_c               : 4;   //  4: 7
	UINT32 req_rd_stv_v_cve_y              : 4;   //  8:11
	UINT32 req_rd_stv_v_cve_c              : 4;   // 12:15
} H14A0_MIFD_RD_REQ_STV0_T;

/*-----------------------------------------------------------------------------
	0x3e1c mifd_wd_req_stv0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_wd_stv_v_sm_y               : 4;   //  0: 3
	UINT32 req_wd_stv_v_sm_c               : 4;   //  4: 7
} H14A0_MIFD_WD_REQ_STV0_T;

/*-----------------------------------------------------------------------------
	0x3e20 mifd_rd_cmd_pend0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_pend_v_ov_y              : 4;   //  0: 3
	UINT32 cmd_rd_pend_v_ov_c              : 4;   //  4: 7
	UINT32 cmd_rd_pend_v_cve_y             : 4;   //  8:11
	UINT32 cmd_rd_pend_v_cve_c             : 4;   // 12:15
} H14A0_MIFD_RD_CMD_PEND0_T;

/*-----------------------------------------------------------------------------
	0x3e24 mifd_wd_cmd_pend0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_wd_pend_v_sm_y              : 4;   //  0: 3
	UINT32 cmd_wd_pend_v_sm_c              : 4;   //  4: 7
} H14A0_MIFD_WD_CMD_PEND0_T;

/*-----------------------------------------------------------------------------
	0x3e28 mifd_rd_cmd_stride0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_stride_v_ov_y            : 4;   //  0: 3
	UINT32 cmd_rd_stride_v_ov_c            : 4;   //  4: 7
	UINT32 cmd_rd_stride_v_cve_y           : 4;   //  8:11
	UINT32 cmd_rd_stride_v_cve_c           : 4;   // 12:15
} H14A0_MIFD_RD_CMD_STRIDE0_T;

/*-----------------------------------------------------------------------------
	0x3e2c mifd_rd_cmd_stride_set ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 man_cmd_rd_stride_v_ov_y        : 1;   //     0
	UINT32 man_cmd_rd_stride_v_ov_c        : 1;   //     1
	UINT32 man_cmd_rd_stride_v_cve_y       : 1;   //     2
	UINT32 man_cmd_rd_stride_v_cve_c       : 1;   //     3
} H14A0_MIFD_RD_CMD_STRIDE_SET_T;

/*-----------------------------------------------------------------------------
	0x3e30 mifd_wd_cmd_stride0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_wd_stride_v_sm_y            : 4;   //  0: 3
	UINT32 cmd_wd_stride_v_sm_c            : 4;   //  4: 7
} H14A0_MIFD_WD_CMD_STRIDE0_T;

/*-----------------------------------------------------------------------------
	0x3e34 mifd_wd_cmd_stride_set ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 man_cmd_wd_stride_v_sm_y        : 1;   //     0
	UINT32 man_cmd_wd_stride_v_sm_c        : 1;   //     1
} H14A0_MIFD_WD_CMD_STRIDE_SET_T;

/*-----------------------------------------------------------------------------
	0x3e38 mifd_saddr_rd_ov_y_ov_c ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                 : 3;   //  0: 2 reserved
	UINT32 req_rd_saddr_ov_y               : 13;  //  3:15
	UINT32                                 : 3;   // 16:18 reserved
	UINT32 req_rd_saddr_ov_c               : 13;  // 19:31
} H14A0_MIFD_SADDR_RD_OV_Y_OV_C_T;

/*-----------------------------------------------------------------------------
	0x3e3c mifd_saddr_rd_cve_y_cve_c ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                 : 3;   //  0: 2 reserved
	UINT32 req_rd_saddr_cve_y              : 13;  //  3:15
	UINT32                                 : 3;   // 16:18 reserved
	UINT32 req_rd_saddr_cve_c              : 13;  // 19:31
} H14A0_MIFD_SADDR_RD_CVE_Y_CVE_C_T;

/*-----------------------------------------------------------------------------
	0x3e40 mifd_saddr_wd_sm_yc ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                 : 3;   //  0: 2 reserved
	UINT32 req_wd_saddr_sm_y               : 13;  //  3:15
	UINT32                                 : 3;   // 16:18 reserved
	UINT32 req_wd_saddr_sm_c               : 13;  // 19:31
} H14A0_MIFD_SADDR_WD_SM_YC_T;

/*-----------------------------------------------------------------------------
	0x3e44 mifd_saddr_rd_ext0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_saddr_ov_y_msb4b         : 4;   //  0: 3
	UINT32 req_rd_saddr_ov_c_msb4b         : 4;   //  4: 7
	UINT32 req_rd_saddr_cve_y_msb4b        : 4;   //  8:11
	UINT32 req_rd_saddr_cve_c_msb4b        : 4;   // 12:15
} H14A0_MIFD_SADDR_RD_EXT0_T;

/*-----------------------------------------------------------------------------
	0x3e48 mifd_saddr_wd_ext0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_wd_saddr_sm_y_msb4b         : 4;   //  0: 3
	UINT32 req_wd_saddr_sm_c_msb4b         : 4;   //  4: 7
} H14A0_MIFD_SADDR_WD_EXT0_T;

/*-----------------------------------------------------------------------------
	0x3e4c mifd_status ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 fr_rd_valid_ov_y                : 1;   //     0
	UINT32 fr_rd_valid_ov_c                : 1;   //     1
	UINT32 fr_rd_valid_cve_y               : 1;   //     2
	UINT32 fr_rd_valid_cve_c               : 1;   //     3
	UINT32 fr_wd_q_full_sm_y               : 1;   //     4
	UINT32 fr_wd_q_full_sm_c               : 1;   //     5
} H14A0_MIFD_STATUS_T;

/*-----------------------------------------------------------------------------
	0x3e50 mifd_saddr_param_update0_m ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 load                            : 1;   //     0
	UINT32                                 : 10;  //  1:10 reserved
	UINT32 register_update_address0_m      : 21;  // 11:31
} H14A0_MIFD_SADDR_PARAM_UPDATE0_M_T;

/*-----------------------------------------------------------------------------
	0x3e54 mifd_rd_bank_8_addr0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_bank_split_8_ov_y        : 1;   //     0
	UINT32 req_rd_bank_addr_8_ov_y         : 3;   //  1: 3
	UINT32 req_rd_bank_split_8_ov_c        : 1;   //     4
	UINT32 req_rd_bank_addr_8_ov_c         : 3;   //  5: 7
	UINT32 req_rd_bank_split_cve_y         : 1;   //     8
	UINT32 req_rd_bank_addr_cve_y          : 3;   //  9:11
	UINT32 req_rd_bank_split_cve_c         : 1;   //    12
	UINT32 req_rd_bank_addr_cve_c          : 3;   // 13:15
} H14A0_MIFD_RD_BANK_8_ADDR0_T;

/*-----------------------------------------------------------------------------
	0x3e58 mifd_rd_bank_8_addr1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 mifd_rd_bank_8_addr1_f          : 32;   // 31: 0
} H14A0_MIFD_RD_BANK_8_ADDR1_T;

/*-----------------------------------------------------------------------------
	0x3e5c mifd_wd_bank_8_addr0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_wd_bank_split_8_sm_y        : 1;   //     0
	UINT32 req_wd_bank_addr_8_sm_y         : 3;   //  1: 3
	UINT32 req_wd_bank_split_8_sm_c        : 1;   //     4
	UINT32 req_wd_bank_addr_8_sm_c         : 3;   //  5: 7
} H14A0_MIFD_WD_BANK_8_ADDR0_T;

/*-----------------------------------------------------------------------------
	0x3e60 mifd_wd_bank_8_addr1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 mifd_wd_bank_8_addr1_f          : 32;   // 31: 0
} H14A0_MIFD_WD_BANK_8_ADDR1_T;

/*-----------------------------------------------------------------------------
	0x3e64 mifd_rd_req_pend0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_pend_v_ov_y              : 8;   //  0: 7
	UINT32 req_rd_pend_v_ov_c              : 8;   //  8:15
} H14A0_MIFD_RD_REQ_PEND0_T;

/*-----------------------------------------------------------------------------
	0x3e68 mifd_wd_req_pend0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_wd_pend_v_sm_y              : 8;   //  0: 7
	UINT32 req_wd_pend_v_sm_c              : 8;   //  8:15
} H14A0_MIFD_WD_REQ_PEND0_T;

/*-----------------------------------------------------------------------------
	0x3e6c mifd_rd_cmd_pri0_s ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_pri_s_v_ov_y             : 4;   //  0: 3
	UINT32 cmd_rd_pri_s_v_ov_c             : 4;   //  4: 7
	UINT32 cmd_rd_pri_s_v_cve_y            : 4;   //  8:11
	UINT32 cmd_rd_pri_s_v_cve_c            : 4;   // 12:15
} H14A0_MIFD_RD_CMD_PRI0_S_T;

/*-----------------------------------------------------------------------------
	0x3e70 mifd_wd_cmd_pri0_s ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_wd_pri_s_v_sm_y             : 4;   //  0: 3
	UINT32 cmd_wd_pri_s_v_sm_c             : 4;   //  4: 7
} H14A0_MIFD_WD_CMD_PRI0_S_T;

/*-----------------------------------------------------------------------------
	0x3e74 mifd_reset ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 reset_mif_r_ov_y                : 1;   //     0
	UINT32 reset_mif_r_ov_c                : 1;   //     1
	UINT32 reset_mif_r_cve_y               : 1;   //     2
	UINT32 reset_mif_r_cve_c               : 1;   //     3
	UINT32 reset_mif_w_sm_y                : 1;   //     4
	UINT32 reset_mif_w_sm_c                : 1;   //     5
	UINT32                                 : 14;  //  6:19 reserved
	UINT32 reset_time_out_cnt              : 12;  // 20:31
} H14A0_MIFD_RESET_T;

/*-----------------------------------------------------------------------------
	0x3e78 mifd_rd_set_10b ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 set_10b_req_rd_ov_y             : 1;   //     0
	UINT32 set_10b_req_rd_ov_c             : 1;   //     1
} H14A0_MIFD_RD_SET_10B_T;

/*-----------------------------------------------------------------------------
	0x3e7c mifd_wd_set_10b ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 set_10b_req_wd_sm_y             : 1;   //     0
	UINT32 set_10b_req_wd_sm_c             : 1;   //     1
} H14A0_MIFD_WD_SET_10B_T;

/*-----------------------------------------------------------------------------
	0x3e80 mifd_rd_cmd_2_stride0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_stride_v_2_ov_y          : 4;   //  0: 3
	UINT32 cmd_rd_stride_v_2_ov_c          : 4;   //  4: 7
} H14A0_MIFD_RD_CMD_2_STRIDE0_T;

/*-----------------------------------------------------------------------------
	0x3e84 mifd_wd_cmd_2_stride0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_wd_stride_v_2_sm_y          : 4;   //  0: 3
	UINT32 cmd_wd_stride_v_2_sm_c          : 4;   //  4: 7
} H14A0_MIFD_WD_CMD_2_STRIDE0_T;

/*-----------------------------------------------------------------------------
	0x3e88 mifd_saddr_rd_ov_y_ov_c_2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                 : 3;   //  0: 2 reserved
	UINT32 req_rd_saddr_2_ov_y             : 13;  //  3:15
	UINT32                                 : 3;   // 16:18 reserved
	UINT32 req_rd_saddr_2_ov_c             : 13;  // 19:31
} H14A0_MIFD_SADDR_RD_OV_Y_OV_C_2_T;

/*-----------------------------------------------------------------------------
	0x3e8c mifd_saddr_wd_sm_yc_2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                 : 3;   //  0: 2 reserved
	UINT32 req_wd_saddr_2_sm_y             : 13;  //  3:15
	UINT32                                 : 3;   // 16:18 reserved
	UINT32 req_wd_saddr_2_sm_c             : 13;  // 19:31
} H14A0_MIFD_SADDR_WD_SM_YC_2_T;

/*-----------------------------------------------------------------------------
	0x3e90 mifd_saddr_rd_2_ext0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_saddr_2_ov_y_msb4b       : 4;   //  0: 3
	UINT32 req_rd_saddr_2_ov_c_msb4b       : 4;   //  4: 7
} H14A0_MIFD_SADDR_RD_2_EXT0_T;

/*-----------------------------------------------------------------------------
	0x3e94 mifd_saddr_wd_2_ext0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_wd_saddr_2_sm_y_msb4b       : 4;   //  0: 3
	UINT32 req_wd_saddr_2_sm_c_msb4b       : 4;   //  4: 7
} H14A0_MIFD_SADDR_WD_2_EXT0_T;

/*-----------------------------------------------------------------------------
	0x3e98 mifd_rd_bank_2_addr0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_bank_split_2_ov_y        : 1;   //     0
	UINT32 req_rd_bank_addr_2_ov_y         : 3;   //  1: 3
	UINT32 req_rd_bank_split_2_ov_c        : 1;   //     4
	UINT32 req_rd_bank_addr_2_ov_c         : 3;   //  5: 7
} H14A0_MIFD_RD_BANK_2_ADDR0_T;

/*-----------------------------------------------------------------------------
	0x3e9c mifd_rd_bank_2_addr1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 mifd_rd_bank_2_addr1_f          : 32;   // 31: 0
} H14A0_MIFD_RD_BANK_2_ADDR1_T;

/*-----------------------------------------------------------------------------
	0x3ea0 mifd_wd_bank_2_addr0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_wd_bank_split_2_sm_y        : 1;   //     0
	UINT32 req_wd_bank_addr_2_sm_y         : 3;   //  1: 3
	UINT32 req_wd_bank_split_2_sm_c        : 1;   //     4
	UINT32 req_wd_bank_addr_2_sm_c         : 3;   //  5: 7
} H14A0_MIFD_WD_BANK_2_ADDR0_T;

/*-----------------------------------------------------------------------------
	0x3ea4 mifd_wd_bank_2_addr1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 mifd_wd_bank_2_addr1_f          : 32;   // 31: 0
} H14A0_MIFD_WD_BANK_2_ADDR1_T;

/*-----------------------------------------------------------------------------
	0x3ea8 mifd_lsb_switch ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_lsb_ov_y                 : 2;   //  0: 1
	UINT32 req_rd_lsb_ov_c                 : 2;   //  2: 3
} H14A0_MIFD_LSB_SWITCH_T;

/*-----------------------------------------------------------------------------
	0x5900 mifo_rd_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                 : 12;  //  0:11 reserved
	UINT32 disable_rd_mif                  : 1;   //    12
	UINT32 load_rd_req_stv_v               : 1;   //    13
	UINT32 load_rd_req_pri_v               : 1;   //    14
	UINT32 load_rd_cmd_saddr_v             : 1;   //    15
	UINT32 load_rd_cmd_pend_v              : 1;   //    16
	UINT32 load_rd_cmd_pri_v               : 1;   //    17
	UINT32                                 : 1;   //    18 reserved
	UINT32 enable_rd_stv_cnt               : 1;   //    19
	UINT32                                 : 1;   //    20 reserved
	UINT32 mrb_bank_mode                   : 3;   // 21:23
} H14A0_MIFO_RD_CTRL_T;

/*-----------------------------------------------------------------------------
	0x5904 mifo_rd_cmd_pri0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_pri_v_osd_0              : 4;   //  0: 3
	UINT32 cmd_rd_pri_v_osd_1              : 4;   //  4: 7
	UINT32                                 : 4;   //  8:11 reserved
	UINT32 cmd_rd_pri_v_osd_3              : 4;   // 12:15
} H14A0_MIFO_RD_CMD_PRI0_T;

/*-----------------------------------------------------------------------------
	0x5908 mifo_rd_req_pri0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_pri_v_osd_0              : 4;   //  0: 3
	UINT32 req_rd_pri_v_osd_1              : 4;   //  4: 7
	UINT32                                 : 4;   //  8:11 reserved
	UINT32 req_rd_pri_v_osd_3              : 4;   // 12:15
} H14A0_MIFO_RD_REQ_PRI0_T;

/*-----------------------------------------------------------------------------
	0x590c mifo_rd_req_stv0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_stv_v_osd_0              : 4;   //  0: 3
	UINT32 req_rd_stv_v_osd_1              : 4;   //  4: 7
	UINT32                                 : 4;   //  8:11 reserved
	UINT32 req_rd_stv_v_osd_3              : 4;   // 12:15
} H14A0_MIFO_RD_REQ_STV0_T;

/*-----------------------------------------------------------------------------
	0x5910 mifo_rd_cmd_pend0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_pend_v_osd_0             : 4;   //  0: 3
	UINT32 cmd_rd_pend_v_osd_1             : 4;   //  4: 7
	UINT32                                 : 4;   //  8:11 reserved
	UINT32 cmd_rd_pend_v_osd_3             : 4;   // 12:15
} H14A0_MIFO_RD_CMD_PEND0_T;

/*-----------------------------------------------------------------------------
	0x5914 mifo_saddr_osd_0_l ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_saddr_osd_0_l            : 32;  //  0:31
} H14A0_MIFO_SADDR_OSD_0_L_T;

/*-----------------------------------------------------------------------------
	0x5918 mifo_saddr_osd_0_r ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_saddr_osd_0_r            : 32;  //  0:31
} H14A0_MIFO_SADDR_OSD_0_R_T;

/*-----------------------------------------------------------------------------
	0x591c mifo_saddr_osd_1_l ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_saddr_osd_1_l            : 32;  //  0:31
} H14A0_MIFO_SADDR_OSD_1_L_T;

/*-----------------------------------------------------------------------------
	0x5920 mifo_saddr_osd_1_r ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_saddr_osd_1_r            : 32;  //  0:31
} H14A0_MIFO_SADDR_OSD_1_R_T;

/*-----------------------------------------------------------------------------
	0x5924 mifo_saddr_osd_3_l ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_saddr_osd_3_l            : 32;  //  0:31
} H14A0_MIFO_SADDR_OSD_3_L_T;

/*-----------------------------------------------------------------------------
	0x5928 mifo_saddr_osd_3_r ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_saddr_osd_3_r            : 32;  //  0:31
} H14A0_MIFO_SADDR_OSD_3_R_T;

/*-----------------------------------------------------------------------------
	0x592c mifo_status ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 fr_rd_valid_osd_0               : 1;   //     0
	UINT32 fr_rd_valid_osd_1               : 1;   //     1
	UINT32                                 : 1;   //     2 reserved
	UINT32 fr_rd_valid_osd_3               : 1;   //     3
} H14A0_MIFO_STATUS_T;

/*-----------------------------------------------------------------------------
	0x5930 mifo_rd_req_pend0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_pend_v_osd_0             : 8;   //  0: 7
	UINT32 req_rd_pend_v_osd_1             : 8;   //  8:15
	UINT32                                 : 8;   // 16:23 reserved
	UINT32 req_rd_pend_v_osd_3             : 8;   // 24:31
} H14A0_MIFO_RD_REQ_PEND0_T;

/*-----------------------------------------------------------------------------
	0x5934 mifo_rd_cmd_pri0_s ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_pri_s_v_osd_0            : 4;   //  0: 3
	UINT32 cmd_rd_pri_s_v_osd_1            : 4;   //  4: 7
	UINT32                                 : 4;   //  8:11 reserved
	UINT32 cmd_rd_pri_s_v_osd_3            : 4;   // 12:15
} H14A0_MIFO_RD_CMD_PRI0_S_T;

/*-----------------------------------------------------------------------------
	0x5938 mifo_reset ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 reset_mif_r_osd_0               : 1;   //     0
	UINT32 reset_mif_r_osd_1               : 1;   //     1
	UINT32                                 : 1;   //     2 reserved
	UINT32 reset_mif_r_osd_3               : 1;   //     3
	UINT32                                 : 16;  //  4:19 reserved
	UINT32 reset_time_out_cnt              : 12;  // 20:31
} H14A0_MIFO_RESET_T;

typedef struct {
	H14A0_MIFA_RD_CTRL_T                    mifa_rd_ctrl;                            // 0x0e00
	H14A0_MIFA_WD_CTRL_T                    mifa_wd_ctrl;                            // 0x0e04
	H14A0_MIFA_RD_SET_10B_T                 mifa_rd_set_10b;                         // 0x0e08
	H14A0_MIFA_WD_SET_10B_T                 mifa_wd_set_10b;                         // 0x0e0c
	H14A0_MIFA_RD_CMD_PRI0_T                mifa_rd_cmd_pri0;                        // 0x0e10
	H14A0_MIFA_RD_CMD_PRI1_T                mifa_rd_cmd_pri1;                        // 0x0e14
	H14A0_MIFA_WD_CMD_PRI0_T                mifa_wd_cmd_pri0;                        // 0x0e18
	H14A0_MIFA_RD_REQ_PRI0_T                mifa_rd_req_pri0;                        // 0x0e1c
	H14A0_MIFA_RD_REQ_PRI1_T                mifa_rd_req_pri1;                        // 0x0e20
	H14A0_MIFA_WD_REQ_PRI0_T                mifa_wd_req_pri0;                        // 0x0e24
	H14A0_MIFA_RD_REQ_STV0_T                mifa_rd_req_stv0;                        // 0x0e28
	H14A0_MIFA_RD_REQ_STV1_T                mifa_rd_req_stv1;                        // 0x0e2c
	H14A0_MIFA_WD_REQ_STV0_T                mifa_wd_req_stv0;                        // 0x0e30
	H14A0_MIFA_RD_CMD_PEND0_T               mifa_rd_cmd_pend0;                       // 0x0e34
	H14A0_MIFA_RD_CMD_PEND1_T               mifa_rd_cmd_pend1;                       // 0x0e38
	H14A0_MIFA_WD_CMD_PEND0_T               mifa_wd_cmd_pend0;                       // 0x0e3c
	H14A0_MIFA_RD_CMD_STRIDE0_T             mifa_rd_cmd_stride0;                     // 0x0e40
	H14A0_MIFA_RD_CMD_STRIDE1_T             mifa_rd_cmd_stride1;                     // 0x0e44
	H14A0_MIFA_RD_CMD_STRIDE2_T             mifa_rd_cmd_stride2;                     // 0x0e48
	H14A0_MIFA_RD_CMD_STRIDE_SET_T          mifa_rd_cmd_stride_set;                  // 0x0e4c
	H14A0_MIFA_WD_CMD_STRIDE0_T             mifa_wd_cmd_stride0;                     // 0x0e50
	H14A0_MIFA_WD_CMD_STRIDE1_T             mifa_wd_cmd_stride1;                     // 0x0e54
	H14A0_MIFA_WD_CMD_STRIDE_SET_T          mifa_wd_cmd_stride_set;                  // 0x0e58
	H14A0_MIFA_SADDR_RD_MVI_Y_MVI_C_T       mifa_saddr_rd_mvi_y_mvi_c;               // 0x0e5c
	H14A0_MIFA_SADDR_RD_PREW_Y_T            mifa_saddr_rd_prew_y;                    // 0x0e60
	H14A0_MIFA_SADDR_RD_PREW_C_T            mifa_saddr_rd_prew_c;                    // 0x0e64
	H14A0_MIFA_SADDR_RD_IPC0_Y_T            mifa_saddr_rd_ipc0_y;                    // 0x0e68
	H14A0_MIFA_SADDR_RD_IPC0_C_T            mifa_saddr_rd_ipc0_c;                    // 0x0e6c
	H14A0_MIFA_SADDR_RD_IPC1_Y_T            mifa_saddr_rd_ipc1_y;                    // 0x0e70
	H14A0_MIFA_SADDR_RD_TNR0_Y_T            mifa_saddr_rd_tnr0_y;                    // 0x0e74
	H14A0_MIFA_SADDR_RD_TNR_C_T             mifa_saddr_rd_tnr_c;                     // 0x0e78
	H14A0_MIFA_SADDR_RD_TNR1_Y_T            mifa_saddr_rd_tnr1_y;                    // 0x0e7c
	H14A0_MIFA_SADDR_RD_IPC_M_T             mifa_saddr_rd_ipc_m;                     // 0x0e80
	H14A0_MIFA_SADDR_WD_TNR_Y_T             mifa_saddr_wd_tnr_y;                     // 0x0e84
	H14A0_MIFA_SADDR_WD_TNR_C_T             mifa_saddr_wd_tnr_c;                     // 0x0e88
	H14A0_MIFA_SADDR_WD_PREW_Y_T            mifa_saddr_wd_prew_y;                    // 0x0e8c
	H14A0_MIFA_SADDR_WD_PREW_C_T            mifa_saddr_wd_prew_c;                    // 0x0e90
	H14A0_MIFA_SADDR_WD_TNR_M_T             mifa_saddr_wd_tnr_m;                     // 0x0e94
	H14A0_MIFA_SADDR_RD_DL_Y_T              mifa_saddr_rd_dl_y;                      // 0x0e98
	H14A0_MIFA_SADDR_RD_DL_C_T              mifa_saddr_rd_dl_c;                      // 0x0e9c
	H14A0_MIFA_SADDR_RD_DR_Y_T              mifa_saddr_rd_dr_y;                      // 0x0ea0
	H14A0_MIFA_SADDR_RD_DR_C_T              mifa_saddr_rd_dr_c;                      // 0x0ea4
	H14A0_MIFA_SADDR_WD_DL_Y_T              mifa_saddr_wd_dl_y;                      // 0x0ea8
	H14A0_MIFA_SADDR_WD_DL_C_T              mifa_saddr_wd_dl_c;                      // 0x0eac
	H14A0_MIFA_SADDR_WD_DR_Y_T              mifa_saddr_wd_dr_y;                      // 0x0eb0
	H14A0_MIFA_SADDR_WD_DR_C_T              mifa_saddr_wd_dr_c;                      // 0x0eb4
	H14A0_MIFA_SADDR_RD_EXT0_T              mifa_saddr_rd_ext0;                      // 0x0eb8
	H14A0_MIFA_SADDR_RD_EXT1_T              mifa_saddr_rd_ext1;                      // 0x0ebc
	H14A0_MIFA_SADDR_RD_EXT2_T              mifa_saddr_rd_ext2;                      // 0x0ec0
	H14A0_MIFA_SADDR_WD_EXT0_T              mifa_saddr_wd_ext0;                      // 0x0ec4
	H14A0_MIFA_SADDR_WD_EXT1_T              mifa_saddr_wd_ext1;                      // 0x0ec8
	H14A0_MIFA_RD_DR_DL_EXT_T               mifa_rd_dr_dl_ext;                       // 0x0ecc
	H14A0_MIFA_WD_DR_DL_EXT_T               mifa_wd_dr_dl_ext;                       // 0x0ed0
	H14A0_MIFA_STATUS_T                     mifa_status;                             // 0x0ed4
	H14A0_MIFA_SADDR_PARAM_UPDATE0_M_T      mifa_saddr_param_update0_m;              // 0x0ed8
	H14A0_MIFA_SADDR_PARAM_UPDATE0_M1_T     mifa_saddr_param_update0_m1;             // 0x0edc
	H14A0_MIFA_RD_BANK_8_ADDR0_T            mifa_rd_bank_8_addr0;                    // 0x0ee0
	H14A0_MIFA_RD_BANK_8_ADDR1_T            mifa_rd_bank_8_addr1;                    // 0x0ee4
	H14A0_MIFA_RD_BANK_2_ADDR0_T            mifa_rd_bank_2_addr0;                    // 0x0ee8
	H14A0_MIFA_RD_BANK_2_ADDR1_T            mifa_rd_bank_2_addr1;                    // 0x0eec
	H14A0_MIFA_WD_BANK_8_ADDR0_T            mifa_wd_bank_8_addr0;                    // 0x0ef0
	H14A0_MIFA_WD_BANK_8_ADDR1_T            mifa_wd_bank_8_addr1;                    // 0x0ef4
	H14A0_MIFA_WD_BANK_2_ADDR0_T            mifa_wd_bank_2_addr0;                    // 0x0ef8
	H14A0_MIFA_WD_BANK_2_ADDR1_T            mifa_wd_bank_2_addr1;                    // 0x0efc
	H14A0_MIFA_LSB_SWITCH_T                 mifa_lsb_switch;                         // 0x0f00
	H14A0_MIFA_RD_REQ_PEND0_T               mifa_rd_req_pend0;                       // 0x0f04
	H14A0_MIFA_RD_REQ_PEND1_T               mifa_rd_req_pend1;                       // 0x0f08
	H14A0_MIFA_RD_REQ_PEND2_T               mifa_rd_req_pend2;                       // 0x0f0c
	H14A0_MIFA_WD_REQ_PEND0_T               mifa_wd_req_pend0;                       // 0x0f10
	H14A0_MIFA_WD_REQ_PEND1_T               mifa_wd_req_pend1;                       // 0x0f14
	H14A0_MIFA_RD_CMD_PRI0_S_T              mifa_rd_cmd_pri0_s;                      // 0x0f18
	H14A0_MIFA_RD_CMD_PRI1_S_T              mifa_rd_cmd_pri1_s;                      // 0x0f1c
	H14A0_MIFA_WD_CMD_PRI0_S_T              mifa_wd_cmd_pri0_s;                      // 0x0f20
	H14A0_MIFA_RESET_T                      mifa_reset;                              // 0x0f24
	H14A0_MIFA_SADDR_RD_IPC1_C_T            mifa_saddr_rd_ipc1_c;                    // 0x0f28
	H14A0_MIFA_MVI_R_STRIDE_T               mifa_mvi_r_stride;                       // 0x0f2c
} DE_MIA_REG_H14A0_T;

typedef struct {
	H14A0_MIFB_RD_CTRL_T                    mifb_rd_ctrl;                            // 0x1e00
	H14A0_MIFB_WD_CTRL_T                    mifb_wd_ctrl;                            // 0x1e04
	H14A0_MIFB_RD_SET_10B_T                 mifb_rd_set_10b;                         // 0x1e08
	H14A0_MIFB_WD_SET_10B_T                 mifb_wd_set_10b;                         // 0x1e0c
	H14A0_MIFB_RD_CMD_PRI0_T                mifb_rd_cmd_pri0;                        // 0x1e10
	H14A0_MIFB_WD_CMD_PRI0_T                mifb_wd_cmd_pri0;                        // 0x1e14
	H14A0_MIFB_RD_REQ_PRI0_T                mifb_rd_req_pri0;                        // 0x1e18
	H14A0_MIFB_WD_REQ_PRI0_T                mifb_wd_req_pri0;                        // 0x1e1c
	H14A0_MIFB_RD_REQ_STV0_T                mifb_rd_req_stv0;                        // 0x1e20
	H14A0_MIFB_WD_REQ_STV0_T                mifb_wd_req_stv0;                        // 0x1e24
	H14A0_MIFB_RD_CMD_PEND0_T               mifb_rd_cmd_pend0;                       // 0x1e28
	H14A0_MIFB_WD_CMD_PEND0_T               mifb_wd_cmd_pend0;                       // 0x1e2c
	H14A0_MIFB_RD_CMD_STRIDE0_T             mifb_rd_cmd_stride0;                     // 0x1e30
	H14A0_MIFB_RD_CMD_STRIDE_SET_T          mifb_rd_cmd_stride_set;                  // 0x1e34
	H14A0_MIFB_WD_CMD_STRIDE0_T             mifb_wd_cmd_stride0;                     // 0x1e38
	H14A0_MIFB_WD_CMD_STRIDE_SET_T          mifb_wd_cmd_stride_set;                  // 0x1e3c
	H14A0_MIFB_SADDR_RD_OV_Y_T              mifb_saddr_rd_ov_y;                      // 0x1e40
	H14A0_MIFB_SADDR_RD_OV_C_T              mifb_saddr_rd_ov_c;                      // 0x1e44
	H14A0_MIFB_SADDR_WD_SM_Y_T              mifb_saddr_wd_sm_y;                      // 0x1e48
	H14A0_MIFB_SADDR_WD_SM_C_T              mifb_saddr_wd_sm_c;                      // 0x1e4c
	H14A0_MIFB_SADDR_RD_EXT0_T              mifb_saddr_rd_ext0;                      // 0x1e50
	H14A0_MIFB_SADDR_WD_EXT0_T              mifb_saddr_wd_ext0;                      // 0x1e54
	H14A0_MIFB_STATUS_T                     mifb_status;                             // 0x1e58
	H14A0_MIFB_RD_BANK_8_ADDR0_T            mifb_rd_bank_8_addr0;                    // 0x1e5c
	H14A0_MIFB_RD_BANK_8_ADDR1_T            mifb_rd_bank_8_addr1;                    // 0x1e60
	H14A0_MIFB_RD_BANK_2_ADDR0_T            mifb_rd_bank_2_addr0;                    // 0x1e64
	H14A0_MIFB_RD_BANK_2_ADDR1_T            mifb_rd_bank_2_addr1;                    // 0x1e68
	H14A0_MIFB_WD_BANK_8_ADDR0_T            mifb_wd_bank_8_addr0;                    // 0x1e6c
	H14A0_MIFB_WD_BANK_8_ADDR1_T            mifb_wd_bank_8_addr1;                    // 0x1e70
	H14A0_MIFB_WD_BANK_2_ADDR0_T            mifb_wd_bank_2_addr0;                    // 0x1e74
	H14A0_MIFB_WD_BANK_2_ADDR1_T            mifb_wd_bank_2_addr1;                    // 0x1e78
	H14A0_MIFB_LSB_SWITCH_T                 mifb_lsb_switch;                         // 0x1e7c
	H14A0_MIFB_RD_REQ_PEND0_T               mifb_rd_req_pend0;                       // 0x1e80
	H14A0_MIFB_WD_REQ_PEND0_T               mifb_wd_req_pend0;                       // 0x1e84
	H14A0_MIFB_RD_CMD_PRI0_S_T              mifb_rd_cmd_pri0_s;                      // 0x1e88
	H14A0_MIFB_WD_CMD_PRI0_S_T              mifb_wd_cmd_pri0_s;                      // 0x1e8c
	H14A0_MIFB_RESET_T                      mifb_reset;                              // 0x1e90
	H14A0_MIFB_SADDR_RD_OVS_Y_OVS_C_T       mifb_saddr_rd_ovs_y_ovs_c;               // 0x1e94
} DE_MIB_REG_H14A0_T;

typedef struct {
	H14A0_MIFC_RD_CTRL_T                    mifc_rd_ctrl;                            // 0x2e00
	H14A0_MIFC_WD_CTRL_T                    mifc_wd_ctrl;                            // 0x2e04
	H14A0_MIFC_RD_CMD_PRI0_T                mifc_rd_cmd_pri0;                        // 0x2e08
	H14A0_MIFC_RD_CMD_PRI1_T                mifc_rd_cmd_pri1;                        // 0x2e0c
	H14A0_MIFC_WD_CMD_PRI0_T                mifc_wd_cmd_pri0;                        // 0x2e10
	H14A0_MIFC_RD_REQ_PRI0_T                mifc_rd_req_pri0;                        // 0x2e14
	H14A0_MIFC_RD_REQ_PRI1_T                mifc_rd_req_pri1;                        // 0x2e18
	H14A0_MIFC_WD_REQ_PRI0_T                mifc_wd_req_pri0;                        // 0x2e1c
	H14A0_MIFC_RD_REQ_STV0_T                mifc_rd_req_stv0;                        // 0x2e20
	H14A0_MIFC_RD_REQ_STV1_T                mifc_rd_req_stv1;                        // 0x2e24
	H14A0_MIFC_WD_REQ_STV0_T                mifc_wd_req_stv0;                        // 0x2e28
	H14A0_MIFC_RD_CMD_PEND0_T               mifc_rd_cmd_pend0;                       // 0x2e2c
	H14A0_MIFC_RD_CMD_PEND1_T               mifc_rd_cmd_pend1;                       // 0x2e30
	H14A0_MIFC_WD_CMD_PEND0_T               mifc_wd_cmd_pend0;                       // 0x2e34
	H14A0_MIFC_RD_CMD_STRIDE0_T             mifc_rd_cmd_stride0;                     // 0x2e38
	H14A0_MIFC_RD_CMD_STRIDE1_T             mifc_rd_cmd_stride1;                     // 0x2e3c
	H14A0_MIFC_RD_CMD_STRIDE_SET_T          mifc_rd_cmd_stride_set;                  // 0x2e40
	H14A0_MIFC_WD_CMD_STRIDE0_T             mifc_wd_cmd_stride0;                     // 0x2e44
	H14A0_MIFC_WD_CMD_STRIDE_SET_T          mifc_wd_cmd_stride_set;                  // 0x2e48
	H14A0_MIFC_SADDR_RD_MVI_Y_MVI_C_T       mifc_saddr_rd_mvi_y_mvi_c;               // 0x2e4c
	H14A0_MIFC_SADDR_RD_PREW_Y_PREW_C_T     mifc_saddr_rd_prew_y_prew_c;             // 0x2e50
	H14A0_MIFC_SADDR_RD_IPC0_Y_IPC0_C_T     mifc_saddr_rd_ipc0_y_ipc0_c;             // 0x2e54
	H14A0_MIFC_SADDR_RD_IPC1_Y_TNR0_Y_T     mifc_saddr_rd_ipc1_y_tnr0_y;             // 0x2e58
	H14A0_MIFC_SADDR_RD_SS_Y_SS_C_T         mifc_saddr_rd_ss_y_ss_c;                 // 0x2e5c
	H14A0_MIFC_SADDR_RD_TNR_C_IPC1_C_T      mifc_saddr_rd_tnr_c_ipc1_c;              // 0x2e60
	H14A0_MIFC_SADDR_WD_SS_Y_SS_C_T         mifc_saddr_wd_ss_y_ss_c;                 // 0x2e64
	H14A0_MIFC_SADDR_WD_TNR_Y_TNR_C_T       mifc_saddr_wd_tnr_y_tnr_c;               // 0x2e68
	H14A0_MIFC_SADDR_WD_PREW_Y_PREW_C_T     mifc_saddr_wd_prew_y_prew_c;             // 0x2e6c
	H14A0_MIFC_SADDR_RD_DL_Y_DL_C_T         mifc_saddr_rd_dl_y_dl_c;                 // 0x2e70
	H14A0_MIFC_SADDR_RD_DR_Y_DR_C_T         mifc_saddr_rd_dr_y_dr_c;                 // 0x2e74
	H14A0_MIFC_SADDR_WD_DL_Y_DL_C_T         mifc_saddr_wd_dl_y_dl_c;                 // 0x2e78
	H14A0_MIFC_SADDR_WD_DR_Y_DR_C_T         mifc_saddr_wd_dr_y_dr_c;                 // 0x2e7c
	H14A0_MIFC_SADDR_RD_EXT0_T              mifc_saddr_rd_ext0;                      // 0x2e80
	H14A0_MIFC_SADDR_RD_EXT1_T              mifc_saddr_rd_ext1;                      // 0x2e84
	H14A0_MIFC_SADDR_WD_EXT0_T              mifc_saddr_wd_ext0;                      // 0x2e88
	H14A0_MIFC_RD_DR_DL_EXT_T               mifc_rd_dr_dl_ext;                       // 0x2e8c
	H14A0_MIFC_WD_DR_DL_EXT_T               mifc_wd_dr_dl_ext;                       // 0x2e90
	H14A0_MIFC_STATUS_T                     mifc_status;                             // 0x2e94
	H14A0_MIFC_SADDR_PARAM_UPDATE0_M_T      mifc_saddr_param_update0_m;              // 0x2e98
	H14A0_MIFC_RD_BANK_8_ADDR0_T            mifc_rd_bank_8_addr0;                    // 0x2e9c
	H14A0_MIFC_RD_BANK_8_ADDR1_T            mifc_rd_bank_8_addr1;                    // 0x2ea0
	H14A0_MIFC_WD_BANK_8_ADDR0_T            mifc_wd_bank_8_addr0;                    // 0x2ea4
	H14A0_MIFC_WD_BANK_8_ADDR1_T            mifc_wd_bank_8_addr1;                    // 0x2ea8
	H14A0_MIFC_RD_REQ_PEND0_T               mifc_rd_req_pend0;                       // 0x2eac
	H14A0_MIFC_RD_REQ_PEND1_T               mifc_rd_req_pend1;                       // 0x2eb0
	H14A0_MIFC_RD_REQ_PEND2_T               mifc_rd_req_pend2;                       // 0x2eb4
	H14A0_MIFC_WD_REQ_PEND0_T               mifc_wd_req_pend0;                       // 0x2eb8
	H14A0_MIFC_WD_REQ_PEND1_T               mifc_wd_req_pend1;                       // 0x2ebc
	H14A0_MIFC_RD_CMD_PRI0_S_T              mifc_rd_cmd_pri0_s;                      // 0x2ec0
	H14A0_MIFC_RD_CMD_PRI1_S_T              mifc_rd_cmd_pri1_s;                      // 0x2ec4
	H14A0_MIFC_WD_CMD_PRI0_S_T              mifc_wd_cmd_pri0_s;                      // 0x2ec8
	H14A0_MIFC_RESET_T                      mifc_reset;                              // 0x2ecc
	H14A0_MIFC_RD_SET_10B_T                 mifc_rd_set_10b;                         // 0x2ed0
	H14A0_MIFC_WD_SET_10B_T                 mifc_wd_set_10b;                         // 0x2ed4
	H14A0_MIFC_RD_CMD_2_STRIDE0_T           mifc_rd_cmd_2_stride0;                   // 0x2ed8
	H14A0_MIFC_WD_CMD_2_STRIDE0_T           mifc_wd_cmd_2_stride0;                   // 0x2edc
	H14A0_MIFC_SADDR_RD_PREW_Y_PREW_C_2_T   mifc_saddr_rd_prew_y_prew_c_2;           // 0x2ee0
	H14A0_MIFC_SADDR_RD_IPC0_Y_IPC0_C_2_T   mifc_saddr_rd_ipc0_y_ipc0_c_2;           // 0x2ee4
	H14A0_MIFC_SADDR_RD_IPC1_Y_TNR0_Y_2_T   mifc_saddr_rd_ipc1_y_tnr0_y_2;           // 0x2ee8
	H14A0_MIFC_SADDR_RD_TNR_C_IPC1_C_2_T    mifc_saddr_rd_tnr_c_ipc1_c_2;            // 0x2eec
	H14A0_MIFC_SADDR_WD_TNR_Y_TNR_C_2_T     mifc_saddr_wd_tnr_y_tnr_c_2;             // 0x2ef0
	H14A0_MIFC_SADDR_WD_PREW_Y_PREW_C_2_T   mifc_saddr_wd_prew_y_prew_c_2;           // 0x2ef4
	H14A0_MIFC_SADDR_RD_2_EXT0_T            mifc_saddr_rd_2_ext0;                    // 0x2ef8
	H14A0_MIFC_SADDR_WD_2_EXT0_T            mifc_saddr_wd_2_ext0;                    // 0x2efc
	H14A0_MIFC_RD_BANK_2_ADDR0_T            mifc_rd_bank_2_addr0;                    // 0x2f00
	H14A0_MIFC_RD_BANK_2_ADDR1_T            mifc_rd_bank_2_addr1;                    // 0x2f04
	H14A0_MIFC_WD_BANK_2_ADDR0_T            mifc_wd_bank_2_addr0;                    // 0x2f08
	H14A0_MIFC_WD_BANK_2_ADDR1_T            mifc_wd_bank_2_addr1;                    // 0x2f0c
	H14A0_MIFC_LSB_SWITCH_T                 mifc_lsb_switch;                         // 0x2f10
	H14A0_MIFC_MVI_R_STRIDE_T               mifc_mvi_r_stride;                       // 0x2f14
} DE_MIC_REG_H14A0_T;

typedef struct {
	H14A0_MIFD_RD_CTRL_T                    mifd_rd_ctrl;                            // 0x3e00
	H14A0_MIFD_WD_CTRL_T                    mifd_wd_ctrl;                            // 0x3e04
	H14A0_MIFD_RD_CMD_PRI0_T                mifd_rd_cmd_pri0;                        // 0x3e08
	H14A0_MIFD_WD_CMD_PRI0_T                mifd_wd_cmd_pri0;                        // 0x3e0c
	H14A0_MIFD_RD_REQ_PRI0_T                mifd_rd_req_pri0;                        // 0x3e10
	H14A0_MIFD_WD_REQ_PRI0_T                mifd_wd_req_pri0;                        // 0x3e14
	H14A0_MIFD_RD_REQ_STV0_T                mifd_rd_req_stv0;                        // 0x3e18
	H14A0_MIFD_WD_REQ_STV0_T                mifd_wd_req_stv0;                        // 0x3e1c
	H14A0_MIFD_RD_CMD_PEND0_T               mifd_rd_cmd_pend0;                       // 0x3e20
	H14A0_MIFD_WD_CMD_PEND0_T               mifd_wd_cmd_pend0;                       // 0x3e24
	H14A0_MIFD_RD_CMD_STRIDE0_T             mifd_rd_cmd_stride0;                     // 0x3e28
	H14A0_MIFD_RD_CMD_STRIDE_SET_T          mifd_rd_cmd_stride_set;                  // 0x3e2c
	H14A0_MIFD_WD_CMD_STRIDE0_T             mifd_wd_cmd_stride0;                     // 0x3e30
	H14A0_MIFD_WD_CMD_STRIDE_SET_T          mifd_wd_cmd_stride_set;                  // 0x3e34
	H14A0_MIFD_SADDR_RD_OV_Y_OV_C_T         mifd_saddr_rd_ov_y_ov_c;                 // 0x3e38
	H14A0_MIFD_SADDR_RD_CVE_Y_CVE_C_T       mifd_saddr_rd_cve_y_cve_c;               // 0x3e3c
	H14A0_MIFD_SADDR_WD_SM_YC_T             mifd_saddr_wd_sm_yc;                     // 0x3e40
	H14A0_MIFD_SADDR_RD_EXT0_T              mifd_saddr_rd_ext0;                      // 0x3e44
	H14A0_MIFD_SADDR_WD_EXT0_T              mifd_saddr_wd_ext0;                      // 0x3e48
	H14A0_MIFD_STATUS_T                     mifd_status;                             // 0x3e4c
	H14A0_MIFD_SADDR_PARAM_UPDATE0_M_T      mifd_saddr_param_update0_m;              // 0x3e50
	H14A0_MIFD_RD_BANK_8_ADDR0_T            mifd_rd_bank_8_addr0;                    // 0x3e54
	H14A0_MIFD_RD_BANK_8_ADDR1_T            mifd_rd_bank_8_addr1;                    // 0x3e58
	H14A0_MIFD_WD_BANK_8_ADDR0_T            mifd_wd_bank_8_addr0;                    // 0x3e5c
	H14A0_MIFD_WD_BANK_8_ADDR1_T            mifd_wd_bank_8_addr1;                    // 0x3e60
	H14A0_MIFD_RD_REQ_PEND0_T               mifd_rd_req_pend0;                       // 0x3e64
	H14A0_MIFD_WD_REQ_PEND0_T               mifd_wd_req_pend0;                       // 0x3e68
	H14A0_MIFD_RD_CMD_PRI0_S_T              mifd_rd_cmd_pri0_s;                      // 0x3e6c
	H14A0_MIFD_WD_CMD_PRI0_S_T              mifd_wd_cmd_pri0_s;                      // 0x3e70
	H14A0_MIFD_RESET_T                      mifd_reset;                              // 0x3e74
	H14A0_MIFD_RD_SET_10B_T                 mifd_rd_set_10b;                         // 0x3e78
	H14A0_MIFD_WD_SET_10B_T                 mifd_wd_set_10b;                         // 0x3e7c
	H14A0_MIFD_RD_CMD_2_STRIDE0_T           mifd_rd_cmd_2_stride0;                   // 0x3e80
	H14A0_MIFD_WD_CMD_2_STRIDE0_T           mifd_wd_cmd_2_stride0;                   // 0x3e84
	H14A0_MIFD_SADDR_RD_OV_Y_OV_C_2_T       mifd_saddr_rd_ov_y_ov_c_2;               // 0x3e88
	H14A0_MIFD_SADDR_WD_SM_YC_2_T           mifd_saddr_wd_sm_yc_2;                   // 0x3e8c
	H14A0_MIFD_SADDR_RD_2_EXT0_T            mifd_saddr_rd_2_ext0;                    // 0x3e90
	H14A0_MIFD_SADDR_WD_2_EXT0_T            mifd_saddr_wd_2_ext0;                    // 0x3e94
	H14A0_MIFD_RD_BANK_2_ADDR0_T            mifd_rd_bank_2_addr0;                    // 0x3e98
	H14A0_MIFD_RD_BANK_2_ADDR1_T            mifd_rd_bank_2_addr1;                    // 0x3e9c
	H14A0_MIFD_WD_BANK_2_ADDR0_T            mifd_wd_bank_2_addr0;                    // 0x3ea0
	H14A0_MIFD_WD_BANK_2_ADDR1_T            mifd_wd_bank_2_addr1;                    // 0x3ea4
	H14A0_MIFD_LSB_SWITCH_T                 mifd_lsb_switch;                         // 0x3ea8
} DE_MID_REG_H14A0_T;

typedef struct {
	H14A0_MIFO_RD_CTRL_T                    mifo_rd_ctrl;                            // 0x5900
	H14A0_MIFO_RD_CMD_PRI0_T                mifo_rd_cmd_pri0;                        // 0x5904
	H14A0_MIFO_RD_REQ_PRI0_T                mifo_rd_req_pri0;                        // 0x5908
	H14A0_MIFO_RD_REQ_STV0_T                mifo_rd_req_stv0;                        // 0x590c
	H14A0_MIFO_RD_CMD_PEND0_T               mifo_rd_cmd_pend0;                       // 0x5910
	H14A0_MIFO_SADDR_OSD_0_L_T              mifo_saddr_osd_0_l;                      // 0x5914
	H14A0_MIFO_SADDR_OSD_0_R_T              mifo_saddr_osd_0_r;                      // 0x5918
	H14A0_MIFO_SADDR_OSD_1_L_T              mifo_saddr_osd_1_l;                      // 0x591c
	H14A0_MIFO_SADDR_OSD_1_R_T              mifo_saddr_osd_1_r;                      // 0x5920
	H14A0_MIFO_SADDR_OSD_3_L_T              mifo_saddr_osd_3_l;                      // 0x5924
	H14A0_MIFO_SADDR_OSD_3_R_T              mifo_saddr_osd_3_r;                      // 0x5928
	H14A0_MIFO_STATUS_T                     mifo_status;                             // 0x592c
	H14A0_MIFO_RD_REQ_PEND0_T               mifo_rd_req_pend0;                       // 0x5930
	H14A0_MIFO_RD_CMD_PRI0_S_T              mifo_rd_cmd_pri0_s;                      // 0x5934
	H14A0_MIFO_RESET_T                      mifo_reset;                              // 0x5938
} DE_MIF_REG_H14A0_T;

#endif
