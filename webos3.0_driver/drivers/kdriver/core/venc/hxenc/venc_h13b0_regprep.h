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

/** @file venc_h13_regprep.h
  *
  *  Brief description. 
  *  VENC PREP register details. ( used only within kdriver )
  *
  *  @author	 	So, Jaeseop (jaeseop.so@lge.com)
  *  @version	 	1.0
  *  @date		2012.04.27
  */

#ifndef _VENC_H13_REG_PREP_H_
#define _VENC_H13_REG_PREP_H_

/*-----------------------------------------------------------------------------
    Control Constants
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
    File Inclusions
-----------------------------------------------------------------------------*/


#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------------
	0x0000 intr_en ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 9,	//  0: 8 reserved
	intr_en_scd_done                : 1,	//     9
	intr_en_cfc_done                : 1,	//    10
	intr_en_yuvc_done               : 1,	//    11
	intr_en_prep_swrst_done         : 1,	//    12
	intr_en_prep_lockup             : 1;	//    13
} INTR_EN;

/*-----------------------------------------------------------------------------
	0x0004 intr_clr ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	intr_status                     :15,	//  0:14
	                                :10,	// 15:24 reserved
	intr_cl_scd_done                : 1,	//    25
	intr_cl_cfc_done                : 1,	//    26
	intr_cl_yuvc_done               : 1,	//    27
	intr_cl_prep_swrst_done         : 1,	//    28
	intr_cl_prep_lockup             : 1;	//    29
} INTR_CLR;

/*-----------------------------------------------------------------------------
	0x0008 gmau_config ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	apg_endian_sw                   : 1,	//     0
	                                : 3,	//  1: 3 reserved
	gmau_arb_swrst                  : 1,	//     4
	                                :11,	//  5:15 reserved
	apg_to_cnt                      :10;	// 16:25
} GMAU_CONFIG;

/*-----------------------------------------------------------------------------
	0x000c prep_wbase1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	wmif1_cmd_base                  ;   	// 31: 0
} PREP_WBASE1;

/*-----------------------------------------------------------------------------
	0x0010 prep_bytediff ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	byte_diff                       :11;	//  0:10
} PREP_BYTEDIFF;

/*-----------------------------------------------------------------------------
	0x0014 prep_rdly_max ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	rmif_dly_cnt_max                : 8;	//  0: 7
} PREP_RDLY_MAX;

/*-----------------------------------------------------------------------------
	0x0018 ipc_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	de_ipc_enable                   : 1,	//     0
	vd_ipc_enable                   : 1;	//     1
} IPC_CTRL;

/*-----------------------------------------------------------------------------
	0x0020 vdecintr_i_en ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	vdec_intr_i_en                  : 1;	//     0
} VDECINTR_I_EN;

/*-----------------------------------------------------------------------------
	0x0024 vdecintr_i_cl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	vdec_intr_i_cl                  : 1;	//     0
} VDECINTR_I_CL;

/*-----------------------------------------------------------------------------
	0x0028 vdecintr_i_st ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	vdec_intr_i_st                  : 1;	//     0
} VDECINTR_I_ST;

/*-----------------------------------------------------------------------------
	0x002c vdecintr_i_ev ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	vdec_intr_i_ev                  : 1;	//     0
} VDECINTR_I_EV;

/*-----------------------------------------------------------------------------
	0x0030 vdecintr_e_en ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	vdec_intr_e_en                  : 1,	//     0
	de_intr_e_en					: 1,	//		1
	h1_intr_e_en					: 1;	//		2
} VDECINTR_E_EN;

/*-----------------------------------------------------------------------------
	0x0034 vdecintr_e_cl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	vdec_intr_e_cl                  : 1,	//     0
	de_intr_e_cl					: 1;	//		1
} VDECINTR_E_CL;

/*-----------------------------------------------------------------------------
	0x0038 vdecintr_e_st ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	vdec_intr_e_st                  : 1,	//     0
	de_intr_e_st					: 1,	//		1
	h1_intr_e_st					: 1;	//		2
} VDECINTR_E_ST;

/*-----------------------------------------------------------------------------
	0x003c vdecintr_e_ev ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	vdec_intr_e_ev                  : 1,	//     0
	de_intr_e_ev					: 1,	//		1
	h1_intr_e_ev					: 1;	//		2
} VDECINTR_E_EV;

/*-----------------------------------------------------------------------------
	0x0040 vdec_info0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	vdec_info0                      ;   	// 31: 0
} VDEC_INFO0;

/*-----------------------------------------------------------------------------
	0x0044 vdec_info1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	vdec_info1                      ;   	// 31: 0
} VDEC_INFO1;

/*-----------------------------------------------------------------------------
	0x0048 vdec_info2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	vdec_info2                      ;   	// 31: 0
} VDEC_INFO2;

/*-----------------------------------------------------------------------------
	0x004c vdec_info3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	vdec_info3                      ;   	// 31: 0
} VDEC_INFO3;

/*-----------------------------------------------------------------------------
	0x0050 vdec_gstcc0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	gstcc0                          ;   	// 31: 0
} VDEC_GSTCC0;

/*-----------------------------------------------------------------------------
	0x0054 vdec_gstcc1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	gstcc1                          ;   	// 31: 0
} VDEC_GSTCC1;

/*-----------------------------------------------------------------------------
	0x0058 vdec_info3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	venc_stcc                       ;   	// 31: 0
} VENC_STCC;

/*-----------------------------------------------------------------------------
	0x0070 prep_lock ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	prep_lock                       ;   	// 31: 0
} PREP_LOCK;

/*-----------------------------------------------------------------------------
	0x0074 prep_op ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 4,	//  0: 3 reserved
	cfc_srt                         : 1,	//     4
	                                : 3,	//  5: 7 reserved
	yuvc_srt                        : 1,	//     8
	                                : 3,	//  9:11 reserved
	sw_rst                          : 1,	//    12
	                                : 3,	// 13:15 reserved
	one_dimension_flag              : 1;	//    16
} PREP_OP;

/*-----------------------------------------------------------------------------
	0x0078 prep_imgi ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	height                          :11,	//  0:10
	                                : 5,	// 11:15 reserved
	width                           :11,	// 16:26
	                                : 3,	// 27:29 reserved
	bot_field                       : 1,	//    30
	prog_frame                      : 1;	//    31
} PREP_IMGI;

/*-----------------------------------------------------------------------------
	0x007c prep_imgmi ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	height                          :11,	//  0:10
	                                : 5,	// 11:15 reserved
	width                           :11;	// 16:26
} PREP_IMGMI;

/*-----------------------------------------------------------------------------
	0x0080 prep_imgmo ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	height                          :11,	//  0:10
	                                : 5,	// 11:15 reserved
	width                           :11;	// 16:26
} PREP_IMGMO;

/*-----------------------------------------------------------------------------
	0x0084 prep_rconfig ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	rmif_cmd_mode                   : 4,	//  0: 3
	rmif_cmd_pri2                   : 4,	//  4: 7
	rmif_cmd_pri1                   : 4,	//  8:11
	rmif_cmd_svid                   : 4,	// 12:15
	rmif_cmd_field                  : 2,	// 16:17
	                                : 2,	// 18:19 reserved
	rmif_line_size                  : 3,	// 20:22
	                                : 1,	//    23 reserved
	rmif_line_tap                   : 1,	//    24
	                                : 3,	// 25:27 reserved
	rmif_direction                  : 1,	//    28
	rmif_endian_sw                  : 3;	// 29:31
} PREP_RCONFIG;

/*-----------------------------------------------------------------------------
	0x0088 prep_rbase ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	rmif_cmd_base                   ;   	// 31: 0
} PREP_RBASE;

/*-----------------------------------------------------------------------------
	0x008c prep_wconfig ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	wmif_cmd_mode                   : 4,	//  0: 3
	wmif_cmd_pri2                   : 4,	//  4: 7
	wmif_cmd_pri1                   : 4,	//  8:11
	wmif0_cmd_svid                  : 4,	// 12:15
	wmif_cmd_field                  : 2,	// 16:17
	                                : 2,	// 18:19 reserved
	wmif_line_size                  : 3,	// 20:22
	                                : 1,	//    23 reserved
	wmif_line_tap                   : 1,	//    24
	wmif_direction                  : 1,	//    25
	wmif_cmd_pxl_sz                 : 1,	//    26
	                                : 1,	//    27 reserved
	wmif1_cmd_svid                  : 4;	// 28:31
} PREP_WCONFIG;

/*-----------------------------------------------------------------------------
	0x0090 prep_wbase0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	wmif0_cmd_base                  ;   	// 31: 0
} PREP_WBASE0;

/*-----------------------------------------------------------------------------
	0x0094 prep_status ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	scd_done                        : 1,	//     0
	                                : 3,	//  1: 3 reserved
	cfc_done                        : 1,	//     4
	                                : 3,	//  5: 7 reserved
	yuvc_done                       : 1,	//     8
	wmif1_state                     : 3,	//  9:11
	wmif0_state                     : 3,	// 12:14
	                                : 1,	//    15 reserved
	rmif_state                      : 3;	// 16:18
} PREP_STATUS;

/*-----------------------------------------------------------------------------
	0x0098 prep_opcycle ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	op_cycle                        ;   	// 31: 0
} PREP_OPCYCLE;

/*-----------------------------------------------------------------------------
	0x009c prep_sum ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	sum                             :24,	//  0:23
	                                : 7,	// 24:30 reserved
	init_hist                       : 1;	//    31
} PREP_SUM;

/*-----------------------------------------------------------------------------
	0x00a0 de_status ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	status                          : 3,	//  0: 2
	                                :13,	//  3:15 reserved
	info                            :16;	// 16:31
} DE_STATUS;

/*-----------------------------------------------------------------------------
	0x00a4 de_dsize ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	vsize                           :12,	//  0:11
	                                : 4,	// 12:15 reserved
	hsize                           :12;	// 16:27
} DE_DSIZE;

/*-----------------------------------------------------------------------------
	0x00a8 decsize ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dec_vsize                       :12,	//  0:11
	                                : 4,	// 12:15 reserved
	dec_hsize                       :12;	// 16:27
} DECSIZE;

/*-----------------------------------------------------------------------------
	0x00ac hoffset ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	roffset                         :16,	//  0:15
	loffset                         :16;	// 16:31
} HOFFSET;

/*-----------------------------------------------------------------------------
	0x00b0 voffset ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	boffset                         :16,	//  0:15
	toffset                         :16;	// 16:31
} VOFFSET;

/*-----------------------------------------------------------------------------
	0x00b4 seqinfo ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	aspect_ratio                    : 5,	//  0: 4
	                                :11,	//  5:15 reserved
	prog_seq                        : 1,	//    16
	                                :11,	// 17:27 reserved
	frm_rate                        : 4;	// 28:31
} SEQINFO;

/*-----------------------------------------------------------------------------
	0x00b8 pic_info ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	prog_frame                      : 1,	//     0
	                                : 3,	//  1: 3 reserved
	topfield_first                  : 1,	//     4
	rpt_first_field                 : 1,	//     5
	frm_struct                      : 2,	//  6: 7
	frm_ptr                         : 8,	//  8:15
	clr_info                        :16;	// 16:31
} PIC_INFO;

/*-----------------------------------------------------------------------------
	0x00bc info ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	info                            ;   	// 31: 0
} INFO;

/*-----------------------------------------------------------------------------
	0x00c0 y_addr ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	addr                            ;   	// 31: 0
} Y_ADDR;

/*-----------------------------------------------------------------------------
	0x00c4 y_stride ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	stride                          ;   	// 31: 0
} Y_STRIDE;

/*-----------------------------------------------------------------------------
	0x00c8 c_addr ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	addr                            ;   	// 31: 0
} C_ADDR;

/*-----------------------------------------------------------------------------
	0x00cc c_stride ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	stride                          ;   	// 31: 0
} C_STRIDE;

/*-----------------------------------------------------------------------------
	0x00d0 stc_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	select_stc                      : 5,	//  0: 4
	dummy                           : 3,	//  5: 7
	select_stc_src                  : 2;	//  8: 9
} PREP_STC_CTRL;

/*-----------------------------------------------------------------------------
	0x00d4 stcc ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	stcc                            ;   	// 31: 0
} STCC;

/*-----------------------------------------------------------------------------
	0x00dc prep_config_stride_size ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	rmif_stride_size                :16,	//  0:15
	wmif_stride_size                :16;	// 16:31
} PREP_CONFIG_STRIDE_SIZE;

/*-----------------------------------------------------------------------------
	0x00e4 yuvhs ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	header_size                     :11;	//  0:10
} YUVHS;

/*-----------------------------------------------------------------------------
	0x00f4 cmd_dly ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	apg_cmd_dly_cnt                 :16;	//  0:15
} CMD_DLY;

typedef struct {
	INTR_EN                         	intr_en                         ;	// 0x0000 : ''
	INTR_CLR                        	intr_clr                        ;	// 0x0004 : ''
	GMAU_CONFIG                     	gmau_config                     ;	// 0x0008 : ''
	PREP_WBASE1                     	prep_wbase1                     ;	// 0x000c : ''
	PREP_BYTEDIFF                   	prep_bytediff                   ;	// 0x0010 : ''
	PREP_RDLY_MAX                   	prep_rdly_max                   ;	// 0x0014 : ''
	IPC_CTRL                        	ipc_ctrl                        ;	// 0x0018 : ''
	UINT32                          	                 __rsvd_00[   1];	// 0x001c
	VDECINTR_I_EN                   	vdecintr_i_en                   ;	// 0x0020 : ''
	VDECINTR_I_CL                   	vdecintr_i_cl                   ;	// 0x0024 : ''
	VDECINTR_I_ST                   	vdecintr_i_st                   ;	// 0x0028 : ''
	VDECINTR_I_EV                   	vdecintr_i_ev                   ;	// 0x002c : ''
	VDECINTR_E_EN                   	vdecintr_e_en                   ;	// 0x0030 : ''
	VDECINTR_E_CL                   	vdecintr_e_cl                   ;	// 0x0034 : ''
	VDECINTR_E_ST                   	vdecintr_e_st                   ;	// 0x0038 : ''
	VDECINTR_E_EV                   	vdecintr_e_ev                   ;	// 0x003c : ''
	VDEC_INFO0                      	vdec_info0                      ;	// 0x0040 : ''
	VDEC_INFO1                      	vdec_info1                      ;	// 0x0044 : ''
	VDEC_INFO2                      	vdec_info2                      ;	// 0x0048 : ''
	VDEC_INFO3                      	vdec_info3                      ;	// 0x004c : ''
	VDEC_GSTCC0                     	vdec_gstcc0                     ;	// 0x0050 : ''
	VDEC_GSTCC1                     	vdec_gstcc1                     ;	// 0x0054 : ''
	VENC_STCC                       	venc_stcc                       ;	// 0x0058 : ''
	UINT32                          	                 __rsvd_01[   5];	// 0x005c ~ 0x006c
	PREP_LOCK                       	prep_lock                       ;	// 0x0070 : ''
	PREP_OP                         	prep_op                         ;	// 0x0074 : ''
	PREP_IMGI                       	prep_imgi                       ;	// 0x0078 : ''
	PREP_IMGMI                      	prep_imgmi                      ;	// 0x007c : ''
	PREP_IMGMO                      	prep_imgmo                      ;	// 0x0080 : ''
	PREP_RCONFIG                    	prep_rconfig                    ;	// 0x0084 : ''
	PREP_RBASE                      	prep_rbase                      ;	// 0x0088 : ''
	PREP_WCONFIG                    	prep_wconfig                    ;	// 0x008c : ''
	PREP_WBASE0                     	prep_wbase0                     ;	// 0x0090 : ''
	PREP_STATUS                     	prep_status                     ;	// 0x0094 : ''
	PREP_OPCYCLE                    	prep_opcycle                    ;	// 0x0098 : ''
	PREP_SUM                        	prep_sum                        ;	// 0x009c : ''
	DE_STATUS                       	de_status                       ;	// 0x00a0 : ''
	DE_DSIZE                        	de_dsize                        ;	// 0x00a4 : ''
	DECSIZE                         	decsize                         ;	// 0x00a8 : ''
	HOFFSET                         	hoffset                         ;	// 0x00ac : ''
	VOFFSET                         	voffset                         ;	// 0x00b0 : ''
	SEQINFO                         	seqinfo                         ;	// 0x00b4 : ''
	PIC_INFO                        	pic_info                        ;	// 0x00b8 : ''
	INFO                            	info                            ;	// 0x00bc : ''
	Y_ADDR                          	y_addr                          ;	// 0x00c0 : ''
	Y_STRIDE                        	y_stride                        ;	// 0x00c4 : ''
	C_ADDR                          	c_addr                          ;	// 0x00c8 : ''
	C_STRIDE                        	c_stride                        ;	// 0x00cc : ''
	PREP_STC_CTRL                       stc_ctrl                        ;	// 0x00d0 : ''
	STCC                            	stcc                            ;	// 0x00d4 : ''
	UINT32                          	                 __rsvd_02[   1];	// 0x00d8
	PREP_CONFIG_STRIDE_SIZE         	prep_config_stride_size         ;	// 0x00dc : ''
	UINT32                          	                 __rsvd_03[   1];	// 0x00e0
	YUVHS                           	yuvhs                           ;	// 0x00e4 : ''
	UINT32                          	                 __rsvd_04[   3];	// 0x00e8 ~ 0x00f0
	CMD_DLY                         	cmd_dly                         ;	// 0x00f4 : ''
} VENC_H13_REG_PREP_T;
/* 51 regs, 51 types */

/* 51 regs, 51 types in Total*/

/*
 * @{
 * Naming for register pointer.
 * gpRealRegVENCPREP : real register of VENCIPC.
 * gpRegVENCPREP     : shadow register.
 *
 * @def VENC_PREP_RdFL: Read  FLushing : Shadow <- Real.
 * @def VENC_PREP_WrFL: Write FLushing : Shadow -> Real.
 * @def VENC_PREP_Rd  : Read  whole register(UINT32) from Shadow register.
 * @def VENC_PREP_Wr  : Write whole register(UINT32) from Shadow register.
 * @def VENC_PREP_Rd01 ~ VENC_PREP_Rdnn: Read  given '01~nn' fields from Shadow register.
 * @def VENC_PREP_Wr01 ~ VENC_PREP_Wrnn: Write given '01~nn' fields to   Shadow register.
 * */
#define VENC_PREP_RdFL(_r)			((gpRegVENCPREP->_r)=(gpRealRegVENCPREP->_r))
#define VENC_PREP_WrFL(_r)			((gpRealRegVENCPREP->_r)=(gpRegVENCPREP->_r))

#define VENC_PREP_Rd(_r)			*((UINT32*)(&(gpRegVENCPREP->_r)))
#define VENC_PREP_Wr(_r,_v)			((VENC_PREP_Rd(_r))=((UINT32)(_v)))

#define VENC_PREP_Rd01(_r,_f01,_v01)													\
								do { 											\
									(_v01) = (gpRegVENCPREP->_r._f01);				\
								} while(0)

#define VENC_PREP_Rd02(_r,_f01,_v01,_f02,_v02)										\
								do { 											\
									(_v01) = (gpRegVENCPREP->_r._f01);				\
									(_v02) = (gpRegVENCPREP->_r._f02);				\
								} while(0)

#define VENC_PREP_Rd03(_r,_f01,_v01,_f02,_v02,_f03,_v03)								\
								do { 											\
									(_v01) = (gpRegVENCPREP->_r._f01);				\
									(_v02) = (gpRegVENCPREP->_r._f02);				\
									(_v03) = (gpRegVENCPREP->_r._f03);				\
								} while(0)

#define VENC_PREP_Rd04(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04)					\
								do { 											\
									(_v01) = (gpRegVENCPREP->_r._f01);				\
									(_v02) = (gpRegVENCPREP->_r._f02);				\
									(_v03) = (gpRegVENCPREP->_r._f03);				\
									(_v04) = (gpRegVENCPREP->_r._f04);				\
								} while(0)

#define VENC_PREP_Rd05(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05)													\
								do { 											\
									(_v01) = (gpRegVENCPREP->_r._f01);				\
									(_v02) = (gpRegVENCPREP->_r._f02);				\
									(_v03) = (gpRegVENCPREP->_r._f03);				\
									(_v04) = (gpRegVENCPREP->_r._f04);				\
									(_v05) = (gpRegVENCPREP->_r._f05);				\
								} while(0)

#define VENC_PREP_Rd06(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06)										\
								do { 											\
									(_v01) = (gpRegVENCPREP->_r._f01);				\
									(_v02) = (gpRegVENCPREP->_r._f02);				\
									(_v03) = (gpRegVENCPREP->_r._f03);				\
									(_v04) = (gpRegVENCPREP->_r._f04);				\
									(_v05) = (gpRegVENCPREP->_r._f05);				\
									(_v06) = (gpRegVENCPREP->_r._f06);				\
								} while(0)

#define VENC_PREP_Rd07(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07)								\
								do { 											\
									(_v01) = (gpRegVENCPREP->_r._f01);				\
									(_v02) = (gpRegVENCPREP->_r._f02);				\
									(_v03) = (gpRegVENCPREP->_r._f03);				\
									(_v04) = (gpRegVENCPREP->_r._f04);				\
									(_v05) = (gpRegVENCPREP->_r._f05);				\
									(_v06) = (gpRegVENCPREP->_r._f06);				\
									(_v07) = (gpRegVENCPREP->_r._f07);				\
								} while(0)

#define VENC_PREP_Rd08(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08)					\
								do { 											\
									(_v01) = (gpRegVENCPREP->_r._f01);				\
									(_v02) = (gpRegVENCPREP->_r._f02);				\
									(_v03) = (gpRegVENCPREP->_r._f03);				\
									(_v04) = (gpRegVENCPREP->_r._f04);				\
									(_v05) = (gpRegVENCPREP->_r._f05);				\
									(_v06) = (gpRegVENCPREP->_r._f06);				\
									(_v07) = (gpRegVENCPREP->_r._f07);				\
									(_v08) = (gpRegVENCPREP->_r._f08);				\
								} while(0)

#define VENC_PREP_Rd09(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09)													\
								do { 											\
									(_v01) = (gpRegVENCPREP->_r._f01);				\
									(_v02) = (gpRegVENCPREP->_r._f02);				\
									(_v03) = (gpRegVENCPREP->_r._f03);				\
									(_v04) = (gpRegVENCPREP->_r._f04);				\
									(_v05) = (gpRegVENCPREP->_r._f05);				\
									(_v06) = (gpRegVENCPREP->_r._f06);				\
									(_v07) = (gpRegVENCPREP->_r._f07);				\
									(_v08) = (gpRegVENCPREP->_r._f08);				\
									(_v09) = (gpRegVENCPREP->_r._f09);				\
								} while(0)

#define VENC_PREP_Rd10(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10)										\
								do { 											\
									(_v01) = (gpRegVENCPREP->_r._f01);				\
									(_v02) = (gpRegVENCPREP->_r._f02);				\
									(_v03) = (gpRegVENCPREP->_r._f03);				\
									(_v04) = (gpRegVENCPREP->_r._f04);				\
									(_v05) = (gpRegVENCPREP->_r._f05);				\
									(_v06) = (gpRegVENCPREP->_r._f06);				\
									(_v07) = (gpRegVENCPREP->_r._f07);				\
									(_v08) = (gpRegVENCPREP->_r._f08);				\
									(_v09) = (gpRegVENCPREP->_r._f09);				\
									(_v10) = (gpRegVENCPREP->_r._f10);				\
								} while(0)

#define VENC_PREP_Rd11(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11)								\
								do { 											\
									(_v01) = (gpRegVENCPREP->_r._f01);				\
									(_v02) = (gpRegVENCPREP->_r._f02);				\
									(_v03) = (gpRegVENCPREP->_r._f03);				\
									(_v04) = (gpRegVENCPREP->_r._f04);				\
									(_v05) = (gpRegVENCPREP->_r._f05);				\
									(_v06) = (gpRegVENCPREP->_r._f06);				\
									(_v07) = (gpRegVENCPREP->_r._f07);				\
									(_v08) = (gpRegVENCPREP->_r._f08);				\
									(_v09) = (gpRegVENCPREP->_r._f09);				\
									(_v10) = (gpRegVENCPREP->_r._f10);				\
									(_v11) = (gpRegVENCPREP->_r._f11);				\
								} while(0)

#define VENC_PREP_Rd12(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12)					\
								do { 											\
									(_v01) = (gpRegVENCPREP->_r._f01);				\
									(_v02) = (gpRegVENCPREP->_r._f02);				\
									(_v03) = (gpRegVENCPREP->_r._f03);				\
									(_v04) = (gpRegVENCPREP->_r._f04);				\
									(_v05) = (gpRegVENCPREP->_r._f05);				\
									(_v06) = (gpRegVENCPREP->_r._f06);				\
									(_v07) = (gpRegVENCPREP->_r._f07);				\
									(_v08) = (gpRegVENCPREP->_r._f08);				\
									(_v09) = (gpRegVENCPREP->_r._f09);				\
									(_v10) = (gpRegVENCPREP->_r._f10);				\
									(_v11) = (gpRegVENCPREP->_r._f11);				\
									(_v12) = (gpRegVENCPREP->_r._f12);				\
								} while(0)

#define VENC_PREP_Rd13(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13)													\
								do { 											\
									(_v01) = (gpRegVENCPREP->_r._f01);				\
									(_v02) = (gpRegVENCPREP->_r._f02);				\
									(_v03) = (gpRegVENCPREP->_r._f03);				\
									(_v04) = (gpRegVENCPREP->_r._f04);				\
									(_v05) = (gpRegVENCPREP->_r._f05);				\
									(_v06) = (gpRegVENCPREP->_r._f06);				\
									(_v07) = (gpRegVENCPREP->_r._f07);				\
									(_v08) = (gpRegVENCPREP->_r._f08);				\
									(_v09) = (gpRegVENCPREP->_r._f09);				\
									(_v10) = (gpRegVENCPREP->_r._f10);				\
									(_v11) = (gpRegVENCPREP->_r._f11);				\
									(_v12) = (gpRegVENCPREP->_r._f12);				\
									(_v13) = (gpRegVENCPREP->_r._f13);				\
								} while(0)

#define VENC_PREP_Rd14(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14)										\
								do { 											\
									(_v01) = (gpRegVENCPREP->_r._f01);				\
									(_v02) = (gpRegVENCPREP->_r._f02);				\
									(_v03) = (gpRegVENCPREP->_r._f03);				\
									(_v04) = (gpRegVENCPREP->_r._f04);				\
									(_v05) = (gpRegVENCPREP->_r._f05);				\
									(_v06) = (gpRegVENCPREP->_r._f06);				\
									(_v07) = (gpRegVENCPREP->_r._f07);				\
									(_v08) = (gpRegVENCPREP->_r._f08);				\
									(_v09) = (gpRegVENCPREP->_r._f09);				\
									(_v10) = (gpRegVENCPREP->_r._f10);				\
									(_v11) = (gpRegVENCPREP->_r._f11);				\
									(_v12) = (gpRegVENCPREP->_r._f12);				\
									(_v13) = (gpRegVENCPREP->_r._f13);				\
									(_v14) = (gpRegVENCPREP->_r._f14);				\
								} while(0)

#define VENC_PREP_Rd15(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15)								\
								do { 											\
									(_v01) = (gpRegVENCPREP->_r._f01);				\
									(_v02) = (gpRegVENCPREP->_r._f02);				\
									(_v03) = (gpRegVENCPREP->_r._f03);				\
									(_v04) = (gpRegVENCPREP->_r._f04);				\
									(_v05) = (gpRegVENCPREP->_r._f05);				\
									(_v06) = (gpRegVENCPREP->_r._f06);				\
									(_v07) = (gpRegVENCPREP->_r._f07);				\
									(_v08) = (gpRegVENCPREP->_r._f08);				\
									(_v09) = (gpRegVENCPREP->_r._f09);				\
									(_v10) = (gpRegVENCPREP->_r._f10);				\
									(_v11) = (gpRegVENCPREP->_r._f11);				\
									(_v12) = (gpRegVENCPREP->_r._f12);				\
									(_v13) = (gpRegVENCPREP->_r._f13);				\
									(_v14) = (gpRegVENCPREP->_r._f14);				\
									(_v15) = (gpRegVENCPREP->_r._f15);				\
								} while(0)

#define VENC_PREP_Rd16(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15,_f16,_v16)					\
								do { 											\
									(_v01) = (gpRegVENCPREP->_r._f01);				\
									(_v02) = (gpRegVENCPREP->_r._f02);				\
									(_v03) = (gpRegVENCPREP->_r._f03);				\
									(_v04) = (gpRegVENCPREP->_r._f04);				\
									(_v05) = (gpRegVENCPREP->_r._f05);				\
									(_v06) = (gpRegVENCPREP->_r._f06);				\
									(_v07) = (gpRegVENCPREP->_r._f07);				\
									(_v08) = (gpRegVENCPREP->_r._f08);				\
									(_v09) = (gpRegVENCPREP->_r._f09);				\
									(_v10) = (gpRegVENCPREP->_r._f10);				\
									(_v11) = (gpRegVENCPREP->_r._f11);				\
									(_v12) = (gpRegVENCPREP->_r._f12);				\
									(_v13) = (gpRegVENCPREP->_r._f13);				\
									(_v14) = (gpRegVENCPREP->_r._f14);				\
									(_v15) = (gpRegVENCPREP->_r._f15);				\
									(_v16) = (gpRegVENCPREP->_r._f16);				\
								} while(0)


#define VENC_PREP_Wr01(_r,_f01,_v01)													\
								do { 											\
									(gpRegVENCPREP->_r._f01) = (_v01);				\
								} while(0)

#define VENC_PREP_Wr02(_r,_f01,_v01,_f02,_v02)										\
								do { 											\
									(gpRegVENCPREP->_r._f01) = (_v01);				\
									(gpRegVENCPREP->_r._f02) = (_v02);				\
								} while(0)

#define VENC_PREP_Wr03(_r,_f01,_v01,_f02,_v02,_f03,_v03)								\
								do { 											\
									(gpRegVENCPREP->_r._f01) = (_v01);				\
									(gpRegVENCPREP->_r._f02) = (_v02);				\
									(gpRegVENCPREP->_r._f03) = (_v03);				\
								} while(0)

#define VENC_PREP_Wr04(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04)					\
								do { 											\
									(gpRegVENCPREP->_r._f01) = (_v01);				\
									(gpRegVENCPREP->_r._f02) = (_v02);				\
									(gpRegVENCPREP->_r._f03) = (_v03);				\
									(gpRegVENCPREP->_r._f04) = (_v04);				\
								} while(0)

#define VENC_PREP_Wr05(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05)													\
								do { 											\
									(gpRegVENCPREP->_r._f01) = (_v01);				\
									(gpRegVENCPREP->_r._f02) = (_v02);				\
									(gpRegVENCPREP->_r._f03) = (_v03);				\
									(gpRegVENCPREP->_r._f04) = (_v04);				\
									(gpRegVENCPREP->_r._f05) = (_v05);				\
								} while(0)

#define VENC_PREP_Wr06(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06)										\
								do { 											\
									(gpRegVENCPREP->_r._f01) = (_v01);				\
									(gpRegVENCPREP->_r._f02) = (_v02);				\
									(gpRegVENCPREP->_r._f03) = (_v03);				\
									(gpRegVENCPREP->_r._f04) = (_v04);				\
									(gpRegVENCPREP->_r._f05) = (_v05);				\
									(gpRegVENCPREP->_r._f06) = (_v06);				\
								} while(0)

#define VENC_PREP_Wr07(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07)								\
								do { 											\
									(gpRegVENCPREP->_r._f01) = (_v01);				\
									(gpRegVENCPREP->_r._f02) = (_v02);				\
									(gpRegVENCPREP->_r._f03) = (_v03);				\
									(gpRegVENCPREP->_r._f04) = (_v04);				\
									(gpRegVENCPREP->_r._f05) = (_v05);				\
									(gpRegVENCPREP->_r._f06) = (_v06);				\
									(gpRegVENCPREP->_r._f07) = (_v07);				\
								} while(0)

#define VENC_PREP_Wr08(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08)					\
								do { 											\
									(gpRegVENCPREP->_r._f01) = (_v01);				\
									(gpRegVENCPREP->_r._f02) = (_v02);				\
									(gpRegVENCPREP->_r._f03) = (_v03);				\
									(gpRegVENCPREP->_r._f04) = (_v04);				\
									(gpRegVENCPREP->_r._f05) = (_v05);				\
									(gpRegVENCPREP->_r._f06) = (_v06);				\
									(gpRegVENCPREP->_r._f07) = (_v07);				\
									(gpRegVENCPREP->_r._f08) = (_v08);				\
								} while(0)

#define VENC_PREP_Wr09(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09)													\
								do { 											\
									(gpRegVENCPREP->_r._f01) = (_v01);				\
									(gpRegVENCPREP->_r._f02) = (_v02);				\
									(gpRegVENCPREP->_r._f03) = (_v03);				\
									(gpRegVENCPREP->_r._f04) = (_v04);				\
									(gpRegVENCPREP->_r._f05) = (_v05);				\
									(gpRegVENCPREP->_r._f06) = (_v06);				\
									(gpRegVENCPREP->_r._f07) = (_v07);				\
									(gpRegVENCPREP->_r._f08) = (_v08);				\
									(gpRegVENCPREP->_r._f09) = (_v09);				\
								} while(0)

#define VENC_PREP_Wr10(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10)										\
								do { 											\
									(gpRegVENCPREP->_r._f01) = (_v01);				\
									(gpRegVENCPREP->_r._f02) = (_v02);				\
									(gpRegVENCPREP->_r._f03) = (_v03);				\
									(gpRegVENCPREP->_r._f04) = (_v04);				\
									(gpRegVENCPREP->_r._f05) = (_v05);				\
									(gpRegVENCPREP->_r._f06) = (_v06);				\
									(gpRegVENCPREP->_r._f07) = (_v07);				\
									(gpRegVENCPREP->_r._f08) = (_v08);				\
									(gpRegVENCPREP->_r._f09) = (_v09);				\
									(gpRegVENCPREP->_r._f10) = (_v10);				\
								} while(0)

#define VENC_PREP_Wr11(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11)								\
								do { 											\
									(gpRegVENCPREP->_r._f01) = (_v01);				\
									(gpRegVENCPREP->_r._f02) = (_v02);				\
									(gpRegVENCPREP->_r._f03) = (_v03);				\
									(gpRegVENCPREP->_r._f04) = (_v04);				\
									(gpRegVENCPREP->_r._f05) = (_v05);				\
									(gpRegVENCPREP->_r._f06) = (_v06);				\
									(gpRegVENCPREP->_r._f07) = (_v07);				\
									(gpRegVENCPREP->_r._f08) = (_v08);				\
									(gpRegVENCPREP->_r._f09) = (_v09);				\
									(gpRegVENCPREP->_r._f10) = (_v10);				\
									(gpRegVENCPREP->_r._f11) = (_v11);				\
								} while(0)

#define VENC_PREP_Wr12(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12)					\
								do { 											\
									(gpRegVENCPREP->_r._f01) = (_v01);				\
									(gpRegVENCPREP->_r._f02) = (_v02);				\
									(gpRegVENCPREP->_r._f03) = (_v03);				\
									(gpRegVENCPREP->_r._f04) = (_v04);				\
									(gpRegVENCPREP->_r._f05) = (_v05);				\
									(gpRegVENCPREP->_r._f06) = (_v06);				\
									(gpRegVENCPREP->_r._f07) = (_v07);				\
									(gpRegVENCPREP->_r._f08) = (_v08);				\
									(gpRegVENCPREP->_r._f09) = (_v09);				\
									(gpRegVENCPREP->_r._f10) = (_v10);				\
									(gpRegVENCPREP->_r._f11) = (_v11);				\
									(gpRegVENCPREP->_r._f12) = (_v12);				\
								} while(0)

#define VENC_PREP_Wr13(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13)													\
								do { 											\
									(gpRegVENCPREP->_r._f01) = (_v01);				\
									(gpRegVENCPREP->_r._f02) = (_v02);				\
									(gpRegVENCPREP->_r._f03) = (_v03);				\
									(gpRegVENCPREP->_r._f04) = (_v04);				\
									(gpRegVENCPREP->_r._f05) = (_v05);				\
									(gpRegVENCPREP->_r._f06) = (_v06);				\
									(gpRegVENCPREP->_r._f07) = (_v07);				\
									(gpRegVENCPREP->_r._f08) = (_v08);				\
									(gpRegVENCPREP->_r._f09) = (_v09);				\
									(gpRegVENCPREP->_r._f10) = (_v10);				\
									(gpRegVENCPREP->_r._f11) = (_v11);				\
									(gpRegVENCPREP->_r._f12) = (_v12);				\
									(gpRegVENCPREP->_r._f13) = (_v13);				\
								} while(0)

#define VENC_PREP_Wr14(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14)										\
								do { 											\
									(gpRegVENCPREP->_r._f01) = (_v01);				\
									(gpRegVENCPREP->_r._f02) = (_v02);				\
									(gpRegVENCPREP->_r._f03) = (_v03);				\
									(gpRegVENCPREP->_r._f04) = (_v04);				\
									(gpRegVENCPREP->_r._f05) = (_v05);				\
									(gpRegVENCPREP->_r._f06) = (_v06);				\
									(gpRegVENCPREP->_r._f07) = (_v07);				\
									(gpRegVENCPREP->_r._f08) = (_v08);				\
									(gpRegVENCPREP->_r._f09) = (_v09);				\
									(gpRegVENCPREP->_r._f10) = (_v10);				\
									(gpRegVENCPREP->_r._f11) = (_v11);				\
									(gpRegVENCPREP->_r._f12) = (_v12);				\
									(gpRegVENCPREP->_r._f13) = (_v13);				\
									(gpRegVENCPREP->_r._f14) = (_v14);				\
								} while(0)

#define VENC_PREP_Wr15(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15)								\
								do { 											\
									(gpRegVENCPREP->_r._f01) = (_v01);				\
									(gpRegVENCPREP->_r._f02) = (_v02);				\
									(gpRegVENCPREP->_r._f03) = (_v03);				\
									(gpRegVENCPREP->_r._f04) = (_v04);				\
									(gpRegVENCPREP->_r._f05) = (_v05);				\
									(gpRegVENCPREP->_r._f06) = (_v06);				\
									(gpRegVENCPREP->_r._f07) = (_v07);				\
									(gpRegVENCPREP->_r._f08) = (_v08);				\
									(gpRegVENCPREP->_r._f09) = (_v09);				\
									(gpRegVENCPREP->_r._f10) = (_v10);				\
									(gpRegVENCPREP->_r._f11) = (_v11);				\
									(gpRegVENCPREP->_r._f12) = (_v12);				\
									(gpRegVENCPREP->_r._f13) = (_v13);				\
									(gpRegVENCPREP->_r._f14) = (_v14);				\
									(gpRegVENCPREP->_r._f15) = (_v15);				\
								} while(0)

#define VENC_PREP_Wr16(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15,_f16,_v16)					\
								do { 											\
									(gpRegVENCPREP->_r._f01) = (_v01);				\
									(gpRegVENCPREP->_r._f02) = (_v02);				\
									(gpRegVENCPREP->_r._f03) = (_v03);				\
									(gpRegVENCPREP->_r._f04) = (_v04);				\
									(gpRegVENCPREP->_r._f05) = (_v05);				\
									(gpRegVENCPREP->_r._f06) = (_v06);				\
									(gpRegVENCPREP->_r._f07) = (_v07);				\
									(gpRegVENCPREP->_r._f08) = (_v08);				\
									(gpRegVENCPREP->_r._f09) = (_v09);				\
									(gpRegVENCPREP->_r._f10) = (_v10);				\
									(gpRegVENCPREP->_r._f11) = (_v11);				\
									(gpRegVENCPREP->_r._f12) = (_v12);				\
									(gpRegVENCPREP->_r._f13) = (_v13);				\
									(gpRegVENCPREP->_r._f14) = (_v14);				\
									(gpRegVENCPREP->_r._f15) = (_v15);				\
									(gpRegVENCPREP->_r._f16) = (_v16);				\
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
 * _rwname: name of rw    bit field : shall be 0 after VENC_PREP_Wind(), 1 for VENC_PREP_Rind()
 * _iname : name of index bit field
 * _ival  : index value
 * _fname : field name
 * _fval  : field variable that field value shall be stored.
 *
 * VENC_PREP_Rind : General indexed register Read.(
 * VENC_PREP_Wind : General indexed register Read.
 *
 * VENC_PREP_Ridx : For 'index', 'rw', 'load' field name
 * VENC_PREP_Widx : For 'index', 'rw', 'load' field name and NO_LOAD.
 */
#define VENC_PREP_Rind(_r, _lname, _rwname, _iname, _ival, _fname, _fval)				\
							do {												\
								VENC_PREP_Wr03(_r,_lname,0,_rwname,1,_iname,_ival);	\
								VENC_PREP_WrFL(_r);									\
								VENC_PREP_RdFL(_r);									\
								VENC_PREP_Rd01(_r,_fname,_fval);						\
							} while (0)

#define VENC_PREP_Wind(_r, _lname, _rwname, _iname, _ival, _fname, _fval)				\
				VENC_PREP_Wr04(_r, _lname,0, _rwname,0, _iname,_ival, _fname,_fval)


#define VENC_PREP_Ridx(_r, _ival, _fname, _fval)	VENC_PREP_Rind(_r,load,rw,index,_ival,_fname,_fval)

#define VENC_PREP_Widx(_r, _ival, _fname, _fval)	VENC_PREP_Wind(_r,load,rw,index,_ival,_fname,_fval)

/** @} *//* end of macro documentation */

#ifdef __cplusplus
}
#endif

#endif	/* _VENC_H13_REG_PREP_H_ */

/* from 'vencipc.csv' 20120406 17:08:54   대한민국 표준시 by getregs v2.7 */

