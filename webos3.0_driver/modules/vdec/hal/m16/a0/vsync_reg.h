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
 *  VSYNC register details. ( used only within kdriver )
 *
 *  author     user name (user_name@lge.com)
 *  version    1.0
 *  date       2010.xx.xx
 *
 */

#ifndef _VSYNC_REG_H_
#define _VSYNC_REG_H_

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
	0x0000 sync_conf ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_first_vsync0_pulse_en       : 1,	//     0
	reg_first_vsync1_pulse_en       : 1,	//     1
	reg_first_vsync2_pulse_en       : 1,	//     2
	reg_first_vsync3_pulse_en       : 1,	//     3
	reg_first_vsync4_pulse_en       : 1,	//     4
	reg_first_vsync5_pulse_en       : 1,	//     5
	                                : 2,	//  6: 7 reserved
	reg_vsync0_update_mode          : 1,	//     8
	reg_vsync1_update_mode          : 1,	//     9
	reg_vsync2_update_mode          : 1,	//    10
	reg_vsync3_update_mode          : 1,	//    11
	reg_vsync4_update_mode          : 1,	//    12
	reg_vsync5_update_mode          : 1,	//    13
	                                : 2,	// 14:15 reserved
	reg_vsync0_srst                 : 1,	//    16
	reg_vsync1_srst                 : 1,	//    17
	reg_vsync2_srst                 : 1,	//    18
	reg_vsync3_srst                 : 1,	//    19
	reg_vsync4_srst                 : 1,	//    20
	reg_vsync5_srst                 : 1,	//    21
	                                : 2,	// 22:23 reserved
	reg_vsync0_en                   : 1,	//    24
	reg_vsync1_en                   : 1,	//    25
	reg_vsync2_en                   : 1,	//    26
	reg_vsync3_en                   : 1,	//    27
	reg_vsync4_en                   : 1,	//    28
	reg_vsync5_en                   : 1;	//    29
} SYNC_CONF;

#define VSYNC_EN_BIT		24

/*-----------------------------------------------------------------------------
	0x0004 sync_sel ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_vsync_ch0_sel               : 4,	//  0: 3
	reg_vsync_ch1_sel               : 4,	//  4: 7
	reg_vsync_ch2_sel               : 4,	//  8:11
	reg_vsync_ch3_sel               : 4,	// 12:15
	reg_vsync_ch4_sel               : 4,	// 16:19
	reg_vsync_ch5_sel               : 4,	// 20:23
	reg_vsync0_field_sel            : 1,	//    24
	reg_vsync1_field_sel            : 1,	//    25
	reg_vsync2_field_sel            : 1,	//    26
	reg_vsync3_field_sel            : 1,	//    27
	reg_vsync4_field_sel            : 1,	//    28
	reg_vsync5_field_sel            : 1;	//    29
} SYNC_SEL;
#define VSYNC_CH_SEL_BIT		0
#define VSYNC_FIELD_SEL_BIT		24

/*-----------------------------------------------------------------------------
	0x0008 sync_update ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_vsync0_update               : 1,	//     0
	reg_vsync1_update               : 1,	//     1
	reg_vsync2_update               : 1,	//     2
	reg_vsync3_update               : 1,	//     3
	reg_vsync4_update               : 1,	//     4
	reg_vsync5_update               : 1;	//     5
} SYNC_UPDATE;

/*-----------------------------------------------------------------------------
	0x0010~0x003c vsync0_field_num ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32	reg_field0_num           ;   	// 31: 0
	UINT32	reg_field1_num           ;   	// 31: 0
} VSYNC_FIELD_NUM;

/*-----------------------------------------------------------------------------
	0x0010 vsync0_field0_num ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_vsync0_field0_num           ;   	// 31: 0
} VSYNC0_FIELD0_NUM;

/*-----------------------------------------------------------------------------
	0x0014 vsync0_field1_num ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_vsync0_field1_num           ;   	// 31: 0
} VSYNC0_FIELD1_NUM;

/*-----------------------------------------------------------------------------
	0x0018 vsync1_field0_num ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_vsync1_field0_num           ;   	// 31: 0
} VSYNC1_FIELD0_NUM;

/*-----------------------------------------------------------------------------
	0x001c vsync1_field1_num ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_vsync1_field1_num           ;   	// 31: 0
} VSYNC1_FIELD1_NUM;

/*-----------------------------------------------------------------------------
	0x0020 vsync2_field0_num ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_vsync2_field0_num           ;   	// 31: 0
} VSYNC2_FIELD0_NUM;

/*-----------------------------------------------------------------------------
	0x0024 vsync2_field1_num ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_vsync2_field1_num           ;   	// 31: 0
} VSYNC2_FIELD1_NUM;

/*-----------------------------------------------------------------------------
	0x0028 vsync3_field0_num ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_vsync3_field0_num           ;   	// 31: 0
} VSYNC3_FIELD0_NUM;

/*-----------------------------------------------------------------------------
	0x002c vsync3_field1_num ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_vsync3_field1_num           ;   	// 31: 0
} VSYNC3_FIELD1_NUM;

/*-----------------------------------------------------------------------------
	0x0030 vsync4_field0_num ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_vsync4_field0_num           : 1;	//     0
} VSYNC4_FIELD0_NUM;

/*-----------------------------------------------------------------------------
	0x0034 vsync4_field1_num ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_vsync4_field1_num           : 1;	//     0
} VSYNC4_FIELD1_NUM;

/*-----------------------------------------------------------------------------
	0x0038 vsync5_field0_num ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_vsync5_field0_num           : 1;	//     0
} VSYNC5_FIELD0_NUM;

/*-----------------------------------------------------------------------------
	0x003c vsync5_field1_num ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_vsync5_field1_num           : 1;	//     0
} VSYNC5_FIELD1_NUM;

/*-----------------------------------------------------------------------------
	0x0040~0x0058 vsync_int_field ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_int_vsync_field         : 1;   	//     0
} VSYNC_INT_FIELD;

/*-----------------------------------------------------------------------------
	0x0060 timer_conf ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_timer0_update_mode         : 1,	//     0
	reg_timer1_update_mode         : 1,	//     1
	reg_timer2_update_mode         : 1,	//     2
	reg_timer3_update_mode         : 1,	//     3
	                                :20,	//  4:23 reserved
	reg_timer0_srst                 : 1,	//    24
	reg_timer1_srst                 : 1,	//    25
	reg_timer2_srst                 : 1,	//    26
	reg_timer3_srst                 : 1,	//    27
	reg_timer0_en                   : 1,	//    28
	reg_timer1_en                   : 1,	//    29
	reg_timer2_en                   : 1,	//    30
	reg_timer3_en                   : 1;	//    31
} TIMER_CONF;

/*-----------------------------------------------------------------------------
	0x0064 timer_update ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_timer0_update               : 1,	//     0
	reg_timer1_update               : 1,	//     1
	reg_timer2_update               : 1,	//     2
	reg_timer3_update               : 1;	//     3
} TIMER_UPDATE;

/*-----------------------------------------------------------------------------
	0x0070 timer0_num ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_timer0_num                  : 1;	//     0
} TIMER0_NUM;

/*-----------------------------------------------------------------------------
	0x0074 timer1_num ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_timer1_num                  : 1;	//     0
} TIMER1_NUM;

/*-----------------------------------------------------------------------------
	0x0078 timer2_num ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_timer2_num                  : 1;	//     0
} TIMER2_NUM;

/*-----------------------------------------------------------------------------
	0x007c timer3_num ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_timer3_num                  : 1;	//     0
} TIMER3_NUM;

/*-----------------------------------------------------------------------------
	0x0080 vsync0_cnt ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_vsync0_cnt                  :16;	//  0:15
} VSYNC0_CNT;

/*-----------------------------------------------------------------------------
	0x0084 vsync1_cnt ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_vsync1_cnt                  :16;	//  0:15
} VSYNC1_CNT;

/*-----------------------------------------------------------------------------
	0x0088 vsync2_cnt ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_vsync2_cnt                  :16;	//  0:15
} VSYNC2_CNT;

/*-----------------------------------------------------------------------------
	0x0090 vsync3_cnt ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_vsync3_cnt                  :16;	//  0:15
} VSYNC3_CNT;

/*-----------------------------------------------------------------------------
	0x0094 vsync4_cnt ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_vsync4_cnt                  :16;	//  0:15
} VSYNC4_CNT;

/*-----------------------------------------------------------------------------
	0x0098 vsync5_cnt ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_vsync5_cnt                  :16;	//  0:15
} VSYNC5_CNT;

/*-----------------------------------------------------------------------------
	0x00fc version ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	yyyymmdd                        ;   	// 31: 0
} VERSION;

typedef struct {
	UINT32                          	                 __rsvd_00[  56];	// 0x0a00 ~ 0x0ad9
	UINT32                          	reg_name                        ;	// 0x0add : ''
} CATEGORY_REG_T;
/* 1 regs, 1 types */

typedef struct {
	SYNC_CONF                       	sync_conf                       ;	// 0x0000 : ''
	SYNC_SEL                        	sync_sel                        ;	// 0x0004 : ''
	SYNC_UPDATE                     	sync_update                     ;	// 0x0008 : ''
	UINT32                          	                 __rsvd_00[   1];	// 0x000c
	VSYNC_FIELD_NUM						vsync_field_num[6]				;	// 0x0010:0x003c
	VSYNC_INT_FIELD       	         	vsync0_int_field                ;	// 0x0040 : ''
	VSYNC_INT_FIELD         	       	vsync1_int_field                ;	// 0x0044 : ''
	VSYNC_INT_FIELD             	   	vsync2_int_field                ;	// 0x0048 : ''
	UINT32                          	                 __rsvd_01[   1];	// 0x004c
	VSYNC_INT_FIELD                		vsync3_int_field                ;	// 0x0050 : ''
	VSYNC_INT_FIELD               	 	vsync4_int_field                ;	// 0x0054 : ''
	VSYNC_INT_FIELD                		vsync5_int_field                ;	// 0x0058 : ''
	UINT32                          	                 __rsvd_02[   1];	// 0x005c
	TIMER_CONF                      	timer_conf                      ;	// 0x0060 : ''
	TIMER_UPDATE                    	timer_update                    ;	// 0x0064 : ''
	UINT32                          	                 __rsvd_03[   2];	// 0x0068 ~ 0x006c
	TIMER0_NUM                      	timer0_num                      ;	// 0x0070 : ''
	TIMER1_NUM                      	timer1_num                      ;	// 0x0074 : ''
	TIMER2_NUM                      	timer2_num                      ;	// 0x0078 : ''
	TIMER3_NUM                      	timer3_num                      ;	// 0x007c : ''
	VSYNC0_CNT                      	vsync0_cnt                      ;	// 0x0080 : ''
	VSYNC1_CNT                      	vsync1_cnt                      ;	// 0x0084 : ''
	VSYNC2_CNT                      	vsync2_cnt                      ;	// 0x0088 : ''
	UINT32                          	                 __rsvd_04[   1];	// 0x008c
	VSYNC3_CNT                      	vsync3_cnt                      ;	// 0x0090 : ''
	VSYNC4_CNT                      	vsync4_cnt                      ;	// 0x0094 : ''
	VSYNC5_CNT                      	vsync5_cnt                      ;	// 0x0098 : ''
	UINT32                          	                 __rsvd_05[  24];	// 0x009c ~ 0x00f8
	UINT32 	                        	version                         ;	// 0x00fc : ''
} Sync_REG_T;
/* 34 regs, 34 types */

/* 35 regs, 35 types in Total*/

/*
 * @{
 * Naming for register pointer.
 * gpRealRegCATEGORY : real register of CATEGORY.
 * gpRegCATEGORY     : shadow register.
 *
 * @def CATEGORY_RdFL: Read  FLushing : Shadow <- Real.
 * @def CATEGORY_WrFL: Write FLushing : Shadow -> Real.
 * @def CATEGORY_Rd  : Read  whole register(UINT32) from Shadow register.
 * @def CATEGORY_Wr  : Write whole register(UINT32) from Shadow register.
 * @def CATEGORY_Rd01 ~ CATEGORY_Rdnn: Read  given '01~nn' fields from Shadow register.
 * @def CATEGORY_Wr01 ~ CATEGORY_Wrnn: Write given '01~nn' fields to   Shadow register.
 * */
#define CATEGORY_RdFL(_r)			((gpRegCATEGORY->_r)=(gpRealRegCATEGORY->_r))
#define CATEGORY_WrFL(_r)			((gpRealRegCATEGORY->_r)=(gpRegCATEGORY->_r))

#define CATEGORY_Rd(_r)			*((UINT32*)(&(gpRegCATEGORY->_r)))
#define CATEGORY_Wr(_r,_v)			((CATEGORY_Rd(_r))=((UINT32)(_v)))

#define CATEGORY_Rd01(_r,_f01,_v01)													\
								do { 											\
									(_v01) = (gpRegCATEGORY->_r._f01);				\
								} while(0)

#define CATEGORY_Rd02(_r,_f01,_v01,_f02,_v02)										\
								do { 											\
									(_v01) = (gpRegCATEGORY->_r._f01);				\
									(_v02) = (gpRegCATEGORY->_r._f02);				\
								} while(0)

#define CATEGORY_Rd03(_r,_f01,_v01,_f02,_v02,_f03,_v03)								\
								do { 											\
									(_v01) = (gpRegCATEGORY->_r._f01);				\
									(_v02) = (gpRegCATEGORY->_r._f02);				\
									(_v03) = (gpRegCATEGORY->_r._f03);				\
								} while(0)

#define CATEGORY_Rd04(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04)					\
								do { 											\
									(_v01) = (gpRegCATEGORY->_r._f01);				\
									(_v02) = (gpRegCATEGORY->_r._f02);				\
									(_v03) = (gpRegCATEGORY->_r._f03);				\
									(_v04) = (gpRegCATEGORY->_r._f04);				\
								} while(0)

#define CATEGORY_Rd05(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05)													\
								do { 											\
									(_v01) = (gpRegCATEGORY->_r._f01);				\
									(_v02) = (gpRegCATEGORY->_r._f02);				\
									(_v03) = (gpRegCATEGORY->_r._f03);				\
									(_v04) = (gpRegCATEGORY->_r._f04);				\
									(_v05) = (gpRegCATEGORY->_r._f05);				\
								} while(0)

#define CATEGORY_Rd06(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06)										\
								do { 											\
									(_v01) = (gpRegCATEGORY->_r._f01);				\
									(_v02) = (gpRegCATEGORY->_r._f02);				\
									(_v03) = (gpRegCATEGORY->_r._f03);				\
									(_v04) = (gpRegCATEGORY->_r._f04);				\
									(_v05) = (gpRegCATEGORY->_r._f05);				\
									(_v06) = (gpRegCATEGORY->_r._f06);				\
								} while(0)

#define CATEGORY_Rd07(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07)								\
								do { 											\
									(_v01) = (gpRegCATEGORY->_r._f01);				\
									(_v02) = (gpRegCATEGORY->_r._f02);				\
									(_v03) = (gpRegCATEGORY->_r._f03);				\
									(_v04) = (gpRegCATEGORY->_r._f04);				\
									(_v05) = (gpRegCATEGORY->_r._f05);				\
									(_v06) = (gpRegCATEGORY->_r._f06);				\
									(_v07) = (gpRegCATEGORY->_r._f07);				\
								} while(0)

#define CATEGORY_Rd08(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08)					\
								do { 											\
									(_v01) = (gpRegCATEGORY->_r._f01);				\
									(_v02) = (gpRegCATEGORY->_r._f02);				\
									(_v03) = (gpRegCATEGORY->_r._f03);				\
									(_v04) = (gpRegCATEGORY->_r._f04);				\
									(_v05) = (gpRegCATEGORY->_r._f05);				\
									(_v06) = (gpRegCATEGORY->_r._f06);				\
									(_v07) = (gpRegCATEGORY->_r._f07);				\
									(_v08) = (gpRegCATEGORY->_r._f08);				\
								} while(0)

#define CATEGORY_Rd09(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09)													\
								do { 											\
									(_v01) = (gpRegCATEGORY->_r._f01);				\
									(_v02) = (gpRegCATEGORY->_r._f02);				\
									(_v03) = (gpRegCATEGORY->_r._f03);				\
									(_v04) = (gpRegCATEGORY->_r._f04);				\
									(_v05) = (gpRegCATEGORY->_r._f05);				\
									(_v06) = (gpRegCATEGORY->_r._f06);				\
									(_v07) = (gpRegCATEGORY->_r._f07);				\
									(_v08) = (gpRegCATEGORY->_r._f08);				\
									(_v09) = (gpRegCATEGORY->_r._f09);				\
								} while(0)

#define CATEGORY_Rd10(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10)										\
								do { 											\
									(_v01) = (gpRegCATEGORY->_r._f01);				\
									(_v02) = (gpRegCATEGORY->_r._f02);				\
									(_v03) = (gpRegCATEGORY->_r._f03);				\
									(_v04) = (gpRegCATEGORY->_r._f04);				\
									(_v05) = (gpRegCATEGORY->_r._f05);				\
									(_v06) = (gpRegCATEGORY->_r._f06);				\
									(_v07) = (gpRegCATEGORY->_r._f07);				\
									(_v08) = (gpRegCATEGORY->_r._f08);				\
									(_v09) = (gpRegCATEGORY->_r._f09);				\
									(_v10) = (gpRegCATEGORY->_r._f10);				\
								} while(0)

#define CATEGORY_Rd11(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11)								\
								do { 											\
									(_v01) = (gpRegCATEGORY->_r._f01);				\
									(_v02) = (gpRegCATEGORY->_r._f02);				\
									(_v03) = (gpRegCATEGORY->_r._f03);				\
									(_v04) = (gpRegCATEGORY->_r._f04);				\
									(_v05) = (gpRegCATEGORY->_r._f05);				\
									(_v06) = (gpRegCATEGORY->_r._f06);				\
									(_v07) = (gpRegCATEGORY->_r._f07);				\
									(_v08) = (gpRegCATEGORY->_r._f08);				\
									(_v09) = (gpRegCATEGORY->_r._f09);				\
									(_v10) = (gpRegCATEGORY->_r._f10);				\
									(_v11) = (gpRegCATEGORY->_r._f11);				\
								} while(0)

#define CATEGORY_Rd12(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12)					\
								do { 											\
									(_v01) = (gpRegCATEGORY->_r._f01);				\
									(_v02) = (gpRegCATEGORY->_r._f02);				\
									(_v03) = (gpRegCATEGORY->_r._f03);				\
									(_v04) = (gpRegCATEGORY->_r._f04);				\
									(_v05) = (gpRegCATEGORY->_r._f05);				\
									(_v06) = (gpRegCATEGORY->_r._f06);				\
									(_v07) = (gpRegCATEGORY->_r._f07);				\
									(_v08) = (gpRegCATEGORY->_r._f08);				\
									(_v09) = (gpRegCATEGORY->_r._f09);				\
									(_v10) = (gpRegCATEGORY->_r._f10);				\
									(_v11) = (gpRegCATEGORY->_r._f11);				\
									(_v12) = (gpRegCATEGORY->_r._f12);				\
								} while(0)

#define CATEGORY_Rd13(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13)													\
								do { 											\
									(_v01) = (gpRegCATEGORY->_r._f01);				\
									(_v02) = (gpRegCATEGORY->_r._f02);				\
									(_v03) = (gpRegCATEGORY->_r._f03);				\
									(_v04) = (gpRegCATEGORY->_r._f04);				\
									(_v05) = (gpRegCATEGORY->_r._f05);				\
									(_v06) = (gpRegCATEGORY->_r._f06);				\
									(_v07) = (gpRegCATEGORY->_r._f07);				\
									(_v08) = (gpRegCATEGORY->_r._f08);				\
									(_v09) = (gpRegCATEGORY->_r._f09);				\
									(_v10) = (gpRegCATEGORY->_r._f10);				\
									(_v11) = (gpRegCATEGORY->_r._f11);				\
									(_v12) = (gpRegCATEGORY->_r._f12);				\
									(_v13) = (gpRegCATEGORY->_r._f13);				\
								} while(0)

#define CATEGORY_Rd14(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14)										\
								do { 											\
									(_v01) = (gpRegCATEGORY->_r._f01);				\
									(_v02) = (gpRegCATEGORY->_r._f02);				\
									(_v03) = (gpRegCATEGORY->_r._f03);				\
									(_v04) = (gpRegCATEGORY->_r._f04);				\
									(_v05) = (gpRegCATEGORY->_r._f05);				\
									(_v06) = (gpRegCATEGORY->_r._f06);				\
									(_v07) = (gpRegCATEGORY->_r._f07);				\
									(_v08) = (gpRegCATEGORY->_r._f08);				\
									(_v09) = (gpRegCATEGORY->_r._f09);				\
									(_v10) = (gpRegCATEGORY->_r._f10);				\
									(_v11) = (gpRegCATEGORY->_r._f11);				\
									(_v12) = (gpRegCATEGORY->_r._f12);				\
									(_v13) = (gpRegCATEGORY->_r._f13);				\
									(_v14) = (gpRegCATEGORY->_r._f14);				\
								} while(0)

#define CATEGORY_Rd15(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15)								\
								do { 											\
									(_v01) = (gpRegCATEGORY->_r._f01);				\
									(_v02) = (gpRegCATEGORY->_r._f02);				\
									(_v03) = (gpRegCATEGORY->_r._f03);				\
									(_v04) = (gpRegCATEGORY->_r._f04);				\
									(_v05) = (gpRegCATEGORY->_r._f05);				\
									(_v06) = (gpRegCATEGORY->_r._f06);				\
									(_v07) = (gpRegCATEGORY->_r._f07);				\
									(_v08) = (gpRegCATEGORY->_r._f08);				\
									(_v09) = (gpRegCATEGORY->_r._f09);				\
									(_v10) = (gpRegCATEGORY->_r._f10);				\
									(_v11) = (gpRegCATEGORY->_r._f11);				\
									(_v12) = (gpRegCATEGORY->_r._f12);				\
									(_v13) = (gpRegCATEGORY->_r._f13);				\
									(_v14) = (gpRegCATEGORY->_r._f14);				\
									(_v15) = (gpRegCATEGORY->_r._f15);				\
								} while(0)

#define CATEGORY_Rd16(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15,_f16,_v16)					\
								do { 											\
									(_v01) = (gpRegCATEGORY->_r._f01);				\
									(_v02) = (gpRegCATEGORY->_r._f02);				\
									(_v03) = (gpRegCATEGORY->_r._f03);				\
									(_v04) = (gpRegCATEGORY->_r._f04);				\
									(_v05) = (gpRegCATEGORY->_r._f05);				\
									(_v06) = (gpRegCATEGORY->_r._f06);				\
									(_v07) = (gpRegCATEGORY->_r._f07);				\
									(_v08) = (gpRegCATEGORY->_r._f08);				\
									(_v09) = (gpRegCATEGORY->_r._f09);				\
									(_v10) = (gpRegCATEGORY->_r._f10);				\
									(_v11) = (gpRegCATEGORY->_r._f11);				\
									(_v12) = (gpRegCATEGORY->_r._f12);				\
									(_v13) = (gpRegCATEGORY->_r._f13);				\
									(_v14) = (gpRegCATEGORY->_r._f14);				\
									(_v15) = (gpRegCATEGORY->_r._f15);				\
									(_v16) = (gpRegCATEGORY->_r._f16);				\
								} while(0)


#define CATEGORY_Wr01(_r,_f01,_v01)													\
								do { 											\
									(gpRegCATEGORY->_r._f01) = (_v01);				\
								} while(0)

#define CATEGORY_Wr02(_r,_f01,_v01,_f02,_v02)										\
								do { 											\
									(gpRegCATEGORY->_r._f01) = (_v01);				\
									(gpRegCATEGORY->_r._f02) = (_v02);				\
								} while(0)

#define CATEGORY_Wr03(_r,_f01,_v01,_f02,_v02,_f03,_v03)								\
								do { 											\
									(gpRegCATEGORY->_r._f01) = (_v01);				\
									(gpRegCATEGORY->_r._f02) = (_v02);				\
									(gpRegCATEGORY->_r._f03) = (_v03);				\
								} while(0)

#define CATEGORY_Wr04(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04)					\
								do { 											\
									(gpRegCATEGORY->_r._f01) = (_v01);				\
									(gpRegCATEGORY->_r._f02) = (_v02);				\
									(gpRegCATEGORY->_r._f03) = (_v03);				\
									(gpRegCATEGORY->_r._f04) = (_v04);				\
								} while(0)

#define CATEGORY_Wr05(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05)													\
								do { 											\
									(gpRegCATEGORY->_r._f01) = (_v01);				\
									(gpRegCATEGORY->_r._f02) = (_v02);				\
									(gpRegCATEGORY->_r._f03) = (_v03);				\
									(gpRegCATEGORY->_r._f04) = (_v04);				\
									(gpRegCATEGORY->_r._f05) = (_v05);				\
								} while(0)

#define CATEGORY_Wr06(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06)										\
								do { 											\
									(gpRegCATEGORY->_r._f01) = (_v01);				\
									(gpRegCATEGORY->_r._f02) = (_v02);				\
									(gpRegCATEGORY->_r._f03) = (_v03);				\
									(gpRegCATEGORY->_r._f04) = (_v04);				\
									(gpRegCATEGORY->_r._f05) = (_v05);				\
									(gpRegCATEGORY->_r._f06) = (_v06);				\
								} while(0)

#define CATEGORY_Wr07(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07)								\
								do { 											\
									(gpRegCATEGORY->_r._f01) = (_v01);				\
									(gpRegCATEGORY->_r._f02) = (_v02);				\
									(gpRegCATEGORY->_r._f03) = (_v03);				\
									(gpRegCATEGORY->_r._f04) = (_v04);				\
									(gpRegCATEGORY->_r._f05) = (_v05);				\
									(gpRegCATEGORY->_r._f06) = (_v06);				\
									(gpRegCATEGORY->_r._f07) = (_v07);				\
								} while(0)

#define CATEGORY_Wr08(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08)					\
								do { 											\
									(gpRegCATEGORY->_r._f01) = (_v01);				\
									(gpRegCATEGORY->_r._f02) = (_v02);				\
									(gpRegCATEGORY->_r._f03) = (_v03);				\
									(gpRegCATEGORY->_r._f04) = (_v04);				\
									(gpRegCATEGORY->_r._f05) = (_v05);				\
									(gpRegCATEGORY->_r._f06) = (_v06);				\
									(gpRegCATEGORY->_r._f07) = (_v07);				\
									(gpRegCATEGORY->_r._f08) = (_v08);				\
								} while(0)

#define CATEGORY_Wr09(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09)													\
								do { 											\
									(gpRegCATEGORY->_r._f01) = (_v01);				\
									(gpRegCATEGORY->_r._f02) = (_v02);				\
									(gpRegCATEGORY->_r._f03) = (_v03);				\
									(gpRegCATEGORY->_r._f04) = (_v04);				\
									(gpRegCATEGORY->_r._f05) = (_v05);				\
									(gpRegCATEGORY->_r._f06) = (_v06);				\
									(gpRegCATEGORY->_r._f07) = (_v07);				\
									(gpRegCATEGORY->_r._f08) = (_v08);				\
									(gpRegCATEGORY->_r._f09) = (_v09);				\
								} while(0)

#define CATEGORY_Wr10(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10)										\
								do { 											\
									(gpRegCATEGORY->_r._f01) = (_v01);				\
									(gpRegCATEGORY->_r._f02) = (_v02);				\
									(gpRegCATEGORY->_r._f03) = (_v03);				\
									(gpRegCATEGORY->_r._f04) = (_v04);				\
									(gpRegCATEGORY->_r._f05) = (_v05);				\
									(gpRegCATEGORY->_r._f06) = (_v06);				\
									(gpRegCATEGORY->_r._f07) = (_v07);				\
									(gpRegCATEGORY->_r._f08) = (_v08);				\
									(gpRegCATEGORY->_r._f09) = (_v09);				\
									(gpRegCATEGORY->_r._f10) = (_v10);				\
								} while(0)

#define CATEGORY_Wr11(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11)								\
								do { 											\
									(gpRegCATEGORY->_r._f01) = (_v01);				\
									(gpRegCATEGORY->_r._f02) = (_v02);				\
									(gpRegCATEGORY->_r._f03) = (_v03);				\
									(gpRegCATEGORY->_r._f04) = (_v04);				\
									(gpRegCATEGORY->_r._f05) = (_v05);				\
									(gpRegCATEGORY->_r._f06) = (_v06);				\
									(gpRegCATEGORY->_r._f07) = (_v07);				\
									(gpRegCATEGORY->_r._f08) = (_v08);				\
									(gpRegCATEGORY->_r._f09) = (_v09);				\
									(gpRegCATEGORY->_r._f10) = (_v10);				\
									(gpRegCATEGORY->_r._f11) = (_v11);				\
								} while(0)

#define CATEGORY_Wr12(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12)					\
								do { 											\
									(gpRegCATEGORY->_r._f01) = (_v01);				\
									(gpRegCATEGORY->_r._f02) = (_v02);				\
									(gpRegCATEGORY->_r._f03) = (_v03);				\
									(gpRegCATEGORY->_r._f04) = (_v04);				\
									(gpRegCATEGORY->_r._f05) = (_v05);				\
									(gpRegCATEGORY->_r._f06) = (_v06);				\
									(gpRegCATEGORY->_r._f07) = (_v07);				\
									(gpRegCATEGORY->_r._f08) = (_v08);				\
									(gpRegCATEGORY->_r._f09) = (_v09);				\
									(gpRegCATEGORY->_r._f10) = (_v10);				\
									(gpRegCATEGORY->_r._f11) = (_v11);				\
									(gpRegCATEGORY->_r._f12) = (_v12);				\
								} while(0)

#define CATEGORY_Wr13(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13)													\
								do { 											\
									(gpRegCATEGORY->_r._f01) = (_v01);				\
									(gpRegCATEGORY->_r._f02) = (_v02);				\
									(gpRegCATEGORY->_r._f03) = (_v03);				\
									(gpRegCATEGORY->_r._f04) = (_v04);				\
									(gpRegCATEGORY->_r._f05) = (_v05);				\
									(gpRegCATEGORY->_r._f06) = (_v06);				\
									(gpRegCATEGORY->_r._f07) = (_v07);				\
									(gpRegCATEGORY->_r._f08) = (_v08);				\
									(gpRegCATEGORY->_r._f09) = (_v09);				\
									(gpRegCATEGORY->_r._f10) = (_v10);				\
									(gpRegCATEGORY->_r._f11) = (_v11);				\
									(gpRegCATEGORY->_r._f12) = (_v12);				\
									(gpRegCATEGORY->_r._f13) = (_v13);				\
								} while(0)

#define CATEGORY_Wr14(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14)										\
								do { 											\
									(gpRegCATEGORY->_r._f01) = (_v01);				\
									(gpRegCATEGORY->_r._f02) = (_v02);				\
									(gpRegCATEGORY->_r._f03) = (_v03);				\
									(gpRegCATEGORY->_r._f04) = (_v04);				\
									(gpRegCATEGORY->_r._f05) = (_v05);				\
									(gpRegCATEGORY->_r._f06) = (_v06);				\
									(gpRegCATEGORY->_r._f07) = (_v07);				\
									(gpRegCATEGORY->_r._f08) = (_v08);				\
									(gpRegCATEGORY->_r._f09) = (_v09);				\
									(gpRegCATEGORY->_r._f10) = (_v10);				\
									(gpRegCATEGORY->_r._f11) = (_v11);				\
									(gpRegCATEGORY->_r._f12) = (_v12);				\
									(gpRegCATEGORY->_r._f13) = (_v13);				\
									(gpRegCATEGORY->_r._f14) = (_v14);				\
								} while(0)

#define CATEGORY_Wr15(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15)								\
								do { 											\
									(gpRegCATEGORY->_r._f01) = (_v01);				\
									(gpRegCATEGORY->_r._f02) = (_v02);				\
									(gpRegCATEGORY->_r._f03) = (_v03);				\
									(gpRegCATEGORY->_r._f04) = (_v04);				\
									(gpRegCATEGORY->_r._f05) = (_v05);				\
									(gpRegCATEGORY->_r._f06) = (_v06);				\
									(gpRegCATEGORY->_r._f07) = (_v07);				\
									(gpRegCATEGORY->_r._f08) = (_v08);				\
									(gpRegCATEGORY->_r._f09) = (_v09);				\
									(gpRegCATEGORY->_r._f10) = (_v10);				\
									(gpRegCATEGORY->_r._f11) = (_v11);				\
									(gpRegCATEGORY->_r._f12) = (_v12);				\
									(gpRegCATEGORY->_r._f13) = (_v13);				\
									(gpRegCATEGORY->_r._f14) = (_v14);				\
									(gpRegCATEGORY->_r._f15) = (_v15);				\
								} while(0)

#define CATEGORY_Wr16(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15,_f16,_v16)					\
								do { 											\
									(gpRegCATEGORY->_r._f01) = (_v01);				\
									(gpRegCATEGORY->_r._f02) = (_v02);				\
									(gpRegCATEGORY->_r._f03) = (_v03);				\
									(gpRegCATEGORY->_r._f04) = (_v04);				\
									(gpRegCATEGORY->_r._f05) = (_v05);				\
									(gpRegCATEGORY->_r._f06) = (_v06);				\
									(gpRegCATEGORY->_r._f07) = (_v07);				\
									(gpRegCATEGORY->_r._f08) = (_v08);				\
									(gpRegCATEGORY->_r._f09) = (_v09);				\
									(gpRegCATEGORY->_r._f10) = (_v10);				\
									(gpRegCATEGORY->_r._f11) = (_v11);				\
									(gpRegCATEGORY->_r._f12) = (_v12);				\
									(gpRegCATEGORY->_r._f13) = (_v13);				\
									(gpRegCATEGORY->_r._f14) = (_v14);				\
									(gpRegCATEGORY->_r._f15) = (_v15);				\
									(gpRegCATEGORY->_r._f16) = (_v16);				\
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
 * _rwname: name of rw    bit field : shall be 0 after CATEGORY_Wind(), 1 for CATEGORY_Rind()
 * _iname : name of index bit field
 * _ival  : index value
 * _fname : field name
 * _fval  : field variable that field value shall be stored.
 *
 * CATEGORY_Rind : General indexed register Read.(
 * CATEGORY_Wind : General indexed register Read.
 *
 * CATEGORY_Ridx : For 'index', 'rw', 'load' field name
 * CATEGORY_Widx : For 'index', 'rw', 'load' field name and NO_LOAD.
 */
#define CATEGORY_Rind(_r, _lname, _rwname, _iname, _ival, _fname, _fval)				\
							do {												\
								CATEGORY_Wr03(_r,_lname,0,_rwname,1,_iname,_ival);	\
								CATEGORY_WrFL(_r);									\
								CATEGORY_RdFL(_r);									\
								CATEGORY_Rd01(_r,_fname,_fval);						\
							} while (0)

#define CATEGORY_Wind(_r, _lname, _rwname, _iname, _ival, _fname, _fval)				\
				CATEGORY_Wr04(_r, _lname,0, _rwname,0, _iname,_ival, _fname,_fval)


#define CATEGORY_Ridx(_r, _ival, _fname, _fval)	CATEGORY_Rind(_r,load,rw,index,_ival,_fname,_fval)

#define CATEGORY_Widx(_r, _ival, _fname, _fval)	CATEGORY_Wind(_r,load,rw,index,_ival,_fname,_fval)

/** @} *//* end of macro documentation */

/*
 * @{
 * Naming for register pointer.
 * gpRealRegSync : real register of Sync.
 * gpRegSync     : shadow register.
 *
 * @def Sync_RdFL: Read  FLushing : Shadow <- Real.
 * @def Sync_WrFL: Write FLushing : Shadow -> Real.
 * @def Sync_Rd  : Read  whole register(UINT32) from Shadow register.
 * @def Sync_Wr  : Write whole register(UINT32) from Shadow register.
 * @def Sync_Rd01 ~ Sync_Rdnn: Read  given '01~nn' fields from Shadow register.
 * @def Sync_Wr01 ~ Sync_Wrnn: Write given '01~nn' fields to   Shadow register.
 * */
#define Sync_RdFL(_r)			((gpRegSync->_r)=(gpRealRegSync->_r))
#define Sync_WrFL(_r)			((gpRealRegSync->_r)=(gpRegSync->_r))

#define Sync_Rd(_r)			*((UINT32*)(&(gpRegSync->_r)))
#define Sync_Wr(_r,_v)			((Sync_Rd(_r))=((UINT32)(_v)))

#define Sync_Rd01(_r,_f01,_v01)													\
								do { 											\
									(_v01) = (gpRegSync->_r._f01);				\
								} while(0)

#define Sync_Rd02(_r,_f01,_v01,_f02,_v02)										\
								do { 											\
									(_v01) = (gpRegSync->_r._f01);				\
									(_v02) = (gpRegSync->_r._f02);				\
								} while(0)

#define Sync_Rd03(_r,_f01,_v01,_f02,_v02,_f03,_v03)								\
								do { 											\
									(_v01) = (gpRegSync->_r._f01);				\
									(_v02) = (gpRegSync->_r._f02);				\
									(_v03) = (gpRegSync->_r._f03);				\
								} while(0)

#define Sync_Rd04(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04)					\
								do { 											\
									(_v01) = (gpRegSync->_r._f01);				\
									(_v02) = (gpRegSync->_r._f02);				\
									(_v03) = (gpRegSync->_r._f03);				\
									(_v04) = (gpRegSync->_r._f04);				\
								} while(0)

#define Sync_Rd05(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05)													\
								do { 											\
									(_v01) = (gpRegSync->_r._f01);				\
									(_v02) = (gpRegSync->_r._f02);				\
									(_v03) = (gpRegSync->_r._f03);				\
									(_v04) = (gpRegSync->_r._f04);				\
									(_v05) = (gpRegSync->_r._f05);				\
								} while(0)

#define Sync_Rd06(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06)										\
								do { 											\
									(_v01) = (gpRegSync->_r._f01);				\
									(_v02) = (gpRegSync->_r._f02);				\
									(_v03) = (gpRegSync->_r._f03);				\
									(_v04) = (gpRegSync->_r._f04);				\
									(_v05) = (gpRegSync->_r._f05);				\
									(_v06) = (gpRegSync->_r._f06);				\
								} while(0)

#define Sync_Rd07(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07)								\
								do { 											\
									(_v01) = (gpRegSync->_r._f01);				\
									(_v02) = (gpRegSync->_r._f02);				\
									(_v03) = (gpRegSync->_r._f03);				\
									(_v04) = (gpRegSync->_r._f04);				\
									(_v05) = (gpRegSync->_r._f05);				\
									(_v06) = (gpRegSync->_r._f06);				\
									(_v07) = (gpRegSync->_r._f07);				\
								} while(0)

#define Sync_Rd08(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08)					\
								do { 											\
									(_v01) = (gpRegSync->_r._f01);				\
									(_v02) = (gpRegSync->_r._f02);				\
									(_v03) = (gpRegSync->_r._f03);				\
									(_v04) = (gpRegSync->_r._f04);				\
									(_v05) = (gpRegSync->_r._f05);				\
									(_v06) = (gpRegSync->_r._f06);				\
									(_v07) = (gpRegSync->_r._f07);				\
									(_v08) = (gpRegSync->_r._f08);				\
								} while(0)

#define Sync_Rd09(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09)													\
								do { 											\
									(_v01) = (gpRegSync->_r._f01);				\
									(_v02) = (gpRegSync->_r._f02);				\
									(_v03) = (gpRegSync->_r._f03);				\
									(_v04) = (gpRegSync->_r._f04);				\
									(_v05) = (gpRegSync->_r._f05);				\
									(_v06) = (gpRegSync->_r._f06);				\
									(_v07) = (gpRegSync->_r._f07);				\
									(_v08) = (gpRegSync->_r._f08);				\
									(_v09) = (gpRegSync->_r._f09);				\
								} while(0)

#define Sync_Rd10(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10)										\
								do { 											\
									(_v01) = (gpRegSync->_r._f01);				\
									(_v02) = (gpRegSync->_r._f02);				\
									(_v03) = (gpRegSync->_r._f03);				\
									(_v04) = (gpRegSync->_r._f04);				\
									(_v05) = (gpRegSync->_r._f05);				\
									(_v06) = (gpRegSync->_r._f06);				\
									(_v07) = (gpRegSync->_r._f07);				\
									(_v08) = (gpRegSync->_r._f08);				\
									(_v09) = (gpRegSync->_r._f09);				\
									(_v10) = (gpRegSync->_r._f10);				\
								} while(0)

#define Sync_Rd11(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11)								\
								do { 											\
									(_v01) = (gpRegSync->_r._f01);				\
									(_v02) = (gpRegSync->_r._f02);				\
									(_v03) = (gpRegSync->_r._f03);				\
									(_v04) = (gpRegSync->_r._f04);				\
									(_v05) = (gpRegSync->_r._f05);				\
									(_v06) = (gpRegSync->_r._f06);				\
									(_v07) = (gpRegSync->_r._f07);				\
									(_v08) = (gpRegSync->_r._f08);				\
									(_v09) = (gpRegSync->_r._f09);				\
									(_v10) = (gpRegSync->_r._f10);				\
									(_v11) = (gpRegSync->_r._f11);				\
								} while(0)

#define Sync_Rd12(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12)					\
								do { 											\
									(_v01) = (gpRegSync->_r._f01);				\
									(_v02) = (gpRegSync->_r._f02);				\
									(_v03) = (gpRegSync->_r._f03);				\
									(_v04) = (gpRegSync->_r._f04);				\
									(_v05) = (gpRegSync->_r._f05);				\
									(_v06) = (gpRegSync->_r._f06);				\
									(_v07) = (gpRegSync->_r._f07);				\
									(_v08) = (gpRegSync->_r._f08);				\
									(_v09) = (gpRegSync->_r._f09);				\
									(_v10) = (gpRegSync->_r._f10);				\
									(_v11) = (gpRegSync->_r._f11);				\
									(_v12) = (gpRegSync->_r._f12);				\
								} while(0)

#define Sync_Rd13(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13)													\
								do { 											\
									(_v01) = (gpRegSync->_r._f01);				\
									(_v02) = (gpRegSync->_r._f02);				\
									(_v03) = (gpRegSync->_r._f03);				\
									(_v04) = (gpRegSync->_r._f04);				\
									(_v05) = (gpRegSync->_r._f05);				\
									(_v06) = (gpRegSync->_r._f06);				\
									(_v07) = (gpRegSync->_r._f07);				\
									(_v08) = (gpRegSync->_r._f08);				\
									(_v09) = (gpRegSync->_r._f09);				\
									(_v10) = (gpRegSync->_r._f10);				\
									(_v11) = (gpRegSync->_r._f11);				\
									(_v12) = (gpRegSync->_r._f12);				\
									(_v13) = (gpRegSync->_r._f13);				\
								} while(0)

#define Sync_Rd14(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14)										\
								do { 											\
									(_v01) = (gpRegSync->_r._f01);				\
									(_v02) = (gpRegSync->_r._f02);				\
									(_v03) = (gpRegSync->_r._f03);				\
									(_v04) = (gpRegSync->_r._f04);				\
									(_v05) = (gpRegSync->_r._f05);				\
									(_v06) = (gpRegSync->_r._f06);				\
									(_v07) = (gpRegSync->_r._f07);				\
									(_v08) = (gpRegSync->_r._f08);				\
									(_v09) = (gpRegSync->_r._f09);				\
									(_v10) = (gpRegSync->_r._f10);				\
									(_v11) = (gpRegSync->_r._f11);				\
									(_v12) = (gpRegSync->_r._f12);				\
									(_v13) = (gpRegSync->_r._f13);				\
									(_v14) = (gpRegSync->_r._f14);				\
								} while(0)

#define Sync_Rd15(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15)								\
								do { 											\
									(_v01) = (gpRegSync->_r._f01);				\
									(_v02) = (gpRegSync->_r._f02);				\
									(_v03) = (gpRegSync->_r._f03);				\
									(_v04) = (gpRegSync->_r._f04);				\
									(_v05) = (gpRegSync->_r._f05);				\
									(_v06) = (gpRegSync->_r._f06);				\
									(_v07) = (gpRegSync->_r._f07);				\
									(_v08) = (gpRegSync->_r._f08);				\
									(_v09) = (gpRegSync->_r._f09);				\
									(_v10) = (gpRegSync->_r._f10);				\
									(_v11) = (gpRegSync->_r._f11);				\
									(_v12) = (gpRegSync->_r._f12);				\
									(_v13) = (gpRegSync->_r._f13);				\
									(_v14) = (gpRegSync->_r._f14);				\
									(_v15) = (gpRegSync->_r._f15);				\
								} while(0)

#define Sync_Rd16(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15,_f16,_v16)					\
								do { 											\
									(_v01) = (gpRegSync->_r._f01);				\
									(_v02) = (gpRegSync->_r._f02);				\
									(_v03) = (gpRegSync->_r._f03);				\
									(_v04) = (gpRegSync->_r._f04);				\
									(_v05) = (gpRegSync->_r._f05);				\
									(_v06) = (gpRegSync->_r._f06);				\
									(_v07) = (gpRegSync->_r._f07);				\
									(_v08) = (gpRegSync->_r._f08);				\
									(_v09) = (gpRegSync->_r._f09);				\
									(_v10) = (gpRegSync->_r._f10);				\
									(_v11) = (gpRegSync->_r._f11);				\
									(_v12) = (gpRegSync->_r._f12);				\
									(_v13) = (gpRegSync->_r._f13);				\
									(_v14) = (gpRegSync->_r._f14);				\
									(_v15) = (gpRegSync->_r._f15);				\
									(_v16) = (gpRegSync->_r._f16);				\
								} while(0)


#define Sync_Wr01(_r,_f01,_v01)													\
								do { 											\
									(gpRegSync->_r._f01) = (_v01);				\
								} while(0)

#define Sync_Wr02(_r,_f01,_v01,_f02,_v02)										\
								do { 											\
									(gpRegSync->_r._f01) = (_v01);				\
									(gpRegSync->_r._f02) = (_v02);				\
								} while(0)

#define Sync_Wr03(_r,_f01,_v01,_f02,_v02,_f03,_v03)								\
								do { 											\
									(gpRegSync->_r._f01) = (_v01);				\
									(gpRegSync->_r._f02) = (_v02);				\
									(gpRegSync->_r._f03) = (_v03);				\
								} while(0)

#define Sync_Wr04(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04)					\
								do { 											\
									(gpRegSync->_r._f01) = (_v01);				\
									(gpRegSync->_r._f02) = (_v02);				\
									(gpRegSync->_r._f03) = (_v03);				\
									(gpRegSync->_r._f04) = (_v04);				\
								} while(0)

#define Sync_Wr05(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05)													\
								do { 											\
									(gpRegSync->_r._f01) = (_v01);				\
									(gpRegSync->_r._f02) = (_v02);				\
									(gpRegSync->_r._f03) = (_v03);				\
									(gpRegSync->_r._f04) = (_v04);				\
									(gpRegSync->_r._f05) = (_v05);				\
								} while(0)

#define Sync_Wr06(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06)										\
								do { 											\
									(gpRegSync->_r._f01) = (_v01);				\
									(gpRegSync->_r._f02) = (_v02);				\
									(gpRegSync->_r._f03) = (_v03);				\
									(gpRegSync->_r._f04) = (_v04);				\
									(gpRegSync->_r._f05) = (_v05);				\
									(gpRegSync->_r._f06) = (_v06);				\
								} while(0)

#define Sync_Wr07(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07)								\
								do { 											\
									(gpRegSync->_r._f01) = (_v01);				\
									(gpRegSync->_r._f02) = (_v02);				\
									(gpRegSync->_r._f03) = (_v03);				\
									(gpRegSync->_r._f04) = (_v04);				\
									(gpRegSync->_r._f05) = (_v05);				\
									(gpRegSync->_r._f06) = (_v06);				\
									(gpRegSync->_r._f07) = (_v07);				\
								} while(0)

#define Sync_Wr08(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08)					\
								do { 											\
									(gpRegSync->_r._f01) = (_v01);				\
									(gpRegSync->_r._f02) = (_v02);				\
									(gpRegSync->_r._f03) = (_v03);				\
									(gpRegSync->_r._f04) = (_v04);				\
									(gpRegSync->_r._f05) = (_v05);				\
									(gpRegSync->_r._f06) = (_v06);				\
									(gpRegSync->_r._f07) = (_v07);				\
									(gpRegSync->_r._f08) = (_v08);				\
								} while(0)

#define Sync_Wr09(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09)													\
								do { 											\
									(gpRegSync->_r._f01) = (_v01);				\
									(gpRegSync->_r._f02) = (_v02);				\
									(gpRegSync->_r._f03) = (_v03);				\
									(gpRegSync->_r._f04) = (_v04);				\
									(gpRegSync->_r._f05) = (_v05);				\
									(gpRegSync->_r._f06) = (_v06);				\
									(gpRegSync->_r._f07) = (_v07);				\
									(gpRegSync->_r._f08) = (_v08);				\
									(gpRegSync->_r._f09) = (_v09);				\
								} while(0)

#define Sync_Wr10(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10)										\
								do { 											\
									(gpRegSync->_r._f01) = (_v01);				\
									(gpRegSync->_r._f02) = (_v02);				\
									(gpRegSync->_r._f03) = (_v03);				\
									(gpRegSync->_r._f04) = (_v04);				\
									(gpRegSync->_r._f05) = (_v05);				\
									(gpRegSync->_r._f06) = (_v06);				\
									(gpRegSync->_r._f07) = (_v07);				\
									(gpRegSync->_r._f08) = (_v08);				\
									(gpRegSync->_r._f09) = (_v09);				\
									(gpRegSync->_r._f10) = (_v10);				\
								} while(0)

#define Sync_Wr11(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11)								\
								do { 											\
									(gpRegSync->_r._f01) = (_v01);				\
									(gpRegSync->_r._f02) = (_v02);				\
									(gpRegSync->_r._f03) = (_v03);				\
									(gpRegSync->_r._f04) = (_v04);				\
									(gpRegSync->_r._f05) = (_v05);				\
									(gpRegSync->_r._f06) = (_v06);				\
									(gpRegSync->_r._f07) = (_v07);				\
									(gpRegSync->_r._f08) = (_v08);				\
									(gpRegSync->_r._f09) = (_v09);				\
									(gpRegSync->_r._f10) = (_v10);				\
									(gpRegSync->_r._f11) = (_v11);				\
								} while(0)

#define Sync_Wr12(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12)					\
								do { 											\
									(gpRegSync->_r._f01) = (_v01);				\
									(gpRegSync->_r._f02) = (_v02);				\
									(gpRegSync->_r._f03) = (_v03);				\
									(gpRegSync->_r._f04) = (_v04);				\
									(gpRegSync->_r._f05) = (_v05);				\
									(gpRegSync->_r._f06) = (_v06);				\
									(gpRegSync->_r._f07) = (_v07);				\
									(gpRegSync->_r._f08) = (_v08);				\
									(gpRegSync->_r._f09) = (_v09);				\
									(gpRegSync->_r._f10) = (_v10);				\
									(gpRegSync->_r._f11) = (_v11);				\
									(gpRegSync->_r._f12) = (_v12);				\
								} while(0)

#define Sync_Wr13(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13)													\
								do { 											\
									(gpRegSync->_r._f01) = (_v01);				\
									(gpRegSync->_r._f02) = (_v02);				\
									(gpRegSync->_r._f03) = (_v03);				\
									(gpRegSync->_r._f04) = (_v04);				\
									(gpRegSync->_r._f05) = (_v05);				\
									(gpRegSync->_r._f06) = (_v06);				\
									(gpRegSync->_r._f07) = (_v07);				\
									(gpRegSync->_r._f08) = (_v08);				\
									(gpRegSync->_r._f09) = (_v09);				\
									(gpRegSync->_r._f10) = (_v10);				\
									(gpRegSync->_r._f11) = (_v11);				\
									(gpRegSync->_r._f12) = (_v12);				\
									(gpRegSync->_r._f13) = (_v13);				\
								} while(0)

#define Sync_Wr14(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14)										\
								do { 											\
									(gpRegSync->_r._f01) = (_v01);				\
									(gpRegSync->_r._f02) = (_v02);				\
									(gpRegSync->_r._f03) = (_v03);				\
									(gpRegSync->_r._f04) = (_v04);				\
									(gpRegSync->_r._f05) = (_v05);				\
									(gpRegSync->_r._f06) = (_v06);				\
									(gpRegSync->_r._f07) = (_v07);				\
									(gpRegSync->_r._f08) = (_v08);				\
									(gpRegSync->_r._f09) = (_v09);				\
									(gpRegSync->_r._f10) = (_v10);				\
									(gpRegSync->_r._f11) = (_v11);				\
									(gpRegSync->_r._f12) = (_v12);				\
									(gpRegSync->_r._f13) = (_v13);				\
									(gpRegSync->_r._f14) = (_v14);				\
								} while(0)

#define Sync_Wr15(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15)								\
								do { 											\
									(gpRegSync->_r._f01) = (_v01);				\
									(gpRegSync->_r._f02) = (_v02);				\
									(gpRegSync->_r._f03) = (_v03);				\
									(gpRegSync->_r._f04) = (_v04);				\
									(gpRegSync->_r._f05) = (_v05);				\
									(gpRegSync->_r._f06) = (_v06);				\
									(gpRegSync->_r._f07) = (_v07);				\
									(gpRegSync->_r._f08) = (_v08);				\
									(gpRegSync->_r._f09) = (_v09);				\
									(gpRegSync->_r._f10) = (_v10);				\
									(gpRegSync->_r._f11) = (_v11);				\
									(gpRegSync->_r._f12) = (_v12);				\
									(gpRegSync->_r._f13) = (_v13);				\
									(gpRegSync->_r._f14) = (_v14);				\
									(gpRegSync->_r._f15) = (_v15);				\
								} while(0)

#define Sync_Wr16(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15,_f16,_v16)					\
								do { 											\
									(gpRegSync->_r._f01) = (_v01);				\
									(gpRegSync->_r._f02) = (_v02);				\
									(gpRegSync->_r._f03) = (_v03);				\
									(gpRegSync->_r._f04) = (_v04);				\
									(gpRegSync->_r._f05) = (_v05);				\
									(gpRegSync->_r._f06) = (_v06);				\
									(gpRegSync->_r._f07) = (_v07);				\
									(gpRegSync->_r._f08) = (_v08);				\
									(gpRegSync->_r._f09) = (_v09);				\
									(gpRegSync->_r._f10) = (_v10);				\
									(gpRegSync->_r._f11) = (_v11);				\
									(gpRegSync->_r._f12) = (_v12);				\
									(gpRegSync->_r._f13) = (_v13);				\
									(gpRegSync->_r._f14) = (_v14);				\
									(gpRegSync->_r._f15) = (_v15);				\
									(gpRegSync->_r._f16) = (_v16);				\
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
 * _rwname: name of rw    bit field : shall be 0 after Sync_Wind(), 1 for Sync_Rind()
 * _iname : name of index bit field
 * _ival  : index value
 * _fname : field name
 * _fval  : field variable that field value shall be stored.
 *
 * Sync_Rind : General indexed register Read.(
 * Sync_Wind : General indexed register Read.
 *
 * Sync_Ridx : For 'index', 'rw', 'load' field name
 * Sync_Widx : For 'index', 'rw', 'load' field name and NO_LOAD.
 */
#define Sync_Rind(_r, _lname, _rwname, _iname, _ival, _fname, _fval)				\
							do {												\
								Sync_Wr03(_r,_lname,0,_rwname,1,_iname,_ival);	\
								Sync_WrFL(_r);									\
								Sync_RdFL(_r);									\
								Sync_Rd01(_r,_fname,_fval);						\
							} while (0)

#define Sync_Wind(_r, _lname, _rwname, _iname, _ival, _fname, _fval)				\
				Sync_Wr04(_r, _lname,0, _rwname,0, _iname,_ival, _fname,_fval)


#define Sync_Ridx(_r, _ival, _fname, _fval)	Sync_Rind(_r,load,rw,index,_ival,_fname,_fval)

#define Sync_Widx(_r, _ival, _fname, _fval)	Sync_Wind(_r,load,rw,index,_ival,_fname,_fval)

/** @} *//* end of macro documentation */

#ifdef __cplusplus
}
#endif

#endif	/* _VSYNC_REG_H_ */

/* from 'H15-VDEC0-MAN-VDEC-REG_v1.0_SYNC.csv'  ???ѹα? ǥ?ؽ? by getregs v2.7 */
