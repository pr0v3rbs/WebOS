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
#ifndef _DE_MIF_REG_H13B0_H_
#define _DE_MIF_REG_H13B0_H_


#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------------
		0x5900 MIFO_RD_CTRL ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 block_req_r_osd_0                  : 1 ;  //     0     
	UINT32 block_req_r_osd_1                  : 1 ;  //     1     
	UINT32 block_req_r_osd_2                  : 1 ;  //     2     
	UINT32 block_req_r_osd_3                  : 1 ;  //     3     
	UINT32 reserved1                          : 8 ;  // 11: 4     
	UINT32 disable_rd_mif                     : 1 ;  //    12     
	UINT32 load_rd_req_stv_v                  : 1 ;  //    13     
	UINT32 load_rd_req_pri_v                  : 1 ;  //    14     
	UINT32 load_rd_cmd_saddr_v                : 1 ;  //    15     
	UINT32 load_rd_cmd_pend_v                 : 1 ;  //    16     
	UINT32 load_rd_cmd_pri_v                  : 1 ;  //    17     
	UINT32                                    : 1 ;  //    18     reserved
	UINT32 enable_rd_stv_cnt                  : 1 ;  //    19     
	UINT32 reserved2                          : 1 ;  //    20     
	UINT32 mrb_bank_mode                      : 3 ;  // 23:21     
	UINT32 reserved                           : 8 ;  // 31:24     
} MIF_OSD_H13_RD_CTRL_T;

/*-----------------------------------------------------------------------------
		0x5904 MIFO_RD_CMD_PRI0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_pri_v_osd_0                 : 4 ;  //  3: 0     
	UINT32 cmd_rd_pri_v_osd_1                 : 4 ;  //  7: 4     
	UINT32 reserved1                          : 4 ;  // 11: 8     
	UINT32 cmd_rd_pri_v_osd_3                 : 4 ;  // 15:12     
	UINT32 reserved2                          : 16;  // 31:16     
} MIF_OSD_H13_RD_CMD_PRI0_T;

/*-----------------------------------------------------------------------------
		0x5908 MIFO_RD_REQ_PRI0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_pri_v_osd_0                 : 4 ;  //  3: 0     
	UINT32 req_rd_pri_v_osd_1                 : 4 ;  //  7: 4     
	UINT32 reserved1                          : 4 ;  // 11: 8     
	UINT32 req_rd_pri_v_osd_3                 : 4 ;  // 15:12     
	UINT32 reserved2                          : 16;  // 31:16     
} MIF_OSD_H13_RD_REQ_PRI0_T;

/*-----------------------------------------------------------------------------
		0x590c MIFO_RD_REQ_STV0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_stv_v_osd_0                 : 4 ;  //  3: 0     
	UINT32 req_rd_stv_v_osd_1                 : 4 ;  //  7: 4     
	UINT32 reserved1                          : 4 ;  // 11: 8     
	UINT32 req_rd_stv_v_osd_3                 : 4 ;  // 15:12     
	UINT32 reserved2                          : 16;  // 31:16     
} MIF_OSD_H13_RD_REQ_STV0_T;

/*-----------------------------------------------------------------------------
		0x5910 MIFO_RD_CMD_PEND0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_pend_v_osd_0                : 4 ;  //  3: 0     
	UINT32 cmd_rd_pend_v_osd_1                : 4 ;  //  7: 4     
	UINT32 reserved1                          : 4 ;  // 11: 8     
	UINT32 cmd_rd_pend_v_osd_3                : 4 ;  // 15:12     
	UINT32 reserved2                          : 16;  // 31:16     
} MIF_OSD_H13_RD_CMD_PEND0_T;

/*-----------------------------------------------------------------------------
		0x5914 MIFO_SADDR_OSD_0_L ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_saddr_osd_0_l               : 32;  // 31: 0     
} MIF_OSD_H13_SADDR_OSD_0_L_T;

/*-----------------------------------------------------------------------------
		0x5918 MIFO_SADDR_OSD_0_R ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_saddr_osd_0_r               : 32;  // 31: 0     
} MIF_OSD_H13_SADDR_OSD_0_R_T;

/*-----------------------------------------------------------------------------
		0x591c MIFO_SADDR_OSD_1_L ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_saddr_osd_1_l               : 32;  // 31: 0     
} MIF_OSD_H13_SADDR_OSD_1_L_T;

/*-----------------------------------------------------------------------------
		0x5920 MIFO_SADDR_OSD_1_R ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_saddr_osd_1_r               : 32;  // 31: 0     
} MIF_OSD_H13_SADDR_OSD_1_R_T;

/*-----------------------------------------------------------------------------
		0x5924 MIFO_SADDR_OSD_3_L ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_saddr_osd_3_l               : 32;  // 31: 0     
} MIF_OSD_H13_SADDR_OSD_3_L_T;

/*-----------------------------------------------------------------------------
		0x5928 MIFO_SADDR_OSD_3_R ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_saddr_osd_3_r               : 32;  // 31: 0     
} MIF_OSD_H13_SADDR_OSD_3_R_T;

/*-----------------------------------------------------------------------------
		0x592c MIFO_STATUS ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 fr_rd_valid_osd_0                  : 1 ;  //     0     
	UINT32 fr_rd_valid_osd_1                  : 1 ;  //     1     
	UINT32 reserved1                          : 1 ;  //     2     
	UINT32 fr_rd_valid_osd_3                  : 1 ;  //     3     
	UINT32 reserved2                          : 28;  // 31: 4     
} MIF_OSD_H13_STATUS_T;

/*-----------------------------------------------------------------------------
		0x5930 MIFO_RD_REQ_PEND0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 req_rd_pend_v_osd_0                : 8 ;  //  7: 0     
	UINT32 req_rd_pend_v_osd_1                : 8 ;  // 15: 8     
	UINT32 reserved                           : 8 ;  // 23:16     
	UINT32 req_rd_pend_v_osd_3                : 8 ;  // 31:24     
} MIF_OSD_H13_RD_REQ_PEND0_T;

/*-----------------------------------------------------------------------------
		0x5934 MIFO_RD_CMD_PRI0_S ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cmd_rd_pri_s_v_osd_0               : 4 ;  //  3: 0     
	UINT32 cmd_rd_pri_s_v_osd_1               : 4 ;  //  7: 4     
	UINT32 reserved1                          : 4 ;  // 11: 8     
	UINT32 cmd_rd_pri_s_v_osd_3               : 4 ;  // 15:12     
	UINT32 reserved2                          : 16;  // 31:16     
} MIF_OSD_H13_RD_CMD_PRI0_S_T;

/*-----------------------------------------------------------------------------
		0x5938 MIFO_RESET ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reset_mif_r_osd_0                  : 1 ;  //     0     
	UINT32 reset_mif_r_osd_1                  : 1 ;  //     1     
	UINT32 reserved1                          : 1 ;  //     2     
	UINT32 reset_mif_r_osd_3                  : 1 ;  //     3     
	UINT32 reserved2                          : 16;  // 19: 4     
	UINT32 reset_time_out_cnt                 : 12;  // 31:20     
} MIF_OSD_H13_RESET_T;

typedef struct {
	MIF_OSD_H13_RD_CTRL_T               mifo_rd_ctrl;                 //0x5900
	MIF_OSD_H13_RD_CMD_PRI0_T           mifo_rd_cmd_pri0;             //0x5904
	MIF_OSD_H13_RD_REQ_PRI0_T           mifo_rd_req_pri0;             //0x5908
	MIF_OSD_H13_RD_REQ_STV0_T           mifo_rd_req_stv0;             //0x590c
	MIF_OSD_H13_RD_CMD_PEND0_T          mifo_rd_cmd_pend0;            //0x5910
	MIF_OSD_H13_SADDR_OSD_0_L_T         mifo_saddr_osd_0_l;           //0x5914
	MIF_OSD_H13_SADDR_OSD_0_R_T         mifo_saddr_osd_0_r;           //0x5918
	MIF_OSD_H13_SADDR_OSD_1_L_T         mifo_saddr_osd_1_l;           //0x591c
	MIF_OSD_H13_SADDR_OSD_1_R_T         mifo_saddr_osd_1_r;           //0x5920
	MIF_OSD_H13_SADDR_OSD_3_L_T         mifo_saddr_osd_3_l;           //0x5924
	MIF_OSD_H13_SADDR_OSD_3_R_T         mifo_saddr_osd_3_r;           //0x5928
	MIF_OSD_H13_STATUS_T                mifo_status;                  //0x592c
	MIF_OSD_H13_RD_REQ_PEND0_T          mifo_rd_req_pend0;            //0x5930
	MIF_OSD_H13_RESET_T                 mifo_reset;                   //0x5934
}
MIF_OSD_H13_REG_T;

#define MIF_OSD_H13_RdFL(_r)			((g_mif_osd_h13_reg_cached->_r)=(g_mif_osd_h13_reg->_r))
#define MIF_OSD_H13_WrFL(_r)			((g_mif_osd_h13_reg->_r)=(g_mif_osd_h13_reg_cached->_r))

#define MIF_OSD_H13_Rd(_r)				*((UINT32*)(&(g_mif_osd_h13_reg_cached->_r)))
#define MIF_OSD_H13_Wr(_r,_v)			((MIF_OSD_H13_Rd(_r))=((UINT32)(_v)))

#define MIF_OSD_H13_Rd01(_r,_f01,_v01)													\
								do { 											\
									(_v01) = (g_mif_osd_h13_reg_cached->_r._f01);				\
								} while(0)

#define MIF_OSD_H13_Rd02(_r,_f01,_v01,_f02,_v02)										\
								do { 											\
									(_v01) = (g_mif_osd_h13_reg_cached->_r._f01);				\
									(_v02) = (g_mif_osd_h13_reg_cached->_r._f02);				\
								} while(0)

#define MIF_OSD_H13_Rd03(_r,_f01,_v01,_f02,_v02,_f03,_v03)								\
								do { 											\
									(_v01) = (g_mif_osd_h13_reg_cached->_r._f01);				\
									(_v02) = (g_mif_osd_h13_reg_cached->_r._f02);				\
									(_v03) = (g_mif_osd_h13_reg_cached->_r._f03);				\
								} while(0)

#define MIF_OSD_H13_Rd04(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04)					\
								do { 											\
									(_v01) = (g_mif_osd_h13_reg_cached->_r._f01);				\
									(_v02) = (g_mif_osd_h13_reg_cached->_r._f02);				\
									(_v03) = (g_mif_osd_h13_reg_cached->_r._f03);				\
									(_v04) = (g_mif_osd_h13_reg_cached->_r._f04);				\
								} while(0)

#define MIF_OSD_H13_Rd05(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05)													\
								do { 											\
									(_v01) = (g_mif_osd_h13_reg_cached->_r._f01);				\
									(_v02) = (g_mif_osd_h13_reg_cached->_r._f02);				\
									(_v03) = (g_mif_osd_h13_reg_cached->_r._f03);				\
									(_v04) = (g_mif_osd_h13_reg_cached->_r._f04);				\
									(_v05) = (g_mif_osd_h13_reg_cached->_r._f05);				\
								} while(0)

#define MIF_OSD_H13_Rd06(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06)										\
								do { 											\
									(_v01) = (g_mif_osd_h13_reg_cached->_r._f01);				\
									(_v02) = (g_mif_osd_h13_reg_cached->_r._f02);				\
									(_v03) = (g_mif_osd_h13_reg_cached->_r._f03);				\
									(_v04) = (g_mif_osd_h13_reg_cached->_r._f04);				\
									(_v05) = (g_mif_osd_h13_reg_cached->_r._f05);				\
									(_v06) = (g_mif_osd_h13_reg_cached->_r._f06);				\
								} while(0)

#define MIF_OSD_H13_Rd07(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07)								\
								do { 											\
									(_v01) = (g_mif_osd_h13_reg_cached->_r._f01);				\
									(_v02) = (g_mif_osd_h13_reg_cached->_r._f02);				\
									(_v03) = (g_mif_osd_h13_reg_cached->_r._f03);				\
									(_v04) = (g_mif_osd_h13_reg_cached->_r._f04);				\
									(_v05) = (g_mif_osd_h13_reg_cached->_r._f05);				\
									(_v06) = (g_mif_osd_h13_reg_cached->_r._f06);				\
									(_v07) = (g_mif_osd_h13_reg_cached->_r._f07);				\
								} while(0)

#define MIF_OSD_H13_Rd08(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08)					\
								do { 											\
									(_v01) = (g_mif_osd_h13_reg_cached->_r._f01);				\
									(_v02) = (g_mif_osd_h13_reg_cached->_r._f02);				\
									(_v03) = (g_mif_osd_h13_reg_cached->_r._f03);				\
									(_v04) = (g_mif_osd_h13_reg_cached->_r._f04);				\
									(_v05) = (g_mif_osd_h13_reg_cached->_r._f05);				\
									(_v06) = (g_mif_osd_h13_reg_cached->_r._f06);				\
									(_v07) = (g_mif_osd_h13_reg_cached->_r._f07);				\
									(_v08) = (g_mif_osd_h13_reg_cached->_r._f08);				\
								} while(0)

#define MIF_OSD_H13_Rd09(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09)													\
								do { 											\
									(_v01) = (g_mif_osd_h13_reg_cached->_r._f01);				\
									(_v02) = (g_mif_osd_h13_reg_cached->_r._f02);				\
									(_v03) = (g_mif_osd_h13_reg_cached->_r._f03);				\
									(_v04) = (g_mif_osd_h13_reg_cached->_r._f04);				\
									(_v05) = (g_mif_osd_h13_reg_cached->_r._f05);				\
									(_v06) = (g_mif_osd_h13_reg_cached->_r._f06);				\
									(_v07) = (g_mif_osd_h13_reg_cached->_r._f07);				\
									(_v08) = (g_mif_osd_h13_reg_cached->_r._f08);				\
									(_v09) = (g_mif_osd_h13_reg_cached->_r._f09);				\
								} while(0)

#define MIF_OSD_H13_Rd10(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10)										\
								do { 											\
									(_v01) = (g_mif_osd_h13_reg_cached->_r._f01);				\
									(_v02) = (g_mif_osd_h13_reg_cached->_r._f02);				\
									(_v03) = (g_mif_osd_h13_reg_cached->_r._f03);				\
									(_v04) = (g_mif_osd_h13_reg_cached->_r._f04);				\
									(_v05) = (g_mif_osd_h13_reg_cached->_r._f05);				\
									(_v06) = (g_mif_osd_h13_reg_cached->_r._f06);				\
									(_v07) = (g_mif_osd_h13_reg_cached->_r._f07);				\
									(_v08) = (g_mif_osd_h13_reg_cached->_r._f08);				\
									(_v09) = (g_mif_osd_h13_reg_cached->_r._f09);				\
									(_v10) = (g_mif_osd_h13_reg_cached->_r._f10);				\
								} while(0)

#define MIF_OSD_H13_Rd11(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11)								\
								do { 											\
									(_v01) = (g_mif_osd_h13_reg_cached->_r._f01);				\
									(_v02) = (g_mif_osd_h13_reg_cached->_r._f02);				\
									(_v03) = (g_mif_osd_h13_reg_cached->_r._f03);				\
									(_v04) = (g_mif_osd_h13_reg_cached->_r._f04);				\
									(_v05) = (g_mif_osd_h13_reg_cached->_r._f05);				\
									(_v06) = (g_mif_osd_h13_reg_cached->_r._f06);				\
									(_v07) = (g_mif_osd_h13_reg_cached->_r._f07);				\
									(_v08) = (g_mif_osd_h13_reg_cached->_r._f08);				\
									(_v09) = (g_mif_osd_h13_reg_cached->_r._f09);				\
									(_v10) = (g_mif_osd_h13_reg_cached->_r._f10);				\
									(_v11) = (g_mif_osd_h13_reg_cached->_r._f11);				\
								} while(0)

#define MIF_OSD_H13_Rd12(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12)					\
								do { 											\
									(_v01) = (g_mif_osd_h13_reg_cached->_r._f01);				\
									(_v02) = (g_mif_osd_h13_reg_cached->_r._f02);				\
									(_v03) = (g_mif_osd_h13_reg_cached->_r._f03);				\
									(_v04) = (g_mif_osd_h13_reg_cached->_r._f04);				\
									(_v05) = (g_mif_osd_h13_reg_cached->_r._f05);				\
									(_v06) = (g_mif_osd_h13_reg_cached->_r._f06);				\
									(_v07) = (g_mif_osd_h13_reg_cached->_r._f07);				\
									(_v08) = (g_mif_osd_h13_reg_cached->_r._f08);				\
									(_v09) = (g_mif_osd_h13_reg_cached->_r._f09);				\
									(_v10) = (g_mif_osd_h13_reg_cached->_r._f10);				\
									(_v11) = (g_mif_osd_h13_reg_cached->_r._f11);				\
									(_v12) = (g_mif_osd_h13_reg_cached->_r._f12);				\
								} while(0)

#define MIF_OSD_H13_Rd13(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13)													\
								do { 											\
									(_v01) = (g_mif_osd_h13_reg_cached->_r._f01);				\
									(_v02) = (g_mif_osd_h13_reg_cached->_r._f02);				\
									(_v03) = (g_mif_osd_h13_reg_cached->_r._f03);				\
									(_v04) = (g_mif_osd_h13_reg_cached->_r._f04);				\
									(_v05) = (g_mif_osd_h13_reg_cached->_r._f05);				\
									(_v06) = (g_mif_osd_h13_reg_cached->_r._f06);				\
									(_v07) = (g_mif_osd_h13_reg_cached->_r._f07);				\
									(_v08) = (g_mif_osd_h13_reg_cached->_r._f08);				\
									(_v09) = (g_mif_osd_h13_reg_cached->_r._f09);				\
									(_v10) = (g_mif_osd_h13_reg_cached->_r._f10);				\
									(_v11) = (g_mif_osd_h13_reg_cached->_r._f11);				\
									(_v12) = (g_mif_osd_h13_reg_cached->_r._f12);				\
									(_v13) = (g_mif_osd_h13_reg_cached->_r._f13);				\
								} while(0)

#define MIF_OSD_H13_Rd14(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14)										\
								do { 											\
									(_v01) = (g_mif_osd_h13_reg_cached->_r._f01);				\
									(_v02) = (g_mif_osd_h13_reg_cached->_r._f02);				\
									(_v03) = (g_mif_osd_h13_reg_cached->_r._f03);				\
									(_v04) = (g_mif_osd_h13_reg_cached->_r._f04);				\
									(_v05) = (g_mif_osd_h13_reg_cached->_r._f05);				\
									(_v06) = (g_mif_osd_h13_reg_cached->_r._f06);				\
									(_v07) = (g_mif_osd_h13_reg_cached->_r._f07);				\
									(_v08) = (g_mif_osd_h13_reg_cached->_r._f08);				\
									(_v09) = (g_mif_osd_h13_reg_cached->_r._f09);				\
									(_v10) = (g_mif_osd_h13_reg_cached->_r._f10);				\
									(_v11) = (g_mif_osd_h13_reg_cached->_r._f11);				\
									(_v12) = (g_mif_osd_h13_reg_cached->_r._f12);				\
									(_v13) = (g_mif_osd_h13_reg_cached->_r._f13);				\
									(_v14) = (g_mif_osd_h13_reg_cached->_r._f14);				\
								} while(0)

#define MIF_OSD_H13_Rd15(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15)								\
								do { 											\
									(_v01) = (g_mif_osd_h13_reg_cached->_r._f01);				\
									(_v02) = (g_mif_osd_h13_reg_cached->_r._f02);				\
									(_v03) = (g_mif_osd_h13_reg_cached->_r._f03);				\
									(_v04) = (g_mif_osd_h13_reg_cached->_r._f04);				\
									(_v05) = (g_mif_osd_h13_reg_cached->_r._f05);				\
									(_v06) = (g_mif_osd_h13_reg_cached->_r._f06);				\
									(_v07) = (g_mif_osd_h13_reg_cached->_r._f07);				\
									(_v08) = (g_mif_osd_h13_reg_cached->_r._f08);				\
									(_v09) = (g_mif_osd_h13_reg_cached->_r._f09);				\
									(_v10) = (g_mif_osd_h13_reg_cached->_r._f10);				\
									(_v11) = (g_mif_osd_h13_reg_cached->_r._f11);				\
									(_v12) = (g_mif_osd_h13_reg_cached->_r._f12);				\
									(_v13) = (g_mif_osd_h13_reg_cached->_r._f13);				\
									(_v14) = (g_mif_osd_h13_reg_cached->_r._f14);				\
									(_v15) = (g_mif_osd_h13_reg_cached->_r._f15);				\
								} while(0)

#define MIF_OSD_H13_Rd16(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15,_f16,_v16)					\
								do { 											\
									(_v01) = (g_mif_osd_h13_reg_cached->_r._f01);				\
									(_v02) = (g_mif_osd_h13_reg_cached->_r._f02);				\
									(_v03) = (g_mif_osd_h13_reg_cached->_r._f03);				\
									(_v04) = (g_mif_osd_h13_reg_cached->_r._f04);				\
									(_v05) = (g_mif_osd_h13_reg_cached->_r._f05);				\
									(_v06) = (g_mif_osd_h13_reg_cached->_r._f06);				\
									(_v07) = (g_mif_osd_h13_reg_cached->_r._f07);				\
									(_v08) = (g_mif_osd_h13_reg_cached->_r._f08);				\
									(_v09) = (g_mif_osd_h13_reg_cached->_r._f09);				\
									(_v10) = (g_mif_osd_h13_reg_cached->_r._f10);				\
									(_v11) = (g_mif_osd_h13_reg_cached->_r._f11);				\
									(_v12) = (g_mif_osd_h13_reg_cached->_r._f12);				\
									(_v13) = (g_mif_osd_h13_reg_cached->_r._f13);				\
									(_v14) = (g_mif_osd_h13_reg_cached->_r._f14);				\
									(_v15) = (g_mif_osd_h13_reg_cached->_r._f15);				\
									(_v16) = (g_mif_osd_h13_reg_cached->_r._f16);				\
								} while(0)


#define MIF_OSD_H13_Wr01(_r,_f01,_v01)													\
								do { 											\
									(g_mif_osd_h13_reg_cached->_r._f01) = (_v01);				\
								} while(0)

#define MIF_OSD_H13_Wr02(_r,_f01,_v01,_f02,_v02)										\
								do { 											\
									(g_mif_osd_h13_reg_cached->_r._f01) = (_v01);				\
									(g_mif_osd_h13_reg_cached->_r._f02) = (_v02);				\
								} while(0)

#define MIF_OSD_H13_Wr03(_r,_f01,_v01,_f02,_v02,_f03,_v03)								\
								do { 											\
									(g_mif_osd_h13_reg_cached->_r._f01) = (_v01);				\
									(g_mif_osd_h13_reg_cached->_r._f02) = (_v02);				\
									(g_mif_osd_h13_reg_cached->_r._f03) = (_v03);				\
								} while(0)

#define MIF_OSD_H13_Wr04(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04)					\
								do { 											\
									(g_mif_osd_h13_reg_cached->_r._f01) = (_v01);				\
									(g_mif_osd_h13_reg_cached->_r._f02) = (_v02);				\
									(g_mif_osd_h13_reg_cached->_r._f03) = (_v03);				\
									(g_mif_osd_h13_reg_cached->_r._f04) = (_v04);				\
								} while(0)

#define MIF_OSD_H13_Wr05(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05)													\
								do { 											\
									(g_mif_osd_h13_reg_cached->_r._f01) = (_v01);				\
									(g_mif_osd_h13_reg_cached->_r._f02) = (_v02);				\
									(g_mif_osd_h13_reg_cached->_r._f03) = (_v03);				\
									(g_mif_osd_h13_reg_cached->_r._f04) = (_v04);				\
									(g_mif_osd_h13_reg_cached->_r._f05) = (_v05);				\
								} while(0)

#define MIF_OSD_H13_Wr06(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06)										\
								do { 											\
									(g_mif_osd_h13_reg_cached->_r._f01) = (_v01);				\
									(g_mif_osd_h13_reg_cached->_r._f02) = (_v02);				\
									(g_mif_osd_h13_reg_cached->_r._f03) = (_v03);				\
									(g_mif_osd_h13_reg_cached->_r._f04) = (_v04);				\
									(g_mif_osd_h13_reg_cached->_r._f05) = (_v05);				\
									(g_mif_osd_h13_reg_cached->_r._f06) = (_v06);				\
								} while(0)

#define MIF_OSD_H13_Wr07(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07)								\
								do { 											\
									(g_mif_osd_h13_reg_cached->_r._f01) = (_v01);				\
									(g_mif_osd_h13_reg_cached->_r._f02) = (_v02);				\
									(g_mif_osd_h13_reg_cached->_r._f03) = (_v03);				\
									(g_mif_osd_h13_reg_cached->_r._f04) = (_v04);				\
									(g_mif_osd_h13_reg_cached->_r._f05) = (_v05);				\
									(g_mif_osd_h13_reg_cached->_r._f06) = (_v06);				\
									(g_mif_osd_h13_reg_cached->_r._f07) = (_v07);				\
								} while(0)

#define MIF_OSD_H13_Wr08(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08)					\
								do { 											\
									(g_mif_osd_h13_reg_cached->_r._f01) = (_v01);				\
									(g_mif_osd_h13_reg_cached->_r._f02) = (_v02);				\
									(g_mif_osd_h13_reg_cached->_r._f03) = (_v03);				\
									(g_mif_osd_h13_reg_cached->_r._f04) = (_v04);				\
									(g_mif_osd_h13_reg_cached->_r._f05) = (_v05);				\
									(g_mif_osd_h13_reg_cached->_r._f06) = (_v06);				\
									(g_mif_osd_h13_reg_cached->_r._f07) = (_v07);				\
									(g_mif_osd_h13_reg_cached->_r._f08) = (_v08);				\
								} while(0)

#define MIF_OSD_H13_Wr09(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09)													\
								do { 											\
									(g_mif_osd_h13_reg_cached->_r._f01) = (_v01);				\
									(g_mif_osd_h13_reg_cached->_r._f02) = (_v02);				\
									(g_mif_osd_h13_reg_cached->_r._f03) = (_v03);				\
									(g_mif_osd_h13_reg_cached->_r._f04) = (_v04);				\
									(g_mif_osd_h13_reg_cached->_r._f05) = (_v05);				\
									(g_mif_osd_h13_reg_cached->_r._f06) = (_v06);				\
									(g_mif_osd_h13_reg_cached->_r._f07) = (_v07);				\
									(g_mif_osd_h13_reg_cached->_r._f08) = (_v08);				\
									(g_mif_osd_h13_reg_cached->_r._f09) = (_v09);				\
								} while(0)

#define MIF_OSD_H13_Wr10(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10)										\
								do { 											\
									(g_mif_osd_h13_reg_cached->_r._f01) = (_v01);				\
									(g_mif_osd_h13_reg_cached->_r._f02) = (_v02);				\
									(g_mif_osd_h13_reg_cached->_r._f03) = (_v03);				\
									(g_mif_osd_h13_reg_cached->_r._f04) = (_v04);				\
									(g_mif_osd_h13_reg_cached->_r._f05) = (_v05);				\
									(g_mif_osd_h13_reg_cached->_r._f06) = (_v06);				\
									(g_mif_osd_h13_reg_cached->_r._f07) = (_v07);				\
									(g_mif_osd_h13_reg_cached->_r._f08) = (_v08);				\
									(g_mif_osd_h13_reg_cached->_r._f09) = (_v09);				\
									(g_mif_osd_h13_reg_cached->_r._f10) = (_v10);				\
								} while(0)

#define MIF_OSD_H13_Wr11(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11)								\
								do { 											\
									(g_mif_osd_h13_reg_cached->_r._f01) = (_v01);				\
									(g_mif_osd_h13_reg_cached->_r._f02) = (_v02);				\
									(g_mif_osd_h13_reg_cached->_r._f03) = (_v03);				\
									(g_mif_osd_h13_reg_cached->_r._f04) = (_v04);				\
									(g_mif_osd_h13_reg_cached->_r._f05) = (_v05);				\
									(g_mif_osd_h13_reg_cached->_r._f06) = (_v06);				\
									(g_mif_osd_h13_reg_cached->_r._f07) = (_v07);				\
									(g_mif_osd_h13_reg_cached->_r._f08) = (_v08);				\
									(g_mif_osd_h13_reg_cached->_r._f09) = (_v09);				\
									(g_mif_osd_h13_reg_cached->_r._f10) = (_v10);				\
									(g_mif_osd_h13_reg_cached->_r._f11) = (_v11);				\
								} while(0)

#define MIF_OSD_H13_Wr12(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12)					\
								do { 											\
									(g_mif_osd_h13_reg_cached->_r._f01) = (_v01);				\
									(g_mif_osd_h13_reg_cached->_r._f02) = (_v02);				\
									(g_mif_osd_h13_reg_cached->_r._f03) = (_v03);				\
									(g_mif_osd_h13_reg_cached->_r._f04) = (_v04);				\
									(g_mif_osd_h13_reg_cached->_r._f05) = (_v05);				\
									(g_mif_osd_h13_reg_cached->_r._f06) = (_v06);				\
									(g_mif_osd_h13_reg_cached->_r._f07) = (_v07);				\
									(g_mif_osd_h13_reg_cached->_r._f08) = (_v08);				\
									(g_mif_osd_h13_reg_cached->_r._f09) = (_v09);				\
									(g_mif_osd_h13_reg_cached->_r._f10) = (_v10);				\
									(g_mif_osd_h13_reg_cached->_r._f11) = (_v11);				\
									(g_mif_osd_h13_reg_cached->_r._f12) = (_v12);				\
								} while(0)

#define MIF_OSD_H13_Wr13(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13)													\
								do { 											\
									(g_mif_osd_h13_reg_cached->_r._f01) = (_v01);				\
									(g_mif_osd_h13_reg_cached->_r._f02) = (_v02);				\
									(g_mif_osd_h13_reg_cached->_r._f03) = (_v03);				\
									(g_mif_osd_h13_reg_cached->_r._f04) = (_v04);				\
									(g_mif_osd_h13_reg_cached->_r._f05) = (_v05);				\
									(g_mif_osd_h13_reg_cached->_r._f06) = (_v06);				\
									(g_mif_osd_h13_reg_cached->_r._f07) = (_v07);				\
									(g_mif_osd_h13_reg_cached->_r._f08) = (_v08);				\
									(g_mif_osd_h13_reg_cached->_r._f09) = (_v09);				\
									(g_mif_osd_h13_reg_cached->_r._f10) = (_v10);				\
									(g_mif_osd_h13_reg_cached->_r._f11) = (_v11);				\
									(g_mif_osd_h13_reg_cached->_r._f12) = (_v12);				\
									(g_mif_osd_h13_reg_cached->_r._f13) = (_v13);				\
								} while(0)

#define MIF_OSD_H13_Wr14(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14)										\
								do { 											\
									(g_mif_osd_h13_reg_cached->_r._f01) = (_v01);				\
									(g_mif_osd_h13_reg_cached->_r._f02) = (_v02);				\
									(g_mif_osd_h13_reg_cached->_r._f03) = (_v03);				\
									(g_mif_osd_h13_reg_cached->_r._f04) = (_v04);				\
									(g_mif_osd_h13_reg_cached->_r._f05) = (_v05);				\
									(g_mif_osd_h13_reg_cached->_r._f06) = (_v06);				\
									(g_mif_osd_h13_reg_cached->_r._f07) = (_v07);				\
									(g_mif_osd_h13_reg_cached->_r._f08) = (_v08);				\
									(g_mif_osd_h13_reg_cached->_r._f09) = (_v09);				\
									(g_mif_osd_h13_reg_cached->_r._f10) = (_v10);				\
									(g_mif_osd_h13_reg_cached->_r._f11) = (_v11);				\
									(g_mif_osd_h13_reg_cached->_r._f12) = (_v12);				\
									(g_mif_osd_h13_reg_cached->_r._f13) = (_v13);				\
									(g_mif_osd_h13_reg_cached->_r._f14) = (_v14);				\
								} while(0)

#define MIF_OSD_H13_Wr15(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15)								\
								do { 											\
									(g_mif_osd_h13_reg_cached->_r._f01) = (_v01);				\
									(g_mif_osd_h13_reg_cached->_r._f02) = (_v02);				\
									(g_mif_osd_h13_reg_cached->_r._f03) = (_v03);				\
									(g_mif_osd_h13_reg_cached->_r._f04) = (_v04);				\
									(g_mif_osd_h13_reg_cached->_r._f05) = (_v05);				\
									(g_mif_osd_h13_reg_cached->_r._f06) = (_v06);				\
									(g_mif_osd_h13_reg_cached->_r._f07) = (_v07);				\
									(g_mif_osd_h13_reg_cached->_r._f08) = (_v08);				\
									(g_mif_osd_h13_reg_cached->_r._f09) = (_v09);				\
									(g_mif_osd_h13_reg_cached->_r._f10) = (_v10);				\
									(g_mif_osd_h13_reg_cached->_r._f11) = (_v11);				\
									(g_mif_osd_h13_reg_cached->_r._f12) = (_v12);				\
									(g_mif_osd_h13_reg_cached->_r._f13) = (_v13);				\
									(g_mif_osd_h13_reg_cached->_r._f14) = (_v14);				\
									(g_mif_osd_h13_reg_cached->_r._f15) = (_v15);				\
								} while(0)

#define MIF_OSD_H13_Wr16(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15,_f16,_v16)					\
								do { 											\
									(g_mif_osd_h13_reg_cached->_r._f01) = (_v01);				\
									(g_mif_osd_h13_reg_cached->_r._f02) = (_v02);				\
									(g_mif_osd_h13_reg_cached->_r._f03) = (_v03);				\
									(g_mif_osd_h13_reg_cached->_r._f04) = (_v04);				\
									(g_mif_osd_h13_reg_cached->_r._f05) = (_v05);				\
									(g_mif_osd_h13_reg_cached->_r._f06) = (_v06);				\
									(g_mif_osd_h13_reg_cached->_r._f07) = (_v07);				\
									(g_mif_osd_h13_reg_cached->_r._f08) = (_v08);				\
									(g_mif_osd_h13_reg_cached->_r._f09) = (_v09);				\
									(g_mif_osd_h13_reg_cached->_r._f10) = (_v10);				\
									(g_mif_osd_h13_reg_cached->_r._f11) = (_v11);				\
									(g_mif_osd_h13_reg_cached->_r._f12) = (_v12);				\
									(g_mif_osd_h13_reg_cached->_r._f13) = (_v13);				\
									(g_mif_osd_h13_reg_cached->_r._f14) = (_v14);				\
									(g_mif_osd_h13_reg_cached->_r._f15) = (_v15);				\
									(g_mif_osd_h13_reg_cached->_r._f16) = (_v16);				\
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
 * _rwname: name of rw    bit field : shall be 0 after MIF_OSD_H13_Wind(), 1 for MIF_OSD_H13_Rind()
 * _iname : name of index bit field
 * _ival  : index value
 * _fname : field name
 * _fval  : field variable that field value shall be stored.
 *
 * MIF_OSD_H13_Rind : General indexed register Read.(
 * MIF_OSD_H13_Wind : General indexed register Read.
 *
 * MIF_OSD_H13_Ridx : For 'index', 'rw', 'load' field name
 * MIF_OSD_H13_Widx : For 'index', 'rw', 'load' field name and NO_LOAD.
 */
#define MIF_OSD_H13_Rind(_r, _lname, _rwname, _iname, _ival, _fname, _fval)				\
							do {												\
								MIF_OSD_H13_Wr03(_r,_lname,0,_rwname,1,_iname,_ival);	\
								MIF_OSD_H13_WrFL(_r);									\
								MIF_OSD_H13_RdFL(_r);									\
								MIF_OSD_H13_Rd01(_r,_fname,_fval);						\
							} while (0)

#define MIF_OSD_H13_Wind(_r, _lname, _rwname, _iname, _ival, _fname, _fval)				\
				MIF_OSD_H13_Wr04(_r, _lname,0, _rwname,0, _iname,_ival, _fname,_fval)


#define MIF_OSD_H13_Ridx(_r, _ival, _fname, _fval)	MIF_OSD_H13_Rind(_r,load,rw,index,_ival,_fname,_fval)

#define MIF_OSD_H13_Widx(_r, _ival, _fname, _fval)	MIF_OSD_H13_Wind(_r,load,rw,index,_ival,_fname,_fval)

/** @} *//* end of macro documentation */

#ifdef __cplusplus
}
#endif

/* from 'H13_MIF_OSD_H13_RegFile.csv' 20120409 16:25:56   대한민국 표준시 by getregs v2.7 */
#endif
