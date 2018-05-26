#ifndef _DE_SMUX_M1_H_
#define _DE_SMUX_M1_H_
/*-----------------------------------------------------------------------------
		0xc8001400L smux_ctrl0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 o_c_fmt                            : 3 ;  //  2: 0     
	UINT32 prew_rd_422                        : 1 ;  //     3     
	UINT32 i_c_fmt                            : 3 ;  //  6: 4     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 hdmi420_on                         : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 prew_mux_sel                       : 1 ;  //    12     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 cvi_mux_sel                        : 3 ;  // 18:16     
	UINT32                                    : 5 ;  // 23:19     reserved
	UINT32 wr_disalbe                         : 1 ;  //    24     
	UINT32                                    : 4 ;  // 28:25     reserved
	UINT32 set_mvi_val                        : 1 ;  //    29     
	UINT32                                    : 1 ;  //    30     reserved
	UINT32 pr_sq                              : 1 ;  //    31     
} SMUX_CTRL0_T;

/*-----------------------------------------------------------------------------
		0xc8001404L smux_ctrl1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mifr_mode                          : 1 ;  //     0     
	UINT32                                    : 7 ;  //  7: 1     reserved
	UINT32 hsc_enable                         : 1 ;  //     8     
	UINT32 vsc_enable                         : 1 ;  //     9     
	UINT32                                    : 2 ;  // 11:10     reserved
	UINT32 gos_enable                         : 1 ;  //    12     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 line_diff_en                       : 1 ;  //    16     
	UINT32                                    : 3 ;  // 19:17     reserved
	UINT32 line_diff                          : 12;  // 31:20     
} SMUX_CTRL1_T;

/*-----------------------------------------------------------------------------
		0xc8001408L smux_ctrl2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_req_pend_val                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 c_req_pend_val                     : 12;  // 27:16     
} SMUX_CTRL2_T;

/*-----------------------------------------------------------------------------
		0xc800140cL smux_ctrl3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 lr_switch_on                       : 1 ;  //     0     
	UINT32 lr_switch_mode                     : 1 ;  //     1     
	UINT32 lr_switch_auto                     : 1 ;  //     2     
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 fifo_split                         : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 lr_write_sel                       : 1 ;  //     8     
	UINT32 lr_write_mode                      : 1 ;  //     9     
	UINT32 lr_write_auto                      : 1 ;  //    10     
	UINT32                                    : 1 ;  //    11     reserved
	UINT32 dec3d_mux_sel                      : 1 ;  //    12     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 auto_3d_mode                       : 3 ;  // 18:16     
	UINT32                                    : 1 ;  //    19     reserved
	UINT32 smux_front_buffer_detour           : 1 ;  //    20     
} SMUX_CTRL3_T;

/*-----------------------------------------------------------------------------
		0xc8001410L smux_ctrlr ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_o_c_fmt                          : 3 ;  //  2: 0     
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 r_i_c_fmt                          : 3 ;  //  6: 4     
	UINT32                                    : 24;  // 30: 7     reserved
	UINT32 r_pr_sq                            : 1 ;  //    31     
} SMUX_CTRLR_T;

/*-----------------------------------------------------------------------------
		0xc8001414L smux_hsc_y_set_val_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 phase_offset_y                     : 6 ;  //  5: 0     
	UINT32                                    : 6 ;  // 11: 6     reserved
	UINT32 aal_coef_0_y                       : 10;  // 21:12     
} SMUX_HSC_Y_SET_VAL_0_T;

/*-----------------------------------------------------------------------------
		0xc8001418L smux_hsc_y_set_val_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 aal_coef_1_y                       : 10;  //  9: 0     
	UINT32                                    : 2 ;  // 11:10     reserved
	UINT32 aal_coef_2_y                       : 10;  // 21:12     
} SMUX_HSC_Y_SET_VAL_1_T;

/*-----------------------------------------------------------------------------
		0xc800141cL smux_hsc_y_set_val_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 aal_coef_3_y                       : 10;  //  9: 0     
	UINT32                                    : 2 ;  // 11:10     reserved
	UINT32 aal_coef_4_y                       : 10;  // 21:12     
} SMUX_HSC_Y_SET_VAL_2_T;

/*-----------------------------------------------------------------------------
		0xc8001420L smux_hsc_y_set_val_3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 aal_coef_5_y                       : 10;  //  9: 0     
	UINT32                                    : 2 ;  // 11:10     reserved
	UINT32 aal_coef_6_y                       : 10;  // 21:12     
} SMUX_HSC_Y_SET_VAL_3_T;

/*-----------------------------------------------------------------------------
		0xc8001424L smux_hsc_c_set_val_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 phase_offset_c                     : 6 ;  //  5: 0     
	UINT32                                    : 6 ;  // 11: 6     reserved
	UINT32 aal_coef_0_c                       : 10;  // 21:12     
} SMUX_HSC_C_SET_VAL_0_T;

/*-----------------------------------------------------------------------------
		0xc8001428L smux_hsc_c_set_val_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 aal_coef_1_c                       : 10;  //  9: 0     
	UINT32                                    : 2 ;  // 11:10     reserved
	UINT32 aal_coef_2_c                       : 10;  // 21:12     
} SMUX_HSC_C_SET_VAL_1_T;

/*-----------------------------------------------------------------------------
		0xc800142cL smux_hsc_c_set_val_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 aal_coef_3_c                       : 10;  //  9: 0     
	UINT32                                    : 2 ;  // 11:10     reserved
	UINT32 aal_coef_4_c                       : 10;  // 21:12     
} SMUX_HSC_C_SET_VAL_2_T;

/*-----------------------------------------------------------------------------
		0xc8001430L smux_hsc_c_set_val_3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 aal_coef_5_c                       : 10;  //  9: 0     
	UINT32                                    : 2 ;  // 11:10     reserved
	UINT32 aal_coef_6_c                       : 10;  // 21:12     
} SMUX_HSC_C_SET_VAL_3_T;

/*-----------------------------------------------------------------------------
		0xc8001434L smux_size_i ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hsize_i                            : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 vsize_i                            : 12;  // 27:16     
} SMUX_SIZE_I_T;

/*-----------------------------------------------------------------------------
		0xc8001438L smux_size_o ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hsize                              : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 vsize                              : 12;  // 27:16     
} SMUX_SIZE_O_T;

/*-----------------------------------------------------------------------------
		0xc800143cL smux_size_r ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hsize_r                            : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 vsize_r                            : 12;  // 27:16     
} SMUX_SIZE_R_T;

/*-----------------------------------------------------------------------------
		0xc8001440L smux_crop_offset ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hoffset                            : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 voffset                            : 12;  // 27:16     
} SMUX_CROP_OFFSET_T;

/*-----------------------------------------------------------------------------
		0xc8001444L smux_crop_active ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hactive                            : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 vactive                            : 12;  // 27:16     
} SMUX_CROP_ACTIVE_T;

/*-----------------------------------------------------------------------------
		0xc8001448L smux_pixel_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_data                            : 10;  //  9: 0     
	UINT32 cb_data                            : 10;  // 19:10     
	UINT32 y_data                             : 10;  // 29:20     
	UINT32                                    : 1 ;  //    30     reserved
	UINT32 reg_pixel_data_en                  : 1 ;  //    31     
} SMUX_PIXEL_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc800144cL smux_hsc_input_info ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hsc_i_hsize                        : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 hsc_i_vsize                        : 12;  // 27:16     
} SMUX_HSC_INPUT_INFO_T;

/*-----------------------------------------------------------------------------
		0xc8001450L smux_status ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 smux_ro_lcnt                       : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 smux_pic_init_w_count              : 4 ;  // 19:16     
	UINT32 smux_pic_init_r_count              : 4 ;  // 23:20     
} SMUX_STATUS_T;

/*-----------------------------------------------------------------------------
		0xc8001454L smux_status1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 smux_ro_w_lcnt                     : 12;  // 11: 0     
} SMUX_STATUS1_T;

/*-----------------------------------------------------------------------------
		0xc8001458L smux_vsc_y_set_val_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vsc_phase_offset_y                 : 9 ;  //  8: 0     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 vsc_aal_coef_0_y                   : 10;  // 21:12     
} SMUX_VSC_Y_SET_VAL_0_T;

/*-----------------------------------------------------------------------------
		0xc800145cL smux_vsc_y_set_val_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vsc_aal_coef_1_y                   : 10;  //  9: 0     
	UINT32                                    : 2 ;  // 11:10     reserved
	UINT32 vsc_aal_coef_2_y                   : 10;  // 21:12     
} SMUX_VSC_Y_SET_VAL_1_T;

/*-----------------------------------------------------------------------------
		0xc8001460L smux_vsc_y_set_val_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vsc_aal_coef_3_y                   : 10;  //  9: 0     
	UINT32                                    : 2 ;  // 11:10     reserved
	UINT32 vsc_aal_coef_4_y                   : 10;  // 21:12     
} SMUX_VSC_Y_SET_VAL_2_T;

/*-----------------------------------------------------------------------------
		0xc8001464L smux_vsc_y_set_val_3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vsc_aal_coef_5_y                   : 10;  //  9: 0     
	UINT32                                    : 2 ;  // 11:10     reserved
	UINT32 vsc_aal_coef_6_y                   : 10;  // 21:12     
} SMUX_VSC_Y_SET_VAL_3_T;

/*-----------------------------------------------------------------------------
		0xc8001468L smux_vsc_c_set_val_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vsc_phase_offset_c                 : 9 ;  //  8: 0     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 vsc_aal_coef_0_c                   : 10;  // 21:12     
} SMUX_VSC_C_SET_VAL_0_T;

/*-----------------------------------------------------------------------------
		0xc800146cL smux_vsc_c_set_val_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vsc_aal_coef_1_c                   : 10;  //  9: 0     
	UINT32                                    : 2 ;  // 11:10     reserved
	UINT32 vsc_aal_coef_2_c                   : 10;  // 21:12     
} SMUX_VSC_C_SET_VAL_1_T;

/*-----------------------------------------------------------------------------
		0xc8001470L smux_vsc_c_set_val_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vsc_aal_coef_3_c                   : 10;  //  9: 0     
	UINT32                                    : 2 ;  // 11:10     reserved
	UINT32 vsc_aal_coef_4_c                   : 10;  // 21:12     
} SMUX_VSC_C_SET_VAL_2_T;

/*-----------------------------------------------------------------------------
		0xc8001474L smux_vsc_c_set_val_3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vsc_aal_coef_5_c                   : 10;  //  9: 0     
	UINT32                                    : 2 ;  // 11:10     reserved
	UINT32 vsc_aal_coef_6_c                   : 10;  // 21:12     
} SMUX_VSC_C_SET_VAL_3_T;

/*-----------------------------------------------------------------------------
		0xc8001478L smux_vsc_y_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 denominator_y                      : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 numerator_y                        : 12;  // 27:16     
	UINT32                                    : 1 ;  //    28     reserved
	UINT32 vsc_config_y                       : 1 ;  //    29     
	UINT32 bilinear_y                         : 1 ;  //    30     
	UINT32 sampling_mode_y                    : 1 ;  //    31     
} SMUX_VSC_Y_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc800147cL smux_vsc_c_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 denominator_c                      : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 numerator_c                        : 12;  // 27:16     
	UINT32                                    : 1 ;  //    28     reserved
	UINT32 vsc_config_c                       : 1 ;  //    29     
	UINT32 bilinear_c                         : 1 ;  //    30     
	UINT32 sampling_mode_c                    : 1 ;  //    31     
} SMUX_VSC_C_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc8001480L smux_sc_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_sc_crtl                        : 32;  // 31: 0     
} SMUX_SC_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc8001484L smux_sc_dectect ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 out_scene_change_mv                : 1 ;  //     0     
	UINT32 out_scene_change                   : 1 ;  //     1     
	UINT32                                    : 21;  // 22: 2     reserved
	UINT32 out_scd                            : 8 ;  // 30:23     
	UINT32 scd_done                           : 1 ;  //    31     
} SMUX_SC_DECTECT_T;

/*-----------------------------------------------------------------------------
		0xc8001488L smux_sc_dectect1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 out_scd_23b                        : 23;  // 22: 0     
} SMUX_SC_DECTECT1_T;
typedef struct {
	SMUX_CTRL0_T                       	smux_ctrl0;                   //0xc8001400L
	SMUX_CTRL1_T                       	smux_ctrl1;                   //0xc8001404L
	SMUX_CTRL2_T                       	smux_ctrl2;                   //0xc8001408L
	SMUX_CTRL3_T                       	smux_ctrl3;                   //0xc800140cL
	SMUX_CTRLR_T                       	smux_ctrlr;                   //0xc8001410L
	SMUX_HSC_Y_SET_VAL_0_T             	smux_hsc_y_set_val_0;         //0xc8001414L
	SMUX_HSC_Y_SET_VAL_1_T             	smux_hsc_y_set_val_1;         //0xc8001418L
	SMUX_HSC_Y_SET_VAL_2_T             	smux_hsc_y_set_val_2;         //0xc800141cL
	SMUX_HSC_Y_SET_VAL_3_T             	smux_hsc_y_set_val_3;         //0xc8001420L
	SMUX_HSC_C_SET_VAL_0_T             	smux_hsc_c_set_val_0;         //0xc8001424L
	SMUX_HSC_C_SET_VAL_1_T             	smux_hsc_c_set_val_1;         //0xc8001428L
	SMUX_HSC_C_SET_VAL_2_T             	smux_hsc_c_set_val_2;         //0xc800142cL
	SMUX_HSC_C_SET_VAL_3_T             	smux_hsc_c_set_val_3;         //0xc8001430L
	SMUX_SIZE_I_T                      	smux_size_i;                  //0xc8001434L
	SMUX_SIZE_O_T                      	smux_size_o;                  //0xc8001438L
	SMUX_SIZE_R_T                      	smux_size_r;                  //0xc800143cL
	SMUX_CROP_OFFSET_T                 	smux_crop_offset;             //0xc8001440L
	SMUX_CROP_ACTIVE_T                 	smux_crop_active;             //0xc8001444L
	SMUX_PIXEL_CTRL_T                  	smux_pixel_ctrl;              //0xc8001448L
	SMUX_HSC_INPUT_INFO_T              	smux_hsc_input_info;          //0xc800144cL
	SMUX_STATUS_T                      	smux_status;                  //0xc8001450L
	SMUX_STATUS1_T                     	smux_status1;                 //0xc8001454L
	SMUX_VSC_Y_SET_VAL_0_T             	smux_vsc_y_set_val_0;         //0xc8001458L
	SMUX_VSC_Y_SET_VAL_1_T             	smux_vsc_y_set_val_1;         //0xc800145cL
	SMUX_VSC_Y_SET_VAL_2_T             	smux_vsc_y_set_val_2;         //0xc8001460L
	SMUX_VSC_Y_SET_VAL_3_T             	smux_vsc_y_set_val_3;         //0xc8001464L
	SMUX_VSC_C_SET_VAL_0_T             	smux_vsc_c_set_val_0;         //0xc8001468L
	SMUX_VSC_C_SET_VAL_1_T             	smux_vsc_c_set_val_1;         //0xc800146cL
	SMUX_VSC_C_SET_VAL_2_T             	smux_vsc_c_set_val_2;         //0xc8001470L
	SMUX_VSC_C_SET_VAL_3_T             	smux_vsc_c_set_val_3;         //0xc8001474L
	SMUX_VSC_Y_CTRL_T                  	smux_vsc_y_ctrl;              //0xc8001478L
	SMUX_VSC_C_CTRL_T                  	smux_vsc_c_ctrl;              //0xc800147cL
	SMUX_SC_CTRL_T                     	smux_sc_ctrl;                 //0xc8001480L
	SMUX_SC_DECTECT_T                  	smux_sc_dectect;              //0xc8001484L
	SMUX_SC_DECTECT1_T                 	smux_sc_dectect1;             //0xc8001488L
} DE_SMM_REG_H15A0_T;
#endif
