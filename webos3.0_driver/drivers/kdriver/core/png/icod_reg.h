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


/** @file
 *
 *  ICOD register details. ( used only within kdriver )
 *
 *  author     Youngwoo Jin (youngwoo.jin@lge.com)
 *  version    1.0
 *  date       2011.11.27
 *
 */

#ifndef _ICOD_REG_H13_H_
#define _ICOD_REG_H13_H_

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
	0x0000 sw_rst ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	sw_rst_status_mjpeg             : 4,	//  0: 3
	intr_ack_en_mjpeg               : 1,	//     4
	                                :10,	//  5:14 reserved
	sw_rst_mjpeg                    : 1,	//    15
	sw_rst_status_png               : 4,	// 16:19
	intr_ack_en_png                 : 1,	//    20
	                                :10,	// 21:30 reserved
	sw_rst_png                      : 1;	//    31
} SW_RST;

/*-----------------------------------------------------------------------------
	0x0004 clk_enable ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	clk_enable                      : 1;	//     0
} CLK_ENABLE;

/*-----------------------------------------------------------------------------
	0x0008 intr_en_sw_rst_done ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	intr_en_sw_rst_done_mjpeg       : 1,	//     0
	intr_en_sw_rst_done_png         : 1;	//     1
} INTR_EN_SW_RST_DONE;

/*-----------------------------------------------------------------------------
	0x000c intr_status_sw_rst_done ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	intr_status_sw_rst_done_mjpeg   : 1,	//     0
	intr_status_sw_rst_done_png     : 1;	//     1
} INTR_STATUS_SW_RST_DONE;

/*-----------------------------------------------------------------------------
	0x0010 intr_clear_sw_rst_done ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	intr_clear_sw_rst_done_mjpeg    : 1,	//     0
	intr_clear_sw_rst_done_png      : 1;	//     1
} INTR_CLEAR_SW_RST_DONE;

typedef struct {
	UINT32
	flush_wait_cycle_png			: 16,
	flush_wait_cycle_mjpeg			: 16;
}
FLUSH_WAIT_CYCLE;

typedef struct {
	UINT32
	sw_rst_wait_cycle_png			: 16,
	sw_rst_wait_cycle_mjpeg			: 16;
}
SW_RST_WAIT_CYCLE;

typedef struct {
	SW_RST                          	sw_rst                          ;	// 0x0000 : ''
	CLK_ENABLE                      	clk_enable                      ;	// 0x0004 : ''
	INTR_EN_SW_RST_DONE             	intr_en_sw_rst_done             ;	// 0x0008 : ''
	INTR_STATUS_SW_RST_DONE         	intr_status_sw_rst_done         ;	// 0x000c : ''
	INTR_CLEAR_SW_RST_DONE          	intr_clear_sw_rst_done          ;	// 0x0010 : ''
	FLUSH_WAIT_CYCLE					flush_wait_cycle;
	SW_RST_WAIT_CYCLE					sw_rst_wait_cycle;
} ICOD_REG_T;
/* 5 regs, 5 types */

/* 5 regs, 5 types in Total*/

/*
 * @{
 * Naming for register pointer.
 * gpRealRegICOD : real register of ICOD.
 * gpRegICOD     : shadow register.
 *
 * @def ICOD_RdFL: Read  FLushing : Shadow <- Real.
 * @def ICOD_WrFL: Write FLushing : Shadow -> Real.
 * @def ICOD_Rd  : Read  whole register(UINT32) from Shadow register.
 * @def ICOD_Wr  : Write whole register(UINT32) from Shadow register.
 * @def ICOD_Rd01 ~ ICOD_Rdnn: Read  given '01~nn' fields from Shadow register.
 * @def ICOD_Wr01 ~ ICOD_Wrnn: Write given '01~nn' fields to   Shadow register.
 * */
#define ICOD_RdFL(_r)			((gpRegICOD->_r)=(gpRealRegICOD->_r))
#define ICOD_WrFL(_r)			((gpRealRegICOD->_r)=(gpRegICOD->_r))

#define ICOD_Rd(_r)				*((UINT32*)(&(gpRegICOD->_r)))
#define ICOD_Wr(_r,_v)			((ICOD_Rd(_r))=((UINT32)(_v)))

#define ICOD_Rd00(_r,_f01)	 	(gpRegICOD->_r._f01)

#define ICOD_Rd01(_r,_f01,_v01)													\
								do { 											\
									(_v01) = (gpRegICOD->_r._f01);				\
								} while(0)

#define ICOD_Rd02(_r,_f01,_v01,_f02,_v02)										\
								do { 											\
									(_v01) = (gpRegICOD->_r._f01);				\
									(_v02) = (gpRegICOD->_r._f02);				\
								} while(0)

#define ICOD_Rd03(_r,_f01,_v01,_f02,_v02,_f03,_v03)								\
								do { 											\
									(_v01) = (gpRegICOD->_r._f01);				\
									(_v02) = (gpRegICOD->_r._f02);				\
									(_v03) = (gpRegICOD->_r._f03);				\
								} while(0)

#define ICOD_Rd04(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04)					\
								do { 											\
									(_v01) = (gpRegICOD->_r._f01);				\
									(_v02) = (gpRegICOD->_r._f02);				\
									(_v03) = (gpRegICOD->_r._f03);				\
									(_v04) = (gpRegICOD->_r._f04);				\
								} while(0)

#define ICOD_Rd05(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05)													\
								do { 											\
									(_v01) = (gpRegICOD->_r._f01);				\
									(_v02) = (gpRegICOD->_r._f02);				\
									(_v03) = (gpRegICOD->_r._f03);				\
									(_v04) = (gpRegICOD->_r._f04);				\
									(_v05) = (gpRegICOD->_r._f05);				\
								} while(0)

#define ICOD_Rd06(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06)										\
								do { 											\
									(_v01) = (gpRegICOD->_r._f01);				\
									(_v02) = (gpRegICOD->_r._f02);				\
									(_v03) = (gpRegICOD->_r._f03);				\
									(_v04) = (gpRegICOD->_r._f04);				\
									(_v05) = (gpRegICOD->_r._f05);				\
									(_v06) = (gpRegICOD->_r._f06);				\
								} while(0)

#define ICOD_Rd07(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07)								\
								do { 											\
									(_v01) = (gpRegICOD->_r._f01);				\
									(_v02) = (gpRegICOD->_r._f02);				\
									(_v03) = (gpRegICOD->_r._f03);				\
									(_v04) = (gpRegICOD->_r._f04);				\
									(_v05) = (gpRegICOD->_r._f05);				\
									(_v06) = (gpRegICOD->_r._f06);				\
									(_v07) = (gpRegICOD->_r._f07);				\
								} while(0)

#define ICOD_Rd08(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08)					\
								do { 											\
									(_v01) = (gpRegICOD->_r._f01);				\
									(_v02) = (gpRegICOD->_r._f02);				\
									(_v03) = (gpRegICOD->_r._f03);				\
									(_v04) = (gpRegICOD->_r._f04);				\
									(_v05) = (gpRegICOD->_r._f05);				\
									(_v06) = (gpRegICOD->_r._f06);				\
									(_v07) = (gpRegICOD->_r._f07);				\
									(_v08) = (gpRegICOD->_r._f08);				\
								} while(0)

#define ICOD_Rd09(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09)													\
								do { 											\
									(_v01) = (gpRegICOD->_r._f01);				\
									(_v02) = (gpRegICOD->_r._f02);				\
									(_v03) = (gpRegICOD->_r._f03);				\
									(_v04) = (gpRegICOD->_r._f04);				\
									(_v05) = (gpRegICOD->_r._f05);				\
									(_v06) = (gpRegICOD->_r._f06);				\
									(_v07) = (gpRegICOD->_r._f07);				\
									(_v08) = (gpRegICOD->_r._f08);				\
									(_v09) = (gpRegICOD->_r._f09);				\
								} while(0)

#define ICOD_Rd10(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10)										\
								do { 											\
									(_v01) = (gpRegICOD->_r._f01);				\
									(_v02) = (gpRegICOD->_r._f02);				\
									(_v03) = (gpRegICOD->_r._f03);				\
									(_v04) = (gpRegICOD->_r._f04);				\
									(_v05) = (gpRegICOD->_r._f05);				\
									(_v06) = (gpRegICOD->_r._f06);				\
									(_v07) = (gpRegICOD->_r._f07);				\
									(_v08) = (gpRegICOD->_r._f08);				\
									(_v09) = (gpRegICOD->_r._f09);				\
									(_v10) = (gpRegICOD->_r._f10);				\
								} while(0)

#define ICOD_Rd11(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11)								\
								do { 											\
									(_v01) = (gpRegICOD->_r._f01);				\
									(_v02) = (gpRegICOD->_r._f02);				\
									(_v03) = (gpRegICOD->_r._f03);				\
									(_v04) = (gpRegICOD->_r._f04);				\
									(_v05) = (gpRegICOD->_r._f05);				\
									(_v06) = (gpRegICOD->_r._f06);				\
									(_v07) = (gpRegICOD->_r._f07);				\
									(_v08) = (gpRegICOD->_r._f08);				\
									(_v09) = (gpRegICOD->_r._f09);				\
									(_v10) = (gpRegICOD->_r._f10);				\
									(_v11) = (gpRegICOD->_r._f11);				\
								} while(0)

#define ICOD_Rd12(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12)					\
								do { 											\
									(_v01) = (gpRegICOD->_r._f01);				\
									(_v02) = (gpRegICOD->_r._f02);				\
									(_v03) = (gpRegICOD->_r._f03);				\
									(_v04) = (gpRegICOD->_r._f04);				\
									(_v05) = (gpRegICOD->_r._f05);				\
									(_v06) = (gpRegICOD->_r._f06);				\
									(_v07) = (gpRegICOD->_r._f07);				\
									(_v08) = (gpRegICOD->_r._f08);				\
									(_v09) = (gpRegICOD->_r._f09);				\
									(_v10) = (gpRegICOD->_r._f10);				\
									(_v11) = (gpRegICOD->_r._f11);				\
									(_v12) = (gpRegICOD->_r._f12);				\
								} while(0)

#define ICOD_Rd13(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13)													\
								do { 											\
									(_v01) = (gpRegICOD->_r._f01);				\
									(_v02) = (gpRegICOD->_r._f02);				\
									(_v03) = (gpRegICOD->_r._f03);				\
									(_v04) = (gpRegICOD->_r._f04);				\
									(_v05) = (gpRegICOD->_r._f05);				\
									(_v06) = (gpRegICOD->_r._f06);				\
									(_v07) = (gpRegICOD->_r._f07);				\
									(_v08) = (gpRegICOD->_r._f08);				\
									(_v09) = (gpRegICOD->_r._f09);				\
									(_v10) = (gpRegICOD->_r._f10);				\
									(_v11) = (gpRegICOD->_r._f11);				\
									(_v12) = (gpRegICOD->_r._f12);				\
									(_v13) = (gpRegICOD->_r._f13);				\
								} while(0)

#define ICOD_Rd14(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14)										\
								do { 											\
									(_v01) = (gpRegICOD->_r._f01);				\
									(_v02) = (gpRegICOD->_r._f02);				\
									(_v03) = (gpRegICOD->_r._f03);				\
									(_v04) = (gpRegICOD->_r._f04);				\
									(_v05) = (gpRegICOD->_r._f05);				\
									(_v06) = (gpRegICOD->_r._f06);				\
									(_v07) = (gpRegICOD->_r._f07);				\
									(_v08) = (gpRegICOD->_r._f08);				\
									(_v09) = (gpRegICOD->_r._f09);				\
									(_v10) = (gpRegICOD->_r._f10);				\
									(_v11) = (gpRegICOD->_r._f11);				\
									(_v12) = (gpRegICOD->_r._f12);				\
									(_v13) = (gpRegICOD->_r._f13);				\
									(_v14) = (gpRegICOD->_r._f14);				\
								} while(0)

#define ICOD_Rd15(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15)								\
								do { 											\
									(_v01) = (gpRegICOD->_r._f01);				\
									(_v02) = (gpRegICOD->_r._f02);				\
									(_v03) = (gpRegICOD->_r._f03);				\
									(_v04) = (gpRegICOD->_r._f04);				\
									(_v05) = (gpRegICOD->_r._f05);				\
									(_v06) = (gpRegICOD->_r._f06);				\
									(_v07) = (gpRegICOD->_r._f07);				\
									(_v08) = (gpRegICOD->_r._f08);				\
									(_v09) = (gpRegICOD->_r._f09);				\
									(_v10) = (gpRegICOD->_r._f10);				\
									(_v11) = (gpRegICOD->_r._f11);				\
									(_v12) = (gpRegICOD->_r._f12);				\
									(_v13) = (gpRegICOD->_r._f13);				\
									(_v14) = (gpRegICOD->_r._f14);				\
									(_v15) = (gpRegICOD->_r._f15);				\
								} while(0)

#define ICOD_Rd16(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15,_f16,_v16)					\
								do { 											\
									(_v01) = (gpRegICOD->_r._f01);				\
									(_v02) = (gpRegICOD->_r._f02);				\
									(_v03) = (gpRegICOD->_r._f03);				\
									(_v04) = (gpRegICOD->_r._f04);				\
									(_v05) = (gpRegICOD->_r._f05);				\
									(_v06) = (gpRegICOD->_r._f06);				\
									(_v07) = (gpRegICOD->_r._f07);				\
									(_v08) = (gpRegICOD->_r._f08);				\
									(_v09) = (gpRegICOD->_r._f09);				\
									(_v10) = (gpRegICOD->_r._f10);				\
									(_v11) = (gpRegICOD->_r._f11);				\
									(_v12) = (gpRegICOD->_r._f12);				\
									(_v13) = (gpRegICOD->_r._f13);				\
									(_v14) = (gpRegICOD->_r._f14);				\
									(_v15) = (gpRegICOD->_r._f15);				\
									(_v16) = (gpRegICOD->_r._f16);				\
								} while(0)


#define ICOD_Wr01(_r,_f01,_v01)													\
								do { 											\
									(gpRegICOD->_r._f01) = (_v01);				\
								} while(0)

#define ICOD_Wr02(_r,_f01,_v01,_f02,_v02)										\
								do { 											\
									(gpRegICOD->_r._f01) = (_v01);				\
									(gpRegICOD->_r._f02) = (_v02);				\
								} while(0)

#define ICOD_Wr03(_r,_f01,_v01,_f02,_v02,_f03,_v03)								\
								do { 											\
									(gpRegICOD->_r._f01) = (_v01);				\
									(gpRegICOD->_r._f02) = (_v02);				\
									(gpRegICOD->_r._f03) = (_v03);				\
								} while(0)

#define ICOD_Wr04(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04)					\
								do { 											\
									(gpRegICOD->_r._f01) = (_v01);				\
									(gpRegICOD->_r._f02) = (_v02);				\
									(gpRegICOD->_r._f03) = (_v03);				\
									(gpRegICOD->_r._f04) = (_v04);				\
								} while(0)

#define ICOD_Wr05(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05)													\
								do { 											\
									(gpRegICOD->_r._f01) = (_v01);				\
									(gpRegICOD->_r._f02) = (_v02);				\
									(gpRegICOD->_r._f03) = (_v03);				\
									(gpRegICOD->_r._f04) = (_v04);				\
									(gpRegICOD->_r._f05) = (_v05);				\
								} while(0)

#define ICOD_Wr06(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06)										\
								do { 											\
									(gpRegICOD->_r._f01) = (_v01);				\
									(gpRegICOD->_r._f02) = (_v02);				\
									(gpRegICOD->_r._f03) = (_v03);				\
									(gpRegICOD->_r._f04) = (_v04);				\
									(gpRegICOD->_r._f05) = (_v05);				\
									(gpRegICOD->_r._f06) = (_v06);				\
								} while(0)

#define ICOD_Wr07(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07)								\
								do { 											\
									(gpRegICOD->_r._f01) = (_v01);				\
									(gpRegICOD->_r._f02) = (_v02);				\
									(gpRegICOD->_r._f03) = (_v03);				\
									(gpRegICOD->_r._f04) = (_v04);				\
									(gpRegICOD->_r._f05) = (_v05);				\
									(gpRegICOD->_r._f06) = (_v06);				\
									(gpRegICOD->_r._f07) = (_v07);				\
								} while(0)

#define ICOD_Wr08(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08)					\
								do { 											\
									(gpRegICOD->_r._f01) = (_v01);				\
									(gpRegICOD->_r._f02) = (_v02);				\
									(gpRegICOD->_r._f03) = (_v03);				\
									(gpRegICOD->_r._f04) = (_v04);				\
									(gpRegICOD->_r._f05) = (_v05);				\
									(gpRegICOD->_r._f06) = (_v06);				\
									(gpRegICOD->_r._f07) = (_v07);				\
									(gpRegICOD->_r._f08) = (_v08);				\
								} while(0)

#define ICOD_Wr09(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09)													\
								do { 											\
									(gpRegICOD->_r._f01) = (_v01);				\
									(gpRegICOD->_r._f02) = (_v02);				\
									(gpRegICOD->_r._f03) = (_v03);				\
									(gpRegICOD->_r._f04) = (_v04);				\
									(gpRegICOD->_r._f05) = (_v05);				\
									(gpRegICOD->_r._f06) = (_v06);				\
									(gpRegICOD->_r._f07) = (_v07);				\
									(gpRegICOD->_r._f08) = (_v08);				\
									(gpRegICOD->_r._f09) = (_v09);				\
								} while(0)

#define ICOD_Wr10(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10)										\
								do { 											\
									(gpRegICOD->_r._f01) = (_v01);				\
									(gpRegICOD->_r._f02) = (_v02);				\
									(gpRegICOD->_r._f03) = (_v03);				\
									(gpRegICOD->_r._f04) = (_v04);				\
									(gpRegICOD->_r._f05) = (_v05);				\
									(gpRegICOD->_r._f06) = (_v06);				\
									(gpRegICOD->_r._f07) = (_v07);				\
									(gpRegICOD->_r._f08) = (_v08);				\
									(gpRegICOD->_r._f09) = (_v09);				\
									(gpRegICOD->_r._f10) = (_v10);				\
								} while(0)

#define ICOD_Wr11(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11)								\
								do { 											\
									(gpRegICOD->_r._f01) = (_v01);				\
									(gpRegICOD->_r._f02) = (_v02);				\
									(gpRegICOD->_r._f03) = (_v03);				\
									(gpRegICOD->_r._f04) = (_v04);				\
									(gpRegICOD->_r._f05) = (_v05);				\
									(gpRegICOD->_r._f06) = (_v06);				\
									(gpRegICOD->_r._f07) = (_v07);				\
									(gpRegICOD->_r._f08) = (_v08);				\
									(gpRegICOD->_r._f09) = (_v09);				\
									(gpRegICOD->_r._f10) = (_v10);				\
									(gpRegICOD->_r._f11) = (_v11);				\
								} while(0)

#define ICOD_Wr12(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12)					\
								do { 											\
									(gpRegICOD->_r._f01) = (_v01);				\
									(gpRegICOD->_r._f02) = (_v02);				\
									(gpRegICOD->_r._f03) = (_v03);				\
									(gpRegICOD->_r._f04) = (_v04);				\
									(gpRegICOD->_r._f05) = (_v05);				\
									(gpRegICOD->_r._f06) = (_v06);				\
									(gpRegICOD->_r._f07) = (_v07);				\
									(gpRegICOD->_r._f08) = (_v08);				\
									(gpRegICOD->_r._f09) = (_v09);				\
									(gpRegICOD->_r._f10) = (_v10);				\
									(gpRegICOD->_r._f11) = (_v11);				\
									(gpRegICOD->_r._f12) = (_v12);				\
								} while(0)

#define ICOD_Wr13(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13)													\
								do { 											\
									(gpRegICOD->_r._f01) = (_v01);				\
									(gpRegICOD->_r._f02) = (_v02);				\
									(gpRegICOD->_r._f03) = (_v03);				\
									(gpRegICOD->_r._f04) = (_v04);				\
									(gpRegICOD->_r._f05) = (_v05);				\
									(gpRegICOD->_r._f06) = (_v06);				\
									(gpRegICOD->_r._f07) = (_v07);				\
									(gpRegICOD->_r._f08) = (_v08);				\
									(gpRegICOD->_r._f09) = (_v09);				\
									(gpRegICOD->_r._f10) = (_v10);				\
									(gpRegICOD->_r._f11) = (_v11);				\
									(gpRegICOD->_r._f12) = (_v12);				\
									(gpRegICOD->_r._f13) = (_v13);				\
								} while(0)

#define ICOD_Wr14(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14)										\
								do { 											\
									(gpRegICOD->_r._f01) = (_v01);				\
									(gpRegICOD->_r._f02) = (_v02);				\
									(gpRegICOD->_r._f03) = (_v03);				\
									(gpRegICOD->_r._f04) = (_v04);				\
									(gpRegICOD->_r._f05) = (_v05);				\
									(gpRegICOD->_r._f06) = (_v06);				\
									(gpRegICOD->_r._f07) = (_v07);				\
									(gpRegICOD->_r._f08) = (_v08);				\
									(gpRegICOD->_r._f09) = (_v09);				\
									(gpRegICOD->_r._f10) = (_v10);				\
									(gpRegICOD->_r._f11) = (_v11);				\
									(gpRegICOD->_r._f12) = (_v12);				\
									(gpRegICOD->_r._f13) = (_v13);				\
									(gpRegICOD->_r._f14) = (_v14);				\
								} while(0)

#define ICOD_Wr15(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15)								\
								do { 											\
									(gpRegICOD->_r._f01) = (_v01);				\
									(gpRegICOD->_r._f02) = (_v02);				\
									(gpRegICOD->_r._f03) = (_v03);				\
									(gpRegICOD->_r._f04) = (_v04);				\
									(gpRegICOD->_r._f05) = (_v05);				\
									(gpRegICOD->_r._f06) = (_v06);				\
									(gpRegICOD->_r._f07) = (_v07);				\
									(gpRegICOD->_r._f08) = (_v08);				\
									(gpRegICOD->_r._f09) = (_v09);				\
									(gpRegICOD->_r._f10) = (_v10);				\
									(gpRegICOD->_r._f11) = (_v11);				\
									(gpRegICOD->_r._f12) = (_v12);				\
									(gpRegICOD->_r._f13) = (_v13);				\
									(gpRegICOD->_r._f14) = (_v14);				\
									(gpRegICOD->_r._f15) = (_v15);				\
								} while(0)

#define ICOD_Wr16(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15,_f16,_v16)					\
								do { 											\
									(gpRegICOD->_r._f01) = (_v01);				\
									(gpRegICOD->_r._f02) = (_v02);				\
									(gpRegICOD->_r._f03) = (_v03);				\
									(gpRegICOD->_r._f04) = (_v04);				\
									(gpRegICOD->_r._f05) = (_v05);				\
									(gpRegICOD->_r._f06) = (_v06);				\
									(gpRegICOD->_r._f07) = (_v07);				\
									(gpRegICOD->_r._f08) = (_v08);				\
									(gpRegICOD->_r._f09) = (_v09);				\
									(gpRegICOD->_r._f10) = (_v10);				\
									(gpRegICOD->_r._f11) = (_v11);				\
									(gpRegICOD->_r._f12) = (_v12);				\
									(gpRegICOD->_r._f13) = (_v13);				\
									(gpRegICOD->_r._f14) = (_v14);				\
									(gpRegICOD->_r._f15) = (_v15);				\
									(gpRegICOD->_r._f16) = (_v16);				\
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
 * _rwname: name of rw    bit field : shall be 0 after ICOD_Wind(), 1 for ICOD_Rind()
 * _iname : name of index bit field
 * _ival  : index value
 * _fname : field name
 * _fval  : field variable that field value shall be stored.
 *
 * ICOD_Rind : General indexed register Read.(
 * ICOD_Wind : General indexed register Read.
 *
 * ICOD_Ridx : For 'index', 'rw', 'load' field name
 * ICOD_Widx : For 'index', 'rw', 'load' field name and NO_LOAD.
 */
#define ICOD_Rind(_r, _lname, _rwname, _iname, _ival, _fname, _fval)				\
							do {												\
								ICOD_Wr03(_r,_lname,0,_rwname,1,_iname,_ival);	\
								ICOD_WrFL(_r);									\
								ICOD_RdFL(_r);									\
								ICOD_Rd01(_r,_fname,_fval);						\
							} while (0)

#define ICOD_Wind(_r, _lname, _rwname, _iname, _ival, _fname, _fval)				\
				ICOD_Wr04(_r, _lname,0, _rwname,0, _iname,_ival, _fname,_fval)


#define ICOD_Ridx(_r, _ival, _fname, _fval)	ICOD_Rind(_r,load,rw,index,_ival,_fname,_fval)

#define ICOD_Widx(_r, _ival, _fname, _fval)	ICOD_Wind(_r,load,rw,index,_ival,_fname,_fval)

/** @} *//* end of macro documentation */

#ifdef __cplusplus
}
#endif

#endif	/* _ICOD_REG_H13_H_ */

/* from 'ICOD_ICOD.csv' 20111127 09:59:07   대한민국 표준시 by getregs v2.7 */
