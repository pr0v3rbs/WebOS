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

/** @file venc_h13_reg.h
  *
  *  Brief description. 
  *  VENC PREP register details. ( used only within kdriver )
  *
  *  @author	 	So, Jaeseop (jaeseop.so@lge.com)
  *  @version	 	1.0
  *  @date		2012.04.27
  */

#ifndef _VENC_H13_REG_H_
#define _VENC_H13_REG_H_

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
} VENC_H13_INTR_EN;

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
} VENC_H13_INTR_CLR;

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
} VENC_H13_GMAU_CONFIG;

#if 0
/*-----------------------------------------------------------------------------
	0x000c prep_wbase1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	wmif1_cmd_base                  ;   	// 31: 0
} VENC_H13_PREP_WBASE1;
#else
/*-----------------------------------------------------------------------------
	0x000c timer ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	timer                  			;   	// 31: 0
} VENC_H13_TIMER;
#endif

/*-----------------------------------------------------------------------------
	0x0010 prep_bytediff ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	byte_diff                       :11;	//  0:10
} VENC_H13_PREP_BYTEDIFF;

/*-----------------------------------------------------------------------------
	0x0014 prep_rdly_max ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	rmif_dly_cnt_max                : 8;	//  0: 7
} VENC_H13_PREP_RDLY_MAX;

/*-----------------------------------------------------------------------------
	0x0018 ipc_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	de_ipc_enable                   : 1,	//     0
	vd_ipc_enable                   : 1;	//     1
} VENC_H13_IPC_CTRL;

/*-----------------------------------------------------------------------------
	0x0020 vdecintr_i_en ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	vdec_intr_i_en                  : 1;	//     0
} VENC_H13_VDECINTR_I_EN;

/*-----------------------------------------------------------------------------
	0x0024 vdecintr_i_cl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	vdec_intr_i_cl                  : 1;	//     0
} VENC_H13_VDECINTR_I_CL;

/*-----------------------------------------------------------------------------
	0x0028 vdecintr_i_st ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	vdec_intr_i_st                  : 1;	//     0
} VENC_H13_VDECINTR_I_ST;

/*-----------------------------------------------------------------------------
	0x002c vdecintr_i_ev ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	vdec_intr_i_ev                  : 1;	//     0
} VENC_H13_VDECINTR_I_EV;

/*-----------------------------------------------------------------------------
	0x0030 vdecintr_e_en ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	vdec_intr_e_en                  : 1,	//     0
	de_intr_e_en					: 1,	//		1
	h1_intr_e_en					: 1;	//		2
} VENC_H13_VDECINTR_E_EN;

/*-----------------------------------------------------------------------------
	0x0034 vdecintr_e_cl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	vdec_intr_e_cl                  : 1,	//     0
	de_intr_e_cl					: 1;	//		1
} VENC_H13_VDECINTR_E_CL;

/*-----------------------------------------------------------------------------
	0x0038 vdecintr_e_st ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	vdec_intr_e_st                  : 1,	//     0
	de_intr_e_st					: 1,	//		1
	h1_intr_e_st					: 1;	//		2
} VENC_H13_VDECINTR_E_ST;

/*-----------------------------------------------------------------------------
	0x003c vdecintr_e_ev ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	vdec_intr_e_ev                  : 1,	//     0
	de_intr_e_ev					: 1,	//		1
	h1_intr_e_ev					: 1;	//		2
} VENC_H13_VDECINTR_E_EV;

/*-----------------------------------------------------------------------------
	0x0040 vdec_info0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	vdec_info0                      ;   	// 31: 0
} VENC_H13_VDEC_INFO0;

/*-----------------------------------------------------------------------------
	0x0044 vdec_info1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	vdec_info1                      ;   	// 31: 0
} VENC_H13_VDEC_INFO1;

/*-----------------------------------------------------------------------------
	0x0048 vdec_info2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	vdec_info2                      ;   	// 31: 0
} VENC_H13_VDEC_INFO2;

/*-----------------------------------------------------------------------------
	0x004c vdec_info3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	vdec_info3                      ;   	// 31: 0
} VENC_H13_VDEC_INFO3;

/*-----------------------------------------------------------------------------
	0x0050 vdec_gstcc0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	gstcc0                          ;   	// 31: 0
} VENC_H13_VDEC_GSTCC0;

/*-----------------------------------------------------------------------------
	0x0054 vdec_gstcc1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	gstcc1                          ;   	// 31: 0
} VENC_H13_VDEC_GSTCC1;

/*-----------------------------------------------------------------------------
	0x0058 vdec_info3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	venc_stcc                       ;   	// 31: 0
} VENC_H13_VENC_STCC;

/*-----------------------------------------------------------------------------
	0x0070 prep_lock ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	prep_lock                       ;   	// 31: 0
} VENC_H13_PREP_LOCK;

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
} VENC_H13_PREP_OP;

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
} VENC_H13_PREP_IMGI;

/*-----------------------------------------------------------------------------
	0x007c prep_imgmi ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	height                          :11,	//  0:10
	                                : 5,	// 11:15 reserved
	width                           :11;	// 16:26
} VENC_H13_PREP_IMGMI;

/*-----------------------------------------------------------------------------
	0x0080 prep_imgmo ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	height                          :11,	//  0:10
	                                : 5,	// 11:15 reserved
	width                           :11;	// 16:26
} VENC_H13_PREP_IMGMO;

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
} VENC_H13_PREP_RCONFIG;

/*-----------------------------------------------------------------------------
	0x0088 prep_rbase ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	rmif_cmd_base                   ;   	// 31: 0
} VENC_H13_PREP_RBASE;

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
} VENC_H13_PREP_WCONFIG;

/*-----------------------------------------------------------------------------
	0x0090 prep_wbase0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	wmif0_cmd_base                  ;   	// 31: 0
} VENC_H13_PREP_WBASE0;

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
} VENC_H13_PREP_STATUS;

/*-----------------------------------------------------------------------------
	0x0098 prep_opcycle ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	op_cycle                        ;   	// 31: 0
} VENC_H13_PREP_OPCYCLE;

/*-----------------------------------------------------------------------------
	0x009c prep_sum ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	sum                             :24,	//  0:23
	                                : 7,	// 24:30 reserved
	init_hist                       : 1;	//    31
} VENC_H13_PREP_SUM;

/*-----------------------------------------------------------------------------
	0x00a0 de_status ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	status                          : 3,	//  0: 2
	                                :13,	//  3:15 reserved
	info                            :16;	// 16:31
} VENC_H13_DE_STATUS;

/*-----------------------------------------------------------------------------
	0x00a4 de_dsize ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	vsize                           :12,	//  0:11
	                                : 4,	// 12:15 reserved
	hsize                           :12;	// 16:27
} VENC_H13_DE_DSIZE;

/*-----------------------------------------------------------------------------
	0x00a8 decsize ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dec_vsize                       :12,	//  0:11
	                                : 4,	// 12:15 reserved
	dec_hsize                       :12;	// 16:27
} VENC_H13_DECSIZE;

/*-----------------------------------------------------------------------------
	0x00ac hoffset ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	roffset                         :16,	//  0:15
	loffset                         :16;	// 16:31
} VENC_H13_HOFFSET;

/*-----------------------------------------------------------------------------
	0x00b0 voffset ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	boffset                         :16,	//  0:15
	toffset                         :16;	// 16:31
} VENC_H13_VOFFSET;

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
} VENC_H13_SEQINFO;

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
} VENC_H13_PIC_INFO;

/*-----------------------------------------------------------------------------
	0x00bc info ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	info                            ;   	// 31: 0
} VENC_H13_INFO;

/*-----------------------------------------------------------------------------
	0x00c0 y_addr ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	addr                            ;   	// 31: 0
} VENC_H13_Y_ADDR;

/*-----------------------------------------------------------------------------
	0x00c4 y_stride ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	stride                          ;   	// 31: 0
} VENC_H13_Y_STRIDE;

/*-----------------------------------------------------------------------------
	0x00c8 c_addr ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	addr                            ;   	// 31: 0
} VENC_H13_C_ADDR;

/*-----------------------------------------------------------------------------
	0x00cc c_stride ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	stride                          ;   	// 31: 0
} VENC_H13_C_STRIDE;

/*-----------------------------------------------------------------------------
	0x00d0 stc_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	select_stc                      : 5,	//  0: 4
	dummy                           : 3,	//  5: 7
	select_stc_src                  : 2;	//  8: 9
} VENC_H13_PREP_STC_CTRL;

/*-----------------------------------------------------------------------------
	0x00d4 stcc ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	stcc                            ;   	// 31: 0
} VENC_H13_STCC;

/*-----------------------------------------------------------------------------
	0x00dc prep_config_stride_size ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	rmif_stride_size                :16,	//  0:15
	wmif_stride_size                :16;	// 16:31
} VENC_H13_PREP_CONFIG_STRIDE_SIZE;

/*-----------------------------------------------------------------------------
	0x00e4 yuvhs ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	header_size                     :11;	//  0:10
} VENC_H13_YUVHS;

/*-----------------------------------------------------------------------------
	0x00f4 cmd_dly ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32
	apg_cmd_dly_cnt                 :16;	//  0:15
} VENC_H13_CMD_DLY;

typedef struct {
	VENC_H13_INTR_EN                         	intr_en                         ;	// 0x0000 : ''
	VENC_H13_INTR_CLR                        	intr_clr                        ;	// 0x0004 : ''
	VENC_H13_GMAU_CONFIG                     	gmau_config                     ;	// 0x0008 : ''
//	VENC_H13_PREP_WBASE1                     	prep_wbase1                     ;	// 0x000c : ''
	VENC_H13_TIMER								timer							;	// 0x000C
	VENC_H13_PREP_BYTEDIFF                   	prep_bytediff                   ;	// 0x0010 : ''
	VENC_H13_PREP_RDLY_MAX                   	prep_rdly_max                   ;	// 0x0014 : ''
	VENC_H13_IPC_CTRL                        	ipc_ctrl                        ;	// 0x0018 : ''
	UINT32                          	                 __rsvd_00[   1];	// 0x001c
	VENC_H13_VDECINTR_I_EN                   	vdecintr_i_en                   ;	// 0x0020 : ''
	VENC_H13_VDECINTR_I_CL                   	vdecintr_i_cl                   ;	// 0x0024 : ''
	VENC_H13_VDECINTR_I_ST                   	vdecintr_i_st                   ;	// 0x0028 : ''
	VENC_H13_VDECINTR_I_EV                   	vdecintr_i_ev                   ;	// 0x002c : ''
	VENC_H13_VDECINTR_E_EN                   	vdecintr_e_en                   ;	// 0x0030 : ''
	VENC_H13_VDECINTR_E_CL                   	vdecintr_e_cl                   ;	// 0x0034 : ''
	VENC_H13_VDECINTR_E_ST                   	vdecintr_e_st                   ;	// 0x0038 : ''
	VENC_H13_VDECINTR_E_EV                   	vdecintr_e_ev                   ;	// 0x003c : ''
	VENC_H13_VDEC_INFO0                      	vdec_info0                      ;	// 0x0040 : ''
	VENC_H13_VDEC_INFO1                      	vdec_info1                      ;	// 0x0044 : ''
	VENC_H13_VDEC_INFO2                      	vdec_info2                      ;	// 0x0048 : ''
	VENC_H13_VDEC_INFO3                      	vdec_info3                      ;	// 0x004c : ''
	VENC_H13_VDEC_GSTCC0                     	vdec_gstcc0                     ;	// 0x0050 : ''
	VENC_H13_VDEC_GSTCC1                     	vdec_gstcc1                     ;	// 0x0054 : ''
	VENC_H13_VENC_STCC                       	venc_stcc                       ;	// 0x0058 : ''
	UINT32                          	                 __rsvd_01[   5];	// 0x005c ~ 0x006c
	VENC_H13_PREP_LOCK                       	prep_lock                       ;	// 0x0070 : ''
	VENC_H13_PREP_OP                         	prep_op                         ;	// 0x0074 : ''
	VENC_H13_PREP_IMGI                       	prep_imgi                       ;	// 0x0078 : ''
	VENC_H13_PREP_IMGMI                      	prep_imgmi                      ;	// 0x007c : ''
	VENC_H13_PREP_IMGMO                      	prep_imgmo                      ;	// 0x0080 : ''
	VENC_H13_PREP_RCONFIG                    	prep_rconfig                    ;	// 0x0084 : ''
	VENC_H13_PREP_RBASE                      	prep_rbase                      ;	// 0x0088 : ''
	VENC_H13_PREP_WCONFIG                    	prep_wconfig                    ;	// 0x008c : ''
	VENC_H13_PREP_WBASE0                     	prep_wbase0                     ;	// 0x0090 : ''
	VENC_H13_PREP_STATUS                     	prep_status                     ;	// 0x0094 : ''
	VENC_H13_PREP_OPCYCLE                    	prep_opcycle                    ;	// 0x0098 : ''
	VENC_H13_PREP_SUM                        	prep_sum                        ;	// 0x009c : ''
	VENC_H13_DE_STATUS                       	de_status                       ;	// 0x00a0 : ''
	VENC_H13_DE_DSIZE                        	de_dsize                        ;	// 0x00a4 : ''
	VENC_H13_DECSIZE                         	decsize                         ;	// 0x00a8 : ''
	VENC_H13_HOFFSET                         	hoffset                         ;	// 0x00ac : ''
	VENC_H13_VOFFSET                         	voffset                         ;	// 0x00b0 : ''
	VENC_H13_SEQINFO                         	seqinfo                         ;	// 0x00b4 : ''
	VENC_H13_PIC_INFO                        	pic_info                        ;	// 0x00b8 : ''
	VENC_H13_INFO                            	info                            ;	// 0x00bc : ''
	VENC_H13_Y_ADDR                          	y_addr                          ;	// 0x00c0 : ''
	VENC_H13_Y_STRIDE                        	y_stride                        ;	// 0x00c4 : ''
	VENC_H13_C_ADDR                          	c_addr                          ;	// 0x00c8 : ''
	VENC_H13_C_STRIDE                        	c_stride                        ;	// 0x00cc : ''
	VENC_H13_PREP_STC_CTRL                      stc_ctrl                        ;	// 0x00d0 : ''
	VENC_H13_STCC                            	stcc                            ;	// 0x00d4 : ''
	UINT32                          	                 __rsvd_02[   1];	// 0x00d8
	VENC_H13_PREP_CONFIG_STRIDE_SIZE         	prep_config_stride_size         ;	// 0x00dc : ''
	UINT32                          	                 __rsvd_03[   1];	// 0x00e0
	VENC_H13_YUVHS                           	yuvhs                           ;	// 0x00e4 : ''
	UINT32                          	                 __rsvd_04[   3];	// 0x00e8 ~ 0x00f0
	VENC_H13_CMD_DLY                         	cmd_dly                         ;	// 0x00f4 : ''
} VENC_H13_REG_T;


#ifdef __cplusplus
}
#endif

#endif	/* _VENC_H13_REG_H_ */

/* from 'vencipc.csv' 20120406 17:08:54   대한민국 표준시 by getregs v2.7 */

