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

#ifndef _VD3_RC_REG_H_
#define _VD3_RC_REG_H_

#include "vdec_type_defs.h"

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
} RC_VERSION;

/*-----------------------------------------------------------------------------
	0x0004 sw_reset ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reserve00			            : 12,	//  0:11
	reg_st_vdo_rst_ctrl				: 4,	// 12:15
	reserve01			            : 3,	// 16:18
	reg_vdo_intr_ack_en             : 1,	//    19
	reserve02					    : 3,	// 20:22
	reg_vdo_srst_without_fdone      : 1,	//    23
	sw_reset_vdo                	: 1;	//    24
} SW_RESET;

/*-----------------------------------------------------------------------------
	0x0008 apb_conf ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_apb_s0_prt                  : 4,	//  0: 3
	reg_apb_s1_prt                  : 4,	//  4: 7
	reg_apb_s2_prt                  : 4;	//  8:11
} APB_CONF;

/*-----------------------------------------------------------------------------
	0x0014 vsync_src_sel ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 8,	//  0: 7 reserved
	reg_vsync0_src_sel				: 2,	//  9: 8
	reg_vsync1_src_sel				: 2,	// 11:10
	reg_vsync2_src_sel				: 2,	// 13:12
	reg_vsync3_src_sel				: 2;	// 15:14
} VSYNC_SRC_SEL;

/*-----------------------------------------------------------------------------
	0x001c timer_src_sel ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32

	reg_timer0_src_sel             : 2,	//  0: 1
	reg_timer1_src_sel             : 2,	//  2: 3
	reg_timer2_src_sel             : 2,	//  4: 5
	reg_timer3_src_sel             : 2,	//  6: 7
	resvd       					: 24;
} TIMER_SRC_SEL;

/*-----------------------------------------------------------------------------
	0x0040 intr_i_en ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pic0_done                       : 1,	//     0
	pic1_done                       : 1,	//     1
	gdi_err                         : 1,	//     2
	vdo_srst_done                   : 1,	//     3
	vsync0                          : 1,	//     4
	vsync1                          : 1,	//     5
	vsync2                          : 1,	//     6
	vsync3                          : 1,	//     7
	timer0	                        : 1,	//     8
	timer1	                        : 1,	//     9
	timer2	                        : 1,	//    10
	timer3	                        : 1,	//    11
	vsync0_rdy		                : 1,	//    12
	vsync1_rdy		                : 1;	//    13
} INTR_I_EN;

/*-----------------------------------------------------------------------------
	0x0044 intr_i_st ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pic0_done                       : 1,	//     0
	pic1_done                       : 1,	//     1
	gdi_err                         : 1,	//     2
	vdo_srst_done                   : 1,	//     3
	vsync0                          : 1,	//     4
	vsync1                          : 1,	//     5
	vsync2                          : 1,	//     6
	vsync3                          : 1,	//     7
	timer0	                        : 1,	//     8
	timer1	                        : 1,	//     9
	timer2	                        : 1,	//    10
	timer3	                        : 1,	//    11
	vsync0_rdy		                : 1,	//    12
	vsync1_rdy		                : 1;	//    13
} INTR_I_ST;

/*-----------------------------------------------------------------------------
	0x0048 intr_i_cl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pic0_done                       : 1,	//     0
	pic1_done                       : 1,	//     1
	gdi_err                         : 1,	//     2
	vdo_srst_done                   : 1,	//     3
	vsync0                          : 1,	//     4
	vsync1                          : 1,	//     5
	vsync2                          : 1,	//     6
	vsync3                          : 1,	//     7
	timer0	                        : 1,	//     8
	timer1	                        : 1,	//     9
	timer2	                        : 1,	//    10
	timer3	                        : 1,	//    11
	vsync0_rdy		                : 1,	//    12
	vsync1_rdy		                : 1;	//    13
} INTR_I_CL;

/*-----------------------------------------------------------------------------
	0x007c vdo_srst_count ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	mach3_srst_count                ;   	// 31: 0
} VDO_SRST_COUNT;

/*-----------------------------------------------------------------------------
	0x0080 srst_mask ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
					                 : 12,	//  0:11
	vdo_srst_mask   	             : 3;	// 12:14
} SRST_MASK;

typedef struct {
	RC_VERSION                    	 	version                         ;	// 0x0000 : ''
	SW_RESET                        	sw_reset                        ;	// 0x0004 : ''
	APB_CONF                        	apb_conf                        ;	// 0x0008 : ''
	UINT32                              __rsvd_00[   2]					;	// 0x000c : 0x0010
	VSYNC_SRC_SEL                      	vsync_src_sel                   ;	// 0x0014 : ''
	UINT32		                       	__rsvd_11[   1]                 ;	// 0x0018 : ''
	TIMER_SRC_SEL                      	timer_src_sel                   ;	// 0x001c : ''
	UINT32                              __rsvd_01[   8]					;	// 0x0020 : 0x003c
	UINT32 		                      	intr_i_en                       ;	// 0x0040 : ''
	UINT32      		               	intr_i_st                       ;	// 0x0044 : ''
	UINT32              	         	intr_i_cl                       ;	// 0x0048 : ''
	UINT32            	                __rsvd_02[  12]					;	// 0x004c : 0x0078
	VDO_SRST_COUNT                  	vdo_srst_count                  ;	// 0x007c : ''
	SRST_MASK                       	srst_mask                       ;	// 0x0080 : ''
} VD3_RC_REG_T;
/* 27 regs, 27 types */

/* 28 regs, 28 types in Total*/

#define gpRealRegVd3Rc0		pstVd3RcReg
#define gpRegVd3Rc0			pstVd3RcShd

/*
 * @{
 * Naming for register pointer.
 * gpRealRegVd3Rc0 : real register of RC0.
 * gpRegVd3Rc0     : shadow register.
 *
 * @def VD3_RC_RdFL: Read  FLushing : Shadow <- Real.
 * @def VD3_RC_WrFL: Write FLushing : Shadow -> Real.
 * @def VD3_RC_Rd  : Read  whole register(UINT32) from Shadow register.
 * @def VD3_RC_Wr  : Write whole register(UINT32) from Shadow register.
 * @def VD3_RC_Rd01 ~ VD3_RC_Rdnn: Read  given '01~nn' fields from Shadow register.
 * @def VD3_RC_Wr01 ~ VD3_RC_Wrnn: Write given '01~nn' fields to   Shadow register.
 * */
#define VD3_RC_RdFL(_r)			((gpRegVd3Rc0->_r)=(gpRealRegVd3Rc0->_r))
#define VD3_RC_WrFL(_r)			((gpRealRegVd3Rc0->_r)=(gpRegVd3Rc0->_r))

#define VD3_RC_Rd(_r)			*((UINT32*)(&(gpRegVd3Rc0->_r)))
#define VD3_RC_Wr(_r,_v)			((VD3_RC_Rd(_r))=((UINT32)(_v)))

#define VD3_RC_Rd01(_r,_f01,_v01)													\
								do { 											\
									(_v01) = (gpRegVd3Rc0->_r._f01);				\
								} while(0)

#define VD3_RC_Rd02(_r,_f01,_v01,_f02,_v02)										\
								do { 											\
									(_v01) = (gpRegVd3Rc0->_r._f01);				\
									(_v02) = (gpRegVd3Rc0->_r._f02);				\
								} while(0)

#define VD3_RC_Rd03(_r,_f01,_v01,_f02,_v02,_f03,_v03)								\
								do { 											\
									(_v01) = (gpRegVd3Rc0->_r._f01);				\
									(_v02) = (gpRegVd3Rc0->_r._f02);				\
									(_v03) = (gpRegVd3Rc0->_r._f03);				\
								} while(0)

#define VD3_RC_Rd04(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04)					\
								do { 											\
									(_v01) = (gpRegVd3Rc0->_r._f01);				\
									(_v02) = (gpRegVd3Rc0->_r._f02);				\
									(_v03) = (gpRegVd3Rc0->_r._f03);				\
									(_v04) = (gpRegVd3Rc0->_r._f04);				\
								} while(0)

#define VD3_RC_Rd05(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05)													\
								do { 											\
									(_v01) = (gpRegVd3Rc0->_r._f01);				\
									(_v02) = (gpRegVd3Rc0->_r._f02);				\
									(_v03) = (gpRegVd3Rc0->_r._f03);				\
									(_v04) = (gpRegVd3Rc0->_r._f04);				\
									(_v05) = (gpRegVd3Rc0->_r._f05);				\
								} while(0)

#define VD3_RC_Rd06(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06)										\
								do { 											\
									(_v01) = (gpRegVd3Rc0->_r._f01);				\
									(_v02) = (gpRegVd3Rc0->_r._f02);				\
									(_v03) = (gpRegVd3Rc0->_r._f03);				\
									(_v04) = (gpRegVd3Rc0->_r._f04);				\
									(_v05) = (gpRegVd3Rc0->_r._f05);				\
									(_v06) = (gpRegVd3Rc0->_r._f06);				\
								} while(0)

#define VD3_RC_Rd07(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07)								\
								do { 											\
									(_v01) = (gpRegVd3Rc0->_r._f01);				\
									(_v02) = (gpRegVd3Rc0->_r._f02);				\
									(_v03) = (gpRegVd3Rc0->_r._f03);				\
									(_v04) = (gpRegVd3Rc0->_r._f04);				\
									(_v05) = (gpRegVd3Rc0->_r._f05);				\
									(_v06) = (gpRegVd3Rc0->_r._f06);				\
									(_v07) = (gpRegVd3Rc0->_r._f07);				\
								} while(0)

#define VD3_RC_Rd08(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08)					\
								do { 											\
									(_v01) = (gpRegVd3Rc0->_r._f01);				\
									(_v02) = (gpRegVd3Rc0->_r._f02);				\
									(_v03) = (gpRegVd3Rc0->_r._f03);				\
									(_v04) = (gpRegVd3Rc0->_r._f04);				\
									(_v05) = (gpRegVd3Rc0->_r._f05);				\
									(_v06) = (gpRegVd3Rc0->_r._f06);				\
									(_v07) = (gpRegVd3Rc0->_r._f07);				\
									(_v08) = (gpRegVd3Rc0->_r._f08);				\
								} while(0)

#define VD3_RC_Rd09(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09)													\
								do { 											\
									(_v01) = (gpRegVd3Rc0->_r._f01);				\
									(_v02) = (gpRegVd3Rc0->_r._f02);				\
									(_v03) = (gpRegVd3Rc0->_r._f03);				\
									(_v04) = (gpRegVd3Rc0->_r._f04);				\
									(_v05) = (gpRegVd3Rc0->_r._f05);				\
									(_v06) = (gpRegVd3Rc0->_r._f06);				\
									(_v07) = (gpRegVd3Rc0->_r._f07);				\
									(_v08) = (gpRegVd3Rc0->_r._f08);				\
									(_v09) = (gpRegVd3Rc0->_r._f09);				\
								} while(0)

#define VD3_RC_Rd10(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10)										\
								do { 											\
									(_v01) = (gpRegVd3Rc0->_r._f01);				\
									(_v02) = (gpRegVd3Rc0->_r._f02);				\
									(_v03) = (gpRegVd3Rc0->_r._f03);				\
									(_v04) = (gpRegVd3Rc0->_r._f04);				\
									(_v05) = (gpRegVd3Rc0->_r._f05);				\
									(_v06) = (gpRegVd3Rc0->_r._f06);				\
									(_v07) = (gpRegVd3Rc0->_r._f07);				\
									(_v08) = (gpRegVd3Rc0->_r._f08);				\
									(_v09) = (gpRegVd3Rc0->_r._f09);				\
									(_v10) = (gpRegVd3Rc0->_r._f10);				\
								} while(0)

#define VD3_RC_Rd11(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11)								\
								do { 											\
									(_v01) = (gpRegVd3Rc0->_r._f01);				\
									(_v02) = (gpRegVd3Rc0->_r._f02);				\
									(_v03) = (gpRegVd3Rc0->_r._f03);				\
									(_v04) = (gpRegVd3Rc0->_r._f04);				\
									(_v05) = (gpRegVd3Rc0->_r._f05);				\
									(_v06) = (gpRegVd3Rc0->_r._f06);				\
									(_v07) = (gpRegVd3Rc0->_r._f07);				\
									(_v08) = (gpRegVd3Rc0->_r._f08);				\
									(_v09) = (gpRegVd3Rc0->_r._f09);				\
									(_v10) = (gpRegVd3Rc0->_r._f10);				\
									(_v11) = (gpRegVd3Rc0->_r._f11);				\
								} while(0)

#define VD3_RC_Rd12(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12)					\
								do { 											\
									(_v01) = (gpRegVd3Rc0->_r._f01);				\
									(_v02) = (gpRegVd3Rc0->_r._f02);				\
									(_v03) = (gpRegVd3Rc0->_r._f03);				\
									(_v04) = (gpRegVd3Rc0->_r._f04);				\
									(_v05) = (gpRegVd3Rc0->_r._f05);				\
									(_v06) = (gpRegVd3Rc0->_r._f06);				\
									(_v07) = (gpRegVd3Rc0->_r._f07);				\
									(_v08) = (gpRegVd3Rc0->_r._f08);				\
									(_v09) = (gpRegVd3Rc0->_r._f09);				\
									(_v10) = (gpRegVd3Rc0->_r._f10);				\
									(_v11) = (gpRegVd3Rc0->_r._f11);				\
									(_v12) = (gpRegVd3Rc0->_r._f12);				\
								} while(0)

#define VD3_RC_Rd13(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13)													\
								do { 											\
									(_v01) = (gpRegVd3Rc0->_r._f01);				\
									(_v02) = (gpRegVd3Rc0->_r._f02);				\
									(_v03) = (gpRegVd3Rc0->_r._f03);				\
									(_v04) = (gpRegVd3Rc0->_r._f04);				\
									(_v05) = (gpRegVd3Rc0->_r._f05);				\
									(_v06) = (gpRegVd3Rc0->_r._f06);				\
									(_v07) = (gpRegVd3Rc0->_r._f07);				\
									(_v08) = (gpRegVd3Rc0->_r._f08);				\
									(_v09) = (gpRegVd3Rc0->_r._f09);				\
									(_v10) = (gpRegVd3Rc0->_r._f10);				\
									(_v11) = (gpRegVd3Rc0->_r._f11);				\
									(_v12) = (gpRegVd3Rc0->_r._f12);				\
									(_v13) = (gpRegVd3Rc0->_r._f13);				\
								} while(0)

#define VD3_RC_Rd14(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14)										\
								do { 											\
									(_v01) = (gpRegVd3Rc0->_r._f01);				\
									(_v02) = (gpRegVd3Rc0->_r._f02);				\
									(_v03) = (gpRegVd3Rc0->_r._f03);				\
									(_v04) = (gpRegVd3Rc0->_r._f04);				\
									(_v05) = (gpRegVd3Rc0->_r._f05);				\
									(_v06) = (gpRegVd3Rc0->_r._f06);				\
									(_v07) = (gpRegVd3Rc0->_r._f07);				\
									(_v08) = (gpRegVd3Rc0->_r._f08);				\
									(_v09) = (gpRegVd3Rc0->_r._f09);				\
									(_v10) = (gpRegVd3Rc0->_r._f10);				\
									(_v11) = (gpRegVd3Rc0->_r._f11);				\
									(_v12) = (gpRegVd3Rc0->_r._f12);				\
									(_v13) = (gpRegVd3Rc0->_r._f13);				\
									(_v14) = (gpRegVd3Rc0->_r._f14);				\
								} while(0)

#define VD3_RC_Rd15(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15)								\
								do { 											\
									(_v01) = (gpRegVd3Rc0->_r._f01);				\
									(_v02) = (gpRegVd3Rc0->_r._f02);				\
									(_v03) = (gpRegVd3Rc0->_r._f03);				\
									(_v04) = (gpRegVd3Rc0->_r._f04);				\
									(_v05) = (gpRegVd3Rc0->_r._f05);				\
									(_v06) = (gpRegVd3Rc0->_r._f06);				\
									(_v07) = (gpRegVd3Rc0->_r._f07);				\
									(_v08) = (gpRegVd3Rc0->_r._f08);				\
									(_v09) = (gpRegVd3Rc0->_r._f09);				\
									(_v10) = (gpRegVd3Rc0->_r._f10);				\
									(_v11) = (gpRegVd3Rc0->_r._f11);				\
									(_v12) = (gpRegVd3Rc0->_r._f12);				\
									(_v13) = (gpRegVd3Rc0->_r._f13);				\
									(_v14) = (gpRegVd3Rc0->_r._f14);				\
									(_v15) = (gpRegVd3Rc0->_r._f15);				\
								} while(0)

#define VD3_RC_Rd16(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15,_f16,_v16)					\
								do { 											\
									(_v01) = (gpRegVd3Rc0->_r._f01);				\
									(_v02) = (gpRegVd3Rc0->_r._f02);				\
									(_v03) = (gpRegVd3Rc0->_r._f03);				\
									(_v04) = (gpRegVd3Rc0->_r._f04);				\
									(_v05) = (gpRegVd3Rc0->_r._f05);				\
									(_v06) = (gpRegVd3Rc0->_r._f06);				\
									(_v07) = (gpRegVd3Rc0->_r._f07);				\
									(_v08) = (gpRegVd3Rc0->_r._f08);				\
									(_v09) = (gpRegVd3Rc0->_r._f09);				\
									(_v10) = (gpRegVd3Rc0->_r._f10);				\
									(_v11) = (gpRegVd3Rc0->_r._f11);				\
									(_v12) = (gpRegVd3Rc0->_r._f12);				\
									(_v13) = (gpRegVd3Rc0->_r._f13);				\
									(_v14) = (gpRegVd3Rc0->_r._f14);				\
									(_v15) = (gpRegVd3Rc0->_r._f15);				\
									(_v16) = (gpRegVd3Rc0->_r._f16);				\
								} while(0)


#define VD3_RC_Wr01(_r,_f01,_v01)													\
								do { 											\
									(gpRegVd3Rc0->_r._f01) = (_v01);				\
								} while(0)

#define VD3_RC_Wr02(_r,_f01,_v01,_f02,_v02)										\
								do { 											\
									(gpRegVd3Rc0->_r._f01) = (_v01);				\
									(gpRegVd3Rc0->_r._f02) = (_v02);				\
								} while(0)

#define VD3_RC_Wr03(_r,_f01,_v01,_f02,_v02,_f03,_v03)								\
								do { 											\
									(gpRegVd3Rc0->_r._f01) = (_v01);				\
									(gpRegVd3Rc0->_r._f02) = (_v02);				\
									(gpRegVd3Rc0->_r._f03) = (_v03);				\
								} while(0)

#define VD3_RC_Wr04(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04)					\
								do { 											\
									(gpRegVd3Rc0->_r._f01) = (_v01);				\
									(gpRegVd3Rc0->_r._f02) = (_v02);				\
									(gpRegVd3Rc0->_r._f03) = (_v03);				\
									(gpRegVd3Rc0->_r._f04) = (_v04);				\
								} while(0)

#define VD3_RC_Wr05(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05)													\
								do { 											\
									(gpRegVd3Rc0->_r._f01) = (_v01);				\
									(gpRegVd3Rc0->_r._f02) = (_v02);				\
									(gpRegVd3Rc0->_r._f03) = (_v03);				\
									(gpRegVd3Rc0->_r._f04) = (_v04);				\
									(gpRegVd3Rc0->_r._f05) = (_v05);				\
								} while(0)

#define VD3_RC_Wr06(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06)										\
								do { 											\
									(gpRegVd3Rc0->_r._f01) = (_v01);				\
									(gpRegVd3Rc0->_r._f02) = (_v02);				\
									(gpRegVd3Rc0->_r._f03) = (_v03);				\
									(gpRegVd3Rc0->_r._f04) = (_v04);				\
									(gpRegVd3Rc0->_r._f05) = (_v05);				\
									(gpRegVd3Rc0->_r._f06) = (_v06);				\
								} while(0)

#define VD3_RC_Wr07(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07)								\
								do { 											\
									(gpRegVd3Rc0->_r._f01) = (_v01);				\
									(gpRegVd3Rc0->_r._f02) = (_v02);				\
									(gpRegVd3Rc0->_r._f03) = (_v03);				\
									(gpRegVd3Rc0->_r._f04) = (_v04);				\
									(gpRegVd3Rc0->_r._f05) = (_v05);				\
									(gpRegVd3Rc0->_r._f06) = (_v06);				\
									(gpRegVd3Rc0->_r._f07) = (_v07);				\
								} while(0)

#define VD3_RC_Wr08(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08)					\
								do { 											\
									(gpRegVd3Rc0->_r._f01) = (_v01);				\
									(gpRegVd3Rc0->_r._f02) = (_v02);				\
									(gpRegVd3Rc0->_r._f03) = (_v03);				\
									(gpRegVd3Rc0->_r._f04) = (_v04);				\
									(gpRegVd3Rc0->_r._f05) = (_v05);				\
									(gpRegVd3Rc0->_r._f06) = (_v06);				\
									(gpRegVd3Rc0->_r._f07) = (_v07);				\
									(gpRegVd3Rc0->_r._f08) = (_v08);				\
								} while(0)

#define VD3_RC_Wr09(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09)													\
								do { 											\
									(gpRegVd3Rc0->_r._f01) = (_v01);				\
									(gpRegVd3Rc0->_r._f02) = (_v02);				\
									(gpRegVd3Rc0->_r._f03) = (_v03);				\
									(gpRegVd3Rc0->_r._f04) = (_v04);				\
									(gpRegVd3Rc0->_r._f05) = (_v05);				\
									(gpRegVd3Rc0->_r._f06) = (_v06);				\
									(gpRegVd3Rc0->_r._f07) = (_v07);				\
									(gpRegVd3Rc0->_r._f08) = (_v08);				\
									(gpRegVd3Rc0->_r._f09) = (_v09);				\
								} while(0)

#define VD3_RC_Wr10(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10)										\
								do { 											\
									(gpRegVd3Rc0->_r._f01) = (_v01);				\
									(gpRegVd3Rc0->_r._f02) = (_v02);				\
									(gpRegVd3Rc0->_r._f03) = (_v03);				\
									(gpRegVd3Rc0->_r._f04) = (_v04);				\
									(gpRegVd3Rc0->_r._f05) = (_v05);				\
									(gpRegVd3Rc0->_r._f06) = (_v06);				\
									(gpRegVd3Rc0->_r._f07) = (_v07);				\
									(gpRegVd3Rc0->_r._f08) = (_v08);				\
									(gpRegVd3Rc0->_r._f09) = (_v09);				\
									(gpRegVd3Rc0->_r._f10) = (_v10);				\
								} while(0)

#define VD3_RC_Wr11(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11)								\
								do { 											\
									(gpRegVd3Rc0->_r._f01) = (_v01);				\
									(gpRegVd3Rc0->_r._f02) = (_v02);				\
									(gpRegVd3Rc0->_r._f03) = (_v03);				\
									(gpRegVd3Rc0->_r._f04) = (_v04);				\
									(gpRegVd3Rc0->_r._f05) = (_v05);				\
									(gpRegVd3Rc0->_r._f06) = (_v06);				\
									(gpRegVd3Rc0->_r._f07) = (_v07);				\
									(gpRegVd3Rc0->_r._f08) = (_v08);				\
									(gpRegVd3Rc0->_r._f09) = (_v09);				\
									(gpRegVd3Rc0->_r._f10) = (_v10);				\
									(gpRegVd3Rc0->_r._f11) = (_v11);				\
								} while(0)

#define VD3_RC_Wr12(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12)					\
								do { 											\
									(gpRegVd3Rc0->_r._f01) = (_v01);				\
									(gpRegVd3Rc0->_r._f02) = (_v02);				\
									(gpRegVd3Rc0->_r._f03) = (_v03);				\
									(gpRegVd3Rc0->_r._f04) = (_v04);				\
									(gpRegVd3Rc0->_r._f05) = (_v05);				\
									(gpRegVd3Rc0->_r._f06) = (_v06);				\
									(gpRegVd3Rc0->_r._f07) = (_v07);				\
									(gpRegVd3Rc0->_r._f08) = (_v08);				\
									(gpRegVd3Rc0->_r._f09) = (_v09);				\
									(gpRegVd3Rc0->_r._f10) = (_v10);				\
									(gpRegVd3Rc0->_r._f11) = (_v11);				\
									(gpRegVd3Rc0->_r._f12) = (_v12);				\
								} while(0)

#define VD3_RC_Wr13(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13)													\
								do { 											\
									(gpRegVd3Rc0->_r._f01) = (_v01);				\
									(gpRegVd3Rc0->_r._f02) = (_v02);				\
									(gpRegVd3Rc0->_r._f03) = (_v03);				\
									(gpRegVd3Rc0->_r._f04) = (_v04);				\
									(gpRegVd3Rc0->_r._f05) = (_v05);				\
									(gpRegVd3Rc0->_r._f06) = (_v06);				\
									(gpRegVd3Rc0->_r._f07) = (_v07);				\
									(gpRegVd3Rc0->_r._f08) = (_v08);				\
									(gpRegVd3Rc0->_r._f09) = (_v09);				\
									(gpRegVd3Rc0->_r._f10) = (_v10);				\
									(gpRegVd3Rc0->_r._f11) = (_v11);				\
									(gpRegVd3Rc0->_r._f12) = (_v12);				\
									(gpRegVd3Rc0->_r._f13) = (_v13);				\
								} while(0)

#define VD3_RC_Wr14(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14)										\
								do { 											\
									(gpRegVd3Rc0->_r._f01) = (_v01);				\
									(gpRegVd3Rc0->_r._f02) = (_v02);				\
									(gpRegVd3Rc0->_r._f03) = (_v03);				\
									(gpRegVd3Rc0->_r._f04) = (_v04);				\
									(gpRegVd3Rc0->_r._f05) = (_v05);				\
									(gpRegVd3Rc0->_r._f06) = (_v06);				\
									(gpRegVd3Rc0->_r._f07) = (_v07);				\
									(gpRegVd3Rc0->_r._f08) = (_v08);				\
									(gpRegVd3Rc0->_r._f09) = (_v09);				\
									(gpRegVd3Rc0->_r._f10) = (_v10);				\
									(gpRegVd3Rc0->_r._f11) = (_v11);				\
									(gpRegVd3Rc0->_r._f12) = (_v12);				\
									(gpRegVd3Rc0->_r._f13) = (_v13);				\
									(gpRegVd3Rc0->_r._f14) = (_v14);				\
								} while(0)

#define VD3_RC_Wr15(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15)								\
								do { 											\
									(gpRegVd3Rc0->_r._f01) = (_v01);				\
									(gpRegVd3Rc0->_r._f02) = (_v02);				\
									(gpRegVd3Rc0->_r._f03) = (_v03);				\
									(gpRegVd3Rc0->_r._f04) = (_v04);				\
									(gpRegVd3Rc0->_r._f05) = (_v05);				\
									(gpRegVd3Rc0->_r._f06) = (_v06);				\
									(gpRegVd3Rc0->_r._f07) = (_v07);				\
									(gpRegVd3Rc0->_r._f08) = (_v08);				\
									(gpRegVd3Rc0->_r._f09) = (_v09);				\
									(gpRegVd3Rc0->_r._f10) = (_v10);				\
									(gpRegVd3Rc0->_r._f11) = (_v11);				\
									(gpRegVd3Rc0->_r._f12) = (_v12);				\
									(gpRegVd3Rc0->_r._f13) = (_v13);				\
									(gpRegVd3Rc0->_r._f14) = (_v14);				\
									(gpRegVd3Rc0->_r._f15) = (_v15);				\
								} while(0)

#define VD3_RC_Wr16(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15,_f16,_v16)					\
								do { 											\
									(gpRegVd3Rc0->_r._f01) = (_v01);				\
									(gpRegVd3Rc0->_r._f02) = (_v02);				\
									(gpRegVd3Rc0->_r._f03) = (_v03);				\
									(gpRegVd3Rc0->_r._f04) = (_v04);				\
									(gpRegVd3Rc0->_r._f05) = (_v05);				\
									(gpRegVd3Rc0->_r._f06) = (_v06);				\
									(gpRegVd3Rc0->_r._f07) = (_v07);				\
									(gpRegVd3Rc0->_r._f08) = (_v08);				\
									(gpRegVd3Rc0->_r._f09) = (_v09);				\
									(gpRegVd3Rc0->_r._f10) = (_v10);				\
									(gpRegVd3Rc0->_r._f11) = (_v11);				\
									(gpRegVd3Rc0->_r._f12) = (_v12);				\
									(gpRegVd3Rc0->_r._f13) = (_v13);				\
									(gpRegVd3Rc0->_r._f14) = (_v14);				\
									(gpRegVd3Rc0->_r._f15) = (_v15);				\
									(gpRegVd3Rc0->_r._f16) = (_v16);				\
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
 * _rwname: name of rw    bit field : shall be 0 after VD3_RC_Wind(), 1 for VD3_RC_Rind()
 * _iname : name of index bit field
 * _ival  : index value
 * _fname : field name
 * _fval  : field variable that field value shall be stored.
 *
 * VD3_RC_Rind : General indexed register Read.(
 * VD3_RC_Wind : General indexed register Read.
 *
 * VD3_RC_Ridx : For 'index', 'rw', 'load' field name
 * VD3_RC_Widx : For 'index', 'rw', 'load' field name and NO_LOAD.
 */
#define VD3_RC_Rind(_r, _lname, _rwname, _iname, _ival, _fname, _fval)				\
							do {												\
								VD3_RC_Wr03(_r,_lname,0,_rwname,1,_iname,_ival);	\
								VD3_RC_WrFL(_r);									\
								VD3_RC_RdFL(_r);									\
								VD3_RC_Rd01(_r,_fname,_fval);						\
							} while (0)

#define VD3_RC_Wind(_r, _lname, _rwname, _iname, _ival, _fname, _fval)				\
				VD3_RC_Wr04(_r, _lname,0, _rwname,0, _iname,_ival, _fname,_fval)


#define VD3_RC_Ridx(_r, _ival, _fname, _fval)	VD3_RC_Rind(_r,load,rw,index,_ival,_fname,_fval)

#define VD3_RC_Widx(_r, _ival, _fname, _fval)	VD3_RC_Wind(_r,load,rw,index,_ival,_fname,_fval)

/** @} *//* end of macro documentation */


#ifdef __cplusplus
}
#endif

#endif	/* _VD3_RC_REG_H_ */
