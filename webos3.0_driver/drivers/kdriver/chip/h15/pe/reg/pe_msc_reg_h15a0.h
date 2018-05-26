#ifndef _PE_MSC_REG_H15A0_H_
#define _PE_MSC_REG_H15A0_H_
/*-----------------------------------------------------------------------------
		0xc8007500L msc_m1_y_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_mif_rd_on_m1                   : 1 ;  //     0     
	UINT32 reg_mif_wr_on_m1                   : 1 ;  //     1     
	UINT32 reg_merge_on                       : 1 ;  //     2     
	UINT32 reg_vsc_mode_m1                    : 1 ;  //     3     
	UINT32 reg_adaptive_on_m1                 : 1 ;  //     4     
	UINT32 reg_bilinear_on_m1                 : 1 ;  //     5     
	UINT32 reg_th                             : 4 ;  //  9: 6     
	UINT32 blending_range                     : 2 ;  // 11:10     
	UINT32                                    : 2 ;  // 13:12     reserved
	UINT32 reg_min_max_on_m1                  : 1 ;  //    14     
	UINT32 reg_min_max_th_m1                  : 10;  // 24:15     
	UINT32 reg_v_reverse_sm_m1                : 1 ;  //    25     
	UINT32                                    : 4 ;  // 29:26     reserved
	UINT32 reg_mode_32p_m1                    : 1 ;  //    30     
	UINT32 reg_sampling_mode_m1               : 1 ;  //    31     
} PE_H15A0_MSC_M1_Y_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc8007504L msc_m1_y_mif_rd_size ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hsize_m1                       : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_vsize_m1                       : 12;  // 27:16     
} PE_H15A0_MSC_M1_Y_MIF_RD_SIZE_T;

/*-----------------------------------------------------------------------------
		0xc8007508L msc_m1_y_outsize ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hosize_m1                      : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_vosize_m1                      : 12;  // 27:16     
} PE_H15A0_MSC_M1_Y_OUTSIZE_T;

/*-----------------------------------------------------------------------------
		0xc800750cL msc_m1_y_canvas_write ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_canvas_xoffset_m1              : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 reg_canvas_yoffset_m1              : 11;  // 26:16     
} PE_H15A0_MSC_M1_Y_CANVAS_WRITE_T;

/*-----------------------------------------------------------------------------
		0xc8007510L msc_m1_y_merge_param ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_active_merge_m1                : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_overlap_merge_m1               : 13;  // 28:16     
} PE_H15A0_MSC_M1_Y_MERGE_PARAM_T;

/*-----------------------------------------------------------------------------
		0xc8007514L msc_m1_y_crop_param1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hoffset_m1                     : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_voffset_m1                     : 12;  // 27:16     
} PE_H15A0_MSC_M1_Y_CROP_PARAM1_T;

/*-----------------------------------------------------------------------------
		0xc8007518L msc_m1_y_crop_param2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hactive_m1                     : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_vactive_m1                     : 12;  // 27:16     
} PE_H15A0_MSC_M1_Y_CROP_PARAM2_T;

/*-----------------------------------------------------------------------------
		0xc800751cL msc_m1_y_crop_param3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hisize_m1                      : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_visize_m1                      : 12;  // 27:16     
} PE_H15A0_MSC_M1_Y_CROP_PARAM3_T;

/*-----------------------------------------------------------------------------
		0xc8007520L msc_m1_y_phase_offset ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_phase_off_v_m1                 : 9 ;  //  8: 0     
	UINT32                                    : 7 ;  // 15: 9     reserved
	UINT32 reg_phase_off_h_m1                 : 9 ;  // 24:16     
} PE_H15A0_MSC_M1_Y_PHASE_OFFSET_T;

/*-----------------------------------------------------------------------------
		0xc8007524L msc_m1_y_sampling_h ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_denominator_h_m1               : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_numerator_h_m1                 : 13;  // 28:16     
} PE_H15A0_MSC_M1_Y_SAMPLING_H_T;

/*-----------------------------------------------------------------------------
		0xc8007528L msc_m1_y_sampling_v ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_denominator_v_m1               : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 reg_numerator_v_m1                 : 12;  // 27:16     
} PE_H15A0_MSC_M1_Y_SAMPLING_V_T;

/*-----------------------------------------------------------------------------
		0xc8007540L pic_init_pe0_sel ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 16;  // 15: 0     reserved
	UINT32 pic_init_pe0_l_sel                 : 1 ;  //    16     
	UINT32 pic_init_pe0_r_sel                 : 1 ;  //    17     
} PE_H15A0_MSC_Y_PIC_INIT_PE0_SEL_T;

/*-----------------------------------------------------------------------------
		0xc8007544L msc_m1_y_v1_fir_coef_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 update                             : 1 ;  //     0     
	UINT32 rnw                                : 1 ;  //     1     
	UINT32 addr                               : 8 ;  //  9: 2     
} PE_H15A0_MSC_M1_Y_V1_FIR_COEF_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc8007548L msc_m1_y_v1_y_fir_coef_data0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient0                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient1                     : 12;  // 27:16     
} PE_H15A0_MSC_M1_Y_V1_Y_FIR_COEF_DATA0_T;

/*-----------------------------------------------------------------------------
		0xc800754cL msc_m1_y_v1_y_fir_coef_data1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient2                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient3                     : 12;  // 27:16     
} PE_H15A0_MSC_M1_Y_V1_Y_FIR_COEF_DATA1_T;

/*-----------------------------------------------------------------------------
		0xc8007550L msc_m1_y_v1_y_fir_coef_data2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient4                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient5                     : 12;  // 27:16     
} PE_H15A0_MSC_M1_Y_V1_Y_FIR_COEF_DATA2_T;

/*-----------------------------------------------------------------------------
		0xc8007554L msc_m1_y_v1_y_fir_coef_data3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient6                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient7                     : 12;  // 27:16     
} PE_H15A0_MSC_M1_Y_V1_Y_FIR_COEF_DATA3_T;

/*-----------------------------------------------------------------------------
		0xc8007558L msc_m1_y_v1_y_fir_coef_data4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient8                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient9                     : 12;  // 27:16     
} PE_H15A0_MSC_M1_Y_V1_Y_FIR_COEF_DATA4_T;

/*-----------------------------------------------------------------------------
		0xc800755cL msc_m1_y_v1_y_fir_coef_data5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient10                    : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient11                    : 12;  // 27:16     
} PE_H15A0_MSC_M1_Y_V1_Y_FIR_COEF_DATA5_T;

/*-----------------------------------------------------------------------------
		0xc8007560L msc_m1_y_v2_fir_coef_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 update                             : 1 ;  //     0     
	UINT32 rnw                                : 1 ;  //     1     
	UINT32 addr                               : 8 ;  //  9: 2     
} PE_H15A0_MSC_M1_Y_V2_FIR_COEF_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc8007564L msc_m1_y_v2_y_fir_coef_data0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient0                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient1                     : 12;  // 27:16     
} PE_H15A0_MSC_M1_Y_V2_Y_FIR_COEF_DATA0_T;

/*-----------------------------------------------------------------------------
		0xc8007568L msc_m1_y_v2_y_fir_coef_data1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient2                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient3                     : 12;  // 27:16     
} PE_H15A0_MSC_M1_Y_V2_Y_FIR_COEF_DATA1_T;

/*-----------------------------------------------------------------------------
		0xc800756cL msc_m1_y_v2_y_fir_coef_data2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient4                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient5                     : 12;  // 27:16     
} PE_H15A0_MSC_M1_Y_V2_Y_FIR_COEF_DATA2_T;

/*-----------------------------------------------------------------------------
		0xc8007570L msc_m1_y_v2_y_fir_coef_data3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient6                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient7                     : 12;  // 27:16     
} PE_H15A0_MSC_M1_Y_V2_Y_FIR_COEF_DATA3_T;

/*-----------------------------------------------------------------------------
		0xc8007574L msc_m1_y_h1_fir_coef_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 update                             : 1 ;  //     0     
	UINT32 rnw                                : 1 ;  //     1     
	UINT32 addr                               : 8 ;  //  9: 2     
} PE_H15A0_MSC_M1_Y_H1_FIR_COEF_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc8007578L msc_m1_y_h1_y_fir_coef_data0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient0                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient1                     : 12;  // 27:16     
} PE_H15A0_MSC_M1_Y_H1_Y_FIR_COEF_DATA0_T;

/*-----------------------------------------------------------------------------
		0xc800757cL msc_m1_y_h1_y_fir_coef_data1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient2                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient3                     : 12;  // 27:16     
} PE_H15A0_MSC_M1_Y_H1_Y_FIR_COEF_DATA1_T;

/*-----------------------------------------------------------------------------
		0xc8007580L msc_m1_y_h1_y_fir_coef_data2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient4                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient5                     : 12;  // 27:16     
} PE_H15A0_MSC_M1_Y_H1_Y_FIR_COEF_DATA2_T;

/*-----------------------------------------------------------------------------
		0xc8007584L msc_m1_y_h1_y_fir_coef_data3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient6                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient7                     : 12;  // 27:16     
} PE_H15A0_MSC_M1_Y_H1_Y_FIR_COEF_DATA3_T;

/*-----------------------------------------------------------------------------
		0xc8007588L msc_m1_y_h1_y_fir_coef_data4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient8                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient9                     : 12;  // 27:16     
} PE_H15A0_MSC_M1_Y_H1_Y_FIR_COEF_DATA4_T;

/*-----------------------------------------------------------------------------
		0xc800758cL msc_m1_y_h1_y_fir_coef_data5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient10                    : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient11                    : 12;  // 27:16     
} PE_H15A0_MSC_M1_Y_H1_Y_FIR_COEF_DATA5_T;

/*-----------------------------------------------------------------------------
		0xc8007590L msc_m1_y_h2_fir_coef_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 update                             : 1 ;  //     0     
	UINT32 rnw                                : 1 ;  //     1     
	UINT32 addr                               : 8 ;  //  9: 2     
} PE_H15A0_MSC_M1_Y_H2_FIR_COEF_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc8007594L msc_m1_y_h2_y_fir_coef_data0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient0                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient1                     : 12;  // 27:16     
} PE_H15A0_MSC_M1_Y_H2_Y_FIR_COEF_DATA0_T;

/*-----------------------------------------------------------------------------
		0xc8007598L msc_m1_y_h2_y_fir_coef_data1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient2                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient3                     : 12;  // 27:16     
} PE_H15A0_MSC_M1_Y_H2_Y_FIR_COEF_DATA1_T;

/*-----------------------------------------------------------------------------
		0xc800759cL msc_m1_y_h2_y_fir_coef_data2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient4                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient5                     : 12;  // 27:16     
} PE_H15A0_MSC_M1_Y_H2_Y_FIR_COEF_DATA2_T;

/*-----------------------------------------------------------------------------
		0xc80075a0L msc_m1_y_h2_y_fir_coef_data3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient6                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient7                     : 12;  // 27:16     
} PE_H15A0_MSC_M1_Y_H2_Y_FIR_COEF_DATA3_T;

/*-----------------------------------------------------------------------------
		0xc80075a4L msc_m2_y_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_mif_rd_on_m2                   : 1 ;  //     0     
	UINT32 reg_mif_rd_on_m2_sub               : 1 ;  //     1     
	UINT32 reg_mif_wr_on_sm_m2                : 1 ;  //     2     
	UINT32 reg_vsc_mode_m2                    : 1 ;  //     3     
	UINT32 reg_adaptive_on_m2                 : 1 ;  //     4     
	UINT32 reg_bilinear_on_m2                 : 1 ;  //     5     
	UINT32 reg_th                             : 4 ;  //  9: 6     
	UINT32 blending_range                     : 2 ;  // 11:10     
	UINT32                                    : 2 ;  // 13:12     reserved
	UINT32 reg_min_max_on_m2                  : 1 ;  //    14     
	UINT32 reg_min_max_th_m2                  : 10;  // 24:15     
	UINT32 reg_v_reverse_sm_m2                : 1 ;  //    25     
	UINT32                                    : 4 ;  // 29:26     reserved
	UINT32 reg_mode_32p_m2                    : 1 ;  //    30     
	UINT32 reg_sampling_mode_m2               : 1 ;  //    31     
} PE_H15A0_MSC_M2_Y_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc80075a8L msc_m2_y_mif_rd_size ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hsize_r                        : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_vsize_r                        : 12;  // 27:16     
} PE_H15A0_MSC_M2_Y_MIF_RD_SIZE_T;

/*-----------------------------------------------------------------------------
		0xc80075acL msc_m2_y_outsize ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hosize_r                       : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_vosize_r                       : 12;  // 27:16     
} PE_H15A0_MSC_M2_Y_OUTSIZE_T;

/*-----------------------------------------------------------------------------
		0xc80075b0L msc_m2_y_canvas_write ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_canvas_xoffset_r               : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 reg_canvas_yoffset_r               : 11;  // 26:16     
} PE_H15A0_MSC_M2_Y_CANVAS_WRITE_T;

/*-----------------------------------------------------------------------------
		0xc80075b4L msc_m2_y_merge_param ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_active_merge_r                 : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_overlap_merge_r                : 13;  // 28:16     
} PE_H15A0_MSC_M2_Y_MERGE_PARAM_T;

/*-----------------------------------------------------------------------------
		0xc80075b8L msc_m2_y_crop_param1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hoffset_r                      : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_voffset_r                      : 12;  // 27:16     
} PE_H15A0_MSC_M2_Y_CROP_PARAM1_T;

/*-----------------------------------------------------------------------------
		0xc80075bcL msc_m2_y_crop_param2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hactive_r                      : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_vactive_r                      : 12;  // 27:16     
} PE_H15A0_MSC_M2_Y_CROP_PARAM2_T;

/*-----------------------------------------------------------------------------
		0xc80075c0L msc_m2_y_crop_param3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hisize_r                       : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_visize_r                       : 12;  // 27:16     
} PE_H15A0_MSC_M2_Y_CROP_PARAM3_T;

/*-----------------------------------------------------------------------------
		0xc80075c4L msc_m2_y_phase_offset ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_phase_off_v_r                  : 9 ;  //  8: 0     
	UINT32                                    : 7 ;  // 15: 9     reserved
	UINT32 reg_phase_off_h_r                  : 9 ;  // 24:16     
} PE_H15A0_MSC_M2_Y_PHASE_OFFSET_T;

/*-----------------------------------------------------------------------------
		0xc80075c8L msc_m2_y_sampling_h ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_denominator_h_r                : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_numerator_h_r                  : 13;  // 28:16     
} PE_H15A0_MSC_M2_Y_SAMPLING_H_T;

/*-----------------------------------------------------------------------------
		0xc80075ccL msc_m2_y_sampling_v ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_denominator_v_r                : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 reg_numerator_v_r                  : 12;  // 27:16     
} PE_H15A0_MSC_M2_Y_SAMPLING_V_T;

/*-----------------------------------------------------------------------------
		0xc80075d0L msc_m2_sub_y_mif_rd_size ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hsize_r_sub                    : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_vsize_r_sub                    : 12;  // 27:16     
} PE_H15A0_MSC_M2_SUB_Y_MIF_RD_SIZE_T;

/*-----------------------------------------------------------------------------
		0xc80075d4L msc_m2_sub_y_crop_param1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hoffset_r_sub                  : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_voffset_r_sub                  : 12;  // 27:16     
} PE_H15A0_MSC_M2_SUB_Y_CROP_PARAM1_T;

/*-----------------------------------------------------------------------------
		0xc80075d8L msc_m2_sub_y_crop_param2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hactive_r_sub                  : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_vactive_r_sub                  : 12;  // 27:16     
} PE_H15A0_MSC_M2_SUB_Y_CROP_PARAM2_T;

/*-----------------------------------------------------------------------------
		0xc80075dcL msc_m2_sub_y_crop_param3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hisize_r_sub                   : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_visize_r_sub                   : 12;  // 27:16     
} PE_H15A0_MSC_M2_SUB_Y_CROP_PARAM3_T;

/*-----------------------------------------------------------------------------
		0xc80075e8L msc_m2_y_v1_fir_coef_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 update                             : 1 ;  //     0     
	UINT32 rnw                                : 1 ;  //     1     
	UINT32 addr                               : 8 ;  //  9: 2     
} PE_H15A0_MSC_M2_Y_V1_FIR_COEF_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc80075ecL msc_m2_y_v1_fir_coef_data0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient0                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient1                     : 12;  // 27:16     
} PE_H15A0_MSC_M2_Y_V1_FIR_COEF_DATA0_T;

/*-----------------------------------------------------------------------------
		0xc80075f0L msc_m2_y_v1_fir_coef_data1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient2                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient3                     : 12;  // 27:16     
} PE_H15A0_MSC_M2_Y_V1_FIR_COEF_DATA1_T;

/*-----------------------------------------------------------------------------
		0xc80075f4L msc_m2_y_v1_fir_coef_data2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient4                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient5                     : 12;  // 27:16     
} PE_H15A0_MSC_M2_Y_V1_FIR_COEF_DATA2_T;

/*-----------------------------------------------------------------------------
		0xc80075f8L msc_m2_y_v1_fir_coef_data3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient6                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient7                     : 12;  // 27:16     
} PE_H15A0_MSC_M2_Y_V1_FIR_COEF_DATA3_T;

/*-----------------------------------------------------------------------------
		0xc80075fcL msc_m2_y_v1_fir_coef_data4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient8                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient9                     : 12;  // 27:16     
} PE_H15A0_MSC_M2_Y_V1_FIR_COEF_DATA4_T;

/*-----------------------------------------------------------------------------
		0xc8007600L msc_m2_y_v1_fir_coef_data5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient10                    : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient11                    : 12;  // 27:16     
} PE_H15A0_MSC_M2_Y_V1_FIR_COEF_DATA5_T;

/*-----------------------------------------------------------------------------
		0xc8007604L msc_m2_y_v2_fir_coef_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 update                             : 1 ;  //     0     
	UINT32 rnw                                : 1 ;  //     1     
	UINT32 addr                               : 8 ;  //  9: 2     
} PE_H15A0_MSC_M2_Y_V2_FIR_COEF_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc8007608L msc_m2_y_v2_fir_coef_data0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient0                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient1                     : 12;  // 27:16     
} PE_H15A0_MSC_M2_Y_V2_FIR_COEF_DATA0_T;

/*-----------------------------------------------------------------------------
		0xc800760cL msc_m2_y_v2_fir_coef_data1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient2                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient3                     : 12;  // 27:16     
} PE_H15A0_MSC_M2_Y_V2_FIR_COEF_DATA1_T;

/*-----------------------------------------------------------------------------
		0xc8007610L msc_m2_y_v2_fir_coef_data2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient4                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient5                     : 12;  // 27:16     
} PE_H15A0_MSC_M2_Y_V2_FIR_COEF_DATA2_T;

/*-----------------------------------------------------------------------------
		0xc8007614L msc_m2_y_v2_fir_coef_data3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient6                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient7                     : 12;  // 27:16     
} PE_H15A0_MSC_M2_Y_V2_FIR_COEF_DATA3_T;

/*-----------------------------------------------------------------------------
		0xc8007618L msc_m2_y_h1_fir_coef_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 update                             : 1 ;  //     0     
	UINT32 rnw                                : 1 ;  //     1     
	UINT32 addr                               : 8 ;  //  9: 2     
} PE_H15A0_MSC_M2_Y_H1_FIR_COEF_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc800761cL msc_m2_y_h1_fir_coef_data0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient0                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient1                     : 12;  // 27:16     
} PE_H15A0_MSC_M2_Y_H1_FIR_COEF_DATA0_T;

/*-----------------------------------------------------------------------------
		0xc8007620L msc_m2_y_h1_fir_coef_data1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient2                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient3                     : 12;  // 27:16     
} PE_H15A0_MSC_M2_Y_H1_FIR_COEF_DATA1_T;

/*-----------------------------------------------------------------------------
		0xc8007624L msc_m2_y_h1_fir_coef_data2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient4                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient5                     : 12;  // 27:16     
} PE_H15A0_MSC_M2_Y_H1_FIR_COEF_DATA2_T;

/*-----------------------------------------------------------------------------
		0xc8007628L msc_m2_y_h1_fir_coef_data3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient6                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient7                     : 12;  // 27:16     
} PE_H15A0_MSC_M2_Y_H1_FIR_COEF_DATA3_T;

/*-----------------------------------------------------------------------------
		0xc800762cL msc_m2_y_h1_fir_coef_data4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient8                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient9                     : 12;  // 27:16     
} PE_H15A0_MSC_M2_Y_H1_FIR_COEF_DATA4_T;

/*-----------------------------------------------------------------------------
		0xc8007630L msc_m2_y_h1_fir_coef_data5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient10                    : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient11                    : 12;  // 27:16     
} PE_H15A0_MSC_M2_Y_H1_FIR_COEF_DATA5_T;

/*-----------------------------------------------------------------------------
		0xc8007634L msc_m2_y_h2_fir_coef_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 update                             : 1 ;  //     0     
	UINT32 rnw                                : 1 ;  //     1     
	UINT32 addr                               : 8 ;  //  9: 2     
} PE_H15A0_MSC_M2_Y_H2_FIR_COEF_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc8007638L msc_m2_y_h2_fir_coef_data0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient0                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient1                     : 12;  // 27:16     
} PE_H15A0_MSC_M2_Y_H2_FIR_COEF_DATA0_T;

/*-----------------------------------------------------------------------------
		0xc800763cL msc_m2_y_h2_fir_coef_data1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient2                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient3                     : 12;  // 27:16     
} PE_H15A0_MSC_M2_Y_H2_FIR_COEF_DATA1_T;

/*-----------------------------------------------------------------------------
		0xc8007640L msc_m2_y_h2_fir_coef_data2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient4                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient5                     : 12;  // 27:16     
} PE_H15A0_MSC_M2_Y_H2_FIR_COEF_DATA2_T;

/*-----------------------------------------------------------------------------
		0xc8007644L msc_m2_y_h2_fir_coef_data3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient6                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient7                     : 12;  // 27:16     
} PE_H15A0_MSC_M2_Y_H2_FIR_COEF_DATA3_T;

/*-----------------------------------------------------------------------------
		0xc8007648L msc_h3d_y_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_h3d_half_on                    : 1 ;  //     0     
	UINT32 reg_h3d_pe0_sel                    : 1 ;  //     1     
	UINT32 reg_h3d_on                         : 1 ;  //     2     
	UINT32 reg_h3d_merge_on                   : 1 ;  //     3     
	UINT32 reg_h3d_bilinear_on                : 1 ;  //     4     
	UINT32                                    : 24;  // 28: 5     reserved
	UINT32 reg_h3d_mode_32p                   : 1 ;  //    29     
	UINT32 reg_h3d_flow_ctrl_on               : 1 ;  //    30     
	UINT32 reg_h3d_sampling_mode              : 1 ;  //    31     
} PE_H15A0_MSC_H3D_Y_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc800764cL msc_h3d_y_insize1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_h3d_hsize_l                    : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_h3d_vsize_l                    : 12;  // 27:16     
} PE_H15A0_MSC_H3D_Y_INSIZE1_T;

/*-----------------------------------------------------------------------------
		0xc8007650L msc_h3d_y_insize2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_h3d_hsize_r                    : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_h3d_vsize_r                    : 12;  // 27:16     
} PE_H15A0_MSC_H3D_Y_INSIZE2_T;

/*-----------------------------------------------------------------------------
		0xc8007654L msc_h3d_y_outsize ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_h3d_hosize                     : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_h3d_vosize                     : 12;  // 27:16     
} PE_H15A0_MSC_H3D_Y_OUTSIZE_T;

/*-----------------------------------------------------------------------------
		0xc8007658L msc_h3d_y_merge_param1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_h3d_active_merge_l             : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_h3d_overlap_merge_l            : 13;  // 28:16     
} PE_H15A0_MSC_H3D_Y_MERGE_PARAM1_T;

/*-----------------------------------------------------------------------------
		0xc800765cL msc_h3d_y_merge_param2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_h3d_active_merge_r             : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_h3d_overlap_merge_r            : 13;  // 28:16     
} PE_H15A0_MSC_H3D_Y_MERGE_PARAM2_T;

/*-----------------------------------------------------------------------------
		0xc8007660L msc_h3d_y_crop_param1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_h3d_hoffset                    : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_h3d_voffset                    : 12;  // 27:16     
} PE_H15A0_MSC_H3D_Y_CROP_PARAM1_T;

/*-----------------------------------------------------------------------------
		0xc8007664L msc_h3d_y_crop_param2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_h3d_hactive                    : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_h3d_vactive                    : 12;  // 27:16     
} PE_H15A0_MSC_H3D_Y_CROP_PARAM2_T;

/*-----------------------------------------------------------------------------
		0xc8007668L msc_h3d_y_crop_param3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_h3d_hisize                     : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_h3d_visize                     : 12;  // 27:16     
} PE_H15A0_MSC_H3D_Y_CROP_PARAM3_T;

/*-----------------------------------------------------------------------------
		0xc800766cL msc_h3d_y_phase_off ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_h3d_phase_off_v                : 9 ;  //  8: 0     
	UINT32                                    : 7 ;  // 15: 9     reserved
	UINT32 reg_h3d_phase_off_h                : 9 ;  // 24:16     
} PE_H15A0_MSC_H3D_Y_PHASE_OFF_T;

/*-----------------------------------------------------------------------------
		0xc8007670L msc_h3d_y_sampling_h ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_h3d_denominator_h              : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_h3d_numerator_h                : 13;  // 28:16     
} PE_H15A0_MSC_H3D_Y_SAMPLING_H_T;

/*-----------------------------------------------------------------------------
		0xc8007674L msc_h3d_y_sampling_v ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_h3d_denominator_v              : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 reg_h3d_numerator_v                : 12;  // 27:16     
} PE_H15A0_MSC_H3D_Y_SAMPLING_V_T;

/*-----------------------------------------------------------------------------
		0xc80076bcL msc_h3d_y_h1_fir_coef_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 update                             : 1 ;  //     0     
	UINT32 rnw                                : 1 ;  //     1     
	UINT32 addr                               : 8 ;  //  9: 2     
} PE_H15A0_MSC_H3D_Y_H1_FIR_COEF_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc80076c0L msc_h3d_y_h1_fir_coef_data0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient0                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient1                     : 12;  // 27:16     
} PE_H15A0_MSC_H3D_Y_H1_FIR_COEF_DATA0_T;

/*-----------------------------------------------------------------------------
		0xc80076c4L msc_h3d_y_h1_fir_coef_data1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient2                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient3                     : 12;  // 27:16     
} PE_H15A0_MSC_H3D_Y_H1_FIR_COEF_DATA1_T;

/*-----------------------------------------------------------------------------
		0xc80076c8L msc_h3d_y_h1_fir_coef_data2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient4                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient5                     : 12;  // 27:16     
} PE_H15A0_MSC_H3D_Y_H1_FIR_COEF_DATA2_T;

/*-----------------------------------------------------------------------------
		0xc80076ccL msc_h3d_y_h1_fir_coef_data3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient6                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient7                     : 12;  // 27:16     
} PE_H15A0_MSC_H3D_Y_H1_FIR_COEF_DATA3_T;

/*-----------------------------------------------------------------------------
		0xc80076d0L msc_h3d_y_h1_fir_coef_data4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient8                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient9                     : 12;  // 27:16     
} PE_H15A0_MSC_H3D_Y_H1_FIR_COEF_DATA4_T;

/*-----------------------------------------------------------------------------
		0xc80076d4L msc_h3d_y_h1_fir_coef_data5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient10                    : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient11                    : 12;  // 27:16     
} PE_H15A0_MSC_H3D_Y_H1_FIR_COEF_DATA5_T;

/*-----------------------------------------------------------------------------
		0xc80076d8L msc_m1_y_pre_write_on ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_mif_wr_on_pe0_l                : 1 ;  //     0     
} PE_H15A0_MSC_M1_Y_PRE_WRITE_ON_T;

/*-----------------------------------------------------------------------------
		0xc80076dcL msc_m2_y_pre_write_on ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_mif_wr_on_pe0_r                : 1 ;  //     0     
} PE_H15A0_MSC_M2_Y_PRE_WRITE_ON_T;

/*-----------------------------------------------------------------------------
		0xc80076e0L msc_m1_vcnt_out ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vcnt_out                           : 12;  // 11: 0     
} PE_H15A0_MSC_M1_Y_VCNT_OUT_T;

/*-----------------------------------------------------------------------------
		0xc80076e4L msc_m2_vcnt_out ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vcnt_out                           : 12;  // 11: 0     
} PE_H15A0_MSC_M2_Y_VCNT_OUT_T;

/*-----------------------------------------------------------------------------
		0xc80076e8L msc_h3d_vcnt_out ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vcnt_out                           : 12;  // 11: 0     
} PE_H15A0_MSC_H3D_Y_VCNT_OUT_T;

/*-----------------------------------------------------------------------------
		0xc80076ecL msc_m1_vcnt_in ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vcnt_in                            : 12;  // 11: 0     
} PE_H15A0_MSC_M1_Y_VCNT_IN_T;

/*-----------------------------------------------------------------------------
		0xc80076f0L msc_m2_vcnt_in ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vcnt_in                            : 12;  // 11: 0     
} PE_H15A0_MSC_M2_Y_VCNT_IN_T;

/*-----------------------------------------------------------------------------
		0xc80076f4L msc_h3d_vcnt_in ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vcnt_in                            : 12;  // 11: 0     
} PE_H15A0_MSC_H3D_Y_VCNT_IN_T;

/*-----------------------------------------------------------------------------
		0xc8007700L msc_m1_c_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_mif_rd_on_m1                   : 1 ;  //     0     
	UINT32 reg_mif_wr_on_m1                   : 1 ;  //     1     
	UINT32 reg_merge_on                       : 1 ;  //     2     
	UINT32 reg_vsc_mode_m1                    : 2 ;  //  4: 3     
	UINT32 reg_bilinear_on_m1                 : 1 ;  //     5     
	UINT32 reg_mode_32p_m1                    : 1 ;  //     6     
	UINT32                                    : 7 ;  // 13: 7     reserved
	UINT32 reg_min_max_on_m1                  : 1 ;  //    14     
	UINT32 reg_min_max_th_m1                  : 10;  // 24:15     
	UINT32 reg_v_reverse_m1                   : 1 ;  //    25     
	UINT32                                    : 1 ;  //    26     reserved
	UINT32 reg_min_max_on_h_m1                : 1 ;  //    27     
	UINT32 reg_v_cti_en_m1                    : 1 ;  //    28     
	UINT32 reg_v_cti_ratio_m1                 : 2 ;  // 30:29     
	UINT32 reg_sampling_mode_m1               : 1 ;  //    31     
} PE_H15A0_MSC_M1_C_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc8007704L msc_m1_c_mif_rd_size ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hsize_m1                       : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_vsize_m1                       : 12;  // 27:16     
} PE_H15A0_MSC_M1_C_MIF_RD_SIZE_T;

/*-----------------------------------------------------------------------------
		0xc8007708L msc_m1_c_outsize ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hosize_m1                      : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_vosize_m1                      : 12;  // 27:16     
} PE_H15A0_MSC_M1_C_OUTSIZE_T;

/*-----------------------------------------------------------------------------
		0xc800770cL msc_m1_c_canvas_write ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_canvas_xoffset_m1              : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 reg_canvas_yoffset_m1              : 11;  // 26:16     
} PE_H15A0_MSC_M1_C_CANVAS_WRITE_T;

/*-----------------------------------------------------------------------------
		0xc8007710L msc_m1_c_merge_param ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_active_merge_m1                : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_overlap_merge_m1               : 13;  // 28:16     
} PE_H15A0_MSC_M1_C_MERGE_PARAM_T;

/*-----------------------------------------------------------------------------
		0xc8007714L msc_m1_c_crop_param1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hoffset_m1                     : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_voffset_m1                     : 12;  // 27:16     
} PE_H15A0_MSC_M1_C_CROP_PARAM1_T;

/*-----------------------------------------------------------------------------
		0xc8007718L msc_m1_c_crop_param2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hactive_m1                     : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_vactive_m1                     : 12;  // 27:16     
} PE_H15A0_MSC_M1_C_CROP_PARAM2_T;

/*-----------------------------------------------------------------------------
		0xc800771cL msc_m1_c_crop_param3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hisize_m1                      : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_visize_m1                      : 12;  // 27:16     
} PE_H15A0_MSC_M1_C_CROP_PARAM3_T;

/*-----------------------------------------------------------------------------
		0xc8007720L msc_m1_c_phase_offset ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_phase_off_v_m1                 : 9 ;  //  8: 0     
	UINT32                                    : 7 ;  // 15: 9     reserved
	UINT32 reg_phase_off_h_m1                 : 9 ;  // 24:16     
} PE_H15A0_MSC_M1_C_PHASE_OFFSET_T;

/*-----------------------------------------------------------------------------
		0xc8007724L msc_m1_c_sampling_h ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_denominator_h_m1               : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_numerator_h_m1                 : 13;  // 28:16     
} PE_H15A0_MSC_M1_C_SAMPLING_H_T;

/*-----------------------------------------------------------------------------
		0xc8007728L msc_m1_c_sampling_v ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_denominator_v_m1               : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 reg_numerator_v_m1                 : 12;  // 27:16     
} PE_H15A0_MSC_M1_C_SAMPLING_V_T;

/*-----------------------------------------------------------------------------
		0xc800773cL msc_m1_c_chroma_vfilter ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_auto_chroma_filter_en_m1       : 1 ;  //     0     
	UINT32 reg_detect_level_th_m1             : 8 ;  //  8: 1     
	UINT32 reg_tearing_count_th_m1            : 8 ;  // 16: 9     
	UINT32 reg_frame_count_th_m1              : 4 ;  // 20:17     
	UINT32                                    : 8 ;  // 28:21     reserved
	UINT32 reg_manual_chroma_pre_filter_on_m1 : 1 ;  //    29     
} PE_H15A0_MSC_M1_C_CHROMA_VFILTER_T;

/*-----------------------------------------------------------------------------
		0xc8007740L pic_init_pe0_sel ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 16;  // 15: 0     reserved
	UINT32 pic_init_pe0_l_sel                 : 1 ;  //    16     
	UINT32 pic_init_pe0_r_sel                 : 1 ;  //    17     
} PE_H15A0_MSC_C_PIC_INIT_PE0_SEL_T;

/*-----------------------------------------------------------------------------
		0xc8007744L msc_m1_c_v1_fir_coef_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 update                             : 1 ;  //     0     
	UINT32 rnw                                : 1 ;  //     1     
	UINT32 addr                               : 8 ;  //  9: 2     
} PE_H15A0_MSC_M1_C_V1_FIR_COEF_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc8007748L msc_m1_c_v1_fir_coef_data0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient0                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient1                     : 12;  // 27:16     
} PE_H15A0_MSC_M1_C_V1_FIR_COEF_DATA0_T;

/*-----------------------------------------------------------------------------
		0xc800774cL msc_m1_c_v1_fir_coef_data1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient2                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient3                     : 12;  // 27:16     
} PE_H15A0_MSC_M1_C_V1_FIR_COEF_DATA1_T;

/*-----------------------------------------------------------------------------
		0xc8007750L msc_m1_c_v1_fir_coef_data2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient4                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient5                     : 12;  // 27:16     
} PE_H15A0_MSC_M1_C_V1_FIR_COEF_DATA2_T;

/*-----------------------------------------------------------------------------
		0xc8007754L msc_m1_c_v1_fir_coef_data3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient6                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient7                     : 12;  // 27:16     
} PE_H15A0_MSC_M1_C_V1_FIR_COEF_DATA3_T;

/*-----------------------------------------------------------------------------
		0xc8007758L msc_m1_c_v1_fir_coef_data4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient8                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient9                     : 12;  // 27:16     
} PE_H15A0_MSC_M1_C_V1_FIR_COEF_DATA4_T;

/*-----------------------------------------------------------------------------
		0xc800775cL msc_m1_c_v1_fir_coef_data5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient10                    : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient11                    : 12;  // 27:16     
} PE_H15A0_MSC_M1_C_V1_FIR_COEF_DATA5_T;

/*-----------------------------------------------------------------------------
		0xc8007774L msc_m1_c_h1_fir_coef_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 update                             : 1 ;  //     0     
	UINT32 rnw                                : 1 ;  //     1     
	UINT32 addr                               : 8 ;  //  9: 2     
} PE_H15A0_MSC_M1_C_H1_FIR_COEF_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc8007778L msc_m1_c_h1_fir_coef_data0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient0                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient1                     : 12;  // 27:16     
} PE_H15A0_MSC_M1_C_H1_FIR_COEF_DATA0_T;

/*-----------------------------------------------------------------------------
		0xc800777cL msc_m1_c_h1_fir_coef_data1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient2                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient3                     : 12;  // 27:16     
} PE_H15A0_MSC_M1_C_H1_FIR_COEF_DATA1_T;

/*-----------------------------------------------------------------------------
		0xc8007780L msc_m1_c_h1_fir_coef_data2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient4                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient5                     : 12;  // 27:16     
} PE_H15A0_MSC_M1_C_H1_FIR_COEF_DATA2_T;

/*-----------------------------------------------------------------------------
		0xc8007784L msc_m1_c_h1_fir_coef_data3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient6                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient7                     : 12;  // 27:16     
} PE_H15A0_MSC_M1_C_H1_FIR_COEF_DATA3_T;

/*-----------------------------------------------------------------------------
		0xc8007788L msc_m1_c_h1_fir_coef_data4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient8                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient9                     : 12;  // 27:16     
} PE_H15A0_MSC_M1_C_H1_FIR_COEF_DATA4_T;

/*-----------------------------------------------------------------------------
		0xc800778cL msc_m1_c_h1_fir_coef_data5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient10                    : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient11                    : 12;  // 27:16     
} PE_H15A0_MSC_M1_C_H1_FIR_COEF_DATA5_T;

/*-----------------------------------------------------------------------------
		0xc80077a4L msc_m2_c_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_mif_rd_on_m2                   : 1 ;  //     0     
	UINT32 reg_mif_rd_on_m2_sub               : 1 ;  //     1     
	UINT32 reg_mif_wr_on_m2                   : 1 ;  //     2     
	UINT32 reg_vsc_mode_m2                    : 2 ;  //  4: 3     
	UINT32 reg_bilinear_m2                    : 1 ;  //     5     
	UINT32 reg_mode_32p_m2                    : 1 ;  //     6     
	UINT32                                    : 7 ;  // 13: 7     reserved
	UINT32 reg_min_max_on_m2                  : 1 ;  //    14     
	UINT32 reg_min_max_th_m2                  : 10;  // 24:15     
	UINT32 reg_v_reverse_m2                   : 1 ;  //    25     
	UINT32 reg_v_reverse_m2_sub               : 1 ;  //    26     
	UINT32 reg_min_max_on_h_m2                : 1 ;  //    27     
	UINT32 reg_v_cti_en_m2                    : 1 ;  //    28     
	UINT32 reg_v_cti_m2atio_m2                : 2 ;  // 30:29     
	UINT32 reg_sampling_mode_m2               : 1 ;  //    31     
} PE_H15A0_MSC_M2_C_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc80077a8L msc_m2_c_mif_rd_size ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hsize_m2                       : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_vsize_m2                       : 12;  // 27:16     
} PE_H15A0_MSC_M2_C_MIF_RD_SIZE_T;

/*-----------------------------------------------------------------------------
		0xc80077acL msc_m2_c_outsize ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hosize_m2                      : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_vosize_m2                      : 12;  // 27:16     
} PE_H15A0_MSC_M2_C_OUTSIZE_T;

/*-----------------------------------------------------------------------------
		0xc80077b0L msc_m2_c_canvas_write ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_canvas_xoffset_m2              : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 reg_canvas_yoffset_m2              : 11;  // 26:16     
} PE_H15A0_MSC_M2_C_CANVAS_WRITE_T;

/*-----------------------------------------------------------------------------
		0xc80077b4L msc_m2_c_merge_param ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_active_merge_m2                : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_overlap_merge_m2               : 13;  // 28:16     
} PE_H15A0_MSC_M2_C_MERGE_PARAM_T;

/*-----------------------------------------------------------------------------
		0xc80077b8L msc_m2_c_crop_param1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hoffset_m2                     : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_voffset_m2                     : 12;  // 27:16     
} PE_H15A0_MSC_M2_C_CROP_PARAM1_T;

/*-----------------------------------------------------------------------------
		0xc80077bcL msc_m2_c_crop_param2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hactive_m2                     : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_vactive_m2                     : 12;  // 27:16     
} PE_H15A0_MSC_M2_C_CROP_PARAM2_T;

/*-----------------------------------------------------------------------------
		0xc80077c0L msc_m2_c_crop_param3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hisize_m2                      : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_visize_m2                      : 12;  // 27:16     
} PE_H15A0_MSC_M2_C_CROP_PARAM3_T;

/*-----------------------------------------------------------------------------
		0xc80077c4L msc_m2_c_phase_offset ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_phase_off_v_m2                 : 9 ;  //  8: 0     
	UINT32                                    : 7 ;  // 15: 9     reserved
	UINT32 reg_phase_off_h_m2                 : 9 ;  // 24:16     
} PE_H15A0_MSC_M2_C_PHASE_OFFSET_T;

/*-----------------------------------------------------------------------------
		0xc80077c8L msc_m2_c_sampling_h ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_denominator_h_m2               : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_numerator_h_m2                 : 13;  // 28:16     
} PE_H15A0_MSC_M2_C_SAMPLING_H_T;

/*-----------------------------------------------------------------------------
		0xc80077ccL msc_m2_c_sampling_v ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_denominator_v_m2               : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 reg_numerator_v_m2                 : 12;  // 27:16     
} PE_H15A0_MSC_M2_C_SAMPLING_V_T;

/*-----------------------------------------------------------------------------
		0xc80077d0L msc_m2_sub_c_mif_rd_size ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hsize_m2_sub                   : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_vsize_m2_sub                   : 12;  // 27:16     
} PE_H15A0_MSC_M2_SUB_C_MIF_RD_SIZE_T;

/*-----------------------------------------------------------------------------
		0xc80077d4L msc_m2_sub_c_crop_param1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hoffset_m2_sub                 : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_voffset_m2_sub                 : 12;  // 27:16     
} PE_H15A0_MSC_M2_SUB_C_CROP_PARAM1_T;

/*-----------------------------------------------------------------------------
		0xc80077d8L msc_m2_sub_c_crop_param2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hactive_m2_sub                 : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_vactive_m2_sub                 : 12;  // 27:16     
} PE_H15A0_MSC_M2_SUB_C_CROP_PARAM2_T;

/*-----------------------------------------------------------------------------
		0xc80077dcL msc_m2_sub_c_crop_param3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hisize_m2_sub                  : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_visize_m2_sub                  : 12;  // 27:16     
} PE_H15A0_MSC_M2_SUB_C_CROP_PARAM3_T;

/*-----------------------------------------------------------------------------
		0xc80077e0L msc_m2_c_chroma_vfilter ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_auto_chroma_filter_en_m2       : 1 ;  //     0     
	UINT32 reg_detect_m1evel_th_m2            : 8 ;  //  8: 1     
	UINT32 reg_tearing_count_th_m2            : 8 ;  // 16: 9     
	UINT32 reg_frame_count_th_m2              : 4 ;  // 20:17     
	UINT32                                    : 8 ;  // 28:21     reserved
	UINT32 reg_manual_chroma_pre_filter_on_m2 : 1 ;  //    29     
} PE_H15A0_MSC_M2_C_CHROMA_VFILTER_T;

/*-----------------------------------------------------------------------------
		0xc80077e8L msc_m2_c_v1_fir_coef_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 update                             : 1 ;  //     0     
	UINT32 rnw                                : 1 ;  //     1     
	UINT32 addr                               : 8 ;  //  9: 2     
} PE_H15A0_MSC_M2_C_V1_FIR_COEF_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc80077ecL msc_m2_c_v1_c_fir_coef_data0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient0                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient1                     : 12;  // 27:16     
} PE_H15A0_MSC_M2_C_V1_C_FIR_COEF_DATA0_T;

/*-----------------------------------------------------------------------------
		0xc80077f0L msc_m2_c_v1_c_fir_coef_data1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient2                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient3                     : 12;  // 27:16     
} PE_H15A0_MSC_M2_C_V1_C_FIR_COEF_DATA1_T;

/*-----------------------------------------------------------------------------
		0xc80077f4L msc_m2_c_v1_c_fir_coef_data2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient4                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient5                     : 12;  // 27:16     
} PE_H15A0_MSC_M2_C_V1_C_FIR_COEF_DATA2_T;

/*-----------------------------------------------------------------------------
		0xc80077f8L msc_m2_c_v1_c_fir_coef_data3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient6                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient7                     : 12;  // 27:16     
} PE_H15A0_MSC_M2_C_V1_C_FIR_COEF_DATA3_T;

/*-----------------------------------------------------------------------------
		0xc80077fcL msc_m2_c_v1_c_fir_coef_data4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient8                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient9                     : 12;  // 27:16     
} PE_H15A0_MSC_M2_C_V1_C_FIR_COEF_DATA4_T;

/*-----------------------------------------------------------------------------
		0xc8007800L msc_m2_c_v1_c_fir_coef_data5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient10                    : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient11                    : 12;  // 27:16     
} PE_H15A0_MSC_M2_C_V1_C_FIR_COEF_DATA5_T;

/*-----------------------------------------------------------------------------
		0xc8007818L msc_m2_c_h1_fir_coef_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 update                             : 1 ;  //     0     
	UINT32 rnw                                : 1 ;  //     1     
	UINT32 addr                               : 8 ;  //  9: 2     
} PE_H15A0_MSC_M2_C_H1_FIR_COEF_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc800781cL msc_m2_c_h1_c_fir_coef_data0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient0                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient1                     : 12;  // 27:16     
} PE_H15A0_MSC_M2_C_H1_C_FIR_COEF_DATA0_T;

/*-----------------------------------------------------------------------------
		0xc8007820L msc_m2_c_h1_c_fir_coef_data1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient2                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient3                     : 12;  // 27:16     
} PE_H15A0_MSC_M2_C_H1_C_FIR_COEF_DATA1_T;

/*-----------------------------------------------------------------------------
		0xc8007824L msc_m2_c_h1_c_fir_coef_data2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient4                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient5                     : 12;  // 27:16     
} PE_H15A0_MSC_M2_C_H1_C_FIR_COEF_DATA2_T;

/*-----------------------------------------------------------------------------
		0xc8007828L msc_m2_c_h1_c_fir_coef_data3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient6                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient7                     : 12;  // 27:16     
} PE_H15A0_MSC_M2_C_H1_C_FIR_COEF_DATA3_T;

/*-----------------------------------------------------------------------------
		0xc800782cL msc_m2_c_h1_c_fir_coef_data4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient8                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient9                     : 12;  // 27:16     
} PE_H15A0_MSC_M2_C_H1_C_FIR_COEF_DATA4_T;

/*-----------------------------------------------------------------------------
		0xc8007830L msc_m2_c_h1_c_fir_coef_data5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient10                    : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient11                    : 12;  // 27:16     
} PE_H15A0_MSC_M2_C_H1_C_FIR_COEF_DATA5_T;

/*-----------------------------------------------------------------------------
		0xc8007848L msc_h3d_c_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_h3d_half_on                    : 1 ;  //     0     
	UINT32 reg_h3d_pe0_sel                    : 1 ;  //     1     
	UINT32 reg_h3d_on                         : 1 ;  //     2     
	UINT32 reg_h3d_merge_on                   : 1 ;  //     3     
	UINT32 reg_h3d_bilinear_on                : 1 ;  //     4     
	UINT32                                    : 24;  // 28: 5     reserved
	UINT32 reg_h3d_mode_32p                   : 1 ;  //    29     
	UINT32 reg_h3d_flow_ctrl_on               : 1 ;  //    30     
	UINT32 reg_h3d_sampling_mode              : 1 ;  //    31     
} PE_H15A0_MSC_H3D_C_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc800784cL msc_h3d_c_insize1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_h3d_hsize_m1                   : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_h3d_vsize_m1                   : 12;  // 27:16     
} PE_H15A0_MSC_H3D_C_INSIZE1_T;

/*-----------------------------------------------------------------------------
		0xc8007850L msc_h3d_c_insize2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_h3d_hsize_m2                   : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_h3d_vsize_m2                   : 12;  // 27:16     
} PE_H15A0_MSC_H3D_C_INSIZE2_T;

/*-----------------------------------------------------------------------------
		0xc8007854L msc_h3d_c_outsize ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_h3d_hosize                     : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_h3d_vosize                     : 12;  // 27:16     
} PE_H15A0_MSC_H3D_C_OUTSIZE_T;

/*-----------------------------------------------------------------------------
		0xc8007858L msc_h3d_c_merge_param1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_h3d_active_merge_m1            : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_h3d_overlap_merge_m1           : 13;  // 28:16     
} PE_H15A0_MSC_H3D_C_MERGE_PARAM1_T;

/*-----------------------------------------------------------------------------
		0xc800785cL msc_h3d_c_merge_param2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_h3d_active_merge_m2            : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_h3d_overlap_merge_m2           : 13;  // 28:16     
} PE_H15A0_MSC_H3D_C_MERGE_PARAM2_T;

/*-----------------------------------------------------------------------------
		0xc8007860L msc_h3d_c_crop_param1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_h3d_hoffset                    : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_h3d_voffset                    : 12;  // 27:16     
} PE_H15A0_MSC_H3D_C_CROP_PARAM1_T;

/*-----------------------------------------------------------------------------
		0xc8007864L msc_h3d_c_crop_param2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_h3d_hactive                    : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_h3d_vactive                    : 12;  // 27:16     
} PE_H15A0_MSC_H3D_C_CROP_PARAM2_T;

/*-----------------------------------------------------------------------------
		0xc8007868L msc_h3d_c_crop_param3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_h3d_hisize                     : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_h3d_visize                     : 12;  // 27:16     
} PE_H15A0_MSC_H3D_C_CROP_PARAM3_T;

/*-----------------------------------------------------------------------------
		0xc800786cL msc_h3d_c_phase_off ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_h3d_phase_off_v                : 9 ;  //  8: 0     
	UINT32                                    : 7 ;  // 15: 9     reserved
	UINT32 reg_h3d_phase_off_h                : 9 ;  // 24:16     
} PE_H15A0_MSC_H3D_C_PHASE_OFF_T;

/*-----------------------------------------------------------------------------
		0xc8007870L msc_h3d_c_sampling_h ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_h3d_denominator_h              : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_h3d_numerator_h                : 13;  // 28:16     
} PE_H15A0_MSC_H3D_C_SAMPLING_H_T;

/*-----------------------------------------------------------------------------
		0xc8007874L msc_h3d_c_sampling_v ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_h3d_denominator_v              : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 reg_h3d_numerator_v                : 12;  // 27:16     
} PE_H15A0_MSC_H3D_C_SAMPLING_V_T;

/*-----------------------------------------------------------------------------
		0xc80078d8L msc_m1_c_pre_write_on ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_mif_wr_on_pe0_m1               : 1 ;  //     0     
} PE_H15A0_MSC_M1_C_PRE_WRITE_ON_T;

/*-----------------------------------------------------------------------------
		0xc80078dcL msc_m2_c_pre_write_on ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_mif_wr_on_pe0_m2               : 1 ;  //     0     
} PE_H15A0_MSC_M2_C_PRE_WRITE_ON_T;

/*-----------------------------------------------------------------------------
		0xc80078e0L msc_m1_vcnt_out ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vcnt_out                           : 12;  // 11: 0     
} PE_H15A0_MSC_M1_C_VCNT_OUT_T;

/*-----------------------------------------------------------------------------
		0xc80078e4L msc_m2_vcnt_out ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vcnt_out                           : 12;  // 11: 0     
} PE_H15A0_MSC_M2_C_VCNT_OUT_T;

/*-----------------------------------------------------------------------------
		0xc80078e8L msc_h3d_vcnt_out ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vcnt_out                           : 12;  // 11: 0     
} PE_H15A0_MSC_H3D_C_VCNT_OUT_T;

/*-----------------------------------------------------------------------------
		0xc80078ecL msc_m1_vcnt_in ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vcnt_in                            : 12;  // 11: 0     
} PE_H15A0_MSC_M1_C_VCNT_IN_T;

/*-----------------------------------------------------------------------------
		0xc80078f0L msc_m2_vcnt_in ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vcnt_in                            : 12;  // 11: 0     
} PE_H15A0_MSC_M2_C_VCNT_IN_T;

/*-----------------------------------------------------------------------------
		0xc80078f4L msc_h3d_vcnt_in ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vcnt_in                            : 12;  // 11: 0     
} PE_H15A0_MSC_H3D_C_VCNT_IN_T;

typedef struct {
	PE_H15A0_MSC_M1_Y_CTRL_T              	msc_m1_y_ctrl;                //0xc8007500L
	PE_H15A0_MSC_M1_Y_MIF_RD_SIZE_T       	msc_m1_y_mif_rd_size;         //0xc8007504L
	PE_H15A0_MSC_M1_Y_OUTSIZE_T           	msc_m1_y_outsize;             //0xc8007508L
	PE_H15A0_MSC_M1_Y_CANVAS_WRITE_T      	msc_m1_y_canvas_write;        //0xc800750cL
	PE_H15A0_MSC_M1_Y_MERGE_PARAM_T       	msc_m1_y_merge_param;         //0xc8007510L
	PE_H15A0_MSC_M1_Y_CROP_PARAM1_T       	msc_m1_y_crop_param1;         //0xc8007514L
	PE_H15A0_MSC_M1_Y_CROP_PARAM2_T       	msc_m1_y_crop_param2;         //0xc8007518L
	PE_H15A0_MSC_M1_Y_CROP_PARAM3_T       	msc_m1_y_crop_param3;         //0xc800751cL
	PE_H15A0_MSC_M1_Y_PHASE_OFFSET_T      	msc_m1_y_phase_offset;        //0xc8007520L
	PE_H15A0_MSC_M1_Y_SAMPLING_H_T        	msc_m1_y_sampling_h;          //0xc8007524L
	PE_H15A0_MSC_M1_Y_SAMPLING_V_T        	msc_m1_y_sampling_v;          //0xc8007528L
	UINT32                             	    reserved00[5];                //0xc800752cL~0xc800753cL
	PE_H15A0_MSC_Y_PIC_INIT_PE0_SEL_T       pic_init_pe0_sel;             //0xc8007540L
	PE_H15A0_MSC_M1_Y_V1_FIR_COEF_CTRL_T  	msc_m1_y_v1_fir_coef_ctrl;    //0xc8007544L
	PE_H15A0_MSC_M1_Y_V1_Y_FIR_COEF_DATA0_T	msc_m1_y_v1_y_fir_coef_data0; //0xc8007548L
	PE_H15A0_MSC_M1_Y_V1_Y_FIR_COEF_DATA1_T	msc_m1_y_v1_y_fir_coef_data1; //0xc800754cL
	PE_H15A0_MSC_M1_Y_V1_Y_FIR_COEF_DATA2_T	msc_m1_y_v1_y_fir_coef_data2; //0xc8007550L
	PE_H15A0_MSC_M1_Y_V1_Y_FIR_COEF_DATA3_T	msc_m1_y_v1_y_fir_coef_data3; //0xc8007554L
	PE_H15A0_MSC_M1_Y_V1_Y_FIR_COEF_DATA4_T	msc_m1_y_v1_y_fir_coef_data4; //0xc8007558L
	PE_H15A0_MSC_M1_Y_V1_Y_FIR_COEF_DATA5_T	msc_m1_y_v1_y_fir_coef_data5; //0xc800755cL
	PE_H15A0_MSC_M1_Y_V2_FIR_COEF_CTRL_T  	msc_m1_y_v2_fir_coef_ctrl;    //0xc8007560L
	PE_H15A0_MSC_M1_Y_V2_Y_FIR_COEF_DATA0_T	msc_m1_y_v2_y_fir_coef_data0; //0xc8007564L
	PE_H15A0_MSC_M1_Y_V2_Y_FIR_COEF_DATA1_T	msc_m1_y_v2_y_fir_coef_data1; //0xc8007568L
	PE_H15A0_MSC_M1_Y_V2_Y_FIR_COEF_DATA2_T	msc_m1_y_v2_y_fir_coef_data2; //0xc800756cL
	PE_H15A0_MSC_M1_Y_V2_Y_FIR_COEF_DATA3_T	msc_m1_y_v2_y_fir_coef_data3; //0xc8007570L
	PE_H15A0_MSC_M1_Y_H1_FIR_COEF_CTRL_T  	msc_m1_y_h1_fir_coef_ctrl;    //0xc8007574L
	PE_H15A0_MSC_M1_Y_H1_Y_FIR_COEF_DATA0_T	msc_m1_y_h1_y_fir_coef_data0; //0xc8007578L
	PE_H15A0_MSC_M1_Y_H1_Y_FIR_COEF_DATA1_T	msc_m1_y_h1_y_fir_coef_data1; //0xc800757cL
	PE_H15A0_MSC_M1_Y_H1_Y_FIR_COEF_DATA2_T	msc_m1_y_h1_y_fir_coef_data2; //0xc8007580L
	PE_H15A0_MSC_M1_Y_H1_Y_FIR_COEF_DATA3_T	msc_m1_y_h1_y_fir_coef_data3; //0xc8007584L
	PE_H15A0_MSC_M1_Y_H1_Y_FIR_COEF_DATA4_T	msc_m1_y_h1_y_fir_coef_data4; //0xc8007588L
	PE_H15A0_MSC_M1_Y_H1_Y_FIR_COEF_DATA5_T	msc_m1_y_h1_y_fir_coef_data5; //0xc800758cL
	PE_H15A0_MSC_M1_Y_H2_FIR_COEF_CTRL_T  	msc_m1_y_h2_fir_coef_ctrl;    //0xc8007590L
	PE_H15A0_MSC_M1_Y_H2_Y_FIR_COEF_DATA0_T	msc_m1_y_h2_y_fir_coef_data0; //0xc8007594L
	PE_H15A0_MSC_M1_Y_H2_Y_FIR_COEF_DATA1_T	msc_m1_y_h2_y_fir_coef_data1; //0xc8007598L
	PE_H15A0_MSC_M1_Y_H2_Y_FIR_COEF_DATA2_T	msc_m1_y_h2_y_fir_coef_data2; //0xc800759cL
	PE_H15A0_MSC_M1_Y_H2_Y_FIR_COEF_DATA3_T	msc_m1_y_h2_y_fir_coef_data3; //0xc80075a0L
	PE_H15A0_MSC_M2_Y_CTRL_T              	msc_m2_y_ctrl;                //0xc80075a4L
	PE_H15A0_MSC_M2_Y_MIF_RD_SIZE_T       	msc_m2_y_mif_rd_size;         //0xc80075a8L
	PE_H15A0_MSC_M2_Y_OUTSIZE_T           	msc_m2_y_outsize;             //0xc80075acL
	PE_H15A0_MSC_M2_Y_CANVAS_WRITE_T      	msc_m2_y_canvas_write;        //0xc80075b0L
	PE_H15A0_MSC_M2_Y_MERGE_PARAM_T       	msc_m2_y_merge_param;         //0xc80075b4L
	PE_H15A0_MSC_M2_Y_CROP_PARAM1_T       	msc_m2_y_crop_param1;         //0xc80075b8L
	PE_H15A0_MSC_M2_Y_CROP_PARAM2_T       	msc_m2_y_crop_param2;         //0xc80075bcL
	PE_H15A0_MSC_M2_Y_CROP_PARAM3_T       	msc_m2_y_crop_param3;         //0xc80075c0L
	PE_H15A0_MSC_M2_Y_PHASE_OFFSET_T      	msc_m2_y_phase_offset;        //0xc80075c4L
	PE_H15A0_MSC_M2_Y_SAMPLING_H_T        	msc_m2_y_sampling_h;          //0xc80075c8L
	PE_H15A0_MSC_M2_Y_SAMPLING_V_T        	msc_m2_y_sampling_v;          //0xc80075ccL
	PE_H15A0_MSC_M2_SUB_Y_MIF_RD_SIZE_T   	msc_m2_sub_y_mif_rd_size;     //0xc80075d0L
	PE_H15A0_MSC_M2_SUB_Y_CROP_PARAM1_T   	msc_m2_sub_y_crop_param1;     //0xc80075d4L
	PE_H15A0_MSC_M2_SUB_Y_CROP_PARAM2_T   	msc_m2_sub_y_crop_param2;     //0xc80075d8L
	PE_H15A0_MSC_M2_SUB_Y_CROP_PARAM3_T   	msc_m2_sub_y_crop_param3;     //0xc80075dcL
	UINT32                             	    reserved01[2];                //0xc80075e0L~0xc80075e4L
	PE_H15A0_MSC_M2_Y_V1_FIR_COEF_CTRL_T  	msc_m2_y_v1_fir_coef_ctrl;    //0xc80075e8L
	PE_H15A0_MSC_M2_Y_V1_FIR_COEF_DATA0_T 	msc_m2_y_v1_fir_coef_data0;   //0xc80075ecL
	PE_H15A0_MSC_M2_Y_V1_FIR_COEF_DATA1_T 	msc_m2_y_v1_fir_coef_data1;   //0xc80075f0L
	PE_H15A0_MSC_M2_Y_V1_FIR_COEF_DATA2_T 	msc_m2_y_v1_fir_coef_data2;   //0xc80075f4L
	PE_H15A0_MSC_M2_Y_V1_FIR_COEF_DATA3_T 	msc_m2_y_v1_fir_coef_data3;   //0xc80075f8L
	PE_H15A0_MSC_M2_Y_V1_FIR_COEF_DATA4_T 	msc_m2_y_v1_fir_coef_data4;   //0xc80075fcL
	PE_H15A0_MSC_M2_Y_V1_FIR_COEF_DATA5_T 	msc_m2_y_v1_fir_coef_data5;   //0xc8007600L
	PE_H15A0_MSC_M2_Y_V2_FIR_COEF_CTRL_T  	msc_m2_y_v2_fir_coef_ctrl;    //0xc8007604L
	PE_H15A0_MSC_M2_Y_V2_FIR_COEF_DATA0_T 	msc_m2_y_v2_fir_coef_data0;   //0xc8007608L
	PE_H15A0_MSC_M2_Y_V2_FIR_COEF_DATA1_T 	msc_m2_y_v2_fir_coef_data1;   //0xc800760cL
	PE_H15A0_MSC_M2_Y_V2_FIR_COEF_DATA2_T 	msc_m2_y_v2_fir_coef_data2;   //0xc8007610L
	PE_H15A0_MSC_M2_Y_V2_FIR_COEF_DATA3_T 	msc_m2_y_v2_fir_coef_data3;   //0xc8007614L
	PE_H15A0_MSC_M2_Y_H1_FIR_COEF_CTRL_T  	msc_m2_y_h1_fir_coef_ctrl;    //0xc8007618L
	PE_H15A0_MSC_M2_Y_H1_FIR_COEF_DATA0_T 	msc_m2_y_h1_fir_coef_data0;   //0xc800761cL
	PE_H15A0_MSC_M2_Y_H1_FIR_COEF_DATA1_T 	msc_m2_y_h1_fir_coef_data1;   //0xc8007620L
	PE_H15A0_MSC_M2_Y_H1_FIR_COEF_DATA2_T 	msc_m2_y_h1_fir_coef_data2;   //0xc8007624L
	PE_H15A0_MSC_M2_Y_H1_FIR_COEF_DATA3_T 	msc_m2_y_h1_fir_coef_data3;   //0xc8007628L
	PE_H15A0_MSC_M2_Y_H1_FIR_COEF_DATA4_T 	msc_m2_y_h1_fir_coef_data4;   //0xc800762cL
	PE_H15A0_MSC_M2_Y_H1_FIR_COEF_DATA5_T 	msc_m2_y_h1_fir_coef_data5;   //0xc8007630L
	PE_H15A0_MSC_M2_Y_H2_FIR_COEF_CTRL_T  	msc_m2_y_h2_fir_coef_ctrl;    //0xc8007634L
	PE_H15A0_MSC_M2_Y_H2_FIR_COEF_DATA0_T 	msc_m2_y_h2_fir_coef_data0;   //0xc8007638L
	PE_H15A0_MSC_M2_Y_H2_FIR_COEF_DATA1_T 	msc_m2_y_h2_fir_coef_data1;   //0xc800763cL
	PE_H15A0_MSC_M2_Y_H2_FIR_COEF_DATA2_T 	msc_m2_y_h2_fir_coef_data2;   //0xc8007640L
	PE_H15A0_MSC_M2_Y_H2_FIR_COEF_DATA3_T 	msc_m2_y_h2_fir_coef_data3;   //0xc8007644L
	PE_H15A0_MSC_H3D_Y_CTRL_T             	msc_h3d_y_ctrl;               //0xc8007648L
	PE_H15A0_MSC_H3D_Y_INSIZE1_T          	msc_h3d_y_insize1;            //0xc800764cL
	PE_H15A0_MSC_H3D_Y_INSIZE2_T          	msc_h3d_y_insize2;            //0xc8007650L
	PE_H15A0_MSC_H3D_Y_OUTSIZE_T          	msc_h3d_y_outsize;            //0xc8007654L
	PE_H15A0_MSC_H3D_Y_MERGE_PARAM1_T     	msc_h3d_y_merge_param1;       //0xc8007658L
	PE_H15A0_MSC_H3D_Y_MERGE_PARAM2_T     	msc_h3d_y_merge_param2;       //0xc800765cL
	PE_H15A0_MSC_H3D_Y_CROP_PARAM1_T      	msc_h3d_y_crop_param1;        //0xc8007660L
	PE_H15A0_MSC_H3D_Y_CROP_PARAM2_T      	msc_h3d_y_crop_param2;        //0xc8007664L
	PE_H15A0_MSC_H3D_Y_CROP_PARAM3_T      	msc_h3d_y_crop_param3;        //0xc8007668L
	PE_H15A0_MSC_H3D_Y_PHASE_OFF_T        	msc_h3d_y_phase_off;          //0xc800766cL
	PE_H15A0_MSC_H3D_Y_SAMPLING_H_T       	msc_h3d_y_sampling_h;         //0xc8007670L
	PE_H15A0_MSC_H3D_Y_SAMPLING_V_T       	msc_h3d_y_sampling_v;         //0xc8007674L
	UINT32                             	    reserved02[17];               //0xc8007678L~0xc80076b8L
	PE_H15A0_MSC_H3D_Y_H1_FIR_COEF_CTRL_T 	msc_h3d_y_h1_fir_coef_ctrl;   //0xc80076bcL
	PE_H15A0_MSC_H3D_Y_H1_FIR_COEF_DATA0_T	msc_h3d_y_h1_fir_coef_data0;  //0xc80076c0L
	PE_H15A0_MSC_H3D_Y_H1_FIR_COEF_DATA1_T	msc_h3d_y_h1_fir_coef_data1;  //0xc80076c4L
	PE_H15A0_MSC_H3D_Y_H1_FIR_COEF_DATA2_T	msc_h3d_y_h1_fir_coef_data2;  //0xc80076c8L
	PE_H15A0_MSC_H3D_Y_H1_FIR_COEF_DATA3_T	msc_h3d_y_h1_fir_coef_data3;  //0xc80076ccL
	PE_H15A0_MSC_H3D_Y_H1_FIR_COEF_DATA4_T	msc_h3d_y_h1_fir_coef_data4;  //0xc80076d0L
	PE_H15A0_MSC_H3D_Y_H1_FIR_COEF_DATA5_T	msc_h3d_y_h1_fir_coef_data5;  //0xc80076d4L
	PE_H15A0_MSC_M1_Y_PRE_WRITE_ON_T      	msc_m1_y_pre_write_on;        //0xc80076d8L
	PE_H15A0_MSC_M2_Y_PRE_WRITE_ON_T      	msc_m2_y_pre_write_on;        //0xc80076dcL
	PE_H15A0_MSC_M1_Y_VCNT_OUT_T            msc_m1_vcnt_out;              //0xc80076e0L
	PE_H15A0_MSC_M2_Y_VCNT_OUT_T            msc_m2_vcnt_out;              //0xc80076e4L
	PE_H15A0_MSC_H3D_Y_VCNT_OUT_T           msc_h3d_vcnt_out;             //0xc80076e8L
	PE_H15A0_MSC_M1_Y_VCNT_IN_T             msc_m1_vcnt_in;               //0xc80076ecL
	PE_H15A0_MSC_M2_Y_VCNT_IN_T             msc_m2_vcnt_in;               //0xc80076f0L
	PE_H15A0_MSC_H3D_Y_VCNT_IN_T            msc_h3d_vcnt_in;              //0xc80076f4L
}PE_MSC_Y_REG_H15A0_T;

typedef struct {
	PE_H15A0_MSC_M1_C_CTRL_T              	msc_m1_c_ctrl;                //0xc8007700L
	PE_H15A0_MSC_M1_C_MIF_RD_SIZE_T       	msc_m1_c_mif_rd_size;         //0xc8007704L
	PE_H15A0_MSC_M1_C_OUTSIZE_T           	msc_m1_c_outsize;             //0xc8007708L
	PE_H15A0_MSC_M1_C_CANVAS_WRITE_T      	msc_m1_c_canvas_write;        //0xc800770cL
	PE_H15A0_MSC_M1_C_MERGE_PARAM_T       	msc_m1_c_merge_param;         //0xc8007710L
	PE_H15A0_MSC_M1_C_CROP_PARAM1_T       	msc_m1_c_crop_param1;         //0xc8007714L
	PE_H15A0_MSC_M1_C_CROP_PARAM2_T       	msc_m1_c_crop_param2;         //0xc8007718L
	PE_H15A0_MSC_M1_C_CROP_PARAM3_T       	msc_m1_c_crop_param3;         //0xc800771cL
	PE_H15A0_MSC_M1_C_PHASE_OFFSET_T      	msc_m1_c_phase_offset;        //0xc8007720L
	PE_H15A0_MSC_M1_C_SAMPLING_H_T        	msc_m1_c_sampling_h;          //0xc8007724L
	PE_H15A0_MSC_M1_C_SAMPLING_V_T        	msc_m1_c_sampling_v;          //0xc8007728L
	UINT32                             	    reserved00[4];                //0xc800772cL~0xc8007738L
	PE_H15A0_MSC_M1_C_CHROMA_VFILTER_T    	msc_m1_c_chroma_vfilter;      //0xc800773cL
	PE_H15A0_MSC_C_PIC_INIT_PE0_SEL_T       pic_init_pe0_sel;             //0xc8007740L
	PE_H15A0_MSC_M1_C_V1_FIR_COEF_CTRL_T  	msc_m1_c_v1_fir_coef_ctrl;    //0xc8007744L
	PE_H15A0_MSC_M1_C_V1_FIR_COEF_DATA0_T 	msc_m1_c_v1_fir_coef_data0;   //0xc8007748L
	PE_H15A0_MSC_M1_C_V1_FIR_COEF_DATA1_T 	msc_m1_c_v1_fir_coef_data1;   //0xc800774cL
	PE_H15A0_MSC_M1_C_V1_FIR_COEF_DATA2_T 	msc_m1_c_v1_fir_coef_data2;   //0xc8007750L
	PE_H15A0_MSC_M1_C_V1_FIR_COEF_DATA3_T 	msc_m1_c_v1_fir_coef_data3;   //0xc8007754L
	PE_H15A0_MSC_M1_C_V1_FIR_COEF_DATA4_T 	msc_m1_c_v1_fir_coef_data4;   //0xc8007758L
	PE_H15A0_MSC_M1_C_V1_FIR_COEF_DATA5_T 	msc_m1_c_v1_fir_coef_data5;   //0xc800775cL
	UINT32                             	    reserved01[5];                //0xc8007760L~0xc8007770L
	PE_H15A0_MSC_M1_C_H1_FIR_COEF_CTRL_T  	msc_m1_c_h1_fir_coef_ctrl;    //0xc8007774L
	PE_H15A0_MSC_M1_C_H1_FIR_COEF_DATA0_T 	msc_m1_c_h1_fir_coef_data0;   //0xc8007778L
	PE_H15A0_MSC_M1_C_H1_FIR_COEF_DATA1_T 	msc_m1_c_h1_fir_coef_data1;   //0xc800777cL
	PE_H15A0_MSC_M1_C_H1_FIR_COEF_DATA2_T 	msc_m1_c_h1_fir_coef_data2;   //0xc8007780L
	PE_H15A0_MSC_M1_C_H1_FIR_COEF_DATA3_T 	msc_m1_c_h1_fir_coef_data3;   //0xc8007784L
	PE_H15A0_MSC_M1_C_H1_FIR_COEF_DATA4_T 	msc_m1_c_h1_fir_coef_data4;   //0xc8007788L
	PE_H15A0_MSC_M1_C_H1_FIR_COEF_DATA5_T 	msc_m1_c_h1_fir_coef_data5;   //0xc800778cL
	UINT32                             	    reserved02[5];                //0xc8007790L~0xc80077a0L
	PE_H15A0_MSC_M2_C_CTRL_T              	msc_m2_c_ctrl;                //0xc80077a4L
	PE_H15A0_MSC_M2_C_MIF_RD_SIZE_T       	msc_m2_c_mif_rd_size;         //0xc80077a8L
	PE_H15A0_MSC_M2_C_OUTSIZE_T           	msc_m2_c_outsize;             //0xc80077acL
	PE_H15A0_MSC_M2_C_CANVAS_WRITE_T      	msc_m2_c_canvas_write;        //0xc80077b0L
	PE_H15A0_MSC_M2_C_MERGE_PARAM_T       	msc_m2_c_merge_param;         //0xc80077b4L
	PE_H15A0_MSC_M2_C_CROP_PARAM1_T       	msc_m2_c_crop_param1;         //0xc80077b8L
	PE_H15A0_MSC_M2_C_CROP_PARAM2_T       	msc_m2_c_crop_param2;         //0xc80077bcL
	PE_H15A0_MSC_M2_C_CROP_PARAM3_T       	msc_m2_c_crop_param3;         //0xc80077c0L
	PE_H15A0_MSC_M2_C_PHASE_OFFSET_T      	msc_m2_c_phase_offset;        //0xc80077c4L
	PE_H15A0_MSC_M2_C_SAMPLING_H_T        	msc_m2_c_sampling_h;          //0xc80077c8L
	PE_H15A0_MSC_M2_C_SAMPLING_V_T        	msc_m2_c_sampling_v;          //0xc80077ccL
	PE_H15A0_MSC_M2_SUB_C_MIF_RD_SIZE_T   	msc_m2_sub_c_mif_rd_size;     //0xc80077d0L
	PE_H15A0_MSC_M2_SUB_C_CROP_PARAM1_T   	msc_m2_sub_c_crop_param1;     //0xc80077d4L
	PE_H15A0_MSC_M2_SUB_C_CROP_PARAM2_T   	msc_m2_sub_c_crop_param2;     //0xc80077d8L
	PE_H15A0_MSC_M2_SUB_C_CROP_PARAM3_T   	msc_m2_sub_c_crop_param3;     //0xc80077dcL
	PE_H15A0_MSC_M2_C_CHROMA_VFILTER_T    	msc_m2_c_chroma_vfilter;      //0xc80077e0L
	UINT32                             	    reserved03;                   //0xc80077e4L
	PE_H15A0_MSC_M2_C_V1_FIR_COEF_CTRL_T  	msc_m2_c_v1_fir_coef_ctrl;    //0xc80077e8L
	PE_H15A0_MSC_M2_C_V1_C_FIR_COEF_DATA0_T	msc_m2_c_v1_c_fir_coef_data0; //0xc80077ecL
	PE_H15A0_MSC_M2_C_V1_C_FIR_COEF_DATA1_T	msc_m2_c_v1_c_fir_coef_data1; //0xc80077f0L
	PE_H15A0_MSC_M2_C_V1_C_FIR_COEF_DATA2_T	msc_m2_c_v1_c_fir_coef_data2; //0xc80077f4L
	PE_H15A0_MSC_M2_C_V1_C_FIR_COEF_DATA3_T	msc_m2_c_v1_c_fir_coef_data3; //0xc80077f8L
	PE_H15A0_MSC_M2_C_V1_C_FIR_COEF_DATA4_T	msc_m2_c_v1_c_fir_coef_data4; //0xc80077fcL
	PE_H15A0_MSC_M2_C_V1_C_FIR_COEF_DATA5_T	msc_m2_c_v1_c_fir_coef_data5; //0xc8007800L
	UINT32                             	    reserved04[5];                //0xc8007804L~0xc8007814L
	PE_H15A0_MSC_M2_C_H1_FIR_COEF_CTRL_T  	msc_m2_c_h1_fir_coef_ctrl;    //0xc8007818L
	PE_H15A0_MSC_M2_C_H1_C_FIR_COEF_DATA0_T	msc_m2_c_h1_c_fir_coef_data0; //0xc800781cL
	PE_H15A0_MSC_M2_C_H1_C_FIR_COEF_DATA1_T	msc_m2_c_h1_c_fir_coef_data1; //0xc8007820L
	PE_H15A0_MSC_M2_C_H1_C_FIR_COEF_DATA2_T	msc_m2_c_h1_c_fir_coef_data2; //0xc8007824L
	PE_H15A0_MSC_M2_C_H1_C_FIR_COEF_DATA3_T	msc_m2_c_h1_c_fir_coef_data3; //0xc8007828L
	PE_H15A0_MSC_M2_C_H1_C_FIR_COEF_DATA4_T	msc_m2_c_h1_c_fir_coef_data4; //0xc800782cL
	PE_H15A0_MSC_M2_C_H1_C_FIR_COEF_DATA5_T	msc_m2_c_h1_c_fir_coef_data5; //0xc8007830L
	UINT32                             	    reserved05[5];                //0xc8007834L~0xc8007844L
	PE_H15A0_MSC_H3D_C_CTRL_T             	msc_h3d_c_ctrl;               //0xc8007848L
	PE_H15A0_MSC_H3D_C_INSIZE1_T          	msc_h3d_c_insize1;            //0xc800784cL
	PE_H15A0_MSC_H3D_C_INSIZE2_T          	msc_h3d_c_insize2;            //0xc8007850L
	PE_H15A0_MSC_H3D_C_OUTSIZE_T          	msc_h3d_c_outsize;            //0xc8007854L
	PE_H15A0_MSC_H3D_C_MERGE_PARAM1_T     	msc_h3d_c_merge_param1;       //0xc8007858L
	PE_H15A0_MSC_H3D_C_MERGE_PARAM2_T     	msc_h3d_c_merge_param2;       //0xc800785cL
	PE_H15A0_MSC_H3D_C_CROP_PARAM1_T      	msc_h3d_c_crop_param1;        //0xc8007860L
	PE_H15A0_MSC_H3D_C_CROP_PARAM2_T      	msc_h3d_c_crop_param2;        //0xc8007864L
	PE_H15A0_MSC_H3D_C_CROP_PARAM3_T      	msc_h3d_c_crop_param3;        //0xc8007868L
	PE_H15A0_MSC_H3D_C_PHASE_OFF_T        	msc_h3d_c_phase_off;          //0xc800786cL
	PE_H15A0_MSC_H3D_C_SAMPLING_H_T       	msc_h3d_c_sampling_h;         //0xc8007870L
	PE_H15A0_MSC_H3D_C_SAMPLING_V_T       	msc_h3d_c_sampling_v;         //0xc8007874L
	UINT32                             	    reserved06[24];               //0xc8007878L~0xc80078d4L
	PE_H15A0_MSC_M1_C_PRE_WRITE_ON_T      	msc_m1_c_pre_write_on;        //0xc80078d8L
	PE_H15A0_MSC_M2_C_PRE_WRITE_ON_T      	msc_m2_c_pre_write_on;        //0xc80078dcL
	PE_H15A0_MSC_M1_C_VCNT_OUT_T            msc_m1_vcnt_out;              //0xc80078e0L
	PE_H15A0_MSC_M2_C_VCNT_OUT_T            msc_m2_vcnt_out;              //0xc80078e4L
	PE_H15A0_MSC_H3D_C_VCNT_OUT_T           msc_h3d_vcnt_out;             //0xc80078e8L
	PE_H15A0_MSC_M1_C_VCNT_IN_T             msc_m1_vcnt_in;               //0xc80078ecL
	PE_H15A0_MSC_M2_C_VCNT_IN_T             msc_m2_vcnt_in;               //0xc80078f0L
	PE_H15A0_MSC_H3D_C_VCNT_IN_T            msc_h3d_vcnt_in;              //0xc80078f4L
}PE_MSC_C_REG_H15A0_T;
#endif
