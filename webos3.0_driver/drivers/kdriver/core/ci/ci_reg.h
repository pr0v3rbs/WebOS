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
 *  CI register details. ( used only within kdriver )
 *
 *  @author		Srinivasan Shanmugam (srinivasan.shanmugam@lge.com) 
 *  @author		Hwajeong Lee (hwajeong.lee@lge.com)
 *  @author		Jinhwan Bae (jinhwan.bae@lge.com) - modifier
 *  @version	1.0
 *  @date		2010.2.22
 *
 *  @addtogroup lg1150_ci
 *	@{
 */

#ifndef __CI_REG_H__
#define __CI_REG_H__

#include "base_types.h"

/*-----------------------------------------------------------------------------
	0x0000 ctrl_stat1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT16
	                                : 2,	//  0: 1 reserved
	vccen_n                         : 1,	//     2
	                                : 2,	//  3: 4 reserved
	en_pccard                       : 1,	//     5
	en_hw_hot_swap                  : 1,	//     6
	card_reset                      : 1;	//     7
} CTRL_STAT1;

/*-----------------------------------------------------------------------------
	0x0002 ctrl_stat2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT16
	cd1                             : 1,	//     0
	cd2                             : 1,	//     1
	vs1                             : 1,	//     2
	vs2                             : 1,	//     3
	ready_ireq                      : 1,	//     4
	bvd1                            : 1,	//     5
	bvd2                            : 1,	//     6
	inpack                          : 1,	//     7
	iois16                          : 1;	//     8
} CTRL_STAT2;

/*-----------------------------------------------------------------------------
	0x0004 ctrl_stat3 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT16
	                                : 2,	//  0: 1 reserved
	io_int_mode                     : 1,	//     2
	                                : 1,	//     3 reserved
	pod_mode                        : 1,	//     4
	card_is_mem                     : 1;	//     5
} CTRL_STAT3;

/*-----------------------------------------------------------------------------
	0x0006 pc_card_ctrl1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT16
	chip_mode                       : 5,	//  0: 4
	c_flash_mode                    : 1,	//     5
	ide_mode                        : 1;	//     6
} PC_CARD_CTRL1;

/*-----------------------------------------------------------------------------
	0x0008 pc_card_ctrl2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT16
	setup_timing                    : 8,	//  0: 7
	command_timing                  : 8;	//  8:15
} PC_CARD_CTRL2;

/*-----------------------------------------------------------------------------
	0x000a pc_card_ctrl3 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT16
	recovery_timing                 : 8;	//  0: 7
} PC_CARD_CTRL3;

/*-----------------------------------------------------------------------------
	0x0026 intr_flag ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT16
	cd_changed                      : 1,	//     0
	                                : 1,	//     1 reserved
	hw_hot_swap_done                : 1,	//     2
	ireq                            : 1,	//     3
	bvd1                            : 1,	//     4
	bvd2                            : 1,	//     5
	stschg                          : 1;	//     6
} INTR_FLAG;

/*-----------------------------------------------------------------------------
	0x0028 intr_mask ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT16
	cd_changed                      : 1,	//     0
	                                : 1,	//     1 reserved
	hw_hot_swap_done                : 1,	//     2
	ireq                            : 1,	//     3
	bvd1                            : 1,	//     4
	bvd2                            : 1,	//     5
	stschg                          : 1,	//     6
	tsc                             : 1,	//     7
	p_tsc                           : 1;	//     8
} INTR_MASK;

/*-----------------------------------------------------------------------------
	0x002a intr_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT16
	bvd1_pol                        : 1,	//     0
	interrupt_happens_when_either_nc: 1,	//     1
	gpio_int_sel                    : 2,	//  2: 3
	en_gpio_int                     : 1,	//     4
	ireq_pol                        : 1,	//     5
	cd_int_mode                     : 1,	//     6
	cd_pol                          : 1,	//     7
	intr_tsc_en                     : 1,	//     8
	p_intr_tsc_en                   : 1;	//     9
} INTR_CTRL;

/*-----------------------------------------------------------------------------
	0x002e hw_hot_swap_ctrl1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT16
	power_on_interval0              ;   	// 15: 0
} HW_HOT_SWAP_CTRL1;

/*-----------------------------------------------------------------------------
	0x0030 hw_hot_swap_ctrl2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT16
	power_on_interval1              :11;	//  0:10
} HW_HOT_SWAP_CTRL2;

/*-----------------------------------------------------------------------------
	0x0032 hw_hot_swap_ctrl3 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT16
	enable_interval0                ;   	// 15: 0
} HW_HOT_SWAP_CTRL3;

/*-----------------------------------------------------------------------------
	0x0034 hw_hot_swap_ctrl4 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT16
	enable_interval1                :11;	//  0:10
} HW_HOT_SWAP_CTRL4;

/*-----------------------------------------------------------------------------
	0x0036 hw_hot_swap_ctrl5 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT16
	card_insert_done_interval0      ;   	// 15: 0
} HW_HOT_SWAP_CTRL5;

/*-----------------------------------------------------------------------------
	0x0038 hw_hot_swap_ctrl6 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT16
	card_insert_done_interval1      :11;	//  0:10
} HW_HOT_SWAP_CTRL6;

/*-----------------------------------------------------------------------------
	0x003a hw_hot_swap_ctrl7 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT16
	card_removal_done_interval0     ;   	// 15: 0
} HW_HOT_SWAP_CTRL7;

/*-----------------------------------------------------------------------------
	0x003c hw_hot_swap_ctrl8 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT16
	card_removal_done_interval1     :11;	//  0:10
} HW_HOT_SWAP_CTRL8;

typedef struct {
	UINT16
	burst_mode     :2;	//  0:1
} BURST_CTRL;

typedef struct {
	CTRL_STAT1                      	ctrl_stat1                      ;	// 0x0000 : ''
	CTRL_STAT2                      	ctrl_stat2                      ;	// 0x0002 : ''
	CTRL_STAT3                      	ctrl_stat3                      ;	// 0x0004 : ''
	PC_CARD_CTRL1                   	pc_card_ctrl1                   ;	// 0x0006 : ''
	PC_CARD_CTRL2                   	pc_card_ctrl2                   ;	// 0x0008 : ''
	PC_CARD_CTRL3                   	pc_card_ctrl3                   ;	// 0x000a : ''
	UINT16                          	                 __rsvd_02[   2];	// 0x000c ~ 0x00e
	BURST_CTRL				burst_ctrl           	        ;	// 0x0010 : ''
	UINT16                          	                 __rsvd_00[  10];	// 0x0012 ~ 0x0024
	INTR_FLAG                       	intr_flag                       ;	// 0x0026 : ''
	INTR_MASK                       	intr_mask                       ;	// 0x0028 : ''
	INTR_CTRL                       	intr_ctrl                       ;	// 0x002a : ''
	UINT16                          	                 __rsvd_01[   1];	// 0x002c
	HW_HOT_SWAP_CTRL1               	hw_hot_swap_ctrl1               ;	// 0x002e : ''
	HW_HOT_SWAP_CTRL2               	hw_hot_swap_ctrl2               ;	// 0x0030 : ''
	HW_HOT_SWAP_CTRL3               	hw_hot_swap_ctrl3               ;	// 0x0032 : ''
	HW_HOT_SWAP_CTRL4               	hw_hot_swap_ctrl4               ;	// 0x0034 : ''
	HW_HOT_SWAP_CTRL5               	hw_hot_swap_ctrl5               ;	// 0x0036 : ''
	HW_HOT_SWAP_CTRL6               	hw_hot_swap_ctrl6               ;	// 0x0038 : ''
	HW_HOT_SWAP_CTRL7               	hw_hot_swap_ctrl7               ;	// 0x003a : ''
	HW_HOT_SWAP_CTRL8               	hw_hot_swap_ctrl8               ;	// 0x003c : ''
// TODO : got no Category name T.T
} _REG_T;
/* 17 regs, 17 types */

/* 17 regs, 17 types in Total*/

#if 0
/*
 * @{
 * Naming for register pointer.
 * gpRealReg#UNKNOWN# : real register of #UNKNOWN#.
 * gpReg#UNKNOWN#     : shadow register.
 *
 * @def #UNKNOWN#_RdFL: Read  FLushing : Shadow <- Real.
 * @def #UNKNOWN#_WrFL: Write FLushing : Shadow -> Real.
 * @def #UNKNOWN#_Rd  : Read  whole register(UINT16) from Shadow register.
 * @def #UNKNOWN#_Wr  : Write whole register(UINT16) from Shadow register.
 * @def #UNKNOWN#_Rd01 ~ #UNKNOWN#_Rdnn: Read  given '01~nn' fields from Shadow register.
 * @def #UNKNOWN#_Wr01 ~ #UNKNOWN#_Wrnn: Write given '01~nn' fields to   Shadow register.
 * */
#define #UNKNOWN#_RdFL(_r)			((gpReg#UNKNOWN#->_r)=(gpRealReg#UNKNOWN#->_r))
#define #UNKNOWN#_WrFL(_r)			((gpRealReg#UNKNOWN#->_r)=(gpReg#UNKNOWN#->_r))

#define #UNKNOWN#_Rd(_r)			*((UINT16*)(&(gpReg#UNKNOWN#->_r)))
#define #UNKNOWN#_Wr(_r,_v)			((#UNKNOWN#_Rd(_r))=((UINT16)(_v)))

#define #UNKNOWN#_Rd01(_r,_f01,_v01)													\
								do { 											\
									(_v01) = (gpReg#UNKNOWN#->_r._f01);				\
								} while(0)

#define #UNKNOWN#_Rd02(_r,_f01,_v01,_f02,_v02)										\
								do { 											\
									(_v01) = (gpReg#UNKNOWN#->_r._f01);				\
									(_v02) = (gpReg#UNKNOWN#->_r._f02);				\
								} while(0)

#define #UNKNOWN#_Rd03(_r,_f01,_v01,_f02,_v02,_f03,_v03)								\
								do { 											\
									(_v01) = (gpReg#UNKNOWN#->_r._f01);				\
									(_v02) = (gpReg#UNKNOWN#->_r._f02);				\
									(_v03) = (gpReg#UNKNOWN#->_r._f03);				\
								} while(0)

#define #UNKNOWN#_Rd04(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04)					\
								do { 											\
									(_v01) = (gpReg#UNKNOWN#->_r._f01);				\
									(_v02) = (gpReg#UNKNOWN#->_r._f02);				\
									(_v03) = (gpReg#UNKNOWN#->_r._f03);				\
									(_v04) = (gpReg#UNKNOWN#->_r._f04);				\
								} while(0)

#define #UNKNOWN#_Rd05(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05)													\
								do { 											\
									(_v01) = (gpReg#UNKNOWN#->_r._f01);				\
									(_v02) = (gpReg#UNKNOWN#->_r._f02);				\
									(_v03) = (gpReg#UNKNOWN#->_r._f03);				\
									(_v04) = (gpReg#UNKNOWN#->_r._f04);				\
									(_v05) = (gpReg#UNKNOWN#->_r._f05);				\
								} while(0)

#define #UNKNOWN#_Rd06(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06)										\
								do { 											\
									(_v01) = (gpReg#UNKNOWN#->_r._f01);				\
									(_v02) = (gpReg#UNKNOWN#->_r._f02);				\
									(_v03) = (gpReg#UNKNOWN#->_r._f03);				\
									(_v04) = (gpReg#UNKNOWN#->_r._f04);				\
									(_v05) = (gpReg#UNKNOWN#->_r._f05);				\
									(_v06) = (gpReg#UNKNOWN#->_r._f06);				\
								} while(0)

#define #UNKNOWN#_Rd07(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07)								\
								do { 											\
									(_v01) = (gpReg#UNKNOWN#->_r._f01);				\
									(_v02) = (gpReg#UNKNOWN#->_r._f02);				\
									(_v03) = (gpReg#UNKNOWN#->_r._f03);				\
									(_v04) = (gpReg#UNKNOWN#->_r._f04);				\
									(_v05) = (gpReg#UNKNOWN#->_r._f05);				\
									(_v06) = (gpReg#UNKNOWN#->_r._f06);				\
									(_v07) = (gpReg#UNKNOWN#->_r._f07);				\
								} while(0)

#define #UNKNOWN#_Rd08(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08)					\
								do { 											\
									(_v01) = (gpReg#UNKNOWN#->_r._f01);				\
									(_v02) = (gpReg#UNKNOWN#->_r._f02);				\
									(_v03) = (gpReg#UNKNOWN#->_r._f03);				\
									(_v04) = (gpReg#UNKNOWN#->_r._f04);				\
									(_v05) = (gpReg#UNKNOWN#->_r._f05);				\
									(_v06) = (gpReg#UNKNOWN#->_r._f06);				\
									(_v07) = (gpReg#UNKNOWN#->_r._f07);				\
									(_v08) = (gpReg#UNKNOWN#->_r._f08);				\
								} while(0)

#define #UNKNOWN#_Rd09(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09)													\
								do { 											\
									(_v01) = (gpReg#UNKNOWN#->_r._f01);				\
									(_v02) = (gpReg#UNKNOWN#->_r._f02);				\
									(_v03) = (gpReg#UNKNOWN#->_r._f03);				\
									(_v04) = (gpReg#UNKNOWN#->_r._f04);				\
									(_v05) = (gpReg#UNKNOWN#->_r._f05);				\
									(_v06) = (gpReg#UNKNOWN#->_r._f06);				\
									(_v07) = (gpReg#UNKNOWN#->_r._f07);				\
									(_v08) = (gpReg#UNKNOWN#->_r._f08);				\
									(_v09) = (gpReg#UNKNOWN#->_r._f09);				\
								} while(0)

#define #UNKNOWN#_Rd10(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10)										\
								do { 											\
									(_v01) = (gpReg#UNKNOWN#->_r._f01);				\
									(_v02) = (gpReg#UNKNOWN#->_r._f02);				\
									(_v03) = (gpReg#UNKNOWN#->_r._f03);				\
									(_v04) = (gpReg#UNKNOWN#->_r._f04);				\
									(_v05) = (gpReg#UNKNOWN#->_r._f05);				\
									(_v06) = (gpReg#UNKNOWN#->_r._f06);				\
									(_v07) = (gpReg#UNKNOWN#->_r._f07);				\
									(_v08) = (gpReg#UNKNOWN#->_r._f08);				\
									(_v09) = (gpReg#UNKNOWN#->_r._f09);				\
									(_v10) = (gpReg#UNKNOWN#->_r._f10);				\
								} while(0)

#define #UNKNOWN#_Rd11(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11)								\
								do { 											\
									(_v01) = (gpReg#UNKNOWN#->_r._f01);				\
									(_v02) = (gpReg#UNKNOWN#->_r._f02);				\
									(_v03) = (gpReg#UNKNOWN#->_r._f03);				\
									(_v04) = (gpReg#UNKNOWN#->_r._f04);				\
									(_v05) = (gpReg#UNKNOWN#->_r._f05);				\
									(_v06) = (gpReg#UNKNOWN#->_r._f06);				\
									(_v07) = (gpReg#UNKNOWN#->_r._f07);				\
									(_v08) = (gpReg#UNKNOWN#->_r._f08);				\
									(_v09) = (gpReg#UNKNOWN#->_r._f09);				\
									(_v10) = (gpReg#UNKNOWN#->_r._f10);				\
									(_v11) = (gpReg#UNKNOWN#->_r._f11);				\
								} while(0)

#define #UNKNOWN#_Rd12(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12)					\
								do { 											\
									(_v01) = (gpReg#UNKNOWN#->_r._f01);				\
									(_v02) = (gpReg#UNKNOWN#->_r._f02);				\
									(_v03) = (gpReg#UNKNOWN#->_r._f03);				\
									(_v04) = (gpReg#UNKNOWN#->_r._f04);				\
									(_v05) = (gpReg#UNKNOWN#->_r._f05);				\
									(_v06) = (gpReg#UNKNOWN#->_r._f06);				\
									(_v07) = (gpReg#UNKNOWN#->_r._f07);				\
									(_v08) = (gpReg#UNKNOWN#->_r._f08);				\
									(_v09) = (gpReg#UNKNOWN#->_r._f09);				\
									(_v10) = (gpReg#UNKNOWN#->_r._f10);				\
									(_v11) = (gpReg#UNKNOWN#->_r._f11);				\
									(_v12) = (gpReg#UNKNOWN#->_r._f12);				\
								} while(0)

#define #UNKNOWN#_Rd13(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13)													\
								do { 											\
									(_v01) = (gpReg#UNKNOWN#->_r._f01);				\
									(_v02) = (gpReg#UNKNOWN#->_r._f02);				\
									(_v03) = (gpReg#UNKNOWN#->_r._f03);				\
									(_v04) = (gpReg#UNKNOWN#->_r._f04);				\
									(_v05) = (gpReg#UNKNOWN#->_r._f05);				\
									(_v06) = (gpReg#UNKNOWN#->_r._f06);				\
									(_v07) = (gpReg#UNKNOWN#->_r._f07);				\
									(_v08) = (gpReg#UNKNOWN#->_r._f08);				\
									(_v09) = (gpReg#UNKNOWN#->_r._f09);				\
									(_v10) = (gpReg#UNKNOWN#->_r._f10);				\
									(_v11) = (gpReg#UNKNOWN#->_r._f11);				\
									(_v12) = (gpReg#UNKNOWN#->_r._f12);				\
									(_v13) = (gpReg#UNKNOWN#->_r._f13);				\
								} while(0)

#define #UNKNOWN#_Rd14(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14)										\
								do { 											\
									(_v01) = (gpReg#UNKNOWN#->_r._f01);				\
									(_v02) = (gpReg#UNKNOWN#->_r._f02);				\
									(_v03) = (gpReg#UNKNOWN#->_r._f03);				\
									(_v04) = (gpReg#UNKNOWN#->_r._f04);				\
									(_v05) = (gpReg#UNKNOWN#->_r._f05);				\
									(_v06) = (gpReg#UNKNOWN#->_r._f06);				\
									(_v07) = (gpReg#UNKNOWN#->_r._f07);				\
									(_v08) = (gpReg#UNKNOWN#->_r._f08);				\
									(_v09) = (gpReg#UNKNOWN#->_r._f09);				\
									(_v10) = (gpReg#UNKNOWN#->_r._f10);				\
									(_v11) = (gpReg#UNKNOWN#->_r._f11);				\
									(_v12) = (gpReg#UNKNOWN#->_r._f12);				\
									(_v13) = (gpReg#UNKNOWN#->_r._f13);				\
									(_v14) = (gpReg#UNKNOWN#->_r._f14);				\
								} while(0)

#define #UNKNOWN#_Rd15(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15)								\
								do { 											\
									(_v01) = (gpReg#UNKNOWN#->_r._f01);				\
									(_v02) = (gpReg#UNKNOWN#->_r._f02);				\
									(_v03) = (gpReg#UNKNOWN#->_r._f03);				\
									(_v04) = (gpReg#UNKNOWN#->_r._f04);				\
									(_v05) = (gpReg#UNKNOWN#->_r._f05);				\
									(_v06) = (gpReg#UNKNOWN#->_r._f06);				\
									(_v07) = (gpReg#UNKNOWN#->_r._f07);				\
									(_v08) = (gpReg#UNKNOWN#->_r._f08);				\
									(_v09) = (gpReg#UNKNOWN#->_r._f09);				\
									(_v10) = (gpReg#UNKNOWN#->_r._f10);				\
									(_v11) = (gpReg#UNKNOWN#->_r._f11);				\
									(_v12) = (gpReg#UNKNOWN#->_r._f12);				\
									(_v13) = (gpReg#UNKNOWN#->_r._f13);				\
									(_v14) = (gpReg#UNKNOWN#->_r._f14);				\
									(_v15) = (gpReg#UNKNOWN#->_r._f15);				\
								} while(0)

#define #UNKNOWN#_Rd16(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15,_f16,_v16)					\
								do { 											\
									(_v01) = (gpReg#UNKNOWN#->_r._f01);				\
									(_v02) = (gpReg#UNKNOWN#->_r._f02);				\
									(_v03) = (gpReg#UNKNOWN#->_r._f03);				\
									(_v04) = (gpReg#UNKNOWN#->_r._f04);				\
									(_v05) = (gpReg#UNKNOWN#->_r._f05);				\
									(_v06) = (gpReg#UNKNOWN#->_r._f06);				\
									(_v07) = (gpReg#UNKNOWN#->_r._f07);				\
									(_v08) = (gpReg#UNKNOWN#->_r._f08);				\
									(_v09) = (gpReg#UNKNOWN#->_r._f09);				\
									(_v10) = (gpReg#UNKNOWN#->_r._f10);				\
									(_v11) = (gpReg#UNKNOWN#->_r._f11);				\
									(_v12) = (gpReg#UNKNOWN#->_r._f12);				\
									(_v13) = (gpReg#UNKNOWN#->_r._f13);				\
									(_v14) = (gpReg#UNKNOWN#->_r._f14);				\
									(_v15) = (gpReg#UNKNOWN#->_r._f15);				\
									(_v16) = (gpReg#UNKNOWN#->_r._f16);				\
								} while(0)


#define #UNKNOWN#_Wr01(_r,_f01,_v01)													\
								do { 											\
									(gpReg#UNKNOWN#->_r._f01) = (_v01);				\
								} while(0)

#define #UNKNOWN#_Wr02(_r,_f01,_v01,_f02,_v02)										\
								do { 											\
									(gpReg#UNKNOWN#->_r._f01) = (_v01);				\
									(gpReg#UNKNOWN#->_r._f02) = (_v02);				\
								} while(0)

#define #UNKNOWN#_Wr03(_r,_f01,_v01,_f02,_v02,_f03,_v03)								\
								do { 											\
									(gpReg#UNKNOWN#->_r._f01) = (_v01);				\
									(gpReg#UNKNOWN#->_r._f02) = (_v02);				\
									(gpReg#UNKNOWN#->_r._f03) = (_v03);				\
								} while(0)

#define #UNKNOWN#_Wr04(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04)					\
								do { 											\
									(gpReg#UNKNOWN#->_r._f01) = (_v01);				\
									(gpReg#UNKNOWN#->_r._f02) = (_v02);				\
									(gpReg#UNKNOWN#->_r._f03) = (_v03);				\
									(gpReg#UNKNOWN#->_r._f04) = (_v04);				\
								} while(0)

#define #UNKNOWN#_Wr05(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05)													\
								do { 											\
									(gpReg#UNKNOWN#->_r._f01) = (_v01);				\
									(gpReg#UNKNOWN#->_r._f02) = (_v02);				\
									(gpReg#UNKNOWN#->_r._f03) = (_v03);				\
									(gpReg#UNKNOWN#->_r._f04) = (_v04);				\
									(gpReg#UNKNOWN#->_r._f05) = (_v05);				\
								} while(0)

#define #UNKNOWN#_Wr06(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06)										\
								do { 											\
									(gpReg#UNKNOWN#->_r._f01) = (_v01);				\
									(gpReg#UNKNOWN#->_r._f02) = (_v02);				\
									(gpReg#UNKNOWN#->_r._f03) = (_v03);				\
									(gpReg#UNKNOWN#->_r._f04) = (_v04);				\
									(gpReg#UNKNOWN#->_r._f05) = (_v05);				\
									(gpReg#UNKNOWN#->_r._f06) = (_v06);				\
								} while(0)

#define #UNKNOWN#_Wr07(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07)								\
								do { 											\
									(gpReg#UNKNOWN#->_r._f01) = (_v01);				\
									(gpReg#UNKNOWN#->_r._f02) = (_v02);				\
									(gpReg#UNKNOWN#->_r._f03) = (_v03);				\
									(gpReg#UNKNOWN#->_r._f04) = (_v04);				\
									(gpReg#UNKNOWN#->_r._f05) = (_v05);				\
									(gpReg#UNKNOWN#->_r._f06) = (_v06);				\
									(gpReg#UNKNOWN#->_r._f07) = (_v07);				\
								} while(0)

#define #UNKNOWN#_Wr08(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08)					\
								do { 											\
									(gpReg#UNKNOWN#->_r._f01) = (_v01);				\
									(gpReg#UNKNOWN#->_r._f02) = (_v02);				\
									(gpReg#UNKNOWN#->_r._f03) = (_v03);				\
									(gpReg#UNKNOWN#->_r._f04) = (_v04);				\
									(gpReg#UNKNOWN#->_r._f05) = (_v05);				\
									(gpReg#UNKNOWN#->_r._f06) = (_v06);				\
									(gpReg#UNKNOWN#->_r._f07) = (_v07);				\
									(gpReg#UNKNOWN#->_r._f08) = (_v08);				\
								} while(0)

#define #UNKNOWN#_Wr09(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09)													\
								do { 											\
									(gpReg#UNKNOWN#->_r._f01) = (_v01);				\
									(gpReg#UNKNOWN#->_r._f02) = (_v02);				\
									(gpReg#UNKNOWN#->_r._f03) = (_v03);				\
									(gpReg#UNKNOWN#->_r._f04) = (_v04);				\
									(gpReg#UNKNOWN#->_r._f05) = (_v05);				\
									(gpReg#UNKNOWN#->_r._f06) = (_v06);				\
									(gpReg#UNKNOWN#->_r._f07) = (_v07);				\
									(gpReg#UNKNOWN#->_r._f08) = (_v08);				\
									(gpReg#UNKNOWN#->_r._f09) = (_v09);				\
								} while(0)

#define #UNKNOWN#_Wr10(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10)										\
								do { 											\
									(gpReg#UNKNOWN#->_r._f01) = (_v01);				\
									(gpReg#UNKNOWN#->_r._f02) = (_v02);				\
									(gpReg#UNKNOWN#->_r._f03) = (_v03);				\
									(gpReg#UNKNOWN#->_r._f04) = (_v04);				\
									(gpReg#UNKNOWN#->_r._f05) = (_v05);				\
									(gpReg#UNKNOWN#->_r._f06) = (_v06);				\
									(gpReg#UNKNOWN#->_r._f07) = (_v07);				\
									(gpReg#UNKNOWN#->_r._f08) = (_v08);				\
									(gpReg#UNKNOWN#->_r._f09) = (_v09);				\
									(gpReg#UNKNOWN#->_r._f10) = (_v10);				\
								} while(0)

#define #UNKNOWN#_Wr11(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11)								\
								do { 											\
									(gpReg#UNKNOWN#->_r._f01) = (_v01);				\
									(gpReg#UNKNOWN#->_r._f02) = (_v02);				\
									(gpReg#UNKNOWN#->_r._f03) = (_v03);				\
									(gpReg#UNKNOWN#->_r._f04) = (_v04);				\
									(gpReg#UNKNOWN#->_r._f05) = (_v05);				\
									(gpReg#UNKNOWN#->_r._f06) = (_v06);				\
									(gpReg#UNKNOWN#->_r._f07) = (_v07);				\
									(gpReg#UNKNOWN#->_r._f08) = (_v08);				\
									(gpReg#UNKNOWN#->_r._f09) = (_v09);				\
									(gpReg#UNKNOWN#->_r._f10) = (_v10);				\
									(gpReg#UNKNOWN#->_r._f11) = (_v11);				\
								} while(0)

#define #UNKNOWN#_Wr12(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12)					\
								do { 											\
									(gpReg#UNKNOWN#->_r._f01) = (_v01);				\
									(gpReg#UNKNOWN#->_r._f02) = (_v02);				\
									(gpReg#UNKNOWN#->_r._f03) = (_v03);				\
									(gpReg#UNKNOWN#->_r._f04) = (_v04);				\
									(gpReg#UNKNOWN#->_r._f05) = (_v05);				\
									(gpReg#UNKNOWN#->_r._f06) = (_v06);				\
									(gpReg#UNKNOWN#->_r._f07) = (_v07);				\
									(gpReg#UNKNOWN#->_r._f08) = (_v08);				\
									(gpReg#UNKNOWN#->_r._f09) = (_v09);				\
									(gpReg#UNKNOWN#->_r._f10) = (_v10);				\
									(gpReg#UNKNOWN#->_r._f11) = (_v11);				\
									(gpReg#UNKNOWN#->_r._f12) = (_v12);				\
								} while(0)

#define #UNKNOWN#_Wr13(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13)													\
								do { 											\
									(gpReg#UNKNOWN#->_r._f01) = (_v01);				\
									(gpReg#UNKNOWN#->_r._f02) = (_v02);				\
									(gpReg#UNKNOWN#->_r._f03) = (_v03);				\
									(gpReg#UNKNOWN#->_r._f04) = (_v04);				\
									(gpReg#UNKNOWN#->_r._f05) = (_v05);				\
									(gpReg#UNKNOWN#->_r._f06) = (_v06);				\
									(gpReg#UNKNOWN#->_r._f07) = (_v07);				\
									(gpReg#UNKNOWN#->_r._f08) = (_v08);				\
									(gpReg#UNKNOWN#->_r._f09) = (_v09);				\
									(gpReg#UNKNOWN#->_r._f10) = (_v10);				\
									(gpReg#UNKNOWN#->_r._f11) = (_v11);				\
									(gpReg#UNKNOWN#->_r._f12) = (_v12);				\
									(gpReg#UNKNOWN#->_r._f13) = (_v13);				\
								} while(0)

#define #UNKNOWN#_Wr14(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14)										\
								do { 											\
									(gpReg#UNKNOWN#->_r._f01) = (_v01);				\
									(gpReg#UNKNOWN#->_r._f02) = (_v02);				\
									(gpReg#UNKNOWN#->_r._f03) = (_v03);				\
									(gpReg#UNKNOWN#->_r._f04) = (_v04);				\
									(gpReg#UNKNOWN#->_r._f05) = (_v05);				\
									(gpReg#UNKNOWN#->_r._f06) = (_v06);				\
									(gpReg#UNKNOWN#->_r._f07) = (_v07);				\
									(gpReg#UNKNOWN#->_r._f08) = (_v08);				\
									(gpReg#UNKNOWN#->_r._f09) = (_v09);				\
									(gpReg#UNKNOWN#->_r._f10) = (_v10);				\
									(gpReg#UNKNOWN#->_r._f11) = (_v11);				\
									(gpReg#UNKNOWN#->_r._f12) = (_v12);				\
									(gpReg#UNKNOWN#->_r._f13) = (_v13);				\
									(gpReg#UNKNOWN#->_r._f14) = (_v14);				\
								} while(0)

#define #UNKNOWN#_Wr15(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15)								\
								do { 											\
									(gpReg#UNKNOWN#->_r._f01) = (_v01);				\
									(gpReg#UNKNOWN#->_r._f02) = (_v02);				\
									(gpReg#UNKNOWN#->_r._f03) = (_v03);				\
									(gpReg#UNKNOWN#->_r._f04) = (_v04);				\
									(gpReg#UNKNOWN#->_r._f05) = (_v05);				\
									(gpReg#UNKNOWN#->_r._f06) = (_v06);				\
									(gpReg#UNKNOWN#->_r._f07) = (_v07);				\
									(gpReg#UNKNOWN#->_r._f08) = (_v08);				\
									(gpReg#UNKNOWN#->_r._f09) = (_v09);				\
									(gpReg#UNKNOWN#->_r._f10) = (_v10);				\
									(gpReg#UNKNOWN#->_r._f11) = (_v11);				\
									(gpReg#UNKNOWN#->_r._f12) = (_v12);				\
									(gpReg#UNKNOWN#->_r._f13) = (_v13);				\
									(gpReg#UNKNOWN#->_r._f14) = (_v14);				\
									(gpReg#UNKNOWN#->_r._f15) = (_v15);				\
								} while(0)

#define #UNKNOWN#_Wr16(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15,_f16,_v16)					\
								do { 											\
									(gpReg#UNKNOWN#->_r._f01) = (_v01);				\
									(gpReg#UNKNOWN#->_r._f02) = (_v02);				\
									(gpReg#UNKNOWN#->_r._f03) = (_v03);				\
									(gpReg#UNKNOWN#->_r._f04) = (_v04);				\
									(gpReg#UNKNOWN#->_r._f05) = (_v05);				\
									(gpReg#UNKNOWN#->_r._f06) = (_v06);				\
									(gpReg#UNKNOWN#->_r._f07) = (_v07);				\
									(gpReg#UNKNOWN#->_r._f08) = (_v08);				\
									(gpReg#UNKNOWN#->_r._f09) = (_v09);				\
									(gpReg#UNKNOWN#->_r._f10) = (_v10);				\
									(gpReg#UNKNOWN#->_r._f11) = (_v11);				\
									(gpReg#UNKNOWN#->_r._f12) = (_v12);				\
									(gpReg#UNKNOWN#->_r._f13) = (_v13);				\
									(gpReg#UNKNOWN#->_r._f14) = (_v14);				\
									(gpReg#UNKNOWN#->_r._f15) = (_v15);				\
									(gpReg#UNKNOWN#->_r._f16) = (_v16);				\
								} while(0)
#endif /* #if 0 */

/* from 'LG1150-DVB_CI-MAN v1.0.csv' 20100311 00:55:16   대한민국 표준시 by getregs v2.3 */

#endif /* __CI_REG_H__ */
