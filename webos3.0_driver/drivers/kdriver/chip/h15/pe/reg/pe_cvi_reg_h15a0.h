#ifndef _PE_CVI_REG_H15A0_H_
#define _PE_CVI_REG_H15A0_H_
/*-----------------------------------------------------------------------------
		0xc8000100L hdmi_mux_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 src_sel_e                          : 2 ;  //  1: 0     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 src_sel_o                          : 2 ;  //  5: 4     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 pre_mux_sel                        : 2 ;  //  9: 8     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 pre_out_mux_l_sel                  : 2 ;  // 17:16     
	UINT32                                    : 2 ;  // 19:18     reserved
	UINT32 pre_out_mux_r_sel                  : 2 ;  // 21:20     
} PE_H15A0_A_HDMI_MUX_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc8000104L hdmi_420in_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 yin_delay                          : 2 ;  //  1: 0     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 cbin_delay                         : 2 ;  //  5: 4     
	UINT32 cin_delay                          : 2 ;  //  7: 6     
	UINT32 y_swap                             : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 c_swap                             : 1 ;  //    12     
} PE_H15A0_A_HDMI_420IN_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc8000108L hdmi_hsc_ctrl0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hsc_en                             : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 sw_reset                           : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 sampling_mode                      : 1 ;  //     8     
	UINT32                                    : 7 ;  // 15: 9     reserved
	UINT32 phase_off                          : 9 ;  // 24:16     
} PE_H15A0_A_HDMI_HSC_CTRL0_T;

/*-----------------------------------------------------------------------------
		0xc800010cL hdmi_hsc_ctrl1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 in_pic_width                       : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 out_pic_width                      : 13;  // 28:16     
} PE_H15A0_A_HDMI_HSC_CTRL1_T;

/*-----------------------------------------------------------------------------
		0xc8000110L hdmi_hsc_ctrl2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 numerator                          : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 denominator                        : 13;  // 28:16     
} PE_H15A0_A_HDMI_HSC_CTRL2_T;

/*-----------------------------------------------------------------------------
		0xc8000114L hdmi_fir_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 fir_y_en                           : 1 ;  //     0     
	UINT32 fir_c_en                           : 1 ;  //     1     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 sum_sample_en                      : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 dual_en                            : 1 ;  //     8     
	UINT32                                    : 7 ;  // 15: 9     reserved
	UINT32 bound_cnt                          : 13;  // 28:16     
} PE_H15A0_A_HDMI_FIR_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc8000118L hdmi_fir_y_coef0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 fir_y_coef0                        : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 fir_y_coef1                        : 10;  // 25:16     
} PE_H15A0_A_HDMI_FIR_Y_COEF0_T;

/*-----------------------------------------------------------------------------
		0xc800011cL hdmi_fir_y_coef1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 fir_y_coef2                        : 10;  //  9: 0     
} PE_H15A0_A_HDMI_FIR_Y_COEF1_T;

/*-----------------------------------------------------------------------------
		0xc8000120L hdmi_fir_c_coef0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 fir_y_coef0                        : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 fir_y_coef1                        : 10;  // 25:16     
} PE_H15A0_A_HDMI_FIR_C_COEF0_T;

/*-----------------------------------------------------------------------------
		0xc8000124L hdmi_fir_c_coef1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 fir_y_coef2                        : 10;  //  9: 0     
} PE_H15A0_A_HDMI_FIR_C_COEF1_T;

/*-----------------------------------------------------------------------------
		0xc8000128L hdmi_eo2lr_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 left_max                           : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 right_min                          : 13;  // 28:16     
} PE_H15A0_A_HDMI_EO2LR_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc800012cL hdmi_rx_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 color_format                       : 2 ;  //  1: 0     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 color_domain                       : 1 ;  //     4     
} PE_H15A0_A_HDMI_RX_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc8000140L hdmi_mux_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 src_sel_e                          : 2 ;  //  1: 0     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 src_sel_o                          : 2 ;  //  5: 4     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 pre_mux_sel                        : 2 ;  //  9: 8     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 pre_out_mux_l_sel                  : 2 ;  // 17:16     
	UINT32                                    : 2 ;  // 19:18     reserved
	UINT32 pre_out_mux_r_sel                  : 2 ;  // 21:20     
} PE_H15A0_B_HDMI_MUX_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc8000144L hdmi_420in_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 yin_delay                          : 2 ;  //  1: 0     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 cbin_delay                         : 2 ;  //  5: 4     
	UINT32 cin_delay                          : 2 ;  //  7: 6     
	UINT32 y_swap                             : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 c_swap                             : 1 ;  //    12     
} PE_H15A0_B_HDMI_420IN_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc8000148L hdmi_hsc_ctrl0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hsc_en                             : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 sw_reset                           : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 sampling_mode                      : 1 ;  //     8     
	UINT32                                    : 7 ;  // 15: 9     reserved
	UINT32 phase_off                          : 9 ;  // 24:16     
} PE_H15A0_B_HDMI_HSC_CTRL0_T;

/*-----------------------------------------------------------------------------
		0xc800014cL hdmi_hsc_ctrl1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 in_pic_width                       : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 out_pic_width                      : 13;  // 28:16     
} PE_H15A0_B_HDMI_HSC_CTRL1_T;

/*-----------------------------------------------------------------------------
		0xc8000150L hdmi_hsc_ctrl2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 numerator                          : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 denominator                        : 13;  // 28:16     
} PE_H15A0_B_HDMI_HSC_CTRL2_T;

/*-----------------------------------------------------------------------------
		0xc8000154L hdmi_fir_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 fir_y_en                           : 1 ;  //     0     
	UINT32 fir_c_en                           : 1 ;  //     1     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 sum_sample_en                      : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 dual_en                            : 1 ;  //     8     
	UINT32                                    : 7 ;  // 15: 9     reserved
	UINT32 bound_cnt                          : 13;  // 28:16     
} PE_H15A0_B_HDMI_FIR_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc8000158L hdmi_fir_y_coef0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 fir_y_coef0                        : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 fir_y_coef1                        : 10;  // 25:16     
} PE_H15A0_B_HDMI_FIR_Y_COEF0_T;

/*-----------------------------------------------------------------------------
		0xc800015cL hdmi_fir_y_coef1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 fir_y_coef2                        : 10;  //  9: 0     
} PE_H15A0_B_HDMI_FIR_Y_COEF1_T;

/*-----------------------------------------------------------------------------
		0xc8000160L hdmi_fir_c_coef0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 fir_y_coef0                        : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 fir_y_coef1                        : 10;  // 25:16     
} PE_H15A0_B_HDMI_FIR_C_COEF0_T;

/*-----------------------------------------------------------------------------
		0xc8000164L hdmi_fir_c_coef1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 fir_y_coef2                        : 10;  //  9: 0     
} PE_H15A0_B_HDMI_FIR_C_COEF1_T;

/*-----------------------------------------------------------------------------
		0xc8000168L hdmi_eo2lr_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 left_max                           : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 right_min                          : 13;  // 28:16     
} PE_H15A0_B_HDMI_EO2LR_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc800016cL hdmi_rx_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 color_format                       : 2 ;  //  1: 0     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 color_domain                       : 1 ;  //     4     
} PE_H15A0_B_HDMI_RX_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc8000180L hdmi_mux_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 src_sel_e                          : 2 ;  //  1: 0     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 src_sel_o                          : 2 ;  //  5: 4     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 pre_mux_sel                        : 2 ;  //  9: 8     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 pre_out_mux_l_sel                  : 2 ;  // 17:16     
	UINT32                                    : 2 ;  // 19:18     reserved
	UINT32 pre_out_mux_r_sel                  : 2 ;  // 21:20     
} PE_H15A0_C_HDMI_MUX_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc8000184L hdmi_420in_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 yin_delay                          : 2 ;  //  1: 0     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 cbin_delay                         : 2 ;  //  5: 4     
	UINT32 cin_delay                          : 2 ;  //  7: 6     
	UINT32 y_swap                             : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 c_swap                             : 1 ;  //    12     
} PE_H15A0_C_HDMI_420IN_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc8000188L hdmi_hsc_ctrl0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hsc_en                             : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 sw_reset                           : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 sampling_mode                      : 1 ;  //     8     
	UINT32                                    : 7 ;  // 15: 9     reserved
	UINT32 phase_off                          : 9 ;  // 24:16     
} PE_H15A0_C_HDMI_HSC_CTRL0_T;

/*-----------------------------------------------------------------------------
		0xc800018cL hdmi_hsc_ctrl1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 in_pic_width                       : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 out_pic_width                      : 13;  // 28:16     
} PE_H15A0_C_HDMI_HSC_CTRL1_T;

/*-----------------------------------------------------------------------------
		0xc8000190L hdmi_hsc_ctrl2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 numerator                          : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 denominator                        : 13;  // 28:16     
} PE_H15A0_C_HDMI_HSC_CTRL2_T;

/*-----------------------------------------------------------------------------
		0xc8000194L hdmi_fir_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 fir_y_en                           : 1 ;  //     0     
	UINT32 fir_c_en                           : 1 ;  //     1     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 sum_sample_en                      : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 dual_en                            : 1 ;  //     8     
	UINT32                                    : 7 ;  // 15: 9     reserved
	UINT32 bound_cnt                          : 13;  // 28:16     
} PE_H15A0_C_HDMI_FIR_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc8000198L hdmi_fir_y_coef0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 fir_y_coef0                        : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 fir_y_coef1                        : 10;  // 25:16     
} PE_H15A0_C_HDMI_FIR_Y_COEF0_T;

/*-----------------------------------------------------------------------------
		0xc800019cL hdmi_fir_y_coef1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 fir_y_coef2                        : 10;  //  9: 0     
} PE_H15A0_C_HDMI_FIR_Y_COEF1_T;

/*-----------------------------------------------------------------------------
		0xc80001a0L hdmi_fir_c_coef0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 fir_y_coef0                        : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 fir_y_coef1                        : 10;  // 25:16     
} PE_H15A0_C_HDMI_FIR_C_COEF0_T;

/*-----------------------------------------------------------------------------
		0xc80001a4L hdmi_fir_c_coef1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 fir_y_coef2                        : 10;  //  9: 0     
} PE_H15A0_C_HDMI_FIR_C_COEF1_T;

/*-----------------------------------------------------------------------------
		0xc80001a8L hdmi_eo2lr_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 left_max                           : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 right_min                          : 13;  // 28:16     
} PE_H15A0_C_HDMI_EO2LR_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc80001c0L hdmi_mux_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 src_sel_e                          : 2 ;  //  1: 0     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 src_sel_o                          : 2 ;  //  5: 4     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 pre_mux_sel                        : 2 ;  //  9: 8     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 pre_out_mux_l_sel                  : 2 ;  // 17:16     
	UINT32                                    : 2 ;  // 19:18     reserved
	UINT32 pre_out_mux_r_sel                  : 2 ;  // 21:20     
} PE_H15A0_D_HDMI_MUX_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc80001c4L hdmi_420in_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 yin_delay                          : 2 ;  //  1: 0     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 cbin_delay                         : 2 ;  //  5: 4     
	UINT32 cin_delay                          : 2 ;  //  7: 6     
	UINT32 y_swap                             : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 c_swap                             : 1 ;  //    12     
} PE_H15A0_D_HDMI_420IN_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc80001c8L hdmi_hsc_ctrl0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hsc_en                             : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 sw_reset                           : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 sampling_mode                      : 1 ;  //     8     
	UINT32                                    : 7 ;  // 15: 9     reserved
	UINT32 phase_off                          : 9 ;  // 24:16     
} PE_H15A0_D_HDMI_HSC_CTRL0_T;

/*-----------------------------------------------------------------------------
		0xc80001ccL hdmi_hsc_ctrl1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 in_pic_width                       : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 out_pic_width                      : 13;  // 28:16     
} PE_H15A0_D_HDMI_HSC_CTRL1_T;

/*-----------------------------------------------------------------------------
		0xc80001d0L hdmi_hsc_ctrl2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 numerator                          : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 denominator                        : 13;  // 28:16     
} PE_H15A0_D_HDMI_HSC_CTRL2_T;

/*-----------------------------------------------------------------------------
		0xc80001d4L hdmi_fir_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 fir_y_en                           : 1 ;  //     0     
	UINT32 fir_c_en                           : 1 ;  //     1     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 sum_sample_en                      : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 dual_en                            : 1 ;  //     8     
	UINT32                                    : 7 ;  // 15: 9     reserved
	UINT32 bound_cnt                          : 13;  // 28:16     
} PE_H15A0_D_HDMI_FIR_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc80001d8L hdmi_fir_y_coef0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 fir_y_coef0                        : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 fir_y_coef1                        : 10;  // 25:16     
} PE_H15A0_D_HDMI_FIR_Y_COEF0_T;

/*-----------------------------------------------------------------------------
		0xc80001dcL hdmi_fir_y_coef1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 fir_y_coef2                        : 10;  //  9: 0     
} PE_H15A0_D_HDMI_FIR_Y_COEF1_T;

/*-----------------------------------------------------------------------------
		0xc80001e0L hdmi_fir_c_coef0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 fir_y_coef0                        : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 fir_y_coef1                        : 10;  // 25:16     
} PE_H15A0_D_HDMI_FIR_C_COEF0_T;

/*-----------------------------------------------------------------------------
		0xc80001e4L hdmi_fir_c_coef1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 fir_y_coef2                        : 10;  //  9: 0     
} PE_H15A0_D_HDMI_FIR_C_COEF1_T;

/*-----------------------------------------------------------------------------
		0xc80001e8L hdmi_eo2lr_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 left_max                           : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 right_min                          : 13;  // 28:16     
} PE_H15A0_D_HDMI_EO2LR_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc8000200L top_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 scan_type                          : 1 ;  //     0     
	UINT32 field_gen_mode                     : 2 ;  //  2: 1     
	UINT32 auto_field_gen                     : 2 ;  //  4: 3     
	UINT32 data_bit_mode                      : 1 ;  //     5     
	UINT32 chroma_sampling                    : 1 ;  //     6     
	UINT32 aspect_ratio                       : 1 ;  //     7     
	UINT32 yc_delay_mode_r                    : 2 ;  //  9: 8     
	UINT32 yc_delay_mode_b                    : 2 ;  // 11:10     
	UINT32 yc_delay_mode_g                    : 2 ;  // 13:12     
	UINT32 data_in_sel                        : 4 ;  // 17:14     
	UINT32                                    : 1 ;  //    18     reserved
	UINT32 auto_sync_polarity                 : 1 ;  //    19     
	UINT32 manual_sync_polarity               : 2 ;  // 21:20     
	UINT32 adc_cut_direction                  : 1 ;  //    22     
	UINT32 adc_bit_sel                        : 1 ;  //    23     
	UINT32 br_port_swap                       : 1 ;  //    24     
	UINT32 r_port_swap                        : 1 ;  //    25     
	UINT32 b_port_swap                        : 1 ;  //    26     
	UINT32 g_port_swap                        : 1 ;  //    27     
	UINT32 bit_numbering_lsb2                 : 3 ;  // 30:28     
	UINT32 data_en_mode                       : 1 ;  //    31     
} PE_H15A0_CVI_TOP_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc8000204L offset_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 offset_h                           : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 offset_v                           : 13;  // 28:16     
} PE_H15A0_CVI_OFFSET_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc8000208L size_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 size_h                             : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 size_v                             : 13;  // 28:16     
} PE_H15A0_CVI_SIZE_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc800020cL int_sync_ctrl0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 res_hsync                          : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 res_hfp                            : 10;  // 25:16     
} PE_H15A0_CVI_INT_SYNC_CTRL0_T;

/*-----------------------------------------------------------------------------
		0xc8000210L int_sync_ctrl1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 res_vfp                            : 5 ;  //  4: 0     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 res_vsync                          : 5 ;  // 12: 8     
	UINT32 res_field_add_line                 : 2 ;  // 14:13     
	UINT32 res_field_sel                      : 1 ;  //    15     
	UINT32 res_sync_sel                       : 1 ;  //    16     
} PE_H15A0_CVI_INT_SYNC_CTRL1_T;

/*-----------------------------------------------------------------------------
		0xc8000224L csc_ctrl0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 csc_coefficient1                   : 15;  // 14: 0     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 csc_coefficient0                   : 15;  // 30:16     
} PE_H15A0_CVI_CSC_CTRL0_T;

/*-----------------------------------------------------------------------------
		0xc8000228L csc_ctrl1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 csc_coefficient3                   : 15;  // 14: 0     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 csc_coefficient2                   : 15;  // 30:16     
} PE_H15A0_CVI_CSC_CTRL1_T;

/*-----------------------------------------------------------------------------
		0xc800022cL csc_ctrl2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 csc_coefficient5                   : 15;  // 14: 0     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 csc_coefficient4                   : 15;  // 30:16     
} PE_H15A0_CVI_CSC_CTRL2_T;

/*-----------------------------------------------------------------------------
		0xc8000230L csc_ctrl3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 csc_coefficient7                   : 15;  // 14: 0     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 csc_coefficient6                   : 15;  // 30:16     
} PE_H15A0_CVI_CSC_CTRL3_T;

/*-----------------------------------------------------------------------------
		0xc8000234L csc_ctrl4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 csc_offset0                        : 11;  // 10: 0     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 csc_coefficient8                   : 15;  // 30:16     
} PE_H15A0_CVI_CSC_CTRL4_T;

/*-----------------------------------------------------------------------------
		0xc8000238L csc_ctrl5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 csc_offset2                        : 11;  // 10: 0     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 csc_offset1                        : 11;  // 26:16     
} PE_H15A0_CVI_CSC_CTRL5_T;

/*-----------------------------------------------------------------------------
		0xc800023cL csc_ctrl6 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 csc_offset4                        : 11;  // 10: 0     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 csc_offset3                        : 11;  // 26:16     
} PE_H15A0_CVI_CSC_CTRL6_T;

/*-----------------------------------------------------------------------------
		0xc8000240L csc_ctrl7 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 csc_coeff_load                     : 1 ;  //     0     
	UINT32 csc_en                             : 1 ;  //     1     
	UINT32 csc_msb_4b_only_mode               : 1 ;  //     2     
	UINT32                                    : 13;  // 15: 3     reserved
	UINT32 csc_offset5                        : 11;  // 26:16     
} PE_H15A0_CVI_CSC_CTRL7_T;

/*-----------------------------------------------------------------------------
		0xc8000244L misc_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pattern_type                       : 3 ;  //  2: 0     
	UINT32 pattern_csc                        : 1 ;  //     3     
	UINT32 pattern_detail                     : 4 ;  //  7: 4     
	UINT32 reg_update_pulse                   : 1 ;  //     8     
	UINT32 field_fix_mode                     : 2 ;  // 10: 9     
	UINT32 black_sel                          : 1 ;  //    11     
	UINT32 write_inhibit                      : 1 ;  //    12     
	UINT32 sim_mode                           : 1 ;  //    13     
	UINT32 black_off                          : 1 ;  //    14     
	UINT32 field_edge                         : 1 ;  //    15     
	UINT32 ha_gen                             : 1 ;  //    16     
	UINT32 xsize_divide                       : 2 ;  // 18:17     
	UINT32                                    : 1 ;  //    19     reserved
	UINT32 yc_delay_mode_r                    : 2 ;  // 21:20     
	UINT32 yc_delay_mode_b                    : 2 ;  // 23:22     
	UINT32 yc_delay_mode_g                    : 2 ;  // 25:24     
	UINT32 field_detect_vris                  : 1 ;  //    26     
	UINT32                                    : 1 ;  //    27     reserved
	UINT32 reg_update_position                : 4 ;  // 31:28     
} PE_H15A0_CVI_MISC_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc8000248L offset_read ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hoffset_read                       : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 voffset_read                       : 13;  // 28:16     
} PE_H15A0_CVI_OFFSET_READ_T;

/*-----------------------------------------------------------------------------
		0xc800024cL pre3d_ctrl0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pre3d_sync_enable                  : 1 ;  //     0     
	UINT32 pre3d_sync_mode                    : 2 ;  //  2: 1     
	UINT32 pre3d_field_mode                   : 1 ;  //     3     
	UINT32 pre3d_frame_lr_mode                : 1 ;  //     4     
	UINT32 pre3d_mid_vsync_bypass             : 1 ;  //     5     
	UINT32 pre3d_field_bypass                 : 1 ;  //     6     
	UINT32 reserved                           : 1 ;  //     7     
	UINT32                                    : 8 ;  // 15: 8     reserved
	UINT32 pre3d_active_line_num              : 13;  // 28:16     
} PE_H15A0_CVI_PRE3D_CTRL0_T;

/*-----------------------------------------------------------------------------
		0xc8000250L pre3d_ctrl1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pre3d_active_space1_line_num       : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 pre3d_active_space2_line_num       : 13;  // 28:16     
} PE_H15A0_CVI_PRE3D_CTRL1_T;

/*-----------------------------------------------------------------------------
		0xc8000254L pre3d_ctrl2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pre3d_front_porch_line_num         : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 pre3d_sync_line_num                : 13;  // 28:16     
} PE_H15A0_CVI_PRE3D_CTRL2_T;

/*-----------------------------------------------------------------------------
		0xc8000258L size_status1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sd_hoffset_indurance_read          : 8 ;  //  7: 0     
	UINT32 sd_voffset_indurance_read          : 8 ;  // 15: 8     
	UINT32 sd_hsync_fault_flag_read           : 1 ;  //    16     
	UINT32 sd_vsync_fault_flag_read           : 1 ;  //    17     
	UINT32 h_sync_polarity                    : 1 ;  //    18     
	UINT32 v_sync_polarity                    : 1 ;  //    19     
	UINT32 field_signal                       : 4 ;  // 23:20     
	UINT32 hsync_at_vsync_fall                : 4 ;  // 27:24     
	UINT32 outfield_at_vactive_fall           : 2 ;  // 29:28     
	UINT32 hoffset_over                       : 1 ;  //    30     
	UINT32 v_offset_over                      : 1 ;  //    31     
} PE_H15A0_CVI_SIZE_STATUS1_T;

/*-----------------------------------------------------------------------------
		0xc800025cL size_status2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 de_line_size                       : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 de_line_count                      : 12;  // 27:16     
} PE_H15A0_CVI_SIZE_STATUS2_T;

/*-----------------------------------------------------------------------------
		0xc8000260L size_status3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hd_sync_size                       : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 hd_line_size                       : 12;  // 27:16     
} PE_H15A0_CVI_SIZE_STATUS3_T;

/*-----------------------------------------------------------------------------
		0xc8000264L size_status4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vd_sync_count                      : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 vd_line_count                      : 12;  // 27:16     
} PE_H15A0_CVI_SIZE_STATUS4_T;

/*-----------------------------------------------------------------------------
		0xc8000268L pix_sum_ctrl0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 start_pos_x                        : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 end_pos_x                          : 12;  // 27:16     
	UINT32 pix_sum_enable                     : 1 ;  //    28     
	UINT32 field_selection                    : 2 ;  // 30:29     
	UINT32 debug_enable                       : 1 ;  //    31     
} PE_H15A0_CVI_PIX_SUM_CTRL0_T;

/*-----------------------------------------------------------------------------
		0xc800026cL pix_sum_ctrl1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 start_pos_y                        : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 end_pos_y                          : 12;  // 27:16     
} PE_H15A0_CVI_PIX_SUM_CTRL1_T;

/*-----------------------------------------------------------------------------
		0xc8000270L pix_sum_status0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pix_sum_g                          : 32;  // 31: 0     
} PE_H15A0_CVI_PIX_SUM_STATUS0_T;

/*-----------------------------------------------------------------------------
		0xc8000274L pix_sum_status1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pix_sum_b                          : 32;  // 31: 0     
} PE_H15A0_CVI_PIX_SUM_STATUS1_T;

/*-----------------------------------------------------------------------------
		0xc8000278L pix_sum_status2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pix_sum_r                          : 32;  // 31: 0     
} PE_H15A0_CVI_PIX_SUM_STATUS2_T;

/*-----------------------------------------------------------------------------
		0xc800027cL sync_fil_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hsync_fil_en                       : 1 ;  //     0     
	UINT32 hsync_counter_en                   : 1 ;  //     1     
	UINT32 vsync_fil_en                       : 1 ;  //     2     
	UINT32 vsync_counter_en                   : 1 ;  //     3     
	UINT32 de_fil_en                          : 1 ;  //     4     
	UINT32 de_counter_en                      : 1 ;  //     5     
	UINT32 field_fil_en                       : 1 ;  //     6     
	UINT32 field_counter_en                   : 1 ;  //     7     
	UINT32                                    : 8 ;  // 15: 8     reserved
	UINT32 line_count_sel                     : 13;  // 28:16     
} PE_H15A0_CVI_SYNC_FIL_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc8000280L sync_fil_status ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hsync_coutner                      : 8 ;  //  7: 0     
	UINT32 vsync_counter                      : 8 ;  // 15: 8     
	UINT32 de_counter                         : 8 ;  // 23:16     
	UINT32 field_counter                      : 8 ;  // 31:24     
} PE_H15A0_CVI_SYNC_FIL_STATUS_T;

/*-----------------------------------------------------------------------------
		0xc8000290L sync_repeat_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 repeat_y_en                        : 1 ;  //     0     
	UINT32 repeat_cb_en                       : 1 ;  //     1     
	UINT32 repeat_cr_en                       : 1 ;  //     2     
	UINT32 repeat_pixel_odd                   : 1 ;  //     3     
	UINT32                                    : 4 ;  //  7: 4     reserved
	UINT32 repeat_threshold_y                 : 6 ;  // 13: 8     
	UINT32 avg_threshold_y                    : 6 ;  // 19:14     
	UINT32 repeat_threshold_c                 : 6 ;  // 25:20     
	UINT32 avg_threshold_c                    : 6 ;  // 31:26     
} PE_H15A0_CVI_SYNC_REPEAT_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc8000294L sync_repeat_size0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 repeat_pixel_y                     : 21;  // 20: 0     
} PE_H15A0_CVI_SYNC_REPEAT_SIZE0_T;

/*-----------------------------------------------------------------------------
		0xc8000298L sync_repeat_size1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 avg_pixel_y                        : 21;  // 20: 0     
} PE_H15A0_CVI_SYNC_REPEAT_SIZE1_T;

/*-----------------------------------------------------------------------------
		0xc800029cL sync_repeat_size2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 repeat_pixel_cb                    : 21;  // 20: 0     
} PE_H15A0_CVI_SYNC_REPEAT_SIZE2_T;

/*-----------------------------------------------------------------------------
		0xc80002a0L sync_repeat_size3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 avg_pixel_cb                       : 21;  // 20: 0     
} PE_H15A0_CVI_SYNC_REPEAT_SIZE3_T;

/*-----------------------------------------------------------------------------
		0xc80002a4L sync_repeat_size4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 repeat_pixel_cr                    : 21;  // 20: 0     
} PE_H15A0_CVI_SYNC_REPEAT_SIZE4_T;

/*-----------------------------------------------------------------------------
		0xc80002a8L sync_repeat_size5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 avg_pixel_cb                       : 21;  // 20: 0     
} PE_H15A0_CVI_SYNC_REPEAT_SIZE5_T;

/*-----------------------------------------------------------------------------
		0xc80002c0L size_detect_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sd_vd_range                        : 2 ;  //  1: 0     
	UINT32 sd_ext_range                       : 2 ;  //  3: 2     
	UINT32 intr_mask_node                     : 1 ;  //     4     
	UINT32 intr_mask_nohsync                  : 1 ;  //     5     
	UINT32 intrl_mask_novsync                 : 1 ;  //     6     
	UINT32 intr_mask_abnormal                 : 1 ;  //     7     
	UINT32 intr_mask_invalid                  : 1 ;  //     8     
	UINT32 intr_mask_de                       : 1 ;  //     9     
	UINT32 intr_mask_hsync                    : 1 ;  //    10     
	UINT32 intr_mask_vsync                    : 1 ;  //    11     
	UINT32 intr_mask_normal                   : 1 ;  //    12     
	UINT32 intr_mask_valid                    : 1 ;  //    13     
	UINT32 intr_mask_h_cnahge                 : 1 ;  //    14     
	UINT32 sd_en                              : 1 ;  //    15     
	UINT32 sd_frame_cnt                       : 3 ;  // 18:16     
	UINT32                                    : 1 ;  //    19     reserved
	UINT32 sd_novald_duration                 : 2 ;  // 21:20     
	UINT32                                    : 1 ;  //    22     reserved
	UINT32 sd_one_field                       : 1 ;  //    23     
	UINT32                                    : 2 ;  // 25:24     reserved
	UINT32 nosig_duration_de                  : 2 ;  // 27:26     
	UINT32 nosig_duration_hsync               : 2 ;  // 29:28     
	UINT32 nosig_duration_vsync               : 2 ;  // 31:30     
} PE_H15A0_CVI_SIZE_DETECT_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc80002c4L size_detect_read ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 line_num_read                      : 16;  // 15: 0     
	UINT32 line_width_read                    : 16;  // 31:16     
} PE_H15A0_CVI_SIZE_DETECT_READ_T;

/*-----------------------------------------------------------------------------
		0xc80002c8L size_status0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 current_line_number_read           : 12;  // 11: 0     
	UINT32 de_signal_detect                   : 1 ;  //    12     
	UINT32 hsync_signal_detect                : 1 ;  //    13     
	UINT32 vsync_signal_detect                : 1 ;  //    14     
	UINT32 source_valid                       : 1 ;  //    15     
	UINT32 sd_line_num                        : 12;  // 27:16     
	UINT32 field_at_vactive_fall              : 4 ;  // 31:28     
} PE_H15A0_CVI_SIZE_STATUS0_T;

/*-----------------------------------------------------------------------------
		0xc80002ccL size_status5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sd_2field_size                     : 24;  // 23: 0     
} PE_H15A0_CVI_SIZE_STATUS5_T;

/*-----------------------------------------------------------------------------
		0xc80002d0L size_status6 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 diff_vd_vcnt                       : 16;  // 15: 0     
	UINT32 diff_ext_vcnt                      : 12;  // 27:16     
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 vd_intr                            : 1 ;  //    31     
} PE_H15A0_CVI_SIZE_STATUS6_T;

/*-----------------------------------------------------------------------------
		0xc80002d4L size_detect_ctrl1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 abnormal0_en                       : 1 ;  //     0     
	UINT32 abnormal1_en                       : 1 ;  //     1     
	UINT32 abnormal2_en                       : 1 ;  //     2     
	UINT32 abnormal3_en                       : 1 ;  //     3     
	UINT32 abnormal4_en                       : 1 ;  //     4     
	UINT32 abnormal5_en                       : 1 ;  //     5     
	UINT32 abnormal6_en                       : 1 ;  //     6     
	UINT32 abnormal7_en                       : 1 ;  //     7     
	UINT32 abnormal0_flag                     : 1 ;  //     8     
	UINT32 abnormal1_flag                     : 1 ;  //     9     
	UINT32 abnormal2_flag                     : 1 ;  //    10     
	UINT32 abnormal3_flag                     : 1 ;  //    11     
	UINT32 abnormal4_flag                     : 1 ;  //    12     
	UINT32 abnormal5_flag                     : 1 ;  //    13     
	UINT32 abnormal6_flag                     : 1 ;  //    14     
	UINT32 abnormal7_flag                     : 1 ;  //    15     
	UINT32 ext_clk_valid_count                : 15;  // 30:16     
	UINT32 ext_clk_valid                      : 1 ;  //    31     
} PE_H15A0_CVI_SIZE_DETECT_CTRL1_T;

/*-----------------------------------------------------------------------------
		0xc80002d8L size_detect_ctrl2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 abnormal_in_h                      : 8 ;  //  7: 0     
	UINT32 abnormal_out_h                     : 8 ;  // 15: 8     
	UINT32 abnormal_in_v                      : 8 ;  // 23:16     
	UINT32 abnormal_out_v                     : 8 ;  // 31:24     
} PE_H15A0_CVI_SIZE_DETECT_CTRL2_T;

/*-----------------------------------------------------------------------------
		0xc80002dcL pseudo_sync_ctrl0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vsync_pesudo_sel                   : 1 ;  //     0     
	UINT32 hsync_pesudo_sel                   : 1 ;  //     1     
	UINT32 vactive_pesudo_sel                 : 1 ;  //     2     
	UINT32 hactive_pesudo_sel                 : 1 ;  //     3     
	UINT32 field_pesudo_sel                   : 1 ;  //     4     
	UINT32 y_pseudo_sel                       : 1 ;  //     5     
	UINT32 cb_psuedo_sel                      : 1 ;  //     6     
	UINT32 cr_psuedo_sel                      : 1 ;  //     7     
	UINT32 valid_pseudo_sel                   : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 auto_pseudo_sync_en                : 1 ;  //    12     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 pseudo_sync_ctrl_en                : 1 ;  //    16     
	UINT32 pseudo_y_data_sel                  : 1 ;  //    17     
	UINT32 pseudo_cb_data_sel                 : 1 ;  //    18     
	UINT32 pseudo_cr_data_sel                 : 1 ;  //    19     
	UINT32 pseudo_y                           : 4 ;  // 23:20     
	UINT32 pseudo_cb                          : 4 ;  // 27:24     
	UINT32 pseudo_cr                          : 4 ;  // 31:28     
} PE_H15A0_CVI_PSEUDO_SYNC_CTRL0_T;

/*-----------------------------------------------------------------------------
		0xc80002e0L pseudo_sync_ctrl1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pseudo_hfp                         : 14;  // 13: 0     
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 pseudo_hsync                       : 14;  // 29:16     
} PE_H15A0_CVI_PSEUDO_SYNC_CTRL1_T;

/*-----------------------------------------------------------------------------
		0xc80002e4L pseudo_sync_ctrl2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pseudo_hblk                        : 14;  // 13: 0     
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 pseudo_hn                          : 14;  // 29:16     
} PE_H15A0_CVI_PSEUDO_SYNC_CTRL2_T;

/*-----------------------------------------------------------------------------
		0xc80002e8L pseudo_sync_ctrl3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pseudo_vfp                         : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 pseudo_vsync                       : 12;  // 27:16     
} PE_H15A0_CVI_PSEUDO_SYNC_CTRL3_T;

/*-----------------------------------------------------------------------------
		0xc80002ecL pseudo_sync_ctrl4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pseudo_vblk                        : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 pseudo_vn                          : 12;  // 27:16     
} PE_H15A0_CVI_PSEUDO_SYNC_CTRL4_T;

/*-----------------------------------------------------------------------------
		0xc80002f0L size_detect_ctrl3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 interrupt_mask_method              : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 interrupt_mask_count               : 12;  // 15: 4     
	UINT32 diff_hsize                         : 14;  // 29:16     
} PE_H15A0_CVI_SIZE_DETECT_CTRL3_T;

typedef struct {
	PE_H15A0_A_HDMI_MUX_CTRL_T         	hdmi_mux_ctrl;              //0xc8000100L
	PE_H15A0_A_HDMI_420IN_CTRL_T       	hdmi_420in_ctrl;            //0xc8000104L
	PE_H15A0_A_HDMI_HSC_CTRL0_T        	hdmi_hsc_ctrl0;             //0xc8000108L
	PE_H15A0_A_HDMI_HSC_CTRL1_T        	hdmi_hsc_ctrl1;             //0xc800010cL
	PE_H15A0_A_HDMI_HSC_CTRL2_T        	hdmi_hsc_ctrl2;             //0xc8000110L
	PE_H15A0_A_HDMI_FIR_CTRL_T         	hdmi_fir_ctrl;              //0xc8000114L
	PE_H15A0_A_HDMI_FIR_Y_COEF0_T      	hdmi_fir_y_coef0;           //0xc8000118L
	PE_H15A0_A_HDMI_FIR_Y_COEF1_T      	hdmi_fir_y_coef1;           //0xc800011cL
	PE_H15A0_A_HDMI_FIR_C_COEF0_T      	hdmi_fir_c_coef0;           //0xc8000120L
	PE_H15A0_A_HDMI_FIR_C_COEF1_T      	hdmi_fir_c_coef1;           //0xc8000124L
	PE_H15A0_A_HDMI_EO2LR_CTRL_T       	hdmi_eo2lr_ctrl;            //0xc8000128L
	PE_H15A0_A_HDMI_RX_CTRL_T          	hdmi_rx_ctrl;               //0xc800012cL
}PE_HDMI_A_REG_H15A0_T;

typedef struct {
	PE_H15A0_B_HDMI_MUX_CTRL_T         	hdmi_mux_ctrl;              //0xc8000140L
	PE_H15A0_B_HDMI_420IN_CTRL_T       	hdmi_420in_ctrl;            //0xc8000144L
	PE_H15A0_B_HDMI_HSC_CTRL0_T        	hdmi_hsc_ctrl0;             //0xc8000148L
	PE_H15A0_B_HDMI_HSC_CTRL1_T        	hdmi_hsc_ctrl1;             //0xc800014cL
	PE_H15A0_B_HDMI_HSC_CTRL2_T        	hdmi_hsc_ctrl2;             //0xc8000150L
	PE_H15A0_B_HDMI_FIR_CTRL_T         	hdmi_fir_ctrl;              //0xc8000154L
	PE_H15A0_B_HDMI_FIR_Y_COEF0_T      	hdmi_fir_y_coef0;           //0xc8000158L
	PE_H15A0_B_HDMI_FIR_Y_COEF1_T      	hdmi_fir_y_coef1;           //0xc800015cL
	PE_H15A0_B_HDMI_FIR_C_COEF0_T      	hdmi_fir_c_coef0;           //0xc8000160L
	PE_H15A0_B_HDMI_FIR_C_COEF1_T      	hdmi_fir_c_coef1;           //0xc8000164L
	PE_H15A0_B_HDMI_EO2LR_CTRL_T       	hdmi_eo2lr_ctrl;            //0xc8000168L
	PE_H15A0_B_HDMI_RX_CTRL_T          	hdmi_rx_ctrl;               //0xc800016cL
}PE_HDMI_B_REG_H15A0_T;

typedef struct {
	PE_H15A0_C_HDMI_MUX_CTRL_T         	hdmi_mux_ctrl;              //0xc8000180L
	PE_H15A0_C_HDMI_420IN_CTRL_T       	hdmi_420in_ctrl;            //0xc8000184L
	PE_H15A0_C_HDMI_HSC_CTRL0_T        	hdmi_hsc_ctrl0;             //0xc8000188L
	PE_H15A0_C_HDMI_HSC_CTRL1_T        	hdmi_hsc_ctrl1;             //0xc800018cL
	PE_H15A0_C_HDMI_HSC_CTRL2_T        	hdmi_hsc_ctrl2;             //0xc8000190L
	PE_H15A0_C_HDMI_FIR_CTRL_T         	hdmi_fir_ctrl;              //0xc8000194L
	PE_H15A0_C_HDMI_FIR_Y_COEF0_T      	hdmi_fir_y_coef0;           //0xc8000198L
	PE_H15A0_C_HDMI_FIR_Y_COEF1_T      	hdmi_fir_y_coef1;           //0xc800019cL
	PE_H15A0_C_HDMI_FIR_C_COEF0_T      	hdmi_fir_c_coef0;           //0xc80001a0L
	PE_H15A0_C_HDMI_FIR_C_COEF1_T      	hdmi_fir_c_coef1;           //0xc80001a4L
	PE_H15A0_C_HDMI_EO2LR_CTRL_T       	hdmi_eo2lr_ctrl;            //0xc80001a8L
}PE_HDMI_C_REG_H15A0_T;

typedef struct {
	PE_H15A0_D_HDMI_MUX_CTRL_T         	hdmi_mux_ctrl;              //0xc80001c0L
	PE_H15A0_D_HDMI_420IN_CTRL_T       	hdmi_420in_ctrl;            //0xc80001c4L
	PE_H15A0_D_HDMI_HSC_CTRL0_T        	hdmi_hsc_ctrl0;             //0xc80001c8L
	PE_H15A0_D_HDMI_HSC_CTRL1_T        	hdmi_hsc_ctrl1;             //0xc80001ccL
	PE_H15A0_D_HDMI_HSC_CTRL2_T        	hdmi_hsc_ctrl2;             //0xc80001d0L
	PE_H15A0_D_HDMI_FIR_CTRL_T         	hdmi_fir_ctrl;              //0xc80001d4L
	PE_H15A0_D_HDMI_FIR_Y_COEF0_T      	hdmi_fir_y_coef0;           //0xc80001d8L
	PE_H15A0_D_HDMI_FIR_Y_COEF1_T      	hdmi_fir_y_coef1;           //0xc80001dcL
	PE_H15A0_D_HDMI_FIR_C_COEF0_T      	hdmi_fir_c_coef0;           //0xc80001e0L
	PE_H15A0_D_HDMI_FIR_C_COEF1_T      	hdmi_fir_c_coef1;           //0xc80001e4L
	PE_H15A0_D_HDMI_EO2LR_CTRL_T       	hdmi_eo2lr_ctrl;            //0xc80001e8L
}PE_HDMI_D_REG_H15A0_T;

typedef struct {
	PE_H15A0_CVI_TOP_CTRL_T             top_ctrl;                     //0xc8000200L
	PE_H15A0_CVI_OFFSET_CTRL_T          offset_ctrl;                  //0xc8000204L
	PE_H15A0_CVI_SIZE_CTRL_T            size_ctrl;                    //0xc8000208L
	PE_H15A0_CVI_INT_SYNC_CTRL0_T       int_sync_ctrl0;               //0xc800020cL
	PE_H15A0_CVI_INT_SYNC_CTRL1_T       int_sync_ctrl1;               //0xc8000210L
	UINT32                             	reserved00[4];                //0xc8000214L~0xc8000220L
	PE_H15A0_CVI_CSC_CTRL0_T            csc_ctrl0;                    //0xc8000224L
	PE_H15A0_CVI_CSC_CTRL1_T            csc_ctrl1;                    //0xc8000228L
	PE_H15A0_CVI_CSC_CTRL2_T            csc_ctrl2;                    //0xc800022cL
	PE_H15A0_CVI_CSC_CTRL3_T            csc_ctrl3;                    //0xc8000230L
	PE_H15A0_CVI_CSC_CTRL4_T            csc_ctrl4;                    //0xc8000234L
	PE_H15A0_CVI_CSC_CTRL5_T            csc_ctrl5;                    //0xc8000238L
	PE_H15A0_CVI_CSC_CTRL6_T            csc_ctrl6;                    //0xc800023cL
	PE_H15A0_CVI_CSC_CTRL7_T            csc_ctrl7;                    //0xc8000240L
	PE_H15A0_CVI_MISC_CTRL_T            misc_ctrl;                    //0xc8000244L
	PE_H15A0_CVI_OFFSET_READ_T          offset_read;                  //0xc8000248L
	PE_H15A0_CVI_PRE3D_CTRL0_T          pre3d_ctrl0;                  //0xc800024cL
	PE_H15A0_CVI_PRE3D_CTRL1_T          pre3d_ctrl1;                  //0xc8000250L
	PE_H15A0_CVI_PRE3D_CTRL2_T          pre3d_ctrl2;                  //0xc8000254L
	PE_H15A0_CVI_SIZE_STATUS1_T         size_status1;                 //0xc8000258L
	PE_H15A0_CVI_SIZE_STATUS2_T         size_status2;                 //0xc800025cL
	PE_H15A0_CVI_SIZE_STATUS3_T         size_status3;                 //0xc8000260L
	PE_H15A0_CVI_SIZE_STATUS4_T         size_status4;                 //0xc8000264L
	PE_H15A0_CVI_PIX_SUM_CTRL0_T        pix_sum_ctrl0;                //0xc8000268L
	PE_H15A0_CVI_PIX_SUM_CTRL1_T        pix_sum_ctrl1;                //0xc800026cL
	PE_H15A0_CVI_PIX_SUM_STATUS0_T      pix_sum_status0;              //0xc8000270L
	PE_H15A0_CVI_PIX_SUM_STATUS1_T      pix_sum_status1;              //0xc8000274L
	PE_H15A0_CVI_PIX_SUM_STATUS2_T      pix_sum_status2;              //0xc8000278L
	PE_H15A0_CVI_SYNC_FIL_CTRL_T        sync_fil_ctrl;                //0xc800027cL
	PE_H15A0_CVI_SYNC_FIL_STATUS_T      sync_fil_status;              //0xc8000280L
	UINT32                             	reserved01[3];                //0xc8000284L~0xc800028cL
	PE_H15A0_CVI_SYNC_REPEAT_CTRL_T     sync_repeat_ctrl;             //0xc8000290L
	PE_H15A0_CVI_SYNC_REPEAT_SIZE0_T    sync_repeat_size0;            //0xc8000294L
	PE_H15A0_CVI_SYNC_REPEAT_SIZE1_T    sync_repeat_size1;            //0xc8000298L
	PE_H15A0_CVI_SYNC_REPEAT_SIZE2_T    sync_repeat_size2;            //0xc800029cL
	PE_H15A0_CVI_SYNC_REPEAT_SIZE3_T    sync_repeat_size3;            //0xc80002a0L
	PE_H15A0_CVI_SYNC_REPEAT_SIZE4_T    sync_repeat_size4;            //0xc80002a4L
	PE_H15A0_CVI_SYNC_REPEAT_SIZE5_T    sync_repeat_size5;            //0xc80002a8L
	UINT32                             	reserved02[5];                //0xc80002acL~0xc80002bcL
	PE_H15A0_CVI_SIZE_DETECT_CTRL_T     size_detect_ctrl;             //0xc80002c0L
	PE_H15A0_CVI_SIZE_DETECT_READ_T     size_detect_read;             //0xc80002c4L
	PE_H15A0_CVI_SIZE_STATUS0_T         size_status0;                 //0xc80002c8L
	PE_H15A0_CVI_SIZE_STATUS5_T         size_status5;                 //0xc80002ccL
	PE_H15A0_CVI_SIZE_STATUS6_T         size_status6;                 //0xc80002d0L
	PE_H15A0_CVI_SIZE_DETECT_CTRL1_T    size_detect_ctrl1;            //0xc80002d4L
	PE_H15A0_CVI_SIZE_DETECT_CTRL2_T    size_detect_ctrl2;            //0xc80002d8L
	PE_H15A0_CVI_PSEUDO_SYNC_CTRL0_T    pseudo_sync_ctrl0;            //0xc80002dcL
	PE_H15A0_CVI_PSEUDO_SYNC_CTRL1_T    pseudo_sync_ctrl1;            //0xc80002e0L
	PE_H15A0_CVI_PSEUDO_SYNC_CTRL2_T    pseudo_sync_ctrl2;            //0xc80002e4L
	PE_H15A0_CVI_PSEUDO_SYNC_CTRL3_T    pseudo_sync_ctrl3;            //0xc80002e8L
	PE_H15A0_CVI_PSEUDO_SYNC_CTRL4_T    pseudo_sync_ctrl4;            //0xc80002ecL
	PE_H15A0_CVI_SIZE_DETECT_CTRL3_T    size_detect_ctrl3;            //0xc80002f0L
}PE_CVI_REG_H15A0_T;

#endif
