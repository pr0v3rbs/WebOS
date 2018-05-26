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

/** file
 *
 *  LQC Register Map.
 *  Lipsync Queue Control register details. ( used only within kdriver )
 *
 *  author     sooya.joo(sooya.joo@lge.com)
 *  version    1.0
 *  date       2010.04.05
 *
 */

#ifndef _LQ_REG_H_
#define _LQ_REG_H_

/*----------------------------------------------------------------------------------------
    Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include "vdec_type_defs.h"
#include "../lg1154_vdec_base.h"


#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------------
	0x0000 lqc_conf ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_lqc_channel                : 2,	//  0: 1
	                                : 2,	//  2: 3 reserved
	reg_lqc_lq_keep_count          : 8,	//  4:11
	                                : 3,	// 12:14 reserved
	reg_lqc_auto_end_address       : 1,	//    15
	reg_lqc_stcc_discontinuity     : 1,	//    16
	reg_lqc_lq_vsync_en            : 1,	//    17
	reg_lqc_de_vsync_en            : 1,	//    18
	reg_lqc_use_global_stc         : 1,	//    19
	reg_lqc_wait_sync1             : 1,	//    20
	reg_lqc_wait_sync0             : 1,	//    21
	reg_lqc_lq_discard             : 1,	//    22
	reg_lqc_lq_skip                : 1,	//    23
	reg_lqc_wait_wptr_dis          : 1,	//    24
	reg_lqc_ignore_stc_dts_matching: 1,	//    25
	reg_lqc_lq_delay_dis           : 1,	//    26
	reg_lqc_auto_update_rptr_dis   : 1,	//    27
	reg_lqc_no_wait_intr_ack       : 1,	//    28
	reg_lqc_pause                  : 1,	//    29
	reg_lqc_stop                   : 1,	//    30
	reg_lqc_enable                 : 1;	//    31
} LQC_CONF;

/*-----------------------------------------------------------------------------
	0x0004 lqc_lq_conf ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_lqc_lq_base                :12,	//  0:11
	                                : 4,	// 12:15 reserved
	reg_lqc_lq_size                : 8;	// 16:23
} LQC_LQ_CONF;

/*-----------------------------------------------------------------------------
	0x0008 lqc_lq_aempty_time ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_lqc_lq_aempty_timer        :24,	//  0:23
	                                : 7,	// 24:30 reserved
	reg_lqc_lq_aempty_timer_en     : 1;	//    31
} LQC_LQ_AEMPTY_TIME;

/*-----------------------------------------------------------------------------
	0x000c lqc_stc_lower ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_lqc_lq_stc_ignore_lower 	:29,	//  0:28
	                                : 2,	// 29:30 reserved
	reg_lqc_lq_stc_lower_limit_en	: 1;	//    31
} LQC_STC_LOWER;

/*-----------------------------------------------------------------------------
	0x0010 lqc_lq_wptr ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_lqc_lq_wptr                : 8;	//  0: 7
} LQC_LQ_WPTR;

/*-----------------------------------------------------------------------------
	0x0014 lqc_lq_rptr ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_lqc_lq_rptr                : 8;	//  0: 7
} LQC_LQ_RPTR;

/*-----------------------------------------------------------------------------
	0x0018 lqc_lq_afull_lev ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_lqc_lq_afull_lev           : 8;	//  0: 7
} LQC_LQ_AFULL_LEV;

/*-----------------------------------------------------------------------------
	0x001c lqc_lq_aempty_lev ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_lqc_lq_aempty_lev          : 8;	//  0: 7
} LQC_LQ_AEMPTY_LEV;

/*-----------------------------------------------------------------------------
	0x0020 lqc_stc_upper ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_lqc_stc_ignore_upper       :29,	//  0:28
	                                : 2,	// 29:30 reserved
	reg_lqc_stc_upper_limit_en     : 1;	//    31
} LQC_STC_UPPER;

/*-----------------------------------------------------------------------------
	0x0024 lqc_stc ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_lqc_stc                    :28;	//  0:27
} LQC_STC;

/*-----------------------------------------------------------------------------
	0x0028 lqc_stc_gstc ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_lqc_stc_gstcc_diff         :28;	//  0:27
} LQC_STC_GSTC;

/*-----------------------------------------------------------------------------
	0x002c lqc_intr_timer ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_lqc_intr_timer             :28,	//  0:27
	                                : 3,	// 28:30 reserved
	reg_lqc_intr_timer_en          : 1;	//    31
} LQC_INTR_TIMER;

/*-----------------------------------------------------------------------------
	0x0030 lqc_status ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_lqc_lq_st                  : 8,	//  0: 7
	                                : 8,	//  8:15 reserved
	reg_lqc_discont_cnt            : 8;	// 16:23
} LQC_STATUS;

/*-----------------------------------------------------------------------------
	0x0034 lqc_sync_cnt ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_lqc_sync_cnt_init_value    :16,	//  0:15
	reg_lqc_sync_cnt_adj_num       : 8;	// 16:23
} LQC_SYNC_CNT;

/*-----------------------------------------------------------------------------
	0x0038 lqc_dts_stc ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_lqc_dts_stc_diff           :29;	//  0:28
} LQC_DTS_STC;

/*-----------------------------------------------------------------------------
	0x003c lqc_dts_gstc ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_lqc_dts_gstc_diff          :29;	//  0:28
} LQC_DTS_GSTC;

/*-----------------------------------------------------------------------------
	0x0104 lq_reset ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :28,	//  0:27 reserved
	reg_lqc0_srst                   : 1,	//    28
	reg_lqc1_srst                   : 1,	//    29
	reg_lqc2_srst                   : 1,	//    30
	reg_lqc3_srst                   : 1;	//    31
} LQ_RESET;

/*-----------------------------------------------------------------------------
	0x0108 lq_apb_prt ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_lq_apb_s0_prt               : 4,	//  0: 3
	reg_lq_apb_s1_prt               : 4,	//  4: 7
	reg_lq_apb_s2_prt               : 4,	//  8:11
	reg_lq_apb_s3_prt               : 4,	// 12:15
	reg_lq_apb_s4_prt               : 4;	// 16:19
} LQ_APB_PRT;

/*-----------------------------------------------------------------------------
	0x01e0 lqc_cmd_type ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_lqc_cmd_type               : 8;	//  0: 7
} LQC_CMD_TYPE;

/*-----------------------------------------------------------------------------
	0x01e4 lqc0_cmd_data ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_lqc_cmd_data               ;   	// 31: 0
} LQC_CMD_DATA;

typedef struct {
	LQC_CMD_TYPE	cmd_type;
	UINT32			cmd_data;
} LQC_CMD_T;


typedef struct {
	LQC_CONF                       	lqc_conf                       ;	// 0x0000 : ''
	LQC_LQ_CONF                    	lqc_lq_conf                    ;	// 0x0004 : ''
	LQC_LQ_AEMPTY_TIME             	lqc_lq_aempty_time             ;	// 0x0008 : ''
	LQC_STC_LOWER                   lqc_stc_lower                  ;	// 0x000c
	LQC_LQ_WPTR                    	lqc_lq_wptr                    ;	// 0x0010 : ''
	LQC_LQ_RPTR                    	lqc_lq_rptr                    ;	// 0x0014 : ''
	LQC_LQ_AFULL_LEV               	lqc_lq_afull_lev               ;	// 0x0018 : ''
	LQC_LQ_AEMPTY_LEV              	lqc_lq_aempty_lev              ;	// 0x001c : ''
	LQC_STC_UPPER                  	lqc_stc_upper                  ;	// 0x0020 : ''
	LQC_STC                         	lqc_stc                         ;	// 0x0024 : ''
	LQC_STC_GSTC                   	lqc_stc_gstc                   ;	// 0x0028 : ''
	LQC_INTR_TIMER                 	lqc_intr_timer                 ;	// 0x002c : ''
	LQC_STATUS                     	lqc_status                     ;	// 0x0030 : ''
	LQC_SYNC_CNT                   	lqc_sync_cnt                   ;	// 0x0034 : ''
	LQC_DTS_STC                    	lqc_dts_stc                    ;	// 0x0038 : ''
	LQC_DTS_GSTC                   	lqc_dts_gstc                   ;	// 0x003c : ''
} LQC_REG_T;
/* 15 regs, 15 types */

typedef struct {
	UINT32                          	lq_ver                          ;	// 0x0100 : ''
	LQ_RESET                        	lq_reset                        ;	// 0x0104 : ''
	LQ_APB_PRT                      	lq_apb_prt                      ;	// 0x0108 : ''
	UINT32                          	                 __rsvd_01[  53];	// 0x010c ~ 0x01dc
	LQC_CMD_T				lqc_cmd[4]					;	// 0x01e0 ~ 0x01fc
} LQ_REG_T;
/* 10 regs, 10 types */

/* 72 regs, 72 types in Total*/

/*
 * @{
 * Naming for register pointer.
 * gpRealRegLQC0 : real register of LQC0.
 * gpRegLQC0     : shadow register.
 *
 * @def LQC0_RdFL: Read  FLushing : Shadow <- Real.
 * @def LQC0_WrFL: Write FLushing : Shadow -> Real.
 * @def LQC0_Rd  : Read  whole register(UINT32) from Shadow register.
 * @def LQC0_Wr  : Write whole register(UINT32) from Shadow register.
 * @def LQC0_Rd01 ~ LQC0_Rdnn: Read  given '01~nn' fields from Shadow register.
 * @def LQC0_Wr01 ~ LQC0_Wrnn: Write given '01~nn' fields to   Shadow register.
 * */
#define LQC0_RdFL(_r)			((gpRegLQC0->_r)=(gpRealRegLQC0->_r))
#define LQC0_WrFL(_r)			((gpRealRegLQC0->_r)=(gpRegLQC0->_r))

#define LQC0_Rd(_r)			*((UINT32*)(&(gpRegLQC0->_r)))
#define LQC0_Wr(_r,_v)			((LQC0_Rd(_r))=((UINT32)(_v)))

#define LQC0_Rd01(_r,_f01,_v01)													\
								do { 											\
									(_v01) = (gpRegLQC0->_r._f01);				\
								} while(0)

#define LQC0_Rd02(_r,_f01,_v01,_f02,_v02)										\
								do { 											\
									(_v01) = (gpRegLQC0->_r._f01);				\
									(_v02) = (gpRegLQC0->_r._f02);				\
								} while(0)

#define LQC0_Rd03(_r,_f01,_v01,_f02,_v02,_f03,_v03)								\
								do { 											\
									(_v01) = (gpRegLQC0->_r._f01);				\
									(_v02) = (gpRegLQC0->_r._f02);				\
									(_v03) = (gpRegLQC0->_r._f03);				\
								} while(0)

#define LQC0_Rd04(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04)					\
								do { 											\
									(_v01) = (gpRegLQC0->_r._f01);				\
									(_v02) = (gpRegLQC0->_r._f02);				\
									(_v03) = (gpRegLQC0->_r._f03);				\
									(_v04) = (gpRegLQC0->_r._f04);				\
								} while(0)

#define LQC0_Rd05(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05)													\
								do { 											\
									(_v01) = (gpRegLQC0->_r._f01);				\
									(_v02) = (gpRegLQC0->_r._f02);				\
									(_v03) = (gpRegLQC0->_r._f03);				\
									(_v04) = (gpRegLQC0->_r._f04);				\
									(_v05) = (gpRegLQC0->_r._f05);				\
								} while(0)

#define LQC0_Rd06(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06)										\
								do { 											\
									(_v01) = (gpRegLQC0->_r._f01);				\
									(_v02) = (gpRegLQC0->_r._f02);				\
									(_v03) = (gpRegLQC0->_r._f03);				\
									(_v04) = (gpRegLQC0->_r._f04);				\
									(_v05) = (gpRegLQC0->_r._f05);				\
									(_v06) = (gpRegLQC0->_r._f06);				\
								} while(0)

#define LQC0_Rd07(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07)								\
								do { 											\
									(_v01) = (gpRegLQC0->_r._f01);				\
									(_v02) = (gpRegLQC0->_r._f02);				\
									(_v03) = (gpRegLQC0->_r._f03);				\
									(_v04) = (gpRegLQC0->_r._f04);				\
									(_v05) = (gpRegLQC0->_r._f05);				\
									(_v06) = (gpRegLQC0->_r._f06);				\
									(_v07) = (gpRegLQC0->_r._f07);				\
								} while(0)

#define LQC0_Rd08(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08)					\
								do { 											\
									(_v01) = (gpRegLQC0->_r._f01);				\
									(_v02) = (gpRegLQC0->_r._f02);				\
									(_v03) = (gpRegLQC0->_r._f03);				\
									(_v04) = (gpRegLQC0->_r._f04);				\
									(_v05) = (gpRegLQC0->_r._f05);				\
									(_v06) = (gpRegLQC0->_r._f06);				\
									(_v07) = (gpRegLQC0->_r._f07);				\
									(_v08) = (gpRegLQC0->_r._f08);				\
								} while(0)

#define LQC0_Rd09(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09)													\
								do { 											\
									(_v01) = (gpRegLQC0->_r._f01);				\
									(_v02) = (gpRegLQC0->_r._f02);				\
									(_v03) = (gpRegLQC0->_r._f03);				\
									(_v04) = (gpRegLQC0->_r._f04);				\
									(_v05) = (gpRegLQC0->_r._f05);				\
									(_v06) = (gpRegLQC0->_r._f06);				\
									(_v07) = (gpRegLQC0->_r._f07);				\
									(_v08) = (gpRegLQC0->_r._f08);				\
									(_v09) = (gpRegLQC0->_r._f09);				\
								} while(0)

#define LQC0_Rd10(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10)										\
								do { 											\
									(_v01) = (gpRegLQC0->_r._f01);				\
									(_v02) = (gpRegLQC0->_r._f02);				\
									(_v03) = (gpRegLQC0->_r._f03);				\
									(_v04) = (gpRegLQC0->_r._f04);				\
									(_v05) = (gpRegLQC0->_r._f05);				\
									(_v06) = (gpRegLQC0->_r._f06);				\
									(_v07) = (gpRegLQC0->_r._f07);				\
									(_v08) = (gpRegLQC0->_r._f08);				\
									(_v09) = (gpRegLQC0->_r._f09);				\
									(_v10) = (gpRegLQC0->_r._f10);				\
								} while(0)

#define LQC0_Rd11(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11)								\
								do { 											\
									(_v01) = (gpRegLQC0->_r._f01);				\
									(_v02) = (gpRegLQC0->_r._f02);				\
									(_v03) = (gpRegLQC0->_r._f03);				\
									(_v04) = (gpRegLQC0->_r._f04);				\
									(_v05) = (gpRegLQC0->_r._f05);				\
									(_v06) = (gpRegLQC0->_r._f06);				\
									(_v07) = (gpRegLQC0->_r._f07);				\
									(_v08) = (gpRegLQC0->_r._f08);				\
									(_v09) = (gpRegLQC0->_r._f09);				\
									(_v10) = (gpRegLQC0->_r._f10);				\
									(_v11) = (gpRegLQC0->_r._f11);				\
								} while(0)

#define LQC0_Rd12(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12)					\
								do { 											\
									(_v01) = (gpRegLQC0->_r._f01);				\
									(_v02) = (gpRegLQC0->_r._f02);				\
									(_v03) = (gpRegLQC0->_r._f03);				\
									(_v04) = (gpRegLQC0->_r._f04);				\
									(_v05) = (gpRegLQC0->_r._f05);				\
									(_v06) = (gpRegLQC0->_r._f06);				\
									(_v07) = (gpRegLQC0->_r._f07);				\
									(_v08) = (gpRegLQC0->_r._f08);				\
									(_v09) = (gpRegLQC0->_r._f09);				\
									(_v10) = (gpRegLQC0->_r._f10);				\
									(_v11) = (gpRegLQC0->_r._f11);				\
									(_v12) = (gpRegLQC0->_r._f12);				\
								} while(0)

#define LQC0_Rd13(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13)													\
								do { 											\
									(_v01) = (gpRegLQC0->_r._f01);				\
									(_v02) = (gpRegLQC0->_r._f02);				\
									(_v03) = (gpRegLQC0->_r._f03);				\
									(_v04) = (gpRegLQC0->_r._f04);				\
									(_v05) = (gpRegLQC0->_r._f05);				\
									(_v06) = (gpRegLQC0->_r._f06);				\
									(_v07) = (gpRegLQC0->_r._f07);				\
									(_v08) = (gpRegLQC0->_r._f08);				\
									(_v09) = (gpRegLQC0->_r._f09);				\
									(_v10) = (gpRegLQC0->_r._f10);				\
									(_v11) = (gpRegLQC0->_r._f11);				\
									(_v12) = (gpRegLQC0->_r._f12);				\
									(_v13) = (gpRegLQC0->_r._f13);				\
								} while(0)

#define LQC0_Rd14(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14)										\
								do { 											\
									(_v01) = (gpRegLQC0->_r._f01);				\
									(_v02) = (gpRegLQC0->_r._f02);				\
									(_v03) = (gpRegLQC0->_r._f03);				\
									(_v04) = (gpRegLQC0->_r._f04);				\
									(_v05) = (gpRegLQC0->_r._f05);				\
									(_v06) = (gpRegLQC0->_r._f06);				\
									(_v07) = (gpRegLQC0->_r._f07);				\
									(_v08) = (gpRegLQC0->_r._f08);				\
									(_v09) = (gpRegLQC0->_r._f09);				\
									(_v10) = (gpRegLQC0->_r._f10);				\
									(_v11) = (gpRegLQC0->_r._f11);				\
									(_v12) = (gpRegLQC0->_r._f12);				\
									(_v13) = (gpRegLQC0->_r._f13);				\
									(_v14) = (gpRegLQC0->_r._f14);				\
								} while(0)

#define LQC0_Rd15(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15)								\
								do { 											\
									(_v01) = (gpRegLQC0->_r._f01);				\
									(_v02) = (gpRegLQC0->_r._f02);				\
									(_v03) = (gpRegLQC0->_r._f03);				\
									(_v04) = (gpRegLQC0->_r._f04);				\
									(_v05) = (gpRegLQC0->_r._f05);				\
									(_v06) = (gpRegLQC0->_r._f06);				\
									(_v07) = (gpRegLQC0->_r._f07);				\
									(_v08) = (gpRegLQC0->_r._f08);				\
									(_v09) = (gpRegLQC0->_r._f09);				\
									(_v10) = (gpRegLQC0->_r._f10);				\
									(_v11) = (gpRegLQC0->_r._f11);				\
									(_v12) = (gpRegLQC0->_r._f12);				\
									(_v13) = (gpRegLQC0->_r._f13);				\
									(_v14) = (gpRegLQC0->_r._f14);				\
									(_v15) = (gpRegLQC0->_r._f15);				\
								} while(0)

#define LQC0_Rd16(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15,_f16,_v16)					\
								do { 											\
									(_v01) = (gpRegLQC0->_r._f01);				\
									(_v02) = (gpRegLQC0->_r._f02);				\
									(_v03) = (gpRegLQC0->_r._f03);				\
									(_v04) = (gpRegLQC0->_r._f04);				\
									(_v05) = (gpRegLQC0->_r._f05);				\
									(_v06) = (gpRegLQC0->_r._f06);				\
									(_v07) = (gpRegLQC0->_r._f07);				\
									(_v08) = (gpRegLQC0->_r._f08);				\
									(_v09) = (gpRegLQC0->_r._f09);				\
									(_v10) = (gpRegLQC0->_r._f10);				\
									(_v11) = (gpRegLQC0->_r._f11);				\
									(_v12) = (gpRegLQC0->_r._f12);				\
									(_v13) = (gpRegLQC0->_r._f13);				\
									(_v14) = (gpRegLQC0->_r._f14);				\
									(_v15) = (gpRegLQC0->_r._f15);				\
									(_v16) = (gpRegLQC0->_r._f16);				\
								} while(0)


#define LQC0_Wr01(_r,_f01,_v01)													\
								do { 											\
									(gpRegLQC0->_r._f01) = (_v01);				\
								} while(0)

#define LQC0_Wr02(_r,_f01,_v01,_f02,_v02)										\
								do { 											\
									(gpRegLQC0->_r._f01) = (_v01);				\
									(gpRegLQC0->_r._f02) = (_v02);				\
								} while(0)

#define LQC0_Wr03(_r,_f01,_v01,_f02,_v02,_f03,_v03)								\
								do { 											\
									(gpRegLQC0->_r._f01) = (_v01);				\
									(gpRegLQC0->_r._f02) = (_v02);				\
									(gpRegLQC0->_r._f03) = (_v03);				\
								} while(0)

#define LQC0_Wr04(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04)					\
								do { 											\
									(gpRegLQC0->_r._f01) = (_v01);				\
									(gpRegLQC0->_r._f02) = (_v02);				\
									(gpRegLQC0->_r._f03) = (_v03);				\
									(gpRegLQC0->_r._f04) = (_v04);				\
								} while(0)

#define LQC0_Wr05(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05)													\
								do { 											\
									(gpRegLQC0->_r._f01) = (_v01);				\
									(gpRegLQC0->_r._f02) = (_v02);				\
									(gpRegLQC0->_r._f03) = (_v03);				\
									(gpRegLQC0->_r._f04) = (_v04);				\
									(gpRegLQC0->_r._f05) = (_v05);				\
								} while(0)

#define LQC0_Wr06(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06)										\
								do { 											\
									(gpRegLQC0->_r._f01) = (_v01);				\
									(gpRegLQC0->_r._f02) = (_v02);				\
									(gpRegLQC0->_r._f03) = (_v03);				\
									(gpRegLQC0->_r._f04) = (_v04);				\
									(gpRegLQC0->_r._f05) = (_v05);				\
									(gpRegLQC0->_r._f06) = (_v06);				\
								} while(0)

#define LQC0_Wr07(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07)								\
								do { 											\
									(gpRegLQC0->_r._f01) = (_v01);				\
									(gpRegLQC0->_r._f02) = (_v02);				\
									(gpRegLQC0->_r._f03) = (_v03);				\
									(gpRegLQC0->_r._f04) = (_v04);				\
									(gpRegLQC0->_r._f05) = (_v05);				\
									(gpRegLQC0->_r._f06) = (_v06);				\
									(gpRegLQC0->_r._f07) = (_v07);				\
								} while(0)

#define LQC0_Wr08(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08)					\
								do { 											\
									(gpRegLQC0->_r._f01) = (_v01);				\
									(gpRegLQC0->_r._f02) = (_v02);				\
									(gpRegLQC0->_r._f03) = (_v03);				\
									(gpRegLQC0->_r._f04) = (_v04);				\
									(gpRegLQC0->_r._f05) = (_v05);				\
									(gpRegLQC0->_r._f06) = (_v06);				\
									(gpRegLQC0->_r._f07) = (_v07);				\
									(gpRegLQC0->_r._f08) = (_v08);				\
								} while(0)

#define LQC0_Wr09(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09)													\
								do { 											\
									(gpRegLQC0->_r._f01) = (_v01);				\
									(gpRegLQC0->_r._f02) = (_v02);				\
									(gpRegLQC0->_r._f03) = (_v03);				\
									(gpRegLQC0->_r._f04) = (_v04);				\
									(gpRegLQC0->_r._f05) = (_v05);				\
									(gpRegLQC0->_r._f06) = (_v06);				\
									(gpRegLQC0->_r._f07) = (_v07);				\
									(gpRegLQC0->_r._f08) = (_v08);				\
									(gpRegLQC0->_r._f09) = (_v09);				\
								} while(0)

#define LQC0_Wr10(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10)										\
								do { 											\
									(gpRegLQC0->_r._f01) = (_v01);				\
									(gpRegLQC0->_r._f02) = (_v02);				\
									(gpRegLQC0->_r._f03) = (_v03);				\
									(gpRegLQC0->_r._f04) = (_v04);				\
									(gpRegLQC0->_r._f05) = (_v05);				\
									(gpRegLQC0->_r._f06) = (_v06);				\
									(gpRegLQC0->_r._f07) = (_v07);				\
									(gpRegLQC0->_r._f08) = (_v08);				\
									(gpRegLQC0->_r._f09) = (_v09);				\
									(gpRegLQC0->_r._f10) = (_v10);				\
								} while(0)

#define LQC0_Wr11(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11)								\
								do { 											\
									(gpRegLQC0->_r._f01) = (_v01);				\
									(gpRegLQC0->_r._f02) = (_v02);				\
									(gpRegLQC0->_r._f03) = (_v03);				\
									(gpRegLQC0->_r._f04) = (_v04);				\
									(gpRegLQC0->_r._f05) = (_v05);				\
									(gpRegLQC0->_r._f06) = (_v06);				\
									(gpRegLQC0->_r._f07) = (_v07);				\
									(gpRegLQC0->_r._f08) = (_v08);				\
									(gpRegLQC0->_r._f09) = (_v09);				\
									(gpRegLQC0->_r._f10) = (_v10);				\
									(gpRegLQC0->_r._f11) = (_v11);				\
								} while(0)

#define LQC0_Wr12(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12)					\
								do { 											\
									(gpRegLQC0->_r._f01) = (_v01);				\
									(gpRegLQC0->_r._f02) = (_v02);				\
									(gpRegLQC0->_r._f03) = (_v03);				\
									(gpRegLQC0->_r._f04) = (_v04);				\
									(gpRegLQC0->_r._f05) = (_v05);				\
									(gpRegLQC0->_r._f06) = (_v06);				\
									(gpRegLQC0->_r._f07) = (_v07);				\
									(gpRegLQC0->_r._f08) = (_v08);				\
									(gpRegLQC0->_r._f09) = (_v09);				\
									(gpRegLQC0->_r._f10) = (_v10);				\
									(gpRegLQC0->_r._f11) = (_v11);				\
									(gpRegLQC0->_r._f12) = (_v12);				\
								} while(0)

#define LQC0_Wr13(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13)													\
								do { 											\
									(gpRegLQC0->_r._f01) = (_v01);				\
									(gpRegLQC0->_r._f02) = (_v02);				\
									(gpRegLQC0->_r._f03) = (_v03);				\
									(gpRegLQC0->_r._f04) = (_v04);				\
									(gpRegLQC0->_r._f05) = (_v05);				\
									(gpRegLQC0->_r._f06) = (_v06);				\
									(gpRegLQC0->_r._f07) = (_v07);				\
									(gpRegLQC0->_r._f08) = (_v08);				\
									(gpRegLQC0->_r._f09) = (_v09);				\
									(gpRegLQC0->_r._f10) = (_v10);				\
									(gpRegLQC0->_r._f11) = (_v11);				\
									(gpRegLQC0->_r._f12) = (_v12);				\
									(gpRegLQC0->_r._f13) = (_v13);				\
								} while(0)

#define LQC0_Wr14(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14)										\
								do { 											\
									(gpRegLQC0->_r._f01) = (_v01);				\
									(gpRegLQC0->_r._f02) = (_v02);				\
									(gpRegLQC0->_r._f03) = (_v03);				\
									(gpRegLQC0->_r._f04) = (_v04);				\
									(gpRegLQC0->_r._f05) = (_v05);				\
									(gpRegLQC0->_r._f06) = (_v06);				\
									(gpRegLQC0->_r._f07) = (_v07);				\
									(gpRegLQC0->_r._f08) = (_v08);				\
									(gpRegLQC0->_r._f09) = (_v09);				\
									(gpRegLQC0->_r._f10) = (_v10);				\
									(gpRegLQC0->_r._f11) = (_v11);				\
									(gpRegLQC0->_r._f12) = (_v12);				\
									(gpRegLQC0->_r._f13) = (_v13);				\
									(gpRegLQC0->_r._f14) = (_v14);				\
								} while(0)

#define LQC0_Wr15(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15)								\
								do { 											\
									(gpRegLQC0->_r._f01) = (_v01);				\
									(gpRegLQC0->_r._f02) = (_v02);				\
									(gpRegLQC0->_r._f03) = (_v03);				\
									(gpRegLQC0->_r._f04) = (_v04);				\
									(gpRegLQC0->_r._f05) = (_v05);				\
									(gpRegLQC0->_r._f06) = (_v06);				\
									(gpRegLQC0->_r._f07) = (_v07);				\
									(gpRegLQC0->_r._f08) = (_v08);				\
									(gpRegLQC0->_r._f09) = (_v09);				\
									(gpRegLQC0->_r._f10) = (_v10);				\
									(gpRegLQC0->_r._f11) = (_v11);				\
									(gpRegLQC0->_r._f12) = (_v12);				\
									(gpRegLQC0->_r._f13) = (_v13);				\
									(gpRegLQC0->_r._f14) = (_v14);				\
									(gpRegLQC0->_r._f15) = (_v15);				\
								} while(0)

#define LQC0_Wr16(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15,_f16,_v16)					\
								do { 											\
									(gpRegLQC0->_r._f01) = (_v01);				\
									(gpRegLQC0->_r._f02) = (_v02);				\
									(gpRegLQC0->_r._f03) = (_v03);				\
									(gpRegLQC0->_r._f04) = (_v04);				\
									(gpRegLQC0->_r._f05) = (_v05);				\
									(gpRegLQC0->_r._f06) = (_v06);				\
									(gpRegLQC0->_r._f07) = (_v07);				\
									(gpRegLQC0->_r._f08) = (_v08);				\
									(gpRegLQC0->_r._f09) = (_v09);				\
									(gpRegLQC0->_r._f10) = (_v10);				\
									(gpRegLQC0->_r._f11) = (_v11);				\
									(gpRegLQC0->_r._f12) = (_v12);				\
									(gpRegLQC0->_r._f13) = (_v13);				\
									(gpRegLQC0->_r._f14) = (_v14);				\
									(gpRegLQC0->_r._f15) = (_v15);				\
									(gpRegLQC0->_r._f16) = (_v16);				\
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
 * _rwname: name of rw    bit field : shall be 0 after LQC0_Wind(), 1 for LQC0_Rind()
 * _iname : name of index bit field
 * _ival  : index value
 * _fname : field name
 * _fval  : field variable that field value shall be stored.
 *
 * LQC0_Rind : General indexed register Read.(
 * LQC0_Wind : General indexed register Read.
 *
 * LQC0_Ridx : For 'index', 'rw', 'load' field name
 * LQC0_Widx : For 'index', 'rw', 'load' field name and NO_LOAD.
 */
#define LQC0_Rind(_r, _lname, _rwname, _iname, _ival, _fname, _fval)				\
							do {												\
								LQC0_Wr03(_r,_lname,0,_rwname,1,_iname,_ival);	\
								LQC0_WrFL(_r);									\
								LQC0_RdFL(_r);									\
								LQC0_Rd01(_r,_fname,_fval);						\
							} while (0)

#define LQC0_Wind(_r, _lname, _rwname, _iname, _ival, _fname, _fval)				\
				LQC0_Wr04(_r, _lname,0, _rwname,0, _iname,_ival, _fname,_fval)


#define LQC0_Ridx(_r, _ival, _fname, _fval)	LQC0_Rind(_r,load,rw,index,_ival,_fname,_fval)

#define LQC0_Widx(_r, _ival, _fname, _fval)	LQC0_Wind(_r,load,rw,index,_ival,_fname,_fval)

/** @} *//* end of macro documentation */

/*
 * @{
 * Naming for register pointer.
 * gpRealRegLQC : real register of LQC.
 * gpRegLQC     : shadow register.
 *
 * @def LQC_RdFL: Read  FLushing : Shadow <- Real.
 * @def LQC_WrFL: Write FLushing : Shadow -> Real.
 * @def LQC_Rd  : Read  whole register(UINT32) from Shadow register.
 * @def LQC_Wr  : Write whole register(UINT32) from Shadow register.
 * @def LQC_Rd01 ~ LQC_Rdnn: Read  given '01~nn' fields from Shadow register.
 * @def LQC_Wr01 ~ LQC_Wrnn: Write given '01~nn' fields to   Shadow register.
 * */
#define LQC_RdFL(_r)			((gpRegLQC->_r)=(gpRealRegLQC->_r))
#define LQC_WrFL(_r)			((gpRealRegLQC->_r)=(gpRegLQC->_r))

#define LQC_Rd(_r)			*((UINT32*)(&(gpRegLQC->_r)))
#define LQC_Wr(_r,_v)			((LQC_Rd(_r))=((UINT32)(_v)))

#define LQC_Rd01(_r,_f01,_v01)													\
								do { 											\
									(_v01) = (gpRegLQC->_r._f01);				\
								} while(0)

#define LQC_Rd02(_r,_f01,_v01,_f02,_v02)										\
								do { 											\
									(_v01) = (gpRegLQC->_r._f01);				\
									(_v02) = (gpRegLQC->_r._f02);				\
								} while(0)

#define LQC_Rd03(_r,_f01,_v01,_f02,_v02,_f03,_v03)								\
								do { 											\
									(_v01) = (gpRegLQC->_r._f01);				\
									(_v02) = (gpRegLQC->_r._f02);				\
									(_v03) = (gpRegLQC->_r._f03);				\
								} while(0)

#define LQC_Rd04(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04)					\
								do { 											\
									(_v01) = (gpRegLQC->_r._f01);				\
									(_v02) = (gpRegLQC->_r._f02);				\
									(_v03) = (gpRegLQC->_r._f03);				\
									(_v04) = (gpRegLQC->_r._f04);				\
								} while(0)

#define LQC_Rd05(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05)													\
								do { 											\
									(_v01) = (gpRegLQC->_r._f01);				\
									(_v02) = (gpRegLQC->_r._f02);				\
									(_v03) = (gpRegLQC->_r._f03);				\
									(_v04) = (gpRegLQC->_r._f04);				\
									(_v05) = (gpRegLQC->_r._f05);				\
								} while(0)

#define LQC_Rd06(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06)										\
								do { 											\
									(_v01) = (gpRegLQC->_r._f01);				\
									(_v02) = (gpRegLQC->_r._f02);				\
									(_v03) = (gpRegLQC->_r._f03);				\
									(_v04) = (gpRegLQC->_r._f04);				\
									(_v05) = (gpRegLQC->_r._f05);				\
									(_v06) = (gpRegLQC->_r._f06);				\
								} while(0)

#define LQC_Rd07(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07)								\
								do { 											\
									(_v01) = (gpRegLQC->_r._f01);				\
									(_v02) = (gpRegLQC->_r._f02);				\
									(_v03) = (gpRegLQC->_r._f03);				\
									(_v04) = (gpRegLQC->_r._f04);				\
									(_v05) = (gpRegLQC->_r._f05);				\
									(_v06) = (gpRegLQC->_r._f06);				\
									(_v07) = (gpRegLQC->_r._f07);				\
								} while(0)

#define LQC_Rd08(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08)					\
								do { 											\
									(_v01) = (gpRegLQC->_r._f01);				\
									(_v02) = (gpRegLQC->_r._f02);				\
									(_v03) = (gpRegLQC->_r._f03);				\
									(_v04) = (gpRegLQC->_r._f04);				\
									(_v05) = (gpRegLQC->_r._f05);				\
									(_v06) = (gpRegLQC->_r._f06);				\
									(_v07) = (gpRegLQC->_r._f07);				\
									(_v08) = (gpRegLQC->_r._f08);				\
								} while(0)

#define LQC_Rd09(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09)													\
								do { 											\
									(_v01) = (gpRegLQC->_r._f01);				\
									(_v02) = (gpRegLQC->_r._f02);				\
									(_v03) = (gpRegLQC->_r._f03);				\
									(_v04) = (gpRegLQC->_r._f04);				\
									(_v05) = (gpRegLQC->_r._f05);				\
									(_v06) = (gpRegLQC->_r._f06);				\
									(_v07) = (gpRegLQC->_r._f07);				\
									(_v08) = (gpRegLQC->_r._f08);				\
									(_v09) = (gpRegLQC->_r._f09);				\
								} while(0)

#define LQC_Rd10(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10)										\
								do { 											\
									(_v01) = (gpRegLQC->_r._f01);				\
									(_v02) = (gpRegLQC->_r._f02);				\
									(_v03) = (gpRegLQC->_r._f03);				\
									(_v04) = (gpRegLQC->_r._f04);				\
									(_v05) = (gpRegLQC->_r._f05);				\
									(_v06) = (gpRegLQC->_r._f06);				\
									(_v07) = (gpRegLQC->_r._f07);				\
									(_v08) = (gpRegLQC->_r._f08);				\
									(_v09) = (gpRegLQC->_r._f09);				\
									(_v10) = (gpRegLQC->_r._f10);				\
								} while(0)

#define LQC_Rd11(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11)								\
								do { 											\
									(_v01) = (gpRegLQC->_r._f01);				\
									(_v02) = (gpRegLQC->_r._f02);				\
									(_v03) = (gpRegLQC->_r._f03);				\
									(_v04) = (gpRegLQC->_r._f04);				\
									(_v05) = (gpRegLQC->_r._f05);				\
									(_v06) = (gpRegLQC->_r._f06);				\
									(_v07) = (gpRegLQC->_r._f07);				\
									(_v08) = (gpRegLQC->_r._f08);				\
									(_v09) = (gpRegLQC->_r._f09);				\
									(_v10) = (gpRegLQC->_r._f10);				\
									(_v11) = (gpRegLQC->_r._f11);				\
								} while(0)

#define LQC_Rd12(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12)					\
								do { 											\
									(_v01) = (gpRegLQC->_r._f01);				\
									(_v02) = (gpRegLQC->_r._f02);				\
									(_v03) = (gpRegLQC->_r._f03);				\
									(_v04) = (gpRegLQC->_r._f04);				\
									(_v05) = (gpRegLQC->_r._f05);				\
									(_v06) = (gpRegLQC->_r._f06);				\
									(_v07) = (gpRegLQC->_r._f07);				\
									(_v08) = (gpRegLQC->_r._f08);				\
									(_v09) = (gpRegLQC->_r._f09);				\
									(_v10) = (gpRegLQC->_r._f10);				\
									(_v11) = (gpRegLQC->_r._f11);				\
									(_v12) = (gpRegLQC->_r._f12);				\
								} while(0)

#define LQC_Rd13(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13)													\
								do { 											\
									(_v01) = (gpRegLQC->_r._f01);				\
									(_v02) = (gpRegLQC->_r._f02);				\
									(_v03) = (gpRegLQC->_r._f03);				\
									(_v04) = (gpRegLQC->_r._f04);				\
									(_v05) = (gpRegLQC->_r._f05);				\
									(_v06) = (gpRegLQC->_r._f06);				\
									(_v07) = (gpRegLQC->_r._f07);				\
									(_v08) = (gpRegLQC->_r._f08);				\
									(_v09) = (gpRegLQC->_r._f09);				\
									(_v10) = (gpRegLQC->_r._f10);				\
									(_v11) = (gpRegLQC->_r._f11);				\
									(_v12) = (gpRegLQC->_r._f12);				\
									(_v13) = (gpRegLQC->_r._f13);				\
								} while(0)

#define LQC_Rd14(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14)										\
								do { 											\
									(_v01) = (gpRegLQC->_r._f01);				\
									(_v02) = (gpRegLQC->_r._f02);				\
									(_v03) = (gpRegLQC->_r._f03);				\
									(_v04) = (gpRegLQC->_r._f04);				\
									(_v05) = (gpRegLQC->_r._f05);				\
									(_v06) = (gpRegLQC->_r._f06);				\
									(_v07) = (gpRegLQC->_r._f07);				\
									(_v08) = (gpRegLQC->_r._f08);				\
									(_v09) = (gpRegLQC->_r._f09);				\
									(_v10) = (gpRegLQC->_r._f10);				\
									(_v11) = (gpRegLQC->_r._f11);				\
									(_v12) = (gpRegLQC->_r._f12);				\
									(_v13) = (gpRegLQC->_r._f13);				\
									(_v14) = (gpRegLQC->_r._f14);				\
								} while(0)

#define LQC_Rd15(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15)								\
								do { 											\
									(_v01) = (gpRegLQC->_r._f01);				\
									(_v02) = (gpRegLQC->_r._f02);				\
									(_v03) = (gpRegLQC->_r._f03);				\
									(_v04) = (gpRegLQC->_r._f04);				\
									(_v05) = (gpRegLQC->_r._f05);				\
									(_v06) = (gpRegLQC->_r._f06);				\
									(_v07) = (gpRegLQC->_r._f07);				\
									(_v08) = (gpRegLQC->_r._f08);				\
									(_v09) = (gpRegLQC->_r._f09);				\
									(_v10) = (gpRegLQC->_r._f10);				\
									(_v11) = (gpRegLQC->_r._f11);				\
									(_v12) = (gpRegLQC->_r._f12);				\
									(_v13) = (gpRegLQC->_r._f13);				\
									(_v14) = (gpRegLQC->_r._f14);				\
									(_v15) = (gpRegLQC->_r._f15);				\
								} while(0)

#define LQC_Rd16(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15,_f16,_v16)					\
								do { 											\
									(_v01) = (gpRegLQC->_r._f01);				\
									(_v02) = (gpRegLQC->_r._f02);				\
									(_v03) = (gpRegLQC->_r._f03);				\
									(_v04) = (gpRegLQC->_r._f04);				\
									(_v05) = (gpRegLQC->_r._f05);				\
									(_v06) = (gpRegLQC->_r._f06);				\
									(_v07) = (gpRegLQC->_r._f07);				\
									(_v08) = (gpRegLQC->_r._f08);				\
									(_v09) = (gpRegLQC->_r._f09);				\
									(_v10) = (gpRegLQC->_r._f10);				\
									(_v11) = (gpRegLQC->_r._f11);				\
									(_v12) = (gpRegLQC->_r._f12);				\
									(_v13) = (gpRegLQC->_r._f13);				\
									(_v14) = (gpRegLQC->_r._f14);				\
									(_v15) = (gpRegLQC->_r._f15);				\
									(_v16) = (gpRegLQC->_r._f16);				\
								} while(0)


#define LQC_Wr01(_r,_f01,_v01)													\
								do { 											\
									(gpRegLQC->_r._f01) = (_v01);				\
								} while(0)

#define LQC_Wr02(_r,_f01,_v01,_f02,_v02)										\
								do { 											\
									(gpRegLQC->_r._f01) = (_v01);				\
									(gpRegLQC->_r._f02) = (_v02);				\
								} while(0)

#define LQC_Wr03(_r,_f01,_v01,_f02,_v02,_f03,_v03)								\
								do { 											\
									(gpRegLQC->_r._f01) = (_v01);				\
									(gpRegLQC->_r._f02) = (_v02);				\
									(gpRegLQC->_r._f03) = (_v03);				\
								} while(0)

#define LQC_Wr04(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04)					\
								do { 											\
									(gpRegLQC->_r._f01) = (_v01);				\
									(gpRegLQC->_r._f02) = (_v02);				\
									(gpRegLQC->_r._f03) = (_v03);				\
									(gpRegLQC->_r._f04) = (_v04);				\
								} while(0)

#define LQC_Wr05(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05)													\
								do { 											\
									(gpRegLQC->_r._f01) = (_v01);				\
									(gpRegLQC->_r._f02) = (_v02);				\
									(gpRegLQC->_r._f03) = (_v03);				\
									(gpRegLQC->_r._f04) = (_v04);				\
									(gpRegLQC->_r._f05) = (_v05);				\
								} while(0)

#define LQC_Wr06(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06)										\
								do { 											\
									(gpRegLQC->_r._f01) = (_v01);				\
									(gpRegLQC->_r._f02) = (_v02);				\
									(gpRegLQC->_r._f03) = (_v03);				\
									(gpRegLQC->_r._f04) = (_v04);				\
									(gpRegLQC->_r._f05) = (_v05);				\
									(gpRegLQC->_r._f06) = (_v06);				\
								} while(0)

#define LQC_Wr07(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07)								\
								do { 											\
									(gpRegLQC->_r._f01) = (_v01);				\
									(gpRegLQC->_r._f02) = (_v02);				\
									(gpRegLQC->_r._f03) = (_v03);				\
									(gpRegLQC->_r._f04) = (_v04);				\
									(gpRegLQC->_r._f05) = (_v05);				\
									(gpRegLQC->_r._f06) = (_v06);				\
									(gpRegLQC->_r._f07) = (_v07);				\
								} while(0)

#define LQC_Wr08(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08)					\
								do { 											\
									(gpRegLQC->_r._f01) = (_v01);				\
									(gpRegLQC->_r._f02) = (_v02);				\
									(gpRegLQC->_r._f03) = (_v03);				\
									(gpRegLQC->_r._f04) = (_v04);				\
									(gpRegLQC->_r._f05) = (_v05);				\
									(gpRegLQC->_r._f06) = (_v06);				\
									(gpRegLQC->_r._f07) = (_v07);				\
									(gpRegLQC->_r._f08) = (_v08);				\
								} while(0)

#define LQC_Wr09(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09)													\
								do { 											\
									(gpRegLQC->_r._f01) = (_v01);				\
									(gpRegLQC->_r._f02) = (_v02);				\
									(gpRegLQC->_r._f03) = (_v03);				\
									(gpRegLQC->_r._f04) = (_v04);				\
									(gpRegLQC->_r._f05) = (_v05);				\
									(gpRegLQC->_r._f06) = (_v06);				\
									(gpRegLQC->_r._f07) = (_v07);				\
									(gpRegLQC->_r._f08) = (_v08);				\
									(gpRegLQC->_r._f09) = (_v09);				\
								} while(0)

#define LQC_Wr10(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10)										\
								do { 											\
									(gpRegLQC->_r._f01) = (_v01);				\
									(gpRegLQC->_r._f02) = (_v02);				\
									(gpRegLQC->_r._f03) = (_v03);				\
									(gpRegLQC->_r._f04) = (_v04);				\
									(gpRegLQC->_r._f05) = (_v05);				\
									(gpRegLQC->_r._f06) = (_v06);				\
									(gpRegLQC->_r._f07) = (_v07);				\
									(gpRegLQC->_r._f08) = (_v08);				\
									(gpRegLQC->_r._f09) = (_v09);				\
									(gpRegLQC->_r._f10) = (_v10);				\
								} while(0)

#define LQC_Wr11(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11)								\
								do { 											\
									(gpRegLQC->_r._f01) = (_v01);				\
									(gpRegLQC->_r._f02) = (_v02);				\
									(gpRegLQC->_r._f03) = (_v03);				\
									(gpRegLQC->_r._f04) = (_v04);				\
									(gpRegLQC->_r._f05) = (_v05);				\
									(gpRegLQC->_r._f06) = (_v06);				\
									(gpRegLQC->_r._f07) = (_v07);				\
									(gpRegLQC->_r._f08) = (_v08);				\
									(gpRegLQC->_r._f09) = (_v09);				\
									(gpRegLQC->_r._f10) = (_v10);				\
									(gpRegLQC->_r._f11) = (_v11);				\
								} while(0)

#define LQC_Wr12(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12)					\
								do { 											\
									(gpRegLQC->_r._f01) = (_v01);				\
									(gpRegLQC->_r._f02) = (_v02);				\
									(gpRegLQC->_r._f03) = (_v03);				\
									(gpRegLQC->_r._f04) = (_v04);				\
									(gpRegLQC->_r._f05) = (_v05);				\
									(gpRegLQC->_r._f06) = (_v06);				\
									(gpRegLQC->_r._f07) = (_v07);				\
									(gpRegLQC->_r._f08) = (_v08);				\
									(gpRegLQC->_r._f09) = (_v09);				\
									(gpRegLQC->_r._f10) = (_v10);				\
									(gpRegLQC->_r._f11) = (_v11);				\
									(gpRegLQC->_r._f12) = (_v12);				\
								} while(0)

#define LQC_Wr13(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13)													\
								do { 											\
									(gpRegLQC->_r._f01) = (_v01);				\
									(gpRegLQC->_r._f02) = (_v02);				\
									(gpRegLQC->_r._f03) = (_v03);				\
									(gpRegLQC->_r._f04) = (_v04);				\
									(gpRegLQC->_r._f05) = (_v05);				\
									(gpRegLQC->_r._f06) = (_v06);				\
									(gpRegLQC->_r._f07) = (_v07);				\
									(gpRegLQC->_r._f08) = (_v08);				\
									(gpRegLQC->_r._f09) = (_v09);				\
									(gpRegLQC->_r._f10) = (_v10);				\
									(gpRegLQC->_r._f11) = (_v11);				\
									(gpRegLQC->_r._f12) = (_v12);				\
									(gpRegLQC->_r._f13) = (_v13);				\
								} while(0)

#define LQC_Wr14(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14)										\
								do { 											\
									(gpRegLQC->_r._f01) = (_v01);				\
									(gpRegLQC->_r._f02) = (_v02);				\
									(gpRegLQC->_r._f03) = (_v03);				\
									(gpRegLQC->_r._f04) = (_v04);				\
									(gpRegLQC->_r._f05) = (_v05);				\
									(gpRegLQC->_r._f06) = (_v06);				\
									(gpRegLQC->_r._f07) = (_v07);				\
									(gpRegLQC->_r._f08) = (_v08);				\
									(gpRegLQC->_r._f09) = (_v09);				\
									(gpRegLQC->_r._f10) = (_v10);				\
									(gpRegLQC->_r._f11) = (_v11);				\
									(gpRegLQC->_r._f12) = (_v12);				\
									(gpRegLQC->_r._f13) = (_v13);				\
									(gpRegLQC->_r._f14) = (_v14);				\
								} while(0)

#define LQC_Wr15(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15)								\
								do { 											\
									(gpRegLQC->_r._f01) = (_v01);				\
									(gpRegLQC->_r._f02) = (_v02);				\
									(gpRegLQC->_r._f03) = (_v03);				\
									(gpRegLQC->_r._f04) = (_v04);				\
									(gpRegLQC->_r._f05) = (_v05);				\
									(gpRegLQC->_r._f06) = (_v06);				\
									(gpRegLQC->_r._f07) = (_v07);				\
									(gpRegLQC->_r._f08) = (_v08);				\
									(gpRegLQC->_r._f09) = (_v09);				\
									(gpRegLQC->_r._f10) = (_v10);				\
									(gpRegLQC->_r._f11) = (_v11);				\
									(gpRegLQC->_r._f12) = (_v12);				\
									(gpRegLQC->_r._f13) = (_v13);				\
									(gpRegLQC->_r._f14) = (_v14);				\
									(gpRegLQC->_r._f15) = (_v15);				\
								} while(0)

#define LQC_Wr16(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15,_f16,_v16)					\
								do { 											\
									(gpRegLQC->_r._f01) = (_v01);				\
									(gpRegLQC->_r._f02) = (_v02);				\
									(gpRegLQC->_r._f03) = (_v03);				\
									(gpRegLQC->_r._f04) = (_v04);				\
									(gpRegLQC->_r._f05) = (_v05);				\
									(gpRegLQC->_r._f06) = (_v06);				\
									(gpRegLQC->_r._f07) = (_v07);				\
									(gpRegLQC->_r._f08) = (_v08);				\
									(gpRegLQC->_r._f09) = (_v09);				\
									(gpRegLQC->_r._f10) = (_v10);				\
									(gpRegLQC->_r._f11) = (_v11);				\
									(gpRegLQC->_r._f12) = (_v12);				\
									(gpRegLQC->_r._f13) = (_v13);				\
									(gpRegLQC->_r._f14) = (_v14);				\
									(gpRegLQC->_r._f15) = (_v15);				\
									(gpRegLQC->_r._f16) = (_v16);				\
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
 * _rwname: name of rw    bit field : shall be 0 after LQC_Wind(), 1 for LQC_Rind()
 * _iname : name of index bit field
 * _ival  : index value
 * _fname : field name
 * _fval  : field variable that field value shall be stored.
 *
 * LQC_Rind : General indexed register Read.(
 * LQC_Wind : General indexed register Read.
 *
 * LQC_Ridx : For 'index', 'rw', 'load' field name
 * LQC_Widx : For 'index', 'rw', 'load' field name and NO_LOAD.
 */
#define LQC_Rind(_r, _lname, _rwname, _iname, _ival, _fname, _fval)				\
							do {												\
								LQC_Wr03(_r,_lname,0,_rwname,1,_iname,_ival);	\
								LQC_WrFL(_r);									\
								LQC_RdFL(_r);									\
								LQC_Rd01(_r,_fname,_fval);						\
							} while (0)

#define LQC_Wind(_r, _lname, _rwname, _iname, _ival, _fname, _fval)				\
				LQC_Wr04(_r, _lname,0, _rwname,0, _iname,_ival, _fname,_fval)


#define LQC_Ridx(_r, _ival, _fname, _fval)	LQC_Rind(_r,load,rw,index,_ival,_fname,_fval)

#define LQC_Widx(_r, _ival, _fname, _fval)	LQC_Wind(_r,load,rw,index,_ival,_fname,_fval)

/** @} *//* end of macro documentation */

/*
 * @{
 * Naming for register pointer.
 * gpRealRegLQ : real register of LQ.
 * gpRegLQ     : shadow register.
 *
 * @def LQ_RdFL: Read  FLushing : Shadow <- Real.
 * @def LQ_WrFL: Write FLushing : Shadow -> Real.
 * @def LQ_Rd  : Read  whole register(UINT32) from Shadow register.
 * @def LQ_Wr  : Write whole register(UINT32) from Shadow register.
 * @def LQ_Rd01 ~ LQ_Rdnn: Read  given '01~nn' fields from Shadow register.
 * @def LQ_Wr01 ~ LQ_Wrnn: Write given '01~nn' fields to   Shadow register.
 * */
#define LQ_RdFL(_r)			((gpRegLQ->_r)=(gpRealRegLQ->_r))
#define LQ_WrFL(_r)			((gpRealRegLQ->_r)=(gpRegLQ->_r))

#define LQ_Rd(_r)			*((UINT32*)(&(gpRegLQ->_r)))
#define LQ_Wr(_r,_v)			((LQ_Rd(_r))=((UINT32)(_v)))

#define LQ_Rd01(_r,_f01,_v01)													\
								do { 											\
									(_v01) = (gpRegLQ->_r._f01);				\
								} while(0)

#define LQ_Rd02(_r,_f01,_v01,_f02,_v02)										\
								do { 											\
									(_v01) = (gpRegLQ->_r._f01);				\
									(_v02) = (gpRegLQ->_r._f02);				\
								} while(0)

#define LQ_Rd03(_r,_f01,_v01,_f02,_v02,_f03,_v03)								\
								do { 											\
									(_v01) = (gpRegLQ->_r._f01);				\
									(_v02) = (gpRegLQ->_r._f02);				\
									(_v03) = (gpRegLQ->_r._f03);				\
								} while(0)

#define LQ_Rd04(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04)					\
								do { 											\
									(_v01) = (gpRegLQ->_r._f01);				\
									(_v02) = (gpRegLQ->_r._f02);				\
									(_v03) = (gpRegLQ->_r._f03);				\
									(_v04) = (gpRegLQ->_r._f04);				\
								} while(0)

#define LQ_Rd05(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05)													\
								do { 											\
									(_v01) = (gpRegLQ->_r._f01);				\
									(_v02) = (gpRegLQ->_r._f02);				\
									(_v03) = (gpRegLQ->_r._f03);				\
									(_v04) = (gpRegLQ->_r._f04);				\
									(_v05) = (gpRegLQ->_r._f05);				\
								} while(0)

#define LQ_Rd06(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06)										\
								do { 											\
									(_v01) = (gpRegLQ->_r._f01);				\
									(_v02) = (gpRegLQ->_r._f02);				\
									(_v03) = (gpRegLQ->_r._f03);				\
									(_v04) = (gpRegLQ->_r._f04);				\
									(_v05) = (gpRegLQ->_r._f05);				\
									(_v06) = (gpRegLQ->_r._f06);				\
								} while(0)

#define LQ_Rd07(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07)								\
								do { 											\
									(_v01) = (gpRegLQ->_r._f01);				\
									(_v02) = (gpRegLQ->_r._f02);				\
									(_v03) = (gpRegLQ->_r._f03);				\
									(_v04) = (gpRegLQ->_r._f04);				\
									(_v05) = (gpRegLQ->_r._f05);				\
									(_v06) = (gpRegLQ->_r._f06);				\
									(_v07) = (gpRegLQ->_r._f07);				\
								} while(0)

#define LQ_Rd08(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08)					\
								do { 											\
									(_v01) = (gpRegLQ->_r._f01);				\
									(_v02) = (gpRegLQ->_r._f02);				\
									(_v03) = (gpRegLQ->_r._f03);				\
									(_v04) = (gpRegLQ->_r._f04);				\
									(_v05) = (gpRegLQ->_r._f05);				\
									(_v06) = (gpRegLQ->_r._f06);				\
									(_v07) = (gpRegLQ->_r._f07);				\
									(_v08) = (gpRegLQ->_r._f08);				\
								} while(0)

#define LQ_Rd09(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09)													\
								do { 											\
									(_v01) = (gpRegLQ->_r._f01);				\
									(_v02) = (gpRegLQ->_r._f02);				\
									(_v03) = (gpRegLQ->_r._f03);				\
									(_v04) = (gpRegLQ->_r._f04);				\
									(_v05) = (gpRegLQ->_r._f05);				\
									(_v06) = (gpRegLQ->_r._f06);				\
									(_v07) = (gpRegLQ->_r._f07);				\
									(_v08) = (gpRegLQ->_r._f08);				\
									(_v09) = (gpRegLQ->_r._f09);				\
								} while(0)

#define LQ_Rd10(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10)										\
								do { 											\
									(_v01) = (gpRegLQ->_r._f01);				\
									(_v02) = (gpRegLQ->_r._f02);				\
									(_v03) = (gpRegLQ->_r._f03);				\
									(_v04) = (gpRegLQ->_r._f04);				\
									(_v05) = (gpRegLQ->_r._f05);				\
									(_v06) = (gpRegLQ->_r._f06);				\
									(_v07) = (gpRegLQ->_r._f07);				\
									(_v08) = (gpRegLQ->_r._f08);				\
									(_v09) = (gpRegLQ->_r._f09);				\
									(_v10) = (gpRegLQ->_r._f10);				\
								} while(0)

#define LQ_Rd11(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11)								\
								do { 											\
									(_v01) = (gpRegLQ->_r._f01);				\
									(_v02) = (gpRegLQ->_r._f02);				\
									(_v03) = (gpRegLQ->_r._f03);				\
									(_v04) = (gpRegLQ->_r._f04);				\
									(_v05) = (gpRegLQ->_r._f05);				\
									(_v06) = (gpRegLQ->_r._f06);				\
									(_v07) = (gpRegLQ->_r._f07);				\
									(_v08) = (gpRegLQ->_r._f08);				\
									(_v09) = (gpRegLQ->_r._f09);				\
									(_v10) = (gpRegLQ->_r._f10);				\
									(_v11) = (gpRegLQ->_r._f11);				\
								} while(0)

#define LQ_Rd12(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12)					\
								do { 											\
									(_v01) = (gpRegLQ->_r._f01);				\
									(_v02) = (gpRegLQ->_r._f02);				\
									(_v03) = (gpRegLQ->_r._f03);				\
									(_v04) = (gpRegLQ->_r._f04);				\
									(_v05) = (gpRegLQ->_r._f05);				\
									(_v06) = (gpRegLQ->_r._f06);				\
									(_v07) = (gpRegLQ->_r._f07);				\
									(_v08) = (gpRegLQ->_r._f08);				\
									(_v09) = (gpRegLQ->_r._f09);				\
									(_v10) = (gpRegLQ->_r._f10);				\
									(_v11) = (gpRegLQ->_r._f11);				\
									(_v12) = (gpRegLQ->_r._f12);				\
								} while(0)

#define LQ_Rd13(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13)													\
								do { 											\
									(_v01) = (gpRegLQ->_r._f01);				\
									(_v02) = (gpRegLQ->_r._f02);				\
									(_v03) = (gpRegLQ->_r._f03);				\
									(_v04) = (gpRegLQ->_r._f04);				\
									(_v05) = (gpRegLQ->_r._f05);				\
									(_v06) = (gpRegLQ->_r._f06);				\
									(_v07) = (gpRegLQ->_r._f07);				\
									(_v08) = (gpRegLQ->_r._f08);				\
									(_v09) = (gpRegLQ->_r._f09);				\
									(_v10) = (gpRegLQ->_r._f10);				\
									(_v11) = (gpRegLQ->_r._f11);				\
									(_v12) = (gpRegLQ->_r._f12);				\
									(_v13) = (gpRegLQ->_r._f13);				\
								} while(0)

#define LQ_Rd14(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14)										\
								do { 											\
									(_v01) = (gpRegLQ->_r._f01);				\
									(_v02) = (gpRegLQ->_r._f02);				\
									(_v03) = (gpRegLQ->_r._f03);				\
									(_v04) = (gpRegLQ->_r._f04);				\
									(_v05) = (gpRegLQ->_r._f05);				\
									(_v06) = (gpRegLQ->_r._f06);				\
									(_v07) = (gpRegLQ->_r._f07);				\
									(_v08) = (gpRegLQ->_r._f08);				\
									(_v09) = (gpRegLQ->_r._f09);				\
									(_v10) = (gpRegLQ->_r._f10);				\
									(_v11) = (gpRegLQ->_r._f11);				\
									(_v12) = (gpRegLQ->_r._f12);				\
									(_v13) = (gpRegLQ->_r._f13);				\
									(_v14) = (gpRegLQ->_r._f14);				\
								} while(0)

#define LQ_Rd15(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15)								\
								do { 											\
									(_v01) = (gpRegLQ->_r._f01);				\
									(_v02) = (gpRegLQ->_r._f02);				\
									(_v03) = (gpRegLQ->_r._f03);				\
									(_v04) = (gpRegLQ->_r._f04);				\
									(_v05) = (gpRegLQ->_r._f05);				\
									(_v06) = (gpRegLQ->_r._f06);				\
									(_v07) = (gpRegLQ->_r._f07);				\
									(_v08) = (gpRegLQ->_r._f08);				\
									(_v09) = (gpRegLQ->_r._f09);				\
									(_v10) = (gpRegLQ->_r._f10);				\
									(_v11) = (gpRegLQ->_r._f11);				\
									(_v12) = (gpRegLQ->_r._f12);				\
									(_v13) = (gpRegLQ->_r._f13);				\
									(_v14) = (gpRegLQ->_r._f14);				\
									(_v15) = (gpRegLQ->_r._f15);				\
								} while(0)

#define LQ_Rd16(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15,_f16,_v16)					\
								do { 											\
									(_v01) = (gpRegLQ->_r._f01);				\
									(_v02) = (gpRegLQ->_r._f02);				\
									(_v03) = (gpRegLQ->_r._f03);				\
									(_v04) = (gpRegLQ->_r._f04);				\
									(_v05) = (gpRegLQ->_r._f05);				\
									(_v06) = (gpRegLQ->_r._f06);				\
									(_v07) = (gpRegLQ->_r._f07);				\
									(_v08) = (gpRegLQ->_r._f08);				\
									(_v09) = (gpRegLQ->_r._f09);				\
									(_v10) = (gpRegLQ->_r._f10);				\
									(_v11) = (gpRegLQ->_r._f11);				\
									(_v12) = (gpRegLQ->_r._f12);				\
									(_v13) = (gpRegLQ->_r._f13);				\
									(_v14) = (gpRegLQ->_r._f14);				\
									(_v15) = (gpRegLQ->_r._f15);				\
									(_v16) = (gpRegLQ->_r._f16);				\
								} while(0)


#define LQ_Wr01(_r,_f01,_v01)													\
								do { 											\
									(gpRegLQ->_r._f01) = (_v01);				\
								} while(0)

#define LQ_Wr02(_r,_f01,_v01,_f02,_v02)										\
								do { 											\
									(gpRegLQ->_r._f01) = (_v01);				\
									(gpRegLQ->_r._f02) = (_v02);				\
								} while(0)

#define LQ_Wr03(_r,_f01,_v01,_f02,_v02,_f03,_v03)								\
								do { 											\
									(gpRegLQ->_r._f01) = (_v01);				\
									(gpRegLQ->_r._f02) = (_v02);				\
									(gpRegLQ->_r._f03) = (_v03);				\
								} while(0)

#define LQ_Wr04(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04)					\
								do { 											\
									(gpRegLQ->_r._f01) = (_v01);				\
									(gpRegLQ->_r._f02) = (_v02);				\
									(gpRegLQ->_r._f03) = (_v03);				\
									(gpRegLQ->_r._f04) = (_v04);				\
								} while(0)

#define LQ_Wr05(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05)													\
								do { 											\
									(gpRegLQ->_r._f01) = (_v01);				\
									(gpRegLQ->_r._f02) = (_v02);				\
									(gpRegLQ->_r._f03) = (_v03);				\
									(gpRegLQ->_r._f04) = (_v04);				\
									(gpRegLQ->_r._f05) = (_v05);				\
								} while(0)

#define LQ_Wr06(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06)										\
								do { 											\
									(gpRegLQ->_r._f01) = (_v01);				\
									(gpRegLQ->_r._f02) = (_v02);				\
									(gpRegLQ->_r._f03) = (_v03);				\
									(gpRegLQ->_r._f04) = (_v04);				\
									(gpRegLQ->_r._f05) = (_v05);				\
									(gpRegLQ->_r._f06) = (_v06);				\
								} while(0)

#define LQ_Wr07(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07)								\
								do { 											\
									(gpRegLQ->_r._f01) = (_v01);				\
									(gpRegLQ->_r._f02) = (_v02);				\
									(gpRegLQ->_r._f03) = (_v03);				\
									(gpRegLQ->_r._f04) = (_v04);				\
									(gpRegLQ->_r._f05) = (_v05);				\
									(gpRegLQ->_r._f06) = (_v06);				\
									(gpRegLQ->_r._f07) = (_v07);				\
								} while(0)

#define LQ_Wr08(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08)					\
								do { 											\
									(gpRegLQ->_r._f01) = (_v01);				\
									(gpRegLQ->_r._f02) = (_v02);				\
									(gpRegLQ->_r._f03) = (_v03);				\
									(gpRegLQ->_r._f04) = (_v04);				\
									(gpRegLQ->_r._f05) = (_v05);				\
									(gpRegLQ->_r._f06) = (_v06);				\
									(gpRegLQ->_r._f07) = (_v07);				\
									(gpRegLQ->_r._f08) = (_v08);				\
								} while(0)

#define LQ_Wr09(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09)													\
								do { 											\
									(gpRegLQ->_r._f01) = (_v01);				\
									(gpRegLQ->_r._f02) = (_v02);				\
									(gpRegLQ->_r._f03) = (_v03);				\
									(gpRegLQ->_r._f04) = (_v04);				\
									(gpRegLQ->_r._f05) = (_v05);				\
									(gpRegLQ->_r._f06) = (_v06);				\
									(gpRegLQ->_r._f07) = (_v07);				\
									(gpRegLQ->_r._f08) = (_v08);				\
									(gpRegLQ->_r._f09) = (_v09);				\
								} while(0)

#define LQ_Wr10(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10)										\
								do { 											\
									(gpRegLQ->_r._f01) = (_v01);				\
									(gpRegLQ->_r._f02) = (_v02);				\
									(gpRegLQ->_r._f03) = (_v03);				\
									(gpRegLQ->_r._f04) = (_v04);				\
									(gpRegLQ->_r._f05) = (_v05);				\
									(gpRegLQ->_r._f06) = (_v06);				\
									(gpRegLQ->_r._f07) = (_v07);				\
									(gpRegLQ->_r._f08) = (_v08);				\
									(gpRegLQ->_r._f09) = (_v09);				\
									(gpRegLQ->_r._f10) = (_v10);				\
								} while(0)

#define LQ_Wr11(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11)								\
								do { 											\
									(gpRegLQ->_r._f01) = (_v01);				\
									(gpRegLQ->_r._f02) = (_v02);				\
									(gpRegLQ->_r._f03) = (_v03);				\
									(gpRegLQ->_r._f04) = (_v04);				\
									(gpRegLQ->_r._f05) = (_v05);				\
									(gpRegLQ->_r._f06) = (_v06);				\
									(gpRegLQ->_r._f07) = (_v07);				\
									(gpRegLQ->_r._f08) = (_v08);				\
									(gpRegLQ->_r._f09) = (_v09);				\
									(gpRegLQ->_r._f10) = (_v10);				\
									(gpRegLQ->_r._f11) = (_v11);				\
								} while(0)

#define LQ_Wr12(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12)					\
								do { 											\
									(gpRegLQ->_r._f01) = (_v01);				\
									(gpRegLQ->_r._f02) = (_v02);				\
									(gpRegLQ->_r._f03) = (_v03);				\
									(gpRegLQ->_r._f04) = (_v04);				\
									(gpRegLQ->_r._f05) = (_v05);				\
									(gpRegLQ->_r._f06) = (_v06);				\
									(gpRegLQ->_r._f07) = (_v07);				\
									(gpRegLQ->_r._f08) = (_v08);				\
									(gpRegLQ->_r._f09) = (_v09);				\
									(gpRegLQ->_r._f10) = (_v10);				\
									(gpRegLQ->_r._f11) = (_v11);				\
									(gpRegLQ->_r._f12) = (_v12);				\
								} while(0)

#define LQ_Wr13(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13)													\
								do { 											\
									(gpRegLQ->_r._f01) = (_v01);				\
									(gpRegLQ->_r._f02) = (_v02);				\
									(gpRegLQ->_r._f03) = (_v03);				\
									(gpRegLQ->_r._f04) = (_v04);				\
									(gpRegLQ->_r._f05) = (_v05);				\
									(gpRegLQ->_r._f06) = (_v06);				\
									(gpRegLQ->_r._f07) = (_v07);				\
									(gpRegLQ->_r._f08) = (_v08);				\
									(gpRegLQ->_r._f09) = (_v09);				\
									(gpRegLQ->_r._f10) = (_v10);				\
									(gpRegLQ->_r._f11) = (_v11);				\
									(gpRegLQ->_r._f12) = (_v12);				\
									(gpRegLQ->_r._f13) = (_v13);				\
								} while(0)

#define LQ_Wr14(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14)										\
								do { 											\
									(gpRegLQ->_r._f01) = (_v01);				\
									(gpRegLQ->_r._f02) = (_v02);				\
									(gpRegLQ->_r._f03) = (_v03);				\
									(gpRegLQ->_r._f04) = (_v04);				\
									(gpRegLQ->_r._f05) = (_v05);				\
									(gpRegLQ->_r._f06) = (_v06);				\
									(gpRegLQ->_r._f07) = (_v07);				\
									(gpRegLQ->_r._f08) = (_v08);				\
									(gpRegLQ->_r._f09) = (_v09);				\
									(gpRegLQ->_r._f10) = (_v10);				\
									(gpRegLQ->_r._f11) = (_v11);				\
									(gpRegLQ->_r._f12) = (_v12);				\
									(gpRegLQ->_r._f13) = (_v13);				\
									(gpRegLQ->_r._f14) = (_v14);				\
								} while(0)

#define LQ_Wr15(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15)								\
								do { 											\
									(gpRegLQ->_r._f01) = (_v01);				\
									(gpRegLQ->_r._f02) = (_v02);				\
									(gpRegLQ->_r._f03) = (_v03);				\
									(gpRegLQ->_r._f04) = (_v04);				\
									(gpRegLQ->_r._f05) = (_v05);				\
									(gpRegLQ->_r._f06) = (_v06);				\
									(gpRegLQ->_r._f07) = (_v07);				\
									(gpRegLQ->_r._f08) = (_v08);				\
									(gpRegLQ->_r._f09) = (_v09);				\
									(gpRegLQ->_r._f10) = (_v10);				\
									(gpRegLQ->_r._f11) = (_v11);				\
									(gpRegLQ->_r._f12) = (_v12);				\
									(gpRegLQ->_r._f13) = (_v13);				\
									(gpRegLQ->_r._f14) = (_v14);				\
									(gpRegLQ->_r._f15) = (_v15);				\
								} while(0)

#define LQ_Wr16(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15,_f16,_v16)					\
								do { 											\
									(gpRegLQ->_r._f01) = (_v01);				\
									(gpRegLQ->_r._f02) = (_v02);				\
									(gpRegLQ->_r._f03) = (_v03);				\
									(gpRegLQ->_r._f04) = (_v04);				\
									(gpRegLQ->_r._f05) = (_v05);				\
									(gpRegLQ->_r._f06) = (_v06);				\
									(gpRegLQ->_r._f07) = (_v07);				\
									(gpRegLQ->_r._f08) = (_v08);				\
									(gpRegLQ->_r._f09) = (_v09);				\
									(gpRegLQ->_r._f10) = (_v10);				\
									(gpRegLQ->_r._f11) = (_v11);				\
									(gpRegLQ->_r._f12) = (_v12);				\
									(gpRegLQ->_r._f13) = (_v13);				\
									(gpRegLQ->_r._f14) = (_v14);				\
									(gpRegLQ->_r._f15) = (_v15);				\
									(gpRegLQ->_r._f16) = (_v16);				\
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
 * _rwname: name of rw    bit field : shall be 0 after LQ_Wind(), 1 for LQ_Rind()
 * _iname : name of index bit field
 * _ival  : index value
 * _fname : field name
 * _fval  : field variable that field value shall be stored.
 *
 * LQ_Rind : General indexed register Read.(
 * LQ_Wind : General indexed register Read.
 *
 * LQ_Ridx : For 'index', 'rw', 'load' field name
 * LQ_Widx : For 'index', 'rw', 'load' field name and NO_LOAD.
 */
#define LQ_Rind(_r, _lname, _rwname, _iname, _ival, _fname, _fval)				\
							do {												\
								LQ_Wr03(_r,_lname,0,_rwname,1,_iname,_ival);	\
								LQ_WrFL(_r);									\
								LQ_RdFL(_r);									\
								LQ_Rd01(_r,_fname,_fval);						\
							} while (0)

#define LQ_Wind(_r, _lname, _rwname, _iname, _ival, _fname, _fval)				\
				LQ_Wr04(_r, _lname,0, _rwname,0, _iname,_ival, _fname,_fval)


#define LQ_Ridx(_r, _ival, _fname, _fval)	LQ_Rind(_r,load,rw,index,_ival,_fname,_fval)

#define LQ_Widx(_r, _ival, _fname, _fval)	LQ_Wind(_r,load,rw,index,_ival,_fname,_fval)

/** @} *//* end of macro documentation */

#define gpRealRegLQC		stpLQC_Reg
#define gpRegLQC			stpLQC_RegShadow

#define gpRealRegLQ		stpLQ_Reg
#define gpRegLQ			stpLQ_RegShadow

#ifdef __cplusplus
}
#endif

#endif	/* _LQ_REG_H_ */

/* from 'VDEC_LQ.csv' 20110322 14:04:24   대한민국 표준시 by getregs v2.7 */
