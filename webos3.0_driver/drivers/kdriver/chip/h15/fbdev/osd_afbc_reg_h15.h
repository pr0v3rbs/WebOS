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

/*
 *
 *  #MOD# register details. ( used only within kdriver )
 *
 *  author     user name (user_name@lge.com)
 *  version    1.0
 *  date       2010.xx.xx
 *
 */

#ifndef __OSD_AFBC_REG_H15_H__
#define __OSD_AFBC_REG_H15_H__

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
	0x0000 ctrl_afbc_0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	start                           : 1,	//     0
	                                : 3,	//  1: 3 reserved
	ready                           :12,	//  4:15
	sw_reset                        : 1,	//    16
	sw_reset_fifo                   : 1,	//    17
	run_mode                        : 1,	//    18
	pl_addr_mode                    : 1,	//    19
	sw_reset_ack                    : 2,	// 20:21
	                                : 2,	// 22:23 reserved
	gmau_arb_sw_reset               : 1;	//    24
} CTRL_AFBC_0;

/*-----------------------------------------------------------------------------
	0x0004 ctrl_afbc_1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	hd_base                         ;   	// 31: 0
} CTRL_AFBC_1;

/*-----------------------------------------------------------------------------
	0x0008 ctrl_afbc_2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	start_x                         :12,	//  0:11
	                                : 4,	// 12:15 reserved
	start_y                         :12;	// 16:27
} CTRL_AFBC_2;

/*-----------------------------------------------------------------------------
	0x000c ctrl_afbc_3 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	end_x                           :12,	//  0:11
	                                : 4,	// 12:15 reserved
	end_y                           :12;	// 16:27
} CTRL_AFBC_3;

/*-----------------------------------------------------------------------------
	0x0010 ctrl_afbc_4 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	img_width                       :12;	//  0:11
} CTRL_AFBC_4;

/*-----------------------------------------------------------------------------
	0x0014 ctrl_afbc_5 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pl_buf_depth_m1                 : 8,	//  0: 7
	                                : 8,	//  8:15 reserved
	dec_buf_depth_m1                :12;	// 16:27
} CTRL_AFBC_5;

/*-----------------------------------------------------------------------------
	0x0018 ctrl_afbc_6 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	hd_data_base                    ;   	// 31: 0
} CTRL_AFBC_6;

/*-----------------------------------------------------------------------------
	0x001c ctrl_afbc_7 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	hd_data_end                     ;   	// 31: 0
} CTRL_AFBC_7;

/*-----------------------------------------------------------------------------
	0x0020 ctrl_afbc_8 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pl_data_base                    ;   	// 31: 0
} CTRL_AFBC_8;

/*-----------------------------------------------------------------------------
	0x0024 ctrl_afbc_9 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pl_data_end                     ;   	// 31: 0
} CTRL_AFBC_9;

/*-----------------------------------------------------------------------------
	0x0028 ctrl_afbc_a ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	hd_gmau_dly_cnt                 :16,	//  0:15
	pl_gmau_dly_cnt                 :16;	// 16:31
} CTRL_AFBC_A;

/*-----------------------------------------------------------------------------
	0x002c ctrl_afbc_b ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	hd_gmau_pri1                    : 4,	//  0: 3
	                                : 4,	//  4: 7 reserved
	hd_gmau_pri2                    : 4,	//  8:11
	                                : 4,	// 12:15 reserved
	pl_gmau_pri1                    : 4,	// 16:19
	                                : 4,	// 20:23 reserved
	pl_gmau_pri2                    : 4;	// 24:27
} CTRL_AFBC_B;

/*-----------------------------------------------------------------------------
	0x0030 ctrl_afbc_c ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	hd_gmau_txid                    : 2,	//  0: 1
	                                : 2,	//  2: 3 reserved
	pl_gmau_id                      : 2,	//  4: 5
	                                : 2,	//  6: 7 reserved
	hd_endian_sw                    : 4,	//  8:11
	pl_endian_sw                    : 4,	// 12:15
	hd_endian_arb                   : 1,	//    16
	                                : 3,	// 17:19 reserved
	pl_endian_arb                   : 1;	//    20
} CTRL_AFBC_C;

/*-----------------------------------------------------------------------------
	0x0034 ctrl_afbc_d ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	hd_stride                       ;   	// 31: 0
} CTRL_AFBC_D;

/*-----------------------------------------------------------------------------
	0x0038 ctrl_afbc_e ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pl_stride                       ;   	// 31: 0
} CTRL_AFBC_E;

/*-----------------------------------------------------------------------------
	0x003c ctrl_afbc_f ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	port0_max_cnt                   : 6,	//  0: 5
	                                : 2,	//  6: 7 reserved
	port1_max_cnt                   : 6,	//  8:13
	                                : 2,	// 14:15 reserved
	port2_max_cnt                   : 6;	// 16:21
} CTRL_AFBC_F;

/*-----------------------------------------------------------------------------
	0x0040 info_afbc_0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	info_afbc_0                     ;   	// 31: 0
} INFO_AFBC_0;

/*-----------------------------------------------------------------------------
	0x0044 info_afbc_1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	info_afbc_0                     ;   	// 31: 0
} INFO_AFBC_1;

/*-----------------------------------------------------------------------------
	0x0048 info_afbc_2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	info_afbc_0                     ;   	// 31: 0
} INFO_AFBC_2;

/*-----------------------------------------------------------------------------
	0x004c info_afbc_3 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	info_afbc_0                     ;   	// 31: 0
} INFO_AFBC_3;

/*-----------------------------------------------------------------------------
	0x0050 info_afbc_4 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	info_afbc_0                     ;   	// 31: 0
} INFO_AFBC_4;

/*-----------------------------------------------------------------------------
	0x0054 info_afbc_5 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	info_afbc_0                     ;   	// 31: 0
} INFO_AFBC_5;

/*-----------------------------------------------------------------------------
	0x0058 info_afbc_6 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	info_afbc_0                     ;   	// 31: 0
} INFO_AFBC_6;

/*-----------------------------------------------------------------------------
	0x005c info_afbc_7 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	info_afbc_0                     ;   	// 31: 0
} INFO_AFBC_7;

/*-----------------------------------------------------------------------------
	0x0060 info_afbc_8 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	info_afbc_0                     ;   	// 31: 0
} INFO_AFBC_8;

/*-----------------------------------------------------------------------------
	0x00e0 ctrl_lencon_cmd0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_lencon_sw_reset             : 1,	//     0
	reg_lencon_ready                : 1,	//     1
	reg_lencon_start                : 1,	//     2
	                                : 1,	//     3 reserved
	reg_lencon_start_yrptr          :18;	//  4:21
} CTRL_LENCON_CMD0;

/*-----------------------------------------------------------------------------
	0x00e4 ctrl_lencon_cmd1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_lencon_arid                 : 4,	//  0: 3
	                                :12,	//  4:15 reserved
	reg_lencon_stride               : 6;	// 16:21
} CTRL_LENCON_CMD1;

/*-----------------------------------------------------------------------------
	0x00e8 ctrl_lencon_cmd2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_lencon_startx               : 6,	//  0: 5
	                                :10,	//  6:15 reserved
	reg_lencon_endx                 : 6;	// 16:21
} CTRL_LENCON_CMD2;

/*-----------------------------------------------------------------------------
	0x00ec ctrl_lencon_cmd3 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_lencon_starty               :11,	//  0:10
	                                : 5,	// 11:15 reserved
	reg_lencon_endy                 :11;	// 16:26
} CTRL_LENCON_CMD3;

typedef struct {
	CTRL_AFBC_0                     	ctrl_afbc_0                     ;	// 0x0000 : ''
	CTRL_AFBC_1                     	ctrl_afbc_1                     ;	// 0x0004 : ''
	CTRL_AFBC_2                     	ctrl_afbc_2                     ;	// 0x0008 : ''
	CTRL_AFBC_3                     	ctrl_afbc_3                     ;	// 0x000c : ''
	CTRL_AFBC_4                     	ctrl_afbc_4                     ;	// 0x0010 : ''
	CTRL_AFBC_5                     	ctrl_afbc_5                     ;	// 0x0014 : ''
	CTRL_AFBC_6                     	ctrl_afbc_6                     ;	// 0x0018 : ''
	CTRL_AFBC_7                     	ctrl_afbc_7                     ;	// 0x001c : ''
	CTRL_AFBC_8                     	ctrl_afbc_8                     ;	// 0x0020 : ''
	CTRL_AFBC_9                     	ctrl_afbc_9                     ;	// 0x0024 : ''
	CTRL_AFBC_A                     	ctrl_afbc_a                     ;	// 0x0028 : ''
	CTRL_AFBC_B                     	ctrl_afbc_b                     ;	// 0x002c : ''
	CTRL_AFBC_C                     	ctrl_afbc_c                     ;	// 0x0030 : ''
	CTRL_AFBC_D                     	ctrl_afbc_d                     ;	// 0x0034 : ''
	CTRL_AFBC_E                     	ctrl_afbc_e                     ;	// 0x0038 : ''
	CTRL_AFBC_F                     	ctrl_afbc_f                     ;	// 0x003c : ''
	INFO_AFBC_0                     	info_afbc_0                     ;	// 0x0040 : ''
	INFO_AFBC_1                     	info_afbc_1                     ;	// 0x0044 : ''
	INFO_AFBC_2                     	info_afbc_2                     ;	// 0x0048 : ''
	INFO_AFBC_3                     	info_afbc_3                     ;	// 0x004c : ''
	INFO_AFBC_4                     	info_afbc_4                     ;	// 0x0050 : ''
	INFO_AFBC_5                     	info_afbc_5                     ;	// 0x0054 : ''
	INFO_AFBC_6                     	info_afbc_6                     ;	// 0x0058 : ''
	INFO_AFBC_7                     	info_afbc_7                     ;	// 0x005c : ''
	INFO_AFBC_8                     	info_afbc_8                     ;	// 0x0060 : ''
	UINT32                          	                 __rsvd_00[  31];	// 0x0064 ~ 0x00dc
	CTRL_LENCON_CMD0                	ctrl_lencon_cmd0                ;	// 0x00e0 : ''
	CTRL_LENCON_CMD1                	ctrl_lencon_cmd1                ;	// 0x00e4 : ''
	CTRL_LENCON_CMD2                	ctrl_lencon_cmd2                ;	// 0x00e8 : ''
	CTRL_LENCON_CMD3                	ctrl_lencon_cmd3                ;	// 0x00ec : ''
} OSD_AFBC_H15_REG_T;
/* 29 regs, 29 types */

/* 29 regs, 29 types in Total*/

/*
 * @{
 * Naming for register pointer.
 * g_osd_afbc_h15_reg : real register of OSD_AFBC_CTRL.
 * g_osd_afbc_h15_reg_cached     : shadow register.
 *
 * @def OSD_AFBC_CTRL_RdFL: Read  FLushing : Shadow <- Real.
 * @def OSD_AFBC_CTRL_WrFL: Write FLushing : Shadow -> Real.
 * @def OSD_AFBC_CTRL_Rd  : Read  whole register(UINT32) from Shadow register.
 * @def OSD_AFBC_CTRL_Wr  : Write whole register(UINT32) from Shadow register.
 * @def OSD_AFBC_CTRL_Rd01 ~ OSD_AFBC_CTRL_Rdnn: Read  given '01~nn' fields from Shadow register.
 * @def OSD_AFBC_CTRL_Wr01 ~ OSD_AFBC_CTRL_Wrnn: Write given '01~nn' fields to   Shadow register.
 * */
#define OSD_AFBC_CTRL_RdFL(_r)			((g_osd_afbc_h15_reg_cached->_r)=(g_osd_afbc_h15_reg->_r))
#define OSD_AFBC_CTRL_WrFL(_r)			((g_osd_afbc_h15_reg->_r)=(g_osd_afbc_h15_reg_cached->_r))

#define OSD_AFBC_CTRL_Rd(_r)			*((UINT32*)(&(g_osd_afbc_h15_reg_cached->_r)))
#define OSD_AFBC_CTRL_Wr(_r,_v)			((OSD_AFBC_CTRL_Rd(_r))=((UINT32)(_v)))

#define OSD_AFBC_CTRL_Rd01(_r,_f01,_v01)													\
								do { 											\
									(_v01) = (g_osd_afbc_h15_reg_cached->_r._f01);				\
								} while(0)

#define OSD_AFBC_CTRL_Rd02(_r,_f01,_v01,_f02,_v02)										\
								do { 											\
									(_v01) = (g_osd_afbc_h15_reg_cached->_r._f01);				\
									(_v02) = (g_osd_afbc_h15_reg_cached->_r._f02);				\
								} while(0)

#define OSD_AFBC_CTRL_Rd03(_r,_f01,_v01,_f02,_v02,_f03,_v03)								\
								do { 											\
									(_v01) = (g_osd_afbc_h15_reg_cached->_r._f01);				\
									(_v02) = (g_osd_afbc_h15_reg_cached->_r._f02);				\
									(_v03) = (g_osd_afbc_h15_reg_cached->_r._f03);				\
								} while(0)

#define OSD_AFBC_CTRL_Rd04(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04)					\
								do { 											\
									(_v01) = (g_osd_afbc_h15_reg_cached->_r._f01);				\
									(_v02) = (g_osd_afbc_h15_reg_cached->_r._f02);				\
									(_v03) = (g_osd_afbc_h15_reg_cached->_r._f03);				\
									(_v04) = (g_osd_afbc_h15_reg_cached->_r._f04);				\
								} while(0)

#define OSD_AFBC_CTRL_Rd05(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05)													\
								do { 											\
									(_v01) = (g_osd_afbc_h15_reg_cached->_r._f01);				\
									(_v02) = (g_osd_afbc_h15_reg_cached->_r._f02);				\
									(_v03) = (g_osd_afbc_h15_reg_cached->_r._f03);				\
									(_v04) = (g_osd_afbc_h15_reg_cached->_r._f04);				\
									(_v05) = (g_osd_afbc_h15_reg_cached->_r._f05);				\
								} while(0)

#define OSD_AFBC_CTRL_Rd06(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06)										\
								do { 											\
									(_v01) = (g_osd_afbc_h15_reg_cached->_r._f01);				\
									(_v02) = (g_osd_afbc_h15_reg_cached->_r._f02);				\
									(_v03) = (g_osd_afbc_h15_reg_cached->_r._f03);				\
									(_v04) = (g_osd_afbc_h15_reg_cached->_r._f04);				\
									(_v05) = (g_osd_afbc_h15_reg_cached->_r._f05);				\
									(_v06) = (g_osd_afbc_h15_reg_cached->_r._f06);				\
								} while(0)

#define OSD_AFBC_CTRL_Rd07(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07)								\
								do { 											\
									(_v01) = (g_osd_afbc_h15_reg_cached->_r._f01);				\
									(_v02) = (g_osd_afbc_h15_reg_cached->_r._f02);				\
									(_v03) = (g_osd_afbc_h15_reg_cached->_r._f03);				\
									(_v04) = (g_osd_afbc_h15_reg_cached->_r._f04);				\
									(_v05) = (g_osd_afbc_h15_reg_cached->_r._f05);				\
									(_v06) = (g_osd_afbc_h15_reg_cached->_r._f06);				\
									(_v07) = (g_osd_afbc_h15_reg_cached->_r._f07);				\
								} while(0)

#define OSD_AFBC_CTRL_Rd08(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08)					\
								do { 											\
									(_v01) = (g_osd_afbc_h15_reg_cached->_r._f01);				\
									(_v02) = (g_osd_afbc_h15_reg_cached->_r._f02);				\
									(_v03) = (g_osd_afbc_h15_reg_cached->_r._f03);				\
									(_v04) = (g_osd_afbc_h15_reg_cached->_r._f04);				\
									(_v05) = (g_osd_afbc_h15_reg_cached->_r._f05);				\
									(_v06) = (g_osd_afbc_h15_reg_cached->_r._f06);				\
									(_v07) = (g_osd_afbc_h15_reg_cached->_r._f07);				\
									(_v08) = (g_osd_afbc_h15_reg_cached->_r._f08);				\
								} while(0)

#define OSD_AFBC_CTRL_Rd09(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09)													\
								do { 											\
									(_v01) = (g_osd_afbc_h15_reg_cached->_r._f01);				\
									(_v02) = (g_osd_afbc_h15_reg_cached->_r._f02);				\
									(_v03) = (g_osd_afbc_h15_reg_cached->_r._f03);				\
									(_v04) = (g_osd_afbc_h15_reg_cached->_r._f04);				\
									(_v05) = (g_osd_afbc_h15_reg_cached->_r._f05);				\
									(_v06) = (g_osd_afbc_h15_reg_cached->_r._f06);				\
									(_v07) = (g_osd_afbc_h15_reg_cached->_r._f07);				\
									(_v08) = (g_osd_afbc_h15_reg_cached->_r._f08);				\
									(_v09) = (g_osd_afbc_h15_reg_cached->_r._f09);				\
								} while(0)

#define OSD_AFBC_CTRL_Rd10(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10)										\
								do { 											\
									(_v01) = (g_osd_afbc_h15_reg_cached->_r._f01);				\
									(_v02) = (g_osd_afbc_h15_reg_cached->_r._f02);				\
									(_v03) = (g_osd_afbc_h15_reg_cached->_r._f03);				\
									(_v04) = (g_osd_afbc_h15_reg_cached->_r._f04);				\
									(_v05) = (g_osd_afbc_h15_reg_cached->_r._f05);				\
									(_v06) = (g_osd_afbc_h15_reg_cached->_r._f06);				\
									(_v07) = (g_osd_afbc_h15_reg_cached->_r._f07);				\
									(_v08) = (g_osd_afbc_h15_reg_cached->_r._f08);				\
									(_v09) = (g_osd_afbc_h15_reg_cached->_r._f09);				\
									(_v10) = (g_osd_afbc_h15_reg_cached->_r._f10);				\
								} while(0)

#define OSD_AFBC_CTRL_Rd11(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11)								\
								do { 											\
									(_v01) = (g_osd_afbc_h15_reg_cached->_r._f01);				\
									(_v02) = (g_osd_afbc_h15_reg_cached->_r._f02);				\
									(_v03) = (g_osd_afbc_h15_reg_cached->_r._f03);				\
									(_v04) = (g_osd_afbc_h15_reg_cached->_r._f04);				\
									(_v05) = (g_osd_afbc_h15_reg_cached->_r._f05);				\
									(_v06) = (g_osd_afbc_h15_reg_cached->_r._f06);				\
									(_v07) = (g_osd_afbc_h15_reg_cached->_r._f07);				\
									(_v08) = (g_osd_afbc_h15_reg_cached->_r._f08);				\
									(_v09) = (g_osd_afbc_h15_reg_cached->_r._f09);				\
									(_v10) = (g_osd_afbc_h15_reg_cached->_r._f10);				\
									(_v11) = (g_osd_afbc_h15_reg_cached->_r._f11);				\
								} while(0)

#define OSD_AFBC_CTRL_Rd12(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12)					\
								do { 											\
									(_v01) = (g_osd_afbc_h15_reg_cached->_r._f01);				\
									(_v02) = (g_osd_afbc_h15_reg_cached->_r._f02);				\
									(_v03) = (g_osd_afbc_h15_reg_cached->_r._f03);				\
									(_v04) = (g_osd_afbc_h15_reg_cached->_r._f04);				\
									(_v05) = (g_osd_afbc_h15_reg_cached->_r._f05);				\
									(_v06) = (g_osd_afbc_h15_reg_cached->_r._f06);				\
									(_v07) = (g_osd_afbc_h15_reg_cached->_r._f07);				\
									(_v08) = (g_osd_afbc_h15_reg_cached->_r._f08);				\
									(_v09) = (g_osd_afbc_h15_reg_cached->_r._f09);				\
									(_v10) = (g_osd_afbc_h15_reg_cached->_r._f10);				\
									(_v11) = (g_osd_afbc_h15_reg_cached->_r._f11);				\
									(_v12) = (g_osd_afbc_h15_reg_cached->_r._f12);				\
								} while(0)

#define OSD_AFBC_CTRL_Rd13(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13)													\
								do { 											\
									(_v01) = (g_osd_afbc_h15_reg_cached->_r._f01);				\
									(_v02) = (g_osd_afbc_h15_reg_cached->_r._f02);				\
									(_v03) = (g_osd_afbc_h15_reg_cached->_r._f03);				\
									(_v04) = (g_osd_afbc_h15_reg_cached->_r._f04);				\
									(_v05) = (g_osd_afbc_h15_reg_cached->_r._f05);				\
									(_v06) = (g_osd_afbc_h15_reg_cached->_r._f06);				\
									(_v07) = (g_osd_afbc_h15_reg_cached->_r._f07);				\
									(_v08) = (g_osd_afbc_h15_reg_cached->_r._f08);				\
									(_v09) = (g_osd_afbc_h15_reg_cached->_r._f09);				\
									(_v10) = (g_osd_afbc_h15_reg_cached->_r._f10);				\
									(_v11) = (g_osd_afbc_h15_reg_cached->_r._f11);				\
									(_v12) = (g_osd_afbc_h15_reg_cached->_r._f12);				\
									(_v13) = (g_osd_afbc_h15_reg_cached->_r._f13);				\
								} while(0)

#define OSD_AFBC_CTRL_Rd14(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14)										\
								do { 											\
									(_v01) = (g_osd_afbc_h15_reg_cached->_r._f01);				\
									(_v02) = (g_osd_afbc_h15_reg_cached->_r._f02);				\
									(_v03) = (g_osd_afbc_h15_reg_cached->_r._f03);				\
									(_v04) = (g_osd_afbc_h15_reg_cached->_r._f04);				\
									(_v05) = (g_osd_afbc_h15_reg_cached->_r._f05);				\
									(_v06) = (g_osd_afbc_h15_reg_cached->_r._f06);				\
									(_v07) = (g_osd_afbc_h15_reg_cached->_r._f07);				\
									(_v08) = (g_osd_afbc_h15_reg_cached->_r._f08);				\
									(_v09) = (g_osd_afbc_h15_reg_cached->_r._f09);				\
									(_v10) = (g_osd_afbc_h15_reg_cached->_r._f10);				\
									(_v11) = (g_osd_afbc_h15_reg_cached->_r._f11);				\
									(_v12) = (g_osd_afbc_h15_reg_cached->_r._f12);				\
									(_v13) = (g_osd_afbc_h15_reg_cached->_r._f13);				\
									(_v14) = (g_osd_afbc_h15_reg_cached->_r._f14);				\
								} while(0)

#define OSD_AFBC_CTRL_Rd15(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15)								\
								do { 											\
									(_v01) = (g_osd_afbc_h15_reg_cached->_r._f01);				\
									(_v02) = (g_osd_afbc_h15_reg_cached->_r._f02);				\
									(_v03) = (g_osd_afbc_h15_reg_cached->_r._f03);				\
									(_v04) = (g_osd_afbc_h15_reg_cached->_r._f04);				\
									(_v05) = (g_osd_afbc_h15_reg_cached->_r._f05);				\
									(_v06) = (g_osd_afbc_h15_reg_cached->_r._f06);				\
									(_v07) = (g_osd_afbc_h15_reg_cached->_r._f07);				\
									(_v08) = (g_osd_afbc_h15_reg_cached->_r._f08);				\
									(_v09) = (g_osd_afbc_h15_reg_cached->_r._f09);				\
									(_v10) = (g_osd_afbc_h15_reg_cached->_r._f10);				\
									(_v11) = (g_osd_afbc_h15_reg_cached->_r._f11);				\
									(_v12) = (g_osd_afbc_h15_reg_cached->_r._f12);				\
									(_v13) = (g_osd_afbc_h15_reg_cached->_r._f13);				\
									(_v14) = (g_osd_afbc_h15_reg_cached->_r._f14);				\
									(_v15) = (g_osd_afbc_h15_reg_cached->_r._f15);				\
								} while(0)

#define OSD_AFBC_CTRL_Rd16(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15,_f16,_v16)					\
								do { 											\
									(_v01) = (g_osd_afbc_h15_reg_cached->_r._f01);				\
									(_v02) = (g_osd_afbc_h15_reg_cached->_r._f02);				\
									(_v03) = (g_osd_afbc_h15_reg_cached->_r._f03);				\
									(_v04) = (g_osd_afbc_h15_reg_cached->_r._f04);				\
									(_v05) = (g_osd_afbc_h15_reg_cached->_r._f05);				\
									(_v06) = (g_osd_afbc_h15_reg_cached->_r._f06);				\
									(_v07) = (g_osd_afbc_h15_reg_cached->_r._f07);				\
									(_v08) = (g_osd_afbc_h15_reg_cached->_r._f08);				\
									(_v09) = (g_osd_afbc_h15_reg_cached->_r._f09);				\
									(_v10) = (g_osd_afbc_h15_reg_cached->_r._f10);				\
									(_v11) = (g_osd_afbc_h15_reg_cached->_r._f11);				\
									(_v12) = (g_osd_afbc_h15_reg_cached->_r._f12);				\
									(_v13) = (g_osd_afbc_h15_reg_cached->_r._f13);				\
									(_v14) = (g_osd_afbc_h15_reg_cached->_r._f14);				\
									(_v15) = (g_osd_afbc_h15_reg_cached->_r._f15);				\
									(_v16) = (g_osd_afbc_h15_reg_cached->_r._f16);				\
								} while(0)


#define OSD_AFBC_CTRL_Wr01(_r,_f01,_v01)													\
								do { 											\
									(g_osd_afbc_h15_reg_cached->_r._f01) = (_v01);				\
								} while(0)

#define OSD_AFBC_CTRL_Wr02(_r,_f01,_v01,_f02,_v02)										\
								do { 											\
									(g_osd_afbc_h15_reg_cached->_r._f01) = (_v01);				\
									(g_osd_afbc_h15_reg_cached->_r._f02) = (_v02);				\
								} while(0)

#define OSD_AFBC_CTRL_Wr03(_r,_f01,_v01,_f02,_v02,_f03,_v03)								\
								do { 											\
									(g_osd_afbc_h15_reg_cached->_r._f01) = (_v01);				\
									(g_osd_afbc_h15_reg_cached->_r._f02) = (_v02);				\
									(g_osd_afbc_h15_reg_cached->_r._f03) = (_v03);				\
								} while(0)

#define OSD_AFBC_CTRL_Wr04(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04)					\
								do { 											\
									(g_osd_afbc_h15_reg_cached->_r._f01) = (_v01);				\
									(g_osd_afbc_h15_reg_cached->_r._f02) = (_v02);				\
									(g_osd_afbc_h15_reg_cached->_r._f03) = (_v03);				\
									(g_osd_afbc_h15_reg_cached->_r._f04) = (_v04);				\
								} while(0)

#define OSD_AFBC_CTRL_Wr05(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05)													\
								do { 											\
									(g_osd_afbc_h15_reg_cached->_r._f01) = (_v01);				\
									(g_osd_afbc_h15_reg_cached->_r._f02) = (_v02);				\
									(g_osd_afbc_h15_reg_cached->_r._f03) = (_v03);				\
									(g_osd_afbc_h15_reg_cached->_r._f04) = (_v04);				\
									(g_osd_afbc_h15_reg_cached->_r._f05) = (_v05);				\
								} while(0)

#define OSD_AFBC_CTRL_Wr06(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06)										\
								do { 											\
									(g_osd_afbc_h15_reg_cached->_r._f01) = (_v01);				\
									(g_osd_afbc_h15_reg_cached->_r._f02) = (_v02);				\
									(g_osd_afbc_h15_reg_cached->_r._f03) = (_v03);				\
									(g_osd_afbc_h15_reg_cached->_r._f04) = (_v04);				\
									(g_osd_afbc_h15_reg_cached->_r._f05) = (_v05);				\
									(g_osd_afbc_h15_reg_cached->_r._f06) = (_v06);				\
								} while(0)

#define OSD_AFBC_CTRL_Wr07(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07)								\
								do { 											\
									(g_osd_afbc_h15_reg_cached->_r._f01) = (_v01);				\
									(g_osd_afbc_h15_reg_cached->_r._f02) = (_v02);				\
									(g_osd_afbc_h15_reg_cached->_r._f03) = (_v03);				\
									(g_osd_afbc_h15_reg_cached->_r._f04) = (_v04);				\
									(g_osd_afbc_h15_reg_cached->_r._f05) = (_v05);				\
									(g_osd_afbc_h15_reg_cached->_r._f06) = (_v06);				\
									(g_osd_afbc_h15_reg_cached->_r._f07) = (_v07);				\
								} while(0)

#define OSD_AFBC_CTRL_Wr08(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08)					\
								do { 											\
									(g_osd_afbc_h15_reg_cached->_r._f01) = (_v01);				\
									(g_osd_afbc_h15_reg_cached->_r._f02) = (_v02);				\
									(g_osd_afbc_h15_reg_cached->_r._f03) = (_v03);				\
									(g_osd_afbc_h15_reg_cached->_r._f04) = (_v04);				\
									(g_osd_afbc_h15_reg_cached->_r._f05) = (_v05);				\
									(g_osd_afbc_h15_reg_cached->_r._f06) = (_v06);				\
									(g_osd_afbc_h15_reg_cached->_r._f07) = (_v07);				\
									(g_osd_afbc_h15_reg_cached->_r._f08) = (_v08);				\
								} while(0)

#define OSD_AFBC_CTRL_Wr09(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09)													\
								do { 											\
									(g_osd_afbc_h15_reg_cached->_r._f01) = (_v01);				\
									(g_osd_afbc_h15_reg_cached->_r._f02) = (_v02);				\
									(g_osd_afbc_h15_reg_cached->_r._f03) = (_v03);				\
									(g_osd_afbc_h15_reg_cached->_r._f04) = (_v04);				\
									(g_osd_afbc_h15_reg_cached->_r._f05) = (_v05);				\
									(g_osd_afbc_h15_reg_cached->_r._f06) = (_v06);				\
									(g_osd_afbc_h15_reg_cached->_r._f07) = (_v07);				\
									(g_osd_afbc_h15_reg_cached->_r._f08) = (_v08);				\
									(g_osd_afbc_h15_reg_cached->_r._f09) = (_v09);				\
								} while(0)

#define OSD_AFBC_CTRL_Wr10(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10)										\
								do { 											\
									(g_osd_afbc_h15_reg_cached->_r._f01) = (_v01);				\
									(g_osd_afbc_h15_reg_cached->_r._f02) = (_v02);				\
									(g_osd_afbc_h15_reg_cached->_r._f03) = (_v03);				\
									(g_osd_afbc_h15_reg_cached->_r._f04) = (_v04);				\
									(g_osd_afbc_h15_reg_cached->_r._f05) = (_v05);				\
									(g_osd_afbc_h15_reg_cached->_r._f06) = (_v06);				\
									(g_osd_afbc_h15_reg_cached->_r._f07) = (_v07);				\
									(g_osd_afbc_h15_reg_cached->_r._f08) = (_v08);				\
									(g_osd_afbc_h15_reg_cached->_r._f09) = (_v09);				\
									(g_osd_afbc_h15_reg_cached->_r._f10) = (_v10);				\
								} while(0)

#define OSD_AFBC_CTRL_Wr11(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11)								\
								do { 											\
									(g_osd_afbc_h15_reg_cached->_r._f01) = (_v01);				\
									(g_osd_afbc_h15_reg_cached->_r._f02) = (_v02);				\
									(g_osd_afbc_h15_reg_cached->_r._f03) = (_v03);				\
									(g_osd_afbc_h15_reg_cached->_r._f04) = (_v04);				\
									(g_osd_afbc_h15_reg_cached->_r._f05) = (_v05);				\
									(g_osd_afbc_h15_reg_cached->_r._f06) = (_v06);				\
									(g_osd_afbc_h15_reg_cached->_r._f07) = (_v07);				\
									(g_osd_afbc_h15_reg_cached->_r._f08) = (_v08);				\
									(g_osd_afbc_h15_reg_cached->_r._f09) = (_v09);				\
									(g_osd_afbc_h15_reg_cached->_r._f10) = (_v10);				\
									(g_osd_afbc_h15_reg_cached->_r._f11) = (_v11);				\
								} while(0)

#define OSD_AFBC_CTRL_Wr12(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12)					\
								do { 											\
									(g_osd_afbc_h15_reg_cached->_r._f01) = (_v01);				\
									(g_osd_afbc_h15_reg_cached->_r._f02) = (_v02);				\
									(g_osd_afbc_h15_reg_cached->_r._f03) = (_v03);				\
									(g_osd_afbc_h15_reg_cached->_r._f04) = (_v04);				\
									(g_osd_afbc_h15_reg_cached->_r._f05) = (_v05);				\
									(g_osd_afbc_h15_reg_cached->_r._f06) = (_v06);				\
									(g_osd_afbc_h15_reg_cached->_r._f07) = (_v07);				\
									(g_osd_afbc_h15_reg_cached->_r._f08) = (_v08);				\
									(g_osd_afbc_h15_reg_cached->_r._f09) = (_v09);				\
									(g_osd_afbc_h15_reg_cached->_r._f10) = (_v10);				\
									(g_osd_afbc_h15_reg_cached->_r._f11) = (_v11);				\
									(g_osd_afbc_h15_reg_cached->_r._f12) = (_v12);				\
								} while(0)

#define OSD_AFBC_CTRL_Wr13(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13)													\
								do { 											\
									(g_osd_afbc_h15_reg_cached->_r._f01) = (_v01);				\
									(g_osd_afbc_h15_reg_cached->_r._f02) = (_v02);				\
									(g_osd_afbc_h15_reg_cached->_r._f03) = (_v03);				\
									(g_osd_afbc_h15_reg_cached->_r._f04) = (_v04);				\
									(g_osd_afbc_h15_reg_cached->_r._f05) = (_v05);				\
									(g_osd_afbc_h15_reg_cached->_r._f06) = (_v06);				\
									(g_osd_afbc_h15_reg_cached->_r._f07) = (_v07);				\
									(g_osd_afbc_h15_reg_cached->_r._f08) = (_v08);				\
									(g_osd_afbc_h15_reg_cached->_r._f09) = (_v09);				\
									(g_osd_afbc_h15_reg_cached->_r._f10) = (_v10);				\
									(g_osd_afbc_h15_reg_cached->_r._f11) = (_v11);				\
									(g_osd_afbc_h15_reg_cached->_r._f12) = (_v12);				\
									(g_osd_afbc_h15_reg_cached->_r._f13) = (_v13);				\
								} while(0)

#define OSD_AFBC_CTRL_Wr14(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14)										\
								do { 											\
									(g_osd_afbc_h15_reg_cached->_r._f01) = (_v01);				\
									(g_osd_afbc_h15_reg_cached->_r._f02) = (_v02);				\
									(g_osd_afbc_h15_reg_cached->_r._f03) = (_v03);				\
									(g_osd_afbc_h15_reg_cached->_r._f04) = (_v04);				\
									(g_osd_afbc_h15_reg_cached->_r._f05) = (_v05);				\
									(g_osd_afbc_h15_reg_cached->_r._f06) = (_v06);				\
									(g_osd_afbc_h15_reg_cached->_r._f07) = (_v07);				\
									(g_osd_afbc_h15_reg_cached->_r._f08) = (_v08);				\
									(g_osd_afbc_h15_reg_cached->_r._f09) = (_v09);				\
									(g_osd_afbc_h15_reg_cached->_r._f10) = (_v10);				\
									(g_osd_afbc_h15_reg_cached->_r._f11) = (_v11);				\
									(g_osd_afbc_h15_reg_cached->_r._f12) = (_v12);				\
									(g_osd_afbc_h15_reg_cached->_r._f13) = (_v13);				\
									(g_osd_afbc_h15_reg_cached->_r._f14) = (_v14);				\
								} while(0)

#define OSD_AFBC_CTRL_Wr15(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15)								\
								do { 											\
									(g_osd_afbc_h15_reg_cached->_r._f01) = (_v01);				\
									(g_osd_afbc_h15_reg_cached->_r._f02) = (_v02);				\
									(g_osd_afbc_h15_reg_cached->_r._f03) = (_v03);				\
									(g_osd_afbc_h15_reg_cached->_r._f04) = (_v04);				\
									(g_osd_afbc_h15_reg_cached->_r._f05) = (_v05);				\
									(g_osd_afbc_h15_reg_cached->_r._f06) = (_v06);				\
									(g_osd_afbc_h15_reg_cached->_r._f07) = (_v07);				\
									(g_osd_afbc_h15_reg_cached->_r._f08) = (_v08);				\
									(g_osd_afbc_h15_reg_cached->_r._f09) = (_v09);				\
									(g_osd_afbc_h15_reg_cached->_r._f10) = (_v10);				\
									(g_osd_afbc_h15_reg_cached->_r._f11) = (_v11);				\
									(g_osd_afbc_h15_reg_cached->_r._f12) = (_v12);				\
									(g_osd_afbc_h15_reg_cached->_r._f13) = (_v13);				\
									(g_osd_afbc_h15_reg_cached->_r._f14) = (_v14);				\
									(g_osd_afbc_h15_reg_cached->_r._f15) = (_v15);				\
								} while(0)

#define OSD_AFBC_CTRL_Wr16(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15,_f16,_v16)					\
								do { 											\
									(g_osd_afbc_h15_reg_cached->_r._f01) = (_v01);				\
									(g_osd_afbc_h15_reg_cached->_r._f02) = (_v02);				\
									(g_osd_afbc_h15_reg_cached->_r._f03) = (_v03);				\
									(g_osd_afbc_h15_reg_cached->_r._f04) = (_v04);				\
									(g_osd_afbc_h15_reg_cached->_r._f05) = (_v05);				\
									(g_osd_afbc_h15_reg_cached->_r._f06) = (_v06);				\
									(g_osd_afbc_h15_reg_cached->_r._f07) = (_v07);				\
									(g_osd_afbc_h15_reg_cached->_r._f08) = (_v08);				\
									(g_osd_afbc_h15_reg_cached->_r._f09) = (_v09);				\
									(g_osd_afbc_h15_reg_cached->_r._f10) = (_v10);				\
									(g_osd_afbc_h15_reg_cached->_r._f11) = (_v11);				\
									(g_osd_afbc_h15_reg_cached->_r._f12) = (_v12);				\
									(g_osd_afbc_h15_reg_cached->_r._f13) = (_v13);				\
									(g_osd_afbc_h15_reg_cached->_r._f14) = (_v14);				\
									(g_osd_afbc_h15_reg_cached->_r._f15) = (_v15);				\
									(g_osd_afbc_h15_reg_cached->_r._f16) = (_v16);				\
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
 * _rwname: name of rw    bit field : shall be 0 after OSD_AFBC_CTRL_Wind(), 1 for OSD_AFBC_CTRL_Rind()
 * _iname : name of index bit field
 * _ival  : index value
 * _fname : field name
 * _fval  : field variable that field value shall be stored.
 *
 * OSD_AFBC_CTRL_Rind : General indexed register Read.(
 * OSD_AFBC_CTRL_Wind : General indexed register Read.
 *
 * OSD_AFBC_CTRL_Ridx : For 'index', 'rw', 'load' field name
 * OSD_AFBC_CTRL_Widx : For 'index', 'rw', 'load' field name and NO_LOAD.
 */
#define OSD_AFBC_CTRL_Rind(_r, _lname, _rwname, _iname, _ival, _fname, _fval)				\
							do {												\
								OSD_AFBC_CTRL_Wr03(_r,_lname,0,_rwname,1,_iname,_ival);	\
								OSD_AFBC_CTRL_WrFL(_r);									\
								OSD_AFBC_CTRL_RdFL(_r);									\
								OSD_AFBC_CTRL_Rd01(_r,_fname,_fval);						\
							} while (0)

#define OSD_AFBC_CTRL_Wind(_r, _lname, _rwname, _iname, _ival, _fname, _fval)				\
				OSD_AFBC_CTRL_Wr04(_r, _lname,0, _rwname,0, _iname,_ival, _fname,_fval)


#define OSD_AFBC_CTRL_Ridx(_r, _ival, _fname, _fval)	OSD_AFBC_CTRL_Rind(_r,load,rw,index,_ival,_fname,_fval)

#define OSD_AFBC_CTRL_Widx(_r, _ival, _fname, _fval)	OSD_AFBC_CTRL_Wind(_r,load,rw,index,_ival,_fname,_fval)

/** @} *//* end of macro documentation */

#ifdef __cplusplus
}
#endif

#endif	/* _#MOD#_REG_H_ */

/* from 'H15A0_RegFile_DE_OSD_AFBC.csv'  대한민국 표준시 by getregs v2.8 */
