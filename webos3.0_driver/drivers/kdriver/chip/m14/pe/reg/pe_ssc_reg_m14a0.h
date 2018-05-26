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

#ifndef _PE_SSC_REG_M14A0_H_
#define _PE_SSC_REG_M14A0_H_
/*-----------------------------------------------------------------------------
		0xc0015100L ssc_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 bypass_n                           : 1 ;  //     0     
	UINT32 fir_config                         : 1 ;  //     1     
	UINT32 filter_type_sel                    : 1 ;  //     2     
	UINT32 size_info_manual_load              : 1 ;  //     3     
	UINT32                                    : 1 ;  //     4     reserved
	UINT32 ddr_update_endian_change           : 1 ;  //     5     
	UINT32 stride_cvi_mode                    : 1 ;  //     6     
	UINT32 dvr_write_mode                     : 1 ;  //     7     
	UINT32 source_sel                         : 3 ;  // 10: 8     
} PE_M14A0_SSC_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc0015104L ssc_field ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 interlace_mode                     : 1 ;  //     0     
	UINT32 curr_field                         : 1 ;  //     1     
	UINT32 mpeg_stream                        : 1 ;  //     2     
} PE_M14A0_SSC_FIELD_T;

/*-----------------------------------------------------------------------------
		0xc0015108L ssc_y_init_pos_h ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_init_pos_h                       : 6 ;  //  5: 0     
} PE_M14A0_SSC_Y_INIT_POS_H_T;

/*-----------------------------------------------------------------------------
		0xc001510cL ssc_y_init_pos_v ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_init_pos_v                       : 6 ;  //  5: 0     
} PE_M14A0_SSC_Y_INIT_POS_V_T;

/*-----------------------------------------------------------------------------
		0xc0015110L ssc_c_init_pos_h ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 c_init_pos_h                       : 6 ;  //  5: 0     
} PE_M14A0_SSC_C_INIT_POS_H_T;

/*-----------------------------------------------------------------------------
		0xc0015114L ssc_c_init_pos_v ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 c_init_pos_v                       : 6 ;  //  5: 0     
} PE_M14A0_SSC_C_INIT_POS_V_T;

/*-----------------------------------------------------------------------------
		0xc0015118L ssc_scr_size ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 input_h_size                       : 11;  // 10: 0     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 input_v_size                       : 11;  // 26:16     
} PE_M14A0_SSC_SCR_SIZE_T;

/*-----------------------------------------------------------------------------
		0xc001511cL ssc_win_out_size ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 out_win_h_size                     : 11;  // 10: 0     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 out_win_v_size                     : 11;  // 26:16     
} PE_M14A0_SSC_WIN_OUT_SIZE_T;

/*-----------------------------------------------------------------------------
		0xc0015120L ssc_win_in_offset ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 in_win_h_offset                    : 11;  // 10: 0     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 in_win_v_offset                    : 11;  // 26:16     
} PE_M14A0_SSC_WIN_IN_OFFSET_T;

/*-----------------------------------------------------------------------------
		0xc0015124L ssc_win_in_size ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 in_win_h_size                      : 11;  // 10: 0     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 in_win_v_size                      : 11;  // 26:16     
} PE_M14A0_SSC_WIN_IN_SIZE_T;

/*-----------------------------------------------------------------------------
		0xc0015128L ssc_c_format ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 in_c_format                        : 3 ;  //  2: 0     
	UINT32                                    : 13;  // 15: 3     reserved
	UINT32 out_c_format                       : 3 ;  // 18:16     
} PE_M14A0_SSC_C_FORMAT_T;

/*-----------------------------------------------------------------------------
		0xc0015130L ssc_h_fir_coef_data0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 h_coefficient0                     : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 h_coefficient1                     : 10;  // 25:16     
} PE_M14A0_SSC_H_FIR_COEF_DATA0_T;

/*-----------------------------------------------------------------------------
		0xc0015134L ssc_h_fir_coef_data1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 h_coefficient2                     : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 h_coefficient3                     : 10;  // 25:16     
} PE_M14A0_SSC_H_FIR_COEF_DATA1_T;

/*-----------------------------------------------------------------------------
		0xc0015138L ssc_h_fir_coef_data2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 h_coefficient4                     : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 h_coefficient5                     : 10;  // 25:16     
} PE_M14A0_SSC_H_FIR_COEF_DATA2_T;

/*-----------------------------------------------------------------------------
		0xc001513cL ssc_h_fir_coef_data3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 h_coefficient6                     : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 h_coefficient7                     : 10;  // 25:16     
} PE_M14A0_SSC_H_FIR_COEF_DATA3_T;

/*-----------------------------------------------------------------------------
		0xc0015140L ssc_h_fir_coef_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 addr                               : 4 ;  //  3: 0     
	UINT32 h_v_sel                            : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 update                             : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 rnw                                : 1 ;  //    12     
} PE_M14A0_SSC_H_FIR_COEF_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc0015168L ssc_status_rev ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_status                           : 11;  // 10: 0     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 c_status                           : 11;  // 26:16     
} PE_M14A0_SSC_STATUS_REV_T;

/*-----------------------------------------------------------------------------
		0xc001516cL ssc_manual_load ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 update_input_frm_size              : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 update_wout_size                   : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 update_win_offset                  : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 update_win_size                    : 1 ;  //    12     
} PE_M14A0_SSC_MANUAL_LOAD_T;

/*-----------------------------------------------------------------------------
		0xc0015170L ssc_canvas_mode ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 h_offset                           : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 v_offset                           : 11;  // 26:16     
} PE_M14A0_SSC_CANVAS_MODE_T;

typedef struct {
	PE_M14A0_SSC_CTRL_T                   	ssc_ctrl;                     //0xc0015100L
	PE_M14A0_SSC_FIELD_T                  	ssc_field;                    //0xc0015104L
	PE_M14A0_SSC_Y_INIT_POS_H_T           	ssc_y_init_pos_h;             //0xc0015108L
	PE_M14A0_SSC_Y_INIT_POS_V_T           	ssc_y_init_pos_v;             //0xc001510cL
	PE_M14A0_SSC_C_INIT_POS_H_T           	ssc_c_init_pos_h;             //0xc0015110L
	PE_M14A0_SSC_C_INIT_POS_V_T           	ssc_c_init_pos_v;             //0xc0015114L
	PE_M14A0_SSC_SCR_SIZE_T               	ssc_scr_size;                 //0xc0015118L
	PE_M14A0_SSC_WIN_OUT_SIZE_T           	ssc_win_out_size;             //0xc001511cL
	PE_M14A0_SSC_WIN_IN_OFFSET_T          	ssc_win_in_offset;            //0xc0015120L
	PE_M14A0_SSC_WIN_IN_SIZE_T            	ssc_win_in_size;              //0xc0015124L
	PE_M14A0_SSC_C_FORMAT_T               	ssc_c_format;                 //0xc0015128L
	UINT32                              	reserved00;                   //0xc001512cL
	PE_M14A0_SSC_H_FIR_COEF_DATA0_T       	ssc_h_fir_coef_data0;         //0xc0015130L
	PE_M14A0_SSC_H_FIR_COEF_DATA1_T       	ssc_h_fir_coef_data1;         //0xc0015134L
	PE_M14A0_SSC_H_FIR_COEF_DATA2_T       	ssc_h_fir_coef_data2;         //0xc0015138L
	PE_M14A0_SSC_H_FIR_COEF_DATA3_T       	ssc_h_fir_coef_data3;         //0xc001513cL
	PE_M14A0_SSC_H_FIR_COEF_CTRL_T        	ssc_h_fir_coef_ctrl;          //0xc0015140L
	UINT32                              	reserved01[9];                //0xc0015144L~0xc0015164L
	PE_M14A0_SSC_STATUS_REV_T             	ssc_status_rev;               //0xc0015168L
	PE_M14A0_SSC_MANUAL_LOAD_T            	ssc_manual_load;              //0xc001516cL
	PE_M14A0_SSC_CANVAS_MODE_T            	ssc_canvas_mode;              //0xc0015170L
}PE_SSC_REG_M14A0_T;
#endif