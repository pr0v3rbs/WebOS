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

/** @file
 *
 *  #MOD# register details. ( used only within kdriver )
 *
 *  author     user name (user_name@lge.com)
 *  version    1.0
 *  date       2010.xx.xx
 *
 */

#ifndef _RC1_REG_H_
#define _RC1_REG_H_

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
	0x0add reg_name ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x0000 version ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	yyyymmdd                        ;   	// 31: 0
} RC1_VERSION;

/*-----------------------------------------------------------------------------
	0x0004 axi_conf ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_debug_sel                   : 3,	//  0: 2
	                                : 1,	//     3 reserved
	reg_mach0_pri_nama_en           : 1,	//     4
	reg_mach0_sec_nama_en           : 1,	//     5
	reg_mach1_pri_nama_en           : 1,	//     6
	reg_mach1_sec_nama_en           : 1,	//     7
	reg_pdec_nama_en                : 1,	//     8
	                                : 3,	//  9:11 reserved
	reg_mach0_pri_st_nama           : 1,	//    12
	reg_mach0_sec_st_nama           : 1,	//    13
	reg_mach1_pri_st_nama           : 1,	//    14
	reg_mach1_sec_st_nama           : 1,	//    15
	reg_pdec_st_nama                : 1,	//    16
	                                : 3,	// 17:19 reserved
	reg_mach0_pri_axi_locked_dis    : 1,	//    20
	reg_mach0_sec_axi_locked_dis    : 1,	//    21
	reg_mach1_pri_axi_locked_dis    : 1,	//    22
	reg_mach1_sec_axi_locked_dis    : 1,	//    23
	sw_reset_tm0p                   : 1,	//    24
	sw_reset_tm0s                   : 1,	//    25
	                                : 1,	//    26 reserved
	sw_reset_tm1p                   : 1,	//    27
	sw_reset_tm1s                   : 1,	//    28
	sw_reset_tpdec                  : 1,	//    29
	sw_reset_nama                   : 1;	//    30
} AXI_CONF;

/*-----------------------------------------------------------------------------
	0x0010 axi_user0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_awuser0                     : 2,	//  0: 1
	                                : 2,	//  2: 3 reserved
	reg_awuser0_arid_mask           : 4,	//  4: 7
	reg_awuser0_arid                : 4,	//  8:11
	reg_awuser0_en                  : 1,	//    12
	                                : 3,	// 13:15 reserved
	reg_aruser0                     : 2,	// 16:17
	                                : 2,	// 18:19 reserved
	reg_aruser0_arid_mask           : 4,	// 20:23
	reg_aruser0_arid                : 4,	// 24:27
	reg_aruser0_en                  : 1;	//    28
} AXI_USER0;

/*-----------------------------------------------------------------------------
	0x0014 axi_user1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_awuser1                     : 2,	//  0: 1
	                                : 2,	//  2: 3 reserved
	reg_awuser1_arid_mask           : 4,	//  4: 7
	reg_awuser1_arid                : 4,	//  8:11
	reg_awuser1_en                  : 1,	//    12
	                                : 3,	// 13:15 reserved
	reg_aruser1                     : 2,	// 16:17
	                                : 2,	// 18:19 reserved
	reg_aruser1_arid_mask           : 4,	// 20:23
	reg_aruser1_arid                : 4,	// 24:27
	reg_aruser1_en                  : 1;	//    28
} AXI_USER1;

/*-----------------------------------------------------------------------------
	0x0018 axi_user2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_awuser2                     : 2,	//  0: 1
	                                : 2,	//  2: 3 reserved
	reg_awuser2_arid_mask           : 4,	//  4: 7
	reg_awuser2_arid                : 4,	//  8:11
	reg_awuser2_en                  : 1,	//    12
	                                : 3,	// 13:15 reserved
	reg_aruser2                     : 2,	// 16:17
	                                : 2,	// 18:19 reserved
	reg_aruser2_arid_mask           : 4,	// 20:23
	reg_aruser2_arid                : 4,	// 24:27
	reg_aruser2_en                  : 1;	//    28
} AXI_USER2;

/*-----------------------------------------------------------------------------
	0x0030 axi_sptr ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_sptr                        ;   	// 31: 0
} AXI_SPTR;

/*-----------------------------------------------------------------------------
	0x0034 axi_eptr ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_eptr                        ;   	// 31: 0
} AXI_EPTR;

/*-----------------------------------------------------------------------------
	0x0040 axi_st_aw_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_st_aw_ctrl                  ;   	// 31: 0
} AXI_ST_AW_CTRL;

/*-----------------------------------------------------------------------------
	0x0044 axi_st_aw_addr ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_st_aw_addr                  ;   	// 31: 0
} AXI_ST_AW_ADDR;

/*-----------------------------------------------------------------------------
	0x0048 axi_st_dw_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_st_dw_ctrl                  ;   	// 31: 0
} AXI_ST_DW_CTRL;

/*-----------------------------------------------------------------------------
	0x004c axi_st_db_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_st_db_ctrl                  ;   	// 31: 0
} AXI_ST_DB_CTRL;

/*-----------------------------------------------------------------------------
	0x0050 axi_st_ar_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_st_ar_ctrl                  ;   	// 31: 0
} AXI_ST_AR_CTRL;

/*-----------------------------------------------------------------------------
	0x0054 axi_st_ar_addr ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_st_ar_addr                  ;   	// 31: 0
} AXI_ST_AR_ADDR;

/*-----------------------------------------------------------------------------
	0x0058 axi_st_dr_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_st_dr_ctrl                  ;   	// 31: 0
} AXI_ST_DR_CTRL;

/*-----------------------------------------------------------------------------
	0x005c axi_st_wlen ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_st_wlen                     ;   	// 31: 0
} AXI_ST_WLEN;

/*-----------------------------------------------------------------------------
	0x0060 mnt_trigger ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_mnt_trigger                 : 3;	//  0: 2
} MNT_TRIGGER;


typedef struct {
	RC1_VERSION                     	version                         ;	// 0x0000 : ''
	AXI_CONF                        	axi_conf                        ;	// 0x0004 : ''
	UINT32                          	                 __rsvd_00[   2];	// 0x0008 ~ 0x000c
	AXI_USER0                       	axi_user0                       ;	// 0x0010 : ''
	AXI_USER1                       	axi_user1                       ;	// 0x0014 : ''
	AXI_USER2                       	axi_user2                       ;	// 0x0018 : ''
	UINT32                          	                 __rsvd_01[   5];	// 0x001c ~ 0x002c
	AXI_SPTR                        	axi_sptr                        ;	// 0x0030 : ''
	AXI_EPTR                        	axi_eptr                        ;	// 0x0034 : ''
	UINT32                          	                 __rsvd_02[   2];	// 0x0038 ~ 0x003c
	AXI_ST_AW_CTRL                  	axi_st_aw_ctrl                  ;	// 0x0040 : ''
	AXI_ST_AW_ADDR                  	axi_st_aw_addr                  ;	// 0x0044 : ''
	AXI_ST_DW_CTRL                  	axi_st_dw_ctrl                  ;	// 0x0048 : ''
	AXI_ST_DB_CTRL                  	axi_st_db_ctrl                  ;	// 0x004c : ''
	AXI_ST_AR_CTRL                  	axi_st_ar_ctrl                  ;	// 0x0050 : ''
	AXI_ST_AR_ADDR                  	axi_st_ar_addr                  ;	// 0x0054 : ''
	AXI_ST_DR_CTRL                  	axi_st_dr_ctrl                  ;	// 0x0058 : ''
	AXI_ST_WLEN                     	axi_st_wlen                     ;	// 0x005c : ''
	MNT_TRIGGER                     	mnt_trigger                     ;	// 0x0060 : ''
} RC1_REG_T;
/* 16 regs, 16 types */

/* 17 regs, 17 types in Total*/

/*
 * @{
 * Naming for register pointer.
 * gpRealRegRC1 : real register of RC1.
 * gpRegRC1     : shadow register.
 *
 * @def RC1_RdFL: Read  FLushing : Shadow <- Real.
 * @def RC1_WrFL: Write FLushing : Shadow -> Real.
 * @def RC1_Rd  : Read  whole register(UINT32) from Shadow register.
 * @def RC1_Wr  : Write whole register(UINT32) from Shadow register.
 * @def RC1_Rd01 ~ RC1_Rdnn: Read  given '01~nn' fields from Shadow register.
 * @def RC1_Wr01 ~ RC1_Wrnn: Write given '01~nn' fields to   Shadow register.
 * */
#define RC1_RdFL(_r)			((gpRegRC1->_r)=(gpRealRegRC1->_r))
#define RC1_WrFL(_r)			((gpRealRegRC1->_r)=(gpRegRC1->_r))

#define RC1_Rd(_r)			*((UINT32*)(&(gpRegRC1->_r)))
#define RC1_Wr(_r,_v)			((RC1_Rd(_r))=((UINT32)(_v)))

#define RC1_Rd01(_r,_f01,_v01)													\
								do { 											\
									(_v01) = (gpRegRC1->_r._f01);				\
								} while(0)

#define RC1_Rd02(_r,_f01,_v01,_f02,_v02)										\
								do { 											\
									(_v01) = (gpRegRC1->_r._f01);				\
									(_v02) = (gpRegRC1->_r._f02);				\
								} while(0)

#define RC1_Rd03(_r,_f01,_v01,_f02,_v02,_f03,_v03)								\
								do { 											\
									(_v01) = (gpRegRC1->_r._f01);				\
									(_v02) = (gpRegRC1->_r._f02);				\
									(_v03) = (gpRegRC1->_r._f03);				\
								} while(0)

#define RC1_Rd04(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04)					\
								do { 											\
									(_v01) = (gpRegRC1->_r._f01);				\
									(_v02) = (gpRegRC1->_r._f02);				\
									(_v03) = (gpRegRC1->_r._f03);				\
									(_v04) = (gpRegRC1->_r._f04);				\
								} while(0)

#define RC1_Rd05(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05)													\
								do { 											\
									(_v01) = (gpRegRC1->_r._f01);				\
									(_v02) = (gpRegRC1->_r._f02);				\
									(_v03) = (gpRegRC1->_r._f03);				\
									(_v04) = (gpRegRC1->_r._f04);				\
									(_v05) = (gpRegRC1->_r._f05);				\
								} while(0)

#define RC1_Rd06(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06)										\
								do { 											\
									(_v01) = (gpRegRC1->_r._f01);				\
									(_v02) = (gpRegRC1->_r._f02);				\
									(_v03) = (gpRegRC1->_r._f03);				\
									(_v04) = (gpRegRC1->_r._f04);				\
									(_v05) = (gpRegRC1->_r._f05);				\
									(_v06) = (gpRegRC1->_r._f06);				\
								} while(0)

#define RC1_Rd07(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07)								\
								do { 											\
									(_v01) = (gpRegRC1->_r._f01);				\
									(_v02) = (gpRegRC1->_r._f02);				\
									(_v03) = (gpRegRC1->_r._f03);				\
									(_v04) = (gpRegRC1->_r._f04);				\
									(_v05) = (gpRegRC1->_r._f05);				\
									(_v06) = (gpRegRC1->_r._f06);				\
									(_v07) = (gpRegRC1->_r._f07);				\
								} while(0)

#define RC1_Rd08(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08)					\
								do { 											\
									(_v01) = (gpRegRC1->_r._f01);				\
									(_v02) = (gpRegRC1->_r._f02);				\
									(_v03) = (gpRegRC1->_r._f03);				\
									(_v04) = (gpRegRC1->_r._f04);				\
									(_v05) = (gpRegRC1->_r._f05);				\
									(_v06) = (gpRegRC1->_r._f06);				\
									(_v07) = (gpRegRC1->_r._f07);				\
									(_v08) = (gpRegRC1->_r._f08);				\
								} while(0)

#define RC1_Rd09(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09)													\
								do { 											\
									(_v01) = (gpRegRC1->_r._f01);				\
									(_v02) = (gpRegRC1->_r._f02);				\
									(_v03) = (gpRegRC1->_r._f03);				\
									(_v04) = (gpRegRC1->_r._f04);				\
									(_v05) = (gpRegRC1->_r._f05);				\
									(_v06) = (gpRegRC1->_r._f06);				\
									(_v07) = (gpRegRC1->_r._f07);				\
									(_v08) = (gpRegRC1->_r._f08);				\
									(_v09) = (gpRegRC1->_r._f09);				\
								} while(0)

#define RC1_Rd10(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10)										\
								do { 											\
									(_v01) = (gpRegRC1->_r._f01);				\
									(_v02) = (gpRegRC1->_r._f02);				\
									(_v03) = (gpRegRC1->_r._f03);				\
									(_v04) = (gpRegRC1->_r._f04);				\
									(_v05) = (gpRegRC1->_r._f05);				\
									(_v06) = (gpRegRC1->_r._f06);				\
									(_v07) = (gpRegRC1->_r._f07);				\
									(_v08) = (gpRegRC1->_r._f08);				\
									(_v09) = (gpRegRC1->_r._f09);				\
									(_v10) = (gpRegRC1->_r._f10);				\
								} while(0)

#define RC1_Rd11(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11)								\
								do { 											\
									(_v01) = (gpRegRC1->_r._f01);				\
									(_v02) = (gpRegRC1->_r._f02);				\
									(_v03) = (gpRegRC1->_r._f03);				\
									(_v04) = (gpRegRC1->_r._f04);				\
									(_v05) = (gpRegRC1->_r._f05);				\
									(_v06) = (gpRegRC1->_r._f06);				\
									(_v07) = (gpRegRC1->_r._f07);				\
									(_v08) = (gpRegRC1->_r._f08);				\
									(_v09) = (gpRegRC1->_r._f09);				\
									(_v10) = (gpRegRC1->_r._f10);				\
									(_v11) = (gpRegRC1->_r._f11);				\
								} while(0)

#define RC1_Rd12(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12)					\
								do { 											\
									(_v01) = (gpRegRC1->_r._f01);				\
									(_v02) = (gpRegRC1->_r._f02);				\
									(_v03) = (gpRegRC1->_r._f03);				\
									(_v04) = (gpRegRC1->_r._f04);				\
									(_v05) = (gpRegRC1->_r._f05);				\
									(_v06) = (gpRegRC1->_r._f06);				\
									(_v07) = (gpRegRC1->_r._f07);				\
									(_v08) = (gpRegRC1->_r._f08);				\
									(_v09) = (gpRegRC1->_r._f09);				\
									(_v10) = (gpRegRC1->_r._f10);				\
									(_v11) = (gpRegRC1->_r._f11);				\
									(_v12) = (gpRegRC1->_r._f12);				\
								} while(0)

#define RC1_Rd13(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13)													\
								do { 											\
									(_v01) = (gpRegRC1->_r._f01);				\
									(_v02) = (gpRegRC1->_r._f02);				\
									(_v03) = (gpRegRC1->_r._f03);				\
									(_v04) = (gpRegRC1->_r._f04);				\
									(_v05) = (gpRegRC1->_r._f05);				\
									(_v06) = (gpRegRC1->_r._f06);				\
									(_v07) = (gpRegRC1->_r._f07);				\
									(_v08) = (gpRegRC1->_r._f08);				\
									(_v09) = (gpRegRC1->_r._f09);				\
									(_v10) = (gpRegRC1->_r._f10);				\
									(_v11) = (gpRegRC1->_r._f11);				\
									(_v12) = (gpRegRC1->_r._f12);				\
									(_v13) = (gpRegRC1->_r._f13);				\
								} while(0)

#define RC1_Rd14(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14)										\
								do { 											\
									(_v01) = (gpRegRC1->_r._f01);				\
									(_v02) = (gpRegRC1->_r._f02);				\
									(_v03) = (gpRegRC1->_r._f03);				\
									(_v04) = (gpRegRC1->_r._f04);				\
									(_v05) = (gpRegRC1->_r._f05);				\
									(_v06) = (gpRegRC1->_r._f06);				\
									(_v07) = (gpRegRC1->_r._f07);				\
									(_v08) = (gpRegRC1->_r._f08);				\
									(_v09) = (gpRegRC1->_r._f09);				\
									(_v10) = (gpRegRC1->_r._f10);				\
									(_v11) = (gpRegRC1->_r._f11);				\
									(_v12) = (gpRegRC1->_r._f12);				\
									(_v13) = (gpRegRC1->_r._f13);				\
									(_v14) = (gpRegRC1->_r._f14);				\
								} while(0)

#define RC1_Rd15(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15)								\
								do { 											\
									(_v01) = (gpRegRC1->_r._f01);				\
									(_v02) = (gpRegRC1->_r._f02);				\
									(_v03) = (gpRegRC1->_r._f03);				\
									(_v04) = (gpRegRC1->_r._f04);				\
									(_v05) = (gpRegRC1->_r._f05);				\
									(_v06) = (gpRegRC1->_r._f06);				\
									(_v07) = (gpRegRC1->_r._f07);				\
									(_v08) = (gpRegRC1->_r._f08);				\
									(_v09) = (gpRegRC1->_r._f09);				\
									(_v10) = (gpRegRC1->_r._f10);				\
									(_v11) = (gpRegRC1->_r._f11);				\
									(_v12) = (gpRegRC1->_r._f12);				\
									(_v13) = (gpRegRC1->_r._f13);				\
									(_v14) = (gpRegRC1->_r._f14);				\
									(_v15) = (gpRegRC1->_r._f15);				\
								} while(0)

#define RC1_Rd16(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15,_f16,_v16)					\
								do { 											\
									(_v01) = (gpRegRC1->_r._f01);				\
									(_v02) = (gpRegRC1->_r._f02);				\
									(_v03) = (gpRegRC1->_r._f03);				\
									(_v04) = (gpRegRC1->_r._f04);				\
									(_v05) = (gpRegRC1->_r._f05);				\
									(_v06) = (gpRegRC1->_r._f06);				\
									(_v07) = (gpRegRC1->_r._f07);				\
									(_v08) = (gpRegRC1->_r._f08);				\
									(_v09) = (gpRegRC1->_r._f09);				\
									(_v10) = (gpRegRC1->_r._f10);				\
									(_v11) = (gpRegRC1->_r._f11);				\
									(_v12) = (gpRegRC1->_r._f12);				\
									(_v13) = (gpRegRC1->_r._f13);				\
									(_v14) = (gpRegRC1->_r._f14);				\
									(_v15) = (gpRegRC1->_r._f15);				\
									(_v16) = (gpRegRC1->_r._f16);				\
								} while(0)


#define RC1_Wr01(_r,_f01,_v01)													\
								do { 											\
									(gpRegRC1->_r._f01) = (_v01);				\
								} while(0)

#define RC1_Wr02(_r,_f01,_v01,_f02,_v02)										\
								do { 											\
									(gpRegRC1->_r._f01) = (_v01);				\
									(gpRegRC1->_r._f02) = (_v02);				\
								} while(0)

#define RC1_Wr03(_r,_f01,_v01,_f02,_v02,_f03,_v03)								\
								do { 											\
									(gpRegRC1->_r._f01) = (_v01);				\
									(gpRegRC1->_r._f02) = (_v02);				\
									(gpRegRC1->_r._f03) = (_v03);				\
								} while(0)

#define RC1_Wr04(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04)					\
								do { 											\
									(gpRegRC1->_r._f01) = (_v01);				\
									(gpRegRC1->_r._f02) = (_v02);				\
									(gpRegRC1->_r._f03) = (_v03);				\
									(gpRegRC1->_r._f04) = (_v04);				\
								} while(0)

#define RC1_Wr05(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05)													\
								do { 											\
									(gpRegRC1->_r._f01) = (_v01);				\
									(gpRegRC1->_r._f02) = (_v02);				\
									(gpRegRC1->_r._f03) = (_v03);				\
									(gpRegRC1->_r._f04) = (_v04);				\
									(gpRegRC1->_r._f05) = (_v05);				\
								} while(0)

#define RC1_Wr06(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06)										\
								do { 											\
									(gpRegRC1->_r._f01) = (_v01);				\
									(gpRegRC1->_r._f02) = (_v02);				\
									(gpRegRC1->_r._f03) = (_v03);				\
									(gpRegRC1->_r._f04) = (_v04);				\
									(gpRegRC1->_r._f05) = (_v05);				\
									(gpRegRC1->_r._f06) = (_v06);				\
								} while(0)

#define RC1_Wr07(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07)								\
								do { 											\
									(gpRegRC1->_r._f01) = (_v01);				\
									(gpRegRC1->_r._f02) = (_v02);				\
									(gpRegRC1->_r._f03) = (_v03);				\
									(gpRegRC1->_r._f04) = (_v04);				\
									(gpRegRC1->_r._f05) = (_v05);				\
									(gpRegRC1->_r._f06) = (_v06);				\
									(gpRegRC1->_r._f07) = (_v07);				\
								} while(0)

#define RC1_Wr08(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08)					\
								do { 											\
									(gpRegRC1->_r._f01) = (_v01);				\
									(gpRegRC1->_r._f02) = (_v02);				\
									(gpRegRC1->_r._f03) = (_v03);				\
									(gpRegRC1->_r._f04) = (_v04);				\
									(gpRegRC1->_r._f05) = (_v05);				\
									(gpRegRC1->_r._f06) = (_v06);				\
									(gpRegRC1->_r._f07) = (_v07);				\
									(gpRegRC1->_r._f08) = (_v08);				\
								} while(0)

#define RC1_Wr09(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09)													\
								do { 											\
									(gpRegRC1->_r._f01) = (_v01);				\
									(gpRegRC1->_r._f02) = (_v02);				\
									(gpRegRC1->_r._f03) = (_v03);				\
									(gpRegRC1->_r._f04) = (_v04);				\
									(gpRegRC1->_r._f05) = (_v05);				\
									(gpRegRC1->_r._f06) = (_v06);				\
									(gpRegRC1->_r._f07) = (_v07);				\
									(gpRegRC1->_r._f08) = (_v08);				\
									(gpRegRC1->_r._f09) = (_v09);				\
								} while(0)

#define RC1_Wr10(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10)										\
								do { 											\
									(gpRegRC1->_r._f01) = (_v01);				\
									(gpRegRC1->_r._f02) = (_v02);				\
									(gpRegRC1->_r._f03) = (_v03);				\
									(gpRegRC1->_r._f04) = (_v04);				\
									(gpRegRC1->_r._f05) = (_v05);				\
									(gpRegRC1->_r._f06) = (_v06);				\
									(gpRegRC1->_r._f07) = (_v07);				\
									(gpRegRC1->_r._f08) = (_v08);				\
									(gpRegRC1->_r._f09) = (_v09);				\
									(gpRegRC1->_r._f10) = (_v10);				\
								} while(0)

#define RC1_Wr11(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11)								\
								do { 											\
									(gpRegRC1->_r._f01) = (_v01);				\
									(gpRegRC1->_r._f02) = (_v02);				\
									(gpRegRC1->_r._f03) = (_v03);				\
									(gpRegRC1->_r._f04) = (_v04);				\
									(gpRegRC1->_r._f05) = (_v05);				\
									(gpRegRC1->_r._f06) = (_v06);				\
									(gpRegRC1->_r._f07) = (_v07);				\
									(gpRegRC1->_r._f08) = (_v08);				\
									(gpRegRC1->_r._f09) = (_v09);				\
									(gpRegRC1->_r._f10) = (_v10);				\
									(gpRegRC1->_r._f11) = (_v11);				\
								} while(0)

#define RC1_Wr12(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12)					\
								do { 											\
									(gpRegRC1->_r._f01) = (_v01);				\
									(gpRegRC1->_r._f02) = (_v02);				\
									(gpRegRC1->_r._f03) = (_v03);				\
									(gpRegRC1->_r._f04) = (_v04);				\
									(gpRegRC1->_r._f05) = (_v05);				\
									(gpRegRC1->_r._f06) = (_v06);				\
									(gpRegRC1->_r._f07) = (_v07);				\
									(gpRegRC1->_r._f08) = (_v08);				\
									(gpRegRC1->_r._f09) = (_v09);				\
									(gpRegRC1->_r._f10) = (_v10);				\
									(gpRegRC1->_r._f11) = (_v11);				\
									(gpRegRC1->_r._f12) = (_v12);				\
								} while(0)

#define RC1_Wr13(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13)													\
								do { 											\
									(gpRegRC1->_r._f01) = (_v01);				\
									(gpRegRC1->_r._f02) = (_v02);				\
									(gpRegRC1->_r._f03) = (_v03);				\
									(gpRegRC1->_r._f04) = (_v04);				\
									(gpRegRC1->_r._f05) = (_v05);				\
									(gpRegRC1->_r._f06) = (_v06);				\
									(gpRegRC1->_r._f07) = (_v07);				\
									(gpRegRC1->_r._f08) = (_v08);				\
									(gpRegRC1->_r._f09) = (_v09);				\
									(gpRegRC1->_r._f10) = (_v10);				\
									(gpRegRC1->_r._f11) = (_v11);				\
									(gpRegRC1->_r._f12) = (_v12);				\
									(gpRegRC1->_r._f13) = (_v13);				\
								} while(0)

#define RC1_Wr14(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14)										\
								do { 											\
									(gpRegRC1->_r._f01) = (_v01);				\
									(gpRegRC1->_r._f02) = (_v02);				\
									(gpRegRC1->_r._f03) = (_v03);				\
									(gpRegRC1->_r._f04) = (_v04);				\
									(gpRegRC1->_r._f05) = (_v05);				\
									(gpRegRC1->_r._f06) = (_v06);				\
									(gpRegRC1->_r._f07) = (_v07);				\
									(gpRegRC1->_r._f08) = (_v08);				\
									(gpRegRC1->_r._f09) = (_v09);				\
									(gpRegRC1->_r._f10) = (_v10);				\
									(gpRegRC1->_r._f11) = (_v11);				\
									(gpRegRC1->_r._f12) = (_v12);				\
									(gpRegRC1->_r._f13) = (_v13);				\
									(gpRegRC1->_r._f14) = (_v14);				\
								} while(0)

#define RC1_Wr15(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15)								\
								do { 											\
									(gpRegRC1->_r._f01) = (_v01);				\
									(gpRegRC1->_r._f02) = (_v02);				\
									(gpRegRC1->_r._f03) = (_v03);				\
									(gpRegRC1->_r._f04) = (_v04);				\
									(gpRegRC1->_r._f05) = (_v05);				\
									(gpRegRC1->_r._f06) = (_v06);				\
									(gpRegRC1->_r._f07) = (_v07);				\
									(gpRegRC1->_r._f08) = (_v08);				\
									(gpRegRC1->_r._f09) = (_v09);				\
									(gpRegRC1->_r._f10) = (_v10);				\
									(gpRegRC1->_r._f11) = (_v11);				\
									(gpRegRC1->_r._f12) = (_v12);				\
									(gpRegRC1->_r._f13) = (_v13);				\
									(gpRegRC1->_r._f14) = (_v14);				\
									(gpRegRC1->_r._f15) = (_v15);				\
								} while(0)

#define RC1_Wr16(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15,_f16,_v16)					\
								do { 											\
									(gpRegRC1->_r._f01) = (_v01);				\
									(gpRegRC1->_r._f02) = (_v02);				\
									(gpRegRC1->_r._f03) = (_v03);				\
									(gpRegRC1->_r._f04) = (_v04);				\
									(gpRegRC1->_r._f05) = (_v05);				\
									(gpRegRC1->_r._f06) = (_v06);				\
									(gpRegRC1->_r._f07) = (_v07);				\
									(gpRegRC1->_r._f08) = (_v08);				\
									(gpRegRC1->_r._f09) = (_v09);				\
									(gpRegRC1->_r._f10) = (_v10);				\
									(gpRegRC1->_r._f11) = (_v11);				\
									(gpRegRC1->_r._f12) = (_v12);				\
									(gpRegRC1->_r._f13) = (_v13);				\
									(gpRegRC1->_r._f14) = (_v14);				\
									(gpRegRC1->_r._f15) = (_v15);				\
									(gpRegRC1->_r._f16) = (_v16);				\
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
 * _rwname: name of rw    bit field : shall be 0 after RC1_Wind(), 1 for RC1_Rind()
 * _iname : name of index bit field
 * _ival  : index value
 * _fname : field name
 * _fval  : field variable that field value shall be stored.
 *
 * RC1_Rind : General indexed register Read.(
 * RC1_Wind : General indexed register Read.
 *
 * RC1_Ridx : For 'index', 'rw', 'load' field name
 * RC1_Widx : For 'index', 'rw', 'load' field name and NO_LOAD.
 */
#define RC1_Rind(_r, _lname, _rwname, _iname, _ival, _fname, _fval)				\
							do {												\
								RC1_Wr03(_r,_lname,0,_rwname,1,_iname,_ival);	\
								RC1_WrFL(_r);									\
								RC1_RdFL(_r);									\
								RC1_Rd01(_r,_fname,_fval);						\
							} while (0)

#define RC1_Wind(_r, _lname, _rwname, _iname, _ival, _fname, _fval)				\
				RC1_Wr04(_r, _lname,0, _rwname,0, _iname,_ival, _fname,_fval)


#define RC1_Ridx(_r, _ival, _fname, _fval)	RC1_Rind(_r,load,rw,index,_ival,_fname,_fval)

#define RC1_Widx(_r, _ival, _fname, _fval)	RC1_Wind(_r,load,rw,index,_ival,_fname,_fval)

/** @} *//* end of macro documentation */

/*
 * @{
 * Naming for register pointer.
 * gpRealRegRC1 : real register of RC1.
 * gpRegRC1     : shadow register.
 *
 * @def RC1_RdFL: Read  FLushing : Shadow <- Real.
 * @def RC1_WrFL: Write FLushing : Shadow -> Real.
 * @def RC1_Rd  : Read  whole register(UINT32) from Shadow register.
 * @def RC1_Wr  : Write whole register(UINT32) from Shadow register.
 * @def RC1_Rd01 ~ RC1_Rdnn: Read  given '01~nn' fields from Shadow register.
 * @def RC1_Wr01 ~ RC1_Wrnn: Write given '01~nn' fields to   Shadow register.
 * */
#define RC1_RdFL(_r)			((gpRegRC1->_r)=(gpRealRegRC1->_r))
#define RC1_WrFL(_r)			((gpRealRegRC1->_r)=(gpRegRC1->_r))

#define RC1_Rd(_r)			*((UINT32*)(&(gpRegRC1->_r)))
#define RC1_Wr(_r,_v)			((RC1_Rd(_r))=((UINT32)(_v)))

#define RC1_Rd01(_r,_f01,_v01)													\
								do { 											\
									(_v01) = (gpRegRC1->_r._f01);				\
								} while(0)

#define RC1_Rd02(_r,_f01,_v01,_f02,_v02)										\
								do { 											\
									(_v01) = (gpRegRC1->_r._f01);				\
									(_v02) = (gpRegRC1->_r._f02);				\
								} while(0)

#define RC1_Rd03(_r,_f01,_v01,_f02,_v02,_f03,_v03)								\
								do { 											\
									(_v01) = (gpRegRC1->_r._f01);				\
									(_v02) = (gpRegRC1->_r._f02);				\
									(_v03) = (gpRegRC1->_r._f03);				\
								} while(0)

#define RC1_Rd04(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04)					\
								do { 											\
									(_v01) = (gpRegRC1->_r._f01);				\
									(_v02) = (gpRegRC1->_r._f02);				\
									(_v03) = (gpRegRC1->_r._f03);				\
									(_v04) = (gpRegRC1->_r._f04);				\
								} while(0)

#define RC1_Rd05(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05)													\
								do { 											\
									(_v01) = (gpRegRC1->_r._f01);				\
									(_v02) = (gpRegRC1->_r._f02);				\
									(_v03) = (gpRegRC1->_r._f03);				\
									(_v04) = (gpRegRC1->_r._f04);				\
									(_v05) = (gpRegRC1->_r._f05);				\
								} while(0)

#define RC1_Rd06(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06)										\
								do { 											\
									(_v01) = (gpRegRC1->_r._f01);				\
									(_v02) = (gpRegRC1->_r._f02);				\
									(_v03) = (gpRegRC1->_r._f03);				\
									(_v04) = (gpRegRC1->_r._f04);				\
									(_v05) = (gpRegRC1->_r._f05);				\
									(_v06) = (gpRegRC1->_r._f06);				\
								} while(0)

#define RC1_Rd07(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07)								\
								do { 											\
									(_v01) = (gpRegRC1->_r._f01);				\
									(_v02) = (gpRegRC1->_r._f02);				\
									(_v03) = (gpRegRC1->_r._f03);				\
									(_v04) = (gpRegRC1->_r._f04);				\
									(_v05) = (gpRegRC1->_r._f05);				\
									(_v06) = (gpRegRC1->_r._f06);				\
									(_v07) = (gpRegRC1->_r._f07);				\
								} while(0)

#define RC1_Rd08(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08)					\
								do { 											\
									(_v01) = (gpRegRC1->_r._f01);				\
									(_v02) = (gpRegRC1->_r._f02);				\
									(_v03) = (gpRegRC1->_r._f03);				\
									(_v04) = (gpRegRC1->_r._f04);				\
									(_v05) = (gpRegRC1->_r._f05);				\
									(_v06) = (gpRegRC1->_r._f06);				\
									(_v07) = (gpRegRC1->_r._f07);				\
									(_v08) = (gpRegRC1->_r._f08);				\
								} while(0)

#define RC1_Rd09(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09)													\
								do { 											\
									(_v01) = (gpRegRC1->_r._f01);				\
									(_v02) = (gpRegRC1->_r._f02);				\
									(_v03) = (gpRegRC1->_r._f03);				\
									(_v04) = (gpRegRC1->_r._f04);				\
									(_v05) = (gpRegRC1->_r._f05);				\
									(_v06) = (gpRegRC1->_r._f06);				\
									(_v07) = (gpRegRC1->_r._f07);				\
									(_v08) = (gpRegRC1->_r._f08);				\
									(_v09) = (gpRegRC1->_r._f09);				\
								} while(0)

#define RC1_Rd10(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10)										\
								do { 											\
									(_v01) = (gpRegRC1->_r._f01);				\
									(_v02) = (gpRegRC1->_r._f02);				\
									(_v03) = (gpRegRC1->_r._f03);				\
									(_v04) = (gpRegRC1->_r._f04);				\
									(_v05) = (gpRegRC1->_r._f05);				\
									(_v06) = (gpRegRC1->_r._f06);				\
									(_v07) = (gpRegRC1->_r._f07);				\
									(_v08) = (gpRegRC1->_r._f08);				\
									(_v09) = (gpRegRC1->_r._f09);				\
									(_v10) = (gpRegRC1->_r._f10);				\
								} while(0)

#define RC1_Rd11(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11)								\
								do { 											\
									(_v01) = (gpRegRC1->_r._f01);				\
									(_v02) = (gpRegRC1->_r._f02);				\
									(_v03) = (gpRegRC1->_r._f03);				\
									(_v04) = (gpRegRC1->_r._f04);				\
									(_v05) = (gpRegRC1->_r._f05);				\
									(_v06) = (gpRegRC1->_r._f06);				\
									(_v07) = (gpRegRC1->_r._f07);				\
									(_v08) = (gpRegRC1->_r._f08);				\
									(_v09) = (gpRegRC1->_r._f09);				\
									(_v10) = (gpRegRC1->_r._f10);				\
									(_v11) = (gpRegRC1->_r._f11);				\
								} while(0)

#define RC1_Rd12(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12)					\
								do { 											\
									(_v01) = (gpRegRC1->_r._f01);				\
									(_v02) = (gpRegRC1->_r._f02);				\
									(_v03) = (gpRegRC1->_r._f03);				\
									(_v04) = (gpRegRC1->_r._f04);				\
									(_v05) = (gpRegRC1->_r._f05);				\
									(_v06) = (gpRegRC1->_r._f06);				\
									(_v07) = (gpRegRC1->_r._f07);				\
									(_v08) = (gpRegRC1->_r._f08);				\
									(_v09) = (gpRegRC1->_r._f09);				\
									(_v10) = (gpRegRC1->_r._f10);				\
									(_v11) = (gpRegRC1->_r._f11);				\
									(_v12) = (gpRegRC1->_r._f12);				\
								} while(0)

#define RC1_Rd13(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13)													\
								do { 											\
									(_v01) = (gpRegRC1->_r._f01);				\
									(_v02) = (gpRegRC1->_r._f02);				\
									(_v03) = (gpRegRC1->_r._f03);				\
									(_v04) = (gpRegRC1->_r._f04);				\
									(_v05) = (gpRegRC1->_r._f05);				\
									(_v06) = (gpRegRC1->_r._f06);				\
									(_v07) = (gpRegRC1->_r._f07);				\
									(_v08) = (gpRegRC1->_r._f08);				\
									(_v09) = (gpRegRC1->_r._f09);				\
									(_v10) = (gpRegRC1->_r._f10);				\
									(_v11) = (gpRegRC1->_r._f11);				\
									(_v12) = (gpRegRC1->_r._f12);				\
									(_v13) = (gpRegRC1->_r._f13);				\
								} while(0)

#define RC1_Rd14(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14)										\
								do { 											\
									(_v01) = (gpRegRC1->_r._f01);				\
									(_v02) = (gpRegRC1->_r._f02);				\
									(_v03) = (gpRegRC1->_r._f03);				\
									(_v04) = (gpRegRC1->_r._f04);				\
									(_v05) = (gpRegRC1->_r._f05);				\
									(_v06) = (gpRegRC1->_r._f06);				\
									(_v07) = (gpRegRC1->_r._f07);				\
									(_v08) = (gpRegRC1->_r._f08);				\
									(_v09) = (gpRegRC1->_r._f09);				\
									(_v10) = (gpRegRC1->_r._f10);				\
									(_v11) = (gpRegRC1->_r._f11);				\
									(_v12) = (gpRegRC1->_r._f12);				\
									(_v13) = (gpRegRC1->_r._f13);				\
									(_v14) = (gpRegRC1->_r._f14);				\
								} while(0)

#define RC1_Rd15(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15)								\
								do { 											\
									(_v01) = (gpRegRC1->_r._f01);				\
									(_v02) = (gpRegRC1->_r._f02);				\
									(_v03) = (gpRegRC1->_r._f03);				\
									(_v04) = (gpRegRC1->_r._f04);				\
									(_v05) = (gpRegRC1->_r._f05);				\
									(_v06) = (gpRegRC1->_r._f06);				\
									(_v07) = (gpRegRC1->_r._f07);				\
									(_v08) = (gpRegRC1->_r._f08);				\
									(_v09) = (gpRegRC1->_r._f09);				\
									(_v10) = (gpRegRC1->_r._f10);				\
									(_v11) = (gpRegRC1->_r._f11);				\
									(_v12) = (gpRegRC1->_r._f12);				\
									(_v13) = (gpRegRC1->_r._f13);				\
									(_v14) = (gpRegRC1->_r._f14);				\
									(_v15) = (gpRegRC1->_r._f15);				\
								} while(0)

#define RC1_Rd16(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15,_f16,_v16)					\
								do { 											\
									(_v01) = (gpRegRC1->_r._f01);				\
									(_v02) = (gpRegRC1->_r._f02);				\
									(_v03) = (gpRegRC1->_r._f03);				\
									(_v04) = (gpRegRC1->_r._f04);				\
									(_v05) = (gpRegRC1->_r._f05);				\
									(_v06) = (gpRegRC1->_r._f06);				\
									(_v07) = (gpRegRC1->_r._f07);				\
									(_v08) = (gpRegRC1->_r._f08);				\
									(_v09) = (gpRegRC1->_r._f09);				\
									(_v10) = (gpRegRC1->_r._f10);				\
									(_v11) = (gpRegRC1->_r._f11);				\
									(_v12) = (gpRegRC1->_r._f12);				\
									(_v13) = (gpRegRC1->_r._f13);				\
									(_v14) = (gpRegRC1->_r._f14);				\
									(_v15) = (gpRegRC1->_r._f15);				\
									(_v16) = (gpRegRC1->_r._f16);				\
								} while(0)


#define RC1_Wr01(_r,_f01,_v01)													\
								do { 											\
									(gpRegRC1->_r._f01) = (_v01);				\
								} while(0)

#define RC1_Wr02(_r,_f01,_v01,_f02,_v02)										\
								do { 											\
									(gpRegRC1->_r._f01) = (_v01);				\
									(gpRegRC1->_r._f02) = (_v02);				\
								} while(0)

#define RC1_Wr03(_r,_f01,_v01,_f02,_v02,_f03,_v03)								\
								do { 											\
									(gpRegRC1->_r._f01) = (_v01);				\
									(gpRegRC1->_r._f02) = (_v02);				\
									(gpRegRC1->_r._f03) = (_v03);				\
								} while(0)

#define RC1_Wr04(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04)					\
								do { 											\
									(gpRegRC1->_r._f01) = (_v01);				\
									(gpRegRC1->_r._f02) = (_v02);				\
									(gpRegRC1->_r._f03) = (_v03);				\
									(gpRegRC1->_r._f04) = (_v04);				\
								} while(0)

#define RC1_Wr05(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05)													\
								do { 											\
									(gpRegRC1->_r._f01) = (_v01);				\
									(gpRegRC1->_r._f02) = (_v02);				\
									(gpRegRC1->_r._f03) = (_v03);				\
									(gpRegRC1->_r._f04) = (_v04);				\
									(gpRegRC1->_r._f05) = (_v05);				\
								} while(0)

#define RC1_Wr06(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06)										\
								do { 											\
									(gpRegRC1->_r._f01) = (_v01);				\
									(gpRegRC1->_r._f02) = (_v02);				\
									(gpRegRC1->_r._f03) = (_v03);				\
									(gpRegRC1->_r._f04) = (_v04);				\
									(gpRegRC1->_r._f05) = (_v05);				\
									(gpRegRC1->_r._f06) = (_v06);				\
								} while(0)

#define RC1_Wr07(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07)								\
								do { 											\
									(gpRegRC1->_r._f01) = (_v01);				\
									(gpRegRC1->_r._f02) = (_v02);				\
									(gpRegRC1->_r._f03) = (_v03);				\
									(gpRegRC1->_r._f04) = (_v04);				\
									(gpRegRC1->_r._f05) = (_v05);				\
									(gpRegRC1->_r._f06) = (_v06);				\
									(gpRegRC1->_r._f07) = (_v07);				\
								} while(0)

#define RC1_Wr08(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08)					\
								do { 											\
									(gpRegRC1->_r._f01) = (_v01);				\
									(gpRegRC1->_r._f02) = (_v02);				\
									(gpRegRC1->_r._f03) = (_v03);				\
									(gpRegRC1->_r._f04) = (_v04);				\
									(gpRegRC1->_r._f05) = (_v05);				\
									(gpRegRC1->_r._f06) = (_v06);				\
									(gpRegRC1->_r._f07) = (_v07);				\
									(gpRegRC1->_r._f08) = (_v08);				\
								} while(0)

#define RC1_Wr09(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09)													\
								do { 											\
									(gpRegRC1->_r._f01) = (_v01);				\
									(gpRegRC1->_r._f02) = (_v02);				\
									(gpRegRC1->_r._f03) = (_v03);				\
									(gpRegRC1->_r._f04) = (_v04);				\
									(gpRegRC1->_r._f05) = (_v05);				\
									(gpRegRC1->_r._f06) = (_v06);				\
									(gpRegRC1->_r._f07) = (_v07);				\
									(gpRegRC1->_r._f08) = (_v08);				\
									(gpRegRC1->_r._f09) = (_v09);				\
								} while(0)

#define RC1_Wr10(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10)										\
								do { 											\
									(gpRegRC1->_r._f01) = (_v01);				\
									(gpRegRC1->_r._f02) = (_v02);				\
									(gpRegRC1->_r._f03) = (_v03);				\
									(gpRegRC1->_r._f04) = (_v04);				\
									(gpRegRC1->_r._f05) = (_v05);				\
									(gpRegRC1->_r._f06) = (_v06);				\
									(gpRegRC1->_r._f07) = (_v07);				\
									(gpRegRC1->_r._f08) = (_v08);				\
									(gpRegRC1->_r._f09) = (_v09);				\
									(gpRegRC1->_r._f10) = (_v10);				\
								} while(0)

#define RC1_Wr11(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11)								\
								do { 											\
									(gpRegRC1->_r._f01) = (_v01);				\
									(gpRegRC1->_r._f02) = (_v02);				\
									(gpRegRC1->_r._f03) = (_v03);				\
									(gpRegRC1->_r._f04) = (_v04);				\
									(gpRegRC1->_r._f05) = (_v05);				\
									(gpRegRC1->_r._f06) = (_v06);				\
									(gpRegRC1->_r._f07) = (_v07);				\
									(gpRegRC1->_r._f08) = (_v08);				\
									(gpRegRC1->_r._f09) = (_v09);				\
									(gpRegRC1->_r._f10) = (_v10);				\
									(gpRegRC1->_r._f11) = (_v11);				\
								} while(0)

#define RC1_Wr12(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12)					\
								do { 											\
									(gpRegRC1->_r._f01) = (_v01);				\
									(gpRegRC1->_r._f02) = (_v02);				\
									(gpRegRC1->_r._f03) = (_v03);				\
									(gpRegRC1->_r._f04) = (_v04);				\
									(gpRegRC1->_r._f05) = (_v05);				\
									(gpRegRC1->_r._f06) = (_v06);				\
									(gpRegRC1->_r._f07) = (_v07);				\
									(gpRegRC1->_r._f08) = (_v08);				\
									(gpRegRC1->_r._f09) = (_v09);				\
									(gpRegRC1->_r._f10) = (_v10);				\
									(gpRegRC1->_r._f11) = (_v11);				\
									(gpRegRC1->_r._f12) = (_v12);				\
								} while(0)

#define RC1_Wr13(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13)													\
								do { 											\
									(gpRegRC1->_r._f01) = (_v01);				\
									(gpRegRC1->_r._f02) = (_v02);				\
									(gpRegRC1->_r._f03) = (_v03);				\
									(gpRegRC1->_r._f04) = (_v04);				\
									(gpRegRC1->_r._f05) = (_v05);				\
									(gpRegRC1->_r._f06) = (_v06);				\
									(gpRegRC1->_r._f07) = (_v07);				\
									(gpRegRC1->_r._f08) = (_v08);				\
									(gpRegRC1->_r._f09) = (_v09);				\
									(gpRegRC1->_r._f10) = (_v10);				\
									(gpRegRC1->_r._f11) = (_v11);				\
									(gpRegRC1->_r._f12) = (_v12);				\
									(gpRegRC1->_r._f13) = (_v13);				\
								} while(0)

#define RC1_Wr14(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14)										\
								do { 											\
									(gpRegRC1->_r._f01) = (_v01);				\
									(gpRegRC1->_r._f02) = (_v02);				\
									(gpRegRC1->_r._f03) = (_v03);				\
									(gpRegRC1->_r._f04) = (_v04);				\
									(gpRegRC1->_r._f05) = (_v05);				\
									(gpRegRC1->_r._f06) = (_v06);				\
									(gpRegRC1->_r._f07) = (_v07);				\
									(gpRegRC1->_r._f08) = (_v08);				\
									(gpRegRC1->_r._f09) = (_v09);				\
									(gpRegRC1->_r._f10) = (_v10);				\
									(gpRegRC1->_r._f11) = (_v11);				\
									(gpRegRC1->_r._f12) = (_v12);				\
									(gpRegRC1->_r._f13) = (_v13);				\
									(gpRegRC1->_r._f14) = (_v14);				\
								} while(0)

#define RC1_Wr15(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15)								\
								do { 											\
									(gpRegRC1->_r._f01) = (_v01);				\
									(gpRegRC1->_r._f02) = (_v02);				\
									(gpRegRC1->_r._f03) = (_v03);				\
									(gpRegRC1->_r._f04) = (_v04);				\
									(gpRegRC1->_r._f05) = (_v05);				\
									(gpRegRC1->_r._f06) = (_v06);				\
									(gpRegRC1->_r._f07) = (_v07);				\
									(gpRegRC1->_r._f08) = (_v08);				\
									(gpRegRC1->_r._f09) = (_v09);				\
									(gpRegRC1->_r._f10) = (_v10);				\
									(gpRegRC1->_r._f11) = (_v11);				\
									(gpRegRC1->_r._f12) = (_v12);				\
									(gpRegRC1->_r._f13) = (_v13);				\
									(gpRegRC1->_r._f14) = (_v14);				\
									(gpRegRC1->_r._f15) = (_v15);				\
								} while(0)

#define RC1_Wr16(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15,_f16,_v16)					\
								do { 											\
									(gpRegRC1->_r._f01) = (_v01);				\
									(gpRegRC1->_r._f02) = (_v02);				\
									(gpRegRC1->_r._f03) = (_v03);				\
									(gpRegRC1->_r._f04) = (_v04);				\
									(gpRegRC1->_r._f05) = (_v05);				\
									(gpRegRC1->_r._f06) = (_v06);				\
									(gpRegRC1->_r._f07) = (_v07);				\
									(gpRegRC1->_r._f08) = (_v08);				\
									(gpRegRC1->_r._f09) = (_v09);				\
									(gpRegRC1->_r._f10) = (_v10);				\
									(gpRegRC1->_r._f11) = (_v11);				\
									(gpRegRC1->_r._f12) = (_v12);				\
									(gpRegRC1->_r._f13) = (_v13);				\
									(gpRegRC1->_r._f14) = (_v14);				\
									(gpRegRC1->_r._f15) = (_v15);				\
									(gpRegRC1->_r._f16) = (_v16);				\
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
 * _rwname: name of rw    bit field : shall be 0 after RC1_Wind(), 1 for RC1_Rind()
 * _iname : name of index bit field
 * _ival  : index value
 * _fname : field name
 * _fval  : field variable that field value shall be stored.
 *
 * RC1_Rind : General indexed register Read.(
 * RC1_Wind : General indexed register Read.
 *
 * RC1_Ridx : For 'index', 'rw', 'load' field name
 * RC1_Widx : For 'index', 'rw', 'load' field name and NO_LOAD.
 */
#define RC1_Rind(_r, _lname, _rwname, _iname, _ival, _fname, _fval)				\
							do {												\
								RC1_Wr03(_r,_lname,0,_rwname,1,_iname,_ival);	\
								RC1_WrFL(_r);									\
								RC1_RdFL(_r);									\
								RC1_Rd01(_r,_fname,_fval);						\
							} while (0)

#define RC1_Wind(_r, _lname, _rwname, _iname, _ival, _fname, _fval)				\
				RC1_Wr04(_r, _lname,0, _rwname,0, _iname,_ival, _fname,_fval)


#define RC1_Ridx(_r, _ival, _fname, _fval)	RC1_Rind(_r,load,rw,index,_ival,_fname,_fval)

#define RC1_Widx(_r, _ival, _fname, _fval)	RC1_Wind(_r,load,rw,index,_ival,_fname,_fval)

/** @} *//* end of macro documentation */

#ifdef __cplusplus
}
#endif

#endif	/* _RC1_REG_H_ */
