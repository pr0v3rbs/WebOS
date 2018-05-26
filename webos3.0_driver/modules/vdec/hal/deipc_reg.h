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
 *  DEIPC register details. ( used only within kdriver )
 *
 *  author     Youngwoo Jin (youngwoo.jin@lge.com)
 *  version    1.0
 *  date       2012.03.26
 *
 */

#ifndef _DEIPC_REG_H_
#define _DEIPC_REG_H_

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
	0x0000 frame_idx ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	frame_idx                       : 8,	//  0: 7
	scaler_freeze                   : 1,	//     8
/* for B0 */
	tield_map			: 1,	//     9
	dpb_map_type			: 2;    // 10:11
} FRAME_IDX;

/*-----------------------------------------------------------------------------
	0x0004 display_info ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	display_mode                    : 2,	//  0: 1
	field_repeat                    : 1,	//     2
	vdec_paused                     : 1,	//     3
	color_sample                    : 2,	//  4: 5
	                                :10,	//  6:15 reserved
	deinter_mode                    : 3,	// 16:18
	                                :12,	// 19:30 reserved
	prog_seq                        : 1;	//    31
} DISPLAY_INFO;

/*-----------------------------------------------------------------------------
	0x0008 frame_rate ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	framerateresidual               :16,	//  0:15
	frameratediv                    :16;	// 16:31
} FRAME_RATE;

/*-----------------------------------------------------------------------------
	0x000c aspect_ratio ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	aspect_ratio                    : 4;	//  0: 3
} ASPECT_RATIO;

/*-----------------------------------------------------------------------------
	0x0010 picture size ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	height                          :16,	//  0:15
	width                           :16;	// 16:31
} PICTURE_SIZE;

/*-----------------------------------------------------------------------------
	0x0014 h_offset ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	right_offset                    :16,	//  0:15
	left_offset                     :16;	// 16:31
} H_OFFSET;

/*-----------------------------------------------------------------------------
	0x0018 v_offset ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	bottom_offset                   :16,	//  0:15
	top_offset                      :16;	// 16:31
} V_OFFSET;

/*-----------------------------------------------------------------------------
	0x001c update ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	frame_update                    : 1,	//     0
	field_update                    : 1,	//     1
	sequence_update                 : 1;	//     2
} UPDATE;

/*-----------------------------------------------------------------------------
	0x0020 y frame
base_address ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	start_addr                      :12,	//  0:11
	base_addr                       :20;	// 12:31
} Y_FRAME_BASE_ADDRESS;

/*-----------------------------------------------------------------------------
	0x0024 c frame
base_address ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	start_addr                      :12,	//  0:11
	base_addr                       :20;	// 12:31
} C_FRAME_BASE_ADDRESS;

/*-----------------------------------------------------------------------------
	0x0028 y_frame_offset ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	frame_offset                    ;   	// 31: 0
} Y_FRAME_OFFSET;

/*-----------------------------------------------------------------------------
	0x002c c_frame_offset ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	frame_offset                    ;   	// 31: 0
} C_FRAME_OFFSET;

/*-----------------------------------------------------------------------------
	0x0030 stride ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	stride                          :16,	//  0:15
	tb_gap                          :16;	// 16:31
} STRIDE;

/*-----------------------------------------------------------------------------
	0x0034 pixel aspect ratio ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	par_height                      :16,	//  0:15
	par_width                       :16;	// 16:31
} PIXEL_ASPECT_RATIO;

/*-----------------------------------------------------------------------------
	0x0038 framepackarrange ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	frame_pack_arrange              : 8,	//  0: 7
	lr_order                        : 2;	//  8: 9
} FRAME_PACK_ARRANGE;

/*-----------------------------------------------------------------------------
	0x003c pts ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pts_info                        ;   	// 31: 0
} PTS;

typedef struct {
/*	FRAME_IDX                       	frame_idx                       ;	// 0x0000 : ''
	DISPLAY_INFO                    	display_info                    ;	// 0x0004 : ''
	FRAME_RATE                      	frame_rate                      ;	// 0x0008 : ''
	ASPECT_RATIO                    	aspect_ratio                    ;	// 0x000c : ''
	PICTURE_SIZE                    	picture_size                    ;	// 0x0010 : ''
	H_OFFSET                        	h_offset                        ;	// 0x0014 : ''
	V_OFFSET                        	v_offset                        ;	// 0x0018 : ''
	UPDATE                          	update                          ;	// 0x001c : ''
	Y_FRAME_BASE_ADDRESS        		y_frame_base_address            ;	// 0x0020 : ''
	C_FRAME_BASE_ADDRESS        		c_frame_base_address            ;	// 0x0024 : ''
	Y_FRAME_OFFSET                  	y_frame_offset                  ;	// 0x0028 : ''
	C_FRAME_OFFSET                  	c_frame_offset                  ;	// 0x002c : ''
	STRIDE                          	stride                          ;	// 0x0030 : ''
	PIXEL_ASPECT_RATIO              	pixel_aspect_ratio              ;	// 0x0034 : ''
	FRAME_PACK_ARRANGE              	frame_pack_arrange              ;	// 0x0038 : ''
	PTS                             	pts                             ;	// 0x003C : ''
*/
	UINT32                          	frame_idx                       ;	// 0x0000 : ''
	UINT32                          	display_info                    ;	// 0x0004 : ''
	UINT32                          	frame_rate                      ;	// 0x0008 : ''
	UINT32                          	aspect_ratio                    ;	// 0x000c : ''
	UINT32                          	picture_size                    ;	// 0x0010 : ''
	UINT32                          	h_offset                        ;	// 0x0014 : ''
	UINT32                          	v_offset                        ;	// 0x0018 : ''
	UINT32                          	update                          ;	// 0x001c : ''
	UINT32                      		y_frame_base_address            ;	// 0x0020 : ''
	UINT32                      		c_frame_base_address            ;	// 0x0024 : ''
#if defined (CHIP_NAME_h15) || defined (CHIP_NAME_m16)
	UINT32                          	smart_bal_offset                ;	// 0x0028 : ''
	UINT32                          	reserved_0x2c	                ;	// 0x002c : ''
#else
	UINT32                          	y_frame_offset                  ;	// 0x0028 : ''
	UINT32                          	c_frame_offset                  ;	// 0x002c : ''
#endif
	UINT32                          	stride                          ;	// 0x0030 : ''
	UINT32                          	pixel_aspect_ratio              ;	// 0x0034 : ''
	UINT32                          	frame_pack_arrange              ;	// 0x0038 : ''
	UINT32                             	pts_info                        ;	// 0x003C : ''
} DEIPC_REG_T;
/* 17 regs, 17 types */

/* 17 regs, 17 types in Total*/

/*
 * @{
 * Naming for register pointer.
 * gpRealRegDEIPC : real register of DEIPC.
 * gpRegDEIPC     : shadow register.
 *
 * @def DEIPC_RdFL: Read  FLushing : Shadow <- Real.
 * @def DEIPC_WrFL: Write FLushing : Shadow -> Real.
 * @def DEIPC_Rd  : Read  whole register(UINT32) from Shadow register.
 * @def DEIPC_Wr  : Write whole register(UINT32) from Shadow register.
 * @def DEIPC_Rd01 ~ DEIPC_Rdnn: Read  given '01~nn' fields from Shadow register.
 * @def DEIPC_Wr01 ~ DEIPC_Wrnn: Write given '01~nn' fields to   Shadow register.
 * */
#define DEIPC_RdFL(_r)			((gpRegDEIPC->_r)=(gpRealRegDEIPC->_r))
#define DEIPC_WrFL(_r)			((gpRealRegDEIPC->_r)=(gpRegDEIPC->_r))

#define DEIPC_Rd(_r)			*((UINT32*)(&(gpRegDEIPC->_r)))
#define DEIPC_Wr(_r,_v)			((DEIPC_Rd(_r))=((UINT32)(_v)))

#define DEIPC_Rd01(_r,_f01,_v01)													\
								do { 											\
									(_v01) = (gpRegDEIPC->_r._f01);				\
								} while(0)

#define DEIPC_Rd02(_r,_f01,_v01,_f02,_v02)										\
								do { 											\
									(_v01) = (gpRegDEIPC->_r._f01);				\
									(_v02) = (gpRegDEIPC->_r._f02);				\
								} while(0)

#define DEIPC_Rd03(_r,_f01,_v01,_f02,_v02,_f03,_v03)								\
								do { 											\
									(_v01) = (gpRegDEIPC->_r._f01);				\
									(_v02) = (gpRegDEIPC->_r._f02);				\
									(_v03) = (gpRegDEIPC->_r._f03);				\
								} while(0)

#define DEIPC_Rd04(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04)					\
								do { 											\
									(_v01) = (gpRegDEIPC->_r._f01);				\
									(_v02) = (gpRegDEIPC->_r._f02);				\
									(_v03) = (gpRegDEIPC->_r._f03);				\
									(_v04) = (gpRegDEIPC->_r._f04);				\
								} while(0)

#define DEIPC_Rd05(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05)													\
								do { 											\
									(_v01) = (gpRegDEIPC->_r._f01);				\
									(_v02) = (gpRegDEIPC->_r._f02);				\
									(_v03) = (gpRegDEIPC->_r._f03);				\
									(_v04) = (gpRegDEIPC->_r._f04);				\
									(_v05) = (gpRegDEIPC->_r._f05);				\
								} while(0)

#define DEIPC_Rd06(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06)										\
								do { 											\
									(_v01) = (gpRegDEIPC->_r._f01);				\
									(_v02) = (gpRegDEIPC->_r._f02);				\
									(_v03) = (gpRegDEIPC->_r._f03);				\
									(_v04) = (gpRegDEIPC->_r._f04);				\
									(_v05) = (gpRegDEIPC->_r._f05);				\
									(_v06) = (gpRegDEIPC->_r._f06);				\
								} while(0)

#define DEIPC_Rd07(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07)								\
								do { 											\
									(_v01) = (gpRegDEIPC->_r._f01);				\
									(_v02) = (gpRegDEIPC->_r._f02);				\
									(_v03) = (gpRegDEIPC->_r._f03);				\
									(_v04) = (gpRegDEIPC->_r._f04);				\
									(_v05) = (gpRegDEIPC->_r._f05);				\
									(_v06) = (gpRegDEIPC->_r._f06);				\
									(_v07) = (gpRegDEIPC->_r._f07);				\
								} while(0)

#define DEIPC_Rd08(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08)					\
								do { 											\
									(_v01) = (gpRegDEIPC->_r._f01);				\
									(_v02) = (gpRegDEIPC->_r._f02);				\
									(_v03) = (gpRegDEIPC->_r._f03);				\
									(_v04) = (gpRegDEIPC->_r._f04);				\
									(_v05) = (gpRegDEIPC->_r._f05);				\
									(_v06) = (gpRegDEIPC->_r._f06);				\
									(_v07) = (gpRegDEIPC->_r._f07);				\
									(_v08) = (gpRegDEIPC->_r._f08);				\
								} while(0)

#define DEIPC_Rd09(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09)													\
								do { 											\
									(_v01) = (gpRegDEIPC->_r._f01);				\
									(_v02) = (gpRegDEIPC->_r._f02);				\
									(_v03) = (gpRegDEIPC->_r._f03);				\
									(_v04) = (gpRegDEIPC->_r._f04);				\
									(_v05) = (gpRegDEIPC->_r._f05);				\
									(_v06) = (gpRegDEIPC->_r._f06);				\
									(_v07) = (gpRegDEIPC->_r._f07);				\
									(_v08) = (gpRegDEIPC->_r._f08);				\
									(_v09) = (gpRegDEIPC->_r._f09);				\
								} while(0)

#define DEIPC_Rd10(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10)										\
								do { 											\
									(_v01) = (gpRegDEIPC->_r._f01);				\
									(_v02) = (gpRegDEIPC->_r._f02);				\
									(_v03) = (gpRegDEIPC->_r._f03);				\
									(_v04) = (gpRegDEIPC->_r._f04);				\
									(_v05) = (gpRegDEIPC->_r._f05);				\
									(_v06) = (gpRegDEIPC->_r._f06);				\
									(_v07) = (gpRegDEIPC->_r._f07);				\
									(_v08) = (gpRegDEIPC->_r._f08);				\
									(_v09) = (gpRegDEIPC->_r._f09);				\
									(_v10) = (gpRegDEIPC->_r._f10);				\
								} while(0)

#define DEIPC_Rd11(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11)								\
								do { 											\
									(_v01) = (gpRegDEIPC->_r._f01);				\
									(_v02) = (gpRegDEIPC->_r._f02);				\
									(_v03) = (gpRegDEIPC->_r._f03);				\
									(_v04) = (gpRegDEIPC->_r._f04);				\
									(_v05) = (gpRegDEIPC->_r._f05);				\
									(_v06) = (gpRegDEIPC->_r._f06);				\
									(_v07) = (gpRegDEIPC->_r._f07);				\
									(_v08) = (gpRegDEIPC->_r._f08);				\
									(_v09) = (gpRegDEIPC->_r._f09);				\
									(_v10) = (gpRegDEIPC->_r._f10);				\
									(_v11) = (gpRegDEIPC->_r._f11);				\
								} while(0)

#define DEIPC_Rd12(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12)					\
								do { 											\
									(_v01) = (gpRegDEIPC->_r._f01);				\
									(_v02) = (gpRegDEIPC->_r._f02);				\
									(_v03) = (gpRegDEIPC->_r._f03);				\
									(_v04) = (gpRegDEIPC->_r._f04);				\
									(_v05) = (gpRegDEIPC->_r._f05);				\
									(_v06) = (gpRegDEIPC->_r._f06);				\
									(_v07) = (gpRegDEIPC->_r._f07);				\
									(_v08) = (gpRegDEIPC->_r._f08);				\
									(_v09) = (gpRegDEIPC->_r._f09);				\
									(_v10) = (gpRegDEIPC->_r._f10);				\
									(_v11) = (gpRegDEIPC->_r._f11);				\
									(_v12) = (gpRegDEIPC->_r._f12);				\
								} while(0)

#define DEIPC_Rd13(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13)													\
								do { 											\
									(_v01) = (gpRegDEIPC->_r._f01);				\
									(_v02) = (gpRegDEIPC->_r._f02);				\
									(_v03) = (gpRegDEIPC->_r._f03);				\
									(_v04) = (gpRegDEIPC->_r._f04);				\
									(_v05) = (gpRegDEIPC->_r._f05);				\
									(_v06) = (gpRegDEIPC->_r._f06);				\
									(_v07) = (gpRegDEIPC->_r._f07);				\
									(_v08) = (gpRegDEIPC->_r._f08);				\
									(_v09) = (gpRegDEIPC->_r._f09);				\
									(_v10) = (gpRegDEIPC->_r._f10);				\
									(_v11) = (gpRegDEIPC->_r._f11);				\
									(_v12) = (gpRegDEIPC->_r._f12);				\
									(_v13) = (gpRegDEIPC->_r._f13);				\
								} while(0)

#define DEIPC_Rd14(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14)										\
								do { 											\
									(_v01) = (gpRegDEIPC->_r._f01);				\
									(_v02) = (gpRegDEIPC->_r._f02);				\
									(_v03) = (gpRegDEIPC->_r._f03);				\
									(_v04) = (gpRegDEIPC->_r._f04);				\
									(_v05) = (gpRegDEIPC->_r._f05);				\
									(_v06) = (gpRegDEIPC->_r._f06);				\
									(_v07) = (gpRegDEIPC->_r._f07);				\
									(_v08) = (gpRegDEIPC->_r._f08);				\
									(_v09) = (gpRegDEIPC->_r._f09);				\
									(_v10) = (gpRegDEIPC->_r._f10);				\
									(_v11) = (gpRegDEIPC->_r._f11);				\
									(_v12) = (gpRegDEIPC->_r._f12);				\
									(_v13) = (gpRegDEIPC->_r._f13);				\
									(_v14) = (gpRegDEIPC->_r._f14);				\
								} while(0)

#define DEIPC_Rd15(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15)								\
								do { 											\
									(_v01) = (gpRegDEIPC->_r._f01);				\
									(_v02) = (gpRegDEIPC->_r._f02);				\
									(_v03) = (gpRegDEIPC->_r._f03);				\
									(_v04) = (gpRegDEIPC->_r._f04);				\
									(_v05) = (gpRegDEIPC->_r._f05);				\
									(_v06) = (gpRegDEIPC->_r._f06);				\
									(_v07) = (gpRegDEIPC->_r._f07);				\
									(_v08) = (gpRegDEIPC->_r._f08);				\
									(_v09) = (gpRegDEIPC->_r._f09);				\
									(_v10) = (gpRegDEIPC->_r._f10);				\
									(_v11) = (gpRegDEIPC->_r._f11);				\
									(_v12) = (gpRegDEIPC->_r._f12);				\
									(_v13) = (gpRegDEIPC->_r._f13);				\
									(_v14) = (gpRegDEIPC->_r._f14);				\
									(_v15) = (gpRegDEIPC->_r._f15);				\
								} while(0)

#define DEIPC_Rd16(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15,_f16,_v16)					\
								do { 											\
									(_v01) = (gpRegDEIPC->_r._f01);				\
									(_v02) = (gpRegDEIPC->_r._f02);				\
									(_v03) = (gpRegDEIPC->_r._f03);				\
									(_v04) = (gpRegDEIPC->_r._f04);				\
									(_v05) = (gpRegDEIPC->_r._f05);				\
									(_v06) = (gpRegDEIPC->_r._f06);				\
									(_v07) = (gpRegDEIPC->_r._f07);				\
									(_v08) = (gpRegDEIPC->_r._f08);				\
									(_v09) = (gpRegDEIPC->_r._f09);				\
									(_v10) = (gpRegDEIPC->_r._f10);				\
									(_v11) = (gpRegDEIPC->_r._f11);				\
									(_v12) = (gpRegDEIPC->_r._f12);				\
									(_v13) = (gpRegDEIPC->_r._f13);				\
									(_v14) = (gpRegDEIPC->_r._f14);				\
									(_v15) = (gpRegDEIPC->_r._f15);				\
									(_v16) = (gpRegDEIPC->_r._f16);				\
								} while(0)


#define DEIPC_Wr01(_r,_f01,_v01)													\
								do { 											\
									(gpRegDEIPC->_r._f01) = (_v01);				\
								} while(0)

#define DEIPC_Wr02(_r,_f01,_v01,_f02,_v02)										\
								do { 											\
									(gpRegDEIPC->_r._f01) = (_v01);				\
									(gpRegDEIPC->_r._f02) = (_v02);				\
								} while(0)

#define DEIPC_Wr03(_r,_f01,_v01,_f02,_v02,_f03,_v03)								\
								do { 											\
									(gpRegDEIPC->_r._f01) = (_v01);				\
									(gpRegDEIPC->_r._f02) = (_v02);				\
									(gpRegDEIPC->_r._f03) = (_v03);				\
								} while(0)

#define DEIPC_Wr04(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04)					\
								do { 											\
									(gpRegDEIPC->_r._f01) = (_v01);				\
									(gpRegDEIPC->_r._f02) = (_v02);				\
									(gpRegDEIPC->_r._f03) = (_v03);				\
									(gpRegDEIPC->_r._f04) = (_v04);				\
								} while(0)

#define DEIPC_Wr05(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05)													\
								do { 											\
									(gpRegDEIPC->_r._f01) = (_v01);				\
									(gpRegDEIPC->_r._f02) = (_v02);				\
									(gpRegDEIPC->_r._f03) = (_v03);				\
									(gpRegDEIPC->_r._f04) = (_v04);				\
									(gpRegDEIPC->_r._f05) = (_v05);				\
								} while(0)

#define DEIPC_Wr06(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06)										\
								do { 											\
									(gpRegDEIPC->_r._f01) = (_v01);				\
									(gpRegDEIPC->_r._f02) = (_v02);				\
									(gpRegDEIPC->_r._f03) = (_v03);				\
									(gpRegDEIPC->_r._f04) = (_v04);				\
									(gpRegDEIPC->_r._f05) = (_v05);				\
									(gpRegDEIPC->_r._f06) = (_v06);				\
								} while(0)

#define DEIPC_Wr07(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07)								\
								do { 											\
									(gpRegDEIPC->_r._f01) = (_v01);				\
									(gpRegDEIPC->_r._f02) = (_v02);				\
									(gpRegDEIPC->_r._f03) = (_v03);				\
									(gpRegDEIPC->_r._f04) = (_v04);				\
									(gpRegDEIPC->_r._f05) = (_v05);				\
									(gpRegDEIPC->_r._f06) = (_v06);				\
									(gpRegDEIPC->_r._f07) = (_v07);				\
								} while(0)

#define DEIPC_Wr08(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08)					\
								do { 											\
									(gpRegDEIPC->_r._f01) = (_v01);				\
									(gpRegDEIPC->_r._f02) = (_v02);				\
									(gpRegDEIPC->_r._f03) = (_v03);				\
									(gpRegDEIPC->_r._f04) = (_v04);				\
									(gpRegDEIPC->_r._f05) = (_v05);				\
									(gpRegDEIPC->_r._f06) = (_v06);				\
									(gpRegDEIPC->_r._f07) = (_v07);				\
									(gpRegDEIPC->_r._f08) = (_v08);				\
								} while(0)

#define DEIPC_Wr09(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09)													\
								do { 											\
									(gpRegDEIPC->_r._f01) = (_v01);				\
									(gpRegDEIPC->_r._f02) = (_v02);				\
									(gpRegDEIPC->_r._f03) = (_v03);				\
									(gpRegDEIPC->_r._f04) = (_v04);				\
									(gpRegDEIPC->_r._f05) = (_v05);				\
									(gpRegDEIPC->_r._f06) = (_v06);				\
									(gpRegDEIPC->_r._f07) = (_v07);				\
									(gpRegDEIPC->_r._f08) = (_v08);				\
									(gpRegDEIPC->_r._f09) = (_v09);				\
								} while(0)

#define DEIPC_Wr10(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10)										\
								do { 											\
									(gpRegDEIPC->_r._f01) = (_v01);				\
									(gpRegDEIPC->_r._f02) = (_v02);				\
									(gpRegDEIPC->_r._f03) = (_v03);				\
									(gpRegDEIPC->_r._f04) = (_v04);				\
									(gpRegDEIPC->_r._f05) = (_v05);				\
									(gpRegDEIPC->_r._f06) = (_v06);				\
									(gpRegDEIPC->_r._f07) = (_v07);				\
									(gpRegDEIPC->_r._f08) = (_v08);				\
									(gpRegDEIPC->_r._f09) = (_v09);				\
									(gpRegDEIPC->_r._f10) = (_v10);				\
								} while(0)

#define DEIPC_Wr11(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11)								\
								do { 											\
									(gpRegDEIPC->_r._f01) = (_v01);				\
									(gpRegDEIPC->_r._f02) = (_v02);				\
									(gpRegDEIPC->_r._f03) = (_v03);				\
									(gpRegDEIPC->_r._f04) = (_v04);				\
									(gpRegDEIPC->_r._f05) = (_v05);				\
									(gpRegDEIPC->_r._f06) = (_v06);				\
									(gpRegDEIPC->_r._f07) = (_v07);				\
									(gpRegDEIPC->_r._f08) = (_v08);				\
									(gpRegDEIPC->_r._f09) = (_v09);				\
									(gpRegDEIPC->_r._f10) = (_v10);				\
									(gpRegDEIPC->_r._f11) = (_v11);				\
								} while(0)

#define DEIPC_Wr12(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12)					\
								do { 											\
									(gpRegDEIPC->_r._f01) = (_v01);				\
									(gpRegDEIPC->_r._f02) = (_v02);				\
									(gpRegDEIPC->_r._f03) = (_v03);				\
									(gpRegDEIPC->_r._f04) = (_v04);				\
									(gpRegDEIPC->_r._f05) = (_v05);				\
									(gpRegDEIPC->_r._f06) = (_v06);				\
									(gpRegDEIPC->_r._f07) = (_v07);				\
									(gpRegDEIPC->_r._f08) = (_v08);				\
									(gpRegDEIPC->_r._f09) = (_v09);				\
									(gpRegDEIPC->_r._f10) = (_v10);				\
									(gpRegDEIPC->_r._f11) = (_v11);				\
									(gpRegDEIPC->_r._f12) = (_v12);				\
								} while(0)

#define DEIPC_Wr13(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13)													\
								do { 											\
									(gpRegDEIPC->_r._f01) = (_v01);				\
									(gpRegDEIPC->_r._f02) = (_v02);				\
									(gpRegDEIPC->_r._f03) = (_v03);				\
									(gpRegDEIPC->_r._f04) = (_v04);				\
									(gpRegDEIPC->_r._f05) = (_v05);				\
									(gpRegDEIPC->_r._f06) = (_v06);				\
									(gpRegDEIPC->_r._f07) = (_v07);				\
									(gpRegDEIPC->_r._f08) = (_v08);				\
									(gpRegDEIPC->_r._f09) = (_v09);				\
									(gpRegDEIPC->_r._f10) = (_v10);				\
									(gpRegDEIPC->_r._f11) = (_v11);				\
									(gpRegDEIPC->_r._f12) = (_v12);				\
									(gpRegDEIPC->_r._f13) = (_v13);				\
								} while(0)

#define DEIPC_Wr14(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14)										\
								do { 											\
									(gpRegDEIPC->_r._f01) = (_v01);				\
									(gpRegDEIPC->_r._f02) = (_v02);				\
									(gpRegDEIPC->_r._f03) = (_v03);				\
									(gpRegDEIPC->_r._f04) = (_v04);				\
									(gpRegDEIPC->_r._f05) = (_v05);				\
									(gpRegDEIPC->_r._f06) = (_v06);				\
									(gpRegDEIPC->_r._f07) = (_v07);				\
									(gpRegDEIPC->_r._f08) = (_v08);				\
									(gpRegDEIPC->_r._f09) = (_v09);				\
									(gpRegDEIPC->_r._f10) = (_v10);				\
									(gpRegDEIPC->_r._f11) = (_v11);				\
									(gpRegDEIPC->_r._f12) = (_v12);				\
									(gpRegDEIPC->_r._f13) = (_v13);				\
									(gpRegDEIPC->_r._f14) = (_v14);				\
								} while(0)

#define DEIPC_Wr15(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15)								\
								do { 											\
									(gpRegDEIPC->_r._f01) = (_v01);				\
									(gpRegDEIPC->_r._f02) = (_v02);				\
									(gpRegDEIPC->_r._f03) = (_v03);				\
									(gpRegDEIPC->_r._f04) = (_v04);				\
									(gpRegDEIPC->_r._f05) = (_v05);				\
									(gpRegDEIPC->_r._f06) = (_v06);				\
									(gpRegDEIPC->_r._f07) = (_v07);				\
									(gpRegDEIPC->_r._f08) = (_v08);				\
									(gpRegDEIPC->_r._f09) = (_v09);				\
									(gpRegDEIPC->_r._f10) = (_v10);				\
									(gpRegDEIPC->_r._f11) = (_v11);				\
									(gpRegDEIPC->_r._f12) = (_v12);				\
									(gpRegDEIPC->_r._f13) = (_v13);				\
									(gpRegDEIPC->_r._f14) = (_v14);				\
									(gpRegDEIPC->_r._f15) = (_v15);				\
								} while(0)

#define DEIPC_Wr16(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15,_f16,_v16)					\
								do { 											\
									(gpRegDEIPC->_r._f01) = (_v01);				\
									(gpRegDEIPC->_r._f02) = (_v02);				\
									(gpRegDEIPC->_r._f03) = (_v03);				\
									(gpRegDEIPC->_r._f04) = (_v04);				\
									(gpRegDEIPC->_r._f05) = (_v05);				\
									(gpRegDEIPC->_r._f06) = (_v06);				\
									(gpRegDEIPC->_r._f07) = (_v07);				\
									(gpRegDEIPC->_r._f08) = (_v08);				\
									(gpRegDEIPC->_r._f09) = (_v09);				\
									(gpRegDEIPC->_r._f10) = (_v10);				\
									(gpRegDEIPC->_r._f11) = (_v11);				\
									(gpRegDEIPC->_r._f12) = (_v12);				\
									(gpRegDEIPC->_r._f13) = (_v13);				\
									(gpRegDEIPC->_r._f14) = (_v14);				\
									(gpRegDEIPC->_r._f15) = (_v15);				\
									(gpRegDEIPC->_r._f16) = (_v16);				\
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
 * _rwname: name of rw    bit field : shall be 0 after DEIPC_Wind(), 1 for DEIPC_Rind()
 * _iname : name of index bit field
 * _ival  : index value
 * _fname : field name
 * _fval  : field variable that field value shall be stored.
 *
 * DEIPC_Rind : General indexed register Read.(
 * DEIPC_Wind : General indexed register Read.
 *
 * DEIPC_Ridx : For 'index', 'rw', 'load' field name
 * DEIPC_Widx : For 'index', 'rw', 'load' field name and NO_LOAD.
 */
#define DEIPC_Rind(_r, _lname, _rwname, _iname, _ival, _fname, _fval)				\
							do {												\
								DEIPC_Wr03(_r,_lname,0,_rwname,1,_iname,_ival);	\
								DEIPC_WrFL(_r);									\
								DEIPC_RdFL(_r);									\
								DEIPC_Rd01(_r,_fname,_fval);						\
							} while (0)

#define DEIPC_Wind(_r, _lname, _rwname, _iname, _ival, _fname, _fval)				\
				DEIPC_Wr04(_r, _lname,0, _rwname,0, _iname,_ival, _fname,_fval)


#define DEIPC_Ridx(_r, _ival, _fname, _fval)	DEIPC_Rind(_r,load,rw,index,_ival,_fname,_fval)

#define DEIPC_Widx(_r, _ival, _fname, _fval)	DEIPC_Wind(_r,load,rw,index,_ival,_fname,_fval)

/** @} *//* end of macro documentation */

#ifdef __cplusplus
}
#endif

#endif	/* _DEIPC_REG_H_ */

/* from 'deipc-REG.csv' 20120326 19:07:48   대한민국 표준시 by getregs v2.7 */
