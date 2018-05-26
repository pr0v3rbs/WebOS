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

#ifndef _REG_MSC_M14B0_H_
#define _REG_MSC_M14B0_H_
/*-----------------------------------------------------------------------------
		0xc0023800L msc_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 bnd_pel_filtering_scheme           : 1 ;  //     0     
	UINT32 adaptive_fir_filter_en             : 1 ;  //     1     
	UINT32 v_scaler_config                    : 1 ;  //     2     
	UINT32 buffer_location_sel                : 1 ;  //     3     
	UINT32 filter_type_sel_luma               : 1 ;  //     4     
	UINT32 filter_type_sel_chroma             : 1 ;  //     5     
	UINT32 manual_sampling_mode               : 1 ;  //     6     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 size_info_manual_load              : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 bilinear_y_h_off                   : 1 ;  //    12     
	UINT32 bilinear_y_v_off                   : 1 ;  //    13     
	UINT32 bilinear_c_h_off                   : 1 ;  //    14     
	UINT32 bilinear_c_v_off                   : 1 ;  //    15     
	UINT32 write_coef_mem_sel                 : 1 ;  //    16     
	UINT32 read_coef_mem_sel                  : 1 ;  //    17     
	UINT32                                    : 6 ;  // 23:18     reserved
	UINT32 debug_sel                          : 2 ;  // 25:24     
	UINT32                                    : 4 ;  // 29:26     reserved
	UINT32 lr_flag                            : 1 ;  //    30     
	UINT32 bit_8_mode_en                      : 1 ;  //    31     
} M14B0_MSC_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc0023804L msc_y_init_pos_h ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_init_pos_h                       : 6 ;  //  5: 0     
} M14B0_MSC_Y_INIT_POS_H_T;

/*-----------------------------------------------------------------------------
		0xc0023808L msc_y_init_pos_v ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_init_pos_v                       : 6 ;  //  5: 0     
} M14B0_MSC_Y_INIT_POS_V_T;

/*-----------------------------------------------------------------------------
		0xc002380cL msc_c_init_pos_h ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 c_init_pos_h                       : 6 ;  //  5: 0     
} M14B0_MSC_C_INIT_POS_H_T;

/*-----------------------------------------------------------------------------
		0xc0023810L msc_c_init_pos_v ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 c_init_pos_v                       : 6 ;  //  5: 0     
} M14B0_MSC_C_INIT_POS_V_T;

/*-----------------------------------------------------------------------------
		0xc0023814L msc_adaptive_th ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_th                             : 4 ;  //  3: 0     
	UINT32 blending_range                     : 2 ;  //  5: 4     
} M14B0_MSC_ADAPTIVE_TH_T;

/*-----------------------------------------------------------------------------
		0xc0023818L msc_scr_size ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 input_h_size                       : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 input_v_size                       : 12;  // 27:16     
} M14B0_MSC_SCR_SIZE_T;

/*-----------------------------------------------------------------------------
		0xc002381cL msc_win_out_size ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 out_win_h_size                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 out_win_v_size                     : 12;  // 27:16     
} M14B0_MSC_WIN_OUT_SIZE_T;

/*-----------------------------------------------------------------------------
		0xc0023820L msc_win_in_offset ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 in_win_h_offset                    : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 in_win_v_offset                    : 12;  // 27:16     
} M14B0_MSC_WIN_IN_OFFSET_T;

/*-----------------------------------------------------------------------------
		0xc0023824L msc_win_in_size ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 in_win_h_size                      : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 in_win_v_size                      : 12;  // 27:16     
} M14B0_MSC_WIN_IN_SIZE_T;

/*-----------------------------------------------------------------------------
		0xc0023828L msc_c_format ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 in_c_format                        : 3 ;  //  2: 0     
	UINT32                                    : 13;  // 15: 3     reserved
	UINT32 out_c_format                       : 3 ;  // 18:16     
} M14B0_MSC_C_FORMAT_T;

/*-----------------------------------------------------------------------------
		0xc0023830L msc_y_fir_coef_data0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient0                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient1                     : 12;  // 27:16     
} M14B0_MSC_Y_FIR_COEF_DATA0_T;

/*-----------------------------------------------------------------------------
		0xc0023834L msc_y_fir_coef_data1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient2                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient3                     : 12;  // 27:16     
} M14B0_MSC_Y_FIR_COEF_DATA1_T;

/*-----------------------------------------------------------------------------
		0xc0023838L msc_y_fir_coef_data2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient4                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient5                     : 12;  // 27:16     
} M14B0_MSC_Y_FIR_COEF_DATA2_T;

/*-----------------------------------------------------------------------------
		0xc002383cL msc_y_fir_coef_data3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient6                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient7                     : 12;  // 27:16     
} M14B0_MSC_Y_FIR_COEF_DATA3_T;

/*-----------------------------------------------------------------------------
		0xc0023840L msc_y_fir_coef_data4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient8                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient9                     : 12;  // 27:16     
} M14B0_MSC_Y_FIR_COEF_DATA4_T;

/*-----------------------------------------------------------------------------
		0xc0023844L msc_y_fir_coef_data5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient10                    : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient11                    : 12;  // 27:16     
} M14B0_MSC_Y_FIR_COEF_DATA5_T;

/*-----------------------------------------------------------------------------
		0xc0023848L msc_y_fir_coef_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 addr                               : 4 ;  //  3: 0     
	UINT32 h_v_sel                            : 3 ;  //  6: 4     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 update                             : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 rnw                                : 1 ;  //    12     
} M14B0_MSC_Y_FIR_COEF_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc0023850L msc_c_fir_coef_data0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 c_coefficient0                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 c_coefficient1                     : 12;  // 27:16     
} M14B0_MSC_C_FIR_COEF_DATA0_T;

/*-----------------------------------------------------------------------------
		0xc0023854L msc_c_fir_coef_data1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 c_coefficient2                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 c_coefficient3                     : 12;  // 27:16     
} M14B0_MSC_C_FIR_COEF_DATA1_T;

/*-----------------------------------------------------------------------------
		0xc0023858L msc_c_fir_coef_data2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 c_coefficient4                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 c_coefficient5                     : 12;  // 27:16     
} M14B0_MSC_C_FIR_COEF_DATA2_T;

/*-----------------------------------------------------------------------------
		0xc002385cL msc_c_fir_coef_data3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 c_coefficient6                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 c_coefficient7                     : 12;  // 27:16     
} M14B0_MSC_C_FIR_COEF_DATA3_T;

/*-----------------------------------------------------------------------------
		0xc0023860L msc_c_fir_coef_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 addr                               : 4 ;  //  3: 0     
	UINT32 h_v_sel                            : 3 ;  //  6: 4     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 update                             : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 rnw                                : 1 ;  //    12     
} M14B0_MSC_C_FIR_COEF_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc0023868L msc_status_rev ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_status                           : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 c_status                           : 12;  // 27:16     
	UINT32 pic_init_cnt                       : 4 ;  // 31:28     
} M14B0_MSC_STATUS_REV_T;

/*-----------------------------------------------------------------------------
		0xc002386cL msc_manuaload ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 update_input_frm_size              : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 update_wout_size                   : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 update_win_offset                  : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 update_win_size                    : 1 ;  //    12     
} M14B0_MSC_MANUALOAD_T;

/*-----------------------------------------------------------------------------
		0xc0023874L msc_chroma_tear ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 tearing_count_result               : 8 ;  //  7: 0     
} M14B0_MSC_CHROMA_TEAR_T;

/*-----------------------------------------------------------------------------
		0xc0023878L msc_chroma_tear_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 auto_chroma_filter_en              : 1 ;  //     0     
	UINT32 vsc_chroma_pre_filter_en           : 1 ;  //     1     
	UINT32 vsc_chroma_median_filter_en        : 1 ;  //     2     
	UINT32 manual_chroma_pre_filter_on        : 1 ;  //     3     
	UINT32 manual_chroma_median_filter_on     : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 detect_level_th                    : 8 ;  // 15: 8     
	UINT32 tearing_count_th                   : 8 ;  // 23:16     
	UINT32 frame_count_th                     : 4 ;  // 27:24     
} M14B0_MSC_CHROMA_TEAR_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc0023880L msc_min_max_ctrl_y ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 min_max_filter_en                  : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 min_max_th                         : 10;  // 13: 4     
} M14B0_MSC_MIN_MAX_CTRL_Y_T;

/*-----------------------------------------------------------------------------
		0xc0023884L msc_min_max_ctrl_c ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 min_max_filter_en                  : 1 ;  //     0     
	UINT32 h_filter_en                        : 1 ;  //     1     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 min_max_th                         : 10;  // 13: 4     
} M14B0_MSC_MIN_MAX_CTRL_C_T;

/*-----------------------------------------------------------------------------
		0xc0023890L manual_sampling_y_h ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 numerator                          : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 denominator                        : 12;  // 27:16     
} M14B0_MANUAL_SAMPLING_Y_H_T;

/*-----------------------------------------------------------------------------
		0xc0023894L manual_sampling_y_v ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 numerator                          : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 denominator                        : 12;  // 27:16     
} M14B0_MANUAL_SAMPLING_Y_V_T;

/*-----------------------------------------------------------------------------
		0xc0023898L manual_sampling_c_h ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 numerator                          : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 denominator                        : 12;  // 27:16     
} M14B0_MANUAL_SAMPLING_C_H_T;

/*-----------------------------------------------------------------------------
		0xc002389cL manual_sampling_c_v ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 numerator                          : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 denominator                        : 12;  // 27:16     
} M14B0_MANUAL_SAMPLING_C_V_T;

/*-----------------------------------------------------------------------------
		0xc00238a0L chroma_v_cti ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 chroma_v_cti_enable                : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 v_cti_strength                     : 2 ;  //  5: 4     
} M14B0_CHROMA_V_CTI_T;

/*-----------------------------------------------------------------------------
		0xc00238a4L msc_frame_cnt ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 frame_cnt1                         : 8 ;  //  7: 0     
	UINT32                                    : 8 ;  // 15: 8     reserved
	UINT32 frame_cnt2                         : 8 ;  // 23:16     
	UINT32                                    : 7 ;  // 30:24     reserved
	UINT32 frame_cnt_reset                    : 1 ;  //    31     
} M14B0_MSC_FRAME_CNT_T;

/*-----------------------------------------------------------------------------
		0xc00238a8L msc_pic_end_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pic_end_early_line                 : 6 ;  //  5: 0     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 pic_end_sel                        : 2 ;  //  9: 8     
} M14B0_MSC_PIC_END_CTRL_T;

typedef struct {
	M14B0_MSC_CTRL_T                   	msc_ctrl;                     //0xc0023800L
	M14B0_MSC_Y_INIT_POS_H_T           	msc_y_init_pos_h;             //0xc0023804L
	M14B0_MSC_Y_INIT_POS_V_T           	msc_y_init_pos_v;             //0xc0023808L
	M14B0_MSC_C_INIT_POS_H_T           	msc_c_init_pos_h;             //0xc002380cL
	M14B0_MSC_C_INIT_POS_V_T           	msc_c_init_pos_v;             //0xc0023810L
	M14B0_MSC_ADAPTIVE_TH_T            	msc_adaptive_th;              //0xc0023814L
	M14B0_MSC_SCR_SIZE_T               	msc_scr_size;                 //0xc0023818L
	M14B0_MSC_WIN_OUT_SIZE_T           	msc_win_out_size;             //0xc002381cL
	M14B0_MSC_WIN_IN_OFFSET_T          	msc_win_in_offset;            //0xc0023820L
	M14B0_MSC_WIN_IN_SIZE_T            	msc_win_in_size;              //0xc0023824L
	M14B0_MSC_C_FORMAT_T               	msc_c_format;                 //0xc0023828L
	UINT32                             	reserved00;                   //0xc002382cL
	M14B0_MSC_Y_FIR_COEF_DATA0_T       	msc_y_fir_coef_data0;         //0xc0023830L
	M14B0_MSC_Y_FIR_COEF_DATA1_T       	msc_y_fir_coef_data1;         //0xc0023834L
	M14B0_MSC_Y_FIR_COEF_DATA2_T       	msc_y_fir_coef_data2;         //0xc0023838L
	M14B0_MSC_Y_FIR_COEF_DATA3_T       	msc_y_fir_coef_data3;         //0xc002383cL
	M14B0_MSC_Y_FIR_COEF_DATA4_T       	msc_y_fir_coef_data4;         //0xc0023840L
	M14B0_MSC_Y_FIR_COEF_DATA5_T       	msc_y_fir_coef_data5;         //0xc0023844L
	M14B0_MSC_Y_FIR_COEF_CTRL_T        	msc_y_fir_coef_ctrl;          //0xc0023848L
	UINT32                             	reserved01;                   //0xc002384cL
	M14B0_MSC_C_FIR_COEF_DATA0_T       	msc_c_fir_coef_data0;         //0xc0023850L
	M14B0_MSC_C_FIR_COEF_DATA1_T       	msc_c_fir_coef_data1;         //0xc0023854L
	M14B0_MSC_C_FIR_COEF_DATA2_T       	msc_c_fir_coef_data2;         //0xc0023858L
	M14B0_MSC_C_FIR_COEF_DATA3_T       	msc_c_fir_coef_data3;         //0xc002385cL
	M14B0_MSC_C_FIR_COEF_CTRL_T        	msc_c_fir_coef_ctrl;          //0xc0023860L
	UINT32                             	reserved02;                   //0xc0023864L
	M14B0_MSC_STATUS_REV_T             	msc_status_rev;               //0xc0023868L
	M14B0_MSC_MANUALOAD_T              	msc_manuaload;                //0xc002386cL
	UINT32                             	reserved03;                   //0xc0023870L
	M14B0_MSC_CHROMA_TEAR_T            	msc_chroma_tear;              //0xc0023874L
	M14B0_MSC_CHROMA_TEAR_CTRL_T       	msc_chroma_tear_ctrl;         //0xc0023878L
	UINT32                             	reserved04;                   //0xc002387cL
	M14B0_MSC_MIN_MAX_CTRL_Y_T         	msc_min_max_ctrl_y;           //0xc0023880L
	M14B0_MSC_MIN_MAX_CTRL_C_T         	msc_min_max_ctrl_c;           //0xc0023884L
	UINT32                             	reserved05[2];                //0xc0023888L~0xc002388cL
	M14B0_MANUAL_SAMPLING_Y_H_T        	manual_sampling_y_h;          //0xc0023890L
	M14B0_MANUAL_SAMPLING_Y_V_T        	manual_sampling_y_v;          //0xc0023894L
	M14B0_MANUAL_SAMPLING_C_H_T        	manual_sampling_c_h;          //0xc0023898L
	M14B0_MANUAL_SAMPLING_C_V_T        	manual_sampling_c_v;          //0xc002389cL
	M14B0_CHROMA_V_CTI_T               	chroma_v_cti;                 //0xc00238a0L
	M14B0_MSC_FRAME_CNT_T              	msc_frame_cnt;                //0xc00238a4L
	M14B0_MSC_PIC_END_CTRL_T           	msc_pic_end_ctrl;             //0xc00238a8L
} DE_MSC_REG_M14B0_T;

#endif