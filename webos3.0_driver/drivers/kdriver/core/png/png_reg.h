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
 *  PNG register details. ( used only within kdriver )
 *
 *  author     Youngwoo Jin (youngwoo.jin@lge.com)
 *  version    1.0
 *  date       2011.11.27
 *
 */

#ifndef _PNG_REG_H13_H_
#define _PNG_REG_H13_H_

/*----------------------------------------------------------------------------------------
    Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include "os_util.h"


#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------------
	0x0000 cmd ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	start                           : 1,	//     0
	                                : 7,	//  1: 7 reserved
	mid_start                       : 1;	//     8
} CMD;

/*-----------------------------------------------------------------------------
	0x0004 read_base_addr ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x0008 read_byte_size ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x000c write_base_addr ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x0010 image_info1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	image_height                    :16,	//  0:15
	image_width                     :16;	// 16:31
} IMAGE_INFO1;

/*-----------------------------------------------------------------------------
	0x0014 image_info2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	bitdepth                        : 5,	//  0: 4
	                                : 3,	//  5: 7 reserved
	color_type                      : 3,	//  8:10
	                                : 5,	// 11:15 reserved
	out_format                      : 2;	// 16:17
} IMAGE_INFO2;

/*-----------------------------------------------------------------------------
	0x0018 intr_enable ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	png_done_en                     : 1,	//     0
	inflate_done_en                 : 1,	//     1
	tcopt_err_en                    : 1,	//     2
	tlen_err_en                     : 1,	//     3
	tgen_err_en                     : 1,	//     4
	win_err_en                      : 1,	//     5
	code_err_en                     : 1,	//     6
	btype_err_en                    : 1,	//     7
	len_err_en                      : 1,	//     8
	row_err_en                      : 1,	//     9
	ftype_err_en                    : 1,	//    10
	read_done_en                    : 1,	//    11
	time_out_en                     : 1;	//    12
} INTR_ENABLE;

/*-----------------------------------------------------------------------------
	0x001c intr_status ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	png_done                        : 1,	//     0
	inflate_done                    : 1,	//     1
	tcopy_err                       : 1,	//     2
	tlen_err                        : 1,	//     3
	tgen_err                        : 1,	//     4
	win_err                         : 1,	//     5
	code_err                        : 1,	//     6
	btype_err                       : 1,	//     7
	len_err                         : 1,	//     8
	row_err                         : 1,	//     9
	ftype_err                       : 1,	//    10
	read_done                       : 1,	//    11
	time_out                        : 1;	//    12
} INTR_STATUS;

/*-----------------------------------------------------------------------------
	0x0020 intr_clear ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	png_done_clear                  : 1,	//     0
	inflate_done_clear              : 1,	//     1
	tcopt_err_clear                 : 1,	//     2
	tlen_err_clear                  : 1,	//     3
	tgen_err_clear                  : 1,	//     4
	win_err_clear                   : 1,	//     5
	code_err_clear                  : 1,	//     6
	btype_err_clear                 : 1,	//     7
	len_err_clear                   : 1,	//     8
	row_err_clear                   : 1,	//     9
	ftype_err_clear                 : 1,	//    10
	read_done_clear                 : 1,	//    11
	time_out_clear                  : 1;	//    12
} INTR_CLEAR;

/*-----------------------------------------------------------------------------
	0x0024 read_gmau_config ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	cmd_rpri2                       : 4,	//  0: 3
	cmd_rpri1                       : 4,	//  4: 7
	cmd_rdly                        :16,	//  8:23
	cmd_rtxid                       : 4;	// 24:27
} READ_GMAU_CONFIG;

/*-----------------------------------------------------------------------------
	0x0028 write_gmau_config ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	cmd_wpri2                       : 4,	//  0: 3
	cmd_wpri1                       : 4,	//  4: 7
	cmd_wdly                        :16,	//  8:23
	cmd_wtxid                       : 4;	// 24:27
} WRITE_GMAU_CONFIG;

/*-----------------------------------------------------------------------------
	0x002c inflate_config ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	window_size                     : 3;	//  0: 2
} INFLATE_CONFIG;

/*-----------------------------------------------------------------------------
	0x0030 fifo_status ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	in_fifo_valid_bits              : 8,	//  0: 7
	                                : 4,	//  8:11 reserved
	r_fifo_space                    : 5;	// 12:16
} FIFO_STATUS;

/*-----------------------------------------------------------------------------
	0x0034 inflate_status ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	main_state                      : 4,	//  0: 3
	                                : 4,	//  4: 7 reserved
	table_state                     : 4,	//  8:11
	                                : 4,	// 12:15 reserved
	inflate_state                   : 4;	// 16:19
} INFLATE_STATUS;

/*-----------------------------------------------------------------------------
	0x0038 cur_read_addr ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x003c cur_ftr_wptr ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x0040 ftr_wptr_bnd ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x0044 cur_write_addr ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x0048 ftr_sl_cnt_msb ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x004c ftr_sl_cnt_lsb ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x0050 ftr_in_cnt_msb ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x0054 ftr_in_cnt_lsb ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x0058 ftr_type ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	ftr_type                        : 8;	//  0: 7
} FTR_TYPE;

/*-----------------------------------------------------------------------------
	0x005c ftr_status ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	filter_done                     : 1,	//     0
	                                : 3,	//  1: 3 reserved
	filter_last_line                : 1,	//     4
	inflate_done                    : 1;	//     5
} FTR_STATUS;

/*-----------------------------------------------------------------------------
	0x0060 time_out_mask ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x0064 cur_time_cnt ''
------------------------------------------------------------------------------*/
/*	no field */

typedef struct {
	CMD                             	cmd                             ;	// 0x0000 : ''
	UINT32                          	read_base_addr                  ;	// 0x0004 : ''
	UINT32                          	read_byte_size                  ;	// 0x0008 : ''
	UINT32                          	write_base_addr                 ;	// 0x000c : ''
	IMAGE_INFO1                     	image_info1                     ;	// 0x0010 : ''
	IMAGE_INFO2                     	image_info2                     ;	// 0x0014 : ''
	INTR_ENABLE                     	intr_enable                     ;	// 0x0018 : ''
	INTR_STATUS                     	intr_status                     ;	// 0x001c : ''
	INTR_CLEAR                      	intr_clear                      ;	// 0x0020 : ''
	READ_GMAU_CONFIG                	read_gmau_config                ;	// 0x0024 : ''
	WRITE_GMAU_CONFIG               	write_gmau_config               ;	// 0x0028 : ''
	INFLATE_CONFIG                  	inflate_config                  ;	// 0x002c : ''
	FIFO_STATUS                     	fifo_status                     ;	// 0x0030 : ''
	INFLATE_STATUS                  	inflate_status                  ;	// 0x0034 : ''
	UINT32                          	cur_read_addr                   ;	// 0x0038 : ''
	UINT32                          	cur_ftr_wptr                    ;	// 0x003c : ''
	UINT32                          	ftr_wptr_bnd                    ;	// 0x0040 : ''
	UINT32                          	cur_write_addr                  ;	// 0x0044 : ''
	UINT32                          	ftr_sl_cnt_msb                  ;	// 0x0048 : ''
	UINT32                          	ftr_sl_cnt_lsb                  ;	// 0x004c : ''
	UINT32                          	ftr_in_cnt_msb                  ;	// 0x0050 : ''
	UINT32                          	ftr_in_cnt_lsb                  ;	// 0x0054 : ''
	FTR_TYPE                        	ftr_type                        ;	// 0x0058 : ''
	FTR_STATUS                      	ftr_status                      ;	// 0x005c : ''
	UINT32                          	time_out_mask                   ;	// 0x0060 : ''
	UINT32                          	cur_time_cnt                    ;	// 0x0064 : ''

	UINT32								debug_rsvd_1;					;	// 0x0068
	UINT32								debug_valid_chk;				;	// 0x006c
	UINT32								debug_rd_addr;					;	// 0x0070
	UINT32								debug_rd_len;					;	// 0x0074
	UINT32								debug_wr_addr;					;	// 0x0078
	UINT32								debug_wr_len;					;	// 0x006c
} PNG_REG_T;
/* 26 regs, 26 types */

/* 26 regs, 26 types in Total*/

/*
 * @{
 * Naming for register pointer.
 * gpRealRegPNG : real register of PNG.
 * gpRegPNG     : shadow register.
 *
 * @def PNG_RdFL: Read  FLushing : Shadow <- Real.
 * @def PNG_WrFL: Write FLushing : Shadow -> Real.
 * @def PNG_Rd  : Read  whole register(UINT32) from Shadow register.
 * @def PNG_Wr  : Write whole register(UINT32) from Shadow register.
 * @def PNG_Rd01 ~ PNG_Rdnn: Read  given '01~nn' fields from Shadow register.
 * @def PNG_Wr01 ~ PNG_Wrnn: Write given '01~nn' fields to   Shadow register.
 *
 * @def PNG_Rd00: access to direct field 
 * */
#define PNG_RdFL(_r)			((gpRegPNG->_r)=(gpRealRegPNG->_r))
#define PNG_WrFL(_r)			((gpRealRegPNG->_r)=(gpRegPNG->_r))

#define PNG_Rd(_r)				*((UINT32*)(&(gpRegPNG->_r)))
#define PNG_Wr(_r,_v)			((PNG_Rd(_r))=((UINT32)(_v)))

#define PNG_Rd00(_r,_f) 		gpRegPNG->_r._f

#define PNG_Rd01(_r,_f01,_v01)													\
								do { 											\
									(_v01) = (gpRegPNG->_r._f01);				\
								} while(0)

#define PNG_Rd02(_r,_f01,_v01,_f02,_v02)										\
								do { 											\
									(_v01) = (gpRegPNG->_r._f01);				\
									(_v02) = (gpRegPNG->_r._f02);				\
								} while(0)

#define PNG_Rd03(_r,_f01,_v01,_f02,_v02,_f03,_v03)								\
								do { 											\
									(_v01) = (gpRegPNG->_r._f01);				\
									(_v02) = (gpRegPNG->_r._f02);				\
									(_v03) = (gpRegPNG->_r._f03);				\
								} while(0)

#define PNG_Rd04(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04)					\
								do { 											\
									(_v01) = (gpRegPNG->_r._f01);				\
									(_v02) = (gpRegPNG->_r._f02);				\
									(_v03) = (gpRegPNG->_r._f03);				\
									(_v04) = (gpRegPNG->_r._f04);				\
								} while(0)

#define PNG_Rd05(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05)													\
								do { 											\
									(_v01) = (gpRegPNG->_r._f01);				\
									(_v02) = (gpRegPNG->_r._f02);				\
									(_v03) = (gpRegPNG->_r._f03);				\
									(_v04) = (gpRegPNG->_r._f04);				\
									(_v05) = (gpRegPNG->_r._f05);				\
								} while(0)

#define PNG_Rd06(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06)										\
								do { 											\
									(_v01) = (gpRegPNG->_r._f01);				\
									(_v02) = (gpRegPNG->_r._f02);				\
									(_v03) = (gpRegPNG->_r._f03);				\
									(_v04) = (gpRegPNG->_r._f04);				\
									(_v05) = (gpRegPNG->_r._f05);				\
									(_v06) = (gpRegPNG->_r._f06);				\
								} while(0)

#define PNG_Rd07(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07)								\
								do { 											\
									(_v01) = (gpRegPNG->_r._f01);				\
									(_v02) = (gpRegPNG->_r._f02);				\
									(_v03) = (gpRegPNG->_r._f03);				\
									(_v04) = (gpRegPNG->_r._f04);				\
									(_v05) = (gpRegPNG->_r._f05);				\
									(_v06) = (gpRegPNG->_r._f06);				\
									(_v07) = (gpRegPNG->_r._f07);				\
								} while(0)

#define PNG_Rd08(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08)					\
								do { 											\
									(_v01) = (gpRegPNG->_r._f01);				\
									(_v02) = (gpRegPNG->_r._f02);				\
									(_v03) = (gpRegPNG->_r._f03);				\
									(_v04) = (gpRegPNG->_r._f04);				\
									(_v05) = (gpRegPNG->_r._f05);				\
									(_v06) = (gpRegPNG->_r._f06);				\
									(_v07) = (gpRegPNG->_r._f07);				\
									(_v08) = (gpRegPNG->_r._f08);				\
								} while(0)

#define PNG_Rd09(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09)													\
								do { 											\
									(_v01) = (gpRegPNG->_r._f01);				\
									(_v02) = (gpRegPNG->_r._f02);				\
									(_v03) = (gpRegPNG->_r._f03);				\
									(_v04) = (gpRegPNG->_r._f04);				\
									(_v05) = (gpRegPNG->_r._f05);				\
									(_v06) = (gpRegPNG->_r._f06);				\
									(_v07) = (gpRegPNG->_r._f07);				\
									(_v08) = (gpRegPNG->_r._f08);				\
									(_v09) = (gpRegPNG->_r._f09);				\
								} while(0)

#define PNG_Rd10(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10)										\
								do { 											\
									(_v01) = (gpRegPNG->_r._f01);				\
									(_v02) = (gpRegPNG->_r._f02);				\
									(_v03) = (gpRegPNG->_r._f03);				\
									(_v04) = (gpRegPNG->_r._f04);				\
									(_v05) = (gpRegPNG->_r._f05);				\
									(_v06) = (gpRegPNG->_r._f06);				\
									(_v07) = (gpRegPNG->_r._f07);				\
									(_v08) = (gpRegPNG->_r._f08);				\
									(_v09) = (gpRegPNG->_r._f09);				\
									(_v10) = (gpRegPNG->_r._f10);				\
								} while(0)

#define PNG_Rd11(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11)								\
								do { 											\
									(_v01) = (gpRegPNG->_r._f01);				\
									(_v02) = (gpRegPNG->_r._f02);				\
									(_v03) = (gpRegPNG->_r._f03);				\
									(_v04) = (gpRegPNG->_r._f04);				\
									(_v05) = (gpRegPNG->_r._f05);				\
									(_v06) = (gpRegPNG->_r._f06);				\
									(_v07) = (gpRegPNG->_r._f07);				\
									(_v08) = (gpRegPNG->_r._f08);				\
									(_v09) = (gpRegPNG->_r._f09);				\
									(_v10) = (gpRegPNG->_r._f10);				\
									(_v11) = (gpRegPNG->_r._f11);				\
								} while(0)

#define PNG_Rd12(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12)					\
								do { 											\
									(_v01) = (gpRegPNG->_r._f01);				\
									(_v02) = (gpRegPNG->_r._f02);				\
									(_v03) = (gpRegPNG->_r._f03);				\
									(_v04) = (gpRegPNG->_r._f04);				\
									(_v05) = (gpRegPNG->_r._f05);				\
									(_v06) = (gpRegPNG->_r._f06);				\
									(_v07) = (gpRegPNG->_r._f07);				\
									(_v08) = (gpRegPNG->_r._f08);				\
									(_v09) = (gpRegPNG->_r._f09);				\
									(_v10) = (gpRegPNG->_r._f10);				\
									(_v11) = (gpRegPNG->_r._f11);				\
									(_v12) = (gpRegPNG->_r._f12);				\
								} while(0)

#define PNG_Rd13(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13)													\
								do { 											\
									(_v01) = (gpRegPNG->_r._f01);				\
									(_v02) = (gpRegPNG->_r._f02);				\
									(_v03) = (gpRegPNG->_r._f03);				\
									(_v04) = (gpRegPNG->_r._f04);				\
									(_v05) = (gpRegPNG->_r._f05);				\
									(_v06) = (gpRegPNG->_r._f06);				\
									(_v07) = (gpRegPNG->_r._f07);				\
									(_v08) = (gpRegPNG->_r._f08);				\
									(_v09) = (gpRegPNG->_r._f09);				\
									(_v10) = (gpRegPNG->_r._f10);				\
									(_v11) = (gpRegPNG->_r._f11);				\
									(_v12) = (gpRegPNG->_r._f12);				\
									(_v13) = (gpRegPNG->_r._f13);				\
								} while(0)

#define PNG_Rd14(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14)										\
								do { 											\
									(_v01) = (gpRegPNG->_r._f01);				\
									(_v02) = (gpRegPNG->_r._f02);				\
									(_v03) = (gpRegPNG->_r._f03);				\
									(_v04) = (gpRegPNG->_r._f04);				\
									(_v05) = (gpRegPNG->_r._f05);				\
									(_v06) = (gpRegPNG->_r._f06);				\
									(_v07) = (gpRegPNG->_r._f07);				\
									(_v08) = (gpRegPNG->_r._f08);				\
									(_v09) = (gpRegPNG->_r._f09);				\
									(_v10) = (gpRegPNG->_r._f10);				\
									(_v11) = (gpRegPNG->_r._f11);				\
									(_v12) = (gpRegPNG->_r._f12);				\
									(_v13) = (gpRegPNG->_r._f13);				\
									(_v14) = (gpRegPNG->_r._f14);				\
								} while(0)

#define PNG_Rd15(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15)								\
								do { 											\
									(_v01) = (gpRegPNG->_r._f01);				\
									(_v02) = (gpRegPNG->_r._f02);				\
									(_v03) = (gpRegPNG->_r._f03);				\
									(_v04) = (gpRegPNG->_r._f04);				\
									(_v05) = (gpRegPNG->_r._f05);				\
									(_v06) = (gpRegPNG->_r._f06);				\
									(_v07) = (gpRegPNG->_r._f07);				\
									(_v08) = (gpRegPNG->_r._f08);				\
									(_v09) = (gpRegPNG->_r._f09);				\
									(_v10) = (gpRegPNG->_r._f10);				\
									(_v11) = (gpRegPNG->_r._f11);				\
									(_v12) = (gpRegPNG->_r._f12);				\
									(_v13) = (gpRegPNG->_r._f13);				\
									(_v14) = (gpRegPNG->_r._f14);				\
									(_v15) = (gpRegPNG->_r._f15);				\
								} while(0)

#define PNG_Rd16(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15,_f16,_v16)					\
								do { 											\
									(_v01) = (gpRegPNG->_r._f01);				\
									(_v02) = (gpRegPNG->_r._f02);				\
									(_v03) = (gpRegPNG->_r._f03);				\
									(_v04) = (gpRegPNG->_r._f04);				\
									(_v05) = (gpRegPNG->_r._f05);				\
									(_v06) = (gpRegPNG->_r._f06);				\
									(_v07) = (gpRegPNG->_r._f07);				\
									(_v08) = (gpRegPNG->_r._f08);				\
									(_v09) = (gpRegPNG->_r._f09);				\
									(_v10) = (gpRegPNG->_r._f10);				\
									(_v11) = (gpRegPNG->_r._f11);				\
									(_v12) = (gpRegPNG->_r._f12);				\
									(_v13) = (gpRegPNG->_r._f13);				\
									(_v14) = (gpRegPNG->_r._f14);				\
									(_v15) = (gpRegPNG->_r._f15);				\
									(_v16) = (gpRegPNG->_r._f16);				\
								} while(0)


#define PNG_Wr01(_r,_f01,_v01)													\
								do { 											\
									(gpRegPNG->_r._f01) = (_v01);				\
								} while(0)

#define PNG_Wr02(_r,_f01,_v01,_f02,_v02)										\
								do { 											\
									(gpRegPNG->_r._f01) = (_v01);				\
									(gpRegPNG->_r._f02) = (_v02);				\
								} while(0)

#define PNG_Wr03(_r,_f01,_v01,_f02,_v02,_f03,_v03)								\
								do { 											\
									(gpRegPNG->_r._f01) = (_v01);				\
									(gpRegPNG->_r._f02) = (_v02);				\
									(gpRegPNG->_r._f03) = (_v03);				\
								} while(0)

#define PNG_Wr04(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04)					\
								do { 											\
									(gpRegPNG->_r._f01) = (_v01);				\
									(gpRegPNG->_r._f02) = (_v02);				\
									(gpRegPNG->_r._f03) = (_v03);				\
									(gpRegPNG->_r._f04) = (_v04);				\
								} while(0)

#define PNG_Wr05(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05)													\
								do { 											\
									(gpRegPNG->_r._f01) = (_v01);				\
									(gpRegPNG->_r._f02) = (_v02);				\
									(gpRegPNG->_r._f03) = (_v03);				\
									(gpRegPNG->_r._f04) = (_v04);				\
									(gpRegPNG->_r._f05) = (_v05);				\
								} while(0)

#define PNG_Wr06(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06)										\
								do { 											\
									(gpRegPNG->_r._f01) = (_v01);				\
									(gpRegPNG->_r._f02) = (_v02);				\
									(gpRegPNG->_r._f03) = (_v03);				\
									(gpRegPNG->_r._f04) = (_v04);				\
									(gpRegPNG->_r._f05) = (_v05);				\
									(gpRegPNG->_r._f06) = (_v06);				\
								} while(0)

#define PNG_Wr07(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07)								\
								do { 											\
									(gpRegPNG->_r._f01) = (_v01);				\
									(gpRegPNG->_r._f02) = (_v02);				\
									(gpRegPNG->_r._f03) = (_v03);				\
									(gpRegPNG->_r._f04) = (_v04);				\
									(gpRegPNG->_r._f05) = (_v05);				\
									(gpRegPNG->_r._f06) = (_v06);				\
									(gpRegPNG->_r._f07) = (_v07);				\
								} while(0)

#define PNG_Wr08(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08)					\
								do { 											\
									(gpRegPNG->_r._f01) = (_v01);				\
									(gpRegPNG->_r._f02) = (_v02);				\
									(gpRegPNG->_r._f03) = (_v03);				\
									(gpRegPNG->_r._f04) = (_v04);				\
									(gpRegPNG->_r._f05) = (_v05);				\
									(gpRegPNG->_r._f06) = (_v06);				\
									(gpRegPNG->_r._f07) = (_v07);				\
									(gpRegPNG->_r._f08) = (_v08);				\
								} while(0)

#define PNG_Wr09(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09)													\
								do { 											\
									(gpRegPNG->_r._f01) = (_v01);				\
									(gpRegPNG->_r._f02) = (_v02);				\
									(gpRegPNG->_r._f03) = (_v03);				\
									(gpRegPNG->_r._f04) = (_v04);				\
									(gpRegPNG->_r._f05) = (_v05);				\
									(gpRegPNG->_r._f06) = (_v06);				\
									(gpRegPNG->_r._f07) = (_v07);				\
									(gpRegPNG->_r._f08) = (_v08);				\
									(gpRegPNG->_r._f09) = (_v09);				\
								} while(0)

#define PNG_Wr10(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10)										\
								do { 											\
									(gpRegPNG->_r._f01) = (_v01);				\
									(gpRegPNG->_r._f02) = (_v02);				\
									(gpRegPNG->_r._f03) = (_v03);				\
									(gpRegPNG->_r._f04) = (_v04);				\
									(gpRegPNG->_r._f05) = (_v05);				\
									(gpRegPNG->_r._f06) = (_v06);				\
									(gpRegPNG->_r._f07) = (_v07);				\
									(gpRegPNG->_r._f08) = (_v08);				\
									(gpRegPNG->_r._f09) = (_v09);				\
									(gpRegPNG->_r._f10) = (_v10);				\
								} while(0)

#define PNG_Wr11(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11)								\
								do { 											\
									(gpRegPNG->_r._f01) = (_v01);				\
									(gpRegPNG->_r._f02) = (_v02);				\
									(gpRegPNG->_r._f03) = (_v03);				\
									(gpRegPNG->_r._f04) = (_v04);				\
									(gpRegPNG->_r._f05) = (_v05);				\
									(gpRegPNG->_r._f06) = (_v06);				\
									(gpRegPNG->_r._f07) = (_v07);				\
									(gpRegPNG->_r._f08) = (_v08);				\
									(gpRegPNG->_r._f09) = (_v09);				\
									(gpRegPNG->_r._f10) = (_v10);				\
									(gpRegPNG->_r._f11) = (_v11);				\
								} while(0)

#define PNG_Wr12(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12)					\
								do { 											\
									(gpRegPNG->_r._f01) = (_v01);				\
									(gpRegPNG->_r._f02) = (_v02);				\
									(gpRegPNG->_r._f03) = (_v03);				\
									(gpRegPNG->_r._f04) = (_v04);				\
									(gpRegPNG->_r._f05) = (_v05);				\
									(gpRegPNG->_r._f06) = (_v06);				\
									(gpRegPNG->_r._f07) = (_v07);				\
									(gpRegPNG->_r._f08) = (_v08);				\
									(gpRegPNG->_r._f09) = (_v09);				\
									(gpRegPNG->_r._f10) = (_v10);				\
									(gpRegPNG->_r._f11) = (_v11);				\
									(gpRegPNG->_r._f12) = (_v12);				\
								} while(0)

#define PNG_Wr13(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13)													\
								do { 											\
									(gpRegPNG->_r._f01) = (_v01);				\
									(gpRegPNG->_r._f02) = (_v02);				\
									(gpRegPNG->_r._f03) = (_v03);				\
									(gpRegPNG->_r._f04) = (_v04);				\
									(gpRegPNG->_r._f05) = (_v05);				\
									(gpRegPNG->_r._f06) = (_v06);				\
									(gpRegPNG->_r._f07) = (_v07);				\
									(gpRegPNG->_r._f08) = (_v08);				\
									(gpRegPNG->_r._f09) = (_v09);				\
									(gpRegPNG->_r._f10) = (_v10);				\
									(gpRegPNG->_r._f11) = (_v11);				\
									(gpRegPNG->_r._f12) = (_v12);				\
									(gpRegPNG->_r._f13) = (_v13);				\
								} while(0)

#define PNG_Wr14(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14)										\
								do { 											\
									(gpRegPNG->_r._f01) = (_v01);				\
									(gpRegPNG->_r._f02) = (_v02);				\
									(gpRegPNG->_r._f03) = (_v03);				\
									(gpRegPNG->_r._f04) = (_v04);				\
									(gpRegPNG->_r._f05) = (_v05);				\
									(gpRegPNG->_r._f06) = (_v06);				\
									(gpRegPNG->_r._f07) = (_v07);				\
									(gpRegPNG->_r._f08) = (_v08);				\
									(gpRegPNG->_r._f09) = (_v09);				\
									(gpRegPNG->_r._f10) = (_v10);				\
									(gpRegPNG->_r._f11) = (_v11);				\
									(gpRegPNG->_r._f12) = (_v12);				\
									(gpRegPNG->_r._f13) = (_v13);				\
									(gpRegPNG->_r._f14) = (_v14);				\
								} while(0)

#define PNG_Wr15(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15)								\
								do { 											\
									(gpRegPNG->_r._f01) = (_v01);				\
									(gpRegPNG->_r._f02) = (_v02);				\
									(gpRegPNG->_r._f03) = (_v03);				\
									(gpRegPNG->_r._f04) = (_v04);				\
									(gpRegPNG->_r._f05) = (_v05);				\
									(gpRegPNG->_r._f06) = (_v06);				\
									(gpRegPNG->_r._f07) = (_v07);				\
									(gpRegPNG->_r._f08) = (_v08);				\
									(gpRegPNG->_r._f09) = (_v09);				\
									(gpRegPNG->_r._f10) = (_v10);				\
									(gpRegPNG->_r._f11) = (_v11);				\
									(gpRegPNG->_r._f12) = (_v12);				\
									(gpRegPNG->_r._f13) = (_v13);				\
									(gpRegPNG->_r._f14) = (_v14);				\
									(gpRegPNG->_r._f15) = (_v15);				\
								} while(0)

#define PNG_Wr16(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15,_f16,_v16)					\
								do { 											\
									(gpRegPNG->_r._f01) = (_v01);				\
									(gpRegPNG->_r._f02) = (_v02);				\
									(gpRegPNG->_r._f03) = (_v03);				\
									(gpRegPNG->_r._f04) = (_v04);				\
									(gpRegPNG->_r._f05) = (_v05);				\
									(gpRegPNG->_r._f06) = (_v06);				\
									(gpRegPNG->_r._f07) = (_v07);				\
									(gpRegPNG->_r._f08) = (_v08);				\
									(gpRegPNG->_r._f09) = (_v09);				\
									(gpRegPNG->_r._f10) = (_v10);				\
									(gpRegPNG->_r._f11) = (_v11);				\
									(gpRegPNG->_r._f12) = (_v12);				\
									(gpRegPNG->_r._f13) = (_v13);				\
									(gpRegPNG->_r._f14) = (_v14);				\
									(gpRegPNG->_r._f15) = (_v15);				\
									(gpRegPNG->_r._f16) = (_v16);				\
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
 * _rwname: name of rw    bit field : shall be 0 after PNG_Wind(), 1 for PNG_Rind()
 * _iname : name of index bit field
 * _ival  : index value
 * _fname : field name
 * _fval  : field variable that field value shall be stored.
 *
 * PNG_Rind : General indexed register Read.(
 * PNG_Wind : General indexed register Read.
 *
 * PNG_Ridx : For 'index', 'rw', 'load' field name
 * PNG_Widx : For 'index', 'rw', 'load' field name and NO_LOAD.
 */
#define PNG_Rind(_r, _lname, _rwname, _iname, _ival, _fname, _fval)				\
							do {												\
								PNG_Wr03(_r,_lname,0,_rwname,1,_iname,_ival);	\
								PNG_WrFL(_r);									\
								PNG_RdFL(_r);									\
								PNG_Rd01(_r,_fname,_fval);						\
							} while (0)

#define PNG_Wind(_r, _lname, _rwname, _iname, _ival, _fname, _fval)				\
				PNG_Wr04(_r, _lname,0, _rwname,0, _iname,_ival, _fname,_fval)


#define PNG_Ridx(_r, _ival, _fname, _fval)	PNG_Rind(_r,load,rw,index,_ival,_fname,_fval)

#define PNG_Widx(_r, _ival, _fname, _fval)	PNG_Wind(_r,load,rw,index,_ival,_fname,_fval)

/** @} *//* end of macro documentation */


/*----------------------------------------------------------------------------------------
    Global Function Prototypes Declarations
----------------------------------------------------------------------------------------*/
void PNG_Reg_Init(unsigned int png_reg_addr, unsigned int icod_reg_addr);
void PNG_Reg_Cleanup(void);

void PNG_Reg_StartDecode(void);
void PNG_Reg_StartDecodePartial(void);
void PNG_Reg_SetDecodeImageSize(unsigned short width, unsigned short height);
unsigned int PNG_Reg_GetDecodeImageWidth(void);
unsigned int PNG_Reg_GetDecodeImageHeight(void);
void PNG_Reg_SetDecodeImageInformation(unsigned char bit_depth, unsigned char color_type, unsigned char output_format);
unsigned int PNG_Reg_GetDecodeImageBitDepth(void);
unsigned int PNG_Reg_GetDecodeImageColorType(void);
unsigned int PNG_Reg_GetDecodeImageOutputFormat(void);
void PNG_Reg_SetReadStartAddress(unsigned int addr);
unsigned int PNG_Reg_GetReadStartAddress(void);
void PNG_Reg_SetReadLength(unsigned int size);
void PNG_Reg_SetWriteStartAddress(unsigned int addr);
unsigned int PNG_Reg_GetWriteStartAddress(void);
void PNG_Reg_EnableInterrupt(unsigned int flag);
void PNG_Reg_EnableInterruptAll(void);
void PNG_Reg_DisableInterrupt(unsigned int flag);
void PNG_Reg_DisableInterruptAll(void);
unsigned int PNG_Reg_GetInterruptStatusAll(void);
void PNG_Reg_ClearInterrupt(unsigned short flag);
void PNG_Reg_ClearInterruptAll(void);
unsigned int PNG_Reg_GetCurrentReadAddress(void);
unsigned int PNG_Reg_GetWriteOffset(void);
unsigned int PNG_Reg_GetCurrentWriteAddress(void);
void PNG_Reg_SetWriteAddressBoundary(unsigned int addr);
unsigned int PNG_Reg_GetWriteAddressBoundary(void);
void PNG_Reg_SetTimeoutInterrupt(unsigned int value);
unsigned int PNG_Reg_GetCurrentTimeCount(void);
unsigned int PNG_Reg_GetInFIFOValidBits(void);
unsigned int PNG_Reg_GetFIFOSpace(void);
unsigned int PNG_Reg_GetInflateStatus(void);
unsigned int PNG_Reg_GetMainState(void);
unsigned int PNG_Reg_GetTableState(void);
unsigned int PNG_Reg_GetInflateState(void);
unsigned int PNG_Reg_GetFilterType(void);
unsigned int PNG_Reg_GetFilterStatus(void);
unsigned int PNG_Reg_GetFilterDone(void);
unsigned int PNG_Reg_GetFilterLastLine(void);
unsigned int PNG_Reg_GetInflateDone(void);


void ICOD_Reg_Reset(void);






#ifdef __cplusplus
}
#endif

#endif	/* _PNG_REG_H13_H_ */

/* from 'png.csv' 20111127 10:22:03   대한민국 표준시 by getregs v2.7 */
