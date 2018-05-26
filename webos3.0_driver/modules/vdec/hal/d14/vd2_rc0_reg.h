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

#ifndef _VD2_RC0_REG_H_
#define _VD2_RC0_REG_H_

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
	0x0000 version ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	yyyymmdd                        ;   	// 31: 0
} VD2_RC0_VERSION;

/*-----------------------------------------------------------------------------
	0x0004 sw_reset ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_st_wave320_rst_ctrl         : 4,	//  0: 3
	reg_st_mach1_rst_ctrl           :12,	//  4:15
	reg_wave320_intr_ack_en         : 1,	//    16
	                                : 3,	// 17:19 reserved
	reg_wave320_srst_without_fdone  : 1,	//    20
	                                : 3,	// 21:23 reserved
	sw_reset_wave320                : 1;	//    24
} VD2_SW_RESET;

/*-----------------------------------------------------------------------------
	0x0008 apb_conf ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_apb_s0_prt                  : 4,	//  0: 3
	reg_apb_s1_prt                  : 4,	//  4: 7
	reg_apb_s2_prt                  : 4,	//  8:11
	                                :11,	// 12:22 reserved
	reg_apb_mach0_offset            : 1;	//    23
} VD2_APB_CONF;

/*-----------------------------------------------------------------------------
	0x0018 mach_conf ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_core0_intr_mode             : 1,	//     0
	reg_core1_intr_mode             : 1,	//     1
	                                : 2,	//  2: 3 reserved
	reg_core0_vpu_idle              : 1,	//     4
	reg_core0_vpu_underrun          : 1,	//     5
	                                : 2,	//  6: 7 reserved
	reg_core1_vpu_idle              : 1,	//     8
	reg_core1_vpu_underrun          : 1;	//     9
} VD2_MACH_CONF;

/*-----------------------------------------------------------------------------
	0x0040 intr_i_en ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	core0                           : 1,	//     0
	core1                           : 1,	//     1
	wave320_srst_done               : 1,	//     2
	                                : 4,	//  3: 6 reserved
	nama                            : 1,	//     7
	feed_timer                      : 1;	//     8
} VD2_INTR_I_EN;

/*-----------------------------------------------------------------------------
	0x0044 intr_i_st ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	core0                           : 1,	//     0
	core1                           : 1,	//     1
	wave320_srst_done               : 1,	//     2
	                                : 4,	//  3: 6 reserved
	nama                            : 1,	//     7
	feed_timer                      : 1;	//     8
} VD2_INTR_I_ST;

/*-----------------------------------------------------------------------------
	0x0048 intr_i_cl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	core0                           : 1,	//     0
	core1                           : 1,	//     1
	wave320_srst_done               : 1,	//     2
	                                : 4,	//  3: 6 reserved
	nama                            : 1,	//     7
	feed_timer                      : 1;	//     8
} VD2_INTR_I_CL;

/*-----------------------------------------------------------------------------
	0x0070 wave320_srst_count ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	mach0_srst_count                ;   	// 31: 0
} VD2_MACH0_SRST_COUNT;

/*-----------------------------------------------------------------------------
	0x0080 srst_mask ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	mach0_srst_mask                 : 3;	//  0: 2
} VD2_SRST_MASK;

/*-----------------------------------------------------------------------------
	0x00b0 feed_timer_conf ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	feed_timer						:28,	// 0: 27	timer value
									: 2,	// 28: 29 rsvd
	feed_timer_rst					: 1,	// 30
	feed_timer_enable				: 1;	// 31
} VD2_FEED_TIMER_CONF;

typedef struct {
	VD2_RC0_VERSION                     version                         ;	// 0x0000 : ''
	VD2_SW_RESET                       	sw_reset                        ;	// 0x0004 : ''
	VD2_APB_CONF                       	apb_conf                        ;	// 0x0008 : ''
	UINT32                          	                 __rsvd_00[   3];	// 0x000c ~ 0x0014
	VD2_MACH_CONF                      	mach_conf                       ;	// 0x0018 : ''
	UINT32                          	                 __rsvd_01[   9];	// 0x001c ~ 0x003c
	VD2_INTR_I_EN                      	intr_i_en                       ;	// 0x0040 : ''
	VD2_INTR_I_ST                      	intr_i_st                       ;	// 0x0044 : ''
	VD2_INTR_I_CL                      	intr_i_cl                       ;	// 0x0048 : ''
	UINT32                          	                 __rsvd_02[   9];	// 0x004c ~ 0x006c
	VD2_MACH0_SRST_COUNT               	mach0_srst_count                ;	// 0x0070 : ''
	UINT32                          	                 __rsvd_03[   3];	// 0x0074 ~ 0x007c
	VD2_SRST_MASK                      	srst_mask                       ;	// 0x0080 : ''
	UINT32												 __rsvd_05[	 11];	// 0x0084 ~ 0x00ac
	VD2_FEED_TIMER_CONF					feed_timer_conf					;	// 0x00b0 : ''
} VD2_RC0_REG_T;
/* 10 regs, 10 types */

/* 10 regs, 10 types in Total*/

/*
 * @{
 * Naming for register pointer.
 * gpRealRegVD2RC0 : real register of RC0.
 * gpRegVD2RC0     : shadow register.
 *
 * @def VD2_RC0_RdFL: Read  FLushing : Shadow <- Real.
 * @def VD2_RC0_WrFL: Write FLushing : Shadow -> Real.
 * @def VD2_RC0_Rd  : Read  whole register(UINT32) from Shadow register.
 * @def VD2_RC0_Wr  : Write whole register(UINT32) from Shadow register.
 * @def VD2_RC0_Rd01 ~ VD2_RC0_Rdnn: Read  given '01~nn' fields from Shadow register.
 * @def VD2_RC0_Wr01 ~ VD2_RC0_Wrnn: Write given '01~nn' fields to   Shadow register.
 * */
#define VD2_RC0_RdFL(_r)			((gpRegVD2RC0->_r)=(gpRealRegVD2RC0->_r))
#define VD2_RC0_WrFL(_r)			((gpRealRegVD2RC0->_r)=(gpRegVD2RC0->_r))

#define VD2_RC0_Rd(_r)				*((UINT32*)(&(gpRegVD2RC0->_r)))
#define VD2_RC0_Wr(_r,_v)			((VD2_RC0_Rd(_r))=((UINT32)(_v)))

#define VD2_RC0_Rd01(_r,_f01,_v01)												\
								do { 											\
									(_v01) = (gpRegVD2RC0->_r._f01);			\
								} while(0)

#define VD2_RC0_Rd02(_r,_f01,_v01,_f02,_v02)									\
								do { 											\
									(_v01) = (gpRegVD2RC0->_r._f01);			\
									(_v02) = (gpRegVD2RC0->_r._f02);			\
								} while(0)

#define VD2_RC0_Rd03(_r,_f01,_v01,_f02,_v02,_f03,_v03)							\
								do { 											\
									(_v01) = (gpRegVD2RC0->_r._f01);			\
									(_v02) = (gpRegVD2RC0->_r._f02);			\
									(_v03) = (gpRegVD2RC0->_r._f03);			\
								} while(0)

#define VD2_RC0_Rd04(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04)				\
								do { 											\
									(_v01) = (gpRegVD2RC0->_r._f01);			\
									(_v02) = (gpRegVD2RC0->_r._f02);			\
									(_v03) = (gpRegVD2RC0->_r._f03);			\
									(_v04) = (gpRegVD2RC0->_r._f04);			\
								} while(0)

#define VD2_RC0_Rd05(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,				\
					_f05,_v05)													\
								do { 											\
									(_v01) = (gpRegVD2RC0->_r._f01);			\
									(_v02) = (gpRegVD2RC0->_r._f02);			\
									(_v03) = (gpRegVD2RC0->_r._f03);			\
									(_v04) = (gpRegVD2RC0->_r._f04);			\
									(_v05) = (gpRegVD2RC0->_r._f05);			\
								} while(0)

#define VD2_RC0_Rd06(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,				\
					_f05,_v05,_f06,_v06)										\
								do { 											\
									(_v01) = (gpRegVD2RC0->_r._f01);			\
									(_v02) = (gpRegVD2RC0->_r._f02);			\
									(_v03) = (gpRegVD2RC0->_r._f03);			\
									(_v04) = (gpRegVD2RC0->_r._f04);			\
									(_v05) = (gpRegVD2RC0->_r._f05);			\
									(_v06) = (gpRegVD2RC0->_r._f06);			\
								} while(0)

#define VD2_RC0_Rd07(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,				\
					_f05,_v05,_f06,_v06,_f07,_v07)								\
								do { 											\
									(_v01) = (gpRegVD2RC0->_r._f01);			\
									(_v02) = (gpRegVD2RC0->_r._f02);			\
									(_v03) = (gpRegVD2RC0->_r._f03);			\
									(_v04) = (gpRegVD2RC0->_r._f04);			\
									(_v05) = (gpRegVD2RC0->_r._f05);			\
									(_v06) = (gpRegVD2RC0->_r._f06);			\
									(_v07) = (gpRegVD2RC0->_r._f07);			\
								} while(0)

#define VD2_RC0_Rd08(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,				\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08)					\
								do { 											\
									(_v01) = (gpRegVD2RC0->_r._f01);			\
									(_v02) = (gpRegVD2RC0->_r._f02);			\
									(_v03) = (gpRegVD2RC0->_r._f03);			\
									(_v04) = (gpRegVD2RC0->_r._f04);			\
									(_v05) = (gpRegVD2RC0->_r._f05);			\
									(_v06) = (gpRegVD2RC0->_r._f06);			\
									(_v07) = (gpRegVD2RC0->_r._f07);			\
									(_v08) = (gpRegVD2RC0->_r._f08);			\
								} while(0)

#define VD2_RC0_Rd09(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,				\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09)													\
								do { 											\
									(_v01) = (gpRegVD2RC0->_r._f01);			\
									(_v02) = (gpRegVD2RC0->_r._f02);			\
									(_v03) = (gpRegVD2RC0->_r._f03);			\
									(_v04) = (gpRegVD2RC0->_r._f04);			\
									(_v05) = (gpRegVD2RC0->_r._f05);			\
									(_v06) = (gpRegVD2RC0->_r._f06);			\
									(_v07) = (gpRegVD2RC0->_r._f07);			\
									(_v08) = (gpRegVD2RC0->_r._f08);			\
									(_v09) = (gpRegVD2RC0->_r._f09);			\
								} while(0)

#define VD2_RC0_Rd10(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,				\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10)										\
								do { 											\
									(_v01) = (gpRegVD2RC0->_r._f01);			\
									(_v02) = (gpRegVD2RC0->_r._f02);			\
									(_v03) = (gpRegVD2RC0->_r._f03);			\
									(_v04) = (gpRegVD2RC0->_r._f04);			\
									(_v05) = (gpRegVD2RC0->_r._f05);			\
									(_v06) = (gpRegVD2RC0->_r._f06);			\
									(_v07) = (gpRegVD2RC0->_r._f07);			\
									(_v08) = (gpRegVD2RC0->_r._f08);			\
									(_v09) = (gpRegVD2RC0->_r._f09);			\
									(_v10) = (gpRegVD2RC0->_r._f10);			\
								} while(0)

#define VD2_RC0_Rd11(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,				\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11)								\
								do { 											\
									(_v01) = (gpRegVD2RC0->_r._f01);			\
									(_v02) = (gpRegVD2RC0->_r._f02);			\
									(_v03) = (gpRegVD2RC0->_r._f03);			\
									(_v04) = (gpRegVD2RC0->_r._f04);			\
									(_v05) = (gpRegVD2RC0->_r._f05);			\
									(_v06) = (gpRegVD2RC0->_r._f06);			\
									(_v07) = (gpRegVD2RC0->_r._f07);			\
									(_v08) = (gpRegVD2RC0->_r._f08);			\
									(_v09) = (gpRegVD2RC0->_r._f09);			\
									(_v10) = (gpRegVD2RC0->_r._f10);			\
									(_v11) = (gpRegVD2RC0->_r._f11);			\
								} while(0)

#define VD2_RC0_Rd12(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,				\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12)					\
								do { 											\
									(_v01) = (gpRegVD2RC0->_r._f01);			\
									(_v02) = (gpRegVD2RC0->_r._f02);			\
									(_v03) = (gpRegVD2RC0->_r._f03);			\
									(_v04) = (gpRegVD2RC0->_r._f04);			\
									(_v05) = (gpRegVD2RC0->_r._f05);			\
									(_v06) = (gpRegVD2RC0->_r._f06);			\
									(_v07) = (gpRegVD2RC0->_r._f07);			\
									(_v08) = (gpRegVD2RC0->_r._f08);			\
									(_v09) = (gpRegVD2RC0->_r._f09);			\
									(_v10) = (gpRegVD2RC0->_r._f10);			\
									(_v11) = (gpRegVD2RC0->_r._f11);			\
									(_v12) = (gpRegVD2RC0->_r._f12);			\
								} while(0)

#define VD2_RC0_Rd13(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,				\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13)													\
								do { 											\
									(_v01) = (gpRegVD2RC0->_r._f01);			\
									(_v02) = (gpRegVD2RC0->_r._f02);			\
									(_v03) = (gpRegVD2RC0->_r._f03);			\
									(_v04) = (gpRegVD2RC0->_r._f04);			\
									(_v05) = (gpRegVD2RC0->_r._f05);			\
									(_v06) = (gpRegVD2RC0->_r._f06);			\
									(_v07) = (gpRegVD2RC0->_r._f07);			\
									(_v08) = (gpRegVD2RC0->_r._f08);			\
									(_v09) = (gpRegVD2RC0->_r._f09);			\
									(_v10) = (gpRegVD2RC0->_r._f10);			\
									(_v11) = (gpRegVD2RC0->_r._f11);			\
									(_v12) = (gpRegVD2RC0->_r._f12);			\
									(_v13) = (gpRegVD2RC0->_r._f13);			\
								} while(0)

#define VD2_RC0_Rd14(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,				\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14)										\
								do { 											\
									(_v01) = (gpRegVD2RC0->_r._f01);			\
									(_v02) = (gpRegVD2RC0->_r._f02);			\
									(_v03) = (gpRegVD2RC0->_r._f03);			\
									(_v04) = (gpRegVD2RC0->_r._f04);			\
									(_v05) = (gpRegVD2RC0->_r._f05);			\
									(_v06) = (gpRegVD2RC0->_r._f06);			\
									(_v07) = (gpRegVD2RC0->_r._f07);			\
									(_v08) = (gpRegVD2RC0->_r._f08);			\
									(_v09) = (gpRegVD2RC0->_r._f09);			\
									(_v10) = (gpRegVD2RC0->_r._f10);			\
									(_v11) = (gpRegVD2RC0->_r._f11);			\
									(_v12) = (gpRegVD2RC0->_r._f12);			\
									(_v13) = (gpRegVD2RC0->_r._f13);			\
									(_v14) = (gpRegVD2RC0->_r._f14);			\
								} while(0)

#define VD2_RC0_Rd15(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,				\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15)								\
								do { 											\
									(_v01) = (gpRegVD2RC0->_r._f01);			\
									(_v02) = (gpRegVD2RC0->_r._f02);			\
									(_v03) = (gpRegVD2RC0->_r._f03);			\
									(_v04) = (gpRegVD2RC0->_r._f04);			\
									(_v05) = (gpRegVD2RC0->_r._f05);			\
									(_v06) = (gpRegVD2RC0->_r._f06);			\
									(_v07) = (gpRegVD2RC0->_r._f07);			\
									(_v08) = (gpRegVD2RC0->_r._f08);			\
									(_v09) = (gpRegVD2RC0->_r._f09);			\
									(_v10) = (gpRegVD2RC0->_r._f10);			\
									(_v11) = (gpRegVD2RC0->_r._f11);			\
									(_v12) = (gpRegVD2RC0->_r._f12);			\
									(_v13) = (gpRegVD2RC0->_r._f13);			\
									(_v14) = (gpRegVD2RC0->_r._f14);			\
									(_v15) = (gpRegVD2RC0->_r._f15);			\
								} while(0)

#define VD2_RC0_Rd16(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,				\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15,_f16,_v16)					\
								do { 											\
									(_v01) = (gpRegVD2RC0->_r._f01);			\
									(_v02) = (gpRegVD2RC0->_r._f02);			\
									(_v03) = (gpRegVD2RC0->_r._f03);			\
									(_v04) = (gpRegVD2RC0->_r._f04);			\
									(_v05) = (gpRegVD2RC0->_r._f05);			\
									(_v06) = (gpRegVD2RC0->_r._f06);			\
									(_v07) = (gpRegVD2RC0->_r._f07);			\
									(_v08) = (gpRegVD2RC0->_r._f08);			\
									(_v09) = (gpRegVD2RC0->_r._f09);			\
									(_v10) = (gpRegVD2RC0->_r._f10);			\
									(_v11) = (gpRegVD2RC0->_r._f11);			\
									(_v12) = (gpRegVD2RC0->_r._f12);			\
									(_v13) = (gpRegVD2RC0->_r._f13);			\
									(_v14) = (gpRegVD2RC0->_r._f14);			\
									(_v15) = (gpRegVD2RC0->_r._f15);			\
									(_v16) = (gpRegVD2RC0->_r._f16);			\
								} while(0)


#define VD2_RC0_Wr01(_r,_f01,_v01)												\
								do { 											\
									(gpRegVD2RC0->_r._f01) = (_v01);			\
								} while(0)

#define VD2_RC0_Wr02(_r,_f01,_v01,_f02,_v02)									\
								do { 											\
									(gpRegVD2RC0->_r._f01) = (_v01);			\
									(gpRegVD2RC0->_r._f02) = (_v02);			\
								} while(0)

#define VD2_RC0_Wr03(_r,_f01,_v01,_f02,_v02,_f03,_v03)							\
								do { 											\
									(gpRegVD2RC0->_r._f01) = (_v01);			\
									(gpRegVD2RC0->_r._f02) = (_v02);			\
									(gpRegVD2RC0->_r._f03) = (_v03);			\
								} while(0)

#define VD2_RC0_Wr04(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04)				\
								do { 											\
									(gpRegVD2RC0->_r._f01) = (_v01);			\
									(gpRegVD2RC0->_r._f02) = (_v02);			\
									(gpRegVD2RC0->_r._f03) = (_v03);			\
									(gpRegVD2RC0->_r._f04) = (_v04);			\
								} while(0)

#define VD2_RC0_Wr05(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,				\
					_f05,_v05)													\
								do { 											\
									(gpRegVD2RC0->_r._f01) = (_v01);			\
									(gpRegVD2RC0->_r._f02) = (_v02);			\
									(gpRegVD2RC0->_r._f03) = (_v03);			\
									(gpRegVD2RC0->_r._f04) = (_v04);			\
									(gpRegVD2RC0->_r._f05) = (_v05);			\
								} while(0)

#define VD2_RC0_Wr06(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,				\
					_f05,_v05,_f06,_v06)										\
								do { 											\
									(gpRegVD2RC0->_r._f01) = (_v01);			\
									(gpRegVD2RC0->_r._f02) = (_v02);			\
									(gpRegVD2RC0->_r._f03) = (_v03);			\
									(gpRegVD2RC0->_r._f04) = (_v04);			\
									(gpRegVD2RC0->_r._f05) = (_v05);			\
									(gpRegVD2RC0->_r._f06) = (_v06);			\
								} while(0)

#define VD2_RC0_Wr07(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,				\
					_f05,_v05,_f06,_v06,_f07,_v07)								\
								do { 											\
									(gpRegVD2RC0->_r._f01) = (_v01);			\
									(gpRegVD2RC0->_r._f02) = (_v02);			\
									(gpRegVD2RC0->_r._f03) = (_v03);			\
									(gpRegVD2RC0->_r._f04) = (_v04);			\
									(gpRegVD2RC0->_r._f05) = (_v05);			\
									(gpRegVD2RC0->_r._f06) = (_v06);			\
									(gpRegVD2RC0->_r._f07) = (_v07);			\
								} while(0)

#define VD2_RC0_Wr08(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,				\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08)					\
								do { 											\
									(gpRegVD2RC0->_r._f01) = (_v01);			\
									(gpRegVD2RC0->_r._f02) = (_v02);			\
									(gpRegVD2RC0->_r._f03) = (_v03);			\
									(gpRegVD2RC0->_r._f04) = (_v04);			\
									(gpRegVD2RC0->_r._f05) = (_v05);			\
									(gpRegVD2RC0->_r._f06) = (_v06);			\
									(gpRegVD2RC0->_r._f07) = (_v07);			\
									(gpRegVD2RC0->_r._f08) = (_v08);			\
								} while(0)

#define VD2_RC0_Wr09(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,				\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09)													\
								do { 											\
									(gpRegVD2RC0->_r._f01) = (_v01);			\
									(gpRegVD2RC0->_r._f02) = (_v02);			\
									(gpRegVD2RC0->_r._f03) = (_v03);			\
									(gpRegVD2RC0->_r._f04) = (_v04);			\
									(gpRegVD2RC0->_r._f05) = (_v05);			\
									(gpRegVD2RC0->_r._f06) = (_v06);			\
									(gpRegVD2RC0->_r._f07) = (_v07);			\
									(gpRegVD2RC0->_r._f08) = (_v08);			\
									(gpRegVD2RC0->_r._f09) = (_v09);			\
								} while(0)

#define VD2_RC0_Wr10(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,				\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10)										\
								do { 											\
									(gpRegVD2RC0->_r._f01) = (_v01);			\
									(gpRegVD2RC0->_r._f02) = (_v02);			\
									(gpRegVD2RC0->_r._f03) = (_v03);			\
									(gpRegVD2RC0->_r._f04) = (_v04);			\
									(gpRegVD2RC0->_r._f05) = (_v05);			\
									(gpRegVD2RC0->_r._f06) = (_v06);			\
									(gpRegVD2RC0->_r._f07) = (_v07);			\
									(gpRegVD2RC0->_r._f08) = (_v08);			\
									(gpRegVD2RC0->_r._f09) = (_v09);			\
									(gpRegVD2RC0->_r._f10) = (_v10);			\
								} while(0)

#define VD2_RC0_Wr11(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,				\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11)								\
								do { 											\
									(gpRegVD2RC0->_r._f01) = (_v01);			\
									(gpRegVD2RC0->_r._f02) = (_v02);			\
									(gpRegVD2RC0->_r._f03) = (_v03);			\
									(gpRegVD2RC0->_r._f04) = (_v04);			\
									(gpRegVD2RC0->_r._f05) = (_v05);			\
									(gpRegVD2RC0->_r._f06) = (_v06);			\
									(gpRegVD2RC0->_r._f07) = (_v07);			\
									(gpRegVD2RC0->_r._f08) = (_v08);			\
									(gpRegVD2RC0->_r._f09) = (_v09);			\
									(gpRegVD2RC0->_r._f10) = (_v10);			\
									(gpRegVD2RC0->_r._f11) = (_v11);			\
								} while(0)

#define VD2_RC0_Wr12(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,				\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12)					\
								do { 											\
									(gpRegVD2RC0->_r._f01) = (_v01);			\
									(gpRegVD2RC0->_r._f02) = (_v02);			\
									(gpRegVD2RC0->_r._f03) = (_v03);			\
									(gpRegVD2RC0->_r._f04) = (_v04);			\
									(gpRegVD2RC0->_r._f05) = (_v05);			\
									(gpRegVD2RC0->_r._f06) = (_v06);			\
									(gpRegVD2RC0->_r._f07) = (_v07);			\
									(gpRegVD2RC0->_r._f08) = (_v08);			\
									(gpRegVD2RC0->_r._f09) = (_v09);			\
									(gpRegVD2RC0->_r._f10) = (_v10);			\
									(gpRegVD2RC0->_r._f11) = (_v11);			\
									(gpRegVD2RC0->_r._f12) = (_v12);			\
								} while(0)

#define VD2_RC0_Wr13(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,				\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13)													\
								do { 											\
									(gpRegVD2RC0->_r._f01) = (_v01);			\
									(gpRegVD2RC0->_r._f02) = (_v02);			\
									(gpRegVD2RC0->_r._f03) = (_v03);			\
									(gpRegVD2RC0->_r._f04) = (_v04);			\
									(gpRegVD2RC0->_r._f05) = (_v05);			\
									(gpRegVD2RC0->_r._f06) = (_v06);			\
									(gpRegVD2RC0->_r._f07) = (_v07);			\
									(gpRegVD2RC0->_r._f08) = (_v08);			\
									(gpRegVD2RC0->_r._f09) = (_v09);			\
									(gpRegVD2RC0->_r._f10) = (_v10);			\
									(gpRegVD2RC0->_r._f11) = (_v11);			\
									(gpRegVD2RC0->_r._f12) = (_v12);			\
									(gpRegVD2RC0->_r._f13) = (_v13);			\
								} while(0)

#define VD2_RC0_Wr14(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,				\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14)										\
								do { 											\
									(gpRegVD2RC0->_r._f01) = (_v01);			\
									(gpRegVD2RC0->_r._f02) = (_v02);			\
									(gpRegVD2RC0->_r._f03) = (_v03);			\
									(gpRegVD2RC0->_r._f04) = (_v04);			\
									(gpRegVD2RC0->_r._f05) = (_v05);			\
									(gpRegVD2RC0->_r._f06) = (_v06);			\
									(gpRegVD2RC0->_r._f07) = (_v07);			\
									(gpRegVD2RC0->_r._f08) = (_v08);			\
									(gpRegVD2RC0->_r._f09) = (_v09);			\
									(gpRegVD2RC0->_r._f10) = (_v10);			\
									(gpRegVD2RC0->_r._f11) = (_v11);			\
									(gpRegVD2RC0->_r._f12) = (_v12);			\
									(gpRegVD2RC0->_r._f13) = (_v13);			\
									(gpRegVD2RC0->_r._f14) = (_v14);			\
								} while(0)

#define VD2_RC0_Wr15(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,				\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15)								\
								do { 											\
									(gpRegVD2RC0->_r._f01) = (_v01);			\
									(gpRegVD2RC0->_r._f02) = (_v02);			\
									(gpRegVD2RC0->_r._f03) = (_v03);			\
									(gpRegVD2RC0->_r._f04) = (_v04);			\
									(gpRegVD2RC0->_r._f05) = (_v05);			\
									(gpRegVD2RC0->_r._f06) = (_v06);			\
									(gpRegVD2RC0->_r._f07) = (_v07);			\
									(gpRegVD2RC0->_r._f08) = (_v08);			\
									(gpRegVD2RC0->_r._f09) = (_v09);			\
									(gpRegVD2RC0->_r._f10) = (_v10);			\
									(gpRegVD2RC0->_r._f11) = (_v11);			\
									(gpRegVD2RC0->_r._f12) = (_v12);			\
									(gpRegVD2RC0->_r._f13) = (_v13);			\
									(gpRegVD2RC0->_r._f14) = (_v14);			\
									(gpRegVD2RC0->_r._f15) = (_v15);			\
								} while(0)

#define VD2_RC0_Wr16(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,				\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15,_f16,_v16)					\
								do { 											\
									(gpRegVD2RC0->_r._f01) = (_v01);			\
									(gpRegVD2RC0->_r._f02) = (_v02);			\
									(gpRegVD2RC0->_r._f03) = (_v03);			\
									(gpRegVD2RC0->_r._f04) = (_v04);			\
									(gpRegVD2RC0->_r._f05) = (_v05);			\
									(gpRegVD2RC0->_r._f06) = (_v06);			\
									(gpRegVD2RC0->_r._f07) = (_v07);			\
									(gpRegVD2RC0->_r._f08) = (_v08);			\
									(gpRegVD2RC0->_r._f09) = (_v09);			\
									(gpRegVD2RC0->_r._f10) = (_v10);			\
									(gpRegVD2RC0->_r._f11) = (_v11);			\
									(gpRegVD2RC0->_r._f12) = (_v12);			\
									(gpRegVD2RC0->_r._f13) = (_v13);			\
									(gpRegVD2RC0->_r._f14) = (_v14);			\
									(gpRegVD2RC0->_r._f15) = (_v15);			\
									(gpRegVD2RC0->_r._f16) = (_v16);			\
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
 * _rwname: name of rw    bit field : shall be 0 after VD2_RC0_Wind(), 1 for VD2_RC0_Rind()
 * _iname : name of index bit field
 * _ival  : index value
 * _fname : field name
 * _fval  : field variable that field value shall be stored.
 *
 * VD2_RC0_Rind : General indexed register Read.(
 * VD2_RC0_Wind : General indexed register Read.
 *
 * VD2_RC0_Ridx : For 'index', 'rw', 'load' field name
 * VD2_RC0_Widx : For 'index', 'rw', 'load' field name and NO_LOAD.
 */
#define VD2_RC0_Rind(_r, _lname, _rwname, _iname, _ival, _fname, _fval)				\
							do {													\
								VD2_RC0_Wr03(_r,_lname,0,_rwname,1,_iname,_ival);	\
								VD2_RC0_WrFL(_r);									\
								VD2_RC0_RdFL(_r);									\
								VD2_RC0_Rd01(_r,_fname,_fval);						\
							} while (0)

#define VD2_RC0_Wind(_r, _lname, _rwname, _iname, _ival, _fname, _fval)				\
				VD2_RC0_Wr04(_r, _lname,0, _rwname,0, _iname,_ival, _fname,_fval)


#define VD2_RC0_Ridx(_r, _ival, _fname, _fval)	VD2_RC0_Rind(_r,load,rw,index,_ival,_fname,_fval)

#define VD2_RC0_Widx(_r, _ival, _fname, _fval)	VD2_RC0_Wind(_r,load,rw,index,_ival,_fname,_fval)

/** @} *//* end of macro documentation */

#define gpRealRegVD2RC0		pstVD2RC0_Reg
#define gpRegVD2RC0			stVD2RC0_RegShadow

#ifdef __cplusplus
}
#endif

#endif	/* _VD2_RC0_REG_H_ */
