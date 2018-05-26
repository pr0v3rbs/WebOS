#ifndef _PE_SS_REG_H15A0_H_
#define _PE_SS_REG_H15A0_H_
/*-----------------------------------------------------------------------------
		0xc8006500L ssc_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 bypass_n                           : 1 ;  //     0     
	UINT32 fir_config                         : 1 ;  //     1     
	UINT32 filter_type_sel                    : 1 ;  //     2     
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 size_info_manual_load              : 1 ;  //     4     
	UINT32                                    : 2 ;  //  6: 5     reserved
	UINT32 dvr_write_mode                     : 1 ;  //     7     
	UINT32                                    : 16;  // 23: 8     reserved
	UINT32 pic_end_early_line                 : 6 ;  // 29:24     
	UINT32 pic_end_sel                        : 2 ;  // 31:30     
} PE_H15A0_SSC_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc8006508L ssc_y_init_pos_h ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_init_pos_h                       : 6 ;  //  5: 0     
} PE_H15A0_SSC_Y_INIT_POS_H_T;

/*-----------------------------------------------------------------------------
		0xc800650cL ssc_y_init_pos_v ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_init_pos_v                       : 6 ;  //  5: 0     
} PE_H15A0_SSC_Y_INIT_POS_V_T;

/*-----------------------------------------------------------------------------
		0xc8006510L ssc_c_init_pos_h ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 c_init_pos_h                       : 6 ;  //  5: 0     
} PE_H15A0_SSC_C_INIT_POS_H_T;

/*-----------------------------------------------------------------------------
		0xc8006514L ssc_c_init_pos_v ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 c_init_pos_v                       : 6 ;  //  5: 0     
} PE_H15A0_SSC_C_INIT_POS_V_T;

/*-----------------------------------------------------------------------------
		0xc8006518L ssc_scr_size ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 input_h_size                       : 11;  // 10: 0     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 input_v_size                       : 11;  // 26:16     
} PE_H15A0_SSC_SCR_SIZE_T;

/*-----------------------------------------------------------------------------
		0xc800651cL ssc_win_out_size ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 out_win_h_size                     : 11;  // 10: 0     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 out_win_v_size                     : 11;  // 26:16     
} PE_H15A0_SSC_WIN_OUT_SIZE_T;

/*-----------------------------------------------------------------------------
		0xc8006520L ssc_win_in_offset ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 in_win_h_offset                    : 11;  // 10: 0     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 in_win_v_offset                    : 11;  // 26:16     
} PE_H15A0_SSC_WIN_IN_OFFSET_T;

/*-----------------------------------------------------------------------------
		0xc8006524L ssc_win_in_size ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 in_win_h_size                      : 11;  // 10: 0     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 in_win_v_size                      : 11;  // 26:16     
} PE_H15A0_SSC_WIN_IN_SIZE_T;

/*-----------------------------------------------------------------------------
		0xc8006528L ssc_c_format ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 in_c_format                        : 3 ;  //  2: 0     
	UINT32                                    : 13;  // 15: 3     reserved
	UINT32 out_c_format                       : 3 ;  // 18:16     
} PE_H15A0_SSC_C_FORMAT_T;

/*-----------------------------------------------------------------------------
		0xc8006530L ssc_h_fir_coef_data0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 h_coefficient0                     : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 h_coefficient1                     : 10;  // 25:16     
} PE_H15A0_SSC_H_FIR_COEF_DATA0_T;

/*-----------------------------------------------------------------------------
		0xc8006534L ssc_h_fir_coef_data1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 h_coefficient2                     : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 h_coefficient3                     : 10;  // 25:16     
} PE_H15A0_SSC_H_FIR_COEF_DATA1_T;

/*-----------------------------------------------------------------------------
		0xc8006538L ssc_h_fir_coef_data2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 h_coefficient4                     : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 h_coefficient5                     : 10;  // 25:16     
} PE_H15A0_SSC_H_FIR_COEF_DATA2_T;

/*-----------------------------------------------------------------------------
		0xc800653cL ssc_h_fir_coef_data3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 h_coefficient6                     : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 h_coefficient7                     : 10;  // 25:16     
} PE_H15A0_SSC_H_FIR_COEF_DATA3_T;

/*-----------------------------------------------------------------------------
		0xc8006540L ssc_h_fir_coef_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 addr                               : 4 ;  //  3: 0     
	UINT32 h_v_sel                            : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 update                             : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 rnw                                : 1 ;  //    12     
} PE_H15A0_SSC_H_FIR_COEF_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc8006568L ssc_status_rev ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_status                           : 11;  // 10: 0     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 c_status                           : 11;  // 26:16     
} PE_H15A0_SSC_STATUS_REV_T;

/*-----------------------------------------------------------------------------
		0xc800656cL ssc_manual_load ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 update_input_frm_size              : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 update_wout_size                   : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 update_win_offset                  : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 update_win_size                    : 1 ;  //    12     
} PE_H15A0_SSC_MANUAL_LOAD_T;

/*-----------------------------------------------------------------------------
		0xc8006570L ssc_canvas_mode ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 3 ;  //  2: 0     reserved
	UINT32 h_offset                           : 8 ;  // 10: 3     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 v_offset                           : 11;  // 26:16     
} PE_H15A0_SSC_CANVAS_MODE_T;

typedef struct {
	PE_H15A0_SSC_CTRL_T                   	ssc_ctrl;                     //0xc8006500L
	UINT32                             	    reserved00;                   //0xc8006504L
	PE_H15A0_SSC_Y_INIT_POS_H_T           	ssc_y_init_pos_h;             //0xc8006508L
	PE_H15A0_SSC_Y_INIT_POS_V_T           	ssc_y_init_pos_v;             //0xc800650cL
	PE_H15A0_SSC_C_INIT_POS_H_T           	ssc_c_init_pos_h;             //0xc8006510L
	PE_H15A0_SSC_C_INIT_POS_V_T           	ssc_c_init_pos_v;             //0xc8006514L
	PE_H15A0_SSC_SCR_SIZE_T               	ssc_scr_size;                 //0xc8006518L
	PE_H15A0_SSC_WIN_OUT_SIZE_T           	ssc_win_out_size;             //0xc800651cL
	PE_H15A0_SSC_WIN_IN_OFFSET_T          	ssc_win_in_offset;            //0xc8006520L
	PE_H15A0_SSC_WIN_IN_SIZE_T            	ssc_win_in_size;              //0xc8006524L
	PE_H15A0_SSC_C_FORMAT_T               	ssc_c_format;                 //0xc8006528L
	UINT32                             	    reserved01;                   //0xc800652cL
	PE_H15A0_SSC_H_FIR_COEF_DATA0_T       	ssc_h_fir_coef_data0;         //0xc8006530L
	PE_H15A0_SSC_H_FIR_COEF_DATA1_T       	ssc_h_fir_coef_data1;         //0xc8006534L
	PE_H15A0_SSC_H_FIR_COEF_DATA2_T       	ssc_h_fir_coef_data2;         //0xc8006538L
	PE_H15A0_SSC_H_FIR_COEF_DATA3_T       	ssc_h_fir_coef_data3;         //0xc800653cL
	PE_H15A0_SSC_H_FIR_COEF_CTRL_T        	ssc_h_fir_coef_ctrl;          //0xc8006540L
	UINT32                             	    reserved02[9];                //0xc8006544L~0xc8006564L
	PE_H15A0_SSC_STATUS_REV_T             	ssc_status_rev;               //0xc8006568L
	PE_H15A0_SSC_MANUAL_LOAD_T            	ssc_manual_load;              //0xc800656cL
	PE_H15A0_SSC_CANVAS_MODE_T            	ssc_canvas_mode;              //0xc8006570L
}PE_SSC_REG_H15A0_T;
#endif